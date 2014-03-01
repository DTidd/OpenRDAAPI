/* restoyrm.c - Real Estate Master Maintain Screen */
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
#include <rlstmgt.h>

extern short filenum,rstmst,fsclyr,rstown,rstyr,taxmst;
extern char editable;
extern char *module;
extern void browse_msr(RDArsrc *,RDATData *);
static void save_record(RDArsrc *,RDATData *,short);
static void save_check(RDArsrc *,RDATData *,short);
static void save_record_quit(RDArsrc *,RDATData *);
static void readscreen(RDArsrc *,RDATData *);
static void save_record_up(RDArsrc *,RDATData *);
static void save_record_nup(RDArsrc *,RDATData *);
static void quit_record(RDArsrc *,RDATData *);
static void quit_record(RDArsrc *,RDATData *);
static void quit_record(RDArsrc *,RDATData *);
static void getrecord(RDArsrc *,RDATData *);
MakeBrowseList *mbl_restoyr=NULL;
extern MaintainMaster *MASTER;
void restoyrm(short,void *targetkey);
static void getsupportingfiles(RDArsrc *);

static void save_record_quit(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check(mtnrsrc,previous,TRUE);
}
static void quit_record_test(RDArsrc *mtnrsrc,RDATData *previous)
{
	char *desc=NULL;

	readscreen(mtnrsrc,previous);
	if(QUITvfy(filenum,previous))
	{
		desc=QUITvfydesc(filenum,previous);
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!",
			desc,save_record_quit,quit_record,FALSE,2,mtnrsrc,previous,NULL);
		if(desc!=NULL) Rfree(desc);
	} else {
		quit_record(mtnrsrc,previous);
	}
}
void restoyrm(short dowhich,void *targetkey)
{
	RDArsrc *mtnrsrc=NULL;
	int nofields=0,x;
	NRDfield *fields=NULL,*fldx;
	short edit_rsrc=FALSE;
	RDATData *previous=NULL;
	
	mtnrsrc=RDArsrcNEW(module,MASTER->maintain_screen);
	addDFincvir(mtnrsrc,module,"RESTOYR",NULL,filenum);
	addDFincvir(mtnrsrc,module,"RESTMST",NULL,rstmst);
	addDFincvir(mtnrsrc,module,"RESTOWN",NULL,rstown);
	addDFincvir(mtnrsrc,module,"RESTYEAR",NULL,rstyr);
	addDFincvir(mtnrsrc,"TAXCLT","TAXMST",NULL,taxmst);
	addDFincvir(mtnrsrc,"FINMGT","FINYEAR",NULL,fsclyr);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	ZERNRD(filenum);
	previous=RDATDataNEW(filenum);
	nofields=NUMFLDS(filenum);
	fields=FLDPOINTER(filenum);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=(short)editable;
			if((fldx->type==DOLLARS) &&
				!strncmp(fldx->name,"JANUARY",7) &&
				!strncmp(fldx->name,"FEBRUARY",8) &&
				!strncmp(fldx->name,"MARCH",5) &&
				!strncmp(fldx->name,"APRIL",5) &&
				!strncmp(fldx->name,"MAY",3) &&
				!strncmp(fldx->name,"JUNE",4) &&
				!strncmp(fldx->name,"JULY",4) &&
				!strncmp(fldx->name,"AUGUST",6) &&
				!strncmp(fldx->name,"SEPTEMBER",9) &&
				!strncmp(fldx->name,"OCTOBER",7) &&
				!strncmp(fldx->name,"NOVEMBER",8) &&
				!strncmp(fldx->name,"DECEMBER",8))
			{
				nonlstmakefld(mtnrsrc,filenum,fldx,FALSE);
			} else {
				nonlstmakefld(mtnrsrc,filenum,fldx,edit_rsrc);
			}
		}
	}
	nofields=0;
	file2rsrc(rstmst,mtnrsrc,FALSE);
	file2rsrc(rstown,mtnrsrc,FALSE);
	file2rsrc(rstyr,mtnrsrc,FALSE);
	file2rsrc(taxmst,mtnrsrc,FALSE);
	file2rsrc(fsclyr,mtnrsrc,FALSE);
	FINDRSCSETFUNC(mtnrsrc,"[RESTOYR][REAL ESTATE IDENTIFICATION]",
		getrecord,previous);
	FINDRSCSETFUNC(mtnrsrc,"[RESTOYR][OWNER IDENTIFICATION]",getrecord,
		previous);
	FINDRSCSETFUNC(mtnrsrc,"[RESTOYR][FISCAL YEAR]",getrecord,previous);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			ApplyPassKey(MASTER->passkey);
			if(ADVEQLNRDsec(filenum,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(filenum,MASTER->passkey->keyno);
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
			} else {
				GetRDATData(filenum,previous);
				filerecord2rsrc(filenum,mtnrsrc);
			}
			break;
		case 2:
			ApplyPassKey(MASTER->passkey);
			ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
			break;
		default:
		case 0:
			if(ADVEQLNRDKEYsec(filenum,MASTER->passkey->keyno,targetkey,SCRNvirtualSubData,mtnrsrc))
			{
				ZERNRD(filenum);
				ApplyPassKey(MASTER->passkey);
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
			} else {
				GetRDATData(filenum,previous);
				filerecord2rsrc(filenum,mtnrsrc);
			}
			break;
	}
	getsupportingfiles(mtnrsrc);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	if(editable)
	{
		addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,previous);
		if(mbl_restoyr!=NULL)
		{
			addrfcbrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
		} else {
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
		}
	}
	if(mbl_restoyr==NULL)
	{
		addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_msr,previous);
	}
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakeMasterButtons(mtnrsrc,MASTER);
	computeallSCRNvirtuals(mtnrsrc);
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,
		(mbl_restoyr==NULL ? TRUE:FALSE));
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
static void readscreen(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(filenum);
	readallwidgets(mtnrsrc);
	rsrc2filerecord(filenum,mtnrsrc);
}
static void save_check(RDArsrc *mtnrsrc,RDATData *previous,
	short update_list)
{
	char *temp=NULL,*warnmessage=NULL,delflag=FALSE,*ownid=NULL;
	int yearid=0;

	FINDFLDGETCHAR(filenum,"DELETEFLAG",&delflag);
	if(!delflag)
	{
		if(rstmst!=(-1))
		{
			FINDFLDGETSTRING(filenum,"REAL ESTATE IDENTIFICATION",&temp);
			FINDFLDSETSTRING(rstmst,"REAL ESTATE IDENTIFICATION",temp);
			if(ADVEQLNRDsec(rstmst,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(rstmst,1);
				warnmessage=Rmalloc(RDAstrlen(temp)+137);
				sprintf(warnmessage,"The REAL ESTATE IDENTIFICATION of [%s] entered is not valid.  It does not exist as a Real Estate Master.",(temp==NULL?"":temp));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID REAL ESTATE IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr("The REAL ESTATE IDENTIFICATION of [%s] entered does not exist in the [RLSTMGT][RESTMST] data file.",(temp==NULL ? "":temp));
				if(temp!=NULL) Rfree(temp); 
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(rstmst,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(temp)+140);
					sprintf(warnmessage,"The REAL ESTATE IDENTIFICATION of [%s] entered is deleted.  It exists as a Real Estate, but is presently set up in a deleted state.",(temp==NULL ? "":temp));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED REAL ESTATE IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					prterr("The REAL ESTATE IDENTIFICATION of [%s] entered is deleted.  It exist as a Real Estate, but is presently set up in a deleted state.",(temp==NULL?"":temp));
					if(temp!=NULL) Rfree(temp); 
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				}
			}
		}
		if(taxmst!=(-1))
		{
			FINDFLDGETSTRING(filenum,"OWNER IDENTIFICATION",&temp);
			FINDFLDSETSTRING(taxmst,"OWNER IDENTIFICATION",temp);
			if(ADVEQLNRDsec(taxmst,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(taxmst,1);
				warnmessage=Rmalloc(RDAstrlen(temp)+137);
				sprintf(warnmessage,"The OWNER IDENTIFICATION of [%s] entered is not valid.  It does not exist as a Tax Master.",(temp==NULL?"":temp));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID OWNER IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr("The OWNER IDENTIFICATION of [%s] entered does not exist in the [TAXCLT][TAXMST] data file.",(temp==NULL ? "":temp));
				if(temp!=NULL) Rfree(temp); 
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(taxmst,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(temp)+140);
					sprintf(warnmessage,"The OWNER IDENTIFICATION of [%s] entered is deleted.  It exists as a Tax Master, but is presently set up in a deleted state.",(temp==NULL ? "":temp));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED OWNER IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					prterr("The OWNER IDENTIFICATION of [%s] entered is deleted.  It exist as a Tax Master, but is presently set up in a deleted state.",(temp==NULL?"":temp));
					if(temp!=NULL) Rfree(temp); 
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				}
			}
		}
		if(fsclyr!=(-1))
		{
			FINDFLDGETINT(filenum,"FISCAL YEAR",&yearid);
			FINDFLDSETINT(fsclyr,"FISCAL YEAR",yearid);
			if(ADVEQLNRDsec(fsclyr,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(fsclyr,1);
				warnmessage=Rmalloc(RDAstrlen(temp)+137);
				sprintf(warnmessage,"The FISCAL YEAR of [%d] entered is not valid.  It does not exist as a Fiscal Year.",yearid);
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID FISCAL YEAR!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr("The FISCAL YEAR of [%d] entered does not exist in the [FINMGT][FINYEAR] data file.",yearid);
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(fsclyr,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(temp)+140);
					sprintf(warnmessage,"The FISCAL YEAR of [%d] entered is deleted.  It exists as a Fiscal Year, but is presently set up in a deleted state.",yearid);
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED FISCAL YEAR!",warnmessage,NULL,NULL,TRUE,NULL);
					prterr("The FISCAL YEAR of [%d] entered is deleted.  It exist as a Fiscal Year, but is presently set up in a deleted state.",yearid);
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				} else {
					FINDFLDGETCHAR(fsclyr,"ACTIVE",&delflag);
					if(!delflag)
					{
						warnmessage=Rmalloc(RDAstrlen(temp)+140);
						sprintf(warnmessage,"The FISCAL YEAR of [%d] entered is not active.  It exists as a Fiscal Year, but is presently set up in an inactive state.",yearid);
						WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE FISCAL YEAR!",warnmessage,NULL,NULL,TRUE,NULL);
						prterr("The FISCAL YEAR of [%d] entered is not active.  It exists as a Fiscal Year, but is presently set up in an inactive state.",yearid);
						if(warnmessage!=NULL) Rfree(warnmessage);
						return;
					}
				}
			}
		}
		if(rstown!=(-1))
		{
			FINDFLDGETSTRING(filenum,"REAL ESTATE IDENTIFICATION",&temp);
			FINDFLDGETSTRING(filenum,"OWNER IDENTIFICATION",&ownid);
			FINDFLDSETSTRING(rstown,"REAL ESTATE IDENTIFICATION",temp);
			FINDFLDSETSTRING(rstown,"OWNER IDENTIFICATION",ownid);
			if(ADVEQLNRDsec(rstown,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(rstown,1);
				warnmessage=Rmalloc(RDAstrlen(temp)+RDAstrlen(ownid)+160);
				sprintf(warnmessage,"The OWNER IDENTIFICATION of [%s] entered is not valid.  It does not exist as an Owner of [%s] Real Estate.",(ownid!=NULL ? ownid:""),(temp==NULL?"":temp));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID OWNER IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr("The OWNER IDENTIFICATION of [%s] entered does not exist in the [RLSTMGT][RESTOWN] data file.",(temp==NULL ? "":temp));
				if(temp!=NULL) Rfree(temp); 
				if(ownid!=NULL) Rfree(ownid);
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(rstown,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(temp)+RDAstrlen(ownid)+160);
					sprintf(warnmessage,"The OWNER IDENTIFICATION of [%s] entered is deleted.  It exists as a Real Estate Owner for [%s], but is presently set up in a deleted state.",(ownid!=NULL ? ownid:""),(temp==NULL ? "":temp));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED OWNER IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					prterr("The OWNER IDENTIFICATION of [%s] entered is deleted.  It exists as a Real Estate Owner for [%s], but is presently set up in a deleted state.",(ownid!=NULL ? ownid:""),(temp==NULL?"":temp));
					if(temp!=NULL) Rfree(temp); 
					if(ownid!=NULL) Rfree(ownid);
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				}
			}
		}
		if(rstyr!=(-1))
		{
			FINDFLDGETSTRING(filenum,"REAL ESTATE IDENTIFICATION",&temp);
			FINDFLDGETINT(filenum,"FISCAL YEAR",&yearid);
			FINDFLDSETSTRING(rstyr,"REAL ESTATE IDENTIFICATION",temp);
			FINDFLDSETINT(rstyr,"FISCAL YEAR",yearid);
			if(ADVEQLNRDsec(rstyr,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(rstyr,1);
				warnmessage=Rmalloc(RDAstrlen(temp)+160);
				sprintf(warnmessage,"The FISCAL YEAR of [%d] entered is not valid.  It does not exist as a Real Estate Year Master for [%s].",yearid,(temp==NULL?"":temp));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID FISCAL YEAR!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr("The FISCAL YEAR of [%d] entered does not exist in the [RLSTMGT][RESTYEAR] data file for [%s].",yearid,(temp==NULL ? "":temp));
				if(temp!=NULL) Rfree(temp); 
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(rstyr,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(temp)+150);
					sprintf(warnmessage,"The FISCAL YEAR of [%d] entered is deleted.  It exists as a Real Estate Year for [%s], but is presently set up in a deleted state.",yearid,(temp==NULL ? "":temp));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED FISCAL YEAR!",warnmessage,NULL,NULL,TRUE,NULL);
					prterr("The FISCAL YEAR of [%d] entered is deleted.  It exist as a Real Estate Year for [%s], but is presently set up in a deleted state.",yearid,(temp==NULL?"":temp));
					if(temp!=NULL) Rfree(temp); 
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				}
			}
		}
	}
	if(temp!=NULL) Rfree(temp);
	if(ownid!=NULL) Rfree(ownid);
	save_record(mtnrsrc,previous,update_list);
}
static void save_record(RDArsrc *mtnrsrc,RDATData *previous,
	short update_list)
{
	char *rcddesc=NULL,*realestateid1=NULL,*realestateid2=NULL;
	char *realestateid3=NULL;
	char *ownid1=NULL,*ownid2=NULL,*ownid3=NULL;
	char deleteflag=0,vdeleteflag=0;
	char tdeleteflag=0,dist=FALSE;
	short ryrnum=0,byrnum=0,trannum=0,paidnum=0,taxdwr=0;
	short ef_paid=0,ef_tran=0,deftype=0,p_month=0,type=0;
	double amount=0.0;
	int tran1=0,tran2=0,yearid1=0,yearid2=0,yearid3=0;
	RDATData *prev=NULL;
	RDArsrc *tmprsrc=NULL;

	if(ADVRECORDsec(filenum,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(filenum,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE); 
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(filenum,0,NULL,previous);
		FINDFLDGETCHAR(filenum,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			taxdwr=OPNNRDsec("TAXCLT","TAXDRWR",TRUE,TRUE,FALSE);
			trannum=OPNNRDsec(module,"RESTRAN",TRUE,TRUE,FALSE);
			paidnum=OPNNRDsec(module,"RESTPAID",TRUE,TRUE,FALSE);
			ryrnum=OPNNRDsec("FINMGT","FINRYR",TRUE,TRUE,FALSE);
			byrnum=OPNNRDsec("FINMGT","FINBYR",TRUE,TRUE,FALSE);
			tmprsrc=RDArsrcNEW(module,NULL);
			addDFincvir(tmprsrc,"TAXCLT","TAXDRWR",NULL,taxdwr);
			addDFincvir(tmprsrc,module,"RESTOYR",NULL,filenum);
			addDFincvir(tmprsrc,module,"RESTMST",NULL,rstmst);
			addDFincvir(tmprsrc,module,"RESTOWN",NULL,rstown);
			addDFincvir(tmprsrc,module,"RESTRAN",NULL,trannum);
			addDFincvir(tmprsrc,module,"RESTPAID",NULL,paidnum);
			addDFincvir(tmprsrc,"FINMGT","FINRYR",NULL,ryrnum);
			addDFincvir(tmprsrc,"FINMGT","FINBYR",NULL,byrnum);
			GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
			FINDFLDGETSTRING(filenum,"REAL ESTATE IDENTIFICATION",
				&realestateid1);
			FINDFLDGETSTRING(filenum,"OWNER IDENTIFICATION",&ownid1);
			FINDFLDGETINT(filenum,"FISCAL YEAR",&yearid1);
			if(trannum!=(-1))
			{
				ZERNRD(trannum);
				FINDFLDSETSTRING(trannum,"REAL ESTATE IDENTIFICATION",realestateid1);
				FINDFLDSETSTRING(trannum,"OWNER IDENTIFICATION",ownid1);
				FINDFLDSETINT(trannum,"FISCAL YEAR",yearid1);
				ef_tran=ADVGTENRDsec(trannum,1,SCRNvirtualSubData,tmprsrc);
				while(!ef_tran)
				{
					FINDFLDGETCHAR(trannum,"DELETEFLAG",&tdeleteflag);
					if(!tdeleteflag)
					{
						FINDFLDGETSTRING(trannum,"REAL ESTATE IDENTIFICATION",&realestateid2);
						FINDFLDGETSTRING(trannum,"OWNER IDENTIFICATION",&ownid2);
						FINDFLDGETINT(trannum,"FISCAL YEAR",&yearid2);
						if(RDAstrcmp(realestateid1,realestateid2) || RDAstrcmp(ownid1,ownid2) || yearid1!=yearid2) break;
						FINDFLDGETINT(trannum,"TRANSACTION NUMBER",&tran1);
						FINDFLDGETCHAR(trannum,"UPDATED",&vdeleteflag);
						if(!vdeleteflag)
						{
							FINDFLDGETDOUBLE(trannum,"AMOUNT",&amount);
							ADVUpdateRealEstateTransactions(rstmst,rstyr,rstown,filenum,trannum,-amount,SCRNvirtualSubData,tmprsrc);
						}
						prev=RDATDataNEW(trannum);
						FINDFLDSETCHAR(trannum,"DELETEFLAG",deleteflag);
						ADVWRTTRANSsec(trannum,0,NULL,prev,SCRNvirtualSubData,tmprsrc);		
						if(prev!=NULL) FreeRDATData(prev);
						if(paidnum!=(-1) && ryrnum!=(-1) && byrnum!=(-1))
						{
							ZERNRD(paidnum);
							FINDFLDSETSTRING(paidnum,"REAL ESTATE IDENTIFICATION",realestateid1);
							FINDFLDSETSTRING(paidnum,"OWNER IDENTIFICATION",ownid1);
							FINDFLDSETINT(paidnum,"FISCAL YEAR",yearid1);
							FINDFLDSETINT(paidnum,"TRANSACTION NUMBER",tran1);
							ef_paid=ADVGTENRDsec(paidnum,1,SCRNvirtualSubData,tmprsrc);
							while(!ef_paid)
							{
								FINDFLDGETCHAR(paidnum,"DELETEFLAG",&vdeleteflag);
								if(!vdeleteflag)
								{
									FINDFLDGETSTRING(paidnum,"REAL ESTATE IDENTIFICATION",&realestateid3);
									FINDFLDGETSTRING(paidnum,"OWNER IDENTIFICATION",&ownid3);
									FINDFLDGETINT(paidnum,"FISCAL YEAR",&yearid3);
									FINDFLDGETINT(paidnum,"TRANSACTION NUMBER",&tran2);
									if(RDAstrcmp(realestateid3,realestateid1) || RDAstrcmp(ownid1,ownid3) || yearid1!=yearid3 || tran1!=tran2) break;
									prev=RDATDataNEW(paidnum);
									FINDFLDGETCHAR(paidnum,"DISTRIBUTED",&dist);
									if(dist==FALSE)
									{
										FINDFLDGETSHORT(paidnum,"TRANSACTION TYPE",&type);
										FINDFLDGETSHORT(paidnum,"DEFINITION TYPE",&deftype);
				    					FINDFLDGETSHORT(paidnum,"PROCESSING MONTH",&p_month);
				    					FINDFLDGETDOUBLE(paidnum,"AMOUNT",&amount);
										if(!type)
										{
											ADVupdateundist(trannum,NULL,"REVENUE CODE","CASH CODE",NULL,ryrnum,byrnum,yearid1,deftype,"REVENUE",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,tmprsrc);
										} else {
											ADVupdateundist(trannum,"ACCOUNT CODE","REVENUE CODE","CASH CODE",NULL,ryrnum,byrnum,yearid1,deftype,"REVENUE",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,tmprsrc);
										}
									}
									ADVUpdateRealEstatePayments(taxdwr,rstmst,rstyr,rstown,filenum,trannum,paidnum,-amount,SCRNvirtualSubData,tmprsrc);
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
		if(realestateid1!=NULL) Rfree(realestateid1);
		if(realestateid2!=NULL) Rfree(realestateid2);
		if(realestateid3!=NULL) Rfree(realestateid3);
		if(ownid1!=NULL) Rfree(ownid1);
		if(ownid2!=NULL) Rfree(ownid2);
		if(ownid3!=NULL) Rfree(ownid3);
		updatebrowse(update_list,mbl_restoyr,filenum,module,mtnrsrc);
		if(mbl_restoyr!=NULL) quit_record(mtnrsrc,previous);
			else GetRDATData(filenum,previous);
	}
}
static void quit_record(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(previous!=NULL) FreeRDATData(previous);
	if(WindowCount==1)
	{
		if(filenum!=(-1)) CLSNRD(filenum);
		if(rstmst!=(-1)) CLSNRD(rstmst);
		if(rstown!=(-1)) CLSNRD(rstown);
		if(rstyr!=(-1)) CLSNRD(rstyr);
		if(taxmst!=(-1)) CLSNRD(taxmst);
		if(fsclyr!=(-1)) CLSNRD(fsclyr);
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
static void getrecord(RDArsrc *mtnrsrc,RDATData *previous)
{
	char *realestateid1=NULL,*ownerid=NULL;
	int fiscalyear=0;

	readwidget(mtnrsrc,"[RESTOYR][REAL ESTATE IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[RESTOYR][REAL ESTATE IDENTIFICATION]",&realestateid1);
	FINDFLDSETSTRING(filenum,"REAL ESTATE IDENTIFICATION",realestateid1);
	readwidget(mtnrsrc,"[RESTOYR][OWNER IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[RESTOYR][OWNER IDENTIFICATION]",&ownerid);
	FINDFLDSETSTRING(filenum,"OWNER IDENTIFICATION",ownerid);
	readwidget(mtnrsrc,"[RESTOYR][FISCAL YEAR]");
	FINDRSCGETINT(mtnrsrc,"[RESTOYR][FISCAL YEAR]",&fiscalyear);
	FINDFLDSETINT(filenum,"FISCAL YEAR",fiscalyear);
	if(ADVEQLNRDsec(filenum,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc)) 
	{
		KEYNRD(filenum,MASTER->passkey->keyno);
		ClearRDATData(previous);
	} else {
		GetRDATData(filenum,previous);
		filerecord2rsrc(filenum,mtnrsrc);	
	}
	if(realestateid1!=NULL) Rfree(realestateid1);
	if(ownerid!=NULL) Rfree(ownerid);
	getsupportingfiles(mtnrsrc);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void getsupportingfiles(RDArsrc *mtnrsrc)
{
	if(rstmst!=(-1))
	{
		COPYFIELD(filenum,rstmst,"REAL ESTATE IDENTIFICATION");
		if(ADVEQLNRDsec(rstmst,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(rstmst,1);
		}
		filerecord2rsrc(rstmst,mtnrsrc);
	}
	if(rstown!=(-1))
	{
		COPYFIELD(filenum,rstown,"REAL ESTATE IDENTIFICATION");
		COPYFIELD(filenum,rstown,"OWNER IDENTIFICATION");
		if(ADVEQLNRDsec(rstown,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(rstown,1);
		}
		filerecord2rsrc(rstown,mtnrsrc);
	}
	if(rstyr!=(-1))
	{
		COPYFIELD(filenum,rstyr,"REAL ESTATE IDENTIFICATION");
		COPYFIELD(filenum,rstyr,"FISCAL YEAR");
		if(ADVEQLNRDsec(rstyr,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(rstyr,1);
		}
		filerecord2rsrc(rstyr,mtnrsrc);
	}
	if(taxmst!=(-1))
	{
		COPYFIELD(filenum,taxmst,"OWNER IDENTIFICATION");
		if(ADVEQLNRDsec(taxmst,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(taxmst,1);
		}
		filerecord2rsrc(taxmst,mtnrsrc);
	}
	if(fsclyr!=(-1))
	{
		COPYFIELD(filenum,fsclyr,"FISCAL YEAR");
		if(ADVEQLNRDsec(fsclyr,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(fsclyr,1);
		}
		filerecord2rsrc(fsclyr,mtnrsrc);
	}
}
