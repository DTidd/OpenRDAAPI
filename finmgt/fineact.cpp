/* fineact.c - Expenditure Account Activity Browse Program */
/*lint -library */
#ifndef WIN32
#define __NAM__ "fineact.lnx"
#endif
#ifdef WIN32
#define __NAM__ "fineact.exe"
#endif
#include <app.h>
#include <stdlib.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <alloc.h>
#include <gui.h>
#include <nrd.h>
#include <trans.h> 
#include <sec.h>
#include <mix.h>
/*CPP_OPART fineactm */
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
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern void fineactm(short,void *);
extern char editable;
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short eyrnum=(-1);
MakeBrowseList *mbl_fineact=NULL;
void browse_fineact(RDArsrc *,mtnstruct *);
MaintainMaster *MASTER=NULL;

void browse_fineact(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *defstr=NULL;
	RDAacct *acct=NULL;
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;
	short deftype=(-1);

	if(mtn!=NULL)
	{
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
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
	FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",&deftype);
	if(deftype!=(-1))
	{
		acct=EXPENDITURE_ACCOUNT->codetype;
		if(deftype<=EXPENDITURE_ACCOUNT->num)
			acct+=deftype;
		defstr=acct->name;
		addstdrsrc(mainrsrc,"DEFINITION STRING",VARIABLETEXT,RDAstrlen(defstr),defstr,FALSE);
	}
	MakeRunFunction(mainrsrc,MASTER);
	mbl_fineact=ADVPbrowse(MASTER->passkey->fileno,MASTER->passkey->keyno,mainrsrc,
		searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MASTER->passkey);
}
void main(int argc,char **argv)
{
	short filenum=(-1);
	char *warnmessage=NULL;
	char *module=NULL,*mtnname=NULL;

	module="FINMGT";
	mtnname="MTN FINEACT";
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
		if((filenum=APPOPNNRDEDITABLE(MASTER->module,MASTER->mainfile,&editable))==(-1))
		{
			doexit(NULL);
			return;
		}
		SetSupportingWritable(MASTER,MASTER->module,"FINEYR",1,TRUE,&editable);
		if(!OPEN_MASTER_FILES(MASTER))
		{
			doexit(NULL);
			return;
		}
		eyrnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINEYR",1);
		MASTER->passkey=READPassKey(filenum);
		MASTER->inheritrsrcs=TRUE;
		switch(MASTER->start_with)
		{
			default:
			case 0:
				fineactm(1,NULL);
				break;
			case 1:
				browse_fineact(NULL,NULL);
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
	if(mbl_fineact!=NULL) mbl_fineact=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func))
	{
		if(!run_func)
		{
			fineactm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MASTER);
		}
	} else {
		fineactm(0,targetkey);
	}
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	fineactm(2,NULL);
} 
