/* pursitem.c - Purchase Order Site Maintain Screen */
#include <cstdio>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>

extern short filenum;
extern char editable;
extern char *module;
extern MakeBrowseList *mbl_site;
extern void browse_site(RDArsrc *,RDATData *);
static void save_record(RDArsrc *,RDATData *,short);
static void save_check(RDArsrc *,RDATData *,short);
static void quit_record(RDArsrc *,RDATData *);
static void save_record_up(RDArsrc *,RDATData *);
static void save_record_nup(RDArsrc *,RDATData *);
static void getrecord(RDArsrc *,RDATData *);
extern MaintainMaster *MASTER;
void pursitem(short,void *);

static void quit_record_test(RDArsrc *mtnrsrc,RDATData *previous)
{
	char *desc=NULL;

	readallwidgets(mtnrsrc);
	rsrc2filerecord(filenum,mtnrsrc);
	if(QUITvfy(filenum,previous))
	{
		desc=QUITvfydesc(filenum,previous);
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!",
			desc,save_record_up,quit_record,FALSE,2,mtnrsrc,previous,NULL);
		if(desc!=NULL) Rfree(desc);
	} else {
		quit_record(mtnrsrc,previous);
	}
}
void pursitem(short dowhich,void *targetkey)
{
	char *siteid=NULL;
	RDArsrc *mtnrsrc=NULL;
	RDATData *previous=NULL;

	mtnrsrc=RDArsrcNEW(module,MASTER->maintain_screen);
	file2rsrc(filenum,mtnrsrc,TRUE);
	DefaultScreens(mtnrsrc);
	ZERNRD(filenum);
	previous=RDATDataNEW(filenum);
	addDFincvir(mtnrsrc,module,"PURSIT",NULL,filenum);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	switch(dowhich)
	{
		case 1:
			ApplyPassKey(MASTER->passkey);
			if(ADVEQLNRDsec(filenum,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(filenum,MASTER->passkey->keyno);
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
			} else {
				filerecord2rsrc(filenum,mtnrsrc);
				GetRDATData(filenum,previous);
			}
			break;
		case 2:
			ApplyPassKey(MASTER->passkey);
			ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
			break;
		default:
		case 0:
			if(ADVEQLNRDKEYsec(filenum,MASTER->passkey->keyno,targetkey,SCRNvirtualSubData,mtnrsrc))
			{
				ZERNRD(filenum);
				ApplyPassKey(MASTER->passkey);
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
			} else {
				filerecord2rsrc(filenum,mtnrsrc);
				GetRDATData(filenum,previous);
			}	
			break;
	}
	if(siteid!=NULL) Rfree(siteid);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	MakeMasterButtons(mtnrsrc,MASTER);
	if(editable)
	{
		addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,previous);
		if(mbl_site!=NULL)
		{
			addrfcbrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
		} else {
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,previous);
		}
	}
	if(mbl_site==NULL)
	{
		addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_site,previous);
	}
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	FINDRSCSETFUNC(mtnrsrc,"[PURSIT][SITE IDENTIFICATION]",getrecord,previous);
	computeallSCRNvirtuals(mtnrsrc);
	APPmakescrn(mtnrsrc,FALSE,quit_record,NULL,(mbl_site==NULL?TRUE:FALSE));
}
static void save_record_up(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check(mtnrsrc,previous,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check(mtnrsrc,previous,FALSE);
}
static void save_check(RDArsrc *mtnrsrc,RDATData *previous,short update_list)
{
	readallwidgets(mtnrsrc);
	save_record(mtnrsrc,previous,update_list);
}
static void save_record(RDArsrc *mtnrsrc,RDATData *previous,short update_list)
{
	char *rcddesc=NULL;

	rsrc2filerecord(filenum,mtnrsrc);
	if(ADVRECORDsec(filenum,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(filenum,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(filenum,0,NULL,previous);
		updatebrowse(update_list,mbl_site,filenum,module,mtnrsrc);
		if(mbl_site!=NULL) quit_record(mtnrsrc,previous);
		else GetRDATData(filenum,previous);
	}
}
static void quit_record(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(previous!=NULL) FreeRDATData(previous);
	if(WindowCount==1)
	{
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
		CLSNRD(filenum);
		ShutdownSubsystems();
		exit(0);
	}
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
}
static void getrecord(RDArsrc *mtnrsrc,RDATData *previous)
{
	char *siteid=NULL;

	readallwidgets(mtnrsrc);
	if(FINDRSCGETSTRING(mtnrsrc,"[PURSIT][SITE IDENTIFICATION]",&siteid)) 
		return;
	FINDFLDSETSTRING(filenum,"SITE IDENTIFICATION",siteid);
	if(siteid!=NULL) Rfree(siteid);
	if(ADVEQLNRDsec(filenum,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc)) 
	{
		KEYNRD(filenum,MASTER->passkey->keyno);
		ClearRDATData(previous);
	} else {
		GetRDATData(filenum,previous);
		filerecord2rsrc(filenum,mtnrsrc);	
	}
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
