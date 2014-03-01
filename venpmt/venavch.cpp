/* venavch.c - Vendor Add Vouchers Main Window*/
#ifndef WIN32
#define __NAM__ "venavch.lnx"
#endif
#ifdef WIN32
#define __NAM__ "venavch.exe"
#endif
#include <app.h>
#include <stdlib.h>
#include <rdaglob.h>
#include <addlists.h>
#include <mix.h>
#include <fin.h>
#include <venpmt.h>
/*CPP_OPART venavch2 */
/*CPP_OPART venavci */
/*CPP_OPART venavcim */
/*CPP_OPART venavch1 */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

extern char editinv;
extern APPlib *DEFtypes,*acctavl,*debitavl,*creditavl,*payableavl;
extern APPlib *clearDEFtypes,*duetoavl,*clearfromavl,*clearcashavl;
char editable=TRUE;
extern MakeBrowseList *mbl_venmsr;
APPlib *venlist=NULL/*,*yearlist=NULL*/;
short vchnum=(-1),yearnum=(-1),expnum=(-1),revnum=(-1),balnum=(-1);
short filenum=(-1),adrnum=(-1);
short byrnum=(-1),eyrnum=(-1),ryrnum=(-1),invnum=(-1),finyrnum=(-1);
short banknum=(-1);
char *module="VENPMT";
RDAfinmgt *finsetup=NULL;
RDAvenpmt *venpmt=NULL;
extern void venmsrm(short,void *);
extern void veninvm(short,void *);
char *reportname=NULL;
int VOUCHER_COUNT=0;
MaintainMaster *VENMSR_MASTER=NULL,*VENINV_MASTER=NULL;
extern char which_modal;

static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
static short selectfunction(MakeBrowseList *);
int getcurrentcalyear(void);
void browse_venmsr(RDArsrc *,RDATData *);
extern int Calendar_Year;

void okfunc(MakeBrowseList *b,void *target)
{
	readwidget(b->mainrsrc,"CALENDAR YEAR");
	FINDRSCGETINT(b->mainrsrc,"CALENDAR YEAR",&Calendar_Year);
	veninvm(1,target);
}
void new_record(RDArsrc *parent,MakeBrowseList *b)
{
	which_modal=TRUE;
	venmsrm(2,NULL);
}
short selectfunction(MakeBrowseList *b)
{
	return(TRUE);
}
static void doexit(MakeBrowseList *blist)
{
	short exit_program=FALSE;
	APPlib *args=NULL;

	if(acctavl!=NULL) { freeapplib(acctavl); acctavl=NULL; }
	if(debitavl!=NULL) { freeapplib(debitavl); debitavl=NULL; }
	if(creditavl!=NULL) { freeapplib(creditavl); creditavl=NULL; }
	if(payableavl!=NULL) { freeapplib(payableavl); payableavl=NULL; }
	if(DEFtypes!=NULL) { freeapplib(DEFtypes); DEFtypes=NULL; }
	if(duetoavl!=NULL) { freeapplib(duetoavl); duetoavl=NULL; }
	if(clearDEFtypes!=NULL) { freeapplib(clearDEFtypes); clearDEFtypes=NULL; }
	if(clearfromavl!=NULL) { freeapplib(clearfromavl); clearfromavl=NULL; }
	if(clearcashavl!=NULL) { freeapplib(clearcashavl); clearcashavl=NULL; }
	if(VOUCHER_COUNT>0)
	{
		args=APPlibNEW();
		addAPPlib(args,"VENPMT");
		addAPPlib(args,reportname);
		ExecuteProgram("doreport",args);	
		if(args!=NULL) freeapplib(args);
	} else {
		exit_program=TRUE;
	}
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
		blist->mainrsrc=NULL;
	}
	if(filenum!=(-1)) CLSNRD(filenum);
	if(invnum!=(-1)) CLSNRD(invnum);
	if(vchnum!=(-1)) CLSNRD(vchnum);
	if(adrnum!=(-1)) CLSNRD(adrnum);
	if(expnum!=(-1)) CLSNRD(expnum);
	if(revnum!=(-1)) CLSNRD(revnum);
	if(balnum!=(-1)) CLSNRD(balnum);
	if(yearnum!=(-1)) CLSNRD(yearnum);
	if(finyrnum!=(-1)) CLSNRD(finyrnum);
	if(eyrnum!=(-1)) CLSNRD(eyrnum);
	if(ryrnum!=(-1)) CLSNRD(ryrnum);
	if(byrnum!=(-1)) CLSNRD(byrnum);
	if(banknum!=(-1)) CLSNRD(banknum);
	if(VENINV_MASTER!=NULL) FreeMaintainMaster(VENINV_MASTER);
	if(VENMSR_MASTER!=NULL) FreeMaintainMaster(VENMSR_MASTER);
	if(finsetup!=NULL) free_finmgt(finsetup);
	if(venpmt!=NULL) free_venpmt(venpmt);
	if(reportname!=NULL) Rfree(reportname);
	if(exit_program)
	{
		ShutdownSubsystems();
		exit(0);
	}
}
int getcurrentcalyear(void)
{
	int currentcalyear=0;

	currentcalyear=GETCURRENTCALENDARYEAR();
	return(currentcalyear);
}
void browse_venmsr(RDArsrc *mtnrsrc,RDATData *previous)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;
	int currentcalyear=0;

	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	if(previous!=NULL) FreeRDATData(previous);
	mainrsrc=RDArsrcNEW(VENMSR_MASTER->module,VENMSR_MASTER->browse_screen);
	searchrsrc=RDArsrcNEW(VENMSR_MASTER->module,VENMSR_MASTER->search_browse);
	definelist=RDArsrcNEW(VENMSR_MASTER->module,VENMSR_MASTER->define_list);
        GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,VENMSR_MASTER,SCRNvirtualSubData,(void *)mainrsrc);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
        GET_SCREEN_VIRTUALS(definelist,1);
        GET_SCREEN_VIRTUALS(searchrsrc,2);
	MakeRunFunction(mainrsrc,VENMSR_MASTER);
	currentcalyear=getcurrentcalyear();
	addstdrsrc(mainrsrc,"CALENDAR YEAR",LONGV,4,&currentcalyear,TRUE);
	DefaultScreens(mainrsrc);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	mbl_venmsr=ADVbrowse(VENMSR_MASTER->passkey->fileno,1,
		mainrsrc,searchrsrc,definelist,NULL,
		selectfunction,doexit,okfunc,new_record,
		NULL,TRUE,NULL,0,0,1,FALSE);
}
void main(int argc,char **argv)
{
	PassKey *tmpPassKey=NULL,*tmpPassKeyVENMSR;
	short keyno=(-1);
	char *warnmessage=NULL;
	char *module="VENPMT",*mtnname="ADD VENMSR";

	if(InitializeSubsystems(argc,argv,module,"ADD VOUCHERS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	GetAddList();
	if(argc>1)
	{
		reportname=stralloc(argv[1]);
	} else reportname=stralloc("VCAUDIT");
	if((byrnum=APPOPNNRD("FINMGT","FINBYR",TRUE,TRUE))==(-1)) return;
	if((eyrnum=APPOPNNRD("FINMGT","FINEYR",TRUE,TRUE))==(-1)) return;
	if((ryrnum=APPOPNNRD("FINMGT","FINRYR",TRUE,TRUE))==(-1)) return;
	if((yearnum=APPOPNNRD(module,"VENYR",TRUE,FALSE))==(-1)) return;
	if((finyrnum=APPOPNNRD("FINMGT","FINYEAR",TRUE,FALSE))==(-1)) return;
	if((expnum=APPOPNNRD("FINMGT","FINEACC",TRUE,FALSE))==(-1)) return;
	if((revnum=APPOPNNRD("FINMGT","FINRACC",TRUE,FALSE))==(-1)) return;
	if((balnum=APPOPNNRD("FINMGT","FINBACC",TRUE,FALSE))==(-1)) return;
	editinv=TRUE;
	if((invnum=APPOPNNRDEDITABLE(module,"VENINV",&editinv))==(-1)) return;
	if((vchnum=APPOPNNRD(module,"VENVCH",TRUE,TRUE))==(-1)) return;
	editable=TRUE;
	if((filenum=APPOPNNRDEDITABLE(module,"VENMSR",&editable))==(-1)) return;
	if((adrnum=APPOPNNRD(module,"VENADR",TRUE,FALSE))==(-1)) return;
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
	if(getfinmgtbin(finsetup)==(-1))
	{
		ERRORDIALOG("Financial Management Setup Not Available","The Financial Management Setup File cannot opened.\n",NULL,TRUE);
		ShutdownSecurity();
		RDAAPPMAINLOOP();
		return;
	}
	venpmt=RDAvenpmtNEW();
	if(getvenpmtbin(venpmt)==(-1))
	{
		ERRORDIALOG("Venpmt Setup Not Available","The Venpmt Setup File cannot opened.\n",NULL,TRUE);
		ShutdownSecurity();
		RDAAPPMAINLOOP();
		return;
	}
	keyno=KEYNUMBER(invnum,"VENDOR INVOICE KEY");
	if(keyno==(-1))
	{
		keyno=1;
	}
	VENINV_MASTER=MaintainMasterNew("VENPMT","ADD VENDOR INVOICE");
	VENINV_MASTER->mainfile=stralloc("VENINV");
	VENINV_MASTER->browse_screen=stralloc("ADD VENDOR INVOICE BROWSE");
	VENINV_MASTER->search_browse=stralloc("VENDOR INVOICE SEARCH BROWSE");
	VENINV_MASTER->define_list=stralloc("VENDOR INVOICE DEFINE LIST");
	VENINV_MASTER->maintain_screen=stralloc("ADD INVOICE SCREEN");
	tmpPassKey=PassKeyNew();
	tmpPassKey->fileno=invnum;
	tmpPassKey->keyno=keyno;
	VENINV_MASTER->passkey=tmpPassKey;
/*
	VENINV_MASTER->inheritrsrcs=FALSE;
*/
	VENINV_MASTER->inheritrsrcs=TRUE;
	VOUCHER_COUNT=0;
	keyno=KEYNUMBER(filenum,"VENDOR ID KEY");
	if(keyno==(-1))
	{
		keyno=1;
	}
	VENMSR_MASTER=MaintainMasterNew(module,mtnname);
	if(findMaintainMaster(VENMSR_MASTER))
	{
		warnmessage=Rmalloc(300+RDAstrlen(VENMSR_MASTER->module)+RDAstrlen(mtnname));
		sprintf(warnmessage,"Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.",mtnname,VENMSR_MASTER->module);
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.",warnmessage,NULL,TRUE);
		prterr("%s",warnmessage);
		if(warnmessage!=NULL) Rfree(warnmessage);
		doexit(NULL);
	} else {
		tmpPassKeyVENMSR=PassKeyNew();
		tmpPassKeyVENMSR->fileno=filenum;
		tmpPassKeyVENMSR->keyno=keyno;
		VENMSR_MASTER->passkey=tmpPassKeyVENMSR;
		VENMSR_MASTER->inheritrsrcs=TRUE;
		if(VENMSR_MASTER->start_with==1)
		{
		browse_venmsr(NULL,NULL);
		} else {
		venmsrm(2,NULL);
		}
		RDAAPPMAINLOOP();
	}
}
