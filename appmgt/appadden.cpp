/* appadden.c - Add Applicants Endorsements */
#include <cstdio>
#include <mix.hpp>

short append=(-1),appendt=(-1),appmgt_certificates=(-1);
extern MaintainMaster *MTNMASTER;
RDArsrc *ENDrsrc=NULL,*ENDdefinelist=NULL;
APPlib *PREVENDS=NULL,*CERTIFICATES=NULL;
static void savelist(RDArsrc *,RDArsrc *);
static void savelistcb(RDArsrc *,RDArsrc *);
static void ENDdefinelistfunc(RDArsrc *);
static void set_defaults(RDArsrc *);
static void save_check(RDArsrc *);
static void quit_record(RDArsrc *);
static void save_record(RDArsrc *);
static void makedflist(void);
static void setfiles(RDArsrc *parent);
static void getsupportingrecords(RDArsrc *,short);
static void makedflist(void);
static void readscreen(RDArsrc *);

static void docertificatelist(RDArsrc *ENDrsrc)
{
	short ef=FALSE;
	char *appid=NULL,*lappid=NULL,deleteflag=FALSE;
	int certno=0;

	if(CERTIFICATES!=NULL) freeapplib(CERTIFICATES);
	CERTIFICATES=APPlibNEW();
	if(appmgt_certificates!=(-1))
	{
		ZERNRD(appmgt_certificates);
		FINDFLDGETSTRING(MTNMASTER->passkey->fileno,"APPLICANT IDENTIFICATION",&appid);
		FINDFLDSETSTRING(appmgt_certificates,"APPLICANT IDENTIFICATION",appid);
		ef=ADVGTENRDsec(appmgt_certificates,1,SCRNvirtualSubData,ENDrsrc);
		while(!ef)
		{
			FINDFLDGETSTRING(appmgt_certificates,"APPLICANT IDENTIFICATION",&lappid);
			if(RDAstrcmp(lappid,appid)) break;
			FINDFLDGETCHAR(appmgt_certificates,"DELETEFLAG",&deleteflag);
			if(!deleteflag)
			{
				FINDFLDGETINT(appmgt_certificates,"CERTIFICATE NUMBER",&certno);
				memset(stemp,0,101);
				sprintf(stemp,"%d",certno);
				addAPPlib(CERTIFICATES,stemp);
			}
			ef=ADVNXTNRDsec(appmgt_certificates,1,SCRNvirtualSubData,ENDrsrc);
		}
	}
	if(appid!=NULL) Rfree(appid);
	if(lappid!=NULL) Rfree(lappid);
	if(CERTIFICATES->numlibs<1)
	{
		addAPPlib(CERTIFICATES,"No Certificates Available");
	}
}
static void savelistcb(RDArsrc *ENDdefinelist,RDArsrc *parent)
{
	savelist(ENDdefinelist,parent);
	if(IsScreenDisplayed(ENDdefinelist)) NULL_RSCS(ENDdefinelist);
}
static void savelist(RDArsrc *ENDdefinelist,RDArsrc *parent)
{
	short ef=0;
	char deleteflag=FALSE,*appid=NULL,*lappid=NULL;

	if(IsScreenDisplayed(ENDdefinelist)) readallwidgets(ENDdefinelist);
	if(PREVENDS!=NULL) freeapplib(PREVENDS);
	PREVENDS=APPlibNEW();
	ZERNRD(append);
	FINDRSCGETSTRING(parent,"[APPEND][APPLICANT IDENTIFICATION]",&appid);
	FINDFLDSETSTRING(append,"APPLICANT IDENTIFICATION",appid);
	ef=ADVGTENRDsec(append,1,SCRNvirtualSubData,parent);
	while(!ef)
	{
		FINDFLDGETSTRING(append,"APPLICANT IDENTIFICATION",&lappid);
		if(RDAstrcmp(appid,lappid)) break;
		FINDFLDGETCHAR(append,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			makedflist();
		}
		ef=ADVNXTNRDsec(append,1,SCRNvirtualSubData,parent);
	}
	if(PREVENDS->numlibs<1)
	{
		addAPPlib(PREVENDS,"No Endorsements added this session");
	}
	FINDRSCLISTAPPlib(parent,"PREVIOUS ENDORSEMENTS",(PREVENDS->numlibs-1),
		PREVENDS);
	updatersrc(parent,"PREVIOUS ENDORSEMENTS");
}
static void makedflist()
{
	short x,cur=1,length=0,lcount=0;
	RDArmem *len_member,*pos_member;
	char *s,*hold=NULL,*temp=NULL;
	NRDfield *field;

	if(PREVENDS!=NULL)
	{
		if(PREVENDS->numlibs==1 && !RDAstrcmp(PREVENDS->libs[0],
			"No Endorsements added this session"))
		{
			freeapplib(PREVENDS);
			PREVENDS=APPlibNEW();
		}
	} else {
		PREVENDS=APPlibNEW();
	}
	for(x=1,length=0;x<(ENDdefinelist->numrscs-5);x+=2)
	{
		pos_member=ENDdefinelist->rscs+x;
		len_member=ENDdefinelist->rscs+(x+1);
		if(*pos_member->value.short_value>0) 
		{
			length+=(*len_member->value.short_value+2);
		}
	}
	hold=Rmalloc(length+1);
	lcount=0;
	cur=0;
	while(cur<(ENDdefinelist->numrscs-5))
	{
		for(x=1;x<(ENDdefinelist->numrscs-5);x+=2)
		{
			pos_member=ENDdefinelist->rscs+x;
			len_member=ENDdefinelist->rscs+(x+1);
			if(*pos_member->value.short_value>0 && *len_member->value.short_value>0)
			{
				if(*pos_member->value.short_value==cur)
				{
					s=stralloc(pos_member->rscrname);
					s[RDAstrlen(s)-9]=0;
					field=FLDNRD(append,s);
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
	addAPPlib(PREVENDS,hold);
	if(hold!=NULL) Rfree(hold);
}
static void ENDdefinelistfunc(RDArsrc *parent)
{
	computeallSCRNvirtuals(ENDdefinelist);
	APPmakescrn(ENDdefinelist,TRUE,NULL,NULL,FALSE);
}
static void reset_defaults(RDArsrc *parent)
{
	set_defaults(parent);
	updateallrsrc(parent);
}
static void set_defaults(RDArsrc *parent)
{
	char *appid=NULL;

	FINDRSCGETSTRING(parent,"[APPEND][APPLICANT IDENTIFICATION]",&appid);
	DefaultResetScreen(parent,TRUE);
	filerecord2rsrc(MTNMASTER->passkey->fileno,parent);
	FINDRSCSETSTRING(parent,"[APPEND][APPLICANT IDENTIFICATION]",appid);
	FINDFLDSETSTRING(append,"APPLICANT IDENTIFICATION",appid);
	if(appid!=NULL) Rfree(appid);
	ReadRDAScrolledLists(parent);
	getsupportingrecords(parent,FALSE);
	computeallSCRNvirtuals(parent);
}
static void getsupportingrecords(RDArsrc *parent,short update)
{
	int selected=0,certno=0;

	COPYFIELD(append,appendt,"TYPE");
	if(ADVEQLNRDsec(appendt,1,SCRNvirtualSubData,parent)) KEYNRD(appendt,1);
	COPYFIELD(append,appmgt_certificates,"APPLICANT IDENTIFICATION");
	FINDRSCGETINT(parent,"CERTIFICATE NUMBERS",&selected);
	certno=Ratoi(CERTIFICATES->libs[selected]);
	FINDFLDSETINT(appmgt_certificates,"CERTIFICATE NUMBER",certno);
	if(ADVEQLNRDsec(appmgt_certificates,1,SCRNvirtualSubData,parent)) KEYNRD(appmgt_certificates,1);
	if(update)
	{
		updatefilerecord2rsrc(appendt,parent);
		updatefilerecord2rsrc(appmgt_certificates,parent);
	} else {
		filerecord2rsrc(appendt,parent);
		filerecord2rsrc(appmgt_certificates,parent);
	}
}
static void setfiles(RDArsrc *parent)
{
        readallwidgets(parent);
        rsrc2filerecord(append,parent);
        getsupportingrecords(parent,TRUE);
        updateSCRNvirtuals(parent);
}
static void readscreen(RDArsrc *mtnrsrc)
{
	int selected=0,certno=0;

        ZERNRD(append);
        readallwidgets(mtnrsrc);
        ReadRDAScrolledLists(mtnrsrc);
        rsrc2filerecord(append,mtnrsrc);
	FINDRSCGETINT(mtnrsrc,"CERTIFICATE NUMBERS",&selected);
	certno=Ratoi(CERTIFICATES->libs[selected]);
	FINDFLDSETINT(append,"CERTIFICATE NUMBER",certno);
}
static void save_check(RDArsrc *mtnrsrc)
{
	int sel_type=0,certno=0;
	char *perid=NULL,*warnmessage=NULL,*type=NULL,delflag=FALSE;

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
	FINDRSCGETSTRING(mtnrsrc,"[APPEND][APPLICANT IDENTIFICATION]",&perid);
	FINDRSCGETINT(mtnrsrc,"CERTIFICATE NUMBERS",&sel_type);
	certno=Ratoi(CERTIFICATES->libs[sel_type]);
	FINDFLDSETSTRING(appmgt_certificates,"APPLICANT IDENTIFICATION",perid);
	FINDFLDSETINT(appmgt_certificates,"CERTIFICATE NUMBER",certno);
	if(ADVEQLNRDsec(appmgt_certificates,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(appmgt_certificates,1);
		warnmessage=Rmalloc(RDAstrlen(perid)+RDAstrlen(CERTIFICATES->libs[sel_type])+194);
		sprintf(warnmessage,"The Certificate Number [%s] with the Application Identification [%s] is invalid. It does not exist in the [APPMGT][APPCERT] data file.  Please select a valid Cerificate Number to save this record.",(CERTIFICATES->libs[sel_type]==NULL?"":CERTIFICATES->libs[sel_type]),(perid==NULL?"":perid));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CERTIFICATE NUMBER",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		if(perid!=NULL) Rfree(perid);
		return;
	} else {
		FINDFLDGETCHAR(appmgt_certificates,"DELETEFLAG",&delflag);
		if(delflag)
		{
			warnmessage=Rmalloc(RDAstrlen(perid)+RDAstrlen(CERTIFICATES->libs[sel_type])+197);
			sprintf(warnmessage,"The Certificate Number [%s] with the Application Identification [%s] exists in the [APPMGT][APPCERT] data file but is in a deleted state.  Please select a valid Cerificate Number to save this record.",(CERTIFICATES->libs[sel_type]==NULL?"":CERTIFICATES->libs[sel_type]),(perid==NULL?"":perid));
			WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED CERIFICATE!",warnmessage,NULL,NULL,TRUE,NULL);
			if(warnmessage!=NULL) Rfree(warnmessage);
			if(perid!=NULL) Rfree(perid);
			return;
		}
	}
	if(perid!=NULL) Rfree(perid);
	FINDFLDGETSTRING(append,"TYPE",&type);
	FINDFLDSETSTRING(appendt,"TYPE",type);
	if(ADVEQLNRDsec(appendt,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(appendt,1);
		warnmessage=Rmalloc(RDAstrlen(type)+140);
		sprintf(warnmessage,"The Type of [%s] is an invalid value. It does not exist in the [APPMGT][APPENDT] data file.  Please select a valid Type to save this record.",(type==NULL?"":type));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID TYPE!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		if(type!=NULL) Rfree(type);
		return;
	} else {
		FINDFLDGETCHAR(appendt,"DELETEFLAG",&delflag);
		if(delflag)
		{
			warnmessage=Rmalloc(RDAstrlen(type)+138);
			sprintf(warnmessage,"The Type of [%s] exists in the [APPMGT][APPENDT] data file but is in a deleted state.  Please and select a valid Type to save this record.",(type==NULL?"":type));
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

	if(ADVRECORDsec(append,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(append,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
      			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(append,1,"ENDORSEMENT NUMBER",NULL);
		makedflist();
		if(!FINDRSCLISTAPPlib(mtnrsrc,"PREVIOUS ENDORSEMENTS",
			(PREVENDS->numlibs-1),PREVENDS))
		{
			updatersrc(mtnrsrc,"PREVIOUS ENDORSEMENTS");
		}
  	}
}
static void quit_record(RDArsrc *ENDrsrc)
{
	if(ENDrsrc!=NULL) free_rsrc(ENDrsrc);
	if(ENDdefinelist!=NULL) free_rsrc(ENDdefinelist);
	if(CERTIFICATES!=NULL) freeapplib(CERTIFICATES);
	if(append!=(-1)) CLSNRD(append);
	if(appmgt_certificates!=(-1)) CLSNRD(appmgt_certificates);
	if(appendt!=(-1)) CLSNRD(appendt);
	if(PREVENDS!=NULL) freeapplib(PREVENDS);
}
void add_endorsements(RDArsrc *mtnrsrc)
{
	char *name=NULL,*filename=NULL;
	NRDkey *keys=NULL;
	NRDfield *fields=NULL,*fldx=NULL;
	short nokeys=0,nofields=0,x=0,edit_rsrc=TRUE;
	int start=0;
	char *appid=NULL;

	append=OPNNRDsec(MTNMASTER->module,"APPEND",TRUE,TRUE,FALSE);
	appendt=OPNNRDsec(MTNMASTER->module,"APPENDT",TRUE,FALSE,FALSE);
	appmgt_certificates=OPNNRDsec(MTNMASTER->module,"APPCERT",TRUE,FALSE,FALSE);
	if(append!=(-1) && appendt!=(-1))
	{
		ZERNRD(append);
		FINDFLDGETSTRING(MTNMASTER->passkey->fileno,"APPLICANT IDENTIFICATION",&appid);
		FINDFLDSETSTRING(append,"APPLICANT IDENTIFICATION",appid);
		ENDrsrc=RDArsrcNEW(MTNMASTER->module,"ADD APPLICANTS ENDORSEMENTS");
		nokeys=NUMKEYS(append);
		keys=KEYPOINTER(append);
		nofields=NUMFLDS(append);
		fields=FLDPOINTER(append);
		ENDdefinelist=RDArsrcNEW(MTNMASTER->module,"ADD APPLICANTS ENDORSEMENTS DEFINE LIST");
		addDFincvir(ENDrsrc,MTNMASTER->module,"APPCNT",NULL,MTNMASTER->passkey->fileno);
		addDFincvir(ENDrsrc,MTNMASTER->module,"APPEND",NULL,append);
		addDFincvir(ENDrsrc,MTNMASTER->module,"APPCERT",NULL,appmgt_certificates);
		addDFincvir(ENDrsrc,MTNMASTER->module,"APPENDT",NULL,appendt);
		addDFincvir(ENDdefinelist,MTNMASTER->module,"APPCNT",NULL,MTNMASTER->passkey->fileno);
		addDFincvir(ENDdefinelist,MTNMASTER->module,"APPCERT",NULL,appmgt_certificates);
		addDFincvir(ENDdefinelist,MTNMASTER->module,"APPEND",NULL,append);
		GET_SCREEN_VIRTUALS(ENDrsrc,0);
		GET_SCREEN_VIRTUALS(ENDdefinelist,1);
		if(fields!=NULL)
		{
			filename=FILENAME(append);
			for(x=0,fldx=fields;x<nofields;++x,++fldx)
			{
				if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
				{
				} else if(!RDAstrcmp(fldx->name,"ENDORSEMENT NUMBER"))
				{
				} else if(!RDAstrcmp(fldx->name,"CERTIFICATE NUMBER"))
				{
					start=0;
					edit_rsrc=TRUE;
					if(FIELDscrn(append,fldx->name,&edit_rsrc))
					{
						docertificatelist(ENDrsrc);
						addlstrsrc(ENDrsrc,"CERTIFICATE NUMBERS",
							&start,(edit_rsrc==TRUE?TRUE:FALSE),
							setfiles,CERTIFICATES->numlibs,
							&CERTIFICATES->libs,NULL);
						if(!edit_rsrc)
							FINDRSCSETEDITABLE(ENDrsrc,"CERTIFICATE NUMBERS",
								FALSE);
					}
				} else if(!RDAstrcmp(fldx->name,"APPLICANT IDENTIFICATION"))
				{
					edit_rsrc=FALSE;
					nonlstmakefld(ENDrsrc,append,fldx,edit_rsrc);
				} else {
					edit_rsrc=TRUE;
					nonlstmakefld(ENDrsrc,append,fldx,edit_rsrc);
					if(name!=NULL)
					{
						name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(filename));
					} else {
						name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(filename));
					}
					sprintf(name,"[%s][%s]",filename,fldx->name);
					FINDRSCSETFUNC(ENDrsrc,name,setfiles,NULL);
				}
			}
		}
		if(name!=NULL) Rfree(name);
		FINDRSCSETSTRING(ENDrsrc,"[APPEND][APPLICANT IDENTIFICATION]",appid);
		file2rsrc(appmgt_certificates,ENDrsrc,FALSE);
		file2rsrc(appendt,ENDrsrc,FALSE);
		file2rsrc(MTNMASTER->passkey->fileno,ENDrsrc,FALSE);
		SCRNvirtual2rsrc(ENDrsrc);
		filerecord2rsrc(MTNMASTER->passkey->fileno,ENDrsrc);
		if(PREVENDS!=NULL) freeapplib(PREVENDS);
		PREVENDS=APPlibNEW();
		addAPPlib(PREVENDS,"No Endorsements added this session");
		addlstrsrc(ENDrsrc,"PREVIOUS ENDORSEMENTS",&x,TRUE,NULL,PREVENDS->numlibs,
			&PREVENDS->libs,NULL);
		mk_define_list(ENDdefinelist,nofields,fields,nokeys,keys,NULL,1);
		FINDRSCSETFUNC(ENDdefinelist,"SELECT",savelistcb,ENDrsrc);
		DefaultScreens(ENDdefinelist);
		savelist(ENDdefinelist,ENDrsrc);
		SetVirFieldFuncs(ENDrsrc);
		addbtnrsrc(ENDrsrc,"DEFINE LIST",TRUE,ENDdefinelistfunc,NULL);
		addbtnrsrc(ENDrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
		addbtnrsrc(ENDrsrc,"RESET DEFAULTS",TRUE,reset_defaults,NULL);
		addrfcbrsrc(ENDrsrc,"SAVE",TRUE,save_check,NULL);
		addrfkwrsrc(ENDrsrc,"QUIT",TRUE,quit_record,NULL);
		addrfcbrsrc(ENDrsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(ENDrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		set_defaults(ENDrsrc);
		APPmakescrn(ENDrsrc,TRUE,quit_record,NULL,TRUE);
	} else {
		if(append!=(-1)) CLSNRD(append);
		if(appmgt_certificates!=(-1)) CLSNRD(appmgt_certificates);
		if(appendt!=(-1)) CLSNRD(appendt);
		return;
	}
}
