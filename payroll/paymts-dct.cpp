/* paymts.c - Payroll's Make Transactions */
#ifndef WIN32
#define __NAM__ "paymts.lnx"
#endif
#ifdef WIN32
#define __NAM__ "paymts.exe"
#endif
#include <app.hpp>

#include <mix.hpp>
#include <fin.hpp>
#include <ldval.hpp>
#include <pay.hpp>
/*COMPMEM 40960 */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static char *module="PAYROLL",*audit_reportname=NULL;
static char printed_errorlist=FALSE,printed_warninglist=FALSE;
static char Force_Warning=TRUE,Use_Compute_Completed=FALSE;
static char Use_Rate_Accounts=FALSE,did_payunmts=FALSE,did_findit=FALSE;
static char job_rollsover_deduction=FALSE,EncumberPayroll=FALSE;
static short payjprm=(-1),posracc=(-1);
static short payjpms=(-1),paymstr=(-1),payddac=(-1),payppms=(-1);
static short payjtrn=(-1),payjdpm=(-1),paydtrn=(-1),payjams=(-1);
static short payddms=(-1),payjmst=(-1),paydpms=(-1),payjdac=(-1);
static short paydedm=(-1),posacc=(-1),eyrnum=(-1),eaccnum=(-1),byrnum=(-1),baccnum=(-1);
static int DO_AUDIT=FALSE;
RDApayroll *PAYROLL_SETUP=NULL;
static RDAfinmgt *finsetup=NULL;
static APPlib *errorlist=NULL,*warnlist=NULL;
#define SeeJobStruct(a)	xSeeJobStruct(a,__LINE__,__FILE__)
double ROLL_OVER_TEST(short,double,char *);

struct AccountStructs
{
	int accno;
	short deftype;
	char *account;
	char *debit;
	char *credit;
	short pdeftype;
	char *payable;
	char *cash;
	char *due_from;
	char *enc_code;
	char *res_code;
	char disenc;
	char prorate;
	char skip_deductions;
	double percentage;
	double amount;
};
typedef struct AccountStructs AccountStruct;

#define SeeAccountStruct(a)	xSeeAccountStruct(a,__LINE__,__FILE__)
void xSeeAccountStruct(AccountStruct *a,int line,char *file)
{
	if(a!=NULL)
	{
		prterr("DIAG SeeAccountStruct at line [%d] program [%s].",line,file);
		prterr("DIAG SeeAccountStruct Acc No [%d] Def [%d] Account [%s] Debit [%s] Credit [%s] Payable [%s] Prorate [%s] Percentage [%f] Amount [%.0f] Skip Deductions [%s].\n",
			a->accno,a->deftype,(a->account!=NULL ? a->account:""),
			(a->debit!=NULL ? a->debit:""),(a->credit!=NULL ? 
			a->credit:""),(a->payable!=NULL ? a->payable:""),
			(a->prorate ? "True":"False"),a->percentage,
			a->amount,(a->skip_deductions ? "True":"False"));
	} else {
		prterr("DIAG SeeAccountStruct is NULL at line [%d] program [%s].",line,file);
	}
}
struct paymtss
{
	int num;
	char roll_over_budget;
	AccountStruct *a;
};
typedef struct paymtss PAYMTS;

#define SeePAYMTS(a)	xSeePAYMTS(a,__LINE__,__FILE__)
void xSeePAYMTS(PAYMTS *p,int line,char *file)
{
	AccountStruct *a;
	int x;

	if(p!=NULL)
	{
		prterr("DIAG SeePAYMTS at line [%d] program [%s].",line,file);
		if(p->a!=NULL && p->num>0)
		{
			prterr("PAYMTS Has [%d] AccountStructs.\n",p->num);
			for(x=0,a=p->a;x<p->num;++x,++a) 
			{
				prterr("Account Struct #%d:\n",x);
				SeeAccountStruct(a);
			}
		} else {
			prterr("DIAG SeePAYMTS AccountStruct is NULL at line [%d] program [%s].",line,file);
		}
	} else {
		prterr("DIAG SeePAYMTS is NULL at line [%d] program [%s].",line,file);
	}
}
static PAYMTS *PAYMTSNEW()
{
	PAYMTS *tmp=NULL;

	tmp=Rmalloc(sizeof(PAYMTS));
	tmp->num=0;
	tmp->roll_over_budget=0;
	tmp->a=NULL;
	return(tmp);
}
static void AddAccountStruct(PAYMTS *p,int accno,short dtype,char *acc,
	char *deb,char *cred,short pdef,
	char *payable,char *cash,char *due_from,char *enc_code,
	char *res_code,char prorate,double percentage,double amount,char disenc,char no_deds)
{
	AccountStruct *a;

	if(p->a!=NULL)
	{
		p->a=Rrealloc(p->a,(p->num+1)*sizeof(AccountStruct));
	} else {
		p->a=Rmalloc(sizeof(AccountStruct));
		p->num=0;
	}

	
	a=p->a+p->num;
	a->accno=accno;
	a->deftype=dtype;
	a->disenc=disenc;
	if(!isEMPTY(acc)) a->account=stralloc(acc);
		else a->account=NULL;
	if(!isEMPTY(deb)) a->debit=stralloc(deb);
		else a->debit=NULL;
	if(!isEMPTY(cred)) a->credit=stralloc(cred);
		else a->credit=NULL;
	a->pdeftype=pdef;
	if(!isEMPTY(payable)) a->payable=stralloc(payable);
		else a->payable=NULL;
	if(!isEMPTY(cash)) a->cash=stralloc(cash);
		else a->cash=NULL;
	if(RDAstrcmp(XPERT_SETUP->zip,"62702-5030"))
	{
	if(!isEMPTY(due_from)) a->due_from=stralloc(due_from);
		else a->due_from=NULL;
	} else {
		memset(stemp,0,101);
		sprintf(stemp,"09-0000-113%.2s",a->account);
		a->due_from=stralloc(stemp);
	}
	if(!isEMPTY(enc_code)) a->enc_code=stralloc(enc_code);
		else a->enc_code=NULL;
	if(!isEMPTY(res_code)) a->res_code=stralloc(res_code);
		else a->res_code=NULL;
	a->prorate=prorate;
	a->percentage=percentage;
	a->amount=amount;
	a->skip_deductions=no_deds;
	if(diagapps)
	{
		SeeAccountStruct(a);TRACE;
	}
	++p->num;
}
static void FreePAYMTS(PAYMTS *p)
{
	int x;
	AccountStruct *a;

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
				if(a->due_from!=NULL) Rfree(a->due_from);
				if(a->enc_code!=NULL) Rfree(a->enc_code);
				if(a->res_code!=NULL) Rfree(a->res_code);
			}
			Rfree(p->a);
		}
		Rfree(p);
	}
}
static short BlendExists(PAYMTS *mts,char *perid,int jobno,char *payid)
{
	char *perid1=NULL,*payid1=NULL,delflag=FALSE,*rateid=NULL;
	int jobno1=0;
	int x=0;
	AccountStruct *a;
	double gross=0.0;
	short ef=0;

	if(mts==NULL) return(FALSE);
	ZERNRD(payjprm);
	FINDFLDSETSTRING(payjprm,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETSTRING(payjprm,"PAY IDENTIFICATION",payid);
	FINDFLDSETINT(payjprm,"JOB NUMBER",jobno);
	ef=GTENRD(payjprm,1);
	while(!ef)
	{
		FINDFLDGETSTRING(payjprm,"PERSONNEL IDENTIFICATION",&perid1);
		FINDFLDGETSTRING(payjprm,"PAY IDENTIFICATION",&payid1);
		FINDFLDGETINT(payjprm,"JOB NUMBER",&jobno1);
		if(RDAstrcmp(perid,perid1) || RDAstrcmp(payid,payid1) || jobno!=jobno1) break;
		FINDFLDGETCHAR(payjprm,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			FINDFLDGETSTRING(payjprm,"RATE IDENTIFICATION",&rateid);
			FINDFLDGETDOUBLE(payjprm,"GROSS",&gross);
			if(gross!=0.0)
			{
			for(x=0,a=mts->a;x<mts->num;++x,++a)
			{
				ZERNRD(posracc);
				FINDFLDSETSTRING(posracc,"RATE IDENTIFICATION",rateid);
				FINDFLDSETSHORT(posracc,"DEFINITION TYPE",a->deftype);
				if(!EQLNRD(posracc,1))
				{
					FINDFLDGETCHAR(posracc,"DELETEFLAG",&delflag);
					if(!delflag)
					{
						if(rateid!=NULL) Rfree(rateid);
						if(perid1!=NULL) Rfree(perid1);
						if(payid1!=NULL) Rfree(payid1);
						return(TRUE);	
					}
				}
			}
			}
		}
		ef=NXTNRD(payjprm,1);
	}
	if(rateid!=NULL) Rfree(rateid);
	if(perid1!=NULL) Rfree(perid1);
	if(payid1!=NULL) Rfree(payid1);
	return(FALSE);
}
static PAYMTS *BlendedRates(PAYMTS *mts,char *perid,int jobno,char *payid,double jobgross,RDArsrc *diagrsrc,int fiscal_year)
{
	char *perid1=NULL,*payid1=NULL;
	char *acc_image=NULL,*deb_image=NULL,*cred_image=NULL,*pay_image=NULL;
	char *due_image=NULL,*cash_image=NULL,*rateid=NULL,delflag=FALSE;
	char *enc_image=NULL,*res_image=NULL;
	char *new_acc=NULL,*new_deb=NULL,*new_cred=NULL,*new_pay=NULL;
	char *new_due=NULL,*new_cash=NULL,*new_enc=NULL,*new_res=NULL;
	char No_Encumbrance=FALSE;
	short pf_deftype=0,ef=0,which=0;
	int jobno1=0;
	int x=0;
	PAYMTS *blend=NULL;
	AccountStruct *a;
	double percentage=0.0,total_percentage=0.0,amount=0.0,total_amount=0.0;
	double gross=0.0,p=0.0,total_rgross=0.0;
	double zamt=0.0;
	char nodeds=FALSE;
	
	if(mts==NULL) return(NULL);
	blend=PAYMTSNEW();
	if(!BlendExists(mts,perid,jobno,payid))
	{
		for(x=0,a=mts->a;x<mts->num;++x,++a)
		{
			AddAccountStruct(blend,a->accno,a->deftype,a->account,a->debit,a->credit,a->pdeftype,a->payable,a->cash,a->due_from,a->enc_code,a->res_code,a->prorate,a->percentage,a->amount,a->disenc,FALSE);
		}
		return(blend);
	}
	ZERNRD(payjprm);
	FINDFLDSETSTRING(payjprm,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETSTRING(payjprm,"PAY IDENTIFICATION",payid);
	FINDFLDSETINT(payjprm,"JOB NUMBER",jobno);
	ef=GTENRD(payjprm,1);
	while(!ef)
	{
		FINDFLDGETSTRING(payjprm,"PERSONNEL IDENTIFICATION",&perid1);
		FINDFLDGETSTRING(payjprm,"PAY IDENTIFICATION",&payid1);
		FINDFLDGETINT(payjprm,"JOB NUMBER",&jobno1);
		if(RDAstrcmp(perid,perid1) || RDAstrcmp(payid,payid1) || jobno!=jobno1) break;
		FINDFLDGETCHAR(payjprm,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			FINDFLDGETSTRING(payjprm,"RATE IDENTIFICATION",&rateid);
			FINDFLDGETDOUBLE(payjprm,"GROSS",&gross);
			if(gross!=0.0)
			{
			total_amount=0;total_percentage=0;
			for(x=0,a=mts->a;x<mts->num;++x,++a)
			{
				if(total_amount>=gross && gross>0.0) 
				{
					break;
				}
				ZERNRD(posracc);
				FINDFLDSETSTRING(posracc,"RATE IDENTIFICATION",rateid);
				FINDFLDSETSHORT(posracc,"DEFINITION TYPE",a->deftype);
				which=FALSE;
				No_Encumbrance=FALSE;
				if(!EQLNRD(posracc,1))
				{
					FINDFLDGETCHAR(posracc,"DELETEFLAG",&delflag);
					if(!delflag)
					{
						FINDFLDGETSTRING(posracc,"ACCOUNT CODE IMAGE",&acc_image);
						FINDFLDGETSTRING(posracc,"DEBIT CODE IMAGE",&deb_image);
						FINDFLDGETSTRING(posracc,"CREDIT CODE IMAGE",&cred_image);
						FINDFLDGETSTRING(posracc,"PAYABLE CODE IMAGE",&pay_image);
						FINDFLDGETSTRING(posracc,"ENCUMBRANCE CODE IMAGE",&enc_image);
						FINDFLDGETSTRING(posracc,"RESERVE CODE IMAGE",&enc_image);
						FINDFLDGETSHORT(posracc,"PAYROLL FUND DEFINITION TYPE",&pf_deftype);
						FINDFLDGETSTRING(posracc,"CASH CODE IMAGE",&cash_image);
						FINDFLDGETSTRING(posracc,"DUE FROM IMAGE",&due_image);
						FINDFLDGETCHAR(posracc,"IGNORE ENCUMBRANCE REQUEST",&No_Encumbrance);
						which=TRUE;
					}

				}
				if((which==TRUE))
				{
				if(!RDAstrcmp(rateid,"MILEAGE") || !RDAstrcmp(rateid,"REIMBURSEMENT"))
				{
					 nodeds=TRUE; 
				} else {
					nodeds=FALSE;
				}
				total_rgross+=gross;
				new_acc=MergeImage(a->account,acc_image,EXPENDITURE,a->deftype);
				new_deb=MergeImage(a->debit,deb_image,BALANCESHEET,a->deftype);
				new_cred=MergeImage(a->credit,cred_image,BALANCESHEET,a->deftype);
				new_pay=MergeImage(a->payable,pay_image,BALANCESHEET,a->deftype);
				new_enc=MergeImage(a->enc_code,enc_image,BALANCESHEET,a->deftype);
				new_res=MergeImage(a->res_code,res_image,BALANCESHEET,a->deftype);
				new_cash=MergeImage(a->cash,cash_image,BALANCESHEET,pf_deftype);
				new_due=MergeImage(a->due_from,due_image,BALANCESHEET,pf_deftype);
				if((x+1)<mts->num)
				{
					if(a->prorate)
					{
						if((total_percentage+a->percentage)>100.0)
						{
							percentage=100.0-total_percentage;
						} else percentage=a->percentage;
						amount=round(gross*percentage/100);
						total_percentage+=percentage;
					} else {
						if(a->amount<0.0) 
						{
							amount=a->amount;
						} else if(a->amount<(gross-total_amount) && a->amount!=0.0 && ((x+1)==mts->num)) 
						{
							amount=a->amount;
						} else if(a->amount<(gross-total_amount) && a->amount!=0.0 && ((x+1)<mts->num))
						{
							amount=round(a->amount/jobgross*gross);
						} else if(a->amount==0.0) 
						{
							amount=gross-total_amount;
						} else {
							amount=gross-total_amount;
						}
						if(gross!=0.0) 
							total_percentage+=(amount/jobgross)*100;
					}
					if(PAYROLL_SETUP->payjams_rollover && mts->roll_over_budget)
					{
						ZERNRD(eyrnum);
						FINDFLDSETSTRING(eyrnum,"ACCOUNT CODE",new_acc);
						FINDFLDSETSHORT(eyrnum,"DEFINITION TYPE",a->deftype);
						FINDFLDSETINT(eyrnum,"FISCAL YEAR",fiscal_year);
						if(ADVEQLNRDsec(eyrnum,1,SCRNvirtualSubData,diagrsrc))
						{
							KEYNRD(eyrnum,1);
						}
						zamt=ROLL_OVER_TEST(eyrnum,amount,NULL);
						if(zamt!=(-1))
						{
							amount=zamt;
						}
					}
					total_amount+=amount;
				} else {
					if(gross>total_amount && gross>=0.0)
					{
						amount=gross-total_amount;
					} else if(gross<0)
					{
						amount=gross-total_amount;
					} else amount=0;
					total_amount+=amount;
				}
				if(amount!=0)
				{
					if((x+1)<mts->num || total_rgross!=jobgross)
					{
						p=(round(amount/jobgross*100000000))/1000000;	
						AddAccountStruct(blend,a->accno,a->deftype,new_acc,new_deb,new_cred,a->pdeftype,new_pay,new_cash,new_due,new_enc,new_res,TRUE,p,amount,(No_Encumbrance==TRUE ? FALSE:a->disenc),nodeds);
					} else {
						AddAccountStruct(blend,a->accno,a->deftype,new_acc,new_deb,new_cred,a->pdeftype,new_pay,new_cash,new_due,new_enc,new_res,FALSE,0.0,0.0,(No_Encumbrance==TRUE ? FALSE:a->disenc),nodeds);
					}
				}
				if(acc_image!=NULL) Rfree(acc_image);
				if(deb_image!=NULL) Rfree(deb_image);
				if(cred_image!=NULL) Rfree(cred_image);
				if(pay_image!=NULL) Rfree(pay_image);
				if(enc_image!=NULL) Rfree(enc_image);
				if(res_image!=NULL) Rfree(res_image);
				if(cash_image!=NULL) Rfree(cash_image);
				if(due_image!=NULL) Rfree(due_image);
				if(new_acc!=NULL) Rfree(new_acc);
				if(new_deb!=NULL) Rfree(new_deb);
				if(new_cred!=NULL) Rfree(new_cred);
				if(new_pay!=NULL) Rfree(new_pay);
				if(new_enc!=NULL) Rfree(new_enc);
				if(new_res!=NULL) Rfree(new_res);
				if(new_cash!=NULL) Rfree(new_cash);
				if(new_due!=NULL) Rfree(new_due);
				}
			}
			}
		}
		ef=NXTNRD(payjprm,1);
	}
	if((jobgross-total_rgross)!=0.0)
	{
		total_amount=0;total_percentage=0;
		gross=jobgross-total_rgross;
		for(x=0,a=mts->a;x<mts->num;++x,++a)
		{
			if(total_amount>=gross && gross>0.0) 
			{
				break;
			}
			if((x+1)<mts->num)
			{
				if(a->prorate)
				{
					if((total_percentage+a->percentage)>100.0)
					{
						percentage=100.0-total_percentage;
					} else percentage=a->percentage;
					amount=round(gross*percentage/100);
					total_percentage+=percentage;
				} else {
					if(a->amount<0.0) 
					{
						amount=a->amount;
					} else if(a->amount<(gross-total_amount) && a->amount!=0.0 && ((x+1)==mts->num)) 
					{
						amount=a->amount;
					} else if(a->amount<(gross-total_amount) && a->amount!=0.0 && ((x+1)<mts->num))
					{
						amount=round(a->amount/jobgross*gross);
					} else if(a->amount==0.0) 
					{
						amount=gross-total_amount;
					} else {
						amount=gross-total_amount;
					}
					if(gross!=0.0) 
						total_percentage+=(amount/jobgross)*100;
				}
				if(PAYROLL_SETUP->payjams_rollover && mts->roll_over_budget)
				{
					ZERNRD(eyrnum);
					FINDFLDSETSTRING(eyrnum,"ACCOUNT CODE",a->account);
					FINDFLDSETSHORT(eyrnum,"DEFINITION TYPE",a->deftype);
					FINDFLDSETINT(eyrnum,"FISCAL YEAR",fiscal_year);
					if(ADVEQLNRDsec(eyrnum,1,SCRNvirtualSubData,diagrsrc))
					{
						KEYNRD(eyrnum,1);
					}
					zamt=ROLL_OVER_TEST(eyrnum,amount,NULL);
					if(zamt!=(-1))
					{
						amount=zamt;
					}
				}
				total_amount+=amount;
			} else {
				if(gross>total_amount && gross>=0.0)
				{
					amount=gross-total_amount;
				} else if(gross<0)
				{
					amount=gross-total_amount;
				} else amount=0;
				total_amount+=amount;
			}
			if(amount!=0)
			{
				if((x+1)<mts->num || total_rgross!=jobgross)
				{
					p=(round(amount/jobgross*10000))/100;	
					AddAccountStruct(blend,a->accno,a->deftype,a->account,a->debit,a->credit,a->pdeftype,a->payable,a->cash,a->due_from,a->enc_code,a->res_code,TRUE,p,0.0,a->disenc,FALSE);
				} else {
					AddAccountStruct(blend,a->accno,a->deftype,a->account,a->debit,a->credit,a->pdeftype,a->payable,a->cash,a->due_from,a->enc_code,a->res_code,FALSE,0.0,0.0,a->disenc,FALSE);
				}
			}
		}
	}
	if(perid1!=NULL) Rfree(perid1);
	if(payid1!=NULL) Rfree(payid1);
	if(rateid!=NULL) Rfree(rateid);
	return(blend);
}
static PAYMTS *GetPAYMTS(RDArsrc *r,char *perid,int jobno,char *payid,double jobgross,int fiscal_year)
{
	short ef=FALSE,deftype=0,pdeftype=0,keyno=(-1);
	PAYMTS *mts=NULL,*blend=NULL;
	char *lperid=NULL,*acctcode=NULL,prorate=FALSE;
	char /* active=FALSE,*/ deleteflag=FALSE;
	char *debit=NULL,*credit=NULL,*payable=NULL,job_accts=TRUE,*posid=NULL;
	char *lposid=NULL,*cash=NULL,*due_from=NULL;
	char *enc_code=NULL,*res_code=NULL;
	char *tmp=NULL;
	char disenc=FALSE,block_racc=FALSE;
	int ljobno=0,accno=0;
	double perc=0.0,amt=0.0;

	mts=PAYMTSNEW();
	FINDFLDSETSTRING(payjmst,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(payjmst,"JOB NUMBER",jobno);
	if(ADVEQLNRDsec(payjmst,1,SCRNvirtualSubData,r)) KEYNRD(payjmst,1);
	FINDFLDGETCHAR(payjmst,"DELETEFLAG",&deleteflag);
	if(PAYROLL_SETUP->payjams_rollover)
	{
		FINDFLDGETCHAR(payjmst,"ROLL OVER BUDGET",&mts->roll_over_budget);
	}
	if(!deleteflag)
	{
/* removed for timing reasons, users can inactivate after select is run
	FINDFLDGETCHAR(payjmst,"ACTIVE",&active);
	if(active)
	{
*/
		FINDFLDGETCHAR(payjmst,"JOB ACCOUNTS",&job_accts);
		FINDFLDGETCHAR(payjmst,"BLOCK RATE ACCOUNT IMAGES",&block_racc);
		if(job_accts)
		{
			ZERNRD(payjams);
			FINDFLDSETSTRING(payjams,"PERSONNEL IDENTIFICATION",perid);
			FINDFLDSETINT(payjams,"JOB NUMBER",jobno);
			keyno=KEYNUMBER(payjams,"PAYJAMS KEY");
			if(keyno<1) keyno=1;
			ef=ADVGTENRDsec(payjams,keyno,SCRNvirtualSubData,r);
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
					FINDFLDGETSTRING(payjams,"ACCOUNT CODE",&acctcode);
					FINDFLDGETSTRING(payjams,"DEBIT CODE",&debit);
					FINDFLDGETSTRING(payjams,"CREDIT CODE",&credit);
					FINDFLDGETSHORT(payjams,"PAYROLL FUND DEFINITION TYPE",&pdeftype);
					FINDFLDGETSTRING(payjams,"PAYABLE CODE",&payable);
					FINDFLDGETSTRING(payjams,"CASH CODE",&cash);
					FINDFLDGETSTRING(payjams,"DUE FROM",&due_from);
					if(EncumberPayroll) FINDFLDGETCHAR(payjams,"DISENCUMBER",&disenc);
						else disenc=FALSE;
					if(disenc)
					{
						FINDFLDGETSTRING(payjams,"ENCUMBRANCE CODE",&enc_code);
						FINDFLDGETSTRING(payjams,"RESERVE CODE",&res_code);
					} else {
						if(enc_code!=NULL) Rfree(enc_code);
						if(res_code!=NULL) Rfree(res_code);
					}
					FINDFLDGETCHAR(payjams,"PRORATE",&prorate);
					FINDFLDGETDOUBLE(payjams,"PERCENTAGE",&perc);
					FINDFLDGETDOUBLE(payjams,"AMOUNT",&amt);
					AddAccountStruct(mts,accno,deftype,acctcode,debit,credit,pdeftype,payable,cash,due_from,enc_code,res_code,prorate,perc,amt,disenc,FALSE);
				}
				ef=ADVNXTNRDsec(payjams,keyno,SCRNvirtualSubData,r);
			}
			if(mts->num<1)
			{
				tmp=Rmalloc(256);
				sprintf(tmp,"MISSING JOB ACCOUNT(s)!: Personnel ID [%-15s], Job Number [%4d]",(perid==NULL?"":perid),jobno);
				addAPPlib(errorlist,tmp);
				if(tmp!=NULL) Rfree(tmp);
			}
		} else {
			FINDFLDGETSTRING(payjmst,"POSITION IDENTIFICATION",&posid);
			ZERNRD(posacc);
			FINDFLDSETSTRING(posacc,"POSITION IDENTIFICATION",posid);
			keyno=KEYNUMBER(posacc,"POSACC KEY");
			if(keyno<1) keyno=1;
			ef=ADVGTENRDsec(posacc,keyno,SCRNvirtualSubData,r);
			while(!ef)
			{
				FINDFLDGETSTRING(posacc,"POSITION IDENTIFICATION",&lposid);
				if(RDAstrcmp(posid,lposid)) break;
				FINDFLDGETCHAR(posacc,"DELETEFLAG",&deleteflag);
				if(!deleteflag)
				{
					FINDFLDGETINT(posacc,"ACCOUNT NUMBER",&accno);
					FINDFLDGETSHORT(posacc,"DEFINITION TYPE",&deftype);
					FINDFLDGETSTRING(posacc,"ACCOUNT CODE",&acctcode);
					FINDFLDGETSTRING(posacc,"DEBIT CODE",&debit);
					FINDFLDGETSTRING(posacc,"CREDIT CODE",&credit);
					FINDFLDGETSHORT(posacc,"PAYROLL FUND DEFINITION TYPE",&pdeftype);
					FINDFLDGETSTRING(posacc,"PAYABLE CODE",&payable);
					FINDFLDGETSTRING(posacc,"CASH CODE",&cash);
					FINDFLDGETSTRING(posacc,"DUE FROM",&due_from);
					if(EncumberPayroll) FINDFLDGETCHAR(posacc,"DISENCUMBER",&disenc);
						else disenc=FALSE;
					if(disenc)
					{
						FINDFLDGETSTRING(posacc,"ENCUMBRANCE CODE",&enc_code);
						FINDFLDGETSTRING(posacc,"RESERVE CODE",&res_code);
					} else {
						if(enc_code!=NULL) Rfree(enc_code);
						if(res_code!=NULL) Rfree(res_code);
					}
					FINDFLDGETCHAR(posacc,"PRORATE",&prorate);
					FINDFLDGETDOUBLE(posacc,"PERCENTAGE",&perc);
					FINDFLDGETDOUBLE(posacc,"AMOUNT",&amt);
					AddAccountStruct(mts,accno,deftype,acctcode,debit,credit,pdeftype,payable,cash,due_from,enc_code,res_code,prorate,perc,amt,disenc,FALSE);
				}
				ef=ADVNXTNRDsec(posacc,keyno,SCRNvirtualSubData,r);
			}
			if(mts->num<1)
			{
				tmp=Rmalloc(256);
				sprintf(tmp,"MISSING POSITION ACCOUNT(s)!: Personnel ID [%-15s], Job Number [%4d], Position ID [%-30s]",(perid==NULL?"":perid),jobno,(posid==NULL?"":posid));
				addAPPlib(errorlist,tmp);
				if(tmp!=NULL) Rfree(tmp);
			}
		}
/*
	} else if(diagapps)
	{
		prterr("Skipped: Personnel Job Master record of Personnel Identification of [%s] and Job Number of [%d] because of Non-Active.",(perid==NULL?"":perid),jobno);
	}
*/
	} else if(diagapps)
	{
		prterr("Skipped: Personnel Job Master record of Personnel Identification of [%s] and Job Number of [%d] because of Deleted.",(perid==NULL?"":perid),jobno);
	}
	if(acctcode!=NULL) Rfree(acctcode);
	if(debit!=NULL) Rfree(debit);
	if(enc_code!=NULL) Rfree(enc_code);
	if(res_code!=NULL) Rfree(res_code);
	if(credit!=NULL) Rfree(credit);
	if(lperid!=NULL) Rfree(lperid);
	if(payable!=NULL) Rfree(payable);
	if(cash!=NULL) Rfree(cash);
	if(due_from!=NULL) Rfree(due_from);
	if(lposid!=NULL) Rfree(lposid);
	if(posid!=NULL) Rfree(posid);
	if(mts->num<1) 
	{
		FreePAYMTS(mts);
		mts=NULL;
	}
	if(Use_Rate_Accounts==TRUE && block_racc==FALSE)
	{
		if(mts!=NULL)
		{
			blend=BlendedRates(mts,perid,jobno,payid,jobgross,r,fiscal_year);
			blend->roll_over_budget=mts->roll_over_budget;
			FreePAYMTS(mts);
			mts=blend;
		} 
	}
	if(diagapps)
	{
		SeePAYMTS(mts);TRACE;
	}
	return(mts);
}
struct DeductionStructs
{
	char *dedid;
	char *dedtype;
	char employee;
	char expensed;
	char emp_disenc;
	char disenc;
	char liability;
	double gross;
	double amount;
	char use_jdac;
	char roll_over_budget;
	int num;
	AccountStruct *a;
};
typedef struct DeductionStructs DeductionStruct;

struct JobStructs
{
	int num;
	DeductionStruct *d;
};
typedef struct JobStructs JobStruct;

static JobStruct *JobStructNEW()
{
	JobStruct *tmp=NULL;

	tmp=Rmalloc(sizeof(JobStruct));
	tmp->num=0;
	tmp->d=NULL;
	return(tmp);
}
static void FreeJobStruct(JobStruct *j)
{
	int x,y;
	DeductionStruct *d=NULL;
	AccountStruct *a=NULL;

	if(j!=NULL)
	{
		if(j->d!=NULL)
		{
			for(x=0,d=j->d;x<j->num;++x,++d)
			{
				if(d->dedid!=NULL) Rfree(d->dedid);
				if(d->dedtype!=NULL) Rfree(d->dedtype);
				if(d->a!=NULL) 
				{
					for(y=0,a=d->a;y<d->num;++y,++a)
					{
						if(a->account!=NULL) Rfree(a->account);
						if(a->debit!=NULL) Rfree(a->debit);
						if(a->credit!=NULL) Rfree(a->credit);
						if(a->payable!=NULL) Rfree(a->payable);
						if(a->cash!=NULL) Rfree(a->cash);
						if(a->due_from!=NULL) Rfree(a->due_from);
						if(a->enc_code!=NULL) Rfree(a->enc_code);
						if(a->res_code!=NULL) Rfree(a->res_code);
					}
					Rfree(d->a);
				}
			}
			Rfree(j->d);
		}
		Rfree(j);
		j=NULL;
	}
}
static void AddDeductionAccountStruct(DeductionStruct *d,int accno,
	short dtype,char *acc,char *deb,char *cred,short pdef,
	char *payable,char *cash,char *due_from,char *enc_code,
	char *res_code,char prorate,double percentage,double amount,char disenc)
{
	AccountStruct *a;

	if(d->a!=NULL)
	{
		d->a=Rrealloc(d->a,(d->num+1)*sizeof(AccountStruct));
	} else {
		d->a=Rmalloc(sizeof(AccountStruct));
		d->num=0;
	}
	a=d->a+d->num;
	a->accno=accno;
	a->deftype=dtype;
	a->disenc=disenc;
	if(!isEMPTY(acc)) a->account=stralloc(acc);
		else a->account=NULL;
	if(!isEMPTY(deb)) a->debit=stralloc(deb);
		else a->debit=NULL;
	if(!isEMPTY(cred)) a->credit=stralloc(cred);
		else a->credit=NULL;
	a->pdeftype=pdef;
	if(!isEMPTY(payable)) a->payable=stralloc(payable);
		else a->payable=NULL;
	if(!isEMPTY(cash)) a->cash=stralloc(cash);
		else a->cash=NULL;
	if(RDAstrcmp(XPERT_SETUP->zip,"62702-5030"))
	{
		if(!isEMPTY(due_from)) a->due_from=stralloc(due_from);
			else a->due_from=NULL;
	} else {
		memset(stemp,0,101);
		sprintf(stemp,"09-0000-113%.2s",a->account);
		a->due_from=stralloc(stemp);
	}
	if(!isEMPTY(enc_code)) a->enc_code=stralloc(enc_code);
		else a->enc_code=NULL;
	if(!isEMPTY(res_code)) a->res_code=stralloc(res_code);
		else a->res_code=NULL;
	a->prorate=prorate;
	a->percentage=percentage;
	a->amount=amount;
	if(diagapps)
	{
		SeeAccountStruct(a);TRACE;
	}
	++d->num;
}
void AddDeductionStruct(RDArsrc *r,JobStruct *j,char *perid,int jobno,
	char *dedid,char employee,char expensed,char empdisenc,
	char disenc,char liability,char use_jdac,
	char roll_over_budget,double gross,double amount,char *dedtype)
{
	DeductionStruct *d=NULL;
	char *acctcode=NULL,*debit=NULL,*enc_code=NULL;
	char *res_code=NULL,*credit=NULL,*payable=NULL;
	char *cash=NULL,*due_from=NULL,*lperid=NULL,*ldedid=NULL;
	char deleteflag=FALSE,prorate=FALSE;
	short ef=FALSE,deftype=0,pdeftype=0,keyno=(-1);
	int ljobno=0,accno=0;
	double amt=0.0,perc=0.0;

	if(j->d!=NULL)
	{
		j->d=Rrealloc(j->d,(j->num+1)*sizeof(DeductionStruct));
	} else {
		j->d=Rmalloc(sizeof(DeductionStruct));
		j->num=0;
	}
	d=j->d+j->num;
	if(!isEMPTY(dedid)) d->dedid=stralloc(dedid);
		else d->dedid=NULL;
	d->employee=employee;
	d->expensed=expensed;
	d->disenc=disenc;
	d->dedtype=stralloc(dedtype);
	d->emp_disenc=empdisenc;
	d->liability=liability;
	d->roll_over_budget=roll_over_budget;
	d->use_jdac=use_jdac;
	d->gross=gross;
	d->amount=amount;
	d->a=NULL;
/* SJS NEW CODE SEGMENT BEGIN */
	if(use_jdac==TRUE)
	{
		ZERNRD(payjdac);
		FINDFLDSETSTRING(payjdac,"PERSONNEL IDENTIFICATION",perid);
		FINDFLDSETINT(payjdac,"JOB NUMBER",jobno);
		FINDFLDSETSTRING(payjdac,"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
		keyno=KEYNUMBER(payjdac,"PAYJDAC KEY");
		if(keyno<1) keyno=1;
		ef=ADVGTENRDsec(payjdac,keyno,SCRNvirtualSubData,r);
		while(!ef)
		{
			FINDFLDGETSTRING(payjdac,"PERSONNEL IDENTIFICATION",&lperid);
			FINDFLDGETINT(payjdac,"JOB NUMBER",&ljobno);
			FINDFLDGETSTRING(payjdac,"DEDUCTION DESCRIPTION IDENTIFICATION",&ldedid);
			if(RDAstrcmp(perid,lperid) || jobno!=ljobno || 
				RDAstrcmp(dedid,ldedid)) break;
			FINDFLDGETCHAR(payjdac,"DELETEFLAG",&deleteflag);
			if(!deleteflag)
			{
				FINDFLDGETINT(payjdac,"ACCOUNT NUMBER",&accno);
				FINDFLDGETSHORT(payjdac,"DEFINITION TYPE",&deftype);
				FINDFLDGETSTRING(payjdac,"ACCOUNT CODE",&acctcode);
				FINDFLDGETSTRING(payjdac,"DEBIT CODE",&debit);
				FINDFLDGETSTRING(payjdac,"CREDIT CODE",&credit);

				ZERNRD(payddac);
				FINDFLDSETSTRING(payddac,"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
				FINDFLDSETSHORT(payddac,"DEFINITION TYPE",deftype);
				if(ADVEQLNRDsec(payddac,1,SCRNvirtualSubData,r)) 
				{
					KEYNRD(payddac,1);
					prterr("Error DEDUCTION ACCOUNT RECORD NOT FOUND....");
				}
				FINDFLDGETSHORT(payjdac,"PAYROLL FUND DEFINITION TYPE",&pdeftype);
				FINDFLDGETSTRING(payddac,"LIABILITY CODE",&payable);
				FINDFLDGETSTRING(payddac,"CASH CODE",&cash);
				FINDFLDGETSTRING(payddac,"DUE FROM",&due_from);
				if(EncumberPayroll) FINDFLDGETCHAR(payddac,"DISENCUMBER",&disenc);
						else disenc=FALSE;

				if(disenc)
				{
					FINDFLDGETSTRING(payjdac,"ENCUMBRANCE CODE",&enc_code);
					FINDFLDGETSTRING(payjdac,"RESERVE CODE",&res_code);
				} else {
					if(enc_code!=NULL) Rfree(enc_code);
					if(res_code!=NULL) Rfree(res_code);
				}
				FINDFLDGETCHAR(payjdac,"PRORATE",&prorate);
				FINDFLDGETDOUBLE(payjdac,"PERCENTAGE",&perc);
				FINDFLDGETDOUBLE(payjdac,"AMOUNT",&amt);
				AddDeductionAccountStruct(d,accno,deftype,acctcode,debit,credit,pdeftype,payable,cash,due_from,enc_code,res_code,prorate,perc,amt,disenc);
			}
			ef=ADVNXTNRDsec(payjdac,keyno,SCRNvirtualSubData,r);
		}
	} else {
		d->a=NULL;
	}
	if(acctcode!=NULL) Rfree(acctcode);
	if(debit!=NULL) Rfree(debit);
	if(enc_code!=NULL) Rfree(enc_code);
	if(res_code!=NULL) Rfree(res_code);
	if(credit!=NULL) Rfree(credit);
	if(lperid!=NULL) Rfree(lperid);
	if(payable!=NULL) Rfree(payable);
	if(cash!=NULL) Rfree(cash);
	if(due_from!=NULL) Rfree(due_from);
/* SJS NEW CODE SEGMENT END */
	++j->num;
}
static double ComputeNet(double gross,JobStruct *job)
{
	int x;
	DeductionStruct *d;

	if(job!=NULL)
	{
		if(job->d!=NULL)
		{
			for(x=0,d=job->d;x<job->num;++x,++d)
			{
				if(d->employee && (d->expensed || !RDAstrcmp(d->dedtype,"ADVANCE-REPAY"))) gross-=d->amount;
			}
		}
	} 
	return(gross);
}
void xSeeJobStruct(JobStruct *job,int line,char *file)
{
	DeductionStruct *d=NULL;
	int x;
	double total_amt=0.0,total_ded=0.0,total_emp=0.0;

	if(job!=NULL)
	{
		prterr("DIAG SeeJobStruct at line [%d] program [%s].",line,file);
		if(job->d!=NULL)
		{
			for(x=0,d=job->d;x<job->num;++x,++d)
			{
				prterr("\tDeduction [%s] Employee [%s] Expensed [%s] Gross [%.0f] Amount [%.0f].\n",d->dedid,(d->employee ? "True":"False"),(d->expensed ? "True":"False"),d->gross,d->amount);
				total_amt+=d->amount;
				if(d->employee && d->expensed) total_ded+=d->amount;
				if(!d->employee) total_emp+=d->amount;
			}
		}
		prterr("\tTotal Employee: %12.02f Total Employer: %12.02f Total All: %12.02f\n",total_ded,total_emp,total_amt);
	} else {
		prterr("DIAG SeeJobStruct is NULL at line [%d] program [%s].",line,
			file);
	}
}
static JobStruct *GetJobStruct(RDArsrc *r,char *perid,int jobno,char *payid)
{
	JobStruct *job=NULL;
	short ef=FALSE,keyno=0;
	int ljobno=0;
	char *lperid=NULL,*lpayid=NULL,employee=FALSE;
	char expensed=FALSE,*dedid=NULL;
	char empdisenc=FALSE,disenc=FALSE,deleteflag=FALSE;
	char liability=FALSE,suv=FALSE,use_jdac=FALSE;
	double gross=0.0,amount=0.0;
	char roll_over_budget=FALSE,*dedtype=NULL;

	job=JobStructNEW();
	ZERNRD(payjmst);
	FINDFLDSETSTRING(payjmst,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(payjmst,"JOB NUMBER",jobno);
	if(ADVEQLNRDsec(payjmst,1,SCRNvirtualSubData,r)) KEYNRD(payjmst,1);
	FINDFLDGETCHAR(payjmst,"DELETEFLAG",&deleteflag);
	ZERNRD(payjdpm);
	keyno=KEYNUMBER(payjdpm,"RDA PAYJDPM KEY");
	if(keyno<1) keyno=1;
	FINDFLDSETSTRING(payjdpm,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(payjdpm,"JOB NUMBER",jobno);
	FINDFLDSETSTRING(payjdpm,"PAY IDENTIFICATION",payid);
	ef=ADVGTENRDsec(payjdpm,keyno,SCRNvirtualSubData,r);
	while(!ef)
	{
		FINDFLDGETSTRING(payjdpm,"PERSONNEL IDENTIFICATION",&lperid);
		FINDFLDGETINT(payjdpm,"JOB NUMBER",&ljobno);
		FINDFLDGETSTRING(payjdpm,"PAY IDENTIFICATION",&lpayid);
		if(RDAstrcmp(perid,lperid) || jobno!=ljobno || RDAstrcmp(payid,lpayid)) break;
		FINDFLDGETCHAR(payjdpm,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDGETSTRING(payjdpm,"DEDUCTION DESCRIPTION IDENTIFICATION",&dedid);
			ZERNRD(paydedm);
			FINDFLDSETSTRING(paydedm,"PERSONNEL IDENTIFICATION",lperid);
			FINDFLDSETSTRING(paydedm,"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
			if(ADVEQLNRDsec(paydedm,1,SCRNvirtualSubData,r)) 
			{
				KEYNRD(paydedm,1);
				prterr("Error DEDUCTION MASTER NOT FOUND (PAYDEDM) for the Personnel Identificatino of [%s] and the Deduction Description Identification of [%s]",(lperid!=NULL?lperid:""),(dedid!=NULL?dedid:""));
			}
			FINDFLDSETSTRING(paydpms,"PERSONNEL IDENTIFICATION",lperid);
			FINDFLDSETSTRING(paydpms,"PAY IDENTIFICATION",lpayid);
			FINDFLDSETSTRING(paydpms,"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
			if(ADVEQLNRDsec(paydpms,1,SCRNvirtualSubData,r)) KEYNRD(paydpms,1);
			FINDFLDGETCHAR(paydpms,"SOURCE USER VERIFIED",&suv);
			if(suv)
			{
				FINDFLDGETDOUBLE(payjdpm,"AMOUNT",&amount);
				if(amount!=0.0)
				{
					FINDFLDSETSTRING(payddms,"DEDUCTION DESCRIPTION IDENTIFICATION",dedid);
					if(ADVEQLNRDsec(payddms,1,SCRNvirtualSubData,r)) KEYNRD(payddms,1);
					FINDFLDGETSTRING(payddms,"DEDUCTION TYPE",&dedtype);
					FINDFLDGETCHAR(payjmst,"ROLL OVER DEDUCTION BUDGET",&roll_over_budget);
					FINDFLDGETCHAR(payjmst,"JOB DEDUCTION ACCOUNTS",&use_jdac);
					if(EncumberPayroll) FINDFLDGETCHAR(paydedm,"DISENCUMBER",&empdisenc);
						else empdisenc=FALSE;
					FINDFLDGETCHAR(payddms,"EXPENSED",&expensed);
					FINDFLDGETCHAR(payddms,"LIABILITY",&liability);
					FINDFLDGETCHAR(payddms,"REDUCE NET",&employee);
					if(EncumberPayroll) FINDFLDGETCHAR(payddms,"DISENCUMBER",&disenc);
						else disenc=FALSE;
					FINDFLDGETDOUBLE(payjdpm,"GROSS",&gross);
					AddDeductionStruct(r,job,perid,jobno,dedid,employee,expensed,empdisenc,disenc,liability,use_jdac,roll_over_budget,gross,amount,dedtype);
				} else {
					if(diagapps)
					{
						prterr("DIAG Skipped Adding Deduction to DeductionStructure of Pay Identification=[%s], Personnel Identification=[%s], Job Number=[%d], Deduction=[%s] because of because of AMOUNT=0.0 in PAYJDPM (Job Deduction Pay Master).",(payid==NULL?"":payid),(perid==NULL?"":perid),jobno,(dedid==NULL?"":dedid));
					}
				}
			} else {
				if(diagapps)
				{
					prterr("DIAG Skipped Adding Deduction to DeductionStructure of Pay Identification=[%s], Personnel Identification=[%s], Deduction=[%s] because of because of SOURCE USER VERIFIED=FALSE in PAYDPMS (Deduction Pay Master).",(payid==NULL?"":payid),(perid==NULL?"":perid),(dedid==NULL?"":dedid));
				}
			}
		} else {
			if(diagapps)
			{
				prterr("DIAG Skipped Adding Deduction to DeductionStructure of Pay Identification=[%s], Personnel Identification=[%s], Job Number=[%d], Deduction=[%s] because of because of DELETEFLAG=TRUE in PAYJDPM (Job Deduction Pay Master).",(payid==NULL?"":payid),(perid==NULL?"":perid),jobno,(dedid==NULL?"":dedid));
			}
		}
		ef=ADVNXTNRDsec(payjdpm,keyno,SCRNvirtualSubData,r);
	}
	if(lpayid!=NULL) Rfree(lpayid);
	if(lperid!=NULL) Rfree(lperid);
	if(dedid!=NULL) Rfree(dedid);
	if(dedtype!=NULL) Rfree(dedtype);
	if(job->num<1) 
	{
		FreeJobStruct(job);
		job=NULL;
	}
	if(diagapps)
	{
		SeeJobStruct(job);TRACE;
	}
	return(job);
}
static void okbpaymstr(MakeBrowseList *b,void *targetkey)
{
	RDArsrc *diagrsrc=NULL;
	short pmonth=0,ef=FALSE,jef=FALSE,upd_diag=FALSE,keyno=0,jkeyno=0;
	int jobno=0,x,y,fiscal_year=0;
	char deleteflag=FALSE,*perid=NULL,*lastperid=NULL;
	char made_trans=FALSE,locked=TRUE;
	char *payid=NULL,*payid1=NULL,*lastpayid=NULL,*curdate=NULL,*curtime=NULL;
	char *curdate10=NULL;
	char *acc_image=NULL,*deb_image=NULL,*cred_image=NULL,*temp=NULL,*e=NULL;
	char *liab_code=NULL,*cash_code=NULL,*due_from=NULL;
	char *enc_image=NULL,*res_image=NULL;
	char suv=FALSE;
	char *errorstr=NULL,*errorstr1=NULL,*errorstr2=NULL,*warnstr=NULL;
	char *acc_merge=NULL,*deb_merge=NULL,*cred_merge=NULL,*tmp=NULL;
	char *enc_merge=NULL,*res_merge=NULL,*liab_merge=NULL,*cash_merge=NULL,*duefrom_merge=NULL;
	char compute_completed=FALSE,benefits_computed=FALSE;
	short pdeftype=0;
	double percentage=0.0,total_percentage=0.0,amount=0.0,total_amount=0.0;
	double gross=0.0,units=0.0,jobgross=0.0,total_jobgross=0.0,tamt=0.0;
	double zamt=0.0,a_total=0.0,t=0.0;
	PAYMTS *pmts=NULL;
	AccountStruct *acc=NULL;
	JobStruct *job=NULL;
	DeductionStruct *d=NULL;
	RDATData *prev_file=NULL,*prev=NULL;
	NRDfield *field=NULL;
	char *tgstr=NULL,*tbstr=NULL,*tpstr=NULL,*temp1=NULL,*tdstr=NULL;
	char *tastr=NULL;
	double tg=0.0,tb=0.0,tp=0.0,td=0.0,ta=0.0;

	printed_errorlist=FALSE;
	printed_warninglist=FALSE;
	did_payunmts=FALSE;
	did_findit=FALSE;
	FINDRSCSETSENSITIVE(b->mainrsrc,"ERROR LIST",TRUE);
	FINDRSCSETSENSITIVE(b->mainrsrc,"WARNING LIST",TRUE);
	FINDRSCSETEDITABLE(b->mainrsrc,"PAYUNMTS",FALSE);
	FINDRSCSETSENSITIVE(b->mainrsrc,"PAYUNMTS",FALSE);
	FINDRSCSETEDITABLE(b->mainrsrc,"FINDIT",FALSE);
	FINDRSCSETSENSITIVE(b->mainrsrc,"FINDIT",FALSE);
	updatersrc(b->mainrsrc,"PAYUNMTS");
	updatersrc(b->mainrsrc,"FINDIT");
	diagrsrc=diagscrn(payjpms,"DIAGNOSTIC SCREEN",module,
		"Making Payroll Transactions.....",NULL);
	addDFincvir(diagrsrc,module,"PAYMSTR",NULL,paymstr);
	addDFincvir(diagrsrc,module,"PAYPPMS",NULL,payppms);
	addDFincvir(diagrsrc,module,"PAYJPMS",NULL,payjpms);
	addDFincvir(diagrsrc,module,"PAYJAMS",NULL,payjams);
	addDFincvir(diagrsrc,module,"PAYDPMS",NULL,paydpms);
	addDFincvir(diagrsrc,module,"PAYDDAC",NULL,payddac);
	addDFincvir(diagrsrc,module,"PAYJDAC",NULL,payjdac);
	addDFincvir(diagrsrc,module,"PAYJMST",NULL,payjmst);
	addDFincvir(diagrsrc,module,"PAYDDMS",NULL,payddms);
	addDFincvir(diagrsrc,module,"PAYJTRN",NULL,payjtrn);
	addDFincvir(diagrsrc,module,"PAYJDPM",NULL,payjdpm);
	addDFincvir(diagrsrc,module,"PAYDEDM",NULL,paydedm);
	addDFincvir(diagrsrc,module,"PAYDTRN",NULL,paydtrn);
	addDFincvir(diagrsrc,"POSTRK","POSACC",NULL,posacc);
	addDFincvir(diagrsrc,"FINMGT","FINEYR",NULL,eyrnum);
	addDFincvir(diagrsrc,"FINMGT","FINEACC",NULL,eaccnum);
	addDFincvir(diagrsrc,"FINMGT","FINBYR",NULL,byrnum);
	addDFincvir(diagrsrc,"FINMGT","FINBACC",NULL,baccnum);
	GET_ALL_SCREEN_VIRTUALS(diagrsrc,0);
	ZERNRD(paymstr);
	if(ADVEQLNRDKEYsec(paymstr,1,targetkey,SCRNvirtualSubData,diagrsrc)) KEYNRD(paymstr,1);
	FINDFLDGETSTRING(paymstr,"PAY IDENTIFICATION",&payid);
	FINDFLDGETINT(paymstr,"FISCAL YEAR",&fiscal_year);
/*
	FINDFLDGETSHORT(paymstr,"MONTH",&pmonth);
*/
	FINDFLDGETSHORT(paymstr,"FISCAL MONTH",&pmonth);
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
	keyno=KEYNUMBER(payppms,"RDA PAYPPMS KEY");
	if(keyno<1) keyno=1;
	ZERNRD(payppms);
	FINDFLDSETSTRING(payppms,"PAY IDENTIFICATION",payid);
	ef=ADVGTENRDsec(payppms,keyno,SCRNvirtualSubData,diagrsrc);
	if(errorlist==NULL)
	{
		errorlist=APPlibNEW();
	} else {
		if(!RDAstrcmp(errorlist->libs[0],"No Errors Detected") && errorlist->numlibs==1)
		{		
			freeapplib(errorlist);
			errorlist=APPlibNEW();
		}
	}
	if(warnlist==NULL)
	{
		warnlist=APPlibNEW();
	} else {
		if(!RDAstrcmp(warnlist->libs[0],"No Warnings Detected") && warnlist->numlibs==1)
		{		
			freeapplib(warnlist);
			warnlist=APPlibNEW();
		}
	}
	while(!ef)
	{
	upd_diag=FALSE;
	FINDFLDGETSTRING(payppms,"PAY IDENTIFICATION",&payid1);
	if(RDAstrcmp(payid,payid1)) break;
	FINDFLDGETCHAR(payppms,"DELETEFLAG",&deleteflag);
	FINDFLDGETSTRING(payppms,"PERSONNEL IDENTIFICATION",&perid);
	if(!deleteflag)
	{
	FINDFLDGETCHAR(payppms,"MADE TRANSACTIONS",&made_trans);
	FINDFLDGETCHAR(payppms,"COMPUTE COMPLETED",&compute_completed);
	FINDFLDGETCHAR(payppms,"BENEFITS COMPUTED",&benefits_computed);
	if(!benefits_computed)
	{
		tmp=Rmalloc(456);
		sprintf(tmp,"BENEFITS NOT COMPUTED!: Personnel ID [%-15s], Pay Identification [%-15s]  Benefits have not been computed for this employee.",(perid==NULL?"":perid),(payid1==NULL?"":payid1));
		addAPPlib(errorlist,tmp);
		if(tmp!=NULL) Rfree(tmp);
	} else if(!compute_completed && Use_Compute_Completed)
	{
		tmp=Rmalloc(456);
		if(PAYROLL_SETUP->paynet_audit==TRUE)
		{
		sprintf(tmp,"COMPUTE NOT COMPLETED!: Personnel ID [%-15s], Pay Identification [%-15s]  Compute Audit Trail and Deduction Register must be completed.",(perid==NULL?"":perid),(payid1==NULL?"":payid1));
		} else {
		sprintf(tmp,"COMPUTE NOT COMPLETED!: Personnel ID [%-15s], Pay Identification [%-15s]  Deduction Register must be completed.",(perid==NULL?"":perid),(payid1==NULL?"":payid1));
		}
		addAPPlib(errorlist,tmp);
		if(tmp!=NULL) Rfree(tmp);
	} else if(!made_trans && benefits_computed && (compute_completed || !Use_Compute_Completed))
	{
		jkeyno=KEYNUMBER(payjpms,"RDA PAYJPMS KEY");
		if(jkeyno<1) jkeyno=1;
		ZERNRD(payjpms);
		FINDFLDSETSTRING(payjpms,"PAY IDENTIFICATION",payid);
		FINDFLDSETSTRING(payjpms,"PERSONNEL IDENTIFICATION",perid);
		jef=ADVGTENRDsec(payjpms,jkeyno,SCRNvirtualSubData,diagrsrc);
		while(!jef)
		{
			FINDFLDGETSTRING(payjpms,"PAY IDENTIFICATION",&lastpayid);
			FINDFLDGETSTRING(payjpms,"PERSONNEL IDENTIFICATION",&lastperid);
			if(RDAstrcmp(payid,lastpayid) || RDAstrcmp(perid,lastperid)) break;
			FINDFLDGETCHAR(payjpms,"DELETEFLAG",&deleteflag);
			FINDFLDGETCHAR(payjpms,"SOURCE USER VERIFIED",&suv);
			FINDFLDGETDOUBLE(payjpms,"GROSS",&jobgross);
			a_total=jobgross;
			FINDFLDGETDOUBLE(payjpms,"UNITS",&units);
			if(!deleteflag && suv)
			{
				curdate=GETCURRENTDATE();
				curdate10=GETCURRENTDATE10();
				FINDFLDGETINT(payjpms,"JOB NUMBER",&jobno);
				if(diagapps)
				{
					prterr("DIAG Personnel ID: [%s] Job Number [%d] PayID [%s].",(perid!=NULL ? perid:""),jobno,(payid!=NULL ? payid:""));
				}
				job=GetJobStruct(diagrsrc,perid,jobno,payid);
				pmts=GetPAYMTS(diagrsrc,perid,jobno,payid,jobgross,fiscal_year);
				if(pmts!=NULL)
				{
					gross=ComputeNet(jobgross,job);
					total_amount=0;total_percentage=0;
					for(x=0,acc=pmts->a;x<pmts->num;++x,++acc)
					{
						if(total_amount>=gross && gross>0.0) 
						{
							break;
						}
						if((x+1)<pmts->num)
						{
							if(acc->prorate && acc->skip_deductions==FALSE)
							{
								if(diagapps)
								{
								prterr("acc->amount [%f] amount [%f] a_total [%f] total_amount [%f] gross [%f] x [%d] pmts->num [%d] Skip Deds [%s] ",acc->amount,amount,a_total,total_amount,gross,x,pmts->num,(acc->skip_deductions ? "True":"False"));TRACE;
								}
								if((total_percentage+acc->percentage)>100.0)
								{
									percentage=100.0-total_percentage;
								} else percentage=acc->percentage;
								amount=round(gross*percentage/100);
								total_percentage+=percentage;
							} else if(acc->prorate && acc->skip_deductions==TRUE)
							{
								if(diagapps)
								{
								prterr("acc->amount [%f] amount [%f] a_total [%f] total_amount [%f] gross [%f] x [%d] pmts->num [%d] Skip Deds [%s] ",acc->amount,amount,a_total,total_amount,gross,x,pmts->num,(acc->skip_deductions ? "True":"False"));TRACE;
								}
								amount=acc->amount;
								percentage=round(amount/gross)*100;
								total_percentage+=percentage;
							} else {
								if(diagapps)
								{
/*
									prterr("acc->amount [%f] amount [%f] a_total [%f] total_amount [%f] gross [%f] x [%d] pmts->num [%d] Skip Deds [%s] ",acc->amount,amount,a_total,total_amount,gross,x,pmts->num,(acc->skip_deductions ? "True":"False"));TRACE;
*/
								}
								if(acc->amount<0.0) 
								{
									amount=acc->amount;
								} else if(acc->amount<(gross-total_amount) && acc->amount!=0.0 && ((x+1)==pmts->num)) 
								{
									amount=acc->amount;
								} else if(((acc->amount>=(round(acc->amount/a_total*gross))) && acc->amount!=0.0 && ((x+1)<pmts->num)))
								{
									amount=round(acc->amount/a_total*gross);
								} else {
									amount=gross-total_amount;
									/* this one is getting everything when the net exceeds the gross */
								}
								if(gross!=0.0) 
									total_percentage+=(amount/gross)*100;
							}
							if(PAYROLL_SETUP->payjams_rollover && pmts->roll_over_budget)
							{
								ZERNRD(eyrnum);
								FINDFLDSETSTRING(eyrnum,"ACCOUNT CODE",acc->account);
								FINDFLDSETSHORT(eyrnum,"DEFINITION TYPE",acc->deftype);
								FINDFLDSETINT(eyrnum,"FISCAL YEAR",fiscal_year);
								if(ADVEQLNRDsec(eyrnum,1,SCRNvirtualSubData,diagrsrc))
								{
									KEYNRD(eyrnum,1);
								}
								zamt=ROLL_OVER_TEST(eyrnum,amount,NULL);
								if(zamt!=(-1))
								{
									amount=zamt;
								}
							}
							total_amount+=amount;
						} else {
							if(gross>total_amount && gross>=0.0)
							{
								amount=gross-total_amount;
							} else if(gross<0)
							{
								amount=gross-total_amount;
							} else amount=0;
							total_amount+=amount;
						}
						ZERNRD(payjtrn);
						FINDFLDSETSTRING(payjtrn,"PERSONNEL IDENTIFICATION",perid);
						FINDFLDSETINT(payjtrn,"JOB NUMBER",jobno);
						FINDFLDSETSTRING(payjtrn,"PAY IDENTIFICATION",payid);
						FINDFLDSETINT(payjtrn,"ACCOUNT NUMBER",acc->accno);
						LOCNRDFILE(payjtrn);
						if(!ADVEQLNRDsec(payjtrn,1,SCRNvirtualSubData,diagrsrc)) prev=RDATDataNEW(payjtrn);
						else prev=NULL;
						FINDFLDSETDOUBLE(payjtrn,"AMOUNT",amount);
						deleteflag=FALSE;
						FINDFLDSETCHAR(payjtrn,"DELETEFLAG",deleteflag);
						FINDFLDSETSHORT(payjtrn,"DEFINITION TYPE",acc->deftype);
						FINDFLDSETSTRING(payjtrn,"ACCOUNT CODE",acc->account);
						FINDFLDSETSTRING(payjtrn,"DEBIT CODE",acc->debit);
						FINDFLDSETSTRING(payjtrn,"CREDIT CODE",acc->credit);
						FINDFLDSETSHORT(payjtrn,"PAYROLL FUND DEFINITION TYPE",acc->pdeftype);
						FINDFLDSETSTRING(payjtrn,"PAYABLE CODE",acc->payable);
						FINDFLDSETSTRING(payjtrn,"CASH CODE",acc->cash);
						FINDFLDSETSTRING(payjtrn,"DUE FROM",acc->due_from);
						if(EncumberPayroll) FINDFLDSETCHAR(payjtrn,"DISENCUMBER",acc->disenc);
							else acc->disenc=FALSE;
						if(acc->disenc)
						{
							FINDFLDSETSTRING(payjtrn,"ENCUMBRANCE CODE",acc->enc_code);
							FINDFLDSETSTRING(payjtrn,"RESERVE CODE",acc->res_code);
						}
						FINDFLDSETSTRING(payjtrn,"SOURCE USER",USERLOGIN);
						field=FLDNRD(payjtrn,"ENTRY DATE");
						if(field!=NULL)
						{
						if(field->len==8)
						{
						FINDFLDSETSTRING(payjtrn,"ENTRY DATE",curdate);
						} else FINDFLDSETSTRING(payjtrn,"ENTRY DATE",curdate10);
						}
						curtime=GETCURRENTTIME();
						FINDFLDSETSTRING(payjtrn,"ENTRY TIME",curtime);
						if(curtime!=NULL) Rfree(curtime);
						FINDFLDSETSHORT(payjtrn,"USE PAYROLL FUND TYPE",PAYROLL_SETUP->use_payroll_fund);
						if(PAYROLL_SETUP->paymts_audit)
						{
							FINDFLDSETCHAR(payjtrn,"SOURCE USER VERIFIED",FALSE);
						} else {
							FINDFLDSETCHAR(payjtrn,"SOURCE USER VERIFIED",TRUE);
						}
						if(Use_Rate_Accounts==FALSE)
						{
							ADVWRTTRANSsec(payjtrn,0,NULL,prev,SCRNvirtualSubData,diagrsrc);
						} else {
							ADVWRTTRANSsec(payjtrn,TRUE,"ACCOUNT NUMBER",prev,SCRNvirtualSubData,diagrsrc);
						}
						DO_AUDIT=TRUE;
						if(amount!=0)
						{
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
						if(XPERT_SETUP->OrgType!=4)
						{
						errorstr=ADVCHECKACCT2(eyrnum,eaccnum,acc->account,EXP_ACCT,acc->deftype,fiscal_year,1,SCRNvirtualSubData,diagrsrc);
						if(!isEMPTY(errorstr))
						{ 
							tmp=Rmalloc(RDAstrlen(errorstr)+RDAstrlen(perid)+280);
							sprintf(tmp,"CANNOT UPDATE EXPENSE TO ACCOUNT CODE : Personnel ID [%-15s], Job Number [%4d], Account Number [%3d] Reason: %s",(perid==NULL?"":perid),jobno,acc->accno,errorstr);
							addAPPlib(errorlist,tmp);
							if(tmp!=NULL) Rfree(tmp);
						}
						}
						if(errorstr!=NULL) Rfree(errorstr);
						errorstr1=ADVCHECKACCT2(byrnum,baccnum,acc->debit,BAL_ACCT,acc->deftype,fiscal_year,1,SCRNvirtualSubData,diagrsrc);
						if(!isEMPTY(errorstr1))
						{
							tmp=Rmalloc(RDAstrlen(errorstr1)+RDAstrlen(perid)+280);
							sprintf(tmp,"CANNOT UPDATE DEBIT TO DEBIT CODE     : Personnel ID [%-15s], Job Number [%4d], Account Number [%3d] Reason: %s",(perid==NULL?"":perid),jobno,acc->accno,errorstr1);
							addAPPlib(errorlist,tmp);
							if(tmp!=NULL) Rfree(tmp);
						}
						if(errorstr1!=NULL) Rfree(errorstr1);
						errorstr2=ADVCHECKACCT2(byrnum,baccnum,acc->credit,BAL_ACCT,acc->deftype,fiscal_year,1,SCRNvirtualSubData,diagrsrc);
						if(!isEMPTY(errorstr2))
						{
							tmp=Rmalloc(RDAstrlen(errorstr2)+RDAstrlen(perid)+280);
							sprintf(tmp,"CANNOT UPDATE CREDIT TO CREDIT CODE   : Personnel ID [%-15s], Job Number [%4d], Account Number [%3d] Reason: %s",(perid==NULL?"":perid),jobno,acc->accno,errorstr2);
							addAPPlib(errorlist,tmp);
							if(tmp!=NULL) Rfree(tmp);
						}
						if(errorstr2!=NULL) Rfree(errorstr2);
						if(acc->disenc)
						{
						errorstr2=ADVCHECKACCT2(byrnum,baccnum,acc->enc_code,BAL_ACCT,acc->deftype,fiscal_year,1,SCRNvirtualSubData,diagrsrc);
						if(!isEMPTY(errorstr2))
						{
							tmp=Rmalloc(RDAstrlen(errorstr2)+RDAstrlen(perid)+280);
							sprintf(tmp,"CANNOT UPDATE DEBIT TO ENCUMBRANCE CODE   : Personnel ID [%-15s], Job Number [%4d], Account Number [%3d] Reason: %s",(perid==NULL?"":perid),jobno,acc->accno,errorstr2);
							addAPPlib(errorlist,tmp);
							if(tmp!=NULL) Rfree(tmp);
						}
						if(errorstr2!=NULL) Rfree(errorstr2);
						errorstr2=ADVCHECKACCT2(byrnum,baccnum,acc->res_code,BAL_ACCT,acc->deftype,fiscal_year,1,SCRNvirtualSubData,diagrsrc);
						if(!isEMPTY(errorstr2))
						{
							tmp=Rmalloc(RDAstrlen(errorstr2)+RDAstrlen(perid)+280);
							sprintf(tmp,"CANNOT UPDATE CREDIT TO RESERVE CODE   : Personnel ID [%-15s], Job Number [%4d], Account Number [%3d] Reason: %s",(perid==NULL?"":perid),jobno,acc->accno,errorstr2);
							addAPPlib(errorlist,tmp);
							if(tmp!=NULL) Rfree(tmp);
						}
						if(errorstr2!=NULL) Rfree(errorstr2);
						if(XPERT_SETUP->OrgType!=4)
						{
							ADVupdateundist(payjtrn,"ACCOUNT CODE","ENCUMBRANCE CODE","RESERVE CODE",NULL,eyrnum,byrnum,fiscal_year,acc->deftype,"ENCUMBRANCES",FISCALPERIOD[pmonth],-amount,SCRNvirtualSubData,diagrsrc);
						} else {
							ADVupdateundist(payjtrn,NULL,"ENCUMBRANCE CODE","RESERVE CODE",NULL,(-1),byrnum,fiscal_year,acc->deftype,"ENCUMBRANCES",FISCALPERIOD[pmonth],-amount,SCRNvirtualSubData,diagrsrc);
						}
						}
						if(finsetup->budget==ANNUALBUDGET && XPERT_SETUP->OrgType!=4)
						{
							if(!PAYROLL_SETUP->payjams_rollover || !pmts->roll_over_budget || 
								(PAYROLL_SETUP->payjams_rollover && pmts->roll_over_budget && (x+1)==pmts->num))
							{
								if(testapprotot(eyrnum,amount,NULL)==FALSE)
								{
									warnstr=Rmalloc(RDAstrlen(acc->account)+RDAstrlen(perid)+350);
									sprintf(warnstr,"INSUFFICIENT FUNDS IN AVAILABLE BALANCE: Personnel ID [%-15s], Job Number [%4d], Account Number [%3d], Account Code [%-40s], Year [%4d], Amount [$ %15.02f]",(perid==NULL?"":perid),jobno,acc->accno,(acc->account==NULL?"":acc->account),fiscal_year,amount/100);
									addAPPlib(warnlist,warnstr);
									if(warnstr!=NULL) Rfree(warnstr);
								}
							}
						} else if(XPERT_SETUP->OrgType!=4)
						{
							if(!PAYROLL_SETUP->payjams_rollover || !pmts->roll_over_budget || 
								(PAYROLL_SETUP->payjams_rollover && pmts->roll_over_budget && (x+1)==pmts->num))
							{
								if(testapprotot(eyrnum,amount,FISCALPERIOD[pmonth])==FALSE)
								{
									warnstr=Rmalloc(RDAstrlen(acc->account)+RDAstrlen(perid)+RDAstrlen(FISCALPERIOD[pmonth])+300);
									sprintf(warnstr,"INSUFFICIENT FUNDS IN AVAILABLE BALANCE: Personnel ID [%-15s], Job Number [%4d], Account Number [%3d], Account Code [%-40s], Year [%4d], Month [%9s], Amount [$ %15.02f]",(perid==NULL?"":perid),jobno,acc->accno,(acc->account==NULL?
"":acc->account),fiscal_year,(FISCALPERIOD[pmonth]==NULL?"":FISCALPERIOD[pmonth]),amount/100);
									addAPPlib(warnlist,warnstr);
									if(warnstr!=NULL) Rfree(warnstr);
								}
							}
						}
						if(XPERT_SETUP->OrgType!=4)
						{
							ADVupdateundist(payjtrn,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,eyrnum,byrnum,fiscal_year,acc->deftype,"EXPENDITURES",FISCALPERIOD[pmonth],amount,SCRNvirtualSubData,diagrsrc);
						} else {
							ADVupdateundist(payjtrn,NULL,"DEBIT CODE","CREDIT CODE",NULL,(-1),byrnum,fiscal_year,acc->deftype,"EXPENDITURES",FISCALPERIOD[pmonth],amount,SCRNvirtualSubData,diagrsrc);
						}
						if(PAYROLL_SETUP->use_payroll_fund==2)
						{
						errorstr1=ADVCHECKACCT2(byrnum,baccnum,acc->due_from,BAL_ACCT,acc->pdeftype,fiscal_year,1,SCRNvirtualSubData,diagrsrc);
						if(!isEMPTY(errorstr1))
						{
							tmp=Rmalloc(RDAstrlen(errorstr1)+RDAstrlen(perid)+280);
							sprintf(tmp,"CANNOT UPDATE DEBIT TO DUE FROM: Personnel ID [%-15s], Job Number [%4d], Account Number [%3d] Reason: %s",(perid==NULL?"":perid),jobno,acc->accno,errorstr1);
							addAPPlib(errorlist,tmp);
							if(tmp!=NULL) Rfree(tmp);
						}
						} else {
						if(errorstr1!=NULL) Rfree(errorstr1);
						errorstr1=ADVCHECKACCT2(byrnum,baccnum,acc->cash,BAL_ACCT,acc->pdeftype,fiscal_year,1,SCRNvirtualSubData,diagrsrc);
						if(!isEMPTY(errorstr1))
						{
							tmp=Rmalloc(RDAstrlen(errorstr1)+RDAstrlen(perid)+280);
							sprintf(tmp,"CANNOT UPDATE DEBIT TO CASH CODE      : Personnel ID [%-15s], Job Number [%4d], Account Number [%3d] Reason: %s",(perid==NULL?"":perid),jobno,acc->accno,errorstr1);
							addAPPlib(errorlist,tmp);
							if(tmp!=NULL) Rfree(tmp);
						}
						}
						if(errorstr1!=NULL) Rfree(errorstr1);
						errorstr2=ADVCHECKACCT2(byrnum,baccnum,acc->payable,BAL_ACCT,acc->pdeftype,fiscal_year,1,SCRNvirtualSubData,diagrsrc);
						if(!isEMPTY(errorstr2))
						{
							tmp=Rmalloc(RDAstrlen(errorstr2)+RDAstrlen(perid)+280);
							sprintf(tmp,"CANNOT UPDATE CREDIT TO PAYABLE CODE : Personnel ID [%-15s], Job Number [%4d], Account Number [%3d] Reason: %s",(perid==NULL?"":perid),jobno,acc->accno,errorstr2);
							addAPPlib(errorlist,tmp);
							if(tmp!=NULL) Rfree(tmp);
						}
						if(errorstr2!=NULL) Rfree(errorstr2);
						if(PAYROLL_SETUP->use_payroll_fund==2)
						{
							ADVupdateundist(payjtrn,NULL,"DUE FROM","PAYABLE CODE",NULL,(-1),byrnum,fiscal_year,acc->pdeftype,"EXPENDITURES",FISCALPERIOD[pmonth],amount,SCRNvirtualSubData,diagrsrc);
						} else {
							ADVupdateundist(payjtrn,NULL,"CASH CODE","PAYABLE CODE",NULL,(-1),byrnum,fiscal_year,acc->pdeftype,"EXPENDITURES",FISCALPERIOD[pmonth],amount,SCRNvirtualSubData,diagrsrc);
						}
						UNLNRDFILE(payjtrn);
						if(prev!=NULL) FreeRDATData(prev);
						upd_diag=TRUE;
						}
					}

/* DEDUCTION SECTION */
					if(job!=NULL)
					{
							total_jobgross=0.0;
							for(y=0,d=job->d;y<job->num;++y,++d)
							{
								if(d->expensed || d->liability)
								{
									gross=d->amount;
									total_amount=0;total_percentage=0;
	/* SJS USE PAYJDAC TEST BEGIN */
									if(d->use_jdac==TRUE && d->a!=NULL)
									{
										for(x=0,acc=d->a;x<d->num;++x,++acc)
										{
											FINDFLDSETSTRING(payddac,"DEDUCTION DESCRIPTION IDENTIFICATION",d->dedid);
											FINDFLDSETSHORT(payddac,"DEFINITION TYPE",acc->deftype);
											if(ADVEQLNRDsec(payddac,1,SCRNvirtualSubData,diagrsrc)) 
											{
												KEYNRD(payddac,1);
												locked=FALSE;
											} else {
												if(d->liability) 
												{
													LOCNRD(payddac);
													prev_file=RDATDataNEW(payddac);
													locked=TRUE;
												} else locked=FALSE;
											}
											if((total_amount>=gross) && (gross>0.0))
											{
												break;
											}
											ZERNRD(paydtrn);
											FINDFLDSETSTRING(paydtrn,"PERSONNEL IDENTIFICATION",perid);
											FINDFLDSETINT(paydtrn,"JOB NUMBER",jobno);
											FINDFLDSETSTRING(paydtrn,"PAY IDENTIFICATION",payid);
											FINDFLDSETINT(paydtrn,"ACCOUNT NUMBER",acc->accno);
											FINDFLDSETSTRING(paydtrn,"DEDUCTION DESCRIPTION IDENTIFICATION",d->dedid);
											LOCNRDFILE(paydtrn);
											if(!ADVEQLNRDsec(paydtrn,1,SCRNvirtualSubData,diagrsrc)) prev=RDATDataNEW(paydtrn);
											else prev=NULL;
											if((x+1)<d->num)
											{
												if(acc->prorate)
												{
													if((total_percentage+acc->percentage)>100.0)
													{
														percentage=100.0-total_percentage;
													} else percentage=acc->percentage;
													amount=round(gross*percentage/100);
													total_percentage+=percentage;
												} else {
													if(acc->amount<(jobgross-total_jobgross) && acc->amount!=0.0) 
												{
													if(jobgross!=0.0)
													{
														amount=round((gross*acc->amount)/jobgross);
													} else {
/* freaky deaky by raddog */
														amount=gross-total_amount;
													}
												} else if(acc->amount==0.0)
												{
													amount=gross-total_amount;
												} else {
													amount=gross-total_amount;
												}
												if(gross!=0.0) 
												{
													total_percentage=(round(amount/jobgross))*100;
												}
											}
											if(PAYROLL_SETUP->payjams_rollover && d->roll_over_budget && XPERT_SETUP->OrgType!=4)
											{
												ZERNRD(eyrnum);
												FINDFLDSETSTRING(eyrnum,"ACCOUNT CODE",acc->account);
												FINDFLDSETSHORT(eyrnum,"DEFINITION TYPE",acc->deftype);
												FINDFLDSETINT(eyrnum,"FISCAL YEAR",fiscal_year);
												if(ADVEQLNRDsec(eyrnum,1,SCRNvirtualSubData,diagrsrc))
												{
													KEYNRD(eyrnum,1);
												}
												zamt=ROLL_OVER_TEST(eyrnum,amount,NULL);
												if(zamt!=(-1))
												{
													amount=zamt;
												}
											}
											total_jobgross+=amount;
											total_amount+=amount;
										} else {
											if(gross>total_amount && gross>=0.0) 
											{
												amount=gross-total_amount;
											} else if(gross<0)
											{
												amount=gross-total_amount;
											} else amount=0;
										}
										if(amount!=0.0)
										{
											FINDFLDSETDOUBLE(paydtrn,"AMOUNT",amount);
											deleteflag=FALSE;
											FINDFLDSETCHAR(paydtrn,"DELETEFLAG",deleteflag);
											FINDFLDSETSHORT(paydtrn,"DEFINITION TYPE",acc->deftype);
											if(d->expensed)
											{
												FINDFLDSETSTRING(paydtrn,"ACCOUNT CODE",acc->account);
												FINDFLDSETSTRING(paydtrn,"DEBIT CODE",acc->debit);
												FINDFLDSETSTRING(paydtrn,"CREDIT CODE",acc->credit);
												if(acc->disenc && d->disenc && d->emp_disenc)
												{
													FINDFLDSETSTRING(paydtrn,"ENCUMBRANCE CODE",acc->enc_code);
													FINDFLDSETSTRING(paydtrn,"RESERVE CODE",acc->res_code);
												} else {
													temp=NULL;
													FINDFLDSETSTRING(paydtrn,"ENCUMBRANCE CODE",temp);
													FINDFLDSETSTRING(paydtrn,"RESERVE CODE",temp);
												}
											} else {
												temp=NULL;
												FINDFLDSETSTRING(paydtrn,"ACCOUNT CODE",temp);
												FINDFLDSETSTRING(paydtrn,"DEBIT CODE",temp);
												FINDFLDSETSTRING(paydtrn,"CREDIT CODE",temp);
												FINDFLDSETSTRING(paydtrn,"ENCUMBRANCE CODE",temp);
												FINDFLDSETSTRING(paydtrn,"RESERVE CODE",temp);
											}
											FINDFLDSETSTRING(paydtrn,"SOURCE USER",USERLOGIN);
											field=FLDNRD(paydtrn,"ENTRY DATE");
											if(field!=NULL)
											{
											if(field->len==8)
											{
												FINDFLDSETSTRING(paydtrn,"ENTRY DATE",curdate);
											} else FINDFLDSETSTRING(paydtrn,"ENTRY DATE",curdate10);
											}
											curtime=GETCURRENTTIME();
											FINDFLDSETSTRING(paydtrn,"ENTRY TIME",curtime);
											if(curtime!=NULL) Rfree(curtime);
											FINDFLDSETSHORT(paydtrn,"PAYROLL FUND DEFINITION TYPE",acc->pdeftype);
											if(d->liability)
											{
												FINDFLDGETSTRING(payddac,"LIABILITY CODE",&acc->payable);
												FINDFLDGETSTRING(payddac,"CASH CODE",&acc->cash);
												FINDFLDGETSTRING(payddac,"DUE FROM",&due_from);
												if(RDAstrcmp(XPERT_SETUP->zip,"62702-5030"))
												{
													duefrom_merge=MergeImage(acc->debit,due_from,
														BALANCESHEET,acc->deftype);
												} else {
													duefrom_merge=Rmalloc(41);
													sprintf(duefrom_merge,"%.2s-0000-113%.2s",&acc->cash[0],&acc->account[0]);
												}
												FINDFLDSETSTRING(paydtrn,"DUE FROM",duefrom_merge);
												if(isEMPTY(acc->payable))
												{
													tmp=Rmalloc(RDAstrlen(d->dedid)+280);
													sprintf(tmp,"MISSING LIABILITY CODE FOR DEDUCTION: Deduction Description ID [%-30s] Reason: Deduction Description marked as Liability type deduction but no Liability code(s) defined in the Deduction Description Account file.",(d->dedid==NULL?"":d->dedid));
													addAPPlib(errorlist,tmp);
													if(tmp!=NULL) Rfree(tmp);
												}
												if(PAYROLL_SETUP->use_payroll_fund==2)
												{
													if(isEMPTY(duefrom_merge))
													{
														tmp=Rmalloc(RDAstrlen(d->dedid)+290);
														sprintf(tmp,"MISSING DUE FROM CODE FOR DEDUCTION: Deduction Description ID [%-30s] Reason: Deduction Description marked as Liability type deduction but no Due From code(s) defined in the Deduction Description Account file.",(d->dedid==NULL?"":d->dedid));
														addAPPlib(errorlist,tmp);
														if(tmp!=NULL) Rfree(tmp);
													}
												} else {
													if(isEMPTY(acc->cash))
													{
														tmp=Rmalloc(RDAstrlen(d->dedid)+290);
														sprintf(tmp,"MISSING CASH CODE FOR DEDUCTION: Deduction Description ID [%-30s] Reason: Deduction Description marked as Liability type deduction but no Cash code(s) defined in the Deduction Description Account file.",(d->dedid==NULL?"":d->dedid));
														addAPPlib(errorlist,tmp);
														if(tmp!=NULL) Rfree(tmp);
													}
												}
												FINDFLDSETSTRING(paydtrn,"LIABILITY CODE",acc->payable);
												FINDFLDSETSTRING(paydtrn,"CASH CODE",acc->cash);
												FINDFLDSETSTRING(paydtrn,"DUE FROM",duefrom_merge);
											} else {
												temp=NULL;
												FINDFLDSETSTRING(paydtrn,"LIABILITY CODE",temp);
												FINDFLDSETSTRING(paydtrn,"CASH CODE",temp);
												FINDFLDSETSTRING(paydtrn,"DUE FROM",temp);
											}
											FINDFLDSETCHAR(paydtrn,"EXPENSED",d->expensed);
											FINDFLDSETCHAR(paydtrn,"LIABILITY",d->liability);
											FINDFLDSETSHORT(paydtrn,"USE PAYROLL FUND TYPE",PAYROLL_SETUP->use_payroll_fund);
											if(PAYROLL_SETUP->paymts_audit)
											{
												FINDFLDSETCHAR(paydtrn,"SOURCE USER VERIFIED",FALSE);
											} else {
												FINDFLDSETCHAR(paydtrn,"SOURCE USER VERIFIED",TRUE);
											}
											if(Use_Rate_Accounts==FALSE)
											{
												ADVWRTTRANSsec(paydtrn,0,NULL,prev,SCRNvirtualSubData,diagrsrc);
											} else {
												ADVWRTTRANSsec(paydtrn,TRUE,"ACCOUNT NUMBER",prev,SCRNvirtualSubData,diagrsrc);
											}
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
											DO_AUDIT=TRUE;
											if(d->expensed)
											{
												if(XPERT_SETUP->OrgType!=4)
												{
												errorstr=ADVCHECKACCT2(eyrnum,eaccnum,acc->account,EXP_ACCT,acc->deftype,fiscal_year,1,SCRNvirtualSubData,diagrsrc);
												if(!isEMPTY(errorstr))
												{ 
													tmp=Rmalloc(RDAstrlen(errorstr)+RDAstrlen(perid)+RDAstrlen(d->dedid)+325);
													sprintf(tmp,"CANNOT EXPENSE DEDUCTION ACCOUNT CODE : Personnel ID [%-15s], Job Number [%4d], Account Number [%3d], Deduction Description ID [%-30s] gathered from Job Deduction Accounts.  Reason: %s",(perid==NULL?"":perid),jobno,acc->accno,(d->dedid==NULL?"":d->dedid),errorstr);
													addAPPlib(errorlist,tmp);
													if(tmp!=NULL) Rfree(tmp);
												}
												if(errorstr!=NULL) Rfree(errorstr);
												}
												errorstr1=ADVCHECKACCT2(byrnum,baccnum,acc->debit,BAL_ACCT,acc->deftype,fiscal_year,1,SCRNvirtualSubData,diagrsrc);
												if(!isEMPTY(errorstr1))
												{
													tmp=Rmalloc(RDAstrlen(errorstr1)+RDAstrlen(perid)+RDAstrlen(d->dedid)+325);
													sprintf(tmp,"CANNOT UPDATE DEDUCTION DEBIT CODE    : Personnel ID [%-15s], Job Number [%4d], Account Number [%3d], Deduction Description ID [%-30s] from Job Deduction Accounts.  Reason: %s",(perid==NULL?"":perid),jobno,acc->accno,(d->dedid==NULL?"":d->dedid),errorstr1);
													addAPPlib(errorlist,tmp);
													if(tmp!=NULL) Rfree(tmp);
												}
												if(errorstr1!=NULL) Rfree(errorstr1);
												errorstr2=ADVCHECKACCT2(byrnum,baccnum,acc->credit,BAL_ACCT,acc->deftype,fiscal_year,1,SCRNvirtualSubData,diagrsrc);
												if(!isEMPTY(errorstr2))
												{
													tmp=Rmalloc(RDAstrlen(errorstr2)+RDAstrlen(perid)+RDAstrlen(d->dedid)+325);
													sprintf(tmp,"CANNOT UPDATE DEDUCTION CREDIT CODE   : Personnel ID [%-30s], Job Number [%6d], Account Number [%6d], Deduction Description ID [%-30s] from Job Deduction Accounts.  Reason: %s",(perid==NULL?"":perid),jobno,acc->accno,(d->dedid==NULL?"":d->dedid),errorstr2);
													addAPPlib(errorlist,tmp);
													if(tmp!=NULL) Rfree(tmp);
												}
												if(errorstr2!=NULL) Rfree(errorstr2);
												if(acc->disenc && d->disenc && d->emp_disenc)
												{
													errorstr2=ADVCHECKACCT2(byrnum,baccnum,acc->enc_code,BAL_ACCT,acc->deftype,fiscal_year,1,SCRNvirtualSubData,diagrsrc);
													if(!isEMPTY(errorstr2))
													{
														tmp=Rmalloc(RDAstrlen(errorstr2)+RDAstrlen(perid)+RDAstrlen(d->dedid)+280);
														sprintf(tmp,"CANNOT UPDATE DEDUCTION ENCUMBRANCE CODE   : Personnel ID [%-30s], Job Number [%6d], Account Number [%6d], Deduction Description ID [%-30s] Reason: %s",(perid==NULL?"":perid),jobno,acc->accno,(d->dedid==NULL?"":d->dedid),errorstr2);
														addAPPlib(errorlist,tmp);
														if(tmp!=NULL) Rfree(tmp);
													}
													if(errorstr2!=NULL) Rfree(errorstr2);
													errorstr2=ADVCHECKACCT2(byrnum,baccnum,acc->res_code,BAL_ACCT,acc->deftype,fiscal_year,1,SCRNvirtualSubData,diagrsrc);
													if(!isEMPTY(errorstr2))
													{
														tmp=Rmalloc(RDAstrlen(errorstr2)+RDAstrlen(perid)+RDAstrlen(d->dedid)+280);
														sprintf(tmp,"CANNOT UPDATE DEDUCTION RESERVE CODE   : Personnel ID [%-30s], Job Number [%6d], Account Number [%6d], Deduction Description ID [%-30s] Reason: %s",(perid==NULL?"":perid),jobno,acc->accno,(d->dedid==NULL?"":d->dedid),errorstr2);
														addAPPlib(errorlist,tmp);
														if(tmp!=NULL) Rfree(tmp);
													}
													if(errorstr2!=NULL) Rfree(errorstr2);
													if(XPERT_SETUP->OrgType!=4)
													{
														ADVupdateundist(paydtrn,"ACCOUNT CODE","ENCUMBRANCE CODE","RESERVE CODE",NULL,eyrnum,byrnum,fiscal_year,acc->deftype,"ENCUMBRANCES",FISCALPERIOD[pmonth],-amount,SCRNvirtualSubData,diagrsrc);
													} else {
														ADVupdateundist(paydtrn,NULL,"ENCUMBRANCE CODE","RESERVE CODE",NULL,(-1),byrnum,fiscal_year,acc->deftype,"ENCUMBRANCES",FISCALPERIOD[pmonth],-amount,SCRNvirtualSubData,diagrsrc);
													}
												}	
												if(finsetup->budget==ANNUALBUDGET && XPERT_SETUP->OrgType!=4)
												{
													if(testapprotot(eyrnum,amount,NULL)==FALSE)
													{
														if(!PAYROLL_SETUP->payjams_rollover || !d->roll_over_budget || 
															(PAYROLL_SETUP->payjams_rollover && d->roll_over_budget))
															/*
															(PAYROLL_SETUP->payjams_rollover && d->roll_over_budget && (x+1)==d->num))
															*/
														{
															warnstr=Rmalloc(RDAstrlen(acc->account)+RDAstrlen(perid)+RDAstrlen(d->dedid)+350);
															sprintf(warnstr,"INSUFFICIENT FUNDS IN AVAILABLE BALANCE: Personnel ID [%-30s], Job Number [%6d], Account Number [%6d], Deduction Description ID [%-30s], Account Code [%-40s], Year [%4d], Amount [$ %20.02f]",(perid==NULL?"":perid),jobno,acc->accno,(d->dedid==NULL?"":d->dedid),(acc->account==NULL?"":acc->account),fiscal_year,amount/100);
															addAPPlib(warnlist,warnstr);
															if(warnstr!=NULL) Rfree(warnstr);
														}
													}
												} else if(XPERT_SETUP->OrgType!=4)
												{
													if(testapprotot(eyrnum,amount,FISCALPERIOD[pmonth])==FALSE)
													{
														if(!PAYROLL_SETUP->payjams_rollover || !d->roll_over_budget || 
															(PAYROLL_SETUP->payjams_rollover && d->roll_over_budget))
															/*
															(PAYROLL_SETUP->payjams_rollover && d->roll_over_budget && (x+1)==d->num))
															*/
														{
															warnstr=Rmalloc(RDAstrlen(acc->account)+RDAstrlen(perid)+RDAstrlen(d->dedid)+RDAstrlen(FISCALPERIOD[pmonth])+400);
															sprintf(warnstr,"INSUFFICIENT FUNDS IN AVAILABLE BALANCE: Personnel ID [%-30s], Job Number [%6d], Account Number [%6d], Deduction Description ID [%-30s], Account Code [%-40s], Year [%4d], Processing Month [%9s], Amount [$ %20.02f]",(perid==NULL?"":perid),jobno,acc->accno,(d->dedid==NULL?"":d->dedid),(acc->account==NULL?"":acc->account),fiscal_year,(FISCALPERIOD[pmonth]==NULL?"":FISCALPERIOD[pmonth]),amount/100);
															addAPPlib(warnlist,warnstr);
															if(warnstr!=NULL) Rfree(warnstr);
														}
													}
												}
												if(XPERT_SETUP->OrgType!=4)
												{
													ADVupdateundist(paydtrn,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,eyrnum,byrnum,fiscal_year,acc->deftype,"EXPENDITURES",FISCALPERIOD[pmonth],amount,SCRNvirtualSubData,diagrsrc);
												} else {
													ADVupdateundist(paydtrn,NULL,"DEBIT CODE","CREDIT CODE",NULL,(-1),byrnum,fiscal_year,acc->deftype,"EXPENDITURES",FISCALPERIOD[pmonth],amount,SCRNvirtualSubData,diagrsrc);
												}
											}
											if(d->liability)
											{
												FINDFLDGETDOUBLE(payddac,"AMOUNT",&tamt);
												tamt+=amount;
												FINDFLDSETDOUBLE(payddac,"AMOUNT",tamt);
												ADVWRTTRANSsec(payddac,0,NULL,prev_file,SCRNvirtualSubData,diagrsrc);
												if(locked) UNLNRD(payddac);
												if(prev_file!=NULL) FreeRDATData(prev_file);
												if(PAYROLL_SETUP->use_payroll_fund==2)
												{
													errorstr1=ADVCHECKACCT2(byrnum,baccnum,duefrom_merge,BAL_ACCT,acc->pdeftype,fiscal_year,1,SCRNvirtualSubData,diagrsrc);
													if(!isEMPTY(errorstr1))
													{
														tmp=Rmalloc(RDAstrlen(errorstr1)+RDAstrlen(perid)+RDAstrlen(d->dedid)+280);
														sprintf(tmp,"CANNOT DEBIT DEDUCTION DUE FROM: Personnel ID [%-30s], Job Number [%6d], Deduction Description ID [%-30s], Account Number [%6d] Reason: %s",(perid==NULL?"":perid),jobno,(d->dedid==NULL?"":d->dedid),acc->accno,errorstr1);
														addAPPlib(errorlist,tmp);
														if(tmp!=NULL) Rfree(tmp);
													}
												} else {
													if(errorstr1!=NULL) Rfree(errorstr1);
													errorstr1=ADVCHECKACCT2(byrnum,baccnum,acc->cash,BAL_ACCT,acc->pdeftype,fiscal_year,1,SCRNvirtualSubData,diagrsrc);
													if(!isEMPTY(errorstr1))
													{
														tmp=Rmalloc(RDAstrlen(errorstr1)+RDAstrlen(perid)+RDAstrlen(d->dedid)+280);
														sprintf(tmp,"CANNOT DEBIT DEDUCTION CASH CODE      : Personnel ID [%-30s], Job Number [%6d], Deduction Description ID [%-30s], Account Number [%6d] Reason: %s",(perid==NULL?"":perid),jobno,(d->dedid==NULL?"":d->dedid),acc->accno,errorstr1);
														addAPPlib(errorlist,tmp);
														if(tmp!=NULL) Rfree(tmp);
													}
												}
												if(errorstr1!=NULL) Rfree(errorstr1);
												errorstr2=ADVCHECKACCT2(byrnum,baccnum,acc->payable,BAL_ACCT,acc->pdeftype,fiscal_year,1,SCRNvirtualSubData,diagrsrc);
												if(!isEMPTY(errorstr2))
												{
													tmp=Rmalloc(RDAstrlen(errorstr2)+RDAstrlen(perid)+RDAstrlen(d->dedid)+280);
													sprintf(tmp,"CANNOT CREDIT DEDUCTION LIABILITY CODE: Personnel ID [%-30s], Job Number [%6d], Deduction Description ID [%-30s], Account Number [%6d] Reason: %s",(perid==NULL?"":perid),jobno,(d->dedid==NULL?"":d->dedid),acc->accno,errorstr2);
													addAPPlib(errorlist,tmp);
													if(tmp!=NULL) Rfree(tmp);
												}
												if(errorstr2!=NULL) Rfree(errorstr2);
												if(PAYROLL_SETUP->use_payroll_fund==2)
												{
													ADVupdateundist(paydtrn,NULL,"DUE FROM","LIABILITY CODE",NULL,(-1),byrnum,fiscal_year,acc->pdeftype,"EXPENDITURES",FISCALPERIOD[pmonth],amount,SCRNvirtualSubData,diagrsrc);
												} else {
													ADVupdateundist(paydtrn,NULL,"CASH CODE","LIABILITY CODE",NULL,(-1),byrnum,fiscal_year,acc->pdeftype,"EXPENDITURES",FISCALPERIOD[pmonth],amount,SCRNvirtualSubData,diagrsrc);
												}
											}
											upd_diag=TRUE;
										}
										UNLNRDFILE(paydtrn);
										if(prev!=NULL) Rfree(prev);
										if(duefrom_merge!=NULL) Rfree(duefrom_merge);
									}
								} else {
/* SJS NEW CODE SEGMENT END */
									if(diagapps)
									{
									prterr("d->dedid [%s] pmts->num [%d] jobgross [%f] d->amount [%f] d->gross [%f] ",d->dedid,pmts->num,jobgross,d->amount,d->gross);TRACE;
									}
									total_jobgross=0.0;
									for(x=0,acc=pmts->a;x<pmts->num;++x,++acc)
									{
									if(acc->skip_deductions==FALSE)
									{
										FINDFLDSETSTRING(payddac,"DEDUCTION DESCRIPTION IDENTIFICATION",d->dedid);
										FINDFLDSETSHORT(payddac,"DEFINITION TYPE",acc->deftype);
										if(ADVEQLNRDsec(payddac,1,SCRNvirtualSubData,diagrsrc)) 
										{
											KEYNRD(payddac,1);
											locked=FALSE;
										} else {
											if(d->liability) 
											{
												LOCNRD(payddac);
												prev_file=RDATDataNEW(payddac);
												locked=TRUE;
											} else locked=FALSE;
										}
										if(d->expensed)
										{
											FINDFLDGETSTRING(payddac,"ACCOUNT CODE IMAGE",&acc_image);
											FINDFLDGETSTRING(payddac,"DEBIT CODE IMAGE",&deb_image);
											FINDFLDGETSTRING(payddac,"CREDIT CODE IMAGE",&cred_image);
											if(acc->disenc && d->disenc && d->emp_disenc)
											{
												FINDFLDGETSTRING(payddac,"ENCUMBRANCE CODE IMAGE",&enc_image);
												FINDFLDGETSTRING(payddac,"RESERVE CODE IMAGE",&res_image);
											}
										}
										if((total_amount>=gross) && (gross>0.0))
										{
											break;
										}
										ZERNRD(paydtrn);
										FINDFLDSETSTRING(paydtrn,"PERSONNEL IDENTIFICATION",perid);
										FINDFLDSETINT(paydtrn,"JOB NUMBER",jobno);
										FINDFLDSETSTRING(paydtrn,"PAY IDENTIFICATION",payid);
										FINDFLDSETINT(paydtrn,"ACCOUNT NUMBER",acc->accno);
										FINDFLDSETSTRING(paydtrn,"DEDUCTION DESCRIPTION IDENTIFICATION",d->dedid);
										LOCNRDFILE(paydtrn);
										if(!ADVEQLNRDsec(paydtrn,1,SCRNvirtualSubData,diagrsrc)) prev=RDATDataNEW(paydtrn);
											else prev=NULL;
										if(diagapps)
										{
											prterr("Number of job accounts is [%d].\n",pmts->num);
											prterr("Job rollover when in calculating deductions is [%s].\n",(pmts->roll_over_budget==TRUE? "TRUE":"FALSE")); TRACE;
										}
										if((x+1)<pmts->num)
										{
											if(acc->prorate)
											{
												if((total_percentage+acc->percentage)>100.0)
												{
													percentage=100.0-total_percentage;
												} else percentage=acc->percentage;
												amount=round(gross*percentage/100);
												total_percentage+=percentage;
											} else {
												if(diagapps)
												{
												prterr("	x [%d] acc->amount [%f] total_jobgross [%f] jobgross [%f] gross [%f] total_percentage [%f] ",x,acc->amount,total_jobgross,jobgross,gross,total_percentage);TRACE;
												}
												if(acc->amount<(jobgross-total_jobgross) && acc->amount!=0.0) 
												{
													if(jobgross!=0.0)
													{
														amount=round((gross*acc->amount)/jobgross);
													} else {
/* freaky deaky by raddog */
														amount=gross-total_amount;
													}
												} else if(acc->amount==0.0)
												{
													amount=gross-total_amount;
												} else {
													amount=gross-total_amount;
												}
												if(gross!=0.0) 
												{
/*
													total_percentage+=(round(acc->amount/jobgross))*100;
*/
													total_percentage=(round(amount/jobgross))*100;
												}
											}
/* To explain what's going on in the following if statement, if job_rollsover_deduction is TRUE, the job rollover has to be TRUE in the job master or the deduction won't check for rollover (otherwise, why tie them together?).  If job_rollsover_deduction is FALSE, it only cares if the deduction rollover is TRUE to check if it should roll over.  Of course, nothing rolls over if the payroll setup variable for Payjams rollover is FALSE.
*/
											if(PAYROLL_SETUP->payjams_rollover && 
											((pmts->roll_over_budget && job_rollsover_deduction && d->roll_over_budget) ||
											(!job_rollsover_deduction && d->roll_over_budget)) && XPERT_SETUP->OrgType!=4)
											{
												ZERNRD(eyrnum);
												if(job_rollsover_deduction)
												{
													FINDFLDSETSTRING(eyrnum,"ACCOUNT CODE",acc->account);
													if(diagapps)
													{
														prterr("Expenditure Year Master checked for Fiscal Year [%d], Account Code [%s]\n",fiscal_year,acc->account); TRACE;
													}
												} else {
													acc_merge=MergeImage(acc->account,acc_image,
															EXPENDITURE,acc->deftype);
													FINDFLDSETSTRING(eyrnum,"ACCOUNT CODE",acc_merge);
													if(diagapps)
													{
														prterr("Expenditure Year Master checked for Fiscal Year [%d], Account Code [%s]\n",fiscal_year,acc_merge); TRACE;
													}
												}
												FINDFLDSETSHORT(eyrnum,"DEFINITION TYPE",acc->deftype);
												FINDFLDSETINT(eyrnum,"FISCAL YEAR",fiscal_year);
												if(ADVEQLNRDsec(eyrnum,1,SCRNvirtualSubData,diagrsrc))
												{
													KEYNRD(eyrnum,1);
												}
												zamt=ROLL_OVER_TEST(eyrnum,amount,NULL);
												if(zamt!=(-1))
												{
													amount=zamt;
												}
											} else {
												if(diagapps)
												{
													prterr("One or more of the criteria failed for rollovers. Payroll setup rollover [%s], job rollover [%s], deduction rollover [%s] and Organization type [%d]\n",(PAYROLL_SETUP->payjams_rollover==TRUE?"TRUE":"FALSE"), (pmts->roll_over_budget==TRUE?"TRUE":"FALSE"),(d->roll_over_budget==TRUE?"TRUE":"FALSE"), XPERT_SETUP->OrgType); TRACE;
												}
											}
/*
											total_jobgross+=acc->amount;
*/
											total_jobgross+=amount;
											total_amount+=amount;
										} else {
											if(gross>total_amount && gross>=0.0) 
											{
												amount=gross-total_amount;
											} else if(gross<0)
											{
												amount=gross-total_amount;
											} else amount=0;
										}
										if(amount!=0.0)
										{
											FINDFLDSETDOUBLE(paydtrn,"AMOUNT",amount);
											deleteflag=FALSE;
											FINDFLDSETCHAR(paydtrn,"DELETEFLAG",deleteflag);
											FINDFLDSETSHORT(paydtrn,"DEFINITION TYPE",acc->deftype);
											if(d->expensed)
											{
												/*
												if((PAYROLL_SETUP->payjams_rollover && pmts->roll_over_budget && XPERT_SETUP->OrgType!=4 && (job_rollsover_deduction==FALSE)) || ((x+1)>=pmts->num))
												*/
												/* If job rolls over the deduction or it's the last job account */
												if(acc_merge==NULL)
												{
													acc_merge=MergeImage(acc->account,acc_image,
															EXPENDITURE,acc->deftype);
												}
												FINDFLDSETSTRING(paydtrn,"ACCOUNT CODE",acc_merge);
												deb_merge=MergeImage(acc->debit,deb_image,
													BALANCESHEET,acc->deftype);
												FINDFLDSETSTRING(paydtrn,"DEBIT CODE",deb_merge);
												cred_merge=MergeImage(acc->credit,cred_image,
													BALANCESHEET,acc->deftype);
												FINDFLDSETSTRING(paydtrn,"CREDIT CODE",cred_merge);
												if(acc->disenc && d->disenc && d->emp_disenc)
												{
													enc_merge=MergeImage(acc->enc_code,enc_image,BALANCESHEET,acc->deftype);
													FINDFLDSETSTRING(paydtrn,"ENCUMBRANCE CODE",enc_merge);
													res_merge=MergeImage(acc->res_code,res_image,BALANCESHEET,acc->deftype);
													FINDFLDSETSTRING(paydtrn,"RESERVE CODE",res_merge);
												} else {
													temp=NULL;
													FINDFLDSETSTRING(paydtrn,"ENCUMBRANCE CODE",temp);
													FINDFLDSETSTRING(paydtrn,"RESERVE CODE",temp);
												}
											} else {
												temp=NULL;
												FINDFLDSETSTRING(paydtrn,"ACCOUNT CODE",temp);
												FINDFLDSETSTRING(paydtrn,"DEBIT CODE",temp);
												FINDFLDSETSTRING(paydtrn,"CREDIT CODE",temp);
												FINDFLDSETSTRING(paydtrn,"ENCUMBRANCE CODE",temp);
												FINDFLDSETSTRING(paydtrn,"RESERVE CODE",temp);
											}
											FINDFLDSETSTRING(paydtrn,"SOURCE USER",USERLOGIN);
											field=FLDNRD(paydtrn,"ENTRY DATE");
											if(field!=NULL)
											{
											if(field->len==8)
											{
												FINDFLDSETSTRING(paydtrn,"ENTRY DATE",curdate);
											} else FINDFLDSETSTRING(paydtrn,"ENTRY DATE",curdate10);
											}
											curtime=GETCURRENTTIME();
											FINDFLDSETSTRING(paydtrn,"ENTRY TIME",curtime);
											if(curtime!=NULL) Rfree(curtime);
											FINDFLDGETSHORT(payddac,"PAYROLL FUND DEFINITION TYPE",&pdeftype);
											FINDFLDSETSHORT(paydtrn,"PAYROLL FUND DEFINITION TYPE",pdeftype);
											if(diagapps) { SEENRDRECORD(payddac);TRACE; }
											if(d->liability)
											{
												FINDFLDGETSTRING(payddac,"LIABILITY CODE",&liab_code);
												FINDFLDGETSTRING(payddac,"CASH CODE",&cash_code);
												FINDFLDGETSTRING(payddac,"DUE FROM",&due_from);
												/* MERGE HERE */
												liab_merge=MergeImage(acc->payable,liab_code,
													BALANCESHEET,acc->deftype);
												FINDFLDSETSTRING(paydtrn,"LIABILITY CODE",liab_merge);
												cash_merge=MergeImage(acc->cash,cash_code,
													BALANCESHEET,acc->deftype);
												FINDFLDSETSTRING(paydtrn,"CASH CODE",cash_merge);
												if(RDAstrcmp(XPERT_SETUP->zip,"62702-5030"))
												{
													FINDFLDSETSTRING(paydtrn,"DUE FROM",duefrom_merge);
													if(!isEMPTY(acc->due_from))
													{
													duefrom_merge=MergeImage(acc->due_from,due_from,
														BALANCESHEET,acc->deftype);
													} else duefrom_merge=NULL;
												} else {
													duefrom_merge=Rmalloc(41);
													sprintf(duefrom_merge,"%.2s-0000-113%.2s",&cash_merge[0],&acc_merge[0]);
												}
												FINDFLDSETSTRING(paydtrn,"DUE FROM",duefrom_merge);
												if(isEMPTY(liab_merge))
												{
													tmp=Rmalloc(RDAstrlen(d->dedid)+280);
													sprintf(tmp,"MISSING LIABILITY CODE FOR DEDUCTION: Deduction Description ID [%-30s] Reason: Deduction Description marked as Liability type deduction but no Liability code(s) defined in the Deduction Description Account file.",(d->dedid==NULL?"":d->dedid));
													addAPPlib(errorlist,tmp);
													if(tmp!=NULL) Rfree(tmp);
												}
												if(PAYROLL_SETUP->use_payroll_fund==2)
												{
													if(isEMPTY(duefrom_merge))
													{
														tmp=Rmalloc(RDAstrlen(d->dedid)+290);
														sprintf(tmp,"MISSING DUE FROM CODE FOR DEDUCTION: Deduction Description ID [%-30s] Reason: Deduction Description marked as Liability type deduction but no Due From code(s) defined in the Deduction Description Account file.",(d->dedid==NULL?"":d->dedid));
														addAPPlib(errorlist,tmp);
														if(tmp!=NULL) Rfree(tmp);
													}
												} else {
													if(isEMPTY(cash_merge))
													{
														tmp=Rmalloc(RDAstrlen(d->dedid)+290);
														sprintf(tmp,"MISSING CASH CODE FOR DEDUCTION: Deduction Description ID [%-30s] Reason: Deduction Description marked as Liability type deduction but no Cash code(s) defined in the Deduction Description Account file.",(d->dedid==NULL?"":d->dedid));
														addAPPlib(errorlist,tmp);
														if(tmp!=NULL) Rfree(tmp);
													}
												}
											} else {
												temp=NULL;
												FINDFLDSETSTRING(paydtrn,"LIABILITY CODE",temp);
												FINDFLDSETSTRING(paydtrn,"CASH CODE",temp);
												FINDFLDSETSTRING(paydtrn,"DUE FROM",temp);
											}
											FINDFLDSETCHAR(paydtrn,"EXPENSED",d->expensed);
											FINDFLDSETCHAR(paydtrn,"LIABILITY",d->liability);
											FINDFLDSETSHORT(paydtrn,"USE PAYROLL FUND TYPE",PAYROLL_SETUP->use_payroll_fund);
											if(PAYROLL_SETUP->paymts_audit)
											{
												FINDFLDSETCHAR(paydtrn,"SOURCE USER VERIFIED",FALSE);
											} else {
												FINDFLDSETCHAR(paydtrn,"SOURCE USER VERIFIED",TRUE);
											}
											if(Use_Rate_Accounts==FALSE)
											{
												ADVWRTTRANSsec(paydtrn,0,NULL,prev,SCRNvirtualSubData,diagrsrc);
											} else {
												ADVWRTTRANSsec(paydtrn,TRUE,"ACCOUNT NUMBER",prev,SCRNvirtualSubData,diagrsrc);
											}
											DO_AUDIT=TRUE;
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
											if(d->expensed)
											{
												if(XPERT_SETUP->OrgType!=4)
												{
												errorstr=ADVCHECKACCT2(eyrnum,eaccnum,acc_merge,EXP_ACCT,acc->deftype,fiscal_year,1,SCRNvirtualSubData,diagrsrc);
												if(!isEMPTY(errorstr))
												{ 
													tmp=Rmalloc(RDAstrlen(errorstr)+RDAstrlen(perid)+RDAstrlen(d->dedid)+280);
													sprintf(tmp,"CANNOT EXPENSE DEDUCTION ACCOUNT CODE : Personnel ID [%-15s], Job Number [%4d], Account Number [%3d], Deduction Description ID [%-30s] Reason: %s",(perid==NULL?"":perid),jobno,acc->accno,(d->dedid==NULL?"":d->dedid),errorstr);
													addAPPlib(errorlist,tmp);
													if(tmp!=NULL) Rfree(tmp);
												}
												}
												if(errorstr!=NULL) Rfree(errorstr);
												errorstr1=ADVCHECKACCT2(byrnum,baccnum,deb_merge,BAL_ACCT,acc->deftype,fiscal_year,1,SCRNvirtualSubData,diagrsrc);
												if(!isEMPTY(errorstr1))
												{
													tmp=Rmalloc(RDAstrlen(errorstr1)+RDAstrlen(perid)+RDAstrlen(d->dedid)+280);
													sprintf(tmp,"CANNOT UPDATE DEDUCTION DEBIT CODE    : Personnel ID [%-15s], Job Number [%4d], Account Number [%3d], Deduction Description ID [%-30s] Reason: %s",(perid==NULL?"":perid),jobno,acc->accno,(d->dedid==NULL?"":d->dedid),errorstr1);
													addAPPlib(errorlist,tmp);
													if(tmp!=NULL) Rfree(tmp);
												}
												if(errorstr1!=NULL) Rfree(errorstr1);
												errorstr2=ADVCHECKACCT2(byrnum,baccnum,cred_merge,BAL_ACCT,acc->deftype,fiscal_year,1,SCRNvirtualSubData,diagrsrc);
												if(!isEMPTY(errorstr2))
												{
													tmp=Rmalloc(RDAstrlen(errorstr2)+RDAstrlen(perid)+RDAstrlen(d->dedid)+280);
													sprintf(tmp,"CANNOT UPDATE DEDUCTION CREDIT CODE   : Personnel ID [%-15s], Job Number [%4d], Account Number [%3d], Deduction Description ID [%-30s] Reason: %s",(perid==NULL?"":perid),jobno,acc->accno,(d->dedid==NULL?"":d->dedid),errorstr2);
													addAPPlib(errorlist,tmp);
													if(tmp!=NULL) Rfree(tmp);
												}
												if(errorstr2!=NULL) Rfree(errorstr2);
												if(acc->disenc && d->disenc && d->emp_disenc)
												{
													errorstr2=ADVCHECKACCT2(byrnum,baccnum,enc_merge,BAL_ACCT,acc->deftype,fiscal_year,1,SCRNvirtualSubData,diagrsrc);
													if(!isEMPTY(errorstr2))
													{
														tmp=Rmalloc(RDAstrlen(errorstr2)+RDAstrlen(perid)+RDAstrlen(d->dedid)+280);
														sprintf(tmp,"CANNOT UPDATE DEDUCTION ENCUMBRANCE CODE   : Personnel ID [%-15s], Job Number [%4d], Account Number [%3d], Deduction Description ID [%-30s] Reason: %s",(perid==NULL?"":perid),jobno,acc->accno,(d->dedid==NULL?"":d->dedid),errorstr2);
														addAPPlib(errorlist,tmp);
														if(tmp!=NULL) Rfree(tmp);
													}
													if(errorstr2!=NULL) Rfree(errorstr2);
													errorstr2=ADVCHECKACCT2(byrnum,baccnum,res_merge,BAL_ACCT,acc->deftype,fiscal_year,1,SCRNvirtualSubData,diagrsrc);
													if(!isEMPTY(errorstr2))
													{
														tmp=Rmalloc(RDAstrlen(errorstr2)+RDAstrlen(perid)+RDAstrlen(d->dedid)+280);
														sprintf(tmp,"CANNOT UPDATE DEDUCTION RESERVE CODE   : Personnel ID [%-15s], Job Number [%4d], Account Number [%3d], Deduction Description ID [%-30s] Reason: %s",(perid==NULL?"":perid),jobno,acc->accno,(d->dedid==NULL?"":d->dedid),errorstr2);
														addAPPlib(errorlist,tmp);
														if(tmp!=NULL) Rfree(tmp);
													}
													if(errorstr2!=NULL) Rfree(errorstr2);
													if(XPERT_SETUP->OrgType!=4)
													{
														ADVupdateundist(paydtrn,"ACCOUNT CODE","ENCUMBRANCE CODE","RESERVE CODE",NULL,eyrnum,byrnum,fiscal_year,acc->deftype,"ENCUMBRANCES",FISCALPERIOD[pmonth],-amount,SCRNvirtualSubData,diagrsrc);
													} else {
														ADVupdateundist(paydtrn,NULL,"ENCUMBRANCE CODE","RESERVE CODE",NULL,(-1),byrnum,fiscal_year,acc->deftype,"ENCUMBRANCES",FISCALPERIOD[pmonth],-amount,SCRNvirtualSubData,diagrsrc);
													}
												}	
												if(finsetup->budget==ANNUALBUDGET && XPERT_SETUP->OrgType!=4)
												{
													if(testapprotot(eyrnum,amount,NULL)==FALSE)
													{
														if(!PAYROLL_SETUP->payjams_rollover || !pmts->roll_over_budget || 
															(PAYROLL_SETUP->payjams_rollover && pmts->roll_over_budget))
															/* BY DDC 12/2/2005 - It was desired that the deduction warning appear regardless of whether it was the last account code.  This is for when deductions rollover with jobs.
															(PAYROLL_SETUP->payjams_rollover && pmts->roll_over_budget && (x+1)==pmts->num))
															*/
														{
															warnstr=Rmalloc(RDAstrlen(acc_merge)+RDAstrlen(perid)+RDAstrlen(d->dedid)+350);
															sprintf(warnstr,"INSUFFICIENT FUNDS IN AVAILABLE BALANCE: Personnel ID [%-15s], Job Number [%4d], Account Number [%3d], Deduction Description ID [%-30s], Account Code [%-40s], Year [%4d], Amount [$ %20.02f]",(perid==NULL?"":perid),
						jobno,acc->accno,(d->dedid==NULL?"":d->dedid),(acc_merge==NULL?"":acc_merge),fiscal_year,amount/100);
															addAPPlib(warnlist,warnstr);
															if(warnstr!=NULL) Rfree(warnstr);
														}
													}
												} else if(XPERT_SETUP->OrgType!=4)
												{
													if(testapprotot(eyrnum,amount,FISCALPERIOD[pmonth])==FALSE)
													{
														if(!PAYROLL_SETUP->payjams_rollover || !pmts->roll_over_budget || 
					(PAYROLL_SETUP->payjams_rollover && pmts->roll_over_budget))
														/*
					(PAYROLL_SETUP->payjams_rollover && pmts->roll_over_budget && (x+1)==pmts->num))
														*/
														{
															warnstr=Rmalloc(RDAstrlen(acc_merge)+RDAstrlen(perid)+RDAstrlen(d->dedid)+RDAstrlen(FISCALPERIOD[pmonth])+400);
															sprintf(warnstr,"INSUFFICIENT FUNDS IN AVAILABLE BALANCE: Personnel ID [%-15s], Job Number [%4d], Account Number [%3d], Deduction Description ID [%-30s], Account Code [%-40s], Year [%4d], Processing Month [%9s], Amount [$ %20.02f]",
																(perid==NULL?"":perid),jobno,acc->accno,(d->dedid==NULL?"":d->dedid),(acc_merge==NULL?"":acc_merge),fiscal_year,(FISCALPERIOD[pmonth]==NULL?"":FISCALPERIOD[pmonth]),amount/100);
															addAPPlib(warnlist,warnstr);
															if(warnstr!=NULL) Rfree(warnstr);
														}
													}
												}
												if(XPERT_SETUP->OrgType!=4)
												{
													ADVupdateundist(paydtrn,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,eyrnum,byrnum,fiscal_year,acc->deftype,"EXPENDITURES",FISCALPERIOD[pmonth],amount,SCRNvirtualSubData,diagrsrc);
												} else {
													ADVupdateundist(paydtrn,NULL,"DEBIT CODE","CREDIT CODE",NULL,(-1),byrnum,fiscal_year,acc->deftype,"EXPENDITURES",FISCALPERIOD[pmonth],amount,SCRNvirtualSubData,diagrsrc);
												}
											}
											if(d->liability)
											{
												FINDFLDGETDOUBLE(payddac,"AMOUNT",&tamt);
												tamt+=amount;
												FINDFLDSETDOUBLE(payddac,"AMOUNT",tamt);
												ADVWRTTRANSsec(payddac,0,NULL,prev_file,SCRNvirtualSubData,diagrsrc);
												if(locked) UNLNRD(payddac);
												if(prev_file!=NULL) FreeRDATData(prev_file);
												if(PAYROLL_SETUP->use_payroll_fund==2)
												{
													errorstr1=ADVCHECKACCT2(byrnum,baccnum,duefrom_merge,BAL_ACCT,pdeftype,fiscal_year,1,SCRNvirtualSubData,diagrsrc);
													if(!isEMPTY(errorstr1))
													{
														tmp=Rmalloc(RDAstrlen(errorstr1)+RDAstrlen(perid)+RDAstrlen(d->dedid)+280);
														sprintf(tmp,"CANNOT DEBIT DEDUCTION DUE FROM: Personnel ID [%-15s], Job Number [%4d], Deduction Description ID [%-30s], Account Number [%6d] Reason: %s",(perid==NULL?"":perid),jobno,(d->dedid==NULL?"":d->dedid),acc->accno,errorstr1);
														addAPPlib(errorlist,tmp);
														if(tmp!=NULL) Rfree(tmp);
													}
												} else {
													if(errorstr1!=NULL) Rfree(errorstr1);
													errorstr1=ADVCHECKACCT2(byrnum,baccnum,cash_merge,BAL_ACCT,pdeftype,fiscal_year,1,SCRNvirtualSubData,diagrsrc);
													if(!isEMPTY(errorstr1))
													{
														tmp=Rmalloc(RDAstrlen(errorstr1)+RDAstrlen(perid)+RDAstrlen(d->dedid)+280);
														sprintf(tmp,"CANNOT DEBIT DEDUCTION CASH CODE      : Personnel ID [%-15s], Job Number [%4d], Deduction Description ID [%-30s], Account Number [%6d] Reason: %s",(perid==NULL?"":perid),jobno,(d->dedid==NULL?"":d->dedid),acc->accno,errorstr1);
														addAPPlib(errorlist,tmp);
														if(tmp!=NULL) Rfree(tmp);
													}
												}
												if(errorstr1!=NULL) Rfree(errorstr1);
												errorstr2=ADVCHECKACCT2(byrnum,baccnum,liab_merge,BAL_ACCT,pdeftype,fiscal_year,1,SCRNvirtualSubData,diagrsrc);
												if(!isEMPTY(errorstr2))
												{
													tmp=Rmalloc(RDAstrlen(errorstr2)+RDAstrlen(perid)+RDAstrlen(d->dedid)+280);
													sprintf(tmp,"CANNOT CREDIT DEDUCTION LIABILITY CODE: Personnel ID [%-15s], Job Number [%4d], Deduction Description ID [%-30s], Account Number [%6d] Reason: %s",(perid==NULL?"":perid),jobno,(d->dedid==NULL?"":d->dedid),acc->accno,errorstr2);
													addAPPlib(errorlist,tmp);
													if(tmp!=NULL) Rfree(tmp);
												}
												if(errorstr2!=NULL) Rfree(errorstr2);
												if(PAYROLL_SETUP->use_payroll_fund==2)
												{
													ADVupdateundist(paydtrn,NULL,"DUE FROM","LIABILITY CODE",NULL,(-1),byrnum,fiscal_year,acc->pdeftype,"EXPENDITURES",FISCALPERIOD[pmonth],amount,SCRNvirtualSubData,diagrsrc);
												} else {
													ADVupdateundist(paydtrn,NULL,"CASH CODE","LIABILITY CODE",NULL,(-1),byrnum,fiscal_year,acc->pdeftype,"EXPENDITURES",FISCALPERIOD[pmonth],amount,SCRNvirtualSubData,diagrsrc);
												}
											}
											upd_diag=TRUE;
										}
										UNLNRDFILE(paydtrn);
										if(prev!=NULL) Rfree(prev);
										if(liab_merge!=NULL) Rfree(liab_merge);
										if(cash_merge!=NULL) Rfree(cash_merge);
										if(duefrom_merge!=NULL) Rfree(duefrom_merge);
									} else {
									}
									if(acc_merge!=NULL) Rfree(acc_merge);
									}
								}
							}
							if(acc_image!=NULL) Rfree(acc_image);
							if(deb_image!=NULL) Rfree(deb_image);
							if(cred_image!=NULL) Rfree(cred_image);
							if(acc_merge!=NULL) Rfree(acc_merge);
							if(deb_merge!=NULL) Rfree(deb_merge);
							if(cred_merge!=NULL) Rfree(cred_merge);
							if(enc_merge!=NULL) Rfree(enc_merge);
							if(liab_merge!=NULL) Rfree(liab_merge);
							if(cash_merge!=NULL) Rfree(cash_merge);
							if(duefrom_merge!=NULL) Rfree(duefrom_merge);
							if(res_merge!=NULL) Rfree(res_merge);
							if(errorstr!=NULL) Rfree(errorstr);
							if(errorstr1!=NULL) Rfree(errorstr1);
							if(errorstr2!=NULL) Rfree(errorstr2);
							if(warnstr!=NULL) Rfree(warnstr);
							if(tmp!=NULL) Rfree(tmp);
						}
					}
					if(pmts!=NULL) FreePAYMTS(pmts);
					pmts=NULL;
					if(job!=NULL) FreeJobStruct(job);
					job=NULL;
					if(acc_image!=NULL) Rfree(acc_image);
					if(deb_image!=NULL) Rfree(deb_image);
					if(cred_image!=NULL) Rfree(cred_image);
					if(acc_merge!=NULL) Rfree(acc_merge);
					if(deb_merge!=NULL) Rfree(deb_merge);
					if(cred_merge!=NULL) Rfree(cred_merge);
					if(liab_merge!=NULL) Rfree(liab_merge);
					if(cash_merge!=NULL) Rfree(cash_merge);
					if(duefrom_merge!=NULL) Rfree(duefrom_merge);
					if(enc_merge!=NULL) Rfree(enc_merge);
					if(res_merge!=NULL) Rfree(res_merge);
					if(cash_code!=NULL) Rfree(cash_code);
					if(liab_code!=NULL) Rfree(liab_code);
					if(due_from!=NULL) Rfree(due_from);
					if(errorstr!=NULL) Rfree(errorstr);
					if(errorstr1!=NULL) Rfree(errorstr1);
					if(errorstr2!=NULL) Rfree(errorstr2);
					if(warnstr!=NULL) Rfree(warnstr);
					if(tmp!=NULL) Rfree(tmp);
				}
			}
			if(upd_diag)
			{
				if(prev_file!=NULL) FreeRDATData(prev_file);
				LOCNRDFILE(payppms);
				prev_file=RDATDataNEW(payppms);
				FINDFLDSETCHAR(payppms,"MADE TRANSACTIONS",TRUE);
				field=FLDNRD(payppms,"MADE TRANSACTIONS DATE");
				if(field!=NULL)
				{
				if(field->len==8)
				{
					FINDFLDSETSTRING(payppms,"MADE TRANSACTIONS DATE",curdate);
				} else FINDFLDSETSTRING(payppms,"MADE TRANSACTIONS DATE",curdate10);
				}
				FINDFLDSETSTRING(payppms,"MADE TRANSACTIONS BY",USERLOGIN);
				curtime=GETCURRENTTIME();
				FINDFLDSETSTRING(payppms,"MADE TRANSACTIONS TIME",curtime);
				if(curtime!=NULL) Rfree(curtime);
				ADVWRTTRANSsec(payppms,0,NULL,prev_file,SCRNvirtualSubData,diagrsrc);
				UNLNRDFILE(payppms);
				if(prev_file!=NULL) FreeRDATData(prev_file);
			}
			if(diagrsrc!=NULL) update_diagnostic(diagrsrc,(upd_diag ? TRUE:FALSE));
			if(curdate!=NULL) Rfree(curdate);
			if(curdate10!=NULL) Rfree(curdate10);
			jef=ADVNXTNRDsec(payjpms,jkeyno,SCRNvirtualSubData,diagrsrc);
		}
	}
	}
	ef=ADVNXTNRDsec(payppms,keyno,SCRNvirtualSubData,diagrsrc);
	}
	ZERNRD(paymstr);
	FINDFLDSETSTRING(paymstr,"PAY IDENTIFICATION",payid);
	if(ADVEQLNRDsec(paymstr,1,SCRNvirtualSubData,diagrsrc))
	{
		KEYNRD(paymstr,1);
	}
	FINDFLDGETDOUBLE(paymstr,"TOTAL GROSS",&tg);
	FINDFLDGETDOUBLE(paymstr,"TOTAL BENEFITS",&tb);
	FINDFLDGETDOUBLE(paymstr,"TOTAL ADVANCE REPAYMENTS",&ta);
	FINDFLDGETDOUBLE(paymstr,"TOTAL PAYMTS",&tp);
	td=(tg+tb+ta)-tp;
	if((tg+tb+ta)!=tp)
	{
		tdstr=famt(td,20);
		tgstr=famt(tg,20);
		tbstr=famt(tb,20);
		tastr=famt(ta,20);
		tpstr=famt(tp,20);
		temp1=Rmalloc(512);
		sprintf(temp1,"Error:  The total gross [%s] plus total benefits [%s] and advance repayments [%s]  is unequal to total expenditures [%s].  There is a difference of [%s].  Please refer to the Payroll Undo Submenu for the FINDIT Reports.",tgstr,tbstr,tastr,tpstr,tdstr);
		addAPPlib(errorlist,temp1);
		if(temp1!=NULL) Rfree(temp1);
		if(tgstr!=NULL) Rfree(tgstr);
		if(tastr!=NULL) Rfree(tastr);
		if(tbstr!=NULL) Rfree(tbstr);
		if(tpstr!=NULL) Rfree(tpstr);
		if(tdstr!=NULL) Rfree(tdstr);
	}
	if(diagrsrc!=NULL)
	{
		killwindow(diagrsrc);
		free_rsrc(diagrsrc);
	}	
	if(perid!=NULL) Rfree(perid);
	if(payid!=NULL) Rfree(payid);
	if(payid1!=NULL) Rfree(payid1);
	if(lastpayid!=NULL) Rfree(lastpayid);
	if(lastperid!=NULL) Rfree(lastperid);
	if(errorlist->numlibs<1) addAPPlib(errorlist,"No Errors Detected");
	FINDRSCLISTAPPlib(b->mainrsrc,"ERROR LIST",0,errorlist);
	updatersrc(b->mainrsrc,"ERROR LIST");
	if(warnlist->numlibs<1) addAPPlib(warnlist,"No Warnings Detected");
	FINDRSCLISTAPPlib(b->mainrsrc,"WARNING LIST",0,warnlist);
	updatersrc(b->mainrsrc,"WARNING LIST");
	if(errorlist!=NULL)
	{
		if(!RDAstrcmp(errorlist->libs[0],"No Errors Detected") && errorlist->numlibs==1)
		{		
		} else {
			FINDRSCSETEDITABLE(b->mainrsrc,"FINDIT",TRUE);
			FINDRSCSETSENSITIVE(b->mainrsrc,"FINDIT",TRUE);
		}
	}
	if(warnlist!=NULL)
	{
		if(!RDAstrcmp(warnlist->libs[0],"No Warnings Detected") && warnlist->numlibs==1)
		{		
		} else {
			FINDRSCSETEDITABLE(b->mainrsrc,"FINDIT",TRUE);
			FINDRSCSETSENSITIVE(b->mainrsrc,"FINDIT",TRUE);
		}
	}
	updatersrc(b->mainrsrc,"PAYUNMTS");
	updatersrc(b->mainrsrc,"FINDIT");
}
static short selectbpaymstr(MakeBrowseList *blist)
{
	char delflag=FALSE;

	FINDFLDGETCHAR(blist->fileno,"DELETEFLAG",&delflag);
	return((short)!delflag);
}
static void quit_paymts(RDArsrc *parent)
{
	APPlib *args=NULL;

	if(parent!=NULL) 
	{
		killwindow(parent);
		free_rsrc(parent);
	}
	CLSNRD(payjtrn);
	CLSNRD(payjprm);
	CLSNRD(posracc);
	CLSNRD(paydpms);
	CLSNRD(payjmst);
	CLSNRD(payjams);
	CLSNRD(posacc);
	CLSNRD(payppms);
	CLSNRD(payjpms);
	CLSNRD(eyrnum);
	CLSNRD(eaccnum);
	CLSNRD(byrnum);
	CLSNRD(baccnum);
	CLSNRD(payddac);
	CLSNRD(payjdac);
	CLSNRD(paymstr);
	CLSNRD(payddms);
	CLSNRD(payjdpm);
	CLSNRD(paydtrn);
	CLSNRD(paydedm);
	if((errorlist->numlibs!=1) || RDAstrcmp(errorlist->libs[0],"No Errors Detected"))
	{
		APPlib2SupportRequest("PAYROLL","paymts -> Make Payroll Transactions",errorlist);
	}
	if(errorlist!=NULL) freeapplib(errorlist);
	if(warnlist!=NULL) freeapplib(warnlist);
	if(DO_AUDIT && PAYROLL_SETUP->paymts_audit && did_payunmts==FALSE)
	{
		args=APPlibNEW();
		addAPPlib(args,module);
		addAPPlib(args,audit_reportname);
		ExecuteProgram("doreport",args);
		if(args!=NULL) freeapplib(args);
	}
	if(audit_reportname!=NULL) Rfree(audit_reportname);
	if(PAYROLL_SETUP!=NULL) free_payroll(PAYROLL_SETUP);
	if(finsetup!=NULL) free_finmgt(finsetup);
	ShutdownSubsystems();
	exit(0);
}
static void quitbpaymstr(MakeBrowseList *blist)
{
	char *temp=NULL,*payid=NULL;

	if(printed_errorlist==FALSE && errorlist->numlibs>0) 
	{
		if((errorlist->numlibs!=1) || RDAstrcmp(errorlist->libs[0],"No Errors Detected"))
		{
			ERRORDIALOG("MAKE TRANSACTION ERRORS","There were errors detected in the make transactions process.\nYou must print the error list out before you can exit this process.",NULL,FALSE);
			return;
		}
	}
	if(Force_Warning==TRUE && printed_warninglist==FALSE && warnlist->numlibs>0) 
	{
		if((warnlist->numlibs!=1) || RDAstrcmp(warnlist->libs[0],"No Warnings Detected"))
		{
			ERRORDIALOG("MAKE TRANSACTION WARNINGS","There were warnings detected in the make transactions process.\nYou must print the warning list out before you can exit this process.",NULL,FALSE);
			return;
		}
	}
	ZERNRD(paymstr);
	if(ADVEQLNRDKEYsec(paymstr,1,blist->list->keyvalue,SCRNvirtualSubData,blist->mainrsrc)) KEYNRD(paymstr,1);
	FINDFLDGETSTRING(paymstr,"PAY IDENTIFICATION",&payid);
	if(!isEMPTY(payid))
	{
		temp=Rmalloc(RDAstrlen(payid)+20);
		sprintf(temp,"PAY_IDENTIFICATION=%s",payid);
		RDA_PutEnv(temp);
	}
	if(payid!=NULL) Rfree(payid);
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
		killwindow(blist->mainrsrc);
		free_rsrc(blist->mainrsrc);
	}
	quit_paymts(blist->mainrsrc);
}
static void quit_print(RDArsrc *prsrc)
{
	if(diaggui)
	{
		prterr("DIAG quit_print Quitting Print Screen Definition.");
	}
	if(prsrc!=NULL) free_rsrc(prsrc);
}
static void printlistformatted(RDA_PFILE *fp,APPlib *list)
{
	char *temp1=NULL;
	int x=0,y=0;

	for(x=0;x<list->numlibs;++x)
	{
		temp1=Rmalloc(RDAstrlen(list->libs[x]));
		sprintf(temp1,list->libs[x]);
		for(y=0;y<(RDAstrlen(temp1));)
		{
			if(y==0)
			{
				RDA_fprintf(fp,"(%5d) ",(x+1));
				RDA_fprintf(fp,"%-.72s\r\n",temp1);
				y+=72;
			} else {
				RDA_fprintf(fp,"%-.80s\r\n",temp1+y);
				y+=80;
			}
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	RDA_fprintf(fp,"\f");
}
static void printerrorlist(RDArsrc *parent,RDA_PFILE *fp)
{
	char *date=NULL,*timex=NULL;

	if(fp!=NULL)
	{
/*
		if(doheaders)
		{
*/
			date=GETCURRENTDATE();
			timex=GETCURRENTTIME();
			RDA_fprintf(fp,"%*sXPERT PAYROLL MAKE TRANSACTIONS ERROR LIST    Date: %*s\r\n",18,"",8,date);
			RDA_fprintf(fp,"%*sExecuted By: %s%*sTime: %s\r\n\r\n",
				((80-(15+RDAstrlen(USERLOGIN)))/2),
				"",USERLOGIN,
				(((80-(15+RDAstrlen(USERLOGIN)))/2)-14),"",
				timex);
				if(date!=NULL) Rfree(date);
				if(timex!=NULL) Rfree(timex);
			printlistformatted(fp,errorlist);
/*
		}
*/
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
	char *date=NULL,*timex=NULL;

	if(fp!=NULL)
	{
/*
		if(doheaders)
		{
*/
			date=GETCURRENTDATE();
			timex=GETCURRENTTIME();
			RDA_fprintf(fp,"%*sXPERT PAYROLL MAKE TRANSACTIONS WARNING LIST   Date: %*s\r\n",17,"",8,date);
			RDA_fprintf(fp,"%*sExecuted By: %s%*sTime: %s\r\n\r\n",
				((80-(15+RDAstrlen(USERLOGIN)))/2),
				"",USERLOGIN,
				(((80-(15+RDAstrlen(USERLOGIN)))/2)-14),"",
				timex);
				if(date!=NULL) Rfree(date);
				if(timex!=NULL) Rfree(timex);
/*
		}
*/
			printlistformatted(fp,warnlist);
	}
	printed_warninglist=TRUE;
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
	char *defaultprinter=NULL,normal_print=FALSE;

	if(diaggui)
	{
		prterr("DIAG printerrorlist Displaying the Print Screen in preparation to print Error List.");
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
static void findit(RDArsrc *r)
{
	APPlib *args=NULL;

	did_findit=TRUE;
	args=APPlibNEW();
	addAPPlib(args,"PAYROLL");
	addAPPlib(args,"FINDIT");
	ExecuteProgram("doreport",args);
	if(args!=NULL) freeapplib(args);
	FINDRSCSETEDITABLE(r,"PAYUNMTS",TRUE);
	FINDRSCSETSENSITIVE(r,"PAYUNMTS",TRUE);
	updatersrc(r,"PAYUNMTS");
}
static void payunmts(RDArsrc *r)
{
	did_payunmts=TRUE;
	ExecuteProgram("payunmts",NULL);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mrsrc=NULL,*srsrc=NULL,*drsrc=NULL;
	int start=0;
	RDAGenericSetup *GSV=NULL;
	char *libx=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,module,"PAYMTS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if(argc>1)
	{
		audit_reportname=stralloc(argv[1]);
	} else audit_reportname=stralloc("PAYMTSA");
	PAYROLL_SETUP=RDApayrollNEW();
	getpayrollbin(PAYROLL_SETUP);
	finsetup=RDAfinmgtNEW();
	if(GetFinmgtSetup(finsetup,quit_paymts,mrsrc)==(-1)) return;
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif
	GSV=RDAGenericSetupNew(module,"ENCUMBER PAYROLL");
	if(getRDAGenericSetupbin(libx,GSV))
	{
		EncumberPayroll=FALSE;
	} else {
		EncumberPayroll=*GSV->value.string_value;
	}
	FreeRDAGenericSetup(GSV);	
	GSV=RDAGenericSetupNew(module,"FORCE WARNINGS");
	if(getRDAGenericSetupbin(libx,GSV))
	{
		Force_Warning=TRUE;
	} else {
		Force_Warning=*GSV->value.string_value;
	}
	FreeRDAGenericSetup(GSV);	
	GSV=RDAGenericSetupNew(module,"USE COMPUTE COMPLETED");
	if(getRDAGenericSetupbin(libx,GSV))
	{
		Use_Compute_Completed=TRUE;
	} else {
		Use_Compute_Completed=*GSV->value.string_value;
	}
	FreeRDAGenericSetup(GSV);	
	GSV=RDAGenericSetupNew(module,"JOB ACCOUNTS ROLLOVER DEDUCTIONS");
	if(getRDAGenericSetupbin(libx,GSV))
	{
		job_rollsover_deduction=FALSE;
	} else {
		job_rollsover_deduction=*GSV->value.string_value;
	}
	FreeRDAGenericSetup(GSV);	
	if(libx!=NULL) Rfree(libx);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("POSTRK")+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"POSTRK");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"POSTRK");
#endif
	GSV=RDAGenericSetupNew("POSTRK","USE POSRACC");
	if(getRDAGenericSetupbin(libx,GSV))
	{
		Use_Rate_Accounts=FALSE;
	} else {
		Use_Rate_Accounts=*GSV->value.string_value;
	}
	FreeRDAGenericSetup(GSV);	
	if(libx!=NULL) Rfree(libx);
	if((payppms=APPOPNNRD(module,"PAYPPMS",TRUE,TRUE))==(-1)) return;
	if((payjpms=APPOPNNRD(module,"PAYJPMS",TRUE,FALSE))==(-1)) return;
	if((paymstr=APPOPNNRD(module,"PAYMSTR",TRUE,FALSE))==(-1)) return;
	if((payjams=APPOPNNRD(module,"PAYJAMS",TRUE,FALSE))==(-1)) return;
	if((paydpms=APPOPNNRD(module,"PAYDPMS",TRUE,FALSE))==(-1)) return;
	if((payddac=APPOPNNRD(module,"PAYDDAC",TRUE,FALSE))==(-1)) return;
	if((payjdac=APPOPNNRD(module,"PAYJDAC",TRUE,FALSE))==(-1)) return;
	if((eyrnum=APPOPNNRD("FINMGT","FINEYR",TRUE,TRUE))==(-1)) return;
	if((eaccnum=APPOPNNRD("FINMGT","FINEACC",TRUE,TRUE))==(-1)) return;
	if((byrnum=APPOPNNRD("FINMGT","FINBYR",TRUE,TRUE))==(-1)) return;
	if((baccnum=APPOPNNRD("FINMGT","FINBACC",TRUE,TRUE))==(-1)) return;
	if((payjmst=APPOPNNRD(module,"PAYJMST",TRUE,FALSE))==(-1)) return;
	if((posacc=APPOPNNRD("POSTRK","POSACC",TRUE,FALSE))==(-1)) return;
	if((payddms=APPOPNNRD(module,"PAYDDMS",TRUE,FALSE))==(-1)) return;
	if((payjtrn=APPOPNNRD(module,"PAYJTRN",TRUE,FALSE))==(-1)) return;
	if((payjdpm=APPOPNNRD(module,"PAYJDPM",TRUE,FALSE))==(-1)) return;
	if((paydedm=APPOPNNRD(module,"PAYDEDM",TRUE,FALSE))==(-1)) return;
	if((paydtrn=APPOPNNRD(module,"PAYDTRN",TRUE,TRUE))==(-1)) return;
	if((payjprm=APPOPNNRD(module,"PAYJPRM",TRUE,FALSE))==(-1)) return;
	posracc=OPNNRD("POSTRK","POSRACC");
	if(posracc==(-1)) Use_Rate_Accounts=FALSE;
	printed_errorlist=FALSE;
	printed_warninglist=FALSE;
	mrsrc=RDArsrcNEW(module,"MAKE TRANSACTIONS BROWSE");
	srsrc=RDArsrcNEW(module,"MAKE TRANSACTIONS SEARCH BROWSE");
	drsrc=RDArsrcNEW(module,"MAKE TRANSACTIONS DEFINE LIST");
	addDFincvir(drsrc,module,"PAYMSTR",NULL,paymstr);
	addDFincvir(srsrc,module,"PAYMSTR",NULL,paymstr);
	GET_SCREEN_VIRTUALS(drsrc,1);
	GET_SCREEN_VIRTUALS(srsrc,2);
	errorlist=APPlibNEW();
	addAPPlib(errorlist,"No Errors Detected");
	warnlist=APPlibNEW();
	addAPPlib(warnlist,"No Warnings Detected");
	addlstrsrc(mrsrc,"ERROR LIST",&start,TRUE,NULL,errorlist->numlibs,&errorlist->libs,NULL);
	addlstrsrc(mrsrc,"WARNING LIST",&start,TRUE,NULL,warnlist->numlibs,&warnlist->libs,NULL);
/*lint -e611 */
	addrfcbrsrc(mrsrc,"PRINT ERROR LIST",TRUE,print_list,(void *)printerrorlistcb);
	addrfcbrsrc(mrsrc,"PRINT WARNING LIST",TRUE,print_list,(void *)printwarnlistcb);
	FINDRSCSETSENSITIVE(mrsrc,"ERROR LIST",FALSE);
	FINDRSCSETSENSITIVE(mrsrc,"WARNING LIST",FALSE);
	addrfcbrsrc(mrsrc,"PAYUNMTS",TRUE,payunmts,NULL);
	addrfcbrsrc(mrsrc,"FINDIT",TRUE,findit,NULL);
	FINDRSCSETSENSITIVE(mrsrc,"PAYUNMTS",FALSE);
	FINDRSCSETEDITABLE(mrsrc,"PAYUNMTS",FALSE);
	FINDRSCSETSENSITIVE(mrsrc,"FINDIT",FALSE);
	FINDRSCSETEDITABLE(mrsrc,"FINDIT",FALSE);
/*lint +e611 */
	ADVSbrowse(paymstr,1,mrsrc,srsrc,drsrc,NULL,NULL,selectbpaymstr,quitbpaymstr,
		okbpaymstr,NULL,NULL,TRUE,NULL,0,0,0,TRUE);
	RDAAPPMAINLOOP();
}

double ROLL_OVER_TEST(short fileno,double amount,char *pmonth)
{
	short numflds=0,x=0;
	NRDfield *field=NULL;
	double apptot=0,exptot=0,enctot=0,avlbal=0;
	char budgettype=ANNUALBUDGET;

	if(pmonth==NULL)
	{
		budgettype=ANNUALBUDGET;
	} else {
		budgettype=MONTHLYBUDGET;
	}
	switch(budgettype)
	{
		case ANNUALBUDGET:
			numflds=NUMFLDS(fileno);
			for(x=0;x<numflds;++x)
			{
				field=FLDNUM(fileno,x);
				if(field!=NULL)
				{
					if(field->type==DOLLARS || field->type==DOLLARS_NOCENTS)
					{
						if((RDAstrstr(field->name,"AMEND")==NULL) && (strstr(field->name,"UNDISTRIBUTED APPROPRIATION")==NULL)
							&&(strstr(field->name,"DISTRIBUTED APPROPRIATION")==NULL)
							&&(strstr(field->name,"APPROPRIATION")!=NULL))
						{
							apptot+=*field->data.float_value;
						} else if((strstr(field->name,"EXPENDITURE")!=NULL))
						{
							exptot+=*field->data.float_value;
						} else if((strstr(field->name,"ENCUMBRANCE")!=NULL))
						{
							enctot+=*field->data.float_value;
						}
					}
				}
			}
			break;
		case MONTHLYBUDGET:
			numflds=NUMFLDS(fileno);
			for(x=0;x<numflds;++x)
			{
				field=FLDNUM(fileno,x);
				if(field!=NULL)
				{
					if(((field->type==DOLLARS || field->type==DOLLARS_NOCENTS)&&(strstr(pmonth,field->name))!=NULL))
					{
						if((RDAstrstr(field->name,"AMEND")==NULL) && (RDAstrstr(field->name,"UNDISTRIBUTED APPROPRIATION")==NULL)
							&&(RDAstrstr(field->name,"DISTRIBUTED APPROPRIATION")==NULL)
							&&(RDAstrstr(field->name,"APPROPRIATION")!=NULL))
						{
							apptot+=*field->data.float_value;
						} else if((RDAstrstr(field->name,"EXPENDITURE")!=NULL))
						{
							exptot+=*field->data.float_value;
						} else if((RDAstrstr(field->name,"ENCUMBRANCE")!=NULL))
						{
							enctot+=*field->data.float_value;
						}
					}
				}
			}
			break;
		default:
			prterr("Error Invalid Budget Type used in Testing Available Balance.");
			break;
	}
	avlbal=apptot-exptot-enctot;
/*
	if(apptot==0.0)
	{
		return(-1);
	}
*/
	if((avlbal-amount)>=0)
	{
		return(-1);
	} else if(avlbal>0)
	{
		return(avlbal);
	} else  {
		return(0);
	}
}
