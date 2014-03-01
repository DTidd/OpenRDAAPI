/* submst.c - Xpert Substitute Master  */
/*lint -library */
#ifndef WIN32
#define __NAM__ "submst.lnx"
#endif
#ifdef WIN32
#define __NAM__ "submst.exe"
#endif
#include <app.hpp>



#include <cstdlib>
#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
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

static void doexit(MakeBrowseList *);
static void okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
static short filenum=(-1),dmgnum=(-1);
static char editable=TRUE,*module="SUBMGT";
static MakeBrowseList *mbl_mst=NULL;
static MaintainMaster *MTNMASTER=NULL;
static void submstm(short,void *),browse_submst(RDArsrc *,mtnstruct *);
static void save_record(RDArsrc *,mtnstruct *,char);
static void quit_record(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void save_check(RDArsrc *,mtnstruct *,char);
static void getrecord(RDArsrc *,mtnstruct *);
static void readscreen(RDArsrc *,mtnstruct *);

static void save_record_quit(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,TRUE);
}
static void quit_record_test(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *desc=NULL;

	readscreen(mtnrsrc,mtn);
	if(QUITvfy(filenum,mtn->previous))
	{
		desc=QUITvfydesc(filenum,mtn->previous);
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN",
			"CHANGES NOT SAVED!",desc,
			save_record_quit,quit_record,FALSE,2,
			mtnrsrc,mtn,NULL);
		if(desc!=NULL) Rfree(desc);
	} else {
		quit_record(mtnrsrc,mtn);
	}
}
static void getsupportingrecs(RDArsrc *mtnrsrc)
{
	if(dmgnum!=(-1))
	{
		COPYFIELD(filenum,dmgnum,"PERSONNEL IDENTIFICATION");
		if(EQLNRDsec(dmgnum,1)) KEYNRD(dmgnum,1);
		filerecord2rsrc(dmgnum,mtnrsrc);
	}
}
void browse_submst(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *searchrsrc=NULL,*definelist=NULL;
	RDArsrc *mainrsrc=NULL;

	if(mtn!=NULL)
	{
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	mainrsrc=RDArsrcNEW(module,MTNMASTER->browse_screen);
	searchrsrc=RDArsrcNEW(module,MTNMASTER->search_browse);
	definelist=RDArsrcNEW(module,MTNMASTER->define_list);
	ZERNRD(filenum);
	addDFincvir(definelist,module,"SUBMASTR",NULL,filenum);
	addDFincvir(definelist,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(searchrsrc,module,"SUBMASTR",NULL,filenum);
	addDFincvir(searchrsrc,"PRSNNL","PERDMG",NULL,dmgnum);
	ApplyPassKey(MTNMASTER->passkey);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	MakeRunFunction(mainrsrc,MTNMASTER);
	mbl_mst=ADVPbrowse(filenum,MTNMASTER->passkey->keyno,mainrsrc,searchrsrc,
		definelist,doexit,okfunc,new_record,NULL,FALSE,NULL,0,0,2,0,
		MTNMASTER->passkey);
}
int main(int argc,char **argv)
{
	if(InitializeSubsystems(argc,argv,module,"MTN SUBMASTR")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((filenum=APPOPNNRDEDITABLE(module,"SUBMASTR",&editable))==(-1)) return;
	if((dmgnum=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	MTNMASTER=MaintainMasterNew(module,"MTN SUBMASTR");
	if(findMaintainMaster(MTNMASTER))
	{
		ERRORDIALOG("MAINTAIN MTNMASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE); 
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN SUBMASTR",module);
		doexit(NULL);
	} else {
		MTNMASTER->passkey=READPassKey(filenum);
		switch(MTNMASTER->start_with)
		{
			default:
			case 0:
				submstm(1,NULL);
				break;
			case 1:
				browse_submst(NULL,NULL);
				break;
		}
	}
	RDAAPPMAINLOOP();
}
static void doexit(MakeBrowseList *b)
{
	if(b!=NULL)
	{
		if(b->definelist!=NULL) 
		{
			killwindow(b->definelist);
			free_rsrc(b->definelist);
		}
		if(b->searchrsrc!=NULL) 
		{
			killwindow(b->searchrsrc);
			free_rsrc(b->searchrsrc);
		}
		if(b->mainrsrc!=NULL) free_rsrc(b->mainrsrc);
	}
	if(WindowCount==1)
	{
		if(filenum!=(-1)) CLSNRD(filenum);
		if(dmgnum!=(-1)) CLSNRD(dmgnum);
		if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
		ShutdownSubsystems();
	}
	mbl_mst=NULL;
}
static void okfunc(MakeBrowseList *b,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(b->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func)
		{
			submstm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MTNMASTER);
		}
	} else {
		submstm(0,targetkey);
	}
}
static void new_record(RDArsrc *parent,MakeBrowseList *b)
{
	submstm(2,NULL);
} 
void submstm(short dowhich,void *targetkey)
{
	RDArsrc *mtnrsrc=NULL;
	mtnstruct *mtn=NULL;

	mtnrsrc=RDArsrcNEW(module,MTNMASTER->maintain_screen);
	ZERNRD(filenum);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->previous=RDATDataNEW(filenum);
	file2rsrc(filenum,mtnrsrc,editable);
	file2rsrc(dmgnum,mtnrsrc,FALSE);
	FINDRSCSETFUNC(mtnrsrc,"[SUBMASTR][PERSONNEL IDENTIFICATION]",
		getrecord,mtn);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			ApplyPassKey(MTNMASTER->passkey);
			if(EQLNRDsec(filenum,MTNMASTER->passkey->keyno))
			{
				KEYNRD(filenum,MTNMASTER->passkey->keyno);
				ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
			} else {
				filerecord2rsrc(filenum,mtnrsrc);
				GetRDATData(filenum,mtn->previous);
			}
			break;
		case 2:
			ApplyPassKey(MTNMASTER->passkey);
			ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
			break;
		default:
		case 0:
			if(EQLNRDKEYsec(filenum,MTNMASTER->passkey->keyno,targetkey))
			{
				ZERNRD(filenum);
				ApplyPassKey(MTNMASTER->passkey);
				ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
			} else {
				filerecord2rsrc(filenum,mtnrsrc);
				GetRDATData(filenum,mtn->previous);
			}
			break;
	}
	getsupportingrecs(mtnrsrc);
	addDFincvir(mtnrsrc,module,"SUBMASTR",NULL,filenum);
	addDFincvir(mtnrsrc,"PRSNNL","PERDMG",NULL,dmgnum);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	MakeMasterButtons(mtnrsrc,MTNMASTER);
	if(editable)
	{
		addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
		if(mbl_mst!=NULL)
		{
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
		} else {
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
		}
	}
	if(mbl_mst==NULL)
	{
		addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_submst,mtn);
	}
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mtnrsrc);
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_mst==NULL?TRUE:FALSE));
}
static void save_record_up(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,FALSE);
}
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(filenum);
	readallwidgets(mtnrsrc);
	rsrc2filerecord(filenum,mtnrsrc);
}
static void save_check(RDArsrc *mtnrsrc,mtnstruct *mtn,char update_list)
{
	readscreen(mtnrsrc,mtn);
	save_record(mtnrsrc,mtn,update_list);
}
static void save_record(RDArsrc *mtnrsrc,mtnstruct *mtn,char update_list)
{
	char *rcddesc=NULL,deleteflag=FALSE,subdeleteflag=FALSE;
	char *perid=NULL,*subid=NULL;
	short subfilenum=0,ef=0,x=0;
	RDATData *prev=NULL;

	if(RECORDsec(filenum))
	{
		rcddesc=RECORDsecDesc(filenum);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
      			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(filenum,0,NULL,mtn->previous);
		FINDFLDGETCHAR(filenum,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			for(x=0;x<5;++x)
			{
				if(x==0) subfilenum=OPNNRDsec(module,"SUBLOC",TRUE,TRUE,FALSE);	
				else if(x==1) subfilenum=OPNNRDsec(module,"SUBEMP",TRUE,TRUE,FALSE);	
				else if(x==2) subfilenum=OPNNRDsec(module,"SUBCTACT",TRUE,TRUE,FALSE);	
				else if(x==3) subfilenum=OPNNRDsec(module,"SUBWKMAS",TRUE,TRUE,FALSE);	
				else if(x==4) subfilenum=OPNNRDsec(module,"SUBPAID",TRUE,TRUE,FALSE);	
				if(subfilenum!=(-1))
				{
					ZERNRD(subfilenum);
					FINDFLDGETSTRING(filenum,"PERSONNEL IDENTIFICATION",&perid);
					FINDFLDSETSTRING(subfilenum,"PERSONNEL IDENTIFICATION",perid);
					ef=GTENRDsec(subfilenum,1);
					while(!ef)
					{
						FINDFLDGETCHAR(subfilenum,"DELETEFLAG",&subdeleteflag);
						if(!subdeleteflag)
						{
							FINDFLDGETSTRING(subfilenum,"PERSONNEL IDENTIFICATION",&subid);
							if(RDAstrcmp(subid,perid)) break;
							prev=RDATDataNEW(subfilenum);
							FINDFLDSETCHAR(subfilenum,"DELETEFLAG",deleteflag);
							WRTTRANSsec(subfilenum,0,NULL,prev);
							if(prev!=NULL) FreeRDATData(prev);
						}
						ef=NXTNRDsec(subfilenum,1); 	
					}
					CLSNRD(subfilenum);		
				}
			}
		} 
		if(perid!=NULL) Rfree(perid);
		if(subid!=NULL) Rfree(subid);
		updatebrowse(update_list,mbl_mst,filenum,module,mtnrsrc);
  		if(mbl_mst!=NULL) quit_record(mtnrsrc,mtn);
		else GetRDATData(filenum,mtn->previous);
  	}
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(WindowCount==1)
	{
		if(filenum!=(-1)) CLSNRD(filenum);
		if(dmgnum!=(-1)) CLSNRD(dmgnum);
		if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
		ShutdownSubsystems();
		exit(0);
	}
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
}
static void getrecord(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *perid=NULL;

	readallwidgets(mtnrsrc);
	if(FINDRSCGETSTRING(mtnrsrc,"[SUBMASTR][PERSONNEL IDENTIFICATION]",&perid)) return;
	ZERNRD(filenum);
	FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid);
	if(EQLNRDsec(filenum,MTNMASTER->passkey->keyno)) 
	{
		KEYNRD(filenum,MTNMASTER->passkey->keyno);
		ClearRDATData(mtn->previous);
	} else {
		filerecord2rsrc(filenum,mtnrsrc);
		GetRDATData(filenum,mtn->previous);
	}
	if(perid!=NULL) Rfree(perid);
	getsupportingrecs(mtnrsrc);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
