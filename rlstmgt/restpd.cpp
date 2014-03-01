/* restpd.c - Real Estate Payment Master Program*/
/*lint -library */
#ifndef WIN32
#define __NAM__ "restpd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "restpd.exe"
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
#include <fin.h>
/*CPP_OPART restpdm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

extern void restpdm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short byrnum=(-1),ryrnum=(-1),rstrn=(-1);
short rstmst=(-1),taxdwr=(-1);
char editable=TRUE;
MakeBrowseList *mbl_restpd=NULL;
char *module="RLSTMGT";
void browse_vch(RDArsrc *,RDATData *);
MaintainMaster *MASTER=NULL;

void browse_vch(RDArsrc *mtnrsrc,RDATData *previous)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	if(previous!=NULL) FreeRDATData(previous);
	mainrsrc=RDArsrcNEW(module,MASTER->browse_screen);
	searchrsrc=RDArsrcNEW(module,MASTER->search_browse);
	definelist=RDArsrcNEW(module,MASTER->define_list);
	ZERNRD(MASTER->passkey->fileno);
	ApplyPassKey(MASTER->passkey);
	GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,MASTER,
		SCRNvirtualSubData,(void *)mainrsrc);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	MakeRunFunction(mainrsrc,MASTER);
	mbl_restpd=ADVPbrowse(MASTER->passkey->fileno,MASTER->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MASTER->passkey);
}
void main(int argc,char **argv)
{
	short filenum=(-1);

	if(InitializeSubsystems(argc,argv,module,"MTN RESTPAID")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	MASTER=MaintainMasterNew(module,"MTN RESTPAID");
	if(findMaintainMaster(MASTER))
	{
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE); 
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN RESTPAID",module);
		doexit(NULL);
	} else {
		if((filenum=APPOPNNRDEDITABLE(MASTER->module,MASTER->mainfile,
			&editable))==(-1))
		{
			doexit(NULL);
			return;
		}
		SetSupportingWritable(MASTER,"FINMGT","FINBYR",1,TRUE,&editable);
		SetSupportingWritable(MASTER,"FINMGT","FINRYR",1,TRUE,&editable);
		SetSupportingWritable(MASTER,module,"RESTMSTR",1,TRUE,&editable);
		SetSupportingWritable(MASTER,module,"RESTRAN",1,TRUE,&editable);
		SetSupportingWritable(MASTER,"TAXCLT","TAXDRWR",1,TRUE,&editable);
		if(!OPEN_MASTER_FILES(MASTER))
		{
			doexit(NULL);
			return;
		}
		rstmst=APPReturnSupportingFileno(MASTER,module,"RESTMSTR",1);
		rstrn=APPReturnSupportingFileno(MASTER,module,"RESTRAN",1);
		byrnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINBYR",1);
		ryrnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINRYR",1);
		taxdwr=APPReturnSupportingFileno(MASTER,"TAXCLT","TAXDRWR",1);
		MASTER->passkey=READPassKey(filenum);
		MASTER->inheritrsrcs=FALSE;
		switch(MASTER->start_with)
		{
			default:
			case 0:
				restpdm(1,NULL);
				break;
			case 1:
				browse_vch(NULL,NULL);
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
	if(mbl_restpd!=NULL) mbl_restpd=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func) restpdm(0,targetkey);
		else ExecuteListFunc(run_func-1,targetkey,MASTER);
	} else restpdm(0,targetkey);
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	restpdm(2,NULL);
} 
