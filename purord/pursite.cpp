/* pursite.c - Purchase Orders Site Master Browse Program */
/*lint -library */
#ifndef WIN32
#define __NAM__ "pursite.lnx"
#endif
#ifdef WIN32
#define __NAM__ "pursite.exe"
#endif
#include <app.hpp>
#include <cstdlib>


#include <mix.hpp>
/*CPP_OPART pursitem */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

extern void pursitem(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short filenum=0;
char editable=TRUE;
char *module="PURORD";
MaintainMaster *MASTER=NULL;
MakeBrowseList *mbl_site=NULL;
void browse_site(RDArsrc *,RDATData *previous);

void browse_site(RDArsrc *mtnrsrc,RDATData *previous)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(previous!=NULL) FreeRDATData(previous);
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	mainrsrc=RDArsrcNEW(module,MASTER->browse_screen);
	searchrsrc=RDArsrcNEW(module,MASTER->search_browse);
	definelist=RDArsrcNEW(module,MASTER->define_list);
	ZERNRD(filenum);
	addDFincvir(searchrsrc,module,"PURSIT",NULL,filenum);
	addDFincvir(definelist,module,"PURSIT",NULL,filenum);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	MakeRunFunction(mainrsrc,MASTER);
	mbl_site=ADVPbrowse(filenum,MASTER->passkey->keyno,mainrsrc,searchrsrc,
		definelist,doexit,okfunc,new_record,NULL,FALSE,NULL,0,0,2,0,
		MASTER->passkey);
}
int main(int argc,char **argv)
{
	if(InitializeSubsystems(argc,argv,module,"MTN PURSIT"))
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((filenum=APPOPNNRDEDITABLE(module,"PURSIT",&editable))==(-1)) return;
	MASTER=MaintainMasterNew(module,"MTN PURSIT");
	if(findMaintainMaster(MASTER))
	{
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE); 
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN PURSIT",module);
		doexit(NULL);
	} else {
		MASTER->passkey=READPassKey(filenum);
		switch(MASTER->start_with)
		{
			default:
			case 0:
				pursitem(1,NULL);
				break;
			case 1:
				browse_site(NULL,NULL);
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
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
		CLSNRD(filenum);
		ShutdownSubsystems();
	}
	mbl_site=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func) 
		{
			pursitem(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MASTER);
		}
	} else pursitem(0,targetkey);
}
static void new_record(RDArsrc *mtnrsrc,MakeBrowseList *blist)
{
	pursitem(2,NULL);
} 
