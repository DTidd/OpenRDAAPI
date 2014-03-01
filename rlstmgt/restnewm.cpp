/* restnewm.c - RESTNEW Maintain Screen */
#include <cstdio>


#include <mix.hpp>

extern char editable;
extern void browse_utl(RDArsrc *,RDATData *);
static void save_check(RDArsrc *,RDATData *,short);
static void save_record(RDArsrc *,RDATData *,short);
static void save_record_up(RDArsrc *,RDATData *);
static void save_record_nup(RDArsrc *,RDATData *);
static void quit_record(RDArsrc *,RDATData *);
static void quit_record_test(RDArsrc *,RDATData *);
static void readscreen(RDArsrc *,RDATData *);
MakeBrowseList *mbl_utlmstr=NULL;
extern MaintainMaster *MTNMASTER;
void mtnutlmstr(short,void *targetkey);

static void resetdefs(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(MTNMASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	ApplyActions(mtnrsrc,MTNMASTER,SCRNvirtualSubData,mtnrsrc,TRUE);
	updateallrsrc(mtnrsrc);
	SET_KEYFUNC(mtnrsrc,MTNMASTER,previous,SCRNvirtualSubData,(void *)mtnrsrc,
		MTNMASTER->inheritrsrcs);
}
static void getpreviouscb(RDArmem *member,RDATData *previous)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MTNMASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1))
	{
		keyno=MTNMASTER->passkey->keyno;
	}
	GET_PREVIOUS(member->parent,MTNMASTER,previous,SCRNvirtualSubData,
		member->parent,keyno);
}
static void getnextcb(RDArmem *member,RDATData *previous)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MTNMASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1))
	{
		keyno=MTNMASTER->passkey->keyno;
	}
	GET_NEXT(member->parent,MTNMASTER,previous,SCRNvirtualSubData,
		member->parent,keyno);
}
static void SETID(RDArsrc *mtnrsrc,RDATData *previous)
{
	SET_KEYFUNC(mtnrsrc,MTNMASTER,previous,SCRNvirtualSubData,(void *)mtnrsrc,
		MTNMASTER->inheritrsrcs);
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER) 
{
	SET_MTNMASTER(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc);
}
void mtnutlmstr(short dowhich,void *targetkey)
{
	char *name=NULL;
	short edit_rsrc=TRUE,nofields=0,nokeys=0,x=0,y=0;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDArsrc *mtnrsrc=NULL;
	RDATData *previous=NULL;
	
	mtnrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->maintain_screen);
	ZERNRD(MTNMASTER->passkey->fileno);
	previous=RDATDataNEW(MTNMASTER->passkey->fileno);
	nokeys=NUMKEYS(MTNMASTER->passkey->fileno);
	keys=KEYPOINTER(MTNMASTER->passkey->fileno);
	nofields=NUMFLDS(MTNMASTER->passkey->fileno);
	fields=FLDPOINTER(MTNMASTER->passkey->fileno);
	GET_MAINTAIN_INCVARS(mtnrsrc,MTNMASTER);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=(short)editable;
			nonlstmakefld(mtnrsrc,MTNMASTER->passkey->fileno,fldx,edit_rsrc);
			if(name!=NULL) 
			{ 
				name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(MTNMASTER->mainfile));
			} else {
				name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(MTNMASTER->mainfile));
			}
			sprintf(name,"[%s][%s]",MTNMASTER->mainfile,fldx->name);
			FINDRSCSETFUNC(mtnrsrc,name,SETFILES,MTNMASTER);
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
						name=Rrealloc(name,RDAstrlen(part->name)+5+RDAstrlen(MTNMASTER->mainfile));
					} else {
						name=Rmalloc(RDAstrlen(part->name)+5+RDAstrlen(MTNMASTER->mainfile));
					}
					sprintf(name,"[%s][%s]",MTNMASTER->mainfile,part->name);
					FINDRSCSETFUNC(mtnrsrc,name,SETID,previous);
					FINDRSCSETEDITABLE(mtnrsrc,name,TRUE);
				}
			}
		}
	}
	if(name!=NULL) Rfree(name);
	SET_MAINTAIN_FILE2RSRCS(mtnrsrc,MTNMASTER);
	SCRNvirtual2rsrc(mtnrsrc);
	DefaultScreens(mtnrsrc);
	ApplyActions(mtnrsrc,MTNMASTER,SCRNvirtualSubData,mtnrsrc,TRUE);
	switch(dowhich)
	{
		case 1:
			ApplyPassKey(MTNMASTER->passkey);
			if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
				if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
				} else {
					GetRDATData(MTNMASTER->passkey->fileno,previous);
					filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
				}
			} else {
				GetRDATData(MTNMASTER->passkey->fileno,previous);
				filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
			}
			break;
		case 2:
			ApplyPassKey(MTNMASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
			if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
			} else {
				GetRDATData(MTNMASTER->passkey->fileno,previous);
				filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
			}
			break;
		default:
		case 0:
			if(ADVEQLNRDKEYsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,targetkey,SCRNvirtualSubData,mtnrsrc))
			{
				ZERNRD(MTNMASTER->passkey->fileno);
				ApplyPassKey(MTNMASTER->passkey);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
				if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
				} else {
					GetRDATData(MTNMASTER->passkey->fileno,previous);
					filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
				}
			} else {
				GetRDATData(MTNMASTER->passkey->fileno,previous);
				filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
			}
			break;
	}
	GET_SUPPORTING(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,previous);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,previous);
	if(mbl_utlmstr!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,previous);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_utl,previous);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,previous);
	MakeMasterButtons(mtnrsrc,MTNMASTER);
	computeallSCRNvirtuals(mtnrsrc);
	if(mbl_utlmstr!=NULL)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	}
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,
		(mbl_utlmstr==NULL?TRUE:FALSE));
	ApplyActions(mtnrsrc,MTNMASTER,SCRNvirtualSubData,mtnrsrc,TRUE);
}
static void readscreen(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(MTNMASTER->passkey->fileno);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MTNMASTER->passkey->fileno,mtnrsrc);
	ApplyActions(mtnrsrc,MTNMASTER,SCRNvirtualSubData,mtnrsrc,TRUE);
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

	if(!SAVE_CHECK(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc))
	{
		return;
	}
	if(!isEMPTY(MTNMASTER->save_expression))
	{
		if(EVALUATEbol(MTNMASTER->save_expression,SCRNvirtualSubData,mtnrsrc))
		{
			desc=EVALUATEstr(MTNMASTER->save_error_desc,
				SCRNvirtualSubData,mtnrsrc);
			ERRORDIALOG("SAVE ERROR",desc,NULL,FALSE);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	if(!isEMPTY(MTNMASTER->save_warning))
	{
		if(EVALUATEbol(MTNMASTER->save_warning,SCRNvirtualSubData,mtnrsrc))
		{
			desc=EVALUATEstr(MTNMASTER->save_warning_desc,
				SCRNvirtualSubData,mtnrsrc);
			ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","SAVE WARNING",
				desc,(update ? save_record_now:save_record_now1),
				NULL,FALSE,2,mtnrsrc,previous,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	save_record(mtnrsrc,previous,update);
}
static void save_record_up(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check(mtnrsrc,previous,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check(mtnrsrc,previous,FALSE);
}
static void save_record(RDArsrc *mtnrsrc,RDATData *previous,short update_list)
{
	char *rcddesc=NULL;
	char deleteflag=FALSE,*realid=NULL,*realid1=NULL;
	char *ownerid=NULL,*ownerid1=NULL;
	short ef=FALSE;
	RDATData *prev=NULL;

	readscreen(mtnrsrc,previous);
	if(ADVRECORDsec(MTNMASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(MTNMASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE); 
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		FINDFLDGETSTRING(MTNMASTER->passkey->fileno,"REAL ESTATE IDENTIFICATION",&realid);
		FINDFLDGETSTRING(MTNMASTER->passkey->fileno,"OWNER IDENTIFICATION",&ownerid);
		ZERNRD(MTNMASTER->passkey->fileno);
		FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"REAL ESTATE IDENTIFICATION",realid);
		ef=ADVGTENRDsec(MTNMASTER->passkey->fileno,1,SCRNvirtualSubData,
			mtnrsrc);
		while(!ef)
		{
			FINDFLDGETSTRING(MTNMASTER->passkey->fileno,
				"REAL ESTATE IDENTIFICATION",&realid1);
			if(RDAstrcmp(realid,realid1)) break;
			FINDFLDGETCHAR(MTNMASTER->passkey->fileno,"DELETEFLAG",&deleteflag);
			FINDFLDGETSTRING(MTNMASTER->passkey->fileno,"OWNER IDENTIFICATION",&ownerid1);
			if(!deleteflag && RDAstrcmp(ownerid,ownerid1))
			{
				prev=RDATDataNEW(MTNMASTER->passkey->fileno);
				deleteflag=TRUE;
				FINDFLDSETCHAR(MTNMASTER->passkey->fileno,
					"DELETEFLAG",deleteflag);
				ADVWRTTRANSsec(MTNMASTER->passkey->fileno,0,NULL,prev,
					SCRNvirtualSubData,mtnrsrc);
				if(prev!=NULL) FreeRDATData(prev);
			}
			ef=ADVNXTNRDsec(MTNMASTER->passkey->fileno,1,
				SCRNvirtualSubData,mtnrsrc);
		}
		if(realid!=NULL) Rfree(realid);
		if(realid1!=NULL) Rfree(realid1);
		if(ownerid!=NULL) Rfree(ownerid);
		if(ownerid1!=NULL) Rfree(ownerid1);
		readscreen(mtnrsrc,previous);
		WRTTRANS(MTNMASTER->passkey->fileno,0,NULL,previous);
		DELETE_SUBORDINATES(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update_list,mbl_utlmstr,MTNMASTER->passkey->fileno,MTNMASTER->module,mtnrsrc);
		if(mbl_utlmstr!=NULL) quit_record(mtnrsrc,previous);
			else GetRDATData(MTNMASTER->passkey->fileno,previous);
	}
}
static void quit_record(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(previous!=NULL) FreeRDATData(previous);
	if(WindowCount<2)
	{
		CLOSE_MASTER_FILES(MTNMASTER);
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
void save_record_quit(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check(mtnrsrc,previous,TRUE);
}
static void quit_record_test(RDArsrc *mtnrsrc,RDATData *previous)
{
	QUIT_RECORD_TEST(mtnrsrc,MTNMASTER->passkey->fileno,previous,readscreen,
		save_record_quit,quit_record,previous);
}
