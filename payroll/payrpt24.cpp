/* PAYROLL's - Make (CHGSTEPS) Report */
#include <cstdio>
#include <rptgen.hpp>
/*COMPMEM 40960 */

extern void free_pvals(RDApval *,int);
extern char *module;
static void CHGSTEP_MAINTAIN_SCREEN(void);
static void CHGSTEP_RANGE_SCREEN(void);
static void CHGSTEP_SEARCH_BROWSE(void);
static void CHGSTEP_DEFINE_LIST(void);
static void CHGSTEP_BROWSE(void);
void CHGSTEPS(void);

void CHGSTEPS()
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
	rptname=Rmalloc(8+1);
	sprintf(rptname,"CHGSTEPS");
	t=Rmalloc(12+1);
	sprintf(t,"Change Steps");
	desc=Rmalloc(80+1);
	sprintf(desc,"This output definition is used to manage the Change Steps (Years of Experience).");
	tmp=RDAreportNEWADV(rptname,t,desc,1,"",TRUE,TRUE,FALSE,TRUE,TRUE,"PAYROLL","CHGSTEPS1",TRUE,1,FALSE,"");
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

	adddisplayADV(tmp,(double)8.500000,(double)10.000000,0,FALSE,0,"",0,1,0,NULL);

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
	t=Rmalloc(28+1);
	sprintf(t,"\"CHANGED STEP AUDIT TRAIL\"");
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
	addimageline(tmp,"PAGE HEADER","                        @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC              PAGE:@RRRRRRR@@",FALSE,NULL,4,pval);
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
	addimageline(tmp,"PAGE HEADER","                        @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC              DATE:@RRRRRRR@@",FALSE,NULL,4,pval);
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
	addimageline(tmp,"PAGE HEADER","                        @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC              TIME:@RRRRRRR@@",FALSE,NULL,4,pval);
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


	pval=Rmalloc(7*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYJMST][PERSONNEL IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYJMST][JOB NUMBER]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PRSNNL][PERDMG][NAME]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[PAYROLL][PAYJMST][STEP]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[NEW STEP]");
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
	addimageline(tmp,"DETAIL LINES","@LLLLLLLLLLLLLLL @RRR @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLL @RR @RR @@",FALSE,NULL,7,pval);
	free_pvals(pval,7);


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
			CHGSTEP_MAINTAIN_SCREEN();
			CHGSTEP_RANGE_SCREEN();
			CHGSTEP_SEARCH_BROWSE();
			CHGSTEP_DEFINE_LIST();
			CHGSTEP_BROWSE();
		}

		FreeRDAreport(tmp);
	}
	if(rptname!=NULL) Rfree(rptname);
}
void CHGSTEP_BROWSE()
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


	scrn=RDAscrnNEW("PAYROLL","CHGSTEP BROWSE");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"DEFINE LIST","Define List","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SEARCH BUTTON","Search","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"TOP","Top","","",0,0,41,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"BOTTOM","Bottom","","",0,0,42,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SEARCH ALL","Search All","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"CLEAR SEARCH & LPM","Clear (Search & LPM)","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","LPM:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"BROWSE LIST LPM","Lexical Pattern Match:","","",0,30,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Search Field:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"SEARCH FIELD","Search Field","","",0,40,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,7,"BROWSE LIST","Browse List","","",10,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"ADD","Add","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"CONTINUE","Continue","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+7);
		sprintf(defdir,"%s/rda/%s",CURRENTDIRECTORY,module);
		if(writescrnbin(defdir,scrn))
		{
			ERRORDIALOG("Cannot Save Screen","This screen may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
}
void CHGSTEP_DEFINE_LIST()
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


	scrn=RDAscrnNEW("PAYROLL","CHGSTEP DEFINE LIST");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","FIELD","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","POS.","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","LEN.","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","FIELD","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","POS.","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,10,"","","","",0,0,2,"","","","");
		ADVaddwdgt(scrn,5,"","LEN.","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Delete flag","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"DELETEFLAG POSITION","Deleteflag Position","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,0,"DELETEFLAG LENGTH","Deleteflag Length","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Personnel Identification","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[PAYROLL][PAYJMST][PERSONNEL IDENTIFICATION] POSITION","Personnel Identification Position","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,0,"[PAYROLL][PAYJMST][PERSONNEL IDENTIFICATION] LENGTH","Personnel Identification Length","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Job Number","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[PAYROLL][PAYJMST][JOB NUMBER] POSITION","Job Number Position","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,0,"[PAYROLL][PAYJMST][JOB NUMBER] LENGTH","Job Number Length","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Position ID","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[PAYROLL][PAYJMST][POSITION IDENTIFICATION] POSITION","Position Identification Position","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,0,"[PAYROLL][PAYJMST][POSITION IDENTIFICATION] LENGTH","Position Identification Length","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Gross ID","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[PAYROLL][PAYJMST][GROSS IDENTIFICATION] POSITION","Gross Identification Position","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,0,"[PAYROLL][PAYJMST][GROSS IDENTIFICATION] LENGTH","Gross Identification Length","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Step","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[PAYROLL][PAYJMST][STEP] POSITION","Step Position","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,0,"[PAYROLL][PAYJMST][STEP] LENGTH","Step Length","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","New Step","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[NEW STEP] POSITION","New Step Position","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,0,"[NEW STEP] LENGTH","New Step Length","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Pay Group:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[PAYROLL][PAYJMST][PAY GROUP] POSITION","Pay Group Position","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,0,"[PAYROLL][PAYJMST][PAY GROUP] LENGTH","Pay Group Length","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+7);
		sprintf(defdir,"%s/rda/%s",CURRENTDIRECTORY,module);
		if(writescrnbin(defdir,scrn))
		{
			ERRORDIALOG("Cannot Save Screen","This screen may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
}
static void CHGSTEP_MAINTAIN_SCREEN()
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


	scrn=RDAscrnNEW("PAYROLL","CHGSTEP MAINTAIN SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"[DELETEFLAG]","Skip processing this record","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Personnel Identification:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[PAYROLL][PAYJMST][PERSONNEL IDENTIFICATION]","Personnel Identification","","",0,25,0,"","","","");
		ADVaddwdgt(scrn,0,"[PRSNNL][PERDMG][NAME]","Name","","",0,30,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Job Number:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[PAYROLL][PAYJMST][JOB NUMBER]","Job Number","","",0,6,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Position Identification:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[PAYROLL][PAYJMST][POSITION IDENTIFICATION]","Position Identification","","",0,48,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Gross Identification:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[PAYROLL][PAYJMST][GROSS IDENTIFICATION]","Gross Identification","","",0,40,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Present Step:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[PAYROLL][PAYJMST][STEP]","Step","","",0,4,0,"","","","");
		ADVaddwdgt(scrn,5,"","FTE Amount:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[PAYROLL][PAYJMST][FTE AMOUNT]","Fte Amount","","",0,6,0,"","","","");
		ADVaddwdgt(scrn,5,"","Pay Group:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[PAYROLL][PAYJMST][PAY GROUP]","Pay Group","","",0,1,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Number of Pays:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[PAYROLL][PAYJMST][NUMBER OF PAYS]","Number of Pays","","",0,2,0,"","","","");
		ADVaddwdgt(scrn,5,"","Contract Months:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[PAYROLL][PAYJMST][CONTRACT MONTHS]","Contract Months","","",0,2,0,"","","","");
		ADVaddwdgt(scrn,5,"","Contract Days:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[PAYROLL][PAYJMST][CONTRACT DAYS]","Contract Days","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,5,"","Hours Per Day:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"[PAYROLL][PAYJMST][HOURS PER DAY]","Hours Per Day","","",0,6,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,11,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"BROWSE","Browse","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SAVE NO UPDATE","Fast Save","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,11,"","","","",0,0,1,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+7);
		sprintf(defdir,"%s/rda/%s",CURRENTDIRECTORY,module);
		if(writescrnbin(defdir,scrn))
		{
			ERRORDIALOG("Cannot Save Screen","This screen may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
}
static void CHGSTEP_RANGE_SCREEN()
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


	scrn=RDAscrnNEW("PAYROLL","CHGSTEP RANGE SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Step Change Amount:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"STEP CHANGE","","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,5,"","Step Max:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"STEP MAX","Step Max","","",0,5,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [PAYJMST][GROSS IDENTIFICATION]","Range on Gross Identification","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [PAYJMST][GROSS IDENTIFICATION]","From Gross Identification","","",0,20,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [PAYJMST][GROSS IDENTIFICATION]","To Gross Identification","","",0,20,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [POSMSTR][CLASS IDENTIFICATION]","Range on Class Identification","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [POSMSTR][CLASS IDENTIFICATION]","From Class Identification","","",0,20,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [POSMSTR][CLASS IDENTIFICATION]","To Class Identification","","",0,20,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [POSMSTR][LOCATION IDENTIFICATION]","Range on Location Identification","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [POSMSTR][LOCATION IDENTIFICATION]","From Location Identificatin","","",0,20,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [POSMSTR][LOCATION IDENTIFICATION]","To Location Identification","","",0,20,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [PAYJMST][PERSONNEL IDENTIFICATION]","Range on Personnel Identification","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [PAYJMST][PERSONNEL IDENTIFICATION]","From Personnel Identification","","",0,20,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [PAYJMST][PERSONNEL IDENTIFICATION]","To Personnel Identification","","",0,20,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [PAYJMST][JOB NUMBER]","Range on Job Number","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [PAYJMST][JOB NUMBER]","From Job Number","","",0,6,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [PAYJMST][JOB NUMBER]","To Job Number","","",0,6,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [PAYJMST][PAY GROUP]","Range on Pay Group","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [PAYJMST][PAY GROUP]","From Pay Group","","",0,4,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [PAYJMST][PAY GROUP]","To Pay Group","","",0,4,0,"","","","");
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

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+7);
		sprintf(defdir,"%s/rda/%s",CURRENTDIRECTORY,module);
		if(writescrnbin(defdir,scrn))
		{
			ERRORDIALOG("Cannot Save Screen","This screen may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
}
static void CHGSTEP_SEARCH_BROWSE()
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


	scrn=RDAscrnNEW("PAYROLL","CHGSTEP SEARCH BROWSE");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [PAYROLL][PAYJMST][PERSONNEL IDENTIFICATION]","Range on Personnel Identification","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [PAYROLL][PAYJMST][PERSONNEL IDENTIFICATION]","From Personnel Identification","","",0,25,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [PAYROLL][PAYJMST][PERSONNEL IDENTIFICATION]","To Personnel Identification","","",0,25,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [PAYROLL][PAYJMST][JOB NUMBER]","Range on Job Number","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [PAYROLL][PAYJMST][JOB NUMBER]","From Job Number","","",0,6,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [PAYROLL][PAYJMST][JOB NUMBER]","To Job Number","","",0,6,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [PAYROLL][PAYJMST][PAY GROUP]","Range on Pay Group","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [PAYROLL][PAYJMST][PAY GROUP]","From Pay Group","","",0,1,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [PAYROLL][PAYJMST][PAY GROUP]","To Pay Group","","",0,1,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [PAYROLL][PAYJMST][POSITION IDENTIFICATION]","Range on Position ID","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [PAYROLL][PAYJMST][POSITION IDENTIFICATION]","From Position Identification","","",0,35,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [PAYROLL][PAYJMST][POSITION IDENTIFICATION]","Position Identification","","",0,35,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [PAYROLL][PAYJMST][GROSS IDENTIFICATION]","Range on Gross ID","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [PAYROLL][PAYJMST][GROSS IDENTIFICATION]","From Gross Identification","","",0,25,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [PAYROLL][PAYJMST][GROSS IDENTIFICATION]","To Gross Identification","","",0,25,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [PAYROLL][PAYJMST][CONTRACT MONTHS]","Range on Contact Mo.","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [PAYROLL][PAYJMST][CONTRACT MONTHS]","From Contract Months","","",0,2,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [PAYROLL][PAYJMST][CONTRACT MONTHS]","To Contract Months","","",0,2,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [PAYROLL][PAYJMST][CONTRACT DAYS]","Range on Contract Days","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [PAYROLL][PAYJMST][CONTRACT DAYS]","From Contract Days","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [PAYROLL][PAYJMST][CONTRACT DAYS]","To Contract Days","","",0,3,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [PAYROLL][PAYJMST][HOURS PER DAY]","Range on Hours Per Day","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [PAYROLL][PAYJMST][HOURS PER DAY]","From Hours Per Day","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [PAYROLL][PAYJMST][HOURS PER DAY]","To Hours Per Day","","",0,8,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,0,"","","","");
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
