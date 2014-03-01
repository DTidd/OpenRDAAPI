/* subpaid.c - Real Estate Payment Master Program*/
/*lint -library */
#ifndef WIN32
#define __NAM__ "subpaid.lnx"
#endif
#ifdef WIN32
#define __NAM__ "subpaid.exe"
#endif
#include <app.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <fin.hpp>
/*CPP_OPART subpaidm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

struct mtnstructs
{
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern void subpaidm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short filenum=(-1),subwknum=(-1),mstnum=(-1),dmgnum=(-1);
short paymstr=(-1),payppms=(-1);
char editable=TRUE;
MakeBrowseList *mbl_subpaid=NULL;
char *module="SUBMGT";
void browse_subpaid(RDArsrc *,mtnstruct *);
MaintainMaster *MTNMASTER=NULL;

void browse_subpaid(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	if(mtn!=NULL)
	{
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	mainrsrc=RDArsrcNEW(module,MTNMASTER->browse_screen);
	searchrsrc=RDArsrcNEW(module,MTNMASTER->search_browse);
	definelist=RDArsrcNEW(module,MTNMASTER->define_list);
	ZERNRD(filenum);
	addDFincvir(mainrsrc,module,"SUBWKMAS",NULL,subwknum);
	addDFincvir(mainrsrc,module,"SUBMASTR",NULL,mstnum);
	addDFincvir(mainrsrc,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(searchrsrc,module,"SUBPAID",NULL,filenum);
	addDFincvir(searchrsrc,module,"SUBWKMAS",NULL,subwknum);
	addDFincvir(searchrsrc,module,"SUBMASTR",NULL,mstnum);
	addDFincvir(searchrsrc,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(searchrsrc,"PAYROLL","PAYMSTR",NULL,paymstr);
	addDFincvir(definelist,module,"SUBPAID",NULL,filenum);
	addDFincvir(definelist,module,"SUBWKMAS",NULL,subwknum);
	addDFincvir(definelist,module,"SUBMASTR",NULL,mstnum);
	addDFincvir(definelist,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(definelist,"PAYROLL","PAYMSTR",NULL,paymstr);
	ApplyPassKey(MTNMASTER->passkey);
	if(dmgnum!=(-1))
	{
		COPYFIELD(filenum,dmgnum,"PERSONNEL IDENTIFICATION");
		if(EQLNRDsec(dmgnum,1)) 
		{
			ZERNRD(dmgnum);
		} else {
			file2rsrc(dmgnum,mainrsrc,FALSE);
			filerecord2rsrc(dmgnum,mainrsrc);
		}
	}
	if(mstnum!=(-1))
	{
		COPYFIELD(filenum,mstnum,"PERSONNEL IDENTIFICATION");
		if(EQLNRDsec(mstnum,1)) 
		{
			ZERNRD(mstnum);
		} else {
			file2rsrc(mstnum,mainrsrc,FALSE);
			filerecord2rsrc(mstnum,mainrsrc);
		}
	}
	if(subwknum!=(-1))
	{
		COPYFIELD(filenum,subwknum,"PERSONNEL IDENTIFICATION");
		COPYFIELD(filenum,subwknum,"TRANSACTION NUMBER");
		if(EQLNRDsec(subwknum,1)) 
		{
			ZERNRD(subwknum);
		} else {
			file2rsrc(subwknum,mainrsrc,FALSE);
			filerecord2rsrc(subwknum,mainrsrc);
		}
	}
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	MakeRunFunction(mainrsrc,MTNMASTER);
	mbl_subpaid=ADVPbrowse(filenum,MTNMASTER->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MTNMASTER->passkey);
}
int main(int argc,char **argv)
{
	if(InitializeSubsystems(argc,argv,module,"MTN SUBPAID")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((filenum=APPOPNNRDEDITABLE(module,"SUBPAID",&editable))==(-1)) return;
	if((mstnum=APPOPNNRD(module,"SUBMASTR",TRUE,FALSE))==(-1)) return;
	if((subwknum=APPOPNNRD(module,"SUBWKMAS",TRUE,FALSE))==(-1)) return;
	if((dmgnum=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((paymstr=APPOPNNRD("PAYROLL","PAYMSTR",TRUE,FALSE))==(-1)) return;
	if((payppms=APPOPNNRD("PAYROLL","PAYPPMS",TRUE,FALSE))==(-1)) return;
	MTNMASTER=MaintainMasterNew(module,"MTN SUBPAID");
	if(findMaintainMaster(MTNMASTER))
	{
		ERRORDIALOG("MAINTAIN MTNMASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE); 
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN SUBPAID",module);
		doexit(NULL);
	} else {
		MTNMASTER->passkey=READPassKey(filenum);
		switch(MTNMASTER->start_with)
		{
			default:
			case 0:
				subpaidm(1,NULL);
				break;
			case 1:
				browse_subpaid(NULL,NULL);
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
		if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
		if(filenum!=(-1)) CLSNRD(filenum);
		if(mstnum!=(-1)) CLSNRD(mstnum);
		if(subwknum!=(-1)) CLSNRD(subwknum);
		if(dmgnum!=(-1)) CLSNRD(dmgnum);
                if(paymstr!=(-1)) CLSNRD(paymstr);
                if(payppms!=(-1)) CLSNRD(payppms);
		ShutdownSubsystems();
	}
	if(mbl_subpaid!=NULL) mbl_subpaid=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func) subpaidm(0,targetkey);
		else ExecuteListFunc(run_func-1,targetkey,MTNMASTER);
	} else subpaidm(0,targetkey);
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	subpaidm(2,NULL);
} 
