/* appaddct.c - Add Applicants Certifications */
#include <cstdio>
#include <mix.hpp>

short appcert=(-1),appctyp=(-1);
extern MaintainMaster *MTNMASTER;
RDArsrc *crsrc=NULL,*cdefinelist=NULL;
APPlib *PREVCERTS=NULL;
static void savelist(RDArsrc *,RDArsrc *);
static void savelistcb(RDArsrc *,RDArsrc *);
static void cdefinelistfunc(RDArsrc *);
static void reset_defaults(RDArsrc *);
static void set_defaults(RDArsrc *);
static void save_check(RDArsrc *);
static void quit_record(RDArsrc *);
static void save_record(RDArsrc *);
static void makedflist(void);
static void setfiles(RDArsrc *parent);
static void getsupportingrecords(RDArsrc *,short);
static void readscreen(RDArsrc *);

static void savelistcb(RDArsrc *cdefinelist,RDArsrc *parent)
{
	savelist(cdefinelist,parent);
	if(IsScreenDisplayed(cdefinelist)) NULL_RSCS(cdefinelist);
}
static void savelist(RDArsrc *cdefinelist,RDArsrc *parent)
{
	short ef=0;
	char deleteflag=FALSE,*appid=NULL,*lappid=NULL;

	if(IsScreenDisplayed(cdefinelist)) readallwidgets(cdefinelist);
	if(PREVCERTS!=NULL) freeapplib(PREVCERTS);
	PREVCERTS=APPlibNEW();
	ZERNRD(appcert);
	FINDRSCGETSTRING(parent,"[APPCERT][APPLICANT IDENTIFICATION]",&appid);
	FINDFLDSETSTRING(appcert,"APPLICANT IDENTIFICATION",appid);
	ef=ADVGTENRDsec(appcert,1,SCRNvirtualSubData,parent);
	while(!ef)
	{
		FINDFLDGETSTRING(appcert,"APPLICANT IDENTIFICATION",&lappid);
		if(RDAstrcmp(appid,lappid)) break;
		FINDFLDGETCHAR(appcert,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			makedflist();
		}
		ef=ADVNXTNRDsec(appcert,1,SCRNvirtualSubData,parent);
	}
	if(PREVCERTS->numlibs<1)
	{
		addAPPlib(PREVCERTS,"No Certifications added this session");
	}
	FINDRSCLISTAPPlib(parent,"PREVIOUS CERTIFICATIONS",(PREVCERTS->numlibs-1),
		PREVCERTS);
	updatersrc(parent,"PREVIOUS CERTIFICATIONS");
}
static void makedflist()
{
	short x,cur=1,length=0,lcount=0;
	RDArmem *len_member,*pos_member;
	char *s,*hold=NULL,*temp=NULL;
	NRDfield *field;

	if(PREVCERTS!=NULL)
	{
		if(PREVCERTS->numlibs==1 && !RDAstrcmp(PREVCERTS->libs[0],
			"No Certifications added this session"))
		{
			freeapplib(PREVCERTS);
			PREVCERTS=APPlibNEW();
		}
	} else {
		PREVCERTS=APPlibNEW();
	}
	for(x=1,length=0;x<(cdefinelist->numrscs-5);x+=2)
	{
		pos_member=cdefinelist->rscs+x;
		len_member=cdefinelist->rscs+(x+1);
		if(*pos_member->value.short_value>0) 
		{
			length+=(*len_member->value.short_value+2);
		}
	}
	hold=Rmalloc(length+1);
	lcount=0;
	cur=0;
	while(cur<(cdefinelist->numrscs-5))
	{
		for(x=1;x<(cdefinelist->numrscs-5);x+=2)
		{
			pos_member=cdefinelist->rscs+x;
			len_member=cdefinelist->rscs+(x+1);
			if(*pos_member->value.short_value>0 && *len_member->value.short_value>0)
			{
				if(*pos_member->value.short_value==cur)
				{
					s=stralloc(pos_member->rscrname);
					s[RDAstrlen(s)-9]=0;
					field=FLDNRD(appcert,s);
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
	addAPPlib(PREVCERTS,hold);
	if(hold!=NULL) Rfree(hold);
}
static void cdefinelistfunc(RDArsrc *parent)
{
	computeallSCRNvirtuals(cdefinelist);
	APPmakescrn(cdefinelist,TRUE,NULL,NULL,FALSE);
}
static void reset_defaults(RDArsrc *parent)
{
	set_defaults(parent);
	updateallrsrc(parent);
}
static void set_defaults(RDArsrc *parent)
{
	char *appid=NULL;

	FINDRSCGETSTRING(parent,"[APPCERT][APPLICANT IDENTIFICATION]",&appid);
	DefaultResetScreen(parent,TRUE);
	filerecord2rsrc(MTNMASTER->passkey->fileno,parent);
	FINDRSCSETSTRING(parent,"[APPCERT][APPLICANT IDENTIFICATION]",appid);
	FINDFLDSETSTRING(appcert,"APPLICANT IDENTIFICATION",appid);
	if(appid!=NULL) Rfree(appid);
	ReadRDAScrolledLists(parent);
	getsupportingrecords(parent,FALSE);
	computeallSCRNvirtuals(parent);
}
static void getsupportingrecords(RDArsrc *parent,short update)
{
	COPYFIELD(appcert,appctyp,"TYPE");
	if(ADVEQLNRDsec(appctyp,1,SCRNvirtualSubData,parent)) KEYNRD(appctyp,1);
	if(update)
	{
		updatefilerecord2rsrc(appctyp,parent);
	} else {
		filerecord2rsrc(appctyp,parent);
	}
}
static void setfiles(RDArsrc *parent)
{
	readallwidgets(parent);
	rsrc2filerecord(appcert,parent);
	getsupportingrecords(parent,TRUE);
	updateSCRNvirtuals(parent);
}
static void readscreen(RDArsrc *mtnrsrc)
{
	ZERNRD(appcert);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(appcert,mtnrsrc);
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
	FINDFLDGETSTRING(appcert,"TYPE",&type);
	FINDFLDSETSTRING(appctyp,"TYPE",type);
	if(ADVEQLNRDsec(appctyp,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(appctyp,1);
		warnmessage=Rmalloc(RDAstrlen(type)+140);
		sprintf(warnmessage,"The Type of [%s] is an invalid value. It does not exist in the [APPMGT][APPCTYP] data file.  Please select a valid Type to save this record.",(type==NULL?"":type));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID TYPE!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		if(type!=NULL) Rfree(type);
		return;
	} else {
		FINDFLDGETCHAR(appctyp,"DELETEFLAG",&delflag);
		if(delflag)
		{
			warnmessage=Rmalloc(RDAstrlen(type)+138);
			sprintf(warnmessage,"The Type of [%s] exists in the [APPMGT][APPCTYP] data file but is in a deleted state.  Please and select a valid Type to save this record.",(type==NULL?"":type));
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

	if(ADVRECORDsec(appcert,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(appcert,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
      			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(appcert,1,"CERTIFICATE NUMBER",NULL);
		makedflist();
		if(!FINDRSCLISTAPPlib(mtnrsrc,"PREVIOUS CERTIFICATIONS",
			(PREVCERTS->numlibs-1),PREVCERTS))
		{
			updatersrc(mtnrsrc,"PREVIOUS CERTIFICATIONS");
		}
  	}
}
static void quit_record(RDArsrc *crsrc)
{
	if(crsrc!=NULL) free_rsrc(crsrc);
	if(cdefinelist!=NULL) free_rsrc(cdefinelist);
	if(appcert!=(-1)) CLSNRD(appcert);
	if(appctyp!=(-1)) CLSNRD(appctyp);
	if(PREVCERTS!=NULL) freeapplib(PREVCERTS);
}
void add_certifications(RDArsrc *mtnrsrc)
{
	char *name=NULL,*filename=NULL;
	NRDkey *keys=NULL;
	NRDfield *fields=NULL,*fldx=NULL;
	short nokeys=0,nofields=0,x=0,edit_rsrc=TRUE;
	char *appid=NULL;

	appcert=OPNNRDsec(MTNMASTER->module,"APPCERT",TRUE,TRUE,FALSE);
	appctyp=OPNNRDsec(MTNMASTER->module,"APPCTYP",TRUE,FALSE,FALSE);
	if(appcert!=(-1) && appctyp!=(-1))
	{
		ZERNRD(appcert);
		FINDFLDGETSTRING(MTNMASTER->passkey->fileno,"APPLICANT IDENTIFICATION",&appid);
		FINDFLDSETSTRING(appcert,"APPLICANT IDENTIFICATION",appid);
		crsrc=RDArsrcNEW(MTNMASTER->module,"ADD APPLICANTS CERTIFICATIONS");
		nokeys=NUMKEYS(appcert);
		keys=KEYPOINTER(appcert);
		nofields=NUMFLDS(appcert);
		fields=FLDPOINTER(appcert);
		cdefinelist=RDArsrcNEW(MTNMASTER->module,"ADD APPLICANTS CERTIFICATIONS DEFINE LIST");
		addDFincvir(crsrc,MTNMASTER->module,"APPCNT",NULL,MTNMASTER->passkey->fileno);
		addDFincvir(crsrc,MTNMASTER->module,"APPCERT",NULL,appcert);
		addDFincvir(crsrc,MTNMASTER->module,"APPCTYP",NULL,appctyp);
		addDFincvir(cdefinelist,MTNMASTER->module,"APPCNT",NULL,MTNMASTER->passkey->fileno);
		addDFincvir(cdefinelist,MTNMASTER->module,"APPCTYP",NULL,appctyp);
		addDFincvir(cdefinelist,MTNMASTER->module,"APPCERT",NULL,appcert);
		GET_SCREEN_VIRTUALS(crsrc,0);
		GET_SCREEN_VIRTUALS(cdefinelist,1);
		if(fields!=NULL)
		{
			filename=FILENAME(appcert);
			for(x=0,fldx=fields;x<nofields;++x,++fldx)
			{
				if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
				{
				} else if(!RDAstrcmp(fldx->name,"CERTIFICATE NUMBER"))
				{
				} else if(!RDAstrcmp(fldx->name,"APPLICANT IDENTIFICATION"))
				{
					edit_rsrc=FALSE;
					nonlstmakefld(crsrc,appcert,fldx,edit_rsrc);
				} else {
					edit_rsrc=TRUE;
					nonlstmakefld(crsrc,appcert,fldx,edit_rsrc);
					if(name!=NULL)
					{
						name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(filename));
					} else {
						name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(filename));
					}
					sprintf(name,"[%s][%s]",filename,fldx->name);
					FINDRSCSETFUNC(crsrc,name,setfiles,NULL);
				}
			}
		}
		if(name!=NULL) Rfree(name);
		FINDRSCSETSTRING(crsrc,"[APPCERT][APPLICANT IDENTIFICATION]",appid);
		file2rsrc(appctyp,crsrc,FALSE);
		file2rsrc(MTNMASTER->passkey->fileno,crsrc,FALSE);
		SCRNvirtual2rsrc(crsrc);
		filerecord2rsrc(MTNMASTER->passkey->fileno,crsrc);
		if(PREVCERTS!=NULL) freeapplib(PREVCERTS);
		PREVCERTS=APPlibNEW();
		addAPPlib(PREVCERTS,"No Certifications added this session");
		addlstrsrc(crsrc,"PREVIOUS CERTIFICATIONS",&x,TRUE,NULL,PREVCERTS->numlibs,
			&PREVCERTS->libs,NULL);
		mk_define_list(cdefinelist,nofields,fields,nokeys,keys,NULL,1);
		FINDRSCSETFUNC(cdefinelist,"SELECT",savelistcb,crsrc);
		DefaultScreens(cdefinelist);
		savelist(cdefinelist,crsrc);
		SetVirFieldFuncs(crsrc);
		addbtnrsrc(crsrc,"DEFINE LIST",TRUE,cdefinelistfunc,NULL);
		addbtnrsrc(crsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
		addbtnrsrc(crsrc,"RESET DEFAULTS",TRUE,reset_defaults,NULL);
		addrfcbrsrc(crsrc,"SAVE",TRUE,save_check,NULL);
		addrfkwrsrc(crsrc,"QUIT",TRUE,quit_record,NULL);
		addrfcbrsrc(crsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(crsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		set_defaults(crsrc);
		APPmakescrn(crsrc,TRUE,quit_record,NULL,TRUE);
	} else {
		if(appcert!=(-1)) CLSNRD(appcert);
		if(appctyp!=(-1)) CLSNRD(appctyp);
		return;
	}
}
