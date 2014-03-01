/* PAYROLL's - Make (PAYVDST1) Report */
#include <cstdio>
#include <rptgen.hpp>
/*COMPMEM 40960 */

extern void free_pvals(RDApval *,int);
extern char *module;
void PAYVDST1(void);

void PAYVDST1()
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
	rptname=Rmalloc(8+1);
	sprintf(rptname,"PAYVDST1");
	t=Rmalloc(39+1);
	sprintf(t,"Payroll Vendor Withholding Distribution");
	desc=Rmalloc(117+1);
	sprintf(desc,"This output definition is used to update the appropriate database(s) associated with Vendor Withholding Distribution.");
	tmp=RDAreportNEWADV(rptname,t,desc,1,"",FALSE,TRUE,FALSE,FALSE,FALSE,"","",TRUE,0,TRUE,"");
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
	adddisplayADV(tmp,(double)8.500000,(double)11.000000,4,FALSE,0,"",0,1,0,pvars);

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

	t=Rmalloc(134+1);
	sprintf(t,"([PAYROLL][PAYDDSV][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYDDSV][SOURCE USER VERIFIED]=TRUE) AND ([PAYROLL][PAYDDSV][DISTRIBUTED]=FALSE)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDDSV",0,NULL,NULL,"PAYDDSV KEY",TRUE,t,NULL,2);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","");
		addsearchconfld(s,"FISCAL YEAR","");
		addsearchconfld(s,"TRANSACTION NUMBER","");
	}
	if(t!=NULL) Rfree(t);

	x=addsearchfileADV(tmp,"FINMGT","FINBACC",2,"PAYROLL","PAYDDSV","ACCOUNT CODE KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[PAYDDSV][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
	}

	x=addsearchfileADV(tmp,"FINMGT","FINBYR",2,"PAYROLL","PAYDDSV","FINBYR KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[PAYDDSV][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
		addsearchconfld(s,"FISCAL YEAR","[PAYDDSV][FISCAL YEAR]");
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
	addvirtuallineADV(tmp,"EXECUTED BY",0,0,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(13+1);
	sprintf(t,"(\"PAYROLL\")");
	addvirtuallineADV(tmp,"SOURCE MODULE",0,0,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(13+1);
	sprintf(t,"(\"PAYVDST\")");
	addvirtuallineADV(tmp,"SOURCE PROCESS",0,0,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(121+1);
	sprintf(t,"IF([WRITE COUNT]=1) THEN RETURN_VALUE=[PAYROLL][PAYDDSV][DEBIT CODE] ELSE RETURN_VALUE=[PAYROLL][PAYDDSV][CREDIT CODE] FI");
	addvirtuallineADV(tmp,"ACCOUNT CODE",1,40,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(87+1);
	sprintf(t,"IF([WRITE COUNT]=1) THEN RETURN_VALUE=[PAYROLL][PAYDDSV][AMOUNT] ELSE RETURN_VALUE=0 FI");
	addvirtuallineADV(tmp,"DEBIT AMOUNT",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(87+1);
	sprintf(t,"IF([WRITE COUNT]=2) THEN RETURN_VALUE=[PAYROLL][PAYDDSV][AMOUNT] ELSE RETURN_VALUE=0 FI");
	addvirtuallineADV(tmp,"CREDIT AMOUNT",2,20,t,0,0);

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

	addsortline(tmp,0,"[PAYROLL][PAYDDSV][FISCAL YEAR]");
	addsortline(tmp,0,"[PAYROLL][PAYDDSV][DEFINITION TYPE]");
	addsortline(tmp,0,"[ACCOUNT CODE]");
	addsortline(tmp,0,"[PAYROLL][PAYDDSV][PROCESSING MONTH]");
	addaccumline(tmp,"TOTAL DEBIT AMOUNT",7,"[DEBIT AMOUNT]",0);
	addaccumline(tmp,"TOTAL CREDIT AMOUNT",7,"[CREDIT AMOUNT]",0);
	addaccumline(tmp,"LAST FISCAL YEAR",3,"[PAYROLL][PAYDDSV][FISCAL YEAR]",0);
	addaccumline(tmp,"LAST DEFINITION TYPE",3,"[PAYROLL][PAYDDSV][DEFINITION TYPE]",0);
	addaccumline(tmp,"LAST ACCOUNT CODE",3,"[ACCOUNT CODE]",0);
	addaccumline(tmp,"LAST ACCOUNT CODE DESCRIPTION",3,"[FINMGT][FINBACC][DESCRIPTION]",0);
	addaccumline(tmp,"LAST PROCESSING MONTH",3,"[PAYROLL][PAYDDSV][PROCESSING MONTH]",0);
	addcontrolline(tmp,"[PAYROLL][PAYDDSV][FISCAL YEAR]",0);
	addcontrolline(tmp,"[PAYROLL][PAYDDSV][DEFINITION TYPE]",0);
	addcontrolline(tmp,"[ACCOUNT CODE]",0);
	addcontrolline(tmp,"[PAYROLL][PAYDDSV][PROCESSING MONTH]",0);




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

	addimageline(tmp,"REPORT HEADER","OPEN [FINMGT][FINBACT]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT HEADER","OPEN [FINMGT][FINBYR]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT HEADER","OPEN [PAYROLL][PAYDDSV]",FALSE,NULL,0,pval);


	pval=Rmalloc(7*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYDDSV][DEDUCTION DESCRIPTION IDENTIFICATION]");
	pvals->fieldname=stralloc("[PAYROLL][PAYDDSV][DEDUCTION DESCRIPTION IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYDDSV][FISCAL YEAR]");
	pvals->fieldname=stralloc("[PAYROLL][PAYDDSV][FISCAL YEAR]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYDDSV][TRANSACTION NUMBER]");
	pvals->fieldname=stralloc("[PAYROLL][PAYDDSV][TRANSACTION NUMBER]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("TRUE");
	pvals->fieldname=stralloc("[PAYROLL][PAYDDSV][DISTRIBUTED]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LOGIN IDENTIFICATION]");
	pvals->fieldname=stralloc("[PAYROLL][PAYDDSV][DISTRIBUTED BY]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM DATE]");
	pvals->fieldname=stralloc("[PAYROLL][PAYDDSV][DISTRIBUTED DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM TIME]");
	pvals->fieldname=stralloc("[PAYROLL][PAYDDSV][DISTRIBUTED TIME]");
	addimageline(tmp,"DETAIL LINES","WRITE [PAYROLL][PAYDDSV]",FALSE,NULL,7,pval);
	free_pvals(pval,7);


	pval=Rmalloc(12*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST DEFINITION TYPE]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][DEFINITION TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST ACCOUNT CODE]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][ACCOUNT CODE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST FISCAL YEAR]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][FISCAL YEAR]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL DEBIT AMOUNT]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][AMOUNT]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM DATE]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][ENTRY DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM TIME]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][ENTRY TIME]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("[LAST PROCESSING MONTH]+1");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][PROCESSING MONTH]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SOURCE MODULE]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][SOURCE MODULE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SOURCE PROCESS]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][SOURCE PROCESS]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LOGIN IDENTIFICATION]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][SOURCE USER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("NEXT TRANSACTION NO");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][TRANSACTION NUMBER]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("0");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][TYPE]");
	addimageline(tmp,"[PAYROLL][PAYDDSV][PROCESSING MONTH] FOOTER","WRITE [FINMGT][FINBACT]",TRUE,"([TOTAL DEBIT AMOUNT]!=0)",12,pval);
	free_pvals(pval,12);


	pval=Rmalloc(12*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST ACCOUNT CODE]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][ACCOUNT CODE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL CREDIT AMOUNT]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][AMOUNT]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST DEFINITION TYPE]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][DEFINITION TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM DATE]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][ENTRY DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM TIME]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][ENTRY TIME]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("[LAST PROCESSING MONTH]+1");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][PROCESSING MONTH]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SOURCE MODULE]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][SOURCE MODULE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SOURCE PROCESS]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][SOURCE PROCESS]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LOGIN IDENTIFICATION]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][SOURCE USER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("NEXT TRANSACTION NO");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][TRANSACTION NUMBER]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("1");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST FISCAL YEAR]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][FISCAL YEAR]");
	addimageline(tmp,"[PAYROLL][PAYDDSV][PROCESSING MONTH] FOOTER","WRITE [FINMGT][FINBACT]",TRUE,"([TOTAL CREDIT AMOUNT]!=0)",12,pval);
	free_pvals(pval,12);


	pval=Rmalloc(27*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST DEFINITION TYPE]");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][DEFINITION TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST ACCOUNT CODE]");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][ACCOUNT CODE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST FISCAL YEAR]");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][FISCAL YEAR]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=0) THEN RETURN_VALUE=[FINMGT][FINBYR][JANUARY UNDISTRIBUTED DEBITS]-[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][JANUARY UNDISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JANUARY UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=0) THEN RETURN_VALUE=[FINMGT][FINBYR][JANUARY DISTRIBUTED DEBITS]+[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][JANUARY DISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JANUARY DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=1) THEN RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED DEBITS]-[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=1) THEN RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY DISTRIBUTED DEBITS]+[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY DISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][FEBRUARY DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=2) THEN RETURN_VALUE=[FINMGT][FINBYR][MARCH UNDISTRIBUTED DEBITS]-[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][MARCH UNDISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][MARCH UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=2) THEN RETURN_VALUE=[FINMGT][FINBYR][MARCH DISTRIBUTED DEBITS]+[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][MARCH DISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][MARCH DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=3) THEN RETURN_VALUE=[FINMGT][FINBYR][APRIL UNDISTRIBUTED DEBITS]-[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][APRIL UNDISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][APRIL UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=3) THEN RETURN_VALUE=[FINMGT][FINBYR][APRIL DISTRIBUTED DEBITS]+[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][APRIL DISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][APRIL DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=4) THEN RETURN_VALUE=[FINMGT][FINBYR][MAY UNDISTRIBUTED DEBITS]-[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][MAY UNDISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][MAY UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=4) THEN RETURN_VALUE=[FINMGT][FINBYR][MAY DISTRIBUTED DEBITS]+[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][MAY DISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][MAY DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=5) THEN RETURN_VALUE=[FINMGT][FINBYR][JUNE UNDISTRIBUTED DEBITS]-[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][JUNE UNDISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JUNE UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=5) THEN RETURN_VALUE=[FINMGT][FINBYR][JUNE DISTRIBUTED DEBITS]+[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][JUNE DISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JUNE DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=6) THEN RETURN_VALUE=[FINMGT][FINBYR][JULY UNDISTRIBUTED DEBITS]-[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][JULY UNDISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JULY UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=6) THEN RETURN_VALUE=[FINMGT][FINBYR][JULY DISTRIBUTED DEBITS]+[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][JULY DISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JULY DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=7) THEN RETURN_VALUE=[FINMGT][FINBYR][AUGUST UNDISTRIBUTED DEBITS]-[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][AUGUST UNDISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][AUGUST UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=7) THEN RETURN_VALUE=[FINMGT][FINBYR][AUGUST DISTRIBUTED DEBITS]+[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][AUGUST DISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][AUGUST DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=8) THEN RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED DEBITS]-[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=8) THEN RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER DISTRIBUTED DEBITS]+[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER DISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][SEPTEMBER DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=9) THEN RETURN_VALUE=[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED DEBITS]-[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=9) THEN RETURN_VALUE=[FINMGT][FINBYR][OCTOBER DISTRIBUTED DEBITS]+[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][OCTOBER DISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][OCTOBER DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=10) THEN RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED DEBITS]-[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=10) THEN RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER DISTRIBUTED DEBITS]+[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER DISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][NOVEMBER DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=11) THEN RETURN_VALUE=[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED DEBITS]-[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=11) THEN RETURN_VALUE=[FINMGT][FINBYR][DECEMBER DISTRIBUTED DEBITS]+[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][DECEMBER DISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][DECEMBER DISTRIBUTED DEBITS]");
	addimageline(tmp,"[PAYROLL][PAYDDSV][PROCESSING MONTH] FOOTER","WRITE [FINMGT][FINBYR]",TRUE,"([TOTAL DEBIT AMOUNT]!=0.0)",27,pval);
	free_pvals(pval,27);


	pval=Rmalloc(27*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST DEFINITION TYPE]");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][DEFINITION TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST ACCOUNT CODE]");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][ACCOUNT CODE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST FISCAL YEAR]");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][FISCAL YEAR]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=0) THEN RETURN_VALUE=[FINMGT][FINBYR][JANUARY UNDISTRIBUTED CREDITS]-[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][JANUARY UNDISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JANUARY UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=0) THEN RETURN_VALUE=[FINMGT][FINBYR][JANUARY DISTRIBUTED CREDITS]+[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][JANUARY DISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JANUARY DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=1) THEN RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED CREDITS]-[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=1) THEN RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY DISTRIBUTED CREDITS]+[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY DISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][FEBRUARY DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=2) THEN RETURN_VALUE=[FINMGT][FINBYR][MARCH UNDISTRIBUTED CREDITS]-[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][MARCH UNDISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][MARCH UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=2) THEN RETURN_VALUE=[FINMGT][FINBYR][MARCH DISTRIBUTED CREDITS]+[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][MARCH DISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][MARCH DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=3) THEN RETURN_VALUE=[FINMGT][FINBYR][APRIL UNDISTRIBUTED CREDITS]-[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][APRIL UNDISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][APRIL UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=3) THEN RETURN_VALUE=[FINMGT][FINBYR][APRIL DISTRIBUTED CREDITS]+[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][APRIL DISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][APRIL DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=4) THEN RETURN_VALUE=[FINMGT][FINBYR][MAY UNDISTRIBUTED CREDITS]-[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][MAY UNDISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][MAY UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=4) THEN RETURN_VALUE=[FINMGT][FINBYR][MAY DISTRIBUTED CREDITS]+[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][MAY DISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][MAY DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=5) THEN RETURN_VALUE=[FINMGT][FINBYR][JUNE UNDISTRIBUTED CREDITS]-[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][JUNE UNDISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JUNE UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=5) THEN RETURN_VALUE=[FINMGT][FINBYR][JUNE DISTRIBUTED CREDITS]+[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][JUNE DISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JUNE DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=6) THEN RETURN_VALUE=[FINMGT][FINBYR][JULY UNDISTRIBUTED CREDITS]-[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][JULY UNDISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JULY UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=6) THEN RETURN_VALUE=[FINMGT][FINBYR][JULY DISTRIBUTED CREDITS]+[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][JULY DISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JULY DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=7) THEN RETURN_VALUE=[FINMGT][FINBYR][AUGUST UNDISTRIBUTED CREDITS]-[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][AUGUST UNDISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][AUGUST UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=7) THEN RETURN_VALUE=[FINMGT][FINBYR][AUGUST DISTRIBUTED CREDITS]+[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][AUGUST DISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][AUGUST DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=8) THEN RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED CREDITS]-[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=8) THEN RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER DISTRIBUTED CREDITS]+[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][SEPTEMBER DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=9) THEN RETURN_VALUE=[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED CREDITS]-[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=9) THEN RETURN_VALUE=[FINMGT][FINBYR][OCTOBER DISTRIBUTED CREDITS]+[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][OCTOBER DISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][OCTOBER DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=10) THEN RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED CREDITS]-[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=10) THEN RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER DISTRIBUTED CREDITS]+[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER DISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][NOVEMBER DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=11) THEN RETURN_VALUE=[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED CREDITS]-[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=11) THEN RETURN_VALUE=[FINMGT][FINBYR][DECEMBER DISTRIBUTED CREDITS]+[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][DECEMBER DISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][DECEMBER DISTRIBUTED CREDITS]");
	addimageline(tmp,"[PAYROLL][PAYDDSV][PROCESSING MONTH] FOOTER","WRITE [FINMGT][FINBYR]",TRUE,"([TOTAL CREDIT AMOUNT]!=0.0)",27,pval);
	free_pvals(pval,27);


	addimageline(tmp,"REPORT FOOTER","CLOSE [FINMGT][FINBACT]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT FOOTER","CLOSE [FINMGT][FINBYR]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT FOOTER","CLOSE [PAYROLL][PAYDDSV]",FALSE,NULL,0,pval);






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
