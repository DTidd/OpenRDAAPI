/* PAYROLL's - Make (DIRDEPRPT) Report */
#include <cstdio>
#include <rptgen.hpp>
/*COMPMEM 40960 */

extern void free_pvals(RDApval *,int);
extern char *module;
void DIRDEPRPT(void);

void DIRDEPRPT()
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
	rptname=Rmalloc(9+1);
	sprintf(rptname,"DIRDEPRPT");
	t=Rmalloc(21+1);
	sprintf(t,"Direct Deposit Report");
	desc=Rmalloc(80+1);
	sprintf(desc,"This output definition is used to print the Direct Deposit Report for the Banks.");
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
	adddisplayADV(tmp,(double)8.500000,(double)10.000000,0,FALSE,1,"",0,1,0,pvars);

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
	sprintf(t,"([PAYROLL][PAYDPMS][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYDPMS][PAY IDENTIFICATION]=[SELECTED PAY IDENTIFICATION])");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDPMS",0,NULL,NULL,"RDA PAYDPMS KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PAY IDENTIFICATION","[SELECTED PAY IDENTIFICATION]");
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","");
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(76+1);
	sprintf(t,"([PAYROLL][PAYDEDM][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYDEDM][ACTIVE]=TRUE)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDEDM",1,"PAYROLL","PAYDPMS","PAYDEDM KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYDPMS][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","[PAYDPMS][DEDUCTION DESCRIPTION IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(136+1);
	sprintf(t,"([PAYROLL][PAYDDMS][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYDDMS][ACTIVE]=TRUE) AND ([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"DIRECT DEPOSIT\")");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDDMS",1,"PAYROLL","PAYDPMS","PAYDDMS KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","[PAYDPMS][DEDUCTION DESCRIPTION IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(87+1);
	sprintf(t,"([PAYROLL][PAYPPMS][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYPPMS][BENEFITS COMPUTED]=TRUE)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYPPMS",1,"PAYROLL","PAYDPMS","PAYPPMS KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYDPMS][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYDPMS][PAY IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	x=addsearchfileADV(tmp,"PRSNNL","PERDMG",1,"PAYROLL","PAYDPMS","PERDMG KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYDPMS][PERSONNEL IDENTIFICATION]");
	}

	x=addsearchfileADV(tmp,"VENPMT","VENMSR",1,"PAYROLL","PAYDPMS","VENDOR ID KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"VENDOR IDENTIFICATION","[VENDOR IDENTIFICATION]");
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
	addvirtuallineADV(tmp,"SELECTED PAY IDENTIFICATION",0,15,t,0,2);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(25+1);
	sprintf(t,"\"DIRECT DEPOSIT REPORT\"");
	addvirtuallineADV(tmp,"REPORT TITLE",0,0,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(41+1);
	sprintf(t,"[PAYROLL][PAYDDMS][VENDOR IDENTIFICATION]");
	addvirtuallineADV(tmp,"VENDOR IDENTIFICATION",1,30,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(42+1);
	sprintf(t,"(\"Executed By: \"+[LOGIN IDENTIFICATION])");
	addvirtuallineADV(tmp,"EXECUTED BY",0,0,t,1,0);

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

	addsortline(tmp,0,"[PAYROLL][PAYDDMS][VENDOR IDENTIFICATION]");
	addsortline(tmp,0,"[PAYROLL][PAYDPMS][DEDUCTION DESCRIPTION IDENTIFICATION]");
	addsortline(tmp,0,"[PAYROLL][PAYDPMS][PERSONNEL IDENTIFICATION]");
	addaccumline(tmp,"LAST VENDOR IDENTIFICATION",3,"[PAYROLL][PAYDDMS][VENDOR IDENTIFICATION]",0);
	addaccumline(tmp,"LAST DEDUCTION DESCRIPTION IDENTIFICATION",3,"[PAYROLL][PAYDPMS][DEDUCTION DESCRIPTION IDENTIFICATION]",0);
	addaccumline(tmp,"LAST PERSONNEL IDENTIFICATION",3,"[PAYROLL][PAYDPMS][PERSONNEL IDENTIFICATION]",0);
	addaccumline(tmp,"TOTAL AMOUNT",7,"[PAYROLL][PAYDPMS][AMOUNT]",0);
	addcontrolline(tmp,"[PAYROLL][PAYDDMS][VENDOR IDENTIFICATION]",0);
	addcontrolline(tmp,"[PAYROLL][PAYDPMS][DEDUCTION DESCRIPTION IDENTIFICATION]",0);
	addcontrolline(tmp,"[PAYROLL][PAYDPMS][PERSONNEL IDENTIFICATION]",0);




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
	addimageline(tmp,"PAGE HEADER","                     @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC        PAGE:@RRRRRRR@@",FALSE,NULL,4,pval);
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
	addimageline(tmp,"PAGE HEADER","                     @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC        DATE:@RRRRRRR@@",FALSE,NULL,4,pval);
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
	addimageline(tmp,"PAGE HEADER","                     @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC        TIME:@RRRRRRR@@",FALSE,NULL,4,pval);
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
	pvals->name=stralloc("[PAYROLL][PAYDDMS][VENDOR IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[VENDOR IDENTIFICATION]");
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
	addimageline(tmp,"[PAYROLL][PAYDDMS][VENDOR IDENTIFICATION] HEADER","@VVVVVVVVVVVVV @VVVVVVVVVVVVVVVVV@@",FALSE,NULL,4,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDMS][VENDOR IDENTIFICATION] HEADER","@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(5*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYDPMS][PERSONNEL IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PRSNNL][PERDMG][NAME]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYDPMS][AMOUNT]");
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
	addimageline(tmp,"DETAIL LINES","     @LLLLLLLLLLLLLLLLL @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL @,.RRRRRRRRRRRR@@",FALSE,NULL,5,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDMS][VENDOR IDENTIFICATION] FOOTER","                                                           ---------------@@",FALSE,NULL,2,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDMS][VENDOR IDENTIFICATION] FOOTER","                                                           @,.RRRRRRRRRRRR@@",FALSE,NULL,3,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDMS][VENDOR IDENTIFICATION] FOOTER","@@@",FALSE,NULL,3,pval);
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
