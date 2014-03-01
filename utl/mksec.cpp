#ifndef WIN32
#define __NAM__ "mksec.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mksec.exe"
#endif
#include <app.hpp>
#include <mkmsc.hpp>
#include <rptgen.hpp>
#include <mksecs.hpp>
#ifdef XXXXX
#include <mix.hpp>
#endif
#include <curver.hpp>

#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */
/*CPP_OPART mksec2 */
/*CPP_OPART mksec3 */
/*CPP_OPART mksec4 */
/*CPP_OPART mksec5 */
/*CPP_OPART mksec6 */
/*CPP_OPART mksec7 */
/*CPP_OPART mksec8 */
/*CPP_OPART mksec9 */
/*CPP_OPART mksec10 */
/*CPP_OPART mksec11 */
/*CPP_OPART mksec12 */
/*CPP_OPART mksec13 */
/*CPP_OPART mksec14 */
/*CPP_OPART mksec15 */
/*CPP_OPART mksec16 */
/*CPP_OPART mksec17 */
/*CPP_OPART mksec18 */
/*CPP_OPART mksec19 */
/*CPP_OPART mksec20 */
/*CPP_OPART mksec21 */
/*CPP_OPART mksec22 */
/*CPP_OPART mksec23 */
/*CPP_OPART mksec24 */
/*CPP_OPART mksec25 */
/*CPP_OPART mksec26 */
/*CPP_OPART mksec27 */
/*CPP_OPART mksec28 */
/*CPP_OPART mksec29 */
/*CPP_OPART mksec30 */
/*CPP_OPART mksec31 */
/*CPP_OPART mksec32 */
/*CPP_OPART mksec33 */
/*CPP_OPART mksec34 */
/*CPP_OPART mksec35 */
/*CPP_OPART mksec36 */
/*CPP_OPART mksec37 */
/*CPP_OPART mksec38 */
/*CPP_OPART mksec39 */
/*CPP_OPART mksec40 */
/*ADDLIB cver */
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

char *module="SECURITY";
APPlib *scnlist=NULL;
APPlib *errorlist=NULL;
void print_list(RDArsrc *,void (*)(...));
static void make_screens(int,char **);
static void mk_diagscrn(void);
static void MAKE_SECURITY(void),MKGUEST(void);

struct RPTSTRUCTs
{
        char *name;
        char *desc;
        void (*func)(...);
};
extern void LoginTemplates(void),DB_TOKENS(void);
extern void SETWTPASSWD(void),MTNTOKENS(void);
extern void USERS(void),MODSEC(void),PROCSEC(void),FILESEC(void),FLDSEC(void);
extern void USERS_MASTER(void),MODSEC_MASTER(void),PROCSEC_MASTER(void);
extern void FILESEC_MASTER(void),FLDSEC_MASTER(void),SECURITY_SUBMENU(void);
extern void USER_LIST(void),MODACCESS(void),copymodsec(void);
extern void ADDUSER(void),ADDMODSEC(void),COPYPOSLMST(void),RPTSETPROCSEC(void);
extern void ADDFILESEC(void),ADDFLDSEC(void),ADDPROCSEC(void),RPTADDMODSEC(void);
extern void MAKE_VIEW_LICENSES(void),USERSLDIF(void),RPTMODSECLIST(void),SECURITY_DEPARTMENT(void);
extern void MTNDEPARTMENT(void),EMAILUSERS(void),COPYPAYLOC(void);
extern void USERS_IMPORT(void),MODSEC_IMPORT(void),FILESEC_IMPORT(void);
extern void CHGRECORDSEC(void);
extern void PROCSEC_IMPORT(void),FLDSEC_IMPORT(void);
extern void RPTPURGEFILESEC(void),RPTPURGEPROCSEC(void),RPTPURGEFLDSEC(void),RPTPURGEMODSEC(void),SCNSECURITY_PURGE(void);
extern void MakeStdUsers(void),MAKE_RPT_COPYMSMTPRC(),MAKE_RPT_CREATEMSMTPRC(),MAKE_SCRIPTMoveMSMTPRC();
extern void MAKE_RPT_SETPOSTACTIVITY(void),EMAILUSERSINFO(void);
extern void MakeRDACronUser(void);

struct RPTSTRUCTs RptAvl[]=
{
	{"DEPARTMENT             ","DEPARTMENT Database",SECURITY_DEPARTMENT},
	{"USERS                  ","USERS      Database",USERS},
	{"MODSEC                 ","MODSEC     Database",MODSEC},
	{"PROCSEC                ","PROCSEC    Database",PROCSEC},
	{"FILESEC                ","FILESEC    Database",FILESEC},
	{"FLDSEC                 ","FLDSEC     Database",FLDSEC},
	{"TOKENS                 ","TOKENS     Database",DB_TOKENS},
	{"GUEST LOGIN            ","GUEST LOGIN (New Installation's Only)",MKGUEST},
	{"LoginTemplates_xml     ","Wt Login Templates ",LoginTemplates},
	{"SETWTPASSWD            ","Set WT Default Password",SETWTPASSWD},
	{"SECURITY SUBMENU       ","Security Submenu",SECURITY_SUBMENU},
	{"MTN DEPARTMENT         ","DEPARTMENT Maintain Master",MTNDEPARTMENT},
	{"MTN USERS              ","USERS      Maintain Master",USERS_MASTER},
	{"MTN MODSEC             ","MODSEC     Maintain Master",MODSEC_MASTER},
	{"MTN PROCSEC            ","PROCSEC    Maintain Master",PROCSEC_MASTER},
	{"MTN FILESEC            ","FILESEC    Maintain Master",FILESEC_MASTER},
	{"MTN FLDSEC             ","FLDSEC     Maintain Master",FLDSEC_MASTER},
	{"MTN TOKENS             ","TOKENS     Maintain Master",MTNTOKENS},
	{"USERLIST               ","Report of Xpert Users",USER_LIST},
	{"MODACCESS              ","Report of Module Access",MODACCESS},
	{"COPYMODSEC             ","Copy Security Profile",copymodsec},
	{"ADD USERS              ","Add Security Users",ADDUSER},
	{"ADDMODSEC              ","Add Module Security",ADDMODSEC},
	{"ADDFILESEC             ","Add File Security",ADDFILESEC},
	{"ADDFLDSEC              ","Add Field Security",ADDFLDSEC},
	{"ADDPROCSEC             ","Add Process Security",ADDPROCSEC},
	{"VIEW LICENSES          ","View Licenses Tool",MAKE_VIEW_LICENSES},
	{"USERSLDIF              ","Report of Users for Address Book",USERSLDIF},
	{"RPTMODSECLIST          ","MODULE SECURITY Report",RPTMODSECLIST},
	{"EMAILUSERS             ","EMAIL RDA Users",EMAILUSERS},
	{"COPYPOSLMST            ","Copy Position Locations to Departments",COPYPOSLMST},
	{"COPYPAYLOC             ","Copy Pay Locations to Departments",COPYPAYLOC},
	{"ADDMODSEC              ","Copy Add Secuirty By Module",RPTADDMODSEC},
	{"COPYMSMTPRC            ","Copy the msmtprc file to home directories",MAKE_RPT_COPYMSMTPRC},
	{"CREATEMSMTPRC          ","Create the msmtprc files",MAKE_RPT_CREATEMSMTPRC},
	{"MoveMSMTPRC            ","Copy script for msmtprc files",MAKE_SCRIPTMoveMSMTPRC},
	{"USERS_IMPORT           ","Import/Export Users",USERS_IMPORT},
	{"MODSEC_IMPORT          ","Import/Export Module Security",MODSEC_IMPORT},
	{"PROCSEC_IMPORT         ","Import/Export Process Security",PROCSEC_IMPORT},
	{"FILESEC_IMPORT         ","Import/Export File/Record Security",FILESEC_IMPORT},
	{"FLDSEC_IMPORT          ","Import/Export Field Security",FLDSEC_IMPORT},
	{"RPT PURGEFLDSEC        ","Purge Field Security Report ",RPTPURGEFLDSEC},
	{"RPT PURGEFILESEC       ","Purge File Security Report ",RPTPURGEFILESEC},
	{"RPT PURGEPROCSEC       ","Purge Process Security Report ",RPTPURGEPROCSEC},
	{"RPT PURGEMODSEC        ","Purge Module Security Report ",RPTPURGEMODSEC},
	{"RPT CHGRECORDSEC       ","Change Record Security",CHGRECORDSEC},
	{"RPT SETPOSTACTIVITY    ","Make Report to Set Allow Post Activity Flag",MAKE_RPT_SETPOSTACTIVITY},
	{"RPT EMAILUSERSINFO     ","Email Workstation Information to Users",EMAILUSERSINFO},
	{"SECURITY PURGE SUBMENU ","Purge Security Submenu ",SCNSECURITY_PURGE},
	{"SETUP RDACRON USER     ","Setup RDACRON User/Group Security          ",MakeRDACronUser},
	{"Make Std Users         ","Make Standard Users (admin, acctmgr, data-entry)",MakeStdUsers},
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
		RDA_fprintf(fp,"                     XPERT MAKE SECURITY ERROR LIST          Date: %*s\n",8,date);
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
		APPlib2SupportRequest("SECURITY","mksec -> Security Update Package",errorlist);
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
		if(r.func!=NULL && (!RDAstrstr(r.name,"Make Std Users")) && (!RDAstrstr(r.name,"Set Allow Post Activity")))
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
	WriteBinaryVersion("SECURITY",CurrentBinaryVersion);
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
				prterr("ERROR: Report [%s] not in collection of defined reports for SECURITY.",argv[x]);
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

	if(InitializeSubsystems(argc,argv,module,"MAKE SECURITY")) 
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
	RDAmkdir("SECURITY",00770,&ret_int);
	RDAmkdir("guest",00770,&ret_int);
	RDAmkdir("admin",00770,&ret_int);
	RDAmkdir("staff",00770,&ret_int);
	mk_diagscrn();
	MAKE_SECURITY();
	if(MODSEC_FILENO!=(-1)) CLSNRD(MODSEC_FILENO);
	if(PROCSEC_FILENO!=(-1)) CLSNRD(PROCSEC_FILENO);
	if(FILESEC_FILENO!=(-1)) CLSNRD(FILESEC_FILENO);
	if(FLDSEC_FILENO!=(-1)) CLSNRD(FLDSEC_FILENO);
	BinaryVersionInstalled=GetBinaryVersion("SECURITY");
	CurrentBinaryVersion=1;
	if(argc>1)
	{
		make_screens(argc,argv);
		ShutdownSubsystems();
		return(-1);
	} else {
		mainrsrc=RDArsrcNEW(module,"MAKE SECURITY");
		scnlist=makescnlist();
		addstdrsrc(mainrsrc,"BINARY VERSION INSTALLED",LONGV,8,&BinaryVersionInstalled,FALSE);
		addstdrsrc(mainrsrc,"CURRENT BINARY VERSION",LONGV,8,&CurrentBinaryVersion,FALSE);
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
static void MAKE_SECURITY()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;

	scrn=RDAscrnNEW("SECURITY","MAKE SECURITY");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Binary Version Installed:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"BINARY VERSION INSTALLED","Version Installed","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,5,"","Installing Version:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"CURRENT BINARY VERSION","Current Binary Version","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
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
static void mk_diagscrn()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;

	scrn=RDAscrnNEW("SECURITY","DIAGNOSTIC SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"PROCESS NAME","Process Name","","",0,40,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"STATUS","Status","","",0,40,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
#ifdef USING_QT
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,22,"PROGRESS BAR","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
#endif /* USING_QT */
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Selected","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Total Read","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Total All","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"SELECTED","Selected","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,0,"TOTAL READ","Total Read","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,0,"TOTAL ALL","Total All","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Start Time","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Current","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Elapsed","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"START TIME","Start Time","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,0,"CURRENT TIME","Current Time","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,0,"ELAPSED TIME","Elapsed Time","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Increment in:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"INCREMENT","Increment","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,5,"","seconds minimums","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SUSPEND","Suspend","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"RESUME","Resume","","",0,0,0,"","","","");
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
static void MKGUEST()
{
	short msec=(-1),user=(-1);
	user=OPNNRD("SECURITY","USERS");
	msec=OPNNRD("SECURITY","MODSEC");
	ZERNRD(user);
	ZERNRD(msec);
	FINDFLDSETSTRING(user,"USER IDENTIFICATION","GUEST");
	if(!EQLNRD(user,1) || SIZNRD(user) || SIZNRD(msec))
	{
		/* Security Profiles Exist, do not add GUEST. */
	} else {
		/* No Security Profiles Exist, Add GUEST for Setup */
		ZERNRD(user);
		FINDFLDSETSTRING(user,"USER IDENTIFICATION","GUEST");
		FINDFLDSETSTRING(user,"USER NAME","GUEST OF RDA SYSTEMS");
#ifdef WIN32
		FINDFLDSETSTRING(user,"DEFAULT PRINTER","rdaout.exe");
#endif
#ifdef SCO50P
		FINDFLDSETSTRING(user,"DEFAULT PRINTER","rdaout.sco");
#endif
#ifdef RS6000
		FINDFLDSETSTRING(user,"DEFAULT PRINTER","rdaout.rsc");
#endif
#ifdef UNIXWARE
		FINDFLDSETSTRING(user,"DEFAULT PRINTER","rdaout.unw");
#endif
#ifdef LINUX
		FINDFLDSETSTRING(user,"DEFAULT PRINTER","rdaout.lnx");
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
		FINDFLDSETSTRING(user,"DEFAULT PRINTER","rdaout.lnx");
#endif
		WRTNRD(user,0,NULL);
		ZERNRD(msec);
		FINDFLDSETSTRING(msec,"USER IDENTIFICATION","GUEST");
		FINDFLDSETCHAR(msec,"ALL PROCESSES",TRUE);
		FINDFLDSETCHAR(msec,"ALL FILES",TRUE);
		FINDFLDSETCHAR(msec,"EXECUTE FUNCTIONS",TRUE);
		FINDFLDSETCHAR(msec,"READ DATA",TRUE);
		FINDFLDSETCHAR(msec,"WRITE DATA",TRUE);
		FINDFLDSETCHAR(msec,"PRINT DATA",TRUE);
		FINDFLDSETCHAR(msec,"DELETE DATA",TRUE);
		FINDFLDSETCHAR(msec,"DELETEFLAG",FALSE);
		FINDFLDSETSTRING(msec,"MODULE NAME","DATABASE");
		WRTNRD(msec,0,NULL);
		FINDFLDSETSTRING(msec,"MODULE NAME","GUI");
		WRTNRD(msec,0,NULL);
		FINDFLDSETSTRING(msec,"MODULE NAME","TRANS");
		WRTNRD(msec,0,NULL);
		FINDFLDSETSTRING(msec,"MODULE NAME","RPTGEN");
		WRTNRD(msec,0,NULL);
		FINDFLDSETSTRING(msec,"MODULE NAME","OLHELP");
		WRTNRD(msec,0,NULL);
		FINDFLDSETSTRING(msec,"MODULE NAME","RPTGEN");
		WRTNRD(msec,0,NULL);
		FINDFLDSETSTRING(msec,"MODULE NAME","SECURITY");
		WRTNRD(msec,0,NULL);
		FINDFLDSETSTRING(msec,"MODULE NAME","UTILITIES");
		WRTNRD(msec,0,NULL);
		FINDFLDSETSTRING(msec,"MODULE NAME","CONVERSION");
		WRTNRD(msec,0,NULL);
		FINDFLDSETSTRING(msec,"MODULE NAME","ARCHIVE");
		WRTNRD(msec,0,NULL);
	}
	CLSNRD(user);
	CLSNRD(msec);	
}
