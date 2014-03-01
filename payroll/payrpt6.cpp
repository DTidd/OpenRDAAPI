/* PAYROLL's - Make (DIRDEPDSK1) Report */
#include <cstdio>
#include <rptgen.hpp>
/*COMPMEM 40960 */

extern void free_pvals(RDApval *,int);
extern void free_pvars(RDApvar *,int);
extern char *module;
void DIRDEPDSK1(void);

void DIRDEPDSK1()
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
	rptname=Rmalloc(10+1);
	sprintf(rptname,"DIRDEPDSK1");
	t=Rmalloc(22+1);
	sprintf(t,"Direct Deposits Report");
	desc=Rmalloc(118+1);
	sprintf(desc,"This output definition is used to generate a printout of the Direct Deposit Diskette format for verification purposes.");
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
	adddisplayADV(tmp,(double)0.000000,(double)0.000000,1,FALSE,0,"drctdpst",0,1,0,pvars);
	free_pvars(pvars,0);

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

	t=Rmalloc(115+1);
	sprintf(t,"([PAYROLL][PAYJDPM][DELETEFLAG]=FALSE) AND \n([PAYROLL][PAYJDPM][PAY IDENTIFICATION]=[SELECTED PAY IDENTIFICATION])");
	x=addsearchfileADV(tmp,"PAYROLL","PAYJDPM",0,NULL,NULL,"PAYJDPM KEY #2",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PAY IDENTIFICATION","[SELECTED PAY IDENTIFICATION]");
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","");
		addsearchconfld(s,"JOB NUMBER","");
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(38+1);
	sprintf(t,"([PAYROLL][PAYDDMS][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDDMS",1,"PAYROLL","PAYJDPM","PAYDDMS KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","[PAYJDPM][DEDUCTION DESCRIPTION IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(105+1);
	sprintf(t,"([PAYROLL][PAYDEDM][DELETEFLAG]=FALSE) AND \n(NOT([PAYROLL][PAYDEDM][ACH ACTIVE]=[PRENOTIFICATION ONLY]))");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDEDM",1,"PAYROLL","PAYJDPM","PAYDEDM KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJDPM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","[PAYJDPM][DEDUCTION DESCRIPTION IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(171+1);
	sprintf(t,"([PAYROLL][PAYJPMS][DELETEFLAG]=FALSE) AND \n([PAYROLL][PAYJPMS][SOURCE USER VERIFIED]=TRUE) AND \n([PAYROLL][PAYJPMS][BANK IDENTIFICATION]=[SELECTED BANK IDENTIFICATION])");
	x=addsearchfileADV(tmp,"PAYROLL","PAYJPMS",1,"PAYROLL","PAYJDPM","PAYJPMS KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJDPM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYJDPM][JOB NUMBER]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJDPM][PAY IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(75+1);
	sprintf(t,"([PAYROLL][PAYDPMS][DELETEFLAG]=FALSE) AND \n([PAYROLL][PAYDPMS][AMOUNT]>0)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDPMS",1,"PAYROLL","PAYJDPM","PAYDPMS KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJDPM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","[PAYJDPM][DEDUCTION DESCRIPTION IDENTIFICATION]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJDPM][PAY IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(36+1);
	sprintf(t,"([PRSNNL][PERDMG][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"PRSNNL","PERDMG",2,"PAYROLL","PAYJDPM","PERDMG KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJDPM][PERSONNEL IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	x=addsearchfileADV(tmp,"PAYROLL","PAYMSTR",1,"PAYROLL","PAYJDPM","PAYMSTR KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJDPM][PAY IDENTIFICATION]");
	}

	t=Rmalloc(38+1);
	sprintf(t,"([PAYROLL][PAYDTYP][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDTYP",1,"PAYROLL","PAYJDPM","PAYDTYP KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEDUCTION TYPE","[DEDUCTION TYPE]");
	}
	if(t!=NULL) Rfree(t);

	x=addsearchfileADV(tmp,"BNKREC","BNKACC",1,"PAYROLL","PAYJDPM","BNKACC KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"BANK IDENTIFICATION","[BANK IDENTIFICATION]");
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
	t=Rmalloc(309+1);
	sprintf(t,"IF([PRSNNL][PERDMG][LINEAGE]!=\"\") THEN RETURN_VALUE=([PRSNNL][PERDMG][LAST NAME]+\" \"+[PRSNNL][PERDMG][LINEAGE]+\", \"+[PRSNNL][PERDMG][FIRST NAME]+\" \"+[PRSNNL][PERDMG][MIDDLE NAME]) ELSE RETURN_VALUE=([P\
RSNNL][PERDMG][LAST NAME]+\", \"+[PRSNNL][PERDMG][FIRST NAME]+\" \"+[PRSNNL][PERDMG][MIDDLE NAME])");
	addvirtuallineADV(tmp,"NAME LAST LINEAGE FIRST MIDDLE",1,40,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(5+1);
	sprintf(t,"\"1\"");
	addvirtuallineADV(tmp,"JUST A ONE",1,1,t,1,0);

	if(t!=NULL) Rfree(t);
	addvirtuallineADV(tmp,"SELECTED PAY IDENTIFICATION",1,20,t,0,2);

	if(t!=NULL) Rfree(t);
	addvirtuallineADV(tmp,"SELECTED BANK IDENTIFICATION",0,0,t,0,2);

	if(t!=NULL) Rfree(t);
	addvirtuallineADV(tmp,"SELECTED TRANSIT/ROUTING NUMBER",0,9,t,0,2);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(39+1);
	sprintf(t,"[PAYROLL][PAYJPMS][BANK IDENTIFICATION]");
	addvirtuallineADV(tmp,"BANK IDENTIFICATION",1,30,t,0,0);

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
	addvirtuallineADV(tmp,"ORGANIZATION ACCOUNT NUMBER",0,0,t,0,2);

	if(t!=NULL) Rfree(t);
	addvirtuallineADV(tmp,"USE OFFSETTING TRANSACTIONS",10,1,t,0,2);

	if(t!=NULL) Rfree(t);
	addvirtuallineADV(tmp,"USE CARRAGE RETURN/LINE FEED",10,1,t,0,2);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(318+1);
	sprintf(t,"IF(([PAYROLL][PAYJDPM][PERSONNEL IDENTIFICATION]=[LAST PERSONNEL IDENTIFICATION]) AND \n   ([TRANSMITTAL ROUTE NUMBER]=[LAST RECEIVING DFI TRANSIT ROUTING NUMBER]) AND \n   ([PAYROLL][PAYDEDM][BANK ACCOU\
NT NUMBER]=[LAST BANK ACCOUNT NUMBER]))\nTHEN RETURN_VALUE=[LINE COUNT] \nELSE RETURN_VALUE=([LINE COUNT]+1) \nFI");
	addvirtuallineADV(tmp,"LINE COUNT",8,7,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(14+1);
	sprintf(t,"[LINE COUNT]-1");
	addvirtuallineADV(tmp,"FREQUENCY LINE COUNT",8,7,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(34+1);
	sprintf(t,"[PAYROLL][PAYDDMS][DEDUCTION TYPE]");
	addvirtuallineADV(tmp,"DEDUCTION TYPE",1,40,t,0,0);

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
	sprintf(t,"\" \"+[TAX IDENTIFICATION NUMBER]");
	addvirtuallineADV(tmp,"IMMEDIATE ORIGIN",1,10,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(11+1);
	sprintf(t,"\"PAYROLL\"");
	addvirtuallineADV(tmp,"COMPANY ENTRY DESCRIPTION",1,10,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(107+1);
	sprintf(t,"[PAYROLL][PAYMSTR][PAY DATE][1,1,2]+[PAYROLL][PAYMSTR][PAY DATE][1,4,2]+[PAYROLL][PAYMSTR][PAY DATE][1,7,2]");
	addvirtuallineADV(tmp,"COMPANY DESCRIPTIVE DATE",1,6,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(59+1);
	sprintf(t,"[POSTING DATE][7,2]+[POSTING DATE][1,2]+[POSTING DATE][4,2]");
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
	t=Rmalloc(284+1);
	sprintf(t,"IF([PAYROLL][PAYDEDM][ACH ACTIVE]=TRUE) \nTHEN IF([PAYROLL][PAYDEDM][CHECKING ACCOUNT]=TRUE) \n     THEN RETURN_VALUE=\"22\" \n     ELSE RETURN_VALUE=\"32\" FI\nELSE IF([PAYROLL][PAYDEDM][CHECKING ACCOUNT]=TRU\
E) \n     THEN RETURN_VALUE=\"23\" \n     ELSE RETURN_VALUE=\"33\" FI\nFI");
	addvirtuallineADV(tmp,"TRANSACTION CODE",1,2,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(101+1);
	sprintf(t,"IF([PRENOTIFICATION ONLY]=TRUE) THEN RETURN_VALUE=0 \nELSE RETURN_VALUE=[PAYROLL][PAYJDPM][AMOUNT] FI");
	addvirtuallineADV(tmp,"CREDIT ENTRY AMOUNT",9,12,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(101+1);
	sprintf(t,"IF([PRENOTIFICATION ONLY]=TRUE) THEN RETURN_VALUE=0 \nELSE RETURN_VALUE=[PAYROLL][PAYJDPM][AMOUNT] FI");
	addvirtuallineADV(tmp,"CREDIT ENTRY AMOUNT DOLLARS",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(42+1);
	sprintf(t,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION]");
	addvirtuallineADV(tmp,"INDIVIDUAL IDENTIFICATION NUMBER",1,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(132+1);
	sprintf(t,"([PRSNNL][PERDMG][FIRST NAME]+\" \"+[PRSNNL][PERDMG][MIDDLE NAME]+\" \"+[PRSNNL][PERDMG][LAST NAME]+\" \"+[PRSNNL][PERDMG][LINEAGE])");
	addvirtuallineADV(tmp,"NAME FIRST MIDDLE LAST LINEAGE",1,40,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(49+1);
	sprintf(t,"STRING_TO_UPPER([NAME FIRST MIDDLE LAST LINEAGE])");
	addvirtuallineADV(tmp,"INDIVIDUAL NAME",1,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(98+1);
	sprintf(t,"IF([USE OFFSETTING TRANSACTIONS]) THEN RETURN_VALUE=[CREDIT ENTRY AMOUNT] \nELSE RETURN_VALUE=0 FI");
	addvirtuallineADV(tmp,"DEBIT ENTRY AMOUNT",9,12,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(106+1);
	sprintf(t,"IF([USE OFFSETTING TRANSACTIONS]) THEN RETURN_VALUE=[CREDIT ENTRY AMOUNT DOLLARS] \nELSE RETURN_VALUE=0 FI");
	addvirtuallineADV(tmp,"DEBIT ENTRY AMOUNT DOLLARS",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(41+1);
	sprintf(t,"(ROUND((([FREQUENCY LINE COUNT]+4)/10))))");
	addvirtuallineADV(tmp,"BLOCK COUNT",8,6,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(7+1);
	sprintf(t,"\"PPD\"");
	addvirtuallineADV(tmp,"STANDARD ENTRY CLASS",1,3,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(5+1);
	sprintf(t,"\"A\"");
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
	sprintf(t,"IF([PAYROLL][PAYJDPM][PERSONNEL IDENTIFICATION]=[LAST PERSONNEL IDENTIFICATION])\nTHEN RETURN_VALUE=0 \nELSE RETURN_VALUE=1  \nFI");
	addvirtuallineADV(tmp,"PERSONNEL COUNTER",9,10,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(293+1);
	sprintf(t,"IF(([PAYROLL][PAYJDPM][PERSONNEL IDENTIFICATION]=[LAST PERSONNEL IDENTIFICATION]) AND \n   ([TRANSMITTAL ROUTE NUMBER]=[LAST RECEIVING DFI TRANSIT ROUTING NUMBER]) AND \n   ([PAYROLL][PAYDEDM][BANK ACCOU\
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
	t=Rmalloc(46+1);
	sprintf(t,"(INTEGER_TO_STRING([TOTAL ENTRY HASH AMOUNT]))");
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
	addsortline(tmp,0,"[PAYROLL][PAYJDPM][PERSONNEL IDENTIFICATION]");
	addsortline(tmp,0,"[TRANSMITTAL ROUTE NUMBER]");
	addsortline(tmp,0,"[PAYROLL][PAYDEDM][BANK ACCOUNT NUMBER]");
	addaccumline(tmp,"LAST SELECTED TRANSIT/ROUTING NUMBER",3,"[SELECTED TRANSIT/ROUTING NUMBER]",0);
	addaccumline(tmp,"LAST IMMEDIATE DESTINATION",3,"[IMMEDIATE DESTINATION]",0);
	addaccumline(tmp,"LAST BATCH HEADER DISCRETIONARY DATA",3,"[BATCH HEADER DISCRETIONARY DATA]",0);
	addaccumline(tmp,"LAST COMPANY DESCRIPTIVE DATE",3,"[COMPANY DESCRIPTIVE DATE]",0);
	addaccumline(tmp,"LAST EFFECTIVE ENTRY DATE",3,"[EFFECTIVE ENTRY DATE]",0);
	addaccumline(tmp,"LAST TRANSACTION CODE",3,"[TRANSACTION CODE]",0);
	addaccumline(tmp,"LAST ORIGINATION DFI IDENTIFICATION",3,"[ORIGINATING DFI IDENTIFICATION]",0);
	addaccumline(tmp,"LAST RECEIVING DFI TRANSIT ROUTING NUMBER",3,"[TRANSMITTAL ROUTE NUMBER]",0);
	addaccumline(tmp,"LAST DFI ACCOUNT NUMBER",3,"[PAYROLL][PAYDEDM][BANK ACCOUNT NUMBER]",0);
	addaccumline(tmp,"LAST CREDIT ENTRY AMOUNT",3,"[CREDIT ENTRY AMOUNT]",0);
	addaccumline(tmp,"LAST INDIVIDUAL IDENTIFICATION NUMBER",3,"[INDIVIDUAL IDENTIFICATION NUMBER]",0);
	addaccumline(tmp,"LAST INDIVIDUAL NAME",3,"[INDIVIDUAL NAME]",0);
	addaccumline(tmp,"LAST PERSONNEL IDENTIFICATION",3,"[PAYROLL][PAYJDPM][PERSONNEL IDENTIFICATION]",0);
	addaccumline(tmp,"LAST JOB NUMBER",3,"[PAYROLL][PAYJDPM][JOB NUMBER]",0);
	addaccumline(tmp,"LAST BANK ACCOUNT NUMBER",3,"[PAYROLL][PAYDEDM][BANK ACCOUNT NUMBER]",0);
	addaccumline(tmp,"LAST USE CARRIAGE RETURN/LINE FEED",3,"[USE CARRAGE RETURN/LINE FEED]",0);
	addaccumline(tmp,"TOTAL ENTRY HASH",7,"[ENTRY HASH]",0);
	addaccumline(tmp,"TOTAL ENTRY HASH AMOUNT",7,"[ENTRY HASH AMOUNT]",0);
	addaccumline(tmp,"TOTAL DEBIT ENTRY AMOUNT",7,"[DEBIT ENTRY AMOUNT]",0);
	addaccumline(tmp,"TOTAL DEBIT ENTRY AMOUNT DOLLARS",7,"[DEBIT ENTRY AMOUNT DOLLARS]",0);
	addaccumline(tmp,"TOTAL CREDIT ENTRY AMOUNT",7,"[CREDIT ENTRY AMOUNT]",0);
	addaccumline(tmp,"TOTAL CREDIT ENTRY AMOUNT DOLLARS",7,"[CREDIT ENTRY AMOUNT DOLLARS]",0);
	addaccumline(tmp,"TOTAL PERSONNEL COUNTER",7,"[PERSONNEL COUNTER]",0);
	addaccumline(tmp,"TOTAL BANK ACCOUNT COUNTER",7,"[BANK ACCOUNT COUNTER]",0);
	addcontrolline(tmp,"[NAME LAST LINEAGE FIRST MIDDLE]",0);
	addcontrolline(tmp,"[PAYROLL][PAYJDPM][PERSONNEL IDENTIFICATION]",0);
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
	pvals->name=stralloc("[IMMEDIATE DESTINATION]");
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
	pvals->name=stralloc("[DESTINATION]");
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
	addimageline(tmp,"REPORT HEADER","101@LLLLLLLLL@LLLLLLLLL@LLLLL@LLL@@LL@L@@LLLLLLLLLLLLLLLLLLLLLL@LLLLLLLLLLLLLLLLLLLLLL@LLLLLLL@@",TRUE,"[USE CARRAGE RETURN/LINE FEED]",13,pval);
	free_pvals(pval,13);


	pval=Rmalloc(11*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[IMMEDIATE DESTINATION]");
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
	pvals->name=stralloc("[DESTINATION]");
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
	addimageline(tmp,"REPORT HEADER","101@LLLLLLLLL@LLLLLLLLL@LLLLL@LLL@@LL@L@@LLLLLLLLLLLLLLLLLLLLLL@LLLLLLLLLLLLLLLLLLLLLL@LLLLLLL",TRUE,"NOT([USE CARRAGE RETURN/LINE FEED])",11,pval);
	free_pvals(pval,11);


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
	pvals->name=stralloc("[COMPANY DESCRIPTIVE DATE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[EFFECTIVE ENTRY DATE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ORIGINATING DFI IDENTIFICATION]");
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
	addimageline(tmp,"REPORT HEADER","5220@LLLLLLLLLLLLLLL@LLLLLLLLLLLLLLLLLLL@LLLLLLLLL@LL@LLLLLLLLL@LLLLL@LLLLL   1@LLLLLLL@0RRRRR@@",TRUE,"[USE CARRAGE RETURN/LINE FEED]",11,pval);
	free_pvals(pval,11);


	pval=Rmalloc(9*sizeof(RDApval));
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
	pvals->name=stralloc("[COMPANY DESCRIPTIVE DATE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[EFFECTIVE ENTRY DATE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ORIGINATING DFI IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[BATCH NUMBER]");
	pvals->fieldname=NULL;
	addimageline(tmp,"REPORT HEADER","5220@LLLLLLLLLLLLLLL@LLLLLLLLLLLLLLLLLLL@LLLLLLLLL@LL@LLLLLLLLL@LLLLL@LLLLL   1@LLLLLLL@0RRRRR",TRUE,"NOT([USE CARRAGE RETURN/LINE FEED])",9,pval);
	free_pvals(pval,9);


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
	pvals->name=stralloc("[TOTAL CREDIT ENTRY AMOUNT]");
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
	addimageline(tmp,"[PAYROLL][PAYDEDM][BANK ACCOUNT NUMBER] FOOTER","6@L@LLLLLLLL@LLLLLLLLLLLLLLLL@0RRRRRRRR@LLLLLLLLLLLLLL@LLLLLLLLLLLLLLLLLLLLL  0@LLLLLLL@0RRRRR@@",TRUE,"[LAST USE CARRIAGE RETURN/LINE FEED]",10,pval);
	free_pvals(pval,10);


	pval=Rmalloc(8*sizeof(RDApval));
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
	pvals->name=stralloc("[TOTAL CREDIT ENTRY AMOUNT]");
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
	addimageline(tmp,"[PAYROLL][PAYDEDM][BANK ACCOUNT NUMBER] FOOTER","6@L@LLLLLLLL@LLLLLLLLLLLLLLLL@0RRRRRRRR@LLLLLLLLLLLLLL@LLLLLLLLLLLLLLLLLLLLL  0@LLLLLLL@0RRRRR",TRUE,"NOT([LAST USE CARRIAGE RETURN/LINE FEED])",8,pval);
	free_pvals(pval,8);


	pval=Rmalloc(10*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[FREQUENCY LINE COUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL HASH LAST 10]");
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
	pvals->name=stralloc("[JUST A ONE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TAX IDENTIFICATION NUMBER]");
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
	addimageline(tmp,"REPORT FOOTER","8220@0RRRR@0RRRRRRRR@0RRRRRRRRRR@0RRRRRRRRRR@@LLLLLLLL                         @LLLLLLL@0RRRRR@@",TRUE,"[LAST USE CARRIAGE RETURN/LINE FEED]",10,pval);
	free_pvals(pval,10);


	pval=Rmalloc(10*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[FREQUENCY LINE COUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL HASH LAST 10]");
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
	pvals->name=stralloc("[JUST A ONE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TAX IDENTIFICATION NUMBER]");
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
	addimageline(tmp,"REPORT FOOTER","8220@0RRRR@0RRRRRRRR@0RRRRRRRRRR@0RRRRRRRRRR@@LLLLLLLL                         @LLLLLLL@0RRRRR@@",TRUE,"NOT([LAST USE CARRIAGE RETURN/LINE FEED])",10,pval);
	free_pvals(pval,10);


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
	pvals->name=stralloc("[TOTAL HASH LAST 10]");
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
	addimageline(tmp,"REPORT FOOTER","9@0RRRR@0RRRR@0RRRRRR@0RRRRRRRR@0RRRRRRRRRR@0RRRRRRRRRR                                       @@",TRUE,"[LAST USE CARRIAGE RETURN/LINE FEED]",8,pval);
	free_pvals(pval,8);


	pval=Rmalloc(7*sizeof(RDApval));
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
	pvals->name=stralloc("[TOTAL HASH LAST 10]");
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
	pvals->name=stralloc("[BLANK FILL]");
	pvals->fieldname=NULL;
	addimageline(tmp,"REPORT FOOTER","9@0RRRR@0RRRR@0RRRRRRR@0RRRRRRRR@0RRRRRRRRR@0RRRRRRRRRR                                      @",TRUE,"NOT([LAST USE CARRIAGE RETURN/LINE FEED])",7,pval);
	free_pvals(pval,7);


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
	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999@@",TRUE,"((([FREQUENCY LINE COUNT]+4)MOD(10))<10) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0) AND ([LAST USE CARRIAGE RETURN/LINE FEED])",2,pval);
	free_pvals(pval,2);


	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999",TRUE,"((([FREQUENCY LINE COUNT]+4)MOD(10))<10) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0) AND (NOT([LAST USE CARRIAGE RETURN/LINE FEED]))",0,pval);


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
	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999@@",TRUE,"((([FREQUENCY LINE COUNT]+4)MOD(10))<9) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0) AND ([LAST USE CARRIAGE RETURN/LINE FEED])",2,pval);
	free_pvals(pval,2);


	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999",TRUE,"((([FREQUENCY LINE COUNT]+4)MOD(10))<9) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0) AND (NOT([LAST USE CARRIAGE RETURN/LINE FEED]))",0,pval);


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
	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999@@",TRUE,"((([FREQUENCY LINE COUNT]+4)MOD(10))<8) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0) AND ([LAST USE CARRIAGE RETURN/LINE FEED])",2,pval);
	free_pvals(pval,2);


	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999",TRUE,"((([FREQUENCY LINE COUNT]+4)MOD(10))<8) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0) AND (NOT([LAST USE CARRIAGE RETURN/LINE FEED]))",0,pval);


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
	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999@@",TRUE,"((([FREQUENCY LINE COUNT]+4)MOD(10))<7) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0) AND ([LAST USE CARRIAGE RETURN/LINE FEED])",2,pval);
	free_pvals(pval,2);


	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999",TRUE,"((([FREQUENCY LINE COUNT]+4)MOD(10))<7) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0) AND (NOT([LAST USE CARRIAGE RETURN/LINE FEED]))",0,pval);


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
	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999@@",TRUE,"((([FREQUENCY LINE COUNT]+4)MOD(10))<6) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0) AND ([LAST USE CARRIAGE RETURN/LINE FEED])",2,pval);
	free_pvals(pval,2);


	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999",TRUE,"((([FREQUENCY LINE COUNT]+4)MOD(10))<6) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0) AND (NOT([LAST USE CARRIAGE RETURN/LINE FEED]))",0,pval);


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
	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999@@",TRUE,"((([FREQUENCY LINE COUNT]+4)MOD(10))<5) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0) AND ([LAST USE CARRIAGE RETURN/LINE FEED])",2,pval);
	free_pvals(pval,2);


	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999",TRUE,"((([FREQUENCY LINE COUNT]+4)MOD(10))<5) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0) AND (NOT([LAST USE CARRIAGE RETURN/LINE FEED]))",0,pval);


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
	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999@@",TRUE,"IF([FREQUENCY LINE COUNT]<7) THEN RETURN_VALUE=FALSE \nELSE IF(((([FREQUENCY LINE COUNT]+4)MOD(10))<4) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0) AND ([LAST USE CARRIAGE RETURN/LINE FEED])) THEN RETURN\
_VALUE=TRUE\nELSE RETURN_VALUE=FALSE FI FI",2,pval);
	free_pvals(pval,2);


	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999",TRUE,"IF([FREQUENCY LINE COUNT]<7) THEN RETURN_VALUE=FALSE \nELSE IF(((([FREQUENCY LINE COUNT]+4)MOD(10))<4) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0) AND (NOT([LAST USE CARRIAGE RETURN/LINE FEED]))) THEN R\
ETURN_VALUE=TRUE\nELSE RETURN_VALUE=FALSE FI FI",0,pval);


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
	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999@@",TRUE,"IF([FREQUENCY LINE COUNT]<7) THEN RETURN_VALUE=FALSE \nELSE IF(((([FREQUENCY LINE COUNT]+4)MOD(10))<3) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0) AND ([LAST USE CARRIAGE RETURN/LINE FEED])) THEN RETURN\
_VALUE=TRUE\nELSE RETURN_VALUE=FALSE FI FI",2,pval);
	free_pvals(pval,2);


	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999",TRUE,"IF([FREQUENCY LINE COUNT]<7) THEN RETURN_VALUE=FALSE \nELSE IF(((([FREQUENCY LINE COUNT]+4)MOD(10))<3) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0) AND (NOT([LAST USE CARRIAGE RETURN/LINE FEED]))) THEN R\
ETURN_VALUE=TRUE\nELSE RETURN_VALUE=FALSE FI FI",0,pval);


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
	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999@@",TRUE,"IF([FREQUENCY LINE COUNT]<7) THEN RETURN_VALUE=FALSE \nELSE IF(((([FREQUENCY LINE COUNT]+4)MOD(10))<2) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0) AND ([LAST USE CARRIAGE RETURN/LINE FEED])) THEN RETURN\
_VALUE=TRUE\nELSE RETURN_VALUE=FALSE FI FI",2,pval);
	free_pvals(pval,2);


	addimageline(tmp,"REPORT FOOTER","9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999",TRUE,"IF([FREQUENCY LINE COUNT]<7) THEN RETURN_VALUE=FALSE \nELSE IF(((([FREQUENCY LINE COUNT]+4)MOD(10))<2) AND ((([FREQUENCY LINE COUNT]+4)MOD(10))>0) AND (NOT([LAST USE CARRIAGE RETURN/LINE FEED]))) THEN R\
ETURN_VALUE=TRUE\nELSE RETURN_VALUE=FALSE FI FI",0,pval);





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
