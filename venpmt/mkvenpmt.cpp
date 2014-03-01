#ifndef WIN32
#define __NAM__ "mkvenpmt.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mkvenpmt.exe"
#endif
#include <app.h>
#include <doheader.h>
#include <mkmsc.h>
#ifdef XXXXX
#include <mix.h>
#endif
/*CPP_OPART mkvend1 */
/*CPP_OPART mkvens1 */
/*CPP_OPART mkvenm1 */
/*CPP_OPART mkvenm2 */
/*CPP_OPART mkvenm3 */
/*CPP_OPART mkvenm4 */
/*CPP_OPART mkvenm5 */
/*CPP_OPART mkvenm6 */
/*CPP_OPART mkvenr1 */
/*ADDLIB mklib */
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="VENPMT";
APPlib *scnlist=NULL;
APPlib *errorlist=NULL;
void print_list(RDArsrc *,void (*)());
static void make_screens(int,char **);
static void mk_diagscrn(void);
static void MAKE_VENPMT(void);
/*
void free_pvals(RDApval *,int);
void free_pvars(RDApvar *,int);
*/
static void checkheader(FILE *,int *,int *,char *);
static void printheader(FILE *,int *,int *,char *,char *);

struct RPTSTRUCTs
{
        char *name;
        char *desc;
        void (*func)();
};
extern void DB_VENADR(void);
extern void DB_VENFYR(void);
extern void DB_VENINV(void);
extern void DB_VENMSR(void);
extern void DB_VENRMSR(void);
extern void DB_VENRVCH(void);
extern void DB_VENVCH(void);
extern void DB_VENYR(void);
/*
extern void MNU_VENPMT(void);
extern void MNU_VENPMT_PROCESS(void);
extern void MNU_VENPMT_REPORT(void);
extern void MNU_VENPMT_RECURRING(void);
extern void MNU_VENPMT_SETUP(void);
*/
extern void MTN_VENMSR(void);
extern void MTN_VENYR(void);
extern void MTN_VENADR(void);
extern void MTN_VENFYR(void);
extern void MTN_VENINV(void);
extern void MTN_VENVCH(void);
/*
extern void MTN_VENRMSR(void);
extern void MTN_VENRVCH(void);
extern void MTN_VENVCH_ADD(void);
extern void RPT_VENMSR(void);
extern void RPT_BIDDERLIST(void);
extern void RPT_VENADLAB(void);
extern void RPT_VENADDR(void);
extern void RPT_VENADDR2(void);
extern void RPT_VENADDR3(void);
extern void RPT_VENYR(void);
extern void RPT_VENYRSUM(void);
extern void RPT_VCH_ORDER(void);
extern void RPT_VENVCH(void);
extern void RPT_VENVCH2(void);
extern void RPT_VCAUDIT(void);
extern void RPT_TREASURE(void);
extern void RPT_VENCKREG(void);
extern void RPT_VENCKREGMO(void);
extern void RPT_VENCHKLAB(void);
extern void RPT_VENCHECK(void);
extern void RPT_VENCHECK2(void);
extern void RPT_VENCHECK3(void);
*/
extern void RPT_VCHDIST(void);
/*
extern void RPT_VCHREDIST(void);
extern void RPT_VENCLCK(void);
extern void RPT_UPDRECON(void);
extern void RPT_VOIDCHK(void);
extern void RPT_VCHPURGE(void);
*/
void STP_VENPMT(void);
void VIR_VENPMT(void);

/*
void xaddERRlist(APPlib *lst,char *string,int line,char *file)
{
	if(diagapps)
	{
		prterr("DIAG addERRlist adding element [%s] to list [%s] at line %d file %s",string,(lst==NULL?"NULL":"Error List"),line,file);
	}
	if(lst!=NULL)
	{
		if(lst->numlibs==1)
		{
			if(!RDAstrcmp(lst->libs[0],"No Errors Detected"))
			{
				freeapplib(lst);
				lst=APPlibNEW();
			}
		}
	} else {
		lst=APPlibNEW();
	}
	addAPPlib(lst,string);
}
*/

struct RPTSTRUCTs RptAvl[]=
{
	{"STP VENPMT    ","VENPMT Setup",STP_VENPMT},
	{"DB VENMSR     ","VENMSR Database",DB_VENMSR},
	{"DB VENADR     ","VENADR Database",DB_VENADR},
	{"DB VENYR      ","VENYR Database",DB_VENYR},
	{"DB VENFYR     ","VENFYR Database",DB_VENFYR},
	{"DB VENINV     ","VENINV Database",DB_VENINV},
	{"DB VENVCH     ","VENVCH Database",DB_VENVCH},
	{"DB VENRMSR    ","VENRMSR Database",DB_VENRMSR},
	{"DB VENRVCH    ","VENRVCH Database",DB_VENRVCH},
	{"VIR VENPMT    ","VENPMT Virtuals",VIR_VENPMT},
/*
	{"MNU VENPMT    ","Vendor Payments Main Menu",MNU_VENPMT},
	{"MNU PROCESS   ","Vendor Payments Process Menu",MNU_VENPMT_PROCESS},
	{"MNU REPORT    ","Vendor Payments Report Menu",MNU_VENPMT_REPORT},
	{"MNU RECURRING ","Vendor Payments Recurring Menu",MNU_VENPMT_RECURRING},
	{"MNU SETUP     ","Vendor Payments Setup Menu",MNU_VENPMT_SETUP},
*/
	{"MTN VENMSR    ","VENMSR Maintain Master",MTN_VENMSR},
	{"MTN VENADR    ","VENADR Maintain Master",MTN_VENADR},
	{"MTN VENYR     ","VENYR Maintain Master",MTN_VENYR},
	{"MTN VENFYR    ","VENFYR Maintain Master",MTN_VENFYR},
	{"MTN VENINV    ","VENINV Maintain Master",MTN_VENINV},
	{"MTN VENVCH    ","VENVCH Maintain Master",MTN_VENVCH},
/*
	{"MTN VENRMSR   ","VENRMSR Maintain Master",MTN_VENRMSR},
	{"MTN VENRVCH   ","VENRVCH Maintain Master",MTN_VENRVCH},
	{"ADD VENVCH    ","Add Vendor Vouchers Power Add",ADD_VENVCH},
	{"RPT VENMSR    ","Vendor Master Report",RPT_VENMSR},
	{"RPT BIDDER    ","Vendor Master Bidder Report",RPT_BIDDERLIST},
	{"RPT VENADLAB  ","Vendor Address Labels Report",RPT_VENADLAB},
	{"RPT VENADDR   ","Vendor Address Style 1 Report",RPT_VENADDR},
	{"RPT VENADDR2  ","Vendor Address Style 2 Report",RPT_VENADDR2},
	{"RPT VENADDR3  ","Vendor Address Style 3 Report",RPT_VENADDR3},
	{"RPT VENYR     ","Vendor Year Report",RPT_VENYR},
	{"RPT VENYRSUM  ","Vendor Year Summary Report",RPT_VENYRSUM},
	{"RPT VCH_ORDER ","Vouchers Order Report",RPT_VCH_ORDER},
	{"RPT VENVCH    ","Vendor Voucher Report",RPT_VENVCH},
	{"RPT VENVCH2   ","Vendor Voucher Report",RPT_VENVCH2},
	{"RPT VCAUDIT   ","Voucher Audit Trial Report",RPT_VCAUDIT},
	{"RPT TREASURE  ","Treasure Report",RPT_TREASURE},
	{"RPT VENCKREG  ","Vendor Check Register Report",RPT_VENCKREG},
	{"RPT VENCKREGMO","Vendor Monthly Check Register Report",RPT_VENCKREGMO},
	{"RPT VENCHKLAB ","Vendor Check Address Labels Report",RPT_VENCKLAB},
	{"RPT VENCHECK  ","Vendor Check Style 1 Report",RPT_VENCHECK},
	{"RPT VENCHECK2 ","Vendor Check Style 2 Report",RPT_VENCHECK2},
	{"RPT VENCHECK3 ","Vendor Check Style 3 Report",RPT_VENCHECK3},
*/
	{"RPT VCHDIST   ","Voucher Distribution Reports",RPT_VCHDIST},
/*
	{"RPT VCHREDIST ","After-the-Fact Distribution Report",RPT_VCHREDIST},
	{"RPT VENCLCK   ","Clear Checks Report",RPT_VENCLCK},
	{"RPT UPDRECON  ","Update Bank Recon Report",RPT_UPDRECON},
	{"RPT VOIDCHK   ","Void Vendor Check Report",RPT_VOIDCHK},
	{"RPT VCHPURGE  ","Perge Vouchers Report",RPT_VCHPURGE},
*/
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



/*
	lead_ws=(int)(RDAroundup((double)(line_length-(10+RDAstrlen(modname)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(10+RDAstrlen(modname)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	fprintf(fp,"%*s Module: %s %*sDate: %s\n",
		lead_ws,"",(modname!=NULL?modname:""),tail_ws,"",date);
	++*lines;
*/


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
/*
	char *date=NULL,*timex=NULL;
*/

	if(fp!=NULL)
	{
/*
		date=GETCURRENTDATE10();
		timex=GETCURRENTTIME();
		fprintf(fp,"                         XPERT MAKE VENPMT ERROR LIST           Date: %*s\n",10,date);
			fprintf(fp,"%*sExecuted By: %s%*sTime:   %s\n\n",
			 ((80-(14+RDAstrlen(USERLOGIN)))/2),
			"",USERLOGIN,
			(((80-(14+RDAstrlen(USERLOGIN)))/2)-14),"",
			timex);
		if(date!=NULL) Rfree(date);
		if(timex!=NULL) Rfree(timex);
*/
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

void print_list(RDArsrc *parent,void (*printfunc)())
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
		tmp=Rmalloc(RDAstrlen(r.name)+RDAstrlen(r.desc)+6);
		sprintf(tmp,"[%s] [%s]",r.name,r.desc);
		addAPPlib(a,tmp);
		if(tmp!=NULL) Rfree(tmp);
	}
	return(a);
}

static void quitfinrpt(RDArsrc *mainrsrc)
{
	if(scnlist!=NULL) freeapplib(scnlist);
	if(errorlist!=NULL) freeapplib(errorlist);
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
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
				prterr("ERROR: [%s] not in collection of defined setup values for [%s].",argv[x],(module!=NULL?module:""));
			}
		}
	}
}
static void MAKE_VENPMT()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	scrn=RDAscrnNEW("VENPMT","MAKE VENPMT");
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
		ADVaddwdgt(scrn,7,"RESOURCE LIST","Resource List","","",10,0,0,"","","","");
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
				addERRlist(errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
}
/*
void free_pvars(RDApvar *pvars,int numpvars)
{
	RDApvar *pvar=NULL;

	int y=0;

	if(pvars!=NULL)
	{
        	for(y=0,pvar=pvars;y<numpvars;++y,++pvar)
        	{
                	if(pvar->name!=NULL) Rfree(pvar->name);
                	if(pvar->command!=NULL) Rfree(pvar->command);
        	}
        	Rfree(pvars);
	}
}
void free_pvals(RDApval *pvals,int numpvals)
{
	RDApval *pval=NULL;
	int y=0;

	if(pvals!=NULL)
	{
        	for(y=0,pval=pvals;y<numpvals;++y,++pval)
        	{
                	if(pval->name!=NULL) Rfree(pval->name);
                	if(pval->fieldname!=NULL) Rfree(pval->fieldname);
        	}
        	Rfree(pvals);
	}
}
*/
static void mk_diagscrn()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	scrn=RDAscrnNEW("VENPMT","DIAGNOSTIC SCREEN");
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
/*
			ERRORDIALOG("Cannot Save Screen","This screen may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
*/
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(RDAstrlen(scrn->module)+RDAstrlen(scrn->name)+100+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [%s] Screen [%s], Can Not Save Screen!",(scrn->module!=NULL?scrn->module:""),(scrn->name!=NULL?scrn->name:""));
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
}
void STP_VENPMT()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	scrn=RDAscrnNEW("VENPMT","VENPMT SETUP");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"USE VENPMT FUND","Use Clearing Fund","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","EIN #:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"EIN NUMBER","EIN Number","","",0,20,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"USE FISCAL SUMMARY","Use Fiscal Summary","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"USE FISCAL MONTHS","Use Fiscal Months (Pre-Year through Post Year)","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"USE TAXES","Use Taxes","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"USE TAX1","Use Tax1","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"SEPERATE TAX1 VENDOR","Seperate","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Vendor ID:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"DEFAULT TAX1 VENDOR","Default Tax1 Vendor","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,5,"","Address ID:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"DEFAULT TAX1 ADDRESS","Default Tax1 Address","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,5,"","Rate:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TAX1 RATE","Tax1 Rate","","",0,10,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"USE TAX2","Use Tax2","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"SEPERATE TAX2 VENDOR","Seperate","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Vendor ID:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"DEFAULT TAX2 VENDOR","Default Tax2 Vendor","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,5,"","Address ID:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"DEFAULT TAX2 ADDRESS","Default Tax2 Address","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,5,"","Rate:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TAX2 RATE","Tax2 Rate","","",0,10,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"USE TAX3","Use Tax3","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"SEPERATE TAX3 VENDOR","Seperate","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Vendor ID:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"DEFAULT TAX3 VENDOR","Default Tax3 Vendor","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,5,"","Address ID:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"DEFAULT TAX3 ADDRESS","Default Tax3 Address","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,5,"","Rate:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TAX3 RATE","Tax3 Rate","","",0,10,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"USE TAX4","Use Tax4","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"SEPERATE TAX4 VENDOR","Seperate","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Vendor ID:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"DEFAULT TAX4 VENDOR","Default Tax4 Vendor","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,5,"","Address ID:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"DEFAULT TAX4 ADDRESS","Default Tax4 Address","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,5,"","Rate:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TAX4 RATE","Tax4 Rate","","",0,10,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"USE TAX5","Use Tax5","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"SEPERATE TAX5 VENDOR","Seperate","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Vendor ID:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"DEFAULT TAX5 VENDOR","Default Tax5 Vendor","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,5,"","Address ID:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"DEFAULT TAX5 ADDRESS","Default Tax5 Address","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,5,"","Rate:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TAX5 RATE","Tax5 Rate","","",0,10,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Admin Fee:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,8,"ADMIN FEE","Admin Fee","","",4,80,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,0,"","","","");
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
				addERRlist(errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);

			if(defdir!=NULL) Rfree(defdir);
			if(scrn!=NULL) free_scrn(scrn);
		} else {
			if(defdir!=NULL) Rfree(defdir);
			if(scrn!=NULL) free_scrn(scrn);
			Execute2Program("venstp",NULL);
		}
	}
}
void VIR_VENPMT()
{
	APPlib *args=NULL;

	args=APPlibNEW();
	addAPPlib(args,"VENPMT");
	addAPPlib(args,"CREATE VIRTUALS");
	ADVExecute2Program("mkvirt",args,NULL);
	if(args!=NULL) freeapplib(args);
}
void main(int argc,char **argv)
{
	RDArsrc *mainrsrc=NULL;
	int x=0;

	if(InitializeSubsystems(argc,argv,module,"MAKE VENPMT")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if(XPERT_SETUP==NULL)
	{
		prterr("ERROR: Make Utilities can not load Xpert Setup, exiting abnormally");
		exit(1);
	}
	if(XPERT_SETUP->VENPMT==TRUE)
	{
		RDAmkdir("VENPMT",00770,&ret_int);
		mk_diagscrn();
		if(argc>1)
		{
			make_screens(argc,argv);
			ShutdownSubsystems();
			exit(0);
		} else {
			MAKE_VENPMT();
			mainrsrc=RDArsrcNEW(module,"MAKE VENPMT");
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
		exit(1);
	}
}
