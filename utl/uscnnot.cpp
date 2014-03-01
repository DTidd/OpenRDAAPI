/* uscnnot.c - User's Screen Notes */
/*lint -library */
#ifndef WIN32
#define __NAM__ "uscnnot.lnx"
#endif
#ifdef WIN32
#define __NAM__ "uscnnot.exe"
#endif
#include <app.hpp>


#include <mix.hpp>
/*CPP_OPART uscnnotm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

extern void mtnuscnnot(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short usernum=0,filenum=0,scnnot=0;
char editable=TRUE;
char *module="OLHELP";
MakeBrowseList *mbl_uscnnot;
void browse_uscnnot(RDArsrc *,RDATData *);
MaintainMaster *MASTER=NULL;

void browse_uscnnot(RDArsrc *mtnrsrc,RDATData *previous)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	if(previous!=NULL) FreeRDATData(previous);
	mainrsrc=RDArsrcNEW(module,MASTER->browse_screen);
	searchrsrc=RDArsrcNEW(module,MASTER->search_browse);
	definelist=RDArsrcNEW(module,MASTER->define_list);
	ZERNRD(filenum);
	addDFincvir(mainrsrc,module,"SCNNOT",NULL,scnnot);
	addDFincvir(definelist,module,"USCNNOT",NULL,filenum);
	addDFincvir(searchrsrc,module,"USCNNOT",NULL,filenum);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	ApplyPassKey(MASTER->passkey);
	if(scnnot!=(-1))
	{
		COPYFIELD(filenum,scnnot,"MODULE NAME");
		COPYFIELD(filenum,scnnot,"SCREEN NAME");
		if(EQLNRDsec(scnnot,1)) 
		{
			ZERNRD(scnnot);
		} else {
			file2rsrc(scnnot,mainrsrc,FALSE);
			filerecord2rsrc(scnnot,mainrsrc);
		}
	}
	MakeRunFunction(mainrsrc,MASTER);
	mbl_uscnnot=ADVPbrowse(filenum,MASTER->passkey->keyno,mainrsrc,searchrsrc,
			definelist,doexit,okfunc,new_record,NULL,FALSE,NULL,0,0,2,0,
			MASTER->passkey);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	if(InitializeSubsystems(argc,argv,module,"MTN USCNNOT")) 
	{
		return;
	}
	if((filenum=APPOPNNRDEDITABLE(module,"USCNNOT",&editable))==(-1)) return;
	if((scnnot=APPOPNNRD(module,"SCNNOT",TRUE,FALSE))==(-1)) return;
	if((usernum=APPOPNNRD("SECURITY","USERS",TRUE,FALSE))==(-1)) return;
	MASTER=MaintainMasterNew(module,"MTN USCNNOT");
	if(findMaintainMaster(MASTER))
	{
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE);
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN USCNNOT",module);
		doexit(NULL);
	} else {
		MASTER->passkey=READPassKey(filenum);
		switch(MASTER->start_with)
		{
			default:
			case 0:
				mtnuscnnot(1,NULL);
				break;
			case 1:
				browse_uscnnot(NULL,NULL);
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
		CLSNRD(scnnot);
		CLSNRD(usernum);
		ShutdownSubsystems();
	}
	if(mbl_uscnnot!=NULL) mbl_uscnnot=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func) mtnuscnnot(0,targetkey);
		else ExecuteListFunc(run_func-1,targetkey,MASTER);
	} else mtnuscnnot(0,targetkey);
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	mtnuscnnot(2,NULL);
} 
