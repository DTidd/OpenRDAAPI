/* comptax.c - Compute Real Estate Taxes */
#ifndef WIN32
#define __NAM__ "comprlstbkld.lnx"
#endif
#ifdef WIN32
#define __NAM__ "comprlstbkld.exe"
#endif
#include <app.hpp>
#include <mix.hpp>
#include <fin.hpp>
#include <mkmsc.hpp>

/*ADDLIB mklib */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static char *module="RLSTMGT",*COMPUTE_DATE=NULL;
static char USE_COMMON_OWNER=FALSE;
static short rstmst=(-1),rstcml=(-1);
static short rstmexp=(-1),rstexp=(-1);
static short rstcls=(-1),fiscal_month=0;
static short rstran=(-1),rstmil=(-1);
static short rstmmexp=(-1),owner=(-1);
static short finracc=(-1),finbacc=(1),finryr=(-1),finbyr=(-1);
static int tax_year=1999,fiscalyear=1999;
static double tot_val_exmp=0.0;
static double tot_tax_exmp=0.0;
static RDArsrc *diagrsrc=NULL;
static APPlib *errorlist=NULL;
static double tax_value=0.0;
static double tax_net_value=0.0;
static char use_exemption_detail=FALSE;

short MySubData(char **tmp,char *modulename)
{
/*
        short start=1,length=0,str_length=0;
        char *temp=NULL,*gn=NULL;
*/
	double units=0.0;

        if(diagvirtual || diageval)
        {
                prterr("DIAG MySubData() Substituting for [%s].",*tmp);
        }
	if(!RDAstrcmp(modulename,"TAXABLE"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%f",tax_value);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"TAXABLE NET"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%f",tax_net_value);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"UNITS"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%.0f",units);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
	}
	return(1);
}
void ComputeRlstMgtSubData(char **tmp,RDArsrc *r)
{
	char *modulename=NULL;

	if(diagvirtual || diageval)
	{
		prterr("DIAG ComputeRlstMgtSubData Substituting for [%s].",*tmp);
	}
	modulename=stripmodulename(*tmp);
	if(!isEMPTY(modulename))
	{
		if(SCRNExecuteSubData(SCRNvirtualFIELDSubData,tmp,r,modulename)==0)
		{
			if(diagvirtual|| diageval)
			{
				prterr("DIAG ComputeRlstMgtSubData Returning [%s].",*tmp);
			}
			if(modulename!=NULL) Rfree(modulename);
			return;
		} else if(MySubData(tmp,modulename)==0)
		{
			if(diagvirtual|| diageval)
			{
				prterr("DIAG ComputeRlstMgtSubData Returning [%s].",*tmp);
			}
			if(modulename!=NULL) Rfree(modulename);
			return;
		} else if(SCRNExecuteSubData(SCRNvirtualVIRTUALSubData,tmp,r,modulename)==0)
		{
			if(diagvirtual|| diageval)
			{
				prterr("DIAG ComputeRlstMgtSubData Returning [%s].",*tmp);
			}
			if(modulename!=NULL) Rfree(modulename);
			return;
		} else if(ScrolledListSubData(tmp,r)==0)
		{
			if(diagvirtual|| diageval)
			{
				prterr("DIAG ComputeRlstMgtSubData Returning [%s].",*tmp);
			}
			if(modulename!=NULL) Rfree(modulename);
			return;
		} else if(GLOBALSubData(tmp,modulename)==0)
		{
			if(diagvirtual|| diageval)
			{
				prterr("DIAG ComputeRlstMgtSubData Returning [%s].",*tmp);
			}
			if(modulename!=NULL) Rfree(modulename);
			return;
		} else {
			prterr("Error ComputeRlstMgtSubData [%s] not found.",*tmp);
			if(*tmp!=NULL) Rfree(*tmp);
			*tmp=stralloc("\"\"");
			if(diagvirtual || diageval)
			{
					prterr("DIAG ComputeRlstMgtSubData Returning [%s].",*tmp);
			}
			if(modulename!=NULL) Rfree(modulename);
			return;
		}
	} else {
		prterr("Error ComputeRlstMgtSubData [%s] not found.",*tmp);
		if(*tmp!=NULL) Rfree(*tmp);
		*tmp=stralloc("\"\"");
		if(diagvirtual || diageval)
		{
			prterr("DIAG ComputeRlstMgtSubData Returning [%s].",*tmp);
		}
		if(modulename!=NULL) Rfree(modulename);
	}
}
static void quit_print(RDArsrc *prsrc)
{
	if(diaggui)
	{
		prterr("DIAG quit_print Quitting Print Screen Definition.");
	}
	if(prsrc!=NULL) free_rsrc(prsrc);
}

static void printerrorlist(RDArsrc *parent,RDA_PFILE *fp)
{
	int x;
	char *date=NULL,*timex=NULL;

	if(fp!=NULL)
	{
		date=GETCURRENTDATE();
		timex=GETCURRENTTIME();
		RDA_fprintf(fp,"                     MBGUI COMPUTE TAX ERROR LIST          Date: %*s\n",8,date);
			RDA_fprintf(fp,"%*sExecuted By: %s%*sTime: %s\n\n",
			 ((80-(14+RDAstrlen(USERLOGIN)))/2),
			"",USERLOGIN,
			(((80-(14+RDAstrlen(USERLOGIN)))/2)-14),"",
			timex);
		if(date!=NULL) Rfree(date);
		if(timex!=NULL) Rfree(timex);
		for(x=0;x<errorlist->numlibs;++x)
		{
			RDA_fprintf(fp,"(%5d) %s\n",(x+1),errorlist->libs[x]);
		}
		RDA_fprintf(fp,"\f");
	}
}

void printerrorlistcb(RDArsrc *prsrc,RDArsrc *parent)
{
	char *outdevice=NULL,*message=NULL;
	RDA_PFILE *fp=NULL;

	readwidget(prsrc,"DEVICE");
	FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
	if(outdevice!=NULL)
	{
		fp=RDA_popen(outdevice,"w");
		if(fp!=NULL)
		{
			printerrorlist(parent,fp);
			RDA_pclose(fp);
		} else {
			message=Rmalloc(RDAstrlen(outdevice)+69);
			sprintf(message,"The output attemp to the DEVICE [%s] failed when trying to open pipe.",outdevice);
			WARNINGDIALOG("WARNING DIALOG SCREEN","DEVICE COMMUNICATIONS FAILED!",message,NULL,NULL,TRUE,NULL);
			prterr(message);
			if(message!=NULL) Rfree(message);
		}
		if(outdevice!=NULL) Rfree(outdevice);
	} else {
		ERRORDIALOG("Output Device Failure","There was no output device specified.",NULL,FALSE);
	}
}

void print_list(RDArsrc *parent,void (*printfunc)(...))
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL;

	if(diaggui)
	{
		prterr("DIAG printerrorlist Displaying the Print Screen in preparation to print Error List.");
	}
	prsrc=RDArsrcNEW("GUI","PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printfunc,parent);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_print,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PRINT SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		quit_print(prsrc);
	}
}
static void quitfunc(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	if(rstmst!=(-1)) CLSNRD(rstmst);
	if(rstcml!=(-1)) CLSNRD(rstcml);
	if(rstmexp!=(-1)) CLSNRD(rstmexp);
	if(rstmmexp!=(-1)) CLSNRD(rstmmexp);
	if(rstexp!=(-1)) CLSNRD(rstexp);
	if(rstcls!=(-1)) CLSNRD(rstcls);
	if(rstran!=(-1)) CLSNRD(rstran);
	if(rstmil!=(-1)) CLSNRD(rstmil);
	if(finracc!=(-1)) CLSNRD(finracc);
	if(finryr!=(-1)) CLSNRD(finryr);
	if(finbacc!=(-1)) CLSNRD(finbacc);
	if(finbyr!=(-1)) CLSNRD(finbyr);
	if(owner!=(-1)) CLSNRD(owner);
	if((errorlist->numlibs!=1) || RDAstrcmp(errorlist->libs[0],"No Errors Detected"))
	{
		APPlib2SupportRequest("RLSTMGT","comprlstbkld -> Compute Real Estate Taxes",errorlist);
	}
	if(errorlist!=NULL) freeapplib(errorlist);
	if(COMPUTE_DATE!=NULL) Rfree(COMPUTE_DATE);
	ShutdownSubsystems();
}
static void comptax(RDArsrc *mainrsrc)
{
	char *e=NULL,delflag=FALSE;
	char *realid=NULL,*realid1=NULL;
	char *ownid=NULL,*ownid1=NULL;
	char *classid=NULL,*classid1=NULL,*classid2=NULL;
	char *curdate=NULL,*curtime=NULL;
	short transtype=0;
	char tax_booln=FALSE;
	int tax_year1=0,taxyear=0;
	short ef=0,ef1=0,ef2=0,doit=FALSE,exempt_type=0;
	RangeScreen *rs=NULL;
	int x=0;
	RDAvirtual *v=NULL;
	RDATData *prev=NULL;
	char DID_STUFF=FALSE,use_exemptions=FALSE,active=FALSE;
	char *val_exp=NULL,*rate_exp=NULL,*exempt_exp=NULL;
	double est_val=0.0,act_val=0.0,amt=0.0,tax_amt=0.0;
	char use_units=FALSE,rwc=FALSE,defrev=FALSE,all_classes=FALSE,all_rates=FALSE,dit=FALSE;
	char *millage=NULL,*millage1=NULL;
	short acctype=2,deftype=0;
	char *acct=NULL,*revcode=NULL,*defcode=NULL,*reccode=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETSTRING(mainrsrc,"DUE DATE",&COMPUTE_DATE);
	FINDRSCGETINT(mainrsrc,"TAX YEAR",&tax_year);
	FINDRSCGETSHORT(mainrsrc,"FISCAL MONTH",&fiscal_month);
	if(XPERT_SETUP->software_type<2)
	{
		FINDRSCGETINT(mainrsrc,"FISCAL YEAR",&fiscalyear);
	}
	rs=RangeScreenNew(mainrsrc);
	ReadRangeScreen(rstmst,rs);
	ReadRangeScreen(rstcml,rs);
	ReadRangeScreen(rstcls,rs);
	if(mainrsrc->virflds!=NULL)
	{
		for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
		{
			ReadVirtualRangeStruct(v,rs);
		}
	}
	ZERNRD(rstmst);
	diagrsrc=diagscrn(rstmst,"DIAGNOSTIC SCREEN",module,"Selecting Real Estate Masters",stop_diagnostic);
	addDFincvir(diagrsrc,module,"RESTMSTR",NULL,rstmst);
	addDFincvir(diagrsrc,module,"RESTCLML",NULL,rstcml);
	addDFincvir(diagrsrc,module,"RESTMEXP",NULL,rstmexp);
	addDFincvir(diagrsrc,module,"RESTMIL",NULL,rstmil);
	addDFincvir(diagrsrc,module,"RESTEXMP",NULL,rstexp);
	addDFincvir(diagrsrc,module,"RESTCLAS",NULL,rstcls);
	if(XPERT_SETUP->software_type<2)
	{
		addDFincvir(diagrsrc,"FINMGT","FINRYR",NULL,finryr);
		addDFincvir(diagrsrc,"FINMGT","FINRACC",NULL,finracc);
		addDFincvir(diagrsrc,"FINMGT","FINBACC",NULL,finbacc);
		addDFincvir(diagrsrc,"FINMGT","FINBYR",NULL,finbyr);
	}
	GET_ALL_SCREEN_VIRTUALS(diagrsrc,0);
	curdate=GETCURRENTDATE10();
	if(diagrsrc!=NULL)
	{
		if(!ADVmakescrn(diagrsrc,TRUE))
		{
			ForceWindowUpdate(diagrsrc);
		} else {
			e=Rmalloc(130+RDAstrlen(diagrsrc->module)+RDAstrlen(diagrsrc->screen));
			sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",diagrsrc->module,diagrsrc->screen);
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
			prterr("Error %s.",e);
			if(e!=NULL) Rfree(e);
		}
	}
	ef=ADVBEGNRDsec(rstmst,ComputeRlstMgtSubData,diagrsrc);
	while(!ef)
	{
		if(abort_diagnostic) break;
		DID_STUFF=FALSE;
		FINDFLDGETCHAR(rstmst,"DELETEFLAG",&delflag);
		FINDFLDGETINT(rstmst,"TAX YEAR",&taxyear);
		tax_value=0;
		tax_net_value=0;
		tax_amt=0.0;
		if(!delflag && taxyear==tax_year)
		{
			if(!testfilerangersrc(rstmst,rs))
			{
				doit=FALSE;
				if(mainrsrc->virflds!=NULL)
				{
					for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
					{
						if(ADVtestvirtualrangersrc(v,rs,ComputeRlstMgtSubData,mainrsrc)) break;
					}
					if(x>=mainrsrc->num)
					{
						doit=TRUE;
					} else {
						doit=FALSE;
					}
				} else doit=TRUE;
				if(doit==TRUE)
				{
					if(curtime!=NULL) Rfree(curtime);
					curtime=GETCURRENTTIME();
					FINDFLDGETSTRING(rstmst,"CLASS IDENTIFICATION",&classid);
					ZERNRD(rstcml);
					FINDFLDSETSTRING(rstcml,"CLASS IDENTIFICATION",classid);
					ef1=ADVGTENRDsec(rstcml,1,ComputeRlstMgtSubData,diagrsrc);
					while(!ef1)
					{
						tax_net_value=0;
						FINDFLDGETSTRING(rstcml,"CLASS IDENTIFICATION",&classid1);
						if(RDAstrcmp(classid,classid1)) break;
						COPYFIELD(rstcml,rstmil,"MILLAGE IDENTIFICATION");
						COPYFIELD(rstmst,rstmil,"OWNER IDENTIFICATION");
						COPYFIELD(rstmst,rstmil,"REAL ESTATE IDENTIFICATION");
						COPYFIELD(rstmst,rstmil,"TAX YEAR");
						if(ADVEQLNRDsec(rstmil,1,ComputeRlstMgtSubData,diagrsrc)) KEYNRD(rstmil,1);
						COPYFIELD(rstcml,rstcls,"CLASS IDENTIFICATION");
						if(ADVEQLNRDsec(rstcls,1,ComputeRlstMgtSubData,diagrsrc)) KEYNRD(rstcls,1);
						FINDFLDGETCHAR(rstcml,"DELETEFLAG",&delflag);
						FINDFLDGETCHAR(rstcml,"USE UNITS",&use_units);
						FINDFLDGETSTRING(rstcml,"MILLAGE IDENTIFICATION",&millage);
						FINDFLDGETCHAR(rstcml,"ACTIVE",&active);
						if(!delflag && !use_units && active && !testfilerangersrc(rstcml,rs) && !testfilerangersrc(rstcls,rs))
						{
							FINDFLDGETSTRING(rstmst,"REAL ESTATE IDENTIFICATION",&realid);
							FINDFLDGETSTRING(rstmst,"OWNER IDENTIFICATION",&ownid);
							FINDFLDGETCHAR(rstcml,"EXEMPTIONS",&use_exemptions);
							FINDFLDGETCHAR(rstcml,"TAX",&tax_booln);
							transtype=0;
							if(tax_booln==TRUE)
							{
								transtype=0;
							} else {
								transtype=4;
							}
							tot_val_exmp=0.0;
							tot_tax_exmp=0.0;
							FINDFLDGETSTRING(rstcml,"RATE EXPRESSION",&rate_exp);
							if(use_exemptions==TRUE)
							{
								FINDFLDGETSTRING(rstcml,"TAXABLE EXPRESSION",&val_exp);
							}
							if(use_exemptions==TRUE && !isEMPTY(val_exp))
							{
								tax_value=(round(EVALUATEdbl(val_exp,ComputeRlstMgtSubData,diagrsrc)));
								if(tax_value<0) tax_value=0;
							} else {
								tax_value=0;
							}
							tax_net_value=tax_value;
							if(use_exemptions==TRUE)
							{
								ZERNRD(rstmexp);
								FINDFLDSETSTRING(rstmexp,"REAL ESTATE IDENTIFICATION",realid);
								FINDFLDSETSTRING(rstmexp,"OWNER IDENTIFICATION",ownid);
								FINDFLDSETINT(rstmexp,"TAX YEAR",tax_year);
								ef2=ADVGTENRDsec(rstmexp,1,ComputeRlstMgtSubData,diagrsrc);
								while(!ef2)
								{
									FINDFLDGETSTRING(rstmexp,"REAL ESTATE IDENTIFICATION",&realid1);
									FINDFLDGETSTRING(rstmexp,"OWNER IDENTIFICATION",&ownid1);
									FINDFLDGETINT(rstmexp,"TAX YEAR",&tax_year1);
									if(RDAstrcmp(realid,realid1) ||
										RDAstrcmp(ownid,ownid1) ||
										tax_year!=tax_year1) break;
									ZERNRD(rstexp);
									FINDFLDGETCHAR(rstmexp,"DELETEFLAG",&delflag);
									if(!delflag)
									{
										COPYFIELD(rstmexp,rstexp,"EXEMPTION TYPE");
										if(!ADVEQLNRDsec(rstexp,1,ComputeRlstMgtSubData,diagrsrc))
										{
											FINDFLDGETCHAR(rstexp,"DELETEFLAG",&delflag);
											FINDFLDGETCHAR(rstexp,"ACTIVE",&active);
											FINDFLDGETCHAR(rstexp,"ALL CLASSES",&all_classes);
											FINDFLDGETCHAR(rstexp,"ALL MILLAGES",&all_rates);
											FINDFLDGETSTRING(rstexp,"MILLAGE IDENTIFICATION",&millage1);
											FINDFLDGETSTRING(rstexp,"CLASS IDENTIFICATION",&classid2);
											dit=FALSE;
											if(all_classes && all_rates) 
											{
												dit=TRUE;
											} else if(all_classes && !all_rates)
											{
												if(!RDAstrcmp(millage,millage1)) dit=TRUE;
											} else if(!all_classes && all_rates)
											{
												if(!RDAstrcmp(classid,classid2)) dit=TRUE;
											} else if(!all_classes && !all_rates)
											{
												if(!RDAstrcmp(classid,classid2) && !RDAstrcmp(millage,millage1)) dit=TRUE;
											}
											if(classid2!=NULL) Rfree(classid2);
											if(millage1!=NULL) Rfree(millage1);
											if(!delflag && active==TRUE && dit==TRUE)
											{

												FINDFLDGETSHORT(rstexp,"TYPE",&exempt_type);
												FINDFLDGETSTRING(rstexp,"EXPRESSION",&exempt_exp);
												FINDFLDGETDOUBLE(rstexp,"AMOUNT",&amt);
												switch(exempt_type)
												{
													default:
													case 0:
														tot_val_exmp+=amt;
														break;
													case 1:
														amt=EVALUATEdbl(exempt_exp,ComputeRlstMgtSubData,diagrsrc);
														tot_val_exmp+=amt;
														break;
													case 2:
														tot_tax_exmp+=amt;
														break;
													case 3:
														amt=EVALUATEdbl(exempt_exp,ComputeRlstMgtSubData,diagrsrc);
														tot_tax_exmp+=amt;
														break;
												}
												if(use_exemption_detail==TRUE)
												{
													tax_net_value=tax_net_value-(tot_val_exmp/100);
													if(tax_net_value<0) tax_net_value=0;
													ZERNRD(rstmmexp);
													FINDFLDSETSTRING(rstmmexp,"REAL ESTATE IDENTIFICATION",realid);
													FINDFLDSETSTRING(rstmmexp,"OWNER IDENTIFICATION",ownid);
													FINDFLDSETINT(rstmmexp,"TAX YEAR",tax_year);
													COPYFIELD(rstcml,rstmmexp,"MILLAGE IDENTIFICATION");
													COPYFIELD(rstmexp,rstmmexp,"EXEMPTION TYPE");
													if(!ADVEQLNRDsec(rstmmexp,1,ComputeRlstMgtSubData,diagrsrc))
													{
														prev=RDATDataNEW(rstmmexp);
														FINDFLDSETCHAR(rstmmexp,"DELETEFLAG",FALSE);
													} else {
														KEYNRD(rstmmexp,1);
														prev=RDATDataNEW(rstmmexp);
													}
													FINDFLDSETDOUBLE(rstmmexp,"EXEMPTION AMOUNT",amt);
													FINDFLDSETSHORT(rstmmexp,"TYPE",exempt_type);
													FINDFLDSETDOUBLE(rstmmexp,"ORIGINAL VALUE",(tax_value*100));
													FINDFLDSETDOUBLE(rstmmexp,"NET VALUE",(tax_net_value*100));
													COPYFIELD(rstcml,rstmmexp,"MILLAGE RATE");
													FINDFLDSETSHORT(rstmmexp,"TRANSACTION TYPE",transtype);
													ADVWRTTRANSsec(rstmmexp,0,NULL,prev,ComputeRlstMgtSubData,diagrsrc);	
													if(prev!=NULL) FreeRDATData(prev);	
												}
											}
										}
									}
									ef2=ADVNXTNRDsec(rstmexp,1,ComputeRlstMgtSubData,diagrsrc);
								}
								if(exempt_exp!=NULL) Rfree(exempt_exp);
								if(realid1!=NULL) Rfree(realid1);
								if(ownid1!=NULL) Rfree(ownid1);
							}
						}
						if(use_exemptions==TRUE && !isEMPTY(val_exp))
						{
							tax_value=tax_value-(tot_val_exmp/100);
							if(tax_value<0) tax_value=0;
						}
						if(!isEMPTY(rate_exp))
						{
							tax_amt=(round(EVALUATEdbl(rate_exp,ComputeRlstMgtSubData,diagrsrc)))-tot_tax_exmp;
						} else {
							tax_amt=0.0;
						}
						if(tax_amt!=0.0)
						{
/* WRITE TO DATABASES */
							ZERNRD(rstran);
							FINDFLDSETSTRING(rstran,"REAL ESTATE IDENTIFICATION",realid);
							FINDFLDSETSTRING(rstran,"OWNER IDENTIFICATION",ownid);
							FINDFLDSETINT(rstran,"TAX YEAR",tax_year);
							FINDFLDGETCHAR(rstcml,"REALIZE WHEN CHARGED",&rwc);
							FINDFLDGETCHAR(rstcml,"DEFERRED REVENUE",&defrev);
/* default account type to typical value for completeness */
							FINDFLDSETSHORT(rstran,"ACCOUNT TYPE",BAL_ACCT);
							FINDFLDGETSHORT(rstcml,"DEFAULT DEFINITION TYPE",&deftype);
							if(rwc==TRUE)
							{
								FINDFLDGETSHORT(rstcml,"DEFAULT ACCOUNT TYPE",&acctype);
								FINDFLDSETSHORT(rstran,"ACCOUNT TYPE",acctype);
								FINDFLDSETSHORT(rstran,"DEFINITION TYPE",deftype);
								if(acctype==1)
								{
									FINDFLDGETSTRING(rstcml,"DEFAULT ACCOUNT CODE",&acct);
									FINDFLDSETSTRING(rstran,"ACCOUNT CODE",acct);
								}
								FINDFLDGETSTRING(rstcml,"DEFAULT RECEIVABLE CODE",&reccode);
								FINDFLDGETSTRING(rstcml,"DEFAULT REVENUE CODE",&revcode);
								FINDFLDSETSTRING(rstran,"CASH CODE",reccode);
								FINDFLDSETSTRING(rstran,"REVENUE CODE",revcode);
								FINDFLDSETCHAR(rstran,"REALIZE REVENUE",rwc);
							}
							if(defrev==TRUE)
							{
								FINDFLDSETSHORT(rstran,"DEFINITION TYPE",deftype);
								FINDFLDGETSTRING(rstcml,"DEFAULT DEFERRED REVENUE",&defcode);
								FINDFLDGETSTRING(rstcml,"DEFAULT RECEIVABLE CODE",&reccode);
								FINDFLDSETSTRING(rstran,"DEFERRED REVENUE",defcode);
								FINDFLDSETSTRING(rstran,"RECEIVABLE CODE",reccode);
								FINDFLDSETCHAR(rstran,"USE DEFERRED REVENUE",defrev);
							}

							FINDFLDGETDOUBLE(rstmst,"LAND VALUE",&amt);
							act_val=amt;
							FINDFLDGETDOUBLE(rstmst,"BUILDING AND IMPROVEMENT VALUE",&amt);
							act_val+=amt;
							FINDFLDGETDOUBLE(rstmst,"ESTIMATED LAND VALUE",&amt);
							est_val=amt;
							FINDFLDGETDOUBLE(rstmst,"ESTIMATED BUILDING AND IMPROVEMENT VALUE",&amt);
							est_val+=amt;
							FINDFLDSETDOUBLE(rstran,"VALUE",act_val);
							FINDFLDSETSTRING(rstran,"ENTRY DATE",curdate);
							FINDFLDSETSTRING(rstran,"ENTRY TIME",curtime);
							FINDFLDSETDOUBLE(rstran,"ESTIMATED VALUE",est_val);
							FINDFLDSETDOUBLE(rstran,"TAXABLE",tax_value);
							FINDFLDSETDOUBLE(rstran,"AMOUNT",tax_amt);
							FINDFLDSETDOUBLE(rstran,"OUTSTANDING AMOUNT",tax_amt);
							FINDFLDSETSHORT(rstran,"TRANSACTION TYPE",transtype);
							FIELDCOPY(rstcml,"MILLAGE IDENTIFICATION",rstran,"REFERENCE IDENTIFICATION");
							FIELDCOPY(rstcml,"NAME",rstran,"DESCRIPTION");
							FINDFLDGETCHAR(rstcml,"REALIZE WHEN CHARGED",&delflag);
							FINDFLDSETCHAR(rstran,"REALIZE REVENUE",delflag);
							FINDFLDSETSHORT(rstran,"FISCAL MONTH",fiscal_month);
							FINDFLDSETSTRING(rstran,"DUE DATE",COMPUTE_DATE);
							FINDFLDSETSTRING(rstran,"DATE INTEREST COMPUTED",COMPUTE_DATE);
							FINDFLDSETSTRING(rstran,"SOURCE USER",USERLOGIN);
							FINDFLDSETINT(rstran,"FISCAL YEAR",fiscalyear);
							COPYFIELD(rstmst,rstran,"SUPPLEMENT");
							DID_STUFF=TRUE;
							ZERNRD(rstmil);
							FINDFLDSETSTRING(rstmil,"REAL ESTATE IDENTIFICATION",realid);
							FINDFLDSETSTRING(rstmil,"OWNER IDENTIFICATION",ownid);
							FIELDCOPY(rstran,"REFERENCE IDENTIFICATION",rstmil,"MILLAGE IDENTIFICATION");
							FINDFLDSETINT(rstmil,"TAX YEAR",tax_year);
							LOCNRDFILE(rstmil);
							if(ADVEQLNRDsec(rstmil,1,ComputeRlstMgtSubData,diagrsrc))
							{
								KEYNRD(rstmil,1);
							}
							prev=RDATDataNEW(rstmil);
							FINDFLDGETDOUBLE(rstmil,"AMOUNT",&amt);
							amt+=tax_amt;
							FINDFLDSETDOUBLE(rstmil,"AMOUNT",amt);
							ADVWRTTRANSsec(rstmil,0,NULL,prev,ComputeRlstMgtSubData,diagrsrc);	
							if(prev!=NULL) FreeRDATData(prev);	
							UNLNRDFILE(rstmil);
							if((XPERT_SETUP->software_type<2) && (USE_COMMON_OWNER))
							{
								ZERNRD(owner);
								FINDFLDSETSTRING(owner,"OWNER IDENTIFICATION",ownid);
								LOCNRDFILE(owner);
								if(ADVEQLNRDsec(owner,1,ComputeRlstMgtSubData,diagrsrc))
								{
								} else {
									prev=RDATDataNEW(owner);
									FINDFLDGETDOUBLE(owner,"RLSTMGT OUTSTANDING AMOUNT",&amt);
									amt+=tax_amt;
									FINDFLDSETDOUBLE(owner,"RLSTMGT OUTSTANDING AMOUNT",amt);
									FINDFLDGETDOUBLE(owner,"OUTSTANDING AMOUNT",&amt);
									amt+=tax_amt;
									FINDFLDSETDOUBLE(owner,"OUTSTANDING AMOUNT",amt);
									ADVWRTTRANSsec(owner,0,NULL,prev,ComputeRlstMgtSubData,diagrsrc);	
									if(prev!=NULL) FreeRDATData(prev);	
								}	
								UNLNRDFILE(owner);
							}
						}
						if(acct!=NULL) Rfree(acct);
						if(defcode!=NULL) Rfree(defcode);
						if(revcode!=NULL) Rfree(revcode);
						if(reccode!=NULL) Rfree(reccode);

						if(rate_exp!=NULL) Rfree(rate_exp);
						if(val_exp!=NULL) Rfree(val_exp);
						ef1=ADVNXTNRDsec(rstcml,1,ComputeRlstMgtSubData,diagrsrc);
					}
					if(classid1!=NULL) Rfree(classid1);
					if(millage!=NULL) Rfree(millage);
				}
			}
		}
		if(diagrsrc!=NULL) update_diagnostic(diagrsrc,(DID_STUFF ? TRUE:FALSE));
		CLEAR_SCREEN_VIRTUAL(diagrsrc);
		ef=ADVSEQNRDsec(rstmst,ComputeRlstMgtSubData,diagrsrc);
	}
	if(classid!=NULL) Rfree(classid);
	if(ownid!=NULL) Rfree(ownid);
	if(realid!=NULL) Rfree(realid);
	if(curdate!=NULL) Rfree(curdate);
	if(curtime!=NULL) Rfree(curtime);
	if(diagrsrc!=NULL)
	{
		killwindow(diagrsrc);
		free_rsrc(diagrsrc);
	}
	if(rs!=NULL) FreeRangeScreen(rs);
	FINDRSCLISTAPPlib(mainrsrc,"ERROR LIST",0,errorlist);
	updatersrc(mainrsrc,"ERROR LIST");
}
static char CSHDWRDetermineCommonOwner()
{
	char *libx=NULL,d=FALSE;
	RDAGenericSetup *gsv=NULL;

	if(XPERT_SETUP->RLSTMGT==FALSE || XPERT_SETUP->PROPERTY==FALSE || XPERT_SETUP->MISCBILL==FALSE) return(FALSE);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("CSHDWR")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"CSHDWR");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"CSHDWR");
#endif

	gsv=RDAGenericSetupNew("CSHDWR","USE COMMON OWNER");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			d=*gsv->value.string_value;
		} else {
			d=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	return(d);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	int x=0;
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	if(InitializeSubsystems(argc,argv,module,"COMPUTE REAL ESTATE TAXES")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((rstmst=APPOPNNRD(module,"RESTMSTR",TRUE,TRUE))==(-1)) return;
	if((rstcml=APPOPNNRD(module,"RESTCLML",TRUE,TRUE))==(-1)) return;
	if((rstran=APPOPNNRD(module,"RESTRAN",TRUE,TRUE))==(-1)) return;
	if((rstmil=APPOPNNRD(module,"RESTMIL",TRUE,TRUE))==(-1)) return;
	if((rstmexp=APPOPNNRD(module,"RESTMEXP",TRUE,TRUE))==(-1)) return;
	if((rstmmexp=APPOPNNRD(module,"RESTMMEXP",TRUE,TRUE))==(-1)) return;
	if((rstexp=APPOPNNRD(module,"RESTEXMP",TRUE,FALSE))==(-1)) return;
	if((rstcls=APPOPNNRD(module,"RESTCLAS",TRUE,TRUE))==(-1)) return;
	if(XPERT_SETUP->software_type<2)
	{
		if((finracc=APPOPNNRD("FINMGT","FINRACC",TRUE,FALSE))==(-1)) return;
		if((finryr=APPOPNNRD("FINMGT","FINRYR",TRUE,TRUE))==(-1)) return;
		if((finbacc=APPOPNNRD("FINMGT","FINBACC",TRUE,FALSE))==(-1)) return;
		if((finbyr=APPOPNNRD("FINMGT","FINBYR",TRUE,TRUE))==(-1)) return;
		USE_COMMON_OWNER=CSHDWRDetermineCommonOwner();
		if(USE_COMMON_OWNER)
		{
			if((owner=APPOPNNRD("OWNERS","OWNER",TRUE,TRUE))==(-1)) return;
		}
	}
	mainrsrc=RDArsrcNEW(module,"COMPUTE REAL ESTATE TAXES");
	addDFincvir(mainrsrc,module,"RESTMSTR",NULL,rstmst);
	GET_SCREEN_VIRTUALS(mainrsrc,2);
	COMPUTE_DATE=stralloc(CURRENT_DATE10);
	addstdrsrc(mainrsrc,"DUE DATE",DATES,10,COMPUTE_DATE,TRUE);
	addstdrsrc(mainrsrc,"TAX YEAR",LONGV,4,&tax_year,TRUE);
	addstdrsrc(mainrsrc,"FISCAL YEAR",LONGV,4,&fiscalyear,TRUE);
	addstdrsrc(mainrsrc,"FISCAL MONTH",SHORTV,2,NULL,TRUE);
	if(rstmst!=(-1)) file2rangersrc(rstmst,mainrsrc);
	if(rstcml!=(-1)) file2rangersrc(rstcml,mainrsrc);
	if(rstcls!=(-1)) file2rangersrc(rstcls,mainrsrc);
	SCRNvirtual2rangersrc(mainrsrc);
	errorlist=APPlibNEW();
	addAPPlib(errorlist,"No Errors Detected");
	addlstrsrc(mainrsrc,"ERROR LIST",&x,TRUE,NULL,errorlist->numlibs,
		&errorlist->libs,NULL);
/*lint -e611 */
	addbtnrsrc(mainrsrc,"PRINT ERROR LIST",TRUE,print_list,(void *)printerrorlistcb);
/*lint +e611 */
	SetRangersrcsensitive(mainrsrc);
	ReadRDAScrolledLists(mainrsrc);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,comptax,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	DefaultScreens(mainrsrc);
	computeallSCRNvirtuals(mainrsrc);

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew("RLSTMGT","USE EXEMPTION DETAIL");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			use_exemption_detail=(*gsv->value.string_value==FALSE?FALSE:TRUE);
		} else {
			use_exemption_detail=FALSE;
		}
	}
	if(temp1!=NULL) Rfree(temp1);
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
	RDAAPPMAINLOOP();
}
