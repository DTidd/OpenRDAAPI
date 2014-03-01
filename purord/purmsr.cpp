/* purmsr.c - Xpert Purchase Order Master Maintain Master Program*/
/*lint -library */
#ifndef WIN32
#define __NAM__ "purmsr.lnx"
#endif
#ifdef WIN32
#define __NAM__ "purmsr.exe"
#endif
#include <app.hpp>


#include <mix.hpp>
#include <venpmt.hpp>
#include <purord.hpp>
/*CPP_OPART purmsrm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

struct mtnstructs
{
	APPlib *addresslist;
	char *POID;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern void purmsrm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
char editable=TRUE;
MakeBrowseList *mbl_purmsr=NULL;
void browse_purmsr(RDArsrc *parent,mtnstruct *);
MaintainMaster *MASTER=NULL;
short venadr=(-1),venadr2=(-1),purids=(-1),vennum=(-1);
short venadrkeyno=(-1);
RDApurord *purord=NULL;
RDAvenpmt *venpmt=NULL;

void browse_purmsr(RDArsrc *parent,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtn!=NULL)
	{
		if(mtn->addresslist!=NULL) freeapplib(mtn->addresslist);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		if(mtn->POID!=NULL) Rfree(mtn->POID);
		Rfree(mtn);
	}
	if(parent!=NULL) free_rsrc(parent);
	mainrsrc=RDArsrcNEW(MASTER->module,MASTER->browse_screen);
	searchrsrc=RDArsrcNEW(MASTER->module,MASTER->search_browse);
	definelist=RDArsrcNEW(MASTER->module,MASTER->define_list);
	ZERNRD(MASTER->passkey->fileno);
	ApplyPassKey(MASTER->passkey);
	GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,MASTER,SCRNvirtualSubData,(void *)mainrsrc);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	MakeRunFunction(mainrsrc,MASTER);
	mbl_purmsr=ADVPbrowse(MASTER->passkey->fileno,MASTER->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MASTER->passkey);
}
int main(int argc,char **argv)
{
	short filenum=(-1);
	char *warnmessage=NULL;
	char *module="PURORD",*mtnname="MTN PURMST";

	if(InitializeSubsystems(argc,argv,module,mtnname)) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	venpmt=RDAvenpmtNEW();
	if(getvenpmtbin(venpmt)==(-1))
	{
		warnmessage=Rmalloc(350);
		sprintf(warnmessage,"Error Vendor Payment Setup definition not found, this process cannot be executed without it's definition in the [VENPMT.STP] library.");
		ERRORDIALOG("VENDOR PAYMENT SETUP NOT FOUND!",warnmessage,NULL,TRUE);
		prterr("%s",warnmessage);
		if(warnmessage!=NULL) Rfree(warnmessage);
		doexit(NULL);
	}
	purord=RDApurordNEW();
	getpurordbin(purord);
	MASTER=MaintainMasterNew(module,mtnname);
	if(findMaintainMaster(MASTER))
	{
		warnmessage=Rmalloc(300+RDAstrlen(MASTER->module)+RDAstrlen(mtnname));
		sprintf(warnmessage,"Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.",mtnname,MASTER->module);
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.",warnmessage,NULL,TRUE);
		prterr("%s",warnmessage);
		if(warnmessage!=NULL) Rfree(warnmessage);
		doexit(NULL);
	} else {
		if((filenum=APPOPNNRDEDITABLE(MASTER->module,MASTER->mainfile,
			&editable))==(-1))
		{
			doexit(NULL);
			return;
		}
		if(!OPEN_MASTER_FILES(MASTER))
		{
			doexit(NULL);
			return;
		}
		vennum=APPReturnSupportingFileno(MASTER,"VENPMT","VENMSR",1);
		venadr=APPReturnSupportingFileno(MASTER,"VENPMT","VENADR",1);
		venadrkeyno=KEYNUMBER(venadr,"VENDOR ADDRESS KEY");
		if(venadrkeyno==(-1))
		{
			venadrkeyno=1;
		}
		venadr2=APPReturnSupportingFileno(MASTER,"VENPMT","VENADR",2);
		purids=APPReturnSupportingFileno(MASTER,"PURORD","PURIDS",1);
		MASTER->passkey=READPassKey(filenum);
		MASTER->inheritrsrcs=FALSE;
		switch(MASTER->start_with)
		{
			default:
			case 0:
				purmsrm(1,NULL);
				break;
			case 1:
				browse_purmsr(NULL,NULL);
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
		CLOSE_MASTER_FILES(MASTER);
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
		if(purord!=NULL) free_purord(purord);
		if(venpmt!=NULL) free_venpmt(venpmt);
		ShutdownSubsystems();
	}
	if(mbl_purmsr!=NULL) mbl_purmsr=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func)
		{
			purmsrm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MASTER);
		}
	} else {
		purmsrm(0,targetkey);
	}
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	purmsrm(2,NULL);
} 
