/* PAYROLL's - Make (DIRDEPTRAN11) Report */
#include <cstdio>
#include <rptgen.hpp>
/*COMPMEM 40960 */

struct DEDTYPESTRUCTS
{
        APPlib *vol_dedtypes;
        APPlib *man_dedtypes;
        APPlib *all_dedtypes;
};
typedef struct DEDTYPESTRUCTS DEDTYPESTRUCT;

/*lint ++flb */
struct RateStructs
{
        char *rateid;
        int seq;
};
typedef struct RateStructs RateStruct;
/*lint --flb */

struct PayRateStructs
{
        RateStruct *Rates;
        int num;
};
typedef struct PayRateStructs PayRateStruct;

extern void buildDEDlists(short,DEDTYPESTRUCT *);
extern DEDTYPESTRUCT *MakeDEDTYPElists(short,char *,char *,void (*)(short,
	DEDTYPESTRUCT *),short);
extern APPlib *MakeLVEDSClist(short,char *,char *,void (*)(short,APPlib *),short);
extern void FreePayRateStruct(PayRateStruct *);
extern void buildPayRateStruct(short,PayRateStruct *);
extern void buildLVElist(short,APPlib *);
extern PayRateStruct *MakePayRateStruct(short,char *,char *,void (*)(short,
	PayRateStruct *),short);


extern void free_pvals(RDApval *,int);
extern void free_pvars(RDApvar *,int);
extern char *module;
static void DIRDEPTRAN11(short,DEDTYPESTRUCT *,short,PayRateStruct *,short,APPlib *);

void dirdeptran11(void)
{
        short dedtypnum=(-1);
        DEDTYPESTRUCT *Deduction_Types=NULL;
        short posratenum=(-1);
        PayRateStruct *PayRates=NULL;
        short lvedscnum=(-1);
        APPlib *lvedsclist=NULL;

        if((dedtypnum=APPOPNNRD(module,"PAYDTYP",TRUE,FALSE))==(-1))
        {
                ERRORDIALOG("CANNOT CREATE PAYROLL REGISTER REPORT!","Error in PAYDTYP data base file.  Please check that the Payroll Deduction Type data file exist and is defined correctly and then retry.",NULL,FALSE);
                prterr("Error in PAYDTYP data base file.");
        } else {
                Deduction_Types=MakeDEDTYPElists(dedtypnum,module,"Creating Application's Deduction Type Lists",buildDEDlists,1);
                if(Deduction_Types==NULL)
                {
                        ERRORDIALOG("CANNOT CREATE PAYROLL REGISTER REPORT!","Error in PAYDTYP data base file.  Can not Allocate Space for Deduction Types Structure.  Please check that their is enough per user memory available and that the Payroll Deduction Type data file exist and is defined correctly and then retry.",NULL,FALSE);
                        prterr("Error in PAYDTYP data base file.  Can not Allocate Space for Deduction Types Structure.");
                } else {
                        if(Deduction_Types->vol_dedtypes==NULL ||
                                Deduction_Types->man_dedtypes==NULL)
                        {
                                ERRORDIALOG("CANNOT CREATE PAYROLL CHECKS REPORT!","Error in PAYDTYP data base file.  Can not find any Voluntary or Manditory Deduction Types in the PAYDTYP data file.  Please check that the Payroll Deduction Type data fileexist and is defined correctly and then retry.",NULL,FALSE);
                                prterr("Error in PAYDTYP data base file.  Can not find any Voluntary or Manditory Deduction Types in the PAYDTYP data file.");
                        } else if((Deduction_Types->vol_dedtypes->numlibs<1) &&
                                (Deduction_Types->man_dedtypes->numlibs<1))
                        {
                                ERRORDIALOG("CANNOT CREATE PAYROLL CHECKS REPORT!","No Valid, Non-Deleted Deduction Types were found in the PAYDTYP data file.  Please check that the Payroll Deduction Type data file exist and is defined correctly and then retry.",NULL,FALSE);
                                prterr("No Valid, Non-Deleted Deduction Types were found in the PAYDTYP data file.");
                        } else {
                                if((posratenum=APPOPNNRD("POSTRK","POSRATE",TRUE,FALSE))==(-1))
                                {
                                        WARNINGDIALOG("WARNING DIALOG SCREEN",
                                                "POSITION TRACKING MODULE NOT PRESENT!","Error in POSRATE data base file.  Please check that the Rate Identification datafile exist if you want to have the Earning Rates to appear in the PAYROLL REGISTER REPORT.",
                                                NULL,NULL,TRUE,NULL);
                                        prterr("Error in POSRATE data base file.");
                                } else {
                                        PayRates=MakePayRateStruct(posratenum,module,"Creating Application's Pay Rate Structure",buildPayRateStruct,1);
                                        if(PayRates==NULL)
                                        {
                                                WARNINGDIALOG("WARNING DIALOG SCREEN","POSITION TRACKING MODULE NOT PRESENT!",
                                                "Error in POSRATE data base file.  Can not Allocate Space for Application's Pay Rate Structure or POSRATE Module Not Present.  Please check that if this module is to be present their is enough per user memory available and that the POSRATE data file exist and is defined correctly and then retry.",
                                                        NULL,NULL,TRUE,NULL);
                                                prterr("Error in POSRATE data base file.  Can not Allocate Space for Application's Rate Id Structure or POSTRK Module is not setup.");
                                        } else {
                                                lvedscnum=APPOPNNRD("LVEMGT","LVEDSC",TRUE,FALSE);
                                                if(lvedscnum==(-1))
                                                {
                                                        WARNINGDIALOG("WARNING DIALOGSCREEN",
                                                                "LEAVE MANAGEMENT MODULE NOT PRESENT!","Error in LVEDSC data base file.  Please check that the Leave Description data file exist if you want to have the Leave Descriptions to appear in the PAYROLL REGISTER REPORT.",NULL,NULL,TRUE,NULL);
                                                        prterr("Error in LVEDSC database file.");
                                                } else {
                                                        lvedsclist=MakeLVEDSClist(lvedscnum,module,"Creating Application's Leave Description Lists",buildLVElist,1); 
							if(lvedsclist==NULL)
                                                        {
                                                                WARNINGDIALOG("WARNING DIALOG SCREEN","LEAVE MANAGEMENT MODULE NOT PRESENT!",
                                                                "Error in LVEDSC database file.  Can not Allocate Space for Application's Leave Description List or LVEMGT Module Not Present.  Please check that if this module is to be present their is enough per user memory available and that the Leave Description data file exist and is defined correctly and then retry.",
								NULL,NULL,TRUE,NULL);
                                                                prterr("Error in LVEDSC data base file.  Can not Allocate Space for Application's Leave Description List or LVEMGT Module is not setup.");
                                                        } else {
                                                                DIRDEPTRAN11(dedtypnum,Deduction_Types,posratenum,PayRates,lvedscnum,lvedsclist);
                                                        }
                                                }
                                        }
                                }
                        }
                }
        }
        if(dedtypnum!=(-1)) CLSNRD(dedtypnum);
        if(Deduction_Types!=NULL)
        {
                if(Deduction_Types->vol_dedtypes!=NULL)
                        freeapplib(Deduction_Types->vol_dedtypes);
                if(Deduction_Types->man_dedtypes!=NULL)
                        freeapplib(Deduction_Types->man_dedtypes);
                if(Deduction_Types->all_dedtypes!=NULL)
                        freeapplib(Deduction_Types->all_dedtypes);
                Rfree(Deduction_Types);
        }
        FreePayRateStruct(PayRates);
        if(lvedscnum!=(-1)) CLSNRD(lvedscnum);
        if(lvedsclist!=NULL) freeapplib(lvedsclist);
}



static void DIRDEPTRAN11(short dedtypnum,DEDTYPESTRUCT *Deduction_Types,
                short posratenum,PayRateStruct *PayRates,
                short lvedscnum,APPlib *lvedsclist)
{
	int nvs=0,x=0,count=0,z=0,expoffset=0;
	int rate_count=0,man_count=0,vol_count=0,lve_count=0;
	double db=0.0;
	RDAreport *tmp=NULL;
	RDAsearch *s=NULL;
	RDApval *pval=NULL,*pvals=NULL;
	RateStruct *Rate=NULL;
	char *expression=NULL,temp[15000];
/*
	int y,z;
	short longest=132;
	int numpvars=0;
	DFvirtual *d=NULL;
	RDAacct *acc=NULL;
	RDAaccpart *part=NULL;
	RDApvar *pvar=NULL;
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
	rptname=Rmalloc(13+1);
	sprintf(rptname,"DIRDEPTRAN11");
	t=Rmalloc(35+1);
	sprintf(t,"Payroll Direct Deposits on HP Laser");
	desc=Rmalloc(86+1);
	sprintf(desc,"This output definition is used to print the Actual Payroll Direct Deposit Transmittal.");
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
	adddisplayADV(tmp,(double)8.500000,(double)11.00000,0,FALSE,1,"",0,1,0,pvars);
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

	t=Rmalloc(113+1);
	sprintf(t,"([PAYROLL][PAYJDPM][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYJDPM][PAY IDENTIFICATION]=[SELECTED PAY IDENTIFICATION])");
	x=addsearchfileADV(tmp,"PAYROLL","PAYJDPM",0,NULL,NULL,"PAYJDPM KEY #2",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PAY IDENTIFICATION","[SELECTED PAY IDENTIFICATION]");
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","");
		addsearchconfld(s,"JOB NUMBER","");
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","");
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

	t=Rmalloc(90+1);
	sprintf(t,"(([PAYROLL][PAYJPMS][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYJPMS][DIRECT DEPOSIT NUMBER]>0))");
	x=addsearchfileADV(tmp,"PAYROLL","PAYJPMS",2,"PAYROLL","PAYJDPM","PAYJPMS KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJDPM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYJDPM][JOB NUMBER]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJDPM][PAY IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

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

	x=addsearchfileADV(tmp,"PAYROLL","PAYMSTR",1,"PAYROLL","PAYJDPM","PAYMSTR KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJDPM][PAY IDENTIFICATION]");
	}

	x=addsearchfileADV(tmp,"PAYROLL","PAYDCSM",1,"PAYROLL","PAYJDPM","PAYDCSM KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJDPM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","[PAYJDPM][DEDUCTION DESCRIPTION IDENTIFICATION]");
		addsearchconfld(s,"CALENDAR YEAR","[CALENDAR YEAR]");
	}

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
	x=addsearchfileADV(tmp,"PAYROLL","PAYDDMS",1,"PAYROLL","PAYJDPM","PAYDDMS KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","[PAYJDPM][DEDUCTION DESCRIPTION IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	x=addsearchfileADV(tmp,"VENPMT","VENMSR",1,"PAYROLL","PAYJDPM","VENDOR ID KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"VENDOR IDENTIFICATION","[VENDOR IDENTIFICATION]");
	}

	t=Rmalloc(36+1);
	sprintf(t,"[PAYROLL][PAYDEDM][DELETEFLAG]=FALSE");
	x=addsearchfileADV(tmp,"PAYROLL","PAYDEDM",1,"PAYROLL","PAYJDPM","PAYDEDM KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJDPM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","[PAYJDPM][DEDUCTION DESCRIPTION IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

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
	x=addsearchfileADV(tmp,"PAYROLL","PAYJPRM",0,NULL,NULL,"RDA PAYJPRM KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PAY IDENTIFICATION","[SELECTED PAY IDENTIFICATION]");
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","");
		addsearchconfld(s,"JOB NUMBER","");
		addsearchconfld(s,"RATE IDENTIFICATION","");
	}
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

	t=Rmalloc(90+1);
	sprintf(t,"(([PAYROLL][PAYJPMS][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYJPMS][DIRECT DEPOSIT NUMBER]>0))");
	x=addsearchfileADV(tmp,"PAYROLL","PAYJPMS",2,"PAYROLL","PAYJPRM","PAYJPMS KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJPRM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYJPRM][JOB NUMBER]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJPRM][PAY IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

	t=Rmalloc(113+1);
	sprintf(t,"(([BNKREC][BNKACC][DELETEFLAG]=FALSE) AND ([BNKREC][BNKACC][BANK IDENTIFICATION]=[SELECTED BANK IDENTIFICATION]))");
	x=addsearchfileADV(tmp,"BNKREC","BNKACC",1,"PAYROLL","PAYJPRM","BNKACC KEY",FALSE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"BANK IDENTIFICATION","[BANK IDENTIFICATION]");
	}
	if(t!=NULL) Rfree(t);

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

	x=addsearchfileADV(tmp,"PAYROLL","PAYMSTR",1,"PAYROLL","PAYJPRM","PAYMSTR KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJPRM][PAY IDENTIFICATION]");
	}

	x=addsearchfileADV(tmp,"PAYROLL","PAYJRCS",1,"PAYROLL","PAYJPRM","PAYJRCS KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJPRM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYJPRM][JOB NUMBER]");
		addsearchconfld(s,"RATE IDENTIFICATION","[PAYJPRM][RATE IDENTIFICATION]");
		addsearchconfld(s,"CALENDAR YEAR","[CALENDAR YEAR]");
	}

	x=addsearchfileADV(tmp,"PAYROLL","PAYPCSM",1,"PAYROLL","PAYJPRM","PAYPCSM KEY",FALSE,NULL,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJPRM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"CALENDAR YEAR","[CALENDAR YEAR]");
	}

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

	if(lvedsclist!=NULL)
	{
		for(z=0;z<lvedsclist->numlibs;++z)
		{
			x=addsearchfile(tmp,"LVEMGT","LVEMSTR",1,"PAYROLL","PAYJPRM","LVEMSTR KEY",FALSE,"([LVEMGT][LVEMSTR][DELETEFLAG]=FALSE)",1);
			if(x!=(-1))
			{
				s=tmp->search+(x-1);
				addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJPRM][PERSONNEL IDENTIFICATION]");
				memset(stemp,0,101);
				sprintf(stemp,"[LEAVE DESCRIPTION IDENTIFICATION #%d]",(z+1));
				addsearchconfld(s,"LEAVE DESCRIPTION IDENTIFICATION",stemp);
			}
		}
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
	addvirtuallineADV(tmp,"SELECTED PAY IDENTIFICATION",1,15,t,0,2);

	if(t!=NULL) Rfree(t);
	addvirtuallineADV(tmp,"SELECTED BANK IDENTIFICATION",1,15,t,0,2);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(44+1);
	sprintf(t,"([PRSNNL][PERDMG][PERSONNEL IDENTIFICATION])");
	addvirtuallineADV(tmp,"PERSONNEL IDENTIFICATION",1,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(111+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=2) THEN RETURN_VALUE=[PAYROLL][PAYJPRM][RATE IDENTIFICATION] ELSE RETURN_VALUE=\"\" FI");
	addvirtuallineADV(tmp,"RATE IDENTIFICATION",1,25,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(35+1);
	sprintf(t,"([PAYROLL][PAYMSTR][CALENDAR YEAR])");
	addvirtuallineADV(tmp,"CALENDAR YEAR",8,4,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(36+1);
	sprintf(t,"([PAYROLL][PAYDDMS][DEDUCTION TYPE])");
	addvirtuallineADV(tmp,"DEDUCTION TYPE",1,30,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(45+1);
	sprintf(t,"([PAYROLL][PAYJMST][POSITION IDENTIFICATION])");
	addvirtuallineADV(tmp,"POSITION IDENTIFICATION",1,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(41+1);
	sprintf(t,"([PAYROLL][PAYJPMS][BANK IDENTIFICATION])");
	addvirtuallineADV(tmp,"BANK IDENTIFICATION",1,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(200+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) THEN RETURN_VALUE=([PAYROLL][PAYDDMS][VENDOR IDENTIFICATION]) ELSE RETURN_VALUE=\"\" FI");
	addvirtuallineADV(tmp,"VENDOR IDENTIFICATION",1,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(41+1);
	sprintf(t,"([PRSNNL][PERDMG][LOCATION IDENTIFICATION])");
	addvirtuallineADV(tmp,"LOCATION IDENTIFICATION",1,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(661+1);
	sprintf(t,"IF(([PRSNNL][PERDMG][MAILING ADDRESS #1]=\"\") AND ([PRSNNL][PERDMG][MAILING ADDRESS #2]=\"\") AND  ([PRSNNL][PERDMG][MAILING CITY]=\"\") AND  ([PRSNNL][PERDMG][MAILING STATE]=\"\") AND  ([PRSNNL][PERDMG][MAIL\
ING ZIP CODE]=\"\")) THEN RETURN_VALUE=\"\" ELSE IF(([PRSNNL][PERDMG][MAILING ADDRESS #1]=\"\") AND ([PRSNNL][PERDMG][MAILING ADDRESS #2]=\"\")) THEN RETURN_VALUE=([PRSNNL][PERDMG][MAILING CITY]+\", \"+[PRSNNL]\
[PERDMG][MAILING STATE]+\"  \"+[PRSNNL][PERDMG][MAILING ZIP CODE]) ELSE IF([PRSNNL][PERDMG][MAILING ADDRESS #1]=\"\") THEN RETURN_VALUE=[PRSNNL][PERDMG][MAILING ADDRESS #2] ELSE RETURN_VALUE=[PRSNNL][PERD\
MG][MAILING ADDRESS #1] FI FI FI");
	addvirtuallineADV(tmp,"PERSONNEL MAILING ADDRESS 1",1,49,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(578+1);
	sprintf(t,"IF(([PRSNNL][PERDMG][MAILING ADDRESS #1]=\"\") AND ([PRSNNL][PERDMG][MAILING ADDRESS #2]=\"\")) THEN RETURN_VALUE=\"\" ELSE IF(([PRSNNL][PERDMG][MAILING ADDRESS #1]=\"\") AND ([PRSNNL][PERDMG][MAILING CITY]=\"\"\
) AND ([PRSNNL][PERDMG][MAILING STATE]=\"\") AND ([PRSNNL][PERDMG][MAILING ZIP CODE]=\"\")) THEN RETURN_VALUE=(\"\") ELSE IF([PRSNNL][PERDMG][MAILING ADDRESS #2]=\"\") THEN RETURN_VALUE=([PRSNNL][PERDMG][MAIL\
ING CITY]+\", \"+[PRSNNL][PERDMG][MAILING STATE]+\"  \"+[PRSNNL][PERDMG][MAILING ZIP CODE]) ELSE RETURN_VALUE=[PRSNNL][PERDMG][MAILING ADDRESS #2] FI FI FI");
	addvirtuallineADV(tmp,"PERSONNEL MAILING ADDRESS 2",1,49,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(385+1);
	sprintf(t,"IF(([PRSNNL][PERDMG][MAILING ADDRESS #1]=\"\") OR ([PRSNNL][PERDMG][MAILING ADDRESS #2]=\"\") OR (([PRSNNL][PERDMG][MAILING CITY]=\"\")AND ([PRSNNL][PERDMG][MAILING STATE]=\"\") AND ([PRSNNL][PERDMG][MAILING Z\
IP CODE]=\"\"))) THEN RETURN_VALUE=\"\" ELSE RETURN_VALUE=([PRSNNL][PERDMG][MAILING CITY]+\", \"+[PRSNNL][PERDMG][MAILING STATE]+\"  \"+[PRSNNL][PERDMG][MAILING ZIP CODE]) FI");
	addvirtuallineADV(tmp,"PERSONNEL MAILING ADDRESS 3",1,49,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(148+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) \nTHEN RETURN_VALUE=1 \nELSE IF([PRIMARY FILE NUMBER]=2) \n     THEN RETURN_VALUE=2 \n     ELSE RETURN_VALUE=0 \nFI \nFI");
	addvirtuallineADV(tmp,"WHICH TYPE",7,2,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(169+1);
	sprintf(t,"IF([PRIMARY FILE NUMBER]=1) \nTHEN RETURN_VALUE=[PAYROLL][PAYDDMS][DEDUCTION DESCRIPTION IDENTIFICATION] \nELSE RETURN_VALUE=[PAYROLL][PAYJPRM][RATE IDENTIFICATION] \nFI");
	addvirtuallineADV(tmp,"IDENTIFICATION",1,30,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(111+1);
	sprintf(t,"IF(!([PRIMARY FILE NUMBER]=2)) \nTHEN RETURN_VALUE=0 \nELSE RETURN_VALUE=(([PAYROLL][PAYJPRM][GROSS])/100) \nFI");
	addvirtuallineADV(tmp,"GROSS",20,15,t,0,0);

	if(PayRates!=NULL)
	{
		if(PayRates->Rates!=NULL)
		{
			for(z=0,Rate=PayRates->Rates;z<PayRates->num;++z,++Rate)
			{
				memset(stemp,0,101);
				sprintf(stemp,"RATE IDENTIFICATION #%d",(z+1));
				t=Rmalloc(RDAstrlen(Rate->rateid)+3);
				sprintf(t,"\"%s\"",
					(Rate->rateid==NULL?"":Rate->rateid));
				addvirtualline(tmp,stemp,PLAINTEXT,15,t,0);
				if(t!=NULL) Rfree(t);

				t=Rmalloc(RDAstrlen(Rate->rateid)+350);
				sprintf(t,"%s%s%s%s%s%s%s%s%s",
					"IF(!([PRIMARY FILE NUMBER]=2)) \n",
					"THEN RETURN_VALUE=0 \n",
					"ELSE IF([PAYROLL][PAYJPRM][RATE IDENTIFICATION]=\"",
					Rate->rateid,
					"\") \n",
					"     THEN RETURN_VALUE=[PAYROLL][PAYJPRM][GROSS]/100 \n",
					"     ELSE RETURN_VALUE=0 \n",
					"     FI \n",
					"FI");
				memset(stemp,0,101);
				sprintf(stemp,"RATE IDENTIFICATION GROSS #%d",z+1);
				addvirtualline(tmp,stemp,DECIMALV,15,t,0);
				if(t!=NULL) Rfree(t);

				t=Rmalloc(RDAstrlen(Rate->rateid)+350);
				sprintf(t,"%s%s%s%s%s%s%s%s%s",
					"IF(!([PRIMARY FILE NUMBER]=2)) \n",
					"THEN RETURN_VALUE=0 \n",
					"ELSE IF([PAYROLL][PAYJPRM][RATE IDENTIFICATION]=\"",
					Rate->rateid,
					"\") \n",
					"     THEN RETURN_VALUE=[ANNUAL CALENDAR PERSONNEL JOB RATE GROSS]/100 \n",
					"     ELSE RETURN_VALUE=0 \n",
					"     FI \n",
					"FI");
				memset(stemp,0,101);
				sprintf(stemp,"RATE IDENTIFICATION GROSS YTD #%d",z+1);
				addvirtualline(tmp,stemp,DECIMALV,15,t,0);
				if(t!=NULL) Rfree(t);

				t=Rmalloc(RDAstrlen(Rate->rateid)+350);
				sprintf(t,"%s%s%s%s%s%s%s%s%s",
					"IF(!([PRIMARY FILE NUMBER]=2)) \n",
					"THEN RETURN_VALUE=0 \n",
					"ELSE IF([PAYROLL][PAYJPRM][RATE IDENTIFICATION]=\"",
					Rate->rateid,
					"\") \n",
					"     THEN RETURN_VALUE=[PAYROLL][PAYJPRM][UNITS] \n",
					"     ELSE RETURN_VALUE=0 \n",
					"     FI \n",
					"FI");
				memset(stemp,0,101);
				sprintf(stemp,"RATE IDENTIFICATION UNITS #%d",z+1);
				addvirtualline(tmp,stemp,SDECIMALV,15,t,0);
				if(t!=NULL) Rfree(t);

				t=Rmalloc(RDAstrlen(Rate->rateid)+350);
				sprintf(t,"%s%s%s%s%s%s%s%s%s",
					"IF(!([PRIMARY FILE NUMBER]=2)) \n",
					"THEN RETURN_VALUE=0 \n",
					"ELSE IF([PAYROLL][PAYJPRM][RATE IDENTIFICATION]=\"",
					Rate->rateid,
					"\") \n",
					"     THEN RETURN_VALUE=[ANNUAL CALENDAR PERSONNEL JOB RATE UNITS] \n",
					"     ELSE RETURN_VALUE=0 \n",
					"     FI \n",
					"FI");
				memset(stemp,0,101);
				sprintf(stemp,"RATE IDENTIFICATION UNITS YTD #%d",z+1);
				addvirtualline(tmp,stemp,SDECIMALV,15,t,0);
				if(t!=NULL) Rfree(t);

			}
			for(nvs=0;nvs<PayRates->num;++nvs)

			{
				memset(temp,0,15000);
				if(nvs<PayRates->num)
				{
				if(nvs>0)
				{
					sprintf(temp,
						"IF([RATE SLOT #%d]=0) THEN RETURN_VALUE=0 \n",(nvs));
					t=stralloc(temp);
				}
				for(z=nvs,Rate=PayRates->Rates;z<PayRates->num;++z,++Rate)
				{
					if(z==0)
					{
						sprintf(temp,
							"IF((!([TOTAL RATE IDENTIFICATION GROSS #%d]=0)) OR (!([TOTAL RATE IDENTIFICATION UNITS #%d]=0))) THEN RETURN_VALUE=%d \n",(z+1),(z+1),(z+1));
					} else { 
						if(nvs>0)
						{
						sprintf(temp,
							"ELSE IF(((!([TOTAL RATE IDENTIFICATION GROSS #%d]=0)) OR (!([TOTAL RATE IDENTIFICATION UNITS #%d]=0))) AND ([RATE SLOT #%d]<%d)) THEN RETURN_VALUE=%d \n",(z+1),(z+1),nvs,(z+1),(z+1));
						} else {
						sprintf(temp,
							"ELSE IF((!([TOTAL RATE IDENTIFICATION GROSS #%d]=0)) OR (!([TOTAL RATE IDENTIFICATION UNITS #%d]=0))) THEN RETURN_VALUE=%d \n",(z+1),(z+1),(z+1));
						}
					}
					t=stralloccat(t,temp);
				}
				t=stralloccat(t,"ELSE RETURN_VALUE=(0) \n");
				for(z=nvs;z<(PayRates->num);++z)
				{
					t=stralloccat(t,"FI ");
				}
				} else {
					t=stralloccat(t,"0");
				}
				memset(stemp,0,101);
				sprintf(stemp,"RATE SLOT #%d",(nvs+1));
				addvirtualline(tmp,stemp,LONGV,4,t,1);
				if(t!=NULL) Rfree(t);
				

				memset(temp,0,15000);
				if(nvs<PayRates->num)
				{
				sprintf(temp,
					"IF([RATE SLOT #%d])=0) THEN RETURN_VALUE=\"\" \n",nvs+1);
				t=stralloc(temp);
				for(z=nvs;z<(PayRates->num);++z)
				{
					sprintf(temp,
						"ELSE IF([RATE SLOT #%d]=%d) THEN RETURN_VALUE=[LAST RATE IDENTIFICATION #%d] \n",nvs+1,(z+1),(z+1));

					t=stralloccat(t,temp);
				}
				t=stralloccat(t,"ELSE RETURN_VALUE=\"\" \n");
				for(z=nvs;z<(PayRates->num+1);++z)
				{
					t=stralloccat(t,"FI ");
				}
				} else {
					t=stralloccat(t,"\"\"");
				}
				memset(stemp,0,101);
				sprintf(stemp,"RATE SLOT TYPE #%d",(nvs+1));
				addvirtualline(tmp,stemp,PLAINTEXT,15,t,1);
				if(t!=NULL) Rfree(t);

				memset(temp,0,15000);
				if(nvs<PayRates->num)
				{
				sprintf(temp,
					"IF([RATE SLOT #%d])=0) THEN RETURN_VALUE=0 \n",nvs+1);
				t=stralloc(temp);
				for(z=nvs;z<(PayRates->num);++z)
				{
					sprintf(temp,
						"ELSE IF([RATE SLOT #%d]=%d) THEN RETURN_VALUE=[TOTAL RATE IDENTIFICATION GROSS #%d] \n",nvs+1,(z+1),(z+1));


					t=stralloccat(t,temp);
				}
				t=stralloccat(t,"ELSE RETURN_VALUE=0 \n");
				for(z=nvs;z<(PayRates->num+1);++z)
				{
					t=stralloccat(t,"FI ");
				}
				} else {
					t=stralloccat(t,"0");
				}
				memset(stemp,0,101);
				sprintf(stemp,"RATE SLOT GROSS #%d",(nvs+1));
				addvirtualline(tmp,stemp,DECIMALV,15,t,1);
				if(t!=NULL) Rfree(t);
					
				memset(temp,0,15000);
				if(nvs<PayRates->num)
				{
				sprintf(temp,
					"IF([RATE SLOT #%d])=0) THEN RETURN_VALUE=0 \n",nvs+1);
				t=stralloc(temp);
				for(z=nvs;z<(PayRates->num);++z)
				{
					sprintf(temp,
						"ELSE IF([RATE SLOT #%d]=%d) THEN RETURN_VALUE=[MAXIMUM RATE IDENTIFICATION GROSS YTD #%d]+[TOTAL RATE IDENTIFICATION GROSS #%d] \n",nvs+1,(z+1),(z+1),(z+1));

					t=stralloccat(t,temp);
				}
				t=stralloccat(t,"ELSE RETURN_VALUE=0 \n");
				for(z=nvs;z<(PayRates->num+1);++z)
				{
					t=stralloccat(t,"FI ");
				}
				} else {
					t=stralloccat(t,"0");
				}
				memset(stemp,0,101);
				sprintf(stemp,"RATE SLOT GROSS YTD #%d",(nvs+1));
				addvirtualline(tmp,stemp,SDECIMALV,15,t,1);
				if(t!=NULL) Rfree(t);

				memset(temp,0,15000);
				if(nvs<PayRates->num)
				{
				sprintf(temp,
					"IF([RATE SLOT #%d])=0) THEN RETURN_VALUE=0 \n",nvs+1);
				t=stralloc(temp);
				for(z=nvs;z<(PayRates->num);++z)
				{
					sprintf(temp,
						"ELSE IF([RATE SLOT #%d]=%d) THEN RETURN_VALUE=[TOTAL RATE IDENTIFICATION UNITS #%d] \n",nvs+1,(z+1),(z+1));


					t=stralloccat(t,temp);
				}
				t=stralloccat(t,"ELSE RETURN_VALUE=0 \n");
				for(z=nvs;z<(PayRates->num+1);++z)
				{
					t=stralloccat(t,"FI ");
				}
				} else {
					t=stralloccat(t,"0");
				}
				memset(stemp,0,101);
				sprintf(stemp,"RATE SLOT UNITS #%d",(nvs+1));
				addvirtualline(tmp,stemp,DECIMALV,15,t,1);
				if(t!=NULL) Rfree(t);
					
				memset(temp,0,15000);
				if(nvs<PayRates->num)
				{
				sprintf(temp,
					"IF([RATE SLOT #%d])=0) THEN RETURN_VALUE=0 \n",nvs+1);
				t=stralloc(temp);
				for(z=nvs;z<(PayRates->num);++z)
				{
					sprintf(temp,
						"ELSE IF([RATE SLOT #%d]=%d) THEN RETURN_VALUE=[MAXIMUM RATE IDENTIFICATION UNITS YTD #%d]+[TOTAL RATE IDENTIFICATION UNITS #%d] \n",nvs+1,(z+1),(z+1),(z+1));

					t=stralloccat(t,temp);
				}
				t=stralloccat(t,"ELSE RETURN_VALUE=0 \n");
				for(z=nvs;z<(PayRates->num+1);++z)
				{
					t=stralloccat(t,"FI ");
				}
				} else {
					t=stralloccat(t,"0");
				}
				memset(stemp,0,101);
				sprintf(stemp,"RATE SLOT UNITS YTD #%d",(nvs+1));
				addvirtualline(tmp,stemp,SDECIMALV,15,t,1);
				if(t!=NULL) Rfree(t);
			}
		}
	}

	expoffset=0;
	if(Deduction_Types->man_dedtypes!=NULL)
	{
		if((Deduction_Types->man_dedtypes->numlibs)>0)
		{
			for(z=0;z<(Deduction_Types->man_dedtypes->numlibs);++z)
			{
			
				if(expression==NULL)
				{
					expression=Rmalloc(RDAstrlen(Deduction_Types->man_dedtypes->libs[z])+77);
					sprintf(expression,
						"IF([PRIMARY FILE NUMBER]=1) \nTHEN IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"%s\")",
(Deduction_Types->man_dedtypes->libs[z]==NULL?"":Deduction_Types->man_dedtypes->libs[z]));
					expoffset=RDAstrlen(expression);
				} else { 
					expression=Rrealloc(expression,RDAstrlen(expression)+RDAstrlen(Deduction_Types->man_dedtypes->libs[z])+46);
					sprintf(&expression[expoffset],
						"\n  OR ([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"%s\")",
(Deduction_Types->man_dedtypes->libs[z]==NULL?"":Deduction_Types->man_dedtypes->libs[z]));
					expoffset=(RDAstrlen(expression));
				}
				memset(stemp,0,101);
				sprintf(stemp,"MANDATORY DEDUCTION TYPE #%d",(z+1));
				t=Rmalloc(RDAstrlen(Deduction_Types->man_dedtypes->libs[z])+3);
				sprintf(t,
					"\"%s\"",
					(Deduction_Types->man_dedtypes->libs[z]==NULL?"":Deduction_Types->man_dedtypes->libs[z]));
				addvirtualline(tmp,stemp,PLAINTEXT,15,t,0);
				if(t!=NULL) Rfree(t);

				t=Rmalloc(RDAstrlen(Deduction_Types->man_dedtypes->libs[z])+400);
				sprintf(t,
					"IF([PRIMARY FILE NUMBER]=1) \nTHEN IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"%s\") AND ([PAYROLL][PAYDDMS][REDUCE NET])) \n     THEN RETURN_VALUE=(([PAYROLL][PAYJDPM][AMOUNT])/100) \n     ELSE RETURN_VALUE=0 \n     FI \nELSE RETURN_VALUE=0 \nFI",
					(Deduction_Types->man_dedtypes->libs[z]==NULL?"":Deduction_Types->man_dedtypes->libs[z]));
				memset(stemp,0,101);
				sprintf(stemp,"MANDATORY DEDUCTION AMOUNT #%d",(z+1));
				addvirtualline(tmp,stemp,SDECIMALV,15,t,0);
				if(t!=NULL) Rfree(t);

				t=Rmalloc(RDAstrlen(Deduction_Types->man_dedtypes->libs[z])+400);
				sprintf(t,
					"IF([PRIMARY FILE NUMBER]=1) \nTHEN IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"%s\") AND ([PAYROLL][PAYDDMS][REDUCE NET])) \n     THEN RETURN_VALUE=(([ANNUAL CALENDAR PERSONNEL DEDUCTION TOTAL TO DATE])/100) \n     ELSE RETURN_VALUE=0 \n     FI \nELSE RETURN_VALUE=0 \nFI",
					(Deduction_Types->man_dedtypes->libs[z]==NULL?"":Deduction_Types->man_dedtypes->libs[z]));
				memset(stemp,0,101);
				sprintf(stemp,"MANDATORY DEDUCTION YTD #%d",(z+1));
				addvirtualline(tmp,stemp,SDECIMALV,15,t,0);
				if(t!=NULL) Rfree(t);

			}
			for(z=0;z<(Deduction_Types->man_dedtypes->numlibs);++z)
			{
				memset(temp,0,15000);
				if(t==NULL)
				{
					sprintf(temp,"([MANDATORY DEDUCTION AMOUNT #%d]+",(z+1));
				} else if((z+1)==Deduction_Types->man_dedtypes->numlibs)
				{
					sprintf(temp,"[MANDATORY DEDUCTION AMOUNT #%d]+",(z+1));
				} else {
					sprintf(temp,"[MANDATORY DEDUCTION AMOUNT #%d])",(z+1));
				}
				t=stralloccat(t,temp);
			}
			if(isEMPTY(t)) t=stralloc("0.00");
			addvirtuallineADV(tmp,"MANDATORY DEDUCTION AMOUNT TOTAL",6,25,t,1,0);

			if(t!=NULL) Rfree(t);

			for(z=0;z<(Deduction_Types->man_dedtypes->numlibs);++z)
			{
				memset(temp,0,15000);
				if(t==NULL)
				{
					sprintf(temp,"([MANDATORY DEDUCTION YTD #%d]+",(z+1));
				} else if((z+1)==Deduction_Types->man_dedtypes->numlibs)
				{
					sprintf(temp,"[MANDATORY DEDUCTION YTD #%d]+",(z+1));
				} else {
					sprintf(temp,"[VOLUNTARY DEDUCTION YTD #%d])",(z+1));
				}
				t=stralloccat(t,temp);
			}
			if(isEMPTY(t)) t=stralloc("0.00");
			addvirtuallineADV(tmp,"MANDATORY DEDUCTION YTD TOTAL",6,25,t,1,0);

			if(t!=NULL) Rfree(t);

			for(nvs=0;nvs<Deduction_Types->man_dedtypes->numlibs;++nvs)
			{
				memset(temp,0,15000);
				if(nvs<Deduction_Types->man_dedtypes->numlibs)
				{
				if(nvs>0)
				{
					sprintf(temp,
						"IF([MANDATORY SLOT #%d]=0) THEN RETURN_VALUE=0 \n",(nvs));
					t=stralloc(temp);
				}
				for(z=nvs;z<(Deduction_Types->man_dedtypes->numlibs);++z)
				{
					if(z==0)
					{
						sprintf(temp,
							"IF(!([TOTAL MANDATORY DEDUCTION AMOUNT #%d]=0)) THEN RETURN_VALUE=%d \n",(z+1),(z+1));
					} else { 
						if(nvs>0)
						{
						sprintf(temp,
							"ELSE IF((!([TOTAL MANDATORY DEDUCTION AMOUNT #%d]=0)) AND ([MANDATORY SLOT #%d]<%d)) THEN RETURN_VALUE=%d \n",(z+1),nvs,(z+1),(z+1));
						} else {
						sprintf(temp,
							"ELSE IF(!([TOTAL MANDATORY DEDUCTION AMOUNT #%d]=0)) THEN RETURN_VALUE=%d \n",(z+1),(z+1));
						}
					}
					t=stralloccat(t,temp);
				}
				t=stralloccat(t,"ELSE RETURN_VALUE=(0) \n");
				for(z=nvs;z<(Deduction_Types->man_dedtypes->numlibs);++z)
				{
					t=stralloccat(t,"FI ");
				}
				} else {
					t=stralloccat(t,"0");
				}
				memset(stemp,0,101);
				sprintf(stemp,"MANDATORY SLOT #%d",(nvs+1));
				addvirtualline(tmp,stemp,LONGV,4,t,1);
				if(t!=NULL) Rfree(t);
				

				memset(temp,0,15000);
				if(nvs<Deduction_Types->man_dedtypes->numlibs)
				{
				sprintf(temp,
					"IF([MANDATORY SLOT #%d])=0) THEN RETURN_VALUE=\"\" \n",nvs+1);
				t=stralloc(temp);
				for(z=nvs;z<(Deduction_Types->man_dedtypes->numlibs);++z)
				{
					sprintf(temp,
						"ELSE IF([MANDATORY SLOT #%d]=%d) THEN RETURN_VALUE=[LAST MANDATORY DEDUCTION TYPE #%d] \n",nvs+1,(z+1),(z+1));

					t=stralloccat(t,temp);
				}
				t=stralloccat(t,"ELSE RETURN_VALUE=\"\" \n");
				for(z=nvs;z<(Deduction_Types->man_dedtypes->numlibs+1);++z)
				{
					t=stralloccat(t,"FI ");
				}
				} else {
					t=stralloccat(t,"\"\"");
				}
				memset(stemp,0,101);
				sprintf(stemp,"MANDATORY SLOT TYPE #%d",(nvs+1));
				addvirtualline(tmp,stemp,PLAINTEXT,15,t,1);
				if(t!=NULL) Rfree(t);

				memset(temp,0,15000);
				if(nvs<Deduction_Types->man_dedtypes->numlibs)
				{
				sprintf(temp,
					"IF([MANDATORY SLOT #%d])=0) THEN RETURN_VALUE=0 \n",nvs+1);
				t=stralloc(temp);
				for(z=nvs;z<(Deduction_Types->man_dedtypes->numlibs);++z)
				{
					sprintf(temp,
						"ELSE IF([MANDATORY SLOT #%d]=%d) THEN RETURN_VALUE=[TOTAL MANDATORY DEDUCTION AMOUNT #%d] \n",nvs+1,(z+1),(z+1));


					t=stralloccat(t,temp);
				}
				t=stralloccat(t,"ELSE RETURN_VALUE=0 \n");
				for(z=nvs;z<(Deduction_Types->man_dedtypes->numlibs+1);++z)
				{
					t=stralloccat(t,"FI ");
				}
				} else {
					t=stralloccat(t,"0.00");
				}
				memset(stemp,0,101);
				sprintf(stemp,"MANDATORY SLOT AMOUNT #%d",(nvs+1));
				addvirtualline(tmp,stemp,DECIMALV,15,t,1);
				if(t!=NULL) Rfree(t);
					
				memset(temp,0,15000);
				if(nvs<Deduction_Types->man_dedtypes->numlibs)
				{
				sprintf(temp,
					"IF([MANDATORY SLOT #%d])=0) THEN RETURN_VALUE=0 \n",nvs+1);
				t=stralloc(temp);
				for(z=nvs;z<(Deduction_Types->man_dedtypes->numlibs);++z)
				{
					sprintf(temp,
						"ELSE IF([MANDATORY SLOT #%d]=%d) THEN RETURN_VALUE=[MAXIMUM MANDATORY DEDUCTION YTD #%d]+[TOTAL MANDATORY DEDUCTION AMOUNT #%d] \n",nvs+1,(z+1),(z+1),(z+1));

					t=stralloccat(t,temp);
				}
				t=stralloccat(t,"ELSE RETURN_VALUE=0 \n");
				for(z=nvs;z<(Deduction_Types->man_dedtypes->numlibs+1);++z)
				{
					t=stralloccat(t,"FI ");
				}
				} else {
					t=stralloccat(t,"0.00");
				}
				memset(stemp,0,101);
				sprintf(stemp,"MANDATORY SLOT YTD #%d",(nvs+1));
				addvirtualline(tmp,stemp,SDECIMALV,15,t,1);
				if(t!=NULL) Rfree(t);

			}
		}
	}
	if(Deduction_Types->vol_dedtypes!=NULL)
	{
		if((Deduction_Types->vol_dedtypes->numlibs)>0)
		{
			for(z=0;z<(Deduction_Types->vol_dedtypes->numlibs);++z)
			{
			
				if(expression==NULL)
				{
					expression=Rmalloc(RDAstrlen(Deduction_Types->vol_dedtypes->libs[z])+77);
					sprintf(expression,
						"IF([PRIMARY FILE NUMBER]=1) \nTHEN IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"%s\")",
(Deduction_Types->vol_dedtypes->libs[z]==NULL?"":Deduction_Types->vol_dedtypes->libs[z]));
					expoffset=RDAstrlen(expression);
				} else { 
					expression=Rrealloc(expression,RDAstrlen(expression)+RDAstrlen(Deduction_Types->vol_dedtypes->libs[z])+46);
					sprintf(&expression[expoffset],
						"\n  OR ([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"%s\")",
(Deduction_Types->vol_dedtypes->libs[z]==NULL?"":Deduction_Types->vol_dedtypes->libs[z]));
					expoffset=(RDAstrlen(expression));
				}
				memset(stemp,0,101);
				sprintf(stemp,"VOLUNTARY DEDUCTION TYPE #%d",(z+1));
				t=Rmalloc(RDAstrlen(Deduction_Types->vol_dedtypes->libs[z])+3);
				sprintf(t,
					"\"%s\"",
					(Deduction_Types->vol_dedtypes->libs[z]==NULL?"":Deduction_Types->vol_dedtypes->libs[z]));
				addvirtualline(tmp,stemp,PLAINTEXT,15,t,0);
				if(t!=NULL) Rfree(t);

				t=Rmalloc(RDAstrlen(Deduction_Types->vol_dedtypes->libs[z])+400);
				sprintf(t,
					"IF([PRIMARY FILE NUMBER]=1) \nTHEN IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"%s\") AND ([PAYROLL][PAYDDMS][REDUCE NET])) \n     THEN RETURN_VALUE=(([PAYROLL][PAYJDPM][AMOUNT])/100) \n     ELSE RETURN_VALUE=0 \n     FI \nELSE RETURN_VALUE=0 \nFI",
					(Deduction_Types->vol_dedtypes->libs[z]==NULL?"":Deduction_Types->vol_dedtypes->libs[z]));
				memset(stemp,0,101);
				sprintf(stemp,"VOLUNTARY DEDUCTION AMOUNT #%d",(z+1));
				addvirtualline(tmp,stemp,SDECIMALV,15,t,0);
				if(t!=NULL) Rfree(t);

				t=Rmalloc(RDAstrlen(Deduction_Types->vol_dedtypes->libs[z])+400);
				sprintf(t,
					"IF([PRIMARY FILE NUMBER]=1) \nTHEN IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"%s\") AND ([PAYROLL][PAYDDMS][REDUCE NET])) \n     THEN RETURN_VALUE=(([ANNUAL CALENDAR PERSONNEL DEDUCTION TOTAL TO DATE])/100) \n     ELSE RETURN_VALUE=0 \n     FI \nELSE RETURN_VALUE=0 \nFI",
					(Deduction_Types->vol_dedtypes->libs[z]==NULL?"":Deduction_Types->vol_dedtypes->libs[z]));
				memset(stemp,0,101);
				sprintf(stemp,"VOLUNTARY DEDUCTION YTD #%d",(z+1));
				addvirtualline(tmp,stemp,SDECIMALV,15,t,0);
				if(t!=NULL) Rfree(t);

			}
			for(z=0;z<(Deduction_Types->vol_dedtypes->numlibs);++z)
			{
				memset(temp,0,15000);
				if(t==NULL)
				{
					sprintf(temp,"([VOLUNTARY DEDUCTION AMOUNT #%d]+",(z+1));
				} else if((z+1)==Deduction_Types->vol_dedtypes->numlibs)
				{
					sprintf(temp,"[VOLUNTARY DEDUCTION AMOUNT #%d]+",(z+1));
				} else {
					sprintf(temp,"[VOLUNTARY DEDUCTION AMOUNT #%d])",(z+1));
				}
				t=stralloccat(t,temp);
			}
			if(isEMPTY(t)) t=stralloc("0.00");
			addvirtuallineADV(tmp,"VOLUNTARY DEDUCTION AMOUNT TOTAL",6,25,t,1,0);

			if(t!=NULL) Rfree(t);

			for(z=0;z<(Deduction_Types->vol_dedtypes->numlibs);++z)
			{
				memset(temp,0,15000);
				if(t==NULL)
				{
					sprintf(temp,"([VOLUNTARY DEDUCTION YTD #%d]+",(z+1));
				} else if((z+1)==Deduction_Types->vol_dedtypes->numlibs)
				{
					sprintf(temp,"[VOLUNTARY DEDUCTION YTD #%d]+",(z+1));
				} else {
					sprintf(temp,"[VOLUNTARY DEDUCTION YTD #%d])",(z+1));
				}
				t=stralloccat(t,temp);
			}
			if(isEMPTY(t)) t=stralloc("0.00");
			addvirtuallineADV(tmp,"VOLUNTARY DEDUCTION YTD TOTAL",6,25,t,1,0);

			if(t!=NULL) Rfree(t);

			for(nvs=0;nvs<Deduction_Types->vol_dedtypes->numlibs;++nvs)
			{
				memset(temp,0,15000);
				if(nvs<Deduction_Types->vol_dedtypes->numlibs)
				{
				if(nvs>0)
				{
					sprintf(temp,
						"IF([VOLUNTARY SLOT #%d]=0) THEN RETURN_VALUE=0 \n",(nvs));
					t=stralloc(temp);
				}
				for(z=nvs;z<(Deduction_Types->vol_dedtypes->numlibs);++z)
				{
					if(z==0)
					{
						sprintf(temp,
							"IF(!([TOTAL VOLUNTARY DEDUCTION AMOUNT #%d]=0)) THEN RETURN_VALUE=%d \n",(z+1),(z+1));
					} else { 
						if(nvs>0)
						{
						sprintf(temp,
							"ELSE IF((!([TOTAL VOLUNTARY DEDUCTION AMOUNT #%d]=0)) AND ([VOLUNTARY SLOT #%d]<%d)) THEN RETURN_VALUE=%d \n",(z+1),nvs,(z+1),(z+1));
						} else {
						sprintf(temp,
							"ELSE IF(!([TOTAL VOLUNTARY DEDUCTION AMOUNT #%d]=0)) THEN RETURN_VALUE=%d \n",(z+1),(z+1));
						}
					}
					t=stralloccat(t,temp);
				}
				t=stralloccat(t,"ELSE RETURN_VALUE=(0) \n");
				for(z=nvs;z<(Deduction_Types->vol_dedtypes->numlibs);++z)
				{
					t=stralloccat(t,"FI ");
				}
				} else {
					t=stralloccat(t,"0");
				}
				memset(stemp,0,101);
				sprintf(stemp,"VOLUNTARY SLOT #%d",(nvs+1));
				addvirtualline(tmp,stemp,LONGV,4,t,1);
				if(t!=NULL) Rfree(t);
				

				memset(temp,0,15000);
				if(nvs<Deduction_Types->vol_dedtypes->numlibs)
				{
				sprintf(temp,
					"IF([VOLUNTARY SLOT #%d])=0) THEN RETURN_VALUE=\"\" \n",nvs+1);
				t=stralloc(temp);
				for(z=nvs;z<(Deduction_Types->vol_dedtypes->numlibs);++z)
				{
					sprintf(temp,
						"ELSE IF([VOLUNTARY SLOT #%d]=%d) THEN RETURN_VALUE=[LAST VOLUNTARY DEDUCTION TYPE #%d] \n",nvs+1,(z+1),(z+1));

					t=stralloccat(t,temp);
				}
				t=stralloccat(t,"ELSE RETURN_VALUE=\"\" \n");
				for(z=nvs;z<(Deduction_Types->vol_dedtypes->numlibs+1);++z)
				{
					t=stralloccat(t,"FI ");
				}
				} else {
					t=stralloccat(t,"\"\"");
				}
				memset(stemp,0,101);
				sprintf(stemp,"VOLUNTARY SLOT TYPE #%d",(nvs+1));
				addvirtualline(tmp,stemp,PLAINTEXT,15,t,1);
				if(t!=NULL) Rfree(t);

				memset(temp,0,15000);
				if(nvs<Deduction_Types->vol_dedtypes->numlibs)
				{
				sprintf(temp,
					"IF([VOLUNTARY SLOT #%d])=0) THEN RETURN_VALUE=0 \n",nvs+1);
				t=stralloc(temp);
				for(z=nvs;z<(Deduction_Types->vol_dedtypes->numlibs);++z)
				{
					sprintf(temp,
						"ELSE IF([VOLUNTARY SLOT #%d]=%d) THEN RETURN_VALUE=[TOTAL VOLUNTARY DEDUCTION AMOUNT #%d] \n",nvs+1,(z+1),(z+1));

					t=stralloccat(t,temp);
				}
				t=stralloccat(t,"ELSE RETURN_VALUE=0 \n");
				for(z=nvs;z<(Deduction_Types->vol_dedtypes->numlibs+1);++z)
				{
					t=stralloccat(t,"FI ");
				}
				} else {
					t=stralloccat(t,"0.00");
				}
				memset(stemp,0,101);
				sprintf(stemp,"VOLUNTARY SLOT AMOUNT #%d",(nvs+1));
				addvirtualline(tmp,stemp,DECIMALV,15,t,1);
				if(t!=NULL) Rfree(t);
					
				memset(temp,0,15000);
				if(nvs<Deduction_Types->vol_dedtypes->numlibs)
				{
				sprintf(temp,
					"IF([VOLUNTARY SLOT #%d])=0) THEN RETURN_VALUE=0 \n",nvs+1);
				t=stralloc(temp);
				for(z=nvs;z<(Deduction_Types->vol_dedtypes->numlibs);++z)
				{
					sprintf(temp,
						"ELSE IF([VOLUNTARY SLOT #%d]=%d) THEN RETURN_VALUE=[MAXIMUM VOLUNTARY DEDUCTION YTD #%d]+[TOTAL VOLUNTARY DEDUCTION AMOUNT #%d] \n",nvs+1,(z+1),(z+1),(z+1));

					t=stralloccat(t,temp);
				}
				t=stralloccat(t,"ELSE RETURN_VALUE=0 \n");
				for(z=nvs;z<(Deduction_Types->vol_dedtypes->numlibs+1);++z)
				{
					t=stralloccat(t,"FI ");
				}
				} else {
					t=stralloccat(t,"0.00");
				}
				memset(stemp,0,101);
				sprintf(stemp,"VOLUNTARY SLOT YTD #%d",(nvs+1));
				addvirtualline(tmp,stemp,SDECIMALV,15,t,1);
				if(t!=NULL) Rfree(t);

			}
		}
	}
	t=Rmalloc(380);
	sprintf(t,"%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]=1) \n",
		"THEN RETURN_VALUE=1 \n",
		"ELSE IF([PRIMARY FILE NUMBER]=2) \n",
		"     THEN RETURN_VALUE=0 \n",
		"ELSE RETURN_VALUE=(-1) FI FI\n");
	addvirtualline(tmp,"WHICH TYPE",SSHORTV,2,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(450);
	sprintf(t,"%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]=1) \n",
		"THEN RETURN_VALUE=[PAYROLL][PAYDDMS][DEDUCTION TYPE] \n",
		"ELSE IF([PRIMARY FILE NUMBER]=2) \n",
		"     THEN RETURN_VALUE=[PAYROLL][PAYJMST][POSITION IDENTIFICATION]+\" \"+[PAYROLL][PAYJPRM][RATE IDENTIFICATION] \n",
		"ELSE IF([PRIMARY FILE NUMBER]=3) \n",
		"     THEN RETURN_VALUE=[LVEMGT][LVEDSC][LEAVE TYPE IDENTIFICATION] \n",
		"ELSE RETURN_VALUE=(\"\") FI FI FI");
	addvirtualline(tmp,"IDENTIFICATION",PLAINTEXT,30,t,0);
	if(t!=NULL) Rfree(t);

	if(lvedsclist!=NULL)
	{
		for(z=0;z<lvedsclist->numlibs;++z)
		{
			memset(stemp,0,101);
			sprintf(stemp,"LEAVE DESCRIPTION IDENTIFICATION #%d",(z+1));
			expression=Rmalloc(RDAstrlen(lvedsclist->libs[z])+3);
			sprintf(expression,"\"%s\"",lvedsclist->libs[z]);
			addvirtualline(tmp,stemp,PLAINTEXT,15,expression,0);
			if(expression!=NULL) Rfree(expression);

			expression=Rmalloc(RDAstrlen(stemp)+300);
			sprintf(expression,"%s%s%d%s%s%s%s%s%d%s",
				"IF(!([PRIMARY FILE NUMBER]=2)) THEN RETURN_VALUE=0 \n",
				"ELSE IF(!([LVEMGT][LVEMSTR][LEAVE DESCRIPTION IDENTIFICATION][",
				(z+1),
				",1,40]=[",
				stemp,
				"])) \n",
				"     THEN RETURN_VALUE=0 \n",
				"     ELSE RETURN_VALUE=[LVEMGT][LVEMSTR][BEGINNING BALANCE][",
				(z+1),
				",1,10] FI FI");
			t=Rmalloc(30+6);
			sprintf(t,"LEAVE BEGINNING BALANCE #%d",(z+1));
			addvirtualline(tmp,t,SDECIMALV,8,expression,0);
			if(expression!=NULL) Rfree(expression);
			if(t!=NULL) Rfree(t);

			expression=Rmalloc(RDAstrlen(stemp)+300);
			sprintf(expression,"%s%s%d%s%s%s%s%s%d%s",
				"IF(!([PRIMARY FILE NUMBER]=2)) THEN RETURN_VALUE=0 \n",
				"ELSE IF(!([LVEMGT][LVEMSTR][LEAVE DESCRIPTION IDENTIFICATION][",
				(z+1),
				",1,40]=[",
				stemp,
				"])) \n",
				"     THEN RETURN_VALUE=0 \n",
				"     ELSE RETURN_VALUE=[LVEMGT][LVEMSTR][PROCESSED EARNINGS][",
				(z+1),
				",1,10] FI FI");
			t=Rmalloc(30+6);
			sprintf(t,"LEAVE PROCESSED EARNINGS #%d",(z+1));
			addvirtualline(tmp,t,SDECIMALV,8,expression,0);
			if(expression!=NULL) Rfree(expression);
			if(t!=NULL) Rfree(t);

			expression=Rmalloc(RDAstrlen(stemp)+302);
			sprintf(expression,"%s%s%d%s%s%s%s%s%d%s",
				"IF(!([PRIMARY FILE NUMBER]=2)) THEN RETURN_VALUE=0 \n",
				"ELSE IF(!([LVEMGT][LVEMSTR][LEAVE DESCRIPTION IDENTIFICATION][",
				(z+1),
				",1,40]=[",
				stemp,
				"])) \n",
				"     THEN RETURN_VALUE=0 \n",
				"     ELSE RETURN_VALUE=[LVEMGT][LVEMSTR][UNPROCESSED EARNINGS][",
				(z+1),
				",1,10] FI FI");
			t=Rmalloc(32+6);
			sprintf(t,"LEAVE UNPROCESSED EARNINGS #%d",(z+1));
			addvirtualline(tmp,t,SDECIMALV,8,expression,0);
			if(expression!=NULL) Rfree(expression);
			if(t!=NULL) Rfree(t);

			expression=Rmalloc(RDAstrlen(stemp)+300);
			sprintf(expression,"%s%s%d%s%s%s%s%s%d%s",
				"IF(!([PRIMARY FILE NUMBER]=2)) THEN RETURN_VALUE=0 \n",
				"ELSE IF(!([LVEMGT][LVEMSTR][LEAVE DESCRIPTION IDENTIFICATION][",
				(z+1),
				",1,40]=[",
				stemp,
				"])) \n",
				"     THEN RETURN_VALUE=0 \n",
				"     ELSE RETURN_VALUE=[LVEMGT][LVEMSTR][PROCESSED USED][",
				(z+1),
				",1,10] FI FI");
			t=Rmalloc(30+6);
			sprintf(t,"LEAVE PROCESSED USED #%d",(z+1));
			addvirtualline(tmp,t,SDECIMALV,8,expression,0);
			if(expression!=NULL) Rfree(expression);
			if(t!=NULL) Rfree(t);

			expression=Rmalloc(RDAstrlen(stemp)+302);
			sprintf(expression,"%s%s%d%s%s%s%s%s%d%s",
				"IF(!([PRIMARY FILE NUMBER]=2)) THEN RETURN_VALUE=0 \n",
				"ELSE IF(!([LVEMGT][LVEMSTR][LEAVE DESCRIPTION IDENTIFICATION][",
				(z+1),
				",1,40]=[",
				stemp,
				"])) \n",
				"     THEN RETURN_VALUE=0 \n",
				"     ELSE RETURN_VALUE=[LVEMGT][LVEMSTR][UNPROCESSED USED][",
				(z+1),
				",1,10] FI FI");
			t=Rmalloc(32+6);
			sprintf(t,"LEAVE UNPROCESSED USED #%d",(z+1));
			addvirtualline(tmp,t,SDECIMALV,8,expression,0);
			if(expression!=NULL) Rfree(expression);
			if(t!=NULL) Rfree(t);

			expression=Rmalloc(RDAstrlen(stemp)+450);
			sprintf(expression,"%s%s%d%s%s%s%s%s%d%s%d%s%d%s%d%s%d%s",
				"IF(!([PRIMARY FILE NUMBER]=2)) THEN RETURN_VALUE=0 \n",
				"ELSE IF(!([LVEMGT][LVEMSTR][LEAVE DESCRIPTION IDENTIFICATION][",
				(z+1),
				",1,40]=[",
				stemp,
				"])) \n",
				"     THEN RETURN_VALUE=0 \n",
				"     ELSE RETURN_VALUE=([LVEMGT][LVEMSTR][PROCESSED EARNINGS][",
				(z+1),
				",1,10]+[LVEMGT][LVEMSTR][UNPROCESSED EARNINGS][",
				(z+1),",1,10]+[LVEMGT][LVEMSTR][BEGINNING BALANCE][",
				(z+1),
				",1,10]-[LVEMGT][LVEMSTR][PROCESSED USED][",
				(z+1),
				",1,10]-[LVEMGT][LVEMSTR][UNPROCESSED USED][",
				(z+1),",1,10]) FI FI");
			t=Rmalloc(30+6);
			sprintf(t,"LEAVE AVAILABLE #%d",(z+1));
			addvirtualline(tmp,t,SDECIMALV,8,expression,0);
			if(expression!=NULL) Rfree(expression);
			if(t!=NULL) Rfree(t);
		}
		for(nvs=0;nvs<lvedsclist->numlibs;++nvs)
		{
			memset(temp,0,15000);
			if(nvs<lvedsclist->numlibs)
			{
			if(nvs>0)
			{
				sprintf(temp,
					"IF([LEAVE SLOT #%d]=0) THEN RETURN_VALUE=0 \n",(nvs));
				t=stralloc(temp);
			}
			for(z=0;z<lvedsclist->numlibs;++z)
			{
				if(z==0 && nvs==0)
				{
					sprintf(temp,
						"IF(([LAST LEAVE BEGINNING BALANCE #%d]!=0) OR  ([LAST LEAVE PROCESSED EARNINGS #%d]!=0) OR ([LAST LEAVE PROCESSED USED #%d]!=0) OR ([LAST LEAVE UNPROCESSED EARNINGS #%d]!=0) OR ([LAST LEAVE UNPROCESSED USED #%d]!=0)) THEN RETURN_VALUE=%d \n",
						(z+1),(z+1),(z+1),(z+1),(z+1),(z+1));
				} else { 
					if(nvs>0)
					{
					sprintf(temp,
						"ELSE IF((([LAST LEAVE BEGINNING BALANCE #%d]!=0) OR ([LAST LEAVE PROCESSED EARNINGS #%d]!=0) OR ([LAST LEAVE PROCESSED USED #%d]!=0) OR ([LAST LEAVE UNPROCESSED EARNINGS #%d]!=0) OR ([LAST LEAVE UNPROCESSED USED #%d]!=0)) AND ([LEAVE SLOT #%d]<%d)) THEN RETURN_VALUE=%d \n",
						(z+1),(z+1),(z+1),(z+1),(z+1),nvs,(z+1),(z+1));
					} else {
					sprintf(temp,
						"ELSE IF(([LAST LEAVE BEGINNING BALANCE #%d]!=0) OR ([LAST LEAVE PROCESSED EARNINGS #%d]!=0) OR ([LAST LEAVE PROCESSED USED #%d]!=0) OR ([LAST LEAVE UNPROCESSED EARNINGS #%d]!=0) OR ([LAST LEAVE UNPROCESSED USED #%d]!=0)) THEN RETURN_VALUE=%d \n",
						(z+1),(z+1),(z+1),(z+1),(z+1),(z+1));
					}
				}
				t=stralloccat(t,temp);
			}
			t=stralloccat(t,"ELSE RETURN_VALUE=0 \n");
			for(z=nvs;z<(lvedsclist->numlibs);++z)
			{
				t=stralloccat(t,"FI ");
			}
			} else {
				t=stralloccat(t,"0");
			}
			memset(stemp,0,101);
			sprintf(stemp,"LEAVE SLOT #%d",(nvs+1));
			addvirtualline(tmp,stemp,LONGV,4,t,1);
			if(t!=NULL) Rfree(t);
			

			memset(temp,0,15000);
			if(nvs<lvedsclist->numlibs)
			{
			sprintf(temp,
				"IF([LEAVE SLOT #%d])=0) THEN RETURN_VALUE=\"\" \n",nvs+1);
			t=stralloc(temp);
			for(z=nvs;z<(lvedsclist->numlibs);++z)
			{
				sprintf(temp,
					"ELSE IF([LEAVE SLOT #%d]=%d) THEN RETURN_VALUE=[LAST LEAVE DESCRIPTION IDENTIFICATION #%d] \n",nvs+1,(z+1),(z+1));

				t=stralloccat(t,temp);
			}
			t=stralloccat(t,"ELSE RETURN_VALUE=\"\" \n");
			for(z=nvs;z<(lvedsclist->numlibs+1);++z)
			{
				t=stralloccat(t,"FI ");
			}
			} else {
				t=stralloccat(t,"\"\"");
			}
			memset(stemp,0,101);
			sprintf(stemp,"LEAVE SLOT TYPE #%d",(nvs+1));
			addvirtualline(tmp,stemp,PLAINTEXT,15,t,1);
			if(t!=NULL) Rfree(t);

			memset(temp,0,15000);
			if(nvs<lvedsclist->numlibs)
			{
			sprintf(temp,
				"IF([LEAVE SLOT #%d])=0) THEN RETURN_VALUE=0 \n",nvs+1);
			t=stralloc(temp);
			for(z=nvs;z<(lvedsclist->numlibs);++z)
			{
				sprintf(temp,
					"ELSE IF([LEAVE SLOT #%d]=%d) THEN RETURN_VALUE=[LAST LEAVE PROCESSED EARNINGS #%d] \n",nvs+1,(z+1),(z+1));

				t=stralloccat(t,temp);
			}
			t=stralloccat(t,"ELSE RETURN_VALUE=0 \n");
			for(z=nvs;z<(lvedsclist->numlibs+1);++z)
			{
				t=stralloccat(t,"FI ");
			}
			} else {
				t=stralloccat(t,"0");
			}
			memset(stemp,0,101);
			sprintf(stemp,"LEAVE SLOT PROCESSED EARNINGS #%d",(nvs+1));
			addvirtualline(tmp,stemp,SDECIMALV,15,t,1);
			if(t!=NULL) Rfree(t);
				
			memset(temp,0,15000);
			if(nvs<lvedsclist->numlibs)
			{
			sprintf(temp,
				"IF([LEAVE SLOT #%d])=0) THEN RETURN_VALUE=0 \n",nvs+1);
			t=stralloc(temp);
			for(z=nvs;z<(lvedsclist->numlibs);++z)
			{
				sprintf(temp,
					"ELSE IF([LEAVE SLOT #%d]=%d) THEN RETURN_VALUE=[LAST LEAVE BEGINNING BALANCE #%d] \n",nvs+1,(z+1),(z+1));

				t=stralloccat(t,temp);
			}
			t=stralloccat(t,"ELSE RETURN_VALUE=0 \n");
			for(z=nvs;z<(lvedsclist->numlibs+1);++z)
			{
				t=stralloccat(t,"FI ");
			}
			} else {
				t=stralloccat(t,"0");
			}
			memset(stemp,0,101);
			sprintf(stemp,"LEAVE SLOT BEGINNING BALANCE #%d",(nvs+1));
			addvirtualline(tmp,stemp,SDECIMALV,15,t,1);
			if(t!=NULL) Rfree(t);

			memset(temp,0,15000);
			if(nvs<lvedsclist->numlibs)
			{
			sprintf(temp,
				"IF([LEAVE SLOT #%d])=0) THEN RETURN_VALUE=0 \n",nvs+1);
			t=stralloc(temp);
			for(z=nvs;z<(lvedsclist->numlibs);++z)
			{
				sprintf(temp,
					"ELSE IF([LEAVE SLOT #%d]=%d) THEN RETURN_VALUE=[LAST LEAVE UNPROCESSED EARNINGS #%d] \n",nvs+1,(z+1),(z+1));

				t=stralloccat(t,temp);
			}
			t=stralloccat(t,"ELSE RETURN_VALUE=0 \n");
			for(z=nvs;z<(lvedsclist->numlibs+1);++z)
			{
				t=stralloccat(t,"FI ");
			}
			} else {
				t=stralloccat(t,"0");
			}
			memset(stemp,0,101);
			sprintf(stemp,"LEAVE SLOT UNPROCESSED EARNINGS #%d",(nvs+1));
			addvirtualline(tmp,stemp,SDECIMALV,15,t,1);
			if(t!=NULL) Rfree(t);

			memset(temp,0,15000);
			if(nvs<lvedsclist->numlibs)
			{
			sprintf(temp,
				"IF([LEAVE SLOT #%d])=0) THEN RETURN_VALUE=0 \n",nvs+1);
			t=stralloc(temp);
			for(z=nvs;z<(lvedsclist->numlibs);++z)
			{
				sprintf(temp,
					"ELSE IF([LEAVE SLOT #%d]=%d) THEN RETURN_VALUE=[LAST LEAVE PROCESSED USED #%d] \n",nvs+1,(z+1),(z+1));

				t=stralloccat(t,temp);
			}
			t=stralloccat(t,"ELSE RETURN_VALUE=0 \n");
			for(z=nvs;z<(lvedsclist->numlibs+1);++z)
			{
				t=stralloccat(t,"FI ");
			}
			} else {
				t=stralloccat(t,"0");
			}
			memset(stemp,0,101);
			sprintf(stemp,"LEAVE SLOT PROCESSED USED #%d",(nvs+1));
			addvirtualline(tmp,stemp,SDECIMALV,15,t,1);
			if(t!=NULL) Rfree(t);
				
			memset(temp,0,15000);
			if(nvs<lvedsclist->numlibs)
			{
			sprintf(temp,
				"IF([LEAVE SLOT #%d])=0) THEN RETURN_VALUE=0 \n",nvs+1);
			t=stralloc(temp);
			for(z=nvs;z<(lvedsclist->numlibs);++z)
			{
				sprintf(temp,
					"ELSE IF([LEAVE SLOT #%d]=%d) THEN RETURN_VALUE=[LAST LEAVE UNPROCESSED USED #%d] \n",nvs+1,(z+1),(z+1));

				t=stralloccat(t,temp);
			}
			t=stralloccat(t,"ELSE RETURN_VALUE=0 \n");
			for(z=nvs;z<(lvedsclist->numlibs+1);++z)
			{
				t=stralloccat(t,"FI ");
			}
			} else {
				t=stralloccat(t,"0");
			}
			memset(stemp,0,101);
			sprintf(stemp,"LEAVE SLOT UNPROCESSED USED #%d",(nvs+1));
			addvirtualline(tmp,stemp,SDECIMALV,15,t,1);
			if(t!=NULL) Rfree(t);

				
			t=Rmalloc(85);
			sprintf(t,
				"[LEAVE SLOT PROCESSED EARNINGS #%d]+[LEAVE SLOT UNPROCESSED EARNINGS #%d]",(nvs+1),(nvs+1));
			memset(stemp,0,101);
			sprintf(stemp,"LEAVE SLOT YTD EARNINGS #%d",(nvs+1));
			addvirtualline(tmp,stemp,SDECIMALV,15,t,1);
			if(t!=NULL) Rfree(t);

			t=Rmalloc(85);
			sprintf(t,
				"[LEAVE SLOT PROCESSED USED #%d]+[LEAVE SLOT UNPROCESSED USED #%d]",(nvs+1),(nvs+1));
			memset(stemp,0,101);
			sprintf(stemp,"LEAVE SLOT YTD USED #%d",(nvs+1));
			addvirtualline(tmp,stemp,SDECIMALV,15,t,1);
			if(t!=NULL) Rfree(t);


			memset(temp,0,15000);
			if(nvs<lvedsclist->numlibs)
			{
			sprintf(temp,
				"IF([LEAVE SLOT #%d])=0) THEN RETURN_VALUE=0 \n",nvs+1);
			t=stralloc(temp);
			for(z=nvs;z<(lvedsclist->numlibs);++z)
			{
				sprintf(temp,
					"ELSE IF([LEAVE SLOT #%d]=%d) THEN RETURN_VALUE=[LAST LEAVE AVAILABLE #%d] \n",nvs+1,(z+1),(z+1));

				t=stralloccat(t,temp);
			}
			t=stralloccat(t,"ELSE RETURN_VALUE=0 \n");
			for(z=nvs;z<(lvedsclist->numlibs+1);++z)
			{
				t=stralloccat(t,"FI ");
			}
			} else {
				t=stralloccat(t,"0");
			}
			memset(stemp,0,101);
			sprintf(stemp,"LEAVE SLOT AVAILABLE #%d",(nvs+1));
			addvirtualline(tmp,stemp,SDECIMALV,15,t,1);
			if(t!=NULL) Rfree(t);
		}
	}

	if(t!=NULL) Rfree(t);
	t=Rmalloc(277+1);
	sprintf(t,"IF(!([PRIMARY FILE NUMBER]=1)) \nTHEN RETURN_VALUE=0 \nELSE IF(([PAYROLL][PAYDDMS][REDUCE NET]=TRUE) \n       AND (!([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"DIRECT DEPOSIT\"))) \n     THEN RETURN_VALUE=(([PAYRO\
LL][PAYJDPM][AMOUNT])/100) \n     ELSE RETURN_VALUE=0 \n     FI \nFI");
	addvirtuallineADV(tmp,"REDUCE NET AMOUNT",20,15,t,0,0);

	if(t!=NULL) Rfree(t);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(4+1);
	sprintf(t,"\"\"");
	addvirtuallineADV(tmp,"ZERO STRING",0,0,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(6+1);
	sprintf(t,"(0.00)");
	addvirtuallineADV(tmp,"ZERO AMOUNT",20,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(384+1);
	sprintf(t,"IF(!([PRIMARY FILE NUMBER]=1)) \nTHEN RETURN_VALUE=0 \nELSE IF([PAYROLL][PAYDDMS][REDUCE NET]=FALSE) \n     THEN RETURN_VALUE=0 \n     ELSE IF(([PAYROLL][PAYDTYP][VOLUNTARY]=TRUE) \n            AND (!([PAYR\
OLL][PAYDDMS][DEDUCTION TYPE]=\"DIRECT DEPOSIT\"))) \n          THEN RETURN_VALUE=([PAYROLL][PAYJDPM][AMOUNT]/100) \n          ELSE RETURN_VALUE=0 \n          FI \n     FI \nFI");
	addvirtuallineADV(tmp,"VOLUNTARY DEDUCTION AMOUNTS",6,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(254+1);
	sprintf(t,"IF(!([PRIMARY FILE NUMBER]=1)) \nTHEN RETURN_VALUE=0 \nELSE IF(([PAYROLL][PAYDDMS][REDUCE NET]=TRUE) \n       AND ([PAYROLL][PAYDTYP][VOLUNTARY]=FALSE)) \n     THEN RETURN_VALUE=([PAYROLL][PAYJDPM][AMOUNT]\
/100) \n     ELSE RETURN_VALUE=0 \n     FI \nFI");
	addvirtuallineADV(tmp,"MANDITORY DEDUCTION AMOUNTS",20,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(3433+1);
	sprintf(t,"%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"IF ([PAYROLL][PAYMSTR][MONTH]=0) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT])  \n",
		"ELSE IF ([PAYROLL][PAYMSTR][MONTH]=1) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT])  \n",
		"ELSE IF ([PAYROLL][PAYMSTR][MONTH]=2) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]+[PAYROLL][PAYDCSM][MARCH AMOUNT])  \n",
		"ELSE IF ([PAYROLL][PAYMSTR][MONTH]=3) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]+[PAYROLL][PAYDCSM][MARCH AMOUNT]+[PAYROLL][PAYDCSM][APRIL AMOUNT])  \n",
		"ELSE IF ([PAYROLL][PAYMSTR][MONTH]=4) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]+[PAYROLL][PAYDCSM][MARCH AMOUNT]+[PAYROLL][PAYDCSM][APRIL AMOUNT]+[PAYROLL][PAYDCSM][MAY AMOUNT])  \n",
		"ELSE IF ([PAYROLL][PAYMSTR][MONTH]=5) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]+[PAYROLL][PAYDCSM][MARCH AMOUNT]+[PAYROLL][PAYDCSM][APRIL AMOUNT]+[PAYROLL][PAYDCSM][MAY AMOUNT]+[PAYROLL][PAYDCSM][JUNE AMOUNT])  \n",
		"ELSE IF ([PAYROLL][PAYMSTR][MONTH]=6) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]+[PAYROLL][PAYDCSM][MARCH AMOUNT]+[PAYROLL][PAYDCSM][APRIL AMOUNT]+[PAYROLL][PAYDCSM][MAY AMOUNT]+[PAYROLL][PAYDCSM][JUNE AMOUNT]+[PAYROLL][PAYDCSM][JULY AMOUNT])  \n",
		"ELSE IF ([PAYROLL][PAYMSTR][MONTH]=7) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]+[PAYROLL][PAYDCSM][MARCH AMOUNT]+[PAYROLL][PAYDCSM][APRIL AMOUNT]+[PAYROLL][PAYDCSM][MAY AMOUNT]+[PAYROLL][PAYDCSM][JUNE AMOUNT]+[PAYROLL][PAYDCSM][JULY AMOUNT]+[PAYROLL][PAYDCSM][AUGUST AMOUNT])  \n",
		"ELSE IF ([PAYROLL][PAYMSTR][MONTH]=8) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]+[PAYROLL][PAYDCSM][MARCH AMOUNT]+[PAYROLL][PAYDCSM][APRIL AMOUNT]+[PAYROLL][PAYDCSM][MAY AMOUNT]+[PAYROLL][PAYDCSM][JUNE AMOUNT]+[PAYROLL][PAYDCSM][JULY AMOUNT]+[PAYROLL][PAYDCSM][AUGUST AMOUNT]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT])  \n",
		"ELSE IF ([PAYROLL][PAYMSTR][MONTH]=9) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]+[PAYROLL][PAYDCSM][MARCH AMOUNT]+[PAYROLL][PAYDCSM][APRIL AMOUNT]+[PAYROLL][PAYDCSM][MAY AMOUNT]+[PAYROLL][PAYDCSM][JUNE AMOUNT]+[PAYROLL][PAYDCSM][JULY AMOUNT]+[PAYROLL][PAYDCSM][AUGUST AMOUNT]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT])  \n",
		"ELSE IF ([PAYROLL][PAYMSTR][MONTH]=10) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]+[PAYROLL][PAYDCSM][MARCH AMOUNT]+[PAYROLL][PAYDCSM][APRIL AMOUNT]+[PAYROLL][PAYDCSM][MAY AMOUNT]+[PAYROLL][PAYDCSM][JUNE AMOUNT]+[PAYROLL][PAYDCSM][JULY AMOUNT]+[PAYROLL][PAYDCSM][AUGUST AMOUNT]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT]+[PAYROLL][PAYDCSM][NOVEMBER AMOUNT])  \n",
		"ELSE IF ([PAYROLL][PAYMSTR][MONTH]=11) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT]+[PAYROLL][PAYDCSM][MARCH AMOUNT]+[PAYROLL][PAYDCSM][APRIL AMOUNT]+[PAYROLL][PAYDCSM][MAY AMOUNT]+[PAYROLL][PAYDCSM][JUNE AMOUNT]+[PAYROLL][PAYDCSM][JULY AMOUNT]+[PAYROLL][PAYDCSM][AUGUST AMOUNT]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT]+[PAYROLL][PAYDCSM][NOVEMBER AMOUNT]+[PAYROLL][PAYDCSM][DECEMBER AMOUNT])   ",
		"FI FI FI FI FI FI FI FI FI FI FI FI");
	addvirtuallineADV(tmp,"ANNUAL CALENDAR PERSONNEL DEDUCTION TOTAL TO DATE",20,20,t,0,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(327+1);
	sprintf(t,"IF(!([PRIMARY FILE NUMBER]=1)) \nTHEN RETURN_VALUE=0 \nELSE IF(([PAYROLL][PAYDDMS][REDUCE NET]=TRUE) \n       AND ([PAYROLL][PAYDTYP][VOLUNTARY]=TRUE) \n       AND (!([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"DIRECT DEPOSIT\"))) \n     THEN RETURN_VALUE=(([PAYROLL][PAYJDPM][AMOUNT])/100) \n     ELSE RETURN_VALUE=0 \n     FI \nFI");
	addvirtuallineADV(tmp,"DEDUCTION AMOUNT",20,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(111+1);
	sprintf(t,"IF(!([PRIMARY FILE NUMBER]=2)) \nTHEN RETURN_VALUE=0 \nELSE RETURN_VALUE=(([PAYROLL][PAYJPRM][GROSS])/100) \nFI");
	addvirtuallineADV(tmp,"GROSS AMOUNT",20,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(90+1);
	sprintf(t,"([MAXIMUM ANNUAL CALENDAR PERSONNEL GROSS TOTAL TO DATE]/100+[TOTAL GROSS AMOUNT])");
	addvirtuallineADV(tmp,"TOTAL YEAR TO DATE GROSS",SDECIMALV,20,t,1,0);


	if(t!=NULL) Rfree(t);
	t=Rmalloc(3413+1);
	sprintf(t,"%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"IF ([PAYROLL][PAYMSTR][MONTH]=0) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]) \n",
		"ELSE IF ([PAYROLL][PAYMSTR][MONTH]=1) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS])  \n",
		"ELSE IF ([PAYROLL][PAYMSTR][MONTH]=2) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS])  \n",
		"ELSE IF ([PAYROLL][PAYMSTR][MONTH]=3) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS])  \n",
		"ELSE IF ([PAYROLL][PAYMSTR][MONTH]=4) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS])  \n",
		"ELSE IF ([PAYROLL][PAYMSTR][MONTH]=5) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS]+[PAYROLL][PAYPCSM][JUNE GROSS])  \n",
		"ELSE IF ([PAYROLL][PAYMSTR][MONTH]=6) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS]+[PAYROLL][PAYPCSM][JUNE GROSS]+[PAYROLL][PAYPCSM][JULY GROSS])  \n",
		"ELSE IF ([PAYROLL][PAYMSTR][MONTH]=7) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS]+[PAYROLL][PAYPCSM][JUNE GROSS]+[PAYROLL][PAYPCSM][JULY GROSS]+[PAYROLL][PAYPCSM][AUGUST GROSS])  \n",
		"ELSE IF ([PAYROLL][PAYMSTR][MONTH]=8) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS]+[PAYROLL][PAYPCSM][JUNE GROSS]+[PAYROLL][PAYPCSM][JULY GROSS]+[PAYROLL][PAYPCSM][AUGUST GROSS]+[PAYROLL][PAYPCSM][SEPTEMBER GROSS])  \n",
		"ELSE IF ([PAYROLL][PAYMSTR][MONTH]=9) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS]+[PAYROLL][PAYPCSM][JUNE GROSS]+[PAYROLL][PAYPCSM][JULY GROSS]+[PAYROLL][PAYPCSM][AUGUST GROSS]+[PAYROLL][PAYPCSM][SEPTEMBER GROSS]+[PAYROLL][PAYPCSM][OCTOBER GROSS])  \n",
		"ELSE IF ([PAYROLL][PAYMSTR][MONTH]=10) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH\
 GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS]+[PAYROLL][PAYPCSM][JUNE GROSS]+[PAYROLL][PAYPCSM][JULY GROSS]+[PAYROLL][PAYPCSM][AUGUST GROSS]+[PAYROLL][PAYPCSM][SEPTEMBER GROSS]+[PAYROLL][PAYPCSM][OCTOBER GROSS]+[PAYROLL][PAYPCSM][NOVEMBER GROSS])  \n",
		"ELSE IF ([PAYROLL][PAYMSTR][MONTH]=11) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS]+[PAYROLL][PAYPCSM][JUNE GROSS]+[PAYROLL][PAYPCSM][JULY GROSS]+[PAYROLL][PAYPCSM][AUGUST GROSS]+",
		"[PAYROLL][PAYPCSM][SEPTEMBER GROSS]+[PAYROLL][PAYPCSM][OCTOBER GROSS]+[PAYROLL][PAYPCSM][NOVEMBER GROSS]+[PAYROLL][PAYPCSM][DECEMBER GROSS])  \nFI FI FI FI FI FI FI FI FI FI FI FI");
	addvirtuallineADV(tmp,"ANNUAL CALENDAR PERSONNEL GROSS TOTAL TO DATE",20,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(223+1);
	sprintf(t,"IF(!([PRIMARY FILE NUMBER]=1)) \nTHEN RETURN_VALUE=0 \nELSE IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"DIRECT DEPOSIT\")) \n     THEN RETURN_VALUE=(([PAYROLL][PAYJDPM][AMOUNT])/100) \n     ELSE RETURN_VALUE=0\n     FI \nFI");
	addvirtuallineADV(tmp,"DIRECT DEPOSIT",20,15,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(36+1);
	sprintf(t,"([GROSS AMOUNT]-[REDUCE NET AMOUNT])");
	addvirtuallineADV(tmp,"NET AMOUNT",20,15,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(48+1);
	sprintf(t,"([TOTAL GROSS AMOUNT]-[TOTAL REDUCE NET AMOUNT])");
	addvirtuallineADV(tmp,"SUM NET AMOUNT",20,15,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(42+1);
	sprintf(t,"(([GROSS AMOUNT]-[REDUCE NET AMOUNT])*100)");
	addvirtuallineADV(tmp,"FORMATTED NET AMOUNT",2,15,t,0,0);

	if(t!=NULL) Rfree(t);
	addvirtuallineADV(tmp,"DESCRIPTION LONG #1",1,70,t,1,2);

	if(t!=NULL) Rfree(t);
	addvirtuallineADV(tmp,"DESCRIPTION LONG #2",1,80,t,1,2);

	if(t!=NULL) Rfree(t);
	addvirtuallineADV(tmp,"DESCRIPTION LONG #3",1,80,t,1,2);

	if(t!=NULL) Rfree(t);
	addvirtuallineADV(tmp,"DESCRIPTION LONG #4",1,80,t,1,2);

	if(t!=NULL) Rfree(t);
	addvirtuallineADV(tmp,"DESCRIPTION LONG #5",1,80,t,1,2);

	if(t!=NULL) Rfree(t);
	addvirtuallineADV(tmp,"DESCRIPTION LONG #6",1,80,t,1,2);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(200+1);
	sprintf(t,"([PRSNNL][PERDMG][FIRST NAME]+\" \"+[PRSNNL][PERDMG][MIDDLE NAME]+\" \"+[PRSNNL][PERDMG][LAST NAME]+\" \"+[PRSNNL][PERDMG][LINEAGE])");
	addvirtuallineADV(tmp,"EMPLOYEE NAME",1,80,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(247+1);
	sprintf(t,"IF(!([PRIMARY FILE NUMBER]=1)) \nTHEN RETURN_VALUE=\"\" \nELSE IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"DIRECT DEPOSIT\")) \nTHEN RETURN_VALUE=[PAYROLL][PAYDEDM][BANK ACCOUNT NUMBER] \nELSE RETURN_VALUE=\"\" \nFI FI");
	addvirtuallineADV(tmp,"BANK ACCOUNT NUMBER",1,17,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(128+1);
	sprintf(t,"IF([LAST BANK ACCOUNT]!=\"\") THEN RETURN_VALUE=[LAST BANK ACCOUNT] \nELSE RETURN_VALUE=[DIRECT DEPOSIT BANK ACCOUNT NUMBER]\nFI");
	addvirtuallineADV(tmp,"DIRECT DEPOSIT BANK ACCOUNT NUMBER",1,17,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(112+1);
	sprintf(t,"IF([LAST BANK NAME]!=\"\") THEN RETURN_VALUE=[LAST BANK NAME] \nELSE RETURN_VALUE=[DIRECT DEPOSIT BANK NAME]\nFI");
	addvirtuallineADV(tmp,"DIRECT DEPOSIT BANK NAME",1,30,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(163+1);
	sprintf(t,"IF([LAST TRANSMITTAL ROUTING NUMBER]!=\"\") THEN RETURN_VALUE=[LAST TRANSMITTAL ROUTING NUMBER]  \nELSE RETURN_VALUE=[DIRECT DEPOSIT TRANSMITTAL ROUTE NUMBER] \nFI");
	addvirtuallineADV(tmp,"DIRECT DEPOSIT TRANSMITTAL ROUTE NUMBER",1,9,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(252+1);
	sprintf(t,"IF(!([PRIMARY FILE NUMBER]=1)) \nTHEN RETURN_VALUE=\"\" \nELSE IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"DIRECT DEPOSIT\")) \n     THEN RETURN_VALUE=[PAYROLL][PAYDEDM][TRANSMITTAL ROUTE NUMBER] \n \
    ELSE RETURN_VALUE=\"\" \n     FI \nFI");
	addvirtuallineADV(tmp,"TRANSMITTAL ROUTE NUMBER",1,9,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(271+1);
	sprintf(t,"IF(!([PRIMARY FILE NUMBER]=1)) \nTHEN RETURN_VALUE=\"\" \nELSE IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"DIRECT DEPOSIT\"))\n     THEN RETURN_VALUE=[VENPMT][VENMSR][VENDOR NAME] \n     ELSE RETURN_VALUE=\"\" \n     FI \nFI");
	addvirtuallineADV(tmp,"BANK NAME",1,30,t,0,0);
	if(t!=NULL) Rfree(t);
	
	t=Rmalloc(506+1);
	sprintf(t,"IF(([ORGANIZATION ADDRESS #1]=\"\") AND ([ORGANIZATION ADDRESS #2]=\"\") AND  ([ORGANIZATION CITY]=\"\") AND  ([ORGANIZATION STATE]=\"\") AND  ([ORGANIZATION ZIP]=\"\")) THEN RETURN_VALUE=\"\" ELSE IF(([ORGANIZATI\
ON ADDRESS #1]=\"\") AND ([ORGANIZATION ADDRESS #2]=\"\")) THEN RETURN_VALUE=([ORGANIZATION CITY]+\", \"+[ORGANIZATION STATE]+\"  \"+[ORGANIZATION ZIP]) ELSE IF([ORGANIZATION ADDRESS #1]=\"\") THEN RETURN_VALUE\
=[ORGANIZATION ADDRESS #2] ELSE RETURN_VALUE=[ORGANIZATION ADDRESS #1] FI FI FI");
	addvirtuallineADV(tmp,"ORGANIZATIONAL ADDRESS 1",0,0,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(447+1);
	sprintf(t,"IF(([ORGANIZATION ADDRESS #1]=\"\") AND ([ORGANIZATION ADDRESS #2]=\"\")) THEN RETURN_VALUE=\"\" ELSE IF(([ORGANIZATION ADDRESS #1]=\"\") AND ([ORGANIZATION CITY]=\"\") AND ([ORGANIZATION STATE]=\"\") AND ([ORGANI\
ZATION ZIP]=\"\")) THEN RETURN_VALUE=(\"\") ELSE IF([ORGANIZATION ADDRESS #2]=\"\") THEN RETURN_VALUE=([ORGANIZATION CITY]+\", \"+[ORGANIZATION STATE]+\"  \"+[ORGANIZATION ZIP]) ELSE RETURN_VALUE=[ORGANIZATION \
ADDRESS #2] FI FI FI");
	addvirtuallineADV(tmp,"ORGANIZATIONAL ADDRESS 2",0,0,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(288+1);
	sprintf(t,"IF(([ORGANIZATION ADDRESS #1]=\"\") OR ([ORGANIZATION ADDRESS #2]=\"\") OR (([ORGANIZATION CITY]=\"\") AND ([ORGANIZATION STATE]=\"\") AND ([ORGANIZATION ZIP]=\"\"))) THEN RETURN_VALUE=\"\" ELSE RETURN_VALUE=([ORGANIZATION CITY]+\", \"+[ORGANIZATION STATE]+\"  \"+[ORGANIZATION ZIP]) FI");
	addvirtuallineADV(tmp,"ORGANIZATIONAL ADDRESS 3",0,0,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(34+1);
	sprintf(t,"(\"PHONE: \"+[ORGANIZATION PHONE])");
	addvirtuallineADV(tmp,"ORG PHONE NUMBER",0,0,t,1,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(399+1);
	sprintf(t,"%s%s",
		"[PAYROLL][PAYJRCS][JANUARY GROSS]+[PAYROLL][PAYJRCS][FEBRUARY GROSS]+[PAYROLL][PAYJRCS][MARCH GROSS]+[PAYROLL][PAYJRCS][APRIL GROSS]+[PAYROLL][PAYJRCS][MAY GROSS]+[PAYROLL][PAYJRCS][JUNE GROSS]",
	"+[PAYROLL][PAYJRCS][JULY GROSS]+[PAYROLL][PAYJRCS][AUGUST GROSS]+[PAYROLL][PAYJRCS][SEPTEMBER GROSS]+[PAYROLL][PAYJRCS][OCTOBER GROSS]+[PAYROLL][PAYJRCS][NOVEMBER GROSS]+[PAYROLL][PAYJRCS][DECEMBER GROSS]");
	addvirtuallineADV(tmp,"ANNUAL CALENDAR PERSONNEL JOB RATE GROSS",2,20,t,0,0);

	if(t!=NULL) Rfree(t);
	t=Rmalloc(399+1);
	sprintf(t,"%s%s",
		"[PAYROLL][PAYJRCS][JANUARY UNITS]+[PAYROLL][PAYJRCS][FEBRUARY UNITS]+[PAYROLL][PAYJRCS][MARCH UNITS]+[PAYROLL][PAYJRCS][APRIL UNITS]+[PAYROLL][PAYJRCS][MAY UNITS]+[PAYROLL][PAYJRCS][JUNE UNITS]",
	"+[PAYROLL][PAYJRCS][JULY UNITS]+[PAYROLL][PAYJRCS][AUGUST UNITS]+[PAYROLL][PAYJRCS][SEPTEMBER UNITS]+[PAYROLL][PAYJRCS][OCTOBER UNITS]+[PAYROLL][PAYJRCS][NOVEMBER UNITS]+[PAYROLL][PAYJRCS][DECEMBER UNITS]");
	addvirtuallineADV(tmp,"ANNUAL CALENDAR PERSONNEL JOB RATE UNITS",20,12,t,0,0);

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

	addsortline(tmp,0,"[PAYROLL][PAYJPMS][DIRECT DEPOSIT NUMBER]");
	addsortline(tmp,0,"[LOCATION IDENTIFICATION]");
	addsortline(tmp,0,"[POSITION IDENTIFICATION]");
	addsortline(tmp,0,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION]");
	addsortline(tmp,0,"[WHICH TYPE]");
	addsortline(tmp,0,"[IDENTIFICATION]");
	addaccumline(tmp,"LAST BANK ACCOUNT",3,"[BANK ACCOUNT NUMBER]",0);
	addaccumline(tmp,"LAST LOCATION IDENTIFICATION",3,"[LOCATION IDENTIFICATION]",PLAINTEXT);
	addaccumline(tmp,"LAST PERSONNEL IDENTIFICATION",3,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION]",PLAINTEXT);
	addaccumline(tmp,"LAST SOCIAL SECURITY NUMBER",3,"[PRSNNL][PERDMG][SOCIAL SECURITY NUMBER]",PLAINTEXT);
	addaccumline(tmp,"LAST EMPLOYEE NAME",3,"[EMPLOYEE NAME]",PLAINTEXT);
	addaccumline(tmp,"LAST BANK NAME",3,"[BANK NAME]",0);
	addaccumline(tmp,"LAST DIRECT DEPOSIT BANK ACCOUNT NUMBER",3,"[DIRECT DEPOSIT BANK ACCOUNT NUMBER]",0);
	addaccumline(tmp,"LAST DIRECT DEPOSIT BANK NAME",3,"[DIRECT DEPOSIT BANK NAME]",0);
	addaccumline(tmp,"LAST DIRECT DEPOSIT DATE",3,"[PAYROLL][PAYJPMS][DIRECT DEPOSIT DATE]",DATES);
	addaccumline(tmp,"LAST DIRECT DEPOSIT NUMBER",3,"[PAYROLL][PAYJPMS][DIRECT DEPOSIT NUMBER]",LONGV);
	addaccumline(tmp,"LAST DIRECT DEPOSIT TRANSMITTAL ROUTE NUMBER",3,"[DIRECT DEPOSIT TRANSMITTAL ROUTE NUMBER]",0);
	addaccumline(tmp,"LAST TRANSMITTAL ROUTING NUMBER",3,"[TRANSMITTAL ROUTE NUMBER]",0);
	addaccumline(tmp,"TOTAL GROSS",7,"[GROSS]",0);
	if(PayRates!=NULL)
	{
		if(PayRates->Rates!=NULL)
		{
			for(z=0,Rate=PayRates->Rates;z<PayRates->num;++z,++Rate)
			{
				memset(temp,0,15000);
				memset(stemp,0,101);
				sprintf(temp,"[RATE IDENTIFICATION #%d]",(z+1));
				sprintf(stemp,"LAST RATE IDENTIFICATION #%d",z+1);
				addaccumline(tmp,stemp,3,temp,PLAINTEXT);

				memset(temp,0,15000);
				memset(stemp,0,101);
				sprintf(temp,"[RATE IDENTIFICATION GROSS #%d]",(z+1));
				sprintf(stemp,"TOTAL RATE IDENTIFICATION GROSS #%d",z+1);
				addaccumline(tmp,stemp,7,temp,SDECIMALV);
				memset(temp,0,15000);
				memset(stemp,0,101);
				sprintf(temp,"[RATE IDENTIFICATION GROSS YTD #%d]",(z+1));
				sprintf(stemp,"MAXIMUM RATE IDENTIFICATION GROSS YTD #%d",z+1);
				addaccumline(tmp,stemp,4,temp,SDECIMALV);
				memset(temp,0,15000);
				memset(stemp,0,101);
				sprintf(temp,"[RATE IDENTIFICATION UNITS #%d]",(z+1));
				sprintf(stemp,"TOTAL RATE IDENTIFICATION UNITS #%d",z+1);
				addaccumline(tmp,stemp,7,temp,SDECIMALV);
				memset(temp,0,15000);
				memset(stemp,0,101);
				sprintf(temp,"[RATE IDENTIFICATION UNITS YTD #%d]",(z+1));
				sprintf(stemp,"MAXIMUM RATE IDENTIFICATION UNITS YTD #%d",z+1);
				addaccumline(tmp,stemp,4,temp,SDECIMALV);

			}
		}
	}
	if(Deduction_Types->man_dedtypes!=NULL)
	{
		if((Deduction_Types->man_dedtypes->numlibs)>0)
		{
			for(z=0;z<(Deduction_Types->man_dedtypes->numlibs);++z)
			{
				memset(temp,0,15000);
				memset(stemp,0,101);
				sprintf(temp,"[MANDATORY DEDUCTION AMOUNT #%d]",(z+1));
				sprintf(stemp,"TOTAL MANDATORY DEDUCTION AMOUNT #%d",z+1);
				addaccumline(tmp,stemp,7,temp,SDECIMALV);
				memset(temp,0,15000);
				memset(stemp,0,101);
				sprintf(temp,"[MANDATORY DEDUCTION YTD #%d]",(z+1));
				sprintf(stemp,"MAXIMUM MANDATORY DEDUCTION YTD #%d",z+1);
				addaccumline(tmp,stemp,4,temp,SDECIMALV);

				memset(temp,0,15000);
				memset(stemp,0,101);
				sprintf(temp,"[MANDATORY DEDUCTION TYPE #%d]",(z+1));
				sprintf(stemp,"LAST MANDATORY DEDUCTION TYPE #%d",z+1);
				addaccumline(tmp,stemp,3,temp,PLAINTEXT);

			}
		}
	}
	if(Deduction_Types->vol_dedtypes!=NULL)
	{
		if((Deduction_Types->vol_dedtypes->numlibs)>0)
		{
			for(z=0;z<(Deduction_Types->vol_dedtypes->numlibs);++z)
			{
				memset(temp,0,15000);
				memset(stemp,0,101);
				sprintf(temp,"[VOLUNTARY DEDUCTION AMOUNT #%d]",(z+1));
				sprintf(stemp,"TOTAL VOLUNTARY DEDUCTION AMOUNT #%d",z+1);
				addaccumline(tmp,stemp,7,temp,SDECIMALV);
				memset(temp,0,15000);
				memset(stemp,0,101);
				sprintf(temp,"[VOLUNTARY DEDUCTION YTD #%d]",(z+1));
				sprintf(stemp,"MAXIMUM VOLUNTARY DEDUCTION YTD #%d",z+1);
				addaccumline(tmp,stemp,4,temp,SDECIMALV);
				memset(temp,0,15000);
				memset(stemp,0,101);
				sprintf(temp,"[VOLUNTARY DEDUCTION TYPE #%d]",(z+1));
				sprintf(stemp,"LAST VOLUNTARY DEDUCTION TYPE #%d",z+1);
				addaccumline(tmp,stemp,3,temp,PLAINTEXT);

			}
		}
	}
				
	addaccumline(tmp,"TOTAL ALL VOL DEDUCTIONS",7,"[VOLUNTARY DEDUCTION AMOUNTS]",0);
	addaccumline(tmp,"TOTAL REDUCE NET AMOUNT",7,"[REDUCE NET AMOUNT]",0);
	addaccumline(tmp,"TOTAL GROSS AMOUNT",7,"[GROSS AMOUNT]",0);
	addaccumline(tmp,"MAXIMUM ANNUAL CALENDAR PERSONNEL GROSS TOTAL TO DATE",4,"[ANNUAL CALENDAR PERSONNEL GROSS TOTAL TO DATE]",SDECIMALV);
	addaccumline(tmp,"TOTAL DIRECT DEPOSIT",7,"[DIRECT DEPOSIT]",0);
	addaccumline(tmp,"TOTAL DEDUCTION AMOUNT",7,"[DEDUCTION AMOUNT]",0);
	addaccumline(tmp,"TOTAL NET AMOUNT",7,"[NET AMOUNT]",0);
	addaccumline(tmp,"TOTAL FORMATTED NET AMOUNT",7,"[FORMATTED NET AMOUNT]",0);
	addaccumline(tmp,"LAST CHECK ADDRESS 1",3,"[PERSONNEL MAILING ADDRESS 1]",0);
	addaccumline(tmp,"LAST CHECK ADDRESS 2",3,"[PERSONNEL MAILING ADDRESS 2]",0);
	addaccumline(tmp,"LAST CHECK ADDRESS 3",3,"[PERSONNEL MAILING ADDRESS 3]",0);
	addaccumline(tmp,"LAST DESCRIPTION LONG #1",3,"[DESCRIPTION LONG #1]",0);
	addaccumline(tmp,"LAST DESCRIPTION LONG #2",3,"[DESCRIPTION LONG #2]",0);
	addaccumline(tmp,"LAST DESCRIPTION LONG #3",3,"[DESCRIPTION LONG #3]",0);
	addaccumline(tmp,"LAST DESCRIPTION LONG #4",3,"[DESCRIPTION LONG #4]",0);
	addaccumline(tmp,"LAST DESCRIPTION LONG #5",3,"[DESCRIPTION LONG #5]",0);
	addaccumline(tmp,"LAST DESCRIPTION LONG #6",3,"[DESCRIPTION LONG #6]",0);
	addaccumline(tmp,"LAST LEAVE BEGINNING BALANCE",3,"[LVEMGT][LVEMSTR][BEGINNING BALANCE]",0);
	addaccumline(tmp,"LAST LEAVE DESCRIPTION ID",3,"[LVEMGT][LVEMSTR][LEAVE DESCRIPTION IDENTIFICATION]",0);
	addaccumline(tmp,"LAST LEAVE PROCESSED EARNED",3,"[LVEMGT][LVEMSTR][PROCESSED EARNINGS]",0);
	addaccumline(tmp,"LAST LEAVE UNPROCESSED EARNED",3,"[LVEMGT][LVEMSTR][UNPROCESSED EARNINGS]",0);
	addaccumline(tmp,"LAST LEAVE PROCESSED USED",3,"[LVEMGT][LVEMSTR][PROCESSED USED]",0);
	addaccumline(tmp,"LAST LEAVE UNPROCESSED USED",3,"[LVEMGT][LVEMSTR][UNPROCESSED USED]",0);
	if(lvedsclist!=NULL)
	{
		for(z=0;z<lvedsclist->numlibs;++z)
		{
			memset(temp,0,15000);
			memset(stemp,0,101);
			sprintf(temp,"[LEAVE DESCRIPTION IDENTIFICATION #%d]",(z+1));
			sprintf(stemp,"LAST LEAVE DESCRIPTION IDENTIFICATION #%d",(z+1));
			addaccumline(tmp,stemp,3,temp,PLAINTEXT);

			memset(temp,0,15000);
			memset(stemp,0,101);
			sprintf(temp,"[LEAVE BEGINNING BALANCE #%d]",(z+1));
			sprintf(stemp,"LAST LEAVE BEGINNING BALANCE #%d",(z+1));
			addaccumline(tmp,stemp,3,temp,SDECIMALV);

			memset(temp,0,15000);
			memset(stemp,0,101);
			sprintf(temp,"[LEAVE PROCESSED EARNINGS #%d]",(z+1));
			sprintf(stemp,"LAST LEAVE PROCESSED EARNINGS #%d",(z+1));
			addaccumline(tmp,stemp,3,temp,SDECIMALV);

			memset(temp,0,15000);
			memset(stemp,0,101);
			sprintf(temp,"[LEAVE UNPROCESSED EARNINGS #%d]",(z+1));
			sprintf(stemp,"LAST LEAVE UNPROCESSED EARNINGS #%d",(z+1));
			addaccumline(tmp,stemp,3,temp,SDECIMALV);

			memset(temp,0,15000);
			memset(stemp,0,101);
			sprintf(temp,"[LEAVE PROCESSED USED #%d]",(z+1));
			sprintf(stemp,"LAST LEAVE PROCESSED USED #%d",(z+1));
			addaccumline(tmp,stemp,3,temp,SDECIMALV);

			memset(temp,0,15000);
			memset(stemp,0,101);
			sprintf(temp,"[LEAVE UNPROCESSED USED #%d]",(z+1));
			sprintf(stemp,"LAST LEAVE UNPROCESSED USED #%d",(z+1));
			addaccumline(tmp,stemp,3,temp,SDECIMALV);

			memset(temp,0,15000);
			memset(stemp,0,101);
			sprintf(temp,"[LEAVE AVAILABLE #%d]",(z+1));
			sprintf(stemp,"LAST LEAVE AVAILABLE #%d",(z+1));
			addaccumline(tmp,stemp,3,temp,SDECIMALV);

#ifdef XXXX
			memset(temp,0,15000);
			memset(stemp,0,101);
			sprintf(temp,"[LEAVE SLOT #%d]",(z+1));
			sprintf(stemp,"LAST LEAVE SLOT #%d",(z+1));
			addaccumline(tmp,stemp,3,temp,LONGV);

			memset(temp,0,15000);
			memset(stemp,0,101);
			sprintf(temp,"[LEAVE SLOT TYPE #%d]",(z+1));
			sprintf(stemp,"LAST LEAVE SLOT TYPE #%d",(z+1));
			addaccumline(tmp,stemp,3,temp,PLAINTEXT);

			memset(temp,0,15000);
			memset(stemp,0,101);
			sprintf(temp,"[LEAVE SLOT BEGINNING BALANCE #%d]",(z+1));
			sprintf(stemp,"LAST LEAVE SLOT BEGINNING BALANCE #%d",(z+1));
			addaccumline(tmp,stemp,3,temp,SDECIMALV);

			memset(temp,0,15000);
			memset(stemp,0,101);
			sprintf(temp,"[LEAVE SLOT PROCESSED EARNINGS #%d]",(z+1));
			sprintf(stemp,"LAST LEAVE SLOT PROCESSED EARNINGS #%d",(z+1));
			addaccumline(tmp,stemp,3,temp,SDECIMALV);

			memset(temp,0,15000);
			memset(stemp,0,101);
			sprintf(temp,"[LEAVE SLOT UNPROCESSED EARNINGS #%d]",(z+1));
			sprintf(stemp,"LAST LEAVE SLOT UNPROCESSED EARNINGS #%d",(z+1));
			addaccumline(tmp,stemp,3,temp,SDECIMALV);

			memset(temp,0,15000);
			memset(stemp,0,101);
			sprintf(temp,"[LEAVE SLOT PROCESSED USED #%d]",(z+1));
			sprintf(stemp,"LAST LEAVE SLOT PROCESSED USED #%d",(z+1));
			addaccumline(tmp,stemp,3,temp,SDECIMALV);

			memset(temp,0,15000);
			memset(stemp,0,101);
			sprintf(temp,"[LEAVE SLOT UNPROCESSED USED #%d]",(z+1));
			sprintf(stemp,"LAST LEAVE SLOT UNPROCESSED USED #%d",(z+1));
			addaccumline(tmp,stemp,3,temp,SDECIMALV);

			memset(temp,0,15000);
			memset(stemp,0,101);
			sprintf(temp,"[LEAVE SLOT AVAILABLE #%d]",(z+1));
			sprintf(stemp,"LAST LEAVE SLOT AVAILABLE #%d",(z+1));
			addaccumline(tmp,stemp,3,temp,SDECIMALV);
#endif /* XXXX */
		}
	}

	addcontrolline(tmp,"[PAYROLL][PAYJPMS][DIRECT DEPOSIT NUMBER]",0);
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


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ORGANIZATION NAME]");
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","            @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC@@",FALSE,NULL,3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ORGANIZATIONAL ADDRESS 1]");
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","            @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC@@",TRUE,"([ORGANIZATIONAL ADDRESS 1]!=\"\")",3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ORGANIZATIONAL ADDRESS 2]");
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","            @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC@@",TRUE,"([ORGANIZATIONAL ADDRESS 2]!=\"\")",3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ORGANIZATIONAL ADDRESS 3]");
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","            @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC@@",TRUE,"([ORGANIZATIONAL ADDRESS 3]!=\"\")",3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[ORG PHONE NUMBER]");
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","            @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC@@",FALSE,NULL,3,pval);
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);

	pval=Rmalloc(7*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST PERSONNEL IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST SOCIAL SECURITY NUMBER]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST LOCATION IDENTIFICATION]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST DIRECT DEPOSIT DATE]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST DIRECT DEPOSIT NUMBER]");
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","ID:@LLLLLLLLLLLLLL SSN:@LLLLLLLLLL Pay Loc:@LLLLLLLLL Date:@LLLLLLL DD#:@RRRR@@",FALSE,NULL,7,pval);
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,2,pval);
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER"," Rate of Pay         Units       Gross  Ytd Units  Ytd Gross@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);

	rate_count=PayRates->num;
	if((rate_count+tmp->numimages+14)>66) rate_count=66-tmp->numimages-14;
	for(z=0;z<rate_count;++z)
	{
		pval=Rmalloc(7*sizeof(RDApval));
		pvals=pval;
		pvals->symbol_type=0;
		pvals->type=0;
		memset(stemp,0,101);
		sprintf(stemp,"[RATE SLOT TYPE #%d]",z+1);
		pvals->name=stralloc(stemp);
		pvals->fieldname=NULL;
		++pvals;
		pvals->symbol_type=0;
		pvals->type=0;
		memset(stemp,0,101);
		sprintf(stemp,"[RATE SLOT UNITS #%d]",z+1);
		pvals->name=stralloc(stemp);
		pvals->fieldname=NULL;
		++pvals;
		pvals->symbol_type=0;
		pvals->type=0;
		memset(stemp,0,101);
		sprintf(stemp,"[RATE SLOT GROSS #%d]",z+1);
		pvals->name=stralloc(stemp);
		pvals->fieldname=NULL;
		++pvals;
		pvals->symbol_type=0;
		pvals->type=0;
		memset(stemp,0,101);
		sprintf(stemp,"[RATE SLOT UNITS YTD #%d]",z+1);
		pvals->name=stralloc(stemp);
		pvals->fieldname=NULL;
		++pvals;
		pvals->symbol_type=0;
		pvals->type=0;
		memset(stemp,0,101);
		sprintf(stemp,"[RATE SLOT GROSS YTD #%d]",z+1);
		pvals->name=stralloc(stemp);
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
		memset(temp,0,15000);
		sprintf(temp,"([RATE SLOT TYPE #%d]!=\"\")",(z+1));
		addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER"," @LLLLLLLLLLLLLL @RRRRRR.RR @RRRRRR.RR @RRRRRR.RR @RRRRRR.RR@@",TRUE,temp,7,pval);
		free_pvals(pval,7);
	}
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","Mandatory Deductions:   Mtd        Ytd                       Mtd         Ytd@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);

	db=(Deduction_Types->man_dedtypes->numlibs+1);
	man_count=(int)round(db/2);
	if((man_count+tmp->numimages+14)>66) man_count=66-tmp->numimages-14;
	z=0;
	for(x=0;x<man_count;++x)
	{
		if((z+1)<Deduction_Types->man_dedtypes->numlibs)
		{
			pval=Rmalloc(8*sizeof(RDApval));
			pvals=pval;
			pvals->symbol_type=0;
			pvals->type=0;
			memset(stemp,0,101);
			sprintf(stemp,"[MANDATORY SLOT TYPE #%d]",z+1);
			pvals->name=stralloc(stemp);
			pvals->fieldname=NULL;
			++pvals;
			pvals->symbol_type=0;
			pvals->type=0;
			memset(stemp,0,101);
			sprintf(stemp,"[MANDATORY SLOT AMOUNT #%d]",z+1);
			pvals->name=stralloc(stemp);
			pvals->fieldname=NULL;
			++pvals;
			pvals->symbol_type=0;
			pvals->type=0;
			memset(stemp,0,101);
			sprintf(stemp,"[MANDATORY SLOT YTD #%d]",z+1);
			pvals->name=stralloc(stemp);
			pvals->fieldname=NULL;
			++pvals;
			pvals->symbol_type=0;
			pvals->type=0;
			memset(stemp,0,101);
			sprintf(stemp,"[MANDATORY SLOT TYPE #%d]",z+2);
			pvals->name=stralloc(stemp);
			pvals->fieldname=NULL;
			++pvals;
			pvals->symbol_type=0;
			pvals->type=0;
			memset(stemp,0,101);
			sprintf(stemp,"[MANDATORY SLOT AMOUNT #%d]",z+2);
			pvals->name=stralloc(stemp);
			pvals->fieldname=NULL;
			++pvals;
			pvals->symbol_type=0;
			pvals->type=0;
			memset(stemp,0,101);
			sprintf(stemp,"[MANDATORY SLOT YTD #%d]",z+2);
			pvals->name=stralloc(stemp);
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
			memset(temp,0,15000);
			sprintf(temp,"([MANDATORY SLOT TYPE #%d]!=\"\")",(z+1));
			addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER"," @LLLLLLLLLLLLLL @RRRRRR.RR @RRRRRR.RR @LLLLLLLLLLLLLLL @RRRRR.RR @RRRRRR.RR@@",TRUE,temp,8,pval);
			free_pvals(pval,8);
			z+=2;
		} else if(z<Deduction_Types->man_dedtypes->numlibs)
		{
			pval=Rmalloc(5*sizeof(RDApval));
			pvals=pval;
			pvals->symbol_type=0;
			pvals->type=0;
			memset(stemp,0,101);
			sprintf(stemp,"[MANDATORY SLOT TYPE #%d]",z+1);
			pvals->name=stralloc(stemp);
			pvals->fieldname=NULL;
			++pvals;
			pvals->symbol_type=0;
			pvals->type=0;
			memset(stemp,0,101);
			sprintf(stemp,"[MANDATORY SLOT AMOUNT #%d]",z+1);
			pvals->name=stralloc(stemp);
			pvals->fieldname=NULL;
			++pvals;
			pvals->symbol_type=0;
			pvals->type=0;
			memset(stemp,0,101);
			sprintf(stemp,"[MANDATORY SLOT YTD #%d]",z+1);
			pvals->name=stralloc(stemp);
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
			memset(temp,0,15000);
			sprintf(temp,"([MANDATORY SLOT TYPE #%d]!=\"\")",(z+1));
			addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER"," @LLLLLLLLLLLLLL @RRRRRR.RR @RRRRRR.RR@@",TRUE,temp,5,pval);
			free_pvals(pval,5);
			++z;
		}	
	}
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","Voluntary Deductions:   Mtd        Ytd                        Mtd        Ytd@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);

	db=(Deduction_Types->vol_dedtypes->numlibs+1);
	vol_count=(int)round(db/2);
	if((vol_count+tmp->numimages+14)>66) vol_count=66-tmp->numimages-14;
	z=0;
	for(x=0;x<vol_count;++x)
	{
		if((z+1)<Deduction_Types->vol_dedtypes->numlibs)
		{
			pval=Rmalloc(8*sizeof(RDApval));
			pvals=pval;
			pvals->symbol_type=0;
			pvals->type=0;
			memset(stemp,0,101);
			sprintf(stemp,"[VOLUNTARY SLOT TYPE #%d]",z+1);
			pvals->name=stralloc(stemp);
			pvals->fieldname=NULL;
			++pvals;
			pvals->symbol_type=0;
			pvals->type=0;
			memset(stemp,0,101);
			sprintf(stemp,"[VOLUNTARY SLOT AMOUNT #%d]",z+1);
			pvals->name=stralloc(stemp);
			pvals->fieldname=NULL;
			++pvals;
			pvals->symbol_type=0;
			pvals->type=0;
			memset(stemp,0,101);
			sprintf(stemp,"[VOLUNTARY SLOT YTD #%d]",z+1);
			pvals->name=stralloc(stemp);
			pvals->fieldname=NULL;
			++pvals;
			pvals->symbol_type=0;
			pvals->type=0;
			memset(stemp,0,101);
			sprintf(stemp,"[VOLUNTARY SLOT TYPE #%d]",z+2);
			pvals->name=stralloc(stemp);
			pvals->fieldname=NULL;
			++pvals;
			pvals->symbol_type=0;
			pvals->type=0;
			memset(stemp,0,101);
			sprintf(stemp,"[VOLUNTARY SLOT AMOUNT #%d]",z+2);
			pvals->name=stralloc(stemp);
			pvals->fieldname=NULL;
			++pvals;
			pvals->symbol_type=0;
			pvals->type=0;
			memset(stemp,0,101);
			sprintf(stemp,"[VOLUNTARY SLOT YTD #%d]",z+2);
			pvals->name=stralloc(stemp);
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
			memset(temp,0,15000);
			sprintf(temp,"([VOLUNTARY SLOT TYPE #%d]!=\"\")",(z+1));
			addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER"," @LLLLLLLLLLLLLL @RRRRRR.RR @RRRRRR.RR @LLLLLLLLLLLLLLL @RRRRR.RR @RRRRRR.RR@@",TRUE,temp,8,pval);
			free_pvals(pval,8);
			z+=2;
		} else if(z<Deduction_Types->vol_dedtypes->numlibs)
		{
			pval=Rmalloc(5*sizeof(RDApval));
			pvals=pval;
			pvals->symbol_type=0;
			pvals->type=0;
			memset(stemp,0,101);
			sprintf(stemp,"[VOLUNTARY SLOT TYPE #%d]",z+1);
			pvals->name=stralloc(stemp);
			pvals->fieldname=NULL;
			++pvals;
			pvals->symbol_type=0;
			pvals->type=0;
			memset(stemp,0,101);
			sprintf(stemp,"[VOLUNTARY SLOT AMOUNT #%d]",z+1);
			pvals->name=stralloc(stemp);
			pvals->fieldname=NULL;
			++pvals;
			pvals->symbol_type=0;
			pvals->type=0;
			memset(stemp,0,101);
			sprintf(stemp,"[VOLUNTARY SLOT YTD #%d]",z+1);
			pvals->name=stralloc(stemp);
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
			memset(temp,0,15000);
			sprintf(temp,"([VOLUNTARY SLOT TYPE #%d]!=\"\")",(z+1));
			addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER"," @LLLLLLLLLLLLLL @RRRRRR.RR @RRRRRR.RR@@",TRUE,temp,5,pval);
			free_pvals(pval,5);
			++z;
		}	
	}
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
	memset(temp,0,15000);
	sprintf(temp,"%s%d%s%d%s%d%s%d%s%d%s%d%s",
		"((([LEAVE SLOT PROCESSED EARNINGS #",1,
		"]!=0) OR ([LEAVE SLOT BEGINNING BALANCE #",1,
		"]!=0) OR ([LEAVE SLOT PROCESSED USED #",1,
		"]!=0) OR ([LEAVE SLOT UNPROCESSED EARNINGS #",
		1,"]!=0) OR ([LEAVE SLOT UNPROCESSED USED #",
		1,"]!=0))) AND ([LEAVE SLOT TYPE #",1,"]!=\"\")");
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER"," Leave ID        Beg Bal  Ytd Earn  Ytd Used  Mtd Earn Mtd Used Ytd Total@@",FALSE,temp,2,pval);
	free_pvals(pval,2);

	if(lvedsclist!=NULL)
	{
		if(lvedsclist->numlibs>0)
		{
			lve_count=lvedsclist->numlibs;
			if((lve_count+tmp->numimages+14)>66) lve_count=66-tmp->numimages-14;
			for(z=0;z<lve_count;++z)
			{
				pval=Rmalloc(9*sizeof(RDApval));
				pvals=pval;
				pvals->symbol_type=0;
				pvals->type=0;
				memset(stemp,0,101);
				sprintf(stemp,"[LEAVE SLOT TYPE #%d]",(z+1));
				pvals->name=stralloc(stemp);

				pvals->fieldname=NULL;
				++pvals;

				pvals->symbol_type=0;
				pvals->type=0;
				memset(stemp,0,101);
				sprintf(stemp,"[LEAVE SLOT BEGINNING BALANCE #%d]",(z+1));
				pvals->name=stralloc(stemp);
				pvals->fieldname=NULL;
				++pvals;

				pvals->symbol_type=0;
				pvals->type=0;
				memset(stemp,0,101);
				sprintf(stemp,"[LEAVE SLOT PROCESSED EARNINGS #%d]",(z+1));
				pvals->name=stralloc(stemp);
				pvals->fieldname=NULL;
				++pvals;

				pvals->symbol_type=0;
				pvals->type=0;
				memset(stemp,0,101);
				sprintf(stemp,"[LEAVE SLOT PROCESSED USED #%d]",(z+1));
				pvals->name=stralloc(stemp);
				pvals->fieldname=NULL;
				++pvals;

				pvals->symbol_type=0;
				pvals->type=0;
				memset(stemp,0,101);
				sprintf(stemp,"[LEAVE SLOT UNPROCESSED EARNINGS #%d]",(z+1));
				pvals->name=stralloc(stemp);
				pvals->fieldname=NULL;
				++pvals;
				pvals->symbol_type=0;
				pvals->type=0;
				memset(stemp,0,101);
				sprintf(stemp,"[LEAVE SLOT UNPROCESSED USED #%d]",(z+1));
				pvals->name=stralloc(stemp);
				pvals->fieldname=NULL;
				++pvals;
				pvals->symbol_type=0;
				pvals->type=0;
				memset(stemp,0,101);
				sprintf(stemp,"[LEAVE SLOT AVAILABLE #%d]",(z+1));
				pvals->name=stralloc(stemp);
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
				memset(temp,0,15000);
				sprintf(temp,"%s%d%s%d%s%d%s%d%s%d%s%d%s",
					"((([LEAVE SLOT PROCESSED EARNINGS #",(z+1),
					"]!=0) OR ([LEAVE SLOT BEGINNING BALANCE #",(z+1),
					"]!=0) OR ([LEAVE SLOT PROCESSED USED #",(z+1),
					"]!=0) OR ([LEAVE SLOT UNPROCESSED EARNINGS #",(z+1),
					"]!=0) OR ([LEAVE SLOT UNPROCESSED USED #",
					(z+1),"]!=0))) AND ([LEAVE SLOT TYPE #",
					(z+1),"]!=\"\")");
				addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER"," @LLLLLLLLLLLLLL @RRRR.RR @RRRR.RR @RRRR.RR @RRRR.RR @RRRR.RR @RRRRR.RR@@",FALSE,temp,9,pval);
				free_pvals(pval,9);
			}
		}
	}

	pval=Rmalloc(5*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->fieldname=NULL;
	pvals->name=stralloc("[LAST DIRECT DEPOSIT BANK NAME]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->fieldname=NULL;
	pvals->name=stralloc("[LAST DIRECT DEPOSIT TRANSMITTAL ROUTE NUMBER]");
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->fieldname=NULL;
	pvals->name=stralloc("[LAST DIRECT DEPOSIT BANK ACCOUNT NUMBER");
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","Bank:@LLLLLLLLLLLLLLL @LLLLLLLLLLLLLLLLLLLLLLL @LLLLLLLLLLLLLLLLLLLLLLLLL@@",FALSE,NULL,5,pval);
	free_pvals(pval,5);


	pval=Rmalloc(6*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL YEAR TO DATE GROSS]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL GROSS AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[TOTAL REDUCE NET AMOUNT]");
	pvals->fieldname=NULL;
	++pvals;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[SUM NET AMOUNT]");
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
	++pvals;
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER"," Ytd Gross: @RRRRRRR.RR Gross:@RRRRRRR.RR - Ded's: @RRRRR.RR = Net:@RRRRR.RR@@",FALSE,NULL,6,pval);
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);

	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DESCRIPTION LONG #1]");
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","Comments:@VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV@@",TRUE,"([DESCRIPTION LONG #1]!=\"\")",3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DESCRIPTION LONG #2]");
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV@@",TRUE,"([DESCRIPTION LONG #2]!=\"\")",3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DESCRIPTION LONG #3]");
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV@@",TRUE,"([DESCRIPTION LONG #3]!=\"\")",3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DESCRIPTION LONG #4]");
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV@@",TRUE,"([DESCRIPTION LONG #4]!=\"\")",3,pval);
	free_pvals(pval,3);

	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DESCRIPTION LONG #5]");
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV@@",TRUE,"([DESCRIPTION LONG #5]!=\"\")",3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[DESCRIPTION LONG #6]");
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV@@",TRUE,"([DESCRIPTION LONG #6]!=\"\")",3,pval);
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,2,pval);
	free_pvals(pval,2);

/* Compute Remaining Space before accounting for non-used items */
	count=61-tmp->numimages;
/* Account for non-used items */
	for(z=0;z<6;++z)
	{
		memset(temp,0,15000);
		sprintf(temp,"([DESCRIPTION LONG #%d]=\"\")",(z+1));
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
		addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,temp,2,pval);
		free_pvals(pval,2);
	}
	for(z=0;z<rate_count;++z)
	{
		memset(temp,0,15000);
		sprintf(temp,"([RATE SLOT TYPE #%d]=\"\")",(z+1));
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
		addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,temp,2,pval);
		free_pvals(pval,2);
	}
	for(z=0;z<man_count;++z)
	{
		memset(temp,0,15000);
		sprintf(temp,"([MANDATORY SLOT TYPE #%d]=\"\")",(z+1));
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
		addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,temp,2,pval);
		free_pvals(pval,2);
	}
	for(z=0;z<vol_count;++z)
	{
		memset(temp,0,15000);
		sprintf(temp,"([VOLUNTARY SLOT TYPE #%d]=\"\")",(z+1));
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
		addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,temp,2,pval);
		free_pvals(pval,2);
	}
	for(z=0;z<lve_count;++z)
	{
		memset(temp,0,15000);
		sprintf(temp,"%s%d%s%d%s%d%s%d%s%d%s",
			"((([LEAVE SLOT PROCESSED EARNINGS #",(z+1),
			"]=0) AND ([LEAVE SLOT PROCESSED USED #",(z+1),
			"]=0) AND ([LEAVE SLOT UNPROCESSED EARNINGS #",
			(z+1),"]=0) AND ([LEAVE SLOT UNPROCESSED USED #",
			(z+1),"]=0))) OR ([LEAVE SLOT TYPE #",
			(z+1),"]=\"\")");
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
		addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,temp,2,pval);
		free_pvals(pval,2);
	}
	if(count>0)
	{
		for(z=0;z<count;++z)
		{
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
		}
	} else if(count<0)
	{
		prterr("Error in count of lines %d...... how did that happen?  Please check manually.....",count);
	}

	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST EMPLOYEE NAME]");
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","             @LLLLLLLLLLLLLLLLLLLLLLLLLLLLL@@",FALSE,NULL,3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST CHECK ADDRESS 1]");
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","             @LLLLLLLLLLLLLLLLLLLLLLLLLLLLL@@",FALSE,NULL,3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(3*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST CHECK ADDRESS 2]");
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
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","             @LLLLLLLLLLLLLLLLLLLLLLLLLLLLL@@",FALSE,NULL,3,pval);
	free_pvals(pval,3);


	pval=Rmalloc(4*sizeof(RDApval));
	pvals=pval;
	pvals->symbol_type=0;
	pvals->type=0;
	pvals->name=stralloc("[LAST CHECK ADDRESS 3]");
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
	++pvals;
	pvals->symbol_type=1;
	pvals->type=0;
	pvals->name=stralloc("FORMFEED");
	pvals->fieldname=NULL;
	addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","             @LLLLLLLLLLLLLLLLLLLLLLLLLLLLL@@@",FALSE,NULL,4,pval);
	free_pvals(pval,4);


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
