/* veninv.c - Xpert Vendor Invoices Master Program*/
/*lint -library */
#include <stdio.h>
#include <rdaglob.h>
#include <mix.h>

struct mtnstructs
{
	APPlib *addresslist;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern int setaddrlist(RDArsrc *,mtnstruct *);
extern int changeADDRlist(RDArsrc *,mtnstruct *);
extern short vchnum,yearnum,expnum,revnum,balnum,filenum,adrnum;
extern short byearnum,eyearnum,ryearnum,invnum;
extern MaintainMaster *VENINV_MASTER;
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void initfunc(MakeBrowseList *);
static short selectfunction(MakeBrowseList *);
MakeBrowseList *mbl_veninv=NULL;
char *vendorid=NULL,*invid=NULL;
int venyrid=0;
void browse_veninv(RDArsrc *,mtnstruct *);

struct HoldIts
{
	RDArsrc *mtnrsrc;
	mtnstruct *mtn;
};
typedef struct HoldIts HoldIt;

void browse_veninv(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;
	HoldIt *h=NULL;

	h=Rmalloc(sizeof(HoldIt));
	h->mtnrsrc=mtnrsrc;
	h->mtn=mtn;
	mainrsrc=RDArsrcNEW(VENINV_MASTER->module,VENINV_MASTER->browse_screen);
	searchrsrc=RDArsrcNEW(VENINV_MASTER->module,VENINV_MASTER->search_browse);
	definelist=RDArsrcNEW(VENINV_MASTER->module,VENINV_MASTER->define_list);
	addDFincvir(mainrsrc,"VENPMT","VENYR",NULL,yearnum);
	addDFincvir(mainrsrc,"VENPMT","VENMSR",NULL,filenum);
	addDFincvir(definelist,"VENPMT","VENINV",NULL,invnum);
	addDFincvir(searchrsrc,"VENPMT","VENINV",NULL,invnum);
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
		if(filenum!=(-1))
		{
			file2rsrc(filenum,mainrsrc,FALSE);
			FINDFLDSETSTRING(filenum,"VENDOR IDENTIFICATION",vendorid);
			if(ADVEQLNRDsec(filenum,1,SCRNvirtualSubData,mainrsrc)) 
				KEYNRD(filenum,1);
			filerecord2rsrc(filenum,mainrsrc);
		}
	} else {
		if(vendorid!=NULL) Rfree(vendorid);
		venyrid=0;
		if(invid!=NULL) Rfree(invid);
	}
	SCRNvirtual2rsrc(mainrsrc);
	computeallSCRNvirtuals(mainrsrc);
	mbl_veninv=ADVbrowse(invnum,1,mainrsrc,searchrsrc,definelist
		,initfunc,selectfunction,doexit,okfunc,NULL,h,TRUE,
		NULL,2,2,2,FALSE);
}
static void initfunc(MakeBrowseList *blist)
{
	if(vendorid!=NULL)
	{
		FINDFLDSETSTRING(invnum,"VENDOR IDENTIFICATION",vendorid);
		FINDFLDSETINT(invnum,"CALENDAR YEAR",venyrid);
	}
}
static short selectfunction(MakeBrowseList *blist)
{
	char *venid1=NULL;
	int yearid=0;

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
static void doexit(MakeBrowseList *blist)
{
	mbl_veninv=NULL;
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
static void okfunc(MakeBrowseList *b,void *targetkey)
{
	RDArsrc *mtnrsrc=NULL;
	mtnstruct *mtn=NULL;
	HoldIt *h=NULL;
	char *addrid1=NULL;

	h=b->funcargs;
	if(h!=NULL)
	{
		mtnrsrc=h->mtnrsrc;
		mtn=h->mtn;
		Rfree(h);
	}
	mbl_veninv=NULL;
	if(b->definelist!=NULL) killwindow(b->definelist);
	if(b->searchrsrc!=NULL) killwindow(b->searchrsrc);
/*
	ZERNRD(invnum);
*/
	FINDFLDGETSTRING(invnum,"ADDRESS IDENTIFICATION",&addrid1);
	KEYNRD(invnum,1);
	readallwidgets(mtnrsrc);
	if(ADVEQLNRDKEYsec(invnum,1,targetkey,SCRNvirtualSubData,mtnrsrc))
	{
		updatefilersrc(invnum,mtnrsrc);
		FINDFLDSETSTRING(invnum,"VENDOR IDENTIFICATION",vendorid);
		FINDFLDSETINT(invnum,"CALENDAR YEAR",venyrid);
		FINDFLDSETSTRING(invnum,"ADDRESS IDENTIFICATION",addrid1);
		ClearRDATData(mtn->previous);
	} else {
		updatefilerecord2rsrc(invnum,mtnrsrc);
		GetRDATData(invnum,mtn->previous);
	}
	if(addrid1!=NULL) Rfree(addrid1);
	updateSCRNvirtuals(mtnrsrc);
	setaddrlist(mtnrsrc,mtn);
	changeADDRlist(mtnrsrc,mtn);
}
