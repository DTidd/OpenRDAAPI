/*LASER - Program to Test Laser Forms*/
#ifndef WIN32
#define __NAM__ "getforms.lnx"
#endif
#ifdef WIN32
#define __NAM__ "getforms.exe"
#endif
#include <app.hpp>
#include <mix.hpp>
#include <mkmsc.hpp>
#include <curl/curl.h>
#include <curl/easy.h>
/*ADDLIB curl */
/*ADDLIB mklib */
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

struct FtpFile {
	char *filename;
	FILE *stream;
};


char *module="UTILITIES";
char *devlicense=NULL,download_dirdep=TRUE,*ftpsvr=NULL,*devicex=NULL;
char download_payvwmicr=FALSE,download_paymicr=FALSE,download_venmicr=FALSE,download_941=FALSE;
char download_prtapo=FALSE,download_bftrecap=FALSE;
short dev_license=FALSE;
CURL *RDAcurl=NULL;
static char *overpass=NULL;
#define USE_PROXY
#ifdef USE_PROXY
char use_proxy=FALSE,*http_proxy=NULL;
int http_proxy_port=1080;
#endif /* USE PROXY */

int getprogram(void *buffer,size_t size,size_t nmemb,void *stream)
{
	struct FtpFile *out=(struct FtpFile *)stream;

	if(out && !out->stream)
	{
		unlink(out->filename);
		out->stream=fopen(out->filename,"wb");
		if(!out->stream) return(-1);
	}
	return(fwrite(buffer,size,nmemb,out->stream));
}
static void getForm(short which)
{
	char *nam=NULL,*temp=NULL,*namx=NULL,stemp1[512];
	CURLcode res=0;
	struct FtpFile plst={NULL,NULL};
	long epsv=0;
	APPlib *args=NULL;


	RDAcurl=curl_easy_init();
	switch(which)
	{
		prterr("Which download = %d", which);
		default:
		case 1:
			temp=stralloc("dirdepform.7z");
			break;
		case 2:
			temp=stralloc("dirdepforma.7z");
			break;
		case 3:
			temp=stralloc("pay.hppeckmicr.7z");
			break;
		case 4:
			temp=stralloc("pay.hppeckmicra.7z");
			break;
		case 5:
			temp=stralloc("vencheckmicr.7z");
			break;
		case 6:
			temp=stralloc("vencheckmicra.7z");
			break;
		case 7:
			temp=stralloc("pay941.7z");
			break;
		case 8:
			temp=stralloc("benefitrecap.7z");
			break;
		case 9:
			temp=stralloc("prtapoform.7z");
			break;
		case 10:
			temp=stralloc("payvwcheckmicr.7z");
			break;
		case 11:
			temp=stralloc("payvwcheckmicra.7z");
			break;
	}
	nam=Rmalloc(RDAstrlen(ftpsvr)+RDAstrlen(temp)+512);
	if(plst.filename!=NULL) Rfree(plst.filename);
	plst.filename=stralloc(temp);
	plst.stream=NULL;
	sprintf(nam,"ftp://%s/./forms/%s",(ftpsvr!=NULL ? ftpsvr:""),temp);
	/*prterr("Name is %s \n",nam);*/
	if(temp!=NULL) Rfree(temp);
	curl_easy_setopt(RDAcurl,CURLOPT_URL,nam);
	curl_easy_setopt(RDAcurl,CURLOPT_FTP_USE_EPSV,epsv);
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
#ifdef USE_PROXY
	if(use_proxy)
	{
		sprintf(stemp,"%s:%d",(http_proxy!=NULL ? http_proxy:""),http_proxy_port);
		curl_easy_setopt(RDAcurl,CURLOPT_PROXY,(char *)stemp);
	}
#endif /* USE PROXY */
	
	/* curl_easy_setopt(RDAcurl,CURLOPT_VERBOSE,FALSE); */
	curl_easy_setopt(RDAcurl,CURLOPT_WRITEFUNCTION,getprogram);
	curl_easy_setopt(RDAcurl,CURLOPT_FILE,&plst);

	res=curl_easy_perform(RDAcurl);
	curl_easy_cleanup(RDAcurl);
	if(plst.stream) fclose(plst.stream);
	namx=stralloc(plst.filename);
	if(( (!strncmp(&namx[RDAstrlen(namx)-3],".7z",3)) || (!strncmp(&namx[RDAstrlen(namx)-4],".zip",4))))
	{
#ifndef WIN32
		RDA_NOGUI=1;
		args=APPlibNEW();
		addAPPlib(args,"x");
		addAPPlib(args,"-aoa");
		memset(stemp1,0,512);
		sprintf(stemp1,"-o%s/",CURRENTDIRECTORY);
		addAPPlib(args,stemp1);
		addAPPlib(args,plst.filename);
		ADVExecute2Program("7za",args,NULL);
		if(args!=NULL) freeapplib(args);
		unlink(plst.filename);
		RDA_NOGUI=0;

#endif
#ifdef WIN32
		args=APPlibNEW();
		addAPPlib(args,"x");
		addAPPlib(args,"-aoa");
		memset(stemp1,0,512);
		sprintf(stemp1,"-o%s/",CURRENTDIRECTORY);
		addAPPlib(args,stemp1);
		addAPPlib(args,plst.filename);
		ADVExecute2Program("7zg",args,NULL);
		if(args!=NULL) freeapplib(args);
		unlink(plst.filename);
#endif
	}
	if(namx!=NULL) Rfree(namx);
	if(CURLE_OK!=res)
	{
		prterr("Error:  Failed to obtain program [%s] with error [%d].",plst.filename,res);
	}
	if(nam!=NULL) Rfree(nam);
	if(plst.filename!=NULL) Rfree(plst.filename);
}
static void SCN_LASER_FORM_TEST()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","GET LASER FORMS");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,23,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Instructions","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Use this tool to download decorative forms and setup a laser Printer to use them.","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Get Forms","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Development License:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DEV_LICENSE","Development License","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,11,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","DOWNLOAD WHICH FORMS:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"DOWNLOAD DIRDEP","Download Direct Deposit","","",0,00,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"DOWNLOAD PAYMICR","Download Payroll Decorated Stub","","",0,00,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"DOWNLOAD VENMICR","Download Vendor Decorated Stub","","",0,00,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"DOWNLOAD 941","Download 941","","",0,00,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"DOWNLOAD BENEFIT RECAP","Download Benefit Recap","","",0,00,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"DOWNLOAD PRTAPO","Download Purchase Order","","",0,00,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"DOWNLOAD PAYVWMICR","Download Payroll VW Decorated Stub","","",0,00,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,11,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"FALSE","","","");
		ADVaddwdgt(scrn,5,"","Setup Output Device:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"LOAD DEVICE NAME","Load Device Name:","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"DEVICE","Device","","",0,40,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,11,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","  Override Password:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"OVERRIDE PASSWORD","Override Password","","",0,60,3,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,11,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"FTP USE PROXY","Use Proxy","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,""," PROXY:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FTP PROXY","Proxy","","",0,40,0,"","","","");
		ADVaddwdgt(scrn,5,"","PORT:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FTP PROXY PORT","Port","","",0,4,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,11,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","       URL Examples: ftp.rdasys.com or 66.135.38.166","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","                URL:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FTP SERVER","Ftp Server","","",0,60,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,25,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Quit","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,17,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [GET LASER FORMS], Can Not Save Maintain Master!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
}
static void quitx(RDArsrc *r)
{
	if(devicex!=NULL) Rfree(devicex);
	if(r!=NULL) 
	{
		killwindow(r);
		free_rsrc(r);
	}
	ShutdownSubsystems();
}
static void selectx(RDArsrc *r)
{
	RDApdev *tdev=NULL;
	char *p=NULL;

	readallwidgets(r);
	memset(stemp,0,101);
	sprintf(stemp,"TXT2PS");
	tdev=RDApdevNEW(stemp);
	if(!getpdevbin(tdev))
	{
		if(tdev->name!=NULL) Rfree(tdev->name);
		tdev->name=stralloc(devicex);
		tdev->active=TRUE;
		writepdevbin(tdev);
	} else {
		ERRORDIALOG("TXT2PS NOT FOUND","The RPTGEN Updates (mkrpt) have not be executed or the PRTDEV.STP is corrupted.  Submit a support request for assistance.",NULL,FALSE);
	}
	if(tdev!=NULL) free_pdev(tdev);
	FINDRSCGETSTRING(r,"DEVICE",&p);
	if(!isEMPTY(p))
	{
		memset(stemp,0,101);
		sprintf(stemp,"HP LASER 4000n");
		tdev=RDApdevNEW(stemp);
		if(!getpdevbin(tdev))
		{
			if(tdev->name!=NULL) Rfree(tdev->name);
			tdev->name=stralloc(p);
			tdev->active=TRUE;
			writepdevbin(tdev);
		} else {
			ERRORDIALOG("TXT2PS NOT FOUND","The RPTGEN Updates (mkrpt) have not be executed or the PRTDEV.STP is corrupted.  Submit a support request for assistance.",NULL,FALSE);
		}
		if(p!=NULL) Rfree(p);
	}
	if(tdev!=NULL) free_pdev(tdev);
	FINDRSCGETCHAR(r,"DOWNLOAD DIRDEP",&download_dirdep);
	FINDRSCGETCHAR(r,"DOWNLOAD PAYMICR",&download_paymicr);
	FINDRSCGETCHAR(r,"DOWNLOAD PAYVWMICR",&download_payvwmicr);
	FINDRSCGETCHAR(r,"DOWNLOAD VENMICR",&download_venmicr);
	FINDRSCGETCHAR(r,"DOWNLOAD 941",&download_941);
	FINDRSCGETCHAR(r,"DOWNLOAD BENEFIT RECAP",&download_bftrecap);
	FINDRSCGETCHAR(r,"DOWNLOAD PRTAPO",&download_prtapo);
	FINDRSCGETSTRING(r,"OVERRIDE PASSWORD",&overpass);
	/*prterr(" 4==>> download_payvwmicr = %d \n", download_payvwmicr);*/
	if(download_dirdep) 
	{
		getForm(1);
		getForm(2);
	}
	if(download_paymicr) 
	{
		getForm(3);
		getForm(4);
	}
	if(download_venmicr) 
	{
		getForm(5);
		getForm(6);
	}
	if(download_941) 
	{
		getForm(7);
	}
	if(download_bftrecap)
	{
		getForm(8);
	} 
	if(download_prtapo)
	{
		getForm(9);
	}
	if(download_payvwmicr) 
	{
		getForm(10);
		getForm(11);
	}
}
static void setdevlicense(RDArsrc *r)
{
	readwidget(r,"DEV_LICENSE");
	FINDRSCGETSTRING(r,"DEV_LICENSE",&devlicense);
	if(CheckDevLicense(devlicense))
	{
		dev_license=TRUE;
	} else {
		dev_license=FALSE;
	}
	FINDRSCSETEDITABLE(r,"SELECT",(dev_license==TRUE ? TRUE:FALSE));
	FINDRSCSETSENSITIVE(r,"SELECT",(dev_license==TRUE ? TRUE:FALSE));
	updateallrsrc(r);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	char *temp1=NULL,*tmp=NULL,*defaultprinter=NULL;

	if(InitializeSubsystems(argc,argv,"UTILITIES","GET LASER FORMS")) 
	{
		return(1);
	}
	tmp=RDA_GetEnv("DEV_LICENSE");
	if(tmp!=NULL)
	{
		temp1=strtok(tmp," ");
		devlicense=stralloc(temp1);
		if(CheckDevLicense(devlicense))
		{
			dev_license=TRUE;
		} else {
			dev_license=FALSE;
		}
	}
	SCN_LASER_FORM_TEST();
	mainrsrc=RDArsrcNEW("UTILITIES","GET LASER FORMS");
	addstdrsrc(mainrsrc,"DEV_LICENSE",VARIABLETEXT,0,devlicense,TRUE);
	FINDRSCSETFUNC(mainrsrc,"DEV_LICENSE",setdevlicense,NULL);
	addstdrsrc(mainrsrc,"OVERRIDE PASSWORD",PLAINTEXT,60,NULL,TRUE);
	addstdrsrc(mainrsrc,"DOWNLOAD DIRDEP",BOOLNS,1,&download_dirdep,TRUE);
	addstdrsrc(mainrsrc,"DOWNLOAD PAYMICR",BOOLNS,1,&download_paymicr,TRUE);
	addstdrsrc(mainrsrc,"DOWNLOAD PAYVWMICR",BOOLNS,1,&download_payvwmicr,TRUE);
	addstdrsrc(mainrsrc,"DOWNLOAD PAYVMICR",BOOLNS,1,&download_payvwmicr,TRUE);
	addstdrsrc(mainrsrc,"DOWNLOAD VENMICR",BOOLNS,1,&download_venmicr,TRUE);
	addstdrsrc(mainrsrc,"DOWNLOAD PRTAPO",BOOLNS,1,&download_prtapo,TRUE);
	addstdrsrc(mainrsrc,"DOWNLOAD 941",BOOLNS,1,&download_941,TRUE);
	addstdrsrc(mainrsrc,"DOWNLOAD BENEFIT RECAP",BOOLNS,1,&download_bftrecap,TRUE);
	addstdrsrc(mainrsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	addbtnrsrc(mainrsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	ftpsvr=stralloc("66.135.38.166");
	addstdrsrc(mainrsrc,"FTP SERVER",VARIABLETEXT,0,ftpsvr,TRUE);
#ifdef USE_PROXY
	addstdrsrc(mainrsrc,"FTP USE PROXY",BOOLNS,1,&use_proxy,TRUE);
	addstdrsrc(mainrsrc,"FTP PROXY",VARIABLETEXT,0,http_proxy,TRUE);
	addstdrsrc(mainrsrc,"FTP PROXY PORT",LONGV,4,&http_proxy_port,TRUE);
#endif /* USE PROXY */
#ifndef WIN32
	devicex=stralloc("viewpdf.lnx");
#else
	devicex=stralloc("viewpdf.exe");
#endif
	addbtnrsrc(mainrsrc,"SELECT",TRUE,selectx,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitx,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,quitx,NULL,TRUE);
	setdevlicense(mainrsrc);
	return(0);
}
