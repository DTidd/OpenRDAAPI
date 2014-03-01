/* fldsec.c - Field Security Browse Program */
/*lint -library */
#ifndef WIN32
#define __NAM__ "fldsec.lnx"
#endif
#ifdef WIN32
#define __NAM__ "fldsec.exe"
#endif
#include <app.hpp>


#include <gui.hpp>
#include <nrd.hpp>
#include <mix.hpp>
/*CPP_OPART fldsecm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

struct fldsecms
{
	APPlib *modlst;
	APPlib *file_defs;
	APPlib *field_defs;
	RDATData *previous;
};
typedef struct fldsecms fldsecm;

static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
short filenum,fnum,usernum,modnum;
char *userid=NULL,*modname=NULL,*filename=NULL,*fieldname=NULL;
char editable=TRUE;
char *module="SECURITY";
MakeBrowseList *mbl_fldsec=NULL;
void browse_fldsec(RDArsrc *,fldsecm *);
extern void mtnfldsec(short,void *);
MaintainMaster *MASTER=NULL;

void browse_fldsec(RDArsrc *mtnrsrc,fldsecm *fdsc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	if(fdsc!=NULL)
	{
		if(fdsc->modlst!=NULL) freeapplib(fdsc->modlst);
		if(fdsc->file_defs!=NULL) freeapplib(fdsc->file_defs);
		if(fdsc->field_defs!=NULL) freeapplib(fdsc->field_defs);
		if(fdsc->previous!=NULL) FreeRDATData(fdsc->previous);
		Rfree(fdsc);
	}
	mainrsrc=RDArsrcNEW(module,MASTER->browse_screen);
	searchrsrc=RDArsrcNEW(module,MASTER->search_browse);
	definelist=RDArsrcNEW(module,MASTER->define_list);
	ZERNRD(filenum);
	addDFincvir(mainrsrc,module,"USERS",NULL,usernum);
	addDFincvir(mainrsrc,module,"MODSEC",NULL,modnum);
	addDFincvir(mainrsrc,module,"FILESEC",NULL,fnum);
	addDFincvir(definelist,module,"FLDSEC",NULL,filenum);
	addDFincvir(searchrsrc,module,"FLDSEC",NULL,filenum);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	ApplyPassKey(MASTER->passkey);
	if(fnum!=(-1))
	{
		COPYFIELD(filenum,fnum,"USER IDENTIFICATION");
		COPYFIELD(filenum,fnum,"MODULE NAME");
		COPYFIELD(filenum,fnum,"MODULE NAME");
		if(EQLNRDsec(fnum,1))
		{
			ZERNRD(fnum);
		} else {
			file2rsrc(fnum,mainrsrc,FALSE);
			filerecord2rsrc(fnum,mainrsrc);
		}
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
	}
	MakeRunFunction(mainrsrc,MASTER);
	mbl_fldsec=ADVPbrowse(filenum,MASTER->passkey->keyno,mainrsrc,searchrsrc,
			definelist,doexit,okfunc,new_record,NULL,FALSE,NULL,0,0,2,0,
			MASTER->passkey);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	if(InitializeSubsystems(argc,argv,module,"MTN FLDSEC"))
	{
		return;
	}
	if((filenum=APPOPNNRDEDITABLE(module,"FLDSEC",&editable))==(-1)) return;
	if((usernum=APPOPNNRD(module,"USERS",TRUE,FALSE))==(-1)) return;
	if((fnum=APPOPNNRD(module,"FILESEC",TRUE,FALSE))==(-1)) return;
	if((modnum=APPOPNNRD(module,"MODSEC",TRUE,FALSE))==(-1)) return;
	MASTER=MaintainMasterNew(module,"MTN FLDSEC");
	if(findMaintainMaster(MASTER))
	{
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE);
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN FLDSEC",module);
		doexit(NULL);
	} else {
		MASTER->passkey=READPassKey(filenum);
		switch(MASTER->start_with)
		{
			default:
			case 0:
				mtnfldsec(1,NULL);
				break;
			case 1:
				browse_fldsec(NULL,NULL);
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
	if(filename!=NULL) Rfree(filename);
	if(userid!=NULL) Rfree(userid);
	if(modname!=NULL) Rfree(modname);
	if(WindowCount<2)
	{
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
		CLSNRD(filenum);
		CLSNRD(usernum);
		CLSNRD(modnum);
		CLSNRD(fnum);
		ShutdownSecurity();
	}
	if(mbl_fldsec!=NULL) mbl_fldsec=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func))
	{
		if(!run_func) mtnfldsec(0,targetkey);
		else ExecuteListFunc(run_func-1,targetkey,MASTER);
	} else mtnfldsec(0,targetkey);
}
static void new_record(RDArsrc *parent,MakeBrowseList *blist)
{
	mtnfldsec(2,NULL);
} 
