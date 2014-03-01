/* restcls.c - Real Estate Class Master Program*/
/*lint -library */
#ifndef WIN32
#define __NAM__ "restcls.lnx"
#endif
#ifdef WIN32
#define __NAM__ "restcls.exe"
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
/*CPP_OPART restclsm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

extern MakeBrowseList *mbl_restclas;
extern void restclasm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short filenum=0;
char editable=TRUE;
char *module="RLSTMGT";
void browse_msr(RDArsrc *parent,RDATData *previous);
MaintainMaster *MASTER=NULL;

void browse_msr(RDArsrc *parent,RDATData *previous)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(parent!=NULL) free_rsrc(parent);
	if(previous!=NULL) FreeRDATData(previous);
	mainrsrc=RDArsrcNEW(module,MASTER->browse_screen);
	searchrsrc=RDArsrcNEW(module,MASTER->search_browse);
	definelist=RDArsrcNEW(module,MASTER->define_list);
	ZERNRD(filenum);
	addDFincvir(searchrsrc,module,"RESTCLAS",NULL,filenum); 
	addDFincvir(definelist,module,"RESTCLAS",NULL,filenum); 
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	MakeRunFunction(mainrsrc,MASTER);
	mbl_restclas=ADVPbrowse(filenum,MASTER->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MASTER->passkey);
}
void main(int argc,char **argv)
{
	if(InitializeSubsystems(argc,argv,module,"MTN RESTCLAS")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((filenum=APPOPNNRDEDITABLE(module,"RESTCLAS",&editable))==(-1)) return;
	MASTER=MaintainMasterNew(module,"MTN RESTCLAS");
	if(findMaintainMaster(MASTER))
	{
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE); 
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN RESTCLAS",module);
		doexit(NULL);
	} else {
		MASTER->passkey=READPassKey(filenum);
		switch(MASTER->start_with)
		{
			default:
			case 0:
				restclasm(1,NULL);
				break;
			case 1:
				browse_msr(NULL,NULL);
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
	if(WindowCount==1)
	{
		if(filenum!=(-1)) CLSNRD(filenum);
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
		ShutdownSubsystems();
	}
	if(mbl_restclas!=NULL) mbl_restclas=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func) restclasm(0,targetkey);
			else ExecuteListFunc(run_func-1,targetkey,MASTER);
	} else restclasm(0,targetkey);
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	restclasm(2,NULL);
} 
