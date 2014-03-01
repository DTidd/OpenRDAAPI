/* subpaidm.c - Xpert Substitute Paid Master Maintain Screen */
#include <cstdio>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <rlstmgt.h>
#include <fin.hpp>

struct mtnstructs
{
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;
extern short filenum,mstnum,subwknum,dmgnum,paymstr;
extern short paymstr,payppms;
static void getrecordsetedit(RDArsrc *,mtnstruct *);
static void seteditable(RDArsrc *,mtnstruct *);
static void save_record(RDArsrc *,mtnstruct *,short);
static void quit_record(RDArsrc *,mtnstruct *);
static void quit_record_test(RDArsrc *,mtnstruct *);
static void save_record_quit(RDArsrc *,mtnstruct *);
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void getrecordcb(RDArsrc *,mtnstruct *mtn);
static void getsupportingrecords(RDArsrc *,char);
static void save_check(RDArsrc *,mtnstruct *,short);
static short check_payid(char *,char *,char *);
static void payidcb(RDArsrc *);
extern char *module;
extern char editable;
extern MakeBrowseList *mbl_subpaid;
extern void browse_subpaid(RDArsrc *,mtnstruct *);
void subpaidm(short,void *);
extern MaintainMaster *MTNMASTER;

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
void subpaidm(short dowhich,void *targetkey)
{
	int x=0;
	char *date=NULL,*timex=NULL;
	short nofields=0,edit_rsrc=FALSE;
	NRDfield *fields=NULL,*fldx=NULL;
	RDArsrc *mtnrsrc=NULL;
	mtnstruct *mtn=NULL;
	
	mtn=Rmalloc(sizeof(mtnstruct));
	ZERNRD(filenum);
	mtn->previous=RDATDataNEW(filenum);
	mtnrsrc=RDArsrcNEW(module,MTNMASTER->maintain_screen);
	nofields=NUMFLDS(filenum);
	fields=FLDPOINTER(filenum);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=(short)editable;
			nonlstmakefld(mtnrsrc,filenum,fldx,edit_rsrc);
		}
	}
	nofields=0;
	FINDRSCSETFUNC(mtnrsrc,"[SUBPAID][PERSONNEL IDENTIFICATION]",
		getrecordcb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[SUBPAID][TRANSACTION NUMBER]",getrecordcb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[SUBPAID][PAYMENT NUMBER]",getrecordcb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[SUBPAID][PAY IDENTIFICATION]",payidcb,NULL);
	if(mstnum!=(-1)) file2rsrc(mstnum,mtnrsrc,FALSE);
	if(subwknum!=(-1)) file2rsrc(subwknum,mtnrsrc,FALSE);
	if(dmgnum!=(-1)) file2rsrc(dmgnum,mtnrsrc,FALSE);
	if(paymstr!=(-1)) file2rsrc(paymstr,mtnrsrc,FALSE);
	if(payppms!=(-1)) file2rsrc(payppms,mtnrsrc,FALSE);
	addDFincvir(mtnrsrc,module,"SUBPAID",NULL,filenum);
	addDFincvir(mtnrsrc,module,"SUBMASTR",NULL,mstnum);
	addDFincvir(mtnrsrc,module,"SUBWKMAS",NULL,subwknum);
	addDFincvir(mtnrsrc,"PRSNNL","PERDMG",NULL,dmgnum);
	addDFincvir(mtnrsrc,"PAYROLL","PAYMSTR",NULL,paymstr);
	addDFincvir(mtnrsrc,"PAYROLL","PAYPPMS",NULL,payppms);
	date=GETCURRENTDATE();
	timex=GETCURRENTTIME();
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			ApplyPassKey(MTNMASTER->passkey);
			if(EQLNRDsec(filenum,MTNMASTER->passkey->keyno))
			{
				KEYNRD(filenum,MTNMASTER->passkey->keyno);
				ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
				FINDRSCSETSTRING(mtnrsrc,"[SUBPAID][SOURCE USER]",USERLOGIN);
				FINDRSCSETSTRING(mtnrsrc,"[SUBPAID][ENTRY DATE]",date);
				FINDRSCSETSTRING(mtnrsrc,"[SUBPAID][ENTRY TIME]",timex);
			} else {
				GetRDATData(filenum,mtn->previous);
				getrecordsetedit(mtnrsrc,mtn);
			}
			break;
		case 2:
			ApplyPassKey(MTNMASTER->passkey);
			ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
			FINDRSCSETSTRING(mtnrsrc,"[SUBPAID][SOURCE USER]",USERLOGIN);
			FINDRSCSETSTRING(mtnrsrc,"[SUBPAID][ENTRY DATE]",date);
			FINDRSCSETSTRING(mtnrsrc,"[SUBPAID][ENTRY TIME]",timex);
			break;
		case 0:
		default:
			if(EQLNRDKEYsec(filenum,MTNMASTER->passkey->keyno,targetkey))
			{
				ZERNRD(filenum);
				ApplyPassKey(MTNMASTER->passkey);
				ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
				FINDRSCSETSTRING(mtnrsrc,"[SUBPAID][SOURCE USER]",USERLOGIN);
				FINDRSCSETSTRING(mtnrsrc,"[SUBPAID][ENTRY DATE]",date);
				FINDRSCSETSTRING(mtnrsrc,"[SUBPAID][ENTRY TIME]",timex);
			} else {
				GetRDATData(filenum,mtn->previous);
				getrecordsetedit(mtnrsrc,mtn);
			}
			break;
	}
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
	getsupportingrecords(mtnrsrc,FALSE);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	MakeMasterButtons(mtnrsrc,MTNMASTER);
	if(editable)
	{
		addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
		if(mbl_subpaid!=NULL)
		{
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
		} else {
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
		}
	}
	if(mbl_subpaid==NULL)
	{
		addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_subpaid,mtn);
	}
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mtnrsrc);
	MakeMasterButtons(mtnrsrc,MTNMASTER);
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_subpaid==NULL?TRUE:FALSE));
}
static void getrecordsetedit(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	filerecord2rsrc(filenum,mtnrsrc);
	seteditable(mtnrsrc,mtn);
}
static void seteditable(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char suv=FALSE;

	FINDRSCGETCHAR(mtnrsrc,"[SUBPAID][SOURCE USER VERIFIED]",&suv);
	if(suv)
	{
		updateeditfilersrcsnokeys(filenum,MTNMASTER->passkey->keyno,mtnrsrc,FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"[SUBPAID][DELETEFLAG]",TRUE);
	} else {
		updateeditfilersrcsnokeys(filenum,MTNMASTER->passkey->keyno,mtnrsrc,TRUE);
	}
}
static void save_record_up(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,FALSE);
}
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(filenum);
	readallwidgets(mtnrsrc);
	rsrc2filerecord(filenum,mtnrsrc);
}
static void payidcb(RDArsrc *mtnrsrc)
{
	char *payid=NULL,*warnmessage=NULL,*header=NULL;

	readwidget(mtnrsrc,"[SUBPAID][PAY IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[SUBPAID][PAY IDENTIFICATION]",&payid);
	warnmessage=Rmalloc(500);
	header=Rmalloc(60);
	if(check_payid(warnmessage,header,payid))
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN",header,warnmessage,NULL,NULL,TRUE,NULL);
	}
	if(warnmessage!=NULL) Rfree(warnmessage);
	if(header!=NULL) Rfree(header);
	if(payid!=NULL) Rfree(payid);
	if(EQLNRDsec(paymstr,1)) KEYNRD(paymstr,1);
	updatefilerecord2rsrc(paymstr,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
static short check_payid(char *warnmessage,char *header,char *payid1)
{
	char delflag=FALSE;

	FINDFLDSETSTRING(paymstr,"PAY IDENTIFICATION",payid1);
	if(EQLNRDsec(paymstr,1))
	{
		sprintf(warnmessage,"The Pay Identification of [%s] is an invalid value.  It does not exist in the [PAYROLL][PAYMSTR] file.  Please select a valid Pay Identification.",(payid1==NULL?"":payid1));
		sprintf(header,"INVALID PAY IDENTIFICATION!");
		return(TRUE);
	} else {
		FINDFLDGETCHAR(paymstr,"DELETEFLAG",&delflag);
		if(delflag)
		{
			sprintf(warnmessage,"The Pay Identification of [%s] is a deleted value. It exists for the [PAYROLL][PAYMSTR] file but is in a deleted state.  Please select a valid Pay Identification from this file.",(payid1==NULL?"":payid1));
			sprintf(header,"DELETED PAY IDENTIFICATION!");
			return(TRUE);
		}
	}
	return(FALSE);
}
static void save_check(RDArsrc *mtnrsrc,mtnstruct *mtn,
	short update_list)
{
	char *temp=NULL,*warnmessage=NULL,delflag=FALSE;
	int transno=0;

	readscreen(mtnrsrc,mtn);
	FINDFLDGETCHAR(filenum,"DELETEFLAG",&delflag);
	if(!delflag)
	{
		if(dmgnum!=(-1))
		{
			FINDFLDGETSTRING(filenum,"PERSONNEL IDENTIFICATION",&temp);
			FINDFLDSETSTRING(dmgnum,"PERSONNEL IDENTIFICATION",temp);
			if(EQLNRDsec(dmgnum,1))
			{
				KEYNRD(dmgnum,1);
				warnmessage=Rmalloc(RDAstrlen(temp)+180);
				sprintf(warnmessage,"The PERSONNEL IDENTIFICATION of [%s] entered is not valid.  It does not exist in the [PRSNNL][PERDMG] data file.",(temp==NULL?"":temp));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr("The PERSONNEL IDENTIFICATION of [%s] entered does not exist in the [PRSNNL][PERDMG] data file.",(temp==NULL ? "":temp));
				if(temp!=NULL) Rfree(temp); 
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(dmgnum,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(temp)+170);
					sprintf(warnmessage,"The PERSONNEL IDENTIFICATION of [%s] entered is deleted.  It exists in the [PRSNNL][PERDMG] data file but is presently set up in a deleted state.",(temp==NULL ? "":temp));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					prterr("The PERSONNEL IDENTIFICATION of [%s] entered is deleted.  It exist in the [PRSNNL][PERDMG] but is presently set up in a deleted state.",(temp==NULL?"":temp));
					if(temp!=NULL) Rfree(temp); 
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				} else {
					FINDFLDGETCHAR(dmgnum,"CURRENT EMPLOYEE",&delflag);
					if(!delflag)
					{
						warnmessage=Rmalloc(RDAstrlen(temp)+180);
						sprintf(warnmessage,"The PERSONNEL IDENTIFICATION of [%s] entered is not a current employee.  It exists in the [PRSNNL][PERDMG] data file but is presently set up as a non-currnent employee.",(temp==NULL?"":temp));
						WARNINGDIALOG("WARNING DIALOG SCREEN","NON-CURRENT EMPLOYEE!",warnmessage,NULL,NULL,TRUE,NULL);
						prterr("The PERSONNEL IDENTIFICATION of [%s] entered is deleted.  It exist in the [PRSNNL][PERDMG] data file but is presently set up in a non-current employee state.",(temp==NULL?"":temp));
						if(temp!=NULL) Rfree(temp); 
						if(warnmessage!=NULL) Rfree(warnmessage);
						return;
					}
				}
			}
		}
		if(mstnum!=(-1))
		{
			FINDFLDGETSTRING(filenum,"PERSONNEL IDENTIFICATION",&temp);
			FINDFLDSETSTRING(mstnum,"PERSONNEL IDENTIFICATION",temp);
			if(EQLNRDsec(mstnum,1))
			{
				KEYNRD(mstnum,1);
				warnmessage=Rmalloc(RDAstrlen(temp)+140);
				sprintf(warnmessage,"The PERSONNEL IDENTIFICATION of [%s] entered is not valid.  It does not exist as a Substite Master.",(temp==NULL?"":temp));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr("The PERSONNEL IDENTIFICATION of [%s] entered does not exist in the [SUBMGT][SUBMASTR] data file.",(temp==NULL ? "":temp));
				if(temp!=NULL) Rfree(temp); 
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(mstnum,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(temp)+140);
					sprintf(warnmessage,"The PERSONNEL IDENTIFICATION of [%s] entered is deleted.  It exists as a Substitue, but is presently set up in a deleted state.",(temp==NULL ? "":temp));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					prterr("The PERSONNEL IDENTIFICATION of [%s] entered is deleted.  It exist as a Substitue, but is presently set up in a deleted state.",(temp==NULL?"":temp));
					if(temp!=NULL) Rfree(temp); 
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				} else {
					FINDFLDGETCHAR(mstnum,"ACTIVE",&delflag);
					if(!delflag)
					{
						warnmessage=Rmalloc(RDAstrlen(temp)+150);
						sprintf(warnmessage,"The PERSONNEL IDENTIFICATION of [%s] entered is not active.  It exists as a Substitue, but is presently set up in a non-active state.",(temp==NULL ? "":temp));
						WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
						prterr("The PERSONNEL IDENTIFICATION of [%s] entered is deleted.  It exist as a Substitue, but is presently set up in a non-active state.",(temp==NULL?"":temp));
						if(temp!=NULL) Rfree(temp); 
						if(warnmessage!=NULL) Rfree(warnmessage);
						return;
					}
				}
			}
		}
		if(subwknum!=(-1))
		{
			FINDFLDGETSTRING(filenum,"PERSONNEL IDENTIFICATION",&temp);
			FINDFLDGETINT(filenum,"TRANSACTION NUMBER",&transno);
			FINDFLDSETSTRING(subwknum,"REAL ESTATE IDENTIFICATION",temp);
			FINDFLDSETINT(subwknum,"TRANSACTION NUJMBER",transno);
			if(EQLNRDsec(subwknum,1))
			{
				KEYNRD(subwknum,1);
				warnmessage=Rmalloc(RDAstrlen(temp)+166);
				sprintf(warnmessage,"The TRANSACTION NUMBER of [%6d] entered is not valid.  It does not exist as an Substitue Work Transaction of the [%s] Substitue.",transno,(temp==NULL?"":temp));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID TRANSACTION NUMBER!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr("The TRANSACTION NUMBER of [%6d] entered does not exist in the [SUBMGT][SUBWKMAS] data file.",transno);
				if(temp!=NULL) Rfree(temp); 
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(subwknum,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(temp)+166);
					sprintf(warnmessage,"The TRANSACTION NUMBER of [%6d] entered is deleted.  It exists as for the Substitute of [%s], but is presently set up in a deleted state.",transno,(temp==NULL ? "":temp));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED TRANSACTION NUMBER!",warnmessage,NULL,NULL,TRUE,NULL);
					prterr("The TRANSACTION NUMBER of [%6d] entered is deleted.  It exists as for the Substitute for [%s], but is presently set up in a deleted state.",transno,(temp==NULL?"":temp));
					if(temp!=NULL) Rfree(temp); 
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				}
			}
		}
		if(paymstr!=(-1))
		{
			FINDFLDGETSTRING(filenum,"PAY IDENTIFICATION",&temp);
			FINDFLDSETSTRING(paymstr,"PAY IDENTIFICATION",temp);
			if(EQLNRDsec(paymstr,1))
			{
				KEYNRD(paymstr,1);
				warnmessage=Rmalloc(RDAstrlen(temp)+180);
				sprintf(warnmessage,"The PAY IDENTIFICATION of [%s] entered is not valid.  It does not exist in the [PAYROLL][PAYMSTR] data file.",(temp==NULL?"":temp));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PAY IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr("The PAY IDENTIFICATION of [%s] entered does not exist in the [PAYROLL][PAYMSTR] data file.",(temp==NULL ? "":temp));
				if(temp!=NULL) Rfree(temp); 
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(paymstr,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(temp)+180);
					sprintf(warnmessage,"The PAY IDENTIFICATION of [%s] entered is deleted.  It exists in the [PAYROLL][PAYMSTR] data file but is presently set up in a deleted state.",(temp==NULL ? "":temp));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED PAY IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					prterr("The PAY IDENTIFICATION of [%s] entered is deleted.  It exist in the [PAYROLL][PAYMSTR] data file but is presently set up in a deleted state.",(temp==NULL?"":temp));
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
	char deleteflag=0,*rcddesc=NULL;
	short numflds=0,y=0;
	double initunits=0.0,units=0.0;
	NRDfield *tmpfld=NULL,*fldx=NULL,*tmp=NULL;
	RDATData *prev=NULL;

	if(RECORDsec(filenum))
	{
		rcddesc=RECORDsecDesc(filenum);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE); 
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		numflds=NUMFLDS(filenum);
		if(numflds>0)
		{
			tmpfld=Rmalloc(numflds*sizeof(NRDfield));
			for(y=0,fldx=tmpfld;y<numflds;++y,++fldx)
			{
				tmp=FLDNUM(filenum,y);
				cpfldinfo(fldx,tmp);
			}
			LOCNRDFILE(filenum);
			if(!EQLNRDsec(filenum,MTNMASTER->passkey->keyno))
			{
				FINDFLDGETCHAR(filenum,"DELETEFLAG",&deleteflag);
				if(!deleteflag)
				{
					FINDFLDGETDOUBLE(filenum,"UNITS",&units);
					COPYFIELD(filenum,subwknum,"PERSONNEL IDENTIFICATION");
					COPYFIELD(filenum,subwknum,"TRANSACTION NUMBER");
					LOCNRDFILE(subwknum);
					if(EQLNRDsec(subwknum,1))
					{
						KEYNRD(subwknum,1);
					}  else {
						prev=RDATDataNEW(subwknum);
						GetRDATData(subwknum,prev);
						FINDFLDGETDOUBLE(subwknum,"PAID UNITS",&initunits);
						initunits-=units;
						FINDFLDSETDOUBLE(subwknum,"PAID UNITS",initunits);
						WRTTRANS(subwknum,0,NULL,prev);
						if(prev!=NULL) FreeRDATData(prev);
					}
					UNLNRDFILE(subwknum);
				}
			}
			UNLNRDFILE(filenum);
			fields2file(filenum,tmpfld);
		}
		WRTTRANS(filenum,0,NULL,mtn->previous);
		FINDFLDGETCHAR(filenum,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDGETDOUBLE(filenum,"UNITS",&units);
			COPYFIELD(filenum,subwknum,"PERSONNEL IDENTIFICATION");
			COPYFIELD(filenum,subwknum,"TRANSACTION NUMBER");
			LOCNRDFILE(subwknum);
			if(EQLNRDsec(subwknum,1))
			{
				KEYNRD(subwknum,1);
			} else {
				prev=RDATDataNEW(subwknum);
				GetRDATData(subwknum,prev);
				FINDFLDGETDOUBLE(subwknum,"PAID UNITS",&initunits);
				initunits+=units;
				FINDFLDSETDOUBLE(subwknum,"PAID UNITS",initunits);
				WRTTRANS(subwknum,0,NULL,prev);
				if(prev!=NULL) FreeRDATData(prev);
			}
			UNLNRDFILE(subwknum);
		}
		updatebrowse(update_list,mbl_subpaid,filenum,module,mtnrsrc);
		if(mbl_subpaid!=NULL) 
		{
			quit_record(mtnrsrc,mtn);
		} else {
			seteditable(mtnrsrc,mtn);
			GetRDATData(filenum,mtn->previous);
		}
	}
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(WindowCount==1)
	{
		if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
		if(filenum!=(-1)) CLSNRD(filenum);
		if(mstnum!=(-1)) CLSNRD(mstnum);
		if(subwknum!=(-1)) CLSNRD(subwknum);
		if(dmgnum!=(-1)) CLSNRD(dmgnum);
		if(paymstr!=(-1)) CLSNRD(paymstr);
		if(payppms!=(-1)) CLSNRD(payppms);
		ShutdownSubsystems();
		exit(0);
	}
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
}
static void getrecordcb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *perid=NULL,*date=NULL,*timex=NULL;
	int transno=0,paynum=0;

	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"[SUBPAID][PERSONNEL IDENTIFICATION]",&perid);
	FINDRSCGETINT(mtnrsrc,"[SUBPAID][TRANSACTION NUMBER]",&transno);
	FINDRSCGETINT(mtnrsrc,"[SUBPAID][PAYMENT NUMBER]",&paynum);
	ZERNRD(filenum);
	FINDFLDSETSTRING(filenum,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(filenum,"TRANSACTION NUMBER",transno);
	FINDFLDSETINT(filenum,"PAYMENT NUMBER",paynum);
	seteditfilersrc(filenum,MTNMASTER->passkey->keyno,mtnrsrc,TRUE,FALSE);
	if(EQLNRDsec(filenum,MTNMASTER->passkey->keyno))
	{
		KEYNRD(filenum,MTNMASTER->passkey->keyno);
		FINDRSCSETSTRING(mtnrsrc,"[SUBPAID][SOURCE USER]",USERLOGIN);
		date=GETCURRENTDATE();
		timex=GETCURRENTTIME();
		FINDRSCSETSTRING(mtnrsrc,"[SUBPAID][ENTRY DATE]",date);
		FINDRSCSETSTRING(mtnrsrc,"[SUBPAID][ENTRY TIME]",timex);
		if(date!=NULL) Rfree(date);
		if(timex!=NULL) Rfree(timex);
		ClearRDATData(mtn->previous);
	} else {
		GetRDATData(filenum,mtn->previous);
		getrecordsetedit(mtnrsrc,mtn);
	}
	getsupportingrecords(mtnrsrc,FALSE);
	if(perid!=NULL) Rfree(perid);
	updateallrsrc(mtnrsrc);
}
static void getsupportingrecords(RDArsrc *mtnrsrc,char update)
{
	if(mstnum!=(-1))
	{
		COPYFIELD(filenum,mstnum,"PERSONNEL IDENTIFICATION");
		if(EQLNRDsec(mstnum,1)) KEYNRD(mstnum,1);
		if(update) updatefilerecord2rsrc(mstnum,mtnrsrc);
		else filerecord2rsrc(mstnum,mtnrsrc);
	}
	if(dmgnum!=(-1))
	{
		COPYFIELD(filenum,dmgnum,"PERSONNEL IDENTIFICATION");
		if(EQLNRDsec(dmgnum,1)) KEYNRD(dmgnum,1);
		if(update) updatefilerecord2rsrc(dmgnum,mtnrsrc);
		else filerecord2rsrc(dmgnum,mtnrsrc);
	}
	if(subwknum!=(-1))
	{
		COPYFIELD(filenum,subwknum,"PERSONNEL IDENTIFICATION");
		COPYFIELD(filenum,subwknum,"TRANSACTION NUMBER");
		if(EQLNRDsec(subwknum,1)) KEYNRD(subwknum,1);
		if(update) updatefilerecord2rsrc(subwknum,mtnrsrc);
		else filerecord2rsrc(subwknum,mtnrsrc);
	}
	if(paymstr!=(-1))
	{
		COPYFIELD(filenum,paymstr,"PAY IDENTIFICATION");
		if(EQLNRDsec(paymstr,1)) KEYNRD(paymstr,1);
		if(update) updatefilerecord2rsrc(paymstr,mtnrsrc);
		else filerecord2rsrc(paymstr,mtnrsrc);
	}
	if(payppms!=(-1))
	{
		COPYFIELD(filenum,payppms,"PERSONNEL IDENTIFICATION");
		COPYFIELD(filenum,payppms,"PAY IDENTIFICATION");
		if(EQLNRDsec(payppms,1)) KEYNRD(payppms,1);
		if(update) updatefilerecord2rsrc(payppms,mtnrsrc);
		else filerecord2rsrc(payppms,mtnrsrc);
	}
	if(update) updateSCRNvirtuals(mtnrsrc);
	else computeallSCRNvirtuals(mtnrsrc);
}
