/* reqadd.c - Xpert Add Requisitions Program*/
/*lint -library */
#ifndef WIN32
#define __NAM__ "reqadd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "reqadd.exe"
#endif
#include <app.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <reqmgt.hpp>
/*CPP_OPART reqaddm */
/*CPP_OPART reqadds */
/*CPP_OPART reqaddc */
/*CPP_OPART reqaddi */
/*CPP_OPART reqaddv */
/*CPP_OPART reqaddvm */
/*CPP_OPART reqfunc */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

short reqmst=(-1),vennum=(-1),reqven=(-1),reqpven=(-1),reqloc=(-1);
short reqitm=(-1),reqacc=(-1),reqcat=(-1),reqctlg=(-1);
short eyrnum=(-1),byrnum=(-1),venadrnum=(-1);
short expnum=(-1),balnum=(-1),finyrnum=(-1),banknum=(-1);
extern RDAfinmgt *finsetup;
void browse_msr(RDArsrc *mtnrsrc,RDATData *previous);
char editven=TRUE,*reportname=NULL,*module="REQMGT";
extern int REQ_COUNT;
char *reqid=NULL,*locid=NULL;
extern APPlib *locations;
extern MakeBrowseList *mbl_reqmst;
extern void reqmstm(short,void *);
extern void setlocationids(RDArsrc *);
extern void getsupportingrecords(RDArsrc *,char);
extern APPlib *acctavl,*debitavl,*creditavl,*sitesavl;
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void initfunc(MakeBrowseList *);
static short breakfunc(MakeBrowseList *b);
static short selectfunc(MakeBrowseList *b);

struct HoldIts
{
	RDArsrc *mtnrsrc;
	RDATData *previous;
};
typedef struct HoldIts HoldIt;

void browse_msr(RDArsrc *mtnrsrc,RDATData *previous)
{
	HoldIt *h=NULL;
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;
	int x=0;

	h=Rmalloc(sizeof(HoldIt));
	h->mtnrsrc=mtnrsrc;
	h->previous=previous;
	FINDRSCGETINT(mtnrsrc,"LOCATION IDENTIFICATIONS",&x);
	if(locid!=NULL) Rfree(locid);
	locid=stralloc(locations->libs[x]);
	mainrsrc=RDArsrcNEW(module,"ADD REQUISITION BROWSE");
	addDFincvir(mainrsrc,module,"REQLOC",NULL,reqloc);
	searchrsrc=RDArsrcNEW(module,"ADD REQUISITION SEARCH BROWSE");
	definelist=RDArsrcNEW(module,"ADD REQUISITION DEFINE LIST");
	addDFincvir(searchrsrc,module,"REQMST",NULL,reqmst); 
	addDFincvir(searchrsrc,module,"REQLOC",NULL,reqloc);
	addDFincvir(definelist,module,"REQMST",NULL,reqmst); 
	addDFincvir(definelist,module,"REQLOC",NULL,reqloc);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	ZERNRD(reqloc);
	file2rsrc(reqloc,mainrsrc,FALSE);
	FINDFLDSETSTRING(reqloc,"LOCATION IDENTIFICATION",locid);
	if(ADVEQLNRDsec(reqloc,1,SCRNvirtualSubData,mainrsrc)) KEYNRD(reqloc,1);
	filerecord2rsrc(reqloc,mainrsrc);
	mbl_reqmst=ADVSbrowse(reqmst,1,mainrsrc,searchrsrc,definelist,initfunc,
		breakfunc,selectfunc,doexit,okfunc,NULL,h,TRUE,NULL,1,1,1,FALSE);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	if(InitializeSubsystems(argc,argv,module,"ADD REQUISITIONS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if(argc>1)
	{
		reportname=stralloc(argv[1]);
	} else reportname=stralloc("ADDADT");
	if((vennum=APPOPNNRDEDITABLE("VENPMT","VENMSR",&editven))==(-1)) return;
	if((venadrnum=APPOPNNRD("VENPMT","VENADR",TRUE,FALSE))==(-1)) return;
	if((reqmst=APPOPNNRD(module,"REQMST",TRUE,TRUE))==(-1)) return;
	if((reqitm=APPOPNNRD(module,"REQITM",TRUE,TRUE))==(-1)) return;
	if((reqacc=APPOPNNRD(module,"REQACC",TRUE,TRUE))==(-1)) return;
	if((reqloc=APPOPNNRD(module,"REQLOC",TRUE,FALSE))==(-1)) return;
	if((reqven=APPOPNNRD("CATALOGUE","CTLGVEN",TRUE,FALSE))==(-1)) return;
	if((reqpven=APPOPNNRD("CATALOGUE","CTLGVP",TRUE,FALSE))==(-1)) return;
	if((reqcat=APPOPNNRD("CATALOGUE","CATMST",TRUE,FALSE))==(-1)) return;
	if((reqctlg=APPOPNNRD("CATALOGUE","CTLGMST",TRUE,FALSE))==(-1)) return;
	if((expnum=APPOPNNRD("FINMGT","FINEACC",TRUE,FALSE))==(-1)) return;
	if((balnum=APPOPNNRD("FINMGT","FINBACC",TRUE,FALSE))==(-1)) return;
	if((eyrnum=APPOPNNRD("FINMGT","FINEYR",TRUE,FALSE))==(-1)) return;
	if((byrnum=APPOPNNRD("FINMGT","FINBYR",TRUE,FALSE))==(-1)) return;
	if((finyrnum=APPOPNNRD("FINMGT","FINYEAR",TRUE,FALSE))==(-1)) return;
	if((banknum=OPNNRDsec("BNKREC","BNKACC",TRUE,FALSE,FALSE))==(-1))
	{
		if((banknum=OPNNRD("BNKREC","BNKACC"))>(-1))
		{
			ERRORDIALOG("Security Access Denied","Security Access denied to the [BNKACC] file or the file cannot otherwise be opened.\n",NULL,TRUE);
			ShutdownSecurity();
			RDAAPPMAINLOOP();
			return;
		} else {
			CLSNRD(banknum);
			banknum=(-1);
		}
	}
	finsetup=RDAfinmgtNEW();
	if(GetFinmgtSetup(finsetup,NULL,NULL)==(-1)) return;
	REQ_COUNT=0;
	reqmstm(1,NULL);
	RDAAPPMAINLOOP();
}
static void initfunc(MakeBrowseList *b)
{
	ZERNRD(reqmst);
	if(!isEMPTY(locid))
		FINDFLDSETSTRING(reqmst,"LOCATION IDENTIFICATION",locid);
}
static short breakfunc(MakeBrowseList *b)
{
	char *locid1=NULL;

	FINDFLDGETSTRING(reqmst,"LOCATION IDENTIFICATION",&locid1);
	if(RDAstrcmp(locid,locid1))
	{
		if(locid1!=NULL) Rfree(locid1);
		return(FALSE);
	} else {
		if(locid1!=NULL) Rfree(locid1);
		return(TRUE);
	}
}
static short selectfunc(MakeBrowseList *b)
{
	char flag=FALSE;

	FINDFLDGETCHAR(reqmst,"DELETEFLAG",&flag);
	if(flag) return(FALSE);
	FINDFLDGETCHAR(reqmst,"SUBMITTED",&flag);
	if(flag) return(FALSE);
	FINDFLDGETCHAR(reqmst,"APPROVED",&flag);
	if(flag) return(FALSE);
	return(TRUE);
}
static void doexit(MakeBrowseList *b)
{
	if(b->funcargs!=NULL) Rfree(b->funcargs);
	if(b->definelist!=NULL) 
	{
		killwindow(b->definelist);
		free_rsrc(b->definelist);
		b->definelist=NULL;
	}
	if(b->searchrsrc!=NULL) 
	{
		killwindow(b->searchrsrc);
		free_rsrc(b->searchrsrc);
		b->searchrsrc=NULL;
	}
	if(b->mainrsrc!=NULL)
	{
		free_rsrc(b->mainrsrc);
		b->mainrsrc=NULL;
	}
}
static void okfunc(MakeBrowseList *b,void *targetkey)
{
	RDArsrc *mtnrsrc=NULL;
	RDATData *previous=NULL;
	HoldIt *h;
	int x;

	if(b->funcargs!=NULL)
	{
		h=(HoldIt *)b->funcargs;
		mtnrsrc=h->mtnrsrc;
		previous=h->previous;
		Rfree(b->funcargs);
	}
	if(b->mainrsrc!=NULL) free_rsrc(b->mainrsrc);
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
	FINDRSCGETINT(mtnrsrc,"LOCATION IDENTIFICATIONS",&x);
	readwidget(mtnrsrc,"[REQMST][REQUISITION IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[REQMST][REQUISITION IDENTIFICATION]",&reqid);
	ZERNRD(reqmst);
	if(ADVEQLNRDKEYsec(reqmst,1,targetkey,SCRNvirtualSubData,mtnrsrc))
	{
		FINDFLDSETSTRING(reqmst,"REQUISITION IDENTIFICATION",reqid);
		FINDFLDSETSTRING(reqmst,"LOCATION IDENTIFICATION",locations->libs[x]);
		ClearRDATData(previous);
		if(locid!=NULL) Rfree(locid);
		locid=stralloc(locations->libs[x]);
	} else {
		GetRDATData(reqmst,previous);
		filerecord2rsrc(reqmst,mtnrsrc);
		setlocationids(mtnrsrc);
		FINDFLDGETSTRING(reqmst,"REQUISITION IDENTIFICATION",&reqid);
		FINDRSCGETINT(mtnrsrc,"LOCATION IDENTIFICATIONS",&x);
		if(locid!=NULL) Rfree(locid);
		locid=stralloc(locations->libs[x]);
	}
	getsupportingrecords(mtnrsrc,TRUE);
	updateallrsrc(mtnrsrc);
}
