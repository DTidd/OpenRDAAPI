/* dosshget.c - Program to SSH Get Programs */
/*lint -library */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */
#ifndef WIN32
#define __NAM__ "dosshget.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dosshget.exe"
#endif
#include <app.hpp>


#include <mix.hpp>
#ifdef WIN32
#include <sys/stat.h>
#endif /* ifdef WIN32 */

#ifdef __cplusplus
extern "C" {
#endif 

#include <libssh/libssh.h>
#include <libssh/sftp.h>

#ifdef __cplusplus
}
#endif 

/* User: sftpxpert Password: weibeeXahcohgieQui2e */
#define DOSSHGET_SFTP_USERNAME "sftpxpert"
#define DOSSHGET_SFTP_PASSWORD "weibeeXahcohgieQui2e"
//#define TRANSFER_BUFFER_SIZE 4096
//#define TRANSFER_BUFFER_SIZE 8192
//#define TRANSFER_BUFFER_SIZE 16384
//#define TRANSFER_BUFFER_SIZE 32768
//#define TRANSFER_BUFFER_SIZE 65536
#define TRANSFER_BUFFER_SIZE 131072


#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)
#define MODE_RWXRWX___	(S_IRWXU|S_IRWXG)
#endif /* Not LINUX && Not LINUX2_2 && Not UBUNTU_OS */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___	(00770)
#endif /* LINUX || LINUX2_2 || UBUNTU_OS*/

static char *typeString[]={"motif","gtk","qt"};
char *module="UTILITIES",*ftpsvr=NULL,*dest_path=NULL,defexe=TRUE,defshow=TRUE;
char deflib=FALSE,defpdf=FALSE,defzip=TRUE,deftgz=FALSE;
char defurl=FALSE,defoth=FALSE;
char *DEF_OS=NULL;
static char *overpass=NULL;
int ptype=0;
APPlib *ptypes=NULL,*proglist=NULL,*srcfilelist=NULL,*dstfilelist=NULL;
ssh_session session;
unsigned long current_dl_bytes=0;
unsigned long total_dl_bytes=0;



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

int authenticate_console(ssh_session session, const char *passphrase)
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
		rc=authenticate_kbdint(session,passphrase);
		if (rc == SSH_AUTH_ERROR) 
		{
			error(session);
			return rc;
		} else if (rc == SSH_AUTH_SUCCESS) 
		{
			break;
		}
		rc = ssh_userauth_password(session, NULL, passphrase);
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

ssh_session connect_ssh(const char *host, int port, const char *user, const char *passphrase, int verbosity)
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
	auth=authenticate_console(session,passphrase);  
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
int new_persistent_sftp_session(ssh_session *ssh_session, sftp_session *sftp)
{
	*sftp=sftp_new(*ssh_session);
	if(!*sftp)
	{
		prterr("sftp error initialising channel: %s\n",ssh_get_error(*ssh_session));
		return(1);
	}
	if(sftp_init(*sftp))
	{
      	 	prterr("error initializing sftp: %s\n",ssh_get_error(*ssh_session));
		return(1);
	}
	return(0);
}
static int sftp_list_file(ssh_session session, sftp_session sftp, sftp_attributes fileinfo, char *remote_dir,char *remote_file, char **fileinfostr)
{
	sftp_attributes file;
	sftp_session mysftp;
	char datex[22];
	char sizetemp[15];
	char mtemp[1024];
	time_t mtime=0;
#ifdef WIN32
	struct tm *mytime=NULL;
	int x=0;
#endif
	
	if(sftp!=NULL)
	{
		if(diagmisc) { prterr("Reusing sftp session."); }
		mysftp=sftp;
	}else{
		if(diagmisc) { prterr("Creating new sftp session."); }
		mysftp=sftp_new(session);

		if(!mysftp)
		{
			prterr("sftp error initialising channel: %s\n",ssh_get_error(session));
			return(1);
		}
		if(sftp_init(mysftp))
		{
       	 	prterr("error initializing sftp: %s\n",ssh_get_error(session));
			return(1);
		}
	}
	
	memset(datex,0,22);
	memset(mtemp,0,1024);
	if(isEMPTY(remote_dir) || !RDAstrcmp(remote_dir,"\"\""))
	{
		sprintf(mtemp,"%s",remote_file);
	} else {
		sprintf(mtemp,"%s/%s",remote_dir,remote_file);
	}
	file=sftp_stat(mysftp,mtemp);
	if(file==NULL)
	{
		/* when file=NULL, an error has occured OR the directory listing is end of file */
		prterr("Error getting file info for [%s]",mtemp);
		if(sftp==NULL)
		{
			sftp_free(mysftp);
		}
		return(1);
	}else{
	

#ifndef WIN32
#ifdef _POSIX_SOURCE
		mtime=file->mtime;
		strftime(datex,22,"%b %d %Y %l:%M %p",localtime(&mtime));
#else
		ascftime(datex,"%b %d %Y %l:%M %p",localtime(&file->mtime));
#endif /* _POSIX_SOURCE */
#else /* WIN32 */
		mytime=localctime(&file->mtime);
		x=mytime->tm_year;
		while(x>=200) x-=100;
		sprintf(datex,"%02d/%02d/%04d  ",mytime->tm_mon+1,mytime->tm_mday,1900+my_time->tm_year);
#endif

		*fileinfostr=Rmalloc(1024); 
		if(*fileinfostr!=NULL)
		{
			/* Atempting to center size in display string caused bad widgetds.  
			 * Need to review sizing of ints etc and try again.
			memset(sizetemp,0,15);
			sprintf(sizetemp,"%llu",(long long unsigned int) file->size);
			sprintf(*fileinfostr,"%-30s\t\t%15s\t\t%-25s",remote_file,sizetemp,datex);
			*/
			sprintf(*fileinfostr,"%-30s\t\t%llu\t\t%-25s",remote_file, file->size,datex);
			total_dl_bytes=(total_dl_bytes+ file->size);
		}

		if(diagapps)
		{
		prterr("\"%30s\"(%.8o) : %s(%.5d) %s(%.5d) : %.10llu bytes  %.10llu\n",
			file->name,
			file->permissions,
			file->owner,
			file->uid,
			file->group,
			file->gid,
			 file->size,
			 file->mtime);
		}
		sftp_attributes_free(file);
	}
	if(sftp==NULL)
	{
		sftp_free(mysftp);
	}
	return(0);
}
static int sftp_download(ssh_session session,sftp_session psftp,char *remote_dir,char *remote_file,char *local_dir,char *local_file, RDArsrc *diagrsrc)
{
	sftp_session sftp;
	sftp_file from;
	sftp_attributes file;
	FILE *to=NULL;
	char temp[1024];
	char buffer[TRANSFER_BUFFER_SIZE];
	int x=0,y=0;
	int z=0;


	if(psftp!=NULL)
	{
		if(diagmisc) { prterr("Reusing sftp session."); }
		sftp=psftp;
	}else{
		if(diagmisc) { prterr("Creating new sftp session."); }
		sftp=sftp_new(session);

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
	file=sftp_stat(sftp,temp);



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
		while((x=sftp_read(from,buffer,TRANSFER_BUFFER_SIZE))>0)
		{
			y=fwrite(buffer,1,x,to);
			if((ferror(to))!=0)
			{
				prterr("Error:  Writing %d bytes of %d.\n",y,x);
				return(2);
				break;
			}
			
			current_dl_bytes=(current_dl_bytes+x);
			if((file!=NULL)&&(diagrsrc!=NULL))
			{
			   	
				for(z=0;z<(x/1024);++z)
				{
					update_diagnostic(diagrsrc,1);
				}
				/*
				ForceWindowUpdate(diagrsrc);
				prterr("Read %u bytes of %u    Current %u of %u total bytes.\n",(sftp_tell(from)/1024),((long long unsigned int) file->size/1024), (current_dl_bytes/1024),(total_dl_bytes/1024));
				*/
/*
*/
			}
		}
	}
	if(from!=NULL) sftp_close(from);
	if(to!=NULL) fclose(to);
	if(psftp==NULL)	sftp_free(sftp);
	return(0);
}


#ifdef WIN32
static char DID_STUFF=FALSE;
BOOL MySystemShutdown( LPTSTR lpMsg )
{
   HANDLE hToken;              // handle to process token 
   TOKEN_PRIVILEGES tkp;       // pointer to token structure 
 
   BOOL fResult;               // system shutdown flag 
 
   // Get the current process token handle so we can get shutdown 
   // privilege. 
 
   if (!OpenProcessToken(GetCurrentProcess(), 
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
      return FALSE; 
 
   // Get the LUID for shutdown privilege. 
 
   LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, 
        &tkp.Privileges[0].Luid); 
 
   tkp.PrivilegeCount = 1;  // one privilege to set    
   tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
 
   // Get shutdown privilege for this process. 
 
   AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
      (PTOKEN_PRIVILEGES) NULL, 0); 
 
   // Cannot test the return value of AdjustTokenPrivileges. 
 
   if (GetLastError() != ERROR_SUCCESS) 
      return FALSE; 
 
   // Display the shutdown dialog box and start the countdown. 
 
   fResult = InitiateSystemShutdown( 
      NULL,    // shut down local computer 
      lpMsg,   // message for user
      20,      // time-out period 
      FALSE,   // ask user to close apps 
      TRUE);   // reboot after shutdown 
 
   if (!fResult) 
      return FALSE; 
 
   // Disable shutdown privilege. 
 
   tkp.Privileges[0].Attributes = 0; 
   AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
        (PTOKEN_PRIVILEGES) NULL, 0); 
 
   return TRUE; 
}
/* 
If the AbortSystemShutdown function is executed in the time-out period specified by InitiateSystemShutdown, the system does not shut down.
*/
BOOL PreventSystemShutdown()
{
   HANDLE hToken;              // handle to process token 
   TOKEN_PRIVILEGES tkp;       // pointer to token structure 
 
   // Get the current process token handle  so we can get shutdown 
   // privilege. 
 
   if (!OpenProcessToken(GetCurrentProcess(), 
         TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
      return FALSE; 
 
   // Get the LUID for shutdown privilege. 
 
   LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, 
         &tkp.Privileges[0].Luid); 
 
   tkp.PrivilegeCount = 1;  // one privilege to set    
   tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
 
   // Get shutdown privilege for this process. 
 
   AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
        (PTOKEN_PRIVILEGES)NULL, 0); 
 
   if (GetLastError() != ERROR_SUCCESS) 
      return FALSE; 
 
   // Prevent the system from shutting down. 
 
   if ( !AbortSystemShutdown(NULL) ) 
      return FALSE; 
 
   // Disable shutdown privilege. 
 
   tkp.Privileges[0].Attributes = 0; 
   AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
       (PTOKEN_PRIVILEGES) NULL, 0); 
 
   return TRUE;
}
#endif /* WIN32 */
static void quitfunc(RDArsrc *r)
{
	ssh_disconnect(session);
	ssh_free(session);
	if(r!=NULL) free_rsrc(r);
	if(ftpsvr!=NULL) Rfree(ftpsvr);
	if(ptypes!=NULL) freeapplib(ptypes);
	if(proglist!=NULL) freeapplib(proglist);
	if(srcfilelist!=NULL) freeapplib(srcfilelist);
	if(dstfilelist!=NULL) freeapplib(dstfilelist);
	if(dest_path!=NULL) Rfree(dest_path);
	ShutdownSubsystems();
#ifdef WIN32
	if(DID_STUFF)
	{
		MySystemShutdown("Please close all running applications, your machine will reboot in 20 seconds.....");
#ifdef WIN32_DONT_COMPILE
		/* This is the start to using the UAC on Server 2008, Vista and higher....   */
		if(reboot)
		{
			system_command("shutdown -r -f -t 20 -c \"Please close all running applications, your machine will reboot in 20 seconds.....\"");	
		}
#endif
	}
#endif /* WIN32 */
}
static void dosshget(RDArsrc *r)
{
	int x=0;
	/* char *t=NULL */
	char flag=FALSE,*nam=NULL,*temp=NULL,*namx=NULL;
	RDArsrc *tmprsrc=NULL;
	APPlib *args=NULL;
	int runinstdeps=0;
	char instdepcmd[200];
	sftp_session download_session=NULL;

#ifdef WIN32
	short y=0;
	char *tmp=NULL,*tempdir=NULL,*t1=NULL,*t2=NULL;
	int j=0,k=0;
	char stemp1[101],stemp2[101];
#endif

	readallwidgets(r);
	FINDRSCGETSTRING(r,"DEFAULT OPERATING SYSTEM",&DEF_OS);
	if(r!=NULL) killwindow(r);

	tmprsrc=odiagscrn("DIAGNOSTIC SCREEN","UTILITIES","Obtaining Files using SFTP",stop_diagnostic,(total_dl_bytes/1024));
	if(tmprsrc!=NULL)
	{
		tmprsrc->scn=RDAscrnNEW(module,"DIAGNOSTIC SCREEN");
		ADVaddwdgt(tmprsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,0,"PROCESS NAME","Process Name","","",0,40,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,0,"STATUS","Status","","",0,40,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,2,"","","","",0,0,0,"","","","");
#ifdef USING_QT
		ADVaddwdgt(tmprsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(tmprsrc->scn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(tmprsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(tmprsrc->scn,22,"PROGRESS BAR","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(tmprsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(tmprsrc->scn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(tmprsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
#endif /* USING_QT */
		ADVaddwdgt(tmprsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(tmprsrc->scn,5,"","Selected","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(tmprsrc->scn,5,"","Total Read","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(tmprsrc->scn,5,"","Total All","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,0,"SELECTED","Selected","","",0,8,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,0,"TOTAL READ","Total Read","","",0,8,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,0,"TOTAL ALL","Total All","","",0,8,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(tmprsrc->scn,5,"","Start Time","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(tmprsrc->scn,5,"","Current","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(tmprsrc->scn,5,"","Elapsed","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,0,"START TIME","Start Time","","",0,8,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,0,"CURRENT TIME","Current Time","","",0,8,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,0,"ELAPSED TIME","Elapsed Time","","",0,8,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,5,"","Increment in:","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,0,"INCREMENT","Increment","","",0,3,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,5,"","seconds minimums","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,6,"SUSPEND","Suspend","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,6,"RESUME","Resume","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,6,"QUIT","Cancel","","",0,0,14,"","","","");
		ADVaddwdgt(tmprsrc->scn,6,"HELP","Help","","",0,0,11,"","","","");
		ADVaddwdgt(tmprsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,2,"","","","",0,0,0,"","","","");
		if(!APPmakescrn(tmprsrc,FALSE,NULL,NULL,FALSE)) ForceWindowUpdate(tmprsrc);
	}
	memset(stemp,0,101);
	sprintf(stemp,"%d",1);
	FINDRSCSETSTRING(tmprsrc,"INCREMENT",stemp);
	
	memset(stemp,0,101);
	sprintf(stemp,"%uld",(total_dl_bytes/1024));
	FINDRSCSETSTRING(tmprsrc,"TOTAL ALL",stemp);
	ForceWindowUpdate(tmprsrc);

	new_persistent_sftp_session(&session,&download_session);
	for(x=0;x<proglist->numlibs;++x)
	{
		checkforinterrupt(tmprsrc);
		if(abort_diagnostic==TRUE) break;
		FINDRSCGETCHAR(r,proglist->libs[x],&flag);
		if(flag==TRUE)
		{			
/*
			nam=Rmalloc(RDAstrlen(ftpsvr)+RDAstrlen(proglist->libs[x])+512);
			temp=stralloc(proglist->libs[x]);
			t=strchr(temp,' ');
			if(t!=NULL) *t=0;

*/
			/* prterr("src [%s]   dst [%s]",srcfilelist->libs[x],dstfilelist->libs[x]); */
			sftp_download(session,download_session,NULL,srcfilelist->libs[x],NULL,dstfilelist->libs[x],tmprsrc);


			if(temp!=NULL) Rfree(temp);

			if(( (!strncmp(&dstfilelist->libs[x][RDAstrlen(dstfilelist->libs[x])-3],".7z",3)) || (!strncmp(&dstfilelist->libs[x][RDAstrlen(dstfilelist->libs[x])-4],".zip",4))) && defexe==TRUE)
			{
#ifndef WIN32
				if(!chdir(dest_path))
				{
					args=APPlibNEW();
					addAPPlib(args,"x");
					addAPPlib(args,"-aoa");
					addAPPlib(args,dstfilelist->libs[x]);
					ADVExecute2Program("7za",args,NULL);
					if(args!=NULL) freeapplib(args);
					unlink(dstfilelist->libs[x]);

#ifndef UBUNTU_OS
					if((XPERT_SETUP->ASP!=TRUE)&&(!strncmp(proglist->libs[x],"UTILITIES",9)))
					{
						runinstdeps=1;
					}
#endif

					chdir(CURRENTDIRECTORY);
				} else {
					prterr("Error:  Unable to change directory to Destination Path [%s], didn't unpack or remove zip [%s].",(dest_path!=NULL ? dest_path:""),dstfilelist->libs[x]);
				}
#endif
#ifdef WIN32
				if(!strncmp(dest_path+1,":",1)) 
				{
					memset(stemp,0,101);
					memset(stemp1,0,101);
					memset(stemp2,0,101);
					strncpy(stemp2,dest_path,1);
					sprintf(stemp,"%s:\\rda",stemp2);
					if(!mkdir(stemp)) 
					{
						prterr("Could Not Create Directory [%s]i, It May Already Exist.\n",stemp);
					}
					sprintf(stemp,"%s:\\rda\\updates",stemp2);
					if(!mkdir(stemp)) 
					{
						prterr("Could Not Create Directory [%s]i, It May Already Exist.\n",stemp);
					}
					
					k=strlen(dest_path);
					for(j=k;j>0;j--)
					{
						strncpy(stemp1,dest_path+j,1);
						if(!strncmp(stemp1,"\\",1)) 
						{
							j++;
							strncpy(stemp1,dest_path+(j),(k-j));
							break;
						}
					}
					sprintf(stemp,"%s:\\rda\\updates\\%s",stemp2,stemp1);
					prterr("Temp Dir [%s]\n",stemp);
					if(!mkdir(stemp)) 
					{
						prterr("Could Not Create Directory [%s]i, It May Already Exist.\n",stemp);
					}
					sprintf(stemp,"-o%s:\\rda\\updates\\%s",stemp2,stemp1);
				}else{
					memset(stemp,0,101);
					mkdir("C:\\TEMP\\xpgms");
					sprintf(stemp,"-oC:\\TEMP\\xpgms");
					prterr("Temp Dir [C:\\TEMP\\xpgms]\n");
				}
		
				args=APPlibNEW();
				addAPPlib(args,"x");
				addAPPlib(args,"-aoa");
				addAPPlib(args,stemp);
				addAPPlib(args,tempx);
				ADVExecute2Program("7zg",args,NULL);
				if(args!=NULL) freeapplib(args);
				unlink(tempx);
#endif
			}
#ifndef WIN32	
#ifndef UBUNTU_OS
			if(runinstdeps) {
				memset(instdepcmd,0,200);
				sprintf(instdepcmd,"/usr/bin/X11/xterm -ls -T \"INSTALL DEPENDENCIES\" -n \"INSTALL DEPENDENCIES\" -e %s/install_qt_deps.lnx",dest_path);
				system_command(instdepcmd);
			}
#endif
#endif
			if(namx!=NULL) Rfree(namx);
			if(nam!=NULL) Rfree(nam);
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
		}
	}
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);		
	}
#ifdef WIN32
	memset(stemp,0,101); 
	if(!strncmp(dest_path+1,":",1)) 
	{
		sprintf(stemp,"%s:\\rda\\updates\\%s\\",stemp2,stemp1);
	}else{
		sprintf(stemp,"C:\\TEMP\\xpgms\\");
	}
	for(x=0;findfile(stemp,"*",&tmp,(int)x+1);++x)
	{
		t1=Rmalloc(RDAstrlen(stemp)+RDAstrlen(tmp)+10);
		sprintf(t1,"%s%s",stemp,tmp);
		t2=Rmalloc(RDAstrlen(dest_path)+RDAstrlen(tmp)+10);
		sprintf(t2,"%s\\%s",(dest_path!=NULL ? dest_path:""),tmp);
		 /* Change back to read/write: */
		if(_chmod(t2,_S_IWRITE )==-1)
		{
			prterr( "File not found: [%s]\n",t2);
		} else {
			prterr( "Mode changed to read/write: [%s]\n",t2);
		}

		if((y=MoveFileEx(t2,NULL,MOVEFILE_DELAY_UNTIL_REBOOT))==0)
		{
			prterr("Error:  MoveFileEx Failed with [%d] [%s] attempting Source [%s] Destination [%s] at line [%d] program [%s].",y,GetLastError(y),(t1!=NULL ? t1:""),(t2!=NULL ? t2:""),__LINE__,__FILE__); 
		}		
		if((y=MoveFileEx(t1,t2,MOVEFILE_DELAY_UNTIL_REBOOT))==0)
		{
			prterr("Error:  MoveFileEx Failed with [%d] [%s] attempting Source [%s] Destination [%s] at line [%d] program [%s].",y,GetLastError(y),(t1!=NULL ? t1:""),(t2!=NULL ? t2:""),__LINE__,__FILE__); 
		}		

/* Should probably add a finish dialog that lists any failed downloads here. */

		if(t1!=NULL) Rfree(t1);
		if(t2!=NULL) Rfree(t2);
		DID_STUFF=TRUE;
	}
	if(tmp!=NULL) Rfree(tmp);
	sftp_free(download_session);
#endif
	quitfunc(r);
}
static void ContentstoAPPlib(APPlib *proglist,char *dirx,RDArsrc *tmprsrc,ssh_session *mysession, sftp_session *mysftpsession)
{
	char mtemp1[1024];
	char mtemp2[1024];
	char *mtemp3=NULL;


	memset(mtemp1,0,1024);
	memset(mtemp2,0,1024);
	// Add check for empty/null on dirx variable
	if(XPERT_SETUP->software_type<2)
	{
		sprintf(mtemp1,"/%s/%s",typeString[ptype],DEF_OS);
		sprintf(mtemp2,"%s/%s.7z",dirx,dirx);
	} else {
		sprintf(mtemp1,"/%s/%s",typeString[ptype],DEF_OS);
		sprintf(mtemp2,"%s/%s.7z",dirx,dirx);
	}
	
	
	sftp_list_file(session, *mysftpsession , NULL, mtemp1, mtemp2, &mtemp3);
	if(mtemp3!=NULL)
	{
		addAPPlib(proglist,mtemp3);

		memset(stemp,0,101);
		sprintf(stemp,"%s/%s.7z",dest_path,dirx);
		addAPPlib(dstfilelist,stemp);

		memset(stemp,0,101);
		sprintf(stemp,"%s/%s",mtemp1,mtemp2);
		addAPPlib(srcfilelist,stemp);
		
	}
	if(mtemp3!=NULL) Rfree(mtemp3);
	
}
static int ftpget(RDArsrc *r)
{
/*
	char *temp=NULL,*t=NULL;
	selected=0;
*/
	int x=0;
	RDArsrc *editrsrc=NULL;
	RDArsrc *tmprsrc=NULL;
#ifdef WIN32_DONT_COMPILE
	int reboot=0;
#endif
	sftp_session list_session=NULL;

	readwidget(r,"DEFAULT OPERATING SYSTEM");
	FINDRSCGETSTRING(r,"DEFAULT OPERATING SYSTEM",&DEF_OS);
	readwidget(r,"SSH SERVER");
	FINDRSCGETSTRING(r,"SSH SERVER",&ftpsvr);
	readwidget(r,"DESTINATION PATH");
	FINDRSCGETSTRING(r,"DESTINATION PATH",&dest_path);
	readwidget(r,"UNPACK ZIPS");
	FINDRSCGETCHAR(r,"UNPACK ZIPS",&defexe);
	readwidget(r,"OVERRIDE PASSWORD");
	FINDRSCGETSTRING(r,"OVERRIDE PASSWORD",&overpass);
	FINDRSCGETINT(r,"TYPES",&ptype);
#ifdef WIN32_DONT_COMPILE
	readwidget(r,"REBOOT WHEN FINISHED",&reboot);
	FINDRSCGETCHAR(r,"REBOOT WHEN FINISHED",&reboot);
#endif

	if(overpass!=NULL)
	{
		if(!isEMPTY(overpass))
		{
			session=connect_ssh(ftpsvr,22,DOSSHGET_SFTP_USERNAME,overpass,FALSE);
			if(session==NULL) return EXIT_FAILURE;
		} else {
		}
	} else {
		session=connect_ssh(ftpsvr,22,DOSSHGET_SFTP_USERNAME,DOSSHGET_SFTP_PASSWORD,FALSE);
		if(session==NULL) return EXIT_FAILURE;
	}

	new_persistent_sftp_session(&session,&list_session);

	if(proglist!=NULL) freeapplib(proglist);
	if(srcfilelist!=NULL) freeapplib(srcfilelist);
	if(dstfilelist!=NULL) freeapplib(dstfilelist);
	proglist=APPlibNEW();
	srcfilelist=APPlibNEW();
	dstfilelist=APPlibNEW();
	if(XPERT_SETUP->software_type<2)
	{
		if(XPERT_SETUP->UTILITIES) ContentstoAPPlib(proglist,"UTILITIES",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->FINMGT) ContentstoAPPlib(proglist,"FINMGT",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->BNKREC) ContentstoAPPlib(proglist,"BNKREC",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->PRSNNL) ContentstoAPPlib(proglist,"PRSNNL",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->HRM) ContentstoAPPlib(proglist,"OPENSS",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->IIDMGT) ContentstoAPPlib(proglist,"IIDMGT",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->APPMGT) ContentstoAPPlib(proglist,"APPMGT",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->POSTRK) ContentstoAPPlib(proglist,"POSTRK",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->PAYROLL) ContentstoAPPlib(proglist,"PAYROLL",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->SUBMGT) ContentstoAPPlib(proglist,"SUBMGT",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->LVEMGT) ContentstoAPPlib(proglist,"LVEMGT",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->EMPABS) ContentstoAPPlib(proglist,"EMPABS",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->BFTMGT) ContentstoAPPlib(proglist,"BFTMGT",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->VENPMT) ContentstoAPPlib(proglist,"VENPMT",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->PURORD) ContentstoAPPlib(proglist,"PURORD",tmprsrc,&session,&list_session);
/*
		if(XPERT_SETUP->CATALOGUE) ContentstoAPPlib(proglist,"CATALOGUE",tmprsrc,&session,&list_session);
*/
		if(XPERT_SETUP->REQMGT) ContentstoAPPlib(proglist,"REQMGT",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->FIXASS) ContentstoAPPlib(proglist,"FIXASS",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->INVCTL) ContentstoAPPlib(proglist,"INVCTL",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->OCCTAX) ContentstoAPPlib(proglist,"OCCTAX",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->BUDPREP) ContentstoAPPlib(proglist,"BUDPREP",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->MISCBILL) ContentstoAPPlib(proglist,"MISCBILL",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->TAXCLT) ContentstoAPPlib(proglist,"TAXCLT",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->RLSTMGT) ContentstoAPPlib(proglist,"RLSTMGT",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->PPTMGT) ContentstoAPPlib(proglist,"PPTMGT",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->STUDFIN) ContentstoAPPlib(proglist,"STUDFIN",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->TOOLS) ContentstoAPPlib(proglist,"TOOLS",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->PROPERTY) ContentstoAPPlib(proglist,"PROPERTY",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->UTLBLL) ContentstoAPPlib(proglist,"UTLBLL",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->DMVREG) ContentstoAPPlib(proglist,"DMVREG",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->LVSIMP) ContentstoAPPlib(proglist,"LVSIMP",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->CSHDWR) ContentstoAPPlib(proglist,"CSHDWR",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->BLDPRMT) ContentstoAPPlib(proglist,"BLDPRMT",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->ARCHIVE) ContentstoAPPlib(proglist,"ARCHIVE",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->CITWEB) ContentstoAPPlib(proglist,"CITWEB",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->OPENSOURCE) ContentstoAPPlib(proglist,"OPENSOURCE",tmprsrc,&session,&list_session);
	} else {
		if(XPERT_SETUP->UTILITIES) ContentstoAPPlib(proglist,"UTILITIES",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->FINMGT) ContentstoAPPlib(proglist,"FINMGT",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->BNKREC) ContentstoAPPlib(proglist,"BNKREC",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->PRSNNL) ContentstoAPPlib(proglist,"PRSNNL",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->PAYROLL) ContentstoAPPlib(proglist,"PAYROLL",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->LVEMGT) ContentstoAPPlib(proglist,"LVEMGT",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->VENPMT) ContentstoAPPlib(proglist,"VENPMT",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->PURORD) ContentstoAPPlib(proglist,"PURORD",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->REQMGT) ContentstoAPPlib(proglist,"REQMGT",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->FIXASS) ContentstoAPPlib(proglist,"FIXASS",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->OCCTAX) ContentstoAPPlib(proglist,"OCCTAX",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->BUDPREP) ContentstoAPPlib(proglist,"BUDPREP",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->MISCBILL) ContentstoAPPlib(proglist,"MISCBILL",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->RLSTMGT) ContentstoAPPlib(proglist,"RLSTMGT",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->STUDFIN) ContentstoAPPlib(proglist,"STUDFIN",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->TOOLS) ContentstoAPPlib(proglist,"TOOLS",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->PROPERTY) ContentstoAPPlib(proglist,"PROPERTY",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->RLVEMGT) ContentstoAPPlib(proglist,"RLVEMGT",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->UTLBLL) ContentstoAPPlib(proglist,"UTLBLL",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->DMVREG) ContentstoAPPlib(proglist,"DMVREG",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->CSHDWR) ContentstoAPPlib(proglist,"CSHDWR",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->RREQMGT) ContentstoAPPlib(proglist,"RREQMGT",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->BLDPRMT) ContentstoAPPlib(proglist,"BLDPRMT",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->ARCHIVE) ContentstoAPPlib(proglist,"ARCHIVE",tmprsrc,&session,&list_session);
		if(XPERT_SETUP->OPENSOURCE) ContentstoAPPlib(proglist,"OPENSOURCE",tmprsrc,&session,&list_session);
	}
	sftp_free(list_session);
	SORTAPPlib(proglist);
	SORTAPPlib(srcfilelist);
	SORTAPPlib(dstfilelist);

	if(proglist->numlibs>0)
	{
		editrsrc=RDArsrcNEW("UTILITIES","SSH PROGRAM LIST");
		for(x=0;x<proglist->numlibs;++x)
		{
			addstdrsrc(editrsrc,proglist->libs[x],BOOLNS,1,&defshow,TRUE);
		}
		addrfexrsrc(editrsrc,"SELECT",TRUE,dosshget,NULL);
		addrfexrsrc(editrsrc,"QUIT",TRUE,quitfunc,NULL);
		addrfcbrsrc(editrsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(editrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		editrsrc->scn=RDAscrnNEW(module,"SSH PROGRAMS");
		ADVaddwdgt(editrsrc->scn,12,"","","","",400,600,0,"","","","");
		for(x=0;x<proglist->numlibs;++x)
		{
			ADVaddwdgt(editrsrc->scn,1,"","","","",0,0,0,"","","","");
			ADVaddwdgt(editrsrc->scn,9,proglist->libs[x],proglist->libs[x],"","",0,0,0,"","","","");
			ADVaddwdgt(editrsrc->scn,2,"","","","",0,0,0,"","","","");
		}
		ADVaddwdgt(editrsrc->scn,13,"","","","",0,0,0,"","","","");
		ADVaddwdgt(editrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(editrsrc->scn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(editrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(editrsrc->scn,6,"SELECT","Select","","",0,0,23,"","","","");
		ADVaddwdgt(editrsrc->scn,6,"QUIT","Cancel","","",0,0,14,"","","","");
		ADVaddwdgt(editrsrc->scn,6,"HELP","Help","","",0,0,11,"","","","");
		ADVaddwdgt(editrsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(editrsrc->scn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(editrsrc->scn,2,"","","","",0,0,0,"","","","");
		APPmakescrn(editrsrc,TRUE,quitfunc,NULL,TRUE);
	} else {
		prterr("Error:  No progams available.");
	}
}
#define ORIG_THOUGHT

#ifdef WIN32
#ifdef USE_ADMIN_USER
static BOOL _checkTokenMembership(HANDLE htok,void* psid,BOOL* pbIsMember) 
{ 
	DWORD cb; 
	HANDLE hheap;
 	TOKEN_GROUPS *ptg;
	BOOL bOk = FALSE; 
	SID_AND_ATTRIBUTES *end,*it;

	if(!pbIsMember) return FALSE; 
	if(!IsValidSid(psid)) return FALSE; 
	if(GetTokenInformation(htok,TokenGroups,0,0,&cb)) return FALSE; 
	if(ERROR_INSUFFICIENT_BUFFER!=GetLastError()) return FALSE; 
	hheap = GetProcessHeap(); 
	if(!hheap) return FALSE; 
	if(GetTokenInformation( htok, TokenGroups, ptg, cb, &cb ) ) 
	{ 
		*pbIsMember = FALSE; 
		end = ptg->Groups + ptg->GroupCount; 
		for ((SID_AND_ATTRIBUTES*)it = ptg->Groups; it != end; ++it ) 
		{ 
			if(EqualSid(it->Sid,psid)) 
			{ 
				*pbIsMember = TRUE; 
				break; 
			} 
		} 
		bOk = TRUE; 
	} 
	HeapFree( hheap, 0, ptg ); 
	return bOk; 
}
static BOOL IsAdministrator(HANDLE htok) 
{ 
	SID_IDENTIFIER_AUTHORITY ntauth = SECURITY_NT_AUTHORITY; 
	void* psid = 0; 
	BOOL bIsMember = FALSE; 
	BOOL bIsAdmin=FALSE;
	BOOL bOk;

	bOk = AllocateAndInitializeSid( &ntauth, 2,
		SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0, &psid ); 
	if(bOk) 
	{ 
		bIsAdmin=_checkTokenMembership(htok,psid,&bIsMember) 					&& bIsMember; 
		FreeSid( psid ); 
		return bIsAdmin; 
	} 
	return false; 
} 
short AmIAdministrator() 
{ 
	HANDLE hToken;              // handle to process token 
	short a=FALSE;

	if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&hToken)) 
	{ 
		return(a);
	} 
	if(IsAdministrator(hToken)) a=TRUE;
	CloseHandle(hToken);
	return(a);
}
#endif /* USE_ADMIN_USER */
#endif /* WIN32 */
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	char *temp=NULL;

	if(InitializeSubsystems(argc,argv,module,"SSH PROGRAMS")) 
	{
		return;
	}
#ifdef WIN32
#ifdef USE_MYISAM
	DEF_OS=stralloc("winxp_mysql");
#else
	DEF_OS=stralloc("win32");
#endif
#endif
#ifndef WIN32
#ifdef USE_MYISAM
#ifdef RDA_64BITS
	DEF_OS=stralloc("lnx_mysql64");
#else
	DEF_OS=stralloc("lnx_mysql");
#endif
#else
	DEF_OS=stralloc("lnx");
#endif
#endif
#ifdef WIN32
#ifdef USE_ADMIN_USER
	if(!AmIAdministrator())
	{
		ERRORDIALOG("NON-ADMINISTRATIVE USER","Permissions preclude you from doing Administrative Tasks.  You must either log in as the administrator or obtain administrative permissions for dosshget.exe.",NULL,TRUE);
		return;
	}
#endif /* USE_ADMIN_USER */
#endif /* WIN32 */
	mainrsrc=RDArsrcNEW(module,"SSH PROGRAMS");
	ftpsvr=stralloc("updates.openrda.com");
	addstdrsrc(mainrsrc,"SSH SERVER",VARIABLETEXT,0,ftpsvr,TRUE);
	addstdrsrc(mainrsrc,"DEFAULT OPERATING SYSTEM",VARIABLETEXT,0,DEF_OS,TRUE);
	addstdrsrc(mainrsrc,"SSH SERVER",VARIABLETEXT,0,ftpsvr,TRUE);
#ifdef ORIG_THOUGHT
#ifdef WIN32
	dest_path=stralloc(CURRENTDIRECTORY);
#endif
#ifndef WIN32
	if(XPERT_SETUP->ASP==TRUE)
	{
		temp=RDA_GetEnv("GROUP");
		if(!isEMPTY(temp)) sprintf(stemp,"/hosted/%s/xpgms",temp);
			else sprintf(stemp,"%s/../xpgms",CURRENTDIRECTORY);
		dest_path=stralloc(stemp);
	} else {
		dest_path=stralloc("/rda/xpgms");
	}
#endif
#endif
#ifndef ORIG_THOUGHT
	dest_path=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+25);
#ifdef WIN32
	sprintf(dest_path,"%s\\DOWNLOADS",CURRENTDIRECTORY);
#endif
#ifndef WIN32
	sprintf(dest_path,"%s/DOWNLOADS",CURRENTDIRECTORY);
#endif
#endif
	addstdrsrc(mainrsrc,"DESTINATION PATH",VARIABLETEXT,0,dest_path,TRUE);
	ptypes=APPlibNEW();
	addAPPlib(ptypes,"Motif");
	addAPPlib(ptypes,"Gtk");
	addAPPlib(ptypes,"Qt");
#ifdef WIN32
#ifdef USING_QT
	ptype=2;
#else
	ptype=1;
#endif
#endif 
#ifndef WIN32
#ifdef USING_QT
	ptype=2;
#else
	ptype=0;
#endif
#endif
	addlstrsrc(mainrsrc,"TYPES",&ptype,TRUE,NULL,ptypes->numlibs,&ptypes->libs,NULL);
	addstdrsrc(mainrsrc,"UNPACK ZIPS",BOOLNS,1,&defexe,TRUE);
	addstdrsrc(mainrsrc,"OVERRIDE PASSWORD",PLAINTEXT,60,NULL,TRUE);
#ifdef WIN32_DONT_COMPILE
	addstdrsrc(mainrsrc,"REBOOT WHEN FINISHED",BOOLNS,1,&reboot,TRUE);
#endif
	addrfkwrsrc(mainrsrc,"SELECT",TRUE,ftpget,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	DefaultScreens(mainrsrc);
	mainrsrc->scn=RDAscrnNEW(module,"SSH PROGRAMS");
	if(mainrsrc->scn!=NULL)
	{
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","Default O/S:","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,0,"DEFAULT OPERATING SYSTEM","Default O/S:","","",0,15,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","Program Type:","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,7,"TYPES","Program Type","","",1,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","   ","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","                    URL Examples: updates.openrda.com or 66.135.38.166","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","              URL:","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,0,"SSH SERVER","Ftp Server","","",0,60,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
#ifdef WIN32_DONT_COMPILE
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,9,"REBOOT WHEN FINISHED","Reboot When Finished","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
#endif
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","      Destination:","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,0,"DESTINATION PATH","Destination Path","","",0,60,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","Override Password:","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,0,"OVERRIDE PASSWORD","Override Password","","",0,60,3,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");

		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,6,"SELECT","Select","","",0,0,25,"","","","");
		ADVaddwdgt(mainrsrc->scn,6,"QUIT","Cancel","","",0,0,14,"","","","");
		ADVaddwdgt(mainrsrc->scn,6,"HELP","Help","","",0,0,11,"","","","");
		ADVaddwdgt(mainrsrc->scn,6,"DEFAULTS","Save Defaults","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
		APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
	} else {
		prterr("Error:  Can't create SSH PROGRAMS screen.");
		quitfunc(mainrsrc);
	}
}
