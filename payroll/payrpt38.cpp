/* PAYROLL's - Make (VWUPDBR1-ET) Report */
#include <cstdio>
#include <rptgen.hpp>
/*COMPMEM 40960 */

extern void free_pvals(RDApval *,int);
extern char *module;
void VWUPDBR1_ET(void);

void VWUPDBR1_ET()
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
	rptname=Rmalloc(11+1);
	sprintf(rptname,"VWUPDBR1-ET");
	t=Rmalloc(47+1);
	sprintf(t,"Update Vendor Withholding Bank Reconciliation 1");
	desc=Rmalloc(133+1);
	sprintf(desc,"This output definition is used to update the data files in bank reconciliation from the payroll vendor withholding check information.");
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
	adddisplayADV(tmp,(double)8.500000,(double)10.500000,4,FALSE,0,"",10,1,0,pvars);

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

	t=Rmalloc(178+1);
	sprintf(t,"([PAYROLL][PAYDDSV][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYDDSV][SOURCE USER VERIFIED]=TRUE) AND ([PAYROLL][PAYDDSV][BNKREC UPDATED]=FALSE) AND ([PAYROLL][PAYDDSV][CHECK NUMBER]=0)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDDSV",0,NULL,NULL,"PAYDDSV KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","");
		addsearchconfld(s,"FISCAL YEAR","");
		addsearchconfld(s,"TRANSACTION NUMBER","");
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

	x=addsearchfileADV(tmp,"VENPMT","VENADR",1,"PAYROLL","PAYDDSV","VENDOR ADDRESS KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"VENDOR IDENTIFICATION","[PAYDDSV][VENDOR IDENTIFICATION]");
		addsearchconfld(s,"ADDRESS IDENTIFICATION","[PAYDDSV][ADDRESS IDENTIFICATION]");
	}

	t=Rmalloc(36+1);
	sprintf(t,"([BNKREC][BNKACC][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"BNKREC","BNKACC",2,"PAYROLL","PAYDDSV","BNKACC KEY",FALSE,t,NULL,1);
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
	sprintf(t,"(\"Executed by: \"+[LOGIN IDENTIFICATION])");
	addvirtuallineADV(tmp,"EXECUTED BY",0,0,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(32+1);
	sprintf(t,"[PAYROLL][PAYDDSV][CHECK NUMBER]");
	addvirtuallineADV(tmp,"CHECK NUMBER",8,12,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(27+1);
	sprintf(t,"[TOTAL CHECK][CHECK NUMBER]");
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
	addsortline(tmp,0,"[PAYROLL][PAYDDSV][VENDOR IDENTIFICATION]");
	addsortline(tmp,0,"[PAYROLL][PAYDDSV][DEDUCTION DESCRIPTION IDENTIFICATION]");
	addsortline(tmp,0,"[PAYROLL][PAYDDSV][FISCAL YEAR]");
	addsortline(tmp,0,"[PAYROLL][PAYDDSV][TRANSACTION NUMBER]");
	addaccumline(tmp,"LAST BANK IDENTIFICATION",3,"[PAYROLL][PAYDDSV][BANK IDENTIFICATION]",0);
	addaccumline(tmp,"TOTAL CHECK",7,"[PAYROLL][PAYDDSV][AMOUNT]",0);
	addaccumline(tmp,"LAST BANK NAME",3,"[BNKREC][BNKACC][BANK NAME]",0);
	addaccumline(tmp,"LAST VENDOR IDENTIFICATION",3,"[PAYROLL][PAYDDSV][VENDOR IDENTIFICATION]",0);
	addaccumline(tmp,"LAST ADDRESS IDENTIFICATION",3,"[PAYROLL][PAYDDSV][ADDRESS IDENTIFICATION]",0);
	addaccumline(tmp,"LAST VENDOR NAME",3,"[VENPMT][VENMSR][VENDOR NAME]",0);
	addcontrolline(tmp,"[PAYROLL][PAYDDSV][BANK IDENTIFICATION]",0);
	addcontrolline(tmp,"[PAYROLL][PAYDDSV][VENDOR IDENTIFICATION]",0);
	addcontrolline(tmp,"[PAYROLL][PAYDDSV][DEDUCTION DESCRIPTION IDENTIFICATION]",0);




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


	addimageline(tmp,"REPORT HEADER","OPEN [BNKREC][BNKDEP]",FALSE,NULL,0,pval);


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
	pvals->fieldname=stralloc("[BNKREC][BNKDEP][BANK IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("NEXT TRANSACTION NO");
	pvals->fieldname=stralloc("[BNKREC][BNKDEP][DEPOSIT NUMBER]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("([TOTAL CHECK]*(-1))");
	pvals->fieldname=stralloc("[BNKREC][BNKDEP][DEPOSIT AMOUNT]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYDDSV][ENTRY DATE]");
	pvals->fieldname=stralloc("[BNKREC][BNKDEP][BANK IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM DATE]");
	pvals->fieldname=stralloc("[BNKREC][BNKDEP][ENTRY DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM TIME]");
	pvals->fieldname=stralloc("[BNKREC][BNKDEP][ENTRY TIME]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VENDOR IDENTIFICATION]");
	pvals->fieldname=stralloc("[BNKREC][BNKDEP][REFERENCE ID]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VENDOR NAME]");
	pvals->fieldname=stralloc("[BNKREC][BNKDEP][REFERENCE NAME]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("\"PAYROLL\"");
	pvals->fieldname=stralloc("[BNKREC][BNKDEP][SOURCE MODULE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("\"VWUPDBR-ET\"");
	pvals->fieldname=stralloc("[BNKREC][BNKDEP][SOURCE PROCESS]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LOGIN IDENTIFICATION]");
	pvals->fieldname=stralloc("[BNKREC][BNKDEP][SOURCE USER]");
	addimageline(tmp,"[PAYROLL][PAYDDSV][VENDOR IDENTIFICATION] FOOTER","WRITE [BNKREC][BNKDEP]",FALSE,NULL,11,pval);
	free_pvals(pval,11);


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
	addimageline(tmp,"[PAYROLL][PAYDDSV][BANK IDENTIFICATION] FOOTER","WRITE [BNKREC][BNKACC]",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	addimageline(tmp,"REPORT FOOTER","CLOSE [PAYROLL][PAYDDSV]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT FOOTER","CLOSE [BNKREC][BNKACC]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT FOOTER","CLOSE [BNKREC][BNKDEP]",FALSE,NULL,0,pval);






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
