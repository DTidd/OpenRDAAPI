/* purapom.c - Add Purchase Order Master Maintain Screen */
#include <cstdio>


#include <mix.hpp>
#include <fin.hpp>
#include <venpmt.hpp>
#include <purord.hpp>

extern APPlib *categorylist;
extern APPlib *itemlist;
extern char *category_id;
extern char *vendor_id;
extern APPlib *acctavl,*debitavl,*creditavl;
extern APPlib *defdebavl,*defpayavl,*defcredavl,*defclearcashavl;
extern APPlib *DEFtypes,*clearDEFtypes,*defduetoavl,*defclearfromavl;
extern char editmsr,editline,edittrans;
extern char *module;
extern void addlines(void);
extern void addtransactions(void);
extern short purids,vennum,venadrnum,venadr2num;
extern short venadrkeyno;
extern RDAfinmgt *finsetup;
extern char *reportname;
extern int yearid;
extern RDAvenpmt *venpmt;
extern void browse_purmst(RDArsrc *,RDATData *);
extern void browse_venmsr(RDArsrc *);
extern MaintainMaster *VENMSR_MTNMASTER,*PURMST_MTNMASTER;
static short check_vendor_save(RDArsrc *,RDATData *);
static void check_vendor_savecb(RDArsrc *,RDATData *);
static void check_vendor_save_quit(RDArsrc *,RDATData *);
static void check_vendorcb(RDArsrc *,RDATData *);
static short check_vendor(RDArsrc *,RDATData *,short);
static void save_check(RDArsrc *,RDATData *,short);
static void save_record(RDArsrc *,RDATData *,short);
static void save_record_now(RDArsrc *,RDATData *);
static void save_record_now1(RDArsrc *,RDATData *);
static void save_record_up(RDArsrc *,RDATData *);
static void save_record_nup(RDArsrc *,RDATData *);
static void quit_record(RDArsrc *,RDATData *);
static void quit_record_test(RDArsrc *,RDATData *);
static void quit_record_undo(RDArsrc *,RDATData *);
static void puralinefunc(RDArsrc *,RDATData *);
static void puratranfunc(RDArsrc *,RDATData *);
static short check_poid(RDArsrc *,char *,int,RDATData *);
static void SETFILES(RDArsrc *,MaintainMaster *);
static void list2file(RDArsrc *,RDATData *);
static void resetdefs(RDArsrc *mtnrsrc,RDATData *);
static void SetAppDefcb(RDArsrc *,MaintainMaster *,void (*)(...),void *,RDATData *);
static void seteditablecb(RDArsrc *,MaintainMaster *,void (*)(...),void *,RDATData *);
static void setlistscb(RDArsrc *,MaintainMaster *,void (*)(...),void *,RDATData *);
static void getpreviouscb(RDArmem *,RDATData *);
static void getnextcb(RDArmem *,RDATData *);
static void readscreen(RDArsrc *,RDATData *);
static void SetAppDef(RDArsrc *,RDATData *,short,short);
static void default_addresses(RDArsrc *,RDATData *,short);
int PO_COUNT=0; 
char *POID=NULL;
MakeBrowseList *mbl_purmsr=NULL;
MakeBrowseList *mbl_venmsr=NULL;
APPlib *sitesavl=NULL,*ordadravl=NULL,*billadravl=NULL;
void purmsrm(short,void *targetkey);
void setaddresses(RDArsrc *,RDATData *,short);
void seteditable(RDArsrc *,RDATData *,short);
void setlists(RDArsrc *,RDATData *,short);
void get_POIDS(RDArsrc *,RDATData *);
void getrecordx(RDArsrc *,RDATData *);

void purmsrm(short dowhich,void *targetkey)
{
	char *poid1=NULL,*vendorid1=NULL,*name=NULL;
	int which_type=0,yearid1=0;
	short nofields=0,nokeys=0,y=0,x=0,edit_rsrc=FALSE;
	NRDfield *fields=NULL,*fldx;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDArsrc *mtnrsrc=NULL;
	RDATData *previous=NULL;
	
	mtnrsrc=RDArsrcNEW(PURMST_MTNMASTER->module,PURMST_MTNMASTER->maintain_screen);
	ZERNRD(PURMST_MTNMASTER->passkey->fileno);
	previous=RDATDataNEW(PURMST_MTNMASTER->passkey->fileno);
	nokeys=NUMKEYS(PURMST_MTNMASTER->passkey->fileno);
	keys=KEYPOINTER(PURMST_MTNMASTER->passkey->fileno);
	nofields=NUMFLDS(PURMST_MTNMASTER->passkey->fileno);
	fields=FLDPOINTER(PURMST_MTNMASTER->passkey->fileno);
	GET_MAINTAIN_INCVARS(mtnrsrc,PURMST_MTNMASTER);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=(short)editmsr;
			if(!RDAstrcmp(fldx->name,"PO VENDOR ADDRESS IDENTIFICATION"))
			{
				if(FIELDscrn(PURMST_MTNMASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					which_type=0;
					if(ordadravl!=NULL) freeapplib(ordadravl);
					ordadravl=APPlibNEW();
					addAPPlib(ordadravl,"No Address Defined");
					addlstrsrc(mtnrsrc,"PO VENDOR ADDRESS IDENTIFICATIONS",
						&which_type,(edit_rsrc==TRUE?TRUE:FALSE),
						NULL,ordadravl->numlibs,&ordadravl->libs,NULL);
				}
			} else if(!RDAstrcmp(fldx->name,
				"BILLING VENDOR ADDRESS IDENTIFICATION"))
			{
				if(FIELDscrn(PURMST_MTNMASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					which_type=0;
					if(billadravl!=NULL) freeapplib(billadravl);
					billadravl=APPlibNEW();
					addAPPlib(billadravl,"No Address Defined");
					addlstrsrc(mtnrsrc,
						"BILLING VENDOR ADDRESS IDENTIFICATIONS",
						&which_type,(edit_rsrc==TRUE?TRUE:FALSE),NULL,
						billadravl->numlibs,&billadravl->libs,NULL);
				}
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER"))
			{
			} else if(!RDAstrcmp(fldx->name,"CLOSED"))
			{
			} else if(!RDAstrcmp(fldx->name,"PRINTED"))
			{
			} else if(!RDAstrcmp(fldx->name,"OUTSTANDING AMOUNT"))
			{
				nonlstmakefld(mtnrsrc,PURMST_MTNMASTER->passkey->fileno,fldx,FALSE);
			} else {
				nonlstmakefld(mtnrsrc,PURMST_MTNMASTER->passkey->fileno,fldx,edit_rsrc);
				if(name!=NULL)
				{
					name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(PURMST_MTNMASTER->mainfile));
				} else {
					name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(PURMST_MTNMASTER->mainfile));
				}
				sprintf(name,"[%s][%s]",PURMST_MTNMASTER->mainfile,fldx->name);
				FINDRSCSETFUNC(mtnrsrc,name,SETFILES,PURMST_MTNMASTER);
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
						name=Rrealloc(name,RDAstrlen(part->name)+5+RDAstrlen(PURMST_MTNMASTER->mainfile));
					} else {
						name=Rmalloc(RDAstrlen(part->name)+5+RDAstrlen(PURMST_MTNMASTER->mainfile));
					}
					sprintf(name,"[%s][%s]",PURMST_MTNMASTER->mainfile,part->name);
					FINDRSCSETFUNC(mtnrsrc,name,getrecordx,previous);
					FINDRSCSETEDITABLE(mtnrsrc,name,TRUE);
				}
			}
		}
	}
	if(name!=NULL) Rfree(name);
	SET_MAINTAIN_FILE2RSRCS(mtnrsrc,PURMST_MTNMASTER);
	SCRNvirtual2rsrc(mtnrsrc);
	DefaultScreens(mtnrsrc);
	FINDRSCSETFUNC(mtnrsrc,"[PURMST][VENDOR IDENTIFICATION]",check_vendorcb,previous);
	switch(dowhich)
	{
		case 1:
			FINDRSCGETINT(mtnrsrc,"[PURMST][FISCAL YEAR]",&yearid1);
			FINDRSCGETSTRING(mtnrsrc,"[PURMST][PO IDENTIFICATION]",&poid1);
			FINDRSCGETSTRING(mtnrsrc,"[PURMST][VENDOR IDENTIFICATION]",&vendorid1);
			if(ADVEQLNRDsec(PURMST_MTNMASTER->passkey->fileno,PURMST_MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{	
				KEYNRD(PURMST_MTNMASTER->passkey->fileno,PURMST_MTNMASTER->passkey->keyno);
/*
				rsrc2filerecord(PURMST_MTNMASTER->passkey->fileno,mtnrsrc);
				ReadRDAScrolledLists(mtnrsrc);
*/
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,PURMST_MTNMASTER->passkey);
				if(ADVEQLNRDsec(PURMST_MTNMASTER->passkey->fileno,PURMST_MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					FINDFLDSETINT(PURMST_MTNMASTER->passkey->fileno,"FISCAL YEAR",yearid1);
					FINDFLDSETSTRING(PURMST_MTNMASTER->passkey->fileno,"PO IDENTIFICATION",poid1);
					FINDFLDSETSTRING(PURMST_MTNMASTER->passkey->fileno,"VENDOR IDENTIFICATION",vendorid1);
/*
					if(ADVEQLNRDsec(PURMST_MTNMASTER->passkey->fileno,PURMST_MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
					{
						KEYNRD(PURMST_MTNMASTER->passkey->fileno,PURMST_MTNMASTER->passkey->keyno);
						FINDFLDSETSTRING(PURMST_MTNMASTER->passkey->fileno,"VENDOR IDENTIFICATION",vendorid1);
						setlists(mtnrsrc,previous,FALSE);
						SetAppDef(mtnrsrc,previous,FALSE,FALSE);
					} else {
						GetRDATData(PURMST_MTNMASTER->passkey->fileno,previous);
						setlists(mtnrsrc,previous,FALSE);
						filerecord2rsrc(PURMST_MTNMASTER->passkey->fileno,mtnrsrc);
						seteditable(mtnrsrc,previous,FALSE);
					}
*/
				} else {
					GetRDATData(PURMST_MTNMASTER->passkey->fileno,previous);
					setlists(mtnrsrc,previous,FALSE);
					filerecord2rsrc(PURMST_MTNMASTER->passkey->fileno,mtnrsrc);
					seteditable(mtnrsrc,previous,FALSE);
				}
			} else {
				GetRDATData(PURMST_MTNMASTER->passkey->fileno,previous);
				setlists(mtnrsrc,previous,FALSE);
				filerecord2rsrc(PURMST_MTNMASTER->passkey->fileno,mtnrsrc);
				seteditable(mtnrsrc,previous,FALSE);
			}
			break;
		case 2:
			break;
		default:
		case 0: /* not used */
			if(ADVEQLNRDKEYsec(PURMST_MTNMASTER->passkey->fileno,PURMST_MTNMASTER->passkey->keyno,targetkey,SCRNvirtualSubData,mtnrsrc))
			{
				ZERNRD(PURMST_MTNMASTER->passkey->fileno);
				ReadRDAScrolledLists(mtnrsrc);
				if(ADVEQLNRDsec(PURMST_MTNMASTER->passkey->fileno,PURMST_MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					SetAppDef(mtnrsrc,previous,FALSE,FALSE);
				} else {
					GetRDATData(PURMST_MTNMASTER->passkey->fileno,previous);
					setlists(mtnrsrc,previous,FALSE);
					filerecord2rsrc(PURMST_MTNMASTER->passkey->fileno,mtnrsrc);
					seteditable(mtnrsrc,previous,FALSE);
				}
			} else {
				GetRDATData(PURMST_MTNMASTER->passkey->fileno,previous);
				setlists(mtnrsrc,previous,FALSE);
				filerecord2rsrc(PURMST_MTNMASTER->passkey->fileno,mtnrsrc);
				seteditable(mtnrsrc,previous,FALSE);
			}
	}
	if(poid1!=NULL) Rfree(poid1);
	if(vendorid1!=NULL) Rfree(vendorid1);
	GET_SUPPORTING(mtnrsrc,PURMST_MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,previous);
	if(editmsr)
	{
		addrfcbrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
	}
	if(editline)
	{
		addrfcbrsrc(mtnrsrc,"ADD PURCHASE ORDER LINES",TRUE,puralinefunc,previous);
	}
	if(edittrans)
	{
		addrfcbrsrc(mtnrsrc,"ADD PURCHASE ORDER TRANSACTIONS",TRUE,puratranfunc,previous);
	}
	addbtnrsrc(mtnrsrc,"BROWSE PURCHASE ORDERS",TRUE,browse_purmst,previous);
	addbtnrsrc(mtnrsrc,"BROWSE VENDORS",TRUE,browse_venmsr,NULL);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,previous);
	addbtnrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,previous);
	MakeMasterButtons(mtnrsrc,PURMST_MTNMASTER);
	computeallSCRNvirtuals(mtnrsrc);
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,TRUE);
}
static void resetdefs(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(PURMST_MTNMASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	if(POID!=NULL) Rfree(POID);
	FINDRSCGETSTRING(mtnrsrc,"PO IDENTIFICATION",&POID);
	FINDFLDSETSTRING(PURMST_MTNMASTER->passkey->fileno,"PO IDENTIFICATION",POID);
	FINDRSCGETINT(mtnrsrc,"FISCAL YEAR",&yearid);
	FINDFLDSETINT(PURMST_MTNMASTER->passkey->fileno,"FISCAL YEAR",yearid);
	if(ADVEQLNRDsec(PURMST_MTNMASTER->passkey->fileno,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(PURMST_MTNMASTER->passkey->fileno,1);
		ClearRDATData(previous);
		rsrc2filerecord(PURMST_MTNMASTER->passkey->fileno,mtnrsrc);
		setlists(mtnrsrc,previous,FALSE);
	} else {
		GetRDATData(PURMST_MTNMASTER->passkey->fileno,previous);
		setlists(mtnrsrc,previous,FALSE);
		filerecord2rsrc(PURMST_MTNMASTER->passkey->fileno,mtnrsrc);
	}
	seteditable(mtnrsrc,previous,FALSE);
	GET_SUPPORTING(mtnrsrc,PURMST_MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void getpreviouscb(RDArmem *member,RDATData *previous)
{
	short keyno=(-1);

	keyno=KEYNUMBER(PURMST_MTNMASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1))
	{
		keyno=PURMST_MTNMASTER->passkey->keyno;
	}
	get_POIDS(member->parent,previous);
	ADV2GET_PREVIOUS(member->parent,PURMST_MTNMASTER,previous,
		SCRNvirtualSubData,member->parent,keyno,
		SetAppDefcb,previous,setlistscb,
		previous,seteditablecb,previous);
}
static void getnextcb(RDArmem *member,RDATData *previous)
{
	short keyno=(-1);

	keyno=KEYNUMBER(PURMST_MTNMASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1))
	{
		keyno=PURMST_MTNMASTER->passkey->keyno;
	}
	get_POIDS(member->parent,previous);
	ADV2GET_NEXT(member->parent,PURMST_MTNMASTER,previous,
		SCRNvirtualSubData,member->parent,keyno,
		SetAppDefcb,previous,setlistscb,
		previous,seteditablecb,previous);
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *PURMST_MTNMASTER)
{
	SET_MTNMASTER(mtnrsrc,PURMST_MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc);
}
static void list2file(RDArsrc *mtnrsrc,RDATData *previous)
{
	int selected;

	FINDRSCGETINT(mtnrsrc,"PO VENDOR ADDRESS IDENTIFICATIONS",&selected);
	if(RDAstrcmp(ordadravl->libs[selected],"No Addresses Defined"))
	{
		FINDFLDSETSTRING(PURMST_MTNMASTER->passkey->fileno,"PO VENDOR ADDRESS IDENTIFICATION",ordadravl->libs[selected]);
	} else {
		FINDFLDSETSTRING(PURMST_MTNMASTER->passkey->fileno,"PO VENDOR ADDRESS IDENTIFICATION",NULL);
	}
	FINDRSCGETINT(mtnrsrc,"BILLING VENDOR ADDRESS IDENTIFICATIONS",&selected);
	if(RDAstrcmp(billadravl->libs[selected],"No Addresses Defined"))
	{
		FINDFLDSETSTRING(PURMST_MTNMASTER->passkey->fileno,"BILLING VENDOR ADDRESS IDENTIFICATION",billadravl->libs[selected]);
	} else {
		FINDFLDSETSTRING(PURMST_MTNMASTER->passkey->fileno,"BILLING VENDOR ADDRESS IDENTIFICATION",NULL);
	}
}
void readscreen(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(PURMST_MTNMASTER->passkey->fileno);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(PURMST_MTNMASTER->passkey->fileno,mtnrsrc);
	list2file(mtnrsrc,previous);
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

	if(!SAVE_CHECK(mtnrsrc,PURMST_MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc))
	{
		return;
	}
	if(!isEMPTY(PURMST_MTNMASTER->save_expression))
	{
		if(EVALUATEbol(PURMST_MTNMASTER->save_expression,SCRNvirtualSubData,mtnrsrc))
		{
			desc=EVALUATEstr(PURMST_MTNMASTER->save_error_desc,
			SCRNvirtualSubData,mtnrsrc);
			ERRORDIALOG("SAVE ERROR",desc,NULL,FALSE);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	if(!isEMPTY(PURMST_MTNMASTER->save_warning))
	{
		if(EVALUATEbol(PURMST_MTNMASTER->save_warning,SCRNvirtualSubData,mtnrsrc))
		{
			desc=EVALUATEstr(PURMST_MTNMASTER->save_warning_desc,
				SCRNvirtualSubData,mtnrsrc);
			ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN",
				"SAVE WARNING",desc,
				(update?save_record_now:save_record_now1),
				NULL,FALSE,2,mtnrsrc,previous,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	if((check_vendor(mtnrsrc,previous,TRUE))==FALSE)
	{
		return;
	}
	readscreen(mtnrsrc,previous);
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
void setlists(RDArsrc *mtnrsrc,RDATData *previous,short update)
{
	setaddresses(mtnrsrc,previous,update);
}
static void setlistscb(RDArsrc *mtnrsrc,MaintainMaster *M,
	void (*SubFunc)(...),void *args,RDATData *previous)
{
	setlists(mtnrsrc,previous,TRUE);
}
static void seteditablecb(RDArsrc *mtnrsrc,MaintainMaster *M,
	void (*SubFunc)(...),void *args,RDATData *previous)
{
	seteditable(mtnrsrc,previous,TRUE);
	updatefilerecord2rsrc(M->passkey->fileno,mtnrsrc);
}
void seteditable(RDArsrc *mtnrsrc,RDATData *previous,short update)
{
	char closed=FALSE;

	FINDFLDGETCHAR(PURMST_MTNMASTER->passkey->fileno,"CLOSED",&closed);
	if(closed)
	{
		if(update)
		{
			updateeditfilersrcsnokeys(PURMST_MTNMASTER->passkey->fileno,PURMST_MTNMASTER->passkey->keyno,mtnrsrc,FALSE);
		} else {
			seteditfilersrc(PURMST_MTNMASTER->passkey->fileno,PURMST_MTNMASTER->passkey->keyno,mtnrsrc,FALSE,FALSE);
		}
		SetRDAScrolledListEditable(mtnrsrc,PURMST_MTNMASTER->passkey->fileno,PURMST_MTNMASTER->passkey->keyno,FALSE,FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"PO VENDOR ADDRESS IDENTIFICATIONS",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"BILLING VENDOR ADDRESS IDENTIFICATIONS",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PURMST][CLOSED]",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"[PURMST][DELETEFLAG]",TRUE);
	} else {
		if(update)
		{
			updateeditfilersrcsnokeys(PURMST_MTNMASTER->passkey->fileno,PURMST_MTNMASTER->passkey->keyno,mtnrsrc,TRUE);
		} else {
			seteditfilersrc(PURMST_MTNMASTER->passkey->fileno,PURMST_MTNMASTER->passkey->keyno,mtnrsrc,TRUE,FALSE);
		}
		SetRDAScrolledListEditable(mtnrsrc,PURMST_MTNMASTER->passkey->fileno,PURMST_MTNMASTER->passkey->keyno,TRUE,TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"PO VENDOR ADDRESS IDENTIFICATIONS",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"BILLING VENDOR ADDRESS IDENTIFICATIONS",TRUE);
	}
}
static void default_addresses(RDArsrc *mtnrsrc,RDATData *previous,short update)
{
	short ef;
	char *vendorid=NULL,*vendorid1=NULL;
	char *adrid=NULL;
	char boolval=FALSE;
	int selected=0;

	FINDFLDGETSTRING(PURMST_MTNMASTER->passkey->fileno,"PO VENDOR ADDRESS IDENTIFICATION",&adrid);
	selected=FINDAPPLIBELEMENT(ordadravl,adrid);
	if(selected==(-1))
	{
		ZERNRD(venadrnum);
		FINDFLDGETSTRING(PURMST_MTNMASTER->passkey->fileno,"VENDOR IDENTIFICATION",&vendorid);
		FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid);
		ef=ADVGTENRDsec(venadrnum,venadrkeyno,SCRNvirtualSubData,mtnrsrc);
		while(!ef)
		{
			FINDFLDGETSTRING(venadrnum,"VENDOR IDENTIFICATION",&vendorid1);
			if(RDAstrcmp(vendorid1,vendorid))
			{
				boolval=FALSE;
				break;
			}
			FINDFLDGETCHAR(venadrnum,"DEFAULT ORDER",&boolval);
			if(boolval==TRUE)
			{
				FINDFLDGETSTRING(venadrnum,"ADDRESS IDENTIFICATION",&adrid);
				selected=FINDAPPLIBELEMENT(ordadravl,adrid);
				break;
			}
			ef=ADVNXTNRDsec(venadrnum,venadrkeyno,SCRNvirtualSubData,mtnrsrc);
		}
		if(boolval==FALSE)
		{
			selected=0;
		}
	}
	ZERNRD(venadrnum);
	FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid);
	FINDFLDSETSTRING(venadrnum,"ADDRESS IDENTIFICATION",ordadravl->libs[selected]);
	if(ADVEQLNRDsec(venadrnum,venadrkeyno,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(venadrnum,venadrkeyno);
	}
	FINDRSCSETINT(mtnrsrc,"PO VENDOR ADDRESS IDENTIFICATIONS",selected);
	FINDFLDGETSTRING(PURMST_MTNMASTER->passkey->fileno,"BILLING VENDOR ADDRESS IDENTIFICATION",&adrid);
	selected=FINDAPPLIBELEMENT(billadravl,adrid);
	if(selected==(-1))
	{
		ZERNRD(venadr2num);
		FINDFLDGETSTRING(PURMST_MTNMASTER->passkey->fileno,"VENDOR IDENTIFICATION",&vendorid);
		FINDFLDSETSTRING(venadr2num,"VENDOR IDENTIFICATION",vendorid);
		ef=ADVGTENRDsec(venadr2num,venadrkeyno,SCRNvirtualSubData,mtnrsrc);
		while(!ef)
		{
			FINDFLDGETSTRING(venadr2num,"VENDOR IDENTIFICATION",&vendorid1);
			if(RDAstrcmp(vendorid1,vendorid))
			{
				boolval=FALSE;
				break;
			}
			FINDFLDGETCHAR(venadr2num,"DEFAULT BILLING",&boolval);
			if(boolval==TRUE)
			{
				FINDFLDGETSTRING(venadr2num,"ADDRESS IDENTIFICATION",&adrid);
				selected=FINDAPPLIBELEMENT(billadravl,adrid);
				break;
			}
			ef=ADVNXTNRDsec(venadr2num,venadrkeyno,SCRNvirtualSubData,mtnrsrc);
		}
		if(boolval==FALSE)
		{
			selected=0;
		}
	}
	ZERNRD(venadr2num);
	FINDFLDSETSTRING(venadr2num,"VENDOR IDENTIFICATION",vendorid);
	FINDFLDSETSTRING(venadr2num,"ADDRESS IDENTIFICATION",billadravl->libs[selected]);
	if(ADVEQLNRDsec(venadr2num,venadrkeyno,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(venadr2num,venadrkeyno);
	}
	FINDRSCSETINT(mtnrsrc,"BILLING VENDOR ADDRESS IDENTIFICATIONS",selected);
	if(update)
	{
		updatersrc(mtnrsrc,"PO VENDOR ADDRESS IDENTIFICATIONS");
		updatersrc(mtnrsrc,"BILLING VENDOR ADDRESS IDENTIFICATIONS");
	}
	if(adrid!=NULL) Rfree(adrid);
	if(vendorid!=NULL) Rfree(vendorid);
}
void SetAppDef(RDArsrc *mtnrsrc,RDATData *previous,short update,short initial)
{
	char *temp=NULL;

	if(initial)
	{
	FINDFLDGETSTRING(PURMST_MTNMASTER->passkey->fileno,"PO IDENTIFICATION",&POID);
	ADVUndoIDifnoPO(purids,PURMST_MTNMASTER->passkey->fileno,POID,SCRNvirtualSubData,mtnrsrc);
	if(ADVCheckPOID(purids,POID,SCRNvirtualSubData,mtnrsrc))
	{
		ADVAssignPOID(purids,POID,SCRNvirtualSubData,mtnrsrc);
	} else {
		temp=ADVGetAssignNextPOID(purids,POID,SCRNvirtualSubData,mtnrsrc);
		if(POID!=NULL) Rfree(POID);
		if(temp!=NULL)
		{
			POID=stralloc(temp);
			Rfree(temp);
		}
		FINDFLDSETSTRING(PURMST_MTNMASTER->passkey->fileno,"PO IDENTIFICATION",POID);
		FINDRSCSETSTRING(mtnrsrc,"[PURMST][PO IDENTIFICATION]",POID);
		if(isEMPTY(POID))
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PO IDENTIFICATION!","There are no valid PO Identifications Available.  Please create more PO Identifications.",NULL,NULL,TRUE,NULL);
		}
	}
	if(POID!=NULL) Rfree(POID);
	}
	setaddresses(mtnrsrc,previous,FALSE);
	default_addresses(mtnrsrc,previous,TRUE);
	if(update)
	{
		updatersrc(mtnrsrc,"[PURMST][PO IDENTIFICATION]");
	}
}
static void SetAppDefcb(RDArsrc *mtnrsrc,MaintainMaster *M,
	void (*SubFunc)(...),void *args,RDATData *previous)
{
        SetAppDef(mtnrsrc,previous,TRUE,TRUE);
}
/*
void getsupportingrecords(RDArsrc *mtnrsrc,char update)
{
	char *vendorid=NULL;

	seteditable(mtnrsrc,update);
	FINDRSCGETSTRING(mtnrsrc,"[PURMST][VENDOR IDENTIFICATION]",&vendorid);
	FINDFLDSETSTRING(vennum,"VENDOR IDENTIFICATION",vendorid);
	if(ADVEQLNRDsec(vennum,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(vennum,1);
	if(update) updatefilerecord2rsrc(vennum,mtnrsrc);
		else filerecord2rsrc(vennum,mtnrsrc);
}
*/
static void check_vendorcb(RDArsrc *mtnrsrc,RDATData *previous)
{
	readscreen(mtnrsrc,previous);
	if((check_vendor(mtnrsrc,previous,TRUE))==TRUE)
	{
		default_addresses(mtnrsrc,previous,TRUE);
	} else {
		KEYNRD(vennum,1);
	}
/*
	if(ADVEQLNRDsec(vennum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(vennum,1);
	} else {
		default_addresses(mtnrsrc,previous,TRUE);
	}
*/
	updatefilerecord2rsrc(vennum,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
static void check_vendor_save_quit(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(!check_vendor_save(mtnrsrc,previous)) quit_record(mtnrsrc,previous);
}
static void check_vendor_savecb(RDArsrc *mtnrsrc,RDATData *previous)
{
	check_vendor_save(mtnrsrc,previous);
}
static short check_vendor_save(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(check_vendor(mtnrsrc,previous,FALSE))
	{
		save_record(mtnrsrc,previous,TRUE);
		return(TRUE);
	} else {
		return(FALSE);
	}
}
static short check_vendor(RDArsrc *mtnrsrc,RDATData *previous,short update)
{
	char *vendorid1=NULL,*tmp=NULL;
	char flag=FALSE;

	FINDRSCGETSTRING(mtnrsrc,"[PURMST][VENDOR IDENTIFICATION]",&vendorid1);
	ZERNRD(vennum);
	FINDFLDSETSTRING(vennum,"VENDOR IDENTIFICATION",vendorid1);
	FINDFLDSETSTRING(PURMST_MTNMASTER->passkey->fileno,"VENDOR IDENTIFICATION",vendorid1);
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
			setaddresses(mtnrsrc,previous,update);
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
			setaddresses(mtnrsrc,previous,update);
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
		setaddresses(mtnrsrc,previous,update);
		if(tmp!=NULL) Rfree(tmp);
		return(FALSE);
	}
	setaddresses(mtnrsrc,previous,update);
	return(TRUE);
}
void setaddresses(RDArsrc *mtnrsrc,RDATData *previous,short update)
{
	short ef;
	char *vendorid=NULL,*vendorid2=NULL;
	char *addrid=NULL,*poadrid=NULL,*billadrid=NULL;
	char delflag=FALSE,ordboolval=FALSE,billboolval=FALSE;
	int selordadr=0,selbilladr=0;

	if(ordadravl!=NULL) freeapplib(ordadravl);
	ordadravl=APPlibNEW();
	if(billadravl!=NULL) freeapplib(billadravl);
	billadravl=APPlibNEW();
	FINDFLDGETSTRING(PURMST_MTNMASTER->passkey->fileno,"VENDOR IDENTIFICATION",&vendorid);
	FINDFLDGETSTRING(PURMST_MTNMASTER->passkey->fileno,"PO VENDOR ADDRESS IDENTIFICATION",&poadrid);
	FINDFLDGETSTRING(PURMST_MTNMASTER->passkey->fileno,"BILLING VENDOR ADDRESS IDENTIFICATION",&billadrid);
	ZERNRD(venadrnum);
	FINDFLDSETSTRING(venadrnum,"VENDOR IDENTIFICATION",vendorid);
	ef=ADVGTENRDsec(venadrnum,1,SCRNvirtualSubData,mtnrsrc);
	while(!ef)
	{
		FINDFLDGETSTRING(venadrnum,"VENDOR IDENTIFICATION",&vendorid2);
		if(RDAstrcmp(vendorid2,vendorid)) 
		{
			ZERNRD(venadrnum);
			break;
		}
		FINDFLDGETCHAR(venadrnum,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			if(addrid!=NULL) Rfree(addrid); 
   			FINDFLDGETSTRING(venadrnum,"ADDRESS IDENTIFICATION",&addrid);
			addAPPlib(ordadravl,addrid);
			if(poadrid!=NULL)
			{
				if(!RDAstrcmp(ordadravl->libs[(ordadravl->numlibs)-1],poadrid))
				{
					selordadr=(ordadravl->numlibs)-1;
				}
			} else {
				if(ordboolval==FALSE)
				{
					FINDFLDGETCHAR(venadrnum,"DEFAULT ORDER",&ordboolval);
					if(ordboolval==TRUE) selordadr=(ordadravl->numlibs)-1;
				}
			}
			addAPPlib(billadravl,addrid);
			if(billadrid!=NULL)
			{
				if(!RDAstrcmp(billadravl->libs[(billadravl->numlibs)-1],
					billadrid))
				{
					selbilladr=(billadravl->numlibs)-1;
				}
			} else {
				if(billboolval==FALSE)
				{
					FINDFLDGETCHAR(venadrnum,"DEFAULT BILLING",&billboolval);
					if(billboolval==TRUE) selbilladr=(billadravl->numlibs)-1;
				}
			}
		}
		ef=ADVNXTNRDsec(venadrnum,1,SCRNvirtualSubData,mtnrsrc);
	}
	if(ordadravl->numlibs<1) addAPPlib(ordadravl,"No Address Defined");
	if(!FINDRSCLISTAPPlib(mtnrsrc,"PO VENDOR ADDRESS IDENTIFICATIONS",
		selordadr,ordadravl))
	{
		if(update)
		{
			updatersrc(mtnrsrc,"PO VENDOR ADDRESS IDENTIFICATIONS");
		}
	}
	if(billadravl->numlibs<1) addAPPlib(billadravl,"No Address Defined");
	if(!FINDRSCLISTAPPlib(mtnrsrc,"BILLING VENDOR ADDRESS IDENTIFICATIONS",
		selbilladr,billadravl))
	{
		if(update)
		{
			updatersrc(mtnrsrc,"BILLING VENDOR ADDRESS IDENTIFICATIONS");
		}
	}
	if(poadrid!=NULL) Rfree(poadrid);
	if(billadrid!=NULL) Rfree(billadrid); 
	if(addrid!=NULL) Rfree(addrid); 
	if(vendorid!=NULL) Rfree(vendorid); 
	if(vendorid2!=NULL) Rfree(vendorid2);
}
static void save_record(RDArsrc *mtnrsrc,RDATData *previous,short update)
{
	char *rcddesc=NULL;

	FINDFLDSETSTRING(PURMST_MTNMASTER->passkey->fileno,"SOURCE USER",USERLOGIN);
	if(ADVRECORDsec(PURMST_MTNMASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(PURMST_MTNMASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE); 
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(PURMST_MTNMASTER->passkey->fileno,0,NULL,previous);
		FINDFLDGETSTRING(PURMST_MTNMASTER->passkey->fileno,"PO IDENTIFICATION",&POID);
		ADVRemovePOID(purids,POID,SCRNvirtualSubData,mtnrsrc);
		if(POID!=NULL) Rfree(POID);
	}
	GetRDATData(PURMST_MTNMASTER->passkey->fileno,previous);
	updateSCRNvirtuals(mtnrsrc);
}
static void quit_record(RDArsrc *mtnrsrc,RDATData *previous)
{
	char exit_status=TRUE;
	APPlib *args=NULL;

	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc); 
	if(PO_COUNT>0)
	{
		exit_status=FALSE;
		args=APPlibNEW();
		addAPPlib(args,module);
		addAPPlib(args,reportname);
		ExecuteProgram("doreport",args);
		if(args!=NULL) freeapplib(args);
	}
	if(categorylist!=NULL) freeapplib(categorylist);
	if(itemlist!=NULL) freeapplib(itemlist);
	if(category_id!=NULL) Rfree(category_id);
	if(vendor_id!=NULL) Rfree(vendor_id);
	if(acctavl!=NULL) freeapplib(acctavl);
	if(creditavl!=NULL) freeapplib(creditavl);
	if(debitavl!=NULL) freeapplib(debitavl);
	if(defdebavl!=NULL) freeapplib(defdebavl);
	if(defcredavl!=NULL) freeapplib(defcredavl);
	if(defpayavl!=NULL) freeapplib(defpayavl);
	if(DEFtypes!=NULL) freeapplib(DEFtypes);
	if(clearDEFtypes!=NULL) freeapplib(clearDEFtypes);
	if(defduetoavl!=NULL) freeapplib(defduetoavl);
	if(defclearfromavl!=NULL) freeapplib(defclearfromavl);
	if(defclearcashavl!=NULL) freeapplib(defclearcashavl);
	if(mbl_venmsr!=NULL) FREE_BROWSE_EXIT(mbl_venmsr);
	if(POID!=NULL) Rfree(POID);
	if(finsetup!=NULL) free_finmgt(finsetup);
	if(venpmt!=NULL) free_venpmt(venpmt);
	if(exit_status)
	{
		CLOSE_MASTER_FILES(PURMST_MTNMASTER);
		CLOSE_MASTER_FILES(VENMSR_MTNMASTER);
		if(PURMST_MTNMASTER!=NULL) FreeMaintainMaster(PURMST_MTNMASTER);
		if(VENMSR_MTNMASTER!=NULL) FreeMaintainMaster(VENMSR_MTNMASTER);
		ShutdownSubsystems();
		exit(0);
	}
}
static short check_poid(RDArsrc *mtnrsrc,char *poid,int yearid,RDATData *previous)
{
	char flag=FALSE,*warnmessage=NULL;

        ZERNRD(PURMST_MTNMASTER->passkey->fileno);
        FINDFLDSETINT(PURMST_MTNMASTER->passkey->fileno,"FISCAL YEAR",yearid);
        FINDFLDSETSTRING(PURMST_MTNMASTER->passkey->fileno,"PO IDENTIFICATION",poid);
        if(ADVEQLNRDsec(PURMST_MTNMASTER->passkey->fileno,1,SCRNvirtualSubData,mtnrsrc))
        {
		FINDFLDSETDOUBLE(PURMST_MTNMASTER->passkey->fileno,"OUTSTANDING AMOUNT",0.00);
		warnmessage=Rmalloc(RDAstrlen(poid)+301);
		sprintf(warnmessage,"Purchase Order Lines and/or Transactions can not be added until the Purchase Order Master has been saved.  The FISCAL YEAR of [%4d] and the PO IDENTIFICATION of [%s] entered does not presently exist in the [PURORD][PURMST] file. Do you wish to create it?",yearid,(poid==NULL?"":poid));
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","PURCHASE ORDER MASTER DOES NOT EXIST!",warnmessage,check_vendor_savecb,NULL,FALSE,2,mtnrsrc,previous,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		return(FALSE);
	} else {
		FINDFLDGETCHAR(PURMST_MTNMASTER->passkey->fileno,"DELETEFLAG",&flag);
		if(flag)
		{
			warnmessage=Rmalloc(RDAstrlen(poid)+301);
			sprintf(warnmessage,"Purchase Order Lines and/or Transactions can not be added to a deleted Purchase Order Master.  The master record for the FISCAL YEAR of [%4d] and the PO IDENTIFICATION of [%s] entered is in a state of deletion in the [PURORD][PURMST] file.",yearid,(poid==NULL?"":poid));
			WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED PURCHASE ORDER MASTER!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(PURMST_MTNMASTER->passkey->fileno,"CLOSED",&flag);
			if(flag)
			{
				warnmessage=Rmalloc(RDAstrlen(poid)+256);
				sprintf(warnmessage,"Purchase Order Lines and/or Transactions can not be added to a closed Purchase Order Master.  The master record for the FISCAL YEAR of [%4d] and the PO IDENTIFICATION of [%s] entered has been closed.",yearid,(poid==NULL?"":poid));
				WARNINGDIALOG("WARNING DIALOG SCREEN","CLOSED PURCHASE ORDER MASTER!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				return(FALSE);
			}
		}
	}
	return(TRUE);
}
static void quit_record_undo(RDArsrc *mtnrsrc,RDATData *previous)
{
	char *temp=NULL;

	FINDRSCGETSTRING(mtnrsrc,"[PURMST][PO IDENTIFICATION]",&temp);
	ADVUndoIDifnoPO(purids,PURMST_MTNMASTER->passkey->fileno,temp,SCRNvirtualSubData,mtnrsrc);
	if(temp!=NULL) Rfree(temp);
	quit_record(mtnrsrc,previous);
}
static void quit_record_test(RDArsrc *mtnrsrc,RDATData *previous)
{
	char *desc=NULL;

	readscreen(mtnrsrc,previous);
	FINDFLDSETSTRING(PURMST_MTNMASTER->passkey->fileno,"SOURCE USER",USERLOGIN);
	if(QUITvfy(PURMST_MTNMASTER->passkey->fileno,previous))
	{
		desc=QUITvfydesc(PURMST_MTNMASTER->passkey->fileno,previous);
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!",
			desc,check_vendor_save_quit,quit_record_undo,FALSE,2,mtnrsrc,
			previous,NULL);
		if(desc!=NULL) Rfree(desc);
	} else {
		quit_record(mtnrsrc,previous);
	}
}
static void puratranfunc(RDArsrc *mtnrsrc,RDATData *previous)
{
	readscreen(mtnrsrc,previous);
	FINDRSCGETINT(mtnrsrc,"[PURMST][FISCAL YEAR]",&yearid);
	if(POID!=NULL) Rfree(POID);
	FINDRSCGETSTRING(mtnrsrc,"[PURMST][PO IDENTIFICATION]",&POID);
	if(vendor_id!=NULL) Rfree(vendor_id);
	FINDRSCGETSTRING(mtnrsrc,"[PURMST][VENDOR IDENTIFICATION]",&vendor_id);
	if(check_poid(mtnrsrc,POID,yearid,previous)==FALSE) return;
	addtransactions();
}
static void puralinefunc(RDArsrc *mtnrsrc,RDATData *previous)
{
	readscreen(mtnrsrc,previous);
	FINDRSCGETINT(mtnrsrc,"[PURMST][FISCAL YEAR]",&yearid);
	if(POID!=NULL) Rfree(POID);
	FINDRSCGETSTRING(mtnrsrc,"[PURMST][PO IDENTIFICATION]",&POID);
	if(vendor_id!=NULL) Rfree(vendor_id);
	FINDRSCGETSTRING(mtnrsrc,"[PURMST][VENDOR IDENTIFICATION]",&vendor_id);
	if(check_poid(mtnrsrc,POID,yearid,previous)==FALSE) return;
	addlines();
}
void get_POIDS(RDArsrc *mtnrsrc,RDATData *previous)
{
	FINDRSCGETSTRING(mtnrsrc,"[PURMST][PO IDENTIFICATION]",&POID);
	ADVUndoIDifnoPO(purids,PURMST_MTNMASTER->passkey->fileno,POID,SCRNvirtualSubData,mtnrsrc);
}
void getrecordx(RDArsrc *mtnrsrc,RDATData *previous)
{
	char *temp=NULL;
	double zerodollar=0.00;

/* removing if locked record previously, intentionally before readallwidgets */
	FINDRSCGETSTRING(mtnrsrc,"[PURMST][PO IDENTIFICATION]",&temp);
	ADVUndoIDifnoPO(purids,PURMST_MTNMASTER->passkey->fileno,temp,SCRNvirtualSubData,mtnrsrc);
	if(temp!=NULL) Rfree(temp);
	readallwidgets(mtnrsrc);
	FINDRSCGETINT(mtnrsrc,"[PURMST][FISCAL YEAR]",&yearid);
	if(POID!=NULL) Rfree(POID);
	FINDRSCGETSTRING(mtnrsrc,"[PURMST][PO IDENTIFICATION]",&POID);
	ZERNRD(PURMST_MTNMASTER->passkey->fileno);
	FINDFLDSETINT(PURMST_MTNMASTER->passkey->fileno,"FISCAL YEAR",yearid);
	FINDFLDSETSTRING(PURMST_MTNMASTER->passkey->fileno,"PO IDENTIFICATION",POID);
	seteditfilersrc(PURMST_MTNMASTER->passkey->fileno,1,mtnrsrc,TRUE,FALSE);
	if(ADVEQLNRDsec(PURMST_MTNMASTER->passkey->fileno,1,SCRNvirtualSubData,mtnrsrc))
	{
		FINDRSCSETDOUBLE(mtnrsrc,"[PURMST][OUTSTANDING AMOUNT]",zerodollar);
		updatersrc(mtnrsrc,"[PURMST][OUTSTANDING AMOUNT]");
		KEYNRD(PURMST_MTNMASTER->passkey->fileno,1);
		ClearRDATData(previous);
		if(ADVCheckPOID(purids,POID,SCRNvirtualSubData,mtnrsrc))
		{
			ADVAssignPOID(purids,POID,SCRNvirtualSubData,mtnrsrc);	
			FINDRSCSETSTRING(mtnrsrc,"[PURMST][PO IDENTIFICATION]",POID);
/*
			filerecord2rsrc(PURMST_MTNMASTER->passkey->fileno,mtnrsrc);
*/
		} else {
			temp=ADVGetAssignNextPOID(purids,POID,SCRNvirtualSubData,mtnrsrc);
			if(POID!=NULL) Rfree(POID);
			if(temp!=NULL)
			{
				POID=stralloc(temp);
				Rfree(temp);
			}
			FINDFLDSETSTRING(PURMST_MTNMASTER->passkey->fileno,"PO IDENTIFICATION",POID);
			FINDRSCSETSTRING(mtnrsrc,"[PURMST][PO IDENTIFICATION]",POID);
			updatersrc(mtnrsrc,"[PURMST][PO IDENTIFICATION]");
/*
			if(!isEMPTY(POID))
			{
				getrecordx(mtnrsrc,previous);
			} else {
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PO IDENTIFICATION!","There are no valid PO Identifications Available.  Please create more PO Identifications.",NULL,NULL,TRUE,NULL);
				filerecord2rsrc(PURMST_MTNMASTER->passkey->fileno,mtnrsrc);
			}
*/
			if(isEMPTY(POID))
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PO IDENTIFICATION!","There are no valid PO Identifications Available.  Please create more PO Identifications.",NULL,NULL,TRUE,NULL);
			} else {
				GetRDATData(PURMST_MTNMASTER->passkey->fileno,previous);
/*
				filerecord2rsrc(PURMST_MTNMASTER->passkey->fileno,mtnrsrc);
				GET_SUPPORTING(mtnrsrc,PURMST_MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
				setaddresses(mtnrsrc,previous,FALSE);
				computeallSCRNvirtuals(mtnrsrc);
				updateallrsrc(mtnrsrc);
*/
			}
		}
	} else {
		GetRDATData(PURMST_MTNMASTER->passkey->fileno,previous);
		filerecord2rsrc(PURMST_MTNMASTER->passkey->fileno,mtnrsrc);
		GET_SUPPORTING(mtnrsrc,PURMST_MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
		setaddresses(mtnrsrc,previous,FALSE);
		computeallSCRNvirtuals(mtnrsrc);
		updateallrsrc(mtnrsrc);
	}	
}
