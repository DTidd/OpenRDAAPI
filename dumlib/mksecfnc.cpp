/* mksecfnc.c - Makes Security File functions */
/*lint -library */
#include <mksecs.hpp>

void DB_TOKENS()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	if(SEC_TOKEN_FILENO!=(-1))
	{
		CLSNRD(SEC_TOKEN_FILENO);
		SEC_TOKEN_FILENO=(-1);
	}
	sort=DBsortNEW("SECURITY","TOKENS",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"TOKEN",1,25);
		addDBfield(sort,"USER IDENTIFICATION",1,15);
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBkey(sort,"TOKEN KEY 1");
		addDBkeypart(sort,1,"TOKEN");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(6+8+77+1);
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [SECURITY] Screen [TOKENS], Can Not Save Database File!");
				prterr(temp1);
				if(temp1!=NULL) Rfree(temp1);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
	if(SEC_TOKEN_FILENO==(-1))
	{
		SEC_TOKEN_FILENO=OPNNRD("SECURITY","TOKENS");	
	}
}
void USERS()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *tempstr=NULL;

	if(SEC_USERS_FILENO!=(-1))
	{
		CLSNRD(SEC_USERS_FILENO);
		SEC_USERS_FILENO=(-1);
	}
	sort=DBsortNEW("SECURITY","USERS",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"USER IDENTIFICATION",1,15);
		addDBfield(sort,"PHONE NUMBER",3,12);
		addDBfield(sort,"PHONE EXTENSION",1,10);
		addDBfield(sort,"HELP INCLUSION TYPE",10,1);
		addDBfield(sort,"SITE IDENTIFICATION",1,15);
		addDBfield(sort,"USER NAME",1,40);
		addDBfield(sort,"DEFAULT PRINTER",1,40);
		addDBfield(sort,"DEFAULT FILE OUTPUT DEVICE",1,40);
		addDBfield(sort,"DEPARTMENT",1,40);
		addDBfield(sort,"LOCATION IDENTIFICATION",1,15);
		addDBfield(sort,"HOST",1,60);
		addDBfield(sort,"EMAIL",1,60);
		addDBfield(sort,"WEB LOGIN",1,15);
		addDBfield(sort,"WEB PASSWORD",1,15);
		addDBfield(sort,"BUDGET OVERRIDE",10,1);
		addDBfield(sort,"SUPERVISOR",10,1);
		addDBfield(sort,"EDIT TEXT BROWSER",10,1);
		addDBfield(sort,"USE BROWSE LIST LABEL",10,1);
		addDBfield(sort,"ALLOW POST ACTIVITY",10,1);
		addDBfield(sort,"ALLOW BEGINNING BALANCES",10,1);
		if(XPERT_SETUP->software_type<2)
		{
		addDBfield(sort,"ABLE TO APPROVE",10,1);
		addDBfield(sort,"APPROVAL LIMIT",2,15);
		addDBfield(sort,"CELL PHONE",3,12);
		addDBfield(sort,"CARRIER",1,40);
		addDBfield(sort,"ALLOW MULTIPLE ENTRIES",10,1);
		addDBfield(sort,"CARRIER TYPE",7,1);
		addDBfield(sort,"TEXT MESSAGE",10,1);
		addDBfield(sort,"PREFER TEXT MESSAGE",10,1);
		addDBfield(sort,"JOURNAL ENTRY APPROVAL",10,1);
		addDBfield(sort,"VOUCHER APPROVAL",10,1);
		addDBfield(sort,"ADMIN EDITS",10,1);
		addDBfield(sort,"SUBMIT REQUISITIONS",10,1);
		addDBfield(sort,"APPROVE REQUISITIONS",10,1);
		addDBfield(sort,"AUTO COMPLETE",10,1);
		addDBfield(sort,"DISENGAGE WORK FLOW NAVIGATION",10,1);
		addDBfield(sort,"IGNORE RESTRICTED PROCESS CONSTRAINTS",10,1);
		addDBfield(sort,"WORKSTATION VALIDATED",10,1);
		addDBfield(sort,"WORKSTATION LOGIN",1,40);
		addDBfield(sort,"WORKSTATION PASSWORD",1,25);
		addDBfield(sort,"WORKSTATION DOCUMENTS",1,120);
		addDBfield(sort,"DISPLAY PURGE FUNCTIONS",10,1);
		}
		addDBfield(sort,"DOMENU STYLE",SHORTV,2);
		addDBfield(sort,"SMTP SERVER",PLAINTEXT,40);
		addDBfield(sort,"SMTP PORT",LONGV,5);
		addDBfield(sort,"SMTP AUTH",PLAINTEXT,15);
		addDBfield(sort,"CONNECT TIMEOUT",SHORTV,3);
		addDBfield(sort,"PROTOCOL",PLAINTEXT,60);
		addDBfield(sort,"USERNAME",PLAINTEXT,50);
		addDBfield(sort,"PASSWORD",PLAINTEXT,60);
		addDBfield(sort,"NTLMDOMAIN",PLAINTEXT,60);
		addDBfield(sort,"TLS",10,1);
		addDBfield(sort,"TLS START",10,1);
		addDBfield(sort,"TLS TRUST FILE",PLAINTEXT,60);
		addDBfield(sort,"TLS KEY FILE",PLAINTEXT,60);
		addDBfield(sort,"TLS CRT FILE",PLAINTEXT,60);
		addDBfield(sort,"TLS CERT CHECK",10,1);
		addDBfield(sort,"JOB DESCRIPTION",14,1024);
		addDBfield(sort,"SCAN DIRECTORY",1,40);
		addDBfield(sort,"VENPMT SIGNATURE FILENAME",1,80);
		addDBfield(sort,"VENPMT SIGNATURE PASSWORD",1,15);
		addDBfield(sort,"PAYROLL SIGNATURE FILENAME",1,80);
		addDBfield(sort,"PAYROLL SIGNATURE PASSWORD",1,15);
		addDBfield(sort,"PURORD SIGNATURE FILENAME",1,80);
		addDBfield(sort,"PURORD SIGNATURE PASSWORD",1,15);
		addDBfield(sort,"PUSHBUTTON STYLE",SHORTV,2);
		addDBfield(sort,"MODULE NAME",1,15);
		addDBfield(sort,"WT PASSWORD",1,60);
		addDBfield(sort,"WT PASSWORD DATE",DATES,10);
		addDBfield(sort,"WT PASSWORD METHOD",1,40);
		addDBfield(sort,"WT FORCE PASSWORD CHANGE",10,1);
		addDBfield(sort,"WT FAILED LOGIN ATTEMPTS",LONGV,4);
		addDBfield(sort,"WT LAST LOGIN ATTEMPT DATE",DATES,10);
		addDBfield(sort,"WT LAST LOGIN ATTEMPT TIME",TIMES,8);

		addDBkey(sort,"USERS KEY");
		addDBkeypart(sort,1,"USER IDENTIFICATION");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				tempstr=Rmalloc(171+sizeof(sort->module)+sizeof(sort->filename));
				sprintf(tempstr,"The Database File Changes may not be saved to the specified library: [%s][%s].  Check your permissions and re-try.  Call your installer for further instructions.",sort->module,sort->filename);
				prterr("ERROR: Cannot Save Database File, [%s]",tempstr);
				ERRORDIALOG("Cannot Save Database File",tempstr,NULL,FALSE);
				if(tempstr!=NULL) Rfree(tempstr);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
	if(SEC_USERS_FILENO==(-1))
	{
		SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");	
	}
}
void MODSEC()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *tempstr=NULL;

	if(MODSEC_FILENO!=(-1)) CLSNRD(MODSEC_FILENO);
	MODSEC_FILENO=(-1);
	sort=DBsortNEW("SECURITY","MODSEC",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"USER IDENTIFICATION",1,15);
		addDBfield(sort,"MODULE NAME",1,15);
		addDBfield(sort,"ALL PROCESSES",10,1);
		addDBfield(sort,"ALL FILES",10,1);
		addDBfield(sort,"EXECUTE FUNCTIONS",10,1);
		addDBfield(sort,"READ DATA",10,1);
		addDBfield(sort,"WRITE DATA",10,1);
		addDBfield(sort,"PRINT DATA",10,1);
		addDBfield(sort,"DELETE DATA",10,1);
		addDBkey(sort,"MODSEC KEY");
		addDBkeypart(sort,1,"USER IDENTIFICATION");
		addDBkeypart(sort,1,"MODULE NAME");
		addDBkey(sort,"RDA MODSEC KEY");
		addDBkeypart(sort,2,"MODULE NAME");
		addDBkeypart(sort,2,"USER IDENTIFICATION");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				tempstr=Rmalloc(171+sizeof(sort->module)+sizeof(sort->filename));
				sprintf(tempstr,"The Database File Changes may not be saved to the specified library: [%s][%s].  Check your permissions and re-try.  Call your installer for further instructions.",sort->module,sort->filename);
				prterr("ERROR: Cannot Save Database File, [%s]",tempstr);
				ERRORDIALOG("Cannot Save Database File",tempstr,NULL,FALSE);
				if(tempstr!=NULL) Rfree(tempstr);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
	if(MODSEC_FILENO==(-1)) MODSEC_FILENO=OPNNRD("SECURITY","MODSEC");
}
void PROCSEC()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *tempstr=NULL;

	if(PROCSEC_FILENO!=(-1)) CLSNRD(PROCSEC_FILENO);
	PROCSEC_FILENO=(-1);
	sort=DBsortNEW("SECURITY","PROCSEC",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"EXECUTE PROCESS",10,1);
		addDBfield(sort,"USER IDENTIFICATION",1,15);
		addDBfield(sort,"MODULE NAME",1,15);
		addDBfield(sort,"PROCESS NAME",1,60);
		addDBkey(sort,"PROCSEC KEY");
		addDBkeypart(sort,1,"USER IDENTIFICATION");
		addDBkeypart(sort,1,"MODULE NAME");
		addDBkeypart(sort,1,"PROCESS NAME");
		addDBkey(sort,"RDA PROCSEC KEY");
		addDBkeypart(sort,2,"MODULE NAME");
		addDBkeypart(sort,2,"PROCESS NAME");
		addDBkeypart(sort,2,"USER IDENTIFICATION");
		addDBkey(sort,"RDA PROCSEC KEY 2");
		addDBkeypart(sort,3,"PROCESS NAME");
		addDBkeypart(sort,3,"MODULE NAME");
		addDBkeypart(sort,3,"USER IDENTIFICATION");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				tempstr=Rmalloc(171+sizeof(sort->module)+sizeof(sort->filename));
				sprintf(tempstr,"The Database File Changes may not be saved to the specified library: [%s][%s].  Check your permissions and re-try.  Call your installer for further instructions.",sort->module,sort->filename);
				prterr("ERROR: Cannot Save Database File, [%s]",tempstr);
				ERRORDIALOG("Cannot Save Database File",tempstr,NULL,FALSE);
				if(tempstr!=NULL) Rfree(tempstr);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
	if(PROCSEC_FILENO==(-1)) PROCSEC_FILENO=OPNNRD("SECURITY","PROCSEC");
}
void FILESEC()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *tempstr=NULL;

	if(FILESEC_FILENO!=(-1)) CLSNRD(FILESEC_FILENO);
	FILESEC_FILENO=(-1);
	sort=DBsortNEW("SECURITY","FILESEC",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"USER IDENTIFICATION",1,15);
		addDBfield(sort,"MODULE NAME",1,15);
		addDBfield(sort,"FILE NAME",1,15);
		addDBfield(sort,"FIELD SECURITY",10,1);
		addDBfield(sort,"READ FILE",10,1);
		addDBfield(sort,"WRITE FILE",10,1);
		addDBfield(sort,"PRINT FILE",10,1);
		addDBfield(sort,"DELETE FILE",10,1);
		addDBfield(sort,"EXPRESSION",14,2700);
		addDBkey(sort,"FILESEC KEY");
		addDBkeypart(sort,1,"USER IDENTIFICATION");
		addDBkeypart(sort,1,"MODULE NAME");
		addDBkeypart(sort,1,"FILE NAME");
		addDBkey(sort,"RDA FILESEC KEY");
		addDBkeypart(sort,2,"MODULE NAME");
		addDBkeypart(sort,2,"FILE NAME");
		addDBkeypart(sort,2,"USER IDENTIFICATION");
		addDBkey(sort,"RDA FILESEC KEY 2");
		addDBkeypart(sort,3,"FILE NAME");
		addDBkeypart(sort,3,"MODULE NAME");
		addDBkeypart(sort,3,"USER IDENTIFICATION");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				tempstr=Rmalloc(171+sizeof(sort->module)+sizeof(sort->filename));
				sprintf(tempstr,"The Database File Changes may not be saved to the specified library: [%s][%s].  Check your permissions and re-try.  Call your installer for further instructions.",sort->module,sort->filename);
				prterr("ERROR: Cannot Save Database File, [%s]",tempstr);
				ERRORDIALOG("Cannot Save Database File",tempstr,NULL,FALSE);
				if(tempstr!=NULL) Rfree(tempstr);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
	if(FILESEC_FILENO==(-1)) FILESEC_FILENO=OPNNRD("SECURITY","FILESEC");
}
void FLDSEC()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *tempstr=NULL;

	if(FLDSEC_FILENO!=(-1)) CLSNRD(FLDSEC_FILENO);
	FLDSEC_FILENO=(-1);
	sort=DBsortNEW("SECURITY","FLDSEC",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"USER IDENTIFICATION",1,15);
		addDBfield(sort,"MODULE NAME",1,15);
		addDBfield(sort,"FILE NAME",1,15);
		addDBfield(sort,"FIELD NAME",1,60);
		addDBfield(sort,"READ FIELD",10,1);
		addDBfield(sort,"WRITE FIELD",10,1);
		addDBfield(sort,"PRINT FIELD",10,1);
		addDBkey(sort,"FLDSEC KEY");
		addDBkeypart(sort,1,"USER IDENTIFICATION");
		addDBkeypart(sort,1,"MODULE NAME");
		addDBkeypart(sort,1,"FILE NAME");
		addDBkeypart(sort,1,"FIELD NAME");
		addDBkey(sort,"RDA FLDSEC KEY");
		addDBkeypart(sort,2,"MODULE NAME");
		addDBkeypart(sort,2,"FILE NAME");
		addDBkeypart(sort,2,"FIELD NAME");
		addDBkeypart(sort,2,"USER IDENTIFICATION");
		addDBkey(sort,"RDA FLDSEC KEY 2");
		addDBkeypart(sort,3,"FILE NAME");
		addDBkeypart(sort,3,"MODULE NAME");
		addDBkeypart(sort,3,"FIELD NAME");
		addDBkeypart(sort,3,"USER IDENTIFICATION");
		addDBkey(sort,"RDA FLDSEC KEY 3");
		addDBkeypart(sort,4,"FIELD NAME");
		addDBkeypart(sort,4,"FILE NAME");
		addDBkeypart(sort,4,"MODULE NAME");
		addDBkeypart(sort,4,"USER IDENTIFICATION");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				tempstr=Rmalloc(171+sizeof(sort->module)+sizeof(sort->filename));
				sprintf(tempstr,"The Database File Changes may not be saved to the specified library: [%s][%s].  Check your permissions and re-try.  Call your installer for further instructions.",sort->module,sort->filename);
				prterr("ERROR: Cannot Save Database File, [%s]",tempstr);
				ERRORDIALOG("Cannot Save Database File",tempstr,NULL,FALSE);
				if(tempstr!=NULL) Rfree(tempstr);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
	if(FLDSEC_FILENO==(-1)) FLDSEC_FILENO=OPNNRD("SECURITY","FLDSEC");
}
void SECURITY_DEPARTMENT()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	sort=DBsortNEW("SECURITY","DEPARTMENT",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"DEPARTMENT",1,40);
		addDBfield(sort,"SUPERVISOR",1,15);
		addDBfield(sort,"COMMENTS",14,800);
		addDBfield(sort,"SECURITY",10,1);

		addDBkey(sort,"DEPARTMENT KEY");
		addDBkeypart(sort,1,"DEPARTMENT");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(10+8+77+1);
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [SECURITY] Screen [DEPARTMENT], Can Not Save Database File!");
				prterr(temp1);
				if(temp1!=NULL) Rfree(temp1);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
}
