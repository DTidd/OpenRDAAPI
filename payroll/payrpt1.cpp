/* payrpt1.c - Payroll Make Reports */
#include <cstdio>
#include <rptgen.hpp>

#define DIAGNOSTICS
void PAYCHECK_RANGE_SCREEN(void);

#define NUM_VOL_SLOTS 8

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

extern char *module;
void pay.hppeck(void);
static void pay.hppeck_rpt(short,short,short,DEDTYPESTRUCT *);
DEDTYPESTRUCT *MakeDEDTYPElists(short,char *,char *,void (*)(short,DEDTYPESTRUCT *),short);
void buildDEDlists(short,DEDTYPESTRUCT *);
void FreePayRateStruct(PayRateStruct *);
void buildPayRateStruct(short,PayRateStruct *);
void SortPayRateStruct(PayRateStruct *);
PayRateStruct *MakePayRateStruct(short,char *,char *,void (*)(short,PayRateStruct *),short);

void buildDEDlists(short dedtypnum,DEDTYPESTRUCT *Deduction_Types)
{
	char deleteflag=FALSE,voluntary=FALSE,*string=NULL;

	FINDFLDGETCHAR(dedtypnum,"DELETEFLAG",&deleteflag);
	FINDFLDGETCHAR(dedtypnum,"VOLUNTARY",&voluntary);
	if(!deleteflag)
	{
		FINDFLDGETSTRING(dedtypnum,"DEDUCTION TYPE",&string);
		if(voluntary)
		{
			addAPPlib(Deduction_Types->vol_dedtypes,string);
		} else {
			addAPPlib(Deduction_Types->man_dedtypes,string);
		}
		addAPPlib(Deduction_Types->all_dedtypes,string);
		if(string!=NULL) Rfree(string);
	}
}

DEDTYPESTRUCT *MakeDEDTYPElists(short filenum,char *module,char *desc,void (*func)(short,DEDTYPESTRUCT *),short keyno)
{
	DEDTYPESTRUCT *Deduction_Types=NULL;
	short ef=0;
	RDArsrc *tmprsrc=NULL;
	char *filename=NULL;

	filename=FILENAME(filenum);
	Deduction_Types=Rmalloc(sizeof(DEDTYPESTRUCT));
	if(Deduction_Types==NULL) return(Deduction_Types);
	Deduction_Types->vol_dedtypes=NULL;
	Deduction_Types->vol_dedtypes=APPlibNEW();
	if(Deduction_Types->vol_dedtypes==NULL) return(Deduction_Types);
	Deduction_Types->man_dedtypes=NULL;
	Deduction_Types->man_dedtypes=APPlibNEW();
	if(Deduction_Types->man_dedtypes==NULL) return(Deduction_Types);
	Deduction_Types->all_dedtypes=NULL;
	Deduction_Types->all_dedtypes=APPlibNEW();
	if(Deduction_Types->all_dedtypes==NULL) return(Deduction_Types);
	tmprsrc=diagscrn(filenum,"DIAGNOSTIC SCREEN",module,desc,NULL);
	addDFincvir(tmprsrc,module,filename,NULL,filenum);
	GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
#ifdef DIAGNOSTICS
	if(tmprsrc!=NULL)
	{
		if(ADVmakescrn(tmprsrc,TRUE))
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED","The Make Screen function failed for the custom screen DIAGNOSTIC SCREEN .  Check to see the screen is available.  If it is, call your installer.\n",NULL,NULL,TRUE,NULL); 
			prterr("Error Cannot Create Diagnostic Screen.");
		} else {
			ForceWindowUpdate(tmprsrc);
		}
	}
#endif
	ef=ADVFRSNRDsec(filenum,keyno,SCRNvirtualSubData,tmprsrc);
	while(!ef)
	{
		func(filenum,Deduction_Types);
#ifdef DIAGNOSTICS
		update_diagnostic(tmprsrc,TRUE);
#endif
		ef=ADVNXTNRDsec(filenum,keyno,SCRNvirtualSubData,tmprsrc);
	}
	if(tmprsrc!=NULL)
	{
#ifdef DIAGNOSTICS
		killwindow(tmprsrc);
#endif
		free_rsrc(tmprsrc);
	}
	return(Deduction_Types);
}

void FreePayRateStruct(PayRateStruct *PayRates)
{
	RateStruct *Rate=NULL; 
	int x=0;

	if(PayRates!=NULL)
	{
		if(PayRates->Rates!=NULL)
		{
			for(x=0,Rate=PayRates->Rates;x<PayRates->num;++x,++Rate)
			{
				if(Rate->rateid!=NULL) Rfree(Rate->rateid);
			}
			Rfree(PayRates->Rates);
		}	
	}
}

void SortPayRateStruct(PayRateStruct *p)
{
	int x,y,comp_level;
	RateStruct *r1=NULL,*r2=NULL;
	char *tmp=NULL;

	for(x=0;x<p->num-1;++x) for(y=x+1;y<p->num;++y)
	{
		r1=p->Rates+x;
		r2=p->Rates+y;
		if(r2->seq<r1->seq)
		{
			comp_level=r1->seq;
			r1->seq=r2->seq;
			r2->seq=comp_level;
			tmp=r1->rateid;
			r1->rateid=r2->rateid;
			r2->rateid=tmp;
		}
	}
}

void SeePayRateStruct(PayRateStruct *PayRates)
{
	int x;
	RateStruct *Rate=NULL;

	if(PayRates!=NULL)
	{
	prterr("Showing Payroll Rates (Total Number of Rates=[%10d]):\n",PayRates->num);
		if(PayRates->Rates!=NULL)
		{
			for(x=0,Rate=PayRates->Rates;x<PayRates->num;++x,++Rate)
			{
				prterr("Rate Identification: [%-25s] Seq: [%10d].\n",Rate->rateid,Rate->seq);
			}
		}
	}
}

void buildPayRateStruct(short filenum,PayRateStruct *PayRates)
{
	char deleteflag=FALSE,*string=NULL;
	int seq=0;
	RateStruct *Rate=NULL;

	FINDFLDGETCHAR(filenum,"DELETEFLAG",&deleteflag);
	if(!deleteflag)
	{
		if(PayRates->Rates==NULL) 
		{
			PayRates->Rates=Rmalloc(sizeof(RateStruct));
			PayRates->num=0;
		} else {
			PayRates->Rates=Rrealloc(PayRates->Rates,(PayRates->num+1)*sizeof(RateStruct));
		}
		Rate=PayRates->Rates+PayRates->num;
		FINDFLDGETSTRING(filenum,"RATE IDENTIFICATION",&string);
		FINDFLDGETINT(filenum,"SELECT SEQUENCE",&seq);
		Rate->rateid=stralloc(string);
		Rate->seq=seq;
		if(string!=NULL) Rfree(string);
		++PayRates->num;
	}
}

PayRateStruct *MakePayRateStruct(short filenum,char *module,char *desc,void (*func)(short,PayRateStruct *),short keyno)
{
	PayRateStruct *PayRates=NULL;
	short ef=0;
	RDArsrc *tmprsrc=NULL;
	char *filename=NULL;

	filename=FILENAME(filenum);
	if(PayRates!=NULL) { FreePayRateStruct(PayRates); PayRates=NULL; }
	PayRates=Rmalloc(sizeof(PayRateStruct));
	PayRates->Rates=NULL;
	PayRates->num=0;
	tmprsrc=diagscrn(filenum,"DIAGNOSTIC SCREEN",module,desc,NULL);
	addDFincvir(tmprsrc,module,filename,NULL,filenum);
	GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
#ifdef DIAGNOSTICS
	if(tmprsrc!=NULL)
	{
		if(ADVmakescrn(tmprsrc,TRUE))
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED","The Make Screen function failed for the custom screen DIAGNOSTIC SCREEN .  Check to see the screen is available.  If it is, call your installer.\n",NULL,NULL,TRUE,NULL); 
			prterr("Error Cannot Create Diagnostic Screen.");
		} else {
			ForceWindowUpdate(tmprsrc);
		}
	}
#endif
	ef=ADVFRSNRDsec(filenum,keyno,SCRNvirtualSubData,tmprsrc);
	while(!ef)
	{
		func(filenum,PayRates);
#ifdef DIAGNOSTICS
		update_diagnostic(tmprsrc,TRUE);
#endif
		ef=ADVNXTNRDsec(filenum,keyno,SCRNvirtualSubData,tmprsrc);
	}
	if(tmprsrc!=NULL)
	{
#ifdef DIAGNOSTICS
		killwindow(tmprsrc);
#endif
		free_rsrc(tmprsrc);
	}
/*
	SeePayRateStruct(PayRates);
*/
	SortPayRateStruct(PayRates);
/*
	SeePayRateStruct(PayRates);
*/
	return(PayRates);
}

void pay.hppeck(void)
{
	short dedtypnum=(-1),posrate=(-1);
	DEDTYPESTRUCT *Deduction_Types=NULL;

	if((posrate=APPOPNNRD("POSTRK","POSRATE",TRUE,FALSE))==(-1))
	{
		ERRORDIALOG("CANNOT CREATE PAYROLL CHECKS REPORT!","Error in POSRATE data base file.  Please check that the Position Tracking Rate Master data file exist and is defined correctly and then retry.",NULL,FALSE);
		prterr("Error in POSTRK POSRATE data base file.");
	}
	if((dedtypnum=APPOPNNRD(module,"PAYDTYP",TRUE,FALSE))==(-1))
	{
		ERRORDIALOG("CANNOT CREATE PAYROLL CHECKS REPORT!","Error in PAYDTYP data base file.  Please check that the Payroll Deduction Type data file exist and is defined correctly and then retry.",NULL,FALSE);
		prterr("Error in PAYDTYP data base file.");
	} else {
		Deduction_Types=MakeDEDTYPElists(dedtypnum,module,"Creating Application's Deduction Type Lists",buildDEDlists,1);
		if(Deduction_Types==NULL)
		{
			ERRORDIALOG("CANNOT CREATE PAYROLL CHECKS REPORT!","Error in PAYDTYP data base file.  Can not Allocate Space for Deduction Types Structure.  Please check that their is enough per user memory available and that the Payroll Deduction Type data file exist and is defined correctly and then retry.",NULL,FALSE);
			prterr("Error in PAYDTYP data base file.  Can not Allocate Space for Deduction Types Structure.");  
		} else {
			if(Deduction_Types->vol_dedtypes==NULL || 
				Deduction_Types->man_dedtypes==NULL)
			{
				ERRORDIALOG("CANNOT CREATE PAYROLL CHECKS REPORT!","Error in PAYDTYP data base file.  Can not find any Voluntary or Manditory Deduction Types in the PAYDTYP data file.  Please check that the Payroll Deduction Type data file exist and is defined correctly and then retry.",NULL,FALSE);
				prterr("Error in PAYDTYP data base file.  Can not find any Voluntary or Manditory Deduction Types in the PAYDTYP data file.");
			} else if((Deduction_Types->vol_dedtypes->numlibs<1) &&
				(Deduction_Types->man_dedtypes->numlibs<1))
			{
				ERRORDIALOG("CANNOT CREATE PAYROLL CHECKS REPORT!","No Valid, Non-Deleted Deduction Types were found in the PAYDTYP data file.  Please check that the Payroll Deduction Type data file exist and is defined correctly and then retry.",NULL,FALSE);
				prterr("No Valid, Non-Deleted Deduction Types were found in the PAYDTYP data file.");
			} else {
				pay.hppeck_rpt(TRUE,posrate,dedtypnum,Deduction_Types);
				pay.hppeck_rpt(FALSE,posrate,dedtypnum,Deduction_Types);
			}
		}
	}
	if(dedtypnum!=(-1)) CLSNRD(dedtypnum);
	if(posrate!=(-1)) CLSNRD(posrate);
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
}
static void pay.hppeck_rpt(short bank,short posrate,short dedtypnum,DEDTYPESTRUCT *Deduction_Types)
{
	int x,z,nvs,imageoff=0,expoffset=0;
	RDAimage *image=NULL;
	RDAreport *tmp=NULL;
	RDAsearch *s=NULL;
	char *t=NULL,*expression=NULL;
	DFvirtual *d=NULL;
	char temp[15001];
	
	if(Deduction_Types!=NULL)
	{

/*======================================================================
Creating RDAreport Structure
RDAreport *xRDAreportNEW(char *name,char *title,char *desc,short engine,
	char *server_name,short process_exceptions,short use_print_screen,
	short stop_on_error,short ask_b4exe,short chain_report,
	char *chain_module,char *chain_name,short range_screen,
	short diag_level,short removesort,int line,
	char *file)

Diagnostic Level Values:
	0=Diagnostic Not-Breakable
	1=Diagnostic Breakable
	2=No Diagnostic
----------------------------------------------------------------------*/
	if(bank)
	{
		tmp=RDAreportNEW("PAYCHECK","Payroll Checks","This output definition is used to print the Actual Payroll Checks.",
			1,NULL,FALSE,TRUE,FALSE,FALSE,FALSE,NULL,NULL,TRUE,1,TRUE);
	} else {
		tmp=RDAreportNEW("PAYCKNB","Payroll Checks without BNKREC","This output definition is used to print the Actual Payroll Checks without the use of the Bank Reconciliation Module.",
			1,NULL,FALSE,TRUE,FALSE,FALSE,FALSE,NULL,NULL,TRUE,1,TRUE);
	}
/*======================================================================*/
/* Creating RDAdisplay Structure */
/*----------------------------------------------------------------------*/
	adddisplay(tmp,(double)8.5,(double)10.5,0,TRUE,1,NULL,0,1);
/*----------------------------------------------------------------------*/
	tmp->select=NULL;
/*======================================================================*/
/* Creating RDAsearch Structures */
/*----------------------------------------------------------------------*/
	addsearchfile(tmp,"PAYROLL","PAYJDPM",0,NULL,NULL,"RDA PAYJDPM KEY",FALSE,"([PAYROLL][PAYJDPM][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYJDPM][PAY IDENTIFICATION]=[SELECTED PAY IDENTIFICATION])",1);

	x=addsearchfile(tmp,"PRSNNL","PERDMG",2,"PAYROLL","PAYJDPM","PERDMG KEY",TRUE,"([PRSNNL][PERDMG][DELETEFLAG]=FALSE)",1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJDPM][PERSONNEL IDENTIFICATION]");
	}

	x=addsearchfile(tmp,"PAYROLL","PAYJPMS",2,"PAYROLL","PAYJDPM","PAYJPMS KEY",TRUE,"(([PAYROLL][PAYJPMS][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYJPMS][CHECK NUMBER]>0))",1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJDPM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYJDPM][JOB NUMBER]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJDPM][PAY IDENTIFICATION]");
	}

	x=addsearchfile(tmp,"PAYROLL","PAYPPMS",2,"PAYROLL","PAYJDPM","PAYPPMS KEY",FALSE,"(([PAYROLL][PAYPPMS][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYPPMS][BENEFITS COMPUTED]=TRUE))",1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJDPM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJDPM][PAY IDENTIFICATION]");
	}

	x=addsearchfile(tmp,"PAYROLL","PAYMSTR",1,"PAYROLL","PAYJDPM","PAYMSTR KEY",FALSE,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJDPM][PAY IDENTIFICATION]");
	}

	x=addsearchfile(tmp,"PAYROLL","PAYDCSM",1,"PAYROLL","PAYJDPM","PAYDCSM KEY",FALSE,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJDPM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","[PAYJDPM][DEDUCTION DESCRIPTION IDENTIFICATION]");
		addsearchconfld(s,"CALENDAR YEAR","[CALENDAR YEAR]");
	}

	x=addsearchfile(tmp,"PAYROLL","PAYDCSM",1,"PAYROLL","PAYJDPM","PAYDCSM KEY",FALSE,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJDPM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","[FICA DEDUCTION DESCRIPTION IDENTIFICATION]");
		addsearchconfld(s,"CALENDAR YEAR","[CALENDAR YEAR]");
	}

	x=addsearchfile(tmp,"PAYROLL","PAYJMST",2,"PAYROLL","PAYJDPM","JOBMSTR KEY",TRUE,"([PAYROLL][PAYJMST][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYJMST][ACTIVE]=TRUE)",1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJDPM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYJDPM][JOB NUMBER]");
	}

	x=addsearchfile(tmp,"POSTRK","POSMSTR",1,"PAYROLL","PAYJDPM","POSMSTR",TRUE,"([POSTRK][POSMSTR][DELETEFLAG]=FALSE)",1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"POSITION IDENTIFICATION","[POSITION IDENTIFICATION]");
	}

	x=addsearchfile(tmp,"POSTRK","POSLMST",1,"PAYROLL","PAYJDPM","POSLMST",FALSE,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"LOCATION IDENTIFICATION","[LOCATION IDENTIFICATION]");
	}

	x=addsearchfile(tmp,"PAYROLL","PAYDDMS",1,"PAYROLL","PAYJDPM","PAYDDMS KEY",FALSE,"([PAYROLL][PAYDDMS][DELETEFLAG]=FALSE)",1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEDUCTION DESCRIPTION IDENTIFICATION","[PAYJDPM][DEDUCTION DESCRIPTION IDENTIFICATION]");
	}

	x=addsearchfile(tmp,"PAYROLL","PAYDTYP",1,"PAYROLL","PAYJDPM","PAYDTYP KEY",FALSE,"([PAYROLL][PAYDTYP][DELETEFLAG]=FALSE)",1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"DEDUCTION TYPE","[DEDUCTION TYPE]");
	}

	addsearchfile(tmp,"PAYROLL","PAYJPRM",0,NULL,NULL,"PAYJPRM KEY",FALSE,"([PAYROLL][PAYJPRM][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYJPRM][PAY IDENTIFICATION]=[SELECTED PAY IDENTIFICATION])",1);

	x=addsearchfile(tmp,"PRSNNL","PERDMG",2,"PAYROLL","PAYJPRM","PERDMG KEY",TRUE,"([PRSNNL][PERDMG][DELETEFLAG]=FALSE)",1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJPRM][PERSONNEL IDENTIFICATION]");
	}

	x=addsearchfile(tmp,"PAYROLL","PAYJPMS",2,"PAYROLL","PAYJPRM","PAYJPMS KEY",TRUE,"(([PAYROLL][PAYJPMS][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYJPMS][CHECK NUMBER]>0))",1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJPRM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYJPRM][JOB NUMBER]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJPRM][PAY IDENTIFICATION]");
	}

	if(bank)
	{
		x=addsearchfile(tmp,"BNKREC","BNKACC",2,"PAYROLL","PAYJPRM","BNKACC KEY",FALSE,"(([BNKREC][BNKACC][DELETEFLAG]=FALSE) AND ([BNKREC][BNKACC][BANK IDENTIFICATION]=[SELECTED BANK IDENTIFICATION]))",1);
		if(x!=(-1))
		{
			s=tmp->search+(x-1);
			addsearchconfld(s,"BANK IDENTIFICATION","[BANK IDENTIFICATION]");
		}
	}

	x=addsearchfile(tmp,"PAYROLL","PAYPPMS",2,"PAYROLL","PAYJPRM","PAYPPMS KEY",FALSE,"([PAYROLL][PAYPPMS][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYPPMS][BENEFITS COMPUTED]=TRUE)",1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJPRM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJPRM][PAY IDENTIFICATION]");
	}

	x=addsearchfile(tmp,"PAYROLL","PAYMSTR",1,"PAYROLL","PAYJPRM","PAYMSTR KEY",FALSE,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJPRM][PAY IDENTIFICATION]");
	}

	x=addsearchfile(tmp,"PAYROLL","PAYPCSM",1,"PAYROLL","PAYJPRM","PAYPCSM KEY",FALSE,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJPRM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"CALENDAR YEAR","[CALENDAR YEAR]");
	}

	x=addsearchfile(tmp,"PAYROLL","PAYJMST",2,"PAYROLL","PAYJPRM","JOBMSTR KEY",TRUE,"([PAYROLL][PAYJMST][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYJMST][ACTIVE]=TRUE)",1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJPRM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYJPRM][JOB NUMBER]");
	}

	x=addsearchfile(tmp,"POSTRK","POSMSTR",1,"PAYROLL","PAYJPRM","POSMSTR",TRUE,"([POSTRK][POSMSTR][DELETEFLAG]=FALSE)",1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"POSITION IDENTIFICATION","[POSITION IDENTIFICATION]");
	}

	x=addsearchfile(tmp,"POSTRK","POSLMST",1,"PAYROLL","PAYJPRM","POSLMST",FALSE,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"LOCATION IDENTIFICATION","[LOCATION IDENTIFICATION]");
	}

	x=addsearchfile(tmp,"LVEMGT","LVEMSTR",1,"PAYROLL","PAYJPRM","LVEMSTR KEY",FALSE,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"LEAVE DESCRIPTION IDENTIFICATION","[VACATION LEAVE NAME]");
	}
	x=addsearchfile(tmp,"LVEMGT","LVEMSTR",1,"PAYROLL","PAYJPRM","LVEMSTR KEY",FALSE,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"LEAVE DESCRIPTION IDENTIFICATION","[SICK LEAVE NAME]");
	}

/*======================================================================*/
/* Creating RDAvirtual Structures */
/*----------------------------------------------------------------------*/

	addvirtuallineADV(tmp,"SELECTED PAY IDENTIFICATION",PLAINTEXT,15,NULL,0,2);

	if(bank)
	{
		addvirtuallineADV(tmp,"SELECTED BANK IDENTIFICATION",PLAINTEXT,15,NULL,0,2);
	}

	addvirtuallineADV(tmp,"MESSAGE #1",VARIABLETEXT,0,NULL,1,2);


	addvirtualline(tmp,"PERSONNEL IDENTIFICATION",PLAINTEXT,15,
		"([PRSNNL][PERDMG][PERSONNEL IDENTIFICATION])",0);

	addvirtualline(tmp,"CALENDAR YEAR",LONGV,4,
		"([PAYROLL][PAYMSTR][CALENDAR YEAR])",0);

	addvirtualline(tmp,"FICA DEDUCTION DESCRIPTION IDENTIFICATION",PLAINTEXT,4,"FICA",0);

	addvirtualline(tmp,"DEDUCTION TYPE",PLAINTEXT,15,
		"([PAYROLL][PAYDDMS][DEDUCTION TYPE])",0);

	addvirtualline(tmp,"POSITION IDENTIFICATION",PLAINTEXT,30,
		"([PAYROLL][PAYJMST][POSITION IDENTIFICATION])",0);

	memset(stemp,0,101);
	sprintf(stemp,"NAME FIRST MIDDLE LAST LINEAGE");
	d=getDFvirtual("PAYROLL",stemp);
	if(d!=NULL)
	{
		addvirtuallineADV(tmp,stemp,d->type,d->length,d->expression,d->when,1);
		FreeDFvirtual(d);
	} else {
		if(t!=NULL) Rfree(t);
		t=Rmalloc(8+RDAstrlen(stemp)+60+1);
		sprintf(t,"VIRTUAL FIELD ERROR: Report [PAYCHECK], Virtual Field Not Found [%s]",stemp);
		prterr(t);
		addvirtuallineADV(tmp,stemp,PLAINTEXT,40,"[PRSNNL][PERDMG][ALTERNATIVE NAME]",0,1);
/*
		if(errorlist!=NULL)
		{
			addERRlist(errorlist,t);
		}
*/
		if(t!=NULL) Rfree(t);
	}

	memset(stemp,0,101);
	sprintf(stemp,"NAME LAST LINEAGE FIRST MIDDLE");
	d=getDFvirtual("PAYROLL",stemp);
	if(d!=NULL)
	{
		addvirtuallineADV(tmp,stemp,d->type,d->length,d->expression,d->when,1);
		FreeDFvirtual(d);
	} else {
		if(t!=NULL) Rfree(t);
		t=Rmalloc(8+RDAstrlen(stemp)+60+1);
		sprintf(t,"VIRTUAL FIELD ERROR: Report [PAYCHECK], Virtual Field Not Found [%s]",stemp);
		prterr(t);
		addvirtuallineADV(tmp,stemp,PLAINTEXT,40,"[PRSNNL][PERDMG][NAME]",0,1);
/*
		if(errorlist!=NULL)
		{
			addERRlist(errorlist,t);
		}
*/
		if(t!=NULL) Rfree(t);
	}

	if(bank)
	{
		addvirtualline(tmp,"BANK IDENTIFICATION",PLAINTEXT,15,
			"[PAYROLL][PAYJPMS][BANK IDENTIFICATION]",0);
	}

	addvirtualline(tmp,"LOCATION IDENTIFICATION",PLAINTEXT,15,
		"([POSTRK][POSMSTR][LOCATION IDENTIFICATION])",0);

	memset(stemp,0,101);
	sprintf(stemp,"PERSONNEL MAILING ADDRESS 1");
	d=getDFvirtual(module,stemp);
	if(d!=NULL)
	{
		addvirtuallineADV(tmp,stemp,d->type,
			d->length,d->expression,
			d->when,0);
		FreeDFvirtual(d);
	}

	memset(stemp,0,101);
	sprintf(stemp,"PERSONNEL MAILING ADDRESS 2");
	d=getDFvirtual(module,stemp);
	if(d!=NULL)
	{
		addvirtuallineADV(tmp,stemp,d->type,
			d->length,d->expression,
			d->when,0);
		FreeDFvirtual(d);
	}

	memset(stemp,0,101);
	sprintf(stemp,"PERSONNEL MAILING ADDRESS 3");
	d=getDFvirtual(module,stemp);
	if(d!=NULL)
	{
		addvirtuallineADV(tmp,stemp,d->type,
			d->length,d->expression,
			d->when,0);
		FreeDFvirtual(d);
	}

	t=Rmalloc(350);
	sprintf(t,"%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]=1) \n",
		"THEN RETURN_VALUE=1 \n",
		"ELSE IF([PRIMARY FILE NUMBER]=2) \n",
		"     THEN RETURN_VALUE=0 \n",
		"     ELSE RETURN_VALUE=2 \n",
		"FI \n",
		"FI");
	addvirtualline(tmp,"WHICH TYPE",SHORTV,2,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(200);
	sprintf(t,"%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]=1) \n",
		"THEN RETURN_VALUE=[PAYROLL][PAYDDMS][DEDUCTION TYPE] \n",
		"ELSE RETURN_VALUE=[PAYROLL][PAYJMST][POSITION IDENTIFICATION]+\"-\"+[PAYROLL][PAYJPRM][RATE IDENTIFICATION] \n",
		"FI");
	addvirtualline(tmp,"IDENTIFICATION",PLAINTEXT,30,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(350);
	sprintf(t,"%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=2) \n",
		"THEN RETURN_VALUE=0 \n",
		"ELSE RETURN_VALUE=(([PAYROLL][PAYJPRM][GROSS])/100) \n",
		"FI");
	addvirtualline(tmp,"GROSS",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(650);
	sprintf(t,"%s%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=2) \n",
		"THEN RETURN_VALUE=0 \n",
		"ELSE IF(([PAYROLL][PAYJPRM][RATE IDENTIFICATION]=\"REGULAR\") OR\n",
		"     ([PAYROLL][PAYJPRM][RATE IDENTIFICATION] C \"UNIT\"))",
		"     THEN RETURN_VALUE=(([PAYROLL][PAYJPRM][GROSS])/100) \n",
		"     ELSE RETURN_VALUE=0 \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"REGULAR",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(650);
	sprintf(t,"%s%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=2) \n",
		"THEN RETURN_VALUE=0 \n",
		"ELSE IF(([PAYROLL][PAYJPRM][RATE IDENTIFICATION]=\"REGULAR\") OR\n",
		"     ([PAYROLL][PAYJPRM][RATE IDENTIFICATION] C \"UNIT\"))",
		"     THEN RETURN_VALUE=[PAYROLL][PAYJPRM][UNITS] \n",
		"     ELSE RETURN_VALUE=0 \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"REGULAR UNITS",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(450);
	sprintf(t,"%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=2) \n",
		"THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYJPRM][RATE IDENTIFICATION] C \"OVERTIME\") \n",
		"     THEN RETURN_VALUE=(([PAYROLL][PAYJPRM][GROSS])/100) \n",
		"     ELSE RETURN_VALUE=0 \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"OVERTIME",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(450);
	sprintf(t,"%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=2) \n",
		"THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYJPRM][RATE IDENTIFICATION] C \"OVERTIME\") \n",
		"     THEN RETURN_VALUE=[PAYROLL][PAYJPRM][UNITS] \n",
		"     ELSE RETURN_VALUE=0 \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"OVERTIME UNITS",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(350);
	sprintf(t,"%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=2) \n",
		"THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYJPRM][RATE IDENTIFICATION]=\"ADJUSTMENTS\") \n",
		"     THEN RETURN_VALUE=(([PAYROLL][PAYJPRM][GROSS])/100) \n",
		"     ELSE RETURN_VALUE=0 \n",
		"     FI \n",	
		"FI");
	addvirtualline(tmp,"ADJUSTMENTS",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(400);
	sprintf(t,"%s%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) \n",
		"THEN RETURN_VALUE=0 \n",
		"ELSE IF(([PAYROLL][PAYDDMS][REDUCE NET]=TRUE) \n",
		"       AND (!([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"DIRECT DEPOSIT\"))) \n",
		"     THEN RETURN_VALUE=(([PAYROLL][PAYJDPM][AMOUNT])/100) \n",
		"     ELSE RETURN_VALUE=0 \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"REDUCE NET AMOUNT",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	expoffset=0;
	if(Deduction_Types->vol_dedtypes!=NULL)
	{
	if((Deduction_Types->vol_dedtypes->numlibs)>0)
	{
	for(z=0;z<(Deduction_Types->vol_dedtypes->numlibs);++z)
	{
		if(strstr(Deduction_Types->vol_dedtypes->libs[z],"RETIREMENT"))
		{
			if(RDAstrcmp(Deduction_Types->vol_dedtypes->libs[z],"STATE RETIREMENT"))
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
			}
		}
		memset(stemp,0,101);
		sprintf(stemp,"VOLUNTARY DEDUCTION TYPE #%d",(z+1));
		t=Rmalloc(RDAstrlen(Deduction_Types->vol_dedtypes->libs[z])+3);
		sprintf(t,
			"\"%s\"",
			(Deduction_Types->vol_dedtypes->libs[z]==NULL?"":Deduction_Types->vol_dedtypes->libs[z]));
		addvirtualline(tmp,stemp,PLAINTEXT,15,t,0);
		if(t!=NULL) Rfree(t);

		t=Rmalloc(RDAstrlen(Deduction_Types->vol_dedtypes->libs[z])+425);
		sprintf(t,
"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \nELSE IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"%s\") AND ([PAYROLL][PAYDDMS][REDUCE NET])) \n     THEN RETURN_VALUE=(([PAYROLL][PAYJDPM][AMOUNT])/100) \n     ELSE RETURN_VALUE=0 FI \nFI",
			(Deduction_Types->vol_dedtypes->libs[z]==NULL?"":Deduction_Types->vol_dedtypes->libs[z]));
		memset(stemp,0,101);
		sprintf(stemp,"VOLUNTARY DEDUCTION AMOUNT #%d",(z+1));
		addvirtualline(tmp,stemp,SDECIMALV,15,t,0);
		if(t!=NULL) Rfree(t);
	}
	for(nvs=0;nvs<NUM_VOL_SLOTS;++nvs)
	{
		memset(temp,0,15000);
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
		memset(stemp,0,101);
		sprintf(stemp,"VOLUNTARY SLOT #%d",(nvs+1));
		addvirtualline(tmp,stemp,LONGV,4,t,1);
		if(t!=NULL) Rfree(t);
		

		memset(temp,0,15000);
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
		memset(stemp,0,101);
		sprintf(stemp,"VOLUNTARY SLOT TYPE #%d",(nvs+1));
		addvirtualline(tmp,stemp,PLAINTEXT,15,t,1);
		if(t!=NULL) Rfree(t);


		memset(temp,0,15000);
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
		memset(stemp,0,101);
		sprintf(stemp,"VOLUNTARY SLOT AMOUNT #%d",(nvs+1));
		addvirtualline(tmp,stemp,DECIMALV,15,t,1);
		if(t!=NULL) Rfree(t);
		
	}
	}
	}
	if(expression==NULL)
	{
		expression=Rmalloc(7);
		sprintf(expression,"(0.00)");
		expoffset=RDAstrlen(expression);
	} else { 
		expression=Rrealloc(expression,RDAstrlen(expression)+122);
		sprintf(&expression[expoffset],
			") \n     THEN RETURN_VALUE=(([PAYROLL][PAYJDPM][AMOUNT])/100) \n     ELSE RETURN_VALUE=0 \n     FI \nELSE RETURN_VALUE=0 \nFI");
		expoffset=(RDAstrlen(expression));
	}

	addvirtualline(tmp,"OTHER RETIREMENTS",SDECIMALV,15,expression,0);
	if(expression!=NULL) Rfree(expression);

	t=Rmalloc(350);
	sprintf(t,"%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"STATE RETIREMENT\") \n",
		"     THEN RETURN_VALUE=(([PAYROLL][PAYJDPM][AMOUNT])/100) \n",
		"     ELSE RETURN_VALUE=0 FI \n",
		"FI");
	addvirtualline(tmp,"STATE RETIREMENT",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	if(Deduction_Types->man_dedtypes!=NULL)
	{
	if(Deduction_Types->man_dedtypes->numlibs>0)
	{
	for(z=0;z<Deduction_Types->man_dedtypes->numlibs;++z)
	{
		memset(stemp,0,101);
		sprintf(stemp,"MANDITORY DEDUCTION TYPE #%d",(z+1));
		t=Rmalloc(RDAstrlen(Deduction_Types->man_dedtypes->libs[z])+3);
		sprintf(t,"\"%s\"",(Deduction_Types->man_dedtypes->libs[z]==NULL?"":Deduction_Types->man_dedtypes->libs[z]));
		addvirtualline(tmp,stemp,PLAINTEXT,15,t,0);
		if(t!=NULL) Rfree(t);

		t=Rmalloc(RDAstrlen(Deduction_Types->man_dedtypes->libs[z])+325);
		sprintf(t,
			"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \nELSE IF([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"%s\") \n     THEN RETURN_VALUE=(([PAYROLL][PAYJDPM][AMOUNT])/100) \n     ELSE RETURN_VALUE=0 FI \nFI"
			,(Deduction_Types->man_dedtypes->libs[z]==NULL?"":Deduction_Types->vol_dedtypes->libs[z]));
		memset(stemp,0,101);
		sprintf(stemp,"MANDITORY DEDUCTION AMOUNT #%d",(z+1));
		addvirtualline(tmp,stemp,SDECIMALV,15,t,0);
		if(t!=NULL) Rfree(t);
	}
	}
	}

	addvirtualline(tmp,"ZERO STRING",PLAINTEXT,15,"\"\"",0);

	addvirtualline(tmp,"ZERO AMOUNT",SDECIMALV,15,"(0.00)",0);

	t=Rmalloc(1100);
	sprintf(t,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYDDMS][REDUCE NET]=FALSE) \n",
		"     THEN RETURN_VALUE=0 \n",
		"     ELSE IF((!([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"FICA\")) \n",
		"            AND (!([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"MEDICARE\")) \n",
		"            AND (!([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"FEDERAL\")) \n",
		"            AND (!([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"STATE\")) \n",
		"            AND (!([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"COUNTY\")) \n",
		"            AND (!([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"CITY\")) \n",
		"            AND (!([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"DIRECT DEPOSIT\"))) \n",
		"          THEN RETURN_VALUE=([PAYROLL][PAYJDPM][AMOUNT]/100) \n",
		"          ELSE RETURN_VALUE=0 \n",
		"          FI \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"STANDARD VOLUNTARY DEDUCTIONS",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(600);
	sprintf(t,"%s%s%s%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF(([PAYROLL][PAYDDMS][REDUCE NET]=FALSE) \n",
		"     THEN RETURN_VALUE=0 \n",
		"     ELSE IF(([PAYROLL][PAYDTYP][VOLUNTARY]=TRUE) \n", 
		"            AND (!([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"DIRECT DEPOSIT\"))) \n",
		"          THEN RETURN_VALUE=([PAYROLL][PAYJDPM][AMOUNT]/100) \n",
		"          ELSE RETURN_VALUE=0 \n",
		"          FI \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"VOLUNTARY DEDUCTION AMOUNTS",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(300);
	sprintf(t,"%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF(([PAYROLL][PAYDDMS][REDUCE NET]=TRUE) \n",
		"       AND ([PAYROLL][PAYDTYP][VOLUNTARY]=FALSE)) \n",
		"     THEN RETURN_VALUE=([PAYROLL][PAYJDPM][AMOUNT]/100) \n",
		"     ELSE RETURN_VALUE=0 \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"MANDITORY DEDUCTION AMOUNTS",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(400);
	sprintf(t,"%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF(([PAYROLL][PAYDDMS][REDUCE NET]=TRUE) \n",
		"       AND ([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"FICA\")) \n",
		"     THEN RETURN_VALUE=(([PAYROLL][PAYJDPM][AMOUNT])/100) \n",
		"     ELSE RETURN_VALUE=0 \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"FICA AMOUNT",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(400);
	sprintf(t,"%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF(([PAYROLL][PAYDDMS][REDUCE NET]=TRUE) \n",
		"       AND ([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"MEDICARE\")) \n",
		"     THEN RETURN_VALUE=(([PAYROLL][PAYJDPM][AMOUNT])/100) \n",
		"     ELSE RETURN_VALUE=0 \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"MEDICARE AMOUNT",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(5100);
	sprintf(t,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]!=\"FICA\") OR ([PAYROLL][PAYDDMS][REDUCE NET]!=TRUE)) THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYDDMS][DEDUCTION DESCRIPTION IDENTIFICATION]!=[FICA DEDUCTION DESCRIPTION IDENTIFICATION] THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=0) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][2,1,20]) \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=1) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][2,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=2) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][2,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=3) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][2,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=4) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][2,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=5) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][2,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=6) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][2,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=7) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][2,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=8) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][2,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=9) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT][2,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=10) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][NOVEMBER AMOUNT][2,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=11) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][NOVEMBER AMOUNT][2,1,20]+[PAYROLL][PAYDCSM][DECEMBER AMOUNT][2,1,20])  \n",
		"FI FI FI FI FI FI FI FI FI FI FI FI FI FI");
	addvirtualline(tmp,"ANNUAL CALENDAR PERSONNEL FICA TOTAL TO DATE 2",SDECIMALV,20,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(5100);
	sprintf(t,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]!=\"FICA\") OR ([PAYROLL][PAYDDMS][REDUCE NET]!=TRUE)) THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYDDMS][DEDUCTION DESCRIPTION IDENTIFICATION]=[FICA DEDUCTION DESCRIPTION IDENTIFICATION]) THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=0) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]) \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=1) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=2) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=3) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=4) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=5) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=6) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=7) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=8) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=9) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=10) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][NOVEMBER AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=11) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][NOVEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][DECEMBER AMOUNT][1,1,20])  \n",
		"FI FI FI FI FI FI FI FI FI FI FI FI FI");
	addvirtualline(tmp,"ANNUAL CALENDAR PERSONNEL FICA TOTAL TO DATE",SDECIMALV,20,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(5000);
	sprintf(t,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]!=\"MEDICARE\") OR ([PAYROLL][PAYDDMS][REDUCE NET]!=TRUE)) THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=0) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]) \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=1) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=2) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=3) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=4) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=5) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=6) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=7) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=8) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=9) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=10) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][NOVEMBER AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=11) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][NOVEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][DECEMBER AMOUNT][1,1,20])  \n",
		"FI FI FI FI FI FI FI FI FI FI FI FI");
	addvirtualline(tmp,"ANNUAL CALENDAR PERSONNEL MEDICARE TOTAL TO DATE",SDECIMALV,20,t,0);
	if(t!=NULL) Rfree(t);


	t=Rmalloc(500);
	sprintf(t,"%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYJDPM][UPDATED]=FALSE) \n",
		"     THEN RETURN_VALUE=(([ANNUAL CALENDAR PERSONNEL FICA TOTAL TO DATE]/100)+([ANNUAL CALENDAR PERSONNEL FICA TOTAL TO DATE 2]/100)+[FICA AMOUNT]) \n",
		"     ELSE RETURN_VALUE=(([ANNUAL CALENDAR PERSONNEL FICA TOTAL TO DATE]/100)+([ANNUAL CALENDAR PERSONNEL FICA TOTAL TO DATE 2]/100)) \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"YEAR TO DATE FICA",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(350);
	sprintf(t,"%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYJDPM][UPDATED]=FALSE) \n",
		"     THEN RETURN_VALUE=((([ANNUAL CALENDAR PERSONNEL MEDICARE TOTAL TO DATE])/100)+[MEDICARE AMOUNT]) \n",
		"     ELSE RETURN_VALUE=(([ANNUAL CALENDAR PERSONNEL MEDICARE TOTAL TO DATE])/100) \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"YEAR TO DATE MEDICARE",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(100);
	sprintf(t,"[MAXIMUM YEAR TO DATE FICA]+[MAXIMUM YEAR TO DATE MEDICARE]");
	addvirtualline(tmp,"MAXIMUM YEAR TO DATE FICA/MEDICARE",SDECIMALV,15,t,1);
	if(t!=NULL) Rfree(t);


	t=Rmalloc(100);
	sprintf(t,"[TOTAL FICA AMOUNT]+[TOTAL MEDICARE AMOUNT]");
	addvirtualline(tmp,"TOTAL FICA/MEDICARE AMOUNT",SDECIMALV,15,t,1);
	if(t!=NULL) Rfree(t);


	t=Rmalloc(400);
	sprintf(t,"%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"FEDERAL\")  AND \n",
		"        ([PAYROLL][PAYDDMS][REDUCE NET]=TRUE)) \n",
		"     THEN RETURN_VALUE=(([PAYROLL][PAYJDPM][AMOUNT])/100) \n",
		"     ELSE RETURN_VALUE=0 \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"FEDERAL AMOUNT",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(5000);
	sprintf(t,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYDDMS][DEDUCTION TYPE]!=\"FEDERAL\") THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=0) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]) \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=1) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=2) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=3) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=4) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=5) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=6) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=7) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=8) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=9) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=10) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][NOVEMBER AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=11) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][NOVEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][DECEMBER AMOUNT][1,1,20])  \n",
		"FI FI FI FI FI FI FI FI FI FI FI FI");
	addvirtualline(tmp,"ANNUAL CALENDAR PERSONNEL FEDERAL TOTAL TO DATE",SDECIMALV,20,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(450);
	sprintf(t,"%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYJDPM][UPDATED]=FALSE) \n",
		"     THEN RETURN_VALUE=((([ANNUAL CALENDAR PERSONNEL FEDERAL TOTAL TO DATE])/100)+[FEDERAL]) \n",
		"     ELSE RETURN_VALUE=(([ANNUAL CALENDAR PERSONNEL FEDERAL TOTAL TO DATE])/100) \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"YEAR TO DATE FEDERAL",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(480);
	sprintf(t,"%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"STATE\") AND \n",
		"        ([PAYROLL][PAYDDMS][REDUCE NET]=TRUE)) \n",
		"     THEN RETURN_VALUE=(([PAYROLL][PAYJDPM][AMOUNT])/100) \n",
		"     ELSE RETURN_VALUE=0 \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"STATE AMOUNT",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(5000);
	sprintf(t,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYDDMS][DEDUCTION TYPE]!=\"STATE\") THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=0) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]) \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=1) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=2) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=3) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=4) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=5) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=6) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=7) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=8) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=9) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=10) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][NOVEMBER AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=11) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][NOVEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][DECEMBER AMOUNT][1,1,20])  \n",
		"FI FI FI FI FI FI FI FI FI FI FI FI");
	addvirtualline(tmp,"ANNUAL CALENDAR PERSONNEL STATE TOTAL TO DATE",SDECIMALV,20,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(400);
	sprintf(t,"%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYJDPM][UPDATED]=FALSE) \n",
		"     THEN RETURN_VALUE=((([ANNUAL CALENDAR PERSONNEL STATE TOTAL TO DATE])/100)+[STATE]) \n",
		"     ELSE RETURN_VALUE=(([ANNUAL CALENDAR PERSONNEL STATE TOTAL TO DATE])/100) \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"YEAR TO DATE STATE",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(450);
	sprintf(t,"%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"COUNTY\") AND \n",
		"        ([PAYROLL][PAYDDMS][REDUCE NET]=TRUE)) \n",
		"     THEN RETURN_VALUE=(([PAYROLL][PAYJDPM][AMOUNT])/100) \n",
		"     ELSE RETURN_VALUE=0 \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"COUNTY AMOUNT",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(5000);
	sprintf(t,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n"
		"ELSE IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]!=\"COUNTY\") OR \n", 
		"        ([PAYROLL][PAYDDMS][REDUCE NET]!=TRUE)) THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYDDMS][DEDUCTION TYPE]!=\"COUNTY\") THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=0) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]) \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=1) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=2) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=3) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=4) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=5) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=6) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=7) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=8) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=9) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=10) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][NOVEMBER AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=11) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][NOVEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][DECEMBER AMOUNT][1,1,20])  \n",
		"FI FI FI FI FI FI FI FI FI FI FI FI FI FI");
	addvirtualline(tmp,"ANNUAL CALENDAR PERSONNEL COUNTY TOTAL TO DATE",SDECIMALV,20,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(400);
	sprintf(t,"%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYJDPM][UPDATED]=FALSE) \n",
		"     THEN RETURN_VALUE=((([ANNUAL CALENDAR PERSONNEL COUNTY TOTAL TO DATE])/100)+[COUNTY]) \n",
		"     ELSE RETURN_VALUE=(([ANNUAL CALENDAR PERSONNEL COUNTY TOTAL TO DATE])/100) \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"YEAR TO DATE COUNTY",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(450);
	sprintf(t,"%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]!=\"CITY\") OR \n",
		"        ([PAYROLL][PAYDDMS][REDUCE NET]!=TRUE)) \n",
		"     THEN RETURN_VALUE=0 \n",
		"     ELSE RETURN_VALUE=(([PAYROLL][PAYJDPM][AMOUNT])/100) \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"CITY AMOUNT",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(5000);
	sprintf(t,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n", 
		"ELSE IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]!=\"CITY\") OR \n",
		"        ([PAYROLL][PAYDDMS][REDUCE NET]!=TRUE)) THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=0) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]) \n",
		"ELSE IF[PAYROLL][PAYMSTR][MONTH]=1) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=2) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=3) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=4) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=5) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=6) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=7) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=8) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=9) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=10) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][NOVEMBER AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=11) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][NOVEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][DECEMBER AMOUNT][1,1,20])  \n",
		"FI FI FI FI FI FI FI FI FI FI FI FI FI FI");
	addvirtualline(tmp,"ANNUAL CALENDAR PERSONNEL CITY TOTAL TO DATE",SDECIMALV,20,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(350);
	sprintf(t,"%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYJDPM][UPDATED]=FALSE) \n",
		"     THEN RETURN_VALUE=((([ANNUAL CALENDAR PERSONNEL CITY TOTAL TO DATE])/100)+[CITY]) \n",
		"     ELSE RETURN_VALUE=(([ANNUAL CALENDAR PERSONNEL CITY TOTAL TO DATE])/100) \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"YEAR TO DATE CITY",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	addvirtualline(tmp,"COUNTY/CITY",SDECIMALV,15,
		"([COUNTY]+[CITY])",0);

	t=Rmalloc(5000);
	sprintf(t,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]!=\"COUNTY\") OR \n",
		"        ([PAYROLL][PAYDDMS][DEDUCTION TYPE]!=\"CITY\") OR \n", 
		"        ([PAYROLL][PAYDDMS][REDUCE NET]!=TRUE)) THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=0) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]) \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=1) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=2) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=3) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=4) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=5) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=6) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=7) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=8) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=9) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=10) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][NOVEMBER AMOUNT][1,1,20])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=11) THEN RETURN_VALUE=([PAYROLL][PAYDCSM][JANUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][FEBRUARY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MARCH AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][APRIL AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][MAY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JUNE AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][JULY AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][AUGUST AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][SEPTEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][OCTOBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][NOVEMBER AMOUNT][1,1,20]+[PAYROLL][PAYDCSM][DECEMBER AMOUNT][1,1,20])  \n",
		"FI FI FI FI FI FI FI FI FI FI FI FI");
	addvirtualline(tmp,"ANNUAL CALENDAR PERSONNEL COUNTY/CITY TOTAL TO DATE",SDECIMALV,20,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(350);
	sprintf(t,"%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYJDPM][UPDATED]=FALSE) \n",
		"     THEN RETURN_VALUE=((([ANNUAL CALENDAR PERSONNEL COUNTY/CITY TOTAL TO DATE])/100)+[COUNTY/CITY]) \n",
		"     ELSE RETURN_VALUE=(([ANNUAL CALENDAR PERSONNEL COUNTY/CITY TOTAL TO DATE])/100) \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"YEAR TO DATE COUNTY/CITY",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(350);
	sprintf(t,"%s%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF(([PAYROLL][PAYDDMS][REDUCE NET]=TRUE) \n",
		"       AND ([PAYROLL][PAYDTYP][VOLUNTARY]=TRUE) \n", 
		"       AND (!([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"DIRECT DEPOSIT\"))) \n",
		"     THEN RETURN_VALUE=(([PAYROLL][PAYJDPM][AMOUNT])/100) \n",
		"     ELSE RETURN_VALUE=0 \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"DEDUCTION AMOUNT",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(200);
	sprintf(t,"%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=2) THEN RETURN_VALUE=0 \n",
		"ELSE RETURN_VALUE=(([PAYROLL][PAYJPRM][GROSS])/100) \n",
		"FI");
	addvirtualline(tmp,"GROSS AMOUNT",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(4000);
	sprintf(t,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=2) THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=0) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]) \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=1) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=2) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=3) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=4) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=5) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS]+[PAYROLL][PAYPCSM][JUNE GROSS])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=6) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS]+[PAYROLL][PAYPCSM][JUNE GROSS]+[PAYROLL][PAYPCSM][JULY GROSS])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=7) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS]+[PAYROLL][PAYPCSM][JUNE GROSS]+[PAYROLL][PAYPCSM][JULY GROSS]+[PAYROLL][PAYPCSM][AUGUST GROSS])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=8) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS]+[PAYROLL][PAYPCSM][JUNE GROSS]+[PAYROLL][PAYPCSM][JULY GROSS]+[PAYROLL][PAYPCSM][AUGUST GROSS]+[PAYROLL][PAYPCSM][SEPTEMBER GROSS])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=9) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS]+[PAYROLL][PAYPCSM][JUNE GROSS]+[PAYROLL][PAYPCSM][JULY GROSS]+[PAYROLL][PAYPCSM][AUGUST GROSS]+[PAYROLL][PAYPCSM][SEPTEMBER GROSS]+[PAYROLL][PAYPCSM][OCTOBER GROSS])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=10) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS]+[PAYROLL][PAYPCSM][JUNE GROSS]+[PAYROLL][PAYPCSM][JULY GROSS]+[PAYROLL][PAYPCSM][AUGUST GROSS]+[PAYROLL][PAYPCSM][SEPTEMBER GROSS]+[PAYROLL][PAYPCSM][OCTOBER GROSS]+[PAYROLL][PAYPCSM][NOVEMBER GROSS])  \n",
		"ELSE IF([PAYROLL][PAYMSTR][MONTH]=11) THEN RETURN_VALUE=([PAYROLL][PAYPCSM][JANUARY GROSS]+[PAYROLL][PAYPCSM][FEBRUARY GROSS]+[PAYROLL][PAYPCSM][MARCH GROSS]+[PAYROLL][PAYPCSM][APRIL GROSS]+[PAYROLL][PAYPCSM][MAY GROSS]+[PAYROLL][PAYPCSM][JUNE GROSS]+[PAYROLL][PAYPCSM][JULY GROSS]+[PAYROLL][PAYPCSM][AUGUST GROSS]+[PAYROLL][PAYPCSM][SEPTEMBER GROSS]+[PAYROLL][PAYPCSM][OCTOBER GROSS]+[PAYROLL][PAYPCSM][NOVEMBER GROSS]+[PAYROLL][PAYPCSM][DECEMBER GROSS])  \n",
		"FI FI FI FI FI FI FI FI FI FI FI FI");
	addvirtualline(tmp,"ANNUAL CALENDAR PERSONNEL GROSS TOTAL TO DATE",SDECIMALV,20,t,0);
	if(t!=NULL) Rfree(t);

        if(t!=NULL) Rfree(t);
        t=Rmalloc(90+1);
        sprintf(t,"([MAXIMUM ANNUAL CALENDAR PERSONNEL GROSS TOTAL TO DATE]/100+[TOTAL GROSS AMOUNT])");
        addvirtuallineADV(tmp,"MAXIMUM YEAR TO DATE GROSS",SDECIMALV,20,t,1,0);


	t=Rmalloc(350);
	sprintf(t,"%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"DIRECT DEPOSIT\")) \n",
		"     THEN RETURN_VALUE=(([PAYROLL][PAYJDPM][AMOUNT])/100) \n",
		"     ELSE RETURN_VALUE=0 \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"DIRECT DEPOSIT",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	addvirtualline(tmp,"TAXES",SDECIMALV,15,
		"([FEDERAL AMOUNT]+[FICA AMOUNT]+[MEDICARE AMOUNT]+[STATE AMOUNT]+[COUNTY AMOUNT]+[CITY AMOUNT])",0);

	t=Rmalloc(1000);
	sprintf(t,"%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=1) THEN RETURN_VALUE=0 \n",
		"ELSE IF([PAYROLL][PAYDDMS][REDUCE NET]=FALSE) THEN RETURN_VALUE=0 \n",
		"     ELSE IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]!=\"FICA\") AND \n",
		"             ([PAYROLL][PAYDDMS][DEDUCTION TYPE]!=\"MEDICARE\") AND \n",
		"             ([PAYROLL][PAYDDMS][DEDUCTION TYPE]!=\"FEDERAL\") AND \n",
		"             ([PAYROLL][PAYDDMS][DEDUCTION TYPE]!=\"STATE\") AND \n",
		"             ([PAYROLL][PAYDDMS][DEDUCTION TYPE]!=\"COUNTY\") AND \n",
		"             ([PAYROLL][PAYDDMS][DEDUCTION TYPE]!=\"CITY\")) \n",
		"          THEN RETURN_VALUE=([PAYROLL][PAYJDPM][AMOUNT]/100) \n",
		"          ELSE RETURN_VALUE=0 \n",
		"          FI \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"NON-TAX DEDUCTIONS",SDECIMALV,15,t,0);
	if(t!=NULL) Rfree(t);

	addvirtualline(tmp,"NET AMOUNT",SDECIMALV,15,
		"([GROSS AMOUNT]-[REDUCE NET AMOUNT])",1);

	addvirtualline(tmp,"FORMATTED NET AMOUNT",DOLLARS,15,
		"(([GROSS AMOUNT]-[REDUCE NET AMOUNT])*100)",0);

	addvirtualline(tmp,"SPELLED NET AMOUNT",VARIABLETEXT,0,
		"SPELL([TOTAL FORMATTED NET AMOUNT])",1);

	addvirtualline(tmp,"SICK LEAVE NAME",PLAINTEXT,15,"\"SICK LEAVE\"",0);

	addvirtualline(tmp,"VACATION LEAVE NAME",PLAINTEXT,15,"\"VACATION\"",0);


	t=Rmalloc(500);
	sprintf(t,"%s%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=2) THEN RETURN_VALUE=0 \n",
		"ELSE IF((!([LVEMGT][LVEMSTR][LEAVE DESCRIPTION IDENTIFICATION][1,1,15]=[VACATION LEAVE NAME])) \n",
		"OR ([LVEMGT][LVEMSTR][DELETEFLAG][1,1,1]=TRUE) \n",
		"OR ([LVEMGT][LVEMSTR][ACTIVE][1,1,1]=FALSE)) \n",
		"     THEN RETURN_VALUE=0 \n",
		"     ELSE RETURN_VALUE=([LVEMGT][LVEMSTR][UNPROCESSED EARNINGS][1,1,8]) \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"VACATION UNPROCESSED EARNINGS",SDECIMALV,8,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(500);
	sprintf(t,"%s%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=2) THEN RETURN_VALUE=0 \n",
		"ELSE IF((!([LVEMGT][LVEMSTR][LEAVE DESCRIPTION IDENTIFICATION][1,1,15]=[VACATION LEAVE NAME])) \n",
		"OR ([LVEMGT][LVEMSTR][DELETEFLAG][1,1,1]=TRUE) \n",
		"OR ([LVEMGT][LVEMSTR][ACTIVE][1,1,1]=FALSE)) \n",
		"     THEN RETURN_VALUE=0 \n",
		"     ELSE RETURN_VALUE=([LVEMGT][LVEMSTR][PROCESSED EARNINGS][1,1,8]) \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"VACATION PROCESSED EARNINGS",SDECIMALV,8,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(500);
	sprintf(t,"%s%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=2) THEN RETURN_VALUE=0 \n",
		"ELSE IF((!([LVEMGT][LVEMSTR][LEAVE DESCRIPTION IDENTIFICATION][1,1,15]=[VACATION LEAVE NAME])) \n",
		"OR ([LVEMGT][LVEMSTR][DELETEFLAG][1,1,1]=TRUE) \n",
		"OR ([LVEMGT][LVEMSTR][ACTIVE][1,1,1]=FALSE)) \n",
		"     THEN RETURN_VALUE=0 \n",
		"     ELSE RETURN_VALUE=([LVEMGT][LVEMSTR][UNPROCESSED USED][1,1,8) \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"VACATION UNPROCESSED USED",SDECIMALV,8,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(500);
	sprintf(t,"%s%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=2) THEN RETURN_VALUE=0 \n",
		"ELSE IF((!([LVEMGT][LVEMSTR][LEAVE DESCRIPTION IDENTIFICATION][1,1,15]=[VACATION LEAVE NAME])) \n",
		"OR ([LVEMGT][LVEMSTR][DELETEFLAG][1,1,1]=TRUE) \n",
		"OR ([LVEMGT][LVEMSTR][ACTIVE][1,1,1]=FALSE)) \n",
		"     THEN RETURN_VALUE=0 \n",
		"     ELSE RETURN_VALUE=([LVEMGT][LVEMSTR][PROCESSED USED][1,1,8) \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"VACATION PROCESSED USED",SDECIMALV,8,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(650);
	sprintf(t,"%s%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=2) THEN RETURN_VALUE=0 \n",
		"ELSE IF(!([LVEMGT][LVEMSTR][LEAVE DESCRIPTION IDENTIFICATION][1,1,15]=[VACATION LEAVE NAME])) \n",
		"     THEN RETURN_VALUE=0 \n",
		"     ELSE RETURN_VALUE=([VACATION BEGINNING BALANCE]+[VACATION PROCESSED EARNINGS] \n",
		"       +[VACATION UNPROCESSED EARNINGS]-[VACATION PROCESSED USED] \n",
		"       -[VACATION UNPROCESSED USED]) \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"VACATION BALANCE",SDECIMALV,8,t,0);
	if(t!=NULL) Rfree(t);


	t=Rmalloc(500);
	sprintf(t,"%s%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=2) THEN RETURN_VALUE=0 \n",
		"ELSE IF((!([LVEMGT][LVEMSTR][LEAVE DESCRIPTION IDENTIFICATION][2,1,15]=[SICK LEAVE NAME])) \n",
		"OR ([LVEMGT][LVEMSTR][DELETEFLAG][2,1,1]=TRUE) \n",
		"OR ([LVEMGT][LVEMSTR][ACTIVE][2,1,1]=FALSE)) \n",
		"     THEN RETURN_VALUE=0 \n",
		"     ELSE RETURN_VALUE=([LVEMGT][LVEMSTR][UNPROCESSED EARNINGS][2,1,8) \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"SICK LEAVE UNPROCESSED EARNINGS",SDECIMALV,8,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(400);
	sprintf(t,"%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=2) THEN RETURN_VALUE=0 \n",
		"ELSE IF(!([LVEMGT][LVEMSTR][LEAVE DESCRIPTION IDENTIFICATION][2,1,15]=[SICK LEAVE NAME])) \n",
		"     THEN RETURN_VALUE=0 \n",
		"     ELSE RETURN_VALUE=([LVEMGT][LVEMSTR][PROCESSED EARNINGS]) \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"SICK LEAVE PROCESSED EARNINGS",SDECIMALV,8,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(500);
	sprintf(t,"%s%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=2) THEN RETURN_VALUE=0 \n",
		"ELSE IF((!([LVEMGT][LVEMSTR][LEAVE DESCRIPTION IDENTIFICATION][2,1,15]=[SICK LEAVE NAME])) \n",
		"OR ([LVEMGT][LVEMSTR][DELETEFLAG][2,1,1]=TRUE) \n",
		"OR ([LVEMGT][LVEMSTR][ACTIVE][2,1,1]=FALSE)) \n",
		"     THEN RETURN_VALUE=0 \n",
		"     ELSE RETURN_VALUE=([LVEMGT][LVEMSTR][UNPROCESSED USED][2,1,8) \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"SICK LEAVE UNPROCESSED USED",SDECIMALV,8,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(350);
	sprintf(t,"%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=2) THEN RETURN_VALUE=0 \n",
		"ELSE IF(!([LVEMGT][LVEMSTR][LEAVE DESCRIPTION IDENTIFICATION][2,1,15]=[SICK LEAVE NAME])) \n",
		"     THEN RETURN_VALUE=0 \n",
		"     ELSE RETURN_VALUE=([LVEMGT][LVEMSTR][PROCESSED USED]) \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"SICK LEAVE PROCESSED USED",SDECIMALV,8,t,0);
	if(t!=NULL) Rfree(t);


	t=Rmalloc(350);
	sprintf(t,"%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=2) THEN RETURN_VALUE=0 \n",
		"ELSE IF(!([LVEMGT][LVEMSTR][LEAVE DESCRIPTION IDENTIFICATION][2,1,15]=[SICK LEAVE NAME])) \n",
		"     THEN RETURN_VALUE=0 \n",
		"     ELSE RETURN_VALUE=([SICK LEAVE PROCESSED EARNINGS]-[SICK LEAVE PROCESSED USED]) \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"SICK LEAVE BALANCE",SDECIMALV,8,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(700);
	sprintf(t,"%s%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]!=2) THEN RETURN_VALUE=0 \n",
		"ELSE IF(!([LVEMGT][LVEMSTR][LEAVE DESCRIPTION IDENTIFICATION][2,1,15]=[SICK LEAVE NAME])) \n",
		"     THEN RETURN_VALUE=0 \n",
		"     ELSE RETURN_VALUE=([SICK LEAVE BEGINNING BALANCE]+[SICK LEAVE PROCESSED EARNINGS] \n",
		"       +[SICK LEAVE UNPROCESSED EARNINGS]-[SICK LEAVE PROCESSED USED] \n",
		"       -[SICK LEAVE UNPROCESSED USED]) \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"SICK LEAVE BALANCE",SDECIMALV,8,t,0);
	if(t!=NULL) Rfree(t);


/*----------------------------------------------------------------------*/

	addsortline(tmp,0,"[PAYROLL][PAYJPMS][CHECK NUMBER]");
	addsortline(tmp,0,"[POSTRK][POSMSTR][LOCATION IDENTIFICATION]");
	addsortline(tmp,0,"[PAYROLL][PAYJMST][POSITION IDENTIFICATION]");
	addsortline(tmp,0,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION]");
	addcontrolline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION]",PLAINTEXT);
	addsortline(tmp,0,"[WHICH TYPE]");
	addsortline(tmp,0,"[IDENTIFICATION]");

/*--------------------------------------------------------------------*/

	addaccumline(tmp,"TOTAL GROSS",7,"[GROSS]",SDECIMALV);
	addaccumline(tmp,"TOTAL REGULAR",7,"[REGULAR]",SDECIMALV);
	addaccumline(tmp,"TOTAL REGULAR UNITS",7,"[REGULAR UNITS]",SDECIMALV);
	addaccumline(tmp,"TOTAL OVERTIME",7,"[OVERTIME]",SDECIMALV);
	addaccumline(tmp,"TOTAL OVERTIME UNITS",7,"[OVERTIME UNITS]",SDECIMALV);
	addaccumline(tmp,"TOTAL ADJUSTMENTS",7,"[ADJUSTMENTS]",SDECIMALV);
	addaccumline(tmp,"TOTAL REDUCE NET AMOUNT",7,"[REDUCE NET AMOUNT]",SDECIMALV);
	addaccumline(tmp,"TOTAL NON-TAX DEDUCTIONS",7,"[NON-TAX DEDUCTIONS]",SDECIMALV);
	addaccumline(tmp,"TOTAL TAXES",7,"[TAXES]",SDECIMALV);
	addaccumline(tmp,"TOTAL GROSS AMOUNT",7,"[GROSS AMOUNT]",SDECIMALV);

        addaccumline(tmp,"MAXIMUM ANNUAL CALENDAR PERSONNEL GROSS TOTAL TO DATE",4,"[ANNUAL CALENDAR PERSONNEL GROSS TOTAL TO DATE]",SDECIMALV);

	addaccumline(tmp,"TOTAL STANDARD VOLUNTARY DEDUCTIONS",7,"[STANDARD VOLUNTARY DEDUCTIONS]",SDECIMALV);
	addaccumline(tmp,"TOTAL FICA AMOUNT",7,"[FICA AMOUNT]",SDECIMALV);
	addaccumline(tmp,"TOTAL MEDICARE AMOUNT",7,"[MEDICARE AMOUNT]",SDECIMALV);
	addaccumline(tmp,"TOTAL FEDERAL AMOUNT",7,"[FEDERAL AMOUNT]",SDECIMALV);
	addaccumline(tmp,"TOTAL STATE AMOUNT",7,"[STATE AMOUNT]",SDECIMALV);
	addaccumline(tmp,"TOTAL COUNTY AMOUNT",7,"[COUNTY AMOUNT]",SDECIMALV);
	addaccumline(tmp,"TOTAL CITY AMOUNT",7,"[CITY AMOUNT]",SDECIMALV);
	addaccumline(tmp,"TOTAL COUNTY/CITY AMOUNT",7,"[COUNTY/CITY AMOUNT]",SDECIMALV);
	addaccumline(tmp,"MAXIMUM YEAR TO DATE FICA",4,"[YEAR TO DATE FICA]",SDECIMALV);
	addaccumline(tmp,"MAXIMUM YEAR TO DATE MEDICARE",4,"[YEAR TO DATE MEDICARE]",SDECIMALV);
	addaccumline(tmp,"MAXIMUM YEAR TO DATE FEDERAL",4,"[YEAR TO DATE FEDERAL]",SDECIMALV);
	addaccumline(tmp,"MAXIMUM YEAR TO DATE STATE",4,"[YEAR TO DATE STATE]",SDECIMALV);
	addaccumline(tmp,"MAXIMUM YEAR TO DATE COUNTY",4,"[YEAR TO DATE COUNTY]",SDECIMALV);
	addaccumline(tmp,"MAXIMUM YEAR TO DATE CITY",4,"[YEAR TO DATE CITY]",SDECIMALV);
	addaccumline(tmp,"MAXIMUM YEAR TO DATE COUNTY/CITY",4,"[YEAR TO DATE COUNTY/CITY]",SDECIMALV);

	addaccumline(tmp,"TOTAL DIRECT DEPOSIT",7,"[DIRECT DEPOSIT]",SDECIMALV);
	addaccumline(tmp,"TOTAL DEDUCTION AMOUNT",7,"[DEDUCTION AMOUNT]",SDECIMALV);
	addaccumline(tmp,"TOTAL NET AMOUNT",7,"[NET AMOUNT]",SDECIMALV);
	addaccumline(tmp,"TOTAL FORMATTED NET AMOUNT",7,"[FORMATTED NET AMOUNT]",SDECIMALV);
	addaccumline(tmp,"LAST PERSONNEL IDENTIFICATION",3,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION]",PLAINTEXT);
	addaccumline(tmp,"LAST EMPLOYEE NAME",3,"[NAME LAST LINEAGE FIRST MIDDLE]",PLAINTEXT);
	addaccumline(tmp,"LAST SOCIAL SECURITY NUMBER",3,"[PRSNNL][PERDMG][SOCIAL SECURITY NUMBER]",SOCSECNUM);
	addaccumline(tmp,"LAST LOCATION IDENTIFICATION",3,"[POSTRK][POSMSTR][LOCATION IDENTIFICATION]",PLAINTEXT);
	addaccumline(tmp,"LAST WHICH TYPE",3,"[WHICH TYPE]",SHORTV);
	addaccumline(tmp,"LAST IDENTIFICATION",3,"[IDENTIFICATION]",PLAINTEXT);
	addaccumline(tmp,"LAST CHECK NUMBER",3,"[PAYROLL][PAYJPMS][CHECK NUMBER]",LONGV);
	addaccumline(tmp,"LAST CHECK DATE",3,"[PAYROLL][PAYJPMS][CHECK DATE]",DATES);
	addaccumline(tmp,"LAST CHECK ADDRESS 1",3,"[PERSONNEL MAILING ADDRESS 1]",PLAINTEXT);
	addaccumline(tmp,"LAST CHECK ADDRESS 2",3,"[PERSONNEL MAILING ADDRESS 2]",PLAINTEXT);
	addaccumline(tmp,"LAST CHECK ADDRESS 3",3,"[PERSONNEL MAILING ADDRESS 3]",PLAINTEXT);
	addaccumline(tmp,"LAST VACATION UNPROCESSED EARNINGS",3,"[VACATION UNPROCESSED EARNINGS]",SDECIMALV);
	addaccumline(tmp,"LAST VACATION PROCESSED EARNINGS",3,"[VACATION PROCESSED EARNINGS]",SDECIMALV);
	addaccumline(tmp,"LAST VACATION UNPROCESSED USED",3,"[VACATION UNPROCESSED USED]",SDECIMALV);
	addaccumline(tmp,"LAST VACATION PROCESSED USED",3,"[VACATION PROCESSED USED]",SDECIMALV);
	addaccumline(tmp,"LAST VACATION BALANCE",3,"[VACATION BALANCE]",SDECIMALV);
	addaccumline(tmp,"LAST SICK LEAVE UNPROCESSED EARNINGS",3,"[SICK LEAVE UNPROCESSED EARNINGS]",SDECIMALV);
	addaccumline(tmp,"LAST SICK LEAVE PROCESSED EARNINGS",3,"[SICK LEAVE PROCESSED EARNINGS]",SDECIMALV);
	addaccumline(tmp,"LAST SICK LEAVE UNPROCESSED USED",3,"[SICK LEAVE UNPROCESSED USED]",SDECIMALV);
	addaccumline(tmp,"LAST SICK LEAVE PROCESSED USED",3,"[SICK LEAVE PROCESSED USED]",SDECIMALV);
	addaccumline(tmp,"LAST SICK LEAVE BALANCE",3,"[SICK LEAVE BALANCE]",SDECIMALV);

	if(Deduction_Types->vol_dedtypes!=NULL)
	{
	if(Deduction_Types->vol_dedtypes->numlibs>0)
	{
	for(z=0;z<Deduction_Types->vol_dedtypes->numlibs;++z)
	{
		memset(stemp,0,101);
		sprintf(stemp,"LAST VOLUNTARY DEDUCTION TYPE #%d",(z+1));
		t=Rmalloc(35+1);
		sprintf(t,"[VOLUNTARY DEDUCTION TYPE #%d]",(z+1));
		addaccumline(tmp,stemp,3,t,PLAINTEXT);
		if(t!=NULL) Rfree(t);

		memset(stemp,0,101);
		sprintf(stemp,"TOTAL VOLUNTARY DEDUCTION AMOUNT #%d",(z+1));
		t=Rmalloc(40+1);
		sprintf(t,"[VOLUNTARY DEDUCTION AMOUNT #%d]",(z+1));
		addaccumline(tmp,stemp,7,t,SDECIMALV);
		if(t!=NULL) Rfree(t);
	}
	}
	}

	if(Deduction_Types->man_dedtypes!=NULL)
	{
	if(Deduction_Types->man_dedtypes->numlibs>0)
	{
	for(z=0;z<Deduction_Types->man_dedtypes->numlibs;++z)
	{
		memset(stemp,0,101);
		sprintf(stemp,"LAST MANDITORY DEDUCTION TYPE #%d",(z+1));
		t=Rmalloc(35+1);
		sprintf(t,"[MANDITORY DEDUCTION TYPE #%d]",(z+1));
		addaccumline(tmp,stemp,3,t,PLAINTEXT);
		if(t!=NULL) Rfree(t);

		memset(stemp,0,101);
		sprintf(stemp,"TOTAL MANDITORY DEDUCTION AMOUNT #%d",(z+1));
		t=Rmalloc(40+1);
		sprintf(t,"[MANDITORY DEDUCTION AMOUNT #%d]",(z+1));
		addaccumline(tmp,stemp,7,t,SDECIMALV);
		if(t!=NULL) Rfree(t);
	}
	}
	}
	addaccumline(tmp,"LAST STATE RETIREMENT",3,"[STATE RETIREMENT]",SDECIMALV);
	addaccumline(tmp,"LAST OTHER RETIREMENTS",3,"[OTHER RETIREMENTS]",SDECIMALV);

/*--------------------------------------------------------------------*/
/* Making Headers */
/* Making Detail Lines*/
/* Making Footers*/

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","       XXX@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","                                                   @LLLLLLLLLLLLLLLLLLLLLLLLLLLL@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[MESSAGE #1]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);
/*
	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);
*/

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","  @LLLLLLLLLLL   @LLLLLLLLLLLLLLLLLLLLLLLLLL @LLLLLLLLLLLLLL             @LLLLLLL@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[LAST SOCIAL SECURITY NUMBER]",0,NULL);
	addimagepval(image,0,"[LAST EMPLOYEE NAME]",0,NULL);
	addimagepval(image,0,"[LAST LOCATION IDENTIFICATION]",0,NULL);
	addimagepval(image,0,"[LAST CHECK DATE]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","     @RRRR.RR @RRRR.RR @RRRR.RR @R.RR @RRRR.RR @.RR @.RR @.RR  @RRRRR.RR @RRRRR.RR@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[TOTAL REGULAR UNITS]",0,NULL);
	addimagepval(image,0,"[TOTAL REGULAR]",0,NULL);
	addimagepval(image,0,"[TOTAL GROSS]",0,NULL);
	addimagepval(image,0,"[TOTAL OVERTIME UNITS]",0,NULL);
	addimagepval(image,0,"[TOTAL OVERTIME]",0,NULL);
	addimagepval(image,0,"[LAST VACATION UNPROCESSED EARNINGS]",0,NULL);
	addimagepval(image,0,"[LAST VACATION UNPROCESSED USED]",0,NULL);
	addimagepval(image,0,"[LAST SICK LEAVE UNPROCESSED USED]",0,NULL);
	addimagepval(image,0,"[TOTAL ADJUSTMENTS]",0,NULL);
	addimagepval(image,0,"[TOTAL GROSS AMOUNT]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","  @RRRRRRR @RRRRRRR @RRRRRRR @RRRRRRR @RRRRRRR @RRRRRRR @RRRRRRR @RRRRRRR@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[VOLUNTARY SLOT TYPE #1]",0,NULL);
	addimagepval(image,0,"[VOLUNTARY SLOT TYPE #2]",0,NULL);
	addimagepval(image,0,"[VOLUNTARY SLOT TYPE #3]",0,NULL);
	addimagepval(image,0,"[VOLUNTARY SLOT TYPE #4]",0,NULL);
	addimagepval(image,0,"[VOLUNTARY SLOT TYPE #5]",0,NULL);
	addimagepval(image,0,"[VOLUNTARY SLOT TYPE #6]",0,NULL);
	addimagepval(image,0,"[VOLUNTARY SLOT TYPE #7]",0,NULL);
	addimagepval(image,0,"[VOLUNTARY SLOT TYPE #8]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","  @RRRR.RR @RRRR.RR @RRRR.RR @RRRR.RR @RRRR.RR @RRRR.RR @RRRR.RR @RRRR.RR @RRRR.RR@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[VOLUNTARY SLOT AMOUNT #1]",0,NULL);
	addimagepval(image,0,"[VOLUNTARY SLOT AMOUNT #2]",0,NULL);
	addimagepval(image,0,"[VOLUNTARY SLOT AMOUNT #3]",0,NULL);
	addimagepval(image,0,"[VOLUNTARY SLOT AMOUNT #4]",0,NULL);
	addimagepval(image,0,"[VOLUNTARY SLOT AMOUNT #5]",0,NULL);
	addimagepval(image,0,"[VOLUNTARY SLOT AMOUNT #6]",0,NULL);
	addimagepval(image,0,"[VOLUNTARY SLOT AMOUNT #7]",0,NULL);
	addimagepval(image,0,"[VOLUNTARY SLOT AMOUNT #8]",0,NULL);
	addimagepval(image,0,"[TOTAL DEDUCTION AMOUNT]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","                                          /MEDICARE@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","    @RRRR.RR @RRRR.RR             @RRRR.RR @RRR.RR @RRR.RR @RRR.RR @RRR.RR @RRR.RR@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[LAST STATE RETIREMENT]",0,NULL);
	addimagepval(image,0,"[LAST OTHER RETIREMENTS]",0,NULL);
	addimagepval(image,0,"[TOTAL FEDERAL AMOUNT]",0,NULL);
	addimagepval(image,0,"[TOTAL FICA/MEDICARE AMOUNT]",0,NULL);
	addimagepval(image,0,"[TOTAL STATE AMOUNT]",0,NULL);
	addimagepval(image,0,"[TOTAL COUNTY AMOUNT]",0,NULL);
	addimagepval(image,0,"[TOTAL CITY AMOUNT]",0,NULL);
	addimagepval(image,0,"[TOTAL TAXES]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","       @RRRR.RR @RRRR.RR @RRRR.RR  @RRRR.RR @RRR.RR @RRR.RR @RRR.RR       @RRRR.RR@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[MAXIMUM YEAR TO DATE FEDERAL]",0,NULL);
	addimagepval(image,0,"[MAXIMUM YEAR TO DATE FICA/MEDICARE]",0,NULL);
	addimagepval(image,0,"[MAXIMUM YEAR TO DATE STATE]",0,NULL);
	addimagepval(image,0,"[MAXIMUM YEAR TO DATE GROSS]",0,NULL);
	addimagepval(image,0,"[LAST VACATION BALANCE]",0,NULL);
	addimagepval(image,0,"[LAST SICK LEAVE BALANCE]",0,NULL);
	addimagepval(image,0,"[ZERO AMOUNT]",0,NULL);
	addimagepval(image,0,"[TOTAL NET AMOUNT]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","    @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[SPELLED NET AMOUNT]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","                                                @LLLLLLL @RRRRRRRR  @*,.RRRRRRRRRRR@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[LAST CHECK DATE]",0,NULL);
	addimagepval(image,0,"[LAST CHECK NUMBER]",0,NULL);
	addimagepval(image,0,"[TOTAL FORMATTED NET AMOUNT]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","                   @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[LAST EMPLOYEE NAME]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","                   @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[LAST CHECK ADDRESS 1]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","                   @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[LAST CHECK ADDRESS 2]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","                   @LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[LAST CHECK ADDRESS 3]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			ERRORDIALOG("Cannot Write Report","Error in permissions, user cannot write the report definition.  Check your permissions, and retry.  Call your installer.",NULL,FALSE);
			prterr("Error in permissions, user cannot write the report definition.  Check your permissions, and retry.");
		} else {
			PAYCHECK_RANGE_SCREEN();
		}
		FreeRDAreport(tmp);
	}
	} else {
		ERRORDIALOG("CANNOT CREATE PAYROLL CHECKS REPORT!","Error in PAYDTYP data base file.  Can not Allocate Space for Deduction Types Structure.  Please check that their is enough per user memory available and that the Payroll Deduction Type data file exist and is defined correctly and then retry.",NULL,FALSE);
		prterr("Error in PAYDTYP data base file.  Can not Allocate Space for Deduction Types Structure.");  
	}
}
/* PAYROLL's - Make (PAYCHECK RANGE SCREEN) Screen */
void PAYCHECK_RANGE_SCREEN()
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


	scrn=RDAscrnNEW("PAYROLL","PAYCHECK RANGE SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Pay Identification:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"SELECTED PAY IDENTIFICATION","Selected Pay Identification","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","Bank Identification:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"SELECTED BANK IDENTIFICATION","Selected Bank Identification","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [PERDMG][PERSONNEL IDENTIFICATION]","Range on Personnel Identification","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [PERDMG][PERSONNEL IDENTIFICATION]","From Personnel Identification","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [PERDMG][PERSONNEL IDENTIFICATION]","To Personnel Identification","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [PERDMG][LOCATION IDENTIFICATION]","Range on Location Identification","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [PERDMG][LOCATION IDENTIFICATION]","From Location Identification","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [PERDMG][LOCATION IDENTIFICATION]","To Location Identification","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,9,"RANGE ON [PAYJPMS][CHECK NUMBER]","Range on Check Numbers","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,5,"","From:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"FROM [PAYJPMS][CHECK NUMBER]","From Check Number","","",0,15,0,"","","","");
		ADVaddwdgt(scrn,5,"","To:","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,0,"TO [PAYJPMS][CHECK NUMBER]","To Check Number","","",0,15,0,"","","","");
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
