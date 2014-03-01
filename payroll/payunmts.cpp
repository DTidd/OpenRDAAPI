/* payunmts.c - Payroll's Undo Make Transactions */
#ifndef WIN32
#define __NAM__ "payunmts.lnx"
#endif
#ifdef WIN32
#define __NAM__ "payunmts.exe"
#endif
#include <app.hpp>

#include <mix.hpp>
#include <fin.hpp>
#include <ldval.hpp>
#include <pay.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static char *module="PAYROLL",*PAYID=NULL;
static char WROTE_RECORDS=FALSE,AUTO_UNDO=FALSE;
static short payjpms=(-1),paymstr=(-1),payddac=(-1);
static short payppms=(-1),eyrnum=(-1),byrnum=(-1);
static short payjtrn=0,paydtrn=0,payjmst=0,posmstr=0,perdmg=0;
static RDArsrc *mrsrc=NULL;
RDApayroll *PAYROLL_SETUP=NULL;

static char ClearedPAYJTRN(char *payid,char *perid)
{
	short ef=0;
	char *payid1=NULL,*perid1=NULL,cleared=FALSE,delflag=FALSE;

	ZERNRD(payjtrn);
	FINDFLDSETSTRING(payjtrn,"PAY IDENTIFICATION",payid);
	FINDFLDSETSTRING(payjtrn,"PERSONNEL IDENTIFICATION",perid);
	ef=GTENRD(payjtrn,2);
	while(!ef)
	{
		FINDFLDGETSTRING(payjtrn,"PAY IDENTIFICATION",&payid1);
		FINDFLDGETSTRING(payjtrn,"PERSONNEL IDENTIFICATION",&perid1);
		if(RDAstrcmp(payid,payid1) || RDAstrcmp(perid,perid1)) break;
		FINDFLDGETCHAR(payjtrn,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			FINDFLDGETCHAR(payjtrn,"CLEARED",&cleared);
			if(cleared==TRUE) break;
		}
		ef=NXTNRD(payjtrn,2);
	}
	if(payid1!=NULL) Rfree(payid1);
	if(perid1!=NULL) Rfree(perid1);
	return(cleared);
}
static void okbpaymstr(MakeBrowseList *b,void *targetkey)
{
	RDArsrc *diagrsrc=NULL;
	short pmonth=0,ef=FALSE,upd_diag=FALSE,keyno=0,deftype=0;
	short pdeftype=0,testfile=FALSE,use_payroll_fund=0;
	int fiscal_year=0,x;
	int jobno=0,transno=0;
	char deleteflag=FALSE,*perid=NULL,*lperid=NULL;
	char *payid=NULL,*lpayid=NULL,*e=NULL;
	char expensed=FALSE,liability=FALSE;
	char *posid=NULL,*dedid=NULL,*message=NULL,distflag=FALSE,clearflag=FALSE;
	char *enc_code=NULL,*reserve_code=NULL;
	double amount=0.0,total=0.0,orgtotal=0.0,t=0.0;
	RangeScreen *rs=NULL;
	RDAvirtual *v=NULL;
	RDATData *prev_tran=NULL,*prev_file=NULL,*prev=NULL;

	WROTE_RECORDS=FALSE;
	ZERNRD(paymstr);
	if(AUTO_UNDO)
	{
		FINDFLDSETSTRING(paymstr,"PAY IDENTIFICATION",PAYID);
		if(ADVEQLNRDsec(paymstr,1,SCRNvirtualSubData,mrsrc)) 
		{
			KEYNRD(paymstr,1);
			prterr("Error:  Pay Identification [%s] not found.",(PAYID!=NULL ? PAYID:""));TRACE;
			return;
		}
	} else {
	if(ADVEQLNRDKEYsec(paymstr,1,targetkey,SCRNvirtualSubData,b->mainrsrc)) KEYNRD(paymstr,1);
	}
	FINDFLDGETSTRING(paymstr,"PAY IDENTIFICATION",&payid);
	FINDFLDGETINT(paymstr,"FISCAL YEAR",&fiscal_year);
	FINDFLDGETSHORT(paymstr,"FISCAL MONTH",&pmonth);
	if(!AUTO_UNDO)
	{
	mrsrc=(RDArsrc *)b->funcargs;
	rs=RangeScreenNew(mrsrc);
	ReadRangeScreen(payjmst,rs);
	ReadRangeScreen(perdmg,rs);
	ReadRangeScreen(posmstr,rs);
        if(mrsrc->virflds!=NULL)
        {
                for(x=0,v=mrsrc->virflds;x<mrsrc->num;++x,++v)
                {
                        ReadVirtualRangeStruct(v,rs);
                }
        }
	}
	diagrsrc=diagscrn(paydtrn,"DIAGNOSTIC SCREEN",module,
		"Removing Payroll Deduction Transactions.....",NULL);
	addDFincvir(diagrsrc,module,"PAYJMST",NULL,payjmst);
	addDFincvir(diagrsrc,module,"PAYPPMS",NULL,payppms);
	addDFincvir(diagrsrc,module,"PAYJPMS",NULL,payjpms);
	addDFincvir(diagrsrc,module,"PAYMSTR",NULL,paymstr);
	addDFincvir(diagrsrc,module,"PAYDDAC",NULL,payddac);
	addDFincvir(diagrsrc,module,"PAYJTRN",NULL,payjtrn);
	addDFincvir(diagrsrc,module,"PAYDTRN",NULL,paydtrn);
	addDFincvir(diagrsrc,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(diagrsrc,"POSTRK","POSMSTR",NULL,posmstr);
	addDFincvir(diagrsrc,"FINMGT","FINEYR",NULL,eyrnum);
	addDFincvir(diagrsrc,"FINMGT","FINBYR",NULL,byrnum);
	GET_ALL_SCREEN_VIRTUALS(diagrsrc,0);
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
	if(perid!=NULL) Rfree(perid);
	perid=NULL;
	keyno=KEYNUMBER(paydtrn,"RDA PAYDTRN KEY");
	if(keyno<1) keyno=1;
	ZERNRD(paydtrn);
	FINDFLDSETSTRING(paydtrn,"PAY IDENTIFICATION",payid);
	ef=ADVGTENRDsec(paydtrn,keyno,SCRNvirtualSubData,diagrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(paydtrn,"PAY IDENTIFICATION",&lpayid);
		if(RDAstrcmp(payid,lpayid)) break;
		FINDFLDGETCHAR(paydtrn,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(paydtrn,"DISTRIBUTED",&distflag);
		if(!deleteflag && !distflag)
		{
			LOCNRDFILE(paydtrn);
			prev_tran=RDATDataNEW(paydtrn);
			FINDFLDGETSTRING(paydtrn,"PERSONNEL IDENTIFICATION",&lperid);
			FINDFLDSETSTRING(perdmg,"PERSONNEL IDENTIFICATION",lperid);
			if(ADVEQLNRDsec(perdmg,1,SCRNvirtualSubData,diagrsrc)) KEYNRD(perdmg,1);
			if(!AUTO_UNDO)
			{
				testfile=testfilerangersrc(perdmg,rs);
			} else testfile=FALSE;
			clearflag=ClearedPAYJTRN(payid,lperid);
			if(!testfile && !clearflag)
			{
			FINDFLDSETSTRING(payjmst,"PERSONNEL IDENTIFICATION",lperid);
			FINDFLDGETINT(paydtrn,"JOB NUMBER",&jobno);
			FINDFLDSETINT(payjmst,"JOB NUMBER",jobno);
/*
			COPYFIELD(paydtrn,payjmst,"JOB NUMBER");
*/
/* Error found and commented out and fixed by sjs 10/31/1996 */
/*
			if(ADVEQLNRDsec(payjmst,1,SCRNvirtualSubData,diagrsrc)) KEYNRD(payjtrn,1);
*/
			if(ADVEQLNRDsec(payjmst,1,SCRNvirtualSubData,diagrsrc)) KEYNRD(payjmst,1);
			if(!AUTO_UNDO)
			{
				testfile=testfilerangersrc(payjmst,rs);
			} else testfile=FALSE;
			
			if(!testfile)
			{
			FINDFLDGETSTRING(payjmst,"POSITION IDENTIFICATION",&posid);
			FINDFLDSETSTRING(posmstr,"POSITION IDENTIFICATION",posid);
/*
			COPYFIELD(payjmst,posmstr,"POSITION IDENTIFICATION");
*/
			if(ADVEQLNRDsec(posmstr,1,SCRNvirtualSubData,diagrsrc)) KEYNRD(posmstr,1);
			if(!AUTO_UNDO)
			{
				testfile=testfilerangersrc(posmstr,rs);
			} else testfile=FALSE;
			if(!testfile)
			{
				if(!AUTO_UNDO)
				{
					for(x=0,v=mrsrc->virflds;x<mrsrc->num;++x,++v)
					{
						if(ADVtestvirtualrangersrc(v,rs,(void (*)(...))SCRNvirtualSubData,mrsrc)) break;
					}
				} else x=mrsrc->num+1;
				if(x>=mrsrc->num)
				{
					if(RDAstrcmp(perid,lperid))
					{
						FINDFLDSETSTRING(payppms,"PERSONNEL IDENTIFICATION",lperid);
						FINDFLDSETSTRING(payppms,"PAY IDENTIFICATION",payid);
						if(ADVEQLNRDsec(payppms,1,SCRNvirtualSubData,diagrsrc)) 
						{
							KEYNRD(payppms,1);
						} else {
							LOCNRD(payppms);
							prev_file=RDATDataNEW(payppms);
						}
						FINDFLDGETCHAR(payppms,"MADE TRANSACTIONS",&deleteflag);
						if(deleteflag==TRUE)
						{
							deleteflag=FALSE;
							FINDFLDSETCHAR(payppms,"MADE TRANSACTIONS",deleteflag);
							ADVWRTTRANSsec(payppms,0,NULL,prev_file,SCRNvirtualSubData,diagrsrc);
						}
						if(prev_file!=NULL) FreeRDATData(prev_file);
						if(perid!=NULL) Rfree(perid);
						perid=stralloc(lperid);
					}
					FINDFLDGETDOUBLE(paydtrn,"AMOUNT",&amount);
					amount=amount*(-1);
					deleteflag=TRUE;
					FINDFLDSETCHAR(paydtrn,"DELETEFLAG",deleteflag);
					ADVWRTTRANSsec(paydtrn,0,NULL,prev_tran,SCRNvirtualSubData,diagrsrc);
					ZERNRD(paymstr);
					FINDFLDSETSTRING(paymstr,"PAY IDENTIFICATION",payid);
					if(ADVEQLNRDsec(paymstr,1,SCRNvirtualSubData,diagrsrc))
					{
						prev=NULL;
						KEYNRD(paymstr,1);
					} else prev=RDATDataNEW(paymstr);
					FINDFLDGETDOUBLE(paymstr,"TOTAL PAYMTS",&t);
					t+=amount;
					FINDFLDSETDOUBLE(paymstr,"TOTAL PAYMTS",t);
					ADVWRTTRANSsec(paymstr,0,NULL,prev,SCRNvirtualSubData,diagrsrc);
					if(prev!=NULL) FreeRDATData(prev);
					WROTE_RECORDS=TRUE;
					FINDFLDGETCHAR(paydtrn,"EXPENSED",&expensed);
					FINDFLDGETCHAR(paydtrn,"LIABILITY",&liability);
					FINDFLDGETSTRING(paydtrn,"ENCUMBRANCE CODE",&enc_code);
					FINDFLDGETSTRING(paydtrn,"RESERVE CODE",&reserve_code);
					FINDFLDGETSHORT(paydtrn,"DEFINITION TYPE",&deftype);
					FINDFLDGETSHORT(paydtrn,"PAYROLL FUND DEFINITION TYPE",&pdeftype);
					FINDFLDGETSHORT(paydtrn,"USE PAYROLL FUND TYPE",&use_payroll_fund);
					if((!isEMPTY(enc_code)) && (!isEMPTY(reserve_code)))
					{
						ADVupdateundist(paydtrn,"ACCOUNT CODE","ENCUMBRANCE CODE","RESERVE CODE",NULL,eyrnum,byrnum,fiscal_year,deftype,"ENCUMBRANCES",FISCALPERIOD[pmonth],-amount,SCRNvirtualSubData,diagrsrc);
					}
					if(expensed)
					{
						ADVupdateundist(paydtrn,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,eyrnum,byrnum,fiscal_year,deftype,"EXPENDITURES",FISCALPERIOD[pmonth],amount,SCRNvirtualSubData,diagrsrc);
					}
					if(diagapps)
					{
						prterr("Backing out [PAYROLL][PAYDTRN] of [%s],[%s],[%s],[%d],[%d] for [$ %f].",(payid!=NULL?payid:""),(lperid!=NULL?lperid:""),(dedid!=NULL?dedid:""),jobno,transno,(amount/100));
					}
					if(liability)
					{
						if(use_payroll_fund==2)
						{
							ADVupdateundist(paydtrn,NULL,"DUE FROM","LIABILITY CODE",NULL,(-1),byrnum,fiscal_year,pdeftype,"EXPENDITURES",FISCALPERIOD[pmonth],amount,SCRNvirtualSubData,diagrsrc);
						} else {
							ADVupdateundist(paydtrn,NULL,"CASH CODE","LIABILITY CODE",NULL,(-1),byrnum,fiscal_year,pdeftype,"EXPENDITURES",FISCALPERIOD[pmonth],amount,SCRNvirtualSubData,diagrsrc);
						}
						FINDFLDGETSTRING(paydtrn,"DEDUCTION DESCRIPTION IDENTIFICATION",&dedid);
						FINDFLDSETSTRING(payddac,"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
/*
						COPYFIELD(paydtrn,payddac,"DEDUCTION DESCRIPTION IDENTIFICATION");
*/
						FINDFLDSETSHORT(payddac,"DEFINITION TYPE",deftype);
						if(ADVEQLNRDsec(payddac,1,SCRNvirtualSubData,diagrsrc)) 
						{
							KEYNRD(payddac,1);
							total=0;
						} else {
							prev_file=RDATDataNEW(payddac);
							FINDFLDGETDOUBLE(payddac,"AMOUNT",&total);
						}
						orgtotal=total;
						total+=amount;
						FINDFLDGETINT(paydtrn,"ACCOUNT NUMBER",&transno);
						if(diagapps)
						{
							prterr("Backing out [PAYROLL][PAYDDAC] for Deduction ID [%s], Definition Type [%d], changed from [$ %f] to [$ %f].",(dedid!=NULL?dedid:""),deftype,(orgtotal/100),(total/100));
						}
						FINDFLDSETDOUBLE(payddac,"AMOUNT",total);
						ADVWRTTRANSsec(payddac,0,NULL,prev_file,SCRNvirtualSubData,diagrsrc);
						WROTE_RECORDS=TRUE;
						if(prev_file!=NULL) FreeRDATData(prev_file);
					}
					upd_diag=TRUE;
				} else {
					if(diagapps)
					{
						prterr("Skipped by Ranging on Virtual Field [%s].",(v->name!=NULL?v->name:""));
					}
					upd_diag=FALSE;
				}
			} else {
				if(diagapps)
				{
					prterr("Skipped by Ranging on [POSTRK][POSMSTR] file fields, POSITION IDENTIFICATION=[%s].",(posid!=NULL?posid:""));
				}
				upd_diag=FALSE;
			}
			} else {
				if(diagapps)
				{
					prterr("Skipped by Ranging on [PAYROLL][PAYJMST] file fields, PERSONNEL IDENTIFICATION=[%s], JOB NUMBER=[%d].",(lperid!=NULL?lperid:""),jobno);
				}
				upd_diag=FALSE;
			}
			} else {
				if(diagapps)
				{
					prterr("Skipped by Ranging on [PRSNNL][PERDMG] file fields, PERSONNEL IDENTIFICATION=[%s].",(lperid!=NULL?lperid:""));
				}
				upd_diag=FALSE;
			}
			UNLNRDFILE(paydtrn);
			if(prev_tran!=NULL) FreeRDATData(prev_tran);
		}
		CLEAR_SCREEN_VIRTUAL(mrsrc);
		if(diagrsrc!=NULL) update_diagnostic(diagrsrc,(upd_diag ? TRUE:FALSE));
		ef=ADVNXTNRDsec(paydtrn,keyno,SCRNvirtualSubData,diagrsrc);
	}
	if(dedid!=NULL) Rfree(dedid);
	if(reserve_code!=NULL) Rfree(reserve_code);
	if(enc_code!=NULL) Rfree(enc_code);
	if(diagrsrc!=NULL)
	{
		killwindow(diagrsrc);
		free_rsrc(diagrsrc);
	}	
	diagrsrc=diagscrn(payjtrn,"DIAGNOSTIC SCREEN",module,
		"Removing Payroll Job Transactions.....",NULL);
	addDFincvir(diagrsrc,module,"PAYJMST",NULL,payjmst);
	addDFincvir(diagrsrc,module,"PAYPPMS",NULL,payppms);
	addDFincvir(diagrsrc,module,"PAYJPMS",NULL,payjpms);
	addDFincvir(diagrsrc,module,"PAYMSTR",NULL,paymstr);
	addDFincvir(diagrsrc,module,"PAYDDAC",NULL,payddac);
	addDFincvir(diagrsrc,module,"PAYJTRN",NULL,payjtrn);
	addDFincvir(diagrsrc,module,"PAYDTRN",NULL,paydtrn);
	addDFincvir(diagrsrc,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(diagrsrc,"POSTRK","POSMSTR",NULL,posmstr);
	addDFincvir(diagrsrc,"FINMGT","FINEYR",NULL,eyrnum);
	addDFincvir(diagrsrc,"FINMGT","FINBYR",NULL,byrnum);
	GET_ALL_SCREEN_VIRTUALS(diagrsrc,0);
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
	if(perid!=NULL) Rfree(perid);
	perid=NULL;
	keyno=KEYNUMBER(payjtrn,"RDA PAYJTRN KEY");
	if(keyno<1) keyno=1;
	ZERNRD(payjtrn);
	FINDFLDSETSTRING(payjtrn,"PAY IDENTIFICATION",payid);
	ef=ADVGTENRDsec(payjtrn,keyno,SCRNvirtualSubData,diagrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(payjtrn,"PAY IDENTIFICATION",&lpayid);
		if(RDAstrcmp(payid,lpayid)) break;
		FINDFLDGETCHAR(payjtrn,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(payjtrn,"DISTRIBUTED",&distflag);
		FINDFLDGETCHAR(payjtrn,"CLEARED",&clearflag);
		if(!deleteflag && !distflag && !clearflag)
		{
			LOCNRDFILE(payjtrn);
			prev_tran=RDATDataNEW(payjtrn);
			FINDFLDGETSTRING(payjtrn,"PERSONNEL IDENTIFICATION",&lperid);
			FINDFLDSETSTRING(perdmg,"PERSONNEL IDENTIFICATION",lperid);
			if(ADVEQLNRDsec(perdmg,1,SCRNvirtualSubData,diagrsrc)) KEYNRD(perdmg,1);
			if(!AUTO_UNDO)
			{
				testfile=testfilerangersrc(perdmg,rs);
			} else testfile=FALSE;
			if(!testfile)
			{
			FINDFLDSETSTRING(payjmst,"PERSONNEL IDENTIFICATION",lperid);
			FINDFLDGETINT(payjtrn,"JOB NUMBER",&jobno);
			FINDFLDSETINT(payjmst,"JOB NUMBER",jobno);
/*
			COPYFIELD(payjtrn,payjmst,"JOB NUMBER");
*/
/* Error found and fixed by sjs on 10/31/96 */
/*
			if(ADVEQLNRDsec(payjmst,1,SCRNvirtualSubData,diagrsrc)) KEYNRD(payjtrn,1);
*/
			if(ADVEQLNRDsec(payjmst,1,SCRNvirtualSubData,diagrsrc)) KEYNRD(payjmst,1);
			if(!AUTO_UNDO)
			{
				testfile=testfilerangersrc(payjmst,rs);
			} else testfile=FALSE;
			if(!testfile)
			{
			FINDFLDGETSTRING(payjmst,"POSITION IDENTIFICATION",&posid);
			FINDFLDSETSTRING(posmstr,"POSITION IDENTIFICATION",posid);
/*
			COPYFIELD(payjmst,posmstr,"POSITION IDENTIFICATION");
*/
			if(ADVEQLNRDsec(posmstr,1,SCRNvirtualSubData,diagrsrc)) KEYNRD(posmstr,1);
			if(!AUTO_UNDO)
			{
				testfile=testfilerangersrc(posmstr,rs);
			} else testfile=FALSE;
			if(!testfile)
			{
				for(x=0,v=mrsrc->virflds;x<mrsrc->num;++x,++v)
				{
					if(ADVtestvirtualrangersrc(v,rs,(void (*)(...))SCRNvirtualSubData,mrsrc)) break;
				}
				if(x>=mrsrc->num)
				{
					if(RDAstrcmp(perid,lperid))
					{
						FINDFLDSETSTRING(payppms,"PERSONNEL IDENTIFICATION",lperid);
						FINDFLDSETSTRING(payppms,"PAY IDENTIFICATION",payid);
						if(ADVEQLNRDsec(payppms,1,SCRNvirtualSubData,diagrsrc)) 
						{
							KEYNRD(payppms,1);
						} else {
							LOCNRD(payppms);
							prev_file=RDATDataNEW(payppms);
						}
						FINDFLDGETCHAR(payppms,"MADE TRANSACTIONS",&deleteflag);
						if(deleteflag==TRUE)
						{
							deleteflag=FALSE;
							FINDFLDSETCHAR(payppms,"MADE TRANSACTIONS",deleteflag);
							ADVWRTTRANSsec(payppms,0,NULL,prev_file,SCRNvirtualSubData,diagrsrc);
						}
						WROTE_RECORDS=TRUE;
						if(prev_file!=NULL) FreeRDATData(prev_file);
						if(perid!=NULL) Rfree(perid);
						perid=stralloc(lperid);
					}
					deleteflag=TRUE;
					FINDFLDSETCHAR(payjtrn,"DELETEFLAG",deleteflag);
					FINDFLDGETDOUBLE(payjtrn,"AMOUNT",&amount);
					FINDFLDGETINT(payjtrn,"ACCOUNT NUMBER",&transno);
					amount=amount*(-1);
					if(diagapps)
					{
						prterr("Backing out [PAYROLL][PAYJTRN] of [%s],[%s],[%d],[%d] for [$ %f].",(payid!=NULL?payid:""),(lperid!=NULL?lperid:""),jobno,transno,(amount/100));
					}
					FINDFLDGETSHORT(payjtrn,"DEFINITION TYPE",&deftype);
					FINDFLDGETSHORT(payjtrn,"PAYROLL FUND DEFINITION TYPE",&pdeftype);
					FINDFLDGETSTRING(payjtrn,"ENCUMBRANCE CODE",&enc_code);
					FINDFLDGETSTRING(payjtrn,"RESERVE CODE",&reserve_code);
					FINDFLDGETSHORT(payjtrn,"USE PAYROLL FUND TYPE",&use_payroll_fund);
					ADVWRTTRANSsec(payjtrn,0,NULL,prev_tran,SCRNvirtualSubData,diagrsrc);
					WROTE_RECORDS=TRUE;
					ZERNRD(paymstr);
					FINDFLDSETSTRING(paymstr,"PAY IDENTIFICATION",payid);
					if(ADVEQLNRDsec(paymstr,1,SCRNvirtualSubData,diagrsrc))
					{
						prev=NULL;
						KEYNRD(paymstr,1);
					} else prev=RDATDataNEW(paymstr);
					FINDFLDGETDOUBLE(paymstr,"TOTAL PAYMTS",&t);
					t+=amount;
					FINDFLDSETDOUBLE(paymstr,"TOTAL PAYMTS",t);
					ADVWRTTRANSsec(paymstr,0,NULL,prev,SCRNvirtualSubData,diagrsrc);
					if(prev!=NULL) FreeRDATData(prev);
					if((!isEMPTY(enc_code)) && (!isEMPTY(reserve_code)))
					{
						ADVupdateundist(payjtrn,"ACCOUNT CODE","ENCUMBRANCE CODE","RESERVE CODE",NULL,eyrnum,byrnum,fiscal_year,deftype,"ENCUMBRANCES",FISCALPERIOD[pmonth],-amount,SCRNvirtualSubData,diagrsrc);
					}
					ADVupdateundist(payjtrn,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,eyrnum,byrnum,fiscal_year,deftype,"EXPENDITURES",FISCALPERIOD[pmonth],amount,SCRNvirtualSubData,diagrsrc);
					if(use_payroll_fund==2)
					{
						ADVupdateundist(payjtrn,NULL,"DUE FROM","PAYABLE CODE",NULL,eyrnum,byrnum,fiscal_year,pdeftype,"EXPENDITURES",FISCALPERIOD[pmonth],amount,SCRNvirtualSubData,diagrsrc);
					} else {
						ADVupdateundist(payjtrn,NULL,"CASH CODE","PAYABLE CODE",NULL,eyrnum,byrnum,fiscal_year,pdeftype,"EXPENDITURES",FISCALPERIOD[pmonth],amount,SCRNvirtualSubData,diagrsrc);
					}
					upd_diag=TRUE;
				} else {
					if(diagapps)
					{
						prterr("Skipped by Ranging on Virtual Field [%s].",(v->name!=NULL?v->name:""));
					}
					upd_diag=FALSE;
				}
			} else {
				if(diagapps)
				{
					prterr("Skipped by Ranging on [POSTRK][POSMSTR] file fields, POSITION IDENTIFICATION=[%s].",(posid!=NULL?posid:""));
				}
				upd_diag=FALSE;
			}
			} else {
				if(diagapps)
				{
					prterr("Skipped by Ranging on [PAYROLL][PAYJMST] file fields, PERSONNEL IDENTIFICATION=[%s], JOB NUMBER=[%d].",(lperid!=NULL?lperid:""),jobno);
				}
				upd_diag=FALSE;
			}
			} else {
				if(diagapps)
				{
					prterr("Skipped by Ranging on [PRSNNL][PERDMG] file fields, PERSONNEL IDENTIFICATION=[%s].",(lperid!=NULL?lperid:""));
				}
				upd_diag=FALSE;
			}
			UNLNRDFILE(payjtrn);
			if(prev_tran!=NULL) FreeRDATData(prev_tran);
		} else {
			upd_diag=FALSE;
		}
		if(diagrsrc!=NULL) update_diagnostic(diagrsrc,(upd_diag ? TRUE:FALSE));
		ef=ADVNXTNRDsec(payjtrn,keyno,SCRNvirtualSubData,diagrsrc);
	}
	if(diagrsrc!=NULL)
	{
		killwindow(diagrsrc);
		free_rsrc(diagrsrc);
	}	
	if(rs!=NULL) FreeRangeScreen(rs);
	if(perid!=NULL) Rfree(perid);
	if(lpayid!=NULL) Rfree(lpayid);
	if(lperid!=NULL) Rfree(lperid);
	if(posid!=NULL) Rfree(posid);
	if(reserve_code!=NULL) Rfree(reserve_code);
	if(enc_code!=NULL) Rfree(enc_code);
	if(WROTE_RECORDS==FALSE)
	{
		message=Rmalloc(RDAstrlen(payid)+112);
		sprintf(message,"There were no Made Transactions undone in the Payroll Identification of [%s]!",(payid!=NULL?payid:""));
		ERRORDIALOG("NO TRANSACTIONS UNDONE!",message,NULL,FALSE);
		if(message!=NULL) Rfree(message);
		if(AUTO_UNDO)
		{
			RDAAPPMAINLOOP();
		}
	}
	if(payid!=NULL) Rfree(payid);
}
static short selectbpaymstr(MakeBrowseList *blist)
{
	return(TRUE);
}
static void quitbpaymstr(MakeBrowseList *blist)
{
	APPlib *args=NULL;

	if(blist->definelist!=NULL) 
	{
		killwindow(blist->definelist);
		free_rsrc(blist->definelist);
	}
	if(blist->searchrsrc!=NULL) 
	{
		killwindow(blist->searchrsrc);
		free_rsrc(blist->searchrsrc);
	}
	if(blist->mainrsrc!=NULL) free_rsrc(blist->mainrsrc);
	CLSNRD(payjtrn);
	CLSNRD(payjmst);
	CLSNRD(payppms);
	CLSNRD(payjpms);
	CLSNRD(eyrnum);
	CLSNRD(byrnum);
	CLSNRD(payddac);
	CLSNRD(paymstr);
	CLSNRD(paydtrn);
	CLSNRD(perdmg);
	CLSNRD(posmstr);
	if(PAYROLL_SETUP!=NULL) free_payroll(PAYROLL_SETUP);
	if(WROTE_RECORDS)
	{
		args=APPlibNEW();
		addAPPlib(args,"PAYROLL");
		addAPPlib(args,"PAYJTRN");
		addAPPlib(args,"ALL");
		ExecuteProgram("remdel",args);
		if(args!=NULL) freeapplib(args);
		args=APPlibNEW();
		addAPPlib(args,"PAYROLL");
		addAPPlib(args,"PAYDTRN");
		addAPPlib(args,"ALL");
		ExecuteProgram("remdel",args);
		if(args!=NULL) freeapplib(args);
	}
	ShutdownSubsystems();
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *srsrc=NULL,*drsrc=NULL;
	MakeBrowseList *b=NULL;
	char *temp=NULL;
	APPlib *args=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,module,"PAYUNMTS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	PAYROLL_SETUP=RDApayrollNEW();
	getpayrollbin(PAYROLL_SETUP);
	if((payppms=APPOPNNRD(module,"PAYPPMS",TRUE,TRUE))==(-1)) return;
	if((payjpms=APPOPNNRD(module,"PAYJPMS",TRUE,FALSE))==(-1)) return;
	if((paymstr=APPOPNNRD(module,"PAYMSTR",TRUE,FALSE))==(-1)) return;
	if((payddac=APPOPNNRD(module,"PAYDDAC",TRUE,TRUE))==(-1)) return;
	if((eyrnum=APPOPNNRD("FINMGT","FINEYR",TRUE,TRUE))==(-1)) return;
	if((byrnum=APPOPNNRD("FINMGT","FINBYR",TRUE,TRUE))==(-1)) return;
	if((payjmst=APPOPNNRD(module,"PAYJMST",TRUE,FALSE))==(-1)) return;
	if((payjtrn=APPOPNNRD(module,"PAYJTRN",TRUE,FALSE))==(-1)) return;
	if((paydtrn=APPOPNNRD(module,"PAYDTRN",TRUE,TRUE))==(-1)) return;
	if((perdmg=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((posmstr=APPOPNNRD("POSTRK","POSMSTR",TRUE,FALSE))==(-1)) return;
	mrsrc=RDArsrcNEW(module,"UNDO MAKE TRANSACTIONS BROWSE");
	srsrc=RDArsrcNEW(module,"UNDO MAKE TRANSACTIONS SEARCH BROWSE");
	drsrc=RDArsrcNEW(module,"UNDO MAKE TRANSACTIONS DEFINE LIST");
	if(payjmst!=(-1)) file2rangersrc(payjmst,mrsrc);
	if(perdmg!=(-1)) file2rangersrc(perdmg,mrsrc);
	if(posmstr!=(-1)) file2rangersrc(posmstr,mrsrc);
	addDFincvir(mrsrc,module,"PAYJMST",NULL,payjmst);
	addDFincvir(mrsrc,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(mrsrc,"POSTRK","POSMSTR",NULL,posmstr);
	addDFincvir(drsrc,module,"PAYMSTR",NULL,paymstr);
	addDFincvir(drsrc,module,"PAYJMST",NULL,payjmst);
	addDFincvir(drsrc,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(drsrc,"POSTRK","POSMSTR",NULL,posmstr);
	addDFincvir(srsrc,module,"PAYMSTR",NULL,paymstr);
	addDFincvir(srsrc,module,"PAYJMST",NULL,payjmst);
	addDFincvir(srsrc,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(srsrc,"POSTRK","POSMSTR",NULL,posmstr);
	GET_SCREEN_VIRTUALS(mrsrc,2);
	GET_SCREEN_VIRTUALS(drsrc,1);
	GET_SCREEN_VIRTUALS(srsrc,2);
	temp=RDA_GetEnv("PAY_IDENTIFICATION");
	if(temp!=NULL)
	{
		PAYID=stralloc(temp);
		AUTO_UNDO=TRUE;
		okbpaymstr(NULL,NULL);
		if(drsrc!=NULL) free_rsrc(drsrc);
		if(srsrc!=NULL) free_rsrc(srsrc);
		if(mrsrc!=NULL) free_rsrc(mrsrc);
		CLSNRD(payjtrn);
		CLSNRD(payjmst);
		CLSNRD(payppms);
		CLSNRD(payjpms);
		CLSNRD(eyrnum);
		CLSNRD(byrnum);
		CLSNRD(payddac);
		CLSNRD(paymstr);
		CLSNRD(paydtrn);
		CLSNRD(perdmg);
		CLSNRD(posmstr);
		if(PAYROLL_SETUP!=NULL) free_payroll(PAYROLL_SETUP);
		if(WROTE_RECORDS)
		{
			args=APPlibNEW();
			addAPPlib(args,"PAYROLL");
			addAPPlib(args,"PAYJTRN");
			addAPPlib(args,"ALL");
			ExecuteProgram("remdel",args);
			if(args!=NULL) freeapplib(args);
			args=APPlibNEW();
			addAPPlib(args,"PAYROLL");
			addAPPlib(args,"PAYDTRN");
			addAPPlib(args,"ALL");
			ExecuteProgram("remdel",args);
			if(args!=NULL) freeapplib(args);
		}
		ShutdownSubsystems();
		exit(0);
	} else {
		PAYID=NULL;
		AUTO_UNDO=FALSE;
		b=ADVbrowse(paymstr,1,mrsrc,srsrc,drsrc,NULL,selectbpaymstr,quitbpaymstr,
			okbpaymstr,NULL,mrsrc,TRUE,NULL,0,0,2,TRUE);

		if(b!=NULL)
		{
			if(b->list!=NULL)
			{
				if(b->list->num>0) FINDRSCSETINT(mrsrc,"BROWSE LIST",b->list->num-1);
			}
		}
		WROTE_RECORDS=FALSE;
		RDAAPPMAINLOOP();
	}
}
