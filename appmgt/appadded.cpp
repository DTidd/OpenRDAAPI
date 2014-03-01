/* appadded.c - Add Applicants Education */
#include <cstdio>
#include <mix.hpp>

short appedm=(-1),appedt=(-1);
extern MaintainMaster *MTNMASTER;
RDArsrc *ersrc=NULL,*definelist=NULL;
APPlib *PREVEDS=NULL;
static void savelist(RDArsrc *,RDArsrc *);
static void savelistcb(RDArsrc *,RDArsrc *);
static void definelistfunc(RDArsrc *);
static void reset_defaults(RDArsrc *);
static void set_defaults(RDArsrc *);
static void save_check(RDArsrc *);
static void quit_record(RDArsrc *);
static void save_record(RDArsrc *);
static void setfiles(RDArsrc *parent);
static void getsupportingrecords(RDArsrc *,short);
static void makedflist(void);
static void readscreen(RDArsrc *);

static void savelistcb(RDArsrc *definelist,RDArsrc *parent)
{
	savelist(definelist,parent);
	if(IsScreenDisplayed(definelist)) NULL_RSCS(definelist);
}
static void savelist(RDArsrc *definelist,RDArsrc *parent)
{
	short ef=0;
	char deleteflag=FALSE,*appid=NULL,*lappid=NULL;

	if(IsScreenDisplayed(definelist)) readallwidgets(definelist);
	if(PREVEDS!=NULL) freeapplib(PREVEDS);
	PREVEDS=APPlibNEW();
	ZERNRD(appedm);
	FINDRSCGETSTRING(parent,"[APPEDM][APPLICANT IDENTIFICATION]",&appid);
	FINDFLDSETSTRING(appedm,"APPLICANT IDENTIFICATION",appid);
	ef=ADVGTENRDsec(appedm,1,SCRNvirtualSubData,parent);
	while(!ef)
	{
		FINDFLDGETSTRING(appedm,"APPLICANT IDENTIFICATION",&lappid);
		if(RDAstrcmp(appid,lappid)) break;
		FINDFLDGETCHAR(appedm,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			makedflist();
		}
		ef=ADVNXTNRDsec(appedm,1,SCRNvirtualSubData,parent);
	}
	if(PREVEDS->numlibs<1)
	{
		addAPPlib(PREVEDS,"No Education added this session");
	}
	FINDRSCLISTAPPlib(parent,"PREVIOUS EDUCATION",(PREVEDS->numlibs-1),
		PREVEDS);
	updatersrc(parent,"PREVIOUS EDUCATION");
}
static void makedflist()
{
	short x,cur=1,length=0,lcount=0;
	RDArmem *len_member,*pos_member;
	char *s,*hold=NULL,*temp=NULL;
	NRDfield *field;

	if(PREVEDS!=NULL)
	{
		if(PREVEDS->numlibs==1 && !RDAstrcmp(PREVEDS->libs[0],
			"No Education added this session"))
		{
			freeapplib(PREVEDS);
			PREVEDS=APPlibNEW();
		}
	} else {
		PREVEDS=APPlibNEW();
	}
	for(x=1,length=0;x<(definelist->numrscs-5);x+=2)
	{
		pos_member=definelist->rscs+x;
		len_member=definelist->rscs+(x+1);
		if(*pos_member->value.short_value>0) 
		{
			length+=(*len_member->value.short_value+2);
		}
	}
	hold=Rmalloc(length+1);
	lcount=0;
	cur=0;
	while(cur<(definelist->numrscs-5))
	{
		for(x=1;x<(definelist->numrscs-5);x+=2)
		{
			pos_member=definelist->rscs+x;
			len_member=definelist->rscs+(x+1);
			if(*pos_member->value.short_value>0 && *len_member->value.short_value>0)
			{
				if(*pos_member->value.short_value==cur)
				{
					s=stralloc(pos_member->rscrname);
					s[RDAstrlen(s)-9]=0;
					field=FLDNRD(appedm,s);
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
	addAPPlib(PREVEDS,hold);
	if(hold!=NULL) Rfree(hold);
}
static void definelistfunc(RDArsrc *parent)
{
	computeallSCRNvirtuals(definelist);
	APPmakescrn(definelist,TRUE,NULL,NULL,FALSE);
}
static void reset_defaults(RDArsrc *parent)
{
	set_defaults(parent);
	updateallrsrc(parent);
}
static void set_defaults(RDArsrc *parent)
{
	char *appid=NULL;

	FINDRSCGETSTRING(parent,"[APPEDM][APPLICANT IDENTIFICATION]",&appid);
	DefaultResetScreen(parent,TRUE);
	filerecord2rsrc(MTNMASTER->passkey->fileno,parent);
	FINDRSCSETSTRING(parent,"[APPEDM][APPLICANT IDENTIFICATION]",appid);
	FINDFLDSETSTRING(appedm,"APPLICANT IDENTIFICATION",appid);
	if(appid!=NULL) Rfree(appid);
	ReadRDAScrolledLists(parent);
	getsupportingrecords(parent,FALSE);
	computeallSCRNvirtuals(parent);
}
static void getsupportingrecords(RDArsrc *parent,short update)
{
	COPYFIELD(appedm,appedt,"TYPE");
	if(ADVEQLNRDsec(appedt,1,SCRNvirtualSubData,parent)) KEYNRD(appedt,1);
	if(update)
	{
		updatefilerecord2rsrc(appedt,parent);
	} else {
		filerecord2rsrc(appedt,parent);
	}
}
static void setfiles(RDArsrc *parent)
{
	readallwidgets(parent);
	rsrc2filerecord(appedm,parent);
	getsupportingrecords(parent,TRUE);
	updateSCRNvirtuals(parent);
} 
static void readscreen(RDArsrc *mtnrsrc)
{
	ZERNRD(appedm);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(appedm,mtnrsrc);
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
	FINDFLDGETSTRING(appedm,"TYPE",&type);
	FINDFLDSETSTRING(appedt,"TYPE",type);
	if(ADVEQLNRDsec(appedt,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(appedt,1);
		warnmessage=Rmalloc(RDAstrlen(type)+139);
		sprintf(warnmessage,"The Type of [%s] is an invalid value. It does not exist in the [APPMGT][APPEDT] data file.  Please select a valid Type to save this record.",(type==NULL?"":type));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID TYPE!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		if(type!=NULL) Rfree(type);
		return;
	} else {
		FINDFLDGETCHAR(appedt,"DELETEFLAG",&delflag);
		if(delflag)
		{
			warnmessage=Rmalloc(RDAstrlen(type)+137);
			sprintf(warnmessage,"The Type of [%s] exists in the [APPMGT][APPEDT] data file but is in a deleted state.  Please and select a valid Type to save this record.",(type==NULL?"":type));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID TYPE!",warnmessage,NULL,NULL,TRUE,NULL);
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

	if(ADVRECORDsec(appedm,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(appedm,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
      			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(appedm,1,"EDUCATION NUMBER",NULL);
		makedflist();
		if(!FINDRSCLISTAPPlib(mtnrsrc,"PREVIOUS EDUCATION",
			(PREVEDS->numlibs-1),PREVEDS))
		{
			updatersrc(mtnrsrc,"PREVIOUS EDUCATION");
		}
  	}
}
static void quit_record(RDArsrc *ersrc)
{
	if(ersrc!=NULL) free_rsrc(ersrc);
	if(definelist!=NULL) free_rsrc(definelist);
	if(appedm!=(-1)) CLSNRD(appedm);
	if(appedt!=(-1)) CLSNRD(appedt);
	if(PREVEDS!=NULL) freeapplib(PREVEDS);
}
void add_education(RDArsrc *mtnrsrc)
{
	char *name=NULL,*filename=NULL;
	NRDkey *keys=NULL;
	NRDfield *fields=NULL,*fldx=NULL;
	short nokeys=0,nofields=0,x=0,edit_rsrc=TRUE;
	char *appid=NULL;

	appedm=OPNNRDsec(MTNMASTER->module,"APPEDM",TRUE,TRUE,FALSE);
	appedt=OPNNRDsec(MTNMASTER->module,"APPEDT",TRUE,FALSE,FALSE);
	if(appedm!=(-1) && appedt!=(-1))
	{
		ZERNRD(appedm);
		FINDFLDGETSTRING(MTNMASTER->passkey->fileno,"APPLICANT IDENTIFICATION",&appid);
		FINDFLDSETSTRING(appedm,"APPLICANT IDENTIFICATION",appid);
		ersrc=RDArsrcNEW(MTNMASTER->module,"ADD APPLICANTS EDUCATION");
		nokeys=NUMKEYS(appedm);
		keys=KEYPOINTER(appedm);
		nofields=NUMFLDS(appedm);
		fields=FLDPOINTER(appedm);
		definelist=RDArsrcNEW(MTNMASTER->module,"ADD APPLICANTS EDUCATION DEFINE LIST");
		addDFincvir(ersrc,MTNMASTER->module,"APPCNT",NULL,MTNMASTER->passkey->fileno);
		addDFincvir(ersrc,MTNMASTER->module,"APPEDM",NULL,appedm);
		addDFincvir(ersrc,MTNMASTER->module,"APPEDT",NULL,appedt);
		addDFincvir(definelist,MTNMASTER->module,"APPCNT",NULL,MTNMASTER->passkey->fileno);
		addDFincvir(definelist,MTNMASTER->module,"APPEDT",NULL,appedt);
		addDFincvir(definelist,MTNMASTER->module,"APPEDM",NULL,appedm);
		GET_SCREEN_VIRTUALS(ersrc,0);
		GET_SCREEN_VIRTUALS(definelist,1);
		if(fields!=NULL)
		{
			filename=FILENAME(appedm);
			for(x=0,fldx=fields;x<nofields;++x,++fldx)
			{
				if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
				{
				} else if(!RDAstrcmp(fldx->name,"EDUCATION NUMBER"))
				{
				} else if(!RDAstrcmp(fldx->name,"APPLICANT IDENTIFICATION"))
				{
					edit_rsrc=FALSE;
					nonlstmakefld(ersrc,appedm,fldx,edit_rsrc);
				} else {
					edit_rsrc=TRUE;
					nonlstmakefld(ersrc,appedm,fldx,edit_rsrc);
					if(name!=NULL)
					{
						name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(filename));
					} else {
						name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(filename));
					}
					sprintf(name,"[%s][%s]",filename,fldx->name);
					FINDRSCSETFUNC(ersrc,name,setfiles,NULL);
				}
			}
		}
		if(name!=NULL) Rfree(name);
		FINDRSCSETSTRING(ersrc,"[APPEDM][APPLICANT IDENTIFICATION]",appid);
		file2rsrc(appedt,ersrc,FALSE);
		file2rsrc(MTNMASTER->passkey->fileno,ersrc,FALSE);
		SCRNvirtual2rsrc(ersrc);
		filerecord2rsrc(MTNMASTER->passkey->fileno,ersrc);
		if(PREVEDS!=NULL) freeapplib(PREVEDS);
		PREVEDS=APPlibNEW();
		addAPPlib(PREVEDS,"No Education added this session");
		addlstrsrc(ersrc,"PREVIOUS EDUCATION",&x,TRUE,NULL,PREVEDS->numlibs,
			&PREVEDS->libs,NULL);
		mk_define_list(definelist,nofields,fields,nokeys,keys,NULL,1);
		FINDRSCSETFUNC(definelist,"SELECT",savelistcb,ersrc);
		DefaultScreens(definelist);
		savelist(definelist,ersrc);
		SetVirFieldFuncs(ersrc);
		addbtnrsrc(ersrc,"DEFINE LIST",TRUE,definelistfunc,NULL);
		addbtnrsrc(ersrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
		addbtnrsrc(ersrc,"RESET DEFAULTS",TRUE,reset_defaults,NULL);
		addrfcbrsrc(ersrc,"SAVE",TRUE,save_check,NULL);
		addrfkwrsrc(ersrc,"QUIT",TRUE,quit_record,NULL);
		addrfcbrsrc(ersrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(ersrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		set_defaults(ersrc);
		APPmakescrn(ersrc,TRUE,quit_record,NULL,TRUE);
	} else {
		if(appedm!=(-1)) CLSNRD(appedm);
		if(appedt!=(-1)) CLSNRD(appedt);
		return;
	}
}
