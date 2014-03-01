/* SECURITY's - Make (USERS) List */
#include <cstdio>
#include <sec.hpp>
#include <mix.hpp>
#include <mknrd.hpp>
#ifdef XXXXXX
#include <nrdp1.h>
#endif

void USERS()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *tempstr=NULL;

	sort=DBsortNEW("SECURITY","USERS",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"PHONE NUMBER",3,12);
		addDBfield(sort,"PHONE EXTENSION",1,10);
		addDBfield(sort,"HELP INCLUSION TYPE",10,1);
		addDBfield(sort,"USER IDENTIFICATION",1,15);
		addDBfield(sort,"SITE IDENTIFICATION",1,15);
		addDBfield(sort,"USER NAME",1,40);
		addDBfield(sort,"DEFAULT PRINTER",1,40);
		addDBfield(sort,"DEFAULT FILE OUTPUT DEVICE",1,40);
		addDBfield(sort,"DEPARTMENT",1,40);
		addDBfield(sort,"JOB DESCRIPTION",14,1024);
		addDBfield(sort,"EMAIL",1,60);
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
}
void MODSEC()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *tempstr=NULL,*loginx=stralloc(USERLOGIN);;

	if(USERLOGIN!=NULL) Rfree(USERLOGIN);
	USERLOGIN=stralloc("root");
	if(MODSEC_FILENO!=(-1)) CLSNRD(MODSEC_FILENO);
	MODSEC_FILENO=(-1);
	sort=DBsortNEW("SECURITY","MODSEC",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"ALL PROCESSES",10,1);
		addDBfield(sort,"ALL FILES",10,1);
		addDBfield(sort,"EXECUTE FUNCTIONS",10,1);
		addDBfield(sort,"READ DATA",10,1);
		addDBfield(sort,"WRITE DATA",10,1);
		addDBfield(sort,"PRINT DATA",10,1);
		addDBfield(sort,"DELETE DATA",10,1);
		addDBfield(sort,"USER IDENTIFICATION",1,15);
		addDBfield(sort,"MODULE NAME",1,15);
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
	if(USERLOGIN!=NULL) Rfree(USERLOGIN);
	USERLOGIN=stralloc(loginx);
	if(loginx!=NULL) Rfree(loginx);
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
		addDBfield(sort,"PROCESS NAME",1,40);
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
		addDBfield(sort,"FIELD SECURITY",10,1);
		addDBfield(sort,"READ FILE",10,1);
		addDBfield(sort,"WRITE FILE",10,1);
		addDBfield(sort,"PRINT FILE",10,1);
		addDBfield(sort,"DELETE FILE",10,1);
		addDBfield(sort,"USER IDENTIFICATION",1,15);
		addDBfield(sort,"MODULE NAME",1,15);
		addDBfield(sort,"FILE NAME",1,15);
		addDBfield(sort,"EXPRESSION",14,1024);
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
		addDBfield(sort,"READ FIELD",10,1);
		addDBfield(sort,"WRITE FIELD",10,1);
		addDBfield(sort,"PRINT FIELD",10,1);
		addDBfield(sort,"USER IDENTIFICATION",1,15);
		addDBfield(sort,"MODULE NAME",1,15);
		addDBfield(sort,"FILE NAME",1,15);
		addDBfield(sort,"FIELD NAME",1,60);
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
