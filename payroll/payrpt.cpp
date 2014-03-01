/* payrpt.c - Payroll's Make Reports */
#ifndef WIN32
#define __NAM__ "payrpt.lnx"
#endif
#ifdef WIN32
#define __NAM__ "payrpt.exe"
#endif
#include <app.hpp>
#include <rptgen.hpp>
/*CPP_OPART payrpt1 */
/*CPP_OPART payrpt2 */
/*CPP_OPART payrpt3 */
/*CPP_OPART payrpt4 */
/*CPP_OPART payrpt5 */
/*CPP_OPART payrpt6 */
/*CPP_OPART payrpt7 */
/*CPP_OPART payrpt8 */
/*CPP_OPART payrpt9 */
/*CPP_OPART payrpt10 */
/*CPP_OPART payrpt11 */
/*CPP_OPART payrpt12 */
/*CPP_OPART payrpt13 */
/*CPP_OPART payrpt14 */
/*CPP_OPART payrpt15 */
/*CPP_OPART payrpt16 */
/*CPP_OPART payrpt17 */
/*CPP_OPART payrpt18 */
/*CPP_OPART payrpt19 */
/*CPP_OPART payrpt20 */
/*CPP_OPART payrpt21 */
/*CPP_OPART payrpt22 */
/*CPP_OPART payrpt23 */
/*CPP_OPART payrpt24 */
/*CPP_OPART payrpt25 */
/*CPP_OPART payrpt26 */
/*CPP_OPART payrpt27 */
/*CPP_OPART payrpt28 */
/*CPP_OPART payrpt29 */
/*CPP_OPART payrpt30 */
/*CPP_OPART payrpt31 */
/*CPP_OPART payrpt32 */
/*CPP_OPART payrpt33 */
/*CPP_OPART payrpt34 */
/*CPP_OPART payrpt35 */
/*CPP_OPART payrpt36 */
/*CPP_OPART payrpt37 */
/*CPP_OPART payrpt38 */
/*CPP_OPART payrpt39 */
/*CPP_OPART payrpt40 */
/*CPP_OPART payrpt41 */
/*CPP_OPART payrpt42 */
/*CPP_OPART payrpt43 */
/*CPP_OPART payrpt44 */
/*CPP_OPART payrpt45 */
/*CPP_OPART payrpt46 */
/*CPP_OPART payrpt47 */
/*CPP_OPART payrpt48 */
/*CPP_OPART payrpt49 */
/*CPP_OPART payrpt50 */
/*CPP_OPART payrpt51 */
/*CPP_OPART payrpt52 */
/*CPP_OPART payrpt53 */
/*CPP_OPART payrpt54 */
/*CPP_OPART payrpt55 */
/*CPP_OPART payrpt56 */
/*CPP_OPART payrpt57 */
/*CPP_OPART payrpt58 */
/*CPP_OPART payrpt59 */
/*CPP_OPART payrpt60 */
/*CPP_OPART payrpt61 */
/*CPP_OPART payrpt62 */
/*CPP_OPART payrpt63 */
/*CPP_OPART payrpt64 */
/*ADDLIB mklib */
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="PAYROLL";
APPlib *rptlist=NULL;
void free_pvals(RDApval *,int);
void free_pvars(RDApvar *,int);
char *PAD_STRING(char,int);
static void make_reports(int,char **);
extern void pay.hppeck(void);
extern void payreg(void);
extern void dirdeptran11(void);
extern void DIRDEPDSK(void);
extern void DIRDEPDSK1(void);
extern void FEDERAL941(void);
extern void STATE941(void);
extern void PAYCKREG_LOC(void);
extern void CHGMSCSAL(void);
extern void CHGMSCSAL1(void);
extern void PREDIRDEP(void);
extern void PREDIRDEP1(void);
extern void DEDREGISTER(void);
extern void PAYCLCK(void);
extern void PAYCLCK1(void);
extern void PAYDIST(void);
extern void PAYDIST1(void);
extern void PAYDIST_WJC(void);
extern void PAYDIST1_WJC(void);
extern void PAYDSTE(void);
extern void PAYDSTE1(void);
extern void CHGDEDMAMTS(void);
extern void CHGDEDMAMTS1(void);
extern void CHGSTEPS(void);
extern void CHGSTEPS1(void);
extern void DEDVERIFY(void);
extern void DIRDDEPREG_WJC(void);
extern void DIRDEPRPT(void);
extern void FICAEARNINGS(void);
extern void FICAEARNINGS_SH(void);
extern void JOBSNOACC(void);
extern void JOBVERIFY(void);
extern void ADDDED(void);
extern void ADDDED1(void);
extern void VWUPDBR(void);
extern void VWUPDBR1(void);
extern void VWUPDBR_ET(void);
extern void VWUPDBR1_ET(void);
extern void VWCHECK_CCH(void);
extern void VWCHECK_WJC(void);
extern void VWAUDIT(void);
extern void VWAUDIT1(void);
extern void PAYVWR(void);
extern void PAYVWETR(void);
extern void PAYVWCR(void);
extern void PAYVOIDVW(void);
extern void PAYVDST(void);
extern void PAYVDST1(void);
extern void PAYCKREG(void);
extern void PAYCKREG_ID(void);
extern void PAYMTSA(void);
extern void PAYMTSA1(void);
extern void PAYMTSA_CCG(void);
extern void PAYMTSA_CCGK(void);
extern void PAYMTSA1_CCG(void);
extern void PAYNETA(void);
extern void PAYNETA1(void);
extern void PAYNETA_CCG(void);
extern void PAYNETA1_CCG(void);
extern void PAYSELA(void);
extern void PAYSELA1(void);
extern void PAYSELA_CHAMB(void);
extern void PAYSELA_CHAMBK(void);
extern void PAYSELA1_CHAMB(void);
extern void PAYUNAVW(void);
extern void COMBDED(void);

struct RPTSTRUCTs
{
	char *name;
	char *desc;
	void (*func)(...);
};

static struct RPTSTRUCTs RptAvl[]=
{
	{"PAYCHECK       ","Payroll Checks (MicroBUDGET Style)",pay.hppeck},
	{"PAYREG         ","Payroll Register",payreg},
	{"DEDREGISTER    ","Deduction Register",DEDREGISTER},
	{"DIRDEPTRAN11   ","Direct Deposit Transmittal (8.5x11)",dirdeptran11},
	{"DIRDEPDSK      ","Direct Deposit Diskette Report",DIRDEPDSK},
	{"DIRDEPDSK1     ","Direct Deposit Diskkete Ascii File",DIRDEPDSK1},
	{"941 FEDERAL    ","941 Federal Report",FEDERAL941},
	{"941 STATE      ","941 State Report",STATE941},
	{"PAYCKREG_LOC   ","Payroll Check Register by Location",PAYCKREG_LOC},
	{"CHGMSCSAL      ","Change Miscellaneous Salaries",CHGMSCSAL},
	{"CHGMSCSAL1     ","Change Miscellaneous Salaries Part 2",CHGMSCSAL1},
	{"PREDIRDEP      ","Prenotification of Direct Deposits",PREDIRDEP},
	{"PREDIRDEP1     ","Prenotification of Direct Deposits Part 2",PREDIRDEP1},
	{"PAYCLCK        ","Clear Payroll Checks",PAYCLCK},
	{"PAYCLCK1       ","Clear Payroll Checks Part 2",PAYCLCK1},
	{"PAYDIST        ","Payroll Distribution",PAYDIST},
	{"PAYDIST1       ","Payroll Distribution Part 2",PAYDIST1},
	{"PAYDIST_WJC    ","Payroll Distribution - WJC",PAYDIST_WJC},
	{"PAYDIST1_WJC   ","Payroll Distribution - WJC Part 1",PAYDIST1_WJC},
	{"PAYDSTE        ","Payroll Distribution with Disencumbrance",PAYDSTE},
	{"PAYDSTE1       ","Payroll Distribution with Disencumbrance Part 2",PAYDSTE1},
	{"CHGDEDMAMTS    ","Change Deduction Amounts",CHGDEDMAMTS},
	{"CHGDEDMAMTS1   ","Change Deduction Amounts Part 2",CHGDEDMAMTS1},	
	{"CHGSTEPS       ","Change Steps",CHGSTEPS},
	{"CHGSTEPS1      ","Change Steps Part 2",CHGSTEPS1},
	{"DEDVERIFY      ","Deduction Verification Report",DEDVERIFY},
	{"DIRDDEPREG_WJC ","Direct Deposit Register - WJC",DIRDDEPREG_WJC},
	{"DIRDEPRPT      ","Direct Deposit Report",DIRDEPRPT},
	{"FICAEARNINGS   ","Fica Earnings Report",FICAEARNINGS},
	{"FICAEARNINGS_SH","Fica Earnings Report",FICAEARNINGS_SH},
	{"JOBSNOACC      ","Jobs with No Accounts",JOBSNOACC},
	{"JOBVERIFY      ","Job Verification Report",JOBVERIFY},
	{"ADDDED         ","Add Deductions using Job Info",ADDDED},
	{"ADDDED1        ","Add Deductions using other Deds",ADDDED1},
	{"VWUPDBR        ","Vendor Withholding Update Bank Rec",VWUPDBR},
	{"VWUPDBR1       ","Vendor Withholding Update BnkRec Part 2",VWUPDBR1},
	{"VWUPDBR_ET     ","Vendor Withholding Upd. BnkRec Elect Trans",VWUPDBR_ET},
	{"VWUPDBR1_ET    ","Vendor Withholding Upd. BnkRec Elect Trans Part 2",VWUPDBR1_ET},
	{"VWCHECK_CCH    ","Vendor Withholding Check - CCH",VWCHECK_CCH},
	{"VWCHECK_WJC    ","Vendor Withholding Check - WJC",VWCHECK_WJC},
	{"VWAUDIT        ","Add Vendor Withholding Audit Trail",VWAUDIT},
	{"VWAUDIT1       ","Add Vendor Withholding Audit Trail Part 2",VWAUDIT1},	
	{"PAYVWR         ","Vendor Withholding Register",PAYVWR},
	{"PAYVWETR       ","Vendor Withholding Electronic Transfer Register",PAYVWETR},
	{"PAYVWCR        ","Vendor Withholding Check Register",PAYVWCR},
	{"PAYVOIDVW      ","Void Vendor Withholding Vouchers",PAYVOIDVW},
	{"PAYVDST        ","Vendor Withholding Distribution",PAYVDST},
	{"PAYVDST1       ","Vendor Withholding Distribution Part 2",PAYVDST1},
	{"PAYCKREG       ","Payroll Check Register",PAYCKREG},
	{"PAYCKREG_ID    ","Payroll Check Register by ID",PAYCKREG_ID},
	{"PAYMTSA        ","Make Transaction Audit Trail",PAYMTSA},
	{"PAYMTSA1       ","Make Transaction Audit Trail Part 2",PAYMTSA1},
	{"PAYMTSA_CCG    ","Make Transaction Audit Trail-CCG",PAYMTSA_CCG},
	{"PAYMTSA_CCGK   ","Make Transaction Audit Trail-CCG for paysel",PAYMTSA_CCGK},
	{"PAYMTSA1_CCG   ","Make Transaction Audit trail-CCG Part 2",PAYMTSA1_CCG},	
	{"PAYNETA        ","Compute Payroll Audit Trail",PAYNETA},
	{"PAYNETA1       ","Compute Payroll Audit Trail Part 2",PAYNETA1},
	{"PAYNETA_CCG    ","Compute Payroll Audit Trail-CCG",PAYNETA_CCG},
	{"PAYNETA1_CCG   ","Compute Payroll Audit Trail Part 2-CCG",PAYNETA1_CCG},
	{"PAYSELA        ","Select Payroll Audit Trail",PAYSELA},
	{"PAYSELA1       ","Select Payroll Audit Trail Part 2",PAYSELA1},
	{"PAYSELA_CHAMB  ","Select Payroll Audit Trail-CHAMB",PAYSELA_CHAMB},
	{"PAYSELA_CHAMBK ","Select Payroll Audit Trail-CHAMBK",PAYSELA_CHAMBK},
	{"PAYSELA1_CHAMB ","Select Payroll Audit Trail Part 2-CHAMB",PAYSELA1_CHAMB},
	{"PAYUNAVW       ","Undo Vendor Withholding Vouchers",PAYUNAVW},
	{"COMBDED        ","Combine Deductions w/Same Deduction Type",COMBDED}
};

char *PAD_STRING(char padchar,int length)
{
	int x=0;
	char *ret_string=NULL;

	ret_string=Rmalloc(length+1);
	for(x=0;x<length;++x)
	{
		ret_string[x]=padchar;
	}
	return(ret_string);
}

static APPlib *makerptlist()
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
static void quitfinrpt(RDArsrc *mainrsrc)
{
	if(rptlist!=NULL) freeapplib(rptlist);
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
}
static void selectrpt(RDArsrc *mainrsrc)
{
	int selected=0;
	struct RPTSTRUCTs r;

	FINDRSCGETINT(mainrsrc,"REPORT LIST",&selected);
	r=RptAvl[selected];
	if(r.func!=NULL)
	{
/*lint -e746 */
		r.func();
/*lint +e746 */
	}
}
static void selectall(RDArsrc *mainrsrc)
{
	int x;
	struct RPTSTRUCTs r;
	short did_it=FALSE;
	RDArsrc *tmprsrc=NULL;

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
	for(x=0;x<NE(RptAvl);++x)
	{
		r=RptAvl[x];
		if(r.func!=NULL)
		{
/*lint -e746 */
			r.func();
/*lint +e746 */
			did_it=TRUE;
		} else did_it=FALSE;
		if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(did_it ? TRUE:FALSE));
	}
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
		tmprsrc=NULL;
	}
}
static void make_reports(int argc,char **argv)
{
	int x,y;
	short did_it=FALSE;
	struct RPTSTRUCTs r;
	RDArsrc *tmprsrc=NULL;

	if(argc==2 && !RDAstrcmp(argv[1],"ALL REPORTS"))
	{
		selectall(NULL);
	} else {
		tmprsrc=odiagscrn("DIAGNOSTIC SCREEN",module,"Creating Standard PAYROLL Reports",NULL,(argc-1));
		if(tmprsrc!=NULL)
		{
			if(ADVmakescrn(tmprsrc,TRUE))
			{
				prterr("Error Cannot Create Diagnostic Screen.");
			} else {
				ForceWindowUpdate(tmprsrc);
			}
		}
		for(x=1;x<argc;++x)
		{
			for(y=0;y<NE(RptAvl);++y)
			{
				did_it=FALSE;
				r=RptAvl[y];
				if(!RDAstrcmp(argv[x],r.name))
				{
/*lint -e746 */
					r.func();
					did_it=TRUE;
/*lint +e746 */
					break;
				
				}
			}
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(did_it ? TRUE:FALSE));
		}
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
		}
	}
}
static void SCN_MAKE_STANDARD_PAYROLL_REPORTS()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("PAYROLL","MAKE STANDARD PAYROLL REPORTS");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","REPORTS","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"REPORT LIST","Report List","","",15,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT ALL","Select All","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Quit","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(29+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [PAYROLL] Screen [MAKE STANDARD PAYROLL REPORTS], Can Not Save Maintain Master!");
			prterr(temp1);
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
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	int x=0;

	if(InitializeSubsystems(argc,argv,module,"MAKE STANDARD PAYROLL REPORTS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if(argc>1)
	{
		make_reports(argc,argv);
		ShutdownSubsystems();
		exit(0);
	} else {
		SCN_MAKE_STANDARD_PAYROLL_REPORTS();
		mainrsrc=RDArsrcNEW(module,"MAKE STANDARD PAYROLL REPORTS");
		rptlist=makerptlist();
		addlstrsrc(mainrsrc,"REPORT LIST",&x,TRUE,NULL,rptlist->numlibs,
			&rptlist->libs,NULL);
		addbtnrsrc(mainrsrc,"SELECT ALL",TRUE,selectall,NULL);
		addbtnrsrc(mainrsrc,"SELECT",TRUE,selectrpt,NULL);
		addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfinrpt,NULL);
		addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
		addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		APPmakescrn(mainrsrc,TRUE,quitfinrpt,NULL,TRUE);
		RDAAPPMAINLOOP();
	}
}
