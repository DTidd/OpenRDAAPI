/* veninvm.c - Catalogue Vendor Master Maintain Screen */
#include <stdio.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <mix.h>
#include <fin.h>
#include <venpmt.h>

struct mtnstructs
{
	APPlib *addresslist;
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
static void list2file(RDArsrc *,mtnstruct *);
static void readscreen(RDArsrc *,mtnstruct *);
static void resetdefs(RDArsrc *,mtnstruct *);
static void getpreviouscb(RDArmem *,mtnstruct *);
static void getnextcb(RDArmem *,mtnstruct *);
static void SETID(RDArsrc *,mtnstruct *);
static void SETFILES(RDArsrc *,MaintainMaster *);
static void setaddresses(RDArsrc *,mtnstruct *,short);
static void changeADDRlist(RDArsrc *,mtnstruct *);
static void setlists(RDArsrc *,mtnstruct *,short);
static void setlistscb(RDArsrc *,MaintainMaster *,void (*)(),void *,mtnstruct *);
static void set_appdefaults(RDArsrc *,mtnstruct *,short);
static void set_appdefaultscb(RDArsrc *,MaintainMaster *,void (*)(),void *,mtnstruct *);
void veninvm(short,void *targetkey);
extern char editable;
extern void browse_veninv(RDArsrc *,mtnstruct *);
extern MakeBrowseList *mbl_veninv;
extern MaintainMaster *MASTER;
extern RDAvenpmt *venpmt;
extern short venadr;

void veninvm(short dowhich,void *targetkey)
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
	mtn->addresslist=NULL;
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
			if(!RDAstrcmp(fldx->name,"ADDRESS IDENTIFICATION"))
			{
				if(FIELDscrn(MASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					if(mtn->addresslist!=NULL) freeapplib(mtn->addresslist);
					mtn->addresslist=APPlibNEW();
					addAPPlib(mtn->addresslist,"No Addresses Defined");
					addlstrsrc(mtnrsrc,
						"VENINV ADDRESS IDENTIFICATIONS",
						&which_type,
						(edit_rsrc?TRUE:FALSE),
						changeADDRlist,
						mtn->addresslist->numlibs,
						&mtn->addresslist->libs,mtn);
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
				} else {
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
					setlists(mtnrsrc,mtn,FALSE);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				setlists(mtnrsrc,mtn,FALSE);
			}
			break;
		case 2:
			ApplyPassKey(MASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
			if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
				set_appdefaults(mtnrsrc,mtn,FALSE);
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				setlists(mtnrsrc,mtn,FALSE);
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
					set_appdefaults(mtnrsrc,mtn,FALSE);
				} else {
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
					setlists(mtnrsrc,mtn,FALSE);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				setlists(mtnrsrc,mtn,FALSE);
			}
			break;
	}
	GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
        if(mbl_veninv!=NULL)
        {
                addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
        } else {
                addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
        }
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_veninv,mtn);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,mtn);
	addbtnrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,mtn);
	MakeMasterButtons(mtnrsrc,MASTER);
	computeallSCRNvirtuals(mtnrsrc);
	if(mbl_veninv!=NULL)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	}
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,
		(mbl_veninv==NULL?TRUE:FALSE));
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
		member->parent,keyno,set_appdefaultscb,
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
	ADV2GET_NEXT(member->parent,MASTER,mtn->previous,SCRNvirtualSubData,
		member->parent,keyno,set_appdefaultscb,mtn,
		setlistscb,mtn,NULL,NULL);
}
static void SETID(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ADV4SET_KEYFUNC(mtnrsrc,MASTER,mtn->previous,SCRNvirtualSubData,
		(void *)mtnrsrc,set_appdefaultscb,mtn,
		setlistscb,mtn,NULL,NULL);
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *MASTER)
{
	SET_MASTER(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
}
static void list2file(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected; 

	FINDRSCGETINT(mtnrsrc,"VENINV ADDRESS IDENTIFICATIONS",&selected);
	if(RDAstrcmp(mtn->addresslist->libs[selected],"No Addresses Defined"))
	{
		FINDFLDSETSTRING(MASTER->passkey->fileno,"ADDRESS IDENTIFICATION",mtn->addresslist->libs[selected]);
	} else {
		FINDFLDSETSTRING(MASTER->passkey->fileno,"ADDRESS IDENTIFICATION",NULL);
	}
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
static void save_check(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
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
				NULL,FALSE,2,mtnrsrc,mtn,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
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
	char *rcddesc=NULL,*vendorid1=NULL,*vendorid2=NULL;
	char *invid1=NULL,*invid2=NULL;
	char deleteflag=0,mdeleteflag=0;
	char dist=FALSE,cleared=FALSE,*debcode1=NULL,*credcode1=NULL;
	short ef_vch=0;
	short venvch=(-1),finbyr=(-1),fineyr=(-1),finryr=(-1);
	short accttype=0,deftype=0,p_month=0;
	int yearid1=0,yearid2=0,procyrid=0;
	double amount=0.0;
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
			FINDFLDGETSTRING(MASTER->passkey->fileno,"VENDOR IDENTIFICATION",&vendorid1);
			FINDFLDGETINT(MASTER->passkey->fileno,"CALENDAR YEAR",&yearid1);
			FINDFLDGETSTRING(MASTER->passkey->fileno,"INVOICE IDENTIFICATION",&invid1);
			venvch=OPNNRDsec("VENPMT","VENVCH",TRUE,TRUE,FALSE);
			fineyr=OPNNRDsec("FINMGT","FINEYR",TRUE,TRUE,FALSE);
			finryr=OPNNRDsec("FINMGT","FINRYR",TRUE,TRUE,FALSE);
			finbyr=OPNNRDsec("FINMGT","FINBYR",TRUE,TRUE,FALSE);
			if(venvch!=(-1) && fineyr!=(-1) && finryr!=(-1) && finbyr!=(-1))
			{
				ZERNRD(venvch);
				FINDFLDSETSTRING(venvch,"VENDOR IDENTIFICATION",vendorid1);
				FINDFLDSETINT(venvch,"CALENDAR YEAR",yearid1);
				FINDFLDSETSTRING(venvch,"INVOICE IDENTIFICATION",invid1);
				ef_vch=ADVGTENRDsec(venvch,1,SCRNvirtualSubData,mtnrsrc);
				while(!ef_vch)
				{
					FINDFLDGETCHAR(venvch,"DELETEFLAG",&mdeleteflag);
					if(!mdeleteflag)
					{
						FINDFLDGETSTRING(venvch,"VENDOR IDENTIFICATION",&vendorid2);
						FINDFLDGETINT(venvch,"CALENDAR YEAR",&yearid2);
						FINDFLDGETSTRING(venvch,"INVOICE IDENTIFICATION",&invid2);
						if(!RDAstrcmp(vendorid2,vendorid1) &&
							yearid2==yearid1 &&
							(!RDAstrcmp(invid2,invid1)))
						{
							prev=RDATDataNEW(venvch);
							FINDFLDGETCHAR(venvch,"DISTRIBUTED",&dist);
							FINDFLDGETCHAR(venvch,"CLEARED",&cleared);
							if(dist==FALSE || cleared==FALSE)
							{
								FINDFLDGETSHORT(venvch,"ACCOUNT TYPE",&accttype);
								FINDFLDGETSHORT(venvch,"DEFINITION TYPE",&deftype);
								FINDFLDGETINT(venvch,"FISCAL YEAR",&procyrid);
								FINDFLDGETSHORT(venvch,"PROCESSING MONTH",&p_month);
/*
								FINDFLDGETSHORT(venvch,"PROCESSING MONTH",&p_month);
*/
								FINDFLDGETSHORT(venvch,"FISCAL PROCESSING MONTH",&p_month);
								FINDFLDGETDOUBLE(venvch,"AMOUNT",&amount);
								FINDFLDGETSTRING(venvch,"DEBIT CODE",&debcode1);
								FINDFLDGETSTRING(venvch,"CREDIT CODE",&credcode1);
								switch(accttype)
								{
									case EXP_ACCT:
										if(credcode1!=NULL && RDAstrlen(credcode1)>0)
										{
										if(venpmt->use_fiscal_months==TRUE)
										{
											ADVupdateundist(venvch,"ACCOUNT CODE",(dist==FALSE?"DEBIT CODE":NULL),(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",fineyr,finbyr,procyrid,deftype,"EXPENDITURES",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
										} else {
											ADVupdateundist(venvch,"ACCOUNT CODE",(dist==FALSE?"DEBIT CODE":NULL),(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",fineyr,finbyr,procyrid,deftype,"EXPENDITURES",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
										}
										} else {
										if(venpmt->use_fiscal_months==TRUE)
										{
											ADVupdateundist(venvch,"ACCOUNT CODE",(dist==FALSE?"DEBIT CODE":NULL),NULL,"PAYABLE CODE",fineyr,finbyr,procyrid,deftype,"EXPENDITURES",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
										} else {
											ADVupdateundist(venvch,"ACCOUNT CODE",(dist==FALSE?"DEBIT CODE":NULL),NULL,"PAYABLE CODE",fineyr,finbyr,procyrid,deftype,"EXPENDITURES",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
										}
										}
										break;
									case REV_ACCT:
										if(debcode1!=NULL && RDAstrlen(debcode1)>0)
										{
										if(venpmt->use_fiscal_months==TRUE)
										{
											ADVupdateundist(venvch,"ACCOUNT CODE",NULL,NULL,NULL,finryr,finbyr,procyrid,deftype,"REVENUE",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
											ADVupdateundist(venvch,NULL,(dist==FALSE?"DEBIT CODE":NULL),(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",finryr,finbyr,procyrid,deftype,"REVENUE",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
										} else {
											ADVupdateundist(venvch,"ACCOUNT CODE",NULL,NULL,NULL,finryr,finbyr,procyrid,deftype,"REVENUE",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,mtnrsrc);
											ADVupdateundist(venvch,NULL,(dist==FALSE?"DEBIT CODE":NULL),(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",finryr,finbyr,procyrid,deftype,"REVENUE",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
										}
										} else {
										if(venpmt->use_fiscal_months==TRUE)
										{
											ADVupdateundist(venvch,"ACCOUNT CODE",NULL,NULL,NULL,finryr,finbyr,procyrid,deftype,"REVENUE",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
											ADVupdateundist(venvch,NULL,NULL,(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",finryr,finbyr,procyrid,deftype,"REVENUE",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
										} else {
											ADVupdateundist(venvch,"ACCOUNT CODE",NULL,NULL,NULL,finryr,finbyr,procyrid,deftype,"REVENUE",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,mtnrsrc);
											ADVupdateundist(venvch,NULL,NULL,(cleared==FALSE?"CREDIT CODE":NULL),"PAYABLE CODE",finryr,finbyr,procyrid,deftype,"REVENUE",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
										}
										}
										break;
									case BAL_ACCT:
										if(venpmt->use_fiscal_months==TRUE)
										{
											ADVupdateundist(venvch,NULL,(cleared==FALSE?"CREDIT CODE":NULL),(dist==FALSE?"ACCOUNT CODE":NULL),"PAYABLE CODE",(-1),finbyr,procyrid,deftype,"CREDITS",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
										} else {
											ADVupdateundist(venvch,NULL,(cleared==FALSE?"CREDIT CODE":NULL),(dist==FALSE?"ACCOUNT CODE":NULL),"PAYABLE CODE",(-1),finbyr,procyrid,deftype,"CREDITS",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,mtnrsrc);
										}
										break;
									default:
										prterr("Invalid Account Type [%d]",accttype);
										break;
								}
								if(debcode1!=NULL) Rfree(debcode1);
								if(credcode1!=NULL) Rfree(credcode1);
							}
							FINDFLDSETCHAR(venvch,"DELETEFLAG",deleteflag);
							ADVWRTTRANSsec(venvch,0,NULL,prev,SCRNvirtualSubData,mtnrsrc);
							if(prev!=NULL) FreeRDATData(prev);
						}
					}
					ef_vch=ADVNXTNRDsec(venvch,1,SCRNvirtualSubData,mtnrsrc);
					if(vendorid2!=NULL) Rfree(vendorid2);
					if(invid2!=NULL) Rfree(invid2);
				}
			}
			if(venvch!=(-1)) CLSNRD(venvch);
			if(fineyr!=(-1)) CLSNRD(fineyr);
			if(finryr!=(-1)) CLSNRD(finryr);
			if(finbyr!=(-1)) CLSNRD(finbyr);
		}
		if(vendorid1!=NULL) Rfree(vendorid1);
		if(vendorid2!=NULL) Rfree(vendorid2);
		if(invid1!=NULL) Rfree(invid1);
		if(invid2!=NULL) Rfree(invid2);
		DELETE_SUBORDINATES(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update,mbl_veninv,MASTER->passkey->fileno,MASTER->module,mtnrsrc);
		if(mbl_veninv!=NULL) quit_record(mtnrsrc,mtn);
		else GetRDATData(MASTER->passkey->fileno,mtn->previous);
	}
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
        	if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
        	if(mtn->addresslist!=NULL) freeapplib(mtn->addresslist);
		Rfree(mtn);
	}
        if(WindowCount<2)
        {
                CLOSE_MASTER_FILES(MASTER);
                if(MASTER!=NULL) FreeMaintainMaster(MASTER);
		if(venpmt!=NULL) free_venpmt(venpmt);
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
static void setaddresses(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	short ef;
	char *vendorid1=NULL,*vendorid2=NULL;
	char *addrid1=NULL,*addrid2=NULL;
	char delflag=FALSE;
	int x=0;

	if(mtn->addresslist!=NULL) freeapplib(mtn->addresslist);
	mtn->addresslist=APPlibNEW();
	FINDFLDGETSTRING(MASTER->passkey->fileno,"VENDOR IDENTIFICATION",&vendorid1);
	FINDFLDGETSTRING(MASTER->passkey->fileno,"ADDRESS IDENTIFICATION",&addrid1);
	ZERNRD(venadr);
	FINDFLDSETSTRING(venadr,"VENDOR IDENTIFICATION",vendorid1);
	ef=ADVGTENRDsec(venadr,1,SCRNvirtualSubData,mtnrsrc);
	while(!ef)
	{
		if(vendorid2!=NULL) Rfree(vendorid2);
		FINDFLDGETSTRING(venadr,"VENDOR IDENTIFICATION",&vendorid2);
		if(RDAstrcmp(vendorid2,vendorid1)) break;
		FINDFLDGETCHAR(venadr,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			if(addrid2!=NULL) Rfree(addrid2);
			FINDFLDGETSTRING(venadr,"ADDRESS IDENTIFICATION",&addrid2);
			addAPPlib(mtn->addresslist,addrid2);
		}
		ef=ADVNXTNRDsec(venadr,1,SCRNvirtualSubData,mtnrsrc);
	}
	if(mtn->addresslist->numlibs<1)
	{
		addAPPlib(mtn->addresslist,"No Addresses Defined");
	} else {
		for(x=0;x<mtn->addresslist->numlibs;x++)
		{
			if(!RDAstrcmp(addrid1,mtn->addresslist->libs[x]))
			break;
		}
		if(x>=mtn->addresslist->numlibs)
		{
			x=0;
		}
	}
	FINDFLDSETSTRING(venadr,"VENDOR IDENTIFICATION",vendorid1);
	FINDFLDSETSTRING(venadr,"ADDRESS IDENTIFICATION",mtn->addresslist->libs[x]);
	if(ADVEQLNRDsec(venadr,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(venadr,1);
	filerecord2rsrc(venadr,mtnrsrc);
	if(!FINDRSCLISTAPPlib(mtnrsrc,"VENINV ADDRESS IDENTIFICATIONS",x,mtn->addresslist))
	{
		updatersrc(mtnrsrc,"VENINV ADDRESS IDENTIFICATIONS");
	}
	if(addrid1!=NULL) Rfree(addrid1);
	if(addrid2!=NULL) Rfree(addrid2);
	if(vendorid1!=NULL) Rfree(vendorid1);
	if(vendorid2!=NULL) Rfree(vendorid2);
}
static void changeADDRlist(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int seladdr;
	char *vendorid1=NULL;

	if(FINDRSCGETINT(mtnrsrc,"VENINV ADDRESS IDENTIFICATIONS",&seladdr)) return;
	ZERNRD(venadr);
	if(!RDAstrcmp(mtn->addresslist->libs[seladdr],"No Addresses Defined"))
	{
		FINDFLDSETSTRING(venadr,"VENDOR IDENTIFICATION",vendorid1);
		FINDFLDSETSTRING(venadr,"ADDRESS IDENTIFICATION",NULL);
		FINDFLDSETSTRING(MASTER->passkey->fileno,"ADDRESS IDENTIFICATION",NULL);
	} else {
		FINDFLDSETSTRING(venadr,"VENDOR IDENTIFICATION",vendorid1);
		FINDFLDSETSTRING(venadr,"ADDRESS IDENTIFICATION",mtn->addresslist->libs[seladdr]);
		FINDFLDSETSTRING(MASTER->passkey->fileno,"ADDRESS IDENTIFICATION",mtn->addresslist->libs[seladdr]);
	}
	if(ADVEQLNRDsec(venadr,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(venadr,1);
	filerecord2rsrc(venadr,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
	if(vendorid1!=NULL) Rfree(vendorid1);
}
static void setlists(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	setaddresses(mtnrsrc,mtn,update);
}
static void setlistscb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,void (*SubFunc)(),void *args,mtnstruct *mtn)
{
	setlists(mtnrsrc,mtn,TRUE);
}
static void set_appdefaults(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *date=NULL,*timex=NULL;

	FINDRSCSETSTRING(mtnrsrc,"[VENINV][SOURCE USER]",USERLOGIN);
        date=GETCURRENTDATE();
        timex=GETCURRENTTIME();
	FINDRSCSETSTRING(mtnrsrc,"[VENINV][ENTRY DATE]",date);
	FINDRSCSETSTRING(mtnrsrc,"[VENINV][ENTRY TIME]",timex);
	if(date!=NULL) Rfree(date);
        if(timex!=NULL) Rfree(timex);
	setaddresses(mtnrsrc,mtn,update);
	if(update)
	{
		updatersrc(mtnrsrc,"[VENINV][SOURCE USER]");
		updatersrc(mtnrsrc,"[VENINV][ENTRY DATE]");
		updatersrc(mtnrsrc,"[VENINV][ENTRY TIME]");
	}
}
static void set_appdefaultscb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
	void (*SubFunc)(),void *args,mtnstruct *mtn)
{
	set_appdefaults(mtnrsrc,mtn,TRUE);
}
