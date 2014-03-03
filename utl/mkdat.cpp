#ifndef WIN32
#define __NAM__ "mkdat.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mkdat.exe"
#endif
#include <app.hpp>
#include <mix.hpp>
#include <mkmsc.hpp>
#include <mkmenu.hpp>
/*CPP_OPART mkdat1 */
/*CPP_OPART mkdat2 */
/*CPP_OPART mkdat3 */
/*CPP_OPART mkdat4 */
/*CPP_OPART mkdat5 */
/*CPP_OPART mkdat6 */
/*CPP_OPART mkdat7 */
/*CPP_OPART mkdat8 */
/*CPP_OPART mkdat9 */
/*CPP_OPART mkdat10 */
/*CPP_OPART mkdat11 */
/*CPP_OPART mkdat12 */
/*CPP_OPART mkdat13 */
/*CPP_OPART mkdat14 */
/*CPP_OPART mkdat15 */
/*CPP_OPART datscn1 */
/*CPP_OPART datscn2 */
/*CPP_OPART datscn3 */
/*CPP_OPART datscn4 */
/*CPP_OPART datscn5 */
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

#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */

char *module="DATABASE";
APPlib *scnlist=NULL;
APPlib *errorlist=NULL;
void print_list(RDArsrc *,void (*)(...));
static void make_screens(int,char **);
static void MAKE_DATABASE(void);
static void RUN_DDOGSV(void);

struct RPTSTRUCTs
{
        char *name;
        char *desc;
        void (*func)(...);
};
extern void DB_DATTRN(void),DB_DATLOG(void);
extern void mk_dfnrd(void),mk_dfie(void),mk_doie(void),mk_remtrans(void);
extern void mk_rnumtran(void),mk_rbdfil(void),mk_restruct(void);
extern void mk_remdel(void),mk_undel(void),mk_viewtran(void);
extern void mk_diagscrn(void),BACKUPDATA(void),RESTOREDATA(void);
extern void TRANSACTION_PROCESSING(void),FILE_MAINTENANCE(void);
extern void mk_crtie(void),DOFTPZIP(void),DAT2CONV(void);
extern void CHKTAPE(void),MTNDATLOG(void),MTNDATTRN(void);
extern void SCRIPTconvzip(void),PURGEDATLOGTRN(void);
extern void SCRIPTdatazip(void),IBSBACKUP(void),LOGSTAT(void);
extern void SCRIPTfixcdrecordlog(void),DFGSV_DATABASE(void);

struct RPTSTRUCTs RptAvl[]=
{
	{"DFGSV DATABASE ","Database Generic Setup Variables",DFGSV_DATABASE},
	{"DOGSV DATABASE ","Execute Database Generic Setup Variables",RUN_DDOGSV},
	{"DIAGSCRN       ","Diagnostic Screen",mk_diagscrn},
	{"DB_DATLOG      ","DATLOG Database",DB_DATLOG},
	{"DB_DATTRN      ","DATTRN Database",DB_DATTRN},
	{"MTN DATLOG     ","DATLOG Maintain Master",MTNDATLOG},
	{"MTN DATTRN     ","DATTRN Maintain Master",MTNDATTRN},
	{"DFNRD          ","Define Files Screens",mk_dfnrd},
	{"DFIE           ","Define Import/Export Screens",mk_dfie},
	{"DOIE           ","Do Import/Export Screen",mk_doie},
	{"PURGEDATLOGTRN ","PURGE DATLOG/TRN Report Definition",PURGEDATLOGTRN},
	{"LOGSTAT        ","Transaction Log Statistics",LOGSTAT},
	{"REMTRANS       ","Remove Transactions",mk_remtrans},
	{"RNUMTRAN       ","Renumber Transactions",mk_rnumtran},
	{"RBDFIL         ","Rebuild Database Screen",mk_rbdfil},
	{"RESTRUCT       ","Restructure Database Screen",mk_restruct},
	{"REMDEL         ","Remove Deleted Records Screen",mk_remdel},
	{"UNDEL          ","Undelete Deleted Records Screen",mk_undel},
	{"VIEWTRAN       ","View/Undo Transactions",mk_viewtran},
	{"FILEMTNC       ","File Maintenance Menu",FILE_MAINTENANCE},
	{"TRANSPRC       ","Transaction Processing Menu",TRANSACTION_PROCESSING},
	{"CRTIE          ","Create Import/Export Screen",mk_crtie},
	{"BACKUPDATA     ","Backup Data",BACKUPDATA},
	{"DOFTPZIP       ","ZIP Data and FTP to RDA",DOFTPZIP},
	{"RESTOREDATA    ","Restore Data",RESTOREDATA},
	{"DAT2CONV       ","Database to Conversion",DAT2CONV},
	{"IBSBACKUP      ","Internet Backup Service",IBSBACKUP},
	{"CHKTAPE        ","DISPLAYTAPE/CHKTAPE",CHKTAPE},
	{"CONV ZIP SCRIPT","Conversion Zip Script ",SCRIPTconvzip},
	{"data ZIP SCRIPT","data directory Zip Script ",SCRIPTdatazip},
	{"Fix Backup Log SCRIPT","fix cdrecord log Script ",SCRIPTfixcdrecordlog},
};

static void quit_print(RDArsrc *prsrc)
{
	if(diaggui)
	{
		prterr("DIAG quit_print Quitting Print Screen Definition.");
	}
	if(prsrc!=NULL) free_rsrc(prsrc);
}

static void printerrorlist(RDArsrc *parent,RDA_PFILE *fp)
{
	int x;
	char *date=NULL,*timex=NULL;

	if(fp!=NULL)
	{
		date=GETCURRENTDATE();
		timex=GETCURRENTTIME();
		RDA_fprintf(fp,"                     XPERT MAKE DATABASE ERROR LIST          Date: %*s\n",8,date);
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
		APPlib2SupportRequest("DATABASE","mkdat -> Database Update Package",errorlist);
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

	if(InitializeSubsystems(argc,argv,module,"MAKE DATABASE")) 
	{
		ShutdownSubsystems();
		return(-1);
	}
	if(XPERT_SETUP==NULL)
	{
		prterr("ERROR: Make Security can not load Xpert Setup, exiting abnormally");
		ShutdownSubsystems();
		return(-1);
	}
	RDAmkdir("DATABASE",00770,&ret_int);
	MAKE_DATABASE();
	if(argc>1)
	{
		make_screens(argc,argv);
		ShutdownSubsystems();
		return(-1);
	} else {
		mainrsrc=RDArsrcNEW(module,"MAKE DATABASE");
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
static void MAKE_DATABASE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;

	scrn=RDAscrnNEW("DATABASE","MAKE DATABASE");
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
static void RUN_DDOGSV()
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
