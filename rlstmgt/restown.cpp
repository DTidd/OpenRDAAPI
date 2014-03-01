/* restown.c - Xpert Real Estate Owner Master Program*/
/*lint -library */
#ifndef WIN32
#define __NAM__ "restown.lnx"
#endif
#ifdef WIN32
#define __NAM__ "restown.exe"
#endif
#include <app.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <alloc.h>
#include <gui.h>
#include <nrd.h>
#include <trans.h>
#include <sec.h>
#include <mix.h>
/*CPP_OPART restownm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

struct mtnstructs 
{
	APPlib *owntypes;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern MakeBrowseList *mbl_restown;
extern void restownm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short filenum=0,rstmst=0,taxmst=0;
char editable=TRUE;
char *module="RLSTMGT";
void browse_msr(RDArsrc *parent,mtnstruct *);
MaintainMaster *MASTER=NULL;

void browse_msr(RDArsrc *parent,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(parent!=NULL) free_rsrc(parent);
	if(mtn!=NULL)
	{
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		if(mtn->owntypes!=NULL) freeapplib(mtn->owntypes);
		Rfree(mtn);
	}
	mainrsrc=RDArsrcNEW(module,MASTER->browse_screen);
	searchrsrc=RDArsrcNEW(module,MASTER->search_browse);
	definelist=RDArsrcNEW(module,MASTER->define_list);
	ZERNRD(filenum);
	addDFincvir(mainrsrc,module,"RESTMST",NULL,rstmst);
	addDFincvir(mainrsrc,"TAXCLT","TAXMST",NULL,taxmst);
	addDFincvir(searchrsrc,module,"RESTOWN",NULL,filenum); 
	addDFincvir(searchrsrc,module,"RESTMST",NULL,rstmst);
	addDFincvir(searchrsrc,"TAXCLT","TAXMST",NULL,taxmst);
	addDFincvir(definelist,module,"RESTOWN",NULL,filenum); 
	addDFincvir(definelist,module,"RESTMST",NULL,rstmst);
	addDFincvir(definelist,"TAXCLT","TAXMST",NULL,taxmst);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	ApplyPassKey(MASTER->passkey);
	if(rstmst!=(-1))
	{
		COPYFIELD(filenum,rstmst,"REAL ESTATE IDENTIFICATION");
		if(ADVEQLNRDsec(rstmst,1,SCRNvirtualSubData,mainrsrc))
		{
			ZERNRD(rstmst);
		} else {
			file2rsrc(rstmst,mainrsrc,FALSE);
			filerecord2rsrc(rstmst,mainrsrc);
		}
	}
	if(taxmst!=(-1))
	{
		COPYFIELD(filenum,taxmst,"OWNER IDENTIFICATION");
		if(ADVEQLNRDsec(taxmst,1,SCRNvirtualSubData,mainrsrc))
		{
			ZERNRD(taxmst);
		} else {
			file2rsrc(taxmst,mainrsrc,FALSE);
			filerecord2rsrc(taxmst,mainrsrc);
		}
	}
	MakeRunFunction(mainrsrc,MASTER);
	mbl_restown=ADVPbrowse(filenum,MASTER->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MASTER->passkey);
}
void main(int argc,char **argv)
{
	if(InitializeSubsystems(argc,argv,module,"MTN RESTOWN")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((filenum=APPOPNNRDEDITABLE(module,"RESTOWN",&editable))==(-1)) return;
	if((rstmst=APPOPNNRD(module,"RESTMST",TRUE,FALSE))==(-1)) return;
	if((taxmst=APPOPNNRD("TAXCLT","TAXMST",TRUE,FALSE))==(-1)) return;
	MASTER=MaintainMasterNew(module,"MTN RESTOWN");
	if(findMaintainMaster(MASTER))
	{
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE); 
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN RESTOWN",module);
		doexit(NULL);
	} else {
		MASTER->passkey=READPassKey(filenum);
		switch(MASTER->start_with)
		{
			default:
			case 0:
				restownm(1,NULL);
				break;
			case 1:
				browse_msr(NULL,NULL);
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
	if(WindowCount==1)
	{
		if(filenum!=(-1)) CLSNRD(filenum);
		if(rstmst!=(-1)) CLSNRD(rstmst);
		if(taxmst!=(-1)) CLSNRD(taxmst);
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
		ShutdownSubsystems();
	}
	if(mbl_restown!=NULL) mbl_restown=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func) restownm(0,targetkey);
			else ExecuteListFunc(run_func-1,targetkey,MASTER);
	} else restownm(0,targetkey);
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	restownm(2,NULL);
} 
