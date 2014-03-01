/* PAYROLL's - Make (PAYDIST1) Report */
#include <cstdio>
#include <rptgen.hpp>
/*COMPMEM 40960 */

extern void free_pvals(RDApval *,int);
extern void free_pvars(RDApvar *,int);
extern char *module;
void PAYDIST1(void);

void PAYDIST1()
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
	sprintf(rptname,"PAYDIST1");
	t=Rmalloc(39+1);
	sprintf(t,"Payroll Distribution Updating Datafiles");
	desc=Rmalloc(111+1);
	sprintf(desc,"This output definition is used to actually update the database files with the Payroll Distribution Information.");
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
	adddisplayADV(tmp,(double)8.500000,(double)11.000000,4,FALSE,0,"",0,1,0,pvars);
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

	t=Rmalloc(217+1);
	sprintf(t,"([PAYROLL][PAYJTRN][DELETEFLAG]=FALSE) AND \n([PAYROLL][PAYJTRN][SOURCE USER VERIFIED]=TRUE) AND \n([PAYROLL][PAYJTRN][DISTRIBUTED]=FALSE) AND \n([PAYROLL][PAYJTRN][PAY IDENTIFICATION]=[SELECTED PAY IDENT\
IFICATION])");
	x=addsearchfileADV(tmp,"PAYROLL","PAYJTRN",0,NULL,NULL,"RDA PAYJTRN KEY",FALSE,t,NULL,5);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PAY IDENTIFICATION","[SELECTED PAY IDENTIFICATION]");
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","");
		addsearchconfld(s,"JOB NUMBER","");
		addsearchconfld(s,"ACCOUNT NUMBER","");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(83+1);
	sprintf(t,"([PAYROLL][PAYJPMS][DELETEFLAG]=FALSE)\nAND ([PAYROLL][PAYJPMS][DISTRIBUTED]=FALSE)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYJPMS",2,"PAYROLL","PAYJTRN","PAYJPMS KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJTRN][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYJTRN][JOB NUMBER]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJTRN][PAY IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(36+1);
	sprintf(t,"([PRSNNL][PERDMG][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"PRSNNL","PERDMG",1,"PAYROLL","PAYJTRN","PERDMG KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJTRN][PERSONNEL IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(38+1);
	sprintf(t,"([PAYROLL][PAYMSTR][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYMSTR",2,"PAYROLL","PAYJTRN","PAYMSTR KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJTRN][PAY IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(136+1);
	sprintf(t,"([PAYROLL][PAYPPMS][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYPPMS][BENEFITS COMPUTED]=TRUE) AND ([PAYROLL][PAYPPMS][MADE TRANSACTIONS]=TRUE)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYPPMS",2,"PAYROLL","PAYJTRN","PAYPPMS KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJTRN][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJTRN][PAY IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	x=addsearchfileADV(tmp,"PAYROLL","PAYJMST",1,"PAYROLL","PAYJTRN","JOBMSTR KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJTRN][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYJTRN][JOB NUMBER]");
	}

	t=Rmalloc(36+1);
	sprintf(t,"([FINMGT][FINBYR][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"FINMGT","FINBYR",1,"PAYROLL","PAYJTRN","FINBYR KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[PAYJTRN][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
		addsearchconfld(s,"FISCAL YEAR","[FISCAL YEAR]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(37+1);
	sprintf(t,"([FINMGT][FINBACC][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"FINMGT","FINBACC",1,"PAYROLL","PAYJTRN","ACCOUNT CODE KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[PAYJTRN][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(37+1);
	sprintf(t,"([FINMGT][FINEACC][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"FINMGT","FINEYR",1,"PAYROLL","PAYJTRN","FINEYR MAIN KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[PAYJTRN][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
		addsearchconfld(s,"FISCAL YEAR","[FISCAL YEAR]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(37+1);
	sprintf(t,"([FINMGT][FINEACC][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"FINMGT","FINEACC",1,"PAYROLL","PAYJTRN","ACCOUNT CODE KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[PAYJTRN][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(217+1);
	sprintf(t,"([PAYROLL][PAYDTRN][DELETEFLAG]=FALSE) AND \n([PAYROLL][PAYDTRN][SOURCE USER VERIFIED]=TRUE) AND \n([PAYROLL][PAYDTRN][DISTRIBUTED]=FALSE) AND \n([PAYROLL][PAYDTRN][PAY IDENTIFICATION]=[SELECTED PAY IDENT\
IFICATION])");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDTRN",0,NULL,NULL,"RDA PAYDTRN KEY",FALSE,t,NULL,5);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PAY IDENTIFICATION","[SELECTED PAY IDENTIFICATION]");
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","");
		addsearchconfld(s,"JOB NUMBER","");
		addsearchconfld(s,"ACCOUNT NUMBER","");
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(38+1);
	sprintf(t,"([PAYROLL][PAYDPMS][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDPMS",2,"PAYROLL","PAYDTRN","PAYDPMS KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYDTRN][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","[PAYDTRN][DEDUCTION DESCRIPTION IDENTIFICATION]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYDTRN][PAY IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	x=addsearchfileADV(tmp,"PAYROLL","PAYDDMS",1,"PAYROLL","PAYDTRN","PAYDDMS KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","[PAYDTRN][DEDUCTION DESCRIPTION IDENTIFICATION]");
	}

	t=Rmalloc(36+1);
	sprintf(t,"([PRSNNL][PERDMG][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"PRSNNL","PERDMG",2,"PAYROLL","PAYDTRN","PERDMG KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYDTRN][PERSONNEL IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(36+1);
	sprintf(t,"[PAYROLL][PAYMSTR][DELETEFLAG]=FALSE");
	x=addsearchfileADV(tmp,"PAYROLL","PAYMSTR",1,"PAYROLL","PAYDTRN","PAYMSTR KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYDTRN][PAY IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(140+1);
	sprintf(t,"([PAYROLL][PAYPPMS][DELETEFLAG]=FALSE) AND \n([PAYROLL][PAYPPMS][BENEFITS COMPUTED]=TRUE) AND \n([PAYROLL][PAYPPMS][MADE TRANSACTIONS]=TRUE)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYPPMS",2,"PAYROLL","PAYDTRN","PAYPPMS KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYDTRN][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYDTRN][PAY IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	x=addsearchfileADV(tmp,"PAYROLL","PAYJMST",1,"PAYROLL","PAYDTRN","JOBMSTR KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYDTRN][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYDTRN][JOB NUMBER]");
	}

	t=Rmalloc(36+1);
	sprintf(t,"([FINMGT][FINBYR][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"FINMGT","FINBYR",1,"PAYROLL","PAYDTRN","FINBYR KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[PAYDTRN][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
		addsearchconfld(s,"FISCAL YEAR","[FISCAL YEAR]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(37+1);
	sprintf(t,"([FINMGT][FINBACC][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"FINMGT","FINBACC",1,"PAYROLL","PAYDTRN","ACCOUNT CODE KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[PAYDTRN][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(36+1);
	sprintf(t,"([FINMGT][FINEYR][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"FINMGT","FINEYR",1,"PAYROLL","PAYDTRN","FINEYR MAIN KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[PAYDTRN][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
		addsearchconfld(s,"FISCAL YEAR","[FISCAL YEAR]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(37+1);
	sprintf(t,"([FINMGT][FINEACC][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"FINMGT","FINEACC",1,"PAYROLL","PAYDTRN","ACCOUNT CODE KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[PAYDTRN][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
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
	t=Rmalloc(42+1);
	sprintf(t,"(\"EXECUTED BY: \"+[LOGIN IDENTIFICATION])");
	addvirtuallineADV(tmp,"EXECUTED BY",1,30,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(21+1);
	sprintf(t,"[PRIMARY FILE NUMBER]");
	addvirtuallineADV(tmp,"TRANSACTION TYPE",7,2,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(68+1);
	sprintf(t,"IF([WRITE COUNT]=1) \nTHEN RETURN_VALUE=0 \nELSE RETURN_VALUE=2 \nFI");
	addvirtuallineADV(tmp,"ACCOUNT TYPE",7,1,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(457+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) \nTHEN IF([WRITE COUNT]<4) \n     THEN RETURN_VALUE=[PAYROLL][PAYJTRN][DEFINITION TYPE] \n     ELSE RETURN_VALUE=[PAYROLL][PAYJTRN][PAYROLL FUND DEFINITION TYPE] \n     FI\nELSE \
IF([PRIMARY FILE NUMBER]=2) \n     THEN IF([WRITE COUNT]<4) \n          THEN RETURN_VALUE=[PAYROLL][PAYDTRN][DEFINITION TYPE] \n          ELSE RETURN_VALUE=[PAYROLL][PAYDTRN][PAYROLL FUND DEFINITION TYPE\
] \n          FI \nELSE RETURN_VALUE=0 FI FI");
	addvirtuallineADV(tmp,"DEFINITION TYPE",7,2,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(887+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) \nTHEN IF([WRITE COUNT]=1) \n       THEN RETURN_VALUE=[PAYROLL][PAYJTRN][ACCOUNT CODE] \n     ELSE IF([WRITE COUNT]=2) \n       THEN RETURN_VALUE=[PAYROLL][PAYJTRN][DEBIT CODE]\n\
     ELSE IF([WRITE COUNT]=3) \n       THEN RETURN_VALUE=[PAYROLL][PAYJTRN][PAYABLE CODE] \n     FI FI FI\nELSE IF([PRIMARY FILE NUMBER]=2) \n     THEN IF([WRITE COUNT]=1) THEN RETURN_VALUE=[PAYROLL][PAYD\
TRN][ACCOUNT CODE]\n	   ELSE IF([WRITE COUNT]=2)\n             THEN RETURN_VALUE=[PAYROLL][PAYDTRN][DEBIT CODE]\n           ELSE IF([WRITE COUNT]=3)\n             THEN RETURN_VALUE=[PAYROLL][PAYDTRN][CRED\
IT CODE]\n           ELSE IF([WRITE COUNT]=4) \n             THEN RETURN_VALUE=[PAYROLL][PAYDTRN][CASH CODE]\n           ELSE IF([WRITE COUNT]=5)\n             THEN RETURN_VALUE=[PAYROLL][PAYDTRN][LIABILI\
TY CODE]\n          FI FI FI FI FI\nELSE RETURN_VALUE=\"\" FI");
	addvirtuallineADV(tmp,"ACCOUNT CODE",1,40,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(127+1);
	sprintf(t,"IF([ACCOUNT TYPE]=0) \nTHEN RETURN_VALUE=[FINMGT][FINEACC][DESCRIPTION] \nELSE RETURN_VALUE=[FINMGT][FINBACC][DESCRIPTION] \nFI");
	addvirtuallineADV(tmp,"ACCOUNT CODE DESCRIPTION",1,60,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(31+1);
	sprintf(t,"[PAYROLL][PAYMSTR][FISCAL YEAR]");
	addvirtuallineADV(tmp,"FISCAL YEAR",8,4,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(253+1);
	sprintf(t,"IF(([PRIMARY FILE NUMBER]=1) AND ([WRITE COUNT]=1)) \nTHEN RETURN_VALUE=[PAYROLL][PAYJTRN][AMOUNT] \nELSE IF(([PRIMARY FILE NUMBER]=2) AND ([WRITE COUNT]=1)) \n     THEN RETURN_VALUE=[PAYROLL][PAYDTRN][AM\
OUNT] \n     ELSE RETURN_VALUE=0 \n     FI \nFI");
	addvirtuallineADV(tmp,"AMOUNT",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(268+1);
	sprintf(t,"IF([WRITE COUNT]=1) \nTHEN IF([PRIMARY FILE NUMBER]=1) \n       THEN RETURN_VALUE=[PAYROLL][PAYJTRN][AMOUNT] \n     ELSE IF([PRIMARY FILE NUMBER]=2) \n       THEN RETURN_VALUE=[PAYROLL][PAYDTRN][AMOUNT] \n \
    ELSE RETURN_VALUE=0 \n     FI FI\nELSE RETURN_VALUE=0 FI");
	addvirtuallineADV(tmp,"EXPENSE AMOUNT",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(286+1);
	sprintf(t,"IF(([PRIMARY FILE NUMBER]=1) AND (([WRITE COUNT]=2) OR ([WRITE COUNT]=4))) \n  THEN RETURN_VALUE=[PAYROLL][PAYJTRN][AMOUNT] \nELSE IF(([PRIMARY FILE NUMBER]=2) AND (([WRITE COUNT]=2) OR ([WRITE COUNT]=4)\
)) \n  THEN RETURN_VALUE=[PAYROLL][PAYDTRN][AMOUNT] \nELSE RETURN_VALUE=0 \nFI FI");
	addvirtuallineADV(tmp,"DEBIT AMOUNT",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(286+1);
	sprintf(t,"IF(([PRIMARY FILE NUMBER]=1) AND (([WRITE COUNT]=3) OR ([WRITE COUNT]=5))) \n  THEN RETURN_VALUE=[PAYROLL][PAYJTRN][AMOUNT] \nELSE IF(([PRIMARY FILE NUMBER]=2) AND (([WRITE COUNT]=3) OR ([WRITE COUNT]=5)\
)) \n  THEN RETURN_VALUE=[PAYROLL][PAYDTRN][AMOUNT] \nELSE RETURN_VALUE=0 \nFI FI");
	addvirtuallineADV(tmp,"CREDIT AMOUNT",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(217+1);
	sprintf(t,"IF([ACCOUNT TYPE]=0) \n  THEN RETURN_VALUE=\"EXPENDITURES\" \nELSE IF([ACCOUNT TYPE]=1) \n  THEN RETURN_VALUE=\"REVENUE\"\nELSE IF([ACCOUNT TYPE]=2) \n  THEN RETURN_VALUE=\"BALANCE SHEET\"\nELSE RETURN_VALUE=\"\" FI\
");
	addvirtuallineADV(tmp,"ACCOUNT TYPE DESCRIPTION",1,15,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(216+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) \n  THEN RETURN_VALUE=[PAYROLL][PAYJTRN][PAY IDENTIFICATION] \nELSE IF([PRIMARY FILE NUMBER]=2) \n  THEN RETURN_VALUE=[PAYROLL][PAYDTRN][PAY IDENTIFICATION] \nELSE RETURN_VALUE=\
0 \nFI FI");
	addvirtuallineADV(tmp,"PAY IDENTIFICATION",1,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(228+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) \n  THEN RETURN_VALUE=[PAYROLL][PAYJTRN][PERSONNEL IDENTIFICATION] \nELSE IF([PRIMARY FILE NUMBER]=2) \n  THEN RETURN_VALUE=[PAYROLL][PAYDTRN][PERSONNEL IDENTIFICATION] \nELSE R\
ETURN_VALUE=0 \nFI FI");
	addvirtuallineADV(tmp,"PERSONNEL IDENTIFICATION",1,25,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(198+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) \n  THEN RETURN_VALUE=[PAYROLL][PAYJTRN][JOB NUMBER] \nELSE IF([PRIMARY FILE NUMBER]=2) \n  THEN RETURN_VALUE=[PAYROLL][PAYDTRN][JOB NUMBER] \nELSE RETURN_VALUE=0 \nFI FI");
	addvirtuallineADV(tmp,"JOB NUMBER",8,6,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(197+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) \n  THEN RETURN_VALUE=\"\"\nELSE IF([PRIMARY FILE NUMBER]=2) \n  THEN RETURN_VALUE=[PAYROLL][PAYDTRN][DEDUCTION DESCRIPTION IDENTIFICATION] \nELSE RETURN_VALUE=0 \nFI FI");
	addvirtuallineADV(tmp,"IDENTIFICATION",1,30,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(208+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) \n  THEN RETURN_VALUE=[PAYROLL][PAYJTRN][ACCOUNT NUMBER] \nELSE IF([PRIMARY FILE NUMBER]=2) \n  THEN RETURN_VALUE=[PAYROLL][PAYDTRN][ACCOUNT NUMBER] \nELSE RETURN_VALUE=0 \nFI FI\
");
	addvirtuallineADV(tmp,"ACCOUNT NUMBER",8,4,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(102+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=2) THEN RETURN_VALUE=[PAYROLL][PAYDDMS][BENEFIT] \nELSE RETURN_VALUE=FALSE FI");
	addvirtuallineADV(tmp,"BENEFIT",10,1,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(110+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=2) THEN RETURN_VALUE=[PAYROLL][PAYDDMS][SHORT DESCRIPTION]\nELSE RETURN_VALUE=\"\" FI");
	addvirtuallineADV(tmp,"BENEFIT DESCRIPTION",1,40,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(21+1);
	sprintf(t,"[PRIMARY FILE NUMBER]");
	addvirtuallineADV(tmp,"WHICH TYPE",7,2,t,0,0);

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

	addsortline(tmp,0,"[ACCOUNT TYPE]");
	addsortline(tmp,0,"[DEFINITION TYPE]");
	addsortline(tmp,0,"[ACCOUNT CODE]");
	addsortline(tmp,0,"[ACCOUNT NUMBER]");
	addsortline(tmp,0,"[PERSONNEL IDENTIFICATION]");
	addsortline(tmp,0,"[IDENTIFICATION]");
	addaccumline(tmp,"LAST PAY IDENTIFICATION",3,"[PAY IDENTIFICATION]",0);
	addaccumline(tmp,"LAST WHICH TYPE",3,"[WHICH TYPE]",0);
	addaccumline(tmp,"LAST IDENTIFICATION",3,"[IDENTIFICATION]",0);
	addaccumline(tmp,"LAST JOB NUMBER",3,"[JOB NUMBER]",0);
	addaccumline(tmp,"TOTAL AMOUNT",7,"[AMOUNT]",0);
	addaccumline(tmp,"TOTAL CREDIT AMOUNT",7,"[CREDIT AMOUNT]",0);
	addaccumline(tmp,"TOTAL DEBIT AMOUNT",7,"[DEBIT AMOUNT]",0);
	addaccumline(tmp,"TOTAL EXPENSED AMOUNT",7,"[EXPENSE AMOUNT]",0);
	addaccumline(tmp,"LAST ACCOUNT TYPE",3,"[ACCOUNT TYPE]",0);
	addaccumline(tmp,"LAST ACCOUNT CODE",3,"[ACCOUNT CODE]",0);
	addaccumline(tmp,"LAST FISCAL YEAR",3,"[PAYROLL][PAYMSTR][FISCAL YEAR]",0);
	addaccumline(tmp,"LAST PROCESSING MONTH",3,"[PAYROLL][PAYMSTR][MONTH]",0);
	addaccumline(tmp,"LAST DEFINITION TYPE",3,"[DEFINITION TYPE]",0);
	addaccumline(tmp,"LAST ACCOUNT TYPE DESCRIPTION",3,"[ACCOUNT TYPE DESCRIPTION]",0);
	addaccumline(tmp,"LAST ACCOUNT CODE DESCRIPTION",3,"[ACCOUNT CODE DESCRIPTION]",0);
	addaccumline(tmp,"LAST ACCOUNT NUMBER",3,"[ACCOUNT NUMBER]",0);
	addaccumline(tmp,"LAST TRANSACTION TYPE",3,"[TRANSACTION TYPE]",0);
	addaccumline(tmp,"LAST BENEFIT",3,"[BENEFIT]",0);
	addaccumline(tmp,"LAST BENEFIT DESCRIPTION",3,"[BENEFIT DESCRIPTION]",0);
	addaccumline(tmp,"LAST PERSONNEL IDENTIFICATION",3,"[PERSONNEL IDENTIFICATION]",0);
	addaccumline(tmp,"LAST CALENDAR YEAR",3,"[PAYROLL][PAYMSTR][CALENDAR YEAR]",0);
	addcontrolline(tmp,"[ACCOUNT TYPE]",0);
	addcontrolline(tmp,"[DEFINITION TYPE]",0);
	addcontrolline(tmp,"[ACCOUNT CODE]",0);
	addcontrolline(tmp,"[PERSONNEL IDENTIFICATION]",0);
	addcontrolline(tmp,"[IDENTIFICATION]",0);




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

	addimageline(tmp,"REPORT HEADER","OPEN [FINMGT][FINEACT]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT HEADER","OPEN [FINMGT][FINEYR]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT HEADER","OPEN [FINMGT][FINBACT]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT HEADER","OPEN [FINMGT][FINBYR]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT HEADER","OPEN [PAYROLL][PAYJTRN]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT HEADER","OPEN [PAYROLL][PAYDTRN]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT HEADER","OPEN [BFTMGT][BFTTRAN]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT HEADER","OPEN [BFTMGT][BFTDDAC]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT HEADER","OPEN [BFTMGT][BFTMDAC]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT HEADER","OPEN [BFTMGT][BFTMSTR]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT HEADER","OPEN [BFTMGT][BFTDESC]",FALSE,NULL,0,pval);


	pval=Rmalloc(11*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST DEFINITION TYPE]");
	pvals->fieldname=stralloc("[BFTMGT][BFTTRAN][DEFINITION TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST PERSONNEL IDENTIFICATION]");
	pvals->fieldname=stralloc("[BFTMGT][BFTTRAN][PERSONNEL IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST IDENTIFICATION]");
	pvals->fieldname=stralloc("[BFTMGT][BFTTRAN][DEDUCTION DESCRIPTION IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("NEXT TRANSACTION NO");
	pvals->fieldname=stralloc("[BFTMGT][BFTTRAN][TRANSACTION NUMBER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL EXPENSED AMOUNT]");
	pvals->fieldname=stralloc("[BFTMGT][BFTTRAN][AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("FALSE");
	pvals->fieldname=stralloc("[BFTMGT][BFTTRAN][DELETEFLAG]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM DATE]");
	pvals->fieldname=stralloc("[BFTMGT][BFTTRAN][ENTRY DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM TIME]");
	pvals->fieldname=stralloc("[BFTMGT][BFTTRAN][ENTRY TIME]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST BENEFIT DESCRIPTION]");
	pvals->fieldname=stralloc("[BFTMGT][BFTTRAN][DESCRIPTION]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LOGIN IDENTIFICATION]");
	pvals->fieldname=stralloc("[BFTMGT][BFTTRAN][SOURCE USER]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("TRUE");
	pvals->fieldname=stralloc("[BFTMGT][BFTTRAN][SOURCE USER VERIFIED]");
	addimageline(tmp,"[IDENTIFICATION] FOOTER","WRITE [BFTMGT][BFTTRAN]",TRUE,"([LAST WHICH TYPE]=2) AND ([LAST ACCOUNT TYPE]=0) AND ([LAST BENEFIT]=TRUE)",11,pval);
	free_pvals(pval,11);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST DEFINITION TYPE]");
	pvals->fieldname=stralloc("[BFTMGT][BFTDDAC][DEFINITION TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST IDENTIFICATION]");
	pvals->fieldname=stralloc("[BFTMGT][BFTDDAC][DEDUCTION DESCRIPTION IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("([TOTAL EXPENSED AMOUNT]+[BFTMGT][BFTDDAC][UNPROCESSED BALANCE])");
	pvals->fieldname=stralloc("[BFTMGT][BFTDDAC][UNPROCESSED BALANCE]");
	addimageline(tmp,"[IDENTIFICATION] FOOTER","WRITE [BFTMGT][BFTDDAC]",TRUE,"([LAST WHICH TYPE]=2) AND ([LAST ACCOUNT TYPE]=0) AND \n([LAST BENEFIT]=TRUE) AND ([TOTAL EXPENSED AMOUNT]!=0)",3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST DEFINITION TYPE]");
	pvals->fieldname=stralloc("[BFTMGT][BFTMDAC][DEFINITION TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST PERSONNEL IDENTIFICATION]");
	pvals->fieldname=stralloc("[BFTMGT][BFTMDAC][PERSONNEL IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST IDENTIFICATION]");
	pvals->fieldname=stralloc("[BFTMGT][BFTMDAC][DEDUCTION DESCRIPTION IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("([TOTAL EXPENSED AMOUNT]+[BFTMGT][BFTMDAC][UNPROCESSED BALANCE])");
	pvals->fieldname=stralloc("[BFTMGT][BFTMDAC][UNPROCESSED BALANCE]");
	addimageline(tmp,"[IDENTIFICATION] FOOTER","WRITE [BFTMGT][BFTMDAC]",TRUE,"([LAST WHICH TYPE]=2) AND ([LAST ACCOUNT TYPE]=0) AND \n([LAST BENEFIT]=TRUE) AND ([TOTAL EXPENSED AMOUNT]!=0)",4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST PERSONNEL IDENTIFICATION]");
	pvals->fieldname=stralloc("[BFTMGT][BFTMSTR][PERSONNEL IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST IDENTIFICATION]");
	pvals->fieldname=stralloc("[BFTMGT][BFTMSTR][DEDUCTION DESCRIPTION IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("([TOTAL EXPENSED AMOUNT]+[BFTMGT][BFTMSTR][UNPROCESSED BALANCE])");
	pvals->fieldname=stralloc("[BFTMGT][BFTMSTR][UNPROCESSED BALANCE]");
	addimageline(tmp,"[IDENTIFICATION] FOOTER","WRITE [BFTMGT][BFTMSTR]",TRUE,"([LAST WHICH TYPE]=2) AND ([LAST ACCOUNT TYPE]=0) AND \n([LAST BENEFIT]=TRUE) AND ([TOTAL EXPENSED AMOUNT]!=0)",3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(2*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST IDENTIFICATION]");
	pvals->fieldname=stralloc("[BFTMGT][BFTDESC][DEDUCTION DESCRIPTION IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("([TOTAL EXPENSED AMOUNT]+[BFTMGT][BFTDESC][UNPROCESSED BALANCE])");
	pvals->fieldname=stralloc("[BFTMGT][BFTDESC][UNPROCESSED BALANCE]");
	addimageline(tmp,"[IDENTIFICATION] FOOTER","WRITE [BFTMGT][BFTDESC]",TRUE,"([LAST WHICH TYPE]=2) AND ([LAST ACCOUNT TYPE]=0) AND \n([LAST BENEFIT]=TRUE) AND ([TOTAL EXPENSED AMOUNT]!=0)",2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(13*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST DEFINITION TYPE]");
	pvals->fieldname=stralloc("[FINMGT][FINEACT][DEFINITION TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST ACCOUNT CODE]");
	pvals->fieldname=stralloc("[FINMGT][FINEACT][ACCOUNT CODE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST FISCAL YEAR]");
	pvals->fieldname=stralloc("[FINMGT][FINEACT][FISCAL YEAR]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("NEXT TRANSACTION NO");
	pvals->fieldname=stralloc("[FINMGT][FINEACT][TRANSACTION NUMBER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL EXPENSED AMOUNT]");
	pvals->fieldname=stralloc("[FINMGT][FINEACT][AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("FALSE");
	pvals->fieldname=stralloc("[FINMGT][FINEACT][DELETEFLAG]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM DATE]");
	pvals->fieldname=stralloc("[FINMGT][FINEACT][ENTRY DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM TIME]");
	pvals->fieldname=stralloc("[FINMGT][FINEACT][ENTRY TIME]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("([LAST PROCESSING MONTH]+1)");
	pvals->fieldname=stralloc("[FINMGT][FINEACT][PROCESSING MONTH]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("\"PAYROLL\"");
	pvals->fieldname=stralloc("[FINMGT][FINEACT][SOURCE MODULE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("\"PAYDIST\"");
	pvals->fieldname=stralloc("[FINMGT][FINEACT][SOURCE PROCESS]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LOGIN IDENTIFICATION]");
	pvals->fieldname=stralloc("[FINMGT][FINEACT][SOURCE USER]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("0");
	pvals->fieldname=stralloc("[FINMGT][FINEACT][TYPE]");
	addimageline(tmp,"[ACCOUNT CODE] FOOTER","WRITE [FINMGT][FINEACT]",TRUE,"[LAST ACCOUNT TYPE]=0",13,pval);
	free_pvals(pval,13);


	pval=Rmalloc(13*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST DEFINITION TYPE]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][DEFINITION TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST ACCOUNT CODE]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][ACCOUNT CODE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST FISCAL YEAR]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][FISCAL YEAR]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("NEXT TRANSACTION NO");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][TRANSACTION NUMBER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL DEBIT AMOUNT]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("FALSE");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][DELETEFLAG]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM DATE]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][ENTRY DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM TIME]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][ENTRY TIME]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("([LAST PROCESSING MONTH]+1)");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][PROCESSING MONTH]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("\"PAYROLL\"");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][SOURCE MODULE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("\"PAYDIST\"");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][SOURCE PROCESS]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LOGIN IDENTIFICATION]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][SOURCE USER]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("0");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][TYPE]");
	addimageline(tmp,"[ACCOUNT CODE] FOOTER","WRITE [FINMGT][FINBACT]",TRUE,"([LAST ACCOUNT TYPE]=2) AND ([TOTAL DEBIT AMOUNT]!=0)",13,pval);
	free_pvals(pval,13);


	pval=Rmalloc(13*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST DEFINITION TYPE]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][DEFINITION TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST ACCOUNT CODE]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][ACCOUNT CODE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST FISCAL YEAR]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][FISCAL YEAR]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("NEXT TRANSACTION NO");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][TRANSACTION NUMBER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL CREDIT AMOUNT]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][AMOUNT]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("FALSE");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][DELETEFLAG]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM DATE]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][ENTRY DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM TIME]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][ENTRY TIME]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("([LAST PROCESSING MONTH]+1)");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][PROCESSING MONTH]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("\"PAYROLL\"");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][SOURCE MODULE]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("\"PAYDIST\"");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][SOURCE PROCESS]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LOGIN IDENTIFICATION]");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][SOURCE USER]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("1");
	pvals->fieldname=stralloc("[FINMGT][FINBACT][TYPE]");
	addimageline(tmp,"[ACCOUNT CODE] FOOTER","WRITE [FINMGT][FINBACT]",TRUE,"([LAST ACCOUNT TYPE]=2) AND ([TOTAL CREDIT AMOUNT]!=0)",13,pval);
	free_pvals(pval,13);


	pval=Rmalloc(27*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST DEFINITION TYPE]");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][DEFINITION TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST ACCOUNT CODE]");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][ACCOUNT CODE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST FISCAL YEAR]");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][FISCAL YEAR]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=0) THEN RETURN_VALUE=[FINMGT][FINEYR][JANUARY UNDISTRIBUTED EXPENDITURES]-[TOTAL EXPENSED AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINEYR][JANUARY UNDISTRIBUTED EXPENDITURES] FI");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][JANUARY UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=0) THEN RETURN_VALUE=[FINMGT][FINEYR][JANUARY DISTRIBUTED EXPENDITURES]+[TOTAL EXPENSED AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINEYR][JANUARY DISTRIBUTED EXPENDITURES] FI");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][JANUARY DISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=1) THEN RETURN_VALUE=[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED EXPENDITURES]-[TOTAL EXPENSED AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED AMOUNT] FI");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][FEBRUARY UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=1) THEN RETURN_VALUE=[FINMGT][FINEYR][FEBRUARY DISTRIBUTED EXPENDITURES]+[TOTAL EXPENSED AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINEYR][FEBRUARY DISTRIBUTED EXPENDITURES] FI");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][FEBRUARY DISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=2) THEN RETURN_VALUE=[FINMGT][FINEYR][MARCH UNDISTRIBUTED EXPENDITURES]-[TOTAL EXPENSED AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINEYR][MARCH UNDISTRIBUTED EXPENDITURES] FI");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][MARCH UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=2) THEN RETURN_VALUE=[FINMGT][FINEYR][MARCH DISTRIBUTED EXPENDITURES]+[TOTAL EXPENSED AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINEYR][MARCH DISTRIBUTED EXPENDITURES] FI");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][MARCH DISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=3) THEN RETURN_VALUE=[FINMGT][FINEYR][APRIL UNDISTRIBUTED EXPENDITURES]-[TOTAL EXPENSED AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINEYR][APRIL UNDISTRIBUTED EXPENDITURES] FI");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][APRIL UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=3) THEN RETURN_VALUE=[FINMGT][FINEYR][APRIL DISTRIBUTED EXPENDITURES]+[TOTAL EXPENSED AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINEYR][APRIL DISTRIBUTED EXPENDITURES] FI");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][APRIL DISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=4) THEN RETURN_VALUE=[FINMGT][FINEYR][MAY UNDISTRIBUTED EXPENDITURES]-[TOTAL EXPENSED AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINEYR][MAY UNDISTRIBUTED EXPENDITURES] FI");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][MAY UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=4) THEN RETURN_VALUE=[FINMGT][FINEYR][MAY DISTRIBUTED EXPENDITURES]+[TOTAL EXPENSED AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINEYR][MAY DISTRIBUTED EXPENDITURES] IF");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][MAY DISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=5) THEN RETURN_VALUE=[FINMGT][FINEYR][JUNE UNDISTRIBUTED EXPENDITURES]-[TOTAL EXPENSED AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINEYR][JUNE UNDISTRIBUTED EXPENDITURES] FI");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][JUNE UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=5) THEN RETURN_VALUE=[FINMGT][FINEYR][JUNE DISTRIBUTED EXPENDITURES]+[TOTAL EXPENSED AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINEYR][JUNE DISTRIBUTED EXPENDITURES] FI");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][JUNE DISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=6) THEN RETURN_VALUE=[FINMGT][FINEYR][JULY UNDISTRIBUTED EXPENDITURES]-[TOTAL EXPENSED AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINEYR][JULY UNDISTRIBUTED EXPENDITURES] FI");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][JULY UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=6) THEN RETURN_VALUE=[FINMGT][FINEYR][JULY DISTRIBUTED EXPENDITURES]+[TOTAL EXPENSED AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINEYR][JULY DISTRIBUTED EXPENDITURES] FI");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][JULY DISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=7) THEN RETURN_VALUE=[FINMGT][FINEYR][AUGUST UNDISTRIBUTED EXPENDITURES]-[TOTAL EXPENSED AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINEYR][AUGUST UNDISTRIBUTED EXPENDITURES] FI");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][AUGUST UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=7) THEN RETURN_VALUE=[FINMGT][FINEYR][AUGUST DISTRIBUTED EXPENDITURES]+[TOTAL EXPENSED AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINEYR][AUGUST DISTRIBUTED EXPENDITURES] FI");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][AUGUST DISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=8) THEN RETURN_VALUE=[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED EXPENDITURES]-[TOTAL EXPENSED AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED EXPENDITURES] FI");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][SEPTEMBER UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=8) THEN RETURN_VALUE=[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED EXPENDITURES]+[TOTAL EXPENSED AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED EXPENDITURES] FI");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][SEPTEMBER DISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=9) THEN RETURN_VALUE=[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED EXPENDITURES]-[TOTAL EXPENSED AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED EXPENDITURES] FI");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][OCTOBER UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=9) THEN RETURN_VALUE=[FINMGT][FINEYR][OCTOBER DISTRIBUTED EXPENDITURES]+[TOTAL EXPENSED AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINEYR][OCTOBER DISTRIBUTED EXPENDITURES] FI");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][OCTOBER DISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=10) THEN RETURN_VALUE=[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED EXPENDITURES]-[TOTAL EXPENSED AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED EXPENDITURES] FI");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][NOVEMBER UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=10) THEN RETURN_VALUE=[FINMGT][FINEYR][NOVEMBER DISTRIBUTED EXPENDITURES]+[TOTAL EXPENSED AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINEYR][NOVEMBER DISTRIBUTED EXPENDITURES] FI");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][NOVEMBER DISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=11) THEN RETURN_VALUE=[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED EXPENDITURES]-[TOTAL EXPENSED AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED EXPENDITURES] FI");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][DECEMBER UNDISTRIBUTED EXPENDITURES]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=11) THEN RETURN_VALUE=[FINMGT][FINEYR][DECEMBER DISTRIBUTED EXPENDITURES]+[TOTAL EXPENSED AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINEYR][DECEMBER DISTRIBUTED EXPENDITURES] FI");
	pvals->fieldname=stralloc("[FINMGT][FINEYR][DECEMBER DISTRIBUTED EXPENDITURES]");
	addimageline(tmp,"[ACCOUNT CODE] FOOTER","WRITE [FINMGT][FINEYR]",TRUE,"[LAST ACCOUNT TYPE]=0",27,pval);
	free_pvals(pval,27);


	pval=Rmalloc(27*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST DEFINITION TYPE]");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][DEFINITION TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST ACCOUNT CODE]");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][ACCOUNT CODE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST FISCAL YEAR]");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][FISCAL YEAR]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=0) THEN RETURN_VALUE=[FINMGT][FINBYR][JANUARY UNDISTRIBUTED DEBITS]-[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][JANUARY UNDISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JANUARY UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=0) THEN RETURN_VALUE=[FINMGT][FINBYR][JANUARY DISTRIBUTED DEBITS]+[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][JANUARY DISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JANUARY DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=1) THEN RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED DEBITS]-[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=1) THEN RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY DISTRIBUTED DEBITS]+[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY DISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][FEBRUARY DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=2) THEN RETURN_VALUE=[FINMGT][FINBYR][MARCH UNDISTRIBUTED DEBITS]-[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][MARCH UNDISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][MARCH UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=2) THEN RETURN_VALUE=[FINMGT][FINBYR][MARCH DISTRIBUTED DEBITS]+[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][MARCH DISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][MARCH DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=3) THEN RETURN_VALUE=[FINMGT][FINBYR][APRIL UNDISTRIBUTED DEBITS]-[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][APRIL UNDISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][APRIL UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=3) THEN RETURN_VALUE=[FINMGT][FINBYR][APRIL DISTRIBUTED DEBITS]+[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][APRIL DISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][APRIL DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=4) THEN RETURN_VALUE=[FINMGT][FINBYR][MAY UNDISTRIBUTED DEBITS]-[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][MAY UNDISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][MAY UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=4) THEN RETURN_VALUE=[FINMGT][FINBYR][MAY DISTRIBUTED DEBITS]+[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][MAY DISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][MAY DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=5) THEN RETURN_VALUE=[FINMGT][FINBYR][JUNE UNDISTRIBUTED DEBITS]-[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][JUNE UNDISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JUNE UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=5) THEN RETURN_VALUE=[FINMGT][FINBYR][JUNE DISTRIBUTED DEBITS]+[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][JUNE DISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JUNE DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=6) THEN RETURN_VALUE=[FINMGT][FINBYR][JULY UNDISTRIBUTED DEBITS]-[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][JULY UNDISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JULY UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=6) THEN RETURN_VALUE=[FINMGT][FINBYR][JULY DISTRIBUTED DEBITS]+[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][JULY DISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JULY DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=7) THEN RETURN_VALUE=[FINMGT][FINBYR][AUGUST UNDISTRIBUTED DEBITS]-[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][AUGUST UNDISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][AUGUST UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=7) THEN RETURN_VALUE=[FINMGT][FINBYR][AUGUST DISTRIBUTED DEBITS]+[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][AUGUST DISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][AUGUST DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=8) THEN RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED DEBITS]-[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=8) THEN RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER DISTRIBUTED DEBITS]+[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER DISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][SEPTEMBER DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=9) THEN RETURN_VALUE=[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED DEBITS]-[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=9) THEN RETURN_VALUE=[FINMGT][FINBYR][OCTOBER DISTRIBUTED DEBITS]+[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][OCTOBER DISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][OCTOBER DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=10) THEN RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED DEBITS]-[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=10) THEN RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER DISTRIBUTED DEBITS]+[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER DISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][NOVEMBER DISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=11) THEN RETURN_VALUE=[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED DEBITS]-[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED DEBITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=11) THEN RETURN_VALUE=[FINMGT][FINBYR][DECEMBER DISTRIBUTED DEBITS]+[TOTAL DEBIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][DECEMBER DISTRIBUTED DEBITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][DECEMBER DISTRIBUTED DEBITS]");
	addimageline(tmp,"[ACCOUNT CODE] FOOTER","WRITE [FINMGT][FINBYR]",TRUE,"([LAST ACCOUNT TYPE]=2) AND ([TOTAL DEBIT AMOUNT]!=0)",27,pval);
	free_pvals(pval,27);


	pval=Rmalloc(27*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST DEFINITION TYPE]");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][DEFINITION TYPE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST ACCOUNT CODE]");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][ACCOUNT CODE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST FISCAL YEAR]");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][FISCAL YEAR]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=0) THEN RETURN_VALUE=[FINMGT][FINBYR][JANUARY UNDISTRIBUTED CREDITS]-[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][JANUARY UNDISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JANUARY UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=0) THEN RETURN_VALUE=[FINMGT][FINBYR][JANUARY DISTRIBUTED CREDITS]+[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][JANUARY DISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JANUARY DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=1) THEN RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED CREDITS]-[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][FEBRUARY UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=1) THEN RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY DISTRIBUTED CREDITS]+[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][FEBRUARY DISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][FEBRUARY DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=2) THEN RETURN_VALUE=[FINMGT][FINBYR][MARCH UNDISTRIBUTED CREDITS]-[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][MARCH UNDISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][MARCH UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=2) THEN RETURN_VALUE=[FINMGT][FINBYR][MARCH DISTRIBUTED CREDITS]+[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][MARCH DISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][MARCH DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=3) THEN RETURN_VALUE=[FINMGT][FINBYR][APRIL UNDISTRIBUTED CREDITS]-[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][APRIL UNDISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][APRIL UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=3) THEN RETURN_VALUE=[FINMGT][FINBYR][APRIL DISTRIBUTED CREDITS]+[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][APRIL DISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][APRIL DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=4) THEN RETURN_VALUE=[FINMGT][FINBYR][MAY UNDISTRIBUTED CREDITS]-[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][MAY UNDISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][MAY UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=4) THEN RETURN_VALUE=[FINMGT][FINBYR][MAY DISTRIBUTED CREDITS]+[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][MAY DISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][MAY DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=5) THEN RETURN_VALUE=[FINMGT][FINBYR][JUNE UNDISTRIBUTED CREDITS]-[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][JUNE UNDISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JUNE UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=5) THEN RETURN_VALUE=[FINMGT][FINBYR][JUNE DISTRIBUTED CREDITS]+[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][JUNE DISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JUNE DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=6) THEN RETURN_VALUE=[FINMGT][FINBYR][JULY UNDISTRIBUTED CREDITS]-[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][JULY UNDISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JULY UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=6) THEN RETURN_VALUE=[FINMGT][FINBYR][JULY DISTRIBUTED CREDITS]+[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][JULY DISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][JULY DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=7) THEN RETURN_VALUE=[FINMGT][FINBYR][AUGUST UNDISTRIBUTED CREDITS]-[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][AUGUST UNDISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][AUGUST UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=7) THEN RETURN_VALUE=[FINMGT][FINBYR][AUGUST DISTRIBUTED CREDITS]+[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][AUGUST DISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][AUGUST DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=8) THEN RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED CREDITS]-[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][SEPTEMBER UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=8) THEN RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER DISTRIBUTED CREDITS]+[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][SEPTEMBER DISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][SEPTEMBER DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=9) THEN RETURN_VALUE=[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED CREDITS]-[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][OCTOBER UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=9) THEN RETURN_VALUE=[FINMGT][FINBYR][OCTOBER DISTRIBUTED CREDITS]+[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][OCTOBER DISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][OCTOBER DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=10) THEN RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED CREDITS]-[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][NOVEMBER UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=10) THEN RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER DISTRIBUTED CREDITS]+[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][NOVEMBER DISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][NOVEMBER DISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=11) THEN RETURN_VALUE=[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED CREDITS]-[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][DECEMBER UNDISTRIBUTED CREDITS]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("IF([LAST PROCESSING MONTH]=11) THEN RETURN_VALUE=[FINMGT][FINBYR][DECEMBER DISTRIBUTED CREDITS]+[TOTAL CREDIT AMOUNT] ELSE RETURN_VALUE=[FINMGT][FINBYR][DECEMBER DISTRIBUTED CREDITS] FI");
	pvals->fieldname=stralloc("[FINMGT][FINBYR][DECEMBER DISTRIBUTED CREDITS]");
	addimageline(tmp,"[ACCOUNT CODE] FOOTER","WRITE [FINMGT][FINBYR]",TRUE,"([LAST ACCOUNT TYPE]=2) AND ([TOTAL CREDIT AMOUNT]!=0)",27,pval);
	free_pvals(pval,27);


	pval=Rmalloc(8*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAY IDENTIFICATION]");
	pvals->fieldname=stralloc("[PAYROLL][PAYJTRN][PAY IDENTIFICATION]");
	++pvals;
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
	pvals->name=stralloc("[ACCOUNT NUMBER]");
	pvals->fieldname=stralloc("[PAYROLL][PAYJTRN][ACCOUNT NUMBER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LOGIN IDENTIFICATION]");
	pvals->fieldname=stralloc("[PAYROLL][PAYJTRN][DISTRIBUTED BY]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM DATE]");
	pvals->fieldname=stralloc("[PAYROLL][PAYJTRN][DISTRIBUTED DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM TIME]");
	pvals->fieldname=stralloc("[PAYROLL][PAYJTRN][DISTRIBUTED TIME]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("TRUE");
	pvals->fieldname=stralloc("[PAYROLL][PAYJTRN][DISTRIBUTED]");
	addimageline(tmp,"DETAIL LINES","WRITE [PAYROLL][PAYJTRN]",TRUE,"[WHICH TYPE]=1",8,pval);
	free_pvals(pval,8);


	pval=Rmalloc(9*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAY IDENTIFICATION]");
	pvals->fieldname=stralloc("[PAYROLL][PAYDTRN][PAY IDENTIFICATION]");
	++pvals;
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
	pvals->name=stralloc("[IDENTIFICATION]");
	pvals->fieldname=stralloc("[PAYROLL][PAYDTRN][DEDUCTION DESCRIPTION IDENTIFICATION]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ACCOUNT NUMBER]");
	pvals->fieldname=stralloc("[PAYROLL][PAYDTRN][ACCOUNT NUMBER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LOGIN IDENTIFICATION]");
	pvals->fieldname=stralloc("[PAYROLL][PAYDTRN][DISTRIBUTED BY]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM DATE]");
	pvals->fieldname=stralloc("[PAYROLL][PAYDTRN][DISTRIBUTED DATE]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SYSTEM TIME]");
	pvals->fieldname=stralloc("[PAYROLL][PAYDTRN][DISTRIBUTED TIME]");
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("TRUE");
	pvals->fieldname=stralloc("[PAYROLL][PAYDTRN][DISTRIBUTED]");
	addimageline(tmp,"DETAIL LINES","WRITE [PAYROLL][PAYDTRN]",TRUE,"[WHICH TYPE]=2",9,pval);
	free_pvals(pval,9);


	addimageline(tmp,"REPORT FOOTER","CLOSE [FINMGT][FINEACT]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT FOOTER","CLOSE [FINMGT][FINEYR]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT FOOTER","CLOSE [FINMGT][FINBACT]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT FOOTER","CLOSE [FINMGT][FINBYR]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT FOOTER","CLOSE [PAYROLL][PAYJTRN]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT FOOTER","CLOSE [PAYROLL][PAYDTRN]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT FOOTER","CLOSE [BFTMGT][BFTTRAN]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT FOOTER","CLOSE [BFTMGT][BFTDDAC]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT FOOTER","CLOSE [BFTMGT][BFTMDAC]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT FOOTER","CLOSE [BFTMGT][BFTDESC]",FALSE,NULL,0,pval);


	addimageline(tmp,"REPORT FOOTER","CLOSE [BFTMGT][BFTMSTR]",FALSE,NULL,0,pval);






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
