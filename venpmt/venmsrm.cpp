/* venmsrm.c - Vendor Payments Address Maintain */
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
void venmsrm(short,void *targetkey);
extern MakeBrowseList *mbl_venmsr;
extern char editable;
extern void browse_venmsr(RDArsrc *,RDATData *);
extern MaintainMaster *MASTER;
extern RDAvenpmt *venpmt;

void venmsrm(short dowhich,void *targetkey)
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
					GetRDATData(MASTER->passkey->fileno,previous);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,previous);
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
				GetRDATData(MASTER->passkey->fileno,previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
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
				} else {
					GetRDATData(MASTER->passkey->fileno,previous);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
			}
			break;
	}
	GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,previous);
	if(mbl_venmsr!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,previous);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_venmsr,previous);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,previous);
	addbtnrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,previous);
	MakeMasterButtons(mtnrsrc,MASTER);
	computeallSCRNvirtuals(mtnrsrc);
	if(mbl_venmsr!=NULL)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	}
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,
		(mbl_venmsr==NULL?TRUE:FALSE));
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
		member->parent,keyno,NULL,NULL,
		NULL,NULL,NULL,NULL);
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
		member->parent,keyno,NULL,NULL,
		NULL,NULL,NULL,NULL);
}
static void SETID(RDArsrc *mtnrsrc,RDATData *previous)
{
	ADV4SET_KEYFUNC(mtnrsrc,MASTER,previous,SCRNvirtualSubData,
		(void *)mtnrsrc,NULL,NULL,
		NULL,NULL,NULL,NULL);
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
	char *rcddesc=NULL,*vendorid1=NULL,*vendorid2=NULL,*vendorid3=NULL;
	char *vendorid4=NULL,*invid1=NULL,*invid2=NULL;
	char deleteflag=0,mdeleteflag=0,pdeleteflag=0,vdeleteflag=0;
	char dist=FALSE,cleared=FALSE,*debcode1=NULL,*credcode1=NULL;
	short venyr=(-1),venadr=(-1),veninv=(-1),venvch=(-1);
	short fineyr=(-1),finryr=(-1),finbyr=(-1);
	short ef_yr=0,ef_add=0,ef_inv=0,ef_vch=0;
	short accttype=0,deftype=0,p_month=0;
	double amount=0.0;
	int yearid1=0,yearid2=0,yearid3=0,procyrid=0;
	RDATData *prev=NULL;

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
		WRTTRANS(MASTER->passkey->fileno,0,NULL,previous);
		FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			FINDFLDGETSTRING(MASTER->passkey->fileno,"VENDOR IDENTIFICATION",&vendorid1);
			venadr=OPNNRDsec(MASTER->module,"VENADR",TRUE,TRUE,FALSE);	
			if(venadr!=(-1))
			{
				ZERNRD(venadr);
				FINDFLDSETSTRING(venadr,"VENDOR IDENTIFICATION",vendorid1);
				ef_add=ADVGTENRDsec(venadr,1,SCRNvirtualSubData,mtnrsrc);
				while(!ef_add)
				{
					FINDFLDGETCHAR(venadr,"DELETEFLAG",&mdeleteflag);
					if(!mdeleteflag)
					{
						FINDFLDGETSTRING(venadr,"VENDOR IDENTIFICATION",&vendorid2);
						if(!RDAstrcmp(vendorid1,vendorid2)) 
						{
							prev=RDATDataNEW(venadr);
							FINDFLDSETCHAR(venadr,"DELETEFLAG",deleteflag);
							ADVWRTTRANSsec(venadr,0,NULL,prev,SCRNvirtualSubData,mtnrsrc);		
							if(prev!=NULL) FreeRDATData(prev);
						}
					}
					ef_add=ADVNXTNRDsec(venadr,1,SCRNvirtualSubData,mtnrsrc);
					if(vendorid2!=NULL) { Rfree(vendorid2); vendorid2=NULL; } 
				}
			}
			if(venadr!=(-1)) CLSNRD(venadr);
			venyr=OPNNRDsec(MASTER->module,"VENYR",TRUE,TRUE,FALSE);
			veninv=OPNNRDsec(MASTER->module,"VENINV",TRUE,TRUE,FALSE);
			venvch=OPNNRDsec(MASTER->module,"VENVCH",TRUE,TRUE,FALSE);
			fineyr=OPNNRDsec("FINMGT","FINEYR",TRUE,TRUE,FALSE);
			finryr=OPNNRDsec("FINMGT","FINRYR",TRUE,TRUE,FALSE);
			finbyr=OPNNRDsec("FINMGT","FINBYR",TRUE,TRUE,FALSE);
			if(venyr!=(-1))
			{
				ZERNRD(venyr);
				FINDFLDSETSTRING(venyr,"VENDOR IDENTIFICATION",vendorid1);
				ef_yr=ADVGTENRDsec(venyr,1,SCRNvirtualSubData,mtnrsrc);
				while(!ef_yr)
				{
					FINDFLDGETCHAR(venyr,"DELETEFLAG",&mdeleteflag);
					if(!mdeleteflag)
					{
						FINDFLDGETSTRING(venyr,"VENDOR IDENTIFICATION",&vendorid2);
						if(!RDAstrcmp(vendorid1,vendorid2)) 
						{
							FINDFLDGETINT(venyr,"CALENDAR YEAR",&yearid1);
							prev=RDATDataNEW(venyr);
							FINDFLDSETCHAR(venyr,"DELETEFLAG",deleteflag);
							ADVWRTTRANSsec(venyr,0,NULL,prev,SCRNvirtualSubData,mtnrsrc);
							if(prev!=NULL) FreeRDATData(prev);
							if(veninv!=(-1))
							{
								ZERNRD(veninv);
								FINDFLDSETSTRING(veninv,"VENDOR IDENTIFICATION",vendorid1);
								FINDFLDSETINT(veninv,"CALENDAR YEAR",yearid1);
								ef_inv=ADVGTENRDsec(veninv,1,SCRNvirtualSubData,mtnrsrc);
								while(!ef_inv)
								{
									FINDFLDGETCHAR(veninv,"DELETEFLAG",&pdeleteflag);
									if(!pdeleteflag)
									{
										FINDFLDGETSTRING(veninv,"VENDOR IDENTIFICATION",&vendorid3);
										FINDFLDGETINT(veninv,"CALENDAR YEAR",&yearid2);
										if(!RDAstrcmp(vendorid3,vendorid1) && 
											yearid2==yearid1)
										{
										  FINDFLDGETSTRING(veninv,"INVOICE IDENTIFICATION",&invid1);
										  prev=RDATDataNEW(veninv);
										  FINDFLDSETCHAR(veninv,"DELETEFLAG",deleteflag);
										  ADVWRTTRANSsec(veninv,0,NULL,prev,SCRNvirtualSubData,mtnrsrc);		
										  if(prev!=NULL) FreeRDATData(prev);
										  if(venvch!=(-1) && fineyr!=(-1) && finryr!=(-1) && finbyr!=(-1))
										  {
										    ZERNRD(venvch);
										    FINDFLDSETSTRING(venvch,"VENDOR IDENTIFICATION",vendorid1);
										    FINDFLDSETINT(venvch,"CALENDAR YEAR",yearid1);
										    FINDFLDSETSTRING(venvch,"INVOICE IDENTIFICATION",invid1);
										    ef_vch=ADVGTENRDsec(venvch,1,SCRNvirtualSubData,mtnrsrc);
										    while(!ef_vch)
										    {
 										      FINDFLDGETCHAR(venvch,"DELETEFLAG",&vdeleteflag);
										      if(!vdeleteflag)
										      {
										        FINDFLDGETSTRING(venvch,"VENDOR IDENTIFICATION",&vendorid4);
										        FINDFLDGETINT(venvch,"CALENDAR YEAR",&yearid3);
										        FINDFLDGETSTRING(venvch,"INVOICE IDENTIFICATION",&invid2);
										        if(!RDAstrcmp(vendorid4,vendorid1) && 
										          yearid3==yearid1 && 
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
										      if(vendorid4!=NULL) Rfree(vendorid4);
										      if(invid2!=NULL) Rfree(invid2);
										      ef_vch=ADVNXTNRDsec(venvch,1,SCRNvirtualSubData,mtnrsrc);
										    }
										  }
										}
									}
									ef_inv=ADVNXTNRDsec(veninv,1,SCRNvirtualSubData,mtnrsrc);
									if(vendorid3!=NULL) Rfree(vendorid3);
									if(invid1!=NULL) Rfree(invid1); 
								}
							}
						}
					}
					ef_yr=ADVNXTNRDsec(venyr,1,SCRNvirtualSubData,mtnrsrc); 	
					if(vendorid2!=NULL) Rfree(vendorid2);
				}
			}
			if(venyr!=(-1)) CLSNRD(venyr);
			if(veninv!=(-1)) CLSNRD(veninv);
			if(venvch!=(-1)) CLSNRD(venvch);
			if(fineyr!=(-1)) CLSNRD(fineyr);
			if(finryr!=(-1)) CLSNRD(finryr);
			if(finbyr!=(-1)) CLSNRD(finbyr);
		}
		if(vendorid1!=NULL) Rfree(vendorid1);
		if(vendorid2!=NULL) Rfree(vendorid2);
		if(vendorid3!=NULL) Rfree(vendorid3);
		if(vendorid4!=NULL) Rfree(vendorid4);
		if(invid1!=NULL) Rfree(invid1);
		if(invid2!=NULL) Rfree(invid2);
		DELETE_SUBORDINATES(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update,mbl_venmsr,MASTER->passkey->fileno,MASTER->module,mtnrsrc);
		if(mbl_venmsr!=NULL)
		{
			quit_record(mtnrsrc,previous);
		} else {
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
