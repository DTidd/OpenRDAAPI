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

struct PassableStructs
{
	RDArsrc *r;
	void *a;
};
typedef struct PassableStructs PassableStruct;

void PurApvSubData(char **,PassableStruct *);

extern RDApurord *purord;
extern RDAvenpmt *venpmt;
extern char set_copy_desc;
extern void purapvm(short,void *,int,char **);
static void PURAPV1_doexit(MakeBrowseList *),PURAPV1_okfunc(MakeBrowseList *,void *);
static void PURAPV1_doexit_test(MakeBrowseList *);
static void saveexit(RDArsrc *,MakeBrowseList *,short);
static void saveexittest(RDArsrc *,MakeBrowseList *);
static void saveexittestnu(RDArsrc *,MakeBrowseList *);
static void new_record(RDArsrc *,MakeBrowseList *);
static short PURAPV1_selectfunction(MakeBrowseList *);
extern short msrnum,sitenum,vennum,venadrnum,percis,perctrn,alias_num;
extern short linenum,accnum,trannum,infnum,invnum,vchnum;
extern short eyrnum,ryrnum,byrnum,yearnum,fyearnum;
extern short expnum,revnum,balnum,finyrnum;
extern RDAfinmgt *finsetup;
extern char *module;
extern char *poid,*vendorid,*invid,*checkdate;
extern char manual_voucher,prepaid,pay_1099,check_detail;
extern char use_grant_management;
extern int venyrid,yearid,checkno,PO_COUNT;
extern short POV_DESCRIPTION,prtapo_type;
extern short pmonth;
extern short fmonth;
extern MakeBrowseList *mbl_addpo;
MakeBrowseList *mbl_rec=NULL;
void browse_purapv(RDArsrc *);
DBsort *dbsort=NULL;
extern char ClosePO;
extern double invoice_total;
extern char prorate_excess;
extern double amt2prorate;
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
		addDBfield(dbsrt,"DISENCUMBRANCE COST",DOLLARS,20);
		addDBfield(dbsrt,"DISENCUMBRANCE TAX AMOUNT",DOLLARS,20);
		addDBfield(dbsrt,"DISENCUMBRANCE ADMIN FEE",DOLLARS,20);
		addDBfield(dbsrt,"DISENCUMBRANCE PRORATE AMOUNT",DOLLARS,20);
		addDBfield(dbsrt,"PO FISCAL YEAR",LONGV,4);
		addDBfield(dbsrt,"PO ACCOUNT TYPE",SHORTV,2);
		addDBfield(dbsrt,"PO DEFINITION TYPE",SHORTV,2);
		addDBfield(dbsrt,"PO ACCOUNT CODE",EXPENDITURE,40);
		addDBfield(dbsrt,"PO DEBIT CODE",BALANCESHEET,40);
		addDBfield(dbsrt,"PO CREDIT CODE",BALANCESHEET,40);
		addDBfield(dbsrt,"FINALED",BOOLNS,1);
		addDBfield(dbsrt,"JOURNAL IDENTIFICATION",PLAINTEXT,15);
		addDBfield(dbsrt,"GRANT IDENTIFICATION",PLAINTEXT,15);
		if(XPERT_SETUP->CIS==TRUE)
		{
			addDBfield(dbsrt,"USE CIS",BOOLNS,1);
			addDBfield(dbsrt,"PERSONNEL IDENTIFICATION",PLAINTEXT,15);
			addDBfield(dbsrt,"CIS TYPE",PLAINTEXT,15);
		}
	}
	if(prtapo_type==0)
	{
		addDBfield(dbsrt,"ITEM IDENTIFICATION",1,15);
	} else {
		addDBfield(dbsrt,"ITEM IDENTIFICATION",1,20);
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
void addsortrecord(RDArsrc *parent,DBsort *dbsrt,int lineno,int accno,double cost,double tax_amount,double adminfee,double prorateamt,double amount,char skip_records)
{
	char falseflag=FALSE,*grantid=NULL,eft=FALSE;
	short accttype=0,deftypex=0,src=1;
	NRDfield *field=NULL;

	FINDFLDSETCHAR(dbsrt->fileno,"DELETEFLAG",skip_records);
	FINDFLDSETSHORT(dbsrt->fileno,"SOURCE",src);
	FINDFLDSETINT(dbsrt->fileno,"CALENDAR YEAR",venyrid);
	FINDFLDSETSTRING(dbsrt->fileno,"PO IDENTIFICATION",poid);
	FINDFLDSETSTRING(dbsrt->fileno,"VENDOR IDENTIFICATION",vendorid);
	FINDFLDSETINT(dbsrt->fileno,"FISCAL YEAR",yearid);
	FINDFLDSETINT(dbsrt->fileno,"PO FISCAL YEAR",yearid);
	FINDFLDSETINT(dbsrt->fileno,"PO LINE NUMBER",lineno);
	FINDFLDSETINT(dbsrt->fileno,"PO ACCOUNT NUMBER",accno);
	FINDFLDSETSTRING(dbsrt->fileno,"INVOICE IDENTIFICATION",invid);
	FINDFLDSETSHORT(dbsrt->fileno,"PROCESSING MONTH",pmonth);
	FINDFLDSETSHORT(dbsrt->fileno,"FISCAL PROCESSING MONTH",fmonth);
	FINDFLDSETDOUBLE(dbsrt->fileno,"AMOUNT",amount);
	if(venpmt->Use_Taxes)
	{
		FINDFLDSETDOUBLE(dbsrt->fileno,"COST",cost);
		FINDFLDSETDOUBLE(dbsrt->fileno,"DISENCUMBRANCE COST",cost);
		FINDFLDSETDOUBLE(dbsrt->fileno,"TAX AMOUNT",tax_amount);
		FINDFLDSETDOUBLE(dbsrt->fileno,"DISENCUMBRANCE TAX AMOUNT",tax_amount);
		FINDFLDSETDOUBLE(dbsrt->fileno,"ADMIN FEE",adminfee);
		FINDFLDSETDOUBLE(dbsrt->fileno,"PRORATE AMOUNT",prorateamt);
		FINDFLDSETDOUBLE(dbsrt->fileno,"DISENCUMBRANCE ADMIN FEE",adminfee);
		FINDFLDSETDOUBLE(dbsrt->fileno,"DISENCUMBRANCE PRORATE AMOUNT",prorateamt);
	}
	FINDFLDSETCHAR(dbsrt->fileno,"FINALED",falseflag);
	FINDFLDSETDOUBLE(dbsrt->fileno,"DISENCUMBRANCE AMOUNT",amount);
	FINDFLDSETCHAR(dbsrt->fileno,"MANUAL VOUCHER",manual_voucher);
	FINDFLDSETINT(dbsrt->fileno,"CHECK NUMBER",checkno);
	/* 
	FINDFLDSETINT(dbsrt->fileno,"VIRTUAL CHECK NUMBER",checkno); 
	FINDFLDSETSTRING(dbsrt->fileno,"CHECK DATE",checkdate);
	*/
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
	COPYFIELD(linenum,dbsrt->fileno,"ITEM IDENTIFICATION");
	switch(POV_DESCRIPTION)
	{
		default:
		case 0:
			break;
		case 1:
			COPYFIELD(linenum,dbsrt->fileno,"DESCRIPTION");
			break;
		case 2:
			FIELDCOPY(linenum,"ITEM IDENTIFICATION",dbsrt->fileno,"DESCRIPTION");
			break;
	}
	COPYFIELD(accnum,dbsrt->fileno,"JOURNAL IDENTIFICATION");
	FINDFLDGETSHORT(accnum,"ACCOUNT TYPE",&accttype);
	FINDFLDSETSHORT(dbsrt->fileno,"ACCOUNT TYPE",accttype);
	FINDFLDSETSHORT(dbsrt->fileno,"PO ACCOUNT TYPE",accttype);
	FINDFLDGETSHORT(accnum,"DEFINITION TYPE",&deftypex);
	FINDFLDSETSHORT(dbsrt->fileno,"DEFINITION TYPE",deftypex);
	FINDFLDSETSHORT(dbsrt->fileno,"PO DEFINITION TYPE",deftypex);
	FIELDCOPY(accnum,"DEFAULT BANK IDENTIFICATION",dbsrt->fileno,"BANK IDENTIFICATION");
	if(XPERT_SETUP->CIS==TRUE)
	{
		FIELDCOPY(accnum,"CIS",dbsrt->fileno,"USE CIS");
		COPYFIELD(accnum,dbsrt->fileno,"CIS TYPE");
		COPYFIELD(accnum,dbsrt->fileno,"PERSONNEL IDENTIFICATION");
	}
	switch(accttype)
	{
		default:
		case EXP_ACCT:
			if(use_grant_management)
			{
				COPYFIELD(accnum,expnum,"DEFINITION TYPE");
				COPYFIELD(accnum,expnum,"ACCOUNT CODE");
				if(ADVEQLNRDsec(expnum,1,SCRNvirtualSubData,parent)) KEYNRD(expnum,1);
				COPYFIELD(expnum,dbsrt->fileno,"GRANT IDENTIFICATION");
				FINDFLDGETSTRING(expnum,"GRANT IDENTIFICATION",&grantid);
				if((grantid!=NULL) && !isEMPTY(grantid))
				{
					FINDFLDSETINT(dbsrt->fileno,"GRANT YEAR",yearid);
				}
			}
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
				FIELDCOPY(accnum,"DEFAULT CLEARING DEFINITION TYPE",
					dbsrt->fileno,"CLEARING DEFINITION TYPE");
				FIELDCOPY(accnum,"DEFAULT CLEARING DUE FROM",
					dbsrt->fileno,"CLEARING DUE FROM");
				FIELDCOPY(accnum,"DEFAULT CLEARING CASH",
					dbsrt->fileno,"CLEARING CASH");
			}
			break;
		case BAL_ACCT:
			FIELDCOPY(accnum,"DEFAULT DEBIT CODE",dbsrt->fileno,
				"DEBIT CODE");
			FIELDCOPY(accnum,"CREDIT CODE",dbsrt->fileno,
				"PO CREDIT CODE");
			FIELDCOPY(accnum,"DEFAULT CREDIT CODE",dbsrt->fileno,
				"CREDIT CODE");
			FIELDCOPY(accnum,"DEFAULT PAYABLE CODE",dbsrt->fileno,
				"PAYABLE CODE");
			if(venpmt->use_venpmt_fund)
			{
				FIELDCOPY(accnum,"DEFAULT CLEARING DEFINITION TYPE",
					dbsrt->fileno,"CLEARING DEFINITION TYPE");
				FIELDCOPY(accnum,"DEFAULT CLEARING DUE FROM",
					dbsrt->fileno,"CLEARING DUE FROM");
				FIELDCOPY(accnum,"DEFAULT CLEARING CASH",
					dbsrt->fileno,"CLEARING CASH");
			}
			break;
	}
	field=FLDNRD(vchnum,"ENTRY DATE");
	if(field!=NULL)
	{
		if(field->len<10)
		{
			FINDFLDSETSTRING(dbsrt->fileno,"ENTRY DATE",CURRENT_DATE);
		} else {
			FINDFLDSETSTRING(dbsrt->fileno,"ENTRY DATE",CURRENT_DATE10);
		}
	} else FINDFLDSETSTRING(dbsrt->fileno,"ENTRY DATE",CURRENT_DATE);
	FINDFLDSETSTRING(dbsrt->fileno,"ENTRY TIME",CURRENT_TIME);
	FINDFLDSETSTRING(dbsrt->fileno,"SOURCE USER",USERLOGIN);
	FINDFLDGETCHAR(vennum,"ELECTRONIC TRANSFER",&eft);
	FINDFLDSETCHAR(dbsrt->fileno,"ELECTRONIC TRANSFER",eft);
	FIELDCOPY(vennum,"DEFAULT 1099 PAYMENT",dbsort->fileno,"1099 PAYMENT");
	FIELDCOPY(vennum,"DEFAULT 1099 TYPE",dbsort->fileno,"1099 TYPE");
	if(skip_records==FALSE) total_po+=amount;
	if(set_copy_desc==TRUE)
	{
	if(alias_num!=(-1))
	{
		ZERNRD(alias_num);
		COPYFIELD(dbsrt->fileno,alias_num,"ACCOUNT TYPE");
		COPYFIELD(dbsrt->fileno,alias_num,"DEFINITION TYPE");
		if(accttype<2)
		{
			COPYFIELD(dbsrt->fileno,alias_num,"ACCOUNT CODE");
		} else {
			FIELDCOPY(dbsrt->fileno,"DEBIT CODE",alias_num,"ACCOUNT CODE");
		}
		if(ADVEQLNRDsec(alias_num,1,SCRNvirtualSubData,parent)) KEYNRD(alias_num,1);
		FIELDCOPY(alias_num,"DESCRIPTION",dbsrt->fileno,"DESCRIPTION");
	}
	}
	WRTNRD(dbsrt->fileno,1,"TRANSACTION NUMBER");
}
void ProrateExcess(DBsort *db,short prorate_type,double prorate_amount,double total_cost,double total_tax,double total_adminfee,double total_prorate,double total)
{
	short ef=FALSE;
	char deleteflag=FALSE;
	double amount=0.0,new_part=0.0,new_amount=0.0,remain_amt=0;
	double cost=0.0,tax_amount=0.0;
	double proamt=0.0,new_proamt=0.0,adminfee=0.0;
	double new_cost=0.0,new_adminfee=0.0,new_tax_amount=0.0;
	
	ZERNRD(db->fileno);
	remain_amt=prorate_amount;
	ef=FRSNRD(db->fileno,1);
	while(!ef)
	{
		FINDFLDGETCHAR(db->fileno,"DELETEFLAG",&deleteflag);
		new_amount=0.0;
		if(remain_amt<=0.0) break;
		if(deleteflag==FALSE)
		{
			if(venpmt->Use_Taxes==FALSE)
			{
				FINDFLDGETDOUBLE(db->fileno,"AMOUNT",&amount);
				if(prorate_type==FALSE) /* invoice total */
				{
					new_amount=round((prorate_amount*amount/total));
					if(new_amount>remain_amt) new_amount=remain_amt;
					remain_amt-=new_amount;
				} else { /* shipping charges */
					new_part=(round((prorate_amount*amount/total)));
					if(new_part>remain_amt) new_part=remain_amt;
					new_amount=amount+new_part;
					remain_amt-=new_part;
					
				}
				FINDFLDSETDOUBLE(db->fileno,"AMOUNT",new_amount);
				if(new_amount!=amount) 
				{
					total_po+=(new_amount-amount);
					WRTNRD(db->fileno,0,NULL);
				}
			} else {
				FINDFLDGETDOUBLE(db->fileno,"COST",&cost);
				FINDFLDGETDOUBLE(db->fileno,"TAX AMOUNT",&tax_amount);
				FINDFLDGETDOUBLE(db->fileno,"ADMIN FEE",&adminfee);
				FINDFLDGETDOUBLE(db->fileno,"PRORATE AMOUNT",&proamt);
				FINDFLDGETDOUBLE(db->fileno,"AMOUNT",&amount);
				if(prorate_type==FALSE) /* invoice total */
				{
					new_amount=round((prorate_amount*amount/total));
					new_cost=round((cost*new_amount/amount));
					new_tax_amount=round((tax_amount*new_amount/amount));
					new_adminfee=round((adminfee*new_amount/amount));
					new_proamt=round((proamt*new_amount/amount));
					if(new_amount!=(new_cost+new_tax_amount+new_adminfee+new_proamt))
					{
						new_tax_amount=new_amount-new_cost-new_adminfee-new_proamt;
					}
					FINDFLDSETDOUBLE(db->fileno,"COST",new_cost);
					FINDFLDSETDOUBLE(db->fileno,"TAX AMOUNT",new_tax_amount);
					FINDFLDSETDOUBLE(db->fileno,"ADMIN FEE",new_adminfee);
					FINDFLDSETDOUBLE(db->fileno,"PRORATE AMOUNT",new_proamt);
					if(new_amount>remain_amt) new_amount=remain_amt;
					remain_amt-=new_amount;
				} else { /* prorate some amount */
					new_part=(round((prorate_amount*cost/total_cost)));
					if(new_part>remain_amt) new_part=remain_amt;
					new_proamt=proamt+new_part;
					new_amount=amount+new_part;
					remain_amt-=new_part;
					FINDFLDSETDOUBLE(db->fileno,"PRORATE AMOUNT",new_proamt);
				}
				FINDFLDSETDOUBLE(db->fileno,"AMOUNT",new_amount);
				if(new_amount!=amount) 
				{
					total_po+=(new_amount-amount);
					WRTNRD(db->fileno,0,NULL);
				}
			}			
		}
		ef=NXTNRD(db->fileno,1);
	}
}
void browse_purapv(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL,*tmprsrc=NULL;
	char *sortname=NULL,deleteflag=FALSE;
	short ef=0;
	char *last_po=NULL,*test_po=NULL;
	int last_year=0,last_line=0,last_acct=0;
	int test_year=0,test_line=0,test_acct=0;
	double cost=0,amount=0;
	double adminfee=0,prorateamt=0,tax_amount=0;
	double total_adminfee=0.0,total_prorateamt=0.0;
	double total_cost=0.0,total_tax_amount=0.0,total_amount=0.0;
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
	addDFincvir(mainrsrc,"VENPMT","VENINF",NULL,infnum);
	addDFincvir(mainrsrc,"VENPMT","VENYR",NULL,yearnum);
	addDFincvir(mainrsrc,"VENPMT","VENMSR",NULL,vennum);
	addDFincvir(mainrsrc,"VENPMT","VENADR",NULL,venadrnum);
	addDFincvir(mainrsrc,module,"PURLIN",NULL,linenum);
	addDFincvir(mainrsrc,module,"PURACC",NULL,accnum);
	addDFincvir(mainrsrc,module,"PURMST",NULL,msrnum);
	addDFincvir(searchrsrc,"VENPMT","VENINV",NULL,invnum);
	addDFincvir(searchrsrc,"VENPMT","VENINF",NULL,infnum);
	addDFincvir(searchrsrc,"VENPMT","VENYR",NULL,yearnum);
	addDFincvir(searchrsrc,"VENPMT","VENMSR",NULL,vennum);
	addDFincvir(searchrsrc,module,"PURLIN",NULL,linenum);
	addDFincvir(definelist,"VENPMT","VENINV",NULL,invnum);
	addDFincvir(definelist,"VENPMT","VENINF",NULL,infnum);
	addDFincvir(searchrsrc,"VENPMT","VENYR",NULL,yearnum);
	addDFincvir(definelist,"VENPMT","VENYR",NULL,yearnum);
	addDFincvir(definelist,"VENPMT","VENMSR",NULL,vennum);
	addDFincvir(definelist,module,"PURLIN",NULL,linenum);
	addDFincvir(definelist,module,"PURTRN",NULL,trannum);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
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
	if(msrnum!=(-1))
	{
		file2rsrc(msrnum,mainrsrc,FALSE);
		ZERNRD(msrnum);
		FINDFLDSETINT(msrnum,"FISCAL YEAR",yearid);
		FINDFLDSETSTRING(msrnum,"PO IDENTIFICATION",poid);
		if(ADVEQLNRDsec(msrnum,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(msrnum,1);
		filerecord2rsrc(msrnum,mainrsrc);
	}
	if(infnum!=(-1))
	{
		file2rsrc(infnum,mainrsrc,FALSE);
		FINDFLDSETSTRING(infnum,"VENDOR IDENTIFICATION",vendorid);
		FINDFLDSETSTRING(infnum,"INVOICE IDENTIFICATION",invid);
		if(ADVEQLNRDsec(infnum,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(infnum,1);
		filerecord2rsrc(infnum,mainrsrc);
	}
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
	}
	if(venadrnum!=(-1))
	{
		file2rsrc(venadrnum,mainrsrc,FALSE);
		FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid);
		if(ADVEQLNRDsec(venadrnum,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(venadrnum,1);
		filerecord2rsrc(venadrnum,mainrsrc);
	}
	tmprsrc=diagscrn(accnum,"DIAGNOSTIC SCREEN",module,
		"Creating / Defaulting Vouchers",NULL);
	addDFincvir(tmprsrc,module,"PURTRAN",NULL,accnum);
	GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
	if(tmprsrc!=NULL)
		if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE)) ForceWindowUpdate(tmprsrc);
	dbsort->fileno=OPNDBsort(dbsort);
	ZERNRD(accnum);
	FINDFLDSETSTRING(accnum,"PO IDENTIFICATION",poid);
	FINDFLDSETINT(accnum,"FISCAL YEAR",yearid);
	ef=ADVGTENRDsec(accnum,1,SCRNvirtualSubData,tmprsrc);
	FINDFLDGETSTRING(accnum,"PO IDENTIFICATION",&last_po);
	FINDFLDGETINT(accnum,"FISCAL YEAR",&last_year);
	FINDFLDGETINT(accnum,"LINE NUMBER",&last_line);
	FINDFLDGETINT(accnum,"ACCOUNT NUMBER",&last_acct);
	while(!ef)
	{
		FINDFLDGETSTRING(accnum,"PO IDENTIFICATION",&test_po);
		FINDFLDGETINT(accnum,"FISCAL YEAR",&test_year);
		if(RDAstrcmp(poid,test_po) || yearid!=test_year) break;
		FINDFLDGETCHAR(accnum,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(accnum,"FINALED",&finaled);
		if(!deleteflag && !finaled)
		{
			FINDFLDGETINT(accnum,"LINE NUMBER",&test_line);
			FINDFLDGETINT(accnum,"ACCOUNT NUMBER",&test_acct);
			FINDFLDGETDOUBLE(accnum,"OUTSTANDING ACCOUNT AMOUNT",&amount);
			if(amount!=0)
			{
				if(venpmt->Use_Taxes==TRUE)
				{
					FINDFLDGETDOUBLE(accnum,"OUTSTANDING ACCOUNT COST",&cost);
					FINDFLDGETDOUBLE(accnum,"OUTSTANDING ACCOUNT TAX AMOUNT",&tax_amount);
					FINDFLDGETDOUBLE(accnum,"OUTSTANDING ACCOUNT ADMIN FEE",&adminfee);
					FINDFLDGETDOUBLE(accnum,"OUTSTANDING ACCOUNT PRORATE AMOUNT",&prorateamt);
				}
				addsortrecord(mainrsrc,dbsort,test_line,test_acct,cost,tax_amount,adminfee,prorateamt,amount,skip_records);
				total_amount+=amount;
				total_cost+=cost;
				if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
			}
		} else {
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,FALSE);
		}
		ef=ADVNXTNRDsec(accnum,1,SCRNvirtualSubData,tmprsrc);
	}
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	if(last_po!=NULL) Rfree(last_po);
	if(test_po!=NULL) Rfree(test_po);
	size_of_file=SIZNRD(dbsort->fileno);
	if(size_of_file>0)
	{
		if((prorate_excess==TRUE) && (invoice_total!=0.0))
		{
			ProrateExcess(dbsort,FALSE,invoice_total,total_cost,total_tax_amount,total_adminfee,total_prorateamt,total_amount);
		} else if(amt2prorate!=0.0)
		{
			ProrateExcess(dbsort,TRUE,amt2prorate,total_cost,total_tax_amount,total_adminfee,total_prorateamt,total_amount);
		}
		addbtnrsrc(mainrsrc,"SAVE",TRUE,saveexittest,mbl_rec);
		addbtnrsrc(mainrsrc,"SAVE NO UPDATE",TRUE,saveexittestnu,mbl_rec);
		addstdrsrc(mainrsrc,"TOTAL PO",DOLLARS,20,&total_po,TRUE);
		mbl_rec=ADVbrowse(dbsort->fileno,1,mainrsrc,searchrsrc,definelist,NULL,PURAPV1_selectfunction,PURAPV1_doexit_test,PURAPV1_okfunc,new_record,dbsort,TRUE,NULL,0,0,0,TRUE);
		FINDRSCSETFUNC(mainrsrc,"SAVE",saveexittest,mbl_rec);
		FINDRSCSETFUNC(mainrsrc,"SAVE NO UPDATE",saveexittestnu,mbl_rec);
	} else {
		ERRORDIALOG("Add Purchase Order Vouchers","No records selected using from the purchase order files.  Please try again.",NULL,FALSE);
		CLSDBsort(dbsort);
		FreeDBsort(dbsort);
	}
}
static short PURAPV1_selectfunction(MakeBrowseList *blist)
{
	ZERNRD(msrnum);
	COPYFIELD(blist->fileno,msrnum,"PO IDENTIFICATION");
	FIELDCOPY(blist->fileno,"PO FISCAL YEAR",msrnum,"FISCAL YEAR");
	if(EQLNRD(blist->fileno,1)) KEYNRD(msrnum,1);
	ZERNRD(linenum);
	FIELDCOPY(blist->fileno,"PO FISCAL YEAR",linenum,"FISCAL YEAR");
	COPYFIELD(blist->fileno,linenum,"PO IDENTIFICATION");
	FIELDCOPY(blist->fileno,"PO LINE NUMBER",linenum,"LINE NUMBER");
	if(EQLNRD(blist->fileno,1)) KEYNRD(linenum,1);
	return(TRUE);
}
static void saveexit(RDArsrc *mainrsrc,MakeBrowseList *blist,short updateb)
{
	NRDfield *field=NULL,*fields=NULL;
	short x,nofields=0,ef=FALSE;
	short accttype=0,poaccttype=0,deftype=0,podeftype=0;
	short cdeftype=0;
	char flag=FALSE,finaled=FALSE;
	char *poid=NULL,Write_Tax_Invoice=FALSE;
	double amount=0.0,disenc_amount=0.0,initamt=0.0;
	double cost=0.0,disenc_cost=0.0,initcost=0.0;
	double tax_amount=0.0,disenc_tax=0.0,inittax=0.0;
	double adminfee=0.0,disenc_admin=0.0,initadmin=0.0;
	double prorateamt=0.0,disenc_prorate=0.0,initprorate=0.0;
	double totalnotax=0.0,acc_admin=0.0,acc_amt=0.0,acc_cost=0.0;
	double acc_prorate=0.0,acc_tax=0.0;
	double amt1=0;
	short procmo=0,tax_type=0;
	char sep_vendor=FALSE,use_tax=FALSE,usecis=FALSE,ddraft=FALSE;
	char eft=FALSE;
	char *Default_Tax_Vendor=NULL,*Default_Tax_Address=NULL;
	int lineno=0,procyr=0,poprocyr=0,yearid=0;
	int noinv=0;
	RDATData *prev=NULL;
	double totalpartsamount=0.0;

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
			FINDFLDGETDOUBLE(dbsort->fileno,"AMOUNT",&amount);
			if(!flag && amount!=0.0)
			{
				ZERNRD(vennum);
				COPYFIELD(dbsort->fileno,vennum,"VENDOR IDENTIFICATION");
				if(ADVEQLNRDsec(vennum,1,SCRNvirtualSubData,mainrsrc))
				{
					ZERNRD(vennum);
					prterr("Error Vendor Master not found.");
				}
				FINDFLDGETSHORT(vennum,"DEFAULT TAX TYPE",&tax_type);
				FINDFLDGETCHAR(dbsort->fileno,"ELECTRONIC TRANSFER",&eft);
				if(eft)
				{
					FINDFLDGETCHAR(vennum,"DIRECT DRAFT",&ddraft);
				} else ddraft=FALSE;
				switch(tax_type)
				{
					default:
					case 0:
						sep_vendor=venpmt->Sep_Tax1_Vendor;
						use_tax=venpmt->Use_Tax1;
						Default_Tax_Vendor=venpmt->Default_Tax1_Vendor;
						Default_Tax_Address=venpmt->Default_Tax1_Address;
						
						break;
					case 1:
						sep_vendor=venpmt->Sep_Tax2_Vendor;
						use_tax=venpmt->Use_Tax2;
						Default_Tax_Vendor=venpmt->Default_Tax2_Vendor;
						Default_Tax_Address=venpmt->Default_Tax2_Address;
						break;
					case 2:
						sep_vendor=venpmt->Sep_Tax3_Vendor;
						use_tax=venpmt->Use_Tax3;
						Default_Tax_Vendor=venpmt->Default_Tax3_Vendor;
						Default_Tax_Address=venpmt->Default_Tax3_Address;
						break;
					case 3:	
						sep_vendor=venpmt->Sep_Tax4_Vendor;
						use_tax=venpmt->Use_Tax4;
						Default_Tax_Vendor=venpmt->Default_Tax4_Vendor;
						Default_Tax_Address=venpmt->Default_Tax4_Address;
						break;
					case 4:
						sep_vendor=venpmt->Sep_Tax5_Vendor;
						use_tax=venpmt->Use_Tax5;
						Default_Tax_Vendor=venpmt->Default_Tax5_Vendor;
						Default_Tax_Address=venpmt->Default_Tax5_Address;
						break;
				}
/* CREATE INVOICE HERE for SEPERATE VENDOR */
				if(use_tax==TRUE && Write_Tax_Invoice==FALSE && sep_vendor==TRUE)
				{
					ZERNRD(infnum);
					COPYFIELD(dbsort->fileno,infnum,"VENDOR IDENTIFICATION");
					COPYFIELD(dbsort->fileno,infnum,"INVOICE IDENTIFICATION");
					if(!ADVEQLNRDsec(infnum,1,SCRNvirtualSubData,mainrsrc))
					{
						FINDFLDSETCHAR(infnum,"DELETEFLAG",FALSE);
						FINDFLDSETCHAR(infnum,"INVOICE IN USE",TRUE);
						ADVWRTTRANSsec(infnum,0,NULL,NULL,SCRNvirtualSubData,mainrsrc);
					}
					ZERNRD(invnum);
					COPYFIELD(dbsort->fileno,invnum,"VENDOR IDENTIFICATION");
					COPYFIELD(dbsort->fileno,invnum,"CALENDAR YEAR");
					COPYFIELD(dbsort->fileno,invnum,"INVOICE IDENTIFICATION");
					if(!ADVEQLNRDsec(invnum,1,SCRNvirtualSubData,mainrsrc))
					{
						FINDFLDSETSTRING(invnum,"VENDOR IDENTIFICATION",Default_Tax_Vendor);
						FINDFLDSETSTRING(invnum,"ADDRESS IDENTIFICATION",Default_Tax_Address);
						ADVWRTTRANSsec(invnum,0,NULL,NULL,SCRNvirtualSubData,mainrsrc);
						ZERNRD(yearnum);
						COPYFIELD(invnum,yearnum,"VENDOR IDENTIFICATION");
						COPYFIELD(invnum,yearnum,"CALENDAR YEAR");
						if(ADVEQLNRDsec(yearnum,1,SCRNvirtualSubData,mainrsrc))
						{
							prev=RDATDataNEW(yearnum);
							FINDFLDGETINT(yearnum,"NUMBER OF INVOICES ISSUED",&noinv);
							FINDFLDSETINT(yearnum,"NUMBER OF INVOICES ISSUED",(noinv+1));
							WRTTRANS(yearnum,0,NULL,prev);
						} else {
							FINDFLDSETINT(yearnum,"NUMBER OF INVOICES ISSUED",1);
							WRTTRANS(yearnum,0,NULL,NULL);
						}
						if(prev!=NULL) FreeRDATData(prev);
						COPYFIELD(dbsort->fileno,yearnum,"VENDOR IDENTIFICATION");
						COPYFIELD(dbsort->fileno,yearnum,"CALENDAR YEAR");
						ADVEQLNRDsec(yearnum,1,SCRNvirtualSubData,mainrsrc);
						if(venpmt->use_fiscal_summary==TRUE && fyearnum!=(-1))
						{
							COPYFIELD(invnum,fyearnum,"VENDOR IDENTIFICATION");
							COPYFIELD(msrnum,fyearnum,"FISCAL YEAR");
							if(ADVEQLNRDsec(fyearnum,1,SCRNvirtualSubData,mainrsrc))
							{
								KEYNRD(fyearnum,1);
								COPYFIELD(invnum,fyearnum,"VENDOR IDENTIFICATION");
								COPYFIELD(msrnum,fyearnum,"FISCAL YEAR");
								prev=RDATDataNEW(fyearnum);
								FINDFLDGETINT(fyearnum,"NUMBER OF INVOICES ISSUED",&noinv);
								FINDFLDSETINT(fyearnum,"NUMBER OF INVOICES ISSUED",(noinv+1));
								WRTTRANS(fyearnum,0,NULL,prev);
							} else {
								FINDFLDSETINT(fyearnum,"NUMBER OF INVOICES ISSUED",1);
								WRTTRANS(fyearnum,0,NULL,NULL);
							}
							if(prev!=NULL) FreeRDATData(prev);
							COPYFIELD(dbsort->fileno,fyearnum,"VENDOR IDENTIFICATION");
							ADVEQLNRDsec(fyearnum,1,SCRNvirtualSubData,mainrsrc);
						}
					}
					Write_Tax_Invoice=TRUE;
					
				}
				if(fields!=NULL)
				{
					for(x=0,field=fields;x<nofields;++x,++field)
					{
						COPYFIELD(dbsort->fileno,vchnum,field->name);
					}
				}
				FINDFLDGETDOUBLE(dbsort->fileno,"AMOUNT",&amount);
				if(venpmt->Use_Taxes)
				{
					FINDFLDGETDOUBLE(dbsort->fileno,"TAX AMOUNT",&tax_amount);
					FINDFLDGETDOUBLE(dbsort->fileno,"COST",&cost);
					FINDFLDGETDOUBLE(dbsort->fileno,"ADMIN FEE",&adminfee);
					FINDFLDGETDOUBLE(dbsort->fileno,"PRORATE AMOUNT",&prorateamt);
					FINDFLDGETDOUBLE(dbsort->fileno,"DISENCUMBRANCE COST",&disenc_cost);
					FINDFLDGETDOUBLE(dbsort->fileno,"DISENCUMBRANCE TAX AMOUNT",&disenc_tax);
					FINDFLDGETDOUBLE(dbsort->fileno,"DISENCUMBRANCE ADMIN FEE",&disenc_admin);
					FINDFLDGETDOUBLE(dbsort->fileno,"DISENCUMBRANCE PRORATE AMOUNT",&disenc_prorate);
					totalnotax=amount-tax_amount;
				}
				if(sep_vendor)
				{
					FINDFLDSETDOUBLE(vchnum,"TAX AMOUNT",0.0);
					FINDFLDSETDOUBLE(vchnum,"AMOUNT",totalnotax);
				}
				COPYFIELD(dbsort->fileno,vchnum,"JOURNAL IDENTIFICATION");
				if(XPERT_SETUP->CIS==TRUE)
				{
					FIELDCOPY(dbsort->fileno,"USE CIS",vchnum,"CIS");
					COPYFIELD(dbsort->fileno,vchnum,"CIS TYPE");
					COPYFIELD(dbsort->fileno,vchnum,"PERSONNEL IDENTIFICATION");
				}
				FINDFLDSETCHAR(vchnum,"PO VOUCHER",TRUE);
				CURRENT_TIME=GETCURRENTTIME();
				FINDFLDSETSTRING(vchnum,"ENTRY TIME",CURRENT_TIME);
				FINDFLDGETCHAR(dbsort->fileno,"ELECTRONIC TRANSFER",&eft);
				if(!eft) ddraft=FALSE;
				FINDFLDSETCHAR(vchnum,"DIRECT DRAFT",ddraft);
				ADVWRTTRANSsec(vchnum,1,"TRANSACTION NUMBER",NULL,SCRNvirtualSubData,mainrsrc);
				if(sep_vendor)
				{
					if(tax_amount!=0.0)
					{
						FINDFLDSETDOUBLE(vchnum,"COST",0.0);
						FINDFLDSETDOUBLE(vchnum,"TAX AMOUNT",tax_amount);
						FINDFLDSETDOUBLE(vchnum,"PRORATE AMOUNT",0.0);
						FINDFLDSETDOUBLE(vchnum,"ADMIN FEE",0.0);
						FINDFLDSETDOUBLE(vchnum,"AMOUNT",tax_amount);
						FINDFLDSETSTRING(vchnum,"VENDOR IDENTIFICATION",Default_Tax_Vendor);
						FINDFLDSETCHAR(vchnum,"PO VOUCHER",TRUE);
						CURRENT_TIME=GETCURRENTTIME();
						FINDFLDSETSTRING(perctrn,"ENTRY TIME",CURRENT_TIME);
						ADVWRTTRANSsec(vchnum,1,"TRANSACTION NUMBER",NULL,SCRNvirtualSubData,mainrsrc);
					}
				}
					FINDFLDGETDOUBLE(dbsort->fileno,"AMOUNT",&amount);
					if(venpmt->Use_Taxes)
					{
						FINDFLDGETDOUBLE(dbsort->fileno,"TAX AMOUNT",&tax_amount);
						FINDFLDGETDOUBLE(dbsort->fileno,"COST",&cost);
						FINDFLDGETDOUBLE(dbsort->fileno,"ADMIN FEE",&adminfee);
						FINDFLDGETDOUBLE(dbsort->fileno,"PRORATE AMOUNT",&prorateamt);
						FINDFLDGETDOUBLE(dbsort->fileno,"DISENCUMBRANCE COST",&disenc_cost);
						FINDFLDGETDOUBLE(dbsort->fileno,"DISENCUMBRANCE TAX AMOUNT",&disenc_tax);
						FINDFLDGETDOUBLE(dbsort->fileno,"DISENCUMBRANCE ADMIN FEE",&disenc_admin);
						FINDFLDGETDOUBLE(dbsort->fileno,"DISENCUMBRANCE PRORATE AMOUNT",&disenc_prorate);
						totalnotax=amount-tax_amount;
					}
					FINDFLDGETDOUBLE(dbsort->fileno,"DISENCUMBRANCE AMOUNT",&disenc_amount);
				FINDFLDGETSHORT(dbsort->fileno,"ACCOUNT TYPE",&accttype);
				FINDFLDGETSHORT(dbsort->fileno,"PO ACCOUNT TYPE",&poaccttype);
				FINDFLDGETSHORT(dbsort->fileno,"DEFINITION TYPE",&deftype);
				FINDFLDGETSHORT(dbsort->fileno,"PO DEFINITION TYPE",&podeftype);
				FINDFLDGETSHORT(dbsort->fileno,"CLEARING DEFINITION TYPE",&cdeftype);
				FINDFLDGETINT(dbsort->fileno,"FISCAL YEAR",&procyr);
				FINDFLDGETINT(dbsort->fileno,"PO FISCAL YEAR",&poprocyr);
/*
				FINDFLDGETSHORT(dbsort->fileno,"PROCESSING MONTH",&procmo);
*/
				if(XPERT_SETUP->CIS==TRUE)
				{
					FINDFLDGETCHAR(dbsort->fileno,"USE CIS",&usecis);
					if(usecis==TRUE)
					{
						ZERNRD(percis);
						COPYFIELD(dbsort->fileno,percis,"CIS TYPE");
						COPYFIELD(dbsort->fileno,percis,"PERSONNEL IDENTIFICATION");
						if(!ADVEQLNRDsec(percis,1,SCRNvirtualSubData,mainrsrc))
						{
							prev=RDATDataNEW(percis);
						} else prev=NULL;
						FINDFLDGETDOUBLE(percis,"INVOICED",&amt1);
						amt1+=amount;
						FINDFLDSETDOUBLE(percis,"INVOICED",amt1);
						FINDFLDGETDOUBLE(percis,"BALANCE",&amt1);
						amt1-=amount;
						FINDFLDSETDOUBLE(percis,"BALANCE",amt1);
						ADVWRTTRANSsec(percis,0,NULL,prev,SCRNvirtualSubData,mainrsrc);
						if(prev!=NULL) FreeRDATData(prev);
						ZERNRD(perctrn);
						COPYFIELD(dbsort->fileno,perctrn,"CIS TYPE");
						COPYFIELD(dbsort->fileno,perctrn,"PO FISCAL YEAR");
						COPYFIELD(dbsort->fileno,perctrn,"PERSONNEL IDENTIFICATION");
						FINDFLDSETDOUBLE(perctrn,"AMOUNT",amount);
						FINDFLDSETSHORT(perctrn,"TYPE",3);
						FINDFLDSETSHORT(perctrn,"SOURCE",3);
						FINDFLDSETSHORT(perctrn,"FISCAL YEAR",procyr);
						FIELDCOPY(dbsort->fileno,"PO LINE NUMBER",perctrn,"LINE NUMBER");
						FIELDCOPY(dbsort->fileno,"PO ACCOUNT NUMBER",perctrn,"ACCOUNT NUMBER");
						COPYFIELD(dbsort->fileno,perctrn,"PO IDENTIFICATION");
						COPYFIELD(vchnum,perctrn,"VENDOR IDENTIFICATION");
						COPYFIELD(vchnum,perctrn,"INVOICE IDENTIFICATION");
						COPYFIELD(vchnum,perctrn,"CALENDAR YEAR");
						FIELDCOPY(vchnum,"TRANSACTION NUMBER",perctrn,"SRC TRANSACTION NUMBER");
						FINDFLDSETSTRING(perctrn,"SOURCE USER",USERLOGIN);
						COPYFIELD(dbsort->fileno,perctrn,"ENTRY DATE");
						COPYFIELD(dbsort->fileno,perctrn,"DESCRIPTION");
						CURRENT_TIME=GETCURRENTTIME();
						FINDFLDSETSTRING(perctrn,"ENTRY TIME",CURRENT_TIME);
						ADVWRTTRANSsec(perctrn,1,"TRANSACTION NUMBER",NULL,SCRNvirtualSubData,mainrsrc);
						
					}
				}
				FINDFLDGETSHORT(dbsort->fileno,"FISCAL PROCESSING MONTH",&procmo);
/* need to update supporting files here */
/* update FINMGT Files *YR's */
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
						ADVupdateundist(dbsort->fileno,"ACCOUNT CODE",NULL,NULL,NULL,ryrnum,(-1),procyr,deftype,"REVENUE",FISCALPERIOD[procmo],(-1)*amount,SCRNvirtualSubData,mainrsrc);
						ADVupdateundist(dbsort->fileno,NULL,"DEBIT CODE","CREDIT CODE","PAYABLE CODE",(-1),byrnum,procyr,deftype,"REVENUE",FISCALPERIOD[procmo],amount,SCRNvirtualSubData,mainrsrc);
						} else {
						ADVupdateundist(dbsort->fileno,"ACCOUNT CODE",NULL,NULL,NULL,ryrnum,(-1),procyr,deftype,"REVENUE",CALENDAR_MONTHS[procmo],(-1)*amount,SCRNvirtualSubData,mainrsrc);
						ADVupdateundist(dbsort->fileno,NULL,"DEBIT CODE","CREDIT CODE","PAYABLE CODE",(-1),byrnum,procyr,deftype,"REVENUE",CALENDAR_MONTHS[procmo],amount,SCRNvirtualSubData,mainrsrc);
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
				if(venpmt->use_fiscal_months==TRUE)
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
				COPYFIELD(dbsort->fileno,trannum,"ENTRY DATE");
				COPYFIELD(dbsort->fileno,trannum,"ENTRY TIME");
				FIELDCOPY(dbsort->fileno,"PO LINE NUMBER",trannum,"LINE NUMBER");
				FIELDCOPY(dbsort->fileno,"PO ACCOUNT NUMBER",trannum,"ACCOUNT NUMBER");
				FINDFLDSETSHORT(trannum,"PROCESSING MONTH",procmo);
				COPYFIELD(dbsort->fileno,trannum,"SOURCE USER");
/*
				FINDFLDSETCHAR(trannum,"SOURCE USER VERIFIED",TRUE);
*/
				FINDFLDSETCHAR(trannum,"SOURCE USER VERIFIED",FALSE);
                                FINDFLDSETCHAR(trannum,"VOUCHER DISENCUMBRANCE",TRUE);
				FINDFLDGETCHAR(dbsort->fileno,"FINALED",&finaled);
				COPYFIELD(dbsort->fileno,accnum,"PO IDENTIFICATION");
				FIELDCOPY(dbsort->fileno,"PO FISCAL YEAR",accnum,"FISCAL YEAR");
				FIELDCOPY(dbsort->fileno,"PO LINE NUMBER",accnum,"LINE NUMBER");
				FIELDCOPY(dbsort->fileno,"PO ACCOUNT NUMBER",accnum,"ACCOUNT NUMBER");
				if(ADVEQLNRDsec(accnum,1,SCRNvirtualSubData,mainrsrc)) 
				{ 
					KEYNRD(accnum,1);
					return;
				}
				if((finaled))
				{
                                	FINDFLDGETDOUBLE(accnum,"OUTSTANDING ACCOUNT AMOUNT",&initamt);
					if(venpmt->Use_Taxes)
					{
                                		FINDFLDGETDOUBLE(accnum,"OUTSTANDING ACCOUNT ADMIN FEE",&initadmin);
                                		FINDFLDGETDOUBLE(accnum,"OUTSTANDING ACCOUNT COST",&initcost);
                                		FINDFLDGETDOUBLE(accnum,"OUTSTANDING ACCOUNT PRORATE AMOUNT",&initprorate);
                                		FINDFLDGETDOUBLE(accnum,"OUTSTANDING ACCOUNT TAX AMOUNT",&inittax);
					}
				} else {
					if(venpmt->Use_Taxes)
					{
/* To keep negative zero fields from being written to the database, this section was changed to test for zero before multiplying the variable by (-1) */
/* DDC 10/28/2002 */
						if(cost>disenc_cost)
						{
							initcost=disenc_cost;
						} else {
							initcost=cost;
						}
						if(tax_amount>disenc_tax)
						{
							inittax=disenc_tax;
						} else {
							inittax=tax_amount;
						}
						if(adminfee>disenc_admin)
						{
							initadmin=disenc_admin;
						} else {
							initadmin=adminfee;
						}
						if(prorateamt>disenc_prorate)
						{
							initprorate=disenc_prorate;
						} else {
							initprorate=prorateamt;
						}
						totalpartsamount=(initcost+inittax+initadmin+initprorate);
						if(totalpartsamount>disenc_amount)
						{
							initamt=disenc_amount;
						} else {
							initamt=totalpartsamount;
						}
					} else {
						if(amount>disenc_amount)
						{
							initamt=disenc_amount;
						} else {
							initamt=amount;
						}
					}
				}
				if(initamt!=0.0)
				{
					initamt=initamt*(-1);
				}
				if(initadmin!=0.0)
				{
					initadmin=initadmin*(-1);
				}
				if(initcost!=0.0)
				{
					initcost=initcost*(-1);
				}
				if(initprorate!=0.0)
				{
					initprorate=initprorate*(-1);
				}
				if(inittax!=0.0)
				{
					inittax=inittax*(-1);
				}
				FINDFLDSETDOUBLE(trannum,"AMOUNT",initamt);
				if(venpmt->Use_Taxes)
				{
					FINDFLDSETDOUBLE(trannum,"COST",initcost);
					FINDFLDSETDOUBLE(trannum,"TAX AMOUNT",inittax);
					FINDFLDSETDOUBLE(trannum,"ADMIN FEE",initadmin);
					FINDFLDSETDOUBLE(trannum,"PRORATE AMOUNT",initprorate);
				}
				CURRENT_TIME=GETCURRENTTIME();
				FINDFLDSETSTRING(trannum,"ENTRY TIME",CURRENT_TIME);
				if(initamt!=0)
/* DCT 2013-01-21 Added this condition so that zeroed PURTRN records aren't created when users zero the amount */
				{
					ADVWRTTRANSsec(trannum,1,"TRANSACTION NUMBER",NULL,SCRNvirtualSubData,mainrsrc);
					if(XPERT_SETUP->CIS==TRUE)
					{
						FINDFLDGETCHAR(dbsort->fileno,"USE CIS",&usecis);
						if(usecis==TRUE)
						{
							ZERNRD(perctrn);
							COPYFIELD(dbsort->fileno,perctrn,"PO FISCAL YEAR");
							COPYFIELD(dbsort->fileno,perctrn,"CIS TYPE");
							COPYFIELD(dbsort->fileno,perctrn,"PERSONNEL IDENTIFICATION");
							FINDFLDSETDOUBLE(perctrn,"AMOUNT",initamt);
							FINDFLDSETSHORT(perctrn,"TYPE",2);
							FINDFLDSETSHORT(perctrn,"SOURCE",2);
							COPYFIELD(dbsort->fileno,perctrn,"PO IDENTIFICATION");
							COPYFIELD(dbsort->fileno,perctrn,"LINE NUMBER");
							COPYFIELD(dbsort->fileno,perctrn,"ACCOUNT NUMBER");
							FINDFLDSETSHORT(perctrn,"FISCAL YEAR",procyr);
							FIELDCOPY(dbsort->fileno,"PO LINE NUMBER",perctrn,"LINE NUMBER");
							FIELDCOPY(dbsort->fileno,"PO ACCOUNT NUMBER",perctrn,"ACCOUNT NUMBER");
							FIELDCOPY(trannum,"TRANSACTION NUMBER",perctrn,"SRC TRANSACTION NUMBER");
							FINDFLDSETSTRING(perctrn,"SOURCE USER",USERLOGIN);
							COPYFIELD(dbsort->fileno,perctrn,"ENTRY DATE");
							COPYFIELD(dbsort->fileno,perctrn,"ENTRY TIME");
							COPYFIELD(dbsort->fileno,perctrn,"DESCRIPTION");
							ADVWRTTRANSsec(perctrn,1,"TRANSACTION NUMBER",NULL,SCRNvirtualSubData,mainrsrc);
							ZERNRD(percis);
							COPYFIELD(dbsort->fileno,percis,"CIS TYPE");
							COPYFIELD(dbsort->fileno,percis,"PERSONNEL IDENTIFICATION");
							if(!ADVEQLNRDsec(percis,1,SCRNvirtualSubData,mainrsrc))
							{
								prev=RDATDataNEW(percis);
							} else prev=NULL;
							FINDFLDGETDOUBLE(percis,"ENCUMBERED",&amt1);
							amt1+=initamt;
							FINDFLDSETDOUBLE(percis,"ENCUMBERED",amt1);
							FINDFLDGETDOUBLE(percis,"BALANCE",&amt1);
							amt1-=initamt;
							FINDFLDSETDOUBLE(percis,"BALANCE",amt1);
							ADVWRTTRANSsec(percis,0,NULL,prev,SCRNvirtualSubData,mainrsrc);
							if(prev!=NULL) FreeRDATData(prev);
							
						}
					}
				}
/* "Subtracting" disencumbrance amounts from the outstanding account amounts */
				prev=RDATDataNEW(accnum);
                               	FINDFLDGETDOUBLE(accnum,"OUTSTANDING ACCOUNT AMOUNT",&acc_amt);
				if(venpmt->Use_Taxes)
				{
                               		FINDFLDGETDOUBLE(accnum,"OUTSTANDING ACCOUNT ADMIN FEE",&acc_admin);
                               		FINDFLDGETDOUBLE(accnum,"OUTSTANDING ACCOUNT COST",&acc_cost);
                               		FINDFLDGETDOUBLE(accnum,"OUTSTANDING ACCOUNT PRORATE AMOUNT",&acc_prorate);
                               		FINDFLDGETDOUBLE(accnum,"OUTSTANDING ACCOUNT TAX AMOUNT",&acc_tax);
				}
/* Remember, the variables named init* have negative values */
				acc_amt+=initamt;
				if(venpmt->Use_Taxes)
				{
					acc_admin+=initadmin;
					acc_cost+=initcost;
					acc_prorate+=initprorate;
					acc_tax+=inittax;
				}
                               	FINDFLDSETDOUBLE(accnum,"OUTSTANDING ACCOUNT AMOUNT",acc_amt);
				if(venpmt->Use_Taxes)
				{
                               		FINDFLDSETDOUBLE(accnum,"OUTSTANDING ACCOUNT ADMIN FEE",acc_admin);
                               		FINDFLDSETDOUBLE(accnum,"OUTSTANDING ACCOUNT COST",acc_cost);
                               		FINDFLDSETDOUBLE(accnum,"OUTSTANDING ACCOUNT PRORATE AMOUNT",acc_prorate);
                               		FINDFLDSETDOUBLE(accnum,"OUTSTANDING ACCOUNT TAX AMOUNT",acc_tax);
				}
				if(ADVWRTTRANSsec(accnum,0,NULL,prev,SCRNvirtualSubData,mainrsrc))
				{
					ERRORDIALOG("WRITE ERROR OCCURRED!","The write function failed on the file PURACC.",NULL,FALSE);
					prterr("Failed to write to PURACC.");
				}
				if(prev!=NULL) FreeRDATData(prev);
				switch(poaccttype)
				{
					case EXP_ACCT:
						if(amount>initamt)
						{
							if(purord->use_fiscal_months==TRUE)
							{
								ADVupdateundist(dbsort->fileno,"PO ACCOUNT CODE","PO DEBIT CODE","PO CREDIT CODE",NULL,eyrnum,byrnum,poprocyr,podeftype,"ENCUMBRANCES",FISCALPERIOD[procmo],initamt,SCRNvirtualSubData,mainrsrc);
							} else {
								ADVupdateundist(dbsort->fileno,"PO ACCOUNT CODE","PO DEBIT CODE","PO CREDIT CODE",NULL,eyrnum,byrnum,poprocyr,podeftype,"ENCUMBRANCES",CALENDAR_MONTHS[procmo],initamt,SCRNvirtualSubData,mainrsrc);
							}
						} else {
							if(purord->use_fiscal_months==TRUE)
							{
								ADVupdateundist(dbsort->fileno,"PO ACCOUNT CODE","PO DEBIT CODE","PO CREDIT CODE",NULL,eyrnum,byrnum,poprocyr,podeftype,"ENCUMBRANCES",FISCALPERIOD[procmo],-amount,SCRNvirtualSubData,mainrsrc);
							} else {
								ADVupdateundist(dbsort->fileno,"PO ACCOUNT CODE","PO DEBIT CODE","PO CREDIT CODE",NULL,eyrnum,byrnum,poprocyr,podeftype,"ENCUMBRANCES",CALENDAR_MONTHS[procmo],-amount,SCRNvirtualSubData,mainrsrc);
							} 
						}
						break;
					case REV_ACCT:
						if(amount>initamt)
						{
						if(purord->use_fiscal_months==TRUE)
						{
							ADVupdateundist(dbsort->fileno,"PO ACCOUNT CODE","PO DEBIT CODE","PO CREDIT CODE",NULL,ryrnum,byrnum,poprocyr,podeftype,"REVENUE POS",FISCALPERIOD[procmo],initamt,SCRNvirtualSubData,mainrsrc);
						} else {
							ADVupdateundist(dbsort->fileno,"PO ACCOUNT CODE","PO DEBIT CODE","PO CREDIT CODE",NULL,ryrnum,byrnum,poprocyr,podeftype,"REVENUE POS",CALENDAR_MONTHS[procmo],initamt,SCRNvirtualSubData,mainrsrc);
						}
						} else {
						if(purord->use_fiscal_months==TRUE)
						{
							ADVupdateundist(dbsort->fileno,"PO ACCOUNT CODE","PO DEBIT CODE","PO CREDIT CODE",NULL,ryrnum,byrnum,poprocyr,podeftype,"REVENUE POS",FISCALPERIOD[procmo],-amount,SCRNvirtualSubData,mainrsrc);
						} else {
							ADVupdateundist(dbsort->fileno,"PO ACCOUNT CODE","PO DEBIT CODE","PO CREDIT CODE",NULL,ryrnum,byrnum,poprocyr,podeftype,"REVENUE POS",CALENDAR_MONTHS[procmo],-amount,SCRNvirtualSubData,mainrsrc);
						}
						}
						break;
					case BAL_ACCT:
						if(amount>initamt)
						{
						if(purord->use_fiscal_months==TRUE)
						{
							ADVupdateundist(dbsort->fileno,NULL,"PO ACCOUNT CODE","PO CREDIT CODE",NULL,(-1),byrnum,poprocyr,podeftype,"",FISCALPERIOD[procmo],initamt,SCRNvirtualSubData,mainrsrc);
						} else {
							ADVupdateundist(dbsort->fileno,NULL,"PO ACCOUNT CODE","PO CREDIT CODE",NULL,(-1),byrnum,poprocyr,podeftype,"",CALENDAR_MONTHS[procmo],initamt,SCRNvirtualSubData,mainrsrc);
						}
						} else {
						if(purord->use_fiscal_months==TRUE)
						{
							ADVupdateundist(dbsort->fileno,NULL,"PO ACCOUNT CODE","PO CREDIT CODE",NULL,(-1),byrnum,poprocyr,podeftype,"",FISCALPERIOD[procmo],-amount,SCRNvirtualSubData,mainrsrc);
						} else {
							ADVupdateundist(dbsort->fileno,NULL,"PO ACCOUNT CODE","PO CREDIT CODE",NULL,(-1),byrnum,poprocyr,podeftype,"",CALENDAR_MONTHS[procmo],-amount,SCRNvirtualSubData,mainrsrc);
						}
						}
						break;
					default:
						prterr("Invalid Account Type [%d]",accttype);
						break;
				}
				++PO_COUNT;
				FINDFLDGETINT(trannum,"FISCAL YEAR",&yearid);
				FINDFLDGETINT(trannum,"LINE NUMBER",&lineno);
				FINDFLDGETSTRING(trannum,"PO IDENTIFICATION",&poid);
				ADV4updateoutstanding(msrnum,linenum,yearid,poid,lineno,initcost,inittax,initadmin,initprorate,initamt,FALSE,1,SCRNvirtualSubData,mainrsrc,venpmt);
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
						field=FLDNRD(msrnum,"CLOSED DATE");
						if(field!=NULL)
						{
							if(field->len<10)
							{
								FINDFLDSETSTRING(msrnum,"CLOSED DATE",CURRENT_DATE);
							} else FINDFLDSETSTRING(msrnum,"CLOSED DATE",CURRENT_DATE10);
						} else FINDFLDSETSTRING(msrnum,"CLOSED DATE",CURRENT_DATE);
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
		if(updateb==TRUE)
		{
			updatebrowse(TRUE,mbl_addpo,msrnum,module,mbl_addpo->mainrsrc);
		}
	}	
}
static void quitback(RDArsrc *mainrsrc,MakeBrowseList *blist)
{
	;
}
static void saveexityu(RDArsrc *mainrsrc,MakeBrowseList *blist)
{
	saveexit(mainrsrc,blist,TRUE);
}
static void saveexittest(RDArsrc *mainrsrc,MakeBrowseList *blist)
{
	if(total_po!=invoice_total && invoice_total!=0)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","INVOICE BALANCE WARNING",
			"Exiting ADD PO VOUCHERS with the total of vouchers out of balance with the total invoice.  Please correct this before continuing.",
			saveexityu,quitback,FALSE,2,mainrsrc,blist,NULL);
	} else {
		saveexit(mainrsrc,blist,TRUE);
	}
}
static void saveexitnu(RDArsrc *mainrsrc,MakeBrowseList *blist)
{
	saveexit(mainrsrc,blist,FALSE);
}
static void saveexittestnu(RDArsrc *mainrsrc,MakeBrowseList *blist)
{
	if(total_po!=invoice_total && invoice_total!=0)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","INVOICE BALANCE WARNING",
			"Exiting ADD PO VOUCHERS with the total of vouchers out of balance with the total invoice.  Please correct this before continuing.",
			saveexitnu,quitback,FALSE,2,mainrsrc,blist,NULL);
	} else {
		saveexit(mainrsrc,blist,FALSE);
	}
}
static void PURAPV1_doexit_test(MakeBrowseList *blist)
{
	DBsort *dbsort=NULL;
	
	dbsort=blist->funcargs;
	if(dbsort!=NULL)
	{
		WARNINGDIALOG("ABORT WARNING DIALOG SCREEN","RECORDS NOT PROCESSED!","Are you sure that you want to quit without processing the Purchase Order Vouchers?",PURAPV1_doexit,NULL,FALSE,blist);
	} else {
		PURAPV1_doexit(blist);
	}
}
static void PURAPV1_doexit(MakeBrowseList *blist)
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
static void PURAPV1_okfunc(MakeBrowseList *blist,void *targetkey)
{
	purapvm(0,targetkey,0,NULL);
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	purapvm(2,NULL,0,NULL);
} 
short FIELDSubData(char **tmp,PassableStruct *p,char *modulename)
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
short VIRTUALSubData(char **tmp,PassableStruct *p,char *modulename)
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
					temp=PP_EVALUATEstr(v->expression,PurApvSubData,p);
					if(v->value.string_value!=NULL) 
						Rfree(v->value.string_value);
					v->value.string_value=temp;
					temp=NULL;
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
					*v->value.short_value=PP_EVALUATEsht(v->expression,PurApvSubData,p);
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
					*v->value.integer_value=PP_EVALUATEint(v->expression,PurApvSubData,p);
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
					*v->value.float_value=PP_EVALUATEdbl(v->expression,PurApvSubData,p);
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
					*v->value.string_value=PP_EVALUATEbol(v->expression,PurApvSubData,p);
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
					*v->value.string_value=PP_EVALUATEbol(v->expression,PurApvSubData,p);
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
			prterr("Error Virtual Field [%s] not found.",*tmp);
		}
		return(1);
	}
	return(0);
}
#ifdef XXXX_GLOBALS
short GLOBALSubData(char **tmp,PassableStruct *p,char *modulename)
{
	char *temp=NULL,*gn=NULL;
	short start=1,length=0,str_length=0;
	RDApurord *purord=NULL;
	RDAfinmgt *fmgt=NULL;

	if(diageval || diagapps)
	{
		prterr("DIAG GLOBALSubData Substituting for [%s].",*tmp);
	}
	if(!RDAstrcmp(modulename,"LOGIN IDENTIFICATION"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				start=Ratoi(gn);
				gn=strtok(NULL,",]");
				length=Ratoi(gn);
			}
			if(start<1) start=1;
		}
		temp=stralloc(USERLOGIN);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
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
	} else if(!RDAstrcmp(modulename,"PURORD SETUP USE TAXES"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%d",purord->Use_Taxes);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP USE TAX1"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%d",purord->Use_Tax1);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP SEPERATE TAX1 VENDOR"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%d",purord->Sep_Tax1_Vendor);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP TAX1 RATE"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%f",purord->Tax1_Rate);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP DEFAULT TAX1 VENDOR"))
	{
		purord=RDApurordNEW();
		getpurordbin(purord);
		if(RDAstrlen(*tmp)<(RDAstrlen(purord->Default_Tax1_Vendor)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(purord->Default_Tax1_Vendor)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+3);
		sprintf(*tmp,"\"%s\"",(purord->Default_Tax1_Vendor!=NULL ? purord->Default_Tax1_Vendor:""));
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP DEFAULT TAX1 ADDRESS"))
	{
		purord=RDApurordNEW();
		getpurordbin(purord);
		if(RDAstrlen(*tmp)<(RDAstrlen(purord->Default_Tax1_Address)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(purord->Default_Tax1_Address)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+3);
		sprintf(*tmp,"\"%s\"",(purord->Default_Tax1_Address!=NULL ? purord->Default_Tax1_Address:""));
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP USE TAX2"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%d",purord->Use_Tax2);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP SEPERATE TAX2 VENDOR"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%d",purord->Sep_Tax2_Vendor);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP TAX2 RATE"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%f",purord->Tax2_Rate);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP DEFAULT TAX2 VENDOR"))
	{
		purord=RDApurordNEW();
		getpurordbin(purord);
		if(RDAstrlen(*tmp)<(RDAstrlen(purord->Default_Tax2_Vendor)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(purord->Default_Tax2_Vendor)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+3);
		sprintf(*tmp,"\"%s\"",(purord->Default_Tax2_Vendor!=NULL ? purord->Default_Tax2_Vendor:""));
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP DEFAULT TAX2 ADDRESS"))
	{
		purord=RDApurordNEW();
		getpurordbin(purord);
		if(RDAstrlen(*tmp)<(RDAstrlen(purord->Default_Tax2_Address)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(purord->Default_Tax2_Address)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+3);
		sprintf(*tmp,"\"%s\"",(purord->Default_Tax2_Address!=NULL ? purord->Default_Tax2_Address:""));
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP USE TAX3"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%d",purord->Use_Tax3);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP SEPERATE TAX3 VENDOR"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%d",purord->Sep_Tax3_Vendor);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP TAX3 RATE"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%f",purord->Tax3_Rate);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP DEFAULT TAX3 VENDOR"))
	{
		purord=RDApurordNEW();
		getpurordbin(purord);
		if(RDAstrlen(*tmp)<(RDAstrlen(purord->Default_Tax3_Vendor)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(purord->Default_Tax3_Vendor)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+3);
		sprintf(*tmp,"\"%s\"",(purord->Default_Tax3_Vendor!=NULL ? purord->Default_Tax3_Vendor:""));
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP DEFAULT TAX3 ADDRESS"))
	{
		purord=RDApurordNEW();
		getpurordbin(purord);
		if(RDAstrlen(*tmp)<(RDAstrlen(purord->Default_Tax3_Address)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(purord->Default_Tax3_Address)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+3);
		sprintf(*tmp,"\"%s\"",(purord->Default_Tax3_Address!=NULL ? purord->Default_Tax3_Address:""));
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP USE TAX4"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%d",purord->Use_Tax4);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP SEPERATE TAX4 VENDOR"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%d",purord->Sep_Tax4_Vendor);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP TAX4 RATE"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%f",purord->Tax4_Rate);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP DEFAULT TAX4 VENDOR"))
	{
		purord=RDApurordNEW();
		getpurordbin(purord);
		if(RDAstrlen(*tmp)<(RDAstrlen(purord->Default_Tax4_Vendor)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(purord->Default_Tax4_Vendor)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+3);
		sprintf(*tmp,"\"%s\"",(purord->Default_Tax4_Vendor!=NULL ? purord->Default_Tax4_Vendor:""));
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP DEFAULT TAX4 ADDRESS"))
	{
		purord=RDApurordNEW();
		getpurordbin(purord);
		if(RDAstrlen(*tmp)<(RDAstrlen(purord->Default_Tax4_Address)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(purord->Default_Tax4_Address)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+3);
		sprintf(*tmp,"\"%s\"",(purord->Default_Tax4_Address!=NULL ? purord->Default_Tax4_Address:""));
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP USE TAX5"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%d",purord->Use_Tax5);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP SEPERATE TAX5 VENDOR"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%d",purord->Sep_Tax5_Vendor);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP TAX5 RATE"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%f",purord->Tax5_Rate);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP DEFAULT TAX5 VENDOR"))
	{
		purord=RDApurordNEW();
		getpurordbin(purord);
		if(RDAstrlen(*tmp)<(RDAstrlen(purord->Default_Tax5_Vendor)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(purord->Default_Tax5_Vendor)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+3);
		sprintf(*tmp,"\"%s\"",(purord->Default_Tax5_Vendor!=NULL ? purord->Default_Tax5_Vendor:""));
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP DEFAULT TAX5 ADDRESS"))
	{
		purord=RDApurordNEW();
		getpurordbin(purord);
		if(RDAstrlen(*tmp)<(RDAstrlen(purord->Default_Tax5_Address)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(purord->Default_Tax5_Address)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+3);
		sprintf(*tmp,"\"%s\"",(purord->Default_Tax5_Address!=NULL ? purord->Default_Tax5_Address:""));
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"STARTING FISCAL MONTH"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",fmgt->month);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
	} else if(!RDAstrcmp(modulename,"SYSTEM DATE"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				start=Ratoi(gn);
				gn=strtok(NULL,",]");
				length=Ratoi(gn);
			}
			if(start<1) start=1;
		}
		temp=GETCURRENTDATE();
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
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
	} else if(!RDAstrcmp(modulename,"SYSTEM TIME"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				start=Ratoi(gn);
				gn=strtok(NULL,",]");
				length=Ratoi(gn);
			}
			if(start<1) start=1;
		}
		temp=GETCURRENTTIME();
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
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
	} else if(!RDAstrcmp(modulename,"ORGANIZATION NAME"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				start=Ratoi(gn);
				gn=strtok(NULL,",]");
				length=Ratoi(gn);
			}
			if(start<1) start=1;
		}
		temp=stralloc(XPERT_SETUP->company_name);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
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
	} else if(!RDAstrcmp(modulename,"ORGANIZATION ADDRESS #1"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				start=Ratoi(gn);
				gn=strtok(NULL,",]");
				length=Ratoi(gn);
			}
			if(start<1) start=1;
		}
		temp=stralloc(XPERT_SETUP->addr1);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
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
	} else if(!RDAstrcmp(modulename,"ORGANIZATION ADDRESS #2"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				start=Ratoi(gn);
				gn=strtok(NULL,",]");
				length=Ratoi(gn);
			}
			if(start<1) start=1;
		}
		temp=stralloc(XPERT_SETUP->addr2);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
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
	} else if(!RDAstrcmp(modulename,"ORGANIZATION CITY"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				start=Ratoi(gn);
				gn=strtok(NULL,",]");
				length=Ratoi(gn);
			}
			if(start<1) start=1;
		}
		temp=stralloc(XPERT_SETUP->city);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
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
	} else if(!RDAstrcmp(modulename,"ORGANIZATION STATE"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				start=Ratoi(gn);
				gn=strtok(NULL,",]");
				length=Ratoi(gn);
			}
			if(start<1) start=1;
		}
		temp=stralloc(XPERT_SETUP->state);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
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
	} else if(!RDAstrcmp(modulename,"ORGANIZATION ZIP"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				start=Ratoi(gn);
				gn=strtok(NULL,",]");
				length=Ratoi(gn);
			}
			if(start<1) start=1;
		}
		temp=stralloc(XPERT_SETUP->zip);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
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
	} else if(!RDAstrcmp(modulename,"ORGANIZATION PHONE"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				start=Ratoi(gn);
				gn=strtok(NULL,",]");
				length=Ratoi(gn);
			}
			if(start<1) start=1;
		}
		temp=stralloc(XPERT_SETUP->phone);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
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
	} else {
		return(1);
	}
	return(0);
}
#endif /* XXXX_GLOBALS */
short SORTFILESubData(char **tmp,PassableStruct *p,char *modulename)
{
	DBsort *dbsort=NULL;
	NRDfield *field=NULL;
	char *temp=NULL,*gn=NULL;
	short start=1,length=0,str_length=0;

	if(diageval || diagapps)
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
			prterr("Error Fatal Error encountered substitution aborted.....");
		}
		if(diageval || diagapps)
		{
			prterr("DIAG PurApvSubData Returning [%s].",*tmp);
		}
	}
	return(retval);
}
void PurApvSubData(char **tmp,PassableStruct *p)
{
	char *modulename=NULL;

	if(diageval || diagapps)
	{
		prterr("DIAG PurApvSubData Substituting for [%s].",*tmp);
	}
	if(diageval || diagapps)
	{
		prterr("DIAG PurApvSubData Returning [%s].",*tmp);
	}
	modulename=stripmodulename(*tmp);
	if(!isEMPTY(modulename))
	{
		if(ExecuteSubData(FIELDSubData,tmp,p,modulename)<1)
		{
			if(modulename!=NULL) Rfree(modulename);
			return;
		}
		if(ExecuteSubData(VIRTUALSubData,tmp,p,modulename)<1)
		{
			if(modulename!=NULL) Rfree(modulename);
			return;
		}
		if(ExecuteSubData(SORTFILESubData,tmp,p,modulename)<1)
		{
			if(modulename!=NULL) Rfree(modulename);
			return;
		}
		if(GLOBALSubData(tmp,modulename)==0)
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
	prterr("Error PurApvSubData [%s] not found.",*tmp);
	if(*tmp!=NULL) Rfree(*tmp);
	*tmp=stralloc("\"\"");
	if(diageval || diagapps)
	{
		prterr("DIAG PurApvSubData Returning [%s].",*tmp);
	}
	if(modulename!=NULL) Rfree(modulename);
}
