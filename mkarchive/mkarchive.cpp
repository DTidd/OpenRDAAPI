/**********************************************************************
                         MAKE XPERT ARCHIVE
                              mkbudprep.c
**********************************************************************/
#ifndef WIN32
#define __NAM__ "mkarchive.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mkarchive.exe"
#endif
#include <app.hpp>
#include <doheader.hpp>
#include <mkmsc.hpp>
#include <fin.hpp>
#include <curver.hpp>
#include <mkvir.hpp>

/*CPP_OPART mkarchive1 */
/*CPP_OPART mkarchive2 */
/*CPP_OPART mkarchive3 */
/*CPP_OPART mkarchive4 */
/*CPP_OPART mkarchive5 */
/*CPP_OPART mkarchive6 */
/*CPP_OPART mkarchive7 */
/*CPP_OPART mkarchive8 */
/*CPP_OPART mkarchive9 */
/*CPP_OPART mkarchive10 */
/*CPP_OPART mkarchive11 */
/*CPP_OPART mkarchive12 */
/*CPP_OPART mkarchive13 */
/*CPP_OPART mkarchive14 */
/*CPP_OPART mkarchive15 */
/*CPP_OPART mkarchive16 */
/*CPP_OPART mkarchive17 */
/*CPP_OPART mkarchive18 */
/*CPP_OPART mkarchive19 */
/*CPP_OPART mkarchive20 */
/*CPP_OPART mkarchive21 */
/*CPP_OPART mkarchive22 */
/*CPP_OPART mkarchive23 */
/*ADDLIB cver */
/*ADDLIB chglib */
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


#ifndef LINUX
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#ifdef LINUX
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */

RDAfinmgt *FINMGT_SETUP=NULL;
char *module="ARCHIVE";
APPlib *scnlist=NULL;
APPlib *errorlist=NULL;
void print_list(RDArsrc *,void (*)(...));
static void make_screens(int,char **);
static void mk_diagscrn(void);
static void MAKE_ARCHIVE(void);
static void RUN_ARCGEN(void);
static void checkheader(FILE *,int *,int *,char *);
static void printheader(FILE *,int *,int *,char *,char *);
static void RUN_AUTOWRKGRP(void);

char *swb_utl_deftype="((GENERIC_SETUP_VARIABLE(\"[UTILITIES][SWB_S4_MULTIPLE_DEFINITION_TYPES]\"))) OR ([SOFTWARE TYPE]=0)";
char *swb_utl3_clrfund="((GENERIC_SETUP_VARIABLE(\"[UTILITIES][SWB_S3_CLEARING_FUND]\"))) OR ([SOFTWARE TYPE]=0)";
char *swb_utl5_rgd="((GENERIC_SETUP_VARIABLE(\"[UTILITIES][SWB_S5_GROUP_DEFAULTS]\"))) OR ([SOFTWARE TYPE]=0)";

struct RPTSTRUCTs
{
        char *name;
        char *desc;
        void (*func)(...);
};

extern void MAKE_BACKUPFOLDER(void);
extern void DB_ARCHCLS(void),DB_ARCHSUB(void),CreateARCHCLS(void);
extern void DB_ARCHIVES(void),DB_ARCHTYPE(void),ARCGEN(void),ADDARCHIVE(void);
extern void MTNARCHTYPE(void),MTNARCHIVES(void),ARCHIVEMENU(void);
extern void MakeARCHTYPE(void),PURGEARCHIVE(void),WORKGROUPS(void);
extern void SCANSCRIPT(void),EMAILARCHIVE(void),ADDWORKGROUP(void);
extern void SENDATTACHED(void),DB_ARCHUSER(void),DB_ARCHWORK(void);
extern void MTNARCHWORK(void),MTNARCHUSER(void),WORKGROUPUSER(void);
extern void RPTARCHIVE(void),COPYARCHIVE(void),COPYGROUPUSER(void);
extern void ARCHIVE_IE(void),MTNARCHCLS(void),MTNARCHSUB(void);
extern void CreateARCHSUB(void),MTNARCHPROG(void),ADDARCHPROG(void);
extern void DB_ARCHPROG(void),MAKE_ARCHPROG(void),MANAGESCANS(void);
extern void AUTOWRKGRP(void),RPTAUTOWRKGRPDEPT(void);
#ifdef USING_QT
extern void CREATE_MENUITEM(void);
#endif /* USING_QT */

struct RPTSTRUCTs RptAvl[]=
{
	{"ARCHCLS            ","ARCHCLS  Database",DB_ARCHCLS},
	{"ARCHIVES           ","ARCHIVES Database",DB_ARCHIVES},
	{"ARCHPROG           ","ARCHPROG Database",DB_ARCHPROG},
	{"ARCHSUB            ","ARCHSUB  Database",DB_ARCHSUB},
	{"ARCHTYPE           ","ARCHTYPE Database",DB_ARCHTYPE},
	{"ARCHUSER           ","ARCHUSER Database",DB_ARCHUSER},
	{"ARCHWORK           ","ARCHWORK Database",DB_ARCHWORK},
	{"MAKE STD ARCHCLS   ","Create Standard Archive Classes",CreateARCHCLS},
	{"MAKE STD ARCHSUB   ","Create Standard Archive Subjects",CreateARCHSUB},
	{"MAKE BACKUPFOLDER  ","Create BACKUPS Work Group/Folder",MAKE_BACKUPFOLDER},
	{"MAKE STD ARCHTYPES ","Create Standard Archive Types",MakeARCHTYPE},
	{"MAKE ARCHPROG      ","Create Standard Viewing Programs",MAKE_ARCHPROG},
	{"GENERIC SETUP VARS ","GENERIC SETUP VARIABLES",ARCGEN},
	{"RUN GENERIC SETUP  ","Execute Generic Setup Variables",RUN_ARCGEN},
	{"SCANDOCS SCRIPT    ","SCANDOCS Script",SCANSCRIPT},
	{"SENDATTACHED       ","SENDATTACHED Scripts",SENDATTACHED},
	{"MTN ARCHCLS        ","ARCHCLS  Maintain Master",MTNARCHCLS},
	{"MTN ARCHIVES       ","ARCHIVES Maintain Master",MTNARCHIVES},
	{"MTN ARCHPROG       ","ARCHPROG Maintain Master",MTNARCHPROG},
	{"MTN ARCHSUB        ","ARCHSUB  Maintain Master",MTNARCHSUB},
	{"MTN ARCHTYPE       ","ARCHTYPE Maintain Master",MTNARCHTYPE},
	{"MTN ARCHUSER       ","ARCHUSER Maintain Master",MTNARCHUSER},
	{"MTN ARCHWORK       ","ARCHWORK Maintain Master",MTNARCHWORK},
	{"MANAGE SCANS       ","Manage Scans Power Add",MANAGESCANS},
	{"AUTOWRKGRP         ","Create Auto-Magic Work Groups/Users",AUTOWRKGRP},
	{"RPT AUTOWRKGRPDEPT ","AUTOWRKGRPDEPT Report Definition   ",RPTAUTOWRKGRPDEPT},
	{"ADD WORK GROUP     ","Work Group Power Add",ADDWORKGROUP},
	{"ADD ARCHIVE        ","Archive Power Add",ADDARCHIVE},
	{"EMAILARCHIVE       ","Email Archive(s)",EMAILARCHIVE},
	{"PURGEARCHIVE       ","Purge Archive    Report Definition",PURGEARCHIVE},
	{"COPYARCHIVE        ","Copy/Move Archive",COPYARCHIVE},
	{"COPYGROUPUSER      ","Copy Work Group Users",COPYGROUPUSER},
	{"ADD ARCHPROG       ","Add Archive Programs",ADDARCHPROG},
	{"RPTARCHIVE         ","Archive          Report Definition",RPTARCHIVE},
	{"WORK GROUPS REPORT ","Work Groups      Report Definition",WORKGROUPS},
	{"WORK GROUP USERS   ","Work Group Users Report Definition",WORKGROUPUSER},
	{"ARCHIVE IMP/EXP    ","Archive Import/Export Functions",ARCHIVE_IE},
	{"ARCHIVE MENU       ","Archive Menu",ARCHIVEMENU},
	{"RUN AUTOWRKGRP     ","Execute Create Auto Work Groups/Users",RUN_AUTOWRKGRP},
#ifdef USING_QT
	{"CREATE_MENUITEM    ","Create OpenRDA Menu Items",CREATE_MENUITEM},
#endif /* USING_QT */
};

static void checkheader(FILE *fp,int *lines,int *pages,char *modname)
{    
	char *temp=NULL;

	if(doheaders)
	{
		if(*lines>57)
		{
			fprintf(fp,"\f");
			temp=Rmalloc(RDAstrlen(modname)+23);
			sprintf(temp,"XPERT MAKE %s ERROR LIST",
				(modname!=NULL?modname:""));
			printheader(fp,lines,pages,temp,modname);
			if(temp!=NULL) Rfree(temp);
		}
	}
}
static void printheader(FILE *fp,int *lines,int *pages,char *header,char *modname)
{
	char *date=NULL,*timex=NULL;
	int lead_ws=0,tail_ws=0;
	unsigned line_length=79;
	char *mainheader=NULL;

	date=GETCURRENTDATE10();
	timex=GETCURRENTTIME();
	fprintf(fp,"\n");
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
	fprintf(fp,"%*s %s %*sPage: %*d\n",
		lead_ws,"",mainheader,tail_ws,"",10,++*pages);
	++*lines;
	if(mainheader!=NULL) Rfree(mainheader);


	lead_ws=(int)(RDAroundup((double)(line_length-(2+RDAstrlen(header)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(2+RDAstrlen(header)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(header)+2+16+lead_ws+tail_ws)>line_length) header[(line_length-16-lead_ws-tail_ws)]=0;
	fprintf(fp,"%*s %s %*sDate: %s\n",
		lead_ws,"",header,tail_ws,"",date);
	++*lines;



	lead_ws=(int)(RDAroundup(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(USERLOGIN)+16+15+lead_ws+tail_ws)>line_length)
	{
		fprintf(fp,"%*s %s %*sTime:   %s\n",
			(int)(RDAroundup(((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2))),"",
			USERLOGIN,
			(int)(Rint((((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2)-16))),"",
			timex);
			++*lines;
	} else {
		fprintf(fp,"%*s Executed By: %s %*sTime:   %s\n",
			lead_ws,"",USERLOGIN,tail_ws,"",timex);
		++*lines;
	}

	fprintf(fp,"\n");
	++*lines;
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
}
static void quit_print(RDArsrc *prsrc)
{
	if(diaggui)
	{
		prterr("DIAG quit_print Quitting Print Screen Definition.");
	}
	if(prsrc!=NULL) free_rsrc(prsrc);
}

static void printerrorlist(FILE *fp,char *modname,int *lines,int *pages)
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
			fprintf(fp,"(%5d) %s\n",(x+1),errorlist->libs[x]);
			++*lines;
			checkheader(fp,lines,pages,modname);
		}
		fprintf(fp,"\f");
	}
}

void printerrorlistcb(RDArsrc *prsrc,RDArsrc *parent)
{
	char *outdevice=NULL,*message=NULL;
	FILE *fp=NULL;
	int lines=0,pages=0;

	readwidget(prsrc,"DEVICE");
	FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
	if(outdevice!=NULL)
	{
		fp=popen(outdevice,"w");
		if(fp!=NULL)
		{
			printerrorlist(fp,module,&lines,&pages);
			pclose(fp);
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
	char *defaultprinter=NULL;

	if(diaggui)
	{
		prterr("DIAG printerrorlist Displaying the Print Screen in preparation to print Error List.");
	}
	prsrc=RDArsrcNEW("GUI","PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
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
		sprintf(tmp,"%4d [%s] [%s]",(x+1),r.name,r.desc);
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
		APPlib2SupportRequest("ARCHIVE","mkarchive -> F.A.S.T. Update Package",errorlist);
	}
	if(errorlist!=NULL) freeapplib(errorlist);
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	if(FINMGT_SETUP!=NULL) free_finmgt(FINMGT_SETUP); 
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
		temp=Rmalloc(RDAstrlen(r.desc)+18);
		sprintf(temp,"Creating %s",(r.desc!=NULL?r.desc:""));
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
		tmprsrc=odiagscrn("DIAGNOSTIC SCREEN",module,"Creating Standards",NULL,NE(RptAvl));
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
		if(r.func!=NULL && strncmp(r.name,"RUN AUTOWRKGRP",14))
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
	WriteBinaryVersion(module,CurrentBinaryVersion);
	Establish_Install_Date(module);
}
static void make_screens(int argc,char **argv)
{
	int x,y;
	struct RPTSTRUCTs r;
	char *temp1=NULL,*temp2=NULL;

	if(argc==2 && !RDAstrcmp(argv[1],"ALL"))
	{
		selectall(NULL);
	} else {
		for(x=1;x<argc;++x)
		{
			for(y=0;y<NE(RptAvl);++y)
			{
				r=RptAvl[y];
				temp1=stralloc(argv[x]);
				temp2=stralloc(r.name);
				unpad(temp1);
				unpad(temp2);
				if(!RDAstrcmp(temp1,temp2))
				{
/*lint -e746 */
					r.func();
/*lint +e746 */
					break;
				
				}
				if(temp1!=NULL) Rfree(temp1);
				if(temp2!=NULL) Rfree(temp2);
			}
			if(y>=NE(RptAvl))
			{
				prterr("ERROR: [%s] not in collection of defined setup values for [%s].",argv[x],(module!=NULL?module:""));
			}
		}
	}
}
static void MAKE_ARCHIVE()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	scrn=RDAscrnNEW("ARCHIVE","MAKE ARCHIVE");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Binary Version Installed:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"BINARY VERSION INSTALLED","Version Installed","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,5,"","Installing Version:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"CURRENT BINARY VERSION","Current Binary Version","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","Resource List","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"RESOURCE LIST","Resource List","","",10,0,0,"","","","");
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
		ADVaddwdgt(scrn,7,"ERROR LIST","Error List","","",5,0,0,"","","","");
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
static void mk_diagscrn()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	scrn=RDAscrnNEW("ARCHIVE","DIAGNOSTIC SCREEN");
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
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	int x=0;
	int ret_int=0;

	if(InitializeSubsystems(argc,argv,module,"MAKE ARCHIVE")) 
	{
		ShutdownSubsystems();
		return(-1);
	}
	if(XPERT_SETUP==NULL)
	{
		prterr("ERROR: Make Utilities can not load XPERT Setup, exiting abnormally");
		ShutdownSubsystems();
		return(-1);
	}
	BinaryVersionInstalled=GetBinaryVersion(module);
	CurrentBinaryVersion=5;
	if(XPERT_SETUP->ARCHIVE==TRUE)
	{
		FINMGT_SETUP=RDAfinmgtNEW();
		getfinmgtbin(FINMGT_SETUP);
		RDAmkdir("ARCHIVE",00770,&ret_int);
		RDAmkdir("ARCHIVE/.BACKUPS",00770,&ret_int);
		RDAmkdir("scan",00770,&ret_int);

		mk_diagscrn();
		MAKE_ARCHIVE();
		if(argc>1)
		{
			make_screens(argc,argv);
			if(FINMGT_SETUP!=NULL) free_finmgt(FINMGT_SETUP); 
			ShutdownSubsystems();
		} else {
			mainrsrc=RDArsrcNEW(module,"MAKE ARCHIVE");
			addstdrsrc(mainrsrc,"BINARY VERSION INSTALLED",LONGV,8,&BinaryVersionInstalled,FALSE);
			addstdrsrc(mainrsrc,"CURRENT BINARY VERSION",LONGV,8,&CurrentBinaryVersion,FALSE);
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
			RDAAPPMAINLOOP();
		}
	} else {
		prterr("ERROR: Client doesn't have [%s] module marked as purchased.",module);
		ShutdownSubsystems();
	}
}
static void RUN_ARCGEN()
{
	APPlib *args=NULL;

	args=APPlibNEW();
	addAPPlib(args,"ARCHIVE");
	ADVExecute2Program("dogsv",args,NULL);
	if(args!=NULL) freeapplib(args);
}
static void RUN_AUTOWRKGRP()
{
	APPlib *args=NULL;
	char *libx=NULL,a=FALSE;
	RDAGenericSetup *gsv=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("ARCHIVE")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"ARCHIVE");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"ARCHIVE");
#endif

	gsv=RDAGenericSetupNew("ARCHIVE","AUTOMATIC REPORT CAPTURE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			a=*gsv->value.string_value;
		} else {
			a=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	if(a==TRUE)
	{
		args=APPlibNEW();
		addAPPlib(args,"ARCHIVE");
		addAPPlib(args,"AUTOWRKGRP");
		ADVExecute2Program("doreport",args,NULL);
		if(args!=NULL) freeapplib(args);
	}
}
