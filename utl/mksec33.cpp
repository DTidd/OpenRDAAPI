#include <cstdio>
#include <mkmtn.hpp>
#include <mkmsc.hpp>
#include <mkdef.hpp>
#include <mix.hpp>

extern char *module;
extern APPlib *errorlist;
#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */

static void AddProcessSecurity(char *id,char *module,char *process,char e)
{
	RDATData *prev=NULL;

	if(e!=TRUE) return;
	ZERNRD(PROCSEC_FILENO);
	FINDFLDSETSTRING(PROCSEC_FILENO,"USER IDENTIFICATION",id);
	FINDFLDSETSTRING(PROCSEC_FILENO,"MODULE NAME",module);
	FINDFLDSETSTRING(PROCSEC_FILENO,"PROCESS NAME",process);
	if(EQLNRD(PROCSEC_FILENO,1))
	{
		KEYNRD(PROCSEC_FILENO,1);
	} else prev=RDATDataNEW(PROCSEC_FILENO);
	FINDFLDSETCHAR(PROCSEC_FILENO,"EXECUTE PROCESS",e);
	FINDFLDSETCHAR(PROCSEC_FILENO,"DELETEFLAG",FALSE);
	WRTTRANS(PROCSEC_FILENO,0,NULL,prev);
	if(prev!=NULL) FreeRDATData(prev);
}	
static void AddFileSecurity(char *id,char *module,char *file,char f,char rd,char wd, char pd,char dd,char *record)
{
	RDATData *prev=NULL;

	if(rd!=TRUE) return;
	ZERNRD(FILESEC_FILENO);
	FINDFLDSETSTRING(FILESEC_FILENO,"USER IDENTIFICATION",id);
	FINDFLDSETSTRING(FILESEC_FILENO,"MODULE NAME",module);
	FINDFLDSETSTRING(FILESEC_FILENO,"FILE NAME",file);
	if(EQLNRD(FILESEC_FILENO,1))
	{
		KEYNRD(FILESEC_FILENO,1);
	} else prev=RDATDataNEW(FILESEC_FILENO);
	FINDFLDSETCHAR(FILESEC_FILENO,"FIELD SECURITY",f);
	FINDFLDSETCHAR(FILESEC_FILENO,"READ FILE",rd);
	FINDFLDSETCHAR(FILESEC_FILENO,"WRITE FILE",wd);
	FINDFLDSETCHAR(FILESEC_FILENO,"PRINT FILE",pd);
	FINDFLDSETCHAR(FILESEC_FILENO,"DELETE FILE",dd);
	FINDFLDSETSTRING(FILESEC_FILENO,"EXPRESSION",record);
	WRTTRANS(FILESEC_FILENO,0,NULL,prev);
	if(prev!=NULL) FreeRDATData(prev);
}
static void AddModuleSecurity(char *id,char *module,char ap,char af,char ef,char rd,char wd, char pd,char dd)
{
	RDATData *prev=NULL;

	if(rd!=TRUE) return;
	ZERNRD(MODSEC_FILENO);
	FINDFLDSETSTRING(MODSEC_FILENO,"USER IDENTIFICATION",id);
	FINDFLDSETSTRING(MODSEC_FILENO,"MODULE NAME",module);
	if(EQLNRD(MODSEC_FILENO,1))
	{
		KEYNRD(MODSEC_FILENO,1);
	} else prev=RDATDataNEW(MODSEC_FILENO);
	FINDFLDSETCHAR(MODSEC_FILENO,"DELETEFLAG",FALSE);
	FINDFLDSETCHAR(MODSEC_FILENO,"ALL PROCESSES",ap);
	FINDFLDSETCHAR(MODSEC_FILENO,"ALL FILES",af);
	FINDFLDSETCHAR(MODSEC_FILENO,"EXECUTE FUNCTIONS",ef);
	FINDFLDSETCHAR(MODSEC_FILENO,"READ DATA",rd);
	FINDFLDSETCHAR(MODSEC_FILENO,"WRITE DATA",wd);
	FINDFLDSETCHAR(MODSEC_FILENO,"PRINT DATA",pd);
	FINDFLDSETCHAR(MODSEC_FILENO,"DELETE DATA",dd);
	WRTTRANS(MODSEC_FILENO,0,NULL,prev);
	if(prev!=NULL) FreeRDATData(prev);
}
/* 

	ROLE DEFINITIONS

	admin		 -	Site Administrator (like principals, assistant principals, etc that need to have a combined outlook of Financials, Procurement and HR.
	acctmgr		 -	Supervisors or Administrators who participate in Expense Budgets, Account Management and Procurement
	data-entry	 -	Global data entry positions performing this function in each module and focus area
	data-entry-ar	 -	Data entry into Financial Receipts and Cash Drawer, access to Revenue and bank reconciliation, revenue being primary focus 
	data-entry-po	 - 	Data entry into procurement with access to Expense budgets, account managers access, and Procurement
	data-entry-hr	 - 	Data entry into HR, primarily employee absences, leave and substitutes, personnel reports with discretionary setup manual option
	data-entry-hrpo - 	Data entry into HR & PO, primarily employee absences, leave and substitutes, personnel reports with discretionary setup manual option and PO's

	NOTE:  You may combine roles using COPYSECURITY to blend access to become the sum of both roles, data-entry is a convenience with this thought in mind.

*/

void MakeStdUsers()
{
	short U=0,style=0,bt=2;
	RDATData *prev=NULL;
	int z=0;
	int ret_int=0;
	RDAacct *acc=NULL;
	char use_approve=FALSE,*libx=NULL,use_site=FALSE;
	short approve_type=FALSE;
	RDAGenericSetup *gsv=NULL;

	if(XPERT_SETUP->software_type>1) return;
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("PURORD")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"PURORD");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"PURORD");
#endif
	gsv=RDAGenericSetupNew("PURORD","APPROVE PO");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			use_approve=*gsv->value.string_value;
		} else {
			use_approve=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("PURORD","USE SITE APPROVAL");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			use_site=*gsv->value.string_value;
		} else {
			use_site=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("PURORD","APPROVAL TYPE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			approve_type=*gsv->value.short_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	U=OPNNRD("SECURITY","USERS");
	if(U!=(-1))
	{
		ZERNRD(U);
		FINDFLDSETSTRING(U,"USER IDENTIFICATION","admin");
		if(EQLNRD(U,1)) KEYNRD(U,1);
			else prev=RDATDataNEW(U);
		FINDFLDSETSTRING(U,"USER NAME","Administrator");
		FINDFLDSETSTRING(U,"MODULE NAME","PURORD");
		FINDFLDSETSTRING(U,"DEPARTMENT","admin");
		style=1;
		FINDFLDSETSHORT(U,"DOMENU STYLE",style);
		FINDFLDSETCHAR(U,"USE BROWSE LIST LABEL",TRUE);
#ifndef WIN32
		FINDFLDSETCHAR(U,"BROWSER TYPE",bt);
		FINDFLDSETSTRING(U,"DEFAULT PRINTER","viewpdf.lnx");
#endif
#ifdef WIN32
		bt=2;
		FINDFLDSETCHAR(U,"BROWSER TYPE",bt);
		FINDFLDSETSTRING(U,"DEFAULT PRINTER","viewpdf.exe");
#endif
		FINDFLDSETCHAR(U,"DELETEFLAG",FALSE);
		WRTTRANS(U,0,NULL,prev);
		if(prev!=NULL) FreeRDATData(prev);
		ZERNRD(U);
		FINDFLDSETSTRING(U,"USER IDENTIFICATION","acctmgr");
		if(EQLNRD(U,1)) KEYNRD(U,1);
			else prev=RDATDataNEW(U);
		FINDFLDSETSTRING(U,"MODULE NAME","FINMGT");
		FINDFLDSETSTRING(U,"DEPARTMENT","acctmgr");
		FINDFLDSETSTRING(U,"USER NAME","Account Manager");
		style=1;
		FINDFLDSETSHORT(U,"DOMENU STYLE",style);
		FINDFLDSETCHAR(U,"USE BROWSE LIST LABEL",TRUE);
#ifndef WIN32
		FINDFLDSETCHAR(U,"BROWSER TYPE",bt);
		FINDFLDSETSTRING(U,"DEFAULT PRINTER","viewpdf.lnx");
#endif
#ifdef WIN32
		bt=2;
		FINDFLDSETCHAR(U,"BROWSER TYPE",bt);
		FINDFLDSETSTRING(U,"DEFAULT PRINTER","viewpdf.exe");
#endif
		FINDFLDSETCHAR(U,"DELETEFLAG",FALSE);
		WRTTRANS(U,0,NULL,prev);
		if(prev!=NULL) FreeRDATData(prev);

		ZERNRD(U);
		FINDFLDSETSTRING(U,"USER IDENTIFICATION","data-entry");
		if(EQLNRD(U,1)) KEYNRD(U,1);
			else prev=RDATDataNEW(U);
		FINDFLDSETSTRING(U,"MODULE NAME","PURORD");
		FINDFLDSETSTRING(U,"DEPARTMENT","data-entry");
		FINDFLDSETSTRING(U,"USER NAME","Limited Data Entry");
		style=2;
		FINDFLDSETSHORT(U,"DOMENU STYLE",style);
		FINDFLDSETCHAR(U,"USE BROWSE LIST LABEL",TRUE);
#ifndef WIN32
		FINDFLDSETCHAR(U,"BROWSER TYPE",bt);
		FINDFLDSETSTRING(U,"DEFAULT PRINTER","viewpdf.lnx");
#endif
#ifdef WIN32
		bt=2;
		FINDFLDSETCHAR(U,"BROWSER TYPE",bt);
		FINDFLDSETSTRING(U,"DEFAULT PRINTER","viewpdf.exe");
#endif
		FINDFLDSETCHAR(U,"DELETEFLAG",FALSE);
		WRTTRANS(U,0,NULL,prev);
		if(prev!=NULL) FreeRDATData(prev);

		ZERNRD(U);
		FINDFLDSETSTRING(U,"USER IDENTIFICATION","data-entry-ar");
		if(EQLNRD(U,1)) KEYNRD(U,1);
			else prev=RDATDataNEW(U);
		FINDFLDSETSTRING(U,"MODULE NAME","FINMGT");
		FINDFLDSETSTRING(U,"DEPARTMENT","data-entry-ar");
		FINDFLDSETSTRING(U,"USER NAME","Limited Data Entry A/R");
		style=2;
		FINDFLDSETSHORT(U,"DOMENU STYLE",style);
		FINDFLDSETCHAR(U,"USE BROWSE LIST LABEL",TRUE);
#ifndef WIN32
		FINDFLDSETCHAR(U,"BROWSER TYPE",bt);
		FINDFLDSETSTRING(U,"DEFAULT PRINTER","viewpdf.lnx");
#endif
#ifdef WIN32
		bt=2;
		FINDFLDSETCHAR(U,"BROWSER TYPE",bt);
		FINDFLDSETSTRING(U,"DEFAULT PRINTER","viewpdf.exe");
#endif
		FINDFLDSETCHAR(U,"DELETEFLAG",FALSE);
		WRTTRANS(U,0,NULL,prev);
		if(prev!=NULL) FreeRDATData(prev);

		ZERNRD(U);
		FINDFLDSETSTRING(U,"USER IDENTIFICATION","data-entry-po");
		if(EQLNRD(U,1)) KEYNRD(U,1);
			else prev=RDATDataNEW(U);
		FINDFLDSETSTRING(U,"MODULE NAME","PURORD");
		FINDFLDSETSTRING(U,"DEPARTMENT","data-entry-po");
		FINDFLDSETSTRING(U,"USER NAME","Limited Data Entry Procurement");
		style=2;
		FINDFLDSETSHORT(U,"DOMENU STYLE",style);
		FINDFLDSETCHAR(U,"USE BROWSE LIST LABEL",TRUE);
#ifndef WIN32
		FINDFLDSETCHAR(U,"BROWSER TYPE",bt);
		FINDFLDSETSTRING(U,"DEFAULT PRINTER","viewpdf.lnx");
#endif
#ifdef WIN32
		bt=2;
		FINDFLDSETCHAR(U,"BROWSER TYPE",bt);
		FINDFLDSETSTRING(U,"DEFAULT PRINTER","viewpdf.exe");
#endif
		FINDFLDSETCHAR(U,"DELETEFLAG",FALSE);
		WRTTRANS(U,0,NULL,prev);
		if(prev!=NULL) FreeRDATData(prev);

		ZERNRD(U);
		FINDFLDSETSTRING(U,"USER IDENTIFICATION","data-entry-hr");
		if(EQLNRD(U,1)) KEYNRD(U,1);
			else prev=RDATDataNEW(U);
		FINDFLDSETSTRING(U,"MODULE NAME","EMPABS");
		FINDFLDSETSTRING(U,"DEPARTMENT","data-entry-hr");
		FINDFLDSETSTRING(U,"USER NAME","Limited Data Entry HR");
		style=2;
		FINDFLDSETSHORT(U,"DOMENU STYLE",style);
		FINDFLDSETCHAR(U,"USE BROWSE LIST LABEL",TRUE);
#ifndef WIN32
		FINDFLDSETCHAR(U,"BROWSER TYPE",bt);
		FINDFLDSETSTRING(U,"DEFAULT PRINTER","viewpdf.lnx");
#endif
#ifdef WIN32
		bt=2;
		FINDFLDSETCHAR(U,"BROWSER TYPE",bt);
		FINDFLDSETSTRING(U,"DEFAULT PRINTER","viewpdf.exe");
#endif
		FINDFLDSETCHAR(U,"DELETEFLAG",FALSE);
		WRTTRANS(U,0,NULL,prev);
		if(prev!=NULL) FreeRDATData(prev);

		ZERNRD(U);
		FINDFLDSETSTRING(U,"USER IDENTIFICATION","data-entry-hrpo");
		if(EQLNRD(U,1)) KEYNRD(U,1);
			else prev=RDATDataNEW(U);
		FINDFLDSETSTRING(U,"MODULE NAME","PURORD");
		FINDFLDSETSTRING(U,"DEPARTMENT","data-entry-hrpo");
		FINDFLDSETSTRING(U,"USER NAME","Limited Data Entry HR & PO");
		style=2;
		FINDFLDSETSHORT(U,"DOMENU STYLE",style);
		FINDFLDSETCHAR(U,"USE BROWSE LIST LABEL",TRUE);
#ifndef WIN32
		FINDFLDSETCHAR(U,"BROWSER TYPE",bt);
		FINDFLDSETSTRING(U,"DEFAULT PRINTER","viewpdf.lnx");
#endif
#ifdef WIN32
		bt=2;
		FINDFLDSETCHAR(U,"BROWSER TYPE",bt);
		FINDFLDSETSTRING(U,"DEFAULT PRINTER","viewpdf.exe");
#endif
		FINDFLDSETCHAR(U,"DELETEFLAG",FALSE);
		WRTTRANS(U,0,NULL,prev);
		if(prev!=NULL) FreeRDATData(prev);
		CLSNRD(U);
	}
	if(MODSEC_FILENO==(-1)) MODSEC_FILENO=OPNNRD("SECURITY","MODSEC");
	if(PROCSEC_FILENO==(-1)) PROCSEC_FILENO=OPNNRD("SECURITY","PROCSEC");
	if(FILESEC_FILENO==(-1)) FILESEC_FILENO=OPNNRD("SECURITY","FILESEC");
	if(MODSEC_FILENO!=(-1))
	{
		RDAmkdir("admin",00770,&ret_int);
		RDAmkdir("acctmgr",00770,&ret_int);
		RDAmkdir("data-entry",00770,&ret_int);
		RDAmkdir("data-entry-ar",00770,&ret_int);
		RDAmkdir("data-entry-hr",00770,&ret_int);
		RDAmkdir("data-entry-po",00770,&ret_int);
		RDAmkdir("data-entry-hrpo",00770,&ret_int);

		AddModuleSecurity("admin","GUI",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
		AddModuleSecurity("admin","OLHELP",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		AddModuleSecurity("admin","RPTGEN",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
		AddModuleSecurity("admin","SECURITY",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
		AddModuleSecurity("admin","UTILITIES",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);

		AddModuleSecurity("acctmgr","GUI",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
		AddModuleSecurity("acctmgr","OLHELP",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		AddModuleSecurity("acctmgr","RPTGEN",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
		AddModuleSecurity("acctmgr","SECURITY",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
		AddModuleSecurity("acctmgr","UTILITIES",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);

		AddModuleSecurity("data-entry","GUI",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
		AddModuleSecurity("data-entry","OLHELP",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		AddModuleSecurity("data-entry","RPTGEN",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
		AddModuleSecurity("data-entry","SECURITY",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
		AddModuleSecurity("data-entry","UTILITIES",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);

		AddModuleSecurity("data-entry-ar","GUI",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
		AddModuleSecurity("data-entry-ar","OLHELP",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		AddModuleSecurity("data-entry-ar","RPTGEN",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
		AddModuleSecurity("data-entry-ar","SECURITY",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
		AddModuleSecurity("data-entry-ar","UTILITIES",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);


		AddModuleSecurity("data-entry-hr","GUI",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
		AddModuleSecurity("data-entry-hr","OLHELP",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		AddModuleSecurity("data-entry-hr","RPTGEN",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
		AddModuleSecurity("data-entry-hr","SECURITY",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
		AddModuleSecurity("data-entry-hr","UTILITIES",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);


		AddModuleSecurity("data-entry-po","GUI",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
		AddModuleSecurity("data-entry-po","OLHELP",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		AddModuleSecurity("data-entry-po","RPTGEN",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
		AddModuleSecurity("data-entry-po","SECURITY",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
		AddModuleSecurity("data-entry-po","UTILITIES",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);

		AddModuleSecurity("data-entry-hrpo","GUI",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
		AddModuleSecurity("data-entry-hrpo","OLHELP",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		AddModuleSecurity("data-entry-hrpo","RPTGEN",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
		AddModuleSecurity("data-entry-hrpo","SECURITY",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
		AddModuleSecurity("data-entry-hrpo","UTILITIES",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);



		AddProcessSecurity("admin","RPTGEN","DO REPORTS",TRUE);
		AddProcessSecurity("admin","RPTGEN","EZ REPORT WRITER",TRUE);
		AddProcessSecurity("admin","RPTGEN","LASER FORM TEST",TRUE);
		AddProcessSecurity("admin","RPTGEN","OK ACKNOWLEDGEMENT",TRUE);
		AddProcessSecurity("admin","UTILITIES","TEXT BROWSER",TRUE);
		AddProcessSecurity("admin","UTILITIES","MAIN MENU",TRUE);
		AddProcessSecurity("admin","UTILITIES","DOHTTP",TRUE);

		AddProcessSecurity("acctmgr","RPTGEN","DO REPORTS",TRUE);
		AddProcessSecurity("acctmgr","RPTGEN","EZ REPORT WRITER",TRUE);
		AddProcessSecurity("acctmgr","RPTGEN","LASER FORM TEST",TRUE);
		AddProcessSecurity("acctmgr","RPTGEN","OK ACKNOWLEDGEMENT",TRUE);
		AddProcessSecurity("acctmgr","UTILITIES","TEXT BROWSER",TRUE);
		AddProcessSecurity("acctmgr","UTILITIES","MAIN MENU",TRUE);
		AddProcessSecurity("acctmgr","UTILITIES","DOHTTP",TRUE);

		AddProcessSecurity("data-entry","RPTGEN","DO REPORTS",TRUE);
		AddProcessSecurity("data-entry","RPTGEN","EZ REPORT WRITER",TRUE);
		AddProcessSecurity("data-entry","RPTGEN","LASER FORM TEST",TRUE);
		AddProcessSecurity("data-entry","RPTGEN","OK ACKNOWLEDGEMENT",TRUE);
		AddProcessSecurity("data-entry","UTILITIES","TEXT BROWSER",TRUE);
		AddProcessSecurity("data-entry","UTILITIES","MAIN MENU",TRUE);
		AddProcessSecurity("data-entry","UTILITIES","DOHTTP",TRUE);

		AddProcessSecurity("data-entry-ar","RPTGEN","DO REPORTS",TRUE);
		AddProcessSecurity("data-entry-ar","RPTGEN","EZ REPORT WRITER",TRUE);
		AddProcessSecurity("data-entry-ar","RPTGEN","LASER FORM TEST",TRUE);
		AddProcessSecurity("data-entry-ar","RPTGEN","OK ACKNOWLEDGEMENT",TRUE);
		AddProcessSecurity("data-entry-ar","UTILITIES","TEXT BROWSER",TRUE);
		AddProcessSecurity("data-entry-ar","UTILITIES","MAIN MENU",TRUE);
		AddProcessSecurity("data-entry-ar","UTILITIES","DOHTTP",TRUE);

		AddProcessSecurity("data-entry-po","RPTGEN","DO REPORTS",TRUE);
		AddProcessSecurity("data-entry-po","RPTGEN","EZ REPORT WRITER",TRUE);
		AddProcessSecurity("data-entry-po","RPTGEN","LASER FORM TEST",TRUE);
		AddProcessSecurity("data-entry-po","RPTGEN","OK ACKNOWLEDGEMENT",TRUE);
		AddProcessSecurity("data-entry-po","UTILITIES","TEXT BROWSER",TRUE);
		AddProcessSecurity("data-entry-po","UTILITIES","MAIN MENU",TRUE);
		AddProcessSecurity("data-entry-po","UTILITIES","DOHTTP",TRUE);

		AddProcessSecurity("data-entry-hr","RPTGEN","DO REPORTS",TRUE);
		AddProcessSecurity("data-entry-hr","RPTGEN","EZ REPORT WRITER",TRUE);
		AddProcessSecurity("data-entry-hr","RPTGEN","LASER FORM TEST",TRUE);
		AddProcessSecurity("data-entry-hr","RPTGEN","OK ACKNOWLEDGEMENT",TRUE);
		AddProcessSecurity("data-entry-hr","UTILITIES","TEXT BROWSER",TRUE);
		AddProcessSecurity("data-entry-hr","UTILITIES","MAIN MENU",TRUE);
		AddProcessSecurity("data-entry-hr","UTILITIES","DOHTTP",TRUE);

		AddProcessSecurity("data-entry-hrpo","RPTGEN","DO REPORTS",TRUE);
		AddProcessSecurity("data-entry-hrpo","RPTGEN","EZ REPORT WRITER",TRUE);
		AddProcessSecurity("data-entry-hrpo","RPTGEN","LASER FORM TEST",TRUE);
		AddProcessSecurity("data-entry-hrpo","RPTGEN","OK ACKNOWLEDGEMENT",TRUE);
		AddProcessSecurity("data-entry-hrpo","UTILITIES","TEXT BROWSER",TRUE);
		AddProcessSecurity("data-entry-hrpo","UTILITIES","MAIN MENU",TRUE);
		AddProcessSecurity("data-entry-hrpo","UTILITIES","DOHTTP",TRUE);

		AddProcessSecurity("admin","RPTGEN","DO REPORTS",TRUE);
		AddProcessSecurity("admin","RPTGEN","EZ REPORT WRITER",TRUE);
		AddProcessSecurity("admin","RPTGEN","LASER FORM TEST",TRUE);
		AddProcessSecurity("admin","RPTGEN","OK ACKNOWLEDGEMENT",TRUE);
		AddProcessSecurity("admin","UTILITIES","TEXT BROWSER",TRUE);
		AddProcessSecurity("admin","UTILITIES","MAIN MENU",TRUE);
		AddProcessSecurity("admin","UTILITIES","DOHTTP",TRUE);

		if(XPERT_SETUP->FINMGT==TRUE)
		{
			AddModuleSecurity("data-entry","FINMGT",FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-ar","FINMGT",FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-po","FINMGT",FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-hrpo","FINMGT",FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-hr","FINMGT",FALSE,FALSE,FALSE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("acctmgr","FINMGT",FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("admin","FINMGT",FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);

			AddFileSecurity("data-entry","FINMGT","FINALIAS",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry","FINMGT","FINBACC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","FINMGT","FINBACT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","FINMGT","FINBREF",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","FINMGT","FINBYR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","FINMGT","FINCOA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","FINMGT","FINEACC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry","FINMGT","FINEACT",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry","FINMGT","FINEREF",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","FINMGT","FINEVA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","FINMGT","FINEYR",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry","FINMGT","FINFND",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","FINMGT","FINFTP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","FINMGT","FINGRNT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","FINMGT","FINGRP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","FINMGT","FINJEN",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry","FINMGT","FINJRNL",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","FINMGT","FINRACC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry","FINMGT","FINRACT",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry","FINMGT","FINREC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry","FINMGT","FINRJEN",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry","FINMGT","FINRJRNL",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","FINMGT","FINRMSR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","FINMGT","FINRR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","FINMGT","FINRREC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry","FINMGT","FINRREF",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","FINMGT","FINRVA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","FINMGT","FINRYR",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry","FINMGT","FINSBC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","FINMGT","FINYEAR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","FINMGT","FINRSRC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","FINMGT","FINRID",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			

			AddFileSecurity("data-entry-ar","FINMGT","FINALIAS",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-ar","FINMGT","FINBACC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","FINMGT","FINBACT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","FINMGT","FINBREF",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","FINMGT","FINBYR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","FINMGT","FINCOA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","FINMGT","FINEACC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-ar","FINMGT","FINEACT",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-ar","FINMGT","FINEREF",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","FINMGT","FINEVA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","FINMGT","FINEYR",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-ar","FINMGT","FINFND",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","FINMGT","FINFTP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","FINMGT","FINGRNT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","FINMGT","FINGRP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","FINMGT","FINJEN",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-ar","FINMGT","FINJRNL",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","FINMGT","FINRACC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-ar","FINMGT","FINRACT",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-ar","FINMGT","FINREC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-ar","FINMGT","FINRJEN",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-ar","FINMGT","FINRJRNL",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","FINMGT","FINRMSR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","FINMGT","FINRR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","FINMGT","FINRREC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-ar","FINMGT","FINRREF",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","FINMGT","FINRVA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","FINMGT","FINRYR",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-ar","FINMGT","FINSBC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","FINMGT","FINYEAR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","FINMGT","FINRSRC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","FINMGT","FINRID",FALSE,TRUE,TRUE,TRUE,FALSE,"");


			AddFileSecurity("data-entry-hr","FINMGT","FINALIAS",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-hr","FINMGT","FINBACC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","FINMGT","FINBACT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","FINMGT","FINBREF",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","FINMGT","FINBYR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","FINMGT","FINCOA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","FINMGT","FINEACC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-hr","FINMGT","FINEACT",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-hr","FINMGT","FINEREF",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","FINMGT","FINEVA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","FINMGT","FINEYR",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-hr","FINMGT","FINFND",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","FINMGT","FINFTP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","FINMGT","FINGRNT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","FINMGT","FINGRP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","FINMGT","FINJEN",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-hr","FINMGT","FINJRNL",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","FINMGT","FINRACC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-hr","FINMGT","FINRACT",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-hr","FINMGT","FINREC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-hr","FINMGT","FINRJEN",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-hr","FINMGT","FINRJRNL",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","FINMGT","FINRMSR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","FINMGT","FINRR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","FINMGT","FINRREC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-hr","FINMGT","FINRREF",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","FINMGT","FINRVA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","FINMGT","FINRYR",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-hr","FINMGT","FINSBC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","FINMGT","FINYEAR",FALSE,TRUE,TRUE,TRUE,FALSE,"");


			AddFileSecurity("data-entry-po","FINMGT","FINALIAS",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-po","FINMGT","FINBACC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","FINMGT","FINBACT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","FINMGT","FINBREF",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","FINMGT","FINBYR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","FINMGT","FINCOA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","FINMGT","FINEACC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-po","FINMGT","FINEACT",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-po","FINMGT","FINEREF",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","FINMGT","FINEVA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","FINMGT","FINEYR",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-po","FINMGT","FINFND",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","FINMGT","FINFTP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","FINMGT","FINGRNT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","FINMGT","FINGRP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","FINMGT","FINJEN",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-po","FINMGT","FINJRNL",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","FINMGT","FINRACC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-po","FINMGT","FINRACT",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-po","FINMGT","FINREC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-po","FINMGT","FINRJEN",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-po","FINMGT","FINRJRNL",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","FINMGT","FINRMSR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","FINMGT","FINRR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","FINMGT","FINRREC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-po","FINMGT","FINRREF",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","FINMGT","FINRVA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","FINMGT","FINRYR",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-po","FINMGT","FINSBC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","FINMGT","FINYEAR",FALSE,TRUE,TRUE,TRUE,FALSE,"");


			AddFileSecurity("data-entry-hrpo","FINMGT","FINALIAS",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINBACC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINBACT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINBREF",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINBYR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINCOA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINEACC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINEACT",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINEREF",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINEVA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINEYR",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINFND",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINFTP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINGRNT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINGRP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINJEN",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINJRNL",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINRACC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINRACT",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINREC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINRJEN",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINRJRNL",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINRMSR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINRR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINRREC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINRREF",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINRVA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINRYR",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINSBC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","FINMGT","FINYEAR",FALSE,TRUE,TRUE,TRUE,FALSE,"");


			AddFileSecurity("acctmgr","FINMGT","FINALIAS",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("acctmgr","FINMGT","FINBACC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","FINMGT","FINBACT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","FINMGT","FINBREF",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","FINMGT","FINBYR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","FINMGT","FINCOA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","FINMGT","FINEACC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("acctmgr","FINMGT","FINEACT",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("acctmgr","FINMGT","FINEREF",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","FINMGT","FINEVA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","FINMGT","FINEYR",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("acctmgr","FINMGT","FINFND",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","FINMGT","FINFTP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","FINMGT","FINGRNT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","FINMGT","FINGRP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","FINMGT","FINJEN",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("acctmgr","FINMGT","FINJRNL",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","FINMGT","FINRACC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("acctmgr","FINMGT","FINRACT",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("acctmgr","FINMGT","FINREC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("acctmgr","FINMGT","FINRJEN",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("acctmgr","FINMGT","FINRJRNL",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","FINMGT","FINRMSR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","FINMGT","FINRR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","FINMGT","FINRREC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("acctmgr","FINMGT","FINRREF",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","FINMGT","FINRVA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","FINMGT","FINRYR",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("acctmgr","FINMGT","FINSBC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","FINMGT","FINYEAR",FALSE,TRUE,TRUE,TRUE,FALSE,"");

			AddFileSecurity("admin","FINMGT","FINALIAS",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("admin","FINMGT","FINBACC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","FINMGT","FINBACT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","FINMGT","FINBREF",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","FINMGT","FINBYR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","FINMGT","FINCOA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","FINMGT","FINEACC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("admin","FINMGT","FINEACT",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("admin","FINMGT","FINEREF",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","FINMGT","FINEVA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","FINMGT","FINEYR",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("admin","FINMGT","FINFND",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","FINMGT","FINFTP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","FINMGT","FINGRNT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","FINMGT","FINGRP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","FINMGT","FINJEN",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("admin","FINMGT","FINJRNL",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","FINMGT","FINRACC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("admin","FINMGT","FINRACT",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("admin","FINMGT","FINREC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("admin","FINMGT","FINRJEN",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("admin","FINMGT","FINRJRNL",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","FINMGT","FINRMSR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","FINMGT","FINRR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","FINMGT","FINRREC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("admin","FINMGT","FINRREF",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","FINMGT","FINRVA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","FINMGT","FINRYR",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("admin","FINMGT","FINSBC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","FINMGT","FINYEAR",FALSE,TRUE,TRUE,TRUE,FALSE,"");


			AddProcessSecurity("acctmgr","FINMGT","ACTOUT",FALSE);
			AddProcessSecurity("acctmgr","FINMGT","ACTRPT",FALSE);
			AddProcessSecurity("acctmgr","FINMGT","MTN FINEACC",TRUE);
			AddProcessSecurity("acctmgr","FINMGT","MTN FINEACC.SEARCH",TRUE);
			AddProcessSecurity("acctmgr","FINMGT","MTN FINEACT",TRUE);
			AddProcessSecurity("acctmgr","FINMGT","MTN FINEACT.BROWSE",TRUE);
			AddProcessSecurity("acctmgr","FINMGT","MTN FINEACT.SEARCH",TRUE);
			AddProcessSecurity("acctmgr","FINMGT","MTN FINEYR",TRUE);
			AddProcessSecurity("acctmgr","FINMGT","MTN FINEYR.BROWSE",TRUE);
			AddProcessSecurity("acctmgr","FINMGT","MTN FINEYR.SEARCH",TRUE);
			AddProcessSecurity("acctmgr","FINMGT","MTN FINRACC",TRUE);
			AddProcessSecurity("acctmgr","FINMGT","MTN FINRACC.SEARCH",TRUE);
			AddProcessSecurity("acctmgr","FINMGT","MTN FINRACT",TRUE);
			AddProcessSecurity("acctmgr","FINMGT","MTN FINRACT.BROWSE",TRUE);
			AddProcessSecurity("acctmgr","FINMGT","MTN FINRACT.SEARCH",TRUE);
			AddProcessSecurity("acctmgr","FINMGT","MTN FINRYR",TRUE);
			AddProcessSecurity("acctmgr","FINMGT","MTN FINRYR.BROWSE",TRUE);
			AddProcessSecurity("acctmgr","FINMGT","MTN FINRYR.SEARCH",TRUE);
			AddProcessSecurity("acctmgr","FINMGT","SUMMARY INQUIRY",TRUE);
			AddProcessSecurity("acctmgr","FINMGT","DETAIL QUERY",TRUE);
			AddProcessSecurity("acctmgr","FINMGT","WORK-ORDER",TRUE);

			AddProcessSecurity("admin","FINMGT","ACTOUT",FALSE);
			AddProcessSecurity("admin","FINMGT","ACTRPT",FALSE);
			AddProcessSecurity("admin","FINMGT","MTN FINEACC",TRUE);
			AddProcessSecurity("admin","FINMGT","MTN FINEACC.SEARCH",TRUE);
			AddProcessSecurity("admin","FINMGT","MTN FINEACT",TRUE);
			AddProcessSecurity("admin","FINMGT","MTN FINEACT.BROWSE",TRUE);
			AddProcessSecurity("admin","FINMGT","MTN FINEACT.SEARCH",TRUE);
			AddProcessSecurity("admin","FINMGT","MTN FINEYR",TRUE);
			AddProcessSecurity("admin","FINMGT","MTN FINEYR.BROWSE",TRUE);
			AddProcessSecurity("admin","FINMGT","MTN FINEYR.SEARCH",TRUE);
			AddProcessSecurity("admin","FINMGT","MTN FINRACC",TRUE);
			AddProcessSecurity("admin","FINMGT","MTN FINRACC.SEARCH",TRUE);
			AddProcessSecurity("admin","FINMGT","MTN FINRACT",TRUE);
			AddProcessSecurity("admin","FINMGT","MTN FINRACT.BROWSE",TRUE);
			AddProcessSecurity("admin","FINMGT","MTN FINRACT.SEARCH",TRUE);
			AddProcessSecurity("admin","FINMGT","MTN FINRYR",TRUE);
			AddProcessSecurity("admin","FINMGT","MTN FINRYR.BROWSE",TRUE);
			AddProcessSecurity("admin","FINMGT","MTN FINRYR.SEARCH",TRUE);
			AddProcessSecurity("admin","FINMGT","SUMMARY INQUIRY",TRUE);
			AddProcessSecurity("admin","FINMGT","DETAIL QUERY",TRUE);
			AddProcessSecurity("admin","FINMGT","WORK-ORDER",TRUE);



			AddProcessSecurity("data-entry","FINMGT","ACTOUT",FALSE);
			AddProcessSecurity("data-entry","FINMGT","ACTRPT",FALSE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINEACC",TRUE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINEACC.SEARCH",TRUE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINEACT",TRUE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINEACT.BROWSE",TRUE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINEACT.SEARCH",TRUE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINEYR",TRUE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINEYR.BROWSE",TRUE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINEYR.SEARCH",TRUE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINRACC",TRUE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINRACC.SEARCH",TRUE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINRACT",TRUE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINRACT.BROWSE",TRUE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINRACT.SEARCH",TRUE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINRYR",TRUE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINRYR.BROWSE",TRUE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINRYR.SEARCH",TRUE);
			AddProcessSecurity("data-entry","FINMGT","SUMMARY INQUIRY",TRUE);
			AddProcessSecurity("data-entry","FINMGT","DETAIL QUERY",TRUE);
			AddProcessSecurity("data-entry","FINMGT","WORK-ORDER",TRUE);

			AddProcessSecurity("data-entry-ar","FINMGT","ACTOUT",FALSE);
			AddProcessSecurity("data-entry-ar","FINMGT","ACTRPT",FALSE);
			AddProcessSecurity("data-entry-ar","FINMGT","MTN FINRACC",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","MTN FINRACC.SEARCH",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","MTN FINRACT",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","MTN FINRACT.BROWSE",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","MTN FINRACT.SEARCH",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","MTN FINRYR",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","MTN FINRYR.BROWSE",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","MTN FINRYR.SEARCH",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","SUMMARY INQUIRY",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","DETAIL QUERY",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","WORK-ORDER",TRUE);

			AddProcessSecurity("data-entry-hrpo","FINMGT","SUMMARY INQUIRY",TRUE);
			AddProcessSecurity("data-entry-hrpo","FINMGT","DETAIL QUERY",TRUE);

			AddProcessSecurity("data-entry-po","FINMGT","SUMMARY INQUIRY",TRUE);
			AddProcessSecurity("data-entry-po","FINMGT","DETAIL QUERY",TRUE);



			if(EXPENDITURE_ACCOUNT!=NULL)
			{
			if(EXPENDITURE_ACCOUNT->codetype!=NULL)
			{
			for(z=0,acc=EXPENDITURE_ACCOUNT->codetype;z<EXPENDITURE_ACCOUNT->num;++z,++acc)
			{
				memset(stemp,0,101);
				sprintf(stemp,"EACC#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-ar","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				sprintf(stemp,"ERPT#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				sprintf(stemp,"ERPTD#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				sprintf(stemp,"ESTAT#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				sprintf(stemp,"ESTATD#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"EWORK-ORDER");
				} else sprintf(stemp,"EWORK-ORDER#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-po","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-hrpo","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"EXPACT-ORDER");
				} else sprintf(stemp,"EXPACT-ORDER#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"EXPACT-ORDERB");
				} else sprintf(stemp,"EXPACT-ORDERB#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"EXPDET-ORDER-B");
				} else sprintf(stemp,"EXPDET-ORDER-B#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"EXPSUM-ORDER");
				} else sprintf(stemp,"EXPSUM-ORDER#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-po","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-hrpo","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"EXPSUM-CSV");
				} else sprintf(stemp,"EXPSUM-CSV#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"EXPTEN-ORDER");
				} else sprintf(stemp,"EXPTEN-ORDER#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-po","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-hrpo","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"EXPSUM-ORDERB");
				} else sprintf(stemp,"EXPSUM-ORDERB#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-po","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-hrpo","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				sprintf(stemp,"FINMGT EWORK-ORDER#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-po","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-hrpo","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				sprintf(stemp,"FINMGT WORK-ORDER#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-ar","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"WORK-ORDER");
				} else sprintf(stemp,"WORK-ORDER#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-ar","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"RPT EXPACT-ORDER");
				} else sprintf(stemp,"RPT EXPACT-ORDER#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"RPT EXPACT-ORDERB");
				} else sprintf(stemp,"RPT EXPACT-ORDERB#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"RPT EXPSUM-CSV");
				} else sprintf(stemp,"RPT EXPSUM-CSV#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
			}
			}
			}
			if(REVENUE_ACCOUNT!=NULL)
			{
			if(REVENUE_ACCOUNT->codetype!=NULL)
			{
			for(z=0,acc=REVENUE_ACCOUNT->codetype;z<REVENUE_ACCOUNT->num;++z,++acc)
			{
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"RPT REVACT-ORDER");
				} else sprintf(stemp,"RPT REVACT-ORDER#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-ar","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"RPT REVACT-ORDERB");
				} else sprintf(stemp,"RPT REVACT-ORDERB#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-ar","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"RPT REVDET-ORDER-B");
				} else sprintf(stemp,"RPT REVDET-ORDER-B#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-ar","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"RPT REVSUMA-ORDER");
				} else sprintf(stemp,"RPT REVSUMA-ORDER#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-ar","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"RPT REVSUM-ORDER");
				} else sprintf(stemp,"RPT REVSUM-ORDER#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-ar","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"RPT REVSUM-CSV");
				} else sprintf(stemp,"RPT REVSUM-CSV#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-ar","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"RPT REVTEN-ORDER");
				} else sprintf(stemp,"RPT REVTEN-ORDER#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-ar","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				sprintf(stemp,"RSTATD#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-ar","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"RWORK-ORDER");
				} else sprintf(stemp,"RWORK-ORDER#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-ar","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"REVACT-ORDER");
				} else sprintf(stemp,"REVACT-ORDER#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-ar","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"REVACT-ORDERB");
				} else sprintf(stemp,"REVACT-ORDERB#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-ar","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"REVSUM-ORDER");
				} else sprintf(stemp,"REVSUM-ORDER#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-ar","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"REVSUM-CSV");
				} else sprintf(stemp,"REVSUM-CSV#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-ar","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"REVSUM-ORDERB");
				} else sprintf(stemp,"REVSUM-ORDERB#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-ar","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				if(z==0)
				{
					sprintf(stemp,"REVTEN-ORDER");
				} else sprintf(stemp,"REVTEN-ORDER#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-ar","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				sprintf(stemp,"RACC#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-ar","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				sprintf(stemp,"RRPT#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-ar","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				sprintf(stemp,"RRPTD#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-ar","FINMGT",stemp,FALSE);
				memset(stemp,0,101);
				sprintf(stemp,"RSTAT#%d",z);
				AddProcessSecurity("admin","FINMGT",stemp,TRUE);
				AddProcessSecurity("acctmgr","FINMGT",stemp,TRUE);
				AddProcessSecurity("data-entry","FINMGT",stemp,FALSE);
				AddProcessSecurity("data-entry-ar","FINMGT",stemp,FALSE);
			}
			}
			}


			AddProcessSecurity("data-entry","FINMGT","ADD RECEIPTS",TRUE);
			AddProcessSecurity("data-entry","FINMGT","ADD MULTIPLE RECEIPTS",TRUE);
			AddProcessSecurity("data-entry","FINMGT","APPLY RECURRING RECEIPTS",TRUE);
			AddProcessSecurity("data-entry","FINMGT","FINARRECE",TRUE);
			AddProcessSecurity("data-entry","FINMGT","FINARRECP",TRUE);
			AddProcessSecurity("data-entry","FINMGT","INTERFUND BANK TRANSFERS",FALSE);
			AddProcessSecurity("data-entry","FINMGT","INTRAFUND ELECTRONIC TRANSFERS",FALSE);
			AddProcessSecurity("data-entry","FINMGT","FINMGT CHGRCJOURNALID",TRUE);
			AddProcessSecurity("data-entry","FINMGT","FINMGT DOREPORT",TRUE);
			AddProcessSecurity("data-entry","FINMGT","FINMGT RECAUDIT",TRUE);
			AddProcessSecurity("data-entry","FINMGT","FINMGT RECREPORTDATE",TRUE);
			AddProcessSecurity("data-entry","FINMGT","FINMGT REVERSERECEIPT",TRUE);
			AddProcessSecurity("data-entry","FINMGT","FINMGT RPT CHGDEPOSITDATE",TRUE);
			AddProcessSecurity("data-entry","FINMGT","FINMGT RPT FINBNKSUM",TRUE);
			AddProcessSecurity("data-entry","FINMGT","FINMGT UNDORECAUDIT",TRUE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINREC",TRUE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINREC.BROWSE",TRUE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINREC.SEARCHBROWSE",TRUE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINRJEN",TRUE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINRJEN.BROWSE",TRUE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINRJRNL",TRUE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINRMSR",TRUE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINRMSR.BROWSE",TRUE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINRR",TRUE);
			AddProcessSecurity("data-entry","FINMGT","MTN FINRREC",TRUE);
			AddProcessSecurity("data-entry","FINMGT","RECAUDIT",TRUE);
			AddProcessSecurity("data-entry","FINMGT","RECAUDIT1",TRUE);
			AddProcessSecurity("data-entry","FINMGT","RECAUDITID",TRUE);
			AddProcessSecurity("data-entry","FINMGT","RECAUDITREF",TRUE);
			AddProcessSecurity("data-entry","FINMGT","RECDEPOSIT",TRUE);
			AddProcessSecurity("data-entry","FINMGT","RECDEPOSIT1",TRUE);
			AddProcessSecurity("data-entry","FINMGT","RECDISTEZ",TRUE);
			AddProcessSecurity("data-entry","FINMGT","RECDIST1EZ",TRUE);
			AddProcessSecurity("data-entry","FINMGT","RECDIST",TRUE);
			AddProcessSecurity("data-entry","FINMGT","RECDIST-G",TRUE);
			AddProcessSecurity("data-entry","FINMGT","RECDIST1",TRUE);
			AddProcessSecurity("data-entry","FINMGT","RECDIST1-G",TRUE);
			AddProcessSecurity("data-entry","FINMGT","RECEIPT AUDIT REPORT",TRUE);
			AddProcessSecurity("data-entry","FINMGT","RECEIPT AUDIT REPORT BY RECEIPT #",TRUE);
			AddProcessSecurity("data-entry","FINMGT","RECEIPT DISTRIBUTION",TRUE);
			AddProcessSecurity("data-entry","FINMGT","RECEIPT REPORT",TRUE);
			AddProcessSecurity("data-entry","FINMGT","RECREPORT",TRUE);
			AddProcessSecurity("data-entry","FINMGT","RECREPORTDATE",TRUE);
			AddProcessSecurity("data-entry","FINMGT","RECURRING RECEIPT AUDIT TRAIL",TRUE);
			AddProcessSecurity("data-entry","FINMGT","REVERSERECEIPT",TRUE);
			AddProcessSecurity("data-entry","FINMGT","REVERSERECEIPT1",TRUE);
			AddProcessSecurity("data-entry","FINMGT","RRECAUDIT",TRUE);
			AddProcessSecurity("data-entry","FINMGT","RRECAUDIT1",TRUE);
			AddProcessSecurity("data-entry","FINMGT","UNDOJEDIST1-G",FALSE);
			AddProcessSecurity("data-entry","FINMGT","UNDORECAUDIT",TRUE);
			AddProcessSecurity("data-entry","FINMGT","UNDORECAUDIT1",TRUE);
			AddProcessSecurity("data-entry","FINMGT","UNDORECDEPOSIT",FALSE);
			AddProcessSecurity("data-entry","FINMGT","UNDORECDEPOSIT1",FALSE);
			AddProcessSecurity("data-entry","FINMGT","UNDORECDIST",FALSE);
			AddProcessSecurity("data-entry","FINMGT","UNDORECDIST-G",FALSE);
			AddProcessSecurity("data-entry","FINMGT","UNDORECDIST1",FALSE);
			AddProcessSecurity("data-entry","FINMGT","UNDORECDIST1-G",FALSE);


			AddProcessSecurity("data-entry-ar","FINMGT","ADD RECEIPTS",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","ADD MULTIPLE RECEIPTS",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","APPLY RECURRING RECEIPTS",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","FINARRECE",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","FINARRECP",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","FINMGT CHGRCJOURNALID",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","FINMGT DOREPORT",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","FINMGT RECAUDIT",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","FINMGT RECREPORTDATE",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","FINMGT REVERSERECEIPT",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","FINMGT RPT CHGDEPOSITDATE",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","FINMGT RPT FINBNKSUM",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","FINMGT UNDORECAUDIT",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","INTERFUND BANK TRANSFERS",FALSE);
			AddProcessSecurity("data-entry-ar","FINMGT","INTRAFUND ELECTRONIC TRANSFERS",FALSE);
			AddProcessSecurity("data-entry-ar","FINMGT","MTN FINREC",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","MTN FINREC.BROWSE",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","MTN FINREC.SEARCHBROWSE",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","MTN FINRJEN",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","MTN FINRJEN.BROWSE",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","MTN FINRJRNL",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","MTN FINRMSR",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","MTN FINRMSR.BROWSE",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","MTN FINRR",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","MTN FINRREC",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","RECAUDIT",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","RECAUDIT1",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","RECAUDITID",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","RECAUDITREF",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","RECDEPOSIT",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","RECDEPOSIT1",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","RECDISTEZ",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","RECDIST1EZ",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","RECDIST",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","RECDIST-G",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","RECDIST1",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","RECDIST1-G",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","RECEIPT AUDIT REPORT",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","RECEIPT AUDIT REPORT BY RECEIPT #",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","RECEIPT DISTRIBUTION",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","RECEIPT REPORT",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","RECREPORT",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","RECREPORTDATE",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","RECURRING RECEIPT AUDIT TRAIL",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","REVERSERECEIPT",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","REVERSERECEIPT1",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","RRECAUDIT",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","RRECAUDIT1",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","UNDOJEDIST1-G",FALSE);
			AddProcessSecurity("data-entry-ar","FINMGT","UNDORECAUDIT",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","UNDORECAUDIT1",TRUE);
			AddProcessSecurity("data-entry-ar","FINMGT","UNDORECDEPOSIT",FALSE);
			AddProcessSecurity("data-entry-ar","FINMGT","UNDORECDEPOSIT1",FALSE);
			AddProcessSecurity("data-entry-ar","FINMGT","UNDORECDIST",FALSE);
			AddProcessSecurity("data-entry-ar","FINMGT","UNDORECDIST-G",FALSE);
			AddProcessSecurity("data-entry-ar","FINMGT","UNDORECDIST1",FALSE);
			AddProcessSecurity("data-entry-ar","FINMGT","UNDORECDIST1-G",FALSE);
		}
		if(XPERT_SETUP->VENPMT==TRUE)
		{
			AddModuleSecurity("admin","VENPMT",FALSE,FALSE,FALSE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("acctmgr","VENPMT",FALSE,FALSE,FALSE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry","VENPMT",FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-ar","VENPMT",FALSE,FALSE,FALSE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-po","VENPMT",FALSE,FALSE,FALSE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-hrpo","VENPMT",FALSE,FALSE,FALSE,TRUE,TRUE,TRUE,FALSE);


			AddFileSecurity("data-entry","VENPMT","VENADR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","VENPMT","VENADRID",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","VENPMT","VENFYR",FALSE,TRUE,FALSE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","VENPMT","VENINV",FALSE,TRUE,FALSE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","VENPMT","VENJRNL",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","VENPMT","VENMSR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","VENPMT","VENPAYR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","VENPMT","VENRMSR",FALSE,TRUE,FALSE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","VENPMT","VENRVCH",FALSE,TRUE,FALSE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry","VENPMT","VENVCH",FALSE,TRUE,FALSE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry","VENPMT","VENYR",FALSE,TRUE,TRUE,TRUE,FALSE,"");


			AddFileSecurity("data-entry-ar","VENPMT","VENADR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","VENPMT","VENADRID",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","VENPMT","VENFYR",FALSE,TRUE,FALSE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","VENPMT","VENINV",FALSE,TRUE,FALSE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","VENPMT","VENJRNL",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","VENPMT","VENMSR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","VENPMT","VENPAYR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","VENPMT","VENRMSR",FALSE,TRUE,FALSE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","VENPMT","VENRVCH",FALSE,TRUE,FALSE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-ar","VENPMT","VENVCH",FALSE,TRUE,FALSE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-ar","VENPMT","VENYR",FALSE,TRUE,TRUE,TRUE,FALSE,"");

			AddFileSecurity("data-entry-po","VENPMT","VENADR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","VENPMT","VENADRID",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","VENPMT","VENFYR",FALSE,TRUE,FALSE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","VENPMT","VENINV",FALSE,TRUE,FALSE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","VENPMT","VENJRNL",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","VENPMT","VENMSR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","VENPMT","VENPAYR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","VENPMT","VENRMSR",FALSE,TRUE,FALSE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","VENPMT","VENRVCH",FALSE,TRUE,FALSE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-po","VENPMT","VENVCH",FALSE,TRUE,FALSE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-po","VENPMT","VENYR",FALSE,TRUE,TRUE,TRUE,FALSE,"");


			AddFileSecurity("data-entry-hrpo","VENPMT","VENADR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","VENPMT","VENADRID",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","VENPMT","VENFYR",FALSE,TRUE,FALSE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","VENPMT","VENINV",FALSE,TRUE,FALSE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","VENPMT","VENJRNL",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","VENPMT","VENMSR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","VENPMT","VENPAYR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","VENPMT","VENRMSR",FALSE,TRUE,FALSE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","VENPMT","VENRVCH",FALSE,TRUE,FALSE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-hrpo","VENPMT","VENVCH",FALSE,TRUE,FALSE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-hrpo","VENPMT","VENYR",FALSE,TRUE,TRUE,TRUE,FALSE,"");

			AddFileSecurity("acctmgr","VENPMT","VENADR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","VENPMT","VENADRID",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","VENPMT","VENFYR",FALSE,TRUE,FALSE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","VENPMT","VENINV",FALSE,TRUE,FALSE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","VENPMT","VENJRNL",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","VENPMT","VENMSR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","VENPMT","VENPAYR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","VENPMT","VENRMSR",FALSE,TRUE,FALSE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","VENPMT","VENRVCH",FALSE,TRUE,FALSE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("acctmgr","VENPMT","VENVCH",FALSE,TRUE,FALSE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("acctmgr","VENPMT","VENYR",FALSE,TRUE,TRUE,TRUE,FALSE,"");

			AddFileSecurity("admin","VENPMT","VENADR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","VENPMT","VENADRID",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","VENPMT","VENFYR",FALSE,TRUE,FALSE,TRUE,FALSE,"");
			AddFileSecurity("admin","VENPMT","VENINV",FALSE,TRUE,FALSE,TRUE,FALSE,"");
			AddFileSecurity("admin","VENPMT","VENJRNL",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","VENPMT","VENMSR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","VENPMT","VENPAYR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","VENPMT","VENRMSR",FALSE,TRUE,FALSE,TRUE,FALSE,"");
			AddFileSecurity("admin","VENPMT","VENRVCH",FALSE,TRUE,FALSE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("admin","VENPMT","VENVCH",FALSE,TRUE,FALSE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("admin","VENPMT","VENYR",FALSE,TRUE,TRUE,TRUE,FALSE,"");



			AddProcessSecurity("admin","VENPMT","VCHAPPROVE",TRUE);
			AddProcessSecurity("admin","VENPMT","COPYADDRESS",TRUE);
			AddProcessSecurity("admin","VENPMT","MTN VENADR.BROWSE",TRUE);
			AddProcessSecurity("admin","VENPMT","MTN VENFYR.BROWSE",TRUE);
			AddProcessSecurity("admin","VENPMT","MTN VENINV.BROWSE",TRUE);
			AddProcessSecurity("admin","VENPMT","MTN VENMSR",TRUE);
			AddProcessSecurity("admin","VENPMT","MTN VENVCH.BROWSE",TRUE);
			AddProcessSecurity("admin","VENPMT","MTN VENVCH.SEARCHBROWSE",TRUE);
			AddProcessSecurity("admin","VENPMT","MTN VENYR",TRUE);
			AddProcessSecurity("admin","VENPMT","MTN VENYR.BROWSE",TRUE);
			AddProcessSecurity("admin","VENPMT","QUERY SUBMENU",TRUE);
			AddProcessSecurity("admin","VENPMT","VENDOR PAYMENTS MENU",TRUE);
			AddProcessSecurity("admin","VENPMT","ADD VENDOR VOUCHERS",FALSE);
			AddProcessSecurity("admin","VENPMT","ADD MULTIPLE VENDOR VOUCHERS",FALSE);
			AddProcessSecurity("admin","VENPMT","ADD MULTIPLE VENDOR VOUCHERS",FALSE);
			AddProcessSecurity("admin","VENPMT","ADD-CHANGE ARCHIVE",FALSE);
			AddProcessSecurity("admin","VENPMT","APPLY RECURRING VOUCHERS",FALSE);
			AddProcessSecurity("admin","VENPMT","APPLY RECURRING VOUCHERS INDIVIDUALLY",FALSE);
			AddProcessSecurity("admin","VENPMT","DIRECT VOUCHERS AUDIT REPORT",FALSE);
			AddProcessSecurity("admin","VENPMT","PRINT DETAILED VOUCHER REPORT",TRUE);
			AddProcessSecurity("admin","VENPMT","PRINT MONTHLY CHECK REGISTER",TRUE);
			AddProcessSecurity("admin","VENPMT","PRINT MONTHLY ELECTRONIC TRANSFER REGIST",TRUE);
			AddProcessSecurity("admin","VENPMT","PRINT MONTHLY RECAP",TRUE);
			AddProcessSecurity("admin","VENPMT","PRINT RECURRING VOUCHERS REPORT",TRUE);
			AddProcessSecurity("admin","VENPMT","PRINT VENDOR ADDRESS REPORT",TRUE);
			AddProcessSecurity("admin","VENPMT","PRINT VENDOR MASTER REPORT",TRUE);
			AddProcessSecurity("admin","VENPMT","PRINT VOUCHER REPORT",TRUE);
			AddProcessSecurity("admin","VENPMT","PRINT VOUCHER REPORT BY VENDOR",TRUE);
			AddProcessSecurity("admin","VENPMT","PRINT VOUCHER TRANSACTION REPORT",TRUE);
			AddProcessSecurity("admin","VENPMT","PVCAUDITORDER",FALSE);
			AddProcessSecurity("admin","VENPMT","PVCRECAP",FALSE);
			AddProcessSecurity("admin","VENPMT","RECURRING VOUCHERS AUDIT REPORT",FALSE);
			AddProcessSecurity("admin","VENPMT","RVCHAUDIT",FALSE);
			AddProcessSecurity("admin","VENPMT","RVCHAUDIT1",FALSE);
			AddProcessSecurity("admin","VENPMT","SELECTIVE DIRECT VOUCHERS AUDIT REPORT",TRUE);
			AddProcessSecurity("admin","VENPMT","UNDO DIRECT VOUCHER AUDIT TRAIL",FALSE);
			AddProcessSecurity("admin","VENPMT","USER REPORT SUBMENU",TRUE);
			AddProcessSecurity("admin","VENPMT","VCAUDIT",FALSE);
			AddProcessSecurity("admin","VENPMT","VCAUDIT1",FALSE);
			AddProcessSecurity("admin","VENPMT","VCAUDITORDER",FALSE);
			AddProcessSecurity("admin","VENPMT","VCAUDITORDER1",FALSE);
			AddProcessSecurity("admin","VENPMT","VCAUDITSEL",FALSE);
			AddProcessSecurity("admin","VENPMT","VCAUDITSEL1",FALSE);
			AddProcessSecurity("admin","VENPMT","VCUNAUDIT",FALSE);
			AddProcessSecurity("admin","VENPMT","VCUNAUDIT1",FALSE);
			AddProcessSecurity("admin","VENPMT","VENADDR",TRUE);
			AddProcessSecurity("admin","VENPMT","VENADDRASCII",TRUE);
			AddProcessSecurity("admin","VENPMT","VENADLAB",FALSE);
			AddProcessSecurity("admin","VENPMT","VENADR",TRUE);
			AddProcessSecurity("admin","VENPMT","VENADR-CCH",TRUE);
			AddProcessSecurity("admin","VENPMT","VENARVNP",TRUE);
			AddProcessSecurity("admin","VENPMT","VENARVPE",TRUE);
			AddProcessSecurity("admin","VENPMT","VENDOR ADDRESS LABELS",TRUE);
			AddProcessSecurity("admin","VENPMT","VENDOR PAYMENTS 1099 REPORT SUBMENU",TRUE);
			AddProcessSecurity("admin","VENPMT","VENDOR PAYMENTS MENU",TRUE);
			AddProcessSecurity("admin","VENPMT","VENDOR PAYMENTS PROCESS SUBMENU",TRUE);
			AddProcessSecurity("admin","VENPMT","VENDOR PAYMENTS RECURRING VOUCHERS SUBME",TRUE);
			AddProcessSecurity("admin","VENPMT","VENDOR PAYMENTS REPORTS SUBMENU",TRUE);
			AddProcessSecurity("admin","VENPMT","VENDOR PAYMENTS UNDO SUBMENU",TRUE);
			AddProcessSecurity("admin","VENPMT","VENMCRG",TRUE);
			AddProcessSecurity("admin","VENPMT","VENMEFTREG",TRUE);
			AddProcessSecurity("admin","VENPMT","VENMSR",TRUE);
			AddProcessSecurity("admin","VENPMT","VENPMT DOREPORT",TRUE);
			AddProcessSecurity("admin","VENPMT","VENPMT EZRPT",TRUE);
			AddProcessSecurity("admin","VENPMT","VENPMT PROCESS IMPORT EXPORT SUBMENU",TRUE);
			AddProcessSecurity("admin","VENPMT","VENPMT PVCAUDITORDER",TRUE);
			AddProcessSecurity("admin","VENPMT","VENPMT PVCRECAP",FALSE);
			AddProcessSecurity("admin","VENPMT","VENPMT VENADDRASCII",TRUE);
			AddProcessSecurity("admin","VENPMT","VENPMT VOUCHRPT3",TRUE);
			AddProcessSecurity("admin","VENPMT","VENYR",TRUE);
			AddProcessSecurity("admin","VENPMT","VENYRSUM",TRUE);
			AddProcessSecurity("admin","VENPMT","VENYRSUM REPORT",TRUE);
			AddProcessSecurity("admin","VENPMT","VENYRSUM2",TRUE);
			AddProcessSecurity("admin","VENPMT","VOUCHER RECONCILIATION REPORT",TRUE);
			AddProcessSecurity("admin","VENPMT","VOUCHRPT",TRUE);
			AddProcessSecurity("admin","VENPMT","VOUCHRPT-CCH",TRUE);
			AddProcessSecurity("admin","VENPMT","VOUCHRPT-ORD",TRUE);
			AddProcessSecurity("admin","VENPMT","VOUCHRPT2",TRUE);
			AddProcessSecurity("admin","VENPMT","VOUCHRPT3",TRUE);
			AddProcessSecurity("admin","VENPMT","VOUCHTRANS",TRUE);
			AddProcessSecurity("admin","VENPMT","VOUCHTRN",TRUE);



			AddProcessSecurity("acctmgr","VENPMT","COPYADDRESS",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","MTN VENADR.BROWSE",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","MTN VENFYR.BROWSE",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","MTN VENINV.BROWSE",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","MTN VENMSR",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","MTN VENVCH.BROWSE",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","MTN VENVCH.SEARCHBROWSE",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","MTN VENYR",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","MTN VENYR.BROWSE",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","QUERY SUBMENU",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENDOR PAYMENTS MENU",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","ADD VENDOR VOUCHERS",FALSE);
			AddProcessSecurity("acctmgr","VENPMT","ADD MULTIPLE VENDOR VOUCHERS",FALSE);
			AddProcessSecurity("acctmgr","VENPMT","ADD MULTIPLE VENDOR VOUCHERS",FALSE);
			AddProcessSecurity("acctmgr","VENPMT","ADD-CHANGE ARCHIVE",FALSE);
			AddProcessSecurity("acctmgr","VENPMT","APPLY RECURRING VOUCHERS",FALSE);
			AddProcessSecurity("acctmgr","VENPMT","APPLY RECURRING VOUCHERS INDIVIDUALLY",FALSE);
			AddProcessSecurity("acctmgr","VENPMT","DIRECT VOUCHERS AUDIT REPORT",FALSE);
			AddProcessSecurity("acctmgr","VENPMT","PRINT DETAILED VOUCHER REPORT",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","PRINT MONTHLY CHECK REGISTER",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","PRINT MONTHLY ELECTRONIC TRANSFER REGIST",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","PRINT MONTHLY RECAP",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","PRINT RECURRING VOUCHERS REPORT",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","PRINT VENDOR ADDRESS REPORT",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","PRINT VENDOR MASTER REPORT",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","PRINT VOUCHER REPORT",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","PRINT VOUCHER REPORT BY VENDOR",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","PRINT VOUCHER TRANSACTION REPORT",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","PVCAUDITORDER",FALSE);
			AddProcessSecurity("acctmgr","VENPMT","PVCRECAP",FALSE);
			AddProcessSecurity("acctmgr","VENPMT","RECURRING VOUCHERS AUDIT REPORT",FALSE);
			AddProcessSecurity("acctmgr","VENPMT","RVCHAUDIT",FALSE);
			AddProcessSecurity("acctmgr","VENPMT","RVCHAUDIT1",FALSE);
			AddProcessSecurity("acctmgr","VENPMT","SELECTIVE DIRECT VOUCHERS AUDIT REPORT",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","UNDO DIRECT VOUCHER AUDIT TRAIL",FALSE);
			AddProcessSecurity("acctmgr","VENPMT","USER REPORT SUBMENU",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VCAUDIT",FALSE);
			AddProcessSecurity("acctmgr","VENPMT","VCAUDIT1",FALSE);
			AddProcessSecurity("acctmgr","VENPMT","VCAUDITORDER",FALSE);
			AddProcessSecurity("acctmgr","VENPMT","VCAUDITORDER1",FALSE);
			AddProcessSecurity("acctmgr","VENPMT","VCAUDITSEL",FALSE);
			AddProcessSecurity("acctmgr","VENPMT","VCAUDITSEL1",FALSE);
			AddProcessSecurity("acctmgr","VENPMT","VCUNAUDIT",FALSE);
			AddProcessSecurity("acctmgr","VENPMT","VCUNAUDIT1",FALSE);
			AddProcessSecurity("acctmgr","VENPMT","VENADDR",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENADDRASCII",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENADLAB",FALSE);
			AddProcessSecurity("acctmgr","VENPMT","VENADR",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENADR-CCH",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENARVNP",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENARVPE",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENDOR ADDRESS LABELS",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENDOR PAYMENTS 1099 REPORT SUBMENU",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENDOR PAYMENTS MENU",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENDOR PAYMENTS PROCESS SUBMENU",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENDOR PAYMENTS RECURRING VOUCHERS SUBME",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENDOR PAYMENTS REPORTS SUBMENU",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENDOR PAYMENTS UNDO SUBMENU",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENMCRG",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENMEFTREG",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENMSR",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENPMT DOREPORT",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENPMT EZRPT",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENPMT PROCESS IMPORT EXPORT SUBMENU",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENPMT PVCAUDITORDER",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENPMT PVCRECAP",FALSE);
			AddProcessSecurity("acctmgr","VENPMT","VENPMT VENADDRASCII",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENPMT VOUCHRPT3",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENYR",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENYRSUM",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENYRSUM REPORT",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VENYRSUM2",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VOUCHER RECONCILIATION REPORT",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VOUCHRPT",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VOUCHRPT-CCH",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VOUCHRPT-ORD",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VOUCHRPT2",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VOUCHRPT3",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VOUCHTRANS",TRUE);
			AddProcessSecurity("acctmgr","VENPMT","VOUCHTRN",TRUE);

			AddProcessSecurity("data-entry","VENPMT","ADD VENDOR VOUCHERS",TRUE);
			AddProcessSecurity("data-entry","VENPMT","ADD MULTIPLE VENDOR VOUCHERS",TRUE);
			AddProcessSecurity("data-entry","VENPMT","ADD-CHANGE ARCHIVE",TRUE);
			AddProcessSecurity("data-entry","VENPMT","APPLY RECURRING VOUCHERS",TRUE);
			AddProcessSecurity("data-entry","VENPMT","APPLY RECURRING VOUCHERS INDIVIDUALLY",TRUE);
			AddProcessSecurity("data-entry","VENPMT","DIRECT VOUCHERS AUDIT REPORT",TRUE);
			AddProcessSecurity("data-entry","VENPMT","ADD MULTIPLE VENDOR VOUCHERS",TRUE);
			AddProcessSecurity("data-entry","VENPMT","ADD-CHANGE ARCHIVE",TRUE);
			AddProcessSecurity("data-entry","VENPMT","APPLY RECURRING VOUCHERS",TRUE);
			AddProcessSecurity("data-entry","VENPMT","APPLY RECURRING VOUCHERS INDIVIDUALLY",TRUE);
			AddProcessSecurity("data-entry","VENPMT","DIRECT VOUCHERS AUDIT REPORT",TRUE);
			AddProcessSecurity("data-entry","VENPMT","MTN VENADR",TRUE);
			AddProcessSecurity("data-entry","VENPMT","MTN VENADR.BROWSE",TRUE);
			AddProcessSecurity("data-entry","VENPMT","MTN VENADR.SEARCHBROWSE",TRUE);
			AddProcessSecurity("data-entry","VENPMT","MTN VENFYR",TRUE);
			AddProcessSecurity("data-entry","VENPMT","MTN VENFYR.BROWSE",TRUE);
			AddProcessSecurity("data-entry","VENPMT","MTN VENINF",TRUE);
			AddProcessSecurity("data-entry","VENPMT","MTN VENINF.SEARCH",TRUE);
			AddProcessSecurity("data-entry","VENPMT","MTN VENINV",TRUE);
			AddProcessSecurity("data-entry","VENPMT","MTN VENINV.BROWSE",TRUE);
			AddProcessSecurity("data-entry","VENPMT","MTN VENINV.SEARCHBROWSE",TRUE);
			AddProcessSecurity("data-entry","VENPMT","MTN VENJRNL",TRUE);
			AddProcessSecurity("data-entry","VENPMT","MTN VENMSR",TRUE);
			AddProcessSecurity("data-entry","VENPMT","MTN VENMSR.BROWSE",TRUE);
			AddProcessSecurity("data-entry","VENPMT","MTN VENMSR.SEARCHBROWSE",TRUE);
			AddProcessSecurity("data-entry","VENPMT","MTN VENRMSR",TRUE);
			AddProcessSecurity("data-entry","VENPMT","MTN VENRVCH",TRUE);
			AddProcessSecurity("data-entry","VENPMT","MTN VENRVCH.BROWSE",TRUE);
			AddProcessSecurity("data-entry","VENPMT","MTN VENVCH",TRUE);
			AddProcessSecurity("data-entry","VENPMT","MTN VENVCH.BROWSE",TRUE);
			AddProcessSecurity("data-entry","VENPMT","MTN VENVCH.SEARCHBROWSE",TRUE);
			AddProcessSecurity("data-entry","VENPMT","MTN VENYR",TRUE);
			AddProcessSecurity("data-entry","VENPMT","MTN VENYR.BROWSE",TRUE);
			AddProcessSecurity("data-entry","VENPMT","PRINT DETAILED VOUCHER REPORT",TRUE);
			AddProcessSecurity("data-entry","VENPMT","PRINT MONTHLY CHECK REGISTER",TRUE);
			AddProcessSecurity("data-entry","VENPMT","PRINT MONTHLY ELECTRONIC TRANSFER REGIST",TRUE);
			AddProcessSecurity("data-entry","VENPMT","PRINT MONTHLY RECAP",TRUE);
			AddProcessSecurity("data-entry","VENPMT","PRINT RECURRING VOUCHERS REPORT",TRUE);
			AddProcessSecurity("data-entry","VENPMT","PRINT VENDOR ADDRESS REPORT",TRUE);
			AddProcessSecurity("data-entry","VENPMT","PRINT VENDOR MASTER REPORT",TRUE);
			AddProcessSecurity("data-entry","VENPMT","PRINT VOUCHER REPORT",TRUE);
			AddProcessSecurity("data-entry","VENPMT","PRINT VOUCHER REPORT BY VENDOR",TRUE);
			AddProcessSecurity("data-entry","VENPMT","PRINT VOUCHER TRANSACTION REPORT",TRUE);
			AddProcessSecurity("data-entry","VENPMT","PVCAUDITORDER",TRUE);
			AddProcessSecurity("data-entry","VENPMT","PVCRECAP",TRUE);
			AddProcessSecurity("data-entry","VENPMT","RECURRING VOUCHERS AUDIT REPORT",TRUE);
			AddProcessSecurity("data-entry","VENPMT","RVCHAUDIT",TRUE);
			AddProcessSecurity("data-entry","VENPMT","RVCHAUDIT1",TRUE);
			AddProcessSecurity("data-entry","VENPMT","SELECTIVE DIRECT VOUCHERS AUDIT REPORT",TRUE);
			AddProcessSecurity("data-entry","VENPMT","UNDO DIRECT VOUCHER AUDIT TRAIL",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VCAUDIT",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VCAUDIT1",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VCAUDITORDER",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VCAUDITORDER1",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VCAUDITSEL",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VCAUDITSEL1",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VCUNAUDIT",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VCUNAUDIT1",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VENADDR",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VENADDRASCII",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VENADLAB",FALSE);
			AddProcessSecurity("data-entry","VENPMT","VENADR",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VENADR-CCH",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VENARVNP",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VENARVPE",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VENDOR ADDRESS LABELS",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VENMCRG",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VENMEFTREG",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VENMSR",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VENYR",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VENYRSUM",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VENYRSUM REPORT",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VENYRSUM2",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VOUCHER RECONCILIATION REPORT",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VOUCHRPT",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VOUCHRPT-CCH",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VOUCHRPT-ORD",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VOUCHRPT2",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VOUCHRPT3",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VOUCHTRANS",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VOUCHTRN",TRUE);
			AddProcessSecurity("data-entry","VENPMT","VOUCHTRNA",TRUE);




		}
		if(XPERT_SETUP->BNKREC==TRUE)
		{
			AddModuleSecurity("admin","BNKREC",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("acctmgr","BNKREC",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry","BNKREC",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-ar","BNKREC",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-po","BNKREC",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-hrpo","BNKREC",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
		}
		if(XPERT_SETUP->PURORD==TRUE)
		{

			AddModuleSecurity("admin","PURORD",FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("acctmgr","PURORD",FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry","PURORD",FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-po","PURORD",FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-ar","PURORD",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-hrpo","PURORD",FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);


			AddFileSecurity("admin","PURORD","PURACC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("admin","PURORD","PURCIS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PURORD","PURDOC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PURORD","PURIDS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PURORD","PURLIN",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PURORD","PURMST",FALSE,TRUE,TRUE,TRUE,FALSE,"(([USERS SITE IDENTIFICATION]=[PURORD][PURMST][SITE IDENTIFICATION]) OR ([USERS SITE IDENTIFICATION]=[PURORD][PURMST][SOURCE IDENTIFICATION]))");
			AddFileSecurity("admin","PURORD","PURORG",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PURORD","PURREC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PURORD","PURSIT",FALSE,TRUE,TRUE,TRUE,FALSE,"([USERS SITE IDENTIFICATION]=[PURORD][PURSIT][SITE IDENTIFICATION])");
			AddFileSecurity("admin","PURORD","PURTRN",FALSE,TRUE,TRUE,TRUE,FALSE,"");


			AddFileSecurity("acctmgr","PURORD","PURACC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("acctmgr","PURORD","PURCIS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PURORD","PURDOC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PURORD","PURIDS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PURORD","PURLIN",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PURORD","PURMST",FALSE,TRUE,TRUE,TRUE,FALSE,"(([USERS SITE IDENTIFICATION]=[PURORD][PURMST][SITE IDENTIFICATION]) OR ([USERS SITE IDENTIFICATION]=[PURORD][PURMST][SOURCE IDENTIFICATION]))");
			AddFileSecurity("acctmgr","PURORD","PURORG",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PURORD","PURREC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PURORD","PURSIT",FALSE,TRUE,TRUE,TRUE,FALSE,"([USERS SITE IDENTIFICATION]=[PURORD][PURSIT][SITE IDENTIFICATION])");
			AddFileSecurity("acctmgr","PURORD","PURTRN",FALSE,TRUE,TRUE,TRUE,FALSE,"");


			AddFileSecurity("data-entry","PURORD","PURACC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry","PURORD","PURCIS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PURORD","PURDOC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PURORD","PURIDS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PURORD","PURLIN",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PURORD","PURMST",FALSE,TRUE,TRUE,TRUE,FALSE,"(([USERS SITE IDENTIFICATION]=[PURORD][PURMST][SITE IDENTIFICATION]) OR ([USERS SITE IDENTIFICATION]=[PURORD][PURMST][SOURCE IDENTIFICATION]))");
			AddFileSecurity("data-entry","PURORD","PURORG",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PURORD","PURREC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PURORD","PURSIT",FALSE,TRUE,TRUE,TRUE,FALSE,"([USERS SITE IDENTIFICATION]=[PURORD][PURSIT][SITE IDENTIFICATION])");
			AddFileSecurity("data-entry","PURORD","PURTRN",FALSE,TRUE,TRUE,TRUE,FALSE,"");


			AddFileSecurity("data-entry-po","PURORD","PURACC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-po","PURORD","PURCIS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","PURORD","PURDOC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","PURORD","PURIDS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","PURORD","PURLIN",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","PURORD","PURMST",FALSE,TRUE,TRUE,TRUE,FALSE,"(([USERS SITE IDENTIFICATION]=[PURORD][PURMST][SITE IDENTIFICATION]) OR ([USERS SITE IDENTIFICATION]=[PURORD][PURMST][SOURCE IDENTIFICATION]))");
			AddFileSecurity("data-entry-po","PURORD","PURORG",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","PURORD","PURREC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","PURORD","PURSIT",FALSE,TRUE,TRUE,TRUE,FALSE,"([USERS SITE IDENTIFICATION]=[PURORD][PURSIT][SITE IDENTIFICATION])");
			AddFileSecurity("data-entry-po","PURORD","PURTRN",FALSE,TRUE,TRUE,TRUE,FALSE,"");


			AddFileSecurity("data-entry-hrpo","PURORD","PURACC",FALSE,TRUE,TRUE,TRUE,FALSE,"([ACCOUNT MANAGER]=[LOGIN IDENTIFICATION]) OR ([ACCOUNT MANAGER]=[USERS SUPERVISOR])");
			AddFileSecurity("data-entry-hrpo","PURORD","PURCIS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PURORD","PURDOC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PURORD","PURIDS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PURORD","PURLIN",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PURORD","PURMST",FALSE,TRUE,TRUE,TRUE,FALSE,"(([USERS SITE IDENTIFICATION]=[PURORD][PURMST][SITE IDENTIFICATION]) OR ([USERS SITE IDENTIFICATION]=[PURORD][PURMST][SOURCE IDENTIFICATION]))");
			AddFileSecurity("data-entry-hrpo","PURORD","PURORG",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PURORD","PURREC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PURORD","PURSIT",FALSE,TRUE,TRUE,TRUE,FALSE,"([USERS SITE IDENTIFICATION]=[PURORD][PURSIT][SITE IDENTIFICATION])");
			AddFileSecurity("data-entry-hrpo","PURORD","PURTRN",FALSE,TRUE,TRUE,TRUE,FALSE,"");



			AddProcessSecurity("data-entry","PURORD","ADD MULTIPLE PURTRN",TRUE);
			AddProcessSecurity("data-entry","PURORD","ADD PURLIN",TRUE);
			AddProcessSecurity("data-entry","PURORD","ADD PURMST",TRUE);
			AddProcessSecurity("data-entry","PURORD","ADD PURTRN",TRUE);
			AddProcessSecurity("data-entry","PURORD","CAPTURE ARCHIVE",TRUE);
			AddProcessSecurity("data-entry","PURORD","EZ ADD PO",TRUE);
			AddProcessSecurity("data-entry","PURORD","EZ PO",TRUE);
			AddProcessSecurity("data-entry","PURORD","MTN PURACC",TRUE);
			AddProcessSecurity("data-entry","PURORD","MTN PURACC.BROWSE",TRUE);
			AddProcessSecurity("data-entry","PURORD","MTN PURACC.SEARCHBROWSE",TRUE);
			AddProcessSecurity("data-entry","PURORD","MTN PURIDS",TRUE);
			AddProcessSecurity("data-entry","PURORD","MTN PURLIN",TRUE);
			AddProcessSecurity("data-entry","PURORD","MTN PURLIN.SEARCHBROWSE",TRUE);
			AddProcessSecurity("data-entry","PURORD","MTN PURMST",TRUE);
			AddProcessSecurity("data-entry","PURORD","MTN PURMST.BROWSE",TRUE);
			AddProcessSecurity("data-entry","PURORD","MTN PURMST.SEARCHBROWSE",TRUE);
			AddProcessSecurity("data-entry","PURORD","MTN PURREC",TRUE);
			AddProcessSecurity("data-entry","PURORD","MTN PURTRAN",TRUE);
			AddProcessSecurity("data-entry","PURORD","MTN PURTRAN.SEARCH",TRUE);
			AddProcessSecurity("data-entry","PURORD","OPENPOS",TRUE);
			AddProcessSecurity("data-entry","PURORD","POAUDIT",TRUE);
			AddProcessSecurity("data-entry","PURORD","POAUDIT1",TRUE);
			AddProcessSecurity("data-entry","PURORD","POAUDITENV",TRUE);
			AddProcessSecurity("data-entry","PURORD","PODIST",FALSE);
			AddProcessSecurity("data-entry","PURORD","POREQ",TRUE);
			AddProcessSecurity("data-entry","PURORD","POREQ-POSUB",TRUE);
			AddProcessSecurity("data-entry","PURORD","POUNAUDIT",TRUE);
			AddProcessSecurity("data-entry","PURORD","POUNAUDIT1",TRUE);
			AddProcessSecurity("data-entry","PURORD","PRE-APPROVAL",TRUE);
			AddProcessSecurity("data-entry","PURORD","PRINT PURCHASE ORDER SITES",TRUE);
			AddProcessSecurity("data-entry","PURORD","PRINT PURCHASE ORDER TRANSACTIONS",TRUE);
			AddProcessSecurity("data-entry","PURORD","PURCHASE ORDERS AUDIT TRAIL",TRUE);
			AddProcessSecurity("data-entry","PURORD","PURMSTAT",TRUE);
			AddProcessSecurity("data-entry","PURORD","PURORD CREATE POAUDIT",TRUE);
			AddProcessSecurity("data-entry","PURORD","OPENACC",TRUE);
			AddProcessSecurity("data-entry","PURORD","OPENACC2",TRUE);
			AddProcessSecurity("data-entry","PURORD","OPENPOS",TRUE);
			AddProcessSecurity("data-entry","PURORD","PURTRN#0",TRUE);
			AddProcessSecurity("data-entry","PURORD","PURTRN#1",TRUE);
			AddProcessSecurity("data-entry","PURORD","PURTRN#2",TRUE);
			AddProcessSecurity("data-entry","PURORD","PURTRN#3",TRUE);
			AddProcessSecurity("data-entry","PURORD","REQAUDIT",TRUE);
			AddProcessSecurity("data-entry","PURORD","REQAUDIT-SUB",TRUE);
			if(use_approve && approve_type>0)
			{

				AddProcessSecurity("data-entry","PURORD","ACCOUNTMANAGER",TRUE);
				AddProcessSecurity("data-entry","PURORD","ACCOUNTMANAGER1",TRUE);
				AddProcessSecurity("data-entry","PURORD","ALLACCTMGRS",TRUE);
			}


			AddProcessSecurity("data-entry-po","PURORD","ADD MULTIPLE PURTRN",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","ADD PURLIN",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","ADD PURMST",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","ADD PURTRN",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","CAPTURE ARCHIVE",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","EZ ADD PO",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","EZ PO",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","MTN PURACC",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","MTN PURACC.BROWSE",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","MTN PURACC.SEARCHBROWSE",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","MTN PURIDS",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","MTN PURLIN",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","MTN PURLIN.SEARCHBROWSE",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","MTN PURMST",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","MTN PURMST.BROWSE",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","MTN PURMST.SEARCHBROWSE",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","MTN PURREC",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","MTN PURTRAN",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","MTN PURTRAN.SEARCH",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","OPENPOS",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","POAUDIT",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","POAUDIT1",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","POAUDITENV",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","PODIST",FALSE);
			AddProcessSecurity("data-entry-po","PURORD","POREQ",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","POREQ-POSUB",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","POUNAUDIT",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","POUNAUDIT1",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","PRE-APPROVAL",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","PRINT PURCHASE ORDER SITES",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","PRINT PURCHASE ORDER TRANSACTIONS",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","PURCHASE ORDERS AUDIT TRAIL",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","PURMSTAT",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","PURORD CREATE POAUDIT",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","OPENACC",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","OPENACC2",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","OPENPOS",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","PURTRN#0",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","PURTRN#1",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","PURTRN#2",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","PURTRN#3",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","REQAUDIT",TRUE);
			AddProcessSecurity("data-entry-po","PURORD","REQAUDIT-SUB",TRUE);
			if(use_approve && approve_type>0)
			{

				AddProcessSecurity("data-entry-po","PURORD","ACCOUNTMANAGER",TRUE);
				AddProcessSecurity("data-entry-po","PURORD","ACCOUNTMANAGER1",TRUE);
				AddProcessSecurity("data-entry-po","PURORD","ALLACCTMGRS",TRUE);
			}

			AddProcessSecurity("data-entry-hrpo","PURORD","ADD MULTIPLE PURTRN",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","ADD PURLIN",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","ADD PURMST",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","ADD PURTRN",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","CAPTURE ARCHIVE",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","EZ ADD PO",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","EZ PO",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","MTN PURACC",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","MTN PURACC.BROWSE",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","MTN PURACC.SEARCHBROWSE",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","MTN PURIDS",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","MTN PURLIN",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","MTN PURLIN.SEARCHBROWSE",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","MTN PURMST",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","MTN PURMST.BROWSE",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","MTN PURMST.SEARCHBROWSE",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","MTN PURREC",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","MTN PURTRAN",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","MTN PURTRAN.SEARCH",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","OPENPOS",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","POAUDIT",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","POAUDIT1",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","POAUDITENV",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","PODIST",FALSE);
			AddProcessSecurity("data-entry-hrpo","PURORD","POREQ",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","POREQ-POSUB",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","POUNAUDIT",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","POUNAUDIT1",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","PRE-APPROVAL",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","PRINT PURCHASE ORDER SITES",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","PRINT PURCHASE ORDER TRANSACTIONS",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","PURCHASE ORDERS AUDIT TRAIL",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","PURMSTAT",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","PURORD CREATE POAUDIT",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","OPENACC",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","OPENACC2",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","OPENPOS",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","PURTRN#0",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","PURTRN#1",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","PURTRN#2",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","PURTRN#3",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","REQAUDIT",TRUE);
			AddProcessSecurity("data-entry-hrpo","PURORD","REQAUDIT-SUB",TRUE);
			if(use_approve && approve_type>0)
			{

				AddProcessSecurity("data-entry-hrpo","PURORD","ACCOUNTMANAGER",TRUE);
				AddProcessSecurity("data-entry-hrpo","PURORD","ACCOUNTMANAGER1",TRUE);
				AddProcessSecurity("data-entry-hrpo","PURORD","ALLACCTMGRS",TRUE);
			}


			AddProcessSecurity("acctmgr","PURORD","ADD MULTIPLE PURTRN",TRUE);
			AddProcessSecurity("acctmgr","PURORD","ADD PURLIN",TRUE);
			AddProcessSecurity("acctmgr","PURORD","ADD PURMST",TRUE);
			AddProcessSecurity("acctmgr","PURORD","ADD PURTRN",TRUE);
			AddProcessSecurity("acctmgr","PURORD","CAPTURE ARCHIVE",TRUE);
			AddProcessSecurity("acctmgr","PURORD","EZ ADD PO",TRUE);
			AddProcessSecurity("acctmgr","PURORD","EZ PO",TRUE);
			AddProcessSecurity("acctmgr","PURORD","MTN PURACC",TRUE);
			AddProcessSecurity("acctmgr","PURORD","MTN PURACC.BROWSE",TRUE);
			AddProcessSecurity("acctmgr","PURORD","MTN PURACC.SEARCHBROWSE",TRUE);
			AddProcessSecurity("acctmgr","PURORD","MTN PURIDS",TRUE);
			AddProcessSecurity("acctmgr","PURORD","MTN PURLIN",TRUE);
			AddProcessSecurity("acctmgr","PURORD","MTN PURLIN.SEARCHBROWSE",TRUE);
			AddProcessSecurity("acctmgr","PURORD","MTN PURMST",TRUE);
			AddProcessSecurity("acctmgr","PURORD","MTN PURMST.BROWSE",TRUE);
			AddProcessSecurity("acctmgr","PURORD","MTN PURMST.SEARCHBROWSE",TRUE);
			AddProcessSecurity("acctmgr","PURORD","MTN PURREC",TRUE);
			AddProcessSecurity("acctmgr","PURORD","MTN PURTRAN",TRUE);
			AddProcessSecurity("acctmgr","PURORD","MTN PURTRAN.SEARCH",TRUE);
			AddProcessSecurity("acctmgr","PURORD","OPENPOS",TRUE);
			AddProcessSecurity("acctmgr","PURORD","POAUDIT",TRUE);
			AddProcessSecurity("acctmgr","PURORD","POAUDIT1",TRUE);
			AddProcessSecurity("acctmgr","PURORD","POAUDITENV",TRUE);
			AddProcessSecurity("acctmgr","PURORD","PODIST",FALSE);
			AddProcessSecurity("acctmgr","PURORD","POREQ",TRUE);
			AddProcessSecurity("acctmgr","PURORD","POREQ-POSUB",TRUE);
			AddProcessSecurity("acctmgr","PURORD","POUNAUDIT",TRUE);
			AddProcessSecurity("acctmgr","PURORD","POUNAUDIT1",TRUE);
			AddProcessSecurity("acctmgr","PURORD","PRE-APPROVAL",TRUE);
			AddProcessSecurity("acctmgr","PURORD","PRINT PURCHASE ORDER SITES",TRUE);
			AddProcessSecurity("acctmgr","PURORD","PRINT PURCHASE ORDER TRANSACTIONS",TRUE);
			AddProcessSecurity("acctmgr","PURORD","PURCHASE ORDERS AUDIT TRAIL",TRUE);
			AddProcessSecurity("acctmgr","PURORD","PURMSTAT",TRUE);
			AddProcessSecurity("acctmgr","PURORD","PURORD CREATE POAUDIT",TRUE);
			AddProcessSecurity("acctmgr","PURORD","OPENACC",TRUE);
			AddProcessSecurity("acctmgr","PURORD","OPENACC2",TRUE);
			AddProcessSecurity("acctmgr","PURORD","OPENPOS",TRUE);
			AddProcessSecurity("acctmgr","PURORD","PURTRN#0",TRUE);
			AddProcessSecurity("acctmgr","PURORD","PURTRN#1",TRUE);
			AddProcessSecurity("acctmgr","PURORD","PURTRN#2",TRUE);
			AddProcessSecurity("acctmgr","PURORD","PURTRN#3",TRUE);
			AddProcessSecurity("acctmgr","PURORD","REQAUDIT",TRUE);
			AddProcessSecurity("acctmgr","PURORD","REQAUDIT-SUB",TRUE);
			if(use_approve && approve_type==0)
			{
				AddProcessSecurity("acctmgr","PURORD","APPROVE",TRUE);
				AddProcessSecurity("acctmgr","PURORD","APPROVE PURCHASE ORDERS",TRUE);
				AddProcessSecurity("acctmgr","PURORD","PRINT PURCHASE ORDERS",TRUE);
				AddProcessSecurity("acctmgr","PURORD","PRTAPO",TRUE);
				AddProcessSecurity("acctmgr","PURORD","PRTAPO-APP",TRUE);
				AddProcessSecurity("acctmgr","PURORD","ARCHIVEPO",TRUE);
				AddProcessSecurity("acctmgr","PURORD","ARCHIVEPO1",TRUE);
				AddProcessSecurity("acctmgr","PURORD","ARCHIVEPO2",TRUE);
				AddProcessSecurity("acctmgr","PURORD","PURORD EMAILPO",TRUE);
				AddProcessSecurity("acctmgr","PURORD","EMAILPO",TRUE);
				AddProcessSecurity("acctmgr","PURORD","EMAILPO2",TRUE);
			} else if(use_approve && approve_type>0)
			{
				AddProcessSecurity("acctmgr","PURORD","ACCOUNTMANAGER",TRUE);
				AddProcessSecurity("acctmgr","PURORD","ACCOUNTMANAGER1",TRUE);
				AddProcessSecurity("acctmgr","PURORD","ALLACCTMGRS",TRUE);
				if(use_site)
				{
					AddProcessSecurity("acctmgr","PURORD","SITEAPPROVE",TRUE);
					AddProcessSecurity("acctmgr","PURORD","SITEAPPROVE1",TRUE);
				}
			}

			AddProcessSecurity("admin","PURORD","ADD MULTIPLE PURTRN",TRUE);
			AddProcessSecurity("admin","PURORD","ADD PURLIN",TRUE);
			AddProcessSecurity("admin","PURORD","ADD PURMST",TRUE);
			AddProcessSecurity("admin","PURORD","ADD PURTRN",TRUE);
			AddProcessSecurity("admin","PURORD","CAPTURE ARCHIVE",TRUE);
			AddProcessSecurity("admin","PURORD","EZ ADD PO",TRUE);
			AddProcessSecurity("admin","PURORD","EZ PO",TRUE);
			AddProcessSecurity("admin","PURORD","MTN PURACC",TRUE);
			AddProcessSecurity("admin","PURORD","MTN PURACC.BROWSE",TRUE);
			AddProcessSecurity("admin","PURORD","MTN PURACC.SEARCHBROWSE",TRUE);
			AddProcessSecurity("admin","PURORD","MTN PURIDS",TRUE);
			AddProcessSecurity("admin","PURORD","MTN PURLIN",TRUE);
			AddProcessSecurity("admin","PURORD","MTN PURLIN.SEARCHBROWSE",TRUE);
			AddProcessSecurity("admin","PURORD","MTN PURMST",TRUE);
			AddProcessSecurity("admin","PURORD","MTN PURMST.BROWSE",TRUE);
			AddProcessSecurity("admin","PURORD","MTN PURMST.SEARCHBROWSE",TRUE);
			AddProcessSecurity("admin","PURORD","MTN PURREC",TRUE);
			AddProcessSecurity("admin","PURORD","MTN PURTRAN",TRUE);
			AddProcessSecurity("admin","PURORD","MTN PURTRAN.SEARCH",TRUE);
			AddProcessSecurity("admin","PURORD","OPENPOS",TRUE);
			AddProcessSecurity("admin","PURORD","POAUDIT",TRUE);
			AddProcessSecurity("admin","PURORD","POAUDIT1",TRUE);
			AddProcessSecurity("admin","PURORD","POAUDITENV",TRUE);
			AddProcessSecurity("admin","PURORD","PODIST",FALSE);
			AddProcessSecurity("admin","PURORD","POREQ",TRUE);
			AddProcessSecurity("admin","PURORD","POREQ-POSUB",TRUE);
			AddProcessSecurity("admin","PURORD","POUNAUDIT",TRUE);
			AddProcessSecurity("admin","PURORD","POUNAUDIT1",TRUE);
			AddProcessSecurity("admin","PURORD","PRE-APPROVAL",TRUE);
			AddProcessSecurity("admin","PURORD","PRINT PURCHASE ORDER SITES",TRUE);
			AddProcessSecurity("admin","PURORD","PRINT PURCHASE ORDER TRANSACTIONS",TRUE);
			AddProcessSecurity("admin","PURORD","PURCHASE ORDERS AUDIT TRAIL",TRUE);
			AddProcessSecurity("admin","PURORD","PURMSTAT",TRUE);
			AddProcessSecurity("admin","PURORD","PURORD CREATE POAUDIT",TRUE);
			AddProcessSecurity("admin","PURORD","OPENACC",TRUE);
			AddProcessSecurity("admin","PURORD","OPENACC2",TRUE);
			AddProcessSecurity("admin","PURORD","OPENPOS",TRUE);
			AddProcessSecurity("admin","PURORD","PURTRN#0",TRUE);
			AddProcessSecurity("admin","PURORD","PURTRN#1",TRUE);
			AddProcessSecurity("admin","PURORD","PURTRN#2",TRUE);
			AddProcessSecurity("admin","PURORD","PURTRN#3",TRUE);
			AddProcessSecurity("admin","PURORD","REQAUDIT",TRUE);
			AddProcessSecurity("admin","PURORD","REQAUDIT-SUB",TRUE);
			if(use_approve && approve_type==0)
			{
				AddProcessSecurity("admin","PURORD","APPROVE",TRUE);
				AddProcessSecurity("admin","PURORD","APPROVE PURCHASE ORDERS",TRUE);
				AddProcessSecurity("admin","PURORD","PRINT PURCHASE ORDERS",TRUE);
				AddProcessSecurity("admin","PURORD","PRTAPO",TRUE);
				AddProcessSecurity("admin","PURORD","PRTAPO-APP",TRUE);
				AddProcessSecurity("admin","PURORD","ARCHIVEPO",TRUE);
				AddProcessSecurity("admin","PURORD","ARCHIVEPO1",TRUE);
				AddProcessSecurity("admin","PURORD","ARCHIVEPO2",TRUE);
				AddProcessSecurity("admin","PURORD","PURORD EMAILPO",TRUE);
				AddProcessSecurity("admin","PURORD","EMAILPO",TRUE);
				AddProcessSecurity("admin","PURORD","EMAILPO2",TRUE);
			} else if(use_approve && approve_type>0)
			{
				AddProcessSecurity("admin","PURORD","ACCOUNTMANAGER",TRUE);
				AddProcessSecurity("admin","PURORD","ACCOUNTMANAGER1",TRUE);
				AddProcessSecurity("admin","PURORD","ALLACCTMGRS",TRUE);
				if(use_site)
				{
					AddProcessSecurity("admin","PURORD","SITEAPPROVE",TRUE);
					AddProcessSecurity("admin","PURORD","SITEAPPROVE1",TRUE);
				}
			}


		}
		if(XPERT_SETUP->CATALOGUE==TRUE)
		{
			AddModuleSecurity("admin","CATALOGUE",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("acctmgr","CATALOGUE",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry","CATALOGUE",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-po","CATALOGUE",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-hrpo","CATALOGUE",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
		}
		if(XPERT_SETUP->BUDPREP==TRUE) 
		{
#ifdef _NOT_YET_
			AddModuleSecurity("admin","BUDPREP",TRUE,FALSE,TRUE,TRUE,TRUE,TRUE,TRUE);
			AddModuleSecurity("acctmgr","BUDPREP",TRUE,FALSE,TRUE,TRUE,TRUE,TRUE,TRUE);
			AddModuleSecurity("data-entry","BUDPREP",TRUE,FALSE,TRUE,TRUE,TRUE,TRUE,TRUE);
#endif /* _NOT_YET_ */
		}
		if(XPERT_SETUP->PRSNNL==TRUE) 
		{
			AddModuleSecurity("admin","PRSNNL",FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("admin","IIDMGT",FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE);
			AddModuleSecurity("admin","JOBPERF",FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE);
			AddModuleSecurity("acctmgr","IIDMGT",FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE);
			AddModuleSecurity("acctmgr","JOBPERF",FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE);
			AddModuleSecurity("acctmgr","PRSNNL",FALSE,FALSE,FALSE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry","PRSNNL",FALSE,FALSE,FALSE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry","JOBPERF",FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE);
			AddModuleSecurity("data-entry","IIDMGT",FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE);
			AddModuleSecurity("data-entry-hr","PRSNNL",FALSE,FALSE,FALSE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-hr","JOBPERF",FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE);
			AddModuleSecurity("data-entry-hr","IIDMGT",FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE);
			AddModuleSecurity("data-entry-hrpo","PRSNNL",FALSE,FALSE,FALSE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-hrpo","JOBPERF",FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE);
			AddModuleSecurity("data-entry-hrpo","IIDMGT",FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE);

			AddFileSecurity("admin","PRSNNL","PERCERT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PRSNNL","PERCRT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PRSNNL","PERCT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PRSNNL","PERCTYP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PRSNNL","PERCTY",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PRSNNL","PERDEP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PRSNNL","PERDMG",FALSE,TRUE,TRUE,TRUE,FALSE,"([PRSNNL][PERDMG][LOCATION IDENTIFICATION]=[USERS LOCATION IDENTIFICATION]) OR ([PRSNNL][PERDMG][PERSONNEL CLASS]=\"SUBSTITUTES\")");
			AddFileSecurity("admin","PRSNNL","PERDNTL",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PRSNNL","PERDOC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PRSNNL","PERDTYP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PRSNNL","PEREDM",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PRSNNL","PEREDT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PRSNNL","PEREND",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PRSNNL","PERENDT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PRSNNL","PERETH",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PRSNNL","PERFLD",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PRSNNL","PERFLEX",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PRSNNL","PERHLTH",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PRSNNL","PERIA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PRSNNL","PERLIFE",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PRSNNL","PERSFS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PRSNNL","PERSCC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","PRSNNL","PERSTAT",FALSE,TRUE,TRUE,TRUE,FALSE,"");


			AddFileSecurity("acctmgr","PRSNNL","PERCERT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PRSNNL","PERCRT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PRSNNL","PERCT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PRSNNL","PERCTYP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PRSNNL","PERDEP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PRSNNL","PERDMG",FALSE,TRUE,TRUE,TRUE,FALSE,"([PRSNNL][PERDMG][LOCATION IDENTIFICATION]=[USERS LOCATION IDENTIFICATION]) OR ([PRSNNL][PERDMG][PERSONNEL CLASS]=\"SUBSTITUTES\")");
			AddFileSecurity("acctmgr","PRSNNL","PERDNTL",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PRSNNL","PERDOC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PRSNNL","PERDTYP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PRSNNL","PEREDM",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PRSNNL","PEREDT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PRSNNL","PEREND",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PRSNNL","PERENDT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PRSNNL","PERETH",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PRSNNL","PERFLD",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PRSNNL","PERFLEX",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PRSNNL","PERHLTH",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PRSNNL","PERIA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PRSNNL","PERLIFE",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PRSNNL","PERSFS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PRSNNL","PERSCC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PRSNNL","PERSTAT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","PRSNNL","PERCTY",FALSE,TRUE,TRUE,TRUE,FALSE,"");

			AddFileSecurity("data-entry","PRSNNL","PERCERT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PRSNNL","PERCRT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PRSNNL","PERCT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PRSNNL","PERCTYP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PRSNNL","PERDEP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PRSNNL","PERDMG",FALSE,TRUE,TRUE,TRUE,FALSE,"([PRSNNL][PERDMG][LOCATION IDENTIFICATION]=[USERS LOCATION IDENTIFICATION]) OR ([PRSNNL][PERDMG][PERSONNEL CLASS]=\"SUBSTITUTES\")");
			AddFileSecurity("data-entry","PRSNNL","PERDNTL",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PRSNNL","PERDOC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PRSNNL","PERDTYP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PRSNNL","PEREDM",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PRSNNL","PEREDT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PRSNNL","PEREND",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PRSNNL","PERENDT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PRSNNL","PERETH",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PRSNNL","PERFLD",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PRSNNL","PERFLEX",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PRSNNL","PERHLTH",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PRSNNL","PERIA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PRSNNL","PERLIFE",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PRSNNL","PERSFS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PRSNNL","PERSCC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PRSNNL","PERSTAT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","PRSNNL","PERCTY",FALSE,TRUE,TRUE,TRUE,FALSE,"");

			AddFileSecurity("data-entry-hr","PRSNNL","PERCERT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","PRSNNL","PERCRT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","PRSNNL","PERCT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","PRSNNL","PERCTYP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","PRSNNL","PERDEP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","PRSNNL","PERDMG",FALSE,TRUE,TRUE,TRUE,FALSE,"([PRSNNL][PERDMG][LOCATION IDENTIFICATION]=[USERS LOCATION IDENTIFICATION]) OR ([PRSNNL][PERDMG][PERSONNEL CLASS]=\"SUBSTITUTES\")");
			AddFileSecurity("data-entry-hr","PRSNNL","PERDNTL",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","PRSNNL","PERDOC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","PRSNNL","PERDTYP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","PRSNNL","PEREDM",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","PRSNNL","PEREDT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","PRSNNL","PERCTY",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","PRSNNL","PEREND",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","PRSNNL","PERENDT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","PRSNNL","PERETH",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","PRSNNL","PERFLD",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","PRSNNL","PERFLEX",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","PRSNNL","PERHLTH",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","PRSNNL","PERIA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","PRSNNL","PERLIFE",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","PRSNNL","PERSFS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","PRSNNL","PERSCC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","PRSNNL","PERSTAT",FALSE,TRUE,TRUE,TRUE,FALSE,"");

			AddFileSecurity("data-entry-hrpo","PRSNNL","PERCERT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PRSNNL","PERCRT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PRSNNL","PERCT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PRSNNL","PERCTYP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PRSNNL","PERDEP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PRSNNL","PERDMG",FALSE,TRUE,TRUE,TRUE,FALSE,"([PRSNNL][PERDMG][LOCATION IDENTIFICATION]=[USERS LOCATION IDENTIFICATION]) OR ([PRSNNL][PERDMG][PERSONNEL CLASS]=\"SUBSTITUTES\")");
			AddFileSecurity("data-entry-hrpo","PRSNNL","PERDNTL",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PRSNNL","PERDOC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PRSNNL","PERDTYP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PRSNNL","PEREDM",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PRSNNL","PEREDT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PRSNNL","PERCTY",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PRSNNL","PEREND",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PRSNNL","PERENDT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PRSNNL","PERETH",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PRSNNL","PERFLD",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PRSNNL","PERFLEX",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PRSNNL","PERHLTH",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PRSNNL","PERIA",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PRSNNL","PERLIFE",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PRSNNL","PERSFS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PRSNNL","PERSCC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","PRSNNL","PERSTAT",FALSE,TRUE,TRUE,TRUE,FALSE,"");

			AddProcessSecurity("admin","PRSNNL","MTN PERDMG",TRUE);
			AddProcessSecurity("admin","PRSNNL","MTN PERDMG.SEARCH",TRUE);
			AddProcessSecurity("admin","PRSNNL","MTN PERDOC",TRUE);
			AddProcessSecurity("admin","PRSNNL","MTN PERDEP",TRUE);
			AddProcessSecurity("admin","PRSNNL","MTN PERCRT",TRUE);
			AddProcessSecurity("admin","PRSNNL","MTN PERCERT",TRUE);
			AddProcessSecurity("admin","PRSNNL","MTN PEREDM",TRUE);
			AddProcessSecurity("admin","PRSNNL","MTN PEREND",TRUE);
			AddProcessSecurity("admin","PRSNNL","EMPLOYEE LIST4",TRUE);
			AddProcessSecurity("admin","PRSNNL","EMPLOYEE LIST",TRUE);
			AddProcessSecurity("admin","PRSNNL","ETHNICREPORT",TRUE);
			AddProcessSecurity("admin","PRSNNL","EMPLOYEE LIST BY CLASS",TRUE);
			AddProcessSecurity("admin","PRSNNL","EMPLOYEE LIST2",TRUE);
			AddProcessSecurity("admin","PRSNNL","EMPLOYEE LIST BY LOCATION",TRUE);
			AddProcessSecurity("admin","PRSNNL","EMPLOYEE LIST BY DATE",TRUE);
			AddProcessSecurity("admin","PRSNNL","EMPLOYEE LIST5",TRUE);
			AddProcessSecurity("admin","PRSNNL","EMPLOYEE LIST3",TRUE);
			AddProcessSecurity("admin","PRSNNL","LABEL",TRUE);
			AddProcessSecurity("admin","PRSNNL","ADDRESSASCII",TRUE);
		}
		if(XPERT_SETUP->POSTRK==TRUE) 
		{
			AddModuleSecurity("admin","POSTRK",TRUE,TRUE,FALSE,TRUE,TRUE,TRUE,TRUE);
			AddModuleSecurity("acctmgr","POSTRK",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry","POSTRK",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,TRUE);
			AddModuleSecurity("data-entry-hr","POSTRK",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,TRUE);
			AddModuleSecurity("data-entry-hrpo","POSTRK",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,TRUE);
		}
		if(XPERT_SETUP->PAYROLL==TRUE) 
		{
			AddModuleSecurity("admin","PAYROLL",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,TRUE);
			AddModuleSecurity("acctmgr","PAYROLL",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,TRUE);
			AddModuleSecurity("data-entry","PAYROLL",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,TRUE);
			AddModuleSecurity("data-entry-hr","PAYROLL",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,TRUE);
			AddModuleSecurity("data-entry-hrpo","PAYROLL",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,TRUE);
			AddModuleSecurity("data-entry-ar","PAYROLL",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,TRUE);
			AddModuleSecurity("data-entry-po","PAYROLL",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,TRUE);
		}
		if(XPERT_SETUP->LVEMGT==TRUE) 
		{
			AddModuleSecurity("admin","LVEMGT",FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("acctmgr","LVEMGT",FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE);
			AddModuleSecurity("data-entry","LVEMGT",FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-hr","LVEMGT",FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-hrpo","LVEMGT",FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);

			AddFileSecurity("admin","LVEMGT","LVEDOCK",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","LVEMGT","LVEDSC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","LVEMGT","LVEFML",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","LVEMGT","LVEJOB",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","LVEMGT","LVELTRN",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","LVEMGT","LVEMSTR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","LVEMGT","LVETPMS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","LVEMGT","LVETRAN",FALSE,TRUE,TRUE,TRUE,FALSE,"([LOGIN IDENTIFICATION]=[LVEMGT][LVETRAN][SOURCE USER]) OR ([LVEMGT][LVETRAN][USER DEPARTMENT]=[USERS LOCATION IDENTIFICATION])");


			AddFileSecurity("data-entry","LVEMGT","LVEDOCK",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","LVEMGT","LVEDSC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","LVEMGT","LVEFML",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","LVEMGT","LVEJOB",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","LVEMGT","LVELTRN",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","LVEMGT","LVEMSTR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","LVEMGT","LVETPMS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","LVEMGT","LVETRAN",FALSE,TRUE,TRUE,TRUE,FALSE,"([LOGIN IDENTIFICATION]=[LVEMGT][LVETRAN][SOURCE USER]) OR ([LVEMGT][LVETRAN][USER DEPARTMENT]=[USERS LOCATION IDENTIFICATION])");


			AddFileSecurity("data-entry-hr","LVEMGT","LVEDOCK",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","LVEMGT","LVEDSC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","LVEMGT","LVEFML",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","LVEMGT","LVEJOB",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","LVEMGT","LVELTRN",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","LVEMGT","LVEMSTR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","LVEMGT","LVETPMS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","LVEMGT","LVETRAN",FALSE,TRUE,TRUE,TRUE,FALSE,"([LOGIN IDENTIFICATION]=[LVEMGT][LVETRAN][SOURCE USER]) OR ([LVEMGT][LVETRAN][USER DEPARTMENT]=[USERS LOCATION IDENTIFICATION])");

			AddFileSecurity("data-entry-hrpo","LVEMGT","LVEDOCK",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","LVEMGT","LVEDSC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","LVEMGT","LVEFML",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","LVEMGT","LVEJOB",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","LVEMGT","LVELTRN",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","LVEMGT","LVEMSTR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","LVEMGT","LVETPMS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","LVEMGT","LVETRAN",FALSE,TRUE,TRUE,TRUE,FALSE,"([LOGIN IDENTIFICATION]=[LVEMGT][LVETRAN][SOURCE USER]) OR ([LVEMGT][LVETRAN][USER DEPARTMENT]=[USERS LOCATION IDENTIFICATION])");


			AddProcessSecurity("admin","LVEMGT","ADD LEAVE USED",TRUE);
			AddProcessSecurity("admin","LVEMGT","ADD LEAVE USED-PERSONNEL",TRUE);
			AddProcessSecurity("admin","LVEMGT","LEAUDIT-TC",TRUE);
			AddProcessSecurity("admin","LVEMGT","LUAUDIT",TRUE);
			AddProcessSecurity("admin","LVEMGT","LUAUDIT1",TRUE);
			AddProcessSecurity("admin","LVEMGT","LVEDETAIL",TRUE);
			AddProcessSecurity("admin","LVEMGT","LVEDETAIL-CLASS",TRUE);
			AddProcessSecurity("admin","LVEMGT","LVEJOBSUMREMOTE",TRUE);
			AddProcessSecurity("admin","LVEMGT","LVEMOREDETAIL",TRUE);
			AddProcessSecurity("admin","LVEMGT","LVESUM-STEP",TRUE);
			AddProcessSecurity("admin","LVEMGT","LVESUM",TRUE);
			AddProcessSecurity("admin","LVEMGT","LVETRAN",TRUE);
			AddProcessSecurity("admin","LVEMGT","LVETRAN2",TRUE);
			AddProcessSecurity("admin","LVEMGT","LVETRAN3",TRUE);
			AddProcessSecurity("admin","LVEMGT","MTN LVETRAN",TRUE);
			AddProcessSecurity("admin","LVEMGT","MTN LVETRAN.SEARCH",TRUE);
			AddProcessSecurity("admin","LVEMGT","PRINCIPALSVCRPT",TRUE);

			AddProcessSecurity("data-entry","LVEMGT","ADD LEAVE USED",TRUE);
			AddProcessSecurity("data-entry","LVEMGT","ADD LEAVE USED-PERSONNEL",TRUE);
			AddProcessSecurity("data-entry","LVEMGT","LEAUDIT-TC",TRUE);
			AddProcessSecurity("data-entry","LVEMGT","LUAUDIT",TRUE);
			AddProcessSecurity("data-entry","LVEMGT","LUAUDIT1",TRUE);
			AddProcessSecurity("data-entry","LVEMGT","LVEDETAIL",TRUE);
			AddProcessSecurity("data-entry","LVEMGT","LVEDETAIL-CLASS",TRUE);
			AddProcessSecurity("data-entry","LVEMGT","LVEJOBSUMREMOTE",TRUE);
			AddProcessSecurity("data-entry","LVEMGT","LVEMOREDETAIL",TRUE);
			AddProcessSecurity("data-entry","LVEMGT","LVESUM-STEP",TRUE);
			AddProcessSecurity("data-entry","LVEMGT","LVESUM",TRUE);
			AddProcessSecurity("data-entry","LVEMGT","LVETRAN",TRUE);
			AddProcessSecurity("data-entry","LVEMGT","LVETRAN2",TRUE);
			AddProcessSecurity("data-entry","LVEMGT","LVETRAN3",TRUE);
			AddProcessSecurity("data-entry","LVEMGT","MTN LVETRAN",TRUE);
			AddProcessSecurity("data-entry","LVEMGT","MTN LVETRAN.SEARCH",TRUE);
			AddProcessSecurity("data-entry","LVEMGT","PRINCIPALSVCRPT",TRUE);


			AddProcessSecurity("data-entry-hr","LVEMGT","ADD LEAVE USED",TRUE);
			AddProcessSecurity("data-entry-hr","LVEMGT","ADD LEAVE USED-PERSONNEL",TRUE);
			AddProcessSecurity("data-entry-hr","LVEMGT","LEAUDIT-TC",TRUE);
			AddProcessSecurity("data-entry-hr","LVEMGT","LUAUDIT",TRUE);
			AddProcessSecurity("data-entry-hr","LVEMGT","LUAUDIT1",TRUE);
			AddProcessSecurity("data-entry-hr","LVEMGT","LVEDETAIL",TRUE);
			AddProcessSecurity("data-entry-hr","LVEMGT","LVEDETAIL-CLASS",TRUE);
			AddProcessSecurity("data-entry-hr","LVEMGT","LVEJOBSUMREMOTE",TRUE);
			AddProcessSecurity("data-entry-hr","LVEMGT","LVEMOREDETAIL",TRUE);
			AddProcessSecurity("data-entry-hr","LVEMGT","LVESUM-STEP",TRUE);
			AddProcessSecurity("data-entry-hr","LVEMGT","LVESUM",TRUE);
			AddProcessSecurity("data-entry-hr","LVEMGT","LVETRAN",TRUE);
			AddProcessSecurity("data-entry-hr","LVEMGT","LVETRAN2",TRUE);
			AddProcessSecurity("data-entry-hr","LVEMGT","LVETRAN3",TRUE);
			AddProcessSecurity("data-entry-hr","LVEMGT","MTN LVETRAN",TRUE);
			AddProcessSecurity("data-entry-hr","LVEMGT","MTN LVETRAN.SEARCH",TRUE);
			AddProcessSecurity("data-entry-hr","LVEMGT","PRINCIPALSVCRPT",TRUE);

			AddProcessSecurity("data-entry-hrpo","LVEMGT","ADD LEAVE USED",TRUE);
			AddProcessSecurity("data-entry-hrpo","LVEMGT","ADD LEAVE USED-PERSONNEL",TRUE);
			AddProcessSecurity("data-entry-hrpo","LVEMGT","LEAUDIT-TC",TRUE);
			AddProcessSecurity("data-entry-hrpo","LVEMGT","LUAUDIT",TRUE);
			AddProcessSecurity("data-entry-hrpo","LVEMGT","LUAUDIT1",TRUE);
			AddProcessSecurity("data-entry-hrpo","LVEMGT","LVEDETAIL",TRUE);
			AddProcessSecurity("data-entry-hrpo","LVEMGT","LVEDETAIL-CLASS",TRUE);
			AddProcessSecurity("data-entry-hrpo","LVEMGT","LVEJOBSUMREMOTE",TRUE);
			AddProcessSecurity("data-entry-hrpo","LVEMGT","LVEMOREDETAIL",TRUE);
			AddProcessSecurity("data-entry-hrpo","LVEMGT","LVESUM-STEP",TRUE);
			AddProcessSecurity("data-entry-hrpo","LVEMGT","LVESUM",TRUE);
			AddProcessSecurity("data-entry-hrpo","LVEMGT","LVETRAN",TRUE);
			AddProcessSecurity("data-entry-hrpo","LVEMGT","LVETRAN2",TRUE);
			AddProcessSecurity("data-entry-hrpo","LVEMGT","LVETRAN3",TRUE);
			AddProcessSecurity("data-entry-hrpo","LVEMGT","MTN LVETRAN",TRUE);
			AddProcessSecurity("data-entry-hrpo","LVEMGT","MTN LVETRAN.SEARCH",TRUE);
			AddProcessSecurity("data-entry-hrpo","LVEMGT","PRINCIPALSVCRPT",TRUE);


		}
		if(XPERT_SETUP->LVSIMP==TRUE)
		{
			AddModuleSecurity("admin","LVSIMP",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
			AddModuleSecurity("acctmgr","LVSIMP",FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE);
			AddModuleSecurity("data-entry","LVSIMP",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		}
		if(XPERT_SETUP->SUBMGT==TRUE) 
		{
			AddModuleSecurity("admin","SUBMGT",FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("acctmgr","SUBMGT",FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE);
			AddModuleSecurity("data-entry","SUBMGT",FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-hr","SUBMGT",FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-hrpo","SUBMGT",FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);

			AddFileSecurity("admin","SUBMGT","SUBCTACT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","SUBMGT","SUBDTYP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","SUBMGT","SUBEMP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","SUBMGT","SUBLOC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","SUBMGT","SUBMASTR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","SUBMGT","SUBPAID",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","SUBMGT","SUBPOS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","SUBMGT","SUBWKMAS",FALSE,TRUE,TRUE,TRUE,FALSE,"([LOGIN IDENTIFICATION]=[SUBMGT][SUBWKMAS][SOURCE USER])");


			AddFileSecurity("data-entry","SUBMGT","SUBCTACT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","SUBMGT","SUBDTYP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","SUBMGT","SUBEMP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","SUBMGT","SUBLOC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","SUBMGT","SUBMASTR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","SUBMGT","SUBPAID",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","SUBMGT","SUBPOS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","SUBMGT","SUBWKMAS",FALSE,TRUE,TRUE,TRUE,FALSE,"([LOGIN IDENTIFICATION]=[SUBMGT][SUBWKMAS][SOURCE USER])");


			AddFileSecurity("data-entry-hr","SUBMGT","SUBCTACT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","SUBMGT","SUBDTYP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","SUBMGT","SUBEMP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","SUBMGT","SUBLOC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","SUBMGT","SUBMASTR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","SUBMGT","SUBPAID",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","SUBMGT","SUBPOS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","SUBMGT","SUBWKMAS",FALSE,TRUE,TRUE,TRUE,FALSE,"([LOGIN IDENTIFICATION]=[SUBMGT][SUBWKMAS][SOURCE USER])");

			AddFileSecurity("data-entry-hrpo","SUBMGT","SUBCTACT",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","SUBMGT","SUBDTYP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","SUBMGT","SUBEMP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","SUBMGT","SUBLOC",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","SUBMGT","SUBMASTR",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","SUBMGT","SUBPAID",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","SUBMGT","SUBPOS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","SUBMGT","SUBWKMAS",FALSE,TRUE,TRUE,TRUE,FALSE,"([LOGIN IDENTIFICATION]=[SUBMGT][SUBWKMAS][SOURCE USER])");


			AddProcessSecurity("admin","SUBMGT","EXTRAPAYREPORT",TRUE);
			AddProcessSecurity("admin","SUBMGT","EXTRAPAYREPORT2",TRUE);
			AddProcessSecurity("admin","SUBMGT","EXTRAPAYREPORT3",TRUE);
			AddProcessSecurity("admin","SUBMGT","MTN SUBWKMAS",TRUE);
			AddProcessSecurity("admin","SUBMGT","MTN SUBWKMAS.SEARCH",TRUE);
			AddProcessSecurity("admin","SUBMGT","SETSUBNAME",TRUE);
			AddProcessSecurity("admin","SUBMGT","SUBLETTER",TRUE);
			AddProcessSecurity("admin","SUBMGT","SUB-PAID",TRUE);
			AddProcessSecurity("admin","SUBMGT","SUB-PAID2",TRUE);
			AddProcessSecurity("admin","SUBMGT","SUBWRKD",TRUE);
			AddProcessSecurity("admin","SUBMGT","SUBWRKD2",TRUE);
			AddProcessSecurity("admin","SUBMGT","SUBWRKD3",TRUE);
			AddProcessSecurity("admin","SUBMGT","SUBWRKD4",TRUE);

			AddProcessSecurity("data-entry","SUBMGT","EXTRAPAYREPORT",TRUE);
			AddProcessSecurity("data-entry","SUBMGT","EXTRAPAYREPORT2",TRUE);
			AddProcessSecurity("data-entry","SUBMGT","EXTRAPAYREPORT3",TRUE);
			AddProcessSecurity("data-entry","SUBMGT","MTN SUBWKMAS",TRUE);
			AddProcessSecurity("data-entry","SUBMGT","MTN SUBWKMAS.SEARCH",TRUE);
			AddProcessSecurity("data-entry","SUBMGT","SETSUBNAME",TRUE);
			AddProcessSecurity("data-entry","SUBMGT","SUBLETTER",TRUE);
			AddProcessSecurity("data-entry","SUBMGT","SUB-PAID",TRUE);
			AddProcessSecurity("data-entry","SUBMGT","SUB-PAID2",TRUE);
			AddProcessSecurity("data-entry","SUBMGT","SUBWRKD",TRUE);
			AddProcessSecurity("data-entry","SUBMGT","SUBWRKD2",TRUE);
			AddProcessSecurity("data-entry","SUBMGT","SUBWRKD3",TRUE);
			AddProcessSecurity("data-entry","SUBMGT","SUBWRKD4",TRUE);

			AddProcessSecurity("data-entry-hr","SUBMGT","EXTRAPAYREPORT",TRUE);
			AddProcessSecurity("data-entry-hr","SUBMGT","EXTRAPAYREPORT2",TRUE);
			AddProcessSecurity("data-entry-hr","SUBMGT","EXTRAPAYREPORT3",TRUE);
			AddProcessSecurity("data-entry-hr","SUBMGT","MTN SUBWKMAS",TRUE);
			AddProcessSecurity("data-entry-hr","SUBMGT","MTN SUBWKMAS.SEARCH",TRUE);
			AddProcessSecurity("data-entry-hr","SUBMGT","SETSUBNAME",TRUE);
			AddProcessSecurity("data-entry-hr","SUBMGT","SUBLETTER",TRUE);
			AddProcessSecurity("data-entry-hr","SUBMGT","SUB-PAID",TRUE);
			AddProcessSecurity("data-entry-hr","SUBMGT","SUB-PAID2",TRUE);
			AddProcessSecurity("data-entry-hr","SUBMGT","SUBWRKD",TRUE);
			AddProcessSecurity("data-entry-hr","SUBMGT","SUBWRKD2",TRUE);
			AddProcessSecurity("data-entry-hr","SUBMGT","SUBWRKD3",TRUE);
			AddProcessSecurity("data-entry-hr","SUBMGT","SUBWRKD4",TRUE);

			AddProcessSecurity("data-entry-hrpo","SUBMGT","EXTRAPAYREPORT",TRUE);
			AddProcessSecurity("data-entry-hrpo","SUBMGT","EXTRAPAYREPORT2",TRUE);
			AddProcessSecurity("data-entry-hrpo","SUBMGT","EXTRAPAYREPORT3",TRUE);
			AddProcessSecurity("data-entry-hrpo","SUBMGT","MTN SUBWKMAS",TRUE);
			AddProcessSecurity("data-entry-hrpo","SUBMGT","MTN SUBWKMAS.SEARCH",TRUE);
			AddProcessSecurity("data-entry-hrpo","SUBMGT","SETSUBNAME",TRUE);
			AddProcessSecurity("data-entry-hrpo","SUBMGT","SUBLETTER",TRUE);
			AddProcessSecurity("data-entry-hrpo","SUBMGT","SUB-PAID",TRUE);
			AddProcessSecurity("data-entry-hrpo","SUBMGT","SUB-PAID2",TRUE);
			AddProcessSecurity("data-entry-hrpo","SUBMGT","SUBWRKD",TRUE);
			AddProcessSecurity("data-entry-hrpo","SUBMGT","SUBWRKD2",TRUE);
			AddProcessSecurity("data-entry-hrpo","SUBMGT","SUBWRKD3",TRUE);
			AddProcessSecurity("data-entry-hrpo","SUBMGT","SUBWRKD4",TRUE);
		}
		if(XPERT_SETUP->EMPABS==TRUE) 
		{
			AddModuleSecurity("admin","EMPABS",FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("acctmgr","EMPABS",FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE);
			AddModuleSecurity("data-entry","EMPABS",FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-hr","EMPABS",FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-hrpo","EMPABS",FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);

			AddFileSecurity("admin","EMPABS","AESOP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","EMPABS","EMPABS",FALSE,TRUE,TRUE,TRUE,FALSE,"([LOGIN IDENTIFICATION]=[EMPABS][EMPABS][SOURCE USER])");
			AddFileSecurity("admin","EMPABS","EMPABTR",FALSE,TRUE,TRUE,TRUE,FALSE,"([LOGIN IDENTIFICATION]=[EMPABS][EMPABTR][SOURCE USER])");
			AddFileSecurity("admin","EMPABS","EMPADEF",FALSE,TRUE,TRUE,TRUE,FALSE,"");

			AddFileSecurity("data-entry","EMPABS","AESOP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","EMPABS","EMPABS",FALSE,TRUE,TRUE,TRUE,FALSE,"([LOGIN IDENTIFICATION]=[EMPABS][EMPABS][SOURCE USER])");
			AddFileSecurity("data-entry","EMPABS","EMPABTR",FALSE,TRUE,TRUE,TRUE,FALSE,"([LOGIN IDENTIFICATION]=[EMPABS][EMPABTR][SOURCE USER])");
			AddFileSecurity("data-entry","EMPABS","EMPADEF",FALSE,TRUE,TRUE,TRUE,FALSE,"");

			AddFileSecurity("data-entry-hr","EMPABS","AESOP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","EMPABS","EMPABS",FALSE,TRUE,TRUE,TRUE,FALSE,"([LOGIN IDENTIFICATION]=[EMPABS][EMPABS][SOURCE USER])");
			AddFileSecurity("data-entry-hr","EMPABS","EMPABTR",FALSE,TRUE,TRUE,TRUE,FALSE,"([LOGIN IDENTIFICATION]=[EMPABS][EMPABTR][SOURCE USER])");
			AddFileSecurity("data-entry-hr","EMPABS","EMPADEF",FALSE,TRUE,TRUE,TRUE,FALSE,"");

			AddFileSecurity("data-entry-hrpo","EMPABS","AESOP",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","EMPABS","EMPABS",FALSE,TRUE,TRUE,TRUE,FALSE,"([LOGIN IDENTIFICATION]=[EMPABS][EMPABS][SOURCE USER])");
			AddFileSecurity("data-entry-hrpo","EMPABS","EMPABTR",FALSE,TRUE,TRUE,TRUE,FALSE,"([LOGIN IDENTIFICATION]=[EMPABS][EMPABTR][SOURCE USER])");
			AddFileSecurity("data-entry-hrpo","EMPABS","EMPADEF",FALSE,TRUE,TRUE,TRUE,FALSE,"");


			AddProcessSecurity("admin","EMPABS","EMPABS RPT EMPALSANOBACK",TRUE);
			AddProcessSecurity("admin","EMPABS","EMPALS",TRUE);
			AddProcessSecurity("admin","EMPABS","EMPALSA1",TRUE);
			AddProcessSecurity("admin","EMPABS","EMPALSANOBACK",TRUE);
			AddProcessSecurity("admin","EMPABS","EMPLOYEE ABSENTEE MENU",TRUE);
			AddProcessSecurity("admin","EMPABS","EMPLOYEE ABSENTEE REPORT SUBMENU",TRUE);
			AddProcessSecurity("admin","EMPABS","MPLOYEE ABSENTEE MENU",TRUE);
			AddProcessSecurity("admin","EMPABS","MTN EMPABS",TRUE);
			AddProcessSecurity("admin","EMPABS","MTN EMPABS.BROWSE",TRUE);
			AddProcessSecurity("admin","EMPABS","MTN EMPABS.MAINTAIN",TRUE);
			AddProcessSecurity("admin","EMPABS","MTN EMPABTR",TRUE);
			AddProcessSecurity("admin","EMPABS","MTN EMPABTR.SEARCH",TRUE);

			AddProcessSecurity("data-entry","EMPABS","EMPABS RPT EMPALSANOBACK",TRUE);
			AddProcessSecurity("data-entry","EMPABS","EMPALS",TRUE);
			AddProcessSecurity("data-entry","EMPABS","EMPALSA1",TRUE);
			AddProcessSecurity("data-entry","EMPABS","EMPALSANOBACK",TRUE);
			AddProcessSecurity("data-entry","EMPABS","EMPLOYEE ABSENTEE MENU",TRUE);
			AddProcessSecurity("data-entry","EMPABS","EMPLOYEE ABSENTEE REPORT SUBMENU",TRUE);
			AddProcessSecurity("data-entry","EMPABS","MPLOYEE ABSENTEE MENU",TRUE);
			AddProcessSecurity("data-entry","EMPABS","MTN EMPABS",TRUE);
			AddProcessSecurity("data-entry","EMPABS","MTN EMPABS.BROWSE",TRUE);
			AddProcessSecurity("data-entry","EMPABS","MTN EMPABS.MAINTAIN",TRUE);
			AddProcessSecurity("data-entry","EMPABS","MTN EMPABTR",TRUE);

			AddProcessSecurity("data-entry-hr","EMPABS","EMPABS RPT EMPALSANOBACK",TRUE);
			AddProcessSecurity("data-entry-hr","EMPABS","EMPALS",TRUE);
			AddProcessSecurity("data-entry-hr","EMPABS","EMPALSA1",TRUE);
			AddProcessSecurity("data-entry-hr","EMPABS","EMPALSANOBACK",TRUE);
			AddProcessSecurity("data-entry-hr","EMPABS","EMPLOYEE ABSENTEE MENU",TRUE);
			AddProcessSecurity("data-entry-hr","EMPABS","EMPLOYEE ABSENTEE REPORT SUBMENU",TRUE);
			AddProcessSecurity("data-entry-hr","EMPABS","MPLOYEE ABSENTEE MENU",TRUE);
			AddProcessSecurity("data-entry-hr","EMPABS","MTN EMPABS",TRUE);
			AddProcessSecurity("data-entry-hr","EMPABS","MTN EMPABS.BROWSE",TRUE);
			AddProcessSecurity("data-entry-hr","EMPABS","MTN EMPABS.MAINTAIN",TRUE);
			AddProcessSecurity("data-entry-hr","EMPABS","MTN EMPABTR",TRUE);

			AddProcessSecurity("data-entry-hrpo","EMPABS","EMPABS RPT EMPALSANOBACK",TRUE);
			AddProcessSecurity("data-entry-hrpo","EMPABS","EMPALS",TRUE);
			AddProcessSecurity("data-entry-hrpo","EMPABS","EMPALSA1",TRUE);
			AddProcessSecurity("data-entry-hrpo","EMPABS","EMPALSANOBACK",TRUE);
			AddProcessSecurity("data-entry-hrpo","EMPABS","EMPLOYEE ABSENTEE MENU",TRUE);
			AddProcessSecurity("data-entry-hrpo","EMPABS","EMPLOYEE ABSENTEE REPORT SUBMENU",TRUE);
			AddProcessSecurity("data-entry-hrpo","EMPABS","MPLOYEE ABSENTEE MENU",TRUE);
			AddProcessSecurity("data-entry-hrpo","EMPABS","MTN EMPABS",TRUE);
			AddProcessSecurity("data-entry-hrpo","EMPABS","MTN EMPABS.BROWSE",TRUE);
			AddProcessSecurity("data-entry-hrpo","EMPABS","MTN EMPABS.MAINTAIN",TRUE);
			AddProcessSecurity("data-entry-hrpo","EMPABS","MTN EMPABTR",TRUE);


		}
		if(XPERT_SETUP->BFTMGT==TRUE) 
		{
			AddModuleSecurity("admin","BFTMGT",FALSE,FALSE,FALSE,TRUE,FALSE,TRUE,TRUE);
			AddModuleSecurity("acctmgr","BFTMGT",FALSE,FALSE,FALSE,TRUE,FALSE,TRUE,TRUE);
		}
		if(XPERT_SETUP->APPMGT==TRUE) 
		{
			AddModuleSecurity("admin","APPMGT",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		}
		if(XPERT_SETUP->ARCHIVE=TRUE) 
		{
			AddModuleSecurity("admin","ARCHIVE",TRUE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddFileSecurity("admin","ARCHIVE","ARCHCLS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","ARCHIVE","ARCHIVES",FALSE,TRUE,TRUE,TRUE,FALSE,"([ARCHIVE][ARCHIVES][USER IDENTIFICATION]=[SOURCE USER])");
			AddFileSecurity("admin","ARCHIVE","ARCHPROG",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","ARCHIVE","ARCHSUB",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","ARCHIVE","ARCHTYPE",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","ARCHIVE","ARCHUSER",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("admin","ARCHIVE","ARCHWORK",FALSE,TRUE,TRUE,TRUE,FALSE,"");

			AddModuleSecurity("acctmgr","ARCHIVE",TRUE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddFileSecurity("acctmgr","ARCHIVE","ARCHCLS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","ARCHIVE","ARCHIVES",FALSE,TRUE,TRUE,TRUE,FALSE,"([ARCHIVE][ARCHIVES][USER IDENTIFICATION]=[SOURCE USER])");
			AddFileSecurity("acctmgr","ARCHIVE","ARCHPROG",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","ARCHIVE","ARCHSUB",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","ARCHIVE","ARCHTYPE",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","ARCHIVE","ARCHUSER",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("acctmgr","ARCHIVE","ARCHWORK",FALSE,TRUE,TRUE,TRUE,FALSE,"");

			AddModuleSecurity("data-entry","ARCHIVE",TRUE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddFileSecurity("data-entry","ARCHIVE","ARCHCLS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","ARCHIVE","ARCHIVES",FALSE,TRUE,TRUE,TRUE,FALSE,"([ARCHIVE][ARCHIVES][USER IDENTIFICATION]=[SOURCE USER])");
			AddFileSecurity("data-entry","ARCHIVE","ARCHPROG",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","ARCHIVE","ARCHSUB",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","ARCHIVE","ARCHTYPE",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","ARCHIVE","ARCHUSER",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry","ARCHIVE","ARCHWORK",FALSE,TRUE,TRUE,TRUE,FALSE,"");

			AddModuleSecurity("data-entry-ar","ARCHIVE",TRUE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddFileSecurity("data-entry-ar","ARCHIVE","ARCHCLS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","ARCHIVE","ARCHIVES",FALSE,TRUE,TRUE,TRUE,FALSE,"([ARCHIVE][ARCHIVES][USER IDENTIFICATION]=[SOURCE USER])");
			AddFileSecurity("data-entry-ar","ARCHIVE","ARCHPROG",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","ARCHIVE","ARCHSUB",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","ARCHIVE","ARCHTYPE",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","ARCHIVE","ARCHUSER",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-ar","ARCHIVE","ARCHWORK",FALSE,TRUE,TRUE,TRUE,FALSE,"");

			AddModuleSecurity("data-entry-po","ARCHIVE",TRUE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddFileSecurity("data-entry-po","ARCHIVE","ARCHCLS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","ARCHIVE","ARCHIVES",FALSE,TRUE,TRUE,TRUE,FALSE,"([ARCHIVE][ARCHIVES][USER IDENTIFICATION]=[SOURCE USER])");
			AddFileSecurity("data-entry-po","ARCHIVE","ARCHPROG",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","ARCHIVE","ARCHSUB",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","ARCHIVE","ARCHTYPE",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","ARCHIVE","ARCHUSER",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-po","ARCHIVE","ARCHWORK",FALSE,TRUE,TRUE,TRUE,FALSE,"");

			AddModuleSecurity("data-entry-hr","ARCHIVE",TRUE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddFileSecurity("data-entry-hr","ARCHIVE","ARCHCLS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","ARCHIVE","ARCHIVES",FALSE,TRUE,TRUE,TRUE,FALSE,"([ARCHIVE][ARCHIVES][USER IDENTIFICATION]=[SOURCE USER])");
			AddFileSecurity("data-entry-hr","ARCHIVE","ARCHPROG",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","ARCHIVE","ARCHSUB",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","ARCHIVE","ARCHTYPE",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","ARCHIVE","ARCHUSER",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hr","ARCHIVE","ARCHWORK",FALSE,TRUE,TRUE,TRUE,FALSE,"");

			AddModuleSecurity("data-entry-hrpo","ARCHIVE",TRUE,FALSE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddFileSecurity("data-entry-hrpo","ARCHIVE","ARCHCLS",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","ARCHIVE","ARCHIVES",FALSE,TRUE,TRUE,TRUE,FALSE,"([ARCHIVE][ARCHIVES][USER IDENTIFICATION]=[SOURCE USER])");
			AddFileSecurity("data-entry-hrpo","ARCHIVE","ARCHPROG",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","ARCHIVE","ARCHSUB",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","ARCHIVE","ARCHTYPE",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","ARCHIVE","ARCHUSER",FALSE,TRUE,TRUE,TRUE,FALSE,"");
			AddFileSecurity("data-entry-hrpo","ARCHIVE","ARCHWORK",FALSE,TRUE,TRUE,TRUE,FALSE,"");


			AddProcessSecurity("admin","ARCHIVE","ADD ARCHIVE",TRUE);
			AddProcessSecurity("admin","ARCHIVE","ADD ARCHPROG",FALSE);
			AddProcessSecurity("admin","ARCHIVE","ADD WORK GROUP",FALSE);
			AddProcessSecurity("admin","ARCHIVE","ADD WORK GROUP USERS",FALSE);
			AddProcessSecurity("admin","ARCHIVE","ARCHCLS-EXPORT",FALSE);
			AddProcessSecurity("admin","ARCHIVE","ARCHIVES-EXPORT",FALSE);
			AddProcessSecurity("admin","ARCHIVE","ARCHIVESIN",FALSE);
			AddProcessSecurity("admin","ARCHIVE","ARCHIVESOUT",FALSE);
			AddProcessSecurity("admin","ARCHIVE","ARCHPROG-EXPORT",FALSE);
			AddProcessSecurity("admin","ARCHIVE","ARCHSUB-EXPORT",FALSE);
			AddProcessSecurity("admin","ARCHIVE","ARCHTYPE-EXPORT",FALSE);
			AddProcessSecurity("admin","ARCHIVE","ARCHUSER-EXPORT",FALSE);
			AddProcessSecurity("admin","ARCHIVE","ARCHWORK-EXPORT",FALSE);
			AddProcessSecurity("admin","ARCHIVE","AUTOWRKGRP",FALSE);
			AddProcessSecurity("admin","ARCHIVE","AUTOWRKGRPDEPT",FALSE);
			AddProcessSecurity("admin","ARCHIVE","COPYARCHIVE",FALSE);
			AddProcessSecurity("admin","ARCHIVE","COPYGROUPUSER",FALSE);
			AddProcessSecurity("admin","ARCHIVE","DOARCHIVE",TRUE);
			AddProcessSecurity("admin","ARCHIVE","EMAILARCHIVE",TRUE);
			AddProcessSecurity("admin","ARCHIVE","EMAILARCHIVE2",TRUE);
			AddProcessSecurity("admin","ARCHIVE","MANAGE SCANS",TRUE);
			AddProcessSecurity("admin","ARCHIVE","MTN ARCHCLS",FALSE);
			AddProcessSecurity("admin","ARCHIVE","MTN ARCHIVES",TRUE);
			AddProcessSecurity("admin","ARCHIVE","MTN ARCHIVES.BROWSE",TRUE);
			AddProcessSecurity("admin","ARCHIVE","MTN ARCHIVES.SEARCH",TRUE);
			AddProcessSecurity("admin","ARCHIVE","MTN ARCHPROG",FALSE);
			AddProcessSecurity("admin","ARCHIVE","MTN ARCHSUB",FALSE);
			AddProcessSecurity("admin","ARCHIVE","MTN ARCHTYPE",FALSE);
			AddProcessSecurity("admin","ARCHIVE","MTN ARCHUSER",FALSE);
			AddProcessSecurity("admin","ARCHIVE","MTN ARCHUSER.SEARCH",FALSE);
			AddProcessSecurity("admin","ARCHIVE","MTN ARCHWORK",FALSE);
			AddProcessSecurity("admin","ARCHIVE","MTN ARCHWORK.SEARCH",FALSE);
			AddProcessSecurity("admin","ARCHIVE","PURGEARCHIVE",FALSE);
			AddProcessSecurity("admin","ARCHIVE","RPTARCHIVE",FALSE);
			AddProcessSecurity("admin","ARCHIVE","WORK GROUPS",FALSE);
			AddProcessSecurity("admin","ARCHIVE","WORKGROUPUSER",FALSE);


			AddProcessSecurity("acctmgr","ARCHIVE","ADD ARCHIVE",TRUE);
			AddProcessSecurity("acctmgr","ARCHIVE","ADD ARCHPROG",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","ADD WORK GROUP",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","ADD WORK GROUP USERS",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","ARCHCLS-EXPORT",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","ARCHIVES-EXPORT",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","ARCHIVESIN",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","ARCHIVESOUT",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","ARCHPROG-EXPORT",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","ARCHSUB-EXPORT",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","ARCHTYPE-EXPORT",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","ARCHUSER-EXPORT",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","ARCHWORK-EXPORT",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","AUTOWRKGRP",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","AUTOWRKGRPDEPT",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","COPYARCHIVE",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","COPYGROUPUSER",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","DOARCHIVE",TRUE);
			AddProcessSecurity("acctmgr","ARCHIVE","EMAILARCHIVE",TRUE);
			AddProcessSecurity("acctmgr","ARCHIVE","EMAILARCHIVE2",TRUE);
			AddProcessSecurity("acctmgr","ARCHIVE","MANAGE SCANS",TRUE);
			AddProcessSecurity("acctmgr","ARCHIVE","MTN ARCHCLS",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","MTN ARCHIVES",TRUE);
			AddProcessSecurity("acctmgr","ARCHIVE","MTN ARCHIVES.BROWSE",TRUE);
			AddProcessSecurity("acctmgr","ARCHIVE","MTN ARCHIVES.SEARCH",TRUE);
			AddProcessSecurity("acctmgr","ARCHIVE","MTN ARCHPROG",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","MTN ARCHSUB",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","MTN ARCHTYPE",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","MTN ARCHUSER",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","MTN ARCHUSER.SEARCH",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","MTN ARCHWORK",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","MTN ARCHWORK.SEARCH",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","PURGEARCHIVE",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","RPTARCHIVE",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","WORK GROUPS",FALSE);
			AddProcessSecurity("acctmgr","ARCHIVE","WORKGROUPUSER",FALSE);


			AddProcessSecurity("data-entry","ARCHIVE","ADD ARCHIVE",TRUE);
			AddProcessSecurity("data-entry","ARCHIVE","ADD ARCHPROG",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","ADD WORK GROUP",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","ADD WORK GROUP USERS",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","ARCHCLS-EXPORT",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","ARCHIVES-EXPORT",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","ARCHIVESIN",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","ARCHIVESOUT",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","ARCHPROG-EXPORT",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","ARCHSUB-EXPORT",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","ARCHTYPE-EXPORT",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","ARCHUSER-EXPORT",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","ARCHWORK-EXPORT",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","AUTOWRKGRP",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","AUTOWRKGRPDEPT",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","COPYARCHIVE",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","COPYGROUPUSER",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","DOARCHIVE",TRUE);
			AddProcessSecurity("data-entry","ARCHIVE","EMAILARCHIVE",TRUE);
			AddProcessSecurity("data-entry","ARCHIVE","EMAILARCHIVE2",TRUE);
			AddProcessSecurity("data-entry","ARCHIVE","MANAGE SCANS",TRUE);
			AddProcessSecurity("data-entry","ARCHIVE","MTN ARCHCLS",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","MTN ARCHIVES",TRUE);
			AddProcessSecurity("data-entry","ARCHIVE","MTN ARCHIVES.BROWSE",TRUE);
			AddProcessSecurity("data-entry","ARCHIVE","MTN ARCHIVES.SEARCH",TRUE);
			AddProcessSecurity("data-entry","ARCHIVE","MTN ARCHPROG",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","MTN ARCHSUB",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","MTN ARCHTYPE",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","MTN ARCHUSER",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","MTN ARCHUSER.SEARCH",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","MTN ARCHWORK",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","MTN ARCHWORK.SEARCH",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","PURGEARCHIVE",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","RPTARCHIVE",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","WORK GROUPS",FALSE);
			AddProcessSecurity("data-entry","ARCHIVE","WORKGROUPUSER",FALSE);

			AddProcessSecurity("data-entry-ar","ARCHIVE","ADD ARCHIVE",TRUE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","ADD ARCHPROG",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","ADD WORK GROUP",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","ADD WORK GROUP USERS",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","ARCHCLS-EXPORT",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","ARCHIVES-EXPORT",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","ARCHIVESIN",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","ARCHIVESOUT",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","ARCHPROG-EXPORT",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","ARCHSUB-EXPORT",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","ARCHTYPE-EXPORT",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","ARCHUSER-EXPORT",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","ARCHWORK-EXPORT",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","AUTOWRKGRP",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","AUTOWRKGRPDEPT",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","COPYARCHIVE",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","COPYGROUPUSER",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","DOARCHIVE",TRUE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","EMAILARCHIVE",TRUE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","EMAILARCHIVE2",TRUE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","MANAGE SCANS",TRUE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","MTN ARCHCLS",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","MTN ARCHIVES",TRUE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","MTN ARCHIVES.BROWSE",TRUE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","MTN ARCHIVES.SEARCH",TRUE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","MTN ARCHPROG",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","MTN ARCHSUB",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","MTN ARCHTYPE",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","MTN ARCHUSER",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","MTN ARCHUSER.SEARCH",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","MTN ARCHWORK",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","MTN ARCHWORK.SEARCH",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","PURGEARCHIVE",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","RPTARCHIVE",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","WORK GROUPS",FALSE);
			AddProcessSecurity("data-entry-ar","ARCHIVE","WORKGROUPUSER",FALSE);

			AddProcessSecurity("data-entry-hr","ARCHIVE","ADD ARCHIVE",TRUE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","ADD ARCHPROG",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","ADD WORK GROUP",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","ADD WORK GROUP USERS",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","ARCHCLS-EXPORT",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","ARCHIVES-EXPORT",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","ARCHIVESIN",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","ARCHIVESOUT",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","ARCHPROG-EXPORT",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","ARCHSUB-EXPORT",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","ARCHTYPE-EXPORT",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","ARCHUSER-EXPORT",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","ARCHWORK-EXPORT",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","AUTOWRKGRP",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","AUTOWRKGRPDEPT",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","COPYARCHIVE",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","COPYGROUPUSER",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","DOARCHIVE",TRUE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","EMAILARCHIVE",TRUE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","EMAILARCHIVE2",TRUE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","MANAGE SCANS",TRUE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","MTN ARCHCLS",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","MTN ARCHIVES",TRUE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","MTN ARCHIVES.BROWSE",TRUE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","MTN ARCHIVES.SEARCH",TRUE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","MTN ARCHPROG",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","MTN ARCHSUB",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","MTN ARCHTYPE",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","MTN ARCHUSER",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","MTN ARCHUSER.SEARCH",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","MTN ARCHWORK",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","MTN ARCHWORK.SEARCH",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","PURGEARCHIVE",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","RPTARCHIVE",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","WORK GROUPS",FALSE);
			AddProcessSecurity("data-entry-hr","ARCHIVE","WORKGROUPUSER",FALSE);

			AddProcessSecurity("data-entry-po","ARCHIVE","ADD ARCHIVE",TRUE);
			AddProcessSecurity("data-entry-po","ARCHIVE","ADD ARCHPROG",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","ADD WORK GROUP",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","ADD WORK GROUP USERS",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","ARCHCLS-EXPORT",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","ARCHIVES-EXPORT",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","ARCHIVESIN",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","ARCHIVESOUT",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","ARCHPROG-EXPORT",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","ARCHSUB-EXPORT",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","ARCHTYPE-EXPORT",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","ARCHUSER-EXPORT",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","ARCHWORK-EXPORT",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","AUTOWRKGRP",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","AUTOWRKGRPDEPT",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","COPYARCHIVE",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","COPYGROUPUSER",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","DOARCHIVE",TRUE);
			AddProcessSecurity("data-entry-po","ARCHIVE","EMAILARCHIVE",TRUE);
			AddProcessSecurity("data-entry-po","ARCHIVE","EMAILARCHIVE2",TRUE);
			AddProcessSecurity("data-entry-po","ARCHIVE","MANAGE SCANS",TRUE);
			AddProcessSecurity("data-entry-po","ARCHIVE","MTN ARCHCLS",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","MTN ARCHIVES",TRUE);
			AddProcessSecurity("data-entry-po","ARCHIVE","MTN ARCHIVES.BROWSE",TRUE);
			AddProcessSecurity("data-entry-po","ARCHIVE","MTN ARCHIVES.SEARCH",TRUE);
			AddProcessSecurity("data-entry-po","ARCHIVE","MTN ARCHPROG",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","MTN ARCHSUB",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","MTN ARCHTYPE",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","MTN ARCHUSER",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","MTN ARCHUSER.SEARCH",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","MTN ARCHWORK",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","MTN ARCHWORK.SEARCH",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","PURGEARCHIVE",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","RPTARCHIVE",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","WORK GROUPS",FALSE);
			AddProcessSecurity("data-entry-po","ARCHIVE","WORKGROUPUSER",FALSE);

			AddProcessSecurity("data-entry-hrpo","ARCHIVE","ADD ARCHIVE",TRUE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","ADD ARCHPROG",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","ADD WORK GROUP",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","ADD WORK GROUP USERS",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","ARCHCLS-EXPORT",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","ARCHIVES-EXPORT",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","ARCHIVESIN",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","ARCHIVESOUT",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","ARCHPROG-EXPORT",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","ARCHSUB-EXPORT",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","ARCHTYPE-EXPORT",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","ARCHUSER-EXPORT",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","ARCHWORK-EXPORT",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","AUTOWRKGRP",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","AUTOWRKGRPDEPT",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","COPYARCHIVE",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","COPYGROUPUSER",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","DOARCHIVE",TRUE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","EMAILARCHIVE",TRUE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","EMAILARCHIVE2",TRUE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","MANAGE SCANS",TRUE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","MTN ARCHCLS",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","MTN ARCHIVES",TRUE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","MTN ARCHIVES.BROWSE",TRUE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","MTN ARCHIVES.SEARCH",TRUE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","MTN ARCHPROG",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","MTN ARCHSUB",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","MTN ARCHTYPE",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","MTN ARCHUSER",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","MTN ARCHUSER.SEARCH",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","MTN ARCHWORK",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","MTN ARCHWORK.SEARCH",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","PURGEARCHIVE",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","RPTARCHIVE",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","WORK GROUPS",FALSE);
			AddProcessSecurity("data-entry-hrpo","ARCHIVE","WORKGROUPUSER",FALSE);
		}
		if(XPERT_SETUP->FIXASS==TRUE) 
		{
			AddModuleSecurity("admin","FIXASS",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("acctmgr","FIXASS",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry","FIXASS",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-ar","FIXASS",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-po","FIXASS",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-hrpo","FIXASS",FALSE,TRUE,FALSE,TRUE,TRUE,TRUE,FALSE);
		}
		if(XPERT_SETUP->CSHDWR==TRUE) 
		{
			AddModuleSecurity("admin","CSHDWR",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
			AddModuleSecurity("admin","OWNERS",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);

			AddModuleSecurity("data-entry","CSHDWR",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry","OWNERS",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-ar","CSHDWR",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-ar","OWNERS",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddProcessSecurity("data-entry","CSHDWR","ADD AT COUNTER",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","ADD RECEIPT",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","ADD RECEIPTS BY COMMON OWNER",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","AINTOWN",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","AINTOWNPE",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","APPLYPAYMENT",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","APPLYRECEIPT",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","APPLYRECEIPT1",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","APPLYRECOWN",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","APPLYRECSVC",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","BLDAPPLYPAYNEW",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","BLDAPPLYPAYTRN",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","BLDAPPLYREC",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","BLDPRMT ADD PERMITS WITH PAYMENTS",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","BLDPRMT ADD RECEIPTS",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","BLDPRMT ADD RECEIPTS BY TRANSACTION",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","CASH DRAWER MENU",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","CASH DRAWER REPORT SUBMENU",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","CSHAUDIT",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","CSHAUDIT-ALL",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","CSHAUDIT-RANGE",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","CSHDWR CSHAUDIT",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","CSHDWR CSHTRNBYTYPE",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","CSHDWR DOREPORT",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","CSHDWR EZRPT",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","CSHDWR PRECASHOUT",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","CSHDWR RPT CSHAUDIT-ALL",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","CSHDWR RPT CSHAUDIT-RANGE",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","CSHDWR RPT CSHRECDET",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","CSHDWR USER REPORT SUBMENU",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","CSHRECDET",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","CSHTRN3",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","CSHTRNBYTYPE",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","DMVTICKETSINGLE",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","FINMGT ADD RECEIPTS",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","MISCAPPLYPAY",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","MISCAPPLYPAYOWN",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","MISCBILL ADD RECEIPTS",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","MISCBILL ADD RECEIPTS BY OWNER",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","MISCBILL ADD RECEIPTS BY TRANS",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","MTN CSHDET",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","MTN CSHDET.MAINTAIN",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","MTN CSHDET.SEARCH",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","MTN CSHID",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","MTN CSHID.MAINTAIN",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","MTN CSHOWN",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","MTN CSHTRN",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","MTN CSHTRN.MAINTAIN",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","OCCADDREC",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","OCCADDREC1",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","OCCTAX ADD RECEIPTS",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","OWNERPAYMENT",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","PRECASHOUT",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","PRINTRECEIPT",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","PRINTRECEIPTB",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","PRINTRECEIPTC",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","PRINTRECEIPTDUP",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","PRINTVAL",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","PRINTVALPAYSTUB",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","PROPERTY ADD RECEIPTS",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","PROPERTY ADD RECEIPTS BY CLASS",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","PROPERTY ADD RECEIPTS BY CUSTOMER",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","PRPTICKETSINGLC",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","PRPTICKETSINGLE",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","RCPTSUM2",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","RECEIPT85",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","RLSTMGT ADD RECEIPTS",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","RLSTMGT ADD RECEIPTS BY PARCEL",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","RLSTSGLETCKET",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","RLSTSINGLETCKET",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","RPT CSHTRN3",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","RPT RCPTSUM2",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","RPT SRCDET",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","RPT SRCSUM2",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","SMALLRECEIPT",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","SRCDET",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","SRCSUM2",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","SYNCDEPOSITID",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","TAGTICKETSINGLE",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","UTLBLL ADD DEPOSITS",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","UTLBLL ADD RECEIPTS",TRUE);
			AddProcessSecurity("data-entry","CSHDWR","UTLBLL ADD RECEIPTS BY CHARGE NUMBER",TRUE);

			AddProcessSecurity("data-entry-ar","CSHDWR","ADD AT COUNTER",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","ADD RECEIPT",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","ADD RECEIPTS BY COMMON OWNER",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","AINTOWN",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","AINTOWNPE",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","APPLYPAYMENT",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","APPLYRECEIPT",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","APPLYRECEIPT1",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","APPLYRECOWN",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","APPLYRECSVC",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","BLDAPPLYPAYNEW",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","BLDAPPLYPAYTRN",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","BLDAPPLYREC",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","BLDPRMT ADD PERMITS WITH PAYMENTS",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","BLDPRMT ADD RECEIPTS",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","BLDPRMT ADD RECEIPTS BY TRANSACTION",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","CASH DRAWER MENU",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","CASH DRAWER REPORT SUBMENU",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","CSHAUDIT",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","CSHAUDIT-ALL",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","CSHAUDIT-RANGE",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","CSHDWR CSHAUDIT",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","CSHDWR CSHTRNBYTYPE",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","CSHDWR DOREPORT",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","CSHDWR EZRPT",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","CSHDWR PRECASHOUT",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","CSHDWR RPT CSHAUDIT-ALL",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","CSHDWR RPT CSHAUDIT-RANGE",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","CSHDWR RPT CSHRECDET",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","CSHDWR USER REPORT SUBMENU",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","CSHRECDET",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","CSHTRN3",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","CSHTRNBYTYPE",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","DMVTICKETSINGLE",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","FINMGT ADD RECEIPTS",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","MISCAPPLYPAY",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","MISCAPPLYPAYOWN",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","MISCBILL ADD RECEIPTS",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","MISCBILL ADD RECEIPTS BY OWNER",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","MISCBILL ADD RECEIPTS BY TRANS",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","MTN CSHDET",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","MTN CSHDET.MAINTAIN",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","MTN CSHDET.SEARCH",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","MTN CSHID",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","MTN CSHID.MAINTAIN",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","MTN CSHOWN",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","MTN CSHTRN",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","MTN CSHTRN.MAINTAIN",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","OCCADDREC",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","OCCADDREC1",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","OCCTAX ADD RECEIPTS",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","OWNERPAYMENT",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","PRECASHOUT",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","PRINTRECEIPT",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","PRINTRECEIPTB",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","PRINTRECEIPTC",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","PRINTRECEIPTDUP",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","PRINTVAL",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","PRINTVALPAYSTUB",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","PROPERTY ADD RECEIPTS",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","PROPERTY ADD RECEIPTS BY CLASS",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","PROPERTY ADD RECEIPTS BY CUSTOMER",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","PRPTICKETSINGLC",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","PRPTICKETSINGLE",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","RCPTSUM2",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","RECEIPT85",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","RLSTMGT ADD RECEIPTS",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","RLSTMGT ADD RECEIPTS BY PARCEL",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","RLSTSGLETCKET",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","RLSTSINGLETCKET",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","RPT CSHTRN3",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","RPT RCPTSUM2",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","RPT SRCDET",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","RPT SRCSUM2",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","SMALLRECEIPT",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","SRCDET",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","SRCSUM2",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","SYNCDEPOSITID",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","TAGTICKETSINGLE",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","UTLBLL ADD DEPOSITS",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","UTLBLL ADD RECEIPTS",TRUE);
			AddProcessSecurity("data-entry-ar","CSHDWR","UTLBLL ADD RECEIPTS BY CHARGE NUMBER",TRUE);
		}
		if(XPERT_SETUP->DMVREG==TRUE) 
		{
#ifdef _NOT_YET_
			AddModuleSecurity("data-entry","DMVREG",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
#endif /* _NOT_YET_ */
		}
		if(XPERT_SETUP->BLDPRMT==TRUE) 
		{
			AddModuleSecurity("data-entry","BLDPRMT",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-ar","BLDPRMT",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
		}
		if(XPERT_SETUP->MISCBILL==TRUE) 
		{
			AddModuleSecurity("data-entry","MISCBILL",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-ar","MISCBILL",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
		}
		if(XPERT_SETUP->PROPERTY==TRUE) 
		{
			AddModuleSecurity("data-entry","PROPERTY",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-ar","PROPERTY",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
		}
		if(XPERT_SETUP->RLSTMGT==TRUE) 
		{
			AddModuleSecurity("data-entry","RLSTMGT",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-ar","RLSTMGT",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
		}
		if(XPERT_SETUP->UTLBLL==TRUE) 
		{
			AddModuleSecurity("data-entry","UTLBLL",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-ar","UTLBLL",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
		}
		if(XPERT_SETUP->OCCTAX==TRUE) 
		{
			AddModuleSecurity("data-entry","OCCTAX",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-ar","OCCTAX",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
		}
		if((!RDAstrcmp(XPERT_SETUP->state,"VA")) && XPERT_SETUP->OrgType==2)
		{
			AddModuleSecurity("data-entry","VATAX",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry","VAIRMS",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);

			AddModuleSecurity("data-entry-ar","VATAX",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE);
			AddModuleSecurity("data-entry-ar","VAIRMS",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		}
	}	
}
void MakeRDACronUser()
{
	short U=0,style=0,bt=1;
	RDATData *prev=NULL;
	int ret_int=0;

	if(XPERT_SETUP->software_type>1) return;
	U=OPNNRD("SECURITY","USERS");
	if(U!=(-1))
	{
		ZERNRD(U);
		FINDFLDSETSTRING(U,"USER IDENTIFICATION","rdacron");
		if(EQLNRD(U,1)) KEYNRD(U,1);
			else prev=RDATDataNEW(U);
		FINDFLDSETSTRING(U,"USER NAME","RDA Cron User");
		FINDFLDSETSHORT(U,"DOMENU STYLE",style);
		FINDFLDSETCHAR(U,"USE BROWSE LIST LABEL",TRUE);
		FINDFLDSETSTRING(U,"DEPARTMENT","OPENRDA");
		FINDFLDSETSTRING(U,"MODULE NAME","FINMGT");
		FINDFLDSETSTRING(U,"EMAIL","support@rdasys.com");
		FINDFLDSETSTRING(U,"WEB LOGIN","RDACRON");
		FINDFLDSETSTRING(U,"WEB PASSWORD","0penRD@12");
		FINDFLDSETSTRING(U,"PHONE NUMBER","800-338-4984");
#ifndef WIN32
		FINDFLDSETCHAR(U,"BROWSER TYPE",bt);
		FINDFLDSETSTRING(U,"DEFAULT PRINTER","AUTOARCHIVE");
#endif
#ifdef WIN32
		bt=2;
		FINDFLDSETCHAR(U,"BROWSER TYPE",bt);
		FINDFLDSETSTRING(U,"DEFAULT PRINTER","AUTOARCHIVE");
#endif
		FINDFLDSETCHAR(U,"DELETEFLAG",FALSE);
		WRTTRANS(U,0,NULL,prev);
		if(prev!=NULL) FreeRDATData(prev);
		CLSNRD(U);
	}
		
	if(XPERT_SETUP->software_type>1) return;
	U=OPNNRD("SECURITY","USERS");
	if(U!=(-1))
	{
		ZERNRD(U);
		FINDFLDSETSTRING(U,"USER IDENTIFICATION","OPENRDA");
		if(EQLNRD(U,1)) 
		{ 
			KEYNRD(U,1);
		}else{
			prev=RDATDataNEW(U);
		}
		FINDFLDSETSTRING(U,"USER NAME","profile OpenRDA Services");
		FINDFLDSETSHORT(U,"DOMENU STYLE",style);
		FINDFLDSETCHAR(U,"USE BROWSE LIST LABEL",TRUE);
		FINDFLDSETSTRING(U,"DEPARTMENT","OPENRDA");
		FINDFLDSETSTRING(U,"MODULE NAME","FINMGT");
/*
		FINDFLDSETSTRING(U,"EMAIL","support@rdasys.com");
		FINDFLDSETSTRING(U,"PHONE NUMBER","800-338-4984");
*/
#ifndef WIN32
		FINDFLDSETCHAR(U,"BROWSER TYPE",bt);
		FINDFLDSETSTRING(U,"DEFAULT PRINTER","viewpdf.lnx");
#endif
#ifdef WIN32
		bt=2;
		FINDFLDSETCHAR(U,"BROWSER TYPE",bt);
		FINDFLDSETSTRING(U,"DEFAULT PRINTER","viewpdf.exe");
#endif
		FINDFLDSETCHAR(U,"DELETEFLAG",FALSE);
		WRTTRANS(U,0,NULL,prev);
		if(prev!=NULL) FreeRDATData(prev);
		CLSNRD(U);
	}

	if(MODSEC_FILENO==(-1)) MODSEC_FILENO=OPNNRD("SECURITY","MODSEC");
	if(PROCSEC_FILENO==(-1)) PROCSEC_FILENO=OPNNRD("SECURITY","PROCSEC");
	if(FILESEC_FILENO==(-1)) FILESEC_FILENO=OPNNRD("SECURITY","FILESEC");
	if(MODSEC_FILENO!=(-1))
	{
		RDAmkdir("rdacron",00770,&ret_int);
		AddModuleSecurity("rdacron","DATABASE",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		AddModuleSecurity("rdacron","GUI",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		AddModuleSecurity("rdacron","OLHELP",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		AddModuleSecurity("rdacron","RPTGEN",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		AddModuleSecurity("rdacron","SECURITY",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		AddModuleSecurity("rdacron","UTILITIES",FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		
		RDAmkdir("OPENRDA",00770,&ret_int);
		if(XPERT_SETUP->FINMGT==TRUE) AddModuleSecurity("OPENRDA","FINMGT",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->VENPMT==TRUE) AddModuleSecurity("OPENRDA","VENPMT",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->VENPMT==TRUE) AddModuleSecurity("OPENRDA","VENSAFE",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->PURORD==TRUE) AddModuleSecurity("OPENRDA","PURORD",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->BNKREC==TRUE) AddModuleSecurity("OPENRDA","BNKREC",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->FIXASS==TRUE) AddModuleSecurity("OPENRDA","FIXASS",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->PRSNNL==TRUE) AddModuleSecurity("OPENRDA","PRSNNL",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->PAYROLL==TRUE) AddModuleSecurity("OPENRDA","PAYROLL",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->POSTRK==TRUE) AddModuleSecurity("OPENRDA","POSTRK",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->BFTMGT==TRUE) AddModuleSecurity("OPENRDA","BFTMGT",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->APPMGT==TRUE) AddModuleSecurity("OPENRDA","APPMGT",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->EMPABS==TRUE) AddModuleSecurity("OPENRDA","EMPABS",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->LVEMGT==TRUE) AddModuleSecurity("OPENRDA","LVEMGT",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->SUBMGT==TRUE) AddModuleSecurity("OPENRDA","SUBMGT",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->LVSIMP==TRUE) AddModuleSecurity("OPENRDA","LVSIMP",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->INVCTL==TRUE) AddModuleSecurity("OPENRDA","INVCTL",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->RLVEMGT==TRUE) AddModuleSecurity("OPENRDA","RLVEMGT",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->REQMGT==TRUE) AddModuleSecurity("OPENRDA","REQMGT",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->RLSTMGT==TRUE) AddModuleSecurity("OPENRDA","RLSTMGT",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->CSHDWR==TRUE) AddModuleSecurity("OPENRDA","CSHDWR",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->CSHDWR==TRUE) AddModuleSecurity("OPENRDA","OWNERS",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->DMVREG==TRUE) AddModuleSecurity("OPENRDA","DMVREG",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->PROPERTY==TRUE) AddModuleSecurity("OPENRDA","PROPERTY",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if((XPERT_SETUP->PROPERTY==TRUE) && (!RDAstrcmp(XPERT_SETUP->state,"VA"))) AddModuleSecurity("OPENRDA","VAIRMS",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if((XPERT_SETUP->RLSTMGT==TRUE) && (!RDAstrcmp(XPERT_SETUP->state,"VA"))) AddModuleSecurity("OPENRDA","VAIRMS",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->MISCBILL==TRUE) AddModuleSecurity("OPENRDA","MISCBILL",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->CATALOGUE==TRUE) AddModuleSecurity("OPENRDA","CATALOGUE",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->BLDPRMT==TRUE) AddModuleSecurity("OPENRDA","BLDPRMT",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->ARCHIVE==TRUE) AddModuleSecurity("OPENRDA","ARCHIVE",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->BUDPREP==TRUE) AddModuleSecurity("OPENRDA","BUDPREP",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->UTLBLL==TRUE) AddModuleSecurity("OPENRDA","UTLBLL",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->OCCTAX==TRUE) AddModuleSecurity("OPENRDA","OCCTAX",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		if(XPERT_SETUP->FOODSVC==TRUE) AddModuleSecurity("OPENRDA","FOODSVC",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		AddModuleSecurity("OPENRDA","CONVERSION",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		AddModuleSecurity("OPENRDA","DATABASE",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		AddModuleSecurity("OPENRDA","GUI",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		AddModuleSecurity("OPENRDA","OLHELP",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		AddModuleSecurity("OPENRDA","RPTGEN",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		AddModuleSecurity("OPENRDA","SECURITY",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		AddModuleSecurity("OPENRDA","TRANS",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		AddModuleSecurity("OPENRDA","TOOLS",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
		AddModuleSecurity("OPENRDA","UTILITIES",TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE);
	}
	
	if(XPERT_SETUP->software_type>1) return;
	U=OPNNRD("SECURITY","DEPARTMENT");
	if(U!=(-1))
	{
		ZERNRD(U);
		FINDFLDSETSTRING(U,"DEPARTMENT","OPENRDA");
		if(EQLNRD(U,1)) 
		{
			KEYNRD(U,1);
			FINDFLDSETSTRING(U,"SUPERVISOR","rdacron");
		}else{
			prev=RDATDataNEW(U);
		}

		FINDFLDSETSTRING(U,"COMMENTS","DEPARTMENT FOR RDA STAFF PROVIDING SERVICES");
		FINDFLDSETCHAR(U,"SECURITY",TRUE);
		FINDFLDSETCHAR(U,"DELETEFLAG",FALSE);
		WRTTRANS(U,0,NULL,prev);
		if(prev!=NULL) FreeRDATData(prev);
		CLSNRD(U);
	}

}
