#ifndef WIN32
#define __NAM__ "mkrpt.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mkrpt.exe"
#endif
#include <app.hpp>
#include <mkmsc.hpp>
#include <rptgen.hpp>
/*CPP_OPART mkrpt1 */
/*CPP_OPART mkrpt2 */
/*CPP_OPART mkrpt3 */
/*CPP_OPART mkrpt4 */
/*CPP_OPART rptscn1 */
/*CPP_OPART rptscn2 */
/*CPP_OPART rptscn3 */
/*CPP_OPART rptscn4 */
/*CPP_OPART rptscn5 */
/*CPP_OPART rptscn6 */
/*CPP_OPART rptscn7 */
/*ADDLIB mklib */
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

char *module="RPTGEN";
APPlib *scnlist=NULL;
APPlib *errorlist=NULL;
void print_list(RDArsrc *,void (*)(...));
static void make_screens(int,char **);
static void MAKE_RPTGEN(void);

struct RPTSTRUCTs
{
        char *name;
        char *desc;
        void (*func)(...);
};
extern void mk_dfrpt(void),mk_doreport(void),mk_ezrpt(void),mk_dfvir(void);
extern void mk_dfprt(void),mk_diagscrn(void),REPORT_GENERATOR(void);
extern void EPSON_OLD_DOT_MATRIX(void),IBM_EMULATION(void);
extern void EPSON_DFX1(void),EPSON_EMULATION(void),EPSON_STAR_NX1000C(void);
extern void HP_LASER_JET_III(void),CASHDRAWER(void);
extern void HP_LASER_8000n(void);
extern void HP_LASER_4000(void);
extern void TXT2PS(void);
extern void HP_DESKJET_600(void);
extern void HP_DESKJET_520(void);
extern void PRT_ANADEX(void);
extern void PRT_BROTHER(void);
extern void PRT_C_ITOH_ELECTRONICS(void);
extern void PRT_CENTRONICS(void);
extern void PRT_DATA_ROYAL(void);
extern void PRT_DATA_SOUTH(void);
extern void PRT_IBM(void);
extern void PRT_NEC(void);
extern void PRT_OKIDATA(void);
extern void PRT_TANDY(void);
extern void PRT_TEXAS_INSTRUMENTS(void);
extern void PRT_TOSHIBA(void);
extern void PRT_ZEROX_LETTER_QUALITY(void);
extern void PRT_HP_LASER_JET_1100(void),PRT_OfficeJet(void);
extern void CONVERT_MICROBUDGET_REPORTS(void),SETPRT(void);
extern void MAKE_COA_EZ_VIRTUAL_FIELDS(void);
extern void PRT_rdaout(void);
#ifndef WIN32
extern void PRT_outputemail_lnx(void),PRT_rdareportcd_lnx(void);
extern void PRT_sendattach2lnx(void);
extern void SCRIPT_sendattach_2_lnx(void);
extern void SCRIPT_rdareportcd_lnx(void),SCRIPT_outputemail_lnx(void);
extern void SCRIPT_sendmail_lnx(void);
#endif


struct RPTSTRUCTs RptAvl[]=
{
	{"DIAGSCRN            ","Diagnostic Screen",mk_diagscrn},
	{"DFRPT               ","Define Report Screens",mk_dfrpt},
	{"DOREPORT            ","Doreport Screen",mk_doreport},
	{"EZRPT               ","EZ-Report Screens",mk_ezrpt},
	{"DFVIR               ","Define EZ-Virtual Fields",mk_dfvir},
	{"MAKE COAS           ","Make COA EZ Virtual Field Screen",MAKE_COA_EZ_VIRTUAL_FIELDS},
	{"DFPRT               ","Define Output Devices",mk_dfprt},
	{"SETPRT              ","Setup Output Devices",SETPRT},
	{"RPT MENU            ","Report Generator Menu",REPORT_GENERATOR},
	{"EPSON_OLD_DOT_MATRIX","Epson Old Dot Matrix Printer Definition",EPSON_OLD_DOT_MATRIX},
	{"EPSON_DFX1          ","Epson DFX1 Printer Definition",EPSON_DFX1},
	{"EPSON EMULATION     ","Epson Emulation Printer Definition",EPSON_EMULATION},
	{"IBM EMULATION       ","IBM Emulation Printer Definition",IBM_EMULATION},
	{"EPSON STARNX1000C   ","Epson Start NX1000C Printer Definition",EPSON_STAR_NX1000C},
	{"HP LASER JET III    ","HP LaserJet III Printer Definition",HP_LASER_JET_III},
	{"HP LASER 8000n      ","HP LaserJet 8000n Printer Definition",HP_LASER_8000n},
	{"HP LASER 4000       ","HP LaserJet 4000  Printer Definition",HP_LASER_4000},
	{"TXT2PS              ","TXT2PS Printer Definition",TXT2PS},
	{"HP DESKJET 600      ","HP DeskJet 600 Printer Definition",HP_DESKJET_600},
	{"HP DESKJET 520      ","HP DeskJet 520 Printer Definition",HP_DESKJET_520},
	{"PRT_ANADEX          ","Anadex Printer Definition",PRT_ANADEX},
	{"PRT_BROTHER         ","Brother Printer Definition",PRT_BROTHER},
	{"PRT_C_ITOH_ELECTRONICS","C. Itoh Electronics Printer Definition",PRT_C_ITOH_ELECTRONICS},
	{"PRT_CENTRONICS      ","Centronics Printer Definition",PRT_CENTRONICS},
	{"PRT_DATA_ROYAL      ","Data Royal Printer Definition",PRT_DATA_ROYAL},
	{"PRT_DATA_SOUTH      ","Data South Printer Definition",PRT_DATA_SOUTH},
	{"PRT_IBM             ","IBM Printer Definition",PRT_IBM},
	{"PRT_NEC             ","NEC Printer Definition",PRT_NEC},
	{"PRT_OfficeJet       ","OfficeJet 1170 cxi",PRT_OfficeJet},
	{"PRT_OKIDATA         ","Okidata Printer Definition",PRT_OKIDATA},
	{"PRT_TANDY           ","Tandy Printer Definition",PRT_TANDY},
	{"PRT_TEXAS_INSTRUMENTS","Texas Instruments Printer Definition",PRT_TEXAS_INSTRUMENTS},
	{"PRT_TOSHIBA         ","Toshiba Printer Definition",PRT_TOSHIBA},
	{"PRT_ZEROX_LETTER_QUALITY","Zerox Letter Quality Printer Definition",PRT_ZEROX_LETTER_QUALITY},
	{"PRT_HP_LASER_JET_1100","HP Laser Jet 1100 Printer Definition",PRT_HP_LASER_JET_1100},
	{"rdaout              ","Printer Definition for RDA Text Browser",PRT_rdaout},
#ifndef WIN32
	{"outputemail.lnx     ","Printer Definition for outputemail.lnx Script",PRT_outputemail_lnx},
	{"rdareportcd.lnx     ","Printer Definition for rdareportcd.lnx Script",PRT_rdareportcd_lnx},
	{"sendattach-2.lnx     ","Printer Definition for sendattach-2.lnx Script",PRT_sendattach2lnx},
	{"SCRIPT sendattach-2 ","Actual sendattach-2.lnx Script",SCRIPT_sendattach_2_lnx},
	{"SCRIPT rdareportcd  ","Actual rdareportcd.lnx Script",SCRIPT_rdareportcd_lnx},
	{"SCRIPT outputemail  ","Actual outputemail.lnx Script",SCRIPT_outputemail_lnx},
	{"SCRIPT sendmail     ","Actual sendmail.lnx Script",SCRIPT_sendmail_lnx},
#endif
	{"CASHDRAWER          ","Cash Drawer / Receipt Printer",CASHDRAWER},
	{"MB2EZRPT Screen     ","CONVERT MICROBUDGET REPORTS",CONVERT_MICROBUDGET_REPORTS},
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
		RDA_fprintf(fp,"                     XPERT MAKE RPTGEN ERROR LIST          Date: %*s\n",8,date);
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
		APPlib2SupportRequest("RPTGEN","mkrpt -> Report Generation Update Package",errorlist);
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
		if(diagapps)
		{
			prterr("[%d] [%s] ",(x+1),r.name);TRACE;
		}
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

	if(InitializeSubsystems(argc,argv,module,"MAKE RPTGEN")) 
	{
		return;
	}
	if(XPERT_SETUP==NULL)
	{
		prterr("ERROR: Make GUI can not load Xpert Setup, exiting abnormally");
		exit(1);
	}
	RDAmkdir("RPTGEN",00770,&ret_int);
	RDAmkdir("forms",00770,&ret_int);
	MAKE_RPTGEN();
	if(argc>1)
	{
		make_screens(argc,argv);
		ShutdownSubsystems();
		std::exit;
	} else {
		mainrsrc=RDArsrcNEW(module,"MAKE RPTGEN");
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
static void MAKE_RPTGEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;

	scrn=RDAscrnNEW("RPTGEN","MAKE RPTGEN");
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
