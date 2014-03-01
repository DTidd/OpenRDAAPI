/* posvac.c - Xpert Position Vacancy Browse */
/*lint -library */
#ifndef WIN32
#define __NAM__ "posvac.lnx"
#endif
#ifdef WIN32
#define __NAM__ "posvac.exe"
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
/*CPP_OPART posvacm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

extern void posvacm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short msrnum=(-1),vacsnum=(-1);
char editable=TRUE;
MakeBrowseList *mbl_posvac=NULL;
MaintainMaster *MASTER=NULL;
void browse_posvac(RDArsrc *,RDATData *);

void browse_posvac(RDArsrc *mtnrsrc,RDATData *previous)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

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
	mbl_posvac=ADVPbrowse(MASTER->passkey->fileno,MASTER->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MASTER->passkey);
}
void main(int argc,char **argv)
{
	short filenum=(-1);
	char *warnmessage=NULL,*module=NULL,*mtnname=NULL;

	module="POSTRK";
	mtnname="MTN POSVAC";
	if(InitializeSubsystems(argc,argv,module,mtnname)) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	MASTER=MaintainMasterNew(module,mtnname);
	if(findMaintainMaster(MASTER))
	{
		warnmessage=Rmalloc(300+RDAstrlen(MASTER->module)+RDAstrlen(mtnname));
		sprintf(warnmessage,"Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN PPTMSTR","PPTMGT");
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.",warnmessage,NULL,TRUE);
		prterr("%s",warnmessage);
		if(warnmessage!=NULL) Rfree(warnmessage);
		doexit(NULL);
	} else {
		if((filenum=APPOPNNRDEDITABLE(MASTER->module,"POSVAC",&editable))==(-1))
		{
			doexit(NULL);
			return;
		}
		SetSupportingWritable(MASTER,MASTER->module,"POSMSTR",1,TRUE,&editable);
		if(!OPEN_MASTER_FILES(MASTER))
		{
			doexit(NULL);
			return;
		}
		msrnum=APPReturnSupportingFileno(MASTER,MASTER->module,"POSMSTR",1);
		vacsnum=APPReturnSupportingFileno(MASTER,MASTER->module,"POSVSTS",1);
		MASTER->passkey=READPassKey(filenum);
		MASTER->inheritrsrcs=TRUE;
		switch(MASTER->start_with)
		{
			default:
			case 0:
				posvacm(1,NULL);
				break;
			case 1:
				browse_posvac(NULL,NULL);
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
	if(mbl_posvac!=NULL) mbl_posvac=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func))
	{
		if(!run_func)
		{
			posvacm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MASTER);
		}
	} else posvacm(0,targetkey);
}
static void new_record(RDArsrc *mtnrsrc,MakeBrowseList *blist)
{
	posvacm(2,NULL);
} 
