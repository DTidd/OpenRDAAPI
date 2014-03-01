/* scnnot.c - Screeen Notes Browse program */
/*lint -library */
#ifndef WIN32
#define __NAM__ "scnnot.lnx"
#endif
#ifdef WIN32
#define __NAM__ "scnnot.exe"
#endif
#include <app.hpp>


#include <mix.hpp>
/*CPP_OPART scnnotm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

extern void mtnscnnot(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short filenum;
char editable=TRUE;
char *module="OLHELP";
MakeBrowseList *mbl_scnnot=NULL;
void browse_scn(RDArsrc *,RDATData *);
MaintainMaster *MASTER=NULL;

void browse_scn(RDArsrc *mtnrsrc,RDATData *previous)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	if(previous!=NULL) FreeRDATData(previous);
	mainrsrc=RDArsrcNEW(module,MASTER->browse_screen);
	searchrsrc=RDArsrcNEW(module,MASTER->search_browse);
	definelist=RDArsrcNEW(module,MASTER->define_list);
	ZERNRD(filenum);
	addDFincvir(definelist,module,"SCNNOT",NULL,filenum);
	addDFincvir(searchrsrc,module,"SCNNOT",NULL,filenum);
	ApplyPassKey(MASTER->passkey);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	MakeRunFunction(mainrsrc,MASTER);
	mbl_scnnot=ADVPbrowse(filenum,MASTER->passkey->keyno,mainrsrc,searchrsrc,
			definelist,doexit,okfunc,new_record,NULL,FALSE,NULL,0,0,2,0,
			MASTER->passkey);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	if(InitializeSubsystems(argc,argv,module,"MTN SCNNOT")) 
	{
		return;
	}
	if((filenum=APPOPNNRDEDITABLE(module,"SCNNOT",&editable))==(-1)) return;
	MASTER=MaintainMasterNew(module,"MTN SCNNOT");
	if(findMaintainMaster(MASTER))
	{
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE);
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN SCNNOT",module);
		doexit(NULL);
	} else {
		MASTER->passkey=READPassKey(filenum);
		switch(MASTER->start_with)
		{
			default:
			case 0:
				mtnscnnot(1,NULL);
				break;
			case 1:
				browse_scn(NULL,NULL);
				break;
		}
	}
}
static void doexit(MakeBrowseList *blist)
{
	if(blist!=NULL)
	{
		if(blist->searchrsrc!=NULL) 
		{
			killwindow(blist->searchrsrc);
			free_rsrc(blist->searchrsrc);
		}
		if(blist->definelist!=NULL) 
		{
			killwindow(blist->definelist);
			free_rsrc(blist->definelist);
		}
		if(blist->mainrsrc!=NULL) free_rsrc(blist->mainrsrc);
	}
	if(WindowCount<2)
	{
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
		CLSNRD(filenum);
		ShutdownSubsystems();
	}
	if(mbl_scnnot!=NULL) mbl_scnnot=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func) mtnscnnot(0,targetkey);
		else ExecuteListFunc(run_func-1,targetkey,MASTER);
	} else mtnscnnot(0,targetkey);
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{ 
	mtnscnnot(2,NULL);
} 
