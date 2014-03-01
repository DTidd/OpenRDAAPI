/* puraline.c - Add Purchase Order Lines */
#include <cstdio>

#include <mix.hpp>
#define DIAGNOSTIC_SCREENS

void addlines(void);
extern short linenum,vennum;
extern short catnum,ctlgnum,cvennum,cvpnum;
extern short fixmst;
extern int yearid;
extern char *POID,*module;
double po_total=0.0;
static RDArsrc *definelist=NULL;
static APPlib *prevline=NULL;
static APPlib *types=NULL;
APPlib *categorylist=NULL;
APPlib *itemlist=NULL;
char *vendor_id=NULL,*category_id=NULL;
static void savelist(RDArsrc *,RDArsrc *),savelistcb(RDArsrc *,RDArsrc *);
static void definelistfunc(RDArsrc *),makedflist(void);
static void save_check(RDArsrc *);
static void save_record(RDArsrc *),quit_record(RDArsrc *);
static void set_def_func(RDArsrc *);
static void set_def_callback(RDArsrc *);
static void caltotal(RDArsrc *);
static void getsupportingrecords(RDArsrc *);
static short cvpkeyno=(-1);
static void readscreen(RDArsrc *);
static void SETFILES(RDArsrc *);
extern MaintainMaster *PURMST_MTNMASTER;

static void savelistcb(RDArsrc *definelist,RDArsrc *parent)
{
	savelist(definelist,parent);
	if(IsScreenDisplayed(definelist)) NULL_RSCS(definelist);
}
static void savelist(RDArsrc *definelist,RDArsrc *parent)
{
	short ef=0;
	char *username=NULL,deleteflag=FALSE/*,verified=FALSE*/;
	char *poid1=NULL;
	int yearid1=0;
	RDArsrc *tmprsrc=NULL;

	po_total=0.0;
	if(IsScreenDisplayed(definelist)) readallwidgets(definelist);
	if(prevline!=NULL) { freeapplib(prevline); prevline=NULL; }
	prevline=APPlibNEW();
#ifdef DIAGNOSTIC_SCREENS
	tmprsrc=diagscrn(linenum,"DIAGNOSTIC SCREEN",module,
		"Reading Undistributed Lines",NULL);
#else 
	tmprsrc=NULL;
#endif
	if(tmprsrc!=NULL)
	{
		if(ADVmakescrn(tmprsrc,TRUE))
		{
 			WARNINGDIALOG("WARNING DIALOG SCREEN","MAKE SCREEN FAILED.","The Make Screen function failed for the screen DIAGNOSTIC SCREEN.  Check to see that this screen is available in this module.  If not, call your installer.\n",NULL,NULL,TRUE,NULL);
			prterr("Error Cannot Create Screen [%s] [%s].",tmprsrc->module,tmprsrc->screen);
		} else ForceWindowUpdate(tmprsrc);
	}
	ZERNRD(linenum);
	FINDFLDSETINT(linenum,"FISCAL YEAR",yearid);
	FINDFLDSETSTRING(linenum,"PO IDENTIFICATION",POID);
	ef=ADVGTENRDsec(linenum,1,SCRNvirtualSubData,parent);
	while(!ef)
	{
		FINDFLDGETINT(linenum,"FISCAL YEAR",&yearid1);
		if(poid1!=NULL) Rfree(poid1); 
		FINDFLDGETSTRING(linenum,"PO IDENTIFICATION",&poid1);
		if(yearid1!=yearid || RDAstrcmp(poid1,POID)) break;
		FINDFLDGETCHAR(linenum,"DELETEFLAG",&deleteflag);
		if(!deleteflag)
		{
			FINDFLDGETSTRING(linenum,"SOURCE USER",&username);
			if(!RDAstrcmp(username,USERLOGIN))
			{
				makedflist();
				if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
			} else {
				if(tmprsrc!=NULL) update_diagnostic(tmprsrc,FALSE);
			}
		}
		ef=ADVNXTNRDsec(linenum,1,SCRNvirtualSubData,parent);
	}
	if(poid1!=NULL) Rfree(poid1);
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
		tmprsrc=NULL;
	}
	if(prevline->numlibs<1)
	{
		addAPPlib(prevline,"No Lines added this session");
	}
	if(!FINDRSCLISTAPPlib(parent,"PREVIOUS LINES",(prevline->numlibs-1),
		prevline))
	{
		updatersrc(parent,"PREVIOUS LINES");
	}
}
static void makedflist()
{
	short x,cur=1,length=0,lcount=0;
	RDArmem *len_member,*pos_member;
	char *s,*hold=NULL,*temp=NULL;
	NRDfield *field;

	if(prevline!=NULL)
	{
		if(prevline->numlibs==1 && !RDAstrcmp(prevline->libs[0],
			"No Lines added this session"))
		{
			freeapplib(prevline);
			prevline=APPlibNEW();
		}
	} else {
		prevline=APPlibNEW();
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
			if(*pos_member->value.short_value>0 && 
				*len_member->value.short_value>0)
			{
				if(*pos_member->value.short_value==cur)
				{
					s=stralloc(pos_member->rscrname);
					s[RDAstrlen(s)-9]=0;
					field=FLDNRD(linenum,s);
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
								if(!RDAstrcmp(field->name,"TOTAL")) po_total+=*field->data.float_value;
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
					}
					if(s!=NULL) Rfree(s);
				}
			}
		}
		++cur;
	}
	addAPPlib(prevline,hold);
	if(hold!=NULL) { Rfree(hold); hold=NULL; }
}
static void definelistfunc(RDArsrc *parent)
{
	updateSCRNvirtuals(definelist);
	APPmakescrn(definelist,TRUE,NULL,NULL,FALSE);
}
static void set_def_func(RDArsrc *parent)
{
	int selprev=0;

	FINDRSCGETINT(parent,"PREVIOUS LINES",&selprev);
	DefaultResetScreen(parent,TRUE);
	FINDRSCSETSTRING(parent,"[PURLIN][PO IDENTIFICATION]",POID);
	FINDFLDSETSTRING(linenum,"PO IDENTIFICATION",POID);
	FINDRSCSETINT(parent,"[PURLIN][FISCAL YEAR]",yearid);
	FINDRSCSETSTRING(parent,"[PURMST][VENDOR IDENTIFICATION]",vendor_id);
	FINDFLDSETSTRING(PURMST_MTNMASTER->passkey->fileno,"VENDOR IDENTIFICATION",vendor_id);
	FINDFLDSETINT(linenum,"FISCAL YEAR",yearid);
	FINDRSCSETDOUBLE(parent,"PO TOTAL",po_total);
	FINDRSCSETINT(parent,"PREVIOUS LINES",selprev);
}
static void set_def_callback(RDArsrc *parent)
{
	set_def_func(parent);
	getsupportingrecords(parent);
	computeallSCRNvirtuals(parent);
	updateallrsrc(parent);
	readscreen(parent);
}
/*
static void gettypes(RDArsrc *parent)
{
	if(types!=NULL) freeapplib(types);
	types=APPlibNEW();
	addAPPlib(types,"Not Applicable");
	addAPPlib(types,"Fixed Assets");
	addAPPlib(types,"Inventory Control");
}
*/
static void getsupportingrecords(RDArsrc *parent)
{
	if(PURMST_MTNMASTER->passkey->fileno!=(-1))
	{
		FINDFLDSETINT(PURMST_MTNMASTER->passkey->fileno,"FISCAL YEAR",yearid);
		FINDFLDSETSTRING(PURMST_MTNMASTER->passkey->fileno,"PO IDENTIFICATION",POID);
		if(ADVEQLNRDsec(PURMST_MTNMASTER->passkey->fileno,1,SCRNvirtualSubData,parent)) KEYNRD(PURMST_MTNMASTER->passkey->fileno,1);
		filerecord2rsrc(PURMST_MTNMASTER->passkey->fileno,parent);
	}
	if(vennum!=(-1))
	{
		COPYFIELD(PURMST_MTNMASTER->passkey->fileno,vennum,"VENDOR IDENTIFICATION");
		if(ADVEQLNRDsec(vennum,1,SCRNvirtualSubData,parent)) KEYNRD(vennum,1);
		filerecord2rsrc(vennum,parent);
	}
}
static void readscreen(RDArsrc *parent)
{
        ZERNRD(linenum);
        readallwidgets(parent);
        ReadRDAScrolledLists(parent);
        rsrc2filerecord(linenum,parent);
}
static void save_check(RDArsrc *parent)
{
/*
	char closed=FALSE,*message=NULL;
*/

	readscreen(parent);
/*
	FINDFLDGETCHAR(PURMST_MTNMASTER->passkey->fileno,"CLOSED",&closed);
	if(closed==TRUE)
	{
		message=Rmalloc(RDAstrlen(POID)+4+100);
		sprintf(message,"Lines cannot be added to the CLOSED Purchase Order [%s] Year [%4d]",(POID==NULL?"":POID),yearid);
		WARNINGDIALOG("WARNING DIALOG SCREEN","PURCHASE ORDER CLOSED!",message,NULL,NULL,FALSE,NULL);
		if(message!=NULL) Rfree(message);
		return;
	} 
*/
	save_record(parent);
}
static void save_record(RDArsrc *parent)
{
	char *rcddesc=NULL,deleteflag=FALSE/*,*temppoid=NULL*/;
	char *catid=NULL;
	char *date=NULL,*timex=NULL,delflag=FALSE;
	int /*tempyrid=(-1),seltypes=0, */type=0,fixqty=0,qty=0;
	short x=0;
	int lf=0;
	RDArmem *member=NULL;
	RDATData *prev=NULL;

/*
	FINDFLDGETINT(linenum,"FISCAL YEAR",&tempyrid);
	FINDFLDGETSTRING(linenum,"PO IDENTIFICATION",&temppoid);
	rsrc2filerecord(linenum,parent);
        FINDRSCGETINT(parent,"TYPES",&seltypes);
        FINDFLDSETINT(linenum,"TYPE",seltypes);
	FINDFLDGETINT(linenum,"FISCAL YEAR",&tempyrid);
	if(temppoid!=NULL) Rfree(temppoid);
	FINDFLDGETSTRING(linenum,"PO IDENTIFICATION",&temppoid);
*/
	FINDFLDSETSTRING(linenum,"SOURCE USER",USERLOGIN);
	FINDFLDSETCHAR(linenum,"DELETEFLAG",deleteflag);
	FINDFLDSETCHAR(linenum,"SOURCE USER VERIFIED",deleteflag);
	date=GETCURRENTDATE();
	timex=GETCURRENTTIME();
	if((x=FINDRSC(parent,"[PURLIN][ENTRY DATE]"))!=(-1))
	{
		member=parent->rscs+x;
		if(!IsWidgetDisplayed(member) || isEMPTY(member->value.string_value))
		{
			lf=RDAstrlen(date);
			if(date!=NULL && lf>0)
			{
				QUICKALLOC(member->value.string_value,member->dlen,lf);
				memcpy(member->value.string_value,date,lf+1);
			} else if(member->dlen>0)
			{
				memset(member->value.string_value,0,member->dlen+1);
			} else {
				member->value.string_value=NULL;
				member->dlen=0;
			}
			FINDFLDSETSTRING(linenum,"ENTRY DATE",date);
		}
	} else {
		FINDFLDSETSTRING(linenum,"ENTRY DATE",date);
	}
	if(date!=NULL) Rfree(date); 
	if((x=FINDRSC(parent,"[PURLIN][ENTRY TIME]"))!=(-1))
	{
		member=parent->rscs+x;
		if(!IsWidgetDisplayed(member) || isEMPTY(member->value.string_value))
		{
			lf=RDAstrlen(timex);
			if(timex!=NULL && lf>0)
			{
				QUICKALLOC(member->value.string_value,member->dlen,lf);
				memcpy(member->value.string_value,timex,lf+1);
			} else if(member->dlen>0)
			{
				memset(member->value.string_value,0,member->dlen+1);
			} else {
				member->value.string_value=NULL;
				member->dlen=0;
			}
			FINDFLDSETSTRING(linenum,"ENTRY TIME",timex);
		}
	} else {
		FINDFLDSETSTRING(linenum,"ENTRY TIME",timex);
	}
	if(timex!=NULL) { Rfree(timex); timex=NULL; }
	if(ADVRECORDsec(linenum,SCRNvirtualSubData,parent))
	{
		rcddesc=ADVRECORDsecDesc(linenum,SCRNvirtualSubData,parent);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("SECURITY ACCESS DENIED!",rcddesc,NULL,FALSE);
			Rfree(rcddesc);
			prterr("Error didn't write record: [%s]",rcddesc);
		} else {
			prterr("Error didn't write record!  ADVRECORDsec() failed without a description returned!");
		}
	} else {
		if(!WRTTRANS(linenum,1,"LINE NUMBER",NULL))
		{
			FINDFLDGETINT(linenum,"TYPE",&type);
			if(type==1) /* Fixed Asset Type */
			{
				if(fixmst!=(-1))
				{
					LOCNRDFILE(fixmst);
					FINDFLDGETSTRING(linenum,"CATEGORY IDENTIFICATION",&catid);
					FINDFLDGETINT(linenum,"QUANTITY",&qty);
					if(ADVEQLNRDsec(fixmst,1,SCRNvirtualSubData,parent))
					{
						KEYNRD(fixmst,1);
						FINDFLDSETSTRING(fixmst,"CATEGORY IDENTIFICATION",catid);
						COPYFIELD(linenum,fixmst,"DESCRIPTION");
						COPYFIELD(linenum,fixmst,"COMMENTS");
						FINDFLDSETINT(fixmst,"QUANTITY",0);
						FINDFLDSETINT(fixmst,"UNRECEIVED QUANTITY",qty);
					} else {
						prev=RDATDataNEW(fixmst);
						FINDFLDGETCHAR(fixmst,"DELETEFLAG",&delflag);
						if(!delflag)
						{
							FINDFLDGETINT(fixmst,"UNRECEIVED QUANTITY",&fixqty);
							fixqty+=qty;
							FINDFLDSETINT(fixmst,"UNRECEIVED QUANTITY",fixqty);
						} else {
							KEYNRD(fixmst,1);
							FINDFLDSETCHAR(fixmst,"DELETEFLAG",FALSE);
							COPYFIELD(linenum,fixmst,"CATEGORY IDENTIFICATION");
							COPYFIELD(linenum,fixmst,"DESCRIPTION");
							COPYFIELD(linenum,fixmst,"COMMENTS");
							FINDFLDSETINT(fixmst,"QUANTITY",0);
							FINDFLDSETINT(fixmst,"UNRECEIVED QUANTITY",qty);
						}
					}
					ADVWRTTRANSsec(fixmst,0,NULL,prev,SCRNvirtualSubData,parent);
					UNLNRDFILE(fixmst);
					if(prev!=NULL) Rfree(prev);
				}
			}
			makedflist();
		} else {
			ERRORDIALOG("WRITE ERROR OCCURRED!","The write function failed on write attempt to [PURORD][PURLIN], the record you attempted to save was not created. Please try again.",NULL,FALSE);
		}
	}
	if(!FINDRSCLISTAPPlib(parent,"PREVIOUS LINES",(prevline->numlibs-1),
		prevline))
	{
		updatersrc(parent,"PREVIOUS LINES");
	}
	FINDRSCSETDOUBLE(parent,"PO TOTAL",po_total);
	updatersrc(parent,"PO TOTAL");
}
static void caltotal(RDArsrc *parent)
{
	int quantity=0;
	double unitrate=0.0,total=0.0;

	readwidget(parent,"[PURLIN][QUANTITY]");
	if(FINDRSCGETINT(parent,"[PURLIN][QUANTITY]",&quantity)) return;
	readwidget(parent,"[PURLIN][UNIT RATE]");
	if(FINDRSCGETDOUBLE(parent,"[PURLIN][UNIT RATE]",&unitrate)) return;
	total=(unitrate*quantity)*100.0;
	FINDRSCSETDOUBLE(parent,"[PURLIN][TOTAL]",total);
	updatersrc(parent,"[PURLIN][TOTAL]");
	updateSCRNvirtuals(parent);
}
static void getsupportingcatrecords(RDArsrc *lcatrsrc)
{
	int selected=0,quantity=0;
	char *catid=NULL,*venid=NULL,*itemid=NULL;

	FINDRSCGETSTRING(lcatrsrc,"VENDOR IDENTIFICATION",&venid);
	FINDRSCGETINT(lcatrsrc,"CATEGORY IDENTIFICATIONS",&selected);
	if(RDAstrcmp(categorylist->libs[selected],"No Categories Defined"))
	{
		catid=stralloc(categorylist->libs[selected]);
	}
	FINDRSCGETINT(lcatrsrc,"ITEM IDENTIFICATIONS",&selected);
	if(RDAstrcmp(itemlist->libs[selected],"No Catalogue Vendor Items Defined"))
	{
		itemid=stralloc(itemlist->libs[selected]);
	}
	FINDRSCGETINT(lcatrsrc,"QUANTITY",&quantity);
	if(catnum!=(-1))
	{
		FINDFLDSETSTRING(catnum,"CATEGORY IDENTIFICATION",catid);
		if(ADVEQLNRDsec(catnum,1,SCRNvirtualSubData,lcatrsrc)) KEYNRD(catnum,1);
		updatefilerecord2rsrc(catnum,lcatrsrc);
	}
	if(ctlgnum!=(-1))
	{
		FINDFLDSETSTRING(ctlgnum,"CATEGORY IDENTIFICATION",catid);
		FINDFLDSETSTRING(ctlgnum,"ITEM IDENTIFICATION",itemid);
		if(ADVEQLNRDsec(ctlgnum,1,SCRNvirtualSubData,lcatrsrc)) KEYNRD(ctlgnum,1);
		updatefilerecord2rsrc(ctlgnum,lcatrsrc);
	}
	if(cvennum!=(-1))
	{
		FINDFLDSETSTRING(cvennum,"VENDOR IDENTIFICATION",venid);
		FINDFLDSETSTRING(cvennum,"CATEGORY IDENTIFICATION",catid);
		FINDFLDSETSTRING(cvennum,"ITEM IDENTIFICATION",itemid);
		if(ADVEQLNRDsec(cvennum,1,SCRNvirtualSubData,lcatrsrc)) KEYNRD(cvennum,1);
		updatefilerecord2rsrc(cvennum,lcatrsrc);
	}
	if(cvpnum!=(-1))
	{
		FINDFLDSETSTRING(cvpnum,"CATEGORY IDENTIFICATION",catid);
		FINDFLDSETSTRING(cvpnum,"ITEM IDENTIFICATION",itemid);
		FINDFLDSETSTRING(cvpnum,"VENDOR IDENTIFICATION",venid);
		FINDFLDSETINT(cvpnum,"QUANTITY",quantity);
		if(ADVEQLNRDsec(cvpnum,1,SCRNvirtualSubData,lcatrsrc)) KEYNRD(cvpnum,1);
		updatefilerecord2rsrc(cvpnum,lcatrsrc);
	}
	if(vennum!=(-1))
	{
		FINDFLDSETSTRING(vennum,"VENDOR IDENTIFICATION",venid);
		if(ADVEQLNRDsec(vennum,1,SCRNvirtualSubData,lcatrsrc)) KEYNRD(vennum,1);
		updatefilerecord2rsrc(vennum,lcatrsrc);
	}
	if(catid!=NULL) Rfree(catid);
	if(venid!=NULL) Rfree(venid);
	if(itemid!=NULL) Rfree(itemid);
}
static void getquantitycb(RDArsrc *lcatrsrc)
{
	int selected=0;
	char *catid=NULL,*venid=NULL,*itemid=NULL;
	char *catid1=NULL,*venid1=NULL,*itemid1=NULL;
	int quantity=0,quantity1=0;
	double unit_rate=0.0;
	char delflag=TRUE,active=FALSE;

	readwidget(lcatrsrc,"VENDOR IDENTIFICATION");
	readwidget(lcatrsrc,"QUANTITY");
	FINDRSCGETSTRING(lcatrsrc,"VENDOR IDENTIFICATION",&venid);
	FINDRSCGETINT(lcatrsrc,"CATEGORY IDENTIFICATIONS",&selected);
	if(RDAstrcmp(categorylist->libs[selected],"No Categories Defined"))
	{ 
		catid=stralloc(categorylist->libs[selected]);
	}
	FINDRSCGETINT(lcatrsrc,"ITEM IDENTIFICATIONS",&selected);
	if(RDAstrcmp(itemlist->libs[selected],"No Catalogue Vendor Items Defined"))
	{ 
		itemid=stralloc(itemlist->libs[selected]);
	}
	FINDRSCGETINT(lcatrsrc,"QUANTITY",&quantity);
	ZERNRD(cvpnum);
	FINDFLDSETSTRING(cvpnum,"VENDOR IDENTIFICATION",venid);
	FINDFLDSETSTRING(cvpnum,"CATEGORY IDENTIFICATION",catid);
	FINDFLDSETSTRING(cvpnum,"ITEM IDENTIFICATION",itemid);
	FINDFLDSETINT(cvpnum,"QUANTITY",quantity);
	if(!ADVLTENRDsec(cvpnum,1,SCRNvirtualSubData,lcatrsrc))
	{
		FINDFLDGETSTRING(cvpnum,"VENDOR IDENTIFICATION",&venid1);
		FINDFLDGETSTRING(cvpnum,"CATEGORY IDENTIFICATION",&catid1);
		FINDFLDGETSTRING(cvpnum,"ITEM IDENTIFICATION",&itemid1);
		FINDFLDGETINT(cvpnum,"QUANTITY",&quantity1);
		if(!RDAstrcmp(catid,catid1) && !RDAstrcmp(itemid,itemid1) &&
			!RDAstrcmp(venid,venid1) && quantity1<=quantity) 
		{
			FINDFLDGETCHAR(cvpnum,"DELETEFLAG",&delflag);
			if(!delflag)
			{
			FINDFLDSETSTRING(cvennum,"VENDOR IDENTIFICATION",venid1);
			FINDFLDSETSTRING(cvennum,"CATEGORY IDENTIFICATION",catid1);
			FINDFLDSETSTRING(cvennum,"ITEM IDENTIFICATION",itemid1);
			if(!ADVEQLNRDsec(cvennum,1,SCRNvirtualSubData,lcatrsrc))
			{
				FINDFLDGETCHAR(cvennum,"DELETEFLAG",&delflag);
				FINDFLDGETCHAR(cvennum,"ACTIVE",&active);
/* Might have to check the valid to date against the system date to test if
the CTLGVEN file is valid or out of date */
				if(active && !delflag)
				{
				FINDFLDSETSTRING(ctlgnum,"CATEGORY IDENTIFICATION",catid1);
				FINDFLDSETSTRING(ctlgnum,"ITEM IDENTIFICATION",itemid1);
				if(!ADVEQLNRDsec(ctlgnum,1,SCRNvirtualSubData,lcatrsrc))
				{
				FINDFLDGETCHAR(ctlgnum,"DELETEFLAG",&delflag);
				FINDFLDGETCHAR(ctlgnum,"ACTIVE",&active);
				if(active && !delflag)
				{
					FINDFLDGETDOUBLE(cvpnum,"UNIT RATE",&unit_rate);
				}
				}
				}
			}
			}
		}
	}
	if(catid!=NULL) Rfree(catid);
	if(venid!=NULL) Rfree(venid);
	if(itemid!=NULL) Rfree(itemid);
	if(catid1!=NULL) Rfree(catid1);
	if(venid1!=NULL) Rfree(venid1);
	if(itemid1!=NULL) Rfree(itemid1);
	FINDRSCSETDOUBLE(lcatrsrc,"UNIT RATE",unit_rate);
	updatersrc(lcatrsrc,"UNIT RATE");
	getsupportingcatrecords(lcatrsrc);
	updateSCRNvirtuals(lcatrsrc);
}
static void getprevquantitycb(RDArsrc *lcatrsrc)
{
	int selected=0;
	char *catid=NULL,*venid=NULL,*itemid=NULL;
	char *catid1=NULL,*venid1=NULL,*itemid1=NULL;
	int quantity=0,quantity1=0;
	double unit_rate=0.0;
	char delflag=TRUE,active=FALSE;

	readwidget(lcatrsrc,"VENDOR IDENTIFICATION");
	readwidget(lcatrsrc,"QUANTITY");
	FINDRSCGETSTRING(lcatrsrc,"VENDOR IDENTIFICATION",&venid);
	FINDRSCGETINT(lcatrsrc,"CATEGORY IDENTIFICATIONS",&selected);
	if(RDAstrcmp(categorylist->libs[selected],"No Categories Defined"))
	{ 
		catid=stralloc(categorylist->libs[selected]);
	}
	FINDRSCGETINT(lcatrsrc,"ITEM IDENTIFICATIONS",&selected);
	if(RDAstrcmp(itemlist->libs[selected],"No Catalogue Vendor Items Defined"))
	{ 
		itemid=stralloc(itemlist->libs[selected]);
	}
	FINDRSCGETINT(lcatrsrc,"QUANTITY",&quantity);
	FINDRSCGETDOUBLE(lcatrsrc,"UNIT RATE",&unit_rate);
	ZERNRD(cvpnum);
	FINDFLDSETSTRING(cvpnum,"VENDOR IDENTIFICATION",venid);
	FINDFLDSETSTRING(cvpnum,"CATEGORY IDENTIFICATION",catid);
	FINDFLDSETSTRING(cvpnum,"ITEM IDENTIFICATION",itemid);
	FINDFLDSETINT(cvpnum,"QUANTITY",quantity);
	if(ADVEQLNRDsec(cvpnum,cvpkeyno,SCRNvirtualSubData,lcatrsrc))
	{
		KEYNRD(cvpnum,cvpkeyno);
		if(ADVLTENRDsec(cvpnum,cvpkeyno,SCRNvirtualSubData,lcatrsrc))
		{
			KEYNRD(cvpnum,cvpkeyno);
			if(catid!=NULL) Rfree(catid);
			if(venid!=NULL) Rfree(venid);
			if(itemid!=NULL) Rfree(itemid);
			return;
		}  
	} else {
                if(ADVPRVNRDsec(cvpnum,cvpkeyno,SCRNvirtualSubData,lcatrsrc))
                {
                        KEYNRD(cvpnum,cvpkeyno);
			if(catid!=NULL) Rfree(catid);
			if(venid!=NULL) Rfree(venid);
			if(itemid!=NULL) Rfree(itemid);
			return;
		}

	}
	FINDFLDGETSTRING(cvpnum,"VENDOR IDENTIFICATION",&venid1);
	FINDFLDGETSTRING(cvpnum,"CATEGORY IDENTIFICATION",&catid1);
	FINDFLDGETSTRING(cvpnum,"ITEM IDENTIFICATION",&itemid1);
	FINDFLDGETINT(cvpnum,"QUANTITY",&quantity1);
	if(!RDAstrcmp(catid,catid1) && !RDAstrcmp(itemid,itemid1) &&
		!RDAstrcmp(venid,venid1) && quantity1<quantity) 
	{
		FINDFLDGETCHAR(cvpnum,"DELETEFLAG",&delflag);
		if(!delflag)
		{
		FINDFLDSETSTRING(cvennum,"VENDOR IDENTIFICATION",venid1);
		FINDFLDSETSTRING(cvennum,"CATEGORY IDENTIFICATION",catid1);
		FINDFLDSETSTRING(cvennum,"ITEM IDENTIFICATION",itemid1);
		if(!ADVEQLNRDsec(cvennum,1,SCRNvirtualSubData,lcatrsrc))
		{
			FINDFLDGETCHAR(cvennum,"DELETEFLAG",&delflag);
			FINDFLDGETCHAR(cvennum,"ACTIVE",&active);
/* Might have to check the valid to date against the system date to test if the CTLGVEN file is valid or out of date */
			if(active && !delflag)
			{
			FINDFLDSETSTRING(ctlgnum,"CATEGORY IDENTIFICATION",catid1);
			FINDFLDSETSTRING(ctlgnum,"ITEM IDENTIFICATION",itemid1);
			if(!ADVEQLNRDsec(ctlgnum,1,SCRNvirtualSubData,lcatrsrc))
			{
			FINDFLDGETCHAR(ctlgnum,"DELETEFLAG",&delflag);
			FINDFLDGETCHAR(ctlgnum,"ACTIVE",&active);
			if(active && !delflag)
			{
				FINDFLDGETDOUBLE(cvpnum,"UNIT RATE",&unit_rate);
			}
			}
			}
		}
		}
		FINDRSCSETINT(lcatrsrc,"QUANTITY",quantity1);
		FINDRSCSETDOUBLE(lcatrsrc,"UNIT RATE",unit_rate);
		updatersrc(lcatrsrc,"QUANTITY");
		updatersrc(lcatrsrc,"UNIT RATE");
		getsupportingcatrecords(lcatrsrc);
	}
	if(catid!=NULL) Rfree(catid);
	if(venid!=NULL) Rfree(venid);
	if(itemid!=NULL) Rfree(itemid);
	if(catid1!=NULL) Rfree(catid1);
	if(venid1!=NULL) Rfree(venid1);
	if(itemid1!=NULL) Rfree(itemid1);
	updateSCRNvirtuals(lcatrsrc);
}
static void getnextquantitycb(RDArsrc *lcatrsrc)
{
	int selected=0;
	char *catid=NULL,*venid=NULL,*itemid=NULL;
	char *catid1=NULL,*venid1=NULL,*itemid1=NULL;
	int quantity=0,quantity1=0;
	double unit_rate=0.0;
	char delflag=TRUE,active=FALSE;

	readwidget(lcatrsrc,"VENDOR IDENTIFICATION");
	readwidget(lcatrsrc,"QUANTITY");
	FINDRSCGETSTRING(lcatrsrc,"VENDOR IDENTIFICATION",&venid);
	FINDRSCGETINT(lcatrsrc,"CATEGORY IDENTIFICATIONS",&selected);
	if(RDAstrcmp(categorylist->libs[selected],"No Categories Defined"))
	{ 
		catid=stralloc(categorylist->libs[selected]);
	}
	FINDRSCGETINT(lcatrsrc,"ITEM IDENTIFICATIONS",&selected);
	if(RDAstrcmp(itemlist->libs[selected],"No Catalogue Vendor Items Defined"))
	{ 
		itemid=stralloc(itemlist->libs[selected]);
	}
	FINDRSCGETINT(lcatrsrc,"QUANTITY",&quantity);
	FINDRSCGETDOUBLE(lcatrsrc,"UNIT RATE",&unit_rate);
	ZERNRD(cvpnum);
	FINDFLDSETSTRING(cvpnum,"VENDOR IDENTIFICATION",venid);
	FINDFLDSETSTRING(cvpnum,"CATEGORY IDENTIFICATION",catid);
	FINDFLDSETSTRING(cvpnum,"ITEM IDENTIFICATION",itemid);
	FINDFLDSETINT(cvpnum,"QUANTITY",quantity);
	if(ADVEQLNRDsec(cvpnum,cvpkeyno,SCRNvirtualSubData,lcatrsrc))
	{
		KEYNRD(cvpnum,cvpkeyno);
		if(ADVGTENRDsec(cvpnum,cvpkeyno,SCRNvirtualSubData,lcatrsrc))
		{
			KEYNRD(cvpnum,cvpkeyno);
			if(catid!=NULL) Rfree(catid);
			if(venid!=NULL) Rfree(venid);
			if(itemid!=NULL) Rfree(itemid);
			return;
		}  
	} else {
                if(ADVNXTNRDsec(cvpnum,cvpkeyno,SCRNvirtualSubData,lcatrsrc))
                {
                        KEYNRD(cvpnum,cvpkeyno);
			if(catid!=NULL) Rfree(catid);
			if(venid!=NULL) Rfree(venid);
			if(itemid!=NULL) Rfree(itemid);
			return;
		}
	}
	FINDFLDGETSTRING(cvpnum,"VENDOR IDENTIFICATION",&venid1);
	FINDFLDGETSTRING(cvpnum,"CATEGORY IDENTIFICATION",&catid1);
	FINDFLDGETSTRING(cvpnum,"ITEM IDENTIFICATION",&itemid1);
	FINDFLDGETINT(cvpnum,"QUANTITY",&quantity1);
	if(!RDAstrcmp(catid,catid1) && !RDAstrcmp(itemid,itemid1) &&
		!RDAstrcmp(venid,venid1) && quantity1>quantity) 
	{
		FINDFLDGETCHAR(cvpnum,"DELETEFLAG",&delflag);
		if(!delflag)
		{
		FINDFLDSETSTRING(cvennum,"VENDOR IDENTIFICATION",venid1);
		FINDFLDSETSTRING(cvennum,"CATEGORY IDENTIFICATION",catid1);
		FINDFLDSETSTRING(cvennum,"ITEM IDENTIFICATION",itemid1);
		if(!ADVEQLNRDsec(cvennum,1,SCRNvirtualSubData,lcatrsrc))
		{
			FINDFLDGETCHAR(cvennum,"DELETEFLAG",&delflag);
			FINDFLDGETCHAR(cvennum,"ACTIVE",&active);
/* Might have to check the valid to date against the system date to test if the CTLGVEN file is valid or out of date */
			if(active && !delflag)
			{
			FINDFLDSETSTRING(ctlgnum,"CATEGORY IDENTIFICATION",catid1);
			FINDFLDSETSTRING(ctlgnum,"ITEM IDENTIFICATION",itemid1);
			if(!ADVEQLNRDsec(ctlgnum,1,SCRNvirtualSubData,lcatrsrc))
			{
			FINDFLDGETCHAR(ctlgnum,"DELETEFLAG",&delflag);
			FINDFLDGETCHAR(ctlgnum,"ACTIVE",&active);
			if(active && !delflag)
			{
				FINDFLDGETDOUBLE(cvpnum,"UNIT RATE",&unit_rate);
			}
			}
			}
		}
		}
		FINDRSCSETINT(lcatrsrc,"QUANTITY",quantity1);
		FINDRSCSETDOUBLE(lcatrsrc,"UNIT RATE",unit_rate);
		updatersrc(lcatrsrc,"QUANTITY");
		updatersrc(lcatrsrc,"UNIT RATE");
		getsupportingcatrecords(lcatrsrc);
	}
	if(catid!=NULL) Rfree(catid);
	if(venid!=NULL) Rfree(venid);
	if(itemid!=NULL) Rfree(itemid);
	if(catid1!=NULL) Rfree(catid1);
	if(venid1!=NULL) Rfree(venid1);
	if(itemid1!=NULL) Rfree(itemid1);
	updateSCRNvirtuals(lcatrsrc);
}
static void setitemlistcb(RDArsrc *lcatrsrc)
{
	int x=0;

	FINDRSCGETINT(lcatrsrc,"ITEM IDENTIFICATIONS",&x);
	if(cvennum!=(-1))
	{
		if(RDAstrcmp(itemlist->libs[x],"No Catalogue Vendor Items Defined"))
		{
			FINDFLDSETSTRING(cvennum,"ITEM IDENTIFICATION",itemlist->libs[x]);
			FINDFLDSETSTRING(cvpnum,"ITEM IDENTIFICATION",itemlist->libs[x]);
			FINDFLDSETSTRING(linenum,"ITEM IDENTIFICATION",itemlist->libs[x]);
			FINDRSCSETSTRING(lcatrsrc,"ITEM IDENTIFICATION",itemlist->libs[x]);
		} else {
			FINDFLDSETSTRING(cvennum,"ITEM IDENTIFICATION",NULL);
			FINDFLDSETSTRING(cvpnum,"ITEM IDENTIFICATION",NULL);
			FINDFLDSETSTRING(linenum,"ITEM IDENTIFICATION",NULL);
			FINDRSCSETSTRING(lcatrsrc,"ITEM IDENTIFICATION",NULL);
		}
	}
	updatersrc(lcatrsrc,"ITEM IDENTIFICATION");
	getquantitycb(lcatrsrc);
}
static void docategorylist(RDArsrc *lcatrsrc)
{
	char *venid=NULL,*venid1=NULL,*catid=NULL;
	short keyno=(-1),ef=0;
#ifdef DIAGNOSTIC_SCREENS
	RDArsrc *tmprsrc=NULL;
#endif

	if(categorylist==NULL)
	{
		if(categorylist!=NULL) freeapplib(categorylist);
		ZERNRD(cvennum);
		FINDRSCGETSTRING(lcatrsrc,"VENDOR IDENTIFICATION",&venid);
		FINDFLDSETSTRING(cvennum,"VENDOR IDENTIFICATION",venid);
		categorylist=APPlibNEW();
		if(categorylist==NULL) return;
#ifdef DIAGNOSTIC_SCREENS
		tmprsrc=diagscrn(cvennum,"DIAGNOSTIC SCREEN",module,"Creating Category List",NULL);
		if(tmprsrc!=NULL)
		{
			if(ADVmakescrn(tmprsrc,FALSE))
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED","The Make Screen function failed for the custom screen DIAGNOSTIC SCREEN .  Check to see the screen is available.  If it is, call your installer.\n",NULL,NULL,TRUE,NULL);
				prterr("Error Cannot Create Diagnostic Screen.");
			} else {
				ForceWindowUpdate(tmprsrc);
			}
		}
#endif
		keyno=KEYNUMBERBYNAME("CATALOGUE","CTLGVEN","RDA CTLGVEN KEY");
		if(keyno==(-1)) keyno=1;
		ef=ADVGTENRDsec(cvennum,keyno,SCRNvirtualSubData,lcatrsrc);
		while(!ef)
		{
			FINDFLDGETSTRING(cvennum,"VENDOR IDENTIFICATION",&venid1);
			if(RDAstrcmp(venid1,venid)) break;
			FINDFLDGETSTRING(cvennum,"CATEGORY IDENTIFICATION",&catid);
			addAPPlibNoDuplicates(categorylist,catid);
			if(catid!=NULL) Rfree(catid);
#ifdef DIAGNOSTIC_SCREENS
			update_diagnostic(tmprsrc,TRUE);
#endif
			ef=ADVNXTNRDsec(cvennum,keyno,SCRNvirtualSubData,lcatrsrc);
		}
		if(venid!=NULL) Rfree(venid);
		if(venid1!=NULL) Rfree(venid1);
		if(catid!=NULL) Rfree(catid);
#ifdef DIAGNOSTIC_SCREENS
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
		}
/* causes an error from the Xserver [Error: Object "" does not have windowed ancestor] and the process dies with no SegmentationFault or MemoryFault system error.  Xserver kills process.
		ForceWindowUpdate(lcatrsrc);
*/
#endif
		if(categorylist->numlibs<1)
		{
			addAPPlib(categorylist,"No Categories Defined");
		}
	}
}
static void doitemlist(RDArsrc *lcatrsrc)
{
	char deleteflag=FALSE,*catid=NULL,*catid1=NULL;
	char *itemid=NULL,*venid=NULL,*venid1=NULL;
	short keyno=(-1),ef=0;
	int selitem=0,selcat=0;
#ifdef DIAGNOSTIC_SCREENS
	RDArsrc *tmprsrc=NULL;
#endif

	FINDRSCGETSTRING(lcatrsrc,"VENDOR IDENTIFICATION",&venid);
	FINDRSCGETINT(lcatrsrc,"CATEGORY IDENTIFICATIONS",&selcat);
	if(RDAstrcmp(categorylist->libs[selcat],"No Categories Defined"))
	{
		catid=stralloc(categorylist->libs[selcat]);
	}
	if(RDAstrcmp(vendor_id,venid) || 
		RDAstrcmp(category_id,catid) || 
		itemlist==NULL)
	{
#ifdef DIAGNOSTIC_SCREENS
		tmprsrc=diagscrn(cvennum,"DIAGNOSTIC SCREEN",module,"Creating Catalogued Items List",NULL);
		if(tmprsrc!=NULL)
		{
			if(ADVmakescrn(tmprsrc,FALSE))
			{
				WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED","The Make Screen function failed for the custom screen DIAGNOSTIC SCREEN .  Check to see the screen is available.  If it is, call your installer.\n",NULL,NULL,TRUE,NULL);
				prterr("Error Cannot Create Diagnostic Screen.");
			} else {
				ForceWindowUpdate(tmprsrc);
			}
		}
#endif
		FINDRSCGETINT(lcatrsrc,"ITEM IDENTIFICATIONS",&selitem);
		if(itemlist!=NULL) freeapplib(itemlist);
		itemlist=APPlibNEW();
		ZERNRD(cvennum);
		FINDFLDSETSTRING(cvennum,"VENDOR IDENTIFICATION",venid);
		FINDFLDSETSTRING(cvennum,"CATEGORY IDENTIFICATION",catid);
		keyno=KEYNUMBERBYNAME("CATALOGUE","CTLGVEN","RDA CTLGVEN KEY");
		if(keyno==(-1)) keyno=1;
		ef=ADVGTENRDsec(cvennum,keyno,SCRNvirtualSubData,lcatrsrc);
		while(!ef)
		{
			FINDFLDGETCHAR(cvennum,"DELETEFLAG",&deleteflag);
			if(!deleteflag)
			{
				FINDFLDGETSTRING(cvennum,"VENDOR IDENTIFICATION",&venid1);
				FINDFLDGETSTRING(cvennum,"CATEGORY IDENTIFICATION",&catid1);
				if(!RDAstrcmp(venid1,venid) && !RDAstrcmp(catid1,catid))
				{
					FINDFLDGETSTRING(cvennum,"ITEM IDENTIFICATION",&itemid);
					addAPPlib(itemlist,itemid);
					if(itemid!=NULL) Rfree(itemid);
				} else break;
			}
#ifdef DIAGNOSTIC_SCREENS
			update_diagnostic(tmprsrc,TRUE);
#endif
			ef=ADVNXTNRDsec(cvennum,keyno,SCRNvirtualSubData,lcatrsrc);
		}
		if(itemlist->numlibs<1)
		{
			addAPPlib(itemlist,"No Catalogue Vendor Items Defined");
		}
		if(selitem>=itemlist->numlibs) selitem=0;
		if(!FINDRSCLISTAPPlib(lcatrsrc,"ITEM IDENTIFICATIONS",selitem,itemlist))
		{
			updatersrc(lcatrsrc,"ITEM IDENTIFICATIONS");
		}
		vendor_id=stralloc(venid);
		category_id=stralloc(catid);
#ifdef DIAGNOSTIC_SCREENS
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc);
		}
/*
		ForceWindowUpdate(lcatrsrc);
*/
#endif
	}
	if(venid!=NULL) Rfree(venid);
	if(venid1!=NULL) Rfree(venid1);
	if(catid!=NULL) Rfree(catid);
	if(catid1!=NULL) Rfree(catid1);
}
static void getitemcb(RDArsrc *lcatrsrc)
{
	char *itemid=NULL;
	int x=0;

	readwidget(lcatrsrc,"ITEM IDENTIFICATION");
	FINDRSCGETSTRING(lcatrsrc,"ITEM IDENTIFICATION",&itemid);
	FINDFLDSETSTRING(linenum,"ITEM IDENTIFICATION",itemid);
	if(cvennum!=(-1))
	{
		FINDFLDSETSTRING(cvennum,"ITEM IDENTIFICATION",itemid);
		FINDFLDSETSTRING(cvpnum,"ITEM IDENTIFICATION",itemid);
		x=FINDAPPLIBELEMENT(itemlist,itemid);
		if(x==(-1)) x=0;
		FINDRSCSETINT(lcatrsrc,"ITEM IDENTIFICATIONS",x);
	}
	if(itemid!=NULL) Rfree(itemid);
	updatersrc(lcatrsrc,"ITEM IDENTIFICATIONS");
	updatersrc(lcatrsrc,"ITEM IDENTIFICATION");
}
static void getcategorycb(RDArsrc *lcatrsrc)
{
	int selected=0;
	char *venid=NULL;

	FINDRSCGETSTRING(lcatrsrc,"VENDOR IDENTIFICATION",&venid);
	FINDRSCGETINT(lcatrsrc,"CATEGORY IDENTIFICATIONS",&selected);
	if(RDAstrcmp(categorylist->libs[selected],"No Categories Defined"))
	{
		FINDFLDSETSTRING(linenum,"CATEGORY IDENTIFICATION",
			categorylist->libs[selected]);
	} else {
		FINDFLDSETSTRING(linenum,"CATEGORY IDENTIFICATION",NULL);
	}
	doitemlist(lcatrsrc);
	setitemlistcb(lcatrsrc);
	if(venid!=NULL) Rfree(venid);
}
static void setcatlists(RDArsrc *lcatrsrc)
{
	char *temp=NULL;
	int x=0;

	if(catnum!=(-1))
	{
		FINDFLDGETSTRING(linenum,"CATEGORY IDENTIFICATION",&temp);
		x=FINDAPPLIBELEMENT(categorylist,temp);
		if(x==(-1)) x=0;
		FINDRSCSETINT(lcatrsrc,"CATEGORY IDENTIFICATIONS",x);
		if(RDAstrcmp(categorylist->libs[x],"No Categories Defined"))
		{
			FINDFLDSETSTRING(linenum,"CATEGORY IDENTIFICATION",categorylist->libs[x]);
		} else {
			FINDFLDSETSTRING(linenum,"CATEGORY IDENTIFICATION",NULL);
		}
	}
	if(temp!=NULL) Rfree(temp);
	updatersrc(lcatrsrc,"CATEGORY IDENTIFICATIONS");
	doitemlist(lcatrsrc);
	if(catnum!=(-1))
	{
		if(temp!=NULL) Rfree(temp);
		FINDFLDGETSTRING(linenum,"ITEM IDENTIFICATION",&temp);
		x=FINDAPPLIBELEMENT(itemlist,temp);
		if(x==(-1)) x=0;
		FINDRSCSETINT(lcatrsrc,"ITEM IDENTIFICATIONS",x);
		if(RDAstrcmp(itemlist->libs[x],"No Catalogue Vendor Items Defined"))
		{
			FINDFLDSETSTRING(linenum,"ITEM IDENTIFICATION",itemlist->libs[x]);
		} else {
			FINDFLDSETSTRING(linenum,"ITEM IDENTIFICATION",NULL);
		}
	}
	setitemlistcb(lcatrsrc);
}
static void quit_cat(RDArsrc *lcatrsrc)
{
	if(lcatrsrc!=NULL) 
	{
		killwindow(lcatrsrc);
		free_rsrc(lcatrsrc);
	}
}
static void selectcat(RDArsrc *lcatrsrc,RDArsrc *parent)
{
	int selcat=0,selitem=0;
	char *venid=NULL,*desc=NULL,*venitemno=NULL;
	int quantity=0;
	double unit_rate=0.0;

	FINDRSCGETSTRING(lcatrsrc,"VENDOR IDENTIFICATION",&venid);
	FINDRSCGETINT(lcatrsrc,"CATEGORY IDENTIFICATIONS",&selcat);
	if(!RDAstrcmp(categorylist->libs[selcat],"No Categories Defined"))
	{
		quit_cat(lcatrsrc);
		return;
	}
	FINDRSCGETINT(lcatrsrc,"ITEM IDENTIFICATIONS",&selitem);
	if(!RDAstrcmp(itemlist->libs[selitem],"No Catalogue Vendor Items Defined"))
	{
		quit_cat(lcatrsrc);
		return;
	}
	FINDRSCGETSTRING(lcatrsrc,"[CTLGMST][ITEM NAME]",&desc);
	FINDRSCGETSTRING(lcatrsrc,"[CTLGVEN][VENDOR ITEM NUMBER]",&venitemno);
	FINDRSCGETINT(lcatrsrc,"QUANTITY",&quantity);
	FINDRSCGETDOUBLE(lcatrsrc,"UNIT RATE",&unit_rate);
	FINDRSCSETSTRING(parent,"[PURLIN][ITEM IDENTIFICATION]",itemlist->libs[selitem]);
	FINDRSCSETSTRING(parent,"[PURLIN][CATEGORY IDENTIFICATION]",categorylist->libs[selcat]);
	FINDRSCSETSTRING(parent,"[PURLIN][VENDOR ITEM NUMBER]",venitemno);
	FINDRSCSETSTRING(parent,"[PURLIN][DESCRIPTION]",desc);
	FINDRSCSETINT(parent,"[PURLIN][QUANTITY]",quantity);
	FINDRSCSETDOUBLE(parent,"[PURLIN][UNIT RATE]",unit_rate);
	updateallrsrc(parent);
	caltotal(parent);
	if(venid!=NULL) Rfree(venid);
	if(desc!=NULL) Rfree(desc);
	if(venitemno!=NULL) Rfree(venitemno);
	quit_cat(lcatrsrc);
}
static void LoadCatalogue(RDArsrc *parent)
{
	char *venid=NULL,*catid=NULL,*itemid=NULL;
	int quantity=0;
	int which_type=0;
	double unit_rate=0.0;
	RDArsrc *lcatrsrc=NULL;

	if(catnum!=(-1))
	{
		lcatrsrc=RDArsrcNEW(module,"LOAD CATALOGUE");
		readwidget(parent,"[PURMST][VENDOR IDENTIFICATION]");
		readwidget(parent,"[PURLIN][CATEGORY IDENTIFICATION]");
		readwidget(parent,"[PURLIN][ITEM IDENTIFICATION]");
		readwidget(parent,"[PURLIN][QUANTITY]");
		if(FINDRSCGETSTRING(parent,"[PURMST][VENDOR IDENTIFICATION]",&venid)) return;
		FINDFLDSETSTRING(linenum,"VENDOR IDENTIFICATION",venid);
		if(FINDRSCGETSTRING(parent,"[PURLIN][CATEGORY IDENTIFICATION]",&catid)) return;
		FINDFLDSETSTRING(linenum,"CATEGORY IDENTIFICATION",catid);
		if(FINDRSCGETSTRING(parent,"[PURLIN][ITEM IDENTIFICATION]",&itemid)) return;
		FINDFLDSETSTRING(linenum,"ITEM IDENTIFICATION",itemid);
		if(FINDRSCGETINT(parent,"[PURLIN][QUANTITY]",&quantity)) return;
		FINDFLDSETINT(linenum,"QUANTITY",quantity);
		addstdrsrc(lcatrsrc,"VENDOR IDENTIFICATION",VARIABLETEXT,0,venid,FALSE);
		addstdrsrc(lcatrsrc,"QUANTITY",LONGV,10,&quantity,TRUE);
		addstdrsrc(lcatrsrc,"UNIT RATE",DECIMALV,10,&unit_rate,TRUE);
		if(catid!=NULL) Rfree(catid);
		if(itemid!=NULL) Rfree(itemid);
		if(venid!=NULL) Rfree(venid);
		docategorylist(lcatrsrc);
		addlstrsrc(lcatrsrc, "CATEGORY IDENTIFICATIONS",&which_type,TRUE,
			getcategorycb,categorylist->numlibs,&categorylist->libs,NULL);
		addstdrsrc(lcatrsrc,"ITEM IDENTIFICATION",VARIABLETEXT,0,itemid,TRUE);
		addlstrsrc(lcatrsrc,"ITEM IDENTIFICATIONS",&which_type,TRUE,
			setitemlistcb,itemlist->numlibs,&itemlist->libs,NULL);
		doitemlist(lcatrsrc);
		file2rsrc(catnum,lcatrsrc,FALSE);
		file2rsrc(ctlgnum,lcatrsrc,FALSE);
		file2rsrc(cvennum,lcatrsrc,FALSE);
		file2rsrc(vennum,lcatrsrc,FALSE);
		FINDRSCSETFUNC(lcatrsrc,"ITEM IDENTIFICATION",getitemcb,NULL);
		FINDRSCSETFUNC(lcatrsrc,"QUANTITY",getquantitycb,NULL);
		setcatlists(lcatrsrc);
		getsupportingcatrecords(lcatrsrc); 
		SetVirFieldFuncs(lcatrsrc);
		SCRNvirtual2rsrc(lcatrsrc);
		ReadRDAScrolledLists(lcatrsrc);
		cvpkeyno=KEYNUMBER(cvpnum,"RDA CTLGVP KEY");
		if(cvpkeyno==(-1))
		{
			cvpkeyno=1;
		}
		addbtnrsrc(lcatrsrc,"PREVIOUS QUANTITY",TRUE,getprevquantitycb,NULL);
		addbtnrsrc(lcatrsrc,"NEXT QUANTITY",TRUE,getnextquantitycb,NULL);
		addbtnrsrc(lcatrsrc,"SELECT",TRUE,selectcat,parent);
		addbtnrsrc(lcatrsrc,"QUIT",TRUE,quit_cat,NULL);
		addbtnrsrc(lcatrsrc,"HELP",TRUE,screenhelp,NULL);
		addbtnrsrc(lcatrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		APPmakescrn(lcatrsrc,TRUE,quit_cat,NULL,FALSE);
	}
}
static void quit_record(RDArsrc *parent)
{
	if(definelist!=NULL) 
	{
		killwindow(definelist);
		free_rsrc(definelist); 
	}
	if(parent!=NULL)
	{
		killwindow(parent);
		free_rsrc(parent);
	}
	if(prevline!=NULL) freeapplib(prevline);
	if(categorylist!=NULL) freeapplib(categorylist);
	if(itemlist!=NULL) freeapplib(itemlist);
	if(types!=NULL) freeapplib(types);
}
static void SETFILES(RDArsrc *parent)
{
	readallwidgets(parent);
	IncVirRsrcstoFile(parent);
	getsupportingrecords(parent);
	updateSCRNvirtuals(parent);
}
void addlines(void)
{
	short nofields=0,nokeys=0,x=0,edit_rsrc=TRUE;
	NRDkey *keys=NULL;
	RDArsrc *parent=NULL;
	NRDfield *fields=NULL,*fldx=NULL;
	char *name=NULL;

	nokeys=NUMKEYS(linenum);
	keys=KEYPOINTER(linenum);
	nofields=NUMFLDS(linenum);
	fields=FLDPOINTER(linenum);
	parent=RDArsrcNEW(module,"ADD PO LINES");
	definelist=RDArsrcNEW(module,"ADD PO LINES DEFINE LIST");
	addDFincvir(parent,module,"PURMST",NULL,PURMST_MTNMASTER->passkey->fileno);
	addDFincvir(parent,module,"PURLIN",NULL,linenum);
	addDFincvir(definelist,module,"PURLIN",NULL,linenum);
	GET_SCREEN_VIRTUALS(parent,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	mk_define_list(definelist,nofields,fields,nokeys,keys,NULL,1);
	FINDRSCSETFUNC(definelist,"SELECT",savelistcb,parent);
	if(PURMST_MTNMASTER->passkey->fileno!=(-1)) file2rsrc(PURMST_MTNMASTER->passkey->fileno,parent,FALSE);
	if(vennum!=(-1)) file2rsrc(vennum,parent,FALSE);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER VERIFIED"))
			{
			} else if(!RDAstrcmp(fldx->name,"SOURCE USER"))
			{
			} else {
				edit_rsrc=TRUE;
				nonlstmakefld(parent,linenum,fldx,edit_rsrc);
				if(name!=NULL)
				{
					name=Rrealloc(name,RDAstrlen(fldx->name)+5+RDAstrlen(FILENAME(linenum)));
				} else {
					name=Rmalloc(RDAstrlen(fldx->name)+5+RDAstrlen(FILENAME(linenum)));
				}
				sprintf(name,"[%s][%s]",FILENAME(linenum),fldx->name);
				FINDRSCSETFUNC(parent,name,SETFILES,NULL);
			}
		}
	}
	nofields=0;
	nokeys=0;
	if(name!=NULL) Rfree(name);
	FINDRSCSETINT(parent,"[PURLIN][FISCAL YEAR]",yearid);
	FINDFLDSETINT(linenum,"FISCAL YEAR",yearid);
	FINDRSCSETSTRING(parent,"[PURLIN][PO IDENTIFICATION]",POID);
	FINDFLDSETSTRING(linenum,"PO IDENTIFICATION",POID);
	getsupportingrecords(parent);
	SetVirFieldFuncs(parent);
	SCRNvirtual2rsrc(parent);
	ReadRDAScrolledLists(parent);
	if(prevline!=NULL) freeapplib(prevline);
	prevline=APPlibNEW();
	addAPPlib(prevline,"No Lines added this session");
	addlstrsrc(parent,"PREVIOUS LINES",&x,TRUE,NULL,prevline->numlibs,
		&prevline->libs,NULL);
	FINDRSCSETSTRING(parent,"[PURLIN][ENTRY DATE]",CURRENT_DATE);
	FINDRSCSETSTRING(parent,"[PURLIN][ENTRY TIME]",CURRENT_TIME);
	savelist(definelist,parent); /* updateSCRNvirtuals in this function */
	FINDRSCSETFUNC(parent,"[PURLIN][QUANTITY]",caltotal,NULL);
	FINDRSCSETFUNC(parent,"[PURLIN][UNIT RATE]",caltotal,NULL);
	FINDRSCSETEDITABLE(parent,"[PURLIN][PO IDENTIFICATION]",FALSE);
	FINDRSCSETEDITABLE(parent,"[PURLIN][FISCAL YEAR]",FALSE);
	addstdrsrc(parent,"PO TOTAL",DOLLARS,20,&po_total,FALSE);
	if(catnum!=(-1)) addbtnrsrc(parent,"LOAD CATALOGUE",TRUE,LoadCatalogue,NULL);
	addbtnrsrc(parent,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(parent,"DEFINE LIST",TRUE,definelistfunc,NULL);
	addbtnrsrc(parent,"RESET DEFAULTS",TRUE,set_def_callback,NULL);
	addrfcbrsrc(parent,"SAVE",TRUE,save_check,NULL);
	addrfkwrsrc(parent,"QUIT",TRUE,quit_record,NULL);
	addrfcbrsrc(parent,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(parent,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	set_def_func(parent);
	getsupportingrecords(parent);
	computeallSCRNvirtuals(parent);
	APPmakescrn(parent,TRUE,quit_record,NULL,FALSE);
}
