/* posacc.c - Xpert Position Account Master Browse */
/*lint -library */
#ifndef WIN32
#define __NAM__ "posacc.lnx"
#endif
#ifdef WIN32
#define __NAM__ "posacc.exe"
#endif
#include <app.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <alloc.h>
#include <gui.h>
#include <nrd.h>
#include <trans.h>
#include <sec.h>
#include <mix.h>
/*CPP_OPART posaccm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

struct mtnstructs
{
	APPlib *DEFtypes;
	APPlib *PDEFtypes;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern void posaccm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short msrnum=(-1),eaccnum=(-1),baccnum=(-1);
char editable=TRUE;
MakeBrowseList *mbl_acc=NULL;
MaintainMaster *MASTER=NULL;
void browse_acct(RDArsrc *,mtnstruct *);

void browse_acct(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtn!=NULL)
	{
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->PDEFtypes!=NULL) freeapplib(mtn->PDEFtypes);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc); 
	mainrsrc=RDArsrcNEW(MASTER->module,MASTER->browse_screen);
	searchrsrc=RDArsrcNEW(MASTER->module,MASTER->search_browse);
	definelist=RDArsrcNEW(MASTER->module,MASTER->define_list);
	ZERNRD(MASTER->passkey->fileno);
	ApplyPassKey(MASTER->passkey);
	GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,MASTER,
		SCRNvirtualSubData,(void *)mainrsrc);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	MakeRunFunction(mainrsrc,MASTER);
	mbl_acc=ADVPbrowse(MASTER->passkey->fileno,MASTER->passkey->keyno,mainrsrc,
		searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MASTER->passkey);
}
void main(int argc,char **argv)
{
	short filenum=(-1);
	char *module=NULL,*mtnname=NULL,*warnmessage;

	module="POSTRK";
	mtnname="MTN POSACC";
	if(InitializeSubsystems(argc,argv,module,mtnname)) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	MASTER=MaintainMasterNew(module,mtnname);
	if(findMaintainMaster(MASTER))
	{
		warnmessage=Rmalloc(300+RDAstrlen(MASTER->module)+RDAstrlen(mtnname));
		sprintf(warnmessage,"Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.",mtnname,MASTER->module);
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.",warnmessage,NULL,TRUE);
		prterr("%s",warnmessage);
		if(warnmessage!=NULL) Rfree(warnmessage);
		doexit(NULL);
	} else {
		if((filenum=APPOPNNRDEDITABLE(MASTER->module,"POSACC",&editable))==(-1))
		{
			doexit(NULL);
			return;
		}
		if(!OPEN_MASTER_FILES(MASTER))
		{
			doexit(NULL);
			return;
		}
		eaccnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINEACC",1);
		baccnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINBACC",1);
		MASTER->passkey=READPassKey(filenum);
		MASTER->inheritrsrcs=TRUE;
		switch(MASTER->start_with)
		{
			default:
			case 0:
				posaccm(1,NULL);
				break;
			case 1:
				browse_acct(NULL,NULL);
				break;
		}
	}
	RDAAPPMAINLOOP();
}
void doexit(MakeBrowseList *blist)
{
	if(blist!=NULL) 
	{
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
	if(WindowCount<2)
	{
		if(MASTER!=NULL)
		{
			CLOSE_MASTER_FILES(MASTER);
			FreeMaintainMaster(MASTER);
		}
		ShutdownSubsystems();
		exit(0);
	}
	if(mbl_acc!=NULL) mbl_acc=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func))
	{
		if(!run_func)
		{
			posaccm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MASTER);
		}
	} else {
		posaccm(0,targetkey);
	}
}
static void new_record(RDArsrc *mtnrsrc,MakeBrowseList *blist)
{
	posaccm(2,NULL);
} 
