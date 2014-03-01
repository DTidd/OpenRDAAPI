/* bftddacm.c - Xpert Benefit Definition Type Maintain Screen */
#include <cstdio>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>

struct mtnstructs
{
	APPlib *DEFtypes;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern char editable;
extern MakeBrowseList *mbl_bftddac;
extern MaintainMaster *MTNMASTER;
extern void browse_bftddac(RDArsrc *,mtnstruct *);
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_record_now(RDArsrc *,mtnstruct *);
static void save_record_quit(RDArsrc *,mtnstruct *);
static void save_check(RDArsrc *,mtnstruct *,short);
static void save_record_up(RDArsrc *,mtnstruct *);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void quit_record(RDArsrc *,mtnstruct *);
static void quit_record_test(RDArsrc *,mtnstruct *);
static void DEFtypecb(RDArsrc *,mtnstruct *);
static void readscreen(RDArsrc *,mtnstruct *);
static void list2file(RDArsrc *,mtnstruct *);
static void setlist(RDArsrc *,mtnstruct *,short);
static void setfiles(RDArsrc *,mtnstruct *);
static void getrecordcb(RDArsrc *,mtnstruct *);
static void resetdefs(RDArsrc *,mtnstruct *);
static void getpreviouscb(RDArmem *,mtnstruct *);
static void getnextcb(RDArmem *,mtnstruct *);
void bftddacm(short,void *);

static void DEFtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	getrecordcb(mtnrsrc,mtn);
}
void bftddacm(short dowhich,void *targetkey)
{
	char *name=NULL;
	int which_type=0;
	short nofields=0,nokeys=0,x=0,y=0,edit_rsrc=FALSE;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDAacct *acct=NULL;
	mtnstruct *mtn=NULL;
	RDArsrc *mtnrsrc=NULL;
	
	mtnrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->maintain_screen);
	ZERNRD(MTNMASTER->passkey->fileno);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->DEFtypes=NULL;
	mtn->previous=NULL;
	mtn->previous=RDATDataNEW(MTNMASTER->passkey->fileno);
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
			edit_rsrc=editable;
			if(!RDAstrcmp(fldx->name,"DEFINITION TYPE"))
			{
				if(FIELDscrn(MTNMASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					mtn->DEFtypes=APPlibNEW();
					if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
					{
						for(y=0,acct=BALANCE_SHEET_ACCOUNT->codetype;
							y<BALANCE_SHEET_ACCOUNT->num;++y,++acct)
						{
							addAPPlib(mtn->DEFtypes,acct->name);
						}
					}
					if(mtn->DEFtypes->numlibs<1)
					{
						addAPPlib(mtn->DEFtypes,"No Definition Types");
					}
					which_type=0;
					addlstrsrc(mtnrsrc,"DEFINITION TYPES",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						DEFtypecb,mtn->DEFtypes->numlibs,
						&mtn->DEFtypes->libs,mtn);
				}
			} else if(!RDAstrcmp(fldx->name,"PROCESSED BALANCE"))
			{
				if(FIELDscrn(MTNMASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					makefieldrsrc(mtnrsrc,
						"[BFTDDAC][PROCESSED BALANCE]",
					fldx->type,fldx->len,FALSE);
				}
			} else if(!RDAstrcmp(fldx->name,"UNPROCESSED BALANCE"))
			{
				if(FIELDscrn(MTNMASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					makefieldrsrc(mtnrsrc,
						"[BFTDDAC][UNPROCESSED BALANCE]",
						fldx->type,fldx->len,FALSE);
				}
			} else {
				nonlstmakefld(mtnrsrc,MTNMASTER->passkey->fileno,fldx,edit_rsrc);
				if(name!=NULL)
				{
					name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(MTNMASTER->mainfile));
				} else {
					name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(MTNMASTER->mainfile));
				}
				sprintf(name,"[%s][%s]",MTNMASTER->mainfile,fldx->name);
				FINDRSCSETFUNC(mtnrsrc,name,setfiles,mtn);
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
						name=Rrealloc(name,RDAstrlen(part->name)+5+RDAstrlen(MTNMASTER->mainfile));
					} else {
						name=Rmalloc(RDAstrlen(part->name)+5+RDAstrlen(MTNMASTER->mainfile));
					}
					sprintf(name,"[%s][%s]",MTNMASTER->mainfile,part->name);
					FINDRSCSETFUNC(mtnrsrc,name,getrecordcb,mtn);
				}
			}
		}
	}
	if(name!=NULL) Rfree(name);
	SET_MAINTAIN_FILE2RSRCS(mtnrsrc,MTNMASTER);
	SCRNvirtual2rsrc(mtnrsrc);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			ApplyPassKey(MTNMASTER->passkey);
			if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
				list2file(mtnrsrc,mtn);
				if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
				} else {
					GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
					filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
					setlist(mtnrsrc,mtn,FALSE);
				}
			} else {
				GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
				setlist(mtnrsrc,mtn,FALSE);
			}
			break;
		case 2:
			ApplyPassKey(MTNMASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
			list2file(mtnrsrc,mtn);
			if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
			} else {
				GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
				setlist(mtnrsrc,mtn,FALSE);
			}
			break;
		case 0:
		default:
			if(ADVEQLNRDKEYsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,targetkey,SCRNvirtualSubData,mtnrsrc))
			{
				ZERNRD(MTNMASTER->passkey->fileno);
				ApplyPassKey(MTNMASTER->passkey);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
				list2file(mtnrsrc,mtn);
				if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
				} else {
					GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
					filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
					setlist(mtnrsrc,mtn,FALSE);
				}
			} else {
				GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
				setlist(mtnrsrc,mtn,FALSE);
			}	
			break;
	}
	GET_SUPPORTING(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
	if(mbl_bftddac!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_bftddac,mtn);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,mtn);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,mtn);
	MakeMasterButtons(mtnrsrc,MTNMASTER);
	computeallSCRNvirtuals(mtnrsrc);
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	if(mbl_bftddac!=NULL) FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_bftddac==NULL?TRUE:FALSE));
}
static void resetdefs(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MTNMASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	updateallrsrc(mtnrsrc);
	getrecordcb(mtnrsrc,mtn);
}
static void save_record_now(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_record(mtnrsrc,mtn,TRUE);
}
static void save_record_now1(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_record(mtnrsrc,mtn,FALSE);
}
static void setlist(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	int x;
	short d;

	FINDFLDGETSHORT(MTNMASTER->passkey->fileno,"DEFINITION TYPE",&d);
	x=d;
	if(mtn->DEFtypes->numlibs<=x)
	{
		x=0;
		d=0;
	}
	FINDRSCSETINT(mtnrsrc,"DEFINITION TYPES",x);
	if(update) updatersrc(mtnrsrc,"DEFINITION TYPES");
}
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MTNMASTER->passkey->fileno);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MTNMASTER->passkey->fileno,mtnrsrc);
	list2file(mtnrsrc,mtn);
}
static void list2file(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected=0;
	short type=0;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selected);
       	if(RDAstrcmp(mtn->DEFtypes->libs[selected],"No Definition Types"))
	{
		type=(short)selected;
	}
	FINDFLDSETSHORT(MTNMASTER->passkey->fileno,"DEFINITION TYPE",type);
}
static void save_record_up(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readscreen(mtnrsrc,mtn);
	save_check(mtnrsrc,mtn,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readscreen(mtnrsrc,mtn);
	save_check(mtnrsrc,mtn,FALSE);
}
static void save_check(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	int seldeftype=0;
	char delflag=FALSE;
	char *desc=NULL;

	FINDRSCGETCHAR(mtnrsrc,"[BFTDDAC][DELETEFLAG]",&delflag);
	if(!delflag)
	{
        	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&seldeftype);
        	if(!RDAstrcmp(mtn->DEFtypes->libs[seldeftype],"No Definition Types"))
        	{
			desc=Rmalloc(250);
			sprintf(desc,"There is not a valid DEFINITION TYPE available for selection.  Before this Transaction can be saved there must be a valid DEFINITION TYPE available for selection.  Check to see if the  BALANCE SHEET ACCOUNT CODE DEFINITION has at least one DEFINITION TYPE available.");
                	WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEFINITION TYPE!",desc,NULL,NULL,FALSE,NULL);
			prterr(desc);
			if(desc!=NULL) Rfree(desc);
                	return;
		}
	}
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
				desc,(update_list ? save_record_now:save_record_now1),
				NULL,FALSE,2,mtnrsrc,mtn->previous,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	save_record(mtnrsrc,mtn,update_list);
}
static void save_record(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	char *rcddesc=NULL;

	if(ADVRECORDsec(MTNMASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(MTNMASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
			prterr("Security Access Deined [%s].  Didn't write record.",rcddesc);
			Rfree(rcddesc);
		} else {
			prterr("Didn't write record.");
		}
	} else {
		WRTTRANS(MTNMASTER->passkey->fileno,0,NULL,mtn->previous);
		DELETE_SUBORDINATES(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update_list,mbl_bftddac,MTNMASTER->passkey->fileno,MTNMASTER->module,mtnrsrc);
		if(mbl_bftddac!=NULL) quit_record(mtnrsrc,mtn);
		else GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
	}
}
static void save_record_quit(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,FALSE);
}
static void quit_record_test(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	QUIT_RECORD_TEST(mtnrsrc,MTNMASTER->passkey->fileno,mtn->previous,readscreen,
		save_record_quit,quit_record,mtn);
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
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
static void getpreviouscb(RDArmem *member,mtnstruct *mtn)
{
	char *benefitid=NULL;
	short keyno=(-1);

	readwidget(member->parent,"[BFTDDAC][DEDUCTION DESCRIPTION IDENTIFICATION]");
	FINDRSCGETSTRING(member->parent,"[BFTDDAC][DEDUCTION DESCRIPTION IDENTIFICATION]",&benefitid);
	FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"DEDUCTION DESCRIPTION IDENTIFICATION",benefitid);
	if(benefitid!=NULL) Rfree(benefitid);
	list2file(member->parent,mtn);
	keyno=KEYNUMBER(MTNMASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1)) keyno=MTNMASTER->passkey->keyno;
	if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,keyno,SCRNvirtualSubData,member->parent))
	{
		KEYNRD(MTNMASTER->passkey->fileno,keyno);
		if(ADVLTENRDsec(MTNMASTER->passkey->fileno,keyno,SCRNvirtualSubData,member->parent))
		{
			KEYNRD(MTNMASTER->passkey->fileno,keyno);
			updatefilerecord2rsrc(MTNMASTER->passkey->fileno,member->parent);
			ClearRDATData(mtn->previous);
			GET_SUPPORTING(member->parent,MTNMASTER,SCRNvirtualSubData,(void *)member->parent,TRUE,FALSE);
		} else {
			GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
			updatefilerecord2rsrc(MTNMASTER->passkey->fileno,member->parent);
			setlist(member->parent,mtn,TRUE);
			GET_SUPPORTING(member->parent,MTNMASTER,SCRNvirtualSubData,(void *)member->parent,TRUE,TRUE);
		}
	} else {
		if(ADVPRVNRDsec(MTNMASTER->passkey->fileno,keyno,SCRNvirtualSubData,member->parent))
		{
			KEYNRD(MTNMASTER->passkey->fileno,keyno);
			updatefilerecord2rsrc(MTNMASTER->passkey->fileno,member->parent);
			ClearRDATData(mtn->previous);
			GET_SUPPORTING(member->parent,MTNMASTER,SCRNvirtualSubData,(void *)member->parent,TRUE,FALSE);
		} else {
			GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
			updatefilerecord2rsrc(MTNMASTER->passkey->fileno,member->parent);
			setlist(member->parent,mtn,TRUE);
			GET_SUPPORTING(member->parent,MTNMASTER,SCRNvirtualSubData,(void *)member->parent,TRUE,TRUE);
		}
	}
	ADVupdateSCRNvirtuals(member->parent,SCRNvirtualSubData,member->parent);
}
static void getnextcb(RDArmem *member,mtnstruct *mtn)
{
	char *benefitid=NULL;
	short keyno=(-1);

	readwidget(member->parent,"[BFTDDAC][DEDUCTION DESCRIPTION IDENTIFICATION]");
	FINDRSCGETSTRING(member->parent,"[BFTDDAC][DEDUCTION DESCRIPTION IDENTIFICATION]",&benefitid);
	FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"DEDUCTION DESCRIPTION IDENTIFICATION",benefitid);
	if(benefitid!=NULL) Rfree(benefitid);
	list2file(member->parent,mtn);
	keyno=KEYNUMBER(MTNMASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1)) keyno=MTNMASTER->passkey->keyno;
	if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,keyno,SCRNvirtualSubData,member->parent))
	{
		KEYNRD(MTNMASTER->passkey->fileno,keyno);
		if(ADVGTENRDsec(MTNMASTER->passkey->fileno,keyno,SCRNvirtualSubData,member->parent))
		{
			KEYNRD(MTNMASTER->passkey->fileno,keyno);
			updatefilerecord2rsrc(MTNMASTER->passkey->fileno,member->parent);
			ClearRDATData(mtn->previous);
			GET_SUPPORTING(member->parent,MTNMASTER,SCRNvirtualSubData,(void *)member->parent,TRUE,FALSE);
		} else {
			GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
			updatefilerecord2rsrc(MTNMASTER->passkey->fileno,member->parent);
			setlist(member->parent,mtn,TRUE);
			GET_SUPPORTING(member->parent,MTNMASTER,SCRNvirtualSubData,(void *)member->parent,TRUE,TRUE);
		}
	} else {
		if(ADVNXTNRDsec(MTNMASTER->passkey->fileno,keyno,SCRNvirtualSubData,member->parent))
		{
			KEYNRD(MTNMASTER->passkey->fileno,keyno);
			updatefilerecord2rsrc(MTNMASTER->passkey->fileno,member->parent);
			ClearRDATData(mtn->previous);
			GET_SUPPORTING(member->parent,MTNMASTER,SCRNvirtualSubData,(void *)member->parent,TRUE,FALSE);
		} else {
			GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
			updatefilerecord2rsrc(MTNMASTER->passkey->fileno,member->parent);
			setlist(member->parent,mtn,TRUE);
			GET_SUPPORTING(member->parent,MTNMASTER,SCRNvirtualSubData,(void *)member->parent,TRUE,TRUE);
		}
	}
	ADVupdateSCRNvirtuals(member->parent,SCRNvirtualSubData,member->parent);
}
static void getrecordcb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *benefitid=NULL;

	readwidget(mtnrsrc,"[BFTDDAC][DEDUCTION DESCRIPTION IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[BFTDDAC][DEDUCTION DESCRIPTION IDENTIFICATION]",&benefitid);
	FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"DEDUCTION DESCRIPTION IDENTIFICATION",benefitid);
	if(benefitid!=NULL) Rfree(benefitid);
	list2file(mtnrsrc,mtn);
	if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno);
		updatefilerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
		ClearRDATData(mtn->previous);
		GET_SUPPORTING(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc,TRUE,FALSE);
	} else {
		GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
		updatefilerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
		setlist(mtnrsrc,mtn,TRUE);
		GET_SUPPORTING(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc,TRUE,TRUE);
	}
	ADVupdateSCRNvirtuals(mtnrsrc,SCRNvirtualSubData,mtnrsrc);
}
static void setfiles(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	list2file(mtnrsrc,mtn);
	SET_MTNMASTER(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc);
}
