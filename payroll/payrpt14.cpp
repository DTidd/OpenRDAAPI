/* PAYROLL's - Make (PAYCLCK) Report */
#include <cstdio>
#include <rptgen.hpp>
/*COMPMEM 40960 */

extern void free_pvals(RDApval *,int);
extern void free_pvars(RDApvar *,int);
extern char *module;
void PAYCLCK(void);
static void PAYCLCK_RANGE_SCREEN(void);

void PAYCLCK()
{
	int x;
	RDAreport *tmp=NULL;
	RDAsearch *s=NULL;
	RDApval *pval=NULL,*pvals=NULL;
/*
	int y,z;
	short longest=132;
	int numpvars=0;
	DFvirtual *d=NULL;
	RDAacct *acc=NULL;
	RDAaccpart *part=NULL;
*/
	char *t=NULL,*desc=NULL,*rptname=NULL;
	RDApvar *pvars=NULL;




/*======================================================================
Creating RDAreport Structure
RDAreport *xRDAreportNEW(char *name,char *title,char *desc,short engine,
	char *server_name,short process_exceptions,short use_print_screen,
	short stop_on_error,short ask_b4exe,short chain_report,
	char *chain_module,char *chain_name,short range_screen,
	short diag_level,short removesort,int line,
	char *file)

diag_level Values:
	0=Diagnostic Not-Breakable
	1=Diagnostic Breakable
	2=No Diagnostic
----------------------------------------------------------------------*/

	memset(stemp,0,101);
	rptname=Rmalloc(7+1);
	sprintf(rptname,"PAYCLCK");
	t=Rmalloc(20+1);
	sprintf(t,"Clear Payroll Checks");
	desc=Rmalloc(91+1);
	sprintf(desc,"This output definition is used to clear the payroll checks and corresponding deductions....");
	tmp=RDAreportNEWADV(rptname,t,desc,1,"",FALSE,TRUE,FALSE,TRUE,TRUE,"PAYROLL","PAYCLCK1",TRUE,1,FALSE,"");
	if(t!=NULL) Rfree(t);
	if(desc!=NULL) Rfree(desc);




/*========================================================================
Creating RDAdisplay Structure
void xadddisplay(RDAreport *report,double page_width,double page_length,
	short device_type,short alignment,short testpages,char *devicename,
	short bodycount,short starting_page_no,
	int numpvars,RDApvar *pvars,int line,char *file)

Device Type(s) - Valid Device Types
	0 - PRINTER
	1 - ASCII FILE
	2 - APPEND ASCII FILE
	3 - DATAFILE
	4 - UPDATE DATAFILE
------------------------------------------------------------------------*/

	pvars=NULL;
	adddisplayADV(tmp,(double)8.500000,(double)10.000000,0,FALSE,0,"dfx1",0,1,0,pvars);
	free_pvars(pvars,0);

/*----------------------------------------------------------------------*/




	tmp->select=NULL;




/*========================================================================
Creating RDAsearch Structures

int xaddsearchline(RDAreport *report,char *mod,char *file,
	int connect_type,char *connect_mod,char *connect_file,
	short num_confld,RDAconfld *confld,
	char *subkeyname,short range,char *expression,short numwrites,
	int line,char *filex)
#define addsearchfile(report,mod,file,connect_type,connect_mod,connect_file,keyname,range?,expression,number_reads/writes)
#define addsearchfile(a,b,c,d,e,f,g,h,i,j)   xaddsearchline(a,b,c,d,e,f,0,NULL,g,h,i,j,__LINE__,__FILE__)

int xaddsearchconfld(RDAsearch *s,char *fldname,char *conname,int line,
	char *file)
#define addsearchconfld(a,b,c) xaddsearchconfld(a,b,c,__LINE__,__FILE__)
------------------------------------------------------------------------*/

	t=Rmalloc(157+1);
	sprintf(t,"([PAYROLL][PAYJTRN][DELETEFLAG]=FALSE) AND \n([PAYROLL][PAYJTRN][CLEARED]=FALSE) AND \n([PAYROLL][PAYJTRN][PAY IDENTIFICATION]=[SELECTED PAY IDENTIFICATION])");
	x=addsearchfileADV(tmp,"PAYROLL","PAYJTRN",0,NULL,NULL,"RDA PAYJTRN KEY",TRUE,t,NULL,2);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PAY IDENTIFICATION","[SELECTED PAY IDENTIFICATION]");
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","");
		addsearchconfld(s,"JOB NUMBER","");
		addsearchconfld(s,"ACCOUNT NUMBER","");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(36+1);
	sprintf(t,"[PAYROLL][PAYJPMS][DELETEFLAG]=FALSE");
	x=addsearchfileADV(tmp,"PAYROLL","PAYJPMS",2,"PAYROLL","PAYJTRN","PAYJPMS KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJTRN][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYJTRN][JOB NUMBER]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJTRN][PAY IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(38+1);
	sprintf(t,"([PAYROLL][PAYMSTR][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYMSTR",2,"PAYROLL","PAYJTRN","PAYMSTR KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJTRN][PAY IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(36+1);
	sprintf(t,"([PRSNNL][PERDMG][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"PRSNNL","PERDMG",2,"PAYROLL","PAYJTRN","PERDMG KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJTRN][PERSONNEL IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(36+1);
	sprintf(t,"([FINMGT][FINBYR][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"FINMGT","FINBYR",2,"PAYROLL","PAYJTRN","FINBYR KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[PAYJTRN][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
		addsearchconfld(s,"FISCAL YEAR","[FISCAL YEAR]");
	}
	if(t!=NULL) Rfree(t);

	x=addsearchfileADV(tmp,"FINMGT","FINBACC",1,"PAYROLL","PAYJTRN","ACCOUNT CODE KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[PAYJTRN][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
	}

	x=addsearchfileADV(tmp,"FINMGT","FINYEAR",1,"FINMGT","FINBYR","FINYEAR KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"FISCAL YEAR","[FINBYR][FISCAL YEAR]");
	}

	x=addsearchfileADV(tmp,"FINMGT","FINCOA",1,"PAYROLL","PAYJTRN","FINCOA KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"ACCOUNT TYPE","[ACCOUNT TYPE]");
		addsearchconfld(s,"DEFINITION TYPE","[PAYJTRN][DEFINITION TYPE]");
		addsearchconfld(s,"DIMENSION","[DIMENSION #0]");
		addsearchconfld(s,"NAME","[DIMENSION #0 CODE]");
	}

	x=addsearchfileADV(tmp,"FINMGT","FINCOA",1,"PAYROLL","PAYJTRN","FINCOA KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"ACCOUNT TYPE","[ACCOUNT TYPE]");
		addsearchconfld(s,"DEFINITION TYPE","[PAYJTRN][DEFINITION TYPE]");
		addsearchconfld(s,"DIMENSION","[DIMENSION #1]");
		addsearchconfld(s,"NAME","[DIMENSION #1 CODE]");
	}





/*========================================================================
Creating RDAvirtual Structures
int xaddvirtualline(RDAreport *r,char *name,int type,short len,
	char *expression,short when,short range,int line,char *file)
FIELD TYPES: rdafld.h 
	#define VARIABLETEXT            0
	#define PLAINTEXT               1
	#define DOLLARS                 2
	#define PHONE                   3
	#define CUSTOMTYPE              4
	#define ZIPCODE                 5
	#define DECIMALV                6
	#define SHORTV                  7
	#define LONGV                   8
	#define DOUBLEV                 9
	#define BOOLNS                  10
	#define CHARACTERS              11
	#define BUTTONS                 12
	#define SCROLLEDLIST            13
	#define SCROLLEDTEXT            14
	#define SOCSECNUM               15
	#define DATES                   16
	#define TIMES                   17
	#define SSHORTV                 18
	#define SLONGV                  19
	#define SDECIMALV               20
	#define SDOUBLEV                21
	#define EXPENDITURE             22
	#define REVENUE                 23
	#define BALANCESHEET            24
	#define BEGINNINGBALANCE        25
	#define OPTIONALFIELDS          26
	#define DOLLARS_NOCENTS         27
------------------------------------------------------------------------*/

	if(t!=NULL) Rfree(t);
	addvirtuallineADV(tmp,"SELECTED PAY IDENTIFICATION",1,15,t,0,2);

	if(t!=NULL) Rfree(t);
	addvirtuallineADV(tmp,"CLEAR PROCESSING MONTH",7,2,t,0,2);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(1025+1);
	sprintf(t,"IF([CLEAR PROCESSING MONTH]=0) THEN RETURN_VALUE=\"PRE-YEAR\" \nELSE IF([CLEAR PROCESSING MONTH]=1) THEN RETURN_VALUE=\"JANUARY\" \nELSE IF([CLEAR PROCESSING MONTH]=2) THEN RETURN_VALUE=\"FEBRUARY\" \nELSE IF([\
CLEAR PROCESSING MONTH]=3) THEN RETURN_VALUE=\"MARCH\" \nELSE IF([CLEAR PROCESSING MONTH]=4) THEN RETURN_VALUE=\"APRIL\" \nELSE IF([CLEAR PROCESSING MONTH]=5) THEN RETURN_VALUE=\"MAY\" \nELSE IF([CLEAR PROCESS\
ING MONTH]=6) THEN RETURN_VALUE=\"JUNE\" \nELSE IF([CLEAR PROCESSING MONTH]=7) THEN RETURN_VALUE=\"JULY\" \nELSE IF([CLEAR PROCESSING MONTH]=8) THEN RETURN_VALUE=\"AUGUST\" \nELSE IF([CLEAR PROCESSING MONTH]=9\
) THEN RETURN_VALUE=\"SEPTEMBER\" \nELSE IF([CLEAR PROCESSING MONTH]=10) THEN RETURN_VALUE=\"OCTOBER\" \nELSE IF([CLEAR PROCESSING MONTH]=11) THEN RETURN_VALUE=\"NOVEMBER\" \nELSE IF([CLEAR PROCESSING MONTH]=1\
2) THEN RETURN_VALUE=\"DECEMBER\" \nELSE IF([CLEAR PROCESSING MONTH]=13) THEN RETURN_VALUE=\"POST YEAR\" \nELSE RETURN_VALUE=\"UNKNOWN\" \nFI FI FI FI FI FI FI FI FI FI FI FI FI FI");
	addvirtuallineADV(tmp,"CLEAR PROCESSING MONTH STRING",1,12,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(309+1);
	sprintf(t,"IF([PRSNNL][PERDMG][LINEAGE]!=\"\") THEN RETURN_VALUE=([PRSNNL][PERDMG][LAST NAME]+\" \"+[PRSNNL][PERDMG][LINEAGE]+\", \"+[PRSNNL][PERDMG][FIRST NAME]+\" \"+[PRSNNL][PERDMG][MIDDLE NAME]) ELSE RETURN_VALUE=([P\
RSNNL][PERDMG][LAST NAME]+\", \"+[PRSNNL][PERDMG][FIRST NAME]+\" \"+[PRSNNL][PERDMG][MIDDLE NAME])");
	addvirtuallineADV(tmp,"NAME LAST LINEAGE FIRST MIDDLE",1,40,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(31+1);
	sprintf(t,"[PAYROLL][PAYMSTR][FISCAL YEAR]");
	addvirtuallineADV(tmp,"FISCAL YEAR",8,4,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(32+1);
	sprintf(t,"[PAYROll][PAYJPMS][CHECK NUMBER]");
	addvirtuallineADV(tmp,"CHECK NUMBER",8,12,t,0,1);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(41+1);
	sprintf(t,"[PAYROll][PAYJPMS][DIRECT DEPOSIT NUMBER]");
	addvirtuallineADV(tmp,"DIRECT DEPOSIT NUMBER",8,12,t,0,1);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(1+1);
	sprintf(t,"2");
	addvirtuallineADV(tmp,"ACCOUNT TYPE",7,2,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(48+1);
	sprintf(t,"[PAYROLL][PAYJTRN][PAYROLL FUND DEFINITION TYPE]");
	addvirtuallineADV(tmp,"DEFINITION TYPE",7,2,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(122+1);
	sprintf(t,"IF([WRITE COUNT]=1) THEN RETURN_VALUE=[PAYROLL][PAYJTRN][PAYABLE CODE]\nELSE RETURN_VALUE=[PAYROLL][PAYJTRN][CASH CODE] FI");
	addvirtuallineADV(tmp,"ACCOUNT CODE",1,40,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(1+1);
	sprintf(t,"0");
	addvirtuallineADV(tmp,"DIMENSION #0",7,2,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(521+1);
	sprintf(t,"IF([ACCOUNT TYPE]=0) THEN\n	IF([FINMGT][FINEACC][DEFINITION TYPE]=0) THEN\n		RETURN_VALUE=[FINMGT][FINEACC][ACCOUNT CODE][1,1,1]\n	ELSE RETURN_VALUE=\"\" FI\nELSE IF([ACCOUNT TYPE]=1) THEN\n	IF([FINMGT][FINRA\
CC][DEFINITION TYPE]=0) THEN\n		RETURN_VALUE=[FINMGT][FINRACC][ACCOUNT CODE][1,1,1]\n	ELSE RETURN_VALUE=\"\" FI\nELSE IF([ACCOUNT TYPE]=2) THEN\n	IF([FINMGT][FINBACC][DEFINITION TYPE]=0) THEN\n		RETURN_VALUE\
=[FINMGT][FINBACC][ACCOUNT CODE][1,1,1]\n	ELSE RETURN_VALUE=\"\" FI\nELSE RETURN_VALUE=\"\" FI FI FI\n");
	addvirtuallineADV(tmp,"DIMENSION #0 CODE",1,1,t,0,1);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(518+1);
	sprintf(t,"IF([ACCOUNT TYPE]=0) THEN\n	IF([FINMGT][FINEACC][DEFINITION TYPE]=0) THEN\n		RETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][1,1,40]\n	ELSE RETURN_VALUE=\"\" FI\nELSE IF([ACCOUNT TYPE]=1) THEN\n	IF([FINMGT][FINRAC\
C][DEFINITION TYPE]=0) THEN\n		RETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][1,1,40]\n	ELSE RETURN_VALUE=\"\" FI\nELSE IF([ACCOUNT TYPE]=2) THEN\n	IF([FINMGT][FINBACC][DEFINITION TYPE]=0) THEN\n		RETURN_VALUE=[\
FINMGT][FINCOA][DESCRIPTION][1,1,40]\n	ELSE RETURN_VALUE=\"\" FI\nELSE RETURN_VALUE=\"\" FI FI FI\n");
	addvirtuallineADV(tmp,"DIMENSION #0 DESCRIPTION",1,40,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(426+1);
	sprintf(t,"IF([ACCOUNT TYPE]=0) THEN\n	IF([FINMGT][FINEACC][DEFINITION TYPE]=0) THEN\n		RETURN_VALUE=\"GLC\"\n	ELSE RETURN_VALUE=\"\" FI\nELSE IF([ACCOUNT TYPE]=1) THEN\n	IF([FINMGT][FINRACC][DEFINITION TYPE]=0) THEN\n		RE\
TURN_VALUE=\"GLC\"\n	ELSE RETURN_VALUE=\"\" FI\nELSE IF([ACCOUNT TYPE]=2) THEN\n	IF([FINMGT][FINBACC][DEFINITION TYPE]=0) THEN\n		RETURN_VALUE=\"B/S\"\n	ELSE RETURN_VALUE=\"\" FI\nELSE RETURN_VALUE=\"\" FI FI FI\n");
	addvirtuallineADV(tmp,"DIMENSION #1 LABEL",1,40,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(1+1);
	sprintf(t,"1");
	addvirtuallineADV(tmp,"DIMENSION #1",7,2,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(521+1);
	sprintf(t,"IF([ACCOUNT TYPE]=0) THEN\n	IF([FINMGT][FINEACC][DEFINITION TYPE]=0) THEN\n		RETURN_VALUE=[FINMGT][FINEACC][ACCOUNT CODE][1,4,2]\n	ELSE RETURN_VALUE=\"\" FI\nELSE IF([ACCOUNT TYPE]=1) THEN\n	IF([FINMGT][FINRA\
CC][DEFINITION TYPE]=0) THEN\n		RETURN_VALUE=[FINMGT][FINRACC][ACCOUNT CODE][1,4,2]\n	ELSE RETURN_VALUE=\"\" FI\nELSE IF([ACCOUNT TYPE]=2) THEN\n	IF([FINMGT][FINBACC][DEFINITION TYPE]=0) THEN\n		RETURN_VALUE\
=[FINMGT][FINBACC][ACCOUNT CODE][1,4,3]\n	ELSE RETURN_VALUE=\"\" FI\nELSE RETURN_VALUE=\"\" FI FI FI\n");
	addvirtuallineADV(tmp,"DIMENSION #1 CODE",1,3,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(518+1);
	sprintf(t,"IF([ACCOUNT TYPE]=0) THEN\n	IF([FINMGT][FINEACC][DEFINITION TYPE]=0) THEN\n		RETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][2,1,40]\n	ELSE RETURN_VALUE=\"\" FI\nELSE IF([ACCOUNT TYPE]=1) THEN\n	IF([FINMGT][FINRAC\
C][DEFINITION TYPE]=0) THEN\n		RETURN_VALUE=[FINMGT][FINCOA][DESCRIPTION][2,1,40]\n	ELSE RETURN_VALUE=\"\" FI\nELSE IF([ACCOUNT TYPE]=2) THEN\n	IF([FINMGT][FINBACC][DEFINITION TYPE]=0) THEN\n		RETURN_VALUE=[\
FINMGT][FINCOA][DESCRIPTION][2,1,40]\n	ELSE RETURN_VALUE=\"\" FI\nELSE RETURN_VALUE=\"\" FI FI FI\n");
	addvirtuallineADV(tmp,"DIMENSION #1 DESCRIPTION",1,40,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(426+1);
	sprintf(t,"IF([ACCOUNT TYPE]=0) THEN\n	IF([FINMGT][FINEACC][DEFINITION TYPE]=0) THEN\n		RETURN_VALUE=\"GLC\"\n	ELSE RETURN_VALUE=\"\" FI\nELSE IF([ACCOUNT TYPE]=1) THEN\n	IF([FINMGT][FINRACC][DEFINITION TYPE]=0) THEN\n		RE\
TURN_VALUE=\"GLC\"\n	ELSE RETURN_VALUE=\"\" FI\nELSE IF([ACCOUNT TYPE]=2) THEN\n	IF([FINMGT][FINBACC][DEFINITION TYPE]=0) THEN\n		RETURN_VALUE=\"B/S\"\n	ELSE RETURN_VALUE=\"\" FI\nELSE RETURN_VALUE=\"\" FI FI FI\n");
	addvirtuallineADV(tmp,"DIMENSION #1 LABEL",1,40,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(89+1);
	sprintf(t,"IF([WRITE COUNT]=1) THEN RETURN_VALUE=[PAYROLL][PAYJTRN][AMOUNT] ELSE RETURN_VALUE=0 FI\n");
	addvirtuallineADV(tmp,"DEBIT AMOUNT",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(88+1);
	sprintf(t,"IF([WRITE COUNT]=2) THEN RETURN_VALUE=[PAYROLL][PAYJTRN][AMOUNT]\nELSE RETURN_VALUE=0 FI");
	addvirtuallineADV(tmp,"CREDIT AMOUNT",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(32+1);
	sprintf(t,"[PAYROLL][PAYMSTR][FISCAL MONTH]");
	addvirtuallineADV(tmp,"FISCAL PROCESSING MONTH",7,2,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(60+1);
	sprintf(t,"(\"CLEAR CHECK REPORT for: \"+[SELECTED PAY IDENTIFICATION])");
	addvirtuallineADV(tmp,"REPORT TITLE",1,60,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(121+1);
	sprintf(t,"(\"Clear Processing Month: \"+(INTEGER_TO_STRING([CLEAR PROCESSING MONTH]))+\"-(\"+[CLEAR PROCESSING MONTH STRING]+\")\")");
	addvirtuallineADV(tmp,"REPORT SUB-TITLE",1,80,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(42+1);
	sprintf(t,"(\"Executed By: \"+[LOGIN IDENTIFICATION])");
	addvirtuallineADV(tmp,"EXECUTED BY",1,30,t,1,0);

	if(t!=NULL) Rfree(t);




/*======================================================================
Creating Sorts, Control Breaks, and Accumulators
int xaddsortline(RDAreport *report,int type,char *name,int line,char *file)
type(s) - Sort Types	0 - Ascending
		1 - Ascending Lower
		2 - Ascending Upper
		3 - Descending
		4 - Descending Lower
		5 - Descending Upper

	int xaddcontrolline(RDAreport *report,char *name,short type,int line,char *file)
	type(s) - Control Types (Field Type: e.g. VARIABLETEXT, PLAINTEXT, SHORTV, ...)

	int xaddaccumline(RDAreport *report,char *name,int atype,char *accname,
		short field_type,int line,char *file)
	atype(s) - Accumulator Types
		0 - AVERAGE
		1 - FREQUENCY
		2 - FIRST OCCURRENCE
		3 - LAST OCCURRENCE
		4 - MAXIMUM
		5 - MINIMUM
		6 - PREVIOUS OCCURRENCE
		7 - SUMMATION

----------------------------------------------------------------------*/

	addsortline(tmp,0,"[FISCAL YEAR]");
	addsortline(tmp,0,"[DEFINITION TYPE]");
	addsortline(tmp,0,"[DIMENSION #0 CODE]");
	addsortline(tmp,0,"[DIMENSION #1 CODE]");
	addsortline(tmp,0,"[PAYROLL][PAYJPMS][PAY IDENTIFICATION]");
	addsortline(tmp,0,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION]");
	addsortline(tmp,0,"[PAYROLL][PAYJPMS][JOB NUMBER]");
	addaccumline(tmp,"TOTAL DEBIT AMOUNT",7,"[DEBIT AMOUNT]",0);
	addaccumline(tmp,"TOTAL CREDIT AMOUNT",7,"[CREDIT AMOUNT]",0);
	addaccumline(tmp,"LAST DIMENSION #0 CODE",3,"[DIMENSION #0 CODE]",0);
	addaccumline(tmp,"LAST DIMENSION #0 DESCRIPTION",3,"[DIMENSION #0 DESCRIPTION]",0);
	addaccumline(tmp,"LAST DIMENSION #0 LABEL",3,"[DIMENSION #0 LABEL]",0);
	addaccumline(tmp,"LAST DIMENSION #1 CODE",3,"[DIMENSION #1 CODE]",0);
	addaccumline(tmp,"LAST DIMENSION #1 DESCRIPTION",3,"[DIMENSION #1 DESCRIPTION]",0);
	addaccumline(tmp,"LAST DIMENSION #1 LABEL",3,"[DIMENSION #1 LABEL]",0);
	addaccumline(tmp,"LAST PERSONNEL IDENTIFICATION",3,"[PAYROLL][PAYJTRN][PERSONNEL IDENTIFICATION]",0);
	addaccumline(tmp,"LAST ACCOUNT NUMBER",3,"[PAYROLL][PAYJTRN][ACCOUNT NUMBER]",0);
	addaccumline(tmp,"LAST PERSONNEL NAME",3,"[NAME LAST LINEAGE FIRST MIDDLE]",0);
	addaccumline(tmp,"LAST FISCAL YEAR",3,"[FISCAL YEAR]",0);
	addaccumline(tmp,"LAST FISCAL PROCESSING MONTH",3,"[FISCAL PROCESSING MONTH]",0);
	addaccumline(tmp,"LAST PAY IDENTIFICATION",3,"[PAYROLL][PAYJTRN][PAY IDENTIFICATION]",0);
	addaccumline(tmp,"LAST JOB NUMBER",3,"[PAYROLL][PAYJTRN][JOB NUMBER]",0);
	addaccumline(tmp,"LAST ACCOUNT CODE",3,"[ACCOUNT CODE]",0);
	addaccumline(tmp,"LAST DEFINITION TYPE",3,"[DEFINITION TYPE]",0);
	addaccumline(tmp,"LAST CLEAR PROCESSING MONTH",3,"[CLEAR PROCESSING MONTH]",0);
	addaccumline(tmp,"LAST CLEAR PROCESSING MONTH STRING",3,"[CLEAR PROCESSING MONTH STRING]",0);
	addaccumline(tmp,"LAST REPORT TITLE",3,"[REPORT TITLE]",0);
	addaccumline(tmp,"LAST REPORT SUB-TITLE",3,"[REPORT SUB-TITLE]",0);
	addcontrolline(tmp,"[FISCAL YEAR]",0);
	addcontrolline(tmp,"[DEFINITION TYPE]",0);
	addcontrolline(tmp,"[DIMENSION #0 CODE]",0);
	addcontrolline(tmp,"[DIMENSION #1 CODE]",0);
	addcontrolline(tmp,"[PAYROLL][PAYJPMS][PAY IDENTIFICATION]",0);
	addcontrolline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION]",0);
	addcontrolline(tmp,"[PAYROLL][PAYJPMS][JOB NUMBER]",0);




/*----------------------------------------------------------------------
Creating Image Lines
int xaddimageline(RDAreport *report,char *breakname,char *line,
	short test_expression,char *expression,
int numpvals,RDApval *pvals,int linex,char *file)

int xaddimagepval(RDAimage *image,short symbol_type,char *name,int type,
	char *field_name,int line,char *file)
Symbol Type(s)
	0 - FIELD / VIRTUAL FIELD / ACCUMULATOR
		OR Copy Variable to Field        for DATAFILE Type Report
	1 - DEVICE CONTROL VARIABLE
		OR Evaluate Expression           for DATAFILE Type Report
Type(s)
	0 - Value from List of Available Variable Names
		(May Already be Computed)
	1 - Value Entered as an Expression
		(Must Evaluate Expression)

----------------------------------------------------------------------*/

	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("CARRIAGE RETURN");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"PAGE HEADER","@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ORGANIZATION NAME]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAGE NO]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("CARRIAGE RETURN");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"PAGE HEADER","              @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC Page: @RRRRRRR@@",FALSE,NULL,4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[REPORT TITLE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM DATE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("CARRIAGE RETURN");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"PAGE HEADER","              @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC Date: @LLLLLLL@@",TRUE,"(NOT([SELECTED PAY IDENTIFICATION]=\"\"))",4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST REPORT TITLE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM DATE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("CARRIAGE RETURN");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"PAGE HEADER","              @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC Date: @LLLLLLL@@",TRUE,"([SELECTED PAY IDENTIFICATION]=\"\")",4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[REPORT SUB-TITLE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("CARRIAGE RETURN");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"PAGE HEADER","              @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC               @@",TRUE,"(NOT([CLEAR PROCESSING MONTH STRING]=\"\"))",3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST REPORT SUB-TITLE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("CARRIAGE RETURN");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"PAGE HEADER","              @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC               @@",TRUE,"([CLEAR PROCESSING MONTH STRING]=\"\")",3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[EXECUTED BY]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM TIME]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("CARRIAGE RETURN");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"PAGE HEADER","              @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC Time: @LLLLLLL@@",FALSE,NULL,4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("CARRIAGE RETURN");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"PAGE HEADER","@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[FISCAL YEAR]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[FINMGT][FINYEAR][DESCRIPTION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("CARRIAGE RETURN");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"[FISCAL YEAR] HEADER","Fiscal Year: @LLLLL ( @VVVVVVVVVVVVVVVVVVVV ) @@",FALSE,NULL,4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(5*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DIMENSION #0 LABEL]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DIMENSION #0 CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DIMENSION #0 DESCRIPTION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("CARRIAGE RETURN");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"[DIMENSION #0 CODE] HEADER","   @VVV @VVVV ( @VVVVVVVVVVVVVVVVVVVVV ) @@",FALSE,NULL,5,pval);
	free_pvals(pval,5);


	pval=Rmalloc(5*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DIMENSION #1 LABEL]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DIMENSION #1 CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DIMENSION #1 DESCRIPTION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("CARRIAGE RETURN");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"[DIMENSION #1 CODE] HEADER","      @VVVV @VVVVVVVVVVV ( @VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV ) @@",FALSE,NULL,5,pval);
	free_pvals(pval,5);


	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("CARRIAGE RETURN");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"PAGE HEADER","         Personnel Name                                     Debit Amount   Credit Amount@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("CARRIAGE RETURN");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"PAGE HEADER","         ----------------------------------------------  ---------------   -------------@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(5*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST PERSONNEL NAME]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL DEBIT AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL CREDIT AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("CARRIAGE RETURN");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","         @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL @,.RRRRRRRRRRRR   @,.RRRRRRRRRR@@",FALSE,NULL,5,pval);
	free_pvals(pval,5);


	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("CARRIAGE RETURN");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"[DIMENSION #1 CODE] FOOTER","                                                         ---------------   -------------@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(6*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST DIMENSION #1 LABEL]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST DIMENSION #1 CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL DEBIT AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL CREDIT AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("CARRIAGE RETURN");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"[DIMENSION #1 CODE] FOOTER","      Total @LLLLLL @LLLLLLLLLLLLLLLLLLLLLLLLLL          @,.RRRRRRRRRRRR   @,.RRRRRRRRRR@@",FALSE,NULL,6,pval);
	free_pvals(pval,6);


	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("CARRIAGE RETURN");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"[DIMENSION #0 CODE] FOOTER","                                                         ---------------   -------------@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(6*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST DIMENSION #0 LABEL]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST DIMENSION #0 CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL DEBIT AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL CREDIT AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("CARRIAGE RETURN");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"[DIMENSION #0 CODE] FOOTER","   Total @LLLLLLLLL @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL @,.RRRRRRRRRRRR   @,.RRRRRRRRRR@@",FALSE,NULL,6,pval);
	free_pvals(pval,6);


	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("CARRIAGE RETURN");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"[FISCAL YEAR] FOOTER","                                                         ---------------   -------------@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(5*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST FISCAL YEAR]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL DEBIT AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL CREDIT AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("CARRIAGE RETURN");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"[FISCAL YEAR] FOOTER","TOTAL YEAR @LLLL                                         @,.RRRRRRRRRRRR   @,.RRRRRRRRRR@@",FALSE,NULL,5,pval);
	free_pvals(pval,5);


	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("CARRIAGE RETURN");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"REPORT FOOTER","                                                         ===============   =============@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL DEBIT AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL CREDIT AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("CARRIAGE RETURN");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"REPORT FOOTER","TOTAL ALL                                                @,.RRRRRRRRRRRR   @,.RRRRRRRRRR@@",FALSE,NULL,4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("CARRIAGE RETURN");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("FORMFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"PAGE FOOTER","@@@",FALSE,NULL,3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("CARRIAGE RETURN");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("FORMFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"REPORT FOOTER","@@@",FALSE,NULL,3,pval);
	free_pvals(pval,3);






	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			ERRORDIALOG("Cannot Write Report","Error in permissions, user cannot write the report definition.  Check your permissions, and retry.  Call your installer.",NULL,FALSE);
			prterr("Cannont Write Report definition for Report [%s].  Error in permissions, user cannot write the report definition.",rptname);
		} else {
			PAYCLCK_RANGE_SCREEN();
		}
		FreeRDAreport(tmp);
	}
	if(rptname!=NULL) Rfree(rptname);
}
/* PAYROLL's - Make (PAYCLCK RANGE SCREEN) Screen */

static void PAYCLCK_RANGE_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;

/*============================================================================
Adding Screen Widgets
void xaddwdgt(RDAscrn *screen,short type,char *resource_name,char *label,
	char *pixmap,char *ywlabel,short rows,short cols,int rtype,
	char *expression,char *editable_expression,char *sensitive_expression,
	char *transversal_expression,int line,char *file)

#define ADVaddwdgt(a,b,c,d,p,w,e,f,g,h,i,j,l)   xaddwdgt(a,b,c,d,p,w,e,f,g,h,i,j,l,__LINE__,__FILE__)

RDAwdgts types
{
	0	-	Standard Resource
	1	-	New Line
	2	-	End Line
	3	-	New Box
	4	-	End Box
	5	-	Label
	6	-	Button
	7	-	Scrolled List
	8	-	Scrolled Text
	9	-	Toggle Button (Boolean)
	10	-	Frame
	11	-	Seperator
	12	-	New Scrolled Window
	13	-	End Scrolled Window
	14	-	Optional Screen
	15	-	Custom Type (Same as Resource Type, except for rtype.)
	16	-	Expenditure Type (Same as Resource Type, except for rtype.)
	17	-	Revenue Type (Same as Resource Type, except for rtype.)
	18	-	Balance Sheet Type (Same as Resource Type, except for rtype.)
	19	-	Beginning Balance Type (Same as Resource Type, except for rtype.)

New Widgets
	??	-	New Menu Bar (Start Menu Bar)
	??	-	End Menu Bar (End Menu Bar)
	??	-	Menu Control (Menu Control Option)
	??	-	Needs a Modifying Character????????
	??	-	Menu Button (Buttons available to the menu)
	??	-	Needs a Modifying Character????????
}
Resource Types
	5-Label	0	-	Text Label
		1	-	Pixmap Label

	6-Button	0	-	Push Button
		1	-	Arrow Button Right
		2	-	Arrow Button Left
		3	-	Arrow Button Up
		4	-	Arrow Button Down
		5	-	Pixmap

	8-Scrolled Text	0	-	WordWap & Horizontal SB (Side Bar)
		1	-	Horizontal SB & No WordWrap

	9-Toggle Button	0	-	Text Label
		1	-	Pixmap Label

	10-Frame	0	-	Shadow In
		1	-	Shadow Out
		2	-	Shadow Etched In
		3	-	Shadow Etched Out

	11-Seperator	0	-	Horizontal Single Line
		1	-	Horizontal Double Line
		2	-	Horizontal Single Dashed
		3	-	Horizontal Double Dashed
		4	-	Vertical Single Line
		5	-	Vertical Double Line
		6	-	Vertical Single Dashed
		7	-	Vertical Double Dashed

		14-OptionalScrn	0	-	Push Button
		1	-	Arrow Button Right
		2	-	Arrow Button Left
		3	-	Arrow Button Up
		4	-	Arrow Button Down
		5	-	Pixmap

============================================================================*/


	scrn=RDAscrnNEW("PAYROLL","PAYCLCK RANGE SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Pay Identification:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"SELECTED PAY IDENTIFICATION","Selected Pay Identification","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Clear Processing Month:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"CLEAR PROCESSING MONTH","Clear Processing Month","","",0,2,0,"","","","");
		ADVaddwdgt(scrn,5,"","(0=Pre-Year,1=January,2=February,...,12=December,13=Post Year)","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [DIMENSION #0 CODE]","Range on Account Fund","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [DIMENSION #0 CODE]","From Dimension #0 Code","","",0,1,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [DIMENSION #0 CODE]","To Dimension #0 Code","","",0,1,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [PERDMG][PERSONNEL IDENTIFICATION]","Range on Personnel Identification","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [PERDMG][PERSONNEL IDENTIFICATION]","From Personnel Identification","","",0,25,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [PERDMG][PERSONNEL IDENTIFICATION]","To Personnel Identification","","",0,25,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"RUN REPORT","Run Report","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
		if(writescrnbin(defdir,scrn))
		{
			ERRORDIALOG("Cannot Save Screen","This screen may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
}
