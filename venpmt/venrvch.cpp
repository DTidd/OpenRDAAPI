/*lint -library */
#ifndef WIN32
#define __NAM__ "venrvch.lnx"
#endif
#ifdef WIN32
#define __NAM__ "venrvch.exe"
#endif
#include <app.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <mix.h>
#include <fin.h>
/*CPP_OPART venrvchm */
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
	APPlib *ACCTtypes;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern void venrvchm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short finbacc=(-1),finracc=(-1),fineacc=(-1);
char editable=TRUE;
MakeBrowseList *mbl_venrvch=NULL;
char *module="VENPMT";
void browse_venrvch(RDArsrc *,mtnstruct *);
MaintainMaster *MASTER=NULL;

void browse_venrvch(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	if(mtn!=NULL)
	{
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->ACCTtypes!=NULL) freeapplib(mtn->ACCTtypes);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	mainrsrc=RDArsrcNEW(MASTER->module,MASTER->browse_screen);
	searchrsrc=RDArsrcNEW(MASTER->module,MASTER->search_browse);
	definelist=RDArsrcNEW(MASTER->module,MASTER->define_list);
	ZERNRD(MASTER->passkey->fileno);
	ApplyPassKey(MASTER->passkey);
	GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,MASTER,SCRNvirtualSubData,(void *)mainrsrc);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	MakeRunFunction(mainrsrc,MASTER);
	mbl_venrvch=ADVPbrowse(MASTER->passkey->fileno,MASTER->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MASTER->passkey);
}
void main(int argc,char **argv)
{
	short filenum=(-1);
	char *warnmessage=NULL;
	char *module="VENPMT",*mtnname="MTN VENRVCH";

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
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND!",warnmessage,NULL,TRUE);
		prterr("%s",warnmessage);
		if(warnmessage!=NULL) Rfree(warnmessage);
		doexit(NULL);
	} else {
		if((filenum=APPOPNNRDEDITABLE(MASTER->module,MASTER->mainfile,
			&editable))==(-1))
		{
			doexit(NULL);
			return;
		}
		if(!OPEN_MASTER_FILES(MASTER))
		{
			doexit(NULL);
			return;
		}
		fineacc=APPReturnSupportingFileno(MASTER,"FINMGT","FINEACC",1);
		finracc=APPReturnSupportingFileno(MASTER,"FINMGT","FINRACC",1);
		finbacc=APPReturnSupportingFileno(MASTER,"FINMGT","FINBACC",1);
		MASTER->passkey=READPassKey(filenum);
		MASTER->inheritrsrcs=TRUE;
		switch(MASTER->start_with)
		{
			default:
			case 0:
				venrvchm(1,NULL);
				break;
			case 1:
				browse_venrvch(NULL,NULL);
				break;
		}
	}
	RDAAPPMAINLOOP();
}
static void doexit(MakeBrowseList *blist)
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
		CLOSE_MASTER_FILES(MASTER);
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
		ShutdownSubsystems();
	}
	if(mbl_venrvch!=NULL) mbl_venrvch=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func)
		{
			venrvchm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MASTER);
		}
	} else {
		venrvchm(0,targetkey);
	}
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	venrvchm(2,NULL);
} 
