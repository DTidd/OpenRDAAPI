/* PAYROLL's - Make (FICAEARNINGS-SH) Report */
#include <cstdio>
#include <rptgen.hpp>
/*COMPMEM 40960 */

extern void free_pvals(RDApval *,int);
extern char *module;
void FICAEARNINGS_SH(void);
static void FICAEARNINGS_SH_RANGE_SCREEN(void);

void FICAEARNINGS_SH()
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
	rptname=Rmalloc(15+1);
	sprintf(rptname,"FICAEARNINGS-SH");
	t=Rmalloc(20+1);
	sprintf(t,"Fica Earnings Report");
	desc=Rmalloc(74+1);
	sprintf(desc,"This output definition is used to print the standard fica earnings report.");
	tmp=RDAreportNEWADV(rptname,t,desc,1,"",FALSE,TRUE,FALSE,FALSE,FALSE,"","",TRUE,1,TRUE,"");
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
	adddisplayADV(tmp,(double)8.500000,(double)10.000000,0,FALSE,0,"",0,1,0,pvars);

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

	t=Rmalloc(105+1);
	sprintf(t,"([PAYROLL][PAYDCSM][CALENDAR YEAR]=[SELECTED CALENDAR YEAR]) AND \n([PAYROLL][PAYDCSM][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDCSM",0,NULL,NULL,"PAYDCSM YEAR KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"CALENDAR YEAR","[SELECTED CALENDAR YEAR]");
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","");
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","");
	}
	if(t!=NULL) Rfree(t);

	x=addsearchfileADV(tmp,"PAYROLL","PAYPCSM",1,"PAYROLL","PAYDCSM","PAYPCSM KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYDCSM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"CALENDAR YEAR","[PAYDCSM][CALENDAR YEAR]");
	}

	t=Rmalloc(127+1);
	sprintf(t,"([PAYROLL][PAYDDMS][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYDDMS][ACTIVE]=TRUE) AND\n([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"FICA\")");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDDMS",3,"PAYROLL","PAYDCSM","PAYDDMS KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","[PAYDCSM][DEDUCTION DESCRIPTION IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	x=addsearchfileADV(tmp,"PRSNNL","PERDMG",3,"PAYROLL","PAYDCSM","PERDMG KEY",TRUE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYDCSM][PERSONNEL IDENTIFICATION]");
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
	addvirtuallineADV(tmp,"SELECTED CALENDAR YEAR",8,4,t,0,2);

	if(t!=NULL) Rfree(t);
	addvirtuallineADV(tmp,"SELECTED MONTH",7,2,t,0,2);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(703+1);
	sprintf(t,"IF([SELECTED MONTH]=0) THEN RETURN_VALUE=\"JANUARY\"\nELSE IF([SELECTED MONTH]=1) THEN RETURN_VALUE=\"FEBRUARY\"\nELSE IF([SELECTED MONTH]=2) THEN RETURN_VALUE=\"MARCH\"\nELSE IF([SELECTED MONTH]=3) THEN RETURN\
_VALUE=\"APRIL\"\nELSE IF([SELECTED MONTH]=4) THEN RETURN_VALUE=\"MAY\"\nELSE IF([SELECTED MONTH]=5) THEN RETURN_VALUE=\"JUNE\"\nELSE IF([SELECTED MONTH]=6) THEN RETURN_VALUE=\"JULY\"\nELSE IF([SELECTED MONTH]=7)\
 THEN RETURN_VALUE=\"AUGUST\"\nELSE IF([SELECTED MONTH]=8) THEN RETURN_VALUE=\"SEPTEMBER\"\nELSE IF([SELECTED MONTH]=9) THEN RETURN_VALUE=\"OCTOBER\"\nELSE IF([SELECTED MONTH]=10) THEN RETURN_VALUE=\"NOVEMBER\"\n\
ELSE RETURN_VALUE=\"DECEMBER\" FI FI FI FI FI FI FI FI FI FI FI");
	addvirtuallineADV(tmp,"MONTH NAME",0,0,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(238+1);
	sprintf(t,"IF([SELECTED MONTH]<3) THEN RETURN_VALUE=\"FIRST QUARTER\"\nELSE IF([SELECTED MONTH]<6) THEN RETURN_VALUE=\"SECOND QUARTER\"\nELSE IF([SELECTED MONTH]<9) THEN RETURN_VALUE=\"THIRD QUARTER\"\nELSE RETURN_VALUE=\"\
FOURTH QUARTER\" FI FI FI");
	addvirtuallineADV(tmp,"QUARTER NAME",0,0,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(399+1);
	sprintf(t,"[PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS]+[PAYROLL][PAYPCSM][JUNE GROSS]+[PAYROL\
L][PAYPCSM][JULY GROSS]+[PAYROLL][PAYPCSM][AUGUST GROSS]+[PAYROLL][PAYPCSM][SEPTEMBER GROSS]+[PAYROLL][PAYPCSM][OCTOBER GROSS]+[PAYROLL][PAYPCSM][NOVEMBER GROSS]+[PAYROLL][PAYPCSM][DECEMBER GROSS]");
	addvirtuallineADV(tmp,"ANNUAL CALENDAR PERSONNEL GROSS",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(413+1);
	sprintf(t,"[PAYROLL][PAYDCSM][JANUARY AMOUNT]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]+[PAYROLL][PAYDCSM][MARCH AMOUNT]+[PAYROLL][PAYDCSM][APRIL AMOUNT]+[PAYROLL][PAYDCSM][MAY AMOUNT]+[PAYROLL][PAYDCSM][JUNE AMOUNT]+[\
PAYROLL][PAYDCSM][JULY AMOUNT]+[PAYROLL][PAYDCSM][AUGUST AMOUNT]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT]+[PAYROLL][PAYDCSM][NOVEMBER AMOUNT]+[PAYROLL][PAYDCSM][DECEMBER\
 AMOUNT]");
	addvirtuallineADV(tmp,"ANNUAL CALENDAR PERSONNEL DEDUCTION AMOUNT",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(399+1);
	sprintf(t,"[PAYROLL][PAYDCSM][JANUARY GROSS]+[PAYROLL][PAYDCSM][FEBRUARY GROSS]+[PAYROLL][PAYDCSM][MARCH GROSS]+[PAYROLL][PAYDCSM][APRIL GROSS]+[PAYROLL][PAYDCSM][MAY GROSS]+[PAYROLL][PAYDCSM][JUNE GROSS]+[PAYROL\
L][PAYDCSM][JULY GROSS]+[PAYROLL][PAYDCSM][AUGUST GROSS]+[PAYROLL][PAYDCSM][SEPTEMBER GROSS]+[PAYROLL][PAYDCSM][OCTOBER GROSS]+[PAYROLL][PAYDCSM][NOVEMBER GROSS]+[PAYROLL][PAYDCSM][DECEMBER GROSS]");
	addvirtuallineADV(tmp,"ANNUAL CALENDAR PERSONNEL DEDUCTION GROSS",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(79+1);
	sprintf(t,"([ANNUAL CALENDAR PERSONNEL GROSS]-[ANNUAL CALENDAR PERSONNEL DEDUCTION GROSS])");
	addvirtuallineADV(tmp,"ANNUAL SHELTERED AMOUNT",2,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(103+1);
	sprintf(t,"[PAYROLL][PAYDCSM][JANUARY AMOUNT]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]+[PAYROLL][PAYDCSM][MARCH AMOUNT]");
	addvirtuallineADV(tmp,"FIRST QUARTER CALENDAR PERSONNEL DEDUCTION AMOUNT",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(100+1);
	sprintf(t,"[PAYROLL][PAYDCSM][JANUARY GROSS]+[PAYROLL][PAYDCSM][FEBRUARY GROSS]+[PAYROLL][PAYDCSM][MARCH GROSS]");
	addvirtuallineADV(tmp,"FIRST QUARTER CALENDAR PERSONNEL DEDUCTION GROSS",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(100+1);
	sprintf(t,"[PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]");
	addvirtuallineADV(tmp,"FIRST QUARTER CALENDAR PERSONNEL GROSS",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(95+1);
	sprintf(t,"[PAYROLL][PAYDCSM][APRIL AMOUNT]+[PAYROLL][PAYDCSM][MAY AMOUNT]+[PAYROLL][PAYDCSM][JUNE AMOUNT]");
	addvirtuallineADV(tmp,"SECOND QUARTER CALENDAR PERSONNEL DEDUCTION AMOUNT",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(92+1);
	sprintf(t,"[PAYROLL][PAYDCSM][APRIL GROSS]+[PAYROLL][PAYDCSM][MAY GROSS]+[PAYROLL][PAYDCSM][JUNE GROSS]");
	addvirtuallineADV(tmp,"SECOND QUARTER CALENDAR PERSONNEL DEDUCTION GROSS",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(92+1);
	sprintf(t,"[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS]+[PAYROLL][PAYPCSM][JUNE GROSS]");
	addvirtuallineADV(tmp,"SECOND QUARTER CALENDAR PERSONNEL GROSS",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(102+1);
	sprintf(t,"[PAYROLL][PAYDCSM][JULY AMOUNT]+[PAYROLL][PAYDCSM][AUGUST AMOUNT]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT]");
	addvirtuallineADV(tmp,"THIRD QUARTER CALENDAR PERSONNEL DEDUCTION AMOUNT",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(99+1);
	sprintf(t,"[PAYROLL][PAYDCSM][JULY GROSS]+[PAYROLL][PAYDCSM][AUGUST GROSS]+[PAYROLL][PAYDCSM][SEPTEMBER GROSS]");
	addvirtuallineADV(tmp,"THIRD QUARTER CALENDAR PERSONNEL DEDUCTION GROSS",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(99+1);
	sprintf(t,"[PAYROLL][PAYPCSM][JULY GROSS]+[PAYROLL][PAYPCSM][AUGUST GROSS]+[PAYROLL][PAYPCSM][SEPTEMBER GROSS]");
	addvirtuallineADV(tmp,"THIRD QUARTER CALENDAR PERSONNEL GROSS",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(106+1);
	sprintf(t,"[PAYROLL][PAYDCSM][OCTOBER AMOUNT]+[PAYROLL][PAYDCSM][NOVEMBER AMOUNT]+[PAYROLL][PAYDCSM][DECEMBER AMOUNT]");
	addvirtuallineADV(tmp,"FOURTH QUARTER CALENDAR PERSONNEL DEDUCTION AMOUNT",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(103+1);
	sprintf(t,"[PAYROLL][PAYDCSM][OCTOBER GROSS]+[PAYROLL][PAYDCSM][NOVEMBER GROSS]+[PAYROLL][PAYDCSM][DECEMBER GROSS]");
	addvirtuallineADV(tmp,"FOURTH QUARTER CALENDAR PERSONNEL DEDUCTION GROSS",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(103+1);
	sprintf(t,"[PAYROLL][PAYPCSM][OCTOBER GROSS]+[PAYROLL][PAYPCSM][NOVEMBER GROSS]+[PAYROLL][PAYPCSM][DECEMBER GROSS]");
	addvirtuallineADV(tmp,"FOURTH QUARTER CALENDAR PERSONNEL GROSS",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(968+1);
	sprintf(t,"IF([SELECTED MONTH]=0) THEN RETURN_VALUE=[PAYROLL][PAYPCSM][JANUARY GROSS]\nELSE IF([SELECTED MONTH]=1) THEN RETURN_VALUE=[PAYROLL][PAYPCSM][FEBRUARY GROSS]\nELSE IF([SELECTED MONTH]=2) THEN RETURN_VALUE\
=[PAYROLL][PAYPCSM][MARCH GROSS]\nELSE IF([SELECTED MONTH]=3) THEN RETURN_VALUE=[PAYROLL][PAYPCSM][APRIL GROSS]\nELSE IF([SELECTED MONTH]=4) THEN RETURN_VALUE=[PAYROLL][PAYPCSM][MAY GROSS]\nELSE IF([SELE\
CTED MONTH]=5) THEN RETURN_VALUE=[PAYROLL][PAYPCSM][JUNE GROSS]\nELSE IF([SELECTED MONTH]=6) THEN RETURN_VALUE=[PAYROLL][PAYPCSM][JULY GROSS]\nELSE IF([SELECTED MONTH]=7) THEN RETURN_VALUE=[PAYROLL][PAY\
PCSM][AUGUST GROSS]\nELSE IF([SELECTED MONTH]=8) THEN RETURN_VALUE=[PAYROLL][PAYPCSM][SEPTEMBER GROSS]\nELSE IF([SELECTED MONTH]=9) THEN RETURN_VALUE=[PAYROLL][PAYPCSM][OCTOBER GROSS]\nELSE IF([SELECTED \
MONTH]=10) THEN RETURN_VALUE=[PAYROLL][PAYPCSM][NOVEMBER GROSS] ELSE RETURN_VALUE=[PAYROLL][PAYPCSM][DECEMBER GROSS] FI FI FI FI FI FI FI FI FI FI FI");
	addvirtuallineADV(tmp,"MONTH TO DATE EARNINGS",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(968+1);
	sprintf(t,"IF([SELECTED MONTH]=0) THEN RETURN_VALUE=[PAYROLL][PAYDCSM][JANUARY GROSS]\nELSE IF([SELECTED MONTH]=1) THEN RETURN_VALUE=[PAYROLL][PAYDCSM][FEBRUARY GROSS]\nELSE IF([SELECTED MONTH]=2) THEN RETURN_VALUE\
=[PAYROLL][PAYDCSM][MARCH GROSS]\nELSE IF([SELECTED MONTH]=3) THEN RETURN_VALUE=[PAYROLL][PAYDCSM][APRIL GROSS]\nELSE IF([SELECTED MONTH]=4) THEN RETURN_VALUE=[PAYROLL][PAYDCSM][MAY GROSS]\nELSE IF([SELE\
CTED MONTH]=5) THEN RETURN_VALUE=[PAYROLL][PAYDCSM][JUNE GROSS]\nELSE IF([SELECTED MONTH]=6) THEN RETURN_VALUE=[PAYROLL][PAYDCSM][JULY GROSS]\nELSE IF([SELECTED MONTH]=7) THEN RETURN_VALUE=[PAYROLL][PAY\
DCSM][AUGUST GROSS]\nELSE IF([SELECTED MONTH]=8) THEN RETURN_VALUE=[PAYROLL][PAYDCSM][SEPTEMBER GROSS]\nELSE IF([SELECTED MONTH]=9) THEN RETURN_VALUE=[PAYROLL][PAYDCSM][OCTOBER GROSS]\nELSE IF([SELECTED \
MONTH]=10) THEN RETURN_VALUE=[PAYROLL][PAYDCSM][NOVEMBER GROSS] ELSE RETURN_VALUE=[PAYROLL][PAYDCSM][DECEMBER GROSS] FI FI FI FI FI FI FI FI FI FI FI");
	addvirtuallineADV(tmp,"MONTH TO DATE GROSS",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(980+1);
	sprintf(t,"IF([SELECTED MONTH]=0) THEN RETURN_VALUE=[PAYROLL][PAYDCSM][JANUARY AMOUNT]\nELSE IF([SELECTED MONTH]=1) THEN RETURN_VALUE=[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]\nELSE IF([SELECTED MONTH]=2) THEN RETURN_VAL\
UE=[PAYROLL][PAYDCSM][MARCH AMOUNT]\nELSE IF([SELECTED MONTH]=3) THEN RETURN_VALUE=[PAYROLL][PAYDCSM][APRIL AMOUNT]\nELSE IF([SELECTED MONTH]=4) THEN RETURN_VALUE=[PAYROLL][PAYDCSM][MAY AMOUNT]\nELSE IF(\
[SELECTED MONTH]=5) THEN RETURN_VALUE=[PAYROLL][PAYDCSM][JUNE AMOUNT]\nELSE IF([SELECTED MONTH]=6) THEN RETURN_VALUE=[PAYROLL][PAYDCSM][JULY AMOUNT]\nELSE IF([SELECTED MONTH]=7) THEN RETURN_VALUE=[PAYRO\
LL][PAYDCSM][AUGUST AMOUNT]\nELSE IF([SELECTED MONTH]=8) THEN RETURN_VALUE=[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT]\nELSE IF([SELECTED MONTH]=9) THEN RETURN_VALUE=[PAYROLL][PAYDCSM][OCTOBER AMOUNT]\nELSE IF(\
[SELECTED MONTH]=10) THEN RETURN_VALUE=[PAYROLL][PAYDCSM][NOVEMBER AMOUNT] ELSE RETURN_VALUE=[PAYROLL][PAYDCSM][DECEMBER AMOUNT] FI FI FI FI FI FI FI FI FI FI FI");
	addvirtuallineADV(tmp,"MONTH TO DATE AMOUNT",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(48+1);
	sprintf(t,"([MONTH TO DATE EARNINGS]-[MONTH TO DATE GROSS])");
	addvirtuallineADV(tmp,"MONTH TO DATE SHELTERED",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(339+1);
	sprintf(t,"IF([SELECTED MONTH]<3) THEN \n	RETURN_VALUE=[FIRST QUARTER CALENDAR PERSONNEL GROSS]\nELSE IF([SELECTED MONTH]<6) THEN\n	RETURN_VALUE=[SECOND QUARTER CALENDAR PERSONNEL GROSS]\nELSE IF([SELECTED MONTH]<9) \
THEN \n	RETURN_VALUE=[THIRD QUARTER CALENDAR PERSONNEL GROSS]\nELSE RETURN_VALUE=[FOURTH QUARTER CALENDAR PERSONNEL GROSS]\nFI FI FI");
	addvirtuallineADV(tmp,"QUARTER TO DATE EARNINGS",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(379+1);
	sprintf(t,"IF([SELECTED MONTH]<3) THEN \n	RETURN_VALUE=[FIRST QUARTER CALENDAR PERSONNEL DEDUCTION GROSS]\nELSE IF([SELECTED MONTH]<6) THEN\n	RETURN_VALUE=[SECOND QUARTER CALENDAR PERSONNEL DEDUCTION GROSS]\nELSE IF(\
[SELECTED MONTH]<9) THEN \n	RETURN_VALUE=[THIRD QUARTER CALENDAR PERSONNEL DEDUCTION GROSS]\nELSE RETURN_VALUE=[FOURTH QUARTER CALENDAR PERSONNEL DEDUCTION GROSS]\nFI FI FI");
	addvirtuallineADV(tmp,"QUARTER TO DATE GROSS",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(383+1);
	sprintf(t,"IF([SELECTED MONTH]<3) THEN \n	RETURN_VALUE=[FIRST QUARTER CALENDAR PERSONNEL DEDUCTION AMOUNT]\nELSE IF([SELECTED MONTH]<6) THEN\n	RETURN_VALUE=[SECOND QUARTER CALENDAR PERSONNEL DEDUCTION AMOUNT]\nELSE I\
F([SELECTED MONTH]<9) THEN \n	RETURN_VALUE=[THIRD QUARTER CALENDAR PERSONNEL DEDUCTION AMOUNT]\nELSE RETURN_VALUE=[FOURTH QUARTER CALENDAR PERSONNEL DEDUCTION AMOUNT]\nFI FI FI");
	addvirtuallineADV(tmp,"QUARTER TO DATE AMOUNT",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(52+1);
	sprintf(t,"([QUARTER TO DATE EARNINGS]-[QUARTER TO DATE GROSS])");
	addvirtuallineADV(tmp,"QUARTER TO DATE SHELTERED",2,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(69+1);
	sprintf(t,"(\"FICA EARNINGS REPORT (\"+[MONTH NAME]+\"--\"+[QUARTER NAME]+\")\")");
	addvirtuallineADV(tmp,"REPORT TITLE",0,0,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(42+1);
	sprintf(t,"(\"Executed By: \"+[LOGIN IDENTIFICATION])");
	addvirtuallineADV(tmp,"EXECUTED BY",0,0,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(132+1);
	sprintf(t,"([PRSNNL][PERDMG][FIRST NAME]+\" \"+[PRSNNL][PERDMG][MIDDLE NAME]+\" \"+[PRSNNL][PERDMG][LAST NAME]+\" \"+[PRSNNL][PERDMG][LINEAGE])");
	addvirtuallineADV(tmp,"NAME FIRST MIDDLE LAST LINEAGE",1,40,t,0,0);

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

	addsortline(tmp,0,"[PAYROLL][PAYDCSM][CALENDAR YEAR]");
	addsortline(tmp,0,"[PAYROLL][PAYDCSM][DEDUCTION DESCRIPTION IDENTIFICATION]");
	addsortline(tmp,0,"[PAYROLL][PAYDCSM][PERSONNEL IDENTIFICATION]");
	addaccumline(tmp,"TOTAL MONTH TO DATE GROSS",7,"[MONTH TO DATE GROSS]",0);
	addaccumline(tmp,"TOTAL MONTH TO DATE AMOUNT",7,"[MONTH TO DATE AMOUNT]",0);
	addaccumline(tmp,"TOTAL MONTH TO DATE SHELTERED",7,"[MONTH TO DATE SHELTERED]",0);
	addaccumline(tmp,"TOTAL QUARTER TO DATE GROSS",7,"[QUARTER TO DATE GROSS]",0);
	addaccumline(tmp,"TOTAL QUARTER TO DATE AMOUNT",7,"[QUARTER TO DATE AMOUNT]",0);
	addaccumline(tmp,"TOTAL QUARTER TO DATE SHELTERED",0,"[QUARTER TO DATE SHELTERED]",0);
	addaccumline(tmp,"TOTAL ANNUAL GROSS",7,"[ANNUAL CALENDAR PERSONNEL DEDUCTION GROSS]",0);
	addaccumline(tmp,"TOTAL ANNUAL AMOUNT",7,"[ANNUAL CALENDAR PERSONNEL DEDUCTION AMOUNT]",0);
	addaccumline(tmp,"TOTAL ANNUAL SHELTERED",7,"[ANNUAL SHELTERED AMOUNT]",0);
	addaccumline(tmp,"LAST DEDUCTION DESCRIPTION IDENTIFICATION",3,"[PAYROLL][PAYDCSM][DEDUCTION DESCRIPTION IDENTIFICATION]",0);
	addaccumline(tmp,"LAST DEDUCTION DESCRIPTION NAME",3,"[PAYROLL][PAYDDMS][SHORT DESCRIPTION]",0);
	addaccumline(tmp,"LAST CALENDAR YEAR",3,"[PAYROLL][PAYDCSM][CALENDAR YEAR]",0);
	addcontrolline(tmp,"[PAYROLL][PAYDCSM][CALENDAR YEAR]",0);
	addcontrolline(tmp,"[PAYROLL][PAYDCSM][DEDUCTION DESCRIPTION IDENTIFICATION]",0);




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
	addimageline(tmp,"PAGE HEADER","                                             @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                                  PAGE: @RRRRRRR@@",FALSE,NULL,4,pval);
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
	addimageline(tmp,"PAGE HEADER","                                            @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                                 DATE: @LLLLLLL@@",FALSE,NULL,4,pval);
	free_pvals(pval,4);


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
	addimageline(tmp,"PAGE HEADER","                                             @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                                  TIME: @LLLLLLL@@",FALSE,NULL,4,pval);
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
	pvals->name=stralloc("[MONTH NAME]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[QUARTER NAME]");
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
	addimageline(tmp,"PAGE HEADER","                                                              @CCCCCCCCCCCCCCCCCC                                @CCCCCCCCCCCCC                                         ANNUAL@@",FALSE,NULL,4,pval);
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
	addimageline(tmp,"PAGE HEADER","PERSONNEL ID    NAME                              GROSS           AMOUNT          SHELTERED          GROSS            AMOUNT        SHELTERED           GROSS           AMOUNT         SHELTERED@@",FALSE,NULL,2,pval);
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
	addimageline(tmp,"PAGE HEADER","@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYDCSM][CALENDAR YEAR]");
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
	addimageline(tmp,"[PAYROLL][PAYDCSM][CALENDAR YEAR] HEADER","Calendar Year @LLLL@@",FALSE,NULL,3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(5*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYDCSM][DEDUCTION DESCRIPTION IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYDDMS][SHORT DESCRIPTION]");
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
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"[PAYROLL][PAYDCSM][DEDUCTION DESCRIPTION IDENTIFICATION] HEADER","     @LLLLLLLLLLLLLLLL @VVVVVVVVVVVVVVVVVVVVVV@@@",FALSE,NULL,5,pval);
	free_pvals(pval,5);


	pval=Rmalloc(13*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYDCSM][PERSONNEL IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[NAME FIRST MIDDLE LAST LINEAGE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[MONTH TO DATE GROSS]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[MONTH TO DATE AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[MONTH TO DATE SHELTERED]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[QUARTER TO DATE GROSS]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[QUARTER TO DATE AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[QUARTER TO DATE SHELTERED]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ANNUAL CALENDAR PERSONNEL DEDUCTION GROSS]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ANNUAL CALENDAR PERSONNEL DEDUCTION AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ANNUAL SHELTERED AMOUNT]");
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
	addimageline(tmp,"DETAIL LINES","@LLLLLLLLLLLLLLL @LLLLLLLLLLLLLLLLLLLLLLL  @,.RRRRRRRRRRRR  @,.RRRRRRRRRRRR  @,.RRRRRRRRRRRR  @,.RRRRRRRRRRRR  @,.RRRRRRRRRRRR  @,.RRRRRRRRRRRR  @,.RRRRRRRRRRRR  @,.RRRRRRRRRRRR  @,.RRRRRRRRRRRR@@",FALSE,NULL,13,pval);
	free_pvals(pval,13);


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
	addimageline(tmp,"[PAYROLL][PAYDCSM][DEDUCTION DESCRIPTION IDENTIFICATION] FOOTER","@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(12*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST DEDUCTION DESCRIPTION IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL MONTH TO DATE GROSS]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL MONTH TO DATE AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL MONTH TO DATE SHELTERED]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL QUARTER TO DATE GROSS]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL QUARTER TO DATE AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL QUARTER TO DATE SHELTERED]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL ANNUAL GROSS]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL ANNUAL AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL ANNUAL SHELTERED]");
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
	addimageline(tmp,"[PAYROLL][PAYDCSM][DEDUCTION DESCRIPTION IDENTIFICATION] FOOTER","                TOTAL @LLLLLLLLLLLLLLLLLL  @,.RRRRRRRRRRRR  @,.RRRRRRRRRRRR  @,.RRRRRRRRRRRR  @,.RRRRRRRRRRRR  @,.RRRRRRRRRRRR  @,.RRRRRRRRRRRR  @,.RRRRRRRRRRRR  @,.RRRRRRRRRRRR  @,.RRRRRRRRRRRR@@",FALSE,NULL,12,pval);
	free_pvals(pval,12);


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
	addimageline(tmp,"[PAYROLL][PAYDCSM][DEDUCTION DESCRIPTION IDENTIFICATION] FOOTER","@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


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






	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			ERRORDIALOG("Cannot Write Report","Error in permissions, user cannot write the report definition.  Check your permissions, and retry.  Call your installer.",NULL,FALSE);
			prterr("Cannont Write Report definition for Report [%s].  Error in permissions, user cannot write the report definition.",rptname);
		} else {
			FICAEARNINGS_SH_RANGE_SCREEN();
		}
		FreeRDAreport(tmp);
	}
	if(rptname!=NULL) Rfree(rptname);
}
static void FICAEARNINGS_SH_RANGE_SCREEN()
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


	scrn=RDAscrnNEW("PAYROLL","FICAEARNINGS-SH RANGE SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Selected Calendar Year:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"SELECTED CALENDAR YEAR","Selected Calendar Year","","",0,4,0,"","","","");
		ADVaddwdgt(scrn,5,"","Selected Month (0=January,11=December):","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"SELECTED MONTH","Selected Month","","",0,2,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [PAYDCSM][DEDUCTION DESCRIPTION IDENTIFICATION]","Range on Deductions ID's","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [PAYDCSM][DEDUCTION DESCRIPTION IDENTIFICATION]","From Deduction ID","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [PAYDCSM][DEDUCTION DESCRIPTION IDENTIFICATION]","To Deduction ID","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [PAYDCSM][PERSONNEL IDENTIFICATION]","Range on Personnel ID's","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [PAYDCSM][PERSONNEL IDENTIFICATION]","From Personnel ID","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [PAYDCSM][PERSONNEL IDENTIFICATION]","To Personnel ID","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"RUN REPORT","Select","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,"","","","");
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
