/* GUI's - Make (AUTOCOMP) Database File */
#include <cstdio>
#include <mix.hpp>
#include <mknrd.hpp>
#include <mkmsc.hpp>

extern APPlib *errorlist;

void DB_AUTOCOMP()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	if(XPERT_SETUP->software_type>1) return;
	sort=DBsortNEW("GUI","AUTOCOMP",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"MODULE",1,15);
		addDBfield(sort,"SCREEN NAME",1,120);
		addDBfield(sort,"WIDGET",1,80);
		addDBfield(sort,"VALUE",1,120);
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBkey(sort,"AUTOCOMP KEY 1");
		addDBkeypart(sort,1,"MODULE");
		addDBkeypart(sort,1,"SCREEN NAME");
		addDBkeypart(sort,1,"WIDGET");
		addDBkeypart(sort,1,"VALUE");
		addDBkey(sort,"AUTOCOMP KEY 2");
		addDBkeypart(sort,2,"MODULE");
		addDBkeypart(sort,2,"WIDGET");
		addDBkeypart(sort,2,"SCREEN NAME");
		addDBkeypart(sort,2,"VALUE");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(8+3+77+1);
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [GUI] Screen [AUTOCOMP], Can Not Save Database File!");
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
