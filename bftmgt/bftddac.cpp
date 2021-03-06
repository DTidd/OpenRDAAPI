/* bftddac.c - Xpert Benefit Personnel Definition Type Master Program*/
/*lint -library */
#ifndef WIN32
#define __NAM__ "bftddac.lnx"
#endif
#ifdef WIN32
#define __NAM__ "bftddac.exe"
#endif
#include <app.hpp>


#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
/*CPP_OPART bftddacm */
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

extern void bftddacm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
char editable=TRUE;
MakeBrowseList *mbl_bftddac=NULL;
MaintainMaster *MTNMASTER=NULL;
void browse_bftddac(RDArsrc *,mtnstruct *);

void browse_bftddac(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtn!=NULL)
	{
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
	}
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	mainrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->browse_screen);
	searchrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->search_browse);
	definelist=RDArsrcNEW(MTNMASTER->module,MTNMASTER->define_list);
	ZERNRD(MTNMASTER->passkey->fileno);
	ApplyPassKey(MTNMASTER->passkey);
	GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,MTNMASTER,
		SCRNvirtualSubData,(void *)mainrsrc);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	MakeRunFunction(mainrsrc,MTNMASTER);
	mbl_bftddac=ADVPbrowse(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,mainrsrc,
		searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MTNMASTER->passkey);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	short filenum=(-1);

	if(InitializeSubsystems(argc,argv,"BFTMGT","MTN BFTDDAC")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	MTNMASTER=MaintainMasterNew("BFTMGT","MTN BFTDDAC");
	if(findMaintainMaster(MTNMASTER))
	{
		ERRORDIALOG("MAINTAIN MTNMASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE);
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN BFTDDAC",MTNMASTER->module);
		doexit(NULL);
	} else {
		if((filenum=APPOPNNRDEDITABLE(MTNMASTER->module,"BFTDDAC",&editable))==(-1))
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
		MTNMASTER->inheritrsrcs=FALSE;
		switch(MTNMASTER->start_with)
		{
			default:
			case 0:
				bftddacm(1,NULL);
				break;
			case 1:
				browse_bftddac(NULL,NULL);
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
	if(mbl_bftddac!=NULL) mbl_bftddac=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func))
	{
		if(!run_func)
		{
			bftddacm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MTNMASTER);
		}
	} else {
		bftddacm(0,targetkey);
	}
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	bftddacm(2,NULL);
} 
