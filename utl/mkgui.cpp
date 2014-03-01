#ifndef WIN32
#define __NAM__ "mkgui.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mkgui.exe"
#endif
#include <app.hpp>
#include <mix.hpp>
#include <mkmsc.hpp>
#include <mkguis.hpp>

#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */

/*CPP_OPART mkwww1 */
/*CPP_OPART mkgui1 */
/*CPP_OPART mkgui3 */
/*CPP_OPART mkgui4 */
/*CPP_OPART mkgui5 */

/*ADDLIB mkmisc */
/*ADDLIB mklib */
/*ADDLIB add */
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="GUI";
APPlib *scnlist=NULL;
APPlib *errorlist=NULL;
void print_list(RDArsrc *,void (*)(...));
static void make_screens(int,char **);
static void MAKE_GUI_SCREENS(void);

struct RPTSTRUCTs
{
        char *name;
        char *desc;
        void (*func)(...);
};

static void mk_dfscn(void),mk_dflist(void),mk_dfmenu(void),mk_dfacct(void);
static void mk_dfscndef(void),mk_convscrn(void),mk_prtbrowse(void);
static void mk_diagscrn(void),mk_errwarn(void),mk_convmenu(void);
static void mk_textbrowser(void);
static void mk_dfload(void);
extern void MAKE_CUSTOM_INPUTS(void),DB_AUTOCOMP(void);
extern void mk_dfwww(void),MTN_AUTOCOMP(void);
extern void ADD_MENU(void),SETCOOKIEX(void);

struct RPTSTRUCTs RptAvl[]=
{
	{"DB_AUTOCOMP        ","Auto Complete Database",DB_AUTOCOMP},
	{"MTN_AUTOCMOP       ","Maintain Auto Complete",MTN_AUTOCOMP},
	{"ERRWARN            ","Error, Warning, and Load Screens ",mk_errwarn},
	{"DIAGSCRN           ","Diagnostic Screen                ",mk_diagscrn},
	{"DFSCN              ","Define Screens                   ",mk_dfscn},
	{"DFLIST             ","Define List Screens              ",mk_dflist},
	{"DFMENU             ","Define Menu Screens              ",mk_dfmenu},
	{"DFACCT             ","Define Accounts Screens          ",mk_dfacct},
	{"DFSCNDEF           ","Define Screen Defaults           ",mk_dfscndef},
	{"DFLOAD             ","Define Load Value                ",mk_dfload},
	{"CONVSCRN           ","Convert Screens                  ",mk_convscrn},
	{"CONVMENU           ","Convert Menu Screens             ",mk_convmenu},
	{"PRTBROWSE          ","Print Browse Screen              ",mk_prtbrowse},
	{"SETCOOKIE          ","Set Cookie                       ",SETCOOKIEX},
	{"TXTBROWSE          ","Print Text Browser Screen        ",mk_textbrowser},
	{"CUSTOM INPUTS      ","Create Standard Custom Inputs    ",MAKE_CUSTOM_INPUTS},
	{"ADD MENU OPTION    ","Add Menu Option",ADD_MENU},
	{"DFWWW              ","Define WWW                   ",mk_dfwww},
};
static void mk_dfscn()
{
	DEFINE_SCREENS();
	MAINTAIN_SCREEN();
	EDIT_SCREEN_WIDGET();	
	PRINT_SCREEN();
	DEFINE_SCREENS_MENU();
	WIDGET_LIST();
}
static void mk_dflist()
{
	DEFINE_SCROLLED_LISTS();
	MAINTAIN_SCROLLED_LIST();
	MAINTAIN_SCROLLED_LIST_CONNECT();
	MAINTAIN_SCROLLED_LIST_TYPE();
	MAINTAIN_ASSOCIATED_FILES();
	MAINTAIN_SCROLLED_LIST_ASSOCIATED_FILES();
	MAINTAIN_LIST_ELEMENTS();
	MAINTAIN_SCROLLED_LIST_ELEMENTS();
	MAINTAIN_SCROLLED_LIST_SELECT();
	MAINTAIN_SCROLLED_LIST_FORMAT();
	DEFINE_SCROLLED_LISTS_MENU();
	MAINTAIN_COMPUTED_LIST_ELEMENTS();
}
static void mk_dfmenu()
{
	DEFINE_MENU();
	MAINTAIN_MENU();
	DEFINE_MENU_MENU();
}
static void mk_dfacct()
{
	DEFINE_ACCOUNTS();
	MAINTAIN_ACCOUNTS();
	MAINTAIN_ACCOUNT_DIMENSION();
	DEFINE_ACCOUNTS_MENU();
}
static void mk_dfscndef()
{
	DEFINE_SCREEN_DEFAULTS();
	MAINTAIN_SCREEN_DEFAULTS();
	MAINTAIN_RESOURCE_DEFAULTS();
	DEFINE_SCREEN_DEFAULTS_MENU();
}
static void MNU_DEFINE_LOAD()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("DEFINE LOAD","dfload.EXT");
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
static void mk_dfload()
{
	MNU_DEFINE_LOAD();
	DEFINE_LOAD_VALUES();
	MAINTAIN_LOAD_VALUE();
	MAINTAIN_LOAD_VALUE_ASSOCIATED_FILES();
	MAINTAIN_LOAD_VALUE_ELEMENTS();
}
static void mk_convscrn()
{
	if(XPERT_SETUP->TOOLS==TRUE)
	{
		CONVERT_SCREENS();
		CONVERT_SCREENS_MENU();
	}
}
static void mk_convmenu()
{
	if(XPERT_SETUP->TOOLS==TRUE)
	{
		CONVERT_MENU();
		CONVERT_MENUS_MENU();
	}
}
static void mk_prtbrowse()
{
	PRINT_BROWSE_SCREEN();
}
static void mk_diagscrn()
{
	GUI_DIAGNOSTIC_SCREEN();
}
static void mk_errwarn()
{
	ABORT_WARNING_DIALOG_SCREEN();
	DELETE_WARNING_DIALOG_SCREEN();
	ERROR_DIALOG_SCREEN();
	SAVE_WARNING_DIALOG_SCREEN();
	QUIT_WARNING_DIALOG_SCREEN();
	WARNING_DIALOG_SCREEN();
	LOAD_CONTROL_SCREEN();
	LOAD_GROUPER_SCREEN();
	LOAD_OPERATOR_SCREEN();
	LOAD_VALUE_SCREEN();
	SAVE_SCREEN_DEFAULTS();
}
static void quit_print(RDArsrc *prsrc)
{
	if(diaggui)
	{
		prterr("DIAG quit_print Quitting Print Screen Definition.");
	}
	if(prsrc!=NULL) free_rsrc(prsrc);
}
static void mk_textbrowser()
{
	TEXT_BROWSER();
	SAVE_SCREEN();
}

static void printerrorlist(RDArsrc *parent,RDA_PFILE *fp)
{
	int x;
	char *date=NULL,*timex=NULL;

	if(fp!=NULL)
	{
		date=GETCURRENTDATE();
		timex=GETCURRENTTIME();
		RDA_fprintf(fp,"                     XPERT MAKE GUI SCREENS ERROR LIST          Date: %*s\n",8,date);
			RDA_fprintf(fp,"%*sExecuted By: %s%*sTime: %s\n\n",
			 ((80-(14+RDAstrlen(USERLOGIN)))/2),
			"",USERLOGIN,
			(((80-(14+RDAstrlen(USERLOGIN)))/2)-14),"",
			timex);
		if(date!=NULL) Rfree(date);
		if(timex!=NULL) Rfree(timex);
		for(x=0;x<errorlist->numlibs;++x)
		{
			RDA_fprintf(fp,"(%5d) %s\n",(x+1),errorlist->libs[x]);
		}
		RDA_fprintf(fp,"\f");
	}
}

void printerrorlistcb(RDArsrc *prsrc,RDArsrc *parent)
{
	char *outdevice=NULL,*message=NULL,print_style=TRUE;
	RDA_PFILE *fp=NULL;

	readwidget(prsrc,"DEVICE");
	FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
	readwidget(prsrc,"NORMAL PRINT");
	FINDRSCGETCHAR(prsrc,"NORMAL PRINT",&print_style);
	if(outdevice!=NULL)
	{
		fp=RDA_popen(outdevice,"w");
		if(fp!=NULL)
		{
			SelectPrintType(outdevice,print_style,fp);
			printerrorlist(parent,fp);
			RDA_pclose(fp);
		} else {
			message=Rmalloc(RDAstrlen(outdevice)+69);
			sprintf(message,"The output attemp to the DEVICE [%s] failed when trying to open pipe.",outdevice);
			WARNINGDIALOG("WARNING DIALOG SCREEN","DEVICE COMMUNICATIONS FAILED!",message,NULL,NULL,TRUE,NULL);
			prterr(message);
			if(message!=NULL) Rfree(message);
		}
		if(outdevice!=NULL) Rfree(outdevice);
	} else {
		ERRORDIALOG("Output Device Failure","There was no output device specified.",NULL,FALSE);
	}
}

void print_list(RDArsrc *parent,void (*printfunc)(...))
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL,normal_print=TRUE;

	if(diaggui)
	{
		prterr("DIAG printerrorlist Displaying the Print Screen in preparation to print Error List.");
	}
	prsrc=RDArsrcNEW("GUI","PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addbtnrsrc(prsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	addstdrsrc(prsrc,"NORMAL PRINT",BOOLNS,1,&normal_print,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printfunc,parent);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_print,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PRINT SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		quit_print(prsrc);
	}
}

static APPlib *makescnlist()
{
	int x;
	char *tmp=NULL;
	struct RPTSTRUCTs r;
	APPlib *a;

	a=APPlibNEW();
	for(x=0;x<NE(RptAvl);++x)
	{
		r=RptAvl[x];
		tmp=Rmalloc(RDAstrlen(r.name)+RDAstrlen(r.desc)+16);
		sprintf(tmp,"%3d [%s] [%s]",(x+1),r.name,r.desc);
		addAPPlib(a,tmp);
		if(tmp!=NULL) Rfree(tmp);
	}
	return(a);
}

static void quitfinrpt(RDArsrc *mainrsrc)
{
	if(scnlist!=NULL) freeapplib(scnlist);
	if((errorlist->numlibs!=1) || RDAstrcmp(errorlist->libs[0],"No Errors Detected"))
	{
		APPlib2SupportRequest("GUI","mkgui -> User Interface Update Package",errorlist);
	}
	if(errorlist!=NULL) freeapplib(errorlist);
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	ShutdownSubsystems();
}
static void selectrpt(RDArsrc *mainrsrc)
{
	int selected=0;
	struct RPTSTRUCTs r;
	char *temp=NULL;
	RDArsrc *tmprsrc=NULL;

	FINDRSCGETINT(mainrsrc,"RESOURCE LIST",&selected);
	r=RptAvl[selected];
	if(r.func!=NULL)
	{
		temp=Rmalloc(RDAstrlen(r.name)+18);
		sprintf(temp,"Creating %s Report",(r.name!=NULL?r.name:""));
		tmprsrc=odiagscrn("DIAGNOSTIC SCREEN",module,temp,NULL,NE(RptAvl));
		if(temp!=NULL) Rfree(temp);
		if(tmprsrc!=NULL)
		{
			if(ADVmakescrn(tmprsrc,TRUE))
			{
				prterr("Error Cannot Create Diagnostic Screen.");
				if(tmprsrc!=NULL) free_rsrc(tmprsrc);
			} else {
				ForceWindowUpdate(tmprsrc);
			}
		}
/*lint -e746 */
		r.func();
/*lint +e746 */
		FINDRSCLISTAPPlib(mainrsrc,"ERROR LIST",0,errorlist);
		updatersrc(mainrsrc,"ERROR LIST");
		if(tmprsrc!=NULL) 
		{
			update_diagnostic(tmprsrc,TRUE);
		}
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
		}
	}
}
static void selectall(RDArsrc *mainrsrc)
{
	int x;
	struct RPTSTRUCTs r;
	short did_it=FALSE;
	RDArsrc *tmprsrc=NULL;

	if(mainrsrc!=NULL)
	{
		tmprsrc=odiagscrn("DIAGNOSTIC SCREEN",module,"Creating Standard Reports",NULL,NE(RptAvl));
		if(tmprsrc!=NULL)
		{
			if(ADVmakescrn(tmprsrc,TRUE))
			{
				prterr("Error Cannot Create Diagnostic Screen.");
				if(tmprsrc!=NULL) free_rsrc(tmprsrc);
			} else {
				ForceWindowUpdate(tmprsrc);
			}
		}
	}
	for(x=0;x<NE(RptAvl);++x)
	{
		r=RptAvl[x];
		if(r.func!=NULL)
		{
/*lint -e746 */
			r.func();
/*lint +e746 */
			if(mainrsrc!=NULL)
			{
				FINDRSCLISTAPPlib(mainrsrc,"ERROR LIST",0,errorlist);
				updatersrc(mainrsrc,"ERROR LIST");
				did_it=TRUE;
			}
		} else {
			if(mainrsrc!=NULL)
			{
				did_it=FALSE;
			}
		}
		if(mainrsrc!=NULL)
		{
			if(tmprsrc!=NULL) 
			{
				update_diagnostic(tmprsrc,(did_it ? TRUE:FALSE));
			}
		}
	}
	if(mainrsrc!=NULL)
	{
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
		}
	}
	Establish_Install_Date(module);
}
static void make_screens(int argc,char **argv)
{
	int x,y;
	struct RPTSTRUCTs r;

	if(argc==2 && !RDAstrcmp(argv[1],"ALL"))
	{
		selectall(NULL);
	} else {
		for(x=1;x<argc;++x)
		{
			for(y=0;y<NE(RptAvl);++y)
			{
				r=RptAvl[y];
				if(!RDAstrcmp(argv[x],r.name))
				{
/*lint -e746 */
					r.func();
/*lint +e746 */
					break;
				
				}
			}
			if(y>=NE(RptAvl))
			{
				prterr("ERROR: Report [%s] not in collection of defined reports for FINMGT.",argv[x]);
			}
		}
	}
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	int x=0;
	int ret_int=0;

	if(InitializeSubsystems(argc,argv,module,"MAKE GUI SCREENS")) 
	{
		ShutdownSubsystems();
		return(-1);
	}
	if(XPERT_SETUP==NULL)
	{
		prterr("ERROR: Make GUI can not load Xpert Setup, exiting abnormally");
		ShutdownSubsystems();
		return(-1);
	}
	RDAmkdir("GUI",00770,&ret_int);
	RDAmkdir("pixmaps",00770,&ret_int);
	RDAmkdir("html",00770,&ret_int);
	RDAmkdir("cgi-bin",00770,&ret_int);
	MAKE_GUI_SCREENS();
	if(argc>1)
	{
		make_screens(argc,argv);
		ShutdownSubsystems();
		return(-1);
	} else {
		mainrsrc=RDArsrcNEW(module,"MAKE GUI SCREENS");
		scnlist=makescnlist();
		addlstrsrc(mainrsrc,"RESOURCE LIST",&x,TRUE,NULL,scnlist->numlibs,
			&scnlist->libs,NULL);
		errorlist=APPlibNEW();
		addAPPlib(errorlist,"No Errors Detected");
		addlstrsrc(mainrsrc,"ERROR LIST",&x,TRUE,NULL,errorlist->numlibs,
			&errorlist->libs,NULL);
/*lint -e611 */
		addbtnrsrc(mainrsrc,"PRINT ERROR LIST",TRUE,print_list,(void *)printerrorlistcb);
/*lint +e611 */
		addbtnrsrc(mainrsrc,"SELECT ALL",TRUE,selectall,NULL);
		addbtnrsrc(mainrsrc,"SELECT",TRUE,selectrpt,NULL);
		addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfinrpt,NULL);
		addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
		addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		APPmakescrn(mainrsrc,TRUE,quitfinrpt,NULL,TRUE);
	}
}
static void MAKE_GUI_SCREENS()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;

	scrn=RDAscrnNEW("GUI","MAKE GUI SCREENS");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Resource List","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"RESOURCE LIST","Resource List","","",25,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SELECT ALL","Select All","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,"","","","");
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
