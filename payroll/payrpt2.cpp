/* payrpt2.c - Payroll Make Reports */
#include <cstdio>
#include <rptgen.hpp>
#include <mkez.hpp>

#define DIAGNOSTICS

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
void payreg(void);
static void payreg_rpt(short,DEDTYPESTRUCT *,short,PayRateStruct *,short,APPlib *);
extern void buildDEDlists(short,DEDTYPESTRUCT *);
extern DEDTYPESTRUCT *MakeDEDTYPElists(short,char *,char *,void (*)(short,DEDTYPESTRUCT *),short);
APPlib *MakeLVEDSClist(short,char *,char *,void (*)(short,APPlib *),short);
extern void FreePayRateStruct(PayRateStruct *);
extern void buildPayRateStruct(short,PayRateStruct *);
extern PayRateStruct *MakePayRateStruct(short,char *,char *,void (*)(short,PayRateStruct *),short);
void PAYREG_RANGE_SCREEN(void);
static void LST_PAYROLL_SELECTED_PAY_IDENTIFICATIONS(void);


char *FORMAT_STRING(char *string,char padchar,int length,int line,char *file)
{
	int x=0;
	char *ret_string=NULL;

	if(diagapps)
	{
		prterr("DIAG FORMAT_STRING Entering function at line [%d] in program [%s].",line,file);
	}
	ret_string=Rmalloc(length+1);
	if(RDAstrlen(string)<length)
	{
		for(x=0;x<RDAstrlen(string);++x)
		{
			ret_string[x]=string[x];
		}
		for(x=RDAstrlen(string);x<length;++x)
		{
			ret_string[x]=padchar;
		}
	} else {
		for(x=0;x<length;++x)
		{
			ret_string[x]=string[x];
		}
	}
	return(ret_string);
}
void buildLVElist(short lvedscnum,APPlib *lvedsclist)
{
	char deleteflag=FALSE,active=FALSE,*string=NULL;

	FINDFLDGETCHAR(lvedscnum,"DELETEFLAG",&deleteflag);
	FINDFLDGETCHAR(lvedscnum,"ACTIVE",&active);
	if(!deleteflag && active)
	{
		FINDFLDGETSTRING(lvedscnum,"LEAVE DESCRIPTION IDENTIFICATION",&string);
		addAPPlib(lvedsclist,string);
		if(string!=NULL) Rfree(string);
	}
}

APPlib *MakeLVEDSClist(short filenum,char *module,char *desc,void (*func)(short,APPlib *),short keyno)
{
	APPlib *list=NULL;
	short ef=0;
	RDArsrc *tmprsrc=NULL;
	char *filename=NULL;

	filename=FILENAME(filenum);
	
	tmprsrc=diagscrn(filenum,"DIAGNOSTIC SCREEN",module,"Creating Leave Descripition List",NULL);
	addDFincvir(tmprsrc,module,filename,NULL,filenum);
	GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
#ifdef DIAGNOSTICS
	if(tmprsrc!=NULL)
	{
		if(ADVmakescrn(tmprsrc,TRUE))
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN",
				"MAKESCRN FAILED",
				"The Make Screen function failed for the custom screen DIAGNOSTIC SCREEN .  Check to see the screen is available.  If it is, call your installer.\n",
				NULL,NULL,TRUE,NULL); 
			prterr("Error Cannot Create Diagnostic Screen.");
		} else {
			ForceWindowUpdate(tmprsrc);
		}
	}
#endif
	if(list!=NULL) freeapplib(list);
	list=APPlibNEW();
	ef=ADVFRSNRDsec(filenum,keyno,SCRNvirtualSubData,tmprsrc);
	while(!ef)
	{
		func(filenum,list);
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
	if(list->numlibs<1) freeapplib(list);
	return(list);
}

void payreg(void)
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
				ERRORDIALOG("CANNOT CREATE PAYROLL CHECKS REPORT!","Error in PAYDTYP data base file.  Can not find any Voluntary or Manditory Deduction Types in the PAYDTYP data file.  Please check that the Payroll Deduction Type data file exist and is defined correctly and then retry.",NULL,FALSE);
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
						"POSITION TRACKING MODULE NOT PRESENT!","Error in POSRATE data base file.  Please check that the Rate Identification data file exist if you want to have the Earning Rates to appear in the PAYROLL REGISTER REPORT.",
						NULL,NULL,TRUE,NULL);
					prterr("Error in POSRATE data base file.");
				} else {
					PayRates=MakePayRateStruct(posratenum,module,"Creating Application's Pay Rate Structure",buildPayRateStruct,1);
					if(PayRates==NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN",
						"POSITION TRACKING MODULE NOT PRESENT!",
						"Error in POSRATE data base file.  Can not Allocate Space for Application's Pay Rate Structure or POSRATE Module Not Present.  Please check that if this module is to be present their is enough per user memory available and that the POSRATE data file exist and is defined correctly and then retry.",
							NULL,NULL,TRUE,NULL);
						prterr("Error in POSRATE data base file.  Can not Allocate Space for Application's Rate Id Structure or POSTRK Module is not setup.");  
					} else {
						lvedscnum=APPOPNNRD("LVEMGT","LVEDSC",TRUE,FALSE);
						if(lvedscnum==(-1))
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN",
								"LEAVE MANAGEMENT MODULE NOT PRESENT!","Error in LVEDSC data base file.  Please check that the Leave Description data file exist if you want to have the Leave Descriptions to appear in the PAYROLL REGISTER REPORT.",
								NULL,NULL,TRUE,NULL);
							prterr("Error in LVEDSC data base file.");
						} else {
							lvedsclist=MakeLVEDSClist(lvedscnum,module,"Creating Application's Leave Description Lists",buildLVElist,1);
							if(lvedsclist==NULL)
							{
								WARNINGDIALOG("WARNING DIALOG SCREEN",
								"LEAVE MANAGEMENT MODULE NOT PRESENT!",
								"Error in LVEDSC data base file.  Can not Allocate Space for Application's Leave Description List or LVEMGT Module Not Present.  Please check that if this module is to be present their is enough per user memory available and that the Leave Description data file exist and is defined correctly and then retry.",
									NULL,NULL,TRUE,NULL);
								prterr("Error in LVEDSC data base file.  Can not Allocate Space for Application's Leave Description List or LVEMGT Module is not setup.");  
							} else {
								payreg_rpt(dedtypnum,Deduction_Types,posratenum,PayRates,lvedscnum,lvedsclist);
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
static void payreg_rpt(short dedtypnum,DEDTYPESTRUCT *Deduction_Types,
		short posratenum,PayRateStruct *PayRates,
		short lvedscnum,APPlib *lvedsclist)
{
	RateStruct *Rate=NULL;
	int x,z,imageoff=0;
	RDAimage *image=NULL;
	RDAreport *tmp=NULL;
	RDAsearch *s=NULL;
	char *t=NULL,*expression=NULL;
	char temp[15000],temp1[15000];
	

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
	tmp=RDAreportNEW("PAYREG","Payroll Register","This output definition is used to print the Payroll Register.",
			1,NULL,FALSE,TRUE,FALSE,FALSE,FALSE,NULL,NULL,TRUE,1,TRUE);
/*======================================================================*/
/* Creating RDAdisplay Structure */
/*----------------------------------------------------------------------*/
	adddisplay(tmp,(double)8.5,(double)10.0,0,FALSE,0,NULL,0,1);
/*----------------------------------------------------------------------*/
	tmp->select=NULL;
/*======================================================================*/
/* Creating RDAsearch Structures */
/*----------------------------------------------------------------------*/


	t=Rmalloc(157+1);
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

/*
	addsearchfile(tmp,"PAYROLL","PAYJDPM",0,NULL,NULL,"RDA PAYJDPM KEY",FALSE,"([PAYROLL][PAYJDPM][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYJDPM][PAY IDENTIFICATION]=[SELECTED PAY IDENTIFICATION])",1);
*/

	x=addsearchfile(tmp,"PRSNNL","PERDMG",2,"PAYROLL","PAYJDPM","PERDMG KEY",TRUE,"([PRSNNL][PERDMG][DELETEFLAG]=FALSE)",1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJDPM][PERSONNEL IDENTIFICATION]");
	}
	x=addsearchfile(tmp,"PAYROLL","PAYMSTR",2,"PAYROLL","PAYJDPM","PAYMSTR KEY",TRUE,"([PAYROLL][PAYMSTR][DELETEFLAG]=FALSE)",1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJDPM][PAY IDENTIFICATION]");
	}

	x=addsearchfile(tmp,"PAYROLL","PAYJPMS",2,"PAYROLL","PAYJDPM","PAYJPMS KEY",FALSE,NULL,1);
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

	x=addsearchfile(tmp,"PAYROLL","PAYDDMS",2,"PAYROLL","PAYJDPM","PAYDDMS KEY",FALSE,"([PAYROLL][PAYDDMS][DELETEFLAG]=FALSE)",1);
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

	t=Rmalloc(157+1);
	sprintf(t,"([PAYROLL][PAYJPRM][DELETEFLAG]=FALSE) AND \n([PAYROLL][PAYJPRM][PAY IDENTIFICATION]=[SELECTED PAY IDENTIFICATION])");
	x=addsearchfileADV(tmp,"PAYROLL","PAYJPRM",0,NULL,NULL,"RDA PAYJPRM KEY",TRUE,t,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PAY IDENTIFICATION","[SELECTED PAY IDENTIFICATION]");
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","");
		addsearchconfld(s,"JOB NUMBER","");
		addsearchconfld(s,"RATE IDENTIFICATION","");
	}
	if(t!=NULL) Rfree(t);
/*
	addsearchfile(tmp,"PAYROLL","PAYJPRM",0,NULL,NULL,"PAYJPRM KEY",FALSE,"([PAYROLL][PAYJPRM][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYJPRM][PAY IDENTIFICATION]=[SELECTED PAY IDENTIFICATION])",1);
*/

	x=addsearchfile(tmp,"PRSNNL","PERDMG",2,"PAYROLL","PAYJPRM","PERDMG KEY",TRUE,"([PRSNNL][PERDMG][DELETEFLAG]=FALSE)",1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJPRM][PERSONNEL IDENTIFICATION]");
	}
	x=addsearchfile(tmp,"PAYROLL","PAYMSTR",2,"PAYROLL","PAYJDPM","PAYMSTR KEY",TRUE,"([PAYROLL][PAYMSTR][DELETEFLAG]=FALSE)",1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJDPM][PAY IDENTIFICATION]");
	}

	x=addsearchfile(tmp,"PAYROLL","PAYJPMS",2,"PAYROLL","PAYJPRM","PAYJPMS KEY",FALSE,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJPRM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[PAYJPRM][JOB NUMBER]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJPRM][PAY IDENTIFICATION]");
	}

	x=addsearchfile(tmp,"PAYROLL","PAYPPMS",2,"PAYROLL","PAYJPRM","PAYPPMS KEY",FALSE,"([PAYROLL][PAYPPMS][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYPPMS][BENEFITS COMPUTED]=TRUE)",1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[PAYJPRM][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"PAY IDENTIFICATION","[PAYJPRM][PAY IDENTIFICATION]");
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
/*
	addsearchfile(tmp,"LVEMGT","LVEJOB",0,NULL,NULL,"LVEJOB KEY",FALSE,"([LVEMGT][LVEJOB][DELETEFLAG]=FALSE) AND ([LVEMGT][LVEJOB][ACTIVE]=TRUE)",1);

	x=addsearchfile(tmp,"LVEMGT","LVEMSTR",2,"LVEMGT","LVEJOB","LVEMSTR KEY",FALSE,"([LVEMGT][LVEMSTR][DELETEFLAG]=FALSE) AND ([LVEMGT][LVEMSTR][ACTIVE]=TRUE)",1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[LVEJOB][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"LEAVE DESCRIPTION IDENTIFICATION","[LVEJOB][LEAVE DESCRIPTION IDENTIFICATION]");
	}

	x=addsearchfile(tmp,"PRSNNL","PERDMG",2,"LVEMGT","LVEJOB","PERDMG KEY",TRUE,"[PRSNNL][PERDMG][DELETEFLAG]=FALSE",1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[LVEJOB][PERSONNEL IDENTIFICATION]");
	}

	x=addsearchfile(tmp,"LVEMGT","LVEDSC",2,"LVEMGT","LVEJOB","LVEDSC KEY",FALSE,"([LVEMGT][LVEDSC][DELETEFLAG]=FALSE) AND ([LVEMGT][LVEDSC][ACTIVE]=TRUE)",1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"LEAVE DESCRIPTION IDENTIFICATION","[LVEJOB][LEAVE DESCRIPTION IDENTIFICATION]");
	}

	x=addsearchfile(tmp,"PAYROLL","PAYJMST",2,"LVEMGT","LVEJOB","JOBMSTR KEY",TRUE,"([PAYROLL][PAYJMST][DELETEFLAG]=FALSE) AND ([PAYROLL][PAYJMST][ACTIVE]=TRUE)",1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"PERSONNEL IDENTIFICATION","[LVEJOB][PERSONNEL IDENTIFICATION]");
		addsearchconfld(s,"JOB NUMBER","[LVEJOB][JOB NUMBER]");
	}

	x=addsearchfile(tmp,"POSTRK","POSMSTR",1,"LVEMGT","LVEJOB","POSMSTR",TRUE,"([POSTRK][POSMSTR][DELETEFLAG]=FALSE)",1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"POSITION IDENTIFICATION","[POSITION IDENTIFICATION]");
	}

	x=addsearchfile(tmp,"POSTRK","POSLMST",1,"LVEMGT","LVEJOB","POSLMST",FALSE,NULL,1);
	if(x!=(-1))
	{
		s=tmp->search+(x-1);
		addsearchconfld(s,"LOCATION IDENTIFICATION","[LOCATION IDENTIFICATION]");
	}	
*/

/*======================================================================*/
/* Creating RDAvirtual Structures */
/*----------------------------------------------------------------------*/

	addvirtuallineADV(tmp,"SELECTED PAY IDENTIFICATION",PLAINTEXT,15,NULL,0,2);

	t=Rmalloc(400);
	sprintf(t,"%s%s%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]=1) \n",
		"THEN RETURN_VALUE=([PAYROLL][PAYJDPM][PERSONNEL IDENTIFICATION]) \n",
		"ELSE IF([PRIMARY FILE NUMBER]=2) \n",
		"     THEN RETURN_VALUE=([PAYROLL][PAYJPRM][PERSONNEL IDENTIFICATION]) \n",
		"ELSE IF([PRIMARY FILE NUMBER]=3) \n",
		"     THEN RETURN_VALUE=([LVEMGT][LVEMSTR][PERSONNEL IDENTIFICATION]) \n",
		"ELSE RETURN_VALUE=(\"\") FI FI FI\n");
	addvirtualline(tmp,"PERSONNEL IDENTIFICATION",PLAINTEXT,15,t,0);
	if(t!=NULL) Rfree(t);


	addvirtuallineADV(tmp,"NAME LAST LINEAGE FIRST MIDDLE",PLAINTEXT,40,
		"([PRSNNL][PERDMG][LAST NAME]+\" \"+[PRSNNL][PERDMG][LINEAGE]+\", \"+[PRSNNL][PERDMG][FIRST NAME]+\" \"+[PRSNNL][PERDMG][MIDDLE NAME])",0,1);

	addvirtualline(tmp,"DEDUCTION TYPE",PLAINTEXT,15,
		"([PAYROLL][PAYDDMS][DEDUCTION TYPE])",0);

	addvirtualline(tmp,"POSITION IDENTIFICATION",PLAINTEXT,30,
		"([PAYROLL][PAYJMST][POSITION IDENTIFICATION])",0);

	addvirtualline(tmp,"LOCATION IDENTIFICATION",PLAINTEXT,15,
		"([PRSNNL][PERDMG][LOCATION IDENTIFICATION])",0);

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
				"     ELSE RETURN_VALUE=[LVEMGT][LVEMSTR][PROCESSED EARNINGS][",
				(z+1),
				",1,10] FI FI");
			t=Rmalloc(30+6);
			sprintf(t,"LEAVE PROCESSED EARNINGS #%d",(z+1));
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

			expression=Rmalloc(RDAstrlen(stemp)+390);
			sprintf(expression,"%s%s%d%s%s%s%s%s%d%s%d%s",
				"IF(!([PRIMARY FILE NUMBER]=2)) THEN RETURN_VALUE=0 \n",
				"ELSE IF(!([LVEMGT][LVEMSTR][LEAVE DESCRIPTION IDENTIFICATION][",
				(z+1),
				",1,40]=[",
				stemp,
				"])) \n",
				"     THEN RETURN_VALUE=0 \n",
				"     ELSE RETURN_VALUE=([LVEMGT][LVEMSTR][PROCESSED EARNINGS][",
				(z+1),
				",1,10]-[LVEMGT][LVEMSTR][PROCESSED USED][",
				(z+1),
				",1,10]) FI FI");
			t=Rmalloc(30+6);
			sprintf(t,"LEAVE AVAILABLE #%d",(z+1));
			addvirtualline(tmp,t,SDECIMALV,8,expression,0);
			if(expression!=NULL) Rfree(expression);
			if(t!=NULL) Rfree(t);
		}
	}

/*
	t=Rmalloc(200);
	sprintf(t,"%s%s",
		"IF(!([PRIMARY FILE NUMBER]=3)) THEN RETURN_VALUE=0 \n",
		"ELSE RETURN_VALUE=[LVEMGT][LVEMSTR][PROCESSED EARNINGS] FI");
	addvirtualline(tmp,"LEAVE PROCESSED EARNINGS",SDECIMALV,8,t,0);

	t=Rmalloc(200);
	sprintf(t,"%s%s",
		"IF(!([PRIMARY FILE NUMBER]=3)) THEN RETURN_VALUE=0 \n",
		"ELSE RETURN_VALUE=[LVEMGT][LVEMSTR][PROCESSED USED] FI");
	addvirtualline(tmp,"LEAVE PROCESSED USED",SDECIMALV,8,t,0);

	t=Rmalloc(200);
	sprintf(t,"%s%s",
		"IF(!([PRIMARY FILE NUMBER]=3)) THEN RETURN_VALUE=0 \n",
		"ELSE RETURN_VALUE=([LVEMGT][LVEMSTR][PROCESSED EARNINGS]-[LVEMGT][LVEMSTR][PROCESSED USED]) FI");
	addvirtualline(tmp,"LEAVE PROCESSED AVAILABLE",SDECIMALV,8,t,0);
*/

	if(PayRates!=NULL)
	{
	if(PayRates->Rates!=NULL)
	{
	for(z=0,Rate=PayRates->Rates;z<PayRates->num;++z,++Rate)
	{
		t=Rmalloc(RDAstrlen(Rate->rateid)+350);
		sprintf(t,"%s%s%s%s%s%s%s%s%s",
			"IF(!([PRIMARY FILE NUMBER]=2)) \n",
			"THEN RETURN_VALUE=0 \n",
			"ELSE IF([PAYROLL][PAYJPRM][RATE IDENTIFICATION]=\"",
			Rate->rateid,
			"\") \n",
			"     THEN RETURN_VALUE=[PAYROLL][PAYJPRM][GROSS] \n",
			"     ELSE RETURN_VALUE=0 \n",
			"     FI \n",
			"FI");
		memset(stemp,0,101);
		sprintf(stemp,"%s",Rate->rateid);
		addvirtualline(tmp,stemp,DOLLARS,15,t,0);
		if(t!=NULL) Rfree(t);
	}
	}
	}

	t=Rmalloc(600);
	sprintf(t,"%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]=1) THEN\n",
		"	IF([PAYROLL][PAYDDMS][REDUCE NET]=TRUE) \n",
		"	THEN RETURN_VALUE=((-1)*([PAYROLL][PAYJDPM][AMOUNT])) \n",
		"	ELSE RETURN_VALUE=0 FI\n",
		"ELSE RETURN_VALUE=0 FI");
	addvirtualline(tmp,"EMPLOYEE AMOUNT",DOLLARS,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(600);
	sprintf(t,"%s%s%s%s%s",
		"IF([PRIMARY FILE NUMBER]=1) THEN\n",
		"	IF([PAYROLL][PAYDDMS][REDUCE NET]=FALSE) \n",
		"	THEN RETURN_VALUE=((-1)*([PAYROLL][PAYJDPM][AMOUNT])) \n",
		"	ELSE RETURN_VALUE=0 FI\n",
		"ELSE RETURN_VALUE=0 FI");
	addvirtualline(tmp,"MATCHING AMOUNT",DOLLARS,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(400);
	sprintf(t,"%s%s%s%s%s%s%s%s",
		"IF(!([PRIMARY FILE NUMBER]=1)) \n",
		"THEN RETURN_VALUE=0 \n",
		"ELSE IF(([PAYROLL][PAYDDMS][REDUCE NET]=TRUE) \n",
		"       AND (!([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"DIRECT DEPOSIT\"))) \n",
		"     THEN RETURN_VALUE=((-1)*([PAYROLL][PAYJDPM][AMOUNT])) \n",
		"     ELSE RETURN_VALUE=0 \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"REDUCE NET AMOUNT",DOLLARS,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(350);
	sprintf(t,"%s%s%s%s%s%s%s%s%s",
		"IF(!([PRIMARY FILE NUMBER]=1)) \n",
		"THEN RETURN_VALUE=0 \n",
		"ELSE IF(([PAYROLL][PAYDDMS][REDUCE NET]=TRUE) \n",
		"       AND ([PAYROLL][PAYDTYP][VOLUNTARY]=TRUE) \n",
		"       AND (!([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"DIRECT DEPOSIT\"))) \n",
		"     THEN RETURN_VALUE=[PAYROLL][PAYJDPM][AMOUNT] \n",
		"     ELSE RETURN_VALUE=0 \n",
		"     FI \n",
		"FI");
	addvirtualline(tmp,"DEDUCTION AMOUNT",DOLLARS,15,t,0);
	if(t!=NULL) Rfree(t);

	t=Rmalloc(250);
	sprintf(t,"%s%s%s",
		"IF([PRIMARY FILE NUMBER]=2) \n",
		"THEN RETURN_VALUE=[PAYROLL][PAYJPRM][GROSS] \n",
		"ELSE RETURN_VALUE=0 FI");
	addvirtualline(tmp,"GROSS AMOUNT",DOLLARS,15,t,0);
	if(t!=NULL) Rfree(t); 

	addvirtualline(tmp,"NET AMOUNT",DOLLARS,15,
		"([TOTAL GROSS AMOUNT]+[TOTAL REDUCE NET AMOUNT])",1);

	if(Deduction_Types->all_dedtypes!=NULL)
	{
	if(Deduction_Types->all_dedtypes->numlibs>0)
	{
	for(z=0;z<Deduction_Types->all_dedtypes->numlibs;++z)
	{
		memset(stemp,0,101);
		sprintf(stemp,"%s AMOUNT",Deduction_Types->all_dedtypes->libs[z]);
		t=Rmalloc(RDAstrlen(Deduction_Types->all_dedtypes->libs[z])+400);
		sprintf(t,"%s%s%s%s%s%s%s%s%s%s",
			"IF([PRIMARY FILE NUMBER]=1) \n",
			"THEN IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"",
			(Deduction_Types->all_dedtypes->libs[z]==NULL?"":Deduction_Types->all_dedtypes->libs[z]),
			"\") AND \n",
			"        ([PAYROLL][PAYDDMS][REDUCE NET]=TRUE)) \n",
			"     THEN RETURN_VALUE=((-1)*([PAYROLL][PAYJDPM][AMOUNT])) \n",
			"     ELSE RETURN_VALUE=0 \n",
			"     FI \n",
			"ELSE RETURN_VALUE=0 \n",
			"FI");
		addvirtualline(tmp,stemp,DOLLARS,15,t,0);
		if(t!=NULL) Rfree(t);

		memset(stemp,0,101);
		sprintf(stemp,"%s MATCHING AMOUNT",Deduction_Types->all_dedtypes->libs[z]);
		t=Rmalloc(RDAstrlen(Deduction_Types->all_dedtypes->libs[z])+400);
		sprintf(t,"%s%s%s%s%s%s%s%s%s%s",
			"IF([PRIMARY FILE NUMBER]=1) \n",
			"THEN IF(([PAYROLL][PAYDDMS][DEDUCTION TYPE]=\"",
			(Deduction_Types->all_dedtypes->libs[z]==NULL?"":Deduction_Types->all_dedtypes->libs[z]),
			"\") AND \n",
			"        ([PAYROLL][PAYDDMS][REDUCE NET]=FALSE)) \n",
			"     THEN RETURN_VALUE=((-1)*([PAYROLL][PAYJDPM][AMOUNT])) \n",
			"     ELSE RETURN_VALUE=0 \n",
			"     FI \n",
			"ELSE RETURN_VALUE=0 \n",
			"FI");
		addvirtualline(tmp,stemp,DOLLARS,15,t,0);
		if(t!=NULL) Rfree(t);
	}
	}
	}

	addvirtualline(tmp,"REPORT TITLE",VARIABLETEXT,0,
		"(\"PAYROLL REGISTER  for \"+[SELECTED PAY IDENTIFICATION])",1);

	addvirtualline(tmp,"EXECUTED BY",VARIABLETEXT,0,
		"(\"Executed By: \"+[LOGIN IDENTIFICATION])",1);

	addvirtualline(tmp,"DEDUCTION TYPE",PLAINTEXT,15,
		"[PAYROLL][PAYDDMS][DEDUCTION TYPE]",0);

	t=Rmalloc(150);
	sprintf(t,"%s%s%s",
		"IF([PRIMARY FILE NUMBER]=1) \n",
		"THEN RETURN_VALUE=([PAYROLL][PAYDTYP][VOLUNTARY]) \n",
		"ELSE RETURN_VALUE=FALSE FI");
	addvirtualline(tmp,"VOLUNTARY",BOOLNS,1,t,0);
	if(t!=NULL) Rfree(t);

/*======================================================================*/

	addsortline(tmp,0,"[LOCATION IDENTIFICATION]");
	addcontrolline(tmp,"[LOCATION IDENTIFICATION]",VARIABLETEXT);
	addsortline(tmp,0,"[POSITION IDENTIFICATION]");
	addcontrolline(tmp,"[POSITION IDENTIFICATION]",VARIABLETEXT);
	addsortline(tmp,0,"[PERSONNEL IDENTIFICATION]");
	addcontrolline(tmp,"[PERSONNEL IDENTIFICATION]",VARIABLETEXT);
	addsortline(tmp,0,"[WHICH TYPE]");
	addcontrolline(tmp,"[WHICH TYPE]",SHORTV);
	addsortline(tmp,0,"[IDENTIFICATION]");
	addcontrolline(tmp,"[IDENTIFICATION]",VARIABLETEXT);

/*--------------------------------------------------------------------*/

	if(PayRates!=NULL)
	{
	if(PayRates->Rates!=NULL)
	{
	for(z=0,Rate=PayRates->Rates;z<PayRates->num;++z,++Rate)
	{
		memset(stemp,0,101);
		sprintf(stemp,"TOTAL %s",Rate->rateid);
		t=Rmalloc(RDAstrlen(Rate->rateid)+5);
		sprintf(t,"[%s]",Rate->rateid);
		addaccumline(tmp,stemp,7,t,DOLLARS);
		if(t!=NULL) Rfree(t);
	}
	}
	}
	addaccumline(tmp,"TOTAL REDUCE NET AMOUNT",7,"[REDUCE NET AMOUNT]",DOLLARS);
	addaccumline(tmp,"TOTAL GROSS AMOUNT",7,"[GROSS AMOUNT]",DOLLARS);
	addaccumline(tmp,"TOTAL DEDUCTION AMOUNT",7,"[DEDUCTION AMOUNT]",DOLLARS);
	addaccumline(tmp,"TOTAL NET AMOUNT",7,"[NET AMOUNT]",SDECIMALV);
	addaccumline(tmp,"TOTAL EMPLOYEE AMOUNT",7,"[EMPLOYEE AMOUNT]",DOLLARS);
	addaccumline(tmp,"TOTAL MATCHING AMOUNT",7,"[MATCHING AMOUNT]",DOLLARS);
	addaccumline(tmp,"TOTAL DIRECT DEPOSIT AMOUNT",7,"[DIRECT DEPOSIT AMOUNT]",DOLLARS);
	addaccumline(tmp,"LAST POSTRK LOCATION IDENTIFICATION",3,"[POSTRK][POSMSTR][LOCATION IDENTIFICATION]",VARIABLETEXT);
	addaccumline(tmp,"LAST LOCATION IDENTIFICATION",3,"[LOCATION IDENTIFICATION]",VARIABLETEXT);
	addaccumline(tmp,"LAST POSITION IDENTIFICATION",3,"[PAYROLL][PAYJMST][POSITION IDENTIFICATION]",VARIABLETEXT);
	addaccumline(tmp,"LAST PERSONNEL IDENTIFICATION",3,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION]",VARIABLETEXT);
	addaccumline(tmp,"LAST EMPLOYEE NAME",3,"[NAME LAST LINEAGE FIRST MIDDLE]",VARIABLETEXT);
	addaccumline(tmp,"LAST SOCIAL SECURITY NUMBER",3,"[PRSNNL][PERDMG][SOCIAL SECURITY NUMBER]",SOCSECNUM);
	addaccumline(tmp,"LAST WHICH TYPE",3,"[WHICH TYPE]",SHORTV);
	addaccumline(tmp,"LAST IDENTIFICATION",3,"[IDENTIFICATION]",VARIABLETEXT);
	addaccumline(tmp,"LAST CHECK NUMBER",3,"[PAYROLL][PAYJPMS][CHECK NUMBER]",LONGV);
	addaccumline(tmp,"LAST CHECK DATE",3,"[PAYROLL][PAYJPMS][CHECK DATE]",DATES);

	if(lvedsclist!=NULL)
	{
		for(z=0;z<lvedsclist->numlibs;++z)
		{
			memset(stemp,0,101);
			sprintf(stemp,"LAST LEAVE DESCRIPTION IDENTIFICATION #%d",(z+1));
			t=Rmalloc(40+6);
			sprintf(t,"[LEAVE DESCRIPTION IDENTIFICATION #%d]",(z+1));
			addaccumline(tmp,stemp,3,t,VARIABLETEXT);
			if(t!=NULL) Rfree(t);

			memset(stemp,0,101);
			sprintf(stemp,"LAST LEAVE PROCESSED EARNINGS #%d",(z+1));
			t=Rmalloc(40+6);
			sprintf(t,"[LEAVE PROCESSED EARNINGS #%d]",(z+1));
			addaccumline(tmp,stemp,3,t,DOLLARS);
			if(t!=NULL) Rfree(t);

			memset(stemp,0,101);
			sprintf(stemp,"LAST LEAVE PROCESSED USED #%d",(z+1));
			t=Rmalloc(40+6);
			sprintf(t,"[LEAVE PROCESSED USED #%d]",(z+1));
			addaccumline(tmp,stemp,3,t,DOLLARS);
			if(t!=NULL) Rfree(t);

			memset(stemp,0,101);
			sprintf(stemp,"LAST LEAVE AVAILABLE #%d",(z+1));
			t=Rmalloc(40+6);
			sprintf(t,"[LEAVE AVAILABLE #%d]",(z+1));
			addaccumline(tmp,stemp,3,t,DOLLARS);
			if(t!=NULL) Rfree(t);
		}
	}
	
/*
	addaccumline(tmp,"TOTAL LEAVE PROCESSED EARNINGS",7,"[LEAVE PROCESSED EARNINGS]",SDECIMALV);
	
	addaccumline(tmp,"TOTAL LEAVE PROCESSED USED",7,"[LEAVE PROCESSED USED]",SDECIMALV);
	
	addaccumline(tmp,"TOTAL LEAVE PROCESSED AVAILABLE",7,"[LEAVE PROCESSED AVAILABLE]",SDECIMALV);
*/

	if(Deduction_Types->all_dedtypes!=NULL)
	{
	if(Deduction_Types->all_dedtypes->numlibs>0)
	{
	for(z=0;z<Deduction_Types->all_dedtypes->numlibs;++z)
	{
		memset(stemp,0,101);
		sprintf(stemp,"TOTAL %s AMOUNT",Deduction_Types->all_dedtypes->libs[z]);
		t=Rmalloc(RDAstrlen(Deduction_Types->all_dedtypes->libs[z])+12);
		sprintf(t,"[%s AMOUNT]",Deduction_Types->all_dedtypes->libs[z]);
		addaccumline(tmp,stemp,7,t,VARIABLETEXT);
		if(t!=NULL) Rfree(t);

		memset(stemp,0,101);
		sprintf(stemp,"TOTAL %s MATCHING AMOUNT",Deduction_Types->all_dedtypes->libs[z]);
		t=Rmalloc(RDAstrlen(Deduction_Types->all_dedtypes->libs[z])+20);
		sprintf(t,"[%s MATCHING AMOUNT]",Deduction_Types->all_dedtypes->libs[z]);
		addaccumline(tmp,stemp,7,t,VARIABLETEXT);
		if(t!=NULL) Rfree(t);
	}
	}
	}


/*--------------------------------------------------------------------*/
/* Making Headers */

	imageoff=addimageline(tmp,"PAGE HEADER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"PAGE HEADER",
	"                @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC  PAGE: @RRRRRRR@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[ORGANIZATION NAME]",0,NULL);
	addimagepval(image,0,"[PAGE NO]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"PAGE HEADER",
	"                @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC DATE: @LLLLLLL@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[REPORT TITLE]",0,NULL);
	addimagepval(image,0,"[SYSTEM DATE]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"PAGE HEADER",
	"                @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC  TIME: @LLLLLLL@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[EXECUTED BY]",0,NULL);
	addimagepval(image,0,"[SYSTEM TIME]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"PAGE HEADER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"PAGE HEADER",
	"LOCATION IDENTIFICATION                                                        LEAVE IDENTIFICATION  EARNINGS - USED= AVAILABLE@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"PAGE HEADER",
	"   POSITION IDENTIFICATION                                                    EMPLOYEE          EMPLOYER   PROCESSED LEAVE@@",
		FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"PAGE HEADER",
	"      SOCIAL SECURITY  EMPLOYEE NAME                      DESCRIPTION           AMOUNT            AMOUNT@@",
		FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"PAGE HEADER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[LOCATION IDENTIFICATION] HEADER",
	"@VVVVVVVVVVVVVVVV @VVVVVVVVVVVVVVVVVV@@",
		FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[LOCATION IDENTIFICATION]",0,NULL);
	addimagepval(image,0,"[POSTRK][POSLMST][DESCRIPTION]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[POSITION IDENTIFICATION] HEADER",
	"   @VVVVVVVV @VVVVVVVVV@@",
		FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[POSTRK][POSMSTR][POSITION IDENTIFICATION]",0,NULL);
	addimagepval(image,0,"[POSTRK][POSMSTR][DESCRIPTION]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[POSITION IDENTIFICATION] HEADER",
	"   ============================================================================================================================@@",
		FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PERSONNEL IDENTIFICATION] HEADER",
	"      @LLLLLLLLLLL @VVVVVVVVVVVVVVVV @VVVVVVVVVVVVVVVVVV@@",
		TRUE,"([WHICH TYPE]=0)",0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[PRSNNL][PERDMG][SOCIAL SECURITY NUMBER]",0,NULL);
	addimagepval(image,0,"[PRSNNL][PERDMG][PERSONNEL IDENTIFICATION]",0,NULL);
	addimagepval(image,0,"[NAME LAST LINEAGE FIRST MIDDLE]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

/* Making Detail Lines*/
/* Making Footers*/

	if(lvedsclist!=NULL)
	{
		for(z=0;z<lvedsclist->numlibs;++z)
		{
			expression=Rmalloc(350);
			sprintf(expression,"(([LAST WHICH TYPE]=0) AND ((!([LAST LEAVE PROCESSED EARNINGS #%d]=0)) OR (!([LAST LEAVE PROCESSED USED #%d]=0))))",(z+1),(z+1));
			imageoff=addimageline(tmp,"[IDENTIFICATION] FOOTER",
			"                                                          @RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR  @RRRR.RR @RRRR.RR @RRRR.RR@@",
				TRUE,
				expression,
				0,NULL);
			if(expression!=NULL) Rfree(expression);
			image=tmp->image+(imageoff-1);
			memset(stemp,0,101);
			sprintf(stemp,"[LAST LEAVE DESCRIPTION IDENTIFICATION #%d]",(z+1));
			addimagepval(image,0,stemp,0,NULL);
			memset(stemp,0,101);
			sprintf(stemp,"[LAST LEAVE PROCESSED EARNINGS #%d]",(z+1));
			addimagepval(image,0,stemp,0,NULL);
			memset(stemp,0,101);
			sprintf(stemp,"[LAST LEAVE PROCESSED USED #%d]",(z+1));
			addimagepval(image,0,stemp,0,NULL);
			memset(stemp,0,101);
			sprintf(stemp,"[LAST LEAVE AVAILABLE #%d]",(z+1));
			addimagepval(image,0,stemp,0,NULL);
			addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
			addimagepval(image,1,"LINEFEED",0,NULL);
		}
	}

/*
	imageoff=addimageline(tmp,"[IDENTIFICATION] FOOTER",
	"                                                         @RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR  @RRRR.RR @RRRR.RR @RRRR.RR@@",
		TRUE,
		"([LAST WHICH TYPE]=0) AND ([TOTAL LEAVE PROCESSED AVAILABLE]!=0)",

		0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[LAST IDENTIFICATION]",0,NULL);
	addimagepval(image,0,"[TOTAL LEAVE PROCESSED EARNINGS]",0,NULL);
	addimagepval(image,0,"[TOTAL LEAVE PROCESSED USED]",0,NULL);
	addimagepval(image,0,"[TOTAL LEAVE PROCESSED AVAILABLE]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);
*/

/*
	imageoff=addimageline(tmp,"[WHICH TYPE] FOOTER",
	"                                                          TOTAL GROSS   @.,RRRRRRRRRRRR@@",
		TRUE,"([LAST WHICH TYPE]=1)",0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[TOTAL GROSS AMOUNT]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);
*/

/*
	expression=FORMAT_STRING(Rate->rateid,' ',64,__LINE__,__FILE__);
*/
	if(PayRates!=NULL)
	{
	if(PayRates->Rates!=NULL)
	{
	for(z=0,Rate=PayRates->Rates;z<PayRates->num;++z,++Rate)
	{
	memset(temp,0,15000);
	memset(temp1,0,15000);
	sprintf(temp,"                  %51s   @,.RRRRRRRRRRRR@@",Rate->rateid);
	memset(stemp,0,101);
	sprintf(stemp,"[TOTAL %s]",Rate->rateid);
	sprintf(temp1,"([LAST WHICH TYPE]=0) AND (%s!=0)",stemp);
	imageoff=addimageline(tmp,"[WHICH TYPE] FOOTER",temp,
		TRUE,temp1,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,stemp,0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);
	}
	}
	}
	imageoff=addimageline(tmp,"[WHICH TYPE] FOOTER",
	"                                                                        ---------------@@",
		TRUE,"([LAST WHICH TYPE]=0) AND ([TOTAL GROSS AMOUNT]!=0)",0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);
	imageoff=addimageline(tmp,"[WHICH TYPE] FOOTER",
	"                                                          TOTAL GROSS   @,.RRRRRRRRRRRR@@",
		TRUE,"([LAST WHICH TYPE]=0) AND ([TOTAL GROSS AMOUNT]!=0)",0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[TOTAL GROSS AMOUNT]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[IDENTIFICATION] FOOTER",
	"                  @RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR   @,.RRRRRRRRRRRR@@",
		TRUE,
		"(([LAST WHICH TYPE]=1) AND ([TOTAL EMPLOYEE AMOUNT]!=0) AND ([TOTAL MATCHING AMOUNT]=0))",
		0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[LAST IDENTIFICATION]",0,NULL);
	addimagepval(image,0,"[TOTAL EMPLOYEE AMOUNT]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[IDENTIFICATION] FOOTER",
	"                  @RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR   @,.RRRRRRRRRRRR   @,.RRRRRRRRRRRR@@",
		TRUE,
		"(([LAST WHICH TYPE]=1) AND ([TOTAL EMPLOYEE AMOUNT]!=0) AND ([TOTAL MATCHING AMOUNT]!=0))",
		0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[LAST IDENTIFICATION]",0,NULL);
	addimagepval(image,0,"[TOTAL EMPLOYEE AMOUNT]",0,NULL);
	addimagepval(image,0,"[TOTAL MATCHING AMOUNT]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[IDENTIFICATION] FOOTER",
	"                  @RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR                     @,.RRRRRRRRRRRR@@",
		TRUE,
		"(([LAST WHICH TYPE]=1) AND ([TOTAL EMPLOYEE AMOUNT]=0) AND ([TOTAL MATCHING AMOUNT]!=0))",
		0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[LAST IDENTIFICATION]",0,NULL);
	addimagepval(image,0,"[TOTAL MATCHING AMOUNT]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,
	"[PERSONNEL IDENTIFICATION] FOOTER",
	"        TOTAL GROSS       TOTAL DEDUCTIONS                              ---------------@@",
		FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,
	"[PERSONNEL IDENTIFICATION] FOOTER",
	"      (@,.RRRRRRRRRRRR )+(@,.RRRRRRRRRRRR ) = (CHECK #:@LLLLLLLL ) NET  @,.RRRRRRRRRRRR CHECK DATE:@LLLLLLLLL@@",
		TRUE,
		"(([TOTAL GROSS AMOUNT]+[TOTAL REDUCE NET AMOUNT]+[TOTAL DIRECT DEPOSIT AMOUNT])>0)",
		0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[TOTAL GROSS AMOUNT]",0,NULL);
	addimagepval(image,0,"[TOTAL REDUCE NET AMOUNT]",0,NULL);
	addimagepval(image,0,"[LAST CHECK NUMBER]",0,NULL);
	addimagepval(image,0,"[NET AMOUNT]",0,NULL);
	addimagepval(image,0,"[LAST CHECK DATE]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,
	"[PERSONNEL IDENTIFICATION] FOOTER",
	"      (@,.RRRRRRRRRRRR )+(@,.RRRRRRRRRRRR ) = (Direct Deposit)     NET  @,.RRRRRRRRRRRR@@",
		TRUE,
		"(([TOTAL GROSS AMOUNT]+[TOTAL REDUCE NET AMOUNT]+[TOTAL DIRECT DEPOSIT AMOUNT])=0)",
		0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[TOTAL GROSS AMOUNT]",0,NULL);
	addimagepval(image,0,"[TOTAL REDUCE NET AMOUNT]",0,NULL);
	addimagepval(image,0,"[NET AMOUNT]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PERSONNEL IDENTIFICATION] FOOTER",
	"      =========================================================================================================================@@",
		FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[PERSONNEL IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[POSITION IDENTIFICATION] FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"[LOCATION IDENTIFICATION] FOOTER","@@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"FORMFEED",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"PAGE FOOTER","@@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);
	addimagepval(image,1,"FORMFEED",0,NULL);

/*
	imageoff=addimageline(tmp,"REPORT FOOTER","@@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"FORMFEED",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"REPORT FOOTER",
	"                                     EMPLOYEE             EMPLOYER@@",
		FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"REPORT FOOTER",
	"                                       AMOUNT               AMOUNT@@",
		FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);
*/

	if(PayRates!=NULL)
	{
	if(PayRates->Rates!=NULL)
	{
	for(z=0,Rate=PayRates->Rates;z<PayRates->num;++z,++Rate)
	{
		expression=FORMAT_STRING(Rate->rateid,' ',64,__LINE__,__FILE__);
		t=Rmalloc(RDAstrlen(expression)+100);
		sprintf(t,"TOTAL %s  @,.RRRRRRRRRRRRR@@",expression);
		memset(stemp,0,101);
		sprintf(stemp,"(!([TOTAL %s]=0))",Rate->rateid);
		imageoff=addimageline(tmp,"REPORT FOOTER",t,TRUE,stemp,0,NULL);
		image=tmp->image+(imageoff-1);
		memset(stemp,0,101);
		sprintf(stemp,"[TOTAL %s]",Rate->rateid);
		addimagepval(image,0,stemp,0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
		if(t!=NULL) Rfree(t);
		memset(stemp,0,101);
	}
	}
	}

	if(Deduction_Types->all_dedtypes!=NULL)
	{
	if(Deduction_Types->all_dedtypes->numlibs>0)
	{
	for(z=0;z<Deduction_Types->all_dedtypes->numlibs;++z)
	{
		expression=FORMAT_STRING(Deduction_Types->all_dedtypes->libs[z],' ',64,__LINE__,__FILE__);
		t=Rmalloc(RDAstrlen(expression)+100);
		sprintf(t,
		"TOTAL %s  @,.RRRRRRRRRRRRR  @,.RRRRRRRRRRRRR@@",expression);
		memset(stemp,0,101);
		sprintf(stemp,"(!([TOTAL %s AMOUNT]=0)) AND (!([TOTAL %s MATCHING AMOUNT]=0))",Deduction_Types->all_dedtypes->libs[z],Deduction_Types->all_dedtypes->libs[z]);
		imageoff=addimageline(tmp,"REPORT FOOTER",t,TRUE,stemp,0,NULL);
		image=tmp->image+(imageoff-1);
		memset(stemp,0,101);
		sprintf(stemp,"[TOTAL %s AMOUNT]",Deduction_Types->all_dedtypes->libs[z]);
		addimagepval(image,0,stemp,0,NULL);
		sprintf(stemp,"[TOTAL %s MATCHING AMOUNT]",Deduction_Types->all_dedtypes->libs[z]);
		addimagepval(image,0,stemp,0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
		if(t!=NULL) Rfree(t);
		t=Rmalloc(RDAstrlen(expression)+100);
		sprintf(t,
		"TOTAL %s  @,.RRRRRRRRRRRRR@@",expression);
		memset(stemp,0,101);
		sprintf(stemp,"(!([TOTAL %s AMOUNT]=0)) AND ([TOTAL %s MATCHING AMOUNT]=0)",Deduction_Types->all_dedtypes->libs[z],Deduction_Types->all_dedtypes->libs[z]);
		imageoff=addimageline(tmp,"REPORT FOOTER",t,TRUE,stemp,0,NULL);
		image=tmp->image+(imageoff-1);
		memset(stemp,0,101);
		sprintf(stemp,"[TOTAL %s AMOUNT]",Deduction_Types->all_dedtypes->libs[z]);
		addimagepval(image,0,stemp,0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
		if(t!=NULL) Rfree(t);
		t=Rmalloc(RDAstrlen(expression)+100);
		sprintf(t,
		"TOTAL %s                    @,.RRRRRRRRRRRRR@@",expression);
		memset(stemp,0,101);
		sprintf(stemp,"([TOTAL %s AMOUNT]=0) AND (!([TOTAL %s MATCHING AMOUNT]=0))",Deduction_Types->all_dedtypes->libs[z],Deduction_Types->all_dedtypes->libs[z]);
		imageoff=addimageline(tmp,"REPORT FOOTER",t,TRUE,stemp,0,NULL);
		image=tmp->image+(imageoff-1);
		memset(stemp,0,101);
		sprintf(stemp,"[TOTAL %s MATCHING AMOUNT]",Deduction_Types->all_dedtypes->libs[z]);
		addimagepval(image,0,stemp,0,NULL);
		addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
		addimagepval(image,1,"LINEFEED",0,NULL);
		if(t!=NULL) Rfree(t);
		if(expression!=NULL) Rfree(expression);
	}
	}
	}

	imageoff=addimageline(tmp,"REPORT FOOTER",
	"           TOTAL GROSS    TOTAL DEDUCTIONS                              ----------------@@",
		FALSE,NULL,0,NULL);
/*
  	"                                                                        ----------------     TOTAL GROSS        TOTAL DEDUCTIONS@@",
*/
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"REPORT FOOTER",
	"     (@,.RRRRRRRRRRRRR )+(@,.RRRRRRRRRRRRR ) =              TOTAL NET   @,.RRRRRRRRRRRRR@@", 
/*
	"TOTAL NET                                                               @,.RRRRRRRRRRRRR  =  @,.RRRRRRRRRRRRR + @,.RRRRRRRRRRRRR@@",
*/
		FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,0,"[TOTAL GROSS AMOUNT]",0,NULL);
	addimagepval(image,0,"[TOTAL REDUCE NET AMOUNT]",0,NULL);
	addimagepval(image,0,"[NET AMOUNT]",0,NULL);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	imageoff=addimageline(tmp,"REPORT FOOTER","@@",FALSE,NULL,0,NULL);
	image=tmp->image+(imageoff-1);
	addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
	addimagepval(image,1,"LINEFEED",0,NULL);

	SetReportDefaultEngine(tmp);

	if(tmp!=NULL)
	{
		if(WRTRDAreport(module,tmp))
		{
			ERRORDIALOG("Cannot Write the Payroll Register Report","Error in permissions, user cannot write the Payroll Register Report definition.  Check your permissions, and retry.  Call your installer.",NULL,FALSE);
			prterr("Error in permissions, user cannot write the report definition.  Check your permissions, and retry.");
		} else {
			PAYREG_RANGE_SCREEN();
			LST_PAYROLL_SELECTED_PAY_IDENTIFICATIONS();
		}
		FreeRDAreport(tmp);
	}
	} else {
		ERRORDIALOG("CANNOT CREATE PAYROLL REGISTER REPORT!","Error in PAYDTYP data base file.  Can not Allocate Space for Deduction Types Structure.  Please check that their is enough per user memory available and that the Payroll Deduction Type data file exist and is defined correctly and then retry.",NULL,FALSE);
		prterr("Error in PAYDTYP data base file.  Can not Allocate Space for Deduction Types Structure.");  
	}
}
static void LST_PAYROLL_SELECTED_PAY_IDENTIFICATIONS()
{
	RDAScrolledList *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	scrn=RDAScrolledListNew("PAYROLL","PAYROLL SELECTED PAY IDENTIFICATIONS");
	if(scrn!=NULL)
	{
		scrn->vtype=0;
		scrn->type=0;
		ScrolledListDFincvir(scrn,"PAYROLL","PAYMSTR","",0);
	temp1=Rmalloc(38+1);
	sprintf(temp1,"%s",
		"([PAYROLL][PAYMSTR][DELETEFLAG]=FALSE)");
		scrn->select_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		scrn->special_function=stralloc("");
		scrn->num=0;
		scrn->initfld=NULL;
		scrn->ffield=stralloc("");
		scrn->ffinfo=Rmalloc(sizeof(DFincvir));
		scrn->ffinfo->module=stralloc("PAYROLL");
		scrn->ffinfo->file=stralloc("PAYMSTR");
		scrn->ffinfo->keyname=stralloc("PAYMSTR KEY");
		scrn->contype=3;
		scrn->conmod=stralloc("SELECTED PAY IDENTIFICATION");
		scrn->confil=stralloc("");
		scrn->confld=stralloc("");
	temp1=Rmalloc(38+1);
	sprintf(temp1,"%s",
		"[PAYROLL][PAYMSTR][PAY IDENTIFICATION]");
		scrn->format_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		scrn->reformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(38+1);
	sprintf(temp1,"%s",
		"[PAYROLL SELECTED PAY IDENTIFICATIONS]");
		scrn->unformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		scrn->desc=stralloc("This Scrolled List is created from PAY IDENTIFICATION field in PAYMSTR and used for the same field.");
		scrn->timing=0;

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.LST",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.LST",CURRENTDIRECTORY,module);
#endif
		if(writeScrolledListbin(defdir,scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(36+7+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [PAYROLL] RDAScrolledList [PAYROLL SELECTED PAY IDENTIFICATIONS], Can Not Save Scrolled List!");
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) FreeRDAScrolledList(scrn);
	}
}
/* PAYROLL's - Make (PAYREG RANGE SCREEN) Screen */
void PAYREG_RANGE_SCREEN()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;

	scrn=RDAscrnNEW("PAYROLL","PAYREG RANGE SCREEN");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Pay Identification:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"PAYROLL SELECTED PAY IDENTIFICATIONS","","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"SELECTED PAY IDENTIFICATION","Selected Pay Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","RANGE CRITERIA","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","FROM","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","TO","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [PERDMG][PERSONNEL IDENTIFICATION]","Personnel ID","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [PERDMG][PERSONNEL IDENTIFICATION]","From Personnel Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [PERDMG][PERSONNEL IDENTIFICATION]","To Personnel Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [PERDMG][LOCATION IDENTIFICATION]","Pay Location","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [PERDMG][LOCATION IDENTIFICATION]","From Location Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [PERDMG][LOCATION IDENTIFICATION]","To Location Identification","","",0,15,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"RANGE ON [NAME LAST LINEAGE FIRST MIDDLE]","Name","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM [NAME LAST LINEAGE FIRST MIDDLE]","From Name","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO [NAME LAST LINEAGE FIRST MIDDLE]","To Name","","",0,25,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"RUN REPORT","Select","","",0,0,0,NULL,NULL,"([SELECTED PAY IDENTIFICATION]!=\"\")",NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);

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
