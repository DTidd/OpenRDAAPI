/* finjen.c - Xpert Journal Entry Browse Program */
/*lint -library */
#ifndef WIN32
#define __NAM__ "finjen.lnx"
#endif
#ifdef WIN32
#define __NAM__ "finjen.exe"
#endif
#include <app.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <gui.h>
#include <nrd.h>
#include <trans.h>
#include <sec.h>
#include <mix.h>
#include <fin.h>
/*CPP_OPART finjenm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

struct mtnstructs
{
	APPlib *ACCTtypes;
	APPlib *DEFtypes;
	APPlib *jtypes;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern char editable;
extern void finjenm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short baccnum=(-1),eaccnum=(-1),raccnum=(-1);
short byrnum=(-1),eyrnum=(-1),ryrnum=(-1);
RDAfinmgt *finsetup=NULL;
short distributable=FALSE;
MakeBrowseList *mbl_finjen=NULL;
void browse_finjen(RDArsrc *,mtnstruct *);
MaintainMaster *MASTER=NULL;

void browse_finjen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtn!=NULL)
	{
		if(mtn->ACCTtypes!=NULL) freeapplib(mtn->ACCTtypes);
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->jtypes!=NULL) freeapplib(mtn->jtypes);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	mainrsrc=RDArsrcNEW(MASTER->module,MASTER->browse_screen);
	searchrsrc=RDArsrcNEW(MASTER->module,MASTER->search_browse);
	definelist=RDArsrcNEW(MASTER->module,MASTER->define_list);
	ZERNRD(MASTER->passkey->fileno);
	ApplyPassKey(MASTER->passkey);
	GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,MASTER,SCRNvirtualSubData,(void *)mainrsrc);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	GET_SCREEN_VIRTUALS(definelist,1);
	MakeRunFunction(mainrsrc,MASTER);
	mbl_finjen=ADVPbrowse(MASTER->passkey->fileno,MASTER->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MASTER->passkey);
}
void main(int argc,char **argv)
{
	short filenum=(-1);
	char *warnmessage=NULL;
	char *module=NULL,*mtnname=NULL;

	module="FINMGT";
	mtnname="MTN FINJEN";
	if(InitializeSubsystems(argc,argv,module,mtnname))
	{
		RDAAPPMAINLOOP();
		return;
	}
	finsetup=RDAfinmgtNEW();
	if(GetFinmgtSetup(finsetup,NULL,NULL)==(-1)) return;
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
		if((filenum=APPOPNNRDEDITABLE(MASTER->module,MASTER->mainfile,&editable))==(-1))
		{
			doexit(NULL);
			return;
		}
		if(!OPEN_MASTER_FILES(MASTER))
		{
			doexit(NULL);
			return;
		}
		baccnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINBACC",1);
		eaccnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINEACC",1);
		raccnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINRACC",1);
		byrnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINBYR",1);
		eyrnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINEYR",1);
		ryrnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINRYR",1);
		MASTER->passkey=READPassKey(filenum);
		MASTER->inheritrsrcs=TRUE;
		switch(MASTER->start_with)
		{
			default:
			case 0:
				finjenm(1,NULL);
				break;
			case 1:
				browse_finjen(NULL,NULL);
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
		if(finsetup!=NULL) free_finmgt(finsetup);
		ShutdownSubsystems();
	}
	if(mbl_finjen!=NULL) mbl_finjen=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func))
	{
		if(!run_func)
		{
			finjenm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MASTER);
		}
	} else {
		finjenm(0,targetkey);
	}
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	finjenm(2,NULL);
} 
