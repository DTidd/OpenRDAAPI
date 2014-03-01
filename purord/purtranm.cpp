/* purtranm.c - Xpert Purchase Order Tranaction Master Maintain Screen */
#include <cstdio>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <fin.hpp>
#include <venpmt.hpp>
#include <purord.hpp>

static void save_check(RDArsrc *,RDATData *,short);
static void save_record(RDArsrc *,RDATData *,short);
static void save_record_quit(RDArsrc *,RDATData *);
static void save_record_up(RDArsrc *,RDATData *);
static void save_record_nup(RDArsrc *,RDATData *);
static void save_record_now(RDArsrc *,RDATData *);
static void save_record_now1(RDArsrc *,RDATData *);
static void quit_record(RDArsrc *,RDATData *);
static void quit_record_test(RDArsrc *,RDATData *);
static void readscreen(RDArsrc *,RDATData *);
static void resetdefs(RDArsrc *,RDATData *);
static void getpreviouscb(RDArmem *,RDATData *);
static void getnextcb(RDArmem *,RDATData *);
static void SETID(RDArsrc *,RDATData *);
static void SETFILES(RDArsrc *,MaintainMaster *);
static void seteditable(RDArsrc *,RDATData *,short);
static void seteditablecb(RDArsrc *,MaintainMaster *,void (*)(...),void *,RDATData *);
static void SetAppDef(RDArsrc *,RDATData *,short);
static void SetAppDefcb(RDArsrc *,MaintainMaster *,void (*)(...),void *,RDATData *);
static void setACCTtype(RDArsrc *,RDATData *);
static void setACCTtypecb(RDArsrc *,MaintainMaster *,void (*)(...),void *,RDATData *);
extern short purmst,purlin,puracc;
extern short finbyr,fineyr,finryr;
extern char editable;
extern void browse_purtran(RDArsrc *,RDATData *);
extern RDApurord *purord;
extern RDAvenpmt *venpmt;
extern MaintainMaster *MASTER;
extern MakeBrowseList *mbl_purtran;
void purtranm(short,void *);

static void caltotal(RDArsrc *mtnrsrc)
{
	double cost=0.0,sales_tax=0.0,total=0.0;
	double prorateamt=0.0,adminfee=0.0;

	readwidget(mtnrsrc,"[PURTRN][COST]");
	readwidget(mtnrsrc,"[PURTRN][TAX AMOUNT]");
	readwidget(mtnrsrc,"[PURTRN][PRORATE AMOUNT]");
	readwidget(mtnrsrc,"[PURTRN][ADMIN FEE]");
	FINDRSCGETDOUBLE(mtnrsrc,"[PURTRN][COST]",&cost);
	FINDRSCGETDOUBLE(mtnrsrc,"[PURTRN][TAX AMOUNT]",&sales_tax);
	FINDRSCGETDOUBLE(mtnrsrc,"[PURTRN][PRORATE AMOUNT]",&prorateamt);
	FINDRSCGETDOUBLE(mtnrsrc,"[PURTRN][ADMIN FEE]",&adminfee);
	total=cost+sales_tax+prorateamt+adminfee;
	FINDRSCSETDOUBLE(mtnrsrc,"[PURTRN][AMOUNT]",total);
	updatersrc(mtnrsrc,"[PURTRN][AMOUNT]");
}
void purtranm(short dowhich,void *targetkey)
{
	char *name=NULL;
	short edit_rsrc=TRUE,nofields=0,nokeys=0,x=0,y=0;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDArsrc *mtnrsrc=NULL;
	RDATData *previous=NULL;

	mtnrsrc=RDArsrcNEW(MASTER->module,MASTER->maintain_screen);
	ZERNRD(MASTER->passkey->fileno);
	previous=RDATDataNEW(MASTER->passkey->fileno);
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
					FINDRSCSETFUNC(mtnrsrc,name,SETID,previous);
					FINDRSCSETEDITABLE(mtnrsrc,name,TRUE);
				}
			}
		}
	}
	if(name!=NULL) Rfree(name);
	SET_MAINTAIN_FILE2RSRCS(mtnrsrc,MASTER);
	SCRNvirtual2rsrc(mtnrsrc);
	if(venpmt->Use_Taxes==TRUE)
	{
		FINDRSCSETFUNC(mtnrsrc,"[PURTRN][COST]",caltotal,NULL);
		FINDRSCSETFUNC(mtnrsrc,"[PURTRN][TAX AMOUNT]",caltotal,NULL);
		FINDRSCSETEDITABLE(mtnrsrc,"[PURTRN][AMOUNT]",FALSE);
	}
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
					SetAppDef(mtnrsrc,previous,FALSE);
				} else {
					GetRDATData(MASTER->passkey->fileno,previous);
					setACCTtype(mtnrsrc,previous);
					seteditable(mtnrsrc,previous,FALSE);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,previous);
				setACCTtype(mtnrsrc,previous);
				seteditable(mtnrsrc,previous,FALSE);
			}
			break;
		case 2:
			ApplyPassKey(MASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
			if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
				SetAppDef(mtnrsrc,previous,FALSE);
			} else {
				GetRDATData(MASTER->passkey->fileno,previous);
				setACCTtype(mtnrsrc,previous);
				seteditable(mtnrsrc,previous,FALSE);
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
					SetAppDef(mtnrsrc,previous,FALSE);
				} else {
					GetRDATData(MASTER->passkey->fileno,previous);
					setACCTtype(mtnrsrc,previous);
					seteditable(mtnrsrc,previous,FALSE);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,previous);
				setACCTtype(mtnrsrc,previous);
				seteditable(mtnrsrc,previous,FALSE);
			}
			break;
	}
	GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,previous);
	if(mbl_purtran!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,previous);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_purtran,previous);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,previous);
	addbtnrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,previous);
	MakeMasterButtons(mtnrsrc,MASTER);
	computeallSCRNvirtuals(mtnrsrc);
	if(mbl_purtran!=NULL)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	}
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,
		(mbl_purtran==NULL?TRUE:FALSE));
}
static void resetdefs(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(MASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	updateallrsrc(mtnrsrc);
	SETID(mtnrsrc,previous);
}
static void getpreviouscb(RDArmem *member,RDATData *previous)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1))
	{
		keyno=MASTER->passkey->keyno;
	}
	ADV2GET_PREVIOUS(member->parent,MASTER,previous,SCRNvirtualSubData,
		member->parent,keyno,SetAppDefcb,previous,
		setACCTtypecb,previous,seteditablecb,previous);
}
static void getnextcb(RDArmem *member,RDATData *previous)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1))
	{
		keyno=MASTER->passkey->keyno;
	}
	ADV2GET_NEXT(member->parent,MASTER,previous,SCRNvirtualSubData,
		member->parent,keyno,SetAppDefcb,previous,
		setACCTtypecb,previous,seteditablecb,previous);
}
static void SETID(RDArsrc *mtnrsrc,RDATData *previous)
{
	ADV4SET_KEYFUNC(mtnrsrc,MASTER,previous,SCRNvirtualSubData,
		(void *)mtnrsrc,SetAppDefcb,previous,
		setACCTtypecb,previous,seteditablecb,previous);
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *MASTER)
{
	SET_MASTER(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
}
static void readscreen(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(MASTER->passkey->fileno);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
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
				NULL,FALSE,2,mtnrsrc,previous,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	save_record(mtnrsrc,previous,update);
}
static void save_record_up(RDArsrc *mtnrsrc,RDATData *previous)
{
	readscreen(mtnrsrc,previous);
	save_check(mtnrsrc,previous,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,RDATData *previous)
{
	readscreen(mtnrsrc,previous);
	save_check(mtnrsrc,previous,FALSE);
}
static void save_record(RDArsrc *mtnrsrc,RDATData *previous,short update)
{
	short numflds=0,y=0,p_month=0;
	char *rcddesc=NULL;
	short deftype=0,accttype=0;
	char delflag=FALSE,dist=FALSE;
	char *poid1=NULL;
	int yearid1=0,lineno1=0;
	double amount=0.0,cost=0.0,salestax=0.0,adminfee=0.0,prorateamt=0.0;
	NRDfield *tmpfld=NULL,*fldx=NULL,*tmp=NULL;

	if(ADVRECORDsec(MASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(MASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
			prterr("Security Access Deined [%s].  Didn't write record.",rcddesc);
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
				FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&delflag);
				if(delflag==FALSE)
				{
					FINDFLDGETSTRING(MASTER->passkey->fileno,"PO IDENTIFICATION",&poid1);
					FINDFLDGETINT(MASTER->passkey->fileno,"FISCAL YEAR",&yearid1);
					FINDFLDGETINT(MASTER->passkey->fileno,"LINE NUMBER",&lineno1);
					FINDFLDGETSHORT(puracc,"ACCOUNT TYPE",&accttype);
					FINDFLDGETDOUBLE(MASTER->passkey->fileno,"AMOUNT",&amount);
					if(venpmt->Use_Taxes==TRUE)
					{
						FINDFLDGETDOUBLE(MASTER->passkey->fileno,"COST",&cost);
						FINDFLDGETDOUBLE(MASTER->passkey->fileno,"TAX AMOUNT",&salestax);
						FINDFLDGETDOUBLE(MASTER->passkey->fileno,"PRORATE AMOUNT",&prorateamt);
						FINDFLDGETDOUBLE(MASTER->passkey->fileno,"ADMIN FEE",&adminfee);
					}
					ADV4updateoutstanding(purmst,purlin,yearid1,poid1,lineno1,-cost,-salestax,-adminfee,-prorateamt,-amount,TRUE,1,SCRNvirtualSubData,mtnrsrc,venpmt);
					FINDFLDGETCHAR(MASTER->passkey->fileno,"DISTRIBUTED",&dist);
					if(dist==FALSE)
					{
						FINDFLDGETSHORT(puracc,"DEFINITION TYPE",&deftype);
						FINDFLDGETSHORT(MASTER->passkey->fileno,"PROCESSING MONTH",&p_month);
						switch(accttype)
						{
							case EXP_ACCT:
							  if(purord->use_fiscal_months==TRUE)
							  {
								ADVupdateundist(puracc,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,fineyr,finbyr,yearid1,deftype,"ENCUMBRANCES",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
							  } else {
								ADVupdateundist(puracc,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,fineyr,finbyr,yearid1,deftype,"ENCUMBRANCES",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
							  }
								break;
							case REV_ACCT:
							  if(purord->use_fiscal_months==TRUE)
							  {
								ADVupdateundist(puracc,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,finryr,finbyr,yearid1,deftype,"ESTIMATED REVENUE",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
							  } else {
								ADVupdateundist(puracc,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,finryr,finbyr,yearid1,deftype,"ESTIMATED REVENUE",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
							  }
								break;
							case BAL_ACCT:
								if(amount<0.0)
								{
								if(purord->use_fiscal_months==TRUE)
								{
									ADVupdateundist(puracc,NULL,"ACCOUNT CODE","CREDIT CODE",NULL,(-1),finbyr,yearid1,deftype,"DEBITS",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
								} else {
									ADVupdateundist(puracc,NULL,"ACCOUNT CODE","CREDIT CODE",NULL,(-1),finbyr,yearid1,deftype,"DEBITS",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
								}
								} else {
								if(purord->use_fiscal_months==TRUE)
								{
									ADVupdateundist(puracc,NULL,"ACCOUNT CODE","CREDIT CODE",NULL,(-1),finbyr,yearid1,deftype,"CREDITS",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
								} else {
									ADVupdateundist(puracc,NULL,"ACCOUNT CODE","CREDIT CODE",NULL,(-1),finbyr,yearid1,deftype,"CREDITS",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,mtnrsrc);
								}
								}
								break;
							default:
								prterr("Invalid Account Type [%d]",accttype);
								break;
						} /* end switch/case */
					} /* end if(delflag==FALSE) */
				} /* end if(dist==FALSE) */
			} /* end if(!ADVEQLNRDsec) */
			UNLNRDFILE(MASTER->passkey->fileno); /* moved this line up, so others can use it.  Should loop in a while(){} until it can lock..... locks in WRT's */
			fields2file(MASTER->passkey->fileno,tmpfld);
			WRTTRANS(MASTER->passkey->fileno,0,NULL,previous);
			FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&delflag);
			if(delflag==FALSE)
			{
				FINDFLDGETSTRING(MASTER->passkey->fileno,"PO IDENTIFICATION",&poid1);
				FINDFLDGETINT(MASTER->passkey->fileno,"FISCAL YEAR",&yearid1);
				FINDFLDGETSHORT(puracc,"ACCOUNT TYPE",&accttype);
				FINDFLDGETDOUBLE(MASTER->passkey->fileno,"AMOUNT",&amount);
				if(venpmt->Use_Taxes==TRUE)
				{
					FINDFLDGETDOUBLE(MASTER->passkey->fileno,"COST",&cost);
					FINDFLDGETDOUBLE(MASTER->passkey->fileno,"TAX AMOUNT",&salestax);
					FINDFLDGETDOUBLE(MASTER->passkey->fileno,"PRORATE AMOUNT",&prorateamt);
					FINDFLDGETDOUBLE(MASTER->passkey->fileno,"ADMIN FEE",&adminfee);
				}
				ADV4updateoutstanding(purmst,purlin,yearid1,poid1,lineno1,cost,salestax,adminfee,prorateamt,amount,TRUE,1,SCRNvirtualSubData,mtnrsrc,venpmt);
				FINDFLDGETCHAR(MASTER->passkey->fileno,"DISTRIBUTED",&dist);
				if(dist==FALSE)
				{
					FINDFLDGETSHORT(puracc,"DEFINITION TYPE",&deftype);
					FINDFLDGETSHORT(MASTER->passkey->fileno,"PROCESSING MONTH",&p_month);
					switch(accttype)
					{
						case EXP_ACCT:
						 if(purord->use_fiscal_months==TRUE)
						 {
							ADVupdateundist(puracc,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,fineyr,finbyr,yearid1,deftype,"ENCUMBRANCES",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
						 } else {
							ADVupdateundist(puracc,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,fineyr,finbyr,yearid1,deftype,"ENCUMBRANCES",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,mtnrsrc);
						 }
							break;
						case REV_ACCT:
						 if(purord->use_fiscal_months==TRUE)
						 {
							ADVupdateundist(puracc,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,finryr,finbyr,yearid1,deftype,"ESTIMATED REVENUE",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
						 } else {
							ADVupdateundist(puracc,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,finryr,finbyr,yearid1,deftype,"ESTIMATED REVENUE",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,mtnrsrc);
						 }
							break;
					case BAL_ACCT:
							if(amount<0.0) 
							{
							if(purord->use_fiscal_months==TRUE) 
							{
								ADVupdateundist(puracc,NULL,"ACCOUNT CODE","CREDIT CODE",NULL,(-1),finbyr,yearid1,deftype,"CREDITS",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
							} else {
								ADVupdateundist(puracc,NULL,"ACCOUNT CODE","CREDIT CODE",NULL,(-1),finbyr,yearid1,deftype,"CREDITS",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
							}
							} else {
							if(purord->use_fiscal_months==TRUE) 
							{
								ADVupdateundist(puracc,NULL,"ACCOUNT CODE","CREDIT CODE",NULL,(-1),finbyr,yearid1,deftype,"DEBITS",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
							} else {
								ADVupdateundist(puracc,NULL,"ACCOUNT CODE","CREDIT CODE",NULL,(-1),finbyr,yearid1,deftype,"DEBITS",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,mtnrsrc);
							}
							}
							break;
						default:
							prterr("Invalid Account Type [%d]",accttype);
							break;
					} /* end switch */
				}
			}
			if(poid1!=NULL) Rfree(poid1);
			DELETE_SUBORDINATES(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
			updatebrowse(update,mbl_purtran,MASTER->passkey->fileno,MASTER->module,mtnrsrc);
		} /* end if(numflds>0) */
		if(mbl_purtran!=NULL)
		{
			quit_record(mtnrsrc,previous);
		} else {
			seteditable(mtnrsrc,previous,TRUE);
			GetRDATData(MASTER->passkey->fileno,previous);
		}
	}
}
static void quit_record(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(previous!=NULL) FreeRDATData(previous);
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
static void save_record_quit(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check(mtnrsrc,previous,TRUE);
}
static void quit_record_test(RDArsrc *mtnrsrc,RDATData *previous)
{
	QUIT_RECORD_TEST(mtnrsrc,MASTER->passkey->fileno,previous,readscreen,
		save_record_quit,quit_record,previous);
}
static void SetAppDef(RDArsrc *mtnrsrc,RDATData *previous,short update)
{
	char *date=NULL,*timex=NULL;

	FINDRSCSETSTRING(mtnrsrc,"[PURTRN][SOURCE USER]",USERLOGIN);
	date=GETCURRENTDATE10();
	timex=GETCURRENTTIME();
	FINDRSCSETSTRING(mtnrsrc,"[PURTRN][ENTRY DATE]",date);
	FINDRSCSETSTRING(mtnrsrc,"[PURTRN][ENTRY TIME]",timex);
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
	if(update)
	{
		updatersrc(mtnrsrc,"[PURTRN][SOURCE USER]");
		updatersrc(mtnrsrc,"[PURTRN][ENTRY DATE]");
		updatersrc(mtnrsrc,"[PURTRN][ENTRY TIME]");
	}

}
static void SetAppDefcb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
	void (*SubFunc)(...),void *args,RDATData *previous)
{
	SetAppDef(mtnrsrc,previous,TRUE);
}
static void setACCTtypecb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
	void (*SubFunc)(...),void *args,RDATData *previous)
{
	setACCTtype(mtnrsrc,previous);
}
static void seteditable(RDArsrc *mtnrsrc,RDATData *previous,short update)
{
	char closed=FALSE,suv=FALSE;

	filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
	FINDFLDGETCHAR(purmst,"CLOSED",&closed);
	FINDFLDGETCHAR(MASTER->passkey->fileno,"SOURCE USER VERIFIED",&suv);
	if(closed || suv)
	{
		if(update)
		{
			updateeditfilersrcsnokeys(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,FALSE);
		} else {
			seteditfilersrc(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,FALSE,FALSE);
		}
		SetRDAScrolledListEditable(mtnrsrc,MASTER->passkey->fileno,MASTER->passkey->keyno,FALSE,FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PURTRN][DELETEFLAG]",TRUE);
	} else {
		if(update)
		{
			updateeditfilersrcsnokeys(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,TRUE);
		} else {
			seteditfilersrc(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,TRUE,FALSE);
		}
		SetRDAScrolledListEditable(mtnrsrc,MASTER->passkey->fileno,MASTER->passkey->keyno,TRUE,TRUE);
	}
}
static void seteditablecb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
	void (*SubFunc)(...),void *args,RDATData *previous)
{
	seteditable(mtnrsrc,previous,TRUE);
}
static void setACCTtype(RDArsrc *mtnrsrc,RDATData *previous)
{
	short deftype=0,accttype=0;

	FINDFLDGETSHORT(puracc,"ACCOUNT TYPE",&accttype);
	FINDFLDGETSHORT(puracc,"DEFINITION TYPE",&deftype);
	FINDRSCSETACCTTYPE(mtnrsrc,"[PURACC][ACCOUNT CODE]",accttype);
	FINDRSCSETDEFTYPE(mtnrsrc,"[PURACC][ACCOUNT CODE]",deftype);
	FINDRSCSETDEFTYPE(mtnrsrc,"[PURACC][DEBIT CODE]",deftype);
	FINDRSCSETDEFTYPE(mtnrsrc,"[PURACC][CREDIT CODE]",deftype);
	FINDRSCSETDEFTYPE(mtnrsrc,"[PURACC][DEFAULT DEBIT CODE]",deftype);
	FINDRSCSETDEFTYPE(mtnrsrc,"[PURACC][DEFAULT CREDIT CODE]",deftype);
	FINDRSCSETDEFTYPE(mtnrsrc,"[PURACC][DEFAULT PAYABLE CODE]",deftype);
}
