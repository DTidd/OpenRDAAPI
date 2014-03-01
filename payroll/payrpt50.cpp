/* PAYROLL's - Make (PAYMTSA) Report */
/* WARNINGSJS - report needs to have PAYJMST, PAYPPMS and PAYMSTR added to 
                second detail for new report generator */
#include <cstdio>
#include <rptgen.hpp>
/*COMPMEM 40960 */

extern void free_pvals(RDApval *,int);
extern char *module;
void PAYMTSA(void);
void PAYMTSA1(void);

void PAYMTSA()
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
	sprintf(rptname,"PAYMTSA");
	t=Rmalloc(29+1);
	sprintf(t,"Make Transactions Audit Trail");
	desc=Rmalloc(79+1);
	sprintf(desc,"This output definition is used to provide an audit trail for Make Transactions.");
	tmp=RDAreportNEWADV(rptname,t,desc,1,"",FALSE,TRUE,FALSE,TRUE,TRUE,"PAYROLL","PAYMTSA1",FALSE,0,FALSE,"");
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
	addvirtuallineADV(tmp,"EXECUTED BY",0,0,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(21+1);
	sprintf(t,"[PRIMARY FILE NUMBER]");
	addvirtuallineADV(tmp,"TRANSACTION TYPE",7,2,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(156+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) THEN RETURN_VALUE=[PAYROLL][PAYJTRN][PERSONNEL IDENTIFICATION] ELSE RETURN_VALUE=[PAYROLL][PAYDTRN][PERSONNEL IDENTIFICATION] FI");
	addvirtuallineADV(tmp,"PERSONNEL IDENTIFICATION",1,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(144+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) THEN RETURN_VALUE=[PAYROLL][PAYJTRN][PAY IDENTIFICATION] ELSE RETURN_VALUE=[PAYROLL][PAYDTRN][PAY IDENTIFICATION] FI");
	addvirtuallineADV(tmp,"PAY IDENTIFICATION",1,15,t,0,0);

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
	t=Rmalloc(126+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) THEN RETURN_VALUE=[PAYROLL][PAYJTRN][CASH CODE] ELSE RETURN_VALUE=[PAYROLL][PAYDTRN][CASH CODE] FI");
	addvirtuallineADV(tmp,"CASH CODE",24,40,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(128+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) THEN RETURN_VALUE=\"\" ELSE RETURN_VALUE=[PAYROLL][PAYDTRN][DEDUCTION DESCRIPTION IDENTIFICATION] FI");
	addvirtuallineADV(tmp,"DEDUCTION DESCRIPTION IDENTIFICATION",1,30,t,0,0);

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

	addsortline(tmp,0,"[PAY IDENTIFICATION]");
	addsortline(tmp,0,"[PERSONNEL IDENTIFICATION]");
	addsortline(tmp,0,"[JOB NUMBER]");
	addsortline(tmp,0,"[ACCOUNT NUMBER]");
	addsortline(tmp,0,"[DEDUCTION DESCRIPTION IDENTIFICATION]");
	addaccumline(tmp,"LAST PAY IDENTIFICATION",3,"[PAY IDENTIFICATION]",0);
	addaccumline(tmp,"LAST PERSONNEL IDENTIFICATION",3,"[PERSONNEL IDENTIFICATION]",0);
	addaccumline(tmp,"LAST JOB NUMBER",3,"[JOB NUMBER]",0);
	addaccumline(tmp,"LAST ACCOUNT NUMBER",3,"[ACCOUNT NUMBER]",0);
	addaccumline(tmp,"TOTAL AMOUNT",7,"[AMOUNT]",0);
	addaccumline(tmp,"FREQUENCY",1,"[DEDUCTION DESCRIPTION IDENTIFICATION]",0);
	addcontrolline(tmp,"[PAY IDENTIFICATION]",0);
	addcontrolline(tmp,"[PERSONNEL IDENTIFICATION]",0);
	addcontrolline(tmp,"[JOB NUMBER]",0);
	addcontrolline(tmp,"[ACCOUNT NUMBER]",0);




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
	addimageline(tmp,"PAGE HEADER","                                             @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                    PAGE:@RRRRRRR@@",FALSE,NULL,4,pval);
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
	addimageline(tmp,"PAGE HEADER","                                                 MAKE TRANSACTIONS AUDIT TRAIL                                      DATE:@LLLLLLL@@",FALSE,NULL,3,pval);
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
	addimageline(tmp,"PAGE HEADER","                                             @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                    TIME:@LLLLLLL@@",FALSE,NULL,4,pval);
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
	addimageline(tmp,"PAGE HEADER","PAY IDENTIFICATION@@",FALSE,NULL,2,pval);
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
	addimageline(tmp,"PAGE HEADER","     PERSONNEL IDENTIFICATION@@",FALSE,NULL,2,pval);
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


	pval=Rmalloc(5*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAY IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYMSTR][DESCRIPTION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYMSTR][PAY DATE]");
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
	addimageline(tmp,"[PAY IDENTIFICATION] HEADER","@VVVVVVVVVV @VVVVVVVVVVVVVV @VVVVV@@",FALSE,NULL,5,pval);
	free_pvals(pval,5);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PERSONNEL IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[NAME FIRST MIDDLE LAST LINEAGE]");
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
	addimageline(tmp,"[PERSONNEL IDENTIFICATION] HEADER","  @VVVVVVVVVVVVVVV @VVVVVVVVVVVVVVVVVVVVVVVV@@",FALSE,NULL,4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(6*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[JOB NUMBER]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYJMST][POSITION IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYJMST][GROSS IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYJMST][DESCRIPTION]");
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
	addimageline(tmp,"[JOB NUMBER] HEADER","    @LL @VVVVVVVVVV @VVVVVVVVVVVVV @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL@@",FALSE,NULL,6,pval);
	free_pvals(pval,6);


	pval=Rmalloc(11*sizeof(RDApval));
	pvals=pval;
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
	addimageline(tmp,"DETAIL LINES","@RRRR JOB TRANSACTION @L @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLL @LLLLLLLLLLLLL @LLLLLLLLLLLLL @L @LLLLLLLLLL @LLLLLLLLLL @,.RRRRRRRRRRRR@@",TRUE,"([DEDUCTION DESCRIPTION IDENTIFICATION]=\"\")",11,pval);
	free_pvals(pval,11);


	pval=Rmalloc(12*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ACCOUNT NUMBER]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DEDUCTION DESCRIPTION IDENTIFICATION]");
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
	addimageline(tmp,"DETAIL LINES","@RRRR @LLLLLLLLLLLLLL @L @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLL @LLLLLLLLLLLLL @LLLLLLLLLLLLL @L @LLLLLLLLLL @LLLLLLLLLL @,.RRRRRRRRRRRR@@",TRUE,"([DEDUCTION DESCRIPTION IDENTIFICATION]!=\"\")",12,pval);
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
	addimageline(tmp,"[JOB NUMBER] FOOTER","                                                                                                                  ----------------@@",TRUE,"([FREQUENCY]>1)",2,pval);
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
	addimageline(tmp,"[PERSONNEL IDENTIFICATION] FOOTER","                                                                                                                  ----------------@@",TRUE,"([FREQUENCY]>1)",2,pval);
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
	addimageline(tmp,"[PAY IDENTIFICATION] FOOTER","                                                                                                                  ----------------@@",TRUE,"([FREQUENCY]>1)",2,pval);
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
	addimageline(tmp,"REPORT FOOTER","                                                                                                                  ----------------@@",TRUE,"([FREQUENCY]>1)",2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST JOB NUMBER]");
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
	addimageline(tmp,"[JOB NUMBER] FOOTER","                                                                                           TOTAL JOB NUMBER: @RRR @,.RRRRRRRRRRRR@@",TRUE,"([FREQUENCY]>1)",4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST PERSONNEL IDENTIFICATION]");
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
	addimageline(tmp,"[PERSONNEL IDENTIFICATION] FOOTER","                                                            TOTAL PERSONNEL IDENTIFICATION: @LLLLLLLLLLLLLLLLLLLL @,.RRRRRRRRRRRR@@",TRUE,"([FREQUENCY]>1)",4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST PAY IDENTIFICATION]");
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
	addimageline(tmp,"[PAY IDENTIFICATION] FOOTER","                                                                  TOTAL PAY IDENTIFICATION: @LLLLLLLLLLLLLLLLLLLL @,.RRRRRRRRRRRR@@",TRUE,"([FREQUENCY]>1)",4,pval);
	free_pvals(pval,4);


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
	addimageline(tmp,"REPORT FOOTER","                                                                                                       TOTAL ALL: @,.RRRRRRRRRRRR@@",TRUE,"([FREQUENCY]>1)",3,pval);
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
	addimageline(tmp,"PAGE FOOTER","@@@",FALSE,NULL,3,pval);
	free_pvals(pval,3);






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
/* PAYROLL's - Make (PAYMTSA1) Report */
void PAYMTSA1()
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
	sprintf(rptname,"PAYMTSA1");
	t=Rmalloc(29+1);
	sprintf(t,"Make Transactions Audit Trail");
	desc=Rmalloc(113+1);
	sprintf(desc,"This output definition is used to update the verified status of transactions in the Make Transactions Audit Trail");
	tmp=RDAreportNEWADV(rptname,t,desc,1,"",FALSE,FALSE,FALSE,FALSE,FALSE,"","",FALSE,0,TRUE,"");
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
	adddisplayADV(tmp,(double)8.500000,(double)10.000000,4,FALSE,0,"",0,1,0,pvars);

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
	addvirtuallineADV(tmp,"EXECUTED BY",0,0,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(21+1);
	sprintf(t,"[PRIMARY FILE NUMBER]");
	addvirtuallineADV(tmp,"TRANSACTION TYPE",7,2,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(156+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) THEN RETURN_VALUE=[PAYROLL][PAYJTRN][PERSONNEL IDENTIFICATION] ELSE RETURN_VALUE=[PAYROLL][PAYDTRN][PERSONNEL IDENTIFICATION] FI");
	addvirtuallineADV(tmp,"PERSONNEL IDENTIFICATION",1,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(144+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) THEN RETURN_VALUE=[PAYROLL][PAYJTRN][PAY IDENTIFICATION] ELSE RETURN_VALUE=[PAYROLL][PAYDTRN][PAY IDENTIFICATION] FI");
	addvirtuallineADV(tmp,"PAY IDENTIFICATION",1,15,t,0,0);

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
	t=Rmalloc(126+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) THEN RETURN_VALUE=[PAYROLL][PAYJTRN][CASH CODE] ELSE RETURN_VALUE=[PAYROLL][PAYDTRN][CASH CODE] FI");
	addvirtuallineADV(tmp,"CASH CODE",24,40,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(128+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) THEN RETURN_VALUE=\"\" ELSE RETURN_VALUE=[PAYROLL][PAYDTRN][DEDUCTION DESCRIPTION IDENTIFICATION] FI");
	addvirtuallineADV(tmp,"DEDUCTION DESCRIPTION IDENTIFICATION",1,30,t,0,0);

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
	addsortline(tmp,0,"[PERSONNEL IDENTIFICATION]");
	addsortline(tmp,0,"[JOB NUMBER]");
	addsortline(tmp,0,"[ACCOUNT NUMBER]");
	addsortline(tmp,0,"[DEDUCTION DESCRIPTION IDENTIFICATION]");
	addaccumline(tmp,"LAST PAY IDENTIFICATION",3,"[PAY IDENTIFICATION]",0);
	addaccumline(tmp,"LAST PERSONNEL IDENTIFICATION",3,"[PERSONNEL IDENTIFICATION]",0);
	addaccumline(tmp,"LAST JOB NUMBER",3,"[JOB NUMBER]",0);
	addaccumline(tmp,"LAST ACCOUNT NUMBER",3,"[ACCOUNT NUMBER]",0);
	addaccumline(tmp,"TOTAL AMOUNT",7,"[AMOUNT]",0);
	addcontrolline(tmp,"[PAY IDENTIFICATION]",0);
	addcontrolline(tmp,"[PERSONNEL IDENTIFICATION]",0);
	addcontrolline(tmp,"[JOB NUMBER]",0);
	addcontrolline(tmp,"[ACCOUNT NUMBER]",0);




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

	addimageline(tmp,"REPORT HEADER","OPEN [PAYROLL][PAYJTRN]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT HEADER","OPEN [PAYROLL][PAYDTRN]",FALSE,NULL,0,pval);


	pval=Rmalloc(5*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PERSONNEL IDENTIFICATION]");
	pvals->fieldname=stralloc("[PAYROLL][PAYJTRN][PERSONNEL IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[JOB NUMBER]");
	pvals->fieldname=stralloc("[PAYROLL][PAYJTRN][JOB NUMBER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAY IDENTIFICATION]");
	pvals->fieldname=stralloc("[PAYROLL][PAYJTRN][PAY IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ACCOUNT NUMBER]");
	pvals->fieldname=stralloc("[PAYROLL][PAYJTRN][ACCOUNT NUMBER]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("TRUE");
	pvals->fieldname=stralloc("[PAYROLL][PAYJTRN][SOURCE USER VERIFIED]");
	addimageline(tmp,"DETAIL LINES","WRITE [PAYROLL][PAYJTRN]",TRUE,"([DEDUCTION DESCRIPTION IDENTIFICATION]=\"\")",5,pval);
	free_pvals(pval,5);


	pval=Rmalloc(6*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PERSONNEL IDENTIFICATION]");
	pvals->fieldname=stralloc("[PAYROLL][PAYDTRN][PERSONNEL IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[JOB NUMBER]");
	pvals->fieldname=stralloc("[PAYROLL][PAYDTRN][JOB NUMBER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAY IDENTIFICATION]");
	pvals->fieldname=stralloc("[PAYROLL][PAYDTRN][PAY IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ACCOUNT NUMBER]");
	pvals->fieldname=stralloc("[PAYROLL][PAYDTRN][ACCOUNT NUMBER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DEDUCTION DESCRIPTION IDENTIFICATION]");
	pvals->fieldname=stralloc("[PAYROLL][PAYDTRN][DEDUCTION DESCRIPTION IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("TRUE");
	pvals->fieldname=stralloc("[PAYROLL][PAYDTRN][SOURCE USER VERIFIED]");
	addimageline(tmp,"DETAIL LINES","WRITE [PAYROLL][PAYDTRN]",TRUE,"([DEDUCTION DESCRIPTION IDENTIFICATION]!=\"\")",6,pval);
	free_pvals(pval,6);


	addimageline(tmp,"REPORT FOOTER","CLOSE [PAYROLL][PAYJTRN]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT FOOTER","CLOSE [PAYROLL][PAYDTRN]",FALSE,NULL,0,pval);






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
