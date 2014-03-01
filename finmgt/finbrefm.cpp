/* finbrefm.c - Balance Sheet Reference Maintain Screen */
#include <stdio.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <alloc.h>
#include <gui.h>
#include <nrd.h>
#include <trans.h>
#include <sec.h>
#include <mix.h>

struct mtnstructs
{
	APPlib *BREFtypes;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern MakeBrowseList *mbl_finbref;
extern MaintainMaster *MASTER;
extern void browse_finbref(RDArsrc *,mtnstruct *);
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_check(RDArsrc *,mtnstruct *,short);
static void save_record_now(RDArsrc *,mtnstruct *);
static void save_record_now1(RDArsrc *,mtnstruct *);
static void save_record_quit(RDArsrc *,mtnstruct *);
static void SETID(RDArsrc *,mtnstruct *);
static void SETFILES(RDArsrc *,MaintainMaster *);
static void resetdefs(RDArsrc *,mtnstruct *);
static void getpreviouscb(RDArmem *,mtnstruct *);
static void getnextcb(RDArmem *,mtnstruct *);
static void quit_record(RDArsrc *,mtnstruct *);
static void quit_record_test(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void doBREFlist(RDArsrc *,mtnstruct *);
static void setBREFtype(RDArsrc *,mtnstruct *,short);
static void setBREFtypecb(RDArsrc *,MaintainMaster *,void (*)(),void *,mtnstruct *);
static void changeBREFtypecb(RDArsrc *,mtnstruct *);
static void readscreen(RDArsrc *,mtnstruct *);
static void list2file(RDArsrc *,mtnstruct *);
void finbrefm(short,void *);
char editable=TRUE;

void finbrefm(short dowhich,void *targetkey)
{
	int which_type=0;
	char *name=NULL;
	short nofields=0,nokeys=0,x=0,y=0,edit_rsrc=TRUE;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDArsrc *mtnrsrc=NULL;
	mtnstruct *mtn=NULL;

	mtnrsrc=RDArsrcNEW(MASTER->module,MASTER->maintain_screen);
	ZERNRD(MASTER->passkey->fileno);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->BREFtypes=NULL;
	doBREFlist(mtnrsrc,mtn);
	mtn->previous=NULL;
	mtn->previous=RDATDataNEW(MASTER->passkey->fileno);
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
			if(!RDAstrcmp(fldx->name,"REFERENCE TYPE"))
			{
				if(FIELDscrn(MASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					addlstrsrc(mtnrsrc,"REFERENCE TYPES",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						changeBREFtypecb,
						mtn->BREFtypes->numlibs,
						&mtn->BREFtypes->libs,mtn);
					addstdrsrc(mtnrsrc,"[FINBREF][REFERENCE TYPE]",VARIABLETEXT,0,NULL,(edit_rsrc==TRUE?TRUE:FALSE));
				}
			} else {
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
					FINDRSCSETFUNC(mtnrsrc,name,SETID,mtn);
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
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
					setBREFtype(mtnrsrc,mtn,FALSE);
				}	
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				setBREFtype(mtnrsrc,mtn,FALSE);
			}	
			break;
		case 2:
			ApplyPassKey(MASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
			if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				setBREFtype(mtnrsrc,mtn,FALSE);
			}	
			break;
		default:
		case 0:
			if(EQLNRDKEYsec(MASTER->passkey->fileno,MASTER->passkey->keyno,targetkey))
			{
				ZERNRD(MASTER->passkey->fileno);
				ApplyPassKey(MASTER->passkey);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
				if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
				} else {
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
					setBREFtype(mtnrsrc,mtn,FALSE);
				}	
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				setBREFtype(mtnrsrc,mtn,FALSE);
			}
			break;
	}
	GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	MSTSetVirFieldFuncs(mtnrsrc,MASTER);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,mtn);
	MakeMasterButtons(mtnrsrc,MASTER);
	MSTcomputeallSCRNvirtuals(mtnrsrc,MASTER);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
	if(mbl_finbref!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_finbref,mtn);
	if(mbl_finbref!=NULL)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	}
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,mtn);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_finbref==NULL?TRUE:FALSE));
}
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MASTER->passkey->fileno);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
	list2file(mtnrsrc,mtn);
}
static void save_record_now(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_record(mtnrsrc,mtn,TRUE);
}
static void save_record_now1(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_record(mtnrsrc,mtn,FALSE);
}
static void save_record_up(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,FALSE);
}
static void save_check(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	char *desc=NULL;

	readscreen(mtnrsrc,mtn);
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
				(update_list?save_record_now:save_record_now1),
				NULL,FALSE,2,mtnrsrc,mtn,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	save_record(mtnrsrc,mtn,update_list);
}
static void save_record(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
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
		WRTTRANS(MASTER->passkey->fileno,0,NULL,mtn->previous);
		DELETE_SUBORDINATES(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update_list,mbl_finbref,MASTER->passkey->fileno,MASTER->module,mtnrsrc);
		if(mbl_finbref!=NULL) quit_record(mtnrsrc,mtn);
		else GetRDATData(MASTER->passkey->fileno,mtn->previous);
	}
}
static void quit_record_test(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	QUIT_RECORD_TEST(mtnrsrc,MASTER->passkey->fileno,mtn->previous,readscreen,
		save_record_quit,quit_record,mtn);
}
static void save_record_quit(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,FALSE);
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->BREFtypes!=NULL) freeapplib(mtn->BREFtypes);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
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
static void setBREFtypecb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
	void (*SubFunc)(),void *args,mtnstruct *mtn)
{
	setBREFtype(mtnrsrc,mtn,TRUE);
}
static void resetdefs(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	MSTcomputeallSCRNvirtuals(mtnrsrc,MASTER);
	updateallrsrc(mtnrsrc);
	SETID(mtnrsrc,mtn);
}
static void getpreviouscb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1))
	{
		keyno=MASTER->passkey->keyno;
	}
	ADV2GET_PREVIOUS(member->parent,MASTER,mtn->previous,SCRNvirtualSubData,
		member->parent,keyno,setBREFtypecb,
		mtn,setBREFtypecb,mtn,NULL,NULL);
}
static void getnextcb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1))
	{
		keyno=MASTER->passkey->keyno;
	}
	ADV2GET_NEXT(member->parent,MASTER,mtn->previous,SCRNvirtualSubData,
		member->parent,keyno,setBREFtypecb,
		mtn,setBREFtypecb,mtn,NULL,NULL);
}
static void SETID(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ADV4SET_KEYFUNC(mtnrsrc,MASTER,mtn->previous,SCRNvirtualSubData,
		(void *)mtnrsrc,setBREFtypecb,mtn,
		setBREFtypecb,mtn,NULL,NULL);
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *MASTER)
{
	SET_MASTER(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
}
static void list2file(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected=0;

	FINDRSCGETINT(mtnrsrc,"REFERENCE TYPES",&selected);
	if(RDAstrcmp(mtn->BREFtypes->libs[selected],"No References Available"))
	{
		FINDFLDSETSTRING(MASTER->passkey->fileno,"REFERENCE TYPE",mtn->BREFtypes->libs[selected]);
	}
}
static void setBREFtype(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	int x=0;
	char *bref=NULL;

	FINDFLDGETSTRING(MASTER->passkey->fileno,"REFERENCE TYPE",&bref);
	if(bref!=NULL && RDAstrlen(bref)>0)
	{
		for(x=0;x<mtn->BREFtypes->numlibs;++x)
		{
			if(!RDAstrcmp(bref,mtn->BREFtypes->libs[x])) break;
		}
		if(x>=mtn->BREFtypes->numlibs)
		{
			addAPPlib(mtn->BREFtypes,bref);
			SORTAPPlib(mtn->BREFtypes);
			for(x=0;x<mtn->BREFtypes->numlibs;++x)
			{
				if(!RDAstrcmp(bref,mtn->BREFtypes->libs[x])) break;
			}
			if(!FINDRSCLISTAPPlib(mtnrsrc,"REFERENCE TYPES",x,mtn->BREFtypes))
			{
				if(update_list)
				{
					updatersrc(mtnrsrc,"REFERENCE TYPES");
				}
			}
		} else {
			if(!FINDRSCSETINT(mtnrsrc,"REFERENCE TYPES",x))
			{
				if(update_list)
				{
					updatersrc(mtnrsrc,"REFERENCE TYPES");
				}
			}
		}
	}
	if(bref!=NULL) Rfree(bref);
}
void changeBREFtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selBREFtype=0;
	char *bref=NULL;

	if(FINDRSCGETINT(mtnrsrc,"REFERENCE TYPES",&selBREFtype)) return;
	if(RDAstrcmp(mtn->BREFtypes->libs[selBREFtype],"No References Available"))
	{
		bref=stralloc(mtn->BREFtypes->libs[selBREFtype]);
	}
	if(FINDRSCSETSTRING(mtnrsrc,"[FINBREF][REFERENCE TYPE]",bref)) return;
	updatersrc(mtnrsrc,"[FINBREF][REFERENCE TYPE]");
	if(bref!=NULL) Rfree(bref);
	SETID(mtnrsrc,mtn);
}
static void doBREFlist(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char match=FALSE;
	int x=0;
	short ef=0;
	char delflag=FALSE,*bref=NULL;

	if(mtn->BREFtypes!=NULL) freeapplib(mtn->BREFtypes);
	mtn->BREFtypes=APPlibNEW();
	addAPPlib(mtn->BREFtypes,"APPROPRIATION");
	addAPPlib(mtn->BREFtypes,"EXPENDITURE");
	addAPPlib(mtn->BREFtypes,"ENCUMBRANCE");
	addAPPlib(mtn->BREFtypes,"RESERVE FOR ENCUMBRANCE");
	addAPPlib(mtn->BREFtypes,"REVENUE");
	addAPPlib(mtn->BREFtypes,"ESTIMATED REVENUE");
	addAPPlib(mtn->BREFtypes,"CASH");
	addAPPlib(mtn->BREFtypes,"ACCOUNTS PAYABLE");
	addAPPlib(mtn->BREFtypes,"ACCOUNTS RECEIVABLE");
	addAPPlib(mtn->BREFtypes,"SALARIES / BENEFITS PAYABLE");
	addAPPlib(mtn->BREFtypes,"DISCOUNTS ALLOWED");
	addAPPlib(mtn->BREFtypes,"DEPOSITS PAYABLE");
	addAPPlib(mtn->BREFtypes,"UNRESERVED FUND BALANCE");
	addAPPlib(mtn->BREFtypes,"FIXED ASSETS");
	addAPPlib(mtn->BREFtypes,"INVESTMENT FIXED ASSETS");
	addAPPlib(mtn->BREFtypes,"DUE FROM");
	addAPPlib(mtn->BREFtypes,"DUE TO");
	ZERNRD(MASTER->passkey->fileno);
	ef=ADVFRSNRDsec(MASTER->passkey->fileno,1,SCRNvirtualSubData,mtnrsrc);
	while(!ef)
	{
		FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&delflag);
		if(delflag==FALSE)
		{
			FINDFLDGETSTRING(MASTER->passkey->fileno,"REFERENCE TYPE",&bref);
			mtn->BREFtypes=mtn->BREFtypes;
			for(x=0,match=FALSE;x<mtn->BREFtypes->numlibs;x++)
			{
				if(!RDAstrcmp(bref,mtn->BREFtypes->libs[x]))
				{
					match=TRUE;
				}
			}
			if(match==FALSE)
				addAPPlib(mtn->BREFtypes,bref);
		}
		ef=ADVNXTNRDsec(MASTER->passkey->fileno,1,SCRNvirtualSubData,mtnrsrc);
	}
	SORTAPPlib(mtn->BREFtypes);
	if(bref!=NULL) Rfree(bref);
}
