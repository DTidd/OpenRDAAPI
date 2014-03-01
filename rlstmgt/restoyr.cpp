/* restoyr.c - Xpert Real Estate Owner Year Master Program*/
/*lint -library */
#ifndef WIN32
#define __NAM__ "restoyr.lnx"
#endif
#ifdef WIN32
#define __NAM__ "restoyr.exe"
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
/*CPP_OPART restoyrm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

extern MakeBrowseList *mbl_restoyr;
extern void restoyrm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short filenum=0,rstmst=0,fsclyr=0,taxmst=0,rstown=0,rstyr=0;
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
	addDFincvir(mainrsrc,module,"RESTOWN",NULL,rstown);
	addDFincvir(mainrsrc,module,"RESTYEAR",NULL,rstyr);
	addDFincvir(mainrsrc,"TAXCLT","TAXMST",NULL,taxmst);
	addDFincvir(mainrsrc,"FINMGT","FINYEAR",NULL,fsclyr);
	addDFincvir(searchrsrc,module,"RESTOYR",NULL,filenum); 
	addDFincvir(searchrsrc,module,"RESTMST",NULL,rstmst);
	addDFincvir(searchrsrc,module,"RESTOWN",NULL,rstown);
	addDFincvir(searchrsrc,module,"RESTYEAR",NULL,rstyr);
	addDFincvir(searchrsrc,"TAXCLT","TAXMST",NULL,taxmst);
	addDFincvir(searchrsrc,"FINMGT","FINYEAR",NULL,fsclyr);
	addDFincvir(definelist,module,"RESTOYR",NULL,filenum); 
	addDFincvir(definelist,module,"RESTMST",NULL,rstmst);
	addDFincvir(definelist,module,"RESTOWN",NULL,rstown);
	addDFincvir(definelist,module,"RESTYEAR",NULL,rstyr);
	addDFincvir(definelist,"TAXCLT","TAXMST",NULL,taxmst);
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
	if(rstown!=(-1))
	{
		COPYFIELD(filenum,rstown,"REAL ESTATE IDENTIFICATION");
		COPYFIELD(filenum,rstown,"OWNER IDENTIFICATION");
		if(ADVEQLNRDsec(rstown,1,SCRNvirtualSubData,mainrsrc))
		{
			ZERNRD(rstown);
		} else {
			file2rsrc(rstown,mainrsrc,FALSE);
			filerecord2rsrc(rstown,mainrsrc);
		}
	}
	if(rstyr!=(-1))
	{
		COPYFIELD(filenum,rstyr,"REAL ESTATE IDENTIFICATION");
		COPYFIELD(filenum,rstyr,"FISCAL YEAR");
		if(ADVEQLNRDsec(rstyr,1,SCRNvirtualSubData,mainrsrc))
		{
			ZERNRD(rstyr);
		} else {
			file2rsrc(rstyr,mainrsrc,FALSE);
			filerecord2rsrc(rstyr,mainrsrc);
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
	mbl_restoyr=ADVPbrowse(filenum,MASTER->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MASTER->passkey);
}
void main(int argc,char **argv)
{
	if(InitializeSubsystems(argc,argv,module,"MTN RESTOYR")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((filenum=APPOPNNRDEDITABLE(module,"RESTOYR",&editable))==(-1)) return;
	if((rstmst=APPOPNNRD(module,"RESTMST",TRUE,FALSE))==(-1)) return;
	if((rstown=APPOPNNRD(module,"RESTOWN",TRUE,FALSE))==(-1)) return;
	if((rstyr=APPOPNNRD(module,"RESTYEAR",TRUE,FALSE))==(-1)) return;
	if((taxmst=APPOPNNRD("TAXCLT","TAXMST",TRUE,FALSE))==(-1)) return;
	if((fsclyr=APPOPNNRD("FINMGT","FINYEAR",TRUE,FALSE))==(-1)) return;
	MASTER=MaintainMasterNew(module,"MTN RESTOYR");
	if(findMaintainMaster(MASTER))
	{
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE); 
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN RESTOYR",module);
		doexit(NULL);
	} else {
		MASTER->passkey=READPassKey(filenum);
		switch(MASTER->start_with)
		{
			default:
			case 0:
				restoyrm(1,NULL);
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
		if(rstyr!=(-1)) CLSNRD(rstyr);
		if(rstown!=(-1)) CLSNRD(rstown);
		if(taxmst!=(-1)) CLSNRD(taxmst);
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
		ShutdownSubsystems();
	}
	if(mbl_restoyr!=NULL) mbl_restoyr=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func) restoyrm(0,targetkey);
			else ExecuteListFunc(run_func-1,targetkey,MASTER);
	} else restoyrm(0,targetkey);
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	restoyrm(2,NULL);
} 
