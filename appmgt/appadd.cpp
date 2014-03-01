/* appadd.c - Add Applicants Program */
/*lint -library */

#ifndef WIN32
#define __NAM__ "appadd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "appadd.exe"
#endif
#include <app.hpp>



#include <cstdlib>
#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
/*CPP_OPART appaddm */
/*CPP_OPART appadded */
/*CPP_OPART appaddcr */
/*CPP_OPART appaddct */
/*CPP_OPART appaddd */
/*CPP_OPART appadden */
/*CPP_OPART appaddp */
/*CPP_OPART appaddr */
/*CPP_OPART appaddpe */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

extern void mtndmg(void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
MaintainMaster *MTNMASTER;
void browse_dmg(RDArsrc *);
char editable=TRUE;

void browse_dmg(RDArsrc *mtnrsrc)
{
	RDArsrc *searchrsrc=NULL,*definelist=NULL;
	RDArsrc *mainrsrc=NULL;

	mainrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->browse_screen);
	searchrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->search_browse);
	definelist=RDArsrcNEW(MTNMASTER->module,MTNMASTER->define_list);
	ZERNRD(MTNMASTER->passkey->fileno);
	ApplyPassKey(MTNMASTER->passkey);
	GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,MTNMASTER,SCRNvirtualSubData,(void *)mainrsrc);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	ADVPbrowse(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,mainrsrc,
		searchrsrc,definelist,doexit,okfunc,NULL,
		mtnrsrc,TRUE,NULL,2,2,2,0,MTNMASTER->passkey);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	short filenum=(-1);
	char *warnmessage=NULL,*module=NULL,*mtnname=NULL;

	RDA_SOFTWARE_TYPE=RDA_XPERT_AND_LITE_APP;
	module="APPMGT";
	mtnname="ADD APPLICANTS";
	if(InitializeSubsystems(argc,argv,module,mtnname))
	{
		RDAAPPMAINLOOP();
		return;
	}
	MTNMASTER=MaintainMasterNew(module,mtnname);
	if(findMaintainMaster(MTNMASTER))
	{
		warnmessage=Rmalloc(300+RDAstrlen(MTNMASTER->module)+RDAstrlen(mtnname));
		sprintf(warnmessage,"Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.",mtnname,MTNMASTER->module);
		ERRORDIALOG("MAINTAIN MTNMASTER NOT FOUND.",warnmessage,NULL,TRUE);
		prterr("%s",warnmessage);
		if(warnmessage!=NULL) Rfree(warnmessage);
		doexit(NULL);
	} else {
		if((filenum=APPOPNNRDEDITABLE(MTNMASTER->module,"APPCNT",&editable))==(-1))
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
		mtndmg(NULL);
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
		exit(0);
	}
}
static void okfunc(MakeBrowseList *b,void *targetkey)
{
	int y=0;
	RDArsrc *r;

	ZERNRD(MTNMASTER->passkey->fileno);
	if(ADVEQLNRDKEYsec(MTNMASTER->passkey->fileno,1,targetkey,SCRNvirtualSubData,b->mainrsrc))
	{
		KEYNRD(MTNMASTER->passkey->fileno,1);
	}
	updatefilerecord2rsrc(MTNMASTER->passkey->fileno,b->funcargs);
	y=FINDRSC(b->funcargs,"[APPCNT][APPLICANT IDENTIFICATION]");
	if(y!=(-1))
	{
		r=(RDArsrc *)b->funcargs;
		ExecuteRDArmemFunction(r->rscs+y);
	}
}
