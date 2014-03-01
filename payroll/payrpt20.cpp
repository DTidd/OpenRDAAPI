/* PAYROLL's - Make (PAYDSTE) Report */
#include <cstdio>
#include <rptgen.hpp>
/*COMPMEM 40960 */

extern void free_pvals(RDApval *,int);
extern void free_pvars(RDApvar *,int);
extern char *module;
void PAYDSTE(void);
static void PAYDSTE_RANGE_SCREEN(void);

void PAYDSTE()
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
	char temp[15000];
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
	rptname=Rmalloc(7+1);
	sprintf(rptname,"PAYDSTE");
	t=Rmalloc(20+1);
	sprintf(t,"Payroll Distribution");
	desc=Rmalloc(286+1);
	sprintf(desc,"This output definition is used to print the Payroll Distribution report for verification before actually updating the database(s).  This report is configured for use when Dis-Encumbering Payroll.  PAYDIST*.RPT should be used when not dis-encumbering, since the assumption can save time.");
	tmp=RDAreportNEWADV(rptname,t,desc,1,"",FALSE,TRUE,FALSE,TRUE,TRUE,"PAYROLL","PAYDSTE1",TRUE,1,FALSE,"");
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
	adddisplayADV(tmp,(double)8.500000,(double)10.000000,0,FALSE,0,"",0,1,0,pvars);
	free_pvars(pvars,0);

/*----------------------------------------------------------------------*/




	memset(temp,0,15000);
	sprintf(temp,"%s%s%s%s%s%s",
		"IF([ACCOUNT CODE]=\"\") THEN RETURN_VALUE=FALSE\n ",
		"ELSE IF([WRITE COUNT]<6) THEN RETURN_VALUE=TRUE\n ",
		"ELSE IF(([PRIMARY FILE NUMBER]=1)) THEN IF(([PAYROLL][PAYJAMS][DISENCUMBER]=TRUE) AND ([PAYROLL][PAYJTRN][ENCUMBRANCE CODE]!=\"\") AND ([PAYROLL][PAYJTRN][RESERVE CODE]!=\"\")) THEN RETURN_VALUE=TRUE ",
		"ELSE RETURN_VALUE=FALSE FI\n ",
		"ELSE IF(([PRIMARY FILE NUMBER]=2)) THEN IF((([PAYROLL][PAYDDMS][DISENCUMBER]=TRUE) AND ([PAYROLL][PAYDEDM][DISENCUMBER]=TRUE)) AND ([PAYROLL][PAYDTRN][ENCUMBRANCE CODE]!=\"\") AND ([PAYROLL][PAYDTRN][RESERVE CODE]!=\"\")) THEN RETURN_VALUE=TRUE ELSE RETURN_VALUE=FALSE FI\n ",
		"ELSE RETURN_VALUE=FALSE FI FI FI FI");

	tmp->select=stralloc(temp);
	memset(temp,0,15000);


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

	t=Rmalloc(215+1);
	sprintf(t,"([PAYROLL][PAYJTRN][DELETEFLAG]=FALSE) AND \n([PAYROLL][PAYJTRN][SOURCE USER VERIFIED]=TRUE) AND ([PAYROLL][PAYJTRN][DISTRIBUTED]=FALSE) AND \n([PAYROLL][PAYJTRN][PAY IDENTIFICATION]=[SELECTED PAY IDENTI\
FICATION])");
	x=addsearchfileADV(tmp,"PAYROLL","PAYJTRN",0,NULL,NULL,"RDA PAYJTRN KEY",FALSE,t,NULL,8);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PAY IDENTIFICATION","[SELECTED PAY IDENTIFICATION]");
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","");
		addsearchconfld(s,"JOB NUMBER","");
		addsearchconfld(s,"ACCOUNT NUMBER","");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(38+1);
	sprintf(t,"([PAYROLL][PAYJPMS][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYJPMS",2,"PAYROLL","PAYJTRN","PAYJPMS KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJTRN][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYJTRN][JOB NUMBER]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJTRN][PAY IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(34+1);
	sprintf(t,"[PRSNNL][PERDMG][DELETEFLAG]=FALSE");
	x=addsearchfileADV(tmp,"PRSNNL","PERDMG",1,"PAYROLL","PAYJTRN","PERDMG KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJTRN][PERSONNEL IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	x=addsearchfileADV(tmp,"PAYROLL","PAYJAMS",1,"PAYROLL","PAYJTRN","PAYJAMS KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJTRN][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYJTRN][JOB NUMBER]");
		addsearchconfld(s,"ACCOUNT NUMBER","[PAYJTRN][ACCOUNT NUMBER]");
	}

	t=Rmalloc(36+1);
	sprintf(t,"[PAYROLL][PAYMSTR][DELETEFLAG]=FALSE");
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

	x=addsearchfileADV(tmp,"FINMGT","FINBYR",1,"PAYROLL","PAYJTRN","FINBYR KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[PAYJTRN][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
		addsearchconfld(s,"FISCAL YEAR","[FISCAL YEAR]");
	}

	x=addsearchfileADV(tmp,"FINMGT","FINBACC",1,"PAYROLL","PAYJTRN","ACCOUNT CODE KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[PAYJTRN][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
	}

	x=addsearchfileADV(tmp,"FINMGT","FINEYR",1,"PAYROLL","PAYJTRN","FINEYR MAIN KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[PAYJTRN][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
		addsearchconfld(s,"FISCAL YEAR","[FISCAL YEAR]");
	}

	x=addsearchfileADV(tmp,"FINMGT","FINEACC",1,"PAYROLL","PAYJTRN","ACCOUNT CODE KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEFINITION TYPE","[PAYJTRN][DEFINITION TYPE]");
		addsearchconfld(s,"ACCOUNT CODE","[ACCOUNT CODE]");
	}

	t=Rmalloc(257+1);
	sprintf(t,"([PAYROLL][PAYDTRN][DELETEFLAG]=FALSE) AND \n([PAYROLL][PAYDTRN][EXPENSED]=TRUE) AND \n([PAYROLL][PAYDTRN][SOURCE USER VERIFIED]=TRUE) AND ([PAYROLL][PAYDTRN][DISTRIBUTED]=FALSE) AND \n([PAYROLL][PAYDTRN]\
[PAY IDENTIFICATION]=[SELECTED PAY IDENTIFICATION])");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDTRN",0,NULL,NULL,"RDA PAYDTRN KEY",FALSE,t,NULL,8);
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

	x=addsearchfileADV(tmp,"PAYROLL","PAYDEDM",1,"PAYROLL","PAYDTRN","PAYDEDM KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYDTRN][PERSONNEL IDENTIFICATION]");
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

	t=Rmalloc(136+1);
	sprintf(t,"([PAYROLL][PAYPPMS][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYPPMS][BENEFITS COMPUTED]=TRUE) AND ([PAYROLL][PAYPPMS][MADE TRANSACTIONS]=TRUE)");
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
	t=Rmalloc(32+1);
	sprintf(t,"[PAYROLL][PAYMSTR][FISCAL MONTH]");
	addvirtuallineADV(tmp,"FISCAL PROCESSING MONTH",7,2,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(1039+1);
	sprintf(t,"IF([FISCAL PROCESSING MONTH]=0) THEN RETURN_VALUE=\"PRE-YEAR\" \nELSE IF([FISCAL PROCESSING MONTH]=1) THEN RETURN_VALUE=\"JANUARY\" \nELSE IF([FISCAL PROCESSING MONTH]=2) THEN RETURN_VALUE=\"FEBRUARY\" \nELSE I\
F([FISCAL PROCESSING MONTH]=3) THEN RETURN_VALUE=\"MARCH\" \nELSE IF([FISCAL PROCESSING MONTH]=4) THEN RETURN_VALUE=\"APRIL\" \nELSE IF([FISCAL PROCESSING MONTH]=5) THEN RETURN_VALUE=\"MAY\" \nELSE IF([FISCAL \
PROCESSING MONTH]=6) THEN RETURN_VALUE=\"JUNE\" \nELSE IF([FISCAL PROCESSING MONTH]=7) THEN RETURN_VALUE=\"JULY\" \nELSE IF([FISCAL PROCESSING MONTH]=8) THEN RETURN_VALUE=\"AUGUST\" \nELSE IF([FISCAL PROCESSIN\
G MONTH]=9) THEN RETURN_VALUE=\"SEPTEMBER\" \nELSE IF([FISCAL PROCESSING MONTH]=10) THEN RETURN_VALUE=\"OCTOBER\" \nELSE IF([FISCAL PROCESSING MONTH]=11) THEN RETURN_VALUE=\"NOVEMBER\" \nELSE IF([FISCAL PROCES\
SING MONTH]=12) THEN RETURN_VALUE=\"DECEMBER\" \nELSE IF([FISCAL PROCESSING MONTH]=13) THEN RETURN_VALUE=\"POST YEAR\" \nELSE RETURN_VALUE=\"UNKNOWN\" \nFI FI FI FI FI FI FI FI FI FI FI FI FI FI");
	addvirtuallineADV(tmp,"FISCAL PROCESSING MONTH STRING",1,12,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(42+1);
	sprintf(t,"(\"EXECUTED BY: \"+[LOGIN IDENTIFICATION])");
	addvirtuallineADV(tmp,"EXECUTED BY",1,30,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(21+1);
	sprintf(t,"[PRIMARY FILE NUMBER]");
	addvirtuallineADV(tmp,"TRANSACTION TYPE",7,2,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(90+1);
	sprintf(t,"IF(([WRITE COUNT]=1) OR ([WRITE COUNT]=6))\nTHEN RETURN_VALUE=0 \nELSE RETURN_VALUE=2 \nFI");
	addvirtuallineADV(tmp,"ACCOUNT TYPE",7,1,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(389+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) THEN \nIF(([WRITE COUNT]<4) OR ([WRITE COUNT]>5))\nTHEN RETURN_VALUE=[PAYROLL][PAYJTRN][DEFINITION TYPE] \nELSE RETURN_VALUE=[PAYROLL][PAYJTRN][PAYROLL FUND DEFINITION TYPE] \nF\
I\nELSE \nIF(([WRITE COUNT]<4) OR ([WRITE COUNT]>5)) THEN\nRETURN_VALUE=[PAYROLL][PAYDTRN][DEFINITION TYPE]\nELSE RETURN_VALUE=[PAYROLL][PAYDTRN][PAYROLL FUND DEFINITION TYPE] FI\nFI");
	addvirtuallineADV(tmp,"DEFINITION TYPE",7,2,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(1562+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) \nTHEN IF([WRITE COUNT]=1) \n       THEN RETURN_VALUE=[PAYROLL][PAYJTRN][ACCOUNT CODE] \n     ELSE IF([WRITE COUNT]=2) \n       THEN RETURN_VALUE=[PAYROLL][PAYJTRN][DEBIT CODE] \
\n     ELSE IF([WRITE COUNT]=3) \n       THEN RETURN_VALUE=[PAYROLL][PAYJTRN][CREDIT CODE]\n     ELSE IF([WRITE COUNT]=4)\n       THEN RETURN_VALUE=[PAYROLL][PAYJTRN][CASH CODE]\n     ELSE IF([WRITE COUNT]\
=5)\n       THEN RETURN_VALUE=[PAYROLL][PAYJTRN][PAYABLE CODE]\n     ELSE IF([WRITE COUNT]=6)\n	THEN RETURN_VALUE=[PAYROLL][PAYJTRN][ACCOUNT CODE]\n     ELSE IF([WRITE COUNT]=7)\n	THEN RETURN_VALUE=[PAYROL\
L][PAYJTRN][ENCUMBRANCE CODE]\n     ELSE IF([WRITE COUNT]=8)\n	THEN RETURN_VALUE=[PAYROLL][PAYJTRN][RESERVE CODE] \n     FI FI FI FI FI FI FI FI\nELSE IF([PRIMARY FILE NUMBER]=2) \nTHEN IF([WRITE COUNT]=1)\
 \n     THEN RETURN_VALUE=[PAYROLL][PAYDTRN][ACCOUNT CODE] \n     ELSE IF([WRITE COUNT]=2)\n       THEN RETURN_VALUE=[PAYROLL][PAYDTRN][DEBIT CODE]\n     ELSE IF([WRITE COUNT]=3)\n       THEN RETURN_VALUE=\
[PAYROLL][PAYDTRN][CREDIT CODE]\n     ELSE IF([WRITE COUNT]=4)\n       THEN RETURN_VALUE=[PAYROLL][PAYDTRN][CASH CODE]\n     ELSE IF([WRITE COUNT]=5)\n       THEN RETURN_VALUE=[PAYROLL][PAYDTRN][LIABILITY\
 CODE]\n     ELSE IF([WRITE COUNT]=6)\n	THEN RETURN_VALUE=[PAYROLL][PAYDTRN][ACCOUNT CODE]\n     ELSE IF([WRITE COUNT]=7)\n	THEN RETURN_VALUE=[PAYROLL][PAYDTRN][ENCUMBRANCE CODE]\n     ELSE IF([WRITE COUNT\
]=8)\n	THEN RETURN_VALUE=[PAYROLL][PAYDTRN][RESERVE CODE]\n     FI FI FI FI FI FI FI FI\nELSE RETURN_VALUE=\"\" FI");
	addvirtuallineADV(tmp,"ACCOUNT CODE",1,40,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(126+1);
	sprintf(t,"IF([ACCOUNT TYPE]=0) \nTHEN RETURN_VALUE=[FINMGT][FINEACC][DESCRIPTION]\nELSE RETURN_VALUE=[FINMGT][FINBACC][DESCRIPTION] \nFI");
	addvirtuallineADV(tmp,"ACCOUNT CODE DESCRIPTION",1,80,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(31+1);
	sprintf(t,"[PAYROLL][PAYMSTR][FISCAL YEAR]");
	addvirtuallineADV(tmp,"FISCAL YEAR",8,4,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(239+1);
	sprintf(t,"IF(([PRIMARY FILE NUMBER]=1) AND ([WRITE COUNT]=1)) \n  THEN RETURN_VALUE=[PAYROLL][PAYJTRN][AMOUNT]\nELSE IF(([PRIMARY FILE NUMBER]=2) AND ([WRITE COUNT]=1)) \n  THEN RETURN_VALUE=[PAYROLL][PAYDTRN][AMOU\
NT] \nELSE RETURN_VALUE=0 \nFI FI");
	addvirtuallineADV(tmp,"AMOUNT",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(266+1);
	sprintf(t,"IF([WRITE COUNT]=1) \nTHEN IF([PRIMARY FILE NUMBER]=1) \n       THEN RETURN_VALUE=[PAYROLL][PAYJTRN][AMOUNT]\n     ELSE IF([PRIMARY FILE NUMBER]=2) \n       THEN RETURN_VALUE=[PAYROLL][PAYDTRN][AMOUNT]\n   \
  ELSE RETURN_VALUE=0 \n     FI FI\nELSE RETURN_VALUE=0 FI");
	addvirtuallineADV(tmp,"EXPENSE AMOUNT",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(266+1);
	sprintf(t,"IF([WRITE COUNT]=6) \nTHEN IF([PRIMARY FILE NUMBER]=1) \n       THEN RETURN_VALUE=[PAYROLL][PAYJTRN][AMOUNT]\n     ELSE IF([PRIMARY FILE NUMBER]=2) \n       THEN RETURN_VALUE=[PAYROLL][PAYDTRN][AMOUNT]\n   \
  ELSE RETURN_VALUE=0 \n     FI FI\nELSE RETURN_VALUE=0 FI");
	addvirtuallineADV(tmp,"ENCUMBRANCE AMOUNT",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(339+1);
	sprintf(t,"IF(([PRIMARY FILE NUMBER]=1) AND (([WRITE COUNT]=2) OR ([WRITE COUNT]=4) OR\n	([WRITE COUNT]=7))) \n  THEN RETURN_VALUE=[PAYROLL][PAYJTRN][AMOUNT] \nELSE IF(([PRIMARY FILE NUMBER]=2) AND (([WRITE COUNT]=2\
) OR ([WRITE COUNT]=4)\n        OR ([WRITE COUNT]=7))) \n  THEN RETURN_VALUE=[PAYROLL][PAYDTRN][AMOUNT] \nELSE RETURN_VALUE=0 \nFI FI");
	addvirtuallineADV(tmp,"DEBIT AMOUNT",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(328+1);
	sprintf(t,"IF(([PRIMARY FILE NUMBER]=1) AND (([WRITE COUNT]=3) OR ([WRITE COUNT]=5) OR ([WRITE COUNT]=8))) \n  THEN RETURN_VALUE=[PAYROLL][PAYJTRN][AMOUNT] \nELSE IF(([PRIMARY FILE NUMBER]=2) AND (([WRITE COUNT]=3)\
 OR ([WRITE COUNT]=5) OR ([WRITE COUNT]=8))) \n  THEN RETURN_VALUE=[PAYROLL][PAYDTRN][AMOUNT] \nELSE RETURN_VALUE=0 \nFI FI");
	addvirtuallineADV(tmp,"CREDIT AMOUNT",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(203+1);
	sprintf(t,"IF([ACCOUNT TYPE]=0) THEN RETURN_VALUE=\"EXPENDITURES\" \nELSE IF([ACCOUNT TYPE]=1) THEN RETURN_VALUE=\"REVENUE\"\nELSE IF([ACCOUNT TYPE]=2) THEN RETURN_VALUE=\"BALANCE SHEET\"\nELSE RETURN_VALUE=\"\" FI");
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
	addvirtuallineADV(tmp,"DEDUCTION DESCRIPTION IDENTIFICATION",1,30,t,0,0);

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
	t=Rmalloc(21+1);
	sprintf(t,"[PRIMARY FILE NUMBER]");
	addvirtuallineADV(tmp,"WHICH TYPE",7,2,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(102+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=2) THEN RETURN_VALUE=[PAYROLL][PAYDDMS][BENEFIT] \nELSE RETURN_VALUE=FALSE FI");
	addvirtuallineADV(tmp,"BENEFIT",10,1,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(111+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=2) THEN RETURN_VALUE=[PAYROLL][PAYDDMS][SHORT DESCRIPTION] \nELSE RETURN_VALUE=\"\" FI");
	addvirtuallineADV(tmp,"BENEFIT DESCRIPTION",1,40,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(69+1);
	sprintf(t,"(\"PAYROLL DISTRIBUTION REPORT for: \"+[SELECTED PAY IDENTIFICATION])");
	addvirtuallineADV(tmp,"REPORT TITLE",1,60,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(124+1);
	sprintf(t,"(\"Fiscal Processing Month: \"+(INTEGER_TO_STRING([FISCAL PROCESSING MONTH]))+\"-(\"+[FISCAL PROCESSING MONTH STRING]+\")\")");
	addvirtuallineADV(tmp,"REPORT SUB-TITLE",1,80,t,0,0);

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
	addaccumline(tmp,"LAST PERSONNEL NAME",3,"[PRSNNL][PERDMG][NAME]",0);
	addaccumline(tmp,"LAST IDENTIFICATION",3,"[IDENTIFICATION]",0);
	addaccumline(tmp,"LAST JOB NUMBER",3,"[JOB NUMBER]",0);
	addaccumline(tmp,"TOTAL AMOUNT",7,"[AMOUNT]",0);
	addaccumline(tmp,"TOTAL ENCUMBRANCE AMOUNT",7,"[ENCUMBRANCE AMOUNT]",0);
	addaccumline(tmp,"TOTAL CREDIT AMOUNT",7,"[CREDIT AMOUNT]",0);
	addaccumline(tmp,"TOTAL DEBIT AMOUNT",7,"[DEBIT AMOUNT]",0);
	addaccumline(tmp,"TOTAL EXPENSED AMOUNT",7,"[EXPENSE AMOUNT]",0);
	addaccumline(tmp,"LAST ACCOUNT TYPE",3,"[ACCOUNT TYPE]",0);
	addaccumline(tmp,"LAST ACCOUNT CODE",3,"[ACCOUNT CODE]",0);
	addaccumline(tmp,"LAST FISCAL YEAR",3,"[PAYROLL][PAYMSTR][FISCAL YEAR]",0);
	addaccumline(tmp,"LAST FISCAL PROCESSING MONTH",3,"[FISCAL PROCESSING MONTH]",0);
	addaccumline(tmp,"LAST FISCAL PROCESSING MONTH STRING",3,"[FISCAL PROCESSING MONTH STRING]",0);
	addaccumline(tmp,"LAST DEFINITION TYPE",3,"[DEFINITION TYPE]",0);
	addaccumline(tmp,"LAST ACCOUNT TYPE DESCRIPTION",3,"[ACCOUNT TYPE DESCRIPTION]",0);
	addaccumline(tmp,"LAST ACCOUNT CODE DESCRIPTION",3,"[ACCOUNT CODE DESCRIPTION]",0);
	addaccumline(tmp,"LAST ACCOUNT NUMBER",3,"[ACCOUNT NUMBER]",0);
	addaccumline(tmp,"LAST TRANSACTION TYPE",3,"[TRANSACTION TYPE]",0);
	addaccumline(tmp,"LAST BENEFIT",3,"[BENEFIT]",0);
	addaccumline(tmp,"LAST BENEFIT DESCRIPTION",3,"[BENEFIT DESCRIPTION]",0);
	addaccumline(tmp,"LAST PERSONNEL IDENTIFICATION",3,"[PERSONNEL IDENTIFICATION]",0);
	addaccumline(tmp,"LAST CALENDAR YEAR",3,"[PAYROLL][PAYMSTR][CALENDAR YEAR]",0);
	addaccumline(tmp,"LAST REPORT TITLE",3,"[REPORT TITLE]",0);
	addaccumline(tmp,"LAST REPORT SUB-TITLE",3,"[REPORT SUB-TITLE]",0);
	addcontrolline(tmp,"[ACCOUNT TYPE]",0);
	addcontrolline(tmp,"[DEFINITION TYPE]",0);
	addcontrolline(tmp,"[ACCOUNT CODE]",0);
	addcontrolline(tmp,"[ACCOUNT NUMBER]",0);
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
	addimageline(tmp,"PAGE HEADER","               @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC        Page: @RRRRRRR@@",FALSE,NULL,4,pval);
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
	addimageline(tmp,"PAGE HEADER","               @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC        Date: @LLLLLLL@@",TRUE,"(NOT([SELECTED PAY IDENTIFICATION]=\"\"))",4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST REPORT TITLE]");
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
	addimageline(tmp,"PAGE HEADER","               @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC        Date: @LLLLLLL@@",TRUE,"([SELECTED PAY IDENTIFICATION]=\"\")",4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[REPORT SUB-TITLE]");
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
	addimageline(tmp,"PAGE HEADER","               @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                      @@",TRUE,"(NOT([FISCAL PROCESSING MONTH STRING]=\"\"))",3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST REPORT SUB-TITLE]");
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
	addimageline(tmp,"PAGE HEADER","               @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                      @@",TRUE,"([FISCAL PROCESSING MONTH STRING]=\"\")",3,pval);
	free_pvals(pval,3);


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
	addimageline(tmp,"PAGE HEADER","               @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC        Time: @LLLLLLL@@",FALSE,NULL,4,pval);
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


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ACCOUNT TYPE DESCRIPTION]");
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
	addimageline(tmp,"[ACCOUNT TYPE] HEADER","@LLLLLLLLLLLLLLLLLLLL                                                                            AMOUNT@@",TRUE,"NOT([ACCOUNT TYPE]=2)",3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ACCOUNT TYPE DESCRIPTION]");
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
	addimageline(tmp,"[ACCOUNT TYPE] HEADER","@LLLLLLLLLLLLLLLLLLLL                                                                             DEBIT              CREDIT@@",TRUE,"[ACCOUNT TYPE]=2",3,pval);
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
	addimageline(tmp,"[ACCOUNT TYPE] HEADER","@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ACCOUNT CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ACCOUNT CODE DESCRIPTION]");
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
	addimageline(tmp,"[ACCOUNT CODE] HEADER","@LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL@@",TRUE,"([ACCOUNT TYPE]<2)",4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(7*sizeof(RDApval));
	pvals=pval;
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
	pvals->name=stralloc("[LAST JOB NUMBER]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL ENCUMBRANCE AMOUNT]");
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
	addimageline(tmp,"[PERSONNEL IDENTIFICATION] FOOTER","   @LLLLLLLLLLLLLLLL @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL @RRRR                 @,.RRRRRRRRRRRRRRRRR @,.RRRRRRRRRRRRRR@@",TRUE,"([LAST ACCOUNT TYPE]<2)",7,pval);
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
	addimageline(tmp,"[ACCOUNT CODE] FOOTER","                                                                                    ==================== ==================@@",TRUE,"([LAST ACCOUNT TYPE]<2)",2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(5*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST ACCOUNT CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL EXPENSED AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL ENCUMBRANCE AMOUNT]");
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
	addimageline(tmp,"[ACCOUNT CODE] FOOTER","  TOTAL @LLLLLLLLLLLLLLLLLLLLLLLLLLLL                                               @,.RRRRRRRRRRRRRRRRR @,.RRRRRRRRRRRRRR@@",TRUE,"[LAST ACCOUNT TYPE]<2",5,pval);
	free_pvals(pval,5);


	pval=Rmalloc(6*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST ACCOUNT CODE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST ACCOUNT CODE DESCRIPTION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL DEBIT AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL CREDIT AMOUNT]");
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
	addimageline(tmp,"[ACCOUNT CODE] FOOTER","  @LLLLLLLLLLLLLL  @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL  @,.RRRRRRRRRRRRRRRRR @,.RRRRRRRRRRRRRRRRR@@",TRUE,"[LAST ACCOUNT TYPE]=2",6,pval);
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
	addimageline(tmp,"[ACCOUNT TYPE] FOOTER","                                                                                    ====================@@",TRUE,"[LAST ACCOUNT TYPE]<2",2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST ACCOUNT TYPE DESCRIPTION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL EXPENSED AMOUNT]");
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
	addimageline(tmp,"[ACCOUNT TYPE] FOOTER","TOTAL @LLLLLLLLLLLLLLLLLLLL                                                         @,.RRRRRRRRRRRRRRRRR@@",TRUE,"[LAST ACCOUNT TYPE]<2",4,pval);
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
	addimageline(tmp,"[ACCOUNT TYPE] FOOTER","                                                                                    ==================== ====================@@",TRUE,"[LAST ACCOUNT TYPE]=2",2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(5*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST ACCOUNT TYPE DESCRIPTION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL DEBIT AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL CREDIT AMOUNT]");
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
	addimageline(tmp,"[ACCOUNT TYPE] FOOTER","TOTAL @LLLLLLLLLLLLLLLLLLLL                                                         @,.RRRRRRRRRRRRRRRRR @,.RRRRRRRRRRRRRRRRR@@",TRUE,"[LAST ACCOUNT TYPE]=2",5,pval);
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
	addimageline(tmp,"[ACCOUNT TYPE] FOOTER","@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


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
	addimageline(tmp,"REPORT FOOTER","@@@",FALSE,NULL,3,pval);
	free_pvals(pval,3);






	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			ERRORDIALOG("Cannot Write Report","Error in permissions, user cannot write the report definition.  Check your permissions, and retry.  Call your installer.",NULL,FALSE);
			prterr("Cannont Write Report definition for Report [%s].  Error in permissions, user cannot write the report definition.",rptname);
		} else {
			PAYDSTE_RANGE_SCREEN();
		}
		FreeRDAreport(tmp);
	}
	if(rptname!=NULL) Rfree(rptname);
}
/* PAYROLL's - Make (PAYDSTE RANGE SCREEN) Screen */
static void PAYDSTE_RANGE_SCREEN()
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


	scrn=RDAscrnNEW("PAYROLL","PAYDSTE RANGE SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Pay Identification:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"SELECTED PAY IDENTIFICATION","Selected Pay Identification","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [PERDMG][PERSONNEL IDENTIFICATION]","Range on Personnel Identification","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [PERDMG][PERSONNEL IDENTIFICATION]","From Personnel Identification","","",0,25,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [PERDMG][PERSONNEL IDENTIFICATION]","To Personnel Identification","","",0,25,0,"","","","");
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
