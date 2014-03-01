/* paydtrn.c - Xpert Payroll Deduction Pay Transactions Program*/
/*lint -library */
#ifndef WIN32
#define __NAM__ "paydtrn.lnx"
#endif
#ifdef WIN32
#define __NAM__ "paydtrn.exe"
#endif
#include <app.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <fin.hpp>
/*CPP_OPART paydtrnm */
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

extern void paydtrnm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short paynum=(-1),jtrnnum=(-1);
short baccnum=(-1),eaccnum=(-1);
short byrnum=(-1),eyrnum=(-1);
char editable=TRUE;
MakeBrowseList *mbl_paydtrn=NULL;
MaintainMaster *MASTER=NULL;
void browse_paydtrn(RDArsrc *,mtnstruct *);

void browse_paydtrn(RDArsrc *mtnrsrc,mtnstruct *mtn)
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
	if(jtrnnum!=(-1))
	{
		FINDFLDGETSHORT(jtrnnum,"DEFINITION TYPE",&deftype);
		acct=EXPENDITURE_ACCOUNT->codetype;
		if(deftype<=EXPENDITURE_ACCOUNT->num) acct+=deftype;
		defstr=acct->name;
		addstdrsrc(mainrsrc,"DEFINITION STRING",VARIABLETEXT,RDAstrlen(defstr),defstr,FALSE);
	}
	MakeRunFunction(mainrsrc,MASTER);
	mbl_paydtrn=ADVPbrowse(MASTER->passkey->fileno,MASTER->passkey->keyno,mainrsrc,
		searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MASTER->passkey);
}
int main(int argc,char **argv)
{
	short filenum=(-1);

	if(InitializeSubsystems(argc,argv,"PAYROLL","MTN PAYDTRN")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	MASTER=MaintainMasterNew("PAYROLL","MTN PAYDTRN");
	if(findMaintainMaster(MASTER))
	{
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE);
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN PAYDTRN",MASTER->module);
		doexit(NULL);
	} else {
		if((filenum=APPOPNNRDEDITABLE("PAYROLL","PAYDTRN",&editable))==(-1))
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
		jtrnnum=APPReturnSupportingFileno(MASTER,MASTER->module,"PAYJTRN",1);
		MASTER->passkey=READPassKey(filenum);
		MASTER->inheritrsrcs=FALSE;
		switch(MASTER->start_with)
		{
			default:
			case 0:
				paydtrnm(1,NULL);
				break;
			case 1:
				browse_paydtrn(NULL,NULL);
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
	if(mbl_paydtrn!=NULL) mbl_paydtrn=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func))
	{
		if(!run_func)
		{
			paydtrnm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MASTER);
		}
	} else {
		paydtrnm(0,targetkey);
	}
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	paydtrnm(2,NULL);
} 
