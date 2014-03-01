/* restmstm.c - Real Estate Master Maintain Screen */
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
	APPlib *books;
	APPlib *class;
	APPlib *districts;
	APPlib *lands;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern short filenum,classnum,deednum,distnum,landnum;
extern char editable;
extern char *module;
extern void browse_msr(RDArsrc *,mtnstruct *);
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_check(RDArsrc *,mtnstruct *,short);
static void save_record_quit(RDArsrc *,mtnstruct *);
static void readscreen(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void quit_record(RDArsrc *,mtnstruct *);
static void quit_record(RDArsrc *,mtnstruct *);
static void quit_record(RDArsrc *,mtnstruct *);
static void getrecord(RDArsrc *,mtnstruct *);
MakeBrowseList *mbl_restmst=NULL;
extern MaintainMaster *MASTER;
void restmstm(short,void *targetkey);
static void getsupportingfiles(RDArsrc *,mtnstruct *);
static void setlists(RDArsrc *,mtnstruct *),dobooklist(RDArsrc *,mtnstruct *);
static void doclasslist(RDArsrc *,mtnstruct *);
static void dodistrictlist(RDArsrc *,mtnstruct *);
static void dolandlist(RDArsrc *,mtnstruct *);
static void changeclass(RDArsrc *,mtnstruct *);
static void changelanduse(RDArsrc *,mtnstruct *);
static void changebooks(RDArsrc *,mtnstruct *);
static void changedistrict(RDArsrc *,mtnstruct *);
static void list2file(RDArsrc *,mtnstruct *);

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
static void dobooklist(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn->books!=NULL)
	{ 
		freeapplib(mtn->books);
		mtn->books=NULL; 
	}
	ZERNRD(deednum);
	mtn->books=ADVMakeNRDlistAllSTRING(deednum,module,"Creating Deed Book List",
		1,"BOOK IDENTIFICATION",SCRNvirtualSubData,mtnrsrc);
	if(mtn->books->numlibs<1)
	{
		addAPPlib(mtn->books,"No Deed Books Defined");
	}
}
static void doclasslist(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn->class!=NULL)
	{ 
		freeapplib(mtn->class);
		mtn->class=NULL; 
	}
	ZERNRD(classnum);
	mtn->class=ADVMakeNRDlistAllSTRING(classnum,module,"Creating Class List",
		1,"CLASS IDENTIFICATION",SCRNvirtualSubData,mtnrsrc);
	if(mtn->class->numlibs<1)
	{
		addAPPlib(mtn->class,"No Class Identifications");
	}
}
static void dodistrictlist(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn->districts!=NULL)
	{ 
		freeapplib(mtn->districts);
		mtn->districts=NULL; 
	}
	ZERNRD(distnum);
	mtn->districts=ADVMakeNRDlistAllSTRING(distnum,module,
		"Creating District List",1,"DISTRICT IDENTIFICATION",
		SCRNvirtualSubData,mtnrsrc);
	if(mtn->districts->numlibs<1)
	{
		addAPPlib(mtn->districts,"No Districts Defined");
	}
}
static void dolandlist(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn->lands!=NULL)
	{ 
		freeapplib(mtn->lands);
		mtn->lands=NULL; 
	}
	ZERNRD(landnum);
	mtn->lands=ADVMakeNRDlistAllSTRING(landnum,module,
		"Creating Land Use Value Code List",1,"LAND USE VALUE CODE",
		SCRNvirtualSubData,mtnrsrc);
	if(mtn->lands->numlibs<1)
	{
		addAPPlib(mtn->lands,"No Land Use Value Codes Defined");
	}
}
static void setlists(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *temp=NULL;
	int x=0;

	if(deednum!=(-1))
	{
		FINDFLDGETSTRING(filenum,"BOOK IDENTIFICATION",&temp);
		x=FINDFLDLISTELEMENT(deednum,"BOOK IDENTIFICATION",mtn->books,temp);
		if(x==(-1)) x=0;
		FINDRSCSETINT(mtnrsrc,"BOOK IDENTIFICATIONS",x);
	}
	if(classnum!=(-1))
	{
		FINDFLDGETSTRING(filenum,"CLASS IDENTIFICATION",&temp);
		x=FINDFLDLISTELEMENT(classnum,"CLASS IDENTIFICATION",mtn->class,temp);
		if(x==(-1)) x=0;
		FINDRSCSETINT(mtnrsrc,"CLASS IDENTIFICATIONS",x);
	}
	if(distnum!=(-1))
	{
		FINDFLDGETSTRING(filenum,"DISTRICT IDENTIFICATION",&temp);
		x=FINDFLDLISTELEMENT(distnum,"DISTRICT IDENTIFICATION",
			mtn->districts,temp);
		if(x==(-1)) x=0;
		FINDRSCSETINT(mtnrsrc,"DISTRICT IDENTIFICATIONS",x);
	}
	if(landnum!=(-1))
	{
		FINDFLDGETSTRING(filenum,"LAND USE VALUE CODE",&temp);
		x=FINDFLDLISTELEMENT(landnum,"LAND USE VALUE CODE",mtn->lands,temp);
		if(x==(-1)) x=0;
		FINDRSCSETINT(mtnrsrc,"LAND USE VALUE CODES",x);
	}
	if(temp!=NULL) Rfree(temp);
}
static void changeclass(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected=0;

	ZERNRD(classnum);
	FINDRSCGETINT(mtnrsrc,"CLASS IDENTIFICATIONS",&selected);
	if(RDAstrcmp(mtn->class->libs[selected],"No Class Identifications"))
	{
		FINDFLDSETSTRING(classnum,"CLASS IDENTIFICATION",
			mtn->class->libs[selected]);
		if(ADVEQLNRDsec(classnum,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(classnum,1);
		}
	}
	updatefilerecord2rsrc(classnum,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
static void changedistrict(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected=0;

	ZERNRD(distnum);
	FINDRSCGETINT(mtnrsrc,"DISTRICT IDENTIFICATIONS",&selected);
	if(RDAstrcmp(mtn->districts->libs[selected],"No Districts Defined"))
	{
		FINDFLDSETSTRING(distnum,"DISTRICT IDENTIFICATION",
			mtn->districts->libs[selected]);
		if(ADVEQLNRDsec(distnum,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(distnum,1);
		}
	}
	updatefilerecord2rsrc(distnum,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
static void changebooks(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected=0;

	ZERNRD(deednum);
	FINDRSCGETINT(mtnrsrc,"BOOK IDENTIFICATIONS",&selected);
	if(RDAstrcmp(mtn->books->libs[selected],"No Deed Books Defined"))
	{
		FINDFLDSETSTRING(deednum,"BOOK IDENTIFICATION",
			mtn->books->libs[selected]);
		if(ADVEQLNRDsec(deednum,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(deednum,1);
		}
	}
	updatefilerecord2rsrc(deednum,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
static void changelanduse(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected=0;

	ZERNRD(landnum);
	FINDRSCGETINT(mtnrsrc,"LAND USE VALUE CODES",&selected);
	if(RDAstrcmp(mtn->lands->libs[selected],"No Land Use Value Codes Defined"))
	{
		FINDFLDSETSTRING(landnum,"LAND USE VALUE CODE",
			mtn->lands->libs[selected]);
		if(ADVEQLNRDsec(landnum,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(landnum,1);
		}
	}
	updatefilerecord2rsrc(landnum,mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
void restmstm(short dowhich,void *targetkey)
{
	RDArsrc *mtnrsrc=NULL;
	int nofields=0,x,which_type=0;
	NRDfield *fields=NULL,*fldx;
	short edit_rsrc=FALSE;
	mtnstruct *mtn=NULL;
	
	mtnrsrc=RDArsrcNEW(module,MASTER->maintain_screen);
	addDFincvir(mtnrsrc,module,"RESTMST",NULL,filenum);
	addDFincvir(mtnrsrc,module,"RESTLNDV",NULL,landnum);
	addDFincvir(mtnrsrc,module,"RESTDIST",NULL,distnum);
	addDFincvir(mtnrsrc,module,"RESTDEED",NULL,deednum);
	addDFincvir(mtnrsrc,module,"RESTCLAS",NULL,classnum);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->books=NULL;
	mtn->class=NULL;
	mtn->districts=NULL;
	mtn->lands=NULL;
	ZERNRD(filenum);
	mtn->previous=RDATDataNEW(filenum);
	nofields=NUMFLDS(filenum);
	fields=FLDPOINTER(filenum);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=(short)editable;
			if(!RDAstrcmp(fldx->name,"BOOK IDENTIFICATION"))
			{
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					dobooklist(mtnrsrc,mtn);
					addlstrsrc(mtnrsrc,"BOOK IDENTIFICATIONS",
						&which_type,(edit_rsrc ? TRUE:FALSE),
						changebooks,mtn->books->numlibs,
						&mtn->books->libs,mtn);
				}
			} else if(!RDAstrcmp(fldx->name,"CLASS IDENTIFICATION"))
			{
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					doclasslist(mtnrsrc,mtn);
					addlstrsrc(mtnrsrc,"CLASS IDENTIFICATIONS",
						&which_type,(edit_rsrc ? TRUE:FALSE),
						changeclass,mtn->class->numlibs,
						&mtn->class->libs,mtn);
				}
			} else if(!RDAstrcmp(fldx->name,"DISTRICT IDENTIFICATION"))
			{
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					dodistrictlist(mtnrsrc,mtn);
					addlstrsrc(mtnrsrc,"DISTRICT IDENTIFICATIONS",
						&which_type,(edit_rsrc ? TRUE:FALSE),
						changedistrict,mtn->districts->numlibs,
						&mtn->districts->libs,mtn);
				}
			} else if(!RDAstrcmp(fldx->name,"LAND USE VALUE CODE"))
			{
				if(FIELDscrn(filenum,fldx->name,&edit_rsrc))
				{
					dolandlist(mtnrsrc,mtn);
					addlstrsrc(mtnrsrc,"LAND USE VALUE CODES",
						&which_type,(edit_rsrc ? TRUE:FALSE),
						changelanduse,mtn->lands->numlibs,
						&mtn->lands->libs,mtn);
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
	FINDRSCSETEDITABLE(mtnrsrc,"[RESTMSTR][STATEMENT ADDRESS 1]",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"[RESTMSTR][STATEMENT ADDRESS 2]",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"[RESTMSTR][STATEMENT CITY]",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"[RESTMSTR][STATEMENT STATE]",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"[RESTMSTR][STATEMENT ZIP CODE]",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"[RESTMSTR][OWNER IDENTIFICATION]",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"[RESTMSTR][REAL ESTATE IDENTIFICATION]",TRUE);
	FINDRSCSETEDITABLE(mtnrsrc,"[RESTMSTR][TAX YEAR]",TRUE);
	nofields=0;
	file2rsrc(deednum,mtnrsrc,FALSE);
	file2rsrc(classnum,mtnrsrc,FALSE);
	file2rsrc(distnum,mtnrsrc,FALSE);
	file2rsrc(landnum,mtnrsrc,FALSE);
	FINDRSCSETFUNC(mtnrsrc,"[RESTMST][REAL ESTATE IDENTIFICATION]",
		getrecord,mtn);
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
				setlists(mtnrsrc,mtn);
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
				setlists(mtnrsrc,mtn);
			}
			break;
	}
	getsupportingfiles(mtnrsrc,mtn);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	if(editable)
	{
		addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
		if(mbl_restmst!=NULL)
		{
			addrfcbrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
		} else {
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
		}
	}
	if(mbl_restmst==NULL)
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
		(mbl_restmst==NULL ? TRUE:FALSE));
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
	ZERNRD(filenum);
	readallwidgets(mtnrsrc);
	rsrc2filerecord(filenum,mtnrsrc);
	list2file(mtnrsrc,mtn);
}
static void save_check(RDArsrc *mtnrsrc,mtnstruct *mtn,
	short update_list)
{
	char *temp=NULL,*warnmessage=NULL,delflag=FALSE;

	FINDFLDGETCHAR(filenum,"DELETEFLAG",&delflag);
	if(!delflag)
	{
		if(deednum!=(-1))
		{
			FINDFLDGETSTRING(filenum,"BOOK IDENTIFICATION",&temp);
			FINDFLDSETSTRING(deednum,"BOOK IDENTIFICATION",temp);
			if(ADVEQLNRDsec(deednum,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(deednum,1);
				warnmessage=Rmalloc(RDAstrlen(temp)+100);
				sprintf(warnmessage,"The BOOK IDENTIFICATION of [%s] entered is not valid.  It does not exist as a DEED BOOK.",(temp==NULL?"":temp));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID BOOK IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr("The BOOK IDENTIFICATION of [%s] entered does not exist in the [RLSTMGT][RESTDEED] data file.",(temp==NULL ? "":temp));
				if(temp!=NULL) Rfree(temp); 
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(deednum,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(temp)+140);
					sprintf(warnmessage,"The BOOK IDENTIFICATION of [%s] entered is deleted.  It exists as a DEED BOOK, but is presently set up in a deleted state.",(temp==NULL ? "":temp));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED BOOK IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					prterr("The BOOK IDENTIFICATION of [%s] entered is deleted.  It exist as a DEED BOOK, but is presently set up in a deleted state.",(temp==NULL?"":temp));
					if(temp!=NULL) Rfree(temp); 
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				}
			}
		}
		if(classnum!=(-1))
		{
			FINDFLDGETSTRING(filenum,"CLASS IDENTIFICATION",&temp);
			FINDFLDSETSTRING(classnum,"CLASS IDENTIFICATION",temp);
			if(ADVEQLNRDsec(classnum,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(classnum,1);
				warnmessage=Rmalloc(RDAstrlen(temp)+100);
				sprintf(warnmessage,"The CLASS IDENTIFICATION of [%s] entered is not valid.  It does not exist as a Classification.",(temp==NULL?"":temp));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CLASS IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr("The CLASS IDENTIFICATION of [%s] entered does not exist in the [RLSTMGT][RESTCLAS] data file.",(temp==NULL ? "":temp));
				if(temp!=NULL) Rfree(temp); 
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(classnum,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(temp)+140);
					sprintf(warnmessage,"The CLASS IDENTIFICATION of [%s] entered is deleted.  It exists as a Classification, but is presently set up in a deleted state.",(temp==NULL ? "":temp));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED CLASS IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					prterr("The CLASS IDENTIFICATION of [%s] entered is deleted.  It exists as a Classification, but is presently set up in a deleted state.",(temp==NULL?"":temp));
					if(temp!=NULL) Rfree(temp); 
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				}
			}
		}
		if(distnum!=(-1))
		{
			FINDFLDGETSTRING(filenum,"DISTRICT IDENTIFICATION",&temp);
			FINDFLDSETSTRING(distnum,"DISTRICT IDENTIFICATION",temp);
			if(ADVEQLNRDsec(distnum,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(distnum,1);
				warnmessage=Rmalloc(RDAstrlen(temp)+100);
				sprintf(warnmessage,"The DISTRICT IDENTIFICATION of [%s] entered is not valid.  It does not exist as a District.",(temp==NULL?"":temp));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DISTRICT IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr("The DISTRICT IDENTIFICATION of [%s] entered does not exist in the [RLSTMGT][RESTDIST] data file.",(temp==NULL ? "":temp));
				if(temp!=NULL) Rfree(temp); 
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(distnum,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(temp)+140);
					sprintf(warnmessage,"The DISTRICT IDENTIFICATION of [%s] entered is deleted.  It exists as a District, but is presently set up in a deleted state.",(temp==NULL ? "":temp));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED DISTRICT IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
					prterr("The DISTRICT IDENTIFICATION of [%s] entered is deleted.  It exist as a District, but is presently set up in a deleted state.",(temp==NULL?"":temp));
					if(temp!=NULL) Rfree(temp); 
					if(warnmessage!=NULL) Rfree(warnmessage);
					return;
				}
			}
		}
		if(landnum!=(-1))
		{
			FINDFLDGETSTRING(filenum,"LAND USE VALUE CODE",&temp);
			FINDFLDSETSTRING(landnum,"LAND USE VALUE CODE",temp);
			if(ADVEQLNRDsec(landnum,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(landnum,1);
				warnmessage=Rmalloc(RDAstrlen(temp)+100);
				sprintf(warnmessage,"The LAND USE VALUE CODE of [%s] entered is not valid.  It does not exist as a Land Use Value Code.",(temp==NULL?"":temp));
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID LAND USE VALUE CODE!",warnmessage,NULL,NULL,TRUE,NULL);
				prterr("The LAND USE VALUE CODE of [%s] entered does not exist in the [RLSTMGT][RESTLNDV] data file.",(temp==NULL ? "":temp));
				if(temp!=NULL) Rfree(temp); 
				if(warnmessage!=NULL) Rfree(warnmessage);
				return;
			} else {
				FINDFLDGETCHAR(landnum,"DELETEFLAG",&delflag);
				if(delflag)
				{
					warnmessage=Rmalloc(RDAstrlen(temp)+140);
					sprintf(warnmessage,"The LAND USE VALUE CODE of [%s] entered is deleted.  It exists as a Land Use Value Code, but is presently set up in a deleted state.",(temp==NULL ? "":temp));
					WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED LAND USE VALUE CODE!",warnmessage,NULL,NULL,TRUE,NULL);
					prterr("The LAND USE VALUE CODE of [%s] entered is deleted.  It exist as a Land Use Value Code, but is presently set up in a deleted state.",(temp==NULL?"":temp));
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
static void list2file(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected=0;

	FINDRSCGETINT(mtnrsrc,"DISTRICT IDENTIFICATIONS",&selected);
	if(RDAstrcmp(mtn->districts->libs[selected],"No Districts Defined"))
	{
		FINDFLDSETSTRING(filenum,"DISTRICT IDENTIFICATION",
			mtn->districts->libs[selected]);
	}
	FINDRSCGETINT(mtnrsrc,"BOOK IDENTIFICATIONS",&selected);
	if(RDAstrcmp(mtn->books->libs[selected],"No Deed Books Defined"))
	{
		FINDFLDSETSTRING(filenum,"BOOK IDENTIFICATION",
			mtn->books->libs[selected]);
	}
	FINDRSCGETINT(mtnrsrc,"CLASS IDENTIFICATIONS",&selected);
	if(RDAstrcmp(mtn->class->libs[selected],"No Class Identifications"))
	{
		FINDFLDSETSTRING(filenum,"CLASS IDENTIFICATION",
			mtn->class->libs[selected]);
	}
	FINDRSCGETINT(mtnrsrc,"LAND USE VALUE CODES",&selected);
	if(RDAstrcmp(mtn->lands->libs[selected],"No Land Use Value Codes Defined"))
	{
		FINDFLDSETSTRING(filenum,"LAND USE VALUE CODE",
			mtn->lands->libs[selected]);
	}
}
static void save_record(RDArsrc *mtnrsrc,mtnstruct *mtn,
	short update_list)
{
	char *rcddesc=NULL,*realestateid1=NULL,*realestateid2=NULL;
	char *realestateid3=NULL,*realestateid5=NULL;
	char *realestateid4=NULL;
	char *ownid1=NULL,*ownid2=NULL,*ownid3=NULL,*ownid4=NULL;
	char deleteflag=0,mdeleteflag=0,pdeleteflag=0,vdeleteflag=0;
	char tdeleteflag=0,dist=FALSE;
	short ynum=0,yrnum=0,paidnum=0,ownnum=0;
	short ryrnum=0,byrnum=0,trannum=0,taxdwr=0;
	short ef_yr=0,ef_paid=0,ef_tran=0,ef_own=0;
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
			FINDFLDGETSTRING(filenum,"REAL ESTATE IDENTIFICATION",&realestateid1);
			ynum=OPNNRDsec(module,"RESTYEAR",TRUE,TRUE,FALSE);	
			taxdwr=OPNNRDsec("TAXCLT","TAXDRWR",TRUE,TRUE,FALSE);
			ownnum=OPNNRDsec(module,"RESTOWN",TRUE,TRUE,FALSE);	
			yrnum=OPNNRDsec(module,"RESTOYR",TRUE,TRUE,FALSE);
			trannum=OPNNRDsec(module,"RESTRAN",TRUE,TRUE,FALSE);
			paidnum=OPNNRDsec(module,"RESTPAID",TRUE,TRUE,FALSE);
			ryrnum=OPNNRDsec("FINMGT","FINRYR",TRUE,TRUE,FALSE);
			byrnum=OPNNRDsec("FINMGT","FINBYR",TRUE,TRUE,FALSE);
			tmprsrc=RDArsrcNEW(module,NULL);
			addDFincvir(tmprsrc,module,"RESTMST",NULL,filenum);
			addDFincvir(tmprsrc,module,"RESTYEAR",NULL,ynum);
			addDFincvir(tmprsrc,"TAXCLT","TAXDRWR",NULL,taxdwr);
			addDFincvir(tmprsrc,module,"RESTOWN",NULL,ownnum);
			addDFincvir(tmprsrc,module,"RESTOYR",NULL,yrnum);
			addDFincvir(tmprsrc,module,"RESTRAN",NULL,trannum);
			addDFincvir(tmprsrc,module,"RESTPAID",NULL,paidnum);
			addDFincvir(tmprsrc,"FINMGT","FINRYR",NULL,ryrnum);
			addDFincvir(tmprsrc,"FINMGT","FINBYR",NULL,byrnum);
			GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
			if(ynum!=(-1))
			{
				ZERNRD(ynum);
				FINDFLDSETSTRING(ynum,"REAL ESTATE IDENTIFICATION",realestateid1);
				ef_yr=ADVGTENRDsec(ynum,1,SCRNvirtualSubData,tmprsrc);
				while(!ef_yr)
				{
					FINDFLDGETCHAR(ynum,"DELETEFLAG",&mdeleteflag);
					if(!mdeleteflag)
					{
						FINDFLDGETSTRING(ynum,"REAL ESTATE IDENTIFICATION",&realestateid2);
						if(RDAstrcmp(realestateid1,realestateid2)) break;
						prev=RDATDataNEW(ynum);
						FINDFLDSETCHAR(ynum,"DELETEFLAG",deleteflag);
						ADVWRTTRANSsec(ynum,0,NULL,prev,SCRNvirtualSubData,tmprsrc);		
						if(prev!=NULL) FreeRDATData(prev);
					}
					ef_yr=ADVNXTNRDsec(ynum,1,SCRNvirtualSubData,tmprsrc);
					if(realestateid2!=NULL) 
					{ 
						Rfree(realestateid2); 
						realestateid2=NULL; 
					} 
				}
			}
			if(ownnum!=(-1))
			{
				ZERNRD(ownnum);
				FINDFLDSETSTRING(ownnum,"REAL ESTATE IDENTIFICATION",realestateid1);
				ef_own=ADVGTENRDsec(ownnum,1,SCRNvirtualSubData,tmprsrc);
				while(!ef_own)
				{
					FINDFLDGETCHAR(ownnum,"DELETEFLAG",&mdeleteflag);
					if(!mdeleteflag)
					{
						FINDFLDGETSTRING(ownnum,"REAL ESTATE IDENTIFICATION",&realestateid2);
						if(RDAstrcmp(realestateid1,realestateid2)) break;
						FINDFLDGETSTRING(ownnum,"OWNER IDENTIFICATION",&ownid1);
						prev=RDATDataNEW(ownnum);
						FINDFLDSETCHAR(ownnum,"DELETEFLAG",deleteflag);
						ADVWRTTRANSsec(ownnum,0,NULL,prev,SCRNvirtualSubData,tmprsrc);
						if(prev!=NULL) FreeRDATData(prev);
						if(yrnum!=(-1))
						{
							ZERNRD(yrnum);
							FINDFLDSETSTRING(yrnum,"REAL ESTATE IDENTIFICATION",realestateid1);
							FINDFLDSETSTRING(yrnum,"OWNER IDENTIFICATION",ownid1);
							ef_yr=ADVGTENRDsec(yrnum,1,SCRNvirtualSubData,tmprsrc);
							while(!ef_yr)
							{
								FINDFLDGETCHAR(yrnum,"DELETEFLAG",&pdeleteflag);
								if(!pdeleteflag)
								{
									FINDFLDGETSTRING(yrnum,"REAL ESTATE IDENTIFICATION",&realestateid3);
									FINDFLDGETSTRING(yrnum,"OWNER IDENTIFICATION",&ownid2);
									if(RDAstrcmp(realestateid3,realestateid1) || RDAstrcmp(ownid1,ownid2)) break;
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
												FINDFLDGETSTRING(trannum,"REAL ESTATE IDENTIFICATION",&realestateid4);
												FINDFLDGETSTRING(trannum,"OWNER IDENTIFICATION",&ownid3);
												FINDFLDGETINT(trannum,"FISCAL YEAR",&yearid2);
												if(RDAstrcmp(realestateid1,realestateid4) || RDAstrcmp(ownid1,ownid3) || yearid1!=yearid2) break;
												FINDFLDGETINT(trannum,"TRANSACTION NUMBER",&tran1);
												prev=RDATDataNEW(trannum);
												FINDFLDGETCHAR(trannum,"UPDATED",&vdeleteflag);
												if(!vdeleteflag)
												{
													FINDFLDGETDOUBLE(trannum,"AMOUNT",&amount);
													ADVUpdateRealEstateTransactions(filenum,ynum,ownnum,yrnum,trannum,-amount,SCRNvirtualSubData,tmprsrc);
												}
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
															FINDFLDGETSTRING(paidnum,"REAL ESTATE IDENTIFICATION",&realestateid5);
															FINDFLDGETSTRING(paidnum,"OWNER IDENTIFICATION",&ownid4);
															FINDFLDGETINT(paidnum,"FISCAL YEAR",&yearid3);
															FINDFLDGETINT(paidnum,"TRANSACTION NUMBER",&tran2);
															if(RDAstrcmp(realestateid5,realestateid1) || RDAstrcmp(ownid1,ownid4) || yearid1!=yearid3 || tran1!=tran2) break;
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
															ADVUpdateRealEstatePayments(taxdwr,filenum,ynum,ownnum,yrnum,trannum,paidnum,-amount,SCRNvirtualSubData,tmprsrc);
															FINDFLDSETCHAR(paidnum,"DELETEFLAG",deleteflag);
															ADVWRTTRANSsec(paidnum,0,NULL,prev,SCRNvirtualSubData,tmprsrc);		
															if(prev!=NULL) FreeRDATData(prev);
																																		}
														ef_paid=ADVNXTNRDsec(paidnum,1,SCRNvirtualSubData,tmprsrc);
													}
													ef_tran=ADVNXTNRDsec(trannum,1,SCRNvirtualSubData,tmprsrc);
												}
												ef_yr=ADVNXTNRDsec(yrnum,1,SCRNvirtualSubData,tmprsrc);
											}
										}
									}
									ef_own=ADVNXTNRDsec(ownnum,1,SCRNvirtualSubData,tmprsrc); 	
								}
							}
						}
					}
				}
			}
			if(tmprsrc!=NULL) free_rsrc(tmprsrc);
			if(taxdwr!=(-1)) CLSNRD(taxdwr);
			if(ownnum!=(-1)) CLSNRD(ownnum);
			if(yrnum!=(-1)) CLSNRD(yrnum);
			if(ynum!=(-1)) CLSNRD(ynum);
			if(trannum!=(-1)) CLSNRD(trannum);
			if(paidnum!=(-1)) CLSNRD(paidnum);
			if(ryrnum!=(-1)) CLSNRD(ryrnum);
			if(byrnum!=(-1)) CLSNRD(byrnum);
		}
		if(realestateid1!=NULL) Rfree(realestateid1);
		if(realestateid2!=NULL) Rfree(realestateid2);
		if(realestateid3!=NULL) Rfree(realestateid3);
		if(realestateid4!=NULL) Rfree(realestateid4);
		if(realestateid5!=NULL) Rfree(realestateid5);
		if(ownid1!=NULL) Rfree(ownid1);
		if(ownid2!=NULL) Rfree(ownid2);
		if(ownid3!=NULL) Rfree(ownid3);
		if(ownid4!=NULL) Rfree(ownid4);
		updatebrowse(update_list,mbl_restmst,filenum,module,mtnrsrc);
		if(mbl_restmst!=NULL) quit_record(mtnrsrc,mtn);
			else GetRDATData(filenum,mtn->previous);
	}
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		if(mtn->books!=NULL) freeapplib(mtn->books);
		if(mtn->class!=NULL) freeapplib(mtn->class);
		if(mtn->districts!=NULL) freeapplib(mtn->districts);
		if(mtn->lands!=NULL) freeapplib(mtn->lands);
		Rfree(mtn);
	}
	if(WindowCount==1)
	{
		if(filenum!=(-1)) CLSNRD(filenum);
		if(landnum!=(-1)) CLSNRD(landnum);
		if(distnum!=(-1)) CLSNRD(distnum);
		if(deednum!=(-1)) CLSNRD(deednum);
		if(classnum!=(-1)) CLSNRD(classnum);
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
	char *realestateid1=NULL;

	readwidget(mtnrsrc,"[RESTMST][REAL ESTATE IDENTIFICATION]");
	FINDRSCGETSTRING(mtnrsrc,"[RESTMST][REAL ESTATE IDENTIFICATION]",&realestateid1);
	FINDFLDSETSTRING(filenum,"REAL ESTATE IDENTIFICATION",realestateid1);
	if(ADVEQLNRDsec(filenum,MASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc)) 
	{
		KEYNRD(filenum,MASTER->passkey->keyno);
		ClearRDATData(mtn->previous);
		list2file(mtnrsrc,mtn);
	} else { 
		GetRDATData(filenum,mtn->previous);
		setlists(mtnrsrc,mtn);
		filerecord2rsrc(filenum,mtnrsrc);	
	}
	if(realestateid1!=NULL) Rfree(realestateid1);
	getsupportingfiles(mtnrsrc,mtn);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
static void getsupportingfiles(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(deednum!=(-1))
	{
		COPYFIELD(filenum,deednum,"BOOK IDENTIFICATION");
		if(ADVEQLNRDsec(deednum,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(deednum,1);
			changebooks(mtnrsrc,mtn);
		}
		filerecord2rsrc(deednum,mtnrsrc);
	}
	if(classnum!=(-1))
	{
		COPYFIELD(filenum,classnum,"CLASS IDENTIFICATION");
		if(ADVEQLNRDsec(classnum,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(classnum,1);
			changeclass(mtnrsrc,mtn);
		}
		filerecord2rsrc(classnum,mtnrsrc);
	}
	if(distnum!=(-1))
	{
		COPYFIELD(filenum,distnum,"DISTRICT IDENTIFICATION");
		if(ADVEQLNRDsec(distnum,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(distnum,1);
			changedistrict(mtnrsrc,mtn);
		}
		filerecord2rsrc(distnum,mtnrsrc);
	}
	if(landnum!=(-1))
	{
		COPYFIELD(filenum,landnum,"LAND USE VALUE CODE");
		if(ADVEQLNRDsec(landnum,1,SCRNvirtualSubData,mtnrsrc))
		{
			KEYNRD(landnum,1);
			changelanduse(mtnrsrc,mtn);
		}
		filerecord2rsrc(landnum,mtnrsrc);
	}
}
