/* PAYROLL's - Make (DIRDDEPREG-WJC) Report */
#include <cstdio>
#include <rptgen.hpp>
/*COMPMEM 40960 */

extern void free_pvals(RDApval *,int);
extern char *module;
void DIRDDEPREG_WJC(void);

void DIRDDEPREG_WJC()
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
	rptname=Rmalloc(15+1);
	sprintf(rptname,"DIRDDEPREG-WJCC");
	t=Rmalloc(23+1);
	sprintf(t,"Direct Deposit Register");
	desc=Rmalloc(68+1);
	sprintf(desc,"This output definition is used to print the Direct Deposit Register.");
	tmp=RDAreportNEWADV(rptname,t,desc,0,"",FALSE,TRUE,FALSE,FALSE,FALSE,"","",FALSE,1,TRUE,"");
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

	adddisplayADV(tmp,(double)8.500000,(double)11.000000,0,FALSE,0,"",0,1,0,pvars);

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
	x=addsearchfileADV(tmp,"PRSNNL","PERDMG",2,"PAYROLL","PAYJDPM","PERDMG KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJDPM][PERSONNEL IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	x=addsearchfileADV(tmp,"PAYROLL","PAYJPMS",2,"PAYROLL","PAYJDPM","PAYJPMS KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJDPM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYJDPM][JOB NUMBER]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJDPM][PAY IDENTIFICATION]");
	}

	t=Rmalloc(89+1);
	sprintf(t,"(([PAYROLL][PAYPPMS][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYPPMS][BENEFITS COMPUTED]=TRUE))");
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
	x=addsearchfileADV(tmp,"PAYROLL","PAYJMST",2,"PAYROLL","PAYJDPM","JOBMSTR KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJDPM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYJDPM][JOB NUMBER]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(37+1);
	sprintf(t,"([POSTRK][POSMSTR][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"POSTRK","POSMSTR",1,"PAYROLL","PAYJDPM","POSMSTR",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"POSITION IDENTIFICATION","[POSITION IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	x=addsearchfileADV(tmp,"POSTRK","POSLMST",1,"PAYROLL","PAYJDPM","POSLMST",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"LOCATION IDENTIFICATION","[LOCATION IDENTIFICATION]");
	}

	t=Rmalloc(38+1);
	sprintf(t,"([PAYROLL][PAYDDMS][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDDMS",2,"PAYROLL","PAYJDPM","PAYDDMS KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","[PAYJDPM][DEDUCTION DESCRIPTION IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	x=addsearchfileADV(tmp,"PAYROLL","PAYDEDM",1,"PAYROLL","PAYJDPM","PAYDEDM KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJDPM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","[PAYJDPM][DEDUCTION DESCRIPTION IDENTIFICATION]");
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

	t=Rmalloc(113+1);
	sprintf(t,"([PAYROLL][PAYJPRM][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYJPRM][PAY IDENTIFICATION]=[SELECTED PAY IDENTIFICATION])");
	x=addsearchfileADV(tmp,"PAYROLL","PAYJPRM",0,NULL,NULL,"PAYJPRM KEY",FALSE,t,NULL,1);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(36+1);
	sprintf(t,"([PRSNNL][PERDMG][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"PRSNNL","PERDMG",2,"PAYROLL","PAYJPRM","PERDMG KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJPRM][PERSONNEL IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	x=addsearchfileADV(tmp,"PAYROLL","PAYJPMS",2,"PAYROLL","PAYJPRM","PAYJPMS KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJPRM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYJPRM][JOB NUMBER]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJPRM][PAY IDENTIFICATION]");
	}

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

	t=Rmalloc(76+1);
	sprintf(t,"([PAYROLL][PAYJMST][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYJMST][ACTIVE]=TRUE)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYJMST",2,"PAYROLL","PAYJPRM","JOBMSTR KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJPRM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYJPRM][JOB NUMBER]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(37+1);
	sprintf(t,"([POSTRK][POSMSTR][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"POSTRK","POSMSTR",1,"PAYROLL","PAYJPRM","POSMSTR",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"POSITION IDENTIFICATION","[POSITION IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	x=addsearchfileADV(tmp,"POSTRK","POSLMST",1,"PAYROLL","PAYJPRM","POSLMST",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"LOCATION IDENTIFICATION","[LOCATION IDENTIFICATION]");
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
	addvirtuallineADV(tmp,"SELECTED PAY IDENTIFICATION",0,0,t,0,2);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(44+1);
	sprintf(t,"([PRSNNL][PERDMG][PERSONNEL IDENTIFICATION])");
	addvirtuallineADV(tmp,"PERSONNEL IDENTIFICATION",0,0,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(36+1);
	sprintf(t,"([PAYROLL][PAYDDMS][DEDUCTION TYPE])");
	addvirtuallineADV(tmp,"DEDUCTION TYPE",0,0,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(45+1);
	sprintf(t,"([PAYROLL][PAYJMST][POSITION IDENTIFICATION])");
	addvirtuallineADV(tmp,"POSITION IDENTIFICATION",0,0,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(44+1);
	sprintf(t,"([POSTRK][POSMSTR][LOCATION IDENTIFICATION])");
	addvirtuallineADV(tmp,"LOCATION IDENTIFICATION",0,0,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(153+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) \nTHEN RETURN_VALUE=1 \nELSE IF([PRIMARY FILE NUMBER]=2) \n     THEN RETURN_VALUE=0 \n     ELSE RETURN_VALUE=2 \n     FI \nFI");
	addvirtuallineADV(tmp,"WHICH TYPE",7,2,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(229+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) \nTHEN RETURN_VALUE=[PAYROLL][PAYDDMS][DEDUCTION TYPE] \nELSE IF([PRIMARY FILE NUMBER]=2) \n     THEN RETURN_VALUE=[PAYROLL][PAYJPRM][RATE IDENTIFICATION] \n     ELSE RETURN_VAL\
UE=\"\" \n     FI \nFI");
	addvirtuallineADV(tmp,"IDENTIFICATION",0,0,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(236+1);
	sprintf(t,"IF([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"DIRECT DEPOSIT\") \nTHEN IF([PAYROLL][PAYDEDM][CHECKING ACCOUNT]=TRUE) \n     THEN RETURN_VALUE=\"CHECKING\" \n     ELSE RETURN_VALUE=\"SAVING\" FI\nELSE RETURN_VALUE=[LAS\
T BANK ACCOUNT TYPE] FI");
	addvirtuallineADV(tmp,"BANK ACCOUNT TYPE",0,0,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(206+1);
	sprintf(t,"IF(!([PRIMARY FILE NUMBER]=2)) \nTHEN RETURN_VALUE=0 \nELSE IF([PAYROLL][PAYJPRM][RATE IDENTIFICATION]=\"DAILY\") \n     THEN RETURN_VALUE=[PAYROLL][PAYJPRM][GROSS] \n     ELSE RETURN_VALUE=0 \n     FI \nFI");
	addvirtuallineADV(tmp,"DAILY",2,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(207+1);
	sprintf(t,"IF(!([PRIMARY FILE NUMBER]=2)) \nTHEN RETURN_VALUE=0 \nELSE IF([PAYROLL][PAYJPRM][RATE IDENTIFICATION]=\"HOURLY\") \n     THEN RETURN_VALUE=[PAYROLL][PAYJPRM][GROSS] \n     ELSE RETURN_VALUE=0 \n     FI \nFI");
	addvirtuallineADV(tmp,"HOURLY",2,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(207+1);
	sprintf(t,"IF(!([PRIMARY FILE NUMBER]=2)) \nTHEN RETURN_VALUE=0 \nELSE IF([PAYROLL][PAYJPRM][RATE IDENTIFICATION]=\"SALARY\") \n     THEN RETURN_VALUE=[PAYROLL][PAYJPRM][GROSS] \n     ELSE RETURN_VALUE=0 \n     FI \nFI");
	addvirtuallineADV(tmp,"SALARY",2,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(223+1);
	sprintf(t,"IF(!([PRIMARY FILE NUMBER]=2)) \nTHEN RETURN_VALUE=0 \nELSE IF([PAYROLL][PAYJPRM][RATE IDENTIFICATION]=\"ADVANCED CERTIFICATE\") \n     THEN RETURN_VALUE=[PAYROLL][PAYJPRM][GROSS] \n     ELSE RETURN_VALUE=0 \
\n     FI \nFI");
	addvirtuallineADV(tmp,"ADVANCED CERTIFICATE",2,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(212+1);
	sprintf(t,"IF(!([PRIMARY FILE NUMBER]=2)) \nTHEN RETURN_VALUE=0 \nELSE IF([PAYROLL][PAYJPRM][RATE IDENTIFICATION]=\"DOCTORATE\") \n     THEN RETURN_VALUE=[PAYROLL][PAYJPRM][GROSS] \n     ELSE RETURN_VALUE=0 \n     FI \nF\
I");
	addvirtuallineADV(tmp,"DOCTORATE",2,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(212+1);
	sprintf(t,"IF(!([PRIMARY FILE NUMBER]=2)) \nTHEN RETURN_VALUE=0 \nELSE IF([PAYROLL][PAYJPRM][RATE IDENTIFICATION]=\"LONGEVITY\") \n     THEN RETURN_VALUE=[PAYROLL][PAYJPRM][GROSS] \n     ELSE RETURN_VALUE=0 \n     FI \nF\
I");
	addvirtuallineADV(tmp,"LONGEVITY",2,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(211+1);
	sprintf(t,"IF(!([PRIMARY FILE NUMBER]=2)) \nTHEN RETURN_VALUE=0 \nELSE IF([PAYROLL][PAYJPRM][RATE IDENTIFICATION]=\"OVERTIME\") \n     THEN RETURN_VALUE=[PAYROLL][PAYJPRM][GROSS] \n     ELSE RETURN_VALUE=0 \n     FI \nFI\
");
	addvirtuallineADV(tmp,"OVERTIME",2,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(215+1);
	sprintf(t,"IF(!([PRIMARY FILE NUMBER]=2)) \nTHEN RETURN_VALUE=0 \nELSE IF([PAYROLL][PAYJPRM][RATE IDENTIFICATION]=\"LEAVE DOCKED\") \n     THEN RETURN_VALUE=[PAYROLL][PAYJPRM][GROSS] \n     ELSE RETURN_VALUE=0 \n     FI\
 \nFI");
	addvirtuallineADV(tmp,"LEAVE DOCKED",2,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(158+1);
	sprintf(t,"IF(([PRIMARY FILE NUMBER]=1) \n  AND ([PAYROLL][PAYDDMS][REDUCE NET]=TRUE)) \nTHEN RETURN_VALUE=((-1)*([PAYROLL][PAYJDPM][AMOUNT])) \nELSE RETURN_VALUE=0 \nFI");
	addvirtuallineADV(tmp,"EMPLOYEE AMOUNT",2,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(159+1);
	sprintf(t,"IF(([PRIMARY FILE NUMBER]=1) \n  AND ([PAYROLL][PAYDDMS][REDUCE NET]=FALSE)) \nTHEN RETURN_VALUE=((-1)*([PAYROLL][PAYJDPM][AMOUNT])) \nELSE RETURN_VALUE=0 \nFI");
	addvirtuallineADV(tmp,"MATCHING AMOUNT",2,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(278+1);
	sprintf(t,"IF(!([PRIMARY FILE NUMBER]=1)) \nTHEN RETURN_VALUE=0 \nELSE IF(([PAYROLL][PAYDDMS][REDUCE NET]=TRUE) \n       AND (!([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"DIRECT DEPOSIT\"))) \n     THEN RETURN_VALUE=((-1)*([\
PAYROLL][PAYJDPM][AMOUNT])) \n     ELSE RETURN_VALUE=0 \n     FI \nFI");
	addvirtuallineADV(tmp,"REDUCE NET AMOUNT",2,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(319+1);
	sprintf(t,"IF(!([PRIMARY FILE NUMBER]=1)) \nTHEN RETURN_VALUE=0 \nELSE IF(([PAYROLL][PAYDDMS][REDUCE NET]=TRUE) \n       AND ([PAYROLL][PAYDTYP][VOLUNTARY]=TRUE) \n       AND (!([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"DI\
RECT DEPOSIT\"))) \n     THEN RETURN_VALUE=[PAYROLL][PAYJDPM][AMOUNT] \n     ELSE RETURN_VALUE=0 \n     FI \nFI");
	addvirtuallineADV(tmp,"DEDUCTION AMOUNT",2,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(98+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=2) \nTHEN RETURN_VALUE=[PAYROLL][PAYJPRM][GROSS] \nELSE RETURN_VALUE=0 FI");
	addvirtuallineADV(tmp,"GROSS AMOUNT",2,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(48+1);
	sprintf(t,"([TOTAL GROSS AMOUNT]+[TOTAL REDUCE NET AMOUNT])");
	addvirtuallineADV(tmp,"NET AMOUNT",2,15,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(547+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) \nTHEN IF((NOT([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"DIRECT DEPOSIT\")) AND\n        (NOT([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"FICA\")) AND \n        (NOT([PAYROLL][PAYDDMS][DEDUCTI\
ON TYPE]=\"MEDICARE\")) AND \n        (NOT([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"FEDERAL\")) AND \n        (NOT([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"STATE\")) AND \n        ([PAYROLL][PAYDDMS][REDUCE NET]=TRUE)\
) \n     THEN RETURN_VALUE=((-1)*([PAYROLL][PAYJDPM][AMOUNT])) \n     ELSE RETURN_VALUE=0 \n     FI \nELSE RETURN_VALUE=0 \nFI");
	addvirtuallineADV(tmp,"NONTAX DEDUCTION AMOUNT",2,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(273+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) \nTHEN IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"DIRECT DEPOSIT\") AND \n        ([PAYROLL][PAYDDMS][REDUCE NET]=TRUE)) \n     THEN RETURN_VALUE=((-1)*([PAYROLL][PAYJDPM][AMOUNT])\
) \n     ELSE RETURN_VALUE=0 \n     FI \nELSE RETURN_VALUE=0 \nFI");
	addvirtuallineADV(tmp,"DIRECT DEPOSIT AMOUNT",2,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(266+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) \nTHEN IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"FEDERAL\") AND \n        ([PAYROLL][PAYDDMS][REDUCE NET]=TRUE)) \n     THEN RETURN_VALUE=((-1)*([PAYROLL][PAYJDPM][AMOUNT])) \n    \
 ELSE RETURN_VALUE=0 \n     FI \nELSE RETURN_VALUE=0 \nFI");
	addvirtuallineADV(tmp,"FEDERAL AMOUNT",2,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(263+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) \nTHEN IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"FICA\") AND \n        ([PAYROLL][PAYDDMS][REDUCE NET]=TRUE)) \n     THEN RETURN_VALUE=((-1)*([PAYROLL][PAYJDPM][AMOUNT])) \n     EL\
SE RETURN_VALUE=0 \n     FI \nELSE RETURN_VALUE=0 \nFI");
	addvirtuallineADV(tmp,"FICA AMOUNT",2,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(267+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) \nTHEN IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"MEDICARE\") AND \n        ([PAYROLL][PAYDDMS][REDUCE NET]=TRUE)) \n     THEN RETURN_VALUE=((-1)*([PAYROLL][PAYJDPM][AMOUNT])) \n   \
  ELSE RETURN_VALUE=0 \n     FI \nELSE RETURN_VALUE=0 \nFI");
	addvirtuallineADV(tmp,"MEDICARE AMOUNT",2,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(264+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) \nTHEN IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"STATE\") AND \n        ([PAYROLL][PAYDDMS][REDUCE NET]=TRUE)) \n     THEN RETURN_VALUE=((-1)*([PAYROLL][PAYJDPM][AMOUNT])) \n     E\
LSE RETURN_VALUE=0 \n     FI \nELSE RETURN_VALUE=0 \nFI");
	addvirtuallineADV(tmp,"STATE AMOUNT",2,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(65+1);
	sprintf(t,"(\"DIRECT DEPOSIT REGISTER  for \"+[SELECTED PAY IDENTIFICATION])");
	addvirtuallineADV(tmp,"REPORT TITLE",0,0,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(42+1);
	sprintf(t,"(\"Executed By: \"+[LOGIN IDENTIFICATION])");
	addvirtuallineADV(tmp,"EXECUTED BY",0,0,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(34+1);
	sprintf(t,"[PAYROLL][PAYDDMS][DEDUCTION TYPE]");
	addvirtuallineADV(tmp,"DEDUCTION TYPE",0,0,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(106+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) \nTHEN RETURN_VALUE=(![PAYROLL][PAYDDMS][REDUCE NET]) \nELSE RETURN_VALUE=0 FI");
	addvirtuallineADV(tmp,"EMPLOYEE PAID",10,1,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(108+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) \nTHEN RETURN_VALUE=([PAYROLL][PAYDTYP][VOLUNTARY]) \nELSE RETURN_VALUE=FALSE FI");
	addvirtuallineADV(tmp,"VOLUNTARY",10,1,t,0,0);

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

	addsortline(tmp,0,"[LOCATION IDENTIFICATION]");
	addsortline(tmp,0,"[POSITION IDENTIFICATION]");
	addsortline(tmp,0,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION]");
	addsortline(tmp,0,"[WHICH TYPE]");
	addsortline(tmp,0,"[IDENTIFICATION]");
	addaccumline(tmp,"TOTAL DAILY",7,"[DAILY]",0);
	addaccumline(tmp,"TOTAL HOURLY",7,"[HOURLY]",0);
	addaccumline(tmp,"TOTAL SALARY",7,"[SALARY]",0);
	addaccumline(tmp,"TOTAL ADVANCED CERTIFICATE",7,"[ADVANCED CERTIFICATE]",0);
	addaccumline(tmp,"TOTAL DOCTORATE",7,"[DOCTORATE]",0);
	addaccumline(tmp,"TOTAL LONGEVITY",7,"[LONGEVITY]",0);
	addaccumline(tmp,"TOTAL OVERTIME",7,"[OVERTIME]",0);
	addaccumline(tmp,"TOTAL LEAVE DOCKED",7,"[LEAVE DOCKED]",0);
	addaccumline(tmp,"TOTAL REDUCE NET AMOUNT",7,"[REDUCE NET AMOUNT]",0);
	addaccumline(tmp,"TOTAL GROSS AMOUNT",7,"[GROSS AMOUNT]",0);
	addaccumline(tmp,"TOTAL DEDUCTION AMOUNT",7,"[DEDUCTION AMOUNT]",0);
	addaccumline(tmp,"TOTAL NET AMOUNT",7,"[NET AMOUNT]",0);
	addaccumline(tmp,"TOTAL EMPLOYEE AMOUNT",7,"[EMPLOYEE AMOUNT]",0);
	addaccumline(tmp,"TOTAL MATCHING AMOUNT",7,"[MATCHING AMOUNT]",0);
	addaccumline(tmp,"TOTAL DIRECT DEPOSIT AMOUNT",7,"[DIRECT DEPOSIT AMOUNT]",0);
	addaccumline(tmp,"LAST LOCATION IDENTIFICATION",3,"[POSTRK][POSMSTR][LOCATION IDENTIFICATION]",0);
	addaccumline(tmp,"LAST POSITION IDENTIFICATION",3,"[PAYROLL][PAYJMST][POSITION IDENTIFICATION]",0);
	addaccumline(tmp,"LAST PERSONNEL IDENTIFICATION",3,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION]",0);
	addaccumline(tmp,"LAST EMPLOYEE NAME",3,"[PRSNNL][PERDMG][NAME]",0);
	addaccumline(tmp,"LAST PERSONNEL IDENTIFICATION",3,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION]",0);
	addaccumline(tmp,"LAST SOCIAL SECURITY NUMBER",3,"[PRSNNL][PERDMG][SOCIAL SECURITY NUMBER]",0);
	addaccumline(tmp,"LAST WHICH TYPE",3,"[WHICH TYPE]",0);
	addaccumline(tmp,"LAST IDENTIFICATION",3,"[IDENTIFICATION]",0);
	addaccumline(tmp,"LAST CHECK NUMBER",3,"[PAYROLL][PAYJPMS][CHECK NUMBER]",0);
	addaccumline(tmp,"LAST CHECK DATE",3,"[PAYROLL][PAYJPMS][CHECK DATE]",0);
	addaccumline(tmp,"LAST EMPLOYEE PAID",3,"[EMPLOYEE PAID]",0);
	addaccumline(tmp,"LAST BANK ACCOUNT NUMBER",3,"[PAYROLL][PAYDEDM][BANK ACCOUNT NUMBER]",0);
	addaccumline(tmp,"LAST TRANSMITTAL ROUTE NUMBER",3,"[PAYROLL][PAYDEDM][TRANSMITTAL ROUTE NUMBER]",0);
	addaccumline(tmp,"LAST BANK ACCOUNT TYPE",3,"[BANK ACCOUNT TYPE]",0);
	addaccumline(tmp,"TOTAL DIRECT DEPOSIT AMOUNT",7,"[DIRECT DEPOSIT AMOUNT]",0);
	addaccumline(tmp,"TOTAL FEDERAL AMOUNT",7,"[FEDERAL AMOUNT]",0);
	addaccumline(tmp,"TOTAL FICA AMOUNT",7,"[FICA AMOUNT]",0);
	addaccumline(tmp,"TOTAL MEDICARE AMOUNT",7,"[MEDICARE AMOUNT]",0);
	addaccumline(tmp,"TOTAL STATE AMOUNT",7,"[STATE AMOUNT]",0);
	addaccumline(tmp,"TOTAL NONTAX DEDUCTION AMOUNT",7,"[NONTAX DEDUCTION AMOUNT]",0);
	addcontrolline(tmp,"[LOCATION IDENTIFICATION]",0);
	addcontrolline(tmp,"[POSITION IDENTIFICATION]",0);
	addcontrolline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION]",0);
	addcontrolline(tmp,"[WHICH TYPE]",0);
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

	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("Compressed Print");
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
	addimageline(tmp,"REPORT HEADER","@@@",FALSE,NULL,3,pval);
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
	addimageline(tmp,"PAGE HEADER","                @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC  PAGE: @RRRRRRR@@",FALSE,NULL,4,pval);
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
	addimageline(tmp,"PAGE HEADER","                @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC DATE: @LLLLLLL@@",FALSE,NULL,4,pval);
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
	addimageline(tmp,"PAGE HEADER","                @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC  TIME: @LLLLLLL@@",FALSE,NULL,4,pval);
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
	addimageline(tmp,"PAGE HEADER","  PERSONNEL IDENTIFICATION                 GROSS   FICA/MEDICARE         FEDERAL           STATE       OTHER DED  DIRECT DEPOSIT             NET@@",FALSE,NULL,2,pval);
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
	addimageline(tmp,"PAGE HEADER","    BANK TRANS#    ACCOUNT                           CHK/SAV@@",FALSE,NULL,2,pval);
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


	pval=Rmalloc(10*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST PERSONNEL IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL GROSS AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL FICA AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL FEDERAL AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL STATE AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL NONTAX DEDUCTION AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL DIRECT DEPOSIT AMOUNT]");
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","  @LLLLLLLLLLLLLLLLLLLLLLLLLLLLL  @RRRRRRRRRRR.RR @RRRRRRRRRRR.RR @RRRRRRRRRRR.RR @RRRRRRRRRRR.RR @RRRRRRRRRRR.RR @RRRRRRRRRRR.RR @RRRRRRRRRRR.RR@@",FALSE,NULL,10,pval);
	free_pvals(pval,10);


	pval=Rmalloc(5*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST BANK ACCOUNT NUMBER]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST TRANSMITTAL ROUTE NUMBER]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST BANK ACCOUNT TYPE]");
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","    @LLLLLLLLLLL   @LLLLLLLLLLLLLLLLLLLLLLLLLLLLL  - @LLLLLLLLLLL@@",FALSE,NULL,5,pval);
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(9*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL GROSS AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL FICA AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL FEDERAL AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL STATE AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL NONTAX DEDUCTION AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL DIRECT DEPOSIT AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL NET AMOUNT]");
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
	addimageline(tmp,"REPORT FOOTER","  ****GRAND TOTALS****            @RRRRRRRRRRR.RR @RRRRRRRRRRR.RR @RRRRRRRRRRR.RR @RRRRRRRRRRR.RR @RRRRRRRRRRR.RR @RRRRRRRRRRR.RR @RRRRRRRRRRR.RR@@",FALSE,NULL,9,pval);
	free_pvals(pval,9);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("Normal Print");
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
	addimageline(tmp,"REPORT FOOTER","@@@",FALSE,NULL,3,pval);
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
