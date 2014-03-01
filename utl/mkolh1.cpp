/* OLHELP's - Make (SCNNOT) List */
#include <cstdio>
#include <mix.hpp>
#include <mknrd.hpp>

void SCNNOT()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *tempstr=NULL;

	sort=DBsortNEW("OLHELP","SCNNOT",0);
	if(sort!=NULL)
	{
		sort->engine=0;
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"MODULE NAME",0,0);
		addDBfield(sort,"SCREEN NAME",0,0);
		addDBfield(sort,"NOTES",14,0);
		addDBkey(sort,"SCNNOT KEY");
		addDBkeypart(sort,1,"MODULE NAME");
		addDBkeypart(sort,1,"SCREEN NAME");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				tempstr=Rmalloc(171+sizeof(sort->module)+sizeof(sort->filename));
				sprintf(tempstr,"The Database File Changes may not be saved to the specified library: [%s][%s].  Check your permissions and re-try.  Call your installer for further instructions.",sort->module,sort->filename)
				;
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
void USCNNOT()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *tempstr=NULL;

	sort=DBsortNEW("OLHELP","USCNNOT",0);
	if(sort!=NULL)
	{
		sort->engine=0;
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"MODULE NAME",0,0);
		addDBfield(sort,"USER IDENTIFICATION",0,0);
		addDBfield(sort,"SCREEN NAME",0,0);
		addDBfield(sort,"NOTES",14,0);
		addDBkey(sort,"USCNNOT KEY");
		addDBkeypart(sort,1,"MODULE NAME");
		addDBkeypart(sort,1,"SCREEN NAME");
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
				sprintf(tempstr,"The Database File Changes may not be saved to the specified library: [%s][%s].  Check your permissions and re-try.  Call your installer for further instructions.",sort->module,sort->filename)
				;
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
void RSCNOT()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *tempstr=NULL;

	sort=DBsortNEW("OLHELP","RSCNOT",0);
	if(sort!=NULL)
	{
		sort->engine=0;
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"MODULE NAME",0,0);
		addDBfield(sort,"SCREEN NAME",0,0);
		addDBfield(sort,"RESOURCE NAME",0,0);
		addDBfield(sort,"NOTES",14,0);
		addDBkey(sort,"RSCNOT KEY");
		addDBkeypart(sort,1,"MODULE NAME");
		addDBkeypart(sort,1,"SCREEN NAME");
		addDBkeypart(sort,1,"RESOURCE NAME");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				tempstr=Rmalloc(171+sizeof(sort->module)+sizeof(sort->filename));
				sprintf(tempstr,"The Database File Changes may not be saved to the specified library: [%s][%s].  Check your permissions and re-try.  Call your installer for further instructions.",sort->module,sort->filename)
				;
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
void URSCNOT()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *tempstr=NULL;

	sort=DBsortNEW("OLHELP","URSCNOT",0);
	if(sort!=NULL)
	{
		sort->engine=0;
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"MODULE NAME",0,0);
		addDBfield(sort,"USER IDENTIFICATION",0,0);
		addDBfield(sort,"SCREEN NAME",0,0);
		addDBfield(sort,"RESOURCE NAME",0,0);
		addDBfield(sort,"NOTES",14,0);
		addDBkey(sort,"URSCNOT KEY");
		addDBkeypart(sort,1,"MODULE NAME");
		addDBkeypart(sort,1,"SCREEN NAME");
		addDBkeypart(sort,1,"RESOURCE NAME");
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
				sprintf(tempstr,"The Database File Changes may not be saved to the specified library: [%s][%s].  Check your permissions and re-try.  Call your installer for further instructions.",sort->module,sort->filename)
				;
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
