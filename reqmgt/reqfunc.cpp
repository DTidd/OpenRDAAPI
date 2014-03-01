/* reqfunc.c - Requisition Functions */
#include <reqmgt.hpp>

short xUpdateCatalogue(short reqctlg,short reqcven,char *categoryid,
	char *itemid,char *itemname,char *itemdesc,char *validtodate,
	char *venid,void (*SubFunc)(...),void *args,int line,char *file)
{
	short return_value=0;

	if(diagmix)
	{
		prterr("DIAG UpdateCatalogue at line [%d] program [%s].",line,file);
	}
	if(reqctlg!=(-1))
	{
		ZERNRD(reqctlg);
		FINDFLDSETSTRING(reqctlg,"CATEGORY IDENTIFICATION",categoryid);
		FINDFLDSETSTRING(reqctlg,"ITEM IDENTIFICATION",itemid);
		LOCNRDFILE(reqctlg);
		if(SubFunc!=NULL)
		{
			if(ADVEQLNRDsec(reqctlg,1,SubFunc,args))
			{
				FINDFLDSETSTRING(reqctlg,"ITEM NAME",itemname);
				FINDFLDSETSTRING(reqctlg,"DESCRIPTION",itemdesc);
				FINDFLDSETCHAR(reqctlg,"ACTIVE",FALSE);
				FINDFLDSETSTRING(reqctlg,"SOURCE USER",USERLOGIN);
				ADVWRTTRANSsec(reqctlg,0,NULL,NULL,SubFunc,args);
				return_value=(0);
			} else {
				return_value=(-1);
			}
		} else {
			if(EQLNRDsec(reqctlg,1))
			{
				FINDFLDSETSTRING(reqctlg,"ITEM NAME",itemname);
				FINDFLDSETSTRING(reqctlg,"DESCRIPTION",itemdesc);
				FINDFLDSETCHAR(reqctlg,"ACTIVE",FALSE);
				FINDFLDSETSTRING(reqctlg,"SOURCE USER",USERLOGIN);
				WRTTRANSsec(reqctlg,0,NULL,NULL);
				return_value=(0);
			} else {
				return_value=(-1);
			}
		}
		UNLNRDFILE(reqctlg);
	}
	if(reqcven!=(-1))
	{
		ZERNRD(reqcven);
		FINDFLDSETSTRING(reqcven,"CATEGORY IDENTIFICATION",categoryid);
		FINDFLDSETSTRING(reqcven,"ITEM IDENTIFICATION",itemid);
		FINDFLDSETSTRING(reqcven,"VENDOR IDENTIFICATION",venid);
		LOCNRDFILE(reqcven);
		if(SubFunc!=NULL)
		{
			if(ADVEQLNRDsec(reqcven,1,SubFunc,args))
			{
				FINDFLDSETSTRING(reqcven,"VALID TO DATE",validtodate);
				FINDFLDSETCHAR(reqcven,"ACTIVE",FALSE);
				ADVWRTTRANSsec(reqcven,0,NULL,NULL,SubFunc,args);
				return_value=(0);
			}
		} else {
			if(EQLNRDsec(reqcven,1))
			{
				FINDFLDSETSTRING(reqcven,"VALID TO DATE",validtodate);
				FINDFLDSETCHAR(reqcven,"ACTIVE",FALSE);
				WRTTRANSsec(reqcven,0,NULL,NULL);
				return_value=(0);
			}
		}
		UNLNRDFILE(reqcven);
	}
	return(return_value);
}
void xSeeReqAcctStruct(ReqAcctStruct *a,int line,char *file)
{
	if(a!=NULL)
	{
		prterr("DIAG SeeReqAcctStruct at line [%d] program [%s].",line,file);
		prterr("DIAG SeeReqAcctStruct Acc No [%d] Def [%d] Account [%s] Debit [%s] Credit [%s] Prorate [%s] Percentage [%f] Amount [%.0f] Encumber [%.0f] Default Debit [%s] Default Credit [%s] Default Payable [%s] Default Bank Identification [%s]  Clearing Def [%d] Clearing Cash [%s] Clearing Due From [%s].\n",
			a->accno,a->deftype,
			(a->account!=NULL ? a->account:""),
			(a->debit!=NULL ? a->debit:""),
			(a->credit!=NULL ? a->credit:""),
			(a->prorate ? "True":"False"),
			a->percentage,a->amount,a->enc,
			(a->defdebit!=NULL?a->defdebit:""),
			(a->defcredit!=NULL?a->defcredit:""),
			(a->defpayable!=NULL?a->defpayable:""),
			(a->defbankid!=NULL?a->defbankid:""),
			(a->cdeftype),
			(a->defccash!=NULL?a->defccash:""),
			(a->defcdfrom!=NULL?a->defcdfrom:""));
	} else {
		prterr("DIAG SeeReqAcctStruct is NULL at line [%d] program [%s].",line,file);
	}
}
void xSeeREQACC(REQACC *p,int line,char *file)
{
	ReqAcctStruct *a;
	int x;

	if(p!=NULL)
	{
		prterr("DIAG SeeREQACC at line [%d] program [%s].",line,file);
		if(p->a!=NULL && p->num>0)
		{
			prterr("REQACC Has [%d] ReqAcctStructs.\n",p->num);
			for(x=0,a=p->a;x<p->num;++x,++a) 
			{
				prterr("Account Struct #%d:\n",x);
				SeeReqAcctStruct(a);
			}
		} else {
			prterr("DIAG SeeREQACC ReqAcctStruct is NULL at line [%d] program [%s].",line,file);
		}
	} else {
		prterr("DIAG SeeREQACC is NULL at line [%d] program [%s].",line,file);
	}
}
REQACC *xREQACCNEW(int line,char *file)
{
	REQACC *tmp=NULL;

	if(diagmix)
	{
		prterr("DIAG REQACCNEW at line [%d] program [%s].",line,file);
	}
	tmp=Rmalloc(sizeof(REQACC));
	tmp->num=0;
	tmp->a=NULL;
	return(tmp);
}
void xAddReqAcctStruct(REQACC *p,int accno,short dtype,char *acc,
	char *deb,char *cred,char prorate,double percentage,double amount,
	char *defdeb,char *defcred,char *defpay,char *defbank,
	short cdtype,char *defclearcash,char *defcleardfrom,
	int line,char *file)
{
	ReqAcctStruct *a;

	if(diagmix)
	{
		prterr("DIAG AddReqAcctStruct at line [%d] program [%s].",line,file);
	}
	if(p->a!=NULL)
	{
		p->a=Rrealloc(p->a,(p->num+1)*sizeof(ReqAcctStruct));
	} else {
		p->a=Rmalloc(sizeof(ReqAcctStruct));
		p->num=0;
	}
	a=p->a+p->num;
	a->accno=accno;
	a->deftype=dtype;
	if(!isEMPTY(acc)) a->account=stralloc(acc);
		else a->account=NULL;
	if(!isEMPTY(deb)) a->debit=stralloc(deb);
		else a->debit=NULL;
	if(!isEMPTY(cred)) a->credit=stralloc(cred);
		else a->credit=NULL;
	a->prorate=prorate;
	a->percentage=percentage;
	a->amount=amount;
	a->enc=0;
	a->pre_enc=0;
	if(!isEMPTY(defdeb)) a->defdebit=stralloc(defdeb);
		else a->defdebit=NULL;
	if(!isEMPTY(defcred)) a->defcredit=stralloc(defcred);
		else a->defcredit=NULL;
	if(!isEMPTY(defpay)) a->defpayable=stralloc(defpay);
		else a->defpayable=NULL;
	if(!isEMPTY(defbank)) a->defbankid=stralloc(defbank);
		else a->defbankid=NULL;
	a->cdeftype=cdtype;
	if(!isEMPTY(defclearcash)) a->defccash=stralloc(defclearcash);
		else a->defccash=NULL;
	if(!isEMPTY(defcleardfrom)) a->defcdfrom=stralloc(defcleardfrom);
		else a->defcdfrom=NULL;
	if(diagmix)
	{
		SeeReqAcctStruct(a);TRACE;
	}
	++p->num;
}
void xFreeREQACC(REQACC *p,int line,char *file)
{
	int x;
	ReqAcctStruct *a;

	if(diagmix)
	{
		prterr("DIAG FreeREQACC at line [%d] program [%s].",line,file);
	}
	if(p!=NULL)
	{
		if(p->a!=NULL)
		{
			for(x=0,a=p->a;x<p->num;++x,++a)
			{
				if(a->account!=NULL) Rfree(a->account);
				if(a->debit!=NULL) Rfree(a->debit);
				if(a->credit!=NULL) Rfree(a->credit);
				if(a->defdebit!=NULL) Rfree(a->defdebit);
				if(a->defcredit!=NULL) Rfree(a->defcredit);
				if(a->defpayable!=NULL) Rfree(a->defpayable);
				if(a->defbankid!=NULL) Rfree(a->defbankid);
				if(a->defccash!=NULL) Rfree(a->defccash);
				if(a->defcdfrom!=NULL) Rfree(a->defcdfrom);
			}
			Rfree(p->a);
			p->a=NULL;
		}
		Rfree(p);
		p=NULL;
	}
}
REQACC *xGetREQACC(short reqacc,char *locid,char *reqid,int itemno,
	void (*SubFunc)(...),void *args,int line,char *file)
{
	short ef=FALSE,deftype=0,dcdeftype=0;
	REQACC *mts=NULL;
	char *llocid=NULL,*lreqid=NULL,*acctcode=NULL,prorate=FALSE;
	char deleteflag=FALSE;
	char *debit=NULL,*credit=NULL;
	char *defdebit=NULL,*defcredit=NULL,*defpayable=NULL,*defbankid=NULL;
	char *ccash=NULL,*cdfrom=NULL;
	int litemno=0,accno=0;
	double perc=0.0,amt=0.0;

	if(diagmix)
	{
		prterr("DIAG GetREQACC at line [%d] program [%s].",line,file);
	}
	mts=REQACCNEW();
	ZERNRD(reqacc);
	FINDFLDSETSTRING(reqacc,"LOCATION IDENTIFICATION",locid);
	FINDFLDSETSTRING(reqacc,"REQUISITION IDENTIFICATION",reqid);
	FINDFLDSETINT(reqacc,"ITEM NUMBER",itemno);
	if(SubFunc!=NULL) ef=ADVGTENRDsec(reqacc,1,SubFunc,args);
	else ef=GTENRDsec(reqacc,1);
	while(!ef)
	{
		FINDFLDGETSTRING(reqacc,"LOCATION IDENTIFICATION",&llocid);
		FINDFLDGETSTRING(reqacc,"REQUISITION IDENTIFICATION",&lreqid);
		FINDFLDGETINT(reqacc,"ITEM NUMBER",&litemno);
		if(RDAstrcmp(locid,llocid) || RDAstrcmp(reqid,lreqid) ||
			litemno!=itemno) break;
		FINDFLDGETCHAR(reqacc,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDGETINT(reqacc,"ACCOUNT NUMBER",&accno);
			FINDFLDGETSHORT(reqacc,"DEFINITION TYPE",&deftype);
			FINDFLDGETSTRING(reqacc,"ACCOUNT CODE",&acctcode);
			FINDFLDGETSTRING(reqacc,"DEBIT CODE",&debit);
			FINDFLDGETSTRING(reqacc,"CREDIT CODE",&credit);
			FINDFLDGETCHAR(reqacc,"PRORATE",&prorate);
			FINDFLDGETDOUBLE(reqacc,"PERCENTAGE",&perc);
			FINDFLDGETDOUBLE(reqacc,"AMOUNT",&amt);
			FINDFLDGETSTRING(reqacc,"DEFAULT DEBIT CODE",&defdebit);
			FINDFLDGETSTRING(reqacc,"DEFAULT CREDIT CODE",&defcredit);
			FINDFLDGETSTRING(reqacc,"DEFAULT PAYABLE CODE",&defpayable);
			FINDFLDGETSTRING(reqacc,"DEFAULT BANK IDENTIFICATION",&defbankid);

			FINDFLDGETSHORT(reqacc,"DEFAULT CLEARING DEFINITION TYPE",&dcdeftype);
			FINDFLDGETSTRING(reqacc,"DEFAULT CLEARING CASH",&ccash);
			FINDFLDGETSTRING(reqacc,"DEFAULT CLEARING DUE FROM",&cdfrom);
			AddReqAcctStruct(mts,accno,deftype,acctcode,debit,credit,prorate,perc,amt,defdebit,defcredit,defpayable,defbankid,dcdeftype,ccash,cdfrom);
		}
		if(SubFunc!=NULL) ef=ADVNXTNRDsec(reqacc,1,SubFunc,args);
		else ef=NXTNRDsec(reqacc,1);
	}
	if(acctcode!=NULL) Rfree(acctcode);
	if(debit!=NULL) Rfree(debit);
	if(credit!=NULL) Rfree(credit);
	if(defdebit!=NULL) Rfree(defdebit);
	if(defcredit!=NULL) Rfree(defcredit);
	if(defpayable!=NULL) Rfree(defpayable);
	if(defbankid!=NULL) Rfree(defbankid);
	if(llocid!=NULL) Rfree(llocid);
	if(lreqid!=NULL) Rfree(lreqid);
	if(mts->num<1) 
	{
		FreeREQACC(mts);
		mts=NULL;
	}
	if(diagmix)
	{
		SeeREQACC(mts);TRACE;
	}
	return(mts);
}
short xCHECKREQACC(REQACC *r,short eyrnum,int fiscalyear,short pmonth,
	RDAfinmgt *finsetup,short type,void (*SubFunc)(...),void *args,
	int line,char *file)
{
	ReqAcctStruct *a;
	int x;
	short testapp=FALSE;

	if(r!=NULL)
	{
		for(x=0,a=r->a;x<r->num;++x,++a)
		{
			ZERNRD(eyrnum);
			FINDFLDSETINT(eyrnum,"FISCAL YEAR",fiscalyear);
			FINDFLDSETSHORT(eyrnum,"DEFINITION TYPE",a->deftype);
 			FINDFLDSETSTRING(eyrnum,"ACCOUNT CODE",a->account);
			if((SubFunc!=NULL ? !ADVEQLNRDsec(eyrnum,1,SubFunc,args)
				: !EQLNRDsec(eyrnum,1)))
			{
				if(!type)
				{
					if(finsetup->budget==ANNUALBUDGET) 
					{ 
						testapp=testapprotot(eyrnum,a->enc,NULL);
					} else {
						testapp=testapprotot(eyrnum,a->enc,
							CALENDAR_MONTHS[pmonth]);
					}
				} else {
					if(finsetup->budget==ANNUALBUDGET) 
					{ 
						testapp=testapprotot(eyrnum,(a->enc-a->pre_enc),
							NULL);
					} else {
						testapp=testapprotot(eyrnum,(a->enc-a->pre_enc),
							CALENDAR_MONTHS[pmonth]);
					}
				}
				if(testapp==FALSE) return(testapp);
			}
		}
		return(TRUE);
	} else {
		prterr("Error no Accounts Available at line [%d] program [%s].",
			line,file);
		return(FALSE);
	}
}
void xREQACC_UPDATE_UNDIST(REQACC *r,int fiscalyear,short pmonth,
	short eyrnum,short byrnum,short type,short which,
	void (*SubFunc)(...),void *args,int line,char *file)
{
	ReqAcctStruct *a;
	int x;

	if(diagmix)
	{
		prterr("DIAG REQACC_UPDATE_UNDIST at line [%d] program [%s].",
			line,file);
	}
	if(r!=NULL)
	{
		for(x=0,a=r->a;x<r->num;++x,++a)
		{
			if(!which)
			{
				if(type!=(-1))
				{
					if(SubFunc!=NULL)
					{
						ADVUpdateUnDistFinMgtYear(a->deftype,a->account,a->debit,a->credit,NULL,eyrnum,byrnum,fiscalyear,"ENCUMBRANCES",CALENDAR_MONTHS[pmonth],a->enc,SubFunc,args);
					} else {
						UpdateUnDistFinMgtYear(a->deftype,a->account,a->debit,a->credit,NULL,eyrnum,byrnum,fiscalyear,"ENCUMBRANCES",CALENDAR_MONTHS[pmonth],a->enc);
					}
				} else {
					if(SubFunc!=NULL)
					{
						ADVUpdateUnDistFinMgtYear(a->deftype,a->account,a->debit,a->credit,NULL,eyrnum,byrnum,fiscalyear,"ENCUMBRANCES",CALENDAR_MONTHS[pmonth],-a->enc,SubFunc,args);
					} else {
						UpdateUnDistFinMgtYear(a->deftype,a->account,a->debit,a->credit,NULL,eyrnum,byrnum,fiscalyear,"ENCUMBRANCES",CALENDAR_MONTHS[pmonth],-a->enc);
					}
				}
			} else {
				if(type!=(-1))
				{
					if(SubFunc!=NULL)
					{
						ADVUpdateUnDistFinMgtYear(a->deftype,a->account,a->debit,a->credit,NULL,eyrnum,byrnum,fiscalyear,"ENCUMBRANCES",CALENDAR_MONTHS[pmonth],(a->enc-a->pre_enc),SubFunc,args);
					} else {
						UpdateUnDistFinMgtYear(a->deftype,a->account,a->debit,a->credit,NULL,eyrnum,byrnum,fiscalyear,"ENCUMBRANCES",CALENDAR_MONTHS[pmonth],(a->enc-a->pre_enc));
					}
				} else {
					if(SubFunc!=NULL)
					{
						ADVUpdateUnDistFinMgtYear(a->deftype,a->account,a->debit,a->credit,NULL,eyrnum,byrnum,fiscalyear,"ENCUMBRANCES",CALENDAR_MONTHS[pmonth],-(a->enc-a->pre_enc),SubFunc,args);
					} else {
						UpdateUnDistFinMgtYear(a->deftype,a->account,a->debit,a->credit,NULL,eyrnum,byrnum,fiscalyear,"ENCUMBRANCES",CALENDAR_MONTHS[pmonth],-(a->enc-a->pre_enc));
					}
				}
			}
		}
	}
}
void xComputeREQACC(REQACC *r,double amount,int line,char *file)
{
	double t=0.0;
	int x;
	ReqAcctStruct *a;

	if(diagmix)
	{
		prterr("DIAG ComputeREQACC at line [%d] program [%s].",line,file);
	}
	if(r!=NULL)
	{
		for(x=0,a=r->a;x<r->num;++x,++a) a->enc=0;
		for(x=0,a=r->a;x<r->num;++x,++a)
		{
			if((amount-t)<=0) break;
			if((x+1)<r->num)
			{
				if(a->prorate)
				{
					a->enc=round((amount*a->percentage/100));
					if((amount-t)<a->enc) a->enc=(amount-t);
					if(a->enc<=0)
					{
						a->enc=0;
						break;	
					} 
				} else {
					if((amount-t)<a->amount) a->enc=(amount-t);
					else a->enc=a->amount;
					if(a->enc<=0)
					{
						a->enc=0;
						break;	
					} 
				}
			} else {
				if((amount-t)>0) a->enc=(amount-t);
			}
			t+=a->enc;
		}
	} else {
		prterr("Error no Accounts Available at line [%d] program [%s].",
			line,file);
	}
}
short xgetAddReqDefAcct(REQACC *r,int line,char *file)
{
	short version=FALSE,x,test=FALSE;
	BIN *bin;
	char *libname=NULL;
	int total=0,y;
	ReqAcctStruct *a;

	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(USERLOGIN)+13);
	sprintf(libname,"%s/%s/REQMGT.STP",CURRENTDIRECTORY,USERLOGIN);	
	if(diagmix)
	{
		prterr("DIAG getReqDefAcct Reading Add Requisitions Default Accounts from [%s] at line [%d] program [%s].",libname,line,file);
	}
	bin=getlibbin(libname,"ADD REQUISITIONS MULTIPLE ACCOUNTS",TRUE);
	if(bin==NULL) 
	{
		prterr("Error Requisition Setup Variables were not found in Library [%s] or the Library doesn't exist at line [%d] program [%s].",libname,line,file);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	x=BINgetshort(bin);
	if(x!=(-1700) && x!=(-1701))

	{
		prterr("Error Invalid version or bad binary in Library [%s] cannot read Add Requisitions Default Accounts at line [%d] program [%s].",libname,line,file);
		BINfree(bin);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	if(x==(-1701)) version=FALSE;
	else version=TRUE;
	total=BINgetint(bin);
	for(y=0;y<total;++y)
	{
		if(r->a!=NULL)
		{
			r->a=Rrealloc(r->a,(r->num+1)*sizeof(ReqAcctStruct));
		} else {
			r->a=Rmalloc(sizeof(ReqAcctStruct));
		}
		a=r->a+r->num;
		a->accno=BINgetint(bin);
		a->deftype=BINgetshort(bin);
		test=BINgetshort(bin);
		if(test) a->account=BINgetstring(bin);
			else a->account=NULL;
		test=BINgetshort(bin);
		if(test) a->debit=BINgetstring(bin);	
			else a->debit=NULL;
		test=BINgetshort(bin);
		if(test) a->credit=BINgetstring(bin);
			else a->credit=NULL;
		a->prorate=BINgetshort(bin);
		a->percentage=BINgetdouble(bin);
		a->amount=BINgetdouble(bin);
		a->enc=0.0;
		a->pre_enc=0.0;
		if(version)
		{
			test=BINgetshort(bin);
			if(test) a->defdebit=BINgetstring(bin);
				else a->defdebit=NULL;
			test=BINgetshort(bin);
			if(test) a->defcredit=BINgetstring(bin);	
				else a->defcredit=NULL;
			test=BINgetshort(bin);
			if(test) a->defpayable=BINgetstring(bin);
				else a->defpayable=NULL;
			test=BINgetshort(bin);
			if(test) a->defbankid=BINgetstring(bin);
				else a->defbankid=NULL;
		} else {
			a->defdebit=NULL;
			a->defcredit=NULL;
			a->defpayable=NULL;
			a->defbankid=NULL;
		}
		++r->num;
	}
	BINfree(bin);
	if(libname!=NULL) Rfree(libname);
	return(0);
}
short xwriteAddReqDefAcct(REQACC *r,int line,char *file)
{
	BIN *bin;
	char *libname=NULL;
	ReqAcctStruct *a;
	int y;

	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(USERLOGIN)+13);
	sprintf(libname,"%s/%s/REQMGT.STP",CURRENTDIRECTORY,USERLOGIN);	
	if(diagmix)
	{
		prterr("DIAG writeAddReqDefAcct Writing Add Requisitions Default Accounts to Library [%s] at line [%d] program [%s].",libname,line,file);
	}
	bin=BINnew();
	BINaddshort(bin,-1701);	/*type identifier*/
	BINaddint(bin,r->num);
	for(y=0,a=r->a;y<r->num;++y,++a)
	{
		BINaddint(bin,a->accno);
		BINaddshort(bin,a->deftype);
		if(!isEMPTY(a->account))
		{
			BINaddshort(bin,TRUE);
			BINaddstring(bin,a->account);
		} else BINaddshort(bin,FALSE);
		if(!isEMPTY(a->debit))
		{
			BINaddshort(bin,TRUE);
			BINaddstring(bin,a->debit);
		} else BINaddshort(bin,FALSE);
		if(!isEMPTY(a->credit))
		{
			BINaddshort(bin,TRUE);
			BINaddstring(bin,a->credit);
		} else BINaddshort(bin,FALSE);
		BINaddshort(bin,a->prorate);
		BINadddouble(bin,a->percentage);
		BINadddouble(bin,a->amount);
		if(!isEMPTY(a->defdebit))
		{
			BINaddshort(bin,TRUE);
			BINaddstring(bin,a->defdebit);
		} else BINaddshort(bin,FALSE);
		if(!isEMPTY(a->defcredit))
		{
			BINaddshort(bin,TRUE);
			BINaddstring(bin,a->defcredit);
		} else BINaddshort(bin,FALSE);
		if(!isEMPTY(a->defpayable))
		{
			BINaddshort(bin,TRUE);
			BINaddstring(bin,a->defpayable);
		} else BINaddshort(bin,FALSE);
		if(!isEMPTY(a->defbankid))
		{
			BINaddshort(bin,TRUE);
			BINaddstring(bin,a->defbankid);
		} else BINaddshort(bin,FALSE);
	}
	if(writelibbin(libname,bin,"ADD REQUISITIONS MULTIPLE ACCOUNTS"))
	{
		prterr("Error Can't write Add Requisitions Default Accounts in binary Library [%s] at line [%d] program [%s].",libname,line,file);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	if(libname!=NULL) Rfree(libname);
	return(0);
}
void xMoveREQACC(REQACC *a,int line,char *file)
{
	ReqAcctStruct *rac=NULL;
	int x;

	if(diagmix)
	{
		prterr("DIAG MoveREQACC at line [%d] program [%s].",line,file);
	}
	if(a->a!=NULL)
	{
		for(x=0,rac=a->a;x<a->num;++x,++rac)
		{
			rac->pre_enc=rac->enc;
		}
	}
}
