/* PAYROLL's - Make (PAYMTSA-CCG) Report */
/* WARNINGSJS - report needs to have PAYJMST, PAYPPMS and PAYMSTR added to 
                second detail for new report generator */

#include <cstdio>
#include <rptgen.hpp>
/*COMPMEM 40960 */

extern void free_pvals(RDApval *,int);
extern char *module;
void PAYMTSA_CCG(void);

void PAYMTSA_CCG()
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
	rptname=Rmalloc(11+1);
	sprintf(rptname,"PAYMTSA_CCG");
	t=Rmalloc(44+1);
	sprintf(t,"Make Transactions Audit Trail by Location ID");
	desc=Rmalloc(100+1);
	sprintf(desc,"This output definition is used to provide an audit trail for Make Transactions sorted by Location ID");
	tmp=RDAreportNEWADV(rptname,t,desc,1,"",FALSE,TRUE,FALSE,TRUE,TRUE,"PAYROLL","PAYMTSA1-CCGT",FALSE,1,FALSE,"");
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

	adddisplayADV(tmp,(double)8.500000,(double)11.000000,0,FALSE,0,"dfx1",0,1,0,NULL);

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

	t=Rmalloc(152+1);
	sprintf(t,"([PAYROLL][PAYJTRN][SOURCE USER VERIFIED]=FALSE) AND ([PAYROLL][PAYJTRN][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYJTRN][SOURCE USER]=[LOGIN IDENTIFICATION])");
	x=addsearchfileADV(tmp,"PAYROLL","PAYJTRN",0,NULL,NULL,"PAYJTRN KEY",FALSE,t,NULL,1);
	if(t!=NULL) Rfree(t);

	x=addsearchfileADV(tmp,"PAYROLL","PAYJMST",1,"PAYROLL","PAYJTRN","JOBMSTR KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJTRN][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYJTRN][JOB NUMBER]");
	}

	x=addsearchfileADV(tmp,"PRSNNL","PERDMG",1,"PAYROLL","PAYJTRN","PERDMG KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJTRN][PERSONNEL IDENTIFICATION]");
	}

	x=addsearchfileADV(tmp,"PAYROLL","PAYPPMS",1,"PAYROLL","PAYJTRN","PAYPPMS KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJTRN][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJTRN][PAY IDENTIFICATION]");
	}

	x=addsearchfileADV(tmp,"PAYROLL","PAYMSTR",1,"PAYROLL","PAYJTRN","PAYMSTR KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJTRN][PAY IDENTIFICATION]");
	}

	t=Rmalloc(152+1);
	sprintf(t,"([PAYROLL][PAYDTRN][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYDTRN][SOURCE USER VERIFIED]=FALSE) AND ([PAYROLL][PAYDTRN][SOURCE USER]=[LOGIN IDENTIFICATION])");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDTRN",0,NULL,NULL,"PAYDTRN KEY",FALSE,t,NULL,1);
	if(t!=NULL) Rfree(t);

	x=addsearchfileADV(tmp,"PAYROLL","PAYDDMS",1,"PAYROLL","PAYDTRN","PAYDDMS KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","[PAYDTRN][DEDUCTION DESCRIPTION IDENTIFICATION]");
	}

	x=addsearchfileADV(tmp,"PAYROLL","PAYDEDM",1,"PAYROLL","PAYDTRN","PAYDEDM KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYDTRN][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","[PAYDTRN][DEDUCTION DESCRIPTION IDENTIFICATION]");
	}

	x=addsearchfileADV(tmp,"PRSNNL","PERDMG",1,"PAYROLL","PAYDTRN","PERDMG KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYDTRN][PERSONNEL IDENTIFICATION]");
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
	t=Rmalloc(42+1);
	sprintf(t,"(\"Executed By: \"+[LOGIN IDENTIFICATION])");
	addvirtuallineADV(tmp,"EXECUTED BY",1,30,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(21+1);
	sprintf(t,"[PRIMARY FILE NUMBER]");
	addvirtuallineADV(tmp,"TRANSACTION TYPE",7,2,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(156+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) THEN RETURN_VALUE=[PAYROLL][PAYJTRN][PERSONNEL IDENTIFICATION] ELSE RETURN_VALUE=[PAYROLL][PAYDTRN][PERSONNEL IDENTIFICATION] FI");
	addvirtuallineADV(tmp,"PERSONNEL IDENTIFICATION",1,25,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(309+1);
	sprintf(t,"IF([PRSNNL][PERDMG][LINEAGE]!=\"\") THEN RETURN_VALUE=([PRSNNL][PERDMG][LAST NAME]+\" \"+[PRSNNL][PERDMG][LINEAGE]+\", \"+[PRSNNL][PERDMG][FIRST NAME]+\" \"+[PRSNNL][PERDMG][MIDDLE NAME]) ELSE RETURN_VALUE=([P\
RSNNL][PERDMG][LAST NAME]+\", \"+[PRSNNL][PERDMG][FIRST NAME]+\" \"+[PRSNNL][PERDMG][MIDDLE NAME])");
	addvirtuallineADV(tmp,"NAME LAST LINEAGE FIRST MIDDLE",1,40,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(144+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) THEN RETURN_VALUE=[PAYROLL][PAYJTRN][PAY IDENTIFICATION] ELSE RETURN_VALUE=[PAYROLL][PAYDTRN][PAY IDENTIFICATION] FI");
	addvirtuallineADV(tmp,"PAY IDENTIFICATION",1,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(41+1);
	sprintf(t,"[PRSNNL][PERDMG][LOCATION IDENTIFICATION]");
	addvirtuallineADV(tmp,"LOCATION IDENTIFICATION",1,25,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(136+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) THEN RETURN_VALUE=[PAYROLL][PAYJTRN][ACCOUNT NUMBER] ELSE RETURN_VALUE=[PAYROLL][PAYDTRN][ACCOUNT NUMBER] FI");
	addvirtuallineADV(tmp,"ACCOUNT NUMBER",8,8,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(128+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) THEN RETURN_VALUE=[PAYROLL][PAYJTRN][JOB NUMBER] ELSE RETURN_VALUE=[PAYROLL][PAYDTRN][JOB NUMBER] FI");
	addvirtuallineADV(tmp,"JOB NUMBER",8,8,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(120+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) THEN RETURN_VALUE=[PAYROLL][PAYJTRN][AMOUNT] ELSE RETURN_VALUE=[PAYROLL][PAYDTRN][AMOUNT] FI");
	addvirtuallineADV(tmp,"AMOUNT",2,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(138+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) THEN RETURN_VALUE=[PAYROLL][PAYJTRN][DEFINITION TYPE] ELSE RETURN_VALUE=[PAYROLL][PAYDTRN][DEFINITION TYPE] FI");
	addvirtuallineADV(tmp,"DEFINITION TYPE",7,2,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(132+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) THEN RETURN_VALUE=[PAYROLL][PAYJTRN][ACCOUNT CODE] ELSE RETURN_VALUE=[PAYROLL][PAYDTRN][ACCOUNT CODE] FI");
	addvirtuallineADV(tmp,"ACCOUNT CODE",22,40,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(128+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) THEN RETURN_VALUE=[PAYROLL][PAYJTRN][DEBIT CODE] ELSE RETURN_VALUE=[PAYROLL][PAYDTRN][DEBIT CODE] FI");
	addvirtuallineADV(tmp,"DEBIT CODE",24,40,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(130+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) THEN RETURN_VALUE=[PAYROLL][PAYJTRN][CREDIT CODE] ELSE RETURN_VALUE=[PAYROLL][PAYDTRN][CREDIT CODE] FI");
	addvirtuallineADV(tmp,"CREDIT CODE",24,40,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(164+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) THEN RETURN_VALUE=[PAYROLL][PAYJTRN][PAYROLL FUND DEFINITION TYPE] ELSE RETURN_VALUE=[PAYROLL][PAYDTRN][PAYROLL FUND DEFINITION TYPE] FI");
	addvirtuallineADV(tmp,"PAYROLL FUND DEFINITION TYPE",7,2,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(134+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) THEN RETURN_VALUE=[PAYROLL][PAYJTRN][PAYABLE CODE] ELSE RETURN_VALUE=[PAYROLL][PAYDTRN][LIABILITY CODE] FI");
	addvirtuallineADV(tmp,"PAYABLE / LIABILITY CODE",24,40,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(128+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) \nTHEN RETURN_VALUE=[PAYROLL][PAYJTRN][CASH CODE] ELSE RETURN_VALUE=[PAYROLL][PAYDTRN][CASH CODE] FI");
	addvirtuallineADV(tmp,"CASH CODE",24,40,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(130+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) \nTHEN RETURN_VALUE=[PAYROLL][PAYJTRN][DUE FROM] \nELSE RETURN_VALUE=[PAYROLL][PAYDTRN][DUE FROM] \nFI");
	addvirtuallineADV(tmp,"DUE FROM",24,40,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(157+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) \nTHEN RETURN_VALUE=[PAYROLL][PAYJTRN][USE PAYROLL FUND TYPE]  \nELSE RETURN_VALUE=[PAYROLL][PAYDTRN][USE PAYROLL FUND TYPE] \nFI");
	addvirtuallineADV(tmp,"USE PAYROLL FUND TYPE",7,2,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(132+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) THEN RETURN_VALUE=\"\" \nELSE RETURN_VALUE=[PAYROLL][PAYDTRN][DEDUCTION DESCRIPTION IDENTIFICATION] \nFI");
	addvirtuallineADV(tmp,"DEDUCTION DESCRIPTION IDENTIFICATION",1,30,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(32+1);
	sprintf(t,"[PAYROLL][PAYMSTR][FISCAL MONTH]");
	addvirtuallineADV(tmp,"FISCAL MONTH",7,2,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(889+1);
	sprintf(t,"IF([FISCAL MONTH]=0) THEN RETURN_VALUE=\"PRE-YEAR\"\nELSE IF([FISCAL MONTH]=1) THEN RETURN_VALUE=\"JANUARY\" \nELSE IF([FISCAL MONTH]=2) THEN RETURN_VALUE=\"FEBRUARY\" \nELSE IF([FISCAL MONTH]=3) THEN RETURN_VA\
LUE=\"MARCH\" \nELSE IF([FISCAL MONTH]=4) THEN RETURN_VALUE=\"APRIL\" \nELSE IF([FISCAL MONTH]=5) THEN RETURN_VALUE=\"MAY\" \nELSE IF([FISCAL MONTH]=6) THEN RETURN_VALUE=\"JUNE\" \nELSE IF([FISCAL MONTH]=7) THEN \
RETURN_VALUE=\"JULY\" \nELSE IF([FISCAL MONTH]=8) THEN RETURN_VALUE=\"AUGUST\" \nELSE IF([FISCAL MONTH]=9) THEN RETURN_VALUE=\"SEPTEMBER\" \nELSE IF([FISCAL MONTH]=10) THEN RETURN_VALUE=\"OCTOBER\" \nELSE IF([FIS\
CAL MONTH]=11) THEN RETURN_VALUE=\"NOVEMBER\" \nELSE IF([FISCAL MONTH]=12) THEN RETURN_VALUE=\"DECEMBER\" \nELSE IF([FISCAL MONTH]=13) THEN RETURN_VALUE=\"POST YEAR\" \nELSE RETURN_VALUE=\"INVALID MONTH!\" FI FI\
 FI FI FI FI FI FI FI FI FI FI FI FI");
	addvirtuallineADV(tmp,"FISCAL MONTH STRING",1,12,t,0,0);

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

	addsortline(tmp,0,"[PAY IDENTIFICATION]");
	addsortline(tmp,0,"[DEDUCTION DESCRIPTION IDENTIFICATION]");
	addsortline(tmp,0,"[NAME LAST LINEAGE FIRST MIDDLE]");
	addsortline(tmp,0,"[JOB NUMBER]");
	addsortline(tmp,0,"[ACCOUNT NUMBER]");
	addaccumline(tmp,"LAST PAY IDENTIFICATION",3,"[PAY IDENTIFICATION]",0);
	addaccumline(tmp,"LAST PERSONNEL IDENTIFICATION",3,"[PERSONNEL IDENTIFICATION]",0);
	addaccumline(tmp,"LAST DEDUCTION DESCRIPTION IDENTIFICATION",3,"[DEDUCTION DESCRIPTION IDENTIFICATION]",0);
	addaccumline(tmp,"LAST LOCATION IDENTIFICATION",3,"[PRSNNL][PERDMG][LOCATION IDENTIFICATION]",0);
	addaccumline(tmp,"LAST JOB NUMBER",3,"[JOB NUMBER]",0);
	addaccumline(tmp,"LAST ACCOUNT NUMBER",3,"[ACCOUNT NUMBER]",0);
	addaccumline(tmp,"LAST USE PAYROLL FUND TYPE",3,"[USE PAYROLL FUND TYPE]",0);
	addaccumline(tmp,"TOTAL AMOUNT",7,"[AMOUNT]",0);
	addaccumline(tmp,"FREQUENCY",1,"[DEDUCTION DESCRIPTION IDENTIFICATION]",0);
	addcontrolline(tmp,"[PAY IDENTIFICATION]",0);
	addcontrolline(tmp,"[DEDUCTION DESCRIPTION IDENTIFICATION]",0);
	addcontrolline(tmp,"[NAME LAST LINEAGE FIRST MIDDLE]",0);
	addcontrolline(tmp,"[JOB NUMBER]",0);




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

	pval=Rmalloc(1*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("Compressed Print");
	pvals->fieldname=NULL;
	addimageline(tmp,"REPORT HEADER","@",FALSE,NULL,1,pval);
	free_pvals(pval,1);


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
	addimageline(tmp,"PAGE HEADER","                 @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC        PAGE:@RRRRRRR@@",FALSE,NULL,4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
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
	addimageline(tmp,"PAGE HEADER","                                                      MAKE TRANSACTIONS AUDIT TRAIL                                             DATE:@LLLLLLL@@",FALSE,NULL,3,pval);
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
	addimageline(tmp,"PAGE HEADER","                 @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC        TIME:@LLLLLLL@@",FALSE,NULL,4,pval);
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


	pval=Rmalloc(8*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAY IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYMSTR][PAY DATE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYMSTR][FISCAL YEAR]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[FISCAL MONTH]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[FISCAL MONTH STRING]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYMSTR][DESCRIPTION]");
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
	addimageline(tmp,"[PAY IDENTIFICATION] HEADER","Pay ID: @LLLLLLLLLLLLLLLL Pay Date: @LLLLLLLLL Fiscal Year-Mo.: @RRR - @R (@LLLLLLLLLLL ) @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL@@",FALSE,NULL,8,pval);
	free_pvals(pval,8);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PRSNNL][PERDMG][LOCATION IDENTIFICATION]");
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
	addimageline(tmp,"[PRSNNL][PERDMG][LOCATION IDENTIFICATION] HEADER"," Pay Location ID: @VVVVVVVVVVVVV@@",FALSE,NULL,3,pval);
	free_pvals(pval,3);


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
	addimageline(tmp,"PAGE HEADER","  Personnel Name-SSN/Deduction ID Job# Personnel/GrossID Ac# DT Expenditure Account Code  Debit    Credit  PDT Payb/Lia Cash          Amount @@",TRUE,"[USE PAYROLL FUND TYPE]!=2",2,pval);
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
	addimageline(tmp,"PAGE HEADER","  Personnel Name-SSN/Deduction ID Job# Personnel/GrossID Ac# DT Expenditure Account Code  Debit    Credit  PDT Payb/Lia Due From      Amount @@",TRUE,"[USE PAYROLL FUND TYPE]=2",2,pval);
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
	addimageline(tmp,"PAGE HEADER","  ------------------------------- ---- ----------------- --- -- ------------------------- -------- -------- -- -------- -------- ------------@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(15*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[NAME LAST LINEAGE FIRST MIDDLE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PERSONNEL IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[JOB NUMBER]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYJMST][GROSS IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ACCOUNT NUMBER]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DEFINITION TYPE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ACCOUNT CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DEBIT CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[CREDIT CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL FUND DEFINITION TYPE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYABLE / LIABILITY CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[CASH CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[AMOUNT]");
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
	addimageline(tmp,"DETAIL LINES","  @LLLLLLLLLLLLLLLLLL @LLLLLLLLLL @RRR @LLLLLLLLLLLLLLLL @RR @L @LLLLLLLLLLLLLLLLLLLLLLLL @LLLLLLL @LLLLLLL @L @LLLLLLL @LLLLLLL @,.RRRRRRRRR@@",TRUE,"([DEDUCTION DESCRIPTION IDENTIFICATION]=\"\") AND ([USE PAYROLL FUND TYPE]!=2)",15,pval);
	free_pvals(pval,15);


	pval=Rmalloc(15*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[NAME LAST LINEAGE FIRST MIDDLE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PERSONNEL IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[JOB NUMBER]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYJMST][GROSS IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ACCOUNT NUMBER]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DEFINITION TYPE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ACCOUNT CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DEBIT CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[CREDIT CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL FUND DEFINITION TYPE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYABLE / LIABILITY CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DUE FROM]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[AMOUNT]");
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
	addimageline(tmp,"DETAIL LINES","  @LLLLLLLLLLLLLLLLLL @LLLLLLLLLL @RRR @LLLLLLLLLLLLLLLL @RR @L @LLLLLLLLLLLLLLLLLLLLLLLL @LLLLLLL @LLLLLLL @L @LLLLLLL @LLLLLLL @,.RRRRRRRRR@@",TRUE,"([DEDUCTION DESCRIPTION IDENTIFICATION]=\"\") AND ([USE PAYROLL FUND TYPE]=2)",15,pval);
	free_pvals(pval,15);


	pval=Rmalloc(15*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DEDUCTION DESCRIPTION IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[NAME LAST LINEAGE FIRST MIDDLE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PERSONNEL IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYDDMS][REDUCE NET]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ACCOUNT NUMBER]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DEFINITION TYPE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ACCOUNT CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DEBIT CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[CREDIT CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL FUND DEFINITION TYPE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYABLE / LIABILITY CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[CASH CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[AMOUNT]");
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
	addimageline(tmp,"DETAIL LINES","  @LLLLLLLLLLLLLLLLLLLLLL @LLLLLLLLLLLLLLLLL @LLLLLLLLLL@@RR @L @LLLLLLLLLLLLLLLLLLLLLLLL @LLLLLLL @LLLLLLL @L @LLLLLLL @LLLLLLL @,.RRRRRRRRR@@",TRUE,"([DEDUCTION DESCRIPTION IDENTIFICATION]!=\"\") AND ([USE PAYROLL FUND TYPE]!=2)",15,pval);
	free_pvals(pval,15);


	pval=Rmalloc(15*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DEDUCTION DESCRIPTION IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[NAME LAST LINEAGE FIRST MIDDLE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PERSONNEL IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYDDMS][REDUCE NET]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ACCOUNT NUMBER]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DEFINITION TYPE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ACCOUNT CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DEBIT CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[CREDIT CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL FUND DEFINITION TYPE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYABLE / LIABILITY CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DUE FROM]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[AMOUNT]");
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
	addimageline(tmp,"DETAIL LINES","  @LLLLLLLLLLLLLLLLLLLLLL @LLLLLLLLLLLLLLLLL @LLLLLLLLLL@@RR @L @LLLLLLLLLLLLLLLLLLLLLLLL @LLLLLLL @LLLLLLL @L @LLLLLLL @LLLLLLL @,.RRRRRRRRR@@",TRUE,"([DEDUCTION DESCRIPTION IDENTIFICATION]!=\"\") AND ([USE PAYROLL FUND TYPE]=2)",15,pval);
	free_pvals(pval,15);


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
	addimageline(tmp,"[DEDUCTION DESCRIPTION IDENTIFICATION] FOOTER","                                                                                                                         ====================@@",TRUE,"([FREQUENCY]>1)",2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL AMOUNT]");
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
	addimageline(tmp,"[DEDUCTION DESCRIPTION IDENTIFICATION] FOOTER","                                                                                             Total Job Earnings Charged: @,.RRRRRRRRRRRRRRRRR@@",TRUE,"([FREQUENCY]>1) AND ([LAST DEDUCTION DESCRIPTION IDENTIFICATION]=\"\")",3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST DEDUCTION DESCRIPTION IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL AMOUNT]");
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
	addimageline(tmp,"[DEDUCTION DESCRIPTION IDENTIFICATION] FOOTER","                                                        Total Deduction Description ID (@LLLLLLLLLLLLLLLLLLLLLLLLLLLL ): @,.RRRRRRRRRRRRRRRRR@@",TRUE,"([FREQUENCY]>1) AND (NOT([LAST DEDUCTION DESCRIPTION IDENTIFICATION]=\"\"))",4,pval);
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
	addimageline(tmp,"[DEDUCTION DESCRIPTION IDENTIFICATION] FOOTER","@@",FALSE,NULL,2,pval);
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
	addimageline(tmp,"[DEDUCTION DESCRIPTION IDENTIFICATION] FOOTER","@@",FALSE,NULL,2,pval);
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
	addimageline(tmp,"REPORT FOOTER","                                                                                                                         ====================@@",TRUE,"([FREQUENCY]>1)",2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL AMOUNT]");
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
	addimageline(tmp,"REPORT FOOTER","                                                                                                              TOTAL ALL: @,.RRRRRRRRRRRRRRRRR@@",TRUE,"([FREQUENCY]>1)",3,pval);
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
	addimageline(tmp,"[PAY IDENTIFICATION] FOOTER","@@@",FALSE,NULL,3,pval);
	free_pvals(pval,3);


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
	addimageline(tmp,"PAGE FOOTER","@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(4*sizeof(RDApval));
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
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("Normal Print");
	pvals->fieldname=NULL;
	addimageline(tmp,"REPORT FOOTER","@@@@",FALSE,NULL,4,pval);
	free_pvals(pval,4);






	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			ERRORDIALOG("Cannot Write Report","Error in permissions, user cannot write the report definition.  Check your permissions, and retry.  Call your installer.",NULL,FALSE);
			prterr("Cannont Write Report definition for Report [%s].  Error in permissions, user cannot write the report definition.",rptname);
		}
		FreeRDAreport(tmp);
	}
	if(rptname!=NULL) Rfree(rptname);
}
