/* reqmst.c - Requisition Management's Requisition Master Program*/
/*lint -library */
#ifndef WIN32
#define __NAM__ "reqmst.lnx"
#endif
#ifdef WIN32
#define __NAM__ "reqmst.exe"
#endif
#include <app.hpp>


#include <mix.hpp>
#include <fin.hpp>
#include <reqmgt.hpp>
/*CPP_OPART reqfunc */
/*CPP_OPART reqmstm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

extern void reqmstm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short reqloc=(-1),fineyr=(-1),finbyr=(-1);
MakeBrowseList *mbl_reqmst=NULL;
char editable=TRUE;
void browse_reqmst(RDArsrc *parent,RDATData *);
MaintainMaster *MTNMASTER=NULL;

void browse_reqmst(RDArsrc *parent,RDATData *previous)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(parent!=NULL) free_rsrc(parent);
	if(previous!=NULL) FreeRDATData(previous);
	mainrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->browse_screen);
	searchrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->search_browse);
	definelist=RDArsrcNEW(MTNMASTER->module,MTNMASTER->define_list);
	ZERNRD(MTNMASTER->passkey->fileno);
	ApplyPassKey(MTNMASTER->passkey);
	GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,MTNMASTER,SCRNvirtualSubData,(void *)mainrsrc);
        GET_SCREEN_VIRTUALS(mainrsrc,0);
        GET_SCREEN_VIRTUALS(definelist,1);
        GET_SCREEN_VIRTUALS(searchrsrc,2);
        MakeRunFunction(mainrsrc,MTNMASTER);
        mbl_reqmst=ADVPbrowse(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,
                mainrsrc,searchrsrc,definelist,doexit,okfunc,new_record,
                NULL,FALSE,NULL,0,0,2,0,MTNMASTER->passkey);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	short filenum=(-1);
	char *warnmessage=NULL;
	char *module="REQMGT",*mtnname="MTN REQMST";

	if(InitializeSubsystems(argc,argv,module,mtnname))
	{
		RDAAPPMAINLOOP();
		return;
	}
	MTNMASTER=MaintainMasterNew(module,mtnname);
	if(findMaintainMaster(MTNMASTER))
	{
		warnmessage=Rmalloc(300+RDAstrlen(MTNMASTER->module)+RDAstrlen(mtnname));
		sprintf(warnmessage,"Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.",mtnname,MTNMASTER->module);
		ERRORDIALOG("MAINTAIN MTNMASTER NOT FOUND.",warnmessage,NULL,TRUE);
		prterr("%s",warnmessage);
		if(warnmessage!=NULL) Rfree(warnmessage);
		doexit(NULL);
	} else {
		if((filenum=APPOPNNRDEDITABLE(MTNMASTER->module,MTNMASTER->mainfile,
			&editable))==(-1))
		{
			doexit(NULL);
			return;
		}
		SetSupportingWritable(MTNMASTER,"FINMGT","FINBYR",1,TRUE,&editable);
		SetSupportingWritable(MTNMASTER,"FINMGT","FINEYR",1,TRUE,&editable);
		if(!OPEN_MASTER_FILES(MTNMASTER))
		{
			doexit(NULL);
			return;
		}
		reqloc=APPReturnSupportingFileno(MTNMASTER,"REQMGT","REQLOC",1);
		finbyr=APPReturnSupportingFileno(MTNMASTER,"FINMGT","FINBYR",1);
		fineyr=APPReturnSupportingFileno(MTNMASTER,"FINMGT","FINEYR",1);
		MTNMASTER->passkey=READPassKey(filenum);
		MTNMASTER->inheritrsrcs=TRUE;
		switch(MTNMASTER->start_with)
		{
			default:
			case 0:
				reqmstm(1,NULL);
				break;
			case 1:
				browse_reqmst(NULL,NULL);
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
		CLOSE_MASTER_FILES(MTNMASTER);
		if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
		ShutdownSubsystems();
	}
	if(mbl_reqmst!=NULL) mbl_reqmst=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func)
		{
			reqmstm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MTNMASTER);
		}
	} else {
		reqmstm(0,targetkey);
	}
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	reqmstm(2,NULL);
} 
