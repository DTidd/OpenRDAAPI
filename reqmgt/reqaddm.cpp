/* reqaddm.c - Add Requisition Maintain Screen */
#include <cstdio>


#include <mix.hpp>
#include <fin.hpp>

int REQ_COUNT=0; 
MakeBrowseList *mbl_reqmst=NULL;
MakeBrowseList *mbl_venmsr=NULL;
APPlib *locations=NULL;
void reqmstm(short,void *targetkey);
void getsupportingrecords(RDArsrc *,char);
extern APPlib *acctavl,*debitavl,*creditavl;
extern APPlib *defdebitavl,*defcreditavl,*defpayableavl;
extern APPlib *BANKids;
extern APPlib *DEFtypes;
extern char *module;
extern void additemsmultiple(void);
extern void additemssingle(void);
extern short reqmst,reqloc,reqven,reqpven,vennum,reqcat,reqctlg;
extern short reqitm,reqacc,expnum,balnum,eyrnum,byrnum,finyrnum,venadrnum;
RDAfinmgt *finsetup=NULL;
extern char *reportname;
extern char *reqid,*locid;
extern void browse_msr(RDArsrc *,RDATData *);
static short save_check_save(RDArsrc *,RDATData *);
static void save_check_savecb(RDArsrc *,RDATData *);
static void save_check_save_quit(RDArsrc *,RDATData *);
static short save_check(RDArsrc *,short);
static void save_record(RDArsrc *,RDATData *);
static void quit_record(RDArsrc *,RDATData *);
static void getrecord(RDArsrc *,RDATData *);
static void addreqitmmultiple(RDArsrc *,RDATData *);
static void addreqitmsingle(RDArsrc *,RDATData *);
static void set_defaults(RDArsrc *);
static short check_reqid(RDArsrc *,char *,char *,RDATData *);
void setlocationids(RDArsrc *);

static void quit_record_test(RDArsrc *mtnrsrc,RDATData *previous)
{
	char *desc=NULL,*locationid=NULL;
	int sellocation=0;

	readallwidgets(mtnrsrc);
	FINDRSCGETINT(mtnrsrc,"LOCATION IDENTIFICATIONS",&sellocation);
	if(RDAstrcmp(locations->libs[sellocation],"No Locations"))
	{
		locationid=stralloc(locations->libs[sellocation]);
	}
	rsrc2filerecord(reqmst,mtnrsrc);
	FINDFLDSETSTRING(reqmst,"LOCATION IDENTIFICATION",locationid);
	if(locationid!=NULL) { Rfree(locationid); locationid=NULL; }
	if(QUITvfy(reqmst,previous))
	{
		desc=QUITvfydesc(reqmst,previous);
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!",
			desc,save_check_save_quit,quit_record,FALSE,2,mtnrsrc,
			previous,NULL);
		if(desc!=NULL) Rfree(desc);
	} else {
		quit_record(mtnrsrc,previous);
	}
}
static void set_defaults(RDArsrc *parent)
{
	int selected=0;
	char *date=NULL,*timex=NULL;

	DefaultResetScreen(parent,TRUE);
	getsupportingrecords(parent,FALSE);
	FINDRSCGETINT(parent,"LOCATION IDENTIFICATIONS",&selected);
	if(selected>=locations->numlibs) selected=0;
	FINDRSCSETINT(parent,"LOCATION IDENTIFICATIONS",selected);
	date=GETCURRENTDATE10();
	timex=GETCURRENTTIME();
	FINDRSCSETSTRING(parent,"[REQMST][ENTRY DATE]",date);
	FINDRSCSETSTRING(parent,"[REQMST][ENTRY TIME]",timex);
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
	computeallSCRNvirtuals(parent);
	updateallrsrc(parent);
}
void reqmstm(short dowhich,void *targetkey)
{
	char *reqid1=NULL,*locid1=NULL,*date=NULL,*timex=NULL;
	int which_type=0;
	short nofields=0,x=0,edit_rsrc=TRUE;
	NRDfield *fields=NULL,*fldx;
	RDArsrc *mtnrsrc=NULL;
	RDATData *previous=NULL;
	
	mtnrsrc=RDArsrcNEW(module,"ADD REQUISITION MAINTAIN SCREEN");
	addDFincvir(mtnrsrc,module,"REQLOC",NULL,reqloc);
	addDFincvir(mtnrsrc,module,"REQMST",NULL,reqmst);
	GET_SCREEN_VIRTUALS(mtnrsrc,0);
	ZERNRD(reqmst);
	previous=RDATDataNEW(reqmst);
	file2rsrc(reqloc,mtnrsrc,FALSE);
	nofields=NUMFLDS(reqmst);
	fields=FLDPOINTER(reqmst);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			edit_rsrc=TRUE;
			if(!RDAstrcmp(fldx->name,"LOCATION IDENTIFICATION"))
			{
				if(FIELDscrn(reqmst,fldx->name,&edit_rsrc))
				{
					which_type=0;
					if(locations!=NULL) freeapplib(locations);
					ZERNRD(reqloc);
					locations=ADVMakeNRDlistAllSTRING(reqloc,module,
						"Creating Location List",1,"LOCATION IDENTIFICATION",
						SCRNvirtualSubData,mtnrsrc);
					if(locations->numlibs<1)
					{
						addAPPlib(locations,"No Locations");
					}
					addlstrsrc(mtnrsrc,"LOCATION IDENTIFICATIONS",
						&which_type,(edit_rsrc==TRUE?TRUE:FALSE),getrecord,
						locations->numlibs,&locations->libs,previous);
				}
			} else if(!strncmp(fldx->name,"SUBMITTED",9))
			{
			} else if(!RDAstrcmp(fldx->name,"APPROVAL STATUS"))
			{
			} else if(!strncmp(fldx->name,"APPROVED",8))
			{
			} else nonlstmakefld(mtnrsrc,reqmst,fldx,edit_rsrc);
		}
	}
	nofields=0;
	FINDRSCSETFUNC(mtnrsrc,"[REQMST][REQUISITION IDENTIFICATION]",getrecord,previous);
	DefaultScreens(mtnrsrc);
	date=GETCURRENTDATE10();
	timex=GETCURRENTTIME();
	switch(dowhich)
	{
		case 1:
			FINDRSCGETSTRING(mtnrsrc,"[REQMST][LOCATION IDENTIFICATION]",&locid1);
			FINDRSCGETSTRING(mtnrsrc,"[REQMST][REQUISITION IDENTIFICATION]",&reqid1);
			if(ADVEQLNRDsec(reqmst,1,SCRNvirtualSubData,mtnrsrc))
			{
				KEYNRD(reqmst,1);
				FINDFLDSETSTRING(reqmst,"LOCATION IDENTIFICATION",locid1);
				FINDFLDSETSTRING(reqmst,"REQUISITION IDENTIFICATION",reqid1);
				FINDRSCSETSTRING(mtnrsrc,"[REQMST][ENTRY DATE]",date);
				FINDRSCSETSTRING(mtnrsrc,"[REQMST][ENTRY TIME]",timex);
				FINDRSCSETSTRING(mtnrsrc,"[REQMST][SOURCE USER]",USERLOGIN);
			} else {
				filerecord2rsrc(reqmst,mtnrsrc);
				setlocationids(mtnrsrc);
				GetRDATData(reqmst,previous);
			}
			break;
		case 2:
			FINDRSCSETSTRING(mtnrsrc,"[REQMST][ENTRY DATE]",date);
			FINDRSCSETSTRING(mtnrsrc,"[REQMST][ENTRY TIME]",timex);
			FINDRSCSETSTRING(mtnrsrc,"[REQMST][SOURCE USER]",USERLOGIN);
			break;
		default:
		case 0: /* not used */
			if(ADVEQLNRDKEYsec(reqmst,1,targetkey,SCRNvirtualSubData,mtnrsrc))
			{
				ZERNRD(reqmst);
				FINDRSCSETSTRING(mtnrsrc,"[REQMST][ENTRY DATE]",date);
				FINDRSCSETSTRING(mtnrsrc,"[REQMST][ENTRY TIME]",timex);
				FINDRSCSETSTRING(mtnrsrc,"[REQMST][SOURCE USER]",USERLOGIN);
			} else {
				filerecord2rsrc(reqmst,mtnrsrc);
				setlocationids(mtnrsrc);
				GetRDATData(reqmst,previous);
			}
			break;
	}
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
	if(locid1!=NULL) Rfree(locid1);
	if(reqid1!=NULL) Rfree(reqid1);
	getsupportingrecords(mtnrsrc,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	SCRNvirtual2rsrc(mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_check_savecb,previous);
	addbtnrsrc(mtnrsrc,"ADD REQUISITION ITEMS MULTIPLE",TRUE,
		addreqitmmultiple,previous);
	addbtnrsrc(mtnrsrc,"ADD REQUISITION ITEMS SINGLE",TRUE,
		addreqitmsingle,previous);
	addbtnrsrc(mtnrsrc,"BROWSE REQUISITIONS",TRUE,browse_msr,previous);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,set_defaults,NULL);
	addbtnrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,previous);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	computeallSCRNvirtuals(mtnrsrc);
	APPmakescrn(mtnrsrc,FALSE,quit_record,previous,TRUE);
}
static void seteditable(RDArsrc *mtnrsrc,char update)
{
	char closed=FALSE;

	FINDRSCGETCHAR(mtnrsrc,"[REQMST][CLOSED]",&closed);
	if(closed)
	{
		if(update)
		{
			updateeditfilersrcsnokeys(reqmst,1,mtnrsrc,FALSE);
		} else seteditfilersrc(reqmst,1,mtnrsrc,FALSE,FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"[REQMST][DELETEFLAG]",TRUE);
	} else {
		if(update)
		{
			updateeditfilersrcsnokeys(reqmst,1,mtnrsrc,TRUE);
		} else seteditfilersrc(reqmst,1,mtnrsrc,TRUE,FALSE);
	}
}
void getsupportingrecords(RDArsrc *mtnrsrc,char update)
{
	int selected=0;

	seteditable(mtnrsrc,update);
	FINDRSCGETINT(mtnrsrc,"LOCATION IDENTIFICATIONS",&selected);
	FINDFLDSETSTRING(reqloc,"LOCATION IDENTIFICATION",
		locations->libs[selected]);
	if(ADVEQLNRDsec(reqloc,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(reqloc,1);
	if(update) updatefilerecord2rsrc(reqloc,mtnrsrc);
	else filerecord2rsrc(reqloc,mtnrsrc);
}
static void save_check_save_quit(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(save_check_save(mtnrsrc,FALSE)) quit_record(mtnrsrc,previous);
}
static void save_check_savecb(RDArsrc *mtnrsrc,RDATData *previous)
{
	save_check_save(mtnrsrc,previous);
}
static short save_check_save(RDArsrc *mtnrsrc,RDATData *previous)
{
	if(save_check(mtnrsrc,FALSE))
	{
		save_record(mtnrsrc,previous);
		return(TRUE);
	} else {
		return(FALSE);
	}
}
static short save_check(RDArsrc *mtnrsrc,short setaddress)
{
	char *tmp=NULL;
	char flag=FALSE;
	int selected=0;

	ZERNRD(reqloc);
	FINDRSCGETINT(mtnrsrc,"LOCATION IDENTIFICATIONS",&selected);
	FINDFLDSETSTRING(reqloc,"LOCATION IDENTIFICATION",
		locations->libs[selected]);
	FINDFLDSETSTRING(reqmst,"LOCATION IDENTIFICATION",
		locations->libs[selected]);
	if(!ADVEQLNRDsec(reqloc,1,SCRNvirtualSubData,mtnrsrc))
	{
		FINDFLDGETCHAR(reqloc,"DELETEFLAG",&flag);
		if(flag==TRUE)
		{
			if(tmp!=NULL) Rfree(tmp);
			tmp=Rmalloc(RDAstrlen(locations->libs[selected])+83);
			sprintf(tmp,"The LOCATION IDENTIFICATION field of [%s] exist but is currently in a deleted state.",locations->libs[selected]);
			WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED LOCATION IDENTIFICATION!",tmp,NULL,NULL,TRUE,NULL);
			if(tmp!=NULL) Rfree(tmp);
			return(FALSE);
		}
		FINDFLDGETCHAR(reqloc,"ACTIVE",&flag);
		if(flag==FALSE)
		{
			if(tmp!=NULL) Rfree(tmp);
			tmp=Rmalloc(RDAstrlen(locations->libs[selected])+86);
			sprintf(tmp,"The LOCATION IDENTIFICATION field of [%s] exist but is currently in a non-active state.",locations->libs[selected]);
			WARNINGDIALOG("WARNING DIALOG SCREEN","INACTIVE LOCATION IDENTIFICATION!",tmp,NULL,NULL,TRUE,NULL);
			if(tmp!=NULL) Rfree(tmp);
			return(FALSE);
		}
	} else {
		if(tmp!=NULL) Rfree(tmp);
		tmp=Rmalloc(RDAstrlen(locations->libs[selected])+57);
		sprintf(tmp,"The LOCATION IDENTIFICATION field of [%s] does not exist.",locations->libs[selected]);
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID LOCATION IDENTIFICATION!",tmp,NULL,NULL,TRUE,NULL);
		if(tmp!=NULL) Rfree(tmp);
		return(FALSE);
	}
	return(TRUE);
}
static void save_record(RDArsrc *mtnrsrc,RDATData *previous)
{
	char *rcddesc=NULL,*locationid=NULL;
	int sellocation=0;

	readallwidgets(mtnrsrc);
	FINDRSCGETINT(mtnrsrc,"LOCATION IDENTIFICATIONS",&sellocation);
	if(RDAstrcmp(locations->libs[sellocation],"No Locations"))
	{
		locationid=stralloc(locations->libs[sellocation]);
	}
	rsrc2filerecord(reqmst,mtnrsrc);
	FINDFLDSETSTRING(reqmst,"LOCATION IDENTIFICATION",locationid);
	if(locationid!=NULL) { Rfree(locationid); locationid=NULL; }
	if(ADVRECORDsec(reqmst,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(reqmst,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE); 
			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(reqmst,0,NULL,previous);
	}
	GetRDATData(reqmst,previous);
	getsupportingrecords(mtnrsrc,TRUE);
	updateSCRNvirtuals(mtnrsrc);
}
static void quit_record(RDArsrc *mtnrsrc,RDATData *previous)
{
	APPlib *args=NULL;

	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc); 
	if(REQ_COUNT)
	{
		args=APPlibNEW();
		addAPPlib(args,module);
		addAPPlib(args,reportname);
		ExecuteProgram("doreport",args);
		if(args!=NULL) freeapplib(args);
	}
	if(acctavl!=NULL) freeapplib(acctavl);
	if(creditavl!=NULL) freeapplib(creditavl);
	if(debitavl!=NULL) freeapplib(debitavl);
	if(defdebitavl!=NULL) freeapplib(defdebitavl);
	if(defcreditavl!=NULL) freeapplib(defcreditavl);
	if(defpayableavl!=NULL) freeapplib(defpayableavl);
	if(BANKids!=NULL) freeapplib(BANKids);
	if(DEFtypes!=NULL) freeapplib(DEFtypes);
	if(mbl_venmsr!=NULL) FREE_BROWSE_EXIT(mbl_venmsr);
	if(previous!=NULL) FreeRDATData(previous);
	if(reqmst!=(-1)) CLSNRD(reqmst);
	if(venadrnum!=(-1)) CLSNRD(venadrnum);
	if(reqitm!=(-1)) CLSNRD(reqitm);
	if(reqacc!=(-1)) CLSNRD(reqacc);
	if(reqloc!=(-1)) CLSNRD(reqloc);
	if(reqven!=(-1)) CLSNRD(reqven);
	if(reqpven!=(-1)) CLSNRD(reqpven);
	if(vennum!=(-1)) CLSNRD(vennum);
	if(reqcat!=(-1)) CLSNRD(reqcat);
	if(reqctlg!=(-1)) CLSNRD(reqctlg);
	if(expnum!=(-1)) CLSNRD(expnum);
	if(balnum!=(-1)) CLSNRD(balnum);
	if(eyrnum!=(-1)) CLSNRD(eyrnum);
	if(byrnum!=(-1)) CLSNRD(byrnum);
	if(finyrnum!=(-1)) CLSNRD(finyrnum);
	if(finsetup!=NULL) free_finmgt(finsetup);
	ShutdownSubsystems();
	exit(0);
}
static short check_reqid(RDArsrc *mtnrsrc,char *reqid,char *locid,
	RDATData *previous)
{
	char flag=FALSE,*warnmessage=NULL;

	ZERNRD(reqmst);
	FINDFLDSETSTRING(reqmst,"LOCATION IDENTIFICATION",locid);
	FINDFLDSETSTRING(reqmst,"REQUISITION IDENTIFICATION",reqid);
	if(ADVEQLNRDsec(reqmst,1,SCRNvirtualSubData,mtnrsrc))
	{
		warnmessage=Rmalloc(RDAstrlen(reqid)+RDAstrlen(locid)+301);
		sprintf(warnmessage,"Requisition Items cannot be added until the Requisition Master has been saved.  The LOCATION IDENTIFICATION of [%s] and the REQUISITION IDENTIFICATION of [%s] entered does not presently exist in the [REQMGT][REQMST] file. Do you wish to create it?",(locid!=NULL ? locid:""),(reqid==NULL?"":reqid));
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","REQUISTION MASTER DOES NOT EXIST!",warnmessage,save_record,NULL,FALSE,2,mtnrsrc,previous,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		return(FALSE);
	} else {
		FINDFLDGETCHAR(reqmst,"DELETEFLAG",&flag);
		if(flag)
		{
			warnmessage=Rmalloc(RDAstrlen(reqid)+RDAstrlen(locid)+301);
			sprintf(warnmessage,"Requisition Items can not be added to a deleted Requisition Master.  The master record for the LOCATION IDENTIFICATION of [%s] and the REQUISITION IDENTIFICATION of [%s] entered is in a state of deletion in the [REQMGT][REQMST] file.",(locid!=NULL ? locid:""),(reqid==NULL?"":reqid));
			WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED REQUISITION MASTER!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			return(FALSE);
		} else {
			FINDFLDGETCHAR(reqmst,"SUBMITTED",&flag);
			if(flag)
			{
				warnmessage=Rmalloc(RDAstrlen(reqid)+RDAstrlen(locid)+301);
				sprintf(warnmessage,"Requisition Items can not be added to a submitted Requisition Master.  The master record for the LOCATION IDENTIFICATION of [%s] and the REQUISITION IDENTIFICATION of [%s] entered has been submitted in the [REQMGT][REQMST] file.",(locid!=NULL ? locid:""),(reqid==NULL?"":reqid));
				WARNINGDIALOG("WARNING DIALOG SCREEN","SUBMITTED REQUISITION MASTER!",warnmessage,NULL,NULL,TRUE,NULL);
				if(warnmessage!=NULL) Rfree(warnmessage);
				return(FALSE);
			}
		}
	}
	return(TRUE);
}
static void addreqitmmultiple(RDArsrc *mtnrsrc,RDATData *previous)
{
	int x=0;
	char *locid=NULL;

	FINDRSCGETINT(mtnrsrc,"LOCATION IDENTIFICATIONS",&x);
	if(locid!=NULL) Rfree(locid);
	locid=stralloc(locations->libs[x]);
	readwidget(mtnrsrc,"[REQMST][REQUISITION IDENTIFICATION]");
	if(reqid!=NULL) Rfree(reqid);
	FINDRSCGETSTRING(mtnrsrc,"[REQMST][REQUISITION IDENTIFICATION]",&reqid);
	if(check_reqid(mtnrsrc,reqid,locid,previous)==FALSE) return;
	additemsmultiple();
}
static void addreqitmsingle(RDArsrc *mtnrsrc,RDATData *previous)
{
	int x=0;
	char *locid=NULL;

	FINDRSCGETINT(mtnrsrc,"LOCATION IDENTIFICATIONS",&x);
	if(locid!=NULL) Rfree(locid);
	locid=stralloc(locations->libs[x]);
	readwidget(mtnrsrc,"[REQMST][REQUISITION IDENTIFICATION]");
	if(reqid!=NULL) Rfree(reqid);
	FINDRSCGETSTRING(mtnrsrc,"[REQMST][REQUISITION IDENTIFICATION]",&reqid);
	if(check_reqid(mtnrsrc,reqid,locid,previous)==FALSE) return;
	additemssingle();
}
static void getrecord(RDArsrc *mtnrsrc,RDATData *previous)
{
	int x;
	char *date=NULL,*timex=NULL;

	readallwidgets(mtnrsrc);
	FINDRSCGETINT(mtnrsrc,"LOCATION IDENTIFICATIONS",&x);
	if(locid!=NULL) Rfree(locid);
	locid=stralloc(locations->libs[x]);
	FINDRSCGETSTRING(mtnrsrc,"[REQMST][REQUISITION IDENTIFICATION]",&reqid);
	ZERNRD(reqmst);
	FINDFLDSETSTRING(reqmst,"LOCATION IDENTIFICATION",locid);
	FINDFLDSETSTRING(reqmst,"REQUISITION IDENTIFICATION",reqid);
	seteditfilersrc(reqmst,1,mtnrsrc,TRUE,FALSE);
	if(ADVEQLNRDsec(reqmst,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(reqmst,1);
		ClearRDATData(previous);
		date=GETCURRENTDATE10();
		timex=GETCURRENTTIME();
/*
		FINDRSCSETSTRING(mtnrsrc,"[REQMST][ENTRY DATE]",date);
		FINDRSCSETSTRING(mtnrsrc,"[REQMST][ENTRY TIME]",timex);
		FINDRSCSETSTRING(mtnrsrc,"[REQMST][SOURCE USER]",USERLOGIN);
*/
		FINDFLDSETSTRING(reqmst,"[REQMST][ENTRY DATE]",date);
		FINDFLDSETSTRING(reqmst,"[REQMST][ENTRY TIME]",timex);
		FINDFLDSETSTRING(reqmst,"[REQMST][SOURCE USER]",USERLOGIN);
		if(date!=NULL) Rfree(date);
		if(timex!=NULL) Rfree(timex);
	} else {
		GetRDATData(reqmst,previous);
	}
	filerecord2rsrc(reqmst,mtnrsrc);
	setlocationids(mtnrsrc);
	getsupportingrecords(mtnrsrc,FALSE);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
void setlocationids(RDArsrc *mtnrsrc)
{
	int x=0;
	char *locationid=NULL;
 	
	FINDFLDGETSTRING(reqmst,"LOCATION IDENTIFICATION",&locationid);
	if((x=FINDAPPLIBELEMENT(locations,locationid))==(-1)) x=0;
	FINDRSCSETINT(mtnrsrc,"LOCATION IDENTIFICATIONS",x);
	if(locationid!=NULL) Rfree(locationid);
}
