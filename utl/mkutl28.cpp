/* UTILITIES's - Make (MENUITEM) Database File */
#include <cstdio>
#include <mix.hpp>
#include <mkmtn.hpp>
#include <mkdef.hpp>
#include <mknrd.hpp>
#include <mkmsc.hpp>

extern char *module;
extern APPlib *errorlist;

void DB_MENUITEM()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	sort=DBsortNEW("UTILITIES","MENUITEM",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"MODULE NAME",1,15);
		addDBfield(sort,"ITEM NUMBER",8,8);
		addDBfield(sort,"ARGUMENT 1",1,512);
		addDBfield(sort,"ARGUMENT 2",1,512);
		addDBfield(sort,"ARGUMENT 3",1,512);
		addDBfield(sort,"ARGUMENT 4",1,512);
		addDBfield(sort,"ARGUMENT 5",1,512);
		addDBfield(sort,"ARGUMENT 6",1,512);
		addDBfield(sort,"ARGUMENT 7",1,512);
		addDBfield(sort,"ARGUMENT 8",1,512);
		addDBfield(sort,"CUSTOM",10,1);
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"DROP DOWN LIST",7,2);
		addDBfield(sort,"ENVIRONMENT NAME 1",1,120);
		addDBfield(sort,"ENVIRONMENT NAME 2",1,120);
		addDBfield(sort,"ENVIRONMENT NAME 3",1,120);
		addDBfield(sort,"ENVIRONMENT NAME 4",1,120);
		addDBfield(sort,"ENVIRONMENT NAME 5",1,120);
		addDBfield(sort,"NAME",1,120);
		addDBfield(sort,"PARENT",8,8);
		addDBfield(sort,"PROGRAM",1,120);
		addDBfield(sort,"RANK",7,4);
		addDBfield(sort,"STYLE",7,2);
		addDBfield(sort,"Z ITEM NUMBER",8,8);
		addDBfield(sort,"Z PARENT",8,8);
		addDBfield(sort,"UIMETRIC COUNT",8,8);
		addDBfield(sort,"ENVIRONMENT VALUE 1",14,2000);
		addDBfield(sort,"ENVIRONMENT VALUE 2",14,2000);
		addDBfield(sort,"ENVIRONMENT VALUE 3",14,2000);
		addDBfield(sort,"ENVIRONMENT VALUE 4",14,2000);
		addDBfield(sort,"ENVIRONMENT VALUE 5",14,2000);
		addDBfield(sort,"VISIBLE EXPRESSION",14,2000);
		addDBfield(sort,"RESTRICTED PROCESS",10,1);
		addDBkey(sort,"MENUITEM KEY 1");
		addDBkeypart(sort,1,"MODULE NAME");
		addDBkeypart(sort,1,"ITEM NUMBER");
		addDBkey(sort,"MENUITEM KEY 2");
		addDBkeypart(sort,2,"MODULE NAME");
		addDBkeypart(sort,2,"DROP DOWN LIST");
		addDBkeypart(sort,2,"PARENT");
		addDBkeypart(sort,2,"RANK");
		addDBkeypart(sort,2,"ITEM NUMBER");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(8+9+77+1);
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [UTILITIES] Screen [MENUITEM], Can Not Save Database File!");
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
/* UTILITIES's - Make (MTN MENUITEM) Maintain Master */
static void MNU_UTILITIES_MTN_MENUITEM()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("UTILITIES MTN MENUITEM","mtnmst.EXT UTILITIES \"MTN MENUITEM\"");
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
static void LST_DROP_DOWN_LISTS()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("UTILITIES","DROP DOWN LISTS");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=1;
		ScrolledListDFincvir(list,"UTILITIES","MENUITEM","",0);
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
		list->contype=0;
		list->conmod=stralloc("UTILITIES");
		list->confil=stralloc("MENUITEM");
		list->confld=stralloc("DROP DOWN LIST");
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
		addAPPlib(list->list,"QUERY");
		addAPPlib(list->list,"REPORTS");
		addAPPlib(list->list,"MAINTENANCE");
		addAPPlib(list->list,"SETUP");
		addAPPlib(list->list,"OTHER");
		addAPPlib(list->list,"PROCESS AREA");

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
			temp1=Rmalloc(15+9+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [UTILITIES] RDAScrolledList [DROP DOWN LISTS], Can Not Save Scrolled List!");
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
static void LST_MENUITEM_MODULES()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("UTILITIES","MENUITEM MODULES");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=2;
		ScrolledListDFincvir(list,"UTILITIES","MENUITEM","",0);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->select_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->special_function=stralloc("MODULES AVAILABLE");
		list->num=0;
		list->initfld=NULL;
		list->ffield=stralloc("");
		list->ffinfo=Rmalloc(sizeof(DFincvir));
		list->ffinfo->module=stralloc("");
		list->ffinfo->file=stralloc("");
		list->ffinfo->keyname=stralloc("");
		list->contype=0;
		list->conmod=stralloc("UTILITIES");
		list->confil=stralloc("MENUITEM");
		list->confld=stralloc("MODULE NAME");
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->format_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(34+1);
	sprintf(temp1,"%s",
		"[UTILITIES][MENUITEM][MODULE NAME]");
		list->reformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(18+1);
	sprintf(temp1,"%s",
		"[MENUITEM MODULES]");
		list->unformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->desc=stralloc("");
		list->timing=0;

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
			temp1=Rmalloc(16+9+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [UTILITIES] RDAScrolledList [MENUITEM MODULES], Can Not Save Scrolled List!");
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
static void SCN_MENUITEM_BROWSE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","MENUITEM BROWSE");
	if(scrn!=NULL)
	{
		temp1=Rmalloc(23+1);
		sprintf(temp1,"%s",
			"[EXTENSIONS SETUP]=TRUE");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFINE LIST","Define List","","",0,0,15,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SEARCH BUTTON","Search","","",0,0,12,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"PRINT BROWSE","Print Browse","","",0,0,7,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"TOP","Top","","",0,0,41,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"BOTTOM","Bottom","","",0,0,42,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SEARCH ALL","Search All","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"CLEAR SEARCH & LPM","Clear (Search & LPM)","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","LPM:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"BROWSE LIST LPM","Lexical Pattern Match:","","",0,20,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Search Field:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SEARCH FIELD","Search Field","","",0,30,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"BROWSE LIST","Browse List","","",10,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Position","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"BROWSE LIST POSITION","Position","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Count","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DELETEFLAG TOTAL","Count","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ADD","Add","","",0,0,16,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,36,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(15+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [MENUITEM BROWSE], Can Not Save Maintain Master!");
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
static void SCN_MENUITEM_DEFINE_LIST()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","MENUITEM DEFINE LIST");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Enter a position and length for each of the following:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,12,"","","","",300,500,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Argument 1:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Argument 2:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Argument 3:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Argument 4:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Argument 5:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Argument 6:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Argument 7:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Argument 8:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Custom:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Deleteflag:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Drop Down List:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Environment Name 1:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Environment Name 2:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Environment Name 3:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Environment Name 4:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Environment Name 5:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Environment Value 1:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Environment Value 2:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Environment Value 3:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Environment Value 4:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Environment Value 5:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Item Number:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Module Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Parent:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Program:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Rank:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Restricted Process:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Style:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Visible Expression:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","UI Metric Count:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 1 POSITION","Argument 1 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 1 LENGTH","Argument 1 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 2 POSITION","Argument 2 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 2 LENGTH","Argument 2 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 3 POSITION","Argument 3 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 3 LENGTH","Argument 3 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 4 POSITION","Argument 4 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 4 LENGTH","Argument 4 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 5 POSITION","Argument 5 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 5 LENGTH","Argument 5 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 6 POSITION","Argument 6 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 6 LENGTH","Argument 6 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 7 POSITION","Argument 7 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 7 LENGTH","Argument 7 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 8 POSITION","Argument 8 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 8 LENGTH","Argument 8 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"CUSTOM POSITION","Custom Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"CUSTOM LENGTH","Custom Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DELETEFLAG POSITION","Deleteflag Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DELETEFLAG LENGTH","Deleteflag Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DROP DOWN LIST POSITION","Drop Down List Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DROP DOWN LIST LENGTH","Drop Down List Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT NAME 1 POSITION","Environment Name 1 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT NAME 1 LENGTH","Environment Name 1 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT NAME 2 POSITION","Environment Name 2 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT NAME 2 LENGTH","Environment Name 2 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT NAME 3 POSITION","Environment Name 3 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT NAME 3 LENGTH","Environment Name 3 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT NAME 4 POSITION","Environment Name 4 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT NAME 4 LENGTH","Environment Name 4 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT NAME 5 POSITION","Environment Name 5 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT NAME 5 LENGTH","Environment Name 5 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT VALUE 1 POSITION","Environment Value 1 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT VALUE 1 LENGTH","Environment Value 1 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT VALUE 2 POSITION","Environment Value 2 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT VALUE 2 LENGTH","Environment Value 2 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT VALUE 3 POSITION","Environment Value 3 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT VALUE 3 LENGTH","Environment Value 3 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT VALUE 4 POSITION","Environment Value 4 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT VALUE 4 LENGTH","Environment Value 4 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT VALUE 5 POSITION","Environment Value 5 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT VALUE 5 LENGTH","Environment Value 5 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ITEM NUMBER POSITION","Item Number Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ITEM NUMBER LENGTH","Item Number Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"MODULE NAME POSITION","Module Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"MODULE NAME LENGTH","Module Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"NAME POSITION","Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"NAME LENGTH","Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"PARENT POSITION","Parent Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"PARENT LENGTH","Parent Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"PROGRAM POSITION","Program Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"PROGRAM LENGTH","Program Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"RANK POSITION","Rank Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"RANK LENGTH","Rank Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"RESTRICTED PROCESS POSITION","Restricted Process Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"RESTRICTED PROCESS LENGTH","Restricted Process Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"STYLE POSITION","Style Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"STYLE LENGTH","Style Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"VISIBLE EXPRESSION POSITION","Visible Expression Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"VISIBLE EXPRESSION LENGTH","Visible Expression Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"UIMETRIC COUNT POSITION","UI Metric Count Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"UIMETRIC COUNT LENGTH","UI Metric Count Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,13,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,15,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,17,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(20+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [MENUITEM DEFINE LIST], Can Not Save Maintain Master!");
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
static void SCN_MENUITEM_SEARCH_BROWSE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","MENUITEM SEARCH BROWSE");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,12,"","","","",350,791,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [MENUITEM][CUSTOM] FALSE","Select when Custom False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [MENUITEM][CUSTOM] TRUE","Select when Custom True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [MENUITEM][DELETEFLAG] FALSE","Select when Deleteflag False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [MENUITEM][DELETEFLAG] TRUE","Select when Deleteflag True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [MENUITEM][RESTRICTED PROCESS] FALSE","Select when Restricted Process False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [MENUITEM][RESTRICTED PROCESS] TRUE","Select when Restricted Process True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","SEARCH CRITERIA","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","FROM","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","TO","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][DROP DOWN LIST]","Range on Drop Down List","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][DROP DOWN LIST]","From Drop Down List","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][DROP DOWN LIST]","To Drop Down List","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ITEM NUMBER]","Range on Item Number","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ITEM NUMBER]","From Item Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ITEM NUMBER]","To Item Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][MODULE NAME]","Range on Module Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][MODULE NAME]","From Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][MODULE NAME]","To Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][NAME]","Range on Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][NAME]","From Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][NAME]","To Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][PARENT]","Range on Parent","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][PARENT]","From Parent","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][PARENT]","To Parent","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][PROGRAM]","Range on Program","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][PROGRAM]","From Program","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][PROGRAM]","To Program","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][RANK]","Range on Rank","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][RANK]","From Rank","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][RANK]","To Rank","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][STYLE]","Range on Style","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][STYLE]","From Style","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][STYLE]","To Style","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 1]","Range on Argument 1","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 1]","From Argument 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 1]","To Argument 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 2]","Range on Argument 2","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 2]","From Argument 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 2]","To Argument 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 3]","Range on Argument 3","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 3]","From Argument 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 3]","To Argument 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 4]","Range on Argument 4","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 4]","From Argument 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 4]","To Argument 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 5]","Range on Argument 5","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 5]","From Argument 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 5]","To Argument 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 6]","Range on Argument 6","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 6]","From Argument 6","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 6]","To Argument 6","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 7]","Range on Argument 7","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 7]","From Argument 7","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 7]","To Argument 7","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 8]","Range on Argument 8","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 8]","From Argument 8","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 8]","To Argument 8","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 1]","Range on Environment Name 1","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 1]","From Environment Name 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 1]","To Environment Name 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 2]","Range on Environment Name 2","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 2]","From Environment Name 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 2]","To Environment Name 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 3]","Range on Environment Name 3","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 3]","From Environment Name 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 3]","To Environment Name 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 4]","Range on Environment Name 4","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 4]","From Environment Name 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 4]","To Environment Name 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 5]","Range on Environment Name 5","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 5]","From Environment Name 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 5]","To Environment Name 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][UIMETRIC COUNT]","Range on UI Metric Count","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][UIMETRIC COUNT]","From UI Metric Count","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][UIMETRIC COUNT]","To UI Metric Count","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,13,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Query:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"QUERY","Query","","",0,60,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,12,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,14,"MENUITEM SEARCH BROWSE EXPRESSION","Expression","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,17,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(22+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [MENUITEM SEARCH BROWSE], Can Not Save Maintain Master!");
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
static void SCN_MENUITEM_SEARCH_BROWSE_EXPRESSION()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","MENUITEM SEARCH BROWSE EXPRESSION");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"EXPRESSION","Expression","","",5,80,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD VALUE","Load Value","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD GROUPER","Load Grouper","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD OPERATOR","Load Operator","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD CONTROL","Load Control","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,11,"","","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(33+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [MENUITEM SEARCH BROWSE EXPRESSION], Can Not Save Maintain Master!");
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
static void SCN_MENUITEM_MAINTAIN_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","MENUITEM MAINTAIN SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"FALSE",NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"MENUITEM SAVE ERROR 01","Menuitem Save Error 01","","",5,80,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"MENUITEM SAVE ERROR 02","Menuitem Save Error 02","","",5,80,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"MENUITEM SAVE ERROR","Menuitem Save Error","","",5,80,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"[MENUITEM][DELETEFLAG]","Delete this record","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Module Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"MENUITEM MODULES","Modules","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[MENUITEM][MODULE NAME]","Module Name:","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Item Number:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"PREVIOUS MENUITEM KEY 1","Previous","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[MENUITEM][ITEM NUMBER]","Item Number:","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"NEXT MENUITEM KEY 1","Next","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","       Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[MENUITEM][NAME]","Name:","","",0,60,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"[MENUITEM][RESTRICTED PROCESS]","Restricted Process","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","     Parent:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD PARENT","Load Parent","","",0,0,1,NULL,NULL,"([UTILITIES][MENUITEM][DROP DOWN LIST]!=5)",NULL);
		ADVaddwdgt(scrn,0,"[MENUITEM][PARENT]","Parent Item Number:","","",0,8,0,NULL,NULL,"([UTILITIES][MENUITEM][DROP DOWN LIST]!=5)",NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"[MENUITEM][CUSTOM]","Custom Menu Item","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Style:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"MENUITEM STYLES","Menu Item Style","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[MENUITEM][STYLE]","Menu Item Style","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","  Drop down:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"DROP DOWN LISTS","Drop Down List","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[MENUITEM][DROP DOWN LIST]","Drop Down List","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Rank:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[MENUITEM][RANK]","Menu Item Rank:","","",0,4,0,NULL,NULL,"([UTILITIES][MENUITEM][DROP DOWN LIST]!=5)",NULL);
		ADVaddwdgt(scrn,5,"","Program:","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(32+1);
		sprintf(temp2,"%s",
			"([UTILITIES][MENUITEM][STYLE]=0)");
		temp3=Rmalloc(32+1);
		sprintf(temp3,"%s",
			"([UTILITIES][MENUITEM][STYLE]=0)");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,7,"MENUITEM PROGRAMS","Program","","",1,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,23,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Arguments","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Argument 1:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Argument 2:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Argument 3:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Argument 4:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Argument 5:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Argument 6:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Argument 7:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Argument 8:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[MENUITEM][ARGUMENT 1]","Argument 1","","",0,60,0,NULL,NULL,"([UTILITIES][MENUITEM][STYLE]=0)",NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[MENUITEM][ARGUMENT 2]","Argument 2","","",0,60,0,NULL,NULL,"([UTILITIES][MENUITEM][STYLE]=0)",NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[MENUITEM][ARGUMENT 3]","Argument 3","","",0,60,0,NULL,NULL,"([UTILITIES][MENUITEM][STYLE]=0)",NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[MENUITEM][ARGUMENT 4]","Argument 4","","",0,60,0,NULL,NULL,"([UTILITIES][MENUITEM][STYLE]=0)",NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[MENUITEM][ARGUMENT 5]","Argument 5","","",0,60,0,NULL,NULL,"([UTILITIES][MENUITEM][STYLE]=0)",NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[MENUITEM][ARGUMENT 6]","Argument 6","","",0,60,0,NULL,NULL,"([UTILITIES][MENUITEM][STYLE]=0)",NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[MENUITEM][ARGUMENT 7]","Argument 7","","",0,60,0,NULL,NULL,"([UTILITIES][MENUITEM][STYLE]=0)",NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[MENUITEM][ARGUMENT 8]","Argument 8","","",0,60,0,NULL,NULL,"([UTILITIES][MENUITEM][STYLE]=0)",NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Visibility","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"[MENUITEM][VISIBLE EXPRESSION]","Visible Expression","","",5,80,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"MENUITEMS LOAD VALUE","Load Value","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"MENUITEMS LOAD GROUPER","Load Grouper","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"MENUITEMS LOAD OPERATOR","Load Operator","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"MENUITEMS LOAD CONTROL","Load Control","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","UI Metric Count:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[MENUITEM][UIMETRIC COUNT]","UI Metric Count:","","",0,8,0,NULL,"FALSE",NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Variable #1","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Environment Variable 1:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[MENUITEM][ENVIRONMENT NAME 1]","Name of Environment Variable 1","","",0,60,0,NULL,NULL,"([UTILITIES][MENUITEM][STYLE]=0)",NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"[MENUITEM][ENVIRONMENT VALUE 1]","Environment Variable Value 1","","",4,80,0,NULL,NULL,"([UTILITIES][MENUITEM][STYLE]=0)",NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Variable #2","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Environment Variable 2:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[MENUITEM][ENVIRONMENT NAME 2]","Environment Variable Name 2","","",0,60,0,NULL,NULL,"([UTILITIES][MENUITEM][STYLE]=0)",NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"[MENUITEM][ENVIRONMENT VALUE 2]","Environment Variable 2 Value","","",5,80,0,NULL,NULL,"([UTILITIES][MENUITEM][STYLE]=0)",NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Variable #3","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Environment Variable 3:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[MENUITEM][ENVIRONMENT NAME 3]","Name of Environment Variable 3","","",0,60,0,NULL,NULL,"([UTILITIES][MENUITEM][STYLE]=0)",NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"[MENUITEM][ENVIRONMENT VALUE 3]","Value of Environment Variable 3","","",5,80,0,NULL,NULL,"([UTILITIES][MENUITEM][STYLE]=0)",NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Variable #4","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Environment Variable 4:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[MENUITEM][ENVIRONMENT NAME 4]","Name of Environment Variable 4","","",0,60,0,NULL,NULL,"([UTILITIES][MENUITEM][STYLE]=0)",NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"[MENUITEM][ENVIRONMENT VALUE 4]","Value of Environment Variable 4","","",5,80,0,NULL,NULL,"([UTILITIES][MENUITEM][STYLE]=0)",NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,20,"","Variable #5","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Environment Variable 5:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"[MENUITEM][ENVIRONMENT NAME 5]","Name of Environment Variable 5","","",0,60,0,NULL,NULL,"([UTILITIES][MENUITEM][STYLE]=0)",NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"[MENUITEM][ENVIRONMENT VALUE 5]","Value of Environment Variable 5","","",5,80,0,NULL,NULL,"([UTILITIES][MENUITEM][STYLE]=0)",NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,21,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"BROWSE","Browse","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RESET DEFAULTS","Reset","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,23,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SAVE NO UPDATE","Fast Save","","",0,0,24,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(24+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [MENUITEM MAINTAIN SCREEN], Can Not Save Maintain Master!");
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
static void LST_MENUITEM_STYLES()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("UTILITIES","MENUITEM STYLES");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=1;
		ScrolledListDFincvir(list,"UTILITIES","MENUITEM","",0);
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
		list->contype=0;
		list->conmod=stralloc("UTILITIES");
		list->confil=stralloc("MENUITEM");
		list->confld=stralloc("STYLE");
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
		addAPPlib(list->list,"Action");
		addAPPlib(list->list,"Anchor");
		addAPPlib(list->list,"Heading");

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
			temp1=Rmalloc(15+9+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [UTILITIES] RDAScrolledList [MENUITEM STYLES], Can Not Save Scrolled List!");
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
static void VIR_MENUITEM_SAVE_ERROR()
{
	DFvirtual *virfld=NULL;
	char *temp1=NULL;

	virfld=DFvirtualNEW("UTILITIES","MENUITEM SAVE ERROR");
	if(virfld!=NULL)
	{
		virfld->vtype=0;
		virfld->type=14;
		DFvirtualDFincvir(virfld,"UTILITIES","MENUITEM","",0);
		virfld->avail2rsrc=1;
		virfld->type=14;
		virfld->length=0;
		virfld->when=0;
	temp1=Rmalloc(76+1);
	sprintf(temp1,"%s",
		"([MENUITEM SAVE ERROR 01]+[MENUITEM SAVE ERROR 02]+[MENUITEM SAVE ERROR 03])");
		virfld->expression=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		virfld->desc=stralloc("");

		if(writeDFvirtual(module,virfld))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(19+9+110+1);
			sprintf(temp1,"VIRTUAL FIELD WRITE ERROR: Module [UTILITIES] Virtual Field [MENUITEM SAVE ERROR], Can Not Save Virtual Field!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(virfld!=NULL) FreeDFvirtual(virfld);
	}
}
static void VIR_MENUITEM_SAVE_ERROR_01()
{
	DFvirtual *virfld=NULL;
	char *temp1=NULL;

	virfld=DFvirtualNEW("UTILITIES","MENUITEM SAVE ERROR 01");
	if(virfld!=NULL)
	{
		virfld->vtype=0;
		virfld->type=14;
		DFvirtualDFincvir(virfld,"UTILITIES","MENUITEM","",0);
		virfld->avail2rsrc=1;
		virfld->type=14;
		virfld->length=0;
		virfld->when=0;
	temp1=Rmalloc(397+1);
	sprintf(temp1,"%s%s",
		"IF(([UTILITIES][MENUITEM][DELETEFLAG]=FALSE) AND ([UTILITIES][MENUITEM][DROP DOWN LIST]=5) AND ([UTILITIES][MENUITEM][STYLE]!=0)) THEN RETURN_VALU",
		"E=\"Error 01:  Only Action Items are available in the Process Area.\" ELSE \nRETURN_vALUE=\"\" FI");
		virfld->expression=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		virfld->desc=stralloc("");

		if(writeDFvirtual(module,virfld))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+9+110+1);
			sprintf(temp1,"VIRTUAL FIELD WRITE ERROR: Module [UTILITIES] Virtual Field [MENUITEM SAVE ERROR 01], Can Not Save Virtual Field!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(virfld!=NULL) FreeDFvirtual(virfld);
	}
}
static void VIR_MENUITEM_SAVE_ERROR_02()
{
	DFvirtual *virfld=NULL;
	char *temp1=NULL;

	virfld=DFvirtualNEW("UTILITIES","MENUITEM SAVE ERROR 02");
	if(virfld!=NULL)
	{
		virfld->vtype=0;
		virfld->type=14;
		DFvirtualDFincvir(virfld,"UTILITIES","MENUITEM","",0);
		virfld->avail2rsrc=1;
		virfld->type=14;
		virfld->length=0;
		virfld->when=0;
	temp1=Rmalloc(393+1);
	sprintf(temp1,"%s%s",
		"IF(([UTILITIES][MENUITEM][DELETEFLAG]=FALSE) AND ([UTILITIES][MENUITEM][STYLE]=0) AND ([UTILITIES][MENUITEM][PROGRAM]=\"\")) THEN RETURN_VALUE=\"E",
		"rror 02: Action items require an associated program to execute.\" ELSE \nRETURN_vALUE=\"\" FI");
		virfld->expression=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		virfld->desc=stralloc("");

		if(writeDFvirtual(module,virfld))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+9+110+1);
			sprintf(temp1,"VIRTUAL FIELD WRITE ERROR: Module [UTILITIES] Virtual Field [MENUITEM SAVE ERROR 02], Can Not Save Virtual Field!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(virfld!=NULL) FreeDFvirtual(virfld);
	}
}
static void MTN_MTN_MENUITEM()
{
	char *defdir=NULL;
	RDAsearch *S=NULL;
	short r=0;
	MaintainMaster *MTNMSTR=NULL;
	APPlib *args=NULL,*args2=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;
	char *temp5=NULL,*temp6=NULL;

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(29+1);
	sprintf(temp2,"%s",
		"([MENUITEM SAVE ERROR]!=\"\")");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	temp4=Rmalloc(21+1);
	sprintf(temp4,"%s",
		"[MENUITEM SAVE ERROR]");
	temp5=Rmalloc(3+1);
	sprintf(temp5,"%s",
		"");
	temp6=Rmalloc(3+1);
	sprintf(temp6,"%s",
		"");

	MTNMSTR=RDAMaintainMasterNEW(module,"MTN MENUITEM",temp1,1,"MENUITEM","MENUITEM BROWSE","MENUITEM DEFINE LIST","MENUITEM SEARCH BROWSE","MENUITEM MAINTAIN SCREEN",0,1,0,0,temp2,temp3,temp5,temp4,temp6,TRUE,FALSE,FALSE,"");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
	if(temp5!=NULL) Rfree(temp5);
	if(temp6!=NULL) Rfree(temp6);
	if(MTNMSTR!=NULL)
	{
/* SUPPORTING FILES Beginning */

/* UTILITIES MENUITEM */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		r=addMaintainMasterSupportingADV(MTNMSTR,"UTILITIES","MENUITEM",1,0,NULL,"MENUITEM KEY 1",temp1,TRUE,"LOAD PARENT","SELECT PARENT BROWSE","SELECT PARENT DEFINE LIST","SELECT PARENT SEARCH BROWSE",0,"UTILITIES","MENUITEM");
		if(temp1!=NULL) Rfree(temp1);
		if(r>0)
		{
			S=MTNMSTR->s+(r-1);
			addSupportingRDAconfld(S,"MODULE NAME","[MENUITEM][MODULE NAME]",TRUE);
			addSupportingRDAconfld(S,"ITEM NUMBER","[MENUITEM][PARENT]",FALSE);
		}


/* SUPPORTING FILES Ending */




		SetRDAsearchOccurrence(MTNMSTR);


		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.MTN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.MTN",CURRENTDIRECTORY,module);
#endif
		if(writeMaintainMaster(defdir,MTNMSTR))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(12+9+89+1);
			sprintf(temp1,"MAINTAIN MTNMASTER WRITE ERROR: Module [UTILITIES] Maintain Master [MTN MENUITEM], Can Not Save Maintain Master!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(defdir!=NULL) Rfree(defdir);
			if(args!=NULL) freeapplib(args);
			if(args2!=NULL) freeapplib(args2);
			if(MTNMSTR!=NULL) FreeMaintainMaster(MTNMSTR);
		} else {
			if(defdir!=NULL) Rfree(defdir);
			if(args!=NULL) freeapplib(args);
			if(args2!=NULL) freeapplib(args2);
			if(MTNMSTR!=NULL) FreeMaintainMaster(MTNMSTR);
		}
	}
}
static void SCN_SELECT_PARENT_BROWSE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","SELECT PARENT BROWSE");
	if(scrn!=NULL)
	{
		temp1=Rmalloc(23+1);
		sprintf(temp1,"%s",
			"[EXTENSIONS SETUP]=TRUE");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFINE LIST","Define List","","",0,0,15,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SEARCH BUTTON","Search","","",0,0,12,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"PRINT BROWSE","Print Browse","","",0,0,7,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"TOP","Top","","",0,0,41,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"BOTTOM","Bottom","","",0,0,42,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SEARCH ALL","Search All","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"CLEAR SEARCH & LPM","Clear (Search & LPM)","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","LPM:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"BROWSE LIST LPM","Lexical Pattern Match:","","",0,20,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Search Field:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SEARCH FIELD","Search Field","","",0,30,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"BROWSE LIST","Browse List","","",10,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Position","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"BROWSE LIST POSITION","Position","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Count","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DELETEFLAG TOTAL","Count","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,36,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(20+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [SELECT PARENT BROWSE], Can Not Save Maintain Master!");
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
static void SCN_SELECT_PARENT_DEFINE_LIST()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","SELECT PARENT DEFINE LIST");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Enter a position and length for each of the following:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,12,"","","","",300,500,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Argument 1:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Argument 2:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Argument 3:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Argument 4:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Argument 5:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Deleteflag:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Drop Down List:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Environment Name 1:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Environment Name 2:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Environment Name 3:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Environment Name 4:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Environment Name 5:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Environment Value 1:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Environment Value 2:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Environment Value 3:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Environment Value 4:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Environment Value 5:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Item Number:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Module Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Parent:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Program:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Rank:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Style:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Visible Expression:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 1 POSITION","Argument 1 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 1 LENGTH","Argument 1 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 2 POSITION","Argument 2 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 2 LENGTH","Argument 2 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 3 POSITION","Argument 3 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 3 LENGTH","Argument 3 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 4 POSITION","Argument 4 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 4 LENGTH","Argument 4 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 5 POSITION","Argument 5 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ARGUMENT 5 LENGTH","Argument 5 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DELETEFLAG POSITION","Deleteflag Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DELETEFLAG LENGTH","Deleteflag Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DROP DOWN LIST POSITION","Drop Down List Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DROP DOWN LIST LENGTH","Drop Down List Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT NAME 1 POSITION","Environment Name 1 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT NAME 1 LENGTH","Environment Name 1 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT NAME 2 POSITION","Environment Name 2 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT NAME 2 LENGTH","Environment Name 2 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT NAME 3 POSITION","Environment Name 3 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT NAME 3 LENGTH","Environment Name 3 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT NAME 4 POSITION","Environment Name 4 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT NAME 4 LENGTH","Environment Name 4 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT NAME 5 POSITION","Environment Name 5 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT NAME 5 LENGTH","Environment Name 5 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT VALUE 1 POSITION","Environment Value 1 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT VALUE 1 LENGTH","Environment Value 1 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT VALUE 2 POSITION","Environment Value 2 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT VALUE 2 LENGTH","Environment Value 2 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT VALUE 3 POSITION","Environment Value 3 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT VALUE 3 LENGTH","Environment Value 3 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT VALUE 4 POSITION","Environment Value 4 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT VALUE 4 LENGTH","Environment Value 4 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT VALUE 5 POSITION","Environment Value 5 Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ENVIRONMENT VALUE 5 LENGTH","Environment Value 5 Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ITEM NUMBER POSITION","Item Number Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"ITEM NUMBER LENGTH","Item Number Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"MODULE NAME POSITION","Module Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"MODULE NAME LENGTH","Module Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"NAME POSITION","Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"NAME LENGTH","Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"PARENT POSITION","Parent Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"PARENT LENGTH","Parent Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"PROGRAM POSITION","Program Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"PROGRAM LENGTH","Program Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"RANK POSITION","Rank Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"RANK LENGTH","Rank Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"STYLE POSITION","Style Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"STYLE LENGTH","Style Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"VISIBLE EXPRESSION POSITION","Visible Expression Position","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"VISIBLE EXPRESSION LENGTH","Visible Expression Length","","",0,3,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,13,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,15,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,17,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(25+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [SELECT PARENT DEFINE LIST], Can Not Save Maintain Master!");
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
static void SCN_SELECT_PARENT_SEARCH_BROWSE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","SELECT PARENT SEARCH BROWSE");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,12,"","","","",350,791,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [MENUITEM][DELETEFLAG] FALSE","Select when Deleteflag False","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"SELECT [MENUITEM][DELETEFLAG] TRUE","Select when Deleteflag True","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","SEARCH CRITERIA","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","FROM","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","TO","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 1]","Range on Argument 1","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 1]","From Argument 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 1]","To Argument 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 2]","Range on Argument 2","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 2]","From Argument 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 2]","To Argument 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 3]","Range on Argument 3","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 3]","From Argument 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 3]","To Argument 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 4]","Range on Argument 4","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 4]","From Argument 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 4]","To Argument 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ARGUMENT 5]","Range on Argument 5","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ARGUMENT 5]","From Argument 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ARGUMENT 5]","To Argument 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][DROP DOWN LIST]","Range on Drop Down List","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][DROP DOWN LIST]","From Drop Down List","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][DROP DOWN LIST]","To Drop Down List","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 1]","Range on Environment Name 1","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 1]","From Environment Name 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 1]","To Environment Name 1","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 2]","Range on Environment Name 2","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 2]","From Environment Name 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 2]","To Environment Name 2","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 3]","Range on Environment Name 3","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 3]","From Environment Name 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 3]","To Environment Name 3","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 4]","Range on Environment Name 4","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 4]","From Environment Name 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 4]","To Environment Name 4","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ENVIRONMENT NAME 5]","Range on Environment Name 5","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ENVIRONMENT NAME 5]","From Environment Name 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ENVIRONMENT NAME 5]","To Environment Name 5","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][ITEM NUMBER]","Range on Item Number","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][ITEM NUMBER]","From Item Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][ITEM NUMBER]","To Item Number","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][MODULE NAME]","Range on Module Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][MODULE NAME]","From Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][MODULE NAME]","To Module Name","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][NAME]","Range on Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][NAME]","From Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][NAME]","To Name","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][PARENT]","Range on Parent","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][PARENT]","From Parent","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][PARENT]","To Parent","","",0,8,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][PROGRAM]","Range on Program","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][PROGRAM]","From Program","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][PROGRAM]","To Program","","",0,34,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][RANK]","Range on Rank","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][RANK]","From Rank","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][RANK]","To Rank","","",0,4,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [MENUITEM][STYLE]","Range on Style","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [MENUITEM][STYLE]","From Style","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [MENUITEM][STYLE]","To Style","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,13,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Query:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"QUERY","Query","","",0,60,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,12,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,14,"SELECT PARENT SEARCH BROWSE EXPRESSION","Expression","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RESET GROUP DEFAULTS","Reset Group Defaults","","",0,0,18,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,17,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(27+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [SELECT PARENT SEARCH BROWSE], Can Not Save Maintain Master!");
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
static void SCN_SELECT_PARENT_SEARCH_BROWSE_EXPRESSION()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","SELECT PARENT SEARCH BROWSE EXPRESSION");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,8,"EXPRESSION","Expression","","",5,80,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD VALUE","Load Value","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD GROUPER","Load Grouper","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD OPERATOR","Load Operator","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"LOAD CONTROL","Load Control","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,11,"","","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(38+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [SELECT PARENT SEARCH BROWSE EXPRESSION], Can Not Save Maintain Master!");
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
static void LST_MENUITEM_PROGRAMS()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("UTILITIES","MENUITEM PROGRAMS");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=1;
		ScrolledListDFincvir(list,"UTILITIES","MENUITEM","",0);
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
		list->contype=0;
		list->conmod=stralloc("UTILITIES");
		list->confil=stralloc("MENUITEM");
		list->confld=stralloc("PROGRAM");
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
		addAPPlib(list->list,"APPAI");
		addAPPlib(list->list,"APPPI");
		addAPPlib(list->list,"APPSI");
		addAPPlib(list->list,"APPSAA");
		addAPPlib(list->list,"APPSPA");
		addAPPlib(list->list,"APPSSA");
		addAPPlib(list->list,"APPSTP");
		addAPPlib(list->list,"APPHIRE");
		addAPPlib(list->list,"BUDSEL");
		addAPPlib(list->list,"BUDNET");
		addAPPlib(list->list,"BUDMTS");
		addAPPlib(list->list,"BUDUNMTS");
		addAPPlib(list->list,"CHARTDAT");
		addAPPlib(list->list,"CHECKEMAIL");
		addAPPlib(list->list,"CSHAIDC");
		addAPPlib(list->list,"RUNPOWERADD");
		addAPPlib(list->list,"MULTIPOWERADD");
		addAPPlib(list->list,"RUNMAINTAIN");
		addAPPlib(list->list,"RUNREPORT");
		addAPPlib(list->list,"DFACCT");
		addAPPlib(list->list,"DFADD");
		addAPPlib(list->list,"DFCHG");
		addAPPlib(list->list,"DFGSV");
		addAPPlib(list->list,"DFHELP");
		addAPPlib(list->list,"DFIE");
		addAPPlib(list->list,"DFLIST");
		addAPPlib(list->list,"DFLOAD");
		addAPPlib(list->list,"DFMATRIX");
		addAPPlib(list->list,"DFMENU");
		addAPPlib(list->list,"DFMTN");
		addAPPlib(list->list,"DFNRD");
		addAPPlib(list->list,"DFPRT");
		addAPPlib(list->list,"SETPRT");
		addAPPlib(list->list,"DFRPT");
		addAPPlib(list->list,"DFSCN");
		addAPPlib(list->list,"DFSCNDEF");
		addAPPlib(list->list,"DFTABLE");
		addAPPlib(list->list,"DFVIR");
		addAPPlib(list->list,"DFWWW");
		addAPPlib(list->list,"DISPLAYTAPE");
		addAPPlib(list->list,"DOARCHIVE");
		addAPPlib(list->list,"DOBACKUP");
		addAPPlib(list->list,"DOGSV");
		addAPPlib(list->list,"DOCHG");
		addAPPlib(list->list,"DOFTPGET");
		addAPPlib(list->list,"DOFTPZIP");
		addAPPlib(list->list,"DOHTTP");
		addAPPlib(list->list,"DOWIKI");
		addAPPlib(list->list,"DOIE");
		addAPPlib(list->list,"DOREPORT");
		addAPPlib(list->list,"DORESTORE");
		addAPPlib(list->list,"EZRPT");
		addAPPlib(list->list,"FINSTP");
		addAPPlib(list->list,"FINCOA");
		addAPPlib(list->list,"LASER");
		addAPPlib(list->list,"GETFORMS");
		addAPPlib(list->list,"LVEASS");
		addAPPlib(list->list,"MISCSSTM");
		addAPPlib(list->list,"MK1099");
		addAPPlib(list->list,"MKAPPMGT");
		addAPPlib(list->list,"MKARCHIVE");
		addAPPlib(list->list,"MKBFTMGT");
		addAPPlib(list->list,"MKBLDPRMT");
		addAPPlib(list->list,"MKBNKREC");
		addAPPlib(list->list,"MKBUDPREP");
		addAPPlib(list->list,"MKCATALOGUE");
		addAPPlib(list->list,"MKCSHDWR");
		addAPPlib(list->list,"MKDAT");
		addAPPlib(list->list,"MKDMV");
		addAPPlib(list->list,"MKEMPABS");
		addAPPlib(list->list,"MKFINMGT");
		addAPPlib(list->list,"MKFIXASS");
		addAPPlib(list->list,"MKGUI");
		addAPPlib(list->list,"MKLVEMGT");
		addAPPlib(list->list,"MKMISCBILL");
		addAPPlib(list->list,"MKOCCTAX");
		addAPPlib(list->list,"MKOLH");
		addAPPlib(list->list,"MKOPENSS");
		addAPPlib(list->list,"MKPAYROLL");
		addAPPlib(list->list,"MKPOSTRK");
		addAPPlib(list->list,"MKPROPERTY");
		addAPPlib(list->list,"MKPRSNNL");
		addAPPlib(list->list,"MKPURORD");
		addAPPlib(list->list,"MKREQMGT");
		addAPPlib(list->list,"MKINVCTL");
		addAPPlib(list->list,"MKRLSTMGT");
		addAPPlib(list->list,"MKRPT");
		addAPPlib(list->list,"MKSEC");
		addAPPlib(list->list,"MKSUBMGT");
		addAPPlib(list->list,"MKUTLBLL");
		addAPPlib(list->list,"MKUTL");
		addAPPlib(list->list,"MKVENPMT");
		addAPPlib(list->list,"MKVLTN");
		addAPPlib(list->list,"MKW2");
		addAPPlib(list->list,"OCCCGL");
		addAPPlib(list->list,"PAYAVW");
		addAPPlib(list->list,"PAYBC");
		addAPPlib(list->list,"PAYCHECK");
		addAPPlib(list->list,"PAYCHECK1");
		addAPPlib(list->list,"PAYCHECK2");
		addAPPlib(list->list,"PAYCHECKA");
		addAPPlib(list->list,"PAYCHECK1A");
		addAPPlib(list->list,"PAYCHECK2A");
		addAPPlib(list->list,"PAYCHECKTXT");
		addAPPlib(list->list,"DIRDEP");
		addAPPlib(list->list,"DIRDEP1");
		addAPPlib(list->list,"DIRDEP2");
		addAPPlib(list->list,"DIRDEPA");
		addAPPlib(list->list,"DIRDEP1A");
		addAPPlib(list->list,"DIRDEP2A");
		addAPPlib(list->list,"DIRDEPFORM");
		addAPPlib(list->list,"DIRDEPFORMA");
		addAPPlib(list->list,"DIRDEPTXT");
		addAPPlib(list->list,"PAYJMST");
		addAPPlib(list->list,"PAYMTS");
		addAPPlib(list->list,"PAYNET");
		addAPPlib(list->list,"PAYSCHK");
		addAPPlib(list->list,"PAYSEL");
		addAPPlib(list->list,"SUBCSUB");
		addAPPlib(list->list,"SUBSEL");
		addAPPlib(list->list,"PAYSDDN");
		addAPPlib(list->list,"PAYSTP");
		addAPPlib(list->list,"PAYTERM");
		addAPPlib(list->list,"PAYUNMTS");
		addAPPlib(list->list,"PAYVSCHK");
		addAPPlib(list->list,"PAYVSETR");
		addAPPlib(list->list,"PAYVWCHECKDS");
		addAPPlib(list->list,"REQADD");
		addAPPlib(list->list,"REQAPP");
		addAPPlib(list->list,"REQCPO");
		addAPPlib(list->list,"REQCHG");
		addAPPlib(list->list,"PRTAPO");
		addAPPlib(list->list,"PRTAPOA");
		addAPPlib(list->list,"PURAPV");
		addAPPlib(list->list,"PURAPVGUSD");
		addAPPlib(list->list,"PURCPO");
		addAPPlib(list->list,"PURAREC");
		addAPPlib(list->list,"PLAINPAPER");
		addAPPlib(list->list,"RBDFIL");
		addAPPlib(list->list,"RDAOUT");
		addAPPlib(list->list,"REMDEL");
		addAPPlib(list->list,"REMTRANS");
		addAPPlib(list->list,"REMTRANS");
		addAPPlib(list->list,"RESTRUCT");
		addAPPlib(list->list,"RNUMTRAN");
		addAPPlib(list->list,"VENCHECKDS");
		addAPPlib(list->list,"VENSCHK");
		addAPPlib(list->list,"DQUERY");
		addAPPlib(list->list,"VENSETR");
		addAPPlib(list->list,"VENSTP");
		addAPPlib(list->list,"VIEWTRAN");
		addAPPlib(list->list,"XPERTSTP");
		addAPPlib(list->list,"MASSDEL");
		addAPPlib(list->list,"ADDMENU");
		addAPPlib(list->list,"MASSREMDEL");
		addAPPlib(list->list,"CHECKFILE");
		addAPPlib(list->list,"CRTIMPEXP");
		addAPPlib(list->list,"MASSIE");
		addAPPlib(list->list,"CHGENGINE");
		addAPPlib(list->list,"MASSDIFF");
		addAPPlib(list->list,"RDADIAG");
		addAPPlib(list->list,"MTNMST");
		addAPPlib(list->list,"DOREPORT");
		addAPPlib(list->list,"DOADD");
		addAPPlib(list->list,"DOADDS");
		addAPPlib(list->list,"DOMENU");
		addAPPlib(list->list,"VENCHECKMICR");
		addAPPlib(list->list,"VENCHECKMICRA");
		addAPPlib(list->list,"PAYVWCHECKMICR");
		addAPPlib(list->list,"PAYVWCHECKMICRA");
		addAPPlib(list->list,"PAYCHECKMICR");
		addAPPlib(list->list,"PAYCHECKMICRA");
		addAPPlib(list->list,"RUNSCRIPT");
		addAPPlib(list->list,"COMPRLST");
		addAPPlib(list->list,"PRPSTT");
		addAPPlib(list->list,"PROGUPD");
		addAPPlib(list->list,"RESTSSTM");
		SORTAPPlib(list->list);

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
			temp1=Rmalloc(17+9+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [UTILITIES] RDAScrolledList [MENUITEM PROGRAMS], Can Not Save Scrolled List!");
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
static void VIR_MENUITEM_SAVE_ERROR_03()
{
	DFvirtual *virfld=NULL;
	char *temp1=NULL;

	virfld=DFvirtualNEW("UTILITIES","MENUITEM SAVE ERROR 03");
	if(virfld!=NULL)
	{
		virfld->vtype=0;
		virfld->type=14;
		DFvirtualDFincvir(virfld,"UTILITIES","MENUITEM","",0);
		virfld->avail2rsrc=1;
		virfld->type=14;
		virfld->length=0;
		virfld->when=0;
	temp1=Rmalloc(493+1);
	sprintf(temp1,"%s%s",
		"IF(([UTILITIES][MENUITEM][DELETEFLAG]=FALSE) AND ([UTILITIES][MENUITEM][RANK]=0) AND ([UTILITIES][MENUITEM][DROP DOWN LIST]!=5)) THEN RETURN_VALUE",
		"=\"Error 03: Items belonging on drop down lists require a Rank.\" ELSE \nRETURN_vALUE=\"\" FI");
		virfld->expression=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		virfld->desc=stralloc("");

		if(writeDFvirtual(module,virfld))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(22+9+110+1);
			sprintf(temp1,"VIRTUAL FIELD WRITE ERROR: Module [UTILITIES] Virtual Field [MENUITEM SAVE ERROR 03], Can Not Save Virtual Field!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(virfld!=NULL) FreeDFvirtual(virfld);
	}
}
static void DEF_MENUITEM_DEFINE_LIST()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	def=RDAdefaultNEW("UTILITIES","MENUITEM DEFINE LIST");
	if(def!=NULL)
	{
		def->input_focus=stralloc("DEFAULTS");
		addDefaultFieldINT(def,"KEY LIST",TRUE,13,0);
		addDefaultFieldSHORT(def,"MODULE NAME POSITION",TRUE,7,2);
		addDefaultFieldSHORT(def,"MODULE NAME LENGTH",TRUE,7,15);
		addDefaultFieldSHORT(def,"ITEM NUMBER POSITION",TRUE,7,3);
		addDefaultFieldSHORT(def,"ITEM NUMBER LENGTH",TRUE,7,8);
		addDefaultFieldSHORT(def,"ARGUMENT 1 POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"ARGUMENT 1 LENGTH",TRUE,7,40);
		addDefaultFieldSHORT(def,"ARGUMENT 2 POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"ARGUMENT 2 LENGTH",TRUE,7,40);
		addDefaultFieldSHORT(def,"ARGUMENT 3 POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"ARGUMENT 3 LENGTH",TRUE,7,40);
		addDefaultFieldSHORT(def,"ARGUMENT 4 POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"ARGUMENT 4 LENGTH",TRUE,7,40);
		addDefaultFieldSHORT(def,"ARGUMENT 5 POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"ARGUMENT 5 LENGTH",TRUE,7,40);
		addDefaultFieldSHORT(def,"DELETEFLAG POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"DELETEFLAG LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"DROP DOWN LIST POSITION",TRUE,7,5);
		addDefaultFieldSHORT(def,"DROP DOWN LIST LENGTH",TRUE,7,2);
		addDefaultFieldSHORT(def,"ENVIRONMENT NAME 1 POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"ENVIRONMENT NAME 1 LENGTH",TRUE,7,20);
		addDefaultFieldSHORT(def,"ENVIRONMENT NAME 2 POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"ENVIRONMENT NAME 2 LENGTH",TRUE,7,20);
		addDefaultFieldSHORT(def,"ENVIRONMENT NAME 3 POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"ENVIRONMENT NAME 3 LENGTH",TRUE,7,20);
		addDefaultFieldSHORT(def,"ENVIRONMENT NAME 4 POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"ENVIRONMENT NAME 4 LENGTH",TRUE,7,20);
		addDefaultFieldSHORT(def,"ENVIRONMENT NAME 5 POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"ENVIRONMENT NAME 5 LENGTH",TRUE,7,20);
		addDefaultFieldSHORT(def,"NAME POSITION",TRUE,7,6);
		addDefaultFieldSHORT(def,"NAME LENGTH",TRUE,7,40);
		addDefaultFieldSHORT(def,"PARENT POSITION",TRUE,7,4);
		addDefaultFieldSHORT(def,"PARENT LENGTH",TRUE,7,8);
		addDefaultFieldSHORT(def,"PROGRAM POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"PROGRAM LENGTH",TRUE,7,40);
		addDefaultFieldSHORT(def,"RANK POSITION",TRUE,7,5);
		addDefaultFieldSHORT(def,"RANK LENGTH",TRUE,7,4);
		addDefaultFieldSHORT(def,"STYLE POSITION",TRUE,7,7);
		addDefaultFieldSHORT(def,"STYLE LENGTH",TRUE,7,2);
		addDefaultFieldSHORT(def,"ENVIRONMENT VALUE 1 POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"ENVIRONMENT VALUE 1 LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"ENVIRONMENT VALUE 2 POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"ENVIRONMENT VALUE 2 LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"ENVIRONMENT VALUE 3 POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"ENVIRONMENT VALUE 3 LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"ENVIRONMENT VALUE 4 POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"ENVIRONMENT VALUE 4 LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"ENVIRONMENT VALUE 5 POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"ENVIRONMENT VALUE 5 LENGTH",TRUE,7,0);
		addDefaultFieldSHORT(def,"VISIBLE EXPRESSION POSITION",TRUE,7,0);
		addDefaultFieldSHORT(def,"VISIBLE EXPRESSION LENGTH",TRUE,7,0);

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
			temp1=Rmalloc(20+9+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [UTILITIES] Screen [MENUITEM DEFINE LIST], Can Not Save Screen Defaults!");
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
static void RLV_MENUITEMS()
{
	RDALoadValue *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	scrn=RDALoadValueNEW("UTILITIES","MENUITEMS");
	if(scrn!=NULL)
	{
		RDALoadValueDFincvir(scrn,"UTILITIES","MENUITEM","",0);
		scrn->conname=stralloc("[MENUITEM][VISIBLE EXPRESSION]");
		scrn->desc=stralloc("");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.LDV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.LDV",CURRENTDIRECTORY,module);
#endif
		if(writeRDALoadValuebin(defdir,scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(9+9+100+1);
			sprintf(temp1,"LOAD VALUE WRITE ERROR: Module [UTILITIES] RDALoadValue [MENUITEMS], Can Not Save Load Value!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) FreeRDALoadValue(scrn);
	}
}
static void MTN_MTN_MENUITEM_SEARCH()
{
	char *defdir=NULL;
	RDAsearch *S=NULL;
	short r=0;
	MaintainMaster *MTNMSTR=NULL;
	APPlib *args=NULL,*args2=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;
	char *temp5=NULL,*temp6=NULL;

	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
	temp2=Rmalloc(29+1);
	sprintf(temp2,"%s",
		"([MENUITEM SAVE ERROR]!=\"\")");
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");
	temp4=Rmalloc(21+1);
	sprintf(temp4,"%s",
		"[MENUITEM SAVE ERROR]");
	temp5=Rmalloc(3+1);
	sprintf(temp5,"%s",
		"");
	temp6=Rmalloc(3+1);
	sprintf(temp6,"%s",
		"");

	MTNMSTR=RDAMaintainMasterNEW(module,"MTN MENUITEM.SEARCH",temp1,1,"MENUITEM","MENUITEM BROWSE","MENUITEM DEFINE LIST","MENUITEM SEARCH BROWSE","MENUITEM MAINTAIN SCREEN",0,2,0,0,temp2,temp3,temp5,temp4,temp6,TRUE,FALSE,FALSE,"");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
	if(temp5!=NULL) Rfree(temp5);
	if(temp6!=NULL) Rfree(temp6);
	if(MTNMSTR!=NULL)
	{
/* SUPPORTING FILES Beginning */

/* UTILITIES MENUITEM */

		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		r=addMaintainMasterSupportingADV(MTNMSTR,"UTILITIES","MENUITEM",1,0,NULL,"MENUITEM KEY 1",temp1,TRUE,"LOAD PARENT","SELECT PARENT BROWSE","SELECT PARENT DEFINE LIST","SELECT PARENT SEARCH BROWSE",0,"UTILITIES","MENUITEM");
		if(temp1!=NULL) Rfree(temp1);
		if(r>0)
		{
			S=MTNMSTR->s+(r-1);
			addSupportingRDAconfld(S,"MODULE NAME","[MENUITEM][MODULE NAME]",TRUE);
			addSupportingRDAconfld(S,"ITEM NUMBER","[MENUITEM][PARENT]",FALSE);
		}


/* SUPPORTING FILES Ending */




		SetRDAsearchOccurrence(MTNMSTR);


		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.MTN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.MTN",CURRENTDIRECTORY,module);
#endif
		if(writeMaintainMaster(defdir,MTNMSTR))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(19+9+89+1);
			sprintf(temp1,"MAINTAIN MTNMASTER WRITE ERROR: Module [UTILITIES] Maintain Master [MTN MENUITEM.SEARCH], Can Not Save Maintain Master!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(defdir!=NULL) Rfree(defdir);
			if(args!=NULL) freeapplib(args);
			if(args2!=NULL) freeapplib(args2);
			if(MTNMSTR!=NULL) FreeMaintainMaster(MTNMSTR);
		} else {
			if(defdir!=NULL) Rfree(defdir);
			if(args!=NULL) freeapplib(args);
			if(args2!=NULL) freeapplib(args2);
			if(MTNMSTR!=NULL) FreeMaintainMaster(MTNMSTR);
		}
	}
}
void MTNMENUITEM()
{
	MTN_MTN_MENUITEM();
	MNU_UTILITIES_MTN_MENUITEM();
	LST_DROP_DOWN_LISTS();
	LST_MENUITEM_MODULES();
	SCN_MENUITEM_BROWSE();
	SCN_MENUITEM_DEFINE_LIST();
	SCN_MENUITEM_SEARCH_BROWSE();
	SCN_MENUITEM_SEARCH_BROWSE_EXPRESSION();
	SCN_MENUITEM_MAINTAIN_SCREEN();
	LST_MENUITEM_STYLES();
	VIR_MENUITEM_SAVE_ERROR();
	VIR_MENUITEM_SAVE_ERROR_01();
	VIR_MENUITEM_SAVE_ERROR_02();
	VIR_MENUITEM_SAVE_ERROR_03();
	SCN_SELECT_PARENT_BROWSE();
	SCN_SELECT_PARENT_DEFINE_LIST();
	SCN_SELECT_PARENT_SEARCH_BROWSE();
	SCN_SELECT_PARENT_SEARCH_BROWSE_EXPRESSION();
	LST_MENUITEM_PROGRAMS();
	DEF_MENUITEM_DEFINE_LIST();
	MTN_MTN_MENUITEM_SEARCH();
	RLV_MENUITEMS();
}
