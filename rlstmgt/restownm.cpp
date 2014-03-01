/* restownm.c - Real Estate Owner Master Maintain Screen */
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

struct mtnstructs
{
	APPlib *owntypes;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern short filenum,rstmst,taxmst;
extern char editable;
extern char *module;
extern void browse_msr(RDArsrc *,mtnstruct *);
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_check(RDArsrc *,mtnstruct *,short);
static void save_record_quit(RDArsrc *,mtnstruct *);
static void readscreen(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void quit_record_test(RDArsrc *,mtnstruct *);
static void quit_record(RDArsrc *,mtnstruct *);
static void getrecord(RDArsrc *,mtnstruct *);
MakeBrowseList *mbl_restown=NULL;
extern MaintainMaster *MASTER;
void restownm(short,void *targetkey);
static void getsupportingfiles(RDArsrc *,mtnstruct *);

static void save_record_quit(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,TRUE);
}
static void quit_record_test(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *desc=NULL;

	readscreen(mtnrsrc,mtn);
	if(QUITvfy(filenum,mtn->previous))
	{
		desc=QUITvfydesc(filenum,mtn->previous);
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!",
			desc,save_record_quit,quit_record,FALSE,2,mtnrsrc,mtn,NULL);
		if(desc!=NULL) Rfree(desc);
	} else {
		quit_record(mtnrsrc,mtn);
	}
}
void restownm(short dowhich,void *targetkey)
{
	RDArsrc *mtnrsrc=NULL;
	int nofields=0,x,which_type=0;
	NRDfield *fields=NULL,*fldx;
	short edit_rsrc=FALSE;
	mtnstruct *mtn=NULL;
	
	mtnrsrc=RDArsrcNEW(module,MASTER->maintain_screen);
	addDFincvir(mtnrsrc,module,"RESTOWN",NULL,filenum);
	addDFincvir(mtnrsrc,module,"RESTMST",NULL,rstmst);
	addDFincvir(mtnrsrc,"TAXCLT","TAXMST",NULL,taxmst);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->owntypes=APPlibNEW();
	addAPPlib(mtn->owntypes,"Percentage");
	addAPPlib(mtn->owntypes,"Flat Amount");
	ZERNRD(filenum);
	mtn->previous=RDATDataNEW(filenum);
	nofields=NUMFLDS(filenum);
	fields=FLDPOINTER(filenum);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=(short)editable;
			if(!RDAstrcmp(fldx->name,"OWNERSHIP TYPE"))
			{
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					addlstrsrc(mtnrsrc,"OWNERSHIP TYPES",
						&which_type,(edit_rsrc ? TRUE:FALSE),
						NULL,mtn->owntypes->numlibs,
						&mtn->owntypes->libs,NULL);
				}
			} else if(!strncmp(fldx->name,"TAX AGED BALANCE",16) ||
				!strncmp(fldx->name,"PENALTY AGED BALANCE",20) ||
				!strncmp(fldx->name,"INTEREST AGED BALANCE",21) ||
				!strncmp(fldx->name,"UNPROCESSED",11))
			{
				nonlstmakefld(mtnrsrc,filenum,fldx,FALSE);
			} else {
				nonlstmakefld(mtnrsrc,filenum,fldx,edit_rsrc);
			}
		}
	}
	nofields=0;
	file2rsrc(rstmst,mtnrsrc,FALSE);
	file2rsrc(taxmst,mtnrsrc,FALSE);
	FINDRSCSETFUNC(mtnrsrc,"[RESTOWN][REAL ESTATE IDENTIFICATION]",
		getrecord,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[RESTOWN][OWNER IDENTIFICATION]",getrecord,mtn);
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
				GetRDATData(filenum,mtn->previous);
				filerecord2rsrc(filenum,mtnrsrc);
				FINDFLDGETINT(filenum,"OWNERSHIP TYPE",&x);
				FINDRSCSETINT(mtnrsrc,"OWNERSHIP TYPES",x);
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
				GetRDATData(filenum,mtn->previous);
				filerecord2rsrc(filenum,mtnrsrc);
				FINDFLDGETINT(filenum,"OWNERSHIP TYPE",&x);
				FINDRSCSETINT(mtnrsrc,"OWNERSHIP TYPES",x);
			}
			break;
	}
	getsupportingfiles(mtnrsrc,mtn);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	if(editable)
	{
		addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
		if(mbl_restown!=NULL)
		{
			addrfcbrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
		} else {
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
		}
	}
	if(mbl_restown==NULL)
	{
		addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_msr,mtn);
	}
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakeMasterButtons(mtnrsrc,MASTER);
	computeallSCRNvirtuals(mtnrsrc);
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,
		(mbl_restown==NULL ? TRUE:FALSE));
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
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected=0;

	ZERNRD(filenum);
	readallwidgets(mtnrsrc);
	rsrc2filerecord(filenum,mtnrsrc);
	FINDRSCGETINT(mtnrsrc,"OWNERSHIP TYPES",&selected);
	FINDFLDSETINT(filenum,"OWNERSHIP TYPE",selected);
}
static void save_check(RDArsrc *mtnrsrc,mtnstruct *mtn,
	short update_list)
{
	char *temp=NULL,*warnmessage=NULL,delflag=FALSE;

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
				warnmessage=Rmalloc(RDAstrlen(temp)+100);
				sprintf(warnmessage,"The REAL ESTATE IDENTIFICATION of [%s] entered is not valid.  It does not exist as a Real Estate.",(temp==NULL?"":temp));
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
				warnmessage=Rmalloc(RDAstrlen(temp)+100);
				sprintf(warnmessage,"The OWNER IDENTIFICATION of [%s] entered is not valid.  It does not exist as an Owner.",(temp==NULL?"":temp));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID REAL ESTATE IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr("The REAL ESTATE IDENTIFICATION of [%s] entered does not exist in the [TAXCLT][TAXMST] data file.",(temp==NULL ? "":temp));
				if(temp!=NULL) Rfree(temp); 
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(taxmst,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(temp)+140);
					sprintf(warnmessage,"The REAL ESTATE IDENTIFICATION of [%s] entered is deleted.  It exists as an Owner, but is presently set up in a deleted state.",(temp==NULL ? "":temp));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED REAL ESTATE IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					prterr("The REAL ESTATE IDENTIFICATION of [%s] entered is deleted.  It exists as an Owner, but is presently set up in a deleted state.",(temp==NULL?"":temp));
					if(temp!=NULL) Rfree(temp); 
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				}
			}
		}
	}
	if(temp!=NULL) Rfree(temp);
	save_record(mtnrsrc,mtn,update_list);
}
static void save_record(RDArsrc *mtnrsrc,mtnstruct *mtn,
	short update_list)
{
	char *rcddesc=NULL,*realestateid1=NULL,*realestateid2=NULL;
	char *realestateid3=NULL,*realestateid4=NULL;
	char *ownid1=NULL,*ownid2=NULL,*ownid3=NULL,*ownid4=NULL;
	char deleteflag=0,pdeleteflag=0,vdeleteflag=0;
	char tdeleteflag=0,dist=FALSE;
	short yrnum=0,paidnum=0,ynum=0,taxdwr=0;
	short ryrnum=0,byrnum=0,trannum=0;
	short ef_yr=0,ef_paid=0,ef_tran=0;
	short deftype=0,p_month=0,type=0;
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
		WRTTRANS(filenum,0,NULL,mtn->previous);
		FINDFLDGETCHAR(filenum,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			taxdwr=OPNNRDsec("TAXCLT","TAXMST",TRUE,TRUE,FALSE);
			ynum=OPNNRDsec(module,"RESTYEAR",TRUE,TRUE,FALSE);
			yrnum=OPNNRDsec(module,"RESTOYR",TRUE,TRUE,FALSE);
			trannum=OPNNRDsec(module,"RESTRAN",TRUE,TRUE,FALSE);
			paidnum=OPNNRDsec(module,"RESTPAID",TRUE,TRUE,FALSE);
			ryrnum=OPNNRDsec("FINMGT","FINRYR",TRUE,TRUE,FALSE);
			byrnum=OPNNRDsec("FINMGT","FINBYR",TRUE,TRUE,FALSE);
			tmprsrc=RDArsrcNEW(module,NULL);
			addDFincvir(tmprsrc,"TAXCLT","TAXMST",NULL,taxdwr);
			addDFincvir(tmprsrc,module,"RESTOWN",NULL,filenum);
			addDFincvir(tmprsrc,module,"RESTMST",NULL,rstmst);
			addDFincvir(tmprsrc,module,"RESTYEAR",NULL,ynum);
			addDFincvir(tmprsrc,module,"RESTOYR",NULL,yrnum);
			addDFincvir(tmprsrc,module,"RESTRAN",NULL,trannum);
			addDFincvir(tmprsrc,module,"RESTPAID",NULL,paidnum);
			addDFincvir(tmprsrc,"FINMGT","FINRYR",NULL,ryrnum);
			addDFincvir(tmprsrc,"FINMGT","FINBYR",NULL,byrnum);
			GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
			FINDFLDGETSTRING(filenum,"REAL ESTATE IDENTIFICATION",
				&realestateid1);
			FINDFLDGETSTRING(filenum,"OWNER IDENTIFICATION",&ownid1);
			if(yrnum!=(-1))
			{
				ZERNRD(yrnum);
				FINDFLDSETSTRING(yrnum,"REAL ESTATE IDENTIFICATION",
					realestateid1);
				FINDFLDSETSTRING(yrnum,"OWNER IDENTIFICATION",ownid1);
				ef_yr=ADVGTENRDsec(yrnum,1,SCRNvirtualSubData,tmprsrc);
				while(!ef_yr)
				{
					FINDFLDGETCHAR(yrnum,"DELETEFLAG",&pdeleteflag);
					if(!pdeleteflag)
					{
						FINDFLDGETSTRING(yrnum,"REAL ESTATE IDENTIFICATION",
							&realestateid2);
						FINDFLDGETSTRING(yrnum,"OWNER IDENTIFICATION",
							&ownid2);
						if(RDAstrcmp(realestateid2,realestateid1) || RDAstrcmp(ownid1,ownid2)) break;
						FINDFLDGETINT(yrnum,"FISCAL YEAR",&yearid1);
						prev=RDATDataNEW(yrnum);
						FINDFLDSETCHAR(yrnum,"DELETEFLAG",deleteflag);
						ADVWRTTRANSsec(yrnum,0,NULL,prev,SCRNvirtualSubData,tmprsrc);		
						if(prev!=NULL) FreeRDATData(prev);
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
									FINDFLDGETSTRING(trannum,"REAL ESTATE IDENTIFICATION",&realestateid3);
									FINDFLDGETSTRING(trannum,"OWNER IDENTIFICATION",&ownid3);
									FINDFLDGETINT(trannum,"FISCAL YEAR",&yearid2);
									if(RDAstrcmp(realestateid1,realestateid3) || RDAstrcmp(ownid1,ownid3) || yearid1!=yearid2) break;
									FINDFLDGETINT(trannum,"TRANSACTION NUMBER",&tran1);
									FINDFLDGETCHAR(trannum,"UPDATED",&vdeleteflag);
									if(!vdeleteflag)
									{
										FINDFLDGETDOUBLE(trannum,"AMOUNT",&amount);
										ADVUpdateRealEstateTransactions(rstmst,ynum,filenum,yrnum,trannum,-amount,SCRNvirtualSubData,tmprsrc);
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
												FINDFLDGETSTRING(paidnum,"REAL ESTATE IDENTIFICATION",&realestateid4);
												FINDFLDGETSTRING(paidnum,"OWNER IDENTIFICATION",&ownid4);
												FINDFLDGETINT(paidnum,"FISCAL YEAR",&yearid3);
												FINDFLDGETINT(paidnum,"TRANSACTION NUMBER",&tran2);
												if(RDAstrcmp(realestateid4,realestateid1) || RDAstrcmp(ownid1,ownid4) || yearid1!=yearid3 || tran1!=tran2) break;
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
												ADVUpdateRealEstatePayments(taxdwr,rstmst,ynum,filenum,yrnum,trannum,paidnum,-amount,SCRNvirtualSubData,tmprsrc);
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
					}
					ef_yr=ADVNXTNRDsec(yrnum,1,SCRNvirtualSubData,tmprsrc);
				}
			}
			if(tmprsrc!=NULL) Rfree(tmprsrc);
			if(taxdwr!=(-1)) CLSNRD(taxdwr);
			if(ynum!=(-1)) CLSNRD(ynum);
			if(yrnum!=(-1)) CLSNRD(yrnum);
			if(trannum!=(-1)) CLSNRD(trannum);
			if(paidnum!=(-1)) CLSNRD(paidnum);
			if(ryrnum!=(-1)) CLSNRD(ryrnum);
			if(byrnum!=(-1)) CLSNRD(byrnum);
		}
		if(realestateid1!=NULL) Rfree(realestateid1);
		if(realestateid2!=NULL) Rfree(realestateid2);
		if(realestateid3!=NULL) Rfree(realestateid3);
		if(realestateid4!=NULL) Rfree(realestateid4);
		if(ownid1!=NULL) Rfree(ownid1);
		if(ownid2!=NULL) Rfree(ownid2);
		if(ownid3!=NULL) Rfree(ownid3);
		if(ownid4!=NULL) Rfree(ownid4);
		updatebrowse(update_list,mbl_restown,filenum,module,mtnrsrc);
		if(mbl_restown!=NULL) quit_record(mtnrsrc,mtn);
			else GetRDATData(filenum,mtn->previous);
	}
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		if(mtn->owntypes!=NULL) freeapplib(mtn->owntypes);
		Rfree(mtn);
	}
	if(WindowCount==1)
	{
		if(filenum!=(-1)) CLSNRD(filenum);
		if(rstmst!=(-1)) CLSNRD(rstmst);
		if(taxmst!=(-1)) CLSNRD(taxmst);
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
static void getrecord(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *realestateid1=NULL,*ownerid=NULL;
	int x=0;

	readwidget(mtnrsrc,"[RESTOWN][REAL ESTATE IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[RESTOWN][REAL ESTATE IDENTIFICATION]",&realestateid1);
	FINDFLDSETSTRING(filenum,"REAL ESTATE IDENTIFICATION",realestateid1);
	readwidget(mtnrsrc,"[RESTOWN][OWNER IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[RESTOWN][OWNER IDENTIFICATION]",&ownerid);
	FINDFLDSETSTRING(filenum,"OWNER IDENTIFICATION",ownerid);
	if(ADVEQLNRDsec(filenum,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc)) 
	{
		KEYNRD(filenum,MASTER->passkey->keyno);
		ClearRDATData(mtn->previous);
	} else {
		GetRDATData(filenum,mtn->previous);
		filerecord2rsrc(filenum,mtnrsrc);	
		FINDFLDGETINT(filenum,"OWNERSHIP TYPE",&x);
		FINDRSCSETINT(mtnrsrc,"OWNERSHIP TYPES",x);
	}
	if(realestateid1!=NULL) Rfree(realestateid1);
	if(ownerid!=NULL) Rfree(ownerid);
	getsupportingfiles(mtnrsrc,mtn);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void getsupportingfiles(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(rstmst!=(-1))
	{
		COPYFIELD(filenum,rstmst,"REAL ESTATE IDENTIFICATION");
		if(ADVEQLNRDsec(rstmst,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(rstmst,1);
		filerecord2rsrc(rstmst,mtnrsrc);
	}
	if(taxmst!=(-1))
	{
		COPYFIELD(filenum,taxmst,"OWNER IDENTIFICATION");
		if(ADVEQLNRDsec(taxmst,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(taxmst,1);
		filerecord2rsrc(taxmst,mtnrsrc);
	}
}
