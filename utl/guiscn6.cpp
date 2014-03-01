#include <cstdio>
#include <mix.hpp>
extern char *module;

static void CONVERT_SCREENS_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("CONVERT SCREENS","convscrn.EXT");
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
static void CONVERT_SCREENS()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;

	scrn=RDAscrnNEW("GUI","CONVERT SCREENS");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Directory:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"DIRECTORY LIST","Directory List","","",2,0,0,"","","","");
		ADVaddwdgt(scrn,0,"DIRECTORY","Directory","","",0,20,0,"","","","");
		ADVaddwdgt(scrn,5,"","Library:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"LIBRARY LIST","Library List","","",2,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Match Type:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"MATCH TYPE","Match Type","","",1,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,11,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","WIDGET TO CHANGE","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Name:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM WIDGET NAME","From Widget Name","","",0,40,0,"","","","");
		ADVaddwdgt(scrn,5,"","Label:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM WIDGET LABEL","From Widget Label","","",0,40,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Widget Pixmap:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM WIDGET PIXMAP","From Widget Pixmap","","",0,40,0,"","","","");
		ADVaddwdgt(scrn,5,"","YWindow Label:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM YWINDOW LABEL","From YWindow Label","","",0,40,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Widget Type:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"FROM WIDGET TYPES","From Widget Types","","",1,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Number Rows:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM NUMBER ROWS","From Number Rows","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,5,"","Number Columns:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM NUMBER COLUMNS","From Number Columns","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Resource Type:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"FROM RESOURCE TYPES","From Resource Types","","",1,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,11,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","NEW WIDGET VALUE","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Name:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO WIDGET NAME","To Widget Name","","",0,40,0,"","","","");
		ADVaddwdgt(scrn,5,"","Label:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO WIDGET LABEL","To Widget Label","","",0,40,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Widget Pixmap:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO WIDGET PIXMAP","To Widget Pixmap","","",0,40,0,"","","","");
		ADVaddwdgt(scrn,5,"","YWindow Label:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO YWINDOW LABEL","To YWindow Label","","",0,40,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Widget Type:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"TO WIDGET TYPES","To Widget Types","","",1,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Number Rows:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO NUMBER ROWS","To Number Rows","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,5,"","Number Columns:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO NUMBER COLUMNS","To Number Columns","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Resource Type:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"TO RESOURCE TYPES","To Resource Types","","",1,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,11,"","","","",0,0,1,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,25,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,"","","","");
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
void mk_convscrn()
{
	if(XPERT_SETUP->TOOLS==TRUE)
	{
		CONVERT_SCREENS();
		CONVERT_SCREENS_MENU();
	}
}
