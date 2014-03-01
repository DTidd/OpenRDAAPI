/* ARCHIVE's - Make (USE KGHOSTVIEW) List */
#include <cstdio>
#include <mix.hpp>
#include <mkmsc.hpp>

extern char *module;
extern APPlib *errorlist;

static void MNU_ARCHIVE_DOGSV()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("ARCHIVE DOGSV","dogsv.EXT ARCHIVE");
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
static void GSV_SCAN_DIRECTORY()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("ARCHIVE","SCAN DIRECTORY");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=1;
			gsv->length=60;
			gsv->desc=stralloc("Default Directory in which to obtain scanned images.	");
			gsv->label=stralloc("Default Scan Directory:");
#ifndef WIN32
			/*gsv->value.string_value=stralloc("./scan");*/
			gsv->value.string_value=stralloc("[CURRENT DIRECTORY]+\"/\"+scan");
#endif
#ifdef WIN32
			/*gsv->value.string_value=stralloc(".\\scan");*/
			gsv->value.string_value=stralloc("[CURRENT DIRECTORY]+\"\\\"+scan");
#endif
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(14+0+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [] GSV [SCAN DIRECTORY], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
		if(libx!=NULL) Rfree(libx);
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	}
}
static void GSV_AUTOMATIC_REPORT_CAPTURE()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("ARCHIVE","AUTOMATIC REPORT CAPTURE");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("Denotes whether to automatically capture procedural reports into F.A.S.T.");
			gsv->label=stralloc("Automatic Report Capture");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=TRUE;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(24+7+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [ARCHIVE] GSV [AUTOMATIC REPORT CAPTURE], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_DEFAULT_NAMESTYLE()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("ARCHIVE","DEFAULT NAME STYLE");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=7;
			gsv->length=2;
			gsv->desc=stralloc("Used to denote the default namestyle for naming archives within Fast.");
			gsv->label=stralloc("Default Name Style:");
			gsv->value.short_value=Rmalloc(sizeof(short));
			*gsv->value.short_value=4;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(17+7+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [ARCHIVE] GSV [DEFAULT NAME STYLE], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void LST_DEFAULT_NAME_STYLES()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("ARCHIVE","DEFAULT NAME STYLES");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=1;
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->select_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->special_function=stralloc("");
		list->num=0;
		list->initfld=NULL;
		list->ffield=stralloc("");
		list->ffinfo=Rmalloc(sizeof(DFincvir));
		list->ffinfo->module=stralloc("");
		list->ffinfo->file=stralloc("");
		list->ffinfo->keyname=stralloc("");
		list->contype=1;
		list->conmod=stralloc("DEFAULT NAME STYLE");
		list->confil=stralloc("");
		list->confld=stralloc("");
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->format_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->reformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->unformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->desc=stralloc("");
		list->timing=0;
		list->list=APPlibNEW();
		addAPPlib(list->list,"YYYY-MM-DD.HH-MM.MODULE.PROCESS");
		addAPPlib(list->list,"YYYY-MM-DD.HH-MM.MODULE.PROCESS.USER");
		addAPPlib(list->list,"YYYY-MM-DD.HH-MM.MODULE.USER.PROCESS");
		addAPPlib(list->list,"YYYY-MM-DD.HH-MM.USER.MODULE.PROCESS");
		addAPPlib(list->list,"YYYY-MM-DD.HH-MM.USER.PROCESS");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.LST",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.LST",CURRENTDIRECTORY,module);
#endif
		if(writeScrolledListbin(defdir,list))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(19+7+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [ARCHIVE] RDAScrolledList [DEFAULT NAME STYLES], Can Not Save Scrolled List!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(list!=NULL) FreeRDAScrolledList(list);
	}
}
static void GSV_ADD_ARCHIVE_DEFAULT_NAMESTYLE()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("ARCHIVE","ADD ARCHIVE DEFAULT NAME STYLE");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=7;
			gsv->length=2;
			gsv->desc=stralloc("Used to denote the default namestyle for naming archives within Fast.");
			gsv->label=stralloc("Add Archive Default Name Style:");
			gsv->value.short_value=Rmalloc(sizeof(short));
			*gsv->value.short_value=12;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(29+7+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [ARCHIVE] GSV [ADD ARCHIVE DEFAULT NAMESTYLE], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_RDAOUT_DEFAULT_NAMESTYLE()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("ARCHIVE","RDAOUT DEFAULT NAME STYLE");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=7;
			gsv->length=2;
			gsv->desc=stralloc("Used to denote the default namestyle for naming archives within Fast.");
			gsv->label=stralloc("RDAOUT Default Name Style:");
			gsv->value.short_value=Rmalloc(sizeof(short));
			*gsv->value.short_value=5;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(24+7+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [ARCHIVE] GSV [RDAOUT DEFAULT NAMESTYLE], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void LST_ADD_ARCHIVE_DEFAULT_NAME_STYLES()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("ARCHIVE","ADD ARCHIVE DEFAULT NAME STYLES");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=1;
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->select_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->special_function=stralloc("");
		list->num=0;
		list->initfld=NULL;
		list->ffield=stralloc("");
		list->ffinfo=Rmalloc(sizeof(DFincvir));
		list->ffinfo->module=stralloc("");
		list->ffinfo->file=stralloc("");
		list->ffinfo->keyname=stralloc("");
		list->contype=1;
		list->conmod=stralloc("ADD ARCHIVE DEFAULT NAME STYLE");
		list->confil=stralloc("");
		list->confld=stralloc("");
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->format_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->reformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->unformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->desc=stralloc("");
		list->timing=0;
		list->list=APPlibNEW();
		addAPPlib(list->list,"Undefined");
		addAPPlib(list->list,"YYYY-MM-DD.");
		addAPPlib(list->list,"YYYY-MM-DD.MODULE.");
		addAPPlib(list->list,"YYYY-MM-DD.MODULE.USER.");
		addAPPlib(list->list,"YYYY-MM-DD.USER.");
		addAPPlib(list->list,"YYYY-MM-DD.USER.MODULE.");
		addAPPlib(list->list,"YYYY-MM.");
		addAPPlib(list->list,"YYYY-MM.MODULE.");
		addAPPlib(list->list,"YYYY-MM.MODULE.USER.");
		addAPPlib(list->list,"YYYY-MM.USER.");
		addAPPlib(list->list,"YYYY-MM.USER.MODULE.");
		addAPPlib(list->list,"YYYY-MM-DD.HH-MM.USER.");
		addAPPlib(list->list,"YYYY-MM-DD.HH-MM.USER.FILENAME");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.LST",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.LST",CURRENTDIRECTORY,module);
#endif
		if(writeScrolledListbin(defdir,list))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(31+7+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [ARCHIVE] RDAScrolledList [ADD ARCHIVE DEFAULT NAME STYLES], Can Not Save Scrolled List!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(list!=NULL) FreeRDAScrolledList(list);
	}
}
static void LST_RDAOUT_DEFAULT_NAME_STYLES()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("ARCHIVE","RDAOUT DEFAULT NAME STYLES");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=1;
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->select_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->special_function=stralloc("");
		list->num=0;
		list->initfld=NULL;
		list->ffield=stralloc("");
		list->ffinfo=Rmalloc(sizeof(DFincvir));
		list->ffinfo->module=stralloc("");
		list->ffinfo->file=stralloc("");
		list->ffinfo->keyname=stralloc("");
		list->contype=1;
		list->conmod=stralloc("RDAOUT DEFAULT NAME STYLE");
		list->confil=stralloc("");
		list->confld=stralloc("");
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->format_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->reformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->unformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->desc=stralloc("");
		list->timing=0;
		list->list=APPlibNEW();
		addAPPlib(list->list,"Undefined");
		addAPPlib(list->list,"YYYY-MM-DD.HH-MM.MODULE.PROCESS");
		addAPPlib(list->list,"YYYY-MM-DD.HH-MM.MODULE.PROCESS.USER");
		addAPPlib(list->list,"YYYY-MM-DD.HH-MM.MODULE.USER.PROCESS"); 
		addAPPlib(list->list,"YYYY-MM-DD.HH-MM.USER.MODULE.PROCESS");
		addAPPlib(list->list,"YYYY-MM-DD.HH-MM.USER.PROCESS");
		addAPPlib(list->list,"YYYY-MM-DD.MODULE.PROCESS"); 
		addAPPlib(list->list,"YYYY-MM-DD.MODULE.PROCESS.USER");
		addAPPlib(list->list,"YYYY-MM-DD.MODULE.USER.PROCESS");
		addAPPlib(list->list,"YYYY-MM-DD.USER.MODULE.PROCESS");
		addAPPlib(list->list,"YYYY-MM-DD.USER.PROCESS");
		addAPPlib(list->list,"YYYY-MM.MODULE.PROCESS"); 
		addAPPlib(list->list,"YYYY-MM.MODULE.PROCESS.USER"); 
		addAPPlib(list->list,"YYYY-MM.MODULE.USER"); 
		addAPPlib(list->list,"YYYY-MM.MODULE.USER.PROCESS"); 
		addAPPlib(list->list,"YYYY-MM.USER.MODULE.PROCESS"); 
		addAPPlib(list->list,"YYYY-MM.USER.PROCESS"); 
		addAPPlib(list->list,"YYYY-MM-DD."); 
		addAPPlib(list->list,"YYYY-MM-DD.MODULE."); 
		addAPPlib(list->list,"YYYY-MM-DD.MODULE.USER.");
		addAPPlib(list->list,"YYYY-MM-DD.USER."); 
		addAPPlib(list->list,"YYYY-MM-DD.USER.MODULE.");
		addAPPlib(list->list,"YYYY-MM.");
		addAPPlib(list->list,"YYYY-MM.MODULE.");
		addAPPlib(list->list,"YYYY-MM.USER.");
		addAPPlib(list->list,"YYYY-MM.USER.MODULE.");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.LST",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.LST",CURRENTDIRECTORY,module);
#endif
		if(writeScrolledListbin(defdir,list))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(26+7+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [ARCHIVE] RDAScrolledList [RDAOUT DEFAULT NAME STYLES], Can Not Save Scrolled List!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(list!=NULL) FreeRDAScrolledList(list);
	}
}
static void GSV_ARCHIVE_BACKUPS()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("ARCHIVE","ARCHIVE BACKUPS");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=10;
			gsv->length=1;
			gsv->desc=stralloc("Used to denote when CRONFTPZIP and DOFTPZIP will automatically archive the backups created.");
			gsv->label=stralloc("Archive Backups");
			gsv->value.string_value=Rmalloc(1);
			*gsv->value.string_value=0;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(15+7+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [ARCHIVE] GSV [ARCHIVE BACKUPS], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void GSV_ARCHIVE_STYLE()
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("ARCHIVE","ARCHIVE STYLE");
	if(gsv!=NULL)
	{
		if(getRDAGenericSetupbin(libx,gsv))
		{
			gsv->type=7;
			gsv->length=1;
			gsv->desc=stralloc("Denotes which style of Archive implemented: 0-Automagical Only, 1-Automagical with Work Groups, 2-Automagical, Work Groups, and Scanning Technologies.");
			gsv->label=stralloc("ARCHIVE STYLE");
			gsv->value.short_value=Rmalloc(sizeof(short));
			*gsv->value.short_value=0;
	
			if(writeRDAGenericSetupbin(libx,gsv))
			{
				if(temp1!=NULL) Rfree(temp1);
				if(libx!=NULL) Rfree(libx);
				temp1=Rmalloc(13+7+110+1);
				sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [ARCHIVE] GSV [ARCHIVE STYLE], Can Not Save Generic Setup Variable!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
static void LST_ARCHIVE_STYLES()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("ARCHIVE","ARCHIVE STYLES");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=1;
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->select_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->special_function=stralloc("");
		list->num=0;
		list->initfld=NULL;
		list->ffield=stralloc("");
		list->ffinfo=Rmalloc(sizeof(DFincvir));
		list->ffinfo->module=stralloc("");
		list->ffinfo->file=stralloc("");
		list->ffinfo->keyname=stralloc("");
		list->contype=1;
		list->conmod=stralloc("ARCHIVE STYLE");
		list->confil=stralloc("");
		list->confld=stralloc("");
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->format_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->reformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->unformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->desc=stralloc("0-Automagical Only, 1-Automagical with Work Groups, 2-Automagical, Work Groups and Scanning Technologies.");
		list->timing=0;
		list->list=APPlibNEW();
		addAPPlib(list->list,"Automagical Report Capture Only");
		addAPPlib(list->list,"Automagical with Work Groups");
		addAPPlib(list->list,"Automagical, Work Groups, and Scanning");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.LST",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.LST",CURRENTDIRECTORY,module);
#endif
		if(writeScrolledListbin(defdir,list))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(14+7+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [ARCHIVE] RDAScrolledList [ARCHIVE STYLES], Can Not Save Scrolled List!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(list!=NULL) FreeRDAScrolledList(list);
	}
}
static void SCN_GENERIC_SETUP_VARIABLES()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","GENERIC SETUP VARIABLES");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Installation Date:","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(5+1);
		sprintf(temp2,"%s",
			"FALSE");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"INSTALLATION DATE","Installation Date","","",0,10,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Compile Date:","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(5+1);
		sprintf(temp2,"%s",
			"FALSE");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,0,"COMPILE DATE","Compile Date","","",0,10,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,11,"","","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","         Archive Style:","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(20+1);
		sprintf(temp2,"%s",
			"([DEV_LICENSE]=TRUE)");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,7,"ARCHIVE STYLES","Archive Style:","","",3,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Default Scan Directory:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SCAN DIRECTORY","Default Scan Directory:","","",0,60,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"AUTOMATIC REPORT CAPTURE","Automatic Report Capture","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","(Using this feature requires READ/WRITE Security to ARCHIVE for everyone.)","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"ARCHIVE BACKUPS","Automatic Archive DoFtpzip/CronFTPzip Backups","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Automatic Report Capture Name Style:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"DEFAULT NAME STYLES","Default Name Style","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","  Build Archives Default Name Style:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"ADD ARCHIVE DEFAULT NAME STYLES","Add Archive Default Name Style","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","          RDAOUT Default Name Style:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"RDAOUT DEFAULT NAME STYLES","RDAOUT Default Name Style","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,23,NULL,NULL,NULL,NULL);
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
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(23+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [GENERIC SETUP VARIABLES], Can Not Save Maintain Master!");
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
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
}
void ARCGEN()
{
	MNU_ARCHIVE_DOGSV();
	SCN_GENERIC_SETUP_VARIABLES();
	GSV_SCAN_DIRECTORY();
	GSV_AUTOMATIC_REPORT_CAPTURE();
	GSV_DEFAULT_NAMESTYLE();
	LST_DEFAULT_NAME_STYLES();
	GSV_ADD_ARCHIVE_DEFAULT_NAMESTYLE();
	GSV_RDAOUT_DEFAULT_NAMESTYLE();
	LST_ADD_ARCHIVE_DEFAULT_NAME_STYLES();
	LST_RDAOUT_DEFAULT_NAME_STYLES();
	GSV_ARCHIVE_BACKUPS();
	GSV_ARCHIVE_STYLE();
	LST_ARCHIVE_STYLES();
}
