/* PAYROLL's - Make (PAYVWETR) Report */
#include <cstdio>
#include <rptgen.hpp>
/*COMPMEM 40960 */

extern void free_pvals(RDApval *,int);
extern char *module;
void PAYVWETR(void);
static void PAYVWETR_RANGE_SCREEN(void);

void PAYVWETR()
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
	sprintf(rptname,"PAYVWETR");
	t=Rmalloc(55+1);
	sprintf(t,"Payroll Vendor Withholding Electronic Transfer Register");
	desc=Rmalloc(92+1);
	sprintf(desc,"This output definition is used to print the Vendor Withholding Electronic Transfer Register.");
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
	adddisplayADV(tmp,(double)8.500000,(double)10.000000,0,FALSE,0,"",0,1,0,pvars);

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

	t=Rmalloc(248+1);
	sprintf(t,"([PAYROLL][PAYDDSV][DELETEFLAG]=FALSE) AND \n([PAYROLL][PAYDDSV][SOURCE USER VERIFIED]=TRUE) AND ([PAYROLL][PAYDDSV][DISTRIBUTED]=FALSE) AND \n([PAYROLL][PAYDDSV][ELECTRONIC TRANSFER NUMBER]>0) AND \n([PA\
YROLL][PAYDDSV][ELECTRONIC TRANSFER]=TRUE)");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDDSV",0,NULL,NULL,"PAYDDSV KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","");
		addsearchconfld(s,"FISCAL YEAR","");
		addsearchconfld(s,"TRANSACTION NUMBER","");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(37+1);
	sprintf(t,"(NOT([PAYROLL][PAYDDMS][DELETEFLAG]))");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDDMS",1,"PAYROLL","PAYDDSV","PAYDDMS KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","[PAYDDSV][DEDUCTION DESCRIPTION IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(36+1);
	sprintf(t,"([VENPMT][VENMSR][DELETEFLAG]=FALSE)");
	x=addsearchfileADV(tmp,"VENPMT","VENMSR",1,"PAYROLL","PAYDDSV","VENDOR ID KEY",TRUE,t,NULL,1);
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
	x=addsearchfileADV(tmp,"BNKREC","BNKACC",1,"PAYROLL","PAYDDSV","BNKACC KEY",TRUE,t,NULL,1);
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
	t=Rmalloc(114+1);
	sprintf(t,"IF([PAYROLL][PAYDDSV][MANUAL VOUCHER]=TRUE) \nTHEN RETURN_VALUE=(\"MANUAL\") \nELSE RETURN_VALUE=(\"DIRECT\") \nFI");
	addvirtuallineADV(tmp,"VOUCHER TYPE",1,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(42+1);
	sprintf(t,"(\"Executed By: \"+[LOGIN IDENTIFICATION])");
	addvirtuallineADV(tmp,"EXECUTED BY",0,0,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(46+1);
	sprintf(t,"[PAYROLL][PAYDDSV][ELECTRONIC TRANSFER NUMBER]");
	addvirtuallineADV(tmp,"ELECTRONIC TRANSFER NUMBER",8,12,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(13+1);
	sprintf(t,"[TOTAL CHECK]");
	addvirtuallineADV(tmp,"CHECK AMOUNT OLD",2,20,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(41+1);
	sprintf(t,"[TOTAL CHECK][ELECTRONIC TRANSFER NUMBER]");
	addvirtuallineADV(tmp,"CHECK AMOUNT",2,20,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(115+1);
	sprintf(t,"IF([ELECTRONIC TRANSFER NUMBER]=[LAST ELECTRONIC TRANSFER NUMBER]) \nTHEN RETURN_VALUE=0 \nELSE RETURN_VALUE=1 \nFI");
	addvirtuallineADV(tmp,"ELECTRONIC TRANSFER NUMBER COUNTER",8,10,t,1,0);

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
	addsortline(tmp,0,"[PAYROLL][PAYDDSV][MANUAL VOUCHER]");
	addsortline(tmp,0,"[ELECTRONIC TRANSFER NUMBER]");
	addsortline(tmp,0,"[PAYROLL][PAYDDSV][VIRTUAL ELECTRONIC TRANSFER NUMBER]");
	addsortline(tmp,0,"[PAYROLL][PAYDDSV][FISCAL YEAR]");
	addsortline(tmp,0,"[PAYROLL][PAYDDSV][DEDUCTION DESCRIPTION IDENTIFICATION]");
	addsortline(tmp,0,"[PAYROLL][PAYDDSV][TRANSACTION NUMBER]");
	addaccumline(tmp,"TOTAL CHECK",7,"[PAYROLL][PAYDDSV][AMOUNT]",0);
	addaccumline(tmp,"LAST BANK IDENTIFICATION",3,"[PAYROLL][PAYDDSV][BANK IDENTIFICATION]",0);
	addaccumline(tmp,"LAST ELECTRONIC TRANSFER NUMBER",3,"[PAYROLL][PAYDDSV][ELECTRONIC TRANSFER NUMBER]",0);
	addaccumline(tmp,"LAST ELECTRONIC TRANSFER DATE",3,"[PAYROLL][PAYDDSV][ELECTRONIC TRANSFER DATE]",0);
	addaccumline(tmp,"LAST VIRTUAL ELECTRONIC TRANSFER NUMBER",3,"[PAYROLL][PAYDDSV][VIRTUAL ELECTRONIC TRANSFER NUMBER]",0);
	addaccumline(tmp,"LAST VENDOR IDENTIFICATION",3,"[PAYROLL][PAYDDSV][VENDOR IDENTIFICATION]",0);
	addaccumline(tmp,"LAST VENDOR NAME",3,"[VENPMT][VENMSR][VENDOR NAME]",0);
	addaccumline(tmp,"LAST VOUCHER DESCRIPTION",3,"[PAYROLL][PAYDDSV][DESCRIPTION]",0);
	addaccumline(tmp,"LAST VOUCHER TYPE",3,"[VOUCHER TYPE]",0);
	addaccumline(tmp,"LAST MANUAL VOUCHER",3,"[PAYROLL][PAYDDSV][MANUAL VOUCHER]",0);
	addaccumline(tmp,"TOTAL NUMBER OF ELECTRONIC TRANFERS",7,"[ELECTRONIC TRANSFER NUMBER COUNTER]",0);
	addcontrolline(tmp,"[PAYROLL][PAYDDSV][BANK IDENTIFICATION]",0);
	addcontrolline(tmp,"[ELECTRONIC TRANSFER NUMBER]",0);
	addcontrolline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL ELECTRONIC TRANSFER NUMBER]",0);




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
	addimageline(tmp,"PAGE HEADER","           @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC       Page: @RRRRRRR@@",FALSE,NULL,4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
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
	addimageline(tmp,"PAGE HEADER","                     VENDOR WITHHOLDING ELECTRONIC TRANSFER REGISTER                Date: @LLLLLLL@@",FALSE,NULL,3,pval);
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
	addimageline(tmp,"PAGE HEADER","           @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC       Time: @LLLLLLL@@",FALSE,NULL,4,pval);
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
	addimageline(tmp,"PAGE HEADER","  Elec Tran No. Date       Vendor Ident.    Vendor Name                 Type                Amount @@",FALSE,NULL,2,pval);
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
	addimageline(tmp,"PAGE HEADER","  ------------- ---------- ---------------- --------------------------- ----------- ---------------@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYDDSV][BANK IDENTIFICATION]");
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][BANK IDENTIFICATION] HEADER","Bank Identification: @VVVVVVVV ( @VVVV )@@",FALSE,NULL,4,pval);
	free_pvals(pval,4);


	pval=Rmalloc(8*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VIRTUAL ELECTRONIC TRANSFER NUMBER]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST ELECTRONIC TRANSFER DATE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VENDOR IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VENDOR NAME]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VOUCHER TYPE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[CHECK AMOUNT]");
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL ELECTRONIC TRANSFER NUMBER] FOOTER","  @RRRRRRRRRRRR @LLLLLLLLL @LLLLLLLLLLLLLLL @LLLLLLLLLLLLLLLLLLLLLLLLLL @LLLLLLLLLL @,.RRRRRRRRRRRR@@",TRUE,"([LAST VIRTUAL ELECTRONIC TRANSFER NUMBER]=[LAST ELECTRONIC TRANSFER NUMBER]) AND ([LAST MANUAL VOUCHER]=FALSE)",8,pval);
	free_pvals(pval,8);


	pval=Rmalloc(8*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST ELECTRONIC TRANSFER NUMBER]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST ELECTRONIC TRANSFER DATE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VENDOR IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VENDOR NAME]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VOUCHER TYPE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[CHECK AMOUNT]");
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL ELECTRONIC TRANSFER NUMBER] FOOTER","  @RRRRRRRRRRRR @LLLLLLLLL @LLLLLLLLLLLLLLL @LLLLLLLLLLLLLLLLLLLLLLLLLL @LLLLLLLLLL @,.RRRRRRRRRRRR@@",TRUE,"([LAST MANUAL VOUCHER]=TRUE) AND ([BODY COUNT]>0)",8,pval);
	free_pvals(pval,8);


	pval=Rmalloc(7*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VIRTUAL ELECTRONIC TRANSFER NUMBER]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST ELECTRONIC TRANSFER DATE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VENDOR IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VENDOR NAME]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST VOUCHER TYPE]");
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][VIRTUAL ELECTRONIC TRANSFER NUMBER] FOOTER","  @RRRRRRRRRRRR @LLLLLLLLL @LLLLLLLLLLLLLLL @LLLLLLLLLL VOID VOID VOID  @LLLLLLLLLL $*********0.00 @@",TRUE,"(NOT([LAST VIRTUAL ELECTRONIC TRANSFER NUMBER]=[LAST ELECTRONIC TRANSFER NUMBER])) AND ([BODY COUNT]>0)",7,pval);
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][BANK IDENTIFICATION] FOOTER","                                                                                    ---------------@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(5*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST BANK IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL NUMBER OF ELECTRONIC TRANFERS]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL CHECK]");
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
	addimageline(tmp,"[PAYROLL][PAYDDSV][BANK IDENTIFICATION] FOOTER","Total Bank: @LLLLLLLLLLLLLLLLLLLLLLLLLLLL Total Number of Elec. Trans.: ( @RRRRR ): @,.RRRRRRRRRRRR@@",FALSE,NULL,5,pval);
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
	addimageline(tmp,"REPORT FOOTER","                                                                                    ===============@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL CHECK]");
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
	addimageline(tmp,"REPORT FOOTER","Total All:                                                                          @,.RRRRRRRRRRRR@@",FALSE,NULL,3,pval);
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
		} else {
			PAYVWETR_RANGE_SCREEN();
		}
		FreeRDAreport(tmp);
	}
	if(rptname!=NULL) Rfree(rptname);
}
/* PAYROLL's - Make (PAYVWETR RANGE SCREEN) Screen */
static void PAYVWETR_RANGE_SCREEN()
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


	scrn=RDAscrnNEW("PAYROLL","PAYVWETR RANGE SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [PAYDDSV][BANK IDENTIFICATION]","Range on Bank Identification","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [PAYDDSV][BANK IDENTIFICATION]","From Bank Identification","","",0,20,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [PAYDDSV][BANK IDENTIFICATION]","To Bank Identification","","",0,20,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [PAYDDSV][VENDOR IDENTIFICATION]","Range on Vendor Identification","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [PAYDDSV][VENDOR IDENTIFICATION]","To Vendor Identification","","",0,20,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [PAYDDSV][VENDOR IDENTIFICATION]","To Vendor Identification","","",0,20,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [PAYDDSV][ELECTRONIC TRANSFER NUMBER]","Range on Eelctronic Transfer Number","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [PAYDDSV][ELECTRONIC TRANSFER NUMBER]","From Electronic Transfer Number","","",0,12,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [PAYDDSV][ELECTRONIC TRANSFER NUMBER]","To Electronic Transfer Number","","",0,12,0,"","","","");
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
