/* finbact.c - Balance Sheet Account Activity Browse Program */
/*lint -library */
#ifndef WIN32
#define __NAM__ "finbact.lnx"
#endif
#ifdef WIN32
#define __NAM__ "finbact.exe"
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
/*CPP_OPART finbactm */
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

extern void finbactm(short,void *);
extern char editable;
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short byrnum=(-1);
MakeBrowseList *mbl_finbact=NULL;
void browse_finbact(RDArsrc *,mtnstruct *);
MaintainMaster *MASTER=NULL;

void browse_finbact(RDArsrc *mtnrsrc,mtnstruct *mtn)
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
		acct=BALANCE_SHEET_ACCOUNT->codetype;
		if(deftype<=BALANCE_SHEET_ACCOUNT->num)
			acct+=deftype;
		defstr=acct->name;
		addstdrsrc(mainrsrc,"DEFINITION STRING",VARIABLETEXT,RDAstrlen(defstr),defstr,FALSE);
	}
	MakeRunFunction(mainrsrc,MASTER);
	mbl_finbact=ADVPbrowse(MASTER->passkey->fileno,MASTER->passkey->keyno,mainrsrc,
		searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MASTER->passkey);
}
void main(int argc,char **argv)
{
	short filenum=(-1);
	char *warnmessage=NULL;
	char *module=NULL,*mtnname=NULL;

	module="FINMGT";
	mtnname="MTN FINBACT";
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
		SetSupportingWritable(MASTER,MASTER->module,"FINBYR",1,TRUE,&editable);
		if(!OPEN_MASTER_FILES(MASTER))
		{
			doexit(NULL);
			return;
		}
		byrnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINBYR",1);
		MASTER->passkey=READPassKey(filenum);
		MASTER->inheritrsrcs=TRUE;
		switch(MASTER->start_with)
		{
			default:
			case 0:
				finbactm(1,NULL);
				break;
			case 1:
				browse_finbact(NULL,NULL);
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
	if(mbl_finbact!=NULL) mbl_finbact=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func))
	{
		if(!run_func)
		{
			finbactm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MASTER);
		}
	} else {
		finbactm(0,targetkey);
	}
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	finbactm(2,NULL);
} 
