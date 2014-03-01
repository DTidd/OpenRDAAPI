/* misctran.c - Misc Billing Transaction Master Program*/
/*lint -library */
#ifndef WIN32
#define __NAM__ "misctran.lnx"
#endif
#ifdef WIN32
#define __NAM__ "misctran.exe"
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
#include <miscbill.h>
/*CPP_OPART misctrnm */
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
	APPlib *classes;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern void misctranm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short baccnum=(-1),raccnum=(-1),byrnum=(-1),ryrnum=(-1);
short pptmstr=(-1),rstcm=(-1),rstcp=(-1),rstci=(-1);
char editable=TRUE;
MakeBrowseList *mbl_misctran=NULL;
char *module="MISCBILL";
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
		if(mtn->classes!=NULL) freeapplib(mtn->classes);
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
	mbl_misctran=ADVPbrowse(MASTER->passkey->fileno,MASTER->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,new_record,NULL,FALSE,
		NULL,0,0,2,0,MASTER->passkey);
}
void main(int argc,char **argv)
{
	short filenum=(-1);

	if(InitializeSubsystems(argc,argv,module,"MTN MISCTRAN")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	MASTER=MaintainMasterNew(module,"MTN MISCTRAN");
	if(findMaintainMaster(MASTER))
	{
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE); 
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN MISCTRAN",module);
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
		SetSupportingWritable(MASTER,module,"MISCMSTR",1,TRUE,&editable);
		if(!OPEN_MASTER_FILES(MASTER))
		{
			doexit(NULL);
			return;
		}
		pptmstr=APPReturnSupportingFileno(MASTER,module,"MISCMSTR",1);
		rstcm=APPReturnSupportingFileno(MASTER,module,"MISCRATE",1);
		rstcp=APPReturnSupportingFileno(MASTER,module,"MISCPEN",1);
		rstci=APPReturnSupportingFileno(MASTER,module,"MISCINT",1);
		byrnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINBYR",1);
		ryrnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINRYR",1);
		baccnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINBACC",1);
		raccnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINRACC",1);
		MASTER->passkey=READPassKey(filenum);
		MASTER->inheritrsrcs=TRUE;
		switch(MASTER->start_with)
		{
			default:
			case 0:
				misctranm(1,NULL);
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
		exit(0);
	}
	if(mbl_misctran!=NULL) mbl_misctran=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func) misctranm(0,targetkey);
		else ExecuteListFunc(run_func-1,targetkey,MASTER);
	} else misctranm(0,targetkey);
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	misctranm(2,NULL);
} 
