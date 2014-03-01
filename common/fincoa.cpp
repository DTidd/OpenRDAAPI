/* fincoa.c - Chart of Accounts Browse Program */
/*lint -library */
#ifndef WIN32
#define __NAM__ "fincoa.lnx"
#endif
#ifdef WIN32
#define __NAM__ "fincoa.exe"
#endif
#include <app.hpp>


#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp> 
#include <sec.hpp>
#include <mix.hpp>
/*CPP_OPART fincoam */
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
	APPlib *DIMension;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern char editable;
extern void fincoam(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
MakeBrowseList *mbl_fincoa=NULL;
void browse_fincoa(RDArsrc *,mtnstruct *);
MaintainMaster *MTNMASTER=NULL;

void browse_fincoa(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtn!=NULL)
	{
		if(mtn->ACCTtypes!=NULL) freeapplib(mtn->ACCTtypes);
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->DIMension!=NULL) freeapplib(mtn->DIMension);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc); 
	mainrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->browse_screen);
	searchrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->search_browse);
	definelist=RDArsrcNEW(MTNMASTER->module,MTNMASTER->define_list);
	ZERNRD(MTNMASTER->passkey->fileno);
	ApplyPassKey(MTNMASTER->passkey);
	GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,MTNMASTER,SCRNvirtualSubData,(void *)mainrsrc);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	MakeRunFunction(mainrsrc,MTNMASTER);
	mbl_fincoa=ADVPbrowse(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,mainrsrc,
		searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MTNMASTER->passkey);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	short filenum=(-1);
	char *warnmessage=NULL;
	char *module=NULL,*mtnname=NULL;

	module="FINMGT";
	mtnname="MTN FINCOA";
	if(InitializeSubsystems(argc,argv,module,mtnname))
	{
		RDAAPPMAINLOOP();
		return;
	}
	MTNMASTER=MaintainMasterNew(module,mtnname);
	if(findMaintainMaster(MTNMASTER))
	{
		warnmessage=Rmalloc(300+RDAstrlen(MTNMASTER->module)+RDAstrlen(mtnname));
		sprintf(warnmessage,"Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.",mtnname,MTNMASTER->module);
		ERRORDIALOG("MAINTAIN MTNMASTER NOT FOUND.",warnmessage,NULL,TRUE);
		prterr("%s",warnmessage);
		if(warnmessage!=NULL) Rfree(warnmessage);
		doexit(NULL);
	} else {
		if((filenum=APPOPNNRDEDITABLE(MTNMASTER->module,MTNMASTER->mainfile,&editable))==(-1))
		{
			doexit(NULL);
			return;
		}
		if(!OPEN_MASTER_FILES(MTNMASTER))
		{
			doexit(NULL);
			return;
		}
		MTNMASTER->passkey=READPassKey(filenum);
		MTNMASTER->inheritrsrcs=TRUE;
		switch(MTNMASTER->start_with)
		{
			default:
			case 0:
				fincoam(1,NULL);
				break;
			case 1:
				browse_fincoa(NULL,NULL);
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
		CLOSE_MASTER_FILES(MTNMASTER);
		if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
		ShutdownSubsystems();
	}
	if(mbl_fincoa!=NULL) mbl_fincoa=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func))
	{
		if(!run_func)
		{
			fincoam(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MTNMASTER);
		}
	} else {
		fincoam(0,targetkey);
	}
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	fincoam(2,NULL);
} 
