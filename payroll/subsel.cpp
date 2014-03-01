/* subsel.c - Select Substitutes */
#ifndef WIN32
#define __NAM__ "subsel.lnx"
#endif
#ifdef WIN32
#define __NAM__ "subsel.exe"
#endif
#include <app.hpp>

#include <mix.hpp>
#include <ldval.hpp>
#include <fin.hpp>
#include <pay.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

struct AccountStructs
{
	char *perid;		/* employee personnel id */
	int jobno;		/* employee job number */
	char setamounts;	/* set amounts flag */
	int accno;
	short deftype;
	char *account;
	char *debit;
	char *credit;
	short pdeftype;
	char *payable;
	char *cash;
	char *duefrom;
	char disencumber;
	char prorate;
	double percentage;
	double amount;
	double distamt;		/* Amount dispersal */
	double distperc;	/* Percentage disperdsal */
};
typedef struct AccountStructs AccountStruct;

struct subsels
{
	short gather_type;
	char *posid;
	char *wposid;
	int num;
        AccountStruct *a;
	double total;		/* Total Amount Dispersal for Perc. calc */
};
typedef struct subsels SUBSEL;

struct PassableStructs
{
	RDArsrc *r;
	void *a;
};
typedef struct PassableStructs PassableStruct;

RDArsrc *mainrsrc=NULL;
static char *module="PAYROLL",**selecttype=NULL,*audit_reportname=NULL;
static APPlib *errorlist=NULL,*warnlist=NULL;
static char Force_Warning=TRUE;
static char printed_errorlist=FALSE,printed_warnlist=FALSE;
static char *select_expression=NULL,**monthlist=NULL,*bank_id=NULL,*pay_id=NULL;
static char **fmonthlist=NULL,RateType=FALSE;
static char *pay_type=NULL,*defbankid=NULL;
static short subwrk=(-1),subpaid=(-1),submstr=(-1),subdtyp=(-1);
static short payjams=(-1),posacc=(-1);
static short payjmst=(-1),payjprm=(-1),payjpms=(-1),paymstr=(-1),prsnnl=(-1);
static short posmstr=(-1),poslmst=(-1),payppms=(-1),pgrat=(-1),payjcsm=(-1);
static short paytyp=(-1),finyear=(-1),perstat=(-1);
static short emp_payjmst=(-1);
static short pgross=(-1),payjfsm=(-1),bnkacc=(-1),payjrcs=(-1),payjrfs=(-1);
static short paypfsm=(-1),posrate=(-1),paypcsm=(-1);
static int select_type=0,DO_AUDIT=FALSE;
double RATE_AMT=0.0,CHECK_GROSS=0.0,UNITS=0.0;
static RDAvirtual *csortsequence=NULL;
static MakeBrowseList *mbl_trans;
static RDAfinmgt *fmgt=NULL;
static RDApayroll *PAYROLL_SETUP=NULL;
static void save_record(RDArsrc *,short,DBsort *);
static void quit_record(RDArsrc *,DBsort *);
static void save_record_up(RDArsrc *,DBsort *);
static void save_record_nup(RDArsrc *,DBsort *);
static void getsupportingfiles(RDArsrc *,DBsort *,short);
static void doexit(MakeBrowseList *);
static void doexit_test(MakeBrowseList *);
static void changemonth(RDArsrc *);
DBsort *dbsort=NULL;
static void resetmtndefs(RDArsrc *,DBsort *);
static void resetmaindefs(RDArsrc *);
static void getfile(RDArsrc *,DBsort *);
static void setfiles(RDArsrc *,DBsort *);
static void getprevpaymstcb(RDArsrc *),getnextpaymstcb(RDArsrc *),setpayid(RDArsrc *);
static void getpreviouscb(RDArsrc *,DBsort *);
static void getnextcb(RDArsrc *,DBsort *);
static void bankidcb(RDArsrc *);
static void paytypcb(RDArsrc *);
static void paytypfunc(RDArsrc *);
static void selectm(short,void *,int,char **,DBsort *);
static void savepaymstrsp(RDArsrc *),savepaymstr(RDArsrc *);
static void test_selectsub(RDArsrc *);
static void ComputeSUBSEL_Amount(SUBSEL *,int,char *,int,double);
static void ComputeSUBSEL_Percent(SUBSEL *);
static void makeAccImages(SUBSEL *,int,char *,int,PassableStruct *);
static void writePAYJAMS(SUBSEL *,char *,int,PassableStruct *);
static void deletePAYJAMS(char *,int,PassableStruct *);
static void SubSelSubData(char **,PassableStruct *);
static RDATData *prev_paymstr=NULL;

#define SeeAccountStruct(a)     xSeeAccountStruct(a,__LINE__,__FILE__)
void xSeeAccountStruct(AccountStruct *a,int line,char *file)
{
	if(a!=NULL)
	{
		prterr("DIAG SeeAccountStruct at line [%d] program [%s].",line,file);
		prterr(
			"DIAG SeeAccountStruct Emp Personnel ID [%s] Job Number [%d] Acc No [%d] Def [%d] Account [%s] Debit [%s] Credit [%s] Pay Fund Def [%d] Payable [%s] Cash [%s] Due From [%s] Disencumber [%s] Prorate [%s] Percentage [%f] Amount [$ %.02f].\n",
			(a->perid!=NULL?a->perid:""),
			a->jobno,
			a->accno,
			a->deftype,
			(a->account!=NULL?a->account:""),
			(a->debit!=NULL?a->debit:""),
			(a->credit!=NULL?a->credit:""),
			a->pdeftype,
			(a->payable!=NULL ? a->payable:""),
			(a->cash!=NULL?a->cash:""),
			(a->duefrom!=NULL?a->duefrom:""),
			(a->disencumber?"True":"False"),
			(a->prorate?"True":"False"),
			a->percentage,
			a->amount/100);
	} else {
		prterr("DIAG SeeAccountStruct is NULL at line [%d] program [%s].",line,file);
	}
}
#define SeeSUBSEL(a)    xSeeSUBSEL(a,__LINE__,__FILE__)
void xSeeSUBSEL(SUBSEL *p,int line,char *file)
{
	AccountStruct *a=NULL;
	int x=0;

	if(p!=NULL)
	{
		prterr("DIAG SeeSUBSEL at line [%d] program [%s].",line,file);
		if(p->a!=NULL && p->num>0)
		{
			prterr("SUBSEL Has [%d] AccountStructs.\n",p->num);
			for(x=0,a=p->a;x<p->num;++x,++a)
			{
				prterr("Account Struct #%d:\n",(x+1));
				SeeAccountStruct(a);

			}
		} else {
			prterr("DIAG SeeSUBSEL AccountStruct is NULL at line [%d] program [%s].",line,file);
		}
	} else {
		prterr("DIAG SeeSUBSEL is NULL at line [%d] program [%s].",line,file);
	}
}
static SUBSEL *SUBSELNEW(char *posid)
{
        SUBSEL *tmp=NULL;

        tmp=Rmalloc(sizeof(SUBSEL));
	if(!isEMPTY(posid))
	{
		tmp->posid=stralloc(posid);
	} else {
		tmp->posid=NULL;
	}
	tmp->wposid=NULL;
        tmp->total=0;
        tmp->num=0;
        tmp->a=NULL;
        return(tmp);
}
static int AddAccountStruct(SUBSEL *p,char *empperid,int empjobno,int accno,
	short dtype,char *acc,char *deb,char *cred,short pdtype,
	char *payable,char *cash,char *duefrom,char disencumber,
	char prorate,double percentage,double amount)
{
	AccountStruct *a=NULL;

	if(p->a!=NULL)
	{
		p->a=Rrealloc(p->a,(p->num+1)*sizeof(AccountStruct));
	} else {
		p->a=Rmalloc(sizeof(AccountStruct));
		p->num=0;
	}
	a=p->a+p->num;
	a->perid=stralloc(empperid);
	a->jobno=empjobno;
	a->setamounts=FALSE;
	a->accno=accno;
	a->deftype=dtype;
	a->pdeftype=pdtype;
	a->account=stralloc(acc);
	a->debit=stralloc(deb);
	a->credit=stralloc(cred);
	a->payable=stralloc(payable);
	a->cash=stralloc(cash);
	a->duefrom=stralloc(duefrom);
	a->disencumber=disencumber;
	a->prorate=prorate;
	a->percentage=percentage;
	a->amount=amount;
	if(diagapps)
	{
		SeeAccountStruct(a);TRACE;
	}
	return(++p->num);
}
static void FreeAccountStruct(SUBSEL *p)
{
	AccountStruct *a=NULL;
	int x=0;

	if(p!=NULL)
	{
		if(p->a!=NULL)
		{
			for(x=0,a=p->a;x<p->num;++x,++a)
			{
				if(a->account!=NULL) Rfree(a->account);
				if(a->debit!=NULL) Rfree(a->debit);
				if(a->credit!=NULL) Rfree(a->credit);
				if(a->payable!=NULL) Rfree(a->payable);
				if(a->cash!=NULL) Rfree(a->cash);
				if(a->duefrom!=NULL) Rfree(a->duefrom);
			}
			Rfree(p->a);
			p->num=0;
		}
	}
}
static void FreeSUBSEL(SUBSEL *p)
{
	if(p!=NULL)
	{
		if(p->posid!=NULL) Rfree(p->posid);
		if(p->wposid!=NULL) Rfree(p->wposid);
		FreeAccountStruct(p);
		Rfree(p);
		p=NULL;
	}
}
static int GetSUBSEL(SUBSEL *mts,char *perid,int jobno,PassableStruct *p)
{
	short ef=FALSE,deftype=0,pdeftype=0;
	char *lperid=NULL,deleteflag=FALSE,*acctcode=NULL,prorate=FALSE;
	char *debit=NULL,*credit=NULL,*payable=NULL,*cash=NULL;
	char *duefrom=NULL;
	char disencumber=FALSE,job_accts=TRUE,*posid=NULL;
	char *lposid=NULL,*temp=NULL;
	int ljobno=0,accno=0,count=0;
	double perc=0.0,amt=0.0;
	int x=(-1),y=(-1);

	if(mts->gather_type==0)
	{
	FINDFLDSETSTRING(payjmst,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(payjmst,"JOB NUMBER",jobno);
	if(ADVEQLNRDsec(payjmst,1,SubSelSubData,p)) KEYNRD(payjmst,1);
	FINDFLDGETCHAR(payjmst,"JOB ACCOUNTS",&job_accts);
	if(!isEMPTY(mts->posid)) Rfree(mts->posid);
	FINDFLDGETSTRING(payjmst,"POSITION IDENTIFICATION",&mts->posid);
	if(job_accts)
	{
		ZERNRD(payjams);
		FINDFLDSETSTRING(payjams,"PERSONNEL IDENTIFICATION",perid);
		FINDFLDSETINT(payjams,"JOB NUMBER",jobno);
		ef=ADVGTENRDsec(payjams,1,SubSelSubData,p);
		while(!ef)
		{
			FINDFLDGETSTRING(payjams,"PERSONNEL IDENTIFICATION",&lperid);
			FINDFLDGETINT(payjams,"JOB NUMBER",&ljobno);
			if(RDAstrcmp(perid,lperid) || jobno!=ljobno) break;
			FINDFLDGETCHAR(payjams,"DELETEFLAG",&deleteflag);
			if(!deleteflag)
			{
				FINDFLDGETINT(payjams,"ACCOUNT NUMBER",&accno);
				FINDFLDGETSHORT(payjams,"DEFINITION TYPE",&deftype);
				FINDFLDGETSHORT(payjams,"PAYROLL FUND DEFINITION TYPE",&pdeftype);
				FINDFLDGETSTRING(payjams,"ACCOUNT CODE",&acctcode);
				FINDFLDGETSTRING(payjams,"DEBIT CODE",&debit);
				FINDFLDGETSTRING(payjams,"CREDIT CODE",&credit);
				FINDFLDGETSTRING(payjams,"PAYABLE CODE",&payable);
				FINDFLDGETSTRING(payjams,"CASH CODE",&cash);
				FINDFLDGETSTRING(payjams,"DUE FROM",&duefrom);
				FINDFLDGETCHAR(payjams,"DISENCUMBER",&disencumber);
				FINDFLDGETCHAR(payjams,"PRORATE",&prorate);
				FINDFLDGETDOUBLE(payjams,"PERCENTAGE",&perc);
				FINDFLDGETDOUBLE(payjams,"AMOUNT",&amt);
				x=AddAccountStruct(mts,perid,jobno,accno,deftype,acctcode,debit,credit,pdeftype,payable,cash,duefrom,disencumber,prorate,perc,amt);
				++count;
				if(y==(-1))
				{
					y=x;
				}
				if(((prorate==TRUE) && (perc==0.0)) || ((prorate==FALSE) && (amt==0.0)))  
				{
					break;
				} 
			}
			ef=ADVNXTNRDsec(payjams,1,SubSelSubData,p);
		}
		if(count<1)
		{
			if((errorlist->numlibs!=1) || RDAstrcmp(errorlist->libs[0],"No Errors Detected"))
			{
				freeapplib(errorlist);
				errorlist=APPlibNEW();
			}
			temp=Rmalloc(512);
			sprintf(temp,"Error no Job Accounts Available for [%s][%d].",perid,jobno);
			prterr("%s",temp);
			addAPPlib(errorlist,temp);
			if(temp!=NULL) Rfree(temp);
		}
	} else {
		FINDFLDGETSTRING(payjmst,"POSITION IDENTIFICATION",&posid);
		ZERNRD(posacc);
		FINDFLDSETSTRING(posacc,"POSITION IDENTIFICATION",posid);
		ef=ADVGTENRDsec(posacc,1,SubSelSubData,p);
		while(!ef)
		{
			FINDFLDGETSTRING(posacc,"POSITION IDENTIFICATION",&lposid);
			if(RDAstrcmp(posid,lposid)) break;
			FINDFLDGETCHAR(posacc,"DELETEFLAG",&deleteflag);
			if(!deleteflag)
			{
				FINDFLDGETINT(posacc,"ACCOUNT NUMBER",&accno);
				FINDFLDGETSHORT(posacc,"DEFINITION TYPE",&deftype);
				FINDFLDGETSHORT(posacc,"PAYROLL FUND DEFINITION TYPE",&pdeftype);
				FINDFLDGETSTRING(posacc,"ACCOUNT CODE",&acctcode);
				FINDFLDGETSTRING(posacc,"DEBIT CODE",&debit);
				FINDFLDGETSTRING(posacc,"CREDIT CODE",&credit);
				FINDFLDGETSTRING(posacc,"PAYABLE CODE",&payable);
				FINDFLDGETSTRING(posacc,"CASH CODE",&cash);
				FINDFLDGETSTRING(posacc,"DUE FROM",&duefrom);
				FINDFLDGETCHAR(posacc,"PRORATE",&prorate);
				FINDFLDGETDOUBLE(posacc,"PERCENTAGE",&perc);
				FINDFLDGETDOUBLE(posacc,"AMOUNT",&amt);
				FINDFLDGETCHAR(posacc,"DISENCUMBER",&disencumber);
				x=AddAccountStruct(mts,perid,jobno,accno,deftype,acctcode,debit,credit,pdeftype,payable,cash,duefrom,disencumber,prorate,perc,amt);
				++count;
				if(y==(-1))
				{
					y=x;
				}
				if(((prorate==TRUE) && (perc==0.0)) || ((prorate==FALSE) && (amt==0.0)))  
				{
					break;
				} 
			}
			ef=ADVNXTNRDsec(posacc,1,SubSelSubData,p);
		}
		if(count<1)
		{
			temp=Rmalloc(512);
			sprintf(temp,"Error no Position Accounts Available for Position [%s] for [%s][%d].",mts->wposid,perid,jobno);
			prterr("%s",temp);
			addAPPlib(errorlist,temp);
			if(temp!=NULL) Rfree(temp);
		}
	}
	} else if(mts->gather_type==1)
	{
		ZERNRD(posacc);
		FINDFLDSETSTRING(posacc,"POSITION IDENTIFICATION",mts->wposid);
		if(!isEMPTY(mts->posid)) Rfree(mts->posid);
		mts->posid=stralloc(mts->wposid);
		ef=ADVGTENRDsec(posacc,1,SubSelSubData,p);
		while(!ef)
		{
			FINDFLDGETSTRING(posacc,"POSITION IDENTIFICATION",&lposid);
			if(RDAstrcmp(mts->wposid,lposid)) break;
			FINDFLDGETCHAR(posacc,"DELETEFLAG",&deleteflag);
			if(!deleteflag)
			{
				FINDFLDGETINT(posacc,"ACCOUNT NUMBER",&accno);
				FINDFLDGETSHORT(posacc,"DEFINITION TYPE",&deftype);
				FINDFLDGETSHORT(posacc,"PAYROLL FUND DEFINITION TYPE",&pdeftype);
				FINDFLDGETSTRING(posacc,"ACCOUNT CODE",&acctcode);
				FINDFLDGETSTRING(posacc,"DEBIT CODE",&debit);
				FINDFLDGETSTRING(posacc,"CREDIT CODE",&credit);
				FINDFLDGETSTRING(posacc,"PAYABLE CODE",&payable);
				FINDFLDGETSTRING(posacc,"CASH CODE",&cash);
				FINDFLDGETSTRING(posacc,"DUE FROM",&duefrom);
				FINDFLDGETCHAR(posacc,"PRORATE",&prorate);
				FINDFLDGETDOUBLE(posacc,"PERCENTAGE",&perc);
				FINDFLDGETDOUBLE(posacc,"AMOUNT",&amt);
				FINDFLDGETCHAR(posacc,"DISENCUMBER",&disencumber);
				x=AddAccountStruct(mts,perid,jobno,accno,deftype,acctcode,debit,credit,pdeftype,payable,cash,duefrom,disencumber,prorate,perc,amt);
				++count;
				if(y==(-1))
				{
					y=x;
				}
				if(((prorate==TRUE) && (perc==0.0)) || ((prorate==FALSE) && (amt==0.0)))  
				{
					break;
				} 
			}
			ef=ADVNXTNRDsec(posacc,1,SubSelSubData,p);
		}
		if(count<1)
		{
			temp=Rmalloc(512);
			sprintf(temp,"Error no Position Accounts Available for Position [%s] for [%s][%d].",mts->wposid,perid,jobno);
			prterr("%s",temp);
			addAPPlib(errorlist,temp);
			if(temp!=NULL) Rfree(temp);
		}
	}
	if(acctcode!=NULL) Rfree(acctcode);
	if(debit!=NULL) Rfree(debit);
	if(credit!=NULL) Rfree(credit);
	if(lperid!=NULL) Rfree(lperid);
	if(payable!=NULL) Rfree(payable);
	if(cash!=NULL) Rfree(cash);
	if(duefrom!=NULL) Rfree(duefrom);
	if(lposid!=NULL) Rfree(lposid);
	if(posid!=NULL) Rfree(posid);
	if(diagapps)
	{
		SeeSUBSEL(mts);TRACE;
	}
	return(y);
}
static void ComputeSUBSEL_Amount(SUBSEL *r,int start,char *empperid,int empjobno,double amount)
{
	double t=0.0;
	int x;
	AccountStruct *a=NULL;

	if(r!=NULL)
	{		
		if(r->a!=NULL)
		{
		for(x=start,a=r->a+start;x<r->num;++x,++a)
		{
			if(a->setamounts==FALSE)
			{
				if(!RDAstrcmp(empperid,a->perid) && empjobno==a->jobno)
				{
					a->distamt=0;
					a->distperc=0;
				}
			}
		}
		for(x=start,a=r->a+start;x<r->num;++x,++a)
		{
			if(a->setamounts==FALSE)
			{
				if(!RDAstrcmp(empperid,a->perid) && empjobno==a->jobno)
				{

					if((amount>0) && (amount-t)<=0) break;
					if((amount<0) && (amount+t)>=0) break;
					if((x+1)<r->num)
					{
						if(a->prorate)
						{
							a->distamt=round((amount*a->percentage/100));
							if((amount-t)<a->distamt)
							{
								a->distamt=(amount-t);
							}
						} else {
							if((amount-t)<a->amount)
							{
								a->distamt=(amount-t);
							} else {
								a->distamt=a->amount;
							}
						}
					} else {
						a->distamt=(amount-t);
					}
					t+=a->distamt;
					r->total+=a->distamt;
					a->setamounts=TRUE;
				}
			}
		}
		}
	}
}
static void ComputeSUBSEL_Percent(SUBSEL *r)
{
	int x;
	AccountStruct *a=NULL;

	if(r!=NULL)
	{
/* SJSCHANGE get percentages for prorate */
		for(x=0,a=r->a;x<r->num;++x,++a)
		{
			if((x+1)<r->num)
			{
				if(r->total==0)
				{	
					a->distperc=0.0;
				} else {
					a->distperc=(round((a->distamt/r->total)*1000000))/10000;
				}
			} else {
				a->distperc=0.0;
			}
		}
	}
}
static void makeAccImages(SUBSEL *p,int start,char *empperid,int empjobno,PassableStruct *ps)
{
	AccountStruct *a=NULL;
	char *acctimage=NULL,*debimage=NULL,*credimage=NULL,*payimage=NULL;
	char *temp=NULL,*cashimage=NULL,*duefromimage=NULL;
	int x=0;


	if(p!=NULL)
	{
		if(p->a!=NULL && p->num>0)
		{
			for(x=start,a=p->a+start;x<p->num;++x,++a)
			{
				if(!RDAstrcmp(empperid,a->perid) && empjobno==a->jobno)
				{
					ZERNRD(subdtyp);
					FINDFLDSETSTRING(subdtyp,"POSITION IDENTIFICATION",p->posid);
					FINDFLDSETSHORT(subdtyp,"DEFINITION TYPE",a->deftype);
					if(!ADVEQLNRDsec(subdtyp,1,SubSelSubData,ps))
					{
						FINDFLDGETSTRING(subdtyp,"ACCOUNT CODE IMAGE",&acctimage);
						FINDFLDGETSTRING(subdtyp,"DEBIT CODE IMAGE",&debimage);
						FINDFLDGETSTRING(subdtyp,"CREDIT CODE IMAGE",&credimage);
						FINDFLDGETSTRING(subdtyp,"PAYABLE CODE IMAGE",&payimage);
						FINDFLDGETSTRING(subdtyp,"CASH CODE IMAGE",&cashimage);
						FINDFLDGETSTRING(subdtyp,"DUE FROM IMAGE",&duefromimage);
						temp=MergeImage(a->account,acctimage,EXPENDITURE,a->deftype);
						if(!isEMPTY(temp))
						{
							if(a->account!=NULL) Rfree(a->account);
							a->account=stralloc(temp);
						}
						if(temp!=NULL) Rfree(temp);
						temp=MergeImage(a->debit,debimage,BALANCESHEET,a->deftype);
						if(!isEMPTY(temp))
						{
							if(a->debit!=NULL) Rfree(a->debit);
							a->debit=stralloc(temp);
						}
						if(temp!=NULL) Rfree(temp);
						temp=MergeImage(a->credit,credimage,BALANCESHEET,a->deftype);
						if(!isEMPTY(temp))
						{
							if(a->credit!=NULL) Rfree(a->credit);
							a->credit=stralloc(temp);
						}
						if(temp!=NULL) Rfree(temp);
						temp=MergeImage(a->payable,payimage,BALANCESHEET,a->pdeftype);
						if(!isEMPTY(temp))
						{
							if(a->payable!=NULL) Rfree(a->payable);
							a->payable=stralloc(temp);
						}
						if(temp!=NULL) Rfree(temp);
						temp=MergeImage(a->cash,cashimage,BALANCESHEET,a->pdeftype);
						if(!isEMPTY(temp))
						{
							if(a->cash!=NULL) Rfree(a->cash);
							a->cash=stralloc(temp);
						}
						if(temp!=NULL) Rfree(temp);
						temp=MergeImage(a->duefrom,duefromimage,BALANCESHEET,a->pdeftype);
						if(!isEMPTY(temp))
						{
							if(a->duefrom!=NULL) Rfree(a->duefrom);
							a->duefrom=stralloc(temp);
						}
						if(temp!=NULL) Rfree(temp);
						if(acctimage!=NULL) Rfree(acctimage);
						if(debimage!=NULL) Rfree(debimage);
						if(credimage!=NULL) Rfree(credimage);
						if(payimage!=NULL) Rfree(payimage);
						if(cashimage!=NULL) Rfree(cashimage);
						if(duefromimage!=NULL) Rfree(duefromimage);
					}
				}
			}
		}
	}
}
static void deletePAYJAMS(char *perid,int jobno,PassableStruct *p)
{
	char *perid2=NULL,delflag=FALSE;
	int jobno2=0;
	short ef=0;
	RDATData *prev_ef=NULL;

	ZERNRD(payjams);
	FINDFLDSETSTRING(payjams,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(payjams,"JOB NUMBER",jobno);
	ef=ADVGTENRDsec(payjams,1,SubSelSubData,p);
	while(!ef)
	{
		FINDFLDGETSTRING(payjams,"PERSONNEL IDENTIFICATION",&perid2);
		FINDFLDGETINT(payjams,"JOB NUMBER",&jobno2);
		if(RDAstrcmp(perid2,perid) || jobno2!=jobno) break;
		LOCNRDFILE(payjams);
		FINDFLDGETCHAR(payjams,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			prev_ef=RDATDataNEW(payjams);
			FINDFLDSETCHAR(payjams,"DELETEFLAG",TRUE);
			ADVWRTTRANSsec(payjams,0,NULL,prev_ef,SubSelSubData,p);
			if(prev_ef!=NULL) FreeRDATData(prev_ef);
		}
		UNLNRDFILE(payjams);
		ef=ADVNXTNRDsec(payjams,1,SubSelSubData,p);
	}
	if(perid2!=NULL) Rfree(perid2);
}
static void smallfirstPAYJAMS(SUBSEL *p)
{
	AccountStruct *a=NULL,*b=NULL;
	int x=0,y=0,jobno=0,accno=0;
	short deftype=0;
	char *temp=NULL,sa=FALSE;
	double amt=0.0;

	if(p!=NULL)
	{
		if(p->a!=NULL && p->num>0)
		{
			for(x=0;x<(p->num-1);++x) for(y=(x+1);y<p->num;++y)
			{
				a=p->a+x;
				b=p->a+y;
				if((double)a->amount>(double)b->amount)
				{
					temp=a->perid;
					a->perid=b->perid;
					b->perid=temp;
					jobno=a->jobno;
					a->jobno=b->jobno;
					b->jobno=jobno;
					sa=a->setamounts;
					a->setamounts=b->setamounts;
					b->setamounts=sa;	
					accno=a->accno;
					a->accno=b->accno;
					b->accno=accno;
					deftype=a->deftype;
					a->deftype=b->deftype;
					b->deftype=deftype;
					temp=a->account;
					a->account=b->account;
					b->account=temp;
					temp=a->debit;
					a->debit=b->debit;
					b->debit=temp;
					temp=a->credit;
					a->credit=b->credit;
					b->credit=temp;
					deftype=a->pdeftype;
					a->pdeftype=b->pdeftype;
					b->pdeftype=deftype;
					temp=a->payable;
					a->payable=b->payable;
					b->payable=temp;
					temp=a->cash;
					a->cash=b->cash;
					b->cash=temp;
					temp=a->duefrom;
					a->duefrom=b->duefrom;
					b->duefrom=temp;
					sa=a->disencumber;
					a->disencumber=b->disencumber;
					b->disencumber=sa;
					sa=a->prorate;
					a->prorate=b->prorate;
					b->prorate=sa;
					amt=a->percentage;
					a->percentage=b->percentage;
					b->percentage=amt;
					amt=a->amount;
					a->amount=b->amount;
					b->amount=amt;
					amt=a->distamt;
					a->distamt=b->distamt;
					b->distamt=amt;
					amt=a->distperc;
					a->distperc=b->distperc;
					b->distperc=amt;
				}
			}
		}
	}
}
static void sortPAYJAMS(SUBSEL *p)
{
	AccountStruct *a=NULL,*b=NULL;
	int x=0,y=0,jobno=0,accno=0;
	short deftype=0;
	char *temp=NULL,sa=FALSE;
	double amt=0.0;

	if(p!=NULL)
	{
		if(p->a!=NULL && p->num>0)
		{
			for(x=0;x<p->num-1;++x) for(y=x+1;y<p->num;++y)
			{
				a=p->a+x;
				b=p->a+y;
				if(RDAstrcmp(a->account,b->account)>0)
				{
					temp=a->perid;
					a->perid=b->perid;
					b->perid=temp;
					jobno=a->jobno;
					a->jobno=b->jobno;
					b->jobno=jobno;
					sa=a->setamounts;
					a->setamounts=b->setamounts;
					b->setamounts=sa;	
					accno=a->accno;
					a->accno=b->accno;
					b->accno=accno;
					deftype=a->deftype;
					a->deftype=b->deftype;
					b->deftype=deftype;
					temp=a->account;
					a->account=b->account;
					b->account=temp;
					temp=a->debit;
					a->debit=b->debit;
					b->debit=temp;
					temp=a->credit;
					a->credit=b->credit;
					b->credit=temp;
					deftype=a->pdeftype;
					a->pdeftype=b->pdeftype;
					b->pdeftype=deftype;
					temp=a->payable;
					a->payable=b->payable;
					b->payable=temp;
					temp=a->cash;
					a->cash=b->cash;
					b->cash=temp;
					temp=a->duefrom;
					a->duefrom=b->duefrom;
					b->duefrom=temp;
					sa=a->disencumber;
					a->disencumber=b->disencumber;
					b->disencumber=sa;
					sa=a->prorate;
					a->prorate=b->prorate;
					b->prorate=sa;
					amt=a->percentage;
					a->percentage=b->percentage;
					b->percentage=amt;
					amt=a->amount;
					a->amount=b->amount;
					b->amount=amt;
					amt=a->distamt;
					a->distamt=b->distamt;
					b->distamt=amt;
					amt=a->distperc;
					a->distperc=b->distperc;
					b->distperc=amt;
				}
			}
		}
	}
}
static void writePAYJAMS(SUBSEL *p,char *perid,int jobno,PassableStruct *ps)
{
	AccountStruct *a=NULL,*last=NULL;
	char *acctcode=NULL,*debcode=NULL,*credcode=NULL,*paycode=NULL;
	char *cashcode=NULL,*duefromcode=NULL;
	int x=0;
	RDATData *prev_file=NULL;
	double zeroamt=0.0,totalamt=0.0;
	SUBSEL *S=NULL;

	if(p!=NULL)
	{
		ZERNRD(payjmst);
		FINDFLDSETSTRING(payjmst,"PERSONNEL IDENTIFICATION",perid);
		FINDFLDSETINT(payjmst,"JOB NUMBER",jobno);
		if(ADVEQLNRDsec(payjmst,1,SubSelSubData,ps))
		{
			KEYNRD(payjmst,1);
		} else {
			LOCNRD(payjmst);
			prev_file=RDATDataNEW(payjmst);
			FINDFLDSETCHAR(payjmst,"JOB ACCOUNTS",TRUE);
			ADVWRTTRANSsec(payjmst,0,NULL,prev_file,SubSelSubData,ps);
			UNLNRD(payjmst);
			if(prev_file!=NULL) FreeRDATData(prev_file);
		}
		sortPAYJAMS(p);
		S=SUBSELNEW(p->posid);
		if(p->a!=NULL && p->num>0)
		{
			for(x=0,a=p->a;x<p->num;++x,++a)
			{
				if(x>0) last=p->a+(x-1);
					else last=NULL;
				if(last==NULL)
				{
					totalamt=a->distamt;
				} else if(!RDAstrcmp(last->account,a->account))
				{
					totalamt+=a->distamt;
				} else {
					if(!isEMPTY(last->account))
					{
						if(acctcode!=NULL) Rfree(acctcode);
						acctcode=stralloc(last->account);
					}
					if(!isEMPTY(last->debit))
					{
						if(debcode!=NULL) Rfree(debcode);
						debcode=stralloc(last->debit);
					}
					if(!isEMPTY(last->credit))
					{
						if(credcode!=NULL) Rfree(credcode);
						credcode=stralloc(last->credit);
					}
					if(!isEMPTY(last->payable))
					{
						if(paycode!=NULL) Rfree(paycode);
						paycode=stralloc(last->payable);
					}
					if(!isEMPTY(last->cash))
					{
						if(cashcode!=NULL) Rfree(cashcode);
						cashcode=stralloc(last->cash);
					}
					if(!isEMPTY(last->duefrom))
					{
						if(duefromcode!=NULL) Rfree(duefromcode);
						duefromcode=stralloc(last->duefrom);
					}
					if(totalamt!=0.0)
					{
						AddAccountStruct(S,perid,jobno,a->accno,last->deftype,acctcode,debcode,credcode,last->pdeftype,paycode,cashcode,duefromcode,FALSE,FALSE,zeroamt,totalamt);
					}
					totalamt=a->distamt;
				}
			}
			last=p->a+(p->num-1);
			if(!isEMPTY(last->account))
			{
				if(acctcode!=NULL) Rfree(acctcode);
				acctcode=stralloc(last->account);
			}
			if(!isEMPTY(last->debit))
			{
				if(debcode!=NULL) Rfree(debcode);
				debcode=stralloc(last->debit);
			}
			if(!isEMPTY(last->credit))
			{
				if(credcode!=NULL) Rfree(credcode);
				credcode=stralloc(last->credit);
			}
			if(!isEMPTY(last->payable))
			{
				if(paycode!=NULL) Rfree(paycode);
				paycode=stralloc(last->payable);
			}
			if(!isEMPTY(last->cash))
			{
				if(cashcode!=NULL) Rfree(cashcode);
				cashcode=stralloc(last->cash);
			}
			if(!isEMPTY(last->duefrom))
			{
				if(duefromcode!=NULL) Rfree(duefromcode);
				duefromcode=stralloc(last->duefrom);
			}
			if(totalamt!=0.0)
			{
				AddAccountStruct(S,perid,jobno,a->accno,last->deftype,acctcode,debcode,credcode,last->pdeftype,paycode,cashcode,duefromcode,FALSE,FALSE,zeroamt,totalamt);
			}
		}
		smallfirstPAYJAMS(S);
		if(S->a!=NULL && S->num>0)
		{
			for(x=0,a=S->a;x<S->num;++x,++a)
			{
				ZERNRD(payjams);
				FINDFLDSETSTRING(payjams,"PERSONNEL IDENTIFICATION",perid);
				FINDFLDSETINT(payjams,"JOB NUMBER",jobno);
				FINDFLDSETINT(payjams,"ACCOUNT NUMBER",(x+1));
				FINDFLDSETINT(payjams,"DEFINITION TYPE",a->deftype);
				FINDFLDSETINT(payjams,"PAYROLL FUND DEFINITION TYPE",a->pdeftype);
				FINDFLDSETSTRING(payjams,"ACCOUNT CODE",a->account);
				FINDFLDSETSTRING(payjams,"DEBIT CODE",a->debit);
				FINDFLDSETSTRING(payjams,"CREDIT CODE",a->credit);
				FINDFLDSETSTRING(payjams,"PAYABLE CODE",a->payable);
				FINDFLDSETSTRING(payjams,"CASH CODE",a->cash);
				FINDFLDSETSTRING(payjams,"DUE FROM",a->duefrom);
				FINDFLDSETCHAR(payjams,"DISENCUMBER",FALSE);
				FINDFLDSETCHAR(payjams,"PRORATE",FALSE);
				FINDFLDSETDOUBLE(payjams,"PERCENTAGE",zeroamt);
				FINDFLDSETDOUBLE(payjams,"AMOUNT",a->amount);
				FINDFLDSETCHAR(payjams,"DELETEFLAG",FALSE);
				ADVWRTTRANSsec(payjams,1,"ACCOUNT NUMBER",NULL,SubSelSubData,ps);
			}
		}
		FreeSUBSEL(S);
		if(acctcode!=NULL) Rfree(acctcode);
		if(debcode!=NULL) Rfree(debcode);
		if(credcode!=NULL) Rfree(credcode);
		if(paycode!=NULL) Rfree(paycode);
		if(cashcode!=NULL) Rfree(cashcode);
		if(duefromcode!=NULL) Rfree(duefromcode);
	}
}
static void SubSelSubData(char **tmp,PassableStruct *p)
{
	char *modulename=NULL,*filename=NULL,*fieldname=NULL;
	NRDfield *field=NULL;
	short start=1,length=0,fileno=(-1),occurrence=1,str_length=0;
	char *temp=NULL,*gn=NULL;
	int y=0;
	RDAvirtual *v=NULL;
	DBsort *dbsort=NULL;
	RDArsrc *r=NULL;

	r=p->r;
	dbsort=p->a;
	if(diageval)
	{
		prterr("DIAG SubSelSubData Expression Evaluator for Virtual Fields on Screen [%s] [%s] Before Evaluating [%s].",r->module,r->screen,*tmp);
	}
	modulename=stripmodulename(*tmp);
	if(modulename!=NULL)
	{
	memset(stemp,0,101);
	filename=stripfilename(*tmp);
	fieldname=stripfieldname(*tmp);
	if(fieldname!=NULL)
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+RDAstrlen(filename)+
			RDAstrlen(fieldname)+6))
		{
			temp=*tmp+(RDAstrlen(modulename)+RDAstrlen(filename)+
				RDAstrlen(fieldname)+7);
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				occurrence=Ratoi(gn);
				gn=strtok(NULL,",]");
				start=Ratoi(gn);
				gn=strtok(NULL,",]");
				length=Ratoi(gn);
			}
			if(start<1) start=1;
		}
		fileno=SCRNFILENUMBER(r,modulename,filename,occurrence);
		if(fileno!=(-1))
		{
			field=FLDNRDsec(fileno,fieldname,TRUE,FALSE,FALSE);
		} else field=NULL;
		if(field!=NULL)
		{
			switch(field->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
					temp=stralloc(field->data.string_value);
					str_length=(temp!=NULL?RDAstrlen(temp):0);
					if(length<1) length=str_length;
					if(RDAstrlen(*tmp)<(length+3))
					{
						*tmp=Rrealloc(*tmp,length+3);
					}
					if(!isEMPTY(temp) && str_length>length)
					{
						gn=temp+(start-1);
						if(RDAstrlen(gn)>(length))
							temp[start-1+length]=0;
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
					} else if(start<=str_length && !isEMPTY(temp))
					{
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
					} else {
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"\"");
					}
					if(temp!=NULL) Rfree(temp);
					temp=NULL;
					break;
				case SHORTV:
				case SSHORTV:
					if(*field->data.short_value<0)
					{
						sprintf(stemp,"(%d)",*field->data.short_value);
					} else {
						sprintf(stemp,"%d",*field->data.short_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<str_length)
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case SLONGV:
				case SCROLLEDLIST:
				case LONGV:
					if(*field->data.integer_value<0)
					{
						sprintf(stemp,"(%d)",*field->data.integer_value);
					} else {
						sprintf(stemp,"%d",*field->data.integer_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case DOLLARS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					if(*field->data.float_value<0)
					{
						sprintf(stemp,"(%f)",*field->data.float_value);
					} else {
						sprintf(stemp,"%f",*field->data.float_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case BOOLNS:
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"%d",(*field->data.string_value ? 1:0));
					break;
				case CHARACTERS:
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"%c",*field->data.string_value);
					break;
				default:
					prterr("Error Field type [%d] is invalid for [%s] file [%s].",field->type,field->name,FILENAME(fileno)); 
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=stralloc("\"\"");
					break;
			}
		}
		if(fieldname!=NULL) Rfree(fieldname);
		if(filename!=NULL) Rfree(filename);
	} else if(!RDAstrcmp(modulename,"UNITS"))
	{
		if(UNITS<0) sprintf(stemp,"(%f)",UNITS);
			else sprintf(stemp,"%f",UNITS);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memcpy(*tmp,stemp,str_length+1);
	} else if(!RDAstrcmp(modulename,"RATE AMOUNT"))
	{
		if(RATE_AMT<0) sprintf(stemp,"(%f)",RATE_AMT);
			else sprintf(stemp,"%f",RATE_AMT);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memcpy(*tmp,stemp,str_length+1);
	} else if(!RDAstrcmp(modulename,"CHECK GROSS"))
	{
		if(CHECK_GROSS<0) sprintf(stemp,"(%f)",CHECK_GROSS);
			else sprintf(stemp,"%f",CHECK_GROSS);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<RDAstrlen(stemp))
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memcpy(*tmp,stemp,str_length+1);
	} else if(GLOBALSubData(tmp,modulename)==0)
	{
/* OK, found in global virtual fields */
	} else { /* virtual field */
		for(y=0,v=r->virflds;y<r->num;++y,++v)
		{
			if(!RDAstrcmp(modulename,v->name)) break;
		}
		if(y<r->num)
		{
			switch(v->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					if(!v->computed) 
					{
						v->computed=TRUE;
						temp=EVALUATEstr(v->expression,SubSelSubData,p);
						if(v->value.string_value!=NULL) 
							Rfree(v->value.string_value);
						v->value.string_value=stralloc(temp);
						if(temp!=NULL) Rfree(temp);
						if(v->value.string_value!=NULL)
						{
							v->dlen=RDAstrlen(v->value.string_value)+1;
							if(v->dlen>v->len && v->len>0)
							{
								v->value.string_value[v->len]=0;
							}
						}
					}
					if(RDAstrlen(*tmp)>(RDAstrlen(v->name)+2))
					{
						temp=*tmp+(RDAstrlen(v->name)+3);
						gn=strtok(temp,",]");
						if(gn!=NULL)
						{
							start=Ratoi(gn);
							gn=strtok(NULL,"]");
							length=Ratoi(gn);
						}
						if(start<1) start=1;
					}
					if(length<1) length=v->dlen;
					if(RDAstrlen(*tmp)<(length+3))
					{
						*tmp=Rrealloc(*tmp,length+3);
					}
					temp=stralloc(v->value.string_value);
					if(!isEMPTY(temp) && v->dlen>length)
					{
						gn=temp+(start-1);
						if(RDAstrlen(gn)>(length))
							temp[start-1+length]=0;
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
					} else if(start<v->dlen && !isEMPTY(temp))
					{
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
					} else {
						sprintf(*tmp,"\"\"");
					}
					if(temp!=NULL) Rfree(temp);
					temp=NULL;
					break;
				case SHORTV:
				case SSHORTV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.short_value=EVALUATEsht(v->expression,SubSelSubData,p);
					}
					if(*v->value.short_value<0)
					{
						sprintf(stemp,"(%d)",*v->value.short_value);
					} else {
						sprintf(stemp,"%d",*v->value.short_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memcpy(*tmp,stemp,str_length+1);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.integer_value=EVALUATEint(v->expression,SubSelSubData,p);
					}
					if(*v->value.integer_value<0)
					{
						sprintf(stemp,"(%d)",*v->value.integer_value);
					} else {
						sprintf(stemp,"%d",*v->value.integer_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memcpy(*tmp,stemp,str_length+1);
					break;
				case DOLLARS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.float_value=EVALUATEdbl(v->expression,SubSelSubData,p);
						if(v->type==DOLLARS ||
							v->type==DOUBLEV ||
							v->type==SDOUBLEV)
						{
							*v->value.float_value=round(*v->value.float_value);
						}	
					}
					if(*v->value.float_value<0)
					{
						sprintf(stemp,"(%f)",*v->value.float_value);
					} else {
						sprintf(stemp,"%f",*v->value.float_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memcpy(*tmp,stemp,str_length+1);
					break;
				case BOOLNS:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.string_value=EVALUATEbol(v->expression,SubSelSubData,p);
					}
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					sprintf(stemp,"%d",(*v->value.string_value ? 1:0));
					break;
				case CHARACTERS:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.string_value=EVALUATEbol(v->expression,SubSelSubData,p);
					}
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					sprintf(*tmp,"%c",*v->value.string_value);
					break;
				default:
					prterr("Error Field type [%d] is invalid for virtual field [%s].",v->type,v->name); 
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=stralloc("\"\"");
					break;
			}
		} else {
			field=FLDNRD(dbsort->fileno,modulename);
			if(field!=NULL)
			{
				switch(field->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case ZIPCODE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
						{
							temp=*tmp+RDAstrlen(modulename)+3;
							gn=strtok(temp,",]");
							if(gn!=NULL)
							{
								start=Ratoi(gn);
								gn=strtok(NULL,"]");
								length=Ratoi(gn);
							}
							if(start<1) start=1;

						}
						temp=stralloc(field->data.string_value);
						str_length=(temp!=NULL?RDAstrlen(temp):0);
						if(length==0) length=str_length;
						if(RDAstrlen(*tmp)<(length+3))
						{
							*tmp=Rrealloc(*tmp,length+3);
						}
						if(!isEMPTY(temp) && str_length>length)
						{
							gn=temp+(start-1);
							if(RDAstrlen(gn)>(length))
								temp[start-1+length]=0;
							if(RDAstrlen(temp)>(start-1+length))
								temp[start-1+length]=0;
							sprintf(*tmp,"\"%s\"",&temp[start-1]);
						} else if(start<=str_length && !isEMPTY(temp))
						{
							sprintf(*tmp,"\"%s\"",&temp[start-1]);
						} else {
							sprintf(*tmp,"\"\"");
						}
						if(temp!=NULL) Rfree(temp);
						temp=NULL;
						break;
					case SHORTV:
					case SSHORTV:
						if(*field->data.short_value<0)
						{
							sprintf(stemp,"(%d)",*field->data.short_value);
						} else {
							sprintf(stemp,"%d",*field->data.short_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<str_length)
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memcpy(*tmp,stemp,str_length+1);
						break;
					case SLONGV:
					case SCROLLEDLIST:
					case LONGV:
						if(*field->data.integer_value<0)
						{
							sprintf(stemp,"(%d)",*field->data.integer_value);
						} else {
							sprintf(stemp,"%d",*field->data.integer_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<str_length)
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memcpy(*tmp,stemp,str_length+1);
						break;
					case DECIMALV:
					case SDECIMALV:
						if(*field->data.float_value<0)
						{
							sprintf(stemp,"(%f)",*field->data.float_value);
						} else {
							sprintf(stemp,"%f",*field->data.float_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<str_length)
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memcpy(*tmp,stemp,str_length+1);
						break;
					case SDOUBLEV:
					case DOLLARS:
					case DOUBLEV:
						if(*field->data.float_value<0)
						{
							sprintf(stemp,"(%.0f)",*field->data.float_value);
						} else {
							sprintf(stemp,"%.0f",*field->data.float_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<str_length)
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memcpy(*tmp,stemp,str_length+1);
						break;
					case BOOLNS:
						if(RDAstrlen(*tmp)<2)
						{
							*tmp=Rrealloc(*tmp,2);
						}
						sprintf(*tmp,"%d",(*field->data.string_value ? 1:0));
						break;
					case CHARACTERS:
						if(RDAstrlen(*tmp)<2)
						{
							*tmp=Rrealloc(*tmp,2);
						}
						sprintf(*tmp,"%c",*field->data.string_value);
						break;
					default:
						prterr("Error field type %d is invalid for %s.",
							field->type,field->name); 
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=NULL;
						break;
				}
			} else {
				if(ScrolledListSubData(tmp,r)==(-1))
				{
					prterr("Error Field [%s] not found.",*tmp);
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=stralloc("\"\"");
				}
			}
		}
	}
	Rfree(modulename);
	}
/*
	prterr("DIAG SubSelSubData for Screen [%s] [%s] After Evaluating [%s].",r->module,r->screen,*tmp);
*/
	if(diageval)
	{
		prterr("DIAG SubSelSubData for Screen [%s] [%s] After Evaluating [%s].",r->module,r->screen,*tmp);
	}
}
APPlib *makefieldvallist()
{
	APPlib *list=NULL,*virflist=NULL;
	RDArsrc *tmprsrc=NULL;
	short dumb=0,addvf=FALSE;
	char *libx=NULL;
	int y,x,z;
	DFincvir *i,*a;
	DFvirtual *d;

	list=APPlibNEW();
	AddFileFields(list,submstr);
	AddFileFields(list,subwrk);
	AddFileFields(list,payjmst);
	AddFileFields(list,prsnnl);
	AddFileFields(list,perstat);
	AddFileFields(list,posmstr);
	AddFileFields(list,pgross);
	AddFileFields(list,pgrat);
	AddFileFields(list,posrate);
	AddFileFields(list,payjcsm);
	AddFileFields(list,payjfsm);
	AddFileFields(list,payjrcs);
	AddFileFields(list,payjrfs);
	tmprsrc=RDArsrcNEW(module,"TEMP");
	addDFincvir(tmprsrc,"SUBMGT","SUBMSTR",NULL,dumb);
	addDFincvir(tmprsrc,"SUBMGT","SUBWKMAS",NULL,dumb);
	addDFincvir(tmprsrc,module,"PAYJMST",NULL,dumb);
	addDFincvir(tmprsrc,"PRSNNL","PERDMG",NULL,dumb);
	addDFincvir(tmprsrc,"PRSNNL","PERSTAT",NULL,dumb);
	addDFincvir(tmprsrc,"POSTRK","POSMSTR",NULL,dumb);
	addDFincvir(tmprsrc,"POSTRK","POSGRS",NULL,dumb);
	addDFincvir(tmprsrc,"POSTRK","POSGRAT",NULL,dumb);
	addDFincvir(tmprsrc,"POSTRK","POSRATE",NULL,dumb);
	addDFincvir(tmprsrc,module,"PAYJCSM",NULL,dumb);
	addDFincvir(tmprsrc,module,"PAYJFSM",NULL,dumb);
	addDFincvir(tmprsrc,module,"PAYJRCS",NULL,dumb);
	addDFincvir(tmprsrc,module,"PAYJRFS",NULL,dumb);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
	sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,module);
	virflist=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	libx=NULL;
	if(virflist!=NULL)
	{
		for(y=0;y<virflist->numlibs;++y)
		{
			d=getDFvirtual(module,virflist->libs[y]);
			if(d!=NULL)
			{
				addvf=TRUE;
				if(d->incvir!=NULL)
				{
					for(z=0,a=d->incvir;z<d->num;++z,++a)
					{
						for(x=0,i=tmprsrc->incvir;x<tmprsrc->numvir;++x,++i)
						{
							if(!RDAstrcmp(i->module,a->module) && 
								!RDAstrcmp(i->file,a->file)) break;
						}
						if(x>=tmprsrc->numvir) 
						{
							addvf=FALSE;
							break;
						}
					}
				}
				if(addvf)
				{
					libx=Rmalloc(RDAstrlen(d->name)+3);
					sprintf(libx,"[%s]",d->name);
					addAPPlibNoDuplicates(list,libx);
					if(libx!=NULL) Rfree(libx);
				}
				FreeDFvirtual(d);
			}
		}
	}
	if(virflist!=NULL) freeapplib(virflist);
	if(libx!=NULL) Rfree(libx);
	loadglobals(list);
	if(tmprsrc!=NULL) free_rsrc(tmprsrc);
	SORTAPPlib(list);
	return(list);
}
void loadvalue(RDArsrc *r,char *fieldname)
{
	int x;
	LoadValue *load;

	ldvalrsrc=RDArsrcNEW("RPTGEN","LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	loadlist=makefieldvallist();
	if(loadlist->numlibs<1)
	{
		addAPPlib(loadlist,"Load List Unavailable");
	}
	x=0;
	addlstrsrc(ldvalrsrc,"LOAD LIST",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addrfkwrsrc(ldvalrsrc,"SELECT",TRUE,okldval,load);
	addrfkwrsrc(ldvalrsrc,"QUIT",TRUE,quitldval,load);
	addbtnrsrc(ldvalrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ldvalrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(ldvalrsrc,TRUE,quitldval,load,FALSE);
}
void select_backend(DBsort *dbsort)
{
	short ef=FALSE,update_diag=FALSE,valid_last_exists=FALSE,src=1;
	char deleteflag=FALSE;
	char *perid=NULL,*lastperid=NULL,*curdate=NULL,*curtime=NULL;
	char *curdate10=NULL;
	NRDfield *field=NULL;
	char *lastrateid=NULL,*rateid=NULL;
	char *empperid=NULL,*e=NULL;
	int jobno=0,lastjobno=0,empjobno=0,subtransno=0;
	double gamt=0.0,tmpunits=0.0,units=0.0,job_amt=0,job_units=0.0;
	double per_gross=0.0,per_net=0.0;
	double wrkunits=0.0,amt=0.0,rate_to_use=0.0;
	double rate_units=0.0,rate_amt=0,a=0.0,last_rate_amt=0.0;
	RDArsrc *tmprsrc=NULL;
	SUBSEL *mts=NULL;
	RDATData *prev_file=NULL,*prev=NULL;
	PassableStruct *p=NULL;
	int y=0;

	if(dbsort!=NULL)
	{
		tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN",module,
			"Updating Payroll Job Masters",NULL);
		addDFincvir(tmprsrc,"SUBMGT","SUBWKMAS",NULL,subwrk);
		addDFincvir(tmprsrc,"SUBMGT","SUBPAID",NULL,subpaid);
		addDFincvir(tmprsrc,"SUBMGT","SUBDTYP",NULL,subdtyp);
		addDFincvir(tmprsrc,"POSTRK","POSACC",NULL,posacc);
		addDFincvir(tmprsrc,module,"PAYJMST",NULL,payjmst);
		addDFincvir(tmprsrc,module,"PAYJAMS",NULL,payjams);
		addDFincvir(tmprsrc,module,"PAYJPRM",NULL,payjprm);
		addDFincvir(tmprsrc,module,"PAYPPMS",NULL,payppms);
		addDFincvir(tmprsrc,module,"PAYJPMS",NULL,payjpms);
		GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
		if(tmprsrc!=NULL)
		{
			if(!ADVmakescrn(tmprsrc,TRUE))
			{
				ForceWindowUpdate(tmprsrc);
			} else {
				e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
				sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
				WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
				prterr("Error %s.",e);
				if(e!=NULL) Rfree(e);
			}
		}
		p=Rmalloc(sizeof(PassableStruct));
		p->r=tmprsrc;
		p->a=dbsort;
		ZERNRD(dbsort->fileno);
		ef=GTENRD(dbsort->fileno,1);
		job_amt=0;
		job_units=0.0;
		rate_amt=0;
		rate_units=0.0;
		per_gross=0.0;
		FINDFLDGETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",&lastperid);
		FINDFLDGETSTRING(dbsort->fileno,"RATE IDENTIFICATION",&lastrateid);
		FINDFLDGETINT(dbsort->fileno,"JOB NUMBER",&lastjobno);
		FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&deleteflag);
		FINDFLDGETDOUBLE(dbsort->fileno,"RATE AMOUNT",&last_rate_amt);
		FINDFLDGETDOUBLE(dbsort->fileno,"CHECK GROSS",&gamt);
		FINDFLDGETDOUBLE(dbsort->fileno,"UNITS",&units);
/*
		if(!deleteflag && (gamt!=0.0 || units!=0.0))
*/
		if(!deleteflag)
		{
			mts=SUBSELNEW(NULL);
			deletePAYJAMS(lastperid,lastjobno,p);
		}
		while(!ef)
		{
			curdate=GETCURRENTDATE();
			curdate10=GETCURRENTDATE10();
			update_diag=FALSE;
			FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&deleteflag);
			FINDFLDGETDOUBLE(dbsort->fileno,"CHECK GROSS",&gamt);
			FINDFLDGETDOUBLE(dbsort->fileno,"UNITS",&units);
			if(curtime!=NULL) Rfree(curtime);
			curtime=GETCURRENTTIME();
			if(!deleteflag)
			{
				FINDFLDGETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",&perid);
				FINDFLDGETINT(dbsort->fileno,"JOB NUMBER",&jobno);
				FINDFLDGETSTRING(dbsort->fileno,"RATE IDENTIFICATION",&rateid);
				if(RDAstrcmp(lastperid,perid) || lastjobno!=jobno)
				{
					ComputeSUBSEL_Percent(mts);
					writePAYJAMS(mts,lastperid,lastjobno,p);
					FreeSUBSEL(mts);
					mts=NULL;
					mts=SUBSELNEW(NULL);
					deletePAYJAMS(perid,jobno,p);
				}
				if(gamt!=0.0 || units!=0.0)
				{
				FINDFLDGETINT(dbsort->fileno,"SUBSTITUTE TRANSACTION NUMBER",&subtransno);
				ZERNRD(subpaid);
				FINDFLDSETSTRING(subpaid,"PERSONNEL IDENTIFICATION",perid);
				FINDFLDSETINT(subpaid,"TRANSACTION NUMBER",subtransno);
				FINDFLDSETSTRING(subpaid,"PAY IDENTIFICATION",pay_id);
				FINDFLDSETDOUBLE(subpaid,"UNITS",units);
				FINDFLDSETSTRING(subpaid,"SOURCE USER",USERLOGIN);
				field=FLDNRD(subpaid,"ENTRY DATE");
				if(field->len==8)
				{
					FINDFLDSETSTRING(subpaid,"ENTRY DATE",curdate);
				} else FINDFLDSETSTRING(subpaid,"ENTRY DATE",curdate10);
				FINDFLDSETSTRING(subpaid,"ENTRY TIME",curtime);
				ADVWRTTRANSsec(subpaid,1,"PAYMENT NUMBER",NULL,SubSelSubData,p);
				ZERNRD(subwrk);
				FINDFLDSETSTRING(subwrk,"PERSONNEL IDENTIFICATION",perid);
				FINDFLDSETINT(subwrk,"TRANSACTION NUMBER",subtransno);
				if(!ADVEQLNRDsec(subwrk,1,SubSelSubData,p))
				{
					LOCNRD(subwrk);
					prev_file=RDATDataNEW(subwrk);
					FINDFLDSETSTRING(subwrk,"PAY IDENTIFICATION",pay_id);
					FINDFLDGETDOUBLE(subwrk,"PAID UNITS",&tmpunits);
					FINDFLDGETDOUBLE(subwrk,"UNITS",&wrkunits);
					tmpunits+=units;
					FINDFLDSETDOUBLE(subwrk,"PAID UNITS",tmpunits);
					if((wrkunits-tmpunits)<=0.0)
					{
						FINDFLDSETCHAR(subwrk,"FINALED",TRUE);
						FINDFLDSETSTRING(subwrk,"FINALED BY",USERLOGIN);
						field=FLDNRD(subwrk,"FINALED DATE");
						if(field->len==8)
						{	
							FINDFLDSETSTRING(subwrk,"FINALED DATE",curdate);
						} else FINDFLDSETSTRING(subwrk,"FINALED DATE",curdate10);
						FINDFLDSETSTRING(subwrk,"FINALED TIME",curtime);
					}
					COPYFIELD(dbsort->fileno,subwrk,"RATE AMOUNT");
					ADVWRTTRANSsec(subwrk,0,NULL,prev_file,SubSelSubData,p);
					if(prev_file!=NULL) FreeRDATData(prev_file);
					UNLNRD(subwrk);
				}
				FINDFLDGETSTRING(dbsort->fileno,"EMPLOYEE IDENTIFICATION",&empperid);
				FINDFLDGETINT(dbsort->fileno,"EMPLOYEE JOB NUMBER",&empjobno);
				FINDFLDGETSHORT(dbsort->fileno,"GATHER TYPE",&mts->gather_type);
				FINDFLDGETSTRING(dbsort->fileno,"WORK POSITION IDENTIFICATION",&mts->wposid);
				y=GetSUBSEL(mts,empperid,empjobno,p);
				ComputeSUBSEL_Amount(mts,(y-1),empperid,empjobno,gamt);
				makeAccImages(mts,(y-1),empperid,empjobno,p);
/*
				writePAYJAMS(mts,perid,jobno,p);
*/
				if(RDAstrcmp(lastperid,perid))
				{
					if(valid_last_exists)
					{
						ZERNRD(payjprm);
						FINDFLDSETSTRING(payjprm,"PERSONNEL IDENTIFICATION",lastperid);
						FINDFLDSETINT(payjprm,"JOB NUMBER",lastjobno);
						FINDFLDSETSTRING(payjprm,"RATE IDENTIFICATION",lastrateid);
						FINDFLDSETSTRING(payjprm,"PAY IDENTIFICATION",pay_id);
						LOCNRDFILE(payjprm);
						if(!ADVEQLNRDsec(payjprm,1,SubSelSubData,p)) prev_file=RDATDataNEW(payjprm);
						else prev_file=NULL;

						FINDFLDSETCHAR(payjprm,"DELETEFLAG",deleteflag);
						FINDFLDSETDOUBLE(payjprm,"GROSS",rate_amt);
						FINDFLDSETDOUBLE(payjprm,"EARNINGS",rate_amt);
						FINDFLDSETDOUBLE(payjprm,"UNITS",rate_units);
						FINDFLDSETSTRING(payjprm,"SOURCE USER",USERLOGIN);
						FINDFLDSETCHAR(payjprm,"SOURCE USER VERIFIED",deleteflag);
						field=FLDNRD(payjprm,"ENTRY DATE");
						if(field->len==8)
						{
							FINDFLDSETSTRING(payjprm,"ENTRY DATE",curdate);
						} else FINDFLDSETSTRING(payjprm,"ENTRY DATE",curdate10);
						FINDFLDSETSTRING(payjprm,"ENTRY TIME",curtime);
						rate_to_use=round(rate_amt/rate_units);
						FINDFLDSETDOUBLE(payjprm,"RATE AMOUNT",rate_to_use);
						FINDFLDSETSHORT(payjprm,"SOURCE",src);
						ADVWRTTRANSsec(payjprm,0,NULL,prev_file,SubSelSubData,p);
						UNLNRDFILE(payjprm);
						if(prev_file!=NULL) FreeRDATData(prev_file);
						ZERNRD(paymstr);
						COPYFIELD(dbsort->fileno,paymstr,"PAY IDENTIFICATION");
						if(ADVEQLNRDsec(paymstr,1,SubSelSubData,p))
						{
							prev=NULL;
							KEYNRD(paymstr,1);
						} else {
							prev=RDATDataNEW(paymstr);
						}
						FINDFLDGETDOUBLE(paymstr,"TOTAL GROSS",&a);
						a+=rate_amt;
						FINDFLDSETDOUBLE(paymstr,"TOTAL GROSS",a);
						ADVWRTTRANSsec(paymstr,0,NULL,prev,SubSelSubData,p);
						if(prev!=NULL) FreeRDATData(prev);	
				
						if(lastrateid!=NULL) Rfree(lastrateid);
						lastrateid=stralloc(rateid);
						rate_amt=0;
						rate_units=0;
						ZERNRD(payjpms);
						FINDFLDSETSTRING(payjpms,"PERSONNEL IDENTIFICATION",lastperid);
						FINDFLDSETINT(payjpms,"JOB NUMBER",lastjobno);
						FINDFLDSETSTRING(payjpms,"PAY IDENTIFICATION",pay_id);
						LOCNRDFILE(payjpms);
						if(!ADVEQLNRDsec(payjpms,1,SubSelSubData,p)) prev_file=RDATDataNEW(payjpms);
						else prev_file=NULL;
						ZERNRD(payjmst);
						FINDFLDSETSTRING(payjmst,"PERSONNEL IDENTIFICATION",lastperid);
						FINDFLDSETINT(payjmst,"JOB NUMBER",lastjobno);
						if(EQLNRD(payjmst,1)) KEYNRD(payjmst,1);
						FINDFLDSETCHAR(payjpms,"DELETEFLAG",deleteflag);
						FINDFLDSETSTRING(payjpms,"PAY IDENTIFICATION",pay_id);
						FINDFLDSETDOUBLE(payjpms,"GROSS",job_amt);
						FINDFLDSETDOUBLE(payjpms,"EARNINGS",job_amt);
						FINDFLDSETDOUBLE(payjpms,"UNITS",job_units);
						FINDFLDSETSTRING(payjpms,"SOURCE USER",USERLOGIN);
						FINDFLDSETSTRING(payjpms,"BANK IDENTIFICATION",bank_id);
						COPYFIELD(payjmst,payjpms,"POSITION IDENTIFICATION");
						COPYFIELD(payjmst,payjpms,"GROSS IDENTIFICATION");
						COPYFIELD(payjmst,payjpms,"GRADE");
						COPYFIELD(payjmst,payjpms,"STEP");
						COPYFIELD(payjmst,payjpms,"STEP ALPHA");
						COPYFIELD(payjmst,payjpms,"LIFE INSURANCE SALARY");
						COPYFIELD(payjmst,payjpms,"RETIREMENT SALARY");
						COPYFIELD(payjmst,payjpms,"FTE AMOUNT");
						COPYFIELD(payjmst,payjpms,"MISC SALARY AMOUNT");
						COPYFIELD(payjmst,payjpms,"MISC UNIT AMOUNT");
						COPYFIELD(payjmst,payjpms,"EXTRA SERVICE AMOUNT");
						field=FLDNRD(payjpms,"ENTRY DATE");
						if(field->len==8)
						{
							FINDFLDSETSTRING(payjpms,"ENTRY DATE",curdate);
						} else FINDFLDSETSTRING(payjpms,"ENTRY DATE",curdate10);
						FINDFLDSETSTRING(payjpms,"ENTRY TIME",curtime);
						if(PAYROLL_SETUP->subsel_audit)
						{
							FINDFLDSETCHAR(payjpms,"SOURCE USER VERIFIED",FALSE);
						} else {
							FINDFLDSETCHAR(payjpms,"SOURCE USER VERIFIED",TRUE);
						}
						ADVWRTTRANSsec(payjpms,0,NULL,prev_file,SubSelSubData,p);
						UNLNRDFILE(payjpms);
						if(prev_file!=NULL) FreeRDATData(prev_file);
						job_amt=0;
						job_units=0;
						lastjobno=jobno;
						FINDFLDGETDOUBLE(dbsort->fileno,"RATE AMOUNT",&last_rate_amt);	
						ZERNRD(payppms);
						FINDFLDSETSTRING(payppms,"PERSONNEL IDENTIFICATION",lastperid);
						FINDFLDSETSTRING(payppms,"PAY IDENTIFICATION",pay_id);
						LOCNRDFILE(payppms);
						if(!ADVEQLNRDsec(payppms,1,SubSelSubData,p)) prev_file=RDATDataNEW(payppms);
						else prev_file=NULL;
						FINDFLDSETCHAR(payppms,"DELETEFLAG",deleteflag);
						FINDFLDSETCHAR(payppms,"BENEFITS COMPUTED",FALSE);
						FINDFLDSETSTRING(payppms,"SOURCE USER",USERLOGIN);
						FINDFLDSETCHAR(payppms,"MADE TRANSACTIONS",FALSE);
						FINDFLDGETDOUBLE(payppms,"GROSS",&amt);
						amt+=per_gross;
						FINDFLDSETDOUBLE(payppms,"GROSS",amt);
						FINDFLDGETDOUBLE(payppms,"NET",&amt);
						amt+=per_net;
						FINDFLDSETDOUBLE(payppms,"NET",amt);
						ADVWRTTRANSsec(payppms,0,NULL,prev_file,SubSelSubData,p);
						UNLNRDFILE(payppms);
						if(prev_file!=NULL) FreeRDATData(prev_file);
					}
					if(lastperid!=NULL) Rfree(lastperid);
					lastperid=stralloc(perid);
					per_gross=0.0;
				} else if(lastjobno!=jobno)
				{
					if(valid_last_exists)
					{
						ZERNRD(payjprm);
						FINDFLDSETSTRING(payjprm,"PERSONNEL IDENTIFICATION",lastperid);
						FINDFLDSETINT(payjprm,"JOB NUMBER",lastjobno);
						FINDFLDSETSTRING(payjprm,"RATE IDENTIFICATION",lastrateid);
						FINDFLDSETSTRING(payjprm,"PAY IDENTIFICATION",pay_id);
						LOCNRDFILE(payjprm);
						if(!ADVEQLNRDsec(payjprm,1,SubSelSubData,p)) prev_file=RDATDataNEW(payjprm);
						else prev_file=NULL;
						FINDFLDSETCHAR(payjprm,"DELETEFLAG",deleteflag);
						FINDFLDSETDOUBLE(payjprm,"GROSS",rate_amt);
						FINDFLDSETDOUBLE(payjprm,"EARNINGS",rate_amt);
						FINDFLDSETDOUBLE(payjprm,"UNITS",rate_units);
						FINDFLDSETSTRING(payjprm,"SOURCE USER",USERLOGIN);
						FINDFLDSETCHAR(payjprm,"SOURCE USER VERIFIED",deleteflag);
						field=FLDNRD(payjprm,"ENTRY DATE");
						if(field->len==8)
						{
							FINDFLDSETSTRING(payjprm,"ENTRY DATE",curdate);
						} else FINDFLDSETSTRING(payjprm,"ENTRY DATE",curdate10);
						FINDFLDSETSTRING(payjprm,"ENTRY TIME",curtime);
						rate_to_use=round(rate_amt/rate_units);
						FINDFLDSETDOUBLE(payjprm,"RATE AMOUNT",rate_to_use);
						FINDFLDSETSHORT(payjprm,"SOURCE",src);
						ADVWRTTRANSsec(payjprm,0,NULL,prev_file,SubSelSubData,p);
						UNLNRDFILE(payjprm);
						if(prev_file!=NULL) FreeRDATData(prev_file);
						ZERNRD(paymstr);
						COPYFIELD(dbsort->fileno,paymstr,"PAY IDENTIFICATION");
						if(ADVEQLNRDsec(paymstr,1,SubSelSubData,p))
						{
							prev=NULL;
							KEYNRD(paymstr,1);
						} else {
							prev=RDATDataNEW(paymstr);
						}
						FINDFLDGETDOUBLE(paymstr,"TOTAL GROSS",&a);
						a+=rate_amt;
						FINDFLDSETDOUBLE(paymstr,"TOTAL GROSS",a);
						ADVWRTTRANSsec(paymstr,0,NULL,prev,SubSelSubData,p);
						if(prev!=NULL) FreeRDATData(prev);	
				
						if(lastrateid!=NULL) Rfree(lastrateid);
						lastrateid=stralloc(rateid);
						rate_units=0;
						rate_amt=0;
						ZERNRD(payjpms);
						FINDFLDSETSTRING(payjpms,"PERSONNEL IDENTIFICATION",lastperid);
						FINDFLDSETINT(payjpms,"JOB NUMBER",lastjobno);
						FINDFLDSETSTRING(payjpms,"PAY IDENTIFICATION",pay_id);
						LOCNRDFILE(payjpms);
						if(!ADVEQLNRDsec(payjpms,1,SubSelSubData,p)) prev_file=RDATDataNEW(payjpms);
						else prev_file=NULL;
						FINDFLDSETCHAR(payjpms,"DELETEFLAG",deleteflag);
						FINDFLDSETDOUBLE(payjpms,"GROSS",job_amt);
						FINDFLDSETDOUBLE(payjpms,"EARNINGS",job_amt);
						FINDFLDSETDOUBLE(payjpms,"UNITS",job_units);
						field=FLDNRD(payjpms,"ENTRY DATE");
						if(field->len==8)
						{
							FINDFLDSETSTRING(payjpms,"ENTRY DATE",curdate);
						} else FINDFLDSETSTRING(payjpms,"ENTRY DATE",curdate10);
						ZERNRD(payjmst);
						FINDFLDSETSTRING(payjmst,"PERSONNEL IDENTIFICATION",lastperid);
						FINDFLDSETINT(payjmst,"JOB NUMBER",lastjobno);
						if(EQLNRD(payjmst,1)) KEYNRD(payjmst,1);
						FINDFLDSETSTRING(payjpms,"ENTRY TIME",curtime);
						FINDFLDSETSTRING(payjpms,"SOURCE USER",USERLOGIN);
						FINDFLDSETSTRING(payjpms,"BANK IDENTIFICATION",bank_id);
						COPYFIELD(payjmst,payjpms,"POSITION IDENTIFICATION");
						COPYFIELD(payjmst,payjpms,"GROSS IDENTIFICATION");
						COPYFIELD(payjmst,payjpms,"GRADE");
						COPYFIELD(payjmst,payjpms,"STEP");
						COPYFIELD(payjmst,payjpms,"STEP ALPHA");
						COPYFIELD(payjmst,payjpms,"LIFE INSURANCE SALARY");
						COPYFIELD(payjmst,payjpms,"RETIREMENT SALARY");
						COPYFIELD(payjmst,payjpms,"FTE AMOUNT");
						COPYFIELD(payjmst,payjpms,"MISC SALARY AMOUNT");
						COPYFIELD(payjmst,payjpms,"MISC UNIT AMOUNT");
						COPYFIELD(payjmst,payjpms,"EXTRA SERVICE AMOUNT");
						if(PAYROLL_SETUP->subsel_audit)
						{
							FINDFLDSETCHAR(payjpms,"SOURCE USER VERIFIED",FALSE);
						} else {
							FINDFLDSETCHAR(payjpms,"SOURCE USER VERIFIED",TRUE);
						}
						ADVWRTTRANSsec(payjpms,0,NULL,prev_file,SubSelSubData,p);
						UNLNRDFILE(payjpms);
						if(prev_file!=NULL) FreeRDATData(prev_file);
						job_amt=0;
						job_units=0;
					}
					lastjobno=jobno;
					FINDFLDGETDOUBLE(dbsort->fileno,"RATE AMOUNT",&last_rate_amt);
				} else if(RDAstrcmp(rateid,lastrateid))
				{
					ZERNRD(payjprm);
					FINDFLDSETSTRING(payjprm,"PERSONNEL IDENTIFICATION",lastperid);
					FINDFLDSETINT(payjprm,"JOB NUMBER",lastjobno);
					FINDFLDSETSTRING(payjprm,"RATE IDENTIFICATION",lastrateid);
					FINDFLDSETSTRING(payjprm,"PAY IDENTIFICATION",pay_id);
					LOCNRDFILE(payjprm);
					if(!ADVEQLNRDsec(payjprm,1,SubSelSubData,p)) prev_file=RDATDataNEW(payjprm);
					else prev_file=NULL;
					FINDFLDSETCHAR(payjprm,"DELETEFLAG",deleteflag);
					FINDFLDSETDOUBLE(payjprm,"GROSS",rate_amt);
					FINDFLDSETDOUBLE(payjprm,"EARNINGS",rate_amt);
					FINDFLDSETDOUBLE(payjprm,"UNITS",rate_units);
					rate_to_use=round(rate_amt/rate_units);
					FINDFLDSETSTRING(payjprm,"SOURCE USER",USERLOGIN);
					FINDFLDSETCHAR(payjprm,"SOURCE USER VERIFIED",deleteflag);
					field=FLDNRD(payjprm,"ENTRY DATE");
					if(field->len==8)
					{
						FINDFLDSETSTRING(payjprm,"ENTRY DATE",curdate);
					} else FINDFLDSETSTRING(payjprm,"ENTRY DATE",curdate10);
					FINDFLDSETSTRING(payjprm,"ENTRY TIME",curtime);
					FINDFLDSETDOUBLE(payjprm,"RATE AMOUNT",rate_to_use);
					FINDFLDSETSHORT(payjprm,"SOURCE",src);
					ADVWRTTRANSsec(payjprm,0,NULL,prev_file,SubSelSubData,p);
					UNLNRDFILE(payjprm);
					if(prev_file!=NULL) FreeRDATData(prev_file);
					ZERNRD(paymstr);
					COPYFIELD(dbsort->fileno,paymstr,"PAY IDENTIFICATION");
					if(ADVEQLNRDsec(paymstr,1,SubSelSubData,p))
					{
						prev=NULL;
						KEYNRD(paymstr,1);
					} else {
						prev=RDATDataNEW(paymstr);
					}
					FINDFLDGETDOUBLE(paymstr,"TOTAL GROSS",&a);
					a+=rate_amt;
					FINDFLDSETDOUBLE(paymstr,"TOTAL GROSS",a);
					ADVWRTTRANSsec(paymstr,0,NULL,prev,SubSelSubData,p);
					if(prev!=NULL) FreeRDATData(prev);	
				
					if(lastrateid!=NULL) Rfree(lastrateid);
					lastrateid=stralloc(rateid);
					rate_units=0;
					rate_amt=0;
				}
				per_gross+=gamt;
				job_amt+=gamt;
				job_units+=units;
				rate_amt+=gamt;
				rate_units+=units;
				update_diag=TRUE;
				DO_AUDIT=TRUE;
				valid_last_exists=TRUE;
				}
			}
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(update_diag ? TRUE:FALSE));
			if(curdate!=NULL) Rfree(curdate);
			if(curdate10!=NULL) Rfree(curdate10);
			ef=NXTNRD(dbsort->fileno,1);
		}
		if(valid_last_exists)
		{
/*
			if(RDAstrcmp(lastperid,perid) || lastjobno!=jobno)
			{
*/
				ComputeSUBSEL_Percent(mts);
				writePAYJAMS(mts,lastperid,lastjobno,p);
				FreeSUBSEL(mts);
				mts=NULL;
/*
			}
*/
			deleteflag=FALSE;
			curdate=GETCURRENTDATE();
			curdate10=GETCURRENTDATE10();
			ZERNRD(payjprm);
			FINDFLDSETSTRING(payjprm,"PERSONNEL IDENTIFICATION",lastperid);
			FINDFLDSETINT(payjprm,"JOB NUMBER",lastjobno);
			FINDFLDSETSTRING(payjprm,"RATE IDENTIFICATION",lastrateid);
			FINDFLDSETSTRING(payjprm,"PAY IDENTIFICATION",pay_id);
			LOCNRDFILE(payjprm);
			if(!ADVEQLNRDsec(payjprm,1,SubSelSubData,p)) prev_file=RDATDataNEW(payjprm);
			else prev_file=NULL;
			FINDFLDSETCHAR(payjprm,"DELETEFLAG",FALSE);
			FINDFLDSETDOUBLE(payjprm,"GROSS",rate_amt);
			FINDFLDSETDOUBLE(payjprm,"EARNINGS",rate_amt);
			FINDFLDSETDOUBLE(payjprm,"UNITS",rate_units);
			FINDFLDSETSTRING(payjprm,"SOURCE USER",USERLOGIN);
			FINDFLDSETCHAR(payjprm,"SOURCE USER VERIFIED",deleteflag);
			field=FLDNRD(payjprm,"ENTRY DATE");
			if(field->len==8)
			{
				FINDFLDSETSTRING(payjprm,"ENTRY DATE",curdate);
			} else FINDFLDSETSTRING(payjprm,"ENTRY DATE",curdate10);
			FINDFLDSETSTRING(payjprm,"ENTRY TIME",curtime);
			rate_to_use=round(rate_amt/rate_units);
			FINDFLDSETDOUBLE(payjprm,"RATE AMOUNT",rate_to_use);
			FINDFLDSETSHORT(payjprm,"SOURCE",src);
			ADVWRTTRANSsec(payjprm,0,NULL,prev_file,SubSelSubData,p);
			UNLNRDFILE(payjprm);
			if(prev_file!=NULL) FreeRDATData(prev_file);
			ZERNRD(paymstr);
			COPYFIELD(dbsort->fileno,paymstr,"PAY IDENTIFICATION");
			if(ADVEQLNRDsec(paymstr,1,SubSelSubData,p))
			{
				prev=NULL;
				KEYNRD(paymstr,1);
			} else {
				prev=RDATDataNEW(paymstr);
			}
			FINDFLDGETDOUBLE(paymstr,"TOTAL GROSS",&a);
			a+=rate_amt;
			FINDFLDSETDOUBLE(paymstr,"TOTAL GROSS",a);
			ADVWRTTRANSsec(paymstr,0,NULL,prev,SubSelSubData,p);
			if(prev!=NULL) FreeRDATData(prev);	
			if(lastrateid!=NULL) Rfree(lastrateid);
			lastrateid=stralloc(rateid);
			rate_amt=0;
			rate_units=0;
			ZERNRD(payjpms);
			FINDFLDSETSTRING(payjpms,"PERSONNEL IDENTIFICATION",lastperid);
			FINDFLDSETINT(payjpms,"JOB NUMBER",lastjobno);
			FINDFLDSETSTRING(payjpms,"PAY IDENTIFICATION",pay_id);
			LOCNRDFILE(payjpms);
			if(!ADVEQLNRDsec(payjpms,1,SubSelSubData,p)) prev_file=RDATDataNEW(payjpms);
			else prev_file=NULL;
			ZERNRD(payjmst);
			FINDFLDSETSTRING(payjmst,"PERSONNEL IDENTIFICATION",lastperid);
			FINDFLDSETINT(payjmst,"JOB NUMBER",lastjobno);
			if(EQLNRD(payjmst,1)) KEYNRD(payjmst,1);
			FINDFLDSETCHAR(payjpms,"DELETEFLAG",FALSE);
			FINDFLDSETDOUBLE(payjpms,"GROSS",job_amt);
			FINDFLDSETDOUBLE(payjpms,"EARNINGS",job_amt);
			FINDFLDSETDOUBLE(payjpms,"UNITS",job_units);
			field=FLDNRD(payjpms,"ENTRY DATE");
			if(field->len==8)
			{
				FINDFLDSETSTRING(payjpms,"ENTRY DATE",curdate);
			} else FINDFLDSETSTRING(payjpms,"ENTRY DATE",curdate10);
			FINDFLDSETSTRING(payjpms,"ENTRY TIME",curtime);
			FINDFLDSETSTRING(payjpms,"SOURCE USER",USERLOGIN);
			FINDFLDSETSTRING(payjpms,"BANK IDENTIFICATION",bank_id);
			COPYFIELD(payjmst,payjpms,"POSITION IDENTIFICATION");
			COPYFIELD(payjmst,payjpms,"GROSS IDENTIFICATION");
			COPYFIELD(payjmst,payjpms,"GRADE");
			COPYFIELD(payjmst,payjpms,"STEP");
			COPYFIELD(payjmst,payjpms,"STEP ALPHA");
			COPYFIELD(payjmst,payjpms,"LIFE INSURANCE SALARY");
			COPYFIELD(payjmst,payjpms,"RETIREMENT SALARY");
			COPYFIELD(payjmst,payjpms,"FTE AMOUNT");
			COPYFIELD(payjmst,payjpms,"MISC SALARY AMOUNT");
			COPYFIELD(payjmst,payjpms,"MISC UNIT AMOUNT");
			COPYFIELD(payjmst,payjpms,"EXTRA SERVICE AMOUNT");
			if(PAYROLL_SETUP->subsel_audit)
			{
				FINDFLDSETCHAR(payjpms,"SOURCE USER VERIFIED",FALSE);
			} else {
				FINDFLDSETCHAR(payjpms,"SOURCE USER VERIFIED",TRUE);
			}
			ADVWRTTRANSsec(payjpms,0,NULL,prev_file,SubSelSubData,p);
			if(prev_file!=NULL) FreeRDATData(prev_file);
			UNLNRDFILE(payjpms);
			job_amt=0;
			job_units=0;
			lastjobno=jobno;
			FINDFLDGETDOUBLE(dbsort->fileno,"RATE AMOUNT",&last_rate_amt);
			ZERNRD(payppms);
			FINDFLDSETSTRING(payppms,"PERSONNEL IDENTIFICATION",lastperid);
			FINDFLDSETSTRING(payppms,"PAY IDENTIFICATION",pay_id);
			LOCNRDFILE(payppms);
			if(!ADVEQLNRDsec(payppms,1,SubSelSubData,p)) prev_file=RDATDataNEW(payppms);
			else prev_file=NULL;
			FINDFLDSETCHAR(payppms,"DELETEFLAG",FALSE);
			FINDFLDSETCHAR(payppms,"BENEFITS COMPUTED",deleteflag);
			FINDFLDSETSTRING(payppms,"SOURCE USER",USERLOGIN);
			FINDFLDSETCHAR(payppms,"MADE TRANSACTIONS",deleteflag);
			FINDFLDGETDOUBLE(payppms,"GROSS",&amt);
			amt+=per_gross;
			FINDFLDSETDOUBLE(payppms,"GROSS",amt);
			FINDFLDGETDOUBLE(payppms,"NET",&amt);
			amt+=per_net;
			FINDFLDSETDOUBLE(payppms,"NET",amt);
			ADVWRTTRANSsec(payppms,0,NULL,prev_file,SubSelSubData,p);
			UNLNRDFILE(payppms);
			if(prev_file!=NULL) FreeRDATData(prev_file);
		}
		if(mts!=NULL) FreeSUBSEL(mts);
		if(perid!=NULL) Rfree(perid);
		if(empperid!=NULL) Rfree(empperid);
		if(lastperid!=NULL) Rfree(lastperid);
		if(lastrateid!=NULL) Rfree(lastrateid);
		if(rateid!=NULL) Rfree(rateid);
		if(curdate!=NULL) Rfree(curdate);
		if(curdate10!=NULL) Rfree(curdate10);
		if(curtime!=NULL) Rfree(curtime);
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
		}
		CLSDBsort(dbsort);
		FreeDBsort(dbsort);
		if(p!=NULL) Rfree(p);
	}
	if(errorlist->numlibs<1) addAPPlib(errorlist,"No Errors Detected");
        FINDRSCLISTAPPlib(mainrsrc,"ERROR LIST",0,errorlist);
        updatersrc(mainrsrc,"ERROR LIST");
	if(warnlist->numlibs<1) addAPPlib(warnlist,"No Warnings Detected");
        FINDRSCLISTAPPlib(mainrsrc,"WARNING LIST",0,warnlist);
        updatersrc(mainrsrc,"WARNING LIST");
}
void makeDBsortfile(DBsort *dbsrt)
{
	DFvirtual *d=NULL;
	NRDkey *key=NULL;
	NRDpart *part;
	NRDfield *field;
	int x=0;
	short keyno=(-1),keyno2=(-1);

	addDBfield(dbsrt,"DELETEFLAG",BOOLNS,1);
	keyno=addDBkey(dbsrt,"SORT FILE MAIN KEY");
	d=getDFvirtual(module,"SELECT SUBSTITUTE PAYROLL SORT SEQUENCE");
	if(d!=NULL)
	{
		csortsequence=Rmalloc(sizeof(RDAvirtual));
		csortsequence->name=stralloc(d->name);
		csortsequence->when=0;
		csortsequence->len=d->length;
		csortsequence->type=d->type;
		if(d->expression!=NULL) 
		{
			csortsequence->expression=stralloc(d->expression);
		} else { 
			csortsequence->expression=NULL;
		}
		csortsequence->computed=FALSE;
		csortsequence->range=FALSE;
		csortsequence->dlen=0;
		switch(csortsequence->type)
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
				csortsequence->value.string_value=NULL;
				csortsequence->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				csortsequence->value.string_value=Rmalloc(1);
				*csortsequence->value.string_value=0;
				csortsequence->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				csortsequence->value.float_value=Rmalloc(sizeof(double));
				*csortsequence->value.float_value=0;
				csortsequence->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				csortsequence->value.short_value=Rmalloc(sizeof(short));
				*csortsequence->value.short_value=0;
				csortsequence->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				csortsequence->value.integer_value=Rmalloc(sizeof(int));
				*csortsequence->value.integer_value=0;
				csortsequence->dlen=sizeof(int);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",csortsequence->type,csortsequence->name);
				break;
		}
		addDBfield(dbsrt,csortsequence->name,csortsequence->type,
			csortsequence->len);
		addDBkeypart(dbsrt,keyno,csortsequence->name);
		FreeDFvirtual(d);
	}
	keyno2=KEYNUMBER(payjprm,"PAYJPRM KEY");
	if(keyno2==(-1))
	{
		keyno2=1;
	}
	key=KEYNUM(payjprm,keyno2);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			field=FLDNRD(payjprm,part->name);
			if(field!=NULL) 
				addDBfield(dbsrt,field->name,field->type,field->len);
			addDBkeypart(dbsrt,keyno,part->name);
		}
	}
	addDBfield(dbsrt,"TRANSACTION NUMBER",LONGV,6);
	addDBkeypart(dbsrt,keyno,"TRANSACTION NUMBER");
	addDBfilefield(dbsrt,pgross,"GROSS IDENTIFICATION");
	addDBfilefield(dbsrt,payjmst,"POSITION IDENTIFICATION");
	addDBfield(dbsrt,"WORK POSITION IDENTIFICATION",1,30);
	addDBfilefield(dbsrt,posrate,"DESCRIPTION");
	addDBfield(dbsrt,"RATE AMOUNT",DOLLARS,20);
	addDBfield(dbsrt,"UNITS",SDECIMALV,20);
	addDBfield(dbsrt,"CHECK GROSS",DOLLARS,20);
	addDBfield(dbsrt,"GATHER TYPE",SHORTV,2);
	addDBfield(dbsrt,"RATE TYPE",BOOLNS,1);
	addDBfilefield(dbsrt,subwrk,"EMPLOYEE IDENTIFICATION");
	addDBfield(dbsrt,"EMPLOYEE JOB NUMBER",LONGV,6);
	addDBfield(dbsrt,"SUBSTITUTE TRANSACTION NUMBER",LONGV,6);
	addDBfield(dbsrt,"STARTING DATE",DATES,10);
	addDBfield(dbsrt,"ENDING DATE",DATES,10);
	addDBfield(dbsrt,"ENTRY DATE",DATES,10);
}
void addsortrecord(DBsort *dbsrt,RDArsrc *mainrsrc,char *subperid,int subjobno,
	int subtransno,char *payid,char *subposid,char *rateid,char *grossid,
	double units,double gross,double rate_amt,short gather_type,char *wpid,
	char *perid,int jobno,char ratetype,PassableStruct *p)
{
	char deleteflag=FALSE;

	FINDFLDSETCHAR(dbsrt->fileno,"DELETEFLAG",deleteflag);
	if(csortsequence!=NULL)
	{
		computevirtual(csortsequence,SubSelSubData,p);
		switch(csortsequence->type)
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
				FINDFLDSETSTRING(dbsrt->fileno,csortsequence->name,
					csortsequence->value.string_value);
				break;
			case BOOLNS:
			case CHARACTERS:
				FINDFLDSETCHAR(dbsrt->fileno,csortsequence->name,
					*csortsequence->value.string_value);
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case SDECIMALV:
				FINDFLDSETDOUBLE(dbsrt->fileno,csortsequence->name,
					*csortsequence->value.float_value);
				break;
			case SHORTV:
			case SSHORTV:
				FINDFLDSETSHORT(dbsrt->fileno,csortsequence->name,
					*csortsequence->value.short_value);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				FINDFLDSETINT(dbsrt->fileno,csortsequence->name,
					*csortsequence->value.integer_value);
				break;
			default:
				prterr("Error virtual field type [%d] not found for field [%s].",csortsequence->type,csortsequence->name);
				break;
		}
	}
	FINDFLDSETSTRING(dbsrt->fileno,"PERSONNEL IDENTIFICATION",subperid);
	FINDFLDSETINT(dbsrt->fileno,"JOB NUMBER",subjobno);
	FINDFLDSETSTRING(dbsrt->fileno,"PAY IDENTIFICATION",payid);
	FINDFLDSETSTRING(dbsrt->fileno,"RATE IDENTIFICATION",rateid);
	FINDFLDSETSTRING(dbsrt->fileno,"POSITION IDENTIFICATION",subposid);
	FINDFLDSETCHAR(dbsrt->fileno,"RATE TYPE",ratetype);
	FINDFLDSETSTRING(dbsrt->fileno,"WORK POSITION IDENTIFICATION",wpid);
	FINDFLDSETSTRING(dbsrt->fileno,"GROSS IDENTIFICATION",grossid);
	FINDFLDSETDOUBLE(dbsrt->fileno,"UNITS",units);
	FINDFLDSETDOUBLE(dbsrt->fileno,"CHECK GROSS",gross);
	FINDFLDSETDOUBLE(dbsrt->fileno,"RATE AMOUNT",rate_amt);
	FINDFLDSETSTRING(dbsrt->fileno,"EMPLOYEE IDENTIFICATION",perid);
	FINDFLDSETSHORT(dbsrt->fileno,"GATHER TYPE",gather_type);
	FINDFLDSETINT(dbsrt->fileno,"EMPLOYEE JOB NUMBER",jobno);
	FINDFLDSETINT(dbsrt->fileno,"SUBSTITUTE TRANSACTION NUMBER",subtransno);
	COPYFIELD(subwrk,dbsrt->fileno,"STARTING DATE");
	COPYFIELD(subwrk,dbsrt->fileno,"ENDING DATE");
	COPYFIELD(subwrk,dbsrt->fileno,"ENTRY DATE");
	WRTNRD(dbsrt->fileno,1,"TRANSACTION NUMBER");
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	selectm(0,targetkey,0,NULL,blist->funcargs);
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	selectm(2,NULL,0,NULL,blist->funcargs);
} 
static void doexit_test(MakeBrowseList *blist)
{
	DBsort *dbsort=NULL;

	if(blist!=NULL)
	{
		dbsort=blist->funcargs;
		if(dbsort!=NULL)
		{
			WARNINGDIALOG("ABORT WARNING DIALOG SCREEN","RECORDS NOT PROCESSED!",
			"Are you sure that you want to quit without processing the Payroll Select Records?",
			doexit,NULL,FALSE,blist);
		} else {
			doexit(blist);
		}
	}
}
static void doexit(MakeBrowseList *blist)
{
	DBsort *dbsort=NULL;
	RDArsrc *brsrc=NULL,*srsrc=NULL,*drsrc=NULL;

	if(blist!=NULL)
	{
		dbsort=blist->funcargs;
		brsrc=blist->mainrsrc;
		drsrc=blist->definelist;
		srsrc=blist->searchrsrc;
		FREE_BROWSE_EXIT(blist);
		if(drsrc!=NULL) 
		{
			killwindow(drsrc);
			free_rsrc(drsrc);
		}
		if(srsrc!=NULL) 
		{
			killwindow(srsrc);
			free_rsrc(srsrc);
		}
		if(brsrc!=NULL)
		{
			killwindow(brsrc);
			free_rsrc(brsrc);
		}
		mbl_trans=NULL;
		if(dbsort!=NULL)
		{
			CLSDBsort(dbsort);
			FreeDBsort(dbsort);
		}
	}
	if(errorlist->numlibs<1) addAPPlib(errorlist,"No Errors Detected");
        FINDRSCLISTAPPlib(mainrsrc,"ERROR LIST",0,errorlist);
        updatersrc(mainrsrc,"ERROR LIST");
	if(warnlist->numlibs<1) addAPPlib(warnlist,"No Warnings Detected");
        FINDRSCLISTAPPlib(mainrsrc,"WARNING LIST",0,warnlist);
        updatersrc(mainrsrc,"WARNING LIST");
}
static void saveexit(RDArsrc *mainrsrc,MakeBrowseList *blist)
{
	DBsort *dbsort=NULL;
	RDArsrc *brsrc=NULL,*srsrc=NULL,*drsrc=NULL;

	if(blist!=NULL)
	{
		dbsort=blist->funcargs;
		brsrc=blist->mainrsrc;
		drsrc=blist->definelist;
		srsrc=blist->searchrsrc;
		FREE_BROWSE_EXIT(blist);
		if(drsrc!=NULL) 
		{
			killwindow(drsrc);
			free_rsrc(drsrc);
		}
		if(srsrc!=NULL) 
		{
			killwindow(srsrc);
			free_rsrc(srsrc);
		}
		if(WindowCount<=2) select_backend(dbsort);
		if(brsrc!=NULL)
		{
			free_rsrc(brsrc);
		}
	}
}
static short selectfunction(MakeBrowseList *b)
{
	return(TRUE);
}
void selectsub(RDArsrc *mainrsrc)
{
	RDArsrc *tmprsrc=NULL,*brsrc=NULL,*srsrc=NULL,*drsrc=NULL;
	RangeScreen *rs=NULL;
	DBsort *dbsort=NULL;
	short ef=FALSE,etest=FALSE,dodiag=FALSE,ef1=FALSE,testfile=FALSE;
	short gather_type=0;
	double units=0.0,paid_units=0.0,unpaid_units=0.0;
	char *subposid=NULL,*grossid=NULL,*grossid1=NULL,*exprssn=NULL,*rateid=NULL;
	char *locid=NULL,*perid=NULL,*wposid=NULL,*substatus=NULL;
	char deleteflag=FALSE,finaled=FALSE,*sortname=NULL;
	char active=TRUE,*subperid=NULL,suv=FALSE;
	char *e=NULL,*subrateid=NULL;
	int subjobno=0,jobno=0,subtransno=0,fyear=0,cyear=0,x;
	long size_of_file=0;
	PassableStruct *p=NULL;
	RDAvirtual *v=NULL;
	char aflag=FALSE,use_aflag=FALSE,select_complete=FALSE;
	char employee_payable=FALSE;
	NRDfield *f=NULL;

	printed_errorlist=FALSE;
	printed_warnlist=FALSE;
	if(errorlist->numlibs>0)
	{
		freeapplib(errorlist);
		errorlist=APPlibNEW();
	}
	if(warnlist->numlibs>0)
	{
		freeapplib(warnlist);
		warnlist=APPlibNEW();
	}
        FINDRSCLISTAPPlib(mainrsrc,"ERROR LIST",0,errorlist);
        updatersrc(mainrsrc,"ERROR LIST");
        FINDRSCLISTAPPlib(mainrsrc,"WARNING LIST",0,warnlist);
        updatersrc(mainrsrc,"WARNING LIST");
	f=FLDNRD(pgrat,"ACTIVE");
	if(f!=NULL) use_aflag=TRUE;
	readallwidgets(mainrsrc);
	FINDRSCGETSTRING(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",&bank_id);
	FINDRSCGETSTRING(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]",&pay_id);
	FINDRSCGETINT(mainrsrc,"[PAYMSTR][FISCAL YEAR]",&fyear);
	FINDRSCGETINT(mainrsrc,"[PAYMSTR][CALENDAR YEAR]",&cyear);
	FINDRSCGETINT(mainrsrc,"SELECT TYPE",&select_type);
	FINDRSCGETSTRING(mainrsrc,"EXPRESSION",&select_expression);
	sortname=unique_name();
	dbsort=DBsortNEW(module,sortname,NRDRdaEngine);
	if(sortname!=NULL) Rfree(sortname);
	makeDBsortfile(dbsort);
	dbsort->fileno=OPNDBsort(dbsort);
	rs=RangeScreenNew(mainrsrc);
	ReadRangeScreen(submstr,rs);
	ReadRangeScreen(subwrk,rs);
	ReadRangeScreen(payjmst,rs);
	ReadRangeScreen(prsnnl,rs);
	ReadRangeScreen(perstat,rs);
	ReadRangeScreen(posmstr,rs);
	ReadRangeScreen(pgross,rs);
	if(mainrsrc->virflds!=NULL)
	{
		for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
		{
			ReadVirtualRangeStruct(v,rs);
		}
	}
	tmprsrc=diagscrn(subwrk,"DIAGNOSTIC SCREEN",module,
		"Selecting Substitute Payroll Jobs",NULL);
	if(tmprsrc!=NULL)
	{
		if(!ADVmakescrn(tmprsrc,TRUE))
		{
			ForceWindowUpdate(tmprsrc);
		} else {
			e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
			sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
			prterr("Error %s.",e);
			if(e!=NULL) Rfree(e);
		}
	}
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mainrsrc;
	p->a=(DBsort *)dbsort;
	ZERNRD(subwrk);
	ef=ADVFRSNRDsec(subwrk,1,SubSelSubData,p);
	while(!ef)
	{
		dodiag=FALSE;
		FINDFLDGETSTRING(subwrk,"PERSONNEL IDENTIFICATION",&subperid);
		FINDFLDGETINT(subwrk,"TRANSACTION NUMBER",&subtransno);
		FINDFLDGETCHAR(subwrk,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
		FINDFLDGETCHAR(subwrk,"SOURCE USER VERIFIED",&suv);
		FINDFLDGETCHAR(subwrk,"FINALED",&finaled);
		ZERNRD(payppms);
		FINDFLDSETSTRING(payppms,"PERSONNEL IDENTIFICATION",subperid);
		FINDFLDSETSTRING(payppms,"PAY IDENTIFICATION",pay_id);
		if(!ADVEQLNRDsec(payppms,1,SubSelSubData,p)) 
		{
			FINDFLDGETCHAR(payppms,"DELETEFLAG",&deleteflag);
			FINDFLDGETCHAR(payppms,"SELECT COMPLETED",&select_complete);
			if(deleteflag==TRUE) select_complete=FALSE;
		}  else select_complete=FALSE;
		if(!finaled && suv==TRUE && !select_complete)
		{
		testfile=testfilerangersrc(subwrk,rs);
		if(!testfile)
		{
			units=0.0;
			paid_units=0.0;
			unpaid_units=0.0;
			FINDFLDGETDOUBLE(subwrk,"UNITS",&units);
			FINDFLDGETDOUBLE(subwrk,"PAID UNITS",&paid_units);
			unpaid_units=units-paid_units;
			if(unpaid_units!=0.0)
			{
			FINDFLDGETSTRING(subwrk,"PERSONNEL IDENTIFICATION",&subperid);
			FINDFLDGETSTRING(subwrk,"EMPLOYEE IDENTIFICATION",&perid);
			FINDFLDGETINT(subwrk,"EMPLOYEE JOB NUMBER",&jobno);
			FINDFLDSETSTRING(submstr,"PERSONNEL IDENTIFICATION",subperid);
			if(!ADVEQLNRDsec(submstr,1,SubSelSubData,p))
			{
			FINDFLDGETCHAR(submstr,"DELETEFLAG",&deleteflag);
			if(!deleteflag)
			{
			FINDFLDGETCHAR(submstr,"ACTIVE",&active);
			if(active)
			{
			testfile=testfilerangersrc(submstr,rs);
			if(!testfile)
			{
			FINDFLDGETINT(subwrk,"JOB NUMBER",&subjobno);
			FINDFLDGETSHORT(subwrk,"GATHER TYPE",&gather_type);
			FINDFLDGETSTRING(subwrk,"POSITION IDENTIFICATION",&wposid);
			if(RateType)
			{
				FINDFLDGETSTRING(subwrk,"RATE IDENTIFICATION",&subrateid);
			} else {
				if(subrateid!=NULL) Rfree(subrateid);
				subrateid=NULL; /* insurance can be a very good thing.... */
			}
			FINDFLDSETSTRING(payjmst,"PERSONNEL IDENTIFICATION",subperid);
			FINDFLDSETINT(payjmst,"JOB NUMBER",subjobno);
			if(!ADVEQLNRDsec(payjmst,1,SubSelSubData,p))
			{
			FINDFLDGETCHAR(payjmst,"DELETEFLAG",&deleteflag);
			if(!deleteflag)
			{
			FINDFLDGETCHAR(payjmst,"ACTIVE",&active);
			if(active)
			{
			testfile=testfilerangersrc(payjmst,rs);
			if(!testfile)
			{
			ZERNRD(posmstr);
			FINDFLDSETSTRING(posmstr,"POSITION IDENTIFICATION",wposid);
			if(ADVEQLNRDsec(posmstr,1,SubSelSubData,p)) KEYNRD(posmstr,1);
			FINDFLDGETCHAR(posmstr,"DELETEFLAG",&deleteflag);
			FINDFLDGETCHAR(posmstr,"ACTIVE",&active);
			if(!deleteflag && active)
			{
			FINDFLDSETSTRING(prsnnl,"PERSONNEL IDENTIFICATION",subperid);
			if(!ADVEQLNRDsec(prsnnl,1,SubSelSubData,p))
			{
				FINDFLDGETCHAR(prsnnl,"DELETEFLAG",&deleteflag);
				if(!deleteflag)
				{
				FINDFLDGETSTRING(prsnnl,"STATUS TITLE",&substatus);
				FINDFLDSETSTRING(perstat,"STATUS TITLE",substatus);
				if(!ADVEQLNRDsec(perstat,1,SubSelSubData,p))
				{
				FINDFLDGETCHAR(perstat,"DELETEFLAG",&deleteflag);
				if(!deleteflag)
				{
				FINDFLDGETCHAR(perstat,"EMPLOYEE PAYABLE",&employee_payable);
				if(employee_payable==TRUE)
				{
				testfile=testfilerangersrc(prsnnl,rs);
				if(!testfile)
				{
					FINDFLDGETSTRING(payjmst,"POSITION IDENTIFICATION",&subposid);
					FINDFLDSETSTRING(posmstr,"POSITION IDENTIFICATION",subposid);
					if(!ADVEQLNRDsec(posmstr,1,SubSelSubData,p))
					{
						FINDFLDGETCHAR(posmstr,"DELETEFLAG",&deleteflag);
						FINDFLDGETCHAR(posmstr,"ACTIVE",&active);
						if(!deleteflag && active)
						{
						testfile=testfilerangersrc(posmstr,rs);
						if(!testfile)
						{
							FINDFLDGETSTRING(posmstr,"LOCATION IDENTIFICATION",&locid);
							FINDFLDSETSTRING(poslmst,"LOCATION IDENTIFICATION",locid);
							if(ADVEQLNRDsec(poslmst,1,SubSelSubData,p)) KEYNRD(poslmst,1);
							FINDFLDGETSTRING(payjmst,"GROSS IDENTIFICATION",&grossid);
							FINDFLDSETSTRING(pgross,"GROSS IDENTIFICATION",grossid);
							if(!ADVEQLNRDsec(pgross,1,SubSelSubData,p))
							{
								FINDFLDGETCHAR(pgross,"DELETEFLAG",&deleteflag);
								if(!deleteflag)
								{
								testfile=testfilerangersrc(pgross,rs);
								if(!testfile)
								{
									if(RateType==FALSE || isEMPTY(subrateid))
									{
									ZERNRD(pgrat);
									FINDFLDSETSTRING(pgrat,"GROSS IDENTIFICATION",grossid);
									ef1=ADVGTENRDsec(pgrat,1,SubSelSubData,p);
									while(!ef1)
									{
										FINDFLDGETSTRING(pgrat,"GROSS IDENTIFICATION",&grossid1);
										if(RDAstrcmp(grossid,grossid1)) break;
										FINDFLDGETCHAR(pgrat,"DELETEFLAG",&deleteflag);
										FINDFLDGETCHAR(pgrat,"ACTIVE",&aflag);
										if(!deleteflag && (use_aflag==FALSE || aflag==TRUE))
										{
										FINDFLDGETSTRING(pgrat,"RATE IDENTIFICATION",&rateid);
										FINDFLDSETSTRING(payjrcs,"PERSONNEL IDENTIFICATION",subperid);
										FINDFLDSETINT(payjrcs,"JOB NUMBER",subjobno);
										FINDFLDSETSTRING(payjrcs,"RATE IDENTIFICATION",rateid);
										FINDFLDSETINT(payjrcs,"CALENDAR YEAR",cyear);
										if(ADVEQLNRDsec(payjrcs,1,SubSelSubData,p)) KEYNRD(payjrcs,1);
										FINDFLDSETSTRING(payjrfs,"PERSONNEL IDENTIFICATION",subperid);
										FINDFLDSETINT(payjrfs,"JOB NUMBER",subjobno);
										FINDFLDSETSTRING(payjrfs,"RATE IDENTIFICATION",rateid);
										FINDFLDSETINT(payjrfs,"FISCAL YEAR",fyear);
										if(ADVEQLNRDsec(payjrfs,1,SubSelSubData,p)) KEYNRD(payjrfs,1);
										FINDFLDSETSTRING(posrate,"RATE IDENTIFICATION",rateid);
										if(ADVEQLNRDsec(posrate,1,SubSelSubData,p)) KEYNRD(posrate,1);
										FINDFLDGETCHAR(posrate,"DELETEFLAG",&deleteflag);
										if(!deleteflag)
										{
											if(!isEMPTY(select_expression))
											{
												etest=EVALUATEbol(select_expression,SubSelSubData,p);
											} else etest=TRUE;
											if(etest)
											{
												for(x=0,v=mainrsrc->virflds;
													x<mainrsrc->num;++x,++v)
												{
													if(ADVtestvirtualrangersrc(v,rs,(void (*)(...))SubSelSubData,p)) break;
												}
												if(x>=mainrsrc->num)
												{
													CLEAR_SCREEN_VIRTUAL(mainrsrc);
													ZERNRD(dbsort->fileno);
													dodiag=TRUE;
													FINDFLDGETSTRING(pgrat,"RATE AMOUNT",&exprssn);
													FINDFLDSETDOUBLE(dbsort->fileno,"UNITS",unpaid_units);
													RATE_AMT=0.0;
													UNITS=unpaid_units;
													if(exprssn!=NULL)
													{
														RATE_AMT=EVALUATEdbl(exprssn,
															SubSelSubData,p);
														Rfree(exprssn);
													}
													FINDFLDGETSTRING(posrate,
														"CHECK GROSS",&exprssn);
													CHECK_GROSS=0.0;
													x=FINDVIRTUAL(mainrsrc,"PAY AMOUNT");
													if(x!=(-1))
													{
														v=mainrsrc->virflds+x;
														v->computed=FALSE;
													}
													if(exprssn!=NULL)
													{
														CHECK_GROSS=EVALUATEdbl(exprssn,SubSelSubData,p);
														CHECK_GROSS=round(CHECK_GROSS);
														Rfree(exprssn);
													}
													addsortrecord(dbsort,mainrsrc,subperid,subjobno,subtransno,
														pay_id,subposid,rateid,
														grossid,unpaid_units,CHECK_GROSS,
														RATE_AMT,gather_type,wposid,perid,jobno,FALSE,p);
												} else {
													if(diagapps)
													{
														prterr("Skipped by Ranging on Virtual Field [%s].",v->name);
													}
												}
											} else {
												if(diagapps)
												{
													prterr("Skipped by Optional Select Expression [%.256s].",(select_expression!=NULL?select_expression:""));
												}
											}
										} else {
											if(diagapps)
											{
												prterr("Skipped by Deleted Position Rate Master [%s].",(rateid!=NULL?rateid:""));
											}
											e=Rmalloc(512);
											sprintf(e,"Skipped by Deleted Position Rate Master [%s].",(rateid!=NULL?rateid:""));
											addAPPlib(errorlist,e);
											if(e!=NULL) Rfree(e);
										}
										} else {
											if(diagapps)
											{
												prterr("Skipped by Deleted Position Gross Rate Master [%s] [%s].",(grossid!=NULL?grossid:""),(rateid!=NULL?rateid:""));
											}
											e=Rmalloc(512);
											sprintf(e,"Skipped by Deleted Position Gross Rate Master [%s] [%s].",(grossid!=NULL?grossid:""),(rateid!=NULL?rateid:""));
											addAPPlib(errorlist,e);
											if(e!=NULL) Rfree(e);
										}
										CLEAR_SCREEN_VIRTUAL(mainrsrc);
										ef1=ADVNXTNRDsec(pgrat,1,SubSelSubData,p);
									}
									} else {
/* HERE: For Use when defining the Rate of pay for the sub units at time of 
keying the hours.... */
										for(x=0,v=mainrsrc->virflds;
											x<mainrsrc->num;++x,++v)
										{
											if(!RDAstrcmp("NAME LAST LINEAGE FIRST MIDDLE",v->name))
											{
												if(ADVtestvirtualrangersrc(v,rs,(void (*)(...))SubSelSubData,p))
													break;
											} else if(!RDAstrcmp("WORK LOCATION",v->name))
											{
												if(ADVtestvirtualrangersrc(v,rs,(void (*)(...))SubSelSubData,p))
													break;
											}
										}
										/*
										Added by DDC on 07/28/2004
										Instead of concerning myself with evaluating all virtual fields (VF), which results in errors to stderr, I focused only on the VFs currently on the range screen.  If we add a bunch more range on VFs to the range screen, it might be better to go back to evaluating all of them.
										*/
										if(x>=mainrsrc->num)
										{
											if(!isEMPTY(select_expression))
											{
												etest=EVALUATEbol(select_expression,SubSelSubData,p);
											} else etest=TRUE;
											if(etest)
											{
												FINDFLDGETDOUBLE(subwrk,"RATE AMOUNT",&RATE_AMT);
												CHECK_GROSS=round(RATE_AMT*unpaid_units);
												FINDFLDGETSTRING(subwrk,"RATE IDENTIFICATION",&rateid);
												addsortrecord(dbsort,mainrsrc,subperid,subjobno,subtransno,pay_id,subposid,rateid,grossid,unpaid_units,CHECK_GROSS,RATE_AMT,gather_type,wposid,perid,jobno,TRUE,p);
											}
										}
									}
								} else {
									if(diagapps)
									{
										prterr("Skipped by Ranging on Position Grosses [%s].",(grossid!=NULL?grossid:""));KEYNRD(pgross,1);SEENRDRECORD(pgross);
									}
								}
								} else {
									if(diagapps)
									{
										prterr("Skipped by Deleted Position Gross. [%s]",(grossid!=NULL?grossid:""));KEYNRD(pgross,1);SEENRDRECORD(pgross);
									}
									e=Rmalloc(512);
									sprintf(e,"Skipped by Deleted Position Gross [%s].",(grossid!=NULL ? grossid:""));
									addAPPlib(errorlist,e);
									if(e!=NULL) Rfree(e);
								}
							} else {
								if(diagapps)
								{
									prterr("Skipped by not finding Position Grosses [%s].",(grossid!=NULL?grossid:""));KEYNRD(pgross,1);SEENRDRECORD(pgross);
								}
								e=Rmalloc(512);
								sprintf(e,"Skipped by not finding Position Gross [%s].",(grossid!=NULL ? grossid:""));
								addAPPlib(errorlist,e);
								if(e!=NULL) Rfree(e);
							}
						} else {
							if(diagapps)
							{
								prterr("Skipped by Ranging on Position Master [%s].",(subposid!=NULL?subposid:""));KEYNRD(posmstr,1);SEENRDRECORD(posmstr);
							}
						}
						} else {
							if(diagapps)
							{
								prterr("Skipped by Deleted or Inactive Position Master [%s].",(subposid!=NULL?subposid:""));KEYNRD(posmstr,1);SEENRDRECORD(posmstr);
							}
							e=Rmalloc(512);
							sprintf(e,"Skipped by Not Activie or Deleted Position Master [%s].",(subposid!=NULL ? subposid:""));
							addAPPlib(errorlist,e);
							if(e!=NULL) Rfree(e);
						}
					} else {
						if(diagapps)
						{
							prterr("Skipped by not finding Position Master [%s].",(subposid!=NULL?subposid:""));KEYNRD(posmstr,1);SEENRDRECORD(posmstr);
						}
						e=Rmalloc(512);
						sprintf(e,"Skipped by not finding Position Master [%s].",(subposid!=NULL ? subposid:""));
						addAPPlib(errorlist,e);
						if(e!=NULL) Rfree(e);
					}
				} else {
					if(diagapps)
					{
						prterr("Skipped by Ranging on Personnel Master [%s].",(subperid!=NULL?subperid:""));KEYNRD(prsnnl,1);SEENRDRECORD(prsnnl);
					}
				}
				} else { /* else, [PERSTAT][EMPLOYEE PAYABLE] is FALSE */
					if(diagapps)
					{
						prterr("Skipped because employee payable is false for substitute [%s].",(subperid!=NULL?subperid:""));KEYNRD(prsnnl,1);SEENRDRECORD(prsnnl);
					}
					e=Rmalloc(512);
					sprintf(e,"Skipped because employee payable is false for substitute [%s].",(subperid!=NULL ? subperid:""));
					addAPPlib(errorlist,e);
					if(e!=NULL) Rfree(e);
				}
				} else { /* else, the status is deleted in PERSTAT */
					if(diagapps)
					{
						prterr("Skipped because status title [%s] is deleted in PERSTAT for substitute [%s].",substatus,(subperid!=NULL?subperid:""));KEYNRD(prsnnl,1);SEENRDRECORD(prsnnl);
					}
					e=Rmalloc(512);
					sprintf(e,"Skipped because status title [%s] is deleted in PERSTAT for substitute [%s].",substatus,(subperid!=NULL ? subperid:""));
					addAPPlib(errorlist,e);
					if(e!=NULL) Rfree(e);
				}
				} else { /* else, the status doesn't exist in PERSTAT */
					if(diagapps)
					{
						prterr("Skipped because status title [%s] doesn't exist in PERSTAT for substitute [%s].",substatus,(subperid!=NULL?subperid:""));KEYNRD(prsnnl,1);SEENRDRECORD(prsnnl);
					}
					e=Rmalloc(512);
					sprintf(e,"Skipped because status title [%s] doesn't exist in PERSTAT for substitute [%s].",substatus,(subperid!=NULL ? subperid:""));
					addAPPlib(errorlist,e);
					if(e!=NULL) Rfree(e);
				}
				} else {
					if(diagapps)
					{
						prterr("Skipped by Deleted Personnel Master [%s].",(subperid!=NULL?subperid:""));KEYNRD(prsnnl,1);SEENRDRECORD(prsnnl);
					}
					e=Rmalloc(512);
					sprintf(e,"Skipped by Deleted Personnel Master [%s].",(subperid!=NULL ? subperid:""));
					addAPPlib(errorlist,e);
					if(e!=NULL) Rfree(e);
				}
			} else {
				if(diagapps)
				{
					prterr("Skipped by not finding Personnel Master [%s].",(subperid!=NULL?subperid:""));KEYNRD(prsnnl,1);SEENRDRECORD(prsnnl);
				}
				e=Rmalloc(512);
				sprintf(e,"Skipped by not finding Personnel Master [%s].",(subperid!=NULL ? subperid:""));
				addAPPlib(errorlist,e);
				if(e!=NULL) Rfree(e);
			}
			} else {
				if(diagapps)
				{
					prterr("Skipped by Inactive or Deleted Work Position [%s] for [%s] [%d].",(wposid!=NULL ? wposid:""),(subperid!=NULL?subperid:""),subjobno);KEYNRD(payjmst,1);SEENRDRECORD(payjmst);
				}
				e=Rmalloc(512);
				sprintf(e,"Skipped by Inactive or Deleted Work Position [%s] for [%s] [%d].",(wposid!=NULL ? wposid:""),(subperid!=NULL?subperid:""),subjobno);
				addAPPlib(errorlist,e);
				if(e!=NULL) Rfree(e);
			}
			} else {
				if(diagapps)
				{
					prterr("Skipped by Ranging on Personnel Job Master [%s] [%d].",(subperid!=NULL?subperid:""),subjobno);KEYNRD(payjmst,1);SEENRDRECORD(payjmst);
				}
			}
			} else {
				if(diagapps)
				{
					prterr("Skipped by Deleted Personnel Job master [%s] [%d].",(subperid!=NULL?subperid:""),subjobno);KEYNRD(payjmst,1);SEENRDRECORD(payjmst);
				}
				e=Rmalloc(512);
				sprintf(e,"Skipped by Deleted Substitute Job [%s] [%d].",(subperid!=NULL ? subperid:""),subjobno);
				addAPPlib(errorlist,e);
				if(e!=NULL) Rfree(e);
			}
			} else {
				if(diagapps)
				{
					prterr("Skipped by non-Active Personnel Job Master [%s] [%d].",(subperid!=NULL?subperid:""),subjobno);KEYNRD(payjmst,1);SEENRDRECORD(payjmst);
				}
				e=Rmalloc(512);
				sprintf(e,"Skipped by inactive Substitute Job [%s] [%d].",(subperid!=NULL ? subperid:""),subjobno);
				addAPPlib(errorlist,e);
				if(e!=NULL) Rfree(e);
			}
			} else {
				if(diagapps)
				{
					prterr("Skipped by not finding Personnel Job Master [%s] [%d].",(subperid!=NULL?subperid:""),subjobno);KEYNRD(payjmst,1);SEENRDRECORD(payjmst);
				}
				e=Rmalloc(512);
				sprintf(e,"Skipped by not finding Substitute Job [%s] [%d].",(subperid!=NULL ? subperid:""),subjobno);
				addAPPlib(errorlist,e);
				if(e!=NULL) Rfree(e);
			}
			} else {
				if(diagapps)
				{
					prterr("Skipped by Ranging on Substitute Master [%s].",(subperid!=NULL?subperid:""));KEYNRD(submstr,1);SEENRDRECORD(submstr);
				}
			}
			} else {
				if(diagapps)
				{
					prterr("Skipped by non-Active Substitute Master [%s].",(subperid!=NULL?subperid:""));KEYNRD(submstr,1);SEENRDRECORD(submstr);
				}
				e=Rmalloc(512);
				sprintf(e,"Skipped by Inactive Substitute Master [%s].",(subperid!=NULL ? subperid:""));
				addAPPlib(errorlist,e);
				if(e!=NULL) Rfree(e);
			}
			} else {
				if(diagapps)
				{
					prterr("Skipped by Deleted Substitute Master [%s].",(subperid!=NULL?subperid:""));KEYNRD(submstr,1);SEENRDRECORD(submstr);
				}
				e=Rmalloc(512);
				sprintf(e,"Skipped by Deleted Substitute Master [%s].",(subperid!=NULL ? subperid:""));
				addAPPlib(errorlist,e);
				if(e!=NULL) Rfree(e);
			}
			} else {
				if(diagapps)
				{
					prterr("Skipped by not finding Substitute Master [%s].",(subperid!=NULL?subperid:""));KEYNRD(submstr,1);SEENRDRECORD(submstr);
				}
				e=Rmalloc(512);
				sprintf(e,"Skipped by not finding Substitute Master [%s].",(subperid!=NULL ? subperid:""));
				addAPPlib(errorlist,e);
				if(e!=NULL) Rfree(e);
			}
			} else {
				if(diagapps)
				{
					prterr("Skipped because Substitute Work Master Units minus Paid Units is less than or equal to 0 [%s].",(subperid!=NULL?subperid:""));KEYNRD(subwrk,1);SEENRDRECORD(subwrk);
				}
			}
		} else {
			if(diagapps)
			{
				prterr("Skipped by Ranging on Substitute Work Master [%s] [%d].",(subperid!=NULL?subperid:""),subtransno);KEYNRD(subwrk,1);SEENRDRECORD(subwrk);
			}
		}
		} else {
			if(diagapps)
			{
				prterr("Skipped by Previously Finaled or not Source User Verified Substitute Work Master [%s] [%d].",(subperid!=NULL?subperid:""),subtransno);KEYNRD(subwrk,1);SEENRDRECORD(subwrk);
			}
		}
		} else {
			if(diagapps)
			{
				prterr("Skipped by Deleted Substitute Work Master [%s] [%d].",(subperid!=NULL?subperid:""),subtransno);KEYNRD(subwrk,1);SEENRDRECORD(subwrk);
			}
		}
		CLEAR_SCREEN_VIRTUAL(mainrsrc);
		if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(dodiag ? TRUE:FALSE));
		ef=ADVNXTNRDsec(subwrk,1,SubSelSubData,p);
	}
	if(p!=NULL) Rfree(p);
	if(subperid!=NULL) Rfree(subperid);
	if(subposid!=NULL) Rfree(subposid);
	if(locid!=NULL) Rfree(locid);
	if(grossid!=NULL) Rfree(grossid);
	if(grossid1!=NULL) Rfree(grossid1);
	if(rateid!=NULL) Rfree(rateid);
	if(perid!=NULL) Rfree(perid);
	if(wposid!=NULL) Rfree(wposid);
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	if(rs!=NULL) FreeRangeScreen(rs);
	size_of_file=SIZNRD(dbsort->fileno);
	if(size_of_file>0)
	{
		switch(select_type)
		{
			default:
			case 0: /* Automatic */
				select_backend(dbsort);
				break;
			case 1: /* Process Exceptions */
				brsrc=RDArsrcNEW(module,"SELECT SUBSTITUTES PAYROLL BROWSE");
				srsrc=RDArsrcNEW(module,"SELECT SUBSTITUTES PAYROLL SEARCH BROWSE");
				drsrc=RDArsrcNEW(module,"SELECT SUBSTITUTES PAYROLL DEFINE LIST");
				addDFincvir(brsrc,"PAYROLL","PAYMSTR",NULL,paymstr);
				addDFincvir(drsrc,"SUBMGT","SUBMASTR",NULL,submstr);
				addDFincvir(drsrc,"SUBMGT","SUBWKMAS",NULL,subwrk);
				addDFincvir(drsrc,module,"PAYJMST",NULL,payjmst);
				addDFincvir(drsrc,"PRSNNL","PERDMG",NULL,prsnnl);
				addDFincvir(drsrc,"PRSNNL","PERSTAT",NULL,perstat);
				addDFincvir(drsrc,"POSTRK","POSMSTR",NULL,posmstr);
				addDFincvir(drsrc,"POSTRK","POSGRS",NULL,pgross);
				addDFincvir(drsrc,"POSTRK","POSGRAT",NULL,pgrat);
				addDFincvir(drsrc,"POSTRK","POSRATE",NULL,posrate);
				addDFincvir(srsrc,"SUBMGT","SUBMASTR",NULL,submstr);
				addDFincvir(srsrc,"SUBMGT","SUBWKMAS",NULL,subwrk);
				addDFincvir(srsrc,module,"PAYJMST",NULL,payjmst);
				addDFincvir(srsrc,"PRSNNL","PERDMG",NULL,prsnnl);
				addDFincvir(srsrc,"POSTRK","POSMSTR",NULL,posmstr);
				addDFincvir(srsrc,"POSTRK","POSGRS",NULL,pgross);
				addDFincvir(srsrc,"POSTRK","POSGRAT",NULL,pgrat);
				addDFincvir(srsrc,"POSTRK","POSRATE",NULL,posrate);
				GET_SCREEN_VIRTUALS(drsrc,1);
				GET_SCREEN_VIRTUALS(srsrc,2);
				file2rsrc(paymstr,brsrc,FALSE);
				filerecord2rsrc(paymstr,brsrc);
/*
				addstdrsrc(brsrc,"[PAYMSTR][PAY IDENTIFICATION]",setpayid,NULL);
*/

				ZERNRD(dbsort->fileno);
				addrfexrsrc(brsrc,"SAVE",TRUE,saveexit,mbl_trans);
				brsrc->scn=RDAscrnNEW(brsrc->module,brsrc->screen);
				findscrnbin(brsrc->scn);
				MakeBrowseListTotals(brsrc,dbsort->fileno,drsrc);
				mbl_trans=ADVbrowse(dbsort->fileno,1,
					brsrc,srsrc,drsrc,NULL,
					selectfunction,doexit_test,okfunc,
					new_record,dbsort,TRUE,NULL,0,0,0,TRUE);
				FINDRSCSETFUNC(brsrc,"SAVE",saveexit,mbl_trans);
				break;
		}
	} else {
		ERRORDIALOG("Select Payroll Error","No records selected using the given select criteria.  Please try again.",NULL,FALSE);
		CLSDBsort(dbsort);
		FreeDBsort(dbsort);
	}
	if(errorlist->numlibs<1) addAPPlib(errorlist,"No Errors Detected");
        FINDRSCLISTAPPlib(mainrsrc,"ERROR LIST",0,errorlist);
        updatersrc(mainrsrc,"ERROR LIST");
	if(warnlist->numlibs<1) addAPPlib(warnlist,"No Warnings Detected");
        FINDRSCLISTAPPlib(mainrsrc,"WARNING LIST",0,warnlist);
        updatersrc(mainrsrc,"WARNING LIST");
}
static void savepaymstrsp(RDArsrc *mtnrsrc)
{
	double zd=0.0;

	FINDRSCSETCHAR(mtnrsrc,"[PAYMSTR][DELETEFLAG]",FALSE);
	updatersrc(mtnrsrc,"[PAYMSTR][DELETEFLAG]");
	FINDRSCSETDOUBLE(mtnrsrc,"[PAYMSTR][TOTAL GROSS]",zd);
	FINDRSCSETDOUBLE(mtnrsrc,"[PAYMSTR][TOTAL BENEFITS]",zd);
	FINDRSCSETDOUBLE(mtnrsrc,"[PAYMSTR][TOTAL PAYMTS]",zd);
	FINDRSCSETDOUBLE(mtnrsrc,"[PAYMSTR][TOTAL ADVANCE REPAYMENTS]",zd);
	savepaymstr(mtnrsrc);
	selectsub(mtnrsrc);
}
static void test_selectsub(RDArsrc *mtnrsrc)
{
	char *payid=NULL,delflag=FALSE,historical=FALSE;

	CLEAR_SCREEN_VIRTUAL(mainrsrc);
	ZERNRD(paymstr);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"[PAYMSTR][PAY IDENTIFICATION]",&payid);
	FINDFLDSETSTRING(paymstr,"PAY IDENTIFICATION",payid);
	if(ADVEQLNRDsec(paymstr,1,SCRNvirtualSubData,mtnrsrc))
	{
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","PAY MASTER DOES NOT EXIST!",
			"Select Substitute transactions can not be added until the Pay Master has been saved.  Pay Identification entered does not presently exist in the PAYMSTR file.",
			savepaymstrsp,NULL,FALSE,mtnrsrc);
	} else {
		FINDFLDGETCHAR(paymstr,"DELETEFLAG",&delflag);
		FINDFLDGETCHAR(paymstr,"HISTORICAL",&historical);
		ZERNRD(finyear);
		COPYFIELD(paymstr,finyear,"FISCAL YEAR");
		if(EQLNRD(finyear,1)) KEYNRD(finyear,1);
		if(delflag)
		{
			WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","PAY MASTER IS FLAGGED TO BE DELETED!",
			"The Pay Identification entered is in a state of deletion in the PAYMSTR file.  The Select Substitute transactions can not be added until the Pay Master ecord is in an undeleted state.  Select SAVE to change the state of the Pay Master record to undeleted.",
				savepaymstrsp,NULL,FALSE,mtnrsrc);
		} else if(historical==TRUE)
		{
			ERRORDIALOG("HISTORICAL PAY IDENTIFICATION","Error:  The selected pay identification has been marked historical.  Create a new pay identification or choose one which is NOT historical.",NULL,FALSE); 
		} else if(isEMPTY(payid))
		{
			ERRORDIALOG("BLANK PAY IDENTIFICATION","Error:  The selected pay identification is blank.  Enter an appropriate Pay Identification.",NULL,FALSE); 
		} else {
			selectsub(mtnrsrc);
		}
	}
	if(errorlist->numlibs<1) addAPPlib(errorlist,"No Errors Detected");
        FINDRSCLISTAPPlib(mainrsrc,"ERROR LIST",0,errorlist);
        updatersrc(mainrsrc,"ERROR LIST");
	if(warnlist->numlibs<1) addAPPlib(warnlist,"No Warnings Detected");
        FINDRSCLISTAPPlib(mainrsrc,"WARNING LIST",0,warnlist);
        updatersrc(mainrsrc,"WARNING LIST");
	if(payid!=NULL) Rfree(payid);
}
static void makeselecttype()
{
	int x=0;

	selecttype=Rmalloc(2*sizeof(char *));
	selecttype[0]=stralloc("Automatic");
	selecttype[1]=stralloc("Process Exceptions");
	monthlist=Rmalloc(12*sizeof(char *));
	for(x=0;x<12;++x)
	{
		monthlist[x]=stralloc(CALENDAR_MONTHS[x]);
	}
	fmonthlist=Rmalloc(14*sizeof(char *));
	for(x=0;x<14;++x)
	{
		fmonthlist[x]=stralloc(FISCALPERIOD[x]);
	}
}
static void freeselecttype()
{
	short x;

	if(selecttype!=NULL)
	{
		for(x=0;x<2;++x) Rfree(selecttype[x]);
		Rfree(selecttype);
	}
	if(monthlist!=NULL)
	{
		for(x=0;x<12;++x) Rfree(monthlist[x]);
		Rfree(monthlist);
	}
	if(fmonthlist!=NULL)
	{
		for(x=0;x<14;++x) Rfree(fmonthlist[x]);
		Rfree(fmonthlist);
		fmonthlist=NULL;
	}
}
void quitselect(RDArsrc *mainrsrc)
{
	APPlib *args=NULL;
	char *temp=NULL;

	if(printed_errorlist==FALSE && errorlist->numlibs>0) 
	{
		if((errorlist->numlibs!=1) || RDAstrcmp(errorlist->libs[0],"No Errors Detected"))
		{
			ERRORDIALOG("SELECT SUBSTITUTE ERRORS","There were errors detected in the select substitute process.\nYou must print the error list out before you can exit this process.",NULL,FALSE);
			return;
		}
	}
	if(Force_Warning==TRUE && printed_warnlist==FALSE && warnlist->numlibs>0) 
	{
		if((warnlist->numlibs!=1) || RDAstrcmp(warnlist->libs[0],"No Warnings Detected"))
		{
			ERRORDIALOG("SELECT SUBSTITUTE WARNINGS","There were warnings detected in the select substitute process.\nYou must print the warning list out before you can exit this process.",NULL,FALSE);
			return;
		}
	}
	if(mainrsrc!=NULL) 
	{
		killwindow(mainrsrc);
		free_rsrc(mainrsrc);
	}
	if(submstr!=(-1)) CLSNRD(submstr);
	if(subwrk!=(-1)) CLSNRD(subwrk);
	if(subpaid!=(-1)) CLSNRD(subpaid);
	if(subdtyp!=(-1)) CLSNRD(subdtyp);
	if(payjams!=(-1)) CLSNRD(payjams);
	if(payjmst!=(-1)) CLSNRD(payjmst);
	if(payjprm!=(-1)) CLSNRD(payjprm);
	if(payppms!=(-1)) CLSNRD(payppms);
	if(payjpms!=(-1)) CLSNRD(payjpms);
	if(paymstr!=(-1)) CLSNRD(paymstr);
	if(paypcsm!=(-1)) CLSNRD(paypcsm);
	if(paypfsm!=(-1)) CLSNRD(paypfsm);
	if(prsnnl!=(-1)) CLSNRD(prsnnl);
	if(perstat!=(-1)) CLSNRD(perstat);
	if(payjrcs!=(-1)) CLSNRD(payjrcs);
	if(payjrfs!=(-1)) CLSNRD(payjrfs);
	if(posmstr!=(-1)) CLSNRD(posmstr);
	if(posacc!=(-1)) CLSNRD(posacc);
	if(poslmst!=(-1)) CLSNRD(poslmst);
	if(pgross!=(-1)) CLSNRD(pgross);
	if(pgrat!=(-1)) CLSNRD(pgrat);
	if(posrate!=(-1)) CLSNRD(posrate);
	if(payjcsm!=(-1)) CLSNRD(payjcsm);
	if(payjfsm!=(-1)) CLSNRD(payjfsm);
	if(finyear!=(-1)) CLSNRD(finyear);
	if(prev_paymstr!=NULL) FreeRDATData(prev_paymstr);
	if(emp_payjmst!=(-1)) CLSNRD(emp_payjmst);
	if(bnkacc!=(-1)) CLSNRD(bnkacc);
	if(paytyp!=(-1)) CLSNRD(paytyp);
	if(pay_type!=NULL) Rfree(pay_type);
	if(bank_id!=NULL) Rfree(bank_id);
	if(defbankid!=NULL) Rfree(defbankid);
	if(selecttype!=NULL) freeselecttype();
	if(fmgt!=NULL) free_finmgt(fmgt);
	if((errorlist->numlibs!=1) || RDAstrcmp(errorlist->libs[0],"No Errors Detected"))
	{
		APPlib2SupportRequest("PAYROLL","subsel -> Select Substitute's Payroll",errorlist);
	}
	if(errorlist!=NULL) freeapplib(errorlist);
	if(warnlist!=NULL) freeapplib(warnlist);
	if(DO_AUDIT && PAYROLL_SETUP->subsel_audit)
	{
		args=APPlibNEW();
		addAPPlib(args,module);
		addAPPlib(args,audit_reportname);
		if(!isEMPTY(pay_id))
		{
			temp=Rmalloc(RDAstrlen(pay_id)+20);
			sprintf(temp,"PAY_IDENTIFICATION=%s",pay_id);
			RDA_PutEnv(temp);
		}
		ExecuteProgram("doreport",args);
		if(args!=NULL) freeapplib(args);
	}
	if(pay_id!=NULL) Rfree(pay_id);
	if(audit_reportname!=NULL) Rfree(audit_reportname);
	if(PAYROLL_SETUP!=NULL) free_payroll(PAYROLL_SETUP);
	ShutdownSubsystems();
	exit(0);
}
static void savepaymstr(RDArsrc *mainrsrc)
{
	int xmonth=0;
	char *rcddesc=NULL;
	short mth=0;
	double zd=0.0;

	if(ADVEQLNRDsec(paymstr,1,SCRNvirtualSubData,mainrsrc))
	{
		ZERNRD(paymstr);
	}
	FINDFLDGETDOUBLE(paymstr,"TOTAL GROSS",&zd);
	FINDRSCSETDOUBLE(mainrsrc,"[PAYMSTR][TOTAL GROSS]",zd);
	FINDFLDGETDOUBLE(paymstr,"TOTAL BENEFITS",&zd);
	FINDRSCSETDOUBLE(mainrsrc,"[PAYMSTR][TOTAL BENEFITS]",zd);
	FINDFLDGETDOUBLE(paymstr,"TOTAL PAYMTS",&zd);
	FINDRSCSETDOUBLE(mainrsrc,"[PAYMSTR][TOTAL PAYMTS]",zd);
	FINDFLDGETDOUBLE(paymstr,"TOTAL ADVANCE REPAYMENTS",&zd);
	FINDRSCSETDOUBLE(mainrsrc,"[PAYMSTR][TOTAL ADVANCE REPAYMENTS]",zd);
	readfilewidgets(paymstr,mainrsrc);
	rsrc2filerecord(paymstr,mainrsrc);
	if(paytyp!=(-1))
	{
		COPYFIELD(paytyp,paymstr,"PAY TYPE");
	}
	FINDRSCGETINT(mainrsrc,"MONTHS",&xmonth);
	mth=(short)xmonth;
	FINDFLDSETSHORT(paymstr,"MONTH",mth);
	FINDRSCGETINT(mainrsrc,"FISCAL MONTHS",&xmonth);
	mth=(short)xmonth;
	FINDFLDSETSHORT(paymstr,"FISCAL MONTH",mth);
	
	if(ADVRECORDsec(paymstr,SCRNvirtualSubData,mainrsrc))
	{
		rcddesc=ADVRECORDsecDesc(paymstr,SCRNvirtualSubData,mainrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE); 
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(paymstr,0,NULL,prev_paymstr);
	}
}
static void test_savepaymstr(RDArsrc *mainrsrc)
{
	char *payid=NULL,delflag=FALSE,historical=FALSE;
	char *warning=NULL;
	short x=0,mth=0;
	int xmonth=0;
	char *expression=NULL;
	char *expression_desc=NULL;
	char *desc=NULL;
	RDAvirtual *v;

	CLEAR_SCREEN_VIRTUAL(mainrsrc);
	ZERNRD(paymstr);
	readfilewidgets(paymstr,mainrsrc);
	rsrc2filerecord(paymstr,mainrsrc);
	FINDRSCGETINT(mainrsrc,"MONTHS",&xmonth);
	mth=(short)xmonth;
	FINDFLDSETSHORT(paymstr,"MONTH",mth);
	FINDRSCGETINT(mainrsrc,"FISCAL MONTHS",&xmonth);
	mth=(short)xmonth;
	FINDFLDSETSHORT(paymstr,"FISCAL MONTH",mth);
	FINDFLDGETSTRING(paymstr,"PAY IDENTIFICATION",&payid);
	if(isEMPTY(payid))
	{
		ERRORDIALOG("BLANK PAY IDENTIFICATION","Error:  The selected pay identification is blank.  Enter an appropriate Pay Identification.",NULL,FALSE); 
		return;
	}
	FINDRSCGETSTRING(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]",&payid);
	FINDFLDSETSTRING(paymstr,"PAY IDENTIFICATION",payid);
	FINDFLDGETCHAR(paymstr,"DELETEFLAG",&delflag);
	FINDFLDGETCHAR(paymstr,"HISTORICAL",&historical);
	ZERNRD(finyear);
	COPYFIELD(paymstr,finyear,"FISCAL YEAR");
	if(EQLNRD(finyear,1)) KEYNRD(finyear,1);
	readwidget(mainrsrc,"SAVE ERROR");
	FINDRSCGETSTRING(mainrsrc,"SAVE ERROR",&expression);
	if(!isEMPTY(expression))
	{
		if(EVALUATEbol(expression,SCRNvirtualSubData,mainrsrc))
		{
			readwidget(mainrsrc,"SAVE ERROR DESCRIPTION");
			FINDRSCGETSTRING(mainrsrc,"SAVE ERROR DESCRIPTION",&expression_desc);
			desc=EVALUATEstr(expression_desc,SCRNvirtualSubData,mainrsrc);
			ERRORDIALOG("SAVE ERROR",desc,NULL,FALSE);
			if(expression!=NULL) Rfree(expression);
			if(expression_desc!=NULL) Rfree(expression_desc);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	readwidget(mainrsrc,"SAVE WARNING");
	FINDRSCGETSTRING(mainrsrc,"SAVE WARNING",&expression);
	if(!isEMPTY(expression))
	{
		if(EVALUATEbol(expression,SCRNvirtualSubData,mainrsrc))
		{
			readwidget(mainrsrc,"SAVE WARNING DESCRIPTION");
			FINDRSCGETSTRING(mainrsrc,"SAVE WARNING DESCRIPTION",&expression_desc);
			desc=EVALUATEstr(expression_desc,SCRNvirtualSubData,mainrsrc);
			ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","SAVE WARNING",desc,savepaymstr,NULL,FALSE,1,mainrsrc,NULL,NULL);
			if(expression!=NULL) Rfree(expression);
			if(expression_desc!=NULL) Rfree(expression_desc);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	ZERNRD(finyear);
	x=FINDVIRTUAL(mainrsrc,"CREATE PAYMSTR SAVE WARNING");
	if(x!=(-1))
	{
		v=mainrsrc->virflds+x;
		v->computed=FALSE;
		computevirtual(v,SCRNvirtualSubData,mainrsrc);
		warning=v->value.string_value;
		if(!isEMPTY(warning))
		{

			WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","PAY MASTER WRNINGS!",warning,savepaymstr,NULL,FALSE,mainrsrc);
			return;
		}
	}
	if(delflag)
	{
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","PAY MASTER IS FLAGGED TO BE DELETED!",
		"The Pay Identification entered is in a state of deletion in the PAYMSTR file.  The Select Substitute transactions can not be added until the Pay Master ecord is in an undeleted state.  Select SAVE to change the state of the Pay Master record to undeleted.",
			savepaymstr,NULL,FALSE,mainrsrc);
	} else if(historical==TRUE)
	{
		ERRORDIALOG("HISTORICAL PAY IDENTIFICATION","Error:  The selected pay identification has been marked historical.  Create a new pay identification or choose one which is NOT historical.",NULL,FALSE); 
	} else if(isEMPTY(payid))
	{
		ERRORDIALOG("BLANK PAY IDENTIFICATION","Error:  The selected pay identification is blank.  Enter an appropriate Pay Identification.",NULL,FALSE); 
	} else if(!isEMPTY(warning))
	{
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","PAY MASTER WARNINGS!",warning,savepaymstr,NULL,FALSE,mainrsrc);
	} else {
		savepaymstr(mainrsrc);
	}
	if(errorlist->numlibs<1) addAPPlib(errorlist,"No Errors Detected");
        FINDRSCLISTAPPlib(mainrsrc,"ERROR LIST",0,errorlist);
        updatersrc(mainrsrc,"ERROR LIST");
	if(warnlist->numlibs<1) addAPPlib(warnlist,"No Warnings Detected");
        FINDRSCLISTAPPlib(mainrsrc,"WARNING LIST",0,warnlist);
        updatersrc(mainrsrc,"WARNING LIST");
	if(payid!=NULL) Rfree(payid);
}
short selectbpaymstr(MakeBrowseList *blist)
{
	char delflag=FALSE;

	FINDFLDGETCHAR(blist->fileno,"DELETEFLAG",&delflag);
	return((short)!delflag);
}
void quitbpaymstr(MakeBrowseList *blist)
{
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
	if(blist->mainrsrc!=NULL)
	{
		free_rsrc(blist->mainrsrc);
	}
}
void okbpaymstr(MakeBrowseList *b,void *targetkey)
{
	short month=0;
	int selmonth=0;

	ZERNRD(paymstr);
	if(ADVEQLNRDKEYsec(paymstr,1,targetkey,SCRNvirtualSubData,b->mainrsrc))
	{
		KEYNRD(paymstr,1);
		ClearRDATData(prev_paymstr);
	} else {
		GetRDATData(paymstr,prev_paymstr);
		updatefilerecord2rsrc(paymstr,b->funcargs);
		FINDFLDGETSHORT(paymstr,"MONTH",&month);
		selmonth=(int)month;
		FINDRSCSETINT(b->funcargs,"MONTHS",selmonth);
		updatersrc(b->funcargs,"MONTHS");
		FINDFLDGETSHORT(paymstr,"FISCAL MONTH",&month);
		selmonth=(int)month;
		FINDRSCSETINT(b->funcargs,"FISCAL MONTHS",selmonth);
		updatersrc(b->funcargs,"FISCAL MONTHS");
	}
	updateSCRNvirtuals(b->funcargs);
	setpayid(b->funcargs);
}
void browse_paymstr(RDArsrc *mainrsrc)
{
	RDArsrc *mrsrc=NULL,*srsrc=NULL,*drsrc=NULL;

	mrsrc=RDArsrcNEW(module,"SELECT SUBSTITUTES PAY MASTER BROWSE");
	srsrc=RDArsrcNEW(module,"SELECT SUBSTITUTES PAY MASTER SEARCH BROWSE");
	drsrc=RDArsrcNEW(module,"SELECT SUBSTITUTES PAY MASTER DEFINE LIST");
	addDFincvir(drsrc,module,"PAYMSTR",NULL,paymstr);
	addDFincvir(srsrc,module,"PAYMSTR",NULL,paymstr);
	GET_SCREEN_VIRTUALS(drsrc,1);
	GET_SCREEN_VIRTUALS(srsrc,2);
	mrsrc->scn=RDAscrnNEW(mrsrc->module,mrsrc->screen);
	findscrnbin(mrsrc->scn);
	MakeBrowseListTotals(mrsrc,paymstr,drsrc);
	ADVSbrowse(paymstr,1,mrsrc,srsrc,drsrc,NULL,NULL,selectbpaymstr,quitbpaymstr,
		okbpaymstr,NULL,mainrsrc,TRUE,NULL,2,2,2,FALSE);
}
static void bankidcb(RDArsrc *mainrsrc)
{
	char delflag=FALSE;

	readwidget(mainrsrc,"[BNKACC][BANK IDENTIFICATION]");
	FINDRSCGETSTRING(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",&bank_id);
	FINDFLDSETSTRING(bnkacc,"BANK IDENTIFICATION",bank_id);
	if(ADVEQLNRDsec(bnkacc,1,SCRNvirtualSubData,mainrsrc))
	{
		ReadRDAScrolledLists(mainrsrc);
		if(ADVEQLNRDsec(bnkacc,1,SCRNvirtualSubData,mainrsrc))
		{
			ZERNRD(bnkacc);
		}
	} else {
		FINDFLDGETCHAR(bnkacc,"DELETEFLAG",&delflag);
		if(delflag)
		{
			ReadRDAScrolledLists(mainrsrc);
			if(ADVEQLNRDsec(bnkacc,1,SCRNvirtualSubData,mainrsrc))
			{
				ZERNRD(bnkacc);
			}
		}
	}
	updatefilerecord2rsrc(bnkacc,mainrsrc);
	updateSCRNvirtuals(mainrsrc);
}
static void resetmtndefs(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	DefaultResetScreen(mtnrsrc,TRUE);
	updateallrsrc(mtnrsrc);
	getfile(mtnrsrc,dbsort);
}
static void resetmaindefs(RDArsrc *mainrsrc)
{
	DefaultResetScreen(mainrsrc,TRUE);
	updateallrsrc(mainrsrc);
	setpayid(mainrsrc);
	FINDRSCSETSTRING(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",defbankid);
	bankidcb(mainrsrc);
}
static void setpayid(RDArsrc *mainrsrc)
{
	char *payid=NULL;
	int xmonth=0;
	short xmtn=0;

	readwidget(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]");
	FINDRSCGETSTRING(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]",&payid);
	FINDFLDSETSTRING(paymstr,"PAY IDENTIFICATION",payid);
	if(payid!=NULL) Rfree(payid);
	if(ADVEQLNRDsec(paymstr,1,SCRNvirtualSubData,mainrsrc))
	{
		KEYNRD(paymstr,1);
		ClearRDATData(prev_paymstr);
	} else {
		GetRDATData(paymstr,prev_paymstr);
		updatefilerecord2rsrc(paymstr,mainrsrc);
		FINDFLDGETSHORT(paymstr,"MONTH",&xmtn);
		xmonth=(int)xmtn;
		FINDRSCSETINT(mainrsrc,"MONTHS",xmonth);
		updatersrc(mainrsrc,"MONTHS");
		FINDFLDGETSHORT(paymstr,"FISCAL MONTH",&xmtn);
		xmonth=(int)xmtn;
		FINDRSCSETINT(mainrsrc,"FISCAL MONTHS",xmonth);
		updatersrc(mainrsrc,"FISCAL MONTHS");
	}
	if(paytyp!=(-1))
	{
		FINDFLDGETSTRING(paymstr,"PAY TYPE",&pay_type);
		FINDRSCSETSTRING(mainrsrc,"[PAYTYP][PAY TYPE]",pay_type);
		FINDFLDSETSTRING(paytyp,"PAY TYPE",pay_type);
		paytypfunc(mainrsrc);
		updatefilerecord2rsrc(paytyp,mainrsrc);
	}
	updateSCRNvirtuals(mainrsrc);
}
static void getprevpaymstcb(RDArsrc *mainrsrc)
{
	char *payid=NULL;
	int xmonth=0;
	short xmtn=0;

	readwidget(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]");
	FINDRSCGETSTRING(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]",&payid);
	FINDFLDSETSTRING(paymstr,"PAY IDENTIFICATION",payid);
	if(payid!=NULL) Rfree(payid);
	if(ADVEQLNRDsec(paymstr,1,SCRNvirtualSubData,mainrsrc)) 
	{
		KEYNRD(paymstr,1);
		if(ADVLTENRDsec(paymstr,1,SCRNvirtualSubData,mainrsrc)) 
		{
			ClearRDATData(prev_paymstr);
		} else {
			GetRDATData(paymstr,prev_paymstr);
			updatefilerecord2rsrc(paymstr,mainrsrc);
			FINDFLDGETSHORT(paymstr,"MONTH",&xmtn);
			xmonth=(int)xmtn;
			FINDRSCSETINT(mainrsrc,"MONTHS",xmonth);
			updatersrc(mainrsrc,"MONTHS");
			FINDFLDGETSHORT(paymstr,"FISCAL MONTH",&xmtn);
			xmonth=(int)xmtn;
			FINDRSCSETINT(mainrsrc,"FISCAL MONTHS",xmonth);
			updatersrc(mainrsrc,"FISCAL MONTHS");
		}
	} else {
		ADVPRVNRDsec(paymstr,1,SCRNvirtualSubData,mainrsrc);
		GetRDATData(paymstr,prev_paymstr);
		updatefilerecord2rsrc(paymstr,mainrsrc);
		FINDFLDGETSHORT(paymstr,"MONTH",&xmtn);
		xmonth=(int)xmtn;
		FINDRSCSETINT(mainrsrc,"MONTHS",xmonth);
		updatersrc(mainrsrc,"MONTHS");
		FINDFLDGETSHORT(paymstr,"FISCAL MONTH",&xmtn);
		xmonth=(int)xmtn;
		FINDRSCSETINT(mainrsrc,"FISCAL MONTHS",xmonth);
		updatersrc(mainrsrc,"FISCAL MONTHS");
	}
	if(paytyp!=(-1))
	{
		FINDFLDGETSTRING(paymstr,"PAY TYPE",&pay_type);
		FINDRSCSETSTRING(mainrsrc,"[PAYTYP][PAY TYPE]",pay_type);
		FINDFLDSETSTRING(paytyp,"PAY TYPE",pay_type);
		paytypfunc(mainrsrc);
		updatefilerecord2rsrc(paytyp,mainrsrc);
	}
	updateSCRNvirtuals(mainrsrc);
}
static void getnextpaymstcb(RDArsrc *mainrsrc)
{
	char *payid=NULL;
	int xmonth=0;
	short xmtn=0;

	readwidget(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]");
	FINDRSCGETSTRING(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]",&payid);
	FINDFLDSETSTRING(paymstr,"PAY IDENTIFICATION",payid);
	if(payid!=NULL) Rfree(payid);
	if(ADVEQLNRDsec(paymstr,1,SCRNvirtualSubData,mainrsrc)) 
	{
		KEYNRD(paymstr,1);
		if(ADVGTENRDsec(paymstr,1,SCRNvirtualSubData,mainrsrc)) 
		{
			ClearRDATData(prev_paymstr);
		} else {
			GetRDATData(paymstr,prev_paymstr);
			updatefilerecord2rsrc(paymstr,mainrsrc);
			FINDFLDGETSHORT(paymstr,"MONTH",&xmtn);
			xmonth=xmtn;
			FINDRSCSETINT(mainrsrc,"MONTHS",xmonth);
			updatersrc(mainrsrc,"MONTHS");
			FINDFLDGETSHORT(paymstr,"FISCAL MONTH",&xmtn);
			xmonth=xmtn;
			FINDRSCSETINT(mainrsrc,"FISCAL MONTHS",xmonth);
			updatersrc(mainrsrc,"FISCAL MONTHS");
		}
	} else {
		ADVNXTNRDsec(paymstr,1,SCRNvirtualSubData,mainrsrc);
		GetRDATData(paymstr,prev_paymstr);
		updatefilerecord2rsrc(paymstr,mainrsrc);
		FINDFLDGETSHORT(paymstr,"MONTH",&xmtn);
		xmonth=xmtn;
		FINDRSCSETINT(mainrsrc,"MONTHS",xmonth);
		updatersrc(mainrsrc,"MONTHS");
		FINDFLDGETSHORT(paymstr,"FISCAL MONTH",&xmtn);
		xmonth=xmtn;
		FINDRSCSETINT(mainrsrc,"FISCAL MONTHS",xmonth);
		updatersrc(mainrsrc,"FISCAL MONTHS");
	}
	if(paytyp!=(-1))
	{
		FINDFLDGETSTRING(paymstr,"PAY TYPE",&pay_type);
		FINDRSCSETSTRING(mainrsrc,"[PAYTYP][PAY TYPE]",pay_type);
		FINDFLDSETSTRING(paytyp,"PAY TYPE",pay_type);
		paytypfunc(mainrsrc);
		updatefilerecord2rsrc(paytyp,mainrsrc);
	}
	updateSCRNvirtuals(mainrsrc);
}
static void changemonth(RDArsrc *mainrsrc)
{
	int selmonth=0,year=0;

	FINDRSCGETINT(mainrsrc,"MONTHS",&selmonth);
	readwidget(mainrsrc,"[PAYMSTR][CALENDAR YEAR]");
	FINDRSCGETINT(mainrsrc,"[PAYMSTR][CALENDAR YEAR]",&year);
	if(selmonth>=fmgt->month) year+=1;
	FINDRSCSETINT(mainrsrc,"[PAYMSTR][FISCAL YEAR]",year);
	updatersrc(mainrsrc,"[PAYMSTR][FISCAL YEAR]");
	FINDRSCSETINT(mainrsrc,"FISCAL MONTHS",selmonth+1);
	updatersrc(mainrsrc,"FISCAL MONTHS");
	updateSCRNvirtuals(mainrsrc);
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
		RDA_fprintf(fp,"%*sXPERT SELECT SUBSTITUTE ERROR LIST          Date: %*s\r\n",24,"",8,date);
		RDA_fprintf(fp,"%*sExecuted By: %s%*sTime: %s\r\n\r\n",
			((80-(15+RDAstrlen(USERLOGIN)))/2),
			"",USERLOGIN,
			(((80-(15+RDAstrlen(USERLOGIN)))/2)-14),"",
			timex);
			if(date!=NULL) Rfree(date);
			if(timex!=NULL) Rfree(timex);
		for(x=0;x<errorlist->numlibs;++x)
		{
			RDA_fprintf(fp,"(%5d) %s\r\n",(x+1),errorlist->libs[x]);
		}
		RDA_fprintf(fp,"\f");
	}
	printed_errorlist=TRUE;
}
void printerrorlistcb(RDArsrc *prsrc,RDArsrc *parent)
{
	char *outdevice=NULL,*message=NULL,print_style=0;
	RDA_PFILE *fp=NULL;

	readwidget(prsrc,"DEVICE");
	FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
	readwidget(prsrc,"NORMAL PRINT");
	FINDRSCGETCHAR(prsrc,"NORMAL PRINT",&print_style);
	if(outdevice!=NULL)
	{
		fp=RDA_popen(outdevice,"w");
		if(fp!=NULL)
		{
			SelectPrintTypeByName(outdevice,"PCL JOB HEADER",fp);
			SelectPrintType(outdevice,print_style,fp);
			printerrorlist(parent,fp);
			SelectPrintTypeByName(outdevice,"PCL JOB FOOTER",fp);
			RDA_pclose(fp);
			printed_errorlist=TRUE;
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
static void printwarnlist(RDArsrc *parent,RDA_PFILE *fp)
{
	int x;
	char *date=NULL,*timex=NULL;

	if(fp!=NULL)
	{
		date=GETCURRENTDATE();
		timex=GETCURRENTTIME();
		RDA_fprintf(fp,"%*sXPERT SELECT SUBSTITUTE WARNING LIST        Date: %*s\r\n",22,"",8,date);
		RDA_fprintf(fp,"%*sExecuted By: %s%*sTime: %s\r\n\r\n",
			((80-(15+RDAstrlen(USERLOGIN)))/2),
			"",USERLOGIN,
			(((80-(15+RDAstrlen(USERLOGIN)))/2)-14),"",
			timex);
			if(date!=NULL) Rfree(date);
			if(timex!=NULL) Rfree(timex);
		for(x=0;x<warnlist->numlibs;++x)
		{
			RDA_fprintf(fp,"(%5d) %s\r\n",(x+1),warnlist->libs[x]);
		}
		RDA_fprintf(fp,"\f");
	}
	printed_warnlist=TRUE;
}
void printwarnlistcb(RDArsrc *prsrc,RDArsrc *parent)
{
	char *outdevice=NULL,*message=NULL,print_style=0;
	RDA_PFILE *fp=NULL;

	readwidget(prsrc,"DEVICE");
	FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
	readwidget(prsrc,"NORMAL PRINT");
	FINDRSCGETCHAR(prsrc,"NORMAL PRINT",&print_style);
	if(outdevice!=NULL)
	{
		fp=RDA_popen(outdevice,"w");
		if(fp!=NULL)
		{
			SelectPrintTypeByName(outdevice,"PCL JOB HEADER",fp);
			SelectPrintType(outdevice,print_style,fp);
			printwarnlist(parent,fp);
			SelectPrintTypeByName(outdevice,"PCL JOB FOOTER",fp);
			RDA_pclose(fp);
			printed_warnlist=TRUE;
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
static void print_list(RDArsrc *parent,void (*printfunc)(...))
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL,normal_print=TRUE;

	if(diaggui)
	{
		prterr("DIAG print error/warnlist Displaying the Print Screen in preparation to print Error or Warning List.");
	}
	prsrc=RDArsrcNEW("GUI","PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addbtnrsrc(prsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	addstdrsrc(prsrc,"NORMAL PRINT",BOOLNS,1,&normal_print,TRUE);
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
static void PAYMSTR_ComputeVirtuals(RDArsrc *r)
{
	readallwidgets(r);
	ReadRDAScrolledLists(r);
	rsrc2filerecord(paymstr,r);
	ADVcomputeandupdateallSCRNvirtuals(r,SCRNvirtualSubData,r,TRUE);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x,xmonth=0,nofields=0,cyear=0,fyear=0;
	int start=0;
	char *name=NULL;
	NRDfield *fldx,*fields;
	short edit_rsrc=TRUE;
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,module,"SELECT SUBSTITUTES PAYROLL")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if(argc>1)
	{
		audit_reportname=stralloc(argv[1]);
	} else audit_reportname=stralloc("SUBSELA");
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("SUBMGT")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"SUBMGT");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"SUBMGT");
#endif

	gsv=RDAGenericSetupNew("SUBMGT","ALLOW MANUAL RATE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			RateType=*gsv->value.string_value;
		} else {
			RateType=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("PAYROLL")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"PAYROLL");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"PAYROLL");
#endif
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
	gsv=RDAGenericSetupNew("PAYROLL","FORCE WARNINGS");
	if(getRDAGenericSetupbin(libx,gsv))
	{
		Force_Warning=TRUE;
	} else {
		Force_Warning=*gsv->value.string_value;
	}
	FreeRDAGenericSetup(gsv);	
	if(libx!=NULL) Rfree(libx);
	fmgt=RDAfinmgtNEW();
	getfinmgtbin(fmgt);
	PAYROLL_SETUP=RDApayrollNEW();
	getpayrollbin(PAYROLL_SETUP);
	if((submstr=APPOPNNRD("SUBMGT","SUBMASTR",TRUE,FALSE))==(-1)) return;
	if((subwrk=APPOPNNRD("SUBMGT","SUBWKMAS",TRUE,FALSE))==(-1)) return;
	if((subpaid=APPOPNNRD("SUBMGT","SUBPAID",TRUE,FALSE))==(-1)) return;
	if((subdtyp=APPOPNNRD("SUBMGT","SUBDTYP",TRUE,FALSE))==(-1)) return;
	if((payjams=APPOPNNRD(module,"PAYJAMS",TRUE,FALSE))==(-1)) return;
	if((posacc=APPOPNNRD("POSTRK","POSACC",TRUE,FALSE))==(-1)) return;
	if((payjmst=APPOPNNRD(module,"PAYJMST",TRUE,FALSE))==(-1)) return;
	if((payjprm=APPOPNNRD(module,"PAYJPRM",TRUE,TRUE))==(-1)) return;
	if((payppms=APPOPNNRD(module,"PAYPPMS",TRUE,TRUE))==(-1)) return;
	if((payjpms=APPOPNNRD(module,"PAYJPMS",TRUE,TRUE))==(-1)) return;
	if((paymstr=APPOPNNRD(module,"PAYMSTR",TRUE,TRUE))==(-1)) return;
	if((paypcsm=APPOPNNRD(module,"PAYPCSM",TRUE,FALSE))==(-1)) return;
	if((paypfsm=APPOPNNRD(module,"PAYPFSM",TRUE,FALSE))==(-1)) return;
	if((payjrcs=APPOPNNRD(module,"PAYJRCS",TRUE,FALSE))==(-1)) return;
	if((payjrfs=APPOPNNRD(module,"PAYJRFS",TRUE,FALSE))==(-1)) return;
	if((prsnnl=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((perstat=APPOPNNRD("PRSNNL","PERSTAT",TRUE,FALSE))==(-1)) return;
	if((posmstr=APPOPNNRD("POSTRK","POSMSTR",TRUE,FALSE))==(-1)) return;
	if((poslmst=APPOPNNRD("POSTRK","POSLMST",TRUE,FALSE))==(-1)) return;
	if((pgross=APPOPNNRD("POSTRK","POSGRS",TRUE,FALSE))==(-1)) return;
	if((pgrat=APPOPNNRD("POSTRK","POSGRAT",TRUE,FALSE))==(-1)) return;
	if((posrate=APPOPNNRD("POSTRK","POSRATE",TRUE,FALSE))==(-1)) return;
	if((payjcsm=APPOPNNRD(module,"PAYJCSM",TRUE,FALSE))==(-1)) return;
	if((payjfsm=APPOPNNRD(module,"PAYJFSM",TRUE,FALSE))==(-1)) return;
	if((emp_payjmst=APPOPNNRD(module,"PAYJMST",TRUE,FALSE))==(-1)) return;
	if((bnkacc=APPOPNNRD("BNKREC","BNKACC",TRUE,FALSE))==(-1)) return;
	if((finyear=APPOPNNRD("FINMGT","FINYEAR",TRUE,FALSE))==(-1)) return;
	makeselecttype();
	if((paytyp=OPNNRDsec(module,"PAYTYP",TRUE,FALSE,FALSE))==(-1))
	{
		if((paytyp=OPNNRD(module,"PAYTYP"))>(-1))
		{
			ERRORDIALOG("Security Access Denied","Security Access denied to the [PAYTYP] file or the file cannot otherwise be opened.\n",NULL,TRUE);
			ShutdownSecurity();
			RDAAPPMAINLOOP();
			return;
		} else {
/*
			CLSNRD(paytyp);
*/
			paytyp=(-1);
		}
	}
	mainrsrc=RDArsrcNEW(module,"SELECT SUBSTITUTES PAYROLL");
	addDFincvir(mainrsrc,"SUBMGT","SUBMASTR",NULL,submstr);
	addDFincvir(mainrsrc,"SUBMGT","SUBWKMAS",NULL,subwrk);
	addDFincvir(mainrsrc,module,"PAYJMST",NULL,payjmst);
	addDFincvir(mainrsrc,"PRSNNL","PERDMG",NULL,prsnnl);
	addDFincvir(mainrsrc,"PRSNNL","PERSTAT",NULL,perstat);
	addDFincvir(mainrsrc,"POSTRK","POSMSTR",NULL,posmstr);
	addDFincvir(mainrsrc,"POSTRK","POSLMST",NULL,poslmst);
	addDFincvir(mainrsrc,"POSTRK","POSGRS",NULL,pgross);
	addDFincvir(mainrsrc,"POSTRK","POSGRAT",NULL,pgrat);
	addDFincvir(mainrsrc,"POSTRK","POSRATE",NULL,posrate);
	addDFincvir(mainrsrc,module,"PAYJCSM",NULL,payjcsm);
	addDFincvir(mainrsrc,module,"PAYJFSM",NULL,payjfsm);
	addDFincvir(mainrsrc,module,"PAYMSTR",NULL,paymstr);
	addDFincvir(mainrsrc,module,"PAYJRCS",NULL,payjrcs);
	addDFincvir(mainrsrc,module,"PAYJRFS",NULL,payjrfs);
	addDFincvir(mainrsrc,"BNKREC","BNKACC",NULL,bnkacc);
	addDFincvir(mainrsrc,"FINMGT","FINYEAR",NULL,finyear);
	if(paytyp!=(-1))
	{
		addDFincvir(mainrsrc,module,"PAYTYP",NULL,paytyp);
	}
	GET_ALL_SCREEN_VIRTUALS(mainrsrc,2);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	file2rangersrc(submstr,mainrsrc);
	file2rangersrc(subwrk,mainrsrc);
	file2rangersrc(payjmst,mainrsrc);
	file2rangersrc(prsnnl,mainrsrc);
	file2rangersrc(perstat,mainrsrc);
	file2rangersrc(posmstr,mainrsrc);
	file2rangersrc(pgross,mainrsrc);
	file2rsrc(bnkacc,mainrsrc,FALSE);
	if(paytyp!=(-1))
	{
		file2rsrc(paytyp,mainrsrc,FALSE);
	}
	FINDRSCSETEDITABLE(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",TRUE);
	if(paytyp!=(-1))
	{
		FINDRSCSETEDITABLE(mainrsrc,"[PAYTYP][PAY TYPE]",TRUE);
	}
	addlstrsrc(mainrsrc,"SELECT TYPE",&select_type,TRUE,NULL,2,&selecttype,NULL);
	nofields=NUMFLDS(paymstr);
	fields=FLDPOINTER(paymstr);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"MONTH"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(paymstr,fldx->name,&edit_rsrc))
				{
					xmonth=val(&CURRENT_DATE[0],2)-1;
					if(xmonth>11) xmonth=0;
					addlstrsrc(mainrsrc,"MONTHS",&xmonth,
						TRUE,changemonth,12,
						&monthlist,NULL);
				}
			} else if(!RDAstrcmp(fldx->name,"FISCAL MONTH"))
			{
				edit_rsrc=TRUE;
				if(FIELDscrn(paymstr,fldx->name,&edit_rsrc))
				{
					xmonth=val(&CURRENT_DATE[0],2);
					if(xmonth>13) xmonth=0;
					addlstrsrc(mainrsrc,"FISCAL MONTHS",
					&xmonth,TRUE,NULL,14,
					&fmonthlist,NULL);
				}
			} else {
				nonlstmakefld(mainrsrc,paymstr,fldx,TRUE);
				memset(stemp,0,101);
				sprintf(stemp,"[PAYMSTR][%s]",fldx->name);
				FINDRSCSETFUNC(mainrsrc,stemp,PAYMSTR_ComputeVirtuals,NULL);
			}
		}
	}
	cyear=GETCURRENTCALENDARYEAR();
	if(xmonth>=fmgt->month) fyear=cyear+1;
	else fyear=cyear;
	FINDRSCSETINT(mainrsrc,"[PAYMSTR][CALENDAR YEAR]",cyear);
	FINDRSCSETINT(mainrsrc,"[PAYMSTR][FISCAL YEAR]",fyear);
	FINDRSCSETSTRING(mainrsrc,"[PAYMSTR][PAY DATE]",CURRENT_DATE10);
	FINDRSCSETFUNC(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]",setpayid,NULL);
	FINDRSCSETFUNC(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",bankidcb,NULL);
	if(paytyp!=(-1))
	{
		FINDRSCSETFUNC(mainrsrc,"[PAYTYP][PAY TYPE]",paytypcb,NULL);
	}
	if(name!=NULL) Rfree(name);
	prev_paymstr=RDATDataNEW(paymstr);
	addsctrsrc(mainrsrc,"EXPRESSION",0,NULL,TRUE);
	addsctrsrc(mainrsrc,"SAVE WARNING",0,NULL,TRUE);
	addsctrsrc(mainrsrc,"SAVE WARNING DESCRIPTION",0,NULL,TRUE);
	addsctrsrc(mainrsrc,"SAVE ERROR",0,NULL,TRUE);
	addsctrsrc(mainrsrc,"SAVE ERROR DESCRIPTION",0,NULL,TRUE);
	SCRNvirtual2rangersrc(mainrsrc);
	SCRNvirtual2rsrc(mainrsrc);
	DefaultScreens(mainrsrc);
	SetRangersrcsensitive(mainrsrc);
	ReadRDAScrolledLists(mainrsrc);
	if(!isEMPTY(defbankid))
	{
		FINDRSCSETSTRING(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",defbankid);
		FINDFLDSETSTRING(bnkacc,"BANK IDENTIFICATION",defbankid);
	} else {
		FINDRSCGETSTRING(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",&bank_id);
		FINDFLDSETSTRING(bnkacc,"BANK IDENTIFICATION",bank_id);
	}
	if(ADVEQLNRDsec(bnkacc,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(bnkacc,1);
	if(!isEMPTY(defbankid)) bankidcb(mainrsrc);
	filerecord2rsrc(bnkacc,mainrsrc);
	if(paytyp!=(-1))
	{
		FINDRSCGETSTRING(mainrsrc,"[PAYTYP][PAY TYPE]",&pay_type);
		FINDFLDSETSTRING(paytyp,"PAY TYPE",pay_type);
		if(ADVEQLNRDsec(paytyp,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(paytyp,1);
	}
	errorlist=APPlibNEW();
	addAPPlib(errorlist,"No Errors Detected");
	addlstrsrc(mainrsrc,"ERROR LIST",&start,TRUE,NULL,errorlist->numlibs,&errorlist->libs,NULL);
/*lint -e611 */
	addrfcbrsrc(mainrsrc,"PRINT ERROR LIST",TRUE,print_list,(void *)printerrorlistcb);
	warnlist=APPlibNEW();
	addAPPlib(warnlist,"No Warnings Detected");
	addlstrsrc(mainrsrc,"WARNING LIST",&start,TRUE,NULL,warnlist->numlibs,&warnlist->libs,NULL);
	addrfcbrsrc(mainrsrc,"PRINT WARNING LIST",TRUE,print_list,(void *)printwarnlistcb);
	addbtnrsrc(mainrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(mainrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addbtnrsrc(mainrsrc,"PREVIOUS",TRUE,getprevpaymstcb,NULL);
	addbtnrsrc(mainrsrc,"NEXT",TRUE,getnextpaymstcb,NULL);
	addbtnrsrc(mainrsrc,"RESET DEFAULTS",TRUE,resetmaindefs,NULL);
	addbtnrsrc(mainrsrc,"BROWSE",TRUE,browse_paymstr,NULL);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mainrsrc,"SAVE",TRUE,test_savepaymstr,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,test_selectsub,NULL);
	addbtnrsrc(mainrsrc,"QUIT",TRUE,quitselect,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,quitselect,NULL,TRUE);
	RDAAPPMAINLOOP();
}
static void setfiles(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	PassableStruct *p=NULL;

	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2singlefilerecord(dbsort->fileno,mtnrsrc);
	getsupportingfiles(mtnrsrc,dbsort,TRUE);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,SubSelSubData,p,TRUE);
	if(p!=NULL) Rfree(p);
}
static void getposition(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *pos=NULL;
	PassableStruct *p=NULL;

	readwidget(mtnrsrc,"POSITION IDENTIFICATION");
	FINDRSCGETSTRING(mtnrsrc,"POSITION IDENTIFICATION",&pos);
	FINDFLDSETSTRING(posmstr,"POSITION IDENTIFICATION",pos);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	if(ADVEQLNRDsec(posmstr,1,SubSelSubData,p)) KEYNRD(posmstr,1);
	if(pos!=NULL) Rfree(pos);
	updatefilerecord2rsrc(posmstr,mtnrsrc);
	ADVupdateSCRNvirtuals(mtnrsrc,SubSelSubData,p);
	if(p!=NULL) Rfree(p);
}
static void getgross(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *pos=NULL,*rat=NULL,*perid=NULL;
	int jobno=0,fyear=0,cyear=0;
	PassableStruct *p=NULL;

	readwidget(mtnrsrc,"GROSS IDENTIFICATION");
	readwidget(mtnrsrc,"RATE IDENTIFICATION");
	readwidget(mtnrsrc,"PERSONNEL IDENTIFICATION");
	readwidget(mtnrsrc,"JOB NUMBER");
	readwidget(mtnrsrc,"RATE IDENTIFICATION");
	readwidget(mtnrsrc,"[PAYMSTR][FISCAL YEAR]");
	readwidget(mtnrsrc,"[PAYMSTR][CALENDAR YEAR]");

	FINDRSCGETSTRING(mtnrsrc,"GROSS IDENTIFICATION",&pos);
	FINDFLDSETSTRING(pgross,"GROSS IDENTIFICATION",pos);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	if(ADVEQLNRDsec(pgross,1,SubSelSubData,p)) KEYNRD(pgross,1);
	updatefilerecord2rsrc(pgross,mtnrsrc);

	FINDRSCGETSTRING(mtnrsrc,"RATE IDENTIFICATION",&rat);
	FINDFLDSETSTRING(posrate,"RATE IDENTIFICATION",rat);
	if(ADVEQLNRDsec(posrate,1,SubSelSubData,p)) KEYNRD(posrate,1);
	updatefilerecord2rsrc(posrate,mtnrsrc);

	FINDFLDSETSTRING(pgrat,"GROSS IDENTIFICATION",pos);
	FINDFLDSETSTRING(pgrat,"RATE IDENTIFICATION",rat);
	if(ADVEQLNRDsec(pgrat,1,SubSelSubData,p)) KEYNRD(pgrat,1);
	updatefilerecord2rsrc(pgrat,mtnrsrc);

	FINDRSCGETSTRING(mtnrsrc,"PERSONNEL IDENTIFICATION",&perid);
	FINDRSCGETINT(mtnrsrc,"JOB NUMBER",&jobno);
	FINDFLDSETSTRING(payjrcs,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(payjrcs,"JOB NUMBER",jobno);
	FINDFLDSETSTRING(payjrcs,"RATE IDENTIFICATION",rat);
	FINDRSCGETINT(mtnrsrc,"[PAYMSTR][CALENDAR YEAR]",&cyear);
	FINDFLDSETINT(payjrcs,"CALENDAR YEAR",cyear);
	if(ADVEQLNRDsec(payjrcs,1,SubSelSubData,p)) KEYNRD(payjrcs,1);
	updatefilerecord2rsrc(payjrcs,mtnrsrc);

	FINDFLDSETSTRING(payjrfs,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(payjrfs,"JOB NUMBER",jobno);
	FINDFLDSETSTRING(payjrfs,"RATE IDENTIFICATION",rat);
	FINDRSCGETINT(mtnrsrc,"[PAYMSTR][FISCAL YEAR]",&fyear);
	FINDFLDSETINT(payjrfs,"FISCAL YEAR",fyear);
	if(ADVEQLNRDsec(payjrfs,1,SubSelSubData,p)) KEYNRD(payjrfs,1);
	updatefilerecord2rsrc(payjrfs,mtnrsrc);

	if(perid!=NULL) Rfree(perid);
	if(pos!=NULL) Rfree(pos);
	if(rat!=NULL) Rfree(rat);
	ADVupdateSCRNvirtuals(mtnrsrc,SubSelSubData,p);
	if(p!=NULL) Rfree(p);
}
static void getnewunits(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *exprssn=NULL,ratetype=FALSE;
	PassableStruct *p=NULL;
	int x;
	RDAvirtual *v;

	readwidget(mtnrsrc,"UNITS");
	CLEAR_SCREEN_VIRTUAL(mtnrsrc);
	FINDRSCGETDOUBLE(mtnrsrc,"UNITS",&UNITS);
	FINDFLDSETDOUBLE(dbsort->fileno,"UNITS",UNITS);
	FINDFLDGETCHAR(dbsort->fileno,"RATE TYPE",&ratetype);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	if(ratetype==FALSE)
	{
		RATE_AMT=0.0;
		FINDFLDGETSTRING(pgrat,"RATE AMOUNT",&exprssn);
		if(exprssn!=NULL)
		{
			RATE_AMT=EVALUATEdbl(exprssn,SubSelSubData,p);
			Rfree(exprssn);
		}
		FINDFLDSETDOUBLE(dbsort->fileno,"RATE AMOUNT",RATE_AMT);
	} else {
		FINDFLDGETDOUBLE(dbsort->fileno,"RATE AMOUNT",&RATE_AMT);
	}
	FINDRSCSETDOUBLE(mtnrsrc,"RATE AMOUNT",RATE_AMT);
	if(exprssn!=NULL) Rfree(exprssn);
	FINDRSCGETSTRING(mtnrsrc,"[POSRATE][CHECK GROSS]",&exprssn);
	x=FINDVIRTUAL(mtnrsrc,"PAY AMOUNT");
	if(x!=(-1))
	{
		v=mtnrsrc->virflds+x;
		v->computed=FALSE;
	}
	if(!isEMPTY(exprssn))
	{
		CHECK_GROSS=EVALUATEdbl(exprssn,SubSelSubData,p);
		CHECK_GROSS=round(CHECK_GROSS);
		Rfree(exprssn);
	}
	FINDFLDSETDOUBLE(dbsort->fileno,"CHECK GROSS",CHECK_GROSS);
	FINDRSCSETDOUBLE(mtnrsrc,"CHECK GROSS",CHECK_GROSS);
	updatersrc(mtnrsrc,"UNITS");
	updatersrc(mtnrsrc,"CHECK GROSS");
	updatersrc(mtnrsrc,"RATE AMOUNT");
	x=FINDVIRTUAL(mtnrsrc,"PAY AMOUNT");
	if(x!=(-1))
	{
		v=mtnrsrc->virflds+x;
		*v->value.float_value=round(UNITS*RATE_AMT);
		FINDRSCSETDOUBLE(mtnrsrc,"PAY AMOUNT",*v->value.float_value);
		updatersrc(mtnrsrc,"PAY AMOUNT");
	}
	ADVupdateSCRNvirtuals(mtnrsrc,SubSelSubData,p);
	if(p!=NULL) Rfree(p);
}
static void getnewamt(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *exprssn=NULL,ratetype=FALSE;
	PassableStruct *p=NULL;
	int x;
	RDAvirtual *v;

	readwidget(mtnrsrc,"UNITS");
	readwidget(mtnrsrc,"RATE AMOUNT");
	CLEAR_SCREEN_VIRTUAL(mtnrsrc);
	FINDRSCGETDOUBLE(mtnrsrc,"UNITS",&UNITS);
	FINDFLDSETDOUBLE(dbsort->fileno,"UNITS",UNITS);
	FINDRSCGETDOUBLE(mtnrsrc,"RATE AMOUNT",&RATE_AMT);
	FINDFLDSETDOUBLE(dbsort->fileno,"RATE AMOUNT",RATE_AMT);
	FINDRSCGETSTRING(mtnrsrc,"[POSRATE][CHECK GROSS]",&exprssn);
	FINDFLDGETCHAR(dbsort->fileno,"RATE TYPE",&ratetype);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	ADVupdateSCRNvirtuals(mtnrsrc,SubSelSubData,p);
	x=FINDVIRTUAL(mtnrsrc,"PAY AMOUNT");
	if(x!=(-1))
	{
		v=mtnrsrc->virflds+x;
		v->computed=FALSE;
	}
	if(exprssn!=NULL)
	{
		CHECK_GROSS=EVALUATEdbl(exprssn,SubSelSubData,p);
		CHECK_GROSS=round(CHECK_GROSS);
		Rfree(exprssn);
	}
	FINDFLDSETDOUBLE(dbsort->fileno,"CHECK GROSS",CHECK_GROSS);
	FINDRSCSETDOUBLE(mtnrsrc,"CHECK GROSS",CHECK_GROSS);
	updatersrc(mtnrsrc,"UNITS");
	updatersrc(mtnrsrc,"CHECK GROSS");
	updatersrc(mtnrsrc,"RATE AMOUNT");
	x=FINDVIRTUAL(mtnrsrc,"PAY AMOUNT");
	if(x!=(-1))
	{
		v=mtnrsrc->virflds+x;
		*v->value.float_value=round(UNITS*RATE_AMT);
		FINDRSCSETDOUBLE(mtnrsrc,"PAY AMOUNT",*v->value.float_value);
		updatersrc(mtnrsrc,"PAY AMOUNT");
	}
	ADVupdateSCRNvirtuals(mtnrsrc,SubSelSubData,p);
	if(p!=NULL) Rfree(p);
}
static void getpreviouscb(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *perid=NULL,*rateid=NULL;
	int jobno=0;
	PassableStruct *p=NULL;

	ZERNRD(dbsort->fileno);
	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"PERSONNEL IDENTIFICATION",&perid);
	FINDRSCGETINT(mtnrsrc,"JOB NUMBER",&jobno);
	FINDRSCGETSTRING(mtnrsrc,"RATE IDENTIFICATION",&rateid);
	FINDFLDSETSTRING(dbsort->fileno,"PAY IDENTIFICATION",pay_id);
	FINDFLDSETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(dbsort->fileno,"JOB NUMBER",jobno);
	FINDFLDSETSTRING(dbsort->fileno,"RATE IDENTIFICATION",rateid);
	if(EQLNRD(dbsort->fileno,1))
	{
		KEYNRD(dbsort->fileno,1);
		if(!LTENRD(dbsort->fileno,1))
		{
			updatesinglerecord2rsrc(dbsort->fileno,mtnrsrc);	
		}
	} else {
		PRVNRD(dbsort->fileno,1);
		updatesinglerecord2rsrc(dbsort->fileno,mtnrsrc);	
	}
	if(perid!=NULL) Rfree(perid);
	if(rateid!=NULL) Rfree(rateid);
	getsupportingfiles(mtnrsrc,dbsort,TRUE);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,SubSelSubData,p,TRUE);
	if(p!=NULL) Rfree(p);
}
static void getnextcb(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *perid=NULL,*rateid=NULL;
	int jobno=0;
	PassableStruct *p=NULL;

	ZERNRD(dbsort->fileno);
	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"PERSONNEL IDENTIFICATION",&perid);
	FINDRSCGETINT(mtnrsrc,"JOB NUMBER",&jobno);
	FINDRSCGETSTRING(mtnrsrc,"RATE IDENTIFICATION",&rateid);
	FINDFLDSETSTRING(dbsort->fileno,"PAY IDENTIFICATION",pay_id);
	FINDFLDSETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(dbsort->fileno,"JOB NUMBER",jobno);
	FINDFLDSETSTRING(dbsort->fileno,"RATE IDENTIFICATION",rateid);
	if(EQLNRD(dbsort->fileno,1))
	{
		KEYNRD(dbsort->fileno,1);
		if(!GTENRD(dbsort->fileno,1))
		{
			updatesinglerecord2rsrc(dbsort->fileno,mtnrsrc);	
		}
	} else {
		NXTNRD(dbsort->fileno,1);
		updatesinglerecord2rsrc(dbsort->fileno,mtnrsrc);	
	}
	if(perid!=NULL) Rfree(perid);
	if(rateid!=NULL) Rfree(rateid);
	getsupportingfiles(mtnrsrc,dbsort,TRUE);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,SubSelSubData,p,TRUE);
	if(p!=NULL) Rfree(p);
}
static void getfile(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	char *perid=NULL,*rateid=NULL;
	int jobno=0;
	PassableStruct *p=NULL;

	ZERNRD(dbsort->fileno);
	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"PERSONNEL IDENTIFICATION",&perid);
	FINDRSCGETINT(mtnrsrc,"JOB NUMBER",&jobno);
	FINDRSCGETSTRING(mtnrsrc,"RATE IDENTIFICATION",&rateid);
	FINDFLDSETSTRING(dbsort->fileno,"PAY IDENTIFICATION",pay_id);
	FINDFLDSETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(dbsort->fileno,"JOB NUMBER",jobno);
	FINDFLDSETSTRING(dbsort->fileno,"RATE IDENTIFICATION",rateid);
	if(EQLNRD(dbsort->fileno,1))
	{
		KEYNRD(dbsort->fileno,1);
	} else {
		updatesinglerecord2rsrc(dbsort->fileno,mtnrsrc);	
	}
	if(perid!=NULL) Rfree(perid);
	if(rateid!=NULL) Rfree(rateid);
	getsupportingfiles(mtnrsrc,dbsort,TRUE);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,SubSelSubData,p,TRUE);
	if(p!=NULL) Rfree(p);
}
static void paytypfunc(RDArsrc *mainrsrc)
{
	char delflag=FALSE;
	PassableStruct *p=NULL;

	p=Rmalloc(sizeof(PassableStruct));
	p->r=mainrsrc;
	p->a=NULL;
	if(ADVEQLNRDsec(paytyp,1,SubSelSubData,p))
	{
		ReadRDAScrolledLists(mainrsrc);
		if(ADVEQLNRDsec(paytyp,1,SubSelSubData,p))
		{
			ZERNRD(paytyp);
		}
	} else {
		FINDFLDGETCHAR(paytyp,"DELETEFLAG",&delflag);
		if(delflag)
		{
			ReadRDAScrolledLists(mainrsrc);
			if(ADVEQLNRDsec(paytyp,1,SubSelSubData,p))
			{
				ZERNRD(paytyp);
			}
		}
	}
	if(p!=NULL) Rfree(p);
}
static void paytypcb(RDArsrc *mainrsrc)
{
	readwidget(mainrsrc,"[PAYTYP][PAY TYPE]");
	FINDRSCGETSTRING(mainrsrc,"[PAYTYP][PAY TYPE]",&pay_type);
	FINDFLDSETSTRING(paytyp,"PAY TYPE",pay_type);
	paytypfunc(mainrsrc);
	updatefilerecord2rsrc(paytyp,mainrsrc);
	updateSCRNvirtuals(mainrsrc);
}
void selectm(short dowhich,void *targetkey,int argc,char **argv,DBsort *t)
{
	int x;
	RDAvirtual *v;
	NRDfield *fields=NULL,*fldx=NULL;
	short nofields=0;
	RDArsrc *mtnrsrc=NULL;
	PassableStruct *p=NULL;

	mtnrsrc=RDArsrcNEW(module,"SELECT SUBSTITUTES PAYROLL MAINTAIN SCREEN");
	ZERNRD(t->fileno);
	addDFincvir(mtnrsrc,"SECURITY","USERS",NULL,t->fileno); 
	addDFincvir(mtnrsrc,"SUBMGT","SUBMASTR",NULL,submstr);
	addDFincvir(mtnrsrc,"SUBMGT","SUBWKMAS",NULL,subwrk);
	addDFincvir(mtnrsrc,module,"PAYJMST",NULL,payjmst);
	addDFincvir(mtnrsrc,"PRSNNL","PERDMG",NULL,prsnnl);
	addDFincvir(mtnrsrc,"PRSNNL","PERSTAT",NULL,perstat);
	addDFincvir(mtnrsrc,"POSTRK","POSMSTR",NULL,posmstr);
	addDFincvir(mtnrsrc,"POSTRK","POSLMST",NULL,poslmst);
	addDFincvir(mtnrsrc,"POSTRK","POSGRS",NULL,pgross);
	addDFincvir(mtnrsrc,"POSTRK","POSGRAT",NULL,pgrat);
	addDFincvir(mtnrsrc,"POSTRK","POSRATE",NULL,posrate);
	addDFincvir(mtnrsrc,module,"PAYJCSM",NULL,payjcsm);
	addDFincvir(mtnrsrc,module,"PAYJFSM",NULL,payjfsm);
	addDFincvir(mtnrsrc,module,"PAYPCSM",NULL,paypcsm);
	addDFincvir(mtnrsrc,module,"PAYPFSM",NULL,paypfsm);
	addDFincvir(mtnrsrc,module,"PAYMSTR",NULL,paymstr);
	addDFincvir(mtnrsrc,module,"PAYJRCS",NULL,payjrcs);
	addDFincvir(mtnrsrc,module,"PAYJRFS",NULL,payjrfs);
	addDFincvir(mtnrsrc,module,"PAYTYP",NULL,paytyp);
	GET_ALL_SCREEN_VIRTUALS(mtnrsrc,0);
	nofields=NUMFLDS(t->fileno);
	fields=FLDPOINTER(t->fileno);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			ADVnonlstmakefld(mtnrsrc,t->fileno,fldx,TRUE,TRUE);
			FINDRSCSETFUNC(mtnrsrc,fldx->name,setfiles,t);
		}
	}
	if(submstr!=(-1)) file2rsrc(submstr,mtnrsrc,FALSE);
	if(subwrk!=(-1)) file2rsrc(subwrk,mtnrsrc,FALSE);
	if(payjmst!=(-1)) file2rsrc(payjmst,mtnrsrc,FALSE);
	if(prsnnl!=(-1)) file2rsrc(prsnnl,mtnrsrc,FALSE);
	if(perstat!=(-1)) file2rsrc(perstat,mtnrsrc,FALSE);
	if(posmstr!=(-1)) file2rsrc(posmstr,mtnrsrc,FALSE);
	if(pgross!=(-1)) file2rsrc(pgross,mtnrsrc,FALSE);
	if(pgrat!=(-1)) file2rsrc(pgrat,mtnrsrc,FALSE);
	if(posrate!=(-1)) file2rsrc(posrate,mtnrsrc,FALSE);
	if(payjcsm!=(-1)) file2rsrc(payjcsm,mtnrsrc,FALSE);
	if(payjfsm!=(-1)) file2rsrc(payjfsm,mtnrsrc,FALSE);
	if(paymstr!=(-1)) file2rsrc(paymstr,mtnrsrc,FALSE);
	if(payjrfs!=(-1)) file2rsrc(payjrfs,mtnrsrc,FALSE);
	if(payjrcs!=(-1)) file2rsrc(payjrcs,mtnrsrc,FALSE);
	if(paypcsm!=(-1)) file2rsrc(paypcsm,mtnrsrc,FALSE);
	if(paypfsm!=(-1)) file2rsrc(paypfsm,mtnrsrc,FALSE);
	if(paytyp!=(-1)) file2rsrc(paytyp,mtnrsrc,FALSE);
	FINDRSCSETFUNC(mtnrsrc,"PERSONNEL IDENTIFICATION",getfile,t);
	FINDRSCSETFUNC(mtnrsrc,"JOB NUMBER",getfile,t);
	FINDRSCSETFUNC(mtnrsrc,"RATE IDENTIFICATION",getfile,t);
	FINDRSCSETFUNC(mtnrsrc,"POSITION IDENTIFICATION",getposition,t);
	FINDRSCSETFUNC(mtnrsrc,"GROSS IDENTIFICATION",getgross,t);
	FINDRSCSETFUNC(mtnrsrc,"PAY IDENTIFICATION",getfile,t);
	FINDRSCSETFUNC(mtnrsrc,"RATE AMOUNT",getnewamt,t);
	FINDRSCSETFUNC(mtnrsrc,"UNITS",getnewunits,t);
	SCRNvirtual2rsrc(mtnrsrc);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1: /* this option isn't used */
			break;
		case 2:
			FINDRSCSETSTRING(mtnrsrc,"PAY IDENTIFICATION",pay_id);
			FINDFLDSETSTRING(t->fileno,"PAY IDENTIFICATION",pay_id);
			ReadRDAScrolledLists(mtnrsrc);
			if(EQLNRD(t->fileno,1))
			{
				rsrc2singlefilerecord(t->fileno,mtnrsrc);
			} else {
				singlefilerecord2rsrc(t->fileno,mtnrsrc);
			}
			break;
		default:
		case 0:
			if(ADVEQLNRDKEYsec(t->fileno,1,targetkey,SubSelSubData,p)) 
			{
				KEYNRD(t->fileno,1);
				ReadRDAScrolledLists(mtnrsrc);
				if(EQLNRD(t->fileno,1))
				{
					rsrc2singlefilerecord(t->fileno,mtnrsrc);
				} else {
					singlefilerecord2rsrc(t->fileno,mtnrsrc);
				}
			} else {
				singlefilerecord2rsrc(t->fileno,mtnrsrc);
			}
			break;
	}
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=t;
	ADVSetVirFieldFuncs(mtnrsrc,SubSelSubData,p);
	getsupportingfiles(mtnrsrc,t,FALSE);
	addbtnrsrc(mtnrsrc,"PREVIOUS",TRUE,getpreviouscb,t);
	addbtnrsrc(mtnrsrc,"NEXT",TRUE,getnextcb,t);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetmtndefs,t);
	addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,t);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,t);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfexrsrc(mtnrsrc,"QUIT",TRUE,quit_record,t);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,SubSelSubData,p,FALSE);
	APPmakescrn(mtnrsrc,TRUE,quit_record,t,FALSE);
	if(p!=NULL) Rfree(p);
	x=FINDVIRTUAL(mtnrsrc,"PAY AMOUNT");
	if(x!=(-1))
	{
		v=mtnrsrc->virflds+x;
		FINDRSCSETDOUBLE(mtnrsrc,"PAY AMOUNT",*v->value.float_value);
	}
	getnewamt(mtnrsrc,t);	
}
static void save_record_up(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	save_record(mtnrsrc,TRUE,dbsort);
}
static void save_record_nup(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	save_record(mtnrsrc,FALSE,dbsort);
}
static void save_record(RDArsrc *mtnrsrc,short update_list,DBsort *dbsort)
{
	int valuex=0;
	RDArsrc *tmprsrc=NULL;
	char *e=NULL;

	readsinglefilewidgets(dbsort->fileno,mtnrsrc);
	rsrc2singlefilerecord(dbsort->fileno,mtnrsrc);
	WRTNRD(dbsort->fileno,0,NULL);
	if(update_list==TRUE)
	{
		if(mbl_trans->mainrsrc!=NULL)
		{
			FINDRSCGETINT(mbl_trans->mainrsrc,"BROWSE LIST",&valuex);
			tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN","SECURITY","Browse Diagnostic",NULL);
			if(tmprsrc!=NULL)
			{
				if(!ADVmakescrn(tmprsrc,TRUE))
				{
					ForceWindowUpdate(tmprsrc);
				} else {
					e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
					sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
					WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
					prterr("Error %s.",e);
					if(e!=NULL) Rfree(e);
				}
			}
			xmakebrowselist(mbl_trans,tmprsrc);
			if(mbl_trans->list->num<=valuex) valuex=0;
			mbl_trans->list->keyvalue=mbl_trans->list->key[valuex];
			if(!FINDRSCSETLIST(mbl_trans->mainrsrc,"BROWSE LIST",valuex,
				mbl_trans->list->num,&mbl_trans->list->string))
			{
				updatersrc(mbl_trans->mainrsrc,"BROWSE LIST");
			}
			if(tmprsrc!=NULL)
			{
				killwindow(tmprsrc);
				free_rsrc(tmprsrc);
			}
			ForceWindowUpdate(mtnrsrc);
		}
	}
	if(mtnrsrc!=NULL) killwindow(mtnrsrc);
	quit_record(mtnrsrc,dbsort);
}
static void quit_record(RDArsrc *mtnrsrc,DBsort *dbsort)
{
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	if(WindowCount<2) select_backend(dbsort);
}
static void getsupportingfiles(RDArsrc *mtnrsrc,DBsort *dbsort,short update)
{
	char *exprssn=NULL,ratetype=FALSE;
	PassableStruct *p=NULL;
	int x;
	RDAvirtual *v;
	char r_edit=FALSE,gr_edit=FALSE;

											CLEAR_SCREEN_VIRTUAL(mtnrsrc);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=dbsort;
	COPYFIELD(dbsort->fileno,paymstr,"PAY IDENTIFICATION");
	if(ADVEQLNRDsec(paymstr,1,SubSelSubData,p)) KEYNRD(paymstr,1);
	if(update) updatefilerecord2rsrc(paymstr,mtnrsrc);
	else filerecord2rsrc(paymstr,mtnrsrc);
	COPYFIELD(paymstr,paytyp,"PAY TYPE");
	if(ADVEQLNRDsec(paytyp,1,SubSelSubData,p)) KEYNRD(paytyp,1);
	if(update) updatefilerecord2rsrc(paytyp,mtnrsrc);
	else filerecord2rsrc(paytyp,mtnrsrc);

	COPYFIELD(dbsort->fileno,posmstr,"POSITION IDENTIFICATION");
	if(ADVEQLNRDsec(posmstr,1,SubSelSubData,p)) KEYNRD(posmstr,1);
	if(update) updatefilerecord2rsrc(posmstr,mtnrsrc);
	else filerecord2rsrc(posmstr,mtnrsrc);

	COPYFIELD(dbsort->fileno,poslmst,"LOCATION IDENTIFICATION");
	if(ADVEQLNRDsec(poslmst,1,SubSelSubData,p)) KEYNRD(poslmst,1);
	if(update) updatefilerecord2rsrc(poslmst,mtnrsrc);
	else filerecord2rsrc(poslmst,mtnrsrc);

	COPYFIELD(dbsort->fileno,pgross,"GROSS IDENTIFICATION");
	if(ADVEQLNRDsec(pgross,1,SubSelSubData,p)) KEYNRD(pgross,1);
	if(update) updatefilerecord2rsrc(pgross,mtnrsrc);
	else filerecord2rsrc(pgross,mtnrsrc);

	COPYFIELD(dbsort->fileno,posrate,"RATE IDENTIFICATION");
	if(ADVEQLNRDsec(posrate,1,SubSelSubData,p)) KEYNRD(posrate,1);
	if(update) updatefilerecord2rsrc(posrate,mtnrsrc);
	else filerecord2rsrc(posrate,mtnrsrc);
	FINDFLDGETCHAR(posrate,"EDITABLE",&r_edit);

	COPYFIELD(dbsort->fileno,pgrat,"GROSS IDENTIFICATION");
	COPYFIELD(dbsort->fileno,pgrat,"RATE IDENTIFICATION");
	if(ADVEQLNRDsec(pgrat,1,SubSelSubData,p)) KEYNRD(pgrat,1);
	if(update) updatefilerecord2rsrc(pgrat,mtnrsrc);
	else filerecord2rsrc(pgrat,mtnrsrc);
	FINDFLDGETCHAR(pgrat,"EDITABLE",&gr_edit);
	if(r_edit && gr_edit)
	{
		FINDRSCSETEDITABLE(mtnrsrc,"RATE AMOUNT",TRUE);
	} else {
		FINDRSCSETEDITABLE(mtnrsrc,"RATE AMOUNT",FALSE);
	}

	COPYFIELD(dbsort->fileno,payjmst,"PERSONNEL IDENTIFICATION");
	COPYFIELD(dbsort->fileno,payjmst,"JOB NUMBER");
	if(ADVEQLNRDsec(payjmst,1,SubSelSubData,p)) KEYNRD(payjmst,1);
	if(update) updatefilerecord2rsrc(payjmst,mtnrsrc);
	else filerecord2rsrc(payjmst,mtnrsrc);

	COPYFIELD(dbsort->fileno,payjfsm,"PERSONNEL IDENTIFICATION");
	COPYFIELD(dbsort->fileno,payjfsm,"JOB NUMBER");
	COPYFIELD(paymstr,payjfsm,"FISCAL YEAR");
	if(ADVEQLNRDsec(payjfsm,1,SubSelSubData,p)) KEYNRD(payjfsm,1);
	if(update) updatefilerecord2rsrc(payjcsm,mtnrsrc);
	else filerecord2rsrc(payjcsm,mtnrsrc);

	COPYFIELD(dbsort->fileno,prsnnl,"PERSONNEL IDENTIFICATION");
	if(ADVEQLNRDsec(prsnnl,1,SubSelSubData,p)) KEYNRD(prsnnl,1);
	if(update) updatefilerecord2rsrc(prsnnl,mtnrsrc);
	else filerecord2rsrc(prsnnl,mtnrsrc);

	COPYFIELD(prsnnl,perstat,"STATUS TITLE");
	if(ADVEQLNRDsec(perstat,1,SubSelSubData,p)) KEYNRD(perstat,1);
	if(update) updatefilerecord2rsrc(perstat,mtnrsrc);
	else filerecord2rsrc(perstat,mtnrsrc);

	COPYFIELD(dbsort->fileno,payjrcs,"PERSONNEL IDENTIFICATION");
	COPYFIELD(dbsort->fileno,payjrcs,"JOB NUMBER");
	COPYFIELD(dbsort->fileno,payjrcs,"RATE IDENTIFICATION");
	COPYFIELD(paymstr,payjrcs,"CALENDAR YEAR");
	if(ADVEQLNRDsec(payjrcs,1,SubSelSubData,p)) KEYNRD(payjrcs,1);
	if(update) updatefilerecord2rsrc(payjrcs,mtnrsrc);
	else filerecord2rsrc(payjrcs,mtnrsrc);

	COPYFIELD(dbsort->fileno,payjrfs,"PERSONNEL IDENTIFICATION");
	COPYFIELD(dbsort->fileno,payjrfs,"JOB NUMBER");
	COPYFIELD(dbsort->fileno,payjrfs,"RATE IDENTIFICATION");
	COPYFIELD(paymstr,payjrfs,"FISCAL YEAR");
	if(ADVEQLNRDsec(payjrfs,1,SubSelSubData,p)) KEYNRD(payjrfs,1);
	if(update) updatefilerecord2rsrc(payjrfs,mtnrsrc);
	else filerecord2rsrc(payjrfs,mtnrsrc);

	COPYFIELD(dbsort->fileno,paypcsm,"PERSONNEL IDENTIFICATION");
	COPYFIELD(paymstr,paypcsm,"CALENDAR YEAR");
	if(ADVEQLNRDsec(paypcsm,1,SubSelSubData,p)) KEYNRD(paypcsm,1);
	if(update) updatefilerecord2rsrc(paypcsm,mtnrsrc);
	else filerecord2rsrc(paypcsm,mtnrsrc);

	COPYFIELD(dbsort->fileno,paypfsm,"PERSONNEL IDENTIFICATION");
	COPYFIELD(paymstr,paypfsm,"FISCAL YEAR");
	if(ADVEQLNRDsec(paypfsm,1,SubSelSubData,p)) KEYNRD(paypfsm,1);
	if(update) updatefilerecord2rsrc(paypfsm,mtnrsrc);
	else filerecord2rsrc(paypfsm,mtnrsrc);

	FINDFLDGETSTRING(pgrat,"RATE AMOUNT",&exprssn);
	FINDFLDGETCHAR(dbsort->fileno,"RATE TYPE",&ratetype);
	if(ratetype==FALSE)
	{
		x=FINDVIRTUAL(mtnrsrc,"PAY AMOUNT");
		if(x!=(-1))
		{
			v=mtnrsrc->virflds+x;
			v->computed=FALSE;
		}
		RATE_AMT=0.0;
		if(exprssn!=NULL)
		{
			RATE_AMT=EVALUATEdbl(exprssn,SubSelSubData,p);
			Rfree(exprssn);
		}
		FINDFLDSETDOUBLE(dbsort->fileno,"RATE AMOUNT",RATE_AMT);
	} else {
		FINDFLDGETDOUBLE(dbsort->fileno,"RATE AMOUNT",&RATE_AMT);
		x=FINDVIRTUAL(mtnrsrc,"PAY AMOUNT");
		if(x!=(-1))
		{
			v=mtnrsrc->virflds+x;
			computevirtual(v,SubSelSubData,p);
			updatersrc(mtnrsrc,"PAY AMOUNT");
		}
	}
	FINDRSCSETDOUBLE(mtnrsrc,"RATE AMOUNT",RATE_AMT);
	FINDFLDGETSTRING(posrate,"CHECK GROSS",&exprssn);
	if(exprssn!=NULL)
	{
		CHECK_GROSS=EVALUATEdbl(exprssn,SubSelSubData,p);
		CHECK_GROSS=round(CHECK_GROSS);
		Rfree(exprssn);
	}
	FINDFLDSETDOUBLE(dbsort->fileno,"CHECK GROSS",CHECK_GROSS);
	FINDRSCSETDOUBLE(mtnrsrc,"CHECK GROSS",CHECK_GROSS);

	COPYFIELD(dbsort->fileno,payjcsm,"PERSONNEL IDENTIFICATION");
	COPYFIELD(dbsort->fileno,payjcsm,"JOB NUMBER");
	COPYFIELD(paymstr,payjcsm,"CALENDAR YEAR");
	if(ADVEQLNRDsec(payjcsm,1,SubSelSubData,p)) KEYNRD(payjcsm,1);
	if(p!=NULL) Rfree(p);
	if(update) updatefilerecord2rsrc(payjcsm,mtnrsrc);
	else filerecord2rsrc(payjcsm,mtnrsrc);
}
