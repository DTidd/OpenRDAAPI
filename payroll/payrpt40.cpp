/* PAYROLL's - Make (VWCHECK-WJCC) Report */
#include <cstdio>
#include <rptgen.hpp>
/*COMPMEM 40960 */

extern void free_pvals(RDApval *,int);
extern char *module;
void VWCHECK_WJC(void);

void VWCHECK_WJC()
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
	rptname=Rmalloc(12+1);
	sprintf(rptname,"VWCHECK-WJCC");
	t=Rmalloc(39+1);
	sprintf(t,"Payroll Vendor Withholding Check Report");
	desc=Rmalloc(133+1);
	sprintf(desc,"This output definition is used to produce the Vendor Checks which outputs the actual checks from payroll deduction vouchers selected.");
	tmp=RDAreportNEWADV(rptname,t,desc,0,"",FALSE,TRUE,FALSE,TRUE,FALSE,"","",TRUE,1,TRUE,"");
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

	adddisplayADV(tmp,(double)8.500000,(double)0.000000,0,TRUE,1,"",14,1,0,NULL);

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

	t=Rmalloc(351+1);
	sprintf(t,"(([PAYROLL][PAYDDSV][SOURCE USER VERIFIED]=TRUE) AND ([PAYROLL][PAYDDSV][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYDDSV][BNKREC UPDATED]=FALSE) AND ([PAYROLL][PAYDDSV][MANUAL VOUCHER]=FALSE) AND ([PAYROLL][\
PAYDDSV][BANK IDENTIFICATION]=[SELECTED BANK IDENTIFICATION]) AND ([PAYROLL][PAYDDSV][DISTRIBUTED]=FALSE) AND ([PAYROLL][PAYDDSV][CHECK NUMBER]!=0))");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDDSV",0,NULL,NULL,"PAYDDSV KEY",TRUE,t,NULL,1);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(38+1);
	sprintf(t,"([PAYROLL][PAYDDMS][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDDMS",1,"PAYROLL","PAYDDSV","PAYDDMS KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","[PAYDDSV][DEDUCTION DESCRIPTION IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(36+1);
	sprintf(t,"([VENPMT][VENMSR][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"VENPMT","VENMSR",2,"PAYROLL","PAYDDSV","VENDOR ID KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"VENDOR IDENTIFICATION","[PAYDDSV][VENDOR IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(36+1);
	sprintf(t,"([BNKREC][BNKACC][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"BNKREC","BNKACC",2,"PAYROLL","PAYDDSV","BNKACC KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"BANK IDENTIFICATION","[PAYDDSV][BANK IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	x=addsearchfileADV(tmp,"VENPMT","VENADR",1,"PAYROLL","PAYDDSV","VENDOR ADDRESS KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"VENDOR IDENTIFICATION","[PAYDDSV][VENDOR IDENTIFICATION]");
		addsearchconfld(s,"ADDRESS IDENTIFICATION","[PAYDDSV][ADDRESS IDENTIFICATION]");
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
	addvirtuallineADV(tmp,"SELECTED BANK IDENTIFICATION",0,0,t,0,2);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(2+1);
	sprintf(t,"14");
	addvirtuallineADV(tmp,"SELECTED BODY COUNT",18,2,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(538+1);
	sprintf(t,"IF(([VENPMT][VENADR][ADDRESS 1]=\"\") AND ([VENPMT][VENADR][ADDRESS 2]=\"\") AND  ([VENPMT][VENADR][CITY]=\"\") AND  ([VENPMT][VENADR][STATE]=\"\") AND  ([VENPMT][VENADR][ZIP]=\"\")) THEN RETURN_VALUE=\"\" ELSE IF\
(([VENPMT][VENADR][ADDRESS 1]=\"\") AND ([VENPMT][VENADR][ADDRESS 2]=\"\")) THEN RETURN_VALUE=([VENPMT][VENADR][CITY]+\", \"+[VENPMT][VENADR][STATE]+\"  \"+[VENPMT][VENADR][ZIP]) ELSE IF([VENPMT][VENADR][ADDR\
ESS 1]=\"\") THEN RETURN_VALUE=[VENPMT][VENADR][ADDRESS 2] ELSE RETURN_VALUE=[VENPMT][VENADR][ADDRESS 1] FI FI FI");
	addvirtuallineADV(tmp,"VENDOR ADDRESS 1",0,0,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(475+1);
	sprintf(t,"IF(([VENPMT][VENADR][ADDRESS 1]=\"\") AND ([VENPMT][VENADR][ADDRESS 2]=\"\")) THEN RETURN_VALUE=\"\" ELSE IF(([VENPMT][VENADR][ADDRESS 1]=\"\") AND ([VENPMT][VENADR][CITY]=\"\") AND ([VENPMT][VENADR][STATE]=\"\") \
AND ([VENPMT][VENADR][ZIP]=\"\")) THEN RETURN_VALUE=(\"\") ELSE IF([VENPMT][VENADR][ADDRESS 2]=\"\") THEN RETURN_VALUE=([VENPMT][VENADR][CITY]+\", \"+[VENPMT][VENADR][STATE]+\"  \"+[VENPMT][VENADR][ZIP]) ELSE R\
ETURN_VALUE=[VENPMT][VENADR][ADDRESS 2] FI FI FI");
	addvirtuallineADV(tmp,"VENDOR ADDRESS 2",0,0,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(309+1);
	sprintf(t,"IF(([VENPMT][VENADR][ADDRESS 1]=\"\") OR ([VENPMT][VENADR][ADDRESS 2]=\"\") OR (([VENPMT][VENADR][CITY]=\"\")AND ([VENPMT][VENADR][STATE]=\"\") AND ([VENPMT][VENADR][ZIP]=\"\"))) THEN RETURN_VALUE=\"\" ELSE RETURN\
_VALUE=([VENPMT][VENADR][CITY]+\", \"+[VENPMT][VENADR][STATE]+\"  \"+[VENPMT][VENADR][ZIP]) FI");
	addvirtuallineADV(tmp,"VENDOR ADDRESS 3",0,0,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(32+1);
	sprintf(t,"[PAYROLL][PAYDDSV][CHECK NUMBER]");
	addvirtuallineADV(tmp,"CHECK NUMBER",8,10,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(31+1);
	sprintf(t,"([TOTAL CHECK][CHECK NUMBER])\n");
	addvirtuallineADV(tmp,"CHECK AMOUNT",2,20,t,1,0);

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

	addsortline(tmp,0,"[CHECK NUMBER]");
	addsortline(tmp,0,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER]");
	addsortline(tmp,0,"[PAYROLL][PAYDDSV][DEDUCTION DESCRIPTION IDENTIFICATION]");
	addsortline(tmp,0,"[PAYROLL][PAYDDSV][FISCAL YEAR]");
	addsortline(tmp,0,"[PAYROLL][PAYDDSV][TRANSACTION NUMBER]");
	addaccumline(tmp,"LAST CHECK NUMBER",3,"[PAYROLL][PAYDDSV][CHECK NUMBER]",0);
	addaccumline(tmp,"LAST VIRTUAL CHECK NUMBER",3,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER]",0);
	addaccumline(tmp,"TOTAL CHECK",7,"[PAYROLL][PAYDDSV][AMOUNT]",0);
	addaccumline(tmp,"LAST CHECK DATE",3,"[PAYROLL][PAYDDSV][CHECK DATE]",0);
	addaccumline(tmp,"LAST BANK IDENTIFICATION",3,"[PAYROLL][PAYDDSV][BANK IDENTIFICATION]",0);
	addaccumline(tmp,"LAST VENDOR NAME",3,"[VENPMT][VENMSR][VENDOR NAME]",0);
	addaccumline(tmp,"LAST VENDOR ADDRESS 1",3,"[VENDOR ADDRESS 1]",0);
	addaccumline(tmp,"LAST VENDOR ADDRESS 2",3,"[VENDOR ADDRESS 2]",0);
	addaccumline(tmp,"LAST VENDOR ADDRESS 3",3,"[VENDOR ADDRESS 3]",0);
	addaccumline(tmp,"LAST VENDOR CITY",3,"[VENPMT][VENADR][CITY]",0);
	addaccumline(tmp,"LAST VENDOR STATE",3,"[VENPMT][VENADR][STATE]",0);
	addaccumline(tmp,"LAST VENDOR ID",3,"[PAYROLL][PAYDDSV][VENDOR IDENTIFICATION]",0);
	addaccumline(tmp,"LAST VENDOR ZIP CODE",3,"[VENPMT][VENADR][ZIP]",0);
	addcontrolline(tmp,"[CHECK NUMBER]",0);
	addcontrolline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER]",0);




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
	addimageline(tmp,"DETAIL LINES","X@@",TRUE,"[BODY COUNT]=0",2,pval);
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
	addimageline(tmp,"DETAIL LINES","@@",TRUE,"[BODY COUNT]=0",2,pval);
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
	addimageline(tmp,"DETAIL LINES","@@",TRUE,"[BODY COUNT]=0",2,pval);
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
	addimageline(tmp,"DETAIL LINES","@@",TRUE,"[BODY COUNT]=0",2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYDDSV][VENDOR IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[VENPMT][VENMSR][VENDOR NAME]");
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
	addimageline(tmp,"DETAIL LINES","            @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLL @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL@@",TRUE,"[BODY COUNT]=0",4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(6*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYDDSV][DEDUCTION DESCRIPTION IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYDDMS][SHORT DESCRIPTION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYDDSV][DESCRIPTION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYDDSV][AMOUNT]");
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
	addimageline(tmp,"DETAIL LINES","     @LLLLLLLLLLLLLLLLLLL @LLLLLLLLLLLLLLLLLLLLLLLLL @LLLLLLLLLLLLLLLLLL @,.RRRRRRRRRR@@",FALSE,NULL,6,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"(([BODY COUNT]<[SELECTED BODY COUNT]) AND ([BODY COUNT]>0))",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"(([BODY COUNT]<([SELECTED BODY COUNT]-1)) AND ([BODY COUNT]>0))",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"(([BODY COUNT]<([SELECTED BODY COUNT]-2)) AND ([BODY COUNT]>0))",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"(([BODY COUNT]<([SELECTED BODY COUNT]-3)) AND ([BODY COUNT]>0))",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"(([BODY COUNT]<([SELECTED BODY COUNT]-4)) AND ([BODY COUNT]>0))",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"(([BODY COUNT]<([SELECTED BODY COUNT]-5)) AND ([BODY COUNT]>0))",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"(([BODY COUNT]<([SELECTED BODY COUNT]-6)) AND ([BODY COUNT]>0))",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"(([BODY COUNT]<([SELECTED BODY COUNT]-7)) AND ([BODY COUNT]>0))",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"(([BODY COUNT]<([SELECTED BODY COUNT]-8)) AND ([BODY COUNT]>0))",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"(([BODY COUNT]<([SELECTED BODY COUNT]-9)) AND ([BODY COUNT]>0))",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"(([BODY COUNT]<([SELECTED BODY COUNT]-10)) AND ([BODY COUNT]>0))",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"(([BODY COUNT]<([SELECTED BODY COUNT]-11)) AND ([BODY COUNT]>0))",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"(([BODY COUNT]<([SELECTED BODY COUNT]-12)) AND ([BODY COUNT]>0))",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"(([BODY COUNT]<([SELECTED BODY COUNT]-13)) AND ([BODY COUNT]>0))",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"(([BODY COUNT]<([SELECTED BODY COUNT]-14)) AND ([BODY COUNT]>0))",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"(([BODY COUNT]<([SELECTED BODY COUNT]-15)) AND ([BODY COUNT]>0))",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"[BODY COUNT]>0",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"[BODY COUNT]>0",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"[BODY COUNT]>0",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"[BODY COUNT]>0",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"[BODY COUNT]>0",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"[BODY COUNT]>0",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"[BODY COUNT]>0",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"[BODY COUNT]>0",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"[BODY COUNT]>0",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"[BODY COUNT]>0",2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(6*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST CHECK DATE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VIRTUAL CHECK NUMBER]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST BANK IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[CHECK AMOUNT]");
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","                                   @LLLLLLL @RRRRRRRRRR @LLLLLLLL @*,.RRRRRRRRRR@@",TRUE,"([LAST VIRTUAL CHECK NUMBER]=[LAST CHECK NUMBER]) AND ([BODY COUNT]>0)",6,pval);
	free_pvals(pval,6);


	pval=Rmalloc(5*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST CHECK DATE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VIRTUAL CHECK NUMBER]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST BANK IDENTIFICATION]");
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","                                   @LLLLLLL @RRRRRRRRRR @LLLLLLLL $********0.00 @@",TRUE,"(NOT([LAST VIRTUAL CHECK NUMBER]=[LAST CHECK NUMBER])) AND ([BODY COUNT]>0)",5,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"[BODY COUNT]>0",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"[BODY COUNT]>0",2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VENDOR NAME]");
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","                @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL@@",TRUE,"[BODY COUNT]>0",3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VENDOR ADDRESS 1]");
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","                @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL@@",TRUE,"[BODY COUNT]>0",3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VENDOR ADDRESS 2]");
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","                @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL@@",TRUE,"[BODY COUNT]>0",3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VENDOR ADDRESS 3]");
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","                @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL@@",TRUE,"[BODY COUNT]>0",3,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"[BODY COUNT]>0",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"[BODY COUNT]>0",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"[BODY COUNT]>0",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"[BODY COUNT]>0",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"[BODY COUNT]>0",2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] FOOTER","@@",TRUE,"[BODY COUNT]>0",2,pval);
	free_pvals(pval,2);






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
