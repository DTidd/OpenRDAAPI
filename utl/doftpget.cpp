/* doftpget.c - Program to Ftp Get Programs */
/*lint -library */
#ifndef WIN32
#define __NAM__ "doftpget.lnx"
#endif
#ifdef WIN32
#define __NAM__ "doftpget.exe"
#endif
#include <app.hpp>


#include <mix.hpp>
#include <curl/curl.h>
#include <curl/easy.h>
#ifdef WIN32
#include <sys/stat.h>
#endif /* ifdef WIN32 */

/*ADDLIB curl */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)
#define MODE_RWXRWX___	(S_IRWXU|S_IRWXG)
#endif /* Not LINUX && Not LINUX2_2 && Not UBUNTU_OS */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___	(00770)
#endif /* LINUX || LINUX2_2 || UBUNTU_OS*/

static char *typeString[]={"motif","gtk","qt"};
char *module="UTILITIES",*ftpsvr=NULL,*dest_path=NULL,defexe=TRUE;
char deflib=FALSE,defpdf=FALSE,defzip=TRUE,deftgz=FALSE;
char defurl=FALSE,defoth=FALSE;
char *DEF_OS=NULL;
static char *overpass=NULL;
int ptype=0;
CURL *RDAcurl=NULL;
APPlib *ptypes=NULL,*proglist=NULL;
#define USE_PROXY
#ifdef USE_PROXY
char use_proxy=FALSE,*http_proxy=NULL;
int http_proxy_port=1080;
#endif /* USE PROXY */

struct FtpFile {
	char *filename;
	FILE *stream;
};

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
	if(r!=NULL) free_rsrc(r);
	if(ftpsvr!=NULL) Rfree(ftpsvr);
	if(ptypes!=NULL) freeapplib(ptypes);
	curl_global_cleanup();
	if(proglist!=NULL) freeapplib(proglist);
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
int packlist(void *buffer,size_t size,size_t nmemb,void *stream)
{
	struct FtpFile *out=(struct FtpFile *)stream;

	if(out && !out->stream)
	{
		out->stream=fopen(out->filename,"wb");
		if(!out->stream) return(-1);
	}
	return(fwrite(buffer,size,nmemb,out->stream));
}
int getprogram(void *buffer,size_t size,size_t nmemb,void *stream)
{
	struct FtpFile *out=(struct FtpFile *)stream;
	char *namx=NULL,*tempx=NULL;

	if(out && !out->stream)
	{
		namx=Rmalloc(RDAstrlen(out->filename)+RDAstrlen(dest_path)+512);
		tempx=strchr(out->filename,'/');
		if(tempx!=NULL) ++tempx;
#ifdef WIN32
		sprintf(namx,"%s\\%s",dest_path,tempx);
#endif
#ifndef WIN32
		sprintf(namx,"%s/%s",dest_path,tempx);
#endif
		unlink(namx);
		out->stream=fopen(namx,"wb");
		if(namx!=NULL) Rfree(namx);
		if(!out->stream) return(-1);
	}
	return(fwrite(buffer,size,nmemb,out->stream));
}
static void doftpget(RDArsrc *r)
{
	int x=0,selected=0,fd=0;
	char flag=FALSE,*nam=NULL,*temp=NULL,*t=NULL,*tempx=NULL,*namx=NULL;
	CURLcode res=0;
	struct FtpFile plst={NULL,NULL};
	RDArsrc *tmprsrc=NULL;
	long epsv=0;
	APPlib *args=NULL;
	int runinstdeps=0;
	char instdepcmd[200];

#ifdef WIN32
	short y=0;
	char *tmp=NULL,*tempdir=NULL,*t1=NULL,*t2=NULL;
	int j=0,k=0;
	char stemp1[101],stemp2[101];
#endif

	readallwidgets(r);
	FINDRSCGETSTRING(r,"DEFAULT OPERATING SYSTEM",&DEF_OS);
	if(r!=NULL) killwindow(r);
	for(x=0;x<proglist->numlibs;++x)
	{
		FINDRSCGETCHAR(r,proglist->libs[x],&flag);
		if(flag==TRUE)
		{			
			++selected;
		}
	}
	tmprsrc=odiagscrn("DIAGNOSTIC SCREEN","UTILITIES","Obtaining Files using FTPGET",stop_diagnostic,selected);
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
		ADVaddwdgt(tmprsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(tmprsrc->scn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(tmprsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(tmprsrc->scn,22,"PROGRESS BAR","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(tmprsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(tmprsrc->scn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(tmprsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
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
	for(x=0;x<proglist->numlibs;++x)
	{
		checkforinterrupt(tmprsrc);
		if(abort_diagnostic==TRUE) break;
		FINDRSCGETCHAR(r,proglist->libs[x],&flag);
		if(flag==TRUE)
		{			
			RDAcurl=curl_easy_init();
			nam=Rmalloc(RDAstrlen(ftpsvr)+RDAstrlen(proglist->libs[x])+512);
			temp=stralloc(proglist->libs[x]);
			t=strchr(temp,' ');
			if(t!=NULL) *t=0;
			if(plst.filename!=NULL) Rfree(plst.filename);
			plst.filename=stralloc(temp);
			plst.stream=NULL;
			if(XPERT_SETUP->software_type<2)
			{
#if defined(LINUX2_2) || defined(UBUNTU_OS)
				sprintf(nam,"ftp://%s/./%s/%s/%s",(ftpsvr!=NULL ? ftpsvr:""),typeString[ptype],DEF_OS,temp);
#endif
#ifdef WIN32
				sprintf(nam,"ftp://%s/./%s/%s/%s",(ftpsvr!=NULL ? ftpsvr:""),typeString[ptype],DEF_OS,temp);
#endif
			} else {
#if defined(LINUX2_2) || defined(UBUNTU_OS)
				sprintf(nam,"ftp://%s/./%s/%s/%s",(ftpsvr!=NULL ? ftpsvr:""),typeString[ptype],DEF_OS,temp);
#endif
#ifdef WIN32
				sprintf(nam,"ftp://%s/./%s/%s/%s",(ftpsvr!=NULL ? ftpsvr:""),typeString[ptype],DEF_OS,temp);
#endif
			}
			if(temp!=NULL) Rfree(temp);
			curl_easy_setopt(RDAcurl,CURLOPT_URL,nam);
			curl_easy_setopt(RDAcurl,CURLOPT_FTP_USE_EPSV,epsv);
			if(XPERT_SETUP->software_type<2)
			{
				if(overpass!=NULL)
				{
					if(!isEMPTY(overpass))
					{
						memset(stemp,0,101);
						sprintf(stemp,"xpert:%s",overpass);
						curl_easy_setopt(RDAcurl,CURLOPT_USERPWD,(char *)stemp);
					} else {
						curl_easy_setopt(RDAcurl,CURLOPT_USERPWD,"xpert:e2giVcYo");
					}
				} else {
					curl_easy_setopt(RDAcurl,CURLOPT_USERPWD,"xpert:e2giVcYo");
				}
			} else {
				if(overpass!=NULL)
				{
					if(!isEMPTY(overpass))
					{
						memset(stemp,0,101);
						sprintf(stemp,"mbgui:%s",overpass);
						curl_easy_setopt(RDAcurl,CURLOPT_USERPWD,(char *)stemp);
					} else {
						curl_easy_setopt(RDAcurl,CURLOPT_USERPWD,"mbgui:m8KreBj5");
					}
				} else {
					curl_easy_setopt(RDAcurl,CURLOPT_USERPWD,"mbgui:m8KreBj5");
				}
			}
#ifdef USE_PROXY
			if(use_proxy)
			{
				sprintf(stemp,"%s:%d",(http_proxy!=NULL ? http_proxy:""),http_proxy_port);
				curl_easy_setopt(RDAcurl,CURLOPT_PROXY,(char *)stemp);
			}
#endif /* USE PROXY */
			
			if(RDA_GetEnv("RDADIAG")!=NULL)
			{
				curl_easy_setopt(RDAcurl,CURLOPT_VERBOSE,FALSE); 
			}
			curl_easy_setopt(RDAcurl,CURLOPT_WRITEFUNCTION,getprogram);
			curl_easy_setopt(RDAcurl,CURLOPT_FILE,&plst);

			res=curl_easy_perform(RDAcurl);
			curl_easy_cleanup(RDAcurl);
			if(plst.stream) fclose(plst.stream);
			namx=Rmalloc(RDAstrlen(plst.filename)+RDAstrlen(dest_path)+512);
			tempx=strchr(plst.filename,'/');
			if(tempx!=NULL) ++tempx;
#ifdef WIN32
			sprintf(namx,"%s\\%s",dest_path,tempx);
#endif
#ifndef WIN32
			sprintf(namx,"%s/%s",dest_path,tempx);
			if(!strncmp(&namx[RDAstrlen(namx)-4],".lnx",4) || !strncmp(&namx[RDAstrlen(namx)-4],".exe",4))
			{
				fd=chmod(namx,(mode_t)MODE_RWXRWX___);
				if(fd==(-1))
				{
					prterr("Error:  Couldn't set Mode for [%s].",(namx!=NULL ? namx:""));
				}
			}
#endif
			/*if(!strncmp(&namx[RDAstrlen(namx)-3],".7z",3) || !strncmp(&namx[RDAstrlen(namx)-4],".zip",4)) && defexe==TRUE) */

			if(( (!strncmp(&namx[RDAstrlen(namx)-3],".7z",3)) || (!strncmp(&namx[RDAstrlen(namx)-4],".zip",4))) && defexe==TRUE)
			{
#ifndef WIN32
				if(!chdir(dest_path))
				{
					args=APPlibNEW();
					addAPPlib(args,"x");
					addAPPlib(args,"-aoa");
					addAPPlib(args,tempx);
					ADVExecute2Program("7za",args,NULL);
					if(args!=NULL) freeapplib(args);
					unlink(tempx);

					if((XPERT_SETUP->ASP!=TRUE)&&(!strncmp(tempx,"UTILITIES",9)))
					{
						runinstdeps=1;
					}

					chdir(CURRENTDIRECTORY);
				} else {
					prterr("Error:  Unable to change directory to Destination Path [%s], didn't unpack or remove zip [%s].",(dest_path!=NULL ? dest_path:""),(namx!=NULL ? namx:""));
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
			if(runinstdeps) {
				memset(instdepcmd,0,200);
				sprintf(instdepcmd,"/usr/bin/X11/xterm -ls -T \"INSTALL DEPENDENCIES\" -n \"INSTALL DEPENDENCIES\" -e %s/install_qt_deps.lnx",dest_path);
				system_command(instdepcmd);
			}
#endif
			if(namx!=NULL) Rfree(namx);
			if(CURLE_OK!=res)
			{
				prterr("Error:  Failed to obtain program [%s] with error [%d].",plst.filename,res);
			}
			if(nam!=NULL) Rfree(nam);
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
			if(plst.filename!=NULL) Rfree(plst.filename);
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
		if(t1!=NULL) Rfree(t1);
		if(t2!=NULL) Rfree(t2);
		DID_STUFF=TRUE;
	}
	if(tmp!=NULL) Rfree(tmp);
#endif
	quitfunc(r);
}
static void ContentstoAPPlib(APPlib *proglist,char *dirx,RDArsrc *tmprsrc)
{
	char *nam=NULL,*temp=NULL,*temp2=NULL,*temp3=NULL;
	char col1[512],col2[512],col3[512],col4[512],col5[512],col6[512],col7[512],col8[512],col9[512];
	char *myptr=NULL;
	CURLcode res=0;
	struct FtpFile plst={"pack.lst",NULL};
	int lsize=512,z=0;
	long epsv=0;

	unlink(plst.filename);
	RDAcurl=curl_easy_init();
	if(RDAcurl==NULL)
	{
		ERRORDIALOG("CURL Initialization Failed","Initialization of Curl function failed.  Verify network integrity.",NULL,TRUE);
		return;
	}
	nam=Rmalloc(RDAstrlen(ftpsvr)+512);
	if(XPERT_SETUP->software_type<2)
	{
#if defined(LINUX2_2) || defined(UBUNTU_OS)
		sprintf(nam,"ftp://%s/./%s/%s/%s/",(ftpsvr!=NULL ? ftpsvr:""),typeString[ptype],DEF_OS,(dirx!=NULL ? dirx:""));
#endif
#ifdef WIN32
		sprintf(nam,"ftp://%s/./%s/%s/%s/",(ftpsvr!=NULL ? ftpsvr:""),typeString[ptype],DEF_OS,(dirx!=NULL ? dirx:""));
#endif
	} else {
#if defined(LINUX2_2) || defined(UBUNTU_OS)
		sprintf(nam,"ftp://%s/./%s/%s/%s/",(ftpsvr!=NULL ? ftpsvr:""),typeString[ptype],DEF_OS,(dirx!=NULL ? dirx:""));
#endif
#ifdef WIN32
		sprintf(nam,"ftp://%s/./%s/%s/%s/",(ftpsvr!=NULL ? ftpsvr:""),typeString[ptype],DEF_OS,(dirx!=NULL ? dirx:""));
#endif
	}
	curl_easy_setopt(RDAcurl,CURLOPT_URL,nam);
	curl_easy_setopt(RDAcurl,CURLOPT_FTP_USE_EPSV,epsv);
	if(XPERT_SETUP->software_type<2)
	{
		if(overpass!=NULL)
		{
			if(!isEMPTY(overpass))
			{
				memset(stemp,0,101);
				sprintf(stemp,"xpert:%s",overpass);
				curl_easy_setopt(RDAcurl,CURLOPT_USERPWD,(char *)stemp);
			} else {
				curl_easy_setopt(RDAcurl,CURLOPT_USERPWD,"xpert:e2giVcYo");
			}
		} else {
			curl_easy_setopt(RDAcurl,CURLOPT_USERPWD,"xpert:e2giVcYo");
		}
	} else {
		if(overpass!=NULL)
		{
			if(!isEMPTY(overpass))
			{
				memset(stemp,0,101);
				sprintf(stemp,"mbgui:%s",overpass);
				curl_easy_setopt(RDAcurl,CURLOPT_USERPWD,(char *)stemp);
			} else {
				curl_easy_setopt(RDAcurl,CURLOPT_USERPWD,"mbgui:m8KreBj5");
			}
		} else {
			curl_easy_setopt(RDAcurl,CURLOPT_USERPWD,"mbgui:m8KreBj5");
		}
	}
#ifdef USE_PROXY
	if(use_proxy)
	{
		sprintf(stemp,"%s:%d",(http_proxy!=NULL ? http_proxy:""),http_proxy_port);
		curl_easy_setopt(RDAcurl,CURLOPT_PROXY,(char *)stemp);
	}
#endif /* USE PROXY */
	curl_easy_setopt(RDAcurl,CURLOPT_VERBOSE,FALSE); 
	curl_easy_setopt(RDAcurl,CURLOPT_WRITEFUNCTION,packlist);
	curl_easy_setopt(RDAcurl,CURLOPT_FILE,&plst);

	res=curl_easy_perform(RDAcurl);
	curl_easy_cleanup(RDAcurl);
	if(plst.stream) fclose(plst.stream);
	if(CURLE_OK!=res)
	{
		prterr("Error:  Failed to obtain packing list [%s] with [%d].",(dirx!=NULL ? dirx:""),res);
	} else {
		plst.stream=fopen(plst.filename,"rb");
		temp=Rmalloc(lsize+1);
		while(fgets(temp,lsize,plst.stream))
		{
			if(!strncmp(temp,"total ",6))
			{
			} else {
				
				myptr=temp;
				

				while(*myptr == ' ') { ++myptr;	}
				memset(col1,0,512);
				z=0;
				while(*myptr != ' ')
				{
					if((*myptr=='\0')||(*myptr==10)||(*myptr==13)) { break; }
					col1[z]=*myptr;
					++z;
					++myptr;
				}
				
				
				while(*myptr == ' ') { ++myptr;	}
				memset(col2,0,512);
				z=0;
				while(*myptr != ' ')
				{
					if((*myptr=='\0')||(*myptr==10)||(*myptr==13)) { break; }
					col2[z]=*myptr;
					++z;
					++myptr;
				}
				

				while(*myptr == ' ') { ++myptr;	}
				memset(col3,0,512);
				z=0;
				while(*myptr != ' ')
				{
					if((*myptr=='\0')||(*myptr==10)||(*myptr==13)) { break; }
					col3[z]=*myptr;
					++z;
					++myptr;
				}
				

				while(*myptr == ' ') { ++myptr;	}
				memset(col4,0,512);
				z=0;
				while(*myptr != ' ')
				{
					if((*myptr=='\0')||(*myptr==10)||(*myptr==13)) { break; }
					col4[z]=*myptr;
					++z;
					++myptr;
				}
				

				while(*myptr == ' ') { ++myptr;	}
				memset(col5,0,512);
				z=0;
				while(*myptr != ' ')
				{
					if((*myptr=='\0')||(*myptr==10)||(*myptr==13)) { break; }
					col5[z]=*myptr;
					++z;
					++myptr;
				}
				

				while(*myptr == ' ') { ++myptr;	}
				memset(col6,0,512);
				z=0;
				while(*myptr != ' ')
				{
					if((*myptr=='\0')||(*myptr==10)||(*myptr==13)) { break; }
					col6[z]=*myptr;
					++z;
					++myptr;
				}
				

				while(*myptr == ' ') { ++myptr;	}
				memset(col7,0,512);
				z=0;
				while(*myptr != ' ')
				{
					if((*myptr=='\0')||(*myptr==10)||(*myptr==13)) { break; }
					col7[z]=*myptr;
					++z;
					++myptr;
				}
				

				while(*myptr == ' ') { ++myptr;	}
				memset(col8,0,512);
				z=0;
				while(*myptr != ' ')
				{
					if((*myptr=='\0')||(*myptr==10)||(*myptr==13)) { break; }
					col8[z]=*myptr;
					++z;
					++myptr;
				}
				

				while(*myptr == ' ') { ++myptr;	}
				memset(col9,0,512);
				z=0;
				while(*myptr != ' ')
				{
					if((*myptr=='\0')||(*myptr==10)||(*myptr==13)) { break; }
					col9[z]=*myptr;
					++z;
					++myptr;
				}
				

				memset(stemp,0,101);
				sprintf(stemp,"%s/%s",(dirx!=NULL ? dirx:""),col9);
				/* prterr("stemp [%s]\n",stemp); */
				temp2=Rmalloc(512);
				sprintf(temp2,"%-40s %12ld %s %s %s",stemp,atoi(col5),col6,col7,col8);
				/* prterr("temp2 [%s]\n",temp2); */
				addAPPlib(proglist,temp2);

				if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
				if(temp2!=NULL) Rfree(temp2);
				if(temp3!=NULL) Rfree(temp3);
			}
		}	
		if(temp!=NULL) Rfree(temp);
		if(plst.stream) fclose(plst.stream);
	}	
	unlink(plst.filename);
	if(nam!=NULL) Rfree(nam);
}
static void ftpget(RDArsrc *r)
{
	char *temp=NULL,*t=NULL;
	int x=0,selected=0;
	RDArsrc *editrsrc=NULL;
	RDArsrc *tmprsrc=NULL;
#ifdef WIN32_DONT_COMPILE
	int reboot=0;
#endif

	readwidget(r,"DEFAULT OPERATING SYSTEM");
	FINDRSCGETSTRING(r,"DEFAULT OPERATING SYSTEM",&DEF_OS);
	readwidget(r,"FTP SERVER");
	FINDRSCGETSTRING(r,"FTP SERVER",&ftpsvr);
	readwidget(r,"DESTINATION PATH");
	FINDRSCGETSTRING(r,"DESTINATION PATH",&dest_path);
	readwidget(r,"FTP USE PROXY");
	FINDRSCGETCHAR(r,"FTP USE PROXY",&use_proxy);	
	readwidget(r,"FTP PROXY");
	FINDRSCGETSTRING(r,"FTP PROXY",&http_proxy);
	readwidget(r,"FTP PROXY PORT");
	FINDRSCGETINT(r,"FTP PROXY PORT",&http_proxy_port);
	readwidget(r,"UNPACK ZIPS");
	FINDRSCGETCHAR(r,"UNPACK ZIPS",&defexe);
	readwidget(r,"DEFAULT LIBRARIES");
	FINDRSCGETCHAR(r,"DEFAULT LIBRARIES",&deflib);
	readwidget(r,"DEFAULT PDFS");
	FINDRSCGETCHAR(r,"DEFAULT PDFS",&defpdf);
	readwidget(r,"DEFAULT ZIPS");
	FINDRSCGETCHAR(r,"DEFAULT ZIPS",&defzip);
	readwidget(r,"DEFAULT TGZS");
	FINDRSCGETCHAR(r,"DEFAULT TGZS",&deftgz);
	readwidget(r,"DEFAULT URLS");
	FINDRSCGETCHAR(r,"DEFAULT URLS",&defurl);
	readwidget(r,"DEFAULT OTHERS");
	FINDRSCGETCHAR(r,"DEFAULT OTHERS",&defoth);
	readwidget(r,"OVERRIDE PASSWORD");
	FINDRSCGETSTRING(r,"OVERRIDE PASSWORD",&overpass);
	FINDRSCGETINT(r,"TYPES",&ptype);
#ifdef WIN32_DONT_COMPILE
	readwidget(r,"REBOOT WHEN FINISHED",&reboot);
	FINDRSCGETCHAR(r,"REBOOT WHEN FINISHED",&reboot);
#endif
	tmprsrc=odiagscrn("DIAGNOSTIC SCREEN","UTILITIES","Obtaining Directory Listing",stop_diagnostic,selected);
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
		ADVaddwdgt(tmprsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(tmprsrc->scn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(tmprsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(tmprsrc->scn,22,"PROGRESS BAR","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(tmprsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(tmprsrc->scn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(tmprsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(tmprsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(tmprsrc->scn,5,"","Selected","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(tmprsrc->scn,5,"","Total Read","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,0,"SELECTED","Selected","","",0,8,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,0,"TOTAL READ","Total Read","","",0,8,0,"","","","");
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
		ADVaddwdgt(tmprsrc->scn,6,"HELP","Help","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(tmprsrc->scn,2,"","","","",0,0,0,"","","","");
		if(!APPmakescrn(tmprsrc,FALSE,NULL,NULL,FALSE)) ForceWindowUpdate(tmprsrc);
	}
	if(proglist!=NULL) freeapplib(proglist);
	proglist=APPlibNEW();
	if(XPERT_SETUP->software_type<2)
	{
		if(XPERT_SETUP->UTILITIES) ContentstoAPPlib(proglist,"UTILITIES",tmprsrc);
		if(XPERT_SETUP->FINMGT) ContentstoAPPlib(proglist,"FINMGT",tmprsrc);
		if(XPERT_SETUP->BNKREC) ContentstoAPPlib(proglist,"BNKREC",tmprsrc);
		if(XPERT_SETUP->PRSNNL) ContentstoAPPlib(proglist,"PRSNNL",tmprsrc);
		if(XPERT_SETUP->HRM) ContentstoAPPlib(proglist,"OPENSS",tmprsrc);
		if(XPERT_SETUP->IIDMGT) ContentstoAPPlib(proglist,"IIDMGT",tmprsrc);
		if(XPERT_SETUP->APPMGT) ContentstoAPPlib(proglist,"APPMGT",tmprsrc);
		if(XPERT_SETUP->POSTRK) ContentstoAPPlib(proglist,"POSTRK",tmprsrc);
		if(XPERT_SETUP->PAYROLL) ContentstoAPPlib(proglist,"PAYROLL",tmprsrc);
		if(XPERT_SETUP->SUBMGT) ContentstoAPPlib(proglist,"SUBMGT",tmprsrc);
		if(XPERT_SETUP->LVEMGT) ContentstoAPPlib(proglist,"LVEMGT",tmprsrc);
		if(XPERT_SETUP->EMPABS) ContentstoAPPlib(proglist,"EMPABS",tmprsrc);
		if(XPERT_SETUP->BFTMGT) ContentstoAPPlib(proglist,"BFTMGT",tmprsrc);
		if(XPERT_SETUP->VENPMT) ContentstoAPPlib(proglist,"VENPMT",tmprsrc);
		if(XPERT_SETUP->PURORD) ContentstoAPPlib(proglist,"PURORD",tmprsrc);
/*
		if(XPERT_SETUP->CATALOGUE) ContentstoAPPlib(proglist,"CATALOGUE",tmprsrc);
*/
		if(XPERT_SETUP->REQMGT) ContentstoAPPlib(proglist,"REQMGT",tmprsrc);
		if(XPERT_SETUP->FIXASS) ContentstoAPPlib(proglist,"FIXASS",tmprsrc);
		if(XPERT_SETUP->INVCTL) ContentstoAPPlib(proglist,"INVCTL",tmprsrc);
		if(XPERT_SETUP->OCCTAX) ContentstoAPPlib(proglist,"OCCTAX",tmprsrc);
		if(XPERT_SETUP->BUDPREP) ContentstoAPPlib(proglist,"BUDPREP",tmprsrc);
		if(XPERT_SETUP->MISCBILL) ContentstoAPPlib(proglist,"MISCBILL",tmprsrc);
		if(XPERT_SETUP->TAXCLT) ContentstoAPPlib(proglist,"TAXCLT",tmprsrc);
		if(XPERT_SETUP->RLSTMGT) ContentstoAPPlib(proglist,"RLSTMGT",tmprsrc);
		if(XPERT_SETUP->PPTMGT) ContentstoAPPlib(proglist,"PPTMGT",tmprsrc);
		if(XPERT_SETUP->STUDFIN) ContentstoAPPlib(proglist,"STUDFIN",tmprsrc);
		if(XPERT_SETUP->TOOLS) ContentstoAPPlib(proglist,"TOOLS",tmprsrc);
		if(XPERT_SETUP->PROPERTY) ContentstoAPPlib(proglist,"PROPERTY",tmprsrc);
		if(XPERT_SETUP->UTLBLL) ContentstoAPPlib(proglist,"UTLBLL",tmprsrc);
		if(XPERT_SETUP->DMVREG) ContentstoAPPlib(proglist,"DMVREG",tmprsrc);
		if(XPERT_SETUP->LVSIMP) ContentstoAPPlib(proglist,"LVSIMP",tmprsrc);
		if(XPERT_SETUP->CSHDWR) ContentstoAPPlib(proglist,"CSHDWR",tmprsrc);
		if(XPERT_SETUP->BLDPRMT) ContentstoAPPlib(proglist,"BLDPRMT",tmprsrc);
		if(XPERT_SETUP->ARCHIVE) ContentstoAPPlib(proglist,"ARCHIVE",tmprsrc);
		if(XPERT_SETUP->CITWEB) ContentstoAPPlib(proglist,"CITWEB",tmprsrc);
		if(XPERT_SETUP->OPENSOURCE) ContentstoAPPlib(proglist,"OPENSOURCE",tmprsrc);
	} else {
		if(XPERT_SETUP->UTILITIES) ContentstoAPPlib(proglist,"UTILITIES",tmprsrc);
		if(XPERT_SETUP->FINMGT) ContentstoAPPlib(proglist,"FINMGT",tmprsrc);
		if(XPERT_SETUP->BNKREC) ContentstoAPPlib(proglist,"BNKREC",tmprsrc);
		if(XPERT_SETUP->PRSNNL) ContentstoAPPlib(proglist,"PRSNNL",tmprsrc);
		if(XPERT_SETUP->PAYROLL) ContentstoAPPlib(proglist,"PAYROLL",tmprsrc);
		if(XPERT_SETUP->LVEMGT) ContentstoAPPlib(proglist,"LVEMGT",tmprsrc);
		if(XPERT_SETUP->VENPMT) ContentstoAPPlib(proglist,"VENPMT",tmprsrc);
		if(XPERT_SETUP->PURORD) ContentstoAPPlib(proglist,"PURORD",tmprsrc);
		if(XPERT_SETUP->REQMGT) ContentstoAPPlib(proglist,"REQMGT",tmprsrc);
		if(XPERT_SETUP->FIXASS) ContentstoAPPlib(proglist,"FIXASS",tmprsrc);
		if(XPERT_SETUP->OCCTAX) ContentstoAPPlib(proglist,"OCCTAX",tmprsrc);
		if(XPERT_SETUP->BUDPREP) ContentstoAPPlib(proglist,"BUDPREP",tmprsrc);
		if(XPERT_SETUP->MISCBILL) ContentstoAPPlib(proglist,"MISCBILL",tmprsrc);
		if(XPERT_SETUP->RLSTMGT) ContentstoAPPlib(proglist,"RLSTMGT",tmprsrc);
		if(XPERT_SETUP->STUDFIN) ContentstoAPPlib(proglist,"STUDFIN",tmprsrc);
		if(XPERT_SETUP->TOOLS) ContentstoAPPlib(proglist,"TOOLS",tmprsrc);
		if(XPERT_SETUP->PROPERTY) ContentstoAPPlib(proglist,"PROPERTY",tmprsrc);
		if(XPERT_SETUP->RLVEMGT) ContentstoAPPlib(proglist,"RLVEMGT",tmprsrc);
		if(XPERT_SETUP->UTLBLL) ContentstoAPPlib(proglist,"UTLBLL",tmprsrc);
		if(XPERT_SETUP->DMVREG) ContentstoAPPlib(proglist,"DMVREG",tmprsrc);
		if(XPERT_SETUP->CSHDWR) ContentstoAPPlib(proglist,"CSHDWR",tmprsrc);
		if(XPERT_SETUP->RREQMGT) ContentstoAPPlib(proglist,"RREQMGT",tmprsrc);
		if(XPERT_SETUP->BLDPRMT) ContentstoAPPlib(proglist,"BLDPRMT",tmprsrc);
		if(XPERT_SETUP->ARCHIVE) ContentstoAPPlib(proglist,"ARCHIVE",tmprsrc);
		if(XPERT_SETUP->OPENSOURCE) ContentstoAPPlib(proglist,"OPENSOURCE",tmprsrc);
	}
	SORTAPPlib(proglist);

	if(proglist->numlibs>0)
	{
		editrsrc=RDArsrcNEW("UTILITIES","FTP PROGRAM LIST");
		for(x=0;x<proglist->numlibs;++x)
		{
			temp=stralloc(proglist->libs[x]);
			t=strchr(temp,' ');
			if(t!=NULL) *t=0;
			stolower(temp);
			if(!strncmp(&temp[RDAstrlen(temp)-4],".pdf",4))
			{
				addstdrsrc(editrsrc,proglist->libs[x],BOOLNS,1,&defpdf,TRUE);
			} else if(!strncmp(&temp[RDAstrlen(temp)-4],".dll",4)  ||
				!strncmp(&temp[RDAstrlen(temp)-3],".so",3) ||
				!strncmp(&temp[RDAstrlen(temp)-3],".la",3))
			{
				addstdrsrc(editrsrc,proglist->libs[x],BOOLNS,1,&deflib,TRUE);
			} else if(!strncmp(&temp[RDAstrlen(temp)-4],".zip",4))
			{
				addstdrsrc(editrsrc,proglist->libs[x],BOOLNS,1,&defzip,TRUE);
			} else if(!strncmp(&temp[RDAstrlen(temp)-3],".7z",3))
			{
				addstdrsrc(editrsrc,proglist->libs[x],BOOLNS,1,&defzip,TRUE);
			} else if(!strncmp(&temp[RDAstrlen(temp)-4],".tgz",4))
			{
				addstdrsrc(editrsrc,proglist->libs[x],BOOLNS,1,&deftgz,TRUE);
			} else if(!strncmp(&temp[RDAstrlen(temp)-4],".url",4))
			{
				addstdrsrc(editrsrc,proglist->libs[x],BOOLNS,1,&defurl,TRUE);
			} else {
				addstdrsrc(editrsrc,proglist->libs[x],BOOLNS,1,&defoth,TRUE);
			}	
		}
		addrfexrsrc(editrsrc,"SELECT",TRUE,doftpget,NULL);
		addrfexrsrc(editrsrc,"QUIT",TRUE,quitfunc,NULL);
		addrfcbrsrc(editrsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(editrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		editrsrc->scn=RDAscrnNEW(module,"FTP PROGRAMS");
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
#ifdef USE_PROXY
static void setproxy(RDArsrc *r)
{
	readwidget(r,"FTP USE PROXY");
	FINDRSCGETCHAR(r,"FTP USE PROXY",&use_proxy);
	if(use_proxy==TRUE)
	{
		FINDRSCSETEDITABLE(r,"FTP PROXY",TRUE);
		FINDRSCSETEDITABLE(r,"FTP PROXY PORT",TRUE);
		FINDRSCSETSENSITIVE(r,"FTP PROXY",TRUE);
		FINDRSCSETSENSITIVE(r,"FTP PROXY PORT",TRUE);
	} else {
		FINDRSCSETEDITABLE(r,"FTP PROXY",FALSE);
		FINDRSCSETEDITABLE(r,"FTP PROXY PORT",FALSE);
		FINDRSCSETSENSITIVE(r,"FTP PROXY",FALSE);
		FINDRSCSETSENSITIVE(r,"FTP PROXY PORT",FALSE);
	}
}
#endif /* USE PROXY */
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
#ifdef USE_PROXY
	RDAGenericSetup *gsv=NULL;
	char *libx=NULL;
#endif /* USE PROXY */
#ifndef WIN32
	char *temp=NULL;
#endif

	if(InitializeSubsystems(argc,argv,module,"FTP PROGRAMS")) 
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
#ifdef UBUNTU_OS
	DEF_OS=stralloc("ubuntu_lts_64");
#else
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
#endif


#ifdef WIN32
#ifdef USE_ADMIN_USER
	if(!AmIAdministrator())
	{
		ERRORDIALOG("NON-ADMINISTRATIVE USER","Permissions preclude you from doing Administrative Tasks.  You must either log in as the administrator or obtain administrative permissions for doftpget.exe.",NULL,TRUE);
		return;
	}
#endif /* USE_ADMIN_USER */
#endif /* WIN32 */
	curl_global_init(CURL_GLOBAL_DEFAULT);	
#ifdef USE_PROXY
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("UTILITIES")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif

	gsv=RDAGenericSetupNew("UTILITIES","FTP USE PROXY");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			use_proxy=*gsv->value.string_value;
		} else {
			use_proxy=FALSE;
		}
	}
	gsv=RDAGenericSetupNew("UTILITIES","FTP PROXY");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			http_proxy=stralloc(gsv->value.string_value);
		} else {
			http_proxy=NULL;
		}
	}
	gsv=RDAGenericSetupNew("UTILITIES","FTP PROXY PORT");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			http_proxy_port=*gsv->value.integer_value;
		} else {
			http_proxy_port=1080;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
#endif /* USE PROXY */
	mainrsrc=RDArsrcNEW(module,"FTP PROGRAMS");
	ftpsvr=stralloc("66.135.38.166");
	addstdrsrc(mainrsrc,"FTP SERVER",VARIABLETEXT,0,ftpsvr,TRUE);
	addstdrsrc(mainrsrc,"DEFAULT OPERATING SYSTEM",VARIABLETEXT,0,DEF_OS,TRUE);
	addstdrsrc(mainrsrc,"FTP SERVER",VARIABLETEXT,0,ftpsvr,TRUE);
#ifdef USE_PROXY
	addstdrsrc(mainrsrc,"FTP USE PROXY",BOOLNS,1,&use_proxy,TRUE);
	addstdrsrc(mainrsrc,"FTP PROXY",VARIABLETEXT,0,http_proxy,TRUE);
	addstdrsrc(mainrsrc,"FTP PROXY PORT",LONGV,4,&http_proxy_port,TRUE);
#endif /* USE PROXY */
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
	addAPPlib(ptypes,"Wt");
	ptype=0;
	addlstrsrc(mainrsrc,"TYPES",&ptype,TRUE,NULL,ptypes->numlibs,&ptypes->libs,NULL);
	addstdrsrc(mainrsrc,"UNPACK ZIPS",BOOLNS,1,&defexe,TRUE);
	addstdrsrc(mainrsrc,"DEFAULT LIBRARIES",BOOLNS,1,&deflib,TRUE);
	addstdrsrc(mainrsrc,"DEFAULT PDFS",BOOLNS,1,&defpdf,TRUE);
	addstdrsrc(mainrsrc,"DEFAULT ZIPS",BOOLNS,1,&defzip,TRUE);
	addstdrsrc(mainrsrc,"DEFAULT TGZS",BOOLNS,1,&deftgz,TRUE);
	addstdrsrc(mainrsrc,"DEFAULT URLS",BOOLNS,1,&defurl,TRUE);
	addstdrsrc(mainrsrc,"DEFAULT OTHERS",BOOLNS,1,&defoth,TRUE);
	addstdrsrc(mainrsrc,"OVERRIDE PASSWORD",PLAINTEXT,60,NULL,TRUE);
#ifdef WIN32_DONT_COMPILE
	addstdrsrc(mainrsrc,"REBOOT WHEN FINISHED",BOOLNS,1,&reboot,TRUE);
#endif
	addrfkwrsrc(mainrsrc,"SELECT",TRUE,ftpget,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
#ifdef USE_PROXY
	FINDRSCSETFUNC(mainrsrc,"FTP USE PROXY",setproxy,NULL);
	setproxy(mainrsrc);
#endif /* USE_PROXY */
	DefaultScreens(mainrsrc);
	mainrsrc->scn=RDAscrnNEW(module,"FTP PROGRAMS");
	if(mainrsrc->scn!=NULL)
	{
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","Default O/S:","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,0,"DEFAULT OPERATING SYSTEM","Default O/S:","","",0,15,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
#ifdef USE_PROXY
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,9,"FTP USE PROXY","Use Proxy","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","PROXY:","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,0,"FTP PROXY","Proxy","","",0,40,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","PORT:","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,0,"FTP PROXY PORT","Port","","",0,4,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,11,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
#endif /* USE PROXY */
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","URL Examples: (New) updates.openrda.com (Old) ftp.rdasys.com or 66.135.38.166","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","URL:","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,0,"FTP SERVER","Ftp Server","","",0,60,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","Program Type:","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,7,"TYPES","Program Type","","",1,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","PROFILE","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,3,"","","","",0,0,0,"","","","");
#ifdef WIN32_DONT_COMPILE
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,9,"REBOOT WHEN FINISHED","Reboot When Finished","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
#endif

		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,9,"DEFAULT ZIPS","ZIPs","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,9,"UNPACK ZIPS","Unpack Zips","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,9,"DEFAULT PDFS","PDFs","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,9,"DEFAULT TGZS","TGZs","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,9,"DEFAULT URLS","URLs","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,9,"DEFAULT LIBRARIES","Libraries","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,9,"DEFAULT OTHERS","Others","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
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
		prterr("Error:  Can't create FTP PROGRAMS screen.");
		quitfunc(mainrsrc);
	}
}
