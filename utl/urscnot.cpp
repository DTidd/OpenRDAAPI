/* urscnot.c - User's Resource Notes */
/*lint -library */
#ifndef WIN32
#define __NAM__ "urscnot.lnx"
#endif
#ifdef WIN32
#define __NAM__ "urscnot.exe"
#endif
#include <app.hpp>


#include <mix.hpp>
/*CPP_OPART urscnotm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

extern void mtnurscnot(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short usernum=0,scnnot=0,filenum=0,rscnot=0;
char editable=TRUE;
char *module="OLHELP";
MakeBrowseList *mbl_urscnot;
void browse_urscnot(RDArsrc *,RDATData *);
MaintainMaster *MASTER=NULL;

void browse_urscnot(RDArsrc *mtnrsrc,RDATData *previous)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	if(previous!=NULL) FreeRDATData(previous);
	mainrsrc=RDArsrcNEW(module,MASTER->browse_screen);
	searchrsrc=RDArsrcNEW(module,MASTER->search_browse);
	definelist=RDArsrcNEW(module,MASTER->define_list);
	ZERNRD(filenum);
	addDFincvir(mainrsrc,"SECURITY","USERS",NULL,usernum);
	addDFincvir(mainrsrc,module,"SCNNOT",NULL,scnnot);
	addDFincvir(mainrsrc,module,"RSCNOT",NULL,rscnot);
	addDFincvir(searchrsrc,module,"URSCNOT",NULL,filenum);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	ApplyPassKey(MASTER->passkey);
	if(rscnot!=(-1))
	{
		COPYFIELD(filenum,rscnot,"MODULE NAME");
		COPYFIELD(filenum,rscnot,"SCREEN NAME");
		COPYFIELD(filenum,rscnot,"RESOURCE NAME");
		if(EQLNRDsec(rscnot,1))
		{
			ZERNRD(rscnot);
		} else {
			file2rsrc(rscnot,mainrsrc,FALSE);
			filerecord2rsrc(rscnot,mainrsrc);
		}
		if(scnnot!=(-1))
		{
			COPYFIELD(filenum,scnnot,"MODULE NAME");
			COPYFIELD(filenum,scnnot,"SCREEN NAME");
			if(EQLNRDsec(scnnot,1))
			{
				ZERNRD(scnnot);
			} else {
				file2rsrc(scnnot,mainrsrc,FALSE);
				filerecord2rsrc(rscnot,mainrsrc);
			}
		}
	}
	MakeRunFunction(mainrsrc,MASTER);
	mbl_urscnot=ADVPbrowse(filenum,MASTER->passkey->keyno,mainrsrc,searchrsrc,
			definelist,doexit,okfunc,new_record,NULL,FALSE,NULL,0,0,2,0,
			MASTER->passkey);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	if(InitializeSubsystems(argc,argv,module,"MTN URSCNOT")) 
	{
		return;
	}
	if((filenum=APPOPNNRDEDITABLE(module,"URSCNOT",&editable))==(-1)) return;
	if((rscnot=APPOPNNRD(module,"RSCNOT",TRUE,FALSE))==(-1)) return;
	if((scnnot=APPOPNNRD(module,"SCNNOT",TRUE,FALSE))==(-1)) return;
	if((usernum=APPOPNNRD("SECURITY","USERS",TRUE,FALSE))==(-1)) return;
	MASTER=MaintainMasterNew(module,"MTN URSCNOT");
	if(findMaintainMaster(MASTER))
	{
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE);
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN URSCNOT",module);
		doexit(NULL);
	} else {
		MASTER->passkey=READPassKey(filenum);
		switch(MASTER->start_with)
		{
			default:
			case 0:
				mtnurscnot(1,NULL);
				break;
			case 1:
				browse_urscnot(NULL,NULL);
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
		CLSNRD(rscnot);
		CLSNRD(scnnot);
		CLSNRD(usernum);
		ShutdownSubsystems();
	}
	if(mbl_urscnot!=NULL) mbl_urscnot=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;
	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func))
	{
		if(!run_func) mtnurscnot(0,targetkey);
		else ExecuteListFunc(run_func-1,targetkey,MASTER);
	} else mtnurscnot(0,targetkey);
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	mtnurscnot(2,NULL);
} 
