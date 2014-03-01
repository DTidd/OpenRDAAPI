/* appaddpe.c - Add Applicants Previous Employment */
#include <cstdio>
#include <mix.hpp>

short appemp=(-1),appprem=(-1);
extern MaintainMaster *MTNMASTER;
RDArsrc *persrc=NULL,*pedefinelist=NULL;
APPlib *petypes=NULL,*PREVEMP=NULL;
static void savelist(RDArsrc *,RDArsrc *);
static void savelistcb(RDArsrc *,RDArsrc *);
static void pedefinelistfunc(RDArsrc *);
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

static void savelistcb(RDArsrc *pedefinelist,RDArsrc *parent)
{
	savelist(pedefinelist,parent);
	if(IsScreenDisplayed(pedefinelist)) NULL_RSCS(pedefinelist);
}
static void savelist(RDArsrc *pedefinelist,RDArsrc *parent)
{
	short ef=0;
	char deleteflag=FALSE,*appid=NULL,*lappid=NULL;

	if(IsScreenDisplayed(pedefinelist)) readallwidgets(pedefinelist);
	if(PREVEMP!=NULL) freeapplib(PREVEMP);
	PREVEMP=APPlibNEW();
	ZERNRD(appemp);
	FINDRSCGETSTRING(parent,"[APPEMP][APPLICANT IDENTIFICATION]",&appid);
	FINDFLDSETSTRING(appemp,"APPLICANT IDENTIFICATION",appid);
	ef=ADVGTENRDsec(appemp,1,SCRNvirtualSubData,parent);
	while(!ef)
	{
		FINDFLDGETSTRING(appemp,"APPLICANT IDENTIFICATION",&lappid);
		if(RDAstrcmp(appid,lappid)) break;
		FINDFLDGETCHAR(appemp,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			makedflist();
		}
		ef=ADVNXTNRDsec(appemp,1,SCRNvirtualSubData,parent);
	}
	if(PREVEMP->numlibs<1)
	{
		addAPPlib(PREVEMP,"No Previous Employment added this session");
	}
	FINDRSCLISTAPPlib(parent,"PREVIOUS EMPLOYMENT",(PREVEMP->numlibs-1),
		PREVEMP);
	updatersrc(parent,"PREVIOUS EMPLOYMENT");
}
static void makedflist()
{
	short x,cur=1,length=0,lcount=0;
	RDArmem *len_member,*pos_member;
	char *s,*hold=NULL,*temp=NULL;
	NRDfield *field;

	if(PREVEMP!=NULL)
	{
		if(PREVEMP->numlibs==1 && !RDAstrcmp(PREVEMP->libs[0],
			"No Previous Employment added this session"))
		{
			freeapplib(PREVEMP);
			PREVEMP=APPlibNEW();
		}
	} else {
		PREVEMP=APPlibNEW();
	}
	for(x=1,length=0;x<(pedefinelist->numrscs-5);x+=2)
	{
		pos_member=pedefinelist->rscs+x;
		len_member=pedefinelist->rscs+(x+1);
		if(*pos_member->value.short_value>0) 
		{
			length+=(*len_member->value.short_value+2);
		}
	}
	hold=Rmalloc(length+1);
	lcount=0;
	cur=0;
	while(cur<(pedefinelist->numrscs-5))
	{
		for(x=1;x<(pedefinelist->numrscs-5);x+=2)
		{
			pos_member=pedefinelist->rscs+x;
			len_member=pedefinelist->rscs+(x+1);
			if(*pos_member->value.short_value>0 && *len_member->value.short_value>0)
			{
				if(*pos_member->value.short_value==cur)
				{
					s=stralloc(pos_member->rscrname);
					s[RDAstrlen(s)-9]=0;
					field=FLDNRD(appemp,s);
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
	addAPPlib(PREVEMP,hold);
	if(hold!=NULL) Rfree(hold);
}
static void pedefinelistfunc(RDArsrc *parent)
{
	computeallSCRNvirtuals(pedefinelist);
	APPmakescrn(pedefinelist,TRUE,NULL,NULL,FALSE);
}
static void reset_defaults(RDArsrc *parent)
{
	set_defaults(parent);
	updateallrsrc(parent);
}
static void set_defaults(RDArsrc *parent)
{
	char *appid=NULL;
	
	FINDRSCGETSTRING(parent,"[APPEMP][APPLICANT IDENTIFICATION]",&appid);
	DefaultResetScreen(parent,TRUE);
	filerecord2rsrc(MTNMASTER->passkey->fileno,parent);
	FINDRSCSETSTRING(parent,"[APPEMP][APPLICANT IDENTIFICATION]",appid);
	FINDFLDSETSTRING(appemp,"APPLICANT IDENTIFICATION",appid);
	if(appid!=NULL) Rfree(appid);
	ReadRDAScrolledLists(parent);
	getsupportingrecords(parent,FALSE);
	computeallSCRNvirtuals(parent);
}
static void getsupportingrecords(RDArsrc *parent,short update)
{
	COPYFIELD(appemp,appprem,"TYPE OF EMPLOYMENT");
	if(ADVEQLNRDsec(appprem,1,SCRNvirtualSubData,parent)) KEYNRD(appprem,1);
	if(update)
	{
		updatefilerecord2rsrc(appprem,parent);
	} else {
		filerecord2rsrc(appprem,parent);
	}
}
static void setfiles(RDArsrc *parent)
{
	readallwidgets(parent);
	rsrc2filerecord(appemp,parent);
	getsupportingrecords(parent,TRUE);
	updateSCRNvirtuals(parent);
}
static void readscreen(RDArsrc *mtnrsrc)
{
	ZERNRD(appemp);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(appemp,mtnrsrc);
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
		sprintf(warnmessage,"The Applicant Identification of [%s] is an invalid value. It does not exist in the [APPMGT][APPCNT] data file.  Please go back to the [%s] and select a valid Applicant Identification.",(perid==NULL?"":perid),MTNMASTER->maintain_screen)
;
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
	FINDFLDGETSTRING(appemp,"TYPE OF EMPLOYMENT",&type);
	FINDFLDSETSTRING(appprem,"TYPE OF EMPLOYMENT",type);
	if(ADVEQLNRDsec(appprem,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(appprem,1);
		warnmessage=Rmalloc(RDAstrlen(type)+165);
		sprintf(warnmessage,"The Type Of Employment [%s] is an invalid value. It does not exist in the [APPMGT][APPPREM] data file.  Please select a valid Type Of Employment to save this record.",(type==NULL?"":type));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID TYPE OF EMPLOYMENT!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		if(type!=NULL) Rfree(type);
		return;
	} else {
		FINDFLDGETCHAR(appprem,"DELETEFLAG",&delflag);
		if(delflag)
		{
			warnmessage=Rmalloc(RDAstrlen(type)+162);
			sprintf(warnmessage,"The Type Of Employment [%s] exists in the [APPMGT][APPPREM] data file but is in a deleted state.  Please and select a valid Type Of Employment to save this record.",(type==NULL?"":type));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID TYPE OF EMPLOYMENT!",warnmessage,NULL,NULL,TRUE,NULL);
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

	if(ADVRECORDsec(appemp,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(appemp,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
      			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(appemp,1,"TRANSACTION NUMBER",NULL);
		makedflist();
		if(!FINDRSCLISTAPPlib(mtnrsrc,"PREVIOUS EMPLOYMENT",
			(PREVEMP->numlibs-1),PREVEMP))
		{
			updatersrc(mtnrsrc,"PREVIOUS EMPLOYMENT");
		}
  	}
}
static void quit_record(RDArsrc *persrc)
{
	if(persrc!=NULL) free_rsrc(persrc);
	if(pedefinelist!=NULL) free_rsrc(pedefinelist);
	if(petypes!=NULL) freeapplib(petypes);
	if(appemp!=(-1)) CLSNRD(appemp);
	if(appprem!=(-1)) CLSNRD(appprem);
	if(PREVEMP!=NULL) freeapplib(PREVEMP);
}
void add_previous_employment(RDArsrc *mtnrsrc)
{
	char *name=NULL,*filename=NULL;
	NRDkey *keys=NULL;
	NRDfield *fields=NULL,*fldx=NULL;
	short nokeys=0,nofields=0,x=0,edit_rsrc=TRUE;
	char *appid=NULL;

	appemp=OPNNRDsec(MTNMASTER->module,"APPEMP",TRUE,TRUE,FALSE);
	appprem=OPNNRDsec(MTNMASTER->module,"APPPREM",TRUE,FALSE,FALSE);
	if(appemp!=(-1) && appprem!=(-1))
	{
		ZERNRD(appemp);
		FINDFLDGETSTRING(MTNMASTER->passkey->fileno,"APPLICANT IDENTIFICATION",&appid);
		FINDFLDSETSTRING(appemp,"APPLICANT IDENTIFICATION",appid);
		persrc=RDArsrcNEW(MTNMASTER->module,"ADD APPLICANTS PREVIOUS EMPLOYMENT");
		nokeys=NUMKEYS(appemp);
		keys=KEYPOINTER(appemp);
		nofields=NUMFLDS(appemp);
		fields=FLDPOINTER(appemp);
		pedefinelist=RDArsrcNEW(MTNMASTER->module,"ADD APPLICANTS PREVIOUS EMPLOYMENT DEFINE LIST");
		addDFincvir(persrc,MTNMASTER->module,"APPCNT",NULL,MTNMASTER->passkey->fileno);
		addDFincvir(persrc,MTNMASTER->module,"APPEMP",NULL,appemp);
		addDFincvir(persrc,MTNMASTER->module,"APPPREM",NULL,appprem);
		addDFincvir(pedefinelist,MTNMASTER->module,"APPCNT",NULL,MTNMASTER->passkey->fileno);
		addDFincvir(pedefinelist,MTNMASTER->module,"APPPREM",NULL,appprem);
		addDFincvir(pedefinelist,MTNMASTER->module,"APPEMP",NULL,appemp);
		GET_SCREEN_VIRTUALS(persrc,0);
		GET_SCREEN_VIRTUALS(pedefinelist,1);
		if(fields!=NULL)
		{
			filename=FILENAME(appemp);	
			for(x=0,fldx=fields;x<nofields;++x,++fldx)
			{
				if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
				{
				} else if(!RDAstrcmp(fldx->name,"TRANSACTION NUMBER"))
				{
				} else if(!RDAstrcmp(fldx->name,"APPLICANT IDENTIFICATION"))
				{
					edit_rsrc=FALSE;
					nonlstmakefld(persrc,appemp,fldx,edit_rsrc);
				} else {
					edit_rsrc=TRUE;
					nonlstmakefld(persrc,appemp,fldx,edit_rsrc);
					if(name!=NULL)
					{
						name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(filename));
					} else {
						name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(filename));
					}
					sprintf(name,"[%s][%s]",filename,fldx->name);
					FINDRSCSETFUNC(persrc,name,setfiles,NULL);
				}
			}
		}
		if(name!=NULL) Rfree(name);
		FINDRSCSETSTRING(persrc,"[APPEMP][APPLICANT IDENTIFICATION]",appid);
		file2rsrc(appprem,persrc,FALSE);
		file2rsrc(MTNMASTER->passkey->fileno,persrc,FALSE);
		SCRNvirtual2rsrc(persrc);
		filerecord2rsrc(MTNMASTER->passkey->fileno,persrc);
		if(PREVEMP!=NULL) freeapplib(PREVEMP);
		PREVEMP=APPlibNEW();
		addAPPlib(PREVEMP,"No Previous Employment added this session");
		addlstrsrc(persrc,"PREVIOUS EMPLOYMENT",&x,TRUE,NULL,PREVEMP->numlibs,
			&PREVEMP->libs,NULL);
		mk_define_list(pedefinelist,nofields,fields,nokeys,keys,NULL,1);
		FINDRSCSETFUNC(pedefinelist,"SELECT",savelistcb,persrc);
		DefaultScreens(pedefinelist);
		savelist(pedefinelist,persrc);
		SetVirFieldFuncs(persrc);
		addbtnrsrc(persrc,"DEFINE LIST",TRUE,pedefinelistfunc,NULL);
		addbtnrsrc(persrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
		addbtnrsrc(persrc,"RESET DEFAULTS",TRUE,reset_defaults,NULL);
		addrfcbrsrc(persrc,"SAVE",TRUE,save_check,NULL);
		addrfkwrsrc(persrc,"QUIT",TRUE,quit_record,NULL);
		addrfcbrsrc(persrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(persrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		set_defaults(persrc);
		APPmakescrn(persrc,TRUE,quit_record,NULL,TRUE);
	} else {
		if(appemp!=(-1)) CLSNRD(appemp);
		if(appprem!=(-1)) CLSNRD(appprem);
		return;
	}
}
