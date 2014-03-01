/* reqloc.c - Requisition Management's Location Master Program*/
/*lint -library */
#ifndef WIN32
#define __NAM__ "reqloc.lnx"
#endif
#ifdef WIN32
#define __NAM__ "reqloc.exe"
#endif
#include <app.hpp>


#include <mix.hpp>
#include <fin.hpp>
#include <reqmgt.hpp>
/*CPP_OPART reqfunc */
/*CPP_OPART reqlocm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

extern void reqlocm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short fineyr=(-1),finbyr=(-1);
MakeBrowseList *mbl_reqloc=NULL;
char editable=TRUE;
void browse_reqloc(RDArsrc *parent,RDATData *);
MaintainMaster *MTNMSTR=NULL;

void browse_reqloc(RDArsrc *parent,RDATData *previous)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(parent!=NULL) free_rsrc(parent);
	if(previous!=NULL) FreeRDATData(previous);
	mainrsrc=RDArsrcNEW(MTNMSTR->module,MTNMSTR->browse_screen);
	searchrsrc=RDArsrcNEW(MTNMSTR->module,MTNMSTR->search_browse);
	definelist=RDArsrcNEW(MTNMSTR->module,MTNMSTR->define_list);
	ZERNRD(MTNMSTR->passkey->fileno);
	ApplyPassKey(MTNMSTR->passkey);
	GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,MTNMSTR,SCRNvirtualSubData,(void *)mainrsrc);
        GET_SCREEN_VIRTUALS(mainrsrc,0);
        GET_SCREEN_VIRTUALS(definelist,1);
        GET_SCREEN_VIRTUALS(searchrsrc,2);
        MakeRunFunction(mainrsrc,MTNMSTR);
        mbl_reqloc=ADVPbrowse(MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,
                mainrsrc,searchrsrc,definelist,doexit,okfunc,new_record,
                NULL,FALSE,NULL,0,0,2,0,MTNMSTR->passkey);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	short filenum=(-1);
	char *warnmessage=NULL;
	char *module="REQMGT",*mtnname="MTN REQLOC";

	if(InitializeSubsystems(argc,argv,module,mtnname))
	{
		RDAAPPMAINLOOP();
		return;
	}
	MTNMSTR=MaintainMasterNew(module,mtnname);
	if(findMaintainMaster(MTNMSTR))
	{
		warnmessage=Rmalloc(300+RDAstrlen(MTNMSTR->module)+RDAstrlen(mtnname));
		sprintf(warnmessage,"Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.",mtnname,MTNMSTR->module);
		ERRORDIALOG("MAINTAIN MTNMSTR NOT FOUND.",warnmessage,NULL,TRUE);
		prterr("%s",warnmessage);
		if(warnmessage!=NULL) Rfree(warnmessage);
		doexit(NULL);
	} else {
		if((filenum=APPOPNNRDEDITABLE(MTNMSTR->module,MTNMSTR->mainfile,
			&editable))==(-1))
		{
			doexit(NULL);
			return;
		}
		SetSupportingWritable(MTNMSTR,"FINMGT","FINBYR",1,TRUE,&editable);
		SetSupportingWritable(MTNMSTR,"FINMGT","FINEYR",1,TRUE,&editable);
		if(!OPEN_MASTER_FILES(MTNMSTR))
		{
			doexit(NULL);
			return;
		}
		finbyr=APPReturnSupportingFileno(MTNMSTR,"FINMGT","FINBYR",1);
		fineyr=APPReturnSupportingFileno(MTNMSTR,"FINMGT","FINEYR",1);
		MTNMSTR->passkey=READPassKey(filenum);
		MTNMSTR->inheritrsrcs=TRUE;
		switch(MTNMSTR->start_with)
		{
			default:
			case 0:
				reqlocm(1,NULL);
				break;
			case 1:
				browse_reqloc(NULL,NULL);
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
		CLOSE_MASTER_FILES(MTNMSTR);
		if(MTNMSTR!=NULL) FreeMaintainMaster(MTNMSTR);
		ShutdownSubsystems();
	}
	if(mbl_reqloc!=NULL) mbl_reqloc=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func)
		{
			reqlocm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MTNMSTR);
		}
	} else {
		reqlocm(0,targetkey);
	}
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	reqlocm(2,NULL);
} 
