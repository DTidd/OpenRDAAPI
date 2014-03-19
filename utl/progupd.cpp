/* progupd.c - Display Program Update Status */
/*lint -library */
#ifndef WIN32
#define __NAM__ "progupd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "progupd.exe"
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

char *module="UTILITIES";

static void quitfunction(RDArsrc *m)
{
	if(IsScreenDisplayed(m)) killwindow(m);
	if(m!=NULL) free_rsrc(m);
	ShutdownSubsystems();
}
static void downloadfunction(RDArsrc *m)
{
	ExecuteProgram("doftpget",NULL);
	quitfunction(m);
}

char use_proxy=FALSE,*http_proxy=NULL,cfu=FALSE;
int http_proxy_port=1080;

struct MemoryStruct {
	char *memory;
	int size;
};

static int WriteMemoryCallback(void *ptr,int size,int nmemb,void *data)
{
	int realsize=size*nmemb;
	struct MemoryStruct *mem=(struct MemoryStruct *)data;

	mem->memory=(char *)Rrealloc(mem->memory,mem->size+realsize+1);
	if(mem->memory)
	{
		memcpy(&(mem->memory[mem->size]),ptr,realsize);
		mem->size+=realsize;
		mem->memory[mem->size]=0;
	}
	return(realsize);
}

static char *CheckRDAUpdates(char *m)
{
	char *temp1=NULL,name[256],*temp=NULL,*date1=NULL,*mdate=NULL;
	CURL *RDAcurl=NULL;
	struct MemoryStruct chunk;
	CURLcode res=0;


	if(isEMPTY(m)) return(NULL);
	chunk.memory=NULL;
	chunk.size=0;
	RDAcurl=curl_easy_init();
	memset(name,0,256);
	if(XPERT_SETUP->software_type<2)
	{
#ifdef WIN32
		sprintf(name,"http://66.135.38.166/checkupdates/xpert/winxp_mysql-%s.lst",m);
#else
#ifndef RDA_64BITS
		sprintf(name,"http://66.135.38.166/checkupdates/xpert/lnx_mysql-%s.lst",m);
#endif 
#ifdef RDA_64BITS
		sprintf(name,"http://66.135.38.166/checkupdates/xpert/lnx_mysql64-%s.lst",m);
#endif 
#endif
	} else {
#ifdef WIN32
		sprintf(name,"http://66.135.38.166/checkupdates/mbgui/winxp_mysql-%s.lst",m);
#else
#ifndef RDA_64BITS
		sprintf(name,"http://66.135.38.166/checkupdates/mbgui/lnx_mysql-%s.lst",m);
#endif 
#ifdef RDA_64BITS
		sprintf(name,"http://66.135.38.166/checkupdates/mbgui/lnx_mysql64-%s.lst",m);
#endif 
#endif
	}
	curl_easy_setopt(RDAcurl,CURLOPT_URL,name);
	if(use_proxy)
	{
		sprintf(stemp,"%s:%d",(http_proxy!=NULL ? http_proxy:""),http_proxy_port);
		curl_easy_setopt(RDAcurl,CURLOPT_PROXY,stemp);
	}
	curl_easy_setopt(RDAcurl,CURLOPT_VERBOSE,FALSE);
	curl_easy_setopt(RDAcurl,CURLOPT_WRITEFUNCTION,WriteMemoryCallback);
	curl_easy_setopt(RDAcurl,CURLOPT_FILE,(void *)&chunk);
	curl_easy_setopt(RDAcurl,CURLOPT_USERAGENT,"libcurl-agent/1.0");
	curl_easy_setopt(RDAcurl,CURLOPT_TIMEOUT,5);
	res=curl_easy_perform(RDAcurl);
	curl_easy_cleanup(RDAcurl);
	if(CURLE_OK!=res)
	{
	} else {
		temp=RDAstrstr(chunk.memory,".7z ");
		if(temp!=NULL)
		{
			temp+=4;
			date1=stralloc(temp);
			date1[10]=0;
			mdate=Rmalloc(11);
			sprintf(mdate,"%.2s/%.2s/%.4s",&date1[5],&date1[8],&date1[0]);
			if(date1!=NULL) Rfree(date1);
		}
	}
	if(chunk.memory) Rfree(chunk.memory);
	if(temp1!=NULL) Rfree(temp1);
	return(mdate);
}
static char *GSV_DATE(char *m,char *name)
{
	char *d=NULL;
	RDAGenericSetup *gsv=NULL;
	char *libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(m)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,m);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,m);
#endif
	gsv=RDAGenericSetupNew(m,name);
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			d=stralloc(gsv->value.string_value);
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	return(d);
}


#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	char *ddate=NULL,*temp=NULL;
	int x=0;

	if(InitializeSubsystems(argc,argv,module,"DISPLAY PROGRAM UPDATE STATUS"))
	{
		return;
	}
	curl_global_init(CURL_GLOBAL_DEFAULT);	
	mainrsrc=RDArsrcNEW(module,"DISPLAY PROGRAM UPDATE STATUS");
	for(x=0;x<27;++x)
	{
		memset(stemp,0,101);
		sprintf(stemp,"MODULE NAME %02d",x+1);
		addstdrsrc(mainrsrc,stemp,VARIABLETEXT,40,NULL,FALSE);
		memset(stemp,0,101);
		sprintf(stemp,"INSTALLED DATE %02d",x+1);
		addstdrsrc(mainrsrc,stemp,DATES,10,NULL,FALSE);
		memset(stemp,0,101);
		sprintf(stemp,"COMPILED DATE %02d",x+1);
		addstdrsrc(mainrsrc,stemp,DATES,10,NULL,FALSE);
		memset(stemp,0,101);
		sprintf(stemp,"RELEASED DATE %02d",x+1);
		addstdrsrc(mainrsrc,stemp,DATES,10,NULL,FALSE);
	}
	addbtnrsrc(mainrsrc,"DOWNLOAD",TRUE,downloadfunction,NULL);
	addbtnrsrc(mainrsrc,"QUIT",TRUE,quitfunction,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);


	FINDRSCSETSTRING(mainrsrc,"MODULE NAME 01","Utilities and Tools");
	ddate=GSV_DATE("UTILITIES","INSTALLATION DATE");
	FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 01",ddate);
	if(ddate!=NULL) Rfree(ddate);
	ddate=GSV_DATE("UTILITIES","COMPILE DATE");
	FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 01",ddate);
	if(ddate!=NULL) Rfree(ddate);
	ddate=CheckRDAUpdates("UTILITIES");
	FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 01",ddate);
	if(ddate!=NULL) Rfree(ddate);

	if(XPERT_SETUP->ARCHIVE)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 02","F.A.S.T.");
		ddate=GSV_DATE("ARCHIVE","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 02",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("ARCHIVE","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 02",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("ARCHIVE");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 02",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	if(XPERT_SETUP->FINMGT)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 03","Financial Management");
		ddate=GSV_DATE("FINMGT","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 03",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("FINMGT","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 03",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("FINMGT");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 03",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	if(XPERT_SETUP->BUDPREP)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 04","Budget Preparation");
		ddate=GSV_DATE("BUDPREP","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 04",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("BUDPREP","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 04",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("BUDPREP");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 04",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	if(XPERT_SETUP->BNKREC)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 05","Bank Reconciliation");
		ddate=GSV_DATE("BNKREC","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 05",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("BNKREC","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 05",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("BNKREC");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 05",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	if(XPERT_SETUP->FIXASS)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 06","Fixed Assets");
		ddate=GSV_DATE("FIXASS","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 06",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("FIXASS","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 06",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("FIXASS");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 06",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	if(XPERT_SETUP->VENPMT)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 07","Vendor Payments");
		ddate=GSV_DATE("VENPMT","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 07",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("VENPMT","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 07",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("VENPMT");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 07",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	if(XPERT_SETUP->PURORD)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 08","Purchase Orders");
		ddate=GSV_DATE("PURORD","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 08",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("PURORD","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 08",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("PURORD");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 08",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	if(XPERT_SETUP->REQMGT)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 09","Requisitions");
		ddate=GSV_DATE("REQMGT","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 09",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("REQMGT","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 09",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("REQMGT");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 09",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	if(XPERT_SETUP->INVCTL)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 10","Inventory Control");
		ddate=GSV_DATE("INVCTL","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 10",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("INVCTL","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 10",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("INVCTL");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 10",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	if(XPERT_SETUP->HRM)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 11","Open Self Serve");
		ddate=GSV_DATE("OPENSS","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 11",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("OPENSS","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 11",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("OPENSS");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 11",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	if(XPERT_SETUP->PRSNNL)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 12","Personnel / HR");
		ddate=GSV_DATE("PRSNNL","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 12",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("PRSNNL","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 12",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("PRSNNL");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 12",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	if(XPERT_SETUP->POSTRK)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 13","Position Tracking");
		ddate=GSV_DATE("POSTRK","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 13",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("POSTRK","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 13",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("POSTRK");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 13",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	if(XPERT_SETUP->PAYROLL)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 14","Payroll");
		ddate=GSV_DATE("PAYROLL","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 14",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("PAYROLL","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 14",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("PAYROLL");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 14",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	if(XPERT_SETUP->LVEMGT)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 15","Leave Management");
		ddate=GSV_DATE("LVEMGT","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 15",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("LVEMGT","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 15",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("LVEMGT");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 15",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	if(XPERT_SETUP->EMPABS)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 16","Employee Absentee");
		ddate=GSV_DATE("EMPABS","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 16",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("EMPABS","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 16",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("EMPABS");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 16",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	if(XPERT_SETUP->SUBMGT)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 17","Substitute Management");
		ddate=GSV_DATE("SUBMGT","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 17",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("SUBMGT","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 17",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("SUBMGT");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 17",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	if(XPERT_SETUP->APPMGT)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 18","Applicant Management");
		ddate=GSV_DATE("APPMGT","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 18",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("APPMGT","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 18",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("APPMGT");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 18",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	if(XPERT_SETUP->BFTMGT)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 19","Benefit Management");
		ddate=GSV_DATE("BFTMGT","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 19",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("BFTMGT","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 19",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("BFTMGT");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 19",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	if(XPERT_SETUP->OCCTAX)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 20","Occ. Tax/Bus. Lic.");
		ddate=GSV_DATE("OCCTAX","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 20",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("OCCTAX","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 20",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("OCCTAX");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 20",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	if(XPERT_SETUP->MISCBILL)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 21","Misc Billing");
		ddate=GSV_DATE("MISCBILL","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 21",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("MISCBILL","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 21",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("MISCBILL");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 21",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	if(XPERT_SETUP->BLDPRMT)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 22","Building Permits");
		ddate=GSV_DATE("BLDPRMT","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 22",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("BLDPRMT","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 22",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("BLDPRMT");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 22",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	if(XPERT_SETUP->RLSTMGT)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 23","Real Estate Taxes");
		ddate=GSV_DATE("RLSTMGT","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 23",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("RLSTMGT","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 23",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("RLSTMGT");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 23",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	if(XPERT_SETUP->PROPERTY)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 24","Property Taxes");
		ddate=GSV_DATE("PROPERTY","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 24",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("PROPERTY","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 24",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("PROPERTY");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 24",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	if(XPERT_SETUP->UTLBLL)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 25","Utility Billing");
		ddate=GSV_DATE("UTLBLL","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 25",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("UTLBLL","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 25",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("UTLBLL");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 25",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	if(XPERT_SETUP->DMVREG)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 26","DMV Registration");
		ddate=GSV_DATE("DMVREG","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 26",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("DMVREG","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 26",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("DMVREG");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 26",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	if(XPERT_SETUP->CSHDWR)
	{
		FINDRSCSETSTRING(mainrsrc,"MODULE NAME 27","Collections");
		ddate=GSV_DATE("CSHDWR","INSTALLATION DATE");
		FINDRSCSETSTRING(mainrsrc,"INSTALLED DATE 27",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=GSV_DATE("CSHDWR","COMPILE DATE");
		FINDRSCSETSTRING(mainrsrc,"COMPILED DATE 27",ddate);
		if(ddate!=NULL) Rfree(ddate);
		ddate=CheckRDAUpdates("CSHDWR");
		FINDRSCSETSTRING(mainrsrc,"RELEASED DATE 27",ddate);
		if(ddate!=NULL) Rfree(ddate);
	}

	mainrsrc->scn=RDAscrnNEW(module,"DISPLAY PROGRAM UPDATE STATUS");
	if(mainrsrc->scn!=NULL)
	{
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,23,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,20,"","Information","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,5,"","","","<p><span style=\"font-family: arial,helvetica,sans-serif; font-size: medium;\"><strong>\"Installed\"</strong> is the last date a complete update to this application was performed. </span></p>\n<p><span style=\"font-family: arial,helvetica,sans-serif; font-size: medium;\"><strong>\"Compiled\"</strong> was the date the application update program was created at RDA.</span></p>\n<p><span style=\"font-family: arial,helvetica,sans-serif; font-size: medium;\"><strong>\"Released\"</strong> is the date programs were made available for download from the website. </span></p>",0,0,4,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,20,"","Application Status","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,28,"","","","",0,0,2,"","","","");
		ADVaddwdgt(mainrsrc->scn,30,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,5,"","Application","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,5,"","Installed","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,5,"","Compiled","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,5,"","Released","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(mainrsrc->scn,31,"","","","",0,0,0,"","","","");
		for(x=0;x<27;++x)
		{
			memset(stemp,0,101);
			sprintf(stemp,"MODULE NAME %02d",x+1);
			FINDRSCGETSTRING(mainrsrc,stemp,&temp);
			if(!isEMPTY(temp))
			{
				ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
				ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,30,0,"","","","");
				memset(stemp,0,101);
				sprintf(stemp,"INSTALLED DATE %02d",x+1);
				ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,10,0,"","","","");
				memset(stemp,0,101);
				sprintf(stemp,"COMPILED DATE %02d",x+1);
				ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,10,0,"","","","");
				memset(stemp,0,101);
				sprintf(stemp,"RELEASED DATE %02d",x+1);
				ADVaddwdgt(mainrsrc->scn,0,stemp,"","","",0,10,0,"","","","");
				ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
			}
		}
		if(temp!=NULL) Rfree(temp);
		ADVaddwdgt(mainrsrc->scn,29,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,21,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");

		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,6,"DOWNLOAD","Download","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,6,"QUIT","Cancel","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,6,"HELP","Help","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(mainrsrc->scn,2,"","","","",0,0,0,"","","","");
	}
	APPmakescrn(mainrsrc,TRUE,quitfunction,NULL,TRUE);
}
