/* PAYROLL's - Make (CHGSTEPS1) Report */
#include <cstdio>
#include <rptgen.hpp>
/*COMPMEM 40960 */

extern void free_pvals(RDApval *,int);
extern char *module;
void CHGSTEPS1(void);

void CHGSTEPS1()
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
	rptname=Rmalloc(9+1);
	sprintf(rptname,"CHGSTEPS1");
	t=Rmalloc(12+1);
	sprintf(t,"Change Steps");
	desc=Rmalloc(80+1);
	sprintf(desc,"This output definition is used to manage the Change Steps (Years of Experience).");
	tmp=RDAreportNEWADV(rptname,t,desc,1,"",FALSE,FALSE,FALSE,FALSE,FALSE,"","",TRUE,0,TRUE,"");
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

	adddisplayADV(tmp,(double)8.500000,(double)10.000000,4,FALSE,0,"",0,1,0,NULL);

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

	t=Rmalloc(76+1);
	sprintf(t,"([PAYROLL][PAYJMST][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYJMST][ACTIVE]=TRUE)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYJMST",0,NULL,NULL,"JOBMSTR KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","");
		addsearchconfld(s,"JOB NUMBER","");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(37+1);
	sprintf(t,"([POSTRK][POSMSTR][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"POSTRK","POSMSTR",2,"PAYROLL","PAYJMST","POSMSTR",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"POSITION IDENTIFICATION","[PAYJMST][POSITION IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(36+1);
	sprintf(t,"([PRSNNL][PERDMG][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"PRSNNL","PERDMG",2,"PAYROLL","PAYJMST","PERDMG KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJMST][PERSONNEL IDENTIFICATION]");
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
	t=Rmalloc(30+1);
	sprintf(t,"\"CHANGED SALARY AUDIT TRAIL\"");
	addvirtuallineADV(tmp,"REPORT TITLE",0,0,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(42+1);
	sprintf(t,"(\"Executed By: \"+[LOGIN IDENTIFICATION])");
	addvirtuallineADV(tmp,"EXECUTED BY",0,0,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(26+1);
	sprintf(t,"[PAYROLL][PAYJMST][STEP]+1");
	addvirtuallineADV(tmp,"NEW STEP",7,4,t,0,0);

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

	addaccumline(tmp,"LAST PERSONNEL IDENTIFICATION",3,"[PAYROLL][PAYJMST][PERSONNEL IDENTIFICATION]",0);
	addaccumline(tmp,"LAST PERSONNEL NAME",3,"[PRSNNL][PERDMG][NAME]",0);
	addaccumline(tmp,"LAST POSITION IDENTIFICATION",3,"[PAYROLL][PAYJMST][POSITION IDENTIFICATION]",0);
	addaccumline(tmp,"LAST GROSS IDENTIFICATION",3,"[PAYROLL][PAYJMST][GROSS IDENTIFICATION]",0);
	addaccumline(tmp,"LAST LOCATION IDENTIFICATION",3,"[POSTRK][POSMSTR][LOCATION IDENTIFICATION]",0);
	addaccumline(tmp,"LAST CLASS IDENTIFICATION",3,"[POSTRK][POSMSTR][CLASS IDENTIFICATION]",0);
	addaccumline(tmp,"LAST JOB NUMBER",3,"[PAYROLL][PAYJMST][JOB NUMBER]",0);
	addaccumline(tmp,"LAST STEP",3,"[PAYROLL][PAYJMST][STEP]",0);
	addaccumline(tmp,"LAST NEW STEP",3,"[NEW STEP]",0);




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

	addimageline(tmp,"REPORT HEADER","OPEN [PAYROLL][PAYJMST]",FALSE,NULL,0,pval);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYJMST][PERSONNEL IDENTIFICATION]");
	pvals->fieldname=stralloc("[PAYROLL][PAYJMST][PERSONNEL IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYJMST][JOB NUMBER]");
	pvals->fieldname=stralloc("[PAYROLL][PAYJMST][JOB NUMBER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[NEW STEP]");
	pvals->fieldname=stralloc("[PAYROLL][PAYJMST][STEP]");
	addimageline(tmp,"DETAIL LINES","WRITE [PAYROLL][PAYJMST]",TRUE,"([PAYROLL][PAYJMST][STEP]!=[NEW STEP])",3,pval);
	free_pvals(pval,3);


	addimageline(tmp,"REPORT FOOTER","CLOSE [PAYROLL][PAYJMST]",FALSE,NULL,0,pval);






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
