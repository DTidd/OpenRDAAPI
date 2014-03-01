#include<locked.xpm>

char use_proxy=FALSE,*http_proxy=NULL,cfu=FALSE;
int http_proxy_port=1080;

struct MemoryStruct {
	char *memory;
	int size;
}
static int WriteMemoryCallback(void *ptr,int size,int nmemb,void *data)
{
	int realsize=size*nmemb;
	struct MemoryStruct *mem=(struct MemoryStruct *)data;

	mem->memory=(char *)Rrealloc(mem->memory,mem->size+realsize+1);
	if(mem->memory)
	{
		memcpy(&(mem->memory[mem-size]),ptr,realsize);
		mem->size+=realsize;
		mem->memory[mem->size]=0;
	}
	return(realsize);
}
void RunRDAUpdate(RDArsrc *mainrsrc,char *m)
{
	APPlib *args=NULL;

	args=APPlibNEW();
	addAPPlib(args,"UTILITIES");
	if(XPERT_SETUP->software_type<2)
	{
		addAPPlib(args,"XPERT UPDATE MENU");
	} else {
		addAPPlib(args,"MBGUI UPDATE MENU");
	}
	ExecuteProgram("domenu",args);
	if(args!=NULL) freeapplib(args);
}
void CheckRDAUpdates(RDArsrc *mainrsrc,char *m)
{
	char *temp1=NULL,name[256],*temp=NULL;
	CURL *RDAcurl=NULL;
	MemoryStruct chunk;
	RDAwdgt *wdgt=NULL;
	int x=0;


	if(isEMPTY(m)) return;
	temp1=Rmalloc(RDAstrlen(m)+15);
	sprintf(temp1,"DUE FOR %s UPDATE",m);
	x=FINDWDGT(mainrsrc->scn,temp1);
	if(x!=(-1))
	{
		wdgt=mainrsrc->scn->wdgts+x;
		addbtnrsrc(mainrsrc,temp1,cfu,RunRDAUpdate,m);
		if(cfu)
		{
			RDAcurl=curl_easy_init();
			memset(name,0,256);
			if(XPERT_SETUP->software_type<2)
			{
#ifndef WIN32
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
#ifndef WIN32
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
			curl_easy_setopt(RDAcurl,CURLOPT_WRITEDATA,(void *)&chunk);
			curl_easy_setopt(RDAcurl,CURLOPT_USERAGENT,"libcurl-agent/1.0");
			res=curl_easy_perform(RDAcurl);
			curl_easy_cleanup(RDAcurl);
			if(CURLE_OK!=res)
			{
				FINDRSCSETSENSITIVE(mainrsrc,temp1,FALSE);
			}
			prterr("read [%s] ",chunk.memory);TRACE;
			temp=RDAstrstr(chunk.memory,".7z ");
			if(temp!=NULL)
			{
				temp+=4;
				memset(stemp,0,101);
				sprintf(stemp,"%.10s %s UPDATE",temp,m);
				if(wdgt->label!=NULL) Rfree(wdgt->label);
				wdgt->label=stralloc(stemp);
			}
		} else {
			FINDRSCSETSENSITIVE(mainrsrc,temp1);
		}
	}
	if(chunk.memory) Rfree(chunk.memory);
	if(temp1!=NULL) Rfree(temp1);
}
	curl_global_init(CURL_GLOBAL_DEFAULT);	
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("UTILITIES")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif

	gsv=RDAGenericSetupNew("UTILITIES","CHECK FOR UPDATES");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			cfu=*gsv->value.string_value;
		} else {
			cfu=FALSE;
		}
	}
	if(cfu)
	{
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
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	if(XPERT_SETUP->FINMGT==TRUE) CheckRDAUpdates(mainrsrc,"FINMGT");
	if(XPERT_SETUP->VENPMT==TRUE) CheckRDAUpdates(mainrsrc,"VENPMT");
	if(XPERT_SETUP->VENPMT==TRUE) CheckRDAUpdates(mainrsrc,"VENSAFE");
	if(XPERT_SETUP->PURORD==TRUE) CheckRDAUpdates(mainrsrc,"PURORD");
	if(XPERT_SETUP->BNKREC==TRUE) CheckRDAUpdates(mainrsrc,"BNKREC");
	if(XPERT_SETUP->FIXASS==TRUE) CheckRDAUpdates(mainrsrc,"FIXASS");
	if(XPERT_SETUP->PRSNNL==TRUE) CheckRDAUpdates(mainrsrc,"PRSNNL");
	if(XPERT_SETUP->PAYROLL==TRUE) CheckRDAUpdates(mainrsrc,"PAYROLL");
	if(XPERT_SETUP->POSTRK==TRUE) CheckRDAUpdates(mainrsrc,"POSTRK");
	if(XPERT_SETUP->BFTMGT==TRUE) CheckRDAUpdates(mainrsrc,"BFTMGT");
	if(XPERT_SETUP->APPMGT==TRUE) CheckRDAUpdates(mainrsrc,"APPMGT");
	if(XPERT_SETUP->EMPABS==TRUE) CheckRDAUpdates(mainrsrc,"EMPABS");
	if(XPERT_SETUP->LVEMGT==TRUE) CheckRDAUpdates(mainrsrc,"LVEMGT");
	if(XPERT_SETUP->SUBMGT==TRUE) CheckRDAUpdates(mainrsrc,"SUBMGT");
	if(XPERT_SETUP->LVSIMP==TRUE) CheckRDAUpdates(mainrsrc,"LVSIMP");
	if(XPERT_SETUP->INVCTL==TRUE) CheckRDAUpdates(mainrsrc,"INVCTL");
	if(XPERT_SETUP->RLVEMGT==TRUE) CheckRDAUpdates(mainrsrc,"RLVEMGT");
	if(XPERT_SETUP->REQMGT==TRUE) CheckRDAUpdates(mainrsrc,"REQMGT");
	if(XPERT_SETUP->RLSTMGT==TRUE) CheckRDAUpdates(mainrsrc,"RLSTMGT");
	if(XPERT_SETUP->CSHDWR==TRUE) CheckRDAUpdates(mainrsrc,"CSHDWR");
	if(XPERT_SETUP->DMVREG==TRUE) CheckRDAUpdates(mainrsrc,"DMVREG");
	if(XPERT_SETUP->PROPERTY==TRUE) CheckRDAUpdates(mainrsrc,"PROPERTY");
	if(XPERT_SETUP->MISCBILL==TRUE) CheckRDAUpdates(mainrsrc,"MISCBILL");
	if(XPERT_SETUP->CATALOGUE==TRUE) CheckRDAUpdates(mainrsrc,"CATALOGUE");
	if(XPERT_SETUP->BLDPRMT==TRUE) CheckRDAUpdates(mainrsrc,"BLDPRMT");
	if(XPERT_SETUP->ARCHIVE==TRUE) CheckRDAUpdates(mainrsrc,"ARCHIVE");
	if(XPERT_SETUP->BUDPREP==TRUE) CheckRDAUpdates(mainrsrc,"BUDPREP");
	if(XPERT_SETUP->UTLBLL==TRUE) CheckRDAUpdates(mainrsrc,"UTLBLL");
	if(XPERT_SETUP->OCCTAX==TRUE) CheckRDAUpdates(mainrsrc,"OCCTAX");
