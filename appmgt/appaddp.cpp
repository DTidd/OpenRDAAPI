/* appaddp.c - Add Applicants Positions */
#include <cstdio>
#include <mix.hpp>

short apppos=(-1),appcat=(-1),posmst=(-1),posadda=(-1);
extern MaintainMaster *MTNMASTER;
RDArsrc *POSrsrc=NULL,*POSdefinelist=NULL;
APPlib *PREVPOS=NULL;
static void savelist(RDArsrc *,RDArsrc *);
static void savelistcb(RDArsrc *,RDArsrc *);
static void POSdefinelistfunc(RDArsrc *);
static void set_defaults(RDArsrc *);
static void reset_defaults(RDArsrc *);
static void save_check(RDArsrc *);
static void quit_record(RDArsrc *);
static void save_record(RDArsrc *);
static void makedflist(void);
static void setfiles(RDArsrc *parent);
static void getsupportingrecords(RDArsrc *,short);
static void makedflist(void);
static void readscreen(RDArsrc *);

static void savelistcb(RDArsrc *POSdefinelist,RDArsrc *parent)
{
	savelist(POSdefinelist,parent);
	if(IsScreenDisplayed(POSdefinelist)) NULL_RSCS(POSdefinelist);
}
static void savelist(RDArsrc *POSdefinelist,RDArsrc *parent)
{
	short ef=0;
	char deleteflag=FALSE,*appid=NULL,*lappid=NULL;
	char acknowledged=FALSE;

	if(IsScreenDisplayed(POSdefinelist)) readallwidgets(POSdefinelist);
	if(PREVPOS!=NULL) freeapplib(PREVPOS);
	PREVPOS=APPlibNEW();
	ZERNRD(apppos);
	FINDRSCGETSTRING(parent,"[APPPOS][APPLICANT IDENTIFICATION]",&appid);
	FINDFLDSETSTRING(apppos,"APPLICANT IDENTIFICATION",appid);
	ef=ADVGTENRDsec(apppos,1,SCRNvirtualSubData,parent);
	while(!ef)
	{
		FINDFLDGETSTRING(apppos,"APPLICANT IDENTIFICATION",&lappid);
		if(RDAstrcmp(appid,lappid)) break;
		COPYFIELD(apppos,appcat,"APPLICANT IDENTIFICATION");
		COPYFIELD(apppos,appcat,"APPLICATION NUMBER");
		if(ADVEQLNRDsec(appcat,1,SCRNvirtualSubData,parent)) 
			KEYNRD(appcat,1);
		FINDFLDGETCHAR(apppos,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(appcat,"ACKNOWLEDGED",&acknowledged);
		if(!deleteflag && !acknowledged)
		{
			makedflist();
		}
		ef=ADVNXTNRDsec(apppos,1,SCRNvirtualSubData,parent);
	}
	if(PREVPOS->numlibs<1)
	{
		addAPPlib(PREVPOS,"No References added this session");
	}
	FINDRSCLISTAPPlib(parent,"PREVIOUS POSITIONS",(PREVPOS->numlibs-1),
		PREVPOS);
	updatersrc(parent,"PREVIOUS POSITIONS");
}
static void makedflist()
{
	short x,cur=1,length=0,lcount=0;
	RDArmem *len_member,*pos_member;
	char *s,*hold=NULL,*temp=NULL;
	NRDfield *field;

	if(PREVPOS!=NULL)
	{
		if(PREVPOS->numlibs==1 && !RDAstrcmp(PREVPOS->libs[0],
			"No References added this session"))
		{
			freeapplib(PREVPOS);
			PREVPOS=APPlibNEW();
		}
	} else {
		PREVPOS=APPlibNEW();
	}
	for(x=1,length=0;x<(POSdefinelist->numrscs-5);x+=2)
	{
		pos_member=POSdefinelist->rscs+x;
		len_member=POSdefinelist->rscs+(x+1);
		if(*pos_member->value.short_value>0) 
		{
			length+=(*len_member->value.short_value+2);
		}
	}
	hold=Rmalloc(length+1);
	lcount=0;
	cur=0;
	while(cur<(POSdefinelist->numrscs-5))
	{
		for(x=1;x<(POSdefinelist->numrscs-5);x+=2)
		{
			pos_member=POSdefinelist->rscs+x;
			len_member=POSdefinelist->rscs+(x+1);
			if(*pos_member->value.short_value>0 && *len_member->value.short_value>0)
			{
				if(*pos_member->value.short_value==cur)
				{
					s=stralloc(pos_member->rscrname);
					s[RDAstrlen(s)-9]=0;
					field=FLDNRD(apppos,s);
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
	addAPPlib(PREVPOS,hold);
	if(hold!=NULL) Rfree(hold);
}
static void POSdefinelistfunc(RDArsrc *parent)
{
	computeallSCRNvirtuals(POSdefinelist);
	APPmakescrn(POSdefinelist,TRUE,NULL,NULL,FALSE);
}
static void reset_defaults(RDArsrc *parent)
{
	set_defaults(parent);
	updateallrsrc(parent);
}
static void set_defaults(RDArsrc *parent)
{
	int selprev=0;
	char *appid=NULL;

	FINDRSCGETINT(parent,"PREVIOUS POSITIONS",&selprev);
	FINDRSCGETSTRING(parent,"[APPPOS][APPLICANT IDENTIFICATION]",&appid);
	DefaultResetScreen(parent,TRUE);
	FINDRSCSETSTRING(parent,"[APPPOS][APPLICANT IDENTIFICATION]",appid);
	FINDFLDSETSTRING(apppos,"APPLICANT IDENTIFICATION",appid);
	FINDFLDSETSTRING(MTNMASTER->passkey->fileno,"APPLICANT IDENTIFICATION",appid);
	if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,1,SCRNvirtualSubData,parent)) KEYNRD(MTNMASTER->passkey->fileno,1);
	filerecord2rsrc(MTNMASTER->passkey->fileno,parent);
	computeallSCRNvirtuals(parent);
	updateallrsrc(parent);
}
static void save_check(RDArsrc *mtnrsrc)
{
	char *perid=NULL,*warnmessage=NULL,delflag=FALSE;
	int appno=0;

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
	FINDRSCGETSTRING(mtnrsrc,"[APPPOS][APPLICANT IDENTIFICATION]",&perid);
	FINDRSCGETINT(mtnrsrc,"[APPPOS][APPLICATION NUMBER]",&appno);
	FINDRSCGETSTRING(mtnrsrc,"[APPPOS][POSITION IDENTIFICATION]",&perid);
	FINDFLDSETSTRING(posmst,"POSITION IDENTIFICATION",perid);
	if(ADVEQLNRDsec(posmst,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(posmst,1);
		warnmessage=Rmalloc(RDAstrlen(perid)+166);
		sprintf(warnmessage,"The Positiion Identification [%s] is invalid. It does not exist in the [POSTRK][POSMSTR] data file.  Please select a valid Position Identification to save this record.",(perid==NULL?"":perid));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID POSITION IDENTIFICATION",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		if(perid!=NULL) Rfree(perid);
		return;
	} else {
		FINDFLDGETCHAR(posmst,"DELETEFLAG",&delflag);
		if(delflag)
		{
			warnmessage=Rmalloc(RDAstrlen(perid)+168);
			sprintf(warnmessage,"The Position Identification [%s] exists in the [POSTRK][POSMSTR] data file but is in a deleted state.  Please select a valid Position Identification to save this record.",(perid==NULL?"":perid));
			WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED POSITION IDENTIFICAITON!",warnmessage,NULL,NULL,TRUE,NULL);
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
	char *rcddesc=NULL,deleteflag=FALSE,*posid=NULL,*lposid=NULL;
	char *curdate=NULL;
	int appno=0;
	RDATData *prev=NULL;
	short ef=FALSE;

	COPYFIELD(MTNMASTER->passkey->fileno,appcat,"APPLICANT IDENTIFICATION");
	COPYFIELD(MTNMASTER->passkey->fileno,appcat,"APPLICATION NUMBER");
	if(ADVEQLNRDsec(appcat,1,SCRNvirtualSubData,mtnrsrc))
	{
		ZERNRD(appcat);
		rsrc2filerecord(appcat,mtnrsrc);
		COPYFIELD(apppos,appcat,"APPLICANT IDENTIFICATION");
		FINDFLDGETSTRING(appcat,"APPLICATION DATE",&curdate);
		if(isEMPTY(curdate))
		{
			if(curdate!=NULL) Rfree(curdate);
			curdate=GETCURRENTDATE10();
			FINDFLDSETSTRING(appcat,"APPLICATION DATE",curdate);
			if(curdate!=NULL) Rfree(curdate);
		}
		FINDFLDGETSTRING(appcat,"APPLICATION TIME",&curdate);
		if(isEMPTY(curdate))
		{
			if(curdate!=NULL) Rfree(curdate);
			curdate=GETCURRENTTIME();
			FINDFLDSETSTRING(appcat,"APPLICATION TIME",curdate);
			if(curdate!=NULL) Rfree(curdate);
		}
		FINDFLDSETCHAR(appcat,"ACKNOWLEDGED",deleteflag);
		ADVWRTTRANSsec(appcat,1,"APPLICATION NUMBER",NULL,
			SCRNvirtualSubData,mtnrsrc);
	}
	FINDFLDGETINT(appcat,"APPLICATION NUMBER",&appno);
	FINDFLDSETINT(apppos,"APPLICATION NUMBER",appno);
	if(ADVRECORDsec(apppos,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(apppos,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
      			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		if(ADVEQLNRDsec(apppos,1,SCRNvirtualSubData,mtnrsrc))
		{
			prev=NULL;
		} else {
			prev=RDATDataNEW(apppos);
			FINDFLDSETCHAR(apppos,"DELETEFLAG",deleteflag);
			rsrc2filerecord(apppos,mtnrsrc);
			FINDFLDSETINT(apppos,"APPLICATION NUMBER",appno);
		}
		ADVWRTTRANSsec(apppos,0,NULL,prev,SCRNvirtualSubData,mtnrsrc);
		if(prev!=NULL) FreeRDATData(prev);
		makedflist();
		COPYFIELD(apppos,posmst,"POSITION IDENTIFICATION");
		if(ADVEQLNRDsec(posmst,1,SCRNvirtualSubData,mtnrsrc)) 
			KEYNRD(posmst,1);
		FINDFLDGETCHAR(posmst,"ADDENDA",&deleteflag);
		if(posadda!=(-1) && deleteflag)
		{
			FINDFLDGETSTRING(apppos,"POSITION IDENTIFICATION",&posid);
			ZERNRD(posadda);
			FINDFLDSETSTRING(posadda,"POSITION IDENTIFICATION",posid);
			ef=ADVGTENRDsec(posadda,1,SCRNvirtualSubData,mtnrsrc);
			while(!ef)
			{
				FINDFLDGETSTRING(posadda,"POSITION IDENTIFICATION",&lposid);
				if(RDAstrcmp(posid,lposid)) break;
				FINDFLDGETCHAR(posadda,"DELETEFLAG",&deleteflag);
				if(!deleteflag)
				{
					FIELDCOPY(posadda,"ADDENDA POSITION IDENTIFICATION",
						apppos,"POSITION IDENTIFICATION");
					if(ADVEQLNRDsec(apppos,1,SCRNvirtualSubData,mtnrsrc))
					{
						prev=NULL;
					} else {
						prev=RDATDataNEW(apppos);
						FINDFLDSETCHAR(apppos,"DELETEFLAG",deleteflag);
						FIELDCOPY(posadda,"ADDENDA POSITION IDENTIFICATION",
							apppos,"POSITION IDENTIFICATION");
						rsrc2filerecord(apppos,mtnrsrc);
						FINDFLDSETINT(apppos,"APPLICATION NUMBER",appno);
					}
					ADVWRTTRANSsec(apppos,0,NULL,prev,SCRNvirtualSubData,
						mtnrsrc);
					if(prev!=NULL) FreeRDATData(prev);
					makedflist();
				}
				ef=ADVNXTNRDsec(posadda,1,SCRNvirtualSubData,mtnrsrc);
			}
			if(posid!=NULL) Rfree(posid);
			if(lposid!=NULL) Rfree(lposid);
		}
		if(!FINDRSCLISTAPPlib(mtnrsrc,"PREVIOUS POSITIONS",
			(PREVPOS->numlibs-1),PREVPOS))
		{
			updatersrc(mtnrsrc,"PREVIOUS POSITIONS");
		}
  	}
}
static void quit_record(RDArsrc *POSrsrc)
{
	if(POSrsrc!=NULL) free_rsrc(POSrsrc);
	if(POSdefinelist!=NULL) free_rsrc(POSdefinelist);
	if(apppos!=(-1)) CLSNRD(apppos);
	if(posadda!=(-1)) CLSNRD(posadda);
	if(posmst!=(-1)) CLSNRD(posmst);
	if(appcat!=(-1)) CLSNRD(appcat);
	if(PREVPOS!=NULL) freeapplib(PREVPOS);
}
static void doexit_addenda(MakeBrowseList *blist)
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
}
static void init_addenda(MakeBrowseList *blist)
{
	char *posid=NULL;

	ZERNRD(posadda);
	readwidget(blist->funcargs,"[APPPOS][POSITION IDENTIFICATION]");
	FINDRSCGETSTRING(blist->funcargs,"[APPPOS][POSITION IDENTIFICATION]",
		&posid);
	FINDFLDSETSTRING(posadda,"POSITION IDENTIFICATION",posid);
	if(posid!=NULL) Rfree(posid);
}
static short break_addenda(MakeBrowseList *blist)
{
	char *posid=NULL,*lposid=NULL;
	short retval=FALSE;

	readwidget(blist->funcargs,"[APPPOS][POSITION IDENTIFICATION]");
	FINDRSCGETSTRING(blist->funcargs,"[APPPOS][POSITION IDENTIFICATION]",
		&posid);
	FINDFLDGETSTRING(posadda,"POSITION IDENTIFICATION",&lposid);
	if(RDAstrcmp(lposid,posid)) retval=FALSE;
		else retval=TRUE;
	if(posid!=NULL) Rfree(posid);
	if(lposid!=NULL) Rfree(lposid);
	return(retval);
}
void display_addendas(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	mainrsrc=RDArsrcNEW(MTNMASTER->module,"SHOW POSITION ADDENDAS");
	searchrsrc=RDArsrcNEW(MTNMASTER->module,"SHOW POSITION ADDENDAS BROWSE");
	definelist=RDArsrcNEW(MTNMASTER->module,"SHOW POSITION ADDENDAS LIST");
	ZERNRD(posmst);
	addDFincvir(searchrsrc,"POSTRK","POSADDA",NULL,posadda);
	addDFincvir(definelist,"POSTRK","POSADDA",NULL,posadda);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	ADVSbrowse(posadda,1,mainrsrc,searchrsrc,definelist,
		init_addenda,break_addenda,NULL,doexit_addenda,NULL,NULL,
		mtnrsrc,TRUE,NULL,2,2,2,0);
}
static void doexit_position(MakeBrowseList *blist)
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
}
static void okfunc_position(MakeBrowseList *b,void *targetkey)
{
	char *posid=NULL; 
	int y=0;
	RDArsrc *r;
	

	ZERNRD(posmst);
	if(ADVEQLNRDKEYsec(posmst,1,targetkey,SCRNvirtualSubData,b->mainrsrc))
	{
		KEYNRD(posmst,1);
	} else {
		if(posid!=NULL) { Rfree(posid); posid=NULL; }
		FINDFLDGETSTRING(posmst,"POSITION IDENTIFICATION",&posid);
	}
	updatefilerecord2rsrc(posmst,b->funcargs);
	FINDRSCSETSTRING(b->funcargs,"[APPPOS][POSITION IDENTIFICATION]",posid);
	updatersrc(b->funcargs,"[APPPOS][POSITION IDENTIFICATION]");
	if(posid!=NULL) Rfree(posid);
	y=FINDRSC(b->funcargs,"[APPCNT][POSITION IDENTIFICATION]");
	if(y!=(-1))
	{
		r=(RDArsrc *)b->funcargs;
		ExecuteRDArmemFunction(r->rscs+y);
	}
}
static short break_position(void)
{
	return(TRUE);
}
void browse_positions(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	mainrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT POSITION BROWSE");
	searchrsrc=RDArsrcNEW(MTNMASTER->module,"SELECT POSITION SEARCH BROWSE");
	definelist=RDArsrcNEW(MTNMASTER->module,"SELECT POSITION DEFINE LIST");
	ZERNRD(posmst);
	addDFincvir(searchrsrc,"POSTRK","POSMSTR",NULL,posmst);
	addDFincvir(definelist,"POSTRK","POSMSTR",NULL,posmst);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	ADVSbrowse(posmst,1,mainrsrc,searchrsrc,definelist,
		NULL,break_position,NULL,doexit_position,okfunc_position,NULL,
		mtnrsrc,TRUE,NULL,2,2,2,0);
}
static void getsupportingrecords(RDArsrc *mtnrsrc,short update)
{

	COPYFIELD(apppos,posmst,"POSITION IDENTIFICATION");
	if(ADVEQLNRDsec(posmst,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(posmst,1);
	COPYFIELD(apppos,appcat,"APPLICANT IDENTIFICATION");
	COPYFIELD(apppos,appcat,"APPLICATION NUMBER");
	if(ADVEQLNRDsec(appcat,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(appcat,1);
	if(update)
	{
		updatefilerecord2rsrc(posmst,mtnrsrc);
		updatefilerecord2rsrc(appcat,mtnrsrc);
	} else {
		filerecord2rsrc(posmst,mtnrsrc);
		filerecord2rsrc(appcat,mtnrsrc);
	}
}
static void setfiles(RDArsrc *parent)
{
	readallwidgets(parent);
	rsrc2filerecord(apppos,parent);
	getsupportingrecords(parent,TRUE);
	updateSCRNvirtuals(parent);
}
static void readscreen(RDArsrc *mtnrsrc)
{
	ZERNRD(apppos);
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	rsrc2filerecord(apppos,mtnrsrc);
}
static void posrespfunc(RDArsrc *parent)
{
	readallwidgets(parent);
	rsrc2filerecord(apppos,parent);
	MTNSCRN_EXECUTEPROGRAM("posresp",parent,"POSPRMS",
		apppos,TRUE,NULL);
}
void add_positions(RDArsrc *mtnrsrc)
{
	char *name=NULL,*filename=NULL;
	NRDkey *keys=NULL;
	NRDfield *fields=NULL,*fldx=NULL;
	short nokeys=0,nofields=0,x=0,edit_rsrc=TRUE;
	char *appid=NULL;

	apppos=OPNNRDsec(MTNMASTER->module,"APPPOS",TRUE,TRUE,FALSE);
	appcat=OPNNRDsec(MTNMASTER->module,"APPCAT",TRUE,TRUE,FALSE);
	posmst=OPNNRDsec("POSTRK","POSMSTR",TRUE,FALSE,FALSE);
	posadda=OPNNRDsec("POSTRK","POSADDA",TRUE,FALSE,FALSE);
	if(apppos!=(-1) && appcat!=(-1) && posmst!=(-1))
	{
		ZERNRD(apppos);
		FINDFLDGETSTRING(MTNMASTER->passkey->fileno,"APPLICANT IDENTIFICATION",&appid);
		FINDFLDSETSTRING(apppos,"APPLICANT IDENTIFICATION",appid);
		FINDFLDSETSTRING(appcat,"APPLICANT IDENTIFICATION",appid);
		POSrsrc=RDArsrcNEW(MTNMASTER->module,"ADD APPLICANTS POSITIONS");
		nokeys=NUMKEYS(apppos);
		keys=KEYPOINTER(apppos);
		nofields=NUMFLDS(apppos);
		fields=FLDPOINTER(apppos);
		POSdefinelist=RDArsrcNEW(MTNMASTER->module,"ADD APPLICANTS POSITIONS DEFINE LIST");
		addDFincvir(POSrsrc,MTNMASTER->module,"APPCNT",NULL,MTNMASTER->passkey->fileno);
		addDFincvir(POSrsrc,MTNMASTER->module,"APPPOS",NULL,apppos);
		addDFincvir(POSrsrc,MTNMASTER->module,"APPCAT",NULL,appcat);
		addDFincvir(POSrsrc,"POSTRK","POSMSTR",NULL,posmst);
		addDFincvir(POSdefinelist,MTNMASTER->module,"APPCNT",NULL,MTNMASTER->passkey->fileno);
		addDFincvir(POSdefinelist,MTNMASTER->module,"APPPOS",NULL,apppos);
		addDFincvir(POSdefinelist,MTNMASTER->module,"APPCAT",NULL,appcat);
		addDFincvir(POSdefinelist,"POSTRK","POSMSTR",NULL,posmst);
		GET_SCREEN_VIRTUALS(POSrsrc,0);
		GET_SCREEN_VIRTUALS(POSdefinelist,1);
		if(fields!=NULL)
		{
			filename=FILENAME(apppos);
			for(x=0,fldx=fields;x<nofields;++x,++fldx)
			{
				if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
				{
				} else if(!RDAstrcmp(fldx->name,"APPLICANT IDENTIFICATION"))
				{
					edit_rsrc=FALSE;
					nonlstmakefld(POSrsrc,apppos,fldx,edit_rsrc);
				} else {
					edit_rsrc=TRUE;
					nonlstmakefld(POSrsrc,apppos,fldx,edit_rsrc);
					if(name!=NULL)
					{
						name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(filename));
					} else {
						name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(filename));
					}
					sprintf(name,"[%s][%s]",filename,fldx->name);
					FINDRSCSETFUNC(POSrsrc,name,setfiles,NULL);
				}
			}
		}
		if(name!=NULL) Rfree(name);
		FINDRSCSETSTRING(POSrsrc,"[APPPOS][APPLICANT IDENTIFICATION]",appid);
		file2rsrc(appcat,POSrsrc,FALSE);
		file2rsrc(MTNMASTER->passkey->fileno,POSrsrc,FALSE);
		file2rsrc(posmst,POSrsrc,FALSE);
		SCRNvirtual2rsrc(POSrsrc);
		filerecord2rsrc(MTNMASTER->passkey->fileno,POSrsrc);
		if(PREVPOS!=NULL) freeapplib(PREVPOS);
		PREVPOS=APPlibNEW();
		addAPPlib(PREVPOS,"No References added this session");
		addlstrsrc(POSrsrc,"PREVIOUS POSITIONS",&x,TRUE,NULL,PREVPOS->numlibs,
			&PREVPOS->libs,NULL);
		mk_define_list(POSdefinelist,nofields,fields,nokeys,keys,NULL,1);
		FINDRSCSETFUNC(POSdefinelist,"SELECT",savelistcb,POSrsrc);
		DefaultScreens(POSdefinelist);
		savelist(POSdefinelist,POSrsrc);
		SetVirFieldFuncs(POSrsrc);
		addbtnrsrc(POSrsrc,"RESPONSIBILITIES",TRUE,posrespfunc,NULL);
		addbtnrsrc(POSrsrc,"DISPLAY ADDENDA",TRUE,display_addendas,NULL);
		addbtnrsrc(POSrsrc,"BROWSE POSITIONS",TRUE,browse_positions,NULL);
		addbtnrsrc(POSrsrc,"DEFINE LIST",TRUE,POSdefinelistfunc,NULL);
		addbtnrsrc(POSrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
		addbtnrsrc(POSrsrc,"RESET DEFAULTS",TRUE,reset_defaults,NULL);
		addrfcbrsrc(POSrsrc,"SAVE",TRUE,save_check,NULL);
		addrfkwrsrc(POSrsrc,"QUIT",TRUE,quit_record,NULL);
		addrfcbrsrc(POSrsrc,"HELP",TRUE,screenhelp,NULL);
		addrfcbrsrc(POSrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		set_defaults(POSrsrc);
		APPmakescrn(POSrsrc,TRUE,quit_record,NULL,TRUE);
	} else {
		if(apppos!=(-1)) CLSNRD(apppos);
		if(posmst!=(-1)) CLSNRD(posmst);
		if(posadda!=(-1)) CLSNRD(posadda);
		if(appcat!=(-1)) CLSNRD(appcat);
		return;
	}
}
