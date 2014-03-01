/* restyr.c - Xpert Real Estate Year Master Program*/
/*lint -library */
#ifndef WIN32
#define __NAM__ "restyr.lnx"
#endif
#ifdef WIN32
#define __NAM__ "restyr.exe"
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
/*CPP_OPART restyrm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

extern MakeBrowseList *mbl_restmst;
extern void restmstm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short filenum=0,rstmst=0,fsclyr=0;
char editable=TRUE;
char *module="RLSTMGT";
void browse_msr(RDArsrc *parent,RDATData *);
MaintainMaster *MASTER=NULL;

void browse_msr(RDArsrc *parent,RDATData *previous)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(parent!=NULL) free_rsrc(parent);
	if(previous!=NULL) FreeRDATData(previous);
	mainrsrc=RDArsrcNEW(module,MASTER->browse_screen);
	searchrsrc=RDArsrcNEW(module,MASTER->search_browse);
	definelist=RDArsrcNEW(module,MASTER->define_list);
	ZERNRD(filenum);
	addDFincvir(mainrsrc,module,"RESTMST",NULL,rstmst);
	addDFincvir(mainrsrc,"FINMGT","FINYEAR",NULL,fsclyr);
	addDFincvir(searchrsrc,module,"RESTYEAR",NULL,filenum); 
	addDFincvir(searchrsrc,module,"RESTMST",NULL,rstmst);
	addDFincvir(searchrsrc,"FINMGT","FINYEAR",NULL,fsclyr);
	addDFincvir(definelist,module,"RESTYEAR",NULL,filenum); 
	addDFincvir(definelist,module,"RESTMST",NULL,rstmst);
	addDFincvir(definelist,"FINMGT","FINYEAR",NULL,fsclyr);
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
	if(fsclyr!=(-1))
	{
		COPYFIELD(filenum,fsclyr,"FISCAL YEAR");
		if(ADVEQLNRDsec(fsclyr,1,SCRNvirtualSubData,mainrsrc))
		{
			ZERNRD(fsclyr);
		} else {
			file2rsrc(fsclyr,mainrsrc,FALSE);
			filerecord2rsrc(fsclyr,mainrsrc);
		}
	}
	MakeRunFunction(mainrsrc,MASTER);
	mbl_restmst=ADVPbrowse(filenum,MASTER->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MASTER->passkey);
}
void main(int argc,char **argv)
{
	if(InitializeSubsystems(argc,argv,module,"MTN RESTYEAR")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((filenum=APPOPNNRDEDITABLE(module,"RESTYEAR",&editable))==(-1)) return;
	if((rstmst=APPOPNNRD(module,"RESTMST",TRUE,FALSE))==(-1)) return;
	if((fsclyr=APPOPNNRD("FINMGT","FINYEAR",TRUE,FALSE))==(-1)) return;
	MASTER=MaintainMasterNew(module,"MTN RESTYEAR");
	if(findMaintainMaster(MASTER))
	{
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE); 
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN RESTYEAR",module);
		doexit(NULL);
	} else {
		MASTER->passkey=READPassKey(filenum);
		switch(MASTER->start_with)
		{
			default:
			case 0:
				restmstm(1,NULL);
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
		if(fsclyr!=(-1)) CLSNRD(fsclyr);
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
		ShutdownSubsystems();
	}
	if(mbl_restmst!=NULL) mbl_restmst=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func) restmstm(0,targetkey);
			else ExecuteListFunc(run_func-1,targetkey,MASTER);
	} else restmstm(0,targetkey);
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	restmstm(2,NULL);
} 
