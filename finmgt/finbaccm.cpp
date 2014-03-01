/* finbaccm.c - Xpert Financial Balance Sheet Accounts Maintain Screen */
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

struct mtnstructs
{
	APPlib *DEFtypes;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern void browse_finbacc(RDArsrc *,mtnstruct *);
extern MaintainMaster *MASTER;
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_check(RDArsrc *,mtnstruct *,short);
static void quit_record(RDArsrc *,mtnstruct *);
static void quit_record_test(RDArsrc *,mtnstruct *);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void save_record_quit(RDArsrc *,mtnstruct *);
static void SETID(RDArsrc *,mtnstruct *);
static void SETFILES(RDArsrc *,MaintainMaster *);
static void resetdefs(RDArsrc *,mtnstruct *);
static void changeDEFtypecb(RDArsrc *,mtnstruct *);
static void setDEFtype(RDArsrc *,mtnstruct *,short);
static void setLISTS(RDArsrc *,mtnstruct *,short);
static void setlistscb(RDArsrc *,MaintainMaster *,void (*)(),void *,mtnstruct *);
static void list2file(RDArsrc *,mtnstruct *);
static void list2filecb(RDArsrc *,MaintainMaster *,void (*)(),void *,mtnstruct *);
static void getpreviouscb(RDArmem *,mtnstruct *);
static void getnextcb(RDArmem *,mtnstruct *);
static void readscreen(RDArsrc *,mtnstruct *);
char editable=TRUE;
MakeBrowseList *mbl_finbacc=NULL;
void finbaccm(short,void *);

void finbaccm(short dowhich,void *targetkey)
{
	int which_type=0;
	char *name=NULL;
	short nofields=0,nokeys=0,y=0,x=0,edit_rsrc=FALSE;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDAacct *acc=NULL;
	RDArsrc *mtnrsrc=NULL;
	mtnstruct *mtn=NULL;

	mtnrsrc=RDArsrcNEW(MASTER->module,MASTER->maintain_screen);
	ZERNRD(MASTER->passkey->fileno);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->DEFtypes=NULL;
	mtn->DEFtypes=APPlibNEW();
	mtn->previous=NULL;
	mtn->previous=RDATDataNEW(MASTER->passkey->fileno);
	if(BALANCE_SHEET_ACCOUNT!=NULL)
	{
		if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
		{
			for(x=0,acc=BALANCE_SHEET_ACCOUNT->codetype;x<BALANCE_SHEET_ACCOUNT->num;++x,++acc)
			{
				addAPPlib(mtn->DEFtypes,acc->name);
			}
		}
	} 
	if(mtn->DEFtypes->numlibs<1)
	{
		addAPPlib(mtn->DEFtypes,"No Definitions Available");
	}
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
			if(!RDAstrcmp(fldx->name,"DEFINITION TYPE"))
			{
				if(FIELDscrn(MASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					which_type=0;	
					addlstrsrc(mtnrsrc,"DEFINITION TYPES",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						changeDEFtypecb,mtn->DEFtypes->numlibs,
						&mtn->DEFtypes->libs,mtn);
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
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
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
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
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
				}
			} else {
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
			}
			break;
	}
	setLISTS(mtnrsrc,mtn,FALSE);
	GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	MSTSetVirFieldFuncs(mtnrsrc,MASTER);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,mtn);
	MakeMasterButtons(mtnrsrc,MASTER);
	MSTcomputeallSCRNvirtuals(mtnrsrc,MASTER);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
	if(mbl_finbacc!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_finbacc,mtn);
	if(mbl_finbacc!=NULL)
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
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_finbacc==NULL?TRUE:FALSE));
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
static void list2file(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected=0;
	short type=0;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selected);
	if(RDAstrcmp(mtn->DEFtypes->libs[selected],"No Definitions Available") &&
		RDAstrcmp(mtn->DEFtypes->libs[selected],"Invalid-Definition Type"))
	{
		type=(short)selected;
		FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",type);
	}
}
static void setLISTS(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	setDEFtype(mtnrsrc,mtn,update);
}
static void save_record_nup(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,FALSE);
}
static void save_record_up(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,TRUE);
}
static void save_check(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	char deleteflag=0,*warnmessage=NULL,*desc=NULL;
	int selDEFtype=0;

	readscreen(mtnrsrc,mtn);
	FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&deleteflag);
	if(!deleteflag)
	{
		FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selDEFtype);
		if(!RDAstrcmp(mtn->DEFtypes->libs[selDEFtype],"No Definitions Available") || !RDAstrcmp(mtn->DEFtypes->libs[selDEFtype],"Invalid-Definition Type"))
		{
			warnmessage=Rmalloc(201);
			sprintf(warnmessage,"The DEFINITION TYPE selected is invalid.  This must be corrected before this record can be saved.");
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEFINITION TYPE!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			return;
		}
	}
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
		updatebrowse(update_list,mbl_finbacc,MASTER->passkey->fileno,MASTER->module,mtnrsrc);
		if(mbl_finbacc!=NULL) quit_record(mtnrsrc,mtn);
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
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
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
static void changeDEFtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	short deftype1=0,deftype2=0;
	int seldeftype2=0;

	readwidget(mtnrsrc,"[FINBACC][ACCOUNT CODE]");
	FINDRSCGETDEFTYPE(mtnrsrc,"[FINBACC][ACCOUNT CODE]",&deftype1);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&seldeftype2);
	deftype2=(short)seldeftype2;
	if(deftype1!=deftype2)
	{
		FINDRSCSETSTRING(mtnrsrc,"[FINBACC][ACCOUNT CODE]",NULL);
		FINDRSCSETDEFTYPE(mtnrsrc,"[FINBACC][ACCOUNT CODE]",deftype2);
		updatersrc(mtnrsrc,"[FINBACC][ACCOUNT CODE]");
	}
	SETID(mtnrsrc,mtn);
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
	list2file(member->parent,mtn);
	ADV2GET_PREVIOUS(member->parent,MASTER,mtn->previous,SCRNvirtualSubData,
		member->parent,keyno,list2filecb,
		mtn,setlistscb,mtn,NULL,NULL);
}
static void getnextcb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1))
	{
		keyno=MASTER->passkey->keyno;
	}
	list2file(member->parent,mtn);
	ADV2GET_NEXT(member->parent,MASTER,mtn->previous,SCRNvirtualSubData,
		member->parent,keyno,list2filecb,
		mtn,setlistscb,mtn,NULL,NULL);
}
static void SETID(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	list2file(mtnrsrc,mtn);
	ADV4SET_KEYFUNC(mtnrsrc,MASTER,mtn->previous,SCRNvirtualSubData,
		(void *)mtnrsrc,list2filecb,mtn,
		setlistscb,mtn,NULL,NULL);
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *MASTER)
{
	SET_MASTER(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
}
static void list2filecb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
	void (*SubFunc)(),void *args,mtnstruct *mtn)
{
	list2file(mtnrsrc,mtn);
}
static void setlistscb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,void (*SubFunc)(),void *args,mtnstruct *mtn)
{
        setLISTS(mtnrsrc,mtn,TRUE);
}
static void setDEFtype(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	int selectedd=0,listlen=0;
	short deftype=0;

	FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",&deftype);
	selectedd=(int)deftype;
	if(selectedd>=mtn->DEFtypes->numlibs)
	{
		listlen=(selectedd-mtn->DEFtypes->numlibs+1);
		for(selectedd=0;selectedd<listlen;selectedd++)
		{
			addAPPlib(mtn->DEFtypes,"Invalid-Definition Type");
		}
	}
	FINDRSCSETDEFTYPE(mtnrsrc,"[FINBACC][ACCOUNT CODE]",deftype);
	if(!FINDRSCLISTAPPlib(mtnrsrc,"DEFINITION TYPES",selectedd,mtn->DEFtypes))
	{
		if(update_list) 
		{
			updatersrc(mtnrsrc,"DEFINITION TYPES");
		}
	}
}
