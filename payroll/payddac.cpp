/* payddac.c - Xpert Payroll Deduction Description Account Master */
/*lint -library */
#ifndef WIN32
#define __NAM__ "payddac.lnx"
#endif
#ifdef WIN32
#define __NAM__ "payddac.exe"
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
	char liability;
	char expensed;
	char disenc;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
static short ddmnum=(-1),baccnum=(-1);
static char editable=TRUE;
static MakeBrowseList *mbl_payddac=NULL;
static MaintainMaster *MASTER=NULL;
static void browse_payddac(RDArsrc *,mtnstruct *);
static void payddacm(short,void *);
static void quit_record(RDArsrc *,mtnstruct *);
static void quit_record_test(RDArsrc *,mtnstruct *);
static void save_record_quit(RDArsrc *,mtnstruct *);
static void save_record_now(RDArsrc *,mtnstruct *);
static void save_record_now1(RDArsrc *,mtnstruct *);
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void save_check(RDArsrc *,mtnstruct *,short);
static void DEFtypecb(RDArsrc *,mtnstruct *);
static void setDEFtypes(RDArsrc *,mtnstruct *,short);
static void PDEFtypecb(RDArsrc *,mtnstruct *);
static void changePDEFtype(RDArsrc *,mtnstruct *,short);
static void setPDEFtypes(RDArsrc *,mtnstruct *,short);
static char getsupportingrecords(RDArsrc *,mtnstruct *,short,short);
static void getrecordsetedit(RDArsrc *,mtnstruct *,short);
static short check_duefrom(RDArsrc *,mtnstruct *,short);
static void check_duefromcb(RDArsrc *,mtnstruct *);
static short check_cashcode(RDArsrc *,mtnstruct *,short);
static void check_cashcodecb(RDArsrc *,mtnstruct *);
static short check_liabcode(RDArsrc *,mtnstruct *,short);
static void check_liabcodecb(RDArsrc *,mtnstruct *);
static void check_codes(RDArsrc *,mtnstruct *,short);
static short setduefrom(RDArsrc *,mtnstruct *,short);
static short setcashcode(RDArsrc *,mtnstruct *,short);
static short setliabcode(RDArsrc *,mtnstruct *,short);
static void setcodes(RDArsrc *,mtnstruct *,short);
static void readscreen(RDArsrc *,mtnstruct *);
static void pdef2file(RDArsrc *,mtnstruct *,short);
static void def2file(RDArsrc *);
static void NEfunction(RDArsrc *,MaintainMaster *,void (*)(...),void *,mtnstruct *);
static void AUEQLfunction(RDArsrc *,MaintainMaster *,void (*)(...),void *,mtnstruct *);
static void SETID(RDArsrc *,mtnstruct *);
static void resetdefs(RDArsrc *,mtnstruct *);
static void getpreviouscb(RDArmem *,mtnstruct *);
static void getnextcb(RDArmem *,mtnstruct *);
static void SETFILES(RDArsrc *,mtnstruct *);
static char HasDUEFROM=FALSE;

static void getpreviouscb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1)) keyno=MASTER->passkey->keyno;
	def2file(member->parent);
	ADV2GET_PREVIOUS(member->parent,MASTER,mtn->previous,SCRNvirtualSubData,
		member->parent,keyno,NEfunction,mtn,NULL,NULL,
		AUEQLfunction,mtn);
}
static void getnextcb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1)) keyno=MASTER->passkey->keyno;
	def2file(member->parent);
	ADV2GET_NEXT(member->parent,MASTER,mtn->previous,SCRNvirtualSubData,
		member->parent,keyno,NEfunction,mtn,NULL,NULL,
		AUEQLfunction,mtn);
}
static void SETID(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	def2file(mtnrsrc);
	ADV4SET_KEYFUNC(mtnrsrc,MASTER,mtn->previous,SCRNvirtualSubData,
		(void *)mtnrsrc,NEfunction,mtn,NULL,NULL,AUEQLfunction,mtn);
}
static void SETFILES(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	def2file(mtnrsrc);
	pdef2file(mtnrsrc,mtn,TRUE);
	SET_MTNMASTER(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
}
void setDEFtypes(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
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
 	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYDDAC][ACCOUNT CODE IMAGE]",deftype1);
 	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYDDAC][DEBIT CODE IMAGE]",deftype1);
 	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYDDAC][CREDIT CODE IMAGE]",deftype1);
	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYDDAC][ENCUMBRANCE CODE IMAGE]",deftype1);
	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYDDAC][RESERVE CODE IMAGE]",deftype1);
 	if(!FINDRSCLISTAPPlib(mtnrsrc,"DEFINITION TYPES",selectedd,mtn->DEFtypes))
 	{
 		if(update) updatersrc(mtnrsrc,"DEFINITION TYPES");
 	}
}
void setPDEFtypes(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
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
 	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYDDAC][CASH CODE]",deftype1);
	if(HasDUEFROM) FINDRSCSETDEFTYPE(mtnrsrc,"[PAYDDAC][DUE FROM]",deftype1);
 	FINDRSCSETDEFTYPE(mtnrsrc,"[PAYDDAC][LIABILITY CODE]",deftype1);
 	if(!FINDRSCLISTAPPlib(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",selectedd,mtn->PDEFtypes))
 	{
 		if(update) updatersrc(mtnrsrc,"PAYROLL FUND DEFINITION TYPES");
 	}
}
static void check_duefromcb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	check_duefrom(mtnrsrc,mtn,TRUE);
	updatersrc(mtnrsrc,"DUE FROM DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static short check_duefrom(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL;
	int d=0;
	short x=0;

	FINDRSCGETINT(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",&d);
	x=(short)d;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"PAYROLL FUND DEFINITION TYPE",x);
	FIELDCOPY(MASTER->passkey->fileno,"PAYROLL FUND DEFINITION TYPE",baccnum,"DEFINITION TYPE");
	readwidget(mtnrsrc,"[PAYDDAC][DUE FROM]");
	FINDRSCGETSTRING(mtnrsrc,"[PAYDDAC][DUE FROM]",&acctcode);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"DUE FROM",acctcode);
	if(acctcode!=NULL) Rfree(acctcode);
	return(setduefrom(mtnrsrc,mtn,update));
}
static short setduefrom(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL,*desc=NULL;

	FIELDCOPY(MASTER->passkey->fileno,"PAYROLL FUND DEFINITION TYPE",baccnum,"DEFINITION TYPE");
	FINDFLDGETSTRING(MASTER->passkey->fileno,"DUE FROM",&acctcode);
	FINDFLDSETSTRING(baccnum,"ACCOUNT CODE",acctcode);
	FINDFLDSETSTRING(baccnum,"ACCOUNT CODE",acctcode);
	if(ADVEQLNRDsec(baccnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(baccnum,1);
	}
	if(acctcode!=NULL) Rfree(acctcode)
	FINDFLDGETSTRING(baccnum,"DESCRIPTION",&desc);
	FINDRSCSETSTRING(mtnrsrc,"DUE FROM DESCRIPTION",desc);
	if(desc!=NULL) Rfree(desc);
	if(update) updatersrc(mtnrsrc,"DUE FROM DESCRIPTION");
	return(TRUE);
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
	int d=0;
	short x=0;

	FINDRSCGETINT(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",&d);
	x=(short)d;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"PAYROLL FUND DEFINITION TYPE",x);
	FIELDCOPY(MASTER->passkey->fileno,"PAYROLL FUND DEFINITION TYPE",baccnum,"DEFINITION TYPE");
	readwidget(mtnrsrc,"[PAYDDAC][CASH CODE]");
	FINDRSCGETSTRING(mtnrsrc,"[PAYDDAC][CASH CODE]",&acctcode);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"CASH CODE",acctcode);
	if(acctcode!=NULL) Rfree(acctcode);
	return(setcashcode(mtnrsrc,mtn,update));
}
static short setcashcode(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL,*desc=NULL;

	FIELDCOPY(MASTER->passkey->fileno,"PAYROLL FUND DEFINITION TYPE",baccnum,"DEFINITION TYPE");
	FINDFLDGETSTRING(MASTER->passkey->fileno,"CASH CODE",&acctcode);
	FINDFLDSETSTRING(baccnum,"ACCOUNT CODE",acctcode);
	FINDFLDSETSTRING(baccnum,"ACCOUNT CODE",acctcode);
	if(ADVEQLNRDsec(baccnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(baccnum,1);
	}
	if(acctcode!=NULL) Rfree(acctcode)
	FINDFLDGETSTRING(baccnum,"DESCRIPTION",&desc);
	FINDRSCSETSTRING(mtnrsrc,"CASH CODE DESCRIPTION",desc);
	if(desc!=NULL) Rfree(desc);
	if(update) updatersrc(mtnrsrc,"CASH CODE DESCRIPTION");
	return(TRUE);
}
static void check_liabcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	check_liabcode(mtnrsrc,mtn,TRUE);
	updatersrc(mtnrsrc,"LIABILITY CODE DESCRIPTION");
	updateSCRNvirtuals(mtnrsrc);
}
static short check_liabcode(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL;
	int d=0;
	short x=0;

	FINDRSCGETINT(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",&d);
	x=(short)d;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"PAYROLL FUND DEFINITION TYPE",x);
	FIELDCOPY(MASTER->passkey->fileno,"PAYROLL FUND DEFINITION TYPE",baccnum,
		"DEFINITION TYPE");
	readwidget(mtnrsrc,"[PAYDDAC][LIABILITY CODE]");
	FINDRSCGETSTRING(mtnrsrc,"[PAYDDAC][LIABILITY CODE]",&acctcode);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"LIABILITY CODE",acctcode);
	if(acctcode!=NULL) Rfree(acctcode);
	return(setliabcode(mtnrsrc,mtn,update));
}
static short setliabcode(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *acctcode=NULL,*desc=NULL;

	FIELDCOPY(MASTER->passkey->fileno,"PAYROLL FUND DEFINITION TYPE",baccnum,
		"DEFINITION TYPE");
	FINDFLDGETSTRING(MASTER->passkey->fileno,"LIABILITY CODE",&acctcode);
	FINDFLDSETSTRING(baccnum,"ACCOUNT CODE",acctcode);
	FINDFLDSETSTRING(baccnum,"ACCOUNT CODE",acctcode);
	if(ADVEQLNRDsec(baccnum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(baccnum,1);
	if(acctcode!=NULL) Rfree(acctcode)
	FINDFLDGETSTRING(baccnum,"DESCRIPTION",&desc);
	FINDRSCSETSTRING(mtnrsrc,"LIABILITY CODE DESCRIPTION",desc);
	if(desc!=NULL) Rfree(desc);
	if(update) updatersrc(mtnrsrc,"LIABILITY CODE DESCRIPTION");
	return(TRUE);
}
static void check_codes(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
        check_cashcode(mtnrsrc,mtn,update);
        check_liabcode(mtnrsrc,mtn,update);
}
static void setcodes(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
        setcashcode(mtnrsrc,mtn,update);
        setliabcode(mtnrsrc,mtn,update);
}
static char getsupportingrecords(RDArsrc *mtnrsrc,mtnstruct *mtn,short update,short display_diag)
{
	char *dedid=NULL,*warnmessage=NULL;
	char delflag=FALSE,return_value=TRUE;

	if(ddmnum!=(-1))
	{
		COPYFIELD(MASTER->passkey->fileno,ddmnum,"DEDUCTION DESCRIPTION IDENTIFICATION");
		if(ADVEQLNRDsec(ddmnum,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(ddmnum,1);
			if(update) updatefilerecord2rsrc(ddmnum,mtnrsrc);
			else filerecord2rsrc(ddmnum,mtnrsrc);
			mtn->liability=TRUE;
			mtn->expensed=TRUE;
			mtn->disenc=TRUE;
			if(display_diag)
			{
				warnmessage=Rmalloc(500);
				sprintf(warnmessage,"The Deduction Description Identification of [%s] is an invalid value.  It does not exist in the [PAYROLL][PAYDDMS] file.  Please select a valid Deduction Description Identification to save this record.",(dedid==NULL?"":dedid));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEDUCTION DESCRIPTION IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				if(dedid!=NULL) Rfree(dedid);
				return_value=FALSE;
			}
		} else {
			if(update) updatefilerecord2rsrc(ddmnum,mtnrsrc);
			else filerecord2rsrc(ddmnum,mtnrsrc);
			FINDFLDGETCHAR(ddmnum,"LIABILITY",&mtn->liability);
			FINDFLDGETCHAR(ddmnum,"EXPENSED",&mtn->expensed);
			FINDFLDGETCHAR(ddmnum,"DISENCUMBER",&mtn->disenc);
			if(display_diag)
			{
				FINDFLDGETCHAR(ddmnum,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(500);
					sprintf(warnmessage,"The Deduction Description Identification of [%s] is deleted value.  It exists in the [PAYROLL][PAYDDMS] but is in a deleted state.  Please select a valid Deduction Description Identification to save this record.",(dedid==NULL?"":dedid)); 
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED DEDUCTION DESCRIPTION IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					if(warnmessage!=NULL) Rfree(warnmessage);
					if(dedid!=NULL) Rfree(dedid);
					return_value=FALSE;
				}
			}
		}
	}
	if(!mtn->liability)
	{
		FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDAC][CASH CODE]",FALSE);
		if(HasDUEFROM) FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDAC][DUE FROM]",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDAC][LIABILITY CODE]",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[PAYDDAC][CASH CODE]",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[PAYDDAC][LIABILITY CODE]",FALSE);
	} else {
		if(HasDUEFROM) FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDAC][DUE FROM]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDAC][CASH CODE]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDAC][LIABILITY CODE]",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[PAYDDAC][CASH CODE]",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[PAYDDAC][LIABILITY CODE]",TRUE);
	}
	FINDRSCSETSENSITIVE(mtnrsrc,"[PAYDDAC][ENCUMBRANCE CODE IMAGE]",FALSE);
	FINDRSCSETSENSITIVE(mtnrsrc,"[PAYDDAC][RESERVE CODE IMAGE]",FALSE);
	FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDAC][ENCUMBRANCE CODE IMAGE]",FALSE);
	FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDAC][RESERVE CODE IMAGE]",FALSE);
	if(!mtn->expensed)
	{
		FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDAC][ACCOUNT CODE IMAGE]",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDAC][DEBIT CODE IMAGE]",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDAC][CREDIT CODE IMAGE]",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[PAYDDAC][ACCOUNT CODE IMAGE]",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[PAYDDAC][DEBIT CODE IMAGE]",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[PAYDDAC][CREDIT CODE IMAGE]",FALSE);
	} else {
		if(mtn->disenc)
		{
			FINDRSCSETSENSITIVE(mtnrsrc,"[PAYDDAC][ENCUMBRANCE CODE IMAGE]",TRUE);
			FINDRSCSETSENSITIVE(mtnrsrc,"[PAYDDAC][RESERVE CODE IMAGE]",TRUE);
			FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDAC][ENCUMBRANCE CODE IMAGE]",TRUE);
			FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDAC][RESERVE CODE IMAGE]",TRUE);
		}
		FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDAC][ACCOUNT CODE IMAGE]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDAC][DEBIT CODE IMAGE]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDAC][CREDIT CODE IMAGE]",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[PAYDDAC][ACCOUNT CODE IMAGE]",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[PAYDDAC][DEBIT CODE IMAGE]",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"[PAYDDAC][CREDIT CODE IMAGE]",TRUE);
	}
	return(return_value);
}
void payddacm(short dowhich,void *targetkey)
{
	char *name=NULL;
	int x=0,defno=0,y=0;
	RDAacct *acc=NULL;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	short nofields=0,nokeys=0,edit_rsrc=FALSE;
	mtnstruct *mtn=NULL;
	RDArsrc *mtnrsrc=NULL;
	
	mtnrsrc=RDArsrcNEW(MASTER->module,MASTER->maintain_screen);
	ZERNRD(MASTER->passkey->fileno);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->DEFtypes=NULL;
	mtn->PDEFtypes=NULL;
	mtn->liability=TRUE;
	mtn->expensed=TRUE;
	mtn->disenc=TRUE;
	mtn->previous=NULL;
	mtn->previous=RDATDataNEW(MASTER->passkey->fileno);;
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
				addlstrsrc(mtnrsrc,"DEFINITION TYPES",
					&defno,TRUE,DEFtypecb,
					mtn->DEFtypes->numlibs,
					&mtn->DEFtypes->libs,mtn);
			} else if(!RDAstrcmp(fldx->name,"PAYROLL FUND DEFINITION TYPE"))
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
				addlstrsrc(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",
					&defno,TRUE,PDEFtypecb,
					mtn->PDEFtypes->numlibs,
					&mtn->PDEFtypes->libs,mtn);
			} else {
				edit_rsrc=(short)(editable && RDAstrcmp(fldx->name,"AMOUNT"));
				nonlstmakefld(mtnrsrc,MASTER->passkey->fileno,fldx,edit_rsrc);
				if(name!=NULL)
				{
					name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(MASTER->mainfile));
				} else {
					name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(MASTER->mainfile));
				}
				sprintf(name,"[%s][%s]",MASTER->mainfile,fldx->name);
				FINDRSCSETFUNC(mtnrsrc,name,SETFILES,mtn);
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
					if(RDAstrcmp(part->name,"DEFINITION TYPE"))
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
	}
	if(name!=NULL) Rfree(name);
	FINDRSCSETFUNC(mtnrsrc,"[PAYDDAC][CASH CODE]",check_cashcodecb,mtn);
	if(HasDUEFROM) FINDRSCSETFUNC(mtnrsrc,"[PAYDDAC][DUE FROM]",check_duefromcb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[PAYDDAC][LIABILITY CODE]",check_liabcodecb,mtn);
	addstdrsrc(mtnrsrc,"CASH CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	if(HasDUEFROM) addstdrsrc(mtnrsrc,"DUE FROM DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"LIABILITY CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	SET_MAINTAIN_FILE2RSRCS(mtnrsrc,MASTER);
	if(ddmnum!=(-1)) file2rsrc(ddmnum,mtnrsrc,FALSE);
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
					pdef2file(mtnrsrc,mtn,FALSE);
					getsupportingrecords(mtnrsrc,mtn,FALSE,FALSE);
				} else {
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					getrecordsetedit(mtnrsrc,mtn,FALSE);
					getsupportingrecords(mtnrsrc,mtn,FALSE,FALSE);
					check_codes(mtnrsrc,mtn,FALSE);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				getrecordsetedit(mtnrsrc,mtn,FALSE);
				getsupportingrecords(mtnrsrc,mtn,FALSE,FALSE);
				check_codes(mtnrsrc,mtn,FALSE);
			}
			break;
		case 2:
			ApplyPassKey(MASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
			if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
				pdef2file(mtnrsrc,mtn,FALSE);
				getsupportingrecords(mtnrsrc,mtn,FALSE,FALSE);
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				getrecordsetedit(mtnrsrc,mtn,FALSE);
				getsupportingrecords(mtnrsrc,mtn,FALSE,FALSE);
				check_codes(mtnrsrc,mtn,FALSE);
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
					pdef2file(mtnrsrc,mtn,FALSE);
					getsupportingrecords(mtnrsrc,mtn,FALSE,FALSE);
				} else {
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					getrecordsetedit(mtnrsrc,mtn,FALSE);
					getsupportingrecords(mtnrsrc,mtn,FALSE,FALSE);
					check_codes(mtnrsrc,mtn,FALSE);
				}
			}  else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				getrecordsetedit(mtnrsrc,mtn,FALSE);
				getsupportingrecords(mtnrsrc,mtn,FALSE,FALSE);
				check_codes(mtnrsrc,mtn,FALSE);
			}
			break;
	}
	GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
	if(mbl_payddac!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
	}
	FINDRSCSETCALLBACKTYPE(mtnrsrc,"[PAYDDAC][ACCOUNT CODE IMAGE]",TRUE);
	FINDRSCSETCALLBACKTYPE(mtnrsrc,"[PAYDDAC][DEBIT CODE IMAGE]",TRUE);
	FINDRSCSETCALLBACKTYPE(mtnrsrc,"[PAYDDAC][ENCUMBRANCE CODE IMAGE]",TRUE);
	FINDRSCSETCALLBACKTYPE(mtnrsrc,"[PAYDDAC][RESERVE CODE IMAGE]",TRUE);
	FINDRSCSETCALLBACKTYPE(mtnrsrc,"[PAYDDAC][CREDIT CODE IMAGE]",TRUE);
	FINDRSCSETCALLBACKTYPE(mtnrsrc,"[PAYDDAC][LIABILITY CODE]",TRUE);
	FINDRSCSETCALLBACKTYPE(mtnrsrc,"[PAYDDAC][CASH CODE]",TRUE);
	if(HasDUEFROM) FINDRSCSETCALLBACKTYPE(mtnrsrc,"[PAYDDAC][DUE FROM]",TRUE);
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_payddac,mtn);
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
	if(mbl_payddac!=NULL) FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_payddac==NULL?TRUE:FALSE));
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
	def2file(mtnrsrc);
	pdef2file(mtnrsrc,mtn,TRUE);
}
static void pdef2file(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	int selected=0;
	short type=0;

	FINDRSCGETINT(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",&selected);
	if(RDAstrcmp(mtn->PDEFtypes->libs[selected],"No Definitions Available"))
	{
		type=(short)selected;
		FINDFLDSETSHORT(MASTER->passkey->fileno,"PAYROLL FUND DEFINITION TYPE",type);
	}
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
	char delflag=FALSE,*desc=NULL;

	FINDRSCGETCHAR(mtnrsrc,"[PAYDDAC][DELETEFLAG]",&delflag);
	if(!delflag)
	{
		if(!getsupportingrecords(mtnrsrc,mtn,TRUE,TRUE))
		{
			return;
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
		updatebrowse(update_list,mbl_payddac,MASTER->passkey->fileno,MASTER->module,mtnrsrc);
		if(mbl_payddac!=NULL) quit_record(mtnrsrc,mtn);
		else GetRDATData(MASTER->passkey->fileno,mtn->previous);
	}
}
static void save_record_now(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_record(mtnrsrc,mtn,TRUE);
}
static void save_record_now1(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
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
static void DEFtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int seld=0;
	short d1=0,d2=0;

	if(FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&seld)) return;
	FINDRSCGETDEFTYPE(mtnrsrc,"[PAYDDAC][ACCOUNT CODE IMAGE]",&d1);
	d2=(short)seld;
	if(d1!=d2)
	{
		FINDRSCSETSTRING(mtnrsrc,"[PAYDDAC][ACCOUNT CODE IMAGE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[PAYDDAC][DEBIT CODE IMAGE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[PAYDDAC][CREDIT CODE IMAGE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[PAYDDAC][ENCUMBRANCE CODE IMAGE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[PAYDDAC][RESERVE CODE IMAGE]",NULL);
		FINDRSCSETDEFTYPE(mtnrsrc,"[PAYDDAC][ACCOUNT CODE IMAGE]",d2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[PAYDDAC][DEBIT CODE IMAGE]",d2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[PAYDDAC][CREDIT CODE IMAGE]",d2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[PAYDDAC][ENCUMBRANCE CODE IMAGE]",d2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[PAYDDAC][RESERVE CODE IMAGE]",d2);
	}
	updatersrc(mtnrsrc,"[PAYDDAC][ACCOUNT CODE IMAGE]");
	updatersrc(mtnrsrc,"[PAYDDAC][DEBIT CODE IMAGE]");
	updatersrc(mtnrsrc,"[PAYDDAC][CREDIT CODE IMAGE]");
	updatersrc(mtnrsrc,"[PAYDDAC][ENCUMBRANCE CODE IMAGE]");
	updatersrc(mtnrsrc,"[PAYDDAC][RESERVE CODE IMAGE]");
	SETID(mtnrsrc,mtn);
}
static void changePDEFtype(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	int seld=0;
	short d1=0,d2=0;

	if(FINDRSCGETINT(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",&seld)) return;
	FINDRSCGETDEFTYPE(mtnrsrc,"[PAYDDAC][CASH CODE]",&d1);
	d2=(short)seld;
	if(d1!=d2)
	{
		if(HasDUEFROM) FINDRSCSETSTRING(mtnrsrc,"[PAYDDAC][DUE FROM]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[PAYDDAC][CASH CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[PAYDDAC][LIABILITY CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"CASH CODE DESCRIPTION",NULL);
		FINDRSCSETSTRING(mtnrsrc,"LIABILITY CODE DESCRIPTION",NULL);
		if(HasDUEFROM) FINDRSCSETDEFTYPE(mtnrsrc,"[PAYDDAC][DUE FROM]",d2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[PAYDDAC][CASH CODE]",d2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[PAYDDAC][LIABILITY CODE]",d2);
	}
	if(update)
	{
		if(HasDUEFROM) updatersrc(mtnrsrc,"[PAYDDAC][DUE FROM]");
		updatersrc(mtnrsrc,"[PAYDDAC][CASH CODE]");
		updatersrc(mtnrsrc,"[PAYDDAC][LIABILITY CODE]");
		updatersrc(mtnrsrc,"CASH CODE DESCRIPTION");
		if(HasDUEFROM) updatersrc(mtnrsrc,"DUE FROM DESCRIPTION");
		updatersrc(mtnrsrc,"LIABILITY CODE DESCRIPTION");
	}
}
static void PDEFtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readallwidgets(mtnrsrc);
	changePDEFtype(mtnrsrc,mtn,TRUE);
	updateSCRNvirtuals(mtnrsrc);
}
static void AUEQLfunction(RDArsrc *mtnrsrc,MaintainMaster *MASTER,void (*SubFunc)(...), void *args,mtnstruct *mtn)
{
	getrecordsetedit(mtnrsrc,mtn,TRUE);
	getsupportingrecords(mtnrsrc,mtn,TRUE,TRUE);
	setcodes(mtnrsrc,mtn,TRUE);
}
static void NEfunction(RDArsrc *mtnrsrc,MaintainMaster *MASTER,void (*SubFunc)(...),
	void *args,mtnstruct *mtn)
{
	seteditfilersrc(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,TRUE,FALSE);
	pdef2file(mtnrsrc,mtn,TRUE);
	getsupportingrecords(mtnrsrc,mtn,TRUE,FALSE);
}
static void def2file(RDArsrc *mtnrsrc)
{
	int defno=0;
	short d=0;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&defno);
	d=(short)defno;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",d);
}
static void getrecordsetedit(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	int seldeftype=0;
	short deftype1=0;

	filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
	FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFINITION TYPE",&deftype1);
	seldeftype=(int)deftype1;
	FINDRSCSETINT(mtnrsrc,"DEFINITION TYPES",seldeftype);
	FINDFLDGETSHORT(MASTER->passkey->fileno,"PAYROLL FUND DEFINITION TYPE",&deftype1);
	seldeftype=(int)deftype1;
	FINDRSCSETINT(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",seldeftype);
	FINDRSCSETSENSITIVE(mtnrsrc,"[PAYDDAC][CASH CODE]",TRUE);
	if(HasDUEFROM) FINDRSCSETSENSITIVE(mtnrsrc,"[PAYDDAC][DUE FROM]",TRUE);
	FINDRSCSETSENSITIVE(mtnrsrc,"[PAYDDAC][LIABILITY CODE]",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDAC][CASH CODE]",TRUE);
	if(HasDUEFROM) FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDAC][DUE FROM]",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"[PAYDDAC][LIABILITY CODE]",TRUE);
	setDEFtypes(mtnrsrc,mtn,update);
	setPDEFtypes(mtnrsrc,mtn,update);
	if(update)
	{
		updateeditfilersrcsnokeys(MASTER->passkey->fileno,
			MASTER->passkey->keyno,mtnrsrc,TRUE);
	} else {
		seteditfilersrc(MASTER->passkey->fileno,MASTER->passkey->keyno,
			mtnrsrc,TRUE,FALSE);
	}
}
void browse_payddac(RDArsrc *mtnrsrc,mtnstruct *mtn)
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
	GET_BROWSE_SUPPORTING(mainrsrc,searchrsrc,definelist,MASTER,SCRNvirtualSubData,(void *)mainrsrc);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	addDFincvir(mainrsrc,MASTER->module,"PAYDDMS",NULL,ddmnum);
	if(ddmnum!=(-1))
	{
		COPYFIELD(MASTER->passkey->fileno,ddmnum,"DEDUCTION DESCRIPTION IDENTIFICATION");
		if(ADVEQLNRDsec(ddmnum,1,SCRNvirtualSubData,mainrsrc))
		{
			ZERNRD(ddmnum);
		} else {
			file2rsrc(ddmnum,mainrsrc,FALSE);
			filerecord2rsrc(ddmnum,mainrsrc);
		}
	}
	MakeRunFunction(mainrsrc,MASTER);
	mbl_payddac=ADVPbrowse(MASTER->passkey->fileno,MASTER->passkey->keyno,mainrsrc,
		searchrsrc,definelist,doexit,okfunc,new_record,
		NULL,FALSE,NULL,0,0,2,0,MASTER->passkey);
}
int main(int argc,char **argv)
{
	short filenum=(-1);

	if(InitializeSubsystems(argc,argv,"PAYROLL","MTN PAYDDAC")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	MASTER=MaintainMasterNew("PAYROLL","MTN PAYDDAC");
	if(findMaintainMaster(MASTER))
	{
		ERRORDIALOG("MAINTAIN MASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE);
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN PAYDDAC",MASTER->module);
		doexit(NULL);
	} else {
		if((filenum=APPOPNNRDEDITABLE(MASTER->module,"PAYDDAC",&editable))==(-1))
		{
			doexit(NULL);
			return;
		}
		if(FLDNRD(filenum,"DUE FROM")!=NULL) HasDUEFROM=TRUE;
			else HasDUEFROM=FALSE;
		if(!OPEN_MASTER_FILES(MASTER))
		{
			doexit(NULL);
			return;
		}
		baccnum=APPReturnSupportingFileno(MASTER,"FINMGT","FINBACC",1);
		ddmnum=APPReturnSupportingFileno(MASTER,MASTER->module,"PAYDDMS",1);
		MASTER->passkey=READPassKey(filenum);
		MASTER->inheritrsrcs=TRUE;
		switch(MASTER->start_with)
		{
			default:
			case 0:
				payddacm(1,NULL);
				break;
			case 1:
				browse_payddac(NULL,NULL);
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
	if(mbl_payddac!=NULL) mbl_payddac=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func))
	{
		if(!run_func)
		{
			payddacm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MASTER);
		}
	} else {
		payddacm(0,targetkey);
	}
}
static void new_record(RDArsrc *mtnrsrc,MakeBrowseList *blist)
{
	payddacm(2,NULL);
} 
