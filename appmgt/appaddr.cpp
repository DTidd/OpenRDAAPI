/* appaddr.c - Add Applicants Reference */
#include <cstdio>
#include <mix.hpp>

short appref=(-1);
extern MaintainMaster *MTNMASTER;
RDArsrc *rrsrc=NULL,*rdefinelist=NULL;
APPlib *PREVREFS=NULL;
static void savelist(RDArsrc *,RDArsrc *);
static void savelistcb(RDArsrc *,RDArsrc *);
static void rdefinelistfunc(RDArsrc *);
static void reset_defaults(RDArsrc *);
static void set_defaults(RDArsrc *);
static void save_check(RDArsrc *);
static void quit_record(RDArsrc *);
static void save_record(RDArsrc *);
static void makedflist(void);
static void readscreen(RDArsrc *);

static void savelistcb(RDArsrc *rdefinelist,RDArsrc *parent)
{
	savelist(rdefinelist,parent);
	if(IsScreenDisplayed(rdefinelist)) NULL_RSCS(rdefinelist);
}
static void savelist(RDArsrc *rdefinelist,RDArsrc *parent)
{
	short ef=0;
	char deleteflag=FALSE,*appid=NULL,*lappid=NULL;

	if(IsScreenDisplayed(rdefinelist)) readallwidgets(rdefinelist);
	if(PREVREFS!=NULL) freeapplib(PREVREFS);
	PREVREFS=APPlibNEW();
	ZERNRD(appref);
	FINDRSCGETSTRING(parent,"[APPREF][APPLICANT IDENTIFICATION]",&appid);
	FINDFLDSETSTRING(appref,"APPLICANT IDENTIFICATION",appid);
	ef=ADVGTENRDsec(appref,1,SCRNvirtualSubData,parent);
	while(!ef)
	{
		FINDFLDGETSTRING(appref,"APPLICANT IDENTIFICATION",&lappid);
		if(RDAstrcmp(appid,lappid)) break;
		FINDFLDGETCHAR(appref,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			makedflist();
		}
		ef=ADVNXTNRDsec(appref,1,SCRNvirtualSubData,parent);
	}
	if(PREVREFS->numlibs<1)
	{
		addAPPlib(PREVREFS,"No References added this session");
	}
	FINDRSCLISTAPPlib(parent,"PREVIOUS REFERENCES",(PREVREFS->numlibs-1),
		PREVREFS);
	updatersrc(parent,"PREVIOUS REFERENCES");
}
static void makedflist()
{
	short x,cur=1,length=0,lcount=0;
	RDArmem *len_member,*pos_member;
	char *s,*hold=NULL,*temp=NULL;
	NRDfield *field;

	if(PREVREFS!=NULL)
	{
		if(PREVREFS->numlibs==1 && !RDAstrcmp(PREVREFS->libs[0],
			"No References added this session"))
		{
			freeapplib(PREVREFS);
			PREVREFS=APPlibNEW();
		}
	} else {
		PREVREFS=APPlibNEW();
	}
	for(x=1,length=0;x<(rdefinelist->numrscs-5);x+=2)
	{
		pos_member=rdefinelist->rscs+x;
		len_member=rdefinelist->rscs+(x+1);
		if(*pos_member->value.short_value>0) 
		{
			length+=(*len_member->value.short_value+2);
		}
	}
	hold=Rmalloc(length+1);
	lcount=0;
	cur=0;
	while(cur<(rdefinelist->numrscs-5))
	{
		for(x=1;x<(rdefinelist->numrscs-5);x+=2)
		{
			pos_member=rdefinelist->rscs+x;
			len_member=rdefinelist->rscs+(x+1);
			if(*pos_member->value.short_value>0 && *len_member->value.short_value>0)
			{
				if(*pos_member->value.short_value==cur)
				{
					s=stralloc(pos_member->rscrname);
					s[RDAstrlen(s)-9]=0;
					field=FLDNRD(appref,s);
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
	addAPPlib(PREVREFS,hold);
	if(hold!=NULL) Rfree(hold);
}
static void rdefinelistfunc(RDArsrc *parent)
{
	computeallSCRNvirtuals(rdefinelist);
	APPmakescrn(rdefinelist,TRUE,NULL,NULL,FALSE);
}
static void reset_defaults(RDArsrc *parent)
{
	set_defaults(parent);
	updateallrsrc(parent);
}
static void set_defaults(RDArsrc *parent)
{
	char *appid=NULL;

	FINDRSCGETSTRING(parent,"[APPREF][APPLICANT IDENTIFICATION]",&appid);
	DefaultResetScreen(parent,TRUE);
	filerecord2rsrc(MTNMASTER->passkey->fileno,parent);
	FINDRSCSETSTRING(parent,"[APPREF][APPLICANT IDENTIFICATION]",appid);
	FINDFLDSETSTRING(appref,"APPLICANT IDENTIFICATION",appid);
	if(appid!=NULL) Rfree(appid);
	ReadRDAScrolledLists(parent);
	computeallSCRNvirtuals(parent);
}
static void readscreen(RDArsrc *mtnrsrc)
{
	ZERNRD(appref);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(appref,mtnrsrc);
}
static void save_check(RDArsrc *mtnrsrc)
{
	char *perid=NULL,*warnmessage=NULL,delflag=FALSE;

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
	save_record(mtnrsrc);
}
static void save_record(RDArsrc *mtnrsrc)
{
	char *rcddesc=NULL;

	if(ADVRECORDsec(appref,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(appref,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
      			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(appref,1,"REFERENCE NUMBER",NULL);
		makedflist();
		if(!FINDRSCLISTAPPlib(mtnrsrc,"PREVIOUS REFERENCES",
			(PREVREFS->numlibs-1),PREVREFS))
		{
			updatersrc(mtnrsrc,"PREVIOUS REFERENCES");
		}
  	}
}
static void quit_record(RDArsrc *rrsrc)
{
	if(rrsrc!=NULL) free_rsrc(rrsrc);
	if(rdefinelist!=NULL) free_rsrc(rdefinelist);
	if(appref!=(-1)) CLSNRD(appref);
	if(PREVREFS!=NULL) freeapplib(PREVREFS);
}
void add_references(RDArsrc *mtnrsrc)
{
	NRDkey *keys=NULL;
	NRDfield *fields=NULL,*fldx=NULL;
	short nokeys=0,nofields=0,x=0,edit_rsrc=TRUE;
	char *appid=NULL;

	appref=OPNNRDsec(MTNMASTER->module,"APPREF",TRUE,TRUE,FALSE);
	if(appref!=(-1))
	{
		ZERNRD(appref);
		FINDFLDGETSTRING(MTNMASTER->passkey->fileno,"APPLICANT IDENTIFICATION",&appid);
		FINDFLDSETSTRING(appref,"APPLICANT IDENTIFICATION",appid);
		rrsrc=RDArsrcNEW(MTNMASTER->module,"ADD APPLICANTS REFERENCES");
		nokeys=NUMKEYS(appref);
		keys=KEYPOINTER(appref);
		nofields=NUMFLDS(appref);
		fields=FLDPOINTER(appref);
		rdefinelist=RDArsrcNEW(MTNMASTER->module,"ADD APPLICANTS REFERENCES DEFINE LIST");
		addDFincvir(rrsrc,MTNMASTER->module,"APPCNT",NULL,MTNMASTER->passkey->fileno);
		addDFincvir(rrsrc,MTNMASTER->module,"APPREF",NULL,appref);
		addDFincvir(rdefinelist,MTNMASTER->module,"APPCNT",NULL,MTNMASTER->passkey->fileno);
		addDFincvir(rdefinelist,MTNMASTER->module,"APPREF",NULL,appref);
		GET_SCREEN_VIRTUALS(rrsrc,0);
		GET_SCREEN_VIRTUALS(rdefinelist,1);
		if(fields!=NULL)
		{
			for(x=0,fldx=fields;x<nofields;++x,++fldx)
			{
				if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
				{
				} else if(!RDAstrcmp(fldx->name,"REFERENCE NUMBER"))
				{
				} else if(!RDAstrcmp(fldx->name,"APPLICANT IDENTIFICATION"))
				{
					edit_rsrc=FALSE;
					nonlstmakefld(rrsrc,appref,fldx,edit_rsrc);
				} else {
					edit_rsrc=TRUE;
					nonlstmakefld(rrsrc,appref,fldx,edit_rsrc);

				}
			}
		}
		FINDRSCSETSTRING(rrsrc,"[APPREF][APPLICANT IDENTIFICATION]",appid);
		file2rsrc(MTNMASTER->passkey->fileno,rrsrc,FALSE);
		SCRNvirtual2rsrc(rrsrc);
		filerecord2rsrc(MTNMASTER->passkey->fileno,rrsrc);
		if(PREVREFS!=NULL) freeapplib(PREVREFS);
		PREVREFS=APPlibNEW();
		addAPPlib(PREVREFS,"No References added this session");
		addlstrsrc(rrsrc,"PREVIOUS REFERENCES",&x,TRUE,NULL,PREVREFS->numlibs,
			&PREVREFS->libs,NULL);
		mk_define_list(rdefinelist,nofields,fields,nokeys,keys,NULL,1);
		FINDRSCSETFUNC(rdefinelist,"SELECT",savelistcb,rrsrc);
		DefaultScreens(rdefinelist);
		savelist(rdefinelist,rrsrc);
		SetVirFieldFuncs(rrsrc);
		addbtnrsrc(rrsrc,"DEFINE LIST",TRUE,rdefinelistfunc,NULL);
		addbtnrsrc(rrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
		addbtnrsrc(rrsrc,"RESET DEFAULTS",TRUE,reset_defaults,NULL);
		addrfcbrsrc(rrsrc,"SAVE",TRUE,save_check,NULL);
		addrfkwrsrc(rrsrc,"QUIT",TRUE,quit_record,NULL);
		addrfcbrsrc(rrsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(rrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		set_defaults(rrsrc);
		APPmakescrn(rrsrc,TRUE,quit_record,NULL,TRUE);
	} else {
		if(appref!=(-1)) CLSNRD(appref);
		return;
	}
}
