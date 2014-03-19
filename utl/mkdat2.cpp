/* DATABASE's - Make (DATLOG) Database File */
#include <cstdio>
#include <mix.hpp>
#include <trans.hpp>
#include <mknrd.hpp>
#include <mkmsc.hpp>

extern APPlib *errorlist;

void DB_DATLOG()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	if(DATLOG_FILENO!=(-1)) CLSNRD(DATLOG_FILENO);
	sort=DBsortNEW("DATABASE","DATLOG",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"MODULE",1,15);
		addDBfield(sort,"USER IDENTIFICATION",1,15);
		addDBfield(sort,"LOG NUMBER",8,8);
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"END DATE",16,10);
		addDBfield(sort,"END TIME",17,8);
		addDBfield(sort,"HAS TRANSACTIONS",10,1);
		addDBfield(sort,"PROCESS NAME",1,120);
		addDBfield(sort,"START DATE",16,10);
		addDBfield(sort,"START TIME",17,8);
		addDBfield(sort,"UNDONE",10,1);
		addDBfield(sort,"UNDONE BY",1,15);
		addDBfield(sort,"UNDONE DATE",16,10);
		addDBfield(sort,"UNDONE TIME",17,8);
		addDBkey(sort,"DATLOG KEY 1");
		addDBkeypart(sort,1,"MODULE");
		addDBkeypart(sort,1,"USER IDENTIFICATION");
		addDBkeypart(sort,1,"LOG NUMBER");
		addDBkey(sort,"DATLOG KEY 2");
		addDBkeypart(sort,2,"USER IDENTIFICATION");
		addDBkeypart(sort,2,"MODULE");
		addDBkeypart(sort,2,"LOG NUMBER");

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
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [DATABASE] Screen [DATLOG], Can Not Save Database File!");
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
	DATLOG_FILENO=OPNNRD("DATABASE","DATLOG");
}
void DB_DATTRN()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	if(DATTRN_FILENO!=(-1)) CLSNRD(DATTRN_FILENO);
	sort=DBsortNEW("DATABASE","DATTRN",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"MODULE",1,15);
		addDBfield(sort,"USER IDENTIFICATION",1,15);
		addDBfield(sort,"LOG NUMBER",8,8);
		addDBfield(sort,"TRANSACTION NUMBER",8,8);
		addDBfield(sort,"KEY FIELD 1 NAME",1,120);
		addDBfield(sort,"KEY FIELD 1 VALUE",1,120);
		addDBfield(sort,"KEY FIELD 2 NAME",1,120);
		addDBfield(sort,"KEY FIELD 2 VALUE",1,120);
		addDBfield(sort,"KEY FIELD 3 NAME",1,120);
		addDBfield(sort,"KEY FIELD 3 VALUE",1,120);
		addDBfield(sort,"KEY FIELD 4 NAME",1,120);
		addDBfield(sort,"KEY FIELD 4 VALUE",1,120);
		addDBfield(sort,"KEY FIELD 5 NAME",1,120);
		addDBfield(sort,"KEY FIELD 5 VALUE",1,120);
		addDBfield(sort,"KEY FIELD 6 NAME",1,120);
		addDBfield(sort,"KEY FIELD 6 VALUE",1,120);
		addDBfield(sort,"KEY FIELD 7 NAME",1,120);
		addDBfield(sort,"KEY FIELD 7 VALUE",1,120);
		addDBfield(sort,"KEY FIELD 8 NAME",1,120);
		addDBfield(sort,"KEY FIELD 8 VALUE",1,120);
		addDBfield(sort,"KEY FIELD 9 NAME",1,120);
		addDBfield(sort,"KEY FIELD 9 VALUE",1,120);
		addDBfield(sort,"CHANGED FIELD",1,120);
		addDBfield(sort,"CHANGED FIELD LENGTH",8,4);
		addDBfield(sort,"CHANGED FIELD TYPE",7,2);
		addDBfield(sort,"CHANGED FILE",1,15);
		addDBfield(sort,"CHANGED MODULE",1,15);
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"NO PREVIOUS VALUE",10,1);
		addDBfield(sort,"PREVIOUS VALUE",14,2000);
		addDBfield(sort,"NEW VALUE",14,2000);
		addDBkey(sort,"DATTRN KEY 1");
		addDBkeypart(sort,1,"MODULE");
		addDBkeypart(sort,1,"USER IDENTIFICATION");
		addDBkeypart(sort,1,"LOG NUMBER");
		addDBkeypart(sort,1,"TRANSACTION NUMBER");
		addDBkey(sort,"DATTRN KEY 2");
		addDBkeypart(sort,2,"USER IDENTIFICATION");
		addDBkeypart(sort,2,"MODULE");
		addDBkeypart(sort,2,"LOG NUMBER");
		addDBkeypart(sort,2,"TRANSACTION NUMBER");

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
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [DATABASE] Screen [DATTRN], Can Not Save Database File!");
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
	DATTRN_FILENO=OPNNRD("DATABASE","DATTRN");
}
