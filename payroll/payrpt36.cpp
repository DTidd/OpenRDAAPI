/* PAYROLL's - Make (VWUPDBR1) Report */
#include <cstdio>
#include <rptgen.hpp>
/*COMPMEM 40960 */

extern void free_pvals(RDApval *,int);
extern char *module;
void VWUPDBR1(void);

void VWUPDBR1()
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
	sprintf(rptname,"VWUPDBR1");
	t=Rmalloc(60+1);
	sprintf(t,"Payroll Vendor Withholding Update Bank Reconciliation Report");
	desc=Rmalloc(97+1);
	sprintf(desc,"This output definition is used to print the Vendor Withholding Update Bank Reconciliation Report.");
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

	t=Rmalloc(238+1);
	sprintf(t,"([PAYROLL][PAYDDSV][SOURCE USER VERIFIED]=TRUE) AND ([PAYROLL][PAYDDSV][DELETEFLAG]=FALSE) AND \n([PAYROLL][PAYDDSV][BNKREC UPDATED]=FALSE) AND \n(([PAYROLL][PAYDDSV][CHECK NUMBER]>0) OR \n([PAYROLL][PAYD\
DSV][ELECTRONIC TRANSFER]=TRUE))");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDDSV",0,NULL,NULL,"PAYDDSV KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","");
		addsearchconfld(s,"FISCAL YEAR","");
		addsearchconfld(s,"TRANSACTION NUMBER","");
	}
	if(t!=NULL) Rfree(t);

	x=addsearchfileADV(tmp,"PAYROLL","PAYDDMS",1,"PAYROLL","PAYDDSV","PAYDDMS KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","[PAYDDSV][DEDUCTION DESCRIPTION IDENTIFICATION]");
	}

	t=Rmalloc(36+1);
	sprintf(t,"([VENPMT][VENMSR][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"VENPMT","VENMSR",2,"PAYROLL","PAYDDSV","VENDOR ID KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"VENDOR IDENTIFICATION","[PAYDDSV][VENDOR IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	x=addsearchfileADV(tmp,"VENPMT","VENADR",1,"PAYROLL","PAYDDSV","VENDOR ADDRESS KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"VENDOR IDENTIFICATION","[PAYDDSV][VENDOR IDENTIFICATION]");
		addsearchconfld(s,"ADDRESS IDENTIFICATION","[PAYDDSV][ADDRESS IDENTIFICATION]");
	}

	t=Rmalloc(36+1);
	sprintf(t,"([BNKREC][BNKACC][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"BNKREC","BNKACC",2,"PAYROLL","PAYDDSV","BNKACC KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"BANK IDENTIFICATION","[PAYDDSV][BANK IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);





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
	t=Rmalloc(32+1);
	sprintf(t,"[PAYROLL][PAYDDSV][CHECK NUMBER]");
	addvirtuallineADV(tmp,"CHECK NUMBER",8,12,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(172+1);
	sprintf(t,"IF([PAYROLL][PAYDDSV][ELECTRONIC TRANSFER]=TRUE) \nTHEN RETURN_VALUE=[PAYROLL][PAYDDSV][ELECTRONIC TRANSFER NUMBER]\nELSE RETURN_VALUE=[PAYROLL][PAYDDSV][CHECK NUMBER] \nFI");
	addvirtuallineADV(tmp,"CHECK/ELECTRONIC TRANSFER NUMBER",8,12,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(334+1);
	sprintf(t,"IF([PAYROLL][PAYDDSV][ELECTRONIC TRANSFER]=TRUE)\nTHEN IF([CHECK/ELECTRONIC TRANSFER NUMBER]=[LAST CHECK/ELECTRONIC NUMBER])  \n     THEN RETURN_VALUE=0\n     ELSE RETURN_VALUE=1 \n     FI \nELSE IF([CHECK/\
ELECTRONIC TRANSFER NUMBER]=[LAST CHECK/ELECTRONIC NUMBER]) \n     THEN RETURN_VALUE=0\n     ELSE RETURN_VALUE=1\n     FI \nFI");
	addvirtuallineADV(tmp,"CHECK/ELECTRONIC TRANSFER NUMBER COUNTER",8,12,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(189+1);
	sprintf(t,"IF([PAYROLL][PAYDDSV][ELECTRONIC TRANSFER]=TRUE) \nTHEN RETURN_VALUE=[PAYROLL][PAYDDSV][VIRTUAL ELECTRONIC TRANSFER NUMBER] \nELSE RETURN_VALUE=[PAYROLL][PAYDDSV][VIRTUAL CHECK NUMBER] \nFI");
	addvirtuallineADV(tmp,"VIRTUAL CHECK/ELECTRONIC TRANSFER NUMBER",8,12,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(47+1);
	sprintf(t,"[TOTAL CHECK][CHECK/ELECTRONIC TRANSFER NUMBER]");
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

	addsortline(tmp,0,"[PAYROLL][PAYDDSV][BANK IDENTIFICATION]");
	addsortline(tmp,0,"[PAYROLL][PAYDDSV][ELECTRONIC TRANSFER]");
	addsortline(tmp,0,"[PAYROLL][PAYDDSV][MANUAL VOUCHER]");
	addsortline(tmp,0,"[CHECK/ELECTRONIC TRANSFER NUMBER]");
	addsortline(tmp,0,"[VIRTUAL CHECK/ELECTRONIC TRANSFER NUMBER]");
	addsortline(tmp,0,"[PAYROLL][PAYDDSV][DEDUCTION DESCRIPTION IDENTIFICATION]");
	addsortline(tmp,0,"[PAYROLL][PAYDDSV][FISCAL YEAR]");
	addaccumline(tmp,"LAST BANK IDENTIFICATION",3,"[PAYROLL][PAYDDSV][BANK IDENTIFICATION]",0);
	addaccumline(tmp,"LAST PAYDDSV TRANSACTION NUMBER",3,"[PAYROLL][PAYDDSV][TRANSACTION NUMBER]",0);
	addaccumline(tmp,"LAST CHECK/ELECTRONIC NUMBER",3,"[CHECK/ELECTRONIC TRANSFER NUMBER]",0);
	addaccumline(tmp,"LAST VIRTUAL CHECK/ELECTRONIC TRANSFER NUMBER",3,"[VIRTUAL CHECK/ELECTRONIC TRANSFER NUMBER]",0);
	addaccumline(tmp,"TOTAL CHECK",7,"[PAYROLL][PAYDDSV][AMOUNT]",0);
	addaccumline(tmp,"LAST BANK NAME",3,"[BNKREC][BNKACC][BANK NAME]",0);
	addaccumline(tmp,"LAST CHECK DATE",3,"[PAYROLL][PAYDDSV][CHECK DATE]",0);
	addaccumline(tmp,"LAST ELECTRONIC TRANSFER DATE",3,"[PAYROLL][PAYDDSV][ELECTRONIC TRANSFER DATE]",0);
	addaccumline(tmp,"LAST VENDOR IDENTIFICATION",3,"[PAYROLL][PAYDDSV][VENDOR IDENTIFICATION]",0);
	addaccumline(tmp,"LAST ADDRESS IDENTIFICATION",3,"[PAYROLL][PAYDDSV][ADDRESS IDENTIFICATION]",0);
	addaccumline(tmp,"LAST VENDOR NAME",3,"[VENPMT][VENMSR][VENDOR NAME]",0);
	addaccumline(tmp,"LAST MANUAL VOUCHER",3,"[PAYROLL][PAYDDSV][MANUAL VOUCHER]",0);
	addaccumline(tmp,"LAST PAYDDSV DESCRIPTION",3,"[PAYROLL][PAYDDSV][DESCRIPTION]",0);
	addaccumline(tmp,"LAST ELECTRONIC TRANSFER",3,"[PAYROLL][PAYDDSV][ELECTRONIC TRANSFER]",0);
	addaccumline(tmp,"TOTAL NUMBER CHECK/ELECTRONIC TRANSFER",7,"[CHECK/ELECTRONIC TRANSFER NUMBER COUNTER]",0);
	addcontrolline(tmp,"[PAYROLL][PAYDDSV][BANK IDENTIFICATION]",0);
	addcontrolline(tmp,"[PAYROLL][PAYDDSV][ELECTRONIC TRANSFER]",0);
	addcontrolline(tmp,"[PAYROLL][PAYDDSV][MANUAL VOUCHER]",0);
	addcontrolline(tmp,"[CHECK/ELECTRONIC TRANSFER NUMBER]",0);
	addcontrolline(tmp,"[VIRTUAL CHECK/ELECTRONIC TRANSFER NUMBER]",0);




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

	addimageline(tmp,"REPORT HEADER","OPEN [BNKREC][BNKACC]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT HEADER","OPEN [BNKREC][BNKCHK]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT HEADER","OPEN [BNKREC][BNKETR]",FALSE,NULL,0,pval);


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
	pvals->fieldname=stralloc("[PAYROLL][PAYDDSV][BNKREC UPDATED]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LOGIN IDENTIFICATION]");
	pvals->fieldname=stralloc("[PAYROLL][PAYDDSV][BNKREC UPDATED BY]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM DATE]");
	pvals->fieldname=stralloc("[PAYROLL][PAYDDSV][BNKREC UPDATED DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM TIME]");
	pvals->fieldname=stralloc("[PAYROLL][PAYDDSV][BNKREC UPDATED TIME]");
	addimageline(tmp,"DETAIL LINES","WRITE [PAYROLL][PAYDDSV]",FALSE,NULL,7,pval);
	free_pvals(pval,7);


	pval=Rmalloc(11*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST BANK IDENTIFICATION]");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][BANK IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VIRTUAL CHECK/ELECTRONIC TRANSFER NUMBER]");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][CHECK NUMBER]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("([CHECK AMOUNT]+[BNKREC][BNKCHK][CHECK AMOUNT])");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][CHECK AMOUNT]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST CHECK DATE]");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][CHECK DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM DATE]");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][ENTRY DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM TIME]");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][ENTRY TIME]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VENDOR IDENTIFICATION]");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][REFERENCE ID]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VENDOR NAME]");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][REFERENCE NAME]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("\"PAYROLL\"");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][SOURCE MODULE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("\"VWUPDBR\"");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][SOURCE PROCESS]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LOGIN IDENTIFICATION]");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][SOURCE USER]");
	addimageline(tmp,"[VIRTUAL CHECK/ELECTRONIC TRANSFER NUMBER] FOOTER","WRITE [BNKREC][BNKCHK]",TRUE,"([LAST VIRTUAL CHECK/ELECTRONIC TRANSFER NUMBER]=[LAST CHECK/ELECTRONIC NUMBER]) AND ([LAST MANUAL VOUCHER]=FALSE) AND ([LAST ELECTRONIC TRANSFER]=FALSE)",11,pval);
	free_pvals(pval,11);


	pval=Rmalloc(12*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST BANK IDENTIFICATION]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][BANK IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("NEXT TRANSACTION NO");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][TRANSACTION NUMBER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST CHECK/ELECTRONIC NUMBER]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][REFERENCE NUMBER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[CHECK AMOUNT]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][TRANSACTION AMOUNT]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST ELECTRONIC TRANSFER DATE]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][TRANSACTION DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM DATE]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][ENTRY DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM TIME]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][ENTRY TIME]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VENDOR IDENTIFICATION]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][REFERENCE ID]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VENDOR NAME]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][REFERENCE NAME]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("\"PAYROLL\"");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][SOURCE MODULE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("\"VWUPDBR\"");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][SOURCE PROCESS]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LOGIN IDENTIFICATION]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][SOURCE USER]");
	addimageline(tmp,"[VIRTUAL CHECK/ELECTRONIC TRANSFER NUMBER] FOOTER","WRITE [BNKREC][BNKETR]",TRUE,"([LAST VIRTUAL CHECK/ELECTRONIC TRANSFER NUMBER]=[LAST CHECK/ELECTRONIC NUMBER]) AND ([LAST MANUAL VOUCHER]=FALSE) AND ([LAST ELECTRONIC TRANSFER]=TRUE)",12,pval);
	free_pvals(pval,12);


	pval=Rmalloc(11*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST BANK IDENTIFICATION]");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][BANK IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VIRTUAL CHECK/ELECTRONIC TRANSFER NUMBER]");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][CHECK NUMBER]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("0");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][CHECK AMOUNT]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST CHECK DATE]");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][CHECK DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM DATE]");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][ENTRY DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM TIME]");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][ENTRY TIME]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VENDOR IDENTIFICATION]");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][REFERENCE ID]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VENDOR NAME]");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][REFERENCE NAME]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("\"PAYROLL\"");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][SOURCE MODULE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("\"VWUPDBR\"");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][SOURCE PROCESS]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LOGIN IDENTIFICATION]");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][SOURCE USER]");
	addimageline(tmp,"[VIRTUAL CHECK/ELECTRONIC TRANSFER NUMBER] FOOTER","WRITE [BNKREC][BNKCHK]",TRUE,"(NOT([LAST VIRTUAL CHECK/ELECTRONIC TRANSFER NUMBER]=[LAST CHECK/ELECTRONIC NUMBER])) AND ([LAST MANUAL VOUCHER]=FALSE) AND ([LAST ELECTRONIC TRANSFER]=FALSE)",11,pval);
	free_pvals(pval,11);


	pval=Rmalloc(12*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST BANK IDENTIFICATION]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][BANK IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("NEXT TRANSACTION NO");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][TRANSACTION NUMBER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VIRTUAL CHECK/ELECTRONIC TRANSFER NUMBER]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][REFERENCE NUMBER]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("0");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][TRANSACTION AMOUNT]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST ELECTRONIC TRANSFER DATE]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][TRANSACTION DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM DATE]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][ENTRY DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM TIME]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][ENTRY TIME]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VENDOR IDENTIFICATION]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][REFERENCE ID]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VENDOR NAME]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][REFERENCE NAME]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("\"PAYROLL\"");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][SOURCE MODULE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("\"VWUPDBR\"");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][SOURCE PROCESS]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LOGIN IDENTIFICATION]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][SOURCE USER]");
	addimageline(tmp,"[VIRTUAL CHECK/ELECTRONIC TRANSFER NUMBER] FOOTER","WRITE [BNKREC][BNKETR]",TRUE,"(NOT([LAST VIRTUAL CHECK/ELECTRONIC TRANSFER NUMBER]=[LAST CHECK/ELECTRONIC NUMBER])) AND ([LAST MANUAL VOUCHER]=FALSE) AND ([LAST ELECTRONIC TRANSFER]=TRUE)",12,pval);
	free_pvals(pval,12);


	pval=Rmalloc(11*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST BANK IDENTIFICATION]");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][BANK IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VIRTUAL CHECK/ELECTRONIC TRANSFER NUMBER]");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][CHECK NUMBER]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("([BNKREC][BNKCHK][CHECK AMOUNT]+[CHECK AMOUNT])");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][CHECK AMOUNT]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST CHECK DATE]");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][CHECK DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM DATE]");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][ENTRY DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM TIME]");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][ENTRY TIME]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VENDOR IDENTIFICATION]");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][REFERENCE ID]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VENDOR NAME]");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][REFERENCE NAME]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("\"PAYROLL\"");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][SOURCE MODULE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("\"VWUPDBR\"");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][SOURCE PROCESS]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LOGIN IDENTIFICATION]");
	pvals->fieldname=stralloc("[BNKREC][BNKCHK][SOURCE USER]");
	addimageline(tmp,"[VIRTUAL CHECK/ELECTRONIC TRANSFER NUMBER] FOOTER","WRITE [BNKREC][BNKCHK]",TRUE,"([LAST MANUAL VOUCHER]=TRUE) AND ([BODY COUNT]>0) AND \n([LAST ELECTRONIC TRANSFER]=FALSE)",11,pval);
	free_pvals(pval,11);


	pval=Rmalloc(12*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST BANK IDENTIFICATION]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][BANK IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("NEXT TRANSACTION NO");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][TRANSACTION NUMBER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VIRTUAL CHECK/ELECTRONIC TRANSFER NUMBER]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][REFERENCE NUMBER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[CHECK AMOUNT]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][TRANSACTION AMOUNT]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST ELECTRONIC TRANSFER DATE]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][TRANSACTION DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM DATE]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][ENTRY DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM TIME]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][ENTRY TIME]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VENDOR IDENTIFICATION]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][REFERENCE ID]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VENDOR NAME]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][REFERENCE NAME]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("\"PAYROLL\"");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][SOURCE MODULE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("\"VWUPDBR\"");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][SOURCE PROCESS]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LOGIN IDENTIFICATION]");
	pvals->fieldname=stralloc("[BNKREC][BNKETR][SOURCE USER]");
	addimageline(tmp,"[VIRTUAL CHECK/ELECTRONIC TRANSFER NUMBER] FOOTER","WRITE [BNKREC][BNKETR]",TRUE,"([LAST MANUAL VOUCHER]=TRUE) AND ([BODY COUNT]>0) AND \n([LAST ELECTRONIC TRANSFER]=TRUE)",12,pval);
	free_pvals(pval,12);


	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST BANK IDENTIFICATION]");
	pvals->fieldname=stralloc("[BNKREC][BNKACC][BANK IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("([BNKREC][BNKACC][UNPROCESSED BALANCE]-[TOTAL CHECK])");
	pvals->fieldname=stralloc("[BNKREC][BNKACC][UNPROCESSED BALANCE]");
	addimageline(tmp,"[PAYROLL][PAYDDSV][BANK IDENTIFICATION] FOOTER","WRITE [BNKREC][BNKACC",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	addimageline(tmp,"REPORT FOOTER","CLOSE [PAYROLL][PAYDDAC]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT FOOTER","CLOSE [BNKREC][BNKETR]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT FOOTER","CLOSE [BNKREC][BNKCHK]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT FOOTER","CLOSE [BNKREC][BNKACC]",FALSE,NULL,0,pval);






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
