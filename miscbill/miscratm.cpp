/* miscratm.c - Misc Billing Class Rate Maintain Screen */
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
#include <ldval.h>
#include <pptmgt.h>

struct mtnstructs
{
	APPlib *class;
	APPlib *DEFtypes;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern short raccnum,baccnum;
extern char editable;
extern char *module;
extern void browse_msr(RDArsrc *,mtnstruct *);
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_check(RDArsrc *,mtnstruct *,short);
static void save_record_up(RDArsrc *,mtnstruct *);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void quit_record(RDArsrc *,mtnstruct *);
static void quit_record_test(RDArsrc *,mtnstruct *);
static void getrecord(RDArsrc *,mtnstruct *);
MakeBrowseList *mbl_pptclml=NULL;
extern MaintainMaster *MASTER;
void miscratem(short,void *targetkey);
static void getsupportingfiles(RDArsrc *,mtnstruct *,short);
static void setlists(RDArsrc *,mtnstruct *);
static void changeDEFtype(RDArsrc *,mtnstruct *);
static short chkaccountcode(RDArsrc *,mtnstruct *);
static void chkaccountcodecb(RDArsrc *,mtnstruct *);
static short chkrevenuecode(RDArsrc *,mtnstruct *);
static void chkrevenuecodecb(RDArsrc *,mtnstruct *);
static void readscreen(RDArsrc *,mtnstruct *);
static short chkcashcode(RDArsrc *,mtnstruct *);
static void chkcashcodecb(RDArsrc *,mtnstruct *);
static void save_record_quit(RDArsrc *,mtnstruct *);
static void getpreviouscb(RDArmem *,mtnstruct *);
static void getnextcb(RDArmem *,mtnstruct *);
static void setlistscb(RDArsrc *,MaintainMaster *,void (*)(),void *,
	mtnstruct *);

static void addholdvalues(APPlib *tmp,char *modulex,char *filex)
{
	NRDfield *f,*fields=NULL;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short z,engine=0,nofields=0,nokeys=0;
	int y;
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
					} else 	{
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
static APPlib *makefieldvallist()
{
	APPlib *tmp=NULL,*virflist=NULL;
	char addvf=FALSE,*libx=NULL;
	int x,y,z;
	DFincvir *i,*a;
	DFvirtual *d=NULL;
	short dumb=0;
	RDArsrc *tmprsrc=NULL;

	tmp=APPlibNEW();
	tmprsrc=RDArsrcNEW(module,"TEMP");
	addDFincvir(tmprsrc,module,"MISCMSTR",NULL,dumb);
	addDFincvir(tmprsrc,module,"MISCTRAN",NULL,dumb);
	addDFincvir(tmprsrc,module,"MISCPAY",NULL,dumb);
	addDFincvir(tmprsrc,module,"MISCCLAS",NULL,dumb);
	addDFincvir(tmprsrc,module,"MISCRATE",NULL,dumb);
	for(x=0,i=tmprsrc->incvir;x<tmprsrc->numvir;++x,++i)
	{
		addholdvalues(tmp,i->module,i->file);
	}
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+17);
	sprintf(libx,"%s/rda/PPTMGT.VIR",CURRENTDIRECTORY);
	virflist=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	libx=NULL;
	if(virflist!=NULL)
	{
		for(y=0;y<virflist->numlibs;++y)
		{
			d=getDFvirtual(module,virflist->libs[y]);
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
	return(tmp);
}
static void loadvalue(RDArsrc *r,char *fieldname)
{
	int x;
	LoadValue *load=NULL;

	ldvalrsrc=RDArsrcNEW("RPTGEN","LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	loadlist=makefieldvallist();
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
static void save_record_quit(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,TRUE);
}
static void quit_record_test(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	QUIT_RECORD_TEST(mtnrsrc,MASTER->passkey->fileno,mtn->previous,readscreen,save_record_quit,quit_record,mtn);
}
static void chkaccountcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *acctcode=NULL;
	short d=0;
	int x=0;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	d=x;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFAULT DEFINITION TYPE",d);
	readwidget(mtnrsrc,"[MISCRATE][DEFAULT ACCOUNT CODE]");
	FINDRSCGETSTRING(mtnrsrc,"[MISCRATE][DEFAULT ACCOUNT CODE]",&acctcode);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"DEFAULT ACCOUNT CODE",acctcode);
	if(acctcode!=NULL) Rfree(acctcode);
	chkaccountcode(mtnrsrc,mtn);
	updateSCRNvirtuals(mtnrsrc);
}
static short chkaccountcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *acctcode=NULL,*desc=NULL,*edesc=NULL;
	short retval=TRUE;

	FIELDCOPY(MASTER->passkey->fileno,"DEFAULT DEFINITION TYPE",raccnum,"DEFINITION TYPE");
	FINDFLDGETSTRING(MASTER->passkey->fileno,"DEFAULT ACCOUNT CODE",&acctcode);
	FINDFLDSETSTRING(raccnum,"ACCOUNT CODE",acctcode);
	if(ADVEQLNRDsec(raccnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(raccnum,1);
		retval=FALSE;
		edesc=Rmalloc(RDAstrlen(acctcode)+118);
		sprintf(edesc,"The Account Code [%s] is invalid.  It doesn't exist. Please enter a correct account or cancel this operation.",(acctcode==NULL?"":acctcode));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID ACCOUNT CODE",edesc,NULL,NULL,TRUE,NULL);
		if(edesc!=NULL) Rfree(edesc);
	}
	if(acctcode!=NULL) Rfree(acctcode);
	FINDFLDGETSTRING(raccnum,"DESCRIPTION",&desc);
	FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",desc);
	updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
	return(retval);
}
static void chkrevenuecodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *acctcode=NULL;
	short d=0;
	int x=0;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	d=x;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFAULT DEFINITION TYPE",d);
	readwidget(mtnrsrc,"[MISCRATE][DEFAULT REVENUE CODE]");
	FINDRSCGETSTRING(mtnrsrc,"[MISCRATE][DEFAULT REVENUE CODE]",&acctcode);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"DEFAULT REVENUE CODE",acctcode);
	if(acctcode!=NULL) Rfree(acctcode);
	chkrevenuecode(mtnrsrc,mtn);
	updateSCRNvirtuals(mtnrsrc);
}
static short chkrevenuecode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *acctcode=NULL,*desc=NULL,*edesc=NULL;
	short retval=TRUE;

	FIELDCOPY(MASTER->passkey->fileno,"DEFAULT DEFINITION TYPE",baccnum,"DEFINITION TYPE");
	FINDFLDGETSTRING(MASTER->passkey->fileno,"DEFAULT REVENUE CODE",&acctcode);
	FINDFLDSETSTRING(baccnum,"ACCOUNT CODE",acctcode);
	if(ADVEQLNRDsec(baccnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(baccnum,1);
		retval=FALSE;
		edesc=Rmalloc(RDAstrlen(acctcode)+118);
		sprintf(edesc,"The Revenue Code [%s] is invalid.  It doesn't exist. Please enter a correct account or cancel this operation.",(acctcode==NULL?"":acctcode));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID REVENUE CODE",edesc,NULL,NULL,TRUE,NULL);
		if(edesc!=NULL) Rfree(edesc);
	}
	if(acctcode!=NULL) Rfree(acctcode);
	FINDFLDGETSTRING(baccnum,"DESCRIPTION",&desc);
	FINDRSCSETSTRING(mtnrsrc,"REVENUE CODE DESCRIPTION",desc);
	updatersrc(mtnrsrc,"REVENUE CODE DESCRIPTION");
	return(retval);
}
static void chkcashcodecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *acctcode=NULL;
	short d=0;
	int x=0;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	d=x;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFAULT DEFINITION TYPE",d);
	readwidget(mtnrsrc,"[MISCRATE][DEFAULT CASH CODE]");
	FINDRSCGETSTRING(mtnrsrc,"[MISCRATE][DEFAULT CASH CODE]",&acctcode);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"DEFAULT CASH CODE",acctcode);
	if(acctcode!=NULL) Rfree(acctcode);
	chkcashcode(mtnrsrc,mtn);
	updateSCRNvirtuals(mtnrsrc);
}
static short chkcashcode(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *acctcode=NULL,*desc=NULL,*edesc=NULL;
	short retval=TRUE;

	FIELDCOPY(MASTER->passkey->fileno,"DEFAULT DEFINITION TYPE",baccnum,"DEFINITION TYPE");
	FINDFLDGETSTRING(MASTER->passkey->fileno,"DEFAULT CASH CODE",&acctcode);
	FINDFLDSETSTRING(baccnum,"ACCOUNT CODE",acctcode);
	if(ADVEQLNRDsec(baccnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(baccnum,1);
		retval=FALSE;
		edesc=Rmalloc(RDAstrlen(acctcode)+115);
		sprintf(edesc,"The Cash Code [%s] is invalid.  It doesn't exist. Please enter a correct account or cancel this operation.",(acctcode==NULL?"":acctcode));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CASH CODE",edesc,NULL,NULL,TRUE,NULL);
		if(edesc!=NULL) Rfree(edesc);
	}
	if(acctcode!=NULL) Rfree(acctcode);
	FINDFLDGETSTRING(baccnum,"DESCRIPTION",&desc);
	FINDRSCSETSTRING(mtnrsrc,"CASH CODE DESCRIPTION",desc);
	updatersrc(mtnrsrc,"CASH CODE DESCRIPTION");
	return(retval);
}
static void setfiles(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	readscreen(mtnrsrc,mtn);
	getsupportingfiles(mtnrsrc,mtn,TRUE);
}
static void setlists(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selectedd=0,listlen=0;
	short deftype=0;

	FINDFLDGETSHORT(MASTER->passkey->fileno,"DEFAULT DEFINITION TYPE",&deftype);
	selectedd=(int)deftype;
	if(selectedd>=mtn->DEFtypes->numlibs)
	{
		listlen=(selectedd-mtn->DEFtypes->numlibs+1);
		for(selectedd=0;selectedd<listlen;selectedd++)
		{
			addAPPlib(mtn->DEFtypes,"Invalid-Definition Type");
		}
	}
	FINDRSCSETDEFTYPE(mtnrsrc,"[MISCRATE][DEFAULT ACCOUNT CODE]",deftype);
	FINDRSCSETDEFTYPE(mtnrsrc,"[MISCRATE][DEFAULT REVENUE CODE]",deftype);
	FINDRSCSETDEFTYPE(mtnrsrc,"[MISCRATE][DEFAULT CASH CODE]",deftype);
	FINDRSCLISTAPPlib(mtnrsrc,"DEFINITION TYPES",selectedd,mtn->DEFtypes);
	chkaccountcode(mtnrsrc,mtn);
	chkrevenuecode(mtnrsrc,mtn);
	chkcashcode(mtnrsrc,mtn);
}
static void changeDEFtype(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	short deftype1=0,deftype2=0;
	int seldeftype2=0;

	readwidget(mtnrsrc,"[MISCRATE][DEFAULT ACCOUNT CODE]");
	FINDRSCGETDEFTYPE(mtnrsrc,"[MISCRATE][DEFAULT ACCOUNT CODE]",&deftype1);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&seldeftype2);
	deftype2=(short)seldeftype2;
	if(deftype1!=deftype2)
	{
		FINDRSCSETSTRING(mtnrsrc,"[MISCRATE][DEFAULT ACCOUNT CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"ACCOUNT CODE DESCRIPTION",NULL);
		FINDRSCSETDEFTYPE(mtnrsrc,"[MISCRATE][DEFAULT ACCOUNT CODE]",deftype2);
		FINDRSCSETSTRING(mtnrsrc,"[MISCRATE][DEFAULT REVENUE CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"REVENUE CODE DESCRIPTION",NULL);
		FINDRSCSETDEFTYPE(mtnrsrc,"[MISCRATE][DEFAULT REVENUE CODE]",deftype2);
		FINDRSCSETSTRING(mtnrsrc,"[MISCRATE][DEFAULT CASH CODE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"CASH CODE DESCRIPTION",NULL);
		FINDRSCSETDEFTYPE(mtnrsrc,"[MISCRATE][DEFAULT CASH CODE]",deftype2);
		updatersrc(mtnrsrc,"[MISCRATE][DEFAULT ACCOUNT CODE]");
		updatersrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION");
		updatersrc(mtnrsrc,"[MISCRATE][DEFAULT REVENUE CODE]");
		updatersrc(mtnrsrc,"REVENUE CODE DESCRIPTION");
		updatersrc(mtnrsrc,"[MISCRATE][DEFAULT CASH CODE]");
		updatersrc(mtnrsrc,"CASH CODE DESCRIPTION");
	}
	updateSCRNvirtuals(mtnrsrc);
}
void miscratem(short dowhich,void *targetkey)
{
	RDArsrc *mtnrsrc=NULL;
	int nofields=0,x,which_type=0;
	NRDfield *fields=NULL,*fldx;
	short edit_rsrc=FALSE;
	mtnstruct *mtn=NULL;
	RDAacct *acc=NULL;
	char *temp=NULL;
	
	mtnrsrc=RDArsrcNEW(module,MASTER->maintain_screen);
	GET_MAINTAIN_INCVARS(mtnrsrc,MASTER);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->class=NULL;
	mtn->DEFtypes=APPlibNEW();
	if(REVENUE_ACCOUNT!=NULL)
	{
		if(REVENUE_ACCOUNT->codetype!=NULL)
		{
			for(x=0,acc=REVENUE_ACCOUNT->codetype;x<REVENUE_ACCOUNT->num;
				++x,++acc)
			{
				addAPPlib(mtn->DEFtypes,acc->name);
			}
		}
	} 
	if(mtn->DEFtypes->numlibs<1)
	{
		addAPPlib(mtn->DEFtypes,"No Definitions Available");
	}
	ZERNRD(MASTER->passkey->fileno);
	mtn->previous=RDATDataNEW(MASTER->passkey->fileno);
	nofields=NUMFLDS(MASTER->passkey->fileno);
	fields=FLDPOINTER(MASTER->passkey->fileno);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=(short)editable;
			if(!RDAstrcmp(fldx->name,"DEFAULT DEFINITION TYPE"))
			{
				if(FIELDscrn(MASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					addlstrsrc(mtnrsrc,"DEFINITION TYPES",
						&which_type,(edit_rsrc ? TRUE:FALSE),
						changeDEFtype,mtn->DEFtypes->numlibs,
						&mtn->DEFtypes->libs,mtn);
				}
			} else { 
				nonlstmakefld(mtnrsrc,MASTER->passkey->fileno,fldx,edit_rsrc);
				temp=Rmalloc(RDAstrlen(module)+RDAstrlen(fldx->name)+5);
				sprintf(temp,"[%s][%s]",module,fldx->name);
				FINDRSCSETFUNC(mtnrsrc,temp,setfiles,mtn);
				if(temp!=NULL) Rfree(temp);
			}
		}
	}
	nofields=0;
	FINDRSCSETFUNC(mtnrsrc,"[MISCRATE][CLASS IDENTIFICATION]",getrecord,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[MISCRATE][RATE IDENTIFICATION]",getrecord,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[MISCRATE][DEFAULT ACCOUNT CODE]",chkaccountcodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[MISCRATE][DEFAULT REVENUE CODE]",chkrevenuecodecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"[MISCRATE][DEFAULT CASH CODE]",chkcashcodecb,mtn);
	addstdrsrc(mtnrsrc,"ACCOUNT CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"REVENUE CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mtnrsrc,"CASH CODE DESCRIPTION",VARIABLETEXT,0,NULL,FALSE);
	SET_MAINTAIN_FILE2RSRCS(mtnrsrc,MASTER);
	SCRNvirtual2rsrc(mtnrsrc);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			ApplyPassKey(MASTER->passkey);
			if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,
				SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(MASTER->passkey->fileno,MASTER->passkey->keyno);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
				if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,
					SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
				} else {
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
					setlists(mtnrsrc,mtn);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				setlists(mtnrsrc,mtn);
			}
			break;
		case 2:
			ApplyPassKey(MASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
			if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,
				SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				setlists(mtnrsrc,mtn);
			}
			break;
		default:
		case 0:
			if(ADVEQLNRDKEYsec(MASTER->passkey->fileno,MASTER->passkey->keyno,
				targetkey,SCRNvirtualSubData,mtnrsrc))
			{
				ZERNRD(MASTER->passkey->fileno);
				ApplyPassKey(MASTER->passkey);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MASTER->passkey);
				if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,
					SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MASTER->passkey);
				} else {
					GetRDATData(MASTER->passkey->fileno,mtn->previous);
					filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
					setlists(mtnrsrc,mtn);
				}
			} else {
				GetRDATData(MASTER->passkey->fileno,mtn->previous);
				filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);
				setlists(mtnrsrc,mtn);
			}
			break;
	}
	getsupportingfiles(mtnrsrc,mtn,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
	if(mbl_pptclml!=NULL)
	{
		addrfcbrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	}
	addbtnrsrc(mtnrsrc,"LOAD VALUE",TRUE,loadvalue,
		"[MISCRATE][RATE EXPRESSION]");
	addbtnrsrc(mtnrsrc,"LOAD OPERATOR",TRUE,loadoperand,
		"[MISCRATE][RATE EXPRESSION]");
	addbtnrsrc(mtnrsrc,"LOAD GROUPER",TRUE,loadgrouper,
		"[MISCRATE][RATE EXPRESSION]");
	addbtnrsrc(mtnrsrc,"LOAD CONTROL",TRUE,loadcontrol,
		"[MISCRATE][RATE EXPRESSION]");
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_msr,mtn);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mtnrsrc);
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"LOAD VALUE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"LOAD CONTROL",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"LOAD GROUPER",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"LOAD OPERATOR",FALSE);
	}
	if(mbl_pptclml!=NULL)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	}
	ADVFMakePrevNextButtons(mtnrsrc,MASTER->passkey->fileno,getpreviouscb,
		mtn,getnextcb,mtn);
	MakeMasterButtons(mtnrsrc,MASTER);
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,
		(mbl_pptclml==NULL ? TRUE:FALSE));
}
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	short deftype=0;
	int selected=0;

	ZERNRD(MASTER->passkey->fileno);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MASTER->passkey->fileno,mtnrsrc);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selected);
	deftype=(short)selected;
	FINDFLDSETSHORT(MASTER->passkey->fileno,"DEFAULT DEFINITION TYPE",deftype);
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
	char *temp=NULL,delflag=FALSE;

	FINDFLDGETCHAR(MASTER->passkey->fileno,"DELETEFLAG",&delflag);
	if(!delflag)
	{
		if(chkaccountcode(mtnrsrc,mtn)) 
		{
			if(chkrevenuecode(mtnrsrc,mtn))
			{
				if(!chkcashcode(mtnrsrc,mtn)) return;
			} else return;
		} else return;
	}
	if(!SAVE_CHECK(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc))
	{
		return;
	}
	if(!isEMPTY(MASTER->save_expression))
	{
		if(EVALUATEbol(MASTER->save_expression,SCRNvirtualSubData,mtnrsrc))
		{
			temp=EVALUATEstr(MASTER->save_error_desc,
				SCRNvirtualSubData,mtnrsrc);
			ERRORDIALOG("SAVE ERROR",temp,NULL,FALSE);
			if(temp!=NULL) Rfree(temp);
			return;
		}
	}
	if(!isEMPTY(MASTER->save_warning))
	{
		if(EVALUATEbol(MASTER->save_warning,SCRNvirtualSubData,mtnrsrc))
		{
			temp=EVALUATEstr(MASTER->save_warning_desc,
				SCRNvirtualSubData,mtnrsrc);
			ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","SAVE WARNING",
				temp,(update_list ? save_record_up:save_record_nup),
				NULL,FALSE,2,mtnrsrc,mtn,NULL);
			if(temp!=NULL) Rfree(temp);
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
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE); 
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(MASTER->passkey->fileno,0,NULL,mtn->previous);
		DELETE_SUBORDINATES(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc);
		updatebrowse(update_list,mbl_pptclml,MASTER->passkey->fileno,module,mtnrsrc);
		if(mbl_pptclml!=NULL) quit_record(mtnrsrc,mtn);
			else GetRDATData(MASTER->passkey->fileno,mtn->previous);
	}
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	if(WindowCount==1)
	{
		if(MASTER!=NULL) 
		{
			CLOSE_MASTER_FILES(MASTER);
			FreeMaintainMaster(MASTER);
		}
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
	char *temp=NULL,*classid=NULL;

	readwidget(mtnrsrc,"[MISCRATE][CLASS IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[MISCRATE][CLASS IDENTIFICATION]",&classid);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"CLASS IDENTIFICATION",classid);
	if(classid!=NULL) Rfree(classid);
	readwidget(mtnrsrc,"[MISCRATE][RATE IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[MISCRATE][RATE IDENTIFICATION]",&temp);
	FINDFLDSETSTRING(MASTER->passkey->fileno,"RATE IDENTIFICATION",temp);
	if(temp!=NULL) Rfree(temp);
	if(ADVEQLNRDsec(MASTER->passkey->fileno,MASTER->passkey->keyno,
		SCRNvirtualSubData,mtnrsrc)) 
	{
		KEYNRD(MASTER->passkey->fileno,MASTER->passkey->keyno);
		ClearRDATData(mtn->previous);
	} else { 
		GetRDATData(MASTER->passkey->fileno,mtn->previous);
		filerecord2rsrc(MASTER->passkey->fileno,mtnrsrc);	
		setlists(mtnrsrc,mtn);
	}
	getsupportingfiles(mtnrsrc,mtn,FALSE);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void getsupportingfiles(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	SetSupportingFiles(mtnrsrc,MASTER,SCRNvirtualSubData,(void *)mtnrsrc,
		update,FALSE);
	if(update) updateSCRNvirtuals(mtnrsrc);
	else computeallSCRNvirtuals(mtnrsrc);
}
static void setlistscb(RDArsrc *mtnrsrc,MaintainMaster *MASTER,
	void (*SubFunc)(),void *args,mtnstruct *mtn)
{
	setlists(mtnrsrc,mtn);
}
static void getpreviouscb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1))
	{
		keyno=MASTER->passkey->keyno;
	}
	ADV2GET_PREVIOUS(member->parent,MASTER,mtn->previous,SCRNvirtualSubData,
		member->parent,keyno,NULL,NULL,
		setlistscb,mtn,NULL,NULL);
}
static void getnextcb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1))
	{
		keyno=MASTER->passkey->keyno;
	}
	ADV2GET_NEXT(member->parent,MASTER,mtn->previous,SCRNvirtualSubData,
		member->parent,keyno,NULL,NULL,
		setlistscb,mtn,NULL,NULL);
}
