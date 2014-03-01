#ifndef WIN32
#define __NAM__ "mkutl.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mkutl.exe"
#endif
#include <app.hpp>
#include <doheader.hpp>
#include <rptgen.hpp>
#include <mkmsc.hpp>
#ifdef XXXXX
#include <mix.hpp>
#endif
/*CPP_OPART mkutl1 */
/*CPP_OPART mkutl2 */
/*CPP_OPART mkutl3 */
/*CPP_OPART mkutl4 */
/*CPP_OPART mkutl5 */
/*CPP_OPART mkutl6 */
/*CPP_OPART mkutl7 */
/*CPP_OPART mkutl8 */
/*CPP_OPART mkutl9 */
/*CPP_OPART mkutl11 */
/*CPP_OPART mkutl12 */
/*CPP_OPART mkutl14 */
/*CPP_OPART mkutl15 */
/*CPP_OPART mkutl16 */
/*CPP_OPART mkutl17 */
/*CPP_OPART mkutl18 */
/*CPP_OPART mkutl19 */
/*CPP_OPART mkutl20 */
/*CPP_OPART mkutl21 */
/*CPP_OPART mkutl22 */
#ifndef WIN32
/*CPP_OPART mkutl23 */
#endif
/*CPP_OPART mkutl24 */
/*CPP_OPART mkutl25 */
/*CPP_OPART mkutl26 */
/*CPP_OPART mkutl27 */
/*CPP_OPART mkutl28 */
/*CPP_OPART mkutl29 */
/*CPP_OPART mkutl30 */
/*CPP_OPART mkutl31 */
/*CPP_OPART mkutl32 */
/*CPP_OPART mkutl33 */
/*CPP_OPART mkutl34 */
/*CPP_OPART mkutl35 */
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

char *module="UTILITIES";
APPlib *scnlist=NULL;
APPlib *errorlist=NULL;
void print_list(RDArsrc *,void (*)(...));
static void make_screens(int,char **);
static void MAKE_UTILITIES(void);
static void RUN_UDOGSV(void);
static void RUN_BUNDLE(void);

#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */

char SWB_S1_FAST=FALSE;
char SWB_S2_INTERNET_ENABLED=FALSE;
char SWB_S3_CLEARING_FUND=FALSE;
char SWB_S4_MULTIPLE_DEFINITION_TYPES=FALSE;
char SWB_S5_GROUP_DEFAULTS=FALSE;
char SWB_S6_CHART_DATABASE=FALSE;

struct RPTSTRUCTs
{
        char *name;
        char *desc;
        void (*func)(...);
};
extern void DFMTN(void);
extern void MAKE_XPERT_SETUP(void);
extern void DFADD(void);
extern void DFMATRIX(void);
extern void DFTABLE(void);
extern void DFCHG(void);
extern void DIAGNOSTIC_SCREEN(void);
extern void Tools_Menu(void);
extern void DFGSV(void),UPLOADFILE(void);
extern void Main_Menu(void),FIND_DATE_POWERADD(void);
extern void Make_Update_Menu(void),FIND_DATE_MAINTAIN_MASTER(void);
extern void DOS_COPY_WINDOW(void);
#ifdef WIN32
extern void SCRIPT_call_netscape_bat(void),SCRIPT_call_acroread_bat(void);
extern void SCRIPT_call_netscape_bat(void);
#endif
#ifndef WIN32
extern void SCRIPT_chgdirmtnmst_lnx(void);
extern void SCRIPT_setuprdacolors_lnx(void);
#endif
extern void SCRIPT_wrkgrpmtnmst_lnx(void);

extern void FIELD_TEST_SCREEN(void),MVFINCOA(void);
extern void CONVDATE(void),FIND_DATE_VIRTUAL(void),UTLGSV(void);
extern void FIND_DATE_REPORT(void);
extern void FIND_DATE_REMOVE_SCREEN_DEFAULTS(void);
extern void FIND_DATE_CHANGE_SCREEN_LENGTHS(void);
extern void FIND_CUSTOMFIELD_SCREEN(void);
extern void FIND_CUSTOMFIELD_SCREEN_DEFAULTS(void);
static void checkheader(RDA_PFILE *,int *,int *,char *);
static void printheader(RDA_PFILE *,int *,int *,char *,char *);
#if defined(LINUX2_2) || defined(UBUNTU_OS)
extern void SCRIPT_FTP_RDASYS_COM_kdelnk(void);
extern void SCRIPT_WWW_RDASYS_COM_kdelnk(void);
extern void SCRIPT_setRDAenv(void);
extern void SCRIPT_call_mozilla_lnx(void);
extern void SCRIPT_openoffice(void);
extern void SCRIPT_Runscript(void);
extern void SCRIPT_call_opera_lnx(void);
extern void SCRIPT_kghostview_lnx(void);
extern void SCRIPT_call_acroread_lnx(void);
#endif
extern void IMPEXP_MENUITEM(void);
extern void README(void);
extern void docrontab(void),MENUITEMRPTS(void);
extern void MTNMENUITEM(void),DB_MENUITEM(void),DB_MODULE(void),MTNMODULE(void);
extern void DB_MSGGATEWAY(void),MTNMSGGATEWAY(void),CreateMSGGATEWAY(void);
extern void CREATE_MENUITEM(void),ADDUSERREPORTSUBMENU(void),USERREPORTMENU(void);

struct RPTSTRUCTs RptAvl[]=
{
	{"XPERT SETUP                      ","Xpert/MBGUI Setup",MAKE_XPERT_SETUP},
	{"UTILITIES GENERIC SETUP VARIABLES","Utility Generic Setup Variables",UTLGSV},
	{"UTILITIES BUNDLES                ","Utilities Payments Software Bundles     ",RUN_BUNDLE},
	{"EXECUTE GENERIC SETUP VARIABLES","Execute Utility Generic Setup Variables",RUN_UDOGSV},
	{"DFMTN                            ","Define Maintain Master Screens",DFMTN},
	{"DFADD                            ","Define Power Add Screens",DFADD},
	{"DFMATRIX                         ","Define Matrix Screens",DFMATRIX},
	{"DFTABLE                          ","Define Table Screens",DFTABLE},
	{"DFCHG                            ","Define Change Screens",DFCHG},
	{"DFGSV                            ","Define Generic Setup Variables",DFGSV},
	{"DIAGSCRN                         ","Diagnostic Screen",DIAGNOSTIC_SCREEN},
	{"DOS COPY WINDOW                  ","Dos Copy Utility Window",DOS_COPY_WINDOW},
	{"MAIN MENU                        ","Xpert/MBGUI Main Menu",Main_Menu},
	{"TOOLS MENU                       ","Xpert/MBGUI Tools Menu",Tools_Menu},
	{"MAKE UPDATE MENU                 ","Make Update Menu",Make_Update_Menu},
	{"FIELD TEST SCREEN                ","Field Test Screen",FIELD_TEST_SCREEN},
	{"CONVDATE                         ","Finds Dates and Converts",CONVDATE},
	{"FIND DATE VIRTUAL                ","Finds Dates in Virtual Fields",FIND_DATE_VIRTUAL},
	{"FIND DATE SCREEN                 ","Finds Dates in Screens and Changes length",FIND_DATE_CHANGE_SCREEN_LENGTHS},
	{"FIND DATE SCREEN DEFAULTS        ","Finds Dates in Screen Defaults and Removes them",FIND_DATE_REMOVE_SCREEN_DEFAULTS},
	{"FIND DATE MAINTAIN MASTER        ","Finds Dates in Maintain Master Expressions",FIND_DATE_MAINTAIN_MASTER},
	{"FIND DATE POWERADD               ","Finds Dates in Power Add Expressions",FIND_DATE_POWERADD},
	{"FIND DATE REPORT                 ","Finds Dates in Report Expressions",FIND_DATE_REPORT},
	{"FIND CUSTOM INPUTS FIELDS        ","Finds Custom Fields in Screen Definitions",FIND_CUSTOMFIELD_SCREEN},
	{"FIND CUSTOM INPUTS DEFAULT FIELDS","Finds Custom Fields in Screen Default Definitions",FIND_CUSTOMFIELD_SCREEN_DEFAULTS},
	{"README                           ","XPERT Readme's",README},
	{"DOCTRONTAB                       ","Manage Crontab's",docrontab},
	{"MVFINCOA                         ","MVFINCOA Report Definition",MVFINCOA},
	{"UPLOAD FILE                      ","Upload File",UPLOADFILE},
#ifdef WIN32
#endif
#ifndef WIN32
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
	{"RUN SCRIPTS                      ","Script to Call runscript",SCRIPT_Runscript},
	{"RDA CRON ENVIRONMENT             ","Script to Set RDA CRON Environment",SCRIPT_setRDAenv},
#endif
	{"DB_MODULE                        ","Database for Modules (Qt)",DB_MODULE},
	{"MTN MODULE                       ","Module Maintain Master (Qt)",MTNMODULE},
	{"DB_MENUITEM                      ","Database for Menu Items (Qt)",DB_MENUITEM},
	{"MTN MENUITEM                     ","MenuItem Maintain Master (Qt)",MTNMENUITEM},
	{"IMPEXP_MENUITEM                  ","Import/Export Menu Items (Qt)",IMPEXP_MENUITEM},
	{"MODULE-MENUITEM REPORTS          ","MODULE/MENUITEM REPORTS",MENUITEMRPTS},
	{"DB_MSG_GATEWAY                   ","Database for MSG-GATEWAY",DB_MSGGATEWAY},
	{"MTN MSG-GATEWAY                  ","MSG-Gateway Maintain Master",MTNMSGGATEWAY},
	{"CreateMSG-GATEWAY                ","Create MSG-Gateway Databaser",CreateMSGGATEWAY},
	{"ADD USER REPORT SUBMENU          ","Add User Report Submenu",ADDUSERREPORTSUBMENU},
	{"MTN USER REPORT SUBMENU          ","MTN User Report Submenu",USERREPORTMENU},
	{"CREATE_MENUITEM                  ","Create OpenRDA Menu Items",CREATE_MENUITEM},
};

static void quit_print(RDArsrc *prsrc)
{
	if(diaggui)
	{
		prterr("DIAG quit_print Quitting Print Screen Definition.");
	}
	if(prsrc!=NULL) free_rsrc(prsrc);
}

static void checkheader(RDA_PFILE *fp,int *lines,int *pages,char *modname)
{
	char *temp=NULL;
	if(doheaders)
	{
		if(*lines>57)
		{
			RDA_fprintf(fp,"\f");
			temp=Rmalloc(RDAstrlen(modname)+23);
			sprintf(temp,"XPERT MAKE %s ERROR LIST",
				(modname!=NULL?modname:""));
			printheader(fp,lines,pages,temp,modname);
			if(temp!=NULL) Rfree(temp);
		}
	}
}
static void printheader(RDA_PFILE *fp,int *lines,int *pages,char *header,char *modname)
{
	char *date=NULL,*timex=NULL;
	int lead_ws=0,tail_ws=0;
	unsigned line_length=79;
	char *mainheader=NULL;

	date=GETCURRENTDATE10();
	timex=GETCURRENTTIME();
	RDA_fprintf(fp,"\n");
	*lines=1;
	
	if(!isEMPTY(XPERT_SETUP->company_name))
	{
		mainheader=stralloc(XPERT_SETUP->company_name);
	} else {
		mainheader=stralloc("No Organization Name");
	}


	lead_ws=(int)(RDAroundup((double)(line_length-(2+RDAstrlen(mainheader)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(2+RDAstrlen(mainheader)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(mainheader)+2+16+lead_ws+tail_ws)>line_length) mainheader[(line_length-16-lead_ws-tail_ws)]=0;
	RDA_fprintf(fp,"%*s %s %*sPage: %*d\n",
		lead_ws,"",mainheader,tail_ws,"",10,++*pages);
	++*lines;
	if(mainheader!=NULL) Rfree(mainheader);


	lead_ws=(int)(RDAroundup((double)(line_length-(2+RDAstrlen(header)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(2+RDAstrlen(header)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(header)+2+16+lead_ws+tail_ws)>line_length) header[(line_length-16-lead_ws-tail_ws)]=0;
	RDA_fprintf(fp,"%*s %s %*sDate: %s\n",
		lead_ws,"",header,tail_ws,"",date);
	++*lines;



/*
	lead_ws=(int)(RDAroundup((double)(line_length-(10+RDAstrlen(modname)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(10+RDAstrlen(modname)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	RDA_fprintf(fp,"%*s Module: %s %*sDate: %s\n",
		lead_ws,"",(modname!=NULL?modname:""),tail_ws,"",date);
	++*lines;
*/

	lead_ws=(int)(RDAroundup(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(USERLOGIN)+16+15+lead_ws+tail_ws)>line_length)
	{
		RDA_fprintf(fp,"%*s %s %*sTime:   %s\n",
			(int)(RDAroundup(((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2))),"",
			USERLOGIN,
			(int)(Rint((((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2)-16))),"",
			timex);
		++*lines;
	} else {
		RDA_fprintf(fp,"%*s Executed By: %s %*sTime:   %s\n",
			lead_ws,"",USERLOGIN,tail_ws,"",timex);
		++*lines;
	}

	RDA_fprintf(fp,"\n");
	++*lines;
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
}
static void printerrorlist(RDA_PFILE *fp,char *modname,int *lines,int *pages)
{
	int x;
	char *temp=NULL;

	if(fp!=NULL)
	{
		temp=Rmalloc(RDAstrlen(modname)+23);
		sprintf(temp,"XPERT MAKE %s ERROR LIST",(modname!=NULL?modname:""));
		printheader(fp,lines,pages,temp,modname);
		if(temp!=NULL) Rfree(temp);

		for(x=0;x<errorlist->numlibs;++x)
		{
			RDA_fprintf(fp,"(%5d) %s\n",(x+1),errorlist->libs[x]);
			++*lines;
			checkheader(fp,lines,pages,modname);
		}
		RDA_fprintf(fp,"\f");
	}
}
void printerrorlistcb(RDArsrc *prsrc,RDArsrc *parent)
{
	char *outdevice=NULL,*message=NULL,print_style=TRUE;
	RDA_PFILE *fp=NULL;
	int lines=0,pages=0;

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
			printerrorlist(fp,module,&lines,&pages);
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
		APPlib2SupportRequest("UTILITIES","mkutl -> Utilities Update Package",errorlist);
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
			} else {
				ForceWindowUpdate(tmprsrc);
			}
		}
	}
	for(x=0;x<NE(RptAvl);++x)
	{
		r=RptAvl[x];
		if(r.func!=NULL && !strstr(r.desc,"kdelnk"))
		{
			if(diagapps)
			{
				prterr("Beginning [%s] ",r.desc);TRACE;
			}
			prterr("Executing: [%s][%s] from collection defined for UTILITIES.",(r.name!=NULL ? r.name:""),(r.desc!=NULL ? r.desc:""));
/*lint -e746 */
			r.func();
/*lint +e746 */
			if(diagapps)
			{
				prterr("Completed [%s] ",r.desc);TRACE;
			}
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
static void MAKE_UTILITIES()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	scrn=RDAscrnNEW("UTILITIES","MAKE UTILITIES");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Resource List","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"RESOURCE LIST","Resource List","","",20,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Error List","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"ERROR LIST","Error List","","",5,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SELECT ALL","Select All","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"PRINT ERROR LIST","Print Errors","","",0,0,0,"","","","");
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
/*
			ERRORDIALOG("Cannot Save Screen","This screen may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
*/
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(RDAstrlen(scrn->module)+RDAstrlen(scrn->name)+100+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [%s] Screen [%s], Can Not Save Screen!",(scrn->module!=NULL?scrn->module:""),(scrn->name!=NULL?scrn->name:""));
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
static void RUN_UDOGSV()
{
	char *temp1=NULL;
	int x=0;
	APPlib *args=NULL;

	args=APPlibNEW();
	addAPPlib(args,module);
	x=ADVExecute2Program("dogsv",args,NULL);
	if(args!=NULL) freeapplib(args);
	if(x!=0)
	{
		temp1=Rmalloc(RDAstrlen(module)+61+1);
		sprintf(temp1,"VIRTUAL FIELD CREATION ERROR: Can Not Create %s Virtual Fields!",(module!=NULL?module:""));
		prterr(temp1);
		if(errorlist!=NULL)
		{
			addERRlist(&errorlist,temp1);
		}
		if(temp1!=NULL) Rfree(temp1);
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

	if(InitializeSubsystems(argc,argv,module,"MAKE UTILITIES")) 
	{
		ShutdownSubsystems();
		return(-1);
	}
	if(XPERT_SETUP==NULL)
	{
		prterr("ERROR: Make Utilities can not load Xpert Setup, exiting abnormally"); 
		ShutdownSubsystems();
		return(-1);
	}
	if(XPERT_SETUP->software_type==0)
	{
		SWB_S1_FAST=TRUE;
		SWB_S2_INTERNET_ENABLED=TRUE;
		SWB_S5_GROUP_DEFAULTS=TRUE;
		SWB_S6_CHART_DATABASE=TRUE;
	} 

	RDAmkdir("UTILITIES",00770,&ret_int);
	RDAmkdir("DOWNLOADS",00770,&ret_int);
	RDAmkdir("TEMP",00770,&ret_int);
	RDAmkdir("scripts",00770,&ret_int);
	MAKE_UTILITIES();
	if(argc>1)
	{
		make_screens(argc,argv);
		ShutdownSubsystems();
		return(-1);
	} else {
		mainrsrc=RDArsrcNEW(module,"MAKE UTILITIES");
		scnlist=makescnlist();
		addlstrsrc(mainrsrc,"RESOURCE LIST",&x,TRUE,NULL,scnlist->numlibs,&scnlist->libs,NULL);
		errorlist=APPlibNEW();
		addAPPlib(errorlist,"No Errors Detected");
		addlstrsrc(mainrsrc,"ERROR LIST",&x,TRUE,NULL,errorlist->numlibs,&errorlist->libs,NULL);
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
static void RUN_BUNDLE()
{
	APPlib *args=NULL;
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;

	if(XPERT_SETUP->software_type>1) return;
	args=APPlibNEW();
	addAPPlib(args,"UTILITIES");
	addAPPlib(args,"SYSTEM WIDE BUNDLES");
	ADVExecute2Program("dogsv",args,NULL);
	if(args!=NULL) freeapplib(args);
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("UTILITIES")+11);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif
		gsv=RDAGenericSetupNew("UTILITIES","SWB_S1_FAST");
		if(gsv!=NULL)
		{
			if(!getRDAGenericSetupbin(libx,gsv))
			{
				SWB_S1_FAST=*gsv->value.string_value;
			}
		}
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
		gsv=RDAGenericSetupNew("UTILITIES","SWB_S2_INTERNET_ENABLED");
		if(gsv!=NULL)
		{
			if(!getRDAGenericSetupbin(libx,gsv))
			{
				SWB_S2_INTERNET_ENABLED=*gsv->value.string_value;
			}
		}
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
		gsv=RDAGenericSetupNew("UTILITIES","SWB_S3_CLEARING_FUND");
		if(gsv!=NULL)
		{
			if(!getRDAGenericSetupbin(libx,gsv))
			{
				SWB_S3_CLEARING_FUND=*gsv->value.string_value;
			}
		}
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
		gsv=RDAGenericSetupNew("UTILITIES","SWB_S4_MULTIPLE_DEFINITION_TYPES");
		if(gsv!=NULL)
		{
			if(!getRDAGenericSetupbin(libx,gsv))
			{
				SWB_S4_MULTIPLE_DEFINITION_TYPES=*gsv->value.string_value;
			}
		}
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
		gsv=RDAGenericSetupNew("UTILITIES","SWB_S5_GROUP_DEFAULTS");
		if(gsv!=NULL)
		{
			if(!getRDAGenericSetupbin(libx,gsv))
			{
				SWB_S5_GROUP_DEFAULTS=*gsv->value.string_value;
			}
		}
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
		gsv=RDAGenericSetupNew("UTILITIES","SWB_S6_CHART_DATABASE");
		if(gsv!=NULL)
		{
			if(!getRDAGenericSetupbin(libx,gsv))
			{
				SWB_S6_CHART_DATABASE=*gsv->value.string_value;
			}
		}
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
		if(libx!=NULL) Rfree(libx);
		UTLGSV();
}
