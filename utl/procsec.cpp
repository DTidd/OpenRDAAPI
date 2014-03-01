/* procsec.c - Process Security Browse Program */
/*lint -library */
#ifndef WIN32
#define __NAM__ "procsec.lnx"
#endif
#ifdef WIN32
#define __NAM__ "procsec.exe"
#endif
#include <app.hpp>


#include <mix.hpp>
/*CPP_OPART procsecm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

struct procsecms
{
	APPlib *scr_libs;
	RDATData *previous;
};
typedef struct procsecms procsecm;

extern void mtnprocsec(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short filenum,usernum,modnum;
char editable=TRUE;
char *module="SECURITY";
MakeBrowseList *mbl_procsec=NULL;
void browse_procsec(RDArsrc *,procsecm *);
MaintainMaster *MTNMASTER=NULL;

void browse_procsec(RDArsrc *mtnrsrc,procsecm *prsc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	if(prsc!=NULL)
	{
		if(prsc->scr_libs!=NULL) freeapplib(prsc->scr_libs);
		if(prsc->previous!=NULL) FreeRDATData(prsc->previous);
		Rfree(prsc);
	}
	mainrsrc=RDArsrcNEW(module,MTNMASTER->browse_screen);
	searchrsrc=RDArsrcNEW(module,MTNMASTER->search_browse);
	definelist=RDArsrcNEW(module,MTNMASTER->define_list);
	ZERNRD(filenum);
	addDFincvir(mainrsrc,module,"USERS",NULL,usernum);
	addDFincvir(mainrsrc,module,"MODSEC",NULL,modnum);
	addDFincvir(definelist,module,"PROCSEC",NULL,filenum);
	addDFincvir(searchrsrc,module,"PROCSEC",NULL,filenum);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	ApplyPassKey(MTNMASTER->passkey);
	if(modnum!=(-1))
	{
		COPYFIELD(filenum,modnum,"USER IDENTIFICATION");
		COPYFIELD(filenum,modnum,"MODULE NAME");
		if(EQLNRDsec(modnum,1))
		{
			ZERNRD(modnum);
		} else {
			file2rsrc(modnum,mainrsrc,FALSE);
			filerecord2rsrc(modnum,mainrsrc);
		}
		if(usernum!=(-1))
		{
			COPYFIELD(filenum,usernum,"USER IDENTIFICATION");
			if(EQLNRDsec(usernum,1))
			{
				ZERNRD(usernum);
			} else {
				file2rsrc(usernum,mainrsrc,FALSE);
				filerecord2rsrc(usernum,mainrsrc);
			}
		}
	}
	MakeRunFunction(mainrsrc,MTNMASTER);
	mbl_procsec=ADVPbrowse(filenum,MTNMASTER->passkey->keyno,mainrsrc,searchrsrc,
			definelist,doexit,okfunc,new_record,NULL,FALSE,NULL,0,0,2,0,
			MTNMASTER->passkey);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	if(InitializeSubsystems(argc,argv,module,"MTN PROCSEC"))
	{
		return;
	}
	if((filenum=APPOPNNRDEDITABLE(module,"PROCSEC",&editable))==(-1)) return;
	if((usernum=APPOPNNRD(module,"USERS",TRUE,FALSE))==(-1)) return;
	if((modnum=APPOPNNRD(module,"MODSEC",TRUE,FALSE))==(-1)) return;
	MTNMASTER=MaintainMasterNew(module,"MTN PROCSEC");
	if(findMaintainMaster(MTNMASTER))
	{
		ERRORDIALOG("MAINTAIN MTNMASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE);
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN PROCSEC",module);
		doexit(NULL);
	} else {
		MTNMASTER->passkey=READPassKey(filenum);
		switch(MTNMASTER->start_with)
		{
			default:
			case 0:
				mtnprocsec(1,NULL);
				break;
			case 1:
				browse_procsec(NULL,NULL);
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
		if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
		CLSNRD(filenum);
		CLSNRD(usernum);
		CLSNRD(modnum);
		ShutdownSubsystems();
	}
	if(mbl_procsec!=NULL) mbl_procsec=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;
	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func))
	{
		if(!run_func) mtnprocsec(0,targetkey);
		else ExecuteListFunc(run_func-1,targetkey,MTNMASTER);
	} else mtnprocsec(0,targetkey);
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	mtnprocsec(2,NULL);
} 
