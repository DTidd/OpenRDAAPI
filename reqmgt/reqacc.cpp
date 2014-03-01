/* reqacc.c - Xpert Requisition Account Master */
/*lint -library */
#ifndef WIN32
#define __NAM__ "reqacc.lnx"
#endif
#ifdef WIN32
#define __NAM__ "reqacc.exe"
#endif
#include <app.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <fin.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

struct mtnstructs
{
	APPlib *DEFtypes;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
static short reqitm=(-1),finbacc=(-1),fineacc=(-1),finbyr=(-1),fineyr=(-1);
static char editable=TRUE;
static MakeBrowseList *mbl_reqacc=NULL;
static MaintainMaster *MTNMSTR=NULL;
static void browse_reqacc(RDArsrc *,mtnstruct *);
static void quit_record(RDArsrc *,mtnstruct *);
static void quit_recordcb(RDArsrc *,mtnstruct *);
static void quit_record_test(RDArsrc *,mtnstruct *);
static void save_record_quit(RDArsrc *,mtnstruct *);
static void save_check(RDArsrc *,mtnstruct *,short);
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void setDEFtypes1(RDArsrc *,mtnstruct *,short);
static void setDEFtypes(RDArsrc *,mtnstruct *,short);
static void DEFtypecb(RDArsrc *,mtnstruct *);
static void set_appdefaultscb(RDArsrc *,MaintainMaster *,void (*)(...),void *,mtnstruct *);
static void seteditable(RDArsrc *,mtnstruct *,short);
static void setlists(RDArsrc *,mtnstruct *,short);
static void checkflds(RDArsrc *,mtnstruct *,short);
static void checkfldscb(RDArsrc *,MaintainMaster *,void (*)(...),void *,mtnstruct *);
static void resetdefs(RDArsrc *,mtnstruct *);
static void getpreviouscb(RDArmem *,mtnstruct *);
static void getnextcb(RDArmem *,mtnstruct *);
static void SETID(RDArsrc *,mtnstruct *);
static void SETFILES(RDArsrc *,MaintainMaster *);
static void acctcodecb(RDArsrc *,mtnstruct *);
static void debcodecb(RDArsrc *,mtnstruct *);
static void credcodecb(RDArsrc *,mtnstruct *);
static void defdebcodecb(RDArsrc *,mtnstruct *);
static void defpaycodecb(RDArsrc *,mtnstruct *);
static void defcredcodecb(RDArsrc *,mtnstruct *);
static short check_code(RDArsrc *,mtnstruct *,short);
static short check_acctcode(RDArsrc *,mtnstruct *);
static short check_debcode(RDArsrc *,mtnstruct *);
static short check_credcode(RDArsrc *,mtnstruct *);
static short check_defdebcode(RDArsrc *,mtnstruct *);
static short check_defcredcode(RDArsrc *,mtnstruct *);
static short check_defpaycode(RDArsrc *,mtnstruct *);
static void checkaccts(RDArsrc *,mtnstruct *,short);
static void readscreen(RDArsrc *,mtnstruct *);
static void list2file(RDArsrc *,mtnstruct *,short);

void reqaccm(short dowhich,void *targetkey)
{
	char *name=NULL;
	short edit_rsrc=TRUE,nofields=0,nokeys=0,x=0,y=0;
	int which_type=0;
	RDAacct *acc=NULL;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDArsrc *mtnrsrc=NULL;
	mtnstruct *mtn=NULL;
	
	mtnrsrc=RDArsrcNEW(MTNMSTR->module,MTNMSTR->maintain_screen);
	ZERNRD(MTNMSTR->passkey->fileno);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->previous=NULL;
	mtn->previous=RDATDataNEW(MTNMSTR->passkey->fileno);
	mtn->DEFtypes=NULL;
	nokeys=NUMKEYS(MTNMSTR->passkey->fileno);
	keys=KEYPOINTER(MTNMSTR->passkey->fileno);
	nofields=NUMFLDS(MTNMSTR->passkey->fileno);
	fields=FLDPOINTER(MTNMSTR->passkey->fileno);
	GET_MAINTAIN_INCVARS(mtnrsrc,MTNMSTR);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"DEFINITION TYPE"))
			{
				if(FIELDscrn(MTNMSTR->passkey->fileno,fldx->name,&edit_rsrc))
				{
					mtn->DEFtypes=APPlibNEW();
					if(EXPENDITURE_ACCOUNT!=NULL)
					{
						if(EXPENDITURE_ACCOUNT->codetype!=NULL)
						{
							for(y=0,acc=EXPENDITURE_ACCOUNT->codetype;y<EXPENDITURE_ACCOUNT->num;++y,++acc)
							{
								addAPPlib(mtn->DEFtypes,acc->name);
							}
						}
					}
					if(mtn->DEFtypes->numlibs<1)
					{
						addAPPlib(mtn->DEFtypes,"No Definitions Available");
					}
					addlstrsrc(mtnrsrc,"DEFINITION TYPES",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						DEFtypecb,mtn->DEFtypes->numlibs,
						&mtn->DEFtypes->libs,mtn);
				}
			} else {
				nonlstmakefld(mtnrsrc,MTNMSTR->passkey->fileno,fldx,edit_rsrc);
				if(name!=NULL)
				{
					name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(MTNMSTR->mainfile));
				} else {
					name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(MTNMSTR->mainfile));
				}
				sprintf(name,"[%s][%s]",MTNMSTR->mainfile,fldx->name);
				FINDRSCSETFUNC(mtnrsrc,name,SETFILES,MTNMSTR);
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
						name=Rrealloc(name,RDAstrlen(part->name)+5+RDAstrlen(MTNMSTR->mainfile));
					} else {
						name=Rmalloc(RDAstrlen(part->name)+5+RDAstrlen(MTNMSTR->mainfile));
					}
					sprintf(name,"[%s][%s]",MTNMSTR->mainfile,part->name);
					FINDRSCSETFUNC(mtnrsrc,name,SETID,mtn);
					FINDRSCSETEDITABLE(mtnrsrc,name,TRUE);
				}
			}
		}
	}
	if(name!=NULL) Rfree(name);
	FINDRSCSETFUNC(mtnrsrc,"[REQACC][ACCOUNT CODE]",acctcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[REQACC][DEBIT CODE]",debcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[REQACC][CREDIT CODE]",credcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[REQACC][DEFAULT DEBIT CODE]",defdebcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[REQACC][DEFAULT CREDIT CODE]",defcredcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[REQACC][DEFAULT PAYABLE CODE]",defpaycodecb,mtn);
	addstdrsrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"DEBIT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"CREDIT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"DEFAULT DEBIT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"DEFAULT CREDIT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"DEFAULT PAYABLE CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	SET_MAINTAIN_FILE2RSRCS(mtnrsrc,MTNMSTR);
	SCRNvirtual2rsrc(mtnrsrc);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			ApplyPassKey(MTNMSTR->passkey);
			if(ADVEQLNRDsec(MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMSTR->passkey);
				if(ADVEQLNRDsec(MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MTNMSTR->passkey);
					list2file(mtnrsrc,mtn,FALSE);
				} else {
					GetRDATData(MTNMSTR->passkey->fileno,mtn->previous);
					checkflds(mtnrsrc,mtn,FALSE);
				}
			} else {
				GetRDATData(MTNMSTR->passkey->fileno,mtn->previous);
				checkflds(mtnrsrc,mtn,FALSE);
			}
			break;
		case 2:
			ApplyPassKey(MTNMSTR->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMSTR->passkey);
			if(ADVEQLNRDsec(MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MTNMSTR->passkey);
				list2file(mtnrsrc,mtn,FALSE);
			} else {
				GetRDATData(MTNMSTR->passkey->fileno,mtn->previous);
				checkflds(mtnrsrc,mtn,FALSE);
			}
			break;
		case 0:
		default:
			if(ADVEQLNRDKEYsec(MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,targetkey,SCRNvirtualSubData,mtnrsrc))
			{
				ZERNRD(MTNMSTR->passkey->fileno);
				ApplyPassKey(MTNMSTR->passkey);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMSTR->passkey);
				if(ADVEQLNRDsec(MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MTNMSTR->passkey);
					list2file(mtnrsrc,mtn,FALSE);
				} else {
					GetRDATData(MTNMSTR->passkey->fileno,mtn->previous);
					checkflds(mtnrsrc,mtn,FALSE);
				}
			} else {
				GetRDATData(MTNMSTR->passkey->fileno,mtn->previous);
				checkflds(mtnrsrc,mtn,FALSE);
			}
			break;
	}
	GET_SUPPORTING(mtnrsrc,MTNMSTR,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	MSTSetVirFieldFuncs(mtnrsrc,MTNMSTR);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
	if(mbl_reqacc!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_reqacc,mtn);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,mtn);
	addbtnrsrc(mtnrsrc,"QUIT",TRUE,quit_recordcb,mtn);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,mtn);
	MakeMasterButtons(mtnrsrc,MTNMSTR);
	MSTcomputeallSCRNvirtuals(mtnrsrc,MTNMSTR);
	if(mbl_reqacc!=NULL)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	}
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_reqacc==NULL?TRUE:FALSE));
}
static void resetdefs(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MTNMSTR->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	updateallrsrc(mtnrsrc);
	SETID(mtnrsrc,mtn);
}
static void getpreviouscb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MTNMSTR->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1))
	{
		keyno=MTNMSTR->passkey->keyno;
	}
	ADV2GET_PREVIOUS(member->parent,MTNMSTR,mtn->previous,SCRNvirtualSubData,
		member->parent,keyno,set_appdefaultscb,mtn,
		NULL,NULL,checkfldscb,mtn);
}
static void getnextcb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MTNMSTR->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1))
	{
		keyno=MTNMSTR->passkey->keyno;
	}
	ADV2GET_NEXT(member->parent,MTNMSTR,mtn->previous,SCRNvirtualSubData,
		member->parent,keyno,set_appdefaultscb,mtn,
		NULL,NULL,checkfldscb,mtn);
}
static void SETID(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ADV4SET_KEYFUNC(mtnrsrc,MTNMSTR,mtn->previous,SCRNvirtualSubData,
		(void *)mtnrsrc,set_appdefaultscb,mtn,
		NULL,NULL,checkfldscb,mtn);
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *MTNMSTR)
{
	SET_MTNMASTER(mtnrsrc,MTNMSTR,SCRNvirtualSubData,(void *)mtnrsrc);
}
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MTNMSTR->passkey->fileno);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MTNMSTR->passkey->fileno,mtnrsrc);
	list2file(mtnrsrc,mtn,FALSE);
}
static void list2file(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	setDEFtypes1(mtnrsrc,mtn,update);
}
static void save_record_now(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_record(mtnrsrc,mtn,TRUE);
}
static void save_record_now1(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_record(mtnrsrc,mtn,FALSE);
}
static void save_check(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *desc=NULL,delflag=FALSE;

	if(!SAVE_CHECK(mtnrsrc,MTNMSTR,SCRNvirtualSubData,(void *)mtnrsrc))
	{
		return;
	}
	if(!isEMPTY(MTNMSTR->save_expression))
	{
		if(EVALUATEbol(MTNMSTR->save_expression,SCRNvirtualSubData,mtnrsrc))
		{
			desc=EVALUATEstr(MTNMSTR->save_error_desc,
				SCRNvirtualSubData,mtnrsrc);
			ERRORDIALOG("SAVE ERROR",desc,NULL,FALSE);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	if(!isEMPTY(MTNMSTR->save_warning))
	{
		if(EVALUATEbol(MTNMSTR->save_warning,SCRNvirtualSubData,mtnrsrc))
		{
			desc=EVALUATEstr(MTNMSTR->save_warning_desc,
				SCRNvirtualSubData,mtnrsrc);
			ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN",
				"SAVE WARNING",desc,
				(update?save_record_now:save_record_now1),
				NULL,FALSE,2,mtnrsrc,mtn,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	FINDRSCGETCHAR(mtnrsrc,"[REQACC][DELETEFLAG]",&delflag);
	if(!delflag)
	{
		if(check_acctcode(mtnrsrc,mtn)==FALSE) return;
		if(check_debcode(mtnrsrc,mtn)==FALSE) return;
		if(check_credcode(mtnrsrc,mtn)==FALSE) return;
		if(check_defdebcode(mtnrsrc,mtn)==FALSE) return;
		if(check_defcredcode(mtnrsrc,mtn)==FALSE) return;
		if(check_defpaycode(mtnrsrc,mtn)==FALSE) return;
	}
	save_record(mtnrsrc,mtn,update);
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
static void save_record(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *rcddesc=NULL;

	if(ADVRECORDsec(MTNMSTR->passkey->fileno,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(MTNMSTR->passkey->fileno,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("SECURITY ACCESS DENIED!",rcddesc,NULL,FALSE);
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(MTNMSTR->passkey->fileno,0,NULL,mtn->previous);
		DELETE_SUBORDINATES(mtnrsrc,MTNMSTR,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update,mbl_reqacc,MTNMSTR->passkey->fileno,MTNMSTR->module,mtnrsrc);
		if(mbl_reqacc!=NULL)
		{
			quit_record(mtnrsrc,mtn);
		} else {
			seteditable(mtnrsrc,mtn,TRUE);
			GetRDATData(MTNMSTR->passkey->fileno,mtn->previous);
		}
	}
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
		CLOSE_MASTER_FILES(MTNMSTR);
		if(MTNMSTR!=NULL) FreeMaintainMaster(MTNMSTR);
		ShutdownSubsystems();
		exit(0);
	}
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
}
static void save_record_quit(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,FALSE);
}
static void quit_record_test(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
        QUIT_RECORD_TEST(mtnrsrc,MTNMSTR->passkey->fileno,
		mtn->previous,readscreen,save_record_quit,quit_record,mtn);
}
static void setDEFtypes1(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	short d=0;
	int x=0;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	d=(short)x;
	FINDFLDSETINT(MTNMSTR->passkey->fileno,"DEFINITION TYPE",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[REQACC][ACCOUNT CODE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[REQACC][DEBIT CODE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[REQACC][CREDIT CODE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[REQACC][DEFAULT DEBIT CODE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[REQACC][DEFAULT CREDIT CODE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[REQACC][DEFAULT PAYABLE CODE]",d);
	if(update)
	{
		updatersrc(mtnrsrc,"[REQACC][ACCOUNT CODE]");
		updatersrc(mtnrsrc,"[REQACC][DEBIT CODE]");
		updatersrc(mtnrsrc,"[REQACC][CREDIT CODE]");
		updatersrc(mtnrsrc,"[REQACC][DEFAULT DEBIT CODE]");
		updatersrc(mtnrsrc,"[REQACC][DEFAULT CREDIT CODE]");
		updatersrc(mtnrsrc,"[REQACC][DEFAULT PAYABLE CODE]");
	}
}
static void setDEFtypes(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *edesc=NULL;
	short d=0;
	int x=0,y=0;

	FINDFLDGETSHORT(MTNMSTR->passkey->fileno,"DEFINITION TYPE",&d);
	x=(int)d;
	if(x>=mtn->DEFtypes->numlibs)
	{
		for(y=0;y<(x-(mtn->DEFtypes->numlibs)-1);++y)
		{
			addAPPlib(mtn->DEFtypes,"Invalid-Definition Type");
		}
		edesc=Rmalloc(105);
		sprintf(edesc,"The Definition Type [%d] is invalid.  It doesn't exist. Please correct or deleted this record.",d);
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEFINITION TYPE!",edesc,NULL,NULL,TRUE,NULL);
		FINDRSCSETDEFTYPE(mtnrsrc,"ACCOUNT CODE",0);
		FINDRSCSETDEFTYPE(mtnrsrc,"DEBIT CODE",0);
		FINDRSCSETDEFTYPE(mtnrsrc,"CREDIT CODE",0);
		FINDRSCSETDEFTYPE(mtnrsrc,"DEFAULT DEBIT CODE",0);
		FINDRSCSETDEFTYPE(mtnrsrc,"DEFAULT CREDIT CODE",0);
		FINDRSCSETDEFTYPE(mtnrsrc,"DEFAULT PAYABLE CODE",0);
		if(edesc!=NULL) Rfree(edesc);
	}
	FINDRSCSETINT(mtnrsrc,"DEFINITION TYPES",x);
	FINDRSCSETDEFTYPE(mtnrsrc,"[REQACC][ACCOUNT CODE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[REQACC][DEBIT CODE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[REQACC][CREDIT CODE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[REQACC][DEFAULT DEBIT CODE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[REQACC][DEFAULT CREDIT CODE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[REQACC][DEFAULT PAYABLE CODE]",d);
	if(update)
	{
		updatersrc(mtnrsrc,"[REQACC][ACCOUNT CODE]");
		updatersrc(mtnrsrc,"[REQACC][DEBIT CODE]");
		updatersrc(mtnrsrc,"[REQACC][CREDIT CODE]");
		updatersrc(mtnrsrc,"[REQACC][DEFAULT DEBIT CODE]");
		updatersrc(mtnrsrc,"[REQACC][DEFAULT CREDIT CODE]");
		updatersrc(mtnrsrc,"[REQACC][DEFAULT PAYABLE CODE]");
		updatersrc(mtnrsrc,"DEFINITION TYPES");
	}
}
static void DEFtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	short d=0;
	int x=0;

	readallwidgets(mtnrsrc);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	d=(short)x;
	FINDFLDSETSHORT(MTNMSTR->passkey->fileno,"DEFINITION TYPE",d);
	FINDRSCSETSTRING(mtnrsrc,"[REQACC][ACCOUNT CODE]",NULL);
	FINDRSCSETSTRING(mtnrsrc,"[REQACC][DEBIT CODE]",NULL);
	FINDRSCSETSTRING(mtnrsrc,"[REQACC][CREDIT CODE]",NULL);
	FINDRSCSETSTRING(mtnrsrc,"[REQACC][DEFAULT DEBIT CODE]",NULL);
	FINDRSCSETSTRING(mtnrsrc,"[REQACC][DEFAULT CREDIT CODE]",NULL);
	FINDRSCSETSTRING(mtnrsrc,"[REQACC][DEFAULT PAYABLE CODE]",NULL);
	FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(mtnrsrc,"DEFAULT DEBIT CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(mtnrsrc,"DEFAULT CREDIT CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(mtnrsrc,"DEFAULT PAYABLE CODE DESCRIPTION",NULL);
	FINDRSCSETDEFTYPE(mtnrsrc,"[REQACC][ACCOUNT CODE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[REQACC][DEBIT CODE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[REQACC][CREDIT CODE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[REQACC][DEFAULT DEBIT CODE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[REQACC][DEFAULT CREDIT CODE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[REQACC][DEFAULT PAYABLE CODE]",d);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void set_appdefaultscb(RDArsrc *mtnrsrc,MaintainMaster *MTNMSTR,
	void (*SubFunc)(...),void *args,mtnstruct *mtn)
{
	list2file(mtnrsrc,mtn,TRUE);
}
static void setlists(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
        setDEFtypes(mtnrsrc,mtn,update);
}
static void checkflds(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	setlists(mtnrsrc,mtn,update);
	filerecord2rsrc(MTNMSTR->passkey->fileno,mtnrsrc);
	seteditable(mtnrsrc,mtn,update);
	if(update) checkaccts(mtnrsrc,mtn,update);
}
static void checkfldscb(RDArsrc *mtnrsrc,MaintainMaster *MTNMSTR,
        void (*SubFunc)(...),void *args,mtnstruct *mtn)
{
	checkflds(mtnrsrc,mtn,TRUE);
}
static void clearwidget(RDArsrc *mtnrsrc,char *widgetname)
{
#ifdef CLEARWDGT
	FINDRSCSETSTRING(mtnrsrc,widgetname,NULL);
	updatersrc(mtnrsrc,widgetname);
#endif
}
static void acctcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readscreen(mtnrsrc,mtn);
	check_acctcode(mtnrsrc,mtn);
	updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static short check_acctcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	return(check_code(mtnrsrc,mtn,0));
}
static void debcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readscreen(mtnrsrc,mtn);
	check_debcode(mtnrsrc,mtn);
	updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static short check_debcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	return(check_code(mtnrsrc,mtn,1));
}
static void credcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readscreen(mtnrsrc,mtn);
	check_credcode(mtnrsrc,mtn);
	updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static short check_credcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	return(check_code(mtnrsrc,mtn,2));
}
static void defdebcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readscreen(mtnrsrc,mtn);
	check_defdebcode(mtnrsrc,mtn);
	updatersrc(mtnrsrc,"DEFAULT DEBIT CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static short check_defdebcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	return(check_code(mtnrsrc,mtn,3));
}
static void defcredcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readscreen(mtnrsrc,mtn);
	check_defdebcode(mtnrsrc,mtn);
	updatersrc(mtnrsrc,"DEFAULT CREDIT CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static short check_defcredcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	return(check_code(mtnrsrc,mtn,4));
}
static void defpaycodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readscreen(mtnrsrc,mtn);
	check_defpaycode(mtnrsrc,mtn);
	updatersrc(mtnrsrc,"DEFAULT PAYABLE CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static short check_defpaycode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	return(check_code(mtnrsrc,mtn,5));
}
static void checkaccts(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	check_code(mtnrsrc,mtn,0);
	check_code(mtnrsrc,mtn,1);
	check_code(mtnrsrc,mtn,2);
	check_code(mtnrsrc,mtn,3);
	check_code(mtnrsrc,mtn,4);
	check_code(mtnrsrc,mtn,5);
	if(update)
	{
		updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"DEFAULT DEBIT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"DEFAULT PAYABLE CODE DESCRIPTION");
		updatersrc(mtnrsrc,"DEFAULT CREDIT CODE DESCRIPTION");
	}
}
static short check_code(RDArsrc *mtnrsrc,mtnstruct *mtn,short acctwidget)
{
	short deftype1=0;
	int seldeftype=0,yearid1=0;
	char *acctcode1=NULL,*tmp=NULL,*tmp2=NULL;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&seldeftype);
	deftype1=(short)seldeftype;
	FINDRSCGETINT(mtnrsrc,"[REQITM][FISCAL YEAR]",&yearid1);
	switch(acctwidget)
	{
		case 0: /* check expenditure account code */
			FINDRSCGETSTRING(mtnrsrc,"[REQACC][ACCOUNT CODE]",&acctcode1);
			if((tmp=ADVCHECKACCT(fineyr,fineacc,acctcode1,EXP_ACCT,deftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID EXPENDITURE ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(mtnrsrc,"[REQACC][ACCOUNT CODE]");
				clearwidget(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
				Rfree(tmp);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			} else {
				FINDFLDGETSTRING(fineacc,"DESCRIPTION",&tmp);
				FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",tmp);
				if(tmp!=NULL) Rfree(tmp);
			}
			break;
		case 1: /* check debit code */
			FINDRSCGETSTRING(mtnrsrc,"[REQACC][DEBIT CODE]",&acctcode1);
			if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode1,BAL_ACCT,deftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(mtnrsrc,"[REQACC][DEBIT CODE]");
				clearwidget(mtnrsrc,"DEBIT CODE DESCRIPTION");
				Rfree(tmp);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			} else {
				if(RDAstrcmp(tmp=ADVGETBALREF(finbacc,acctcode1,deftype1,SCRNvirtualSubData,mtnrsrc),"ENCUMBRANCE"))
				{
					if(tmp2!=NULL) Rfree(tmp2);
					tmp2=Rmalloc(RDAstrlen(tmp)+RDAstrlen(acctcode1)+178);
					sprintf(tmp2,"The Balance Sheet Account Code [%s] exist and is active for this DEBIT CODE but its REFERENCE TYPE is [%s].\nA REFERENCE TYPE of [ENCUMBRANCE] is the required type.",acctcode1,tmp);
					WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE REFERENCE TYPE!",tmp2,NULL,NULL,TRUE,NULL);
					clearwidget(mtnrsrc,"[REQACC][DEBIT CODE]");
					clearwidget(mtnrsrc,"DEBIT CODE DESCRIPTION");
					if(tmp!=NULL) Rfree(tmp);
					if(tmp2!=NULL) Rfree(tmp2);
					if(acctcode1!=NULL) Rfree(acctcode1);
					return(FALSE);
				} else {
					FINDFLDGETSTRING(finbacc,"DESCRIPTION",&tmp);
					FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",tmp);
					if(tmp!=NULL) Rfree(tmp);
				}
			}
			break;
		case 2: /* check credit code */
			FINDRSCGETSTRING(mtnrsrc,"[REQACC][CREDIT CODE]",&acctcode1);
			if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode1,BAL_ACCT,deftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(mtnrsrc,"[REQACC][CREDIT CODE]");
				clearwidget(mtnrsrc,"CREDIT CODE DESCRIPTION");
				Rfree(tmp);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			} else  if(RDAstrcmp(tmp=ADVGETBALREF(finbacc,acctcode1,deftype1,SCRNvirtualSubData,mtnrsrc),"RESERVE FOR ENCUMBRANCE"))
			{
				if(tmp2!=NULL) Rfree(tmp2);
				tmp2=Rmalloc(RDAstrlen(tmp)+RDAstrlen(acctcode1)+178);
				sprintf(tmp2,"The Balance Sheet Account Code [%s] exist and is active for this CREDIT CODE but its REFERENCE TYPE is [%s].\nA REFERENCE TYPE of [RESERVE FOR ENCUMBRANCE] is the required type.",acctcode1,tmp);
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE REFERENCE TYPE!",tmp2,NULL,NULL,TRUE,NULL);
				clearwidget(mtnrsrc,"[REQACC][CREDIT CODE]");
				clearwidget(mtnrsrc,"CREDIT CODE DESCRIPTION");
				if(tmp!=NULL) Rfree(tmp);
				if(tmp2!=NULL) Rfree(tmp2);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			} else {
				FINDFLDGETSTRING(finbacc,"DESCRIPTION",&tmp);
				FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",tmp);
				if(tmp!=NULL) Rfree(tmp);
			}
			break;
		case 3: /* check default debit code */
			FINDRSCGETSTRING(mtnrsrc,"[REQACC][DEFAULT DEBIT CODE]",&acctcode1);
			if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode1,BAL_ACCT,deftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEFAULT DEBIT ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(mtnrsrc,"[REQACC][DEFAULT DEBIT CODE]");
				clearwidget(mtnrsrc,"DEFAULT DEBIT CODE DESCRIPTION");
				Rfree(tmp);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			} else {
				FINDFLDGETSTRING(finbacc,"DESCRIPTION",&tmp);
				FINDRSCSETSTRING(mtnrsrc,"DEFAULT DEBIT CODE DESCRIPTION",tmp);
				if(tmp!=NULL) Rfree(tmp);
			}
			break;
		case 4: /* check default credit code */
			FINDRSCGETSTRING(mtnrsrc,"[REQACC][DEFAULT CREDIT CODE]",&acctcode1);
			if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode1,BAL_ACCT,deftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEFAULT CREDIT ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(mtnrsrc,"[REQACC][DEFAULT CREDIT CODE]");
				clearwidget(mtnrsrc,"DEFAULT CREDIT CODE DESCRIPTION");
				Rfree(tmp);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			} else {
				FINDFLDGETSTRING(finbacc,"DESCRIPTION",&tmp);
				FINDRSCSETSTRING(mtnrsrc,"DEFAULT CREDIT CODE DESCRIPTION",tmp);
				if(tmp!=NULL) Rfree(tmp);
			}
			break;
		case 5: /* check default payable code */
			FINDRSCGETSTRING(mtnrsrc,"[REQACC][DEFAULT PAYABLE CODE]",&acctcode1);
			if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode1,BAL_ACCT,deftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEFAULT PAYABLE ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(mtnrsrc,"[REQACC][DEFAULT PAYABLE CODE]");
				clearwidget(mtnrsrc,"DEFAULT PAYABLE CODE DESCRIPTION");
				Rfree(tmp);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			} else {
				if(RDAstrcmp(tmp=ADVGETBALREF(finbacc,acctcode1,deftype1,SCRNvirtualSubData,mtnrsrc),"ACCOUNTS PAYABLE"))
				{
					if(tmp2!=NULL) Rfree(tmp2);
					tmp2=Rmalloc(RDAstrlen(tmp)+RDAstrlen(acctcode1)+195);
					sprintf(tmp2,"The Balance Sheet Account Code [%s] exist and is active for this DEFAULT PAYABLE CODE but its REFERENCE TYPE is [%s].\nA REFERENCE TYPE of [ACCOUNTS PAYABLE] is the required type.",acctcode1,tmp);
					WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEFAULT PAYABLE CODE REFERENCE TYPE!",tmp2,NULL,NULL,TRUE,NULL);
					clearwidget(mtnrsrc,"[REQACC][DEFAULT PAYABLE CODE]");
					clearwidget(mtnrsrc,"DEFAULT PAYABLE CODE DESCRIPTION");
					if(tmp!=NULL) Rfree(tmp);
					if(tmp2!=NULL) Rfree(tmp2);
					if(acctcode1!=NULL) Rfree(acctcode1);
					return(FALSE);
				} else {
					FINDFLDGETSTRING(finbacc,"DESCRIPTION",&tmp);
					FINDRSCSETSTRING(mtnrsrc,"DEFAULT PAYABLE CODE DESCRIPTION",tmp);
					if(tmp!=NULL) Rfree(tmp);
				}
			}
			break;
		default:
			prterr("\"acctwidget\" value of [%d] out of range.",acctwidget);
			break;
	}
	if(tmp!=NULL) Rfree(tmp);
	if(tmp2!=NULL) Rfree(tmp2);
	if(acctcode1!=NULL) Rfree(acctcode1);
	return(TRUE);
}
static void quit_recordcb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int x=(-1);
	RDArmem *member;
	char test=FALSE;

	if((x=FINDRSC(mtnrsrc,"SAVE"))!=(-1))
	{
		member=mtnrsrc->rscs+x;
		test=(char)IsWidgetDisplayed(member);
	}
	if(!test)
	{
		if((x=FINDRSC(mtnrsrc,"SAVE NO UPDATE"))!=(-1))
		{
			member=mtnrsrc->rscs+x;
			test=(char)IsWidgetDisplayed(member);
		}
	}
	if(test) quit_record_test(mtnrsrc,mtn);
	else quit_record(mtnrsrc,mtn);
}
static void seteditable(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char suv=FALSE;

	FINDFLDGETCHAR(reqitm,"SOURCE USER VERIFIED",&suv);
	if(suv)
	{
		if(update)
		{
			updateeditfilersrcsnokeys(MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,mtnrsrc,FALSE);
		} else {
			seteditfilersrc(MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,mtnrsrc,FALSE,FALSE);
		}
		SetRDAScrolledListEditable(mtnrsrc,MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,FALSE,FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"DEFINITION TYPES",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"[REQACC][DELETEFLAG]",TRUE);
	} else {
		if(update)
		{
			updateeditfilersrcsnokeys(MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,mtnrsrc,TRUE);
		} else {
			seteditfilersrc(MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,mtnrsrc,TRUE,FALSE);
                }
		SetRDAScrolledListEditable(mtnrsrc,MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,TRUE,TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"DEFINITION TYPES",TRUE);
	}
}
void browse_reqacc(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtn!=NULL)
	{
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	mainrsrc=RDArsrcNEW(MTNMSTR->module,MTNMSTR->browse_screen);
	searchrsrc=RDArsrcNEW(MTNMSTR->module,MTNMSTR->search_browse);
	definelist=RDArsrcNEW(MTNMSTR->module,MTNMSTR->define_list);
	ZERNRD(MTNMSTR->passkey->fileno);
	ApplyPassKey(MTNMSTR->passkey);
	GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,MTNMSTR,SCRNvirtualSubData,(void *)mainrsrc);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	MakeRunFunction(mainrsrc,MTNMSTR);
	mbl_reqacc=ADVPbrowse(MTNMSTR->passkey->fileno,MTNMSTR->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MTNMSTR->passkey);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	short filenum=(-1);
	char *warnmessage=NULL;
	char *module="REQMGT",*mtnname="MTN REQACC";

	if(InitializeSubsystems(argc,argv,module,mtnname))
	{
		RDAAPPMAINLOOP();
		return;
	}
	MTNMSTR=MaintainMasterNew(module,mtnname);
	if(findMaintainMaster(MTNMSTR))
	{
		warnmessage=Rmalloc(300+RDAstrlen(MTNMSTR->module)+RDAstrlen(mtnname));
		sprintf(warnmessage,"Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.",mtnname,MTNMSTR->module);
		ERRORDIALOG("MAINTAIN MTNMSTR NOT FOUND!",warnmessage,NULL,TRUE);
		prterr("%s",warnmessage);
		if(warnmessage!=NULL) Rfree(warnmessage);
		doexit(NULL);
	} else {
		if((filenum=APPOPNNRDEDITABLE(MTNMSTR->module,MTNMSTR->mainfile,&editable))==(-1))
		{
			doexit(NULL);
			return;
		}
		SetSupportingWritable(MTNMSTR,"FINMGT","FINEYR",1,TRUE,&editable);
		SetSupportingWritable(MTNMSTR,"FINMGT","FINBYR",1,TRUE,&editable);
		if(!OPEN_MASTER_FILES(MTNMSTR))
		{
			doexit(NULL);
			return;
		}
		reqitm=APPReturnSupportingFileno(MTNMSTR,"REQMGT","REQITM",1);
		fineyr=APPReturnSupportingFileno(MTNMSTR,"FINMGT","FINEYR",1);
		finbyr=APPReturnSupportingFileno(MTNMSTR,"FINMGT","FINBYR",1);
		fineacc=APPReturnSupportingFileno(MTNMSTR,"FINMGT","FINEACC",1);
		finbacc=APPReturnSupportingFileno(MTNMSTR,"FINMGT","FINBACC",1);
		MTNMSTR->passkey=READPassKey(filenum);
		MTNMSTR->inheritrsrcs=TRUE;
		switch(MTNMSTR->start_with)
		{
			default:
			case 0:
				reqaccm(1,NULL);
				break;
			case 1:
				browse_reqacc(NULL,NULL);
				break;
		}
	}
	RDAAPPMAINLOOP();
}
void doexit(MakeBrowseList *blist)
{
	if(blist!=NULL) 
	{
		if(blist->definelist!=NULL) 
		{
			killwindow(blist->definelist);
			free_rsrc(blist->definelist);
		}
		if(blist->searchrsrc!=NULL) 
		{
			killwindow(blist->searchrsrc);
			free_rsrc(blist->searchrsrc);
		}
		if(blist->mainrsrc!=NULL)
		{
			free_rsrc(blist->mainrsrc);
		}
	}
	if(WindowCount<2)
	{
		CLOSE_MASTER_FILES(MTNMSTR);
		if(MTNMSTR!=NULL) FreeMaintainMaster(MTNMSTR);
		ShutdownSubsystems();
	}
	if(mbl_reqacc!=NULL) mbl_reqacc=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func))
	{
		if(!run_func)
		{
			reqaccm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MTNMSTR);
		}
	} else {
		reqaccm(0,targetkey);
	}
}
static void new_record(RDArsrc *mtnrsrc,MakeBrowseList *blist)
{
	reqaccm(2,NULL);
} 
