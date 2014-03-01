/* appaddcr.c - Add Applicants Criteria */
#include <cstdio>
#include <mix.hpp>

short appcrt=(-1),poscrit=(-1);
extern MaintainMaster *MTNMASTER;
RDArsrc *CRTrsrc=NULL,*CRTdefinelist=NULL;
APPlib *PREVCRITERIA=NULL;
static void savelist(RDArsrc *,RDArsrc *);
static void savelistcb(RDArsrc *,RDArsrc *);
static void CRTdefinelistfunc(RDArsrc *);
static void set_defaults(RDArsrc *);
static void reset_defaults(RDArsrc *);
static void save_check(RDArsrc *);
static void quit_record(RDArsrc *);
static void save_record(RDArsrc *);
static void makedflist(void);
static void readscreen(RDArsrc *);
static void setfiles(RDArsrc *);
static void getsupportingrecords(RDArsrc *,short);

static void savelistcb(RDArsrc *CRTdefinelist,RDArsrc *parent)
{
	savelist(CRTdefinelist,parent);
	if(IsScreenDisplayed(CRTdefinelist)) NULL_RSCS(CRTdefinelist);
}
static void savelist(RDArsrc *CRTdefinelist,RDArsrc *parent)
{
	short ef=0;
	char deleteflag=FALSE,*appid=NULL,*lappid=NULL;

	if(IsScreenDisplayed(CRTdefinelist)) readallwidgets(CRTdefinelist);
	if(PREVCRITERIA!=NULL) freeapplib(PREVCRITERIA);
	PREVCRITERIA=APPlibNEW();
	ZERNRD(appcrt);
	FINDRSCGETSTRING(parent,"[APPCRT][APPLICANT IDENTIFICATION]",&appid);
	FINDFLDSETSTRING(appcrt,"APPLICANT IDENTIFICATION",appid);
	ef=ADVGTENRDsec(appcrt,1,SCRNvirtualSubData,parent);
	while(!ef)
	{
		FINDFLDGETSTRING(appcrt,"APPLICANT IDENTIFICATION",&lappid);
		if(RDAstrcmp(appid,lappid)) break;
		FINDFLDGETCHAR(appcrt,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			makedflist();
		}
		ef=ADVNXTNRDsec(appcrt,1,SCRNvirtualSubData,parent);
	}
	if(PREVCRITERIA->numlibs<1)
	{
		addAPPlib(PREVCRITERIA,"No Education added this session");
	}
	FINDRSCLISTAPPlib(parent,"PREVIOUS CRITERIA",(PREVCRITERIA->numlibs-1),
		PREVCRITERIA);
	updatersrc(parent,"PREVIOUS CRITERIA");
}
static void makedflist()
{
	short x,cur=1,length=0,lcount=0;
	RDArmem *len_member,*pos_member;
	char *s,*hold=NULL,*temp=NULL;
	NRDfield *field;

	if(PREVCRITERIA!=NULL)
	{
		if(PREVCRITERIA->numlibs==1 && !RDAstrcmp(PREVCRITERIA->libs[0],
			"No Education added this session"))
		{
			freeapplib(PREVCRITERIA);
			PREVCRITERIA=APPlibNEW();
		}
	} else {
		PREVCRITERIA=APPlibNEW();
	}
	for(x=1,length=0;x<(CRTdefinelist->numrscs-5);x+=2)
	{
		pos_member=CRTdefinelist->rscs+x;
		len_member=CRTdefinelist->rscs+(x+1);
		if(*pos_member->value.short_value>0) 
		{
			length+=(*len_member->value.short_value+2);
		}
	}
	hold=Rmalloc(length+1);
	lcount=0;
	cur=0;
	while(cur<(CRTdefinelist->numrscs-5))
	{
		for(x=1;x<(CRTdefinelist->numrscs-5);x+=2)
		{
			pos_member=CRTdefinelist->rscs+x;
			len_member=CRTdefinelist->rscs+(x+1);
			if(*pos_member->value.short_value>0 && *len_member->value.short_value>0)
			{
				if(*pos_member->value.short_value==cur)
				{
					s=stralloc(pos_member->rscrname);
					s[RDAstrlen(s)-9]=0;
					field=FLDNRD(appcrt,s);
					if(field!=NULL)
					{
						switch(field->type)
						{
							case SCROLLEDTEXT:
							case VARIABLETEXT:
							case PLAINTEXT:
							case PHONE:
							case CUSTOMTYPE:
							case ZIPCODE:
							case SOCSECNUM:
							case DATES:
							case TIMES:
							case EXPENDITURE:
							case BALANCESHEET:
							case REVENUE:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								if(*len_member->value.short_value<RDAstrlen(field->data.string_value))
								{
									sprintf(&hold[lcount],"%.*s ",*len_member->value.short_value,(field->data.string_value!=NULL?field->data.string_value:""));
								} else {
									sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,(field->data.string_value!=NULL?field->data.string_value:""));
								}
								break;
							case DECIMALV:
							case SDECIMALV:
								sprintf(&hold[lcount],"%*f ",*len_member->value.short_value,*field->data.float_value);
								break;
							case SHORTV:
							case SSHORTV:
								sprintf(&hold[lcount],"%*d ",*len_member->value.short_value,*field->data.short_value);
								break;
							case LONGV:
							case SLONGV:
								sprintf(&hold[lcount],"%*d ",*len_member->value.short_value,*field->data.integer_value);
								break;
							case DOLLARS:
								temp=famt(*field->data.float_value,*len_member->value.short_value);
								sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,temp);
								if(temp!=NULL) Rfree(temp);
								break;
							case DOUBLEV:
							case SDOUBLEV:
								sprintf(&hold[lcount],"%*.0f ",*len_member->value.short_value,*field->data.float_value);
								break;
							case BOOLNS:
								sprintf(&hold[lcount],"%c ",(*field->data.string_value ? 'Y':'N'));
								break;
							case CHARACTERS:
								sprintf(&hold[lcount],"%c ",*field->data.string_value);
								break;
							case BUTTONS:
							case SCROLLEDLIST:
								prterr("Huh? Do a %s?",standardfieldtypes[field->type]);
								break;
							default:
								prterr("Invalid Field Type [%d]",field->type);
								break;
						}
						lcount+=*len_member->value.short_value+1;
					} else {
						prterr("Error field [%s] not found.",s);
						if(s!=NULL) Rfree(s);
					}
				}
			}
		}
		++cur;
	}
	addAPPlib(PREVCRITERIA,hold);
	if(hold!=NULL) Rfree(hold);
}
static void CRTdefinelistfunc(RDArsrc *parent)
{
	computeallSCRNvirtuals(CRTdefinelist);
	APPmakescrn(CRTdefinelist,TRUE,NULL,NULL,FALSE);
}
static void reset_defaults(RDArsrc *parent)
{
	set_defaults(parent);
	updateallrsrc(parent);
}
static void set_defaults(RDArsrc *parent)
{
	char *appid=NULL;

	FINDRSCGETSTRING(parent,"[APPCRT][APPLICANT IDENTIFICATION]",&appid);
	DefaultResetScreen(parent,TRUE);
	filerecord2rsrc(MTNMASTER->passkey->fileno,parent);
	FINDRSCSETSTRING(parent,"[APPCRT][APPLICANT IDENTIFICATION]",appid);
	FINDFLDSETSTRING(appcrt,"APPLICANT IDENTIFICATION",appid);
	if(appid!=NULL) Rfree(appid);
	ReadRDAScrolledLists(parent);
	getsupportingrecords(parent,FALSE);
	computeallSCRNvirtuals(parent);
}
static void getsupportingrecords(RDArsrc *parent,short update)
{
	COPYFIELD(appcrt,poscrit,"CRITERIA IDENTIFICATION");
	if(ADVEQLNRDsec(poscrit,1,SCRNvirtualSubData,parent)) KEYNRD(poscrit,1);
	if(update)
	{
		updatefilerecord2rsrc(poscrit,parent);
	} else {
		filerecord2rsrc(poscrit,parent);
	}
}
static void setfiles(RDArsrc *parent)
{
	readallwidgets(parent);
	rsrc2filerecord(appcrt,parent);
	getsupportingrecords(parent,TRUE);
	updateSCRNvirtuals(parent);
}
static void readscreen(RDArsrc *mtnrsrc)
{
	ZERNRD(appcrt);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(appcrt,mtnrsrc);
}
static void save_check(RDArsrc *mtnrsrc)
{
	char *perid=NULL,*warnmessage=NULL,delflag=FALSE,*type=NULL;

	readscreen(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"[APPCRT][APPLICANT IDENTIFICATION]",&perid);
	FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"APPLICANT IDENTIFICATION",perid);
	if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,1,SCRNvirtualSubData,mtnrsrc))
	{
		FINDFLDGETSTRING(MTNMASTER->passkey->fileno,"APPLICANT IDENTIFICATION",&perid);
		warnmessage=Rmalloc(RDAstrlen(perid)+181+RDAstrlen(MTNMASTER->maintain_screen));
		sprintf(warnmessage,"The Applicant Identification of [%s] is an invalid value. It does not exist in the [APPMGT][APPCNT] data file.  Please go back to the [%s] and select a valid Applicant Identification.",(perid==NULL?"":perid),MTNMASTER->maintain_screen);
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID APPLICANT IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		if(perid!=NULL) Rfree(perid);
		return;
	} else {
		FINDFLDGETCHAR(MTNMASTER->passkey->fileno,"DELETEFLAG",&delflag);
		if(delflag)
		{
			FINDFLDGETSTRING(MTNMASTER->passkey->fileno,"APPLICANT IDENTIFICATION",&perid);
			warnmessage=Rmalloc(RDAstrlen(perid)+175+RDAstrlen(MTNMASTER->maintain_screen));
			sprintf(warnmessage,"The Applicant Identification of [%s] exists in the [APPMGT][APPCNT] data file but is in a deleted state.  Please go back to the [%s] and select a valid Applicant Identification.",(perid==NULL?"":perid),MTNMASTER->maintain_screen);
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID APPLICANT IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			if(perid!=NULL) Rfree(perid);
			return;
		}
	}
	FINDFLDGETSTRING(appcrt,"CRITERIA IDENTIFICATION",&type);
	FINDFLDSETSTRING(poscrit,"CRITERIA IDENTIFICATION",type);
	if(ADVEQLNRDsec(poscrit,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(poscrit,1);
		warnmessage=Rmalloc(RDAstrlen(type)+178);
		sprintf(warnmessage,"The Criteria Identification of [%s] is an invalid value. It does not exist in the [POSTRK][POSCRIT] data file.  Please select a valid Criteria Identification to save this record.",(type==NULL?"":type));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CRITERIA IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		if(type!=NULL) Rfree(type);
		return;
	} else {
		FINDFLDGETCHAR(poscrit,"DELETEFLAG",&delflag);
		if(delflag)
		{
			warnmessage=Rmalloc(RDAstrlen(type)+161);
			sprintf(warnmessage,"The Criteria of [%s] exists in the [POSTRK][POSCRIT] data file but is in a deleted state.  Please and select a valid Criteria Identification to save this record.",(type==NULL?"":type));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CRITERIA IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			if(type!=NULL) Rfree(type);
			return;
		}
	}
	if(type!=NULL) Rfree(type);
	if(perid!=NULL) Rfree(perid);
	save_record(mtnrsrc);
}
static void save_record(RDArsrc *mtnrsrc)
{
	char *rcddesc=NULL;
	RDATData *prev=NULL;

	if(ADVRECORDsec(appcrt,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(appcrt,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
      			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		if(!ADVEQLNRDsec(appcrt,1,SCRNvirtualSubData,mtnrsrc))
		{
			prev=RDATDataNEW(appcrt);
			rsrc2filerecord(appcrt,mtnrsrc);
		} 
		WRTTRANS(appcrt,0,NULL,prev);
		if(prev!=NULL) FreeRDATData(prev);
		makedflist();
		if(!FINDRSCLISTAPPlib(mtnrsrc,"PREVIOUS CRITERIA",
			(PREVCRITERIA->numlibs-1),PREVCRITERIA))
		{
			updatersrc(mtnrsrc,"PREVIOUS CRITERIA");
		}
  	}
}
static void quit_record(RDArsrc *CRTrsrc)
{
	if(CRTrsrc!=NULL) free_rsrc(CRTrsrc);
	if(CRTdefinelist!=NULL) free_rsrc(CRTdefinelist);
	if(appcrt!=(-1)) CLSNRD(appcrt);
	if(poscrit!=(-1)) CLSNRD(poscrit);
	if(PREVCRITERIA!=NULL) freeapplib(PREVCRITERIA);
}
void add_criteria(RDArsrc *mtnrsrc)
{
	NRDkey *keys=NULL;
	NRDfield *fields=NULL,*fldx=NULL;
	short nokeys=0,nofields=0,x=0,edit_rsrc=TRUE;
	char *appid=NULL,*name=NULL,*filename=NULL;

	appcrt=OPNNRDsec(MTNMASTER->module,"APPCRT",TRUE,TRUE,FALSE);
	poscrit=OPNNRDsec("POSTRK","POSCRIT",TRUE,FALSE,FALSE);
	if(appcrt!=(-1) && poscrit!=(-1))
	{
		ZERNRD(appcrt);
		FINDFLDGETSTRING(MTNMASTER->passkey->fileno,"APPLICANT IDENTIFICATION",&appid);
		FINDFLDSETSTRING(appcrt,"APPLICANT IDENTIFICATION",appid);
		CRTrsrc=RDArsrcNEW(MTNMASTER->module,"ADD APPLICANTS CRITERIA");
		nokeys=NUMKEYS(appcrt);
		keys=KEYPOINTER(appcrt);
		nofields=NUMFLDS(appcrt);
		fields=FLDPOINTER(appcrt);
		CRTdefinelist=RDArsrcNEW(MTNMASTER->module,"ADD APPLICANTS CRITERIA DEFINE LIST");
		addDFincvir(CRTrsrc,MTNMASTER->module,"APPCNT",NULL,MTNMASTER->passkey->fileno);
		addDFincvir(CRTrsrc,MTNMASTER->module,"APPCRT",NULL,appcrt);
		addDFincvir(CRTrsrc,"POSTRK","POSCRIT",NULL,poscrit);
		addDFincvir(CRTdefinelist,MTNMASTER->module,"APPCNT",NULL,MTNMASTER->passkey->fileno);
		addDFincvir(CRTdefinelist,"POSTRK","POSCRIT",NULL,poscrit);
		addDFincvir(CRTdefinelist,MTNMASTER->module,"APPCRT",NULL,appcrt);
		GET_SCREEN_VIRTUALS(CRTrsrc,0);
		GET_SCREEN_VIRTUALS(CRTdefinelist,1);
		if(fields!=NULL)
		{
			filename=FILENAME(appcrt);
			for(x=0,fldx=fields;x<nofields;++x,++fldx)
			{
				if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
				{
				} else if(!RDAstrcmp(fldx->name,"APPLICANT IDENTIFICATION"))
				{
					edit_rsrc=FALSE;
					nonlstmakefld(CRTrsrc,appcrt,fldx,edit_rsrc);
				} else {
					edit_rsrc=TRUE;
					nonlstmakefld(CRTrsrc,appcrt,fldx,edit_rsrc);
					if(name!=NULL)
					{
						name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(filename));
					} else {
						name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(filename));
					}
					sprintf(name,"[%s][%s]",filename,fldx->name);
					FINDRSCSETFUNC(CRTrsrc,name,setfiles,NULL);
				}
			}
		}
		if(name!=NULL) Rfree(name);
		FINDRSCSETSTRING(CRTrsrc,"[APPCRT][APPLICANT IDENTIFICATION]",appid);
		file2rsrc(poscrit,CRTrsrc,FALSE);
		file2rsrc(MTNMASTER->passkey->fileno,CRTrsrc,FALSE);
		SCRNvirtual2rsrc(CRTrsrc);
		filerecord2rsrc(MTNMASTER->passkey->fileno,CRTrsrc);
		if(PREVCRITERIA!=NULL) freeapplib(PREVCRITERIA);
		PREVCRITERIA=APPlibNEW();
		addAPPlib(PREVCRITERIA,"No Education added this session");
		addlstrsrc(CRTrsrc,"PREVIOUS CRITERIA",&x,TRUE,NULL,PREVCRITERIA->numlibs,
			&PREVCRITERIA->libs,NULL);
		mk_define_list(CRTdefinelist,nofields,fields,nokeys,keys,NULL,1);
		FINDRSCSETFUNC(CRTdefinelist,"SELECT",savelistcb,CRTrsrc);
		DefaultScreens(CRTdefinelist);
		savelist(CRTdefinelist,CRTrsrc);
		SetVirFieldFuncs(CRTrsrc);
		addbtnrsrc(CRTrsrc,"DEFINE LIST",TRUE,CRTdefinelistfunc,NULL);
		addbtnrsrc(CRTrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
		addbtnrsrc(CRTrsrc,"RESET DEFAULTS",TRUE,reset_defaults,NULL);
		addrfcbrsrc(CRTrsrc,"SAVE",TRUE,save_check,NULL);
		addrfkwrsrc(CRTrsrc,"QUIT",TRUE,quit_record,NULL);
		addrfcbrsrc(CRTrsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(CRTrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		set_defaults(CRTrsrc);
		APPmakescrn(CRTrsrc,TRUE,quit_record,NULL,TRUE);
	} else {
		if(appcrt!=(-1)) CLSNRD(appcrt);
		if(poscrit!=(-1)) CLSNRD(poscrit);
		return;
	}
}
