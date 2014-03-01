/* puraccm.c - PURCHASE ORDER ACCOUNT MASTER MAINTAIN */
#include <cstdio>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <fin.hpp>
#include <purord.hpp>

struct mtnstructs
{
	APPlib *ACCTtypes;
	APPlib *DEFtypes;
	RDATData *previous;
	APPlib *clearDEFtypes;
};
typedef struct mtnstructs mtnstruct;

extern RDApurord *purord;
extern short purmst,purlin,finbyr,fineyr,finryr;
extern short finbacc,fineacc,finracc;
extern MakeBrowseList *mbl_puracc;
extern MaintainMaster *MASTER;
extern char editable;
extern RDAvenpmt *venpmt;
extern void browse_puracc(RDArsrc *,mtnstruct *);
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
static void SetAppDef(RDArsrc *,mtnstruct *mtn,short);
static void SetAppDefcb(RDArsrc *,MaintainMaster *,void (*)(...),void *,mtnstruct
*);
static void seteditable(RDArsrc *,mtnstruct *,short);
static void setlists(RDArsrc *,mtnstruct *,short);
static void checkflds(RDArsrc *,mtnstruct *,short);
static void checkfldscb(RDArsrc *,MaintainMaster *,void (*)(...),void *,mtnstruct *);
static void resetdefs(RDArsrc *,mtnstruct *);
static void getpreviouscb(RDArmem *,mtnstruct *);
static void getnextcb(RDArmem *,mtnstruct *);
static void SETID(RDArsrc *,mtnstruct *);
static void SETFILES(RDArsrc *,MaintainMaster *);
static void makeACCTtypes(RDArsrc *,mtnstruct *);
static void acctcodecb(RDArsrc *,mtnstruct *);
static void debcodecb(RDArsrc *,mtnstruct *);
static void credcodecb(RDArsrc *,mtnstruct *);
static void defdebcodecb(RDArsrc *,mtnstruct *);
static void defcredcodecb(RDArsrc *,mtnstruct *);
static void defpaycodecb(RDArsrc *,mtnstruct *);
static short check_code(RDArsrc *,mtnstruct *,short);
static short check_acctcode(RDArsrc *,mtnstruct *);
static short check_debcode(RDArsrc *,mtnstruct *);
static short check_credcode(RDArsrc *,mtnstruct *);
static short check_defdebcode(RDArsrc *,mtnstruct *);
static short check_defcredcode(RDArsrc *,mtnstruct *);
static short check_defpaycode(RDArsrc *,mtnstruct *);
static void checkaccts(RDArsrc *,mtnstruct *,short);
static void save_check(RDArsrc *,mtnstruct *,short);
static void ACCTtypecb(RDArsrc *,mtnstruct *);
static void setACCTtype(RDArsrc *,mtnstruct *,short);
static void setACCTtype1(RDArsrc *,mtnstruct *,short);
static void changeACCTtype(RDArsrc *,mtnstruct *);
static void DEFtypecb(RDArsrc *,mtnstruct *);
static void clearDEFtypecb(RDArsrc *,mtnstruct *);
static void changeDEFtype(RDArsrc *,mtnstruct *,short);
static short check_defcredcode(RDArsrc *,mtnstruct *);
static short check_defclearto(RDArsrc *,mtnstruct *);
static short check_defclearcash(RDArsrc *,mtnstruct *);
static void defcleartocb(RDArsrc *,mtnstruct *);
static void defclearcashcb(RDArsrc *,mtnstruct *);
static void changeclearDEFtype(RDArsrc *,mtnstruct *,short);
void puraccm(short,void *);

void puraccm(short dowhich,void *targetkey)
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

	mtnrsrc=RDArsrcNEW(MASTER->module,MASTER->maintain_screen);
	ZERNRD(MASTER->passkey->fileno);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->previous=RDATDataNEW(MASTER->passkey->fileno);
	mtn->DEFtypes=NULL;
	mtn->ACCTtypes=NULL;
	mtn->clearDEFtypes=NULL;
	mtn->clearDEFtypes=APPlibNEW();
	mtn->DEFtypes=APPlibNEW();
	if(venpmt->use_venpmt_fund)
	{
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
			} else if(!RDAstrcmp(fldx->name,"DEFAULT CLEARING DEFINITION TYPE"))
			{
				if(venpmt->use_venpmt_fund)
				{
				if(FIELDscrn(MASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					addlstrsrc(mtnrsrc,"DEFAULT CLEARING DEFINITION TYPES",
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
	FINDRSCSETFUNC(mtnrsrc,"[PURACC][ACCOUNT CODE]",acctcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[PURACC][DEBIT CODE]",debcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[PURACC][CREDIT CODE]",credcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[PURACC][DEFAULT DEBIT CODE]",defdebcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[PURACC][DEFAULT PAYABLE CODE]",defpaycodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[PURACC][DEFAULT CREDIT CODE]",defcredcodecb,mtn);
	addstdrsrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"DEBIT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"CREDIT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"DEFAULT DEBIT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"DEFAULT CREDIT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"DEFAULT PAYABLE CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	if(venpmt->use_venpmt_fund)
	{
		FINDRSCSETFUNC(mtnrsrc,"[PURACC][DEFAULT CLEARING DUE FROM]",defcleartocb,mtn);
		addstdrsrc(mtnrsrc,"DEFAULT CLEARING DUE FROM DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
		FINDRSCSETFUNC(mtnrsrc,"[PURACC][DEFAULT CLEARING CASH]",defclearcashcb,mtn);
		addstdrsrc(mtnrsrc,"DEFAULT CLEARING CASH DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
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
					SetAppDef(mtnrsrc,mtn,FALSE);
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
				SetAppDef(mtnrsrc,mtn,FALSE);
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
					SetAppDef(mtnrsrc,mtn,FALSE);
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
	if(mbl_puracc!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_puracc,mtn);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,mtn);
	addbtnrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,mtn);
	MakeMasterButtons(mtnrsrc,MASTER);
	computeallSCRNvirtuals(mtnrsrc);
	if(mbl_puracc!=NULL)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	}
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,
		(mbl_puracc==NULL?TRUE:FALSE));
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
		member->parent,keyno,SetAppDefcb,mtn,
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
		member->parent,keyno,SetAppDefcb,mtn,
		NULL,NULL,checkfldscb,mtn);
}
static void SETID(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ADV4SET_KEYFUNC(mtnrsrc,MASTER,mtn->previous,SCRNvirtualSubData,
		(void *)mtnrsrc,SetAppDefcb,mtn,
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
	setACCTtype(mtnrsrc,mtn,update);
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
	FINDRSCGETCHAR(mtnrsrc,"[PURACC][DELETEFLAG]",&delflag);
	if(!delflag)
	{
		if(check_acctcode(mtnrsrc,mtn)==FALSE) return;
		if(check_debcode(mtnrsrc,mtn)==FALSE) return;
		if(check_credcode(mtnrsrc,mtn)==FALSE) return;
		if(check_defdebcode(mtnrsrc,mtn)==FALSE) return;
		if(check_defcredcode(mtnrsrc,mtn)==FALSE) return;
		if(check_defpaycode(mtnrsrc,mtn)==FALSE) return;
		if(venpmt->use_venpmt_fund)
		{
			if(check_defclearto(mtnrsrc,mtn)==FALSE) return;
			if(check_defclearcash(mtnrsrc,mtn)==FALSE) return;
		}
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
	char deleteflag=0,pdeleteflag=0,*poid1=NULL,*poid2=NULL;
	short transnum=0,fineyr=0,finryr=0,finbyr=0;
	int yearid1=0,yearid2=0,lineno1=0,lineno2=0;
	short ef_trans=0,accttype=0,deftype=0,p_month=0;
	char dist=FALSE;
	double amount=0.0,adminfee=0.0,prorateamt=0.0;
	double tax_amount=0.0,cost=0.0;
	RDArsrc *tmprsrc=NULL;
	RDATData *prev=NULL;

	if(ADVRECORDsec(MASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(MASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("SECURITY ACCESS DENIED!",rcddesc,NULL,FALSE);
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(MASTER->passkey->fileno,0,NULL,mtn->previous);
		FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			transnum=OPNNRDsec("PURORD","PURTRN",TRUE,TRUE,FALSE);
			fineyr=OPNNRDsec("FINMGT","FINEYR",TRUE,TRUE,FALSE);
			finryr=OPNNRDsec("FINMGT","FINRYR",TRUE,TRUE,FALSE);
			finbyr=OPNNRDsec("FINMGT","FINBYR",TRUE,TRUE,FALSE);
			tmprsrc=RDArsrcNEW(MASTER->module,NULL);
			copyDFincvir(mtnrsrc,tmprsrc);
			addDFincvir(tmprsrc,"PURORD","PURTRN",NULL,transnum);
			addDFincvir(tmprsrc,"FINMGT","FINEYR",NULL,fineyr);
			addDFincvir(tmprsrc,"FINMGT","FINRYR",NULL,finryr);
			addDFincvir(tmprsrc,"FINMGT","FINBYR",NULL,finbyr);
			FINDFLDGETINT(MASTER->passkey->fileno,"FISCAL YEAR",&yearid1);
			FINDFLDGETSTRING(MASTER->passkey->fileno,"PO IDENTIFICATION",&poid1);
			FINDFLDGETINT(MASTER->passkey->fileno,"LINE NUMBER",&lineno1);
			if(transnum!=(-1) && fineyr!=(-1) && finryr!=(-1) && finbyr!=(-1))
			{
				ZERNRD(transnum);
				FINDFLDSETINT(transnum,"FISCAL YEAR",yearid1);
				FINDFLDSETSTRING(transnum,"PO IDENTIFICATION",poid1);
				FINDFLDSETINT(transnum,"LINE NUMBER",lineno1);
				ef_trans=ADVGTENRDsec(transnum,1,SCRNvirtualSubData,tmprsrc);
				while(!ef_trans)
				{
					FINDFLDGETCHAR(transnum,"DELETEFLAG",&pdeleteflag);
					if(!pdeleteflag)
					{
						FINDFLDGETINT(transnum,"FISCAL YEAR",&yearid2);
						FINDFLDGETSTRING(transnum,"PO IDENTIFICATION",&poid2);
						FINDFLDGETINT(transnum,"LINE NUMBER",&lineno2);
						if(yearid2==yearid1
							&& !RDAstrcmp(poid2,poid1)	
							&& lineno2==lineno1) 
						{
							prev=RDATDataNEW(transnum);
							FINDFLDGETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",&accttype);
							FINDFLDGETDOUBLE(transnum,"AMOUNT",&amount);
							if(venpmt->Use_Taxes==TRUE)
							{
								FINDFLDGETDOUBLE(transnum,"COST",&cost);
								FINDFLDGETDOUBLE(transnum,"TAX AMOUNT",&tax_amount);
								FINDFLDGETDOUBLE(transnum,"PRORATE AMOUNT",&prorateamt);
								FINDFLDGETDOUBLE(transnum,"ADMIN FEE",&adminfee);
							}
							if(accttype==EXP_ACCT)
							{
								ADV4updateoutstanding(purmst,purlin,yearid2,poid2,lineno2,-cost,-tax_amount,-adminfee,-prorateamt,-amount,TRUE,1,SCRNvirtualSubData,tmprsrc,venpmt);
							}
							FINDFLDGETCHAR(transnum,"DISTRIBUTED",&dist);
							if(dist==FALSE)
							{
								FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",&deftype);
								FINDFLDGETSHORT(transnum,"PROCESSING MONTH",&p_month);
								switch(accttype)
								{
									case EXP_ACCT:
										if(purord->use_fiscal_months==TRUE)
										{
										ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,fineyr,finbyr,yearid2,deftype,"ENCUMBRANCES",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,tmprsrc);
										} else {
										ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,fineyr,finbyr,yearid2,deftype,"ENCUMBRANCES",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,tmprsrc);
										}
										break;
									case REV_ACCT:
										if(purord->use_fiscal_months==TRUE)
										{
										ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,finryr,finbyr,yearid2,deftype,"ESTIMATED REVENUE",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,tmprsrc);
										} else {
										ADVupdateundist(MASTER->passkey->fileno,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,finryr,finbyr,yearid2,deftype,"ESTIMATED REVENUE",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,tmprsrc);
										}
										break;
									case BAL_ACCT:
										if(amount<0.0)
										{
										if(purord->use_fiscal_months==TRUE)
										{
/*
											ADVupdateundist(MASTER->passkey->fileno,NULL,"ACCOUNT CODE","CREDIT CODE",NULL,(-1),finbyr,yearid2,deftype,"DEBITS",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,tmprsrc);
*/
											ADVupdateundist(MASTER->passkey->fileno,NULL,"DEBIT CODE","CREDIT CODE",NULL,(-1),finbyr,yearid2,deftype,"DEBITS",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,tmprsrc);
										} else {
/*
											ADVupdateundist(MASTER->passkey->fileno,NULL,"ACCOUNT CODE","CREDIT CODE",NULL,(-1),finbyr,yearid2,deftype,"DEBITS",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,tmprsrc);
*/
											ADVupdateundist(MASTER->passkey->fileno,NULL,"DEBIT CODE","CREDIT CODE",NULL,(-1),finbyr,yearid2,deftype,"DEBITS",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,tmprsrc);
										}
										} else {
										if(purord->use_fiscal_months==TRUE)
										{
/*
											ADVupdateundist(MASTER->passkey->fileno,NULL,"ACCOUNT CODE","CREDIT CODE",NULL,(-1),finbyr,yearid2,deftype,"CREDITS",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,tmprsrc);
*/
											ADVupdateundist(MASTER->passkey->fileno,NULL,"DEBIT CODE","CREDIT CODE",NULL,(-1),finbyr,yearid2,deftype,"CREDITS",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,tmprsrc);
										} else {
/*
											ADVupdateundist(MASTER->passkey->fileno,NULL,"ACCOUNT CODE","CREDIT CODE",NULL,(-1),finbyr,yearid2,deftype,"CREDITS",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,tmprsrc);
											ADVupdateundist(MASTER->passkey->fileno,NULL,"DEBIT CODE","CREDIT CODE",NULL,(-1),finbyr,yearid2,deftype,"CREDITS",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,tmprsrc);
*/
										}
										}
										break;
									default:
										prterr("Invalid Account Type [%d]",accttype);
										break;
								} /* end switch/case */
							} 
							FINDFLDSETCHAR(transnum,"DELETEFLAG",deleteflag);
							ADVWRTTRANSsec(transnum,0,NULL,prev,SCRNvirtualSubData,tmprsrc);		
							if(prev!=NULL) FreeRDATData(prev);
						}
					}
					ef_trans=ADVNXTNRDsec(transnum,1,SCRNvirtualSubData,tmprsrc);
				}
			}
			if(transnum!=(-1)) CLSNRD(transnum);
			if(fineyr!=(-1)) CLSNRD(fineyr);
			if(finryr!=(-1)) CLSNRD(finryr);
			if(finbyr!=(-1)) CLSNRD(finbyr);
		}
		if(tmprsrc!=NULL) free_rsrc(tmprsrc);
		if(poid1!=NULL) Rfree(poid1);
		if(poid2!=NULL) Rfree(poid2);
		DELETE_SUBORDINATES(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update,mbl_puracc,MASTER->passkey->fileno,MASTER->module,mtnrsrc);
		if(mbl_puracc!=NULL)
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
		if(mtn->ACCTtypes!=NULL) freeapplib(mtn->ACCTtypes);
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(WindowCount<2)
	{
		CLOSE_MASTER_FILES(MASTER);
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
		if(venpmt!=NULL) free_venpmt(venpmt);
		if(purord!=NULL) free_purord(purord);
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
	setACCTtype1(mtnrsrc,mtn,update);
}
static void SetAppDef(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	short accttype=0;
	int selecteda=0;

	FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selecteda);
	accttype=(short)selecteda;
	if(accttype==BAL_ACCT)
	{
/*
		FINDRSCSETEDITABLE(mtnrsrc,"[PURACC][DEBIT CODE]",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[PURACC][DEFAULT DEBIT CODE]",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PURACC][DEBIT CODE]",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[PURACC][DEFAULT DEBIT CODE]",FALSE);
*/
		FINDRSCSETEDITABLE(mtnrsrc,"[PURACC][ACCOUNT CODE]",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[PURACC][ACCOUNT CODE]",FALSE);
	}
	if(update)
	{
		updatersrc(mtnrsrc,"[PURACC][DEBIT CODE]");
		updatersrc(mtnrsrc,"[PURACC][DEFAULT DEBIT CODE]");
	}
}
static void SetAppDefcb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
void (*SubFunc)(...),void *args,mtnstruct *mtn)
{
	SetAppDef(mtnrsrc,mtn,TRUE);
	list2file(mtnrsrc,mtn,TRUE);
}
static void checkflds(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	setlists(mtnrsrc,mtn,update);
	filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
        seteditable(mtnrsrc,mtn,update);
        checkaccts(mtnrsrc,mtn,update);
}
static void checkfldscb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
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
	check_defcredcode(mtnrsrc,mtn);
	updatersrc(mtnrsrc,"DEFAULT CREDIT CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static short check_defcredcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	return(check_code(mtnrsrc,mtn,4));
}
static short check_defclearto(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	return(check_code(mtnrsrc,mtn,7));
}
static short check_defclearcash(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	return(check_code(mtnrsrc,mtn,8));
}
static void defcleartocb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readscreen(mtnrsrc,mtn);
	check_defclearto(mtnrsrc,mtn);
	updatersrc(mtnrsrc,"DEFAULT CLEARING DUE FROM DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static void defclearcashcb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readscreen(mtnrsrc,mtn);
	check_defclearcash(mtnrsrc,mtn);
	updatersrc(mtnrsrc,"DEFAULT CLEARING CASH DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
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
	if(venpmt->use_venpmt_fund)
	{
		check_code(mtnrsrc,mtn,6);
		check_code(mtnrsrc,mtn,7);
		check_code(mtnrsrc,mtn,8);
	}
	if(update)
	{
		updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"DEFAULT DEBIT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"DEFAULT PAYABLE CODE DESCRIPTION");
		updatersrc(mtnrsrc,"DEFAULT CREDIT CODE DESCRIPTION");
		if(venpmt->use_venpmt_fund)
		{
			updatersrc(mtnrsrc,"DEFAULT CLEAR DUE TO DESCRIPTION");
			updatersrc(mtnrsrc,"DEFAULT CLEAR CASH DESCRIPTION");
		}
	}
}
static short check_code(RDArsrc *mtnrsrc,mtnstruct *mtn,short acctwidget)
{
	short accttype1=0,deftype1=0,cdeftype1=0;
	int selaccttype=0,seldeftype=0,yearid1=0,selcdeftype=0;
	char *acctcode1=NULL,*tmp2=NULL,*tmp=NULL;

	FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selaccttype);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&seldeftype);
	if(venpmt->use_venpmt_fund)
	{
		FINDRSCGETINT(mtnrsrc,"DEFAULT CLEARING DEFINITION TYPES",&selcdeftype);
		cdeftype1=(short)selcdeftype;
	}
	accttype1=(short)selaccttype;
	deftype1=(short)seldeftype;
	FINDRSCGETINT(mtnrsrc,"[PURACC][FISCAL YEAR]",&yearid1);
	switch(acctwidget)
	{
		case 0:
			FINDRSCGETSTRING(mtnrsrc,"[PURACC][ACCOUNT CODE]",&acctcode1);
			switch(accttype1)
			{
				case EXP_ACCT:
					if((tmp=ADVCHECKACCT(fineyr,fineacc,acctcode1,accttype1,deftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID EXPENDITURE ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
						clearwidget(mtnrsrc,"[PURACC][ACCOUNT CODE]");
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
				case REV_ACCT:
					if((tmp=ADVCHECKACCT(finryr,finracc,acctcode1,accttype1,deftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID REVENUE ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
						clearwidget(mtnrsrc,"[PURACC][ACCOUNT CODE]");
						clearwidget(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
						Rfree(tmp);
						if(acctcode1!=NULL) Rfree(acctcode1);
						return(FALSE);
                                        } else {
						FINDFLDGETSTRING(finracc,"DESCRIPTION",&tmp);
						FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",tmp);
						if(tmp!=NULL) Rfree(tmp);
					}
					break;
				case BAL_ACCT:
					if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode1,accttype1,deftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BALANCE SHEET ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
						clearwidget(mtnrsrc,"[PURACC][ACCOUNT CODE]");
						clearwidget(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
						Rfree(tmp);
						if(acctcode1!=NULL) Rfree(acctcode1);
						return(FALSE);
					} else {
						if(RDAstrcmp(tmp=ADVGETBALREF(finbacc,acctcode1,deftype1,SCRNvirtualSubData,mtnrsrc),"ENCUMBRANCE"))
						{
							if(tmp2!=NULL) { Rfree(tmp2); tmp2=NULL; }
							tmp2=Rmalloc(RDAstrlen(tmp)+RDAstrlen(acctcode1)+178);
							sprintf(tmp2,"The Balance Sheet Account Code [%s] exist and is active for this ACCOUNT CODE but its REFERENCE TYPE is [%s].\nA REFERENCE TYPE of [ENCUMBRANCE] is the required type.",acctcode1,tmp);
							WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE REFERENCE TYPE!",tmp2,NULL,NULL,TRUE,NULL);
							clearwidget(mtnrsrc,"[PURACC][CREDIT CODE]");
							clearwidget(mtnrsrc,"CREDIT CODE DESCRIPTION");
							if(tmp!=NULL) free(tmp); 
							if(tmp2!=NULL) Rfree(tmp2); tmp2=NULL;
							if(acctcode1!=NULL) Rfree(acctcode1);
							return(FALSE);
                                        	} else {
							FINDFLDGETSTRING(finbacc,"DESCRIPTION",&tmp);
							FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",tmp);
							if(tmp!=NULL) Rfree(tmp);
						}
					}
					break;
				default :
					prterr("Invalid Account Type [%d]",accttype1);
					break;
			}
			break;
		case 1:
			FINDRSCGETSTRING(mtnrsrc,"[PURACC][DEBIT CODE]",&acctcode1);
			switch(accttype1)
			{
				case BAL_ACCT:
					if(tmp!=NULL) Rfree(tmp); 
					FINDRSCSETSTRING(mtnrsrc,"[PURACC][DEBIT CODE]",tmp);
					updatersrc(mtnrsrc,"[PURACC][DEBIT CODE]");
					FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",tmp);
					updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
					break;
				case EXP_ACCT:
				case REV_ACCT:
					if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode1,accttype1,deftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!",tmp,NULL,NULL,TRUE,NULL);
						clearwidget(mtnrsrc,"[PURACC][DEBIT CODE]");
						clearwidget(mtnrsrc,"DEBIT CODE DESCRIPTION");
						Rfree(tmp);
						if(acctcode1!=NULL) Rfree(acctcode1);
						return(FALSE);
					} else {
						if(RDAstrcmp(tmp=ADVGETBALREF(finbacc,acctcode1,deftype1,SCRNvirtualSubData,mtnrsrc),"ENCUMBRANCE"))
						{
							if(tmp2!=NULL) Rfree(tmp2);
							tmp2=Rmalloc(RDAstrlen(tmp)+RDAstrlen(acctcode1)+175);
							sprintf(tmp2,"The Balance Sheet Account Code [%s] exist and is active for this DEBIT CODE but its REFERENCE TYPE is [%s].\nA REFERENCE TYPE of [ENCUMBRANCE] is the required type.",acctcode1,tmp);
							WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE REFERENCE TYPE!",tmp2,NULL,NULL,TRUE,NULL);
							clearwidget(mtnrsrc,"[PURACC][DEBIT CODE]");
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
				default :
					prterr("Invalid Account Type [%d]",accttype1);
					break;
			} /* end switch/case */
			break;
		case 2:
			FINDRSCGETSTRING(mtnrsrc,"[PURACC][CREDIT CODE]",&acctcode1);
			switch(accttype1)
			{
				case BAL_ACCT:
				case EXP_ACCT:
				case REV_ACCT:
					if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode1,accttype1,deftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE!",tmp,NULL,NULL,TRUE,NULL);
						clearwidget(mtnrsrc,"[PURACC][CREDIT CODE]");
						clearwidget(mtnrsrc,"CREDIT CODE DESCRIPTION");
						Rfree(tmp);
						if(acctcode1!=NULL) Rfree(acctcode1);
						return(FALSE);
					} else {
						if(RDAstrcmp(tmp=ADVGETBALREF(finbacc,acctcode1,deftype1,SCRNvirtualSubData,mtnrsrc),"RESERVE FOR ENCUMBRANCE"))
						{
							if(tmp2!=NULL) Rfree(tmp2);
							tmp2=Rmalloc(RDAstrlen(tmp)+RDAstrlen(acctcode1)+179);
							sprintf(tmp2,"The Balance Sheet Account Code [%s] exist and is active for this CREDIT CODE but its REFERENCE TYPE is [%s].\nA REFERENCE TYPE of [RESERVE FOR ENCUMBRANCE] is the required type.",acctcode1,tmp);
							WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE REFERENCE TYPE!",tmp2,NULL,NULL,TRUE,NULL);
							clearwidget(mtnrsrc,"[PURACC][CREDIT CODE]");
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
					}
					break;
				default :
					prterr("Invalid Account Type [%d]",accttype1);
					break;
			} /* end switch/case */
			break;
		case 3:
			FINDRSCGETSTRING(mtnrsrc,"[PURACC][DEFAULT DEBIT CODE]",&acctcode1);
			switch(accttype1)
			{
				case REV_ACCT:
					if(!isEMPTY(acctcode1))
					{
						if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode1,accttype1,deftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEFAULT DEBIT CODE!",tmp,NULL,NULL,TRUE,NULL);
							clearwidget(mtnrsrc,"[PURACC][DEFAULT DEBIT CODE]");
							clearwidget(mtnrsrc,"DEFAULT DEBIT CODE DESCRIPTION");
							Rfree(tmp);
							if(acctcode1!=NULL) Rfree(acctcode1);
							return(FALSE);
						}
					} else {
						FINDFLDGETSTRING(finbacc,"DESCRIPTION",&tmp);
						FINDRSCSETSTRING(mtnrsrc,"DEFAULT DEBIT CODE DESCRIPTION",tmp);
						if(tmp!=NULL) Rfree(tmp);
					}
					break;
				case EXP_ACCT:
					if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode1,accttype1,deftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEFAULT DEBIT CODE!",tmp,NULL,NULL,TRUE,NULL);
						clearwidget(mtnrsrc,"[PURACC][DEFAULT DEBIT CODE]");
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
				case BAL_ACCT:
					if(tmp!=NULL) Rfree(tmp);
					FINDRSCSETSTRING(mtnrsrc,"[PURACC][DEFAULT DEBIT CODE]",tmp);
					updatersrc(mtnrsrc,"[PURACC][DEFAULT DEBIT CODE]");
					FINDRSCSETSTRING(mtnrsrc,"DEFAULT DEBIT CODE DESCRIPTION",tmp);
					updatersrc(mtnrsrc,"DEFAULT DEBIT CODE DESCRIPTION");
					break;
				default :
					prterr("Invalid Account Type [%d]",accttype1);
					break;
			} /* end switch/case */
			break;
		case 4:
			FINDRSCGETSTRING(mtnrsrc,"[PURACC][DEFAULT CREDIT CODE]",&acctcode1);
			switch(accttype1)
			{
				case EXP_ACCT:
					if(!isEMPTY(acctcode1))
					{
						if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode1,accttype1,deftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
						{
							WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEFAULT CREDIT CODE!",tmp,NULL,NULL,TRUE,NULL);
							clearwidget(mtnrsrc,"[PURACC][DEFAULT CREDIT CODE]");
							clearwidget(mtnrsrc,"DEFAULT CREDIT CODE DESCRIPTION");
							Rfree(tmp);
							if(acctcode1!=NULL) Rfree(acctcode1);
							return(FALSE);
						} else {
							FINDFLDGETSTRING(finbacc,"DESCRIPTION",&tmp);
							FINDRSCSETSTRING(mtnrsrc,"DEFAULT CREDIT CODE DESCRIPTION",tmp);
							if(tmp!=NULL) Rfree(tmp);
						}
					}
					break;
				case BAL_ACCT:
				case REV_ACCT:
					if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode1,accttype1,deftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEFAULT CREDIT CODE!",tmp,NULL,NULL,TRUE,NULL);
						clearwidget(mtnrsrc,"[PURACC][DEFAULT CREDIT CODE]");
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
				default :
					prterr("Invalid Account Type [%d]",accttype1);
					break;
			} /* end switch/case */
			break;
		case 5:
			FINDRSCGETSTRING(mtnrsrc,"[PURACC][DEFAULT PAYABLE CODE]",&acctcode1);
			if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode1,accttype1,deftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEFAULT PAYABLE CODE!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(mtnrsrc,"[PURACC][DEFAULT PAYABLE CODE]");
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
					clearwidget(mtnrsrc,"[PURACC][DEFAULT PAYABLE CODE]");
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
		case 6: /* due from */
			break;
		case 7: /* Clear To */
			FINDRSCGETSTRING(mtnrsrc,"[PURACC][DEFAULT CLEARING DUE FROM]",&acctcode1);
			if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode1,accttype1,cdeftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEFAULT CLEARING DUE FROM!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(mtnrsrc,"[PURACC][DEFAULT CLEARING DUE FROM]");
				clearwidget(mtnrsrc,"DEFAULT CLEARING DUE FROM DESCRIPTION");
				Rfree(tmp);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			} else {
				if(RDAstrcmp(tmp=ADVGETBALREF(finbacc,acctcode1,cdeftype1,SCRNvirtualSubData,mtnrsrc),"DUE FROM"))
				{
					if(tmp2!=NULL) Rfree(tmp2);
					tmp2=Rmalloc(RDAstrlen(tmp)+RDAstrlen(acctcode1)+210);
					sprintf(tmp2,"The Balance Sheet Account Code [%s] exist and is active for this DEFAULT CLEARING DUE FROM but its REFERENCE TYPE is [%s].\nA REFERENCE TYPE of [DUE FROM] is the required type.",acctcode1,tmp);
					WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEFAULT CLEARING DUE FROM REFERENCE TYPE!",tmp2,NULL,NULL,TRUE,NULL);
					clearwidget(mtnrsrc,"[PURACC][DEFAULT CLEARING DUE FROM]");
					clearwidget(mtnrsrc,"DEFAULT CLEARING DUE FROM DESCRIPTION");
					if(tmp!=NULL) Rfree(tmp); 
					if(tmp2!=NULL) Rfree(tmp2); 
					if(acctcode1!=NULL) Rfree(acctcode1);
					return(FALSE);
				} else {
					FINDFLDGETSTRING(finbacc,"DESCRIPTION",&tmp);
					FINDRSCSETSTRING(mtnrsrc,"DEFAULT CLEARING DUE FROM DESCRIPTION",tmp);
					if(tmp!=NULL) Rfree(tmp);
				}
			}
			break;
		case 8: /* Clear Cash */
			FINDRSCGETSTRING(mtnrsrc,"[PURACC][DEFAULT CLEARING CASH]",&acctcode1);
			if((tmp=ADVCHECKACCT(finbyr,finbacc,acctcode1,accttype1,cdeftype1,yearid1,SCRNvirtualSubData,mtnrsrc))!=NULL)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEFAULT CLEARING CASH!",tmp,NULL,NULL,TRUE,NULL);
				clearwidget(mtnrsrc,"[PURACC][DEFAULT CLEARING CASH]");
				clearwidget(mtnrsrc,"DEFAULT CLEARING CASH DESCRIPTION");
				Rfree(tmp);
				if(acctcode1!=NULL) Rfree(acctcode1);
				return(FALSE);
			} else {
				if(RDAstrcmp(tmp=ADVGETBALREF(finbacc,acctcode1,cdeftype1,SCRNvirtualSubData,mtnrsrc),"CASH"))
				{
					if(tmp2!=NULL) Rfree(tmp2);
					tmp2=Rmalloc(RDAstrlen(tmp)+RDAstrlen(acctcode1)+195);
					sprintf(tmp2,"The Balance Sheet Account Code [%s] exist and is active for this DEFAULT CLEARING CASH but its REFERENCE TYPE is [%s].\nA REFERENCE TYPE of [CASH] is the required type.",acctcode1,tmp);
					WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEFAULT CLEARING CASH REFERENCE TYPE!",tmp2,NULL,NULL,TRUE,NULL);
					clearwidget(mtnrsrc,"[PURACC][DEFAULT CLEARING CASH]");
					clearwidget(mtnrsrc,"DEFAULT CLEARING CASH DESCRIPTION");
					if(tmp!=NULL) Rfree(tmp); 
					if(tmp2!=NULL) Rfree(tmp2); 
					if(acctcode1!=NULL) Rfree(acctcode1);
					return(FALSE);
				} else {
					FINDFLDGETSTRING(finbacc,"DESCRIPTION",&tmp);
					FINDRSCSETSTRING(mtnrsrc,"DEFAULT CLEARING CASH DESCRIPTION",tmp);
					if(tmp!=NULL) Rfree(tmp);
				}
			}
			break;
		default:
			prterr("\"acctwidget\" value of [%d] out of range.",acctwidget);
			break;
	}
	if(acctcode1!=NULL) Rfree(acctcode1); 
	return(TRUE);
}
static void clearDEFtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	changeclearDEFtype(mtnrsrc,mtn,TRUE);
	FINDRSCSETSTRING(mtnrsrc,"[PURACC][CLEARING DUE FROM]",NULL);
	FINDRSCSETSTRING(mtnrsrc,"[PURACC][CLEARING CASH]",NULL);
	updatersrc(mtnrsrc,"[PURACC][CLEARING DUE FROM]");
	updatersrc(mtnrsrc,"[PURACC][CLEARING CASH]");
	updateSCRNvirtuals(mtnrsrc);
}
static void DEFtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	changeDEFtype(mtnrsrc,mtn,TRUE);
	updateSCRNvirtuals(mtnrsrc);
}
/*
static void DEFtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selecteda=0;
	short accttype=0;

	if(FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selecteda)) return;
	accttype=(short)selecteda;
	changeDEFtype(mtnrsrc,mtn,TRUE);
	FINDRSCSETSTRING(mtnrsrc,"[PURACC][ACCOUNT CODE]",NULL);
	FINDRSCSETSTRING(mtnrsrc,"[PURACC][DEBIT CODE]",NULL);
	FINDRSCSETSTRING(mtnrsrc,"[PURACC][CREDIT CODE]",NULL);
	FINDRSCSETSTRING(mtnrsrc,"[PURACC][DEFAULT DEBIT CODE]",NULL);
	FINDRSCSETSTRING(mtnrsrc,"[PURACC][DEFAULT CREDIT CODE]",NULL);
	FINDRSCSETSTRING(mtnrsrc,"[PURACC][DEFAULT PAYABLE CODE]",NULL);
	if(accttype==BAL_ACCT)
	{
		FINDRSCSETEDITABLE(mtnrsrc,"[PURACC][ACCOUNT CODE]",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[PURACC][ACCOUNT CODE]",FALSE);
	}
	updatersrc(mtnrsrc,"[PURACC][ACCOUNT CODE]");
	updatersrc(mtnrsrc,"[PURACC][DEBIT CODE]");
	updatersrc(mtnrsrc,"[PURACC][CREDIT CODE]");
	updatersrc(mtnrsrc,"[PURACC][DEFAULT DEBIT CODE]");
	updatersrc(mtnrsrc,"[PURACC][DEFAULT CREDIT CODE]");
	updatersrc(mtnrsrc,"[PURACC][DEFAULT PAYABLE CODE]");
	updateSCRNvirtuals(mtnrsrc);
}
*/
static void changeclearDEFtype(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	int selectedd=0;
	short deftype1=0;
	short deftype2=0;

	if(FINDRSCGETINT(mtnrsrc,"DEFAULT CLEARING DEFINITION TYPES",&selectedd)) return;
	FINDRSCGETDEFTYPE(mtnrsrc,"[PURACC][DEFAULT CLEARING DUE FROM]",&deftype1);
	deftype2=(short)selectedd;
	if(deftype1!=deftype2)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"[PURACC][DEFAULT CLEARING DUE FROM]",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[PURACC][DEFAULT CLEARING CASH]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PURACC][DEFAULT CLEARING DUE FROM]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PURACC][DEFAULT CLEARING CASH]",TRUE);
		FINDRSCSETSTRING(mtnrsrc,"[PURACC][DEFAULT CLEARING DUE FROM]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"DEFAULT CLEARING DUE FROM DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"DEFAULT CLEARING CASH DESCRIPTION",NULL);
		if(update)
		{
			updatersrc(mtnrsrc,"[PURACC][DEFAULT CLEARING DUE FROM]");
			updatersrc(mtnrsrc,"[PURACC][DEFAULT CLEARING CASH]");
			updatersrc(mtnrsrc,"DEFAULT CLEARING DUE FROM DESCRIPTION");
			updatersrc(mtnrsrc,"DEFAULT CLEARING CASH DESCRIPTION");
		}
		FINDRSCSETDEFTYPE(mtnrsrc,"[PURACC][DEFAULT CLEARING DUE FROM]",deftype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[PURACC][DEFAULT CLEARING CASH]",deftype2);
	}
}
static void changeDEFtype(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	int selecteda=0,selectedd=0;
	short accttype1=0,deftype1=0;
	short accttype2=0,deftype2=0;

	if(FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selecteda)) return;
	if(FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selectedd)) return;
	FINDRSCGETACCTTYPE(mtnrsrc,"[PURACC][ACCOUNT CODE]",&accttype1);
	FINDRSCGETDEFTYPE(mtnrsrc,"[PURACC][ACCOUNT CODE]",&deftype1);
	accttype2=(short)selecteda;
	deftype2=(short)selectedd;
	if(accttype1!=accttype2 || deftype1!=deftype2)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"[PURACC][ACCOUNT CODE]",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[PURACC][DEBIT CODE]",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[PURACC][CREDIT CODE]",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[PURACC][DEFAULT DEBIT CODE]",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[PURACC][DEFAULT CREDIT CODE]",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[PURACC][DEFAULT PAYABLE CODE]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PURACC][ACCOUNT CODE]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PURACC][DEBIT CODE]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PURACC][CREDIT CODE]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PURACC][DEFAULT DEBIT CODE]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PURACC][DEFAULT CREDIT CODE]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PURACC][DEFAULT PAYABLE CODE]",TRUE);
		FINDRSCSETSTRING(mtnrsrc,"[PURACC][ACCOUNT CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[PURACC][DEBIT CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[PURACC][CREDIT CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[PURACC][DEFAULT DEBIT CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[PURACC][DEFAULT CREDIT CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[PURACC][DEFAULT PAYABLE CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"DEFAULT DEBIT CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"DEFAULT CREDIT CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"DEFAULT PAYABLE CODE DESCRIPTION",NULL);
		if(update)
		{
			updatersrc(mtnrsrc,"[PURACC][ACCOUNT CODE]");
			updatersrc(mtnrsrc,"[PURACC][DEBIT CODE]");
			updatersrc(mtnrsrc,"[PURACC][CREDIT CODE]");
			updatersrc(mtnrsrc,"[PURACC][DEFAULT DEBIT CODE]");
			updatersrc(mtnrsrc,"[PURACC][DEFAULT CREDIT CODE]");
			updatersrc(mtnrsrc,"[PURACC][DEFAULT PAYABLE CODE]");
			updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
			updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
			updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
			updatersrc(mtnrsrc,"DEFAULT DEBIT CODE DESCRIPTION");
			updatersrc(mtnrsrc,"DEFAULT CREDIT CODE DESCRIPTION");
			updatersrc(mtnrsrc,"DEFAULT PAYABLE CODE DESCRIPTION");
		}
		FINDRSCSETACCTTYPE(mtnrsrc,"[PURACC][ACCOUNT CODE]",accttype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[PURACC][ACCOUNT CODE]",deftype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[PURACC][DEBIT CODE]",deftype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[PURACC][CREDIT CODE]",deftype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[PURACC][DEFAULT DEBIT CODE]",deftype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[PURACC][DEFAULT CREDIT CODE]",deftype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[PURACC][DEFAULT PAYABLE CODE]",deftype2);
		switch(selecteda)
		{
			case EXP_ACCT:
			case REV_ACCT:
				FINDRSCSETSENSITIVE(mtnrsrc,"[PURACC][DEBIT CODE]",TRUE);
				FINDRSCSETEDITABLE(mtnrsrc,"[PURACC][DEBIT CODE]",TRUE);
				FINDRSCSETSENSITIVE(mtnrsrc,"[PURACC][DEFAULT DEBIT CODE]",TRUE);
				FINDRSCSETEDITABLE(mtnrsrc,"[PURACC][DEFAULT DEBIT CODE]",TRUE);
				break;
			case BAL_ACCT:
/*
				FINDRSCSETSENSITIVE(mtnrsrc,"[PURACC][DEBIT CODE]",FALSE);
				FINDRSCSETEDITABLE(mtnrsrc,"[PURACC][DEBIT CODE]",FALSE);
				FINDRSCSETSENSITIVE(mtnrsrc,"[PURACC][DEFAULT DEBIT CODE]",FALSE);
				FINDRSCSETEDITABLE(mtnrsrc,"[PURACC][DEFAULT DEBIT CODE]",FALSE);
*/
				FINDRSCSETSENSITIVE(mtnrsrc,"[PURACC][ACCOUNT CODE]",FALSE);
				FINDRSCSETEDITABLE(mtnrsrc,"[PURACC][ACCOUNT CODE]",FALSE);
				break;
			default:
				prterr("Invalid Account Type [%d]",selecteda);
		}
	}
}
static void setACCTtype1(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	short deftype=0,accttype=0;
	int seldeftype=0,selaccttype=0;

	FINDFLDGETSHORT(MASTER->passkey->fileno,"ACCOUNT TYPE",&accttype);
	FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",&deftype);
	selaccttype=accttype;
	seldeftype=deftype;
	FINDRSCSETINT(mtnrsrc,"ACCOUNT TYPES",selaccttype);
	FINDRSCSETINT(mtnrsrc,"DEFINITION TYPES",seldeftype);
	if(venpmt->use_venpmt_fund)
	{
	deftype=0;
	FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFAULT CLEARING DEFINITION TYPE",&deftype);
	seldeftype=deftype;
	FINDRSCSETINT(mtnrsrc,"DEFAULT CLEARING DEFINITION TYPES",seldeftype);
	}
	if(update)
	{
		updatersrc(mtnrsrc,"ACCOUNT TYPES");
		updatersrc(mtnrsrc,"DEFINITION TYPES");
		if(venpmt->use_venpmt_fund)
		{
		updatersrc(mtnrsrc,"DEFAULT CLEARING DEFINITION TYPES");
		}
	}
	setACCTtype(mtnrsrc,mtn,update);
}
static void setACCTtype(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	int seldeftype=0;

	changeACCTtype(mtnrsrc,mtn);
	if(FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&seldeftype)) return;
	if(mtn->DEFtypes->numlibs<1)
	{
		addAPPlib(mtn->DEFtypes,"No Definitions Available");
	}
	FINDRSCLISTAPPlib(mtnrsrc,"DEFINITION TYPES",seldeftype,mtn->DEFtypes);
	if(update)
	{
		updatersrc(mtnrsrc,"DEFINITION TYPES");
	}
	changeDEFtype(mtnrsrc,mtn,update);
	if(venpmt->use_venpmt_fund)
	{
	seldeftype=0;
	if(FINDRSCGETINT(mtnrsrc,"DEFAULT CLEARING DEFINITION TYPES",&seldeftype)) return;
	if(mtn->clearDEFtypes->numlibs<1)
	{
		addAPPlib(mtn->clearDEFtypes,"No Definitions Available");
	}
	FINDRSCLISTAPPlib(mtnrsrc,"DEFAULT CLEARING DEFINITION TYPES",seldeftype,mtn->clearDEFtypes);
	if(update)
	{
		updatersrc(mtnrsrc,"DEFAULT CLEARING DEFINITION TYPES");
	}
	changeclearDEFtype(mtnrsrc,mtn,update);
	}
}
static void changeACCTtype(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected=0;
	int x=0;
	RDAacct *acct=NULL;

	if(FINDRSCGETINT(mtnrsrc,"ACCOUNT TYPES",&selected)) return;
	if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
	mtn->DEFtypes=APPlibNEW();
	switch(selected)
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
			prterr("Invalid Account Type [%d]",selected);
			break;
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
static void seteditable(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char closed=FALSE;
	short accttype=0;

	FINDFLDGETCHAR(purmst,"CLOSED",&closed);
	if(closed)
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
		FINDRSCSETEDITABLE(mtnrsrc,"[PURACC][DELETEFLAG]",TRUE);
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
/*
			FINDRSCSETEDITABLE(mtnrsrc,"[PURACC][DEBIT CODE]",FALSE);
			FINDRSCSETSENSITIVE(mtnrsrc,"[PURACC][DEBIT CODE]",FALSE);
			FINDRSCSETEDITABLE(mtnrsrc,"[PURACC][DEFAULT DEBIT CODE]",FALSE);
			FINDRSCSETSENSITIVE(mtnrsrc,"[PURACC][DEFAULT DEBIT CODE]",FALSE);
*/
			FINDRSCSETEDITABLE(mtnrsrc,"[PURACC][ACCOUNT CODE]",FALSE);
			FINDRSCSETSENSITIVE(mtnrsrc,"[PURACC][ACCOUNT CODE]",FALSE);
		}
		FINDRSCSETEDITABLE(mtnrsrc,"ACCOUNT TYPES",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"DEFINITION TYPES",TRUE);
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
