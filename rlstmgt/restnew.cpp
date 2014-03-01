/* restnew.c -  RESTNEW Browse */
/*lint -library */
#ifndef WIN32
#define __NAM__ "restnew.lnx"
#endif
#ifdef WIN32
#define __NAM__ "restnew.exe"
#endif
#include <app.hpp>


#include <mix.hpp>
/*CPP_OPART restnewm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
char editable=TRUE;
MaintainMaster *MTNMASTER=NULL;
extern MakeBrowseList *mbl_utlmstr;
extern void mtnutlmstr(short,void *);
void browse_utl(RDArsrc *,RDATData *);

void shutdownit()
{
	ShutdownSubsystems();
}
void search_utl()
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

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
	mbl_utlmstr=START_ADV2Pbrowse(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,
		MTNMASTER->passkey,MTNMASTER->start_where,2);
}
void browse_utl(RDArsrc *parent,RDATData *previous)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(previous!=NULL) FreeRDATData(previous);
	if(parent!=NULL) free_rsrc(parent);
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
	mbl_utlmstr=ADVPbrowse(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,
		MTNMASTER->passkey);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	short filenum=0;
	char *warnmessage=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,"RLSTMGT","MTN RESTNEW"))
	{
		RDAAPPMAINLOOP();
		return;
	}
	MTNMASTER=MaintainMasterNew("RLSTMGT","MTN RESTNEW");
	if(findMaintainMaster(MTNMASTER))
	{
		warnmessage=Rmalloc(300+RDAstrlen("RLSTMGT")+RDAstrlen("MTN RLSTNEW"));
		sprintf(warnmessage,"Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN RLSTNEW","RLSTMGT");
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
		if(!OPEN_MASTER_FILES(MTNMASTER))
		{
			doexit(NULL);
			return;
		}
		MTNMASTER->passkey=READPassKey(filenum);
		MTNMASTER->inheritrsrcs=TRUE;
		switch(MTNMASTER->start_with)
		{
			default:
			case 0:
				mtnutlmstr(1,NULL);
				break;
			case 1:
				browse_utl(NULL,NULL);
				break;
			case 2:
				search_utl();
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
		if(MTNMASTER!=NULL) 
		{
			CLOSE_MASTER_FILES(MTNMASTER);
			FreeMaintainMaster(MTNMASTER);
		}
		ShutdownSubsystems();
	}
	if(mbl_utlmstr!=NULL) mbl_utlmstr=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func) 
		{
			mtnutlmstr(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MTNMASTER);
		}
	} else {
		mtnutlmstr(0,targetkey);
	}
}
static void new_record(RDArsrc *mtnrsrc,MakeBrowseList *blist)
{
	mtnutlmstr(2,NULL);
} 
