/* venrvchm.c - Xpert Vendor Recurring Vouchers Maintain Screen */
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
	APPlib *ACCTtypes;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

static void save_check(RDArsrc *,mtnstruct *,short);
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_record_quit(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void save_record_now(RDArsrc *,mtnstruct *);
static void save_record_now1(RDArsrc *,mtnstruct *);
static void quit_record(RDArsrc *,mtnstruct *);
static void quit_record_test(RDArsrc *,mtnstruct *);
static void list2file(RDArsrc *,mtnstruct *,short);
static void readscreen(RDArsrc *,mtnstruct *);
static void setlists(RDArsrc *,mtnstruct *,short);
static void setlistscb(RDArsrc *,MaintainMaster *,void (*)(),void *,mtnstruct *);
static void checkfldscb(RDArsrc *,MaintainMaster *,void (*)(),void *,mtnstruct *);
static void resetdefs(RDArsrc *,mtnstruct *);
static void getpreviouscb(RDArmem *,mtnstruct *);
static void getnextcb(RDArmem *,mtnstruct *);
static void SETID(RDArsrc *,mtnstruct *);
static void SETFILES(RDArsrc *,MaintainMaster *);
static void makeACCTtypes(RDArsrc *,mtnstruct *);
static void changeACCTtype(RDArsrc *,mtnstruct *);
static void changeDEFtype(RDArsrc *,mtnstruct *,short);
static void ACCTtypecb(RDArsrc *,mtnstruct *);
static void DEFtypecb(RDArsrc *,mtnstruct *);
static void setACCTtype(RDArsrc *,mtnstruct *,short);
static void setACCTtype1(RDArsrc *,mtnstruct *,short);
static void checkaccts(RDArsrc *,mtnstruct *,short);
static short check_code(RDArsrc *,mtnstruct *,short);
static void acctcodecb(RDArsrc *,mtnstruct *);
static short check_acctcode(RDArsrc *mtnrsrc,mtnstruct *);
static void debcodecb(RDArsrc *,mtnstruct *);
static short check_debcode(RDArsrc *,mtnstruct *);
static void credcodecb(RDArsrc *,mtnstruct *);
static short check_credcode(RDArsrc *,mtnstruct *);
static void paycodecb(RDArsrc *,mtnstruct *);
static short check_paycode(RDArsrc *,mtnstruct *);
extern MaintainMaster *MASTER;
extern short fineacc,finracc,finbacc;
extern char editable;
extern MakeBrowseList *mbl_venrvch;
extern void browse_venrvch(RDArsrc *,mtnstruct *);
void venrvchm(short,void *);

void venrvchm(short dowhich,void *targetkey)
{
	char *name=NULL;
	short edit_rsrc=TRUE,nofields=0,nokeys=0,x=0,y=0;
	int which_type=0;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDArsrc *mtnrsrc=NULL;
	mtnstruct *mtn=NULL;

	mtnrsrc=RDArsrcNEW(MASTER->module,MASTER->maintain_screen);
	ZERNRD(MASTER->passkey->fileno);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->previous=RDATDataNEW(MASTER->passkey->fileno);
	mtn->DEFtypes=NULL;
	mtn->ACCTtypes=NULL;
	mtn->DEFtypes=APPlibNEW();
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
			if(!RDAstrcmp(fldx->name,"ACCOUNT TYPE"))
			{
				if(FIELDscrn(MASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					which_type=0;
					makeACCTtypes(mtnrsrc,mtn);
					addlstrsrc(mtnrsrc,"ACCOUNT TYPES",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						ACCTtypecb,mtn->ACCTtypes->numlibs,
						&mtn->ACCTtypes->libs,mtn);
				}
			} else if(!RDAstrcmp(fldx->name,"DEFINITION TYPE"))
			{
				if(FIELDscrn(MASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					addlstrsrc(mtnrsrc,"DEFINITION TYPES",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						DEFtypecb,mtn->DEFtypes->numlibs,
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
					FINDRSCSETEDITABLE(mtnrsrc,name,TRUE);
				}
			}
		}
	}
	if(name!=NULL) Rfree(name);
	FINDRSCSETFUNC(mtnrsrc,"[VENRVCH][ACCOUNT CODE]",acctcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[VENRVCH][DEBIT CODE]",debcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[VENRVCH][CREDIT CODE]",credcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[VENRVCH][PAYABLE CODE]",paycodecb,mtn);
	addstdrsrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"DEBIT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"CREDIT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"PAYABLE CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
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
					list2file(mtnrsrc,mtn,FALSE);
				} else {
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					setlists(mtnrsrc,mtn,FALSE);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
					checkaccts(mtnrsrc,mtn,FALSE);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				setlists(mtnrsrc,mtn,FALSE);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				checkaccts(mtnrsrc,mtn,FALSE);
			}
			break;
		case 2:
			ApplyPassKey(MASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
			if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
				list2file(mtnrsrc,mtn,FALSE);
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				setlists(mtnrsrc,mtn,FALSE);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				checkaccts(mtnrsrc,mtn,FALSE);
			}
			break;
		case 0:
		default:
			if(ADVEQLNRDKEYsec(MASTER->passkey->fileno,MASTER->passkey->keyno,targetkey,SCRNvirtualSubData,mtnrsrc))
			{
				ZERNRD(MASTER->passkey->fileno);
				ApplyPassKey(MASTER->passkey);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
				if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
					list2file(mtnrsrc,mtn,FALSE);
				} else {
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					setlists(mtnrsrc,mtn,FALSE);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
					checkaccts(mtnrsrc,mtn,FALSE);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				setlists(mtnrsrc,mtn,FALSE);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				checkaccts(mtnrsrc,mtn,FALSE);
			}
			break;
	}
	GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
	if(mbl_venrvch!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_venrvch,mtn);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,mtn);
	addbtnrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,mtn);
	MakeMasterButtons(mtnrsrc,MASTER);
	computeallSCRNvirtuals(mtnrsrc);
	if(mbl_venrvch!=NULL)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	}
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,
		(mbl_venrvch==NULL?TRUE:FALSE));
}
static void resetdefs(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
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
		member->parent,keyno,NULL,NULL,
		setlistscb,mtn,checkfldscb,mtn);
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
		member->parent,keyno,NULL,NULL,
		setlistscb,mtn,checkfldscb,mtn);
}
static void SETID(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ADV4SET_KEYFUNC(mtnrsrc,MASTER,mtn->previous,SCRNvirtualSubData,
		(void *)mtnrsrc,NULL,NULL,
		setlistscb,mtn,checkfldscb,mtn);
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *MASTER)
{
	SET_MASTER(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
}
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MASTER->passkey->fileno);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
	list2file(mtnrsrc,mtn,FALSE);
}
static void list2file(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	setACCTtype1(mtnrsrc,mtn,update);
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
				NULL,FALSE,2,mtnrsrc,mtn,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&delflag);
	if(!delflag)
	{
		if(check_acctcode(mtnrsrc,mtn)==FALSE) return;
		if(check_debcode(mtnrsrc,mtn)==FALSE) return;
		if(check_credcode(mtnrsrc,mtn)==FALSE) return;
		if(check_paycode(mtnrsrc,mtn)==FALSE) return;
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
	char *rcddesc=NULL,*debcode1=NULL;
	short accttype=0;

	FINDFLDGETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",&accttype);
	if(accttype==BAL_ACCT)
	{
		FINDRSCGETSTRING(mtnrsrc,"[VENRVCH][DEBIT CODE]",&debcode1);
		if(!isEMPTY(debcode1))
		{
			FINDRSCSETSTRING(mtnrsrc,"[VENRVCH][DEBIT CODE]",NULL);
			FINDFLDSETSTRING(MASTER->passkey->fileno,"DEBIT CODE",NULL);
			updatersrc(mtnrsrc,"[VENRVCH][DEBIT CODE]");
			Rfree(debcode1);
		}
	}
	if(ADVRECORDsec(MASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(MASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("SECURITY ACCESS DENIED!",rcddesc,NULL,FALSE); 
			prterr("Security Access Denied [%s].  Didn't write record.",rcddesc);
			Rfree(rcddesc);
		} else {
			prterr("Error didn't write record.");
		}
	} else {
		WRTTRANS(MASTER->passkey->fileno,0,NULL,mtn->previous);
 		DELETE_SUBORDINATES(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update,mbl_venrvch,MASTER->passkey->fileno,MASTER->module,mtnrsrc);
		if(mbl_venrvch!=NULL) quit_record(mtnrsrc,mtn);
		else GetRDATData(MASTER->passkey->fileno,mtn->previous);
	}
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->ACCTtypes!=NULL) freeapplib(mtn->DEFtypes);
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
static void save_record_quit(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,FALSE);
}
static void quit_record_test(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	QUIT_RECORD_TEST(mtnrsrc,MASTER->passkey->fileno,mtn->previous,readscreen,
		save_record_quit,quit_record,mtn);
}
static void setlists(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	setACCTtype(mtnrsrc,mtn,update);
}
static void setlistscb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,void (*SubFunc)(),void *args,mtnstruct *mtn)
{
	setlists(mtnrsrc,mtn,TRUE);
}
static void checkfldscb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
	void (*SubFunc)(),void *args,mtnstruct *mtn)
{
	updatefilerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
	checkaccts(mtnrsrc,mtn,TRUE);
}
static void DEFtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	changeDEFtype(mtnrsrc,mtn,TRUE);
	updateSCRNvirtuals(mtnrsrc);
}
static void changeDEFtype(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	int selecteda=0,selectedd=0;
	short accttype1=0,accttype2=0;
	short deftype1=0,deftype2=0;

	if(FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selecteda)) return;
	if(FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selectedd)) return;
	FINDRSCGETACCTTYPE(mtnrsrc,"[VENRVCH][ACCOUNT CODE]",&accttype1);
	FINDRSCGETDEFTYPE(mtnrsrc,"[VENRVCH][ACCOUNT CODE]",&deftype1);
	accttype2=(short)selecteda;
	deftype2=(short)selectedd;
	if(accttype1!=accttype2 || deftype1!=deftype2)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"[VENRVCH][DEBIT CODE]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[VENRVCH][DEBIT CODE]",TRUE);
		FINDRSCSETSTRING(mtnrsrc,"[VENRVCH][ACCOUNT CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[VENRVCH][DEBIT CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[VENRVCH][CREDIT CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[VENRVCH][PAYABLE CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"PAYABLE CODE DESCRIPTION",NULL);
		deftype2=(short)selectedd;
		switch(selecteda)
		{
			case EXP_ACCT:
				FINDRSCSETACCTTYPE(mtnrsrc,"[VENRVCH][ACCOUNT CODE]",EXP_ACCT);
				FINDRSCSETDEFTYPE(mtnrsrc,"[VENRVCH][ACCOUNT CODE]",deftype2);
				FINDRSCSETDEFTYPE(mtnrsrc,"[VENRVCH][DEBIT CODE]",deftype2);
				FINDRSCSETDEFTYPE(mtnrsrc,"[VENRVCH][CREDIT CODE]",deftype2);
				FINDRSCSETDEFTYPE(mtnrsrc,"[VENRVCH][PAYABLE CODE]",deftype2);
				break;
			case REV_ACCT:
				FINDRSCSETACCTTYPE(mtnrsrc,"[VENRVCH][ACCOUNT CODE]",REV_ACCT);
				FINDRSCSETSENSITIVE(mtnrsrc,"[VENRVCH][DEBIT CODE]",TRUE);
				FINDRSCSETEDITABLE(mtnrsrc,"[VENRVCH][DEBIT CODE]",TRUE);
				FINDRSCSETDEFTYPE(mtnrsrc,"[VENRVCH][ACCOUNT CODE]",deftype2);
				FINDRSCSETDEFTYPE(mtnrsrc,"[VENRVCH][DEBIT CODE]",deftype2);
				FINDRSCSETDEFTYPE(mtnrsrc,"[VENRVCH][CREDIT CODE]",deftype2);
				FINDRSCSETDEFTYPE(mtnrsrc,"[VENRVCH][PAYABLE CODE]",deftype2);
				break;
			case BAL_ACCT:
				FINDRSCSETACCTTYPE(mtnrsrc,"[VENRVCH][ACCOUNT CODE]",BAL_ACCT);
				FINDRSCSETDEFTYPE(mtnrsrc,"[VENRVCH][ACCOUNT CODE]",deftype2);
				FINDRSCSETDEFTYPE(mtnrsrc,"[VENRVCH][DEBIT CODE]",deftype2);
				FINDRSCSETDEFTYPE(mtnrsrc,"[VENRVCH][CREDIT CODE]",deftype2);
				FINDRSCSETDEFTYPE(mtnrsrc,"[VENRVCH][PAYABLE CODE]",deftype2);
				FINDRSCSETEDITABLE(mtnrsrc,"[VENRVCH][DEBIT CODE]",TRUE);
				FINDRSCSETSTRING(mtnrsrc,"[VENRVCH][ACCOUNT CODE]",NULL);
				FINDRSCSETSTRING(mtnrsrc,"[VENRVCH][DEBIT CODE]",NULL);
				FINDRSCSETSTRING(mtnrsrc,"[VENRVCH][CREDIT CODE]",NULL);
				FINDRSCSETSTRING(mtnrsrc,"[VENRVCH][PAYABLE CODE]",NULL);
				FINDRSCSETEDITABLE(mtnrsrc,"[VENRVCH][DEBIT CODE]",FALSE);
				FINDRSCSETSENSITIVE(mtnrsrc,"[VENRVCH][DEBIT CODE]",FALSE);
				break;
			default:
				prterr("Invalid Account Type [%d]",selecteda);
				break;
		}
	}
	if(update)
	{
		updatersrc(mtnrsrc,"[VENRVCH][ACCOUNT CODE]");
		updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"[VENRVCH][DEBIT CODE]");
		updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"[VENRVCH][CREDIT CODE]");
		updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"[VENRVCH][PAYABLE CODE]");
		updatersrc(mtnrsrc,"PAYABLE CODE DESCRIPTION");
	}
}
static void setACCTtype1(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	int selecteda=0,selectedd;
	short accttype=0,deftype=0;

	FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selecteda);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selectedd);
	accttype=(short)selecteda;
	deftype=(short)selectedd;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",accttype);
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",deftype);
	setACCTtype(mtnrsrc,mtn,update);
}
static void setACCTtype(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	short accttype=0,deftype=0;
	int selecteda=0,selectedd=0;

	if(!FINDFLDGETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",&accttype))
		selecteda=(int)accttype;
	FINDRSCSETINT(mtnrsrc,"ACCOUNT TYPES",selecteda);
	if(update) updatersrc(mtnrsrc,"ACCOUNT TYPES");
	changeACCTtype(mtnrsrc,mtn);
	if(!FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",&deftype))
		selectedd=(int)deftype;
	FINDRSCSETINT(mtnrsrc,"DEFINITION TYPES",selectedd);
	if(!FINDRSCLISTAPPlib(mtnrsrc,"DEFINITION TYPES",selectedd,mtn->DEFtypes))
	{
		if(update) updatersrc(mtnrsrc,"DEFINITION TYPES");
	}
	changeDEFtype(mtnrsrc,mtn,update);
}
static void ACCTtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	short accttype=0,deftype=0;
	int selecteda=0,selectedd=0;

	if(FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selecteda)) return;
		accttype=(short)selecteda;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",accttype);
	if(FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selectedd)) return;
		deftype=(short)selectedd;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",deftype);
	changeACCTtype(mtnrsrc,mtn);
	if(!FINDRSCLISTAPPlib(mtnrsrc,"DEFINITION TYPES",selectedd,mtn->DEFtypes))
	{
		updatersrc(mtnrsrc,"DEFINITION TYPES");
	}
	changeDEFtype(mtnrsrc,mtn,TRUE);
}
static void changeACCTtype(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selecteda=0,x=0;
	RDAacct *acct=NULL;

	if(FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selecteda)) return;
	if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
	mtn->DEFtypes=APPlibNEW();
	switch(selecteda)
	{
		case EXP_ACCT:	
			if(EXPENDITURE_ACCOUNT->codetype!=NULL)
			{
				for(x=0,acct=EXPENDITURE_ACCOUNT->codetype;
					x<EXPENDITURE_ACCOUNT->num;++x,++acct)
				{
					addAPPlib(mtn->DEFtypes,acct->name);
				}
			}
			break;
		case REV_ACCT:	
			if(REVENUE_ACCOUNT->codetype!=NULL)
			{
				for(x=0,acct=REVENUE_ACCOUNT->codetype;
					x<REVENUE_ACCOUNT->num;++x,++acct)
				{
					addAPPlib(mtn->DEFtypes,acct->name);
				}
			}
			break;
		case BAL_ACCT:	
			if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
			{
				for(x=0,acct=BALANCE_SHEET_ACCOUNT->codetype;
					x<BALANCE_SHEET_ACCOUNT->num;++x,++acct)
				{
					addAPPlib(mtn->DEFtypes,acct->name);
				}
			}
			break;
		default:
			prterr("Invalid Account Type [%d]",selecteda);
			break;
	}
	if(mtn->DEFtypes->numlibs<1)
	{
		addAPPlib(mtn->DEFtypes,"No Definitions Available");
	}
}
static void checkaccts(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	check_code(mtnrsrc,mtn,0);
	check_code(mtnrsrc,mtn,1);
	check_code(mtnrsrc,mtn,2);
	check_code(mtnrsrc,mtn,3);
	if(update)
	{
		updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"PAYABLE CODE DESCRIPTION");
	}
}
static short check_code(RDArsrc *mtnrsrc,mtnstruct *mtn,short acctwidget)
{
	short accttype=0,deftype=0;
	int selaccttype=0,seldeftype=0;
	char *acctcode=NULL,*tmp=NULL;

	FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selaccttype);
	accttype=(short)selaccttype;
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&seldeftype);
	deftype=(short)seldeftype;
	switch(acctwidget)
	{
		case 0:
			FINDRSCGETSTRING(mtnrsrc,"[VENRVCH][ACCOUNT CODE]",&acctcode);
			switch(accttype)
			{
				case REV_ACCT:
					if((tmp=ADVCHECKACCT((-1),finracc,acctcode,accttype,deftype,(-1),SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID REVENUE ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
						Rfree(tmp);
						if(acctcode!=NULL) Rfree(acctcode);
						FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",NULL);
						return(FALSE);
					} else {
						FINDFLDGETSTRING(finracc,"DESCRIPTION",&tmp);
						FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",tmp);
						if(tmp!=NULL) Rfree(tmp);
					}
					break;
				case BAL_ACCT:
					if((tmp=ADVCHECKACCT((-1),finbacc,acctcode,accttype,deftype,(-1),SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
						Rfree(tmp);
						if(acctcode!=NULL) Rfree(acctcode);
						FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",NULL);
						return(FALSE);
					} else {
						FINDFLDGETSTRING(finbacc,"DESCRIPTION",&tmp);
						FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",tmp);
						if(tmp!=NULL) Rfree(tmp);
					}
					break;
				case EXP_ACCT:
					if((tmp=ADVCHECKACCT((-1),fineacc,acctcode,accttype,deftype,(-1),SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID EXPENDITURE ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
						Rfree(tmp);
						if(acctcode!=NULL) Rfree(acctcode);
						FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",NULL);
						return(FALSE);
					} else {
						FINDFLDGETSTRING(fineacc,"DESCRIPTION",&tmp);
						FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",tmp);
						if(tmp!=NULL) Rfree(tmp);
					}
					break;
				default :
					prterr("Invalid Account Type [%d]",accttype);
					break;
			}
			if(acctcode!=NULL) Rfree(acctcode);
			break;
		case 1:
			FINDRSCGETSTRING(mtnrsrc,"[VENRVCH][DEBIT CODE]",&acctcode);
			switch(accttype)
			{
				case REV_ACCT :
					if(!isEMPTY(acctcode))
					{
						if((tmp=ADVCHECKACCT((-1),finbacc,acctcode,accttype,deftype,(-1),SCRNvirtualSubData,mtnrsrc))!=NULL)
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!",tmp,NULL,NULL,TRUE,NULL);
							Rfree(tmp);
							if(acctcode!=NULL) Rfree(acctcode);
							FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
							return(FALSE);
						} else {
							FINDFLDGETSTRING(finbacc,"DESCRIPTION",&tmp);
							FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",tmp);
							if(tmp!=NULL) Rfree(tmp);
						}
					} else {
						FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
					}
					break;
				case EXP_ACCT:
					if((tmp=ADVCHECKACCT((-1),finbacc,acctcode,accttype,deftype,(-1),SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!",tmp,NULL,NULL,TRUE,NULL);
						Rfree(tmp);
						if(acctcode!=NULL) Rfree(acctcode);
						FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
						return(FALSE);
					} else {
						FINDFLDGETSTRING(finbacc,"DESCRIPTION",&tmp);
						FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",tmp);
						if(tmp!=NULL) Rfree(tmp);
					}
					break;
				case BAL_ACCT:
					break;
				default :
					prterr("Invalid Account Type [%d]",accttype);
					break;
			} /* end switch/case */
			if(acctcode!=NULL) Rfree(acctcode);
			break;
		case 2:
			FINDRSCGETSTRING(mtnrsrc,"[VENRVCH][CREDIT CODE]",&acctcode);
			switch(accttype)
			{
				case EXP_ACCT :
					if(!isEMPTY(acctcode))
					{
						if((tmp=ADVCHECKACCT((-1),finbacc,acctcode,accttype,deftype,(-1),SCRNvirtualSubData,mtnrsrc))!=NULL)
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE!",tmp,NULL,NULL,TRUE,NULL);
							Rfree(tmp);
							if(acctcode!=NULL) Rfree(acctcode);
							FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
							return(FALSE);
						} else {
							FINDFLDGETSTRING(finbacc,"DESCRIPTION",&tmp);
							FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",tmp);
							if(tmp!=NULL) Rfree(tmp);
						}
					} else {
						FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
					}
					break;
				case BAL_ACCT :
				case REV_ACCT :
					if((tmp=ADVCHECKACCT((-1),finbacc,acctcode,accttype,deftype,(-1),SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE!",tmp,NULL,NULL,TRUE,NULL);
						Rfree(tmp);
						if(acctcode!=NULL) Rfree(acctcode);
						FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
						return(FALSE);
					} else {
						FINDFLDGETSTRING(finbacc,"DESCRIPTION",&tmp);
						FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",tmp);
						if(tmp!=NULL) Rfree(tmp);
					}
					break;
				default:
					prterr("Invalid Account Type [%d]",accttype);
					break;
			}
			if(acctcode!=NULL) Rfree(acctcode);
			break;
		case 3:
			FINDRSCGETSTRING(mtnrsrc,"[VENRVCH][PAYABLE CODE]",&acctcode);
			if((tmp=ADVCHECKACCT((-1),finbacc,acctcode,accttype,deftype,(-1),SCRNvirtualSubData,mtnrsrc))!=NULL)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PAYABLE CODE!",tmp,NULL,NULL,TRUE,NULL);
				Rfree(tmp);
				if(acctcode!=NULL) Rfree(acctcode);
				FINDRSCSETSTRING(mtnrsrc,"PAYABLE CODE DESCRIPTION",NULL);
				return(FALSE);
			} else {
				if((tmp=ADVGETBALREF(finbacc,acctcode,deftype,SCRNvirtualSubData,mtnrsrc))!=NULL)
				{
					if(RDAstrcmp(tmp,"ACCOUNTS PAYABLE"))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PAYABLE CODE!","The PAYABLE CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of [ACCOUNTS PAYABLE]." ,NULL,NULL,TRUE,NULL);
						Rfree(tmp);
						if(acctcode!=NULL) Rfree(acctcode);
						FINDRSCSETSTRING(mtnrsrc,"PAYABLE CODE DESCRIPTION",NULL);
						return(FALSE);
					} else {
						FINDFLDGETSTRING(finbacc,"DESCRIPTION",&tmp);
						FINDRSCSETSTRING(mtnrsrc,"PAYABLE CODE DESCRIPTION",tmp);
						if(tmp!=NULL) Rfree(tmp);
					}
				} else {
					WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PAYABLE CODE!","The PAYABLE CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of [ACCOUNTS PAYABLE]." ,NULL,NULL,TRUE,NULL);
					if(acctcode!=NULL) Rfree(acctcode);
					FINDRSCSETSTRING(mtnrsrc,"PAYABLE CODE DESCRIPTION",NULL);
					return(FALSE);
				}
			}
			if(acctcode!=NULL) Rfree(acctcode);
			break;
		default:
			prterr("\"acctwidget\" value of [%d] out of range.",acctwidget);
			break;
	}
	return(TRUE);
}
static void acctcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readwidget(mtnrsrc,"[VENRVCH][ACCOUNT CODE]");
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
	readwidget(mtnrsrc,"[VENRVCH][DEBIT CODE]");
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
	readwidget(mtnrsrc,"[VENRVCH][CREDIT CODE]");
	check_credcode(mtnrsrc,mtn);
	updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static short check_credcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	return(check_code(mtnrsrc,mtn,2));
}
static void paycodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readwidget(mtnrsrc,"[VENRVCH][PAYABLE CODE]");
	check_paycode(mtnrsrc,mtn);
	updatersrc(mtnrsrc,"PAYABLE CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static short check_paycode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	return(check_code(mtnrsrc,mtn,3));
}
static void makeACCTtypes(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn->ACCTtypes!=NULL) freeapplib(mtn->ACCTtypes);
	mtn->ACCTtypes=APPlibNEW();
	addAPPlib(mtn->ACCTtypes,accttypes[0]);
	addAPPlib(mtn->ACCTtypes,accttypes[1]);
	addAPPlib(mtn->ACCTtypes,accttypes[2]);
}
