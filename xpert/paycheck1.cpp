/* paycheck1.c - Print Direct Deposit Transmittals */

/******************************************

Due to MicroBUDGET GUI, like it's predecessor, not being a transaction 
based payroll system combined with how payroll historical figures are 
managed (payjmst & paydedm) and that deductions are by job as well, a hard-coded
program to manage this is the simplest fix.  Did we really need this?

******************************************/
#ifndef WIN32
#define __NAM__ "paycheck1.lnx"
#endif
#ifdef WIN32
#define __NAM__ "paycheck1.exe"
#endif
#include <app.hpp>
#include <mix.hpp>
#include <fin.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static char *module="PAYROLL";
static char display_ssn=FALSE;
static short perdmg=(-1),payjmst=(-1),paydedm=(-1),payddms=(-1),posmst=(-1);
static short bnkacc=(-1),poslmst=(-1),lvemst=(-1),lvedsc=(-1),paydtyp=(-1);
static short payppms=(-1),payjpms=(-1),payjprm=(-1),payjdpm=(-1);
static short payjdcm=(-1),payjcsm=(-1),paymst=(-1),payjrcs=(-1);
static short posrate=(-1),paydcsm=(-1);
static RDArsrc *diagrsrc=NULL;
static char *message1=NULL,*message2=NULL,*payending=NULL;
static char check_formfeed=FALSE,top_feeder=FALSE,use_sign_locks=FALSE;
static char skip_leave=FALSE,print_employer_deductions=FALSE;
static short pay_check_offset=0,spelled_pos=0;
static char *bankno=NULL,*payid=NULL;
static char *defbankid=NULL;
static short DID_STUFF=0,check_type=0,check_signature=0;
static short ADDRESS_OFFSET=0;
static DBsort *sortfile=NULL;
static RDAvirtual *EmployeeName=NULL;
static RDAvirtual *EmployeeAddr1=NULL,*EmployeeAddr2=NULL,*EmployeeAddr3=NULL;
static RDAvirtual *OrgAddr1=NULL,*OrgAddr2=NULL,*OrgAddr3=NULL;
static char *outdevice=NULL;

#define QFld(a,b,c)	QAccFld(a,c,b,c)
static void QAccFld(short tfileno,char *tfieldname,short ffileno,char *ffieldname)
{
	double amt=0.0,camt=0.0;

	FINDFLDGETDOUBLE(tfileno,tfieldname,&camt);
	FINDFLDGETDOUBLE(ffileno,ffieldname,&amt);
	camt+=amt;
	FINDFLDSETDOUBLE(tfileno,tfieldname,camt);
}
static char UpdatedAlready(char *perid,int job)
{
	char *payid1=NULL,*perid1=NULL,v=FALSE,delflag=FALSE;
	int job1=0;
	short ef=0;

	ZERNRD(payjprm);
	FINDFLDSETSTRING(payjprm,"PAY IDENTIFICATION",payid);
	FINDFLDSETSTRING(payjprm,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(payjprm,"JOB NUMBER",job);
	ef=ADVGTENRDsec(payjprm,1,SCRNvirtualSubData,diagrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(payjprm,"PAY IDENTIFICATION",&payid1);
		FINDFLDGETSTRING(payjprm,"PERSONNEL IDENTIFICATION",&perid1);
		FINDFLDGETINT(payjprm,"JOB NUMBER",&job1);
		if(job!=job1 || RDAstrcmp(payid,payid1) || RDAstrcmp(perid,perid1)) break;
		FINDFLDGETCHAR(payjprm,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			FINDFLDGETCHAR(payjprm,"UPDATED",&v);
			if(v==TRUE) break;
		}
		ef=ADVNXTNRDsec(payjprm,1,SCRNvirtualSubData,diagrsrc);
	}
	if(payid1!=NULL) Rfree(payid1);
	if(perid1!=NULL) Rfree(perid1);
	return(v);
}
static void quitfunc(RDArsrc *r)
{
	if(r!=NULL) 
	{
		killwindow(r);
		free_rsrc(r);
	}
	if(payppms!=(-1)) CLSNRD(payppms);
	if(payjpms!=(-1)) CLSNRD(payjpms);
	if(payjdpm!=(-1)) CLSNRD(payjdpm);
	if(payjdcm!=(-1)) CLSNRD(payjdcm);
	if(paydcsm!=(-1)) CLSNRD(paydcsm);
	if(payjcsm!=(-1)) CLSNRD(payjcsm);
	if(payjrcs!=(-1)) CLSNRD(payjrcs);
	if(perdmg!=(-1)) CLSNRD(perdmg);
	if(payjmst!=(-1)) CLSNRD(payjmst);
	if(paydedm!=(-1)) CLSNRD(paydedm);
	if(payddms!=(-1)) CLSNRD(payddms);
	if(paydtyp!=(-1)) CLSNRD(paydtyp);
	if(posmst!=(-1)) CLSNRD(posmst);
	if(posrate!=(-1)) CLSNRD(posrate);
	if(poslmst!=(-1)) CLSNRD(poslmst);
	if(bnkacc!=(-1)) CLSNRD(bnkacc);
	if(lvemst!=(-1)) CLSNRD(lvemst);
	if(lvedsc!=(-1)) CLSNRD(lvedsc);
	if(message1!=NULL) Rfree(message1);
	if(message2!=NULL) Rfree(message2);
	if(payending!=NULL) Rfree(payending);
	if(defbankid!=NULL) Rfree(defbankid);
	if(bankno!=NULL) Rfree(bankno);
	FreeRDAvirtual(EmployeeName);
	FreeRDAvirtual(EmployeeAddr1);
	FreeRDAvirtual(EmployeeAddr2);
	FreeRDAvirtual(EmployeeAddr3);
	FreeRDAvirtual(OrgAddr1);
	FreeRDAvirtual(OrgAddr2);
	FreeRDAvirtual(OrgAddr3);

	ShutdownSubsystems();
	exit(0);
}
static void quitprint(RDArsrc *r,RDArsrc *mainrsrc)
{
	if(r!=NULL)
	{
		killwindow(r);
		free_rsrc(r);
	}
	if(sortfile!=NULL)
	{
		CLSDBsort(sortfile);
		FreeDBsort(sortfile);
		sortfile=NULL;
	}
	quitfunc(mainrsrc);
}
static void alignment(RDArsrc *r)
{
	RDA_PFILE *fp=NULL;

	readwidget(r,"OUTPUT DEVICE");
	FINDRSCGETSTRING(r,"OUTPUT DEVICE",&outdevice);
	if(!isEMPTY(outdevice))
	{
		fp=RDA_popen(outdevice,"w");
		SelectPrintTypeByName(outdevice,"PCL JOB HEADER",fp);
		SelectPrintType(outdevice,0,fp);
		if(fp!=NULL)
		{
			RDA_fprintf(fp,"       XXX\r\n");
			SelectPrintTypeByName(outdevice,"PCL JOB FOOTER",fp);
			RDA_pclose(fp);
		} else {
			prterr("Error:  Couldn't open output device [%s].",outdevice);
		}	
	}
}
/*
static char *decimal(double x)
{
	char *tmp=NULL;

	sprintf(stemp,"%.02f",x);
*/
/*
	y=RDAstrlen(stemp);
	if(stemp[y-1]=='0')
	{
		if(stemp[y-2]=='0' && stemp[y-3]=='.')  stemp[y-3]=0;
			else stemp[y-1]=0;
	}
*/
/*
	if(!isEMPTY(stemp)) tmp=stralloc(stemp);
	return(tmp);
}
*/
static void GatherDeductions()
{
	char *empid1=NULL,deleteflag=FALSE,*empid=NULL,bfs=FALSE;
	char *type2=NULL;
	char reduce_net=FALSE,lb=FALSE,ep=FALSE,pay_with=FALSE,electtrans=FALSE;
	char *typ1=NULL,*typ2=NULL,*typ3=NULL,*typ4=NULL,*typ5=NULL,*typ6=NULL;
	char *typ7=NULL,*typ8=NULL;
	char *title=NULL,*code2=NULL;
	short ef=0,y=0;
	int cyear=0,cyear1=0,job=0;

	FINDFLDGETINT(paymst,"CALENDAR YEAR",&cyear);
	ZERNRD(paydcsm);
	FINDFLDGETSTRING(sortfile->fileno,"PERSONNEL IDENTIFICATION",&empid);
	FINDFLDSETSTRING(paydcsm,"PERSONNEL IDENTIFICATION",empid);
	FINDFLDSETINT(paydcsm,"CALENDAR YEAR",cyear);
	ef=ADVGTENRDsec(paydcsm,4,SCRNvirtualSubData,diagrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(paydcsm,"PERSONNEL IDENTIFICATION",&empid1);
		FINDFLDGETINT(paydcsm,"CALENDAR YEAR",&cyear1);
		if(RDAstrcmp(empid,empid1) || cyear!=cyear1) break;
		FINDFLDGETCHAR(paydcsm,"DELETEFLAG",&deleteflag);
		FINDFLDGETINT(payjdcm,"JOB NUMBER",&job);
		if(!UpdatedAlready(empid,job))
		{
		if(!deleteflag)
		{
			ZERNRD(paydedm);
			COPYFIELD(paydcsm,paydedm,"PERSONNEL IDENTIFICATION");
			COPYFIELD(paydcsm,paydedm,"DEDUCTION DESCRIPTION IDENTIFICATION");
			if(ADVEQLNRDsec(paydedm,1,SCRNvirtualSubData,diagrsrc)) KEYNRD(paydedm,1);
			
			COPYFIELD(paydcsm,payddms,"DEDUCTION DESCRIPTION IDENTIFICATION");
			if(ADVEQLNRDsec(payddms,1,SCRNvirtualSubData,diagrsrc))
			{
				KEYNRD(payddms,1);
			}
			FINDFLDGETCHAR(payddms,"REDUCE NET",&reduce_net);
			FINDFLDGETCHAR(payddms,"LIABILITY",&lb);
			FINDFLDGETCHAR(payddms,"EXPENSED",&ep);
			FINDFLDGETCHAR(payddms,"PRINT CHECK",&pay_with);
			FINDFLDGETCHAR(payddms,"ELECTRONIC TRANSFER",&electtrans);
			FINDFLDGETSTRING(payddms,"SHORT DESCRIPTION",&title);
			FINDFLDGETCHAR(payddms,"BLOCK FROM STUB",&bfs);
			if(bfs==FALSE)
			{
				FINDFLDGETSTRING(payddms,"DEDUCTION TYPE",&type2);
			if((reduce_net==TRUE) || (!RDAstrcmp(type2,"STATE RETIRE")))
			/* if(reduce_net==TRUE) */
			{
				FINDFLDGETSTRING(payddms,"DEDUCTION TYPE",&type2);
				FINDFLDGETSTRING(payddms,"DEDUCTION DESCRIPTION IDENTIFICATION",&code2);
				if(!RDAstrcmp(type2,"FEDERAL"))
				{
					for(y=1;y<=12;++y)
					{
						memset(stemp,0,101);
						sprintf(stemp,"%s AMOUNT",FISCALPERIOD[y]);
						QAccFld(sortfile->fileno,"FEDERAL W/H YTD",paydcsm,stemp);
					}
				} else if(!RDAstrcmp(type2,"FICA"))
				{
					for(y=1;y<=12;++y)
					{
						memset(stemp,0,101);
						sprintf(stemp,"%s AMOUNT",FISCALPERIOD[y]);
						QAccFld(sortfile->fileno,"FICA W/H YTD",paydcsm,stemp);
					}
				} else if(!RDAstrcmp(type2,"MEDICARE"))
				{
					for(y=1;y<=12;++y)
					{
						memset(stemp,0,101);
						sprintf(stemp,"%s AMOUNT",FISCALPERIOD[y]);
						QAccFld(sortfile->fileno,"MEDICARE W/H YTD",paydcsm,stemp);
					}
				} else if(!RDAstrcmp(type2,"STATE"))
				{
					for(y=1;y<=12;++y)
					{
						memset(stemp,0,101);
						sprintf(stemp,"%s AMOUNT",FISCALPERIOD[y]);
						QAccFld(sortfile->fileno,"STATE W/H YTD",paydcsm,stemp);
					}
				} else if(!RDAstrcmp(type2,"LOCAL"))
				{
					for(y=1;y<=12;++y)
					{
						memset(stemp,0,101);
						sprintf(stemp,"%s AMOUNT",FISCALPERIOD[y]);
						QAccFld(sortfile->fileno,"LOCAL W/H YTD",paydcsm,stemp);
					}
				} else if(!RDAstrcmp(type2,"STATE RETIRE"))
				{
					for(y=1;y<=12;++y)
					{
						memset(stemp,0,101);
						sprintf(stemp,"%s AMOUNT",FISCALPERIOD[y]);
						QAccFld(sortfile->fileno,"STATE RETIREMENT W/H YTD",paydcsm,stemp);
					}
				} else if(!RDAstrcmp(type2,"RETIREMENT-M"))
				{
					for(y=1;y<=12;++y)
					{
						memset(stemp,0,101);
						sprintf(stemp,"%s AMOUNT",FISCALPERIOD[y]);
						QAccFld(sortfile->fileno,"RETIREMENT W/H YTD",paydcsm,stemp);
					}
#ifdef XXXX
				} else {
					FINDFLDGETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 1",&typ1);
					FINDFLDGETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 2",&typ2);
					FINDFLDGETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 3",&typ3);
					FINDFLDGETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 4",&typ4);
					FINDFLDGETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 5",&typ5);
					FINDFLDGETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 6",&typ6);
					FINDFLDGETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 7",&typ7);
					FINDFLDGETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 8",&typ8);
					if(!RDAstrcmp(typ1,code2) && !isEMPTY(typ1))
					{
						for(y=1;y<=12;++y)
						{
							memset(stemp,0,101);
							sprintf(stemp,"%s AMOUNT",FISCALPERIOD[y]);
							QAccFld(sortfile->fileno,"VOL W/H YTD 1",paydcsm,stemp);
						}
					} else if(isEMPTY(typ1))
					{
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 1",type2);
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION TITLE 1",title);
						for(y=1;y<=12;++y)
						{
							memset(stemp,0,101);
							sprintf(stemp,"%s AMOUNT",FISCALPERIOD[y]);
							QAccFld(sortfile->fileno,"VOL W/H YTD 1",paydcsm,stemp);
						}
					} else if(!RDAstrcmp(typ2,code2) && !isEMPTY(typ2))
					{
						for(y=1;y<=12;++y)
						{
							memset(stemp,0,101);
							sprintf(stemp,"%s AMOUNT",FISCALPERIOD[y]);
							QAccFld(sortfile->fileno,"VOL W/H YTD 2",paydcsm,stemp);
						}
					} else if(isEMPTY(typ2))
					{
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 2",code2);
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION TITLE 2",title);
						for(y=1;y<=12;++y)
						{
							memset(stemp,0,101);
							sprintf(stemp,"%s AMOUNT",FISCALPERIOD[y]);
							QAccFld(sortfile->fileno,"VOL W/H YTD 2",paydcsm,stemp);
						}
					} else if(!RDAstrcmp(typ3,code2) && !isEMPTY(typ3))
					{
						for(y=1;y<=12;++y)
						{
							memset(stemp,0,101);
							sprintf(stemp,"%s AMOUNT",FISCALPERIOD[y]);
							QAccFld(sortfile->fileno,"VOL W/H YTD 3",paydcsm,stemp);
						}
					} else if(isEMPTY(typ3))
					{
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 3",code2);
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION TITLE 3",title);
						for(y=1;y<=12;++y)
						{
							memset(stemp,0,101);
							sprintf(stemp,"%s AMOUNT",FISCALPERIOD[y]);
							QAccFld(sortfile->fileno,"VOL W/H YTD 3",paydcsm,stemp);
						}
					} else if(!RDAstrcmp(typ4,code2) && !isEMPTY(typ4))
					{ 
						for(y=1;y<=12;++y)
						{
							memset(stemp,0,101);
							sprintf(stemp,"%s AMOUNT",FISCALPERIOD[y]);
							QAccFld(sortfile->fileno,"VOL W/H YTD 4",paydcsm,stemp);
						}
					} else if(isEMPTY(typ4))
					{
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 4",code2);
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION TITLE 4",title);
						for(y=1;y<=12;++y)
						{
							memset(stemp,0,101);
							sprintf(stemp,"%s AMOUNT",FISCALPERIOD[y]);
							QAccFld(sortfile->fileno,"VOL W/H YTD 4",paydcsm,stemp);
						}
					} else if(!RDAstrcmp(typ5,code2) && !isEMPTY(typ5))
					{
						for(y=1;y<=12;++y)
						{
							memset(stemp,0,101);
							sprintf(stemp,"%s AMOUNT",FISCALPERIOD[y]);
							QAccFld(sortfile->fileno,"VOL W/H YTD 5",paydcsm,stemp);
						}
					} else if(isEMPTY(typ5))
					{
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 5",code2);
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION TITLE 5",title);
						for(y=1;y<=12;++y)
						{
							memset(stemp,0,101);
							sprintf(stemp,"%s AMOUNT",FISCALPERIOD[y]);
							QAccFld(sortfile->fileno,"VOL W/H YTD 5",paydcsm,stemp);
						}
					} else if(!RDAstrcmp(typ6,code2) && !isEMPTY(typ6))
					{
						for(y=1;y<=12;++y)
						{
							memset(stemp,0,101);
							sprintf(stemp,"%s AMOUNT",FISCALPERIOD[y]);
							QAccFld(sortfile->fileno,"VOL W/H YTD 6",paydcsm,stemp);
						}
					} else if(isEMPTY(typ6))
					{
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 6",code2);
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION TITLE 6",title);
						for(y=1;y<=12;++y)
						{
							memset(stemp,0,101);
							sprintf(stemp,"%s AMOUNT",FISCALPERIOD[y]);
							QAccFld(sortfile->fileno,"VOL W/H YTD 6",paydcsm,stemp);
						}
					} else if(!RDAstrcmp(typ7,code2) && !isEMPTY(typ7))
					{
						for(y=1;y<=12;++y)
						{
							memset(stemp,0,101);
							sprintf(stemp,"%s AMOUNT",FISCALPERIOD[y]);
							QAccFld(sortfile->fileno,"VOL W/H YTD 7",paydcsm,stemp);
						}
					} else if(isEMPTY(typ7))
					{
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 7",code2);
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION TITLE 7",title);
						for(y=1;y<=12;++y)
						{
							memset(stemp,0,101);
							sprintf(stemp,"%s AMOUNT",FISCALPERIOD[y]);
							QAccFld(sortfile->fileno,"VOL W/H YTD 7",paydcsm,stemp);
						}
					} else if(!RDAstrcmp(typ8,code2) && !isEMPTY(typ8))
					{
						for(y=1;y<=12;++y)
						{
							memset(stemp,0,101);
							sprintf(stemp,"%s AMOUNT",FISCALPERIOD[y]);
							QAccFld(sortfile->fileno,"VOL W/H YTD 8",paydcsm,stemp);
						}
					} else if(isEMPTY(typ8))
					{
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 8",code2);
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION TITLE 8",title);
						for(y=1;y<=12;++y)
						{
							memset(stemp,0,101);
							sprintf(stemp,"%s AMOUNT",FISCALPERIOD[y]);
							QAccFld(sortfile->fileno,"VOL W/H YTD 8",paydcsm,stemp);
						}
					}
#endif /* XXXX */
				}
			} else if((print_employer_deductions==TRUE) &&
				reduce_net==FALSE && ((lb==TRUE || ep==TRUE
				|| pay_with==TRUE || electtrans==TRUE)))
			{
				for(y=1;y<=12;++y)
				{
					memset(stemp,0,101);
					sprintf(stemp,"%s AMOUNT",FISCALPERIOD[y]);
					QAccFld(sortfile->fileno,"TOTAL EMPLOYER W/H YTD",paydcsm,stemp);
				}
			}
			}
		}
		} else {
			FINDFLDSETCHAR(sortfile->fileno,"OUT OF ORDER",TRUE);
		}
		ef=ADVNXTNRDsec(paydcsm,4,SCRNvirtualSubData,diagrsrc);
	}
	if(empid!=NULL) Rfree(empid);
	if(empid1!=NULL) Rfree(empid1);	
	if(typ1!=NULL) Rfree(typ1);
	if(typ2!=NULL) Rfree(typ2);
	if(typ3!=NULL) Rfree(typ3);
	if(typ4!=NULL) Rfree(typ4);
	if(typ5!=NULL) Rfree(typ5);
	if(typ6!=NULL) Rfree(typ6);
	if(typ7!=NULL) Rfree(typ7);
	if(typ8!=NULL) Rfree(typ8);
	if(type2!=NULL) Rfree(type2);
	if(code2!=NULL) Rfree(code2);
	if(title!=NULL) Rfree(title);
}
static void GatherLeaveSimple()
{
}
static void GatherLeave()
{
	short ef=0;
	char *lve_type=NULL,nos=FALSE;
	char *empid1=NULL,deleteflag=FALSE,*empid=NULL;
	double amt=0.0,pearn=0.0,pused=0.0,camt=0.0;
	double outlve=0.0,beg=0.0,earn=0.0,used=0.0;
	char active=FALSE;

	if(lvemst!=(-1) && lvedsc!=(-1) && (XPERT_SETUP->LVEMGT==TRUE || XPERT_SETUP->LVSIMP==TRUE) && skip_leave==FALSE)
	{
		FINDFLDGETSTRING(sortfile->fileno,"PERSONNEL IDENTIFICATION",&empid);
		ZERNRD(lvemst);
		FINDFLDSETSTRING(lvemst,"PERSONNEL IDENTIFICATION",empid);
		ef=ADVGTENRDsec(lvemst,1,SCRNvirtualSubData,diagrsrc);
		while(!ef)
		{
			FINDFLDGETSTRING(lvemst,"PERSONNEL IDENTIFICATION",&empid1);
			if(RDAstrcmp(empid,empid1)) break;
			FINDFLDGETCHAR(lvemst,"DELETEFLAG",&deleteflag);
			if(!deleteflag)
			{
				COPYFIELD(lvemst,lvedsc,"LEAVE DESCRIPTION IDENTIFICATION");
				if(ADVEQLNRDsec(lvedsc,1,SCRNvirtualSubData,diagrsrc)) 
				{
					KEYNRD(lvedsc,1);
				} else {
					FINDFLDGETSTRING(lvedsc,"LEAVE TYPE IDENTIFICATION",&lve_type);
					FINDFLDGETDOUBLE(lvemst,"OUTSIDE ORGANIZATION",&outlve);
					FINDFLDGETDOUBLE(lvemst,"BEGINNING BALANCE",&beg);
					FINDFLDGETDOUBLE(lvemst,"PROCESSED EARNINGS",&pearn);
					FINDFLDGETDOUBLE(lvemst,"PROCESSED USED",&pused);
					FINDFLDGETDOUBLE(lvemst,"UNPROCESSED EARNINGS",&earn);
					FINDFLDGETDOUBLE(lvemst,"UNPROCESSED USED",&used);
					FINDFLDGETCHAR(lvedsc,"ACTIVE",&active);
					FINDFLDGETCHAR(lvedsc,"NOT ON STUB",&nos);
					if((nos==FALSE) && active)
					{
						if(!RDAstrcmp(lve_type,"SICK") || !RDAstrcmp(lve_type,"SICK LEAVE"))
						{
							FINDFLDGETDOUBLE(sortfile->fileno,"SICK BEGINNING",&camt);	
							camt+=outlve+beg+pearn-pused;
							FINDFLDSETDOUBLE(sortfile->fileno,"SICK BEGINNING",camt);
							FINDFLDGETDOUBLE(sortfile->fileno,"SICK EARNED",&camt);	
							camt+=earn;
							FINDFLDSETDOUBLE(sortfile->fileno,"SICK EARNED",camt);
							FINDFLDGETDOUBLE(sortfile->fileno,"SICK USED",&camt);	
							camt+=used;
							FINDFLDSETDOUBLE(sortfile->fileno,"SICK USED",used);
							FINDFLDGETDOUBLE(sortfile->fileno,"SICK BALANCE",&amt);
							amt+=outlve+beg+pearn-pused+earn-used;
							FINDFLDSETDOUBLE(sortfile->fileno,"SICK BALANCE",amt);
						} else if(!RDAstrcmp(lve_type,"VACATION"))
						{
							FINDFLDGETDOUBLE(sortfile->fileno,"VACATION BEGINNING",&camt);	
							camt+=outlve+beg+pearn-pused;
							FINDFLDSETDOUBLE(sortfile->fileno,"VACATION BEGINNING",camt);
							FINDFLDGETDOUBLE(sortfile->fileno,"VACATION EARNED",&camt);	
							camt+=earn;
							FINDFLDSETDOUBLE(sortfile->fileno,"VACATION EARNED",camt);
							FINDFLDGETDOUBLE(sortfile->fileno,"VACATION USED",&camt);	
							camt+=used;
							FINDFLDSETDOUBLE(sortfile->fileno,"VACATION USED",used);
							FINDFLDGETDOUBLE(sortfile->fileno,"VACATION BALANCE",&amt);
							amt+=outlve+beg+pearn-pused+earn-used;
							FINDFLDSETDOUBLE(sortfile->fileno,"VACATION BALANCE",amt);
						} else if(!RDAstrcmp(lve_type,"PERSONAL"))
						{
							FINDFLDGETDOUBLE(sortfile->fileno,"PERSONAL BEGINNING",&camt);	
							camt+=outlve+beg+pearn-pused;
							FINDFLDSETDOUBLE(sortfile->fileno,"PERSONAL BEGINNING",camt);
							FINDFLDGETDOUBLE(sortfile->fileno,"PERSONAL EARNED",&camt);	
							camt+=earn;
							FINDFLDSETDOUBLE(sortfile->fileno,"PERSONAL EARNED",camt);
							FINDFLDGETDOUBLE(sortfile->fileno,"PERSONAL USED",&camt);	
							camt+=used;
							FINDFLDSETDOUBLE(sortfile->fileno,"PERSONAL USED",used);
							FINDFLDGETDOUBLE(sortfile->fileno,"PERSONAL BALANCE",&amt);
							amt+=outlve+beg+pearn-pused+earn-used;
							FINDFLDSETDOUBLE(sortfile->fileno,"PERSONAL BALANCE",amt);
						}
					}
				}
			}
			ef=ADVNXTNRDsec(lvemst,1,SCRNvirtualSubData,diagrsrc);
		}
	}
	if(empid!=NULL) Rfree(empid);
	if(empid1!=NULL) Rfree(empid1);
}
static void GatherHistory()
{
	short ef=0,y=0;
	char *empid1=NULL,deleteflag=FALSE,*empid=NULL;
	int jobno=0,cyear=0,cyear1=0;
	char *rateid=NULL;

	FINDFLDGETSTRING(sortfile->fileno,"PERSONNEL IDENTIFICATION",&empid);
	FINDFLDGETINT(paymst,"CALENDAR YEAR",&cyear);
	ZERNRD(payjrcs);
	FINDFLDSETSTRING(payjrcs,"PERSONNEL IDENTIFICATION",empid);
	FINDFLDSETINT(payjrcs,"CALENDAR YEAR",cyear);
	ef=ADVGTENRDsec(payjrcs,3,SCRNvirtualSubData,diagrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(payjrcs,"PERSONNEL IDENTIFICATION",&empid1);
		FINDFLDGETINT(payjrcs,"CALENDAR YEAR",&cyear1);
		if(RDAstrcmp(empid,empid1) || cyear!=cyear1) break;
		FINDFLDGETCHAR(payjrcs,"DELETEFLAG",&deleteflag);
		FINDFLDGETINT(payjrcs,"JOB NUMBER",&jobno);
		if(!UpdatedAlready(empid,jobno))
		{
		if(!deleteflag)
		{
			FINDFLDGETSTRING(payjrcs,"RATE IDENTIFICATION",&rateid);
			ZERNRD(posrate);
			FINDFLDSETSTRING(posrate,"RATE IDENTIFICATION",rateid);
			if(ADVEQLNRDsec(posrate,1,SCRNvirtualSubData,diagrsrc)) KEYNRD(posrate,1);
			if((strstr(rateid,"REGULAR")) || (strstr(rateid,"UNIT")))
			{
				for(y=1;y<=12;++y)
				{
					memset(stemp,0,101);
					sprintf(stemp,"%s GROSS",FISCALPERIOD[y]);
					QAccFld(sortfile->fileno,"UNIT YTD AMOUNT",payjrcs,stemp);
					memset(stemp,0,101);
					sprintf(stemp,"%s UNITS",FISCALPERIOD[y]);
					QAccFld(sortfile->fileno,"NUMBER YTD UNITS",payjrcs,"UNITS");
				}
			} else if(strstr(rateid,"OVERTIME"))
			{
				for(y=1;y<=12;++y)
				{
					memset(stemp,0,101);
					sprintf(stemp,"%s GROSS",FISCALPERIOD[y]);
					QAccFld(sortfile->fileno,"OVER GROSS YTD AMOUNT",payjrcs,stemp);
					memset(stemp,0,101);
					sprintf(stemp,"%s UNITS",FISCALPERIOD[y]);
					QAccFld(sortfile->fileno,"OVER UNITS YTD AMOUNT",payjrcs,stemp);
				}
			} else if(strstr(rateid,"ADJUST"))
			{
				for(y=1;y<=12;++y)
				{
					memset(stemp,0,101);
					sprintf(stemp,"%s GROSS",FISCALPERIOD[y]);
					QAccFld(sortfile->fileno,"ADJUSTED YTD AMOUNT",payjrcs,stemp);
				}
			} else if(strstr(rateid,"DOCKED"))
			{
				for(y=1;y<=12;++y)
				{
					memset(stemp,0,101);
					sprintf(stemp,"%s GROSS",FISCALPERIOD[y]);
					QAccFld(sortfile->fileno,"DOCKED YTD AMOUNT",payjrcs,stemp);
				}
			} else { /* Gross */
				for(y=1;y<=12;++y)
				{
					memset(stemp,0,101);
					sprintf(stemp,"%s GROSS",FISCALPERIOD[y]);
					QAccFld(sortfile->fileno,"GROSS YTD AMOUNT",payjrcs,stemp);
				}
			}
			if(rateid!=NULL) Rfree(rateid);

		}
		} else {
			FINDFLDSETCHAR(sortfile->fileno,"OUT OF ORDER",TRUE);
		}
		ef=ADVNXTNRDsec(payjrcs,3,SCRNvirtualSubData,diagrsrc);
	}
	if(empid!=NULL) Rfree(empid);
	if(empid1!=NULL) Rfree(empid1);
}
static void printactualcheck(RDA_PFILE *fp)
{
	char *ssn=NULL,*emp_name=NULL,*loc=NULL;
	char *emp_add1=NULL,*emp_add2=NULL,*emp_add3=NULL;
	char *title1=NULL,*title2=NULL,*title3=NULL,*title4=NULL;
	char *title5=NULL,*title6=NULL,*title7=NULL,*title8=NULL;
	double vol1=0.0,vol2=0.0,vol3=0.0,vol4=0.0,vol5=0.0,vol6=0.0,vol7=0.0;
	double vol8=0.0,reg_units=0.0,regular=0.0,gross=0.0,over_units=0.0;
	double overtime=0.0,vace=0.0,vacu=0.0,sicku=0.0,total_gross=0.0;
	double adjust=0.0,docked=0.0,sicke=0.0,peru=0.0,pere=0.0;
	double state_ret=0.0,other_ret=0.0,fed=0.0,fica=0.0,med=0.0;
	double state=0.0,local=0.0,net=0.0,total_gross_ytd;
	double fed_ytd=0.0,fica_ytd=0.0,med_ytd=0.0,state_ytd=0.0;
	double regular_ytd=0.0,gross_ytd=0.0,adjust_ytd=0.0,overtime_ytd=0.0;
	double docked_ytd=0.0,vacb=0.0,sickb=0.0,perb=0.0,local_ytd=0.0;
	int checkno=0;
	char *spelled_amt=NULL;
	char *check_date=NULL;
	char *temp1=NULL,outoforder=FALSE;
	short x=0;
	short prtsig=1;


	FINDFLDGETINT(sortfile->fileno,"CHECK NUMBER",&checkno);
	FINDFLDGETSTRING(sortfile->fileno,"CHECK DATE",&check_date);
	FINDFLDGETCHAR(sortfile->fileno,"OUT OF ORDER",&outoforder);
	FINDFLDGETSTRING(sortfile->fileno,"SOCIAL SECURITY NUMBER",&ssn);
	FINDFLDGETSTRING(sortfile->fileno,"EMPLOYEE NAME",&emp_name);
	FINDFLDGETSTRING(sortfile->fileno,"ADDRESS 1",&emp_add1);
	FINDFLDGETSTRING(sortfile->fileno,"ADDRESS 2",&emp_add2);
	FINDFLDGETSTRING(sortfile->fileno,"ADDRESS 3",&emp_add3);
	FINDFLDGETSTRING(sortfile->fileno,"LOCATION IDENTIFICATION",&loc);


	FINDFLDGETDOUBLE(sortfile->fileno,"GROSS YTD AMOUNT",&gross_ytd);
	FINDFLDGETDOUBLE(sortfile->fileno,"ADJUSTED YTD AMOUNT",&adjust_ytd);
	FINDFLDGETDOUBLE(sortfile->fileno,"UNIT YTD AMOUNT",&regular_ytd);
	FINDFLDGETDOUBLE(sortfile->fileno,"OVER GROSS YTD AMOUNT",&overtime_ytd);
	FINDFLDGETDOUBLE(sortfile->fileno,"DOCKED YTD AMOUNT",&docked_ytd);
	total_gross_ytd=(gross_ytd+adjust_ytd+regular_ytd+overtime_ytd+docked_ytd);

	FINDFLDGETDOUBLE(sortfile->fileno,"GROSS CURRENT AMOUNT",&gross);
	FINDFLDGETDOUBLE(sortfile->fileno,"ADJUSTED CURRENT AMOUNT",&adjust);
	FINDFLDGETDOUBLE(sortfile->fileno,"NUMBER CURRENT UNITS",&reg_units);
	FINDFLDGETDOUBLE(sortfile->fileno,"UNIT CURRENT AMOUNT",&regular);
	FINDFLDGETDOUBLE(sortfile->fileno,"OVER GROSS CURRENT AMOUNT",&overtime);
	FINDFLDGETDOUBLE(sortfile->fileno,"OVER UNITS CURRENT AMOUNT",&over_units);
	FINDFLDGETDOUBLE(sortfile->fileno,"DOCKED CUR AMOUNT",&docked);
	FINDFLDGETDOUBLE(sortfile->fileno,"VACATION EARNED",&vace);
	FINDFLDGETDOUBLE(sortfile->fileno,"VACATION USED",&vacu);
	FINDFLDGETDOUBLE(sortfile->fileno,"VACATION BALANCE",&vacb);
	FINDFLDGETDOUBLE(sortfile->fileno,"SICK EARNED",&sicke);
	FINDFLDGETDOUBLE(sortfile->fileno,"SICK USED",&sicku);
	FINDFLDGETDOUBLE(sortfile->fileno,"SICK BALANCE",&sickb);
	FINDFLDGETDOUBLE(sortfile->fileno,"PERSONAL USED",&peru);
	FINDFLDGETDOUBLE(sortfile->fileno,"PERSONAL EARNED",&pere);
	FINDFLDGETDOUBLE(sortfile->fileno,"PERSONAL BALANCE",&perb);
	FINDFLDGETDOUBLE(sortfile->fileno,"STATE RETIREMENT W/H CURRENT",&state_ret);
	FINDFLDGETDOUBLE(sortfile->fileno,"RETIREMENT W/H CURRENT",&other_ret);
	FINDFLDGETDOUBLE(sortfile->fileno,"FEDERAL W/H CURRENT",&fed);
	FINDFLDGETDOUBLE(sortfile->fileno,"FEDERAL W/H YTD",&fed_ytd);
	FINDFLDGETDOUBLE(sortfile->fileno,"FICA W/H CURRENT",&fica);
	FINDFLDGETDOUBLE(sortfile->fileno,"FICA W/H YTD",&fica_ytd);
	FINDFLDGETDOUBLE(sortfile->fileno,"MEDICARE W/H CURRENT",&med);
	FINDFLDGETDOUBLE(sortfile->fileno,"MEDICARE W/H YTD",&med_ytd);
	FINDFLDGETDOUBLE(sortfile->fileno,"LOCAL W/H CURRENT",&local);
	FINDFLDGETDOUBLE(sortfile->fileno,"LOCAL W/H YTD",&local_ytd);
	FINDFLDGETDOUBLE(sortfile->fileno,"STATE W/H CURRENT",&state);
	FINDFLDGETDOUBLE(sortfile->fileno,"STATE W/H YTD",&state_ytd);
	FINDFLDGETDOUBLE(sortfile->fileno,"NET CUR AMOUNT",&net);
	total_gross=gross+adjust+regular+overtime+docked;


	FINDFLDGETSTRING(sortfile->fileno,"VOL DEDUCTION TITLE 1",&title1);
	FINDFLDGETSTRING(sortfile->fileno,"VOL DEDUCTION TITLE 2",&title2);
	FINDFLDGETSTRING(sortfile->fileno,"VOL DEDUCTION TITLE 3",&title3);
	FINDFLDGETSTRING(sortfile->fileno,"VOL DEDUCTION TITLE 4",&title4);
	FINDFLDGETSTRING(sortfile->fileno,"VOL DEDUCTION TITLE 5",&title5);
	FINDFLDGETSTRING(sortfile->fileno,"VOL DEDUCTION TITLE 6",&title6);
	FINDFLDGETSTRING(sortfile->fileno,"VOL DEDUCTION TITLE 7",&title7);
	FINDFLDGETSTRING(sortfile->fileno,"VOL DEDUCTION TITLE 8",&title8);
	FINDFLDGETDOUBLE(sortfile->fileno,"VOL W/H CURRENT 1",&vol1);
	FINDFLDGETDOUBLE(sortfile->fileno,"VOL W/H CURRENT 2",&vol2);
	FINDFLDGETDOUBLE(sortfile->fileno,"VOL W/H CURRENT 3",&vol3);
	FINDFLDGETDOUBLE(sortfile->fileno,"VOL W/H CURRENT 4",&vol4);
	FINDFLDGETDOUBLE(sortfile->fileno,"VOL W/H CURRENT 5",&vol5);
	FINDFLDGETDOUBLE(sortfile->fileno,"VOL W/H CURRENT 6",&vol6);
	FINDFLDGETDOUBLE(sortfile->fileno,"VOL W/H CURRENT 7",&vol7);
	FINDFLDGETDOUBLE(sortfile->fileno,"VOL W/H CURRENT 8",&vol8);

	if(check_formfeed==TRUE && top_feeder==FALSE)
	{
		RDA_fprintf(fp,"\r\n");
		RDA_fprintf(fp,"\r\n");
	}
	for(x=0;x<pay_check_offset;++x)
	{
		RDA_fprintf(fp,"\r\n");
	}
	RDA_fprintf(fp,"\r\n");
	if(RDAstrlen(loc)>10) loc[10]=0;
	if(display_ssn == TRUE) 
	{
	RDA_fprintf(fp,"%-11s %-35s %-10s %-10s %-10s\r\n",(ssn!=NULL ? ssn:""),
		(emp_name!=NULL ? emp_name:""),(loc!=NULL ? loc:""),
		(payending!=NULL ? payending:""),check_date);
		} else {
	RDA_fprintf(fp,"            %-35s %-10s %-10s %-10s\r\n", 
	(emp_name!=NULL ? emp_name:""),(loc!=NULL ? loc:""),
		(payending!=NULL ? payending:""),check_date);
	}
/* (payending!=NULL ? payending:""),CURRENT_DATE10); */
	RDA_fprintf(fp,"%-40s%-40s\r\n",(message1!=NULL ? message1:""),(message2!=NULL ? message2:""));
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"      %6.02f %8.02f %8.02f %5.02f %7.02f ",reg_units,
		regular/100,gross/100,over_units,overtime/100);
	if((XPERT_SETUP->LVEMGT==TRUE || XPERT_SETUP->LVSIMP==TRUE) && skip_leave==FALSE)
	{
		if((check_type==3) || (check_type==4))
		{
			RDA_fprintf(fp," %4.1f %5.1f %5.1f %8.02f %8.02f\r\n",vace,vacu,sicku,(adjust+docked)/100,total_gross/100);
		} else {
			RDA_fprintf(fp," %4.1f %5.1f %5.1f %8.02f %8.02f\r\n",peru,vacu,sicku,(adjust+docked)/100,total_gross/100);
		}
	} else {
		RDA_fprintf(fp,"%18s%8.02f %8.02f\r\n","",(adjust+docked)/100,total_gross/100);
	}
	RDA_fprintf(fp,"\r\n");



/* voluntary deductions */
	if(!isEMPTY(title1))
	{
		RDA_fprintf(fp,"%-8s ",title1);
	} else RDA_fprintf(fp,"%-8s ","");
	if(!isEMPTY(title2))
	{
		RDA_fprintf(fp,"%-8s ",title2);
	} else RDA_fprintf(fp,"%-8s ","");
	if(!isEMPTY(title3))
	{
		RDA_fprintf(fp,"%-8s ",title3);
	} else RDA_fprintf(fp,"%-8s ","");
	if(!isEMPTY(title4))
	{
		RDA_fprintf(fp,"%-8s ",title4);
	} else RDA_fprintf(fp,"%-8s ","");
	if(!isEMPTY(title5))
	{
		RDA_fprintf(fp,"%-8s ",title5);
	} else RDA_fprintf(fp,"%-8s ","");
	if(!isEMPTY(title6))
	{
		RDA_fprintf(fp,"%-8s ",title6);
	} else RDA_fprintf(fp,"%-8s ","");
	if(!isEMPTY(title7))
	{
		RDA_fprintf(fp,"%-8s ",title7);
	} else RDA_fprintf(fp,"%-8s ","");
	if(!isEMPTY(title8))
	{
		RDA_fprintf(fp,"%-8s ",title8);
	} else RDA_fprintf(fp,"%-8s ","");
	RDA_fprintf(fp," \r\n");

	if(!isEMPTY(title1))
	{
		RDA_fprintf(fp,"%8.02f ",vol1/100);
	} else RDA_fprintf(fp,"%-8s ","");
	if(!isEMPTY(title2))
	{
		RDA_fprintf(fp,"%8.02f ",vol2/100);
	} else RDA_fprintf(fp,"%-8s ","");
	if(!isEMPTY(title3))
	{
		RDA_fprintf(fp,"%8.02f ",vol3/100);
	} else RDA_fprintf(fp,"%-8s ","");
	if(!isEMPTY(title4))
	{
		RDA_fprintf(fp,"%8.02f ",vol4/100);
	} else RDA_fprintf(fp,"%-8s ","");
	if(!isEMPTY(title5))
	{
		RDA_fprintf(fp,"%8.02f ",vol5/100);
	} else RDA_fprintf(fp,"%-8s ","");
	if(!isEMPTY(title6))
	{
		RDA_fprintf(fp,"%8.02f ",vol6/100);
	} else RDA_fprintf(fp,"%-8s ","");
	if(!isEMPTY(title7))
	{
		RDA_fprintf(fp,"%8.02f ",vol7/100);
	} else RDA_fprintf(fp,"%-8s ","");
	if(!isEMPTY(title8))
	{
		RDA_fprintf(fp,"%8.02f ",vol8/100);
	} else RDA_fprintf(fp,"%-8s ","");
	RDA_fprintf(fp,"%8.02f",(vol1+vol2+vol3+vol4+vol5+vol6+vol7+vol8)/100);
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");



	RDA_fprintf(fp,"      %7.02f %7.02f%12s %8.02f %8.02f %8.02f %7.02f    %8.02f\r\n",
		state_ret/100,other_ret/100,"",fed/100,(fica+med)/100,
		state/100,local/100,(fed+fica+med+state+local)/100);
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");


if(!outoforder)
{
	if((XPERT_SETUP->LVEMGT==TRUE || XPERT_SETUP->LVSIMP==TRUE) && skip_leave==FALSE)
	{
		if(check_type==3)
		{
			RDA_fprintf(fp,"      %9.02f %9.02f %8.02f %10.02f %6.02f %6.02f %9s %10.02f\r\n",
				fed_ytd/100,(fica_ytd+med_ytd)/100,state_ytd/100,
				total_gross_ytd/100,vacb,sickb,"",net/100);
		} else if(check_type==4)
		{
			RDA_fprintf(fp,"   %9.02f %9.02f %8.02f %10.02f %6.02f %6.02f %6.02f %5s %10.02f\r\n",
				fed_ytd/100,(fica_ytd+med_ytd)/100,state_ytd/100,
				total_gross_ytd/100,vacb,sickb,perb,"",net/100);
		} else if(check_type==5)
		{
			RDA_fprintf(fp,"   %9.02f %9.02f %8.02f %10.02f %6.02f %6.02f %8.02f %3s %10.02f\r\n",
				fed_ytd/100,(fica_ytd+med_ytd)/100,state_ytd/100,
				total_gross_ytd/100,vacb,sickb,local_ytd/100,"",
				net/100);
		} else {
			RDA_fprintf(fp,"   %9.02f %9.02f %8.02f %10.02f %6.02f %6.02f %6.02f %5s %10.02f\r\n",
				fed_ytd/100,(fica_ytd+med_ytd)/100,state_ytd/100,
				total_gross_ytd/100,vacb,sickb,perb,"",net/100);
		}
	} else {
		if((check_type==3))
		{
			RDA_fprintf(fp,"      %9.02f %9.02f %8.02f %10.02f %23s %10.02f\r\n",
				fed_ytd/100,(fica_ytd+med_ytd)/100,state_ytd/100,
				total_gross_ytd/100,"",net/100);
		} else if((check_type==4))
		{
			RDA_fprintf(fp,"   %9.02f %9.02f %8.02f %10.02f %26s %10.02f\r\n",
				fed_ytd/100,(fica_ytd+med_ytd)/100,state_ytd/100,
				total_gross_ytd/100,"",net/100);
		} else if(check_type==5)
		{

			RDA_fprintf(fp,"   %9.02f %9.02f %8.02f %10.02f %13s %8.02f %3s %10.02f\r\n",
				fed_ytd/100,(fica_ytd+med_ytd)/100,state_ytd/100,
				total_gross_ytd/100,"",local_ytd/100,"",net/100);
		} else {
			RDA_fprintf(fp,"   %9.02f %9.02f %8.02f %10.02f %26s %10.02f\r\n",
				fed_ytd/100,(fica_ytd+med_ytd)/100,state_ytd/100,
				total_gross_ytd/100,"",net/100);
		}
	}
} else { 
	if((XPERT_SETUP->LVEMGT==TRUE || XPERT_SETUP->LVSIMP==TRUE) && skip_leave==FALSE)
	{
		if(check_type==3)
		{
			RDA_fprintf(fp,"      %15s %3s %8s %10s %6.02f %6.02f %9s %10.02f\r\n",
				"HISTORY UPDATED","","","",vacb,sickb,"",net/100);
		} else if(check_type==4)
		{
			RDA_fprintf(fp,"   %15s %3s %8s %10s %6.02f %6.02f %6.02f %5s %10.02f\r\n",
				"HISTORY UPDATED","","","",vacb,sickb,perb,"",net/100);
		} else if(check_type==5)
		{
			RDA_fprintf(fp,"   %15s %3s %8s %10s %6.02f %6.02f %8.02f %3s %10.02f\r\n",
				"HISTORY UPDATED","","","",vacb,sickb,"","",
				net/100);
		} else {
			RDA_fprintf(fp,"   %15s %3s %8s %10s %6.02f %6.02f %6.02f %5s %10.02f\r\n",
				"HISTORY UPDATED","","","",vacb,sickb,perb,"",net/100);
		}
	} else {
		if((check_type==3))
		{
			RDA_fprintf(fp,"      %15s %3s %8s %10s %23s %10.02f\r\n",
				"HISTORY UPDATED","","","","",net/100);
		} else if((check_type==4))
		{
			RDA_fprintf(fp,"   %15s %3s %8s %10s %26s %10.02f\r\n",
				"HISTORY UPDATED","","","","",net/100);
		} else if(check_type==5)
		{

			RDA_fprintf(fp,"   %15s %3s %8s %10s %13s %8s %3s %10.02f\r\n",
				"HISTORY UPDATED","","","","","","",net/100);
		} else {
			RDA_fprintf(fp,"   %15s %3s %8s %10s %26s %10.02f\r\n",
				"HISTORY UPDATED","","","","",net/100);
		}
	}
}
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");



	if(check_signature!=3)
	{
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	}


	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	if(spelled_pos==1)
	{
		RDA_fprintf(fp,"\r\n");
	}
	if(net>0)
	{
		spelled_amt=spell(net);
		RDA_fprintf(fp,"  %s\r\n",spelled_amt);
		if(spelled_amt!=NULL) Rfree(spelled_amt);
		prtsig=1;
	} else {
		RDA_fprintf(fp,"  *** V O I D **** V O I D **** V O I D **** \r\n");
		prtsig=0;
	}
	if(spelled_pos==0)
	{
		RDA_fprintf(fp,"\r\n");
	}
	RDA_fprintf(fp,"\r\n");
	temp1=famtpad(net,11,'*');
	RDA_fprintf(fp,"%48s %-10s %8d %-11s\r\n",
		"",check_date,checkno,temp1);
	if(temp1!=NULL) Rfree(temp1);
/*
	RDA_fprintf(fp,"%48s %-10s %8d %10.02f\r\n",
		"",check_date,checkno,net/100);
*/
/*
		"",CURRENT_DATE10,checkno,net/100);
*/
	if(check_signature==0)
	{
		RDA_fprintf(fp,"\r\n");
	} else if((check_signature==5))
	{
		SelectPrintTypeByName(outdevice,"PUSH CURSOR",fp);
		if(prtsig) { SelectPrintTypeByName(outdevice,"PAYROLL SIGNATURE",fp); }
		SelectPrintTypeByName(outdevice,"POP CURSOR",fp);
		RDA_fprintf(fp,"\r\n");
	} else if((check_signature==1) || (check_signature==3))
	{
		RDA_fprintf(fp,"%52s","");
		if(use_sign_locks)
		{
			SelectPrintTypeByName(outdevice,"SIGNATURE UNLOCK",fp);
		}
		if(prtsig) { SelectPrintTypeByName(outdevice,"SIGNATURE DIMM 1",fp); }
		if(use_sign_locks)
		{
			SelectPrintTypeByName(outdevice,"SIGNATURE LOCK",fp);
		}
		RDA_fprintf(fp,"\r\n");
	} else if(check_signature==2)
	{
		if(use_sign_locks)
		{
			SelectPrintTypeByName(outdevice,"SIGNATURE UNLOCK",fp);
		}
		if(prtsig) { SelectPrintTypeByName(outdevice,"SIGNATURE DIMM A",fp); }
		if(use_sign_locks)
		{
			SelectPrintTypeByName(outdevice,"SIGNATURE LOCK",fp);
		}
		RDA_fprintf(fp,"\r\n");
	}
	if(check_signature==3)
	{
		RDA_fprintf(fp,"\r\n");
		RDA_fprintf(fp,"\r\n");
	}
	/*
	RDA_fprintf(fp,"%*s%-40s\r\n","",8+ADDRESS_OFFSET,"",(emp_name!=NULL ? emp_name:""));
	RDA_fprintf(fp,"%*s%-40s\r\n","",8+ADDRESS_OFFSET,"",(emp_add1!=NULL ? emp_add1:""));
	RDA_fprintf(fp,"%*s%-40s\r\n","",8+ADDRESS_OFFSET,"",(emp_add2!=NULL ? emp_add2:""));
	RDA_fprintf(fp,"%*s%-40s\r\n","",8+ADDRESS_OFFSET,"",(emp_add3!=NULL ? emp_add3:""));
	*/
	RDA_fprintf(fp,"%*s%-40s\r\n",8+ADDRESS_OFFSET,"",(emp_name!=NULL ? emp_name:""));
	RDA_fprintf(fp,"%*s%-40s\r\n",8+ADDRESS_OFFSET,"",(emp_add1!=NULL ? emp_add1:""));
	RDA_fprintf(fp,"%*s%-40s\r\n",8+ADDRESS_OFFSET,"",(emp_add2!=NULL ? emp_add2:""));
	RDA_fprintf(fp,"%*s%-40s\r\n",8+ADDRESS_OFFSET,"",(emp_add3!=NULL ? emp_add3:""));
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	RDA_fprintf(fp,"\r\n");
	if(check_formfeed) RDA_fprintf(fp,"\f");


	if(ssn!=NULL) Rfree(ssn);
	if(emp_name!=NULL) Rfree(emp_name);
	if(emp_add1!=NULL) Rfree(emp_add1);
	if(emp_add2!=NULL) Rfree(emp_add2);
	if(emp_add3!=NULL) Rfree(emp_add3);
	if(loc!=NULL) Rfree(loc);
	if(check_date!=NULL) Rfree(check_date);
	if(title1!=NULL) Rfree(title1);
	if(title2!=NULL) Rfree(title2);
	if(title3!=NULL) Rfree(title3);
	if(title4!=NULL) Rfree(title4);
	if(title5!=NULL) Rfree(title5);
	if(title6!=NULL) Rfree(title6);
	if(title7!=NULL) Rfree(title7);
	if(title8!=NULL) Rfree(title8);
}
static void printchecks(RDArsrc *r,RDArsrc *mainrsrc)
{
	short ef=FALSE;
	RDA_PFILE *fp=NULL;

	readwidget(r,"OUTPUT DEVICE");
	FINDRSCGETSTRING(r,"OUTPUT DEVICE",&outdevice);
	if(!isEMPTY(outdevice))
	{
		fp=RDA_popen(outdevice,"w");
		SelectPrintTypeByName(outdevice,"PCL JOB HEADER",fp);
		if(check_signature==5)
		{
			SelectPrintTypeByName(outdevice,"LOAD PAYROLL SIGNATURE",fp);
		}
		SelectPrintTypeByName(outdevice,"SIMPLEX",fp);
		SelectPrintType(outdevice,0,fp);
		if(top_feeder==TRUE)
		{
			RDA_fprintf(fp,"\f");
		}
	} else {
		prterr("Error:  Output device not found.");
		return;
	}
	diagrsrc=diagscrn(sortfile->fileno,"DIAGNOSTIC SCREEN",module,
		"Printing Transmittals",NULL);
	addDFincvir(diagrsrc,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(diagrsrc,module,"PAYJMST",NULL,payjmst);
	addDFincvir(diagrsrc,"POSTRK","POSLMST",NULL,poslmst);
	addDFincvir(diagrsrc,"POSTRK","POSMSTR",NULL,posmst);
	addDFincvir(diagrsrc,module,"PAYDEDM",NULL,paydedm);
	addDFincvir(diagrsrc,module,"PAYDDMS",NULL,payddms);
	addDFincvir(diagrsrc,module,"PAYDTYP",NULL,paydtyp);
	if(XPERT_SETUP->LVSIMP==TRUE)
	{
	addDFincvir(diagrsrc,"LVSIMP","LVSMSTR",NULL,lvemst);
	addDFincvir(diagrsrc,"LVSIMP","LVSDSC",NULL,lvedsc);
	} else {
	addDFincvir(diagrsrc,"LVEMGT","LVEMSTR",NULL,lvemst);
	addDFincvir(diagrsrc,"LVEMGT","LVEDSC",NULL,lvedsc);
	}
	addDFincvir(diagrsrc,"BNKREC","BNKACC",NULL,bnkacc);
	if(diagrsrc!=NULL)
	{
		if(!APPmakescrn(diagrsrc,TRUE,NULL,NULL,FALSE))
			ForceWindowUpdate(diagrsrc);
	}
	ZERNRD(sortfile->fileno);
	ef=FRSNRD(sortfile->fileno,1);
	while(!ef)
	{
		printactualcheck(fp);
		ef=NXTNRD(sortfile->fileno,1);
	}
	if(check_signature==5)
	{
		SelectPrintTypeByName(outdevice,"DELETE_SOFTFONT_200",fp);
		SelectPrintTypeByName(outdevice,"DELETE_SOFTFONT_201",fp);
	}
	SelectPrintTypeByName(outdevice,"PCL JOB FOOTER",fp);
	RDA_pclose(fp);
	if(diagrsrc!=NULL)
	{
		killwindow(diagrsrc);
		free_rsrc(diagrsrc);
		diagrsrc=NULL;
	}
	quitprint(r,mainrsrc);
}
static void GatherCurrentDeductions()
{
	char *empid1=NULL,deleteflag=FALSE,*empid=NULL,bfs=FALSE;
	char *payid1=NULL,*payid2=NULL;
	short ef=0;
	char *type2=NULL;
	char *typ1=NULL,*typ2=NULL,*typ3=NULL,*typ4=NULL,*typ5=NULL,*typ6=NULL;
	char *typ7=NULL,*typ8=NULL;
	char reduce_net=FALSE,lb=FALSE,ep=FALSE,pay_with=FALSE,electtrans=FALSE;
	int jobno=0,jobno1=0;
	char *title=NULL,active1=FALSE,*code2=NULL;
	double amt=0.0;

	ZERNRD(payjdpm);
	FINDFLDGETSTRING(payjpms,"PERSONNEL IDENTIFICATION",&empid);
	FINDFLDGETINT(payjpms,"JOB NUMBER",&jobno);
	FINDFLDGETSTRING(payjpms,"PAY IDENTIFICATION",&payid1);
	FINDFLDSETSTRING(payjdpm,"PERSONNEL IDENTIFICATION",empid);
	FINDFLDSETINT(payjdpm,"JOB NUMBER",jobno);
	FINDFLDSETSTRING(payjdpm,"PAY IDENTIFICATION",payid1);
	ef=ADVGTENRDsec(payjdpm,2,SCRNvirtualSubData,diagrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(payjdpm,"PERSONNEL IDENTIFICATION",&empid1);
		FINDFLDGETINT(payjdpm,"JOB NUMBER",&jobno1);
		FINDFLDGETSTRING(payjdpm,"PAY IDENTIFICATION",&payid2);
		if(RDAstrcmp(empid,empid1) || jobno!=jobno1 || RDAstrcmp(payid1,payid2)) break;
		COPYFIELD(payjdpm,paydedm,"DEDUCTION DESCRIPTION IDENTIFICATION");
		FINDFLDSETSTRING(paydedm,"PERSONNEL IDENTIFICATION",empid);
		FINDFLDGETCHAR(payjdpm,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			COPYFIELD(paydedm,payddms,"DEDUCTION DESCRIPTION IDENTIFICATION");
			if(ADVEQLNRDsec(payddms,1,SCRNvirtualSubData,diagrsrc))
			{
				KEYNRD(payddms,1);
			}
			FINDFLDGETCHAR(payddms,"ACTIVE",&active1);
			FINDFLDGETDOUBLE(payjdpm,"AMOUNT",&amt);
			if(active1 && amt!=0.0)
			{
			FINDFLDGETCHAR(payddms,"REDUCE NET",&reduce_net);
			FINDFLDGETCHAR(payddms,"LIABILITY",&lb);
			FINDFLDGETCHAR(payddms,"EXPENSED",&ep);
			FINDFLDGETCHAR(payddms,"PRINT CHECK",&pay_with);
			FINDFLDGETCHAR(payddms,"ELECTRONIC TRANSFER",&electtrans);
			FINDFLDGETCHAR(payddms,"BLOCK FROM STUB",&bfs);
			if(bfs==FALSE)
			{
				FINDFLDGETSTRING(payddms,"DEDUCTION TYPE",&type2);
			if((reduce_net==TRUE) || (!RDAstrcmp(type2,"STATE RETIRE")))
			/* if(reduce_net==TRUE) */
			{
				FINDFLDGETSTRING(payddms,"DEDUCTION TYPE",&type2);
				FINDFLDGETSTRING(payddms,"DEDUCTION DESCRIPTION IDENTIFICATION",&code2);
				FINDFLDGETSTRING(payddms,"SHORT DESCRIPTION",&title);
				if(!RDAstrcmp(type2,"FEDERAL"))
				{
					FINDFLDSETSTRING(sortfile->fileno,"FEDERAL DEDUCTION TITLE",title);
					QAccFld(sortfile->fileno,"FEDERAL W/H YTD",payjdpm,"AMOUNT");
					QAccFld(sortfile->fileno,"FEDERAL W/H CURRENT",payjdpm,"AMOUNT");
				} else if(!RDAstrcmp(type2,"FICA")) 
				{
					FINDFLDSETSTRING(sortfile->fileno,"FICA DEDUCTION TITLE",title);
					QAccFld(sortfile->fileno,"FICA W/H YTD",payjdpm,"AMOUNT");
					QAccFld(sortfile->fileno,"FICA W/H CURRENT",payjdpm,"AMOUNT");
				} else if(!RDAstrcmp(type2,"MEDICARE"))
				{
					FINDFLDSETSTRING(sortfile->fileno,"MEDICARE DEDUCTION TITLE",title);
					QAccFld(sortfile->fileno,"MEDICARE W/H YTD",payjdpm,"AMOUNT");
					QAccFld(sortfile->fileno,"MEDICARE W/H CURRENT",payjdpm,"AMOUNT");
				} else if(!RDAstrcmp(type2,"STATE"))
				{
					FINDFLDSETSTRING(sortfile->fileno,"STATE DEDUCTION TITLE",title);
					QAccFld(sortfile->fileno,"STATE W/H YTD",payjdpm,"AMOUNT");
					QAccFld(sortfile->fileno,"STATE W/H CURRENT",payjdpm,"AMOUNT");
				} else if(!RDAstrcmp(type2,"LOCAL"))
				{
					FINDFLDSETSTRING(sortfile->fileno,"LOCAL DEDUCTION TITLE",title);
					QAccFld(sortfile->fileno,"LOCAL W/H YTD",payjdpm,"AMOUNT");
					QAccFld(sortfile->fileno,"LOCAL W/H CURRENT",payjdpm,"AMOUNT");
				} else if(!RDAstrcmp(type2,"STATE RETIRE"))
				{
					FINDFLDSETSTRING(sortfile->fileno,"ST RET DEDUCTION TITLE",title);
					QAccFld(sortfile->fileno,"STATE RETIREMENT W/H YTD",payjdpm,"AMOUNT");
					QAccFld(sortfile->fileno,"STATE RETIREMENT W/H CURRENT",payjdpm,"AMOUNT");
				} else if(!RDAstrcmp(type2,"RETIREMENT-M"))
				{
					FINDFLDSETSTRING(sortfile->fileno,"RETIREMENT DEDUCTION TITLE",title);
					QAccFld(sortfile->fileno,"RETIREMENT W/H YTD",payjdpm,"AMOUNT");
					QAccFld(sortfile->fileno,"RETIREMENT W/H CURRENT",payjdpm,"AMOUNT");
				} else if(!RDAstrcmp(type2,"CHECK"))
				{
				} else {
					FINDFLDGETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 1",&typ1);
					FINDFLDGETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 2",&typ2);
					FINDFLDGETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 3",&typ3);
					FINDFLDGETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 4",&typ4);
					FINDFLDGETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 5",&typ5);
					FINDFLDGETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 6",&typ6);
					FINDFLDGETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 7",&typ7);
					FINDFLDGETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 8",&typ8);
					if(!RDAstrcmp(typ1,code2) && !isEMPTY(typ1))
					{
						QAccFld(sortfile->fileno,"VOL W/H CURRENT 1",payjdpm,"AMOUNT");
						QAccFld(sortfile->fileno,"VOL W/H YTD 1",payjdpm,"AMOUNT");
					} else if(isEMPTY(typ1))
					{
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 1",code2);
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION TITLE 1",title);
						QAccFld(sortfile->fileno,"VOL W/H CURRENT 1",payjdpm,"AMOUNT");
						QAccFld(sortfile->fileno,"VOL W/H YTD 1",payjdpm,"AMOUNT");
					} else if(!RDAstrcmp(typ2,code2) && !isEMPTY(typ2))
					{
						QAccFld(sortfile->fileno,"VOL W/H CURRENT 2",payjdpm,"AMOUNT");
						QAccFld(sortfile->fileno,"VOL W/H YTD 2",payjdpm,"AMOUNT");
					} else if(isEMPTY(typ2))
					{
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 2",code2);
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION TITLE 2",title);
						QAccFld(sortfile->fileno,"VOL W/H CURRENT 2",payjdpm,"AMOUNT");
						QAccFld(sortfile->fileno,"VOL W/H YTD 2",payjdpm,"AMOUNT");
					} else if(!RDAstrcmp(typ3,code2) && !isEMPTY(typ3))
					{
						QAccFld(sortfile->fileno,"VOL W/H CURRENT 3",payjdpm,"AMOUNT");
						QAccFld(sortfile->fileno,"VOL W/H YTD 3",payjdpm,"AMOUNT");
					} else if(isEMPTY(typ3))
					{
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 3",code2);
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION TITLE 3",title);
						QAccFld(sortfile->fileno,"VOL W/H CURRENT 3",payjdpm,"AMOUNT");
						QAccFld(sortfile->fileno,"VOL W/H YTD 3",payjdpm,"AMOUNT");
					} else if(!RDAstrcmp(typ4,code2) && !isEMPTY(typ4))
					{ 
						QAccFld(sortfile->fileno,"VOL W/H CURRENT 4",payjdpm,"AMOUNT");
						QAccFld(sortfile->fileno,"VOL W/H YTD 4",payjdpm,"AMOUNT");
					} else if(isEMPTY(typ4))
					{
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 4",code2);
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION TITLE 4",title);
						QAccFld(sortfile->fileno,"VOL W/H CURRENT 4",payjdpm,"AMOUNT");
						QAccFld(sortfile->fileno,"VOL W/H YTD 4",payjdpm,"AMOUNT");
					} else if(!RDAstrcmp(typ5,code2) && !isEMPTY(typ5))
					{
						QAccFld(sortfile->fileno,"VOL W/H CURRENT 5",payjdpm,"AMOUNT");
						QAccFld(sortfile->fileno,"VOL W/H YTD 5",payjdpm,"AMOUNT");
					} else if(isEMPTY(typ5))
					{
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 5",code2);
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION TITLE 5",title);
						QAccFld(sortfile->fileno,"VOL W/H CURRENT 5",payjdpm,"AMOUNT");
						QAccFld(sortfile->fileno,"VOL W/H YTD 5",payjdpm,"AMOUNT");
					} else if(!RDAstrcmp(typ6,code2) && !isEMPTY(typ6))
					{
						QAccFld(sortfile->fileno,"VOL W/H CURRENT 6",payjdpm,"AMOUNT");
						QAccFld(sortfile->fileno,"VOL W/H YTD 6",payjdpm,"AMOUNT");
					} else if(isEMPTY(typ6))
					{
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 6",code2);
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION TITLE 6",title);
						QAccFld(sortfile->fileno,"VOL W/H CURRENT 6",payjdpm,"AMOUNT");
						QAccFld(sortfile->fileno,"VOL W/H YTD 6",payjdpm,"AMOUNT");
					} else if(!RDAstrcmp(typ7,code2) && !isEMPTY(typ7))
					{
						QAccFld(sortfile->fileno,"VOL W/H CURRENT 7",payjdpm,"AMOUNT");
						QAccFld(sortfile->fileno,"VOL W/H YTD 7",payjdpm,"AMOUNT");
					} else if(isEMPTY(typ7))
					{
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 7",code2);
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION TITLE 7",title);
						QAccFld(sortfile->fileno,"VOL W/H CURRENT 7",payjdpm,"AMOUNT");
						QAccFld(sortfile->fileno,"VOL W/H YTD 7",payjdpm,"AMOUNT");
					} else if(!RDAstrcmp(typ8,code2) && !isEMPTY(typ8))
					{
						QAccFld(sortfile->fileno,"VOL W/H CURRENT 8",payjdpm,"AMOUNT");
						QAccFld(sortfile->fileno,"VOL W/H YTD 8",payjdpm,"AMOUNT");
					} else if(isEMPTY(typ8))
					{
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 8",code2);
						FINDFLDSETSTRING(sortfile->fileno,"VOL DEDUCTION TITLE 8",title);
						QAccFld(sortfile->fileno,"VOL W/H CURRENT 8",payjdpm,"AMOUNT");
						QAccFld(sortfile->fileno,"VOL W/H YTD 8",payjdpm,"AMOUNT");
					}
				}
			}
			if((print_employer_deductions==TRUE) && amt!=0.0 &&
				reduce_net==FALSE && ((lb==TRUE || ep==TRUE
				|| pay_with==TRUE || electtrans==TRUE)))
			{
				sprintf(stemp,"TOTAL EMPLOYER W/H YTD");
				QAccFld(sortfile->fileno,stemp,payjdpm,"AMOUNT");
			}
			}
			}
		}
		ef=ADVNXTNRDsec(payjdpm,2,SCRNvirtualSubData,diagrsrc);
	}
	if(empid!=NULL) Rfree(empid);
	if(empid1!=NULL) Rfree(empid1);	
	if(payid1!=NULL) Rfree(payid1);
	if(payid2!=NULL) Rfree(payid2);
	if(typ1!=NULL) Rfree(typ1);
	if(typ2!=NULL) Rfree(typ2);
	if(typ3!=NULL) Rfree(typ3);
	if(typ4!=NULL) Rfree(typ4);
	if(typ5!=NULL) Rfree(typ5);
	if(typ6!=NULL) Rfree(typ6);
	if(typ7!=NULL) Rfree(typ7);
	if(typ8!=NULL) Rfree(typ8);
}
static void getJPRM(char *empid,int jobno,RDArsrc *r)
{
	char *empid1=NULL,*payid1=NULL,deleteflag=FALSE;
	char *rateid=NULL;
	int jobno1=0;
	short ef=0;

	ZERNRD(payjprm);
	FINDFLDSETSTRING(payjprm,"PAY IDENTIFICATION",payid);
	FINDFLDSETSTRING(payjprm,"PERSONNEL IDENTIFICATION",empid);
	FINDFLDSETINT(payjprm,"JOB NUMBER",jobno);
	ef=ADVGTENRDsec(payjprm,2,SCRNvirtualSubData,r);
	while(!ef)
	{
		FINDFLDGETSTRING(payjprm,"PAY IDENTIFICATION",&payid1);
		FINDFLDGETSTRING(payjprm,"PERSONNEL IDENTIFICATION",&empid1);
		FINDFLDGETINT(payjprm,"JOB NUMBER",&jobno1);
		if(RDAstrcmp(empid,empid1) || RDAstrcmp(payid,payid1) || jobno!=jobno1) break;
		FINDFLDGETCHAR(payjprm,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDGETSTRING(payjprm,"RATE IDENTIFICATION",&rateid);
			ZERNRD(posrate);
			FINDFLDSETSTRING(posrate,"RATE IDENTIFICATION",rateid);
			if(ADVEQLNRDsec(posrate,1,SCRNvirtualSubData,r)) KEYNRD(posrate,1);
			if((strstr(rateid,"REGULAR")) || (strstr(rateid,"UNIT")))
			{
				QAccFld(sortfile->fileno,"UNIT CURRENT AMOUNT",payjprm,"GROSS");
				QAccFld(sortfile->fileno,"UNIT YTD AMOUNT",payjprm,"GROSS");
				QAccFld(sortfile->fileno,"NUMBER CURRENT UNITS",payjprm,"UNITS");
				QAccFld(sortfile->fileno,"NUMBER YTD UNITS",payjprm,"UNITS");
			} else if(strstr(rateid,"OVERTIME"))
			{
				QAccFld(sortfile->fileno,"OVER GROSS CURRENT AMOUNT",payjprm,"GROSS");
				QAccFld(sortfile->fileno,"OVER GROSS YTD AMOUNT",payjprm,"GROSS");
				QAccFld(sortfile->fileno,"OVER UNITS CURRENT AMOUNT",payjprm,"UNITS");
				QAccFld(sortfile->fileno,"OVER UNITS YTD AMOUNT",payjprm,"UNITS");
			} else if(strstr(rateid,"ADJUST"))
			{
				QAccFld(sortfile->fileno,"ADJUSTED CURRENT AMOUNT",payjprm,"GROSS");
				QAccFld(sortfile->fileno,"ADJUSTED YTD AMOUNT",payjprm,"GROSS");
			} else if(strstr(rateid,"DOCKED"))
			{
				QAccFld(sortfile->fileno,"DOCKED CUR AMOUNT",payjprm,"GROSS");
				QAccFld(sortfile->fileno,"DOCKED YTD AMOUNT",payjprm,"GROSS");
			} else { /* Gross */
				QAccFld(sortfile->fileno,"GROSS CURRENT AMOUNT",payjprm,"GROSS");
				QAccFld(sortfile->fileno,"GROSS YTD AMOUNT",payjprm,"GROSS");
			}
			if(rateid!=NULL) Rfree(rateid);
		}	
		ef=ADVNXTNRDsec(payjprm,2,SCRNvirtualSubData,r);
	}
	if(empid1!=NULL) Rfree(empid1);
	if(payid1!=NULL) Rfree(payid1);
}
static short AtLeastOneJob(char *empid,RangeScreen *rs)
{
	short retval=FALSE,ef=FALSE;
	char *empid1=NULL,deleteflag=FALSE;
	int transno=0,jobno=0;
	char *bankno1=NULL,*payid1=NULL;

	ZERNRD(payjpms);
	FINDFLDSETSTRING(payjpms,"PAY IDENTIFICATION",payid);
	FINDFLDSETSTRING(payjpms,"PERSONNEL IDENTIFICATION",empid);	
	ef=ADVGTENRDsec(payjpms,2,SCRNvirtualSubData,diagrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(payjpms,"PERSONNEL IDENTIFICATION",&empid1);
		FINDFLDGETSTRING(payjpms,"PAY IDENTIFICATION",&payid1);
		if(RDAstrcmp(empid,empid1) || RDAstrcmp(payid,payid1)) break;
		ZERNRD(payjmst);
		COPYFIELD(payjpms,payjmst,"PERSONNEL IDENTIFICATION");
		COPYFIELD(payjpms,payjmst,"JOB NUMBER");
		if(ADVEQLNRDsec(payjmst,1,SCRNvirtualSubData,diagrsrc))
		{
			KEYNRD(payjmst,1);
		}
		FINDFLDGETCHAR(payjpms,"DELETEFLAG",&deleteflag);
		FINDFLDGETSTRING(payjpms,"BANK IDENTIFICATION",&bankno1);
		FINDFLDGETINT(payjpms,"CHECK NUMBER",&transno);
		ZERNRD(posmst);
		COPYFIELD(payjmst,posmst,"POSITION IDENTIFICATION");
		if(ADVEQLNRDsec(posmst,1,SCRNvirtualSubData,diagrsrc)) KEYNRD(posmst,1);
		ZERNRD(payjcsm);
		COPYFIELD(paymst,payjcsm,"CALENDAR YEAR");
		COPYFIELD(payjpms,payjcsm,"PERSONNEL IDENTIFICATION");
		COPYFIELD(payjpms,payjcsm,"JOB NUMBER");
		if(ADVEQLNRDsec(payjcsm,1,SCRNvirtualSubData,diagrsrc)) KEYNRD(payjcsm,1);
		ZERNRD(poslmst);
		FINDFLDGETINT(payjpms,"JOB NUMBER",&jobno);
		COPYFIELD(perdmg,poslmst,"LOCATION IDENTIFICATION");
		if(ADVEQLNRDsec(poslmst,1,SCRNvirtualSubData,diagrsrc)) KEYNRD(poslmst,1);
		if(!deleteflag && !testfilerangersrc(payjmst,rs) &&
			!testfilerangersrc(payjpms,rs) && 
			!testfilerangersrc(poslmst,rs) && transno!=0 &&
			!RDAstrcmp(bankno,bankno1))
		{
			ZERNRD(sortfile->fileno);
			FINDFLDSETINT(sortfile->fileno,"CHECK NUMBER",transno);
			if(EQLNRD(sortfile->fileno,1)) 
			{
				KEYNRD(sortfile->fileno,1);
				COPYFIELD(poslmst,sortfile->fileno,"LOCATION IDENTIFICATION");
				COPYFIELD(perdmg,sortfile->fileno,"SOCIAL SECURITY NUMBER");
				if(EmployeeName!=NULL)
				{
					EmployeeName->computed=FALSE;
					computevirtual(EmployeeName,SCRNvirtualSubData,diagrsrc);
					FINDFLDSETSTRING(sortfile->fileno,"EMPLOYEE NAME",EmployeeName->value.string_value);
				}
				if(EmployeeAddr1!=NULL)
				{
					EmployeeAddr1->computed=FALSE;
					computevirtual(EmployeeAddr1,SCRNvirtualSubData,diagrsrc);
					FINDFLDSETSTRING(sortfile->fileno,"ADDRESS 1",EmployeeAddr1->value.string_value);
				} 
				if(EmployeeAddr2!=NULL)
				{
					EmployeeAddr2->computed=FALSE;
					computevirtual(EmployeeAddr2,SCRNvirtualSubData,diagrsrc);
					FINDFLDSETSTRING(sortfile->fileno,"ADDRESS 2",EmployeeAddr2->value.string_value);
				}
				if(EmployeeAddr3!=NULL)
				{
					EmployeeAddr3->computed=FALSE;
					computevirtual(EmployeeAddr3,SCRNvirtualSubData,diagrsrc);
					FINDFLDSETSTRING(sortfile->fileno,"ADDRESS 3",EmployeeAddr3->value.string_value);
				}
			} 
			COPYFIELD(payjpms,sortfile->fileno,"PERSONNEL IDENTIFICATION");
			COPYFIELD(payjpms,sortfile->fileno,"CHECK DATE");
			getJPRM(empid,jobno,diagrsrc);
			QAccFld(sortfile->fileno,"DIRDEP CUR AMOUNT",payjpms,"CHECK NET");
			QAccFld(sortfile->fileno,"DIRDEP YTD AMOUNT",payjpms,"CHECK NET");
			QAccFld(sortfile->fileno,"NET CUR AMOUNT",payjpms,"CHECK NET");
			QAccFld(sortfile->fileno,"NET YTD AMOUNT",payjpms,"CHECK NET");
			GatherCurrentDeductions();
			WRTNRD(sortfile->fileno,0,NULL);
			retval=TRUE;
			
		}
		ef=ADVNXTNRDsec(payjpms,2,SCRNvirtualSubData,diagrsrc);
	}
	if(empid1!=NULL) Rfree(empid1);
	if(payid1!=NULL) Rfree(payid1);
	return(retval);
}
static void CreateSortFile(RangeScreen *rs)
{
	short ef=0,diag_update=FALSE;
	char *empid=NULL,deleteflag=FALSE,computed=FALSE;
	char *payid1=NULL;


	diagrsrc=diagscrn(payppms,"DIAGNOSTIC SCREEN",module,
		"Selecting Transmittals",NULL);
	addDFincvir(diagrsrc,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(diagrsrc,module,"PAYJMST",NULL,payjmst);
	addDFincvir(diagrsrc,"POSTRK","POSLMST",NULL,poslmst);
	addDFincvir(diagrsrc,"POSTRK","POSMSTR",NULL,posmst);
	addDFincvir(diagrsrc,module,"PAYPPMS",NULL,payppms);
	addDFincvir(diagrsrc,module,"PAYJPMS",NULL,payjpms);
	addDFincvir(diagrsrc,module,"PAYJPRM",NULL,payjprm);
	addDFincvir(diagrsrc,module,"PAYJDPM",NULL,payjdpm);
	addDFincvir(diagrsrc,module,"PAYJRCS",NULL,payjrcs);
	addDFincvir(diagrsrc,module,"PAYJDCM",NULL,payjdcm);
	addDFincvir(diagrsrc,module,"PAYDCSM",NULL,paydcsm);
	if(diagrsrc!=NULL)
	{
		if(!APPmakescrn(diagrsrc,TRUE,NULL,NULL,FALSE))
			ForceWindowUpdate(diagrsrc);
	}
	ZERNRD(payppms);
	FINDFLDSETSTRING(payppms,"PAY IDENTIFICATION",payid);
	ef=ADVGTENRDsec(payppms,2,SCRNvirtualSubData,diagrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(payppms,"PAY IDENTIFICATION",&payid1);
		if(RDAstrcmp(payid,payid1)) break;
		diag_update=FALSE;
		FINDFLDGETCHAR(payppms,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(payppms,"BENEFITS COMPUTED",&computed);
		ZERNRD(perdmg);
		COPYFIELD(payppms,perdmg,"PERSONNEL IDENTIFICATION");
		if(ADVEQLNRDsec(perdmg,1,SCRNvirtualSubData,diagrsrc))
		{
			KEYNRD(perdmg,1);
		}
		if(!deleteflag && computed && !testfilerangersrc(payppms,rs) && !testfilerangersrc(perdmg,rs))
		{
			FINDFLDGETSTRING(perdmg,"PERSONNEL IDENTIFICATION",&empid);
			if(AtLeastOneJob(empid,rs))
			{
				diag_update=TRUE;
				DID_STUFF=TRUE;
			}
		}
		if(diagrsrc!=NULL) 
			update_diagnostic(diagrsrc,
				(diag_update==TRUE ? TRUE:FALSE));
		ef=ADVNXTNRDsec(payppms,2,SCRNvirtualSubData,diagrsrc);
	}
	if(empid!=NULL) Rfree(empid);
	if(diagrsrc!=NULL)
	{
		killwindow(diagrsrc);
		free_rsrc(diagrsrc);
		diagrsrc=NULL;
	}
}

static void selectchecks(RDArsrc *r)
{
	char *sortname=NULL,*temp1=NULL,*defaultprinter=NULL;
	RangeScreen *rs=NULL;
	short ef=FALSE;
	DFvirtual *d=NULL;
	RDArsrc *rsrc=NULL;

	readallwidgets(r);
	rs=RangeScreenNew(r);
	ReadRangeScreen(perdmg,rs);
	ReadRangeScreen(payjmst,rs);
	ReadRangeScreen(poslmst,rs);
	ReadRangeScreen(payjpms,rs);
	ReadRangeScreen(payppms,rs);

	d=getDFvirtual("PAYROLL","NAME FIRST MIDDLE LAST LINEAGE");
	if(d!=NULL)
	{
		EmployeeName=Rmalloc(sizeof(RDAvirtual));
		EmployeeName->name=stralloc(d->name);
		EmployeeName->when=0;
		EmployeeName->len=d->length;
		EmployeeName->type=d->type;
		if(d->expression!=NULL) 
		{
			EmployeeName->expression=PP_translate(d->expression);
		} else {
			EmployeeName->expression=NULL;
		}
		EmployeeName->computed=FALSE;
		EmployeeName->range=FALSE;
		EmployeeName->dlen=0;
		switch(EmployeeName->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				EmployeeName->value.string_value=NULL;
				EmployeeName->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				EmployeeName->value.string_value=Rmalloc(1);
				*EmployeeName->value.string_value=0;
				EmployeeName->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				EmployeeName->value.float_value=Rmalloc(sizeof(double));
				*EmployeeName->value.float_value=0;
				EmployeeName->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				EmployeeName->value.short_value=Rmalloc(sizeof(short));
				*EmployeeName->value.short_value=0;
				EmployeeName->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				EmployeeName->value.integer_value=Rmalloc(sizeof(int));
				*EmployeeName->value.integer_value=0;
				EmployeeName->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",EmployeeName->type,EmployeeName->name);
				break;
		}
		FreeDFvirtual(d);
	}
	d=getDFvirtual("PAYROLL","PERSONNEL MAILING ADDRESS 1");
	if(d!=NULL)
	{
		EmployeeAddr1=Rmalloc(sizeof(RDAvirtual));
		EmployeeAddr1->name=stralloc(d->name);
		EmployeeAddr1->when=0;
		EmployeeAddr1->len=d->length;
		EmployeeAddr1->type=d->type;
		if(d->expression!=NULL) 
		{
			EmployeeAddr1->expression=PP_translate(d->expression);
		} else {
			EmployeeAddr1->expression=NULL;
		}
		EmployeeAddr1->computed=FALSE;
		EmployeeAddr1->range=FALSE;
		EmployeeAddr1->dlen=0;
		switch(EmployeeAddr1->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				EmployeeAddr1->value.string_value=NULL;
				EmployeeAddr1->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				EmployeeAddr1->value.string_value=Rmalloc(1);
				*EmployeeAddr1->value.string_value=0;
				EmployeeAddr1->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				EmployeeAddr1->value.float_value=Rmalloc(sizeof(double));
				*EmployeeAddr1->value.float_value=0;
				EmployeeAddr1->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				EmployeeAddr1->value.short_value=Rmalloc(sizeof(short));
				*EmployeeAddr1->value.short_value=0;
				EmployeeAddr1->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				EmployeeAddr1->value.integer_value=Rmalloc(sizeof(int));
				*EmployeeAddr1->value.integer_value=0;
				EmployeeAddr1->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",EmployeeAddr1->type,EmployeeAddr1->name);
				break;
		}
		FreeDFvirtual(d);
	}
	d=getDFvirtual("PAYROLL","PERSONNEL MAILING ADDRESS 2");
	if(d!=NULL)
	{
		EmployeeAddr2=Rmalloc(sizeof(RDAvirtual));
		EmployeeAddr2->name=stralloc(d->name);
		EmployeeAddr2->when=0;
		EmployeeAddr2->len=d->length;
		EmployeeAddr2->type=d->type;
		if(d->expression!=NULL) 
		{
			EmployeeAddr2->expression=PP_translate(d->expression);
		} else {
			EmployeeAddr2->expression=NULL;
		}
		EmployeeAddr2->computed=FALSE;
		EmployeeAddr2->range=FALSE;
		EmployeeAddr2->dlen=0;
		switch(EmployeeAddr2->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				EmployeeAddr2->value.string_value=NULL;
				EmployeeAddr2->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				EmployeeAddr2->value.string_value=Rmalloc(1);
				*EmployeeAddr2->value.string_value=0;
				EmployeeAddr2->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				EmployeeAddr2->value.float_value=Rmalloc(sizeof(double));
				*EmployeeAddr2->value.float_value=0;
				EmployeeAddr2->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				EmployeeAddr2->value.short_value=Rmalloc(sizeof(short));
				*EmployeeAddr2->value.short_value=0;
				EmployeeAddr2->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				EmployeeAddr2->value.integer_value=Rmalloc(sizeof(int));
				*EmployeeAddr2->value.integer_value=0;
				EmployeeAddr2->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",EmployeeAddr2->type,EmployeeAddr2->name);
				break;
		}
		FreeDFvirtual(d);
	}
	d=getDFvirtual("PAYROLL","PERSONNEL MAILING ADDRESS 3");
	if(d!=NULL)
	{
		EmployeeAddr3=Rmalloc(sizeof(RDAvirtual));
		EmployeeAddr3->name=stralloc(d->name);
		EmployeeAddr3->when=0;
		EmployeeAddr3->len=d->length;
		EmployeeAddr3->type=d->type;
		if(d->expression!=NULL) 
		{
			EmployeeAddr3->expression=PP_translate(d->expression);
		} else {
			EmployeeAddr3->expression=NULL;
		}
		EmployeeAddr3->computed=FALSE;
		EmployeeAddr3->range=FALSE;
		EmployeeAddr3->dlen=0;
		switch(EmployeeAddr3->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				EmployeeAddr3->value.string_value=NULL;
				EmployeeAddr3->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				EmployeeAddr3->value.string_value=Rmalloc(1);
				*EmployeeAddr3->value.string_value=0;
				EmployeeAddr3->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				EmployeeAddr3->value.float_value=Rmalloc(sizeof(double));
				*EmployeeAddr3->value.float_value=0;
				EmployeeAddr3->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				EmployeeAddr3->value.short_value=Rmalloc(sizeof(short));
				*EmployeeAddr3->value.short_value=0;
				EmployeeAddr3->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				EmployeeAddr3->value.integer_value=Rmalloc(sizeof(int));
				*EmployeeAddr3->value.integer_value=0;
				EmployeeAddr3->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",EmployeeAddr3->type,EmployeeAddr3->name);
				break;
		}
		FreeDFvirtual(d);
	}
	d=getDFvirtual("PAYROLL","ORGANIZATIONAL ADDRESS 1");
	if(d!=NULL)
	{
		OrgAddr1=Rmalloc(sizeof(RDAvirtual));
		OrgAddr1->name=stralloc(d->name);
		OrgAddr1->when=0;
		OrgAddr1->len=d->length;
		OrgAddr1->type=d->type;
		if(d->expression!=NULL) 
		{
			OrgAddr1->expression=PP_translate(d->expression);
		} else {
			OrgAddr1->expression=NULL;
		}
		OrgAddr1->computed=FALSE;
		OrgAddr1->range=FALSE;
		OrgAddr1->dlen=0;
		switch(OrgAddr1->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				OrgAddr1->value.string_value=NULL;
				OrgAddr1->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				OrgAddr1->value.string_value=Rmalloc(1);
				*OrgAddr1->value.string_value=0;
				OrgAddr1->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				OrgAddr1->value.float_value=Rmalloc(sizeof(double));
				*OrgAddr1->value.float_value=0;
				OrgAddr1->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				OrgAddr1->value.short_value=Rmalloc(sizeof(short));
				*OrgAddr1->value.short_value=0;
				OrgAddr1->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				OrgAddr1->value.integer_value=Rmalloc(sizeof(int));
				*OrgAddr1->value.integer_value=0;
				OrgAddr1->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",OrgAddr1->type,OrgAddr1->name);
				break;
		}
		FreeDFvirtual(d);
	}
	d=getDFvirtual("PAYROLL","ORGANIZATIONAL ADDRESS 2");
	if(d!=NULL)
	{
		OrgAddr2=Rmalloc(sizeof(RDAvirtual));
		OrgAddr2->name=stralloc(d->name);
		OrgAddr2->when=0;
		OrgAddr2->len=d->length;
		OrgAddr2->type=d->type;
		if(d->expression!=NULL) 
		{
			OrgAddr2->expression=PP_translate(d->expression);
		} else {
			OrgAddr2->expression=NULL;
		}
		OrgAddr2->computed=FALSE;
		OrgAddr2->range=FALSE;
		OrgAddr2->dlen=0;
		switch(OrgAddr2->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				OrgAddr2->value.string_value=NULL;
				OrgAddr2->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				OrgAddr2->value.string_value=Rmalloc(1);
				*OrgAddr2->value.string_value=0;
				OrgAddr2->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				OrgAddr2->value.float_value=Rmalloc(sizeof(double));
				*OrgAddr2->value.float_value=0;
				OrgAddr2->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				OrgAddr2->value.short_value=Rmalloc(sizeof(short));
				*OrgAddr2->value.short_value=0;
				OrgAddr2->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				OrgAddr2->value.integer_value=Rmalloc(sizeof(int));
				*OrgAddr2->value.integer_value=0;
				OrgAddr2->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",OrgAddr2->type,OrgAddr2->name);
				break;
		}
		FreeDFvirtual(d);
	}
	d=getDFvirtual("PAYROLL","ORGANIZATIONAL ADDRESS 3");
	if(d!=NULL)
	{
		OrgAddr3=Rmalloc(sizeof(RDAvirtual));
		OrgAddr3->name=stralloc(d->name);
		OrgAddr3->when=0;
		OrgAddr3->len=d->length;
		OrgAddr3->type=d->type;
		if(d->expression!=NULL) 
		{
			OrgAddr3->expression=PP_translate(d->expression);
		} else {
			OrgAddr3->expression=NULL;
		}
		OrgAddr3->computed=FALSE;
		OrgAddr3->range=FALSE;
		OrgAddr3->dlen=0;
		switch(OrgAddr3->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				OrgAddr3->value.string_value=NULL;
				OrgAddr3->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				OrgAddr3->value.string_value=Rmalloc(1);
				*OrgAddr3->value.string_value=0;
				OrgAddr3->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				OrgAddr3->value.float_value=Rmalloc(sizeof(double));
				*OrgAddr3->value.float_value=0;
				OrgAddr3->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				OrgAddr3->value.short_value=Rmalloc(sizeof(short));
				*OrgAddr3->value.short_value=0;
				OrgAddr3->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				OrgAddr3->value.integer_value=Rmalloc(sizeof(int));
				*OrgAddr3->value.integer_value=0;
				OrgAddr3->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",OrgAddr3->type,OrgAddr3->name);
				break;
		}
		FreeDFvirtual(d);
	}
	sortname=unique_name();
	sortfile=DBsortNEW("PAYROLL",sortname,NRDRdaEngine);
	addDBfield(sortfile,"DELETEFLAG",BOOLNS,1);
	addDBfield(sortfile,"CHECK NUMBER",LONGV,8);
	addDBfield(sortfile,"LOCATION IDENTIFICATION",PLAINTEXT,15);
	addDBfield(sortfile,"CHECK DATE",DATES,10);

/* Personnel Fields */
	addDBfield(sortfile,"PERSONNEL IDENTIFICATION",PLAINTEXT,15);
	addDBfield(sortfile,"SOCIAL SECURITY NUMBER",SOCSECNUM,11);
	addDBfield(sortfile,"EMPLOYEE NAME",1,40);
	addDBfield(sortfile,"OUT OF ORDER",10,1);
	addDBfield(sortfile,"ADDRESS 1",1,40);
	addDBfield(sortfile,"ADDRESS 2",1,40);
	addDBfield(sortfile,"ADDRESS 3",1,40);

/* Job Fields */
	addDBfield(sortfile,"GROSS YTD AMOUNT",2,20);
	addDBfield(sortfile,"GROSS CURRENT AMOUNT",2,20);
	addDBfield(sortfile,"ADJUSTED YTD AMOUNT",2,20);
	addDBfield(sortfile,"ADJUSTED CURRENT AMOUNT",2,20);
	addDBfield(sortfile,"NUMBER YTD UNITS",20,10);
	addDBfield(sortfile,"NUMBER CURRENT UNITS",20,10);
	addDBfield(sortfile,"UNIT YTD AMOUNT",2,20);
	addDBfield(sortfile,"UNIT CURRENT AMOUNT",2,20);
	addDBfield(sortfile,"OVER GROSS YTD AMOUNT",2,20);
	addDBfield(sortfile,"OVER GROSS CURRENT AMOUNT",2,20);
	addDBfield(sortfile,"OVER UNITS YTD AMOUNT",20,10);
	addDBfield(sortfile,"OVER UNITS CURRENT AMOUNT",20,10);
	addDBfield(sortfile,"DOCKED YTD AMOUNT",2,20);
	addDBfield(sortfile,"DOCKED CUR AMOUNT",2,20);
	addDBfield(sortfile,"NET YTD AMOUNT",2,20);
	addDBfield(sortfile,"NET CUR AMOUNT",2,20);
	addDBfield(sortfile,"DIRDEP YTD AMOUNT",2,20);
	addDBfield(sortfile,"DIRDEP CUR AMOUNT",2,20);

/* Deduction Fields */
	addDBfield(sortfile,"FEDERAL DEDUCTION TITLE",1,8);
	addDBfield(sortfile,"FEDERAL W/H CURRENT",2,20);
	addDBfield(sortfile,"FEDERAL W/H YTD",2,20);
	addDBfield(sortfile,"FICA DEDUCTION TITLE",1,8);
	addDBfield(sortfile,"FICA W/H CURRENT",2,20);
	addDBfield(sortfile,"FICA W/H YTD",2,20);
	addDBfield(sortfile,"MEDICARE DEDUCTION TITLE",1,8);
	addDBfield(sortfile,"MEDICARE W/H CURRENT",2,20);
	addDBfield(sortfile,"MEDICARE W/H YTD",2,20);
	addDBfield(sortfile,"STATE DEDUCTION TITLE",1,8);
	addDBfield(sortfile,"STATE W/H CURRENT",2,20);
	addDBfield(sortfile,"STATE W/H YTD",2,20);
	addDBfield(sortfile,"LOCAL DEDUCTION TITLE",1,8);
	addDBfield(sortfile,"LOCAL W/H CURRENT",2,20);
	addDBfield(sortfile,"LOCAL W/H YTD",2,20);
	addDBfield(sortfile,"ST RET DEDUCTION TITLE",1,8);
	addDBfield(sortfile,"STATE RETIREMENT W/H CURRENT",2,20);
	addDBfield(sortfile,"STATE RETIREMENT W/H YTD",2,20);
	addDBfield(sortfile,"RETIREMENT DEDUCTION TITLE",1,8);
	addDBfield(sortfile,"RETIREMENT W/H CURRENT",2,20);
	addDBfield(sortfile,"RETIREMENT W/H YTD",2,20);

	addDBfield(sortfile,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 1",PLAINTEXT,30);
	addDBfield(sortfile,"VOL DEDUCTION TITLE 1",1,8);
	addDBfield(sortfile,"VOL W/H CURRENT 1",2,20);
	addDBfield(sortfile,"VOL W/H YTD 1",2,20);
	addDBfield(sortfile,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 2",PLAINTEXT,30);
	addDBfield(sortfile,"VOL DEDUCTION TITLE 2",1,8);
	addDBfield(sortfile,"VOL W/H CURRENT 2",2,20);
	addDBfield(sortfile,"VOL W/H YTD 2",2,20);
	addDBfield(sortfile,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 3",PLAINTEXT,30);
	addDBfield(sortfile,"VOL DEDUCTION TITLE 3",1,8);
	addDBfield(sortfile,"VOL W/H CURRENT 3",2,20);
	addDBfield(sortfile,"VOL W/H YTD 3",2,20);
	addDBfield(sortfile,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 4",PLAINTEXT,30);
	addDBfield(sortfile,"VOL DEDUCTION TITLE 4",1,8);
	addDBfield(sortfile,"VOL W/H CURRENT 4",2,20);
	addDBfield(sortfile,"VOL W/H YTD 4",2,20);
	addDBfield(sortfile,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 5",PLAINTEXT,30);
	addDBfield(sortfile,"VOL DEDUCTION TITLE 5",1,8);
	addDBfield(sortfile,"VOL W/H CURRENT 5",2,20);
	addDBfield(sortfile,"VOL W/H YTD 5",2,20);
	addDBfield(sortfile,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 6",PLAINTEXT,30);
	addDBfield(sortfile,"VOL DEDUCTION TITLE 6",1,8);
	addDBfield(sortfile,"VOL W/H CURRENT 6",2,20);
	addDBfield(sortfile,"VOL W/H YTD 6",2,20);
	addDBfield(sortfile,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 7",PLAINTEXT,30);
	addDBfield(sortfile,"VOL DEDUCTION TITLE 7",1,8);
	addDBfield(sortfile,"VOL W/H CURRENT 7",2,20);
	addDBfield(sortfile,"VOL W/H YTD 7",2,20);
	addDBfield(sortfile,"VOL DEDUCTION DESCRIPTION IDENTIFICATION 8",PLAINTEXT,30);
	addDBfield(sortfile,"VOL DEDUCTION TITLE 8",1,8);
	addDBfield(sortfile,"VOL W/H CURRENT 8",2,20);
	addDBfield(sortfile,"VOL W/H YTD 8",2,20);
	if(print_employer_deductions==TRUE)
	{
		addDBfield(sortfile,"TOTAL EMPLOYER W/H YTD",2,20);
	}

/* leave fields */
	addDBfield(sortfile,"VACATION BEGINNING",20,10);
	addDBfield(sortfile,"VACATION EARNED",20,10);
	addDBfield(sortfile,"VACATION USED",20,10);
	addDBfield(sortfile,"VACATION BALANCE",20,10);
	addDBfield(sortfile,"SICK BEGINNING",20,10);
	addDBfield(sortfile,"SICK EARNED",20,10);
	addDBfield(sortfile,"SICK USED",20,10);
	addDBfield(sortfile,"SICK BALANCE",20,10);
	addDBfield(sortfile,"PERSONAL BEGINNING",20,10);
	addDBfield(sortfile,"PERSONAL EARNED",20,10);
	addDBfield(sortfile,"PERSONAL USED",20,10);
	addDBfield(sortfile,"PERSONAL BALANCE",20,10);

/* Key Info */
	addDBkey(sortfile,"PRINT CHECKS KEY");
	addDBkeypart(sortfile,1,"CHECK NUMBER");
	sortfile->fileno=OPNDBsort(sortfile);
	if(sortfile->fileno==(-1))
	{
		if(temp1!=NULL) Rfree(temp1);
		temp1=Rmalloc(6+7+77+1+RDAstrlen(sortfile->filename));
		sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [PAYROLL] Filename [%s], Can Not Save Database File!",sortfile->filename);
		prterr(temp1);
		return;
	}
	FINDRSCGETSTRING(r,"[BNKACC][BANK IDENTIFICATION]",&bankno);
	FINDRSCGETSTRING(r,"[PAYMSTR][PAY IDENTIFICATION]",&payid);
	FINDRSCGETSTRING(r,"MESSAGE 1",&message1);
	FINDRSCGETSTRING(r,"MESSAGE 2",&message2);
	ZERNRD(paymst);
	FINDFLDSETSTRING(paymst,"PAY IDENTIFICATION",payid);
	if(ADVEQLNRDsec(paymst,1,SCRNvirtualSubData,r)) KEYNRD(paymst,1);
	FINDFLDGETSTRING(paymst,"END DATE",&payending);

	CreateSortFile(rs);
	if(DID_STUFF)
	{	
		diagrsrc=diagscrn(sortfile->fileno,"DIAGNOSTIC SCREEN",module,
			"Collecting Details",NULL);
		addDFincvir(diagrsrc,module,"PAYMSTR",NULL,paymst);
		addDFincvir(diagrsrc,"PRSNNL","PERDMG",NULL,perdmg);
		addDFincvir(diagrsrc,module,"PAYJMST",NULL,payjmst);
		addDFincvir(diagrsrc,module,"PAYJPMS",NULL,payjpms);
		addDFincvir(diagrsrc,module,"PAYJPRM",NULL,payjprm);
		addDFincvir(diagrsrc,module,"PAYJDPM",NULL,payjdpm);
		addDFincvir(diagrsrc,module,"PAYJDCM",NULL,payjdcm);
		addDFincvir(diagrsrc,module,"PAYDCSM",NULL,paydcsm);
		addDFincvir(diagrsrc,module,"PAYJRCS",NULL,payjrcs);
		addDFincvir(diagrsrc,"POSTRK","POSLMST",NULL,poslmst);
		addDFincvir(diagrsrc,"POSTRK","POSMSTR",NULL,posmst);
		addDFincvir(diagrsrc,module,"PAYDEDM",NULL,paydedm);
		addDFincvir(diagrsrc,module,"PAYDDMS",NULL,payddms);
		addDFincvir(diagrsrc,module,"PAYDTYP",NULL,paydtyp);
		if(XPERT_SETUP->LVSIMP==TRUE)
		{
		addDFincvir(diagrsrc,"LVSIMP","LVSMSTR",NULL,lvemst);
		addDFincvir(diagrsrc,"LVSIMP","LVSDSC",NULL,lvedsc);
		} else {
		addDFincvir(diagrsrc,"LVEMGT","LVEMSTR",NULL,lvemst);
		addDFincvir(diagrsrc,"LVEMGT","LVEDSC",NULL,lvedsc);
		}
		addDFincvir(diagrsrc,"BNKREC","BNKACC",NULL,bnkacc);
		if(diagrsrc!=NULL)
		{
			if(!APPmakescrn(diagrsrc,TRUE,NULL,NULL,FALSE))
				ForceWindowUpdate(diagrsrc);
		}
		ZERNRD(sortfile->fileno);
		ef=FRSNRD(sortfile->fileno,1);
		while(!ef)
		{
			GatherHistory();
			if(XPERT_SETUP->LVEMGT==TRUE) GatherLeave();	
			else if(XPERT_SETUP->LVSIMP==TRUE) GatherLeaveSimple();
			GatherDeductions();
			WRTNRD(sortfile->fileno,0,NULL);
			if(diagrsrc!=NULL) update_diagnostic(diagrsrc,TRUE);
			ef=NXTNRD(sortfile->fileno,1);
		}
		if(OrgAddr1!=NULL)
		{
			OrgAddr1->computed=FALSE;
			computevirtual(OrgAddr1,SCRNvirtualSubData,diagrsrc);
		}
		if(OrgAddr2!=NULL)
		{
			OrgAddr2->computed=FALSE;
			computevirtual(OrgAddr2,SCRNvirtualSubData,diagrsrc);
		}
		if(OrgAddr3!=NULL)
		{
			OrgAddr3->computed=FALSE;
			computevirtual(OrgAddr3,SCRNvirtualSubData,diagrsrc);
		}
		if(diagrsrc!=NULL)
		{
			killwindow(diagrsrc);
			free_rsrc(diagrsrc);
			diagrsrc=NULL;
		}

		rsrc=RDArsrcNEW("PAYROLL","PRINT CHECKS OUTPUT DEVICE SCREEN");
		defaultprinter=DefaultPrinter();
		addbtnrsrc(rsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncOutputDevice);
		addstdrsrc(rsrc,"OUTPUT DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
		if(defaultprinter!=NULL) Rfree(defaultprinter);
		
		addrfcbrsrc(rsrc,"ALIGNMENT",TRUE,alignment,NULL);
		if(check_formfeed)
		{
			FINDRSCSETEDITABLE(rsrc,"ALIGNMENT",FALSE);
			FINDRSCSETSENSITIVE(rsrc,"ALIGNMENT",FALSE);
		}
		addrfcbrsrc(rsrc,"PRINT",TRUE,printchecks,r);
		addrfcbrsrc(rsrc,"QUIT",TRUE,quitprint,r);
		addrfcbrsrc(rsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(rsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		addbtnrsrc(rsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);

		APPmakescrn(rsrc,TRUE,quitprint,r,TRUE);

	} else {
		prterr("Warning:  No Transmittals were selected using the given criteria.");
		if(sortfile!=NULL)
		{
			CLSDBsort(sortfile);
			FreeDBsort(sortfile);
			sortfile=NULL;
		}
	}
	if(rs!=NULL) FreeRangeScreen(rs);
	if(sortname!=NULL) Rfree(sortname);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,module,"PRINT PAYROLL CHECKS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((paymst=APPOPNNRD(module,"PAYMSTR",TRUE,FALSE))==(-1)) return;
	if((payppms=APPOPNNRD(module,"PAYPPMS",TRUE,FALSE))==(-1)) return;
	if((payjpms=APPOPNNRD(module,"PAYJPMS",TRUE,FALSE))==(-1)) return;
	if((payjprm=APPOPNNRD(module,"PAYJPRM",TRUE,FALSE))==(-1)) return;
	if((payjdpm=APPOPNNRD(module,"PAYJDPM",TRUE,FALSE))==(-1)) return;
	if((payjdcm=APPOPNNRD(module,"PAYJDCM",TRUE,FALSE))==(-1)) return;
	if((paydcsm=APPOPNNRD(module,"PAYDCSM",TRUE,FALSE))==(-1)) return;
	if((payjcsm=APPOPNNRD(module,"PAYJCSM",TRUE,FALSE))==(-1)) return;
	if((payjrcs=APPOPNNRD(module,"PAYJRCS",TRUE,FALSE))==(-1)) return;
	if((perdmg=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((payjmst=APPOPNNRD(module,"PAYJMST",TRUE,FALSE))==(-1)) return;
	if((paydedm=APPOPNNRD(module,"PAYDEDM",TRUE,FALSE))==(-1)) return;
	if((payddms=APPOPNNRD(module,"PAYDDMS",TRUE,FALSE))==(-1)) return;
	if((paydtyp=APPOPNNRD(module,"PAYDTYP",TRUE,FALSE))==(-1)) return;
	if((poslmst=APPOPNNRD("POSTRK","POSLMST",TRUE,FALSE))==(-1)) return;
	if((posrate=APPOPNNRD("POSTRK","POSRATE",TRUE,FALSE))==(-1)) return;
	if((posmst=APPOPNNRD("POSTRK","POSMSTR",TRUE,FALSE))==(-1)) return;
	if((bnkacc=APPOPNNRD("BNKREC","BNKACC",TRUE,FALSE))==(-1)) return;
	if(XPERT_SETUP->LVEMGT==TRUE && skip_leave==FALSE)
	{
		if((lvemst=APPOPNNRD("LVEMGT","LVEMSTR",TRUE,FALSE))==(-1)) return;
		if((lvedsc=APPOPNNRD("LVEMGT","LVEDSC",TRUE,FALSE))==(-1)) return;
	} else if(XPERT_SETUP->LVSIMP==TRUE && skip_leave==FALSE)
	{
		if((lvemst=APPOPNNRD("LVSIMP","LVSMSTR",TRUE,FALSE))==(-1)) return;
		if((lvedsc=APPOPNNRD("LVSIMP","LVSDSC",TRUE,FALSE))==(-1)) return;
	} else {
		lvemst=(-1);
		lvedsc=(-1);
	}

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew(module,"PAYCHECK TYPE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			check_type=*gsv->value.short_value;
		} else {
			check_type=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"FORM FEED CHECKS");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			check_formfeed=(*gsv->value.string_value ? TRUE:FALSE);
		} else {
			check_formfeed=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"PAYCHECK ADDRESS OFFSET");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			ADDRESS_OFFSET=*gsv->value.short_value;
		} else {
			ADDRESS_OFFSET=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"PAY CHECK SIGNATURE TYPE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			check_signature=*gsv->value.short_value;
		} else {
			check_signature=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"USE SIGNATURE LOCKS");
	if(gsv!=NULL)
	{
		use_sign_locks=FALSE;
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			if(*gsv->value.string_value==TRUE) use_sign_locks=TRUE;
				else use_sign_locks=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"TOP FED LASER");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			top_feeder=(*gsv->value.string_value ? TRUE:FALSE);
		} else {
			top_feeder=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"SKIP LEAVE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			skip_leave=(*gsv->value.string_value ? TRUE:FALSE);
		} else {
			skip_leave=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"PRINT EMPLOYER PAID DEDUCTIONS");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			print_employer_deductions=(*gsv->value.string_value ? TRUE:FALSE);
		} else {
			print_employer_deductions=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"CHECK OFFSET");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			pay_check_offset=*gsv->value.short_value;
		} else {
			pay_check_offset=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"PAY CHECK SPELLED AMOUNT POSITION");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			spelled_pos=*gsv->value.short_value;
		} else {
			spelled_pos=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"DISPLAY SSN");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			display_ssn=(*gsv->value.string_value ? TRUE:FALSE);
		} else {
			display_ssn=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("PAYROLL","DEFAULT BANK IDENTIFICATION");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			defbankid=stralloc(gsv->value.string_value);
		} else {
			defbankid=NULL;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	mainrsrc=RDArsrcNEW(module,"PRINT PAYROLL CHECKS");
	addDFincvir(mainrsrc,"BNKREC","BNKACC",NULL,bnkacc);
	if(bnkacc!=(-1)) file2rsrc(bnkacc,mainrsrc,FALSE);
	addDFincvir(mainrsrc,module,"PAYMSTR",NULL,paymst);
	if(paymst!=(-1)) file2rsrc(paymst,mainrsrc,FALSE);
	FINDRSCSETEDITABLE(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",TRUE);
	FINDRSCSETEDITABLE(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]",TRUE);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	addDFincvir(mainrsrc,module,"PAYPPMS",NULL,payppms);
	addDFincvir(mainrsrc,module,"PAYJPMS",NULL,payjpms);
	addDFincvir(mainrsrc,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(mainrsrc,module,"PAYJMST",NULL,payjmst);
	addDFincvir(mainrsrc,"POSTRK","POSLMST",NULL,poslmst);
	GET_SCREEN_VIRTUALS(mainrsrc,2);
	if(payjpms!=(-1)) file2rangersrc(payjpms,mainrsrc);
	if(payppms!=(-1)) file2rangersrc(payppms,mainrsrc);
	if(perdmg!=(-1)) file2rangersrc(perdmg,mainrsrc);
	if(payjmst!=(-1)) file2rangersrc(payjmst,mainrsrc);
	if(poslmst!=(-1)) file2rangersrc(poslmst,mainrsrc);
	addstdrsrc(mainrsrc,"MESSAGE 1",PLAINTEXT,40,NULL,TRUE);
	addstdrsrc(mainrsrc,"MESSAGE 2",PLAINTEXT,40,NULL,TRUE);
	SCRNvirtual2rangersrc(mainrsrc);
	DefaultScreens(mainrsrc);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,selectchecks,NULL);
	addrfcbrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	rsrc2filerecord(bnkacc,mainrsrc);
	rsrc2filerecord(paymst,mainrsrc);
	SetRangersrcsensitive(mainrsrc);
	ReadRDAScrolledLists(mainrsrc);
	if(!isEMPTY(defbankid))
	{
		FINDRSCSETSTRING(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",defbankid);
		FINDFLDSETSTRING(bnkacc,"BANK IDENTIFICATION",defbankid);
	} else {
		FINDRSCGETSTRING(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",&bankno);
		FINDFLDSETSTRING(bnkacc,"BANK IDENTIFICATION",bankno);
	}
	filerecord2rsrc(bnkacc,mainrsrc);
	filerecord2rsrc(paymst,mainrsrc);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mainrsrc);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
	ReadRDAScrolledLists(mainrsrc);
	RDAAPPMAINLOOP();
}
