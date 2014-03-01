/* restran.c - Real Estate Transaction Master Program*/
/*lint -library */
#ifndef WIN32
#define __NAM__ "restran.lnx"
#endif
#ifdef WIN32
#define __NAM__ "restran.exe"
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
/*CPP_OPART restranm */
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
	APPlib *trantypes;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern void restranm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short baccnum=0,raccnum=0,byrnum=0,ryrnum=0;
short rstmst=0,rstcm=0,rstcp=0,rstci=0;
char editable=TRUE;
MakeBrowseList *mbl_restran=NULL;
char *module="RLSTMGT";
void browse_vch(RDArsrc *,mtnstruct *);
MaintainMaster *MASTER=NULL;

void browse_vch(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	if(mtn!=NULL)
	{
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->trantypes!=NULL) freeapplib(mtn->trantypes);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
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
	mbl_restran=ADVPbrowse(MASTER->passkey->fileno,MASTER->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,new_record,NULL,
		FALSE,NULL,0,0,2,0,MASTER->passkey);
}
void main(int argc,char **argv)
{
	short filenum=(-1);

	if(InitializeSubsystems(argc,argv,module,"MTN RESTRAN")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	MASTER=MaintainMasterNew(module,"MTN RESTRAN");
	if(findMaintainMaster(MASTER))
	{
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE); 
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN PPTRAN",module);
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
		if(!OPEN_MASTER_FILES(MASTER))
		{
			doexit(NULL);
			return;
		}
		rstmst=APPReturnSupportingFileno(MASTER,module,"RESTMSTR",1);
		rstcm=APPReturnSupportingFileno(MASTER,module,"RESTCLML",1);
		rstcp=APPReturnSupportingFileno(MASTER,module,"RESTPEN",1);
		rstci=APPReturnSupportingFileno(MASTER,module,"RESTINT",1);
		byrnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINBYR",1);
		ryrnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINRYR",1);
		baccnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINBACC",1);
		raccnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINRACC",1);
		MASTER->passkey=READPassKey(filenum);
		MASTER->inheritrsrcs=FALSE;
		switch(MASTER->start_with)
		{
			default:
			case 0:
				restranm(1,NULL);
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
	if(mbl_restran!=NULL) mbl_restran=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func) restranm(0,targetkey);
		else ExecuteListFunc(run_func-1,targetkey,MASTER);
	} else restranm(0,targetkey);
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	restranm(2,NULL);
} 
