/* payterm.c - Terminate Payroll Program */
/*lint -library */
#ifndef WIN32
#define __NAM__ "payterm.lnx"
#endif
#ifdef WIN32
#define __NAM__ "payterm.exe"
#endif
#include <app.hpp>



#include <cstdlib>
#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <mix.hpp>
#include <postrk.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

short perdmg=(-1),payjmst=(-1),posvsts=(-1),posmst=(-1),posvac=(-1);
char *module="PAYROLL",*reportname=NULL,DID_SOMETHING=FALSE;
RDArsrc *mainrsrc=NULL,*definelist=NULL;
APPlib *jtypes=NULL,*PREVTERMS=NULL;
static void getjtype(RDArsrc *);
static void docodelist(RDArsrc *);
static void changejtype(RDArsrc *);
static void savelist(RDArsrc *,RDArsrc *);
static void savelistcb(RDArsrc *,RDArsrc *);
static void definelistfunc(RDArsrc *);
static void set_defaults(RDArsrc *);
static void save_check(RDArsrc *);
static void quit_record(RDArsrc *);
static void save_record(RDArsrc *);
static void makedflist(void);
static void setSTATUSlist(RDArsrc *);
static void getsupportingfiles(RDArsrc *,short);

static void docodelist(RDArsrc *mainrsrc)
{
	if(jtypes!=NULL) freeapplib(jtypes);
	if(posvsts!=(-1))
	{
		ZERNRD(posvsts);
		jtypes=ADVMakeNRDlistAllSTRING(posvsts,module,
			"Vacancy Status List",1,"VACANT STATUS",SCRNvirtualSubData,mainrsrc);
	} else {
		jtypes=APPlibNEW();
	}
	if(jtypes->numlibs<1)
	{
		addAPPlib(jtypes,"No Vacancy Status Types Available");
	}
}
static void getjtype(RDArsrc *mtnrsrc)
{
	int selected=0;

	readwidget(mtnrsrc,"VACANT STATUS");
	FINDRSCGETINT(mtnrsrc,"VACANT STATUS",&selected);
	FINDFLDSETSTRING(posvac,"VACANT STATUS",jtypes->libs[selected]);
	FINDFLDSETSTRING(posvsts,"VACANT STATUS",jtypes->libs[selected]);
	if(ADVEQLNRDsec(posvsts,1,SCRNvirtualSubData,mtnrsrc)) KEYNRD(posvsts,1);
	updatefilerecord2rsrc(posvsts,mtnrsrc);
}
static void changejtype(RDArsrc *mtnrsrc)
{
	getjtype(mtnrsrc);
	updateSCRNvirtuals(mtnrsrc);
}
static void savelistcb(RDArsrc *definelist,RDArsrc *parent)
{
	savelist(definelist,parent);
	if(IsScreenDisplayed(definelist)) NULL_RSCS(definelist);
}
static void savelist(RDArsrc *definelist,RDArsrc *parent)
{
	short ef=0;
	char deleteflag=FALSE,term=FALSE,ver=FALSE;

	DID_SOMETHING=FALSE;
	if(IsScreenDisplayed(definelist)) readallwidgets(definelist);
	if(PREVTERMS!=NULL) freeapplib(PREVTERMS);
	PREVTERMS=APPlibNEW();
	ef=ADVGTENRDsec(payjmst,1,SCRNvirtualSubData,parent);
	while(!ef)
	{
		FINDFLDGETCHAR(payjmst,"DELETEFLAG",&deleteflag);
		FINDFLDGETCHAR(payjmst,"TERMINATE AT ENDING DATE",&term);
		FINDFLDGETCHAR(payjmst,"TERMINATION VERIFIED",&ver);
		if(!deleteflag && term && !ver)
		{
			DID_SOMETHING=TRUE;
			makedflist();
		}
		ef=ADVNXTNRDsec(payjmst,1,SCRNvirtualSubData,parent);
	}
	if(PREVTERMS->numlibs<1)
	{
		addAPPlib(PREVTERMS,"No Terminations added this session");
	}
	FINDRSCLISTAPPlib(parent,"PREVIOUS TERMINATIONS",(PREVTERMS->numlibs-1),
		PREVTERMS);
	updatersrc(parent,"PREVIOUS TERMINATIONS");
}
static void makedflist()
{
	short x,cur=1,length=0,lcount=0;
	RDArmem *len_member,*pos_member;
	char *s,*hold=NULL,*temp=NULL;
	NRDfield *field;

	if(PREVTERMS!=NULL)
	{
		if(PREVTERMS->numlibs==1 && !RDAstrcmp(PREVTERMS->libs[0],
			"No Terminations added this session"))
		{
			freeapplib(PREVTERMS);
			PREVTERMS=APPlibNEW();
		}
	} else {
		PREVTERMS=APPlibNEW();
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
					field=FLDNRD(payjmst,s);
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
	addAPPlib(PREVTERMS,hold);
	if(hold!=NULL) Rfree(hold);
}
static void definelistfunc(RDArsrc *parent)
{
	computeallSCRNvirtuals(definelist);
	APPmakescrn(definelist,TRUE,NULL,NULL,FALSE);
}
static void set_defaults(RDArsrc *parent)
{
	int selected=0;

	DefaultResetScreen(parent,TRUE);
	FINDRSCGETINT(parent,"VACANT STATUS",&selected);
	FINDFLDSETSTRING(posvsts,"VACANT STATUS",jtypes->libs[selected]);
	if(ADVEQLNRDsec(posvsts,1,SCRNvirtualSubData,parent)) KEYNRD(posvsts,1);
	computeallSCRNvirtuals(parent);
	updateallrsrc(parent);
}
static void save_check(RDArsrc *mtnrsrc)
{
	int sel_type=0,jobno=(-1);
	char *temp=NULL,*perid1=NULL,*warnmessage=NULL,t=TRUE;

	readallwidgets(mtnrsrc);
	rsrc2filerecord(payjmst,mtnrsrc);
	rsrc2filerecord(posvac,mtnrsrc);
	FINDFLDSETCHAR(payjmst,"TERMINATE AT ENDING DATE",t);
	FINDFLDSETSTRING(payjmst,"TERMINATED BY",USERLOGIN);
	t=FALSE;
	FINDFLDSETCHAR(payjmst,"TERMINATION VERIFIED",t);
	COPYFIELD(payjmst,posvac,"POSITION IDENTIFICATION");
	COPYFIELD(payjmst,posvac,"FTE AMOUNT");
	FINDRSCGETSTRING(mtnrsrc,"[PAYJMST][PERSONNEL IDENTIFICATION]",&perid1);
	FINDFLDSETSTRING(perdmg,"PERSONNEL IDENTIFICATION",perid1);
	if(ADVEQLNRDsec(perdmg,1,SCRNvirtualSubData,mtnrsrc))
	{
		warnmessage=Rmalloc(RDAstrlen(perid1)+190);
		sprintf(warnmessage,"The Applicant Identification of [%s] is an invalid value. It does not exist in the [PRSNNL][PERDMG] data file.  Please select a valid Applicant Identification to save this record.",(perid1==NULL?"":perid1));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PERSONNEL IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		FINDFLDSETSTRING(payjmst,"PERSONNEL IDENTIFICATION",perid1);
		if(perid1!=NULL) Rfree(perid1);
		return;
	}
	FINDRSCGETINT(mtnrsrc,"[PAYJMST][JOB NUMBER]",&jobno);
	FINDFLDSETSTRING(payjmst,"PERSONNEL IDENTIFICATION",perid1);
	FINDFLDSETINT(payjmst,"JOB NUMBER",jobno);
	if(ADVEQLNRDsec(payjmst,1,SCRNvirtualSubData,mtnrsrc))
	{
		warnmessage=Rmalloc(290);
		sprintf(warnmessage,"The Job Number of [%d] is an invalid value. It does not exist in the [PAYROLL][PAYJMST] data file.  Please select a valid job number to save this record.",jobno);
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID JOB NUMBER!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		FINDFLDSETSTRING(payjmst,"PERSONNEL IDENTIFICATION",perid1);
		if(perid1!=NULL) Rfree(perid1);
		return;
	}
	FINDRSCGETSTRING(mtnrsrc,"[PAYJMST][POSITION IDENTIFICATION]",&perid1);
	FINDFLDSETSTRING(posmst,"POSITION IDENTIFICATION",perid1);
	if(ADVEQLNRDsec(posmst,1,SCRNvirtualSubData,mtnrsrc))
	{
		warnmessage=Rmalloc(RDAstrlen(perid1)+190);
		sprintf(warnmessage,"The Position Identificaiton of [%s] is an invalid value. It does not exist in the [POSTRK][POSMSTR] data file.  Please select a valid position to save this record.",(perid1!=NULL ? perid1:""));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID POSITION IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		FINDFLDSETSTRING(payjmst,"PERSONNEL IDENTIFICATION",perid1);
		if(perid1!=NULL) Rfree(perid1);
		return;
	}
	FINDFLDSETSTRING(posvac,"POSITION IDENTIFICATION",perid1);
	FINDRSCGETSTRING(mtnrsrc,"[POSVAC][VACANCY IDENTIFICATION]",&perid1);
	FINDFLDSETSTRING(posvac,"VACANCY IDENTIFICATION",perid1);
	if(!ADVEQLNRDsec(posvac,1,SCRNvirtualSubData,mtnrsrc))
	{
		warnmessage=Rmalloc(RDAstrlen(perid1)+190);
		sprintf(warnmessage,"The Vacancy Identificaiton of [%s] already exists. Please select another vacancy identification.",(perid1!=NULL ? perid1:""));
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID VACANCY IDENTIFICATION!",warnmessage,NULL,NULL,TRUE,NULL);
		if(warnmessage!=NULL) Rfree(warnmessage);
		FINDFLDSETSTRING(posvac,"VACANCY IDENTIFICATION",perid1);
		if(perid1!=NULL) Rfree(perid1);
		return;
	}
	FINDRSCGETINT(mtnrsrc,"VACANT STATUS",&sel_type);
	if(!RDAstrcmp(jtypes->libs[sel_type],"No Vacancy Status Types Available"))
	{
		WARNINGDIALOG("WARNING DIALOG SCREEN","No Vacancy Status Types Available","Record cannot be saved until vacancy status types are available.",NULL,NULL,TRUE,NULL);
		if(temp!=NULL) Rfree(temp);
		if(perid1!=NULL) Rfree(perid1);
		return;
	}
	FINDFLDSETSTRING(posvac,"VACANT STATUS",jtypes->libs[sel_type]);
	if(perid1!=NULL) Rfree(perid1);
	save_record(mtnrsrc);
}
static void save_record(RDArsrc *mtnrsrc)
{
	char *rcddesc=NULL,*temp=NULL,t=TRUE;
	RDATData *prev=NULL;
	double fteamt=0;
	NRDfield *field=NULL;

	if(ADVRECORDsec(posvac,SCRNvirtualSubData,mtnrsrc))
	{
		rcddesc=ADVRECORDsecDesc(posvac,SCRNvirtualSubData,mtnrsrc);
		if(rcddesc!=NULL)
		{
			ERRORDIALOG("Security Access Denied.",rcddesc,NULL,FALSE);
      			Rfree(rcddesc);
		}
		prterr("Error didn't write record.");
	} else {
		WRTTRANS(posvac,0,NULL,NULL);
		LOCNRDFILE(payjmst);
		if(!ADVEQLNRDsec(payjmst,1,SCRNvirtualSubData,mtnrsrc))
		{
			prev=RDATDataNEW(payjmst);
			rsrc2filerecord(payjmst,mtnrsrc);
			FINDFLDSETCHAR(payjmst,"TERMINATE AT ENDING DATE",t);
			FINDFLDSETSTRING(payjmst,"TERMINATED BY",USERLOGIN);
		}
		t=FALSE;
		FINDFLDSETCHAR(payjmst,"TERMINATION VERIFIED",t);
		field=FLDNRD(payjmst,"TERMINATED DATE");
		if(field->len==8)
		{
		temp=GETCURRENTDATE();
		} else temp=GETCURRENTDATE10();
		FINDFLDSETSTRING(payjmst,"TERMINATED DATE",temp);
		if(temp!=NULL) Rfree(temp);
		temp=GETCURRENTTIME();
		FINDFLDSETSTRING(payjmst,"TERMINATED TIME",temp);
		if(temp!=NULL) Rfree(temp);
		ADVWRTTRANSsec(payjmst,0,NULL,prev,SCRNvirtualSubData,mtnrsrc);
		if(prev!=NULL) FreeRDATData(prev);
		FINDFLDGETSTRING(payjmst,"POSITION IDENTIFICATION",&temp);
		FINDFLDGETDOUBLE(payjmst,"FTE AMOUNT",&fteamt);
		ADVupdateftes(posmst,temp,-fteamt,SCRNvirtualSubData,mtnrsrc);
		ADVupdatetotfte(posmst,temp,fteamt,SCRNvirtualSubData,mtnrsrc);
		if(temp!=NULL) Rfree(temp);
		makedflist();
		DID_SOMETHING=TRUE;
		if(!FINDRSCLISTAPPlib(mtnrsrc,"PREVIOUS TERMINATIONS",
			(PREVTERMS->numlibs-1),PREVTERMS))
		{
			updatersrc(mtnrsrc,"PREVIOUS TERMINATIONS");
		}
  	}
}
static void quit_record(RDArsrc *mainrsrc)
{
	APPlib *args=NULL;

	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	if(definelist!=NULL) free_rsrc(definelist);
	if(jtypes!=NULL) freeapplib(jtypes);
	if(payjmst!=(-1)) CLSNRD(payjmst);
	if(posvsts!=(-1)) CLSNRD(posvsts);
	if(posmst!=(-1)) CLSNRD(posmst);
	if(posvac!=(-1)) CLSNRD(posvac);
	if(perdmg!=(-1)) CLSNRD(perdmg);
	if(PREVTERMS!=NULL) freeapplib(PREVTERMS);
	if(DID_SOMETHING)
	{
		args=APPlibNEW();
		addAPPlib(args,module);
		addAPPlib(args,reportname);
		ExecuteProgram("doreport",args);
		if(args!=NULL) freeapplib(args);
	}
	if(reportname!=NULL) Rfree(reportname);
	ShutdownSubsystems();
	exit(0);
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
	char *vacid=NULL;

	readwidget(b->funcargs,"[POSVAC][VACANCY IDENTIFICATION]");
	FINDRSCGETSTRING(b->funcargs,"[POSVAC][VACANCY IDENTIFICATION]",&vacid);
	ZERNRD(payjmst);
	if(ADVEQLNRDKEYsec(payjmst,1,targetkey,SCRNvirtualSubData,b->mainrsrc))
	{
		KEYNRD(payjmst,1);
	}
	filerecord2rsrc(payjmst,b->funcargs);
	COPYFIELD(payjmst,posvac,"POSITION IDENTIFICATION");
	COPYFIELD(payjmst,posvac,"FTE AMOUNT");
	FINDFLDSETSTRING(posvac,"VACANCY IDENTIFICATION",vacid);
	if(ADVEQLNRDsec(posvac,1,SCRNvirtualSubData,mainrsrc))
	{
		KEYNRD(posvac,1);
		getjtype(b->funcargs);
	}
	filerecord2rsrc(posvac,mainrsrc);
	COPYFIELD(posvac,posvsts,"VACANCY STATUS");
	if(ADVEQLNRDsec(posvsts,1,SCRNvirtualSubData,mainrsrc))
		KEYNRD(posvsts,1);
	filerecord2rsrc(posvsts,mainrsrc);
	setSTATUSlist(mainrsrc);
	if(vacid!=NULL) Rfree(vacid);
	getsupportingfiles(b->funcargs,TRUE);
	computeallSCRNvirtuals(b->funcargs);
	updateallrsrc(b->funcargs);
}
static short select_position(void)
{
	return TRUE;
}
void browse_positions(RDArsrc *mtnrsrc)
{
	RDArsrc *mainrsrc=NULL,*searchrsrc=NULL,*definelist=NULL;

	mainrsrc=RDArsrcNEW(module,"SELECT JOB MASTER");
	searchrsrc=RDArsrcNEW(module,"SELECT JOB MASTER SEARCH BROWSE");
	definelist=RDArsrcNEW(module,"SELECT JOB MASTER DEFINE LIST");
	ZERNRD(payjmst);
	addDFincvir(searchrsrc,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(searchrsrc,"POSTRK","POSMSTR",NULL,posmst);
	addDFincvir(searchrsrc,module,"PAYJMST",NULL,payjmst);
	addDFincvir(definelist,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(definelist,"POSTRK","POSMSTR",NULL,posmst);
	addDFincvir(definelist,module,"PAYJMST",NULL,payjmst);
	GET_SCREEN_VIRTUALS(definelist,1);
	GET_SCREEN_VIRTUALS(searchrsrc,2);
	ADVbrowse(payjmst,1,mainrsrc,searchrsrc,definelist,
		NULL,select_position,doexit_position,okfunc_position,NULL,
		mtnrsrc,TRUE,NULL,2,2,2,0);
}
static void getpayjmst(RDArsrc *mainrsrc)
{
	char *perid=NULL,*vacid=NULL;
	int jobno=(-1);

	readwidget(mainrsrc,"[PAYJMST][PERSONNEL IDENTIFICATION]");
	readwidget(mainrsrc,"[PAYJMST][JOB NUMBER]");
	readwidget(mainrsrc,"[POSVAC][VACANCY IDENTIFICATION]");
	FINDRSCGETSTRING(mainrsrc,"[POSVAC][VACANCY IDENTIFICATION]",&vacid);
	FINDRSCGETSTRING(mainrsrc,"[PAYJMST][PERSONNEL IDENTIFICATION]",&perid);
	FINDRSCGETINT(mainrsrc,"[PAYJMST][JOB NUMBER]",&jobno);
	FINDFLDSETSTRING(payjmst,"PERSONNEL IDENTIFICATION",perid);
	FINDFLDSETINT(payjmst,"JOB NUMBER",jobno);
	if(ADVEQLNRDsec(payjmst,1,SCRNvirtualSubData,mainrsrc))
	{
		KEYNRD(payjmst,1);
	}
	filerecord2rsrc(payjmst,mainrsrc);
	COPYFIELD(payjmst,posvac,"POSITION IDENTIFICATION");
	COPYFIELD(payjmst,posvac,"FTE AMOUNT");
	FINDFLDSETSTRING(posvac,"VACANCY IDENTIFICATION",vacid);
	if(ADVEQLNRDsec(posvac,1,SCRNvirtualSubData,mainrsrc))
	{
		KEYNRD(posvac,1);
		getjtype(mainrsrc);
	}
	if(vacid!=NULL) Rfree(vacid);
	if(perid!=NULL) Rfree(perid);
	filerecord2rsrc(posvac,mainrsrc);
	COPYFIELD(posvac,posvsts,"VACANCY STATUS");
	if(ADVEQLNRDsec(posvsts,1,SCRNvirtualSubData,mainrsrc))
		KEYNRD(posvsts,1);
	filerecord2rsrc(posvsts,mainrsrc);
	setSTATUSlist(mainrsrc);
	getsupportingfiles(mainrsrc,TRUE);
	computeallSCRNvirtuals(mainrsrc);
	updateallrsrc(mainrsrc);
}
static void getvacancy(RDArsrc *mainrsrc)
{
	char *posid=NULL,*vacid=NULL;

	readwidget(mainrsrc,"[POSVAC][VACANCY IDENTIFICATION]");
	FINDRSCGETSTRING(mainrsrc,"[POSVAC][VACANCY IDENTIFICATION]",&vacid);
	readwidget(mainrsrc,"[PAYJMST][POSITION IDENTIFICATION]");
	FINDRSCGETSTRING(mainrsrc,"[PAYJMST][POSITION IDENTIFICATION]",&posid);
	FINDFLDSETSTRING(posvac,"POSITION IDENTIFICATION",posid);
	FINDFLDSETSTRING(posvac,"VACANCY IDENTIFICATION",vacid);
	if(ADVEQLNRDsec(posvac,1,SCRNvirtualSubData,mainrsrc))
	{
		KEYNRD(posvac,1);
		getjtype(mainrsrc);
	}
	filerecord2rsrc(posvac,mainrsrc);
	COPYFIELD(posvac,posvsts,"VACANCY STATUS");
	if(ADVEQLNRDsec(posvsts,1,SCRNvirtualSubData,mainrsrc))
		KEYNRD(posvsts,1);
	filerecord2rsrc(posvsts,mainrsrc);
	setSTATUSlist(mainrsrc);
	computeallSCRNvirtuals(mainrsrc);
	updateallrsrc(mainrsrc);
}
static void getsupportingfiles(RDArsrc *mainrsrc,short update)
{
	if(perdmg!=(-1))
	{
		COPYFIELD(payjmst,perdmg,"PERSONNEL IDENTIFICATION");
		if(ADVEQLNRDsec(perdmg,1,SCRNvirtualSubData,mainrsrc)) 
			KEYNRD(perdmg,1);
		if(update) updatefilerecord2rsrc(perdmg,mainrsrc);
			else filerecord2rsrc(perdmg,mainrsrc);
	}
	if(posmst!=(-1))
	{
		COPYFIELD(payjmst,posmst,"POSITION IDENTIFICATION");
		if(ADVEQLNRDsec(posmst,1,SCRNvirtualSubData,mainrsrc)) 
			KEYNRD(posmst,1);
		if(update) updatefilerecord2rsrc(posmst,mainrsrc);
			else filerecord2rsrc(posmst,mainrsrc);
	}
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	NRDkey *keys=NULL;
	NRDfield *fields=NULL,*fldx=NULL;
	short nokeys=0,nofields=0,x=0,edit_rsrc=TRUE;
	int start=0;
	char *appid=NULL;

	if(InitializeSubsystems(argc,argv,module,"TERMINATE EMPLOYEES")) 
	{
		RDAAPPMAINLOOP();
		return;
	}
	if(argc>1)
	{
		reportname=stralloc(argv[1]);
	} else reportname=stralloc("PAYTERM");
	if((perdmg=APPOPNNRD("PRSNNL","PERDMG",TRUE,FALSE))==(-1)) return;
	if((payjmst=APPOPNNRD(module,"PAYJMST",TRUE,TRUE))==(-1)) return;
	if((posvac=APPOPNNRD("POSTRK","POSVAC",TRUE,TRUE))==(-1)) return;
	if((posmst=APPOPNNRD("POSTRK","POSMSTR",TRUE,TRUE))==(-1)) return;
	if((posvsts=APPOPNNRD("POSTRK","POSVSTS",TRUE,FALSE))==(-1)) return;
	ZERNRD(payjmst);
	FINDFLDGETSTRING(perdmg,"PERSONNEL IDENTIFICATION",&appid);
	FINDFLDSETSTRING(payjmst,"PERSONNEL IDENTIFICATION",appid);
	mainrsrc=RDArsrcNEW(module,"TERMINATE EMPLOYEES");
	definelist=RDArsrcNEW(module,"TERMINATE EMPLOYEES DEFINE LIST");
	addDFincvir(mainrsrc,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(mainrsrc,module,"PAYJMST",NULL,payjmst);
	addDFincvir(mainrsrc,"POSTRK","POSVAC",NULL,posvac);
	addDFincvir(mainrsrc,"POSTRK","POSMSTR",NULL,posmst);
	addDFincvir(mainrsrc,"POSTRK","POSVSTS",NULL,posvsts);
	addDFincvir(definelist,"PRSNNL","PERDMG",NULL,perdmg);
	addDFincvir(definelist,"POSTRK","POSVAC",NULL,posvac);
	addDFincvir(definelist,"POSTRK","POSMSTR",NULL,posmst);
	addDFincvir(definelist,"POSTRK","POSVSTS",NULL,posvsts);
	addDFincvir(definelist,module,"PAYJMST",NULL,payjmst);
	GET_SCREEN_VIRTUALS(mainrsrc,0);
	GET_SCREEN_VIRTUALS(definelist,1);
	file2rsrc(payjmst,mainrsrc,TRUE);
	FINDRSCSETEDITABLE(mainrsrc,"[PAYJMST][POSITION IDENTIFICATION]",FALSE);
	FINDRSCSETEDITABLE(mainrsrc,"[PAYJMST][FTE AMOUNT]",FALSE);
	nokeys=NUMKEYS(posvac);
	keys=KEYPOINTER(posvac);
	nofields=NUMFLDS(posvac);
	fields=FLDPOINTER(posvac);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			if(!RDAstrcmp(fldx->name,"DELETEFLAG"))
			{
			} else if(!RDAstrcmp(fldx->name,"POSITION IDENTIFICATION"))
			{
			} else if(!RDAstrcmp(fldx->name,"FTE AMOUNT"))
			{
			} else if(!RDAstrcmp(fldx->name,"VACANT STATUS"))
			{
				start=0;
				edit_rsrc=TRUE;
				if(FIELDscrn(posvac,fldx->name,&edit_rsrc))
				{
					docodelist(mainrsrc);
					addlstrsrc(mainrsrc,"VACANT STATUS",
						&start,(edit_rsrc==TRUE?TRUE:FALSE),
						changejtype,jtypes->numlibs,
						&jtypes->libs,NULL);
					if(!edit_rsrc)
						FINDRSCSETEDITABLE(mainrsrc,"VACANT STATUS",FALSE);
				}
			} else {
				edit_rsrc=TRUE;
				nonlstmakefld(mainrsrc,posvac,fldx,edit_rsrc);
			}
		}
	}
	file2rsrc(posvsts,mainrsrc,FALSE);
	file2rsrc(posmst,mainrsrc,FALSE);
	file2rsrc(perdmg,mainrsrc,FALSE);
	FINDRSCSETFUNC(mainrsrc,"[PAYJMST][PERSONNEL IDENTIFICATION]",getpayjmst,
		NULL);
	FINDRSCSETFUNC(mainrsrc,"[PAYJMST][JOB NUMBER]",getpayjmst,NULL);
	FINDRSCSETFUNC(mainrsrc,"[POSVAC][VACANCY IDENTIFICATION]",getvacancy,
		NULL);
	if(PREVTERMS!=NULL) freeapplib(PREVTERMS);
	PREVTERMS=APPlibNEW();
	addAPPlib(PREVTERMS,"No Terminations added this session");
	addlstrsrc(mainrsrc,"PREVIOUS TERMINATIONS",&x,TRUE,NULL,PREVTERMS->numlibs,
		&PREVTERMS->libs,NULL);
	nokeys=NUMKEYS(payjmst);
	keys=KEYPOINTER(payjmst);
	nofields=NUMFLDS(payjmst);
	fields=FLDPOINTER(payjmst);
	mk_define_list(definelist,nofields,fields,nokeys,keys,NULL,1);
	nofields=0;
	nokeys=0;
	FINDRSCSETFUNC(definelist,"SELECT",savelistcb,mainrsrc);
	DefaultScreens(definelist);
	changejtype(mainrsrc);
	savelist(definelist,mainrsrc);
	SetVirFieldFuncs(mainrsrc);
	SCRNvirtual2rsrc(mainrsrc);
	addbtnrsrc(mainrsrc,"BROWSE",TRUE,browse_positions,NULL);
	addbtnrsrc(mainrsrc,"DEFINE LIST",TRUE,definelistfunc,NULL);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addbtnrsrc(mainrsrc,"RESET DEFAULTS",TRUE,set_defaults,NULL);
	addrfcbrsrc(mainrsrc,"SAVE",TRUE,save_check,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quit_record,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	set_defaults(mainrsrc);
	APPmakescrn(mainrsrc,TRUE,quit_record,NULL,TRUE);
	RDAAPPMAINLOOP();
}
static void setSTATUSlist(RDArsrc *mtnrsrc)
{
	char *status=NULL;
	int x=0;

	FINDFLDGETSTRING(posvac,"VACANT STATUS",&status);
	if((x=FINDFLDLISTELEMENT(posvac,"VACANT STATUS",jtypes,status))==(-1))
	{
		x=0;
	}
	FINDRSCSETINT(mtnrsrc,"VACANT STATUS",x);
}
