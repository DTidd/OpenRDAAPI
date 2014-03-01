/* PAYROLL's - Make (PAYCKREG) Report */
#include <cstdio>
#include <rptgen.hpp>
/*COMPMEM 40960 */

extern void free_pvals(RDApval *,int);
extern char *module;
void PAYCKREG(void);

void PAYCKREG()
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
	sprintf(rptname,"PAYCKREG");
	t=Rmalloc(22+1);
	sprintf(t,"Payroll Check Register");
	desc=Rmalloc(109+1);
	sprintf(desc,"This output definition is used to print the Payroll Check Register report from the payroll check information.");
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
	adddisplayADV(tmp,(double)8.500000,(double)10.500000,0,FALSE,0,"",0,1,0,pvars);

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

	t=Rmalloc(113+1);
	sprintf(t,"([PAYROLL][PAYJDPM][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYJDPM][PAY IDENTIFICATION]=[SELECTED PAY IDENTIFICATION])");
	x=addsearchfileADV(tmp,"PAYROLL","PAYJDPM",0,NULL,NULL,"RDA PAYJDPM KEY",FALSE,t,NULL,1);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(36+1);
	sprintf(t,"([PRSNNL][PERDMG][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"PRSNNL","PERDMG",2,"PAYROLL","PAYJDPM","PERDMG KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJDPM][PERSONNEL IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(79+1);
	sprintf(t,"([PAYROLL][PAYJPMS][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYJPMS][CHECK NUMBER]>0)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYJPMS",2,"PAYROLL","PAYJDPM","PAYJPMS KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJDPM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYJDPM][JOB NUMBER]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJDPM][PAY IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(87+1);
	sprintf(t,"([PAYROLL][PAYPPMS][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYPPMS][BENEFITS COMPUTED]=TRUE)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYPPMS",2,"PAYROLL","PAYJDPM","PAYPPMS KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJDPM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJDPM][PAY IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(76+1);
	sprintf(t,"([PAYROLL][PAYJMST][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYJMST][ACTIVE]=TRUE)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYJMST",2,"PAYROLL","PAYJDPM","JOBMSTR KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJDPM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYJDPM][JOB NUMBER]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(37+1);
	sprintf(t,"([POSTRK][POSMSTR][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"POSTRK","POSMSTR",1,"PAYROLL","PAYJDPM","POSMSTR",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"POSITION IDENTIFICATION","[POSITION IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(80+1);
	sprintf(t,"([PAYROLL][PAYDDMS][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYDDMS][REDUCE NET]=TRUE)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDDMS",2,"PAYROLL","PAYJDPM","PAYDDMS KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","[PAYJDPM][DEDUCTION DESCRIPTION IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(36+1);
	sprintf(t,"([BNKREC][BNKACC][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"BNKREC","BNKACC",2,"PAYROLL","PAYJDPM","BNKACC KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"BANK IDENTIFICATION","[BANK IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(113+1);
	sprintf(t,"([PAYROLL][PAYJPRM][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYJPRM][PAY IDENTIFICATION]=[SELECTED PAY IDENTIFICATION])");
	x=addsearchfileADV(tmp,"PAYROLL","PAYJPRM",0,NULL,NULL,"PAYJPRM KEY",FALSE,t,NULL,1);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(36+1);
	sprintf(t,"([PRSNNL][PERDMG][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"PRSNNL","PERDMG",2,"PAYROLL","PAYJPRM","PERDMG KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJPRM][PERSONNEL IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(79+1);
	sprintf(t,"([PAYROLL][PAYJPMS][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYJPMS][CHECK NUMBER]>0)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYJPMS",2,"PAYROLL","PAYJPRM","PAYJPMS KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJPRM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYJPRM][JOB NUMBER]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJPRM][PAY IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(87+1);
	sprintf(t,"([PAYROLL][PAYPPMS][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYPPMS][BENEFITS COMPUTED]=TRUE)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYPPMS",2,"PAYROLL","PAYJPRM","PAYPPMS KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJPRM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJPRM][PAY IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(77+1);
	sprintf(t,"([PAYROLL][PAYJMST][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYJMST][ACTIVE]=TRUE))");
	x=addsearchfileADV(tmp,"PAYROLL","PAYJMST",2,"PAYROLL","PAYJPRM","JOBMSTR KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJPRM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYJPRM][JOB NUMBER]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(37+1);
	sprintf(t,"([POSTRK][POSMSTR][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"POSTRK","POSMSTR",1,"PAYROLL","PAYJPRM","POSMSTR",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"POSITION IDENTIFICATION","[POSITION IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(36+1);
	sprintf(t,"([BNKREC][BNKACC][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"BNKREC","BNKACC",2,"PAYROLL","PAYJPRM","BNKACC KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"BANK IDENTIFICATION","[BANK IDENTIFICATION]");
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
	addvirtuallineADV(tmp,"SELECTED PAY IDENTIFICATION",1,15,t,0,2);

	if(t!=NULL) Rfree(t);
	addvirtuallineADV(tmp,"SELECTED BANK IDENTIFICATION",1,15,t,0,2);

	addvirtuallineADV(tmp,"BANK IDENTIFICATION",1,30,"[PAYROLL][PAYJPMS][BANK IDENTIFICATION]",0,0);

	addvirtuallineADV(tmp,"POSITION IDENTIFICATION",1,30,"[PAYROLL][PAYJMST][POSITION IDENTIFICATION]",0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(126+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) THEN RETURN_VALUE=1 ELSE IF([PRIMARY FILE NUMBER]=2) THEN RETURN_VALUE=0 ELSE RETURN_VALUE=2 FI FI");
	addvirtuallineADV(tmp,"WHICH TYPE",7,2,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(230+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) THEN IF(([PAYROLL][PAYDDMS][REDUCE NET]=TRUE) AND (!([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"DIRECT DEPOSIT\"))) THEN RETURN_VALUE=[PAYROLL][PAYJDPM][AMOUNT] ELSE RETURN_VALUE=0 \
FI ELSE RETURN_VALUE=0 FI");
	addvirtuallineADV(tmp,"REDUCE NET AMOUNT",2,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(94+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=2) THEN RETURN_VALUE=[PAYROLL][PAYJPRM][GROSS] ELSE RETURN_VALUE=0 FI");
	addvirtuallineADV(tmp,"GROSS AMOUNT",2,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(42+1);
	sprintf(t,"(\"Executed by: \"+[LOGIN IDENTIFICATION])");
	addvirtuallineADV(tmp,"EXECUTED BY",0,0,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(48+1);
	sprintf(t,"([TOTAL GROSS AMOUNT]-[TOTAL REDUCE NET AMOUNT])");
	addvirtuallineADV(tmp,"NET AMOUNT",2,15,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(28+1);
	sprintf(t,"(\"PAYROLL CHECK REGISTER\")");
	addvirtuallineADV(tmp,"REPORT TITLE",0,0,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(132+1);
	sprintf(t,"([PRSNNL][PERDMG][FIRST NAME]+\" \"+[PRSNNL][PERDMG][MIDDLE NAME]+\" \"+[PRSNNL][PERDMG][LAST NAME]+\" \"+[PRSNNL][PERDMG][LINEAGE])");
	addvirtuallineADV(tmp,"NAME FIRST MIDDLE LAST LINEAGE",1,40,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(32+1);
	sprintf(t,"[PAYROLL][PAYJPMS][CHECK NUMBER]");
	addvirtuallineADV(tmp,"CHECK NUMBER",8,12,t,0,2);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(30+1);
	sprintf(t,"[PAYROLL][PAYJPMS][CHECK DATE]");
	addvirtuallineADV(tmp,"CHECK DATE",1,8,t,0,2);

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

	addsortline(tmp,0,"[PAYROLL][PAYJPMS][BANK IDENTIFICATION]");
	addsortline(tmp,0,"[PAYROLL][PAYJPMS][CHECK DATE]");
	addsortline(tmp,0,"[PAYROLL][PAYJPMS][CHECK NUMBER]");
	addsortline(tmp,0,"[PAYROLL][PAYJPRM][PERSONNEL IDENTIFICATION]");
	addsortline(tmp,0,"[WHICH TYPE]");
	addaccumline(tmp,"TOTAL REDUCE NET AMOUNT",7,"[REDUCE NET AMOUNT]",0);
	addaccumline(tmp,"TOTAL GROSS AMOUNT",7,"[GROSS AMOUNT]",0);
	addaccumline(tmp,"LAST PERSONNEL IDENTIFICATION",3,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION]",0);
	addaccumline(tmp,"LAST WHICH TYPE",3,"[WHICH TYPE]",0);
	addaccumline(tmp,"LAST BANK IDENTIFICATION",3,"[PAYROLL][PAYJPMS][BANK IDENTIFICATION]",0);
	addaccumline(tmp,"LAST CHECK NUMBER",3,"[PAYROLL][PAYJPMS][CHECK NUMBER]",0);
	addaccumline(tmp,"LAST BANK NAME",3,"[BNKREC][BNKACC][BANK NAME]",0);
	addaccumline(tmp,"LAST PERSONNEL NAME",3,"[NAME FIRST MIDDLE LAST LINEAGE]",0);
	addaccumline(tmp,"LAST CHECK DATE",3,"[PAYROLL][PAYJPMS][CHECK DATE]",0);
	addaccumline(tmp,"LAST SSN",3,"[PRSNNL][PERDMG][SOCIAL SECURITY NUMBER]",0);
	addcontrolline(tmp,"[PAYROLL][PAYJPMS][BANK IDENTIFICATION]",0);
	addcontrolline(tmp,"[PAYROLL][PAYJPMS][CHECK DATE]",0);
	addcontrolline(tmp,"[PAYROLL][PAYJPMS][CHECK NUMBER]",0);
	addcontrolline(tmp,"[PAYROLL][PAYJPRM][PERSONNEL IDENTIFICATION]",0);




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
	addimageline(tmp,"PAGE HEADER","             @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC         Page: @RRRRRRR@@",FALSE,NULL,4,pval);
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
	addimageline(tmp,"PAGE HEADER","             @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC         Date: @LLLLLLL@@",FALSE,NULL,4,pval);
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
	addimageline(tmp,"PAGE HEADER","             @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC         Time: @LLLLLLL@@",FALSE,NULL,4,pval);
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
	addimageline(tmp,"PAGE HEADER","Bank@@",FALSE,NULL,2,pval);
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
	addimageline(tmp,"PAGE HEADER","   Check Num      Personnel ID  Name                                                Net Pay @@",FALSE,NULL,2,pval);
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
	addimageline(tmp,"PAGE HEADER","   -------------  ------------  -------------------------------------------  ---------------@@",FALSE,NULL,2,pval);
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
	pvals->name=stralloc("[BNKREC][BNKACC][BANK IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[BNKREC][BNKACC][BANK NAME]");
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
	addimageline(tmp,"[PAYROLL][PAYJPMS][BANK IDENTIFICATION] HEADER","@LLLLLLLLLLLLLL @LLLLLLLLLLLLLLLLLLLLL@@",FALSE,NULL,4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(6*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST CHECK NUMBER]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST PERSONNEL IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST PERSONNEL NAME]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[NET AMOUNT]");
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
	addimageline(tmp,"[PAYROLL][PAYJPMS][CHECK NUMBER] FOOTER","   @RRRRRRRRRRRR  @LLLLLLLLLLL  @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL  @,.RRRRRRRRRRRR@@",FALSE,NULL,6,pval);
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
	addimageline(tmp,"[PAYROLL][PAYJPMS][BANK IDENTIFICATION] FOOTER","                                                                             ---------------@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST BANK IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[NET AMOUNT]");
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
	addimageline(tmp,"[PAYROLL][PAYJPMS][BANK IDENTIFICATION] FOOTER","                                               Total Bank ( @LLLLLLLLLLLL )  @,.RRRRRRRRRRRR@@",FALSE,NULL,4,pval);
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
	addimageline(tmp,"REPORT FOOTER","                                                                             ===============@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[NET AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("CARRIAGE RETURN");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("FORMFEED");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"REPORT FOOTER","                                                                      Total  @,.RRRRRRRRRRRR@@@",FALSE,NULL,4,pval);
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
	pvals->name=stralloc("FORMFEED");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("LINEFEED");
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
