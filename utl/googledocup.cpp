/* googledocup.c - Upload to Google Documents */
/*lint -library */
#include <cstdio>
#include <sys/stat.h>
#ifndef WIN32
#define __NAM__ "googledocup.lnx"
#endif
#ifdef WIN32
#define __NAM__ "googledocup.exe"
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

char *module="ARCHIVE",*drvspc=NULL,*ftpsvr=NULL;
char *logname=NULL,*pass_word=NULL;
CURL *RDAcurl=NULL;
char use_proxy=FALSE,*http_proxy=NULL;
int http_proxy_port=1080;

struct FtpFile {
	char *filename;
	FILE *stream;
};
static void quitfunc(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	if(drvspc!=NULL) Rfree(drvspc);
	if(ftpsvr!=NULL) Rfree(ftpsvr);
	curl_global_cleanup();
	ShutdownSubsystems();
	std::exit;
}
static void googledocput(RDArsrc *r)
{
#ifdef WIN32
#endif
#ifndef WIN32
#endif
	char *nam=NULL,*tempstr=NULL;
	CURLcode res=0;
	FILE *fp=NULL;
	struct stat file_info;
	int hd=0;
	long epsv=0;
	
	readallwidgets(r);
	FINDRSCGETSTRING(r,"DRIVE SPEC",&drvspc);
	FINDRSCGETSTRING(r,"SERVER",&ftpsvr);
	FINDRSCGETCHAR(r,"USE PROXY",&use_proxy);	
	FINDRSCGETSTRING(r,"PROXY",&http_proxy);
	FINDRSCGETINT(r,"PROXY PORT",&http_proxy_port);

	memset(stemp,0,101);
	sprintf(stemp,"%s",drvspc);
	hd=open(stemp,O_RDONLY);
	if(hd!=(-1))
	{
		fstat(hd,&file_info);
		close(hd);
	}
	fprintf(RDA_STDERR,"stemp [%s] filesize [%d] ",stemp,file_info.st_size);TRACE;
	fp=fopen(stemp,"rb");
	if(fp!=NULL)
	{
		RDAcurl=curl_easy_init();
		curl_easy_setopt(RDAcurl,CURLOPT_URL,ftpsvr);
		curl_easy_setopt(RDAcurl,CURLOPT_FTP_USE_EPSV,epsv);
		if(use_proxy)
		{
			sprintf(stemp,"%s:%d",(http_proxy!=NULL ? http_proxy:""),http_proxy_port);
			curl_easy_setopt(RDAcurl,CURLOPT_PROXY,(char *)stemp);
		}
		tempstr=Rmalloc((RDAstrlen(logname)+RDAstrlen(pass_word)+5));
		sprintf(tempstr,"%s:%s",logname,pass_word);
		curl_easy_setopt(RDAcurl,CURLOPT_USERPWD,tempstr);
		curl_easy_setopt(RDAcurl,CURLOPT_VERBOSE,TRUE);
		curl_easy_setopt(RDAcurl,CURLOPT_UPLOAD,TRUE);
		curl_easy_setopt(RDAcurl,CURLOPT_FOLLOWLOCATION,TRUE);
		curl_easy_setopt(RDAcurl,CURLOPT_INFILE,fp);
		curl_easy_setopt(RDAcurl,CURLOPT_INFILESIZE,file_info.st_size);
		curl_easy_setopt(RDAcurl,CURLOPT_VERBOSE,TRUE);
	
		res=curl_easy_perform(RDAcurl);
		curl_easy_cleanup(RDAcurl);
		if(CURLE_OK!=res)
		{
			
			tempstr=Rmalloc(200);
			sprintf(tempstr,"Error:  Failed to PUT Archived Data on the Server With Error [%d].  Please Contact Support.",res);

			ERRORDIALOG("FILE TRANSFER FAILED.",tempstr,NULL,TRUE);
			if(tempstr!=NULL) Rfree(tempstr);

			prterr("Error:  Failed to PUT Archived data on the server with [%d].",res);
		}
		if(nam!=NULL) Rfree(nam);
	} else {
		prterr("Error:  Couldn't open the file for reading.");
	}
	killwindow(r);
	quitfunc(r);
}
static void setproxy(RDArsrc *r)
{
	readwidget(r,"USE PROXY");
	FINDRSCGETCHAR(r,"USE PROXY",&use_proxy);
	if(use_proxy==TRUE)
	{
		FINDRSCSETEDITABLE(r,"PROXY",TRUE);
		FINDRSCSETEDITABLE(r,"PROXY PORT",TRUE);
		FINDRSCSETSENSITIVE(r,"PROXY",TRUE);
		FINDRSCSETSENSITIVE(r,"PROXY PORT",TRUE);
	} else {
		FINDRSCSETEDITABLE(r,"PROXY",FALSE);
		FINDRSCSETEDITABLE(r,"PROXY PORT",FALSE);
		FINDRSCSETSENSITIVE(r,"PROXY",FALSE);
		FINDRSCSETSENSITIVE(r,"PROXY PORT",FALSE);
	}
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	RDAGenericSetup *gsv=NULL;
	char *libx=NULL,*desc=NULL;

	if(InitializeSubsystems(argc,argv,module,"DOFTPZIP")) 
	{
		return;
	}
	curl_global_init(CURL_GLOBAL_ALL);	
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("UTILITIES")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif

	gsv=RDAGenericSetupNew("UTILITIES","USE PROXY");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			use_proxy=*gsv->value.string_value;
		} else {
			use_proxy=FALSE;
		}
	}
	gsv=RDAGenericSetupNew("UTILITIES","PROXY");
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
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
	gsv=RDAGenericSetupNew("UTILITIES","PROXY PORT");
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
	desc=RDA_GetEnv("DEV_LICENSE");
	if(desc!=NULL)
	{
		ftpsvr=stralloc("https://drive.google.com/a/rdasys.com/#my-drive");
	} else {
		ftpsvr=stralloc("https://drive.google.com/#my-drive");
	}
	if(argc>1)
	{
		drvspc=adddashes(argv[1]);
	}
	mainrsrc=RDArsrcNEW(module,"UPLOAD TO GOOGLE DRIVE");
	addstdrsrc(mainrsrc,"SERVER",VARIABLETEXT,0,ftpsvr,TRUE);
	addstdrsrc(mainrsrc,"USE PROXY",BOOLNS,1,&use_proxy,TRUE);
	addstdrsrc(mainrsrc,"PROXY",VARIABLETEXT,0,http_proxy,TRUE);
	addstdrsrc(mainrsrc,"PROXY PORT",LONGV,4,&http_proxy_port,TRUE);
	addstdrsrc(mainrsrc,"DRIVE SPEC",VARIABLETEXT,0,drvspc,TRUE);
	addstdrsrc(mainrsrc,"LOGIN",VARIABLETEXT,0,logname,TRUE);
	addstdrsrc(mainrsrc,"PASSWORD",VARIABLETEXT,0,pass_word,TRUE);
	DefaultScreens(mainrsrc);
	FINDRSCSETSTRING(mainrsrc,"DRIVE SPEC",drvspc);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,googledocput,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	FINDRSCSETFUNC(mainrsrc,"USE PROXY",setproxy,NULL);
	setproxy(mainrsrc);
	FINDRSCSETINPUTFOCUS(mainrsrc,"LOGIN");
	mainrsrc->scn=RDAscrnNEW("ARCHIVE","UPLOAD TO GOOGLE DRIVE");
	if(mainrsrc->scn!=NULL)
	{
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,9,"USE PROXY","Use Proxy","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","PROXY:","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,0,"PROXY","Proxy","","",0,40,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","PORT:","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,0,"PROXY PORT","Port","","",0,4,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,11,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,5,"","https://drive.google.com/?tab=mo&authuser=0&pli=1#my-drive","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"([DEV_LICENSE]=TRUE)",NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,5,"","https://drive.google.com/a/rdasys.com/#my-drive","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,5,"","         URL:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,0,"SERVER","Google Drive","","",0,80,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,5,"","    Filename:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,0,"DRIVE SPEC","Drive Spec","","",0,80,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,11,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","Google Login:","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,0,"LOGIN","Login Identification","","",0,60,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","[username@gmail.com]","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","    Password:","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,0,"PASSWORD","Password","","",0,60,3,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
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
