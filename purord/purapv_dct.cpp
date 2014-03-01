/* purapv.c - Add Purchase Order Vouchers */
#ifndef WIN32
#define __NAM__ "purapv_dct.lnx"
#endif
#ifdef WIN32
#define __NAM__ "purapv_dct.exe"
#endif
#include <app.hpp>
#include <cstdio>

#include <mix.hpp>


#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>

#include <purord.hpp>
#include <venpmt.hpp>
#include <fin.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static char SKIP_OVER_BUDGET_WARNINGS=FALSE;
static short msrnum=(-1),sitenum=(-1),vennum=(-1),venadrnum=(-1),infnum=(-1);
static short linenum=(-1),accnum=(-1),trannum=(-1),invnum=(-1),vchnum=(-1);
static short finyrnum=(-1),eyrnum=(-1),ryrnum=(-1),byrnum=(-1);
static short yearnum=(-1),fyearnum=(-1),percis=(-1),perctrn=(-1);
static short bnkacc=(-1),expnum=(-1),revnum=(-1),balnum=(-1),alias_num=(-1);
static RDAfinmgt *finsetup=NULL;
static RDApurord *purord=NULL;
static RDAvenpmt *venpmt=NULL;
static char *reportname=NULL;
static int PO_COUNT=0;
static short POV_DESCRIPTION=0;
static char *poid=NULL,*vendorid=NULL,*invid=NULL,*checkdate=NULL,manual_voucher=FALSE,prepaid=FALSE,pay_1099=FALSE,check_detail=FALSE, addlist=TRUE;
static int yearid=0,checkno=0,venyrid=0;
static short pmonth=0,fmonth=0,venpmtfmonth=0;
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static short selectfunc(MakeBrowseList *);
static char *module="PURORD";
static MakeBrowseList *mbl_addpo=NULL;

static void PURAVCI_doexit(MakeBrowseList *),PURAVCI_okfunc(MakeBrowseList *,void *);
static void initfunc(MakeBrowseList *);
static short PURAVCI_selectfunction(MakeBrowseList *);
static MakeBrowseList *mbl_purinv=NULL;
static void browse_inv(RDArsrc *,RDATData *);
struct HoldIts
{
	RDArsrc *mtnrsrc;
	RDATData *previous;
};
typedef struct HoldIts HoldIt;

struct PassableStructs
{
	RDArsrc *r;
	void *a;
};
typedef struct PassableStructs PassableStruct;

static void PurApvSubData(char **,PassableStruct *);

static void PURAPV1_doexit(MakeBrowseList *),PURAPV1_okfunc(MakeBrowseList *,void *);
static void PURAPV1_doexit_test(MakeBrowseList *);
static void saveexit(RDArsrc *,MakeBrowseList *,short);
static void saveexittest(RDArsrc *,MakeBrowseList *);
static void saveexittestnu(RDArsrc *,MakeBrowseList *);
static void new_record(RDArsrc *,MakeBrowseList *);
static short PURAPV1_selectfunction(MakeBrowseList *);
static MakeBrowseList *mbl_rec=NULL;
static void browse_purapv(RDArsrc *);
static DBsort *dbsort=NULL;
static double total_po=0;

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

static void getrecordsetedit(RDArsrc *,mtnstruct *);
static void seteditable(RDArsrc *,mtnstruct *);
static void getsupportingrecords(RDArsrc *,DBsort *,short);
static void PURAPV2_save_record(RDArsrc *,mtnstruct *);
static void PURAPV2_save_record_warn(RDArsrc *,mtnstruct *);
static void PURAPV2_quit_record(RDArsrc *,mtnstruct *);
/*
static void quit_record_warn(RDArsrc *,mtnstruct *);
*/
static void PURAPV2_save_record_nup(RDArsrc *,mtnstruct *);
static void PURAPV2_save_record_up(RDArsrc *,mtnstruct *);
static void PURAPV2_getfilecb(RDArsrc *,mtnstruct *mtn);
static void getpmonths(RDArsrc *,mtnstruct *);
static void changeACCTtype(RDArsrc *,mtnstruct *);
static void changeDEFtype(RDArsrc *,mtnstruct *);
static void changeclearDEFtype(RDArsrc *,mtnstruct *);
static void ACCTtypecb(RDArsrc *,mtnstruct *);
static void DEFtypecb(RDArsrc *,mtnstruct *);
static void clearDEFtypecb(RDArsrc *,mtnstruct *);
static void setACCTtype(RDArsrc *,mtnstruct *);
static void setpmonth(RDArsrc *,mtnstruct *);
static void PURAPV2_save_check(RDArsrc *,mtnstruct *);
static short check_code(RDArsrc *,mtnstruct *);
static short check_acctcode(RDArsrc *,mtnstruct *);
static short check_debcode(RDArsrc *,mtnstruct *);
static short check_credcode(RDArsrc *,mtnstruct *);
static short check_paycode(RDArsrc *,mtnstruct *);
static short check_manual(RDArsrc *,mtnstruct *);
/*
static short check_dueto(RDArsrc *,mtnstruct *);
*/
static short check_clearfrom(RDArsrc *,mtnstruct *);
static short check_clearcash(RDArsrc *,mtnstruct *);
static void check_bankidcb(RDArsrc *,mtnstruct *);
static short check_bankid(RDArsrc *,mtnstruct *);
static void acctcodecb(RDArsrc *,mtnstruct *),debcodecb(RDArsrc *,mtnstruct *);
static void credcodecb(RDArsrc *,mtnstruct *);
static void paycodecb(RDArsrc *,mtnstruct *);
static void manualvouchercb(RDArsrc *,mtnstruct *);
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
static char distributable=TRUE;
static void purapvm(short,void *,int,char **);
static int transno=0;
static PassableStruct *PSTRUCT=NULL;

static char skip_invoice_date=0,prorate_excess=FALSE,set_f=FALSE;
static char set_copy_desc=FALSE;
static char prorate_extra=FALSE,*duedate=NULL,*invdate=NULL;
static double amt2prorate=0.0;
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
static char editinv=TRUE,ClosePO=FALSE;
static void purinvm(short,void *,int,char **);
static void setaddrlist(RDArsrc *);
static void changeADDRlist(RDArsrc *);
static double invoice_total=0;

static short selectfunc(MakeBrowseList *b)
{
	char flag=FALSE;

	FINDFLDGETCHAR(msrnum,"CLOSED",&flag);
	if(flag) return(FALSE);
	FINDFLDGETCHAR(msrnum,"DELETEFLAG",&flag);
	if(flag) return(FALSE);
	return(TRUE);
}
static void doexit(MakeBrowseList *b)
{
	APPlib *args=NULL;

	if(PO_COUNT>0)
	{
		args=APPlibNEW();
		addAPPlib(args,module);
		addAPPlib(args,reportname);
		ExecuteProgram("doreport",args);
		if(args!=NULL) freeapplib(args);
	}
	if(b->definelist!=NULL) 
	{
		killwindow(b->definelist);
		free_rsrc(b->definelist);
	}
	if(b->searchrsrc!=NULL) 
	{
		killwindow(b->searchrsrc);
		free_rsrc(b->searchrsrc);
	}
	if(b->mainrsrc!=NULL)
	{
		killwindow(b->mainrsrc);
		free_rsrc(b->mainrsrc);
	}
	if(reportname!=NULL) Rfree(reportname);
	if(yearnum!=(-1)) CLSNRD(yearnum);
	if(venpmt->use_fiscal_summary==TRUE)
	{
		if(fyearnum!=(-1)) CLSNRD(fyearnum);
	}
	if(accnum!=(-1)) CLSNRD(accnum);
	if(alias_num!=(-1)) CLSNRD(alias_num);
	if(bnkacc!=(-1)) CLSNRD(bnkacc);
	if(sitenum!=(-1)) CLSNRD(sitenum);
	if(trannum!=(-1)) CLSNRD(trannum);
	if(vennum!=(-1)) CLSNRD(vennum);
	if(linenum!=(-1)) CLSNRD(linenum);
	if(venadrnum!=(-1)) CLSNRD(venadrnum);
	if(byrnum!=(-1)) CLSNRD(byrnum);
	if(expnum!=(-1)) CLSNRD(expnum);
	if(invnum!=(-1)) CLSNRD(invnum);
	if(infnum!=(-1)) CLSNRD(infnum);
	if(ryrnum!=(-1)) CLSNRD(ryrnum);
	if(revnum!=(-1)) CLSNRD(revnum);
	if(balnum!=(-1)) CLSNRD(balnum);
	if(eyrnum!=(-1)) CLSNRD(eyrnum);
	if(finyrnum!=(-1)) CLSNRD(finyrnum);
	if(percis!=(-1)) CLSNRD(percis);
	if(perctrn!=(-1)) CLSNRD(perctrn);
	if(vchnum!=(-1)) CLSNRD(vchnum);
	if(venpmt!=NULL) free_venpmt(venpmt);
	if(finsetup!=NULL) free_finmgt(finsetup);
	if(purord!=NULL) free_purord(purord);
	ShutdownSubsystems();
	exit(0);
}
static void okfunc(MakeBrowseList *b,void *targetkey)
{
	purinvm(1,targetkey,0,NULL);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;
	int currentcalyear=0;
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,module,"ADD PURCHASE ORDER VOUCHERS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	SKIP_DOMENU_TRANSVERSAL=FALSE;
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("PURORD")+11);
#ifndef WIN32
        sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"PURORD");
#endif
#ifdef WIN32
        sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"PURORD");
#endif
        gsv=RDAGenericSetupNew("PURORD","PURAPV ADDLISTS");
        if(gsv!=NULL)
        {
                if(!getRDAGenericSetupbin(libx,gsv))
                {
                        addlist=*gsv->value.string_value;
                } else {
                        addlist=FALSE;
                }
        }
        if(gsv!=NULL) FreeRDAGenericSetup(gsv);
        gsv=RDAGenericSetupNew("PURORD","SKIP INVOICE DATE");
        if(gsv!=NULL)
        {
                if(!getRDAGenericSetupbin(libx,gsv))
                {
                        skip_invoice_date=*gsv->value.string_value;
                } else {
                        skip_invoice_date=FALSE;
                }
        }
        if(gsv!=NULL) FreeRDAGenericSetup(gsv);
        gsv=RDAGenericSetupNew("PURORD","PO VOUCHER DESCRIPTION");
        if(gsv!=NULL)
        {
                if(!getRDAGenericSetupbin(libx,gsv))
                {
                        POV_DESCRIPTION=*gsv->value.short_value;
                } else {
                        POV_DESCRIPTION=FALSE;
                }
        }
        if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("VENPMT")+11);
#ifndef WIN32
        sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"VENPMT");
#endif
#ifdef WIN32
        sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"VENPMT");
#endif
        gsv=RDAGenericSetupNew("VENPMT","CURRENT VOUCHER CALENDAR YEAR");
        if(gsv!=NULL)
        {
                if(!getRDAGenericSetupbin(libx,gsv))
                {
                        currentcalyear=*gsv->value.integer_value;
                } else {
                        currentcalyear=GETCURRENTCALENDARYEAR();
                }
        }
        if(gsv!=NULL) FreeRDAGenericSetup(gsv);
        gsv=RDAGenericSetupNew("VENPMT","CURRENT VOUCHER CALENDAR MONTH");
        if(gsv!=NULL)
        {
                if(!getRDAGenericSetupbin(libx,gsv))
                {
                        pmonth=*gsv->value.short_value;
                } else {
			CURRENT_DATE10=GETCURRENTDATE10();
			memset(stemp,0,101);
			memcpy(stemp,CURRENT_DATE10,10);
			stemp[2]=0;
			pmonth=(atoi(stemp)-1);
                }
        }
        if(gsv!=NULL) FreeRDAGenericSetup(gsv);
        gsv=RDAGenericSetupNew("VENPMT","CURRENT VOUCHER FISCAL MONTH");
        if(gsv!=NULL)
        {
                if(!getRDAGenericSetupbin(libx,gsv))
                {
                        venpmtfmonth=*gsv->value.short_value;
			set_f=TRUE;
                }
        }
        if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	fmonth=venpmtfmonth;
        gsv=RDAGenericSetupNew("VENPMT","CURRENT VOUCHER DUE DATE");
        if(gsv!=NULL)
        {
                if(!getRDAGenericSetupbin(libx,gsv))
                {
			duedate=stralloc(gsv->value.string_value);
                }
        }
        if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("FINMGT")+11);
#ifndef WIN32
        sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"FINMGT");
#endif
#ifdef WIN32
        sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"FINMGT");
#endif
        gsv=RDAGenericSetupNew("FINMGT","SKIP OVER BUDGET WARNINGS");
        if(gsv!=NULL)
        {
                if(!getRDAGenericSetupbin(libx,gsv))
                {
                        SKIP_OVER_BUDGET_WARNINGS=*gsv->value.string_value;
                } else {
                        SKIP_OVER_BUDGET_WARNINGS=FALSE;
                }
        }
        if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	GetAddList();
	if(argc>1)
	{
		reportname=stralloc(argv[1]);
	} else reportname=stralloc("PVCAUDIT");
	venpmt=RDAvenpmtNEW();
	if(getvenpmtbin(venpmt)==(-1)) return;
	if((msrnum=APPOPNNRD(module,"PURMST",TRUE,TRUE))==(-1)) return;
	if((sitenum=APPOPNNRD(module,"PURSIT",TRUE,FALSE))==(-1)) return;
	if((linenum=APPOPNNRD(module,"PURLIN",TRUE,FALSE))==(-1)) return;
	if((accnum=APPOPNNRD(module,"PURACC",TRUE,TRUE))==(-1)) return;
	if((bnkacc=APPOPNNRD("BNKREC","BNKACC",TRUE,FALSE))==(-1)) return;
	if((trannum=APPOPNNRD(module,"PURTRN",TRUE,TRUE))==(-1)) return;
	if((vennum=APPOPNNRD("VENPMT","VENMSR",TRUE,FALSE))==(-1)) return;
	if((yearnum=APPOPNNRD("VENPMT","VENYR",TRUE,FALSE))==(-1)) return;
	if(XPERT_SETUP->CIS==TRUE)
	{
		if((percis=APPOPNNRD("PRSNNL","PERCIS",TRUE,TRUE))==(-1)) return;
		if((perctrn=APPOPNNRD("PRSNNL","PERCTRN",TRUE,TRUE))==(-1)) return;
	} else {
		percis=(-1);
		perctrn=(-1);
	}
	if(venpmt->use_fiscal_summary==TRUE)
	{
		if((fyearnum=APPOPNNRD("VENPMT","VENFYR",TRUE,FALSE))==(-1)) return;
	}
	if((venadrnum=APPOPNNRD("VENPMT","VENADR",TRUE,FALSE))==(-1)) return;
	if((invnum=APPOPNNRD("VENPMT","VENINV",TRUE,TRUE))==(-1)) return;
	if((infnum=APPOPNNRD("VENPMT","VENINF",TRUE,TRUE))==(-1)) return;
	if((vchnum=APPOPNNRD("VENPMT","VENVCH",TRUE,TRUE))==(-1)) return;
	if((expnum=APPOPNNRD("FINMGT","FINEACC",TRUE,FALSE))==(-1)) return;
	if((alias_num=APPOPNNRD("FINMGT","FINALIAS",TRUE,FALSE))==(-1)) return;
	if((revnum=APPOPNNRD("FINMGT","FINRACC",TRUE,FALSE))==(-1)) return;
	if((balnum=APPOPNNRD("FINMGT","FINBACC",TRUE,FALSE))==(-1)) return;
	if((finyrnum=APPOPNNRD("FINMGT","FINYEAR",TRUE,FALSE))==(-1)) return;
	if((eyrnum=APPOPNNRDEDITABLE("FINMGT","FINEYR",&distributable))==(-1)) 
		return;
	if((ryrnum=APPOPNNRDEDITABLE("FINMGT","FINRYR",&distributable))==(-1)) 
		return;
	if((byrnum=APPOPNNRDEDITABLE("FINMGT","FINBYR",&distributable))==(-1)) 
		return;
	finsetup=RDAfinmgtNEW();
	if(GetFinmgtSetup(finsetup,NULL,NULL)==(-1)) return;
	purord=RDApurordNEW();
	getpurordbin(purord);
	mainrsrc=RDArsrcNEW(module,"ADD PURCHASE ORDER VOUCHERS BROWSE");
	searchrsrc=RDArsrcNEW(module,"ADD PURCHASE ORDER VOUCHERS SEARCH BROWSE");
	definelist=RDArsrcNEW(module,"ADD PURCHASE ORDER VOUCHERS DEFINE LIST");
	addDFincvir(searchrsrc,module,"PURMST",NULL,msrnum); 
	addDFincvir(definelist,module,"PURMST",NULL,msrnum); 
	addDFincvir(definelist,"VENPMT","VENMSR",NULL,vennum); 
	addDFincvir(searchrsrc,"VENPMT","VENMSR",NULL,vennum); 
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	CURRENT_DATE10=GETCURRENTDATE10();
	addstdrsrc(mainrsrc,"CALENDAR YEAR",LONGV,4,&currentcalyear,TRUE);
	addstdrsrc(mainrsrc,"PROCESSING MONTH",SHORTV,2,&pmonth,TRUE);
	DefaultScreens(mainrsrc);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	mbl_addpo=ADVSbrowse(msrnum,1,mainrsrc,searchrsrc,definelist,NULL,
		NULL,selectfunc,doexit,okfunc,NULL,NULL,TRUE,NULL,0,0,1,0);
	if(mbl_addpo!=NULL)
	{
		PO_COUNT=0;
	}
	RDAAPPMAINLOOP();
}
/* veninv.c - Xpert Vendor Invoices Master Program*/

void browse_inv(RDArsrc *mtnrsrc,RDATData *previous)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;
	HoldIt *h=NULL;

	h=Rmalloc(sizeof(HoldIt));
	h->mtnrsrc=mtnrsrc;
	h->previous=previous;
	mainrsrc=RDArsrcNEW(module,"ADD PURCHASE ORDER INVOICE BROWSE");
	searchrsrc=RDArsrcNEW(module,"ADD PURCHASE ORDER INVOICE SEARCH BROWSE");
	definelist=RDArsrcNEW(module,"ADD PURCHASE ORDER INVOICE DEFINE LIST");
	addDFincvir(mainrsrc,"PURORD","PURMST",NULL,msrnum);
	addDFincvir(mainrsrc,"PURORD","PURSIT",NULL,sitenum);
	addDFincvir(mainrsrc,"VENPMT","VENYR",NULL,yearnum);
	addDFincvir(mainrsrc,"VENPMT","VENMSR",NULL,vennum);
	addDFincvir(definelist,"VENPMT","VENINV",NULL,invnum);
	addDFincvir(definelist,"VENPMT","VENINF",NULL,infnum);
	addDFincvir(searchrsrc,"VENPMT","VENINV",NULL,invnum);
	addDFincvir(searchrsrc,"VENPMT","VENINF",NULL,infnum);
	readwidget(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]");
	if(vendorid!=NULL) { Rfree(vendorid); vendorid=NULL; }
	FINDRSCGETSTRING(mtnrsrc,"[VENINV][VENDOR IDENTIFICATION]",&vendorid);
	readwidget(mtnrsrc,"[VENINV][CALENDAR YEAR]");
	FINDRSCGETINT(mtnrsrc,"[VENINV][CALENDAR YEAR]",&venyrid);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
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
			if(ADVEQLNRDsec(vennum,1,SCRNvirtualSubData,mainrsrc)) 
				KEYNRD(vennum,1);
			filerecord2rsrc(vennum,mainrsrc);
		}
	} else {
		if(vendorid!=NULL) Rfree(vendorid); 
		venyrid=0;
		if(invid!=NULL) Rfree(invid); 
	}
	if(msrnum!=(-1))
	{
		file2rsrc(msrnum,mainrsrc,FALSE);
		FINDFLDSETINT(msrnum,"FISCAL YEAR",yearid);
		FINDFLDSETSTRING(msrnum,"PO IDENTIFICATION",poid);
		if(ADVEQLNRDsec(msrnum,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(msrnum,1);
		filerecord2rsrc(msrnum,mainrsrc);
	}
	SCRNvirtual2rsrc(mainrsrc);
	computeallSCRNvirtuals(mainrsrc);
	mbl_purinv=ADVbrowse(invnum,1,mainrsrc,searchrsrc,definelist
		,initfunc,PURAVCI_selectfunction,PURAVCI_doexit,PURAVCI_okfunc,NULL,h,TRUE,
		NULL,1,1,1,FALSE);
}
static void initfunc(MakeBrowseList *blist)
{
	if(vendorid!=NULL)
	{
		FINDFLDSETSTRING(invnum,"VENDOR IDENTIFICATION",vendorid);
		FINDFLDSETINT(invnum,"CALENDAR YEAR",venyrid);
	}
}
static short PURAVCI_selectfunction(MakeBrowseList *blist)
{
	char *venid1=NULL;

	if(vendorid!=NULL)
	{
		if(FINDFLDGETSTRING(invnum,"VENDOR IDENTIFICATION",&venid1)) 
			return FALSE;
		if(FINDFLDGETINT(invnum,"CALENDAR YEAR",&yearid))
			return FALSE;
		unpad(venid1);
		unpad(vendorid);
		if(!RDAstrcmp(venid1,vendorid) && yearid==venyrid)
		{
			if(venid1!=NULL) Rfree(venid1);
			return(TRUE);
		} else {
			if(venid1!=NULL) Rfree(venid1);
			return(FALSE);
		}	
	} else return(TRUE);
}
static void PURAVCI_doexit(MakeBrowseList *blist)
{

	mbl_purinv=NULL;
	if(blist->funcargs!=NULL) Rfree(blist->funcargs);
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
static void PURAVCI_okfunc(MakeBrowseList *b,void *targetkey)
{
	RDArsrc *mtnrsrc=NULL;
	RDATData *previous=NULL;
	HoldIt *h;
	char *addrid1=NULL;
	NRDfield *field=NULL;

	if(b->funcargs!=NULL)
	{
		h=(HoldIt *)b->funcargs;
		mtnrsrc=h->mtnrsrc;
		previous=h->previous;
		Rfree(b->funcargs);
	}
	mbl_purinv=NULL;
	FINDFLDGETSTRING(invnum,"ADDRESS IDENTIFICATION",&addrid1);
	KEYNRD(invnum,1);
	readallwidgets(mtnrsrc);
	if(ADVEQLNRDKEYsec(invnum,1,targetkey,SCRNvirtualSubData,mtnrsrc)) 
	{
		updatefilersrc(invnum,mtnrsrc);
		FINDFLDSETSTRING(invnum,"VENDOR IDENTIFICATION",vendorid);
		FINDFLDSETINT(invnum,"CALENDAR YEAR",venyrid);
		FINDFLDSETSTRING(invnum,"ADDRESS IDENTIFICATION",addrid1);
		field=FLDNRD(invnum,"ENTRY DATE");
		if(field!=NULL)
		{
			if(field->len<10)
			{
				FINDFLDSETSTRING(invnum,"ENTRY DATE",CURRENT_DATE);
			} else FINDFLDSETSTRING(invnum,"ENTRY DATE",CURRENT_DATE10);
		}
		FINDFLDSETSTRING(invnum,"ENTRY TIME",CURRENT_TIME);
		FINDFLDSETSTRING(invnum,"SOURCE USER",USERLOGIN);
		ClearRDATData(previous);
	} else {
		updatefilerecord2rsrc(invnum,mtnrsrc);
		GetRDATData(invnum,previous);
	}
        if(addrid1!=NULL) Rfree(addrid1);
        updateSCRNvirtuals(mtnrsrc);
        setaddrlist(mtnrsrc);
        changeADDRlist(mtnrsrc);
}

static void PURAVCIM_quit_record_test(RDArsrc *mtnrsrc,RDATData *previous)
{
	char *desc=NULL,deleteflag=FALSE;
	int selected_addr=0;

	readallwidgets(mtnrsrc);
	FINDRSCGETCHAR(mtnrsrc,"CLOSE PO",&ClosePO);
	rsrc2filerecord(invnum,mtnrsrc);
	FINDFLDSETCHAR(invnum,"DELETEFLAG",deleteflag);
	FINDFLDSETCHAR(invnum,"SOURCE USER VERIFIED",deleteflag);
	FINDRSCGETINT(mtnrsrc,"ADDRESS LIST",&selected_addr);
	FINDFLDSETSTRING(invnum,"ADDRESS IDENTIFICATION",
		addresslist->libs[selected_addr]);
	if(QUITvfy(invnum,previous))
	{
		desc=QUITvfydesc(invnum,previous);
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!",
			desc,PURAVCIM_save_record_up,quit_record,FALSE,2,mtnrsrc,previous,NULL);
		if(desc!=NULL) Rfree(desc);
	} else {
		quit_record(mtnrsrc,previous);
	}
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
	char deleteflag=FALSE;

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
	if((RDAstrlen(invdate))!=10)
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID INVOICE DATE!","INVALID INVOICE DATE: Your Invoice Date must be in a 10-digit format (e.g. MM/DD/YYYY).  Please key in a correct Invoice Date before proceeding.",NULL,NULL,FALSE,NULL);
		return(FALSE);
	}
	return(TRUE);
}
void saveinv_addvchs(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(mbl_purinv!=NULL)
	{
		if(PURAVCIM_save_check(mtnrsrc,previous,TRUE)) add_vouchers(mtnrsrc);
	} else {
		if(PURAVCIM_save_check(mtnrsrc,previous,FALSE)) add_vouchers(mtnrsrc);
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
	readwidget(r,"[VENINV][DUE DATE]");
	FINDRSCGETSTRING(r,"[VENINV][DUE DATE]",&duedate);
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
	fmonth=venpmtfmonth;
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
	if(save_venyr(mtnrsrc,previous))
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
/* purapv1.c - Add Purchase Order Vouchers Browse Screen */

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
		if(XPERT_SETUP->CIS==TRUE)
		{
			addDBfield(dbsrt,"USE CIS",BOOLNS,1);
			addDBfield(dbsrt,"PERSONNEL IDENTIFICATION",PLAINTEXT,15);
			addDBfield(dbsrt,"CIS TYPE",PLAINTEXT,15);
		}
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
	char falseflag=FALSE;
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
	COPYFIELD(vennum,dbsrt->fileno,"ELECTRONIC TRANSFER");
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
			if(venpmt->Use_Taxes==TRUE)
			{
				FINDFLDGETDOUBLE(accnum,"OUTSTANDING ACCOUNT COST",&cost);
				FINDFLDGETDOUBLE(accnum,"OUTSTANDING ACCOUNT TAX AMOUNT",&tax_amount);
				FINDFLDGETDOUBLE(accnum,"OUTSTANDING ACCOUNT ADMIN FEE",&adminfee);
				FINDFLDGETDOUBLE(accnum,"OUTSTANDING ACCOUNT PRORATE AMOUNT",&prorateamt);
			}
			addsortrecord(mainrsrc,dbsort,test_line,test_acct,cost,tax_amount,adminfee,prorateamt,amount,skip_records);
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
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
	char sep_vendor=FALSE,use_tax=FALSE,usecis=FALSE;
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
static void PURAPV2_quit_record_test(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *desc=NULL;

	readallwidgets(mtnrsrc);
	rsrc2singlefilerecord(dbsort->fileno,mtnrsrc);
	if(QUITvfy(dbsort->fileno,mtn->previous))
	{
		desc=QUITvfydesc(dbsort->fileno,mtn->previous);
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!",
			desc,PURAPV2_save_record_up,PURAPV2_quit_record,FALSE,2,(void *)mtnrsrc,(void *)mtn,NULL);
		if(desc!=NULL) Rfree(desc);
	} else {
		PURAPV2_quit_record(mtnrsrc,mtn);
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
					which_type=fmonth;
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
	FINDRSCSETFUNC(mtnrsrc,"VENDOR IDENTIFICATION",PURAPV2_getfilecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"CALENDAR YEAR",PURAPV2_getfilecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"INVOICE IDENTIFICATION",PURAPV2_getfilecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"TRANSACTION NUMBER",PURAPV2_getfilecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"ACCOUNT CODE",acctcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"DEBIT CODE",debcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"CREDIT CODE",credcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"PAYABLE CODE",paycodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"MANUAL VOUCHER",manualvouchercb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"ELECTRONIC TRANSFER",manualvouchercb,mtn);
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
	if(infnum!=(-1)) file2rsrc(infnum,mtnrsrc,FALSE);
	if(alias_num!=(-1)) file2rsrc(alias_num,mtnrsrc,FALSE);
	DefaultScreens(mtnrsrc);
	addDFincvir(mtnrsrc,module,"PURMST",NULL,msrnum);
	addDFincvir(mtnrsrc,module,"PURSIT",NULL,sitenum);
	addDFincvir(mtnrsrc,module,"PURLIN",NULL,linenum);
	addDFincvir(mtnrsrc,"BNKREC","BNKACC",NULL,bnkacc);
	addDFincvir(mtnrsrc,"VENPMT","VENMSR",NULL,vennum);
	addDFincvir(mtnrsrc,"VENPMT","VENYR",NULL,yearnum);
	addDFincvir(mtnrsrc,"VENPMT","VENINV",NULL,invnum);
	addDFincvir(mtnrsrc,"VENPMT","VENINF",NULL,infnum);
	addDFincvir(mtnrsrc,"FINMGT","FINALIAS",NULL,alias_num);
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
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,PURAPV2_save_record_nup,mtn);
	if(mbl_rec!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,PURAPV2_save_record_up,mtn);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,PURAPV2_save_record_nup,mtn);
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
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,PURAPV2_quit_record_test,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mtnrsrc);
/*
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_rec==NULL?TRUE:FALSE));
*/
	if(xmakescrn(mtnrsrc,TRUE,PURAPV_GUIEVAL,PSTRUCT,addoswidgets,PURAPV_GUIEVALSTR,PSTRUCT,
		PurApvSubData,mtn->p,NULL,__LINE__,__FILE__))
	{
		edesc=Rmalloc(180+RDAstrlen(mtnrsrc->module)+RDAstrlen(mtnrsrc->screen));
		sprintf(edesc,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",(mtnrsrc->module==NULL?"":mtnrsrc->module),(mtnrsrc->screen==NULL?"":mtnrsrc->screen));
		ERRORDIALOG("MAKESCRN FAILED",edesc,ShutdownOnError,TRUE);
		if(edesc!=NULL) Rfree(edesc);
		PURAPV2_quit_record(mtnrsrc,mtn);
	} else {
/* DCT 02-18-2010 */
		setACCTtype(mtnrsrc,mtn);
		updateallrsrc(mtnrsrc);
	}
}
static void seteditable(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char dist=FALSE;
	int accttype=0;

	FINDRSCGETCHAR(mtnrsrc,"DISTRIBUTED",&dist);
	manualvouchercb(mtnrsrc,mtn);
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
			FINDRSCSETEDITABLE(mtnrsrc,"ACCOUNT CODE",FALSE);
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
/*
static void quit_record_warn(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	PURAPV2_quit_record(mtnrsrc,mtn);
}
*/
static short check_manual(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int checknbr=0,etrnbr=0;
	int thisyear=0,chkyear=0,eftyear=0;
	char manualvoucher,electronicvoucher;
	char *checkdate=NULL,*etrdate=NULL;


	FINDRSCGETINT(mtnrsrc,"CHECK NUMBER",&checknbr);
	FINDRSCGETINT(mtnrsrc,"ELECTRONIC TRANSFER NUMBER",&etrnbr);
	FINDRSCGETCHAR(mtnrsrc,"MANUAL VOUCHER",&manualvoucher);
	FINDRSCGETCHAR(mtnrsrc,"ELECTRONIC TRANSFER",&electronicvoucher);
	FINDRSCGETSTRING(mtnrsrc,"ELECTRONIC TRANSFER DATE",&etrdate);
	FINDRSCGETSTRING(mtnrsrc,"CHECK DATE",&checkdate);
	if(manualvoucher && electronicvoucher && etrnbr==0)
	{
		ERRORDIALOG("ELECTRONIC TRANSFER NUMBER IS 0","Please enter an electronic transfer number greater than 0.",NULL,FALSE);
		return(TRUE);
	}
	if(manualvoucher && electronicvoucher && !etrdate)
	{
		ERRORDIALOG("ELECTRONIC TRANSFER DATE IS BLANK","Please enter a 10-digit electronic transfer date in this format: MM/DD/YYYY.",NULL,FALSE);
		return(TRUE);
	}
	if(manualvoucher && !electronicvoucher && checknbr==0)
	{
		ERRORDIALOG("CHECK NUMBER IS 0","Please enter a check number greater than 0.",NULL,FALSE);
		return(TRUE);
	}
	if(manualvoucher && !electronicvoucher && !checkdate)
	{
		ERRORDIALOG("CHECK DATE IS BLANK","Please enter a 10-digit check date in this format: MM/DD/YYYY.",NULL,FALSE);
		return(TRUE);
	}
	if(manualvoucher && !electronicvoucher && RDAstrlen(checkdate)<10)
	{
		ERRORDIALOG("CHECK DATE LESS THAN 10 DIGITS","Please enter a 10-digit check date (e.g. MM/DD/YYYY).",NULL,FALSE);
		return(TRUE);
	}
	if(manualvoucher && electronicvoucher && RDAstrlen(etrdate)<10)
	{
		ERRORDIALOG("ELECTRONIC DATE LESS THAN 10 DIGITS","Please enter a 10-digit electronic transfer date (e.g. MM/DD/YYYY).",NULL,FALSE);
		return(TRUE);
	}
	if(CURRENT_DATE10 != NULL) thisyear=(val(&CURRENT_DATE10[6],4));
	if(checkdate != NULL) chkyear=(val(&checkdate[6],4));
	if(etrdate != NULL) eftyear=(val(&etrdate[6],4));
	//prterr("thisyear = %d chkyear= %d eftyear = %d", thisyear,chkyear,eftyear);
	if(chkyear>thisyear)
	{
	 WARNINGDIALOG("WARNING DIALOG SCREEN","CHECK YEAR IS GREATER!","The Check year is greater than the current year." ,NULL,NULL,TRUE,NULL);
	}
	if(eftyear>thisyear)
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN","EFT YEAR IS GREATER!","The EFT year is greater than the current year." ,NULL,NULL,TRUE,NULL);
	}
	return(FALSE);
}
static void acctcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
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
static void manualvouchercb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char manualvoucher,electronicvoucher;
	readwidget(mtnrsrc,"MANUAL VOUCHER");
	FINDRSCGETCHAR(mtnrsrc,"MANUAL VOUCHER",&manualvoucher);
	readwidget(mtnrsrc,"ELECTRONIC TRANSFER");
	FINDRSCGETCHAR(mtnrsrc,"ELECTRONIC TRANSFER",&electronicvoucher);
	if(manualvoucher)
	{
		if(electronicvoucher)
		{
			FINDRSCSETSENSITIVE(mtnrsrc,"ELECTRONIC TRANSFER NUMBER",TRUE);
			FINDRSCSETSENSITIVE(mtnrsrc,"ELECTRONIC TRANSFER DATE",TRUE);
			FINDRSCSETSENSITIVE(mtnrsrc,"CHECK NUMBER",FALSE);
			FINDRSCSETSENSITIVE(mtnrsrc,"CHECK DATE",FALSE);
		} else {
			FINDRSCSETSENSITIVE(mtnrsrc,"ELECTRONIC TRANSFER NUMBER",FALSE);
			FINDRSCSETSENSITIVE(mtnrsrc,"ELECTRONIC TRANSFER DATE",FALSE);
			FINDRSCSETSENSITIVE(mtnrsrc,"CHECK NUMBER",TRUE);
			FINDRSCSETSENSITIVE(mtnrsrc,"CHECK DATE",TRUE);
		}	
	} else {
		FINDRSCSETSENSITIVE(mtnrsrc,"CHECK NUMBER",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"CHECK DATE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"ELECTRONIC TRANSFER NUMBER",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"ELECTRONIC TRANSFER DATE",FALSE);

	}
}
static short check_paycode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	mtn->acctwidget=3;
	return(check_code(mtnrsrc,mtn));
}
static void PURAPV2_save_record_up(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	mtn->update_list=TRUE;
	PURAPV2_save_check(mtnrsrc,mtn);
}
static void PURAPV2_save_record_nup(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	mtn->update_list=FALSE;
	PURAPV2_save_check(mtnrsrc,mtn);
}
static void PURAPV2_save_check(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int procyrid=0;
	char *tmp=NULL,active=FALSE,delflag=FALSE,venetf=FALSE,electronicvoucher=FALSE;
	int atype=0;

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
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID FISCAL YEAR!","The FISCAL YEAR entered is not valid.\nIt does not exist as a FINANCIAL YEAR.\n",NULL,NULL,FALSE,NULL);
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
			FINDFLDGETCHAR(vennum,"ELECTRONIC TRANSFER",&venetf);
			FINDRSCGETCHAR(mtnrsrc,"ELECTRONIC TRANSFER",&electronicvoucher);
			if(delflag==TRUE)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED VENDOR IDENTIFICATION!","The VENDOR IDENTIFICATION entered is deleted.\nIt exist as a VENDOR but is presently set up in a deletion state.\n",NULL,NULL,TRUE,NULL);
				prterr("The VENDOR IDENTIFICATION entered is deleted.  It exist as a VENDOR but is presently set up in a deletion state.");
				if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
				return;
			}
			if(electronicvoucher && !venetf)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","ELECTRONIC TRANSFER DOES NOT MATCH!","The Vendor does not have electronic transfer\nturned on, but the voucher is electronic transfer.\n",NULL,NULL,TRUE,NULL);
			}
		} else {
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID VENDOR IDENTIFICATION!","The VENDOR IDENTIFICATION entered is not valid.\nIt does not exist as a VENDOR.\n",NULL,NULL,TRUE,NULL);
			if(tmp!=NULL) Rfree(tmp);
			return;
		}
	}
	if(tmp!=NULL) Rfree(tmp);


	if(check_manual(mtnrsrc,mtn)==TRUE) return;

	FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&atype);
	if(atype<2)
	{
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
	} else {
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
	}
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
	PURAPV2_save_record(mtnrsrc,mtn);
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
			if(testavl==FALSE && SKIP_OVER_BUDGET_WARNINGS==FALSE)
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
				ADVWARNINGDIALOG("WARNING DIALOG SCREEN","AVAILABLE BALANCE NOT ENOUGH!",message,(void(*)(...))PURAPV2_save_record_warn,NULL,FALSE,2,(void *)mtnrsrc,(void *)mtn,NULL);
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
static void PURAPV2_save_record_warn(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	PURAPV2_save_record(mtnrsrc,mtn);
}
static void PURAPV2_save_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	short y=0,numflds=0,deftype1=0,source=1;
	NRDfield *tmpfld=NULL,*fldx=NULL,*tmp=NULL;
	double amount=0;
	char readdel=FALSE,manual_voucher=FALSE,electronic_transfer=FALSE,*bankid=NULL,*libx=NULL;
	RDAGenericSetup *gsv=NULL;
	int selectedd=0,checkno=0,orig_check=0;
	RDATData *prev=NULL;
	char record_check_number=FALSE,record_eft_number=FALSE;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("VENPMT")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"VENPMT");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"VENPMT");
#endif

	gsv=RDAGenericSetupNew("VENPMT","RECORD CHECK NUMBER");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			record_check_number=*gsv->value.string_value;
		} else {
			record_check_number=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("VENPMT","RECORD EFT NUMBER");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			record_eft_number=*gsv->value.string_value;
		} else {
			record_eft_number=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);

	readallwidgets(mtnrsrc);
	rsrc2singlefilerecord(dbsort->fileno,mtnrsrc);
	numflds=NUMFLDS(dbsort->fileno);
	if(numflds>0)
	{
		tmpfld=Rmalloc(numflds*sizeof(NRDfield));
		memset(tmpfld,0,(numflds*sizeof(NRDfield)));
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
		readwidget(mtnrsrc,"MANUAL VOUCHER");
		readwidget(mtnrsrc,"ELECTRONIC TRANSFER");
		FINDRSCGETCHAR(mtnrsrc,"MANUAL VOUCHER",&manual_voucher);
		FINDRSCGETCHAR(mtnrsrc,"ELECTRONIC TRANSFER",&electronic_transfer);
		if(manual_voucher==TRUE)
		{
			if(electronic_transfer==FALSE)
			{
				FINDFLDSETINT(dbsort->fileno,"ELECTRONIC TRANSFER NUMBER",0);
				FINDFLDSETSTRING(dbsort->fileno,"ELECTRONIC TRANSFER DATE","");
				FINDRSCGETINT(mtnrsrc,"CHECK NUMBER",&checkno);
				FINDFLDSETINT(dbsort->fileno,"VIRTUAL CHECK NUMBER",checkno);

				FINDFLDGETSTRING(dbsort->fileno,"BANK IDENTIFICATION",&bankid);
		
				if(record_check_number==TRUE)
				{
					prev=RDATDataNEW(bnkacc);
					FINDFLDGETINT(bnkacc,"LAST CHECK NUMBER",&orig_check);
					if(checkno>orig_check)
					{
						FINDFLDSETINT(bnkacc,"LAST CHECK NUMBER",checkno);
						FINDFLDSETSTRING(bnkacc,"BANK IDENTIFICATION",bankid);
						WRTTRANS(bnkacc,0,NULL,prev);
					}
					if(prev!=NULL) FreeRDATData(prev);
				}
			} else {
				FINDFLDSETINT(dbsort->fileno,"CHECK NUMBER",0);
				FINDFLDSETSTRING(dbsort->fileno,"CHECK DATE","");
				if(record_eft_number==TRUE)
				{
					FINDRSCGETINT(mtnrsrc,"ELECTRONIC TRANSFER NUMBER",&checkno);
					prev=RDATDataNEW(bnkacc);
					FINDFLDGETINT(bnkacc,"LAST ELECTRONIC TRANSFER NUMBER",&orig_check);
					if(checkno>orig_check)
					{
						FINDFLDSETINT(bnkacc,"LAST ELECTRONIC TRANSFER NUMBER",checkno);
						FINDFLDSETSTRING(bnkacc,"BANK IDENTIFICATION",bankid);
						WRTTRANS(bnkacc,0,NULL,prev);
					}
					if(prev!=NULL) FreeRDATData(prev);
				}
			}
		} else {
			FINDFLDSETINT(dbsort->fileno,"CHECK NUMBER",0);
			FINDFLDSETSTRING(dbsort->fileno,"CHECK DATE","");
			FINDFLDSETINT(dbsort->fileno,"ELECTRONIC TRANSFER NUMBER",0);
			FINDFLDSETSTRING(dbsort->fileno,"ELECTRONIC TRANSFER DATE","");
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
	if(mbl_rec!=NULL)
	{
		updatebrowse(mtn->update_list,mbl_rec,dbsort->fileno,module,mtnrsrc);
	}
	if(mbl_rec!=NULL) PURAPV2_quit_record(mtnrsrc,mtn);
}
static void PURAPV2_quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
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
		if(infnum!=(-1)) CLSNRD(infnum);
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
static void PURAPV2_getfilecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
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
			FINDRSCSETEDITABLE(mtnrsrc,"ACCOUNT CODE",FALSE);
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
	int lnum=0;

	if(alias_num!=(-1))
	{
		ZERNRD(alias_num);
		COPYFIELD(dbsort->fileno,alias_num,"ACCOUNT TYPE");
		COPYFIELD(dbsort->fileno,alias_num,"DEFINITION TYPE");
		COPYFIELD(dbsort->fileno,alias_num,"ACCOUNT CODE");
		if(ADVEQLNRDsec(alias_num,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(alias_num,1);
		if(doupdate)
		{
			updatefilerecord2rsrc(alias_num,mtnrsrc);
		} else {
			filerecord2rsrc(alias_num,mtnrsrc);
		}
	}
	if(infnum!=(-1))
	{
		COPYFIELD(dbsort->fileno,infnum,"VENDOR IDENTIFICATION");
		COPYFIELD(dbsort->fileno,infnum,"INVOICE IDENTIFICATION");
		if(ADVEQLNRDsec(infnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(infnum,1);
		if(doupdate)
		{
			updatefilerecord2rsrc(infnum,mtnrsrc);
		} else {
			filerecord2rsrc(infnum,mtnrsrc);
		}
	}
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
		FIELDCOPY(vennum,"DEFAULT 1099 PAYMENT",dbsort->fileno,"1099 PAYMENT");
		FIELDCOPY(vennum,"DEFAULT 1099 TYPE",dbsort->fileno,"1099 TYPE");
		singlefilerecord2rsrc(dbsort->fileno,mtnrsrc);
		updatersrc(mtnrsrc,"1099 PAYMENT");
		updatersrc(mtnrsrc,"1099 TYPE");
		
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
		FINDFLDGETINT(linenum,"LINE NUMBER",&lnum);
		if(doupdate)
		{
			updatefilerecord2rsrc(linenum,mtnrsrc);
		} else {
			filerecord2rsrc(linenum,mtnrsrc);
		}
		FINDRSCSETEDITABLE(mtnrsrc,"DESCRIPTION",TRUE);
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
		FINDRSCSETSENSITIVE(mtnrsrc,"ACCOUNT CODE",FALSE);
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
			FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE",NULL);
			FINDRSCSETSENSITIVE(mtnrsrc,"ACCOUNT CODE",FALSE);
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
