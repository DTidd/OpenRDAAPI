/* veninv.c - Xpert Vendor Invoices Master Program*/
/*lint -library */
#include <cstdio>

#include <mix.hpp>

extern void setaddrlist(RDArsrc *);
extern void changeADDRlist(RDArsrc *);
extern short vchnum,yearnum,expnum,revnum,balnum,vennum,adrnum;
extern short byrnum,eyrnum,ryrnum,infnum,invnum,msrnum,sitenum;
extern char *poid,*vendorid,*invid,*checkdate;
extern char manual_voucher,prepaid,pay_1099,check_detail;
extern int yearid,checkno,venyrid;
extern char *module;
static void PURAVCI_doexit(MakeBrowseList *),PURAVCI_okfunc(MakeBrowseList *,void *);
static void initfunc(MakeBrowseList *);
static short PURAVCI_selectfunction(MakeBrowseList *);
MakeBrowseList *mbl_purinv=NULL;
void browse_inv(RDArsrc *,RDATData *);
struct HoldIts
{
	RDArsrc *mtnrsrc;
	RDATData *previous;
};
typedef struct HoldIts HoldIt;

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
