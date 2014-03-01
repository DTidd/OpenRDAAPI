/* restmst0.c - RESTMSTR Maintain Screen */
#include <stdio.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <mix.h>
#include <fin.h>
#include <rlstmgt.h>

extern char editable;
extern void browse_utl(RDArsrc *,RDATData *);
static void save_check(RDArsrc *,RDATData *,short);
static void save_record(RDArsrc *,RDATData *,short);
static void save_record_up(RDArsrc *,RDATData *);
static void save_record_nup(RDArsrc *,RDATData *);
static void quit_record(RDArsrc *,RDATData *);
static void quit_record_test(RDArsrc *,RDATData *);
static void readscreen(RDArsrc *,RDATData *);
MakeBrowseList *mbl_utlmstr=NULL;
extern MaintainMaster *MASTER;
void mtnutlmstr(short,void *targetkey);

static void resetdefs(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(MASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	updateallrsrc(mtnrsrc);
	SET_KEYFUNC(mtnrsrc,MASTER,previous,SCRNvirtualSubData,(void *)mtnrsrc,
		MASTER->inheritrsrcs);
}
static void getpreviouscb(RDArmem *member,RDATData *previous)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1))
	{
		keyno=MASTER->passkey->keyno;
	}
	GET_PREVIOUS(member->parent,MASTER,previous,SCRNvirtualSubData,
		member->parent,keyno);
}
static void getnextcb(RDArmem *member,RDATData *previous)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1))
	{
		keyno=MASTER->passkey->keyno;
	}
	GET_NEXT(member->parent,MASTER,previous,SCRNvirtualSubData,
		member->parent,keyno);
}
static void SETID(RDArsrc *mtnrsrc,RDATData *previous)
{
	SET_KEYFUNC(mtnrsrc,MASTER,previous,SCRNvirtualSubData,(void *)mtnrsrc,
		MASTER->inheritrsrcs);
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *MASTER) 
{
	SET_MASTER(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
}
void mtnutlmstr(short dowhich,void *targetkey)
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
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,previous);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,previous);
	if(mbl_utlmstr!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,previous);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_utl,previous);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,previous);
	MakeMasterButtons(mtnrsrc,MASTER);
	computeallSCRNvirtuals(mtnrsrc);
	if(mbl_utlmstr!=NULL)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	}
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,
		(mbl_utlmstr==NULL?TRUE:FALSE));
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
			ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","SAVE WARNING",
				desc,(update ? save_record_now:save_record_now1),
				NULL,FALSE,2,mtnrsrc,previous,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	save_record(mtnrsrc,previous,update);
}
static void save_record_up(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check(mtnrsrc,previous,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check(mtnrsrc,previous,FALSE);
}
static void save_record(RDArsrc *mtnrsrc,RDATData *previous,short update_list)
{
	char *rcddesc=NULL,deleteflag=FALSE;
	char vdeleteflag=FALSE,tdeleteflag=FALSE;
	char *realestateid=NULL,*realestateid1=NULL,*realestateid2=NULL;
	char dist=FALSE,*ownid=NULL,*ownid1=NULL,*ownid2=NULL;
	short taxdwr=(-1),byrnum=(-1),ryrnum=(-1),paidnum=(-1),trannum=(-1);
	short type=0,ef_tran=0,ef_paid=0;
	short deftype=0,p_month=0;
	int tran=0,tran1=0,yearid=0,yearid1=0,yearid2=0,FiscalYear=0;
	RDArsrc *tmprsrc=NULL;
	RDATData *prev=NULL;
	double amount=0.0;

	readscreen(mtnrsrc,previous);
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
			FINDFLDGETSTRING(MASTER->passkey->fileno,"REAL ESTATE IDENTIFICATION",&realestateid);
			FINDFLDGETSTRING(MASTER->passkey->fileno,"OWNER IDENTIFICATION",&ownid);
			FINDFLDGETINT(MASTER->passkey->fileno,"TAX YEAR",&yearid);
			taxdwr=OPNNRDsec("TAXCLT","TAXDRWR",TRUE,TRUE,FALSE);
			trannum=OPNNRDsec(MASTER->module,"RESTRAN",TRUE,TRUE,FALSE);
			paidnum=OPNNRDsec(MASTER->module,"RESTPAID",TRUE,TRUE,FALSE);
			ryrnum=OPNNRDsec("FINMGT","FINRYR",TRUE,TRUE,FALSE);
			byrnum=OPNNRDsec("FINMGT","FINBYR",TRUE,TRUE,FALSE);
			tmprsrc=RDArsrcNEW(MASTER->module,NULL);
			copyDFincvir(mtnrsrc,tmprsrc);
			addDFincvir(tmprsrc,"TAXCLT","TAXDRWR",NULL,taxdwr);
			addDFincvir(tmprsrc,MASTER->module,"RESTRAN",NULL,trannum);
			addDFincvir(tmprsrc,MASTER->module,"RESTPAID",NULL,paidnum);
			addDFincvir(tmprsrc,"FINMGT","FINRYR",NULL,ryrnum);
			addDFincvir(tmprsrc,"FINMGT","FINBYR",NULL,byrnum);
			GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
			if(trannum!=(-1))
			{
				ZERNRD(trannum);
				FINDFLDSETSTRING(trannum,"REAL ESTATE IDENTIFICATION",realestateid);
				FINDFLDSETSTRING(trannum,"OWNER IDENTIFICATION",ownid);
				FINDFLDSETINT(trannum,"TAX YEAR",yearid);
				ef_tran=ADVGTENRDsec(trannum,1,SCRNvirtualSubData,tmprsrc);
				while(!ef_tran)
				{
					FINDFLDGETSTRING(trannum,"REAL ESTATE IDENTIFICATION",&realestateid1);
					FINDFLDGETSTRING(trannum,"OWNER IDENTIFICATION",&ownid1);
					FINDFLDGETINT(trannum,"TAX YEAR",&yearid1);
					if(RDAstrcmp(realestateid,realestateid1) || RDAstrcmp(ownid,ownid1) || yearid!=yearid1) break;
					FINDFLDGETCHAR(trannum,"DELETEFLAG",&tdeleteflag);
					if(!tdeleteflag)
					{
						FINDFLDGETINT(trannum,"TRANSACTION NUMBER",&tran);
						prev=RDATDataNEW(trannum);
						FINDFLDGETDOUBLE(trannum,"AMOUNT",&amount);
						ADVUpdateRealEstateTransactions(MASTER->passkey->fileno,(-1),trannum,-amount,SCRNvirtualSubData,mtnrsrc);

						FINDFLDSETCHAR(trannum,"DELETEFLAG",deleteflag);
						ADVWRTTRANSsec(trannum,0,NULL,prev,SCRNvirtualSubData,tmprsrc);		
						if(prev!=NULL) FreeRDATData(prev);
						if(paidnum!=(-1) && ryrnum!=(-1) && byrnum!=(-1))
						{
							ZERNRD(paidnum);
							FINDFLDSETSTRING(paidnum,"REAL ESTATE IDENTIFICATION",realestateid);
							FINDFLDSETSTRING(paidnum,"OWNER IDENTIFICATION",ownid);
							FINDFLDSETINT(paidnum,"TAX YEAR",yearid);
							FINDFLDSETINT(paidnum,"TRANSACTION NUMBER",tran);
							ef_paid=ADVGTENRDsec(paidnum,1,SCRNvirtualSubData,tmprsrc);
							while(!ef_paid)
							{
								FINDFLDGETSTRING(paidnum,"REAL ESTATE IDENTIFICATION",&realestateid2);
								FINDFLDGETSTRING(paidnum,"OWNER IDENTIFICATION",&ownid2);
								FINDFLDGETINT(paidnum,"TAX YEAR",&yearid2);
								FINDFLDGETINT(paidnum,"TRANSACTION NUMBER",&tran1);
								if(RDAstrcmp(realestateid1,realestateid2) || RDAstrcmp(ownid1,ownid2) || yearid1!=yearid2 || tran!=tran1) break;
								FINDFLDGETCHAR(paidnum,"DELETEFLAG",&vdeleteflag);
								if(!vdeleteflag)
								{
									prev=RDATDataNEW(paidnum);
									FINDFLDGETCHAR(paidnum,"DISTRIBUTED",&dist);
									if(dist==FALSE)
									{
										FINDFLDGETSHORT(paidnum,"TRANSACTION TYPE",&type);
										FINDFLDGETSHORT(paidnum,"DEFINITION TYPE",&deftype);
										FINDFLDGETSHORT(paidnum,"PROCESSING MONTH",&p_month);
										FINDFLDGETDOUBLE(paidnum,"AMOUNT",&amount);
										FINDFLDGETINT(paidnum,"FISCAL YEAR",&FiscalYear);
										if(!type)
										{
											ADVupdateundist(trannum,NULL,"CASH CODE","REVENUE CODE",NULL,ryrnum,byrnum,FiscalYear,deftype,"REVENUE",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,tmprsrc);
										} else {
											ADVupdateundist(trannum,"ACCOUNT CODE","CASH CODE","REVENUE CODE",NULL,ryrnum,byrnum,FiscalYear,deftype,"REVENUE",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,tmprsrc);
										}
									}
									ADVUpdateRealEstatePayments(taxdwr,MASTER->passkey->fileno,trannum,paidnum,-amount,SCRNvirtualSubData,tmprsrc);
									FINDFLDSETCHAR(paidnum,"DELETEFLAG",deleteflag);
									ADVWRTTRANSsec(paidnum,0,NULL,prev,SCRNvirtualSubData,tmprsrc);		
									if(prev!=NULL) FreeRDATData(prev);
								}		
								ef_paid=ADVNXTNRDsec(paidnum,1,SCRNvirtualSubData,tmprsrc);
							}
						}	
					}
					ef_tran=ADVNXTNRDsec(trannum,1,SCRNvirtualSubData,tmprsrc);
				}
			}
			if(tmprsrc!=NULL) free_rsrc(tmprsrc);
			if(taxdwr!=(-1)) CLSNRD(taxdwr);
			if(trannum!=(-1)) CLSNRD(trannum);
			if(paidnum!=(-1)) CLSNRD(paidnum);
			if(ryrnum!=(-1)) CLSNRD(ryrnum);
			if(byrnum!=(-1)) CLSNRD(byrnum);
		}
		if(realestateid!=NULL) Rfree(realestateid);
		if(realestateid1!=NULL) Rfree(realestateid1);
		if(realestateid2!=NULL) Rfree(realestateid2);
		if(ownid!=NULL) Rfree(ownid);
		if(ownid1!=NULL) Rfree(ownid1);
		if(ownid2!=NULL) Rfree(ownid2);

		DELETE_SUBORDINATES(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update_list,mbl_utlmstr,MASTER->passkey->fileno,MASTER->module,mtnrsrc);
		if(mbl_utlmstr!=NULL) quit_record(mtnrsrc,previous);
			else GetRDATData(MASTER->passkey->fileno,previous);
	}
}
static void quit_record(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(previous!=NULL) FreeRDATData(previous);
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
static void save_record_quit(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check(mtnrsrc,previous,TRUE);
}
static void quit_record_test(RDArsrc *mtnrsrc,RDATData *previous)
{
	QUIT_RECORD_TEST(mtnrsrc,MASTER->passkey->fileno,previous,readscreen,
		save_record_quit,quit_record,previous);
}
