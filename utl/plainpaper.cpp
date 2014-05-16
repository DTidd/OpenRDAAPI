/*LASER - Program to Test Laser Forms*/
#ifndef WIN32
#define __NAM__ "plainpaper.lnx"
#endif
#ifdef WIN32
#define __NAM__ "plainpaper.exe"
#endif
#include <app.hpp>
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
char *devlicense=NULL,download_form=TRUE,*ftpsvr=NULL,*devicex=NULL;
short dev_license=FALSE;
CURL *RDAcurl=NULL;
static char *overpass=NULL;
#define USE_PROXY
#ifdef USE_PROXY
char use_proxy=FALSE,*http_proxy=NULL;
int http_proxy_port=1080;
#endif /* USE PROXY */

size_t getprogram(void *buffer,size_t size,size_t nmemb,void *stream)
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
static void getForm()
{
	char *nam=NULL,*temp=NULL,*namx=NULL,stemp1[512];
	CURLcode res=0;
	struct FtpFile plst={NULL,NULL};
	long epsv=0;
	APPlib *args=NULL;


	RDAcurl=curl_easy_init();
	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+25);
	sprintf(temp,"%s/w2forms.7z",CURRENTDIRECTORY);
	nam=Rmalloc(RDAstrlen(ftpsvr)+RDAstrlen(temp)+512);
	if(plst.filename!=NULL) Rfree(plst.filename);
	plst.filename=stralloc(temp);
	plst.stream=NULL;
	sprintf(nam,"ftp://%s/./forms/w2forms.7z",(ftpsvr!=NULL ? ftpsvr:""));
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
	
	curl_easy_setopt(RDAcurl,CURLOPT_VERBOSE,FALSE); 
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
		sprintf(stemp1,"-o%s/forms/",CURRENTDIRECTORY);
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
		sprintf(stemp1,"-o%s/forms/",CURRENTDIRECTORY);
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

	scrn=RDAscrnNEW("UTILITIES","PLAINPAPER TEST");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,23,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Instructions","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","THE PLAIN PAPER TEST IS ONLY APPLICABLE TO CUSTOMERS WHO HAVE PURCHASED","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","THE CAPABILITY TO PRINT THE ENTIRE W2 FORM ON PLAIN PAPER,","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","AS WELL AS TO CUSTOMERS WHO HAVE EMPLOYEE SELF SERVE (ESS/PIM).","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","If your organization does not have this option, skip this process.","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Otherwise, follow the steps outlined below:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Step #1:  Ensure you have obtained the latest programs.","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Step #2:  Ensure you have loaded your updates especially for RPTGEN and PAYROLL.","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Step #3:  For customers who have purchased this option, contact RDA","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","          support to obtain today's Development License.  The ","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","          Development License is NOT required for customers with ESS/PIM.","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Step #4:  Complete the Plain Paper Test successfully on adjoining tab.","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Plain Paper Test","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Development License:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DEV_LICENSE","Development License","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","(Not required for ESS/PIM Customers)","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"DOWNLOAD FORM","Download W2 Forms","","",0,00,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
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
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [PLAINPAPER TEST], Can Not Save Maintain Master!");
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
	exit(0);
}
static void selectx(RDArsrc *r)
{
	char *prtmessage=NULL;
	RDA_PFILE *fp1=NULL;
	RDApdev *tdev=NULL;

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
	readwidget(r,"DOWNLOAD FORM");
	FINDRSCGETCHAR(r,"DOWNLOAD FORM",&download_form);
	readwidget(r,"OVERRIDE PASSWORD");
	FINDRSCGETSTRING(r,"OVERRIDE PASSWORD",&overpass);
	if(download_form) getForm();
	if(!isEMPTY(devicex))
	{
		if((fp1=RDA_popen(devicex,"w"))!=NULL)
		{
			SelectPrintTypeByName(devicex,"PCL JOB HEADER",fp1);
			SelectPrintTypeByName(devicex,"LOAD_PCLMACRO_2110",fp1);
			SelectPrintTypeByName(devicex,"LOAD_PCLMACRO_2111",fp1);
			SelectPrintTypeByName(devicex,"DUPLEX LONG",fp1);
			SelectPrintType(devicex,FALSE,fp1);



			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," XXX-XX-XXXX       958.81       0.04      XXX-XX-XXXX       958.81        0.04 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                   958.81      40.27                        958.81       40.27 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"XX-XXXXXXX         958.81      13.90      XX-XXXXXXX        958.81       13.90 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," YOUR ORGANIZAZION NAME TO PRINT          YOUR ORGANIZAZION NAME TO PRINT       \r\n");
			RDA_fprintf(fp1," ORGANIZATION ADDRESS                     ORGANIZATION ADDRESS                  \r\n");
			RDA_fprintf(fp1," WORK PLACE, US  00000                    WORK PLACE, US  00000                 \r\n");
			RDA_fprintf(fp1,"                                                                                \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," 1                                        1                                     \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," EVA            E  ADAMS                  EVA            E  ADAMS               \r\n");
			RDA_fprintf(fp1," 100 GARDEN ST.                           100 GARDEN ST.                        \r\n");
			RDA_fprintf(fp1," RESIDENCE, US   00000                    RESIDENCE, US   00000                 \r\n");
			RDA_fprintf(fp1,"                                                                                \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"        0.00         0.00                        0.00         0.00              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"        0.00         0.00         0.00           0.00         0.00         0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                     0.00         0.00                        0.00         0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                     0.00         0.00                        0.00         0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                  0.00                                     0.00 \r\n");
			RDA_fprintf(fp1,"GA 9999999NJ       958.81         1.30   GA 9999999NJ       958.81         1.30 \r\n");
			RDA_fprintf(fp1," \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," XXX-XX-XXXX       958.81       0.04      XXX-XX-XXXX       958.81        0.04 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                   958.81      40.27                        958.81       40.27 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"XX-XXXXXXX         958.81      13.90      XX-XXXXXXX        958.81       13.90 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," YOUR ORGANIZAZION NAME TO PRINT          YOUR ORGANIZAZION NAME TO PRINT       \r\n");
			RDA_fprintf(fp1," ORGANIZATION ADDRESS                     ORGANIZATION ADDRESS                  \r\n");
			RDA_fprintf(fp1," WORK PLACE, US  00000                    WORK PLACE, US  00000                 \r\n");
			RDA_fprintf(fp1,"                                                                                \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," 1                                        1                                     \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," EVA            E  ADAMS                  EVA            E  ADAMS               \r\n");
			RDA_fprintf(fp1," 100 GARDEN ST.                           100 GARDEN ST.                        \r\n");
			RDA_fprintf(fp1," RESIDENCE, US   00000                    RESIDENCE, US   00000                 \r\n");
			RDA_fprintf(fp1,"                                                                                \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"        0.00         0.00                        0.00         0.00              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"        0.00         0.00         0.00           0.00         0.00         0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                     0.00         0.00                        0.00         0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                     0.00         0.00                        0.00         0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                  0.00                                     0.00 \r\n");
			RDA_fprintf(fp1,"GA 9999999NJ       958.81         1.30   GA 9999999NJ       958.81         1.30 \r\n");
			RDA_fprintf(fp1," \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			SelectPrintTypeByName(devicex,"PUSH CURSOR",fp1);
			SelectPrintTypeByName(devicex,"USE_PCLMACRO_2110",fp1);
			SelectPrintTypeByName(devicex,"PUSH CURSOR",fp1);
			RDA_fprintf(fp1,"\r\n");
			RDA_fprintf(fp1,"\r\n\f");
			SelectPrintTypeByName(devicex,"PUSH CURSOR",fp1);
			SelectPrintTypeByName(devicex,"USE_PCLMACRO_2111",fp1);
			SelectPrintTypeByName(devicex,"PUSH CURSOR",fp1);
			RDA_fprintf(fp1,"\r\n");
			RDA_fprintf(fp1,"\r\n\f");

			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," XXX-XX-XXXX      1142.10       0.00      XXX-XX-XXXX      1142.10        0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                  1142.10      47.97                       1142.10       47.97 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"XX-XXXXXXX        1142.10      16.55      XX-XXXXXXX       1142.10       16.55 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," YOUR ORGANIZAZION NAME TO PRINT          YOUR ORGANIZAZION NAME TO PRINT       \r\n");
			RDA_fprintf(fp1," ORGANIZATION ADDRESS                     ORGANIZATION ADDRESS                  \r\n");
			RDA_fprintf(fp1," WORK PLACE, US  00000                    WORK PLACE, US  00000                 \r\n");
			RDA_fprintf(fp1,"                                                                                \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," 2                                        2                                     \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," WILL-I-AM      D  ADAMS                  WILL-I-AM      D  ADAMS               \r\n");
			RDA_fprintf(fp1," 2784 ALABAHA WOODS DR.                   2784 ALABAHA WOODS DR.                \r\n");
			RDA_fprintf(fp1," RESIDENCE, US   00000                    RESIDENCE, US   00000                 \r\n");
			RDA_fprintf(fp1,"                                                                                \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"        0.00         0.00                        0.00         0.00              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"        0.00         0.00         0.00           0.00         0.00         0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                     0.00         0.00                        0.00         0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                     0.00         0.00                        0.00         0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                  0.00                                     0.00 \r\n");
			RDA_fprintf(fp1,"GA 9999999NJ      1142.10         3.98   GA 9999999NJ      1142.10         3.98 \r\n");
			RDA_fprintf(fp1," \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," XXX-XX-XXXX      1142.10       0.00      XXX-XX-XXXX      1142.10        0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                  1142.10      47.97                       1142.10       47.97 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"XX-XXXXXXX        1142.10      16.55      XX-XXXXXXX       1142.10       16.55 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," YOUR ORGANIZAZION NAME TO PRINT          YOUR ORGANIZAZION NAME TO PRINT       \r\n");
			RDA_fprintf(fp1," ORGANIZATION ADDRESS                     ORGANIZATION ADDRESS                  \r\n");
			RDA_fprintf(fp1," WORK PLACE, US  00000                    WORK PLACE, US  00000                 \r\n");
			RDA_fprintf(fp1,"                                                                                \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," 2                                        2                                     \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," WILL-I-AM      D  ADAMS                  WILL-I-AM      D  ADAMS               \r\n");
			RDA_fprintf(fp1," 2784 ALABAHA WOODS DR.                   2784 ALABAHA WOODS DR.                \r\n");
			RDA_fprintf(fp1," RESIDENCE, US   00000                    RESIDENCE, US   00000                 \r\n");
			RDA_fprintf(fp1,"                                                                                \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"        0.00         0.00                        0.00         0.00              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"        0.00         0.00         0.00           0.00         0.00         0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                     0.00         0.00                        0.00         0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                     0.00         0.00                        0.00         0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                  0.00                                     0.00 \r\n");
			RDA_fprintf(fp1,"GA 9999999NJ      1142.10         3.98   GA 9999999NJ      1142.10         3.98 \r\n");
			RDA_fprintf(fp1," \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			SelectPrintTypeByName(devicex,"PUSH CURSOR",fp1);
			SelectPrintTypeByName(devicex,"USE_PCLMACRO_2110",fp1);
			SelectPrintTypeByName(devicex,"PUSH CURSOR",fp1);
			RDA_fprintf(fp1,"\r\n");
			RDA_fprintf(fp1,"\r\n\f");
			SelectPrintTypeByName(devicex,"PUSH CURSOR",fp1);
			SelectPrintTypeByName(devicex,"USE_PCLMACRO_2111",fp1);
			SelectPrintTypeByName(devicex,"PUSH CURSOR",fp1);
			RDA_fprintf(fp1,"\r\n");
			RDA_fprintf(fp1,"\r\n\f");



			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," XXX-XX-XXXX      8507.00     878.25      XXX-XX-XXXX      8507.00      878.25 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                  8507.00     357.30                       8507.00      357.30 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"XX-XXXXXXX        8507.00     123.36      XX-XXXXXXX       8507.00      123.36 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," YOUR ORGANIZAZION NAME TO PRINT          YOUR ORGANIZAZION NAME TO PRINT       \r\n");
			RDA_fprintf(fp1," ORGANIZATION ADDRESS                     ORGANIZATION ADDRESS                  \r\n");
			RDA_fprintf(fp1," WORK PLACE, US  00000                    WORK PLACE, US  00000                 \r\n");
			RDA_fprintf(fp1,"                                                                                \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," 3                                        3                                     \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," ROSE           S  AREPRETTY              ROSE           S  AREPRETTY           \r\n");
			RDA_fprintf(fp1," 1600 PENNSYLVANIA AVE                    1600 PENNSYLVANIA AVE                 \r\n");
			RDA_fprintf(fp1," RESIDENCE, US   00000                    RESIDENCE, US   00000                 \r\n");
			RDA_fprintf(fp1,"                                                                                \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"        0.00         0.00                        0.00         0.00              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"        0.00         0.00         0.00           0.00         0.00         0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"          Retire   493.65         0.00             Retire   493.65         0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"    X     Cafe     419.71         0.00      X      Cafe     419.71         0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                  0.00                                     0.00 \r\n");
			RDA_fprintf(fp1,"GA 9999999NJ      8013.35       398.79   GA 9999999NJ      8013.35       398.79 \r\n");
			RDA_fprintf(fp1," \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," XXX-XX-XXXX      8507.00     878.25      XXX-XX-XXXX      8507.00      878.25 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                  8507.00     357.30                       8507.00      357.30 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"XX-XXXXXXX        8507.00     123.36      XX-XXXXXXX       8507.00      123.36 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," YOUR ORGANIZAZION NAME TO PRINT          YOUR ORGANIZAZION NAME TO PRINT       \r\n");
			RDA_fprintf(fp1," ORGANIZATION ADDRESS                     ORGANIZATION ADDRESS                  \r\n");
			RDA_fprintf(fp1," WORK PLACE, US  00000                    WORK PLACE, US  00000                 \r\n");
			RDA_fprintf(fp1,"                                                                                \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," 3                                        3                                     \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," ROSE           S  AREPRETTY              ROSE           S  AREPRETTY           \r\n");
			RDA_fprintf(fp1," 1600 PENNSYLVANIA AVE                    1600 PENNSYLVANIA AVE                 \r\n");
			RDA_fprintf(fp1," RESIDENCE, US   00000                    RESIDENCE, US   00000                 \r\n");
			RDA_fprintf(fp1,"                                                                                \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"        0.00         0.00                        0.00         0.00              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"        0.00         0.00         0.00           0.00         0.00         0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"          Retire   493.65         0.00             Retire   493.65         0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"    X     Cafe     419.71         0.00      X      Cafe     419.71         0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                  0.00                                     0.00 \r\n");
			RDA_fprintf(fp1,"GA 9999999NJ      8013.35       398.79   GA 9999999NJ      8013.35       398.79 \r\n");
			RDA_fprintf(fp1," \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			SelectPrintTypeByName(devicex,"PUSH CURSOR",fp1);
			SelectPrintTypeByName(devicex,"USE_PCLMACRO_2110",fp1);
			SelectPrintTypeByName(devicex,"PUSH CURSOR",fp1);
			RDA_fprintf(fp1,"\r\n");
			RDA_fprintf(fp1,"\r\n\f");
			SelectPrintTypeByName(devicex,"PUSH CURSOR",fp1);
			SelectPrintTypeByName(devicex,"USE_PCLMACRO_2111",fp1);
			SelectPrintTypeByName(devicex,"PUSH CURSOR",fp1);
			RDA_fprintf(fp1,"\r\n");
			RDA_fprintf(fp1,"\r\n\f");



			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," XXX-XX-XXXX      2397.34      34.46      XXX-XX-XXXX      2397.34       34.46 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                  2397.34     100.69                       2397.34      100.69 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"XX-XXXXXXX        2397.34      34.76      XX-XXXXXXX       2397.34       34.76 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," YOUR ORGANIZAZION NAME TO PRINT          YOUR ORGANIZAZION NAME TO PRINT       \r\n");
			RDA_fprintf(fp1," ORGANIZATION ADDRESS                     ORGANIZATION ADDRESS                  \r\n");
			RDA_fprintf(fp1," WORK PLACE, US  00000                    WORK PLACE, US  00000                 \r\n");
			RDA_fprintf(fp1,"                                                                                \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," 4                                        4                                     \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," CAN-WE         B  FREEMAN                CAN-WE         B  FREEMAN             \r\n");
			RDA_fprintf(fp1," 148 ANN STREET                           148 ANN STREET                        \r\n");
			RDA_fprintf(fp1," RESIDENCE, US   00000                    RESIDENCE, US   00000                 \r\n");
			RDA_fprintf(fp1,"                                                                                \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"        0.00         0.00                        0.00         0.00              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"        0.00         0.00         0.00           0.00         0.00         0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"          Retire    12.00         0.00             Retire    12.00         0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"    X     Cafe      64.08         0.00      X      Cafe      64.08         0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                  0.00                                     0.00 \r\n");
			RDA_fprintf(fp1,"GA 9999999NJ      2397.34        86.67   GA 9999999NJ      2397.34        86.67 \r\n");
			RDA_fprintf(fp1," \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," XXX-XX-XXXX      2397.34      34.46      XXX-XX-XXXX      2397.34       34.46 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                  2397.34     100.69                       2397.34      100.69 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"XX-XXXXXXX        2397.34      34.76      XX-XXXXXXX       2397.34       34.76 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," YOUR ORGANIZAZION NAME TO PRINT          YOUR ORGANIZAZION NAME TO PRINT       \r\n");
			RDA_fprintf(fp1," ORGANIZATION ADDRESS                     ORGANIZATION ADDRESS                  \r\n");
			RDA_fprintf(fp1," WORK PLACE, US  00000                    WORK PLACE, US  00000                 \r\n");
			RDA_fprintf(fp1,"                                                                                \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," 4                                        4                                     \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," CAN-WE         B  FREEMAN                CAN-WE         B  FREEMAN             \r\n");
			RDA_fprintf(fp1," 148 ANN STREET                           148 ANN STREET                        \r\n");
			RDA_fprintf(fp1," RESIDENCE, US   00000                    RESIDENCE, US   00000                 \r\n");
			RDA_fprintf(fp1,"                                                                                \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"        0.00         0.00                        0.00         0.00              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"        0.00         0.00         0.00           0.00         0.00         0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"          Retire    12.00         0.00             Retire    12.00         0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"    X     Cafe      64.08         0.00      X      Cafe      64.08         0.00 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                  0.00                                     0.00 \r\n");
			RDA_fprintf(fp1,"GA 9999999NJ      2397.34        86.67   GA 9999999NJ      2397.34        86.67 \r\n");
			RDA_fprintf(fp1," \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			SelectPrintTypeByName(devicex,"PUSH CURSOR",fp1);
			SelectPrintTypeByName(devicex,"USE_PCLMACRO_2110",fp1);
			SelectPrintTypeByName(devicex,"PUSH CURSOR",fp1);
			RDA_fprintf(fp1,"\r\n");
			RDA_fprintf(fp1,"\r\n\f");
			SelectPrintTypeByName(devicex,"PUSH CURSOR",fp1);
			SelectPrintTypeByName(devicex,"USE_PCLMACRO_2111",fp1);
			SelectPrintTypeByName(devicex,"PUSH CURSOR",fp1);
			RDA_fprintf(fp1,"\r\n");
			RDA_fprintf(fp1,"\r\n\f");




			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                12511.60     912.75                      12511.60      912.75 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                13005.25     546.23                      13005.25      546.23 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                13005.25     188.57                      13005.25      188.57 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," YOUR ORGANIZAZION NAME TO PRINT          YOUR ORGANIZAZION NAME TO PRINT       \r\n");
			RDA_fprintf(fp1," ORGANIZATION ADDRESS                     ORGANIZATION ADDRESS                  \r\n");
			RDA_fprintf(fp1," WORK PLACE, US  00000                    WORK PLACE, US  00000                 \r\n");
			RDA_fprintf(fp1,"                                                                                \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                                                              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                                                              \r\n");
			RDA_fprintf(fp1,"                                                                              \r\n");
			RDA_fprintf(fp1,"                                                                              \r\n");
			RDA_fprintf(fp1,"                                                                              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"        0.00         0.00                        0.00         0.00              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"        0.00         0.00                        0.00         0.00              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                                                                \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                                                                \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                                                                \r\n");
			RDA_fprintf(fp1," \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1,"                12511.60     912.75                      12511.60      912.75 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                13005.25     546.23                      13005.25      546.23 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                13005.25     188.57                      13005.25      188.57 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"   YOUR ORGANIZAZION NAME TO PRINT        YOUR ORGANIZAZION NAME TO PRINT     \r\n");
			RDA_fprintf(fp1,"   ORGANIZATION ADDRESS                   ORGANIZATION ADDRESS                \r\n");
			RDA_fprintf(fp1,"   WORK PLACE, US  00000                  WORK PLACE, US  00000               \r\n");
			RDA_fprintf(fp1,"                                                                              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                                                              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                                                              \r\n");
			RDA_fprintf(fp1,"                                                                              \r\n");
			RDA_fprintf(fp1,"                                                                              \r\n");
			RDA_fprintf(fp1,"                                                                              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"        0.00         0.00                        0.00         0.00              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"        0.00         0.00                        0.00         0.00              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                                                                \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                                                                \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                                                                \r\n");
			RDA_fprintf(fp1," \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			SelectPrintTypeByName(devicex,"PUSH CURSOR",fp1);
			SelectPrintTypeByName(devicex,"USE_PCLMACRO_2110",fp1);
			SelectPrintTypeByName(devicex,"PUSH CURSOR",fp1);
			RDA_fprintf(fp1,"\r\n");
			RDA_fprintf(fp1,"\r\n\f");
			SelectPrintTypeByName(devicex,"PUSH CURSOR",fp1);
			SelectPrintTypeByName(devicex,"USE_PCLMACRO_2111",fp1);
			SelectPrintTypeByName(devicex,"PUSH CURSOR",fp1);
			RDA_fprintf(fp1,"\r\n");
			RDA_fprintf(fp1,"\r\n\f");


			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                12511.60     912.75                      12511.60      912.75 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                13005.25     546.23                      13005.25      546.23 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                13005.25     188.57                      13005.25      188.57 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"   GRAND TOTALS                           GRAND TOTALS                        \r\n");
			RDA_fprintf(fp1,"                                                                              \r\n");
			RDA_fprintf(fp1,"                                                                              \r\n");
			RDA_fprintf(fp1,"                                                                              \r\n");
			RDA_fprintf(fp1,"                                                                              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                                                              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                                                              \r\n");
			RDA_fprintf(fp1,"                                                                              \r\n");
			RDA_fprintf(fp1,"                                                                              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"        0.00         0.00                        0.00         0.00              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"        0.00         0.00                        0.00         0.00              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                                                                \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                                                                \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                                                                \r\n");
			RDA_fprintf(fp1," \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1,"                12511.60     912.75                      12511.60      912.75 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                13005.25     546.23                      13005.25      546.23 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                13005.25     188.57                      13005.25      188.57 \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"   GRAND TOTALS                           GRAND TOTALS                        \r\n");
			RDA_fprintf(fp1,"                                                                              \r\n");
			RDA_fprintf(fp1,"                                                                              \r\n");
			RDA_fprintf(fp1,"                                                                              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                                                              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                                                              \r\n");
			RDA_fprintf(fp1,"                                                                              \r\n");
			RDA_fprintf(fp1,"                                                                              \r\n");
			RDA_fprintf(fp1,"                                                                              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"        0.00         0.00                        0.00         0.00              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"        0.00         0.00                        0.00         0.00              \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                                                                \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                                                                \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1,"                                                                                \r\n");
			RDA_fprintf(fp1," \r\n");
			SelectPrintTypeByName(devicex,"HALF LINE FEED",fp1);
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			RDA_fprintf(fp1," \r\n");
			SelectPrintTypeByName(devicex,"PUSH CURSOR",fp1);
			SelectPrintTypeByName(devicex,"USE_PCLMACRO_2110",fp1);
			SelectPrintTypeByName(devicex,"PUSH CURSOR",fp1);
			RDA_fprintf(fp1,"\r\n");
			RDA_fprintf(fp1,"\r\n\f");
			SelectPrintTypeByName(devicex,"PUSH CURSOR",fp1);
			SelectPrintTypeByName(devicex,"USE_PCLMACRO_2111",fp1);
			SelectPrintTypeByName(devicex,"PUSH CURSOR",fp1);
			RDA_fprintf(fp1,"\r\n");
			RDA_fprintf(fp1,"\r\n\f");



			SelectPrintTypeByName(devicex,"PCL JOB FOOTER",fp1);
			RDA_pclose(fp1);
			return;
		}
	}
	prtmessage=Rmalloc(RDAstrlen(devicex)+67);
	sprintf(prtmessage,"The output device [%s] does not exist or cannot otherwise be opened!",devicex);
	ERRORDIALOG("Output Device Failure",prtmessage,NULL,FALSE);
	if(prtmessage!=NULL) Rfree(prtmessage);
}
static void setdevlicense(RDArsrc *r)
{
	if(XPERT_SETUP->HRM==TRUE)
	{
		dev_license=TRUE;
	} else {
		readwidget(r,"DEV_LICENSE");
		FINDRSCGETSTRING(r,"DEV_LICENSE",&devlicense);
		if(CheckDevLicense(devlicense))
		{
			dev_license=TRUE;
		} else {
			dev_license=FALSE;
		}
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
	char *temp1=NULL,*tmp=NULL;

	if(InitializeSubsystems(argc,argv,"UTILITIES","PLAINPAPER TEST")) 
	{
		return(1);
	}
	if(XPERT_SETUP->HRM==TRUE)
	{
		dev_license=TRUE;
	} else {
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
	}
	SCN_LASER_FORM_TEST();
	mainrsrc=RDArsrcNEW("UTILITIES","PLAINPAPER TEST");
	addstdrsrc(mainrsrc,"DEV_LICENSE",VARIABLETEXT,0,devlicense,TRUE);
	FINDRSCSETFUNC(mainrsrc,"DEV_LICENSE",setdevlicense,NULL);
	addstdrsrc(mainrsrc,"OVERRIDE PASSWORD",PLAINTEXT,60,NULL,TRUE);
	addstdrsrc(mainrsrc,"DOWNLOAD FORM",BOOLNS,1,&download_form,TRUE);
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
