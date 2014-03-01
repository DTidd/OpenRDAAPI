/* subdtyp.c - Xpert Position Addenda Browse */
/*lint -library */
#ifndef WIN32
#define __NAM__ "subdtyp.lnx"
#endif
#ifdef WIN32
#define __NAM__ "subdtyp.exe"
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

static void quit_record(RDArsrc *,mtnstruct *);
static void save_check(RDArsrc *,mtnstruct *,short);
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void save_record_up(RDArsrc *,mtnstruct *);
static void getrecord(RDArsrc *,mtnstruct *);
static void getrecordcb(RDArsrc *,mtnstruct *);
static void getsupportingrecords(RDArsrc *);
static void subdtypm(short,void *);
static void doexit(MakeBrowseList *),okfunc(MakeBrowseList *,void *);
static void new_record(RDArsrc *,MakeBrowseList *);
static short filenum=(-1),posnum=(-1);
static char editable=TRUE;
static char *module="SUBMGT";
static MakeBrowseList *mbl_subdtyp=NULL;
static MaintainMaster *MTNMASTER=NULL;
static void browse_subdtyp(RDArsrc *,mtnstruct *);
static void setdeftypes(RDArsrc *,mtnstruct *);
static void setPDEFtypes(RDArsrc *,mtnstruct *);
static void DEFtypecb(RDArsrc *,mtnstruct *);
static void PDEFtypecb(RDArsrc *,mtnstruct *);
static void changePDEFtype(RDArsrc *,mtnstruct *,short);
static void readscreen(RDArsrc *,mtnstruct *);
static void list2file(RDArsrc *,mtnstruct *);

static void AUEQLfunction(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER,void (*SubFunc)(...),
	void *args,mtnstruct *mtn)
{
	updatefilerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
	getsupportingrecords(mtnrsrc);
}
static void NEfunction(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER,void (*SubFunc)(...),
	void *args,mtnstruct *mtn)
{
	getsupportingrecords(mtnrsrc);
}
static void getpreviouscb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MTNMASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1))
	{
		keyno=MTNMASTER->passkey->keyno;
	}
	ADV2GET_PREVIOUS(member->parent,MTNMASTER,mtn->previous,SCRNvirtualSubData,member->parent,
		keyno,NEfunction,mtn,NULL,NULL,AUEQLfunction,mtn);
}
static void getnextcb(RDArmem *member,mtnstruct *mtn)
{
	short keyno=(-1);

	keyno=KEYNUMBER(MTNMASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1))
	{
		keyno=MTNMASTER->passkey->keyno;
	}
	ADV2GET_NEXT(member->parent,MTNMASTER,mtn->previous,SCRNvirtualSubData,member->parent,
		keyno,NEfunction,mtn,NULL,NULL,AUEQLfunction,mtn);
}
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
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN",
			"CHANGES NOT SAVED!",desc,save_record_quit,
			quit_record,FALSE,2,mtnrsrc,mtn,NULL);
		if(desc!=NULL) Rfree(desc);
	} else {
		quit_record(mtnrsrc,mtn);
	}
}
static void browse_subdtyp(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	if(mtn!=NULL)
	{
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
	mainrsrc=RDArsrcNEW(module,MTNMASTER->browse_screen);
	searchrsrc=RDArsrcNEW(module,MTNMASTER->search_browse);
	definelist=RDArsrcNEW(module,MTNMASTER->define_list);
	addDFincvir(mainrsrc,"POSTRK","POSMSTR",NULL,posnum);
	addDFincvir(searchrsrc,module,"SUBDTYP",NULL,filenum);
	addDFincvir(searchrsrc,"POSTRK","POSMSTR",NULL,posnum);
	addDFincvir(definelist,module,"SUBDTYP",NULL,filenum);
	addDFincvir(definelist,"POSTRK","POSMSTR",NULL,posnum);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	ApplyPassKey(MTNMASTER->passkey);
	if(posnum!=(-1))
	{
		COPYFIELD(filenum,posnum,"POSITION IDENTIFICATION");
		if(EQLNRDsec(posnum,1)) 
		{
			ZERNRD(posnum);
		} else {
			file2rsrc(posnum,mainrsrc,FALSE);
			filerecord2rsrc(posnum,mainrsrc);
		}
	}
	SCRNvirtual2rsrc(mainrsrc);
	computeallSCRNvirtuals(mainrsrc);
	MakeRunFunction(mainrsrc,MTNMASTER);
	mbl_subdtyp=ADVPbrowse(filenum,MTNMASTER->passkey->keyno,
		mainrsrc,searchrsrc,definelist,doexit,okfunc,
		new_record,NULL,FALSE,NULL,0,0,2,0,MTNMASTER->passkey);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	if(InitializeSubsystems(argc,argv,module,"MTN SUBDTYP")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if((filenum=APPOPNNRDEDITABLE(module,"SUBDTYP",&editable))==(-1)) return;
	if((posnum=APPOPNNRD("POSTRK","POSMSTR",TRUE,FALSE))==(-1)) return;
	MTNMASTER=MaintainMasterNew(module,"MTN SUBDTYP");
	if(findMaintainMaster(MTNMASTER))
	{
		ERRORDIALOG("MAINTAIN MTNMASTER NOT FOUND.","The Maintain Master function wasn't found.",NULL,TRUE); 
		prterr("Error Maintain Master Definition not found, this process cannot be executed without it's definition [%s] in the [%s.MTN] library.","MTN SUBDTYP",module);
		doexit(NULL);
	} else {
		MTNMASTER->passkey=READPassKey(filenum);
		switch(MTNMASTER->start_with)
		{
			default:
			case 0:
				subdtypm(1,NULL);
				break;
			case 1:
				browse_subdtyp(NULL,NULL);
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
	if(WindowCount==1)
	{
		if(filenum!=(-1)) CLSNRD(filenum);
		if(posnum!=(-1)) CLSNRD(posnum);
		if(MTNMASTER!=NULL) FreeMaintainMaster(MTNMASTER);
		ShutdownSubsystems();
	}
	mbl_subdtyp=NULL;
}
static void okfunc(MakeBrowseList *blist,void *targetkey)
{
	int run_func=0;

	if(!FINDRSCGETINT(blist->mainrsrc,"RUN FUNCTION",&run_func)) 
	{
		if(!run_func)
		{
			subdtypm(0,targetkey);
		} else {
			ExecuteListFunc(run_func-1,targetkey,MTNMASTER);
		}
	} else {
		subdtypm(0,targetkey);
	}
}
static void new_record(RDArsrc *mtnrsrc,MakeBrowseList *blist)
{
	subdtypm(2,NULL);
} 
static void subdtypm(short dowhich,void *targetkey)
{
	mtnstruct *mtn=NULL;
	int x=0,defno=0,y=0;
	RDAacct *acc=NULL;
	RDArsrc *mtnrsrc=NULL;
	NRDfield *fields=NULL,*fldx=NULL;
	short nofields=0,nokeys=0;
	NRDkey *keys=NULL;
	
	mtn=Rmalloc(sizeof(mtnstruct));
	nokeys=NUMKEYS(MTNMASTER->passkey->fileno);
	keys=KEYPOINTER(MTNMASTER->passkey->fileno);
	ZERNRD(filenum);
	mtn->previous=RDATDataNEW(filenum);
	mtnrsrc=RDArsrcNEW(module,MTNMASTER->maintain_screen);
	nofields=NUMFLDS(filenum);
	fields=FLDPOINTER(filenum);
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
				addlstrsrc(mtnrsrc,"DEFINITION TYPES",&defno,TRUE,NULL,mtn->DEFtypes->numlibs,&mtn->DEFtypes->libs,mtn);
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
				addlstrsrc(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",&defno,TRUE,NULL,mtn->PDEFtypes->numlibs,&mtn->PDEFtypes->libs,mtn);
			} else nonlstmakefld(mtnrsrc,filenum,fldx,editable);
		}
	}
	FINDRSCSETFUNC(mtnrsrc,"[SUBDTYP][POSITION IDENTIFICATION]",getrecordcb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"DEFINITION TYPES",DEFtypecb,mtn);
	FINDRSCSETFUNC(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",PDEFtypecb,mtn);
	if(posnum!=(-1)) file2rsrc(posnum,mtnrsrc,FALSE);
	DefaultScreens(mtnrsrc);
	switch(dowhich)
	{
		case 1:
			ApplyPassKey(MTNMASTER->passkey);
			if(EQLNRDsec(filenum,MTNMASTER->passkey->keyno))
			{
				KEYNRD(filenum,MTNMASTER->passkey->keyno);
				ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
				changePDEFtype(mtnrsrc,mtn,FALSE);
			} else {
				filerecord2rsrc(filenum,mtnrsrc);
				setPDEFtypes(mtnrsrc,mtn);
				GetRDATData(filenum,mtn->previous);
			}
			setdeftypes(mtnrsrc,mtn);
			break;
		case 2:
			ApplyPassKey(MTNMASTER->passkey);
			ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
			changePDEFtype(mtnrsrc,mtn,FALSE);
			setdeftypes(mtnrsrc,mtn);
			break;
		case 0:
		default:
			if(EQLNRDKEYsec(filenum,1,targetkey))
			{
				ZERNRD(filenum);
				ApplyPassKey(MTNMASTER->passkey);
				ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
				changePDEFtype(mtnrsrc,mtn,FALSE);
			} else {
				filerecord2rsrc(filenum,mtnrsrc);
				setPDEFtypes(mtnrsrc,mtn);
				GetRDATData(filenum,mtn->previous);
			}
			setdeftypes(mtnrsrc,mtn);
			break;
	}
	getsupportingrecords(mtnrsrc);
	addDFincvir(mtnrsrc,"POSTRK","POSMSTR",NULL,posnum);
	addDFincvir(mtnrsrc,module,"SUBDTYP",NULL,filenum);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	if(editable)
	{
		addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
		if(mbl_subdtyp!=NULL)
		{
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
		} else {
			addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
		}
	}
	FINDRSCSETCALLBACKTYPE(mtnrsrc,"[SUBDTYP][ACCOUNT CODE IMAGE]",TRUE);
	FINDRSCSETCALLBACKTYPE(mtnrsrc,"[SUBDTYP][DEBIT CODE IMAGE]",TRUE);
	FINDRSCSETCALLBACKTYPE(mtnrsrc,"[SUBDTYP][CREDIT CODE IMAGE]",TRUE);
	FINDRSCSETCALLBACKTYPE(mtnrsrc,"[SUBDTYP][PAYABLE CODE IMAGE]",TRUE);
	FINDRSCSETCALLBACKTYPE(mtnrsrc,"[SUBDTYP][CASH CODE IMAGE]",TRUE);
	FINDRSCSETCALLBACKTYPE(mtnrsrc,"[SUBDTYP][DUE FROM IMAGE]",TRUE);
	if(mbl_subdtyp==NULL) 
		addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_subdtyp,mtn);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mtnrsrc);
	ADVMakePrevNextButtons(mtnrsrc,keys,nokeys,getpreviouscb,mtn,getnextcb,mtn);
	MakeMasterButtons(mtnrsrc,MTNMASTER);
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_subdtyp==NULL?TRUE:FALSE));
}
void setdeftypes(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	short d=0;
	int x=0;

	FINDFLDGETSHORT(filenum,"DEFINITION TYPE",&d);
	x=d;
	if(mtn->DEFtypes->numlibs<=x)
	{
		x=0;
		d=0;
	}
	FINDRSCSETINT(mtnrsrc,"DEFINITION TYPES",x);
	FINDRSCSETDEFTYPE(mtnrsrc,"[SUBDTYP][ACCOUNT CODE IMAGE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[SUBDTYP][DEBIT CODE IMAGE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[SUBDTYP][CREDIT CODE IMAGE]",d);
}
static void setPDEFtypes(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	short d=0;
	int x=0;

	FINDFLDGETSHORT(filenum,"PAYROLL FUND DEFINITION TYPE",&d);
	x=d;
	if(mtn->PDEFtypes->numlibs<=x)
	{
		x=0;
		d=0;
	}
	FINDRSCSETINT(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",x);
	FINDRSCSETDEFTYPE(mtnrsrc,"[SUBDTYP][PAYABLE CODE IMAGE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[SUBDTYP][CASH CODE IMAGE]",d);
	FINDRSCSETDEFTYPE(mtnrsrc,"[SUBDTYP][DUE FROM IMAGE]",d);
}
static void PDEFtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	changePDEFtype(mtnrsrc,mtn,TRUE);	
}
static void changePDEFtype(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	int seld=0;
	short d1=0,d2=0;

	FINDRSCGETINT(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",&seld);
	FINDRSCGETDEFTYPE(mtnrsrc,"[SUBDTYP][PAYABLE CODE IMAGE]",&d1);
	d2=(short)seld;
	if(d1!=d2)
	{
		FINDRSCSETSTRING(mtnrsrc,"[SUBDTYP][PAYABLE CODE IMAGE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[SUBDTYP][CASH CODE IMAGE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[SUBDTYP][DUE FROM IMAGE]",NULL);
		FINDRSCSETDEFTYPE(mtnrsrc,"[SUBDTYP][PAYABLE CODE IMAGE]",d2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[SUBDTYP][CASH CODE IMAGE]",d2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[SUBDTYP][DUE FROM IMAGE]",d2);
		if(update)
		{
			updatersrc(mtnrsrc,"[SUBDTYP][PAYABLE CODE IMAGE]");
			updatersrc(mtnrsrc,"[SUBDTYP][CASH CODE IMAGE]");
			updatersrc(mtnrsrc,"[SUBDTYP][DUE FROM IMAGE]");
		}
	}
}
static void DEFtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int seld=0;
	short d1=0,d2=0;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&seld);
	FINDRSCGETDEFTYPE(mtnrsrc,"[SUBDTYP][ACCOUNT CODE IMAGE]",&d1);
	d2=(short)seld;
	if(d1!=d2)
	{
		FINDRSCSETSTRING(mtnrsrc,"[SUBDTYP][ACCOUNT CODE IMAGE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[SUBDTYP][DEBIT CODE IMAGE]",NULL);
		FINDRSCSETSTRING(mtnrsrc,"[SUBDTYP][CREDIT CODE IMAGE]",NULL);
		FINDRSCSETDEFTYPE(mtnrsrc,"[SUBDTYP][ACCOUNT CODE IMAGE]",d2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[SUBDTYP][DEBIT CODE IMAGE]",d2);
		FINDRSCSETDEFTYPE(mtnrsrc,"[SUBDTYP][CREDIT CODE IMAGE]",d2);
		updatersrc(mtnrsrc,"[SUBDTYP][ACCOUNT CODE IMAGE]");
		updatersrc(mtnrsrc,"[SUBDTYP][DEBIT CODE IMAGE]");
		updatersrc(mtnrsrc,"[SUBDTYP][CREDIT CODE IMAGE]");
	}
	getrecordcb(mtnrsrc,mtn);
}
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(filenum);
	readallwidgets(mtnrsrc);
	rsrc2filerecord(filenum,mtnrsrc);
	list2file(mtnrsrc,mtn);
}
static void list2file(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected=0;
	short type=0;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selected);
	type=(short)selected;
	FINDFLDSETSHORT(filenum,"DEFINITION TYPE",type);
	FINDRSCGETINT(mtnrsrc,"PAYROLL FUND DEFINITION TYPES",&selected);
	type=(short)selected;
	FINDFLDSETSHORT(filenum,"PAYROLL FUND DEFINITION TYPE",type);
}
static void save_record_up(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,TRUE);
}
static void save_record_nup(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,FALSE);
}
static void save_check(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	char delflag=FALSE,*warnmessage=NULL,*header=NULL;
	char *locid1=NULL;
	short return_value=FALSE;

	readscreen(mtnrsrc,mtn);
	FINDRSCGETCHAR(mtnrsrc,"[SUBDTYP][DELETEFLAG]",&delflag);
	FINDRSCGETSTRING(mtnrsrc,"[SUBDTYP][POSITION IDENTIFICATION]",&locid1);
	if(!delflag)
	{
		warnmessage=Rmalloc(500);
		header=Rmalloc(60);
		FINDFLDSETSTRING(posnum,"POSITION IDENTIFICATION",locid1);
		if(EQLNRDsec(posnum,1))
		{
			sprintf(warnmessage,"The Position Identification of [%s] is an invalid value.  It does not exist in the [POSTRK][POSMSTR] file.  Please select a valid Position Identification to save this record.",(locid1==NULL?"":locid1));
			sprintf(header,"INVALID POSITION IDENTIFICATION!");
			return_value=TRUE;
		} else {
			FINDFLDGETCHAR(posnum,"DELETEFLAG",&delflag);
			if(delflag)
			{
				sprintf(warnmessage,"The Position Identification of [%s] is deleted value.  It exist in the [POSTRK][POSMSTR] but is in a delted state.  Please select a valid Position Identification to save this record.",(locid1==NULL?"":locid1));
				sprintf(header,"DELETED POSITION IDENTIFICATION!");
				return_value=TRUE;
                        }
		}
	}
	if(return_value==TRUE)
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN",header,warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		if(header!=NULL) Rfree(header);
	}
	FINDFLDSETSTRING(filenum,"POSITION IDENTIFICATION",locid1);
	getsupportingrecords(mtnrsrc);
	if(locid1!=NULL) Rfree(locid1);
	if(return_value==FALSE) save_record(mtnrsrc,mtn,update_list);
}
static void save_record(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	char *rcddesc=NULL;

	if(RECORDsec(filenum))
	{
		rcddesc=RECORDsecDesc(filenum);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("SECURITY ACCESS DENIED!",rcddesc,NULL,FALSE); 
			prterr("Security Access Denied [%s].  Didn't write record.",rcddesc);
			Rfree(rcddesc);
		} else {
			prterr("Error didn't write record.");
		}
	} else {
		WRTTRANS(filenum,0,NULL,mtn->previous);
		updatebrowse(update_list,mbl_subdtyp,filenum,module,mtnrsrc);
		if(mbl_subdtyp!=NULL) quit_record(mtnrsrc,mtn);
		else GetRDATData(filenum,mtn->previous);
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
		if(filenum!=(-1)) CLSNRD(filenum);
		if(posnum!=(-1)) CLSNRD(posnum);
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
static void getrecordcb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *locid=NULL;
	short deftype=0;
	int x=0;

	readallwidgets(mtnrsrc);
	ZERNRD(filenum);
	FINDRSCGETSTRING(mtnrsrc,"[SUBDTYP][POSITION IDENTIFICATION]",&locid);
	FINDFLDSETSTRING(filenum,"POSITION IDENTIFICATION",locid);
	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&x);
	deftype=(int)x;
	FINDFLDSETSHORT(filenum,"DEFINITION TYPE",deftype);
	getrecord(mtnrsrc,mtn);
	if(locid!=NULL) Rfree(locid);
	setdeftypes(mtnrsrc,mtn);
	getsupportingrecords(mtnrsrc);
	computeallSCRNvirtuals(mtnrsrc);
   	updateallrsrc(mtnrsrc);
}
static void getrecord(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(EQLNRDsec(filenum,MTNMASTER->passkey->keyno)) 
	{
		KEYNRD(filenum,MTNMASTER->passkey->keyno);
		ClearRDATData(mtn->previous);
	} else {
		filerecord2rsrc(filenum,mtnrsrc);	
		setPDEFtypes(mtnrsrc,mtn);
		GetRDATData(filenum,mtn->previous);
	}
}
static void getsupportingrecords(RDArsrc *mtnrsrc)
{
	if(posnum!=(-1))
	{
		COPYFIELD(filenum,posnum,"POSITION IDENTIFICATION");
		if(EQLNRDsec(posnum,1)) KEYNRD(posnum,1);
		filerecord2rsrc(posnum,mtnrsrc);
	}
}
