/* BFTMGT's - Make (BFTDDAC) Database File */
#include <cstdio>
#include <mix.hpp>
#include <mknrd.hpp>
#include <mkmsc.hpp>

extern APPlib *errorlist;

void DB_BFTDDAC()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	sort=DBsortNEW("BFTMGT","BFTDDAC",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"DEFINITION TYPE",7,1);
		addDBfield(sort,"PROCESSED BALANCE",2,20);
		addDBfield(sort,"UNPROCESSED BALANCE",2,20);
		addDBfield(sort,"DEDUCTION DESCRIPTION IDENTIFICATION",1,30);
		addDBkey(sort,"BFTDDAC KEY");
		addDBkeypart(sort,1,"DEDUCTION DESCRIPTION IDENTIFICATION");
		addDBkeypart(sort,1,"DEFINITION TYPE");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(7+6+77+1);
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [BFTMGT] Screen [BFTDDAC], Can Not Save Database File!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
}
void DB_BFTDESC()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	sort=DBsortNEW("BFTMGT","BFTDESC",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"PROCESSED BALANCE",2,20);
		addDBfield(sort,"UNPROCESSED BALANCE",2,20);
		addDBfield(sort,"DEDUCTION DESCRIPTION IDENTIFICATION",1,30);
		addDBfield(sort,"TITLE",1,60);
		addDBfield(sort,"NOTIFICATION FORMULA",14,500);
		addDBfield(sort,"DETAIL DESCRIPTION",14,500);
		addDBkey(sort,"BFTDESC KEY");
		addDBkeypart(sort,1,"DEDUCTION DESCRIPTION IDENTIFICATION");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(7+6+77+1);
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [BFTMGT] Screen [BFTDESC], Can Not Save Database File!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
}
void DB_BFTMDAC()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	sort=DBsortNEW("BFTMGT","BFTMDAC",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"DEFINITION TYPE",7,1);
		addDBfield(sort,"PROCESSED BALANCE",2,20);
		addDBfield(sort,"UNPROCESSED BALANCE",2,20);
		addDBfield(sort,"PERSONNEL IDENTIFICATION",1,15);
		addDBfield(sort,"DEDUCTION DESCRIPTION IDENTIFICATION",1,30);
		addDBkey(sort,"BFTMDAC KEY");
		addDBkeypart(sort,1,"PERSONNEL IDENTIFICATION");
		addDBkeypart(sort,1,"DEDUCTION DESCRIPTION IDENTIFICATION");
		addDBkeypart(sort,1,"DEFINITION TYPE");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(7+6+77+1);
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [BFTMGT] Screen [BFTMDAC], Can Not Save Database File!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
}
void DB_BFTMSTR()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	sort=DBsortNEW("BFTMGT","BFTMSTR",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"PROCESSED BALANCE",2,20);
		addDBfield(sort,"UNPROCESSED BALANCE",2,20);
		addDBfield(sort,"PERSONNEL IDENTIFICATION",1,15);
		addDBfield(sort,"DEDUCTION DESCRIPTION IDENTIFICATION",1,30);
		addDBkey(sort,"BFTMSTR KEY");
		addDBkeypart(sort,1,"PERSONNEL IDENTIFICATION");
		addDBkeypart(sort,1,"DEDUCTION DESCRIPTION IDENTIFICATION");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(7+6+77+1);
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [BFTMGT] Screen [BFTMSTR], Can Not Save Database File!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
}
void DB_BFTTRAN()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	sort=DBsortNEW("BFTMGT","BFTTRAN",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"DEFINITION TYPE",7,1);
		addDBfield(sort,"TRANSACTION NUMBER",8,6);
		addDBfield(sort,"ENTRY DATE",16,8);
		addDBfield(sort,"ENTRY TIME",17,8);
		addDBfield(sort,"AMOUNT",2,15);
		addDBfield(sort,"SOURCE USER VERIFIED",10,1);
		addDBfield(sort,"SOURCE USER",1,15);
		addDBfield(sort,"DEDUCTION DESCRIPTION IDENTIFICATION",1,30);
		addDBfield(sort,"DESCRIPTION",1,40);
		addDBfield(sort,"PERSONNEL IDENTIFICATION",1,15);
		addDBkey(sort,"BFTTRAN KEY");
		addDBkeypart(sort,1,"PERSONNEL IDENTIFICATION");
		addDBkeypart(sort,1,"DEDUCTION DESCRIPTION IDENTIFICATION");
		addDBkeypart(sort,1,"DEFINITION TYPE");
		addDBkeypart(sort,1,"TRANSACTION NUMBER");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(7+6+77+1);
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [BFTMGT] Screen [BFTTRAN], Can Not Save Database File!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
}
