/* sftplist.c - FTP File */
/*lint -library */
#include <cstdio>
#include <sys/stat.h>
#ifndef WIN32
#define __NAM__ "sftplist.lnx"
#endif
#ifdef WIN32
#define __NAM__ "sftplist.exe"
#endif
#include <app.hpp>
#include <mix.hpp>
#include <sec.hpp>
#include <misc.hpp>

#include <sys/statvfs.h>
#include <cstdio>
#include <string.h>
//#include <tcpd.h>
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

/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="ARCHIVE",*workstation_ip=NULL;
char *logname=NULL,*pass_word=NULL,*docfolder=NULL;
short details=FALSE;


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
static int sftp_list_file(ssh_session session,char *remote_dir,char *remote_file)
{
	sftp_attributes file;
	char datex[22];
	char mtemp[1024];
	time_t mtime=0;
#ifdef WIN32
	struct tm *mytime=NULL;
	int x=0;
#endif
	sftp_session sftp=sftp_new(session);

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
	
	memset(datex,0,22);
	memset(mtemp,0,1024);
	if(isEMPTY(remote_dir) || !RDAstrcmp(remote_dir,"\"\""))
	{
		sprintf(mtemp,"%s",remote_file);
	} else {
		sprintf(mtemp,"%s/%s",remote_dir,remote_file);
	}
	file=sftp_stat(sftp,mtemp);
	if(file==NULL)
	{
		/* when file=NULL, an error has occured OR the directory listing is end of file */
		prterr("Error getting file info for [%s]",mtemp);
		sftp_free(sftp);
		return(1);
	}else{
	

#ifndef WIN32
#ifdef _POSIX_SOURCE
		mtime=file->mtime;
		//strftime(datex,22,"%b %d %Y %l:%M %p",localtime(&file->mtime));
		strftime(datex,22,"%b %d %Y %l:%M %p",localtime(&mtime));
#else
		TRACE;
		ascftime(datex,"%b %d %Y %l:%M %p",localtime(&file->mtime));
#endif /* _POSIX_SOURCE */
#else /* WIN32 */
		TRACE;
		mytime=localctime(&file->mtime);
		x=mytime->tm_year;
		while(x>=200) x-=100;
		sprintf(datex,"%02d/%02d/%04d  ",mytime->tm_mon+1,mytime->tm_mday,1900+my_time->tm_year);
#endif
		fprintf(stdout,"%s\t\t%llu\t\t%s\r\n",remote_file,(long long unsigned int) file->size,datex);

		if(diagapps)
		{
		prterr("\"%30s\"(%.8o) : %s(%.5d) %s(%.5d) : %.10llu bytes  %.10llu\n",
			file->name,
			file->permissions,
			file->owner,
			file->uid,
			file->group,
			file->gid,
			(long long unsigned int) file->size,
			(long long unsigned int) file->mtime);
		}
		sftp_attributes_free(file);
	}
	sftp_free(sftp);
	return(0);
}
static void do_sftp(ssh_session session)
{
	sftp_session sftp=sftp_new(session);
	sftp_dir dir;
	sftp_attributes file;
	char datex[11];
	time_t atime=0;
#ifdef WIN32
	struct tm *mytime=NULL;
	int x=0;
#endif

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
	dir=sftp_opendir(sftp,docfolder);
	if(!dir) 
	{
		prterr("Directory not opened(%s)\n", ssh_get_error(session));
		return;
	}
/* reading the whole directory, file by file */
	while((file=sftp_readdir(sftp,dir)))
	{
		if(strncmp(file->name,".",1))
		{
			if(details==FALSE)
			{
				fprintf(stdout,"%s\r\n",file->name);
			} else {
#ifndef WIN32
#ifdef _POSIX_SOURCE
				atime=file->atime;
				strftime(datex,11,"%m/%d/%Y",localtime(&atime));
#else
				ascftime(datex,"%m/%d/%Y",localtime(&file->atime));
#endif /* _POSIX_SOURCE */
#else /* WIN32 */
				mytime=localctime(&file->atime);
				x=mytime->tm_year;
				while(x>=200) x-=100;
				sprintf(datex,"%02d/%02d/%04d",mytime->tm_mon+1,mytime->tm_mday,1900+my_time->tm_year);
#endif
				fprintf(stdout,"%s\t%llu\t%s\r\n",file->name,(long long unsigned int) file->size,datex);
			}
		}
		if(diagapps)
		{
		prterr("%30s(%.8o) : %s(%.5d) %s(%.5d) : %.10llu bytes\n",
			file->name,
			file->permissions,
			file->owner,
			file->uid,
			file->group,
			file->gid,
			(long long unsigned int) file->size);
		}
		sftp_attributes_free(file);
	}
/* when file=NULL, an error has occured OR the directory listing is end of file */
	if(!sftp_dir_eof(dir))
	{
		prterr("Error: %s\n", ssh_get_error(session));
		return;
	}
	if(sftp_closedir(dir))
	{
		prterr("Error: %s\n", ssh_get_error(session));
		return;
	}
	sftp_free(sftp);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	char *env_username=NULL;
	char *env_password=NULL;
	char *env_remote_dir=NULL;
	char *env_remote_file=NULL;
	char *env_remote_server=NULL;
	char *env_temp=NULL;
	int env_remote_port=22;
	int ret=0;
  	ssh_session session;
	char bvalue=FALSE;

	if(argc<5)
	{
		env_username=RDA_GetEnv("SFTP_USERNAME");	
		env_password=RDA_GetEnv("SFTP_PASSWORD");
		env_remote_dir=RDA_GetEnv("SFTP_REMOTE_DIR");
		env_remote_file=RDA_GetEnv("SFTP_REMOTE_FILE");
		env_remote_server=RDA_GetEnv("SFTP_REMOTE_SERVER");
		env_temp=RDA_GetEnv("SFTP_PORT");
		if( (env_username==NULL) && (env_password==NULL) && (env_remote_dir==NULL) && (env_remote_file==NULL) && (env_remote_server==NULL) && (env_temp==NULL) )
		{
			fprintf(stderr,"Not enough arguments specified to continue.\n");
			return(-1);
		}else if( (env_username!=NULL) && (env_password!=NULL) && (env_remote_server!=NULL) )
		{
			pass_word=stralloc(env_password);
			initrdadiag();
			session=connect_ssh(env_remote_server,env_username,FALSE);
			if(session==NULL) return EXIT_FAILURE;
			ret=sftp_list_file(session,env_remote_dir,env_remote_file);
			ssh_disconnect(session);
			ssh_free(session);
			exit(ret);

		}
	}else{
		if(InitializeSubsystems(argc,argv,"ARCHIVE","SFTPLIST")) 
		{
			return(-1);
		}
		bvalue=UsersWorkstationValidated();
		if(!bvalue)
		{
			ShutdownSubsystems();
			return(-1);
		}

		workstation_ip=stralloc(argv[1]);
		docfolder=stralloc(argv[2]);
		logname=stralloc(argv[3]);
		pass_word=stralloc(argv[4]);
		if(argc>5) details=TRUE;
		session=connect_ssh(workstation_ip,logname,FALSE);
		if(session==NULL) return EXIT_FAILURE;
		do_sftp(session);
		ssh_disconnect(session);
		ssh_free(session);
	}
	if(docfolder!=NULL) Rfree(docfolder);
	if(workstation_ip!=NULL) Rfree(workstation_ip);
	if(pass_word!=NULL) Rfree(pass_word);
	if(logname!=NULL) Rfree(logname);
	ShutdownSubsystems();
}
