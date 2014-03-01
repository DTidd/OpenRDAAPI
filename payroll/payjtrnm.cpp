/* payjtrnm.c - Xpert Payroll Job Pay Transactions Maintain Screen */
#include <cstdio>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <fin.hpp>

struct mtnstructs
{
	APPlib *DEFtypes;
	APPlib *PDEFtypes;
	short deftype;
	short pdeftype;
};
typedef struct mtnstructs mtnstruct;

extern short paynum;
extern short baccnum,eaccnum;
extern short byrnum,eyrnum;
/*
extern char editable;
*/
extern MakeBrowseList *mbl_payjtrn;
extern MaintainMaster *MASTER;
extern void browse_payjtrn(RDArsrc *,mtnstruct *);
static void quit_record(RDArsrc *,mtnstruct *);
static void makeDEFtypes(RDArsrc *,mtnstruct *);
static void setDEFtypes(RDArsrc *,mtnstruct *);
static void makePDEFtypes(RDArsrc *,mtnstruct *);
static short check_code(RDArsrc *,mtnstruct *,short,int);
static void check_accts(RDArsrc *,mtnstruct *);
static void setlists(RDArsrc *,mtnstruct *,short);
static void NEfunction(RDArsrc *,MaintainMaster *,void (*)(...),void *,mtnstruct *);
static void BUEQLfunction(RDArsrc *,MaintainMaster *,void (*)(...),void *,mtnstruct *);
static void AUEQLfunction(RDArsrc *,MaintainMaster *,void (*)(...),void *,mtnstruct *);
static void SETID(RDArsrc *,mtnstruct *);
static void resetdefs(RDArsrc *,mtnstruct *);
static void getpreviouscb(RDArmem *,mtnstruct *);
static void getnextcb(RDArmem *,mtnstruct *);
static void getprevnxt(RDArmem *,mtnstruct *,char);
void payjtrnm(short,void *);

static void getpreviouscb(RDArmem *member,mtnstruct *mtn)
{
	getprevnxt(member,mtn,0);
}
static void getnextcb(RDArmem *member,mtnstruct *mtn)
{
	getprevnxt(member,mtn,1);
}
static void getprevnxt(RDArmem *member,mtnstruct *mtn,char next)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+(next?5:9)));
	if(keyno==(-1)) keyno=MASTER->passkey->keyno;
	FINDRSCSETEDITABLE(member->parent,"DEFINITION TYPES",TRUE);
	FINDRSCSETEDITABLE(member->parent,"PAYROLL FUND DEFINITION TYPES",TRUE);
	seteditfilersrc(MASTER->passkey->fileno,MASTER->passkey->keyno,member->parent,TRUE,FALSE);
	if(next)
	{
		ADV2GET_NEXT(member->parent,MASTER,NULL,SCRNvirtualSubData,
			member->parent,keyno,NEfunction,mtn,BUEQLfunction,mtn,
			AUEQLfunction,mtn);
	} else {
		ADV2GET_PREVIOUS(member->parent,MASTER,NULL,SCRNvirtualSubData,
			member->parent,keyno,NEfunction,mtn,BUEQLfunction,mtn,
			AUEQLfunction,mtn);
	}
	seteditfilersrc(MASTER->passkey->fileno,MASTER->passkey->keyno,member->parent,FALSE,FALSE);
	FINDRSCSETEDITABLE(member->parent,"DEFINITION TYPES",FALSE);
	FINDRSCSETEDITABLE(member->parent,"PAYROLL FUND DEFINITION TYPES",FALSE);
}
static void SETID(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	FINDRSCSETEDITABLE(mtnrsrc,"DEFINITION TYPES",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",TRUE);
	seteditfilersrc(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,TRUE,FALSE);
	ADV4SET_KEYFUNC(mtnrsrc,MASTER,NULL,SCRNvirtualSubData,(void *)mtnrsrc,NEfunction,mtn,BUEQLfunction,mtn,AUEQLfunction,mtn);
	seteditfilersrc(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,FALSE,FALSE);
	FINDRSCSETEDITABLE(mtnrsrc,"DEFINITION TYPES",FALSE);
	FINDRSCSETEDITABLE(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",FALSE);
}
void payjtrnm(short dowhich,void *targetkey)
{
	char *name=NULL;
	int which_type=0;
	short nofields=0,nokeys=0,x=0,y=0,edit_rsrc=FALSE;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part=NULL;
	mtnstruct *mtn=NULL;
	RDArsrc *mtnrsrc=NULL;
				
	mtnrsrc=RDArsrcNEW(MASTER->module,MASTER->maintain_screen);
	ZERNRD(MASTER->passkey->fileno);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->DEFtypes=NULL;
	mtn->PDEFtypes=NULL;
	mtn->deftype=0;
	mtn->pdeftype=0;
	mtn->DEFtypes=APPlibNEW();
	makeDEFtypes(mtnrsrc,mtn);
	mtn->PDEFtypes=APPlibNEW();
	makePDEFtypes(mtnrsrc,mtn);
	nofields=NUMFLDS(MASTER->passkey->fileno);
	fields=FLDPOINTER(MASTER->passkey->fileno);
	nokeys=NUMKEYS(MASTER->passkey->fileno);
	keys=KEYPOINTER(MASTER->passkey->fileno);
/* making all widgets not editable for the user to keep the user from changing
these values mid-stream in the Process Functions.  The user should not have
to change these values because they are created in the Process functions and
they can be removed by the unprocess function that relates to the one that
created it.  Also might want to make perge output definitions to delete these
records after the records are no longer needed.
			edit_rsrc=(short)editable;
*/
	GET_MAINTAIN_INCVARS(mtnrsrc,MASTER);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=TRUE;
			if(!RDAstrcmp(fldx->name,"DEFINITION TYPE"))
			{
				if(FIELDscrn(MASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					which_type=0;
					addlstrsrc(mtnrsrc,"DEFINITION TYPES",
						&which_type,edit_rsrc,
						NULL,mtn->DEFtypes->numlibs,
						&mtn->DEFtypes->libs,NULL);
					FINDRSCSETEDITABLE(mtnrsrc,"DEFINITION TYPES",FALSE);
				}
			} else if(!RDAstrcmp(fldx->name,"PAYROLL FUND DEFINITION TYPE"))
			{
				if(FIELDscrn(MASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					which_type=0;
					addlstrsrc(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",
						&which_type,edit_rsrc,
						NULL,mtn->PDEFtypes->numlibs,
						&mtn->PDEFtypes->libs,NULL);
					FINDRSCSETEDITABLE(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",FALSE);
				}
			} else {
				nonlstmakefld(mtnrsrc,MASTER->passkey->fileno,fldx,edit_rsrc);
				if(name!=NULL)
				{
					name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(FILENAME(MASTER->passkey->fileno)));
				} else {
					name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(FILENAME(MASTER->passkey->fileno)));
				}
				sprintf(name,"[%s][%s]",FILENAME(MASTER->passkey->fileno),fldx->name);
				FINDRSCSETEDITABLE(mtnrsrc,name,FALSE);
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
							name=Rrealloc(name,RDAstrlen(part->name)+5+RDAstrlen(FILENAME(MASTER->passkey->fileno)));
						} else {
							name=Rmalloc(RDAstrlen(part->name)+5+RDAstrlen(FILENAME(MASTER->passkey->fileno)));
						}
						sprintf(name,"[%s][%s]",FILENAME(MASTER->passkey->fileno),part->name);
						FINDRSCSETEDITABLE(mtnrsrc,name,TRUE);
						FINDRSCSETFUNC(mtnrsrc,name,SETID,mtn);
					}
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
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
					setlists(mtnrsrc,mtn,FALSE);
					check_accts(mtnrsrc,mtn);
				}
			} else {
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				setlists(mtnrsrc,mtn,FALSE);
				check_accts(mtnrsrc,mtn);
			}
			break;
		case 2:
			ApplyPassKey(MASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
			if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
			} else {
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				setlists(mtnrsrc,mtn,FALSE);
				check_accts(mtnrsrc,mtn);
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
				} else {
					setlists(mtnrsrc,mtn,FALSE);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
					check_accts(mtnrsrc,mtn);
				}
			} else {
				setlists(mtnrsrc,mtn,FALSE);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				check_accts(mtnrsrc,mtn);
			}
			break;
	}
	GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_payjtrn,mtn);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,mtn);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	ADVMakePrevNextButtons(mtnrsrc,keys,nokeys,getpreviouscb,mtn,getnextcb,mtn);
	MakeMasterButtons(mtnrsrc,MASTER);
	computeallSCRNvirtuals(mtnrsrc);
	if(mbl_payjtrn!=NULL) FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_payjtrn==NULL?TRUE:FALSE));
}
static void resetdefs(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	updateallrsrc(mtnrsrc);
	SETID(mtnrsrc,mtn);
}
static void setDEFtypes(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	FINDRSCSETACCTTYPE(mtnrsrc,"[PAYJTRN][ACCOUNT CODE]",0);
	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYJTRN][ACCOUNT CODE]",mtn->deftype);
	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYJTRN][DEBIT CODE]",mtn->deftype);
	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYJTRN][CREDIT CODE]",mtn->deftype);
	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYJTRN][PAYABLE CODE]",mtn->pdeftype);
	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYJTRN][CASH CODE]",mtn->pdeftype);
}
static short check_code(RDArsrc *mtnrsrc,mtnstruct *mtn,short acctwidget,int procyrid)
{
	char *acctcode=NULL,*debcode=NULL,*credcode=NULL;
	char *paycode=NULL,*cashcode=NULL,*tmp=NULL;

	FINDFLDGETSTRING(MASTER->passkey->fileno,"ACCOUNT CODE",&acctcode);
	FINDFLDGETSTRING(MASTER->passkey->fileno,"DEBIT CODE",&debcode);
	FINDFLDGETSTRING(MASTER->passkey->fileno,"CREDIT CODE",&credcode);
	FINDFLDGETSTRING(MASTER->passkey->fileno,"PAYABLE CODE",&paycode);
	FINDFLDGETSTRING(MASTER->passkey->fileno,"CASH CODE",&cashcode);
	FINDRSCGETSTRING(mtnrsrc,"[PAYJTRN][CASH CODE]",&cashcode);
	switch(acctwidget)
	{
		case 0: /* check account code */
			if((tmp=ADVCHECKACCT(eyrnum,eaccnum,acctcode,0,mtn->deftype,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID EXPENDITURE ACCOUNT CODE!",tmp,NULL,NULL,TRUE,NULL);
				Rfree(tmp);
				if(acctcode!=NULL) Rfree(acctcode); 
				if(credcode!=NULL) Rfree(credcode);
				if(debcode!=NULL) Rfree(debcode);
				if(paycode!=NULL) Rfree(paycode);
				if(cashcode!=NULL) Rfree(cashcode);
				return(FALSE);
			}
			break;
		case 1: /* check debit code */
			if((tmp=ADVCHECKACCT(byrnum,baccnum,debcode,0,mtn->deftype,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!",tmp,NULL,NULL,TRUE,NULL);
				Rfree(tmp);
				if(acctcode!=NULL) Rfree(acctcode);
				if(credcode!=NULL) Rfree(credcode);
				if(debcode!=NULL) Rfree(debcode); 
				if(paycode!=NULL) Rfree(paycode);
				if(cashcode!=NULL) Rfree(cashcode);
				return(FALSE);
			} else {
				if((tmp=ADVGETBALREF(baccnum,debcode,mtn->deftype,SCRNvirtualSubData,mtnrsrc))!=NULL)
				{
					if(RDAstrcmp(tmp,"EXPENDITURE"))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE!","The DEBIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of EXPENDITURE.",NULL,NULL,TRUE,NULL);
						Rfree(tmp);
						if(acctcode!=NULL) Rfree(acctcode);
						if(credcode!=NULL) Rfree(credcode);
						if(debcode!=NULL) Rfree(debcode);
						if(paycode!=NULL) Rfree(paycode);
						if(cashcode!=NULL) Rfree(cashcode);
						return(FALSE);
					}
					Rfree(tmp);
				}
			}
			break;
		case 2: /* check credit code */
			if((tmp=ADVCHECKACCT(byrnum,baccnum,credcode,0,mtn->deftype,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE!",tmp,NULL,NULL,TRUE,NULL);
				if(acctcode!=NULL) Rfree(acctcode);
				if(credcode!=NULL) Rfree(credcode);
				if(debcode!=NULL) Rfree(debcode);
				if(paycode!=NULL) Rfree(paycode);
				if(cashcode!=NULL) Rfree(cashcode);
				Rfree(tmp);
				return(FALSE);
			} else {
				if((tmp=ADVGETBALREF(baccnum,credcode,mtn->deftype,SCRNvirtualSubData,mtnrsrc))!=NULL)
				{
					if(RDAstrcmp(tmp,"CASH") && RDAstrcmp(tmp,"DUE TO"))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE!","The CREDIT CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of CASH or DUE TO.",NULL,NULL,TRUE,NULL);
						Rfree(tmp);
						if(acctcode!=NULL) Rfree(acctcode);
						if(credcode!=NULL) Rfree(credcode);
						if(debcode!=NULL) Rfree(debcode);
						if(paycode!=NULL) Rfree(paycode);
						if(cashcode!=NULL) Rfree(cashcode);
						return(FALSE);
					}
					Rfree(tmp);
				}
			}
			break;
		case 3: /* check payable code */
			if((tmp=ADVCHECKACCT(byrnum,baccnum,paycode,0,mtn->pdeftype,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PAYABLE CODE!",tmp,NULL,NULL,TRUE,NULL);
				if(acctcode!=NULL) Rfree(acctcode);
				if(credcode!=NULL) Rfree(credcode);
				if(debcode!=NULL) Rfree(debcode);
				if(paycode!=NULL) Rfree(paycode);
				if(cashcode!=NULL) Rfree(cashcode);
				Rfree(tmp);
				return(FALSE);
			} else {
				if((tmp=ADVGETBALREF(baccnum,paycode,mtn->pdeftype,SCRNvirtualSubData,mtnrsrc))!=NULL)
				{
					if(RDAstrcmp(tmp,"SALARIES / BENEFITS PAYABLE"))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PAYABLE CODE!","The PAYABLE CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of SALARIES/BENEFITS PAYABLE.",NULL,NULL,TRUE,NULL);
						Rfree(tmp);
						if(acctcode!=NULL) Rfree(acctcode);
						if(credcode!=NULL) Rfree(credcode);
						if(debcode!=NULL) Rfree(debcode);
						if(paycode!=NULL) Rfree(paycode);
						if(cashcode!=NULL) Rfree(cashcode);
						return(FALSE);
					}
					Rfree(tmp);
				}
			}
			break;
		case 4:
			if((tmp=ADVCHECKACCT(byrnum,baccnum,cashcode,0,mtn->pdeftype,procyrid,SCRNvirtualSubData,mtnrsrc))!=NULL)
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CASH CODE!",tmp,NULL,NULL,TRUE,NULL);
				if(acctcode!=NULL) Rfree(acctcode);
				if(credcode!=NULL) Rfree(credcode);
				if(debcode!=NULL) Rfree(debcode);
				if(paycode!=NULL) Rfree(paycode);
				if(cashcode!=NULL) Rfree(cashcode);
				Rfree(tmp);
				return(FALSE);
			} else {
				if((tmp=ADVGETBALREF(baccnum,cashcode,mtn->pdeftype,SCRNvirtualSubData,mtnrsrc))!=NULL)
				{
					if(RDAstrcmp(tmp,"CASH"))
					{
						WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CASH CODE!","The CASH CODE entered does not have the correct BALANCE SHEET REFERENCE TYPE of CASH.",NULL,NULL,TRUE,NULL);
						Rfree(tmp);
						if(acctcode!=NULL) Rfree(acctcode);
						if(credcode!=NULL) Rfree(credcode);
						if(debcode!=NULL) Rfree(debcode);
						if(paycode!=NULL) Rfree(paycode);
						if(cashcode!=NULL) Rfree(cashcode);
						return(FALSE);
					}
					Rfree(tmp);
				}
			}
			break;
		default:
			prterr("\"acctwidget\" value of [%d] out of range.",acctwidget);
			break;
	}
	if(acctcode!=NULL) Rfree(acctcode);
	if(credcode!=NULL) Rfree(credcode);
	if(debcode!=NULL) Rfree(debcode);
	if(paycode!=NULL) Rfree(paycode);
	if(cashcode!=NULL) Rfree(cashcode);
	return(TRUE);
}
static void check_accts(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	short x;
	int procyrid=0;

	COPYFIELD(MASTER->passkey->fileno,paynum,"PAY IDENTIFICATION");
	if(ADVEQLNRDsec(paynum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(paynum,1);
	FINDFLDGETINT(paynum,"FISCAL YEAR",&procyrid);
	for(x=0;x<5;++x) check_code(mtnrsrc,mtn,x,procyrid);
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->PDEFtypes!=NULL) freeapplib(mtn->PDEFtypes);
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
static void NEfunction(RDArsrc *mtnrsrc,MaintainMaster *MASTER,void (*SubFunc)(...), void *args,mtnstruct *mtn)
{
	FINDRSCSETINT(mtnrsrc,"DEFINITION TYPES",0);
	FINDRSCSETINT(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",0);
	updatersrc(mtnrsrc,"DEFINITION TYPES");
	updatersrc(mtnrsrc,"PAYROLL FUND DEFINITION TYPES");
}
static void BUEQLfunction(RDArsrc *mtnrsrc,MaintainMaster *MASTER,void (*SubFunc)(...),
	void *args,mtnstruct *mtn)
{
	setlists(mtnrsrc,mtn,TRUE);
}
static void setlists(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	int sel=0,psel=0;

	FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",&mtn->deftype);
	FINDFLDGETSHORT(MASTER->passkey->fileno,"PAYROLL FUND DEFINITION TYPE",&mtn->pdeftype);
	
	if(mtn->deftype>(-1) && mtn->deftype<mtn->DEFtypes->numlibs) sel=mtn->deftype;
	if(mtn->pdeftype>(-1) && mtn->pdeftype<mtn->PDEFtypes->numlibs) psel=mtn->pdeftype;
	FINDRSCSETINT(mtnrsrc,"DEFINITION TYPES",sel);
	FINDRSCSETINT(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",psel);
	if(update)
	{
		updatersrc(mtnrsrc,"DEFINITION TYPES");
		updatersrc(mtnrsrc,"PAYROLL FUND DEFINITION TYPES");
	}
	setDEFtypes(mtnrsrc,mtn);
}
static void AUEQLfunction(RDArsrc *mtnrsrc,MaintainMaster *MASTER,void (*SubFunc)(...),
	void *args,mtnstruct *mtn)
{
	updatefilerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
	check_accts(mtnrsrc,mtn);
}
static void makeDEFtypes(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int x=0;
	RDAacct *acct=NULL;

	if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
	mtn->DEFtypes=APPlibNEW();
	if(EXPENDITURE_ACCOUNT->codetype!=NULL)
	{
		for(x=0,acct=EXPENDITURE_ACCOUNT->codetype;
			x<EXPENDITURE_ACCOUNT->num;++x,++acct)
		{
			addAPPlib(mtn->DEFtypes,acct->name);
		}
	}
}
static void makePDEFtypes(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int x=0;
	RDAacct *acct=NULL;

	if(mtn->PDEFtypes!=NULL) freeapplib(mtn->PDEFtypes);
	mtn->PDEFtypes=APPlibNEW();
	if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
	{
		for(x=0,acct=BALANCE_SHEET_ACCOUNT->codetype;
			x<BALANCE_SHEET_ACCOUNT->num;++x,++acct)
		{
			addAPPlib(mtn->PDEFtypes,acct->name);
		}
	}
}
