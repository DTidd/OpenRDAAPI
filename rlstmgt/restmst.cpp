/* restmst.c - Xpert Real Estate Master Program*/
/*lint -library */
#ifndef WIN32
#define __NAM__ "restmst.lnx"
#endif
#ifdef WIN32
#define __NAM__ "restmst.exe"
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
#include <rlstmgt.h>
/*CPP_OPART restmstm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

struct mtnstructs
{
	APPlib *books;
	APPlib *class;
	APPlib *districts;
	APPlib *lands;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern MakeBrowseList *mbl_restmst;
extern void restmstm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short filenum=0,distnum=0,classnum=0,landnum=0,deednum=0;
char editable=TRUE;
char *module="RLSTMGT";
void browse_msr(RDArsrc *parent,mtnstruct *);
MaintainMaster *MASTER=NULL;

void browse_msr(RDArsrc *parent,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(parent!=NULL) free_rsrc(parent);
	if(mtn!=NULL)
	{
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		if(mtn->books!=NULL) freeapplib(mtn->books);
		if(mtn->class!=NULL) freeapplib(mtn->class);
		if(mtn->districts!=NULL) freeapplib(mtn->districts);
		if(mtn->lands!=NULL) freeapplib(mtn->lands);
		Rfree(mtn);
	}
	mainrsrc=RDArsrcNEW(module,MASTER->browse_screen);
	searchrsrc=RDArsrcNEW(module,MASTER->search_browse);
	definelist=RDArsrcNEW(module,MASTER->define_list);
	ZERNRD(filenum);
	addDFincvir(searchrsrc,module,"RESTMST",NULL,filenum); 
	addDFincvir(definelist,module,"RESTMST",NULL,filenum); 
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	MakeRunFunction(mainrsrc,MASTER);
	mbl_restmst=ADVPbrowse(filenum,MASTER->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MASTER->passkey);
}
void main(int argc,char **argv)
{
	if(InitializeSubsystems(argc,argv,module,"MTN RESTMST")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((filenum=APPOPNNRDEDITABLE(module,"RESTMST",&editable))==(-1)) return;
	if((landnum=APPOPNNRD(module,"RESTLNDV",TRUE,FALSE))==(-1)) return;
	if((distnum=APPOPNNRD(module,"RESTDIST",TRUE,FALSE))==(-1)) return;
	if((deednum=APPOPNNRD(module,"RESTDEED",TRUE,FALSE))==(-1)) return;
	if((classnum=APPOPNNRD(module,"RESTCLAS",TRUE,FALSE))==(-1)) return;
	MASTER=MaintainMasterNew(module,"MTN RESTMST");
	if(findMaintainMaster(MASTER))
	{
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE); 
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN RESTMST",module);
		doexit(NULL);
	} else {
		MASTER->passkey=READPassKey(filenum);
		switch(MASTER->start_with)
		{
			default:
			case 0:
				restmstm(1,NULL);
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
		if(landnum!=(-1)) CLSNRD(landnum);
		if(distnum!=(-1)) CLSNRD(distnum);
		if(deednum!=(-1)) CLSNRD(deednum);
		if(classnum!=(-1)) CLSNRD(classnum);
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
		ShutdownSubsystems();
	}
	if(mbl_restmst!=NULL) mbl_restmst=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func) restmstm(0,targetkey);
			else ExecuteListFunc(run_func-1,targetkey,MASTER);
	} else restmstm(0,targetkey);
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	restmstm(2,NULL);
} 
