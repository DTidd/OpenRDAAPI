/* posaccm.c - Xpert Position Account Maintain Screen */
#include <stdio.h>
#include <rdaglob.h>
#include <rdaflds.h>
#include <alloc.h>
#include <gui.h>
#include <nrd.h>
#include <trans.h>
#include <sec.h>
#include <mix.h>

struct mtnstructs
{
	APPlib *DEFtypes;
	APPlib *PDEFtypes;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern short eaccnum,baccnum;
extern char editable;
extern MakeBrowseList *mbl_acc;
extern MaintainMaster *MASTER;
extern void browse_acct(RDArsrc *,mtnstruct *);
static void quit_record(RDArsrc *,mtnstruct *);
static void quit_record_test(RDArsrc *,mtnstruct *);
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_record_quit(RDArsrc *,mtnstruct *);
static void save_check(RDArsrc *,mtnstruct *,short);
static void save_record_now(RDArsrc *,mtnstruct *);
static void save_record_now1(RDArsrc *,mtnstruct *);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void DEFtypecb(RDArsrc *,mtnstruct *);
static void changeDEFtype(RDArsrc *,mtnstruct *,short);
static void PDEFtypecb(RDArsrc *,mtnstruct *);
static void changePDEFtype(RDArsrc *,mtnstruct *,short);
static short check_acctcode(RDArsrc *,mtnstruct *,short);
static short check_debcode(RDArsrc *,mtnstruct *,short);
static short check_enccode(RDArsrc *,mtnstruct *,short);
static short check_rescode(RDArsrc *,mtnstruct *,short);
static short check_credcode(RDArsrc *,mtnstruct *,short);
static short check_payablecode(RDArsrc *,mtnstruct *,short);
static short check_cashcode(RDArsrc *,mtnstruct *,short);
static void check_acctcodecb(RDArsrc *,mtnstruct *);
static void check_debcodecb(RDArsrc *,mtnstruct *);
static void check_enccodecb(RDArsrc *,mtnstruct *);
static void check_rescodecb(RDArsrc *,mtnstruct *);
static void check_credcodecb(RDArsrc *,mtnstruct *);
static void check_payablecodecb(RDArsrc *,mtnstruct *);
static void check_cashcodecb(RDArsrc *,mtnstruct *);
static short setacctcode(RDArsrc *,mtnstruct *,short);
static short setdebitcode(RDArsrc *,mtnstruct *,short);
static short setcreditcode(RDArsrc *,mtnstruct *,short);
static short setpayablecode(RDArsrc *,mtnstruct *,short);
static short setcashcode(RDArsrc *,mtnstruct *,short);
static short setenccode(RDArsrc *,mtnstruct *,short);
static short setrescode(RDArsrc *,mtnstruct *,short);
static void setDEFtypes(RDArsrc *,mtnstruct *,short);
static void setPDEFtypes(RDArsrc *,mtnstruct *,short);
static void check_codes(RDArsrc *,mtnstruct *,short);
static void setcodes(RDArsrc *,mtnstruct *,short);
static void readscreen(RDArsrc *,mtnstruct *);
static void list2file(RDArsrc *,mtnstruct *,short);
static void NEfunction(RDArsrc *,MaintainMaster *,void (*)(),void *,mtnstruct *);
static void BUEQLfunction(RDArsrc *,MaintainMaster *,void (*)(),void *,mtnstruct *);
static void AUEQLfunction(RDArsrc *,MaintainMaster *,void (*)(),void *,mtnstruct *);
static void SETID(RDArsrc *mtnrsrc,mtnstruct *mtn);
static void SETFILES(RDArsrc *,MaintainMaster *);
static void getpreviouscb(RDArmem *,mtnstruct *);
static void getnextcb(RDArmem *,mtnstruct *);
static void resetdefs(RDArsrc *,mtnstruct *);
void posaccm(short,void *);

static void getpreviouscb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1)) keyno=MASTER->passkey->keyno;
	ADV2GET_PREVIOUS(member->parent,MASTER,mtn->previous,SCRNvirtualSubData,member->parent,
		keyno,NEfunction,mtn,BUEQLfunction,mtn,AUEQLfunction,mtn);
}
static void getnextcb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1)) keyno=MASTER->passkey->keyno;
	ADV2GET_NEXT(member->parent,MASTER,mtn->previous,SCRNvirtualSubData,member->parent,
		keyno,NEfunction,mtn,BUEQLfunction,mtn,AUEQLfunction,mtn);
}
static void NEfunction(RDArsrc *mtnrsrc,MaintainMaster *MASTER,void (*SubFunc)(),
	void *args,mtnstruct *mtn)
{
	FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(mtnrsrc,"PAYABLE CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(mtnrsrc,"CASH CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(mtnrsrc,"ENCUMBRANCE CODE DESCRIPTION",NULL);
	FINDRSCSETSTRING(mtnrsrc,"RESERVE CODE DESCRIPTION",NULL);
	list2file(mtnrsrc,mtn,TRUE);
}
static void BUEQLfunction(RDArsrc *mtnrsrc,MaintainMaster *MASTER,void (*SubFunc)(),
	void *args,mtnstruct *mtn)
{

	setDEFtypes(mtnrsrc,mtn,TRUE);
	setPDEFtypes(mtnrsrc,mtn,TRUE);
}
static void AUEQLfunction(RDArsrc *mtnrsrc,MaintainMaster *MASTER,void (*SubFunc)(),
	void *args,mtnstruct *mtn)
{
	setcodes(mtnrsrc,mtn,TRUE);
	updatefilerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
}
static void SETID(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ADV3SET_KEYFUNC(mtnrsrc,MASTER,mtn->previous,SCRNvirtualSubData,(void *)mtnrsrc,TRUE,NEfunction,mtn,BUEQLfunction,mtn,AUEQLfunction,mtn);
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *MASTER)
{
	SET_MASTER(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
}
static void setDEFtypes(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *edesc=NULL;
	int selectedd=0,listlen=0;
	short deftype1=0;

	FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",&deftype1);
	selectedd=(int)deftype1;
	if(selectedd>=mtn->DEFtypes->numlibs)
	{
		listlen=(selectedd-mtn->DEFtypes->numlibs+1);
		for(selectedd=0;selectedd<listlen;selectedd++)
		{
			addAPPlib(mtn->DEFtypes,"Invalid-Definition Type");
			deftype1=0;
			selectedd=0;
			edesc=Rmalloc(105);
			sprintf(edesc,"The Definition Type [%d] is invalid.  It doesn't exist. Please correct or deleted this record.",deftype1);
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEFINITION TYPE!",edesc,NULL,NULL,TRUE,NULL);
			if(edesc!=NULL) Rfree(edesc);
		}
	}
	FINDRSCSETDEFTYPE(mtnrsrc,"[POSACC][ACCOUNT CODE]",deftype1);
	FINDRSCSETDEFTYPE(mtnrsrc,"[POSACC][DEBIT CODE]",deftype1);
	FINDRSCSETDEFTYPE(mtnrsrc,"[POSACC][CREDIT CODE]",deftype1);
	FINDRSCSETDEFTYPE(mtnrsrc,"[POSACC][ENCUMBRANCE CODE]",deftype1);
	FINDRSCSETDEFTYPE(mtnrsrc,"[POSACC][RESERVE CODE]",deftype1);
	if(!FINDRSCLISTAPPlib(mtnrsrc,"DEFINITION TYPES",selectedd,mtn->DEFtypes))
	{
		if(update) updatersrc(mtnrsrc,"DEFINITION TYPES");
	}
}
static void setPDEFtypes(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *edesc=NULL;
	int selectedd=0,listlen=0;
	short deftype1=0;

	FINDFLDGETSHORT(MASTER->passkey->fileno,"PAYROLL FUND DEFINITION TYPE",&deftype1);
	selectedd=(int)deftype1;
	if(selectedd>=mtn->PDEFtypes->numlibs)
	{
		listlen=(selectedd-mtn->PDEFtypes->numlibs+1);
		for(selectedd=0;selectedd<listlen;selectedd++)
		{
			addAPPlib(mtn->PDEFtypes,"Invalid-Definition Type");
			deftype1=0;
			selectedd=0;
			edesc=Rmalloc(117);
			sprintf(edesc,"The Payroll Fund Definition Type [%d] is invalid.  It doesn't exist. Please correct or deleted this record.",deftype1);
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PAYROLL FUND DEFINITION TYPE!",edesc,NULL,NULL,TRUE,NULL);
			if(edesc!=NULL) Rfree(edesc);
		}
	}
	FINDRSCSETDEFTYPE(mtnrsrc,"[POSACC][PAYABLE CODE]",deftype1);
	FINDRSCSETDEFTYPE(mtnrsrc,"[POSACC][CASH CODE]",deftype1);
	if(!FINDRSCLISTAPPlib(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",selectedd,mtn->PDEFtypes))
	{
		if(update) updatersrc(mtnrsrc,"PAYROLL FUND DEFINITION TYPES");
	}
}
static void PDEFtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readallwidgets(mtnrsrc);
	changePDEFtype(mtnrsrc,mtn,TRUE);
	updateSCRNvirtuals(mtnrsrc);
}
static void DEFtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readallwidgets(mtnrsrc);
	changeDEFtype(mtnrsrc,mtn,TRUE);
	updateSCRNvirtuals(mtnrsrc);
}
static void changePDEFtype(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	int selectedd=0;
	short deftype1=0,deftype2=0;

	if(FINDRSCGETINT(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",&selectedd)) return;
	FINDRSCGETDEFTYPE(mtnrsrc,"[POSACC][PAYABLE CODE]",&deftype1);
	deftype2=(short)selectedd;
	if(deftype1!=deftype2)
	{
		FINDRSCSETSTRING(mtnrsrc,"[POSACC][PAYABLE CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"PAYABLE CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[POSACC][CASH CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"CASH CODE DESCRIPTION",NULL);
		FINDRSCSETDEFTYPE(mtnrsrc,"[POSACC][PAYABLE CODE]",deftype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[POSACC][CASH CODE]",deftype2);
	}
	if(update)
	{
		updatersrc(mtnrsrc,"[POSACC][PAYABLE CODE]");
		updatersrc(mtnrsrc,"PAYABLE CODE DESCRIPTION");
		updatersrc(mtnrsrc,"[POSACC][CASH CODE]");
		updatersrc(mtnrsrc,"CASH CODE DESCRIPTION");
	}
}
static void changeDEFtype(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	int selectedd=0;
	short deftype1=0,deftype2=0;

	if(FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selectedd)) return;
	FINDRSCGETDEFTYPE(mtnrsrc,"[POSACC][ACCOUNT CODE]",&deftype1);
	deftype2=(short)selectedd;
	if(deftype1!=deftype2)
	{
		FINDRSCSETSTRING(mtnrsrc,"[POSACC][ACCOUNT CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[POSACC][DEBIT CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[POSACC][ENCUMBRANCE CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"ENCUMBRANCE CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[POSACC][RESERVE CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"RESERVE CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[POSACC][CREDIT CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
		FINDRSCSETDEFTYPE(mtnrsrc,"[POSACC][ACCOUNT CODE]",deftype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[POSACC][DEBIT CODE]",deftype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[POSACC][CREDIT CODE]",deftype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[POSACC][ENCUMBRANCE CODE]",deftype2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[POSACC][RESERVE CODE]",deftype2);
	}
	if(update)
	{
		updatersrc(mtnrsrc,"[POSACC][ACCOUNT CODE]");
		updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"[POSACC][DEBIT CODE]");
		updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"[POSACC][CREDIT CODE]");
		updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"[POSACC][ENCUMBRANCE CODE]");
		updatersrc(mtnrsrc,"ENCUMBRANCE CODE DESCRIPTION");
		updatersrc(mtnrsrc,"[POSACC][RESERVE CODE]");
		updatersrc(mtnrsrc,"RESERVE CODE DESCRIPTION");
	}
}
static void check_acctcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	check_acctcode(mtnrsrc,mtn,TRUE);
	updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static short check_acctcode(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL;
	short d=0;
	int x=0;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	d=(short)x;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",d);
	readwidget(mtnrsrc,"[POSACC][ACCOUNT CODE]");
	FINDRSCGETSTRING(mtnrsrc,"[POSACC][ACCOUNT CODE]",&acctcode);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"ACCOUNT CODE",acctcode);
	return(setacctcode(mtnrsrc,mtn,update));
}
static short setacctcode(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL,*desc=NULL,*edesc=NULL;

	COPYFIELD(MASTER->passkey->fileno,eaccnum,"DEFINITION TYPE");
	FINDFLDGETSTRING(MASTER->passkey->fileno,"ACCOUNT CODE",&acctcode);
	FINDFLDSETSTRING(eaccnum,"ACCOUNT CODE",acctcode);
	if(ADVEQLNRDsec(eaccnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(eaccnum,1);
		edesc=Rmalloc(RDAstrlen(acctcode)+114);
		sprintf(edesc,"The Account Code [%s] is invalid.  It doesn't exist. Please enter a correct account or cancel this operation.",(acctcode==NULL?"":acctcode));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID ACCOUNT CODE",edesc,NULL,NULL,TRUE,NULL);
		if(acctcode!=NULL) Rfree(acctcode)
		if(edesc!=NULL) Rfree(edesc);
		FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",NULL);
		if(update) updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
		return(FALSE);
	} else {
		if(acctcode!=NULL) Rfree(acctcode)
		FINDFLDGETSTRING(eaccnum,"DESCRIPTION",&desc);
		FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",desc);
		if(update) updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
		return(TRUE);
	}
}
static void check_enccodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	check_enccode(mtnrsrc,mtn,TRUE);
	updatersrc(mtnrsrc,"ENCUMBRANCE CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static short check_enccode(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL;
	short d=0;
	int x=0;
	char disenc=FALSE;

	FINDRSCGETCHAR(mtnrsrc,"[POSACC][DISENCUMBER]",&disenc);
	if(disenc==TRUE)
	{
		FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
		d=(short)x;
		FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",d);
		readwidget(mtnrsrc,"[POSACC][ENCUMBRANCE CODE]");
		FINDRSCGETSTRING(mtnrsrc,"[POSACC][ENCUMBRANCE CODE]",&acctcode);
		FINDFLDSETSTRING(MASTER->passkey->fileno,"ENCUMBRANCE CODE",acctcode);
		return(setenccode(mtnrsrc,mtn,update));
	} else {
		return(TRUE);
	}
}
static short setenccode(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL,*desc=NULL,*edesc=NULL;
	char disenc=FALSE;

	FINDFLDGETCHAR(MASTER->passkey->fileno,"DISENCUMBER",&disenc);
	if(disenc==TRUE)
	{
	COPYFIELD(MASTER->passkey->fileno,baccnum,"DEFINITION TYPE");
	FINDFLDGETSTRING(MASTER->passkey->fileno,"ENCUMBRANCE CODE",&acctcode);
	FINDFLDSETSTRING(baccnum,"ACCOUNT CODE",acctcode);
	if(ADVEQLNRDsec(baccnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(baccnum,1);
		edesc=Rmalloc(RDAstrlen(acctcode)+111);
		sprintf(edesc,"The Encumbrance Code [%s] is invalid.  It doesn't exist. Please enter a correct account or cancel this operation.",(acctcode==NULL?"":acctcode));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID ENCUMBRANCE CODE",edesc,NULL,NULL,TRUE,NULL);
		if(acctcode!=NULL) Rfree(acctcode)
		if(edesc!=NULL) Rfree(edesc);
		FINDRSCSETSTRING(mtnrsrc,"ENCUMBRANCE CODE DESCRIPTION",NULL);
		if(update) updatersrc(mtnrsrc,"ENCUMBRANCE CODE DESCRIPTION");
		return(FALSE);
	} else {
		if(acctcode!=NULL) Rfree(acctcode)
		FINDFLDGETSTRING(baccnum,"DESCRIPTION",&desc);
		FINDRSCSETSTRING(mtnrsrc,"ENCUMBRANCE CODE DESCRIPTION",desc);
		if(update) updatersrc(mtnrsrc,"ENCUMBRANCE CODE DESCRIPTION");
		return(TRUE);
	}
	} else {
		return(TRUE);
	}
}
static void check_rescodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	check_rescode(mtnrsrc,mtn,TRUE);
	updatersrc(mtnrsrc,"RESERVE CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static short check_rescode(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL;
	short d=0;
	int x=0;
	char disenc=FALSE;

	FINDRSCGETCHAR(mtnrsrc,"[POSACC][DISENCUMBER]",&disenc);
	if(disenc==TRUE)
	{
		FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
		d=(short)x;
		FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",d);
		readwidget(mtnrsrc,"[POSACC][RESERVE CODE]");
		FINDRSCGETSTRING(mtnrsrc,"[POSACC][RESERVE CODE]",&acctcode);
		FINDFLDSETSTRING(MASTER->passkey->fileno,"RESERVE CODE",acctcode);
		return(setrescode(mtnrsrc,mtn,update));
	} else {
		return(TRUE);
	}
}
static short setrescode(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL,*desc=NULL,*edesc=NULL;
	char disenc=FALSE;

	FINDFLDGETCHAR(MASTER->passkey->fileno,"DISENCUMBER",&disenc);
	if(disenc==TRUE)
	{
	COPYFIELD(MASTER->passkey->fileno,baccnum,"DEFINITION TYPE");
	FINDFLDGETSTRING(MASTER->passkey->fileno,"RESERVE CODE",&acctcode);
	FINDFLDSETSTRING(baccnum,"ACCOUNT CODE",acctcode);
	if(ADVEQLNRDsec(baccnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(baccnum,1);
		edesc=Rmalloc(RDAstrlen(acctcode)+111);
		sprintf(edesc,"The Reserve Code [%s] is invalid.  It doesn't exist. Please enter a correct account or cancel this operation.",(acctcode==NULL?"":acctcode));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID RESERVE CODE",edesc,NULL,NULL,TRUE,NULL);
		if(acctcode!=NULL) Rfree(acctcode)
		if(edesc!=NULL) Rfree(edesc);
		FINDRSCSETSTRING(mtnrsrc,"RESERVE CODE DESCRIPTION",NULL);
		if(update) updatersrc(mtnrsrc,"RESERVE CODE DESCRIPTION");
		return(FALSE);
	} else {
		if(acctcode!=NULL) Rfree(acctcode)
		FINDFLDGETSTRING(baccnum,"DESCRIPTION",&desc);
		FINDRSCSETSTRING(mtnrsrc,"RESERVE CODE DESCRIPTION",desc);
		if(update) updatersrc(mtnrsrc,"RESERVE CODE DESCRIPTION");
		return(TRUE);
	}
	} else {
		return(TRUE);
	}
}
static void check_debcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	check_debcode(mtnrsrc,mtn,TRUE);
	updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static short check_debcode(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL;
	short d=0;
	int x=0;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	d=(short)x;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",d);
	readwidget(mtnrsrc,"[POSACC][DEBIT CODE]");
	FINDRSCGETSTRING(mtnrsrc,"[POSACC][DEBIT CODE]",&acctcode);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"DEBIT CODE",acctcode);
	return(setdebitcode(mtnrsrc,mtn,update));
}
static short setdebitcode(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL,*desc=NULL,*edesc=NULL;

	COPYFIELD(MASTER->passkey->fileno,baccnum,"DEFINITION TYPE");
	FINDFLDGETSTRING(MASTER->passkey->fileno,"DEBIT CODE",&acctcode);
	FINDFLDSETSTRING(baccnum,"ACCOUNT CODE",acctcode);
	if(ADVEQLNRDsec(baccnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(baccnum,1);
		edesc=Rmalloc(RDAstrlen(acctcode)+111);
		sprintf(edesc,"The Debit Code [%s] is invalid.  It doesn't exist. Please enter a correct account or cancel this operation.",(acctcode==NULL?"":acctcode));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEBIT CODE",edesc,NULL,NULL,TRUE,NULL);
		if(acctcode!=NULL) Rfree(acctcode)
		if(edesc!=NULL) Rfree(edesc);
		FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",NULL);
		if(update) updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
		return(FALSE);
	} else {
		if(acctcode!=NULL) Rfree(acctcode)
		FINDFLDGETSTRING(baccnum,"DESCRIPTION",&desc);
		FINDRSCSETSTRING(mtnrsrc,"DEBIT CODE DESCRIPTION",desc);
		if(update) updatersrc(mtnrsrc,"DEBIT CODE DESCRIPTION");
		return(TRUE);
	}
}
static void check_payablecodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	check_payablecode(mtnrsrc,mtn,TRUE);
	updatersrc(mtnrsrc,"PAYABLE CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static short check_payablecode(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL;
	short d=0;
	int x=0;

	FINDRSCGETINT(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",&x);
	d=(short)x;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"PAYROLL FUND DEFINITION TYPE",d);
	readwidget(mtnrsrc,"[POSACC][PAYABLE CODE]");
	FINDRSCGETSTRING(mtnrsrc,"[POSACC][PAYABLE CODE]",&acctcode);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"PAYABLE CODE",acctcode);
	return(setpayablecode(mtnrsrc,mtn,update));
}
static short setpayablecode(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL,*desc=NULL,*edesc=NULL;

	FIELDCOPY(MASTER->passkey->fileno,"PAYROLL FUND DEFINITION TYPE",baccnum,
		"DEFINITION TYPE");
	FINDFLDGETSTRING(MASTER->passkey->fileno,"PAYABLE CODE",&acctcode);
	FINDFLDSETSTRING(baccnum,"ACCOUNT CODE",acctcode);
	if(ADVEQLNRDsec(baccnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(baccnum,1);
		edesc=Rmalloc(RDAstrlen(acctcode)+113);
		sprintf(edesc,"The Payable Code [%s] is invalid.  It doesn't exist. Please enter a correct account or cancel this operation.",(acctcode==NULL?"":acctcode));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PAYABLE CODE",edesc,NULL,NULL,TRUE,NULL);
		if(acctcode!=NULL) Rfree(acctcode)
		if(edesc!=NULL) Rfree(edesc);
		FINDRSCSETSTRING(mtnrsrc,"PAYABLE CODE DESCRIPTION",NULL);
		if(update) updatersrc(mtnrsrc,"PAYABLE CODE DESCRIPTION");
		return(FALSE);
	} else {
		if(acctcode!=NULL) Rfree(acctcode)
		FINDFLDGETSTRING(baccnum,"DESCRIPTION",&desc);
		FINDRSCSETSTRING(mtnrsrc,"PAYABLE CODE DESCRIPTION",desc);
		if(update) updatersrc(mtnrsrc,"PAYABLE CODE DESCRIPTION");
		return(TRUE);
	}
}
static void check_cashcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	check_cashcode(mtnrsrc,mtn,TRUE);
	updatersrc(mtnrsrc,"CASH CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static short check_cashcode(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL;
	short d=0;
	int x=0;

	FINDRSCGETINT(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",&x);
	d=(short)x;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"PAYROLL FUND DEFINITION TYPE",d);
	readwidget(mtnrsrc,"[POSACC][CASH CODE]");
	FINDRSCGETSTRING(mtnrsrc,"[POSACC][CASH CODE]",&acctcode);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"CASH CODE",acctcode);
	return(setcashcode(mtnrsrc,mtn,update));
}
static short setcashcode(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL,*desc=NULL,*edesc=NULL;

	FIELDCOPY(MASTER->passkey->fileno,"PAYROLL FUND DEFINITION TYPE",baccnum,
		"DEFINITION TYPE");
	FINDFLDGETSTRING(MASTER->passkey->fileno,"CASH CODE",&acctcode);
	FINDFLDSETSTRING(baccnum,"ACCOUNT CODE",acctcode);
	if(ADVEQLNRDsec(baccnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(baccnum,1);
		edesc=Rmalloc(RDAstrlen(acctcode)+113);
		sprintf(edesc,"The Cash Code [%s] is invalid.  It doesn't exist. Please enter a correct account or cancel this operation.",(acctcode==NULL?"":acctcode));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CASH CODE",edesc,NULL,NULL,TRUE,NULL);
		if(acctcode!=NULL) Rfree(acctcode)
		if(edesc!=NULL) Rfree(edesc);
		FINDRSCSETSTRING(mtnrsrc,"CASH CODE DESCRIPTION",NULL);
		if(update) updatersrc(mtnrsrc,"CASH CODE DESCRIPTION");
		return(FALSE);
	} else {
		if(acctcode!=NULL) Rfree(acctcode)
		FINDFLDGETSTRING(baccnum,"DESCRIPTION",&desc);
		FINDRSCSETSTRING(mtnrsrc,"CASH CODE DESCRIPTION",desc);
		if(update) updatersrc(mtnrsrc,"CASH CODE DESCRIPTION");
		return(TRUE);
	}
}
static void check_credcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	check_credcode(mtnrsrc,mtn,TRUE);
	updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static short check_credcode(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL;
	short d=0;
	int x=0;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	d=(short)x;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",d);
	readwidget(mtnrsrc,"[POSACC][CREDIT CODE]");
	FINDRSCGETSTRING(mtnrsrc,"[POSACC][CREDIT CODE]",&acctcode);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"CREDIT CODE",acctcode);
	return(setcreditcode(mtnrsrc,mtn,update));
}
static short setcreditcode(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL,*desc=NULL,*edesc=NULL;

	COPYFIELD(MASTER->passkey->fileno,baccnum,"DEFINITION TYPE");
	FINDFLDGETSTRING(MASTER->passkey->fileno,"CREDIT CODE",&acctcode);
	FINDFLDSETSTRING(baccnum,"ACCOUNT CODE",acctcode);
	if(ADVEQLNRDsec(baccnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(baccnum,1);
		edesc=Rmalloc(RDAstrlen(acctcode)+112);
		sprintf(edesc,"The Credit Code [%s] is invalid.  It doesn't exist. Please enter a correct account or cancel this operation.",(acctcode==NULL?"":acctcode));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CREDIT CODE",edesc,NULL,NULL,TRUE,NULL);
		if(acctcode!=NULL) Rfree(acctcode)
		if(edesc!=NULL) Rfree(edesc);
		FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",NULL);
		if(update) updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
		return(FALSE);
	} else {
		if(acctcode!=NULL) Rfree(acctcode)
		FINDFLDGETSTRING(baccnum,"DESCRIPTION",&desc);
		FINDRSCSETSTRING(mtnrsrc,"CREDIT CODE DESCRIPTION",desc);
		if(update) updatersrc(mtnrsrc,"CREDIT CODE DESCRIPTION");
		return(TRUE);
	}
}
static void check_codes(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	check_acctcode(mtnrsrc,mtn,update);
	check_debcode(mtnrsrc,mtn,update);
	check_payablecode(mtnrsrc,mtn,update);
	check_cashcode(mtnrsrc,mtn,update);
	check_credcode(mtnrsrc,mtn,update);
	check_enccode(mtnrsrc,mtn,update);
	check_rescode(mtnrsrc,mtn,update);
}
static void setcodes(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	setacctcode(mtnrsrc,mtn,update);
	setdebitcode(mtnrsrc,mtn,update);
	setpayablecode(mtnrsrc,mtn,update);
	setcashcode(mtnrsrc,mtn,update);
	setcreditcode(mtnrsrc,mtn,update);
	setenccode(mtnrsrc,mtn,update);
	setenccode(mtnrsrc,mtn,update);
}
void posaccm(short dowhich,void *targetkey)
{
	char *name=NULL;
	int acctno=0;
	short nofields=0,nokeys=0,x,y,edit_rsrc=TRUE;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDAacct *acc=NULL;
	RDArsrc *mtnrsrc=NULL;
	mtnstruct *mtn=NULL;

	mtnrsrc=RDArsrcNEW(MASTER->module,MASTER->maintain_screen);
	ZERNRD(MASTER->passkey->fileno);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->DEFtypes=NULL;
	mtn->PDEFtypes=NULL;
	mtn->previous=NULL;
	mtn->previous=RDATDataNEW(MASTER->passkey->fileno);
	mtn->DEFtypes=APPlibNEW();
	mtn->PDEFtypes=APPlibNEW();
	if(EXPENDITURE_ACCOUNT!=NULL)
	{
		if(EXPENDITURE_ACCOUNT->codetype!=NULL)
		{
			for(x=0,acc=EXPENDITURE_ACCOUNT->codetype;x<EXPENDITURE_ACCOUNT->num;++x,++acc)
			{
				addAPPlib(mtn->DEFtypes,acc->name);
			}
		}
	} 
	if(mtn->DEFtypes->numlibs<1)
	{
		addAPPlib(mtn->DEFtypes,"No Definitions Available");
	}
	if(BALANCE_SHEET_ACCOUNT!=NULL)
	{
		if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
		{
			for(x=0,acc=BALANCE_SHEET_ACCOUNT->codetype;x<BALANCE_SHEET_ACCOUNT->num;++x,++acc)
			{
				addAPPlib(mtn->PDEFtypes,acc->name);
			}
		}
	} 
	if(mtn->PDEFtypes->numlibs<1)
	{
		addAPPlib(mtn->PDEFtypes,"No Definitions Available");
	}
	nokeys=NUMKEYS(MASTER->passkey->fileno);
	keys=KEYPOINTER(MASTER->passkey->fileno);
	nofields=NUMFLDS(MASTER->passkey->fileno);
	fields=FLDPOINTER(MASTER->passkey->fileno);
	GET_MAINTAIN_INCVARS(mtnrsrc,MASTER);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	if(fields!=NULL)
	{
		for(y=0,fldx=fields;y<nofields;++y,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"DEFINITION TYPE"))
			{
				if(FIELDscrn(MASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					edit_rsrc=editable;
					if(mtn->DEFtypes->numlibs<2) edit_rsrc=FALSE;
					addlstrsrc(mtnrsrc,"DEFINITION TYPES"
						,&acctno,TRUE,DEFtypecb
						,mtn->DEFtypes->numlibs
						,&mtn->DEFtypes->libs,mtn);
				}
			} else if(!RDAstrcmp(fldx->name,"PAYROLL FUND DEFINITION TYPE"))
			{
				if(FIELDscrn(MASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					edit_rsrc=editable;
					if(mtn->PDEFtypes->numlibs<2) edit_rsrc=FALSE;
					addlstrsrc(mtnrsrc,"PAYROLL FUND DEFINITION TYPES"
						,&acctno,TRUE,PDEFtypecb
						,mtn->PDEFtypes->numlibs
						,&mtn->PDEFtypes->libs,mtn);
				}
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
	FINDRSCSETFUNC(mtnrsrc,"[POSACC][ACCOUNT CODE]",check_acctcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[POSACC][DEBIT CODE]",check_debcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[POSACC][CREDIT CODE]",check_credcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[POSACC][PAYABLE CODE]",check_payablecodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[POSACC][CASH CODE]",check_cashcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[POSACC][ENCUMBRANCE CODE]",check_enccodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[POSACC][RESERVE CODE]",check_rescodecb,mtn);
	addstdrsrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"DEBIT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"CREDIT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"PAYABLE CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"CASH CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"ENCUMBRANCE CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"RESERVE CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
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
					list2file(mtnrsrc,mtn,FALSE);
				} else {
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
					check_codes(mtnrsrc,mtn,FALSE);
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
				}
			} else {
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				check_codes(mtnrsrc,mtn,FALSE);
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
			}
			break;
		case 2:
			ApplyPassKey(MASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
			if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
				list2file(mtnrsrc,mtn,FALSE);
			} else {
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				check_codes(mtnrsrc,mtn,FALSE);
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
			}
			break;
		case 0:
		default:
			if(ADVEQLNRDKEYsec(MASTER->passkey->fileno,MASTER->passkey->keyno,targetkey,SCRNvirtualSubData,mtnrsrc))
			{
				ZERNRD(MASTER->passkey->fileno);
				ApplyPassKey(MASTER->passkey);
				if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
					list2file(mtnrsrc,mtn,FALSE);
				} else {
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
					check_codes(mtnrsrc,mtn,FALSE);
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
				}
			} else {
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				check_codes(mtnrsrc,mtn,FALSE);
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
			}
			break;
	}
	GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
	if(mbl_acc!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_acct,mtn);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,mtn);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,mtn);
	MakeMasterButtons(mtnrsrc,MASTER);
	computeallSCRNvirtuals(mtnrsrc);
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	if(mbl_acc!=NULL) FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_acc==NULL?TRUE:FALSE));
}
static void resetdefs(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	updateallrsrc(mtnrsrc);
	SETID(mtnrsrc,mtn);
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
static void save_check(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	char *desc=NULL,disenc=FALSE;
	int seldeftype=0;
	short deftype=0;
	char delflag=FALSE;

	FINDRSCGETCHAR(mtnrsrc,"[POSACC][DELETEFLAG]",&delflag);
	FINDRSCGETCHAR(mtnrsrc,"[POSACC][DISENCUMBER]",&disenc);
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
			if(check_acctcode(mtnrsrc,mtn,TRUE))
			{
				if(check_debcode(mtnrsrc,mtn,TRUE))
				{
					if(check_payablecode(mtnrsrc,mtn,TRUE))
					{
						if(check_credcode(mtnrsrc,mtn,TRUE))
						{
							if(check_cashcode(mtnrsrc,mtn,TRUE)==FALSE)
							{
								return;
							} else if(disenc)
							{
								if(check_enccode(mtnrsrc,mtn,TRUE)==FALSE)
								{
									return;
								} else if(check_rescode(mtnrsrc,mtn,TRUE)==FALSE)
								{
									return;
								}
							} 
						} else return;
					} else return;
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
				NULL,FALSE,2,mtnrsrc,mtn->previous,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	save_record(mtnrsrc,mtn,update_list);
}
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MASTER->passkey->fileno);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
	list2file(mtnrsrc,mtn,TRUE);
}
static void list2file(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{	
	int selected=0;
	short type=0;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selected);
	if(RDAstrcmp(mtn->DEFtypes->libs[selected],"No Definitions Available"))
	{
		type=(short)selected;
		FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",type);
	}
	setDEFtypes(mtnrsrc,mtn,update);
	FINDRSCGETINT(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",&selected);
	if(RDAstrcmp(mtn->PDEFtypes->libs[selected],"No Definitions Available"))
	{
		type=(short)selected;
		FINDFLDSETSHORT(MASTER->passkey->fileno,"PAYROLL FUND DEFINITION TYPE",type);
	}
	setPDEFtypes(mtnrsrc,mtn,update);
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
		updatebrowse(update_list,mbl_acc,MASTER->passkey->fileno,MASTER->module,mtnrsrc);
		if(mbl_acc!=NULL) quit_record(mtnrsrc,mtn);
		else GetRDATData(MASTER->passkey->fileno,mtn->previous);
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
