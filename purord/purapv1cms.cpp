/* purapv1.c - Add Purchase Order Vouchers Browse Screen */
/*lint -library */
#include <cstdio>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <fin.hpp>
#include <venpmt.hpp>
#include <purord.hpp>

extern RDAvenpmt *venpmt;
extern RDApurord *purord;
extern void purapvm(short,void *,int,char **);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void doexit_test(MakeBrowseList *);
static void saveexit(RDArsrc *,MakeBrowseList *);
static void saveexittest(RDArsrc *,MakeBrowseList *);
static void new_record(RDArsrc *,MakeBrowseList *);
static short selectfunction(MakeBrowseList *);
extern short msrnum,sitenum,vennum,venadrnum;
extern short linenum,accnum,trannum,invnum,vchnum;
extern short eyrnum,ryrnum,byrnum;
extern short fyearnum,yearnum;
extern short expnum,revnum,balnum,finyrnum;
extern RDAfinmgt *finsetup;
extern char *module;
extern char *poid,*vendorid,*invid,*checkdate;
extern char manual_voucher,prepaid,pay_1099,check_detail;
extern int venyrid,yearid,checkno,PO_COUNT;
extern short pmonth;
extern MakeBrowseList *mbl_addpo;
MakeBrowseList *mbl_rec=NULL;
void browse_purapv(RDArsrc *);
DBsort *dbsort=NULL;
extern char ClosePO;
extern double invoice_total;
double total_po=0;

void makeDBsortfile(DBsort *dbsrt)
{
	NRDkey *key=NULL;
	NRDpart *part;
	NRDfield *field,*fields=NULL;
	int x=0;
	short keyno=(-1),nofields=0;

	nofields=NUMFLDS(vchnum);
	fields=FLDPOINTER(vchnum);
	if(fields!=NULL)
	{
		for(x=0,field=fields;x<nofields;++x,++field)
		{
			addDBfield(dbsrt,field->name,field->type,field->len);
		}
		addDBfield(dbsrt,"DISENCUMBRANCE AMOUNT",DOLLARS,20);
		addDBfield(dbsrt,"PO FISCAL YEAR",LONGV,4);
		addDBfield(dbsrt,"PO ACCOUNT TYPE",SHORTV,2);
		addDBfield(dbsrt,"PO DEFINITION TYPE",SHORTV,2);
		addDBfield(dbsrt,"PO ACCOUNT CODE",EXPENDITURE,40);
		addDBfield(dbsrt,"PO DEBIT CODE",BALANCESHEET,40);
		addDBfield(dbsrt,"PO CREDIT CODE",BALANCESHEET,40);
		addDBfield(dbsrt,"FINALED",BOOLNS,1);
	}
	keyno=addDBkey(dbsrt,"SORT FILE MAIN KEY");
	key=KEYNUM(vchnum,1);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			addDBkeypart(dbsrt,keyno,part->name);
		}
	}
}
void addsortrecord(RDArsrc *parent,DBsort *dbsrt,int lineno,int accno,double amount,char skip_records)
{
	char falseflag=FALSE;
	short accttype=0,deftypex=0,src=1;

	FINDFLDSETCHAR(dbsrt->fileno,"DELETEFLAG",skip_records);
	FINDFLDSETSHORT(dbsrt->fileno,"SOURCE",src);
	FINDFLDSETINT(dbsort->fileno,"CALENDAR YEAR",venyrid);
	FINDFLDSETSTRING(dbsrt->fileno,"PO IDENTIFICATION",poid);
	FINDFLDSETSTRING(dbsrt->fileno,"VENDOR IDENTIFICATION",vendorid);
	FINDFLDSETINT(dbsrt->fileno,"FISCAL YEAR",yearid);
	FINDFLDSETINT(dbsrt->fileno,"PO FISCAL YEAR",yearid);
	FINDFLDSETINT(dbsrt->fileno,"PO LINE NUMBER",lineno);
	FINDFLDSETINT(dbsrt->fileno,"PO ACCOUNT NUMBER",accno);
	FINDFLDSETSTRING(dbsrt->fileno,"INVOICE IDENTIFICATION",invid);
	FINDFLDSETSHORT(dbsrt->fileno,"PROCESSING MONTH",pmonth);
	FINDFLDSETDOUBLE(dbsrt->fileno,"AMOUNT",amount);
	FINDFLDSETCHAR(dbsrt->fileno,"FINALED",falseflag);
	FINDFLDSETDOUBLE(dbsrt->fileno,"DISENCUMBRANCE AMOUNT",amount);
	FINDFLDSETCHAR(dbsrt->fileno,"MANUAL VOUCHER",manual_voucher);
	FINDFLDSETINT(dbsrt->fileno,"CHECK NUMBER",checkno);
	/* FINDFLDSETINT(dbsrt->fileno,"VIRTUAL CHECK NUMBER",checkno); */
	FINDFLDSETSTRING(dbsrt->fileno,"CHECK DATE",checkdate);
	FINDFLDSETCHAR(dbsrt->fileno,"PREPAID",prepaid);
	FINDFLDSETCHAR(dbsrt->fileno,"1099 PAYMENT",pay_1099);
	FINDFLDSETCHAR(dbsrt->fileno,"CHECK DETAIL",check_detail);
	FINDFLDSETSTRING(dbsrt->fileno,"SOURCE USER",USERLOGIN);
	FINDFLDSETCHAR(dbsrt->fileno,"SOURCE USER VERIFIED",falseflag);
	COPYFIELD(dbsrt->fileno,linenum,"PO IDENTIFICATION");
	FIELDCOPY(dbsrt->fileno,"PO FISCAL YEAR",linenum,"FISCAL YEAR");
	FIELDCOPY(dbsrt->fileno,"PO LINE NUMBER",linenum,"LINE NUMBER");
	if(lineno==0) 
	{
		KEYNRD(linenum,1);
	} else if(ADVEQLNRDsec(linenum,1,SCRNvirtualSubData,parent)) 
	{
		KEYNRD(linenum,1);
	}
	COPYFIELD(linenum,dbsrt->fileno,"DESCRIPTION");
	COPYFIELD(dbsrt->fileno,accnum,"PO IDENTIFICATION");
	FIELDCOPY(dbsrt->fileno,"PO FISCAL YEAR",accnum,"FISCAL YEAR");
	FIELDCOPY(dbsrt->fileno,"PO LINE NUMBER",accnum,"LINE NUMBER");
	FIELDCOPY(dbsrt->fileno,"PO ACCOUNT NUMBER",accnum,"ACCOUNT NUMBER");
	if(ADVEQLNRDsec(accnum,1,SCRNvirtualSubData,parent)) 
	{ 
		KEYNRD(accnum,1);
		return;
	}
	FINDFLDGETSHORT(accnum,"ACCOUNT TYPE",&accttype);
	FINDFLDSETSHORT(dbsrt->fileno,"ACCOUNT TYPE",accttype);
	FINDFLDSETSHORT(dbsrt->fileno,"PO ACCOUNT TYPE",accttype);
	FINDFLDGETSHORT(accnum,"DEFINITION TYPE",&deftypex);
	FINDFLDSETSHORT(dbsrt->fileno,"PO DEFINITION TYPE",deftypex);
	FINDFLDSETSHORT(dbsrt->fileno,"DEFINITION TYPE",deftypex);
	FIELDCOPY(accnum,"DEFAULT BANK IDENTIFICATION",dbsrt->fileno,"BANK IDENTIFICATION");
	switch(accttype)
	{
		default:
		case EXP_ACCT:
		case REV_ACCT:
			FIELDCOPY(accnum,"DEBIT CODE",dbsrt->fileno,
				"PO DEBIT CODE");
			FIELDCOPY(accnum,"CREDIT CODE",dbsrt->fileno,
				"PO CREDIT CODE");
			COPYFIELD(accnum,dbsrt->fileno,"ACCOUNT CODE");
			FIELDCOPY(accnum,"ACCOUNT CODE",dbsrt->fileno,
				"PO ACCOUNT CODE");
			FIELDCOPY(accnum,"DEFAULT DEBIT CODE",dbsrt->fileno,
				"DEBIT CODE");
			FIELDCOPY(accnum,"DEFAULT CREDIT CODE",dbsrt->fileno,
				"CREDIT CODE");
			FIELDCOPY(accnum,"DEFAULT PAYABLE CODE",dbsrt->fileno,
				"PAYABLE CODE");
			if(venpmt->use_venpmt_fund)
			{
				FIELDCOPY(accnum,"DEFAULT DUE TO",dbsrt->fileno,
					"DUE TO");
				FIELDCOPY(accnum,"DEFAULT CLEARING DEFINITION TYPE",
					dbsrt->fileno,"CLEARING DEFINITION TYPE");
				FIELDCOPY(accnum,"DEFAULT CLEARING DUE FROM",
					dbsrt->fileno,"CLEARING DUE FROM");
				FIELDCOPY(accnum,"DEFAULT CLEARING CASH",
					dbsrt->fileno,"CLEARING CASH");
			}
			break;
		case BAL_ACCT:
			FIELDCOPY(accnum,"CREDIT CODE",dbsrt->fileno,
				"PO CREDIT CODE");
			COPYFIELD(accnum,dbsrt->fileno,"ACCOUNT CODE");
			FIELDCOPY(accnum,"ACCOUNT CODE",dbsrt->fileno,
				"PO ACCOUNT CODE");
			FIELDCOPY(accnum,"DEFAULT CREDIT CODE",dbsrt->fileno,
				"CREDIT CODE");
			FIELDCOPY(accnum,"DEFAULT PAYABLE CODE",dbsrt->fileno,
				"PAYABLE CODE");
			if(venpmt->use_venpmt_fund)
			{
				FIELDCOPY(accnum,"DEFAULT DUE TO",dbsrt->fileno,
					"DUE TO");
				FIELDCOPY(accnum,"DEFAULT CLEARING DEFINITION TYPE",
					dbsrt->fileno,"CLEARING DEFINITION TYPE");
				FIELDCOPY(accnum,"DEFAULT CLEARING DUE FROM",
					dbsrt->fileno,"CLEARING DUE FROM");
				FIELDCOPY(accnum,"DEFAULT CLEARING CASH",
					dbsrt->fileno,"CLEARING CASH");
			}
			break;
	}
	FINDFLDSETSTRING(dbsrt->fileno,"ENTRY DATE",CURRENT_DATE);
	FINDFLDSETSTRING(dbsrt->fileno,"ENTRY TIME",CURRENT_TIME);
	FINDFLDSETSTRING(dbsrt->fileno,"SOURCE USER",USERLOGIN);
	if(skip_records==FALSE) total_po+=amount;
	WRTNRD(dbsrt->fileno,1,"TRANSACTION NUMBER");
}
void browse_purapv(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL,*tmprsrc=NULL;
	char *sortname=NULL,deleteflag=FALSE;
	short ef=0;
	char *last_po=NULL,*test_po=NULL;
	int last_year=0,last_line=0,last_acct=0;
	int test_year=0,test_line=0,test_acct=0;
	double amount=0,test_amount=0;
	long size_of_file=0;
	char skip_records=FALSE,finaled=FALSE;

	sortname=unique_name();
	dbsort=DBsortNEW("PURORD",sortname,NRDRdaEngine);
	if(sortname!=NULL) Rfree(sortname);
	total_po=0;
	makeDBsortfile(dbsort);
	mainrsrc=RDArsrcNEW(module,"ADD PURCHASE ORDER VOUCHER BROWSE");
	searchrsrc=RDArsrcNEW(module,"ADD PURCHASE ORDER VOUCHER SEARCH BROWSE");
	definelist=RDArsrcNEW(module,"ADD PURCHASE ORDER VOUCHER DEFINE LIST");
	addDFincvir(mainrsrc,"VENPMT","VENINV",NULL,invnum);
	addDFincvir(mainrsrc,"VENPMT","VENYR",NULL,yearnum);
	if(fyearnum!=(-1) && venpmt->use_fiscal_summary==TRUE)
	{
		addDFincvir(mainrsrc,"VENPMT","VENFYR",NULL,fyearnum);
	}
	addDFincvir(mainrsrc,"VENPMT","VENMSR",NULL,vennum);
	addDFincvir(mainrsrc,"VENPMT","VENADR",NULL,venadrnum);
	addDFincvir(mainrsrc,module,"PURLIN",NULL,linenum);
	addDFincvir(mainrsrc,module,"PURACC",NULL,accnum);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	readwidget(mtnrsrc,"[PURMST][FISCAL YEAR]");
	FINDRSCGETINT(mtnrsrc,"[PURMST][FISCAL YEAR]",&yearid);
	readwidget(mtnrsrc,"[PURMST][PO IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[PURMST][PO IDENTIFICATION]",&poid);
	readwidget(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]",&vendorid);
	readwidget(mtnrsrc,"[VENINV][CALENDAR YEAR]");
	FINDRSCGETINT(mtnrsrc,"[VENINV][CALENDAR YEAR]",&venyrid);
	readwidget(mtnrsrc,"[VENINV][INVOICE IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[VENINV][INVOICE IDENTIFICATION]",&invid);
	readwidget(mtnrsrc,"DEFAULT SKIP ALL");
	FINDRSCGETCHAR(mtnrsrc,"DEFAULT SKIP ALL",&skip_records);
	if(invnum!=(-1))
	{
		file2rsrc(invnum,mainrsrc,FALSE);
		FINDFLDSETSTRING(invnum,"VENDOR IDENTIFICATION",vendorid);
		FINDFLDSETINT(invnum,"CALENDAR YEAR",venyrid);
		FINDFLDSETSTRING(invnum,"INVOICE IDENTIFICATION",invid);
		if(ADVEQLNRDsec(invnum,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(invnum,1);
		filerecord2rsrc(invnum,mainrsrc);
		if(yearnum!=(-1))
		{
			file2rsrc(yearnum,mainrsrc,FALSE);
			FINDFLDSETSTRING(yearnum,"VENDOR IDENTIFICATION",vendorid);
			FINDFLDSETINT(yearnum,"CALENDAR YEAR",venyrid);
			if(ADVEQLNRDsec(yearnum,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(yearnum,1);
			filerecord2rsrc(yearnum,mainrsrc);
			if(vennum!=(-1))
			{
				file2rsrc(vennum,mainrsrc,FALSE);
				FINDFLDSETSTRING(vennum,"VENDOR IDENTIFICATION",vendorid);
				if(ADVEQLNRDsec(vennum,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(vennum,1);
				filerecord2rsrc(vennum,mainrsrc);
			}
		}
		if(fyearnum!=(-1))
		{
			file2rsrc(fyearnum,mainrsrc,FALSE);
			FINDFLDSETSTRING(fyearnum,"VENDOR IDENTIFICATION",vendorid);
			FINDFLDSETINT(fyearnum,"FISCAL YEAR",yearid);
			if(ADVEQLNRDsec(fyearnum,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(fyearnum,1);
			filerecord2rsrc(fyearnum,mainrsrc);
		}
	}
	if(venadrnum!=(-1))
	{
		file2rsrc(venadrnum,mainrsrc,FALSE);
		FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid);
		if(ADVEQLNRDsec(venadrnum,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(venadrnum,1);
		filerecord2rsrc(venadrnum,mainrsrc);
	}
	tmprsrc=diagscrn(trannum,"DIAGNOSTIC SCREEN",module,
		"Creating / Defaulting Vouchers",NULL);
	addDFincvir(tmprsrc,module,"PURTRAN",NULL,trannum);
	GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
	if(tmprsrc!=NULL)
		if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE)) ForceWindowUpdate(tmprsrc);
	dbsort->fileno=OPNDBsort(dbsort);
	ZERNRD(trannum);
	FINDFLDSETSTRING(trannum,"PO IDENTIFICATION",poid);
	FINDFLDSETINT(trannum,"FISCAL YEAR",yearid);
	ef=ADVGTENRDsec(trannum,1,SCRNvirtualSubData,tmprsrc);
	FINDFLDGETSTRING(trannum,"PO IDENTIFICATION",&last_po);
	FINDFLDGETINT(trannum,"FISCAL YEAR",&last_year);
	FINDFLDGETINT(trannum,"LINE NUMBER",&last_line);
	FINDFLDGETINT(trannum,"ACCOUNT NUMBER",&last_acct);
	while(!ef)
	{
		FINDFLDGETSTRING(trannum,"PO IDENTIFICATION",&test_po);
		FINDFLDGETINT(trannum,"FISCAL YEAR",&test_year);
		if(RDAstrcmp(poid,test_po) || yearid!=test_year) break;
		FINDFLDGETCHAR(trannum,"DELETEFLAG",&deleteflag);
		COPYFIELD(trannum,accnum,"PO IDENTIFICATION");
		COPYFIELD(trannum,accnum,"FISCAL YEAR");
		COPYFIELD(trannum,accnum,"LINE NUMBER");
		COPYFIELD(trannum,accnum,"ACCOUNT NUMBER");
		if(ADVEQLNRDsec(accnum,1,SCRNvirtualSubData,tmprsrc))
			KEYNRD(accnum,1);
		FINDFLDGETCHAR(accnum,"FINALED",&finaled);
		if(!deleteflag && !finaled)
		{
			FINDFLDGETINT(trannum,"LINE NUMBER",&test_line);
			FINDFLDGETINT(trannum,"ACCOUNT NUMBER",&test_acct);
			FINDFLDGETDOUBLE(trannum,"AMOUNT",&test_amount);
			if(!RDAstrcmp(last_po,test_po) && yearid==test_year 
				&& test_line==last_line && test_acct==last_acct)
			{
				amount+=test_amount;
			} else {
				if(amount!=0.0)
					addsortrecord(mainrsrc,dbsort,last_line,last_acct,amount,skip_records);
				if(last_po!=NULL) Rfree(last_po);
				last_po=stralloc(test_po);
				last_year=test_year;
				last_line=test_line;
				last_acct=test_acct;
				amount=test_amount;
			}
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
		} else {
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,FALSE);
		}
		ef=ADVNXTNRDsec(trannum,1,SCRNvirtualSubData,tmprsrc);
	}
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	if(amount!=0.0) addsortrecord(mainrsrc,dbsort,last_line,last_acct,amount,skip_records);
	if(last_po!=NULL) Rfree(last_po);
	if(test_po!=NULL) Rfree(test_po);
	size_of_file=SIZNRD(dbsort->fileno);
	if(size_of_file>0)
	{
		addbtnrsrc(mainrsrc,"SAVE",TRUE,saveexittest,mbl_rec);
		addstdrsrc(mainrsrc,"TOTAL PO",DOLLARS,20,&total_po,TRUE);
		mbl_rec=ADVbrowse(dbsort->fileno,1,mainrsrc,searchrsrc,definelist,NULL,selectfunction,doexit_test,okfunc,new_record,dbsort,TRUE,NULL,0,0,0,TRUE);
		FINDRSCSETFUNC(mainrsrc,"SAVE",saveexittest,mbl_rec);
	} else {
		ERRORDIALOG("Add Purchase Order Vouchers","No records selected using from the purchase order files.  Please try again.",NULL,FALSE);
		CLSDBsort(dbsort);
		FreeDBsort(dbsort);
	}
}
static short selectfunction(MakeBrowseList *blist)
{
	return(TRUE);
}
static void saveexit(RDArsrc *mainrsrc,MakeBrowseList *blist)
{
	NRDfield *field=NULL,*fields=NULL;
	short x,nofields=0,ef=FALSE;
	short accttype=0,poaccttype=0,deftype=0,podeftype=0;
	short cdeftype=0;
	char flag=FALSE,finaled=FALSE;
	char *poid=NULL;
	double amount=0.0,disenc_amount=0.0,initamt=0.0;
	short procmo=0;
	int lineno=0,procyr=0,poprocyr=0,yearid=0;
	RDATData *prev=NULL;

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
	if(dbsort!=NULL) 
	{
		fields=FLDPOINTER(vchnum);
		nofields=NUMFLDS(vchnum);
		ef=FRSNRD(dbsort->fileno,1);
		while(!ef)
		{
			FINDFLDGETCHAR(dbsort->fileno,"DELETEFLAG",&flag);
			if(!flag)
			{
				if(fields!=NULL)
				{
					for(x=0,field=fields;x<nofields;++x,++field)
					{
						COPYFIELD(dbsort->fileno,vchnum,field->name);
					}
				}
				FINDFLDSETCHAR(vchnum,"PO VOUCHER",TRUE);
				ADVWRTTRANSsec(vchnum,1,"TRANSACTION NUMBER",NULL,SCRNvirtualSubData,mainrsrc);
				
				FINDFLDGETDOUBLE(dbsort->fileno,"AMOUNT",&amount);
				FINDFLDGETDOUBLE(dbsort->fileno,"DISENCUMBRANCE AMOUNT",
					&disenc_amount);
				FINDFLDGETSHORT(dbsort->fileno,"ACCOUNT TYPE",&accttype);
				FINDFLDGETSHORT(dbsort->fileno,"PO ACCOUNT TYPE",&poaccttype);
				FINDFLDGETSHORT(dbsort->fileno,"DEFINITION TYPE",&deftype);
				FINDFLDGETSHORT(dbsort->fileno,"PO DEFINITION TYPE",&podeftype);
				FINDFLDGETSHORT(dbsort->fileno,"CLEARING DEFINITION TYPE",&cdeftype);
				FINDFLDGETINT(dbsort->fileno,"FISCAL YEAR",&procyr);
				FINDFLDGETINT(dbsort->fileno,"PO FISCAL YEAR",&poprocyr);
				FINDFLDGETSHORT(dbsort->fileno,"PROCESSING MONTH",&procmo);
/* need to update supporting files here */
/* update FINMGT Files *YR's */
				switch(poaccttype)
				{
					case EXP_ACCT:
						if(amount>disenc_amount)
						{
						if(purord->use_fiscal_months==TRUE)
						{
							ADVupdateundist(dbsort->fileno,"PO ACCOUNT CODE","PO DEBIT CODE","PO CREDIT CODE",NULL,eyrnum,byrnum,poprocyr,podeftype,"ENCUMBRANCES",FISCALPERIOD[procmo],-disenc_amount,SCRNvirtualSubData,mainrsrc);
#ifdef XXXX
							ADVupdateundist(dbsort->fileno,"PO ACCOUNT CODE",NULL,NULL,NULL,eyrnum,(-1),poprocyr,podeftype,"ENCUMBRANCES",FISCALPERIOD[procmo],-disenc_amount,SCRNvirtualSubData,mainrsrc);
							ADVupdateundist(dbsort->fileno,NULL,"PO CREDIT CODE","PO DEBIT CODE",NULL,(-1),byrnum,poprocyr,podeftype,"",FISCALPERIOD[procmo],disenc_amount,SCRNvirtualSubData,mainrsrc);
#endif
						} else {
							ADVupdateundist(dbsort->fileno,"PO ACCOUNT CODE","PO DEBIT CODE","PO CREDIT CODE",NULL,eyrnum,byrnum,poprocyr,podeftype,"ENCUMBRANCES",CALENDAR_MONTHS[procmo],-disenc_amount,SCRNvirtualSubData,mainrsrc);
#ifdef XXXX
							ADVupdateundist(dbsort->fileno,"PO ACCOUNT CODE",NULL,NULL,NULL,eyrnum,(-1),poprocyr,podeftype,"ENCUMBRANCES",CALENDAR_MONTHS[procmo],-disenc_amount,SCRNvirtualSubData,mainrsrc);
							ADVupdateundist(dbsort->fileno,NULL,"PO CREDIT CODE","PO DEBIT CODE",NULL,(-1),byrnum,poprocyr,podeftype,"",CALENDAR_MONTHS[procmo],disenc_amount,SCRNvirtualSubData,mainrsrc);
#endif
						}
						} else {
						if(purord->use_fiscal_months==TRUE)
						{
							ADVupdateundist(dbsort->fileno,"PO ACCOUNT CODE","PO DEBIT CODE","PO CREDIT CODE",NULL,eyrnum,byrnum,poprocyr,podeftype,"ENCUMBRANCES",FISCALPERIOD[procmo],-amount,SCRNvirtualSubData,mainrsrc);
#ifdef XXXX
							ADVupdateundist(dbsort->fileno,"PO ACCOUNT CODE",NULL,NULL,NULL,eyrnum,(-1),poprocyr,podeftype,"ENCUMBRANCES",FISCALPERIOD[procmo],-amount,SCRNvirtualSubData,mainrsrc);
							ADVupdateundist(dbsort->fileno,NULL,"PO CREDIT CODE","PO DEBIT CODE",NULL,(-1),byrnum,poprocyr,podeftype,"",FISCALPERIOD[procmo],amount,SCRNvirtualSubData,mainrsrc);
#endif
						} else {
							ADVupdateundist(dbsort->fileno,"PO ACCOUNT CODE","PO DEBIT CODE","PO CREDIT CODE",NULL,eyrnum,byrnum,poprocyr,podeftype,"ENCUMBRANCES",CALENDAR_MONTHS[procmo],-amount,SCRNvirtualSubData,mainrsrc);
#ifdef XXXX
							ADVupdateundist(dbsort->fileno,"PO ACCOUNT CODE",NULL,NULL,NULL,eyrnum,(-1),poprocyr,podeftype,"ENCUMBRANCES",CALENDAR_MONTHS[procmo],-amount,SCRNvirtualSubData,mainrsrc);
							ADVupdateundist(dbsort->fileno,NULL,"PO CREDIT CODE","PO DEBIT CODE",NULL,(-1),byrnum,poprocyr,podeftype,"",CALENDAR_MONTHS[procmo],amount,SCRNvirtualSubData,mainrsrc);
#endif
						}
						}
						break;
					case REV_ACCT:
						if(purord->use_fiscal_months==TRUE)
						{
						ADVupdateundist(dbsort->fileno,NULL,"PO DEBIT CODE","PO CREDIT CODE",NULL,(-1),byrnum,poprocyr,podeftype,"",FISCALPERIOD[procmo],-disenc_amount,SCRNvirtualSubData,mainrsrc);
#ifdef XXXX
						ADVupdateundist(dbsort->fileno,NULL,"PO CREDIT CODE","PO DEBIT CODE",NULL,(-1),byrnum,poprocyr,podeftype,"",FISCALPERIOD[procmo],disenc_amount,SCRNvirtualSubData,mainrsrc);
#endif
						} else {
						ADVupdateundist(dbsort->fileno,NULL,"PO DEBIT CODE","PO CREDIT CODE",NULL,(-1),byrnum,poprocyr,podeftype,"",CALENDAR_MONTHS[procmo],-disenc_amount,SCRNvirtualSubData,mainrsrc);
#ifdef XXXX
						ADVupdateundist(dbsort->fileno,NULL,"PO CREDIT CODE","PO DEBIT CODE",NULL,(-1),byrnum,poprocyr,podeftype,"",CALENDAR_MONTHS[procmo],disenc_amount,SCRNvirtualSubData,mainrsrc);
#endif
						}
						break;
					case BAL_ACCT:
						if(purord->use_fiscal_months==TRUE)
						{
						ADVupdateundist(dbsort->fileno,NULL,"PO ACCOUNT CODE","PO CREDIT CODE",NULL,(-1),byrnum,poprocyr,podeftype,"",FISCALPERIOD[procmo],-disenc_amount,SCRNvirtualSubData,mainrsrc);
#ifdef XXXX
						ADVupdateundist(dbsort->fileno,NULL,"PO CREDIT CODE","PO ACCOUNT CODE",NULL,(-1),byrnum,poprocyr,podeftype,"",FISCALPERIOD[procmo],disenc_amount,SCRNvirtualSubData,mainrsrc);
#endif
						} else {
						ADVupdateundist(dbsort->fileno,NULL,"PO ACCOUNT CODE","PO CREDIT CODE",NULL,(-1),byrnum,poprocyr,podeftype,"",CALENDAR_MONTHS[procmo],-disenc_amount,SCRNvirtualSubData,mainrsrc);
#ifdef XXXX
						ADVupdateundist(dbsort->fileno,NULL,"PO CREDIT CODE","PO ACCOUNT CODE",NULL,(-1),byrnum,poprocyr,podeftype,"",CALENDAR_MONTHS[procmo],disenc_amount,SCRNvirtualSubData,mainrsrc);
#endif
						}
						break;
					default:
						prterr("Invalid Account Type [%d]",accttype);
						break;
				}
				switch(accttype)
				{
					case EXP_ACCT:
						if(venpmt->use_fiscal_months==TRUE)
						{
						ADVupdateundist(dbsort->fileno,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE","PAYABLE CODE",eyrnum,byrnum,procyr,deftype,"EXPENDITURES",FISCALPERIOD[procmo],amount,SCRNvirtualSubData,mainrsrc);
						} else {
						ADVupdateundist(dbsort->fileno,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE","PAYABLE CODE",eyrnum,byrnum,procyr,deftype,"EXPENDITURES",CALENDAR_MONTHS[procmo],amount,SCRNvirtualSubData,mainrsrc);
						}
						break;
					case REV_ACCT:
						if(venpmt->use_fiscal_months==TRUE)
						{
						ADVupdateundist(dbsort->fileno,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE","PAYABLE CODE",ryrnum,byrnum,procyr,deftype,"REVENUE",FISCALPERIOD[procmo],amount,SCRNvirtualSubData,mainrsrc);
						} else {
						ADVupdateundist(dbsort->fileno,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE","PAYABLE CODE",ryrnum,byrnum,procyr,deftype,"REVENUE",CALENDAR_MONTHS[procmo],amount,SCRNvirtualSubData,mainrsrc);
						}
						break;
					case BAL_ACCT:
						if(amount>0.0)
						{
						if(venpmt->use_fiscal_months==TRUE)
						{
							ADVupdateundist(dbsort->fileno,NULL,"DEBIT CODE","CREDIT CODE","PAYABLE CODE",(-1),byrnum,procyr,deftype,"",FISCALPERIOD[procmo],amount,SCRNvirtualSubData,mainrsrc);
						} else {
							ADVupdateundist(dbsort->fileno,NULL,"DEBIT CODE","CREDIT CODE","PAYABLE CODE",(-1),byrnum,procyr,deftype,"",CALENDAR_MONTHS[procmo],amount,SCRNvirtualSubData,mainrsrc);
						}
						} else {
						if(venpmt->use_fiscal_months==TRUE)
						{
							ADVupdateundist(dbsort->fileno,NULL,"CREDIT CODE","DEBIT CODE","PAYABLE CODE",(-1),byrnum,procyr,deftype,"",FISCALPERIOD[procmo],-amount,SCRNvirtualSubData,mainrsrc);
						} else {
							ADVupdateundist(dbsort->fileno,NULL,"CREDIT CODE","DEBIT CODE","PAYABLE CODE",(-1),byrnum,procyr,deftype,"",CALENDAR_MONTHS[procmo],-amount,SCRNvirtualSubData,mainrsrc);
						}
						}
						break;
					default:
						prterr("Invalid Account Type [%d]",accttype);
						break;
				}
				if(venpmt->use_venpmt_fund)
				{
				if(venpmt->use_venpmt_fund)
				{
					ADVupdateundist(dbsort->fileno,NULL,"CLEARING DUE FROM","CLEARING CASH",NULL,(-1),byrnum,procyr,cdeftype,"",FISCALPERIOD[procmo],amount,SCRNvirtualSubData,mainrsrc);
				} else {
					ADVupdateundist(dbsort->fileno,NULL,"CLEARING DUE FROM","CLEARING CASH",NULL,(-1),byrnum,procyr,cdeftype,"",CALENDAR_MONTHS[procmo],amount,SCRNvirtualSubData,mainrsrc);
				}
				}
/* update PURTRAN file */
				ZERNRD(trannum);
				FIELDCOPY(dbsort->fileno,"PO FISCAL YEAR",trannum,"FISCAL YEAR");
				COPYFIELD(dbsort->fileno,trannum,"PO IDENTIFICATION");
				FIELDCOPY(dbsort->fileno,"PO LINE NUMBER",trannum,"LINE NUMBER");
				FIELDCOPY(dbsort->fileno,"PO ACCOUNT NUMBER",trannum,"ACCOUNT NUMBER");
				FINDFLDSETSHORT(trannum,"PROCESSING MONTH",procmo);
				COPYFIELD(dbsort->fileno,trannum,"SOURCE USER");
				COPYFIELD(dbsort->fileno,trannum,"ENTRY DATE");
				COPYFIELD(dbsort->fileno,trannum,"ENTRY TIME");
/* Changed per DCT's XPERT PLAN for audit trail changes
				FINDFLDSETCHAR(trannum,"SOURCE USER VERIFIED",TRUE);
*/
				FINDFLDSETCHAR(trannum,"SOURCE USER VERIFIED",FALSE);
				FINDFLDSETCHAR(trannum,"VOUCHER DISENCUMBRANCE",TRUE);
				if(amount>disenc_amount)
				{
					initamt=((-1)*disenc_amount);
				} else {
					initamt=((-1)*amount);
				}
				FINDFLDSETDOUBLE(trannum,"AMOUNT",initamt);
				ADVWRTTRANSsec(trannum,1,"TRANSACTION NUMBER",NULL,SCRNvirtualSubData,mainrsrc);
				++PO_COUNT;
				if(poaccttype==EXP_ACCT)
				{
					FINDFLDGETINT(trannum,"FISCAL YEAR",&yearid);
					FINDFLDGETINT(trannum,"LINE NUMBER",&lineno);
					FINDFLDGETSTRING(trannum,"PO IDENTIFICATION",&poid);
					ADV2updateoutstanding(msrnum,linenum,yearid,poid,lineno,initamt,FALSE,SCRNvirtualSubData,mainrsrc);
				}
				FINDFLDGETCHAR(dbsort->fileno,"FINALED",&finaled);
				if(finaled)
				{
					ZERNRD(accnum);
					FIELDCOPY(dbsort->fileno,"PO FISCAL YEAR",accnum,"FISCAL YEAR");
					COPYFIELD(dbsort->fileno,accnum,"PO IDENTIFICATION");
					FIELDCOPY(dbsort->fileno,"PO LINE NUMBER",accnum,"LINE NUMBER");
					FIELDCOPY(dbsort->fileno,"PO ACCOUNT NUMBER",accnum,"ACCOUNT NUMBER");
					LOCNRDFILE(accnum);
					if(!ADVEQLNRDsec(accnum,1,SCRNvirtualSubData,mainrsrc))
					{
						prev=RDATDataNEW(accnum);
						FINDFLDSETCHAR(accnum,"FINALED",finaled);
						FIELDCOPY(trannum,"ENTRY DATE",accnum,"FINALED DATE");
						FIELDCOPY(trannum,"ENTRY TIME",accnum,"FINALED TIME");
						FIELDCOPY(trannum,"SOURCE USER",accnum,"FINALED BY");
						WRTTRANS(accnum,0,NULL,prev);
						if(prev!=NULL) FreeRDATData(prev);
					}
					UNLNRDFILE(accnum);
				}
				if(ClosePO)
				{
					COPYFIELD(dbsort->fileno,msrnum,"PO IDENTIFICATION");
					FIELDCOPY(dbsort->fileno,"PO FISCAL YEAR",msrnum,"FISCAL YEAR");
					LOCNRDFILE(msrnum);
					if(!ADVEQLNRDsec(msrnum,1,SCRNvirtualSubData,mainrsrc))
					{
						prev=RDATDataNEW(msrnum);
						FINDFLDSETCHAR(msrnum,"CLOSED",ClosePO);
						FINDFLDSETSTRING(msrnum,"CLOSED BY",USERLOGIN);
						FINDFLDSETSTRING(msrnum,"CLOSED DATE",CURRENT_DATE);
						FINDFLDSETSTRING(msrnum,"CLOSED TIME",CURRENT_TIME);
						WRTTRANS(msrnum,0,NULL,prev);
						if(prev!=NULL) FreeRDATData(prev);
					}
					UNLNRDFILE(msrnum);
				}
			}
			ef=NXTNRD(dbsort->fileno,1);
		}
		if(poid!=NULL) Rfree(poid);
		CLSDBsort(dbsort);
		FreeDBsort(dbsort);
		updatebrowse(TRUE,mbl_addpo,msrnum,module,mbl_addpo->mainrsrc);
	}	
}
static void quitback(RDArsrc *mainrsrc,MakeBrowseList *blist)
{
	;
}
static void saveexittest(RDArsrc *mainrsrc,MakeBrowseList *blist)
{
	if(total_po!=invoice_total && invoice_total!=0)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","INVOICE BALANCE WARNING",
			"Exiting ADD PO VOUCHERS with the total of vouchers out of balance with the total invoice.  Please correct this before continuing.",
			saveexit,quitback,FALSE,2,mainrsrc,blist,NULL);
	} else {
		saveexit(mainrsrc,blist);
	}
}
static void doexit_test(MakeBrowseList *blist)
{
	DBsort *dbsort=NULL;
	
	dbsort=blist->funcargs;
	if(dbsort!=NULL)
	{
		WARNINGDIALOG("ABORT WARNING DIALOG SCREEN","RECORDS NOT PROCESSED!","Are you sure that you want to quit without processing the Purchase Order Vouchers?",doexit,NULL,FALSE,blist);
	} else {
		doexit(blist);
	}
}
static void doexit(MakeBrowseList *blist)
{
	DBsort *dbsort=NULL;
	RDArsrc *brsrc=NULL,*srsrc=NULL,*drsrc=NULL;

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
	if(dbsort!=NULL)
	{
		CLSDBsort(dbsort);
		FreeDBsort(dbsort);
	}
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	purapvm(0,targetkey,0,NULL);
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	purapvm(2,NULL,0,NULL);
} 
