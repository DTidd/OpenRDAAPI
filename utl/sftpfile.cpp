/* sftpfile.c - FTP File */
/*lint -library */
#include <cstdio>
#include <sys/stat.h>
#ifndef WIN32
#define __NAM__ "sftpfile.lnx"
#endif
#ifdef WIN32
#define __NAM__ "sftpfile.exe"
#endif
#include <app.hpp>
#include <misc.hpp>

#include <sys/statvfs.h>
#include <cstdio>
#include <unistd.h>
#include <cerrno>
#include <fcntl.h>
#include <cstdlib>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif 

#include <libssh/libssh.h>
#include <libssh/sftp.h>

#ifdef __cplusplus
}
#endif 

/*ADDLIB misc */

char *module="ARCHIVE",*source_filename=NULL,*workstation_ip=NULL;
char *logname=NULL,*pass_word=NULL,*location=NULL,*docfolder=NULL;
char *server_filename=NULL;
short direction=0;


//int authenticate_console(ssh_session session);
//int verify_knownhost(ssh_session session);
//ssh_session connect_ssh(const char *hostname, const char *user);


#define DATALEN 65536

int authenticate_kbdint(ssh_session session, const char *password) 
{
	int err;
	const char *instruction;
        const char *name;
        int i, n;
	const char *answer;
	const char *prompt;
	char echo;

	err = ssh_userauth_kbdint(session, NULL, NULL);
	while (err == SSH_AUTH_INFO) 
	{

		name = ssh_userauth_kbdint_getname(session);
		instruction = ssh_userauth_kbdint_getinstruction(session);
		n = ssh_userauth_kbdint_getnprompts(session);

		for (i = 0; i < n; i++) 
		{
			prompt=ssh_userauth_kbdint_getprompt(session,i,&echo);
			if(prompt==NULL) break;
			if (password && strstr(prompt, "Password:")) 
			{
				answer = password;
				err = ssh_userauth_kbdint_setanswer(session, i, answer);
			}
			if (err < 0) 
			{
				return SSH_AUTH_ERROR;
			}
		}
		err=ssh_userauth_kbdint(session,NULL,NULL);
	}
	return err;
}

static void error(ssh_session session){
	fprintf(stderr,"Authentication failed: %s\n",ssh_get_error(session));
}

int authenticate_console(ssh_session session)
{
	int rc;
	int method;

  // Try to authenticate
	rc = ssh_userauth_none(session, NULL);
	if (rc == SSH_AUTH_ERROR) 
	{
		error(session);
		return rc;
	}

	method = ssh_userauth_list(session,NULL);
	while (rc != SSH_AUTH_SUCCESS) 
	{
    // Try to authenticate with public key first
		rc = ssh_userauth_autopubkey(session, NULL);
		if (rc == SSH_AUTH_ERROR) 
		{
			error(session);
			return rc;
		} else if (rc == SSH_AUTH_SUCCESS) 
		{
			break;
		}
		rc=authenticate_kbdint(session,pass_word);
		if (rc == SSH_AUTH_ERROR) 
		{
			error(session);
			return rc;
		} else if (rc == SSH_AUTH_SUCCESS) 
		{
			break;
		}
		rc = ssh_userauth_password(session, NULL, pass_word);
		if (rc == SSH_AUTH_ERROR) 
		{
			error(session);
			return rc;
		} else if (rc == SSH_AUTH_SUCCESS) 
		{
			break;
		} else if(rc == SSH_AUTH_DENIED)
		{
		} else if(rc == SSH_AUTH_PARTIAL)
		{
		} else if(rc == SSH_AUTH_INFO)
		{
		} else if(rc == SSH_AUTH_AGAIN)
		{
		}
	}
	return rc;
}

int verify_knownhost(ssh_session session){
  char *hexa;
  int state;
  unsigned char *hash = NULL;
  int hlen;

  state=ssh_is_server_known(session);

  hlen = ssh_get_pubkey_hash(session, &hash);
  if (hlen < 0) {
    return -1;
  }
  switch(state){
    case SSH_SERVER_KNOWN_OK:
      break; /* ok */
    case SSH_SERVER_KNOWN_CHANGED:
      fprintf(stderr,"Host key for server changed : server's one is now :\n");
      ssh_print_hexa("Public key hash",hash, hlen);
      ssh_clean_pubkey_hash(&hash);
      fprintf(stderr,"For security reason, connection will be stopped\n");
      return -1;
    case SSH_SERVER_FOUND_OTHER:
      fprintf(stderr,"The host key for this server was not found but an other type of key exists.\n");
      fprintf(stderr,"An attacker might change the default server key to confuse your client"
          "into thinking the key does not exist\n"
          "We advise you to rerun the client with -d or -r for more safety.\n");
      return -1;
    case SSH_SERVER_FILE_NOT_FOUND:
      fprintf(stderr,"Could not find known host file. If you accept the host key here,\n");
      fprintf(stderr,"the file will be automatically created.\n");
      /* fallback to SSH_SERVER_NOT_KNOWN behavior */
    case SSH_SERVER_NOT_KNOWN:
      hexa = ssh_get_hexa(hash, hlen);
      if (ssh_write_knownhost(session) < 0) {
        ssh_clean_pubkey_hash(&hash);
        fprintf(stderr, "error %s\n", strerror(errno));
        return -1;
      }

      break;
    case SSH_SERVER_ERROR:
      ssh_clean_pubkey_hash(&hash);
      fprintf(stderr,"%s",ssh_get_error(session));
      return -1;
  }
  ssh_clean_pubkey_hash(&hash);
  return 0;
}

ssh_session connect_ssh(const char *host, const char *user,int verbosity)
{
	ssh_session session;
	int auth=0;

	session=ssh_new();
	if(session==NULL) return(NULL); 

	if(user!=NULL)
	{
		if(ssh_options_set(session,SSH_OPTIONS_USER,user)<0) 
		{
			ssh_free(session);
			return NULL;
		}
	}
	if(ssh_options_set(session,SSH_OPTIONS_HOST,host)<0) 
	{
		ssh_free(session);
		return(NULL);
	}
	ssh_options_set(session,SSH_OPTIONS_LOG_VERBOSITY,&verbosity);
	if(ssh_connect(session))
	{
		prterr("Connection failed : %s\n",ssh_get_error(session));
		ssh_disconnect(session);
		ssh_free(session);
		return(NULL);
	}
	if(verify_knownhost(session)<0)
	{
		ssh_disconnect(session);
		ssh_free(session);
		return(NULL);
	}
	auth=authenticate_console(session);  
	if(auth==SSH_AUTH_SUCCESS)
	{
		return(session);
	} else if(auth==SSH_AUTH_DENIED)
	{
    		prterr("Error:  Authentication failed.");
  	} else {
		prterr("Error while authenticating : %s\n",ssh_get_error(session));
	}
	ssh_disconnect(session);
	ssh_free(session);
	return(NULL);
}
static int sftp_download(ssh_session session,char *remote_dir,char *remote_file,char *local_dir,char *local_file)
{
	sftp_session sftp=sftp_new(session);
	sftp_file from;
	FILE *to=NULL;
	char temp[1024];
	char buffer[4096];
	int x=0,y=0;


	if(!sftp)
	{
		prterr("sftp error initialising channel: %s\n",ssh_get_error(session));
		return(1);
	}
	if(sftp_init(sftp))
	{
        	prterr("error initializing sftp: %s\n",ssh_get_error(session));
		return(1);
	}

	if(isEMPTY(remote_file) || !RDAstrcmp(remote_file,"\"\""))
	{
        	prterr("Error:  No remote filename specified.\n");
		return(1);
	}

	memset(temp,0,1024);
	if(isEMPTY(remote_dir) || !RDAstrcmp(remote_dir,"\"\""))
	{
		sprintf(temp,"%s",remote_file);
	} else {
		sprintf(temp,"%s/%s",remote_dir,remote_file);
	}
	from=sftp_open(sftp,temp,O_RDONLY,0);
	if(from==NULL) 
	{
       		prterr("Error:  Can not open remote file [%s].\n",temp);
		return(1);
	}

	memset(temp,0,1024);
	if(isEMPTY(local_dir) || !RDAstrcmp(local_dir,"\"\""))
	{
		sprintf(temp,"%s",local_file);
	} else {
		sprintf(temp,"%s/%s",local_dir,local_file);
	}
	to=fopen(temp,"w+b");
	if(to==NULL) 
	{
       		prterr("Error:  Can not open local file [%s].\n",temp);
		return(1);
	}

	if(from!=NULL && to!=NULL)
	{
		while((x=sftp_read(from,buffer,4096))>0)
		{
			y=fwrite(buffer,1,x,to);
			if((ferror(to))!=0)
			{
				prterr("Error:  Writing %d bytes of %d.\n",y,x);
				return(2);
				break;
			}
		}
	}
	if(from!=NULL) sftp_close(from);
	if(to!=NULL) fclose(to);
	sftp_free(sftp);
	return(0);
}
static void do_sftp(ssh_session session)
{
	sftp_session sftp=sftp_new(session);
	sftp_file from;
	sftp_file to;
	char temp[1024];
	FILE *from1=NULL,*to1=NULL;
	char buffer[4096];
	int x=0,y=0;


	if(!sftp)
	{
		prterr("sftp error initialising channel: %s\n",ssh_get_error(session));
		return;
	}
	if(sftp_init(sftp))
	{
        	prterr("error initializing sftp: %s\n",ssh_get_error(session));
		return;
	}
	if(direction==FALSE) /* To Work Station */
	{
		memset(temp,0,1024);
		sprintf(temp,"%s/%s",docfolder,source_filename);
		to=sftp_open(sftp,temp,O_WRONLY|O_CREAT, 0644);
		if(to==NULL)
		{
			prterr("Error:  Cannot create file on workstation [%s] ",(temp!=NULL ? temp:""));
		}
		memset(temp,0,1024);
		if(isEMPTY(location) || !RDAstrcmp(location,"\"\""))
		{
			sprintf(temp,"%s",source_filename);
		} else {
	 		sprintf(temp,"%s/%s",location,source_filename);
		}
		from1=fopen(temp,"r+b");
		if(from1==NULL)
		{
			prterr("Error:  Cannot read file on server [%s] ",(temp!=NULL ? temp:""));
		}
		if(from1!=NULL && to!=NULL)
		{
			while(!feof(from1)) 
			{
				x=fread(buffer,1,4096,from1);
				if((ferror(from1))!=0)
				{
					prterr("Error:  Reading %d bytes of %d.\n",y,x);
					if(x>0)
					{
						if(sftp_write(to,buffer,x)!=x)
						{
							prterr("Error writing %d bytes: %s\n",x,ssh_get_error(session));
							break;
						}
					}
				}
				if(sftp_write(to,buffer,x)!=x)
				{
					prterr("Error writing %d bytes: %s\n",x,ssh_get_error(session));
					break;
				}
			}
		}
		if(from1!=NULL) fclose(from1);
		if(to!=NULL) sftp_close(to);
	} else { /* From Work Station */
		memset(temp,0,1024);
		sprintf(temp,"%s/%s",docfolder,source_filename);
		from=sftp_open(sftp,temp,O_RDONLY,0);
		memset(temp,0,1024);
		if(isEMPTY(location) || !RDAstrcmp(location,"\"\""))
		{
			sprintf(temp,"%s",(server_filename!=NULL ? server_filename:source_filename));
		} else {
			sprintf(temp,"%s/%s",location,(server_filename!=NULL ? server_filename:source_filename));
		}
		to1=fopen(temp,"w+b");
		if(from!=NULL && to1!=NULL)
		{
			while((x=sftp_read(from,buffer,4096))>0)
			{
				y=fwrite(buffer,1,x,to1);
				if((ferror(to1))!=0)
				{
					prterr("Error:  Copying %d bytes of %d.\n",y,x);
					break;
				}
			}
		}
		if(from!=NULL) sftp_close(from);
		if(to1!=NULL) fclose(to1);
	}
	sftp_free(sftp);
}
int main(int argc,char **argv)
{
	char *env_username=NULL;
	char *env_password=NULL;
	char *env_remote_dir=NULL;
	char *env_remote_file=NULL;
	char *env_local_dir=NULL;
	char *env_local_file=NULL;
	char *env_remote_server=NULL;
	char *env_temp=NULL;
	int env_remote_port=22;
	int ret=0;
  	ssh_session session;

	if(argc<7)
	{
		env_username=RDA_GetEnv("SFTP_USERNAME");	
		env_password=RDA_GetEnv("SFTP_PASSWORD");
		env_remote_dir=RDA_GetEnv("SFTP_REMOTE_DIR");
		env_remote_file=RDA_GetEnv("SFTP_REMOTE_FILE");
		env_local_dir=RDA_GetEnv("SFTP_LOCAL_DIR");
		env_local_file=RDA_GetEnv("SFTP_LOCAL_FILE");
		env_remote_server=RDA_GetEnv("SFTP_REMOTE_SERVER");
		env_temp=RDA_GetEnv("SFTP_PORT");
		if( (env_username==NULL) && (env_password==NULL) && (env_remote_dir==NULL) && (env_remote_file==NULL) && (env_local_dir==NULL) && (env_local_file==NULL) &&(env_remote_server==NULL) && (env_temp==NULL) )
		{
			fprintf(stderr,"Not enough arguments specified to continue.\n");
			return(-1);
		}else if( (env_username!=NULL) && (env_password!=NULL) && (env_remote_server!=NULL) )
		{
			pass_word=stralloc(env_password);
			initrdadiag();
			session=connect_ssh(env_remote_server,env_username,FALSE);
			if(session==NULL) return EXIT_FAILURE;
			ret=sftp_download(session,env_remote_dir,env_remote_file,env_local_dir,env_local_file);
			ssh_disconnect(session);
			ssh_free(session);
			exit(ret);

		}
	}else{
		initrdadiag();
		location=stralloc(argv[1]);
		source_filename=stralloc(argv[2]);
		workstation_ip=stralloc(argv[3]);
		docfolder=stralloc(argv[4]);
		logname=stralloc(argv[5]);
		pass_word=stralloc(argv[6]);
		if(argc>7) direction=atoi(argv[7]);
		if(argc>8) server_filename=stralloc(argv[8]);
		if(!isEMPTY(location) && !isEMPTY(source_filename))
		{
		} else if(!isEMPTY(source_filename))
		{
		} else {
			prterr("Error:  Filename given unacceptable.");
			exit(1);
		}

	session=connect_ssh(workstation_ip,logname,FALSE);
	if(session==NULL) return EXIT_FAILURE;
	do_sftp(session);
	ssh_disconnect(session);
	ssh_free(session);
	if(docfolder!=NULL) Rfree(docfolder);
	if(source_filename!=NULL) Rfree(source_filename);
	if(workstation_ip!=NULL) Rfree(workstation_ip);
	if(location!=NULL) Rfree(location);
	if(pass_word!=NULL) Rfree(pass_word);
	if(logname!=NULL) Rfree(logname);
	if(server_filename!=NULL) Rfree(server_filename);
	}
}
