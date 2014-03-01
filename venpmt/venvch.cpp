/*lint -library */
#ifndef WIN32
#define __NAM__ "venvch.lnx"
#endif
#ifdef WIN32
#define __NAM__ "venvch.exe"
#endif
#include <app.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <mix.h>
#include <fin.h>
#include <venpmt.h>
/*CPP_OPART venvchm */
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
	APPlib *clearDEFtypes;
};
typedef struct mtnstructs mtnstruct;

extern void venvchm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short finbacc=(-1),finracc=(-1),fineacc=(-1);
short finbyr=(-1),finryr=(-1),fineyr=(-1);
RDAfinmgt *finsetup=NULL;
RDAvenpmt *venpmt=NULL;
char editable=TRUE;
MakeBrowseList *mbl_venvch=NULL;
void browse_venvch(RDArsrc *,mtnstruct *);
MaintainMaster *MASTER=NULL;

void browse_venvch(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	if(mtn!=NULL)
	{
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->ACCTtypes!=NULL) freeapplib(mtn->ACCTtypes);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		if(mtn->clearDEFtypes!=NULL) freeapplib(mtn->clearDEFtypes);
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
	mbl_venvch=ADVPbrowse(MASTER->passkey->fileno,MASTER->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MASTER->passkey);
}
void main(int argc,char **argv)
{
	short filenum=(-1);
	char *warnmessage=NULL;
	char *module="VENPMT",*mtnname="MTN VENVCH";

	if(InitializeSubsystems(argc,argv,module,mtnname))
	{
		RDAAPPMAINLOOP();
		return;
	}
	MASTER=MaintainMasterNew(module,mtnname);
	finsetup=RDAfinmgtNEW();
	if(getfinmgtbin(finsetup)==(-1))
	{
		ERRORDIALOG("FINANCIAL MANAGEMENT SETUP NOT AVAILABLE!","The Financial Management Setup File cannot opened.\nEither it doesn't exist or it is incorrect in format.  Please have it checked to make sure that it has been created.  If so please call your installer.",NULL,TRUE);
		ShutdownSecurity();
		RDAAPPMAINLOOP();
		return;
	}
	venpmt=RDAvenpmtNEW();
	if(getvenpmtbin(venpmt)==(-1))
	{	
		ERRORDIALOG("VENPMT MANAGEMENT SETUP NOT AVAILABLE!","The Venpmt Management Setup File cannot opened.\nEither it doesn't exist or it is incorrect in format.  Please have it checked to make sure that it has been created.  If so please call your installer.",NULL,TRUE);
		ShutdownSecurity();
		RDAAPPMAINLOOP();
		return;
	}
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
		SetSupportingWritable(MASTER,"FINMGT","FINEYR",1,TRUE,&editable);
		SetSupportingWritable(MASTER,"FINMGT","FINRYR",1,TRUE,&editable);
		SetSupportingWritable(MASTER,"FINMGT","FINBYR",1,TRUE,&editable);
		if(!OPEN_MASTER_FILES(MASTER))
		{
			doexit(NULL);
			return;
		}
		fineyr=APPReturnSupportingFileno(MASTER,"FINMGT","FINEYR",1);
		finryr=APPReturnSupportingFileno(MASTER,"FINMGT","FINRYR",1);
		finbyr=APPReturnSupportingFileno(MASTER,"FINMGT","FINBYR",1);
		fineacc=APPReturnSupportingFileno(MASTER,"FINMGT","FINEACC",1);
		finracc=APPReturnSupportingFileno(MASTER,"FINMGT","FINRACC",1);
		finbacc=APPReturnSupportingFileno(MASTER,"FINMGT","FINBACC",1);
		MASTER->passkey=READPassKey(filenum);
		MASTER->inheritrsrcs=TRUE;
		switch(MASTER->start_with)
		{
			default:
			case 0:
				venvchm(1,NULL);
				break;
			case 1:
				browse_venvch(NULL,NULL);
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
		if(finsetup!=NULL) free_finmgt(finsetup);
		if(venpmt!=NULL) free_venpmt(venpmt);
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
		ShutdownSubsystems();
	}
	if(mbl_venvch!=NULL) mbl_venvch=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func)
		{
			venvchm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MASTER);
		}
	} else {
		venvchm(0,targetkey);
	}
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	venvchm(2,NULL);
} 
