/* venadrm.c - Vendor Payments Address Maintain */
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

static void save_check(RDArsrc *,RDATData *,short);
static void save_record(RDArsrc *,RDATData *,short);
static void save_record_quit(RDArsrc *,RDATData *);
static void save_record_up(RDArsrc *,RDATData *);
static void save_record_nup(RDArsrc *,RDATData *);
static void save_record_now(RDArsrc *,RDATData *);
static void save_record_now1(RDArsrc *,RDATData *);
static void quit_record(RDArsrc *,RDATData *);
static void quit_record_test(RDArsrc *,RDATData *);
static void readscreen(RDArsrc *,RDATData *);
static void resetdefs(RDArsrc *,RDATData *);
static void getpreviouscb(RDArmem *,RDATData *);
static void getnextcb(RDArmem *,RDATData *);
static void SETID(RDArsrc *,RDATData *);
static void SETFILES(RDArsrc *,MaintainMaster *);
void venadrm(short,void *targetkey);
extern MakeBrowseList *mbl_venadr;
extern char editable;
extern void browse_venadr(RDArsrc *,RDATData *);
extern MaintainMaster *MASTER;

void venadrm(short dowhich,void *targetkey)
{
	char *name=NULL;
	short edit_rsrc=TRUE,nofields=0,nokeys=0,x=0,y=0;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDArsrc *mtnrsrc=NULL;
	RDATData *previous=NULL;

	mtnrsrc=RDArsrcNEW(MASTER->module,MASTER->maintain_screen);
	ZERNRD(MASTER->passkey->fileno);
	previous=RDATDataNEW(MASTER->passkey->fileno);
	nokeys=NUMKEYS(MASTER->passkey->fileno);
	keys=KEYPOINTER(MASTER->passkey->fileno);
	nofields=NUMFLDS(MASTER->passkey->fileno);
	fields=FLDPOINTER(MASTER->passkey->fileno);
	GET_MAINTAIN_INCVARS(mtnrsrc,MASTER);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=(short)editable;
			nonlstmakefld(mtnrsrc,MASTER->passkey->fileno,fldx,edit_rsrc);
			if(name!=NULL)
			{
				name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(MASTER->mainfile));
			} else {
				name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(MASTER->mainfile));
			}
			sprintf(name,"[%s][%s]",MASTER->mainfile,fldx->name);
			FINDRSCSETFUNC(mtnrsrc,name,SETFILES,MASTER);
		}
	}
	if(keys!=NULL)
	{
		for(x=0,keyx=keys;x<nokeys;++x,++keyx)
		{
			if(keyx->part!=NULL)
			{
				for(y=0,part=keyx->part;y<keyx->numparts;++y,++part)
				{
					if(name!=NULL)
					{
						name=Rrealloc(name,RDAstrlen(part->name)+5+RDAstrlen(MASTER->mainfile));
					} else {
						name=Rmalloc(RDAstrlen(part->name)+5+RDAstrlen(MASTER->mainfile));
					}
					sprintf(name,"[%s][%s]",MASTER->mainfile,part->name);
					FINDRSCSETFUNC(mtnrsrc,name,SETID,previous);
					FINDRSCSETEDITABLE(mtnrsrc,name,TRUE);
				}
			}
		}
	}
	if(name!=NULL) Rfree(name);
	SET_MAINTAIN_FILE2RSRCS(mtnrsrc,MASTER);
	SCRNvirtual2rsrc(mtnrsrc);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			ApplyPassKey(MASTER->passkey);
			if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(MASTER->passkey->fileno,MASTER->passkey->keyno);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
				if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
				} else {
					GetRDATData(MASTER->passkey->fileno,previous);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
			}
			break;
		case 2:
			ApplyPassKey(MASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
			if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
			} else {
				GetRDATData(MASTER->passkey->fileno,previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
			}
			break;
		default:
		case 0:
			if(ADVEQLNRDKEYsec(MASTER->passkey->fileno,MASTER->passkey->keyno,targetkey,SCRNvirtualSubData,mtnrsrc))
			{
				ZERNRD(MASTER->passkey->fileno);
				ApplyPassKey(MASTER->passkey);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
				if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
				} else {
					GetRDATData(MASTER->passkey->fileno,previous);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
			}
			break;
	}
	GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,previous);
	if(mbl_venadr!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,previous);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_venadr,previous);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,previous);
	addbtnrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,previous);
	MakeMasterButtons(mtnrsrc,MASTER);
	computeallSCRNvirtuals(mtnrsrc);
	if(mbl_venadr!=NULL)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	}
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,
		(mbl_venadr==NULL?TRUE:FALSE));
}
static void resetdefs(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(MASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	updateallrsrc(mtnrsrc);
	SETID(mtnrsrc,previous);
}
static void getpreviouscb(RDArmem *member,RDATData *previous)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1))
	{
		keyno=MASTER->passkey->keyno;
	}
	ADV2GET_PREVIOUS(member->parent,MASTER,previous,SCRNvirtualSubData,
		member->parent,keyno,NULL,NULL,
		NULL,NULL,NULL,NULL);
}
static void getnextcb(RDArmem *member,RDATData *previous)
{
        short keyno=(-1);

        keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+5));
        if(keyno==(-1))
        {
                keyno=MASTER->passkey->keyno;
        }
        ADV2GET_NEXT(member->parent,MASTER,previous,SCRNvirtualSubData,
		member->parent,keyno,NULL,NULL,
		NULL,NULL,NULL,NULL);
}
static void SETID(RDArsrc *mtnrsrc,RDATData *previous)
{
	ADV4SET_KEYFUNC(mtnrsrc,MASTER,previous,SCRNvirtualSubData,
		(void *)mtnrsrc,NULL,NULL,
		NULL,NULL,NULL,NULL);
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *MASTER)
{
	SET_MASTER(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
}
static void readscreen(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(MASTER->passkey->fileno);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
}
static void save_record_now(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_record(mtnrsrc,previous,TRUE);
}
static void save_record_now1(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_record(mtnrsrc,previous,FALSE);
}
static void save_check(RDArsrc *mtnrsrc,RDATData *previous,short update)
{
	char *desc=NULL;

	if(!SAVE_CHECK(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc))
	{
		return;
	}
	if(!isEMPTY(MASTER->save_expression))
	{
		if(EVALUATEbol(MASTER->save_expression,SCRNvirtualSubData,mtnrsrc))
		{
			desc=EVALUATEstr(MASTER->save_error_desc,
				SCRNvirtualSubData,mtnrsrc);
			ERRORDIALOG("SAVE ERROR",desc,NULL,FALSE);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	if(!isEMPTY(MASTER->save_warning))
	{
		if(EVALUATEbol(MASTER->save_warning,SCRNvirtualSubData,mtnrsrc))
		{
			desc=EVALUATEstr(MASTER->save_warning_desc,
				SCRNvirtualSubData,mtnrsrc);
			ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN",
				"SAVE WARNING",desc,
				(update?save_record_now:save_record_now1),
				NULL,FALSE,2,mtnrsrc,previous,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	save_record(mtnrsrc,previous,update);
}
static void save_record_up(RDArsrc *mtnrsrc,RDATData *previous)
{
	readscreen(mtnrsrc,previous);
	save_check(mtnrsrc,previous,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,RDATData *previous)
{
	readscreen(mtnrsrc,previous);
	save_check(mtnrsrc,previous,FALSE);
}
static void save_record(RDArsrc *mtnrsrc,RDATData *previous,short update)
{
	char *rcddesc=NULL;

	if(ADVRECORDsec(MASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(MASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE); 
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(MASTER->passkey->fileno,0,NULL,previous);
		DELETE_SUBORDINATES(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update,mbl_venadr,MASTER->passkey->fileno,MASTER->module,mtnrsrc);
		if(mbl_venadr!=NULL)
		{
			quit_record(mtnrsrc,previous);
		} else {
			GetRDATData(MASTER->passkey->fileno,previous);
		}
	}
}
static void quit_record(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(previous!=NULL) FreeRDATData(previous);
	if(WindowCount<2)
	{
		CLOSE_MASTER_FILES(MASTER);
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
static void save_record_quit(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check(mtnrsrc,previous,TRUE);
}
static void quit_record_test(RDArsrc *mtnrsrc,RDATData *previous)
{
	QUIT_RECORD_TEST(mtnrsrc,MASTER->passkey->fileno,previous,readscreen,
		save_record_quit,quit_record,previous);
}
