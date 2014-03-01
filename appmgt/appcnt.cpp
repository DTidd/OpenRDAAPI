/* appcnt.c Applicant Master Program */
/*lint -library */
#ifndef WIN32
#define __NAM__ "appcnt.lnx"
#endif
#ifdef WIN32
#define __NAM__ "appcnt.exe"
#endif
#include <app.hpp>



#include <cstdlib>
#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
/*CPP_OPART appcntm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

extern void mtndmg(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
char editable=TRUE;
MakeBrowseList *mbl_dmg=NULL;
void browse_dmg(RDArsrc *,RDATData *);
MaintainMaster *MASTER=NULL;

void browse_dmg(RDArsrc *mtnrsrc,RDATData *previous)
{
	RDArsrc *searchrsrc=NULL,*definelist=NULL;
	RDArsrc *mainrsrc=NULL;

	if(previous!=NULL) FreeRDATData(previous);
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
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
	mbl_dmg=ADVPbrowse(MASTER->passkey->fileno,MASTER->passkey->keyno,mainrsrc,
		searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MASTER->passkey);
}
int main(int argc,char **argv)
{
	short filenum=(-1);
	char *warnmessage=NULL,*module=NULL,*mtnname=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	module="APPMGT";
	mtnname="MTN APPCNT";
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
		if((filenum=APPOPNNRDEDITABLE(MASTER->module,"APPCNT",&editable))==(-1))
		{
			doexit(NULL);
			return;
		}
		if(!OPEN_MASTER_FILES(MASTER))
		{
			doexit(NULL);
			return;
		}
		MASTER->passkey=READPassKey(filenum);
		MASTER->inheritrsrcs=TRUE;
		switch(MASTER->start_with)
		{
			default:
			case 0:
				mtndmg(1,NULL);
				break;
			case 1:
				browse_dmg(NULL,NULL);
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
		exit(0);
	}
	if(mbl_dmg!=NULL) mbl_dmg=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func)
		{
			mtndmg(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MASTER);
		}
	} else {
		mtndmg(0,targetkey);
	}
}
static void new_record(RDArsrc *parent,MakeBrowseList *b)
{
	mtndmg(2,NULL);
} 
