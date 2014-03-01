/* PAYROLL's - Make (PREDIRDEP-CMS) Report */
#include <cstdio>
#include <rptgen.hpp>
/*COMPMEM 40960 */

extern void free_pvals(RDApval *,int);
extern char *module;
void PREDIRDEP(void);
static void PREDIRDEP_RANGE_SCREEN(void);

void PREDIRDEP()
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
	rptname=Rmalloc(13+1);
	sprintf(rptname,"PREDIRDEP");
	t=Rmalloc(38+1);
	sprintf(t,"Prenotification Direct Deposits Report");
	desc=Rmalloc(177+1);
	sprintf(desc,"This output definition is used to generate a printout of the Direct Deposit Diskette format for only Prenotifications on employees with DIRECT DEPOSIT for verification purposes.");
	tmp=RDAreportNEWADV(rptname,t,desc,1,"",FALSE,TRUE,FALSE,TRUE,TRUE,"PAYROLL","PREDIRDEP1",TRUE,1,FALSE,"");
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

	adddisplayADV(tmp,(double)0.000000,(double)0.000000,0,FALSE,0,"dfx1",0,1,0,NULL);

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

	t=Rmalloc(283+1);
	sprintf(t,"IF([PRENOTIFICATION ONLY]=FALSE) \nTHEN RETURN_VALUE=(([PAYROLL][PAYDEDM][DELETEFLAG]=FALSE) AND \n([PAYROLL][PAYDEDM][ACTIVE]=TRUE)) \nELSE RETURN_VALUE=(([PAYROLL][PAYDEDM][DELETEFLAG]=FALSE) AND \n([PAY\
ROLL][PAYDEDM][ACTIVE]=TRUE) AND ([PAYROLL][PAYDEDM][ACH ACTIVE]=FALSE))\nFI");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDEDM",0,NULL,NULL,"PAYDEDM KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","");
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(36+1);
	sprintf(t,"[PAYROLL][PAYDDMS][DELETEFLAG]=FALSE");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDDMS",1,"PAYROLL","PAYDEDM","PAYDDMS KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","[PAYDEDM][DEDUCTION DESCRIPTION IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(36+1);
	sprintf(t,"([PRSNNL][PERDMG][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"PRSNNL","PERDMG",2,"PAYROLL","PAYDEDM","PERDMG KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYDEDM][PERSONNEL IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(38+1);
	sprintf(t,"([PAYROLL][PAYDTYP][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDTYP",1,"PAYROLL","PAYDEDM","PAYDTYP KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEDUCTION TYPE","[DEDUCTION TYPE]");
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
	t=Rmalloc(309+1);
	sprintf(t,"IF([PRSNNL][PERDMG][LINEAGE]!=\"\") THEN RETURN_VALUE=([PRSNNL][PERDMG][LAST NAME]+\" \"+[PRSNNL][PERDMG][LINEAGE]+\", \"+[PRSNNL][PERDMG][FIRST NAME]+\" \"+[PRSNNL][PERDMG][MIDDLE NAME]) ELSE RETURN_VALUE=([P\
RSNNL][PERDMG][LAST NAME]+\", \"+[PRSNNL][PERDMG][FIRST NAME]+\" \"+[PRSNNL][PERDMG][MIDDLE NAME])");
	addvirtuallineADV(tmp,"NAME LAST LINEAGE FIRST MIDDLE",1,40,t,0,0);

	if(t!=NULL) Rfree(t);
	addvirtuallineADV(tmp,"SELECTED TRANSIT/ROUTING NUMBER",1,9,t,0,2);

	if(t!=NULL) Rfree(t);
	addvirtuallineADV(tmp,"POSTING DATE",16,8,t,0,2);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(19+1);
	sprintf(t,"[ORGANIZATION NAME]");
	addvirtuallineADV(tmp,"SENDING COMPANY NAME",1,23,t,1,0);

	if(t!=NULL) Rfree(t);
	addvirtuallineADV(tmp,"RECEIVING POINT NAME",1,23,t,0,2);

	if(t!=NULL) Rfree(t);
	addvirtuallineADV(tmp,"PRENOTIFICATION ONLY",10,1,t,0,2);

	if(t!=NULL) Rfree(t);
	addvirtuallineADV(tmp,"USE OFFSETTING TRANSACTIONS",10,1,t,0,2);

	if(t!=NULL) Rfree(t);
	addvirtuallineADV(tmp,"USE CARRAGE RETURN/LINE FEED",10,1,t,0,2);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(318+1);
	sprintf(t,"IF(([PAYROLL][PAYDPMS][PERSONNEL IDENTIFICATION]=[LAST PERSONNEL IDENTIFICATION]) AND \n   ([TRANSMITTAL ROUTE NUMBER]=[LAST RECEIVING DFI TRANSIT ROUTING NUMBER]) AND \n   ([PAYROLL][PAYDEDM][BANK ACCOU\
NT NUMBER]=[LAST BANK ACCOUNT NUMBER]))\nTHEN RETURN_VALUE=[LINE COUNT] \nELSE RETURN_VALUE=([LINE COUNT]+1) \nFI");
	addvirtuallineADV(tmp,"LINE COUNT",8,7,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(34+1);
	sprintf(t,"[PAYROLL][PAYDDMS][DEDUCTION TYPE]");
	addvirtuallineADV(tmp,"DEDUCTION TYPE",1,25,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(39+1);
	sprintf(t,"\" \"+[SELECTED TRANSIT/ROUTING NUMBER]");
	addvirtuallineADV(tmp,"IMMEDIATE DESTINATION",1,10,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(56+1);
	sprintf(t,"[SYSTEM DATE][7,2]+[SYSTEM DATE][1,2]+[SYSTEM DATE][4,2]");
	addvirtuallineADV(tmp,"TRANSMISSION DATE",1,6,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(37+1);
	sprintf(t,"[SYSTEM TIME][1,2]+[SYSTEM TIME][4,2]");
	addvirtuallineADV(tmp,"TRANSMISSION TIME",1,4,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(39+1);
	sprintf(t,"STRING_TO_UPPER([RECEIVING POINT NAME])");
	addvirtuallineADV(tmp,"DESTINATION",1,23,t,1,0);

	if(t!=NULL) Rfree(t);
	addvirtuallineADV(tmp,"REFERENCE CODE",1,8,t,0,2);

	if(t!=NULL) Rfree(t);
	addvirtuallineADV(tmp,"BATCH HEADER DISCRETIONARY DATA",1,20,t,0,2);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(12+1);
	sprintf(t,"[EIN NUMBER]");
	addvirtuallineADV(tmp,"TAX IDENTIFICATION NUMBER",1,9,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(33+1);
	sprintf(t,"\"1\"+[TAX IDENTIFICATION NUMBER]");
	addvirtuallineADV(tmp,"COMPANY IDENTIFICATION",1,10,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(33+1);
	sprintf(t,"\"1\"+[TAX IDENTIFICATION NUMBER]");
	addvirtuallineADV(tmp,"IMMEDIATE ORIGIN",1,10,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(11+1);
	sprintf(t,"\"PAYROLL\"");
	addvirtuallineADV(tmp,"COMPANY ENTRY DESCRIPTION",1,10,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(58+1);
	sprintf(t,"([SYSTEM DATE][1,2]+[SYSTEM DATE][4,2]+[SYSTEM DATE][7,2])");
	addvirtuallineADV(tmp,"COMPANY DESCRIPTIVE DATE",1,6,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(61+1);
	sprintf(t,"([POSTING DATE][7,2]+[POSTING DATE][1,2]+[POSTING DATE][4,2])");
	addvirtuallineADV(tmp,"EFFECTIVE ENTRY DATE",1,6,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(38+1);
	sprintf(t,"[SELECTED TRANSIT/ROUTING NUMBER][1,8]");
	addvirtuallineADV(tmp,"ORIGINATING DFI IDENTIFICATION",1,8,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(1+1);
	sprintf(t,"1");
	addvirtuallineADV(tmp,"BATCH NUMBER",8,7,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(106+1);
	sprintf(t,"IF([PAYROLL][PAYDEDM][CHECKING ACCOUNT]=TRUE) \nTHEN RETURN_VALUE=\"23\" \nELSE RETURN_VALUE=\"33\" FI\nFI");
	addvirtuallineADV(tmp,"TRANSACTION CODE",1,2,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(1+1);
	sprintf(t,"0");
	addvirtuallineADV(tmp,"CREDIT ENTRY AMOUNT",9,12,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(42+1);
	sprintf(t,"[PRSNNL][PERDMG][SOCIAL SECURITY NUMBER]");
	addvirtuallineADV(tmp,"INDIVIDUAL IDENTIFICATION NUMBER",1,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(49+1);
	sprintf(t,"STRING_TO_UPPER([NAME LAST LINEAGE FIRST MIDDLE])");
	addvirtuallineADV(tmp,"INDIVIDUAL NAME",1,22,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(98+1);
	sprintf(t,"IF([USE OFFSETTING TRANSACTIONS]) THEN RETURN_VALUE=[CREDIT ENTRY AMOUNT] \nELSE RETURN_VALUE=0 FI");
	addvirtuallineADV(tmp,"DEBIT ENTRY AMOUNT",9,12,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(41+1);
	sprintf(t,"(ROUNDUP(([FREQUENCY LINE COUNT]+4)/10)))");
	addvirtuallineADV(tmp,"BLOCK COUNT",8,6,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(7+1);
	sprintf(t,"\"PPD\"");
	addvirtuallineADV(tmp,"STANDARD ENTRY CLASS",1,3,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(5+1);
	sprintf(t,"\"1\"");
	addvirtuallineADV(tmp,"FILE ID MODIFIER",1,1,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(7+1);
	sprintf(t,"\"094\"");
	addvirtuallineADV(tmp,"RECORD SIZE",1,3,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(6+1);
	sprintf(t,"\"10\"");
	addvirtuallineADV(tmp,"BLOCKING FACTOR",1,2,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(5+1);
	sprintf(t,"\"1\"");
	addvirtuallineADV(tmp,"FORMAT CODE",1,1,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(162+1);
	sprintf(t,"IF([DIRECT DEPOSIT TYPE]=0) \nTHEN RETURN_VALUE=[PAYROLL][PAYDDMS][TRANSMITTAL ROUTE NUMBER] \nELSE RETURN_VALUE=[PAYROLL][PAYDEDM][TRANSMITTAL ROUTE NUMBER] \nFI");
	addvirtuallineADV(tmp,"TRANSMITTAL ROUTE NUMBER",1,9,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(50+1);
	sprintf(t,"(STRING_TO_VALUE([TRANSMITTAL ROUTE NUMBER][1,8]))");
	addvirtuallineADV(tmp,"ENTRY HASH",9,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(129+1);
	sprintf(t,"IF([PAYROLL][PAYDPMS][PERSONNEL IDENTIFICATION]=[LAST PERSONNEL IDENTIFICATION])\nTHEN RETURN_VALUE=0 \nELSE RETURN_VALUE=1  \nFI");
	addvirtuallineADV(tmp,"PERSONNEL COUNTER",9,10,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(293+1);
	sprintf(t,"IF(([PAYROLL][PAYDPMS][PERSONNEL IDENTIFICATION]=[LAST PERSONNEL IDENTIFICATION]) AND \n   ([TRANSMITTAL ROUTE NUMBER]=[LAST RECEIVING DFI TRANSIT ROUTING NUMBER]) AND \n   ([PAYROLL][PAYDEDM][BANK ACCOU\
NT NUMBER]=[LAST BANK ACCOUNT NUMBER]))\nTHEN RETURN_VALUE=0 \nELSE RETURN_VALUE=1  \nFI");
	addvirtuallineADV(tmp,"BANK ACCOUNT COUNTER",9,10,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(343+1);
	sprintf(t,"IF(([PAYROLL][PAYJDPM][PERSONNEL IDENTIFICATION]=[LAST PERSONNEL IDENTIFICATION]) AND \n   ([TRANSMITTAL ROUTE NUMBER]=[LAST RECEIVING DFI TRANSIT ROUTING NUMBER]) AND \n   ([PAYROLL][PAYDEDM][BANK ACCOU\
NT NUMBER]=[LAST BANK ACCOUNT NUMBER]))\nTHEN RETURN_VALUE=0.0 \nELSE RETURN_VALUE=(STRING_TO_VALUE([TRANSMITTAL ROUTE NUMBER][1,8])) \nFI");
	addvirtuallineADV(tmp,"ENTRY HASH AMOUNT",9,10,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(5+1);
	sprintf(t,"\" \"");
	addvirtuallineADV(tmp,"BLANK FILL",1,1,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(39+1);
	sprintf(t,"(INTEGER_TO_STRING([TOTAL ENTRY HASH]))");
	addvirtuallineADV(tmp,"TOTAL HASH NUMBER",0,0,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(97+1);
	sprintf(t,"(\"[TOTAL HASH NUMBER][\"+(INTEGER_TO_STRING(((STRING_LENGTH([TOTAL HASH NUMBER]))-9)))+\",10]\")");
	addvirtuallineADV(tmp,"TOTAL HASH LAST 10 EXP",0,0,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(43+1);
	sprintf(t,"(EVALUATE_STRING([TOTAL HASH LAST 10 EXP]))");
	addvirtuallineADV(tmp,"TOTAL HASH LAST 10 STRING",1,10,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(46+1);
	sprintf(t,"(STRING_TO_VALUE([TOTAL HASH LAST 10 STRING]))");
	addvirtuallineADV(tmp,"TOTAL HASH LAST 10",9,10,t,1,0);

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

	addsortline(tmp,0,"[NAME LAST LINEAGE FIRST MIDDLE]");
	addsortline(tmp,0,"[PAYROLL][PAYDEDM][PERSONNEL IDENTIFICATION]");
	addsortline(tmp,0,"[TRANSMITTAL ROUTE NUMBER]");
	addsortline(tmp,0,"[PAYROLL][PAYDEDM][BANK ACCOUNT NUMBER]");
	addaccumline(tmp,"FIRST USE CARRIAGE RETURN/LINE FEED",2,"[USE CARRAGE RETURN/LINE FEED]",0);
	addaccumline(tmp,"FREQUENCY LINE COUNT",1,"[PAYROLL][PAYDEDM][PERSONNEL IDENTIFICATION]",0);
	addaccumline(tmp,"LAST SELECTED TRANSIT/ROUTING NUMBER",3,"[SELECTED TRANSIT/ROUTING NUMBER]",0);
	addaccumline(tmp,"FIRST IMMEDIATE DESTINATION",2,"[IMMEDIATE DESTINATION]",0);
	addaccumline(tmp,"LAST IMMEDIATE DESTINATION",3,"[IMMEDIATE DESTINATION]",0);
	addaccumline(tmp,"LAST RECEIVING POINT NAME",3,"[RECEIVING POINT NAME]",0);
	addaccumline(tmp,"FIRST DESTINATION",2,"[DESTINATION]",0);
	addaccumline(tmp,"LAST BATCH HEADER DISCRETIONARY DATA",3,"[BATCH HEADER DISCRETIONARY DATA]",0);
	addaccumline(tmp,"FIRST COMPANY DESCRIPTIVE DATE",2,"[COMPANY DESCRIPTIVE DATE]",0);
	addaccumline(tmp,"LAST COMPANY IDENTIFICATION",3,"[COMPANY IDENTIFICATION]",0);
	addaccumline(tmp,"FIRST EFFECTIVE ENTRY DATE",2,"[EFFECTIVE ENTRY DATE]",0);
	addaccumline(tmp,"LAST TRANSACTION CODE",3,"[TRANSACTION CODE]",0);
	addaccumline(tmp,"LAST ORIGINATION DFI IDENTIFICATION",3,"[ORIGINATING DFI IDENTIFICATION]",0);
	addaccumline(tmp,"FIRST ORIGINATION DFI IDENTIFICATION",2,"[ORIGINATING DFI IDENTIFICATION]",0);
	addaccumline(tmp,"LAST RECEIVING DFI TRANSIT ROUTING NUMBER",3,"[TRANSMITTAL ROUTE NUMBER]",0);
	addaccumline(tmp,"LAST DFI ACCOUNT NUMBER",3,"[PAYROLL][PAYDEDM][BANK ACCOUNT NUMBER]",0);
	addaccumline(tmp,"LAST CREDIT ENTRY AMOUNT",3,"[CREDIT ENTRY AMOUNT]",0);
	addaccumline(tmp,"LAST INDIVIDUAL IDENTIFICATION NUMBER",3,"[INDIVIDUAL IDENTIFICATION NUMBER]",0);
	addaccumline(tmp,"LAST INDIVIDUAL NAME",3,"[INDIVIDUAL NAME]",0);
	addaccumline(tmp,"LAST BANK ACCOUNT NUMBER",3,"[PAYROLL][PAYDEDM][BANK ACCOUNT NUMBER]",0);
	addaccumline(tmp,"LAST ENTRY HASH",3,"[ENTRY HASH]",0);
	addaccumline(tmp,"TOTAL ENTRY HASH",7,"[ENTRY HASH]",0);
	addaccumline(tmp,"TOTAL ENTRY HASH AMOUNT",7,"[ENTRY HASH AMOUNT]",0);
	addaccumline(tmp,"LAST USE CARRIAGE RETURN/LINE FEED",3,"[USE CARRAGE RETURN/LINE FEED]",0);
	addaccumline(tmp,"TOTAL DEBIT ENTRY AMOUNT",7,"[DEBIT ENTRY AMOUNT]",0);
	addaccumline(tmp,"TOTAL CREDIT ENTRY AMOUNT",7,"[CREDIT ENTRY AMOUNT]",0);
	addaccumline(tmp,"TOTAL PERSONNEL COUNTER",7,"[PERSONNEL COUNTER]",0);
	addaccumline(tmp,"TOTAL BANK ACCOUNT COUNTER",7,"[BANK ACCOUNT COUNTER]",0);
	addcontrolline(tmp,"[NAME LAST LINEAGE FIRST MIDDLE]",0);
	addcontrolline(tmp,"[PAYROLL][PAYDEDM][PERSONNEL IDENTIFICATION]",0);
	addcontrolline(tmp,"[TRANSMITTAL ROUTE NUMBER]",0);
	addcontrolline(tmp,"[PAYROLL][PAYDEDM][BANK ACCOUNT NUMBER]",0);




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

	pval=Rmalloc(13*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[FIRST IMMEDIATE DESTINATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[IMMEDIATE ORIGIN]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TRANSMISSION DATE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TRANSMISSION TIME]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[FILE ID MODIFIER]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[RECORD SIZE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[BLOCKING FACTOR]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[FORMAT CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[FIRST DESTINATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SENDING COMPANY NAME]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[REFERENCE CODE]");
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
	addimageline(tmp,"REPORT HEADER","101@LLLLLLLLL@LLLLLLLLL@LLLLL@LLL@@LL@L@@LLLLLLLLLLLLLLLLLLLLLL@LLLLLLLLLLLLLLLLLLLLLL@LLLLLLL@@",FALSE,NULL,13,pval);
	free_pvals(pval,13);


	pval=Rmalloc(11*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SENDING COMPANY NAME]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[BATCH HEADER DISCRETIONARY DATA]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[COMPANY IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[STANDARD ENTRY CLASS]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[COMPANY ENTRY DESCRIPTION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[FIRST COMPANY DESCRIPTIVE DATE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[FIRST EFFECTIVE ENTRY DATE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[FIRST ORIGINATION DFI IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[BATCH NUMBER]");
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
	addimageline(tmp,"REPORT HEADER","5220@LLLLLLLLLLLLLLL@LLLLLLLLLLLLLLLLLLL@LLLLLLLLL@LL@LLLLLLLLL@LLLLL@LLLLL   1@LLLLLLL@0RRRRR@@",FALSE,NULL,11,pval);
	free_pvals(pval,11);


	pval=Rmalloc(10*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST TRANSACTION CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST RECEIVING DFI TRANSIT ROUTING NUMBER]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST DFI ACCOUNT NUMBER]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST CREDIT ENTRY AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST INDIVIDUAL IDENTIFICATION NUMBER]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST INDIVIDUAL NAME]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST ORIGINATION DFI IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LINE COUNT]");
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
	addimageline(tmp,"[PAYROLL][PAYDEDM][BANK ACCOUNT NUMBER] FOOTER","6@L@LLLLLLLL@LLLLLLLLLLLLLLLL@0RRRRRRRR@LLLLLLLLLLLLLL@LLLLLLLLLLLLLLLLLLLLL  0@LLLLLLL@0RRRRR@@",FALSE,NULL,10,pval);
	free_pvals(pval,10);


	pval=Rmalloc(9*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[FREQUENCY LINE COUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL ENTRY HASH]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL DEBIT ENTRY AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL CREDIT ENTRY AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST COMPANY IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST ORIGINATION DFI IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[BATCH NUMBER]");
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
	addimageline(tmp,"REPORT FOOTER","8220@0RRRR@000000000@0RRRRRRRRRR@0RRRRRRRRRR@LLLLLLLLL                         @LLLLLLL@0RRRRR@@",FALSE,NULL,9,pval);
	free_pvals(pval,9);


	pval=Rmalloc(8*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[BATCH NUMBER]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[BLOCK COUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[FREQUENCY LINE COUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL ENTRY HASH]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL DEBIT ENTRY AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL CREDIT ENTRY AMOUNT]");
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
	addimageline(tmp,"REPORT FOOTER","9@0RRRR@0RRRR@0RRRRRR@000000000@0RRRRRRRRRR@0RRRRRRRRRR                                       @@",FALSE,NULL,8,pval);
	free_pvals(pval,8);


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
	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999@@",TRUE,"((([FREQUENCY LINE COUNT]+4)MOD(10))<10) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0)",2,pval);
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
	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999@@",TRUE,"((([FREQUENCY LINE COUNT]+4)MOD(10))<9) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0)",2,pval);
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
	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999@@",TRUE,"((([FREQUENCY LINE COUNT]+4)MOD(10))<8) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0)",2,pval);
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
	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999@@",TRUE,"((([FREQUENCY LINE COUNT]+4)MOD(10))<7) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0)",2,pval);
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
	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999@@",TRUE,"((([FREQUENCY LINE COUNT]+4)MOD(10))<6) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0)",2,pval);
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
	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999@@",TRUE,"((([FREQUENCY LINE COUNT]+4)MOD(10))<5) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0)",2,pval);
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
	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999@@",TRUE,"IF([FREQUENCY LINE COUNT]<7) THEN RETURN_VALUE=FALSE \nELSE IF(((([FREQUENCY LINE COUNT]+4)MOD(10))<4) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0)) THEN RETURN_VALUE=TRUE\nELSE RETURN_VALUE=FALSE FI FI",2,pval);
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
	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999@@",TRUE,"IF([FREQUENCY LINE COUNT]<7) THEN RETURN_VALUE=FALSE \nELSE IF(((([FREQUENCY LINE COUNT]+4)MOD(10))<3) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0)) THEN RETURN_VALUE=TRUE\nELSE RETURN_VALUE=FALSE FI FI",2,pval);
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
	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999@@",TRUE,"IF([FREQUENCY LINE COUNT]<7) THEN RETURN_VALUE=FALSE \nELSE IF(((([FREQUENCY LINE COUNT]+4)MOD(10))<2) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0)) THEN RETURN_VALUE=TRUE\nELSE RETURN_VALUE=FALSE FI FI",2,pval);
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
	addimageline(tmp,"REPORT FOOTER","@@",FALSE,NULL,2,pval);
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
	addimageline(tmp,"REPORT FOOTER","@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL BANK ACCOUNT COUNTER]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL PERSONNEL COUNTER]");
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
	addimageline(tmp,"REPORT FOOTER","Total Bank Accounts:    @RRRRRRRRRRRRRRRRR   Total Personnel:           @RRRRRRRRRRRRR@@",FALSE,NULL,4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL HASH LAST 10]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[BLOCK COUNT]");
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
	addimageline(tmp,"REPORT FOOTER","Entry Hash:                     @0RRRRRRRR   Block Count:               @RRRRRRRRRRRRR@@",FALSE,NULL,4,pval);
	free_pvals(pval,4);






	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			ERRORDIALOG("Cannot Write Report","Error in permissions, user cannot write the report definition.  Check your permissions, and retry.  Call your installer.",NULL,FALSE);
			prterr("Cannont Write Report definition for Report [%s].  Error in permissions, user cannot write the report definition.",rptname);
		} else {
			PREDIRDEP_RANGE_SCREEN();
		}
		FreeRDAreport(tmp);
	}
	if(rptname!=NULL) Rfree(rptname);
}
static void PREDIRDEP_RANGE_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;

/*============================================================================
Adding Screen Widgets
void xaddwdgt(RDAscrn *screen,short type,char *resource_name,char *label,
	char *pixmap,char *ywlabel,short rows,short cols,int rtype,
	char *expression,char *editable_expression,char *sensitive_expression,
	char *transversal_expression,int line,char *file)

#define ADVaddwdgt(a,b,c,d,p,w,e,f,g,h,i,j,l)   xaddwdgt(a,b,c,d,p,w,e,f,g,h,i,j,l,__LINE__,__FILE__)

RDAwdgts types
{
	0	-	Standard Resource
	1	-	New Line
	2	-	End Line
	3	-	New Box
	4	-	End Box
	5	-	Label
	6	-	Button
	7	-	Scrolled List
	8	-	Scrolled Text
	9	-	Toggle Button (Boolean)
	10	-	Frame
	11	-	Seperator
	12	-	New Scrolled Window
	13	-	End Scrolled Window
	14	-	Optional Screen
	15	-	Custom Type (Same as Resource Type, except for rtype.)
	16	-	Expenditure Type (Same as Resource Type, except for rtype.)
	17	-	Revenue Type (Same as Resource Type, except for rtype.)
	18	-	Balance Sheet Type (Same as Resource Type, except for rtype.)
	19	-	Beginning Balance Type (Same as Resource Type, except for rtype.)

New Widgets
	??	-	New Menu Bar (Start Menu Bar)
	??	-	End Menu Bar (End Menu Bar)
	??	-	Menu Control (Menu Control Option)
	??	-	Needs a Modifying Character????????
	??	-	Menu Button (Buttons available to the menu)
	??	-	Needs a Modifying Character????????
}
Resource Types
	5-Label	0	-	Text Label
		1	-	Pixmap Label

	6-Button	0	-	Push Button
		1	-	Arrow Button Right
		2	-	Arrow Button Left
		3	-	Arrow Button Up
		4	-	Arrow Button Down
		5	-	Pixmap

	8-Scrolled Text	0	-	WordWap & Horizontal SB (Side Bar)
		1	-	Horizontal SB & No WordWrap

	9-Toggle Button	0	-	Text Label
		1	-	Pixmap Label

	10-Frame	0	-	Shadow In
		1	-	Shadow Out
		2	-	Shadow Etched In
		3	-	Shadow Etched Out

	11-Seperator	0	-	Horizontal Single Line
		1	-	Horizontal Double Line
		2	-	Horizontal Single Dashed
		3	-	Horizontal Double Dashed
		4	-	Vertical Single Line
		5	-	Vertical Double Line
		6	-	Vertical Single Dashed
		7	-	Vertical Double Dashed

		14-OptionalScrn	0	-	Push Button
		1	-	Arrow Button Right
		2	-	Arrow Button Left
		3	-	Arrow Button Up
		4	-	Arrow Button Down
		5	-	Pixmap

============================================================================*/


	scrn=RDAscrnNEW("PAYROLL","PREDIRDEP-CMS RANGE SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Transit/Routing Number (Immediate Destination):","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"SELECTED TRANSIT/ROUTING NUMBER","Select Transit/Routing Number","","",0,9,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Posting Date:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"POSTING DATE","Posting Date","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Receiving Point Name:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"RECEIVING POINT NAME","Receiving Point Name","","",0,23,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"PRENOTIFICATION ONLY","ACH Inactives Only","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"USE OFFSETTING TRANSACTIONS","Use Offsetting Transactions","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"USE CARRAGE RETURN/LINE FEED","Use Carrage Return/Line Feeds","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [PAYDDMS][DEDUCTION TYPE]","Range on Direct Deposit Deduction Type","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [PAYDDMS][DEDUCTION TYPE]","From Direct Deposit Deduction Type","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [PAYDDMS][DEDUCTION TYPE]","To Direct Deposit Deduction Type","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [PAYDEDM][PERSONNEL IDENTIFICATION]","Range on Personnel Identification","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [PAYDEDM][PERSONNEL IDENTIFICATION]","From Personnel Identification","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [PAYDEDM][PERSONNEL IDENTIFICATION]","To Personnel Identification","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Optional Reference Code:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"REFERENCE CODE","Reference Code","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,5,"","Optional Batch Header Discretionary Data:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"BATCH HEADER DISCRETIONARY DATA","Batch Header Discretionary Data","","",0,20,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"RUN REPORT","Run Report","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
		if(writescrnbin(defdir,scrn))
		{
			ERRORDIALOG("Cannot Save Screen","This screen may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
}
