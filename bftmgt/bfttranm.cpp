/* bfttranm.c - Xpert Benefit Tranaction Master Maintain Screen */
#include <cstdio>



#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>

struct mtnstructs
{
	APPlib *DEFtypes;
	RDATData *previous;
};
typedef struct mtnstructs mtnstruct;

extern short msrnum,descnum,mdacnum,ddacnum;
extern char editable;
extern MakeBrowseList *mbl_bfttran;
extern MaintainMaster *MTNMASTER;
extern void browse_bfttran(RDArsrc *,mtnstruct *);
static void save_record(RDArsrc *,mtnstruct *,short);
static void save_record_now(RDArsrc *,mtnstruct *);
static void save_record_quit(RDArsrc *,mtnstruct *);
static void save_check(RDArsrc *,mtnstruct *,short);
static void save_record_up(RDArsrc *,mtnstruct *);
static void save_record_nup(RDArsrc *,mtnstruct *);
static void quit_record(RDArsrc *,mtnstruct *);
static void quit_record_test(RDArsrc *,mtnstruct *);
static void DEFtypecb(RDArsrc *,mtnstruct *);
static void readscreen(RDArsrc *,mtnstruct *);
static void list2file(RDArsrc *,mtnstruct *);
static void setlist(RDArsrc *,mtnstruct *,short);
static void set_appdefaults(RDArsrc *,mtnstruct *,short);
static void seteditable(RDArsrc *,mtnstruct *,short);
static void getrecordsetedit(RDArsrc *,mtnstruct *,short);
static void setfiles(RDArsrc *,mtnstruct *);
static void getrecordcb(RDArsrc *,mtnstruct *);
static void resetdefs(RDArsrc *,mtnstruct *);
static void getpreviouscb(RDArmem *,mtnstruct *);
static void getnextcb(RDArmem *,mtnstruct *);
static void updatesupportingrecord(RDArsrc *,short,char *,double);
void bfttranm(short,void *);

static void updatesupportingrecord(RDArsrc *mtnrsrc,short fileno,char *fieldname,double amount)
{
	char *errordesc=NULL,*modulename=NULL,*filename=NULL;
	double initamt=0.0;
	RDATData *prev_file=NULL;

	if(ADVEQLNRDsec(fileno,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(fileno,1);
		FINDFLDSETDOUBLE(fileno,fieldname,amount);
		if(ADVWRTTRANSsec(fileno,0,NULL,NULL,SCRNvirtualSubData,mtnrsrc))
		{
			modulename=MODULENAME(fileno);
			filename=FILENAME(fileno);
			if(errordesc!=NULL) Rfree(errordesc);
			errordesc=Rmalloc(RDAstrlen(modulename)+RDAstrlen(filename)+100);
			sprintf(errordesc,"Write failed to the file [%s][%s], this supporting record that you attempted to update was not saved.",(modulename==NULL?"":modulename),(filename==NULL?"":filename));
			ERRORDIALOG("WRITE ERROR OCCURRED!",errordesc,NULL,FALSE);
			prterr("Error Write Failed to Supporting Record: [%s]",errordesc);
			if(errordesc!=NULL) Rfree(errordesc);
		}
	} else {
		LOCNRD(fileno);
		prev_file=RDATDataNEW(fileno);
		FINDFLDGETDOUBLE(fileno,fieldname,&initamt);
		initamt+=amount;
		FINDFLDSETDOUBLE(fileno,fieldname,initamt);
		if(ADVWRTTRANSsec(fileno,0,NULL,prev_file,SCRNvirtualSubData,mtnrsrc))
		{
			modulename=MODULENAME(fileno);
			filename=FILENAME(fileno);
			if(errordesc!=NULL) Rfree(errordesc);
			errordesc=Rmalloc(RDAstrlen(modulename)+RDAstrlen(filename)+100);
			sprintf(errordesc,"Write failed to the file [%s][%s], this supporting record that you attempted to update was not saved.",(modulename==NULL?"":modulename),(filename==NULL?"":filename));
			ERRORDIALOG("WRITE ERROR OCCURRED!",errordesc,NULL,FALSE);
			prterr("Error Write Failed to Supporting Record: [%s]",errordesc);
			if(errordesc!=NULL) Rfree(errordesc);
		}
		UNLNRD(fileno);
		if(prev_file!=NULL) FreeRDATData(prev_file);
	}
}
static void DEFtypecb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	getrecordcb(mtnrsrc,mtn);
}
void bfttranm(short dowhich,void *targetkey)
{
	char *name=NULL;
	int which_type=0;
	short nofields=0,nokeys=0,x=0,y=0,edit_rsrc=FALSE;
	NRDfield *fields=NULL,*fldx=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part;
	RDAacct *acct=NULL;
	mtnstruct *mtn=NULL;
	RDArsrc *mtnrsrc=NULL;
	
	mtnrsrc=RDArsrcNEW(MTNMASTER->module,MTNMASTER->maintain_screen);
	ZERNRD(MTNMASTER->passkey->fileno);
	mtn=Rmalloc(sizeof(mtnstruct));
	mtn->DEFtypes=NULL;
	mtn->previous=NULL;
	mtn->previous=RDATDataNEW(MTNMASTER->passkey->fileno);
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
			if(!RDAstrcmp(fldx->name,"DEFINITION TYPE"))
			{
				if(FIELDscrn(MTNMASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					mtn->DEFtypes=APPlibNEW();
					if(BALANCE_SHEET_ACCOUNT->codetype!=NULL)
					{
						for(y=0,acct=BALANCE_SHEET_ACCOUNT->codetype;
							y<BALANCE_SHEET_ACCOUNT->num;++y,++acct)
						{
							addAPPlib(mtn->DEFtypes,acct->name);
						}
					}
					if(mtn->DEFtypes->numlibs<1)
					{
						addAPPlib(mtn->DEFtypes,"No Definition Types");
					}
					which_type=0;
					addlstrsrc(mtnrsrc,"DEFINITION TYPES",
						&which_type,
						(edit_rsrc==TRUE?TRUE:FALSE),
						DEFtypecb,mtn->DEFtypes->numlibs,
						&mtn->DEFtypes->libs,mtn);
				}
			} else if(!RDAstrcmp(fldx->name,"PROCESSED BALANCE"))
			{
				if(FIELDscrn(MTNMASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					makefieldrsrc(mtnrsrc,
						"[BFTTRAN][PROCESSED BALANCE]",
					fldx->type,fldx->len,FALSE);
				}
			} else if(!RDAstrcmp(fldx->name,"UNPROCESSED BALANCE"))
			{
				if(FIELDscrn(MTNMASTER->passkey->fileno,fldx->name,&edit_rsrc))
				{
					makefieldrsrc(mtnrsrc,
						"[BFTTRAN][UNPROCESSED BALANCE]",
						fldx->type,fldx->len,FALSE);
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
				FINDRSCSETFUNC(mtnrsrc,name,setfiles,mtn);
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
					FINDRSCSETFUNC(mtnrsrc,name,getrecordcb,mtn);
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
				list2file(mtnrsrc,mtn);
				if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
					set_appdefaults(mtnrsrc,mtn,FALSE);
				} else {
					GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
					getrecordsetedit(mtnrsrc,mtn,FALSE);
				}
			} else {
				GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
				getrecordsetedit(mtnrsrc,mtn,FALSE);
			}
			break;
		case 2:
			ApplyPassKey(MTNMASTER->passkey);
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
			list2file(mtnrsrc,mtn);
			if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
				set_appdefaults(mtnrsrc,mtn,FALSE);
			} else {
				GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
				getrecordsetedit(mtnrsrc,mtn,FALSE);
			}
			break;
		case 0:
		default:
			if(ADVEQLNRDKEYsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,targetkey,SCRNvirtualSubData,mtnrsrc))
			{
				ZERNRD(MTNMASTER->passkey->fileno);
				ApplyPassKey(MTNMASTER->passkey);
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
				list2file(mtnrsrc,mtn);
				if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
				{
					ApplyPassKey2Rsrc(mtnrsrc,MTNMASTER->passkey);
					set_appdefaults(mtnrsrc,mtn,FALSE);
				} else {
					GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
					getrecordsetedit(mtnrsrc,mtn,FALSE);
				}
			} else {
				GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
				getrecordsetedit(mtnrsrc,mtn,FALSE);
			}	
			break;
	}
	GET_SUPPORTING(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc,FALSE,FALSE);
	SetVirFieldFuncs(mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE NO UPDATE",TRUE,save_record_nup,mtn);
	if(mbl_bfttran!=NULL)
	{
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_up,mtn);
	} else {
		addbtnrsrc(mtnrsrc,"SAVE",TRUE,save_record_nup,mtn);
	}
	addrfkwrsrc(mtnrsrc,"BROWSE",TRUE,browse_bfttran,mtn);
	addbtnrsrc(mtnrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mtnrsrc,"RESET DEFAULTS",TRUE,resetdefs,mtn);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quit_record_test,mtn);
	addrfcbrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	MakePrevNextButtons(mtnrsrc,keys,nokeys,mtn);
	MakeMasterButtons(mtnrsrc,MTNMASTER);
	computeallSCRNvirtuals(mtnrsrc);
	if(!editable)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE NO UPDATE",FALSE);
	}
	if(mbl_bfttran!=NULL) FINDRSCSETSENSITIVE(mtnrsrc,"BROWSE",FALSE);
	APPmakescrn(mtnrsrc,FALSE,quit_record,mtn,(mbl_bfttran==NULL?TRUE:FALSE));
}
static void getrecordsetedit(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	filerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
	setlist(mtnrsrc,mtn,update);
	seteditable(mtnrsrc,mtn,update);
}
static void seteditable(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char suv=FALSE;

	FINDRSCGETCHAR(mtnrsrc,"[BFTTRAN][SOURCE USER VERIFIED]",&suv);
	if(suv)
	{
		if(update)
		{
			updateeditfilersrcsnokeys(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,mtnrsrc,FALSE);
		} else {
			seteditfilersrc(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,mtnrsrc,FALSE,FALSE);
		}
		SetRDAScrolledListEditable(mtnrsrc,MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,FALSE,FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"[BFTTRAN][DELETEFLAG]",TRUE);
	} else {
		if(update)
		{
			updateeditfilersrcsnokeys(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,mtnrsrc,TRUE);
		} else {
			seteditfilersrc(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,mtnrsrc,TRUE,TRUE);
		}
		SetRDAScrolledListEditable(mtnrsrc,MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,TRUE,TRUE);
	}
}
static void resetdefs(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MTNMASTER->passkey->fileno);
	DefaultResetScreen(mtnrsrc,TRUE);
	updateallrsrc(mtnrsrc);
	getrecordcb(mtnrsrc,mtn);
}
static void save_record_now(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_record(mtnrsrc,mtn,TRUE);
}
static void save_record_now1(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_record(mtnrsrc,mtn,FALSE);
}
static void setlist(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	int x;
	short d;

	FINDFLDGETSHORT(MTNMASTER->passkey->fileno,"DEFINITION TYPE",&d);
	x=d;
	if(mtn->DEFtypes->numlibs<=x)
	{
		x=0;
		d=0;
	}
	FINDRSCSETINT(mtnrsrc,"DEFINITION TYPES",x);
	if(update) updatersrc(mtnrsrc,"DEFINITION TYPES");
}
static void readscreen(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	ZERNRD(MTNMASTER->passkey->fileno);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(MTNMASTER->passkey->fileno,mtnrsrc);
	list2file(mtnrsrc,mtn);
}
static void list2file(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	int selected=0;
	short type=0;

	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&selected);
       	if(RDAstrcmp(mtn->DEFtypes->libs[selected],"No Definition Types"))
	{
		type=(short)selected;
	}
	FINDFLDSETSHORT(MTNMASTER->passkey->fileno,"DEFINITION TYPE",type);
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
	int seldeftype=0;
	char delflag=FALSE;
	char *desc=NULL;

	FINDRSCGETCHAR(mtnrsrc,"[BFTTRAN][DELETEFLAG]",&delflag);
	if(!delflag)
	{
        	FINDRSCGETINT(mtnrsrc,"DEFINITION TYPES",&seldeftype);
        	if(!RDAstrcmp(mtn->DEFtypes->libs[seldeftype],"No Definition Types"))
        	{
			desc=Rmalloc(250);
			sprintf(desc,"There is not a valid DEFINITION TYPE available for selection.  Before this Transaction can be saved there must be a valid DEFINITION TYPE available for selection.  Check to see if the  BALANCE SHEET ACCOUNT CODE DEFINITION has at least one DEFINITION TYPE available.");
                	WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DEFINITION TYPE!",desc,NULL,NULL,FALSE,NULL);
			prterr(desc);
			if(desc!=NULL) Rfree(desc);
                	return;
		}
	}
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
				desc,(update_list ? save_record_now:save_record_now1),
				NULL,FALSE,2,mtnrsrc,mtn->previous,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	save_record(mtnrsrc,mtn,update_list);
}
static void save_record(RDArsrc *mtnrsrc,mtnstruct *mtn,short update_list)
{
	short numflds=0,y=0,deftype1=0;
	char *rcddesc=NULL,*errordesc=NULL;
	char delflag=FALSE;
	char *perid1=NULL,*benefitid1=NULL,*modulename=NULL,*filename=NULL;
	double amount=0.0,initamt=0.0;
	NRDfield *tmpfld=NULL,*fldx=NULL,*tmp=NULL;
	RDATData *prev_file=NULL;

	FINDFLDGETSTRING(MTNMASTER->passkey->fileno,"PERSONNEL IDENTIFICATION",&perid1);
	FINDFLDGETSTRING(MTNMASTER->passkey->fileno,"DEDUCTION DESCRIPTION IDENTIFICATION",&benefitid1);
	FINDFLDGETSHORT(MTNMASTER->passkey->fileno,"DEFINITION TYPE",&deftype1);
	if(ADVRECORDsec(MTNMASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(MTNMASTER->passkey->fileno,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
			prterr("Security Access Deined [%s].  Didn't write record.",rcddesc);
			Rfree(rcddesc);
		} else {
			prterr("Didn't write record.");
		}
	} else {
		numflds=NUMFLDS(MTNMASTER->passkey->fileno);
		if(numflds>0)
		{
			tmpfld=Rmalloc(numflds*sizeof(NRDfield));
			for(y=0,fldx=tmpfld;y<numflds;++y,++fldx) 
			{
				tmp=FLDNUM(MTNMASTER->passkey->fileno,y);
				cpfldinfo(fldx,tmp);
			}
			if(!ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
			{
				FINDFLDGETCHAR(MTNMASTER->passkey->fileno,"DELETEFLAG",&delflag);
				if(!delflag)
				{
					FINDFLDGETDOUBLE(MTNMASTER->passkey->fileno,"AMOUNT",&amount);
					FINDFLDSETSTRING(descnum,"DEDUCTION DESCRIPTION IDENTIFICATION",benefitid1);
					if(ADVEQLNRDsec(descnum,1,SCRNvirtualSubData,mtnrsrc))
					{
						modulename=MODULENAME(descnum);
						filename=FILENAME(descnum);
						errordesc=Rmalloc(RDAstrlen(modulename)+RDAstrlen(filename)+200);
						sprintf(errordesc,"RECORD NOT FOUND or Security Failed, this record you attempted to write was not saved because they supporting record does not exist in the file [%s][%s].",(modulename==NULL?"":modulename),(filename==NULL?"":filename));
						ERRORDIALOG("SUPPORTING RECORD NOT FOUND!",errordesc,NULL,FALSE);
						prterr("Error Security Doesn't Allow Save or Supporting Record not found [%s]",errordesc);
						if(errordesc!=NULL) Rfree(errordesc);
					} else {
						LOCNRD(descnum);
						prev_file=RDATDataNEW(descnum);
						FINDFLDGETDOUBLE(descnum,"UNPROCESSED BALANCE",&initamt);
						initamt-=amount;
						FINDFLDSETDOUBLE(descnum,"UNPROCESSED BALANCE",initamt);
						if(ADVWRTTRANSsec(descnum,0,NULL,prev_file,SCRNvirtualSubData,mtnrsrc))
						{
							modulename=MODULENAME(descnum);
							filename=FILENAME(descnum);
							if(errordesc!=NULL) Rfree(errordesc);
							errordesc=Rmalloc(RDAstrlen(modulename)+RDAstrlen(filename)+200);
							sprintf(errordesc,"Write failed to the file [%s][%s], this supporting record that you attempted to update was not saved therefore no records were updated.",(modulename==NULL?"":modulename),(filename==NULL?"":filename));
							ERRORDIALOG("WRITE ERROR OCCURRED!",errordesc,NULL,FALSE);
							prterr("Error Write Failed to Supporting Record: [%s]",errordesc);
							if(errordesc!=NULL) Rfree(errordesc);
							UNLNRD(descnum);
							if(prev_file!=NULL) FreeRDATData(prev_file);
						} else {
							UNLNRD(descnum);
							if(prev_file!=NULL) FreeRDATData(prev_file);
							FINDFLDSETSTRING(mdacnum,"PERSONNEL IDENTIFICATION",perid1);
							FINDFLDSETSTRING(mdacnum,"DEDUCTION DESCRIPTION IDENTIFICATION",benefitid1);
							FINDFLDSETSHORT(mdacnum,"DEFINITION TYPE",deftype1);
							updatesupportingrecord(mtnrsrc,mdacnum,"UNPROCESSED BALANCE",-amount);
							FINDFLDSETSTRING(ddacnum,"DEDUCTION DESCRIPTION IDENTIFICATION",benefitid1);
							FINDFLDSETSHORT(ddacnum,"DEFINITION TYPE",deftype1);
							updatesupportingrecord(mtnrsrc,ddacnum,"UNPROCESSED BALANCE",-amount);
							ZERNRD(msrnum);
							FINDFLDSETSTRING(msrnum,"PERSONNEL IDENTIFICATION",perid1);
							FINDFLDSETSTRING(msrnum,"DEDUCTION DESCRIPTION IDENTIFICATION",benefitid1);
							updatesupportingrecord(mtnrsrc,msrnum,"UNPROCESSED BALANCE",-amount);
						}
					}
				} /* end if(!delflag) */
			} /* end if(!ADVEQLNRDsec) */
			fields2file(MTNMASTER->passkey->fileno,tmpfld);
			WRTTRANS(MTNMASTER->passkey->fileno,0,NULL,mtn->previous);
			DELETE_SUBORDINATES(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc);
			FINDFLDGETCHAR(MTNMASTER->passkey->fileno,"DELETEFLAG",&delflag);
			if(!delflag)
			{
				FINDFLDGETDOUBLE(MTNMASTER->passkey->fileno,"AMOUNT",&amount);
				FINDFLDSETSTRING(descnum,"DEDUCTION DESCRIPTION IDENTIFICATION",benefitid1);
				if(ADVEQLNRDsec(descnum,1,SCRNvirtualSubData,mtnrsrc))
				{
					modulename=MODULENAME(descnum);
					filename=FILENAME(descnum);
					errordesc=Rmalloc(RDAstrlen(modulename)+RDAstrlen(filename)+120);
					sprintf(errordesc,"Security failed on the file [%s][%s] or RECORD NOT FOUND, this supporting records that you attempted to update were not saved.",(modulename==NULL?"":modulename),(filename==NULL?"":filename));
					ERRORDIALOG("SUPPORTING RECORD NOT FOUND!",errordesc,NULL,FALSE);
					prterr("Error Security Doesn't Allow Save to Supporting Record: [%s]",errordesc);
					if(errordesc!=NULL) Rfree(errordesc);
				} else {
					LOCNRD(descnum);
					prev_file=RDATDataNEW(descnum);
					FINDFLDGETDOUBLE(descnum,"UNPROCESSED BALANCE",&initamt);
					initamt+=amount;
					FINDFLDSETDOUBLE(descnum,"UNPROCESSED BALANCE",initamt);
					if(ADVWRTTRANSsec(descnum,0,NULL,prev_file,SCRNvirtualSubData,mtnrsrc))
					{
						modulename=MODULENAME(descnum);
						filename=FILENAME(descnum);
						if(errordesc!=NULL) Rfree(errordesc);
						errordesc=Rmalloc(RDAstrlen(modulename)+RDAstrlen(filename)+100);
						sprintf(errordesc,"Write failed to the file [%s][%s], this supporting record that you attempted to update was not saved.",(modulename==NULL?"":modulename),(filename==NULL?"":filename));
						ERRORDIALOG("WRITE ERROR OCCURRED!",errordesc,NULL,FALSE);
						prterr("Error Write Failed to Supporting Record: [%s]",errordesc);
						if(errordesc!=NULL) Rfree(errordesc);
						UNLNRD(descnum);
						if(prev_file!=NULL) FreeRDATData(prev_file);
					} else {
						UNLNRD(descnum);
						if(prev_file!=NULL) FreeRDATData(prev_file);
						FINDFLDSETSTRING(mdacnum,"PERSONNEL IDENTIFICATION",perid1);
						FINDFLDSETSTRING(mdacnum,"DEDUCTION DESCRIPTION IDENTIFICATION",benefitid1);
						FINDFLDSETSHORT(mdacnum,"DEFINITION TYPE",deftype1);
						updatesupportingrecord(mtnrsrc,mdacnum,"UNPROCESSED BALANCE",amount);
						FINDFLDSETSTRING(ddacnum,"DEDUCTION DESCRIPTION IDENTIFICATION",benefitid1);
						FINDFLDSETSHORT(ddacnum,"DEFINITION TYPE",deftype1);
						updatesupportingrecord(mtnrsrc,ddacnum,"UNPROCESSED BALANCE",amount);
						FINDFLDSETSTRING(msrnum,"PERSONNEL IDENTIFICATION",perid1);
						FINDFLDSETSTRING(msrnum,"DEDUCTION DESCRIPTION IDENTIFICATION",benefitid1);
						updatesupportingrecord(mtnrsrc,msrnum,"UNPROCESSED BALANCE",amount);
					}
				}
			}
			updatebrowse(update_list,mbl_bfttran,MTNMASTER->passkey->fileno,MTNMASTER->module,mtnrsrc);
		} /* end if(numflds>0) */
		if(perid1!=NULL) Rfree(perid1);
		if(benefitid1!=NULL) Rfree(benefitid1);
		if(mbl_bfttran!=NULL)
		{
			quit_record(mtnrsrc,mtn);
		} else {
			seteditable(mtnrsrc,mtn,TRUE);
			GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
		}
	}
}
static void save_record_quit(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	save_check(mtnrsrc,mtn,FALSE);
}
static void quit_record_test(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	QUIT_RECORD_TEST(mtnrsrc,MTNMASTER->passkey->fileno,mtn->previous,readscreen,
		save_record_quit,quit_record,mtn);
}
static void quit_record(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	if(mtn!=NULL)
	{
		if(mtn->DEFtypes!=NULL) freeapplib(mtn->DEFtypes);
		if(mtn->previous!=NULL) FreeRDATData(mtn->previous);
		Rfree(mtn);
	}
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
static void getpreviouscb(RDArmem *member,mtnstruct *mtn)
{
	char *perid=NULL,*benefitid=NULL;
	short keyno=(-1);
	int tranno=0;

	readwidget(member->parent,"[BFTTRAN][PERSONNEL IDENTIFICATION]");
	readwidget(member->parent,"[BFTTRAN][DEDUCTION DESCRIPTION IDENTIFICATION]");
	readwidget(member->parent,"[BFTTRAN][TRANSACTION NUMBER]");
	FINDRSCGETSTRING(member->parent,"[BFTTRAN][PERSONNEL IDENTIFICATION]",&perid);
	FINDRSCGETSTRING(member->parent,"[BFTTRAN][DEDUCTION DESCRIPTION IDENTIFICATION]",&benefitid);
	FINDRSCGETINT(member->parent,"[BFTTRAN][TRANSACTION NUMBER]",&tranno);
	FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"DEDUCTION DESCRIPTION IDENTIFICATION",benefitid);
	FINDFLDSETINT(MTNMASTER->passkey->fileno,"TRANSACTION NUMBER",tranno);
	if(perid!=NULL) Rfree(perid);
	if(benefitid!=NULL) Rfree(benefitid);
	list2file(member->parent,mtn);
	keyno=KEYNUMBER(MTNMASTER->passkey->fileno,(member->rscrname+9));
	if(keyno==(-1)) keyno=MTNMASTER->passkey->keyno;
	if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,keyno,SCRNvirtualSubData,member->parent))
	{
		KEYNRD(MTNMASTER->passkey->fileno,keyno);
		if(ADVLTENRDsec(MTNMASTER->passkey->fileno,keyno,SCRNvirtualSubData,member->parent))
		{
			KEYNRD(MTNMASTER->passkey->fileno,keyno);
			set_appdefaults(member->parent,mtn,TRUE);
			ClearRDATData(mtn->previous);
			GET_SUPPORTING(member->parent,MTNMASTER,SCRNvirtualSubData,(void *)member->parent,TRUE,FALSE);
		} else {
			GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
			getrecordsetedit(member->parent,mtn,TRUE);
			GET_SUPPORTING(member->parent,MTNMASTER,SCRNvirtualSubData,(void *)member->parent,TRUE,TRUE);
		}
	} else {
		ADVPRVNRDsec(MTNMASTER->passkey->fileno,keyno,SCRNvirtualSubData,member->parent);
		GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
		getrecordsetedit(member->parent,mtn,TRUE);
		GET_SUPPORTING(member->parent,MTNMASTER,SCRNvirtualSubData,(void *)member->parent,TRUE,TRUE);
	}
	ADVupdateSCRNvirtuals(member->parent,SCRNvirtualSubData,member->parent);
}
static void getnextcb(RDArmem *member,mtnstruct *mtn)
{
	char *perid=NULL,*benefitid=NULL;
	short keyno=(-1);
	int tranno=0;

	readwidget(member->parent,"[BFTTRAN][PERSONNEL IDENTIFICATION]");
	readwidget(member->parent,"[BFTTRAN][DEDUCTION DESCRIPTION IDENTIFICATION]");
	readwidget(member->parent,"[BFTTRAN][TRANSACTION NUMBER]");
	FINDRSCGETSTRING(member->parent,"[BFTTRAN][PERSONNEL IDENTIFICATION]",&perid);
	FINDRSCGETSTRING(member->parent,"[BFTTRAN][DEDUCTION DESCRIPTION IDENTIFICATION]",&benefitid);
	FINDRSCGETINT(member->parent,"[BFTTRAN][TRANSACTION NUMBER]",&tranno);
	FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"DEDUCTION DESCRIPTION IDENTIFICATION",benefitid);
	FINDFLDSETINT(MTNMASTER->passkey->fileno,"TRANSACTION NUMBER",tranno);
	if(perid!=NULL) Rfree(perid);
	if(benefitid!=NULL) Rfree(benefitid);
	list2file(member->parent,mtn);
	keyno=KEYNUMBER(MTNMASTER->passkey->fileno,(member->rscrname+5));
	if(keyno==(-1)) keyno=MTNMASTER->passkey->keyno;
	if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,keyno,SCRNvirtualSubData,member->parent))
	{
		KEYNRD(MTNMASTER->passkey->fileno,keyno);
		if(ADVGTENRDsec(MTNMASTER->passkey->fileno,keyno,SCRNvirtualSubData,member->parent))
		{
			KEYNRD(MTNMASTER->passkey->fileno,keyno);
			set_appdefaults(member->parent,mtn,TRUE);
			ClearRDATData(mtn->previous);
			GET_SUPPORTING(member->parent,MTNMASTER,SCRNvirtualSubData,(void *)member->parent,TRUE,FALSE);
		} else {
			GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
			getrecordsetedit(member->parent,mtn,TRUE);
			GET_SUPPORTING(member->parent,MTNMASTER,SCRNvirtualSubData,(void *)member->parent,TRUE,TRUE);
		}
	} else {
		ADVNXTNRDsec(MTNMASTER->passkey->fileno,keyno,SCRNvirtualSubData,member->parent);
		GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
		getrecordsetedit(member->parent,mtn,TRUE);
		GET_SUPPORTING(member->parent,MTNMASTER,SCRNvirtualSubData,(void *)member->parent,TRUE,TRUE);
	}
	ADVupdateSCRNvirtuals(member->parent,SCRNvirtualSubData,member->parent);
}
static void getrecordcb(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	char *perid=NULL,*benefitid=NULL;
	int tranno=0;

	readwidget(mtnrsrc,"[BFTTRAN][PERSONNEL IDENTIFICATION]");
	readwidget(mtnrsrc,"[BFTTRAN][DEDUCTION DESCRIPTION IDENTIFICATION]");
	readwidget(mtnrsrc,"[BFTTRAN][TRANSACTION NUMBER]");
	FINDRSCGETSTRING(mtnrsrc,"[BFTTRAN][PERSONNEL IDENTIFICATION]",&perid);
	FINDRSCGETSTRING(mtnrsrc,"[BFTTRAN][DEDUCTION DESCRIPTION IDENTIFICATION]",&benefitid);
	FINDRSCGETINT(mtnrsrc,"[BFTTRAN][TRANSACTION NUMBER]",&tranno);
	FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"DEDUCTION DESCRIPTION IDENTIFICATION",benefitid);
	FINDFLDSETINT(MTNMASTER->passkey->fileno,"TRANSACTION NUMBER",tranno);
	if(perid!=NULL) Rfree(perid);
	if(benefitid!=NULL) Rfree(benefitid);
	list2file(mtnrsrc,mtn);
	if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno);
		set_appdefaults(mtnrsrc,mtn,TRUE);
		ClearRDATData(mtn->previous);
		GET_SUPPORTING(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc,TRUE,FALSE);
	} else {
		GetRDATData(MTNMASTER->passkey->fileno,mtn->previous);
		getrecordsetedit(mtnrsrc,mtn,TRUE);
		GET_SUPPORTING(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc,TRUE,TRUE);
	}
	ADVupdateSCRNvirtuals(mtnrsrc,SCRNvirtualSubData,mtnrsrc);
}
static void set_appdefaults(RDArsrc *mtnrsrc,mtnstruct *mtn,short update)
{
	char *date=NULL,*timex=NULL;

	seteditfilersrc(MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,mtnrsrc,TRUE,TRUE);
	SetRDAScrolledListEditable(mtnrsrc,MTNMASTER->passkey->fileno,MTNMASTER->passkey->keyno,TRUE,TRUE);
	FINDRSCSETSTRING(mtnrsrc,"[BFTTRAN][SOURCE USER]",USERLOGIN);
	date=GETCURRENTDATE();
	timex=GETCURRENTTIME();
	FINDRSCSETSTRING(mtnrsrc,"[BFTTRAN][ENTRY DATE]",date);
	FINDRSCSETSTRING(mtnrsrc,"[BFTTRAN][ENTRY TIME]",timex);
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
	if(update) updatefilersrc(MTNMASTER->passkey->fileno,mtnrsrc);
}
static void setfiles(RDArsrc *mtnrsrc,mtnstruct *mtn)
{
	list2file(mtnrsrc,mtn);
	SET_MTNMASTER(mtnrsrc,MTNMASTER,SCRNvirtualSubData,(void *)mtnrsrc);
}
