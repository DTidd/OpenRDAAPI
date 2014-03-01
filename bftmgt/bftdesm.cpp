/* bftdesm.c - Xpert's Benefit Description Maintain Screen */
#include <cstdio>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <ldval.hpp>

extern char editable;
extern MakeBrowseList *mbl_bftdes;
extern MaintainMaster *MTNMASTER;
extern void browse_bftdes(RDArsrc *,RDATData *);
static void save_record(RDArsrc *,RDATData *,short);
static void save_check(RDArsrc *,RDATData *,short);
static void save_record_now(RDArsrc *,RDATData *);
static void save_record_now1(RDArsrc *,RDATData *);
static void save_record_quit(RDArsrc *,RDATData *);
static void quit_record(RDArsrc *,RDATData *);
static void quit_record_test(RDArsrc *,RDATData *);
static void save_record_up(RDArsrc *,RDATData *);
static void save_record_nup(RDArsrc *,RDATData *);
static void readscreen(RDArsrc *,RDATData *previous);
void bftdesm(short,void *);
static void SETID(RDArsrc *,RDATData *);
static void SETFILES(RDArsrc *,MaintainMaster *);
static void getpreviouscb(RDArmem *,RDATData *);
static void getnextcb(RDArmem *,RDATData *);
static void resetdefs(RDArsrc *,RDATData *);

static void getpreviouscb(RDArmem *member,RDATData *previous)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MTNMASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1)) keyno=MTNMASTER->passkey->keyno;
	ADV2GET_PREVIOUS(member->parent,MTNMASTER,previous,SCRNvirtualSubData,
		member->parent,keyno,NULL,NULL,NULL,NULL,
		NULL,NULL);
}
static void getnextcb(RDArmem *member,RDATData *previous)
{
	short keyno=(-1);
	keyno=KEYNUMBER(MTNMASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1)) keyno=MTNMASTER->passkey->keyno;
	ADV2GET_NEXT(member->parent,MTNMASTER,previous,SCRNvirtualSubData,
		member->parent,keyno,NULL,NULL,NULL,NULL,
		NULL,NULL);
}
static void SETID(RDArsrc *mtnrsrc,RDATData *previous)
{
	ADV4SET_KEYFUNC(mtnrsrc,MTNMASTER,previous,SCRNvirtualSubData,(void *)mtnrsrc,NULL,NULL,NULL,NULL,NULL,NULL);
}
static void SETFILES(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER)
{
	SET_MTNMASTER(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc);
}
static void addholdvalues(APPlib *tmp,char *modulex,char *filex)
{
	NRDfield *f=NULL,*fields=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part=NULL;
	short z=0,engine=0,nofields=0,nokeys=0;
	int y=0;
	char *temp=NULL;

	if(diagrptgen)
	{
		prterr("DIAG addholdvalues Module [%s] File [%s] ",modulex,filex);
	}
	if(!GETBIN(modulex,filex,&engine,&nofields,&fields,
		&nokeys,&keys))
	{
		if(keys!=NULL)
		{
			for(y=0,keyx=keys;y<nokeys;++y,++keyx)
			{
				if(keyx->name!=NULL) Rfree(keyx->name);
				if(keyx->part!=NULL)
				{
					for(z=0,part=keyx->part;z<keyx->numparts;
						++z,++part)
					{
						if(part->name!=NULL) Rfree(part->name);
					}
				}
				Rfree(keyx->part);
			}
			Rfree(keys);
			if(fields!=NULL)
			{
				for(y=0,f=fields;y<nofields;++y,++f)
				{
					if(temp!=NULL)
					{
						temp=Rrealloc(temp,RDAstrlen(f->name)+
							RDAstrlen(modulex)+
							RDAstrlen(filex)+7);
					} else  {
						temp=Rmalloc(RDAstrlen(f->name)+RDAstrlen(modulex)+
							RDAstrlen(filex)+7);
					}
					sprintf(temp,"[%s][%s][%s]",modulex,
						filex,f->name);
					addAPPlib(tmp,temp);
					if(f->name!=NULL) Rfree(f->name);
				}
				Rfree(fields);
			}
		}
	}
	if(temp!=NULL) Rfree(temp);
}
static APPlib *makefieldvallist(RDArsrc *r)
{
	APPlib *tmp=NULL,*virflist=NULL;
	char addvf=FALSE,*libx=NULL,deleteflag=FALSE,active=FALSE,*dedid=NULL;
	int x,y,z;
	DFincvir *i,*a;
	DFvirtual *d=NULL;
	short dumb=0;
	RDArsrc *tmprsrc=NULL;

	tmp=APPlibNEW();
	tmprsrc=RDArsrcNEW(MTNMASTER->module,"TEMP");
	addDFincvir(tmprsrc,MTNMASTER->module,"BFTMSTR",NULL,dumb);
	addDFincvir(tmprsrc,MTNMASTER->module,"BFTDESC",NULL,dumb);
	addDFincvir(tmprsrc,"PRSNNL","PERDMG",NULL,dumb);
/*
	addDFincvir(tmprsrc,"POSTRK","POSMSTR",NULL,dumb);
	addDFincvir(tmprsrc,"POSTRK","POSGRS",NULL,dumb);
	addDFincvir(tmprsrc,"POSTRK","POSGRAT",NULL,dumb);
	addDFincvir(tmprsrc,"POSTRK","POSPER",NULL,dumb);
	addDFincvir(tmprsrc,"PAYROLL","PAYJMST",NULL,dumb);
	addDFincvir(tmprsrc,"PAYROLL","PAYJCSM",NULL,dumb);
	addDFincvir(tmprsrc,"PAYROLL","PAYJFSM",NULL,dumb);
	addDFincvir(tmprsrc,"PAYROLL","PAYJPRM",NULL,dumb);
	addDFincvir(tmprsrc,"PAYROLL","PAYMSTR",NULL,dumb);
	addDFincvir(tmprsrc,"PAYROLL","PAYJRCS",NULL,dumb);
	addDFincvir(tmprsrc,"PAYROLL","PAYJRFS",NULL,dumb);
*/
	addDFincvir(tmprsrc,"PAYROLL","PAYDDMS",NULL,dumb);
	addDFincvir(tmprsrc,"PAYROLL","PAYDDAC",NULL,dumb);
	addDFincvir(tmprsrc,"PAYROLL","PAYDTYP",NULL,dumb);
	addDFincvir(tmprsrc,"PAYROLL","PAYDEDM",NULL,dumb);
	addDFincvir(tmprsrc,"PAYROLL","PAYDCSM",NULL,dumb);
	addDFincvir(tmprsrc,"PAYROLL","PAYDFSM",NULL,dumb);
	addDFincvir(tmprsrc,"PAYROLL","PAYPCSM",NULL,dumb);
	addDFincvir(tmprsrc,"PAYROLL","PAYPFSM",NULL,dumb);
	for(x=0,i=tmprsrc->incvir;x<tmprsrc->numvir;++x,++i)
	{
		addholdvalues(tmp,i->module,i->file);
	}
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+17);
	sprintf(libx,"%s/rda/PAYROLL.VIR",CURRENTDIRECTORY);
	virflist=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	libx=NULL;
	if(virflist!=NULL)
	{
		for(y=0;y<virflist->numlibs;++y)
		{
			d=getDFvirtual(MTNMASTER->module,virflist->libs[y]);
			if(d!=NULL)
			{
				addvf=TRUE;
				if(d->incvir!=NULL)
				{
					for(z=0,a=d->incvir;z<d->num;++z,++a)
					{
						for(x=0,i=tmprsrc->incvir;x<tmprsrc->numvir;++x,++i)
						{
							if(!RDAstrcmp(i->module,a->module) &&
								!RDAstrcmp(i->file,a->file)) break;
						}
						if(x>=tmprsrc->numvir)
						{
							addvf=FALSE;
							break;
						}
					}
				}
				if(addvf)
				{
					libx=Rmalloc(RDAstrlen(d->name)+3);
					sprintf(libx,"[%s]",d->name);
					addAPPlibNoDuplicates(tmp,libx);
					if(libx!=NULL) Rfree(libx);
				}
				FreeDFvirtual(d);
			}
		}
	}
	if(virflist!=NULL) freeapplib(virflist);
	if(libx!=NULL) Rfree(libx);
	addAPPlibNoDuplicates(tmp,"[SYSTEM DATE]");
	addAPPlibNoDuplicates(tmp,"[SYSTEM TIME]");
	addAPPlibNoDuplicates(tmp,"[LOGIN IDENTIFICATION]");
	addAPPlibNoDuplicates(tmp,"[ORGANIZATION NAME]");
	addAPPlibNoDuplicates(tmp,"[ORGANIZATION ADDRESS #1]");
	addAPPlibNoDuplicates(tmp,"[ORGANIZATION ADDRESS #2]");
	addAPPlibNoDuplicates(tmp,"[ORGANIZATION CITY]");
	addAPPlibNoDuplicates(tmp,"[ORGANIZATION STATE]");
	addAPPlibNoDuplicates(tmp,"[ORGANIZATION ZIP]");
	addAPPlibNoDuplicates(tmp,"[ORGANIZATION PHONE]");
	addAPPlibNoDuplicates(tmp,"[STARTING FISCAL MONTH]");
	if(tmprsrc!=NULL) free_rsrc(tmprsrc);
	ZERNRD(MTNMASTER->passkey->fileno);
	dumb=ADVFRSNRDsec(MTNMASTER->passkey->fileno,1,SCRNvirtualSubData,r);
	while(!dumb)
	{
		FINDFLDGETCHAR(MTNMASTER->passkey->fileno,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(MTNMASTER->passkey->fileno,"ACTIVE",&active);
		if(!deleteflag && active)
		{
			FINDFLDGETSTRING(MTNMASTER->passkey->fileno,"DEDUCTION DESCRIPTION IDENTIFICATION",
				&dedid);
			libx=Rmalloc(RDAstrlen(dedid)+16);
			sprintf(libx,"[%s][CHECK GROSS]",dedid);
			addAPPlibNoDuplicates(tmp,libx);
			if(libx!=NULL) Rfree(libx);
			libx=Rmalloc(RDAstrlen(dedid)+11);
			sprintf(libx,"[%s][AMOUNT]",dedid);
			addAPPlibNoDuplicates(tmp,libx);
			if(libx!=NULL) Rfree(libx);
			libx=Rmalloc(RDAstrlen(dedid)+9);
			sprintf(libx,"[%s][BASE]",dedid);
			addAPPlibNoDuplicates(tmp,libx);
			addAPPlibNoDuplicates(tmp,libx);
			if(libx!=NULL) Rfree(libx);
			libx=Rmalloc(RDAstrlen(dedid)+22);
			sprintf(libx,"[%s][COMPUTATION LEVEL]",dedid);
			addAPPlibNoDuplicates(tmp,libx);
			if(libx!=NULL) Rfree(libx);
			libx=Rmalloc(RDAstrlen(dedid)+23);
			sprintf(libx,"[%s][ANNUAL AFFECT BASE]",dedid);
			addAPPlibNoDuplicates(tmp,libx);
			if(libx!=NULL) Rfree(libx);
			libx=Rmalloc(RDAstrlen(dedid)+16);
			sprintf(libx,"[%s][AFFECT BASE]",dedid);
			addAPPlibNoDuplicates(tmp,libx);
			if(libx!=NULL) Rfree(libx);
			libx=Rmalloc(RDAstrlen(dedid)+10);
			sprintf(libx,"[%s][RATIO]",dedid);
			addAPPlibNoDuplicates(tmp,libx);
			if(libx!=NULL) Rfree(libx);
			if(dedid!=NULL) Rfree(dedid);
		}
		dumb=ADVNXTNRDsec(MTNMASTER->passkey->fileno,1,SCRNvirtualSubData,r);
	}
	SORTAPPlib(tmp);
	return(tmp);
}
static void loadvalue(RDArsrc *r,char *fieldname)
{
	int x;
	LoadValue *load=NULL;

	ldvalrsrc=RDArsrcNEW("RPTGEN","LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	loadlist=makefieldvallist(r);
	if(loadlist->numlibs<1)
	{
		addAPPlib(loadlist,"Load List Unavailable");
	}
	x=0;
	addlstrsrc(ldvalrsrc,"LOAD LIST",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addrfkwrsrc(ldvalrsrc,"SELECT",TRUE,okldval,load);
	addrfkwrsrc(ldvalrsrc,"QUIT",TRUE,quitldval,load);
	addbtnrsrc(ldvalrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ldvalrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(ldvalrsrc,TRUE,quitldval,load,FALSE);
}
void bftdesm(short dowhich,void *targetkey)
{
	char *name=NULL;
	short nofields=0,nokeys=0,x,y,edit_rsrc=FALSE;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDATData *previous=NULL;
	RDArsrc *mtnrsrc=NULL;

	mtnrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->maintain_screen);
	ZERNRD(MTNMASTER->passkey->fileno);
	previous=RDATDataNEW(MTNMASTER->passkey->fileno);
	nokeys=NUMKEYS(MTNMASTER->passkey->fileno);
	keys=KEYPOINTER(MTNMASTER->passkey->fileno);
	nofields=NUMFLDS(MTNMASTER->passkey->fileno);
	fields=FLDPOINTER(MTNMASTER->passkey->fileno);
	GET_MAINTAIN_INCVARS(mtnrsrc,MTNMASTER);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=editable;
			if(!RDAstrcmp(fldx->name,"PROCESSED BALANCE"))
			{
				if(FIELDscrn(MTNMASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					makefieldrsrc(mtnrsrc
						,"[BFTDESC][PROCESSED BALANCE]"
						,fldx->type,fldx->len,FALSE);
				}
			} else if(!RDAstrcmp(fldx->name,"UNPROCESSED BALANCE"))
			{
				if(FIELDscrn(MTNMASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					makefieldrsrc(mtnrsrc
						,"[BFTDESC][UNPROCESSED BALANCE]"
						,fldx->type,fldx->len,FALSE);
				}
			} else {
				nonlstmakefld(mtnrsrc,MTNMASTER->passkey->fileno,fldx,edit_rsrc);
				if(name!=NULL)
				{
					name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(MTNMASTER->mainfile));
				} else {
					name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(MTNMASTER->mainfile));
				}
				sprintf(name,"[%s][%s]",MTNMASTER->mainfile,fldx->name);
				FINDRSCSETFUNC(mtnrsrc,name,SETFILES,MTNMASTER);
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
						name=Rrealloc(name,RDAstrlen(part->name)+5+RDAstrlen(MTNMASTER->mainfile));
					} else {
						name=Rmalloc(RDAstrlen(part->name)+5+RDAstrlen(MTNMASTER->mainfile));
					}
					sprintf(name,"[%s][%s]",MTNMASTER->mainfile,part->name);
					FINDRSCSETFUNC(mtnrsrc,name,SETID,previous);
				}
			}
		}
	}
	if(name!=NULL) Rfree(name);
	SET_MAINTAIN_FILE2RSRCS(mtnrsrc,MTNMASTER);
	SCRNvirtual2rsrc(mtnrsrc);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			ApplyPassKey(MTNMASTER->passkey);
			if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
				if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
				} else {
					GetRDATData(MTNMASTER->passkey->fileno,previous);
					filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
				}
			} else {
				GetRDATData(MTNMASTER->passkey->fileno,previous);
				filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
			}
			break;
		case 2:
			ApplyPassKey(MTNMASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
			if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
			} else {
				GetRDATData(MTNMASTER->passkey->fileno,previous);
				filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
			}
			break;
		default:
		case 0:
			if(ADVEQLNRDKEYsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,targetkey,SCRNvirtualSubData,mtnrsrc))
			{
				ZERNRD(MTNMASTER->passkey->fileno);
				ApplyPassKey(MTNMASTER->passkey);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
				if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
				} else {
					GetRDATData(MTNMASTER->passkey->fileno,previous);
					filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
				}
			} else {
				GetRDATData(MTNMASTER->passkey->fileno,previous);
				filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
			}
			break;
	}
	GET_SUPPORTING(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"LOAD VALUE NOTIFICATION",TRUE,loadvalue,
		"[BFTDESC][NOTIFICATION FORMULA]");
	addbtnrsrc(mtnrsrc,"LOAD OPERATOR NOTIFICATION",TRUE,loadoperand,
		"[BFTDESC][NOTIFICATION FORMULA]");
	addbtnrsrc(mtnrsrc,"LOAD GROUPER NOTIFICATION",TRUE,loadgrouper,
		"[BFTDESC][NOTIFICATION FORMULA]");
	addbtnrsrc(mtnrsrc,"LOAD CONTROL NOTIFICATION",TRUE,loadcontrol,
		"[BFTDESC][NOTIFICATION FORMULA]");
	addbtnrsrc(mtnrsrc,"LOAD VALUE DESCRIPTION",TRUE,loadvalue,
		"[BFTDESC][DETAIL DESCRIPTION]");
	addbtnrsrc(mtnrsrc,"LOAD OPERATOR DESCRIPTION",TRUE,loadoperand,
		"[BFTDESC][DETAIL DESCRIPTION]");
	addbtnrsrc(mtnrsrc,"LOAD GROUPER DESCRIPTION",TRUE,loadgrouper,
		"[BFTDESC][DETAIL DESCRIPTION]");
	addbtnrsrc(mtnrsrc,"LOAD CONTROL DESCRIPTION",TRUE,loadcontrol,
		"[BFTDESC][DETAIL DESCRIPTION]");
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,previous);
	if(mbl_bftdes!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,previous);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,previous);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_bftdes,previous);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,previous);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,previous);
	MakeMasterButtons(mtnrsrc,MTNMASTER);
	computeallSCRNvirtuals(mtnrsrc);
	if(mbl_bftdes!=NULL) FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"LOAD VALUE NOTIFICATION",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"LOAD OPERATOR NOTIFICATION",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"LOAD GROUPER NOTIFICATION",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"LOAD CONTROL NOTIFICATION",FALSE);
	}
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,(mbl_bftdes==NULL?TRUE:FALSE));
}
static void resetdefs(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(MTNMASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	updateallrsrc(mtnrsrc);
	SETID(mtnrsrc,previous);
}
static void readscreen(RDArsrc *mtnrsrc,RDATData *previous)
{
	ZERNRD(MTNMASTER->passkey->fileno);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MTNMASTER->passkey->fileno,mtnrsrc);
}
static void save_record_now(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_record(mtnrsrc,previous,TRUE);
}
static void save_record_now1(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_record(mtnrsrc,previous,FALSE);
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
static void save_check(RDArsrc *mtnrsrc,RDATData *previous,short update)
{
	char *desc=NULL;

	if(!SAVE_CHECK(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc))
	{
		return;
	}
	if(!isEMPTY(MTNMASTER->save_expression))
	{
		if(EVALUATEbol(MTNMASTER->save_expression,SCRNvirtualSubData,mtnrsrc))
		{
			desc=EVALUATEstr(MTNMASTER->save_error_desc,
				SCRNvirtualSubData,mtnrsrc);
			ERRORDIALOG("SAVE ERROR",desc,NULL,FALSE);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	if(!isEMPTY(MTNMASTER->save_warning))
	{
		if(EVALUATEbol(MTNMASTER->save_warning,SCRNvirtualSubData,mtnrsrc))
		{
			desc=EVALUATEstr(MTNMASTER->save_warning_desc,
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
static void save_record(RDArsrc *mtnrsrc,RDATData *previous,short update_list)
{
	char *rcddesc=NULL;

	if(ADVRECORDsec(MTNMASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(MTNMASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(MTNMASTER->passkey->fileno,0,NULL,previous);
		DELETE_SUBORDINATES(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update_list,mbl_bftdes,MTNMASTER->passkey->fileno,MTNMASTER->module,mtnrsrc);
		if(mbl_bftdes!=NULL) quit_record(mtnrsrc,previous);
		else GetRDATData(MTNMASTER->passkey->fileno,previous);
	}
}
static void quit_record(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(previous!=NULL) FreeRDATData(previous); 
	if(WindowCount<2)
	{
		CLOSE_MASTER_FILES(MTNMASTER);
		if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
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
	QUIT_RECORD_TEST(mtnrsrc,MTNMASTER->passkey->fileno,previous,readscreen,
	save_record_quit,quit_record,previous);
}
