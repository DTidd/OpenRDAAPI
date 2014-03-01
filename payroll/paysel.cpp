/* paysel.c - Select Payroll */
#ifndef WIN32
#define __NAM__ "paysel.lnx"
#endif
#ifdef WIN32
#define __NAM__ "paysel.exe"
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

struct RateStructs
{
	char *rateid;
	int seq;
	char *EARNINGS_AMOUNT;
	char *PERIOD_EARNINGS;
	char *CHECK_GROSS;
	char *RATE_AMOUNT;
	char *DEFAULT_UNITS;
	double earnings;
	double period_earnings;
	double gross;
	double units;
	double rate;
	double ytd_earnings;
	double ytd_gross;
	double ytd_units;
	double ftd_earnings;
	double ftd_gross;
	double ftd_units;
};
typedef struct RateStructs RateStruct;

struct PaySelStructs
{
	char *perid;
	char *posid;
	char *payid;
	char *grossid;
	int jobno;
	int num;
	RateStruct *rates;
};
typedef struct PaySelStructs PaySelStruct;

struct PassableStructs
{
	RDArsrc *r;
	void *a;
	PaySelStruct *p;
};
typedef struct PassableStructs PassableStruct;

struct mtnstructs
{
	DBsort *dbsort;
	PaySelStruct *ps;
	RDATData *previous;
	APPlib *ratesavl;
	PassableStruct *p;
};
typedef struct mtnstructs mtnstruct;

PassableStruct *GLOBALp=NULL;
RDArsrc *mainrsrc=NULL;
char AUTO_LEAVE_DOCKED=FALSE;
static APPlib *banklist=NULL;
static APPlib *errorlist=NULL,*warnlist=NULL;
static char Force_Warning=TRUE,display_grant_year=FALSE,use_grant_management=FALSE;
static char printed_errorlist=FALSE,printed_warnlist=FALSE;
static char *module="PAYROLL",**selecttype=NULL,*audit_reportname=NULL;
static char *select_expression=NULL,**monthlist=NULL,*bank_id=NULL,*pay_id=NULL;
static char **fmonthlist=NULL;
static char *pay_type=NULL,*defbankid=NULL;
static short payjmst=(-1),payjprm=(-1),payjpms=(-1),paymstr=(-1),prsnnl=(-1);
static short posmstr=(-1),payppms=(-1),pgrat=(-1),payjcsm=(-1),pgross=(-1);
static short payjfsm=(-1),bnkacc=(-1),payjrcs=(-1),payjrfs=(-1),paypcsm=(-1);
static short paypfsm=(-1),posrate=(-1),lvemst=(-1),lvejob=(-1),lvetrn=(-1);
static short paytyp=(-1),perstat=(-1),finyear=(-1),poscat=(-1),paygrp=(-1);
static short lvedck=(-1),lvedsc=(-1),earnings_style=0,paytcsum=(-1);
static int select_type=0,DO_AUDIT=FALSE;
static short dosortwhen=FALSE,TIMECARD_TYPE=0;
double RATE_AMT=0.0,CHECK_GROSS=0.0,EARNINGS_AMT=0.0,PERIOD_EARNINGS_AMT=0.0;
double UNITS=0.0;
static RDAvirtual *csortsequence=NULL;
static MakeBrowseList *mbl_trans;
static RDAfinmgt *fmgt=NULL;
static RDApayroll *PAYROLL_SETUP=NULL;
static RDATData *prev_paymstr=NULL;
static void getfile(RDArsrc *,mtnstruct *);
static void save_record(RDArsrc *,short,mtnstruct *);
static void quit_record(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void getsupportingfiles(RDArsrc *,mtnstruct *,short);
static void doexit(MakeBrowseList *);
static void doexit_test(MakeBrowseList *);
static void changemonth(RDArsrc *);
static void resetmtndefs(RDArsrc *,mtnstruct *);
static void resetmaindefs(RDArsrc *);
DBsort *dbsort=NULL,*lvesort;
static void selectm(short,void *,int,char **,DBsort *);
static void getprevpaymstcb(RDArsrc *),getnextpaymstcb(RDArsrc *),setpayid(RDArsrc *);
static void getpreviouscb(RDArsrc *,mtnstruct *);
static void getnextcb(RDArsrc *,mtnstruct *);
static void bankidcb(RDArsrc *);
static void paytypcb(RDArsrc *);
static void paytypfunc(RDArsrc *);
static void savepaymstrsp(RDArsrc *);
static short savepaymstr(RDArsrc *);
static void savepaymstr_front(RDArsrc *);
static void test_selectpay(RDArsrc *);
static short test_selectpay2(RDArsrc *);
static void savepaymstr_do(RDArsrc *);
static short savepaymstr_check(RDArsrc *);
static void PaySelSubData(char **,PassableStruct *);
static void GetNewRateStruct(DBsort *,PaySelStruct *,char *,int,RDArsrc *);

#ifdef DIAGNOSTICS
#define SeePaySelStruct(a)	xSeePaySelStruct(a,__LINE__,__FILE__)
void xSeePaySelStruct(PaySelStruct *p,int line,char *file)
{
	int x;
	RateStruct *r=NULL;

	prterr("DIAG PaySelStruct at line [%d] program [%s].",line,file);
	if(p!=NULL)
	{
		if(p->rates!=NULL)
		{
			for(x=0,r=p->rates;x<p->num;++x,++r)
			{
				if(PAYROLL_SETUP->manage_earnings==FALSE)
				{
					prterr("Rate ID:[%s] Seq: [%d] Gross [%f] Units [%f] Rate [%f].\n",r->rateid,r->seq,r->gross,r->units,r->rate);
				} else {
					prterr("Rate ID:[%s] Seq: [%d] Earnings [%f] Period Earnings [%f] Gross [%f] Units [%f] Rate [%f].\n",r->rateid,r->seq,r->earnings,r->period_earnings,r->gross,r->units,r->rate);
				}
			}
		}
	}
}
#endif
#define PaySelStructNew(a,b,c,d,e)	xPaySelStructNew(a,b,c,d,e,__LINE__,__FILE__)
PaySelStruct *xPaySelStructNew(char *perid,int jobno,
	char *posid,char *payid,char *grossid,int line,char *file)
{
	PaySelStruct *p=NULL;

	if(diagapps)
	{
		prterr("DIAG PaySelStructNew at line [%d] program [%s].",line,file);
	}
	p=Rmalloc(sizeof(PaySelStruct));
	if(!isEMPTY(perid)) p->perid=stralloc(perid);
		else p->perid=NULL;
	if(!isEMPTY(posid)) p->posid=stralloc(posid);
		else p->posid=NULL;
	if(!isEMPTY(payid)) p->payid=stralloc(payid);
		else p->payid=NULL;
	if(!isEMPTY(grossid)) p->grossid=stralloc(grossid);
		else p->grossid=NULL;
	p->jobno=jobno;
	p->num=0;
	p->rates=NULL;
	return(p);
}
#define FreeRateStruct(a)	xFreeRateStruct(a,__LINE__,__FILE__)
void xFreeRateStruct(PaySelStruct *p,int line,char *file)
{
	RateStruct *r=NULL;
	int x;

	if(diagapps)
	{
		prterr("DIAG FreeRateStruct at line [%d] program [%s].",line,file);
	}
	if(p!=NULL)
	{
		if(p->rates!=NULL)
		{
			for(x=0,r=p->rates;x<p->num;++x,++r)
			{
				if(r->rateid!=NULL) Rfree(r->rateid);
				if(r->CHECK_GROSS!=NULL) Rfree(r->CHECK_GROSS);
				if(r->RATE_AMOUNT!=NULL) Rfree(r->RATE_AMOUNT);
				if(r->DEFAULT_UNITS!=NULL) Rfree(r->DEFAULT_UNITS);
				if(r->EARNINGS_AMOUNT!=NULL) Rfree(r->EARNINGS_AMOUNT);
				if(r->PERIOD_EARNINGS!=NULL) Rfree(r->PERIOD_EARNINGS);
			}
			Rfree(p->rates);
		}
	}
}
#define FreePaySelStruct(a)	{xFreePaySelStruct(a,__LINE__,__FILE__);a=NULL; }
void xFreePaySelStruct(PaySelStruct *p,int line,char *file)
{
	if(diagapps)
	{
		prterr("DIAG FreePaySelStruct at line [%d] program [%s].",line,file);
	}
	if(p!=NULL)
	{
		if(p->perid!=NULL) Rfree(p->perid);
		if(p->posid!=NULL) Rfree(p->posid);
		if(p->grossid!=NULL) Rfree(p->grossid);
		if(p->payid!=NULL) Rfree(p->payid);
		if(p->rates!=NULL)
		{
			FreeRateStruct(p);
			p->rates=NULL;
		}
		Rfree(p);
	}
}
#define AddRate2PaySelStruct(a,b,c,d,e,f,g,h)	xAddRate2PaySelStruct(a,b,c,d,e,f,g,0.0,0.0,0.0,0.0,0.0,h,__LINE__,__FILE__)
#define PEAddRate2PaySelStruct(a,b,c,d,e,f,g,h)	xAddRate2PaySelStruct(a,b,NULL,NULL,NULL,NULL,NULL,c,d,e,f,g,h,__LINE__,__FILE__)
int xAddRate2PaySelStruct(PaySelStruct *p,char *rateid,char *earnings_amount,
	char *period_earnings,char *check_gross,
	char *rate_amount,char *defunits,double earnings,double per_earnings,
	double units,double gross,double rate,
	int seq,int line,char *file)
{
	RateStruct *r=NULL;

	if(diagapps)
	{
		prterr("DIAG AddRatePaySelStruct at line [%d] program [%s].",
			line,file);
	}
	if(p->rates!=NULL)
	{
		p->rates=Rrealloc(p->rates,(p->num+1)*sizeof(RateStruct));
	} else {
		p->rates=Rmalloc(sizeof(RateStruct));
		p->num=0;
	}
	r=p->rates+p->num;
	if(!isEMPTY(rateid)) r->rateid=stralloc(rateid);
		else r->rateid=NULL;
	if(!isEMPTY(check_gross)) r->CHECK_GROSS=stralloc(check_gross);
		else r->CHECK_GROSS=NULL;
	if(PAYROLL_SETUP->manage_earnings==TRUE)
	{
	r->PERIOD_EARNINGS=stralloc(period_earnings);
	r->EARNINGS_AMOUNT=stralloc(earnings_amount);
	r->earnings=earnings;
	r->period_earnings=per_earnings;
	} else {
		r->PERIOD_EARNINGS=NULL;
		r->EARNINGS_AMOUNT=NULL;
		r->earnings=0;
		r->period_earnings=0;
	}
	if(!isEMPTY(rate_amount)) r->RATE_AMOUNT=stralloc(rate_amount);
		else r->RATE_AMOUNT=NULL;
	if(!isEMPTY(defunits)) r->DEFAULT_UNITS=stralloc(defunits);
		else r->DEFAULT_UNITS=NULL;
	r->gross=gross;
	r->units=units;
	r->rate=rate;
	r->seq=seq;
	return(++p->num);
}
RateStruct *GetRateStruct(PaySelStruct *p,char *name)
{
	RateStruct *r=NULL;
	int x;

	if(p!=NULL)
	{
		if(p->rates!=NULL)
		{
			for(x=0,r=p->rates;x<p->num;++x,++r)
			{
				if(!RDAstrcmp(r->rateid,"name")) return(r);
			}
		}
	}
	return(NULL);
}
void SortRateStruct(PaySelStruct *p)
{
	int x,y,comp_level;
	RateStruct *r1,*r2;
	char *tmp=NULL;
	double rtmp=0.0;

	for(x=0;x<p->num-1;++x) for(y=x+1;y<p->num;++y)
	{
		r1=p->rates+x;
		r2=p->rates+y;
		if(r2->seq<r1->seq)
		{
			comp_level=r1->seq;
			r1->seq=r2->seq;
			r2->seq=comp_level;
			tmp=r1->rateid;
			r1->rateid=r2->rateid;
			r2->rateid=tmp;
			tmp=r1->CHECK_GROSS;
			r1->CHECK_GROSS=r2->CHECK_GROSS;
			r2->CHECK_GROSS=tmp;
			tmp=r1->RATE_AMOUNT;
			r1->RATE_AMOUNT=r2->RATE_AMOUNT;
			r2->RATE_AMOUNT=tmp;
			tmp=r1->DEFAULT_UNITS;
			r1->DEFAULT_UNITS=r2->DEFAULT_UNITS;
			r2->DEFAULT_UNITS=tmp;
			if(PAYROLL_SETUP->manage_earnings==TRUE)
			{
			tmp=r1->EARNINGS_AMOUNT;
			r1->EARNINGS_AMOUNT=r2->EARNINGS_AMOUNT;
			r2->EARNINGS_AMOUNT=tmp;
			tmp=r1->PERIOD_EARNINGS;
			r1->PERIOD_EARNINGS=r2->PERIOD_EARNINGS;
			r2->PERIOD_EARNINGS=tmp;
			rtmp=r1->earnings;
			r1->earnings=r2->earnings;
			r2->earnings=rtmp;
			rtmp=r1->period_earnings;
			r1->period_earnings=r2->period_earnings;
			r2->period_earnings=rtmp;
			rtmp=r1->ytd_earnings;
			r1->ytd_earnings=r2->ytd_earnings;
			r2->ytd_earnings=rtmp;
			rtmp=r1->ftd_earnings;
			r1->ftd_earnings=r2->ftd_earnings;
			r2->ftd_earnings=rtmp;
			}
			rtmp=r1->gross;
			r1->gross=r2->gross;
			r2->gross=rtmp;
			rtmp=r1->units;
			r1->units=r2->units;
			r2->units=rtmp;
			rtmp=r1->rate;
			r1->rate=r2->rate;
			r2->rate=rtmp;
			rtmp=r1->ytd_gross;
			r1->ytd_gross=r2->ytd_gross;
			r2->ytd_gross=rtmp;
			rtmp=r1->ytd_units;
			r1->ytd_units=r2->ytd_units;
			r2->ytd_units=rtmp;
			rtmp=r1->ftd_gross;
			r1->ftd_gross=r2->ftd_gross;
			r2->ftd_gross=rtmp;
			rtmp=r1->ftd_units;
			r1->ftd_units=r2->ftd_units;
			r2->ftd_units=rtmp;
		}
	} 
}
short FIELDSubData(char **tmp,PassableStruct *p,char *modulename)
{
	char *filename=NULL,*fieldname=NULL;
	char *temp=NULL,*gn=NULL;
	short start=1,length=0,fileno=(-1),occurrence=1,str_length=0;
	NRDfield *field=NULL;

	if(diageval || diagapps || diageval_field)
	{
		prterr("DIAG FIELDSubData Substituting for [%s].",*tmp);
	}
	filename=stripfilename(*tmp);
	if(filename!=NULL)
	{
		fieldname=stripfieldname(*tmp);
		if(fieldname!=NULL)
		{
			memset(stemp,0,101);
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
			fileno=SCRNFILENUMBER(p->r,modulename,filename,occurrence);
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
						if(fieldname!=NULL) Rfree(fieldname);
						if(filename!=NULL) Rfree(filename);
						return(-1);
				}
			} else {
/*
				prterr("Error Field [%s] not found.",*tmp);
*/
				if(*tmp!=NULL) Rfree(*tmp);
				*tmp=stralloc("\"\"");
				if(fieldname!=NULL) Rfree(fieldname);
				if(filename!=NULL) Rfree(filename);
				return(1);
			}
			if(fieldname!=NULL) Rfree(fieldname);
			if(filename!=NULL) Rfree(filename);
			return(0);	
		} else {
			if(filename!=NULL) Rfree(filename);
			return(1);
		}
	} else {
		return(1);
	}
}
short STRUCTSubData(char **tmp,PassableStruct *p,char *modulename)
{
	char *filename=NULL;
	short str_length=0;
	RateStruct *d;
	PaySelStruct *c;
	int x;
	int retval=0;

	if(diageval || diagapps || diageval_field)
	{
		prterr("DIAG STRUCTSubData Substituting for [%s].",*tmp);
	}
	filename=stripfilename(*tmp);
	if(!isEMPTY(filename))
	{
		c=(PaySelStruct *)p->p;
		if(c!=NULL)
		{
			if(c->rates!=NULL)
			{
				memset(stemp,0,101);
				if(!RDAstrcmp(filename,"SELECT SEQUENCE"))
				{
					for(x=0,d=c->rates;x<c->num;++x,++d)
					{
						if(!RDAstrcmp(d->rateid,modulename)) break;
					}
					if(x<c->num)
					{
						sprintf(stemp,"%d",d->seq);
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						retval=0;
					} else {
						prterr("Error Rate Identification [%s] not found, or [%s] isn't available for this employee.",modulename,*tmp);
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("\"\"");
						retval=1;
					} 
				} else if(!RDAstrcmp(filename,"EARNINGS"))
				{
					for(x=0,d=c->rates;x<c->num;++x,++d)
					{
						if(!RDAstrcmp(d->rateid,modulename)) break;
					}
					if(x<c->num)
					{
						if(d->gross<0) 
						{
							sprintf(stemp,"(%f)",d->earnings);
						} else sprintf(stemp,"%f",d->earnings);
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						retval=0;
					} else {
						prterr("Error Rate Identification [%s] not found, or [%s] isn't available for this employee.",filename,*tmp);
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("\"\"");
						retval=1;
					}
				} else if(!RDAstrcmp(filename,"PERIOD EARNINGS"))
				{
					for(x=0,d=c->rates;x<c->num;++x,++d)
					{
						if(!RDAstrcmp(d->rateid,modulename)) break;
					}
					if(x<c->num)
					{
						if(d->period_earnings<0) 
						{
							sprintf(stemp,"(%f)",d->period_earnings);
						} else sprintf(stemp,"%f",d->period_earnings);
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						retval=0;
					} else {
						prterr("Error Rate Identification [%s] not found, or [%s] isn't available for this employee.",filename,*tmp);
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("\"\"");
						retval=1;
					}
				} else if(!RDAstrcmp(filename,"YTD EARNINGS"))
				{
					for(x=0,d=c->rates;x<c->num;++x,++d)
					{
						if(!RDAstrcmp(d->rateid,modulename)) break;
					}
					if(x<c->num)
					{
						if(d->gross<0) 
						{
							sprintf(stemp,"(%f)",d->ytd_earnings);
						} else sprintf(stemp,"%f",d->ytd_earnings);
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						retval=0;
					} else {
						prterr("Error Rate Identification [%s] not found, or [%s] isn't available for this employee.",filename,*tmp);
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("\"\"");
						retval=1;
					}
				} else if(!RDAstrcmp(filename,"FTD EARNINGS"))
				{
					for(x=0,d=c->rates;x<c->num;++x,++d)
					{
						if(!RDAstrcmp(d->rateid,modulename)) break;
					}
					if(x<c->num)
					{
						if(d->gross<0) 
						{
							sprintf(stemp,"(%f)",d->ftd_earnings);
						} else sprintf(stemp,"%f",d->ftd_earnings);
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						retval=0;
					} else {
						prterr("Error Rate Identification [%s] not found, or [%s] isn't available for this employee.",filename,*tmp);
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("\"\"");
						retval=1;
					}
				} else if(!RDAstrcmp(filename,"GROSS"))
				{
					for(x=0,d=c->rates;x<c->num;++x,++d)
					{
						if(!RDAstrcmp(d->rateid,modulename)) break;
					}
					if(x<c->num)
					{
						if(d->gross<0) 
						{
							sprintf(stemp,"(%f)",d->gross);
						} else sprintf(stemp,"%f",d->gross);
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						retval=0;
					} else {
						prterr("Error Rate Identification [%s] not found, or [%s] isn't available for this employee.",filename,*tmp);
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("\"\"");
						retval=1;
					}
				} else if(!RDAstrcmp(filename,"RATE AMOUNT"))
				{
					for(x=0,d=c->rates;x<c->num;++x,++d)
					{
						if(!RDAstrcmp(d->rateid,modulename)) break;
					}
					if(x<c->num)
					{
						memset(stemp,0,101);
						if(d->rate<0) sprintf(stemp,"(%f)",d->rate);
							else sprintf(stemp,"%f",d->rate);
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						retval=0;
					} else {
						prterr("Error Rate Identification [%s] not found, or [%s] isn't available for this employee.",filename,*tmp);
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("\"\"");
						retval=1;
					}
				} else if(!RDAstrcmp(filename,"YTD GROSS"))
				{
					for(x=0,d=c->rates;x<c->num;++x,++d)
					{
						if(!RDAstrcmp(d->rateid,modulename)) break;
					}
					if(x<c->num)
					{
						memset(stemp,0,101);
						if(d->ytd_gross<0) sprintf(stemp,"(%f)",d->ytd_gross);
							else sprintf(stemp,"%f",d->ytd_gross);
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						retval=0;
					} else {
						prterr("Error Rate Identification [%s] not found, or [%s] isn't available for this employee.",filename,*tmp);
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("\"\"");
						retval=1;
					}
				} else if(!RDAstrcmp(filename,"YTD UNITS"))
				{
					for(x=0,d=c->rates;x<c->num;++x,++d)
					{
						if(!RDAstrcmp(d->rateid,modulename)) break;
					}
					if(x<c->num)
					{
						memset(stemp,0,101);
						if(d->ytd_units<0) sprintf(stemp,"(%f)",d->ytd_units);
							else sprintf(stemp,"%f",d->ytd_units);
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						retval=0;
					} else {
						prterr("Error Rate Identification [%s] not found, or [%s] isn't available for this employee.",filename,*tmp);
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("\"\"");
						retval=1;
					}
				} else if(!RDAstrcmp(filename,"FTD GROSS"))
				{
					for(x=0,d=c->rates;x<c->num;++x,++d)
					{
						if(!RDAstrcmp(d->rateid,modulename)) break;
					}
					if(x<c->num)
					{
						memset(stemp,0,101);
						if(d->ftd_gross<0) sprintf(stemp,"(%f)",d->ftd_gross);
							else sprintf(stemp,"%f",d->ftd_gross);
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						retval=0;
					} else {
						prterr("Error Rate Identification [%s] not found, or [%s] isn't available for this employee.",filename,*tmp);
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("\"\"");
						retval=1;
					}
				} else if(!RDAstrcmp(filename,"FTD UNITS"))
				{
					for(x=0,d=c->rates;x<c->num;++x,++d)
					{
						if(!RDAstrcmp(d->rateid,modulename)) break;
					}
					if(x<c->num)
					{
						memset(stemp,0,101);
						if(d->ftd_units<0) sprintf(stemp,"(%f)",d->ftd_units);
							else sprintf(stemp,"%f",d->ftd_units);
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						retval=0;
					} else {
						prterr("Error Rate Identification [%s] not found, or [%s] isn't available for this employee.",filename,*tmp);
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("\"\"");
						retval=1;
					}
				} else if(!RDAstrcmp(filename,"UNITS"))
				{
					for(x=0,d=c->rates;x<c->num;++x,++d)
					{
						if(!RDAstrcmp(d->rateid,modulename)) break;
					}
					if(x<c->num)
					{
						memset(stemp,0,101);
						if(d->units<0) sprintf(stemp,"(%f)",d->units);
							else sprintf(stemp,"%f",d->units);
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						retval=0;
					} else {
						prterr("Error Rate Identification [%s] not found, or [%s] isn't available for this employee.",filename,*tmp);
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("\"\"");
						retval=1;
					}
				} else {
					if(filename!=NULL) Rfree(filename);
					return(1);
				}
			} else {
				if(filename!=NULL) Rfree(filename);
				return(1);
			}
		} else {
			if(filename!=NULL) Rfree(filename);
			return(1);
		}
	} else {
		if(filename!=NULL) Rfree(filename);
		return(1);
	}
	if(filename!=NULL) Rfree(filename);
	return(retval);
}
short VIRTUALSubData(char **tmp,PassableStruct *p,char *modulename)
{
	short start=1,length=0,str_length=0;
	char *temp=NULL,*gn;
	int y;
	RDAvirtual *v;

	/* virtual field */
	if(diageval || diagapps || diageval_field)
	{
		prterr("DIAG VIRTUALSubData Substituting for [%s].",*tmp);
	}
	for(y=0,v=p->r->virflds;y<p->r->num;++y,++v)
	{
		if(!RDAstrcmp(modulename,v->name)) break;
	}
	if(y<p->r->num)
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
					temp=PP_EVALUATEstr(v->expression,PaySelSubData,p);
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
				temp=stralloc(v->value.string_value);
				if(RDAstrlen(*tmp)<(length+3))
				{
					*tmp=Rrealloc(*tmp,length+3);
					memset(*tmp,0,length+3);
				}
				if(!isEMPTY(temp) && v->dlen>length)
				{
					gn=temp+(start-1);
					if(RDAstrlen(gn)>(length))
						temp[start-1+length]=0;
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"\"%s\"",&temp[start-1]);
				} else if(start<v->dlen && !isEMPTY(temp))
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
				if(!v->computed) 
				{
					v->computed=TRUE;
					*v->value.short_value=PP_EVALUATEsht(v->expression,PaySelSubData,p);
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
				memset(*tmp,0,RDAstrlen(*tmp)+1);
				memcpy(*tmp,stemp,str_length+1);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				if(!v->computed) 
				{
					v->computed=TRUE;
					*v->value.integer_value=PP_EVALUATEint(v->expression,PaySelSubData,p);
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
				memset(*tmp,0,RDAstrlen(*tmp)+1);
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
					*v->value.float_value=PP_EVALUATEdbl(v->expression,PaySelSubData,p);
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
				memset(*tmp,0,RDAstrlen(*tmp)+1);
				memcpy(*tmp,stemp,str_length+1);
				break;
			case BOOLNS:
				if(!v->computed) 
				{
					v->computed=TRUE;
					*v->value.string_value=PP_EVALUATEbol(v->expression,PaySelSubData,p);
				}
				if(RDAstrlen(*tmp)<2)
				{
					*tmp=Rrealloc(*tmp,2);
				}
				memset(*tmp,0,RDAstrlen(*tmp)+1);
				sprintf(*tmp,"%d",(*v->value.string_value ? 1:0));
				break;
			case CHARACTERS:
				if(!v->computed) 
				{
					v->computed=TRUE;
					*v->value.string_value=PP_EVALUATEbol(v->expression,PaySelSubData,p);
				}
				if(RDAstrlen(*tmp)<2)
				{
					*tmp=Rrealloc(*tmp,2);
				}
				memset(*tmp,0,RDAstrlen(*tmp)+1);
				sprintf(*tmp,"%c",*v->value.string_value);
				break;
			default:
				prterr("Error field type [%d] is invalid for virtual field [%s].",v->type,v->name); 
				if(*tmp!=NULL) Rfree(*tmp);
				*tmp=stralloc("\"\"");
				return(-1);
		}
	} else {
		if(diageval || diagapps)
		{	
			prterr("DIAG VIRTUALSubData Field [%s] not found.",*tmp);
		}
		return(1);
	}
	return(0);
}
short PAYGLOBALSubData(char **tmp,PassableStruct *p,char *modulename)
{
	short str_length=0;

	if(diageval || diagapps || diageval_field)
	{
		prterr("DIAG PAYGLOBALSubData Substituting for [%s].",*tmp);
	}
	if(!RDAstrcmp(modulename,"EARNINGS AMOUNT"))
	{
		if(EARNINGS_AMT<0) sprintf(stemp,"(%f)",EARNINGS_AMT);
			else sprintf(stemp,"%f",EARNINGS_AMT);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
	if(!RDAstrcmp(modulename,"PERIOD EARNINGS AMOUNT"))
	{
		if(PERIOD_EARNINGS_AMT<0) sprintf(stemp,"(%f)",PERIOD_EARNINGS_AMT);
			else sprintf(stemp,"%f",PERIOD_EARNINGS_AMT);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
	if(!RDAstrcmp(modulename,"UNITS"))
	{
		if(RATE_AMT<0) sprintf(stemp,"(%f)",UNITS);
			else sprintf(stemp,"%f",UNITS);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
	if(!RDAstrcmp(modulename,"RATE AMOUNT"))
	{
		if(RATE_AMT<0) sprintf(stemp,"(%f)",RATE_AMT);
			else sprintf(stemp,"%f",RATE_AMT);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CHECK GROSS"))
	{
		if(CHECK_GROSS<0) sprintf(stemp,"(%f)",CHECK_GROSS);
			else sprintf(stemp,"%f",CHECK_GROSS);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<RDAstrlen(stemp))
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
	if(!RDAstrcmp(modulename,"PAY BALANCE OF CONTRACT"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(Pay_Balance_of_Contract ? TRUE:FALSE));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	if(!RDAstrcmp(modulename,"DUMP DEFERRED PAY BALANCE"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(Dump_Deferred_Pay_Balance ? TRUE:FALSE));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		return(0);
	}
	return(1);
}
short SORTFILESubData(char **tmp,PassableStruct *p,char *modulename)
{
	DBsort *dbsort=NULL;
	NRDfield *field=NULL;
	char *temp=NULL,*gn=NULL;
	short start=1,length=0,str_length=0;

	if(diageval || diagapps || diageval_field)
	{
		prterr("DIAG SORTFILESubData Substituting for [%s].",*tmp);
	}
	if(p!=NULL) dbsort=(DBsort *)p->a;
	if(dbsort!=NULL)
	{
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
					str_length=(field->data.string_value!=NULL ? RDAstrlen(field->data.string_value):0);
					if(length==0) length=str_length;
					if(RDAstrlen(*tmp)<(length+3))
					{
						*tmp=Rrealloc(*tmp,length+3);
					}
					if(field->data.string_value!=NULL && str_length>length)
					{
						temp=stralloc(field->data.string_value);
						gn=temp+(start-1);
						if(RDAstrlen(gn)>(start-1+length))
							temp[start-1+length]=0;
						if(RDAstrlen(temp)>(start-1+length))
							temp[start-1+length]=0;
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
						if(temp!=NULL) Rfree(temp);
						temp=NULL;
					} else {
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"%s\"",(field->data.string_value!=NULL ? field->data.string_value:""));
					}
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
					if(RDAstrlen(*tmp)<str_length)
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
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
					memset(*tmp,0,RDAstrlen(*tmp)+1);
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
					prterr("Error field type %d is invalid for %s.",
					field->type,field->name); 
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=NULL;
					return(-1);
			}
		} else {
			return(1);
		}
	} else {
		return(1);
	}
	return(0);
}
static short ExecuteSubData(short (*func)(...),char **tmp,PassableStruct *p,char *modulename)
{
	short retval=FALSE;

/*lint -e746*/
	if(func==ScrolledListSubData)
	{
		retval=func(tmp,p->r);
	} else {
		retval=func(tmp,p,modulename);
	}
/*lint +e746*/
	if(retval<1) 
	{
		if(retval==(-1))
		{
			if(diageval || diagapps || diageval_field)
			{
				prterr("Error Fatal Error encountered substitution aborted.....");
			}
		}
		if(diageval || diagapps || diageval_field)
		{
			prterr("DIAG PaySelSubData Returning [%s].",*tmp);
		}
	}
	return(retval);
}
static void PaySelSubData(char **tmp,PassableStruct *p)
{
	char *modulename=NULL;

	if(diageval || diagapps || diageval_field)
	{
		prterr("DIAG PaySelSubData Substituting for [%s].",*tmp);
	}
	modulename=stripmodulename(*tmp);
	if(!isEMPTY(modulename))
	{
		if(ExecuteSubData(FIELDSubData,tmp,p,modulename)<1)
		{
			if(modulename!=NULL) Rfree(modulename);
			if(diageval || diagapps)
			{
				prterr("DIAG PaySelSubData Returning [%s].",*tmp);
			}
			return;
		}
		if(ExecuteSubData(STRUCTSubData,tmp,p,modulename)<1)
		{
			if(modulename!=NULL) Rfree(modulename);
			if(diageval || diagapps)
			{
				prterr("DIAG PaySelSubData Returning [%s].",*tmp);
			}
			return;
		}
		if(ExecuteSubData(VIRTUALSubData,tmp,p,modulename)<1)
		{
			if(modulename!=NULL) Rfree(modulename);
			if(diageval || diagapps)
			{
				prterr("DIAG PaySelSubData Returning [%s].",*tmp);
			}
			return;
		}
		if(dosortwhen==1)
		{	
		if(ExecuteSubData(SORTFILESubData,tmp,p,modulename)<1)
		{
			if(modulename!=NULL) Rfree(modulename);
			if(diageval || diagapps)
			{
				prterr("DIAG PaySelSubData Returning [%s].",*tmp);
			}
			return;
		}
		}
		if(ExecuteSubData(PAYGLOBALSubData,tmp,p,modulename)==0)
		{
			if(modulename!=NULL) Rfree(modulename);
			if(diageval || diagapps)
			{
				prterr("DIAG PaySelSubData Returning [%s].",*tmp);
			}
			return;
		}
		if(GLOBALSubData(tmp,modulename)==0)
		{
			if(modulename!=NULL) Rfree(modulename);
			if(diageval || diagapps)
			{
				prterr("DIAG PaySelSubData Returning [%s].",*tmp);
			}
			return;
		}
		if(ExecuteSubData(ScrolledListSubData,tmp,p,NULL)<1)
		{
			if(modulename!=NULL) Rfree(modulename);
			if(diageval || diagapps)
			{
				prterr("DIAG PaySelSubData Returning [%s].",*tmp);
			}
			return;
		}
		if(dosortwhen==0)
		{	
		if(ExecuteSubData(SORTFILESubData,tmp,p,modulename)<1)
		{
			if(modulename!=NULL) Rfree(modulename);
			if(diageval || diagapps)
			{
				prterr("DIAG PaySelSubData Returning [%s].",*tmp);
			}
			return;
		}
		}
	}
	prterr("Error PaySelSubData [%s] not found.",*tmp);
	if(*tmp!=NULL) Rfree(*tmp);
	*tmp=stralloc("\"\"");
	if(diageval || diagapps || diageval_field)
	{
		prterr("DIAG PaySelSubData Returning [%s].",*tmp);
	}
	if(modulename!=NULL) Rfree(modulename);
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
	AddFileFields(list,payjmst);
	AddFileFields(list,prsnnl);
	AddFileFields(list,posmstr);
	AddFileFields(list,pgross);
	AddFileFields(list,payjcsm);
	AddFileFields(list,payjfsm);
	AddFileFields(list,payjrcs);
	AddFileFields(list,payjrfs);
	AddFileFields(list,paymstr);
	AddFileFields(list,posrate);
	tmprsrc=RDArsrcNEW(module,"TEMP");
	addDFincvir(tmprsrc,module,"PAYJMST",NULL,dumb);
	addDFincvir(tmprsrc,"PRSNNL","PERDMG",NULL,dumb);
	addDFincvir(tmprsrc,"POSTRK","POSMSTR",NULL,dumb);
	addDFincvir(tmprsrc,"POSTRK","POSGRS",NULL,dumb);
	addDFincvir(tmprsrc,module,"PAYJCSM",NULL,dumb);
	addDFincvir(tmprsrc,module,"PAYJFSM",NULL,dumb);
	addDFincvir(tmprsrc,module,"PAYMSTR",NULL,dumb);
	addDFincvir(tmprsrc,module,"PAYJRCS",NULL,dumb);
	addDFincvir(tmprsrc,module,"PAYJRFS",NULL,dumb);
	addDFincvir(tmprsrc,"POSTRK","POSRATE",NULL,dumb);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+17);
#ifndef WIN32
	sprintf(libx,"%s/rda/PAYROLL.VIR",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\PAYROLL.VIR",CURRENTDIRECTORY);
#endif
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
APPlib *makefieldvallistB()
{
	APPlib *list=NULL,*virflist=NULL;
	RDArsrc *tmprsrc=NULL;
	short dumb=0,addvf=FALSE;
	char *libx=NULL;
	int y,x,z;
	DFincvir *i,*a;
	DFvirtual *d;

	list=APPlibNEW();
	AddFileFields(list,paymstr);
	if(paytyp!=(-1))
	{
		AddFileFields(list,paytyp);
	}
	tmprsrc=RDArsrcNEW(module,"TEMP");
	addDFincvir(tmprsrc,module,"PAYMSTR",NULL,dumb);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+17);
#ifndef WIN32
	sprintf(libx,"%s/rda/PAYROLL.VIR",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\PAYROLL.VIR",CURRENTDIRECTORY);
#endif
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
void loadvalueB(RDArsrc *r,char *fieldname)
{
	int x;
	LoadValue *load;

	ldvalrsrc=RDArsrcNEW("RPTGEN","LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	loadlist=makefieldvallistB();
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
	short ef=FALSE,update_diag=FALSE,src=0;
	char deleteflag=FALSE,valid_last_exists=FALSE;
	char *perid=NULL,*lastperid=NULL,*curdate=NULL,*curtime=NULL,*e=NULL;
	char *curdate10=NULL,bme=FALSE;
	NRDfield *field=NULL;
	int jobno=0,lastjobno=0;
	double gamt=0.0,units=0.0,job_amt=0,job_units=0.0;
	double job_earnings=0.0,earnings=0.0,a=0.0,tamt=0.0,tunits=0.0;
	double prevgross=0.0,per_gross=0.0,per_net=0.0;
	RDArsrc *tmprsrc=NULL;
	RDATData *prev=NULL;
	PassableStruct *p=NULL;

	if(dbsort!=NULL)
	{
		tmprsrc=diagscrn(dbsort->fileno,"DIAGNOSTIC SCREEN",module,
			"Updating Payroll Job Masters",NULL);
		addDFincvir(tmprsrc,"PRSNNL","PERDMG",NULL,prsnnl);
		addDFincvir(tmprsrc,module,"PAYJMST",NULL,payjmst);
		addDFincvir(tmprsrc,module,"PAYPPMS",NULL,payppms);
		addDFincvir(tmprsrc,module,"PAYJPMS",NULL,payjpms);
		addDFincvir(tmprsrc,module,"PAYJPRM",NULL,payjprm);
		addDFincvir(tmprsrc,module,"PAYMSTR",NULL,paymstr);
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
		ef=FRSNRD(dbsort->fileno,1);
		job_amt=0;
		job_units=0.0;
		FINDFLDGETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",&lastperid);
		FINDFLDGETINT(dbsort->fileno,"JOB NUMBER",&lastjobno);
		while(!ef)
		{
			curdate=GETCURRENTDATE();
			curdate10=GETCURRENTDATE10();
			update_diag=FALSE;
			FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&deleteflag);
			FINDFLDGETCHAR(dbsort->fileno,"BLOCK MANAGED EARNINGS",&bme);
			FINDFLDGETDOUBLE(dbsort->fileno,"CHECK GROSS",&gamt);
			if(PAYROLL_SETUP->manage_earnings==TRUE)
			{
				if(bme==TRUE)
				{
					earnings=gamt;
				} else FINDFLDGETDOUBLE(dbsort->fileno,"PERIOD EARNINGS",&earnings);
			}
			FINDFLDGETDOUBLE(dbsort->fileno,"UNITS",&units);
			if(curtime!=NULL) Rfree(curtime);
			curtime=GETCURRENTTIME();

			if(!deleteflag && (gamt!=0.0 || units!=0.0))
			{
				FINDFLDGETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",&perid);
				FINDFLDGETINT(dbsort->fileno,"JOB NUMBER",&jobno);

				if(RDAstrcmp(lastperid,perid))
				{
					if(valid_last_exists)
					{
						ZERNRD(payjpms);
						FINDFLDSETSTRING(payjpms,"PERSONNEL IDENTIFICATION",
							lastperid);
						FINDFLDSETINT(payjpms,"JOB NUMBER",lastjobno);
						ZERNRD(payjmst);
						FINDFLDSETSTRING(payjmst,"PERSONNEL IDENTIFICATION",lastperid);
						FINDFLDSETINT(payjmst,"JOB NUMBER",lastjobno);
						if(EQLNRD(payjmst,1)) KEYNRD(payjmst,1);
						FINDFLDSETSTRING(payjpms,"PAY IDENTIFICATION",pay_id);
						LOCNRDFILE(payjpms);
						if(!ADVEQLNRDsec(payjpms,1,PaySelSubData,p)) prev=RDATDataNEW(payjpms);
						else prev=NULL;
						FINDFLDSETCHAR(payjpms,"DELETEFLAG",deleteflag);
						if(PAYROLL_SETUP->manage_earnings==TRUE)
						{
							FINDFLDSETDOUBLE(payjpms,"EARNINGS",job_earnings);
						}
						if(earnings_style==1)
						{
							FINDFLDGETDOUBLE(payjpms,"GROSS",&tamt);
							FINDFLDGETDOUBLE(payjpms,"UNITS",&tunits);
							tamt+=job_amt;
							tunits+=job_units;
							FINDFLDSETDOUBLE(payjpms,"GROSS",tamt);
							FINDFLDSETDOUBLE(payjpms,"UNITS",tunits);
							if(PAYROLL_SETUP->manage_earnings==FALSE)
							{
								FINDFLDSETDOUBLE(payjpms,"EARNINGS",tamt);
							}
						} else {
							FINDFLDSETDOUBLE(payjpms,"GROSS",job_amt);
							FINDFLDSETDOUBLE(payjpms,"UNITS",job_units);
							if(PAYROLL_SETUP->manage_earnings==FALSE)
							{
								FINDFLDSETDOUBLE(payjpms,"EARNINGS",job_amt);
							}
						}
						COPYFIELD(payjmst,payjpms,"POSITION IDENTIFICATION");
						COPYFIELD(payjmst,payjpms,"GROSS IDENTIFICATION");
						COPYFIELD(payjmst,payjpms,"GRADE");
						COPYFIELD(payjmst,payjpms,"STEP");
						COPYFIELD(payjmst,payjpms,"STEP ALPHA");
						COPYFIELD(payjmst,payjpms,"LIFE INSURANCE SALARY");
						COPYFIELD(payjmst,payjpms,"RETIREMENT SALARY");
						COPYFIELD(payjmst,payjpms,"PERFORMANCE FACTOR");
						COPYFIELD(payjmst,payjpms,"FTE AMOUNT");
						COPYFIELD(payjmst,payjpms,"MISC SALARY AMOUNT");
						COPYFIELD(payjmst,payjpms,"MISC UNIT AMOUNT");
						COPYFIELD(payjmst,payjpms,"EXTRA SERVICE AMOUNT");
						FINDFLDSETSTRING(payjpms,"SOURCE USER",USERLOGIN);
						FINDFLDSETSTRING(payjpms,"BANK IDENTIFICATION",bank_id);
						field=FLDNRD(payjpms,"ENTRY DATE");
						if(field->len==8)
						{
							FINDFLDSETSTRING(payjpms,"ENTRY DATE",curdate);
						} else FINDFLDSETSTRING(payjpms,"ENTRY DATE",curdate10);
						FINDFLDSETSTRING(payjpms,"ENTRY TIME",curtime);
						if(PAYROLL_SETUP->paysel_audit)
						{
							FINDFLDSETCHAR(payjpms,"SOURCE USER VERIFIED",FALSE);
						} else {
							FINDFLDSETCHAR(payjpms,"SOURCE USER VERIFIED",TRUE);
						}
						ADVWRTTRANSsec(payjpms,0,NULL,prev,PaySelSubData,p);
						UNLNRDFILE(payjpms);
						if(prev!=NULL) FreeRDATData(prev);
						job_amt=0;
						job_units=0;
						job_earnings=0;
						lastjobno=jobno;
						ZERNRD(payppms);
						FINDFLDSETSTRING(payppms,"PERSONNEL IDENTIFICATION",
							lastperid);
						FINDFLDSETSTRING(payppms,"PAY IDENTIFICATION",pay_id);
						LOCNRDFILE(payppms);
						if(!ADVEQLNRDsec(payppms,1,PaySelSubData,p)) prev=RDATDataNEW(payppms);
						else prev=NULL;
						FINDFLDSETCHAR(payppms,"DELETEFLAG",deleteflag);
						FINDFLDSETCHAR(payppms,"BENEFITS COMPUTED",
							deleteflag);
						FINDFLDSETSTRING(payppms,"SOURCE USER",USERLOGIN);
						FINDFLDSETCHAR(payppms,"MADE TRANSACTIONS",deleteflag);
						/*FINDFLDSETDOUBLE(payppms,"GROSS",per_gross);*/
						FINDFLDSETDOUBLE(payppms,"NET",per_net);
						ADVWRTTRANSsec(payppms,0,NULL,prev,PaySelSubData,p);
						if(prev!=NULL) FreeRDATData(prev);
						UNLNRDFILE(payppms);
					} 
					if(lastperid!=NULL) Rfree(lastperid);
					lastperid=stralloc(perid);
					lastjobno=jobno;
					per_gross=0.0;
				} else if(lastjobno!=jobno)
				{
					if(valid_last_exists)
					{
						ZERNRD(payjpms);
						FINDFLDSETSTRING(payjpms,"PERSONNEL IDENTIFICATION",
							lastperid);
						FINDFLDSETINT(payjpms,"JOB NUMBER",lastjobno);
						FINDFLDSETSTRING(payjpms,"PAY IDENTIFICATION",pay_id);
						ZERNRD(payjmst);
						FINDFLDSETSTRING(payjmst,"PERSONNEL IDENTIFICATION",lastperid);
						FINDFLDSETINT(payjmst,"JOB NUMBER",lastjobno);
						if(EQLNRD(payjmst,1)) KEYNRD(payjmst,1);
						LOCNRDFILE(payjpms);
						if(!ADVEQLNRDsec(payjpms,1,PaySelSubData,p)) prev=RDATDataNEW(payjpms);
						else prev=NULL;
						FINDFLDSETCHAR(payjpms,"DELETEFLAG",deleteflag);
						if(PAYROLL_SETUP->manage_earnings==TRUE)
						{
							FINDFLDSETDOUBLE(payjpms,"EARNINGS",job_earnings);
						}
						if(earnings_style==1)
						{
							FINDFLDGETDOUBLE(payjpms,"GROSS",&tamt);
							FINDFLDGETDOUBLE(payjpms,"UNITS",&tunits);
							tamt+=job_amt;
							tunits+=job_units;
							FINDFLDSETDOUBLE(payjpms,"GROSS",tamt);
							FINDFLDSETDOUBLE(payjpms,"UNITS",tunits);
							if(PAYROLL_SETUP->manage_earnings==FALSE)
							{
								FINDFLDSETDOUBLE(payjpms,"EARNINGS",tamt);
							}
						} else {
							FINDFLDSETDOUBLE(payjpms,"GROSS",job_amt);
							FINDFLDSETDOUBLE(payjpms,"UNITS",job_units);
							if(PAYROLL_SETUP->manage_earnings==FALSE)
							{
								FINDFLDSETDOUBLE(payjpms,"EARNINGS",job_amt);
							}
						}
						field=FLDNRD(payjpms,"ENTRY DATE");
						if(field->len==8)
						{
							FINDFLDSETSTRING(payjpms,"ENTRY DATE",curdate);
						} else FINDFLDSETSTRING(payjpms,"ENTRY DATE",curdate10);
						FINDFLDSETSTRING(payjpms,"ENTRY TIME",curtime);
						COPYFIELD(payjmst,payjpms,"POSITION IDENTIFICATION");
						COPYFIELD(payjmst,payjpms,"GROSS IDENTIFICATION");
						COPYFIELD(payjmst,payjpms,"GRADE");
						COPYFIELD(payjmst,payjpms,"STEP");
						COPYFIELD(payjmst,payjpms,"STEP ALPHA");
						COPYFIELD(payjmst,payjpms,"LIFE INSURANCE SALARY");
						COPYFIELD(payjmst,payjpms,"RETIREMENT SALARY");
						COPYFIELD(payjmst,payjpms,"PERFORMANCE FACTOR");
						COPYFIELD(payjmst,payjpms,"FTE AMOUNT");
						COPYFIELD(payjmst,payjpms,"MISC SALARY AMOUNT");
						COPYFIELD(payjmst,payjpms,"MISC UNIT AMOUNT");
						COPYFIELD(payjmst,payjpms,"EXTRA SERVICE AMOUNT");
						FINDFLDSETSTRING(payjpms,"SOURCE USER",USERLOGIN);
						FINDFLDSETSTRING(payjpms,"BANK IDENTIFICATION",bank_id);
						if(PAYROLL_SETUP->paysel_audit)
						{
							FINDFLDSETCHAR(payjpms,"SOURCE USER VERIFIED",FALSE);
						} else {
							FINDFLDSETCHAR(payjpms,"SOURCE USER VERIFIED",TRUE);
						}
						ADVWRTTRANSsec(payjpms,0,NULL,prev,PaySelSubData,p);
						UNLNRDFILE(payjpms);
						if(prev!=NULL) FreeRDATData(prev);
						job_amt=0;
						job_units=0;
						job_earnings=0;
					}
					lastjobno=jobno;
				}
				per_gross+=gamt;
				job_amt+=gamt;
				job_units+=units;
				job_earnings+=earnings;
				ZERNRD(payjprm);
				COPYFIELD(dbsort->fileno,payjprm,"PERSONNEL IDENTIFICATION");
				COPYFIELD(dbsort->fileno,payjprm,"JOB NUMBER");
				COPYFIELD(dbsort->fileno,payjprm,"PAY IDENTIFICATION");
				COPYFIELD(dbsort->fileno,payjprm,"RATE IDENTIFICATION");
				LOCNRDFILE(payjprm);
				if(!ADVEQLNRDsec(payjprm,1,PaySelSubData,p)) prev=RDATDataNEW(payjprm);
				else prev=NULL;
				FINDFLDSETCHAR(payjprm,"DELETEFLAG",deleteflag);
				if(PAYROLL_SETUP->manage_earnings)
 				{
					FINDFLDSETDOUBLE(payjprm,"EARNINGS",earnings);
				} else {
					FINDFLDSETDOUBLE(payjprm,"EARNINGS",gamt);
				}
				COPYFIELD(dbsort->fileno,payjprm,"RATE AMOUNT");
				COPYFIELD(dbsort->fileno,payjprm,"NOTES");
				if(earnings_style==1)
				{
					FINDFLDGETDOUBLE(payjprm,"GROSS",&tamt);
					FINDFLDGETDOUBLE(payjprm,"UNITS",&tunits);
					tamt+=gamt;
					tunits+=units;
					FINDFLDSETDOUBLE(payjprm,"GROSS",tamt);
					FINDFLDSETDOUBLE(payjprm,"UNITS",tunits);
					if(PAYROLL_SETUP->manage_earnings==FALSE)
					{
						FINDFLDSETDOUBLE(payjprm,"EARNINGS",tamt);
					}
				} else {
					FINDFLDSETDOUBLE(payjprm,"GROSS",gamt);
					FINDFLDSETDOUBLE(payjprm,"UNITS",units);
					if(PAYROLL_SETUP->manage_earnings==FALSE)
					{
						FINDFLDSETDOUBLE(payjprm,"EARNINGS",gamt);
					}
				}
				FINDFLDSETSTRING(payjprm,"SOURCE USER",USERLOGIN);
				FINDFLDSETCHAR(payjprm,"SOURCE USER VERIFIED",deleteflag);
				field=FLDNRD(payjprm,"ENTRY DATE");
				if(field->len==8)
				{
					FINDFLDSETSTRING(payjprm,"ENTRY DATE",curdate);
				} else FINDFLDSETSTRING(payjprm,"ENTRY DATE",curdate10);
				FINDFLDSETSTRING(payjprm,"ENTRY TIME",curtime);
				FINDFLDSETSHORT(payjprm,"SOURCE",src);
				ADVWRTTRANSsec(payjprm,0,NULL,prev,PaySelSubData,p);
				UNLNRDFILE(payjprm);
				if(prev!=NULL) FreeRDATData(prev);
				ZERNRD(paymstr);
				COPYFIELD(dbsort->fileno,paymstr,"PAY IDENTIFICATION");
				if(ADVEQLNRDsec(paymstr,1,PaySelSubData,p))
				{
					prev=NULL;
					KEYNRD(paymstr,1);
				} else {
					prev=RDATDataNEW(paymstr);
				}
				FINDFLDGETDOUBLE(paymstr,"TOTAL GROSS",&a);
				a+=gamt;
				FINDFLDSETDOUBLE(paymstr,"TOTAL GROSS",a);
				ADVWRTTRANSsec(paymstr,0,NULL,prev,PaySelSubData,p);
				if(prev!=NULL) FreeRDATData(prev);	

				/* added by mms	*/
				ZERNRD(payppms);
				COPYFIELD(dbsort->fileno,payppms,"PAY IDENTIFICATION");
				COPYFIELD(dbsort->fileno,payppms,"PERSONNEL IDENTIFICATION");
				if(ADVEQLNRDsec(payppms,1,PaySelSubData,p))
				{
					prev=NULL;
					KEYNRD(payppms,1);
				} else {
					prev=RDATDataNEW(payppms);
				}
				FINDFLDGETDOUBLE(payppms,"GROSS",&prevgross);
				prevgross+=gamt;
				FINDFLDSETDOUBLE(payppms,"GROSS",prevgross);
				ADVWRTTRANSsec(payppms,0,NULL,prev,PaySelSubData,p);
				if(prev!=NULL) FreeRDATData(prev);	
				/* end of added by mms	*/
				
				update_diag=TRUE;
				DO_AUDIT=TRUE;
				valid_last_exists=TRUE;
			}
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(update_diag ? TRUE:FALSE));
			if(curdate!=NULL) Rfree(curdate);
			if(curdate10!=NULL) Rfree(curdate10);
			ef=NXTNRD(dbsort->fileno,1);
		}
		if(valid_last_exists)
		{
			deleteflag=FALSE;
			curdate=GETCURRENTDATE();
			curdate10=GETCURRENTDATE10();
			ZERNRD(payjpms);
			FINDFLDSETSTRING(payjpms,"PERSONNEL IDENTIFICATION",lastperid);
			FINDFLDSETINT(payjpms,"JOB NUMBER",lastjobno);
			FINDFLDSETSTRING(payjpms,"PAY IDENTIFICATION",pay_id);
			ZERNRD(payjmst);
			FINDFLDSETSTRING(payjmst,"PERSONNEL IDENTIFICATION",lastperid);
			FINDFLDSETINT(payjmst,"JOB NUMBER",lastjobno);
			if(EQLNRD(payjmst,1)) KEYNRD(payjmst,1);
			LOCNRDFILE(payjpms);
			if(!ADVEQLNRDsec(payjpms,1,PaySelSubData,p)) prev=RDATDataNEW(payjpms);
			else prev=NULL;
			FINDFLDSETCHAR(payjpms,"DELETEFLAG",deleteflag);
			if(PAYROLL_SETUP->manage_earnings)
			{
				FINDFLDSETDOUBLE(payjpms,"EARNINGS",job_earnings);
			}
			if(earnings_style==1)
			{
				FINDFLDGETDOUBLE(payjpms,"GROSS",&tamt);
				FINDFLDGETDOUBLE(payjpms,"UNITS",&tunits);
				tamt+=job_amt;
				tunits+=job_units;
				FINDFLDSETDOUBLE(payjpms,"GROSS",tamt);
				FINDFLDSETDOUBLE(payjpms,"UNITS",tunits);
			} else {
				FINDFLDSETDOUBLE(payjpms,"GROSS",job_amt);
				FINDFLDSETDOUBLE(payjpms,"UNITS",job_units);
			}
			field=FLDNRD(payjpms,"ENTRY DATE");
			if(field->len==8)
			{
				FINDFLDSETSTRING(payjpms,"ENTRY DATE",curdate);
			} else FINDFLDSETSTRING(payjpms,"ENTRY DATE",curdate10);
			FINDFLDSETSTRING(payjpms,"ENTRY TIME",curtime);
			COPYFIELD(payjmst,payjpms,"POSITION IDENTIFICATION");
			COPYFIELD(payjmst,payjpms,"GROSS IDENTIFICATION");
			COPYFIELD(payjmst,payjpms,"GRADE");
			COPYFIELD(payjmst,payjpms,"STEP");
			COPYFIELD(payjmst,payjpms,"STEP ALPHA");
			COPYFIELD(payjmst,payjpms,"LIFE INSURANCE SALARY");
			COPYFIELD(payjmst,payjpms,"RETIREMENT SALARY");
			COPYFIELD(payjmst,payjpms,"PERFORMANCE FACTOR");
			COPYFIELD(payjmst,payjpms,"FTE AMOUNT");
			COPYFIELD(payjmst,payjpms,"MISC SALARY AMOUNT");
			COPYFIELD(payjmst,payjpms,"MISC UNIT AMOUNT");
			COPYFIELD(payjmst,payjpms,"EXTRA SERVICE AMOUNT");
			FINDFLDSETSTRING(payjpms,"SOURCE USER",USERLOGIN);
			FINDFLDSETSTRING(payjpms,"BANK IDENTIFICATION",bank_id);
			if(PAYROLL_SETUP->paysel_audit)
			{
				FINDFLDSETCHAR(payjpms,"SOURCE USER VERIFIED",FALSE);
			} else {
				FINDFLDSETCHAR(payjpms,"SOURCE USER VERIFIED",TRUE);
			}
			ADVWRTTRANSsec(payjpms,0,NULL,prev,PaySelSubData,p);
			if(prev!=NULL) FreeRDATData(prev);
			UNLNRDFILE(payjpms);
			job_amt=0;
			job_units=0;
			job_earnings=0;
			lastjobno=jobno;
			ZERNRD(payppms);
			FINDFLDSETSTRING(payppms,"PERSONNEL IDENTIFICATION",lastperid);
			FINDFLDSETSTRING(payppms,"PAY IDENTIFICATION",pay_id);
			LOCNRDFILE(payppms);
			if(!ADVEQLNRDsec(payppms,1,PaySelSubData,p)) prev=RDATDataNEW(payppms);
			else prev=NULL;
			FINDFLDSETCHAR(payppms,"DELETEFLAG",deleteflag);
			FINDFLDSETCHAR(payppms,"BENEFITS COMPUTED",deleteflag);
			FINDFLDSETSTRING(payppms,"SOURCE USER",USERLOGIN);
			FINDFLDSETCHAR(payppms,"MADE TRANSACTIONS",deleteflag);
			/*FINDFLDSETDOUBLE(payppms,"GROSS",per_gross);*/
			FINDFLDSETDOUBLE(payppms,"NET",per_net);
			ADVWRTTRANSsec(payppms,0,NULL,prev,PaySelSubData,p);
			if(prev!=NULL) FreeRDATData(prev);
			UNLNRDFILE(payppms);
		}
		if(p!=NULL) Rfree(p);
		if(perid!=NULL) Rfree(perid);
		if(lastperid!=NULL) Rfree(lastperid);
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
		dbsort=NULL;
		if(AUTO_LEAVE_DOCKED==TRUE)
		{
			CLSDBsort(lvesort);
			FreeDBsort(lvesort);
			lvesort=NULL;
		}
	}
	if(errorlist->numlibs<1) addAPPlib(errorlist,"No Errors Detected");
        FINDRSCLISTAPPlib(mainrsrc,"ERROR LIST",0,errorlist);
        updatersrc(mainrsrc,"ERROR LIST");
	if(warnlist->numlibs<1) addAPPlib(warnlist,"No Warnings Detected");
        FINDRSCLISTAPPlib(mainrsrc,"WARNING LIST",0,warnlist);
        updatersrc(mainrsrc,"WARNING LIST");
}
void makeDBsortfile(DBsort *dbsrt,DBsort *lvesrt)
{
	DFvirtual *d=NULL;
	NRDkey *key=NULL;
	NRDpart *part;
	NRDfield *field;
	int x=0;
	short keyno=(-1),lkeyno=(-1);

	addDBfield(dbsrt,"DELETEFLAG",BOOLNS,1);
	if(lvesrt!=NULL)
	{
		addDBfield(lvesrt,"DELETEFLAG",BOOLNS,1);
	}
	keyno=addDBkey(dbsrt,"SORT FILE MAIN KEY");
	if(lvesrt!=NULL)
	{
		lkeyno=addDBkey(lvesrt,"SORT FILE MAIN KEY");
	}
	d=getDFvirtual("PAYROLL","SELECT PAYROLL SORT SEQUENCE");
	if(d!=NULL)
	{
		csortsequence=Rmalloc(sizeof(RDAvirtual));
		csortsequence->name=stralloc(d->name);
		csortsequence->when=0;
		csortsequence->len=d->length;
		csortsequence->type=d->type;
		if(d->expression!=NULL) 
		{
			csortsequence->expression=PP_translate(d->expression);
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
		if(lvesrt!=NULL)
		{
		addDBfield(lvesrt,csortsequence->name,csortsequence->type,
			csortsequence->len);
		addDBkeypart(lvesrt,lkeyno,csortsequence->name);
		}
		FreeDFvirtual(d);
	}
	key=KEYNUM(payjprm,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			field=FLDNRD(payjprm,part->name);
			if(field!=NULL) 
			{
				addDBfield(dbsrt,field->name,field->type,field->len);
				addDBkeypart(dbsrt,keyno,part->name);
				if(lvesrt!=NULL)
				{
				addDBfield(lvesrt,field->name,field->type,field->len);
				addDBkeypart(lvesrt,keyno,part->name);
				}
			}
		}
	}
	addDBfield(dbsrt,"SELECT SEQUENCE",LONGV,4);
	if(lvesrt!=NULL)
	{
		addDBfield(lvesrt,"SELECT SEQUENCE",LONGV,4);
	}
	addDBfilefield(dbsrt,pgross,"GROSS IDENTIFICATION");
	addDBfilefield(dbsrt,payjmst,"POSITION IDENTIFICATION");
	addDBfilefield(dbsrt,payjmst,"REMARKS");
	addDBfilefield(dbsrt,posrate,"DESCRIPTION");
	addDBfilefield(dbsrt,pgrat,"BLOCK MANAGED EARNINGS");
	if(PAYROLL_SETUP->manage_earnings==TRUE)
	{
		addDBfield(dbsrt,"EARNINGS AMOUNT",DOLLARS,20);
		addDBfield(dbsrt,"PERIOD EARNINGS",DOLLARS,20);
	}
	addDBfield(dbsrt,"RATE AMOUNT",DOLLARS,20);
	addDBfield(dbsrt,"UNITS",SDECIMALV,20);
	addDBfield(dbsrt,"CHECK GROSS",DOLLARS,20);
	if(lvesrt!=NULL)
	{
	addDBfilefield(lvesrt,lvedsc,"LEAVE DESCRIPTION IDENTIFICATION");
	addDBkeypart(lvesrt,lkeyno,"LEAVE DESCRIPTION IDENTIFICATION");
	addDBfield(lvesrt,"TRANSACTION NUMBER",LONGV,6);
	addDBkeypart(lvesrt,lkeyno,"TRANSACTION NUMBER");
	}
	addDBfield(dbsrt,"NOTES",14,1200);
}
void addsortrecord(DBsort *dbsrt,RDArsrc *mainrsrc,char *perid,int jobno,
	char *payid,char *posid,char *rateid,char *grossid,double earnings,
	double period_earnings,double gross,
	double rate_amt,int seq,PassableStruct *p)
{
	char deleteflag=FALSE;

	FINDFLDSETCHAR(dbsrt->fileno,"DELETEFLAG",deleteflag);
	if(csortsequence!=NULL)
	{
		computevirtual(csortsequence,PaySelSubData,p);
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
	FINDFLDSETINT(dbsrt->fileno,"SELECT SEQUENCE",seq);
	FINDFLDSETSTRING(dbsrt->fileno,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(dbsrt->fileno,"JOB NUMBER",jobno);
	FINDFLDSETSTRING(dbsrt->fileno,"PAY IDENTIFICATION",payid);
	FINDFLDSETSTRING(dbsrt->fileno,"RATE IDENTIFICATION",rateid);
	FINDFLDSETSTRING(dbsrt->fileno,"POSITION IDENTIFICATION",posid);
	FINDFLDSETSTRING(dbsrt->fileno,"GROSS IDENTIFICATION",grossid);
	if(PAYROLL_SETUP->manage_earnings==TRUE)
	{
		FINDFLDSETDOUBLE(dbsrt->fileno,"EARNINGS",earnings);
		FINDFLDSETDOUBLE(dbsrt->fileno,"PERIOD EARNINGS",period_earnings);
	}
	FINDFLDSETDOUBLE(dbsrt->fileno,"CHECK GROSS",gross);
	FINDFLDSETDOUBLE(dbsrt->fileno,"RATE AMOUNT",rate_amt);
	COPYFIELD(pgrat,dbsrt->fileno,"BLOCK MANAGED EARNINGS");
	COPYFIELD(payjmst,dbsrt->fileno,"REMARKS");
	WRTNRD(dbsrt->fileno,0,NULL);
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

	dbsort=blist->funcargs;
	if(dbsort!=NULL)
	{
		WARNINGDIALOG("ABORT WARNING DIALOG SCREEN",
			"RECORDS NOT PROCESSED!",
			"Are you sure that you want to quit without processing the Payroll Select Records?",
			doexit,NULL,FALSE,blist);
	} else {
		doexit(blist);
	}
}
static void doexit(MakeBrowseList *blist)
{
	DBsort *dbsort=NULL;
	RDArsrc *brsrc=NULL,*srsrc=NULL,*drsrc=NULL;
	short ef=FALSE;
	char deleteflag=FALSE;
	double amt=0.0,damt=0.0;
	RDATData *prev=NULL;
	PassableStruct *p=NULL;

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
	if(dbsort!=NULL)
	{
		p=Rmalloc(sizeof(PassableStruct));
		p->r=brsrc;
		p->a=dbsort;
		p->p=NULL;
		if(AUTO_LEAVE_DOCKED==TRUE)
		{
		ef=BEGNRD(lvesort->fileno);
		while(!ef)
		{
			amt=0.0;
			FINDFLDGETCHAR(lvesort->fileno,"DELETEFLAG",&deleteflag);
			if(!deleteflag)
			{
				COPYFIELD(lvesort->fileno,lvedck,"PERSONNEL IDENTIFICATION");
				COPYFIELD(lvesort->fileno,lvedck,"JOB NUMBER");
				COPYFIELD(lvesort->fileno,lvedck,"LEAVE DESCRIPTION IDENTIFICATION");
				COPYFIELD(lvesort->fileno,lvedck,"TRANSACTION NUMBER");
				COPYFIELD(lvesort->fileno,lvedck,"PAY IDENTIFICATION");
				LOCNRDFILE(lvesort->fileno);
				if(!ADVEQLNRDsec(lvedck,1,PaySelSubData,p))
				{
					prev=RDATDataNEW(lvedck);
					FINDFLDGETDOUBLE(lvedck,"DOCK AMOUNT",&amt);
					deleteflag=TRUE;
					FINDFLDSETCHAR(lvedck,"DELETEFLAG",deleteflag);
					WRTTRANS(lvedck,0,NULL,prev);
					if(prev!=NULL) FreeRDATData(prev);
				} else KEYNRD(lvedck,1);
				UNLNRDFILE(lvedck);
				if(amt!=0.0)
				{
					COPYFIELD(lvedck,lvetrn,"PERSONNEL IDENTIFICATION");
					COPYFIELD(lvedck,lvetrn,"JOB NUMBER");
					COPYFIELD(lvedck,lvetrn,"LEAVE DESCRIPTION IDENTIFICATION");
					COPYFIELD(lvedck,lvetrn,"TRANSACTION NUMBER");
					LOCNRDFILE(lvetrn);
					if(!ADVEQLNRDsec(lvetrn,1,PaySelSubData,p))
					{
						prev=RDATDataNEW(lvetrn);
						FINDFLDGETDOUBLE(lvetrn,"DOCKED AMOUNT",&damt);
						damt-=amt;
						deleteflag=FALSE;
						if(damt==0.0)
						{
							FINDFLDSETCHAR(lvetrn,"DOCK COMPUTED",deleteflag);
						}
						FINDFLDSETCHAR(lvetrn,"DOCK COMPLETED",deleteflag);
						FINDFLDSETDOUBLE(lvetrn,"DOCKED AMOUNT",damt);
						WRTTRANS(lvetrn,0,NULL,prev);
						if(prev!=NULL) FreeRDATData(prev);
					}
					UNLNRDFILE(lvetrn);
				}
			}
			ef=SEQNRD(lvesort->fileno);
		}
		CLSDBsort(lvesort);
		FreeDBsort(lvesort);
		}
		if(p!=NULL) Rfree(p);
		CLSDBsort(dbsort);
		FreeDBsort(dbsort);
	}
	if(brsrc!=NULL)
	{
		killwindow(brsrc);
		free_rsrc(brsrc);
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
	RDArsrc *brsrc,*srsrc,*drsrc;
	DBsort *dbsort=NULL;

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
		free_rsrc(brsrc);
	}
	if(WindowCount<=2) select_backend(dbsort);
}
static short selectfunction(MakeBrowseList *b)
{
	return(TRUE);
}
static double ComputeDockAmount(PaySelStruct *ps,PassableStruct *p,
	double *total_gross,int seq,char *pay_id)
{
	double tmp=0.0,dockunits=0.0,dockvalue=0.0,dockamt=0.0,tmpdock=0.0;
	short ef=0,keyno=0;
	char *lperid=NULL,deleteflag=FALSE,computed=FALSE;
	char *exprssn=NULL,*temp=NULL,dockcompleted=FALSE,suv=FALSE;
	int ljobno=0;
	RDATData *prev=NULL;

	if(lvetrn!=(-1))
	{
		keyno=KEYNUMBER(lvetrn,"PAYSEL LVETRAN KEY");
		if(keyno<1) keyno=1;
		ZERNRD(lvetrn);
		FINDFLDSETSTRING(lvetrn,"PERSONNEL IDENTIFICATION",ps->perid);
		FINDFLDSETINT(lvetrn,"JOB NUMBER",ps->jobno);
		ef=ADVGTENRDsec(lvetrn,keyno,PaySelSubData,p);
		while(!ef)
		{
			FINDFLDGETSTRING(lvetrn,"PERSONNEL IDENTIFICATION",&lperid);
			FINDFLDGETINT(lvetrn,"JOB NUMBER",&ljobno);
			if(RDAstrcmp(ps->perid,lperid) || ps->jobno!=ljobno) break;
			FINDFLDGETCHAR(lvetrn,"DELETEFLAG",&deleteflag);
			FINDFLDGETCHAR(lvetrn,"SOURCE USER VERIFIED",&suv);
			FINDFLDGETDOUBLE(lvetrn,"DOCK UNITS",&dockunits);
			if(!deleteflag && suv && dockunits!=0.0)
			{
				FINDFLDGETCHAR(lvetrn,"DOCK COMPUTED",&computed);
				if(!computed)
				{
					COPYFIELD(lvetrn,lvedsc,"LEAVE DESCRIPTION IDENTIFICATION");
					if(!ADVEQLNRDsec(lvedsc,1,PaySelSubData,p))
					{
						FINDFLDGETSTRING(lvedsc,"LEAVE UNITS TO DOCK AMOUNT FORMULA",&exprssn);
						if(!isEMPTY(exprssn))
						{
							dockvalue=EVALUATEdbl(exprssn,PaySelSubData,p);
						} else {
							prterr("Error:  No formula LEAVE UNITS TO DOCK AMOUNT FORMULA in LVEDSC.");
							dockvalue=0.0;
						}
						if(exprssn!=NULL) Rfree(exprssn);
					} else {
						/* Unable to get Leave dock formula */
						FINDFLDGETSTRING(lvetrn,"LEAVE IDENTIFICATION",&temp);
						prterr("Error:  No LVEDSC matching this Identification [%s].",(temp!=NULL ? temp:""));
						if(temp!=NULL) Rfree(temp);
						dockvalue=0.0;
					}
					prev=RDATDataNEW(lvetrn);
					LOCNRDFILE(lvetrn);
					computed=TRUE;
					FINDFLDSETCHAR(lvetrn,"DOCK COMPUTED",computed);
					FINDFLDSETSTRING(lvetrn,"DOCK COMPUTED BY",USERLOGIN);
					temp=GETCURRENTDATE10();
					FINDFLDSETSTRING(lvetrn,"DOCK COMPUTED DATE",temp);
					if(temp!=NULL) Rfree(temp);
					temp=GETCURRENTTIME();
					FINDFLDSETSTRING(lvetrn,"DOCK COMPUTED TIME",temp);
					if(temp!=NULL) Rfree(temp);
					FINDFLDSETDOUBLE(lvetrn,"DOCK VALUE",dockvalue);
					WRTTRANS(lvetrn,0,NULL,prev);
					UNLNRDFILE(lvetrn);
					if(prev!=NULL) Rfree(prev);
				}
			}
			ef=ADVNXTNRDsec(lvetrn,keyno,PaySelSubData,p);
		}
		if(lperid!=NULL) Rfree(lperid);
		ZERNRD(lvetrn);
		FINDFLDSETSTRING(lvetrn,"PERSONNEL IDENTIFICATION",ps->perid);
		FINDFLDSETINT(lvetrn,"JOB NUMBER",ps->jobno);
		ef=ADVGTENRDsec(lvetrn,keyno,PaySelSubData,p);
		while(!ef)
		{
			FINDFLDGETSTRING(lvetrn,"PERSONNEL IDENTIFICATION",&lperid);
			FINDFLDGETINT(lvetrn,"JOB NUMBER",&ljobno);
			if(RDAstrcmp(ps->perid,lperid) || ps->jobno!=ljobno) break;
			FINDFLDGETCHAR(lvetrn,"DELETEFLAG",&deleteflag);
			FINDFLDGETCHAR(lvetrn,"DOCK COMPLETED",&dockcompleted);
			FINDFLDGETDOUBLE(lvetrn,"DOCK VALUE",&dockvalue);
			FINDFLDGETDOUBLE(lvetrn,"DOCKED AMOUNT",&dockamt);
			FINDFLDGETCHAR(lvetrn,"SOURCE USER VERIFIED",&suv);
			tmpdock=dockvalue-dockamt;
			if(dockvalue!=0.0 && suv && dockcompleted==FALSE && deleteflag==FALSE && (tmpdock!=0.0 || *total_gross>0.0))
			{
				prev=RDATDataNEW(lvetrn);
				LOCNRDFILE(lvetrn);
				if(tmpdock>=(*total_gross)) 
				{
					tmpdock=(*total_gross);
					computed=TRUE;
				} else if(tmpdock!=0)
				{
					computed=TRUE;
				} else computed=FALSE;
				if(computed && tmpdock==(dockvalue-dockamt))
				{
					FINDFLDSETCHAR(lvetrn,"DOCK COMPLETED",computed);
					FINDFLDSETSTRING(lvetrn,"DOCK COMPLETED BY",USERLOGIN);
					temp=GETCURRENTDATE10();
					FINDFLDSETSTRING(lvetrn,"DOCK COMPLETED DATE",temp);
					if(temp!=NULL) Rfree(temp);
					temp=GETCURRENTTIME();
					FINDFLDSETSTRING(lvetrn,"DOCK COMPLETED TIME",temp);
					if(temp!=NULL) Rfree(temp);
				}
				dockamt+=tmpdock;
				*total_gross-=tmpdock;
				tmp+=tmpdock;
				FINDFLDSETDOUBLE(lvetrn,"DOCKED AMOUNT",dockamt);
				WRTTRANS(lvetrn,0,NULL,prev);
				if(prev!=NULL) FreeRDATData(prev);
				ZERNRD(lvedck);
				COPYFIELD(lvetrn,lvedck,"PERSONNEL IDENTIFICATION");
				COPYFIELD(lvetrn,lvedck,"LEAVE DESCRIPTION IDENTIFICATION");
				COPYFIELD(lvetrn,lvedck,"JOB NUMBER");
				COPYFIELD(lvetrn,lvedck,"TRANSACTION NUMBER");
				FINDFLDSETSTRING(lvedck,"PAY IDENTIFICATION",pay_id);
				if(ADVEQLNRDsec(lvedck,1,PaySelSubData,p)) 
				{
					KEYNRD(lvedck,1);
					prev=NULL;
				} else {
					prev=RDATDataNEW(lvedck);
				}
				deleteflag=FALSE;
				FINDFLDSETCHAR(lvedck,"DELETEFLAG",deleteflag);
				FINDFLDSETDOUBLE(lvedck,"DOCK AMOUNT",tmpdock);
				temp=GETCURRENTDATE10();
				FINDFLDSETSTRING(lvedck,"ENTRY DATE",temp);
				if(temp!=NULL) Rfree(temp);
				temp=GETCURRENTTIME();
				FINDFLDSETSTRING(lvedck,"ENTRY TIME",temp);
				if(temp!=NULL) Rfree(temp);
				FINDFLDSETSTRING(lvedck,"SOURCE USER",USERLOGIN);
				WRTTRANS(lvedck,0,NULL,prev);
				if(prev!=NULL) FreeRDATData(prev);
				ZERNRD(lvesort->fileno);
				if(csortsequence!=NULL)
				{
					computevirtual(csortsequence,PaySelSubData,p);
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
							FINDFLDSETSTRING(lvesort->fileno,
								csortsequence->name,
								csortsequence->value.string_value);
							break;
						case BOOLNS:
						case CHARACTERS:
							FINDFLDSETCHAR(lvesort->fileno,
								csortsequence->name,
								*csortsequence->value.string_value);
							break;
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case DOLLARS:
						case SDECIMALV:
							FINDFLDSETDOUBLE(lvesort->fileno,
								csortsequence->name,
								*csortsequence->value.float_value);
							break;
						case SHORTV:
						case SSHORTV:
							FINDFLDSETSHORT(lvesort->fileno,
								csortsequence->name,
								*csortsequence->value.short_value);
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							FINDFLDSETINT(lvesort->fileno,
								csortsequence->name,
								*csortsequence->value.integer_value);
							break;
						default:
							prterr("Error virtual field type [%d] not found for field [%s].",csortsequence->type,csortsequence->name);
							break;
					}
				}
				FINDFLDSETSTRING(lvesort->fileno,
					"PERSONNEL IDENTIFICATION",ps->perid);
				FINDFLDSETINT(lvesort->fileno,"JOB NUMBER",ps->jobno);
				FINDFLDSETSTRING(lvesort->fileno,"RATE IDENTIFICATION",
					"LEAVE DOCKED");
				FINDFLDSETINT(lvesort->fileno,"SELECT SEQUENCE",seq);
				FINDFLDSETSTRING(lvesort->fileno,"PAY IDENTIFICATION",
					pay_id);
				COPYFIELD(lvedck,lvesort->fileno,
					"LEAVE DESCRIPTION IDENTIFICATION");
				COPYFIELD(lvedck,lvesort->fileno,"TRANSACTION NUMBER");
				WRTNRD(lvesort->fileno,0,NULL);
			}
			ef=ADVNXTNRDsec(lvetrn,keyno,PaySelSubData,p);
		}
		if(lperid!=NULL) Rfree(lperid);
#ifdef XXXX
		ZERNRD(lvetrn);
		FINDFLDSETSTRING(lvetrn,"PERSONNEL IDENTIFICATION",ps->perid);
		FINDFLDSETINT(lvetrn,"JOB NUMBER",ps->jobno);
		ef=ADVGTENRDsec(lvetrn,keyno,PaySelSubData,p);
		while(!ef)
		{
			FINDFLDGETSTRING(lvetrn,"PERSONNEL IDENTIFICATION",&lperid);
			FINDFLDGETINT(lvetrn,"JOB NUMBER",&ljobno);
			if(RDAstrcmp(ps->perid,lperid) || ps->jobno!=ljobno) break;
			FINDFLDGETCHAR(lvetrn,"DELETEFLAG",&deleteflag);
			FINDFLDGETCHAR(lvetrn,"DOCK COMPLETED",&dockcompleted);
			FINDFLDGETDOUBLE(lvetrn,"DOCK VALUE",&dockvalue);
			FINDFLDGETDOUBLE(lvetrn,"DOCKED AMOUNT",&dockamt);
			tmpdock=dockvalue-dockamt;
			if(dockvalue>0 && dockcompleted==FALSE && deleteflag==FALSE && (tmpdock!=0.0 || *total_gross>0.0))
			{
				prev=RDATDataNEW(lvetrn);
				LOCNRDFILE(lvetrn);
				if(tmpdock>=(*total_gross)) 
				{
					tmpdock=(*total_gross);
					computed=TRUE;
				} else if(tmpdock<0)
				{
					computed=TRUE;
				} else computed=FALSE;
				if(computed && tmpdock==(dockvalue-dockamt))
				{
					FINDFLDSETCHAR(lvetrn,"DOCK COMPLETED",computed);
					FINDFLDSETSTRING(lvetrn,"DOCK COMPLETED BY",USERLOGIN);
					temp=GETCURRENTDATE10();
					FINDFLDSETSTRING(lvetrn,"DOCK COMPLETED DATE",temp);
					if(temp!=NULL) Rfree(temp);
					temp=GETCURRENTTIME();
					FINDFLDSETSTRING(lvetrn,"DOCK COMPLETED TIME",temp);
					if(temp!=NULL) Rfree(temp);
				}
				dockamt+=tmpdock;
				*total_gross-=tmpdock;
				tmp+=tmpdock;
				FINDFLDSETDOUBLE(lvetrn,"DOCKED AMOUNT",dockamt);
				WRTTRANS(lvetrn,0,NULL,prev);
				if(prev!=NULL) FreeRDATData(prev);
				ZERNRD(lvedck);
				COPYFIELD(lvetrn,lvedck,"PERSONNEL IDENTIFICATION");
				COPYFIELD(lvetrn,lvedck,"LEAVE DESCRIPTION IDENTIFICATION");
				COPYFIELD(lvetrn,lvedck,"JOB NUMBER");
				COPYFIELD(lvetrn,lvedck,"TRANSACTION NUMBER");
				FINDFLDSETSTRING(lvedck,"PAY IDENTIFICATION",pay_id);
				if(ADVEQLNRDsec(lvedck,1,PaySelSubData,p)) 
				{
					KEYNRD(lvedck,1);
					prev=NULL;
				} else {
					prev=RDATDataNEW(lvedck);
				}
				deleteflag=FALSE;
				FINDFLDSETCHAR(lvedck,"DELETEFLAG",deleteflag);
				FINDFLDSETDOUBLE(lvedck,"DOCK AMOUNT",tmpdock);
				temp=GETCURRENTDATE10();
				FINDFLDSETSTRING(lvedck,"ENTRY DATE",temp);
				if(temp!=NULL) Rfree(temp);
				temp=GETCURRENTTIME();
				FINDFLDSETSTRING(lvedck,"ENTRY TIME",temp);
				if(temp!=NULL) Rfree(temp);
				FINDFLDSETSTRING(lvedck,"SOURCE USER",USERLOGIN);
				WRTTRANS(lvedck,0,NULL,prev);
				if(prev!=NULL) FreeRDATData(prev);
				ZERNRD(lvesort->fileno);
				if(csortsequence!=NULL)
				{
					computevirtual(csortsequence,PaySelSubData,p);
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
							FINDFLDSETSTRING(lvesort->fileno,
								csortsequence->name,
								csortsequence->value.string_value);
							break;
						case BOOLNS:
						case CHARACTERS:
							FINDFLDSETCHAR(lvesort->fileno,
								csortsequence->name,
								*csortsequence->value.string_value);
							break;
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case DOLLARS:
						case SDECIMALV:
							FINDFLDSETDOUBLE(lvesort->fileno,
								csortsequence->name,
								*csortsequence->value.float_value);
							break;
						case SHORTV:
						case SSHORTV:
							FINDFLDSETSHORT(lvesort->fileno,
								csortsequence->name,
								*csortsequence->value.short_value);
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							FINDFLDSETINT(lvesort->fileno,
								csortsequence->name,
								*csortsequence->value.integer_value);
							break;
						default:
							prterr("Error virtual field type [%d] not found for field [%s].",csortsequence->type,csortsequence->name);
							break;
					}
				}
				FINDFLDSETSTRING(lvesort->fileno,
					"PERSONNEL IDENTIFICATION",ps->perid);
				FINDFLDSETINT(lvesort->fileno,"JOB NUMBER",ps->jobno);
				FINDFLDSETSTRING(lvesort->fileno,"RATE IDENTIFICATION",
					"LEAVE DOCKED");
				FINDFLDSETINT(lvesort->fileno,"SELECT SEQUENCE",seq);
				FINDFLDSETSTRING(lvesort->fileno,"PAY IDENTIFICATION",
					pay_id);
				COPYFIELD(lvedck,lvesort->fileno,
					"LEAVE DESCRIPTION IDENTIFICATION");
				COPYFIELD(lvedck,lvesort->fileno,"TRANSACTION NUMBER");
				WRTNRD(lvesort->fileno,0,NULL);
			}
			ef=ADVNXTNRDsec(lvetrn,keyno,PaySelSubData,p);
		}
		if(lperid!=NULL) Rfree(lperid);
#endif /* XXXX */
	} else {
		prterr("Error:  LVETRN Database is unavailable.");
	}
	return(tmp);
}
static void UndoComputeDock(PaySelStruct *ps)
{
}
static void makepaystruct(RDArsrc *mainrsrc,DBsort *dbsort,RangeScreen *rs,
	PaySelStruct *ps,char *select_expression,PassableStruct *p)
{
	short ef1=0;
	char *exprssn=NULL,*rexprssn=NULL,deleteflag=FALSE,etest=FALSE;
	char *grossid1=NULL,*rateid=NULL,*uexprssn=NULL;
	char *earningsamt=NULL,*period_earnings=NULL,aflag=FALSE,use_aflag=FALSE;
	int seq=0,x;
	RDAvirtual *v=NULL;
	NRDfield *f=NULL;

	f=FLDNRD(pgrat,"ACTIVE");
	if(f!=NULL) use_aflag=TRUE;
	ZERNRD(pgrat);
	FINDFLDSETSTRING(pgrat,"GROSS IDENTIFICATION",ps->grossid);
	ef1=ADVGTENRDsec(pgrat,1,PaySelSubData,p);
	while(!ef1)
	{
		FINDFLDGETSTRING(pgrat,"GROSS IDENTIFICATION",&grossid1);
		if(RDAstrcmp(ps->grossid,grossid1)) break;
		FINDFLDGETCHAR(pgrat,"DELETEFLAG",&deleteflag);
		if(use_aflag==TRUE) FINDFLDGETCHAR(pgrat,"ACTIVE",&aflag);
		if(!deleteflag && (use_aflag==FALSE || aflag==TRUE))
		{
		FINDFLDGETSTRING(pgrat,"RATE IDENTIFICATION",&rateid);
		FINDFLDSETSTRING(posrate,"RATE IDENTIFICATION",rateid);
		if(ADVEQLNRDsec(posrate,1,PaySelSubData,p)) KEYNRD(posrate,1);
		FINDFLDGETCHAR(posrate,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			deleteflag=testfilerangersrc(posrate,rs);
			if(!isEMPTY(select_expression))
			{
				etest=EVALUATEbol(select_expression,PaySelSubData,p);
			} else etest=TRUE;
			if(etest && !deleteflag)
			{
				for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
				{
					if(ADVtestvirtualrangersrc(v,rs,(void (*)(...))PaySelSubData,p)) break;
				}
				if(x>=mainrsrc->num)
				{
					if(PAYROLL_SETUP->manage_earnings==TRUE)
					{
					FINDFLDGETSTRING(pgrat,"EARNINGS AMOUNT",&earningsamt);
					FINDFLDGETSTRING(posrate,"PERIOD EARNINGS",&period_earnings);
					} else {
						earningsamt=NULL;
						period_earnings=NULL;
					}
					FINDFLDGETSTRING(pgrat,
						"RATE AMOUNT",&rexprssn);
					FINDFLDGETSTRING(pgrat,"DEFAULT UNITS",&uexprssn);
					FINDFLDGETSTRING(posrate,
						"CHECK GROSS",&exprssn);
					FINDFLDGETINT(posrate,"SELECT SEQUENCE",&seq);
					AddRate2PaySelStruct(ps,rateid,earningsamt,period_earnings,exprssn,rexprssn,uexprssn,seq);
					if(exprssn!=NULL) Rfree(exprssn);
					if(rexprssn!=NULL) Rfree(rexprssn);
					if(earningsamt!=NULL) Rfree(earningsamt);
					if(period_earnings!=NULL) Rfree(period_earnings);
				} else {
					if(diagapps)
					{
						prterr("Skipped by Ranging on Virtual Field [%s].",(v->name!=NULL?v->name:""));
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
				prterr("Skipped: Position Rate record [POSTRK][POSRATE] Rate Identification of [%s] because of Deleted Record.",(rateid==NULL?"":rateid));KEYNRD(posrate,1);
			}
		}
		} else {
			if(diagapps)
			{
				prterr("Skipped: Position Gross Rate record [POSTRK][POSGRAT] Gross Identification of [%s] and Rate Identification of [%s] because of Deleted Record.",(ps->grossid==NULL?"":ps->grossid),(rateid==NULL?"":rateid));KEYNRD(pgrat,1);
			}
		}
		CLEAR_SCREEN_VIRTUAL(p->r);
		ef1=ADVNXTNRDsec(pgrat,1,PaySelSubData,p);
	}
	if(grossid1!=NULL) Rfree(grossid1);
}
void ComputePaySelStruct(PaySelStruct *ps,int cyear,int fyear,DBsort *dbsort,
	RDArsrc *mainrsrc,short *dodiag,PassableStruct *p)
{
	int x,y;
	RDAvirtual *v=NULL;
	RateStruct *r=NULL;
	char pgrat_selzero=FALSE,posrate_selzero=FALSE;
	double total_gross=0.0,tc_amt=0.0;
	double mg=0,mu=0;
	char *temp=NULL;
	int length=0;

	if(ps->rates!=NULL)
	{
		for(x=0,r=ps->rates;x<ps->num;++x,++r)
		{
			r->ytd_gross=0.0;r->ytd_units=0.0;
			r->ftd_gross=0.0;r->ftd_units=0.0;
			r->ftd_earnings=0.0;r->ytd_earnings=0.0;

			FINDFLDSETSTRING(payjfsm,"PERSONNEL IDENTIFICATION",ps->perid);
			FINDFLDSETINT(payjfsm,"JOB NUMBER",ps->jobno);
			FINDFLDSETINT(payjfsm,"FISCAL YEAR",fyear);

			if(ADVEQLNRDsec(payjfsm,1,PaySelSubData,p))
			{
				KEYNRD(payjfsm,1);
				if(diagapps)
				{
					prterr("DIAG APPLICATION: Cannot find Payroll Job Fiscal summary record [PAYROLL][PAYJFSM] for the keys of Personnel Identification=[%s], Job Number=[%d], Fiscal Year=[%d] and Rate Identification=[%s].",(ps->perid==NULL?"":ps->perid),ps->jobno,fyear,(r->rateid==NULL?"":r->rateid));SEENRDRECORD(payjfsm);
				}
			} else {
				if(diagapps)
				{
					prterr("DIAG APPLICATION: Found Payroll Job Fiscal summary record [PAYROLL][PAYJFSM] for the keys of Personnel Identification=[%s], Job Number=[%d], Fiscal Year=[%d] and Rate Identification=[%s].",(ps->perid==NULL?"":ps->perid),ps->jobno,fyear,(r->rateid==NULL?"":r->rateid));SEENRDRECORD(payjfsm);
				}
			}

			FINDFLDSETSTRING(payjrfs,"PERSONNEL IDENTIFICATION",ps->perid);
			FINDFLDSETINT(payjrfs,"JOB NUMBER",ps->jobno);
			FINDFLDSETINT(payjrfs,"FISCAL YEAR",fyear);
			FINDFLDSETSTRING(payjrfs,"RATE IDENTIFICATION",r->rateid);

			if(ADVEQLNRDsec(payjrfs,1,PaySelSubData,p))
			{
				KEYNRD(payjrfs,1);
				if(diagapps)
				{
					prterr("DIAG APPLICATION: Cannot find Payroll Job Rate Fiscal summary record [PAYROLL][PAYJRFS] for the keys of Personnel Identification=[%s], Job Number=[%d], Fiscal Year=[%d] and Rate Identification=[%s].",(ps->perid==NULL?"":ps->perid),ps->jobno,fyear,(r->rateid==NULL?"":r->rateid));SEENRDRECORD(payjrfs);
				}
			} else {
				if(diagapps)
				{
					prterr("DIAG APPLICATION: Found Payroll Job Rate Fiscal summary record [PAYROLL][PAYJRFS] for the keys of Personnel Identification=[%s], Job Number=[%d], Fiscal Year=[%d] and Rate Identification=[%s].",(ps->perid==NULL?"":ps->perid),ps->jobno,fyear,(r->rateid==NULL?"":r->rateid));SEENRDRECORD(payjrfs);
				}
			}
			for(y=0;y<14;++y)
			{
				length=(RDAstrlen(FISCALPERIOD[y])+20);
				temp=Rrealloc(temp,length);
				sprintf(temp,"%s GROSS",FISCALPERIOD[y]);
				FINDFLDGETDOUBLE(payjrfs,temp,&mg);
				if(diagapps)
				{
					prterr("DIAG APPLICATION: Accumulating the [%s] field of the [PAYROLL][PAYJRFS] file with a value of [$%.02f] into the present total of [$%.02f].",(temp==NULL?"":temp),(mg/100),r->ftd_gross);
				}
				r->ftd_gross+=mg;
				memset(temp,0,length);
				sprintf(temp,"%s UNITS",FISCALPERIOD[y]);
				FINDFLDGETDOUBLE(payjrfs,temp,&mu);
				if(diagapps)
				{
					prterr("DIAG APPLICATION: Accumulating the [%s] field of the [PAYROLL][PAYJRFS] file with a value of [$%.02f] into the present total of [$%.02f].",(temp==NULL?"":temp),(mu/100),r->ftd_units);
				}
				r->ftd_units+=mu;
				if(PAYROLL_SETUP->manage_earnings==TRUE)
				{
					memset(temp,0,length);
					sprintf(temp,"%s EARNINGS",FISCALPERIOD[y]);
					FINDFLDGETDOUBLE(payjrfs,temp,&mg);
					if(diagapps)
					{
						prterr("DIAG APPLICATION: Accumulating the [%s] field of the [PAYROLL][PAYJRFS] file with a value of [$%.02f] into the present total of [$%.02f].",(temp==NULL?"":temp),(mg/100),r->ftd_earnings);
					}
					r->ftd_earnings+=mg;
				}
			}

			FINDFLDSETSTRING(payjrcs,"PERSONNEL IDENTIFICATION",ps->perid);
			FINDFLDSETINT(payjrcs,"JOB NUMBER",ps->jobno);
			FINDFLDSETINT(payjrcs,"CALENDAR YEAR",cyear);
			FINDFLDSETSTRING(payjrcs,"RATE IDENTIFICATION",r->rateid);	
			if(ADVEQLNRDsec(payjrcs,1,PaySelSubData,p))
			{
				KEYNRD(payjrcs,1);
				if(diagapps)
				{
					prterr("DIAG APPLICATION: Cannot find Payroll Job Rate Calendar summary record [PAYROLL][PAYJRCS] for the keys of Personnel Identification=[%s], Job Number=[%d], Calendar Year=[%d] and Rate Identification=[%s].",(ps->perid==NULL?"":ps->perid),ps->jobno,cyear,(r->rateid==NULL?"":r->rateid));SEENRDRECORD(payjrcs);
				}
			} else {
				if(diagapps)
				{
					prterr("DIAG APPLICATION: Found Payroll Job Rate Fiscal summary record [PAYROLL][PAYJRCS] for the keys of Personnel Identification=[%s], Job Number=[%d], Calendar Year=[%d] and Rate Identification=[%s].",(ps->perid==NULL?"":ps->perid),ps->jobno,cyear,(r->rateid==NULL?"":r->rateid));SEENRDRECORD(payjrcs);
				}
			}
			for(y=0;y<12;++y)
			{
				length=(RDAstrlen(CALENDAR_MONTHS[y])+20);
				temp=Rrealloc(temp,length);
				sprintf(temp,"%s GROSS",CALENDAR_MONTHS[y]);
				FINDFLDGETDOUBLE(payjrcs,temp,&mg);
				if(diagapps)
				{
					prterr("DIAG APPLICATION: Accumulating the [%s] field of the [PAYROLL][PAYJRCS] file with a value of [$%.02f] into the present total of [$%.02f].",(temp==NULL?"":temp),(mg/100),r->ytd_gross);
				}
				r->ytd_gross+=mg;
				memset(temp,0,length);
				sprintf(temp,"%s UNITS",CALENDAR_MONTHS[y]);
				FINDFLDGETDOUBLE(payjrcs,temp,&mu);
				if(diagapps)
				{
					prterr("DIAG APPLICATION: Accumulating the [%s] field of the [PAYROLL][PAYJRCS] file with a value of [$%.02f] into the present total of [$%.02f].",(temp==NULL?"":temp),(mu/100),r->ytd_units);
				}
				r->ytd_units+=mu;
				if(PAYROLL_SETUP->manage_earnings==TRUE)
				{
					memset(temp,0,length);
					sprintf(temp,"%s EARNINGS",CALENDAR_MONTHS[y]);
					FINDFLDGETDOUBLE(payjrcs,temp,&mg);
					if(diagapps)
					{
						prterr("DIAG APPLICATION: Accumulating the [%s] field of the [PAYROLL][PAYJRCS] file with a value of [$%.02f] into the present total of [$%.02f].",(temp==NULL?"":temp),(mg/100),r->ytd_earnings);
					}
					r->ytd_earnings+=mg;
				}
			}
			if(temp!=NULL) Rfree(temp);
		}
		for(x=0,r=ps->rates;x<ps->num;++x,++r)
		{
			CLEAR_SCREEN_VIRTUAL(p->r);
			ZERNRD(dbsort->fileno);
			FINDFLDSETSTRING(pgrat,"GROSS IDENTIFICATION",ps->grossid);
			FINDFLDSETSTRING(pgrat,"RATE IDENTIFICATION",r->rateid);
			if(ADVEQLNRDsec(pgrat,1,PaySelSubData,p)) KEYNRD(pgrat,1);
			FINDFLDGETCHAR(pgrat,"SELECT ZERO",&pgrat_selzero);
			FINDFLDSETSTRING(posrate,"RATE IDENTIFICATION",r->rateid);
			if(ADVEQLNRDsec(posrate,1,PaySelSubData,p)) KEYNRD(posrate,1);
			FINDFLDGETCHAR(posrate,"SELECT ZERO",&posrate_selzero);
			FINDFLDSETSTRING(payjrcs,"PERSONNEL IDENTIFICATION",ps->perid);
			FINDFLDSETINT(payjrcs,"JOB NUMBER",ps->jobno);
			FINDFLDSETSTRING(payjrcs,"RATE IDENTIFICATION",r->rateid);
			FINDFLDSETINT(payjrcs,"CALENDAR YEAR",cyear);
			if(ADVEQLNRDsec(payjrcs,1,PaySelSubData,p)) KEYNRD(payjrcs,1);
			FINDFLDSETSTRING(payjrfs,"PERSONNEL IDENTIFICATION",ps->perid);
			FINDFLDSETINT(payjrfs,"JOB NUMBER",ps->jobno);
			FINDFLDSETSTRING(payjrfs,"RATE IDENTIFICATION",r->rateid);
			FINDFLDSETINT(payjrfs,"FISCAL YEAR",fyear);
			if(ADVEQLNRDsec(payjrfs,1,PaySelSubData,p)) KEYNRD(payjrfs,1);
			r->units=0.0;
			if(TIMECARD_TYPE!=0)
			{
				if(!strncmp(r->rateid,"HOURLY",6))
				{
					FINDFLDGETDOUBLE(paytcsum,"REGULAR HOURS",&tc_amt);
					r->units+=tc_amt;
					FINDFLDGETDOUBLE(paytcsum,"VACATION",&tc_amt);
					r->units+=tc_amt;
					FINDFLDGETDOUBLE(paytcsum,"HOLIDAY",&tc_amt);
					r->units+=tc_amt;
					FINDFLDGETDOUBLE(paytcsum,"SICK",&tc_amt);
					r->units+=tc_amt;
					FINDFLDGETDOUBLE(paytcsum,"OTHER",&tc_amt);
					r->units+=tc_amt;
				} else if(!RDAstrcmp(r->rateid,"OVERTIME-BASE"))
				{
					FINDFLDGETDOUBLE(paytcsum,"APPROVED OT",&r->units);
				} else if(!RDAstrcmp(r->rateid,"OVERTIME-ABOVE"))
				{
					FINDFLDGETDOUBLE(paytcsum,"APPROVED OT TWO",&r->units);
				} else {
				}
			} else if(!isEMPTY(r->DEFAULT_UNITS))
			{
				r->units=EVALUATEdbl(r->DEFAULT_UNITS,PaySelSubData,p);
			}
			FINDFLDSETDOUBLE(dbsort->fileno,"UNITS",r->units);
			UNITS=r->units;
			y=FINDVIRTUAL(mainrsrc,"POSGRAT LOOKUP NAME");
			if(y!=(-1))
			{
				v=mainrsrc->virflds+y;
				v->computed=FALSE;
			} else {
				prterr("Error where is POSGRAT LOOKUP NAME?");
			}
			y=FINDVIRTUAL(mainrsrc,"POSGRAT MINMAX");
			if(y!=(-1))
			{
				v=mainrsrc->virflds+y;
				v->computed=FALSE;
			} else {
				prterr("Error where is POSGRAT MINMAX?");
			}
			y=FINDVIRTUAL(mainrsrc,"POSGRAT X-AXIS NAME");
			if(y!=(-1))
			{
				v=mainrsrc->virflds+y;
				v->computed=FALSE;
			} else {
				prterr("Error where is POSGRAT X-AXIS NAME?");
			}
			if(PAYROLL_SETUP->manage_earnings==TRUE)
			{
				y=FINDVIRTUAL(mainrsrc,"PERIOD EARNINGS AMOUNT");
				if(y!=(-1))
				{
					v=mainrsrc->virflds+y;
					v->computed=FALSE;
				}
				if(!isEMPTY(r->EARNINGS_AMOUNT))
				{
					r->earnings=EVALUATEdbl(r->EARNINGS_AMOUNT,PaySelSubData,p);
				}
				EARNINGS_AMT=r->earnings;
				if(!isEMPTY(r->PERIOD_EARNINGS))
				{
					r->period_earnings=EVALUATEdbl(r->PERIOD_EARNINGS,PaySelSubData,p);
					r->period_earnings=round(r->period_earnings);
				}
				PERIOD_EARNINGS_AMT=r->period_earnings;
			}
			EARNINGS_AMT=r->earnings;
			PERIOD_EARNINGS_AMT=r->period_earnings;
	/* This line and the following 24 lines were added 03/17/2006 */
			y=FINDVIRTUAL(mainrsrc,"POSGRAT LOOKUP NAME");
			if(y!=(-1))
			{
				v=mainrsrc->virflds+y;
				v->computed=FALSE;
			} else {
				prterr("Error where is POSGRAT LOOKUP NAME?");
			}
			y=FINDVIRTUAL(mainrsrc,"POSGRAT MINMAX");
			if(y!=(-1))
			{
				v=mainrsrc->virflds+y;
				v->computed=FALSE;
			} else {
				prterr("Error where is POSGRAT MINMAX?");
			}
			y=FINDVIRTUAL(mainrsrc,"POSGRAT X-AXIS NAME");
			if(y!=(-1))
			{
				v=mainrsrc->virflds+y;
				v->computed=FALSE;
			} else {
				prterr("Error where is POSGRAT X-AXIS NAME?");
			}
			if(!isEMPTY(r->RATE_AMOUNT))
			{
				r->rate=EVALUATEdbl(r->RATE_AMOUNT,PaySelSubData,p);
			}
			RATE_AMT=r->rate;
			FINDFLDSETDOUBLE(dbsort->fileno,"RATE AMOUNT",RATE_AMT);
			y=FINDVIRTUAL(mainrsrc,"CHECK GROSS CALENDAR ADJUSTED AMOUNT");
			if(y!=(-1))
			{
				v=mainrsrc->virflds+y;
				v->computed=FALSE;
			}
			y=FINDVIRTUAL(mainrsrc,"PAY AMOUNT");
			if(y!=(-1))
			{
				v=mainrsrc->virflds+y;
				v->computed=FALSE;
			}
			y=FINDVIRTUAL(mainrsrc,"PERIOD CHECK GROSS");
			if(y!=(-1))
			{
				v=mainrsrc->virflds+y;
				v->computed=FALSE;
			}
			if(!isEMPTY(r->CHECK_GROSS))
			{
				r->gross=EVALUATEdbl(r->CHECK_GROSS,PaySelSubData,p);
				r->gross=round(r->gross);
			}
			if(r->rateid!=NULL)
			{
				if(!RDAstrcmp(r->rateid,"LEAVE DOCKED") && AUTO_LEAVE_DOCKED==TRUE)
				{
					r->gross-=ComputeDockAmount(ps,p,&total_gross,r->seq,ps->payid);
				}
			}
			CHECK_GROSS=r->gross;
			FINDFLDSETDOUBLE(dbsort->fileno,"CHECK GROSS",CHECK_GROSS);
			total_gross+=r->gross;
			if(r->gross!=0.0 || (posrate_selzero && pgrat_selzero))
			{
				*dodiag=TRUE;
				addsortrecord(dbsort,mainrsrc,ps->perid,ps->jobno,
					ps->payid,ps->posid,r->rateid,ps->grossid,
					r->earnings,r->period_earnings,
					r->gross,r->rate,r->seq,p);
			}
		}
	}
}
void selectpay(RDArsrc *mainrsrc)
{
	RDArsrc *tmprsrc=NULL,*brsrc=NULL,*srsrc=NULL,*drsrc=NULL;
	RangeScreen *rs=NULL;
	RDAvirtual *v;
	DBsort *dbsort=NULL;
	short ef=FALSE,dodiag=FALSE,testfile=FALSE;
	char *posid=NULL,*grossid=NULL,*rateid=NULL,*e=NULL;
	char deleteflag=FALSE,active=FALSE,*sortname=NULL,*perid=NULL;
	char payjpms_ok=FALSE,*posid1=NULL,computed=FALSE,madetrans=FALSE;
	char emppayable=FALSE,curemp=FALSE;
	int jobno=0,fyear=0,cyear=0,x=0;
	long size_of_file=0;
	PaySelStruct *ps=NULL;
	PassableStruct *p=NULL;
	short keyrange=FALSE;
	char scompleted=FALSE,benefitsonly=FALSE;

	printed_errorlist=FALSE;
	printed_warnlist=FALSE;
	dosortwhen=FALSE;
	if(errorlist->numlibs>0 && !RDAstrcmp(errorlist->libs[0],"No Errors Detected"))
	{
		freeapplib(errorlist);
		errorlist=APPlibNEW();
	}
	if(warnlist->numlibs>0 && !RDAstrcmp(warnlist->libs[0],"No Warnings Detected"))
	{
		freeapplib(warnlist);
		warnlist=APPlibNEW();
	}
        FINDRSCLISTAPPlib(mainrsrc,"ERROR LIST",0,errorlist);
        updatersrc(mainrsrc,"ERROR LIST");
        FINDRSCLISTAPPlib(mainrsrc,"WARNING LIST",0,warnlist);
        updatersrc(mainrsrc,"WARNING LIST");
	readallwidgets(mainrsrc);
	FINDRSCGETCHAR(mainrsrc,"PAY BALANCE OF CONTRACT",&Pay_Balance_of_Contract);
	FINDRSCGETCHAR(mainrsrc,"DUMP DEFERRED PAY BALANCE",&Dump_Deferred_Pay_Balance);
	FINDRSCGETSTRING(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",&bank_id);
	FINDRSCGETSTRING(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]",&pay_id);
	FINDRSCGETINT(mainrsrc,"[PAYMSTR][FISCAL YEAR]",&fyear);
	FINDRSCGETINT(mainrsrc,"[PAYMSTR][CALENDAR YEAR]",&cyear);
	FINDRSCGETINT(mainrsrc,"SELECT TYPE",&select_type);
	FINDRSCGETSTRING(mainrsrc,"EXPRESSION",&select_expression);
	ZERNRD(paymstr);
	FINDFLDSETSTRING(paymstr,"PAY IDENTIFICATION",pay_id);
	ADVEQLNRDsec(paymstr,1,SCRNvirtualSubData,mainrsrc);
	sortname=unique_name();
	dbsort=DBsortNEW(module,sortname,NRDRdaEngine);
	if(sortname!=NULL) Rfree(sortname);
	if(AUTO_LEAVE_DOCKED==TRUE)
	{
	sortname=ADVunique_name("l");
	lvesort=DBsortNEW(module,sortname,NRDRdaEngine);
	if(sortname!=NULL) Rfree(sortname);
	} else lvesort=NULL;
	makeDBsortfile(dbsort,lvesort);
	dbsort->fileno=OPNDBsort(dbsort);
	if(AUTO_LEAVE_DOCKED==TRUE)
	{
		lvesort->fileno=OPNDBsort(lvesort);
	}
	rs=RangeScreenNew(mainrsrc);
	ReadRangeScreen(payjmst,rs);
	ReadRangeScreen(prsnnl,rs);
	ReadRangeScreen(posmstr,rs);
	ReadRangeScreen(pgross,rs);
	ReadRangeScreen(posrate,rs);
	if(mainrsrc->virflds!=NULL)
	{
		for(x=0,v=mainrsrc->virflds;x<mainrsrc->num;++x,++v)
		{
			ReadVirtualRangeStruct(v,rs);
		}
	}
	tmprsrc=diagscrn(payjmst,"DIAGNOSTIC SCREEN",module,
		"Selecting Payroll Jobs",NULL);
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
	p->a=dbsort;
	p->p=NULL;
	ZERNRD(payjmst);
	keyrange=setfilekeyFROMrangersrc(payjmst,1,rs);
	if(keyrange) 
	{
		ef=ADVGTENRDsec(payjmst,1,PaySelSubData,p);
	} else {
		ef=ADVBEGNRDsec(payjmst,PaySelSubData,p);
	}
	while(!ef)
	{
		dodiag=FALSE;
		FINDFLDGETSTRING(payjmst,"PERSONNEL IDENTIFICATION",&perid);
		FINDFLDGETINT(payjmst,"JOB NUMBER",&jobno);
		FINDFLDGETCHAR(payjmst,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
		FINDFLDGETCHAR(payjmst,"ACTIVE",&active);
		if(active)
		{
		testfile=testfilerangersrc(payjmst,rs);
		if(!testfile)
		{
			FINDFLDSETSTRING(payppms,"PERSONNEL IDENTIFICATION",perid);
			FINDFLDSETSTRING(payppms,"PAY IDENTIFICATION",pay_id);
			if(ADVEQLNRDsec(payppms,1,PaySelSubData,p)) KEYNRD(payppms,1);
			payjpms_ok=FALSE;
			FINDFLDGETCHAR(payppms,"BENEFITS COMPUTED",&computed);
			FINDFLDGETCHAR(payppms,"SELECT COMPLETED",&scompleted);
			FINDFLDGETCHAR(payppms,"BENEFITS ONLY",&benefitsonly);
			FINDFLDSETSTRING(payjpms,"PERSONNEL IDENTIFICATION",perid);
			FINDFLDSETSTRING(payjpms,"PAY IDENTIFICATION",pay_id);
			COPYFIELD(payjmst,payjpms,"JOB NUMBER");
			if(earnings_style==1)
			{
				payjpms_ok=TRUE;
			} else {
				if(ADVEQLNRDsec(payjpms,1,PaySelSubData,p)) 
				{
					KEYNRD(payjpms,1);
					payjpms_ok=TRUE;
				} else {
					FINDFLDGETCHAR(payjpms,"DELETEFLAG",&deleteflag);
					if(deleteflag==TRUE) payjpms_ok=TRUE;
						else payjpms_ok=FALSE;
				}
			}
			FINDFLDGETCHAR(payppms,"DELETEFLAG",&deleteflag);
			if((!benefitsonly || deleteflag) && !computed && payjpms_ok==TRUE && scompleted==FALSE)
			{
			FINDFLDGETCHAR(payppms,"MADE TRANSACTIONS",&madetrans);	
			if(!madetrans)
			{
				FINDFLDSETSTRING(prsnnl,"PERSONNEL IDENTIFICATION",perid);
				if(!ADVEQLNRDsec(prsnnl,1,PaySelSubData,p))
				{
				FINDFLDGETCHAR(prsnnl,"DELETEFLAG",&deleteflag);
				ZERNRD(perstat);
				COPYFIELD(prsnnl,perstat,"STATUS TITLE");
				if(ADVEQLNRDsec(perstat,1,PaySelSubData,p)) KEYNRD(perstat,1);
				FINDFLDGETCHAR(prsnnl,"CURRENT EMPLOYEE",&curemp);
				FINDFLDGETCHAR(perstat,"EMPLOYEE PAYABLE",&emppayable);	
					
				if(!deleteflag && emppayable==TRUE && curemp==TRUE)
				{
				testfile=testfilerangersrc(prsnnl,rs);
				if(!testfile)	
				{
					if(TIMECARD_TYPE!=0)
					{
						ZERNRD(paytcsum);
						if(TIMECARD_TYPE!=3)
						{
						FIELDCOPY(prsnnl,"SOCIAL SECURITY NUMBER",paytcsum,"SOCIAL SECURITY NUMBER");
						} else {
						FIELDCOPY(prsnnl,"PERSONNEL IDENTIFICATION",paytcsum,"EMPLOYEE IDENTIFICATION");
						}
						if(ADVEQLNRDsec(paytcsum,1,PaySelSubData,p)) KEYNRD(paytcsum,1);
						FINDFLDGETCHAR(paytcsum,"DELETEFLAG",&deleteflag);
						if(deleteflag) KEYNRD(paytcsum,1);
					}
					FINDFLDGETSTRING(payjmst,"POSITION IDENTIFICATION",&posid);
					FINDFLDSETSTRING(posmstr,"POSITION IDENTIFICATION",posid);
					if(!ADVEQLNRDsec(posmstr,1,PaySelSubData,p))
					{
					FINDFLDGETCHAR(posmstr,"DELETEFLAG",&deleteflag);
					if(!deleteflag)
					{
					testfile=testfilerangersrc(posmstr,rs);
					if(!testfile)
					{
						FINDFLDGETSTRING(payjmst,"GROSS IDENTIFICATION",&grossid);
						FINDFLDSETSTRING(pgross,"GROSS IDENTIFICATION",grossid);
						if(!ADVEQLNRDsec(pgross,1,PaySelSubData,p))
						{
						FINDFLDGETCHAR(pgross,"DELETEFLAG",&deleteflag);
						if(!deleteflag)
						{
						testfile=testfilerangersrc(pgross,rs);
						if(!testfile)
						{
							ps=PaySelStructNew(perid,jobno,posid,pay_id,grossid);
							p->p=ps;
							GLOBALp->p=ps;
							makepaystruct(mainrsrc,dbsort,rs,ps,select_expression,p);
							SortRateStruct(ps);
							if(diagapps)
							{
							diageval=TRUE;diagvirtual=TRUE;TRACE;
							}
							ComputePaySelStruct(ps,cyear,fyear,dbsort,mainrsrc,&dodiag,p);
							if(diagapps)
							{
							diageval=FALSE;diagvirtual=FALSE;TRACE;
							}
							FreePaySelStruct(ps);
							p->p=NULL;
							GLOBALp->p=NULL;
						} else {
							if(diagapps)
							{
								prterr("Skipped: Position Gross record [POSTRK][POSGRS] Gross Identification of [%s] because of User Ranging.",(grossid==NULL?"":grossid));KEYNRD(pgross,1);
							}
						}
						} else {
							if(diagapps)
							{
								prterr("Skipped: Position Gross record [POSTRK][POSGRS] Gross Identification of [%s] because of Record Deleted.",(grossid==NULL?"":grossid));KEYNRD(pgross,1);
							}
							e=Rmalloc(512);
							sprintf(e,"Skipped: Position Gross record [POSTRK][POSGRS] Gross Identification of [%s] because of Record Deleted.",(grossid==NULL?"":grossid));
							addAPPlib(errorlist,e);
							if(e!=NULL) Rfree(e);
						}
						} else {
							if(diagapps)
							{
								prterr("Skipped: Position Gross record [POSTRK][POSGRS] Gross Identification of [%s] because of Record Not Found.",(grossid==NULL?"":grossid));KEYNRD(pgross,1);
							} 
							e=Rmalloc(512);
							sprintf(e,"Skipped: Position Gross record [POSTRK][POSGRS] Gross Identification of [%s] because of Record Not Found.",(grossid==NULL?"":grossid));
							addAPPlib(errorlist,e);
							if(e!=NULL) Rfree(e);
						}
					} else {
						if(diagapps)
						{
							prterr("Skipped: Position Master record [POSTRK][POSMSTR] Position Identification of [%s] because of User Ranging.",(posid==NULL?"":posid));KEYNRD(posmstr,1);
						} 
					}
					} else {
						if(diagapps)
						{
							prterr("Skipped: Position Master record [POSTRK][POSMSTR] Position Identification of [%s] because of Deleted.",(posid==NULL?"":posid));KEYNRD(posmstr,1);
						} 
						e=Rmalloc(512);
						sprintf(e,"Skipped: Position Master record [POSTRK][POSMSTR] Position Identification of [%s] because of Deleted.",(posid==NULL?"":posid));
						addAPPlib(errorlist,e);
						if(e!=NULL) Rfree(e);
					}
					} else {
						if(diagapps)
						{
							prterr("Skipped: Position Master record [POSTRK][POSMSTR] Position Identification of [%s] because of Record Not Found.",(posid==NULL?"":posid));KEYNRD(posmstr,1);
						} 
						e=Rmalloc(512);
						sprintf(e,"Skipped: Position Master record [POSTRK][POSMSTR] Position Identification of [%s] because of Record Not Found.",(posid==NULL?"":posid));
						addAPPlib(errorlist,e);
						if(e!=NULL) Rfree(e);
					}
				} else {
					if(diagapps)
					{
						prterr("Skipped: Personnel Master record [PRSNNL][PERDMG] Personnel Identification of [%s] because of User Ranging.",(perid==NULL?"":perid));KEYNRD(prsnnl,1);
					} 
				}
				} else {
					if(emppayable==FALSE)
					{
						if(diagapps)
						{
						prterr("Skipped: Personnel Master record [PRSNNL][PERDMG] Personnel Identification of [%s] because Employee Status is not PAYABLE.",(perid==NULL?"":perid));KEYNRD(prsnnl,1);
						e=Rmalloc(512);
						sprintf(e,"Skipped: Personnel Master record [PRSNNL][PERDMG] Personnel Identification of [%s] because Employee Status is not PAYABLE.",(perid==NULL?"":perid));
						addAPPlib(warnlist,e);
						if(e!=NULL) Rfree(e);
						}
					} else {
						if(diagapps)
						{
						prterr("Skipped: Personnel Master record [PRSNNL][PERDMG] Personnel Identification of [%s] because Deleted.",(perid==NULL?"":perid));KEYNRD(prsnnl,1);
						e=Rmalloc(512);
						sprintf(e,"Skipped: Personnel Master record [PRSNNL][PERDMG] Personnel Identification of [%s] because Deleted.",(perid==NULL?"":perid));KEYNRD(prsnnl,1);
						addAPPlib(warnlist,e);
						if(e!=NULL) Rfree(e);
						}
					}
				}
				} else {
					if(diagapps)
					{
						prterr("Skipped: Personnel Master record [PRSNNL][PERDMG] of Personnel Identification of [%s] because Record Not Found.",(perid==NULL?"":perid));KEYNRD(prsnnl,1);
					}
					e=Rmalloc(512);
					sprintf(e,"Skipped: Personnel Master record [PRSNNL][PERDMG] of Personnel Identification of [%s] because Record Not Found.",(perid==NULL?"":perid));
					addAPPlib(errorlist,e);
					if(e!=NULL) Rfree(e);
				}
			} else {
				if(diagapps)
				{
					prterr("Skipped: Personnel Job Master record of Personnel Identification of [%s] and Job Number of [%d] because of Previously Made Transactions in Pay Identificastion of [%s].",(perid==NULL?"":perid),jobno,(pay_id!=NULL?pay_id:""));
				}
				e=Rmalloc(512);
				sprintf(e,"Skipped: Personnel Job Master record of Personnel Identification of [%s] and Job Number of [%d] because of Previously Made Transactions in Pay Identificastion of [%s].",(perid==NULL?"":perid),jobno,(pay_id!=NULL?pay_id:""));
				addAPPlib(warnlist,e);
				if(e!=NULL) Rfree(e);
			}
			} else if(scompleted)
			{
				if(diagapps)
				{
					prterr("Skipped: Personnel Job Master record of Personnel Identification of [%s] and Job Number of [%d] because select completed has already been assigned for Pay Identification of [%s] .",(perid==NULL?"":perid),jobno,(pay_id!=NULL?pay_id:""));
				}
				e=Rmalloc(512);
				sprintf(e,"Skipped: Personnel Job Master record of Personnel Identification of [%s] and Job Number of [%d] because select completed has already been assigned for Pay Identification of [%s] .",(perid==NULL?"":perid),jobno,(pay_id!=NULL?pay_id:""));
				addAPPlib(warnlist,e);
				if(e!=NULL) Rfree(e);
			} else if(computed)
			{
				if(diagapps)
				{
					prterr("Skipped: Personnel Job Master record of Personnel Identification of [%s] and Job Number of [%d] because of Previously Computed Deductions in Pay Identification of [%s] .",(perid==NULL?"":perid),jobno,(pay_id!=NULL?pay_id:""));
				}
				e=Rmalloc(512);
				sprintf(e,"Skipped: Personnel Job Master record of Personnel Identification of [%s] and Job Number of [%d] because of Previously Computed Deductions in Pay Identification of [%s] .",(perid==NULL?"":perid),jobno,(pay_id!=NULL?pay_id:""));
				addAPPlib(warnlist,e);
				if(e!=NULL) Rfree(e);
			} else if(payjpms_ok==FALSE)
			{
				if(diagapps)
				{
					prterr("Skipped: Personnel Job Master record of Personnel Identification of [%s] and Job Number of [%d] because this job has been previously selected for for Pay Identification of [%s] .",(perid==NULL?"":perid),jobno,(pay_id!=NULL?pay_id:""));
					e=Rmalloc(512);
					sprintf(e,"Skipped: Personnel Job Master record of Personnel Identification of [%s] and Job Number of [%d] because this job has been previously selected for Pay Identification of [%s] .",(perid==NULL?"":perid),jobno,(pay_id!=NULL?pay_id:""));
					addAPPlib(warnlist,e);
					if(e!=NULL) Rfree(e);
				}
			}
		} else {
			if(diagapps)
			{
				prterr("Skipped: Personnel Job Master record of Personnel Identification of [%s] and Job Number of [%d] because of User Ranging.",(perid==NULL?"":perid),jobno);
			}
		}
		} else {
			if(diagapps)
			{
				prterr("Skipped: Personnel Job Master record of Personnel Identification of [%s] and Job Number of [%d] because of non-Active.",(perid==NULL?"":perid),jobno);
				e=Rmalloc(512);
				sprintf(e,"Skipped: Personnel Job Master record of Personnel Identification of [%s] and Job Number of [%d] because of non-Active.",(perid==NULL?"":perid),jobno);
				addAPPlib(warnlist,e);
				if(e!=NULL) Rfree(e);
			}
		}
		} else {
			if(diagapps)
			{
				prterr("Skipped: Personnel Job Master record of Personnel Identification of [%s] and Job Number of [%d] because of Deleted.",(perid==NULL?"":perid),jobno);
				e=Rmalloc(512);
				sprintf(e,"Skipped: Personnel Job Master record of Personnel Identification of [%s] and Job Number of [%d] because of Deleted.",(perid==NULL?"":perid),jobno);
				addAPPlib(warnlist,e);
				if(e!=NULL) Rfree(e);
			}
		}
		CLEAR_SCREEN_VIRTUAL(p->r);
		CLEAR_SCREEN_VIRTUAL(GLOBALp->r);
		if(tmprsrc!=NULL) update_diagnostic(tmprsrc,(dodiag ? TRUE:FALSE));
		if(keyrange)
		{
			ef=ADVNXTNRDsec(payjmst,1,PaySelSubData,p);
			if(!ef)
			{
				ef=testfilekeyTOrangersrc(payjmst,1,rs);
			}
		} else {
			ef=ADVSEQNRDsec(payjmst,PaySelSubData,p);
		}
	}
	if(posid1!=NULL) Rfree(posid1);
	if(perid!=NULL) Rfree(perid);
	if(grossid!=NULL) Rfree(grossid);
	if(rateid!=NULL) Rfree(rateid);
	if(posid!=NULL) Rfree(posid);
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	if(rs!=NULL) FreeRangeScreen(rs);
	size_of_file=SIZNRD(dbsort->fileno);
	if(size_of_file>0)
	{
		dosortwhen=TRUE;
		switch(select_type)
		{
			default:
			case 0: /* Automatic */
				select_backend(dbsort);
				break;
			case 1: /* Process Exceptions */
				brsrc=RDArsrcNEW(module,"SELECT PAYROLL BROWSE");
				srsrc=RDArsrcNEW(module,"SELECT PAYROLL SEARCH BROWSE");
				drsrc=RDArsrcNEW(module,"SELECT PAYROLL DEFINE LIST");
				addDFincvir(brsrc,module,"PAYMSTR",NULL,paymstr);
				addDFincvir(drsrc,module,"PAYJMST",NULL,payjmst);
				addDFincvir(drsrc,"PRSNNL","PERDMG",NULL,prsnnl);
				addDFincvir(drsrc,"POSTRK","POSMSTR",NULL,posmstr);
				addDFincvir(drsrc,"POSTRK","POSRATE",NULL,posrate);
				addDFincvir(drsrc,"POSTRK","POSGRAT",NULL,pgrat);
				addDFincvir(drsrc,"POSTRK","POSGRS",NULL,pgross);
				addDFincvir(drsrc,module,"PAYMSTR",NULL,paymstr);
				addDFincvir(srsrc,module,"PAYJMST",NULL,payjmst);
				addDFincvir(srsrc,"PRSNNL","PERDMG",NULL,prsnnl);
				addDFincvir(srsrc,"POSTRK","POSMSTR",NULL,posmstr);
				addDFincvir(srsrc,"POSTRK","POSGRS",NULL,pgross);
				addDFincvir(srsrc,"POSTRK","POSRATE",NULL,posrate);
				addDFincvir(srsrc,"POSTRK","POSGRAT",NULL,pgrat);
				addDFincvir(srsrc,module,"PAYMSTR",NULL,paymstr);
				GET_ALL_SCREEN_VIRTUALS(drsrc,1);
				GET_ALL_SCREEN_VIRTUALS(srsrc,2);
				SCRNvirtual2rangersrc(srsrc);
				ZERNRD(dbsort->fileno);
				addrfexrsrc(brsrc,"SAVE",TRUE,saveexit,mbl_trans);
				GLOBALp->p=NULL;
				brsrc->SubFunc=PaySelSubData;
				brsrc->SubFuncArgs=GLOBALp;
				drsrc->SubFunc=PaySelSubData;
				drsrc->SubFuncArgs=GLOBALp;
				srsrc->SubFunc=PaySelSubData;
				srsrc->SubFuncArgs=GLOBALp;
				brsrc->scn=RDAscrnNEW(brsrc->module,brsrc->screen);
				findscrnbin(brsrc->scn);
				MakeBrowseListTotals(brsrc,dbsort->fileno,drsrc);
				mbl_trans=ADVbrowse(dbsort->fileno,1,
					brsrc,srsrc,drsrc,NULL,selectfunction,
					doexit_test,okfunc,new_record,dbsort,
					TRUE,NULL,0,0,0,TRUE);
				FINDRSCSETFUNC(brsrc,"SAVE",saveexit,mbl_trans);
				break;
		}
	} else {
		ERRORDIALOG("Select Payroll Error","No records selected using the given select criteria.  Please try again.",NULL,FALSE);
		CLSDBsort(dbsort);
		FreeDBsort(dbsort);
		if(AUTO_LEAVE_DOCKED==TRUE)
		{
			CLSDBsort(lvesort);
			FreeDBsort(lvesort);
		}
	}
	if(p!=NULL) Rfree(p);
}
static void savepaymstrsp(RDArsrc *mtnrsrc)
{
	short x=0;

	FINDRSCSETCHAR(mtnrsrc,"[PAYMSTR][DELETEFLAG]",FALSE);
	updatersrc(mtnrsrc,"[PAYMSTR][DELETEFLAG]");
	x=savepaymstr(mtnrsrc);
	if(x==0)
	{
		selectpay(mtnrsrc);
	} else {
	}
}
static void test_selectpay(RDArsrc *mtnrsrc)
{
	char delflag=FALSE,historical=FALSE;
	char *payid=NULL;
	PassableStruct *p=NULL;

	CLEAR_SCREEN_VIRTUAL(mtnrsrc);
	ZERNRD(paymstr);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"[PAYMSTR][PAY IDENTIFICATION]",&payid);
	FINDFLDSETSTRING(paymstr,"PAY IDENTIFICATION",payid);
	COPYFIELD(paymstr,finyear,"FISCAL YEAR");
	if(EQLNRD(finyear,1)) KEYNRD(finyear,1);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=NULL;
	p->p=NULL;
	if(ADVEQLNRDsec(paymstr,1,PaySelSubData,p))
	{
		if(p!=NULL) Rfree(p);
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","PAY MASTER DOES NOT EXIST!",
			"Select Payroll transactions can not be added until the Pay Master has been saved.  Pay Identification entered does not presently exist in the PAYMSTR file.",
			savepaymstrsp,NULL,FALSE,mtnrsrc);
	} else {
		if(p!=NULL) Rfree(p);
		FINDFLDGETCHAR(paymstr,"DELETEFLAG",&delflag);
		FINDFLDGETCHAR(paymstr,"HISTORICAL",&historical);
		if(delflag)
		{
			WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","PAY MASTER IS FLAGGED TO BE DELETED!",
				"The Pay Identification entered is in a state of deletion in the PAYMSTR file.  The Select Payroll transactions can not be added until the Pay Master ecord is in an undeleted state.  Select SAVE to change the state of the Pay Master record to undeleted.",
				savepaymstrsp,NULL,FALSE,mtnrsrc);
		} else if(historical==TRUE)
		{
			ERRORDIALOG("HISTORICAL PAY IDENTIFICATION","Error:  The selected pay identification has been marked historical.  Create a new pay identification or choose one which is NOT historical.",NULL,FALSE); 
		} else if(isEMPTY(payid))
		{
			ERRORDIALOG("BLANK PAY IDENTIFICATION","Error:  The selected pay identification is blank.  Enter an appropriate Pay Identification.",NULL,FALSE); 
		} else if(!test_selectpay2(mtnrsrc))
		{
			selectpay(mtnrsrc);
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
		selecttype=NULL;
	}
	if(monthlist!=NULL)
	{
		for(x=0;x<12;++x) Rfree(monthlist[x]);
		Rfree(monthlist);
		monthlist=NULL;
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
			ERRORDIALOG("SELECT PAY ERRORS","There were errors detected in the select pay process.\nYou must print the error list out before you can exit this process.",NULL,FALSE);
			return;
		}
	}
	if(Force_Warning==TRUE && printed_warnlist==FALSE && warnlist->numlibs>0) 
	{
		if((warnlist->numlibs!=1) || RDAstrcmp(warnlist->libs[0],"No Warnings Detected"))
		{
			ERRORDIALOG("SELECT PAY WARNINGS","There were warnings detected in the select pay process.\nYou must print the warning list out before you can exit this process.",NULL,FALSE);
			return;
		}
	}
	if(mainrsrc!=NULL) 
	{
		killwindow(mainrsrc);
		free_rsrc(mainrsrc);
	}
	if((errorlist->numlibs!=1) || RDAstrcmp(errorlist->libs[0],"No Errors Detected"))
	{
		APPlib2SupportRequest("PAYROLL","paysel -> Select Payroll in Mass",errorlist);
	}
	if(errorlist!=NULL) freeapplib(errorlist);
	CLSNRD(payjmst);
	CLSNRD(payjprm);
	CLSNRD(payppms);
	CLSNRD(payjpms);
	CLSNRD(paymstr);
	CLSNRD(paypcsm);
	CLSNRD(paypfsm);
	CLSNRD(prsnnl);
	CLSNRD(payjrcs);
	CLSNRD(payjrfs);
	CLSNRD(posmstr);
	CLSNRD(poscat);
	CLSNRD(pgross);
	CLSNRD(pgrat);
	CLSNRD(posrate);
	CLSNRD(payjcsm);
	CLSNRD(perstat);
	CLSNRD(payjfsm);
	if(paytyp!=(-1)) CLSNRD(paytyp);
	if(paytcsum!=(-1)) CLSNRD(paytcsum);
	CLSNRD(bnkacc);
	CLSNRD(finyear);
	if(AUTO_LEAVE_DOCKED==TRUE)
	{
		CLSNRD(lvedsc);
		CLSNRD(lvemst);
		CLSNRD(lvejob);
		CLSNRD(lvetrn);
		CLSNRD(lvedck);
	}
	if(prev_paymstr!=NULL) FreeRDATData(prev_paymstr);
	if(bank_id!=NULL) Rfree(bank_id);
	if(pay_type!=NULL) Rfree(pay_type);
	if(banklist!=NULL) freeapplib(banklist);
	if(selecttype!=NULL) freeselecttype();
	if(defbankid!=NULL) Rfree(defbankid);
	if(fmgt!=NULL) free_finmgt(fmgt);
	if(DO_AUDIT && PAYROLL_SETUP->paysel_audit)
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
	if(audit_reportname!=NULL) Rfree(audit_reportname);
	if(PAYROLL_SETUP!=NULL) free_payroll(PAYROLL_SETUP);
	ShutdownSubsystems();
	exit(0);
}
static void savepaymstr_do(RDArsrc *mainrsrc)
{
	PassableStruct *p=NULL;
	char *rcddesc=NULL;

	p=Rmalloc(sizeof(PassableStruct));
	p->r=mainrsrc;
	p->a=NULL;
	p->p=NULL;
	if(ADVRECORDsec(paymstr,PaySelSubData,p))
	{
		rcddesc=ADVRECORDsecDesc(paymstr,PaySelSubData,p);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE); 
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		FINDFLDSETCHAR(paymstr,"HISTORICAL",FALSE);
		WRTTRANS(paymstr,0,NULL,prev_paymstr);
	}
	if(p!=NULL) Rfree(p);
}
static short savepaymstr_check(RDArsrc *mainrsrc)
{
	char *expression=NULL;
	char *expression_desc=NULL;
	char *desc=NULL;
	char *warning=NULL;
	short x=0;
	RDAvirtual *v;

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
			return(-1);
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
			ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","SAVE WARNING",desc,savepaymstr_do,NULL,FALSE,1,mainrsrc,NULL,NULL);
			if(expression!=NULL) Rfree(expression);
			if(expression_desc!=NULL) Rfree(expression_desc);
			if(desc!=NULL) Rfree(desc);
			return(-1);
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

			WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","PAY MASTER WRNINGS!",warning,savepaymstr_do,NULL,FALSE,mainrsrc);
			return(-1);
		}
	}
	return(0);
}
static short test_selectpay2(RDArsrc *mainrsrc)
{
	char *expression=NULL;
	char *expression_desc=NULL;
	char *desc=NULL;
	char *warning=NULL;
	short x=0;
	RDAvirtual *v;

	
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
			return(-1);
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
			ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","SAVE WARNING",desc,selectpay,NULL,FALSE,1,mainrsrc,NULL,NULL);
			if(expression!=NULL) Rfree(expression);
			if(expression_desc!=NULL) Rfree(expression_desc);
			if(desc!=NULL) Rfree(desc);
			return(-1);
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

			WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","PAY MASTER WRNINGS!",warning,selectpay,NULL,FALSE,mainrsrc);
			return(-1);
		}
	}
	return(0);
}
static void savepaymstr_front(RDArsrc *mainrsrc)
{
	CLEAR_SCREEN_VIRTUAL(mainrsrc);
	savepaymstr(mainrsrc);
}
static short savepaymstr(RDArsrc *mainrsrc)
{
	int xmonth=0,fmonth=0;
	short x=0;
	short mth=0,fmth=0;
	double zd=0.0;

	readfilewidgets(paymstr,mainrsrc);
	ReadRDAScrolledLists(mainrsrc);
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
	ReadRDAScrolledLists(mainrsrc);
	rsrc2filerecord(paymstr,mainrsrc);
	if(paytyp!=(-1))
	{
		COPYFIELD(paytyp,paymstr,"PAY TYPE");
	}
	FINDRSCGETINT(mainrsrc,"MONTHS",&xmonth);
	mth=(short)xmonth;
	FINDFLDSETSHORT(paymstr,"MONTH",mth);
	FINDRSCGETINT(mainrsrc,"FISCAL MONTHS",&fmonth);
	fmth=(short)fmonth;
	FINDFLDSETSHORT(paymstr,"FISCAL MONTH",fmth);
	x=savepaymstr_check(mainrsrc);
	if(x==0)
	{
		savepaymstr_do(mainrsrc);
		return(0);
	} else {
		return(-1);
	}
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
	short month=0,fmonth=0;
	int selmonth=0,selfmonth=0;

	ZERNRD(paymstr);
	if(ADVEQLNRDKEYsec(paymstr,1,targetkey,PaySelSubData,b->mainrsrc))
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
		FINDFLDGETSHORT(paymstr,"FISCAL MONTH",&fmonth);
		selfmonth=(int)fmonth;
		FINDRSCSETINT(b->funcargs,"FISCAL MONTHS",selfmonth);
		updatersrc(b->funcargs,"FISCAL MONTHS");
	}
	updateSCRNvirtuals(b->funcargs);
	setpayid(b->funcargs);
	if(b->definelist!=NULL) killwindow(b->definelist);
	if(b->searchrsrc!=NULL) killwindow(b->searchrsrc);
	if(b->mainrsrc!=NULL) killwindow(b->mainrsrc);
}
void browse_paymstr(RDArsrc *mainrsrc)
{
	RDArsrc *mrsrc=NULL,*srsrc=NULL,*drsrc=NULL;

	mrsrc=RDArsrcNEW(module,"SELECT PAY MASTER BROWSE");
	srsrc=RDArsrcNEW(module,"SELECT PAY MASTER SEARCH BROWSE");
	drsrc=RDArsrcNEW(module,"SELECT PAY MASTER DEFINE LIST");
	addDFincvir(drsrc,module,"PAYMSTR",NULL,paymstr);
	addDFincvir(srsrc,module,"PAYMSTR",NULL,paymstr);
	GET_SCREEN_VIRTUALS(drsrc,1);
	GET_SCREEN_VIRTUALS(srsrc,2);
	SCRNvirtual2rangersrc(srsrc);
	mrsrc->scn=RDAscrnNEW(mrsrc->module,mrsrc->screen);
	findscrnbin(mrsrc->scn);
	MakeBrowseListTotals(mrsrc,paymstr,drsrc);
/*
	xbrowse(paymstr,1,mrsrc,srsrc,drsrc,NULL,NULL,selectbpaymstr,quitbpaymstr,okbpaymstr,NULL,mainrsrc,TRUE,listfunc,0,1,__LINE__,__FILE__);
*/
	ADVSbrowse(paymstr,1,mrsrc,srsrc,drsrc,NULL,NULL,selectbpaymstr,quitbpaymstr,
		okbpaymstr,NULL,mainrsrc,TRUE,NULL,0,0,2,FALSE);
}
static void getprevpaymstcb(RDArsrc *mainrsrc)
{
	char *payid=NULL;
	int xmonth=0,fmonth=0;
	short xmtn=0,fmtn=0;
	PassableStruct *p=NULL;

	readwidget(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]");
	FINDRSCGETSTRING(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]",&payid);
	FINDFLDSETSTRING(paymstr,"PAY IDENTIFICATION",payid);
	if(payid!=NULL) Rfree(payid);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mainrsrc;
	p->a=NULL;
	p->p=NULL;
	if(ADVEQLNRDsec(paymstr,1,PaySelSubData,p)) 
	{
		KEYNRD(paymstr,1);
		if(ADVLTENRDsec(paymstr,1,PaySelSubData,p)) 
		{
			ClearRDATData(prev_paymstr);
		} else {
			GetRDATData(paymstr,prev_paymstr);
			updatefilerecord2rsrc(paymstr,mainrsrc);
			FINDFLDGETSHORT(paymstr,"MONTH",&xmtn);
			xmonth=xmtn;
			FINDRSCSETINT(mainrsrc,"MONTHS",xmonth);
			updatersrc(mainrsrc,"MONTHS");
			FINDFLDGETSHORT(paymstr,"FISCAL MONTH",&fmtn);
			fmonth=fmtn;
			FINDRSCSETINT(mainrsrc,"FISCAL MONTHS",fmonth);
			updatersrc(mainrsrc,"FISCAL MONTHS");
		}
	} else {
		ADVPRVNRDsec(paymstr,1,PaySelSubData,p);
		GetRDATData(paymstr,prev_paymstr);
		updatefilerecord2rsrc(paymstr,mainrsrc);
		FINDFLDGETSHORT(paymstr,"MONTH",&xmtn);
		xmonth=xmtn;
		FINDRSCSETINT(mainrsrc,"MONTHS",xmonth);
		updatersrc(mainrsrc,"MONTHS");
		FINDFLDGETSHORT(paymstr,"FISCAL MONTH",&fmtn);
		fmonth=fmtn;
		FINDRSCSETINT(mainrsrc,"FISCAL MONTHS",fmonth);
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
	if(p!=NULL) Rfree(p);
	updateSCRNvirtuals(mainrsrc);
}
static void getnextpaymstcb(RDArsrc *mainrsrc)
{
	char *payid=NULL;
	int xmonth=0,fmonth=0;
	short xmtn=0,fmtn=0;
	PassableStruct *p=NULL;

	readwidget(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]");
	FINDRSCGETSTRING(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]",&payid);
	FINDFLDSETSTRING(paymstr,"PAY IDENTIFICATION",payid);
	if(payid!=NULL) Rfree(payid);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mainrsrc;
	p->a=NULL;
	p->p=NULL;
	if(ADVEQLNRDsec(paymstr,1,PaySelSubData,p)) 
	{
		KEYNRD(paymstr,1);
		if(ADVGTENRDsec(paymstr,1,PaySelSubData,p)) 
		{
			ClearRDATData(prev_paymstr);
		} else {
			GetRDATData(paymstr,prev_paymstr);
			updatefilerecord2rsrc(paymstr,mainrsrc);
			FINDFLDGETSHORT(paymstr,"MONTH",&xmtn);
			xmonth=xmtn;
			FINDRSCSETINT(mainrsrc,"MONTHS",xmonth);
			updatersrc(mainrsrc,"MONTHS");
			FINDFLDGETSHORT(paymstr,"FISCAL MONTH",&fmtn);
			fmonth=fmtn;
			FINDRSCSETINT(mainrsrc,"FISCAL MONTHS",fmonth);
			updatersrc(mainrsrc,"FISCAL MONTHS");
		}
	} else {
		ADVNXTNRDsec(paymstr,1,PaySelSubData,p);
		GetRDATData(paymstr,prev_paymstr);
		updatefilerecord2rsrc(paymstr,mainrsrc);
		FINDFLDGETSHORT(paymstr,"MONTH",&xmtn);
		xmonth=xmtn;
		FINDRSCSETINT(mainrsrc,"MONTHS",xmonth);
		updatersrc(mainrsrc,"MONTHS");
		FINDFLDGETSHORT(paymstr,"FISCAL MONTH",&fmtn);
		fmonth=fmtn;
		FINDRSCSETINT(mainrsrc,"FISCAL MONTHS",fmonth);
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
	if(p!=NULL) Rfree(p);
	updateSCRNvirtuals(mainrsrc);
}
static void bankidcb(RDArsrc *mainrsrc)
{
	char delflag=FALSE;
	PassableStruct *p=NULL;

	readwidget(mainrsrc,"[BNKACC][BANK IDENTIFICATION]");
	FINDRSCGETSTRING(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",&bank_id);
	FINDFLDSETSTRING(bnkacc,"BANK IDENTIFICATION",bank_id);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mainrsrc;
	p->a=NULL;
	p->p=NULL;
	if(ADVEQLNRDsec(bnkacc,1,PaySelSubData,p))
	{
		ReadRDAScrolledLists(mainrsrc);
		if(ADVEQLNRDsec(bnkacc,1,PaySelSubData,p))
		{
			ZERNRD(bnkacc);
		}
	} else {
		FINDFLDGETCHAR(bnkacc,"DELETEFLAG",&delflag);
		if(delflag)
		{
			ReadRDAScrolledLists(mainrsrc);
			if(ADVEQLNRDsec(bnkacc,1,PaySelSubData,p))
			{
				ZERNRD(bnkacc);
			}
		}
	}
	if(p!=NULL) Rfree(p);
	updatefilerecord2rsrc(bnkacc,mainrsrc);
	updateSCRNvirtuals(mainrsrc);
}
static void paytypfunc(RDArsrc *mainrsrc)
{
	char delflag=FALSE;
	PassableStruct *p=NULL;

	p=Rmalloc(sizeof(PassableStruct));
	p->r=mainrsrc;
	p->a=NULL;
	p->p=NULL;
	if(ADVEQLNRDsec(paytyp,1,PaySelSubData,p))
	{
		ReadRDAScrolledLists(mainrsrc);
		if(ADVEQLNRDsec(paytyp,1,PaySelSubData,p))
		{
			ZERNRD(paytyp);
		}
	} else {
		FINDFLDGETCHAR(paytyp,"DELETEFLAG",&delflag);
		if(delflag)
		{
			ReadRDAScrolledLists(mainrsrc);
			if(ADVEQLNRDsec(paytyp,1,PaySelSubData,p))
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
static void resetmtndefs(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	DefaultResetScreen(mtnrsrc,TRUE);
	updateallrsrc(mtnrsrc);
	getfile(mtnrsrc,mtn);
}
static void resetmaindefs(RDArsrc *mainrsrc)
{
	DefaultResetScreen(mainrsrc,TRUE);
	updateallrsrc(mainrsrc);
	setpayid(mainrsrc);
	FINDRSCSETSTRING(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",defbankid);
	bankidcb(mainrsrc);
/*
	if(paytyp!=(-1))
	{
		paytypcb(mainrsrc);
	}
*/
}
static void setpayid(RDArsrc *mainrsrc)
{
	char *payid=NULL;
	int xmonth=0,fmonth=0;
	short xmtn=0,fmtn=0;
	PassableStruct *p=NULL;

	readwidget(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]");
	FINDRSCGETSTRING(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]",&payid);
	FINDFLDSETSTRING(paymstr,"PAY IDENTIFICATION",payid);
	if(payid!=NULL) Rfree(payid);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mainrsrc;
	p->a=NULL;
	p->p=NULL;
	if(ADVEQLNRDsec(paymstr,1,PaySelSubData,p)) 
	{
		KEYNRD(paymstr,1);
		ClearRDATData(prev_paymstr);
	} else {
		GetRDATData(paymstr,prev_paymstr);
		updatefilerecord2rsrc(paymstr,mainrsrc);
		FINDFLDGETSHORT(paymstr,"MONTH",&xmtn);
		xmonth=xmtn;
		FINDRSCSETINT(mainrsrc,"MONTHS",xmonth);
		updatersrc(mainrsrc,"MONTHS");
		FINDFLDGETSHORT(paymstr,"FISCAL MONTH",&fmtn);
		fmonth=fmtn;
		FINDRSCSETINT(mainrsrc,"FISCAL MONTHS",fmonth);
		updatersrc(mainrsrc,"FISCAL MONTHS");
	}
	if(p!=NULL) Rfree(p);
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
        if(fmgt->month>0)
        {
		if(selmonth>=fmgt->month) year+=1;
        }
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
		RDA_fprintf(fp,"%*sXPERT SELECT PAYROLL ERROR LIST          Date: %*s\r\n",24,"",8,date);
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
		RDA_fprintf(fp,"%*sXPERT SELECT PAYROLL WARNING LIST        Date: %*s\r\n",22,"",8,date);
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
static void PayselResetGroupDefaults(RDArsrc *r)
{
	ResetGroupDefaults(r);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x,xmonth=0,nofields=0,cyear=0,fyear=0;
	int start=0;
	char *name=NULL,*tp=NULL,*libx=NULL;
	NRDfield *fldx,*fields,*field=NULL;
	short edit_rsrc=TRUE;
	RDAGenericSetup *gsv=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,module,"SELECT PAYROLL")) 
	{
		return;
	}
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("PAYROLL")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"PAYROLL");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"PAYROLL");
#endif

	gsv=RDAGenericSetupNew("PAYROLL","EARNINGS STYLE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			earnings_style=*gsv->value.short_value;
		} else {
			earnings_style=FALSE;
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
	gsv=RDAGenericSetupNew("PAYROLL","FORCE WARNINGS");
	if(getRDAGenericSetupbin(libx,gsv))
	{
		Force_Warning=TRUE;
	} else {
		Force_Warning=*gsv->value.string_value;
	}

	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("PAYROLL","DISPLAY GRANT YEAR");
	if(getRDAGenericSetupbin(libx,gsv))
	{
		display_grant_year=FALSE;
	} else {
		display_grant_year=*gsv->value.string_value;
	}

	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("PAYROLL","TIME CARD SYSTEM TYPE");
	if(getRDAGenericSetupbin(libx,gsv))
	{
		TIMECARD_TYPE=0;
	} else {
		TIMECARD_TYPE=*gsv->value.short_value;
	}

	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	if(argc>1)
	{
		audit_reportname=stralloc(argv[1]);
	} else audit_reportname=stralloc("PAYSELA");
	if(XPERT_SETUP->LVEMGT==TRUE)
	{
	tp=RDA_GetEnv("SKIP_LEAVE_DOCKED");
	if(tp!=NULL)
	{
		AUTO_LEAVE_DOCKED=FALSE;
	} else AUTO_LEAVE_DOCKED=TRUE;
	} else {
		AUTO_LEAVE_DOCKED=FALSE;
	}
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("FINMGT")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"FINMGT");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"FINMGT");
#endif
        gsv=RDAGenericSetupNew("FINMGT","SWB_FM11_GRANT_MANAGEMENT");
        if(gsv!=NULL)
        {
                if(!getRDAGenericSetupbin(libx,gsv))
                {
                        use_grant_management=*gsv->value.string_value;
                } else {
                        use_grant_management=FALSE;
                }
		if(XPERT_SETUP->software_type==0) use_grant_management=TRUE;
        }
        if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	fmgt=RDAfinmgtNEW();
	getfinmgtbin(fmgt);
	PAYROLL_SETUP=RDApayrollNEW();
	getpayrollbin(PAYROLL_SETUP);
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
	if((poscat=APPOPNNRD("POSTRK","POSCAT",TRUE,FALSE))==(-1)) return;
	if((pgross=APPOPNNRD("POSTRK","POSGRS",TRUE,FALSE))==(-1)) return;
	if((pgrat=APPOPNNRD("POSTRK","POSGRAT",TRUE,FALSE))==(-1)) return;
	if((posrate=APPOPNNRD("POSTRK","POSRATE",TRUE,FALSE))==(-1)) return;
	if((payjcsm=APPOPNNRD(module,"PAYJCSM",TRUE,FALSE))==(-1)) return;
	if((payjfsm=APPOPNNRD(module,"PAYJFSM",TRUE,FALSE))==(-1)) return;
	if((paygrp=APPOPNNRD(module,"PAYGRP",TRUE,FALSE))==(-1)) return;
	if((bnkacc=APPOPNNRD("BNKREC","BNKACC",TRUE,FALSE))==(-1)) return;
	if((finyear=APPOPNNRD("FINMGT","FINYEAR",TRUE,FALSE))==(-1)) return;
	if(TIMECARD_TYPE!=0)
	{
		if((paytcsum=APPOPNNRD("PAYROLL","PAYTCSUM",TRUE,FALSE))==(-1)) return;
	}
	if(AUTO_LEAVE_DOCKED==TRUE)
	{
		if((lvedsc=APPOPNNRD("LVEMGT","LVEDSC",TRUE,FALSE))==(-1)) return;
		if((lvemst=APPOPNNRD("LVEMGT","LVEMSTR",TRUE,FALSE))==(-1)) return;
		if((lvejob=APPOPNNRD("LVEMGT","LVEJOB",TRUE,FALSE))==(-1)) return;
		if((lvetrn=APPOPNNRD("LVEMGT","LVETRAN",TRUE,TRUE))==(-1)) return;
		if((lvedck=APPOPNNRD("LVEMGT","LVEDOCK",TRUE,TRUE))==(-1)) return;
	} else {
		lvemst=(-1);
		lvejob=(-1);
		lvetrn=(-1);
		lvedck=(-1);
	}
        if((paytyp=OPNNRDsec(module,"PAYTYP",TRUE,FALSE,FALSE))==(-1))
        {
                if((paytyp=OPNNRD(module,"PAYTYP"))>(-1))
                {
                        ERRORDIALOG("Security Access Denied","Security Access denied to the [PAYTYP] file or the file cannot otherwise be opened.\n",NULL,TRUE);			ShutdownSecurity();
                        return;
                } else {
                        paytyp=(-1);
                }
        }
	mainrsrc=RDArsrcNEW(module,"SELECT PAYROLL");
	addDFincvir(mainrsrc,module,"PAYJMST",NULL,payjmst);
	addDFincvir(mainrsrc,"PRSNNL","PERDMG",NULL,prsnnl);
	addDFincvir(mainrsrc,"PRSNNL","PERSTAT",NULL,perstat);
	addDFincvir(mainrsrc,"POSTRK","POSMSTR",NULL,posmstr);
	addDFincvir(mainrsrc,"POSTRK","POSCAT",NULL,poscat);
	addDFincvir(mainrsrc,"POSTRK","POSGRS",NULL,pgross);
	addDFincvir(mainrsrc,"POSTRK","POSGRAT",NULL,pgrat);
	addDFincvir(mainrsrc,"POSTRK","POSRATE",NULL,posrate);
	addDFincvir(mainrsrc,module,"PAYJCSM",NULL,payjcsm);
	addDFincvir(mainrsrc,module,"PAYJFSM",NULL,payjfsm);
	addDFincvir(mainrsrc,module,"PAYMSTR",NULL,paymstr);
	addDFincvir(mainrsrc,module,"PAYJRCS",NULL,payjrcs);
	addDFincvir(mainrsrc,module,"PAYJRFS",NULL,payjrfs);
	addDFincvir(mainrsrc,module,"PAYPPMS",NULL,payppms);
	addDFincvir(mainrsrc,module,"PAYPCSM",NULL,paypcsm);
	addDFincvir(mainrsrc,module,"PAYPFSM",NULL,paypfsm);
	addDFincvir(mainrsrc,module,"PAYPFSM",NULL,paypfsm);
	addDFincvir(mainrsrc,module,"PAYGRP",NULL,paygrp);
	addDFincvir(mainrsrc,"FINMGT","FINYEAR",NULL,finyear);
	if(paytyp!=(-1))
	{
		addDFincvir(mainrsrc,module,"PAYTYP",NULL,paytyp);
	}
	if(AUTO_LEAVE_DOCKED==TRUE)
	{
	addDFincvir(mainrsrc,"LVEMGT","LVETRAN",NULL,lvetrn);
	addDFincvir(mainrsrc,"LVEMGT","LVEDOCK",NULL,lvedck);
	addDFincvir(mainrsrc,"LVEMGT","LVEMSTR",NULL,lvemst);
	addDFincvir(mainrsrc,"LVEMGT","LVEJOB",NULL,lvejob);
	addDFincvir(mainrsrc,"LVEMGT","LVEDSC",NULL,lvedsc);
	}
	addDFincvir(mainrsrc,"BNKREC","BNKACC",NULL,bnkacc);
	mainrsrc->scn=RDAscrnNEW(mainrsrc->module,mainrsrc->screen);
	findscrnbin(mainrsrc->scn);
	GET_ALL_SCREEN_VIRTUALS(mainrsrc,2);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	makeselecttype();
	file2rangersrc(payjmst,mainrsrc);
	file2rangersrc(prsnnl,mainrsrc);
	file2rangersrc(posmstr,mainrsrc);
	file2rangersrc(pgross,mainrsrc);
	file2rangersrc(posrate,mainrsrc);
	addstdrsrc(mainrsrc,"PAY BALANCE OF CONTRACT",BOOLNS,1,&Pay_Balance_of_Contract,TRUE);
	addstdrsrc(mainrsrc,"DUMP DEFERRED PAY BALANCE",BOOLNS,1,&Dump_Deferred_Pay_Balance,TRUE);
	if(paytyp!=(-1))
	{
		file2rsrc(paytyp,mainrsrc,FALSE);
	}
	file2rsrc(bnkacc,mainrsrc,FALSE);
	FINDRSCSETEDITABLE(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",TRUE);
	if(paytyp!=(-1))
	{
		FINDRSCSETEDITABLE(mainrsrc,"[PAYTYP][PAY TYPE]",TRUE);
	}
	addlstrsrc(mainrsrc,"SELECT TYPE",&select_type,TRUE,NULL,2,
		&selecttype,NULL);
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
	if(name!=NULL) Rfree(name);
	prev_paymstr=RDATDataNEW(paymstr);
	xmonth=val(&CURRENT_DATE[0],2)-1;
	cyear=GETCURRENTCALENDARYEAR();
	if(fmgt->month!=0)
	{
		if(xmonth>=fmgt->month) fyear=cyear+1;
	 	else fyear=cyear;
	} else fyear=cyear;
	FINDRSCSETINT(mainrsrc,"[PAYMSTR][CALENDAR YEAR]",cyear);
	FINDRSCSETINT(mainrsrc,"[PAYMSTR][FISCAL YEAR]",fyear);
	if(display_grant_year && use_grant_management)
	{
		FINDRSCSETINT(mainrsrc,"[PAYMSTR][GRANT YEAR]",fyear);
	} else {
		FINDRSCSETINT(mainrsrc,"[PAYMSTR][GRANT YEAR]",0);
	}
	field=FLDNRD(paymstr,"PAY DATE");
	if(field->len==8)
	{
		FINDRSCSETSTRING(mainrsrc,"[PAYMSTR][PAY DATE]",CURRENT_DATE);
	} else {
		FINDRSCSETSTRING(mainrsrc,"[PAYMSTR][PAY DATE]",CURRENT_DATE10);
	}
	FINDRSCSETFUNC(mainrsrc,"[PAYMSTR][PAY IDENTIFICATION]",setpayid,NULL);
	FINDRSCSETFUNC(mainrsrc,"[BNKACC][BANK IDENTIFICATION]",bankidcb,NULL);
	if(paytyp!=(-1))
	{
		FINDRSCSETFUNC(mainrsrc,"[PAYTYP][PAY TYPE]",paytypcb,NULL);
	}
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
	if(paytyp!=(-1))
	{
		FINDRSCGETSTRING(mainrsrc,"[PAYTYP][PAY TYPE]",&pay_type);
		FINDFLDSETSTRING(paytyp,"PAY TYPE",pay_type);
	}
	GLOBALp=Rmalloc(sizeof(PassableStruct));
	GLOBALp->r=mainrsrc;
	GLOBALp->a=NULL;
	GLOBALp->p=NULL;
	if(ADVEQLNRDsec(bnkacc,1,PaySelSubData,GLOBALp)) KEYNRD(bnkacc,1);
	if(paytyp!=(-1))
	{
		if(ADVEQLNRDsec(paytyp,1,PaySelSubData,GLOBALp)) KEYNRD(paytyp,1);
	}
	mainrsrc->SubFunc=PaySelSubData;
	mainrsrc->SubFuncArgs=GLOBALp;
	filerecord2rsrc(bnkacc,mainrsrc);
	if(paytyp!=(-1))
	{
		filerecord2rsrc(paytyp,mainrsrc);
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

	addbtnrsrc(mainrsrc,"LOAD VALUE SAVE WARNING",TRUE,loadvalueB,"SAVE WARNING");
	addbtnrsrc(mainrsrc,"LOAD OPERATOR SAVE WARNING",TRUE,loadoperand,"SAVE WARNING");
	addbtnrsrc(mainrsrc,"LOAD GROUPER SAVE WARNING",TRUE,loadgrouper,"SAVE WARNING");
	addbtnrsrc(mainrsrc,"LOAD CONTROL SAVE WARNING",TRUE,loadcontrol,"SAVE WARNING");

	addbtnrsrc(mainrsrc,"LOAD VALUE SAVE WARNING DESCRIPTION",TRUE,loadvalueB,"SAVE WARNING DESCRIPTION");
	addbtnrsrc(mainrsrc,"LOAD OPERATOR SAVE WARNING DESCRIPTION",TRUE,loadoperand,"SAVE WARNING DESCRIPTION");
	addbtnrsrc(mainrsrc,"LOAD GROUPER SAVE WARNING DESCRIPTION",TRUE,loadgrouper,"SAVE WARNING DESCRIPTION");
	addbtnrsrc(mainrsrc,"LOAD CONTROL SAVE WARNING DESCRIPTION",TRUE,loadcontrol,"SAVE WARNING DESCRIPTION");

	addbtnrsrc(mainrsrc,"LOAD VALUE SAVE ERROR",TRUE,loadvalueB,"SAVE ERROR");
	addbtnrsrc(mainrsrc,"LOAD OPERATOR SAVE ERROR",TRUE,loadoperand,"SAVE ERROR");
	addbtnrsrc(mainrsrc,"LOAD GROUPER SAVE ERROR",TRUE,loadgrouper,"SAVE ERROR");
	addbtnrsrc(mainrsrc,"LOAD CONTROL SAVE ERROR",TRUE,loadcontrol,"SAVE ERROR");

	addbtnrsrc(mainrsrc,"LOAD VALUE SAVE ERROR DESCRIPTION",TRUE,loadvalueB,"SAVE ERROR DESCRIPTION");
	addbtnrsrc(mainrsrc,"LOAD OPERATOR SAVE ERROR DESCRIPTION",TRUE,loadoperand,"SAVE ERROR DESCRIPTION");
	addbtnrsrc(mainrsrc,"LOAD GROUPER SAVE ERROR DESCRIPTION",TRUE,loadgrouper,"SAVE ERROR DESCRIPTION");
	addbtnrsrc(mainrsrc,"LOAD CONTROL SAVE ERROR DESCRIPTION",TRUE,loadcontrol,"SAVE ERROR DESCRIPTION");

	addbtnrsrc(mainrsrc,"PREVIOUS",TRUE,getprevpaymstcb,NULL);
	addbtnrsrc(mainrsrc,"NEXT",TRUE,getnextpaymstcb,NULL);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mainrsrc,"RESET GROUP DEFAULTS",TRUE,PayselResetGroupDefaults,NULL);
	addbtnrsrc(mainrsrc,"RESET DEFAULTS",TRUE,resetmaindefs,NULL);
	addbtnrsrc(mainrsrc,"LOAD PAYMSTR",TRUE,browse_paymstr,NULL);
	addbtnrsrc(mainrsrc,"SAVE",TRUE,savepaymstr_front,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,test_selectpay,NULL);
	addbtnrsrc(mainrsrc,"QUIT",TRUE,quitselect,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mainrsrc);
	APPmakescrn(mainrsrc,TRUE,quitselect,NULL,TRUE);
	ADVcomputeandupdateallSCRNvirtuals(mainrsrc,SCRNvirtualSubData,mainrsrc,TRUE);
}
static void setfiles(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2singlefilerecord(mtn->dbsort->fileno,mtnrsrc);
	getsupportingfiles(mtnrsrc,mtn,TRUE);
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,PaySelSubData,mtn->p,TRUE);
}
static void getposition(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *pos=NULL;
	PassableStruct *p=NULL;

	readwidget(mtnrsrc,"POSITION IDENTIFICATION");
	FINDRSCGETSTRING(mtnrsrc,"POSITION IDENTIFICATION",&pos);
	FINDFLDSETSTRING(posmstr,"POSITION IDENTIFICATION",pos);
	if(mtn!=NULL) p=mtn->p;
	if(ADVEQLNRDsec(posmstr,1,PaySelSubData,p)) KEYNRD(posmstr,1);
	if(pos!=NULL) Rfree(pos);
	updatefilerecord2rsrc(posmstr,mtnrsrc);
	ADVupdateSCRNvirtuals(mtnrsrc,PaySelSubData,p);
}
static void getgross(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *pos=NULL,*rat=NULL,*perid=NULL;
	int jobno=0,fyear=0,cyear=0;
	PassableStruct *p=NULL;

	if(mtn!=NULL) p=mtn->p;
	readwidget(mtnrsrc,"GROSS IDENTIFICATION");
	readwidget(mtnrsrc,"RATE IDENTIFICATION");
	readwidget(mtnrsrc,"PERSONNEL IDENTIFICATION");
	readwidget(mtnrsrc,"JOB NUMBER");
	readwidget(mtnrsrc,"RATE IDENTIFICATION");
	readwidget(mtnrsrc,"[PAYMSTR][FISCAL YEAR]");
	readwidget(mtnrsrc,"[PAYMSTR][CALENDAR YEAR]");
	FINDRSCGETSTRING(mtnrsrc,"GROSS IDENTIFICATION",&pos);
	FINDFLDSETSTRING(pgross,"GROSS IDENTIFICATION",pos);
	if(ADVEQLNRDsec(pgross,1,PaySelSubData,p)) KEYNRD(pgross,1);
	updatefilerecord2rsrc(pgross,mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"RATE IDENTIFICATION",&rat);
	FINDFLDSETSTRING(posrate,"RATE IDENTIFICATION",rat);
	if(ADVEQLNRDsec(posrate,1,PaySelSubData,p)) KEYNRD(posrate,1);
	updatefilerecord2rsrc(posrate,mtnrsrc);
	FINDFLDSETSTRING(pgrat,"GROSS IDENTIFICATION",pos);
	FINDFLDSETSTRING(pgrat,"RATE IDENTIFICATION",rat);
	if(ADVEQLNRDsec(pgrat,1,PaySelSubData,p)) KEYNRD(pgrat,1);
	updatefilerecord2rsrc(pgrat,mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"PERSONNEL IDENTIFICATION",&perid);
	FINDRSCGETINT(mtnrsrc,"JOB NUMBER",&jobno);
	FINDFLDSETSTRING(payjrcs,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(payjrcs,"JOB NUMBER",jobno);
	FINDFLDSETSTRING(payjrcs,"RATE IDENTIFICATION",rat);
	FINDRSCGETINT(mtnrsrc,"[PAYMSTR][CALENDAR YEAR]",&cyear);
	FINDFLDSETINT(payjrcs,"CALENDAR YEAR",cyear);
	if(ADVEQLNRDsec(payjrcs,1,PaySelSubData,p)) KEYNRD(payjrcs,1);
	updatefilerecord2rsrc(payjrcs,mtnrsrc);
	FINDFLDSETSTRING(payjrfs,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(payjrfs,"JOB NUMBER",jobno);
	FINDFLDSETSTRING(payjrfs,"RATE IDENTIFICATION",rat);
	FINDRSCGETINT(mtnrsrc,"[PAYMSTR][FISCAL YEAR]",&fyear);
	FINDFLDSETINT(payjrfs,"FISCAL YEAR",fyear);
	if(ADVEQLNRDsec(payjrfs,1,PaySelSubData,p)) KEYNRD(payjrfs,1);
	updatefilerecord2rsrc(payjrfs,mtnrsrc);
	if(perid!=NULL) Rfree(perid);
	if(pos!=NULL) Rfree(pos);
	if(rat!=NULL) Rfree(rat);
	ADVupdateSCRNvirtuals(mtnrsrc,PaySelSubData,p);
}

#ifdef XXX
static void getnewunits(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	double units=0.0;
	char *exprssn=NULL,*earn_exp=NULL,*per_earn_exp=NULL;
	PassableStruct *p=NULL;
	int x;
	RDAvirtual *v;

	readwidget(mtnrsrc,"UNITS");
	FINDRSCGETDOUBLE(mtnrsrc,"UNITS",&units);
	FINDFLDSETDOUBLE(mtn->dbsort->fileno,"UNITS",units);
	UNITS=units;
	FINDFLDGETSTRING(pgrat,"RATE AMOUNT",&exprssn);
	EARNINGS_AMT=0.0;
	PERIOD_EARNINGS_AMT=0.0;
	RATE_AMT=0.0;
	if(mtn!=NULL) p=mtn->p;
	if(PAYROLL_SETUP->manage_earnings==TRUE)
	{
		x=FINDVIRTUAL(mtnrsrc,"PERIOD EARNINGS AMOUNT");
		if(x!=(-1))
		{
			v=mtnrsrc->virflds+x;
			v->computed=FALSE;
		}
		FINDFLDGETSTRING(pgrat,"EARNINGS AMOUNT",&earn_exp);
		if(!isEMPTY(earn_exp))
		{
			EARNINGS_AMT=EVALUATEdbl(earn_exp,PaySelSubData,p);	
		}
		if(earn_exp!=NULL) Rfree(earn_exp);
		FINDRSCGETSTRING(mtnrsrc,"[POSRATE][PERIOD EARNINGS]",&per_earn_exp);
		if(!isEMPTY(per_earn_exp))
		{
			PERIOD_EARNINGS_AMT=EVALUATEdbl(per_earn_exp,PaySelSubData,p);
		}
		if(per_earn_exp!=NULL) Rfree(per_earn_exp);
		FINDFLDSETDOUBLE(mtn->dbsort->fileno,"EARNINGS",EARNINGS_AMT);
		FINDFLDSETDOUBLE(mtn->dbsort->fileno,"PERIOD EARNINGS",PERIOD_EARNINGS_AMT);
	}
	if(!isEMPTY(exprssn))
	{
		RATE_AMT=EVALUATEdbl(exprssn,PaySelSubData,p);
		Rfree(exprssn);
	}
	FINDFLDSETDOUBLE(mtn->dbsort->fileno,"RATE AMOUNT",RATE_AMT);
	FINDRSCSETDOUBLE(mtnrsrc,"RATE AMOUNT",RATE_AMT);
	FINDRSCGETSTRING(mtnrsrc,"[POSRATE][CHECK GROSS]",&exprssn);
	x=FINDVIRTUAL(mtnrsrc,"PAY AMOUNT");
	if(x!=(-1))
	{
		v=mtnrsrc->virflds+x;
		v->computed=FALSE;
	}
	x=FINDVIRTUAL(mtnrsrc,"PERIOD CHECK GROSS");
	if(x!=(-1))
	{
		v=mtnrsrc->virflds+x;
		v->computed=FALSE;
	}
	if(!isEMPTY(exprssn))
	{
		CHECK_GROSS=EVALUATEdbl(exprssn,PaySelSubData,p);
		CHECK_GROSS=round(CHECK_GROSS);
		Rfree(exprssn);
	}
	FINDFLDSETDOUBLE(mtn->dbsort->fileno,"CHECK GROSS",CHECK_GROSS);
	FINDRSCSETDOUBLE(mtnrsrc,"CHECK GROSS",CHECK_GROSS);
	updatersrc(mtnrsrc,"UNITS");
	updatersrc(mtnrsrc,"CHECK GROSS");
	updatersrc(mtnrsrc,"RATE AMOUNT");
	if(PAYROLL_SETUP->manage_earnings==TRUE)
	{
		updatersrc(mtnrsrc,"EARNINGS");
		updatersrc(mtnrsrc,"PERIOD EARNINGS");
	}
	x=FINDVIRTUAL(mtnrsrc,"PAY AMOUNT");
	if(x!=(-1))
	{
		v=mtnrsrc->virflds+x;
		FINDRSCSETDOUBLE(mtnrsrc,"PAY AMOUNT",*v->value.float_value);
	}
	x=FINDVIRTUAL(mtnrsrc,"PERIOD CHECK GROSS");
	if(x!=(-1))
	{
		v=mtnrsrc->virflds+x;
		v->computed=FALSE;
	}
	ADVupdateSCRNvirtuals(mtnrsrc,PaySelSubData,p);
}
#endif /* XXXX */
static void getnewearnings(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	double units=0.0;
	char *exprssn=NULL,*per_earn_exp=NULL;
	PassableStruct *p=NULL;
	int x;
	RDAvirtual *v;

	readwidget(mtnrsrc,"EARNINGS");
	readwidget(mtnrsrc,"UNITS");
	FINDRSCGETDOUBLE(mtnrsrc,"EARNINGS",&EARNINGS_AMT);
	FINDRSCGETDOUBLE(mtnrsrc,"UNITS",&units);
	FINDFLDSETDOUBLE(mtn->dbsort->fileno,"UNITS",units);
	UNITS=units;
	FINDFLDGETSTRING(pgrat,"RATE AMOUNT",&exprssn);
	PERIOD_EARNINGS_AMT=0.0;
	RATE_AMT=0.0;
	if(mtn!=NULL) p=mtn->p;
	/* This line and the following 24 lines were added 03/17/2006 */
	x=FINDVIRTUAL(mainrsrc,"POSGRAT LOOKUP NAME");
	if(x!=(-1))
	{
		v=mainrsrc->virflds+x;
		v->computed=FALSE;
	} else {
		prterr("Error where is POSGRAT LOOKUP NAME?");
	}
	x=FINDVIRTUAL(mainrsrc,"POSGRAT MINMAX");
	if(x!=(-1))
	{
		v=mainrsrc->virflds+x;
		v->computed=FALSE;
	} else {
		prterr("Error where is POSGRAT MINMAX?");
	}
	x=FINDVIRTUAL(mainrsrc,"POSGRAT X-AXIS NAME");
	if(x!=(-1))
	{
		v=mainrsrc->virflds+x;
		v->computed=FALSE;
	} else {
		prterr("Error where is POSGRAT X-AXIS NAME?");
	}
	if(PAYROLL_SETUP->manage_earnings==TRUE)
	{
		x=FINDVIRTUAL(mtnrsrc,"PERIOD EARNINGS AMOUNT");
		if(x!=(-1))
		{
			v=mtnrsrc->virflds+x;
			v->computed=FALSE;
		}
		FINDRSCGETSTRING(mtnrsrc,"[POSRATE][PERIOD EARNINGS]",&per_earn_exp);
		if(!isEMPTY(per_earn_exp))
		{
			PERIOD_EARNINGS_AMT=EVALUATEdbl(per_earn_exp,PaySelSubData,p);
		}
		if(per_earn_exp!=NULL) Rfree(per_earn_exp);
		FINDRSCSETDOUBLE(mtnrsrc,"PERIOD EARNINGS",PERIOD_EARNINGS_AMT);
		FINDFLDSETDOUBLE(mtn->dbsort->fileno,"PERIOD EARNINGS",PERIOD_EARNINGS_AMT);
		updatersrc(mtnrsrc,"PERIOD EARNINGS");
	}
	if(!isEMPTY(exprssn))
	{
		RATE_AMT=EVALUATEdbl(exprssn,PaySelSubData,p);
		Rfree(exprssn);
	}
	FINDFLDSETDOUBLE(mtn->dbsort->fileno,"RATE AMOUNT",RATE_AMT);
	FINDRSCSETDOUBLE(mtnrsrc,"RATE AMOUNT",RATE_AMT);
	FINDRSCGETSTRING(mtnrsrc,"[POSRATE][CHECK GROSS]",&exprssn);
	x=FINDVIRTUAL(mtnrsrc,"PAY AMOUNT");
	if(x!=(-1))
	{
		v=mtnrsrc->virflds+x;
		v->computed=FALSE;
	}
	x=FINDVIRTUAL(mtnrsrc,"PERIOD CHECK GROSS");
	if(x!=(-1))
	{
		v=mtnrsrc->virflds+x;
		v->computed=FALSE;
	}
	if(!isEMPTY(exprssn))
	{
		CHECK_GROSS=EVALUATEdbl(exprssn,PaySelSubData,p);
		CHECK_GROSS=round(CHECK_GROSS);
		Rfree(exprssn);
	}
	FINDFLDSETDOUBLE(mtn->dbsort->fileno,"CHECK GROSS",CHECK_GROSS);
	FINDRSCSETDOUBLE(mtnrsrc,"CHECK GROSS",CHECK_GROSS);
	updatersrc(mtnrsrc,"UNITS");
	updatersrc(mtnrsrc,"CHECK GROSS");
	updatersrc(mtnrsrc,"RATE AMOUNT");
	if(PAYROLL_SETUP->manage_earnings==TRUE)
	{
		updatersrc(mtnrsrc,"PERIOD EARNINGS");
	}
	x=FINDVIRTUAL(mtnrsrc,"PAY AMOUNT");
	if(x!=(-1))
	{
		v=mtnrsrc->virflds+x;
		FINDRSCSETDOUBLE(mtnrsrc,"PAY AMOUNT",*v->value.float_value);
	}
	x=FINDVIRTUAL(mtnrsrc,"PERIOD CHECK GROSS");
	if(x!=(-1))
	{
		v=mtnrsrc->virflds+x;
		v->computed=FALSE;
	}
	/* This line and the following 24 lines were added 03/17/2006 */
	x=FINDVIRTUAL(mainrsrc,"POSGRAT LOOKUP NAME");
	if(x!=(-1))
	{
		v=mainrsrc->virflds+x;
		v->computed=FALSE;
	} else {
		prterr("Error where is POSGRAT LOOKUP NAME?");
	}
	x=FINDVIRTUAL(mainrsrc,"POSGRAT MINMAX");
	if(x!=(-1))
	{
		v=mainrsrc->virflds+x;
		v->computed=FALSE;
	} else {
		prterr("Error where is POSGRAT MINMAX?");
	}
	x=FINDVIRTUAL(mainrsrc,"POSGRAT X-AXIS NAME");
	if(x!=(-1))
	{
		v=mainrsrc->virflds+x;
		v->computed=FALSE;
	} else {
		prterr("Error where is POSGRAT X-AXIS NAME?");
	}
	ADVupdateSCRNvirtuals(mtnrsrc,PaySelSubData,p);
}
static void getnewamt(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	double units=0.0;
	char *exprssn=NULL,*per_earn_exp=NULL;
	PassableStruct *p=NULL;
	int x;
	RDAvirtual *v;

	if(PAYROLL_SETUP->manage_earnings==TRUE)
	{
		getnewearnings(mtnrsrc,mtn);
	}
	readwidget(mtnrsrc,"UNITS");
	readwidget(mtnrsrc,"RATE AMOUNT");
	FINDRSCGETDOUBLE(mtnrsrc,"UNITS",&units);
	FINDFLDSETDOUBLE(mtn->dbsort->fileno,"UNITS",units);
	UNITS=units;
	FINDRSCGETDOUBLE(mtnrsrc,"RATE AMOUNT",&RATE_AMT);
	FINDFLDSETDOUBLE(mtn->dbsort->fileno,"RATE AMOUNT",RATE_AMT);
	FINDRSCGETSTRING(mtnrsrc,"[POSRATE][CHECK GROSS]",&exprssn);
	if(mtn!=NULL) p=mtn->p;
	ADVupdateSCRNvirtuals(mtnrsrc,PaySelSubData,p);
	if(PAYROLL_SETUP->manage_earnings==TRUE)
	{
		getnewearnings(mtnrsrc,mtn);
	}
	x=FINDVIRTUAL(mtnrsrc,"PERIOD EARNINGS AMOUNT");
	if(x!=(-1))
	{
		v=mtnrsrc->virflds+x;
		v->computed=FALSE;
	}
	x=FINDVIRTUAL(mtnrsrc,"PAY AMOUNT");
	if(x!=(-1))
	{
		v=mtnrsrc->virflds+x;
		v->computed=FALSE;
	}
	x=FINDVIRTUAL(mtnrsrc,"PERIOD CHECK GROSS");
	if(x!=(-1))
	{
		v=mtnrsrc->virflds+x;
		v->computed=FALSE;
	}
	/* This line and the following 24 lines were added 03/17/2006 */
	x=FINDVIRTUAL(mainrsrc,"POSGRAT LOOKUP NAME");
	if(x!=(-1))
	{
		v=mainrsrc->virflds+x;
		v->computed=FALSE;
	} else {
		prterr("Error where is POSGRAT LOOKUP NAME?");
	}
	x=FINDVIRTUAL(mainrsrc,"POSGRAT MINMAX");
	if(x!=(-1))
	{
		v=mainrsrc->virflds+x;
		v->computed=FALSE;
	} else {
		prterr("Error where is POSGRAT MINMAX?");
	}
	x=FINDVIRTUAL(mainrsrc,"POSGRAT X-AXIS NAME");
	if(x!=(-1))
	{
		v=mainrsrc->virflds+x;
		v->computed=FALSE;
	} else {
		prterr("Error where is POSGRAT X-AXIS NAME?");
	}
/* New After */
	if(PAYROLL_SETUP->manage_earnings==TRUE)
	{
		x=FINDVIRTUAL(mtnrsrc,"PERIOD EARNINGS AMOUNT");
		if(x!=(-1))
		{
			v=mtnrsrc->virflds+x;
			v->computed=FALSE;
		}
		FINDRSCGETSTRING(mtnrsrc,"[POSRATE][PERIOD EARNINGS]",&per_earn_exp);
		if(!isEMPTY(per_earn_exp))
		{
			PERIOD_EARNINGS_AMT=EVALUATEdbl(per_earn_exp,PaySelSubData,p);
		}
		if(per_earn_exp!=NULL) Rfree(per_earn_exp);
		FINDFLDSETDOUBLE(mtn->dbsort->fileno,"PERIOD EARNINGS",PERIOD_EARNINGS_AMT);
	}
/* New Before */
	if(!isEMPTY(exprssn))
	{
/* Do Not Attempt to Account for Docking of Leave Here, this is where the
   user's are manually accounting / responsible for that themselves */
/* Am aware of timing difficulties, they can be resolved though require
   another data element, like the two booleans in PURIDS for PO ID Mgt */
		CHECK_GROSS=EVALUATEdbl(exprssn,PaySelSubData,p);
		CHECK_GROSS=round(CHECK_GROSS);
		Rfree(exprssn);
	}
	FINDFLDSETDOUBLE(mtn->dbsort->fileno,"CHECK GROSS",CHECK_GROSS);
	FINDRSCSETDOUBLE(mtnrsrc,"CHECK GROSS",CHECK_GROSS);
	updatersrc(mtnrsrc,"CHECK GROSS");
	x=FINDVIRTUAL(mtnrsrc,"PAY AMOUNT");
	if(x!=(-1))
	{
		v=mtnrsrc->virflds+x;
		FINDRSCSETDOUBLE(mtnrsrc,"PAY AMOUNT",*v->value.float_value);
	}
	x=FINDVIRTUAL(mtnrsrc,"PERIOD CHECK GROSS");
	if(x!=(-1))
	{
		v=mtnrsrc->virflds+x;
		v->computed=FALSE;
	}
	/* This line and the following 24 lines were added 03/17/2006 */
	x=FINDVIRTUAL(mainrsrc,"POSGRAT LOOKUP NAME");
	if(x!=(-1))
	{
		v=mainrsrc->virflds+x;
		v->computed=FALSE;
	} else {
		prterr("Error where is POSGRAT LOOKUP NAME?");
	}
	x=FINDVIRTUAL(mainrsrc,"POSGRAT MINMAX");
	if(x!=(-1))
	{
		v=mainrsrc->virflds+x;
		v->computed=FALSE;
	} else {
		prterr("Error where is POSGRAT MINMAX?");
	}
	x=FINDVIRTUAL(mainrsrc,"POSGRAT X-AXIS NAME");
	if(x!=(-1))
	{
		v=mainrsrc->virflds+x;
		v->computed=FALSE;
	} else {
		prterr("Error where is POSGRAT X-AXIS NAME?");
	}
	updatersrc(mtnrsrc,"PAY AMOUNT");
}
static void getpreviouscb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *perid=NULL,*rateid=NULL,xediting=FALSE;
	int jobno=0;
	PassableStruct *p=NULL;

	ZERNRD(mtn->dbsort->fileno);
	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"PERSONNEL IDENTIFICATION",&perid);
	FINDRSCGETINT(mtnrsrc,"JOB NUMBER",&jobno);
	FINDRSCGETSTRING(mtnrsrc,"RATE IDENTIFICATION",&rateid);
	FINDFLDSETSTRING(mtn->dbsort->fileno,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(mtn->dbsort->fileno,"JOB NUMBER",jobno);
	FINDFLDSETSTRING(mtn->dbsort->fileno,"PAY IDENTIFICATION",pay_id);
	FINDFLDSETSTRING(mtn->dbsort->fileno,"RATE IDENTIFICATION",rateid);
	if(EQLNRD(mtn->dbsort->fileno,1))
	{
		KEYNRD(mtn->dbsort->fileno,1);
		if(LTENRD(mtn->dbsort->fileno,1))
		{
			ClearRDATData(mtn->previous);
			seteditsinglefilersrc(mtn->dbsort->fileno,1,mtnrsrc,TRUE,FALSE);
		} else {
			singlefilerecord2rsrc(mtn->dbsort->fileno,mtnrsrc);
			if(!RDAstrcmp(rateid,"LEAVE DOCKED") && AUTO_LEAVE_DOCKED==TRUE) xediting=FALSE;
				else xediting=TRUE;
			seteditsinglefilersrc(mtn->dbsort->fileno,1,mtnrsrc,xediting,FALSE);
			FINDRSCSETEDITABLE(mtnrsrc,"DELETEFLAG",TRUE);
			updatesinglefilersrc(mtn->dbsort->fileno,mtnrsrc);
			GetRDATData(mtn->dbsort->fileno,mtn->previous);
		}
	} else {
		PRVNRD(mtn->dbsort->fileno,1);
		singlefilerecord2rsrc(mtn->dbsort->fileno,mtnrsrc);
		if(!RDAstrcmp(rateid,"LEAVE DOCKED") && AUTO_LEAVE_DOCKED==TRUE) xediting=FALSE;
			else xediting=TRUE;
		seteditsinglefilersrc(mtn->dbsort->fileno,1,mtnrsrc,xediting,FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"DELETEFLAG",TRUE);
		updatesinglefilersrc(mtn->dbsort->fileno,mtnrsrc);
		GetRDATData(mtn->dbsort->fileno,mtn->previous);
	}
	FINDRSCSETEDITABLE(mtnrsrc,"CHECK GROSS",FALSE);
	GetNewRateStruct(mtn->dbsort,mtn->ps,perid,jobno,mtnrsrc);
	if(perid!=NULL) Rfree(perid);
	if(rateid!=NULL) Rfree(rateid);
	getsupportingfiles(mtnrsrc,mtn,TRUE);
	if(mtn!=NULL) p=mtn->p;
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,PaySelSubData,p,TRUE);
}
static void getnextcb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *perid=NULL,*rateid=NULL,xediting=FALSE;
	int jobno=0;
	PassableStruct *p=NULL;

	ZERNRD(mtn->dbsort->fileno);
	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"PERSONNEL IDENTIFICATION",&perid);
	FINDRSCGETINT(mtnrsrc,"JOB NUMBER",&jobno);
	FINDRSCGETSTRING(mtnrsrc,"RATE IDENTIFICATION",&rateid);
	FINDFLDSETSTRING(mtn->dbsort->fileno,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(mtn->dbsort->fileno,"JOB NUMBER",jobno);
	FINDFLDSETSTRING(mtn->dbsort->fileno,"PAY IDENTIFICATION",pay_id);
	FINDFLDSETSTRING(mtn->dbsort->fileno,"RATE IDENTIFICATION",rateid);
	if(EQLNRD(mtn->dbsort->fileno,1))
	{
		KEYNRD(mtn->dbsort->fileno,1);
		if(GTENRD(mtn->dbsort->fileno,1))
		{
			ClearRDATData(mtn->previous);
			seteditsinglefilersrc(mtn->dbsort->fileno,1,mtnrsrc,TRUE,FALSE);
		} else {
			singlefilerecord2rsrc(mtn->dbsort->fileno,mtnrsrc);
			if(!RDAstrcmp(rateid,"LEAVE DOCKED") && AUTO_LEAVE_DOCKED==TRUE) xediting=FALSE;
				else xediting=TRUE;
			seteditsinglefilersrc(mtn->dbsort->fileno,1,mtnrsrc,xediting,FALSE);
			FINDRSCSETEDITABLE(mtnrsrc,"DELETEFLAG",TRUE);
			updatesinglefilersrc(mtn->dbsort->fileno,mtnrsrc);
			GetRDATData(mtn->dbsort->fileno,mtn->previous);
		}
	} else {
		NXTNRD(mtn->dbsort->fileno,1);
		singlefilerecord2rsrc(mtn->dbsort->fileno,mtnrsrc);
		if(!RDAstrcmp(rateid,"LEAVE DOCKED") && AUTO_LEAVE_DOCKED==TRUE) xediting=FALSE;
			else xediting=TRUE;
		seteditsinglefilersrc(mtn->dbsort->fileno,1,mtnrsrc,xediting,FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"DELETEFLAG",TRUE);
		updatesinglefilersrc(mtn->dbsort->fileno,mtnrsrc);
		GetRDATData(mtn->dbsort->fileno,mtn->previous);
	}
	FINDRSCSETEDITABLE(mtnrsrc,"CHECK GROSS",FALSE);
	GetNewRateStruct(mtn->dbsort,mtn->ps,perid,jobno,mtnrsrc);
	if(perid!=NULL) Rfree(perid);
	if(rateid!=NULL) Rfree(rateid);
	getsupportingfiles(mtnrsrc,mtn,TRUE);
	if(mtn!=NULL) p=mtn->p;
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,PaySelSubData,p,TRUE);
}
static void getfile(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *perid=NULL,*rateid=NULL,xediting=FALSE;
	int jobno=0;
	PassableStruct *p=NULL;

	ZERNRD(mtn->dbsort->fileno);
	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"PERSONNEL IDENTIFICATION",&perid);
	FINDRSCGETINT(mtnrsrc,"JOB NUMBER",&jobno);
	FINDRSCGETSTRING(mtnrsrc,"RATE IDENTIFICATION",&rateid);
	FINDFLDSETSTRING(mtn->dbsort->fileno,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(mtn->dbsort->fileno,"JOB NUMBER",jobno);
	FINDFLDSETSTRING(mtn->dbsort->fileno,"PAY IDENTIFICATION",pay_id);
	FINDFLDSETSTRING(mtn->dbsort->fileno,"RATE IDENTIFICATION",rateid);
	if(EQLNRD(mtn->dbsort->fileno,1))
	{
		KEYNRD(mtn->dbsort->fileno,1);
		ClearRDATData(mtn->previous);
		seteditsinglefilersrc(mtn->dbsort->fileno,1,mtnrsrc,TRUE,FALSE);
	} else {
		singlefilerecord2rsrc(mtn->dbsort->fileno,mtnrsrc);
		if(!RDAstrcmp(rateid,"LEAVE DOCKED") && AUTO_LEAVE_DOCKED==TRUE) xediting=FALSE;
			else xediting=TRUE;
		seteditsinglefilersrc(mtn->dbsort->fileno,1,mtnrsrc,xediting,FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"DELETEFLAG",TRUE);
		updatesinglefilersrc(mtn->dbsort->fileno,mtnrsrc);
		GetRDATData(mtn->dbsort->fileno,mtn->previous);
	}
	FINDRSCSETEDITABLE(mtnrsrc,"CHECK GROSS",FALSE);
	GetNewRateStruct(mtn->dbsort,mtn->ps,perid,jobno,mtnrsrc);
	if(perid!=NULL) Rfree(perid);
	if(rateid!=NULL) Rfree(rateid);
	getsupportingfiles(mtnrsrc,mtn,TRUE);
	if(mtn!=NULL) p=mtn->p;
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,PaySelSubData,p,TRUE);
}
static void changeRATEids(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *perid=NULL,*rateid=NULL,xediting=FALSE;
	int jobno=0,sel=0;
	PassableStruct *p=NULL;

	FINDRSCGETINT(mtnrsrc,"RATE IDENTIFICATIONS",&sel);
	FINDRSCSETSTRING(mtnrsrc,"RATE IDENTIFICATION",mtn->ratesavl->libs[sel]);
	updatersrc(mtnrsrc,"RATE IDENTIFICATION");
	ZERNRD(mtn->dbsort->fileno);
	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"PERSONNEL IDENTIFICATION",&perid);
	FINDRSCGETINT(mtnrsrc,"JOB NUMBER",&jobno);
	FINDRSCGETSTRING(mtnrsrc,"RATE IDENTIFICATION",&rateid);
	FINDFLDSETSTRING(mtn->dbsort->fileno,"PAY IDENTIFICATION",pay_id);
	FINDFLDSETSTRING(mtn->dbsort->fileno,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(mtn->dbsort->fileno,"JOB NUMBER",jobno);
	FINDFLDSETSTRING(mtn->dbsort->fileno,"RATE IDENTIFICATION",rateid);
	if(EQLNRD(mtn->dbsort->fileno,1))
	{
		KEYNRD(mtn->dbsort->fileno,1);
		ClearRDATData(mtn->previous);
		seteditsinglefilersrc(mtn->dbsort->fileno,1,mtnrsrc,TRUE,FALSE);
	} else {
		if(!RDAstrcmp(rateid,"LEAVE DOCKED") && AUTO_LEAVE_DOCKED==TRUE) xediting=FALSE;
			else xediting=TRUE;
		singlefilerecord2rsrc(mtn->dbsort->fileno,mtnrsrc);
		seteditsinglefilersrc(mtn->dbsort->fileno,1,mtnrsrc,xediting,FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"DELETEFLAG",TRUE);
		updatesinglefilersrc(mtn->dbsort->fileno,mtnrsrc);
		GetRDATData(mtn->dbsort->fileno,mtn->previous);
	}
	GetNewRateStruct(mtn->dbsort,mtn->ps,perid,jobno,mtnrsrc);
	if(perid!=NULL) Rfree(perid);
	if(rateid!=NULL) Rfree(rateid);
	getsupportingfiles(mtnrsrc,mtn,TRUE);
	if(mtn!=NULL) p=mtn->p;
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,PaySelSubData,p,TRUE);
}
void selectm(short dowhich,void *targetkey,int argc,char **argv,DBsort *t)
{
	char *perid=NULL,*rateid=NULL,xediting=FALSE;
	int x=0,jobno=0;
	int which_type=0,sel=0;
	NRDfield *fields=NULL,*fldx=NULL;
	short nofields=0;
	RDAvirtual *v;
	RDArsrc *mtnrsrc=NULL;
	PassableStruct *p=NULL;
	mtnstruct *mtn=NULL;

	mtnrsrc=RDArsrcNEW(module,"SELECT PAYROLL MAINTAIN SCREEN");
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->dbsort=t;
/* Hoping not to have to implement all parts of PaySelStruct, just 
   RateStructs??????? */
	ZERNRD(t->fileno);
	mtn->ps=PaySelStructNew(NULL,0,NULL,NULL,NULL); /* GET ARGS */
	mtn->previous=RDATDataNEW(t->fileno);
	addDFincvir(mtnrsrc,"SECURITY","USERS",NULL,t->fileno); 
	addDFincvir(mtnrsrc,module,"PAYJMST",NULL,payjmst);
	addDFincvir(mtnrsrc,"PRSNNL","PERDMG",NULL,prsnnl);
	addDFincvir(mtnrsrc,"POSTRK","POSMSTR",NULL,posmstr);
	addDFincvir(mtnrsrc,"POSTRK","POSGRS",NULL,pgross);
	addDFincvir(mtnrsrc,"POSTRK","POSGRAT",NULL,pgrat);
	addDFincvir(mtnrsrc,"POSTRK","POSRATE",NULL,posrate);
	addDFincvir(mtnrsrc,module,"PAYJCSM",NULL,payjcsm);
	addDFincvir(mtnrsrc,module,"PAYJFSM",NULL,payjfsm);
	addDFincvir(mtnrsrc,module,"PAYMSTR",NULL,paymstr);
	addDFincvir(mtnrsrc,module,"PAYJRCS",NULL,payjrcs);
	addDFincvir(mtnrsrc,module,"PAYJRFS",NULL,payjrfs);
	addDFincvir(mtnrsrc,module,"PAYPCSM",NULL,paypcsm);
	addDFincvir(mtnrsrc,module,"PAYPFSM",NULL,paypfsm);
	addDFincvir(mtnrsrc,module,"PAYTYP",NULL,paytyp);
	if(AUTO_LEAVE_DOCKED==TRUE)
	{
	addDFincvir(mtnrsrc,"LVEMGT","LVEDSC",NULL,lvedsc);
	addDFincvir(mtnrsrc,"LVEMGT","LVEMSTR",NULL,lvemst);
	addDFincvir(mtnrsrc,"LVEMGT","LVEJOB",NULL,lvejob);
	addDFincvir(mtnrsrc,"LVEMGT","LVETRAN",NULL,lvetrn);
	addDFincvir(mtnrsrc,"LVEMGT","LVEDOCK",NULL,lvedck);
	}
	GET_ALL_SCREEN_VIRTUALS(mtnrsrc,0);
	nofields=NUMFLDS(t->fileno);
	fields=FLDPOINTER(t->fileno);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			ADVnonlstmakefld(mtnrsrc,t->fileno,fldx,TRUE,TRUE);
			FINDRSCSETFUNC(mtnrsrc,fldx->name,setfiles,mtn);
		}
	}
	if(payjmst!=(-1)) file2rsrc(payjmst,mtnrsrc,FALSE);
	if(prsnnl!=(-1)) file2rsrc(prsnnl,mtnrsrc,FALSE);
	if(posmstr!=(-1)) file2rsrc(posmstr,mtnrsrc,FALSE);
	if(pgross!=(-1)) file2rsrc(pgross,mtnrsrc,FALSE);
	if(pgrat!=(-1)) file2rsrc(pgrat,mtnrsrc,FALSE);
	if(posrate!=(-1)) file2rsrc(posrate,mtnrsrc,FALSE);
	if(paymstr!=(-1)) file2rsrc(paymstr,mtnrsrc,FALSE);
#ifdef RADDOG
	if(payjcsm!=(-1)) file2rsrc(payjcsm,mtnrsrc,FALSE);
	if(payjfsm!=(-1)) file2rsrc(payjfsm,mtnrsrc,FALSE);
	if(payjrfs!=(-1)) file2rsrc(payjrfs,mtnrsrc,FALSE);
	if(payjrcs!=(-1)) file2rsrc(payjrcs,mtnrsrc,FALSE);
	if(paypcsm!=(-1)) file2rsrc(paypcsm,mtnrsrc,FALSE);
	if(paypfsm!=(-1)) file2rsrc(paypfsm,mtnrsrc,FALSE);
#endif /* RADDOG */
	mtn->ratesavl=APPlibNEW();
	if(pgrat==(-1)) addAPPlib(mtn->ratesavl,"No Rates Available.");
	else addAPPlib(mtn->ratesavl,"No Rates Defined.");
	addlstrsrc(mtnrsrc,"RATE IDENTIFICATIONS",&which_type,TRUE,changeRATEids,mtn->ratesavl->numlibs,&mtn->ratesavl->libs,mtn);
	FINDRSCSETFUNC(mtnrsrc,"PERSONNEL IDENTIFICATION",getfile,mtn);
	FINDRSCSETFUNC(mtnrsrc,"JOB NUMBER",getfile,mtn);
	FINDRSCSETFUNC(mtnrsrc,"RATE IDENTIFICATION",getfile,mtn);
	FINDRSCSETFUNC(mtnrsrc,"POSITION IDENTIFICATION",getposition,mtn);
	FINDRSCSETFUNC(mtnrsrc,"GROSS IDENTIFICATION",getgross,mtn);
	if(PAYROLL_SETUP->manage_earnings==TRUE)
	{
		FINDRSCSETFUNC(mtnrsrc,"EARNINGS",getnewearnings,mtn);
	}
	FINDRSCSETFUNC(mtnrsrc,"RATE AMOUNT",getnewamt,mtn);
	FINDRSCSETFUNC(mtnrsrc,"UNITS",getnewamt,mtn);
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
				if(!RDAstrcmp(rateid,"LEAVE DOCKED") && AUTO_LEAVE_DOCKED==TRUE) xediting=FALSE;
					else xediting=TRUE;
				seteditsinglefilersrc(t->fileno,1,mtnrsrc,xediting,FALSE);
				rsrc2singlefilerecord(t->fileno,mtnrsrc);
			} else {
				singlefilerecord2rsrc(t->fileno,mtnrsrc);
				FINDFLDGETSTRING(t->fileno,"RATE IDENTIFICATION",&rateid);
				if(!RDAstrcmp(rateid,"LEAVE DOCKED") && AUTO_LEAVE_DOCKED==TRUE) xediting=FALSE;
					else xediting=TRUE;
				seteditsinglefilersrc(t->fileno,1,mtnrsrc,xediting,FALSE);
				FINDRSCSETEDITABLE(mtnrsrc,"DELETEFLAG",TRUE);
				GetRDATData(t->fileno,mtn->previous);
			}
			break;
		default:
		case 0:
			if(EQLNRDKEY(t->fileno,1,targetkey)) 
			{
				KEYNRD(t->fileno,1);
				ReadRDAScrolledLists(mtnrsrc);
				if(EQLNRD(t->fileno,1))
				{
					if(!RDAstrcmp(rateid,"LEAVE DOCKED") && AUTO_LEAVE_DOCKED==TRUE) xediting=FALSE;
						else xediting=TRUE;
					seteditsinglefilersrc(t->fileno,1,mtnrsrc,xediting,FALSE);
					rsrc2singlefilerecord(t->fileno,mtnrsrc);
				} else {
					singlefilerecord2rsrc(t->fileno,mtnrsrc);
					FINDFLDGETSTRING(t->fileno,"RATE IDENTIFICATION",&rateid);
					if(!RDAstrcmp(rateid,"LEAVE DOCKED") && AUTO_LEAVE_DOCKED==TRUE) xediting=FALSE;
						else xediting=TRUE;
					seteditsinglefilersrc(t->fileno,1,mtnrsrc,xediting,FALSE);
					FINDRSCSETEDITABLE(mtnrsrc,"DELETEFLAG",TRUE);
					GetRDATData(t->fileno,mtn->previous);
				}
			} else {
				singlefilerecord2rsrc(t->fileno,mtnrsrc);
				FINDFLDGETSTRING(t->fileno,"RATE IDENTIFICATION",&rateid);
				if(!RDAstrcmp(rateid,"LEAVE DOCKED") && AUTO_LEAVE_DOCKED==TRUE) xediting=FALSE;
					else xediting=TRUE;
				seteditsinglefilersrc(t->fileno,1,mtnrsrc,xediting,FALSE);
				FINDRSCSETEDITABLE(mtnrsrc,"DELETEFLAG",TRUE);
				GetRDATData(t->fileno,mtn->previous);
			}
			FINDFLDGETSTRING(t->fileno,"PERSONNEL IDENTIFICATION",
				&perid);
			FINDFLDGETINT(t->fileno,"JOB NUMBER",&jobno);
			break;
	}
	mtn->dbsort=t;
	GetNewRateStruct(t,mtn->ps,perid,jobno,mtnrsrc);
	p=Rmalloc(sizeof(PassableStruct));
	p->r=mtnrsrc;
	p->a=t;
	p->p=mtn->ps;
	mtn->p=p;
	ADVSetVirFieldFuncs(mtnrsrc,PaySelSubData,p);
	getsupportingfiles(mtnrsrc,mtn,FALSE);
	if(perid!=NULL) Rfree(perid);
	addbtnrsrc(mtnrsrc,"PREVIOUS",TRUE,getpreviouscb,mtn);
	addbtnrsrc(mtnrsrc,"NEXT",TRUE,getnextcb,mtn);
	addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetmtndefs,mtn);
	addrfexrsrc(mtnrsrc,"QUIT",TRUE,quit_record,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	x=FINDVIRTUAL(mtnrsrc,"PERIOD EARNINGS AMOUNT");
	if(x!=(-1))
	{
		v=mtnrsrc->virflds+x;
		v->computed=FALSE;
	}
/* RADDOG */
/*
	FINDRSCSETEDITABLE(mtnrsrc,"PERIOD EARNINGS",FALSE);
	FINDRSCSETEDITABLE(mtnrsrc,"CHECK GROSS",FALSE);
*/
	FINDRSCSETEDITABLE(mtnrsrc,"PAY IDENTIFICATION",FALSE);
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,PaySelSubData,p,FALSE);
	x=FINDVIRTUAL(mtnrsrc,"PAY AMOUNT");
	if(x!=(-1))
	{
		v=mtnrsrc->virflds+x;
		FINDRSCSETDOUBLE(mtnrsrc,"PAY AMOUNT",*v->value.float_value);
	}
	x=FINDVIRTUAL(mtnrsrc,"PERIOD CHECK GROSS");
	if(x!=(-1))
	{
		v=mtnrsrc->virflds+x;
		v->computed=FALSE;
	}
	for(x=0;x<mtn->ratesavl->numlibs;++x)
	{
		if(!RDAstrcmp(mtn->ratesavl->libs[x],rateid)) break;
	}
	if(rateid!=NULL) Rfree(rateid);
	sel=0;
	if(x<mtn->ratesavl->numlibs) sel=x;
	FINDRSCSETINT(mtnrsrc,"RATE IDENTIFICATIONS",sel);
	FINDRSCSETINPUTFOCUS(mtnrsrc,"UNITS");
	APPmakescrn(mtnrsrc,TRUE,quit_record,mtn,FALSE);
}
static void save_record_up(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_record(mtnrsrc,TRUE,mtn);
}
static void save_record_nup(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_record(mtnrsrc,FALSE,mtn);
}
static void UndoDockAmount(mtnstruct *mtn,char *perid,int jobno)
{
	short ef=FALSE;
	char *lperid=NULL,deleteflag=FALSE;
	int ljobno=0;
	double amt=0.0,damt=0.0;
	RDATData *prev=NULL;
	DBsort *dbsort=NULL;
	PassableStruct *p=NULL;

	if(mtn!=NULL)
	{
		dbsort=mtn->dbsort;
		p=mtn->p;
	}
	COPYFIELD(dbsort->fileno,lvesort->fileno,"SELECT PAYROLL SORT SEQUENCE");
	FINDFLDSETSTRING(lvesort->fileno,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(lvesort->fileno,"JOB NUMBER",jobno);
	ef=GTENRD(lvesort->fileno,1);
	while(!ef)
	{
		FINDFLDGETSTRING(lvesort->fileno,"PERSONNEL IDENTIFICATION",&lperid);
		FINDFLDGETINT(lvesort->fileno,"JOB NUMBER",&ljobno);
		if(RDAstrcmp(perid,lperid) || jobno!=ljobno) break;
		FINDFLDGETCHAR(lvesort->fileno,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			deleteflag=TRUE;
			FINDFLDSETCHAR(lvesort->fileno,"DELETEFLAG",deleteflag);
			WRTNRD(lvesort->fileno,0,NULL);
			LOCNRDFILE(lvedck);
			COPYFIELD(lvesort->fileno,lvedck,"PERSONNEL IDENTIFICATION");
			COPYFIELD(lvesort->fileno,lvedck,"JOB NUMBER");
			COPYFIELD(lvesort->fileno,lvedck,
				"LEAVE DESCRIPTION IDENTIFICATION");
			COPYFIELD(lvesort->fileno,lvedck,"TRANSACTION NUMBER");
			COPYFIELD(lvesort->fileno,lvedck,"PAY IDENTIFICATION");
			if(!ADVEQLNRDsec(lvedck,1,PaySelSubData,p)) 
			{
				prev=RDATDataNEW(lvedck);
				deleteflag=TRUE;
				FINDFLDSETCHAR(lvedck,"DELETEFLAG",deleteflag);
				FINDFLDGETDOUBLE(lvedck,"DOCK AMOUNT",&amt);
				WRTTRANS(lvedck,0,NULL,prev);
				if(prev!=NULL) FreeRDATData(prev);
			} else amt=0.0;
			UNLNRDFILE(lvedck);
			LOCNRDFILE(lvetrn);
			if(amt!=0.0)
			{
				COPYFIELD(lvesort->fileno,lvetrn,"PERSONNEL IDENTIFICATION");
				COPYFIELD(lvesort->fileno,lvetrn,"JOB NUMBER");
				COPYFIELD(lvesort->fileno,lvetrn,
					"LEAVE DESCRIPTION IDENTIFICATION");
				COPYFIELD(lvesort->fileno,lvetrn,"TRANSACTION NUMBER");
				if(!ADVEQLNRDsec(lvetrn,1,PaySelSubData,p))
				{
					prev=RDATDataNEW(lvetrn);
					FINDFLDGETDOUBLE(lvetrn,"DOCKED AMOUNT",&damt);
					damt-=amt;
					if(damt==0.0)
					{
						deleteflag=FALSE;
						FINDFLDSETCHAR(lvetrn,"DOCK COMPUTED",deleteflag);
						FINDFLDSETCHAR(lvetrn,"DOCK COMPLETED",deleteflag);
					}
					FINDFLDSETDOUBLE(lvetrn,"DOCKED AMOUNT",damt);
					WRTTRANS(lvetrn,0,NULL,prev);
					if(prev!=NULL) FreeRDATData(prev);
				}
			}
			UNLNRDFILE(lvetrn);
		}
		ef=NXTNRD(lvesort->fileno,1);
	}
	if(lperid!=NULL) Rfree(lperid);
}
static void ReDoDockAmount(mtnstruct *mtn,char *perid,int jobno)
{
	short ef=FALSE;
	char *lperid=NULL,deleteflag=FALSE;
	int ljobno=0;
	double amt=0.0,damt=0.0;
	RDATData *prev=NULL;
	DBsort *dbsort=NULL;
	PassableStruct *p=NULL;

	if(mtn!=NULL)
	{
		dbsort=mtn->dbsort;
		p=mtn->p;
	}
	COPYFIELD(dbsort->fileno,lvesort->fileno,"SELECT PAYROLL SORT SEQUENCE");
	FINDFLDSETSTRING(lvesort->fileno,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(lvesort->fileno,"JOB NUMBER",jobno);
	ef=GTENRD(lvesort->fileno,1);
	while(!ef)
	{
		FINDFLDGETSTRING(lvesort->fileno,"PERSONNEL IDENTIFICATION",&lperid);
		FINDFLDGETINT(lvesort->fileno,"JOB NUMBER",&ljobno);
		if(RDAstrcmp(perid,lperid) || jobno!=ljobno) break;
		FINDFLDGETCHAR(lvesort->fileno,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			deleteflag=FALSE;
			amt=0.0;
			FINDFLDSETCHAR(lvesort->fileno,"DELETEFLAG",deleteflag);
			WRTNRD(lvesort->fileno,0,NULL);
			LOCNRDFILE(lvedck);
			COPYFIELD(lvesort->fileno,lvedck,"PERSONNEL IDENTIFICATION");
			COPYFIELD(lvesort->fileno,lvedck,"JOB NUMBER");
			COPYFIELD(lvesort->fileno,lvedck,
				"LEAVE DESCRIPTION IDENTIFICATION");
			COPYFIELD(lvesort->fileno,lvedck,"TRANSACTION NUMBER");
			COPYFIELD(lvesort->fileno,lvedck,"PAY IDENTIFICATION");
			if(!ADVEQLNRDsec(lvedck,1,PaySelSubData,p)) 
			{
				prev=RDATDataNEW(lvedck);
				deleteflag=FALSE;
				FINDFLDSETCHAR(lvedck,"DELETEFLAG",deleteflag);
				FINDFLDGETDOUBLE(lvedck,"DOCK AMOUNT",&amt);
				WRTTRANS(lvedck,0,NULL,prev);
				if(prev!=NULL) FreeRDATData(prev);
			} else amt=0.0;
			UNLNRDFILE(lvedck);
			LOCNRDFILE(lvetrn);
			if(amt!=0.0)
			{
				COPYFIELD(lvesort->fileno,lvetrn,"PERSONNEL IDENTIFICATION");
				COPYFIELD(lvesort->fileno,lvetrn,"JOB NUMBER");
				COPYFIELD(lvesort->fileno,lvetrn,
					"LEAVE DESCRIPTION IDENTIFICATION");
				COPYFIELD(lvesort->fileno,lvetrn,"TRANSACTION NUMBER");
				if(!ADVEQLNRDsec(lvetrn,1,PaySelSubData,p))
				{
					prev=RDATDataNEW(lvetrn);
					FINDFLDGETDOUBLE(lvetrn,"DOCKED AMOUNT",&damt);
					damt+=amt;
					FINDFLDSETDOUBLE(lvetrn,"DOCKED AMOUNT",damt);
					deleteflag=TRUE;
					FINDFLDSETCHAR(lvetrn,"DOCK COMPUTED",deleteflag);
					WRTTRANS(lvetrn,0,NULL,prev);
					if(prev!=NULL) FreeRDATData(prev);
				}
			}
			UNLNRDFILE(lvetrn);
		}
		ef=NXTNRD(lvesort->fileno,1);
	}
	if(lperid!=NULL) Rfree(lperid);
}
static void save_record(RDArsrc *mtnrsrc,short update_list,mtnstruct *mtn)
{
	int valuex=0,jobno=0;
	RDArsrc *tmprsrc=NULL;
	char deleteflag=FALSE,*rateid=NULL,pdeleteflag=FALSE,*e=NULL;
	char *perid=NULL;

	readsinglefilewidgets(mtn->dbsort->fileno,mtnrsrc);
	rsrc2singlefilerecord(mtn->dbsort->fileno,mtnrsrc);
	FINDFLDGETSTRING(mtn->dbsort->fileno,"RATE IDENTIFICATION",&rateid);
	if(!RDAstrcmp(rateid,"LEAVE DOCKED") && AUTO_LEAVE_DOCKED==TRUE)
	{
		FINDRDATDataGETCHAR(mtn->previous,"DELETEFLAG",&pdeleteflag);
		FINDFLDGETCHAR(mtn->dbsort->fileno,"DELETEFLAG",&deleteflag);
		if(deleteflag && !pdeleteflag)
		{
			FINDFLDGETSTRING(mtn->dbsort->fileno,"PERSONNEL IDENTIFICATION",
				&perid);
			FINDFLDGETINT(mtn->dbsort->fileno,"JOB NUMBER",&jobno);
			UndoDockAmount(mtn,perid,jobno);
			if(perid!=NULL) Rfree(perid);
		} else if(!deleteflag && pdeleteflag)
		{
			FINDFLDGETSTRING(mtn->dbsort->fileno,"PERSONNEL IDENTIFICATION",
				&perid);
			FINDFLDGETINT(mtn->dbsort->fileno,"JOB NUMBER",&jobno);
			ReDoDockAmount(mtn,perid,jobno);
			if(perid!=NULL) Rfree(perid);
		}
	}
	if(rateid!=NULL) Rfree(rateid);
	WRTNRD(mtn->dbsort->fileno,0,NULL);
	FINDFLDGETCHAR(mtn->dbsort->fileno,"DELETEFLAG",&deleteflag);
	if(deleteflag)
	{
		UndoComputeDock(mtn->ps);
	}
	if(update_list==TRUE)
	{
		if(mbl_trans->mainrsrc!=NULL)
		{
			FINDRSCGETINT(mbl_trans->mainrsrc,"BROWSE LIST",&valuex);
			tmprsrc=diagscrn(mtn->dbsort->fileno,"DIAGNOSTIC SCREEN","SECURITY","Browse Diagnostic",NULL);
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
	quit_record(mtnrsrc,mtn);
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(WindowCount<2) select_backend(mtn->dbsort);
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	if(mtn!=NULL)
	{
		if(mtn->ps!=NULL) FreePaySelStruct(mtn->ps);
		if(mtn->ratesavl!=NULL) freeapplib(mtn->ratesavl);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		if(mtn->p!=NULL) Rfree(mtn->p);
		Rfree(mtn);
	}
}
static void getsupportingfiles(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *exprssn=NULL,*rateid=NULL,*gid=NULL,*gid1=NULL,*rid=NULL;
	char *posid=NULL,*earnings=NULL,*per_earnings=NULL;
	char r_edit=FALSE,gr_edit=FALSE;
	int x=0;
	RDAvirtual *v;
	DBsort *dbsort=NULL;
	PassableStruct *p=NULL;
	RateStruct *RS=NULL;
	short ef=FALSE;
	char deleteflag=FALSE,use_units=FALSE;
	int sel=0;

	CLEAR_SCREEN_VIRTUAL(mtnrsrc);
	if(mtn!=NULL)
	{
		dbsort=mtn->dbsort;
		p=mtn->p;
	}
	x=FINDVIRTUAL(mtnrsrc,"CHECK GROSS");
	if(x!=(-1))
	{
		v=mtnrsrc->virflds+x;
		v->computed=FALSE;
	}
	x=FINDVIRTUAL(mtnrsrc,"PAY AMOUNT");
	if(x!=(-1))
	{
		v=mtnrsrc->virflds+x;
		v->computed=FALSE;
	}
	x=FINDVIRTUAL(mtnrsrc,"PERIOD CHECK GROSS");
	if(x!=(-1))
	{
		v=mtnrsrc->virflds+x;
		v->computed=FALSE;
	}
	COPYFIELD(dbsort->fileno,paymstr,"PAY IDENTIFICATION");
	if(ADVEQLNRDsec(paymstr,1,PaySelSubData,p)) KEYNRD(paymstr,1);
	if(update) updatefilerecord2rsrc(paymstr,mtnrsrc);
	else filerecord2rsrc(paymstr,mtnrsrc);

	FINDRSCGETSTRING(mtnrsrc,"POSITION IDENTIFICATION",&posid);
	FINDFLDSETSTRING(posmstr,"POSITION IDENTIFICATION",posid);
	if(ADVEQLNRDsec(posmstr,1,PaySelSubData,p)) KEYNRD(posmstr,1);
	if(update) updatefilerecord2rsrc(posmstr,mtnrsrc);
	else filerecord2rsrc(posmstr,mtnrsrc);
	if(posid!=NULL) Rfree(posid);

	FINDRSCGETSTRING(mtnrsrc,"GROSS IDENTIFICATION",&gid1);
	FINDFLDSETSTRING(pgross,"GROSS IDENTIFICATION",gid1);
	if(ADVEQLNRDsec(pgross,1,PaySelSubData,p)) KEYNRD(pgross,1);
	if(update) updatefilerecord2rsrc(pgross,mtnrsrc);
	else filerecord2rsrc(pgross,mtnrsrc);

	FINDRSCGETSTRING(mtnrsrc,"RATE IDENTIFICATION",&rateid);
	FINDFLDSETSTRING(posrate,"RATE IDENTIFICATION",rateid);
	if(ADVEQLNRDsec(posrate,1,PaySelSubData,p)) KEYNRD(posrate,1);
	if(update) updatefilerecord2rsrc(posrate,mtnrsrc);
	else filerecord2rsrc(posrate,mtnrsrc);
	FINDFLDGETCHAR(posrate,"EDITABLE",&r_edit);
	FINDFLDGETCHAR(posrate,"USE UNITS",&use_units);
	if(use_units)
	{
		FINDRSCSETEDITABLE(mtnrsrc,"UNITS",TRUE);
	} else {
		FINDRSCSETEDITABLE(mtnrsrc,"UNITS",FALSE);
	}

	ZERNRD(pgrat);
	FINDFLDSETSTRING(pgrat,"GROSS IDENTIFICATION",gid1);
	if(mtn->ratesavl!=NULL) freeapplib(mtn->ratesavl);
	mtn->ratesavl=APPlibNEW();
	ef=ADVGTENRDsec(pgrat,1,PaySelSubData,p);
	while(!ef)
	{
		FINDFLDGETSTRING(pgrat,"GROSS IDENTIFICATION",&gid);
		if(RDAstrcmp(gid,gid1)) break;
		FINDFLDGETCHAR(pgrat,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDGETSTRING(pgrat,"RATE IDENTIFICATION",&rid);
			addAPPlib(mtn->ratesavl,rid);
			if(!RDAstrcmp(rid,rateid)) sel=mtn->ratesavl->numlibs-1;
		}
		ef=ADVNXTNRDsec(pgrat,1,PaySelSubData,p);
	}
	if(mtn->ratesavl->numlibs<1)
	{
		addAPPlib(mtn->ratesavl,"No Rates Defined.");
	}
	if(sel>=mtn->ratesavl->numlibs) sel=0;
	FINDRSCLISTAPPlib(mtnrsrc,"RATE IDENTIFICATIONS",sel,mtn->ratesavl);
	if(update) updatersrc(mtnrsrc,"RATE IDENTIFICATIONS");

	FINDFLDSETSTRING(pgrat,"GROSS IDENTIFICATION",gid1);
	FINDFLDSETSTRING(pgrat,"RATE IDENTIFICATION",rateid);
	if(ADVEQLNRDsec(pgrat,1,PaySelSubData,p)) KEYNRD(pgrat,1);
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
	if(ADVEQLNRDsec(payjmst,1,PaySelSubData,p)) KEYNRD(payjmst,1);
	if(update) updatefilerecord2rsrc(payjmst,mtnrsrc);
	else filerecord2rsrc(payjmst,mtnrsrc);

	if(gid!=NULL) Rfree(gid);
	if(gid1!=NULL) Rfree(gid1);
	if(rid!=NULL) Rfree(rid);

	COPYFIELD(dbsort->fileno,payjfsm,"PERSONNEL IDENTIFICATION");
	COPYFIELD(dbsort->fileno,payjfsm,"JOB NUMBER");
	COPYFIELD(paymstr,payjfsm,"FISCAL YEAR");
	if(ADVEQLNRDsec(payjfsm,1,PaySelSubData,p)) KEYNRD(payjfsm,1);
#ifdef RADDOG
	if(update) updatefilerecord2rsrc(payjfsm,mtnrsrc);
	else filerecord2rsrc(payjfsm,mtnrsrc);
#endif /* RADDOG */

	COPYFIELD(dbsort->fileno,prsnnl,"PERSONNEL IDENTIFICATION");
	if(ADVEQLNRDsec(prsnnl,1,PaySelSubData,p)) KEYNRD(prsnnl,1);
	if(update) updatefilerecord2rsrc(prsnnl,mtnrsrc);
	else filerecord2rsrc(prsnnl,mtnrsrc);

	COPYFIELD(dbsort->fileno,payjrcs,"PERSONNEL IDENTIFICATION");
	COPYFIELD(dbsort->fileno,payjrcs,"JOB NUMBER");
	FINDFLDSETSTRING(payjrcs,"RATE IDENTIFICATION",rateid);
	COPYFIELD(paymstr,payjrcs,"CALENDAR YEAR");
	if(ADVEQLNRDsec(payjrcs,1,PaySelSubData,p)) KEYNRD(payjrcs,1);
#ifdef RADDOG
	if(update) updatefilerecord2rsrc(payjrcs,mtnrsrc);
	else filerecord2rsrc(payjrcs,mtnrsrc);
#endif /* RADDOG */

	COPYFIELD(dbsort->fileno,payjrfs,"PERSONNEL IDENTIFICATION");
	COPYFIELD(dbsort->fileno,payjrfs,"JOB NUMBER");
	FINDFLDSETSTRING(payjrfs,"RATE IDENTIFICATION",rateid);
	COPYFIELD(paymstr,payjrfs,"FISCAL YEAR");
	if(ADVEQLNRDsec(payjrfs,1,PaySelSubData,p)) KEYNRD(payjrfs,1);
#ifdef RADDOG
	if(update) updatefilerecord2rsrc(payjrfs,mtnrsrc);
	else filerecord2rsrc(payjrfs,mtnrsrc);
#endif /* RADDOG */

	COPYFIELD(dbsort->fileno,paypcsm,"PERSONNEL IDENTIFICATION");
	COPYFIELD(paymstr,paypcsm,"CALENDAR YEAR");
	if(ADVEQLNRDsec(paypcsm,1,PaySelSubData,p)) KEYNRD(paypcsm,1);
#ifdef RADDOG
	if(update) updatefilerecord2rsrc(paypcsm,mtnrsrc);
	else filerecord2rsrc(paypcsm,mtnrsrc);
#endif /* RADDOG */

	COPYFIELD(dbsort->fileno,paypfsm,"PERSONNEL IDENTIFICATION");
	COPYFIELD(paymstr,paypfsm,"FISCAL YEAR");
	if(ADVEQLNRDsec(paypfsm,1,PaySelSubData,p)) KEYNRD(paypfsm,1);
#ifdef RADDOG
	if(update) updatefilerecord2rsrc(paypfsm,mtnrsrc);
	else filerecord2rsrc(paypfsm,mtnrsrc);
#endif /* RADDOG */

	FINDFLDGETSTRING(pgrat,"RATE AMOUNT",&exprssn);
	if(RDAstrcmp(rateid,"LEAVE DOCKED") || AUTO_LEAVE_DOCKED==FALSE)
	{
		/* This line and the following 24 lines were added 03/17/2006 */
		x=FINDVIRTUAL(mainrsrc,"POSGRAT LOOKUP NAME");
		if(x!=(-1))
		{
			v=mainrsrc->virflds+x;
			v->computed=FALSE;
		} else {
			prterr("Error where is POSGRAT LOOKUP NAME?");
		}
		x=FINDVIRTUAL(mainrsrc,"POSGRAT MINMAX");
		if(x!=(-1))
		{
			v=mainrsrc->virflds+x;
			v->computed=FALSE;
		} else {
			prterr("Error where is POSGRAT MINMAX?");
		}
		x=FINDVIRTUAL(mainrsrc,"POSGRAT X-AXIS NAME");
		if(x!=(-1))
		{
			v=mainrsrc->virflds+x;
			v->computed=FALSE;
		} else {
			prterr("Error where is POSGRAT X-AXIS NAME?");
		}
		EARNINGS_AMT=0;
		RS=GetRateStruct(mtn->ps,rateid);
		if(PAYROLL_SETUP->manage_earnings==TRUE)
		{
			FINDFLDGETSTRING(pgrat,"EARNINGS AMOUNT",&earnings);
			if(!isEMPTY(earnings))
			{
				EARNINGS_AMT=EVALUATEdbl(earnings,PaySelSubData,p);
				EARNINGS_AMT=round(EARNINGS_AMT);
				if(RS!=NULL)
				{
					RS->earnings=EARNINGS_AMT;
				}
				if(earnings!=NULL) Rfree(earnings);
				FINDFLDSETDOUBLE(dbsort->fileno,"EARNINGS",EARNINGS_AMT);
				FINDRSCSETDOUBLE(mtnrsrc,"EARNINGS",EARNINGS_AMT);
			}
			FINDFLDGETSTRING(posrate,"PERIOD EARNINGS",&per_earnings);
			if(!isEMPTY(per_earnings))
			{
				PERIOD_EARNINGS_AMT=EVALUATEdbl(per_earnings,PaySelSubData,p);
				PERIOD_EARNINGS_AMT=round(PERIOD_EARNINGS_AMT);
				if(RS!=NULL)
				{
					RS->period_earnings=PERIOD_EARNINGS_AMT;
				}
				if(per_earnings!=NULL) Rfree(per_earnings);
				FINDFLDSETDOUBLE(dbsort->fileno,"PERIOD EARNINGS",PERIOD_EARNINGS_AMT);
				FINDRSCSETDOUBLE(mtnrsrc,"PERIOD EARNINGS",PERIOD_EARNINGS_AMT);
			}
		}
		RATE_AMT=0.0;
		if(!isEMPTY(exprssn))
		{
			RATE_AMT=EVALUATEdbl(exprssn,PaySelSubData,p);
			RATE_AMT=round(RATE_AMT);
			if(RS!=NULL)
			{
				RS->rate=RATE_AMT;
			}
			Rfree(exprssn);
		}
		FINDFLDSETDOUBLE(dbsort->fileno,"RATE AMOUNT",RATE_AMT);
		FINDRSCSETDOUBLE(mtnrsrc,"RATE AMOUNT",RATE_AMT);
		FINDFLDGETSTRING(posrate,"CHECK GROSS",&exprssn);
		if(!isEMPTY(exprssn))
		{
			CHECK_GROSS=EVALUATEdbl(exprssn,PaySelSubData,p);
			CHECK_GROSS=round(CHECK_GROSS);
			if(RS!=NULL)
			{
				RS->gross=CHECK_GROSS;
			}
			Rfree(exprssn);
		}
		FINDFLDSETDOUBLE(dbsort->fileno,"CHECK GROSS",CHECK_GROSS);
		FINDRSCSETDOUBLE(mtnrsrc,"CHECK GROSS",CHECK_GROSS);
	}
	if(rateid!=NULL) Rfree(rateid);

	COPYFIELD(dbsort->fileno,payjcsm,"PERSONNEL IDENTIFICATION");
	COPYFIELD(dbsort->fileno,payjcsm,"JOB NUMBER");
	COPYFIELD(paymstr,payjcsm,"CALENDAR YEAR");
	if(ADVEQLNRDsec(payjcsm,1,PaySelSubData,p)) KEYNRD(payjcsm,1);
	if(update) updatefilerecord2rsrc(payjcsm,mtnrsrc);
		else filerecord2rsrc(payjcsm,mtnrsrc);
}
static void GetNewRateStruct(DBsort *dbsort,PaySelStruct *p,char *perid,
	int jobno,RDArsrc *mtnrsrc)
{
	char *lperid=NULL,deleteflag=FALSE,*rateid=NULL,*temp=NULL;
	int ljobno=0,seq=0,x=0,y,length=0,fyear=0,cyear=0;
	short ef=FALSE,len=0;
	double gross=0.0,units=0.0,rate=0.0,earnings=0.0,per_earnings=0.0,mg=0.0,mu=0.0;
	void *keystuff=NULL;
	RateStruct *r=NULL;

	keystuff=GETNRDKEY(dbsort->fileno,1,&len);
	ZERNRD(dbsort->fileno);
	FreeRateStruct(p);
	p->rates=NULL;
	FINDFLDSETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(dbsort->fileno,"JOB NUMBER",jobno);
	ef=GTENRD(dbsort->fileno,1);
	while(!ef)
	{
		FINDFLDGETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",&lperid);
		FINDFLDGETINT(dbsort->fileno,"JOB NUMBER",&ljobno);
		if(RDAstrcmp(perid,lperid) || jobno!=ljobno) break;
		FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&deleteflag);
		FINDFLDGETINT(paymstr,"FISCAL YEAR",&fyear);
		FINDFLDGETINT(paymstr,"CALENDAR YEAR",&cyear);
		if(!deleteflag)
		{
			FINDFLDGETSTRING(dbsort->fileno,"RATE IDENTIFICATION",&rateid);
			FINDFLDGETDOUBLE(dbsort->fileno,"RATE AMOUNT",&rate);
			FINDFLDGETDOUBLE(dbsort->fileno,"UNITS",&units);
			FINDFLDGETDOUBLE(dbsort->fileno,"CHECK GROSS",&gross);
			if(PAYROLL_SETUP->manage_earnings==TRUE)
			{
				FINDFLDGETDOUBLE(dbsort->fileno,"EARNINGS",&earnings);
				FINDFLDGETDOUBLE(dbsort->fileno,"PERIOD EARNINGS",&per_earnings);
			}
/* Place YTD gathering here */
			x=PEAddRate2PaySelStruct(p,rateid,earnings,per_earnings,units,gross,rate,seq);
			r=p->rates+(x-1);
			r->ytd_gross=0.0;r->ytd_units=0.0;
			r->ftd_gross=0.0;r->ftd_units=0.0;
			r->ftd_earnings=0.0;r->ytd_earnings=0.0;
			FINDFLDSETSTRING(payjrfs,"PERSONNEL IDENTIFICATION",lperid);
			FINDFLDSETINT(payjrfs,"JOB NUMBER",ljobno);
			FINDFLDSETINT(payjrfs,"FISCAL YEAR",fyear);
			FINDFLDSETSTRING(payjrfs,"RATE IDENTIFICATION",rateid);

			if(ADVEQLNRDsec(payjrfs,1,PaySelSubData,p))
			{
				KEYNRD(payjrfs,1);
				if(diagapps)
				{
					prterr("DIAG APPLICATION: Cannot find Payroll Job Rate Fiscal summary record [PAYROLL][PAYJRFS] for the keys of Personnel Identification=[%s], Job Number=[%d], Fiscal Year=[%d] and Rate Identification=[%s].",(lperid==NULL?"":lperid),ljobno,fyear,(rateid==NULL?"":rateid));SEENRDRECORD(payjrfs);
				}
			} else {
				if(diagapps)
				{
					prterr("DIAG APPLICATION: Found Payroll Job Rate Fiscal summary record [PAYROLL][PAYJRFS] for the keys of Personnel Identification=[%s], Job Number=[%d], Fiscal Year=[%d] and Rate Identification=[%s].",(lperid==NULL?"":lperid),ljobno,fyear,(rateid==NULL?"":rateid));SEENRDRECORD(payjrfs);
				}
			}
			for(y=0;y<14;++y)
			{
				length=(RDAstrlen(FISCALPERIOD[y])+20);
				temp=Rrealloc(temp,length);
				sprintf(temp,"%s GROSS",FISCALPERIOD[y]);
				FINDFLDGETDOUBLE(payjrfs,temp,&mg);
				if(diagapps)
				{
					prterr("DIAG APPLICATION: Accumulating the [%s] field of the [PAYROLL][PAYJRFS] file with a value of [$%.02f] into the present total of [$%.02f].",(temp==NULL?"":temp),(mg/100),r->ftd_gross);
				}
				r->ftd_gross+=mg;
				memset(temp,0,length);
				sprintf(temp,"%s UNITS",FISCALPERIOD[y]);
				FINDFLDGETDOUBLE(payjrfs,temp,&mu);
				if(diagapps)
				{
					prterr("DIAG APPLICATION: Accumulating the [%s] field of the [PAYROLL][PAYJRFS] file with a value of [$%.02f] into the present total of [$%.02f].",(temp==NULL?"":temp),(mu/100),r->ftd_units);
				}
				r->ftd_units+=mu;
				if(PAYROLL_SETUP->manage_earnings==TRUE)
				{
					memset(temp,0,length);
					sprintf(temp,"%s EARNINGS",FISCALPERIOD[y]);
					FINDFLDGETDOUBLE(payjrfs,temp,&mg);
					if(diagapps)
					{
						prterr("DIAG APPLICATION: Accumulating the [%s] field of the [PAYROLL][PAYJRFS] file with a value of [$%.02f] into the present total of [$%.02f].",(temp==NULL?"":temp),(mg/100),r->ftd_earnings);
					}
					r->ftd_earnings+=mg;
				}
			}
			FINDFLDSETSTRING(payjrcs,"PERSONNEL IDENTIFICATION",lperid);
			FINDFLDSETINT(payjrcs,"JOB NUMBER",ljobno);
			FINDFLDSETINT(payjrcs,"CALENDAR YEAR",cyear);
			FINDFLDSETSTRING(payjrcs,"RATE IDENTIFICATION",rateid);	
			if(ADVEQLNRDsec(payjrcs,1,PaySelSubData,p))
			{
				KEYNRD(payjrcs,1);
				if(diagapps)
				{
					prterr("DIAG APPLICATION: Cannot find Payroll Job Rate Calendar summary record [PAYROLL][PAYJRCS] for the keys of Personnel Identification=[%s], Job Number=[%d], Calendar Year=[%d] and Rate Identification=[%s].",(lperid==NULL?"":lperid),ljobno,cyear,(rateid==NULL?"":rateid));SEENRDRECORD(payjrcs);
				}
			} else {
				if(diagapps)
				{
					prterr("DIAG APPLICATION: Found Payroll Job Rate Fiscal summary record [PAYROLL][PAYJRCS] for the keys of Personnel Identification=[%s], Job Number=[%d], Calendar Year=[%d] and Rate Identification=[%s].",(lperid==NULL?"":lperid),ljobno,cyear,(rateid==NULL?"":rateid));SEENRDRECORD(payjrcs);
				}
			}
			for(y=0;y<12;++y)
			{
				length=(RDAstrlen(CALENDAR_MONTHS[y])+20);
				temp=Rrealloc(temp,length);
				sprintf(temp,"%s GROSS",CALENDAR_MONTHS[y]);
				FINDFLDGETDOUBLE(payjrcs,temp,&mg);
				if(diagapps)
				{
					prterr("DIAG APPLICATION: Accumulating the [%s] field of the [PAYROLL][PAYJRCS] file with a value of [$%.02f] into the present total of [$%.02f].",(temp==NULL?"":temp),(mg/100),r->ytd_gross);
				}
				r->ytd_gross+=mg;
				memset(temp,0,length);
				sprintf(temp,"%s UNITS",CALENDAR_MONTHS[y]);
				FINDFLDGETDOUBLE(payjrcs,temp,&mu);
				if(diagapps)
				{
					prterr("DIAG APPLICATION: Accumulating the [%s] field of the [PAYROLL][PAYJRCS] file with a value of [$%.02f] into the present total of [$%.02f].",(temp==NULL?"":temp),(mu/100),r->ytd_units);
				}
				r->ytd_units+=mu;
				if(PAYROLL_SETUP->manage_earnings==TRUE)
				{
					memset(temp,0,length);
					sprintf(temp,"%s EARNINGS",CALENDAR_MONTHS[y]);
					FINDFLDGETDOUBLE(payjrcs,temp,&mg);
					if(diagapps)
					{
						prterr("DIAG APPLICATION: Accumulating the [%s] field of the [PAYROLL][PAYJRCS] file with a value of [$%.02f] into the present total of [$%.02f].",(temp==NULL?"":temp),(mg/100),r->ytd_earnings);
					}
					r->ytd_earnings+=mg;
				}
			}
			if(temp!=NULL) Rfree(temp);
		}
		ef=NXTNRD(dbsort->fileno,1);
	}
	if(lperid!=NULL) Rfree(lperid);
	if(rateid!=NULL) Rfree(rateid);
	FINDFLDSETSTRING(dbsort->fileno,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(dbsort->fileno,"JOB NUMBER",jobno);
	if(EQLNRDKEY(dbsort->fileno,1,keystuff)) KEYNRD(dbsort->fileno,1);
	if(keystuff!=NULL) Rfree(keystuff);
}
