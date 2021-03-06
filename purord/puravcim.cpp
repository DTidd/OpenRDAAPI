/* puravcim.c - Add Purchase Order Invoice Maintain Screen */
#include <cstdio>

#include <mix.hpp>
#include <venpmt.hpp>
#include <fin.hpp>

char skip_invoice_date=0,prorate_excess=FALSE,set_f=FALSE;
char set_copy_desc=FALSE;
char prorate_extra=FALSE,*duedate=NULL,*invdate=NULL;
double amt2prorate=0.0;
extern RDAvenpmt *venpmt;
extern MakeBrowseList *mbl_purinv;
extern MakeBrowseList *mbl_addpo;
extern char *poid,*vendorid,*invid,*checkdate;
extern char manual_voucher,prepaid,pay_1099,check_detail;
extern int yearid,checkno,venyrid,purordfyear,venpmtfyear;
extern short pmonth;
extern short fmonth,venpmtfmonth,purordfmonth;
extern char *module;
extern short infnum,invnum,vennum,vchnum,venadrnum,msrnum,sitenum;
extern short yearnum,fyearnum,finyrnum;
extern short bnkacc;
extern int PO_COUNT;
extern void browse_purapv(RDArsrc *);
extern void browse_inv(RDArsrc *,RDATData *previous);
static void PURAVCIM_save_record(RDArsrc *,RDATData *previous,short),quit_record(RDArsrc *,RDATData *previous);
static void PURAVCIM_save_record_up(RDArsrc *,RDATData *previous),PURAVCIM_save_record_nup(RDArsrc *,RDATData *previous);
static void getfile(RDArsrc *,RDATData *previous),getfilecb(RDArsrc *,RDATData *previous);
static void getaddr(RDArsrc *),getaddrcb(RDArsrc *);
static void setca(RDArsrc *);
static void add_vouchers(RDArsrc *);
static void test_add_vouchers(RDArsrc *,RDATData *);
static void saveinv_addvchs(RDArsrc *,RDATData *);
static void PURAVCIM_resetdefs(RDArsrc *,RDATData *);
static short PURAVCIM_save_check(RDArsrc *,RDATData *,short);
static short save_venyr(RDArsrc *,RDATData *);
static APPlib *addresslist=NULL,*fmonths=NULL;
char editinv=TRUE,ClosePO=FALSE;
void purinvm(short,void *,int,char **);
void setaddrlist(RDArsrc *);
void changeADDRlist(RDArsrc *);
double invoice_total=0;

static void PURAVCIM_quit_record_test(RDArsrc *mtnrsrc,RDATData *previous)
{
	quit_record(mtnrsrc,previous);
}
void change_invoice_total(RDArsrc *mtnrsrc)
{
	readwidget(mtnrsrc,"INVOICE TOTAL");
	FINDRSCGETDOUBLE(mtnrsrc,"INVOICE TOTAL",&invoice_total);
}
void test_add_vouchers(RDArsrc *mtnrsrc,RDATData *previous)
{
	int venyrid=0,fm=0;

	readwidget(mtnrsrc,"COPY ACCOUNT DESCRIPTION");
	FINDRSCGETCHAR(mtnrsrc,"COPY ACCOUNT DESCRIPTION",&set_copy_desc);
	FINDRSCGETINT(mtnrsrc,"FISCAL MONTHS",&fm);	
	fmonth=fm;
	ZERNRD(invnum);
	readwidget(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]",&vendorid);
	FINDFLDSETSTRING(invnum,"VENDOR IDENTIFICATION",vendorid);
	readwidget(mtnrsrc,"[VENINV][CALENDAR YEAR]");
	FINDRSCGETINT(mtnrsrc,"[VENINV][CALENDAR YEAR]",&venyrid);
	FINDFLDSETINT(invnum,"CALENDAR YEAR",venyrid);
	readwidget(mtnrsrc,"[VENINV][INVOICE IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[VENINV][INVOICE IDENTIFICATION]",&invid);
	FINDFLDSETSTRING(invnum,"INVOICE IDENTIFICATION",invid);
	if(ADVEQLNRDsec(invnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","INVOICE DOES NOT EXIST!","Vouchers can not be added until the Invoice has been saved.  Invoice entered does not presently exist in the VENINV file.",saveinv_addvchs,NULL,FALSE,2,mtnrsrc,previous,NULL);
	} else {
		if(save_venyr(mtnrsrc,previous)) add_vouchers(mtnrsrc);
	}
}
static short save_venyr(RDArsrc *mtnrsrc,RDATData *previous)
{
	char deleteflag=FALSE,mess[1024];
	short d=0,m=0,y=0;

	readallwidgets(mtnrsrc);
	rsrc2filerecord(invnum,mtnrsrc);
	FINDFLDGETSTRING(invnum,"INVOICE DATE",&invdate);
	FINDFLDGETSTRING(invnum,"DUE DATE",&duedate);
	ZERNRD(yearnum);
	FINDFLDSETSTRING(yearnum,"VENDOR IDENTIFICATION",vendorid);
	FINDFLDSETINT(yearnum,"CALENDAR YEAR",venyrid);
	if(ADVEQLNRDsec(yearnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		if(ADVWRTTRANSsec(yearnum,0,NULL,NULL,SCRNvirtualSubData,mtnrsrc))
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","SECURITY FAILED ON WRITE!","Security failed on attempt to New Vendor Calendar Year record.  Vendor entered exist but the Vendor Calendar Year record does not.  Cannot add Vendor Voucher records until this is resolved.",NULL,NULL,FALSE,NULL);
			return(FALSE);
		}
	} else {
		FINDFLDGETCHAR(yearnum,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED VENDOR CALENDAR YEAR!","Vendor Calendar Year record exist for this Vendor but it is in a state of deletion.  Cannot add Vendor Voucher records until this is resolved.",NULL,NULL,FALSE,NULL);
			return(FALSE);
		}
	}
	if(fyearnum!=(-1) && venpmt->use_fiscal_summary==TRUE)
	{
		ZERNRD(fyearnum);
		FINDFLDSETSTRING(fyearnum,"VENDOR IDENTIFICATION",vendorid);
		FINDFLDSETINT(fyearnum,"FISCAL YEAR",yearid);
		if(ADVEQLNRDsec(fyearnum,1,SCRNvirtualSubData,mtnrsrc))
		{
			if(ADVWRTTRANSsec(fyearnum,0,NULL,NULL,SCRNvirtualSubData,mtnrsrc))
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","SECURITY FAILED ON WRITE!","Security failed on attempt to New Vendor Fiscal Year record.  Vendor entered exist but the Vendor Fiscal Year record does not.  Cannot add Vendor Voucher records until this is resolved.",NULL,NULL,FALSE,NULL);
				return(FALSE);
			}
		} else {
			FINDFLDGETCHAR(fyearnum,"DELETEFLAG",&deleteflag);
			if(deleteflag)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED VENDOR FISCAL YEAR!","Vendor Fiscal Year record exist for this Vendor but it is in a state of deletion.  Cannot add Vendor Voucher records until this is resolved.",NULL,NULL,FALSE,NULL);
				return(FALSE);
			}
		}
	}
	if((RDAstrlen(duedate))!=10)
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DUE DATE!","INVALID DUE DATE: Your Due Date must be in a 10-digit format (e.g. MM/DD/YYYY).  Please key in a correct Due Date before proceeding.",NULL,NULL,FALSE,NULL);
		return(FALSE);
	}
	memset(stemp,0,101);
	sprintf(stemp,"%.2s",duedate);
	m=atoi(stemp);
	memset(stemp,0,101);
	sprintf(stemp,"%.2s",&duedate[3]);
	d=atoi(stemp);
	memset(stemp,0,101);
	sprintf(stemp,"%.4s",&duedate[6]);
	y=atoi(stemp);
	if((m<1 || m>12) || (d<1 || d>31) || (y<2000 || y>2100))
	{
		memset(mess,0,sizeof(mess));
		sprintf(mess,"DUE DATE ERROR:  The invoice due date [%s] isn't a valid date.",duedate);
		ERRORDIALOG("DUE DATE ERROR!",mess,NULL,FALSE); 
		return(FALSE);
	}
	if((RDAstrlen(invdate))!=10)
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID INVOICE DATE!","INVALID INVOICE DATE: Your Invoice Date must be in a 10-digit format (e.g. MM/DD/YYYY).  Please key in a correct Invoice Date before proceeding.",NULL,NULL,FALSE,NULL);
		return(FALSE);
	}
	memset(stemp,0,101);
	sprintf(stemp,"%.2s",invdate);
	m=atoi(stemp);
	memset(stemp,0,101);
	sprintf(stemp,"%.2s",&invdate[3]);
	d=atoi(stemp);
	memset(stemp,0,101);
	sprintf(stemp,"%.4s",&invdate[6]);
	y=atoi(stemp);
	if((m<1 || m>12) || (d<1 || d>31) || (y<2000 || y>2100))
	{
		memset(mess,0,sizeof(mess));
		sprintf(mess,"INVOICE DATE ERROR:  The invoice date [%s] isn't a valid date.",invdate);
		ERRORDIALOG("INVOICE DATE ERROR!",mess,NULL,FALSE); 
		return(FALSE);
	}
	return(TRUE);
}
void saveinv_addvchs(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(mbl_purinv!=NULL)
	{
		if(PURAVCIM_save_check(mtnrsrc,previous,TRUE))
		{
			/*
			add_vouchers(mtnrsrc);
			*/
		}
	} else {
		if(PURAVCIM_save_check(mtnrsrc,previous,FALSE))
		{
			/*
			add_vouchers(mtnrsrc);
			*/
		}
	}
}
void add_vouchers(RDArsrc *mtnrsrc)
{
	readallwidgets(mtnrsrc);
	FINDRSCGETCHAR(mtnrsrc,"PRORATE EXCESS",&prorate_excess);
	FINDRSCGETCHAR(mtnrsrc,"PRORATE EXTRA",&prorate_extra);
	FINDRSCGETDOUBLE(mtnrsrc,"AMOUNT TO PRORATE",&amt2prorate);
	FINDRSCGETCHAR(mtnrsrc,"CLOSE PO",&ClosePO);
	FINDRSCGETDOUBLE(mtnrsrc,"INVOICE TOTAL",&invoice_total);
	FINDRSCGETSTRING(mtnrsrc,"[VENINV][INVOICE IDENTIFICATION]",&invid);
	FINDRSCGETSTRING(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]",&vendorid);
/*
	FINDRSCGETINT(mtnrsrc,"[VENINV][CALENDAR YEAR]",&yearid);
*/
	FINDRSCGETINT(mtnrsrc,"[VENINV][CALENDAR YEAR]",&venyrid);
	FINDRSCGETCHAR(mtnrsrc,"MANUAL VOUCHER",&manual_voucher);
	FINDRSCGETINT(mtnrsrc,"CHECK NUMBER",&checkno);
	if(checkdate!=NULL) Rfree(checkdate);
	FINDRSCGETSTRING(mtnrsrc,"CHECK DATE",&checkdate);
	FINDRSCGETCHAR(mtnrsrc,"PREPAID",&prepaid);
	FINDRSCGETCHAR(mtnrsrc,"1099 PAYMENT",&pay_1099);
	FINDRSCGETCHAR(mtnrsrc,"CHECK DETAIL",&check_detail);
	browse_purapv(mtnrsrc);
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
}
void changeADDRlist(RDArsrc *mtnrsrc)
{
	int seladdr;
	char *vendorid1=NULL,*temp=NULL;

	if(FINDRSCGETINT(mtnrsrc,"ADDRESS LIST",&seladdr)) return;
	FINDFLDGETSTRING(invnum,"VENDOR IDENTIFICATION",&vendorid1);
	ZERNRD(venadrnum);
	if(!RDAstrcmp(addresslist->libs[seladdr],"No Addresses Defined"))
	{
		FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid1);
		FINDFLDSETSTRING(venadrnum,"ADDRESS IDENTIFICATION",NULL);
		FINDFLDSETSTRING(invnum,"ADDRESS IDENTIFICATION",NULL);
	} else {
		FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid1);
		FINDFLDSETSTRING(venadrnum,"ADDRESS IDENTIFICATION",addresslist->libs[seladdr]);
		FINDFLDSETSTRING(invnum,"ADDRESS IDENTIFICATION",addresslist->libs[seladdr]);
	}
	if(ADVEQLNRDsec(venadrnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(venadrnum,1);
	filerecord2rsrc(venadrnum,mtnrsrc);
	FINDFLDGETSTRING(venadrnum,"CUSTOMER NUMBER",&temp);
	FINDRSCSETSTRING(mtnrsrc,"[VENINV][CREDIT CARD NUMBER]",temp);
	updatersrc(mtnrsrc,"[VENINV][CREDIT CARD NUMBER]");
	if(temp!=NULL) Rfree(temp);
	updatefilerecord2rsrc(venadrnum,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
	if(vendorid1!=NULL) Rfree(vendorid1);
}
static void setaddrlist1(RDArsrc *mtnrsrc)
{
	short ef;
	char *vendorid1=NULL,*vendorid2=NULL;
	char *addrid2=NULL,*temp=NULL;
	char delflag=FALSE;
	int seladdr=0;

	if(addresslist!=NULL) freeapplib(addresslist);
	addresslist=APPlibNEW();
	FINDFLDGETSTRING(invnum,"VENDOR IDENTIFICATION",&vendorid1);
	FINDRSCGETINT(mtnrsrc,"ADDRESS LIST",&seladdr);
	ZERNRD(venadrnum);
	FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid1);
	ef=ADVGTENRDsec(venadrnum,1,SCRNvirtualSubData,mtnrsrc);
	while(!ef)
	{
		if(vendorid2!=NULL) Rfree(vendorid2);
		FINDFLDGETSTRING(venadrnum,"VENDOR IDENTIFICATION",&vendorid2);
		if(RDAstrcmp(vendorid2,vendorid1)) break;
		FINDFLDGETCHAR(venadrnum,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			if(addrid2!=NULL) Rfree(addrid2);
			FINDFLDGETSTRING(venadrnum,"ADDRESS IDENTIFICATION",&addrid2);
			addAPPlib(addresslist,addrid2);
		}
		ef=ADVNXTNRDsec(venadrnum,1,SCRNvirtualSubData,mtnrsrc);
	}
	if(addresslist->numlibs<1)
	{
		addAPPlib(addresslist,"No Addresses Defined");
		seladdr=0;
	} else {
		if(seladdr>=addresslist->numlibs) seladdr=0;
	}
	FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid1);
	FINDFLDSETSTRING(venadrnum,"ADDRESS IDENTIFICATION",addresslist->libs[seladdr]);
	if(ADVEQLNRDsec(venadrnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(venadrnum,1);
	FINDFLDGETSTRING(venadrnum,"CUSTOMER NUMBER",&temp);
	FINDRSCSETSTRING(mtnrsrc,"[VENINV][CREDIT CARD NUMBER]",temp);
	updatersrc(mtnrsrc,"[VENINV][CREDIT CARD NUMBER]");
	if(temp!=NULL) Rfree(temp);
	updatefilerecord2rsrc(venadrnum,mtnrsrc);
	if(!FINDRSCLISTAPPlib(mtnrsrc,"ADDRESS LIST",seladdr,addresslist))
	{
		updatersrc(mtnrsrc,"ADDRESS LIST");
	}
	if(addrid2!=NULL) Rfree(addrid2);
	if(vendorid1!=NULL) Rfree(vendorid1);
	if(vendorid2!=NULL) Rfree(vendorid2);
}
void setaddrlist(RDArsrc *mtnrsrc)
{
	short ef;
	char *vendorid1=NULL,*vendorid2=NULL;
	char *addrid1=NULL,*addrid2=NULL,*temp=NULL;
	char delflag=FALSE;
	int x=0;

	if(addresslist!=NULL) freeapplib(addresslist);
	addresslist=APPlibNEW();
	FINDFLDGETSTRING(invnum,"VENDOR IDENTIFICATION",&vendorid1);
	FINDFLDGETSTRING(invnum,"ADDRESS IDENTIFICATION",&addrid1);
	ZERNRD(venadrnum);
	FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid1);
	ef=ADVGTENRDsec(venadrnum,1,SCRNvirtualSubData,mtnrsrc);
	while(!ef)
	{
		if(vendorid2!=NULL) Rfree(vendorid2);
		FINDFLDGETSTRING(venadrnum,"VENDOR IDENTIFICATION",&vendorid2);
		if(RDAstrcmp(vendorid2,vendorid1)) break;
		FINDFLDGETCHAR(venadrnum,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			if(addrid2!=NULL) Rfree(addrid2);
			FINDFLDGETSTRING(venadrnum,"ADDRESS IDENTIFICATION",&addrid2);
			addAPPlib(addresslist,addrid2);
		}
		ef=ADVNXTNRDsec(venadrnum,1,SCRNvirtualSubData,mtnrsrc);
	}
	if(addresslist->numlibs<1)
	{
		addAPPlib(addresslist,"No Addresses Defined");
	} else {
		for(x=0;x<addresslist->numlibs;x++)
		{
			if(!RDAstrcmp(addrid1,addresslist->libs[x]))
				break;
		}
		if(x>=addresslist->numlibs)
		{
			x=0;
		}
	}
	FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid1);
	FINDFLDSETSTRING(venadrnum,"ADDRESS IDENTIFICATION",addresslist->libs[x]);
	if(ADVEQLNRDsec(venadrnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(venadrnum,1);
	FINDFLDGETSTRING(venadrnum,"CUSTOMER NUMBER",&temp);
	FINDRSCSETSTRING(mtnrsrc,"[VENINV][CREDIT CARD NUMBER]",temp);
	updatersrc(mtnrsrc,"[VENINV][CREDIT CARD NUMBER]");
	if(temp!=NULL) Rfree(temp);
	updatefilerecord2rsrc(venadrnum,mtnrsrc);
	if(!FINDRSCLISTAPPlib(mtnrsrc,"ADDRESS LIST",x,addresslist))
	{
		updatersrc(mtnrsrc,"ADDRESS LIST");
	}
	if(addrid1!=NULL) Rfree(addrid1);
	if(addrid2!=NULL) Rfree(addrid2);
	if(vendorid1!=NULL) Rfree(vendorid1);
	if(vendorid2!=NULL) Rfree(vendorid2);
}
static void getaddrcb(RDArsrc *mtnrsrc)
{
	char *addrid1=NULL,*temp=NULL;

	readwidget(mtnrsrc,"[VENINV][ADDRESS IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[VENINV][ADDRESS IDENTIFICATION]",&addrid1);
	FINDFLDSETSTRING(venadrnum,"ADDRESS IDENTIFICATION",addrid1);
	FINDFLDGETSTRING(venadrnum,"CUSTOMER NUMBER",&temp);
	FINDRSCSETSTRING(mtnrsrc,"[VENINV][CREDIT CARD NUMBER]",temp);
	updatersrc(mtnrsrc,"[VENINV][CREDIT CARD NUMBER]");
	if(temp!=NULL) Rfree(temp);
	getaddr(mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
	if(addrid1!=NULL) Rfree(addrid1);
}
static void getaddr(RDArsrc *mtnrsrc)
{
	char *addrid1=NULL;
	int x;

	FINDFLDGETSTRING(invnum,"ADDRESS IDENTIFICATION",&addrid1);
	if(RDAstrcmp(addresslist->libs[0],"No Addresses Defined"))
	{
		for(x=0;x<addresslist->numlibs;x++)
		{
			if(!RDAstrcmp(addrid1,addresslist->libs[x]))
				break;
		}
		if(x>=addresslist->numlibs)
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID ADDRESS IDENTIFICATION!","The Vendor Address Identification entered does not exist for the selected Vendor.",NULL,NULL,TRUE,NULL);
		} else {
			FINDRSCSETINT(mtnrsrc,"ADDRESS LIST",x);
			updatersrc(mtnrsrc,"ADDRESS LIST");
		}
	}
	if(addrid1!=NULL) Rfree(addrid1);
}
void setprorateexcess(RDArsrc *mtnrsrc)
{
	char t=FALSE;

	readwidget(mtnrsrc,"PRORATE EXCESS");
	FINDRSCGETCHAR(mtnrsrc,"PRORATE EXCESS",&t);
	if(t==TRUE)
	{
		t=FALSE;
		FINDRSCSETCHAR(mtnrsrc,"PRORATE EXTRA",t);
		updatersrc(mtnrsrc,"PRORATE EXTRA");
		FINDRSCSETEDITABLE(mtnrsrc,"AMOUNT TO PRORATE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"AMOUNT TO PRORATE",FALSE);
		updatersrc(mtnrsrc,"AMOUNT TO PRORATE");
	} else {
		t=TRUE;
		FINDRSCSETCHAR(mtnrsrc,"PRORATE EXTRA",t);
		updatersrc(mtnrsrc,"PRORATE EXTRA");
		FINDRSCSETEDITABLE(mtnrsrc,"AMOUNT TO PRORATE",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"AMOUNT TO PRORATE",TRUE);
		updatersrc(mtnrsrc,"AMOUNT TO PRORATE");
	}
}
void setprorateextra(RDArsrc *mtnrsrc)
{
	char t=FALSE;

	readwidget(mtnrsrc,"PRORATE EXTRA");
	FINDRSCGETCHAR(mtnrsrc,"PRORATE EXTRA",&t);
	if(t==TRUE)
	{
		t=FALSE;
		FINDRSCSETCHAR(mtnrsrc,"PRORATE EXCESS",t);
		updatersrc(mtnrsrc,"PRORATE EXCESS");
		FINDRSCSETEDITABLE(mtnrsrc,"AMOUNT TO PRORATE",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"AMOUNT TO PRORATE",TRUE);
		updatersrc(mtnrsrc,"AMOUNT TO PRORATE");
	} else {
		t=TRUE;
		FINDRSCSETCHAR(mtnrsrc,"PRORATE EXCESS",t);
		updatersrc(mtnrsrc,"PRORATE EXCESS");
		FINDRSCSETEDITABLE(mtnrsrc,"AMOUNT TO PRORATE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"AMOUNT TO PRORATE",FALSE);
		updatersrc(mtnrsrc,"AMOUNT TO PRORATE");
	}
}
static void setca(RDArsrc *r)
{
	char t=FALSE,*temp=NULL,*temp1=NULL;
	char *v=NULL,*i=NULL;
	int c=0;
	APPlib *a=NULL,*e=NULL;

	readallwidgets(r);
	FINDRSCGETCHAR(r,"CAPTURE ARCHIVE",&t);
	if(t==TRUE)
	{
		FINDRSCGETSTRING(r,"[VENINV][VENDOR IDENTIFICATION]",&v);
		FINDRSCGETSTRING(r,"[VENINV][INVOICE IDENTIFICATION]",&i);
		FINDRSCGETINT(r,"[VENINV][CALENDAR YEAR]",&c);
		a=APPlibNEW();
		e=APPlibNEW();
		addAPPlib(a,"PURORD");
		addAPPlib(a,"CAPTURE ARCHIVE");
		memset(stemp,0,101);
		sprintf(stemp,"ADD_VENDOR_IDENTIFICATION=%s",v);
		addAPPlib(e,stemp);
		memset(stemp,0,101);
		sprintf(stemp,"ADD_INVOICE_IDENTIFICATION=%s",i);
		addAPPlib(e,stemp);
		memset(stemp,0,101);
		sprintf(stemp,"ADD_CALENDAR_YEAR=%d",c);
		addAPPlib(e,stemp);
		ADVExecute2Program("doadd",a,e);
		if(a!=NULL) freeapplib(a);
		if(e!=NULL) freeapplib(e);
		temp=Rmalloc(RDAstrlen(v)+RDAstrlen(i)+15);
		sprintf(temp,"%s%s.PDF",(v!=NULL ? v:""),(i!=NULL ? i:""));
		temp1=adddashes(temp);
	}
	if(!isEMPTY(temp1))
	{
		FINDFLDSETSTRING(invnum,"TYPE","PDF");
	} else {
		FINDFLDSETSTRING(invnum,"TYPE","");
	}
	FINDFLDSETSTRING(invnum,"FILENAME",temp1);
	FINDRSCSETSTRING(r,"ARCHIVE FILENAME",temp1);
	if(temp1!=NULL) Rfree(temp1);
	updatersrc(r,"ARCHIVE FILENAME");
}
static void setfmonth(RDArsrc *r)
{
	int l=0;

	FINDRSCGETINT(r,"FISCAL MONTHS",&l);
	fmonth=l;
	set_f=TRUE;
}
static void getinvdate(RDArsrc *r)
{
	readwidget(r,"[VENINV][INVOICE DATE]");
	FINDRSCGETSTRING(r,"[VENINV][INVOICE DATE]",&invdate);
}
static void getduedate(RDArsrc *r)
{
	int x=0;

	readwidget(r,"[VENINV][DUE DATE]");
	FINDRSCGETSTRING(r,"[VENINV][DUE DATE]",&duedate);
/* SPRINGFIELD SPECIFIC BELOW */
	if(!RDAstrcmp(XPERT_SETUP->zip,"62702-5030"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%.2s",(duedate!=NULL ? duedate:""));
		x=atoi(stemp);
		FINDRSCSETINT(r,"FISCAL MONTHS",x);
		updatersrc(r,"FISCAL MONTHS");
	}
/* SPRINGFIELD SPECIFIC ABOVE */
}
static void setcopydesc(RDArsrc *r)
{
	readwidget(r,"COPY ACCOUNT DESCRIPTION");
	FINDRSCGETCHAR(r,"COPY ACCOUNT DESCRIPTION",&set_copy_desc);
}
void purinvm(short dowhich,void *targetkey,int argc,char **argv)
{
	RDArsrc *mtnrsrc=NULL;
	double dlrs=0;
	int x=0,which_type=0;
	char tvalue=FALSE,*addrid=NULL;
	RDATData *previous=NULL;
	char set_skip_records=FALSE;
	char *temp=NULL,cc=FALSE;
	NRDfield *field=NULL;
	int p_f_year=0;

	if(skip_invoice_date) 
	{
		if(invdate!=NULL) Rfree(invdate);
	}
	ClosePO=FALSE;
	mtnrsrc=RDArsrcNEW(module,"ADD PO INVOICE SCREEN");
	if(fmonths!=NULL) freeapplib(fmonths);
	fmonths=APPlibNEW();
	for(x=0;x<14;++x) addAPPlib(fmonths,FISCALPERIOD[x]);
	addDFincvir(mtnrsrc,"PURORD","PURMST",NULL,msrnum);
	addDFincvir(mtnrsrc,"PURORD","PURSIT",NULL,sitenum);
	addDFincvir(mtnrsrc,"VENPMT","VENMSR",NULL,vennum);
	addDFincvir(mtnrsrc,"VENPMT","VENYR",NULL,yearnum);
	if(venpmt->use_fiscal_summary==TRUE)
	{
		addDFincvir(mtnrsrc,"VENPMT","VENFYR",NULL,fyearnum);
	}
	addDFincvir(mtnrsrc,"VENPMT","VENINV",NULL,invnum);
	addDFincvir(mtnrsrc,"VENPMT","VENINF",NULL,infnum);
	addDFincvir(mtnrsrc,"VENPMT","VENADR",NULL,venadrnum);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	ZERNRD(infnum);
	ZERNRD(invnum);
	previous=RDATDataNEW(invnum);
	if(invnum!=(-1)) file2rsrc(invnum,mtnrsrc,editinv);
	FINDRSCSETFUNC(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]",getfilecb,
		previous);
	FINDRSCSETFUNC(mtnrsrc,"[VENINV][CALENDAR YEAR]",getfilecb,
		previous);
	FINDRSCSETFUNC(mtnrsrc,"[VENINV][INVOICE IDENTIFICATION]",getfilecb,
		previous);
	if(infnum!=(-1)) file2rsrc(infnum,mtnrsrc,FALSE);
	if(sitenum!=(-1)) file2rsrc(sitenum,mtnrsrc,FALSE);
	if(msrnum!=(-1)) file2rsrc(msrnum,mtnrsrc,FALSE);
	if(vennum!=(-1)) file2rsrc(vennum,mtnrsrc,FALSE);
	if(yearnum!=(-1)) file2rsrc(yearnum,mtnrsrc,FALSE);
	if(venpmt->use_fiscal_summary==TRUE)
	{
		if(fyearnum!=(-1)) file2rsrc(fyearnum,mtnrsrc,FALSE);
	}
	if(venadrnum!=(-1)) file2rsrc(venadrnum,mtnrsrc,FALSE);
	if(bnkacc!=(-1)) file2rsrc(bnkacc,mtnrsrc,FALSE);
	if(XPERT_SETUP->ARCHIVE==TRUE)
	{
		addstdrsrc(mtnrsrc,"CAPTURE ARCHIVE",BOOLNS,1,&tvalue,TRUE);
		FINDRSCSETFUNC(mtnrsrc,"CAPTURE ARCHIVE",setca,NULL);
		addstdrsrc(mtnrsrc,"ARCHIVE FILENAME",VARIABLETEXT,0,NULL,FALSE);
	}
	addstdrsrc(mtnrsrc,"PRORATE EXCESS",BOOLNS,1,&tvalue,TRUE);
	addstdrsrc(mtnrsrc,"PRORATE EXTRA",BOOLNS,1,&tvalue,TRUE);
	amt2prorate=0.0;
	addstdrsrc(mtnrsrc,"AMOUNT TO PRORATE",DOLLARS,20,&amt2prorate,TRUE);
/* HERE  DDC (03/18/2007): Instead of repeatedly pulling the fiscal month from the FINYEAR file, now it copies the value from the default fiscal month GSV.
	COPYFIELD(msrnum,finyrnum,"FISCAL YEAR");
	if(ADVEQLNRDsec(finyrnum,1,SCRNvirtualSubData,mbl_addpo->mainrsrc)) KEYNRD(finyrnum,1);
	if(set_f==FALSE)
	{
		FINDFLDGETSHORT(finyrnum,"PROCESSING MONTH",&fmonth);
	}
*/
	FINDFLDGETINT(msrnum,"FISCAL YEAR",&p_f_year);
	if(p_f_year==venpmtfyear) fmonth=venpmtfmonth;
		else fmonth=purordfmonth;
	FINDRSCSETFUNC(mtnrsrc,"PRORATE EXCESS",setprorateexcess,NULL);
	FINDRSCSETFUNC(mtnrsrc,"PRORATE EXTRA",setprorateextra,NULL);
	which_type=fmonth;
	addlstrsrc(mtnrsrc,"FISCAL MONTHS",&which_type,TRUE,setfmonth,
		fmonths->numlibs,&fmonths->libs,NULL);
	if(vennum!=(-1))
	{
		COPYFIELD(msrnum,vennum,"VENDOR IDENTIFICATION");
		if(ADVEQLNRDsec(vennum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(vennum,1);
		filerecord2rsrc(vennum,mtnrsrc);
		FINDFLDGETCHAR(vennum,"DEFAULT 1099 PAYMENT",&pay_1099);
	}
	if(yearnum!=(-1))
	{
		FINDFLDSETSTRING(yearnum,"VENDOR IDENTIFICATION",vendorid);
		FINDFLDSETINT(yearnum,"CALENDAR YEAR",venyrid);
		if(ADVEQLNRDsec(yearnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(yearnum,1);
		filerecord2rsrc(yearnum,mtnrsrc);
	}
	if(fyearnum!=(-1) && venpmt->use_fiscal_summary==TRUE)
	{
		FINDFLDSETSTRING(fyearnum,"VENDOR IDENTIFICATION",vendorid);
		FINDFLDSETINT(fyearnum,"FISCAL YEAR",yearid);
		if(ADVEQLNRDsec(fyearnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(fyearnum,1);
		filerecord2rsrc(fyearnum,mtnrsrc);
	}
	if(sitenum!=(-1))
	{
		COPYFIELD(msrnum,sitenum,"SITE IDENTIFICATION");
		if(ADVEQLNRDsec(sitenum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(sitenum,1);
		filerecord2rsrc(sitenum,mtnrsrc);
	}
	if(infnum!=(-1))
	{
		COPYFIELD(invnum,infnum,"VENDOR IDENTIFICATION");
		COPYFIELD(invnum,infnum,"INVOICE IDENTIFICATION");
		if(ADVEQLNRDsec(infnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(infnum,1);
		filerecord2rsrc(infnum,mtnrsrc);
	}
	if(venadrnum!=(-1))
	{
		FINDFLDGETSTRING(invnum,"ADDRESS IDENTIFICATION",&addrid);
		FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid);
		FINDFLDSETSTRING(venadrnum,"ADDRESS IDENTIFICATION",addrid);
		if(ADVEQLNRDsec(venadrnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(venadrnum,1);
		filerecord2rsrc(venadrnum,mtnrsrc);
		if(addrid!=NULL) Rfree(addrid);
	}
	addresslist=APPlibNEW();
	addlstrsrc(mtnrsrc,"ADDRESS LIST",&x,TRUE,changeADDRlist,
		addresslist->numlibs,&addresslist->libs,NULL);
	FINDRSCSETFUNC(mtnrsrc,"[VENINV][ADDRESS IDENTIFICATION]",getaddrcb,NULL);
	FINDRSCSETFUNC(mtnrsrc,"[VENINV][DUE DATE]",getduedate,NULL);
	if(!isEMPTY(duedate))
	{
		FINDRSCSETSTRING(mtnrsrc,"[VENINV][DUE DATE]",duedate);
	} else {
		getduedate(mtnrsrc);
	}
	FINDRSCSETFUNC(mtnrsrc,"[VENINV][INVOICE DATE]",getinvdate,NULL);
	if(!isEMPTY(invdate))
	{
		FINDRSCSETSTRING(mtnrsrc,"[VENINV][INVOICE DATE]",invdate);
	} else {
		getinvdate(mtnrsrc);
	}
	set_skip_records=FALSE;
	addstdrsrc(mtnrsrc,"DEFAULT SKIP ALL",BOOLNS,1,&set_skip_records,TRUE);
	set_copy_desc=FALSE;
	addstdrsrc(mtnrsrc,"COPY ACCOUNT DESCRIPTION",BOOLNS,1,&set_copy_desc,TRUE);
	FINDRSCSETFUNC(mtnrsrc,"COPY ACCOUNT DESCRIPTION",setcopydesc,NULL);
	DefaultScreens(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"[VENINV][INVOICE DATE]",&invdate);
	readwidget(mbl_addpo->mainrsrc,"CALENDAR YEAR");
	FINDRSCGETINT(mbl_addpo->mainrsrc,"CALENDAR YEAR",&venyrid);
	FINDRSCSETINT(mtnrsrc,"[VENINV][CALENDAR YEAR]",venyrid);
	readwidget(mbl_addpo->mainrsrc,"PROCESSING MONTH");
	FINDRSCGETSHORT(mbl_addpo->mainrsrc,"PROCESSING MONTH",&pmonth);

/* HERE
	COPYFIELD(msrnum,finyrnum,"FISCAL YEAR");
	if(ADVEQLNRDsec(finyrnum,1,SCRNvirtualSubData,mbl_addpo->mainrsrc)) KEYNRD(finyrnum,1);
	FINDFLDGETSHORT(finyrnum,"PROCESSING MONTH",&fmonth);
*/
	fmonth=venpmtfmonth;
	switch(dowhich)
	{
		case 1:
			FINDRSCGETSTRING(mtnrsrc,"[VENINV][INVOICE IDENTIFICATION]",&invid);
			if(msrnum!=(-1))
			{
				if(ADVEQLNRDKEYsec(msrnum,1,targetkey,SCRNvirtualSubData,mtnrsrc)) 
				{
					KEYNRD(msrnum,1);
					FINDFLDSETSTRING(msrnum,"VENDOR IDENTIFICATION",vendorid);
				}
				filerecord2rsrc(msrnum,mtnrsrc);
				FINDFLDGETSTRING(msrnum,"PO IDENTIFICATION",&poid);
				FINDFLDGETCHAR(msrnum,"CREDIT CARD",&cc);
				if(!cc)
				{
					FINDFLDGETSTRING(msrnum,"VENDOR IDENTIFICATION",&vendorid);
				} else {
					FINDFLDGETSTRING(msrnum,"CREDIT CARD VENDOR ID",&vendorid);
				}
				FINDFLDGETINT(msrnum,"FISCAL YEAR",&yearid);
			}
			if(invnum!=(-1))
			{
				FINDFLDSETSTRING(invnum,"VENDOR IDENTIFICATION",vendorid);
				FINDFLDSETINT(invnum,"CALENDAR YEAR",venyrid);
				FINDFLDSETSTRING(invnum,"INVOICE IDENTIFICATION",invid);
				if(ADVEQLNRDsec(invnum,1,SCRNvirtualSubData,mtnrsrc))
				{
					KEYNRD(invnum,1);
					FINDRSCSETSTRING(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]",vendorid);
					FINDFLDGETSTRING(msrnum,"BILLING VENDOR ADDRESS IDENTIFICATION",&temp);
					FINDFLDSETSTRING(invnum,"ADDRESS IDENTIFICATION",temp);
					FINDRSCSETSTRING(mtnrsrc,"[VENINV][ADDRESS IDENTIFICATION]",temp);
					if(temp!=NULL) Rfree(temp);
					if(!skip_invoice_date)
					{
					field=FLDNRD(invnum,"INVOICE DATE");
					if(field!=NULL)
					{
						if(field->len==8)
						{
							FINDRSCSETSTRING(mtnrsrc,"[VENINV][INVOICE DATE]",CURRENT_DATE);
						} else FINDRSCSETSTRING(mtnrsrc,"[VENINV][INVOICE DATE]",CURRENT_DATE10);
					}
					}
					field=FLDNRD(invnum,"ENTRY DATE");
					if(field!=NULL)
					{
						if(field->len==8)
						{
							FINDRSCSETSTRING(mtnrsrc,"[VENINV][ENTRY DATE]",CURRENT_DATE);
						} else FINDRSCSETSTRING(mtnrsrc,"[VENINV][ENTRY DATE]",CURRENT_DATE10);
					}
					FINDRSCSETSTRING(mtnrsrc,"[VENINV][ENTRY TIME]",CURRENT_TIME);
					FINDRSCSETSTRING(mtnrsrc,"[VENINV][SOURCE USER]",USERLOGIN);
				} else {
					filerecord2rsrc(invnum,mtnrsrc);
					GetRDATData(invnum,previous);
				}
			}
			break;
		case 2: /* this case isn't used */
			prterr("Error used case 2 unexpectedly.");
			break;
		case 0: /* case 0 is not being used - 11/16/94 bld */
		default:
			if(ADVEQLNRDKEYsec(msrnum,1,targetkey,SCRNvirtualSubData,mtnrsrc)) KEYNRD(msrnum,1);
			filerecord2rsrc(msrnum,mtnrsrc);
			FINDFLDGETSTRING(msrnum,"PO IDENTIFICATION",&poid);
			FINDFLDGETCHAR(msrnum,"CREDIT CARD",&cc);
			if(!cc)
			{
				FINDFLDGETSTRING(msrnum,"VENDOR IDENTIFICATION",&vendorid);
			} else {
				FINDFLDGETSTRING(msrnum,"CREDIT CARD VENDOR ID",&vendorid);
			}
			FINDFLDGETINT(msrnum,"FISCAL YEAR",&yearid);
			FINDFLDGETSTRING(msrnum,"INVOICE IDENTIFICATION",&invid);
			FINDFLDSETSTRING(invnum,"VENDOR IDENTIFICATION",vendorid);
			FINDFLDSETINT(invnum,"CALENDAR YEAR",venyrid);
			FINDFLDSETSTRING(invnum,"INVOICE IDENTIFICATION",invid);
			if(ADVEQLNRDsec(invnum,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(invnum,1);
				FINDFLDGETSTRING(msrnum,"BILLING VENDOR ADDRESS IDENTIFICATION",&temp);
				FINDFLDSETSTRING(invnum,"ADDRESS IDENTIFICATION",temp);
				FINDRSCSETSTRING(mtnrsrc,"[VENINV][ADDRESS IDENTIFICATION]",temp);
				if(temp!=NULL) Rfree(temp);
				FINDRSCSETSTRING(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]",vendorid);
				if(!skip_invoice_date)
				{
				FINDRSCSETSTRING(mtnrsrc,"[VENINV][INVOICE DATE]",CURRENT_DATE);
				}
				FINDRSCSETSTRING(mtnrsrc,"[VENINV][ENTRY DATE]",CURRENT_DATE);
				FINDRSCSETSTRING(mtnrsrc,"[VENINV][ENTRY TIME]",CURRENT_TIME);
				FINDRSCSETSTRING(mtnrsrc,"[VENINV][SOURCE USER]",USERLOGIN);
			} else {
				filerecord2rsrc(invnum,mtnrsrc);
				GetRDATData(invnum,previous);
			}
			break;
	}
/*
	setaddrlist1(mtnrsrc);
*/
	setaddrlist(mtnrsrc);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	if(editinv)
	{
		addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,PURAVCIM_save_record_nup,previous);
		if(mbl_purinv!=NULL)
		{
			addrfcbrsrc(mtnrsrc,"SAVE",TRUE,PURAVCIM_save_record_up,previous);
		} else {
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,PURAVCIM_save_record_nup,previous);
		}
	}
	addstdrsrc(mtnrsrc,"INVOICE TOTAL",DOLLARS,20,&dlrs,TRUE);
	FINDRSCSETFUNC(mtnrsrc,"INVOICE TOTAL",change_invoice_total,NULL);
	addstdrsrc(mtnrsrc,"MANUAL VOUCHER",BOOLNS,1,&manual_voucher,TRUE);
	x=0;
	addstdrsrc(mtnrsrc,"CHECK NUMBER",LONGV,10,&checkno,TRUE);
	field=FLDNRD(vchnum,"CHECK DATE");
	if(field!=NULL)
	{
		if(field->len==8)
		{
			if(checkdate==NULL) checkdate=stralloc(CURRENT_DATE);
			addstdrsrc(mtnrsrc,"CHECK DATE",DATES,8,checkdate,TRUE);
		} else {
			if(checkdate==NULL) checkdate=stralloc(CURRENT_DATE10);
			addstdrsrc(mtnrsrc,"CHECK DATE",DATES,10,checkdate,TRUE);
		}
	} else addstdrsrc(mtnrsrc,"CHECK DATE",DATES,8,checkdate,TRUE);
	addstdrsrc(mtnrsrc,"PREPAID",BOOLNS,1,&prepaid,TRUE);
	addstdrsrc(mtnrsrc,"1099 PAYMENT",BOOLNS,1,&pay_1099,TRUE);
	addstdrsrc(mtnrsrc,"CLOSE PO",BOOLNS,1,&ClosePO,TRUE);
	addstdrsrc(mtnrsrc,"CHECK DETAIL",BOOLNS,1,&check_detail,TRUE);
	getfile(mtnrsrc,previous);
	if(mbl_purinv==NULL)
	{
		addbtnrsrc(mtnrsrc,"BROWSE",TRUE,browse_inv,previous);
	}
	addbtnrsrc(mtnrsrc,"ADD VOUCHERS",TRUE,test_add_vouchers,previous);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,PURAVCIM_resetdefs,previous);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,PURAVCIM_quit_record_test,previous);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	setprorateexcess(mtnrsrc);
	computeallSCRNvirtuals(mtnrsrc);
	FINDRSCSETEDITABLE(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]",FALSE);
	FINDRSCSETEDITABLE(mtnrsrc,"[VENINV][CALENDAR YEAR]",FALSE);
/* SPRINGFIELD SPECIFIC BELOW */
	if(!RDAstrcmp(XPERT_SETUP->zip,"62702-5030"))
	{
		getduedate(mtnrsrc);
	}
/* SPRINGFIELD SPECIFIC ABOVE */
	APPmakescrn(mtnrsrc,TRUE,quit_record,previous,(mbl_purinv==NULL?TRUE:FALSE));
	FINDRSCSETINPUTFOCUS(mtnrsrc,"[VENINV][INVOICE IDENTIFICATION]");
}
static void PURAVCIM_save_record_up(RDArsrc *mtnrsrc,RDATData *previous)
{
	PURAVCIM_save_check(mtnrsrc,previous,TRUE);
}
static void PURAVCIM_save_record_nup(RDArsrc *mtnrsrc,RDATData *previous)
{
	PURAVCIM_save_check(mtnrsrc,previous,FALSE);
}
static short PURAVCIM_save_check(RDArsrc *mtnrsrc,RDATData *previous,short update_list)
{
/* SPRINGFIELD SPECIFIC BELOW */
	char *duedate=NULL;
	char mess[1024];

	if(!RDAstrcmp(XPERT_SETUP->zip,"62702-5030"))
	{
		readallwidgets(mtnrsrc);
		rsrc2filerecord(invnum,mtnrsrc);
		FINDFLDGETSTRING(invnum,"DUE DATE",&duedate);
		memset(stemp,0,101);
		sprintf(stemp,"%.2s",(duedate!=NULL ? duedate:""));
		if((fmonth!=(atoi(stemp))) && fmonth!=0 && fmonth!=13)
		{
			memset(mess,0,sizeof(mess));
			sprintf(mess,"IMPLEMENTATION ERROR:  The invoice due date [%s] doesn't agree with the fiscal month [%d].  This is a Springfield requirement.",duedate,fmonth);
			ERRORDIALOG("SPRINGFIELD REQUIRED ERROR!",mess,NULL,FALSE); 
			if(duedate!=NULL) Rfree(duedate);
			return(TRUE);
		}
		if(duedate!=NULL) Rfree(duedate);
		if(save_venyr(mtnrsrc,previous))
		{
			PURAVCIM_save_record(mtnrsrc,previous,update_list);
			add_vouchers(mtnrsrc);
			return(TRUE);
		} else return(FALSE);
/* SPRINGFIELD SPECIFIC ABOVE */
	} else if(save_venyr(mtnrsrc,previous))
	{
		PURAVCIM_save_record(mtnrsrc,previous,update_list);
		add_vouchers(mtnrsrc);
		return(TRUE);
	} else return(FALSE);
}
void PURAVCIM_save_record(RDArsrc *mtnrsrc,RDATData *previous,short update_list)
{
	char *rcddesc=NULL,*vendorid1=NULL,*vendorid2=NULL;
	char *invid1=NULL,*invid2=NULL; /* ,*date=NULL,*timex=NULL; */
	char *temp=NULL,ca=FALSE;
	char deleteflag=0,mdeleteflag=0;
	short ef_vch=0;
	int yearid1=0,yearid2=0;
	int selected_addr=0;
	int venyrid1=0;
	char *venid=NULL;
	int noinv=0;
	RDATData *prev=NULL;

	readallwidgets(mtnrsrc);
	FINDRSCGETCHAR(mtnrsrc,"CLOSE PO",&ClosePO);
	rsrc2filerecord(invnum,mtnrsrc);
	FINDFLDSETCHAR(invnum,"DELETEFLAG",deleteflag);
	FINDFLDSETCHAR(invnum,"SOURCE USER VERIFIED",deleteflag);
	FINDRSCGETINT(mtnrsrc,"ADDRESS LIST",&selected_addr);
	FINDFLDSETSTRING(invnum,"ADDRESS IDENTIFICATION",addresslist->libs[selected_addr]);
	FINDFLDGETINT(invnum,"CALENDAR YEAR",&venyrid1);
	FINDFLDGETSTRING(invnum,"VENDOR IDENTIFICATION",&venid);
	if(ADVRECORDsec(invnum,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(invnum,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("SECURITY ACCESS DENIED!",rcddesc,NULL,FALSE); 
			Rfree(rcddesc);
		}
		prterr("Error: Didn't write record, Reason [%s].",(rcddesc!=NULL?rcddesc:""));
	} else {
		if(ADVEQLNRDsec(invnum,1,SCRNvirtualSubData,mtnrsrc)) 
		{
			FINDFLDSETSTRING(yearnum,"VENDOR IDENTIFICATION",venid);
			FINDFLDSETINT(yearnum,"CALENDAR YEAR",venyrid1);

			if(!ADVEQLNRDsec(yearnum,1,SCRNvirtualSubData,mtnrsrc)) 
			{
				prev=RDATDataNEW(yearnum);
				FINDFLDGETINT(yearnum,"NUMBER OF INVOICES ISSUED",&noinv);
				FINDFLDSETINT(yearnum,"NUMBER OF INVOICES ISSUED",(noinv+1));
			} else {
				FINDFLDSETINT(yearnum,"NUMBER OF INVOICES ISSUED",1);
			}
			WRTTRANS(yearnum,0,NULL,prev);
			if(prev!=NULL) FreeRDATData(prev);
			if(fyearnum!=(-1) && venpmt->use_fiscal_summary==TRUE)
			{
				FINDFLDSETSTRING(fyearnum,"VENDOR IDENTIFICATION",venid);
				FINDFLDSETINT(fyearnum,"FISCAL YEAR",yearid);

				if(!ADVEQLNRDsec(fyearnum,1,SCRNvirtualSubData,mtnrsrc)) 
				{
					prev=RDATDataNEW(fyearnum);
					FINDFLDGETINT(fyearnum,"NUMBER OF INVOICES ISSUED",&noinv);
					FINDFLDSETINT(fyearnum,"NUMBER OF INVOICES ISSUED",(noinv+1));
				} else {
					FINDFLDSETINT(fyearnum,"NUMBER OF INVOICES ISSUED",1);
				}
				WRTTRANS(fyearnum,0,NULL,prev);
				if(prev!=NULL) FreeRDATData(prev);
			}
		}
		rsrc2filerecord(invnum,mtnrsrc);
		FINDFLDSETCHAR(invnum,"DELETEFLAG",deleteflag);
		FINDFLDSETCHAR(invnum,"SOURCE USER VERIFIED",deleteflag);
		FINDRSCGETINT(mtnrsrc,"ADDRESS LIST",&selected_addr);
		FINDFLDSETSTRING(invnum,"ADDRESS IDENTIFICATION",addresslist->libs[selected_addr]);
		if(XPERT_SETUP->ARCHIVE==TRUE)
		{
			readwidget(mtnrsrc,"CAPTURE ARCHIVE");
			FINDRSCGETCHAR(mtnrsrc,"CAPTURE ARCHIVE",&ca);
			if(ca==TRUE)
			{
				readwidget(mtnrsrc,"ARCHIVE FILENAME");
				FINDRSCGETSTRING(mtnrsrc,"ARCHIVE FILENAME",&temp);
				FINDFLDSETSTRING(invnum,"FILENAME",temp);
				FINDFLDSETSTRING(invnum,"TYPE","PDF");
				if(temp!=NULL) Rfree(temp);
			}
		}
		WRTTRANS(invnum,0,NULL,previous);
		FINDFLDGETCHAR(invnum,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
		ZERNRD(infnum);
		COPYFIELD(invnum,infnum,"VENDOR IDENTIFICATION");
		COPYFIELD(invnum,infnum,"INVOICE IDENTIFICATION");
		if(ADVEQLNRDsec(infnum,1,SCRNvirtualSubData,mtnrsrc)) 
		{
			KEYNRD(infnum,1);
			prev=NULL;
		} else prev=RDATDataNEW(infnum);
		FINDFLDSETCHAR(infnum,"INVOICE IN USE",TRUE);
		FINDFLDSETCHAR(infnum,"DELETEFLAG",FALSE);
		FINDFLDSETSTRING(infnum,"INVOICE DATE",invdate);
		WRTTRANS(infnum,0,NULL,prev);
		if(prev!=NULL) FreeRDATData(prev);
		}
		if(deleteflag)
		{
			FINDFLDGETSTRING(invnum,"VENDOR IDENTIFICATION",&vendorid1);
			FINDFLDGETINT(invnum,"CALENDAR YEAR",&yearid1);
			FINDFLDGETSTRING(invnum,"INVOICE IDENTIFICATION",&invid1);
			ZERNRD(vchnum);
			FINDFLDSETSTRING(vchnum,"VENDOR IDENTIFICATION",vendorid1);
			FINDFLDSETINT(vchnum,"CALENDAR YEAR",yearid1);
			FINDFLDSETSTRING(vchnum,"INVOICE IDENTIFICATION",invid1);
			ef_vch=ADVGTENRDsec(vchnum,1,SCRNvirtualSubData,mtnrsrc);
			while(!ef_vch)
			{
				FINDFLDGETCHAR(vchnum,"DELETEFLAG",&mdeleteflag);
				if(!mdeleteflag)
				{
					FINDFLDGETSTRING(vchnum,"VENDOR IDENTIFICATION",&vendorid2);
					FINDFLDGETINT(vchnum,"CALENDAR YEAR",&yearid2);
					FINDFLDGETSTRING(vchnum,"INVOICE IDENTIFICATION",&invid2);
					if(!RDAstrcmp(vendorid2,vendorid1) && yearid2==yearid1 && 
						(!RDAstrcmp(invid2,invid1)))
					{
						prev=RDATDataNEW(vchnum);
						FINDFLDSETCHAR(vchnum,"DELETEFLAG",deleteflag);
						ADVWRTTRANSsec(vchnum,0,NULL,prev,SCRNvirtualSubData,mtnrsrc);		
						++PO_COUNT;
						if(prev!=NULL) FreeRDATData(prev);
					}
				}
				ef_vch=ADVNXTNRDsec(vchnum,1,SCRNvirtualSubData,mtnrsrc);
				if(vendorid2!=NULL) Rfree(vendorid2);
				if(invid2!=NULL) Rfree(invid2);
			}
		}
		if(vendorid1!=NULL) Rfree(vendorid1);
		if(vendorid2!=NULL) Rfree(vendorid2);
		if(invid1!=NULL) Rfree(invid1);
		if(invid2!=NULL) Rfree(invid2);
		updatebrowse(update_list,mbl_purinv,invnum,"PURORD",mtnrsrc);
		if(mbl_purinv!=NULL) quit_record(mtnrsrc,previous);
		else GetRDATData(invnum,previous);
	}
}
static void quit_record(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(previous!=NULL) FreeRDATData(previous);
	if(addresslist!=NULL) freeapplib(addresslist);
	if(fmonths!=NULL) freeapplib(fmonths);
	if(WindowCount==1)
	{
		if(invnum!=(-1)) CLSNRD(invnum);
		if(infnum!=(-1)) CLSNRD(infnum);
		if(yearnum!=(-1)) CLSNRD(yearnum);
		if(venpmt->use_fiscal_summary==TRUE)
		{
			if(fyearnum!=(-1)) CLSNRD(fyearnum);
		}
		if(vennum!=(-1)) CLSNRD(vennum);
		if(vchnum!=(-1)) CLSNRD(vchnum);
		if(venadrnum!=(-1)) CLSNRD(venadrnum);
		if(msrnum!=(-1)) CLSNRD(msrnum);
		if(sitenum!=(-1)) CLSNRD(sitenum);
		ShutdownSubsystems();
		exit(0);
	}
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
}
static void ClearInvoiceIdentification(RDArsrc *mtnrsrc,RDATData *previous)
{
	char *test=NULL;

	FINDRSCSETINPUTFOCUS(mtnrsrc,"[VENINV][INVOICE IDENTIFICATION]");
	FINDFLDSETSTRING(invnum,"INVOICE IDENTIFICATION",test);
	FINDRSCSETSTRING(mtnrsrc,"[VENINV][INVOICE IDENTIFICATION]",test);
	updatersrc(mtnrsrc,"[VENINV][INVOICE IDENTIFICATION]");
	getfilecb(mtnrsrc,previous);
}
static void getfilecb(RDArsrc *mtnrsrc,RDATData *previous)
{
	char deleteflag=FALSE;

	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]",&vendorid);
	FINDFLDSETSTRING(invnum,"VENDOR IDENTIFICATION",vendorid);
	FINDRSCGETINT(mtnrsrc,"[VENINV][CALENDAR YEAR]",&venyrid);
	FINDFLDSETINT(invnum,"CALENDAR YEAR",venyrid);
	FINDRSCGETSTRING(mtnrsrc,"[VENINV][INVOICE IDENTIFICATION]",&invid);
	FINDFLDSETSTRING(invnum,"INVOICE IDENTIFICATION",invid);
	getfile(mtnrsrc,previous);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
	ZERNRD(infnum);
	COPYFIELD(invnum,infnum,"VENDOR IDENTIFICATION");
	COPYFIELD(invnum,infnum,"INVOICE IDENTIFICATION");
	if(ADVEQLNRDsec(infnum,1,SCRNvirtualSubData,mtnrsrc))
	{
	} else {
		FINDFLDGETCHAR(infnum,"INVOICE COUNTED",&deleteflag);
		if(deleteflag)
		{
			ADVWARNINGDIALOG("WARNING DIALOG SCREEN","INVOICE COUNTED!","This invoice has already been used and counted.  Do you wish to continue?",NULL,ClearInvoiceIdentification,FALSE,2,mtnrsrc,previous,NULL);
		} else {
			FINDFLDGETCHAR(infnum,"INVOICE IN USE",&deleteflag);
			if(deleteflag)
			{
				ADVWARNINGDIALOG("WARNING DIALOG SCREEN","INVOICE COUNTED!","This invoice has already been used in the payment cycle.  Do you with to continue?",NULL,ClearInvoiceIdentification,FALSE,2,mtnrsrc,previous,NULL);
			}
		}
	}
}
static void getfile(RDArsrc *mtnrsrc,RDATData *previous)
{
	NRDfield *field=NULL;

	if(ADVEQLNRDsec(invnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(invnum,1);
		field=FLDNRD(invnum,"ENTRY DATE");
		if(field!=NULL)
		{
			if(field->len==8)
			{
				FINDFLDSETSTRING(invnum,"ENTRY DATE",CURRENT_DATE);
			} else FINDFLDSETSTRING(invnum,"ENTRY DATE",CURRENT_DATE10);
		}
		FINDFLDSETSTRING(invnum,"ENTRY TIME",CURRENT_TIME);
		FINDFLDSETSTRING(invnum,"SOURCE USER",USERLOGIN);
		ClearRDATData(previous);
		setaddrlist1(mtnrsrc);
	} else {
		GetRDATData(invnum,previous);
		filerecord2rsrc(invnum,mtnrsrc);	
		setaddrlist(mtnrsrc);
	}
	if(yearnum!=(-1))
	{
		COPYFIELD(invnum,yearnum,"VENDOR IDENTIFICATION");
		COPYFIELD(invnum,yearnum,"CALENDAR YEAR");
		if(ADVEQLNRDsec(yearnum,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(yearnum,1);
		} else {
			filerecord2rsrc(yearnum,mtnrsrc);
		}
	}
	if(fyearnum!=(-1))
	{
		COPYFIELD(invnum,fyearnum,"VENDOR IDENTIFICATION");
		FINDFLDSETINT(fyearnum,"FISCAL YEAR",yearid);
		if(ADVEQLNRDsec(fyearnum,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(fyearnum,1);
		} else {
			filerecord2rsrc(fyearnum,mtnrsrc);
		}
	}
	if(infnum!=(-1))
	{
		ZERNRD(infnum);
		COPYFIELD(invnum,infnum,"VENDOR IDENTIFICATION");
		COPYFIELD(invnum,infnum,"INVOICE IDENTIFICATION");
		if(ADVEQLNRDsec(infnum,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(infnum,1);
		} else {
			filerecord2rsrc(infnum,mtnrsrc);
		}
	}
	if(vennum!=(-1))
	{
		FINDFLDSETSTRING(vennum,"VENDOR IDENTIFICATION",vendorid);
		if(ADVEQLNRDsec(vennum,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(vennum,1);
		} else { 
			filerecord2rsrc(vennum,mtnrsrc);
			FINDFLDGETCHAR(vennum,"DEFAULT 1099 PAYMENT",&pay_1099);
			FINDRSCSETCHAR(mtnrsrc,"1099 PAYMENT",pay_1099);
		}
	}
	if(msrnum!=(-1))
	{
		FINDFLDSETSTRING(msrnum,"PO IDENTIFICATION",poid);
		FINDFLDSETINT(msrnum,"FISCAL YEAR",yearid);
		if(ADVEQLNRDsec(msrnum,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(yearnum,1);
		} else {
			filerecord2rsrc(msrnum,mtnrsrc);
		}
	}
	if(sitenum!=(-1))
	{
		COPYFIELD(msrnum,sitenum,"SITE IDENTIFICATION");
		if(ADVEQLNRDsec(sitenum,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(sitenum,1);
		} else {
			filerecord2rsrc(sitenum,mtnrsrc);
		}
	}
	if(XPERT_SETUP->ARCHIVE==TRUE)
	{
		setca(mtnrsrc);
	}
}
static void PURAVCIM_resetdefs(RDArsrc *mtnrsrc,RDATData *previous)
{
	char *vendorid=NULL,*invid=NULL,*poid=NULL;
	int venyrid=0,pofyid=0;

	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]",&vendorid);
	FINDFLDSETSTRING(invnum,"VENDOR IDENTIFICATION",vendorid);
	FINDRSCGETINT(mtnrsrc,"[VENINV][CALENDAR YEAR]",&venyrid);
	FINDFLDSETINT(invnum,"CALENDAR YEAR",venyrid);
	FINDRSCGETSTRING(mtnrsrc,"[VENINV][INVOICE IDENTIFICATION]",&invid);
	FINDFLDSETSTRING(invnum,"INVOICE IDENTIFICATION",invid);
	FINDRSCGETSTRING(mtnrsrc,"[PURMST][PO IDENTIFICATION]",&poid);
	FINDFLDSETSTRING(msrnum,"PO IDENTIFICATION",poid);
	FINDRSCGETINT(mtnrsrc,"[PURMST][FISCAL YEAR]",&pofyid);
	FINDFLDSETINT(msrnum,"FISCAL YEAR",pofyid);
	DefaultResetScreen(mtnrsrc,TRUE);
	FINDRSCSETSTRING(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]",vendorid);
	FINDRSCSETINT(mtnrsrc,"[VENINV][CALENDAR YEAR]",venyrid);
	FINDRSCSETSTRING(mtnrsrc,"[VENINV][INVOICE IDENTIFICATION]",invid);
	FINDRSCSETSTRING(mtnrsrc,"[PURMST][PO IDENTIFICATION]",poid);
	FINDRSCSETINT(mtnrsrc,"[PURMST][FISCAL YEAR]",pofyid);
	getfile(mtnrsrc,previous);
	if(vendorid!=NULL) Rfree(vendorid);
	if(invid!=NULL) Rfree(invid);
	if(poid!=NULL) Rfree(poid);
	updateallrsrc(mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
/*
	readwidget(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]");
	readwidget(mtnrsrc,"[VENINV][CALENDAR YEAR]");
	FINDRSCGETSTRING(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]",&venid);
	FINDRSCGETINT(mtnrsrc,"[VENINV][CALENDAR YEAR]",&calyr);
	ZERNRD(VENINV_MASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	FINDRSCSETSTRING(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]",venid);
	FINDRSCSETINT(mtnrsrc,"[VENINV][CALENDAR YEAR]",calyr);
	if(venid!=NULL) Rfree(venid);
	updateallrsrc(mtnrsrc);
*/
}
