/* appaddd.c - Add Applicants Documents */
#include <cstdio>
#include <mix.hpp>

short appdoc=(-1),appdtyp=(-1);
extern MaintainMaster *MTNMASTER;
RDArsrc *drsrc=NULL,*ddefinelist=NULL;
APPlib *PREVDOCS=NULL;
static void savelist(RDArsrc *,RDArsrc *);
static void savelistcb(RDArsrc *,RDArsrc *);
static void ddefinelistfunc(RDArsrc *);
static void reset_defaults(RDArsrc *);
static void set_defaults(RDArsrc *);
static void save_check(RDArsrc *);
static void quit_record(RDArsrc *);
static void save_record(RDArsrc *);
static void makedflist(void);
static void setfiles(RDArsrc *parent);
static void getsupportingrecords(RDArsrc *,short);
static void makedflist(void);
static void readscreen(RDArsrc *);

static void savelistcb(RDArsrc *ddefinelist,RDArsrc *parent)
{
	savelist(ddefinelist,parent);
	if(IsScreenDisplayed(ddefinelist)) NULL_RSCS(ddefinelist);
}
static void savelist(RDArsrc *ddefinelist,RDArsrc *parent)
{
	short ef=0;
	char deleteflag=FALSE,*appid=NULL,*lappid=NULL;

	if(IsScreenDisplayed(ddefinelist)) readallwidgets(ddefinelist);
	if(PREVDOCS!=NULL) freeapplib(PREVDOCS);
	PREVDOCS=APPlibNEW();
	ZERNRD(appdoc);
	FINDRSCGETSTRING(parent,"[APPDOC][APPLICANT IDENTIFICATION]",&appid);
	FINDFLDSETSTRING(appdoc,"APPLICANT IDENTIFICATION",appid);
	ef=ADVGTENRDsec(appdoc,1,SCRNvirtualSubData,parent);
	while(!ef)
	{
		FINDFLDGETSTRING(appdoc,"APPLICANT IDENTIFICATION",&lappid);
		if(RDAstrcmp(appid,lappid)) break;
		FINDFLDGETCHAR(appdoc,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			makedflist();
		}
		ef=ADVNXTNRDsec(appdoc,1,SCRNvirtualSubData,parent);
	}
	if(PREVDOCS->numlibs<1)
	{
		addAPPlib(PREVDOCS,"No Documents added this session");
	}
	FINDRSCLISTAPPlib(parent,"PREVIOUS DOCUMENTS",(PREVDOCS->numlibs-1),
		PREVDOCS);
	updatersrc(parent,"PREVIOUS DOCUMENTS");
}
static void makedflist()
{
	short x,cur=1,length=0,lcount=0;
	RDArmem *len_member,*pos_member;
	char *s,*hold=NULL,*temp=NULL;
	NRDfield *field;

	if(PREVDOCS!=NULL)
	{
		if(PREVDOCS->numlibs==1 && !RDAstrcmp(PREVDOCS->libs[0],
			"No Documents added this session"))
		{
			freeapplib(PREVDOCS);
			PREVDOCS=APPlibNEW();
		}
	} else {
		PREVDOCS=APPlibNEW();
	}
	for(x=1,length=0;x<(ddefinelist->numrscs-5);x+=2)
	{
		pos_member=ddefinelist->rscs+x;
		len_member=ddefinelist->rscs+(x+1);
		if(*pos_member->value.short_value>0) 
		{
			length+=(*len_member->value.short_value+2);
		}
	}
	hold=Rmalloc(length+1);
	lcount=0;
	cur=0;
	while(cur<(ddefinelist->numrscs-5))
	{
		for(x=1;x<(ddefinelist->numrscs-5);x+=2)
		{
			pos_member=ddefinelist->rscs+x;
			len_member=ddefinelist->rscs+(x+1);
			if(*pos_member->value.short_value>0 && *len_member->value.short_value>0)
			{
				if(*pos_member->value.short_value==cur)
				{
					s=stralloc(pos_member->rscrname);
					s[RDAstrlen(s)-9]=0;
					field=FLDNRD(appdoc,s);
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
	addAPPlib(PREVDOCS,hold);
	if(hold!=NULL) Rfree(hold);
}
static void ddefinelistfunc(RDArsrc *parent)
{
	computeallSCRNvirtuals(ddefinelist);
	APPmakescrn(ddefinelist,TRUE,NULL,NULL,FALSE);
}
static void reset_defaults(RDArsrc *parent)
{
	set_defaults(parent);
	updateallrsrc(parent);
}
static void set_defaults(RDArsrc *parent)
{
	char *appid=NULL;

	FINDRSCGETSTRING(parent,"[APPDOC][APPLICANT IDENTIFICATION]",&appid);
	DefaultResetScreen(parent,TRUE);
	filerecord2rsrc(MTNMASTER->passkey->fileno,parent);
	FINDRSCSETSTRING(parent,"[APPDOC][APPLICANT IDENTIFICATION]",appid);
	FINDFLDSETSTRING(appdoc,"APPLICANT IDENTIFICATION",appid);
	if(appid!=NULL) Rfree(appid);
	ReadRDAScrolledLists(parent);
	getsupportingrecords(parent,FALSE);
	computeallSCRNvirtuals(parent);
}
static void getsupportingrecords(RDArsrc *parent,short update)
{
        COPYFIELD(appdoc,appdtyp,"DOCUMENT TYPE IDENTIFICATION");
        if(ADVEQLNRDsec(appdtyp,1,SCRNvirtualSubData,parent)) KEYNRD(appdtyp,1);
        if(update)
        {
                updatefilerecord2rsrc(appdtyp,parent);
        } else {
                filerecord2rsrc(appdtyp,parent);
        }
}
static void setfiles(RDArsrc *parent)
{
	readallwidgets(parent);
	rsrc2filerecord(appdoc,parent);
	getsupportingrecords(parent,TRUE);
	updateSCRNvirtuals(parent);
}
static void readscreen(RDArsrc *mtnrsrc)
{
	ZERNRD(appdoc);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(appdoc,mtnrsrc);
}
static void save_check(RDArsrc *mtnrsrc)
{
	char *perid=NULL,*warnmessage=NULL,*type=NULL;
	char delflag=FALSE;

	readscreen(mtnrsrc);
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
	if(perid!=NULL) Rfree(perid);
	FINDFLDGETSTRING(appdoc,"DOCUMENT TYPE IDENTIFICATION",&type);
	FINDFLDSETSTRING(appdtyp,"DOCUMENT TYPE IDENTIFICATION",type);
	if(ADVEQLNRDsec(appdtyp,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(appdtyp,1);
		warnmessage=Rmalloc(RDAstrlen(type)+188);
		sprintf(warnmessage,"The Document Type Identification of [%s] is an invalid value. It does not exist in the [APPMGT][APPDTYP] data file.  Please select a valid Document Type Identification to save this record.",(type==NULL?"":type));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DOCUMENT TYPE IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		if(type!=NULL) Rfree(type);
		return;
	} else {
		FINDFLDGETCHAR(appdtyp,"DELETEFLAG",&delflag);
		if(delflag)
		{
			warnmessage=Rmalloc(RDAstrlen(type)+186);
			sprintf(warnmessage,"The Document Type Identification of [%s] exists in the [APPMGT][APPDTYP] data file but is in a deleted state.  Please and select a valid Document Type Identification to save this record.",(type==NULL?"":type));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID DOCUMENT TYPE IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			if(type!=NULL) Rfree(type);
			return;
		}
	}
	if(type!=NULL) Rfree(type);
	save_record(mtnrsrc);
}
static void save_record(RDArsrc *mtnrsrc)
{
	char *rcddesc=NULL;

	if(ADVRECORDsec(appdoc,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(appdoc,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
      			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(appdoc,1,"DOCUMENT NUMBER",NULL);
		makedflist();
		if(!FINDRSCLISTAPPlib(mtnrsrc,"PREVIOUS DOCUMENTS",
			(PREVDOCS->numlibs-1),PREVDOCS))
		{
			updatersrc(mtnrsrc,"PREVIOUS DOCUMENTS");
		}
  	}
}
static void quit_record(RDArsrc *drsrc)
{
	if(drsrc!=NULL) free_rsrc(drsrc);
	if(ddefinelist!=NULL) free_rsrc(ddefinelist);
	if(appdoc!=(-1)) CLSNRD(appdoc);
	if(appdtyp!=(-1)) CLSNRD(appdtyp);
	if(PREVDOCS!=NULL) freeapplib(PREVDOCS);
}
void add_documents(RDArsrc *mtnrsrc)
{
	char *name=NULL,*filename=NULL;
	NRDkey *keys=NULL;
	NRDfield *fields=NULL,*fldx=NULL;
	short nokeys=0,nofields=0,x=0,edit_rsrc=TRUE;
	char *appid=NULL;

	appdoc=OPNNRDsec(MTNMASTER->module,"APPDOC",TRUE,TRUE,FALSE);
	appdtyp=OPNNRDsec(MTNMASTER->module,"APPDTYP",TRUE,FALSE,FALSE);
	if(appdoc!=(-1) && appdtyp!=(-1))
	{
		ZERNRD(appdoc);
		FINDFLDGETSTRING(MTNMASTER->passkey->fileno,"APPLICANT IDENTIFICATION",&appid);
		FINDFLDSETSTRING(appdoc,"APPLICANT IDENTIFICATION",appid);
		drsrc=RDArsrcNEW(MTNMASTER->module,"ADD APPLICANTS DOCUMENTS");
		nokeys=NUMKEYS(appdoc);
		keys=KEYPOINTER(appdoc);
		nofields=NUMFLDS(appdoc);
		fields=FLDPOINTER(appdoc);
		ddefinelist=RDArsrcNEW(MTNMASTER->module,"ADD APPLICANTS DOCUMENTS DEFINE LIST");
		addDFincvir(drsrc,MTNMASTER->module,"APPCNT",NULL,MTNMASTER->passkey->fileno);
		addDFincvir(drsrc,MTNMASTER->module,"APPDOC",NULL,appdoc);
		addDFincvir(drsrc,MTNMASTER->module,"APPDTYP",NULL,appdtyp);
		addDFincvir(ddefinelist,MTNMASTER->module,"APPCNT",NULL,MTNMASTER->passkey->fileno);
		addDFincvir(ddefinelist,MTNMASTER->module,"APPDTYP",NULL,appdtyp);
		addDFincvir(ddefinelist,MTNMASTER->module,"APPDOC",NULL,appdoc);
		GET_SCREEN_VIRTUALS(drsrc,0);
		GET_SCREEN_VIRTUALS(ddefinelist,1);
		if(fields!=NULL)
		{
			filename=FILENAME(appdoc);
			for(x=0,fldx=fields;x<nofields;++x,++fldx)
			{
				if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
				{
				} else if(!RDAstrcmp(fldx->name,"DOCUMENT NUMBER"))
                                {
				} else if(!RDAstrcmp(fldx->name,"APPLICANT IDENTIFICATION"))
				{
					edit_rsrc=FALSE;
					nonlstmakefld(drsrc,appdoc,fldx,edit_rsrc);
				} else {
					edit_rsrc=TRUE;
					nonlstmakefld(drsrc,appdoc,fldx,edit_rsrc);
					if(name!=NULL)
					{
						name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(filename));
					} else {
						name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(filename));
					}
					sprintf(name,"[%s][%s]",filename,fldx->name);
					FINDRSCSETFUNC(drsrc,name,setfiles,NULL);
				}
			}
		}
		if(name!=NULL) Rfree(name);
		FINDRSCSETSTRING(drsrc,"[APPDOC][APPLICANT IDENTIFICATION]",appid);
		file2rsrc(appdtyp,drsrc,FALSE);
		file2rsrc(MTNMASTER->passkey->fileno,drsrc,FALSE);
		SCRNvirtual2rsrc(drsrc);
		filerecord2rsrc(MTNMASTER->passkey->fileno,drsrc);
		if(PREVDOCS!=NULL) freeapplib(PREVDOCS);
		PREVDOCS=APPlibNEW();
		addAPPlib(PREVDOCS,"No Documents added this session");
		addlstrsrc(drsrc,"PREVIOUS DOCUMENTS",&x,TRUE,NULL,PREVDOCS->numlibs,
			&PREVDOCS->libs,NULL);
		mk_define_list(ddefinelist,nofields,fields,nokeys,keys,NULL,1);
		nofields=0;
		nokeys=0;
		FINDRSCSETFUNC(ddefinelist,"SELECT",savelistcb,drsrc);
		DefaultScreens(ddefinelist);
		savelist(ddefinelist,drsrc);
		SetVirFieldFuncs(drsrc);
		addbtnrsrc(drsrc,"DEFINE LIST",TRUE,ddefinelistfunc,NULL);
		addbtnrsrc(drsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
		addbtnrsrc(drsrc,"RESET DEFAULTS",TRUE,reset_defaults,NULL);
		addrfcbrsrc(drsrc,"SAVE",TRUE,save_check,NULL);
		addrfkwrsrc(drsrc,"QUIT",TRUE,quit_record,NULL);
		addrfcbrsrc(drsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(drsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		set_defaults(drsrc);
		APPmakescrn(drsrc,TRUE,quit_record,NULL,TRUE);
	} else {
		if(appdoc!=(-1)) CLSNRD(appdoc);
		if(appdtyp!=(-1)) CLSNRD(appdtyp);
		return;
	}
}
