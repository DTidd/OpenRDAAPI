/* posmst.c - Position Master Browse */
/*lint -library */
#ifndef WIN32
#define __NAM__ "posmst.lnx"
#endif
#ifdef WIN32
#define __NAM__ "posmst.exe"
#endif
#include <app.h>
#include <mix.h>
/*CPP_OPART posmstm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

extern MakeBrowseList *mbl_posmst;
extern void posmstm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
char editable=TRUE;
MaintainMaster *MASTER=NULL;
void browse_posmst(RDArsrc *,RDATData *);

void browse_posmst(RDArsrc *mtnrsrc,RDATData *previous)
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
	mbl_posmst=ADVPbrowse(MASTER->passkey->fileno,MASTER->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MASTER->passkey);
}
void main(int argc,char **argv)
{
	short filenum=(-1);
	char *warnmessage=NULL;

	if(InitializeSubsystems(argc,argv,"POSTRK","MTN POSMSTR")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	MASTER=MaintainMasterNew("POSTRK","MTN POSMSTR");
	if(findMaintainMaster(MASTER))
	{
		warnmessage=Rmalloc(300+RDAstrlen("POSTRK")+RDAstrlen("MTN POSMSTR"));
		sprintf(warnmessage,"Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN PPTMSTR","PPTMGT");
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.",warnmessage,NULL,TRUE);
		prterr("%s",warnmessage);
		if(warnmessage!=NULL) Rfree(warnmessage);
		doexit(NULL);
	} else {
		if((filenum=APPOPNNRDEDITABLE(MASTER->module,"POSMSTR",&editable))==(-1))
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
				posmstm(1,NULL);
				break;
			case 1:
				browse_posmst(NULL,NULL);
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
	if(mbl_posmst!=NULL) mbl_posmst=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func)
		{
			posmstm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MASTER);
		}
	} else {
		posmstm(0,targetkey);
	}
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	posmstm(2,NULL);
} 
