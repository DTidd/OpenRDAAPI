/* supreq.c - Automagical Support Request */
#include<mix.hpp>


static char *InstallCompileDate(char *m,short w)
{
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;
	char *temp=NULL;

	if(isEMPTY(m)) return(NULL);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(m)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,m);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,m);
#endif

	switch(w)
	{
		case 0:
		default:
			gsv=RDAGenericSetupNew(m,"COMPILE DATE");
			break;
		case 1:
			gsv=RDAGenericSetupNew(m,"INSTALLATION DATE");
			break;
	}
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			temp=stralloc(gsv->value.string_value);
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	return(temp);
}
void xAddSupportRequest(char *prd,char *prc,char *sub,int line,char *file)
{
	APPlib *args=NULL,*mye=NULL;
	char *temp=NULL;
	char temp2[2048];

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagolh)
	{
		prterr("AddSupportRequest for Product [%s] Process [%s] Subject [%s] at line [%d] Program [%s].",(prd!=NULL ? prd:""),(prc!=NULL ? prc:""),(sub!=NULL ? sub:""),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	args=APPlibNEW();
	mye=APPlibNEW();
	addAPPlib(args,"OLHELP");
	addAPPlib(args,"AUTOSUPPORT");


#ifndef WIN32
	memset(temp2,0,2048);
	snprintf(temp2,2048,"ADD_PRODUCT=%s",(prd!=NULL ? prd:""));
	addAPPlib(mye,temp2);
	
	memset(temp2,0,2048);
	snprintf(temp2,2048,"ADD_PROCESS=%s",(prc!=NULL ? prc:""));
	addAPPlib(mye,temp2);
	
	memset(temp2,0,2048);
	snprintf(temp2,2048,"ADD_SUBJECT=%s",(sub!=NULL ? sub:""));
	addAPPlib(mye,temp2);
	
	temp=InstallCompileDate(prd,0);
	memset(temp2,0,2048);
	snprintf(temp2,2048,"ADD_COMPILE_DATE=%s",(temp!=NULL ? temp:""));
	addAPPlib(mye,temp2);

	if(temp!=NULL) Rfree(temp);
	temp=InstallCompileDate(prd,1);
	memset(temp2,0,2048);
	snprintf(temp2,2048,"ADD_INSTALL_DATE=%s",(temp!=NULL ? temp:""));
	addAPPlib(mye,temp2);
#endif
#ifdef WIN32
	memset(temp2,0,2048);
	_snprintf(temp2,2048,"ADD_PRODUCT=%s",(prd!=NULL ? prd:""));
	addAPPlib(mye,temp2);
	
	memset(temp2,0,2048);
	_snprintf(temp2,2048,"ADD_PROCESS=%s",(prc!=NULL ? prc:""));
	addAPPlib(mye,temp2);
	
	memset(temp2,0,2048);
	_snprintf(temp2,2048,"ADD_SUBJECT=%s",(sub!=NULL ? sub:""));
	addAPPlib(mye,temp2);
	
	temp=InstallCompileDate(prd,0);
	memset(temp2,0,2048);
	_snprintf(temp2,2048,"ADD_COMPILE_DATE=%s",(temp!=NULL ? temp:""));
	addAPPlib(mye,temp2);

	if(temp!=NULL) Rfree(temp);
	temp=InstallCompileDate(prd,1);
	memset(temp2,0,2048);
	_snprintf(temp2,2048,"ADD_INSTALL_DATE=%s",(temp!=NULL ? temp:""));
	addAPPlib(mye,temp2);
#endif
	
	if(temp!=NULL) Rfree(temp);
	ADV3Execute2Program("wwwdoadd",args,mye,TRUE,TRUE);
	if(args!=NULL) freeapplib(args);
	if(mye!=NULL) freeapplib(mye);
	RDA_UnSetEnv("ADD_SUBJECT");
	RDA_UnSetEnv("ADD_PROCESS");
	RDA_UnSetEnv("ADD_PRODUCT");
}
void xAPPlib2SupportRequest(char *prd,char *prc,APPlib *errlst,int line,char *file)
{
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;
	char *subject=NULL,*temp=NULL,*temp1=NULL;
	int x=0;

	temp=RDA_GetEnv("DEV_LICENSE");
	if(temp!=NULL)
	{
		temp1=strtok(temp," ");
		if(CheckDevLicense(temp1)) 
		{
			return;
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagolh)
	{
		prterr("APPlib2SupportRequest for Product [%s] Process [%s] at line [%d] Program [%s].",(prd!=NULL ? prd:""),(prc!=NULL ? prc:""),line,file);
		SEEAPPlib(errlst);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("UTILITIES")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif

	gsv=RDAGenericSetupNew("UTILITIES","AUTOMATICALLY SUBMIT ERRORS AS HELPDESK TICKETS");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			x=*gsv->value.string_value;
		} else {
			x=(-1);
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	if(x<1) return;
	if(errlst!=NULL)
	{
		for(x=0;x<errlst->numlibs;++x)
		{
			temp=Rmalloc(RDAstrlen(errlst->libs[x])+2);
			sprintf(temp,"%s\n",errlst->libs[x]);
			if(subject!=NULL)
			{
				subject=Rrealloc(subject,(RDAstrlen(subject)+RDAstrlen(temp)+1));
			} else {
				subject=Rmalloc((RDAstrlen(temp)+1));
			}
			strcat(subject,temp);
			if(temp!=NULL) Rfree(temp);
			temp=NULL;
		}
		xAddSupportRequest(prd,prc,subject,line,file);
	}
}
