/* restclsm.c - Real Estate Class Maintain Screen */
#include <stdio.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <alloc.h>
#include <gui.h>
#include <nrd.h>
#include <trans.h>
#include <sec.h>
#include <mix.h>
#include <fin.h>

extern short filenum;
extern char editable;
extern char *module;
extern void browse_msr(RDArsrc *,RDATData *);
static void save_record(RDArsrc *,RDATData *,short);
static void save_record_up(RDArsrc *,RDATData *);
static void save_record_nup(RDArsrc *,RDATData *);
static void quit_record(RDArsrc *,RDATData *);
static void quit_record_test(RDArsrc *,RDATData *);
static void save_record_quit(RDArsrc *,RDATData *);
static void readscreen(RDArsrc *,RDATData *);
static void getrecord(RDArsrc *,RDATData *previous);
MakeBrowseList *mbl_restclas=NULL;
extern MaintainMaster *MASTER;
void restclasm(short,void *targetkey);
static char *files2delete[3]={"RESTCLML","RESTINT","RESTPEN"};

static void save_record_quit(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_record(mtnrsrc,previous,TRUE);
}
static void quit_record_test(RDArsrc *mtnrsrc,RDATData *previous)
{
	char *desc=NULL;

	readscreen(mtnrsrc,previous);
	if(QUITvfy(filenum,previous))
	{
		desc=QUITvfydesc(filenum,previous);
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!",
			desc,save_record_quit,quit_record,FALSE,2,mtnrsrc,previous,NULL);
		if(desc!=NULL) Rfree(desc);
	} else {
		quit_record(mtnrsrc,previous);
	}
}
void restclasm(short dowhich,void *targetkey)
{
	RDArsrc *mtnrsrc=NULL;
	RDATData *previous=NULL;
	
	mtnrsrc=RDArsrcNEW(module,MASTER->maintain_screen);
	addDFincvir(mtnrsrc,module,"RESTCLAS",NULL,filenum);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	ZERNRD(filenum);
	previous=RDATDataNEW(filenum);
	file2rsrc(filenum,mtnrsrc,editable);
	FINDRSCSETFUNC(mtnrsrc,"[RESTCLAS][CLASS IDENTIFICATION]",getrecord,
		previous);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			ApplyPassKey(MASTER->passkey);
			if(ADVEQLNRDsec(filenum,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(filenum,MASTER->passkey->keyno);
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
			} else {
				GetRDATData(filenum,previous);
				filerecord2rsrc(filenum,mtnrsrc);
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
				GetRDATData(filenum,previous);
				filerecord2rsrc(filenum,mtnrsrc);
			}
			break;
	}
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	if(editable)
	{
		addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,previous);
		if(mbl_restclas!=NULL)
		{
			addrfcbrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
		} else {
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
		}
	}
	if(mbl_restclas==NULL)
	{
		addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_msr,previous);
	}
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakeMasterButtons(mtnrsrc,MASTER);
	computeallSCRNvirtuals(mtnrsrc);
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,
		(mbl_restclas==NULL?TRUE:FALSE));
}
static void save_record_up(RDArsrc *mtnrsrc,RDATData *previous)
{
	readscreen(mtnrsrc,previous);
	save_record(mtnrsrc,previous,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,RDATData *previous)
{
	readscreen(mtnrsrc,previous);
	save_record(mtnrsrc,previous,FALSE);
}
static void readscreen(RDArsrc *mtnrsrc,RDATData *previous)
{
	readallwidgets(mtnrsrc);
	rsrc2filerecord(filenum,mtnrsrc);
}
static void save_record(RDArsrc *mtnrsrc,RDATData *previous,
	short update_list)
{
	char *rcddesc=NULL,*classid1=NULL,*classid2=NULL;
	char deleteflag=0,mdeleteflag=0;
	short tmpnum=(-1),x;
	short ef=0;
	RDATData *prev=NULL;
	RDArsrc *tmprsrc=NULL;

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
		FINDFLDGETCHAR(filenum,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			FINDFLDGETSTRING(filenum,"CLASS IDENTIFICATION",&classid1);
			for(x=0;x<3;++x)
			{
				tmpnum=OPNNRDsec(module,files2delete[x],TRUE,TRUE,FALSE);	
				tmprsrc=RDArsrcNEW(module,NULL);
				addDFincvir(tmprsrc,module,"RESTCLAS",NULL,filenum);
				addDFincvir(tmprsrc,module,files2delete[x],NULL,tmpnum);
				GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
				if(tmpnum!=(-1))
				{
					ZERNRD(tmpnum);
					FINDFLDSETSTRING(tmpnum,"CLASS IDENTIFICATION",classid1);
					ef=ADVGTENRDsec(tmpnum,1,SCRNvirtualSubData,tmprsrc);
					while(!ef)
					{
						FINDFLDGETCHAR(tmpnum,"DELETEFLAG",&mdeleteflag);
						if(!mdeleteflag)
						{
							FINDFLDGETSTRING(tmpnum,"CLASS IDENTIFICATION",&classid2);
							if(RDAstrcmp(classid1,classid2)) break;
							prev=RDATDataNEW(tmpnum);
							FINDFLDSETCHAR(tmpnum,"DELETEFLAG",deleteflag);
							ADVWRTTRANSsec(tmpnum,0,NULL,prev,SCRNvirtualSubData,tmprsrc);		
							if(prev!=NULL) FreeRDATData(prev);
						}
						ef=ADVNXTNRDsec(tmpnum,1,SCRNvirtualSubData,tmprsrc);
						if(classid2!=NULL) { Rfree(classid2); classid2=NULL; } 
					}
				}
				if(tmpnum!=(-1)) CLSNRD(tmpnum);
				if(tmprsrc!=NULL) free_rsrc(tmprsrc);
			}
		}
		if(classid1!=NULL) Rfree(classid1);
		if(classid2!=NULL) Rfree(classid2);
		updatebrowse(update_list,mbl_restclas,filenum,module,mtnrsrc);
		if(mbl_restclas!=NULL) quit_record(mtnrsrc,previous);
			else GetRDATData(filenum,previous);
	}
}
static void quit_record(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(previous!=NULL) FreeRDATData(previous);
	if(WindowCount==1)
	{
		CLSNRD(filenum);
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
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
	char *classid1=NULL;

	readwidget(mtnrsrc,"[RESTCLAS][CLASS IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[RESTCLAS][CLASS IDENTIFICATION]",&classid1);
	FINDFLDSETSTRING(filenum,"CLASS IDENTIFICATION",classid1);
	if(ADVEQLNRDsec(filenum,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc)) 
	{
		KEYNRD(filenum,MASTER->passkey->keyno);
		ClearRDATData(previous);
	} else {
		GetRDATData(filenum,previous);
		updatefilerecord2rsrc(filenum,mtnrsrc);	
	}
	if(classid1!=NULL) Rfree(classid1);
	updateSCRNvirtuals(mtnrsrc);
}
