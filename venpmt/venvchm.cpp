/* venvchm.c - Xpert Vendor Voucher Master Maintain Screen */
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
#include <venpmt.h>

struct mtnstructs
{
	APPlib *DEFtypes;
	APPlib *ACCTtypes;
	RDATData *previous;
	APPlib *clearDEFtypes;
};
typedef struct mtnstructs mtnstruct;

extern RDAvenpmt *venpmt;
static short *sht_pointer=NULL;
static void save_check(RDArsrc *,mtnstruct *,short);
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_record_quit(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void duetocb(RDArsrc *,mtnstruct *);
static void clearfromcb(RDArsrc *,mtnstruct *);
static void clearcashcb(RDArsrc *,mtnstruct *);
static void save_record_now(RDArsrc *,mtnstruct *);
static void save_record_now1(RDArsrc *,mtnstruct *);
static void quit_record(RDArsrc *,mtnstruct *);
static void quit_record_test(RDArsrc *,mtnstruct *);
static void clearDEFtypecb(RDArsrc *,mtnstruct *);
static void list2file(RDArsrc *,mtnstruct *,short);
static void readscreen(RDArsrc *,mtnstruct *);
static void set_appdefaults(RDArsrc *,mtnstruct *,short);
static void set_appdefaultscb(RDArsrc *,MaintainMaster *,void (*)(),void *,mtnstruct *);
static void seteditable(RDArsrc *,mtnstruct *,short);
static void setlists(RDArsrc *,mtnstruct *,short);
static void checkflds(RDArsrc *,mtnstruct *,short);
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
static short check_acctcode(RDArsrc *,mtnstruct *);
static short check_debcode(RDArsrc *,mtnstruct *);
static short check_credcode(RDArsrc *,mtnstruct *);
static short check_paycode(RDArsrc *,mtnstruct *);
static void acctcodecb(RDArsrc *,mtnstruct *),debcodecb(RDArsrc *,mtnstruct *);
static void credcodecb(RDArsrc *,mtnstruct *);
static void paycodecb(RDArsrc *,mtnstruct *);
static short testavailbalance(RDArsrc *,mtnstruct *,short);
static short check_dueto(RDArsrc *,mtnstruct *);
static short check_clearcash(RDArsrc *,mtnstruct *);
static short check_clearfrom(RDArsrc *,mtnstruct *);
static void changeclearDEFtype(RDArsrc *,mtnstruct *,short);
extern short finbacc,finracc,fineacc;
extern short finbyr,finryr,fineyr;
extern RDAfinmgt *finsetup;
extern char editable;
extern MakeBrowseList *mbl_venvch;
extern void browse_venvch(RDArsrc *,mtnstruct *);
extern MaintainMaster *MASTER;
void venvchm(short,void *);

void venvchm(short dowhich,void *targetkey)
{
	char *name=NULL;
	short edit_rsrc=TRUE,nofields=0,nokeys=0,x=0,y=0;
	int which_type=0;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDArsrc *mtnrsrc=NULL;
	mtnstruct *mtn=NULL;
	RDAacct *acct=NULL;

	sht_pointer=Rmalloc(sizeof(short));
	*sht_pointer=0;
	mtnrsrc=RDArsrcNEW(MASTER->module,MASTER->maintain_screen);
	ZERNRD(MASTER->passkey->fileno);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->previous=RDATDataNEW(MASTER->passkey->fileno);
	mtn->DEFtypes=NULL;
	mtn->clearDEFtypes=NULL;
	mtn->ACCTtypes=NULL;
	mtn->DEFtypes=APPlibNEW();
	if(venpmt->use_venpmt_fund)
	{
		mtn->clearDEFtypes=APPlibNEW();
		if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
		{
			for(x=0,acct=BALANCE_SHEET_ACCOUNT->codetype;
				x<BALANCE_SHEET_ACCOUNT->num;++x,++acct)
			{
				addAPPlib(mtn->clearDEFtypes,acct->name);
			}
		}
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
			} else if(!RDAstrcmp(fldx->name,"CLEARING DEFINITION TYPE"))
			{
				if(venpmt->use_venpmt_fund)
				{
				if(FIELDscrn(MASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					addlstrsrc(mtnrsrc,"CLEARING DEFINITION TYPES",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						clearDEFtypecb,mtn->clearDEFtypes->numlibs,
						&mtn->clearDEFtypes->libs,mtn);
				}
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
	FINDRSCSETFUNC(mtnrsrc,"[VENVCH][ACCOUNT CODE]",acctcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[VENVCH][DEBIT CODE]",debcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[VENVCH][CREDIT CODE]",credcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[VENVCH][PAYABLE CODE]",paycodecb,mtn);
	if(venpmt->use_venpmt_fund)
	{
	FINDRSCSETFUNC(mtnrsrc,"[VENVCH][DUE TO]",duetocb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[VENVCH][CLEARING DUE FROM]",clearfromcb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[VENVCH][CLEARING CASH]",clearcashcb,mtn);
	}
	addstdrsrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"DEBIT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"CREDIT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"PAYABLE CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	if(venpmt->use_venpmt_fund)
	{
	addstdrsrc(mtnrsrc,"DUE TO DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"CLEARING DUE FROM DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"CLEARING CASH DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	}
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
					set_appdefaults(mtnrsrc,mtn,FALSE);
					list2file(mtnrsrc,mtn,FALSE);
				} else {
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					checkflds(mtnrsrc,mtn,FALSE);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				checkflds(mtnrsrc,mtn,FALSE);
			}
			break;
		case 2:
			ApplyPassKey(MASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
			if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
				set_appdefaults(mtnrsrc,mtn,FALSE);
				list2file(mtnrsrc,mtn,FALSE);
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				checkflds(mtnrsrc,mtn,FALSE);
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
					set_appdefaults(mtnrsrc,mtn,FALSE);
					list2file(mtnrsrc,mtn,FALSE);
				} else {
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					checkflds(mtnrsrc,mtn,FALSE);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				checkflds(mtnrsrc,mtn,FALSE);
			}
			break;
	}
	GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
	if(mbl_venvch!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_venvch,mtn);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,mtn);
	addbtnrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,mtn);
	MakeMasterButtons(mtnrsrc,MASTER);
	computeallSCRNvirtuals(mtnrsrc);
	if(mbl_venvch!=NULL)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	}
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,
		(mbl_venvch==NULL?TRUE:FALSE));
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
		member->parent,keyno,set_appdefaultscb,mtn,
		NULL,NULL,checkfldscb,mtn);
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
		member->parent,keyno,set_appdefaultscb,mtn,
		NULL,NULL,checkfldscb,mtn);
}
static void SETID(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ADV4SET_KEYFUNC(mtnrsrc,MASTER,mtn->previous,SCRNvirtualSubData,
		(void *)mtnrsrc,set_appdefaultscb,mtn,
		NULL,NULL,checkfldscb,mtn);
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
static short testavailbalance(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	int selecteda=0,selectedd=0,procyrid=0;
	short pmonth=0,deftype=0,testavl=TRUE;
	char *acctcode=NULL,*header=NULL,*message=NULL;
	double amount=0.0;

	if(FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selecteda)) return(FALSE);
	if(selecteda==EXP_ACCT)
	{
		if(FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selectedd)) return(FALSE);
		if(FINDRSCGETSTRING(mtnrsrc,"[VENVCH][ACCOUNT CODE]",&acctcode)) return(FALSE);
		if(FINDRSCGETDOUBLE(mtnrsrc,"[VENVCH][AMOUNT]",&amount)) return(FALSE);
		if(FINDRSCGETINT(mtnrsrc,"[VENVCH][FISCAL YEAR]",&procyrid)) return(FALSE);
/*
		if(FINDRSCGETSHORT(mtnrsrc,"[VENVCH][PROCESSING MONTH]",&pmonth)) return(FALSE);
*/
		if(FINDRSCGETSHORT(mtnrsrc,"[VENVCH][FISCAL PROCESSING MONTH]",&pmonth)) return(FALSE);
		ZERNRD(fineyr);
		FINDFLDSETINT(fineyr,"FISCAL YEAR",procyrid);
		deftype=(short)selectedd;
		FINDFLDSETSHORT(fineyr,"DEFINITION TYPE",deftype);
		FINDFLDSETSTRING(fineyr,"ACCOUNT CODE",acctcode);
		if(!EQLNRD(fineyr,1))
		{
			if(finsetup->budget==ANNUALBUDGET)
			{
				testavl=testapprotot(fineyr,amount,NULL);
			} else {
				if(venpmt->use_fiscal_months==TRUE)
				{
					testavl=testapprotot(fineyr,amount,FISCALPERIOD[pmonth]);
				} else {
					testavl=testapprotot(fineyr,amount,CALENDAR_MONTHS[pmonth]);
				}
			}
			if(testavl==FALSE)
			{
				header=stralloc("This Voucher will cause the POSTED APPROPRIATED amount to be less than all of the POSTED, DISTRIBUTED, and UNDISTRIBUTED EXPENDITURES and ENCUMBRANCES entered for the ACCOUNT [%s] in the YEAR [%d].");
				message=Rrealloc(message,RDAstrlen(header)+RDAstrlen(acctcode)+50);
				memset(message,0,RDAstrlen(message)+1);
				sprintf(message,header,acctcode,procyrid);
				*sht_pointer=update_list;
				ADVWARNINGDIALOG("WARNING DIALOG SCREEN","AVAILABLE BALANCE NOT ENOUGH!",message,(void (*)())save_record,NULL,FALSE,3,(void *)mtnrsrc,(void *)mtn,(void *)sht_pointer);
				if(header!=NULL) Rfree(header);
				if(message!=NULL) Rfree(message);
				if(acctcode!=NULL) Rfree(acctcode); 
				return(FALSE);
			} else {
				return(TRUE);
			}
		} else {
			/* no account code for that year */
			return(FALSE);
		}
	}
	return(TRUE);
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
	FINDRSCGETCHAR(mtnrsrc,"[VENVCH][DELETEFLAG]",&delflag);
	if(!delflag)
	{
		if(check_acctcode(mtnrsrc,mtn)==FALSE) return;
		if(check_debcode(mtnrsrc,mtn)==FALSE) return;
		if(check_credcode(mtnrsrc,mtn)==FALSE) return;
		if(check_paycode(mtnrsrc,mtn)==FALSE) return;
		if(venpmt->use_venpmt_fund)
		{
		if(check_dueto(mtnrsrc,mtn)==FALSE) return;
		if(check_clearfrom(mtnrsrc,mtn)==FALSE) return;
		if(check_clearcash(mtnrsrc,mtn)==FALSE) return;
		}
		if(testavailbalance(mtnrsrc,mtn,update)==FALSE) return;
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
	short numflds=0,y=0,p_month=0;
	char *rcddesc=NULL,*debcode=NULL,*credcode=NULL;
	short deftype=0,accttype=0/* ,jtype=0 */;
	short cdeftype=0;
	char delflag=FALSE;
	char dist=FALSE,cleared=FALSE;
	char reconciled=FALSE;
	int procyrid=0;
	double amount=0.0;
	NRDfield *tmpfld=NULL,*fldx=NULL,*tmp=NULL;

	FINDFLDGETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",&accttype);
	if(accttype==BAL_ACCT)
	{
		FINDRSCGETSTRING(mtnrsrc,"[VENVCH][DEBIT CODE]",&debcode);
		if(!isEMPTY(debcode))
		{
			FINDRSCSETSTRING(mtnrsrc,"[VENVCH][DEBIT CODE]",NULL);
			FINDFLDSETSTRING(MASTER->passkey->fileno,"DEBIT CODE",NULL);
			updatersrc(mtnrsrc,"[VENVCH][DEBIT CODE]");
			Rfree(debcode);
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
			prterr("Didn't write record.");
		}
	} else {
		numflds=NUMFLDS(MASTER->passkey->fileno);
		if(numflds>0)
		{
			tmpfld=Rmalloc(numflds*sizeof(NRDfield));
			for(y=0,fldx=tmpfld;y<numflds;++y,++fldx)
			{
				tmp=FLDNUM(MASTER->passkey->fileno,y);
				cpfldinfo(fldx,tmp);
			}
			LOCNRDFILE(MASTER->passkey->fileno);
			if(!ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
                		FINDFLDGETCHAR(MASTER->passkey->fileno,"DISTRIBUTED",&dist);
				if(venpmt->use_venpmt_fund)
				{
					FINDFLDGETCHAR(MASTER->passkey->fileno,"RECONCILED",&reconciled);
				}
                		FINDFLDGETCHAR(MASTER->passkey->fileno,"CLEARED",&cleared);
                		FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&delflag);
				if((dist==FALSE || cleared==FALSE) && delflag==FALSE)
				{
					FINDFLDGETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",&accttype);
					FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",&deftype);
					FINDFLDGETSHORT(MASTER->passkey->fileno,"CLEARING DEFINITION TYPE",&cdeftype);
					FINDFLDGETINT(MASTER->passkey->fileno,"FISCAL YEAR",&procyrid);
/*
					FINDFLDGETSHORT(MASTER->passkey->fileno,"PROCESSING MONTH",&p_month);
*/
					FINDFLDGETSHORT(MASTER->passkey->fileno,"FISCAL PROCESSING MONTH",&p_month);
					FINDFLDGETDOUBLE(MASTER->passkey->fileno,"AMOUNT",&amount);
					FINDFLDGETSTRING(MASTER->passkey->fileno,"DEBIT CODE",&debcode);
					FINDFLDGETSTRING(MASTER->passkey->fileno,"CREDIT CODE",&credcode);
					switch(accttype)
					{
						case EXP_ACCT:
							if(!isEMPTY(credcode))
							{
							if(venpmt->use_fiscal_months==TRUE)
							{
								ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",(dist==FALSE?"DEBIT CODE":NULL),(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",fineyr,finbyr,procyrid,deftype,"EXPENDITURES",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
							} else {
								ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",(dist==FALSE?"DEBIT CODE":NULL),(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",fineyr,finbyr,procyrid,deftype,"EXPENDITURES",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
							}
							} else {
							if(venpmt->use_fiscal_months==TRUE)
							{
								ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",(dist==FALSE?"DEBIT CODE":NULL),NULL,"PAYABLE CODE",fineyr,finbyr,procyrid,deftype,"EXPENDITURES",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
							} else {
								ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",(dist==FALSE?"DEBIT CODE":NULL),NULL,"PAYABLE CODE",fineyr,finbyr,procyrid,deftype,"EXPENDITURES",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
							}
							}
							break;
						case REV_ACCT:
							if(!isEMPTY(debcode))
							{
							if(venpmt->use_fiscal_months==TRUE)
							{
								ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",NULL,NULL,NULL,finryr,finbyr,procyrid,deftype,"REVENUE",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
								ADVupdateundist(MASTER->passkey->fileno,NULL,(dist==FALSE?"DEBIT CODE":NULL),(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",finryr,finbyr,procyrid,deftype,"REVENUE",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
							} else {
								ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",NULL,NULL,NULL,finryr,finbyr,procyrid,deftype,"REVENUE",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,mtnrsrc);
								ADVupdateundist(MASTER->passkey->fileno,NULL,(dist==FALSE?"DEBIT CODE":NULL),(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",finryr,finbyr,procyrid,deftype,"REVENUE",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
							}
							} else {
							if(venpmt->use_fiscal_months==TRUE)
							{
								ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",NULL,NULL,NULL,finryr,finbyr,procyrid,deftype,"REVENUE",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
								ADVupdateundist(MASTER->passkey->fileno,NULL,NULL,(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",finryr,finbyr,procyrid,deftype,"REVENUE",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
							} else {
								ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",NULL,NULL,NULL,finryr,finbyr,procyrid,deftype,"REVENUE",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,mtnrsrc);
								ADVupdateundist(MASTER->passkey->fileno,NULL,NULL,(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",finryr,finbyr,procyrid,deftype,"REVENUE",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
							}
							}
							break;
						case BAL_ACCT:
							if(venpmt->use_fiscal_months==TRUE)
							{
								ADVupdateundist(MASTER->passkey->fileno,NULL,(cleared==FALSE?"CREDIT CODE":NULL),(dist==FALSE?"ACCOUNT CODE":NULL),"PAYABLE CODE",(-1),finbyr,procyrid,deftype,"CREDITS",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
							} else {
								ADVupdateundist(MASTER->passkey->fileno,NULL,(cleared==FALSE?"CREDIT CODE":NULL),(dist==FALSE?"ACCOUNT CODE":NULL),"PAYABLE CODE",(-1),finbyr,procyrid,deftype,"CREDITS",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,mtnrsrc);
							}
							break;
						default:
							prterr("Invalid Account Type [%d]",accttype);
							break;
					} 
					if(venpmt->use_venpmt_fund && !cleared && !reconciled)
					{
					if(venpmt->use_fiscal_months==TRUE)
					{
						ADVupdateundist(MASTER->passkey->fileno,NULL,NULL,NULL,"DUE TO",(-1),finbyr,procyrid,deftype,"",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
						ADVupdateundist(MASTER->passkey->fileno,NULL,"CLEARING DUE FROM","CLEARING CASH",NULL,(-1),finbyr,procyrid,cdeftype,"",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
						ADVupdateundist(MASTER->passkey->fileno,NULL,"CLEARING CASH","CLEARING DUE FROM",NULL,(-1),finbyr,procyrid,cdeftype,"",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
					} else {
						ADVupdateundist(MASTER->passkey->fileno,NULL,NULL,NULL,"DUE TO",(-1),finbyr,procyrid,deftype,"",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,mtnrsrc);
						ADVupdateundist(MASTER->passkey->fileno,NULL,"CLEARING DUE FROM","CLEARING CASH",NULL,(-1),finbyr,procyrid,cdeftype,"",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,mtnrsrc);
						ADVupdateundist(MASTER->passkey->fileno,NULL,"CLEARING CASH","CLEARING DUE FROM",NULL,(-1),finbyr,procyrid,cdeftype,"",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,mtnrsrc);
					}
					}
					if(debcode!=NULL) Rfree(debcode);
					if(credcode!=NULL) Rfree(credcode);
				}
			}
			UNLNRDFILE(MASTER->passkey->fileno); /* moved this line up, so others can use it.  Should loop in a while(){} until it can lock..... locks in WRT's */
			fields2file(MASTER->passkey->fileno,tmpfld);
			WRTTRANS(MASTER->passkey->fileno,0,NULL,mtn->previous);
			FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&delflag);
			FINDFLDGETCHAR(MASTER->passkey->fileno,"DISTRIBUTED",&dist);
			if(venpmt->use_venpmt_fund)
			{
				FINDFLDGETCHAR(MASTER->passkey->fileno,"RECONCILED",&reconciled);
			}
			FINDFLDGETCHAR(MASTER->passkey->fileno,"CLEARED",&cleared);
			if(dist==FALSE && delflag==FALSE)
			{
				FINDFLDGETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",&accttype);
				FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",&deftype);
				FINDFLDGETSHORT(MASTER->passkey->fileno,"CLEARING DEFINITION TYPE",&cdeftype);
				FINDFLDGETINT(MASTER->passkey->fileno,"FISCAL YEAR",&procyrid);
/*
				FINDFLDGETSHORT(MASTER->passkey->fileno,"PROCESSING MONTH",&p_month);
*/
				FINDFLDGETSHORT(MASTER->passkey->fileno,"FISCAL PROCESSING MONTH",&p_month);
				FINDFLDGETDOUBLE(MASTER->passkey->fileno,"AMOUNT",&amount);
				FINDFLDGETSTRING(MASTER->passkey->fileno,"DEBIT CODE",&debcode);
				FINDFLDGETSTRING(MASTER->passkey->fileno,"CREDIT CODE",&credcode);
				switch(accttype)
				{
					case EXP_ACCT:
						if(!isEMPTY(credcode))
						{
						if(venpmt->use_fiscal_months==TRUE)
						{
							ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",(dist==FALSE?"DEBIT CODE":NULL),(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",fineyr,finbyr,procyrid,deftype,"EXPENDITURES",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
						} else {
							ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",(dist==FALSE?"DEBIT CODE":NULL),(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",fineyr,finbyr,procyrid,deftype,"EXPENDITURES",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,mtnrsrc);
						}
						} else {
						if(venpmt->use_fiscal_months==TRUE)
						{
							ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",(dist==FALSE?"DEBIT CODE":NULL),NULL,"PAYABLE CODE",fineyr,finbyr,procyrid,deftype,"EXPENDITURES",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
						} else {
							ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",(dist==FALSE?"DEBIT CODE":NULL),NULL,"PAYABLE CODE",fineyr,finbyr,procyrid,deftype,"EXPENDITURES",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,mtnrsrc);
						}
						}
						break;
					case REV_ACCT:
						if(!isEMPTY(debcode))
						{
						if(venpmt->use_fiscal_months==TRUE)
						{
							ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",NULL,NULL,NULL,finryr,finbyr,procyrid,deftype,"REVENUE",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
							ADVupdateundist(MASTER->passkey->fileno,NULL,(dist==FALSE?"DEBIT CODE":NULL),(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",finryr,finbyr,procyrid,deftype,"REVENUE",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
						} else {
							ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",NULL,NULL,NULL,finryr,finbyr,procyrid,deftype,"REVENUE",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
							ADVupdateundist(MASTER->passkey->fileno,NULL,(dist==FALSE?"DEBIT CODE":NULL),(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",finryr,finbyr,procyrid,deftype,"REVENUE",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,mtnrsrc);
						}
						} else {
						if(venpmt->use_fiscal_months==TRUE)
						{
							ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",NULL,NULL,NULL,finryr,finbyr,procyrid,deftype,"REVENUE",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
							ADVupdateundist(MASTER->passkey->fileno,NULL,NULL,(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",finryr,finbyr,procyrid,deftype,"REVENUE",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
						} else {
							ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE",NULL,NULL,NULL,finryr,finbyr,procyrid,deftype,"REVENUE",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
							ADVupdateundist(MASTER->passkey->fileno,NULL,NULL,(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",finryr,finbyr,procyrid,deftype,"REVENUE",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,mtnrsrc);
						}
						}
						break;
					case BAL_ACCT:
						if(venpmt->use_fiscal_months==TRUE)
						{
							ADVupdateundist(MASTER->passkey->fileno,NULL,(dist==FALSE?"ACCOUNT CODE":NULL),(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",(-1),finbyr,procyrid,deftype,"DEBITS",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
						} else {
							ADVupdateundist(MASTER->passkey->fileno,NULL,(dist==FALSE?"ACCOUNT CODE":NULL),(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",(-1),finbyr,procyrid,deftype,"DEBITS",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,mtnrsrc);
						}
						break;
					default:
						prterr("Invalid Account Type [%d]",accttype);
						break;
				} /* end switch/case */
				if(venpmt->use_venpmt_fund && !cleared && !reconciled)
				{
				if(venpmt->use_fiscal_months==TRUE)
				{
					ADVupdateundist(MASTER->passkey->fileno,NULL,NULL,NULL,"DUE TO",(-1),finbyr,procyrid,deftype,"",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
					ADVupdateundist(MASTER->passkey->fileno,NULL,"CLEARING DUE FROM","CLEARING CASH",NULL,(-1),finbyr,procyrid,cdeftype,"",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
					ADVupdateundist(MASTER->passkey->fileno,NULL,"CLEARING CASH","CLEARING DUE FROM",NULL,(-1),finbyr,procyrid,cdeftype,"",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
				} else {
					ADVupdateundist(MASTER->passkey->fileno,NULL,NULL,NULL,"DUE TO",(-1),finbyr,procyrid,deftype,"",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,mtnrsrc);
					ADVupdateundist(MASTER->passkey->fileno,NULL,"CLEARING DUE FROM","CLEARING CASH",NULL,(-1),finbyr,procyrid,cdeftype,"",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,mtnrsrc);
					ADVupdateundist(MASTER->passkey->fileno,NULL,"CLEARING CASH","CLEARING DUE FROM",NULL,(-1),finbyr,procyrid,cdeftype,"",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,mtnrsrc);
				}
				}
				if(debcode!=NULL) Rfree(debcode);
				if(credcode!=NULL) Rfree(credcode);
			}
			DELETE_SUBORDINATES(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
			updatebrowse(update,mbl_venvch,MASTER->passkey->fileno,MASTER->module,mtnrsrc);
		}
		if(mbl_venvch!=NULL) 
		{
			quit_record(mtnrsrc,mtn);
		} else {
			seteditable(mtnrsrc,mtn,TRUE);
			GetRDATData(MASTER->passkey->fileno,mtn->previous);
		}
	}
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->clearDEFtypes!=NULL) freeapplib(mtn->clearDEFtypes);
		if(mtn->ACCTtypes!=NULL) freeapplib(mtn->ACCTtypes);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(sht_pointer!=NULL) Rfree(sht_pointer);
	if(WindowCount<2)
	{
		CLOSE_MASTER_FILES(MASTER);
		if(finsetup!=NULL) free_finmgt(finsetup);
		if(venpmt!=NULL) free_venpmt(venpmt);
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
static void checkflds(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	setlists(mtnrsrc,mtn,update);
	filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
	checkaccts(mtnrsrc,mtn,update);
	seteditable(mtnrsrc,mtn,update);
}
static void checkfldscb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
	void (*SubFunc)(),void *args,mtnstruct *mtn)
{
	checkflds(mtnrsrc,mtn,TRUE);
}
static void set_appdefaults(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *date=NULL,*timex=NULL;
	short accttype=0;
	int selecteda=0;

	FINDRSCSETSTRING(mtnrsrc,"[VENVCH][SOURCE USER]",USERLOGIN);
	date=GETCURRENTDATE();
	timex=GETCURRENTTIME();
	FINDRSCSETSTRING(mtnrsrc,"[VENVCH][ENTRY DATE]",date);
	FINDRSCSETSTRING(mtnrsrc,"[VENVCH][ENTRY TIME]",timex);
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
	FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selecteda);
	accttype=(short)selecteda;
	if(accttype==BAL_ACCT)
	{
		FINDRSCSETEDITABLE(mtnrsrc,"[VENVCH][DEBIT CODE]",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[VENVCH][DEBIT CODE]",FALSE);
	}
	if(update)
	{
		updatersrc(mtnrsrc,"[VENVCH][DEBIT CODE]");
		updatersrc(mtnrsrc,"[VENVCH][SOURCE USER]");
		updatersrc(mtnrsrc,"[VENVCH][ENTRY DATE]");
		updatersrc(mtnrsrc,"[VENVCH][ENTRY TIME]");
	}
}
static void set_appdefaultscb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
	void (*SubFunc)(),void *args,mtnstruct *mtn)
{
	set_appdefaults(mtnrsrc,mtn,TRUE);
	list2file(mtnrsrc,mtn,TRUE);
}
static void seteditable(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	short accttype=0;
	char suv=FALSE;

	FINDRSCGETCHAR(mtnrsrc,"[VENVCH][SOURCE USER VERIFIED]",&suv);
	if(suv)
	{
		if(update)
		{
			updateeditfilersrcsnokeys(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,FALSE);
		} else {
			seteditfilersrc(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,FALSE,FALSE);
		}
		SetRDAScrolledListEditable(mtnrsrc,MASTER->passkey->fileno,MASTER->passkey->keyno,FALSE,FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"ACCOUNT TYPES",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"DEFINITION TYPES",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"[VENVCH][DELETEFLAG]",TRUE);
	} else {
		if(update)
		{
			updateeditfilersrcsnokeys(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,TRUE);
		} else {
			seteditfilersrc(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,TRUE,FALSE);
		}
		SetRDAScrolledListEditable(mtnrsrc,MASTER->passkey->fileno,MASTER->passkey->keyno,TRUE,TRUE);
		FINDFLDGETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",&accttype);
		if(accttype==BAL_ACCT)
		{
			FINDRSCSETEDITABLE(mtnrsrc,"[VENVCH][DEBIT CODE]",FALSE);
		}
		FINDRSCSETEDITABLE(mtnrsrc,"ACCOUNT TYPES",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"DEFINITION TYPES",TRUE);
	}
}
static void checkaccts(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	check_code(mtnrsrc,mtn,0);
	check_code(mtnrsrc,mtn,1);
	check_code(mtnrsrc,mtn,2);
	check_code(mtnrsrc,mtn,3);
	if(venpmt->use_venpmt_fund)
	{
	check_code(mtnrsrc,mtn,4);
	check_code(mtnrsrc,mtn,5);
	check_code(mtnrsrc,mtn,6);
	}
	if(update)
	{
		updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"PAYABLE CODE DESCRIPTION");
		updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
		if(venpmt->use_venpmt_fund)
		{
		updatersrc(mtnrsrc,"DUE TO DESCRIPTION");
		updatersrc(mtnrsrc,"CLEARING DUE FROM DESCRIPTION");
		updatersrc(mtnrsrc,"CLEARING CASH DESCRIPTION");
		}
	}
}
static short check_code(RDArsrc *mtnrsrc,mtnstruct *mtn,short acctwidget)
{
	short accttype=0,deftype=0,cdeftype=0;
	int selcdeftype=0,selaccttype=0,seldeftype=0,procyrid=0;
	char *acctcode=NULL,*tmp=NULL;

	FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selaccttype);
	accttype=(short)selaccttype;
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&seldeftype);
	deftype=(short)seldeftype;
	if(venpmt->use_venpmt_fund)
	{
	FINDRSCGETINT(mtnrsrc,"CLEARING DEFINITION TYPES",&selcdeftype);
	cdeftype=(short)selcdeftype;
	}
	FINDFLDGETINT(MASTER->passkey->fileno,"FISCAL YEAR",&procyrid);
	switch(acctwidget)
	{
		case 0:
			FINDRSCGETSTRING(mtnrsrc,"[VENVCH][ACCOUNT CODE]",&acctcode);
			switch(accttype)
			{
				case REV_ACCT:
					if((tmp=ADVCHECKACCT(finryr,finracc,acctcode,accttype,deftype,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
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
					if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode,accttype,deftype,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
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
					if((tmp=ADVCHECKACCT(fineyr,fineacc,acctcode,accttype,deftype,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
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
			FINDRSCGETSTRING(mtnrsrc,"[VENVCH][DEBIT CODE]",&acctcode);
			switch(accttype)
			{
				case REV_ACCT :
					if(!isEMPTY(acctcode))
					{
						if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode,accttype,deftype,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
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
					if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode,accttype,deftype,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
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
			FINDRSCGETSTRING(mtnrsrc,"[VENVCH][CREDIT CODE]",&acctcode);
			switch(accttype)
			{
				case EXP_ACCT :
					if(!isEMPTY(acctcode))
					{
						if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode,accttype,deftype,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
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
					if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode,accttype,deftype,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
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
			FINDRSCGETSTRING(mtnrsrc,"[VENVCH][PAYABLE CODE]",&acctcode);
			if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode,accttype,deftype,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
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
		case 4: /* due from */
			if(venpmt->use_venpmt_fund)
			{
			FINDRSCGETSTRING(mtnrsrc,"[VENVCH][DUE TO]",&acctcode);
			if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode,accttype,deftype,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DUE TO CODE!",tmp,NULL,NULL,TRUE,NULL);
				Rfree(tmp);
				if(acctcode!=NULL) Rfree(acctcode);
				FINDRSCSETSTRING(mtnrsrc,"DUE TO DESCRIPTION",NULL);
				return(FALSE);
			} else {
				if((tmp=ADVGETBALREF(finbacc,acctcode,deftype,SCRNvirtualSubData,mtnrsrc))!=NULL)
				{
					if(RDAstrcmp(tmp,"DUE TO"))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DUE TO CODE!","The DUE TO entered does not have the correct BALANCE SHEET REFERENCE TYPE of [DUE TO]." ,NULL,NULL,TRUE,NULL);
						Rfree(tmp);
						if(acctcode!=NULL) Rfree(acctcode);
						FINDRSCSETSTRING(mtnrsrc,"DUE TO DESCRIPTION",NULL);
						return(FALSE);
					} else {
						FINDFLDGETSTRING(finbacc,"DESCRIPTION",&tmp);
						FINDRSCSETSTRING(mtnrsrc,"DUE TO DESCRIPTION",tmp);
						if(tmp!=NULL) Rfree(tmp);
					}
				} else {
					WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DUE TO CODE!","The DUE TO entered does not have the correct BALANCE SHEET REFERENCE TYPE of [DUE TO]." ,NULL,NULL,TRUE,NULL);
					if(acctcode!=NULL) Rfree(acctcode);
					FINDRSCSETSTRING(mtnrsrc,"DUE TO DESCRIPTION",NULL);
					return(FALSE);
				}
			}
			}
			break;
		case 5: /* clearing due to */
			if(venpmt->use_venpmt_fund)
			{
			FINDRSCGETSTRING(mtnrsrc,"[VENVCH][CLEARING DUE FROM]",&acctcode);
			if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode,accttype,cdeftype,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CLEARING DUE FROM CODE!",tmp,NULL,NULL,TRUE,NULL);
				Rfree(tmp);
				if(acctcode!=NULL) Rfree(acctcode);
				FINDRSCSETSTRING(mtnrsrc,"CLEARING DUE FROM DESCRIPTION",NULL);
				return(FALSE);
			} else {
				if((tmp=ADVGETBALREF(finbacc,acctcode,cdeftype,SCRNvirtualSubData,mtnrsrc))!=NULL)
				{
					if(RDAstrcmp(tmp,"DUE FROM"))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CLEARING DUE FROM CODE!","The CLEARING DUE FROM entered does not have the correct BALANCE SHEET REFERENCE TYPE of [DUE FROM]." ,NULL,NULL,TRUE,NULL);
						Rfree(tmp);
						if(acctcode!=NULL) Rfree(acctcode);
						FINDRSCSETSTRING(mtnrsrc,"CLEARING DUE FROM DESCRIPTION",NULL);
						return(FALSE);
					} else {
						FINDFLDGETSTRING(finbacc,"DESCRIPTION",&tmp);
						FINDRSCSETSTRING(mtnrsrc,"CLEARING DUE FROM DESCRIPTION",tmp);
						if(tmp!=NULL) Rfree(tmp);
					}
				} else {
					WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CLEARING DUE FROM CODE!","The CLEARING DUE FROM entered does not have the correct BALANCE SHEET REFERENCE TYPE of [DUE TO]." ,NULL,NULL,TRUE,NULL);
					if(acctcode!=NULL) Rfree(acctcode);
					FINDRSCSETSTRING(mtnrsrc,"CLEARING DUE FROM DESCRIPTION",NULL);
					return(FALSE);
				}
			}
			}
			break;
		case 6: /* clearign cash */
			if(venpmt->use_venpmt_fund)
			{
			FINDRSCGETSTRING(mtnrsrc,"[VENVCH][CLEARING CASH]",&acctcode);
			if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode,accttype,cdeftype,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CLEARING CASH!",tmp,NULL,NULL,TRUE,NULL);
				Rfree(tmp);
				if(acctcode!=NULL) Rfree(acctcode);
				FINDRSCSETSTRING(mtnrsrc,"CLEARING CASH DESCRIPTION",NULL);
				return(FALSE);
			} else {
				if((tmp=ADVGETBALREF(finbacc,acctcode,cdeftype,SCRNvirtualSubData,mtnrsrc))!=NULL)
				{
					if(RDAstrcmp(tmp,"CASH"))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CLEARING CASH CODE!","The CLEARING CASH entered does not have the correct BALANCE SHEET REFERENCE TYPE of [CASH]." ,NULL,NULL,TRUE,NULL);
						Rfree(tmp);
						if(acctcode!=NULL) Rfree(acctcode);
						FINDRSCSETSTRING(mtnrsrc,"CLEARING CASH DESCRIPTION",NULL);
						return(FALSE);
					} else {
						FINDFLDGETSTRING(finbacc,"DESCRIPTION",&tmp);
						FINDRSCSETSTRING(mtnrsrc,"CLEARING CASH DESCRIPTION",tmp);
						if(tmp!=NULL) Rfree(tmp);
					}
				} else {
					WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CLEARING CASH CODE!","The CLEARING CASH entered does not have the correct BALANCE SHEET REFERENCE TYPE of [CASH]." ,NULL,NULL,TRUE,NULL);
					if(acctcode!=NULL) Rfree(acctcode);
					FINDRSCSETSTRING(mtnrsrc,"CLEARING CASH DESCRIPTION",NULL);
					return(FALSE);
				}
			}
			}
			break;
		default:
			prterr("\"acctwidget\" value of [%d] out of range.",acctwidget);
			break;
	}
	return(TRUE);
}
static short check_acctcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	return(check_code(mtnrsrc,mtn,0));
}
static void acctcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readwidget(mtnrsrc,"[VENVCH][FISCAL YEAR]");
	readwidget(mtnrsrc,"[VENVCH][ACCOUNT CODE]");
	check_acctcode(mtnrsrc,mtn);
	updatersrc(mtnrsrc,"[VENVCH][ACCOUNT CODE]");
	updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static short check_debcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	return(check_code(mtnrsrc,mtn,1));
}
static void debcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readwidget(mtnrsrc,"[VENVCH][FISCAL YEAR]");
	readwidget(mtnrsrc,"[VENVCH][DEBIT CODE]");
	check_debcode(mtnrsrc,mtn);
	updatersrc(mtnrsrc,"[VENVCH][DEBIT CODE]");
	updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static short check_credcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	return(check_code(mtnrsrc,mtn,2));
}
static void credcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readwidget(mtnrsrc,"[VENVCH][FISCAL YEAR]");
	readwidget(mtnrsrc,"[VENVCH][CREDIT CODE]");
	check_credcode(mtnrsrc,mtn);
	updatersrc(mtnrsrc,"[VENVCH][CREDIT CODE]");
	updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static short check_clearfrom(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	return(check_code(mtnrsrc,mtn,5));
}
static short check_clearcash(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	return(check_code(mtnrsrc,mtn,6));
}
static short check_dueto(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	return(check_code(mtnrsrc,mtn,4));
}
static short check_paycode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	return(check_code(mtnrsrc,mtn,3));
}
static void paycodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readwidget(mtnrsrc,"[VENVCH][FISCAL YEAR]");
	readwidget(mtnrsrc,"[VENVCH][PAYABLE CODE]");
	check_paycode(mtnrsrc,mtn);
	updatersrc(mtnrsrc,"[VENVCH][PAYABLE CODE]");
	updatersrc(mtnrsrc,"PAYABLE CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static void duetocb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readwidget(mtnrsrc,"[VENVCH][FISCAL YEAR]");
	readwidget(mtnrsrc,"[VENVCH][DUE TO]");
	check_dueto(mtnrsrc,mtn);
	updatersrc(mtnrsrc,"[VENVCH][DUE TO]");
	updatersrc(mtnrsrc,"DUE TO DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static void clearfromcb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readwidget(mtnrsrc,"[VENVCH][FISCAL YEAR]");
	readwidget(mtnrsrc,"[VENVCH][CLEARING DUE FROM]");
	check_clearfrom(mtnrsrc,mtn);
	updatersrc(mtnrsrc,"[VENVCH][CLEARING DUE FROM]");
	updatersrc(mtnrsrc,"CLEARING DUE FROM DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static void clearcashcb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readwidget(mtnrsrc,"[VENVCH][FISCAL YEAR]");
	readwidget(mtnrsrc,"[VENVCH][CLEARING CASH]");
	check_clearcash(mtnrsrc,mtn);
	updatersrc(mtnrsrc,"[VENVCH][CLEARING CASH]");
	updatersrc(mtnrsrc,"CLEARING CASH DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static void clearDEFtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	changeclearDEFtype(mtnrsrc,mtn,TRUE);
	updateSCRNvirtuals(mtnrsrc);
}
static void changeclearDEFtype(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	int selectedd=0;
	short deftype1=0;
	short deftype2=0;

	if(FINDRSCGETINT(mtnrsrc,"CLEARING DEFINITION TYPES",&selectedd)) return;
	deftype2=(short)selectedd;
	FINDRSCGETDEFTYPE(mtnrsrc,"[VENVCH][CLEARING DUE FROM]",&deftype1);
	if(deftype1!=deftype2)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"[VENVCH][CLEARING DUE FROM]",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[VENVCH][CLEARING CASH]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[VENVCH][CLEARING DUE FROM]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[VENVCH][CLEARING CASH]",TRUE);
		FINDRSCSETSTRING(mtnrsrc,"[VENVCH][CLEARING DUE FROM]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[VENVCH][CLEARING CASH]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"CLEARING DUE FROM DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"CLEARING CASH DESCRIPTION",NULL);
		if(update)
		{
			updatersrc(mtnrsrc,"[VENVCH][CLEARING DUE FROM]");
			updatersrc(mtnrsrc,"[VENVCH][CLEARING CASH]");
			updatersrc(mtnrsrc,"CLEARING DUE FROM DESCRIPTION");
			updatersrc(mtnrsrc,"CLEARING CASH DESCRIPTION");
		}
		FINDRSCSETDEFTYPE(mtnrsrc,"[VENVCH][CLEARING DUE FROM]",deftype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[VENVCH][CLEARING CASH]",deftype2);
	}
}
static void DEFtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	changeDEFtype(mtnrsrc,mtn,TRUE);
	updateSCRNvirtuals(mtnrsrc);
}
static void changeDEFtype(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	int selecteda=0,selectedd=0;
	short accttype1=0,deftype1=0;
	short accttype2=0,deftype2=0;

	if(FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selecteda)) return;
	if(FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selectedd)) return;
	FINDRSCGETACCTTYPE(mtnrsrc,"[VENVCH][ACCOUNT CODE]",&accttype1);
	FINDRSCGETDEFTYPE(mtnrsrc,"[VENVCH][ACCOUNT CODE]",&deftype1);
	accttype2=(short)selecteda;
	deftype2=(short)selectedd;
	if(accttype1!=accttype2 || deftype1!=deftype2)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"[VENVCH][DEBIT CODE]",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[VENVCH][CREDIT CODE]",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[VENVCH][PAYABLE CODE]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[VENVCH][ACCOUNT CODE]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[VENVCH][DEBIT CODE]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[VENVCH][CREDIT CODE]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[VENVCH][PAYABLE CODE]",TRUE);
		FINDRSCSETSTRING(mtnrsrc,"[VENVCH][ACCOUNT CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[VENVCH][DEBIT CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[VENVCH][CREDIT CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[VENVCH][PAYABLE CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[VENVCH][DUE TO]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"PAYABLE CODE DESCRIPTION",NULL);
		if(venpmt->use_venpmt_fund)
		{
		FINDRSCSETSENSITIVE(mtnrsrc,"[VENVCH][DUE TO]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[VENVCH][DUE TO]",TRUE);
		FINDRSCSETSTRING(mtnrsrc,"DUE TO CODE DESCRIPTION",NULL);
		}
		if(update)
		{
			updatersrc(mtnrsrc,"[VENVCH][ACCOUNT CODE]");
			updatersrc(mtnrsrc,"[VENVCH][DEBIT CODE]");
			updatersrc(mtnrsrc,"[VENVCH][CREDIT CODE]");
			updatersrc(mtnrsrc,"[VENVCH][PAYABLE CODE]");
			updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
			updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
			updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
			updatersrc(mtnrsrc,"PAYABLE CODE DESCRIPTION");
		if(venpmt->use_venpmt_fund)
		{
			updatersrc(mtnrsrc,"[VENVCH][DUE TO]");
			updatersrc(mtnrsrc,"DUE TO DESCRIPTION");
		}
		}
		FINDRSCSETACCTTYPE(mtnrsrc,"[VENVCH][ACCOUNT CODE]",accttype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[VENVCH][ACCOUNT CODE]",deftype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[VENVCH][DEBIT CODE]",deftype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[VENVCH][CREDIT CODE]",deftype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[VENVCH][PAYABLE CODE]",deftype2);
		if(venpmt->use_venpmt_fund)
		{
		FINDRSCSETDEFTYPE(mtnrsrc,"[VENVCH][DUE TO]",deftype2);
		}
		switch(selecteda)
		{
			case EXP_ACCT:
			case REV_ACCT:
				FINDRSCSETSENSITIVE(mtnrsrc,"[VENVCH][DEBIT CODE]",TRUE);
				FINDRSCSETEDITABLE(mtnrsrc,"[VENVCH][DEBIT CODE]",TRUE);
				break;
			case BAL_ACCT:
				FINDRSCSETSENSITIVE(mtnrsrc,"[VENVCH][DEBIT CODE]",FALSE);
				FINDRSCSETEDITABLE(mtnrsrc,"[VENVCH][DEBIT CODE]",FALSE);
				break;
			default:
				prterr("Invalid Account Type [%d]",selecteda);
		}
	}
}
static void setACCTtype1(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	int selaccttype=0,seldeftype=0;
	short accttype=0,deftype=0;

	FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selaccttype);
	accttype=(short)selaccttype;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",accttype);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&seldeftype);
	deftype=(short)seldeftype;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",deftype);
	if(venpmt->use_venpmt_fund)
	{
	FINDRSCGETINT(mtnrsrc,"CLEARING DEFINITION TYPES",&seldeftype);
	deftype=(short)seldeftype;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"CLEARING DEFINITION TYPE",deftype);
	}
	setACCTtype(mtnrsrc,mtn,update);
}
static void setACCTtype(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	short accttype=0,deftype=0;
	int selecteda=0,selectedd=0;

	FINDFLDGETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",&accttype);
	selecteda=(int)accttype;
	FINDRSCSETINT(mtnrsrc,"ACCOUNT TYPES",selecteda);
	if(update) updatersrc(mtnrsrc,"ACCOUNT TYPES");
	changeACCTtype(mtnrsrc,mtn);
	if(FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",&deftype)) return;
	selectedd=(int)deftype;
	if(selectedd>=mtn->DEFtypes->numlibs) selectedd=0;
	if(!FINDRSCLISTAPPlib(mtnrsrc,"DEFINITION TYPES",selectedd,mtn->DEFtypes))
	{
		if(update) updatersrc(mtnrsrc,"DEFINITION TYPES");
	}
	changeDEFtype(mtnrsrc,mtn,update);
	if(venpmt->use_venpmt_fund)
	{
	if(FINDFLDGETSHORT(MASTER->passkey->fileno,"CLEARING DEFINITION TYPE",&deftype)) return;
	selectedd=(int)deftype;
	if(selectedd>=mtn->clearDEFtypes->numlibs) selectedd=0;
	if(!FINDRSCLISTAPPlib(mtnrsrc,"CLEARING DEFINITION TYPES",selectedd,mtn->DEFtypes))
	{
		if(update) updatersrc(mtnrsrc,"CLEARING DEFINITION TYPES");
	}
	changeclearDEFtype(mtnrsrc,mtn,update);
	}
}
static void ACCTtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selecteda=0,selectedd=0;
	short accttype=0,deftype=0;

	if(FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selecteda)) return;
	accttype=(short)selecteda;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",accttype);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selectedd);
	deftype=(short)selectedd;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",deftype);
	changeACCTtype(mtnrsrc,mtn);
	if(selectedd>=mtn->DEFtypes->numlibs) selectedd=0;
	if(!FINDRSCLISTAPPlib(mtnrsrc,"DEFINITION TYPES",selectedd,mtn->DEFtypes))
	{
		updatersrc(mtnrsrc,"DEFINITION TYPES");
	}
	changeDEFtype(mtnrsrc,mtn,TRUE);
	updateSCRNvirtuals(mtnrsrc);
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
static void makeACCTtypes(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn->ACCTtypes!=NULL) freeapplib(mtn->ACCTtypes);
	mtn->ACCTtypes=APPlibNEW();
	addAPPlib(mtn->ACCTtypes,accttypes[0]);
	addAPPlib(mtn->ACCTtypes,accttypes[1]);
	addAPPlib(mtn->ACCTtypes,accttypes[2]);
}
