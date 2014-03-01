/* bfttran.c - Xpert Benefit Transaction Program*/
/*lint -library */
#ifndef WIN32
#define __NAM__ "bfttran.lnx"
#endif
#ifdef WIN32
#define __NAM__ "bfttran.exe"
#endif
#include <app.hpp>


#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
/*CPP_OPART bfttranm */
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

extern void bfttranm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short msrnum=(-1),descnum=(-1),mdacnum=(-1),ddacnum=(-1);
char editable=TRUE;
MakeBrowseList *mbl_bfttran=NULL;
MaintainMaster *MTNMASTER=NULL;
void browse_bfttran(RDArsrc *,mtnstruct *);

void browse_bfttran(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;
	short deftype=(-1);	
	RDAacct *acct=NULL;

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
	addstdrsrc(mainrsrc,"DEFINITION STRING",VARIABLETEXT,0,NULL,FALSE);
	if(mdacnum!=(-1))
	{
		FINDFLDGETSHORT(MTNMASTER->passkey->fileno,"DEFINITION TYPE",&deftype);
		if(deftype>(-1) && deftype<BALANCE_SHEET_ACCOUNT->num)
		{
			acct=BALANCE_SHEET_ACCOUNT->codetype;
			acct+=deftype;
			FINDRSCSETSTRING(mainrsrc,"DEFINITION STRING",acct->name);
		}
	}
	mbl_bfttran=ADVPbrowse(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,mainrsrc,
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

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	if(InitializeSubsystems(argc,argv,"BFTMGT","MTN BFTTRAN")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	MTNMASTER=MaintainMasterNew("BFTMGT","MTN BFTTRAN");
	if(findMaintainMaster(MTNMASTER))
	{
		ERRORDIALOG("MAINTAIN MTNMASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE);
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN BFTTRAN",MTNMASTER->module);
		doexit(NULL);
	} else {
		if((filenum=APPOPNNRDEDITABLE(MTNMASTER->module,"BFTTRAN",&editable))==(-1))
		{
			doexit(NULL);
			return;
		}
		SetSupportingWritable(MTNMASTER,MTNMASTER->module,"BFTDESC",1,TRUE,&editable);
		SetSupportingWritable(MTNMASTER,MTNMASTER->module,"BFTMDAC",1,TRUE,&editable);
		SetSupportingWritable(MTNMASTER,MTNMASTER->module,"BFTDDAC",1,TRUE,&editable);
		SetSupportingWritable(MTNMASTER,MTNMASTER->module,"BFTMSTR",1,TRUE,&editable);
		if(!OPEN_MASTER_FILES(MTNMASTER))
		{
			doexit(NULL);
			return;
		}
		descnum=APPReturnSupportingFileno(MTNMASTER,MTNMASTER->module,"BFTDESC",1);
		mdacnum=APPReturnSupportingFileno(MTNMASTER,MTNMASTER->module,"BFTMDAC",1);
		ddacnum=APPReturnSupportingFileno(MTNMASTER,MTNMASTER->module,"BFTDDAC",1);
		msrnum=APPReturnSupportingFileno(MTNMASTER,MTNMASTER->module,"BFTMSTR",1);
		MTNMASTER->passkey=READPassKey(filenum);
		MTNMASTER->inheritrsrcs=TRUE;
		switch(MTNMASTER->start_with)
		{
			default:
			case 0:
				bfttranm(1,NULL);
				break;
			case 1:
				browse_bfttran(NULL,NULL);
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
	if(mbl_bfttran!=NULL) mbl_bfttran=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func))
	{
		if(!run_func)
		{
			bfttranm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MTNMASTER);
		}
	} else {
		bfttranm(0,targetkey);
	}
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	bfttranm(2,NULL);
} 
