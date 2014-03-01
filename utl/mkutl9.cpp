/* UTILITIES's - Make (DEFINE GENERIC SETUP VARIABLES) Screen */
#include <cstdio>
#include <mkmsc.hpp>

extern char *module;
extern APPlib *errorlist;

static void DEFINE_GENERIC_SETUP_VARIABLES()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	if(XPERT_SETUP->TOOLS==TRUE)
	{
	scrn=RDAscrnNEW("UTILITIES","DEFINE GENERIC SETUP VARIABLES");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Modules Available:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Generic Setup Variables:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"MODULE LIST","Module List","","",15,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"GENERIC SETUP VARIABLES AVAILABLE","Generic Setup Variables Available","","",15,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Description of Setup Variable:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"DESCRIPTION","Description","","",3,50,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"USE DESCRIPTION","Use Description when Making Screen","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,11,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"DFSCN","Define Screen","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"DFLIST","Define List","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"DFMENU","Define Menu","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"DFLOAD","Define Load","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,11,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ADD","Add","","",0,0,16,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,36,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DOGSV","Execute","","",0,0,25,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SCREEN","Make Screen","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"PRINT","Print","","",0,0,7,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DELETE","Delete","","",0,0,33,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
/*
			ERRORDIALOG("Cannot Save Screen","This screen may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
*/
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(30+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [DEFINE GENERIC SETUP VARIABLES], Can Not Save Maintain Master!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
	}
}
static void MAINTAIN_GENERIC_SETUP_VARIABLE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	if(XPERT_SETUP->TOOLS==TRUE)
	{
		scrn=RDAscrnNEW("UTILITIES","MAINTAIN GENERIC SETUP VARIABLE");
		if(scrn!=NULL)
		{
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,5,"","Module:","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,0,"MODULE","Module","","",0,15,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,5,"","Name:","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,0,"NAME","Name","","",0,40,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,11,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,5,"","Description:","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,8,"DESCRIPTION","Description","","",3,70,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,5,"","Label:","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,0,"LABEL","Label","","",0,40,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,11,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,11,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,5,"","Field Type:","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,7,"FIELD TYPES","Field Types","","",1,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,5,"","Length:","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,0,"LENGTH","Length","","",0,3,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,11,"","","","",0,0,1,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,23,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,6,"QUIT","Quit","","",0,0,14,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
			ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	
			defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
			sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
			sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
			if(writescrnbin(defdir,scrn))
			{
/*
				ERRORDIALOG("Cannot Save Screen","This screen may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
*/
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(31+9+89+1);
				sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [MAINTAIN GENERIC SETUP VARIABLE], Can Not Save Maintain Master!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
			if(defdir!=NULL) Rfree(defdir);
			if(scrn!=NULL) free_scrn(scrn);
		}
	}
}
static void DEFINE_GENERIC_SETUP_VARIABLES_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	if(XPERT_SETUP->TOOLS==TRUE)
	{
	menu=RDAmenuNEW("DEFINE GENERIC SETUP VARIABLES","dfgsv.EXT");
	if(menu!=NULL)
	{
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.MNU",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.MNU",CURRENTDIRECTORY,module);
#endif
		if(writemenubin(defdir,menu))
		{
			ERRORDIALOG("Cannot Save Menu","This menu may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(menu!=NULL) free_menu(menu);
	}
	}
}
void DFGSV()
{
	if(XPERT_SETUP->TOOLS==TRUE)
	{
		DEFINE_GENERIC_SETUP_VARIABLES();
		MAINTAIN_GENERIC_SETUP_VARIABLE();
		DEFINE_GENERIC_SETUP_VARIABLES_MENU();
	}
}
