/* reqaddi.c - Add Requisition Items Multiple */
#include <cstdio>

#include <mix.hpp>
#include <reqmgt.hpp>

extern short reqitm,reqmst,reqloc,finyrnum,reqcat,vennum,eyrnum,reqacc;
extern short byrnum,reqctlg,reqven,reqpven;
extern int REQ_COUNT;
extern char *module,*locid,*reqid;
extern RDArsrc *definelist;
extern RDArsrc *defineacclst;
extern APPlib *addresslist,*prevvch,*categories,*pmonth,*yearavl;
APPlib *acclst=NULL;
extern RDAfinmgt *finsetup;
char changedvalues=FALSE;
static int getyears(RDArsrc *,RDAfinmgt *);
static void setyear(RDArsrc *);
static void changeyear(RDArsrc *,short);
static void changeyearcb(RDArsrc *);
static void set_defaults(RDArsrc *);
extern void savelist(RDArsrc *,RDArsrc *,short),savelistcb(RDArsrc *,RDArsrc *);
static void saveacclst(RDArsrc *,RDArsrc *),saveacclstcb(RDArsrc *,RDArsrc *);
extern void definelistfunc(RDArsrc *,RDArsrc *),makedflist(void);
static void defineacclstfunc(RDArsrc *,RDArsrc *);
static void save_record(RDArsrc *),quit_record(RDArsrc *);
static void save_check(RDArsrc *),quit_check(RDArsrc *);
extern void getsupportingfiles(RDArsrc *);
static void mbrowse_items(RDArsrc *);
static void mbrowse_vendors(RDArsrc *);
extern void changecategory(RDArsrc *);
extern void checkvendorcb(RDArsrc *),browse_venmsr(RDArsrc *);
extern short checkvendor(RDArsrc *,short);
extern int year_identification;
REQACC *ACCOUNTS=NULL;
extern void makeaccountlist(void);
extern void mkacclst(ReqAcctStruct *);
extern void edit_account(RDArsrc *);
extern void delete_account(RDArsrc *);
extern void add_account_below(RDArsrc *);
extern void add_account_above(RDArsrc *);
static void multicaltotalqty(RDArsrc *);
static void multicaltotalur(RDArsrc *);
extern void quititem(MakeBrowseList *);
extern void inititem(MakeBrowseList *),initreqven(MakeBrowseList *);
extern short selectitem(MakeBrowseList *);
extern short selectreqven(MakeBrowseList *);
static void mokreqvenfunc(MakeBrowseList *,void *),mbrowse_catalog(RDArsrc *);
static void mokitemfunc(MakeBrowseList *,void *);
extern void mk_define_account_list(RDArsrc *);
extern double total_req;
extern void setaddresses(RDArsrc *);
extern void changeADDRlist(RDArsrc *);

void defineacclstfunc(RDArsrc *parent,RDArsrc *defineacclst)
{
	APPmakescrn(defineacclst,TRUE,NULL,NULL,FALSE);
}
void saveacclstcb(RDArsrc *defineacclst,RDArsrc *parent)
{
	saveacclst(defineacclst,parent);
	if(IsScreenDisplayed(defineacclst)) NULL_RSCS(defineacclst);
}
void saveacclst(RDArsrc *defineacclst,RDArsrc *parent)
{
	int x=0;
	RDArsrc *tmprsrc=NULL;
	ReqAcctStruct *fields=NULL;

	if(IsScreenDisplayed(defineacclst)) readallwidgets(defineacclst);
	if(acclst!=NULL) freeapplib(acclst);
	acclst=APPlibNEW();
#ifdef DIAGNOSTIC_SCREENS
	tmprsrc=odiagscrn("DIAGNOSTIC SCREEN",module,
		"Reading User Multiple Accounts Defaults",NULL,ACCOUNTS->num);
#else 
	tmprsrc=NULL;
#endif
	if(tmprsrc!=NULL)
	{
		if(ADVmakescrn(tmprsrc,TRUE))
		{
			prterr("Error Cannot Create Diagnostic Screen.");
		} else {
			ForceWindowUpdate(tmprsrc);
		}
	}
	if(ACCOUNTS->a!=NULL)
	{
		for(x=0,fields=ACCOUNTS->a;x<ACCOUNTS->num;++x,++fields)
		{
			mkacclst(fields);
			if(tmprsrc!=NULL) 
				update_diagnostic(tmprsrc,TRUE);
		}
	}
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
        if(acclst->numlibs<1)
        {
                addAPPlib(acclst,"No Accounts Defined");
        }
	if(defineacclst!=NULL)
	{
		if(!FINDRSCLISTAPPlib(parent,"ACCOUNT LIST",(acclst->numlibs-1),
			acclst))
		{
			updatersrc(parent,"ACCOUNT LIST");
		}
	}
}
static void multicaltotalqty(RDArsrc *parent)
{
	int quantity=0,selcat=0,value=0;
	double unitrate=0.0,total=0.0,percdisc=0.0;
	char *catid1=NULL,*venid1=NULL,*itemid1=NULL;
	char *catid2=NULL,*venid2=NULL,*itemid2=NULL;

	if(FINDRSCGETINT(parent,"ACCOUNT LIST",&value)) return;
	FINDRSCGETINT(parent,"CATEGORY IDENTIFICATIONS",&selcat);
	if(RDAstrcmp(categories->libs[selcat],"No Categories"))
	{
		catid1=stralloc(categories->libs[selcat]);
	}
	readwidget(parent,"[REQITM][VENDOR IDENTIFICATION]");
	FINDRSCGETSTRING(parent,"[REQITM][VENDOR IDENTIFICATION]",&venid1);
	readwidget(parent,"[REQITM][ITEM IDENTIFICATION]");
	FINDRSCGETSTRING(parent,"[REQITM][ITEM IDENTIFICATION]",&itemid1);
	readwidget(parent,"[REQITM][QUANTITY]");
	if(FINDRSCGETINT(parent,"[REQITM][QUANTITY]",&quantity)) return;
	readwidget(parent,"[REQITM][PERCENTAGE DISCOUNT]");
	if(FINDRSCGETDOUBLE(parent,"[REQITM][PERCENTAGE DISCOUNT]",&percdisc)) return;
	ZERNRD(reqpven);
	FINDFLDSETSTRING(reqpven,"CATEGORY IDENTIFICATION",catid1);
	FINDFLDSETSTRING(reqpven,"ITEM IDENTIFICATION",itemid1);
	FINDFLDSETSTRING(reqpven,"VENDOR IDENTIFICATION",venid1);
	FINDFLDSETINT(reqpven,"QUANTITY",quantity);
	if(!ADVLTENRDsec(reqpven,1,SCRNvirtualSubData,parent))
	{
		FINDFLDGETSTRING(reqpven,"VENDOR IDENTIFICATION",&venid2);
		FINDFLDGETSTRING(reqpven,"ITEM IDENTIFICATION",&itemid2);
		FINDFLDGETSTRING(reqpven,"CATEGORY IDENTIFICATION",&catid2);
		if(RDAstrcmp(venid1,venid2) || RDAstrcmp(itemid1,itemid2) || RDAstrcmp(catid1,catid2))
		{
			readwidget(parent,"[REQITM][UNIT RATE]");
			FINDRSCGETDOUBLE(parent,"[REQITM][UNIT RATE]",&unitrate);
			readwidget(parent,"[REQITM][PERCENTAGE DISCOUNT]");
			FINDRSCGETDOUBLE(parent,"[REQITM][PERCENTAGE DISCOUNT]",&percdisc);
		} else {
			FINDFLDGETDOUBLE(reqpven,"UNIT RATE",&unitrate);
			FINDFLDGETDOUBLE(reqpven,"PERCENTAGE DISCOUNT",&percdisc);
		}
	} else {
		readwidget(parent,"[REQITM][UNIT RATE]");
		FINDRSCGETDOUBLE(parent,"[REQITM][UNIT RATE]",&unitrate);
		readwidget(parent,"[REQITM][PERCENTAGE DISCOUNT]");
		FINDRSCGETDOUBLE(parent,"[REQITM][PERCENTAGE DISCOUNT]",&percdisc);
	}
	FINDRSCSETDOUBLE(parent,"[REQITM][UNIT RATE]",unitrate);
	updatersrc(parent,"[REQITM][UNIT RATE]");
	FINDRSCSETDOUBLE(parent,"[REQITM][PERCENTAGE DISCOUNT]",percdisc);
	updatersrc(parent,"[REQITM][PERCENTAGE DISCOUNT]");
	if(percdisc!=0.0)
	{
		total=round(((unitrate*quantity)*100.0)-(((unitrate*quantity)*100.0)*percdisc/100));
	} else {
		total=round(((unitrate*quantity)*100.0));
	}
	FINDRSCSETDOUBLE(parent,"[REQITM][AMOUNT]",total);
	updatersrc(parent,"[REQITM][AMOUNT]");
	ComputeREQACC(ACCOUNTS,total);
	makeaccountlist();
	if(!FINDRSCLISTAPPlib(parent,"ACCOUNT LIST",value,acclst))
		updatersrc(parent,"ACCOUNT LIST");
	if(catid1!=NULL) Rfree(catid1);
	if(catid2!=NULL) Rfree(catid2);
	if(venid1!=NULL) Rfree(venid1);
	if(venid2!=NULL) Rfree(venid2);
	if(itemid1!=NULL) Rfree(itemid1);
	if(itemid2!=NULL) Rfree(itemid2);
	updateSCRNvirtuals(parent);
}
static void multicaltotalur(RDArsrc *parent)
{
	int quantity=0,value=0;
	double unitrate=0.0,total=0.0,percdisc=0.0;

	if(FINDRSCGETINT(parent,"ACCOUNT LIST",&value)) return;
	readwidget(parent,"[REQITM][QUANTITY]");
	FINDRSCGETINT(parent,"[REQITM][QUANTITY]",&quantity);
	readwidget(parent,"[REQITM][UNIT RATE]");
	FINDRSCGETDOUBLE(parent,"[REQITM][UNIT RATE]",&unitrate);
	readwidget(parent,"[REQITM][PERCENTAGE DISCOUNT]");
	FINDRSCGETDOUBLE(parent,"[REQITM][PERCENTAGE DISCOUNT]",&percdisc);
	if(percdisc!=0.0)
	{
		total=round(((unitrate*quantity)*100.0)-(((unitrate*quantity)*100.0)*percdisc/100));
	} else {
		total=round(((unitrate*quantity)*100.0));
	}
	FINDRSCSETDOUBLE(parent,"[REQITM][AMOUNT]",total);
	updatersrc(parent,"[REQITM][AMOUNT]");
	ComputeREQACC(ACCOUNTS,total);
	makeaccountlist();
	if(!FINDRSCLISTAPPlib(parent,"ACCOUNT LIST",value,acclst))
		updatersrc(parent,"ACCOUNT LIST");
	updateSCRNvirtuals(parent);
}
static void multitotal(RDArsrc *parent)
{
	int value=0;
	double amt=0.0;

	if(FINDRSCGETINT(parent,"ACCOUNT LIST",&value)) return;
	readwidget(parent,"[REQITM][AMOUNT]");
	FINDRSCGETDOUBLE(parent,"[REQITM][AMOUNT]",&amt);
	ComputeREQACC(ACCOUNTS,amt);
	makeaccountlist();
	if(!FINDRSCLISTAPPlib(parent,"ACCOUNT LIST",value,acclst))
		updatersrc(parent,"ACCOUNT LIST");
}
static void reqaddiSaveDefaults(RDArsrc *parent)
{
	SaveDefaults(parent);
	writeAddReqDefAcct(ACCOUNTS);
}
static void set_defaults(RDArsrc *parent)
{
	int selprev=0;
	char *venid=NULL,*date=NULL,*timex=NULL;
	double total=0;

	FINDRSCGETINT(parent,"PREVIOUS ITEM'S",&selprev);
	FINDRSCGETDOUBLE(parent,"TOTAL REQUISITION",&total);
	DefaultResetScreen(parent,TRUE);
	if(ACCOUNTS!=NULL) FreeREQACC(ACCOUNTS);
	ACCOUNTS=REQACCNEW();
	getAddReqDefAcct(ACCOUNTS);
	makeaccountlist();
	FINDRSCLISTAPPlib(parent,"ACCOUNT LIST",0,acclst);
	date=GETCURRENTDATE10();
	timex=GETCURRENTTIME();
	FINDRSCSETSTRING(parent,"[REQITM][ENTRY DATE]",date);
	FINDRSCSETSTRING(parent,"[REQITM][ENTRY TIME]",timex);
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
	FINDRSCSETSTRING(parent,"[REQITM][LOCATION IDENTIFICATION]",locid);
	FINDRSCSETSTRING(parent,"[REQITM][REQUISITION IDENTIFICATION]",reqid);
	FINDRSCSETINT(parent,"PREVIOUS ITEM'S",selprev);
	FINDRSCSETDOUBLE(parent,"TOTAL REQUISITION",total);
	getsupportingfiles(parent);
	changecategory(parent);
	changeyear(parent,TRUE);
	readwidget(parent,"[REQITM][VENDOR IDENTIFICATION]");
	FINDRSCGETSTRING(parent,"[REQITM][VENDOR IDENTIFICATION]",&venid);
	FINDFLDSETSTRING(reqitm,"VENDOR IDENTIFICATION",venid);
	if(venid!=NULL) Rfree(venid);
	checkvendor(parent,FALSE);
	computeallSCRNvirtuals(parent);
	updateallrsrc(parent);
}
static void save_check(RDArsrc *parent)
{
	int selmonth=0,selyear=0,procyrid=0,selcat=0,seladdr=0;
	char *header=NULL;
	double amount=0.0;

	readallwidgets(parent);
	if(FINDRSCGETINT(parent,"ADDRESS IDENTIFICATIONS",&seladdr)) return;
	if(seladdr>=addresslist->numlibs)
	{
		header=stralloc("This Requisition Item doesn't contain a valid Vendor Address Identification, please select a valid Address Identification or set them up for this Vendor.");
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID VENDOR ADDRESS IDENTIFICATION!",header,NULL,NULL,TRUE,NULL);
		if(header!=NULL) Rfree(header);
		return;
	}
	if(!RDAstrcmp(addresslist->libs[seladdr],"No Addresses Defined."))
	{
		header=stralloc("This Requisition Item doesn't contain a valid Vendor Address Identification, please select a valid Address Identification or set them up for this Vendor.");
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID VENDOR ADDRESS IDENTIFICATION!",header,NULL,NULL,TRUE,NULL);
		if(header!=NULL) Rfree(header);
		return;
	}
	FINDFLDSETSTRING(reqitm,"ADDRESS IDENTIFICATION",addresslist->libs[seladdr]);
	if(FINDRSCGETINT(parent,"FISCAL YEARS",&selyear)) return;
	if(FINDRSCGETINT(parent,"PROCESSING MONTHS",&selmonth)) return;
	if(FINDRSCGETINT(parent,"CATEGORY IDENTIFICATIONS",&selcat)) return;
	procyrid=Ratoi(yearavl->libs[selyear]);
	if(FINDRSCGETDOUBLE(parent,"[REQITM][AMOUNT]",&amount)) return;
	if(amount==0.0) 
	{
		ERRORDIALOG("INVALID AMOUNT","This Requisition Item can not be saved with an AMOUNT of [$  0.00].  Please enter the correct amount for this item.",NULL,FALSE);
		prterr("Attemped to save a requisition item with a zero dollar amount");
		return;
	}
	if(checkvendor(parent,TRUE)==FALSE) return;
	ComputeREQACC(ACCOUNTS,amount);
	if(!ADVCHECKREQACC(ACCOUNTS,eyrnum,procyrid,selmonth,finsetup,SCRNvirtualSubData,parent)) return;
	save_record(parent);
}
static void save_record(RDArsrc *parent)
{
	int selcat=0,selmonth=0,selyear=0,seladdr=0,procyrid=0;
	char *rcddesc=NULL,*errordesc=NULL,deleteflag=FALSE;
	short x=0;
	int lf=0,itemnumber=0;
	char *date=NULL,*timex=NULL,dflag=FALSE;
	double amount=0.0,zer=0.0;
	RDArmem *member=NULL;
	ReqAcctStruct *fields=NULL;

        if(FINDRSCGETINT(parent,"ADDRESS IDENTIFICATIONS",&seladdr)) return;
	if(FINDRSCGETINT(parent,"PROCESSING MONTHS",&selmonth)) return;
	if(FINDRSCGETINT(parent,"FISCAL YEARS",&selyear)) return;
	if(FINDRSCGETDOUBLE(parent,"[REQITM][AMOUNT]",&amount)) return;
	if(FINDRSCGETINT(parent,"CATEGORY IDENTIFICATIONS",&selcat)) return;
	rsrc2filerecord(reqitm,parent);
        FINDFLDSETSTRING(reqitm,"ADDRESS IDENTIFICATION",addresslist->libs[seladdr]);
	procyrid=Ratoi(yearavl->libs[selyear]);
	FINDFLDSETDOUBLE(reqitm,"AMOUNT",amount);
	x=(short)selmonth;
	FINDFLDSETSHORT(reqitm,"PROCESSING MONTH",x);
	FINDFLDSETINT(reqitm,"FISCAL YEAR",procyrid);
	if(RDAstrcmp(categories->libs[selcat],"No Categories"))
	{
		FINDFLDSETSTRING(reqitm,"CATEGORY IDENTIFICATION",
			categories->libs[selcat]);
	} else FINDFLDSETSTRING(reqitm,"CATEGORY IDENTIFICATION",NULL);
	FINDFLDSETSTRING(reqitm,"SOURCE USER",USERLOGIN);
	FINDFLDSETCHAR(reqitm,"DELETEFLAG",deleteflag);
	FINDFLDSETCHAR(reqitm,"SOURCE USER VERIFIED",deleteflag);
	date=GETCURRENTDATE10();
	timex=GETCURRENTTIME();
	if((x=FINDRSC(parent,"[REQITM][ENTRY DATE]"))!=(-1))
	{
		member=parent->rscs+x;
		if(!IsWidgetDisplayed(member) || isEMPTY(member->value.string_value))
		{
			lf=RDAstrlen(date);
			if(date!=NULL && lf>0)
			{
				QUICKALLOC(member->value.string_value,member->dlen,lf);
				memcpy(member->value.string_value,date,lf+1);
			} else if(member->dlen>0)
			{
				memset(member->value.string_value,0,member->dlen+1);
			} else {
				member->value.string_value=NULL;
				member->dlen=0;
			}
			FINDFLDSETSTRING(reqitm,"ENTRY DATE",date);
		}
	} else {
		FINDFLDSETSTRING(reqitm,"ENTRY DATE",date);
	}
	if(date!=NULL) Rfree(date);
	if((x=FINDRSC(parent,"[REQITM][ENTRY TIME]"))!=(-1))
	{
		member=parent->rscs+x;
		if(!IsWidgetDisplayed(member) || isEMPTY(member->value.string_value))
		{
			lf=RDAstrlen(timex);
			if(timex!=NULL && lf>0)
			{
				QUICKALLOC(member->value.string_value,member->dlen,lf);
				memcpy(member->value.string_value,timex,lf+1);
			} else if(member->dlen>0)
			{
				memset(member->value.string_value,0,member->dlen+1);
			} else {
				member->value.string_value=NULL;
				member->dlen=0;
			}
			FINDFLDSETSTRING(reqitm,"ENTRY TIME",timex);
		}
	} else {
		FINDFLDSETSTRING(reqitm,"ENTRY TIME",timex);
	}
	if(timex!=NULL) Rfree(timex);
	FIELDCOPY(reqitm,"VENDOR IDENTIFICATION",reqitm,"REQUESTED VENDOR IDENTIFICATION");
	FIELDCOPY(reqitm,"ITEM IDENTIFICATION",reqitm,"REQUESTED ITEM IDENTIFICATION");
	FIELDCOPY(reqitm,"VENDOR ITEM NUMBER",reqitm,"REQUESTED VENDOR ITEM NUMBER");
	if(ADVRECORDsec(reqitm,SCRNvirtualSubData,parent))
	{
		rcddesc=ADVRECORDsecDesc(reqitm,SCRNvirtualSubData,parent);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
			Rfree(rcddesc);
			prterr("Error didn't write record: [%s]",rcddesc);
		} else {
			prterr("Error didn't write record!  ADVRECORDsec() failed without a description returned!");
		}
	} else {
		if(!WRTTRANS(reqitm,TRUE,"ITEM NUMBER",NULL))
		{
			total_req+=amount;
			FINDRSCSETDOUBLE(parent,"TOTAL REQUISITION",total_req);
			updatersrc(parent,"TOTAL REQUISITION");
			FINDFLDGETINT(reqitm,"ITEM NUMBER",&itemnumber);
			if(ACCOUNTS->a!=NULL)
			{
				for(x=0,fields=ACCOUNTS->a;x<ACCOUNTS->num;++x,++fields)
				{
					COPYFIELD(reqitm,reqacc,"LOCATION IDENTIFICATION");
					COPYFIELD(reqitm,reqacc,"REQUISITION IDENTIFICATION");
					FINDFLDSETINT(reqacc,"ITEM NUMBER",itemnumber);
					FINDFLDSETSTRING(reqacc,"ACCOUNT CODE",fields->account);
					FINDFLDSETSTRING(reqacc,"DEBIT CODE",fields->debit);
					FINDFLDSETSTRING(reqacc,"CREDIT CODE",fields->credit);
					FINDFLDSETCHAR(reqacc,"PRORATE",fields->prorate);
					if(fields->prorate)
					{
						FINDFLDSETDOUBLE(reqacc,"PERCENTAGE",
							fields->percentage);
						FINDFLDSETDOUBLE(reqacc,"AMOUNT",zer);
					} else {
						FINDFLDSETDOUBLE(reqacc,"AMOUNT",fields->amount);
						FINDFLDSETDOUBLE(reqacc,"PERCENTAGE",zer);
					}
					FINDFLDSETSTRING(reqacc,"DEFAULT DEBIT CODE",fields->defdebit);
					FINDFLDSETSTRING(reqacc,"DEFAULT CREDIT CODE",fields->defcredit);
					FINDFLDSETSTRING(reqacc,"DEFAULT PAYABLE CODE",fields->defpayable);
					FINDFLDSETSTRING(reqacc,"DEFAULT BANK IDENTIFICATION",fields->defbankid);
					FINDFLDSETCHAR(reqacc,"DELETEFLAG",dflag);
					ADVWRTTRANSsec(reqacc,TRUE,"ACCOUNT NUMBER",NULL,SCRNvirtualSubData,parent);
				}
			}
			ADVREQACC_UPDATE_UNDIST(ACCOUNTS,procyrid,selmonth,eyrnum,byrnum,SCRNvirtualSubData,parent);
			++REQ_COUNT;
			makedflist();
		} else {
			if(errordesc!=NULL) Rfree(errordesc);
			errordesc=Rmalloc(RDAstrlen(MODULENAME(reqitm))+RDAstrlen(FILENAME(reqitm))+139);
			sprintf(errordesc,"The write function failed to the file [%s][%s], the record and all subfiles that you attempted to save were not created.  Please try again.",MODULENAME(reqitm),FILENAME(reqitm));
			ERRORDIALOG("WRITE ERROR OCCURRED!",errordesc,NULL,FALSE);
			prterr("Error Couldn't Write Record: [%s]",errordesc);
			if(errordesc!=NULL) Rfree(errordesc);
		}
	}
	if(!FINDRSCLISTAPPlib(parent,"PREVIOUS ITEM'S",(prevvch->numlibs-1),
		prevvch))
	{
		updatersrc(parent,"PREVIOUS ITEM'S");
	}
}
static void quit_check(RDArsrc *parent)
{
	quit_record(parent);
}
static void quit_record(RDArsrc *parent)
{
	if(definelist!=NULL) 
	{
		killwindow(definelist);
		free_rsrc(definelist); 
	}
	if(parent!=NULL)
	{
		killwindow(parent);
		free_rsrc(parent);
	}
	if(pmonth!=NULL) freeapplib(pmonth);
	if(addresslist!=NULL) freeapplib(addresslist);
	if(prevvch!=NULL) freeapplib(prevvch);
	if(acclst!=NULL) freeapplib(acclst);
	if(categories!=NULL) freeapplib(categories);
	FreeREQACC(ACCOUNTS);
}
static void setyear(RDArsrc *parent)
{
	int procyearid=0,yearno=0;

	ZERNRD(finyrnum);
	if(FINDRSCGETINT(parent,"FISCAL YEARS",&yearno)) return;
	procyearid=Ratoi(yearavl->libs[yearno]);
	FINDFLDSETINT(finyrnum,"FISCAL YEAR",procyearid);
	FINDFLDSETINT(reqitm,"FISCAL YEAR",procyearid);
	if(ADVEQLNRDsec(finyrnum,1,SCRNvirtualSubData,parent)) KEYNRD(finyrnum,1);
	updatefilerecord2rsrc(finyrnum,parent);
	updateSCRNvirtuals(parent);
}
static void changeyearcb(RDArsrc *parent)
{
	changeyear(parent,FALSE);
}
static void changeyear(RDArsrc *parent,short force)
{
	int selectedyear=0,procyrid=0;

	FINDRSCGETINT(parent,"FISCAL YEARS",&selectedyear);
	if(selectedyear>=yearavl->numlibs)
	{
		selectedyear=0;
		FINDRSCSETINT(parent,"FISCAL YEARS",selectedyear);
	}
	procyrid=Ratoi(yearavl->libs[selectedyear]);
	if((procyrid!=year_identification) || force)
	{
		setyear(parent);
		updateSCRNvirtuals(parent);
	}
}
static int getyears(RDArsrc *parent,RDAfinmgt *finsetup)
{
	short ef;
	int selyear=0,curryear=0,year=0;
	char delflag=FALSE;

	curryear=GETCURRENTFISCALYEAR(finsetup->month);
	if(yearavl!=NULL) freeapplib(yearavl);
	yearavl=APPlibNEW();
	ZERNRD(finyrnum);
	ef=ADVFRSNRDsec(finyrnum,1,SCRNvirtualSubData,parent);
	while(!ef)
	{
		FINDFLDGETCHAR(finyrnum,"DELETEFLAG",&delflag);
		if(!delflag)
		{
		FINDFLDGETCHAR(finyrnum,"ACTIVE",&delflag);
		if(delflag)
		{
			FINDFLDGETINT(finyrnum,"FISCAL YEAR",&year);
			sprintf(stemp,"%-4d",year);
			addAPPlib(yearavl,stemp);
			if(year==curryear) selyear=((yearavl->numlibs)-1);
		}
		}
		ef=ADVNXTNRDsec(finyrnum,1,SCRNvirtualSubData,parent);
	}
	return(selyear);
}
void additemsmultiple(void)
{
	short nofields=0,nokeys=0,x,edit_rsrc=TRUE;
	int y,start=0,presentyear=0,which_type=0;
	RDArsrc *parent=NULL;
	NRDfield *fields=NULL,*fldx;
	NRDkey *keys=NULL;
	char *name=NULL,*date=NULL,*timex=NULL,catalogvendor=TRUE;

	nokeys=NUMKEYS(reqitm);
	keys=KEYPOINTER(reqitm);
	nofields=NUMFLDS(reqitm);
	fields=FLDPOINTER(reqitm);
	parent=RDArsrcNEW(module,"ADD REQUISITION ITEMS MULTIPLE");
	definelist=RDArsrcNEW(module,"ADD REQUISITION ITEMS DEFINE LIST");
	defineacclst=RDArsrcNEW(module,"ADD REQUISITION ITEMS DEFINE ACCOUNT LIST");
	addDFincvir(parent,"REQMGT","REQLOC",NULL,reqloc);
	addDFincvir(parent,"REQMGT","REQITM",NULL,reqitm);
	addDFincvir(parent,"REQMGT","REQMST",NULL,reqmst);
	addDFincvir(parent,"REQMGT","REQACC",NULL,reqacc);
	addDFincvir(parent,"CATALOGUE","CATMST",NULL,reqcat);
	addDFincvir(parent,"FINMGT","FINYEAR",NULL,finyrnum);
	addDFincvir(definelist,"REQMGT","REQITM",NULL,reqitm);
	GET_SCREEN_VIRTUALS(parent,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	mk_define_list(definelist,nofields,fields,nokeys,keys,NULL,1);
	mk_define_account_list(defineacclst);
	FINDRSCSETFUNC(definelist,"SELECT",savelistcb,parent);
	FINDRSCSETFUNC(defineacclst,"SELECT",saveacclstcb,parent);
	if(reqmst!=(-1)) file2rsrc(reqmst,parent,FALSE);
	if(reqloc!=(-1)) file2rsrc(reqloc,parent,FALSE);
	if(reqcat!=(-1)) file2rsrc(reqcat,parent,FALSE);
	if(vennum!=(-1)) file2rsrc(vennum,parent,FALSE);
	getsupportingfiles(parent);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"LOCATION IDENTIFICATION"))
			{
				edit_rsrc=FALSE;
				if(FIELDscrn(reqitm,fldx->name,&edit_rsrc))
				{
					if(name!=NULL) 
					{ 
						name=Rrealloc(name,RDAstrlen(fldx->name)+5+
							RDAstrlen(FILENAME(reqitm)));
					} else {
						name=Rmalloc(RDAstrlen(fldx->name)+5+
							RDAstrlen(FILENAME(reqitm)));
					}
					sprintf(name,"[%s][%s]",FILENAME(reqitm),fldx->name);
					makefieldrsrc(parent,name,fldx->type,
						fldx->len,FALSE);
				}
			} else if(!RDAstrcmp(fldx->name,"REQUISITION IDENTIFICATION"))
			{
				edit_rsrc=FALSE;
				if(FIELDscrn(reqitm,fldx->name,&edit_rsrc))
				{
					if(name!=NULL) 
					{ 
						name=Rrealloc(name,RDAstrlen(fldx->name)+5+
							RDAstrlen(FILENAME(reqitm)));
					} else {
						name=Rmalloc(RDAstrlen(fldx->name)+5+
							RDAstrlen(FILENAME(reqitm)));
					}
					sprintf(name,"[%s][%s]",FILENAME(reqitm),fldx->name);
					makefieldrsrc(parent,name,fldx->type,
						fldx->len,FALSE);
				}
			} else if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER VERIFIED"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER"))
			{
			} else if(!RDAstrcmp(fldx->name,"CATEGORY IDENTIFICATION"))
			{
				which_type=0;
				if(categories!=NULL) freeapplib(categories);
				ZERNRD(reqcat);
				categories=ADVMakeNRDlistAllSTRING(reqcat,module,
					"Creating Category List",1,"CATEGORY IDENTIFICATION",
					SCRNvirtualSubData,parent);
				if(categories->numlibs<1)
				{
					addAPPlib(categories,"No Categories");
				}
				addlstrsrc(parent,"CATEGORY IDENTIFICATIONS",
					&which_type,(edit_rsrc==TRUE?TRUE:FALSE),
					changecategory,categories->numlibs,
					&categories->libs,NULL);
			} else if(!RDAstrcmp(fldx->name,"FISCAL YEAR"))
			{
				presentyear=getyears(parent,finsetup);
				if(yearavl->numlibs<1)
				{
					ERRORDIALOG("FINYEAR Empty File","The FINYEAR file is an empty file.  There must exist at least 1 year on file to process requisitions.\n",NULL,TRUE);
					ShutdownSecurity();
					RDAAPPMAINLOOP();
					quit_record(parent);
					return;
				}
				edit_rsrc=TRUE;
				if(FIELDscrn(reqloc,fldx->name,&edit_rsrc))
				{
					file2rsrc(finyrnum,parent,FALSE);
					addlstrsrc(parent,"FISCAL YEARS",
						&presentyear,edit_rsrc,changeyearcb,
						yearavl->numlibs,
						&yearavl->libs,NULL);
				}
			} else if(!RDAstrcmp(fldx->name,"PROCESSING MONTH"))
			{
				pmonth=APPlibNEW();
				addAPPlib(pmonth,"JANUARY");
				addAPPlib(pmonth,"FEBRUARY");
				addAPPlib(pmonth,"MARCH");
				addAPPlib(pmonth,"APRIL");
				addAPPlib(pmonth,"MAY");
				addAPPlib(pmonth,"JUNE");
				addAPPlib(pmonth,"JULY");
				addAPPlib(pmonth,"AUGUST");
				addAPPlib(pmonth,"SEPTEMBER");
				addAPPlib(pmonth,"OCTOBER");
				addAPPlib(pmonth,"NOVEMBER");
				addAPPlib(pmonth,"DECEMBER");
				edit_rsrc=TRUE;
				if(FIELDscrn(reqloc,fldx->name,&edit_rsrc))
				{
					start=(val(CURRENT_DATE,2)-1);
					addlstrsrc(parent,"PROCESSING MONTHS",
						&start,edit_rsrc,NULL,
						pmonth->numlibs,
						&pmonth->libs,NULL);
				}
			} else {
				edit_rsrc=TRUE;
				nonlstmakefld(parent,reqitm,fldx,edit_rsrc);
			}
		}
	}
	edit_rsrc=TRUE;
	FINDRSCSETFUNC(parent,"[REQITM][QUANTITY]",multicaltotalqty,NULL);
	FINDRSCSETFUNC(parent,"[REQITM][UNIT RATE]",multicaltotalur,NULL);
	FINDRSCSETFUNC(parent,"[REQITM][PERCENTAGE DISCOUNT]",multicaltotalur,NULL);
	FINDRSCSETFUNC(parent,"[REQITM][AMOUNT]",multitotal,NULL);
	FINDRSCSETFUNC(parent,"[REQITM][VENDOR IDENTIFICATION]",checkvendorcb,
		NULL);
	nofields=0;
	nokeys=0;
	if(name!=NULL) Rfree(name);
	if(prevvch!=NULL) freeapplib(prevvch);
	prevvch=APPlibNEW();
	addAPPlib(prevvch,"No Requisition Items added this session");
	addlstrsrc(parent,"PREVIOUS ITEM'S",&x,TRUE,NULL,prevvch->numlibs,
		&prevvch->libs,NULL);
	ACCOUNTS=REQACCNEW();
	getAddReqDefAcct(ACCOUNTS);
	if(acclst!=NULL) freeapplib(acclst);
	acclst=APPlibNEW();
	y=0;
	addlstrsrc(parent,"ACCOUNT LIST",&y,TRUE,NULL,acclst->numlibs,
		&acclst->libs,NULL);
	addbtnrsrc(parent,"DEFINE ACCOUNT LIST",TRUE,defineacclstfunc,defineacclst);
	addbtnrsrc(parent,"ADD ABOVE",TRUE,add_account_above,NULL);
	addbtnrsrc(parent,"ADD BELOW",TRUE,add_account_below,NULL);
	addbtnrsrc(parent,"DELETE",TRUE,delete_account,NULL);
	addbtnrsrc(parent,"SELECT",TRUE,edit_account,NULL);
	makeaccountlist();
	FINDRSCLISTAPPlib(parent,"ACCOUNT LIST",y,acclst);
	date=GETCURRENTDATE10();
	timex=GETCURRENTTIME();
	FINDRSCSETSTRING(parent,"[REQITM][ENTRY DATE]",date);
	FINDRSCSETSTRING(parent,"[REQITM][ENTRY TIME]",timex);
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
	total_req=0.0;
	savelist(definelist,parent,TRUE);
	saveacclst(defineacclst,parent);
	if(addresslist!=NULL) freeapplib(addresslist);
	addresslist=APPlibNEW();
	addlstrsrc(parent,"ADDRESS IDENTIFICATIONS",&x,TRUE,changeADDRlist,
		addresslist->numlibs,&addresslist->libs,NULL);
/*
	FINDRSCSETFUNC(parent,"[REQITM][ADDRESS IDENTIFICATION]",getaddrcb,NULL);
*/
	setaddresses(parent);
	SetVirFieldFuncs(parent);
	SCRNvirtual2rsrc(parent);
	addstdrsrc(parent,"TOTAL REQUISITION",DOLLARS,20,&total_req,TRUE);
	addstdrsrc(parent,"CATALOGUE VENDOR",BOOLNS,1,&catalogvendor,TRUE);
	addbtnrsrc(parent,"BROWSE VENDORS",TRUE,mbrowse_vendors,NULL);
	addbtnrsrc(parent,"BROWSE ITEMS",TRUE,mbrowse_items,NULL);
	addbtnrsrc(parent,"DEFINE LIST",TRUE,definelistfunc,definelist);
	addbtnrsrc(parent,"DEFAULTS",TRUE,reqaddiSaveDefaults,NULL);
	addbtnrsrc(parent,"RESET DEFAULTS",TRUE,set_defaults,NULL);
	addrfcbrsrc(parent,"SAVE",TRUE,save_check,NULL);
	addbtnrsrc(parent,"QUIT",TRUE,quit_check,NULL);
	addrfcbrsrc(parent,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(parent,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	set_defaults(parent);
	APPmakescrn(parent,TRUE,quit_record,NULL,FALSE);
}
static void mbrowse_items(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;
	char primven=TRUE;
	int selcat=0;

	FINDRSCGETINT(mtnrsrc,"CATEGORY IDENTIFICATIONS",&selcat);
	mainrsrc=RDArsrcNEW(module,"ADD REQUISITION CATALOGUE BROWSE");
	searchrsrc=RDArsrcNEW(module,"ADD REQUISITION CATALOGUE SEARCH BROWSE");
	definelist=RDArsrcNEW(module,"ADD REQUISITION CATALOGUE DEFINE LIST");
	ZERNRD(reqctlg);
	addDFincvir(mainrsrc,"CATALOGUE","CATMST",NULL,reqcat); 
	addDFincvir(mainrsrc,"CATALOGUE","CTLGVEN",NULL,reqven); 
	addDFincvir(mainrsrc,"CATALOGUE","CTLGMST",NULL,reqctlg);
	addDFincvir(mainrsrc,"CATALOGUE","CTLGVP",NULL,reqpven);
	addDFincvir(searchrsrc,"CATALOGUE","CTLGMST",NULL,reqctlg); 
	addDFincvir(searchrsrc,"CATALOGUE","CATMST",NULL,reqcat); 
	addDFincvir(definelist,"CATALOGUE","CTLGMST",NULL,reqctlg); 
	addDFincvir(definelist,"CATALOGUE","CATMST",NULL,reqcat); 
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	if(reqcat!=(-1))
	{
		FINDFLDSETSTRING(reqcat,"CATEGORY IDENTIFICATION",
			categories->libs[selcat]);
		if(ADVEQLNRDsec(reqcat,1,SCRNvirtualSubData,mainrsrc))
		{
			ZERNRD(reqcat);
		} else {
			file2rsrc(reqcat,mainrsrc,FALSE);
			filerecord2rsrc(reqcat,mainrsrc);
		}
	}
	addstdrsrc(mainrsrc,"PRIMARY VENDOR",BOOLNS,1,&primven,TRUE);
	addstdrsrc(mainrsrc,"LOAD PRICE",BOOLNS,1,&primven,TRUE);
	ADVbrowse(reqctlg,1,mainrsrc,searchrsrc,definelist,inititem,selectitem,
		quititem,mokitemfunc,NULL,mtnrsrc,TRUE,NULL,2,2,2,FALSE);
}
static void mbrowse_vendors(RDArsrc *mtnrsrc)
{
	char catalogvendor=TRUE;

	readwidget(mtnrsrc,"CATALOGUE VENDOR");
	FINDRSCGETCHAR(mtnrsrc,"CATALOGUE VENDOR",&catalogvendor);
	if(catalogvendor)
	{
		if(diagapps)
		{
			prterr("Browsing on CATALOGUE VENDORS=TRUE Vendor Identifications.");
		}
		mbrowse_catalog(mtnrsrc);
	} else {
		if(diagapps)
		{
			prterr("Browsing on CATALOGUE VENDORS=FALSE Vendor Identifications.");
		}
		browse_venmsr(mtnrsrc);
	}
}
static void mbrowse_catalog(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;
	char *item=NULL,price=TRUE;
	int selcat=0;

	mainrsrc=RDArsrcNEW(module,"ADD REQUISITION CATALOGUE VENDOR BROWSE");
	searchrsrc=RDArsrcNEW(module,"ADD REQUISITION CATALOGUE VENDOR SEARCH BROWSE");
	definelist=RDArsrcNEW(module,"ADD REQUISITION CATALOGUE VENDOR DEFINE LIST");
	ZERNRD(reqven);
	addDFincvir(mainrsrc,"CATALOGUE","CTLGMST",NULL,reqctlg);
	addDFincvir(mainrsrc,"CATALOGUE","CATMST",NULL,reqcat);
	addDFincvir(mainrsrc,"CATALOGUE","CTLGVEN",NULL,reqven); 
	addDFincvir(mainrsrc,"CATALOGUE","CTLGVP",NULL,reqpven); 
	addDFincvir(searchrsrc,"CATALOGUE","CTLGVEN",NULL,reqven); 
	addDFincvir(searchrsrc,"CATALOGUE","CTLGMST",NULL,reqctlg);
	addDFincvir(searchrsrc,"CATALOGUE","CATMST",NULL,reqcat);
	addDFincvir(searchrsrc,"VENPMT","VENMSR",NULL,vennum);
	addDFincvir(definelist,"CATALOGUE","CTLGVEN",NULL,reqven); 
	addDFincvir(definelist,"CATALOGUE","CTLGMST",NULL,reqctlg);
	addDFincvir(definelist,"CATALOGUE","CATMST",NULL,reqcat);
	addDFincvir(definelist,"VENPMT","VENMSR",NULL,vennum);
	FINDRSCGETINT(mtnrsrc,"CATEGORY IDENTIFICATIONS",&selcat);
	readwidget(mtnrsrc,"[REQITM][ITEM IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[REQITM][ITEM IDENTIFICATION]",&item);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	if(reqcat!=(-1))
	{
		FINDFLDSETSTRING(reqcat,"CATEGORY IDENTIFICATION",
			categories->libs[selcat]);
		if(ADVEQLNRDsec(reqcat,1,SCRNvirtualSubData,mainrsrc)) 
		{
			ZERNRD(reqcat);
		} else {
			file2rsrc(reqcat,mainrsrc,FALSE);
			filerecord2rsrc(reqcat,mainrsrc);
		}
	}
	if(reqctlg!=(-1))
	{
		FINDFLDSETSTRING(reqctlg,"CATEGORY IDENTIFICATION",
			categories->libs[selcat]);
		FINDFLDSETSTRING(reqctlg,"ITEM IDENTIFICATION",item);
		if(ADVEQLNRDsec(reqctlg,1,SCRNvirtualSubData,mainrsrc)) 
		{
			ZERNRD(reqctlg);
		} else {
			file2rsrc(reqctlg,mainrsrc,FALSE);
			filerecord2rsrc(reqctlg,mainrsrc);
		}
	}
	if(item!=NULL) Rfree(item);
	addstdrsrc(mainrsrc,"LOAD PRICE",BOOLNS,1,&price,TRUE);
	ADVbrowse(reqven,1,mainrsrc,searchrsrc,definelist,
		initreqven,selectreqven,quititem,mokreqvenfunc,NULL,
		mtnrsrc,TRUE,NULL,2,2,2,FALSE);
}
static void mokreqvenfunc(MakeBrowseList *b,void *targetkey)
{
	char *vendorid=NULL,*venitem=NULL,bestprice=FALSE;
	char *venid1=NULL,*item1=NULL,*item2=NULL,*cat1=NULL,*cat2=NULL;
	char *desc=NULL;
	int selcat=0,quantity=0,value=0;
	double unitrate=0.0,total=0.0,percdisc=0.0;

	ZERNRD(reqven);
	if(ADVEQLNRDKEYsec(reqven,1,targetkey,SCRNvirtualSubData,b->mainrsrc))
	{
		KEYNRD(reqven,1);
	} else {
		FINDFLDGETSTRING(reqven,"VENDOR IDENTIFICATION",&vendorid);
		FINDRSCSETSTRING(b->funcargs,"[REQITM][VENDOR IDENTIFICATION]",
			vendorid);
		FINDFLDSETSTRING(reqitm,"VENDOR IDENTIFICATION",vendorid);
		FINDFLDSETSTRING(vennum,"VENDOR IDENTIFICATION",vendorid);
		FINDFLDGETSTRING(reqven,"VENDOR ITEM NUMBER",&venitem);
		FINDFLDSETSTRING(reqitm,"VENDOR ITEM NUMBER",venitem);
		FINDRSCSETSTRING(b->funcargs,"[REQITM][VENDOR ITEM NUMBER]",
			venitem);
		checkvendor(b->funcargs,TRUE);
		filerecord2rsrc(vennum,b->funcargs);
		FINDFLDGETSTRING(reqven,"ITEM IDENTIFICATION",&item1);
		FINDFLDSETSTRING(reqitm,"ITEM IDENTIFICATION",item1);
		FINDFLDSETSTRING(reqctlg,"ITEM IDENTIFICATION",item1);

		FINDRSCSETSTRING(b->funcargs,"[REQITM][ITEM IDENTIFICATION]",
			item1);
		FINDRSCGETINT(b->funcargs,"CATEGORY IDENTIFICATIONS",&selcat);
		if(RDAstrcmp(categories->libs[selcat],"No Categories"))
		{
			cat1=stralloc(categories->libs[selcat]);
		}
		FINDFLDSETSTRING(reqctlg,"CATEGORY IDENTIFICATION",cat1);
		if(ADVEQLNRDsec(reqctlg,1,SCRNvirtualSubData,b->mainrsrc))
		{
			KEYNRD(reqctlg,1);
		} else {
			filerecord2rsrc(reqctlg,b->funcargs);
			FINDFLDGETSTRING(reqctlg,"DESCRIPTION",&desc);
			FINDRSCSETSTRING(b->funcargs,"[REQITM][DESCRIPTION]",desc);
			if(desc!=NULL) Rfree(desc);
		}
		if(cat1!=NULL) Rfree(cat1);
		readwidget(b->mainrsrc,"LOAD PRICE");
		FINDRSCGETCHAR(b->mainrsrc,"LOAD PRICE",&bestprice);
		if(bestprice)
		{
			ZERNRD(reqpven);
			FINDFLDGETSTRING(reqven,"CATEGORY IDENTIFICATION",&cat1);
			FINDFLDGETSTRING(reqven,"ITEM IDENTIFICATION",&item1);
			FINDFLDSETSTRING(reqpven,"CATEGORY IDENTIFICATION",cat1);
			FINDFLDSETSTRING(reqpven,"ITEM IDENTIFICATION",item1);
			FINDFLDSETSTRING(reqpven,"VENDOR IDENTIFICATION",vendorid);
			readwidget(b->funcargs,"[REQITM][QUANTITY]");
			FINDRSCGETINT(b->funcargs,"[REQITM][QUANTITY]",&quantity);
			readwidget(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]");
			FINDRSCGETDOUBLE(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]",&percdisc);
			FINDFLDSETINT(reqpven,"QUANTITY",quantity);
			if(!ADVLTENRDsec(reqpven,1,SCRNvirtualSubData,b->mainrsrc))
			{
				FINDFLDGETSTRING(reqpven,"VENDOR IDENTIFICATION",&venid1);		
				FINDFLDGETSTRING(reqpven,"ITEM IDENTIFICATION",&item2);
				FINDFLDGETSTRING(reqpven,"CATEGORY IDENTIFICATION",&cat2);
				if(RDAstrcmp(vendorid,venid1) || RDAstrcmp(item1,item2) || RDAstrcmp(cat1,cat2))
				{
					readwidget(b->funcargs,"[REQITM][UNIT RATE]");
					FINDRSCGETDOUBLE(b->funcargs,"[REQITM][UNIT RATE]",&unitrate);
					readwidget(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]");
					FINDRSCGETDOUBLE(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]",&percdisc);
				} else {
					FINDFLDGETDOUBLE(reqpven,"UNIT RATE",&unitrate);
					FINDFLDGETDOUBLE(reqpven,"PERCENTAGE DISCOUNT",&percdisc);
				}
			} else {
				readwidget(b->funcargs,"[REQITM][UNIT RATE]");
				FINDRSCGETDOUBLE(b->funcargs,"[REQITM][UNIT RATE]",&unitrate);
				readwidget(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]");
				FINDRSCGETDOUBLE(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]",&percdisc);
			}
			if(percdisc!=0.0)
			{
				total=round(((unitrate*quantity)*100.0)-(((unitrate*quantity)*100.0)*percdisc/100));
			} else {
				total=round(((unitrate*quantity)*100.0));
			}
			FINDRSCSETDOUBLE(b->funcargs,"[REQITM][UNIT RATE]",unitrate);
			FINDRSCSETDOUBLE(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]",percdisc);
			FINDRSCSETDOUBLE(b->funcargs,"[REQITM][AMOUNT]",total);
			FINDRSCGETINT(b->funcargs,"ACCOUNT LIST",&value);
			ComputeREQACC(ACCOUNTS,total);
			makeaccountlist();
			FINDRSCLISTAPPlib(b->funcargs,"ACCOUNT LIST",value,acclst);
		}
		if(venid1!=NULL) Rfree(venid1);
		if(cat1!=NULL) Rfree(cat1);
		if(cat2!=NULL) Rfree(cat2);
		if(item1!=NULL) Rfree(item1);
		if(item2!=NULL) Rfree(item2);
		if(vendorid!=NULL) Rfree(vendorid);
		if(venitem!=NULL) Rfree(venitem);
		computeallSCRNvirtuals(b->funcargs);
		updateallrsrc(b->funcargs);
	}
	setaddresses(b->funcargs);
}
static void mokitemfunc(MakeBrowseList *b,void *targetkey)
{
	char *itemid=NULL,primven=FALSE,*vendorid=NULL,*desc=NULL;
	char *venitem=NULL,bestprice=FALSE,*itemname=NULL;
	char *cat1=NULL,*cat2=NULL,*itemid1=NULL,*venid1=NULL;
	int quantity=0,value=0;
	double unitrate=0.0,total=0.0,percdisc=0.0;

	ZERNRD(reqctlg);
	if(ADVEQLNRDKEYsec(reqctlg,1,targetkey,SCRNvirtualSubData,b->mainrsrc))
	{
		KEYNRD(reqctlg,1);
	} else {
		FINDFLDGETSTRING(reqctlg,"ITEM IDENTIFICATION",&itemid);
		FINDFLDGETSTRING(reqctlg,"DESCRIPTION",&desc);
		FINDFLDGETSTRING(reqctlg,"ITEM NAME",&itemname);
		FINDRSCSETSTRING(b->funcargs,"[REQITM][DESCRIPTION]",desc);
		FINDRSCSETSTRING(b->funcargs,"[REQITM][ITEM NAME]",itemname);
		FINDFLDSETSTRING(reqitm,"[REQITM][ITEM NAME]",itemname);
		if(itemname!=NULL) Rfree(itemname);
		if(desc!=NULL) Rfree(desc);
		FINDRSCSETSTRING(b->funcargs,"[REQITM][ITEM IDENTIFICATION]",itemid);
		readwidget(b->mainrsrc,"PRIMARY VENDOR");
		FINDRSCGETCHAR(b->mainrsrc,"PRIMARY VENDOR",&primven);
		if(primven)
		{
			readwidget(b->mainrsrc,"LOAD PRICE");
			FINDRSCGETCHAR(b->mainrsrc,"LOAD PRICE",&bestprice);
			FINDFLDGETSTRING(reqctlg,"PRIMARY VENDOR IDENTIFICATION",&vendorid);
			FINDRSCSETSTRING(b->funcargs,"[REQITM][VENDOR IDENTIFICATION]",vendorid);
			FINDFLDSETSTRING(reqitm,"VENDOR IDENTIFICATION",vendorid);
			FINDFLDGETSTRING(reqctlg,"CATEGORY IDENTIFICATION",&cat1);
			FINDFLDSETSTRING(reqven,"CATEGORY IDENTIFICATION",cat1);
			COPYFIELD(reqctlg,reqven,"ITEM IDENTIFICATION");
			FINDFLDSETSTRING(reqven,"VENDOR IDENTIFICATION",vendorid);
			if(!ADVEQLNRDsec(reqven,1,SCRNvirtualSubData,b->mainrsrc))
			{
				FINDFLDGETSTRING(reqven,"VENDOR ITEM NUMBER",&venitem);
			}
			FINDFLDSETSTRING(reqven,"VENDOR ITEM NUMBER",venitem);
			FINDRSCSETSTRING(b->funcargs,"[REQITM][VENDOR ITEM NUMBER]",
				venitem);
			checkvendor(b->funcargs,TRUE);
			filerecord2rsrc(vennum,b->funcargs);
			if(bestprice)
			{
				ZERNRD(reqpven);
				COPYFIELD(reqctlg,reqpven,"CATEGORY IDENTIFICATION");
				FINDFLDSETSTRING(reqpven,"ITEM IDENTIFICATION",itemid);
				FINDFLDSETSTRING(reqpven,"VENDOR IDENTIFICATION",vendorid);
				readwidget(b->funcargs,"[REQITM][QUANTITY]");
				FINDRSCGETINT(b->funcargs,"[REQITM][QUANTITY]",&quantity);
				readwidget(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]");
				FINDRSCGETDOUBLE(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]",&percdisc);
				FINDFLDSETINT(reqpven,"QUANTITY",quantity);
				if(!ADVLTENRDsec(reqpven,1,SCRNvirtualSubData,b->mainrsrc))
				{
					FINDFLDGETSTRING(reqpven,"CATEGORY IDENTIFICATION",&cat2);
					FINDFLDGETSTRING(reqpven,"ITEM IDENTIFICATION",&itemid1);
					FINDFLDGETSTRING(reqpven,"VENDOR IDENTIFICATION",&venid1);
					if(RDAstrcmp(cat1,cat2) || RDAstrcmp(itemid,itemid1) ||
						RDAstrcmp(vendorid,venid1))
					{
						readwidget(b->funcargs,"[REQITM][UNIT RATE]");
						FINDRSCGETDOUBLE(b->funcargs,"[REQITM][UNIT RATE]",&unitrate);
						readwidget(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]");
						FINDRSCGETDOUBLE(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]",&percdisc);
					} else {
						FINDFLDGETDOUBLE(reqpven,"UNIT RATE",&unitrate);
						FINDFLDGETDOUBLE(reqpven,"PERCENTAGE DISCOUNT",&percdisc);
					}
				} else {
					readwidget(b->funcargs,"[REQITM][UNIT RATE]");
					FINDRSCGETDOUBLE(b->funcargs,"[REQITM][UNIT RATE]",&unitrate);
					readwidget(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]");
					FINDRSCGETDOUBLE(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]",&percdisc);
				}
				if(percdisc!=0.0)
				{
					total=round(((unitrate*quantity)*100.0)-(((unitrate*quantity)*100.0)*percdisc/100));
				} else {
					total=round(((unitrate*quantity)*100.0));
				}
				FINDRSCSETDOUBLE(b->funcargs,"[REQITM][UNIT RATE]",unitrate);
				FINDRSCSETDOUBLE(b->funcargs,"[REQITM][PERCENTAGE DISCOUNT]",percdisc);
				FINDRSCSETDOUBLE(b->funcargs,"[REQITM][AMOUNT]",total);
				FINDRSCGETINT(b->funcargs,"ACCOUNT LIST",&value);
				ComputeREQACC(ACCOUNTS,total);
				makeaccountlist();
				FINDRSCLISTAPPlib(b->funcargs,"ACCOUNT LIST",value,acclst);
			}
			setaddresses(b->funcargs);
		}
		if(cat1!=NULL) Rfree(cat1);
		if(cat2!=NULL) Rfree(cat2);
		if(itemid1!=NULL) Rfree(itemid1);
		if(venid1!=NULL) Rfree(venid1);
		if(itemid!=NULL) Rfree(itemid);
		if(vendorid!=NULL) Rfree(vendorid);
		if(venitem!=NULL) Rfree(venitem);
		computeallSCRNvirtuals(b->funcargs);
		updateallrsrc(b->funcargs);
	}
}
