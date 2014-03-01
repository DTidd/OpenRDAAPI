/* puracc.c - Purchase Order Account Master Browse program */
/*lint -library */
#ifndef WIN32
#define __NAM__ "puracc.lnx"
#endif
#ifdef WIN32
#define __NAM__ "puracc.exe"
#endif
#include <app.hpp>


#include <mix.hpp>
#include <venpmt.hpp>
#include <purord.hpp>
/*CPP_OPART puraccm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

struct mtnstructs
{
	APPlib *ACCTtypes;
	APPlib *DEFtypes;
	RDATData *previous;
	APPlib *clearDEFtypes;
};
typedef struct mtnstructs mtnstruct;

RDApurord *purord=NULL;
RDAvenpmt *venpmt=NULL;
extern void puraccm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short purmst=(-1),purlin=(-1),finbacc=(-1),finracc=(-1),fineacc=(-1);
short finbyr=(-1),finryr=(-1),fineyr=(-1);
char editable=TRUE;
MakeBrowseList *mbl_puracc=NULL;
void browse_puracc(RDArsrc *,mtnstruct *);
MaintainMaster *MASTER=NULL;

void browse_puracc(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	if(mtn!=NULL)
	{
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->ACCTtypes!=NULL) freeapplib(mtn->ACCTtypes);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		if(mtn->clearDEFtypes!=NULL) freeapplib(mtn->clearDEFtypes);
		Rfree(mtn);
	}
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
	mbl_puracc=ADVPbrowse(MASTER->passkey->fileno,MASTER->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MASTER->passkey);
}
int main(int argc,char **argv)
{
	short filenum=(-1);
	char *warnmessage=NULL;
	char *module="PURORD",*mtnname="MTN PURACC";

	if(InitializeSubsystems(argc,argv,module,mtnname))
	{
		RDAAPPMAINLOOP();
		return;
	}
	venpmt=RDAvenpmtNEW();
	purord=RDApurordNEW();
	getpurordbin(purord);
	MASTER=MaintainMasterNew(module,mtnname);
	if(findMaintainMaster(MASTER))
	{
		warnmessage=Rmalloc(300+RDAstrlen(MASTER->module)+RDAstrlen(mtnname));
		sprintf(warnmessage,"Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.",mtnname,MASTER->module);
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND!",warnmessage,NULL,TRUE);
		prterr("%s",warnmessage);
		if(warnmessage!=NULL) Rfree(warnmessage);
		doexit(NULL);
	} else if(getvenpmtbin(venpmt)==(-1))
	{
		ERRORDIALOG("VENPMT SETUP NOT FOUND","Venpmt Setup Variables have not been established...... Please establish and try again...",NULL,TRUE);
		doexit(NULL);
	} else {
		if((filenum=APPOPNNRDEDITABLE(MASTER->module,MASTER->mainfile,
			&editable))==(-1))
		{
			doexit(NULL);
			return;
		}
		SetSupportingWritable(MASTER,"FINMGT","FINEYR",1,TRUE,&editable);
		SetSupportingWritable(MASTER,"FINMGT","FINRYR",1,TRUE,&editable);
		SetSupportingWritable(MASTER,"FINMGT","FINBYR",1,TRUE,&editable);
		if(!OPEN_MASTER_FILES(MASTER))
		{
			doexit(NULL);
			return;
		}
		purmst=APPReturnSupportingFileno(MASTER,"PURORD","PURMST",1);
		purlin=APPReturnSupportingFileno(MASTER,"PURORD","PURLIN",1);
		fineyr=APPReturnSupportingFileno(MASTER,"FINMGT","FINEYR",1);
		finryr=APPReturnSupportingFileno(MASTER,"FINMGT","FINRYR",1);
		finbyr=APPReturnSupportingFileno(MASTER,"FINMGT","FINBYR",1);
		fineacc=APPReturnSupportingFileno(MASTER,"FINMGT","FINEACC",1);
		finracc=APPReturnSupportingFileno(MASTER,"FINMGT","FINRACC",1);
		finbacc=APPReturnSupportingFileno(MASTER,"FINMGT","FINBACC",1);
		MASTER->passkey=READPassKey(filenum);
		MASTER->inheritrsrcs=TRUE;
		switch(MASTER->start_with)
		{
			default:
			case 0:
				puraccm(1,NULL);
				break;
			case 1:
				browse_puracc(NULL,NULL);
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
		if(venpmt!=NULL) free_venpmt(venpmt);
		ShutdownSubsystems();
	}
	if(purord!=NULL) free_purord(purord);
	if(mbl_puracc!=NULL) mbl_puracc=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func)
		{
			puraccm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MASTER);
		}
	} else {
		puraccm(0,targetkey);
	}
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	puraccm(2,NULL);
} 
