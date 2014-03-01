#include<mix.hpp>

extern char *module;

static void VIEW_LOG_ENTRIES()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;

	scrn=RDAscrnNEW("DATABASE","VIEW LOG ENTRIES");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Development License:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DEV_LICENSE","Development License","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Modules Available:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"MODULES AVAILABLE","Modules Available","","",1,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Log Entries","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"LOG LIST","Log List","","",10,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"VIEW","View Transaction","","",0,0,36,"","","","");
		ADVaddwdgt(scrn,6,"UNDO","Undo","","",0,0,9,"","","","");
		ADVaddwdgt(scrn,6,"REDO","Redo","","",0,0,8,"","","","");
		ADVaddwdgt(scrn,6,"DELETE","Delete","","",0,0,33,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Quit","","",0,0,14,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
			ERRORDIALOG("Cannot Save Screen","This screen may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
}
static void VIEW_TRANSACTION()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;

	scrn=RDAscrnNEW("DATABASE","VIEW TRANSACTION");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Module Name:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"MODULE NAME","Module Name","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,5,"","Transaction:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TRANSACTION NUMBER","Transaction Number","","",0,10,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Process:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"PROCESS NAME","Process Name","","",0,25,0,"","","","");
		ADVaddwdgt(scrn,5,"","User Name:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"USER NAME","User Name","","",0,25,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Start Time:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"START TIME","Start Time","","",0,20,0,"","","","");
		ADVaddwdgt(scrn,5,"","End Time:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"END TIME","End Time","","",0,20,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"ABLE TO UNDO","Able To Undo","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"COMPLETE","Complete","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Transaction List","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"TRANSACTION LIST","Transaction List","","",10,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"PRINT TRANSACTION","Print Transaction","","",0,0,7,"","","","");
		ADVaddwdgt(scrn,6,"DETAIL","Show Detail","","",0,0,36,"","","","");
		ADVaddwdgt(scrn,6,"UNDO","Undo Transaction","","",0,0,9,"","","","");
		ADVaddwdgt(scrn,6,"REDO","Redo Transaction","","",0,0,8,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Quit","","",0,0,14,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help View Transaction","","",0,0,11,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
			ERRORDIALOG("Cannot Save Screen","This screen may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
}
static void VIEW_TRANSACTION_DETAIL()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;

	scrn=RDAscrnNEW("DATABASE","VIEW TRANSACTION DETAIL");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Module Name:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"MODULE NAME","Module Name","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,5,"","Transaction:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TRANSACTION NUMBER","Transaction Number","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Transaction Description","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,8,"TRANSACTION DESCRIPTION","Transaction Description","","",10,80,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"UNDO","Undo Transaction","","",0,0,9,"","","","");
		ADVaddwdgt(scrn,6,"REDO","Redo Transaction","","",0,0,8,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Quit","","",0,0,14,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help View Detail","","",0,0,11,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
			ERRORDIALOG("Cannot Save Screen","This screen may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
}
static void VIEW_TRANSACTIONS_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("VIEW TRANSACTIONS","viewtran.EXT");
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
void mk_viewtran()
{
	VIEW_LOG_ENTRIES();
	VIEW_TRANSACTION();
	VIEW_TRANSACTION_DETAIL();
	VIEW_TRANSACTIONS_MENU();
}
