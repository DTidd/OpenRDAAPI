/* purlinem.c - Purchase Order Line Maintain Master Program */
#include <cstdio>


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
static void SetAppDef(RDArsrc *,short);
static void SetAppDefcb(RDArsrc *,MaintainMaster *,void (*)(...),void *,RDATData *);
static void seteditable(RDArsrc *,RDATData *,short);
static void totalcalc(RDArsrc *);
static void seteditablecb(RDArsrc *,MaintainMaster *,void (*)(...),void *,RDATData *);
static void caltotal(RDArsrc *);
void purlinm(short,void *targetkey);
extern RDApurord *purord;
extern RDAvenpmt *venpmt;
extern short purmst;
extern MakeBrowseList *mbl_purlin;
extern char editable;
extern void browse_purlin(RDArsrc *,RDATData *);
extern MaintainMaster *MASTER;

#ifdef XXXX
static void set_costifnecessary(RDArsrc *mtnrsrc)
{
	int quantity=0;
	double cc=0.0,tt=0.0,unitrate=0.0,total=0.0;

	if(venpmt->Use_Taxes==TRUE)
	{
		FINDRSCGETINT(mtnrsrc,"[PURLIN][QUANTITY]",&quantity);
		FINDRSCGETDOUBLE(mtnrsrc,"[PURLIN][UNIT RATE]",&unitrate);
		FINDRSCGETDOUBLE(mtnrsrc,"[PURLIN][TOTAL]",&tt);
		total=(unitrate*quantity)*100.0;
		FINDRSCGETDOUBLE(mtnrsrc,"[PURLIN][COST]",&cc);
		if(cc!=tt && cc==0) cc=tt;
		FINDRSCSETDOUBLE(mtnrsrc,"[PURLIN][COST]",total);
		updatersrc(mtnrsrc,"[PURLIN][COST]");
		totalcalc(mtnrsrc);
	}
}
#endif /* XXXX */
void purlinm(short dowhich,void *targetkey)
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
        FINDRSCSETFUNC(mtnrsrc,"[PURLIN][QUANTITY]",caltotal,NULL);
        FINDRSCSETFUNC(mtnrsrc,"[PURLIN][UNIT RATE]",caltotal,NULL);
	if(venpmt->Use_Taxes)
	{
		FINDRSCSETFUNC(mtnrsrc,"[PURLIN][PERCENTAGE DISCOUNT]",caltotal,NULL);
		FINDRSCSETFUNC(mtnrsrc,"[PURLIN][TAX AMOUNT]",totalcalc,NULL);
		FINDRSCSETFUNC(mtnrsrc,"[PURLIN][PRORATE AMOUNT]",totalcalc,NULL);
		FINDRSCSETFUNC(mtnrsrc,"[PURLIN][ADMIN FEE]",totalcalc,NULL);
		FINDRSCSETFUNC(mtnrsrc,"[PURLIN][COST]",totalcalc,NULL);
		FINDRSCSETEDITABLE(mtnrsrc,"[PURLIN][TOTAL]",FALSE);
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
					SetAppDef(mtnrsrc,FALSE);
				} else {
					GetRDATData(MASTER->passkey->fileno,previous);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
					seteditable(mtnrsrc,previous,FALSE);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				seteditable(mtnrsrc,previous,FALSE);
			}
			break;
		case 2:
			ApplyPassKey(MASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
			if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
				SetAppDef(mtnrsrc,FALSE);
			} else {
				GetRDATData(MASTER->passkey->fileno,previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				seteditable(mtnrsrc,previous,FALSE);
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
					SetAppDef(mtnrsrc,FALSE);
				} else {
					GetRDATData(MASTER->passkey->fileno,previous);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
					seteditable(mtnrsrc,previous,FALSE);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				seteditable(mtnrsrc,previous,FALSE);
			}
			break;
	}
	GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,previous);
	if(mbl_purlin!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,previous);
	}
#ifdef XXXX
	set_costifnecessary(mtnrsrc);
#endif /* XXXX */
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_purlin,previous);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,previous);
	addbtnrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,previous);
	MakeMasterButtons(mtnrsrc,MASTER);
	computeallSCRNvirtuals(mtnrsrc);
	if(mbl_purlin!=NULL)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	}
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,
		(mbl_purlin==NULL?TRUE:FALSE));
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
		NULL,NULL,seteditablecb,previous);
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
		NULL,NULL,seteditablecb,previous);
}
static void SETID(RDArsrc *mtnrsrc,RDATData *previous)
{
	ADV4SET_KEYFUNC(mtnrsrc,MASTER,previous,SCRNvirtualSubData,
		(void *)mtnrsrc,SetAppDefcb,previous,
		NULL,NULL,seteditablecb,previous);
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
	char *rcddesc=NULL;
	int yearid1=(-1),yearid2=(-1),yearid3=(-1);
	char *poid1=NULL,*poid2=NULL,*poid3=NULL;
	int lineno1=(-1),lineno2=(-1),lineno3=(-1);
	int acctno1=(-1),acctno2=(-1);
	char deleteflag=0,mdeleteflag=0,pdeleteflag=0;
	short acctnum=0,transnum=0,eyrnum=0,ryrnum=0,byrnum=0;
	short ef_acct=0,ef_trans=0,accttype=0,deftype=0,p_month=0;
	char dist=FALSE;
	double amount=0.0,cost=0.0,salestax=0.0,adminfee=0.0,prorateamt=0.0;
	RDArsrc *tmprsrc=NULL;
	RDATData *prev=NULL;

	if(ADVRECORDsec(MASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(MASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("SECURITY ACCESS DENIED!",rcddesc,NULL,FALSE); 
			prterr("SECURITY ACCESS DENIED!,[%s].  Didn't write record.",rcddesc);
			Rfree(rcddesc);
		} else {
			prterr("Error didn't write record.");
		}
	} else {
		WRTTRANS(MASTER->passkey->fileno,0,NULL,previous);
		FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			FINDFLDGETINT(MASTER->passkey->fileno,"FISCAL YEAR",&yearid1);
			FINDFLDGETSTRING(MASTER->passkey->fileno,"PO IDENTIFICATION",&poid1);
			FINDFLDGETINT(MASTER->passkey->fileno,"LINE NUMBER",&lineno1);
			acctnum=OPNNRDsec("PURORD","PURACC",TRUE,TRUE,FALSE);
			transnum=OPNNRDsec("PURORD","PURTRN",TRUE,TRUE,FALSE);
			eyrnum=OPNNRDsec("FINMGT","FINEYR",TRUE,TRUE,FALSE);
			ryrnum=OPNNRDsec("FINMGT","FINRYR",TRUE,TRUE,FALSE);
			byrnum=OPNNRDsec("FINMGT","FINBYR",TRUE,TRUE,FALSE);
			tmprsrc=RDArsrcNEW(MASTER->module,NULL);
			copyDFincvir(mtnrsrc,tmprsrc);
			addDFincvir(tmprsrc,"PURORD","PURACC",NULL,acctnum);
			addDFincvir(tmprsrc,"PURORD","PURTRN",NULL,transnum);
			addDFincvir(tmprsrc,"FINMGT","FINEYR",NULL,eyrnum);
			addDFincvir(tmprsrc,"FINMGT","FINRYR",NULL,ryrnum);
			addDFincvir(tmprsrc,"FINMGT","FINBYR",NULL,byrnum);
			if(acctnum!=(-1))
			{
				ZERNRD(acctnum);
				FINDFLDSETINT(acctnum,"FISCAL YEAR",yearid1);
				FINDFLDSETSTRING(acctnum,"PO IDENTIFICATION",poid1);
				FINDFLDSETINT(acctnum,"LINE NUMBER",lineno1);
				ef_acct=ADVGTENRDsec(acctnum,1,SCRNvirtualSubData,mtnrsrc);
				while(!ef_acct)
				{
					FINDFLDGETCHAR(acctnum,"DELETEFLAG",&mdeleteflag);
					if(!mdeleteflag)
					{
						FINDFLDGETINT(acctnum,"FISCAL YEAR",&yearid2);
						FINDFLDGETSTRING(acctnum,"PO IDENTIFICATION",&poid2);
						FINDFLDGETINT(acctnum,"LINE NUMBER",&lineno2);
						if(yearid2==yearid1 
							&& !RDAstrcmp(poid2,poid1)
							&& lineno2==lineno1)
						{
							prev=RDATDataNEW(acctnum);
							FINDFLDGETINT(acctnum,"ACCOUNT NUMBER",&acctno1);
							FINDFLDSETCHAR(acctnum,"DELETEFLAG",deleteflag);
							ADVWRTTRANSsec(acctnum,0,NULL,prev,SCRNvirtualSubData,mtnrsrc);
							if(prev!=NULL) FreeRDATData(prev);
							if(transnum!=(-1) && eyrnum!=(-1) && ryrnum!=(-1) && byrnum!=(-1))
							{
								ZERNRD(transnum);
								FINDFLDSETINT(transnum,"FISCAL YEAR",yearid1);
								FINDFLDSETSTRING(transnum,"PO IDENTIFICATION",poid1);
								FINDFLDSETINT(transnum,"LINE NUMBER",lineno1);
								FINDFLDSETINT(transnum,"ACCOUNT NUMBER",acctno1);
								ef_trans=ADVGTENRDsec(transnum,1,SCRNvirtualSubData,mtnrsrc);
								while(!ef_trans)
								{
									FINDFLDGETCHAR(transnum,"DELETEFLAG",&pdeleteflag);
									if(!pdeleteflag)
									{
										FINDFLDGETINT(transnum,"FISCAL YEAR",&yearid3);
										FINDFLDGETSTRING(transnum,"PO IDENTIFICATION",&poid3);
										FINDFLDGETINT(transnum,"LINE NUMBER",&lineno3);
										FINDFLDGETINT(transnum,"ACCOUNT NUMBER",&acctno2);
										if(yearid3==yearid1
										  && !RDAstrcmp(poid3,poid1) 
										  && lineno3==lineno1
										  && acctno2==acctno1)
										{
										  prev=RDATDataNEW(transnum);
										  FINDFLDGETSHORT(acctnum,"ACCOUNT TYPE",&accttype);
										  FINDFLDGETDOUBLE(transnum,"AMOUNT",&amount);
										  if(venpmt->Use_Taxes==TRUE)
										  {
											FINDFLDGETDOUBLE(transnum,"TAX AMOUNT",&salestax);
											FINDFLDGETDOUBLE(transnum,"COST",&cost);
											FINDFLDGETDOUBLE(transnum,"PRORATE AMOUNT",&prorateamt);
											FINDFLDGETDOUBLE(transnum,"ADMIN FEE",&adminfee);
										  }
										  if(accttype==EXP_ACCT)
										  {
										    ADV4updateoutstanding(purmst,MASTER->passkey->fileno,yearid3,poid3,lineno3,-cost,-salestax,-adminfee,-prorateamt,-amount,TRUE,0,SCRNvirtualSubData,mtnrsrc,venpmt);
										  }
										  FINDFLDGETCHAR(transnum,"DISTRIBUTED",&dist);
										  if(dist==FALSE)
										  {
										    FINDFLDGETSHORT(acctnum,"DEFINITION TYPE",&deftype);
										    FINDFLDGETSHORT(transnum,"PROCESSING MONTH",&p_month);
										    switch(accttype)
										    {
										      case EXP_ACCT:
										      if(purord->use_fiscal_months==TRUE)
										      {
										        ADVupdateundist(acctnum,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,eyrnum,byrnum,yearid3,deftype,"ENCUMBRANCES",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
										      } else {
										        ADVupdateundist(acctnum,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,eyrnum,byrnum,yearid3,deftype,"ENCUMBRANCES",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
										      }
										        break;
										      case REV_ACCT:
										      if(purord->use_fiscal_months==TRUE)
										      {
										        ADVupdateundist(acctnum,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,ryrnum,byrnum,yearid3,deftype,"ESTIMATED REVENUE",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
										      } else {
										        ADVupdateundist(acctnum,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,ryrnum,byrnum,yearid3,deftype,"ESTIMATED REVENUE",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
										      }
										        break;
										      case BAL_ACCT:
										        if(amount<0.0)
										        {
										        if(purord->use_fiscal_months==TRUE)
										        {
										          ADVupdateundist(acctnum,NULL,"ACCOUNT CODE","CREDIT CODE",NULL,(-1),byrnum,yearid3,deftype,"DEBITS",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
										        } else {
										          ADVupdateundist(acctnum,NULL,"ACCOUNT CODE","CREDIT CODE",NULL,(-1),byrnum,yearid3,deftype,"DEBITS",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,mtnrsrc);
											}
										        } else {
										        if(purord->use_fiscal_months==TRUE)
										        {
										          ADVupdateundist(acctnum,NULL,"ACCOUNT CODE","CREDIT CODE",NULL,(-1),byrnum,yearid3,deftype,"CREDITS",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,mtnrsrc);
										        } else {
										          ADVupdateundist(acctnum,NULL,"ACCOUNT CODE","CREDIT CODE",NULL,(-1),byrnum,yearid3,deftype,"CREDITS",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,mtnrsrc);
										        }
										        }
										        break;
										      default:
										        prterr("Invalid Account Type [%d]",accttype);
										        break;
										    } /* end switch/case */
										  } 
										  FINDFLDSETCHAR(transnum,"DELETEFLAG",deleteflag);
										  ADVWRTTRANSsec(transnum,0,NULL,prev,SCRNvirtualSubData,mtnrsrc);		
										  if(prev!=NULL) FreeRDATData(prev);
										}
									}
									ef_trans=ADVNXTNRDsec(transnum,1,SCRNvirtualSubData,mtnrsrc);
									if(poid3!=NULL) Rfree(poid3);
								}
							}
						}
					}
					ef_acct=ADVNXTNRDsec(acctnum,1,SCRNvirtualSubData,mtnrsrc); 	
					if(poid2!=NULL) Rfree(poid2);
				}
			}
			if(acctnum!=(-1)) CLSNRD(acctnum);
			if(transnum!=(-1)) CLSNRD(transnum);
			if(eyrnum!=(-1)) CLSNRD(eyrnum);
			if(ryrnum!=(-1)) CLSNRD(ryrnum);
			if(byrnum!=(-1)) CLSNRD(byrnum);
		}
		if(poid1!=NULL) Rfree(poid1);
		if(poid2!=NULL) Rfree(poid2);
		if(poid3!=NULL) Rfree(poid3);
		DELETE_SUBORDINATES(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update,mbl_purlin,MASTER->passkey->fileno,MASTER->module,mtnrsrc);
		if(mbl_purlin!=NULL)
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
static void SetAppDef(RDArsrc *mtnrsrc,short update)
{
	char *date=NULL,*timex=NULL;
	
	FINDRSCSETSTRING(mtnrsrc,"[PURLIN][SOURCE USER]",USERLOGIN);
	date=GETCURRENTDATE();
	timex=GETCURRENTTIME();
	FINDRSCSETSTRING(mtnrsrc,"[PURLIN][ENTRY DATE]",date);
	FINDRSCSETSTRING(mtnrsrc,"[PURLIN][ENTRY TIME]",timex);
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
	if(update)
	{
		 updatersrc(mtnrsrc,"[PURLIN][SOURCE USER]");
		 updatersrc(mtnrsrc,"[PURLIN][ENTRY DATE]");
		 updatersrc(mtnrsrc,"[PURLIN][ENTRY TIME]");
	}
}
static void SetAppDefcb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
	void (*SubFunc)(...),void *args,RDATData *previous)
{
	SetAppDef(mtnrsrc,TRUE);
}
static void seteditable(RDArsrc *mtnrsrc,RDATData *previous,short update)
{
	char closed=FALSE;

	FINDFLDGETCHAR(purmst,"CLOSED",&closed);
	if(closed)
	{
		if(update)
		{
			updateeditfilersrcsnokeys(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,FALSE);
		} else {
			seteditfilersrc(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,FALSE,FALSE);
		}
		FINDRSCSETEDITABLE(mtnrsrc,"[PURLIN][DELETEFLAG]",TRUE);
		SetRDAScrolledListEditable(mtnrsrc,MASTER->passkey->fileno,MASTER->passkey->keyno,FALSE,FALSE);
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
	filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
	seteditable(mtnrsrc,previous,TRUE);
}
static void totalcalc(RDArsrc *mtnrsrc)
{
	double total=0.0,cost=0.0,sales_tax=0.0;
	double adminfee=0.0,prorateamt=0.0;

	readwidget(mtnrsrc,"[PURLIN][COST]");
	readwidget(mtnrsrc,"[PURLIN][TAX AMOUNT]");
	readwidget(mtnrsrc,"[PURLIN][PRORATE AMOUNT]");
	readwidget(mtnrsrc,"[PURLIN][ADMIN FEE]");
	FINDRSCGETDOUBLE(mtnrsrc,"[PURLIN][COST]",&cost);
	FINDRSCGETDOUBLE(mtnrsrc,"[PURLIN][TAX AMOUNT]",&sales_tax);
	FINDRSCGETDOUBLE(mtnrsrc,"[PURLIN][ADMIN FEE]",&adminfee);
	FINDRSCGETDOUBLE(mtnrsrc,"[PURLIN][PRORATE AMOUNT]",&prorateamt);
	total=sales_tax+cost+adminfee+prorateamt;
	FINDRSCSETDOUBLE(mtnrsrc,"[PURLIN][TOTAL]",total);
	updatersrc(mtnrsrc,"[PURLIN][TOTAL]");
	updateSCRNvirtuals(mtnrsrc);
}
static void caltotal(RDArsrc *mtnrsrc)
{
	int quantity=0;
	double percdisc=0.0,unitrate=0.0,total=0.0;

	readwidget(mtnrsrc,"[PURLIN][QUANTITY]");
	if(FINDRSCGETINT(mtnrsrc,"[PURLIN][QUANTITY]",&quantity)) return;
	readwidget(mtnrsrc,"[PURLIN][UNIT RATE]");
	if(FINDRSCGETDOUBLE(mtnrsrc,"[PURLIN][UNIT RATE]",&unitrate)) return;
	readwidget(mtnrsrc,"[PURLIN][PERCENTAGE DISCOUNT]");
	FINDRSCGETDOUBLE(mtnrsrc,"[PURLIN][PERCENTAGE DISCOUNT]",&percdisc);
	if(percdisc!=0.0)
	{
		total=((unitrate*quantity)*100)-((unitrate*quantity*percdisc));
	} else {
		total=(unitrate*quantity)*100.0;
	}
	if(venpmt->Use_Taxes==TRUE)
	{
		FINDRSCSETDOUBLE(mtnrsrc,"[PURLIN][COST]",total);
		updatersrc(mtnrsrc,"[PURLIN][COST]");
		totalcalc(mtnrsrc);
	} else {
		FINDRSCSETDOUBLE(mtnrsrc,"[PURLIN][TOTAL]",total);
		updatersrc(mtnrsrc,"[PURLIN][TOTAL]");
		updateSCRNvirtuals(mtnrsrc);
	}
}
