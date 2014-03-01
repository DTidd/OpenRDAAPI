/* miscmstr.c -  MISCMSTR Browse */
/*lint -library */
#ifndef WIN32
#define __NAM__ "miscmstr.lnx"
#endif
#ifdef WIN32
#define __NAM__ "miscmstr.exe"
#endif
#include <app.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <mix.h>
/*CPP_OPART miscmstm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
char editable=TRUE;
MaintainMaster *MASTER=NULL;
extern MakeBrowseList *mbl_utlmstr;
extern void mtnutlmstr(short,void *);
void browse_utl(RDArsrc *,RDATData *);

void shutdownit()
{
	ShutdownSubsystems();
}
void browse_utl(RDArsrc *parent,RDATData *previous)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(previous!=NULL) FreeRDATData(previous);
	if(parent!=NULL) free_rsrc(parent);
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
	mbl_utlmstr=ADVPbrowse(MASTER->passkey->fileno,MASTER->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,
		MASTER->passkey);
}
void main(int argc,char **argv)
{
	short filenum=0;
	char *warnmessage=NULL;

	if(InitializeSubsystems(argc,argv,"MISCBILL","MTN MISCMSTR"))
	{
		RDAAPPMAINLOOP();
		return;
	}
	MASTER=MaintainMasterNew("MISCBILL","MTN MISCMSTR");
	if(findMaintainMaster(MASTER))
	{
		warnmessage=Rmalloc(300+RDAstrlen("MISCBILL")+RDAstrlen("MTN MISCMSTR"));
		sprintf(warnmessage,"Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN MISCMSTR","MISCBILL");
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.",warnmessage,NULL,TRUE); 
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
		MASTER->passkey=READPassKey(filenum);
		MASTER->inheritrsrcs=FALSE;
		switch(MASTER->start_with)
		{
			default:
			case 0:
				mtnutlmstr(1,NULL);
				break;
			case 1:
				browse_utl(NULL,NULL);
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
		if(MASTER!=NULL) 
		{
			CLOSE_MASTER_FILES(MASTER);
			FreeMaintainMaster(MASTER);
		}
		ShutdownSubsystems();
	}
	if(mbl_utlmstr!=NULL) mbl_utlmstr=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func) 
		{
			mtnutlmstr(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MASTER);
		}
	} else {
		mtnutlmstr(0,targetkey);
	}
}
static void new_record(RDArsrc *mtnrsrc,MakeBrowseList *blist)
{
	mtnutlmstr(2,NULL);
} 
