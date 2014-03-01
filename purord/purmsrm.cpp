/* purmsrm.c - Purchase Order Master Maintain Screen */
#include <cstdio>


#include <mix.hpp>
#include <fin.hpp>
#include <venpmt.hpp>
#include <purord.hpp>

struct mtnstructs
{
	APPlib *addresslist;
	char *POID;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern short venadr,venadr2,purids,vennum;
extern void browse_purmsr(RDArsrc *,mtnstruct *);
extern MaintainMaster *MASTER;
extern MakeBrowseList *mbl_purmsr;
extern char editable;
extern RDApurord *purord;
extern RDAvenpmt *venpmt;
static void save_check(RDArsrc *,mtnstruct *,short);
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_record_quit(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void save_record_now(RDArsrc *,mtnstruct *);
static void save_record_now1(RDArsrc *,mtnstruct *);
static void quit_record(RDArsrc *,mtnstruct *);
static void quit_record_test(RDArsrc *,mtnstruct *);
static void list2file(RDArsrc *,mtnstruct *);
static void readscreen(RDArsrc *,mtnstruct *);
static void resetdefs(RDArsrc *,mtnstruct *);
static void getpreviouscb(RDArmem *,mtnstruct *);
static void getnextcb(RDArmem *,mtnstruct *);
static void SETID(RDArsrc *,mtnstruct *);
static void SETFILES(RDArsrc *,MaintainMaster *);
static void setaddresses(RDArsrc *,mtnstruct *,short);
static void changePOADDRlist(RDArsrc *,mtnstruct *);
static void changeBILLADDRlist(RDArsrc *,mtnstruct *);
static void setlists(RDArsrc *,mtnstruct *,short);
static void setlistscb(RDArsrc *,MaintainMaster *,void (*)(...),void *,mtnstruct *);
static void default_addresses(RDArsrc *,mtnstruct *,short);
static void SetAppDef(RDArsrc *,mtnstruct *,short);
static void SetAppDefcb(RDArsrc *,MaintainMaster *,void (*)(...),void *,mtnstruct *);
extern short venadrkeyno;
static void check_vendorcb(RDArsrc *,mtnstruct *);
static short check_vendor(RDArsrc *,mtnstruct *);
static void seteditable(RDArsrc *,mtnstruct *,short);
static void seteditablecb(RDArsrc *,MaintainMaster *,void (*)(...),void *,mtnstruct *);
static void get_POIDS(RDArsrc *,mtnstruct *);
void purmsrm(short,void *targetkey);

void purmsrm(short dowhich,void *targetkey)
{
	char *name=NULL;
	short edit_rsrc=TRUE,nofields=0,nokeys=0,x=0,y=0;
	int which_type=0;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDArsrc *mtnrsrc=NULL;
	mtnstruct *mtn=NULL;

	mtnrsrc=RDArsrcNEW(MASTER->module,MASTER->maintain_screen);
	ZERNRD(MASTER->passkey->fileno);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->previous=RDATDataNEW(MASTER->passkey->fileno);
	mtn->addresslist=NULL;
	mtn->addresslist=APPlibNEW();
	mtn->POID=NULL;
	addAPPlib(mtn->addresslist,"No Addresses Defined");
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
			if(!RDAstrcmp(fldx->name,"PO VENDOR ADDRESS IDENTIFICATION"))
			{
				if(FIELDscrn(MASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					which_type=0;
					addlstrsrc(mtnrsrc,
						"PO VENDOR ADDRESS IDENTIFICATIONS",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						changePOADDRlist,
						mtn->addresslist->numlibs,
						&mtn->addresslist->libs,mtn);
				}
			} else if(!RDAstrcmp(fldx->name,"BILLING VENDOR ADDRESS IDENTIFICATION"))
			{
				if(FIELDscrn(MASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					which_type=0;
					addlstrsrc(mtnrsrc,
						"BILLING VENDOR ADDRESS IDENTIFICATIONS",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						changeBILLADDRlist,
						mtn->addresslist->numlibs,
						&mtn->addresslist->libs,mtn);
				}
			} else {
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
	SET_MAINTAIN_FILE2RSRCS(mtnrsrc,MASTER);
	SCRNvirtual2rsrc(mtnrsrc);
	DefaultScreens(mtnrsrc);
	FINDRSCSETFUNC(mtnrsrc,"[PURMST][VENDOR IDENTIFICATION]",check_vendorcb,mtn);
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
					SetAppDef(mtnrsrc,mtn,FALSE);
				} else {
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					setlists(mtnrsrc,mtn,FALSE);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
					seteditable(mtnrsrc,mtn,FALSE);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				setlists(mtnrsrc,mtn,FALSE);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				seteditable(mtnrsrc,mtn,FALSE);
			}
			break;
		case 2:
			ApplyPassKey(MASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
			if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
				SetAppDef(mtnrsrc,mtn,FALSE);
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				setlists(mtnrsrc,mtn,FALSE);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				seteditable(mtnrsrc,mtn,FALSE);
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
					SetAppDef(mtnrsrc,mtn,FALSE);
				} else {
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					setlists(mtnrsrc,mtn,FALSE);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
					seteditable(mtnrsrc,mtn,FALSE);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				setlists(mtnrsrc,mtn,FALSE);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				seteditable(mtnrsrc,mtn,FALSE);
			}
	}
	GET_SUPPORTING(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
	if(mbl_purmsr!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_purmsr,mtn);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,mtn);
	addbtnrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,mtn);
	MakeMasterButtons(mtnrsrc,MASTER);
	computeallSCRNvirtuals(mtnrsrc);
	FINDRSCSETEDITABLE(mtnrsrc,"[PURMST][OUTSTANDING AMOUNT]",FALSE);
	if(mbl_purmsr!=NULL)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	}
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,
		(mbl_purmsr==NULL?TRUE:FALSE));
}
static void resetdefs(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	updateallrsrc(mtnrsrc);
	SETID(mtnrsrc,mtn);
}
static void getpreviouscb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	get_POIDS(member->parent,mtn);
	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1))
	{
		keyno=MASTER->passkey->keyno;
	}
	ADV2GET_PREVIOUS(member->parent,MASTER,mtn->previous,SCRNvirtualSubData,
		member->parent,keyno,SetAppDefcb,mtn,
		setlistscb,mtn,seteditablecb,mtn);
}
static void getnextcb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	get_POIDS(member->parent,mtn);
	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1))
	{
		keyno=MASTER->passkey->keyno;
	}
	ADV2GET_NEXT(member->parent,MASTER,mtn->previous,SCRNvirtualSubData,
		member->parent,keyno,SetAppDefcb,mtn,
		setlistscb,mtn,seteditablecb,mtn);
}
static void SETID(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	get_POIDS(mtnrsrc,mtn);
	ADV4SET_KEYFUNC(mtnrsrc,MASTER,mtn->previous,SCRNvirtualSubData,
		(void *)mtnrsrc,SetAppDefcb,mtn,
		setlistscb,mtn,seteditablecb,mtn);
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *MASTER)
{
	SET_MASTER(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
}
static void list2file(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected;

	FINDRSCGETINT(mtnrsrc,"PO VENDOR ADDRESS IDENTIFICATIONS",&selected);
	if(RDAstrcmp(mtn->addresslist->libs[selected],"No Addresses Defined"))
	{
		FINDFLDSETSTRING(MASTER->passkey->fileno,"PO VENDOR ADDRESS IDENTIFICATION",mtn->addresslist->libs[selected]);
	} else {
		FINDFLDSETSTRING(MASTER->passkey->fileno,"PO VENDOR ADDRESS IDENTIFICATION",NULL);
	}
	FINDRSCGETINT(mtnrsrc,"BILLING VENDOR ADDRESS IDENTIFICATIONS",&selected);
	if(RDAstrcmp(mtn->addresslist->libs[selected],"No Addresses Defined"))
	{
		FINDFLDSETSTRING(MASTER->passkey->fileno,"BILLING VENDOR ADDRESS IDENTIFICATION",mtn->addresslist->libs[selected]);
	} else {
		FINDFLDSETSTRING(MASTER->passkey->fileno,"BILLING VENDOR ADDRESS IDENTIFICATION",NULL);
	}
}
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MASTER->passkey->fileno);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
	list2file(mtnrsrc,mtn);
}
static void save_record_now(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_record(mtnrsrc,mtn,TRUE);
}
static void save_record_now1(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_record(mtnrsrc,mtn,FALSE);
}
static void save_check(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
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
				NULL,FALSE,2,mtnrsrc,mtn,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	save_record(mtnrsrc,mtn,update);
}
static short check_vendor(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *vendorid1=NULL,*tmp=NULL;
	char flag=FALSE;

	FINDRSCGETSTRING(mtnrsrc,"[PURMST][VENDOR IDENTIFICATION]",&vendorid1);
	ZERNRD(vennum);
	FINDFLDSETSTRING(vennum,"VENDOR IDENTIFICATION",vendorid1);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"VENDOR IDENTIFICATION",vendorid1);
	if(!ADVEQLNRDsec(vennum,1,SCRNvirtualSubData,mtnrsrc))
	{
		FINDFLDGETCHAR(vennum,"DELETEFLAG",&flag);
		if(flag==TRUE)
		{
			if(tmp!=NULL) Rfree(tmp);
			if(vendorid1!=NULL)
				tmp=Rmalloc(RDAstrlen(vendorid1)+83);
			else tmp=Rmalloc(84);
			sprintf(tmp,"The VENDOR IDENTIFICATION field of [%s] exist but is currently in a deleted state.",( vendorid1!=NULL ? vendorid1:""));
			WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED VENDOR IDENTIFICATION!",tmp,NULL,NULL,TRUE,NULL);
			if(tmp!=NULL) Rfree(tmp);
			return(FALSE);
		}
		FINDFLDGETCHAR(vennum,"ACTIVE",&flag);
		if(flag==FALSE)
		{
			if(tmp!=NULL) Rfree(tmp);
			if(vendorid1!=NULL)
				tmp=Rmalloc(RDAstrlen(vendorid1)+86);
			else tmp=Rmalloc(86);
			sprintf(tmp,"The VENDOR IDENTIFICATION field of [%s] exist but is currently in a non-active state.",(vendorid1!=NULL ? vendorid1:""));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE VENDOR IDENTIFICATION!",tmp,NULL,NULL,TRUE,NULL);
			if(tmp!=NULL) Rfree(tmp);
			return(FALSE);
		}
	} else {
		if(tmp!=NULL) Rfree(tmp);
		if(vendorid1!=NULL)
			tmp=Rmalloc(RDAstrlen(vendorid1)+57);
		else tmp=Rmalloc(58);
		sprintf(tmp,"The VENDOR IDENTIFICATION field of [%s] does not exist.",(vendorid1!=NULL ? vendorid1:""));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID VENDOR IDENTIFICATION!",tmp,NULL,NULL,TRUE,NULL);
		if(tmp!=NULL) Rfree(tmp);
		return(FALSE);
	}
	return(TRUE);
}
static void check_vendorcb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readwidget(mtnrsrc,"[PURMST][VENDOR IDENTIFICATION]");
	check_vendor(mtnrsrc,mtn);
	setaddresses(mtnrsrc,mtn,TRUE);
	if(ADVEQLNRDsec(vennum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(vennum,1);
	updatefilerecord2rsrc(vennum,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
/*
static void loadaddresslist(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	short ef;
	char *vendorid=NULL,*vendorid2=NULL;
	char *addrid=NULL;
	char delflag=FALSE;

	if(mtn->addresslist!=NULL) freeapplib(mtn->addresslist);
	mtn->addresslist=APPlibNEW();
	if(mtn->addresslist!=NULL) freeapplib(mtn->addresslist);
	mtn->addresslist=APPlibNEW();
	ZERNRD(venadr);
	FINDFLDGETSTRING(MASTER->passkey->fileno,"VENDOR IDENTIFICATION",&vendorid);
	FINDFLDSETSTRING(venadr,"VENDOR IDENTIFICATION",vendorid);
	ef=ADVGTENRDsec(venadr,1,SCRNvirtualSubData,mtnrsrc);
	while(!ef)
	{
		if(vendorid2!=NULL) Rfree(vendorid2);
		FINDFLDGETSTRING(venadr,"VENDOR IDENTIFICATION",&vendorid2);
		if(RDAstrcmp(vendorid2,vendorid)) break;
		FINDFLDGETCHAR(venadr,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			if(addrid!=NULL) Rfree(addrid);
			FINDFLDGETSTRING(venadr,"ADDRESS IDENTIFICATION",&addrid);
			addAPPlib(mtn->addresslist,addrid);
			addAPPlib(mtn->addresslist,addrid);
		}
		ef=ADVNXTNRDsec(venadr,1,SCRNvirtualSubData,mtnrsrc);
	}
	if(mtn->addresslist->numlibs<1) 
		addAPPlib(mtn->addresslist,"No Addresses Defined");
	if(!FINDRSCLISTAPPlib(mtnrsrc,"PO VENDOR ADDRESS IDENTIFICATIONS",0,
		mtn->addresslist))
	{
		updatersrc(mtnrsrc,"PO VENDOR ADDRESS IDENTIFICATIONS");
	}
	if(mtn->addresslist->numlibs<1) 
		addAPPlib(mtn->addresslist,"No Addresses Defined");
	if(!FINDRSCLISTAPPlib(mtnrsrc,"BILLING VENDOR ADDRESS IDENTIFICATIONS",0,
		mtn->addresslist))
	{
		updatersrc(mtnrsrc,"BILLING VENDOR ADDRESS IDENTIFICATIONS");
	}
	if(addrid!=NULL) Rfree(addrid);
	if(vendorid!=NULL) Rfree(vendorid);
	if(vendorid2!=NULL) Rfree(vendorid2);
	setaddresses(mtnrsrc,mtn,TRUE);
}
*/
static void changePOADDRlist(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int seladdr;
	char *vendorid1=NULL;

	if(FINDRSCGETINT(mtnrsrc,"PO VENDOR ADDRESS IDENTIFICATIONS",&seladdr)) return;
	ZERNRD(venadr);
	if(!RDAstrcmp(mtn->addresslist->libs[seladdr],"No Addresses Defined"))
	{
		FINDFLDSETSTRING(venadr,"VENDOR IDENTIFICATION",vendorid1);
		FINDFLDSETSTRING(venadr,"ADDRESS IDENTIFICATION",NULL);
		FINDFLDSETSTRING(MASTER->passkey->fileno,"PO VENDOR ADDRESS IDENTIFICATION",NULL);
	} else {
		FINDFLDSETSTRING(venadr,"VENDOR IDENTIFICATION",vendorid1);
		FINDFLDSETSTRING(venadr,"ADDRESS IDENTIFICATION",mtn->addresslist->libs[seladdr]);
		FINDFLDSETSTRING(MASTER->passkey->fileno,"PO VENDOR ADDRESS IDENTIFICATION",mtn->addresslist->libs[seladdr]);
	}
	if(ADVEQLNRDsec(venadr,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(venadr,1);
	filerecord2rsrc(venadr,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
	if(vendorid1!=NULL) Rfree(vendorid1);
}
static void changeBILLADDRlist(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int seladdr;
	char *vendorid1=NULL;

	if(FINDRSCGETINT(mtnrsrc,"BILLING VENDOR ADDRESS IDENTIFICATIONS",&seladdr)) return;
	ZERNRD(venadr2);
	if(!RDAstrcmp(mtn->addresslist->libs[seladdr],"No Addresses Defined"))
	{
		FINDFLDSETSTRING(venadr2,"VENDOR IDENTIFICATION",vendorid1);
		FINDFLDSETSTRING(venadr2,"ADDRESS IDENTIFICATION",NULL);
		FINDFLDSETSTRING(MASTER->passkey->fileno,"BILLING VENDOR ADDRESS IDENTIFICATION",NULL);
	} else {
		FINDFLDSETSTRING(venadr2,"VENDOR IDENTIFICATION",vendorid1);
		FINDFLDSETSTRING(venadr2,"ADDRESS IDENTIFICATION",mtn->addresslist->libs[seladdr]);
		FINDFLDSETSTRING(MASTER->passkey->fileno,"BILLING VENDOR ADDRESS IDENTIFICATION",mtn->addresslist->libs[seladdr]);
	}
	if(ADVEQLNRDsec(venadr2,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(venadr,1);
	filerecord2rsrc(venadr2,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
static void setaddresses(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	short ef;
	char *vendorid1=NULL,*vendorid2=NULL;
	char *addrid1=NULL,*addrid2=NULL;
	char delflag=FALSE;
	int x=0,y=0;

	if(mtn->addresslist!=NULL) freeapplib(mtn->addresslist);
	mtn->addresslist=APPlibNEW();
	FINDFLDGETSTRING(MASTER->passkey->fileno,"VENDOR IDENTIFICATION",&vendorid1);
	ZERNRD(venadr);
	FINDFLDSETSTRING(venadr,"VENDOR IDENTIFICATION",vendorid1);
	ef=ADVGTENRDsec(venadr,1,SCRNvirtualSubData,mtnrsrc);
	while(!ef)
	{
		if(vendorid2!=NULL) Rfree(vendorid2);
		FINDFLDGETSTRING(venadr,"VENDOR IDENTIFICATION",&vendorid2);
		if(RDAstrcmp(vendorid2,vendorid1)) break;
		FINDFLDGETCHAR(venadr,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			if(addrid2!=NULL) Rfree(addrid2);
			FINDFLDGETSTRING(venadr,"ADDRESS IDENTIFICATION",&addrid2);
			addAPPlib(mtn->addresslist,addrid2);
		}
		ef=ADVNXTNRDsec(venadr,1,SCRNvirtualSubData,mtnrsrc);
	}
	if(mtn->addresslist->numlibs<1)
	{
		addAPPlib(mtn->addresslist,"No Addresses Defined");
	} else {
		FINDFLDGETSTRING(MASTER->passkey->fileno,"PO VENDOR ADDRESS IDENTIFICATION",&addrid1);
		x=FINDAPPLIBELEMENT(mtn->addresslist,addrid1);
		if(x==(-1))
		{
			x=0;
		}
		FINDFLDGETSTRING(MASTER->passkey->fileno,"BILLING VENDOR ADDRESS IDENTIFICATION",&addrid1);
		y=FINDAPPLIBELEMENT(mtn->addresslist,addrid1);
		if(y==(-1))
		{
			y=0;
		}
	}
	FINDFLDSETSTRING(venadr,"VENDOR IDENTIFICATION",vendorid1);
	FINDFLDSETSTRING(venadr,"ADDRESS IDENTIFICATION",mtn->addresslist->libs[x]);
	FINDFLDSETSTRING(venadr2,"VENDOR IDENTIFICATION",vendorid1);
	FINDFLDSETSTRING(venadr2,"ADDRESS IDENTIFICATION",mtn->addresslist->libs[y]);
	if(ADVEQLNRDsec(venadr,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(venadr,1);
	filerecord2rsrc(venadr,mtnrsrc);
	if(!FINDRSCLISTAPPlib(mtnrsrc,"PO VENDOR ADDRESS IDENTIFICATIONS",x,mtn->addresslist))
	{
		if(update) updatersrc(mtnrsrc,"PO VENDOR ADDRESS IDENTIFICATIONS");
	}
	if(ADVEQLNRDsec(venadr2,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(venadr2,1);
/*
	filerecord2rsrc(venadr2,mtnrsrc);
*/
	if(!FINDRSCLISTAPPlib(mtnrsrc,"BILLING VENDOR ADDRESS IDENTIFICATIONS",y,mtn->addresslist))
	{
		if(update) updatersrc(mtnrsrc,"BILLING VENDOR ADDRESS IDENTIFICATIONS");
	}
	if(addrid1!=NULL) Rfree(addrid1);
	if(addrid2!=NULL) Rfree(addrid2);
	if(vendorid1!=NULL) Rfree(vendorid1);
	if(vendorid2!=NULL) Rfree(vendorid2);
}
static void default_addresses(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	short ef;
	char *vendorid=NULL,*vendorid1=NULL;
	char *adrid=NULL;
	char boolval=FALSE;
	int selected=0;

	FINDFLDGETSTRING(MASTER->passkey->fileno,"PO VENDOR ADDRESS IDENTIFICATION",&adrid);
	selected=FINDAPPLIBELEMENT(mtn->addresslist,adrid);
	if(selected==(-1))
	{
		ZERNRD(venadr);
		FINDFLDGETSTRING(MASTER->passkey->fileno,"VENDOR IDENTIFICATION",&vendorid);
		FINDFLDSETSTRING(venadr,"VENDOR IDENTIFICATION",vendorid);
		ef=ADVGTENRDsec(venadr,venadrkeyno,SCRNvirtualSubData,mtnrsrc);
		while(!ef)
		{
			FINDFLDGETSTRING(venadr,"VENDOR IDENTIFICATION",&vendorid1);
			if(RDAstrcmp(vendorid1,vendorid)) break;
			FINDFLDGETCHAR(venadr,"DEFAULT ORDER",&boolval);
			if(boolval==TRUE)
			{
				FINDFLDGETSTRING(venadr,"ADDRESS IDENTIFICATION",&adrid);
				selected=FINDAPPLIBELEMENT(mtn->addresslist,adrid);
				break;
			}
			ef=ADVNXTNRDsec(venadr,venadrkeyno,SCRNvirtualSubData,mtnrsrc);
		}
		if(boolval==FALSE)
		{
			selected=0;
		}
	}
	ZERNRD(venadr);
	FINDFLDSETSTRING(venadr,"VENDOR IDENTIFICATION",vendorid);
	FINDFLDSETSTRING(venadr,"ADDRESS IDENTIFICATION",mtn->addresslist->libs[selected]);
	if(ADVEQLNRDsec(venadr,venadrkeyno,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(venadr,venadrkeyno);
	}
	FINDRSCSETINT(mtnrsrc,"PO VENDOR ADDRESS IDENTIFICATIONS",selected);
	FINDFLDGETSTRING(MASTER->passkey->fileno,"BILLING VENDOR ADDRESS IDENTIFICATION",&adrid);
	selected=FINDAPPLIBELEMENT(mtn->addresslist,adrid);
	if(selected==(-1))
	{
		ZERNRD(venadr2);
		FINDFLDGETSTRING(MASTER->passkey->fileno,"VENDOR IDENTIFICATION",&vendorid);
		FINDFLDSETSTRING(venadr2,"VENDOR IDENTIFICATION",vendorid);
        	ef=ADVGTENRDsec(venadr2,venadrkeyno,SCRNvirtualSubData,mtnrsrc);
		while(!ef)
		{
			FINDFLDGETSTRING(venadr2,"VENDOR IDENTIFICATION",&vendorid1);
			if(RDAstrcmp(vendorid1,vendorid)) break;
			FINDFLDGETCHAR(venadr2,"DEFAULT BILLING",&boolval);
			if(boolval==TRUE)
			{
				FINDFLDGETSTRING(venadr2,"ADDRESS IDENTIFICATION",&adrid);
				selected=FINDAPPLIBELEMENT(mtn->addresslist,adrid);
				break;
			}
			ef=ADVNXTNRDsec(venadr2,venadrkeyno,SCRNvirtualSubData,mtnrsrc);
		}
		if(boolval==FALSE)
		{
			selected=0;
		}
	}
	ZERNRD(venadr2);
	FINDFLDSETSTRING(venadr2,"VENDOR IDENTIFICATION",vendorid);
	FINDFLDSETSTRING(venadr2,"ADDRESS IDENTIFICATION",mtn->addresslist->libs[selected]);
	if(ADVEQLNRDsec(venadr2,venadrkeyno,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(venadr2,venadrkeyno);
	}
	FINDRSCSETINT(mtnrsrc,"BILLING VENDOR ADDRESS IDENTIFICATIONS",selected);
	if(update)
	{
		updatersrc(mtnrsrc,"BILLING VENDOR ADDRESS IDENTIFICATIONS");
		updatersrc(mtnrsrc,"PO VENDOR ADDRESS IDENTIFICATIONS");
	} 
	if(adrid!=NULL) Rfree(adrid);
	if(vendorid!=NULL) Rfree(vendorid);
	if(vendorid1!=NULL) Rfree(vendorid1);
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
static void save_record(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *rcddesc=NULL;
	int yearid1=0,yearid2=0,yearid3=0,yearid4=0;
	char *poid1=NULL,*poid2=NULL,*poid3=NULL,*poid4=NULL;
	int lineno1=0,lineno2=0,lineno3=0;
	int acctno1=0,acctno2=0;
	char deleteflag=0,mdeleteflag=0,pdeleteflag=0,vdeleteflag=0;
	short linenum=0,acctnum=0,transnum=0,eyrnum=0,ryrnum=0,byrnum=0;
	short ef_line=0,ef_acct=0,ef_trans=0,accttype=0,deftype=0,p_month=0;
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
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(MASTER->passkey->fileno,0,NULL,mtn->previous);
		FINDFLDGETSTRING(MASTER->passkey->fileno,"PO IDENTIFICATION",&mtn->POID);
		ADVRemovePOID(purids,mtn->POID,SCRNvirtualSubData,mtnrsrc);
		if(mtn->POID!=NULL) Rfree(mtn->POID);
		FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			FINDFLDGETINT(MASTER->passkey->fileno,"FISCAL YEAR",&yearid1);
			FINDFLDGETSTRING(MASTER->passkey->fileno,"PO IDENTIFICATION",&poid1);
			linenum=OPNNRDsec("PURORD","PURLIN",TRUE,TRUE,FALSE);	
			acctnum=OPNNRDsec("PURORD","PURACC",TRUE,TRUE,FALSE);
			transnum=OPNNRDsec("PURORD","PURTRN",TRUE,TRUE,FALSE);
			eyrnum=OPNNRDsec("FINMGT","FINEYR",TRUE,TRUE,FALSE);
			ryrnum=OPNNRDsec("FINMGT","FINRYR",TRUE,TRUE,FALSE);
			byrnum=OPNNRDsec("FINMGT","FINBYR",TRUE,TRUE,FALSE);
			tmprsrc=RDArsrcNEW(MASTER->module,NULL);
			copyDFincvir(mtnrsrc,tmprsrc);
			addDFincvir(tmprsrc,"PURORD","PURLIN",NULL,linenum);
			addDFincvir(tmprsrc,"PURORD","PURACC",NULL,acctnum);
			addDFincvir(tmprsrc,"PURORD","PURTRN",NULL,transnum);
			addDFincvir(tmprsrc,"FINMGT","FINEYR",NULL,eyrnum);
			addDFincvir(tmprsrc,"FINMGT","FINRYR",NULL,ryrnum);
			addDFincvir(tmprsrc,"FINMGT","FINBYR",NULL,byrnum);
			if(linenum!=(-1))
			{
				ZERNRD(linenum);
				FINDFLDSETINT(linenum,"FISCAL YEAR",yearid1);
				FINDFLDSETSTRING(linenum,"PO IDENTIFICATION",poid1);
				ef_line=ADVGTENRDsec(linenum,1,SCRNvirtualSubData,tmprsrc);
				while(!ef_line)
				{
					FINDFLDGETCHAR(linenum,"DELETEFLAG",&mdeleteflag);
					if(!mdeleteflag)
					{
						FINDFLDGETINT(linenum,"FISCAL YEAR",&yearid2);
						FINDFLDGETSTRING(linenum,"PO IDENTIFICATION",&poid2);
						if(yearid1==yearid2 && !RDAstrcmp(poid1,poid2)) 
						{
							prev=RDATDataNEW(linenum);
							FINDFLDGETINT(linenum,"LINE NUMBER",&lineno1);
							FINDFLDSETCHAR(linenum,"DELETEFLAG",deleteflag);
							ADVWRTTRANSsec(linenum,0,NULL,prev,SCRNvirtualSubData,tmprsrc);
							if(prev!=NULL) FreeRDATData(prev);
							if(acctnum!=(-1))
							{
								ZERNRD(acctnum);
								FINDFLDSETINT(acctnum,"FISCAL YEAR",yearid1);
								FINDFLDSETSTRING(acctnum,"PO IDENTIFICATION",poid1);
								FINDFLDSETINT(acctnum,"LINE NUMBER",lineno1);
								ef_acct=ADVGTENRDsec(acctnum,1,SCRNvirtualSubData,tmprsrc);
								while(!ef_acct)
								{
									FINDFLDGETCHAR(acctnum,"DELETEFLAG",&pdeleteflag);
									if(!pdeleteflag)
									{
										FINDFLDGETINT(acctnum,"FISCAL YEAR",&yearid3);
										FINDFLDGETSTRING(acctnum,"PO IDENTIFICATION",&poid3);
										FINDFLDGETINT(acctnum,"LINE NUMBER",&lineno2);
										if(yearid3==yearid1 && !RDAstrcmp(poid3,poid1) && lineno2==lineno1)
										{
										  prev=RDATDataNEW(acctnum);
										  FINDFLDGETINT(acctnum,"ACCOUNT NUMBER",&acctno1);
										  FINDFLDSETCHAR(acctnum,"DELETEFLAG",deleteflag);
										  ADVWRTTRANSsec(acctnum,0,NULL,prev,SCRNvirtualSubData,tmprsrc);		
										  if(prev!=NULL) FreeRDATData(prev);
										  if(transnum!=(-1) && eyrnum!=(-1) && ryrnum!=(-1) && byrnum!=(-1))
										  {
										    ZERNRD(transnum);
										    FINDFLDSETINT(transnum,"FISCAL YEAR",yearid1);
										    FINDFLDSETSTRING(transnum,"PO IDENTIFICATION",poid1);
									  	    FINDFLDSETINT(transnum,"LINE NUMBER",lineno1);
										    FINDFLDSETINT(transnum,"ACCOUNT NUMBER",acctno1);
										    ef_trans=ADVGTENRDsec(transnum,1,SCRNvirtualSubData,tmprsrc);
										    while(!ef_trans)
										    {
 										      FINDFLDGETCHAR(transnum,"DELETEFLAG",&vdeleteflag);
										      if(!vdeleteflag)
										      {
										        FINDFLDGETINT(transnum,"FISCAL YEAR",&yearid4);
										        FINDFLDGETSTRING(transnum,"PO IDENTIFICATION",&poid4);
									  	        FINDFLDGETINT(transnum,"LINE NUMBER",&lineno3);
										        FINDFLDGETINT(transnum,"ACCOUNT NUMBER",&acctno2);
										        if(yearid4==yearid1 && !RDAstrcmp(poid4,poid1) && 
										          lineno3==lineno1 && acctno2==acctno1)
										        {
												  prev=RDATDataNEW(transnum);
												  FINDFLDGETSHORT(acctnum,"ACCOUNT TYPE",&accttype);
												  FINDFLDGETDOUBLE(transnum,"AMOUNT",&amount);
												  if(venpmt->Use_Taxes==TRUE)
												  {
													FINDFLDGETDOUBLE(transnum,"COST",&cost);
													FINDFLDGETDOUBLE(transnum,"TAX AMOUNT",&salestax);
													FINDFLDGETDOUBLE(transnum,"ADMIN FEE",&adminfee);
													FINDFLDGETDOUBLE(transnum,"PRORATE AMOUNT",&prorateamt);
												  }
												  if(accttype==EXP_ACCT)
												  {
													ADV4updateoutstanding(MASTER->passkey->fileno,linenum,yearid4,poid4,lineno3,-cost,-salestax,-adminfee,-prorateamt,-amount,TRUE,0,SCRNvirtualSubData,tmprsrc,venpmt);
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
													ADVupdateundist(acctnum,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,eyrnum,byrnum,yearid4,deftype,"ENCUMBRANCES",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,tmprsrc);
												      } else {
													ADVupdateundist(acctnum,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,eyrnum,byrnum,yearid4,deftype,"ENCUMBRANCES",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,tmprsrc);
												      }
												        break;
												      case REV_ACCT:
												      if(purord->use_fiscal_months==TRUE)
												      {
													ADVupdateundist(acctnum,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,ryrnum,byrnum,yearid4,deftype,"ESTIMATED REVENUE",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,tmprsrc);
												      } else {
													ADVupdateundist(acctnum,"ACCOUNT CODE","DEBIT CODE","CREDIT CODE",NULL,ryrnum,byrnum,yearid4,deftype,"ESTIMATED REVENUE",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,tmprsrc);
												      }
												        break;
												      case BAL_ACCT:
												        if(amount<0.0)
												        {
												        if(purord->use_fiscal_months==TRUE)
												        {
													  ADVupdateundist(acctnum,NULL,"ACCOUNT CODE","CREDIT CODE",NULL,(-1),byrnum,yearid4,deftype,"DEBITS",FISCALPERIOD[p_month],-amount,SCRNvirtualSubData,tmprsrc);
												        } else {
													  ADVupdateundist(acctnum,NULL,"ACCOUNT CODE","CREDIT CODE",NULL,(-1),byrnum,yearid4,deftype,"DEBITS",CALENDAR_MONTHS[p_month],-amount,SCRNvirtualSubData,tmprsrc);
												        }
												        } else {
												        if(purord->use_fiscal_months==TRUE)
												        {
													  ADVupdateundist(acctnum,NULL,"ACCOUNT CODE","CREDIT CODE",NULL,(-1),byrnum,yearid4,deftype,"CREDITS",FISCALPERIOD[p_month],amount,SCRNvirtualSubData,tmprsrc);
												        } else {
													  ADVupdateundist(acctnum,NULL,"ACCOUNT CODE","CREDIT CODE",NULL,(-1),byrnum,yearid4,deftype,"CREDITS",CALENDAR_MONTHS[p_month],amount,SCRNvirtualSubData,tmprsrc);
												        }
												        }
												        break;
												      default:
												        prterr("Invalid Account Type [%d]",accttype);
												        break;
												    } /* end switch/case */
												  } 
												  FINDFLDSETCHAR(transnum,"DELETEFLAG",deleteflag);
												  ADVWRTTRANSsec(transnum,0,NULL,prev,SCRNvirtualSubData,tmprsrc);		
												  if(prev!=NULL) FreeRDATData(prev);
										        }
										      }
										      if(poid4!=NULL) Rfree(poid4);
										      ef_trans=ADVNXTNRDsec(transnum,1,SCRNvirtualSubData,tmprsrc);
										    }
										  }
										}
									}
									if(poid3!=NULL) Rfree(poid3);
									ef_acct=ADVNXTNRDsec(acctnum,1,SCRNvirtualSubData,tmprsrc);
								}
							}
						}
					}
					if(poid2!=NULL) Rfree(poid2);
					ef_line=ADVNXTNRDsec(linenum,1,SCRNvirtualSubData,tmprsrc); 	
				}
			}
			if(tmprsrc!=NULL) free_rsrc(tmprsrc);
			if(linenum!=(-1)) CLSNRD(linenum);
			if(acctnum!=(-1)) CLSNRD(acctnum);
			if(transnum!=(-1)) CLSNRD(transnum);
			if(eyrnum!=(-1)) CLSNRD(eyrnum);
			if(ryrnum!=(-1)) CLSNRD(ryrnum);
			if(byrnum!=(-1)) CLSNRD(byrnum);
		}
		if(poid1!=NULL) Rfree(poid1);
		if(poid2!=NULL) Rfree(poid2);
		if(poid3!=NULL) Rfree(poid3);
		if(poid4!=NULL) Rfree(poid4);
		DELETE_SUBORDINATES(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update,mbl_purmsr,MASTER->passkey->fileno,MASTER->module,mtnrsrc);
		if(mbl_purmsr!=NULL)
		{
			quit_record(mtnrsrc,mtn);
		} else {
			seteditable(mtnrsrc,mtn,TRUE);
			GetRDATData(MASTER->passkey->fileno,mtn->previous);
		}
	}
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->addresslist!=NULL) freeapplib(mtn->addresslist);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		if(mtn->POID!=NULL) Rfree(mtn->POID);
		Rfree(mtn);
	}
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
static void save_record_quit(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,FALSE);
}
static void quit_record_test(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	QUIT_RECORD_TEST(mtnrsrc,MASTER->passkey->fileno,mtn->previous,readscreen,
		save_record_quit,quit_record,mtn);
}
void quit_record_undo(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	FINDRSCGETSTRING(mtnrsrc,"[PURMST][PO IDENTIFICATION]",&mtn->POID);
	ADVUndoIDifnoPO(purids,MASTER->passkey->fileno,mtn->POID,SCRNvirtualSubData,mtnrsrc);
	quit_record(mtnrsrc,mtn);
}
static void setlists(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
        setaddresses(mtnrsrc,mtn,update);
}
static void setlistscb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
	void (*SubFunc)(...),void *args,mtnstruct *mtn)
{
        setlists(mtnrsrc,mtn,TRUE);
}
static void seteditablecb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
        void (*SubFunc)(...),void *args,mtnstruct *mtn)
{
        seteditable(mtnrsrc,mtn,TRUE);
	updatefilerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
}
static void seteditable(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char closed=FALSE;

	FINDFLDGETCHAR(MASTER->passkey->fileno,"CLOSED",&closed);
	if(closed)
	{
		if(update)
		{
			updateeditfilersrcsnokeys(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,FALSE);
		} else {
			seteditfilersrc(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,FALSE,FALSE);
		}
		SetRDAScrolledListEditable(mtnrsrc,MASTER->passkey->fileno,MASTER->passkey->keyno,FALSE,FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"PO VENDOR ADDRESS IDENTIFICATIONS",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"BILLING VENDOR ADDRESS IDENTIFICATIONS",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PURMST][CLOSED]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PURMST][DELETEFLAG]",TRUE);
	} else {
		if(update)
		{
			updateeditfilersrcsnokeys(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,TRUE);
		} else {
			seteditfilersrc(MASTER->passkey->fileno,MASTER->passkey->keyno,mtnrsrc,TRUE,FALSE);
		}
		SetRDAScrolledListEditable(mtnrsrc,MASTER->passkey->fileno,MASTER->passkey->keyno,TRUE,TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"PO VENDOR ADDRESS IDENTIFICATIONS",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"BILLING VENDOR ADDRESS IDENTIFICATIONS",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PURMST][OUTSTANDING AMOUNT]",FALSE);
	}
}
static void get_POIDS(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	FINDRSCGETSTRING(mtnrsrc,"[PURMST][PO IDENTIFICATION]",&mtn->POID);
	ADVUndoIDifnoPO(purids,MASTER->passkey->fileno,mtn->POID,SCRNvirtualSubData,mtnrsrc);
}
static void SetAppDef(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *temp=NULL,*warning=NULL;

	FINDRSCGETSTRING(mtnrsrc,"[PURMST][PO IDENTIFICATION]",&mtn->POID);
	if(mtn->POID!=NULL)
	{
		ADVUndoIDifnoPO(purids,MASTER->passkey->fileno,mtn->POID,SCRNvirtualSubData,mtnrsrc);
	}
	if(ADVCheckPOID(purids,mtn->POID,SCRNvirtualSubData,mtnrsrc))
	{
		ADVAssignPOID(purids,mtn->POID,SCRNvirtualSubData,mtnrsrc);	
	} else {
		temp=ADVGetAssignNextPOID(purids,mtn->POID,SCRNvirtualSubData,mtnrsrc);
		if(!isEMPTY(temp))
		{
			if(mtn->POID!=NULL) Rfree(mtn->POID);
			if(RDAstrlen(temp)>0) mtn->POID=stralloc(temp);
			FINDFLDSETSTRING(MASTER->passkey->fileno,"PO IDENTIFICATION",(mtn->POID==NULL?"":mtn->POID));
/*
			FINDRSCSETSTRING(mtnrsrc,"[PURMST][PO IDENTIFICATION]",(mtn->POID==NULL?"":mtn->POID));
*/
			filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
		} else {
			warning=Rmalloc(180+(RDAstrlen(mtn->POID)*2));
			sprintf(warning,"There are no valid PO Identifications Available with the chosen prefix of [%s].  Please create more PO Identifications if you need them with the PREFIX of [%s].",(mtn->POID==NULL?"":mtn->POID),(mtn->POID==NULL?"":mtn->POID));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PO IDENTIFICATION!",warning,NULL,NULL,TRUE,NULL);
			if(warning!=NULL) Rfree(warning);
			ZERNRD(MASTER->passkey->fileno);
			filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
		}
		if(temp!=NULL) Rfree(temp);
	}
	if(mtn->POID!=NULL) Rfree(mtn->POID);
/*
	loadaddresslist(mtnrsrc,mtn);
	updatefilersrc(MASTER->passkey->fileno,mtnrsrc);
*/
	setaddresses(mtnrsrc,mtn,update);
	default_addresses(mtnrsrc,mtn,update);
	if(update)
	{
		updateallrsrc(mtnrsrc);
	}
}
static void SetAppDefcb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
	void (*SubFunc)(...),void *args,mtnstruct *mtn)
{
	SetAppDef(mtnrsrc,mtn,TRUE);
}
