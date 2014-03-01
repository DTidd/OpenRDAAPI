/* OLHELP's - Make (MTN SCNNOT) Maintain Master */
#include <cstdio>
#include <mkmtn.hpp>
#include <mkdef.hpp>
#include <mkmsc.hpp>

extern char *module;
extern APPlib *errorlist;

static void MTN_SCNNOT()
{
	char *defdir=NULL;
	int x=0;
	MaintainMaster *MTNMSTR=NULL;
	APPlib *args=NULL,*args2=NULL;
	MaintainButton *button=NULL;

	MTNMSTR=RDAMaintainMasterNEW(module,"MTN SCNNOT","This Maintain Master definition is used to define the Screen Notes Master.",1,"SCNNOT","SCREEN NOTES BROWSE","SCREEN NOTES DEFINE LIST","SCREEN NOTES SEARCH BROWSE","SCREEN NOTES MAINTAIN SCREEN",0,1,0,0,
"","","","","",FALSE,FALSE,FALSE,"");
	if(MTNMSTR!=NULL)
	{
		
		args=APPlibNEW();
		addAPPlib(args,"OLHELP");
		addAPPlib(args,"MTN USCNNOT");
		x=addMaintainMasterButton(MTNMSTR,"mtnmst","Maintain User Screen Notes","USER SCREEN NOTES","OLHELP","USCNNOT","USCNNOT KEY","",0,0,args,0,NULL);
		if(args!=NULL) freeapplib(args);
		if(x!=(-1))
		{
			button=MTNMSTR->buttons+(x-1);
			x=addMaintainMasterButtonField(button,"MODULE NAME","MODULE_NAME");
			x=addMaintainMasterButtonField(button,"SCREEN NAME","SCREEN_NAME");
		}
		
		args=APPlibNEW();
		addAPPlib(args,"OLHELP");
		addAPPlib(args,"MTN RSCNOT");
		x=addMaintainMasterButton(MTNMSTR,"mtnmst","Maintain Resource Notes","RESOURCE SCREEN NOTES","OLHELP","RSCNOT","RSCNOT KEY","",0,0,args,0,NULL);
		if(args!=NULL) freeapplib(args);
		if(x!=(-1))
		{
			button=MTNMSTR->buttons+(x-1);
			x=addMaintainMasterButtonField(button,"MODULE NAME","MODULE_NAME");
			x=addMaintainMasterButtonField(button,"SCREEN NAME","SCREEN_NAME");
		}


		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.MTN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.MTN",CURRENTDIRECTORY,module);
#endif
		if(writeMaintainMaster(defdir,MTNMSTR))
		{
			ERRORDIALOG("CANNOT SAVE MAINTAIN MASTER","This Maintain Master definition may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(args!=NULL) freeapplib(args);
		if(args2!=NULL) freeapplib(args2);
		if(MTNMSTR!=NULL) FreeMaintainMaster(MTNMSTR);
	}
}
void SCREEN_NOTES_BROWSE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;

	scrn=RDAscrnNEW("OLHELP","SCREEN NOTES BROWSE");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Execute following function:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"RUN FUNCTION","Run Function","","",1,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"DEFINE LIST","Define List","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SEARCH BUTTON","Search","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"PRINT BROWSE","Print","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"TOP","Top","","",0,0,41,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"BOTTOM","Bottom","","",0,0,42,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SEARCH ALL","Search All","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"CLEAR SEARCH & LPM","Clear (Search & LPM)","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","LPM:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"BROWSE LIST LPM","Lexical Pattern Match:","","",0,20,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Search:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"SEARCH FIELD","Search Field","","",0,30,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"BROWSE LIST","Browse List","","",10,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"ADD","Add","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Quit","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,"","","","");
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
void SCREEN_NOTES_DEFINE_LIST()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;

	scrn=RDAscrnNEW("OLHELP","SCREEN NOTES DEFINE LIST");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Enter the position and length for the following fields:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Delete flag:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"DELETEFLAG POSITION","Deleteflag Position","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"DELETEFLAG LENGTH","Deleteflag Length","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Module name:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"MODULE NAME POSITION","Module Name Position","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"MODULE NAME LENGTH","Module Name Length","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Screen name:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"SCREEN NAME POSITION","Screen Name Position","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"SCREEN NAME LENGTH","Screen Name Length","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Notes:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"NOTES POSITION","Notes Position","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"NOTES LENGTH","Notes Length","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Quit","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,"","","","");
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
static void DEF_SCREEN_NOTES_DEFINE_LIST()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	def=RDAdefaultNEW("OLHELP","SCREEN NOTES DEFINE LIST");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldINT(def,"KEY LIST",TRUE,13,0);
		addDefaultFieldSHORT(def,"DELETEFLAG POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"DELETEFLAG LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"MODULE NAME POSITION",TRUE,7,1);
		addDefaultFieldSHORT(def,"MODULE NAME LENGTH",TRUE,7,15);
		addDefaultFieldSHORT(def,"SCREEN NAME POSITION",TRUE,7,2);
		addDefaultFieldSHORT(def,"SCREEN NAME LENGTH",TRUE,7,25);
		addDefaultFieldSHORT(def,"NOTES POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"NOTES LENGTH",TRUE,7,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.DEF",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.DEF",CURRENTDIRECTORY,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(24+6+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [OLHELP] Screen [SCREEN NOTES DEFINE LIST], Can Not Save Screen Defaults!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(def!=NULL) FreeRDAdefault(def);
	}
}
void SCREEN_NOTES_MAINTAIN_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;

	scrn=RDAscrnNEW("OLHELP","SCREEN NOTES MAINTAIN SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"[SCNNOT][DELETEFLAG]","Delete this record","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Module name:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"PREVIOUS SCNNOT KEY","Previous Key","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,0,"[SCNNOT][MODULE NAME]","Module Name","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,6,"NEXT SCNNOT KEY","Next Key","","",0,0,1,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Screen name:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[SCNNOT][SCREEN NAME]","Screen Name","","",0,60,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Notes:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,8,"[SCNNOT][NOTES]","Notes","","",10,80,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,11,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"BROWSE","Browse","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"USER SCREEN NOTES","User's Screen Notes","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"RESOURCE SCREEN NOTES","Resource Screen Notes","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SAVE NO UPDATE","Fast Save","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,11,"","","","",0,0,1,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help ","","",0,0,0,"","","","");
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
void SCREEN_NOTES_SEARCH_BROWSE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;

	scrn=RDAscrnNEW("OLHELP","SCREEN NOTES SEARCH BROWSE");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [SCNNOT][MODULE NAME]","Range on Module Name","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [SCNNOT][MODULE NAME]","From Module Name","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [SCNNOT][MODULE NAME]","To Module Name","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [SCNNOT][SCREEN NAME]","Range on Screen Name","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [SCNNOT][SCREEN NAME]","From Screen Name","","",0,30,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [SCNNOT][SCREEN NAME]","To Screen Name","","",0,30,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"SELECT [SCNNOT][DELETEFLAG] FALSE","Select non-deleted records","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"SELECT [SCNNOT][DELETEFLAG] TRUE","Select deleted records","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Quit","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,"","","","");
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
static void SCREEN_NOTES_MASTER_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("SCREEN NOTES MASTER","mtnmst.EXT OLHELP \"MTN SCNNOT\"");
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
void SCNNOT_MASTER()
{
	MTN_SCNNOT();
	SCREEN_NOTES_SEARCH_BROWSE();
	SCREEN_NOTES_MAINTAIN_SCREEN();
	SCREEN_NOTES_DEFINE_LIST();
	DEF_SCREEN_NOTES_DEFINE_LIST();
	SCREEN_NOTES_BROWSE();
	SCREEN_NOTES_MASTER_MENU();
}
