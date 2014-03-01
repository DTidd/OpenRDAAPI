/* purtran.c - Purchase Order Transaction Master Program*/
/*lint -library */
#ifndef WIN32
#define __NAM__ "purtran.lnx"
#endif
#ifdef WIN32
#define __NAM__ "purtran.exe"
#endif
#include <app.hpp>


#include <mix.hpp>
#include <venpmt.hpp>
#include <purord.hpp>
/*CPP_OPART purtranm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

RDApurord *purord=NULL;
RDAvenpmt *venpmt=NULL;
extern void purtranm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
char editable=TRUE;
void browse_purtran(RDArsrc *parent,RDATData *);
MakeBrowseList *mbl_purtran=NULL;
MaintainMaster *MASTER=NULL;
short purmst=(-1),purlin=(-1),puracc=(-1);
short finbyr=(-1),fineyr=(-1),finryr=(-1);


void browse_purtran(RDArsrc *parent,RDATData *previous)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(parent!=NULL) free_rsrc(parent);
	if(previous!=NULL) FreeRDATData(previous);
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
	mbl_purtran=ADVPbrowse(MASTER->passkey->fileno,MASTER->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MASTER->passkey);
}
int main(int argc,char **argv)
{
	short filenum=(-1);
	char *warnmessage=NULL;
	char *module="PURORD",*mtnname="MTN PURTRAN";

	if(InitializeSubsystems(argc,argv,module,mtnname))
	{
		RDAAPPMAINLOOP();
		return;
	}
	venpmt=RDAvenpmtNEW();
	if(getvenpmtbin(venpmt)==(-1))
	{
		warnmessage=Rmalloc(350);
		sprintf(warnmessage,"Error Vendor Payment Setup definition not found, this process cannot be executed without it's definition in the [VENPMT.STP] library.");
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.",warnmessage,NULL,TRUE);
		prterr("%s",warnmessage);
		if(warnmessage!=NULL) Rfree(warnmessage);
		doexit(NULL);
	}
	purord=RDApurordNEW();
	getpurordbin(purord);
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
		if((filenum=APPOPNNRDEDITABLE(MASTER->module,MASTER->mainfile,
			&editable))==(-1))
		{
			doexit(NULL);
			return;
		}
		SetSupportingWritable(MASTER,"PURORD","PURMST",1,TRUE,&editable);
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
		puracc=APPReturnSupportingFileno(MASTER,"PURORD","PURACC",1);
		fineyr=APPReturnSupportingFileno(MASTER,"FINMGT","FINEYR",1);
		finryr=APPReturnSupportingFileno(MASTER,"FINMGT","FINRYR",1);
		finbyr=APPReturnSupportingFileno(MASTER,"FINMGT","FINBYR",1);
		MASTER->passkey=READPassKey(filenum);
		MASTER->inheritrsrcs=TRUE;
		switch(MASTER->start_with)
		{
			default:
			case 0:
				purtranm(1,NULL);
				break;
			case 1:
				browse_purtran(NULL,NULL);
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
		if(purord!=NULL) free_purord(purord);
		ShutdownSubsystems();
	}
	if(mbl_purtran!=NULL) mbl_purtran=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func)
		{
			purtranm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MASTER);
		}
	} else {
		purtranm(0,targetkey);
	}
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	purtranm(2,NULL);
} 
