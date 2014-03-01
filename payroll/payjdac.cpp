/* payjams.c - Xpert Payroll Job Account Master */
/*lint -library */
#ifndef WIN32
#define __NAM__ "payjdac.lnx"
#endif
#ifdef WIN32
#define __NAM__ "payjdac.exe"
#endif
#include <app.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

struct mtnstructs
{
	APPlib *DEFtypes;
	APPlib *PDEFtypes;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
static short baccnum=(-1),eaccnum=(-1);
static char editable=TRUE;
static MakeBrowseList *mbl_payjams=NULL;
static MaintainMaster *MASTER=NULL;
static void browse_payjams(RDArsrc *,mtnstruct *);
static void quit_record(RDArsrc *,mtnstruct *);
static void save_record_quit(RDArsrc *,mtnstruct *);
static void quit_record_test(RDArsrc *,mtnstruct *);
static void save_check(RDArsrc *,mtnstruct *,short);
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_record_now(RDArsrc *,mtnstruct *);
static void save_record_now1(RDArsrc *,mtnstruct *);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void setDEFtypes1(RDArsrc *,mtnstruct *);
static void setDEFtypes(RDArsrc *,mtnstruct *,short);
static void setPDEFtypes1(RDArsrc *,mtnstruct *);
static void setPDEFtypes(RDArsrc *,mtnstruct *,short);
static short chkacctcode(RDArsrc *,mtnstruct *,short);
static short checkacct(RDArsrc *,mtnstruct *,short);
static void chkacctcodecb(RDArsrc *,mtnstruct *);
static short chkdebitcode(RDArsrc *,mtnstruct *,short);
static short checkdebit(RDArsrc *,mtnstruct *,short);
static void chkdebitcodecb(RDArsrc *,mtnstruct *);
static short chkpayablecode(RDArsrc *,mtnstruct *,short);
/*
static short checkpayable(RDArsrc *,mtnstruct *,short);
static void chkpayablecodecb(RDArsrc *,mtnstruct *);
static short chkcashcode(RDArsrc *,mtnstruct *,short);
static short checkcash(RDArsrc *,mtnstruct *,short);
static void chkcashcodecb(RDArsrc *,mtnstruct *);
*/
static short chkcreditcode(RDArsrc *,mtnstruct *,short);
static short checkcredit(RDArsrc *,mtnstruct *,short);
static void chkcreditcodecb(RDArsrc *,mtnstruct *);
static short chkencumbrancecode(RDArsrc *,mtnstruct *,short);
static short checkencumbrance(RDArsrc *,mtnstruct *,short);
static void chkencumbrancecodecb(RDArsrc *,mtnstruct *);
static short chkreservecode(RDArsrc *,mtnstruct *,short);
static short checkreserve(RDArsrc *,mtnstruct *,short);
static void chkreservecodecb(RDArsrc *,mtnstruct *);
static void readscreen(RDArsrc *,mtnstruct *);
static void list2file(RDArsrc *,mtnstruct *);
static void NEfunction(RDArsrc *,MaintainMaster *,void (*)(...),void *,mtnstruct *);
static void BUEQLfunction(RDArsrc *,MaintainMaster *,void (*)(...),void *,mtnstruct *);
static void AUEQLfunction(RDArsrc *,MaintainMaster *,void (*)(...),void *,mtnstruct *);
static void SETID(RDArsrc *,mtnstruct *);
static void resetdefs(RDArsrc *,mtnstruct *);
static void getpreviouscb(RDArmem *,mtnstruct *);
static void getnextcb(RDArmem *,mtnstruct *);
static void SETFILES(RDArsrc *,MaintainMaster *);

static void getpreviouscb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1)) keyno=MASTER->passkey->keyno;
	ADV2GET_PREVIOUS(member->parent,MASTER,mtn->previous,SCRNvirtualSubData,
		member->parent,keyno,NEfunction,mtn,BUEQLfunction,mtn,
		AUEQLfunction,mtn);
}
static void getnextcb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1)) keyno=MASTER->passkey->keyno;
	ADV2GET_NEXT(member->parent,MASTER,mtn->previous,SCRNvirtualSubData,
		member->parent,keyno,NEfunction,mtn,BUEQLfunction,mtn,
		AUEQLfunction,mtn);
}
static void SETID(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ADV4SET_KEYFUNC(mtnrsrc,MASTER,mtn->previous,SCRNvirtualSubData,(void *)mtnrsrc,NEfunction,mtn,BUEQLfunction,mtn,AUEQLfunction,mtn);
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *MASTER)
{
	SET_MTNMASTER(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
}
static void setDEFtypes1(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	short d=0;
	int x=0;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	d=(short)x;
	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYJDAC][ACCOUNT CODE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYJDAC][DEBIT CODE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYJDAC][ENCUMBRANCE CODE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYJDAC][RESERVE CODE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYJDAC][CREDIT CODE]",d);
}
static void setDEFtypes(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *edesc=NULL;
	short d=0;
	int x=0,y=0;

	FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",&d);
	x=(int)d;
	if(x>=mtn->DEFtypes->numlibs)
	{
		for(y=0;y<(x-(mtn->DEFtypes->numlibs)-1);++y)	
		{
			addAPPlib(mtn->DEFtypes,"Invalid-Definition Type");
		}
		edesc=Rmalloc(105);
		sprintf(edesc,"The Definition Type [%d] is invalid.  It doesn't exist. Please correct or deleted this record.",d);
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEFINITION TYPE!",edesc,NULL,NULL,TRUE,NULL);
		FINDRSCSETDEFTYPE(mtnrsrc,"ACCOUNT CODE",0);
		FINDRSCSETDEFTYPE(mtnrsrc,"DEBIT CODE",0);
		FINDRSCSETDEFTYPE(mtnrsrc,"ENCUMBRANCE CODE",0);
		FINDRSCSETDEFTYPE(mtnrsrc,"RESERVE CODE",0);
		FINDRSCSETDEFTYPE(mtnrsrc,"CREDIT CODE",0);
		if(edesc!=NULL) Rfree(edesc);
		
	}
	FINDRSCSETINT(mtnrsrc,"DEFINITION TYPES",x);
	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYJDAC][ACCOUNT CODE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYJDAC][DEBIT CODE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYJDAC][ENCUMBRANCE CODE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYJDAC][RESERVE CODE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYJDAC][CREDIT CODE]",d);
	if(update) updatersrc(mtnrsrc,"DEFINITION TYPES");
}
static void changeDEFtype(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	short d1=0,d2=0;
	int x=0;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	d1=(short)x;
	FINDRSCGETDEFTYPE(mtnrsrc,"[PAYJDAC][ACCOUNT CODE]",&d2);
	if(d1!=d2)
	{
		readallwidgets(mtnrsrc);
		FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",d1);
		FINDRSCSETSTRING(mtnrsrc,"[PAYJDAC][ACCOUNT CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[PAYJDAC][CREDIT CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[PAYJDAC][DEBIT CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[PAYJDAC][ENCUMBRANCE CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"ENCUMBRANCE CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[PAYJDAC][RESERVE CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"RESERVE CODE DESCRIPTION",NULL);
		FINDRSCSETDEFTYPE(mtnrsrc,"[PAYJDAC][ACCOUNT CODE]",d1);
		FINDRSCSETDEFTYPE(mtnrsrc,"[PAYJDAC][DEBIT CODE]",d1);
		FINDRSCSETDEFTYPE(mtnrsrc,"[PAYJDAC][ENCUMBRANCE CODE]",d1);
		FINDRSCSETDEFTYPE(mtnrsrc,"[PAYJDAC][RESERVE CODE]",d1);
		FINDRSCSETDEFTYPE(mtnrsrc,"[PAYJDAC][CREDIT CODE]",d1);
		computeallSCRNvirtuals(mtnrsrc);
		updateallrsrc(mtnrsrc);
	}
}
static void setPDEFtypes1(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	short d=0;
	int x=0;

	FINDRSCGETINT(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",&x);
	d=(short)x;
	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYDDAC][LIABILITY CODE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYDDAC][CASH CODE]",d);
}
static void setPDEFtypes(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *edesc=NULL;
	short d=0;
	int x=0,y=0;

	FINDFLDGETSHORT(MASTER->passkey->fileno,"PAYROLL FUND DEFINITION TYPE",&d);
	x=(int)d;
	if(x>=mtn->PDEFtypes->numlibs)
	{
		for(y=0;y<(x-(mtn->PDEFtypes->numlibs)-1);++y)	
		{
			addAPPlib(mtn->PDEFtypes,"Invalid-Definition Type");
		}
		edesc=Rmalloc(117);
		sprintf(edesc,"The Payroll Fund Definition Type [%d] is invalid.  It doesn't exist. Please correct or deleted this record.",d);
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PAYROLL FUND DEFINITION TYPE!",edesc,NULL,NULL,TRUE,NULL);
		FINDRSCSETDEFTYPE(mtnrsrc,"LIABILITY CODE",0);
		FINDRSCSETDEFTYPE(mtnrsrc,"CASH CODE",0);
		if(edesc!=NULL) Rfree(edesc);
	}
	FINDRSCSETINT(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",x);
	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYJDAC][LIABILITY CODE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYJDAC][CASH CODE]",d);
	if(update) updatersrc(mtnrsrc,"PAYROLL FUND DEFINITION TYPES");
}
/*
static void changePDEFtype(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	short d1=0,d2=0;
	int x=0;

	FINDRSCGETINT(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",&x);
	d1=(short)x;
	FINDRSCGETDEFTYPE(mtnrsrc,"[PAYJDAC][CASH CODE]",&d2);
	if(d1!=d2)
	{
		readallwidgets(mtnrsrc);
		FINDFLDSETSHORT(MASTER->passkey->fileno,"PAYROLL FUND DEFINITION TYPE",d1);
		FINDRSCSETSTRING(mtnrsrc,"[PAYJDAC][CASH CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"CASH CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"LIABILITY CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[PAYJDAC][LIABILITY CODE]",NULL);
		FINDRSCSETDEFTYPE(mtnrsrc,"[PAYJDAC][CASH CODE]",d1);
		FINDRSCSETDEFTYPE(mtnrsrc,"[PAYJDAC][LIABILITY CODE]",d1);
		computeallSCRNvirtuals(mtnrsrc);
		updateallrsrc(mtnrsrc);
	}
}
*/
void chkacctcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	checkacct(mtnrsrc,mtn,TRUE);
	updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static short checkacct(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL;
	short d=0;
	int x=0;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	d=x;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",d);
	readwidget(mtnrsrc,"[PAYJDAC][ACCOUNT CODE]");
	FINDRSCGETSTRING(mtnrsrc,"[PAYJDAC][ACCOUNT CODE]",&acctcode);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"ACCOUNT CODE",acctcode);
	if(acctcode!=NULL) Rfree(acctcode);
	return(chkacctcode(mtnrsrc,mtn,update));
}
static short chkacctcode(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL,*desc=NULL,*edesc=NULL;
	short return_value=TRUE;

	COPYFIELD(MASTER->passkey->fileno,eaccnum,"DEFINITION TYPE");
	FINDFLDGETSTRING(MASTER->passkey->fileno,"ACCOUNT CODE",&acctcode);
	FINDFLDSETSTRING(eaccnum,"ACCOUNT CODE",acctcode);
	if(ADVEQLNRDsec(eaccnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(eaccnum,1);
		edesc=Rmalloc(RDAstrlen(acctcode)+109);
		sprintf(edesc,"The Account Code [%s] is invalid.  It doesn't exist. Please enter a correct account or cancel this operation.",(acctcode==NULL?"":acctcode));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID ACCOUNT CODE",edesc,NULL,NULL,TRUE,NULL);
		if(edesc!=NULL) Rfree(edesc);
		return_value=FALSE;
	}
	if(acctcode!=NULL) Rfree(acctcode);
	FINDFLDGETSTRING(eaccnum,"DESCRIPTION",&desc);
	FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",desc);
	if(update) updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
	return(return_value);
}
void chkencumbrancecodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	checkencumbrance(mtnrsrc,mtn,TRUE);
	updateSCRNvirtuals(mtnrsrc);
}
static short checkencumbrance(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL,disenc;
	short d=0;
	int x=0;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	FINDRSCGETCHAR(mtnrsrc,"[PAYDDAC][DISENCUMBER]",&disenc);
	FINDFLDSETCHAR(MASTER->passkey->fileno,"DISENCUMBER",disenc);
	d=x;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",d);
	readwidget(mtnrsrc,"[PAYJDAC][ENCUMBRANCE CODE]");
	FINDRSCGETSTRING(mtnrsrc,"[PAYJDAC][ENCUMBRANCE CODE]",&acctcode);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"ENCUMBRANCE CODE",acctcode);
	if(acctcode!=NULL) Rfree(acctcode);
	return(chkencumbrancecode(mtnrsrc,mtn,update));
}
static short chkencumbrancecode(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char disenc=FALSE,*acctcode=NULL,*desc=NULL,*edesc=NULL;
	short return_value=TRUE;

	FINDFLDGETCHAR(MASTER->passkey->fileno,"[PAYDDAC][DISENCUMBER]",&disenc);
	if(disenc)
	{
		COPYFIELD(MASTER->passkey->fileno,baccnum,"DEFINITION TYPE");
		FINDFLDGETSTRING(MASTER->passkey->fileno,"ENCUMBRANCE CODE",&acctcode);
		FINDFLDSETSTRING(baccnum,"ACCOUNT CODE",acctcode);
		if(ADVEQLNRDsec(baccnum,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(baccnum,1);
			edesc=Rmalloc(RDAstrlen(acctcode)+109);
			sprintf(edesc,"The Encumbrance Code [%s] is invalid.  It doesn't exist. Please enter a correct account or cancel this operation.",(acctcode==NULL?"":acctcode));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID ENCUMBRANCE CODE",edesc,NULL,NULL,TRUE,NULL);
			if(edesc!=NULL) Rfree(edesc);
			return_value=FALSE;
		}
		if(acctcode!=NULL) Rfree(acctcode);
		FINDFLDGETSTRING(baccnum,"DESCRIPTION",&desc);
		FINDRSCSETSTRING(mtnrsrc,"ENCUMBRANCE CODE DESCRIPTION",desc);
		if(update) updatersrc(mtnrsrc,"ENCUMBRANCE CODE DESCRIPTION");
		return(return_value);
	} else return(TRUE);
}
void chkreservecodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	checkreserve(mtnrsrc,mtn,TRUE);
	updateSCRNvirtuals(mtnrsrc);
}
static short checkreserve(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL,disenc=FALSE;
	short d=0;
	int x=0;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	FINDRSCGETCHAR(mtnrsrc,"[PAYDDAC][DISENCUMBER]",&disenc);
	FINDFLDSETCHAR(MASTER->passkey->fileno,"DISENCUMBER",disenc);
	d=x;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",d);
	readwidget(mtnrsrc,"[PAYJDAC][RESERVE CODE]");
	FINDRSCGETSTRING(mtnrsrc,"[PAYJDAC][RESERVE CODE]",&acctcode);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"RESERVE CODE",acctcode);
	if(acctcode!=NULL) Rfree(acctcode);
	return(chkreservecode(mtnrsrc,mtn,update));
}
static short chkreservecode(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char disenc=FALSE,*acctcode=NULL,*desc=NULL,*edesc=NULL;
	short return_value=TRUE;

	FINDFLDGETCHAR(MASTER->passkey->fileno,"[PAYDDAC][DISENCUMBER]",&disenc);
	if(disenc)
	{
		COPYFIELD(MASTER->passkey->fileno,baccnum,"DEFINITION TYPE");
		FINDFLDGETSTRING(MASTER->passkey->fileno,"RESERVE CODE",&acctcode);
		FINDFLDSETSTRING(baccnum,"ACCOUNT CODE",acctcode);
		if(ADVEQLNRDsec(baccnum,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(baccnum,1);
			edesc=Rmalloc(RDAstrlen(acctcode)+109);
			sprintf(edesc,"The Reserve Code [%s] is invalid.  It doesn't exist. Please enter a correct account or cancel this operation.",(acctcode==NULL?"":acctcode));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID RESERVE CODE",edesc,NULL,NULL,TRUE,NULL);
			if(edesc!=NULL) Rfree(edesc);
			return_value=FALSE;
		}
		if(acctcode!=NULL) Rfree(acctcode);
		FINDFLDGETSTRING(baccnum,"DESCRIPTION",&desc);
		FINDRSCSETSTRING(mtnrsrc,"RESERVE CODE DESCRIPTION",desc);
		if(update) updatersrc(mtnrsrc,"RESERVE CODE DESCRIPTION");
		return(return_value);
	} else return(TRUE);
}
void chkdebitcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	checkdebit(mtnrsrc,mtn,TRUE);
	updateSCRNvirtuals(mtnrsrc);
}
static short checkdebit(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL;
	short d=0;
	int x=0;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	d=x;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",d);
	readwidget(mtnrsrc,"[PAYJDAC][DEBIT CODE]");
	FINDRSCGETSTRING(mtnrsrc,"[PAYJDAC][DEBIT CODE]",&acctcode);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"DEBIT CODE",acctcode);
	if(acctcode!=NULL) Rfree(acctcode);
	return(chkdebitcode(mtnrsrc,mtn,update));
}
static short chkdebitcode(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL,*desc=NULL,*edesc=NULL;
	short return_value=TRUE;

	COPYFIELD(MASTER->passkey->fileno,baccnum,"DEFINITION TYPE");
	FINDFLDGETSTRING(MASTER->passkey->fileno,"DEBIT CODE",&acctcode);
	FINDFLDSETSTRING(baccnum,"ACCOUNT CODE",acctcode);
	if(ADVEQLNRDsec(baccnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(baccnum,1);
		edesc=Rmalloc(RDAstrlen(acctcode)+109);
		sprintf(edesc,"The Debit Code [%s] is invalid.  It doesn't exist. Please enter a correct account or cancel this operation.",(acctcode==NULL?"":acctcode));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE",edesc,NULL,NULL,TRUE,NULL);
		if(edesc!=NULL) Rfree(edesc);
		return_value=FALSE;
	}
	if(acctcode!=NULL) Rfree(acctcode);
	FINDFLDGETSTRING(baccnum,"DESCRIPTION",&desc);
	FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",desc);
	if(update) updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
	return(return_value);
}
/*
void chkpayablecodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	checkpayable(mtnrsrc,mtn,TRUE);
	updatersrc(mtnrsrc,"LIABILITY CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static short checkpayable(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL;
	short d=0;
	int x=0;

	FINDRSCGETINT(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",&x);
	d=x;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"PAYROLL FUND DEFINITION TYPE",d);
	readwidget(mtnrsrc,"[PAYDDAC][LIABILITY CODE]");
	FINDRSCGETSTRING(mtnrsrc,"[PAYDDAC][LIABILITY CODE]",&acctcode);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"LIABILITY CODE",acctcode);
	if(acctcode!=NULL) Rfree(acctcode);
	return(chkpayablecode(mtnrsrc,mtn,update));
}
static short chkpayablecode(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL,*desc=NULL,*edesc=NULL;
	short return_value=TRUE;
	
	FIELDCOPY(MASTER->passkey->fileno,"PAYROLL FUND DEFINITION TYPE",baccnum,
		"DEFINITION TYPE");
	FINDFLDGETSTRING(MASTER->passkey->fileno,"LIABILITY CODE",&acctcode);
	FINDFLDSETSTRING(baccnum,"ACCOUNT CODE",acctcode);
	if(ADVEQLNRDsec(baccnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(baccnum,1);
		edesc=Rmalloc(RDAstrlen(acctcode)+111);
		sprintf(edesc,"The Payable Code [%s] is invalid.  It doesn't exist. Please enter a correct account or cancel this operation.",(acctcode==NULL?"":acctcode));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID LIABILITY CODE",edesc,NULL,NULL,TRUE,NULL);
		if(edesc!=NULL) Rfree(edesc);
		return_value=FALSE;
	}
	if(acctcode!=NULL) Rfree(acctcode);
	FINDFLDGETSTRING(baccnum,"DESCRIPTION",&desc);
	FINDRSCSETSTRING(mtnrsrc,"LIABILITY CODE DESCRIPTION",desc);
	if(update) updatersrc(mtnrsrc,"LIABILITY CODE DESCRIPTION");
	return(return_value);
}
void chkcashcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	checkcash(mtnrsrc,mtn,TRUE);
	updateSCRNvirtuals(mtnrsrc);
}
static short checkcash(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL;
	short d=0;
	int x=0;

	FINDRSCGETINT(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",&x);
	d=x;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"PAYROLL FUND DEFINITION TYPE",d);
	readwidget(mtnrsrc,"[PAYDDAC][CASH CODE]");
	FINDRSCGETSTRING(mtnrsrc,"[PAYDDAC][CASH CODE]",&acctcode);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"CASH CODE",acctcode);
	if(acctcode!=NULL) Rfree(acctcode);
	return(chkcashcode(mtnrsrc,mtn,update));
}
static short chkcashcode(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL,*desc=NULL,*edesc=NULL;
	short return_value=TRUE;
	
	FIELDCOPY(MASTER->passkey->fileno,"PAYROLL FUND DEFINITION TYPE",baccnum,
		"DEFINITION TYPE");
	FINDFLDGETSTRING(MASTER->passkey->fileno,"CASH CODE",&acctcode);
	FINDFLDSETSTRING(baccnum,"ACCOUNT CODE",acctcode);
	if(ADVEQLNRDsec(baccnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(baccnum,1);
		edesc=Rmalloc(RDAstrlen(acctcode)+111);
		sprintf(edesc,"The Cash Code [%s] is invalid.  It doesn't exist. Please enter a correct account or cancel this operation.",(acctcode==NULL?"":acctcode));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CASH CODE",edesc,NULL,NULL,TRUE,NULL);
		if(edesc!=NULL) Rfree(edesc);
		return_value=FALSE;
	}
	if(acctcode!=NULL) Rfree(acctcode);
	FINDFLDGETSTRING(baccnum,"DESCRIPTION",&desc);
	FINDRSCSETSTRING(mtnrsrc,"CASH CODE DESCRIPTION",desc);
	if(update) updatersrc(mtnrsrc,"CASH CODE DESCRIPTION");
	return(return_value);
}
*/
void chkcreditcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	checkcredit(mtnrsrc,mtn,TRUE);
	updateSCRNvirtuals(mtnrsrc);
}
static short checkcredit(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL;
	short d=0;
	int x=0;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	d=x;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",d);
	readwidget(mtnrsrc,"[PAYJDAC][CREDIT CODE]");
	FINDRSCGETSTRING(mtnrsrc,"[PAYJDAC][CREDIT CODE]",&acctcode);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"CREDIT CODE",acctcode);
	if(acctcode!=NULL) Rfree(acctcode);
	return(chkcreditcode(mtnrsrc,mtn,update));
}
static short chkcreditcode(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL,*desc=NULL,*edesc=NULL;
	short return_value=TRUE;

	COPYFIELD(MASTER->passkey->fileno,baccnum,"DEFINITION TYPE");
	FINDFLDGETSTRING(MASTER->passkey->fileno,"CREDIT CODE",&acctcode);
	FINDFLDSETSTRING(baccnum,"ACCOUNT CODE",acctcode);
	if(ADVEQLNRDsec(baccnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(baccnum,1);
		edesc=Rmalloc(RDAstrlen(acctcode)+109);
		sprintf(edesc,"The Credit Code [%s] is invalid.  It doesn't exist. Please enter a correct account or cancel this operation.",(acctcode==NULL?"":acctcode));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE",edesc,NULL,NULL,TRUE,NULL);
		if(edesc!=NULL) Rfree(edesc);
		return_value=FALSE;
	}
	if(acctcode!=NULL) Rfree(acctcode);
	FINDFLDGETSTRING(baccnum,"DESCRIPTION",&desc);
	FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",desc);
	if(update) updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
	return(return_value);
}
void payjamsm(short dowhich,void *targetkey)
{
	char *name=NULL;
	int x=0,acctno=0,y=0;
	RDArsrc *mtnrsrc=NULL;
	RDAacct *acc=NULL;
	mtnstruct *mtn=NULL;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	short nofields=0,nokeys=0,edit_rsrc=0;
	
	mtnrsrc=RDArsrcNEW(MASTER->module,MASTER->maintain_screen);
	ZERNRD(MASTER->passkey->fileno);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->DEFtypes=NULL;
	mtn->PDEFtypes=NULL;
	mtn->previous=NULL;
	mtn->previous=RDATDataNEW(MASTER->passkey->fileno);
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
			if(!RDAstrcmp(fldx->name,"DEFINITION TYPE"))
			{
				mtn->DEFtypes=APPlibNEW();
				if(EXPENDITURE_ACCOUNT!=NULL)
				{
					if(EXPENDITURE_ACCOUNT->codetype!=NULL)
					{
						for(y=0,acc=EXPENDITURE_ACCOUNT->codetype;
							y<EXPENDITURE_ACCOUNT->num;++y,++acc)
						{
							addAPPlib(mtn->DEFtypes,acc->name);
						}
					}
				} 
				if(mtn->DEFtypes->numlibs<1)
				{
					addAPPlib(mtn->DEFtypes,"No Definitions Available");
				}
				addlstrsrc(mtnrsrc,"DEFINITION TYPES",&acctno,TRUE,changeDEFtype,mtn->DEFtypes->numlibs,&mtn->DEFtypes->libs,mtn);
			} else {
				edit_rsrc=editable;
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
	FINDRSCSETFUNC(mtnrsrc,"[PAYJDAC][ACCOUNT CODE]",chkacctcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[PAYJDAC][DEBIT CODE]",chkdebitcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[PAYJDAC][ENCUMBRANCE CODE]",chkencumbrancecodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[PAYJDAC][RESERVE CODE]",chkreservecodecb,mtn);
/*
	FINDRSCSETFUNC(mtnrsrc,"[PAYDDAC][LIABILITY CODE]",chkpayablecodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[PAYDDAC][CASH CODE]",chkcashcodecb,mtn);
*/
	FINDRSCSETFUNC(mtnrsrc,"[PAYJDAC][CREDIT CODE]",chkcreditcodecb,mtn);
	addstdrsrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"DEBIT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"ENCUMBRANCE CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"RESERVE CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"LIABILITY CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"CASH CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"CREDIT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	SET_MAINTAIN_FILE2RSRCS(mtnrsrc,MASTER);
	if(!FINDRSCGETINT(mtnrsrc,"[PAYDDAC][PAYROLL FUND DEFINITION TYPE]",&acctno))
	{
		mtn->PDEFtypes=APPlibNEW();
		if(BALANCE_SHEET_ACCOUNT!=NULL)
		{
			if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
			{
				for(y=0,acc=BALANCE_SHEET_ACCOUNT->codetype;
					y<BALANCE_SHEET_ACCOUNT->num;++y,++acc)
				{
					addAPPlib(mtn->PDEFtypes,acc->name);
				}
			}
		} 
		if(mtn->PDEFtypes->numlibs<1)
		{
			addAPPlib(mtn->PDEFtypes,"No Definitions Available");
		}
		addlstrsrc(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",&acctno,
			TRUE,NULL,mtn->PDEFtypes->numlibs,
			&mtn->PDEFtypes->libs,mtn);
	}
	SCRNvirtual2rsrc(mtnrsrc);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
			ApplyPassKey(MASTER->passkey);
			if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(MASTER->passkey->fileno,MASTER->passkey->keyno);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
				if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
					list2file(mtnrsrc,mtn);
				} else {
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
					setDEFtypes(mtnrsrc,mtn,FALSE);
					setPDEFtypes(mtnrsrc,mtn,FALSE);
					chkacctcode(mtnrsrc,mtn,FALSE);
					chkdebitcode(mtnrsrc,mtn,FALSE);
					chkcreditcode(mtnrsrc,mtn,FALSE);
/*
					chkpayablecode(mtnrsrc,mtn,FALSE);
					chkcashcode(mtnrsrc,mtn,FALSE);
*/
					chkencumbrancecode(mtnrsrc,mtn,FALSE);
					chkreservecode(mtnrsrc,mtn,FALSE);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				setDEFtypes(mtnrsrc,mtn,FALSE);
				setPDEFtypes(mtnrsrc,mtn,FALSE);
				chkacctcode(mtnrsrc,mtn,FALSE);
				chkdebitcode(mtnrsrc,mtn,FALSE);
				chkcreditcode(mtnrsrc,mtn,FALSE);
/*
				chkpayablecode(mtnrsrc,mtn,FALSE);
				chkcashcode(mtnrsrc,mtn,FALSE);
*/
				chkencumbrancecode(mtnrsrc,mtn,FALSE);
				chkreservecode(mtnrsrc,mtn,FALSE);
			}
			break;
		case 2:
			ApplyPassKey(MASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
			if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
				list2file(mtnrsrc,mtn);
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				setDEFtypes(mtnrsrc,mtn,FALSE);
				setPDEFtypes(mtnrsrc,mtn,FALSE);
				chkacctcode(mtnrsrc,mtn,FALSE);
				chkdebitcode(mtnrsrc,mtn,FALSE);
				chkcreditcode(mtnrsrc,mtn,FALSE);
/*
				chkpayablecode(mtnrsrc,mtn,FALSE);
				chkcashcode(mtnrsrc,mtn,FALSE);
*/
				chkencumbrancecode(mtnrsrc,mtn,FALSE);
				chkreservecode(mtnrsrc,mtn,FALSE);
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
					list2file(mtnrsrc,mtn);
				} else {
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
					setDEFtypes(mtnrsrc,mtn,FALSE);
					setPDEFtypes(mtnrsrc,mtn,FALSE);
					chkacctcode(mtnrsrc,mtn,FALSE);
					chkdebitcode(mtnrsrc,mtn,FALSE);
					chkcreditcode(mtnrsrc,mtn,FALSE);
/*
					chkpayablecode(mtnrsrc,mtn,FALSE);
					chkcashcode(mtnrsrc,mtn,FALSE);
*/
					chkencumbrancecode(mtnrsrc,mtn,FALSE);
					chkreservecode(mtnrsrc,mtn,FALSE);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				setDEFtypes(mtnrsrc,mtn,FALSE);
				setPDEFtypes(mtnrsrc,mtn,FALSE);
				chkacctcode(mtnrsrc,mtn,FALSE);
				chkdebitcode(mtnrsrc,mtn,FALSE);
				chkcreditcode(mtnrsrc,mtn,FALSE);
/*
				chkpayablecode(mtnrsrc,mtn,FALSE);
				chkcashcode(mtnrsrc,mtn,FALSE);
*/
				chkencumbrancecode(mtnrsrc,mtn,FALSE);
				chkreservecode(mtnrsrc,mtn,FALSE);
			}
			break;
	}
	GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
	if(mbl_payjams!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_payjams,mtn);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,mtn);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	ADVMakePrevNextButtons(mtnrsrc,keys,nokeys,getpreviouscb,mtn,getnextcb,mtn);
	MakeMasterButtons(mtnrsrc,MASTER);
	computeallSCRNvirtuals(mtnrsrc);
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	if(mbl_payjams!=NULL) FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_payjams==NULL?TRUE:FALSE));
}
static void resetdefs(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	updateallrsrc(mtnrsrc);
	SETID(mtnrsrc,mtn);
}
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MASTER->passkey->fileno);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
	list2file(mtnrsrc,mtn);
}
static void list2file(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	setDEFtypes1(mtnrsrc,mtn);
	setPDEFtypes1(mtnrsrc,mtn);
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
static void save_check(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	char *desc=NULL;
	int seldeftype=0;
	short deftype=0;
	char delflag=FALSE,disenc=FALSE;

	FINDRSCGETCHAR(mtnrsrc,"[PAYJDAC][DELETEFLAG]",&delflag);
	FINDRSCGETCHAR(mtnrsrc,"[PAYDDAC][DISENCUMBER]",&disenc);
	if(!delflag)
	{
		FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&seldeftype);
		if(!RDAstrcmp(mtn->DEFtypes->libs[seldeftype],"No Definitions Available"))
		{
			desc=Rmalloc(RDAstrlen(mtn->DEFtypes->libs[seldeftype])+103);
			sprintf(desc,"The Definition Type of [%s] is invalid. Can't save record with invalid Definition Type.",mtn->DEFtypes->libs[seldeftype]);
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEFINITION TYPE!",desc,NULL,NULL,TRUE,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		} else {
			deftype=(short)seldeftype;
			FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",deftype);
			if(checkacct(mtnrsrc,mtn,TRUE)==TRUE)
			{
				if(checkdebit(mtnrsrc,mtn,TRUE)==TRUE)
				{
/*
					if(checkpayable(mtnrsrc,mtn,TRUE)==TRUE)
					{
						if(checkcash(mtnrsrc,mtn,TRUE)==FALSE)
						{
*/
							if(checkcredit(mtnrsrc,mtn,TRUE)==FALSE)
							{
								return;
						
							} else if(disenc)
							{
								if(checkencumbrance(mtnrsrc,mtn,TRUE)==TRUE)
								{
									if(checkreserve(mtnrsrc,mtn,TRUE)==FALSE)
									{
										return;
									}
								} else return;
							}
/*
						} else return;
					} else return;
*/
				} else return;
			} else return;
		}
	}
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
				desc,(update_list ? save_record_now:save_record_now1),
				NULL,FALSE,2,mtnrsrc,mtn,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	save_record(mtnrsrc,mtn,update_list);
}
static void save_record(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	char *rcddesc=NULL;

	if(ADVRECORDsec(MASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(MASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("SECURITY ACCESS DENIED!",rcddesc,NULL,FALSE); 
			prterr("Security Access Denied [%s].  Didn't write record.",rcddesc);
			Rfree(rcddesc);
		} else {
			prterr("Error didn't write record.");
		}
	} else {
		WRTTRANS(MASTER->passkey->fileno,0,NULL,mtn->previous);
		DELETE_SUBORDINATES(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update_list,mbl_payjams,MASTER->passkey->fileno,MASTER->module,mtnrsrc);
		if(mbl_payjams!=NULL) quit_record(mtnrsrc,mtn);
		else GetRDATData(MASTER->passkey->fileno,mtn->previous);
	}
}
static void save_record_now(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readscreen(mtnrsrc,mtn);
	save_record(mtnrsrc,mtn,TRUE);
}
static void save_record_now1(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readscreen(mtnrsrc,mtn);
	save_record(mtnrsrc,mtn,FALSE);
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
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->PDEFtypes!=NULL) freeapplib(mtn->PDEFtypes);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
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
static void NEfunction(RDArsrc *mtnrsrc,MaintainMaster *MASTER,void (*SubFunc)(...),
	void *args,mtnstruct *mtn)
{
	list2file(mtnrsrc,mtn);
}
static void BUEQLfunction(RDArsrc *mtnrsrc,MaintainMaster *MASTER,void (*SubFunc)(...),
	void *args,mtnstruct *mtn)
{
	setDEFtypes(mtnrsrc,mtn,TRUE);
	setPDEFtypes(mtnrsrc,mtn,TRUE);
}
static void AUEQLfunction(RDArsrc *mtnrsrc,MaintainMaster *MASTER,void (*SubFunc)(...),
	void *args,mtnstruct *mtn)
{
	updatefilerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
	chkacctcode(mtnrsrc,mtn,TRUE);
	chkdebitcode(mtnrsrc,mtn,TRUE);
	chkcreditcode(mtnrsrc,mtn,TRUE);
/*
	chkpayablecode(mtnrsrc,mtn,TRUE);
	chkcashcode(mtnrsrc,mtn,TRUE);
*/
	chkencumbrancecode(mtnrsrc,mtn,TRUE);
	chkreservecode(mtnrsrc,mtn,TRUE);
}
void browse_payjams(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtn!=NULL)
	{
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->PDEFtypes!=NULL) freeapplib(mtn->PDEFtypes);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	mainrsrc=RDArsrcNEW(MASTER->module,MASTER->browse_screen);
	searchrsrc=RDArsrcNEW(MASTER->module,MASTER->search_browse);
	definelist=RDArsrcNEW(MASTER->module,MASTER->define_list);
	ZERNRD(MASTER->passkey->fileno);
	ApplyPassKey(MASTER->passkey);
	GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,MASTER,
		SCRNvirtualSubData,(void *)mainrsrc);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	MakeRunFunction(mainrsrc,MASTER);
	mbl_payjams=ADVPbrowse(MASTER->passkey->fileno,MASTER->passkey->keyno,mainrsrc,
		searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MASTER->passkey);
}
int main(int argc,char **argv)
{
	short filenum=(-1);

	if(InitializeSubsystems(argc,argv,"PAYROLL","MTN PAYJDAC")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	MASTER=MaintainMasterNew("PAYROLL","MTN PAYJDAC");
	if(findMaintainMaster(MASTER))
	{
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE);
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN PAYJDAC",MASTER->module);
		doexit(NULL);
	} else {
		if((filenum=APPOPNNRDEDITABLE(MASTER->module,"PAYJDAC",&editable))==(-1))
		{
			doexit(NULL);
			return;
		}
		if(!OPEN_MASTER_FILES(MASTER))
		{
			doexit(NULL);
			return;
		}
		eaccnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINEACC",1);
		baccnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINBACC",1);
		MASTER->passkey=READPassKey(filenum);
		MASTER->inheritrsrcs=TRUE;
		switch(MASTER->start_with)
		{
			default:
			case 0:
				payjamsm(1,NULL);
				break;
			case 1:
				browse_payjams(NULL,NULL);
				break;
		}
	}
	RDAAPPMAINLOOP();
}
void doexit(MakeBrowseList *blist)
{
	if(blist!=NULL) 
	{
		if(blist->definelist!=NULL) 
		{
			killwindow(blist->definelist);
			free_rsrc(blist->definelist);
		}
		if(blist->searchrsrc!=NULL) 
		{
			killwindow(blist->searchrsrc);
			free_rsrc(blist->searchrsrc);
		}
		if(blist->mainrsrc!=NULL)
		{
			free_rsrc(blist->mainrsrc);
		}
	}
	if(WindowCount<2)
	{
		CLOSE_MASTER_FILES(MASTER);
		if(MASTER!=NULL) FreeMaintainMaster(MASTER);
		ShutdownSubsystems();
	}
	if(mbl_payjams!=NULL) mbl_payjams=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func))
	{
		if(!run_func)
		{
			payjamsm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MASTER);
		}
	} else {
		payjamsm(0,targetkey);
	}
}
static void new_record(RDArsrc *mtnrsrc,MakeBrowseList *blist)
{
	payjamsm(2,NULL);
} 
