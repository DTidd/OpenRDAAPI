/* doftpzip.c - Program to Backup Xpert Package */
/*lint -library */
#include <cstdio>
#include <sys/stat.h>
#ifndef WIN32
#define __NAM__ "doftpzip.lnx"
#endif
#ifdef WIN32
#define __NAM__ "doftpzip.exe"
#endif
#include <app.hpp>


#include <mix.hpp>
#include <curl/curl.h>
#include <curl/easy.h>
/*ADDLIB curl */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */

char *module="DATABASE",*drvspc=NULL,*ftpsvr=NULL;
static char *overpass=NULL;
APPlib *whichs=NULL;
CURL *RDAcurl=NULL;
#define USE_PROXY
#ifdef USE_PROXY
char use_proxy=FALSE,*http_proxy=NULL;
int http_proxy_port=1080;
#endif /* USE PROXY */
static char ARCHIVE_BACKUPS=FALSE;

struct FtpFile {
	char *filename;
	FILE *stream;
};
static void quitfunc(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	if(drvspc!=NULL) Rfree(drvspc);
	if(ftpsvr!=NULL) Rfree(ftpsvr);
	if(whichs!=NULL) freeapplib(whichs);
	curl_global_cleanup();
	ShutdownSubsystems();
	std::exit;
}
int getprogram(void *buffer,size_t size,size_t nmemb,void *stream)
{
	struct FtpFile *out=(struct FtpFile *)stream;

	if(out && !out->stream)
	{
		out->stream=fopen(out->filename,"wb");
		if(!out->stream) return(-1);
	}
	return(fwrite(buffer,size,nmemb,out->stream));
}
static void doftpzipget(RDArsrc *r)
{
	char *nam=NULL;
	CURLcode res=0;
	struct FtpFile plst={NULL,NULL};
#ifndef WIN32
	char *command=NULL,*dsp2use=NULL;
#endif
#ifdef WIN32
	APPlib *args=NULL;
#endif
	long epsv=0;

	readallwidgets(r);
	FINDRSCGETSTRING(r,"DRIVE SPEC",&drvspc);
	FINDRSCGETSTRING(r,"FTP SERVER",&ftpsvr);
	FINDRSCGETSTRING(r,"OVERRIDE PASSWORD",&overpass);
	sprintf(stemp,"%s.7z",(drvspc!=NULL ? drvspc:""));
	unlink(stemp); 
	RDAcurl=curl_easy_init();
	nam=Rmalloc(RDAstrlen(ftpsvr)+RDAstrlen(stemp)+512);
	if(plst.filename!=NULL) Rfree(plst.filename);
	plst.filename=stralloc(stemp);
	plst.stream=NULL;
	if(XPERT_SETUP->software_type<2)
	{
#if defined(LINUX2_2) || defined(UBUNTU_OS)
		sprintf(nam,"ftp://%s/%s",(ftpsvr!=NULL ? ftpsvr:""),stemp);
#endif
#ifdef WIN32
		sprintf(nam,"ftp://%s/%s",(ftpsvr!=NULL ? ftpsvr:""),stemp);
#endif
	} else {
#if defined(LINUX2_2) || defined(UBUNTU_OS)
		sprintf(nam,"ftp://%s/%s",(ftpsvr!=NULL ? ftpsvr:""),stemp);
#endif
#ifdef WIN32
		sprintf(nam,"ftp://%s/%s",(ftpsvr!=NULL ? ftpsvr:""),stemp);
#endif
	}
	curl_easy_setopt(RDAcurl,CURLOPT_URL,nam);
	curl_easy_setopt(RDAcurl,CURLOPT_FTP_USE_EPSV,epsv);
#ifdef USE_PROXY
	if(use_proxy)
	{
		sprintf(stemp,"%s:%d",(http_proxy!=NULL ? http_proxy:""),http_proxy_port);
		curl_easy_setopt(RDAcurl,CURLOPT_PROXY,(char *)stemp);
	}
#endif /* USE PROXY */
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
	/* curl_easy_setopt(RDAcurl,CURLOPT_VERBOSE,TRUE); */
	curl_easy_setopt(RDAcurl,CURLOPT_WRITEFUNCTION,getprogram);
	curl_easy_setopt(RDAcurl,CURLOPT_FILE,&plst);

	res=curl_easy_perform(RDAcurl);
	curl_easy_cleanup(RDAcurl);
	if(plst.stream) fclose(plst.stream);
	if(CURLE_OK!=res)
	{
		if(overpass==NULL) 
		{
			prterr("Error:  Failed to obtain data [%s] with error [%d].",stemp,res);
		} else {
			prterr("Error:  Failed to obtain data with error [%d].",res);
		}
	} else {
#ifndef WIN32
		dsp2use=RDA_GetEnv("DISPLAY");
		command=Rmalloc(RDAstrlen(stemp)+200);
#ifdef LINUX
/* Options to use more cores/threads "-m0=lzma2 -mmt6" */
		sprintf(command,"/usr/X11R6/bin/xterm -T \"DOFTPZIP\" -fn 10x24 -display %s -e 7za.lnx x -mmt=on %s",(dsp2use!=NULL ? dsp2use:""),(stemp!=NULL ? stemp:"")); 
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
/* Options to use more cores/threads "-m0=lzma2 -mmt6" */
		sprintf(command,"/usr/X11R6/bin/xterm -T \"DOFTPZIP\" -fn 10x24 -display %s -e 7za.lnx x -mmt=on %s",(dsp2use!=NULL ? dsp2use:""),(stemp!=NULL ? stemp:"")); 
#endif
#ifdef RS6000
		sprintf(command,"/usr/bin/X11/aixterm -ls -fn pc8x16 -T \"DOFTPZIP\" -d %s -e 7za.rsc x -mmt=on %s",(dsp2use!=NULL ? dsp2use:""),(stemp!=NULL ? stemp:"")); 
#endif
#ifdef SCO50
		sprintf(command,"/usr/bin/X11/scoterm -ls -fn pc8x16 -T \"DOFTPZIP\" -d %s -e 7za.sco x -mmt=on %s",(dsp2use!=NULL ? dsp2use:""),(stemp!=NULL ? stemp:"")); 
#endif
#ifdef SCO50P
		sprintf(command,"/usr/bin/X11/scoterm -ls -fn pc8x16 -T \"DOFTPZIP\" -d %s -e 7za.sco x -mmt=on %s",(dsp2use!=NULL ? dsp2use:""),(stemp!=NULL ? stemp:"")); 
#endif
#ifdef UNIXWARE21
		sprintf(command,"/usr/bin/X11/xterm -ls -fn pc8x16 -T \"DOFTPZIP\" -d %s -e 7za.unw x -mmt=on %s",(dsp2use!=NULL ? dsp2use:""),(stemp!=NULL ? stemp:"")); 
#endif
#ifdef UNIXWARE7
		sprintf(command,"/usr/bin/X11/xterm -ls -fn pc8x16 -T \"DOFTPZIP\" -d %s -e 7za.unw x -mmt=on %s",(dsp2use!=NULL ? dsp2use:""),(stemp!=NULL ? stemp:"")); 
#endif
#ifdef SUNX86
		sprintf(command,"/usr/X/bin/xterm -T \"DOFTPZIP\" -display %s -e 7za.sun x -mmt=on %s",(dsp2use!=NULL ? dsp2use:""),(stemp!=NULL ? stemp:"")); 
#endif
		system_command(command);
		if(command!=NULL) Rfree(command);
#endif
#ifdef WIN32
		args=APPlibNEW();
		addAPPlib(args,"x");
		addAPPlib(args,stemp);
		ADVExecute2Program("7zg",args,NULL);
		if(args!=NULL) freeapplib(args);
#endif
	}
	killwindow(r);
	quitfunc(r);
	if(nam!=NULL) Rfree(nam);

}
static char *GetArchiveName(char *module,char *xname)
{
	char *libx=NULL,*name=NULL;
	RDAGenericSetup *gsv=NULL;
	short namestyle=4;
	char *time=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("ARCHIVE")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"ARCHIVE");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"ARCHIVE");
#endif

	gsv=RDAGenericSetupNew("ARCHIVE","DEFAULT NAME STYLE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			namestyle=*gsv->value.short_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	name=Rmalloc(61);
	switch(namestyle)
	{
		case 0: /* YYYY-MM-DD.HH-MM.MODULE.PROCESS */
			time=GETCURRENTTIME();
			sprintf(name,"%4s-%.2s-%.2s.%.2s-%.2s.%s.%s",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],&time[0],&time[3],module,xname);
			if(time!=NULL) Rfree(time);
			break;
		case 1: /* YYYY-MM-DD.HH-MM.MODULE.PROCESS.USER */
			time=GETCURRENTTIME();
			sprintf(name,"%4s-%.2s-%.2s.%.2s-%.2s.%s.%s.%s",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],&time[0],&time[3],module,xname,USERLOGIN);
			if(time!=NULL) Rfree(time);
			break;
		case 2: /* YYYY-MM-DD.HH-MM.MODULE.USER.PROCESS */
			time=GETCURRENTTIME();
			sprintf(name,"%4s-%.2s-%.2s.%.2s-%.2s.%s.%s",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],&time[0],&time[3],module,USERLOGIN,xname);
			if(time!=NULL) Rfree(time);
			break;
		case 3: /* YYYY-MM-DD.HH-MM.USER.MODULE.PROCESS */
			time=GETCURRENTTIME();
			sprintf(name,"%4s-%.2s-%.2s.%.2s-%.2s.%s.%s.%s",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],&time[0],&time[3],USERLOGIN,module,xname);
			if(time!=NULL) Rfree(time);
			break;
		default:
		case 4: /* YYYY-MM-DD.HH-MM.USER.PROCESS */
			time=GETCURRENTTIME();
			sprintf(name,"%4s-%.2s-%.2s.%.2s-%.2s.%s.%s",&CURRENT_DATE10[6],&CURRENT_DATE10[0],&CURRENT_DATE10[3],&time[0],&time[3],USERLOGIN,xname);
			if(time!=NULL) Rfree(time);
			break;
	}
	return(name);
}
static void ArchiveBackup(char *name)
{
	char *aname=NULL,*desc=NULL,*fname=NULL;
	char *temp1=NULL,*c=NULL,*t=NULL,*wgdir=NULL;
	RDATData *prev=NULL;
	int aclass=41;
	char *subject=NULL;
	short s=5,a_fileno=(-1),archwork=(-1);
	int ret_int=0;
	
	a_fileno=APPOPNNRD("ARCHIVE","ARCHIVES",TRUE,TRUE);
	archwork=APPOPNNRD("ARCHIVE","ARCHWORK",TRUE,FALSE);
	if((a_fileno==(-1)) || (archwork==(-1)))
	{
		prterr("Error:  Unable to Archive Database files.");
		if(archwork!=(-1)) CLSNRD(archwork);
		if(a_fileno!=(-1)) CLSNRD(a_fileno);

		return;
	}
	aname=GetArchiveName("BACKUPS",name);
	temp1=adddashes(aname);
	ZERNRD(archwork);
	FINDFLDSETSTRING(archwork,"WORK GROUP","BACKUPS");
	if(EQLNRD(archwork,1)) KEYNRD(archwork,1);
	FINDFLDGETSTRING(archwork,"DIRECTORY",&wgdir);
	stoupper(temp1);

#ifndef WIN32
	sprintf(stemp,"ARCHIVE/.BACKUPS");
#endif
#ifdef WIN32
	sprintf(stemp,"ARCHIVE\\.BACKUPS");
#endif
	RDAmkdir(stemp,00770,&ret_int);

	memset(stemp,0,101);
#ifndef WIN32
	sprintf(stemp,"ARCHIVE/.BACKUPS/%s",temp1);
#endif
#ifdef WIN32
	sprintf(stemp,"ARCHIVE\\.BACKUPS\\%s",temp1);
#endif
	RDAMoveFile(name,stemp);
	
	ZERNRD(a_fileno);
	FINDFLDSETSTRING(a_fileno,"WORK GROUP","BACKUPS");
	FINDFLDSETSTRING(a_fileno,"ARCHIVE NAME",aname);
	if(!EQLNRD(a_fileno,1))
	{
		prev=RDATDataNEW(a_fileno);
	} else KEYNRD(a_fileno,1);
	FINDFLDSETCHAR(a_fileno,"DELETEFLAG",FALSE);
	FINDFLDSETSTRING(a_fileno,"WORK GROUP","BACKUPS");
	FINDFLDSETSTRING(a_fileno,"ARCHIVE NAME",aname);
	FINDFLDSETSTRING(a_fileno,"DESCRIPTION","DOFTPZIP BACKUP");
	FINDFLDSETSTRING(a_fileno,"SUBJECT","UTILITIES");
	FINDFLDSETSHORT(a_fileno,"SOURCE",s);
	FINDFLDSETSTRING(a_fileno,"SOURCE USER",USERLOGIN);
	FINDFLDSETSTRING(a_fileno,"FILENAME",fname);
	FINDFLDSETSTRING(a_fileno,"TYPE","7z");
	FINDFLDSETINT(a_fileno,"CLASS",aclass);
	c=GETCURRENTDATE10();
	FINDFLDSETSTRING(a_fileno,"DATE",c);
	if(c!=NULL) Rfree(c);
	t=GETCURRENTTIME();
	FINDFLDSETSTRING(a_fileno,"TIME",t);
	if(t!=NULL) Rfree(t);
	WRTTRANS(a_fileno,0,NULL,prev);
	if(prev!=NULL) FreeRDATData(prev);	
	if(fname!=NULL) Rfree(fname);
	if(desc!=NULL) Rfree(desc);
	if(aname!=NULL) Rfree(aname);
	if(subject!=NULL) Rfree(subject);
	if(a_fileno!=(-1)) CLSNRD(a_fileno);
	if(archwork!=(-1)) CLSNRD(archwork);
	if(temp1!=NULL) Rfree(temp1);
}
static void doftpzipput(RDArsrc *r)
{
#ifdef WIN32
	APPlib *args=NULL;
#endif
#ifndef WIN32
	char *command=NULL,*dsp2use=NULL;
#endif
	char *nam=NULL,*tempstr=NULL;
	CURLcode res=0;
	FILE *fp=NULL;
	struct stat file_info;
	int hd=0;
	long epsv=0;
	
	readallwidgets(r);
	FINDRSCGETSTRING(r,"DRIVE SPEC",&drvspc);
	FINDRSCGETSTRING(r,"FTP SERVER",&ftpsvr);
	FINDRSCGETCHAR(r,"FTP USE PROXY",&use_proxy);	
	FINDRSCGETSTRING(r,"FTP PROXY",&http_proxy);
	FINDRSCGETINT(r,"FTP PROXY PORT",&http_proxy_port);
	FINDRSCGETSTRING(r,"OVERRIDE PASSWORD",&overpass);
	sprintf(stemp,"%s.7z",(drvspc!=NULL ? drvspc:""));
	unlink(stemp);
#ifndef WIN32
	dsp2use=RDA_GetEnv("DISPLAY");
	command=Rmalloc(RDAstrlen(stemp)+200);
#ifdef LINUX
	sprintf(command,"/usr/X11R6/bin/xterm -T \"DOFTPZIP\" -fn 10x24 -display %s -e 7za.lnx a -t7z -mmt=on %s %s",(dsp2use!=NULL ? dsp2use:""),(stemp!=NULL ? stemp:""),"."); 
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
	sprintf(command,"/usr/X11R6/bin/xterm -T \"DOFTPZIP\" -fn 10x24 -display %s -e 7za.lnx a -t7z -mmt=on %s %s",(dsp2use!=NULL ? dsp2use:""),(stemp!=NULL ? stemp:""),"."); 
#endif
#ifdef RS6000
	sprintf(command,"/usr/bin/X11/aixterm -ls -fn pc8x16 -T \"DOFTPZIP\" -d %s -e 7za.rsc a -t7z -mmt=on %s %s",(dsp2use!=NULL ? dsp2use:""),(stemp!=NULL ? stemp:""),"."); 
#endif
#ifdef SCO50
	sprintf(command,"/usr/bin/X11/scoterm -ls -fn pc8x16 -T \"DOFTPZIP\" -d %s -e 7za.sco a -t7z -mmt=on %s %s",(dsp2use!=NULL ? dsp2use:""),(stemp!=NULL ? stemp:""),"."); 
#endif
#ifdef SCO50P
	sprintf(command,"/usr/bin/X11/scoterm -ls -fn pc8x16 -T \"DOFTPZIP\" -d %s -e 7za.sco a -t7z -mmt=on %s %s",(dsp2use!=NULL ? dsp2use:""),(stemp!=NULL ? stemp:""),"."); 
#endif
#ifdef UNIXWARE21
	sprintf(command,"/usr/bin/X11/xterm -ls -fn pc8x16 -T \"DOFTPZIP\" -d %s -e 7za.unw a -t7z -mmt=on %s %s",(dsp2use!=NULL ? dsp2use:""),(stemp!=NULL ? stemp:""),"."); 
#endif
#ifdef UNIXWARE7
	sprintf(command,"/usr/bin/X11/xterm -ls -fn pc8x16 -T \"DOFTPZIP\" -d %s -e 7za.unw a -t7z -mmt=on %s %s",(dsp2use!=NULL ? dsp2use:""),(stemp!=NULL ? stemp:""),"."); 
#endif
#ifdef SUNX86
	sprintf(command,"/usr/X/bin/xterm -T \"DOFTPZIP\" -display %s -e 7za.sun a -t7z -mmt=on %s %s",(dsp2use!=NULL ? dsp2use:""),(stemp!=NULL ? stemp:""),"."); 
#endif
	system_command(command);
	if(command!=NULL) Rfree(command);
#endif
#ifdef WIN32
	args=APPlibNEW();
	addAPPlib(args,"a");
	addAPPlib(args,"-t7z");
	addAPPlib(args,"-mmt=on");
	addAPPlib(args,"-ssw");
	addAPPlib(args,drvspc);
	addAPPlib(args,"-xr-!*.exe");
	addAPPlib(args,"-xr-!*.dll");
	addAPPlib(args,".");
	ADVExecute2Program("7zg",args,NULL);
	if(args!=NULL) freeapplib(args);
#endif

	hd=open(stemp,O_RDONLY);
	if(hd!=(-1))
	{
		fstat(hd,&file_info);
		close(hd);
	}
	fp=fopen(stemp,"rb");
	if(fp!=NULL)
	{
		RDAcurl=curl_easy_init();
		nam=Rmalloc(RDAstrlen(ftpsvr)+RDAstrlen(stemp)+512);
		if(XPERT_SETUP->software_type<2)
		{
#if defined(LINUX2_2) || defined(UBUNTU_OS)
			sprintf(nam,"ftp://%s/%s",(ftpsvr!=NULL ? ftpsvr:""),stemp);
#endif
#ifdef WIN32
			sprintf(nam,"ftp://%s/%s",(ftpsvr!=NULL ? ftpsvr:""),stemp);
#endif
		} else {
#if defined(LINUX2_2) || defined(UBUNTU_OS)
			sprintf(nam,"ftp://%s/%s",(ftpsvr!=NULL ? ftpsvr:""),stemp);
#endif
#ifdef WIN32
			sprintf(nam,"ftp://%s/%s",(ftpsvr!=NULL ? ftpsvr:""),stemp);
#endif
		}
		curl_easy_setopt(RDAcurl,CURLOPT_URL,nam);
		curl_easy_setopt(RDAcurl,CURLOPT_FTP_USE_EPSV,epsv);
#ifdef USE_PROXY
		if(use_proxy)
		{
			sprintf(stemp,"%s:%d",(http_proxy!=NULL ? http_proxy:""),http_proxy_port);
			curl_easy_setopt(RDAcurl,CURLOPT_PROXY,(char *)stemp);
		}
#endif /* USE PROXY */
		if(XPERT_SETUP->software_type<2)
		{
			curl_easy_setopt(RDAcurl,CURLOPT_USERPWD,"xpert:e2giVcYo");
		} else {
			curl_easy_setopt(RDAcurl,CURLOPT_USERPWD,"mbgui:m8KreBj5");
		}
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
		/* curl_easy_setopt(RDAcurl,CURLOPT_VERBOSE,TRUE);  */
		curl_easy_setopt(RDAcurl,CURLOPT_UPLOAD,TRUE);
		curl_easy_setopt(RDAcurl,CURLOPT_INFILE,fp);
		curl_easy_setopt(RDAcurl,CURLOPT_INFILESIZE,file_info.st_size);
	
		res=curl_easy_perform(RDAcurl);
		curl_easy_cleanup(RDAcurl);
		if(CURLE_OK!=res)
		{
			
			tempstr=Rmalloc(200);
			sprintf(tempstr,"Error:  Failed to FTP PUT Archived Data on the Server With Error [%d].  Please Contact Support.",res);

			ERRORDIALOG("FILE TRANSFER FAILED.",tempstr,NULL,TRUE);
			if(tempstr!=NULL) Rfree(tempstr);

			prterr("Error:  Failed to FTP PUT Archived data on the server with [%d].",res);
		}
		if(nam!=NULL) Rfree(nam);
	} else {
		prterr("Error:  Couldn't open the 7z file for reading.");
	}
	if(XPERT_SETUP->ARCHIVE==TRUE && ARCHIVE_BACKUPS==TRUE)
	{
		tempstr=stralloc(stemp);
		ArchiveBackup(tempstr);
		if(tempstr!=NULL) Rfree(tempstr);
	}
/* This should not delete the file in case of error. 
 * That prevents us from transferring it manually.
	} else {
		unlink(stemp);
	}
*/
	killwindow(r);
	quitfunc(r);
}
static void doftpzip(RDArsrc *r)
{
	int which=0;

	FINDRSCGETINT(r,"WHICH",&which);
	switch(which)
	{
		default:
		case 0:	
			doftpzipput(r);
			break;
		case 1:
			doftpzipget(r);	
			break;
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
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	int x=0;
	RDAGenericSetup *gsv=NULL;
	char *libx=NULL;

	if(InitializeSubsystems(argc,argv,module,"DOFTPZIP")) 
	{
		return;
	}
	curl_global_init(CURL_GLOBAL_DEFAULT);	
	if(XPERT_SETUP->ARCHIVE==TRUE)
	{
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("ARCHIVE")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"ARCHIVE");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"ARCHIVE");
#endif

	gsv=RDAGenericSetupNew("ARCHIVE","ARCHIVE BACKUPS");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			ARCHIVE_BACKUPS=*gsv->value.string_value;
		} else {
			ARCHIVE_BACKUPS=FALSE;
		}
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	} else ARCHIVE_BACKUPS=FALSE;
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
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
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
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
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
	ftpsvr=stralloc("support-srv.rdasys.com");
	drvspc=adddashes(XPERT_SETUP->company_name);
	mainrsrc=RDArsrcNEW(module,"DOFTPZIP");
	addstdrsrc(mainrsrc,"FTP SERVER",VARIABLETEXT,0,ftpsvr,TRUE);
	addstdrsrc(mainrsrc,"OVERRIDE PASSWORD",PLAINTEXT,60,NULL,TRUE);
#ifdef USE_PROXY
	addstdrsrc(mainrsrc,"FTP USE PROXY",BOOLNS,1,&use_proxy,TRUE);
	addstdrsrc(mainrsrc,"FTP PROXY",VARIABLETEXT,0,http_proxy,TRUE);
	addstdrsrc(mainrsrc,"FTP PROXY PORT",LONGV,4,&http_proxy_port,TRUE);
#endif /* USE PROXY */
	addstdrsrc(mainrsrc,"DRIVE SPEC",VARIABLETEXT,0,drvspc,TRUE);
	whichs=APPlibNEW();
	addAPPlib(whichs,"Put 7z");
	addAPPlib(whichs,"Get 7z");
	addlstrsrc(mainrsrc,"WHICH",&x,TRUE,NULL,whichs->numlibs,&whichs->libs,NULL);
	
	DefaultScreens(mainrsrc);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,doftpzip,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
#ifdef USE_PROXY
	FINDRSCSETFUNC(mainrsrc,"FTP USE PROXY",setproxy,NULL);
	setproxy(mainrsrc);
#endif /* USE_PROXY */
	mainrsrc->scn=RDAscrnNEW("DATABASE","DOFTPZIP");
	if(mainrsrc->scn!=NULL)
	{
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
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,5,"","URL Example: support-srv.rdasys.com","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,5,"","URL:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,5,"","7z:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,0,"FTP SERVER","Ftp Server","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,0,"DRIVE SPEC","Drive Spec","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","Override Password:","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,0,"OVERRIDE PASSWORD","Override Password","","",0,60,3,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,5,"","            Which:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,7,"WHICH","Which","","",1,0,0,NULL,"([DEV_LICENSE]=TRUE)","([DEV_LICENSE]=TRUE)",NULL);
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,6,"SELECT","Select","","",0,0,25,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,6,"DEFAULTS","Save Defaults","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	}
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
}
