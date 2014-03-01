/* payjtrn.c - Xpert Payroll Job Pay Transactions Program*/
/*lint -library */
#ifndef WIN32
#define __NAM__ "payjtrn.lnx"
#endif
#ifdef WIN32
#define __NAM__ "payjtrn.exe"
#endif
#include <app.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <fin.hpp>
/*CPP_OPART payjtrnm */
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
	APPlib *PDEFtypes;
	short deftype;
	short pdeftype;
};
typedef struct mtnstructs mtnstruct;

extern void payjtrnm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short paynum=(-1),jamsnum=(-1);
short baccnum=(-1),eaccnum=(-1);
short byrnum=(-1),eyrnum=(-1);
char editable=TRUE,fineditable=TRUE;
MakeBrowseList *mbl_payjtrn=NULL;
MaintainMaster *MASTER=NULL;
void browse_payjtrn(RDArsrc *,mtnstruct *);

void browse_payjtrn(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;
	RDAacct *acct=NULL;
	char *defstr=NULL;
	short deftype=0;

	if(mtn!=NULL)
	{
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->PDEFtypes!=NULL) freeapplib(mtn->PDEFtypes);
		Rfree(mtn);
	}
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	mainrsrc=RDArsrcNEW(MASTER->module,MASTER->browse_screen);
	searchrsrc=RDArsrcNEW(MASTER->module,MASTER->search_browse);
	definelist=RDArsrcNEW(MASTER->module,MASTER->define_list);
	ZERNRD(MASTER->passkey->fileno);
	ApplyPassKey(MASTER->passkey);
	GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,MASTER,
		SCRNvirtualSubData,(void *)mainrsrc);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	if(jamsnum!=(-1))
	{
		FINDFLDGETSHORT(jamsnum,"DEFINITION TYPE",&deftype);
		acct=EXPENDITURE_ACCOUNT->codetype;
		if(deftype<=EXPENDITURE_ACCOUNT->num) acct+=deftype;
		defstr=acct->name;
		addstdrsrc(mainrsrc,"DEFINITION STRING",VARIABLETEXT,RDAstrlen(defstr),defstr,FALSE);
	}
	MakeRunFunction(mainrsrc,MASTER);
	mbl_payjtrn=ADVPbrowse(MASTER->passkey->fileno,MASTER->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MASTER->passkey);
}
int main(int argc,char **argv)
{
	short filenum=(-1);

	if(InitializeSubsystems(argc,argv,"PAYROLL","MTN PAYJTRN")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if(editable==TRUE)
	{
		editable=fineditable;
	}
	if(editable==TRUE)
	{
		editable=fineditable;
	}
	MASTER=MaintainMasterNew("PAYROLL","MTN PAYJTRN");
	if(findMaintainMaster(MASTER))
	{
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE);
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN PAYJTRN",MASTER->module);
		doexit(NULL);
	} else {
		if((filenum=APPOPNNRDEDITABLE(MASTER->module,"PAYJTRN",&editable))==(-1))
		{
			doexit(NULL);
			return;
		}
		if(!OPEN_MASTER_FILES(MASTER))
		{
			doexit(NULL);
			return;
		}
		baccnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINBACC",1);
		eaccnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINEACC",1);
		byrnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINBYR",1);
		eyrnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINEYR",1);
		paynum=APPReturnSupportingFileno(MASTER,MASTER->module,"PAYMSTR",1);
		jamsnum=APPReturnSupportingFileno(MASTER,MASTER->module,"PAYJAMS",1);
		MASTER->passkey=READPassKey(filenum);
		MASTER->inheritrsrcs=FALSE;
		switch(MASTER->start_with)
		{
			default:
			case 0:
				payjtrnm(1,NULL);
				break;
			case 1:
				browse_payjtrn(NULL,NULL);
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
		exit(0);
	}
	if(mbl_payjtrn!=NULL) mbl_payjtrn=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func))
	{
		if(!run_func)
		{
			payjtrnm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MASTER);
		}
	} else {
		payjtrnm(0,targetkey);
	}
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	payjtrnm(2,NULL);
} 
