/*lint -library */
#include <cstdio>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <fin.hpp>
#include <purord.hpp>

struct PassableStructs
{
	RDArsrc *r;
	void *a;
};
typedef struct PassableStructs PassableStruct;

struct mtnstructs
{
	APPlib *DEFtypes;
	APPlib *ACCTtypes;
	APPlib *cpmonth;
	APPlib *pmonth;
	short update_list;
	short acctwidget;
	RDATData *previous;
	APPlib *clearDEFtypes;
	PassableStruct *p;
};
typedef struct mtnstructs mtnstruct;

extern void PurApvSubData(char **,PassableStruct *);
static void getrecordsetedit(RDArsrc *,mtnstruct *);
static void seteditable(RDArsrc *,mtnstruct *);
static void getsupportingrecords(RDArsrc *,DBsort *,short);
static void save_record(RDArsrc *,mtnstruct *);
static void save_record_warn(RDArsrc *,mtnstruct *);
static void quit_record(RDArsrc *,mtnstruct *);
/*
static void quit_record_warn(RDArsrc *,mtnstruct *);
*/
static void save_record_nup(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void getfilecb(RDArsrc *,mtnstruct *mtn);
static void getpmonths(RDArsrc *,mtnstruct *);
static void changeACCTtype(RDArsrc *,mtnstruct *);
static void changeDEFtype(RDArsrc *,mtnstruct *);
static void changeclearDEFtype(RDArsrc *,mtnstruct *);
static void ACCTtypecb(RDArsrc *,mtnstruct *);
static void DEFtypecb(RDArsrc *,mtnstruct *);
static void clearDEFtypecb(RDArsrc *,mtnstruct *);
static void setACCTtype(RDArsrc *,mtnstruct *);
static void setpmonth(RDArsrc *,mtnstruct *);
static void save_check(RDArsrc *,mtnstruct *);
static short check_code(RDArsrc *,mtnstruct *);
static short check_acctcode(RDArsrc *,mtnstruct *);
static short check_debcode(RDArsrc *,mtnstruct *);
static short check_credcode(RDArsrc *,mtnstruct *);
static short check_paycode(RDArsrc *,mtnstruct *);
/*
static short check_dueto(RDArsrc *,mtnstruct *);
*/
static short check_clearfrom(RDArsrc *,mtnstruct *);
static short check_clearcash(RDArsrc *,mtnstruct *);
static void check_bankidcb(RDArsrc *,mtnstruct *);
static short check_bankid(RDArsrc *,mtnstruct *);
static void acctcodecb(RDArsrc *,mtnstruct *);
static void debcodecb(RDArsrc *,mtnstruct *);
static void credcodecb(RDArsrc *,mtnstruct *);
static void paycodecb(RDArsrc *,mtnstruct *);
/*
static void duetocb(RDArsrc *,mtnstruct *);
*/
static void clearfromcb(RDArsrc *,mtnstruct *);
static void clearcashcb(RDArsrc *,mtnstruct *);
static void clearwidget(RDArsrc *,char *);
static short testavailbalance(RDArsrc *,mtnstruct *);
/*
static void check_availbalance(RDArsrc *,mtnstruct *);
*/
extern RDApurord *purord;
extern RDAvenpmt *venpmt;
extern char *module;
extern short msrnum,sitenum,vennum,vchnum;
extern short linenum,invnum,bnkacc;
extern short finyrnum,eyrnum,ryrnum,byrnum,yearnum;
extern short expnum,revnum,balnum;
extern DBsort *dbsort;
extern RDAfinmgt *finsetup;
extern MakeBrowseList *mbl_rec;
extern void browse_purapv(RDArsrc *);
extern char *vendorid;
extern char *invid,*poid;
extern int venyrid,yearid;
extern double total_po;
char distributable=TRUE;
void purapvm(short,void *,int,char **);
int transno=0;
PassableStruct *PSTRUCT=NULL;

short PURAPV_GUIEVAL(char *string,RDArsrc *r,PassableStruct *PSTRUCT)
{
	if(!isEMPTY(string))
	{
		return(EVALUATEbol(string,PurApvSubData,PSTRUCT));
	} else return(TRUE);
}
char *PURAPV_GUIEVALSTR(char *string,RDArsrc *r,PassableStruct *PSTRUCT)
{
	char *tmp=NULL;

	if(!isEMPTY(string))
	{
		tmp=EVALUATEstr(string,PurApvSubData,PSTRUCT);
	}
	return(tmp);
}

static void costtotal(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	double cost=0.0,adminfee=0.0,prorateamt=0.0,taxamt=0.0,total=0.0;
	char deftax=FALSE,defadmin=FALSE;
	double tax_rate=0.0;
	short tax_type=0;

	readwidget(mtnrsrc,"COST");
	readwidget(mtnrsrc,"TAX AMOUNT");
	readwidget(mtnrsrc,"ADMIN FEE");
	readwidget(mtnrsrc,"PRORATE AMOUNT");
	readwidget(mtnrsrc,"DEFAULT TAX");
	readwidget(mtnrsrc,"DEFAULT ADMIN FEE");
	FINDRSCGETCHAR(mtnrsrc,"DEFAULT TAX",&deftax);
	FINDRSCGETCHAR(mtnrsrc,"DEFAULT ADMIN FEE",&defadmin);
	FINDRSCGETDOUBLE(mtnrsrc,"COST",&cost);
	FINDRSCGETDOUBLE(mtnrsrc,"TAX AMOUNT",&taxamt);
	FINDRSCGETDOUBLE(mtnrsrc,"ADMIN FEE",&adminfee);
	FINDRSCGETDOUBLE(mtnrsrc,"PRORATE AMOUNT",&prorateamt);
	if(defadmin)
	{
/* Need a new SubData for this */
		adminfee=EVALUATEdbl(venpmt->admin_fee,PurApvSubData,mtn->p);
		FINDRSCSETDOUBLE(mtnrsrc,"ADMIN FEE",adminfee);
		updatersrc(mtnrsrc,"ADMIN FEE");
	}
	if(deftax)	
	{
		FINDFLDGETSHORT(vennum,"DEFAULT TAX TYPE",&tax_type);
		switch(tax_type)
		{
			default:
			case 0:
				tax_rate=venpmt->Tax1_Rate;
				break;
			case 1:
				tax_rate=venpmt->Tax2_Rate;
				break;
			case 2:	
				tax_rate=venpmt->Tax3_Rate;
				break;
			case 3:	
				tax_rate=venpmt->Tax4_Rate;
				break;
			case 4:	
				tax_rate=venpmt->Tax5_Rate;
				break;
		}
		taxamt=(round(cost*tax_rate));
		FINDRSCSETDOUBLE(mtnrsrc,"TAX AMOUNT",taxamt);
		updatersrc(mtnrsrc,"TAX AMOUNT");
	}
	total=cost+adminfee+taxamt+prorateamt;
	FINDRSCSETDOUBLE(mtnrsrc,"AMOUNT",total);
	updatersrc(mtnrsrc,"AMOUNT");
}
static void totalcalc(RDArsrc *mtnrsrc)
{
	double cost=0.0,adminfee=0.0,prorateamt=0.0,taxamt=0.0,total=0.0;

	readwidget(mtnrsrc,"COST");
	readwidget(mtnrsrc,"TAX AMOUNT");
	readwidget(mtnrsrc,"ADMIN FEE");
	readwidget(mtnrsrc,"PRORATE AMOUNT");
	FINDRSCGETDOUBLE(mtnrsrc,"COST",&cost);
	FINDRSCGETDOUBLE(mtnrsrc,"TAX AMOUNT",&taxamt);
	FINDRSCGETDOUBLE(mtnrsrc,"ADMIN FEE",&adminfee);
	FINDRSCGETDOUBLE(mtnrsrc,"PRORATE AMOUNT",&prorateamt);
	total=cost+adminfee+taxamt+prorateamt;
	FINDRSCSETDOUBLE(mtnrsrc,"AMOUNT",total);
	updatersrc(mtnrsrc,"AMOUNT");
}
static void quit_record_test(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *desc=NULL;

	readallwidgets(mtnrsrc);
	rsrc2singlefilerecord(dbsort->fileno,mtnrsrc);
	if(QUITvfy(dbsort->fileno,mtn->previous))
	{
		desc=QUITvfydesc(dbsort->fileno,mtn->previous);
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!",
			desc,save_record_up,quit_record,FALSE,2,(void *)mtnrsrc,(void *)mtn,NULL);
		if(desc!=NULL) Rfree(desc);
	} else {
		quit_record(mtnrsrc,mtn);
	}
}
void purapvm(short dowhich,void *targetkey,int argc,char **argv)
{
	char tv=FALSE,*date=NULL,*timex=NULL,*temp=NULL;
	char *edesc=NULL;
	short nofields=0,x=0,editable=FALSE,edit_rsrc=TRUE;
	int which_type=0,tmpnum=0;
	RDAacct *acct=NULL;
	NRDfield *fields=NULL,*fldx,*field=NULL;
	RDArsrc *mtnrsrc=NULL;
	mtnstruct *mtn;
	
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->update_list=TRUE;
	mtn->DEFtypes=NULL;
	mtn->ACCTtypes=NULL;
	mtn->clearDEFtypes=NULL;
	mtn->cpmonth=NULL;
	mtn->pmonth=NULL;
	mtn->DEFtypes=APPlibNEW();
	mtn->ACCTtypes=APPlibNEW();
	if(venpmt->use_venpmt_fund)
	{
		mtn->clearDEFtypes=APPlibNEW();
		if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
		{
			for(x=0,acct=BALANCE_SHEET_ACCOUNT->codetype;
				x<BALANCE_SHEET_ACCOUNT->num;++x,++acct)
			{
				addAPPlib(mtn->clearDEFtypes,acct->name);
			}
		}
	}
	mtn->acctwidget=0;
	ZERNRD(dbsort->fileno);
	mtn->previous=RDATDataNEW(dbsort->fileno);
	addAPPlib(mtn->ACCTtypes,accttypes[0]);
	addAPPlib(mtn->ACCTtypes,accttypes[1]);
	addAPPlib(mtn->ACCTtypes,accttypes[2]);
	mtnrsrc=RDArsrcNEW(module,"ADD PURCHASE ORDER VOUCHER MAINTAIN SCREEN");
	mtn->p=Rmalloc(sizeof(PassableStruct));
	mtn->p->r=mtnrsrc;
	mtn->p->a=dbsort;
	PSTRUCT=mtn->p;
	getpmonths(mtnrsrc,mtn);
	nofields=NUMFLDS(dbsort->fileno);
	fields=FLDPOINTER(dbsort->fileno);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=TRUE;
			if(!RDAstrcmp(fldx->name,"ACCOUNT TYPE"))
			{
				if(FIELDscrn(dbsort->fileno,fldx->name,&edit_rsrc))
				{
					which_type=0;
					addlstrsrc(mtnrsrc,"ACCOUNT TYPES",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						ACCTtypecb,mtn->ACCTtypes->numlibs,
						&mtn->ACCTtypes->libs,mtn);
				}
			} else if(!RDAstrcmp(fldx->name,"DEFINITION TYPE"))
			{
				if(FIELDscrn(dbsort->fileno,fldx->name,&edit_rsrc))
				{
					which_type=0;
					addlstrsrc(mtnrsrc,"DEFINITION TYPES",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						DEFtypecb,mtn->DEFtypes->numlibs,
						&mtn->DEFtypes->libs,mtn);
				}
			} else if(!RDAstrcmp(fldx->name,"CLEARING DEFINITION TYPE"))
			{
				if(venpmt->use_venpmt_fund)
				{
				if(FIELDscrn(dbsort->fileno,fldx->name,&edit_rsrc))
				{
					which_type=0;
					addlstrsrc(mtnrsrc,"CLEARING DEFINITION TYPES",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						clearDEFtypecb,mtn->clearDEFtypes->numlibs,
						&mtn->clearDEFtypes->libs,mtn);
				}
				}
			} else if(!RDAstrcmp(fldx->name,"CLEAR PROCESSING MONTH"))
			{
				if(FIELDscrn(dbsort->fileno,fldx->name,&edit_rsrc))
				{
					which_type=1;
					addlstrsrc(mtnrsrc,"CLEAR PROCESSING MONTHS",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),NULL,
						mtn->cpmonth->numlibs,
						&mtn->cpmonth->libs,NULL);
				}
			} else if(!RDAstrcmp(fldx->name,"FISCAL PROCESSING MONTH"))
			{
				if(FIELDscrn(dbsort->fileno,fldx->name,&edit_rsrc))
				{
					which_type=1;
					addlstrsrc(mtnrsrc,"FISCAL PROCESSING MONTHS",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),NULL,
						mtn->cpmonth->numlibs,
						&mtn->cpmonth->libs,NULL);
				}
			} else if(!RDAstrcmp(fldx->name,"PROCESSING MONTH"))
			{
				if(FIELDscrn(dbsort->fileno,fldx->name,&edit_rsrc))
				{
					which_type=1;
					addlstrsrc(mtnrsrc,"PROCESSING MONTHS",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						NULL,mtn->pmonth->numlibs,
						&mtn->pmonth->libs,NULL);
				}
			} else if(!RDAstrcmp(fldx->name,"PO IDENTIFICATION") ||
				!RDAstrcmp(fldx->name,"VENDOR IDENTIFICATION") ||
				!RDAstrcmp(fldx->name,"CALENDAR YEAR") ||
				!RDAstrcmp(fldx->name,"INVOICE IDENTIFICATION") ||
				!RDAstrcmp(fldx->name,"FISCAL YEAR"))
/* setting these field names editable value to FALSE */
			{
				if(FIELDscrn(dbsort->fileno,fldx->name,&editable))
				{
					makefieldrsrc(mtnrsrc,fldx->name,fldx->type,
						fldx->len,editable);
				}
			} else {
				if(FIELDscrn(dbsort->fileno,fldx->name,&edit_rsrc))
				{
					makefieldrsrc(mtnrsrc,fldx->name,fldx->type,
						fldx->len,edit_rsrc);
				}
			}
		}
	}
	nofields=0;
	FINDRSCSETFUNC(mtnrsrc,"VENDOR IDENTIFICATION",getfilecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"CALENDAR YEAR",getfilecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"INVOICE IDENTIFICATION",getfilecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"TRANSACTION NUMBER",getfilecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"ACCOUNT CODE",acctcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"DEBIT CODE",debcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"CREDIT CODE",credcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"PAYABLE CODE",paycodecb,mtn);
	if(venpmt->use_venpmt_fund)
	{
/*
		FINDRSCSETFUNC(mtnrsrc,"DUE TO",duetocb,mtn);
*/
		FINDRSCSETFUNC(mtnrsrc,"CLEARING DUE FROM",clearfromcb,mtn);
		FINDRSCSETFUNC(mtnrsrc,"CLEARING CASH",clearcashcb,mtn);
	}
/*
	FINDRSCSETFUNC(mtnrsrc,"AMOUNT",check_availbalance,mtn);
*/
	if(venpmt->Use_Taxes)
	{
		addstdrsrc(mtnrsrc,"DEFAULT TAX",BOOLNS,1,&tv,TRUE);
		addstdrsrc(mtnrsrc,"DEFAULT ADMIN FEE",BOOLNS,1,&tv,TRUE);
		FINDRSCSETFUNC(mtnrsrc,"DEFAULT ADMIN FEE",costtotal,mtn);
		FINDRSCSETFUNC(mtnrsrc,"DEFAULT TAX",costtotal,mtn);
		FINDRSCSETEDITABLE(mtnrsrc,"AMOUNT",FALSE);
		FINDRSCSETFUNC(mtnrsrc,"COST",costtotal,mtn);
		FINDRSCSETFUNC(mtnrsrc,"TAX AMOUNT",totalcalc,NULL);
		FINDRSCSETFUNC(mtnrsrc,"PRORATE AMOUNT",totalcalc,NULL);
		FINDRSCSETFUNC(mtnrsrc,"ADMIN FEE",totalcalc,NULL);
	}
	FINDRSCSETFUNC(mtnrsrc,"BANK IDENTIFICATION",check_bankidcb,mtn);
	if(msrnum!=(-1)) file2rsrc(msrnum,mtnrsrc,FALSE);
	if(sitenum!=(-1)) file2rsrc(sitenum,mtnrsrc,FALSE);
	if(linenum!=(-1)) file2rsrc(linenum,mtnrsrc,FALSE);
	if(bnkacc!=(-1)) file2rsrc(bnkacc,mtnrsrc,FALSE);
	if(vennum!=(-1)) file2rsrc(vennum,mtnrsrc,FALSE);
	if(yearnum!=(-1)) file2rsrc(yearnum,mtnrsrc,FALSE);
	if(invnum!=(-1)) file2rsrc(invnum,mtnrsrc,FALSE);
	DefaultScreens(mtnrsrc);
	addDFincvir(mtnrsrc,module,"PURMST",NULL,msrnum);
	addDFincvir(mtnrsrc,module,"PURSIT",NULL,sitenum);
	addDFincvir(mtnrsrc,module,"PURLIN",NULL,linenum);
	addDFincvir(mtnrsrc,"BNKREC","BNKACC",NULL,bnkacc);
	addDFincvir(mtnrsrc,"VENPMT","VENMSR",NULL,vennum);
	addDFincvir(mtnrsrc,"VENPMT","VENYR",NULL,yearnum);
	addDFincvir(mtnrsrc,"VENPMT","VENINV",NULL,invnum);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	switch(dowhich)
	{
		case 1:
			FINDRSCGETINT(mtnrsrc,"TRANSACTION NUMBER",&transno);
			if(!CONVERTPASSABLEKEYS(dbsort->fileno,1,argc,argv))
			{
				if(EQLNRD(dbsort->fileno,1))
				{
					KEYNRD(dbsort->fileno,1);
					FINDFLDGETSTRING(dbsort->fileno,"VENDOR IDENTIFICATION",&vendorid);
					FINDFLDGETINT(dbsort->fileno,"CALENDAR YEAR",&venyrid);
					FINDFLDGETSTRING(dbsort->fileno,"INVOICE IDENTIFICATION",&invid);
					FINDFLDSETINT(dbsort->fileno,"TRANSACTION NUMBER",transno);
					if(EQLNRD(dbsort->fileno,1))
					{
						KEYNRD(dbsort->fileno,1);
						FINDRSCSETSTRING(mtnrsrc,"VENDOR IDENTIFICATION",vendorid);
						FINDRSCSETINT(mtnrsrc,"CALENDAR YEAR",venyrid);
						FINDRSCSETSTRING(mtnrsrc,"INVOICE IDENTIFICATION",invid);
						FINDRSCSETSTRING(mtnrsrc,"PO IDENTIFICATION",poid);
						FINDRSCSETINT(mtnrsrc,"FISCAL YEAR",yearid);
						field=FLDNRD(vchnum,"ENTRY DATE");
						if(field!=NULL) 
						{
							if(field->len<10) date=GETCURRENTDATE();
							else date=GETCURRENTDATE10();
						} else date=GETCURRENTDATE();
						timex=GETCURRENTTIME();
						FINDRSCSETSTRING(mtnrsrc,"ENTRY DATE",date);
						FINDRSCSETSTRING(mtnrsrc,"ENTRY TIME",timex);
						FINDRSCSETSTRING(mtnrsrc,"SOURCE USER",USERLOGIN);
						setACCTtype(mtnrsrc,mtn);
					} else {
						getrecordsetedit(mtnrsrc,mtn);
						GetRDATData(dbsort->fileno,mtn->previous);
					}
				} else {
					GetRDATData(dbsort->fileno,mtn->previous);
					getrecordsetedit(mtnrsrc,mtn);
					FINDFLDGETSTRING(dbsort->fileno,"VENDOR IDENTIFICATION",&vendorid);
					FINDFLDGETINT(dbsort->fileno,"CALENDAR YEAR",&venyrid);
					FINDFLDGETSTRING(dbsort->fileno,"INVOICE IDENTIFICATION",&invid);
				}
			}
			break;
		case 2:
			if(argc>0)
			{
				FINDRSCGETINT(mtnrsrc,"TRANSACTION NUMBER",&transno);
				CONVERTPASSABLEKEYS(invnum,1,argc,argv);
				FINDFLDGETSTRING(invnum,"VENDOR IDENTIFICATION",&vendorid);
				FINDFLDGETINT(invnum,"CALENDAR YEAR",&venyrid);
				FINDFLDGETSTRING(invnum,"INVOICE IDENTIFICATION",&invid);
				ZERNRD(dbsort->fileno);
				FINDFLDSETSTRING(dbsort->fileno,"VENDOR IDENTIFICATION",vendorid);
				FINDFLDSETINT(dbsort->fileno,"CALENDAR YEAR",venyrid);
				FINDFLDSETSTRING(dbsort->fileno,"INVOICE IDENTIFICATION",invid);
				FINDFLDSETINT(dbsort->fileno,"TRANSACTION NUMBER",transno);
				if(EQLNRD(dbsort->fileno,1))
				{	
					KEYNRD(dbsort->fileno,1);
					FINDRSCSETSTRING(mtnrsrc,"PO IDENTIFICATION",poid);
					FINDRSCSETINT(mtnrsrc,"FISCAL YEAR",yearid);
					field=FLDNRD(vchnum,"ENTRY DATE");
					if(field!=NULL) 
					{
						if(field->len<10) date=GETCURRENTDATE();
						else date=GETCURRENTDATE10();
					} else date=GETCURRENTDATE();
					timex=GETCURRENTTIME();
					FINDRSCSETSTRING(mtnrsrc,"ENTRY DATE",date);
					FINDRSCSETSTRING(mtnrsrc,"ENTRY TIME",timex);
					FINDRSCSETSTRING(mtnrsrc,"SOURCE USER",USERLOGIN);
					setACCTtype(mtnrsrc,mtn);
				} else {
					getrecordsetedit(mtnrsrc,mtn);
					GetRDATData(dbsort->fileno,mtn->previous);
				}
			} else {
				ZERNRD(dbsort->fileno);
				FINDRSCSETSTRING(mtnrsrc,"VENDOR IDENTIFICATION",vendorid);
				FINDRSCSETINT(mtnrsrc,"CALENDAR YEAR",venyrid);
				FINDRSCSETSTRING(mtnrsrc,"INVOICE IDENTIFICATION",invid);
				FINDRSCSETSTRING(mtnrsrc,"PO IDENTIFICATION",poid);
				FINDRSCSETINT(mtnrsrc,"FISCAL YEAR",yearid);
				FINDFLDSETSTRING(dbsort->fileno,"VENDOR IDENTIFICATION",vendorid);
				FINDFLDSETINT(dbsort->fileno,"CALENDAR YEAR",venyrid);
				FINDFLDSETSTRING(dbsort->fileno,"INVOICE IDENTIFICATION",invid);
				FINDFLDSETSTRING(dbsort->fileno,"PO IDENTIFICATION",poid);
				FINDFLDSETINT(dbsort->fileno,"FISCAL YEAR",yearid);
				timex=GETCURRENTTIME();
				field=FLDNRD(vchnum,"ENTRY DATE");
				if(field!=NULL) 
				{
					if(field->len<10) date=GETCURRENTDATE();
					else date=GETCURRENTDATE10();
				} else date=GETCURRENTDATE();
				FINDRSCSETSTRING(mtnrsrc,"ENTRY DATE",date);
				FINDRSCSETSTRING(mtnrsrc,"ENTRY TIME",timex);
				FINDRSCSETSTRING(mtnrsrc,"SOURCE USER",USERLOGIN);
				FINDRSCGETSTRING(mtnrsrc,"PO IDENTIFICATION",&temp);
				FINDFLDSETSTRING(dbsort->fileno,"PO IDENTIFICATION",temp);
				FINDRSCGETSTRING(mtnrsrc,"SITE IDENTIFICATION",&temp);
				FINDFLDSETSTRING(dbsort->fileno,"SITE IDENTIFICATION",temp);
				FINDRSCGETSTRING(mtnrsrc,"BANK IDENTIFICATION",&temp);
				FINDFLDSETSTRING(dbsort->fileno,"BANK IDENTIFICATION",temp);
				FINDRSCGETINT(mtnrsrc,"FISCAL YEAR",&tmpnum);
				FINDFLDSETINT(dbsort->fileno,"FISCAL YEAR",tmpnum);
				setACCTtype(mtnrsrc,mtn);
			}
			break;
		case 0:
		default:
			if(EQLNRDKEY(dbsort->fileno,1,targetkey))
			{
				ZERNRD(dbsort->fileno);
				FINDRSCSETSTRING(mtnrsrc,"VENDOR IDENTIFICATION",vendorid);
				FINDRSCSETINT(mtnrsrc,"FISCAL YEAR",venyrid);
				FINDRSCSETSTRING(mtnrsrc,"INVOICE IDENTIFICATION",invid);
				FINDFLDSETSTRING(dbsort->fileno,"VENDOR IDENTIFICATION",vendorid);
				FINDFLDSETINT(dbsort->fileno,"CALENDAR YEAR",venyrid);
				FINDFLDSETSTRING(dbsort->fileno,"INVOICE IDENTIFICATION",invid);
				timex=GETCURRENTTIME();
				field=FLDNRD(vchnum,"ENTRY DATE");
				if(field!=NULL) 
				{
					if(field->len<10) date=GETCURRENTDATE();
					else date=GETCURRENTDATE10();
				} else date=GETCURRENTDATE();
				FINDRSCSETSTRING(mtnrsrc,"ENTRY DATE",date);
				FINDRSCSETSTRING(mtnrsrc,"ENTRY TIME",timex);
				FINDRSCSETSTRING(mtnrsrc,"SOURCE USER",USERLOGIN);
				FINDRSCSETSTRING(mtnrsrc,"PO IDENTIFICATION",poid);
				FINDFLDSETSTRING(dbsort->fileno,"PO IDENTIFICATION",poid);
				FINDRSCGETSTRING(mtnrsrc,"SITE IDENTIFICATION",&temp);
				FINDFLDSETSTRING(dbsort->fileno,"SITE IDENTIFICATION",temp);
				FINDRSCGETSTRING(mtnrsrc,"BANK IDENTIFICATION",&temp);
				FINDFLDSETSTRING(dbsort->fileno,"BANK IDENTIFICATION",temp);
				FINDRSCSETINT(mtnrsrc,"FISCAL YEAR",yearid);
				FINDFLDSETINT(dbsort->fileno,"FISCAL YEAR",yearid);
				setACCTtype(mtnrsrc,mtn);
			} else {
				GetRDATData(dbsort->fileno,mtn->previous);
				getrecordsetedit(mtnrsrc,mtn);
			}
			break;
	}
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
	if(temp!=NULL) Rfree(temp);
	getsupportingrecords(mtnrsrc,dbsort,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
	if(mbl_rec!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
	}
	if(mbl_rec==NULL)
	{
		addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_purapv,NULL);
	}
	if(venpmt->Use_Taxes==TRUE)
	{
		totalcalc(mtnrsrc);
	}
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mtnrsrc);
/* APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_rec==NULL?TRUE:FALSE)); */
	if(xmakescrn(mtnrsrc,TRUE,PURAPV_GUIEVAL,PSTRUCT,addoswidgets,PURAPV_GUIEVALSTR,PSTRUCT,
		PurApvSubData,mtn->p,NULL,__LINE__,__FILE__))
	{
		edesc=Rmalloc(180+RDAstrlen(mtnrsrc->module)+RDAstrlen(mtnrsrc->screen));
		sprintf(edesc,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",(mtnrsrc->module==NULL?"":mtnrsrc->module),(mtnrsrc->screen==NULL?"":mtnrsrc->screen));
		ERRORDIALOG("MAKESCRN FAILED",edesc,ShutdownOnError,(exit ? TRUE:FALSE));
		if(edesc!=NULL) Rfree(edesc);
		quit_record(mtnrsrc,mtn);
	}
}
static void seteditable(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char dist=FALSE;
	int accttype=0;

	FINDRSCGETCHAR(mtnrsrc,"DISTRIBUTED",&dist);
	if(dist)
	{
		updatesingleeditfilersrcs(dbsort->fileno,mtnrsrc,FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"TRANSACTION NUMBER",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"ACCOUNT TYPES",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"DEFINITION TYPES",FALSE);
		if(venpmt->use_venpmt_fund)
		{
			FINDRSCSETEDITABLE(mtnrsrc,"CLEARING DEFINITION TYPES",FALSE);
		}
		FINDRSCSETEDITABLE(mtnrsrc,"PROCESSING MONTHS",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"FISCAL PROCESSING MONTHS",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"CLEAR PROCESSING MONTHS",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"DELETEFLAG",TRUE);
	} else {
		editsinglefilersrcsnokeys(dbsort->fileno,1,mtnrsrc,TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"PO IDENTIFICATION",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"FISCAL YEAR",FALSE);
		FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&accttype);
		if(accttype==BAL_ACCT)
		{
			FINDRSCSETEDITABLE(mtnrsrc,"DEBIT CODE",FALSE);
		}
		FINDRSCSETEDITABLE(mtnrsrc,"ACCOUNT TYPES",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"DEFINITION TYPES",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"PROCESSING MONTHS",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"FISCAL PROCESSING MONTHS",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"CLEAR PROCESSING MONTHS",TRUE);
		if(venpmt->use_venpmt_fund)
		{
			FINDRSCSETEDITABLE(mtnrsrc,"CLEARING DEFINITION TYPES",TRUE);
		}
	}
}
static void getrecordsetedit(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int which_type=0;
	short x=0;
	short accttype=0;

	singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);
	FINDFLDGETSHORT(dbsort->fileno,"ACCOUNT TYPE",&accttype);
	which_type=(int)accttype;
	FINDRSCSETINT(mtnrsrc,"ACCOUNT TYPES",which_type);
	FINDFLDGETSHORT(dbsort->fileno,"DEFINITION TYPE",&x);
	which_type=(int)x;
	FINDRSCSETINT(mtnrsrc,"DEFINITION TYPES",which_type);
	if(venpmt->use_venpmt_fund)
	{
		FINDFLDGETSHORT(dbsort->fileno,"CLEARING DEFINITION TYPE",&x);
		which_type=x;
		FINDRSCSETINT(mtnrsrc,"CLEARING DEFINITION TYPES",which_type);
	}
	setpmonth(mtnrsrc,mtn);
	updatersrc(mtnrsrc,"PROCESSING MONTHS");
	updatersrc(mtnrsrc,"FISCAL PROCESSING MONTHS");
	updatersrc(mtnrsrc,"CLEAR PROCESSING MONTHS");
	setACCTtype(mtnrsrc,mtn);
	updatersrc(mtnrsrc,"ACCOUNT TYPES");
	seteditable(mtnrsrc,mtn);
}
static void clearwidget(RDArsrc *mtnrsrc,char *widgetname)
{
#ifdef CLEARWDGT
	FINDRSCSETSTRING(mtnrsrc,widgetname,NULL);
	updatersrc(mtnrsrc,widgetname);
#endif
}
static short check_code(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	short accttype1=0,deftype1=0,cdeftype1=0;
	int selaccttype=0,seldeftype=0,procyrid=0,selcdeftype=0;
	char *acctcode1=NULL,*debcode1=NULL,*credcode1=NULL,*tmp=NULL;
	char *paycode1=NULL;
/*
	char *dueto1=NULL;
*/
	char *clearfrom1=NULL,*clearcash1=NULL;

	FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selaccttype);
	accttype1=(short)selaccttype;
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&seldeftype);
	deftype1=(short)seldeftype;
	if(venpmt->use_venpmt_fund)
	{
		FINDRSCGETINT(mtnrsrc,"CLEARING DEFINITION TYPES",&selcdeftype);
		cdeftype1=(short)selcdeftype;
/*
		readwidget(mtnrsrc,"DUE TO");
		FINDRSCGETSTRING(mtnrsrc,"DUE TO",&dueto1);
*/
		readwidget(mtnrsrc,"CLEARING DUE FROM");
		FINDRSCGETSTRING(mtnrsrc,"CLEARING DUE FROM",&clearfrom1);
		readwidget(mtnrsrc,"CLEARING CASH");
		FINDRSCGETSTRING(mtnrsrc,"CLEARING CASH",&clearcash1);
	}
	readwidget(mtnrsrc,"FISCAL YEAR");
	FINDRSCGETINT(mtnrsrc,"FISCAL YEAR",&procyrid);
	readwidget(mtnrsrc,"ACCOUNT CODE");
	FINDRSCGETSTRING(mtnrsrc,"ACCOUNT CODE",&acctcode1);
	readwidget(mtnrsrc,"DEBIT CODE");
	FINDRSCGETSTRING(mtnrsrc,"DEBIT CODE",&debcode1);
	readwidget(mtnrsrc,"CREDIT CODE");
	FINDRSCGETSTRING(mtnrsrc,"CREDIT CODE",&credcode1);
	readwidget(mtnrsrc,"PAYABLE CODE");
	FINDRSCGETSTRING(mtnrsrc,"PAYABLE CODE",&paycode1);
	switch(mtn->acctwidget)
	{
		case 0:
			switch(accttype1)
			{
				case REV_ACCT:
					if((tmp=ADVCHECKACCT(ryrnum,revnum,acctcode1,accttype1,deftype1,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID REVENUE ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
						clearwidget(mtnrsrc,"ACCOUNT CODE");
						if(acctcode1!=NULL) Rfree(acctcode1);
						if(credcode1!=NULL) Rfree(credcode1);
						if(debcode1!=NULL) Rfree(debcode1);
						if(paycode1!=NULL) Rfree(paycode1);
						if(venpmt->use_venpmt_fund)
						{
/*
							if(dueto1!=NULL) Rfree(dueto1);
*/
							if(clearfrom1!=NULL) Rfree(clearfrom1);	
							if(clearcash1!=NULL) Rfree(clearcash1);
						}
						Rfree(tmp);
						return(FALSE);
					}
					break;
				case BAL_ACCT:
					if((tmp=ADVCHECKACCT(byrnum,balnum,acctcode1,accttype1,deftype1,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
						clearwidget(mtnrsrc,"ACCOUNT CODE");
						if(acctcode1!=NULL) Rfree(acctcode1);
						if(credcode1!=NULL) Rfree(credcode1);
						if(debcode1!=NULL) Rfree(debcode1);
						if(paycode1!=NULL) Rfree(paycode1);
						if(venpmt->use_venpmt_fund)
						{
/*
							if(dueto1!=NULL) Rfree(dueto1);
*/
							if(clearfrom1!=NULL) Rfree(clearfrom1);	
							if(clearcash1!=NULL) Rfree(clearcash1);
						}
						Rfree(tmp);
						return(FALSE);
					}
					break;
				case EXP_ACCT:
					if((tmp=ADVCHECKACCT(eyrnum,expnum,acctcode1,accttype1,deftype1,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID EXPENDITURE ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
						clearwidget(mtnrsrc,"ACCOUNT CODE");
						if(acctcode1!=NULL) Rfree(acctcode1);
						if(credcode1!=NULL) Rfree(credcode1);
						if(debcode1!=NULL) Rfree(debcode1);
						if(paycode1!=NULL) Rfree(paycode1);
						if(venpmt->use_venpmt_fund)
						{
/*
							if(dueto1!=NULL) Rfree(dueto1);
*/
							if(clearfrom1!=NULL) Rfree(clearfrom1);	
							if(clearcash1!=NULL) Rfree(clearcash1);
						}
						Rfree(tmp);
						return(FALSE);
					}
					break;
				default :
					prterr("Invalid Account Type [%d]",accttype1);
					break;
			}
			break;
		case 1:
			switch(accttype1)
			{
				case REV_ACCT :
					if(debcode1!=NULL && RDAstrlen(debcode1)>0)
					{
						if((tmp=ADVCHECKACCT(byrnum,balnum,debcode1,accttype1,deftype1,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!",tmp,NULL,NULL,TRUE,NULL);
							clearwidget(mtnrsrc,"DEBIT CODE");
							if(acctcode1!=NULL) Rfree(acctcode1);
							if(credcode1!=NULL) Rfree(credcode1);
							if(debcode1!=NULL) Rfree(debcode1);
							if(paycode1!=NULL) Rfree(paycode1);
							if(venpmt->use_venpmt_fund)
							{
/*
								if(dueto1!=NULL) Rfree(dueto1);
*/
								if(clearfrom1!=NULL) Rfree(clearfrom1);	
								if(clearcash1!=NULL) Rfree(clearcash1);
							}
							Rfree(tmp);
							return(FALSE);
						}
					}
					break;
				case EXP_ACCT:
					if((tmp=ADVCHECKACCT(byrnum,balnum,debcode1,accttype1,deftype1,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!",tmp,NULL,NULL,TRUE,NULL);
						clearwidget(mtnrsrc,"DEBIT CODE");
						if(acctcode1!=NULL) Rfree(acctcode1);
						if(credcode1!=NULL) Rfree(credcode1);
						if(debcode1!=NULL) Rfree(debcode1);
						if(paycode1!=NULL) Rfree(paycode1);
						if(venpmt->use_venpmt_fund)
						{
/*
							if(dueto1!=NULL) Rfree(dueto1);
*/
							if(clearfrom1!=NULL) Rfree(clearfrom1);	
							if(clearcash1!=NULL) Rfree(clearcash1);
						}
						Rfree(tmp);
						return(FALSE);
					}
					break;
				default :
					break;
			} 
			break;
		case 2:
			switch(accttype1)
			{
				case EXP_ACCT :
					if(credcode1!=NULL && RDAstrlen(credcode1)>0)
					{
						if((tmp=ADVCHECKACCT(byrnum,balnum,credcode1,accttype1,deftype1,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE!",tmp,NULL,NULL,TRUE,NULL);
							clearwidget(mtnrsrc,"CREDIT CODE");
							if(acctcode1!=NULL) Rfree(acctcode1);
							if(credcode1!=NULL) Rfree(credcode1);
							if(debcode1!=NULL) Rfree(debcode1);
							if(paycode1!=NULL) Rfree(paycode1);
							if(venpmt->use_venpmt_fund)
							{
/*
								if(dueto1!=NULL) Rfree(dueto1);
*/
								if(clearfrom1!=NULL) Rfree(clearfrom1);	
								if(clearcash1!=NULL) Rfree(clearcash1);
							}
							Rfree(tmp);
							return(FALSE);
						}
					}
					break;
				case BAL_ACCT :
				case REV_ACCT :
					if((tmp=ADVCHECKACCT(byrnum,balnum,credcode1,accttype1,deftype1,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE!",tmp,NULL,NULL,TRUE,NULL);
						clearwidget(mtnrsrc,"CREDIT CODE");
						if(acctcode1!=NULL) Rfree(acctcode1);
						if(credcode1!=NULL) Rfree(credcode1);
						if(debcode1!=NULL) Rfree(debcode1);
						if(paycode1!=NULL) Rfree(paycode1);
						if(venpmt->use_venpmt_fund)
						{
/*
							if(dueto1!=NULL) Rfree(dueto1);
*/
							if(clearfrom1!=NULL) Rfree(clearfrom1);	
							if(clearcash1!=NULL) Rfree(clearcash1);
						}
						Rfree(tmp);
						return(FALSE);
					}
					break;
				default:
					prterr("Invalid Account Type [%d]",accttype1);
					break;
			}
			break;
		case 3:
			if((tmp=ADVCHECKACCT(byrnum,balnum,paycode1,accttype1,deftype1,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PAYABLE CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(mtnrsrc,"PAYABLE CODE");
				if(acctcode1!=NULL) Rfree(acctcode1);
				if(credcode1!=NULL) Rfree(credcode1);
				if(paycode1!=NULL) Rfree(paycode1); 
				if(debcode1!=NULL) Rfree(debcode1);
				if(venpmt->use_venpmt_fund)
				{
/*
					if(dueto1!=NULL) Rfree(dueto1);
*/
					if(clearfrom1!=NULL) Rfree(clearfrom1);	
					if(clearcash1!=NULL) Rfree(clearcash1);
				}
				Rfree(tmp);
				return(FALSE);
			} else {
				if((tmp=ADVGETBALREF(balnum,paycode1,deftype1,SCRNvirtualSubData,mtnrsrc))!=NULL)
				{
					if(RDAstrcmp(tmp,"ACCOUNTS PAYABLE"))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PAYABLE CODE!","The PAYABLE CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of [ACCOUNTS PAYABLE]." ,NULL,NULL,TRUE,NULL);
						clearwidget(mtnrsrc,"PAYABLE CODE");
						if(acctcode1!=NULL) Rfree(acctcode1);
						if(credcode1!=NULL) Rfree(credcode1);
						if(debcode1!=NULL) Rfree(debcode1);
						if(paycode1!=NULL) Rfree(paycode1);
						if(venpmt->use_venpmt_fund)
						{
/*
							if(dueto1!=NULL) Rfree(dueto1);
*/
							if(clearfrom1!=NULL) Rfree(clearfrom1);	
							if(clearcash1!=NULL) Rfree(clearcash1);
						}
						Rfree(tmp);
						return(FALSE);
					}
				}
			}
			break;
/*
		case 4: - due to -
			if(venpmt->use_venpmt_fund)
			{
			if((tmp=ADVCHECKACCT(byrnum,balnum,dueto1,accttype1,deftype1,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DUE TO!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(mtnrsrc,"DUE TO");
				if(acctcode1!=NULL) Rfree(acctcode1);
				if(credcode1!=NULL) Rfree(credcode1);
				if(paycode1!=NULL) Rfree(paycode1); 
				if(debcode1!=NULL) Rfree(debcode1);
				if(dueto1!=NULL) Rfree(dueto1);
				if(clearfrom1!=NULL) Rfree(clearfrom1);	
				if(clearcash1!=NULL) Rfree(clearcash1);
				Rfree(tmp);
				return(FALSE);
			} else {
				if((tmp=ADVGETBALREF(balnum,dueto1,deftype1,SCRNvirtualSubData,mtnrsrc))!=NULL)
				{
					if(RDAstrcmp(tmp,"DUE TO"))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DUE TO!","The DUE TO entered does not have the correct BALANCE SHEET REFERENCE TYPE of [DUE TO]." ,NULL,NULL,TRUE,NULL);
						clearwidget(mtnrsrc,"DUE TO");
						if(acctcode1!=NULL) Rfree(acctcode1);
						if(credcode1!=NULL) Rfree(credcode1);
						if(debcode1!=NULL) Rfree(debcode1);
						if(paycode1!=NULL) Rfree(paycode1);
						if(dueto1!=NULL) Rfree(dueto1);
						if(clearfrom1!=NULL) Rfree(clearfrom1);	
						if(clearcash1!=NULL) Rfree(clearcash1);
						Rfree(tmp);
						return(FALSE);
					}
				}
			}
			}
			break;
*/
		case 5: /* clear due to */
			if(venpmt->use_venpmt_fund)
			{
			if(!isEMPTY(clearfrom1))
			{
			if((tmp=ADVCHECKACCT(byrnum,balnum,clearfrom1,accttype1,cdeftype1,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CLEARING DUE FROM!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(mtnrsrc,"CLEARING DUE FROM");
				if(acctcode1!=NULL) Rfree(acctcode1);
				if(credcode1!=NULL) Rfree(credcode1);
				if(paycode1!=NULL) Rfree(paycode1); 
				if(debcode1!=NULL) Rfree(debcode1);
/*
				if(dueto1!=NULL) Rfree(dueto1);
*/
				if(clearfrom1!=NULL) Rfree(clearfrom1);	
				if(clearcash1!=NULL) Rfree(clearcash1);
				Rfree(tmp);
				return(FALSE);
			} else {
				if((tmp=ADVGETBALREF(balnum,clearfrom1,cdeftype1,SCRNvirtualSubData,mtnrsrc))!=NULL)
				{
					if(RDAstrcmp(tmp,"DUE FROM"))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CLEARING DUE FROM!","The CLEARING DUE FROM entered does not have the correct BALANCE SHEET REFERENCE TYPE of [DUE TO]." ,NULL,NULL,TRUE,NULL);
						clearwidget(mtnrsrc,"CLEARING DUE FROM");
						if(acctcode1!=NULL) Rfree(acctcode1);
						if(credcode1!=NULL) Rfree(credcode1);
						if(debcode1!=NULL) Rfree(debcode1);
						if(paycode1!=NULL) Rfree(paycode1);
/*
						if(dueto1!=NULL) Rfree(dueto1);
*/
						if(clearfrom1!=NULL) Rfree(clearfrom1);	
						if(clearcash1!=NULL) Rfree(clearcash1);
						Rfree(tmp);
						return(FALSE);
					}
				}
			}
			}
			}
			break;
		case 6: /* clear cash */
			if(venpmt->use_venpmt_fund)
			{
			if(!isEMPTY(clearcash1))
			{
			if((tmp=ADVCHECKACCT(byrnum,balnum,clearcash1,accttype1,cdeftype1,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CLEARING CASH!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(mtnrsrc,"CLEARING CASH");
				if(acctcode1!=NULL) Rfree(acctcode1);
				if(credcode1!=NULL) Rfree(credcode1);
				if(paycode1!=NULL) Rfree(paycode1); 
				if(debcode1!=NULL) Rfree(debcode1);
/*
				if(dueto1!=NULL) Rfree(dueto1);
*/
				if(clearfrom1!=NULL) Rfree(clearfrom1);	
				if(clearcash1!=NULL) Rfree(clearcash1);
				Rfree(tmp);
				return(FALSE);
			} else {
				if((tmp=ADVGETBALREF(balnum,clearcash1,cdeftype1,SCRNvirtualSubData,mtnrsrc))!=NULL)
				{
					if(RDAstrcmp(tmp,"CASH"))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CLEARING CASH!","The CLEARING CASH entered does not have the correct BALANCE SHEET REFERENCE TYPE of [CASH]." ,NULL,NULL,TRUE,NULL);
						clearwidget(mtnrsrc,"CLEARING CASH");
						if(acctcode1!=NULL) Rfree(acctcode1);
						if(credcode1!=NULL) Rfree(credcode1);
						if(debcode1!=NULL) Rfree(debcode1);
						if(paycode1!=NULL) Rfree(paycode1);
/*
						if(dueto1!=NULL) Rfree(dueto1);
*/
						if(clearfrom1!=NULL) Rfree(clearfrom1);	
						if(clearcash1!=NULL) Rfree(clearcash1);
						Rfree(tmp);
						return(FALSE);
					}
				}
			}
			}
			}
			break;
		default:
			prterr("\"mtn->acctwidget\" value of [%d] out of range.",mtn->acctwidget);
			break;
	}
	if(acctcode1!=NULL) Rfree(acctcode1);
	if(credcode1!=NULL) Rfree(credcode1);
	if(debcode1!=NULL) Rfree(debcode1);
	if(paycode1!=NULL) Rfree(paycode1);
	if(venpmt->use_venpmt_fund)
	{
/*
		if(dueto1!=NULL) Rfree(dueto1);
*/
		if(clearfrom1!=NULL) Rfree(clearfrom1);	
		if(clearcash1!=NULL) Rfree(clearcash1);
	}
	return(TRUE);
}
static void acctcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *bs=NULL;
	char *a=NULL,*temp=NULL;
	DFvirtual *d=NULL;

	readwidget(mtnrsrc,"ACCOUNT CODE");
	FINDRSCGETSTRING(mtnrsrc,"ACCOUNT CODE",&a);
	d=getDFvirtual("PURORD","B/S EXPENDITURE");
	if(d!=NULL)
	{
		bs=EVALUATEstr(d->expression,PurApvSubData,PSTRUCT);
		temp=Rmalloc(41);
		sprintf(temp,"%.4s%s",a,(bs!=NULL ?bs:""));
		FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE",temp);
		if(temp!=NULL) Rfree(temp);
		updatersrc(mtnrsrc,"DEBIT CODE");
		if(bs!=NULL) Rfree(bs);
	} else { 
		prterr("Error Virtual Not found! "); 
	}

	d=getDFvirtual("PURORD","B/S CASH");
	if(d!=NULL)
	{
		bs=EVALUATEstr(d->expression,PurApvSubData,PSTRUCT);
		temp=Rmalloc(41);
		sprintf(temp,"%.4s%s",a,(bs!=NULL ?bs:""));
		FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE",temp);
		updatersrc(mtnrsrc,"CREDIT CODE");
		if(temp!=NULL) Rfree(temp);
		if(bs!=NULL) Rfree(bs);
	} else { 
		prterr("Error Virtual Not found! "); 
	}

	d=getDFvirtual("PURORD","B/S ACCOUNTS PAYABLE");
	if(d!=NULL)
	{
		bs=EVALUATEstr(d->expression,PurApvSubData,PSTRUCT);
		temp=Rmalloc(41);
		sprintf(temp,"%.4s%s",a,(bs!=NULL ?bs:""));
		FINDRSCSETSTRING(mtnrsrc,"PAYABLE CODE",temp);
		if(temp!=NULL) Rfree(temp);
		updatersrc(mtnrsrc,"PAYABLE CODE");
		if(bs!=NULL) Rfree(bs);
	} else { 
		prterr("Error Virtual Not found! ");
	}
	check_acctcode(mtnrsrc,mtn);
}
static short check_acctcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	mtn->acctwidget=0;
	return(check_code(mtnrsrc,mtn));
}
static void debcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	check_debcode(mtnrsrc,mtn);
}
static short check_debcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	mtn->acctwidget=1;
	return(check_code(mtnrsrc,mtn));
}
/*
static short check_dueto(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	mtn->acctwidget=4;
	return(check_code(mtnrsrc,mtn));
}
static void duetocb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	check_dueto(mtnrsrc,mtn);
}
*/
static short check_clearfrom(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	mtn->acctwidget=5;
	return(check_code(mtnrsrc,mtn));
}
static void clearfromcb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	check_clearfrom(mtnrsrc,mtn);
}
static short check_clearcash(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	mtn->acctwidget=6;
	return(check_code(mtnrsrc,mtn));
}
static void clearcashcb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	check_clearcash(mtnrsrc,mtn);
}
static void credcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	check_credcode(mtnrsrc,mtn);
}
static short check_credcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	mtn->acctwidget=2;
	return(check_code(mtnrsrc,mtn));
}
static void paycodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	check_paycode(mtnrsrc,mtn);
}
static short check_paycode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	mtn->acctwidget=3;
	return(check_code(mtnrsrc,mtn));
}
static void save_record_up(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	mtn->update_list=TRUE;
	save_check(mtnrsrc,mtn);
}
static void save_record_nup(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	mtn->update_list=FALSE;
	save_check(mtnrsrc,mtn);
}
static void save_check(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int procyrid=0;
	char *tmp=NULL,active=FALSE,delflag=FALSE;

	readallwidgets(mtnrsrc);
	FINDRSCGETCHAR(mtnrsrc,"DELETEFLAG",&delflag);
	if(!delflag)
	{
	if(!FINDRSCGETINT(mtnrsrc,"FISCAL YEAR",&procyrid))
	{
		FINDFLDSETINT(finyrnum,"FISCAL YEAR",procyrid);
		KEYNRD(finyrnum,1);
		if(!ADVEQLNRDsec(finyrnum,1,SCRNvirtualSubData,mtnrsrc))
		{
			FINDFLDGETCHAR(finyrnum,"ACTIVE",&active);
			if(active==FALSE)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","NON-ACTIVE FISCAL YEAR!","The FISCAL YEAR entered is not active.\nIt exist as a FINANCIAL YEAR but is presently set up as inactive.\n",NULL,NULL,TRUE,NULL);
				prterr("The FISCAL YEAR entered is not active.  It exist as a FINANCIAL YEAR but is presently set up as inactive.");
				return;
			}
			FINDFLDGETCHAR(finyrnum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED FISCAL YEAR!","The FISCAL YEAR entered is deleted.\nIt exist as a FINANCIALYEAR but is presently set up in a deletion state.\n",NULL,NULL,TRUE,NULL);
				prterr("The FISCAL YEAR entered is deleted. It exist as a FINANCIAL YEAR but is presently set up in a deletion state.");
				return;
			}
		} else {
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID FISCAL YEAR!","The FISCAL YEAR entered is not valid.\nIt does not exist as a FINANCIAL YEAR.\n",NULL,NULL,TRUE,NULL);
			return;
		}
	}
	readwidget(mtnrsrc,"VENDOR IDENTIFICATION");
	if(tmp!=NULL) Rfree(tmp);
	if(!FINDRSCGETSTRING(mtnrsrc,"VENDOR IDENTIFICATION",&tmp))
	{
		FINDFLDSETSTRING(vennum,"VENDOR IDENTIFICATION",tmp);
		KEYNRD(vennum,1);
		if(!ADVEQLNRDsec(vennum,1,SCRNvirtualSubData,mtnrsrc))
		{
			FINDFLDGETCHAR(vennum,"DELETEFLAG",&delflag);
			if(delflag==TRUE)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED VENDOR IDENTIFICATION!","The VENDOR IDENTIFICATION entered is deleted.\nIt exist as a VENDOR but is presently set up in a deletion state.\n",NULL,NULL,TRUE,NULL);
				prterr("The VENDOR IDENTIFICATION entered is deleted.  It exist as a VENDOR but is presently set up in a deletion state.");
				if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
				return;
			}
		} else {
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID VENDOR IDENTIFICATION!","The VENDOR IDENTIFICATION entered is not valid.\nIt does not exist as a VENDOR.\n",NULL,NULL,TRUE,NULL);
			if(tmp!=NULL) Rfree(tmp);
			return;
		}
	}
	if(tmp!=NULL) Rfree(tmp);
	if(check_acctcode(mtnrsrc,mtn)==TRUE)
	{
		if(check_debcode(mtnrsrc,mtn)==TRUE)
		{
			if(check_credcode(mtnrsrc,mtn)==TRUE) 
			{
				if(check_paycode(mtnrsrc,mtn)==TRUE)
				{
					if(check_bankid(mtnrsrc,mtn)==FALSE) return;
				} else return;
			} else return;
		} else return;
	} else return;
	if(venpmt->use_venpmt_fund)
	{
/*
		if(check_dueto(mtnrsrc,mtn)==FALSE) return;
*/
		if(check_clearfrom(mtnrsrc,mtn)==FALSE) return;
		if(check_clearcash(mtnrsrc,mtn)==FALSE) return;
	}
	if(testavailbalance(mtnrsrc,mtn)==FALSE) return;
	}
	save_record(mtnrsrc,mtn);
}
/*
static void check_availbalance(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	testavailbalance(mtnrsrc,mtn);
}
*/
/* this function also need to account for the amounts in the sort file themselves */
static short testavailbalance(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selecteda=0,selectedd=0,selperiod=0;
	char *period=NULL,*header=NULL,*message=NULL;
	char *vendorid1=NULL,*vendorid2=NULL,*invid1=NULL,*invid2=NULL;
	char *accountcode1=NULL,*accountcode2=NULL;
	char delflag=FALSE;
	short deftype1=0,deftype2=0,accttype1=0,accttype2=0,ef=0,testavl=TRUE;
	short procmo1=0,procmo2=0;
	int procyrid1=0,procyrid2=0,yearid1=0,yearid2,transno1=0,transno2;
	double amount=0.0,tempamt=0.0,total_amount=0.0;

	readallwidgets(mtnrsrc);
	if(FINDRSCGETSTRING(mtnrsrc,"ACCOUNT CODE",&accountcode1)) return(FALSE);
	if(FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selecteda)) return(FALSE);
	if(FINDRSCGETCHAR(mtnrsrc,"DELETEFLAG",&delflag)) return(FALSE);
	if(delflag==FALSE)
	{
	if(selecteda==EXP_ACCT)
	{
		if(FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selectedd)) return(FALSE);
		deftype1=(short)selectedd;
		accttype1=(short)selecteda;
		if(venpmt->use_fiscal_months==TRUE)
		{
			if(FINDRSCGETINT(mtnrsrc,"FISCAL PROCESSING MONTHS",&selperiod)) return(FALSE);
		} else {
			if(FINDRSCGETINT(mtnrsrc,"PROCESSING MONTHS",&selperiod)) return(FALSE);
			selperiod+=1;
		}
		period=stralloc(FISCALPERIOD[selperiod]);
/*
		period=stralloc(mtn->cpmonth->libs[selperiod]);
		procmo1=(short)RRatoi(period);
*/
		procmo1=selperiod;
		if(FINDRSCGETDOUBLE(mtnrsrc,"AMOUNT",&amount)) return(FALSE);
		if(FINDRSCGETINT(mtnrsrc,"FISCAL YEAR",&procyrid1)) return(FALSE);
		if(vendorid1!=NULL) Rfree(vendorid1);
		FINDRSCGETSTRING(mtnrsrc,"VENDOR IDENTIFICATION",&vendorid1);
		FINDRSCGETINT(mtnrsrc,"CALENDAR YEAR",&yearid1);
		if(invid1!=NULL) Rfree(invid1);
		FINDRSCGETSTRING(mtnrsrc,"INVOICE IDENTIFICATION",&invid1);
		FINDRSCGETINT(mtnrsrc,"TRANSACTION NUMBER",&transno1);
		total_amount=amount;
		ZERNRD(dbsort->fileno);
		ef=FRSNRD(dbsort->fileno,1);
		while(!ef)
		{
			FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&delflag);
			if(delflag==FALSE)
			{
				if(vendorid2!=NULL) Rfree(vendorid2);
				FINDFLDGETSTRING(dbsort->fileno,"VENDOR IDENTIFICATION",&vendorid2);
				FINDFLDGETINT(dbsort->fileno,"CALENDAR YEAR",&yearid2);
				if(invid2!=NULL) Rfree(invid2);
				FINDFLDGETSTRING(dbsort->fileno,"INVOICE IDENTIFICATION",&invid2);
				FINDFLDGETINT(dbsort->fileno,"TRANSACTION NUMBER",&transno2);
				FINDFLDGETSHORT(dbsort->fileno,"ACCOUNT TYPE",&accttype2);
				if(accttype2==accttype1)
				{
					FINDFLDGETSHORT(dbsort->fileno,"DEFINITION TYPE",&deftype2);
					if(accountcode2!=NULL) Rfree(accountcode2); 
					FINDFLDGETSTRING(dbsort->fileno,"ACCOUNT CODE",&accountcode2);
					FINDFLDGETINT(dbsort->fileno,"FISCAL YEAR",&procyrid2);
					if(deftype2==deftype1 
						&& !RDAstrcmp(accountcode2,accountcode1)
						&& procyrid2==procyrid1)
					{
						if(!RDAstrcmp(vendorid2,vendorid1) 
							&& yearid2==yearid1 
							&& !RDAstrcmp(invid2,invid1) 
							&& transno2==transno1)
						{
/*
							FINDFLDGETDOUBLE(dbsort->fileno,"AMOUNT",&tempamt);
							if(finsetup->budget==ANNUALBUDGET)
							{
								total_amount-=tempamt;
							} else {
								FINDFLDGETSHORT(dbsort->fileno,"FISCAL PROCESSING MONTH",&procmo2);
								if(procmo2==procmo1)
								{
									total_amount-=tempamt;
								}
							}
*/
						} else {
							tempamt=0.0;
							FINDFLDGETDOUBLE(dbsort->fileno,"AMOUNT",&tempamt);
							if(finsetup->budget==ANNUALBUDGET)
							{
								total_amount+=tempamt;
							} else {
								if(venpmt->use_fiscal_months==TRUE)
								{
									FINDFLDGETSHORT(dbsort->fileno,"FISCAL PROCESSING MONTH",&procmo2);
								} else {
									FINDFLDGETSHORT(dbsort->fileno,"PROCESSING MONTH",&procmo2);
									procmo2+=1;
								}
								if(procmo2==procmo1)
								{
									total_amount+=tempamt;
								}
							}
						}
					}
				}
				FINDFLDGETSHORT(dbsort->fileno,"PO ACCOUNT TYPE",&accttype2);
				if(accttype2==accttype1)
				{
					FINDFLDGETSHORT(dbsort->fileno,"PO DEFINITION TYPE",&deftype2);
					if(accountcode2!=NULL) Rfree(accountcode2); 
					FINDFLDGETSTRING(dbsort->fileno,"PO ACCOUNT CODE",&accountcode2);
					FINDFLDGETINT(dbsort->fileno,"PO FISCAL YEAR",&procyrid2);
					if(deftype2==deftype1 
						&& !RDAstrcmp(accountcode2,accountcode1)
						&& procyrid2==procyrid1)
					{
						tempamt=0.0;
						FINDFLDGETDOUBLE(dbsort->fileno,"DISENCUMBRANCE AMOUNT",&tempamt);
						if(finsetup->budget==ANNUALBUDGET)
						{
							total_amount-=tempamt;
						} else {
							if(venpmt->use_fiscal_months==TRUE)
							{
								FINDFLDGETSHORT(dbsort->fileno,"FISCAL PROCESSING MONTH",&procmo2);
							} else {
								FINDFLDGETSHORT(dbsort->fileno,"PROCESSING MONTH",&procmo2);
								procmo2+=1;
							}
							if(procmo2==procmo1)
							{
								total_amount-=tempamt;
							}
						}
					}
				}
			}
			ef=NXTNRD(dbsort->fileno,1);
		}
		if(vendorid1!=NULL) Rfree(vendorid1);
		if(vendorid2!=NULL) Rfree(vendorid2);
		if(invid1!=NULL) Rfree(invid1);
		if(invid2!=NULL) Rfree(invid2);
		if(accountcode2!=NULL) Rfree(accountcode2);
		ZERNRD(eyrnum);
		FINDFLDSETINT(eyrnum,"FISCAL YEAR",procyrid1);
		FINDFLDSETSHORT(eyrnum,"DEFINITION TYPE",deftype1);
		FINDFLDSETSTRING(eyrnum,"ACCOUNT CODE",accountcode1);
		if(!ADVEQLNRDsec(eyrnum,1,SCRNvirtualSubData,mtnrsrc))
		{
			if(finsetup->budget==ANNUALBUDGET)
			{
				testavl=testapprotot(eyrnum,total_amount,NULL);
			} else {
				testavl=testapprotot(eyrnum,total_amount,period);
			}
			if(testavl==FALSE)
			{
				if(finsetup->budget==ANNUALBUDGET)
				{
					header=stralloc("This Voucher will cause the POSTED APPROPRIATED amount to be less than all of the POSTED, DISTRIBUTED, and UNDISTRIBUTED EXPENDITURES and ENCUMBRANCES entered for the ACCOUNT [%s] in the YEAR [%d].");
					message=Rmalloc(RDAstrlen(header)+RDAstrlen(accountcode1)+20);
					sprintf(message,header,accountcode1,procyrid1);
					message[RDAstrlen(header)+RDAstrlen(accountcode1)+6-1]=0;
				} else {
					header=stralloc("This Voucher will cause the POSTED APPROPRIATED amount to be less than all of the POSTED, DISTRIBUTED, and UNDISTRIBUTED EXPENDITURES and ENCUMBRANCES entered for the ACCOUNT CODE [%s], YEAR [%d], and MONTH [%d].");
					message=Rmalloc(RDAstrlen(header)+RDAstrlen(accountcode1)+25);
					sprintf(message,header,accountcode1,procyrid1,procmo1);
					message[RDAstrlen(header)+RDAstrlen(accountcode1)+9-1]=0;
				}
				ADVWARNINGDIALOG("WARNING DIALOG SCREEN","AVAILABLE BALANCE NOT ENOUGH!",message,(void(*)(...))save_record_warn,NULL,FALSE,2,(void *)mtnrsrc,(void *)mtn,NULL);
				if(header!=NULL) Rfree(header);
				if(message!=NULL) Rfree(message); 
				if(period!=NULL) Rfree(period);
				if(accountcode1!=NULL) Rfree(accountcode1);
				return(FALSE);
			} else {
				if(accountcode1!=NULL) Rfree(accountcode1); 
				return(TRUE);
			}
		} else {
			if(accountcode1!=NULL) Rfree(accountcode1); 
			return(FALSE);
		}
	}
	}
	if(accountcode1!=NULL) Rfree(accountcode1);
	return(TRUE);
}
static void save_record_warn(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_record(mtnrsrc,mtn);
}
static void save_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	short y=0,numflds=0,deftype1=0,source=1;
	NRDfield *tmpfld=NULL,*fldx=NULL,*tmp=NULL;
	double amount=0;
	char readdel=FALSE,manual_voucher=FALSE;
	int selectedd=0,checkno=0;

	readallwidgets(mtnrsrc);
	rsrc2singlefilerecord(dbsort->fileno,mtnrsrc);
	numflds=NUMFLDS(dbsort->fileno);
	if(numflds>0)
	{
		tmpfld=Rmalloc(numflds*sizeof(NRDfield));
		for(y=0,fldx=tmpfld;y<numflds;++y,++fldx)
		{
			tmp=FLDNUM(dbsort->fileno,y);
			cpfldinfo(fldx,tmp);
		}
		if(!EQLNRD(dbsort->fileno,1))
		{
			FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&readdel);
			if(readdel==FALSE)
			{
				FINDFLDGETDOUBLE(dbsort->fileno,"AMOUNT",&amount);
				total_po-=amount;
			}
		}
		fields2file(dbsort->fileno,tmpfld);
		FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&readdel);
		FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selectedd);
		deftype1=(short)selectedd;
		FINDFLDSETSHORT(dbsort->fileno,"DEFINITION TYPE",deftype1);
		FINDRSCGETINT(mtnrsrc,"CLEARING DEFINITION TYPES",&selectedd);
		deftype1=(short)selectedd;
		FINDFLDSETSHORT(dbsort->fileno,"CLEARING DEFINITION TYPE",deftype1);
		FINDFLDSETSHORT(dbsort->fileno,"SOURCE",source);
		FINDRSCGETCHAR(mtnrsrc,"MANUAL VOUCHER",&manual_voucher);
		if(manual_voucher==TRUE)
		{
			FINDRSCGETINT(mtnrsrc,"CHECK NUMBER",&checkno);
			FINDFLDSETINT(dbsort->fileno,"VIRTUAL CHECK NUMBER",checkno);
		}
		if(readdel==FALSE)
		{
			FINDFLDGETDOUBLE(dbsort->fileno,"AMOUNT",&amount);
			total_po+=amount;
		}
	}
	FINDFLDSETSHORT(dbsort->fileno,"SOURCE",source);
	WRTNRD(dbsort->fileno,0,NULL);
	FINDRSCSETDOUBLE(mbl_rec->mainrsrc,"TOTAL PO",total_po);
	updatersrc(mbl_rec->mainrsrc,"TOTAL PO");
	updatebrowse(mtn->update_list,mbl_rec,dbsort->fileno,module,mtnrsrc);
	if(mbl_rec!=NULL) quit_record(mtnrsrc,mtn);
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->ACCTtypes!=NULL) freeapplib(mtn->ACCTtypes);
		if(mtn->cpmonth!=NULL) freeapplib(mtn->cpmonth); 
		if(mtn->pmonth!=NULL) freeapplib(mtn->pmonth);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(WindowCount==1)
	{
		if(expnum!=(-1)) CLSNRD(expnum);
		if(revnum!=(-1)) CLSNRD(revnum);
		if(balnum!=(-1)) CLSNRD(balnum);
		if(vendorid!=NULL) Rfree(vendorid);
		if(invid!=NULL) Rfree(invid);
		if(dbsort->fileno!=(-1)) CLSNRD(dbsort->fileno);
		if(vennum!=(-1)) CLSNRD(vennum);
		if(yearnum!=(-1)) CLSNRD(yearnum);
		if(invnum!=(-1)) CLSNRD(invnum);
		if(balnum!=(-1)) CLSNRD(balnum);
		if(finsetup!=NULL) { free_finmgt(finsetup); finsetup=NULL; }
		ShutdownSubsystems();
		exit(0);
	}
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc); 
	}
}
static void getfilecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *date=NULL,*timex=NULL;
	int accttype=0;
	NRDfield *field=NULL;

	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"VENDOR IDENTIFICATION",&vendorid);
	FINDRSCGETINT(mtnrsrc,"CALENDAR YEAR",&venyrid);
	FINDRSCGETSTRING(mtnrsrc,"INVOICE IDENTIFICATION",&invid);
	FINDRSCGETINT(mtnrsrc,"TRANSACTION NUMBER",&transno);
	ZERNRD(dbsort->fileno);
	FINDFLDSETSTRING(dbsort->fileno,"VENDOR IDENTIFICATION",vendorid);
	FINDFLDSETINT(dbsort->fileno,"CALENDAR YEAR",venyrid);
	FINDFLDSETSTRING(dbsort->fileno,"INVOICE IDENTIFICATION",invid);
	FINDFLDSETINT(dbsort->fileno,"TRANSACTION NUMBER",transno);
	editsinglefilersrcsnokeys(dbsort->fileno,1,mtnrsrc,TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"PO IDENTIFICATION",FALSE);
	FINDRSCSETEDITABLE(mtnrsrc,"FISCAL YEAR",FALSE);
	FINDRSCSETEDITABLE(mtnrsrc,"ACCOUNT TYPES",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"DEFINITION TYPES",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"PROCESSING MONTHS",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"FISCAL PROCESSING MONTHS",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"CLEAR PROCESSING MONTHS",TRUE);
	if(EQLNRD(dbsort->fileno,1))
	{
		KEYNRD(dbsort->fileno,1);
		FINDRSCGETSTRING(mtnrsrc,"PO IDENTIFICATION",&poid);
		FINDFLDSETSTRING(dbsort->fileno,"PO IDENTIFICATION",poid);
		FINDRSCGETINT(mtnrsrc,"FISCAL YEAR",&yearid);
		FINDFLDSETINT(dbsort->fileno,"FISCAL YEAR",yearid);
		timex=GETCURRENTTIME();
		field=FLDNRD(vchnum,"ENTRY DATE");
		if(field!=NULL) 
		{
			if(field->len<10) date=GETCURRENTDATE();
			else date=GETCURRENTDATE10();
		} else date=GETCURRENTDATE();
		FINDFLDSETSTRING(dbsort->fileno,"ENTRY DATE",date);
		FINDFLDSETSTRING(dbsort->fileno,"ENTRY TIME",timex);
		FINDFLDSETSTRING(dbsort->fileno,"SOURCE USER",USERLOGIN);
		FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&accttype);
		if(accttype==BAL_ACCT)
		{
			FINDRSCSETEDITABLE(mtnrsrc,"DEBIT CODE",FALSE);
		}
		ClearRDATData(mtn->previous);
		if(date!=NULL) Rfree(date);
		if(timex!=NULL) Rfree(timex);
		seteditable(mtnrsrc,mtn);
	} else {
		GetRDATData(dbsort->fileno,mtn->previous);
		getrecordsetedit(mtnrsrc,mtn);
	}
	getsupportingrecords(mtnrsrc,dbsort,TRUE);
	updateSCRNvirtuals(mtnrsrc);
}
static void check_bankidcb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *bankid1=NULL;

	readwidget(mtnrsrc,"BANK IDENTIFICATION");
	FINDRSCGETSTRING(mtnrsrc,"BANK IDENTIFICATION",&bankid1);
	FINDFLDSETSTRING(dbsort->fileno,"BANK IDENTIFICATION",bankid1);
	check_bankid(mtnrsrc,mtn);
	if(bankid1!=NULL) Rfree(bankid1);
	updatefilerecord2rsrc(bnkacc,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
static short check_bankid(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *bankid1=NULL,*warnmessage=NULL;
	char delflag=FALSE;

	if(bnkacc!=(-1))
	{
		FINDFLDGETSTRING(dbsort->fileno,"BANK IDENTIFICATION",&bankid1);
		FINDFLDSETSTRING(bnkacc,"BANK IDENTIFICATION",bankid1);
		if(ADVEQLNRDsec(bnkacc,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(bnkacc,1);
			warnmessage=Rmalloc(RDAstrlen(bankid1)+230);
			sprintf(warnmessage,"The Bank Identification of [%s] is an invalid value.  It does not exist in the [BNKREC][BNKACC] data file.  Please select a valid Bank Identification to save this record.",(bankid1==NULL?"":bankid1));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BANK IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			if(bankid1!=NULL) Rfree(bankid1);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(bnkacc,"DELETEFLAG",&delflag);
			if(delflag)
			{
				warnmessage=Rmalloc(RDAstrlen(bankid1)+215);
				sprintf(warnmessage,"The Bank Identification of [%s] is a deleted value in the [BNKREC][BNKACC] data file.  Please select a valid Bank Identification to save this record.",(bankid1==NULL?"":bankid1));
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED BANK IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				if(bankid1!=NULL) Rfree(bankid1);
				return(FALSE);
			}
		}
	}
	if(bankid1!=NULL) Rfree(bankid1);
	return(TRUE);
}
static void getsupportingrecords(RDArsrc *mtnrsrc,DBsort *dbsort,short doupdate)
{
	if(invnum!=(-1))
	{
		COPYFIELD(dbsort->fileno,invnum,"VENDOR IDENTIFICATION");
		COPYFIELD(dbsort->fileno,invnum,"CALENDAR YEAR");
		COPYFIELD(dbsort->fileno,invnum,"INVOICE IDENTIFICATION");
		if(ADVEQLNRDsec(invnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(invnum,1);
		if(doupdate)
		{
			updatefilerecord2rsrc(invnum,mtnrsrc);
		} else {
			filerecord2rsrc(invnum,mtnrsrc);
		}
	}
	if(yearnum!=(-1))
	{
		COPYFIELD(dbsort->fileno,yearnum,"VENDOR IDENTIFICATION");
		COPYFIELD(dbsort->fileno,yearnum,"CALENDAR YEAR");
		if(ADVEQLNRDsec(yearnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(yearnum,1);
		if(doupdate)
		{
			updatefilerecord2rsrc(yearnum,mtnrsrc);
		} else {
			filerecord2rsrc(yearnum,mtnrsrc);
		}
	}
	if(vennum!=(-1))
	{
		COPYFIELD(dbsort->fileno,vennum,"VENDOR IDENTIFICATION");
		if(ADVEQLNRDsec(vennum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(vennum,1);
		if(doupdate)
		{
			updatefilerecord2rsrc(vennum,mtnrsrc);
		} else {
			filerecord2rsrc(vennum,mtnrsrc);
		}
	}
	if(msrnum!=(-1))
	{
		COPYFIELD(dbsort->fileno,msrnum,"PO IDENTIFICATION");
		FIELDCOPY(dbsort->fileno,"FISCAL YEAR",msrnum,
			"FISCAL YEAR");
		if(ADVEQLNRDsec(msrnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(msrnum,1);
		if(doupdate)
		{
			updatefilerecord2rsrc(msrnum,mtnrsrc);
		} else {
			filerecord2rsrc(msrnum,mtnrsrc);
		}
	}
	if(sitenum!=(-1))
	{
		COPYFIELD(msrnum,sitenum,"SITE IDENTIFICATION");
		if(ADVEQLNRDsec(sitenum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(sitenum,1);
		if(doupdate)
		{
			updatefilerecord2rsrc(sitenum,mtnrsrc);
		} else {
			filerecord2rsrc(sitenum,mtnrsrc);
		}
	}
	if(linenum!=(-1))
	{
		COPYFIELD(dbsort->fileno,linenum,"PO IDENTIFICATION");
		FIELDCOPY(dbsort->fileno,"FISCAL YEAR",linenum,
			"FISCAL YEAR");
		FIELDCOPY(dbsort->fileno,"PO LINE NUMBER",linenum,
			"LINE NUMBER");
		if(ADVEQLNRDsec(linenum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(linenum,1);
		if(doupdate)
		{
			updatefilerecord2rsrc(linenum,mtnrsrc);
		} else {
			filerecord2rsrc(linenum,mtnrsrc);
		}
	}
	if(bnkacc!=(-1))
	{
		COPYFIELD(dbsort->fileno,bnkacc,"BANK IDENTIFICATION");
		if(ADVEQLNRDsec(bnkacc,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(bnkacc,1);
		if(doupdate)
		{
			updatefilerecord2rsrc(bnkacc,mtnrsrc);
		} else {
			filerecord2rsrc(bnkacc,mtnrsrc);
		}
	}
}
static void setpmonth(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected=0;
	short month=0;

	if(!FINDFLDGETSHORT(dbsort->fileno,"CLEAR PROCESSING MONTH",&month))
	{
		selected=(int)month;
		FINDRSCSETINT(mtnrsrc,"CLEAR PROCESSING MONTHS",selected);
	}
	if(!FINDFLDGETSHORT(dbsort->fileno,"FISCAL PROCESSING MONTH",&month))
	{
		selected=(int)month;
		FINDRSCSETINT(mtnrsrc,"FISCAL PROCESSING MONTHS",selected);
	}
	if(!FINDFLDGETSHORT(dbsort->fileno,"PROCESSING MONTH",&month))
	{
		selected=(int)month;
		FINDRSCSETINT(mtnrsrc,"PROCESSING MONTHS",selected);
	}
}
static void getpmonths(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn->cpmonth!=NULL) freeapplib(mtn->cpmonth);
	mtn->cpmonth=APPlibNEW();
	if(mtn->pmonth!=NULL) freeapplib(mtn->pmonth);
	mtn->pmonth=APPlibNEW();
	addAPPlib(mtn->cpmonth,"PRE-YEAR");
	addAPPlib(mtn->cpmonth,"JANUARY");
	addAPPlib(mtn->pmonth,"JANUARY");
	addAPPlib(mtn->cpmonth,"FEBRUARY");
	addAPPlib(mtn->pmonth,"FEBRUARY");
	addAPPlib(mtn->cpmonth,"MARCH");
	addAPPlib(mtn->pmonth,"MARCH");
	addAPPlib(mtn->cpmonth,"APRIL");
	addAPPlib(mtn->pmonth,"APRIL");
	addAPPlib(mtn->cpmonth,"MAY");
	addAPPlib(mtn->pmonth,"MAY");
	addAPPlib(mtn->cpmonth,"JUNE");
	addAPPlib(mtn->pmonth,"JUNE");
	addAPPlib(mtn->cpmonth,"JULY");
	addAPPlib(mtn->pmonth,"JULY");
	addAPPlib(mtn->cpmonth,"AUGUST");
	addAPPlib(mtn->pmonth,"AUGUST");
	addAPPlib(mtn->cpmonth,"SEPTEMBER");
	addAPPlib(mtn->pmonth,"SEPTEMBER");
	addAPPlib(mtn->cpmonth,"OCTOBER");
	addAPPlib(mtn->pmonth,"OCTOBER");
	addAPPlib(mtn->cpmonth,"NOVEMBER");
	addAPPlib(mtn->pmonth,"NOVEMBER");
	addAPPlib(mtn->cpmonth,"DECEMBER");
	addAPPlib(mtn->pmonth,"DECEMBER");
	addAPPlib(mtn->cpmonth,"POST YEAR");
}
static void clearDEFtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char dist=FALSE;

	FINDRSCGETCHAR(mtnrsrc,"DISTRIBUTED",&dist);
	FINDRSCSETEDITABLE(mtnrsrc,"CLEARING DUE FROM",TRUE);
	FINDRSCSETSTRING(mtnrsrc,"CLEARING DUE FROM",NULL);
	updatersrc(mtnrsrc,"CLEARING DUE FROM");
	FINDRSCSETEDITABLE(mtnrsrc,"CLEARING CASH",TRUE);
	FINDRSCSETSTRING(mtnrsrc,"CLEARING CASH",NULL);
	updatersrc(mtnrsrc,"CLEARING CASH");
	if(dist==TRUE)
	{
		FINDRSCSETEDITABLE(mtnrsrc,"CLEARING DUE FROM",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"CLEARING CASH",FALSE);
	}
	changeclearDEFtype(mtnrsrc,mtn);
	updateSCRNvirtuals(mtnrsrc);
}
static void DEFtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int x=0;
	char dist=FALSE;

	FINDRSCGETCHAR(mtnrsrc,"DISTRIBUTED",&dist);
	FINDRSCSETEDITABLE(mtnrsrc,"ACCOUNT CODE",TRUE);
	FINDRSCSETSENSITIVE(mtnrsrc,"ACCOUNT CODE",TRUE);
	FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE",NULL);
	updatersrc(mtnrsrc,"ACCOUNT CODE");
	FINDRSCSETSENSITIVE(mtnrsrc,"DEBIT CODE",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"DEBIT CODE",TRUE);
	FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE",NULL);
	updatersrc(mtnrsrc,"DEBIT CODE");
	FINDRSCSETSENSITIVE(mtnrsrc,"CREDIT CODE",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"CREDIT CODE",TRUE);
	FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE",NULL);
	updatersrc(mtnrsrc,"CREDIT CODE");
	FINDRSCSETSENSITIVE(mtnrsrc,"PAYABLE CODE",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"PAYABLE CODE",TRUE);
	FINDRSCSETSTRING(mtnrsrc,"PAYABLE CODE",NULL);
	updatersrc(mtnrsrc,"PAYABLE CODE");
	if(dist==TRUE)
	{
		FINDRSCSETEDITABLE(mtnrsrc,"ACCOUNT CODE",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"DEBIT CODE",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"CREDIT CODE",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"PAYABLE CODE",FALSE);
	}
	FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&x);
	if(x==BAL_ACCT)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"DEBIT CODE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"CREDIT CODE",FALSE);
	}
	x=0;
	changeDEFtype(mtnrsrc,mtn);
	updateSCRNvirtuals(mtnrsrc);
}
static void changeclearDEFtype(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selectedd=0;
	short deftype1=0;

	if(FINDRSCGETINT(mtnrsrc,"CLEARING DEFINITION TYPES",&selectedd)) return;
	deftype1=(short)selectedd;
	FINDRSCSETSENSITIVE(mtnrsrc,"CLEARING DUE FROM",TRUE);
	FINDRSCSETSENSITIVE(mtnrsrc,"CLEARING CASH",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"CLEARING DUE FROM",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"CLEARING CASH",TRUE);
	FINDRSCSETACCTTYPE(mtnrsrc,"CLEARING DUE FROM",BAL_ACCT);
	FINDRSCSETACCTTYPE(mtnrsrc,"CLEARING CASH",BAL_ACCT);
	FINDRSCSETDEFTYPE(mtnrsrc,"CLEARING DUE FROM",deftype1);
	FINDRSCSETDEFTYPE(mtnrsrc,"CLEARING CASH",deftype1);
	updateSCRNvirtuals(mtnrsrc);
}

static void changeDEFtype(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selecteda=0,selectedd=0;
	short accttype=0,deftype1=0;
	char *account_code=NULL;

	if(FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selecteda)) return;
	if(FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selectedd)) return;
	accttype=(short)selecteda;
	deftype1=(short)selectedd;
	FINDRSCSETSENSITIVE(mtnrsrc,"DEBIT CODE",TRUE);
	FINDRSCSETSENSITIVE(mtnrsrc,"CREDIT CODE",TRUE);
	FINDRSCSETSENSITIVE(mtnrsrc,"PAYABLE CODE",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"ACCOUNT CODE",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"DEBIT CODE",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"CREDIT CODE",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"PAYABLE CODE",TRUE);
	FINDRSCSETACCTTYPE(mtnrsrc,"ACCOUNT CODE",accttype);
	FINDRSCSETDEFTYPE(mtnrsrc,"ACCOUNT CODE",deftype1);
	FINDRSCSETACCTTYPE(mtnrsrc,"DEBIT CODE",BAL_ACCT);
	FINDRSCSETACCTTYPE(mtnrsrc,"CREDIT CODE",BAL_ACCT);
	FINDRSCSETACCTTYPE(mtnrsrc,"PAYABLE CODE",BAL_ACCT);
	FINDRSCSETDEFTYPE(mtnrsrc,"DEBIT CODE",deftype1);
	FINDRSCSETDEFTYPE(mtnrsrc,"CREDIT CODE",deftype1);
	FINDRSCSETDEFTYPE(mtnrsrc,"PAYABLE CODE",deftype1);
	switch(selecteda)
	{
		case EXP_ACCT:
		case REV_ACCT:
			if(balnum!=(-1))
			{
				ZERNRD(balnum);
			}
			break;
		case BAL_ACCT:
			FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE",NULL);
			FINDRSCSETSENSITIVE(mtnrsrc,"DEBIT CODE",FALSE);
			if(balnum!=(-1))
			{
				FINDRSCGETSTRING(mtnrsrc,"ACCOUNT CODE",&account_code);
				ZERNRD(balnum);
				FINDFLDSETSHORT(balnum,"DEFINITION TYPE",deftype1);
				FINDFLDSETSTRING(balnum,"ACCOUNT CODE",account_code);
				if(ADVEQLNRDsec(balnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(balnum,1);
				updatefilerecord2rsrc(balnum,mtnrsrc);
			}
			break;
		default:
			prterr("Invalid Account Type [%d]",selecteda);
	}
	updateSCRNvirtuals(mtnrsrc);
	if(account_code!=NULL) Rfree(account_code);
}
static void setACCTtype(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selectedd=0;

	changeACCTtype(mtnrsrc,mtn);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selectedd);
	if(mtn->DEFtypes->numlibs<1)
	{
		addAPPlib(mtn->DEFtypes,"No Definitions Available");
	}
	if(!FINDRSCLISTAPPlib(mtnrsrc,"DEFINITION TYPES",selectedd,mtn->DEFtypes))
	{
		updatersrc(mtnrsrc,"DEFINITION TYPES");
	}
	changeDEFtype(mtnrsrc,mtn);
	if(venpmt->use_venpmt_fund)
	{
	FINDRSCGETINT(mtnrsrc,"CLEARING DEFINITION TYPES",&selectedd);
	if(mtn->clearDEFtypes->numlibs<1)
	{
		addAPPlib(mtn->clearDEFtypes,"No Definitions Available");
	}
	if(!FINDRSCLISTAPPlib(mtnrsrc,"CLEARING DEFINITION TYPES",selectedd,mtn->clearDEFtypes))
	{
		updatersrc(mtnrsrc,"CLEARING DEFINITION TYPES");
	}
	changeclearDEFtype(mtnrsrc,mtn);
	}
}
static void ACCTtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int x=0;
	char dist=FALSE;

	changeACCTtype(mtnrsrc,mtn);
	FINDRSCGETCHAR(mtnrsrc,"DISTRIBUTED",&dist);
	FINDRSCSETEDITABLE(mtnrsrc,"ACCOUNT CODE",TRUE);
	FINDRSCSETSENSITIVE(mtnrsrc,"ACCOUNT CODE",TRUE);
	FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE",NULL);
	updatersrc(mtnrsrc,"ACCOUNT CODE");
	FINDRSCSETEDITABLE(mtnrsrc,"DEBIT CODE",TRUE);
	FINDRSCSETSENSITIVE(mtnrsrc,"DEBIT CODE",TRUE);
	FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE",NULL);
	updatersrc(mtnrsrc,"DEBIT CODE");
	FINDRSCSETEDITABLE(mtnrsrc,"CREDIT CODE",TRUE);
	FINDRSCSETSENSITIVE(mtnrsrc,"CREDIT CODE",TRUE);
	FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE",NULL);
	updatersrc(mtnrsrc,"CREDIT CODE");
	FINDRSCSETEDITABLE(mtnrsrc,"PAYABLE CODE",TRUE);
	FINDRSCSETSENSITIVE(mtnrsrc,"PAYABLE CODE",TRUE);
	FINDRSCSETSTRING(mtnrsrc,"PAYABLE CODE",NULL);
	updatersrc(mtnrsrc,"PAYABLE CODE");
	if(dist==TRUE)
	{
		FINDRSCSETEDITABLE(mtnrsrc,"ACCOUNT CODE",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"DEBIT CODE",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"CREDIT CODE",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"PAYABLE CODE",FALSE);
	}
	if(mtn->DEFtypes->numlibs<1)
	{
		addAPPlib(mtn->DEFtypes,"No Definitions Available");
	}
	x=0;
	readwidget(mtnrsrc,"DEFINITION TYPES");
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	if(!FINDRSCLISTAPPlib(mtnrsrc,"DEFINITION TYPES",x,mtn->DEFtypes))
	{
		updatersrc(mtnrsrc,"DEFINITION TYPES");
	}
	changeDEFtype(mtnrsrc,mtn);
}
static void changeACCTtype(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selecteda=0,x=0;
	RDAacct *acct=NULL;

	if(FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selecteda)) return;
	if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
	mtn->DEFtypes=APPlibNEW();
	switch(selecteda)
	{
		case EXP_ACCT:	
			if(EXPENDITURE_ACCOUNT->codetype!=NULL)
			{
				for(x=0,acct=EXPENDITURE_ACCOUNT->codetype;
					x<EXPENDITURE_ACCOUNT->num;++x,++acct)
				{
					addAPPlib(mtn->DEFtypes,acct->name);
				}
			}
			break;
		case REV_ACCT:	
			if(REVENUE_ACCOUNT->codetype!=NULL)
			{
				for(x=0,acct=REVENUE_ACCOUNT->codetype;
				x<REVENUE_ACCOUNT->num;++x,++acct)
				{
					addAPPlib(mtn->DEFtypes,acct->name);
				}
			}
			break;
		case BAL_ACCT:	
			if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
			{
				for(x=0,acct=BALANCE_SHEET_ACCOUNT->codetype;
					x<BALANCE_SHEET_ACCOUNT->num;++x,++acct)
				{
					addAPPlib(mtn->DEFtypes,acct->name);
				}
			}
			break;
		default:
			prterr("Invalid Account Type [%d]",selecteda);
			break;
	}
}
