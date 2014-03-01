/* PAYROLL's - Make (PAYSELA1) Report */
#include <cstdio>
#include <rptgen.hpp>
/*COMPMEM 40960 */

extern void free_pvals(RDApval *,int);
extern char *module;
void PAYSELA1(void);

void PAYSELA1()
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
	sprintf(rptname,"PAYSELA1");
	t=Rmalloc(33+1);
	sprintf(t,"Select Payroll Audit Trail Part 2");
	desc=Rmalloc(130+1);
	sprintf(desc,"This output definition is used to update the database noting the user has verified select payroll records through the audit trail.");
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

	t=Rmalloc(99+1);
	sprintf(t,"([PAYROLL][PAYJPRM][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYJPRM][SOURCE USER]=[LOGIN IDENTIFICATION])");
	x=addsearchfileADV(tmp,"PAYROLL","PAYJPRM",0,NULL,NULL,"PAYJPRM KEY",FALSE,t,NULL,1);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(152+1);
	sprintf(t,"([PAYROLL][PAYJPMS][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYJPMS][SOURCE USER VERIFIED]=FALSE) AND ([PAYROLL][PAYJPMS][SOURCE USER]=[LOGIN IDENTIFICATION])");
	x=addsearchfileADV(tmp,"PAYROLL","PAYJPMS",1,"PAYROLL","PAYJPRM","PAYJPMS KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJPRM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYJPRM][JOB NUMBER]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJPRM][PAY IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(138+1);
	sprintf(t,"([PAYROLL][PAYPPMS][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYPPMS][BENEFITS COMPUTED]=FALSE) AND ([PAYROLL][PAYPPMS][MADE TRANSACTIONS]=FALSE)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYPPMS",1,"PAYROLL","PAYJPRM","PAYPPMS KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJPRM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJPRM][PAY IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	x=addsearchfileADV(tmp,"PRSNNL","PERDMG",1,"PAYROLL","PAYJPRM","PERDMG KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJPRM][PERSONNEL IDENTIFICATION]");
	}

	x=addsearchfileADV(tmp,"PAYROLL","PAYJMST",1,"PAYROLL","PAYJPRM","JOBMSTR KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJPRM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYJPRM][JOB NUMBER]");
	}

	x=addsearchfileADV(tmp,"PAYROLL","PAYMSTR",1,"PAYROLL","PAYJPRM","PAYMSTR KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJPRM][PAY IDENTIFICATION]");
	}

	x=addsearchfileADV(tmp,"POSTRK","POSRATE",1,"PAYROLL","PAYJPRM","POSRATE KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"RATE IDENTIFICATION","[PAYJPRM][RATE IDENTIFICATION]");
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
	t=Rmalloc(309+1);
	sprintf(t,"IF([PRSNNL][PERDMG][LINEAGE]!=\"\") THEN RETURN_VALUE=([PRSNNL][PERDMG][LAST NAME]+\" \"+[PRSNNL][PERDMG][LINEAGE]+\", \"+[PRSNNL][PERDMG][FIRST NAME]+\" \"+[PRSNNL][PERDMG][MIDDLE NAME]) ELSE RETURN_VALUE=([P\
RSNNL][PERDMG][LAST NAME]+\", \"+[PRSNNL][PERDMG][FIRST NAME]+\" \"+[PRSNNL][PERDMG][MIDDLE NAME])");
	addvirtuallineADV(tmp,"NAME LAST LINEAGE FIRST MIDDLE",1,40,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(713+1);
	sprintf(t,"IF([POSTRK][POSRATE][RATE IDENTIFICATION]=\"REGULAR\") THEN RETURN_VALUE=0\nELSE IF([POSTRK][POSRATE][RATE IDENTIFICATION]=\"SALARY\") THEN RETURN_VALUE=0\nELSE IF([POSTRK][POSRATE][RATE IDENTIFICATION]=\"MIS\
C SALARIES\") THEN RETURN_VALUE=0\nELSE IF([POSTRK][POSRATE][RATE IDENTIFICATION]=\"UNIT PAY\") THEN RETURN_VALUE=0 \nELSE IF([POSTRK][POSRATE][RATE IDENTIFICATION]=\"EXTENDED\") THEN RETURN_VALUE=1 \nELSE IF\
([POSTRK][POSRATE][RATE IDENTIFICATION]=\"OVERTIME\") THEN RETURN_VALUE=1\nELSE IF([POSTRK][POSRATE][RATE IDENTIFICATION]=\"LEAVE DOCKED\") THEN RETURN_VALUE=2\nELSE IF([POSTRK][POSRATE][RATE IDENTIFICATION\
]=\"ADJUSTMENTS\") THEN RETURN_VALUE=3\nELSE RETURN_VALUE=99\nFI FI FI FI FI FI FI FI");
	addvirtuallineADV(tmp,"SELECT PAYROLL SORT SEQUENCE",8,4,t,0,0);

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

	addsortline(tmp,0,"[PAYROLL][PAYJPRM][PAY IDENTIFICATION]");
	addsortline(tmp,0,"[NAME LAST LINEAGE FIRST MIDDLE]");
	addsortline(tmp,0,"[PAYROLL][PAYJPRM][PERSONNEL IDENTIFICATION]");
	addsortline(tmp,0,"[PAYROLL][PAYJPRM][JOB NUMBER]");
	addsortline(tmp,0,"[SELECT PAYROLL SORT SEQUENCE]");
	addsortline(tmp,0,"[PAYROLL][PAYJPRM][RATE IDENTIFICATION]");
	addaccumline(tmp,"LAST PERSONNEL IDENTIFICATION",3,"[PAYROLL][PAYJPRM][PERSONNEL IDENTIFICATION]",0);
	addaccumline(tmp,"LAST JOB NUMBER",3,"[PAYROLL][PAYJPRM][JOB NUMBER]",0);
	addaccumline(tmp,"LAST PAY IDENTIFICATION",3,"[PAYROLL][PAYJPRM][PAY IDENTIFICATION]",0);
	addaccumline(tmp,"TOTAL GROSS",7,"[PAYROLL][PAYJPRM][GROSS]",0);
	addaccumline(tmp,"TOTAL UNITS",7,"[PAYROLL][PAYJPRM][UNITS]",0);
	addaccumline(tmp,"FREQUENCY",1,"[PAYROLL][PAYJPRM][RATE IDENTIFICATION]",0);
	addcontrolline(tmp,"[PAYROLL][PAYJPRM][PAY IDENTIFICATION]",0);
	addcontrolline(tmp,"[NAME LAST LINEAGE FIRST MIDDLE]",0);
	addcontrolline(tmp,"[PAYROLL][PAYJPRM][PERSONNEL IDENTIFICATION]",0);
	addcontrolline(tmp,"[PAYROLL][PAYJPRM][JOB NUMBER]",0);
	addcontrolline(tmp,"[SELECT PAYROLL SORT SEQUENCE]",0);




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

	addimageline(tmp,"REPORT HEADER","OPEN [PAYROLL][PAYJPMS]",FALSE,NULL,0,pval);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST PERSONNEL IDENTIFICATION]");
	pvals->fieldname=stralloc("[PAYROLL][PAYJPMS][PERSONNEL IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST JOB NUMBER]");
	pvals->fieldname=stralloc("[PAYROLL][PAYJPMS][JOB NUMBER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST PAY IDENTIFICATION]");
	pvals->fieldname=stralloc("[PAYROLL][PAYJPMS][PAY IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("TRUE");
	pvals->fieldname=stralloc("[PAYROLL][PAYJPMS][SOURCE USER VERIFIED]");
	addimageline(tmp,"[PAYROLL][PAYJPRM][JOB NUMBER] FOOTER","WRITE [PAYROLL][PAYJPMS]",FALSE,NULL,4,pval);
	free_pvals(pval,4);


	addimageline(tmp,"REPORT FOOTER","CLOSE [PAYROLL][PAYJPMS]",FALSE,NULL,0,pval);






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
