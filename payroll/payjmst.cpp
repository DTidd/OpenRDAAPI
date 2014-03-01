/* payjmst.c - Xpert Payroll Job Master Browse */
/*lint -library */
#ifndef WIN32
#define __NAM__ "payjmst.lnx"
#endif
#ifdef WIN32
#define __NAM__ "payjmst.exe"
#endif
#include <app.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>

#include <rptgen.hpp>
#include <powadd.hpp>
#include <postrk.hpp>
#include <fin.hpp>
#include <pay.hpp>
/*ADDLIB add */
/*ADDLIB rpt */
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
	char *CHECK_GROSS;
	char *RATE_AMOUNT;
	char *DEFAULT_UNITS;
	double gross;
	double units;
	double rate;
	double ytd_gross;
	double ytd_units;
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

struct mtnstructs
{
	RDArsrc *r;
	APPlib *grosslist;
	APPlib *grlist;
	PaySelStruct *ps;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short msrnum=(-1),grsnum=(-1),posnum=(-1),poslnum=(-1),possup=(-1);
short payjfsm=(-1),payjcsm=(-1),grsrate=(-1),ratenum=(-1),pospernum=(-1);
short payjrfs=(-1),payjrcs=(-1),paygrp=(-1),postyp=(-1),loc_place=FALSE;
short eeojfnum=(-1),eeojgnum=(-1);
char use_postyp=FALSE,use_supp=FALSE;
char editable=TRUE,manage_fte=TRUE;
MakeBrowseList *mbl_payjmst=NULL,*mbl_paygroup=NULL,*mbl_possup=NULL;
MakeBrowseList *mbl_posmstr=NULL,*mbl_grsmstr=NULL,*mbl_locmstr=NULL;
MakeBrowseList *mbl_postyp=NULL,*mbl_eeojfmstr=NULL,*mbl_eeojgmstr=NULL;
void browse_payjmst(RDArsrc *,mtnstruct *);
static MaintainMaster *MTNMASTER=NULL;
static void quit_record(RDArsrc *,mtnstruct *);
static void quit_record_test(RDArsrc *,mtnstruct *);
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_record_now(RDArsrc *,mtnstruct *);
static void save_record_now1(RDArsrc *,mtnstruct *);
static void save_record_quit(RDArsrc *,mtnstruct *);
static void save_check(RDArsrc *,mtnstruct *,short);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void resetdefs(RDArsrc *,mtnstruct *);
static void SETID(RDArsrc *,mtnstruct *);
static void SETFILES(RDArsrc *,mtnstruct *);
static void getpreviouscb(RDArmem *,mtnstruct *);
static void getnextcb(RDArmem *,mtnstruct *);
static void getsupportingrecords(RDArsrc *,mtnstruct *,short);
static void grossidcb(RDArsrc *,mtnstruct *);
static void setlist(RDArsrc *,mtnstruct *,short);
static void AUEQLfunction(RDArsrc *,MaintainMaster *,void (*)(...),void *,mtnstruct *);
static void NEfunction(RDArsrc *,MaintainMaster *,void (*)(...),void *,mtnstruct *);
static void readscreen(RDArsrc *,mtnstruct *);
static void UpdateVirtuals(RDArsrc *,mtnstruct *);
double UNIT_AMOUNT=0.0,RATE_AMT=0.0,CHECK_GROSS=0.0;
static short delposper(mtnstruct *mtn);
static short save_posper(mtnstruct *mtn);
void payjmstm(short,void *);
static void UpdateGROSSAmounts(RDArsrc *,mtnstruct *,char *,short);
RDAfinmgt *fmgt=NULL;
static void PayjmstSubData(char **,mtnstruct *);
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
#define AddRate2PaySelStruct(a,b,c,d,e,f)	xAddRate2PaySelStruct(a,b,c,d,e,0.0,0.0,0.0,f,__LINE__,__FILE__)
#define PEAddRate2PaySelStruct(a,b,c,d,e,f)	xAddRate2PaySelStruct(a,b,NULL,NULL,NULL,c,d,e,f,__LINE__,__FILE__)
void xAddRate2PaySelStruct(PaySelStruct *p,char *rateid,char *check_gross,
	char *rate_amount,char *defunits,double units,double gross,double rate,int seq,
	int line,char *file)
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
	if(!isEMPTY(rate_amount)) r->RATE_AMOUNT=stralloc(rate_amount);
		else r->RATE_AMOUNT=NULL;
	if(!isEMPTY(defunits)) r->DEFAULT_UNITS=stralloc(defunits);
		else r->DEFAULT_UNITS=NULL;
	r->gross=gross;
	r->units=units;
	r->rate=rate;
	r->seq=seq;
	++p->num;
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

	if(p->rates==NULL)
	{
		return;
	}
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
static void makepaystruct(RDArsrc *mainrsrc,PaySelStruct *ps,mtnstruct *p)
{
	short ef1=0;
	char *exprssn=NULL,*rexprssn=NULL,deleteflag=FALSE;
	char *grossid1=NULL,*rateid=NULL,*uexprssn=NULL,active=FALSE;
	char use_active=FALSE;
	int seq=0;
	NRDfield *f=NULL;

	f=FLDNRD(grsrate,"ACTIVE");
	if(f!=NULL) use_active=TRUE;
		else use_active=FALSE;
	ZERNRD(grsrate);
	FINDFLDSETSTRING(grsrate,"GROSS IDENTIFICATION",ps->grossid);
	ef1=ADVGTENRDsec(grsrate,1,PayjmstSubData,p);
	while(!ef1)
	{
		FINDFLDGETSTRING(grsrate,"GROSS IDENTIFICATION",&grossid1);
		if(RDAstrcmp(ps->grossid,grossid1)) break;
		FINDFLDGETCHAR(grsrate,"DELETEFLAG",&deleteflag);
		if(use_active==TRUE) FINDFLDGETCHAR(grsrate,"ACTIVE",&active);
		if(!deleteflag && (use_active==FALSE || active==TRUE))
		{
			CLEAR_SCREEN_VIRTUAL(p->r);
			FINDFLDGETSTRING(grsrate,"RATE IDENTIFICATION",&rateid);
			FINDFLDSETSTRING(ratenum,"RATE IDENTIFICATION",rateid);
			if(ADVEQLNRDsec(ratenum,1,PayjmstSubData,p)) KEYNRD(ratenum,1);
			FINDFLDGETCHAR(ratenum,"DELETEFLAG",&deleteflag);
			if(!deleteflag)
			{
				FINDFLDGETSTRING(grsrate,"RATE AMOUNT",&rexprssn);
				FINDFLDGETSTRING(grsrate,"DEFAULT UNITS",&uexprssn);
				FINDFLDGETSTRING(ratenum,"CHECK GROSS",&exprssn);
				FINDFLDGETINT(ratenum,"SELECT SEQUENCE",&seq);
				AddRate2PaySelStruct(ps,rateid,exprssn,rexprssn,
					uexprssn,seq);
				if(uexprssn!=NULL) Rfree(uexprssn);
				if(exprssn!=NULL) Rfree(exprssn);
				if(rexprssn!=NULL) Rfree(rexprssn);
			} else {
				if(diagapps)
				{
					prterr("Skipped: Position Rate record [POSTRK][POSRATE] Rate Identification of [%s] because of Deleted Record.",(rateid==NULL?"":rateid));KEYNRD(ratenum,1);
				}
			}
		} else {
			if(diagapps)
			{
				prterr("Skipped: Position Gross Rate record [POSTRK][POSGRAT] Gross Identification of [%s] and Rate Identification of [%s] because of Deleted Record.",(ps->grossid==NULL?"":ps->grossid),(rateid==NULL?"":rateid));KEYNRD(grsrate,1);
			}
		}
		ef1=ADVNXTNRDsec(grsrate,1,PayjmstSubData,p);
	}
	if(grossid1!=NULL) Rfree(grossid1);
}
void ComputePaySelStruct(PaySelStruct *ps,RDArsrc *mainrsrc,mtnstruct *p)
{
	int x,y;
	RDAvirtual *v=NULL;
	RateStruct *r=NULL;
	char grsrate_selzero=FALSE,ratenum_selzero=FALSE;
	double mg=0,mu=0;
	char *temp=NULL;

	if(ps->rates!=NULL)
	{
		for(x=0,r=ps->rates;x<ps->num;++x,++r)
		{
			r->ytd_gross=0.0;r->ytd_units=0.0;
			r->ftd_gross=0.0;r->ftd_units=0.0;
			COPYFIELD(payjfsm,payjrfs,"PERSONNEL IDENTIFICATION");
			COPYFIELD(payjfsm,payjrfs,"JOB NUMBER");
			COPYFIELD(payjfsm,payjrfs,"FISCAL YEAR");
			FINDFLDSETSTRING(payjrfs,"RATE IDENTIFICATION",r->rateid);
			if(ADVEQLNRDsec(payjrfs,1,PayjmstSubData,p)) KEYNRD(payjrfs,1);
			for(y=0;y<14;++y)
			{
				temp=Rmalloc(RDAstrlen(FISCALPERIOD[y])+10);
				sprintf(temp,"%s GROSS",FISCALPERIOD[y]);
				FINDFLDGETDOUBLE(payjrfs,temp,&mg);
				r->ftd_gross+=mg;
				sprintf(temp,"%s UNITS",FISCALPERIOD[y]);
				FINDFLDGETDOUBLE(payjrfs,temp,&mu);
				r->ftd_units+=mu;
			}
			COPYFIELD(payjcsm,payjrcs,"PERSONNEL IDENTIFICATION");
			COPYFIELD(payjcsm,payjrcs,"JOB NUMBER");
			COPYFIELD(payjcsm,payjrcs,"CALENDAR YEAR");
			FINDFLDSETSTRING(payjrcs,"RATE IDENTIFICATION",r->rateid);	
			if(ADVEQLNRDsec(payjrcs,1,PayjmstSubData,p)) KEYNRD(payjrcs,1);
			for(y=0;y<12;++y)
			{
				temp=Rmalloc(RDAstrlen(CALENDAR_MONTHS[y])+10);
				sprintf(temp,"%s GROSS",CALENDAR_MONTHS[y]);
				FINDFLDGETDOUBLE(payjrcs,temp,&mg);
				r->ytd_gross+=mg;
				sprintf(temp,"%s UNITS",CALENDAR_MONTHS[y]);
				FINDFLDGETDOUBLE(payjrcs,temp,&mu);
				r->ytd_units+=mu;
			}
		}
		for(x=0,r=ps->rates;x<ps->num;++x,++r)
		{
			CLEAR_SCREEN_VIRTUAL(p->r);
			UNIT_AMOUNT=r->units;
			if(RDAstrcmp(r->rateid,"LEAVE DOCKED"))
			{
				COPYFIELD(payjfsm,payjrfs,"PERSONNEL IDENTIFICATION");
				COPYFIELD(payjfsm,payjrfs,"JOB NUMBER");
				COPYFIELD(payjfsm,payjrfs,"FISCAL YEAR");
				FINDFLDSETSTRING(payjrfs,"RATE IDENTIFICATION",r->rateid);
				if(ADVEQLNRDsec(payjrfs,1,PayjmstSubData,p)) KEYNRD(payjrfs,1);
				COPYFIELD(payjcsm,payjrcs,"PERSONNEL IDENTIFICATION");
				COPYFIELD(payjcsm,payjrcs,"JOB NUMBER");
				COPYFIELD(payjcsm,payjrcs,"CALENDAR YEAR");
				FINDFLDSETSTRING(payjrcs,"RATE IDENTIFICATION",r->rateid);	
				if(ADVEQLNRDsec(payjrcs,1,PayjmstSubData,p)) KEYNRD(payjrcs,1);
				FINDFLDSETSTRING(grsrate,"GROSS IDENTIFICATION",ps->grossid);
				FINDFLDSETSTRING(grsrate,"RATE IDENTIFICATION",r->rateid);
				if(ADVEQLNRDsec(grsrate,1,PayjmstSubData,p)) KEYNRD(grsrate,1);
				FINDFLDGETCHAR(grsrate,"SELECT ZERO",&grsrate_selzero);
				FINDFLDSETSTRING(ratenum,"RATE IDENTIFICATION",r->rateid);
				if(ADVEQLNRDsec(ratenum,1,PayjmstSubData,p)) KEYNRD(ratenum,1);
				FINDFLDGETCHAR(ratenum,"SELECT ZERO",&ratenum_selzero);
				r->units=0;
				if(!isEMPTY(r->DEFAULT_UNITS))
				{
					r->units=EVALUATEdbl(r->DEFAULT_UNITS,PayjmstSubData,p);
				}
				UNIT_AMOUNT=r->units;
				if(!isEMPTY(r->RATE_AMOUNT))
				{
					r->rate=EVALUATEdbl(r->RATE_AMOUNT,PayjmstSubData,p);
				}
				RATE_AMT=r->rate;
				y=FINDVIRTUAL(mainrsrc,"PAY AMOUNT");
				if(y!=(-1))
				{
					v=mainrsrc->virflds+y;
					v->computed=FALSE;
				}
				if(!isEMPTY(r->CHECK_GROSS))
				{
					r->gross=EVALUATEdbl(r->CHECK_GROSS,PayjmstSubData,p);
					r->gross=round(r->gross);
				}
				CHECK_GROSS=r->gross;
			} else {
				r->gross=0.0;
				r->rate=0.0;
				r->units=0.0;
				CHECK_GROSS=r->gross;
				RATE_AMT=r->rate;
				UNIT_AMOUNT=r->units;
			}
		}
	}
}
short FIELDSubData(char **tmp,mtnstruct *p,char *modulename)
{
	char *filename=NULL,*fieldname=NULL;
	char *temp=NULL,*gn=NULL;
	short start=1,length=0,fileno=(-1),occurrence=1,str_length=0;
	NRDfield *field=NULL;

	if(diageval || diagapps)
	{
		prterr("DIAG FIELDSubData Substituting for [%s].",*tmp);
	}
	filename=stripfilename(*tmp);
	if(filename!=NULL)
	{
		fieldname=stripfieldname(*tmp);
		if(fieldname!=NULL)
		{
			if(!RDAstrcmp(modulename,"LVEMGT") && !RDAstrcmp(filename,"LVETRAN") && !RDAstrcmp(fieldname,"DOCK UNITS"))
			{
				if(*tmp!=NULL) Rfree(*tmp);
				*tmp=stralloc("\"1\"");
			} else {
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
					prterr("Error Field [%s] not found.",*tmp);
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=stralloc("\"\"");
					if(fieldname!=NULL) Rfree(fieldname);
					if(filename!=NULL) Rfree(filename);
					return(1);
				}
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
short STRUCTSubData(char **tmp,mtnstruct *p,char *modulename)
{
	char *filename=NULL;
	short str_length=0;
	RateStruct *d;
	PaySelStruct *c;
	int x;
	int retval=0;

	if(diageval || diagapps)
	{
		prterr("DIAG STRUCTSubData Substituting for [%s].",*tmp);
	}
	filename=stripfilename(*tmp);
	if(!isEMPTY(filename))
	{
		c=(PaySelStruct *)p->ps;
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
short VIRTUALSubData(char **tmp,mtnstruct *p,char *modulename)
{
	short start=1,length=0,str_length=0;
	char *temp=NULL,*gn;
	int y;
	RDAvirtual *v;

	/* virtual field */
	if(diageval || diagapps)
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
					temp=EVALUATEstr(v->expression,PayjmstSubData,p);
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
					*v->value.short_value=EVALUATEsht(v->expression,PayjmstSubData,p);
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
					*v->value.integer_value=EVALUATEint(v->expression,PayjmstSubData,p);
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
					*v->value.float_value=EVALUATEdbl(v->expression,PayjmstSubData,p);
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
					*v->value.string_value=EVALUATEbol(v->expression,PayjmstSubData,p);
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
					*v->value.string_value=EVALUATEbol(v->expression,PayjmstSubData,p);
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
		return(1);
	}
	return(0);
}
short PAYGLOBALSubData(char **tmp,mtnstruct *p,char *modulename)
{
	short str_length=0;

	if(diageval || diagapps || diagvirtual)
	{
		prterr("DIAG PAYGLOBALSubData Substituting for [%s].",*tmp);
	}
	if(!RDAstrcmp(modulename,"EARNINGS AMOUNT"))
	{
		sprintf(stemp,"0");
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
		sprintf(stemp,"0");
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
	if(!RDAstrcmp(modulename,"UNITS"))
	{
		sprintf(stemp,"%f",UNIT_AMOUNT);
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
static short ExecuteSubData(short (*func)(...),char **tmp,mtnstruct *p,char *modulename)
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
			prterr("Error Fatal Error encountered substitution aborted.....");
		}
		if(diageval || diagapps)
		{
			prterr("DIAG PayjmstSubData Returning [%s].",*tmp);
		}
	}
	return(retval);
}
static void PayjmstSubData(char **tmp,mtnstruct *p)
{
	char *modulename=NULL;

	if(diageval || diagapps)
	{
		prterr("DIAG PayjmstSubData Substituting for [%s].",*tmp);
	}
	if(diageval || diagapps)
	{
		prterr("DIAG PayjmstSubData Returning [%s].",*tmp);
	}
	modulename=stripmodulename(*tmp);
	if(!isEMPTY(modulename))
	{
		if(ExecuteSubData(FIELDSubData,tmp,p,modulename)<1)
		{
			if(modulename!=NULL) Rfree(modulename);
			return;
		}
		if(ExecuteSubData(STRUCTSubData,tmp,p,modulename)<1)
		{
			if(modulename!=NULL) Rfree(modulename);
			return;
		}
		if(ExecuteSubData(VIRTUALSubData,tmp,p,modulename)<1)
		{
			if(modulename!=NULL) Rfree(modulename);
			return;
		}
		if(GLOBALSubData(tmp,modulename)==0)
		{
			if(modulename!=NULL) Rfree(modulename);
			return;
		}
		if(ExecuteSubData(PAYGLOBALSubData,tmp,p,modulename)<1)
		{
			if(modulename!=NULL) Rfree(modulename);
			return;
		}
		if(ExecuteSubData(ScrolledListSubData,tmp,p,NULL)<1)
		{
			if(modulename!=NULL) Rfree(modulename);
			return;
		}
	}
	prterr("Error PayjmstSubData [%s] not found.",*tmp);
	if(*tmp!=NULL) Rfree(*tmp);
	*tmp=stralloc("\"\"");
	if(diageval || diagapps)
	{
		prterr("DIAG PayjmstSubData Returning [%s].",*tmp);
	}
	if(modulename!=NULL) Rfree(modulename);
}
/* BROWSE POSITION TYPE STARTING */
static void doexit_browse_postyp(MakeBrowseList *blist)
{
	Browse_Exit *BE=NULL;

	if(blist!=NULL) 
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
		BE=RETURN_BROWSE_EXIT(blist);
		FreeBrowseExit(BE);
	}
}
static void quit_browse_postyp(MakeBrowseList *BL)
{
	if(BL!=NULL) 
	{
		if(BL->definelist!=NULL) 
		{
			killwindow(BL->definelist);
		}
		if(BL->searchrsrc!=NULL) 
		{
			killwindow(BL->searchrsrc);
		}
		if(BL->mainrsrc!=NULL)
		{
			killwindow(BL->mainrsrc);
		}
	}
}
static void okfunc_browse_postyp(MakeBrowseList *BL,void *targetkey)
{
	char *posid=NULL; 
	int y=0;
	mtnstruct *mtn=NULL;
	RDArsrc *mtnrsrc=NULL;
	RDArmem *mem=NULL;

	ZERNRD(postyp);
	if(ADVEQLNRDKEYsec(postyp,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(postyp,1);
	} else {
		if(posid!=NULL) { Rfree(posid); posid=NULL; }
		FINDFLDGETSTRING(postyp,"POSITION TYPE",&posid);
	}
	filerecord2rsrc(postyp,BL->funcargs);
	FINDRSCSETSTRING(BL->funcargs,"[PAYJMST][POSITION TYPE]",posid);
	FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"POSITION TYPE",posid);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
	y=FINDRSC(BL->funcargs,"GROSS IDENTIFICATIONS");
	if(y>(-1))
	{
		mtnrsrc=(RDArsrc *)BL->funcargs;
		mem=(RDArmem *)mtnrsrc->rscs+y;
		mtn=(mtnstruct *)mem->arglist;
	}
	getsupportingrecords(mtnrsrc,mtn,TRUE);
	quit_browse_postyp(BL);
	if(posid!=NULL) Rfree(posid);
}
static short select_browse_postyp(RDArsrc *mtnrsrc)
{
	return(TRUE);
}
static void init_browse_postyp(MakeBrowseList *BL)
{
	ZERNRD(postyp);
	COPYFIELD(posnum,postyp,"POSITION IDENTIFICATION");
	
}
static short break_browse_postyp(RDArsrc *brsrc,RDArsrc *mtnrsrc)
{
	char *posid1=NULL,*posid2=NULL;

	FINDFLDGETSTRING(posnum,"POSITION IDENTIFICATION",&posid1);
	FINDFLDGETSTRING(postyp,"POSITION IDENTIFICATION",&posid2);
	if(!RDAstrcmp(posid1,posid2)) return(TRUE);
	return(FALSE);
}
void browse_postyp(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_posmstr!=NULL)
	{
		doexit_browse_postyp(mbl_postyp);
		mbl_postyp=NULL;
	}
	mainrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT POSITION TYPE BROWSE");
	searchrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT POSITION TYPE SEARCH BROWSE");
	definelist=RDArsrcNEW(MTNMASTER->module,"SELECT POSITION TYPE DEFINE LIST");
	ZERNRD(postyp);
	addDFincvir(searchrsrc,"POSTRK","POSMSTR",NULL,postyp);
	addDFincvir(definelist,"POSTRK","POSMSTR",NULL,postyp);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	mainrsrc->scn=RDAscrnNEW(mainrsrc->module,mainrsrc->screen);
	findscrnbin(mainrsrc->scn);
	MakeBrowseListTotals(mainrsrc,postyp,definelist);
	mbl_postyp=xbrowse(postyp,1,mainrsrc,searchrsrc,definelist,init_browse_postyp,break_browse_postyp,select_browse_postyp,quit_browse_postyp,okfunc_browse_postyp,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
}
/* BROWSE POSITION TYPE ENDING */
/* BROWSE SUPPLEMENT STARTING */
static void doexit_browse_supplementxx(MakeBrowseList *blist)
{
	Browse_Exit *BE=NULL;

	if(blist!=NULL) 
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
		BE=RETURN_BROWSE_EXIT(blist);
		FreeBrowseExit(BE);
	}
}
static void quit_browse_supplementxx(MakeBrowseList *BL)
{
	if(BL!=NULL) 
	{
		if(BL->definelist!=NULL) 
		{
			killwindow(BL->definelist);
		}
		if(BL->searchrsrc!=NULL) 
		{
			killwindow(BL->searchrsrc);
		}
		if(BL->mainrsrc!=NULL)
		{
			killwindow(BL->mainrsrc);
		}
	}
}
static short select_browse_supplementxx(RDArsrc *mtnrsrc)
{
	return(TRUE);
}
static void init_browse_supplementxx(MakeBrowseList *BL)
{
	ZERNRD(possup);
}
static short break_browse_supplementxx(RDArsrc *brsrc,RDArsrc *mtnrsrc)
{
	return(TRUE);
}
static void okfunc_browse_supplement01(MakeBrowseList *BL,void *targetkey)
{
	int sup=0;
	int y=0;
	mtnstruct *mtn=NULL;
	RDArsrc *mtnrsrc=NULL;
	RDArmem *mem=NULL;

	ZERNRD(possup);
	if(ADVEQLNRDKEYsec(possup,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(possup,1);
	} else {
		FINDFLDGETINT(possup,"SUPPLEMENT",&sup);
	}
	FINDRSCSETINT(BL->funcargs,"[PAYJMST][SUPPLEMENT 01]",sup);
	FINDFLDSETINT(MTNMASTER->passkey->fileno,"SUPPLEMENT 01",sup);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
	y=FINDRSC(BL->funcargs,"GROSS IDENTIFICATIONS");
	if(y>(-1))
	{
		mtnrsrc=(RDArsrc *)BL->funcargs;
		mem=(RDArmem *)mtnrsrc->rscs+y;
		mtn=(mtnstruct *)mem->arglist;
	}
	getsupportingrecords(mtnrsrc,mtn,TRUE);
	quit_browse_supplementxx(BL);
}
void browse_supplement01(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_possup!=NULL)
	{
		mbl_possup->funcargs=(void *)mtnrsrc;
		mbl_possup->okfunction=okfunc_browse_supplement01;
		if(APPmakescrn(mbl_possup->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit_browse_supplementxx(mbl_possup);
		}
	} else {
		mainrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT SUPPLEMENT BROWSE");
		searchrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT SUPPLEMENT SEARCH BROWSE");
		definelist=RDArsrcNEW(MTNMASTER->module,"SELECT SUPPLEMENT DEFINE LIST");
		ZERNRD(possup);
		addDFincvir(searchrsrc,"PAYROLL","PAYGRP",NULL,possup);
		addDFincvir(definelist,"PAYROLL","PAYGRP",NULL,possup);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		mainrsrc->scn=RDAscrnNEW(mainrsrc->module,mainrsrc->screen);
		findscrnbin(mainrsrc->scn);
		MakeBrowseListTotals(mainrsrc,possup,definelist);
		mbl_possup=xbrowse(possup,1,mainrsrc,searchrsrc,definelist,init_browse_supplementxx,break_browse_supplementxx,select_browse_supplementxx,quit_browse_supplementxx,okfunc_browse_supplement01,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
	}
}
static void okfunc_browse_supplement02(MakeBrowseList *BL,void *targetkey)
{
	int sup=0;
	int y=0;
	mtnstruct *mtn=NULL;
	RDArsrc *mtnrsrc=NULL;
	RDArmem *mem=NULL;

	ZERNRD(possup);
	if(ADVEQLNRDKEYsec(possup,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(possup,1);
	} else {
		FINDFLDGETINT(possup,"SUPPLEMENT",&sup);
	}
	FINDRSCSETINT(BL->funcargs,"[PAYJMST][SUPPLEMENT 02]",sup);
	FINDFLDSETINT(MTNMASTER->passkey->fileno,"SUPPLEMENT 02",sup);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
	y=FINDRSC(BL->funcargs,"GROSS IDENTIFICATIONS");
	if(y>(-1))
	{
		mtnrsrc=(RDArsrc *)BL->funcargs;
		mem=(RDArmem *)mtnrsrc->rscs+y;
		mtn=(mtnstruct *)mem->arglist;
	}
	getsupportingrecords(mtnrsrc,mtn,TRUE);
	quit_browse_supplementxx(BL);
}
void browse_supplement02(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_possup!=NULL)
	{
		mbl_possup->funcargs=(void *)mtnrsrc;
		mbl_possup->okfunction=okfunc_browse_supplement02;
		if(APPmakescrn(mbl_possup->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit_browse_supplementxx(mbl_possup);
		}
	} else {
		mainrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT SUPPLEMENT BROWSE");
		searchrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT SUPPLEMENT SEARCH BROWSE");
		definelist=RDArsrcNEW(MTNMASTER->module,"SELECT SUPPLEMENT DEFINE LIST");
		ZERNRD(possup);
		addDFincvir(searchrsrc,"PAYROLL","PAYGRP",NULL,possup);
		addDFincvir(definelist,"PAYROLL","PAYGRP",NULL,possup);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		mainrsrc->scn=RDAscrnNEW(mainrsrc->module,mainrsrc->screen);
		findscrnbin(mainrsrc->scn);
		MakeBrowseListTotals(mainrsrc,possup,definelist);
		mbl_possup=xbrowse(possup,1,mainrsrc,searchrsrc,definelist,init_browse_supplementxx,break_browse_supplementxx,select_browse_supplementxx,quit_browse_supplementxx,okfunc_browse_supplement02,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
	}
}
static void okfunc_browse_supplement03(MakeBrowseList *BL,void *targetkey)
{
	int sup=0;
	int y=0;
	mtnstruct *mtn=NULL;
	RDArsrc *mtnrsrc=NULL;
	RDArmem *mem=NULL;

	ZERNRD(possup);
	if(ADVEQLNRDKEYsec(possup,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(possup,1);
	} else {
		FINDFLDGETINT(possup,"SUPPLEMENT",&sup);
	}
	FINDRSCSETINT(BL->funcargs,"[PAYJMST][SUPPLEMENT 03]",sup);
	FINDFLDSETINT(MTNMASTER->passkey->fileno,"SUPPLEMENT 03",sup);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
	y=FINDRSC(BL->funcargs,"GROSS IDENTIFICATIONS");
	if(y>(-1))
	{
		mtnrsrc=(RDArsrc *)BL->funcargs;
		mem=(RDArmem *)mtnrsrc->rscs+y;
		mtn=(mtnstruct *)mem->arglist;
	}
	getsupportingrecords(mtnrsrc,mtn,TRUE);
	quit_browse_supplementxx(BL);
}
void browse_supplement03(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_possup!=NULL)
	{
		mbl_possup->funcargs=(void *)mtnrsrc;
		mbl_possup->okfunction=okfunc_browse_supplement03;
		if(APPmakescrn(mbl_possup->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit_browse_supplementxx(mbl_possup);
		}
	} else {
		mainrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT SUPPLEMENT BROWSE");
		searchrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT SUPPLEMENT SEARCH BROWSE");
		definelist=RDArsrcNEW(MTNMASTER->module,"SELECT SUPPLEMENT DEFINE LIST");
		ZERNRD(possup);
		addDFincvir(searchrsrc,"PAYROLL","PAYGRP",NULL,possup);
		addDFincvir(definelist,"PAYROLL","PAYGRP",NULL,possup);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		mainrsrc->scn=RDAscrnNEW(mainrsrc->module,mainrsrc->screen);
		findscrnbin(mainrsrc->scn);
		MakeBrowseListTotals(mainrsrc,possup,definelist);
		mbl_possup=xbrowse(possup,1,mainrsrc,searchrsrc,definelist,init_browse_supplementxx,break_browse_supplementxx,select_browse_supplementxx,quit_browse_supplementxx,okfunc_browse_supplement03,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
	}
}
static void okfunc_browse_supplement04(MakeBrowseList *BL,void *targetkey)
{
	int sup=0;
	int y=0;
	mtnstruct *mtn=NULL;
	RDArsrc *mtnrsrc=NULL;
	RDArmem *mem=NULL;

	ZERNRD(possup);
	if(ADVEQLNRDKEYsec(possup,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(possup,1);
	} else {
		FINDFLDGETINT(possup,"SUPPLEMENT",&sup);
	}
	FINDRSCSETINT(BL->funcargs,"[PAYJMST][SUPPLEMENT 04]",sup);
	FINDFLDSETINT(MTNMASTER->passkey->fileno,"SUPPLEMENT 04",sup);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
	y=FINDRSC(BL->funcargs,"GROSS IDENTIFICATIONS");
	if(y>(-1))
	{
		mtnrsrc=(RDArsrc *)BL->funcargs;
		mem=(RDArmem *)mtnrsrc->rscs+y;
		mtn=(mtnstruct *)mem->arglist;
	}
	getsupportingrecords(mtnrsrc,mtn,TRUE);
	quit_browse_supplementxx(BL);
}
void browse_supplement04(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_possup!=NULL)
	{
		mbl_possup->funcargs=(void *)mtnrsrc;
		mbl_possup->okfunction=okfunc_browse_supplement04;
		if(APPmakescrn(mbl_possup->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit_browse_supplementxx(mbl_possup);
		}
	} else {
		mainrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT SUPPLEMENT BROWSE");
		searchrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT SUPPLEMENT SEARCH BROWSE");
		definelist=RDArsrcNEW(MTNMASTER->module,"SELECT SUPPLEMENT DEFINE LIST");
		ZERNRD(possup);
		addDFincvir(searchrsrc,"PAYROLL","PAYGRP",NULL,possup);
		addDFincvir(definelist,"PAYROLL","PAYGRP",NULL,possup);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		mainrsrc->scn=RDAscrnNEW(mainrsrc->module,mainrsrc->screen);
		findscrnbin(mainrsrc->scn);
		MakeBrowseListTotals(mainrsrc,possup,definelist);
		mbl_possup=xbrowse(possup,1,mainrsrc,searchrsrc,definelist,init_browse_supplementxx,break_browse_supplementxx,select_browse_supplementxx,quit_browse_supplementxx,okfunc_browse_supplement04,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
	}
}
static void okfunc_browse_supplement05(MakeBrowseList *BL,void *targetkey)
{
	int sup=0;
	int y=0;
	mtnstruct *mtn=NULL;
	RDArsrc *mtnrsrc=NULL;
	RDArmem *mem=NULL;

	ZERNRD(possup);
	if(ADVEQLNRDKEYsec(possup,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(possup,1);
	} else {
		FINDFLDGETINT(possup,"SUPPLEMENT",&sup);
	}
	FINDRSCSETINT(BL->funcargs,"[PAYJMST][SUPPLEMENT 05]",sup);
	FINDFLDSETINT(MTNMASTER->passkey->fileno,"SUPPLEMENT 05",sup);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
	y=FINDRSC(BL->funcargs,"GROSS IDENTIFICATIONS");
	if(y>(-1))
	{
		mtnrsrc=(RDArsrc *)BL->funcargs;
		mem=(RDArmem *)mtnrsrc->rscs+y;
		mtn=(mtnstruct *)mem->arglist;
	}
	getsupportingrecords(mtnrsrc,mtn,TRUE);
	quit_browse_supplementxx(BL);
}
void browse_supplement05(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_possup!=NULL)
	{
		mbl_possup->funcargs=(void *)mtnrsrc;
		mbl_possup->okfunction=okfunc_browse_supplement05;
		if(APPmakescrn(mbl_possup->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit_browse_supplementxx(mbl_possup);
		}
	} else {
		mainrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT SUPPLEMENT BROWSE");
		searchrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT SUPPLEMENT SEARCH BROWSE");
		definelist=RDArsrcNEW(MTNMASTER->module,"SELECT SUPPLEMENT DEFINE LIST");
		ZERNRD(possup);
		addDFincvir(searchrsrc,"PAYROLL","PAYGRP",NULL,possup);
		addDFincvir(definelist,"PAYROLL","PAYGRP",NULL,possup);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		mainrsrc->scn=RDAscrnNEW(mainrsrc->module,mainrsrc->screen);
		findscrnbin(mainrsrc->scn);
		MakeBrowseListTotals(mainrsrc,possup,definelist);
		mbl_possup=xbrowse(possup,1,mainrsrc,searchrsrc,definelist,init_browse_supplementxx,break_browse_supplementxx,select_browse_supplementxx,quit_browse_supplementxx,okfunc_browse_supplement05,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
	}
}
/* BROWSE SUPPLEMENT ENDING */
/* BROWSE PAY GROUP STARTING */
static void doexit_browse_paygroup(MakeBrowseList *blist)
{
	Browse_Exit *BE=NULL;

	if(blist!=NULL) 
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
		BE=RETURN_BROWSE_EXIT(blist);
		FreeBrowseExit(BE);
	}
}
static void quit_browse_paygroup(MakeBrowseList *BL)
{
	if(BL!=NULL) 
	{
		if(BL->definelist!=NULL) 
		{
			killwindow(BL->definelist);
		}
		if(BL->searchrsrc!=NULL) 
		{
			killwindow(BL->searchrsrc);
		}
		if(BL->mainrsrc!=NULL)
		{
			killwindow(BL->mainrsrc);
		}
	}
}
static void okfunc_browse_paygroup(MakeBrowseList *BL,void *targetkey)
{
	char *locid=NULL; 
	int y=0;
	mtnstruct *mtn=NULL;
	RDArsrc *mtnrsrc=NULL;
	RDArmem *mem=NULL;

	ZERNRD(paygrp);
	if(ADVEQLNRDKEYsec(paygrp,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(paygrp,1);
	} else {
		if(locid!=NULL) { Rfree(locid); locid=NULL; }
		FINDFLDGETSTRING(paygrp,"PAY GROUP",&locid);
	}
	filerecord2rsrc(paygrp,BL->funcargs);
	FINDRSCSETSTRING(BL->funcargs,"[PAYJMST][PAY GROUP]",locid);
	FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"PAY GROUP",locid);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
	y=FINDRSC(BL->funcargs,"GROSS IDENTIFICATIONS");
	if(y>(-1))
	{
		mtnrsrc=(RDArsrc *)BL->funcargs;
		mem=(RDArmem *)mtnrsrc->rscs+y;
		mtn=(mtnstruct *)mem->arglist;
	}
	getsupportingrecords(mtnrsrc,mtn,TRUE);
	quit_browse_paygroup(BL);
	if(locid!=NULL) Rfree(locid);
}
static short select_browse_paygroup(RDArsrc *mtnrsrc)
{
	return(TRUE);
}
static void init_browse_paygroup(MakeBrowseList *BL)
{
	ZERNRD(paygrp);
}
static short break_browse_paygroup(RDArsrc *brsrc,RDArsrc *mtnrsrc)
{
	return(TRUE);
}
void browse_paygroup(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_paygroup!=NULL)
	{
		mbl_paygroup->funcargs=(void *)mtnrsrc;
		if(APPmakescrn(mbl_paygroup->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit_browse_paygroup(mbl_paygroup);
		}
	} else {
		mainrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT PAY GROUP BROWSE");
		searchrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT PAY GROUP SEARCH BROWSE");
		definelist=RDArsrcNEW(MTNMASTER->module,"SELECT PAY GROUP DEFINE LIST");
		ZERNRD(paygrp);
		addDFincvir(searchrsrc,"PAYROLL","PAYGRP",NULL,paygrp);
		addDFincvir(definelist,"PAYROLL","PAYGRP",NULL,paygrp);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		mainrsrc->scn=RDAscrnNEW(mainrsrc->module,mainrsrc->screen);
		findscrnbin(mainrsrc->scn);
		MakeBrowseListTotals(mainrsrc,paygrp,definelist);
		mbl_paygroup=xbrowse(paygrp,1,mainrsrc,searchrsrc,definelist,init_browse_paygroup,break_browse_paygroup,select_browse_paygroup,quit_browse_paygroup,okfunc_browse_paygroup,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
	}
}
/* BROWSE PAY GROUP ENDING */
/* BROWSE LOCATION CODE STARTING */
static void doexit_browse_location(MakeBrowseList *blist)
{
	Browse_Exit *BE=NULL;

	if(blist!=NULL) 
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
		BE=RETURN_BROWSE_EXIT(blist);
		FreeBrowseExit(BE);
	}
}
static void quit_browse_location(MakeBrowseList *BL)
{
	if(BL!=NULL) 
	{
		if(BL->definelist!=NULL) 
		{
			killwindow(BL->definelist);
		}
		if(BL->searchrsrc!=NULL) 
		{
			killwindow(BL->searchrsrc);
		}
		if(BL->mainrsrc!=NULL)
		{
			killwindow(BL->mainrsrc);
		}
	}
}
static void okfunc_browse_location(MakeBrowseList *BL,void *targetkey)
{
	char *locid=NULL; 
	int y=0;
	mtnstruct *mtn=NULL;
	RDArsrc *mtnrsrc=NULL;
	RDArmem *mem=NULL;

	ZERNRD(poslnum);
	if(ADVEQLNRDKEYsec(poslnum,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(poslnum,1);
	} else {
		if(locid!=NULL) { Rfree(locid); locid=NULL; }
		FINDFLDGETSTRING(poslnum,"LOCATION IDENTIFICATION",&locid);
	}
	filerecord2rsrc(poslnum,BL->funcargs);
	FINDRSCSETSTRING(BL->funcargs,"[PAYJMST][LOCATION IDENTIFICATION]",locid);
	FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"LOCATION IDENTIFICATION",locid);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
	y=FINDRSC(BL->funcargs,"GROSS IDENTIFICATIONS");
	if(y>(-1))
	{
		mtnrsrc=(RDArsrc *)BL->funcargs;
		mem=(RDArmem *)mtnrsrc->rscs+y;
		mtn=(mtnstruct *)mem->arglist;
	}
	getsupportingrecords(mtnrsrc,mtn,TRUE);
	quit_browse_location(BL);
	if(locid!=NULL) Rfree(locid);
}
static short select_browse_location(RDArsrc *mtnrsrc)
{
	return(TRUE);
}
static void init_browse_location(MakeBrowseList *BL)
{
	ZERNRD(poslnum);
}
static short break_browse_location(RDArsrc *brsrc,RDArsrc *mtnrsrc)
{
	return(TRUE);
}
void browse_locations(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_locmstr!=NULL)
	{
		mbl_locmstr->funcargs=(void *)mtnrsrc;
		mbl_locmstr->okfunction=okfunc_browse_location;
		if(APPmakescrn(mbl_locmstr->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit_browse_location(mbl_locmstr);
		}
	} else {
		mainrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT LOCATION BROWSE");
		searchrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT LOCATION SEARCH BROWSE");
		definelist=RDArsrcNEW(MTNMASTER->module,"SELECT LOCATION DEFINE LIST");
		ZERNRD(poslnum);
		addDFincvir(searchrsrc,"POSTRK","POSLMST",NULL,poslnum);
		addDFincvir(definelist,"POSTRK","POSLMST",NULL,poslnum);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		mainrsrc->scn=RDAscrnNEW(mainrsrc->module,mainrsrc->screen);
		findscrnbin(mainrsrc->scn);
		MakeBrowseListTotals(mainrsrc,poslnum,definelist);
		mbl_locmstr=xbrowse(poslnum,1,mainrsrc,searchrsrc,definelist,init_browse_location,break_browse_location,select_browse_location,quit_browse_location,okfunc_browse_location,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
	}
}
static void okfunc_browse_clocation(MakeBrowseList *BL,void *targetkey)
{
	char *locid=NULL; 
	int y=0;
	mtnstruct *mtn=NULL;
	RDArsrc *mtnrsrc=NULL;
	RDArmem *mem=NULL;

	ZERNRD(poslnum);
	if(ADVEQLNRDKEYsec(poslnum,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(poslnum,1);
	} else {
		if(locid!=NULL) { Rfree(locid); locid=NULL; }
		FINDFLDGETSTRING(poslnum,"LOCATION IDENTIFICATION",&locid);
	}
	FINDRSCSETSTRING(BL->funcargs,"[PAYJMST][CHARGED LOCATION]",locid);
	FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"CHARGED LOCATION",locid);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
	y=FINDRSC(BL->funcargs,"GROSS IDENTIFICATIONS");
	if(y>(-1))
	{
		mtnrsrc=(RDArsrc *)BL->funcargs;
		mem=(RDArmem *)mtnrsrc->rscs+y;
		mtn=(mtnstruct *)mem->arglist;
	}
	getsupportingrecords(mtnrsrc,mtn,TRUE);
	quit_browse_location(BL);
	if(locid!=NULL) Rfree(locid);
}
void browse_clocations(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_locmstr!=NULL)
	{
		mbl_locmstr->funcargs=(void *)mtnrsrc;
		mbl_locmstr->okfunction=okfunc_browse_clocation;
		if(APPmakescrn(mbl_locmstr->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit_browse_location(mbl_locmstr);
		}
	} else {
		mainrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT LOCATION BROWSE");
		searchrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT LOCATION SEARCH BROWSE");
		definelist=RDArsrcNEW(MTNMASTER->module,"SELECT LOCATION DEFINE LIST");
		ZERNRD(poslnum);
		addDFincvir(searchrsrc,"POSTRK","POSLMST",NULL,poslnum);
		addDFincvir(definelist,"POSTRK","POSLMST",NULL,poslnum);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		mainrsrc->scn=RDAscrnNEW(mainrsrc->module,mainrsrc->screen);
		findscrnbin(mainrsrc->scn);
		MakeBrowseListTotals(mainrsrc,poslnum,definelist);
		mbl_locmstr=xbrowse(poslnum,1,mainrsrc,searchrsrc,definelist,init_browse_location,break_browse_location,select_browse_location,quit_browse_location,okfunc_browse_clocation,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
	}
}
/* BROWSE LOCATION CODE ENDING */
/* BROWSE POSITION CODE STARTING */
static void doexit_browse_position(MakeBrowseList *blist)
{
	Browse_Exit *BE=NULL;

	if(blist!=NULL) 
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
		BE=RETURN_BROWSE_EXIT(blist);
		FreeBrowseExit(BE);
	}
}
static void quit_browse_position(MakeBrowseList *BL)
{
	if(BL!=NULL) 
	{
		if(BL->definelist!=NULL) 
		{
			killwindow(BL->definelist);
		}
		if(BL->searchrsrc!=NULL) 
		{
			killwindow(BL->searchrsrc);
		}
		if(BL->mainrsrc!=NULL)
		{
			killwindow(BL->mainrsrc);
		}
	}
}
static void chgpos(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *posid=NULL; 

	ZERNRD(posnum);
	readwidget(mtnrsrc,"[PAYJMST][POSITION IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[PAYJMST][POSITION IDENTIFICATION]",&posid);
	FINDFLDSETSTRING(posnum,"POSITION IDENTIFICATION",posid);
	if(ADVEQLNRDsec(posnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(posnum,1);
	}
	filerecord2rsrc(posnum,mtnrsrc);
	FINDRSCSETSTRING(mtnrsrc,"[PAYJMST][POSITION IDENTIFICATION]",posid);
	FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"POSITION IDENTIFICATION",posid);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
	getsupportingrecords(mtnrsrc,mtn,TRUE);
	if(posid!=NULL) Rfree(posid);
}
static void chggross(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *grossid=NULL; 

	ZERNRD(grsnum);
	readwidget(mtnrsrc,"[PAYJMST][GROSS IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[PAYJMST][GROSS IDENTIFICATION]",&grossid);
	FINDFLDSETSTRING(grsnum,"GROSS IDENTIFICATION",grossid);
	if(ADVEQLNRDsec(grsnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(grsnum,1);
	}
	filerecord2rsrc(grsnum,mtnrsrc);
	FINDRSCSETSTRING(mtnrsrc,"[PAYJMST][GROSS IDENTIFICATION]",grossid);
	FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"GROSS IDENTIFICATION",grossid);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
	getsupportingrecords(mtnrsrc,mtn,TRUE);
	if(grossid!=NULL) Rfree(grossid);
}
static void chgjfcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *jfcode=NULL; 

	ZERNRD(eeojfnum);
	readwidget(mtnrsrc,"[PAYJMST][EEO JOB FUNCTION CODE]");
	FINDRSCGETSTRING(mtnrsrc,"[PAYJMST][EEO JOB FUNCTION CODE]",&jfcode);
	FINDFLDSETSTRING(eeojfnum,"EEO JOB FUNCTION CODE",jfcode);
	if(ADVEQLNRDsec(eeojfnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(eeojfnum,1);
	}
	filerecord2rsrc(eeojfnum,mtnrsrc);
	FINDRSCSETSTRING(mtnrsrc,"[PAYJMST][EEO JOB FUNCTION CODE]",jfcode);
	FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"EEO JOB FUNCTION CODE",jfcode);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
	getsupportingrecords(mtnrsrc,mtn,TRUE);
	if(jfcode!=NULL) Rfree(jfcode);
}
static void chgjgcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *jgcode=NULL; 

	ZERNRD(eeojgnum);
	readwidget(mtnrsrc,"[PAYJMST][EEO JOB GROUP CODE]");
	FINDRSCGETSTRING(mtnrsrc,"[PAYJMST][EEO JOB GROUP CODE]",&jgcode);
	FINDFLDSETSTRING(eeojgnum,"EEO JOB GROUP CODE",jgcode);
	if(ADVEQLNRDsec(eeojgnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(eeojgnum,1);
	}
	filerecord2rsrc(eeojgnum,mtnrsrc);
	FINDRSCSETSTRING(mtnrsrc,"[PAYJMST][EEO JOB GROUP CODE]",jgcode);
	FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"EEO JOB GROUP CODE",jgcode);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
	getsupportingrecords(mtnrsrc,mtn,TRUE);
	if(jgcode!=NULL) Rfree(jgcode);
}
static void okfunc_browse_position(MakeBrowseList *BL,void *targetkey)
{
	char *posid=NULL; 
	int y=0;
	mtnstruct *mtn=NULL;
	RDArsrc *mtnrsrc=NULL;
	RDArmem *mem=NULL;

	ZERNRD(posnum);
	if(ADVEQLNRDKEYsec(posnum,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(posnum,1);
	} else {
		if(posid!=NULL) { Rfree(posid); posid=NULL; }
		FINDFLDGETSTRING(posnum,"POSITION IDENTIFICATION",&posid);
	}
	filerecord2rsrc(posnum,BL->funcargs);
	FINDRSCSETSTRING(BL->funcargs,"[PAYJMST][POSITION IDENTIFICATION]",posid);
	FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"POSITION IDENTIFICATION",posid);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
	y=FINDRSC(BL->funcargs,"GROSS IDENTIFICATIONS");
	if(y>(-1))
	{
		mtnrsrc=(RDArsrc *)BL->funcargs;
		mem=(RDArmem *)mtnrsrc->rscs+y;
		mtn=(mtnstruct *)mem->arglist;
	}
	getsupportingrecords(mtnrsrc,mtn,TRUE);
	quit_browse_position(BL);
	if(posid!=NULL) Rfree(posid);
}
static short select_browse_position(RDArsrc *mtnrsrc)
{
	return(TRUE);
}
static void init_browse_position(MakeBrowseList *BL)
{
	ZERNRD(posnum);
}
static short break_browse_position(RDArsrc *brsrc,RDArsrc *mtnrsrc)
{
	return(TRUE);
}
void browse_positions(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_posmstr!=NULL)
	{
		mbl_posmstr->funcargs=(void *)mtnrsrc;
		if(APPmakescrn(mbl_posmstr->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit_browse_position(mbl_posmstr);
		}
	} else {
		mainrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT POSITION BROWSE");
		searchrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT POSITION SEARCH BROWSE");
		definelist=RDArsrcNEW(MTNMASTER->module,"SELECT POSITION DEFINE LIST");
		ZERNRD(posnum);
		addDFincvir(searchrsrc,"POSTRK","POSMSTR",NULL,posnum);
		addDFincvir(definelist,"POSTRK","POSMSTR",NULL,posnum);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		mainrsrc->scn=RDAscrnNEW(mainrsrc->module,mainrsrc->screen);
		findscrnbin(mainrsrc->scn);
		MakeBrowseListTotals(mainrsrc,posnum,definelist);
		mbl_posmstr=xbrowse(posnum,1,mainrsrc,searchrsrc,definelist,init_browse_position,break_browse_position,select_browse_position,quit_browse_position,okfunc_browse_position,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
	}
}
/* BROWSE POSITION CODE ENDING */
/* BROWSE GROSS CODE STARTING */
static void doexit_browse_gross(MakeBrowseList *blist)
{
	Browse_Exit *BE=NULL;

	if(blist!=NULL) 
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
		BE=RETURN_BROWSE_EXIT(blist);
		FreeBrowseExit(BE);
	}
}
static void quit_browse_gross(MakeBrowseList *BL)
{
	if(BL!=NULL) 
	{
		if(BL->definelist!=NULL) 
		{
			killwindow(BL->definelist);
		}
		if(BL->searchrsrc!=NULL) 
		{
			killwindow(BL->searchrsrc);
		}
		if(BL->mainrsrc!=NULL)
		{
			killwindow(BL->mainrsrc);
		}
	}
}
static void okfunc_browse_gross(MakeBrowseList *BL,void *targetkey)
{
	int y=0;
	mtnstruct *mtn=NULL;
	RDArsrc *mtnrsrc=NULL;
	RDArmem *mem=NULL;
	char *grossid=NULL; 

	ZERNRD(grsnum);
	if(ADVEQLNRDKEYsec(grsnum,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(posnum,1);
	} else {
		if(grossid!=NULL) { Rfree(grossid); grossid=NULL; }
		FINDFLDGETSTRING(grsnum,"GROSS IDENTIFICATION",&grossid);
	}
	filerecord2rsrc(grsnum,BL->funcargs);
	/*y=FINDRSC(BL->funcargs,"GROSS IDENTIFICATIONS");
	if(y>(-1))
	{
		mtnrsrc=(RDArsrc *)BL->funcargs;
		mem=(RDArmem *)mtnrsrc->rscs+y;
		mtn=(mtnstruct *)mem->arglist;
		x=FINDFLDLISTELEMENT(grsnum,"GROSS IDENTIFICATION",mtn->grosslist,grossid);
		if(x==(-1)) x=0;
		FINDRSCSETINT(BL->funcargs,"GROSS IDENTIFICATIONS",x);
		FINDRSCSETSTRING(BL->funcargs,"[PAYJMST][GROSS IDENTIFICATION]",grossid);
		FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"GROSS IDENTIFICATION",grossid);
		computeallSCRNvirtuals(BL->funcargs);
		updateallrsrc(BL->funcargs);
		getsupportingrecords(mtnrsrc,mtn,TRUE);
	} else {
	}*/
	FINDRSCSETSTRING(BL->funcargs,"[PAYJMST][GROSS IDENTIFICATION]",grossid);
	FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"GROSS IDENTIFICATION",grossid);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
	y=FINDRSC(BL->funcargs,"GROSS IDENTIFICATIONS");
	if(y>(-1))
	{
		mtnrsrc=(RDArsrc *)BL->funcargs;
		mem=(RDArmem *)mtnrsrc->rscs+y;
		mtn=(mtnstruct *)mem->arglist;
	}
	getsupportingrecords(mtnrsrc,mtn,TRUE);
	quit_browse_gross(BL);
	if(grossid!=NULL) Rfree(grossid);
}
static short select_browse_gross(RDArsrc *mtnrsrc)
{
	return(TRUE);
}
static void init_browse_gross(MakeBrowseList *BL)
{
	ZERNRD(grsnum);
}
static short break_browse_gross(RDArsrc *brsrc,RDArsrc *mtnrsrc)
{
	return(TRUE);
}
void browse_grossids(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_grsmstr!=NULL)
	{
		mbl_grsmstr->funcargs=(void *)mtnrsrc;
		if(APPmakescrn(mbl_grsmstr->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit_browse_gross(mbl_grsmstr);
		}
	} else {
		mainrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT GROSS IDENTIFICATION BROWSE");
		searchrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT GROSS IDENTIFICATION SEARCH BROWSE");
		definelist=RDArsrcNEW(MTNMASTER->module,"SELECT GROSS IDENTIFICATION DEFINE LIST");
		ZERNRD(grsnum);
		addDFincvir(searchrsrc,"POSTRK","POSGRS",NULL,grsnum);
		addDFincvir(definelist,"POSTRK","POSGRS",NULL,grsnum);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		mainrsrc->scn=RDAscrnNEW(mainrsrc->module,mainrsrc->screen);
		findscrnbin(mainrsrc->scn);
		MakeBrowseListTotals(mainrsrc,grsnum,definelist);
		mbl_grsmstr=xbrowse(grsnum,1,mainrsrc,searchrsrc,definelist,init_browse_gross,break_browse_gross,select_browse_gross,quit_browse_gross,okfunc_browse_gross,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
	}
}
/* BROWSE GROSS CODE ENDING */
/* BROWSE JOB FUNCTION CODE STARTING */
static void doexit_browse_jfcode(MakeBrowseList *blist)
{
	Browse_Exit *BE=NULL;

	if(blist!=NULL) 
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
		BE=RETURN_BROWSE_EXIT(blist);
		FreeBrowseExit(BE);
	}
}
static void quit_browse_jfcode(MakeBrowseList *BL)
{
	if(BL!=NULL) 
	{
		if(BL->definelist!=NULL) 
		{
			killwindow(BL->definelist);
		}
		if(BL->searchrsrc!=NULL) 
		{
			killwindow(BL->searchrsrc);
		}
		if(BL->mainrsrc!=NULL)
		{
			killwindow(BL->mainrsrc);
		}
	}
}
static void okfunc_browse_jfcode(MakeBrowseList *BL,void *targetkey)
{
	int y=0;
	mtnstruct *mtn=NULL;
	RDArsrc *mtnrsrc=NULL;
	RDArmem *mem=NULL;
	char *jfcode=NULL; 

	ZERNRD(eeojfnum);
	if(ADVEQLNRDKEYsec(eeojfnum,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(eeojfnum,1);
	} else {
		if(jfcode!=NULL) { Rfree(jfcode); jfcode=NULL; }
		FINDFLDGETSTRING(eeojfnum,"EEO JOB FUNCTION CODE",&jfcode);
	}
	filerecord2rsrc(eeojfnum,BL->funcargs);
	FINDRSCSETSTRING(BL->funcargs,"[PAYJMST][EEO JOB FUNCTION CODE]",jfcode);
	FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"EEO JOB FUNCTION CODE",jfcode);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
	y=FINDRSC(BL->funcargs,"GROSS IDENTIFICATIONS");
	if(y>(-1))
	{
		mtnrsrc=(RDArsrc *)BL->funcargs;
		mem=(RDArmem *)mtnrsrc->rscs+y;
		mtn=(mtnstruct *)mem->arglist;
	}
	getsupportingrecords(mtnrsrc,mtn,TRUE);
	quit_browse_jfcode(BL);
	if(jfcode!=NULL) Rfree(jfcode);
}
static short select_browse_jfcode(RDArsrc *mtnrsrc)
{
	return(TRUE);
}
static void init_browse_jfcode(MakeBrowseList *BL)
{
	ZERNRD(eeojfnum);
}
static short break_browse_jfcode(RDArsrc *brsrc,RDArsrc *mtnrsrc)
{
	return(TRUE);
}
void browse_jobfuncs(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_eeojfmstr!=NULL)
	{
		mbl_eeojfmstr->funcargs=(void *)mtnrsrc;
		if(APPmakescrn(mbl_eeojfmstr->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit_browse_jfcode(mbl_eeojfmstr);
		}
	} else {
		mainrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT JOB FUNCTION BROWSE");
		searchrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT JOB FUNCTION SEARCH BROWSE");
		definelist=RDArsrcNEW(MTNMASTER->module,"SELECT JOB FUNCTION DEFINE LIST");
		ZERNRD(eeojfnum);
		addDFincvir(searchrsrc,"POSTRK","POSEEOJF",NULL,eeojfnum);
		addDFincvir(definelist,"POSTRK","POSEEOJF",NULL,eeojfnum);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		mainrsrc->scn=RDAscrnNEW(mainrsrc->module,mainrsrc->screen);
		findscrnbin(mainrsrc->scn);
		MakeBrowseListTotals(mainrsrc,eeojfnum,definelist);
		mbl_eeojfmstr=xbrowse(eeojfnum,1,mainrsrc,searchrsrc,definelist,init_browse_jfcode,break_browse_jfcode,select_browse_jfcode,quit_browse_jfcode,okfunc_browse_jfcode,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
	}
}
/* BROWSE JOB FUNCTION CODE ENDING */
/* BROWSE JOB GROUP CODE STARTING */
static void doexit_browse_jgcode(MakeBrowseList *blist)
{
	Browse_Exit *BE=NULL;

	if(blist!=NULL) 
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
		BE=RETURN_BROWSE_EXIT(blist);
		FreeBrowseExit(BE);
	}
}
static void quit_browse_jgcode(MakeBrowseList *BL)
{
	if(BL!=NULL) 
	{
		if(BL->definelist!=NULL) 
		{
			killwindow(BL->definelist);
		}
		if(BL->searchrsrc!=NULL) 
		{
			killwindow(BL->searchrsrc);
		}
		if(BL->mainrsrc!=NULL)
		{
			killwindow(BL->mainrsrc);
		}
	}
}
static void okfunc_browse_jgcode(MakeBrowseList *BL,void *targetkey)
{
	int y=0;
	mtnstruct *mtn=NULL;
	RDArsrc *mtnrsrc=NULL;
	RDArmem *mem=NULL;
	char *jgcode=NULL; 

	ZERNRD(eeojgnum);
	if(ADVEQLNRDKEYsec(eeojgnum,1,targetkey,SCRNvirtualSubData,BL->mainrsrc))
	{
		KEYNRD(eeojgnum,1);
	} else {
		if(jgcode!=NULL) { Rfree(jgcode); jgcode=NULL; }
		FINDFLDGETSTRING(eeojgnum,"EEO JOB GROUP CODE",&jgcode);
	}
	filerecord2rsrc(eeojgnum,BL->funcargs);
	FINDRSCSETSTRING(BL->funcargs,"[PAYJMST][EEO JOB GROUP CODE]",jgcode);
	FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"EEO JOB GROUP CODE",jgcode);
	computeallSCRNvirtuals(BL->funcargs);
	updateallrsrc(BL->funcargs);
	y=FINDRSC(BL->funcargs,"GROSS IDENTIFICATIONS");
	if(y>(-1))
	{
		mtnrsrc=(RDArsrc *)BL->funcargs;
		mem=(RDArmem *)mtnrsrc->rscs+y;
		mtn=(mtnstruct *)mem->arglist;
	}
	getsupportingrecords(mtnrsrc,mtn,TRUE);
	quit_browse_jgcode(BL);
	if(jgcode!=NULL) Rfree(jgcode);
}
static short select_browse_jgcode(RDArsrc *mtnrsrc)
{
	return(TRUE);
}
static void init_browse_jgcode(MakeBrowseList *BL)
{
	ZERNRD(eeojgnum);
}
static short break_browse_jgcode(RDArsrc *brsrc,RDArsrc *mtnrsrc)
{
	return(TRUE);
}
void browse_jobgroups(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mbl_eeojgmstr!=NULL)
	{
		mbl_eeojgmstr->funcargs=(void *)mtnrsrc;
		if(APPmakescrn(mbl_eeojgmstr->mainrsrc,TRUE,NULL,NULL,FALSE)==(-1))
		{
			doexit_browse_jgcode(mbl_eeojgmstr);
		}
	} else {
		mainrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT JOB GROUP BROWSE");
		searchrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT JOB GROUP SEARCH BROWSE");
		definelist=RDArsrcNEW(MTNMASTER->module,"SELECT JOB GROUP DEFINE LIST");
		ZERNRD(eeojgnum);
		addDFincvir(searchrsrc,"POSTRK","POSEEOJG",NULL,eeojgnum);
		addDFincvir(definelist,"POSTRK","POSEEOJG",NULL,eeojgnum);
		GET_SCREEN_VIRTUALS(definelist,1);
		GET_SCREEN_VIRTUALS(searchrsrc,2);
		mainrsrc->scn=RDAscrnNEW(mainrsrc->module,mainrsrc->screen);
		findscrnbin(mainrsrc->scn);
		MakeBrowseListTotals(mainrsrc,eeojgnum,definelist);
		mbl_eeojgmstr=xbrowse(eeojgnum,1,mainrsrc,searchrsrc,definelist,init_browse_jgcode,break_browse_jgcode,select_browse_jgcode,quit_browse_jgcode,okfunc_browse_jgcode,NULL,mtnrsrc,TRUE,NULL,0,0,0,0,NULL,0,1,__LINE__,__FILE__);
	}
}
/* BROWSE JOB GROUP CODE ENDING */
static void setlist(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *temp=NULL;
	int x=0;

	if(grsnum!=(-1))
	{
		FINDFLDGETSTRING(MTNMASTER->passkey->fileno,"GROSS IDENTIFICATION",&temp);
		x=FINDFLDLISTELEMENT(grsnum,"GROSS IDENTIFICATION",mtn->grosslist,temp);
		if(x==(-1)) x=0;
		FINDRSCSETINT(mtnrsrc,"GROSS IDENTIFICATIONS",x);
	}
	if(temp!=NULL) Rfree(temp);
	if(update)
	{
		updatersrc(mtnrsrc,"GROSS IDENTIFICATIONS");
	}
}
static void UpdateGROSSAmounts(RDArsrc *mtnrsrc,mtnstruct *mtn,char *grossid,short update)
{
	char *temp=NULL,*tmp=NULL,*tmp1=NULL,*tmp2=NULL;
	int x,jobno=0,len=0,len1=0;
	RateStruct *r=NULL;
	double total=0.0;

	FINDRSCGETINT(mtnrsrc,"[PAYJMST][JOB NUMBER]",&jobno);
	FINDRSCGETSTRING(mtnrsrc,"[PAYJMST][PERSONNEL IDENTIFICATION]",&temp);
	if(mtn->grlist!=NULL) freeapplib(mtn->grlist);
	mtn->grlist=APPlibNEW();
	if(mtn->ps->perid!=NULL) Rfree(mtn->ps->perid);
	mtn->ps->perid=stralloc(temp);
	mtn->ps->jobno=jobno;
	if(mtn->ps->grossid!=NULL) Rfree(mtn->ps->grossid);
	mtn->ps->grossid=stralloc(grossid);
	FreeRateStruct(mtn->ps);
	makepaystruct(mtnrsrc,mtn->ps,mtn);
	SortRateStruct(mtn->ps);
	ComputePaySelStruct(mtn->ps,mtnrsrc,mtn);
	if(temp!=NULL) Rfree(temp);
	if(mtn->ps->rates!=NULL)
	{
		for(x=0,r=mtn->ps->rates;x<mtn->ps->num;++x,++r)
		{
			len1=RDAstrlen(r->rateid);
			if(len1>len) len=len1;
		}
		len1=len+50;
		temp=Rmalloc(len1);
		total=0.0;
		for(x=0,r=mtn->ps->rates;x<mtn->ps->num;++x,++r)
		{
			tmp=famt(r->gross,15);
			tmp1=famt(r->rate,15);
			tmp2=floatamt(r->units,15);
			memset(temp,0,len1);
			sprintf(stemp,"%*s: %15s %15s %15s",len,r->rateid,tmp1,tmp,tmp2);
			addAPPlib(mtn->grlist,stemp);
			if(tmp!=NULL) Rfree(tmp);
			if(tmp1!=NULL) Rfree(tmp1);
			if(tmp2!=NULL) Rfree(tmp2);
			if(RDAstrcmp(r->rateid,"ADVANCE")) total+=r->gross;
		}
		if(mtn->grlist->numlibs>0)
		{
			sprintf(stemp,"%*s  %15s %15s %15s",len,"","","---------------","");
			addAPPlib(mtn->grlist,stemp);
			memset(temp,0,len1);
			tmp=famt(total,15);
			sprintf(stemp,"%*s: %15s %15s %15s",len,"TOTAL","",tmp,"");
			addAPPlib(mtn->grlist,stemp);
			if(tmp!=NULL) Rfree(tmp);
		}
		if(temp!=NULL) Rfree(temp);
	}
	if(mtn->grlist->numlibs<1)
	{
		addAPPlib(mtn->grlist,"No Pay Amounts Available");
	}
	if(update)
	{
		FINDRSCLISTAPPlib(mtnrsrc,"PAY AMOUNT LIST",0,mtn->grlist);
		updatersrc(mtnrsrc,"PAY AMOUNT LIST");
	}
}
static void grossidcb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	getsupportingrecords(mtnrsrc,mtn,TRUE);
}
static void getsupportingrecords(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *grossid=NULL,*jfcode=NULL,*jgcode=NULL;
	int fyear=0,cyear=0;

	if(payjfsm!=(-1))
	{
		ZERNRD(payjfsm);
		readwidget(mtnrsrc,"FISCAL YEAR");
		FINDRSCGETINT(mtnrsrc,"FISCAL YEAR",&fyear);
		COPYFIELD(MTNMASTER->passkey->fileno,payjfsm,"PERSONNEL IDENTIFICATION");
		COPYFIELD(MTNMASTER->passkey->fileno,payjfsm,"JOB NUMBER");
		FINDFLDSETINT(payjfsm,"FISCAL YEAR",fyear);
		if(ADVEQLNRDsec(payjfsm,1,PayjmstSubData,mtn)) KEYNRD(payjfsm,1);
	}
	if(payjcsm!=(-1))
	{
		ZERNRD(payjcsm);
		readwidget(mtnrsrc,"CALENDAR YEAR");
		FINDRSCGETINT(mtnrsrc,"CALENDAR YEAR",&cyear);
		COPYFIELD(MTNMASTER->passkey->fileno,payjcsm,"PERSONNEL IDENTIFICATION");
		COPYFIELD(MTNMASTER->passkey->fileno,payjcsm,"JOB NUMBER");
		FINDFLDSETINT(payjfsm,"CALENDAR YEAR",cyear);
		if(ADVEQLNRDsec(payjcsm,1,PayjmstSubData,mtn)) KEYNRD(payjcsm,1);
	}
	if(grsnum!=(-1))
	{
		/*FINDRSCGETINT(mtnrsrc,"GROSS IDENTIFICATIONS",&selgross);*/
		/*if(mtn!=NULL)
		{
			if(mtn->grosslist!=NULL)
			{
				if(mtn->grosslist->libs!=NULL)
				{
					if(mtn->grosslist->numlibs>selgross)
					{
					if(RDAstrcmp(mtn->grosslist->libs[selgross],"No Gross Definitions"))
					{
						FINDFLDSETSTRING(grsnum,"GROSS IDENTIFICATION",
							mtn->grosslist->libs[selgross]);
						grossid=stralloc(mtn->grosslist->libs[selgross]);
					} else {
						FINDFLDSETSTRING(grsnum,"GROSS IDENTIFICATION",NULL);
					}
					}
				}
			}
		}*/
		FINDRSCGETSTRING(mtnrsrc,"[PAYJMST][GROSS IDENTIFICATION]",&grossid);
		FINDFLDSETSTRING(grsnum,"GROSS IDENTIFICATION",grossid);
		if(ADVEQLNRDsec(grsnum,1,PayjmstSubData,mtn)) KEYNRD(grsnum,1);
		COPYFIELD(grsnum,MTNMASTER->passkey->fileno,"GROSS IDENTIFICATION");
		if(update) updatefilerecord2rsrc(grsnum,mtnrsrc);
		else filerecord2rsrc(grsnum,mtnrsrc);
	}
	UpdateGROSSAmounts(mtnrsrc,mtn,grossid,update);
	if(grossid!=NULL) Rfree(grossid)
	if(eeojfnum!=(-1))
	{
		ZERNRD(eeojfnum);
		FINDRSCGETSTRING(mtnrsrc,"EEO JOB FUNCTION CODE",&jfcode);
		COPYFIELD(MTNMASTER->passkey->fileno,eeojfnum,"EEO JOB FUNCTION CODE");
		FINDFLDSETSTRING(eeojfnum,"EEO JOB FUNCTION CODE",jfcode);
		if(ADVEQLNRDsec(eeojfnum,1,PayjmstSubData,mtn)) KEYNRD(eeojfnum,1);
	}
	if(eeojgnum!=(-1))
	{
		ZERNRD(eeojgnum);
		FINDRSCGETSTRING(mtnrsrc,"EEO JOB GROUP CODE",&jgcode);
		COPYFIELD(MTNMASTER->passkey->fileno,eeojgnum,"EEO JOB GROUP CODE");
		FINDFLDSETSTRING(eeojgnum,"EEO JOB GROUP CODE",jgcode);
		if(ADVEQLNRDsec(eeojgnum,1,PayjmstSubData,mtn)) KEYNRD(eeojgnum,1);
	}
}
static void getpreviouscb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MTNMASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1))
	{
		keyno=MTNMASTER->passkey->keyno;
	}
	ADV2GET_PREVIOUS(member->parent,MTNMASTER,mtn->previous,PayjmstSubData,mtn,
		keyno,NEfunction,mtn,NULL,NULL,AUEQLfunction,mtn);
}
static void getnextcb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MTNMASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1))
	{
		keyno=MTNMASTER->passkey->keyno;
	}
	ADV2GET_NEXT(member->parent,MTNMASTER,mtn->previous,PayjmstSubData,mtn,
		keyno,NEfunction,mtn,NULL,NULL,AUEQLfunction,mtn);
}
void payjmstm(short dowhich,void *targetkey)
{
	int fyear=0,cyear=0,which_type=0;
	char *name=NULL;
	mtnstruct *mtn=NULL;
	short nofields=0,nokeys=0,x,y,edit_rsrc=FALSE;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDArsrc *mtnrsrc=NULL;
	
	mtnrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->maintain_screen);
	ZERNRD(MTNMASTER->passkey->fileno);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->grosslist=NULL;
	mtn->grlist=NULL;
	mtn->previous=NULL;
	mtn->previous=RDATDataNEW(MTNMASTER->passkey->fileno);
	mtn->ps=PaySelStructNew(NULL,0,NULL,NULL,NULL);
	mtn->r=mtnrsrc;
	nokeys=NUMKEYS(MTNMASTER->passkey->fileno);
	keys=KEYPOINTER(MTNMASTER->passkey->fileno);
	nofields=NUMFLDS(MTNMASTER->passkey->fileno);
	fields=FLDPOINTER(MTNMASTER->passkey->fileno);
	GET_MAINTAIN_INCVARS(mtnrsrc,MTNMASTER);
	GET_ALL_SCREEN_VIRTUALS(mtnrsrc,0);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=editable;
			if(!RDAstrcmp(fldx->name,"GROSS IDENTIFICATION"))
			{
				mtn->grosslist=ADVMakeNRDlistAllSTRING(grsnum,"POSTRK",
					"Creating Gross Identification List",1,
					"GROSS IDENTIFICATION",PayjmstSubData,mtn);
				if(mtn->grosslist->numlibs<1)
				{
					addAPPlib(mtn->grosslist,"No Gross Definitions");
				}
				if(FIELDscrn(MTNMASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					which_type=0;
					addlstrsrc(mtnrsrc,"GROSS IDENTIFICATIONS",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						grossidcb,mtn->grosslist->numlibs,
						&mtn->grosslist->libs,mtn);
				}
			}
			nonlstmakefld(mtnrsrc,MTNMASTER->passkey->fileno,fldx,edit_rsrc);
			if(name!=NULL)
			{
				name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(MTNMASTER->mainfile));
			} else {
				name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(MTNMASTER->mainfile));
			}
			sprintf(name,"[%s][%s]",MTNMASTER->mainfile,fldx->name);
			FINDRSCSETFUNC(mtnrsrc,name,SETFILES,mtn);
		}
	}
	if(keys!=NULL)
	{
		for(x=0,keyx=keys;x<nokeys;++x,++keyx)
		{
			if(keyx->part!=NULL)
			{
				for(y=0,part=keyx->part;y<keyx->numparts;++y,++part)
				{
					if(name!=NULL)
					{
						name=Rrealloc(name,RDAstrlen(part->name)+5+RDAstrlen(MTNMASTER->mainfile));
					} else {
						name=Rmalloc(RDAstrlen(part->name)+5+RDAstrlen(MTNMASTER->mainfile));
					}
					sprintf(name,"[%s][%s]",MTNMASTER->mainfile,part->name);
					FINDRSCSETFUNC(mtnrsrc,name,SETID,mtn);
					FINDRSCSETEDITABLE(mtnrsrc,name,TRUE);
				}
			}
		}
	}
	if(name!=NULL) Rfree(name);
	addstdrsrc(mtnrsrc,"PAY BALANCE OF CONTRACT",BOOLNS,1,&Pay_Balance_of_Contract,TRUE);
	addstdrsrc(mtnrsrc,"DUMP DEFERRED PAY BALANCE",BOOLNS,1,&Dump_Deferred_Pay_Balance,TRUE);
	FINDRSCSETFUNC(mtnrsrc,"PAY BALANCE OF CONTRACT",SETFILES,mtn);
	FINDRSCSETFUNC(mtnrsrc,"DUMP DEFERRED PAY BALANCE",SETFILES,mtn);
	SET_MAINTAIN_FILE2RSRCS(mtnrsrc,MTNMASTER);
	SCRNvirtual2rsrc(mtnrsrc);
	addstdrsrc(mtnrsrc,"FISCAL YEAR",LONGV,4,&fyear,TRUE);
	FINDRSCSETFUNC(mtnrsrc,"FISCAL YEAR",SETFILES,mtn);
	addstdrsrc(mtnrsrc,"CALENDAR YEAR",LONGV,4,&cyear,TRUE);
	FINDRSCSETFUNC(mtnrsrc,"CALENDAR YEAR",SETFILES,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[PAYJMST][POSITION IDENTIFICATION]",chgpos,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[PAYJMST][GROSS IDENTIFICATION]",chggross,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[PAYJMST][EEO JOB FUNCTION CODE]",chgjfcode,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[PAYJMST][EEO JOB GROUP CODE]",chgjgcode,mtn);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			ApplyPassKey(MTNMASTER->passkey);
			if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,PayjmstSubData,mtn))
			{
				KEYNRD(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
				if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,PayjmstSubData,mtn))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
				} else {
					GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
					filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
					setlist(mtnrsrc,mtn,FALSE);
				}
			} else {
				GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
				setlist(mtnrsrc,mtn,FALSE);
			}
			break;
		case 2:
			ApplyPassKey(MTNMASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
			if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,PayjmstSubData,mtn))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
			} else {
				GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
				setlist(mtnrsrc,mtn,FALSE);
			}
			break;
		case 0:
		default:
			if(ADVEQLNRDKEYsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,targetkey,PayjmstSubData,mtn))
			{
				ZERNRD(MTNMASTER->passkey->fileno);
				ApplyPassKey(MTNMASTER->passkey);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
				if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,PayjmstSubData,mtn))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
				} else {
					GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
					filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
					setlist(mtnrsrc,mtn,FALSE);
				}
			} else {
				filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
				setlist(mtnrsrc,mtn,FALSE);
				GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
			}
			break;
	}
	which_type=0;
	getsupportingrecords(mtnrsrc,mtn,FALSE);
	addlstrsrc(mtnrsrc,"PAY AMOUNT LIST",&which_type,TRUE,
		NULL,mtn->grlist->numlibs,&mtn->grlist->libs,NULL);
        GET_SUPPORTING(mtnrsrc,MTNMASTER,PayjmstSubData,mtn,FALSE,FALSE);
	ADVSetVirFieldFuncs(mtnrsrc,UpdateVirtuals,mtn);
	addbtnrsrc(mtnrsrc,"LOAD POSITIONS",TRUE,browse_positions,NULL);
	addbtnrsrc(mtnrsrc,"LOAD POSITION TYPE",TRUE,browse_postyp,NULL);
	addbtnrsrc(mtnrsrc,"LOAD LOCATIONS",TRUE,browse_locations,NULL);
	addbtnrsrc(mtnrsrc,"LOAD CHARGED LOCATIONS",TRUE,browse_clocations,NULL);
	addbtnrsrc(mtnrsrc,"LOAD PAY GROUP",TRUE,browse_paygroup,NULL);
	addbtnrsrc(mtnrsrc,"LOAD GROSS",TRUE,browse_grossids,NULL);
	addbtnrsrc(mtnrsrc,"LOAD JOB FUNCTIONS",TRUE,browse_jobfuncs,NULL);
	addbtnrsrc(mtnrsrc,"LOAD JOB GROUPS",TRUE,browse_jobgroups,NULL);
	addbtnrsrc(mtnrsrc,"LOAD SUPPLEMENT 01",TRUE,browse_supplement01,NULL);
	addbtnrsrc(mtnrsrc,"LOAD SUPPLEMENT 02",TRUE,browse_supplement02,NULL);
	addbtnrsrc(mtnrsrc,"LOAD SUPPLEMENT 03",TRUE,browse_supplement03,NULL);
	addbtnrsrc(mtnrsrc,"LOAD SUPPLEMENT 04",TRUE,browse_supplement04,NULL);
	addbtnrsrc(mtnrsrc,"LOAD SUPPLEMENT 05",TRUE,browse_supplement05,NULL);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
	if(mbl_payjmst!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_payjmst,mtn);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,mtn);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	ADVMakePrevNextButtons(mtnrsrc,keys,nokeys,getpreviouscb,mtn,getnextcb,mtn);
/*
	MakePowerMasterButtons(mtnrsrc,mtn,SETFILES);
*/
	MakeMasterButtons(mtnrsrc,MTNMASTER);
	mtnrsrc->EvalFunc=PayjmstSubData;
	mtnrsrc->EvalFuncArgs=mtn;
	ADVcomputeandupdateallSCRNvirtuals(mtnrsrc,PayjmstSubData,mtn,FALSE);
	if(mbl_payjmst!=NULL) FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_payjmst==NULL?TRUE:FALSE));
}
static void resetdefs(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MTNMASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	updateallrsrc(mtnrsrc);
	FINDRSCGETCHAR(mtnrsrc,"PAY BALANCE OF CONTRACT",&Pay_Balance_of_Contract);
	FINDRSCGETCHAR(mtnrsrc,"DUMP DEFERRED PAY BALANCE",&Dump_Deferred_Pay_Balance);
	SETID(mtnrsrc,mtn);
}
static short delposper(mtnstruct *mtn)
{
	RDATData *prev_posper=NULL;

	COPYFIELD(MTNMASTER->passkey->fileno,pospernum,"POSITION IDENTIFICATION");
	COPYFIELD(MTNMASTER->passkey->fileno,pospernum,"PERSONNEL IDENTIFICATION");
	COPYFIELD(MTNMASTER->passkey->fileno,pospernum,"JOB NUMBER");
	if(!ADVEQLNRDsec(pospernum,1,PayjmstSubData,mtn))
	{
		LOCNRDFILE(pospernum);
		prev_posper=RDATDataNEW(pospernum);
		FINDFLDSETCHAR(pospernum,"DELETEFLAG",TRUE);
		if(ADVWRTTRANSsec(pospernum,0,NULL,prev_posper,PayjmstSubData,mtn))
		{
			ERRORDIALOG("WRITE ERROR OCCURRED!","The write function failed for supporting record [POSTRK][POSPER].  Therefore, this record cannot be saved.",NULL,FALSE);
			UNLNRDFILE(pospernum);
			if(prev_posper!=NULL) FreeRDATData(prev_posper);
			return(TRUE);
		}
	}
	UNLNRDFILE(pospernum);
	if(prev_posper!=NULL) FreeRDATData(prev_posper);
	return(FALSE);
}
static short save_posper(mtnstruct *mtn)
{
	char delflag=FALSE;
	RDATData *prev_posper=NULL;

	COPYFIELD(MTNMASTER->passkey->fileno,pospernum,"POSITION IDENTIFICATION");
	COPYFIELD(MTNMASTER->passkey->fileno,pospernum,"PERSONNEL IDENTIFICATION");
	COPYFIELD(MTNMASTER->passkey->fileno,pospernum,"JOB NUMBER");
	FINDFLDGETCHAR(MTNMASTER->passkey->fileno,"DELETEFLAG",&delflag);
	if(!ADVEQLNRDsec(pospernum,1,PayjmstSubData,mtn))
	{
		LOCNRDFILE(pospernum);
		prev_posper=RDATDataNEW(pospernum);
	}
	FINDFLDSETCHAR(pospernum,"DELETEFLAG",delflag);
	if(ADVWRTTRANSsec(pospernum,0,NULL,prev_posper,PayjmstSubData,mtn))
	{
		ERRORDIALOG("WRITE ERROR OCCURRED!","The write function failed for supporting record [POSTRK][POSPER].  Therefore, this record cannot be saved.",NULL,FALSE);
		UNLNRDFILE(pospernum);
		if(prev_posper!=NULL) FreeRDATData(prev_posper);
		return(TRUE);
	}
	UNLNRDFILE(pospernum);
	if(prev_posper!=NULL) FreeRDATData(prev_posper);
	return(FALSE);
}
static void save_check(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	char delflag=FALSE,*grossid=NULL,*desc=NULL,manage_fte1=FALSE;

	COPYFIELD(MTNMASTER->passkey->fileno,pospernum,"POSITION IDENTIFICATION");
	COPYFIELD(MTNMASTER->passkey->fileno,pospernum,"PERSONNEL IDENTIFICATION");
	COPYFIELD(MTNMASTER->passkey->fileno,pospernum,"JOB NUMBER");
	FINDFLDGETCHAR(posnum,"MANAGED FTE",&manage_fte1);
	if(ADVEQLNRDsec(pospernum,1,PayjmstSubData,mtn) && manage_fte==FALSE && manage_fte1==TRUE)
	{
		desc=Rmalloc(RDAstrlen(grossid)+125);
		sprintf(desc,"Hire Applicant/Hire Employee must be used to create new jobs, you may not add new jobs in the Job Master.");
		WARNINGDIALOG("WARNING DIALOG SCREEN","NO NEW JOBS!",desc,NULL,NULL,TRUE,NULL);
		if(desc!=NULL) Rfree(desc);
		getsupportingrecords(mtnrsrc,mtn,TRUE);
		GET_SUPPORTING(mtnrsrc,MTNMASTER,PayjmstSubData,mtn,TRUE,FALSE);
		return;
	}
	FINDRSCGETCHAR(mtnrsrc,"[PAYJMST][DELETEFLAG]",&delflag);
	if(!delflag)
	{
		FINDFLDGETSTRING(MTNMASTER->passkey->fileno,"GROSS IDENTIFICATION",&grossid);
		FINDFLDSETSTRING(grsnum,"GROSS IDENTIFICATION",grossid);
		if(ADVEQLNRDsec(grsnum,1,PayjmstSubData,mtn))
		{
			desc=Rmalloc(RDAstrlen(grossid)+125);
			sprintf(desc,"The Gross Identification of [%s] is an invalid value.  Please select a valid Gross Identification to save this record.",(grossid==NULL?"":grossid));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID GROSS IDENTIFICATION!",desc,NULL,NULL,TRUE,NULL);
			if(desc!=NULL) Rfree(desc);
			if(grossid!=NULL) Rfree(grossid);
			getsupportingrecords(mtnrsrc,mtn,TRUE);
			GET_SUPPORTING(mtnrsrc,MTNMASTER,PayjmstSubData,mtn,TRUE,FALSE);
			return;
		} else {
			FINDFLDGETCHAR(grsnum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				desc=Rmalloc(RDAstrlen(grossid)+125);
				sprintf(desc,"The Gross Identification of [%s] is a deleted value.  Please select a valid Gross Identification to save this record.",(grossid==NULL?"":grossid));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED GROSS IDENTIFICATION!",desc,NULL,NULL,TRUE,NULL);
				if(desc!=NULL) Rfree(desc);
				if(grossid!=NULL) Rfree(grossid);
				getsupportingrecords(mtnrsrc,mtn,TRUE);
				GET_SUPPORTING(mtnrsrc,MTNMASTER,PayjmstSubData,mtn,TRUE,FALSE);
				return;
			}
		}
		if(grossid!=NULL) Rfree(grossid);
	}
	if(!SAVE_CHECK(mtnrsrc,MTNMASTER,PayjmstSubData,(void *)mtn))
	{
		return;
	}
	if(!isEMPTY(MTNMASTER->save_expression))
	{
		if(EVALUATEbol(MTNMASTER->save_expression,PayjmstSubData,mtn))
		{
			desc=EVALUATEstr(MTNMASTER->save_error_desc,
				PayjmstSubData,mtn);
			ERRORDIALOG("SAVE ERROR",desc,NULL,FALSE);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	if(!isEMPTY(MTNMASTER->save_warning))
	{
		if(EVALUATEbol(MTNMASTER->save_warning,PayjmstSubData,mtn))
		{
			desc=EVALUATEstr(MTNMASTER->save_warning_desc,
				PayjmstSubData,mtn);
			ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","SAVE WARNING",
				desc,(update_list ? save_record_now:save_record_now1),
				NULL,FALSE,2,mtnrsrc,mtn,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	save_record(mtnrsrc,mtn,update_list);
}
static void save_record_now(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_record(mtnrsrc,mtn,TRUE);
}
static void save_record_now1(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_record(mtnrsrc,mtn,FALSE);
}
static void save_record_up(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readscreen(mtnrsrc,mtn);
	save_check(mtnrsrc,mtn,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readscreen(mtnrsrc,mtn);
	save_check(mtnrsrc,mtn,FALSE);
}
static void save_record_quit(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,TRUE);
}
static void save_record(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	char *rcddesc=NULL,*posid1=NULL,prv_active=FALSE;
	char managed_fte=FALSE;
	char delflag=FALSE,active=FALSE,terminate=FALSE;
	double fteamount=0.0;
	short numflds=0,y=0;
	NRDfield *tmpfld=NULL,*fldx=NULL,*tmp=NULL;
	MTNPassableStruct *PSTRUCT=NULL;
#ifdef LONGEVITY_WHEN_INACTIVE 
	RDAvirtual *v=NULL;
	RDATData *prev=NULL;
	double district=0.0;
#endif /* LONGEVITY_WHEN_INACTIVE */

	if(ADVRECORDsec(MTNMASTER->passkey->fileno,PayjmstSubData,mtn))
	{
		rcddesc=ADVRECORDsecDesc(MTNMASTER->passkey->fileno,PayjmstSubData,mtn);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("SECURITY ACCESS DENIED!",rcddesc,NULL,FALSE); 
			prterr("Security Access Denied [%s].  Didn't write record.",rcddesc);
			Rfree(rcddesc);
		} else {
			prterr("Error didn't write record.");
		}
	} else {
		PSTRUCT=Rmalloc(sizeof(MTNPassableStruct));
		PSTRUCT->MTNMASTER=MTNMASTER;
		PSTRUCT->quitfunc=NULL;
		PSTRUCT->fromrsrc=mtnrsrc;
		PSTRUCT->fromargs=NULL;
		PSTRUCT->args=NULL;
		MTNrun_presave_buttons(mtnrsrc,PSTRUCT,TRUE);
		numflds=NUMFLDS(MTNMASTER->passkey->fileno);
		if(numflds>0)
		{
			tmpfld=Rmalloc(numflds*sizeof(NRDfield));
			for(y=0,fldx=tmpfld;y<numflds;++y,++fldx)
			{
				tmp=FLDNUM(MTNMASTER->passkey->fileno,y);
				cpfldinfo(fldx,tmp);
			}
			if(!ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,PayjmstSubData,mtn))
			{
				FINDFLDGETCHAR(MTNMASTER->passkey->fileno,"ACTIVE",&prv_active);
				if(delposper(mtn))
				{
					if(tmpfld!=NULL) Rfreetmpfld(tmpfld,numflds);
					return;
				}
				FINDFLDGETCHAR(MTNMASTER->passkey->fileno,"ACTIVE",&active);
				FINDFLDGETCHAR(MTNMASTER->passkey->fileno,"DELETEFLAG",&delflag);
				FINDFLDGETCHAR(MTNMASTER->passkey->fileno,"TERMINATE AT ENDING DATE",
					&terminate);
				if(active==TRUE && delflag==FALSE && !terminate)
				{
					FINDFLDGETSTRING(MTNMASTER->passkey->fileno,"POSITION IDENTIFICATION",&posid1);
					FINDFLDGETDOUBLE(MTNMASTER->passkey->fileno,"FTE AMOUNT",&fteamount);
					ADVupdateftes(posnum,posid1,-fteamount,PayjmstSubData,mtn);
				} /* end if(active==TRUE) */
			} /* end if(!ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,PayjmstSubData,mtn)) */
			fields2file(MTNMASTER->passkey->fileno,tmpfld);
			if(XPERT_SETUP->software_type==1)
			{
				FINDFLDSETCHAR(MTNMASTER->passkey->fileno,"JOB ACCOUNTS",TRUE);
			}
			WRTTRANS(MTNMASTER->passkey->fileno,0,NULL,mtn->previous);
			if(save_posper(mtn))
			{
				if(posid1!=NULL) Rfree(posid1); 
				prterr("Error:  Wasn't able to save to posper master!");
			}
			FINDFLDGETCHAR(MTNMASTER->passkey->fileno,"DELETEFLAG",&delflag);
			FINDFLDGETCHAR(MTNMASTER->passkey->fileno,"ACTIVE",&active);
			FINDFLDGETCHAR(MTNMASTER->passkey->fileno,"TERMINATE AT ENDING DATE",
				&terminate);
			FINDFLDGETCHAR(posnum,"MANAGED FTE",&managed_fte);
#ifdef LONGEVITY_WHEN_INACTIVE 
			if(active==FALSE && prv_active==TRUE)
			{
				y=FINDVIRTUAL(mtnrsrc,"LONGEVITY");
				if(y!=(-1))
				{
					v=mtnrsrc->virflds+y;
				}
				ZERNRD(msrnum);
				COPYFIELD(MTNMASTER->passkey->fileno,msrnum,"PERSONNEL IDENTIFICATION");
				if(!EQLNRD(msrnum,1))
				{
					prev=RDATDataNEW(msrnum);
				} else prev=NULL;
				FINDFLDGETDOUBLE(msrnum,"PAYJMST LONGEVITY",&district);
				district+=*v->value.float_value;
				FINDFLDSETDOUBLE(msrnum,"PAYJMST LONGEVITY",district);
				ADVWRTTRANSsec(msrnum,0,NULL,prev,PayjmstSubData,mtn);
				if(prev!=NULL) FreeRDATData(prev);
			}
#endif /* LONGEVITY_WHEN_INACTIVE */
			if(active==TRUE && delflag==FALSE && !terminate)
			{
				if(posid1!=NULL) Rfree(posid1);
				FINDFLDGETSTRING(MTNMASTER->passkey->fileno,"POSITION IDENTIFICATION",&posid1);
				FINDFLDGETDOUBLE(MTNMASTER->passkey->fileno,"FTE AMOUNT",&fteamount);
				ADVupdateftes(posnum,posid1,fteamount,PayjmstSubData,mtn);
			} /* end if(delflag==FALSE) */
			if(posid1!=NULL) Rfree(posid1); 
			DELETE_SUBORDINATES(mtnrsrc,MTNMASTER,PayjmstSubData,(void *)mtn);
			MTNrun_postsave_buttons(mtnrsrc,PSTRUCT,FALSE);
			updatebrowse(update_list,mbl_payjmst,MTNMASTER->passkey->fileno,MTNMASTER->module,mtnrsrc);
		}
		if(mbl_payjmst!=NULL) quit_record(mtnrsrc,mtn);
		else GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
		if(PSTRUCT!=NULL) Rfree(PSTRUCT);
	}
}
static void quit_record_test(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
        QUIT_RECORD_TEST(mtnrsrc,MTNMASTER->passkey->fileno,mtn->previous,readscreen,
                save_record_quit,quit_record,mtn);
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->grosslist!=NULL) freeapplib(mtn->grosslist);
		if(mtn->grlist!=NULL) freeapplib(mtn->grlist);
		mtn->r=NULL;
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		if(mtn->ps!=NULL) FreePaySelStruct(mtn->ps);
		Rfree(mtn);
	}
	if(WindowCount<2)
	{
		CLOSE_MASTER_FILES(MTNMASTER);
		if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
		if(fmgt!=NULL) free_finmgt(fmgt);
		if(mbl_posmstr!=NULL)
		{
			doexit_browse_position(mbl_posmstr);
			mbl_posmstr=NULL;
		}
		ShutdownSubsystems();
		exit(0);
	}
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}	
}
static void AUEQLfunction(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER,void (*SubFunc)(...),
	void *args,mtnstruct *mtn)
{
	updatefilerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
	setlist(mtnrsrc,mtn,TRUE);
	getsupportingrecords(mtnrsrc,mtn,TRUE);
}
static void NEfunction(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER,void (*SubFunc)(...),
	void *args,mtnstruct *mtn)
{
	getsupportingrecords(mtnrsrc,mtn,TRUE);
}
static void SETID(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readwidget(mtnrsrc,"PAY BALANCE OF CONTRACT");
	FINDRSCGETCHAR(mtnrsrc,"PAY BALANCE OF CONTRACT",&Pay_Balance_of_Contract);
	FINDRSCGETCHAR(mtnrsrc,"DUMP DEFERRED PAY BALANCE",&Dump_Deferred_Pay_Balance);
	ADV4SET_KEYFUNC(mtnrsrc,MTNMASTER,mtn->previous,PayjmstSubData,mtn,NEfunction,mtn,NULL,NULL,AUEQLfunction,mtn);
}
static void SETFILES(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int fyear=0,cyear=0;

        readallwidgets(mtnrsrc);
	FINDRSCGETCHAR(mtnrsrc,"PAY BALANCE OF CONTRACT",&Pay_Balance_of_Contract);
	FINDRSCGETCHAR(mtnrsrc,"DUMP DEFERRED PAY BALANCE",&Dump_Deferred_Pay_Balance);
	FINDRSCGETINT(mtnrsrc,"FISCAL YEAR",&fyear);
	FINDRSCGETINT(mtnrsrc,"CALENDAR YEAR",&cyear);
	FINDRSCSETINT(mtnrsrc,"[PAYJFSM][FISCAL YEAR]",fyear);
	FINDRSCSETINT(mtnrsrc,"[PAYJCSM][CALENDAR YEAR]",cyear);
        ReadRDAScrolledLists(mtnrsrc);
        rsrc2filerecord(MTNMASTER->passkey->fileno,mtnrsrc);
	getsupportingrecords(mtnrsrc,mtn,TRUE);
        GET_SUPPORTING(mtnrsrc,MTNMASTER,PayjmstSubData,mtn,TRUE,FALSE);
        ADVupdateSCRNvirtuals(mtnrsrc,PayjmstSubData,mtn);
}
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MTNMASTER->passkey->fileno);
	readallwidgets(mtnrsrc);
	FINDRSCGETCHAR(mtnrsrc,"PAY BALANCE OF CONTRACT",&Pay_Balance_of_Contract);
	FINDRSCGETCHAR(mtnrsrc,"DUMP DEFERRED PAY BALANCE",&Dump_Deferred_Pay_Balance);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MTNMASTER->passkey->fileno,mtnrsrc);
	getsupportingrecords(mtnrsrc,mtn,TRUE);
}
static void UpdateVirtuals(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *grossid=NULL;

	readwidget(mtnrsrc,"PAY BALANCE OF CONTRACT");
	FINDRSCGETCHAR(mtnrsrc,"PAY BALANCE OF CONTRACT",&Pay_Balance_of_Contract);
	readwidget(mtnrsrc,"DUMP DEFERRED PAY BALANCE");
	FINDRSCGETCHAR(mtnrsrc,"DUMP DEFERRED PAY BALANCE",&Dump_Deferred_Pay_Balance);
	readfilewidgets(MTNMASTER->passkey->fileno,mtnrsrc);
	rsrc2filerecord(MTNMASTER->passkey->fileno,mtnrsrc);
	ADVupdateSCRNvirtuals(mtnrsrc,PayjmstSubData,mtn);
	FINDRSCGETSTRING(mtnrsrc,"[PAYJMST][GROSS IDENTIFICATION]",&grossid);
	UpdateGROSSAmounts(mtnrsrc,mtn,grossid,TRUE);
}
static void PAYJMSTExecuteBrowseButtonFunc(RDArsrc *brsrc,MaintainButton *button,void *targetkey,MaintainMaster *MTNMASTER)
{
	APPlib *envpx=NULL,*newargs=NULL,*envpx1=NULL;
	int x;
	char *value=NULL;
	MakeBrowseList *blist=NULL;

	if(button!=NULL)
	{
		if(MTNMASTER!=NULL)
		{
			if(brsrc!=NULL)
			{
				blist=MTNMASTER->mbl_utlmstr;
				if(blist!=NULL)
				{
					if(blist->definelist!=NULL)
					{
						if(EQLNRDKEYsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,targetkey)) 
						{
							ZERNRD(MTNMASTER->passkey->fileno);
							if(diagmix || diagmix_field)
							{
								prterr("Error in ExecuteListFunc targetkey not found at line [%d] program [%s].",__LINE__,__FILE__);
							}
						}
						if(!MTNMASTER->file_type)
						{
							rsrc2filerecord(MTNMASTER->passkey->fileno,blist->definelist);
						} else {
							rsrc2singlefilerecord(MTNMASTER->passkey->fileno,blist->definelist);
						}
						CLEAR_SCREEN_VIRTUAL(blist->definelist);
						SetRDArsrcFiles(blist->fileno,blist->definelist);
						envpx=MaintainButton2PassKey(button,MTNMASTER->passkey->fileno);
						newargs=APPlibNEW();
						envpx1=MaintainButton2ClearPassKey(button,MTNMASTER->passkey->fileno);
						for(x=0;x<button->args->numlibs;++x)
						{
							addAPPlib(newargs,button->args->libs[x]);
						}
						if(button->evalargs==TRUE)
						{
							for(x=0;x<newargs->numlibs;++x)
							{
								if(!isEMPTY(newargs->libs[x]))
								{
									value=EVALUATEstr(newargs->libs[x],SCRNvirtualSubData,blist->definelist);
									if(value==NULL) value=stralloc("");
									if(newargs->libs[x]!=NULL) Rfree(newargs->libs[x]);
									newargs->libs[x]=stralloc(value);
									if(value!=NULL) Rfree(value);
								} else {
									if(newargs->libs[x]!=NULL) Rfree(newargs->libs[x]);
									newargs->libs[x]=stralloc("");
								}
							}
						}
						if(!RDAstrcmp(button->progname,"RUNREPORT"))
						{
							if(envpx!=NULL)
							{
								for(x=0;x<envpx->numlibs;++x)
								{
									RDA_PutEnv(envpx->libs[x]);
								}
							}
							RUNREPORTADV2(newargs->libs[0],newargs->libs[1],NULL,NULL,TRUE,2,NULL,button->func,brsrc,(void *)MTNMASTER,(MTNMASTER->level+1));
						} else if(!RDAstrcmp(button->progname,"RUNPOWERADD"))
						{
							if(envpx!=NULL)
							{
								for(x=0;x<envpx->numlibs;++x)
								{
									RDA_PutEnv(envpx->libs[x]);
								}
							}
							RUNPOWERADDADV(newargs->libs[0],newargs->libs[1],(MTNMASTER->level+1),button->func,brsrc,(void *)MTNMASTER,TRUE);
						} else if(!RDAstrcmp(button->progname,"RUNMAINTAIN"))
						{
							if(envpx!=NULL)
							{
								for(x=0;x<envpx->numlibs;++x)
								{
									RDA_PutEnv(envpx->libs[x]);
								}
							}
							RUNMAINTAINADV(newargs->libs[0],newargs->libs[1],(MTNMASTER->level+1),button->func,brsrc,(void *)MTNMASTER,TRUE);
						} else {
							if(button->type==0)
							{
								ADVExecuteProgram(button->progname,newargs,envpx);
							} else {
								ADVExecute2Program(button->progname,newargs,envpx);
							}
						}
						if(envpx1!=NULL)
						{
							if(!RDAstrcmp(button->progname,"RUNPOWERADD") || !RDAstrcmp(button->progname,"RUNMAINTAIN") || !RDAstrcmp(button->progname,"RUNREPORT"))
							{
								for(x=0;x<envpx1->numlibs;++x)
								{
									RDA_PutEnv(envpx1->libs[x]);
								}
							}
							freeapplib(envpx1);
						}
						if(newargs!=NULL) freeapplib(newargs);
					}
				}
			} else {
			}
		} else { 
			prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	}
}
static void okBrowseButtonFunc(RDArsrc *brsrc,MaintainButton *button)
{
	MakeBrowseList *blist=NULL;
	MaintainMaster *MTNMASTER=NULL;
	RDAbrowse *list=NULL;
	char **key=NULL;
	int sellist=0;

	if(brsrc!=NULL)
	{
		if(button!=NULL)
		{
			MTNMASTER=button->parent;
			if(MTNMASTER!=NULL)
			{
				blist=MTNMASTER->mbl_utlmstr;
				if(blist!=NULL) 
				{
					list=blist->list;
					key=(char **)list->key;
					FINDRSCGETINT(blist->mainrsrc,"BROWSE LIST",&sellist);
					if(sellist>=list->num)
					{
						prterr("Error: Selected Browse Element [%d] isn't in actual Browse List.  Resetting list to first element in Browse List, element number ZERO (0).",sellist);
					} else { 
						list->keyvalue=key[sellist];
						PAYJMSTExecuteBrowseButtonFunc(brsrc,button,list->keyvalue,MTNMASTER);
					}
				}
			}
		}
	}
}
static void PAYJMSTMakePowerMasterBrowseButtons(RDArsrc *brsrc,MaintainMaster *MTNMASTER)
{
	int x;
	MaintainButton *button=NULL;

	if(MTNMASTER!=NULL)
	{
		if(MTNMASTER->buttons!=NULL)
		{
			for(x=0,button=MTNMASTER->buttons;x<MTNMASTER->numbuttons;++x,++button)
			{
				button->parent=MTNMASTER;
				if(!RDAstrcmp(button->progname,"RUNREPORT"))
				{
					button->func=NULL;
				} else  if(!RDAstrcmp(button->progname,"RUNPOWERADD"))
				{
					button->func=NULL;
				} else  if(!RDAstrcmp(button->progname,"RUNMAINTAIN"))
				{
					button->func=NULL;
				}
				addbtnrsrc(brsrc,button->buttonname,TRUE,
					okBrowseButtonFunc,button);
			}
		}
	} else {
		prterr("Error MaintainMaster structure is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
}
short PayjmstSelectFunc(MakeBrowseList *m)
{
	ZERNRD(msrnum);
	COPYFIELD(m->fileno,msrnum,"PERSONNEL IDENTIFICATION");
	if(EQLNRD(msrnum,1)) KEYNRD(msrnum,1);
	return(TRUE);
}
void browse_payjmst(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtn!=NULL)
	{
		if(mtn->grosslist!=NULL) freeapplib(mtn->grosslist);
		if(mtn->grlist!=NULL) freeapplib(mtn->grlist);
		mtn->r=NULL;
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		if(mtn->ps!=NULL) FreePaySelStruct(mtn->ps);
		Rfree(mtn);
	}
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	mainrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->browse_screen);
	searchrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->search_browse);
	definelist=RDArsrcNEW(MTNMASTER->module,MTNMASTER->define_list);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->grosslist=NULL;
	mtn->grlist=NULL;
	mtn->previous=NULL;
	mtn->previous=RDATDataNEW(MTNMASTER->passkey->fileno);
	mtn->ps=PaySelStructNew(NULL,0,NULL,NULL,NULL);
	mtn->r=searchrsrc;
	ZERNRD(MTNMASTER->passkey->fileno);
	ApplyPassKey(MTNMASTER->passkey);
	GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,MTNMASTER,PayjmstSubData,(void *)mtn);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	mainrsrc->scn=RDAscrnNEW(mainrsrc->module,mainrsrc->screen);
	findscrnbin(mainrsrc->scn);
	MakeBrowseListTotals(mainrsrc,MTNMASTER->passkey->fileno,definelist);
	MakeRunFunction(mainrsrc,MTNMASTER);
	PAYJMSTMakePowerMasterBrowseButtons(mainrsrc,MTNMASTER);
/*
	mbl_payjmst=ADVPbrowse(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,mainrsrc,searchrsrc,definelist,doexit,okfunc,(editable ? new_record:NULL),NULL,FALSE,NULL,0,0,2,0,MTNMASTER->passkey);
*/
	mbl_payjmst=xbrowse(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,mainrsrc,searchrsrc,definelist,PASSKEYinitfunc,PASSKEYselectfunction,PayjmstSelectFunc,doexit,okfunc,(editable ? new_record:NULL),NULL,FALSE,NULL,0,0,2,0,MTNMASTER->passkey,0,1,__LINE__,__FILE__);
	MTNMASTER->mbl_utlmstr=mbl_payjmst;
	if(mtn!=NULL)
	{
		if(mtn->grosslist!=NULL) freeapplib(mtn->grosslist);
		if(mtn->grlist!=NULL) freeapplib(mtn->grlist);
		mtn->r=NULL;
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		if(mtn->ps!=NULL) FreePaySelStruct(mtn->ps);
		Rfree(mtn);
	}
}
void search_payjmst(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtn!=NULL)
	{
		if(mtn->grosslist!=NULL) freeapplib(mtn->grosslist);
		if(mtn->grlist!=NULL) freeapplib(mtn->grlist);
		mtn->r=NULL;
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		if(mtn->ps!=NULL) FreePaySelStruct(mtn->ps);
		Rfree(mtn);
	}
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	mainrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->browse_screen);
	searchrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->search_browse);
	definelist=RDArsrcNEW(MTNMASTER->module,MTNMASTER->define_list);
	ZERNRD(MTNMASTER->passkey->fileno);
	ApplyPassKey(MTNMASTER->passkey);
	GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,MTNMASTER,SCRNvirtualSubData,(void *)mainrsrc);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	mainrsrc->scn=RDAscrnNEW(mainrsrc->module,mainrsrc->screen);
	findscrnbin(mainrsrc->scn);
	MakeBrowseListTotals(mainrsrc,MTNMASTER->passkey->fileno,definelist);
	MakeRunFunction(mainrsrc,MTNMASTER);
	PAYJMSTMakePowerMasterBrowseButtons(mainrsrc,MTNMASTER);
	mbl_payjmst=START_ADV2Pbrowse(MTNMASTER->passkey->fileno,
			MTNMASTER->passkey->keyno,mainrsrc,searchrsrc,definelist,
			doexit,okfunc,new_record,
			NULL,(MTNMASTER->level==0?FALSE:TRUE),NULL,0,0,2,0,
			MTNMASTER->passkey,MTNMASTER->start_where,2);
	MTNMASTER->mbl_utlmstr=mbl_payjmst;
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	short filenum=(-1);
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;
	RDAsearch *sup;
	int x;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,"PAYROLL","MTN PAYJMST")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	ComputeVirtualType=FALSE;
	fmgt=RDAfinmgtNEW();
	getfinmgtbin(fmgt);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("PAYROLL")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"PAYROLL");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"PAYROLL");
#endif

	gsv=RDAGenericSetupNew("PAYROLL","PAYJMST MANAGE TOTAL FTE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			manage_fte=*gsv->value.string_value;
		} else {
			manage_fte=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("PAYROLL","USE PAYJMST SUPPLEMENTS");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			use_supp=*gsv->value.string_value;
		} else {
			use_supp=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("PAYROLL","LOCATION PLACEMENT");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			loc_place=*gsv->value.short_value;
		} else {
			loc_place=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("POSTRK")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"POSTRK");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"POSTRK");
#endif

	gsv=RDAGenericSetupNew("POSTRK","USE POSITION TYPE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			use_postyp=*gsv->value.string_value;
		} else {
			use_postyp=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	MTNMASTER=MaintainMasterNew("PAYROLL","MTN PAYJMST");
	if(findMaintainMaster(MTNMASTER))
	{
		ERRORDIALOG("MAINTAIN MTNMASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE);
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN PAYJMST",MTNMASTER->module);
		doexit(NULL);
	} else {
		if((filenum=APPOPNNRDEDITABLE(MTNMASTER->module,"PAYJMST",&editable))==(-1))
		{
			doexit(NULL);
			return;
		}
		if((posnum=APPOPNNRDEDITABLE("POSTRK","POSMSTR",&editable))==(-1))
		{
			editable=FALSE;
		}
		if(posnum!=(-1)) CLSNRD(posnum);
		posnum=(-1);
		SetSupportingWritable(MTNMASTER,"POSTRK","POSPER",1,(editable ? TRUE:FALSE),&editable);
		SetSupportingWritable(MTNMASTER,"POSTRK","POSMSTR",1,(editable ? TRUE:FALSE),&editable);
		if(MTNMASTER!=NULL && editable==FALSE)
		{
			for(x=0,sup=MTNMASTER->s;x<MTNMASTER->num;++x,++sup)
			{
				sup->writable=FALSE;
			}
		}
		if(!OPEN_MASTER_FILES(MTNMASTER))
		{
			doexit(NULL);
			return;
		}
		msrnum=APPReturnSupportingFileno(MTNMASTER,"PRSNNL","PERDMG",1);
		grsnum=APPReturnSupportingFileno(MTNMASTER,"POSTRK","POSGRS",1);
		grsnum=APPReturnSupportingFileno(MTNMASTER,"POSTRK","POSGRS",1);
		posnum=APPReturnSupportingFileno(MTNMASTER,"POSTRK","POSMSTR",1);
		eeojfnum=APPReturnSupportingFileno(MTNMASTER,"POSTRK","POSEEOJF",1);
		eeojgnum=APPReturnSupportingFileno(MTNMASTER,"POSTRK","POSEEOJG",1);
		/*
		if(loc_place==TRUE)
		{
		*/
			poslnum=APPReturnSupportingFileno(MTNMASTER,"POSTRK","POSLMST",1);
		/*
		}
		*/
		if(use_supp==TRUE)
		{
			possup=APPReturnSupportingFileno(MTNMASTER,"POSTRK","POSSUP",1);
		}
		if(use_postyp==TRUE)
		{
			postyp=APPReturnSupportingFileno(MTNMASTER,"POSTRK","POSTYP",1);
		}
		grsrate=APPReturnSupportingFileno(MTNMASTER,"POSTRK","POSGRAT",1);
		ratenum=APPReturnSupportingFileno(MTNMASTER,"POSTRK","POSRATE",1);
		pospernum=APPReturnSupportingFileno(MTNMASTER,"POSTRK","POSPER",1);
		payjfsm=APPReturnSupportingFileno(MTNMASTER,"PAYROLL","PAYJFSM",1);
		payjcsm=APPReturnSupportingFileno(MTNMASTER,"PAYROLL","PAYJCSM",1);
		payjrfs=APPReturnSupportingFileno(MTNMASTER,"PAYROLL","PAYJRFS",1);
		payjrcs=APPReturnSupportingFileno(MTNMASTER,"PAYROLL","PAYJRCS",1);
		paygrp=APPReturnSupportingFileno(MTNMASTER,"PAYROLL","PAYGRP",1);
		MTNMASTER->passkey=READPassKey(filenum);
		MTNMASTER->inheritrsrcs=TRUE;
		if(argc>1)
		{
			if(!RDAstrcmp(argv[1],"0"))
			{
				payjmstm(1,NULL);
			} else if(!RDAstrcmp(argv[1],"1"))
			{
				browse_payjmst(NULL,NULL);
			} else if(!RDAstrcmp(argv[1],"2"))
			{
				search_payjmst(NULL,NULL);
			} else {
				doexit(NULL);
				return;
			}
		} else {
			switch(MTNMASTER->start_with)
			{
				default:
				case 0:
					payjmstm(1,NULL);
					break;
				case 1:
					browse_payjmst(NULL,NULL);
					break;
			}
		}
	}
	RDAAPPMAINLOOP();
}
void doexit(MakeBrowseList *blist)
{
	if(blist!=NULL) 
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
	if(WindowCount<2)
	{
		CLOSE_MASTER_FILES(MTNMASTER);
		if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
		if(fmgt!=NULL) free_finmgt(fmgt);
		if(mbl_posmstr!=NULL)
		{
			doexit_browse_position(mbl_posmstr);
			mbl_posmstr=NULL;
		}
		ShutdownSubsystems();
		exit(0);
	}
	if(mbl_payjmst!=NULL) mbl_payjmst=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func)
		{
			payjmstm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MTNMASTER);
		}
	} else {
		payjmstm(0,targetkey);
	}
}
static void new_record(RDArsrc *mtnrsrc,MakeBrowseList *blist)
{
	payjmstm(2,NULL);
} 
