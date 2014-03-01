/* dfrpt.c - Define Reports */
/*lint -library */
#ifndef WIN32
#define __NAM__ "dfrpt.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dfrpt.exe"
#endif
#include <app.hpp>


#include <misc.hpp>



#include <gui.hpp>
#include <nrd.hpp>
#include <olh.hpp>
#include <mix.hpp>
#include <rptgen.hpp>
#include <ldval.hpp>
#ifdef WIN32
#include<cstdlib>
#include<io.h>
#endif
/*CPP_OPART dfrptvir */
/*CPP_OPART dfrptsch */
/*CPP_OPART dfrptslt */
/*CPP_OPART dfrptctl */
/*CPP_OPART dfrptsrt */
/*CPP_OPART dfrptacc */
/*CPP_OPART dfrptimg */
/*CPP_OPART dfrptimf */
/*CPP_OPART dfmtnsup */
/*CPP_OPART dfmtnsub */
/*CPP_OPART dfmtnnew */
/*CPP_OPART dfmtnprv */
/*CPP_OPART dfmtnswt */
/*CPP_OPART dfmtnact */
/*CPP_OPART dfrptscn */
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */
static void deleterpt(RDArsrc *),deleterpttest(RDArsrc *);
static void saverpt(RDArsrc *,RDArsrc *); 
static void doreport(RDArsrc *);
static void setchainrpt(RDArsrc *);
static void setchainrptcb(RDArsrc *);
static void setremfile(RDArsrc *);
static void setremfilecb(RDArsrc *);

char *module="RPTGEN",*rptmodule=NULL;
APPlib *rptavl=NULL,*devtypes=NULL,*devvars=NULL,*modlstx=NULL;
APPlib *remfile=NULL;
APPlib *dumprttypes=NULL,*BrowseTypes=NULL;
APPlib *dbengines=NULL;
RDAreport *report=NULL;
RDArsrc *dsprsrc=NULL,*rptrsrc=NULL;
RDArsrc *ldvalrsrc1=NULL,*mainrsrc=NULL;
RDApvar *holdpvar=NULL;
int holdpnum=0;
RDAconfld *holdconfld=NULL;
RDArsrc *econrsrc=NULL,*msrchrsrc=NULL,*schrsrc=NULL;
APPlib *ctypes=NULL,*modlst=NULL,*schavl=NULL;
RDAsearch *tempsearch=NULL;
int tmpnum=0;
APPlib *filelst=NULL,*cmodlst=NULL,*subkeylist=NULL,*cnflds=NULL;
APPlib *econflds=NULL,*diaglevel=NULL;
int holdnum=0;
static void setvaluescb(RDArsrc *);
static void setvalues(RDArsrc *,char *,char *);
char changeddisplay=FALSE;
char changedreport=FALSE;
RDAdefault *rptdefaults=NULL,*dspdefaults=NULL;

extern void makeRangeScreen(char *,RDAreport *);
extern void makeRPTBrowseScreen(char *,RDAreport *);
extern void makeRPTDefineList(char *,RDAreport *);
extern void makeRPTSearchBrowse(char *,RDAreport *);
extern void makeRPTMaintainScreen(char *,RDAreport *);
extern void makeDefaultMenu(char *,RDAreport *);

extern void virflds(RDArsrc *),RPTsearch(RDArsrc *),RPTselectrecords(RDArsrc *);
extern void RPTcontrol(RDArsrc *),RPTimage2(RDArsrc *);
extern void RPTsortdef(RDArsrc *),RPTaccum(RDArsrc *),RPTimage1(RDArsrc *);
void getrptentriescb(RDArsrc *);
void getrptentries(RDArsrc *,char *);

char *contype[20]={
	"Detail",
	"Equal To",
	"Equal To or Skip",
	"Equal To or Error",
	"Greater Than or Equal To",
	"Greater Than and Equal To or Skip",
	"Greater Than and Equal To or Error",
	"Less Than and Equal To",
	"Less Than and Equal To or Skip",
	"Less Than and Equal To or Error",
	"Greater Than",
	"Greater Than or Skip",
	"Greater Than or Error",
	"Less Than",
	"Less Than or Skip",
	"Less Than or Error",
	"Supporting Detail",
	"Not Equal To",
	"Detail Break",
	"Supporting Detail Break",
};
char *sequences[6]={"Ascending","Ascending Lower","Ascending Upper","Descending","Descending Lower","Descending Upper"};
char *dvtypes[6]={"PRINTER","ASCII FILE","APPEND ASCII FILE","DATAFILE","UPDATE DATAFILE","POPEN"};
char *accumtypes[8]={"AVERAGE","FREQUENCY","FIRST OCCURRENCE","LAST OCCURRENCE","MAXIMUM","MINIMUM","PREVIOUS OCCURRENCE","SUMMATION"};


RDArsrc *mtnrsrc=NULL,*filersrc=NULL;
APPlib *actstructavl=NULL,*btntypes=NULL,*startwheres=NULL;
APPlib *styles=NULL,*ftypes=NULL,*starttypes=NULL;
APPlib *file_defs=NULL;
APPlib *buttonsavl=NULL,*fieldsavl=NULL,*btnfldavl=NULL,*keysavl=NULL;
APPlib *filelist=NULL,*tmpargs=NULL,*modlstxmtn=NULL;
MaintainMaster *MTNMSTR=NULL;
void addholdvalues(APPlib *,char *,char *);
void addholdvaluessup(APPlib *,char *,char *);
void addholdvaluessub(APPlib *,char *,char *);
void addholdvaluesact(APPlib *,char *,char *);
static void savemaintain(RDArsrc *,RDArsrc *);
static void quitmaintaintest(RDArsrc *,RDArsrc *);
static void quitmaintain(RDArsrc *,RDArsrc *);
static RDAdefault *mtntdefaults=NULL,*buttondefaults=NULL;
static RDAdefault *btnflddefaults=NULL,*argdefaults=NULL;
static char changedargs=FALSE,changedbtnfld=FALSE,changedbutton=FALSE;
char changedvalues=FALSE;
/*
static char *snames[4]={
	"BROWSE SCREEN",
	"DEFINE LIST",
	"SEARCH BROWSE",
	"MAINTAIN SCREEN"}; 
*/
static int numHoldButtons=0,numHoldButtonField=0;
static char *style_names[3]={
	"Hard Coded",
	"Utility Master",
	"Special"};
static char *filetype_names[2]={
	"Normal File",
	"Special File"};
static char *startwith_names[3]={
	"Maintain Screen",
	"Browse Screen",
	"Table Edit"};
static char *startwhere_names[2]={
	"Top of Browse",
	"Bottom of Browse"};
static MaintainButton *HoldButtons=NULL;
static ButtonField *HoldButtonField=NULL;
PowerWrite *tempNewWrites=NULL,*tempPrevWrites=NULL;
int tmpnumActions=0,tmpnumNewWrites=0,tmpnumPrevWrites=0;
MTNAction *tempActions=NULL;
APPlib *newwrtavl=NULL,*prevwrtavl=NULL;
RDAsearch *tempsupport=NULL;
int tmpnumsupp=0;
RDAsubord *tempsubord=NULL;
int tmpnumsub=0;
APPlib *suppavlmtn=NULL,*subavlmtn=NULL,*ctypesmtn=NULL;
APPlib *filelstmtn=NULL,*cmodlstmtn=NULL,*subkeylistmtn=NULL,*cnfldsmtn=NULL;
void loadvalue(RDArsrc *,char *);
void makefieldvallistrpt(APPlib **,short,short,short,short);
APPlib *econfldsmtn=NULL;
char *contypemtn[3]={
	"Equal To and Check on Save",
	"Equal To Only",
	"Include File Only",
};
char *ActionTypeList[5]={
	"Expression Only",
	"Expression & Set Account Type",
        "Expression & Set Definition Type",
	"Set Account Type Only",
        "Set Definition Type Only"};

extern void saveaction(RDArsrc *,MaintainMaster *);
extern void savesupp(RDArsrc *,MaintainMaster *);
extern void savenewwrt(RDArsrc *,MaintainMaster *);
extern void saveprevwrt(RDArsrc *,MaintainMaster *);
extern void savesub(RDArsrc *,MaintainMaster *);

extern void getActions(void);
extern void actionedit(RDArsrc *);
extern void actioncopy(RDArsrc *),actionaddabove(RDArsrc *);
extern void actionaddbelow(RDArsrc *),actiondelete(RDArsrc *);
extern void suppedit(RDArsrc *,RDArsrc *),getsupports(void);
extern void suppaddbelow(RDArsrc *,RDArsrc *),suppaddabove(RDArsrc *,RDArsrc *);
extern void newwrtcopy(RDArsrc *),newwrtedit(RDArsrc *);
extern void getNewWrites(void);
extern void newwrtaddbelow(RDArsrc *),newwrtaddabove(RDArsrc *);
extern void newwrtdelete(RDArsrc *);
extern void prevwrtcopy(RDArsrc *),prevwrtedit(RDArsrc *);
extern void getPrevWrites(void);
extern void prevwrtaddbelow(RDArsrc *),prevwrtaddabove(RDArsrc *);
extern void prevwrtdelete(RDArsrc *);
extern void suppdelete(RDArsrc *);
extern void subedit(RDArsrc *,RDArsrc *),getsubords(void);
extern void subaddbelow(RDArsrc *,RDArsrc *),subaddabove(RDArsrc *,RDArsrc *);
extern void subdelete(RDArsrc *);
static void makebuttonlist(void);
static APPlib *makebtntypeslist(void);
static void makebtnfldlist(void);
static void makespacebuttons(int);
char *orgmodulename=NULL;
char *orgname=NULL;
APPlib *makefieldvallistmtn(MaintainMaster *);
APPlib *makefieldvallist2mtn(RDAsearch *,int,int,short,char *,char *);
void loadvaluemtn(RDArsrc *,char *);
static void procexcpscrn(RDArsrc *,RDAreport *);
APPlib *loadfilefieldlistA(RDArsrc *,char *,char *);


/*
APPlib *makesortfieldvallist(APPlib *field_list,RDAreport *report)
{
	int x=0,y=0,z=0,u=0,t=0;
	RDAaccum *a=NULL;
	RDAimage *i=NULL;
	RDApval *pval=NULL;
	RDAsort *s=NULL;
	NRDfield *f=NULL;
	char *temp=NULL,*fname=NULL;
	RDAvirtual *v=NULL;

	if(diagrptgen)
	{
		prterr("DIAG makesortfilefieldlist.");
	}
	if(APPlib==NULL)
	{
		field_list=APPlibNEW();
	}
	addAPPlib(field_list,"DELETEFLAG");
	addAPPlib(field_list,"[RECORD COUNTER]");
	for(x=0,a=report->accum;xreport->numaccums;++x,++a)
	{
		for(y=0,v=report->virflds;y<report->numvirtuals;++y,++v)
		{
			temp=stripmodulename(a->accname);
			if(!RDAstrcmp(v->name,temp)) break;
			if(temp!=NULL) Rfree(temp);
			temp=NULL;
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
		if((y<report->numvirtuals && !v->when) || y>=report->numvirtuals)
		{
			addAPPlib(field_list,a->accname);
		}
	}
	if(report->image!=NULL)
	{
		for(x=0,i=report->image;x<report->numimages;++x,++i)
		{
			if(i->pvals!=NULL)
			{
				for(y=0,pval=i->pvals;y<i->numpvals;++y,++pval)
				{
					if(pval->symbol_type==FIELDTYPE && 
						RDAstrcmp(pval->name,"[PAGE NO]")
						&& RDAstrcmp(pval->name,"[BREAK LEVEL]")
						&& RDAstrcmp(pval->name,"[BODY COUNT]")	
						&& RDAstrcmp(pval->name,"NEXT TRANSACTION NO")
						&& RDAstrcmp(pval->name,"[PRIMARY FILE NUMBER]")
						&& RDAstrcmp(pval->name,"[WRITE COUNT]")
						&& RDAstrcmp(pval->name,"[SYSTEM DATE]")
						&& RDAstrcmp(pval->name,"[SYSTEM DATE-YYYY]")
						&& RDAstrcmp(pval->name,"[LOGIN IDENTIFICATION]")
						&& RDAstrcmp(pval->name,"[SYSTEM TIME]")
						&& RDAstrcmp(pval->name,"[CURRENT DIRECTORY]")
						&& RDAstrcmp(pval->name,"[SYSTEM TYPE]")
						&& RDAstrcmp(pval->name,"[ORGANIZATION NAME]")
						&& RDAstrcmp(pval->name,"[ORGANIZATION ADDRESS #1]")
						&& RDAstrcmp(pval->name,"[ORGANIZATION ADDRESS #2]")
						&& RDAstrcmp(pval->name,"[ORGANIZATION CITY]")
						&& RDAstrcmp(pval->name,"[ORGANIZATION STATE]")
						&& RDAstrcmp(pval->name,"[ORGANIZATION ZIP]")
						&& RDAstrcmp(pval->name,"[ORGANIZATION PHONE]"))
					{
						for(u=0,a=report->accum;u<report->numaccums;++u,++a)
						{
							temp=stripmodulename(pval->name);
							if(!RDAstrcmp(a->name,temp)) break;
							if(temp!=NULL) Rfree(temp);
							temp=NULL;
						} 
						if(temp!=NULL) Rfree(temp);
						temp=NULL;
						for(t=0,v=report->virflds;t<report->numvirtuals;++t,++v)
						{
							temp=stripmodulename(pval->name);
							if(!RDAstrcmp(v->name,temp))
							{
								if(!v->when) t=report->numvirtuals+1;	
								break;
							}
							if(temp!=NULL) Rfree(temp);
							temp=NULL;
						}
						if(temp!=NULL) Rfree(temp);
						temp=NULL;
						if(u>=report->numaccums && t>=report->numvirtuals)
						{
							addAPPlib(field_list,pval->name);
						}
					}
				}
			}
		}
	}
	if(report->sort!=NULL)
	{
		for(x=0,s=report->sort;x<report->numsorts;++x,++s)
		{
			switch(s->type)
			{
				default:
				case ASCENDING:
					addAPPlib(field_list,s->name);
					break;
				case ASCENDLOWER:
					addAPPlib(field_list,s->name);
					break;
				case ASCENDUPPER:
					addAPPlib(field_list,s->name);
					break;
				case DESCENDING:
					addAPPlib(field_list,s->name);
					break;
				case DESCENDLOWER:
					temp=Rmalloc(RDAstrlen(s->name)+7);
					sprintf(temp,"%s[DESCEND LOWER]",s->name);
					addAPPlib(field_list,s->name);
					if(temp!=NULL) Rfree(temp);
					break;
				case DESCENDUPPER:
					temp=Rmalloc(RDAstrlen(s->name)+7);
					sprintf(temp,"%s[DESCEND UPPER]",s->name);
					addAPPlib(field_list,s->name);
					if(temp!=NULL) Rfree(temp);
					break;
			}
		}
	}
}
*/
void makefieldvallistrpt(APPlib **list,short dofilefields,short doglobals,short virtuals,short accums)
{
	int x,y,z;
	RDAvirtual *v=NULL;
	RDAsearch *s=NULL;
	NRDfield *fields=NULL,*f=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part=NULL;
	short nofields=0,nokeys=0,engine=0;
	char *temp=NULL;
	RDAaccum *a=NULL;
	RDAdisplay *d=NULL;

	if(list==NULL)
	{
		*list=APPlibNEW();
	}
	if(virtuals)
	{
		addAPPlibNoDuplicates(*list,"[RECORD COUNTER]");
		if(report->virflds!=NULL)
		{
			for(x=0,v=report->virflds;x<report->numvirtuals;++x,++v)
			{
				if(v->name!=NULL)
				{
					if(temp!=NULL)
					{
						temp=Rrealloc(temp,RDAstrlen(v->name)+3);
					} else {
						temp=Rmalloc(RDAstrlen(v->name)+3);
					}
					sprintf(temp,"[%s]",v->name);
					addAPPlibNoDuplicates(*list,temp);
				}
			}
		}
	}
	if(accums)
	{
		if(report->accum!=NULL && accums)
		{
			for(x=0,a=report->accum;x<report->numaccums;++x,++a)
			{
				if(a->name!=NULL)
				{
					if(temp!=NULL)
					{
						temp=Rrealloc(temp,RDAstrlen(a->name)+3);
					} else {
						temp=Rmalloc(RDAstrlen(a->name)+3);
					}
					sprintf(temp,"[%s]",a->name);
					addAPPlibNoDuplicates(*list,temp);
				}
			}
		}
	}
	if(doglobals)
	{
		d=report->display;
		if(d!=NULL && (d->device>2 && d->device<5)) addAPPlibNoDuplicates(*list,"NEXT TRANSACTION NO");
		addAPPlibNoDuplicates(*list,"[PAGE NO]");
		addAPPlibNoDuplicates(*list,"[BODY COUNT]");
		addAPPlibNoDuplicates(*list,"[BODY COUNT TYPE]");
		addAPPlibNoDuplicates(*list,"[BODY COUNT LIMIT]");
		addAPPlibNoDuplicates(*list,"[PAGE LENGTH LIMIT]");
		addAPPlibNoDuplicates(*list,"[PAGE WIDTH LIMIT]");
		addAPPlibNoDuplicates(*list,"[PAGE COUNTER]");
		addAPPlibNoDuplicates(*list,"[NUMBER OF LINES]");
		addAPPlibNoDuplicates(*list,"[STARTING PAGE NO]");
		addAPPlibNoDuplicates(*list,"[DEVICE NAME]");
		addAPPlibNoDuplicates(*list,"[DEVICE NAME EVAL]");
		addAPPlibNoDuplicates(*list,"[BREAK LEVEL]");
		addAPPlibNoDuplicates(*list,"[DETAIL COUNT]");
		addAPPlibNoDuplicates(*list,"[SUBORDINATE WRITE COUNT]");
		addAPPlibNoDuplicates(*list,"[PREVIOUS BREAK LEVEL]");
		addAPPlibNoDuplicates(*list,"[PREVIOUS BREAK LEVEL 2]");
		addAPPlibNoDuplicates(*list,"[PRIMARY FILE NUMBER]");
		addAPPlibNoDuplicates(*list,"[WRITE COUNT]");
		addAPPlibNoDuplicates(*list,"[TOTAL COUNTER]");
		addAPPlibNoDuplicates(*list,"[CURRENT FONT PITCH]");
		addAPPlibNoDuplicates(*list,"[CURRENT FONT POINTS]");
		addAPPlibNoDuplicates(*list,"[CURRENT FONT]");
		addAPPlibNoDuplicates(*list,"[FONT PITCH]");
		addAPPlibNoDuplicates(*list,"[FONT POINTS]");
		addAPPlibNoDuplicates(*list,"[LONGEST FONT]");
		loadglobals(*list);
	}
	if(dofilefields)
	{
		for(x=0,s=report->search;x<report->numsearchs;++x,++s)
		{
			if(!GETBIN(s->module,s->filename,&engine,&nofields,&fields,
				&nokeys,&keys)) 
			{
				if(fields!=NULL)
				{
					for(y=0,f=fields;y<nofields;++y,++f)
					{
						if(temp!=NULL)
						{
							temp=Rrealloc(temp,RDAstrlen(f->name)+RDAstrlen(s->module)+RDAstrlen(s->filename)+7);
						} else {
							temp=Rmalloc(RDAstrlen(f->name)+RDAstrlen(s->module)+RDAstrlen(s->filename)+7);
						}
						sprintf(temp,"[%s][%s][%s]",s->module,s->filename,f->name);
						addAPPlibNoDuplicates(*list,temp);
						if(f->name!=NULL) Rfree(f->name);
					}
					Rfree(fields);
				}
				if(keys!=NULL)
				{
					for(y=0,keyx=keys;y<nokeys;++y,++keyx)
					{
						if(keyx->name!=NULL) Rfree(keyx->name);
						if(keyx->part!=NULL)
						{
							for(z=0,part=keyx->part;z<keyx->numparts;++z,++part)
							{
								if(part->name!=NULL) Rfree(part->name);
							}
							Rfree(keyx->part);
						}
					}
					Rfree(keys);
				}
			}
		}
	}
	if(temp!=NULL) Rfree(temp);
	SORTAPPlib(*list);
}
void loadvalue(RDArsrc *r,char *fieldname)
{
	int x;
	LoadValue *load;

	ldvalrsrc1=RDArsrcNEW(module,"LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc1,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	loadlist=APPlibNEW();
	makefieldvallistrpt(&loadlist,TRUE,TRUE,TRUE,TRUE);
	if(loadlist->numlibs<1)
	{
		addAPPlib(loadlist,"Load List Unavailable");
	}
	x=0;
	if(loadlist->numlibs>1)
	{
		for(x=0;x<loadlist->numlibs;++x)
		{
			if(strncmp(loadlist->libs[x],"GENERIC_SETUP",13)) break;
		}
	}
	addlstrsrc(ldvalrsrc1,"LOAD LIST",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addrfkwrsrc(ldvalrsrc1,"SELECT",TRUE,okldval,load);
	addrfkwrsrc(ldvalrsrc1,"QUIT",TRUE,quitldval,load);
	addbtnrsrc(ldvalrsrc1,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ldvalrsrc1,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(ldvalrsrc1,TRUE,quitldval,load,FALSE);
}
void RPTimage(RDArsrc *mtnrsrc)
{
	RDAdisplay *d=NULL;

	d=report->display;
	if(d!=NULL)
	{
		if(d->device>2 && d->device<5) 
		{
			RPTimage2(mtnrsrc);
		} else {
			RPTimage1(mtnrsrc);
		}
	} else {
		RPTimage1(mtnrsrc);
	}
}
void quitdsp(RDArsrc *dsprsrc)
{
	if(dsprsrc!=NULL)
	{
		killwindow(dsprsrc);
		free_rsrc(dsprsrc);
	}
	if(devtypes!=NULL) freeapplib(devtypes);
	if(dumprttypes!=NULL) freeapplib(dumprttypes);
	if(devvars!=NULL) freeapplib(devvars);
	if(changeddisplay) changedreport=TRUE;
	if(dspdefaults!=NULL) FreeRDAdefault(dspdefaults);
}
void quitdspclr(RDArsrc *dsprsrc)
{
	changeddisplay=FALSE;
	quitdsp(dsprsrc);
}
void savedsp(RDArsrc *dsprsrc)
{
	RDAdisplay *d;
	int x=0;
	RDApvar *pvar,*pvar1;
	char print_alignment=FALSE,ph=FALSE;
	int imageoff=0;
	RDAimage *image=NULL;
	char *temp1=NULL;

	readallwidgets(dsprsrc);
	d=report->display;
	if(d==NULL) 
	{
		d=Rmalloc(sizeof(RDAdisplay));
		d->device=0;
		d->page_length=10;
		d->page_width=7.5;
		d->body_count=0;
		d->body_count_type=FALSE;
		d->doscopy=FALSE;
		d->device_spec=NULL;
		d->test_pages=0;
		d->set_lpp=NULL;
		d->set_lpp_eval=FALSE;
		d->starting_page=1;
		d->pvars=NULL;
		d->num=0;
		d->dumprinter=TRUE;
		d->dumprinter_type=TRUE;
		report->display=d;
	}
	FINDRSCGETINT(dsprsrc,"DEVICE TYPES",&x);
	d->device=(short)x;
	FINDRSCGETDOUBLE(dsprsrc,"PAGE WIDTH",&d->page_width);
	FINDRSCGETDOUBLE(dsprsrc,"PAGE LENGTH",&d->page_length);
	FINDRSCGETSHORT(dsprsrc,"BODY COUNT",&d->body_count);
	FINDRSCGETCHAR(dsprsrc,"BODY COUNT TYPE",&d->body_count_type);
	FINDRSCGETCHAR(dsprsrc,"COUNT INVISIBLE",&d->count_invisible);
	FINDRSCGETCHAR(dsprsrc,"DOS COPY",&d->doscopy);
	FINDRSCGETSTRING(dsprsrc,"DEVICE SPECIFICATION",&d->device_spec);
	FINDRSCGETSHORT(dsprsrc,"STARTING PAGE NO",&d->starting_page);
	FINDRSCGETSHORT(dsprsrc,"NUMBER TEST PAGES",&d->test_pages);
	FINDRSCGETSTRING(dsprsrc,"DEVICE NAME",&d->set_lpp);
	FINDRSCGETCHAR(dsprsrc,"DEVICE NAME EVAL",&d->set_lpp_eval);
	FINDRSCGETSTRING(dsprsrc,"DEVICE SPECIFICATION",&d->device_spec);
	FINDRSCGETCHAR(dsprsrc,"SIMPLE PRINTER",&d->dumprinter);
	FINDRSCGETINT(dsprsrc,"SIMPLE PRINTER TYPES",&x);
	FINDRSCGETCHAR(dsprsrc,"MAKE PAGE HEADER",&ph);
	d->dumprinter_type=(short)x;
	FINDRSCGETCHAR(dsprsrc,"ALIGNMENT TEST",&print_alignment);
	d->date_order=(short)print_alignment;
	if(d->pvars!=NULL)
	{
		for(x=0,pvar=d->pvars;x<d->num;++x,++pvar)
		{
			if(pvar->command!=NULL) Rfree(pvar->command);
			if(pvar->name!=NULL) Rfree(pvar->name);
		}
		Rfree(d->pvars);
		d->num=0;
	}
	if(holdpvar!=NULL)
	{
		d->num=holdpnum;
		d->pvars=Rmalloc(d->num*sizeof(RDApvar));
		for(x=0,pvar=d->pvars,pvar1=holdpvar;x<holdpnum;++x,++pvar,++pvar1)
		{
			if(pvar1->name!=NULL)
			{
				pvar->name=stralloc(pvar1->name);
				Rfree(pvar1->name);
			} else pvar->name=NULL;
			if(pvar1->command!=NULL)
			{
				pvar->command=stralloc(pvar1->command);
				Rfree(pvar1->command);
			} else pvar->command=NULL;
			pvar->chgsize=pvar1->chgsize;
			pvar->pitch=pvar1->pitch;
			pvar->points=pvar1->points;
		}
		Rfree(holdpvar);
		holdpnum=0;
	}
	if(ph==TRUE && report->image==NULL && (d->device==0 || d->device==5) && d->dumprinter==TRUE)
	{
		/* EXECUTED BY VIRTUAL FIELD */
		temp1=Rmalloc(42+1);
		sprintf(temp1,"%s",
			"(\"Executed By: \"+[LOGIN IDENTIFICATION])");
		addvirtuallineADV(report,"EXECUTED BY",1,60,temp1,1,0);
		if(temp1!=NULL) Rfree(temp1);


		/* REPORT TITLE VIRTUAL FIELD */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		addvirtuallineADV(report,"REPORT TITLE",1,60,temp1,1,0);
		if(temp1!=NULL) Rfree(temp1);


		imageoff=addimageline(report,"PAGE HEADER","@@",FALSE,NULL,0,NULL);
		if(imageoff>0)
		{
			image=report->image+(imageoff-1);
			addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
			addimagepval(image,1,"LINEFEED",0,NULL);
		}
		if(d->dumprinter_type==0 && ((d->page_width>=7.5) && (d->page_width<=8.5)))
		{
			imageoff=addimageline(report,"PAGE HEADER","123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*@@",TRUE,"FALSE",0,NULL);
			if(imageoff>0)
			{
				image=report->image+(imageoff-1);
				addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
				addimagepval(image,1,"LINEFEED",0,NULL);
			}
			imageoff=addimageline(report,"PAGE HEADER","                 @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC   PAGE: @RRRRRRR@@",FALSE,NULL,0,NULL);
			if(imageoff>0)
			{
				image=report->image+(imageoff-1);
				addimagepval(image,0,"[ORGANIZATION NAME]",0,NULL);
				addimagepval(image,0,"[PAGE NO]",0,NULL);
				addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
				addimagepval(image,1,"LINEFEED",0,NULL);
			}
			imageoff=addimageline(report,"PAGE HEADER","                 @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC   DATE: @LLLLLLL@@",FALSE,NULL,0,NULL);
			if(imageoff>0)
			{
				image=report->image+(imageoff-1);
				addimagepval(image,0,"[REPORT TITLE]",0,NULL);
				addimagepval(image,0,"[SYSTEM DATE]",0,NULL);
				addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
				addimagepval(image,1,"LINEFEED",0,NULL);
			}
			imageoff=addimageline(report,"PAGE HEADER","                 @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC   TIME: @LLLLLLL@@",FALSE,NULL,0,NULL);
			if(imageoff>0)
			{
				image=report->image+(imageoff-1);
				addimagepval(image,0,"[EXECUTED BY]",0,NULL);
				addimagepval(image,0,"[SYSTEM TIME]",0,NULL);
				addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
				addimagepval(image,1,"LINEFEED",0,NULL);
			}
		} else if((d->dumprinter_type==0 || d->dumprinter_type==2) && (d->page_width>=10.0) && (d->page_width<=11.0))
		{
			imageoff=addimageline(report,"PAGE HEADER","123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*@@",TRUE,"FALSE",0,NULL);
			if(imageoff>0)
			{
				image=report->image+(imageoff-1);
				addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
				addimagepval(image,1,"LINEFEED",0,NULL);
			}
			imageoff=addimageline(report,"PAGE HEADER","                 @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC   PAGE: @RRRRRRR@@",FALSE,NULL,0,NULL);
			if(imageoff>0)
			{
				image=report->image+(imageoff-1);
				addimagepval(image,0,"[ORGANIZATION NAME]",0,NULL);
				addimagepval(image,0,"[PAGE NO]",0,NULL);
				addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
				addimagepval(image,1,"LINEFEED",0,NULL);
			}
			imageoff=addimageline(report,"PAGE HEADER","                 @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC   DATE: @LLLLLLL@@",FALSE,NULL,0,NULL);
			if(imageoff>0)
			{
				image=report->image+(imageoff-1);
				addimagepval(image,0,"[REPORT TITLE]",0,NULL);
				addimagepval(image,0,"[SYSTEM DATE]",0,NULL);
				addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
				addimagepval(image,1,"LINEFEED",0,NULL);
			}
			imageoff=addimageline(report,"PAGE HEADER","                 @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC   TIME: @LLLLLLL@@",FALSE,NULL,0,NULL);
			if(imageoff>0)
			{
				image=report->image+(imageoff-1);
				addimagepval(image,0,"[EXECUTED BY]",0,NULL);
				addimagepval(image,0,"[SYSTEM TIME]",0,NULL);
				addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
				addimagepval(image,1,"LINEFEED",0,NULL);
			}
		} else if((d->dumprinter_type==1 || d->dumprinter_type==3) && (d->page_width>=7.5) && (d->page_width<=8.5))
		{
			imageoff=addimageline(report,"PAGE HEADER","123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*12@@",TRUE,"FALSE",0,NULL);
			if(imageoff>0)
			{
				image=report->image+(imageoff-1);
				addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
				addimagepval(image,1,"LINEFEED",0,NULL);
			}
			imageoff=addimageline(report,"PAGE HEADER","                 @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC   PAGE: @RRRRRRR@@",FALSE,NULL,0,NULL);
			if(imageoff>0)
			{
				image=report->image+(imageoff-1);
				addimagepval(image,0,"[ORGANIZATION NAME]",0,NULL);
				addimagepval(image,0,"[PAGE NO]",0,NULL);
				addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
				addimagepval(image,1,"LINEFEED",0,NULL);
			}
			imageoff=addimageline(report,"PAGE HEADER","                 @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC   DATE: @LLLLLLL@@",FALSE,NULL,0,NULL);
			if(imageoff>0)
			{
				image=report->image+(imageoff-1);
				addimagepval(image,0,"[REPORT TITLE]",0,NULL);
				addimagepval(image,0,"[SYSTEM DATE]",0,NULL);
				addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
				addimagepval(image,1,"LINEFEED",0,NULL);
			}
			imageoff=addimageline(report,"PAGE HEADER","                 @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC   TIME: @LLLLLLL@@",FALSE,NULL,0,NULL);
			if(imageoff>0)
			{
				image=report->image+(imageoff-1);
				addimagepval(image,0,"[EXECUTED BY]",0,NULL);
				addimagepval(image,0,"[SYSTEM TIME]",0,NULL);
				addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
				addimagepval(image,1,"LINEFEED",0,NULL);
			}
		} else if(d->dumprinter_type==3 && (d->page_width>=10.0 && d->page_width<=11.0))
		{
			imageoff=addimageline(report,"PAGE HEADER","123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*1234567@@",TRUE,"FALSE",0,NULL);
			if(imageoff>0)
			{
				image=report->image+(imageoff-1);
				addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
				addimagepval(image,1,"LINEFEED",0,NULL);
			}
			imageoff=addimageline(report,"PAGE HEADER","                 @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC   PAGE: @RRRRRRR@@",FALSE,NULL,0,NULL);
			if(imageoff>0)
			{
				image=report->image+(imageoff-1);
				addimagepval(image,0,"[ORGANIZATION NAME]",0,NULL);
				addimagepval(image,0,"[PAGE NO]",0,NULL);
				addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
				addimagepval(image,1,"LINEFEED",0,NULL);
			}
			imageoff=addimageline(report,"PAGE HEADER","                 @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC   DATE: @LLLLLLL@@",FALSE,NULL,0,NULL);
			if(imageoff>0)
			{
				image=report->image+(imageoff-1);
				addimagepval(image,0,"[REPORT TITLE]",0,NULL);
				addimagepval(image,0,"[SYSTEM DATE]",0,NULL);
				addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
				addimagepval(image,1,"LINEFEED",0,NULL);
			}
			imageoff=addimageline(report,"PAGE HEADER","                 @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC   TIME: @LLLLLLL@@",FALSE,NULL,0,NULL);
			if(imageoff>0)
			{
				image=report->image+(imageoff-1);
				addimagepval(image,0,"[EXECUTED BY]",0,NULL);
				addimagepval(image,0,"[SYSTEM TIME]",0,NULL);
				addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
				addimagepval(image,1,"LINEFEED",0,NULL);
			}
		} else if(d->dumprinter_type==3 && (d->page_width>=13.0 && d->page_width<=14.0))
		{
			imageoff=addimageline(report,"PAGE HEADER","123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789*12345678@@",TRUE,"FALSE",0,NULL);
			if(imageoff>0)
			{
				image=report->image+(imageoff-1);
				addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
				addimagepval(image,1,"LINEFEED",0,NULL);
			}
			imageoff=addimageline(report,"PAGE HEADER","                 @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC   PAGE: @RRRRRRR@@",FALSE,NULL,0,NULL);
			if(imageoff>0)
			{
				image=report->image+(imageoff-1);
				addimagepval(image,0,"[ORGANIZATION NAME]",0,NULL);
				addimagepval(image,0,"[PAGE NO]",0,NULL);
				addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
				addimagepval(image,1,"LINEFEED",0,NULL);
			}
			imageoff=addimageline(report,"PAGE HEADER","                 @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC   DATE: @LLLLLLL@@",FALSE,NULL,0,NULL);
			if(imageoff>0)
			{
				image=report->image+(imageoff-1);
				addimagepval(image,0,"[REPORT TITLE]",0,NULL);
				addimagepval(image,0,"[SYSTEM DATE]",0,NULL);
				addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
				addimagepval(image,1,"LINEFEED",0,NULL);
			}
			imageoff=addimageline(report,"PAGE HEADER","                 @CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC   TIME: @LLLLLLL@@",FALSE,NULL,0,NULL);
			if(imageoff>0)
			{
				image=report->image+(imageoff-1);
				addimagepval(image,0,"[EXECUTED BY]",0,NULL);
				addimagepval(image,0,"[SYSTEM TIME]",0,NULL);
				addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
				addimagepval(image,1,"LINEFEED",0,NULL);
			}
		}


		imageoff=addimageline(report,"PAGE HEADER","@@",FALSE,NULL,0,NULL);
		if(imageoff>0)
		{
			image=report->image+(imageoff-1);
			addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
			addimagepval(image,1,"LINEFEED",0,NULL);
		}
		imageoff=addimageline(report,"PAGE FOOTER","@@@",FALSE,NULL,0,NULL);
		if(imageoff>0)
		{
			image=report->image+(imageoff-1);
			addimagepval(image,1,"CARRIAGE RETURN",0,NULL);
			addimagepval(image,1,"LINEFEED",0,NULL);
			addimagepval(image,1,"FORMFEED",0,NULL);
		}
	}
	if(COMPARE_RSRCS(dsprsrc,dspdefaults,2))
	{
		changeddisplay=TRUE;
	}
	quitdsp(dsprsrc);
}
void quitdsptest(RDArsrc *dsprsrc)
{
	readallwidgets(dsprsrc);
	if(COMPARE_RSRCS(dsprsrc,dspdefaults,2))
	{
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","REPORT CHANGES NOT SAVED!","Changes have been made to this DISPLAY DEFININTION!\nDo you want to Save these changes?",savedsp,quitdspclr,FALSE,dsprsrc);
	} else {
		quitdsp(dsprsrc);
	}
}
void setdoscopy(RDArsrc *dsprsrc)
{
	char doscopy=FALSE;

	readwidget(dsprsrc,"DOS COPY");
	if(FINDRSCGETCHAR(dsprsrc,"DOS COPY",&doscopy)) return;
	if(doscopy==FALSE)
	{
		FINDRSCSETSENSITIVE(dsprsrc,"DEVICE SPECIFICATION",FALSE);
		FINDRSCSETEDITABLE(dsprsrc,"DEVICE SPECIFICATION",FALSE);
	} else {
		FINDRSCSETSENSITIVE(dsprsrc,"DEVICE SPECIFICATION",TRUE);
		FINDRSCSETEDITABLE(dsprsrc,"DEVICE SPECIFICATION",TRUE);
	}
}
void setdevvars(RDArsrc *dsprsrc)
{
	int y,seldv=0;
	char *outdev=NULL;
	char outdeveval=FALSE;
	RDApdev *pdev=NULL;
	RDApvar *pvar=NULL,*pv=NULL;
	char dumprinter=FALSE;

	readwidget(dsprsrc,"SIMPLE PRINTER");
	if(FINDRSCGETCHAR(dsprsrc,"SIMPLE PRINTER",&dumprinter)) return;
	if(FINDRSCGETINT(dsprsrc,"DEVICE VARIABLES",&seldv)) return;
	if(FINDRSCGETINT(dsprsrc,"DEVICE VARIABLES",&seldv)) return;
	if(devvars!=NULL) freeapplib(devvars);
	devvars=APPlibNEW();
	readwidget(dsprsrc,"DEVICE NAME");
	readwidget(dsprsrc,"DEVICE NAME EVAL");
	if(FINDRSCGETCHAR(dsprsrc,"DEVICE NAME EVAL",&outdeveval)) return;
	if(FINDRSCGETSTRING(dsprsrc,"DEVICE NAME",&outdev)) return;
	pdev=RDApdevNEW(outdev);
	if(holdpvar!=NULL)
	{
		for(y=0,pvar=holdpvar;y<holdpnum;++y,++pvar) 
		{
			if(pvar->name!=NULL) Rfree(pvar->name);
			if(pvar->command!=NULL) Rfree(pvar->command);
		}
		Rfree(holdpvar);
		holdpvar=NULL;
		holdpnum=0;
	}
	if(dumprinter==FALSE)
	{
		FINDRSCSETSENSITIVE(dsprsrc,"DEVICE NAME",TRUE);
		FINDRSCSETEDITABLE(dsprsrc,"DEVICE NAME",TRUE);
		FINDRSCSETSENSITIVE(dsprsrc,"DEVICE NAME EVAL",TRUE);
		FINDRSCSETEDITABLE(dsprsrc,"DEVICE NAME EVAL",TRUE);
		if(!getpdevbin(pdev))
		{
			holdpnum=pdev->num;
			holdpvar=Rmalloc(holdpnum*sizeof(RDApvar));
			for(y=0,pvar=holdpvar,pv=pdev->pvar;y<holdpnum;++y,++pvar,++pv) 
			{
				pvar->name=stralloc(pv->name);
				pvar->command=stralloc(pv->command);
				pvar->chgsize=pv->chgsize;
				pvar->pitch=pv->pitch;
				pvar->points=pv->points;
				addAPPlib(devvars,pvar->name);
			}
		}
	} else {
		FINDRSCSETSENSITIVE(dsprsrc,"DEVICE NAME",FALSE);
		FINDRSCSETEDITABLE(dsprsrc,"DEVICE NAME",FALSE);
		FINDRSCSETSENSITIVE(dsprsrc,"DEVICE NAME EVAL",FALSE);
		FINDRSCSETEDITABLE(dsprsrc,"DEVICE NAME EVAL",FALSE);
	}
	free_pdev(pdev);
	if(outdev!=NULL) Rfree(outdev);
	if(devvars->numlibs<1)
	{
		addAPPlib(devvars,"No Device Variables Available");
	}
	if(seldv>=devvars->numlibs) seldv=0;
	if(!FINDRSCLISTAPPlib(dsprsrc,"DEVICE VARIABLES",seldv,devvars))
		updatersrc(dsprsrc,"DEVICE VARIABLES");
}
void setdeveval(RDArsrc *dsprsrc)
{
	char *outdev=NULL;
	char outdeveval=FALSE;

	readwidget(dsprsrc,"DEVICE NAME EVAL");
	if(FINDRSCGETCHAR(dsprsrc,"DEVICE NAME EVAL",&outdeveval)) return;
	if(FINDRSCGETSTRING(dsprsrc,"DEVICE NAME",&outdev)) return;
	if(outdeveval==FALSE)
	{
		FINDRSCSETSENSITIVE(dsprsrc,"LOAD VALUE DEVICE NAME",FALSE);
		FINDRSCSETEDITABLE(dsprsrc,"LOAD VALUE DEVICE NAME",FALSE);
		FINDRSCSETSENSITIVE(dsprsrc,"LOAD OPERATOR DEVICE NAME",FALSE);
		FINDRSCSETEDITABLE(dsprsrc,"LOAD OPERATOR DEVICE NAME",FALSE);
		FINDRSCSETSENSITIVE(dsprsrc,"LOAD GROUPER DEVICE NAME",FALSE);
		FINDRSCSETEDITABLE(dsprsrc,"LOAD GROUPER DEVICE NAME",FALSE);
		FINDRSCSETSENSITIVE(dsprsrc,"LOAD CONTROL DEVICE NAME",FALSE);
		FINDRSCSETEDITABLE(dsprsrc,"LOAD CONTROL DEVICE NAME",FALSE);
		FINDRSCSETSENSITIVE(dsprsrc,"DEVICE NAME",TRUE);
		FINDRSCSETEDITABLE(dsprsrc,"DEVICE NAME",TRUE);
		FINDRSCSETSENSITIVE(dsprsrc,"DEVICE NAME EVAL",TRUE);
		FINDRSCSETEDITABLE(dsprsrc,"DEVICE NAME EVAL",TRUE);
	} else {
		FINDRSCSETSENSITIVE(dsprsrc,"LOAD VALUE DEVICE NAME",TRUE);
		FINDRSCSETEDITABLE(dsprsrc,"LOAD VALUE DEVICE NAME",TRUE);
		FINDRSCSETSENSITIVE(dsprsrc,"LOAD OPERATOR DEVICE NAME",TRUE);
		FINDRSCSETEDITABLE(dsprsrc,"LOAD OPERATOR DEVICE NAME",TRUE);
		FINDRSCSETSENSITIVE(dsprsrc,"LOAD GROUPER DEVICE NAME",TRUE);
		FINDRSCSETEDITABLE(dsprsrc,"LOAD GROUPER DEVICE NAME",TRUE);
		FINDRSCSETSENSITIVE(dsprsrc,"LOAD CONTROL DEVICE NAME",TRUE);
		FINDRSCSETEDITABLE(dsprsrc,"LOAD CONTROL DEVICE NAME",TRUE);
		FINDRSCSETSENSITIVE(dsprsrc,"DEVICE NAME",TRUE);
		FINDRSCSETEDITABLE(dsprsrc,"DEVICE NAME",TRUE);
		FINDRSCSETSENSITIVE(dsprsrc,"DEVICE NAME EVAL",TRUE);
		FINDRSCSETEDITABLE(dsprsrc,"DEVICE NAME EVAL",TRUE);
	}
	if(outdev!=NULL) Rfree(outdev);
}
void RPTdisplay(RDArsrc *rptrsrc)
{
	short x=0;
	int y=0;
	RDAdisplay *d;
	/* RDApvar *pvar,*pvar1; */
	char print_alignment=FALSE,ph=FALSE;
	char dumprinter=FALSE;
	RDArsrc *dsprsrc=NULL;

	dsprsrc=RDArsrcNEW(module,"DEFINE DISPLAY"); 
	if(devtypes!=NULL) freeapplib(devtypes);
	devtypes=APPlibNEW();
	for(x=0;x<6;++x) addAPPlib(devtypes,dvtypes[x]);

	if(dumprttypes!=NULL) freeapplib(dumprttypes);
	dumprttypes=APPlibNEW();
	addAPPlib(dumprttypes,"NORMAL TEXT (DUPLEX)");
	addAPPlib(dumprttypes,"COMPRESSED TEXT (DUPLEX)");
	addAPPlib(dumprttypes,"LANDSCAPE NORMAL TEXT (DUPLEX)");
	addAPPlib(dumprttypes,"LANDSCAPE COMPRESSED TEXT (DUPLEX)");
	addAPPlib(dumprttypes,"NORMAL TEXT");
	addAPPlib(dumprttypes,"COMPRESSED TEXT");
	addAPPlib(dumprttypes,"LANDSCAPE NORMAL TEXT");
	addAPPlib(dumprttypes,"LANDSCAPE COMPRESSED TEXT");

	d=report->display;
	if(d==NULL) 
	{
		d=Rmalloc(sizeof(RDAdisplay));
		d->device=0;
		d->page_length=10;
		d->page_width=7.5;
		d->body_count=0;
		d->body_count_type=FALSE;
		d->doscopy=FALSE;
		d->device_spec=NULL;
		d->date_order=FALSE;
		d->test_pages=0;
		d->set_lpp=NULL;
		d->set_lpp_eval=FALSE;
		d->starting_page=1;
		d->pvars=NULL;
		d->num=0;
		d->dumprinter=TRUE;
		d->dumprinter_type=0;
		report->display=d;
	}
	holdpnum=d->num;
	if(devvars!=NULL) freeapplib(devvars);
	devvars=APPlibNEW();
/*
	if(d->num>0)
	{
		holdpvar=Rmalloc(holdpnum*sizeof(RDApvar));
		for(x=0,pvar1=holdpvar,pvar=d->pvars;x<d->num;++x,++pvar,++pvar1)
		{
			pvar1->name=stralloc(pvar->name);
			pvar1->command=stralloc(pvar->command);
			pvar1->chgsize=pvar->chgsize;
			pvar1->pitch=pvar->pitch;
			pvar1->points=pvar->points;
			addAPPlib(devvars,pvar->name);
		}
	}
	if(devvars->numlibs<1)
	{
		addAPPlib(devvars,"No Device Variables Available");
	}
*/
	y=d->device;
	addlstrsrc(dsprsrc,"DEVICE TYPES",&y,TRUE,NULL,devtypes->numlibs,
		&devtypes->libs,NULL);
	addstdrsrc(dsprsrc,"PAGE WIDTH",DECIMALV,8,&d->page_width,TRUE);
	addstdrsrc(dsprsrc,"PAGE LENGTH",DECIMALV,8,&d->page_length,TRUE);
	x=40;
	addstdrsrc(dsprsrc,"BODY COUNT",SHORTV,3,&d->body_count,TRUE);
	addstdrsrc(dsprsrc,"COUNT INVISIBLE",BOOLNS,1,&d->count_invisible,TRUE);
	addstdrsrc(dsprsrc,"MAKE PAGE HEADER",BOOLNS,1,&ph,TRUE);
	if(report->image!=NULL)
	{
		FINDRSCSETSENSITIVE(dsprsrc,"MAKE PAGE HEADER",FALSE);
	}
	addstdrsrc(dsprsrc,"DOS COPY",BOOLNS,1,&d->doscopy,TRUE);
	addstdrsrc(dsprsrc,"DEVICE SPECIFICATION",VARIABLETEXT,0,(d->device_spec
!=NULL?d->device_spec:""),TRUE);
	addstdrsrc(dsprsrc,"BODY COUNT TYPE",BOOLNS,3,&d->body_count_type,TRUE);
	x=1;
	if(d->dumprinter==TRUE) dumprinter=TRUE;
		else dumprinter=FALSE;
	addstdrsrc(dsprsrc,"SIMPLE PRINTER",BOOLNS,TRUE,&dumprinter,TRUE);
	y=(int)d->dumprinter_type;
	addlstrsrc(dsprsrc,"SIMPLE PRINTER TYPES",&y,TRUE,NULL,dumprttypes->numlibs,
		&dumprttypes->libs,NULL);
	if(d->date_order) print_alignment=TRUE;
		else print_alignment=FALSE;
	addstdrsrc(dsprsrc,"ALIGNMENT TEST",BOOLNS,1,&print_alignment,TRUE);
	addstdrsrc(dsprsrc,"NUMBER TEST PAGES",SHORTV,3,&d->test_pages,TRUE);
	x=1;
	addstdrsrc(dsprsrc,"STARTING PAGE NO",SHORTV,3,&d->starting_page,TRUE);
	addstdrsrc(dsprsrc,"DEVICE NAME",SCROLLEDTEXT,0,(d->set_lpp!=NULL ? d->set_lpp:""),TRUE);
	if(d->set_lpp_eval) print_alignment=TRUE;
		else print_alignment=FALSE;
	addstdrsrc(dsprsrc,"DEVICE NAME EVAL",BOOLNS,1,&print_alignment,TRUE);
	addbtnrsrc(dsprsrc,"LOAD VALUE DEVICE NAME",TRUE,loadvalue,"DEVICE NAME");
	addbtnrsrc(dsprsrc,"LOAD OPERATOR DEVICE NAME",TRUE,loadoperand,"DEVICE NAME");
	addbtnrsrc(dsprsrc,"LOAD GROUPER DEVICE NAME",TRUE,loadgrouper,"DEVICE NAME");
	addbtnrsrc(dsprsrc,"LOAD CONTROL DEVICE NAME",TRUE,loadcontrol,"DEVICE NAME");
	FINDRSCSETFUNC(dsprsrc,"DEVICE NAME",setdevvars,NULL);
	FINDRSCSETFUNC(dsprsrc,"DEVICE NAME EVAL",setdeveval,NULL);
	FINDRSCSETFUNC(dsprsrc,"DOS COPY",setdoscopy,NULL);
	FINDRSCSETFUNC(dsprsrc,"SIMPLE PRINTER",setdevvars,NULL);
	y=0;
	addlstrsrc(dsprsrc,"DEVICE VARIABLES",&y,TRUE,NULL,devvars->numlibs,
		&devvars->libs,NULL);
	setdevvars(dsprsrc);
	setdeveval(dsprsrc);
	setdoscopy(dsprsrc);
	if(!FINDRSCLISTAPPlib(dsprsrc,"DEVICE VARIABLES",0,devvars))
	{
		updatersrc(dsprsrc,"DEVICE VARIABLES");
	}
	y=0;
	addrfcbrsrc(dsprsrc,"SAVE",TRUE,savedsp,NULL);
	addrfcbrsrc(dsprsrc,"QUIT",TRUE,quitdsptest,NULL);
	addbtnrsrc(dsprsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(dsprsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	changeddisplay=FALSE;
	dspdefaults=GetDefaults(dsprsrc);
	APPmakescrn(dsprsrc,TRUE,quitdsp,NULL,FALSE);
}
void quitrpt(RDArsrc *rptrsrc)
{
	if(report!=NULL) FreeRDAreport(report);
	if(rptmodule!=NULL) Rfree(rptmodule);
	if(diaglevel!=NULL) freeapplib(diaglevel);
	if(rptrsrc!=NULL)
	{
		killwindow(rptrsrc);
		free_rsrc(rptrsrc);
	}
	if(rptdefaults!=NULL) FreeRDAdefault(rptdefaults);
}
void getrptmod(RDArsrc *rptrsrc)
{
	readwidget(rptrsrc,"REPORT MODULE");
	FINDRSCGETSTRING(rptrsrc,"REPORT MODULE",&rptmodule);
}
void saverpt(RDArsrc *rptrsrc,RDArsrc *mainrsrc)
{
	char holdc=0,default_range=FALSE,default_browse=FALSE;
	char default_searchbrowse=FALSE,default_definelist=FALSE;
	char default_maintain=FALSE,default_menu=FALSE;
	int holdi=0;
	int engine_type=0,x;
	RDAdisplay *d=NULL;
	
	readallwidgets(rptrsrc);
	FINDRSCGETSTRING(rptrsrc,"REPORT MODULE",&rptmodule);
	if(rptmodule!=NULL)
	{
		FINDRSCGETSTRING(rptrsrc,"REPORT NAME",&report->name);
		FINDRSCGETSTRING(rptrsrc,"TITLE",&report->title);
		FINDRSCGETSTRING(rptrsrc,"DESCRIPTION",&report->desc);
		FINDRSCGETINT(rptrsrc,"ENGINE TYPES",&engine_type);
		for(x=0;x<NE(supported_engines);++x)
		{
			if(!RDAstrcmp(supported_engines[x],dbengines->libs[engine_type]))
				break;
		}
		if(x<NE(supported_engines))
		{
			report->engine_type=x;
		} else report->engine_type=0;
		FINDRSCGETSTRING(rptrsrc,"SERVER NAME",&report->server_name);
		FINDRSCGETCHAR(rptrsrc,"USE PRINT SCREEN",&holdc);
		report->use_print_screen=(short)holdc;
		FINDRSCGETCHAR(rptrsrc,"STOP ON ERROR",&holdc);
		report->stop_on_error=(short)holdc;
		FINDRSCGETCHAR(rptrsrc,"NO EMPTY ERROR",&holdc);
		report->no_empty_error=(short)holdc;
		FINDRSCGETCHAR(rptrsrc,"NO WRITECOUNT TOTAL",&holdc);
		report->no_wc_diag=(short)holdc;
		FINDRSCGETCHAR(rptrsrc,"WRITECOUNT ONLY",&holdc);
		report->writecount_only=(short)holdc;
		FINDRSCGETCHAR(rptrsrc,"WRITE ALL DETAIL LEVELS",&holdc);
		report->write_all_detail_levels=(short)holdc;
		FINDRSCGETINT(rptrsrc,"DIAGNOSTIC LEVEL",&holdi);
		report->diag_breakable=(short)holdi;
		FINDRSCGETCHAR(rptrsrc,"PROCESS EXCEPTIONS",&holdc);
		report->process_exceptions=(short)holdc;
		FINDRSCGETCHAR(rptrsrc,"CHAIN REPORT",&holdc);
		report->chain_report=(short)holdc;
		FINDRSCGETSTRING(rptrsrc,"CHAIN MODULE",&report->chain_module);
		FINDRSCGETSTRING(rptrsrc,"CHAIN NAME",&report->chain_name);
		FINDRSCGETCHAR(rptrsrc,"REMOVE SORT",&holdc);
		report->removesort=(short)holdc;
		FINDRSCGETINT(rptrsrc,"REMOVE FILE",&holdi);
		report->remove_file=holdi;
		FINDRSCGETSTRING(rptrsrc,"REMOVE FILENAME",&report->remove_name);
		FINDRSCGETCHAR(rptrsrc,"RANGE SCREEN",&holdc);
		FINDRSCGETCHAR(rptrsrc,"DEFAULT RANGE SCREEN",&default_range);
		FINDRSCGETCHAR(rptrsrc,"DEFAULT BROWSE SCREEN",&default_browse);
		FINDRSCGETCHAR(rptrsrc,"DEFAULT SEARCH BROWSE",&default_searchbrowse);
		FINDRSCGETCHAR(rptrsrc,"DEFAULT DEFINE LIST",&default_definelist);
		FINDRSCGETCHAR(rptrsrc,"DEFAULT MAINTAIN SCREEN",&default_maintain);
		FINDRSCGETCHAR(rptrsrc,"DEFAULT MENU",&default_menu);
		report->range_screen=(short)holdc;
		FINDRSCGETCHAR(rptrsrc,"ASK BEFORE EXECUTE",&holdc);
		report->ask_b4_execute=(short)holdc;
		FINDRSCGETCHAR(rptrsrc,"MANUAL FORM CONTROL",&holdc);
		report->manual_form=(short)holdc;
		FINDRSCGETCHAR(rptrsrc,"REPORT COMPLETION",&holdc);
		report->report_completion=(short)holdc;
		d=report->display;
		if(d==NULL) 
		{
			d=Rmalloc(sizeof(RDAdisplay));
			d->device=0;
			d->page_length=10;
			d->page_width=7.5;
			d->body_count=0;
			d->body_count_type=FALSE;
			d->doscopy=FALSE;
			d->device_spec=NULL;
			d->date_order=FALSE;
			d->test_pages=0;
			d->set_lpp=NULL;
			d->set_lpp_eval=FALSE;
			d->starting_page=1;
			d->pvars=NULL;
			d->num=0;
			d->dumprinter=TRUE;
			d->dumprinter_type=1;
			report->display=d;
		}
		if(WRTRDAreport(rptmodule,report))
		{
			ERRORDIALOG("Cannot Write Report","Error in permissions, users cannot change the report.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
		}
		if(default_range) makeRangeScreen(rptmodule,report);
		if(default_browse) makeRPTBrowseScreen(rptmodule,report);
		if(default_searchbrowse) makeRPTSearchBrowse(rptmodule,report);
		if(default_definelist) makeRPTDefineList(rptmodule,report);
		if(default_maintain) makeRPTMaintainScreen(rptmodule,report);
		if(default_menu) makeDefaultMenu(rptmodule,report);
	}
	getrptentriescb(mainrsrc);
	quitrpt(rptrsrc);
}
void quitrpttest(RDArsrc *rptrsrc,RDArsrc *mainrsrc)
{
	readallwidgets(rptrsrc);
	if(COMPARE_RSRCS(rptrsrc,rptdefaults,2) || changedreport)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","REPORT CHANGES NOT SAVED!","Changes have been made to this REPORT DEFININTION!\nDo you want to Save these changes?",saverpt,quitrpt,FALSE,2,rptrsrc,mainrsrc,NULL);
	} else {
		quitrpt(rptrsrc);
	}
}
void verifyrpt(RDArsrc *mainrsrc)
{
}
static void setrangescreen(RDArsrc *mtnrsrc)
{
	char rangescrn=FALSE,*name=NULL,*rsname=NULL;

	readwidget(mtnrsrc,"RANGE SCREEN");
	FINDRSCGETCHAR(mtnrsrc,"RANGE SCREEN",&rangescrn);
	readwidget(mtnrsrc,"REPORT NAME");
	FINDRSCGETSTRING(mtnrsrc,"REPORT NAME",&name);
	if(rangescrn)
	{
		rsname=Rmalloc(RDAstrlen(name)+15);
		sprintf(rsname,"%s RANGE SCREEN",name);
		FINDRSCSETEDITABLE(mtnrsrc,"DEFAULT RANGE SCREEN",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"DEFAULT RANGE SCREEN",TRUE);
	} else {
		FINDRSCSETEDITABLE(mtnrsrc,"DEFAULT RANGE SCREEN",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"DEFAULT RANGE SCREEN",FALSE);
	}
	if(name!=NULL) Rfree(name);
	FINDRSCSETSENSITIVE(mtnrsrc,"RANGE SCREEN NAME",TRUE);
	FINDRSCSETSTRING(mtnrsrc,"RANGE SCREEN NAME",rsname);
	updatersrc(mtnrsrc,"RANGE SCREEN NAME");
	if(rsname!=NULL) 
	{
		Rfree(rsname);
	} else {
		FINDRSCSETSENSITIVE(mtnrsrc,"RANGE SCREEN NAME",FALSE);
	}
}
static void setchainrpt(RDArsrc *mtnrsrc)
{
	char chainrpt=FALSE;

	readwidget(mtnrsrc,"CHAIN REPORT");
	FINDRSCGETCHAR(mtnrsrc,"CHAIN REPORT",&chainrpt);
	if(chainrpt)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"CHAIN MODULE",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"CHAIN NAME",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"ASK BEFORE EXECUTE",TRUE);
	} else {
		FINDRSCSETSENSITIVE(mtnrsrc,"CHAIN MODULE",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"CHAIN NAME",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"ASK BEFORE EXECUTE",FALSE);
	}
}
static void setchainrptcb(RDArsrc *mtnrsrc)
{
	readwidget(mtnrsrc,"CHAIN REPORT");
	setchainrpt(mtnrsrc);
}
static void setremfile(RDArsrc *mtnrsrc)
{
	int remfile=FALSE;

	FINDRSCGETINT(mtnrsrc,"REMOVE FILE",&remfile);
	if(remfile)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"REMOVE FILENAME",TRUE);
	} else {
		FINDRSCSETSENSITIVE(mtnrsrc,"REMOVE FILENAME",FALSE);
	}
}
static void setremfilecb(RDArsrc *mtnrsrc)
{
	setremfile(mtnrsrc);
}
static void setproc_exceptions(RDArsrc *rptrsrc,char test)
{
	if(test==TRUE)
	{
		FINDRSCSETEDITABLE(rptrsrc,"PROCESS EXCEPTIONS SCREEN",TRUE);
		FINDRSCSETSENSITIVE(rptrsrc,"PROCESS EXCEPTIONS SCREEN",TRUE);
		FINDRSCSETEDITABLE(rptrsrc,"DEFAULT BROWSE SCREEN",TRUE);
		FINDRSCSETSENSITIVE(rptrsrc,"DEFAULT BROWSE SCREEN",TRUE);
		FINDRSCSETEDITABLE(rptrsrc,"DEFAULT DEFINE LIST",TRUE);
		FINDRSCSETSENSITIVE(rptrsrc,"DEFAULT DEFINE LIST",TRUE);
		FINDRSCSETEDITABLE(rptrsrc,"DEFAULT SEARCH BROWSE",TRUE);
		FINDRSCSETSENSITIVE(rptrsrc,"DEFAULT SEARCH BROWSE",TRUE);
		FINDRSCSETEDITABLE(rptrsrc,"DEFAULT MAINTAIN SCREEN",TRUE);
		FINDRSCSETSENSITIVE(rptrsrc,"DEFAULT MAINTAIN SCREEN",TRUE);
	} else {
		FINDRSCSETSENSITIVE(rptrsrc,"PROCESS EXCEPTIONS SCREEN",FALSE);
		FINDRSCSETEDITABLE(rptrsrc,"PROCESS EXCEPTIONS SCREEN",FALSE);
		FINDRSCSETSENSITIVE(rptrsrc,"DEFAULT BROWSE SCREEN",FALSE);
		FINDRSCSETEDITABLE(rptrsrc,"DEFAULT BROWSE SCREEN",FALSE);
		FINDRSCSETEDITABLE(rptrsrc,"DEFAULT DEFINE LIST",FALSE);
		FINDRSCSETSENSITIVE(rptrsrc,"DEFAULT DEFINE LIST",FALSE);
		FINDRSCSETEDITABLE(rptrsrc,"DEFAULT SEARCH BROWSE",FALSE);
		FINDRSCSETSENSITIVE(rptrsrc,"DEFAULT SEARCH BROWSE",FALSE);
		FINDRSCSETEDITABLE(rptrsrc,"DEFAULT MAINTAIN SCREEN",FALSE);
		FINDRSCSETSENSITIVE(rptrsrc,"DEFAULT MAINTAIN SCREEN",FALSE);
	}
}
static void changeproc_exceptions(RDArsrc *rptrsrc)
{
	char test=FALSE; 

	readwidget(rptrsrc,"PROCESS EXCEPTIONS");
	FINDRSCGETCHAR(rptrsrc,"PROCESS EXCEPTIONS",&test);
	setproc_exceptions(rptrsrc,test);
}
static void dfscn(RDArsrc *r)
{
	APPlib *args=NULL;
	char *m=NULL,*name=NULL;
	
	readwidget(r,"REPORT MODULE");
	FINDRSCGETSTRING(r,"REPORT MODULE",&m);
	args=APPlibNEW();
	if(!isEMPTY(m)) 
	{
		addAPPlib(args,m);
		readwidget(r,"REPORT NAME");
		FINDRSCGETSTRING(r,"REPORT NAME",&name);
		if(!isEMPTY(name))
		{
			memset(stemp,0,101);
			sprintf(stemp,"%s RANGE SCREEN",name);
			addAPPlib(args,stemp);
		}
	}
	ExecuteProgram("dfscn",args);
	if(m!=NULL) Rfree(m);
	if(args!=NULL) freeapplib(args);
}
static void dfmenu(RDArsrc *r)
{
	APPlib *args=NULL;
	char *m=NULL,*name=NULL;
	
	readwidget(r,"REPORT MODULE");
	FINDRSCGETSTRING(r,"REPORT MODULE",&m);
	args=APPlibNEW();
	if(!isEMPTY(m)) 
	{
		addAPPlib(args,m);
		readwidget(r,"REPORT NAME");
		FINDRSCGETSTRING(r,"REPORT NAME",&name);
		if(!isEMPTY(name))
		{
			memset(stemp,0,101);
			sprintf(stemp,"%s %s",m,name);
			addAPPlib(args,stemp);
		}
	}
	ExecuteProgram("dfmenu",args);
	if(m!=NULL) Rfree(m);
	if(args!=NULL) freeapplib(args);
}
static void dflist(RDArsrc *r)
{
	APPlib *args=NULL;
	char *m=NULL;
	
	readwidget(r,"REPORT MODULE");
	FINDRSCGETSTRING(r,"REPORT MODULE",&m);
	args=APPlibNEW();
	if(!isEMPTY(m)) addAPPlib(args,m);
	ExecuteProgram("dflist",args);
	if(m!=NULL) Rfree(m);
	if(args!=NULL) freeapplib(args);
}
static void dfload(RDArsrc *r)
{
	APPlib *args=NULL;
	char *m=NULL;
	
	readwidget(r,"REPORT MODULE");
	FINDRSCGETSTRING(r,"REPORT MODULE",&m);
	args=APPlibNEW();
	if(!isEMPTY(m)) addAPPlib(args,m);
	ExecuteProgram("dfload",args);
	if(m!=NULL) Rfree(m);
	if(args!=NULL) freeapplib(args);
}
void reportscreen(RDArsrc *mainrsrc,char *rptmodule1,char *rptname,
	short engine_type,short stop_on_error,short chain_rpt,
	char *chain_module,char *chain_name,short ask_b4_exe,
	char manual_form,char report_completion,
	short range_screen,short diag_breakable,short remove_file,
	char *remove_name,short remove_sort,short process_exceptions,
	char *title,char *desc,short use_print_screen,char *server_name,
	short no_empty_error,short no_wc_diag,short writecount_only,
	short write_all_detail_levels,MaintainMaster *RPTMTNMSTR)
{
	char stoponerr=0,askb4exe=0,chainrpt=0,rangescrn=0,rmsort=0;
	char noemptyerr=0,nowcdiag=0,wonly=0,wadl=0;
	char proc_exceptions=0,*rsname=NULL,usepscreen=FALSE;
	char default_range=FALSE;
	int a,eng_type=0,diagbreak=0,rmfile=0;

	if(dbengines!=NULL) freeapplib(dbengines);
	dbengines=APPlibNEW();
	addAPPlib(dbengines,supported_engines[0]);
#ifdef INFORMIX_ISAM
	addAPPlib(dbengines,supported_engines[1]);
#endif
#ifdef USE_MYISAM
	addAPPlib(dbengines,supported_engines[2]);
#endif
#ifdef USE_MYSQL
	addAPPlib(dbengines,supported_engines[3]);
	addAPPlib(dbengines,supported_engines[4]);
#endif
#ifdef USE_CTREE
	addAPPlib(dbengines,supported_engines[5]);
	addAPPlib(dbengines,supported_engines[6]);
#endif
#ifdef USE_BERKELEY_BTREE
	addAPPlib(dbengines,supported_engines[7]);
#endif
	rptrsrc=RDArsrcNEW(module,"MAINTAIN REPORT");
	addstdrsrc(rptrsrc,"REPORT MODULE",PLAINTEXT,15,(rptmodule1!=NULL ? rptmodule1:""),TRUE);
	FINDRSCSETFUNC(rptrsrc,"REPORT MODULE",getrptmod,NULL);
	addstdrsrc(rptrsrc,"REPORT NAME",PLAINTEXT,40,(rptname!=NULL ? rptname:""),TRUE);
	addstdrsrc(rptrsrc,"SERVER NAME",VARIABLETEXT,0,(server_name!=NULL ? server_name:""),TRUE);
	addstdrsrc(rptrsrc,"TITLE",VARIABLETEXT,0,(title!=NULL ? title:""),TRUE);
	addsctrsrc(rptrsrc,"DESCRIPTION",0,(desc!=NULL ? desc:""),TRUE);
	for(a=0;a<dbengines->numlibs;++a)
	{
		if(!RDAstrcmp(dbengines->libs[a],supported_engines[engine_type])) break;	
	}
	if(a<dbengines->numlibs)
	{
		eng_type=a;
	} else {
		eng_type=0;
	}
	addlstrsrc(rptrsrc,"ENGINE TYPES",&eng_type,TRUE,NULL,dbengines->numlibs,&dbengines->libs,NULL);
	if(stop_on_error) stoponerr=TRUE;
	addstdrsrc(rptrsrc,"STOP ON ERROR",BOOLNS,1,&stoponerr,TRUE);
	if(no_empty_error) noemptyerr=TRUE;
	addstdrsrc(rptrsrc,"NO EMPTY ERROR",BOOLNS,1,&noemptyerr,TRUE);
	if(no_wc_diag) nowcdiag=TRUE;
	addstdrsrc(rptrsrc,"NO WRITECOUNT TOTAL",BOOLNS,1,&nowcdiag,TRUE);
	if(writecount_only) wonly=TRUE;
	addstdrsrc(rptrsrc,"WRITECOUNT ONLY",BOOLNS,1,&wonly,TRUE);
	if(write_all_detail_levels) wadl=TRUE;
	addstdrsrc(rptrsrc,"WRITE ALL DETAIL LEVELS",BOOLNS,1,&wadl,TRUE);
	diagbreak=diag_breakable;
	if(diaglevel!=NULL) freeapplib(diaglevel);
	diaglevel=APPlibNEW();
	addAPPlib(diaglevel,"Diagnostics - Not breakable");
	addAPPlib(diaglevel,"Diagnostics - Breakable");
	addAPPlib(diaglevel,"No Diagnostics");
	addlstrsrc(rptrsrc,"DIAGNOSTIC LEVEL",&diagbreak,TRUE,NULL,diaglevel->numlibs,&diaglevel->libs,NULL);
	if(process_exceptions) proc_exceptions=TRUE;	
	addstdrsrc(rptrsrc,"PROCESS EXCEPTIONS",BOOLNS,1,&proc_exceptions,TRUE);
	FINDRSCSETFUNC(rptrsrc,"PROCESS EXCEPTIONS",changeproc_exceptions,NULL);
	if(chain_rpt) chainrpt=TRUE;
	addstdrsrc(rptrsrc,"CHAIN MODULE",PLAINTEXT,15,(chain_module!=NULL ? chain_module:""),TRUE);
	addstdrsrc(rptrsrc,"CHAIN NAME",PLAINTEXT,40,(chain_name!=NULL ? chain_name:""),TRUE);
	if(ask_b4_exe)
	{
		askb4exe=TRUE;
	} else {
		askb4exe=FALSE;
	}
	addstdrsrc(rptrsrc,"ASK BEFORE EXECUTE",BOOLNS,1,&askb4exe,TRUE);
	addstdrsrc(rptrsrc,"MANUAL FORM CONTROL",BOOLNS,1,&manual_form,TRUE);
	addstdrsrc(rptrsrc,"REPORT COMPLETION",BOOLNS,1,&report_completion,TRUE);
	addstdrsrc(rptrsrc,"CHAIN REPORT",BOOLNS,1,&chainrpt,TRUE);
	FINDRSCSETFUNC(rptrsrc,"CHAIN REPORT",setchainrptcb,NULL);
	if(remove_sort)
	{
		rmsort=TRUE;
	} else {
		rmsort=FALSE;
	}
	addstdrsrc(rptrsrc,"REMOVE SORT",BOOLNS,1,&rmsort,TRUE);
	if(remfile!=NULL) freeapplib(remfile);
	remfile=APPlibNEW();
	addAPPlib(remfile,"Do Nothing");
	addAPPlib(remfile,"Remove/Unlink File");
	addAPPlib(remfile,"dohttp/URL");
	rmfile=remove_file;
	addlstrsrc(rptrsrc,"REMOVE FILE",&rmfile,TRUE,setremfilecb,
		remfile->numlibs,&remfile->libs,NULL);
	addstdrsrc(rptrsrc,"REMOVE FILENAME",VARIABLETEXT,0,(remove_name!=NULL ? remove_name:""),TRUE);
	if(range_screen)
	{
		rangescrn=TRUE;
	} else { 
		rangescrn=FALSE;
	}
	addstdrsrc(rptrsrc,"RANGE SCREEN",BOOLNS,1,&rangescrn,TRUE);
	addstdrsrc(rptrsrc,"DEFAULT RANGE SCREEN",BOOLNS,1,&default_range,TRUE);
	addstdrsrc(rptrsrc,"DEFAULT MENU",BOOLNS,1,&default_range,TRUE);
	addstdrsrc(rptrsrc,"DEFAULT BROWSE SCREEN",BOOLNS,1,&default_range,TRUE);
	if(rangescrn)
	{
		FINDRSCSETEDITABLE(rptrsrc,"DEFAULT RANGE SCREEN",TRUE);
		FINDRSCSETSENSITIVE(rptrsrc,"DEFAULT RANGE SCREEN",TRUE);
	} else {
		FINDRSCSETEDITABLE(rptrsrc,"DEFAULT RANGE SCREEN",FALSE);
		FINDRSCSETSENSITIVE(rptrsrc,"DEFAULT RANGE SCREEN",FALSE);
	}
	addstdrsrc(rptrsrc,"DEFAULT DEFINE LIST",BOOLNS,1,&default_range,TRUE);
	addstdrsrc(rptrsrc,"DEFAULT SEARCH BROWSE",BOOLNS,1,&default_range,TRUE);
	addstdrsrc(rptrsrc,"DEFAULT MAINTAIN SCREEN",BOOLNS,1,&default_range,TRUE);
	if(rangescrn)
	{
		rsname=Rmalloc(RDAstrlen(rptname)+15);
		sprintf(rsname,"%s RANGE SCREEN",rptname);
	}
	addstdrsrc(rptrsrc,"RANGE SCREEN NAME",VARIABLETEXT,0,(rsname!=NULL ? rsname:""),FALSE);
	if(rsname!=NULL) 
	{
		Rfree(rsname);
	} else {
		FINDRSCSETSENSITIVE(rptrsrc,"RANGE SCREEN NAME",FALSE);
	}
	FINDRSCSETFUNC(rptrsrc,"RANGE SCREEN",setrangescreen,NULL);
	usepscreen=use_print_screen;
	addstdrsrc(rptrsrc,"USE PRINT SCREEN",BOOLNS,1,&usepscreen,TRUE);
	addbtnrsrc(rptrsrc,"SEARCH FILES",TRUE,RPTsearch,NULL);
	addbtnrsrc(rptrsrc,"VIRTUAL FIELDS",TRUE,virflds,NULL);
	addbtnrsrc(rptrsrc,"SELECT RECORDS",TRUE,RPTselectrecords,NULL);
	addbtnrsrc(rptrsrc,"SORT DEFINITION",TRUE,RPTsortdef,NULL);
	addbtnrsrc(rptrsrc,"CONTROL BREAKS",TRUE,RPTcontrol,NULL);
	addbtnrsrc(rptrsrc,"ACCUMULATORS",TRUE,RPTaccum,NULL);
	addbtnrsrc(rptrsrc,"DEFINE DISPLAY",TRUE,RPTdisplay,NULL);
	addbtnrsrc(rptrsrc,"DEFINE IMAGE",TRUE,RPTimage,NULL);
	addbtnrsrc(rptrsrc,"VERIFY REPORT",TRUE,verifyrpt,NULL);
	addbtnrsrc(rptrsrc,"PROCESS EXCEPTIONS SCREEN",TRUE,procexcpscrn,report);
	addbtnrsrc(rptrsrc,"PRINT REPORT",TRUE,printreport,report);
	addbtnrsrc(rptrsrc,"DFSCN",TRUE,dfscn,NULL);
	addbtnrsrc(rptrsrc,"DFMENU",TRUE,dfmenu,NULL);
	addbtnrsrc(rptrsrc,"DFLIST",TRUE,dflist,NULL);
	addbtnrsrc(rptrsrc,"DFLOAD",TRUE,dfload,NULL);
	addrfcbrsrc(rptrsrc,"SAVE",TRUE,saverpt,mainrsrc);
	addrfcbrsrc(rptrsrc,"QUIT",TRUE,quitrpttest,mainrsrc);
	addbtnrsrc(rptrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(rptrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	changedreport=FALSE;
	rptdefaults=GetDefaults(rptrsrc);
	setchainrpt(rptrsrc);
	setremfile(rptrsrc);
	setproc_exceptions(rptrsrc,proc_exceptions);
	APPmakescrn(rptrsrc,TRUE,quitrpt,NULL,FALSE);
}
void switchengine(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0,x=0;

	if(FINDRSCGETINT(mainrsrc,"REPORTS AVAILABLE",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(!RDAstrcmp(rptavl->libs[selected],"No Definitions Available"))
	{
		return;
	}
	for(x=0;x<rptavl->numlibs;++x)
	{
		report=GETRDAreport(modlstx->libs[selectedm],rptavl->libs[x]);
		if(report!=NULL)
		{
#ifdef USE_MYISAM
			report->engine_type=NRDMyIsamEngine;
#endif /* USE_MYISAM */
#ifndef USE_MYISAM
			report->engine_type=NRDLocalCtreeEngine;
#endif /* USE_MYISAM */
			if(WRTRDAreport(modlstx->libs[selectedm],report))
			{
				ERRORDIALOG("Cannot Write Report","Error in permissions, users cannot change the report.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
			}
			FreeRDAreport(report);
		}
	}
}
void okreport(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;

	if(FINDRSCGETINT(mainrsrc,"REPORTS AVAILABLE",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(RDAstrcmp(rptavl->libs[selected],"No Definitions Available"))
	{
		report=GETRDAreport(modlstx->libs[selectedm],rptavl->libs[selected]);
		if(report==NULL)
		{
			ERRORDIALOG("Cannot Read Report","Error in permissions, users doesn't have access to read the report definition.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
			return;
		}
	} else report=NULL;
	if(report==NULL)
	{
		report=RDAreportNEW(rptavl->libs[selected],NULL,NULL,0,NULL,0,0,0,0,0,NULL,NULL,0,0,0);
		report->type=TRUE;
		prterr("Error Report [%s] not readable.",rptavl->libs[selected]);
	}
	if(rptmodule!=NULL) Rfree(rptmodule);
	rptmodule=stralloc(modlstx->libs[selectedm]);
	reportscreen(mainrsrc,modlstx->libs[selectedm],rptavl->libs[selected],
		report->engine_type,report->stop_on_error,
		report->chain_report,report->chain_module,
		report->chain_name,report->ask_b4_execute,report->manual_form,report->report_completion,report->range_screen,
		report->diag_breakable,report->remove_file,report->remove_name,
		report->removesort,report->process_exceptions,report->title,
		report->desc,report->use_print_screen,report->server_name,
		report->no_empty_error,report->no_wc_diag,
		report->writecount_only,report->write_all_detail_levels,
		report->MTNMSTR);
}
void newreport(RDArsrc *mainrsrc)
{
	int selectedm=0;

	report=RDAreportNEW(NULL,NULL,NULL,0,NULL,0,0,0,0,0,NULL,NULL,0,0,0);
	report->type=TRUE;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(rptmodule!=NULL) Rfree(rptmodule);
	rptmodule=stralloc(modlstx->libs[selectedm]);
	reportscreen(mainrsrc,modlstx->libs[selectedm],report->name,
		report->engine_type,report->stop_on_error,report->chain_report,
		report->chain_module,report->chain_name,report->ask_b4_execute,
		report->manual_form,report->report_completion,
		report->range_screen,report->diag_breakable,
		report->remove_file,report->remove_name,report->removesort,
		report->process_exceptions,report->title,report->desc,
		report->use_print_screen,report->server_name,
		report->no_empty_error,report->no_wc_diag,
		report->writecount_only,report->write_all_detail_levels,
		report->MTNMSTR);
}
void getmodulelist(RDArsrc *mainrsrc)
{
	int x;
	char *tmp=NULL,*dirx=NULL;

	if(modlstx!=NULL) freeapplib(modlstx);
	modlstx=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	for(x=0;findfile(dirx,"*.FIL",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlibNoDuplicates(modlstx,tmp);
	}
	for(x=0;findfile(dirx,"*.SCN",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlibNoDuplicates(modlstx,tmp);
	}
	if(modlstx->numlibs<1)
	{
		addAPPlib(modlst,"Contains No Database Libraries");
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	SORTAPPlib(modlstx);
	if(mainrsrc!=NULL)
	{
		if(!FINDRSCLISTAPPlib(mainrsrc,"MODULE LIST",0,modlstx))
		{
			updatersrc(mainrsrc,"MODULE LIST");
		}
	}
}
void getrptentries(RDArsrc *mainrsrc,char *modname)
{
	int x;
	char *tmp=NULL,*dirx=NULL,*temp=NULL;

	if(rptavl!=NULL) freeapplib(rptavl);
	rptavl=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modname)+3);
#ifndef WIN32
	sprintf(dirx,"%s/%s/",CURRENTDIRECTORY,modname);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\%s\\",CURRENTDIRECTORY,modname);
#endif
	for(x=0;findfile(dirx,"*.RPT",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		temp=stripdashes(tmp);
		addAPPlib(rptavl,temp);
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	}
	if(rptavl->numlibs<1)
	{
		addAPPlib(rptavl,"No Report Definitions Available");
	}
	if(dirx!=NULL) Rfree(dirx);
	if(tmp!=NULL) Rfree(tmp);
	SORTAPPlib(rptavl);
}
void getrptentriescb(RDArsrc *mainrsrc)
{
	int selected=0,selr=0;
/*
	int x;
	char *tmp=NULL,*dirx=NULL,*temp=NULL;
*/

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"REPORTS AVAILABLE",&selr)) return;
	getrptentries(mainrsrc,modlstx->libs[selected]);
/*
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlstx->libs[selected])+3);
#ifndef WIN32
	sprintf(dirx,"%s/%s/",CURRENTDIRECTORY,modlstx->libs[selected]);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\%s\\",CURRENTDIRECTORY,modlstx->libs[selected]);
#endif
	for(x=0;findfile(dirx,"*.RPT",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		temp=stripdashes(tmp);
		addAPPlib(rptavl,temp);
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	}
	if(rptavl->numlibs<1)
	{
		addAPPlib(rptavl,"No Report Definitions Available");
	}
	if(dirx!=NULL) Rfree(dirx);
	if(tmp!=NULL) Rfree(tmp);
	SORTAPPlib(rptavl);
*/
	if(selr>=rptavl->numlibs) selr=0;
	if(mainrsrc!=NULL)
	{
		if(!FINDRSCLISTAPPlib(mainrsrc,"REPORTS AVAILABLE",selr,rptavl))
			updatersrc(mainrsrc,"REPORTS AVAILABLE");
		setvaluescb(mainrsrc);
	}
}
void deleterpt(RDArsrc *mainrsrc)
{
	char *fname=NULL,*dashes=NULL;
	int selected=0,selectedm=0;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(mainrsrc,"REPORTS AVAILABLE",&selected)) return;
	dashes=adddashes(rptavl->libs[selected]);
	fname=Rmalloc(RDAstrlen(modlstx->libs[selectedm])+RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(dashes)+7);
#ifndef WIN32
	sprintf(fname,"%s/%s/%s.RPT",CURRENTDIRECTORY,modlstx->libs[selectedm],dashes);
#endif
#ifdef WIN32
	sprintf(fname,"%s\\%s\\%s.RPT",CURRENTDIRECTORY,modlstx->libs[selectedm],dashes);
#endif
	if(dashes!=NULL) Rfree(dashes);
	if(access(fname,02))
	{
		ERRORDIALOG("Cannot Delete Report","The Report cannot be deleted.  Check the permissions, and try again.  Call your installer.",NULL,FALSE);
		prterr("Error in permissions, users doesn't have write access to [%s] Report Binary.",fname);
	} else {
		if(unlink(fname)==(-1))
		{
			prterr("Error Cannot Remove Report Definition File [%s].",fname);
		} else {
			getrptentriescb(mainrsrc);
		}
	}
	if(fname!=NULL) Rfree(fname);
}
void deleterpttest(RDArsrc *mainrsrc)
{
	WARNINGDIALOG("DELETE WARNING DIALOG SCREEN","DELETE REPORT DEFINITION?","Are you sure that you want to DELETE this REPORT DEFINITION?",deleterpt,NULL,FALSE,mainrsrc);
}
void quitdfrpt(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	if(modlstx!=NULL) freeapplib(modlstx);
	if(rptavl!=NULL) freeapplib(rptavl);
	if(dbengines!=NULL) freeapplib(dbengines);
	if(remfile!=NULL) freeapplib(remfile);
	ShutdownSubsystems();
	std::exit;
}
void setvalues(RDArsrc *mainrsrc,char *modname,char *rptname)
{
	char *temp=NULL;

	if(RDAstrcmp(rptname,"No Report Definitions Available"))
	{
		temp=getreporttitle(modname,rptname);
	} else temp=NULL;
	if(!FINDRSCSETSTRING(mainrsrc,"TITLE",temp))
	{
		updatersrc(mainrsrc,"TITLE");
	}
	if(temp!=NULL) Rfree(temp);
	if(RDAstrcmp(rptname,"No Report Definitions Available"))
	{
		temp=getreportdesc(modname,rptname);
	} else temp=NULL;
	if(!FINDRSCSETSTRING(mainrsrc,"DESCRIPTION",temp))
		updatersrc(mainrsrc,"DESCRIPTION");
	if(temp!=NULL) Rfree(temp);
}
void setvaluescb(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(mainrsrc,"REPORTS AVAILABLE",&selected)) return;
	setvalues(mainrsrc,modlstx->libs[selectedm],rptavl->libs[selected]);
}
static void doreport(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;
	APPlib *args=NULL;

	if(rptavl->numlibs==1 && !RDAstrcmp(rptavl->libs[0],"No Report Definitions Available")) return;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(mainrsrc,"REPORTS AVAILABLE",&selected)) return;
	args=APPlibNEW();
	addAPPlib(args,modlstx->libs[selectedm]);
	addAPPlib(args,rptavl->libs[selected]);
	ExecuteProgram("doreport",args);
	if(args!=NULL) freeapplib(args);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0,y=0;

	if(InitializeSubsystems(argc,argv,module,"DEFINE REPORTS")) 
	{
		return;
	}
	getmodulelist(mainrsrc);
	mainrsrc=RDArsrcNEW(module,"DEFINE REPORTS");
	if(argc>1)
	{
		for(y=0;y<modlstx->numlibs;++y)
		{
			if(!RDAstrcmp(argv[1],modlstx->libs[y])) break;
		}
		if(y>=modlstx->numlibs)
		{
			y=0;
		}
	}
	addlstrsrc(mainrsrc,"MODULE LIST",&y,TRUE,getrptentriescb,
		modlstx->numlibs,&modlstx->libs,NULL);
	x=0;
	addstdrsrc(mainrsrc,"TITLE",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mainrsrc,"DESCRIPTION",SCROLLEDTEXT,0,NULL,FALSE);
	getrptentries(mainrsrc,modlstx->libs[y]);
	if(argc>2)
	{
		for(x=0;x<rptavl->numlibs;++x)
		{
			if(!RDAstrcmp(argv[2],rptavl->libs[x])) break;
		}
		if(x>=rptavl->numlibs)
		{
			x=0;
		}
	}
	addlstrsrc(mainrsrc,"REPORTS AVAILABLE",&x,TRUE,setvaluescb,rptavl->numlibs,&rptavl->libs,NULL);
	setvalues(mainrsrc,modlstx->libs[y],rptavl->libs[x]);
	addbtnrsrc(mainrsrc,"ADD",TRUE,newreport,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,okreport,NULL);
	addbtnrsrc(mainrsrc,"ENGINE",TRUE,switchengine,NULL);
	addbtnrsrc(mainrsrc,"RUN REPORT",TRUE,doreport,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitdfrpt,NULL);
	addbtnrsrc(mainrsrc,"DELETE",TRUE,deleterpttest,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,quitdfrpt,NULL,TRUE);
}


APPlib *makefieldvallist2mtn(RDAsearch *search,int numsearchs,int number,short dofields,char *submodname,char *subfilename)
{
	APPlib *tmp=NULL;
	int x,y,z;
	NRDfield *fields=NULL,*f;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short nofields=0,nokeys=0,engine=0;
	char *temp=NULL;
	RDAsearch *s;

	tmp=APPlibNEW();
	if(dofields)
	{
		loadglobals(tmp);
	}
	if(search!=NULL)
	{
	for(x=0,s=search;x<number;++x,++s)
	{
		if(!GETBIN(s->module,s->filename,&engine,&nofields,&fields,&nokeys,&keys))
		{
			if(fields!=NULL)
			{
				for(y=0,f=fields;y<nofields;++y,++f)
				{
					if(temp!=NULL)
					{
						temp=Rrealloc(temp,RDAstrlen(f->name)+RDAstrlen(s->module)+RDAstrlen(s->filename)+7);
					} else {
						temp=Rmalloc(RDAstrlen(f->name)+RDAstrlen(s->module)+RDAstrlen(s->filename)+7);
					}
					sprintf(temp,"[%s][%s][%s]",s->module,s->filename,f->name);
					addAPPlibNoDuplicates(tmp,temp);
					if(f->name!=NULL) Rfree(f->name); 
				}
				Rfree(fields);
			}
			if(keys!=NULL)
			{
				for(y=0,keyx=keys;y<nokeys;++y,++keyx)
				{
					if(keyx->name!=NULL) Rfree(keyx->name);
					if(keyx->part!=NULL)
					{
						for(z=0,part=keyx->part;z<keyx->numparts;++z,++part)
						{
							if(part->name!=NULL) Rfree(part->name);
						}
						Rfree(keyx->part);
					}
				}
				Rfree(keys);
			}
		}
	}
	}
	if(!isEMPTY(submodname) && !isEMPTY(subfilename))
	{
		if(!GETBIN(submodname,subfilename,&engine,&nofields,&fields,&nokeys,&keys))
		{
			if(fields!=NULL)
			{
				for(y=0,f=fields;y<nofields;++y,++f)
				{
					if(temp!=NULL)
					{
						temp=Rrealloc(temp,RDAstrlen(f->name)+RDAstrlen(submodname)+RDAstrlen(subfilename)+7);
					} else {
						temp=Rmalloc(RDAstrlen(f->name)+RDAstrlen(submodname)+RDAstrlen(subfilename)+7);
					}
					sprintf(temp,"[%s][%s][%s]",submodname,subfilename,f->name);
					addAPPlibNoDuplicates(tmp,temp);
					if(f->name!=NULL) Rfree(f->name); 
				}
				Rfree(fields);
			}
			if(keys!=NULL)
			{
				for(y=0,keyx=keys;y<nokeys;++y,++keyx)
				{
					if(keyx->name!=NULL) Rfree(keyx->name);
					if(keyx->part!=NULL)
					{
						for(z=0,part=keyx->part;z<keyx->numparts;++z,++part)
						{
							if(part->name!=NULL) Rfree(part->name);
						}
						Rfree(keyx->part);
					}
				}
				Rfree(keys);
			}
		}
	}
	if(temp!=NULL) Rfree(temp);
	makefieldvallistrpt(&tmp,TRUE,TRUE,TRUE,FALSE);
	SORTAPPlib(tmp);
	return(tmp);
}
void addholdvalues(APPlib *tmp,char *modulex,char *filex)
{
	NRDfield *f=NULL,*fields=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part=NULL;
	short z=0,engine=0,nofields=0,nokeys=0;
	int y=0;
	char *temp=NULL;

	if(diagapps)
	{
		prterr("DIAG addholdvalues Module [%s] File [%s] ",modulex,filex);
	}
	if(isEMPTY(modulex) || isEMPTY(filex)) 
	{
		prterr("Error Module [%s] or File [%s] is NULL.",(modulex!=NULL ? modulex:"NULL"),(filex!=NULL ? filex:""));
		return;
	}
/*
	if(!RDAstrcmp(modulex,"RPTGEN") && !RDAstrcmp(filex,"SORTFILE"))
*/
	if(!RDAstrcmp(filex,"SORTFILE"))
	{
		makefieldvallistrpt(&tmp,TRUE,TRUE,TRUE,FALSE);
	} else {
		if(!GETBIN(modulex,filex,&engine,&nofields,&fields,&nokeys,&keys)) 
		{
			if(keys!=NULL)
			{
				for(y=0,keyx=keys;y<nokeys;++y,++keyx)
				{
					if(keyx->name!=NULL) Rfree(keyx->name);
					if(keyx->part!=NULL)
					{
						for(z=0,part=keyx->part;z<keyx->numparts;++z,++part)
						{
							if(part->name!=NULL) Rfree(part->name);
						}
					}
					Rfree(keyx->part);
				}
				Rfree(keys);
				if(fields!=NULL)
				{
					for(y=0,f=fields;y<nofields;++y,++f)
					{
						if(temp!=NULL)
						{
							temp=Rrealloc(temp,RDAstrlen(f->name)+RDAstrlen(modulex)+RDAstrlen(filex)+7);
						} else 	{
							temp=Rmalloc(RDAstrlen(f->name)+RDAstrlen(modulex)+RDAstrlen(filex)+7);
						}
						sprintf(temp,"[%s][%s][%s]",modulex,filex,f->name);
						addAPPlibNoDuplicates(tmp,temp);
						if(f->name!=NULL) Rfree(f->name);
					}
					Rfree(fields);
				}
			}
		}
	}
	if(temp!=NULL) Rfree(temp);
}
void addholdvaluessup(APPlib *tmp,char *modulex,char *filex)
{
	NRDfield *f=NULL,*fields=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part=NULL;
	short z,engine=0,nofields=0,nokeys=0;
	int y;
	char *temp=NULL;

	if(diagrptgen)
	{
		prterr("DIAG addholdvaluessup Module [%s] File [%s] ",modulex,filex);
	}
/*
	if(!RDAstrcmp(modulex,"RPTGEN") && !RDAstrcmp(filex,"SORTFILE"))
*/
	if(!RDAstrcmp(filex,"SORTFILE"))
	{
		makefieldvallistrpt(&tmp,TRUE,FALSE,TRUE,FALSE);
	} else {
		if(!GETBIN(modulex,filex,&engine,&nofields,&fields,&nokeys,&keys)) 
		{
			if(keys!=NULL)
			{
				for(y=0,keyx=keys;y<nokeys;++y,++keyx)
				{
					if(keyx->name!=NULL) Rfree(keyx->name);
					if(keyx->part!=NULL)
					{
						for(z=0,part=keyx->part;z<keyx->numparts;
							++z,++part)
						{
							if(part->name!=NULL) Rfree(part->name);
						}
					}
					Rfree(keyx->part);
				}
				Rfree(keys);
				if(fields!=NULL)
				{
					for(y=0,f=fields;y<nofields;++y,++f)
					{
						if(temp!=NULL)
						{
							temp=Rrealloc(temp,RDAstrlen(f->name)+
								RDAstrlen(filex)+7);
	/*
							temp=Rrealloc(temp,RDAstrlen(f->name)+
								RDAstrlen(modulex)+
								RDAstrlen(filex)+7);
	*/
						} else {
							temp=Rmalloc(RDAstrlen(f->name)+
								RDAstrlen(filex)+7);
	/*
							temp=Rmalloc(RDAstrlen(f->name)+RDAstrlen(modulex)+
								RDAstrlen(filex)+7);
	*/
						}
						sprintf(temp,"[%s][%s]",
							filex,(f->name==NULL?"":f->name));
	/*
						sprintf(temp,"[%s][%s][%s]",modulex,
							filex,(f->name==NULL?"":f->name));
	*/
						addAPPlib(tmp,temp);
						if(f->name!=NULL) Rfree(f->name);
					}
					Rfree(fields);
				}
			}
		}
	}
	if(temp!=NULL) Rfree(temp);
}
void addholdvaluessub(APPlib *tmp,char *modulex,char *filex)
{
	NRDfield *f=NULL,*fields=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part=NULL;
	short z,engine=0,nofields=0,nokeys=0;
	int y;
	char *temp=NULL;

	if(diagrptgen)
	{
		prterr("DIAG addholdvaluessub Module [%s] File [%s] ",modulex,filex);
	}
	if(!RDAstrcmp(filex,"SORTFILE"))
	{
		makefieldvallistrpt(&tmp,TRUE,FALSE,TRUE,FALSE);
	} else {
		if(!GETBIN(modulex,filex,&engine,&nofields,&fields,&nokeys,&keys)) 
		{
			if(keys!=NULL)
			{
				for(y=0,keyx=keys;y<nokeys;++y,++keyx)
				{
					if(keyx->name!=NULL) Rfree(keyx->name);
					if(keyx->part!=NULL)
					{
						for(z=0,part=keyx->part;z<keyx->numparts;
							++z,++part)
						{
							if(part->name!=NULL) Rfree(part->name);
						}
					}
					Rfree(keyx->part);
				}
				Rfree(keys);
				if(fields!=NULL)
				{
					for(y=0,f=fields;y<nofields;++y,++f)
					{
						if(temp!=NULL)
						{
							temp=Rrealloc(temp,RDAstrlen(f->name)+
	/*
								RDAstrlen(modulex)+
	*/
								RDAstrlen(filex)+7);
						} else 	{
							temp=Rmalloc(RDAstrlen(f->name)+
	/*
								RDAstrlen(modulex)+
	*/
								RDAstrlen(filex)+7);
						}
						sprintf(temp,"[%s][%s]",
							filex,(f->name==NULL?"":f->name));
	/*
						sprintf(temp,"[%s][%s][%s]",modulex,
							filex,f->name);
	*/
						addAPPlib(tmp,temp);
						if(f->name!=NULL) Rfree(f->name);
					}
					Rfree(fields);
				}
			}
		}
	}
	if(temp!=NULL) Rfree(temp);
}
void addholdvaluesact(APPlib *tmp,char *modulex,char *filex)
{
	NRDfield *f=NULL,*fields=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part=NULL;
	short z,engine=0,nofields=0,nokeys=0;
	int y;
	char *temp=NULL;

	if(diagrptgen)
	{
		prterr("DIAG addholdvaluessup Module [%s] File [%s] ",modulex,filex);
	}
/*
	if(!RDAstrcmp(modulex,"RPTGEN") && !RDAstrcmp(filex,"SORTFILE"))
*/
	if(!RDAstrcmp(filex,"SORTFILE"))
	{
		makefieldvallistrpt(&tmp,TRUE,FALSE,TRUE,FALSE);
	} else {
		if(!GETBIN(modulex,filex,&engine,&nofields,&fields,&nokeys,&keys)) 
		{
			if(keys!=NULL)
			{
				for(y=0,keyx=keys;y<nokeys;++y,++keyx)
				{
					if(keyx->name!=NULL) Rfree(keyx->name);
					if(keyx->part!=NULL)
					{
						for(z=0,part=keyx->part;z<keyx->numparts;
							++z,++part)
						{
							if(part->name!=NULL) Rfree(part->name);
						}
					}
					Rfree(keyx->part);
				}
				Rfree(keys);
				if(fields!=NULL)
				{
					for(y=0,f=fields;y<nofields;++y,++f)
					{
						if(temp!=NULL)
						{
							temp=Rrealloc(temp,RDAstrlen(f->name)+
								RDAstrlen(filex)+7);
	/*
							temp=Rrealloc(temp,RDAstrlen(f->name)+
								RDAstrlen(modulex)+
								RDAstrlen(filex)+7);
	*/
						} else {
							temp=Rmalloc(RDAstrlen(f->name)+
								RDAstrlen(filex)+7);
	/*
							temp=Rmalloc(RDAstrlen(f->name)+RDAstrlen(modulex)+
								RDAstrlen(filex)+7);
	*/
						}
						sprintf(temp,"[%s][%s]",
							filex,(f->name==NULL?"":f->name));
	/*
						sprintf(temp,"[%s][%s][%s]",modulex,
							filex,(f->name==NULL?"":f->name));
	*/
						addAPPlib(tmp,temp);
						if(f->name!=NULL) Rfree(f->name);
					}
					Rfree(fields);
				}
			}
		}
	}
	if(temp!=NULL) Rfree(temp);
}
APPlib *makefieldvallistmtn(MaintainMaster *MTNMSTR)
{
	APPlib *tmp=NULL,*virflist=NULL;
	char addvf=FALSE,*libx=NULL;
	int x,y,z;
	DFincvir *i,*a;
	DFvirtual *d=NULL;
	short dumb=0;
	RDArsrc *tmprsrc=NULL;
	RDAsearch *temp=NULL;

	tmprsrc=RDArsrcNEW(module,"TEMP");
	tmp=APPlibNEW();
	makefieldvallistrpt(&tmp,TRUE,TRUE,TRUE,FALSE);
	if(tmp==NULL) tmp=APPlibNEW();
	if(RDAstrcmp(MTNMSTR->mainfile,"SORTFILE"))
	{
		addDFincvir(tmprsrc,MTNMSTR->module,MTNMSTR->mainfile,NULL,dumb);
	}
	if(MTNMSTR->s!=NULL)
	{
		for(x=0,temp=MTNMSTR->s;x<MTNMSTR->num;++x,++temp)
		{
			addDFincvir(tmprsrc,temp->module,temp->filename,NULL,dumb);
		}
	}
	for(x=0,i=tmprsrc->incvir;x<tmprsrc->numvir;++x,++i)
	{
		addholdvalues(tmp,i->module,i->file);
	}
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(MTNMSTR->module)+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,MTNMSTR->module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,MTNMSTR->module);
#endif
	if(!isEMPTY(MTNMSTR->module)) virflist=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	libx=NULL;
	if(virflist!=NULL)
	{
		for(y=0;y<virflist->numlibs;++y)
		{
			d=getDFvirtual(module,virflist->libs[y]);
			if(d!=NULL)
			{
				addvf=TRUE;
				if(d->incvir!=NULL)
				{
					for(z=0,a=d->incvir;z<d->num;++z,++a)
					{
						for(x=0,i=tmprsrc->incvir;x<tmprsrc->numvir;++x,++i)
						{
							if(!RDAstrcmp(i->module,a->module) && 
								!RDAstrcmp(i->file,a->file)) break;
						}
						if(x>=tmprsrc->numvir) 
						{
							addvf=FALSE;
							break;
						}
					}
				}
				if(addvf)
				{
					libx=Rmalloc(RDAstrlen(d->name)+3);
					sprintf(libx,"[%s]",d->name);
					addAPPlibNoDuplicates(tmp,libx);
					if(libx!=NULL) Rfree(libx);
				}
				FreeDFvirtual(d);
			}
		}
	}
	if(virflist!=NULL) freeapplib(virflist);
	if(libx!=NULL) Rfree(libx);
	loadglobals(tmp);
	addAPPlibNoDuplicates(tmp,"[NEXT TRANSACTION NO]");
	if(tmprsrc!=NULL) free_rsrc(tmprsrc);
	SORTAPPlib(tmp);
	return(tmp);
}
void loadvaluemtn(RDArsrc *r,char *fieldname)
{
	int x;
	LoadValue *load;

	ldvalrsrc=RDArsrcNEW("RPTGEN","LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	loadlist=makefieldvallistmtn(report->MTNMSTR);
	if(loadlist->numlibs<1)
	{
		addAPPlib(loadlist,"Load List Unavailable");
	}
	x=0;
	addlstrsrc(ldvalrsrc,"LOAD LIST",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addrfkwrsrc(ldvalrsrc,"SELECT",TRUE,okldval,load);
	addrfkwrsrc(ldvalrsrc,"QUIT",TRUE,quitldval,load);
	addbtnrsrc(ldvalrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ldvalrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(ldvalrsrc,TRUE,quitldval,load,FALSE);
}
static void makemodlistx()
{
	int x;
	char *tmp=NULL,*dirx=NULL;

	if(modlstxmtn!=NULL) freeapplib(modlstxmtn);
	modlstxmtn=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	for(x=0;findfile(dirx,"*.FIL",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(modlstxmtn,tmp);
	}
	if(modlstxmtn->numlibs<1)
	{
		addAPPlib(modlstxmtn,"No Modules Available");
	} else {
		SORTAPPlib(modlstxmtn);
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
}
/*
static void getfile(RDArsrc *mrsrc)
{
	char *libname=NULL,*modx=NULL;

	readwidget(mrsrc,"MODULE");
	FINDRSCGETSTRING(mrsrc,"MODULE",&modx);
	if(file_defs!=NULL) freeapplib(file_defs);
	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+10);
#ifndef WIN32
	sprintf(libname,"%s/rda/%s.FIL",CURRENTDIRECTORY,modx);
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modx);
#endif
	if(!isEMPTY(modx)) file_defs=getlibnames(libname,TRUE);
	if(libname!=NULL) Rfree(libname);
	if(modx!=NULL) Rfree(modx);
	if(file_defs==NULL)
	{
		file_defs=APPlibNEW();
		addAPPlib(file_defs,"Contains No File Definitions");
	}
	FINDRSCLISTAPPlib(filersrc,"FILE LIST",0,file_defs);
	APPmakescrn(filersrc,TRUE,NULL,NULL,FALSE);
}
*/
static void quitmaintain(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	MaintainButton *b1;
	ButtonField *bf1;
	int x,y,z;
	RDAconfld *confld;
	RDAsearch *tempsupp;
	RDAsubord *tempsub;
	PowerWrite *Write=NULL;
	PowerWriteField *WF=NULL;
	MTNAction *action=NULL;
	Actionstruct *actstruct=NULL;

	if(suppavlmtn!=NULL) freeapplib(suppavlmtn);
	if(subavlmtn!=NULL) freeapplib(subavlmtn);
	if(ctypesmtn!=NULL) freeapplib(ctypesmtn);
	if(newwrtavl!=NULL) freeapplib(newwrtavl);
	if(prevwrtavl!=NULL) freeapplib(prevwrtavl);
	if(tempPrevWrites!=NULL)
	{
		for(x=0,Write=tempPrevWrites;x<tmpnumPrevWrites;++x,++Write)
		{
			if(Write->module!=NULL) Rfree(Write->module);
			if(Write->filename!=NULL) Rfree(Write->filename);
			if(Write->expression!=NULL) Rfree(Write->expression);
			if(Write->fields!=NULL)
			{
				for(y=0,WF=Write->fields;y<Write->numflds;++y,++WF)
				{
					if(WF->to_name!=NULL) Rfree(WF->to_name);
					if(WF->from_name!=NULL) Rfree(WF->from_name);
				}
				Rfree(Write->fields);
			}
		}
		Rfree(tempPrevWrites);
		tempPrevWrites=NULL;
		tmpnumPrevWrites=0;
	} 
	if(tempNewWrites!=NULL)
	{
		for(x=0,Write=tempNewWrites;x<tmpnumNewWrites;++x,++Write)
		{
			if(Write->module!=NULL) Rfree(Write->module);
			if(Write->filename!=NULL) Rfree(Write->filename);
			if(Write->expression!=NULL) Rfree(Write->expression);
			if(Write->fields!=NULL)
			{
				for(y=0,WF=Write->fields;y<Write->numflds;++y,++WF)
				{
					if(WF->to_name!=NULL) Rfree(WF->to_name);
					if(WF->from_name!=NULL) Rfree(WF->from_name);
				}
				Rfree(Write->fields);
			}
		}
		Rfree(tempNewWrites);
		tempNewWrites=NULL;
		tmpnumNewWrites=0;
	} 
	if(tempsupport!=NULL)
	{
		for(x=0,tempsupp=tempsupport;x<tmpnumsupp;++x,++tempsupp)
		{
			if(tempsupp->module!=NULL) Rfree(tempsupp->module);
			if(tempsupp->filename!=NULL) Rfree(tempsupp->filename);
			if(tempsupp->cmodule!=NULL) Rfree(tempsupp->cmodule);
			if(tempsupp->cfilename!=NULL) Rfree(tempsupp->cfilename);
			if(tempsupp->confld!=NULL)
			{
				for(z=0,confld=tempsupp->confld;z<tempsupp->num;
					++z,++confld)
				{
					if(confld->fldname!=NULL) Rfree(confld->fldname);
					if(confld->conname!=NULL) Rfree(confld->conname);
				}
				Rfree(tempsupp->confld);
			}
			if(tempsupp->subkeyname!=NULL) Rfree(tempsupp->subkeyname);
			if(tempsupp->expression!=NULL) Rfree(tempsupp->expression);
			if(tempsupp->button_name!=NULL) Rfree(tempsupp->button_name);
			if(tempsupp->browse_screen!=NULL) Rfree(tempsupp->browse_screen);
			if(tempsupp->define_list!=NULL) Rfree(tempsupp->define_list);
			if(tempsupp->search_browse!=NULL) Rfree(tempsupp->search_browse);
		}
		Rfree(tempsupport);
		tmpnumsupp=0;
	}
	if(tempsubord!=NULL)
	{
		for(x=0,tempsub=tempsubord;x<tmpnumsub;++x,++tempsub)
		{
			if(tempsub->module!=NULL) Rfree(tempsub->module);
			if(tempsub->filename!=NULL) Rfree(tempsub->filename);
			if(tempsub->cmodule!=NULL) Rfree(tempsub->cmodule);
			if(tempsub->cfilename!=NULL) Rfree(tempsub->cfilename);
			if(tempsub->confld!=NULL)
			{
				for(z=0,confld=tempsub->confld;z<tempsub->num;
					++z,++confld)
				{
					if(confld->fldname!=NULL) Rfree(confld->fldname);
					if(confld->conname!=NULL) Rfree(confld->conname);
				}
				Rfree(tempsub->confld);
			}
			if(tempsub->subkeyname!=NULL) Rfree(tempsub->subkeyname);
			if(tempsubord->Writes!=NULL)
			{
				for(z=0,Write=tempsubord->Writes;z<tempsubord->numWrites;++z,++Write)
				{
					if(Write->module!=NULL) Rfree(Write->module);
					if(Write->filename!=NULL) Rfree(Write->filename);
					if(Write->expression!=NULL) Rfree(Write->expression);
					if(Write->fields!=NULL)
					{
						for(y=0,WF=Write->fields;y<Write->numflds;++y,++WF)
						{
							if(WF->to_name!=NULL) Rfree(WF->to_name);
							if(WF->from_name!=NULL) Rfree(WF->from_name);
						}
						Rfree(Write->fields);
					}
				}
				Rfree(tempsubord->Writes);
			}
			tempsubord->Writes=NULL;
			tempsubord->numWrites=0;
		}
		Rfree(tempsubord);
		tmpnumsub=0;
	}
	if(HoldButtons!=NULL)
	{
		for(x=0,b1=HoldButtons;x<numHoldButtons;++x,++b1)
		{
			if(b1->progname!=NULL) Rfree(b1->progname);
			if(b1->modulename!=NULL) Rfree(b1->modulename);
			if(b1->filename!=NULL) Rfree(b1->filename);
			if(b1->keyname!=NULL) Rfree(b1->keyname);
			if(b1->listname!=NULL) Rfree(b1->listname);
			if(b1->buttonname!=NULL) Rfree(b1->buttonname);
			if(b1->expression!=NULL) Rfree(b1->expression);
			if(b1->args!=NULL) freeapplib(b1->args);
			if(b1->bf!=NULL)
			{
				for(y=0,bf1=b1->bf;y<b1->num;++y,++bf1)
				{
					if(bf1->rname!=NULL) Rfree(bf1->rname);
					if(bf1->ename!=NULL) Rfree(bf1->ename);
				}
				Rfree(b1->bf);
			}
		}
		Rfree(HoldButtons);
		numHoldButtons=0;
	}
	if(tempActions!=NULL)
	{
		for(x=0,action=tempActions;x<tmpnumActions;++x,++action)
		{
			if(action->name!=NULL) Rfree(action->name);
			if(action->a!=NULL)
			{
				for(y=0,actstruct=action->a;y<action->num;++y,++actstruct)
				{
					if(actstruct->to_name!=NULL) Rfree(actstruct->to_name);
					if(actstruct->expression!=NULL) Rfree(actstruct->expression);
				}
				Rfree(action->a);
			}
		}
		Rfree(tempActions);
		tempActions=NULL;
		tmpnumActions=0;
	}
	if(styles!=NULL) freeapplib(styles);
	if(ftypes!=NULL) freeapplib(ftypes);
	if(starttypes!=NULL) freeapplib(starttypes);
	if(startwheres!=NULL) freeapplib(startwheres);
	if(file_defs!=NULL) freeapplib(file_defs);
	if(buttonsavl!=NULL) freeapplib(buttonsavl);
	if(btntypes!=NULL) freeapplib(btntypes);
	changedvalues=FALSE;
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
	if(filersrc!=NULL) free_rsrc(filersrc);
	if(orgmodulename!=NULL) Rfree(orgmodulename);
	if(orgname!=NULL) Rfree(orgname);
	if(mtntdefaults!=NULL) FreeRDAdefault(mtntdefaults);
}
static void quitmaintaintest(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	readallwidgets(mtnrsrc);
	if(COMPARE_RSRCS(mtnrsrc,mtntdefaults,2) || changedvalues)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","There are changes to the MAINTAIN MASTER DEFINITION.\nDo you want to Save these changes?",savemaintain,quitmaintain,FALSE,2,mtnrsrc,mainrsrc,NULL);
	} else {
		quitmaintain(mtnrsrc,mainrsrc);
	}
}
static void changestyles(RDArsrc *mtnrsrc)
{
	int style_selected=0;

	if(FINDRSCGETINT(mtnrsrc,"STYLES",&style_selected)) return;
	if(style_selected==1)
	{
		FINDRSCSETEDITABLE(mtnrsrc,"SUPPORTING FILES",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SUPPORTING FILES",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"ADD SUPPORTING ABOVE",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"ADD SUPPORTING BELOW",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"SELECT SUPPORTING",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"DELETE SUPPORTING",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"SAVE ERROR",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD VALUE SAVE ERROR",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD OPERAND SAVE ERROR",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD GROUPER SAVE ERROR",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD CONTROL SAVE ERROR",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"SAVE ERROR DESCRIPTION",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD VALUE SAVE ERROR DESCRIPTION",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD OPERAND SAVE ERROR DESCRIPTION",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD GROUPER SAVE ERROR DESCRIPTION",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD CONTROL SAVE ERROR DESCRIPTION",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"SAVE WARNING",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD VALUE SAVE WARNING",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD OPERAND SAVE WARNING",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD GROUPER SAVE WARNING",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD CONTROL SAVE WARNING",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"SAVE WARNING DESCRIPTION",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD VALUE SAVE WARNING DESCRIPTION",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD OPERAND SAVE WARNING DESCRIPTION",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD GROUPER SAVE WARNING DESCRIPTION",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD CONTROL SAVE WARNING DESCRIPTION",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE ERROR",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE ERROR DESCRIPTION",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE WARNING",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE WARNING DESCRIPTION",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"INHERIT RESOURCE VALUES",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"INHERIT RESOURCE VALUES",FALSE);
	} else {
		FINDRSCSETSENSITIVE(mtnrsrc,"SUPPORTING FILES",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"SUPPORTING FILES",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"ADD SUPPORTING ABOVE",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"ADD SUPPORTING BELOW",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"SELECT SUPPORTING",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"DELETE SUPPORTING",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"SAVE ERROR",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"SAVE ERROR DESCRIPTION",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"SAVE WARNING",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"SAVE WARNING DESCRIPTION",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE ERROR",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD VALUE SAVE ERROR",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD OPERAND SAVE ERROR",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD GROUPER SAVE ERROR",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD CONTROL SAVE ERROR",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE ERROR DESCRIPTION",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD VALUE SAVE ERROR DESCRIPTION",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD OPERAND SAVE ERROR DESCRIPTION",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD GROUPER SAVE ERROR DESCRIPTION",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD CONTROL SAVE ERROR DESCRIPTION",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE WARNING",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD VALUE SAVE WARNING",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD OPERAND SAVE WARNING",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD GROUPER SAVE WARNING",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD CONTROL SAVE WARNING",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"SAVE WARNING DESCRIPTION",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD VALUE SAVE WARNING DESCRIPTION",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD OPERAND SAVE WARNING DESCRIPTION",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD GROUPER SAVE WARNING DESCRIPTION",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD CONTROL SAVE WARNING DESCRIPTION",TRUE);
		if(style_selected==1)
		{
			FINDRSCSETSENSITIVE(mtnrsrc,"INHERIT RESOURCE VALUES",TRUE);
			FINDRSCSETEDITABLE(mtnrsrc,"INHERIT RESOURCE VALUES",TRUE);
		
		} else {
			FINDRSCSETSENSITIVE(mtnrsrc,"INHERIT RESOURCE VALUES",FALSE);
			FINDRSCSETEDITABLE(mtnrsrc,"INHERIT RESOURCE VALUES",FALSE);
		}
	}
}
static void savemaintain(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	char inheritrsrcs=FALSE;
	int x,y;
	MaintainButton *b1,*b2;
	ButtonField *bf1,*bf2;

	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"NAME",&report->MTNMSTR->name);
	if(!isEMPTY(report->MTNMSTR->name))
	{
		FINDRSCGETCHAR(mtnrsrc,"USE POWER WRITE",&report->MTNMSTR->UsePowerWrite);
		FINDRSCGETCHAR(mtnrsrc,"USE AUDIT TRAIL",&report->MTNMSTR->Use_Audit_Trail);
		FINDRSCGETSTRING(mtnrsrc,"AUDIT TRAIL",&report->MTNMSTR->Audit_Trail);
		FINDRSCGETSTRING(mtnrsrc,"DESCRIPTION",&report->MTNMSTR->desc);
		FINDRSCGETINT(mtnrsrc,"STYLES",&report->MTNMSTR->style);
		FINDRSCGETSTRING(mtnrsrc,"MAINFILE",&report->MTNMSTR->mainfile);
		FINDRSCGETSTRING(mtnrsrc,"BROWSE SCREEN",&report->MTNMSTR->browse_screen);
		FINDRSCGETSTRING(mtnrsrc,"DEFINE LIST",&report->MTNMSTR->define_list);
		FINDRSCGETSTRING(mtnrsrc,"SEARCH BROWSE",&report->MTNMSTR->search_browse);
		FINDRSCGETSTRING(mtnrsrc,"MAINTAIN SCREEN",&report->MTNMSTR->maintain_screen);
		FINDRSCGETINT(mtnrsrc,"FILE TYPES",&report->MTNMSTR->file_type);
		FINDRSCGETINT(mtnrsrc,"START TYPES",&report->MTNMSTR->start_with);
		FINDRSCGETINT(mtnrsrc,"START WHERES",&report->MTNMSTR->start_where);
		FINDRSCGETSTRING(mtnrsrc,"SAVE ERROR",&report->MTNMSTR->save_expression);
		FINDRSCGETSTRING(mtnrsrc,"SAVE ERROR DESCRIPTION",&report->MTNMSTR->save_error_desc);
		FINDRSCGETSTRING(mtnrsrc,"SAVE WARNING",&report->MTNMSTR->save_warning);
		FINDRSCGETSTRING(mtnrsrc,"SAVE WARNING DESCRIPTION",&report->MTNMSTR->save_warning_desc);
		FINDRSCGETCHAR(mtnrsrc,"INHERIT RESOURCE VALUES",&inheritrsrcs);
		report->MTNMSTR->inheritrsrcs=(short)inheritrsrcs;
		if(report->MTNMSTR->buttons!=NULL)
		{
			for(x=0,b1=report->MTNMSTR->buttons;x<report->MTNMSTR->numbuttons;++x,++b1)
			{
				if(b1->progname!=NULL) Rfree(b1->progname);
				if(b1->modulename!=NULL) Rfree(b1->modulename);
				if(b1->filename!=NULL) Rfree(b1->filename);
				if(b1->keyname!=NULL) Rfree(b1->keyname);
				if(b1->listname!=NULL) Rfree(b1->listname);
				if(b1->buttonname!=NULL) Rfree(b1->buttonname);
				if(b1->expression!=NULL) Rfree(b1->expression);
				if(b1->args!=NULL) freeapplib(b1->args);
				if(b1->bf!=NULL)
				{
					for(y=0,bf1=b1->bf;y<b1->num;++y,++bf1)
					{
						if(bf1->rname!=NULL) Rfree(bf1->rname);
						if(bf1->ename!=NULL) Rfree(bf1->ename);
					}
					Rfree(b1->bf);
				}
			}
			Rfree(report->MTNMSTR->buttons);
			report->MTNMSTR->numbuttons=0;
		}
		if(HoldButtons!=NULL)
		{
			report->MTNMSTR->numbuttons=numHoldButtons;
			report->MTNMSTR->buttons=Rmalloc(numHoldButtons*sizeof(MaintainButton));
			for(x=0,b1=HoldButtons,b2=report->MTNMSTR->buttons;x<numHoldButtons;
				++x,++b1,++b2)
			{
				if(b1->progname!=NULL)
				{
					b2->progname=stralloc(b1->progname);
					Rfree(b1->progname);
				} else b2->progname=NULL;
				if(b1->modulename!=NULL)
				{
					b2->modulename=stralloc(b1->modulename);
					Rfree(b1->modulename);
				} else b2->modulename=NULL;
				if(b1->filename!=NULL)
				{
					b2->filename=stralloc(b1->filename);
					Rfree(b1->filename);
				} else b2->filename=NULL;
				if(b1->keyname!=NULL)
				{
					b2->keyname=stralloc(b1->keyname);
					Rfree(b1->keyname);
				} else b2->keyname=NULL;
				if(b1->listname!=NULL)
				{
					b2->listname=stralloc(b1->listname);
					Rfree(b1->listname);
				} else b2->listname=NULL;
				if(b1->buttonname!=NULL)
				{
					b2->buttonname=stralloc(b1->buttonname);
					Rfree(b1->buttonname);
				} else b2->buttonname=NULL;
				if(b1->expression!=NULL)
				{
					b2->expression=stralloc(b1->expression);
					Rfree(b1->expression);
				} else b2->expression=NULL;
				b2->type=b1->type;
				b2->evalargs=b1->evalargs;
				if(b1->args!=NULL)
				{
					b2->args=APPlibNEW();
					for(y=0;y<b1->args->numlibs;++y) 
					{
						addAPPlib(b2->args,b1->args->libs[y]);
					}
					freeapplib(b1->args);
				} else b2->args=NULL;
				if(b1->bf!=NULL)
				{
					b2->num=b1->num;
					b2->bf=Rmalloc(b2->num*sizeof(ButtonField));
					for(y=0,bf1=b1->bf,bf2=b2->bf;y<b1->num;++y,++bf1,++bf2)
					{
						if(bf1->rname!=NULL)
						{
							bf2->rname=stralloc(bf1->rname);
							Rfree(bf1->rname);
						} else bf2->rname=NULL;
						if(bf1->ename!=NULL)
						{
							bf2->ename=stralloc(bf1->ename);
							Rfree(bf1->ename);
						} else bf2->ename=NULL;
					}
					Rfree(b1->bf);
					b1->num=0;
				} else {
					b2->bf=NULL;
					b2->num=0;
				}
			}
			Rfree(HoldButtons);
			numHoldButtons=0;
		}
		MTNMSTR=report->MTNMSTR;
		savesupp(mtnrsrc,report->MTNMSTR);
		savesub(mtnrsrc,report->MTNMSTR);
		savenewwrt(mtnrsrc,report->MTNMSTR);
		saveprevwrt(mtnrsrc,report->MTNMSTR);
		saveaction(mtnrsrc,report->MTNMSTR);
/*
		FreeMaintainMaster(report->MTNMSTR);
		report->MTNMSTR=NULL;
*/
	}
	MTNMSTR=report->MTNMSTR;
	quitmaintain(mtnrsrc,mainrsrc);
}
static void makespacebtnfld(int pos)
{
	short x,y;
	ButtonField *temp,*temp1,*new_buttons;

	++numHoldButtonField;
	new_buttons=Rmalloc(numHoldButtonField*sizeof(ButtonField));
	y=0;
	for(x=0,temp=HoldButtonField;x<(numHoldButtonField-1);++x,++temp)
	{
		temp1=new_buttons+y;
		if(x==pos)
		{
			temp1->rname=NULL;
			temp1->ename=NULL;
			++y;
			temp1=new_buttons+y;
		}
		if(!isEMPTY(temp->rname))
		{
			temp1->rname=stralloc(temp->rname);
			Rfree(temp->rname);
		} else temp1->rname=NULL;
		if(!isEMPTY(temp->ename))
		{
			temp1->ename=stralloc(temp->ename);
			Rfree(temp->ename);
		} else temp1->ename=NULL;
		++y;
	}
	Rfree(HoldButtonField);
	HoldButtonField=Rmalloc(numHoldButtonField*sizeof(ButtonField));
	for(x=0,temp1=HoldButtonField,temp=new_buttons;x<numHoldButtonField;++x,
		++temp,++temp1)
	{
		if(!isEMPTY(temp->rname))
		{
			temp1->rname=stralloc(temp->rname);
			Rfree(temp->rname);
		} else temp1->rname=NULL;
		if(!isEMPTY(temp->ename))
		{
			temp1->ename=stralloc(temp->ename);
			Rfree(temp->ename);
		} else temp1->ename=NULL;
	}
	Rfree(new_buttons);
}
static void quitbtnfld(RDArsrc *btnfldrsrc,RDArsrc *buttonrsrc)
{
	if(btnflddefaults!=NULL) FreeRDAdefault(btnflddefaults);
	if(btnfldrsrc!=NULL) 
	{
		killwindow(btnfldrsrc);
		free_rsrc(btnfldrsrc);
	}
	if(changedbtnfld) changedbutton=TRUE;
}
static void quitbtnfldcls(RDArsrc *btnfldrsrc,RDArsrc *buttonrsrc)
{
	changedbtnfld=FALSE;
	quitbtnfld(btnfldrsrc,buttonrsrc);
}
static void savebtnfld(RDArsrc *btnfldrsrc,RDArsrc *buttonrsrc)
{
	int x,selected=0;
	ButtonField *p;

	readallwidgets(btnfldrsrc);
	if(FINDRSCGETINT(buttonrsrc,"FIELDS AVAILABLE",&x)) return;
	if(HoldButtonField==NULL)
	{
		HoldButtonField=Rmalloc(sizeof(ButtonField));
		HoldButtonField->rname=NULL;
		HoldButtonField->ename=NULL;
		numHoldButtonField=1;
	}
	p=HoldButtonField+x;
	FINDRSCGETINT(btnfldrsrc,"FIELD LIST",&selected);
	p->rname=stralloc(fieldsavl->libs[selected]);
	FINDRSCGETSTRING(btnfldrsrc,"ENVIRONMENT NAME",&p->ename);
	makebtnfldlist();
	if(x>=btnfldavl->numlibs) x=0;
	if(!FINDRSCLISTAPPlib(buttonrsrc,"FIELDS AVAILABLE",x,btnfldavl))
		updatersrc(buttonrsrc,"FIELDS AVAILABLE");
	if(COMPARE_RSRCS(btnfldrsrc,btnflddefaults,2))
	{
		changedbtnfld=TRUE;
	}
	quitbtnfld(btnfldrsrc,buttonrsrc);
}
static void savebtnfldabove(RDArsrc *btnfldrsrc,RDArsrc *buttonrsrc)
{
	int x;

	if(FINDRSCGETINT(buttonrsrc,"FIELDS AVAILABLE",&x)) return;
	makespacebtnfld(x);
	savebtnfld(btnfldrsrc,buttonrsrc);
}
static void savebtnfldbelow(RDArsrc *btnfldrsrc,RDArsrc *buttonrsrc)
{
	int x;

	if(FINDRSCGETINT(buttonrsrc,"FIELDS AVAILABLE",&x)) return;
	makespacebtnfld(++x);
	FINDRSCSETINT(buttonrsrc,"FIELDS AVAILABLE",x);
	updatersrc(buttonrsrc,"FIELDS AVAILABLE");
	savebtnfld(btnfldrsrc,buttonrsrc);
}
static void quitbtnfldtest(RDArsrc *btnfldrsrc,RDArsrc *buttonrsrc)
{
	readallwidgets(btnfldrsrc);
	if(COMPARE_RSRCS(btnfldrsrc,btnflddefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made, and are about to be aborted!\nDo you want to save these changes?",savebtnfld,quitbtnfldcls,FALSE,2,btnfldrsrc,buttonrsrc,NULL);
	} else {
		quitbtnfld(btnfldrsrc,buttonrsrc);
	}
}
static void quitbtnfldabovetest(RDArsrc *btnfldrsrc,RDArsrc *buttonrsrc)
{
	readallwidgets(btnfldrsrc);
	if(COMPARE_RSRCS(btnfldrsrc,btnflddefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made, and are about to be aborted!\nDo you want to save these changes?",savebtnfldabove,quitbtnfldcls,FALSE,2,btnfldrsrc,buttonrsrc,NULL);
	} else {
		quitbtnfld(btnfldrsrc,buttonrsrc);
	}
}
static void quitbtnfldbelowtest(RDArsrc *btnfldrsrc,RDArsrc *buttonrsrc)
{
	readallwidgets(btnfldrsrc);
	if(COMPARE_RSRCS(btnfldrsrc,btnflddefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made, and are about to be aborted!\nDo you want to save these changes?",savebtnfldbelow,quitbtnfldcls,FALSE,2,btnfldrsrc,buttonrsrc,NULL);
	} else {
		quitbtnfld(btnfldrsrc,buttonrsrc);
	}
}
static void setenvname(RDArsrc *btnfldrsrc)
{
	int x;
	char *temp=NULL;

	FINDRSCGETINT(btnfldrsrc,"FIELD LIST",&x);
	temp=adddashes(fieldsavl->libs[x]);
	FINDRSCSETSTRING(btnfldrsrc,"ENVIRONMENT NAME",temp);
	if(temp!=NULL) Rfree(temp);
	updatersrc(btnfldrsrc,"ENVIRONMENT NAME");
}
static void buttonfieldscreen(RDArsrc *buttonrsrc,int fldnum,char *rname,
	char *ename,void (*savefunc)(...),void (*quitfunc)(...))
{
	RDArsrc *btnfldrsrc=NULL;
	int x;

	btnfldrsrc=RDArsrcNEW(module,"MAINTAIN BUTTON FIELD SCREEN");
	addstdrsrc(btnfldrsrc,"NUMBER",LONGV,4,&fldnum,FALSE);
	for(x=0;x<fieldsavl->numlibs;++x)
	{
		if(!RDAstrcmp(rname,fieldsavl->libs[x])) break;
	}
	if(x>=fieldsavl->numlibs) x=0;
	addlstrsrc(btnfldrsrc,"FIELD LIST",&x,TRUE,setenvname,fieldsavl->numlibs,
		&fieldsavl->libs,NULL);
	addstdrsrc(btnfldrsrc,"ENVIRONMENT NAME",VARIABLETEXT,0,ename,TRUE);
	addbtnrsrc(btnfldrsrc,"SAVE",TRUE,savefunc,buttonrsrc);
	addbtnrsrc(btnfldrsrc,"QUIT",TRUE,quitfunc,buttonrsrc);
	addbtnrsrc(btnfldrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(btnfldrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	setenvname(btnfldrsrc);
	btnflddefaults=GetDefaults(btnfldrsrc);
	APPmakescrn(btnfldrsrc,TRUE,quitbtnfld,NULL,FALSE);
}
static void add_btnfld_above(RDArsrc *buttonrsrc)
{
	int x;

	if(FINDRSCGETINT(buttonrsrc,"FIELDS AVAILABLE",&x)) return;
	if(!RDAstrcmp(btnfldavl->libs[x],"No Fields Defined"))
	{
		buttonfieldscreen(buttonrsrc,x,NULL,NULL,savebtnfld,
			quitbtnfldtest);
	} else {
		buttonfieldscreen(buttonrsrc,x,NULL,NULL,savebtnfldabove,
			quitbtnfldabovetest);
	}
}
static void add_btnfld_below(RDArsrc *buttonrsrc)
{
	int x;

	if(FINDRSCGETINT(buttonrsrc,"FIELDS AVAILABLE",&x)) return;
	if(!RDAstrcmp(btnfldavl->libs[x],"No Fields Defined"))
	{
		buttonfieldscreen(buttonrsrc,x,NULL,NULL,savebtnfld,
			quitbtnfldtest);
	} else {
		buttonfieldscreen(buttonrsrc,++x,NULL,NULL,savebtnfldbelow,
			quitbtnfldbelowtest);
	}
}
static void select_btnfld(RDArsrc *buttonrsrc)
{
	int x;
	ButtonField *b=NULL;

	if(FINDRSCGETINT(buttonrsrc,"FIELDS AVAILABLE",&x)) return;
	if(!RDAstrcmp(btnfldavl->libs[x],"No Fields Defined"))
	{
		buttonfieldscreen(buttonrsrc,x,NULL,NULL,savebtnfld,quitbtnfldtest);
	} else {
		b=HoldButtonField+x;
		buttonfieldscreen(buttonrsrc,x,b->rname,b->ename,savebtnfld,
			quitbtnfldtest);
	}
}
static void delete_btnfld(RDArsrc *buttonrsrc)
{
	int x,y,value;
	ButtonField *temp,*new_part,*temp1;

	if(FINDRSCGETINT(buttonrsrc,"FIELDS AVAILABLE",&value)) return;
	if(numHoldButtonField==1 && !RDAstrcmp(btnfldavl->libs[value],"No Fields Defined")) return;
	if(numHoldButtonField>1)
	{
		new_part=Rmalloc((numHoldButtonField-1)*sizeof(ButtonField));
		y=0;
		for(x=0,temp=HoldButtonField;x<numHoldButtonField;++x,++temp)
		{
			if(x!=value)
			{
				temp1=new_part+y;
				if(!isEMPTY(temp->rname))
				{
					temp1->rname=stralloc(temp->rname);
					Rfree(temp->rname);
				} else temp1->rname=NULL;
				if(!isEMPTY(temp->ename))
				{
					temp1->ename=stralloc(temp->ename);
					Rfree(temp->ename);
				} else temp1->ename=NULL;
				++y;
			} else {
				if(temp->rname!=NULL) Rfree(temp->rname);
				if(temp->ename!=NULL) Rfree(temp->ename);
			}
		}
		Rfree(HoldButtonField);
		--numHoldButtonField;
		HoldButtonField=Rmalloc(numHoldButtonField*sizeof(ButtonField));
		for(x=0,temp1=HoldButtonField,temp=new_part;x<numHoldButtonField;
			++x,++temp,++temp1)
		{
			if(!isEMPTY(temp->rname))
			{
				temp1->rname=stralloc(temp->rname);
				Rfree(temp->rname);
			} else temp1->rname=NULL;
			if(!isEMPTY(temp->ename))
			{
				temp1->ename=stralloc(temp->ename);
				Rfree(temp->ename);
			} else temp1->ename=NULL;
		}
		Rfree(new_part);
	} else {
		temp=HoldButtonField;
		if(temp!=NULL)
 		{
 			if(temp->rname!=NULL) Rfree(temp->rname);
 			if(temp->ename!=NULL) Rfree(temp->ename);
 			Rfree(HoldButtonField);
			HoldButtonField=NULL;
 		}
 		numHoldButtonField=0;
	}
	makebtnfldlist();
	if(value>=btnfldavl->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(buttonrsrc,"FIELDS AVAILABLE",value,btnfldavl))
		updatersrc(buttonrsrc,"FIELDS AVAILABLE");
}
static void quit_button(RDArsrc *buttonrsrc,RDArsrc *mtnrsrc)
{
	int x;
	ButtonField *b;

	if(btnfldavl!=NULL) freeapplib(btnfldavl);
	if(tmpargs!=NULL) freeapplib(tmpargs);
	if(fieldsavl!=NULL) freeapplib(fieldsavl);
	if(filelist!=NULL) freeapplib(filelist);
	if(modlstxmtn!=NULL) freeapplib(modlstxmtn);
	if(keysavl!=NULL) freeapplib(keysavl);
	if(buttondefaults!=NULL) FreeRDAdefault(buttondefaults);
	fieldsavl=NULL;
	if(HoldButtonField!=NULL)
	{
		for(x=0,b=HoldButtonField;x<numHoldButtonField;++x,++b)
		{
			if(b->rname!=NULL) Rfree(b->rname);
			if(b->ename!=NULL) Rfree(b->ename);
		}
		Rfree(HoldButtonField);
		numHoldButtonField=0;
	}
	if(changedbutton) changedvalues=TRUE;
	if(buttonrsrc!=NULL) 
	{
		killwindow(buttonrsrc);
		free_rsrc(buttonrsrc);
	}
}
static void quit_buttoncls(RDArsrc *buttonrsrc,RDArsrc *mtnrsrc)
{
	changedbutton=FALSE;
	quit_button(buttonrsrc,mtnrsrc);
}
static void save_button(RDArsrc *buttonrsrc,RDArsrc *mtnrsrc)
{
	int x,y=0,selkey=0,selmod=0,selfil=0;
	MaintainButton *button;
	ButtonField *part,*part1;
	int type=0;

	if(FINDRSCGETINT(mtnrsrc,"BUTTONS AVAILABLE",&y)) return;
	if(HoldButtons==NULL)
	{
		HoldButtons=Rmalloc(sizeof(MaintainButton));
		HoldButtons->progname=NULL;
		HoldButtons->modulename=NULL;
		HoldButtons->filename=NULL;
		HoldButtons->keyname=NULL;
		HoldButtons->listname=NULL;
		HoldButtons->buttonname=NULL;
		HoldButtons->expression=NULL;
		HoldButtons->evalargs=FALSE;
		HoldButtons->args=NULL;
		HoldButtons->num=0;
		HoldButtons->bf=NULL;
		HoldButtons->args=NULL;
		numHoldButtons=1;
	}
	button=HoldButtons+y;
	readallwidgets(buttonrsrc);
	FINDRSCGETSTRING(buttonrsrc,"PROGRAM NAME",&button->progname);
	FINDRSCGETSTRING(buttonrsrc,"LIST NAME",&button->listname);
	FINDRSCGETINT(buttonrsrc,"MODULE LIST",&selmod);
	FINDRSCGETINT(buttonrsrc,"FILE LIST",&selfil);
	FINDRSCGETINT(buttonrsrc,"KEYS AVAILABLE",&selkey);
	FINDRSCGETCHAR(buttonrsrc,"EVALUATE ARGUMENTS",&button->evalargs);
	button->modulename=stralloc(modlstxmtn->libs[selmod]);
	button->filename=stralloc(filelist->libs[selfil]);
	button->keyname=stralloc(keysavl->libs[selkey]);
	if(button->args!=NULL) freeapplib(button->args);
	button->args=APPlibNEW();
	if(tmpargs!=NULL)
	{
		for(x=0;x<tmpargs->numlibs;++x) 
		{
			if(RDAstrcmp(tmpargs->libs[x],"No Arguments")) 
				addAPPlib(button->args,tmpargs->libs[x]);
		}
	}
	FINDRSCGETSTRING(buttonrsrc,"BUTTON NAME",&button->buttonname);
	FINDRSCGETSTRING(buttonrsrc,"EXPRESSION",&button->expression);
	FINDRSCGETINT(buttonrsrc,"TYPES",&type);
	button->type=(short)type;
	if(button->bf!=NULL)
	{
		for(x=0,part=button->bf;x<button->num;++x,++part)
		{
			if(part->rname!=NULL) Rfree(part->rname);
			if(part->ename!=NULL) Rfree(part->ename);
		}
		Rfree(button->bf);
		button->num=0;
	}
	if(HoldButtonField!=NULL)
	{
		button->num=numHoldButtonField;
		button->bf=Rmalloc(button->num*sizeof(ButtonField));
		for(x=0,part=button->bf,part1=HoldButtonField;x<numHoldButtonField;
			++x,++part,++part1)
		{
			if(!isEMPTY(part1->rname))
			{
				part->rname=stralloc(part1->rname);
				Rfree(part1->rname);
			} else part->rname=NULL;
			if(!isEMPTY(part1->ename))
			{
				part->ename=stralloc(part1->ename);
				Rfree(part1->ename);
			} else part->ename=NULL;
		}
		Rfree(HoldButtonField);
		numHoldButtonField=0;
	}
	makebuttonlist();
	if(y>=buttonsavl->numlibs) y=0;
	if(!FINDRSCLISTAPPlib(mtnrsrc,"BUTTONS AVAILABLE",y,buttonsavl))
		updatersrc(mtnrsrc,"BUTTONS AVAILABLE");
	if(COMPARE_RSRCS(buttonrsrc,buttondefaults,2) || changedbtnfld)
	{
		changedbutton=TRUE;
	}
	quit_button(buttonrsrc,mtnrsrc);
}
static void save_button_above(RDArsrc *buttonrsrc,RDArsrc *mtnrsrc)
{
	int x;

	if(FINDRSCGETINT(mtnrsrc,"BUTTONS AVAILABLE",&x)) return;
	makespacebuttons(x);
	save_button(buttonrsrc,mtnrsrc);
}
static void save_button_below(RDArsrc *buttonrsrc,RDArsrc *mtnrsrc)
{
	int x;

	if(FINDRSCGETINT(mtnrsrc,"BUTTONS AVAILABLE",&x)) return;
	makespacebuttons(++x);
	FINDRSCSETINT(mtnrsrc,"BUTTONS AVAILABLE",x);
	updatersrc(mtnrsrc,"BUTTONS AVAILABLE");
	save_button(buttonrsrc,mtnrsrc);
}
static void quitbuttontest(RDArsrc *buttonrsrc,RDArsrc *mtnrsrc)
{
	readallwidgets(mtnrsrc);
	if(COMPARE_RSRCS(mtnrsrc,mtntdefaults,2) || changedvalues)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes were made that are going to be aborted!.\nDo you want to save these changes?",save_button,quit_buttoncls,FALSE,2,buttonrsrc,mtnrsrc,NULL);
	} else {
		quit_button(buttonrsrc,mtnrsrc);
	}
}
static void quitbuttonabovetest(RDArsrc *buttonrsrc,RDArsrc *mtnrsrc)
{
	readallwidgets(mtnrsrc);
	if(COMPARE_RSRCS(mtnrsrc,mtntdefaults,2) || changedbtnfld)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes were made that are going to be aborted!.\nDo you want to save these changes?",save_button_above,quit_buttoncls,FALSE,2,buttonrsrc,mtnrsrc,NULL);
	} else {
		quit_button(buttonrsrc,mtnrsrc);
	}
}
static void quitbuttonbelowtest(RDArsrc *buttonrsrc,RDArsrc *mtnrsrc)
{
	readallwidgets(mtnrsrc);
	if(COMPARE_RSRCS(mtnrsrc,mtntdefaults,2) || changedbtnfld)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes were made that are going to be aborted!.\nDo you want to save these changes?",save_button_below,quit_buttoncls,FALSE,2,buttonrsrc,mtnrsrc,NULL);
	} else {
		quit_button(buttonrsrc,mtnrsrc);
	}
}
static APPlib *makebtntypeslist()
{
	if(btntypes!=NULL) freeapplib(btntypes);
	btntypes=APPlibNEW();
	addAPPlib(btntypes,"Button Only");
	addAPPlib(btntypes,"Pre-Save");
	addAPPlib(btntypes,"Post-Save");
	addAPPlib(btntypes,"Pre-Quit");
	return(btntypes);
}
static void makebtnfldlist()
{
	int x,l;
	ButtonField *button;
	char *s=NULL;

	if(btnfldavl!=NULL) freeapplib(btnfldavl);
	btnfldavl=APPlibNEW();
	if(HoldButtonField==NULL || numHoldButtonField<=0)
	{
		addAPPlib(btnfldavl,"No Fields Defined");
	} else {
		for(x=0,button=HoldButtonField;x<numHoldButtonField;++x,++button)
		{
			l=RDAstrlen(button->rname)+RDAstrlen(button->ename)+27;
			s=Rmalloc(l);
			sprintf(s,"Field: [%s] Environment: [%s]",
				(button->rname!=NULL ? button->rname:""),
				(button->ename!=NULL ? button->ename:""));
			addAPPlib(btnfldavl,s);
			Rfree(s);
		}
	}
}
static void changefilelist(RDArsrc *mainrsrc)
{
	int selected=0,selectedf=0,selectedk=0;
	short x,y,engine=0,nokeys=0,nofields=0;
	NRDfield *fieldx=NULL,*field;
	NRDkey *keyx=NULL,*key;
	NRDpart *part;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"FILE LIST",&selectedf)) return;
	FINDRSCGETINT(mainrsrc,"KEYS AVAILABLE",&selectedk);
	if(keysavl!=NULL) freeapplib(keysavl); 
	keysavl=APPlibNEW();
	if(!RDAstrcmp(filelist->libs[selectedf],"SORTFILE"))
	{
		addAPPlib(keysavl,"USER DEFINED KEY");
	} else {
		if(!GETBIN(modlstxmtn->libs[selected],filelist->libs[selectedf],&engine,&nofields,&fieldx,&nokeys,&keyx))
		{
			if(keyx!=NULL)
			{
				for(x=0,key=keyx;x<nokeys;++x,++key)
				{
					addAPPlib(keysavl,key->name);
					if(key->name!=NULL) Rfree(key->name);
					if(key->part!=NULL)
					{
						for(y=0,part=key->part;y<key->numparts;++y,++part)
						{
							if(part->name!=NULL) Rfree(part->name);
						}
						Rfree(key->part);
					}
				}
				Rfree(keyx);
			}
			if(fieldx!=NULL)
			{
				for(x=0,field=fieldx;x<nofields;++x,++field)
				{
					if(field->name!=NULL) Rfree(field->name);
				}
				Rfree(fieldx);
			}
		}
	}
	if(keysavl->numlibs<1)
	{
		addAPPlib(keysavl,"No Keys Defined");
	}
	if(selectedk>=keysavl->numlibs) selectedk=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"KEYS AVAILABLE",selectedk,keysavl))
	{
		updatersrc(mainrsrc,"KEYS AVAILABLE");
	}
}
static void changemodlist(RDArsrc *mainrsrc)
{
	int selected=0,selectedf=0;
	char *libx=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected))
	{
		return;
	}
	FINDRSCGETINT(mainrsrc,"FILE LIST",&selectedf);
	if(filelist!=NULL)
	{
		freeapplib(filelist); 
	}
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlstxmtn->libs[selected])+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modlstxmtn->libs[selected]);
#endif 
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modlstxmtn->libs[selected]);
#endif
	if(!isEMPTY(modlstxmtn->libs[selected]))
	{
		filelist=getlibnames(libx,TRUE);
	}
	if(libx!=NULL) Rfree(libx);
	if(filelist==NULL)
	{
		filelist=APPlibNEW();
		addAPPlib(filelist,"No Files Defined");
	}
	if(selectedf>=filelist->numlibs)
	{
		selectedf=0;
	}
	if(!FINDRSCLISTAPPlib(mainrsrc,"FILE LIST",selectedf,filelist))
	{
		updatersrc(mainrsrc,"FILE LIST");
	}
	changefilelist(mainrsrc);
}
static void quit_arg(RDArsrc *argrsrc,RDArsrc *buttonrsrc)
{
	if(argdefaults!=NULL) FreeRDAdefault(argdefaults);
	if(argrsrc!=NULL) 
	{
		killwindow(argrsrc);
		free_rsrc(argrsrc);
	}
	if(changedargs) changedbutton=TRUE;
}
static void quit_argcls(RDArsrc *argrsrc,RDArsrc *buttonrsrc)
{
	changedargs=FALSE;
	quit_arg(argrsrc,buttonrsrc);
}
static void save_arg(RDArsrc *argrsrc,RDArsrc *buttonrsrc)
{
	int x,selected=0;
	char *temp=NULL;
	APPlib *a=NULL;

	readallwidgets(argrsrc);
	if(FINDRSCGETINT(buttonrsrc,"ARGUMENT LIST",&selected)) return;
	FINDRSCGETSTRING(argrsrc,"ARGUMENT",&temp);
	if(temp!=NULL)
	{
		a=tmpargs;
		tmpargs=APPlibNEW();
		for(x=0;x<a->numlibs;++x)
		{
			if(x==selected) addAPPlib(tmpargs,temp);
				else addAPPlib(tmpargs,a->libs[x]);
		}
		if(temp!=NULL) Rfree(temp);
		if(a!=NULL) freeapplib(a);
	}
	if(selected>=tmpargs->numlibs) selected=0;
	if(!FINDRSCLISTAPPlib(buttonrsrc,"ARGUMENT LIST",selected,tmpargs))
		updatersrc(buttonrsrc,"ARGUMENT LIST");
	if(COMPARE_RSRCS(argrsrc,argdefaults,2))
	{
		changedargs=TRUE;
	}
	quit_arg(argrsrc,buttonrsrc);
}
static void save_arg_above(RDArsrc *argrsrc,RDArsrc *buttonrsrc)
{
	int x,selected=0;
	char *temp=NULL;
	APPlib *a=NULL;

	readallwidgets(argrsrc);
	if(FINDRSCGETINT(buttonrsrc,"ARGUMENT LIST",&selected)) return;
	FINDRSCGETSTRING(argrsrc,"ARGUMENT",&temp);
	if(temp!=NULL)
	{
		a=tmpargs;
		tmpargs=APPlibNEW();
		for(x=0;x<a->numlibs;++x)
		{
			if(x==selected) addAPPlib(tmpargs,temp);
			addAPPlib(tmpargs,a->libs[x]);
		}
		if(temp!=NULL) Rfree(temp);
		if(a!=NULL) freeapplib(a);
	}
	if(selected>=tmpargs->numlibs) selected=0;
	if(!FINDRSCLISTAPPlib(buttonrsrc,"ARGUMENT LIST",selected,tmpargs))
		updatersrc(buttonrsrc,"ARGUMENT LIST");
	if(COMPARE_RSRCS(argrsrc,argdefaults,2))
	{
		changedargs=TRUE;
	}
	quit_arg(argrsrc,buttonrsrc);
}
static void save_arg_below(RDArsrc *argrsrc,RDArsrc *buttonrsrc)
{
	int x,selected=0;
	char *temp=NULL;
	APPlib *a=NULL;

	readallwidgets(argrsrc);
	if(FINDRSCGETINT(buttonrsrc,"ARGUMENT LIST",&selected)) return;
	FINDRSCGETSTRING(argrsrc,"ARGUMENT",&temp);
	if(temp!=NULL)
	{
		a=tmpargs;
		tmpargs=APPlibNEW();
		for(x=0;x<a->numlibs;++x)
		{
			addAPPlib(tmpargs,a->libs[x]);
			if(x==selected) addAPPlib(tmpargs,temp);
		}
		if(temp!=NULL) Rfree(temp);
		if(a!=NULL) freeapplib(a);
		if(COMPARE_RSRCS(argrsrc,argdefaults,2))
		{
			changedargs=TRUE;
		}
	}
	selected+=1;
	if(selected>=tmpargs->numlibs) selected=0;
	if(!FINDRSCLISTAPPlib(buttonrsrc,"ARGUMENT LIST",selected,tmpargs))
		updatersrc(buttonrsrc,"ARGUMENT LIST");
	quit_arg(argrsrc,buttonrsrc);
}
static void quit_arg_test(RDArsrc *argrsrc,RDArsrc *buttonrsrc)
{
	readallwidgets(argrsrc);
	if(COMPARE_RSRCS(argrsrc,argdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made, and are about to be aborted!\nDo you want to save these changes?",save_arg,quit_argcls,FALSE,2,argrsrc,buttonrsrc,NULL);
	} else {
		quit_arg(argrsrc,buttonrsrc);
	}
}
static void quit_arg_above_test(RDArsrc *argrsrc,RDArsrc *buttonrsrc)
{
	readallwidgets(argrsrc);
	if(COMPARE_RSRCS(argrsrc,argdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made, and are about to be aborted!\nDo you want to save these changes?",save_arg_above,quit_argcls,FALSE,2,argrsrc,buttonrsrc,NULL);
	} else {
		quit_arg(argrsrc,buttonrsrc);
	}
}
static void quit_arg_below_test(RDArsrc *argrsrc,RDArsrc *buttonrsrc)
{
	readallwidgets(argrsrc);
	if(COMPARE_RSRCS(argrsrc,argdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made, and are about to be aborted!\nDo you want to save these changes?",save_arg_below,quit_argcls,FALSE,2,argrsrc,buttonrsrc,NULL);
	} else {
		quit_arg(argrsrc,buttonrsrc);
	}
}
static void argumentscreen(RDArsrc *buttonrsrc,char *argument,
	void (*savefunc)(...),void (*quitfunc)(...))
{
	RDArsrc *argrsrc=NULL;
	char evalargs=FALSE;

	changedargs=FALSE;
	readwidget(buttonrsrc,"EVALUATE ARGUMENTS");
	FINDRSCGETCHAR(buttonrsrc,"EVALUATE ARGUMENTS",&evalargs);
	if(evalargs==FALSE)
	{
		argrsrc=RDArsrcNEW(module,"MAINTAIN ARGUMENT SCREEN");
		addstdrsrc(argrsrc,"ARGUMENT",VARIABLETEXT,0,(argument!=NULL?argument:""),TRUE);
	} else {
		argrsrc=RDArsrcNEW(module,"MAINTAIN ARGUMENT EVALUATE SCREEN");
		addsctrsrc(argrsrc,"ARGUMENT",0,(argument!=NULL?argument:""),TRUE);
	}
	addbtnrsrc(argrsrc,"SAVE",TRUE,savefunc,buttonrsrc);
	addbtnrsrc(argrsrc,"QUIT",TRUE,quitfunc,buttonrsrc);
	addbtnrsrc(argrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(argrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	argdefaults=GetDefaults(argrsrc);
	APPmakescrn(argrsrc,TRUE,quit_arg,NULL,FALSE);
}
static void add_arg_above(RDArsrc *buttonrsrc)
{
	int x;

	if(FINDRSCGETINT(buttonrsrc,"ARGUMENT LIST",&x)) return;
	if(!RDAstrcmp(tmpargs->libs[x],"No Arguments"))
	{
		argumentscreen(buttonrsrc,NULL,save_arg,quit_arg_test);
	} else {
		argumentscreen(buttonrsrc,NULL,save_arg_above,quit_arg_above_test);
	}
}
static void add_arg_below(RDArsrc *buttonrsrc)
{
	int x;

	if(FINDRSCGETINT(buttonrsrc,"ARGUMENT LIST",&x)) return;
	if(!RDAstrcmp(tmpargs->libs[x],"No Arguments"))
	{
		argumentscreen(buttonrsrc,NULL,save_arg,quit_arg_test);
	} else {
		argumentscreen(buttonrsrc,NULL,save_arg_below,quit_arg_below_test);
	}
}
static void select_arg(RDArsrc *buttonrsrc)
{
	int x;

	if(FINDRSCGETINT(buttonrsrc,"ARGUMENT LIST",&x)) return;
	if(!RDAstrcmp(tmpargs->libs[x],"No Arguments"))
	{
		argumentscreen(buttonrsrc,NULL,save_arg,quit_arg_test);
	} else {
		argumentscreen(buttonrsrc,tmpargs->libs[x],save_arg,quit_arg_test);
	}
}
static void delete_arg(RDArsrc *buttonrsrc)
{
	int selected=0,x=0;
	APPlib *a=NULL;

	FINDRSCGETINT(buttonrsrc,"ARGUMENT LIST",&selected);
	if(tmpargs->numlibs>0 && RDAstrcmp(tmpargs->libs[1],"No Arguments"))
	{
		a=tmpargs;
		tmpargs=APPlibNEW();
		for(x=0;x<a->numlibs;++x)
		{
			if(x!=selected)
			{
				addAPPlib(tmpargs,a->libs[x]);
			}
		}
		freeapplib(a);
	} else {
		freeapplib(tmpargs);
		tmpargs=APPlibNEW();
	}
	if(tmpargs->numlibs<1)
	{
		addAPPlib(tmpargs,"No Arguments");
	}
	if(selected>=tmpargs->numlibs) selected=0;
	FINDRSCLISTAPPlib(buttonrsrc,"ARGUMENT LIST",selected,tmpargs);
	updatersrc(buttonrsrc,"ARGUMENT LIST");
}
static void buttonscreen(RDArsrc *mtnrsrc,RDArsrc *mainrsrc,int buttonnum,
	char *progname,char *listname,char *buttonname,char *expression,
	short type,int num,ButtonField *bf,
	void (*savefunc)(...),void (*quitfunc)(...),char *modulename,char *filename,
	char *keyname,char evalargs,APPlib *args)
{
	RDArsrc *buttonrsrc=NULL;
	int x,y,seltype=0;
	short engine=0,nokeys=0,nofields=0;
	ButtonField *b1,*b2;
	char *filx=NULL,*modx=NULL;
	NRDfield *fieldx=NULL,*field;
	NRDkey *key,*keyx=NULL;
	NRDpart *part=NULL;

	readwidget(mtnrsrc,"MODULE");
	readwidget(mtnrsrc,"MAINFILE");
	FINDRSCGETSTRING(mtnrsrc,"MODULE",&modx);
	FINDRSCGETSTRING(mtnrsrc,"MAINFILE",&filx);
	if(fieldsavl!=NULL) freeapplib(fieldsavl);
	fieldsavl=APPlibNEW();
	tmpargs=APPlibNEW();
	if(args!=NULL)
	{
		for(x=0;x<args->numlibs;++x)
		{
			addAPPlib(tmpargs,args->libs[x]);
		}
	}
	if(tmpargs->numlibs<1)
	{
		addAPPlib(tmpargs,"No Arguments");
	}
	if(!RDAstrcmp(filx,"SORTFILE"))
	{
		makefieldvallistrpt(&fieldsavl,TRUE,FALSE,TRUE,FALSE);
	} else {
		if(!GETBIN(modx,filx,&engine,&nofields,&fieldx,&nokeys,&keyx))
		{
			if(keyx!=NULL)
			{
				for(x=0,key=keyx;x<nokeys;++x,++key)
				{
					if(key->name!=NULL) Rfree(key->name);
					if(key->part!=NULL)
					{
						for(y=0,part=key->part;y<key->numparts;++y,++part)
						{
							if(part->name!=NULL) Rfree(part->name);
						}
						Rfree(key->part);
					}
				}
				Rfree(keyx);
			}
			if(fieldx!=NULL)
			{
				for(x=0,field=fieldx;x<nofields;++x,++field)
				{
					addAPPlib(fieldsavl,field->name);
					if(field->name!=NULL) Rfree(field->name);
				}
				Rfree(fieldx);
			}
		}
	}
	if(fieldsavl->numlibs<1)
	{
		addAPPlib(fieldsavl,"No Fields Defined");
	}
	if(modx!=NULL) Rfree(modx);
	if(filx!=NULL) Rfree(filx);
	if(bf!=NULL)
	{
		numHoldButtonField=num;
		HoldButtonField=Rmalloc(num*sizeof(ButtonField));
		for(x=0,b1=bf,b2=HoldButtonField;x<num;++x,++b1,++b2)
		{
			if(b1->rname!=NULL)
			{
				b2->rname=stralloc(b1->rname);
			} else {
				b2->rname=NULL;
			}
			if(b1->ename!=NULL)
			{
				b2->ename=stralloc(b1->ename);
			} else { 
				b2->ename=NULL;
			}
		}
	}
	buttonrsrc=RDArsrcNEW(module,"MAINTAIN BUTTON SCREEN");
	addstdrsrc(buttonrsrc,"NUMBER",LONGV,4,&buttonnum,FALSE);
	addstdrsrc(buttonrsrc,"PROGRAM NAME",VARIABLETEXT,0,progname,TRUE);
	addstdrsrc(buttonrsrc,"LIST NAME",VARIABLETEXT,0,listname,TRUE);
	addstdrsrc(buttonrsrc,"BUTTON NAME",VARIABLETEXT,0,buttonname,TRUE);
	addsctrsrc(buttonrsrc,"EXPRESSION",0,(expression!=NULL?expression:""),TRUE);
	addbtnrsrc(buttonrsrc,"LOAD VALUE",TRUE,loadvaluemtn,"EXPRESSION");
	addbtnrsrc(buttonrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(buttonrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(buttonrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	seltype=(int)type;
	addlstrsrc(buttonrsrc,"TYPES",&seltype,TRUE,NULL,btntypes->numlibs,&btntypes->libs,NULL);
	addstdrsrc(buttonrsrc,"EVALUATE ARGUMENTS",BOOLNS,1,&evalargs,TRUE);
	makebtnfldlist();
	addlstrsrc(buttonrsrc,"FIELDS AVAILABLE",0,TRUE,NULL,btnfldavl->numlibs,&btnfldavl->libs,NULL);
	makemodlistx();
	for(x=0;x<modlstxmtn->numlibs;++x)
	{
		if(!RDAstrcmp(modlstxmtn->libs[x],modulename)) break;
	} 
	if(x>=modlstxmtn->numlibs) 
	{
		x=0;
	}
	addlstrsrc(buttonrsrc,"MODULE LIST",&x,TRUE,changemodlist,modlstxmtn->numlibs,&modlstxmtn->libs,NULL);
	x=0;
	if(filelist!=NULL) freeapplib(filelist);
	filelist=APPlibNEW();
	addlstrsrc(buttonrsrc,"FILE LIST",&x,TRUE,changefilelist,filelist->numlibs,
		&filelist->libs,NULL);
	changemodlist(buttonrsrc);
	for(x=0;x<filelist->numlibs;++x)
	{
		if(!RDAstrcmp(filelist->libs[x],filename)) break;
	}
	if(x<filelist->numlibs)
	{
		FINDRSCSETINT(buttonrsrc,"FILE LIST",x);
	}
	keysavl=APPlibNEW();
	addlstrsrc(buttonrsrc,"KEYS AVAILABLE",&x,TRUE,NULL,keysavl->numlibs,&keysavl->libs,NULL);
	changefilelist(buttonrsrc);
	for(x=0;x<keysavl->numlibs;++x)
	{
		if(!RDAstrcmp(keyname,keysavl->libs[x])) break;
	}
	if(x<keysavl->numlibs)
	{
		FINDRSCSETINT(buttonrsrc,"KEYS AVAILABLE",x);
	}
	addbtnrsrc(buttonrsrc,"ADD BUTTON ABOVE",TRUE,add_btnfld_above,NULL);
	addbtnrsrc(buttonrsrc,"ADD BUTTON BELOW",TRUE,add_btnfld_below,NULL);
	addbtnrsrc(buttonrsrc,"SELECT BUTTON",TRUE,select_btnfld,NULL);
	addbtnrsrc(buttonrsrc,"DELETE BUTTON",TRUE,delete_btnfld,NULL);
	x=0;
	addlstrsrc(buttonrsrc,"ARGUMENT LIST",&x,TRUE,NULL,tmpargs->numlibs,&tmpargs->libs,NULL);
	addbtnrsrc(buttonrsrc,"ADD ARGUMENT ABOVE",TRUE,add_arg_above,NULL);
	addbtnrsrc(buttonrsrc,"ADD ARGUMENT BELOW",TRUE,add_arg_below,NULL);
	addbtnrsrc(buttonrsrc,"SELECT ARGUMENT",TRUE,select_arg,NULL);
	addbtnrsrc(buttonrsrc,"DELETE ARGUMENT",TRUE,delete_arg,NULL);
	addbtnrsrc(buttonrsrc,"SAVE",TRUE,savefunc,mtnrsrc);
	addbtnrsrc(buttonrsrc,"QUIT",TRUE,quitfunc,mtnrsrc);
	addbtnrsrc(buttonrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(buttonrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	buttondefaults=GetDefaults(buttonrsrc);
	APPmakescrn(buttonrsrc,TRUE,quit_button,NULL,FALSE);
}
static void makespacebuttons(int pos)
{
	short x,y,z;
	MaintainButton *temp,*temp1,*new_buttons;
	ButtonField *b1,*b2;

	++numHoldButtons;
	new_buttons=Rmalloc(numHoldButtons*sizeof(MaintainButton));
	y=0;
	for(x=0,temp=HoldButtons;x<(numHoldButtons-1);++x,++temp)
	{
		temp1=new_buttons+y;
		if(x==pos)
		{
			temp1->progname=NULL;
			temp1->modulename=NULL;
			temp1->filename=NULL;
			temp1->keyname=NULL;
			temp1->listname=NULL;
			temp1->buttonname=NULL;
			temp1->expression=NULL;
			temp1->type=0;
			temp1->args=NULL;
			temp1->num=0;
			temp1->bf=NULL;
			++y;
			temp1=new_buttons+y;
		}
		if(temp->progname!=NULL)
		{
			temp1->progname=stralloc(temp->progname);
			Rfree(temp->progname);
		} else temp1->progname=NULL;
		if(temp->modulename!=NULL)
		{
			temp1->modulename=stralloc(temp->modulename);
			Rfree(temp->modulename);
		} else temp1->modulename=NULL;
		if(temp->filename!=NULL)
		{
			temp1->filename=stralloc(temp->filename);
			Rfree(temp->filename);
		} else temp1->filename=NULL;
		if(temp->keyname!=NULL)
		{
			temp1->keyname=stralloc(temp->keyname);
			Rfree(temp->keyname);
		} else temp1->keyname=NULL;	
		if(temp->listname!=NULL)
		{
			temp1->listname=stralloc(temp->listname);
			Rfree(temp->listname);
		} else temp1->listname=NULL;
		if(temp->buttonname!=NULL)
		{
			temp1->buttonname=stralloc(temp->buttonname);
			Rfree(temp->buttonname);
		} else temp1->buttonname=NULL;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		temp1->type=temp->type;
		temp1->evalargs=temp->evalargs;
		if(temp->args!=NULL)
		{
			temp1->args=APPlibNEW();
			for(z=0;z<temp->args->numlibs;++z) 
			{
				addAPPlib(temp1->args,temp->args->libs[z]);
			}
			freeapplib(temp->args);
		} else temp1->args=NULL;
		if(temp->bf!=NULL)
		{
			temp1->num=temp->num;
			temp1->bf=Rmalloc(temp->num*sizeof(ButtonField));
			for(z=0,b1=temp->bf,b2=temp1->bf;z<temp->num;++z,++b1,++b2)
			{
				if(b1->rname!=NULL)
				{
					b2->rname=stralloc(b1->rname);
					Rfree(b1->rname);
				} else b2->rname=NULL;
				if(b1->ename!=NULL)
				{
					b2->ename=stralloc(b1->ename);
					Rfree(b1->ename);
				} else b2->ename=NULL;
			}
			Rfree(temp->bf);
			temp->num=0;
		} else {
			temp1->num=0;
			temp1->bf=NULL;
		}
		++y;
	}
	Rfree(HoldButtons);
	HoldButtons=Rmalloc(numHoldButtons*sizeof(MaintainButton));
	for(x=0,temp1=HoldButtons,temp=new_buttons;x<numHoldButtons;++x,
		++temp,++temp1)
	{
		if(temp->progname!=NULL)
		{
			temp1->progname=stralloc(temp->progname);
			Rfree(temp->progname);
		} else temp1->progname=NULL;
		if(temp->modulename!=NULL)
		{
			temp1->modulename=stralloc(temp->modulename);
			Rfree(temp->modulename);
		} else temp1->modulename=NULL;
		if(temp->filename!=NULL)
		{
			temp1->filename=stralloc(temp->filename);
			Rfree(temp->filename);
		} else temp1->filename=NULL;
		if(temp->keyname!=NULL)
		{
			temp1->keyname=stralloc(temp->keyname);
			Rfree(temp->keyname);
		} else temp1->keyname=NULL;
		if(temp->listname!=NULL)
		{
			temp1->listname=stralloc(temp->listname);
			Rfree(temp->listname);
		} else temp1->listname=NULL;
		if(temp->buttonname!=NULL)
		{
			temp1->buttonname=stralloc(temp->buttonname);
			Rfree(temp->buttonname);
		} else temp1->buttonname=NULL;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		temp1->type=temp->type;
		temp1->evalargs=temp->evalargs;
		if(temp->args!=NULL)
		{
			temp1->args=APPlibNEW();
			for(z=0;z<temp->args->numlibs;++z) 
			{
				addAPPlib(temp1->args,temp->args->libs[z]);
			}
			freeapplib(temp->args);
		} else temp1->args=NULL;
		if(temp->bf!=NULL)
		{
			temp1->num=temp->num;
			temp1->bf=Rmalloc(temp->num*sizeof(ButtonField));
			for(z=0,b1=temp->bf,b2=temp1->bf;z<temp->num;++z,++b1,++b2)
			{
				if(b1->rname!=NULL)
				{
					b2->rname=stralloc(b1->rname);
					Rfree(b1->rname);
				} else b2->rname=NULL;
				if(b1->ename!=NULL)
				{
					b2->ename=stralloc(b1->ename);
					Rfree(b1->ename);
				} else b2->ename=NULL;
			}
			Rfree(temp->bf);
			temp->num=0;
		} else {
			temp1->num=0;
			temp1->bf=NULL;
		}
	}
	Rfree(new_buttons);
}
static void makebuttonlist()
{
	int x,l;
	MaintainButton *button;
	char *s=NULL;

	if(buttonsavl!=NULL) freeapplib(buttonsavl);
	buttonsavl=APPlibNEW();
	if(HoldButtons==NULL || numHoldButtons<=0)
	{
		addAPPlib(buttonsavl,"No Buttons Available");
	} else {
		for(x=0,button=HoldButtons;x<numHoldButtons;++x,++button)
		{
			l=RDAstrlen(button->progname)+RDAstrlen(button->keyname)+
				RDAstrlen(button->listname)+
				RDAstrlen(button->modulename)+RDAstrlen(button->filename)+
				RDAstrlen(button->buttonname)+
				RDAstrlen(btntypes->libs[button->type])+74;
			s=Rmalloc(l);
			sprintf(s,"Program: [%s] Module: [%s] File: [%s] Keyname: [%s] List: [%s] Button: [%s] Type: [%s]",
				(button->progname!=NULL?button->progname:""),
				(button->modulename!=NULL?button->modulename:""),
				(button->filename!=NULL?button->filename:""),
				(button->keyname!=NULL?button->keyname:""),
				(button->listname!=NULL?button->listname:""),
				(button->buttonname!=NULL?button->buttonname:""),
				(btntypes->libs[button->type]!=NULL?btntypes->libs[button->type]:""));
			addAPPlib(buttonsavl,s);
			Rfree(s);
		}
	}
}
static void add_button_above(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	int x;
	char *modulx=NULL;

	readallwidgets(mtnrsrc);
	if(FINDRSCGETINT(mtnrsrc,"BUTTONS AVAILABLE",&x)) return;
	FINDRSCGETSTRING(mtnrsrc,"MODULE",&modulx);
	if(!RDAstrcmp(buttonsavl->libs[x],"No Buttons Available"))
	{
		buttonscreen(mtnrsrc,mainrsrc,x,NULL,NULL,NULL,NULL,0,0,NULL,
			save_button,quitbuttontest,modulx,NULL,NULL,FALSE,NULL);
	} else {
		buttonscreen(mtnrsrc,mainrsrc,x,NULL,NULL,NULL,NULL,0,0,NULL,
			save_button_above,quitbuttonabovetest,modulx,NULL,NULL,FALSE,NULL);
	}
	if(modulx!=NULL) Rfree(modulx);
}
static void add_button_below(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	int x;
	char *modulx=NULL;

	readallwidgets(mtnrsrc);
	if(FINDRSCGETINT(mtnrsrc,"BUTTONS AVAILABLE",&x)) return;
	FINDRSCGETSTRING(mtnrsrc,"MODULE",&modulx);
	if(!RDAstrcmp(buttonsavl->libs[x],"No Buttons Available"))
	{
		buttonscreen(mtnrsrc,mainrsrc,x,NULL,NULL,NULL,NULL,0,0,NULL,
			save_button,quitbuttontest,modulx,NULL,NULL,FALSE,NULL);
	} else {
		buttonscreen(mtnrsrc,mainrsrc,++x,NULL,NULL,NULL,NULL,0,0,NULL,
			save_button_below,quitbuttonbelowtest,modulx,NULL,NULL,FALSE,NULL);
	}
	if(modulx!=NULL) Rfree(modulx);
}
static void select_button(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	int x;
	MaintainButton *b=NULL;
	char *modulx=NULL;

	readallwidgets(mtnrsrc);
	if(FINDRSCGETINT(mtnrsrc,"BUTTONS AVAILABLE",&x)) return;
	FINDRSCGETSTRING(mtnrsrc,"MODULE",&modulx);
	if(!RDAstrcmp(buttonsavl->libs[x],"No Buttons Available"))
	{
		buttonscreen(mtnrsrc,mainrsrc,x,NULL,NULL,NULL,NULL,0,0,NULL,
			save_button,quitbuttontest,modulx,NULL,NULL,FALSE,NULL);
	} else {
		b=HoldButtons+x;
		buttonscreen(mtnrsrc,mainrsrc,x,b->progname,b->listname,
			b->buttonname,b->expression,b->type,b->num,
			b->bf,save_button,quitbuttontest,
			b->modulename,b->filename,b->keyname,b->evalargs,b->args);
	}
	if(modulx!=NULL) Rfree(modulx);
}
static void delete_button(RDArsrc *mtnrsrc)
{
	int x,y,z,value;
	MaintainButton *temp,*new_part,*temp1;
	ButtonField *b1,*b2;

	if(FINDRSCGETINT(mtnrsrc,"BUTTONS AVAILABLE",&value)) return;
	if(numHoldButtons==1 && !RDAstrcmp(buttonsavl->libs[value],"No Buttons Available")) return;
	if(numHoldButtons>1)
	{
		new_part=Rmalloc((numHoldButtons-1)*sizeof(MaintainButton));
		y=0;
		for(x=0,temp=HoldButtons;x<numHoldButtons;++x,++temp)
		{
			if(x!=value)
			{
				temp1=new_part+y;
				if(temp->progname!=NULL)
				{
					temp1->progname=stralloc(temp->progname);
					Rfree(temp->progname);
				} else temp1->progname=NULL;
				if(temp->modulename!=NULL)
				{
					temp1->modulename=stralloc(temp->modulename);
					Rfree(temp->modulename);
				} else temp1->modulename=NULL;
				if(temp->filename!=NULL)
				{
					temp1->filename=stralloc(temp->filename);
					Rfree(temp->filename);
				} else temp1->filename=NULL;
				if(temp->keyname!=NULL)
				{
					temp1->keyname=stralloc(temp->keyname);
					Rfree(temp->keyname);
				} else temp1->keyname=NULL;
				if(temp->listname!=NULL)
				{
					temp1->listname=stralloc(temp->listname);
					Rfree(temp->listname);
				} else temp1->listname=NULL;
				if(temp->buttonname!=NULL)
				{
					temp1->buttonname=stralloc(temp->buttonname);
					Rfree(temp->buttonname);
				} else temp1->buttonname=NULL;
				if(temp->expression!=NULL)
				{
					temp1->expression=stralloc(temp->expression);
					Rfree(temp->expression);
				} else temp1->expression=NULL;
				temp1->type=temp->type;
				temp1->evalargs=temp->evalargs;
				if(temp->args!=NULL)
				{
					temp1->args=APPlibNEW();
					for(z=0;z<temp->args->numlibs;++z) 
					{
						addAPPlib(temp1->args,temp->args->libs[z]);
					}
					freeapplib(temp->args);
				} else temp1->args=NULL;
				if(temp->bf!=NULL)
				{
					temp1->num=temp->num;
					temp1->bf=Rmalloc(temp->num*sizeof(ButtonField));
					for(z=0,b1=temp->bf,b2=temp1->bf;z<temp->num;++z,++b1,++b2)
					{
						if(b1->rname!=NULL)
						{
							b2->rname=stralloc(b1->rname);
							Rfree(b1->rname);
						} else b2->rname=NULL;
						if(b1->ename!=NULL)
						{
							b2->ename=stralloc(b1->ename);
							Rfree(b1->ename);
						} else b2->ename=NULL;
					}
					Rfree(temp->bf);
					temp->num=0;
				} else {
					temp1->num=0;
					temp1->bf=NULL;
				}
				++y;
			} else {
				if(temp->progname!=NULL) Rfree(temp->progname);
				if(temp->modulename!=NULL) Rfree(temp->modulename);
				if(temp->filename!=NULL) Rfree(temp->filename);
				if(temp->keyname!=NULL) Rfree(temp->keyname);
				if(temp->listname!=NULL) Rfree(temp->listname);
				if(temp->buttonname!=NULL) Rfree(temp->buttonname);
				if(temp->expression!=NULL) Rfree(temp->expression);
				if(temp->args!=NULL) freeapplib(temp->args);
				if(temp->bf!=NULL)
				{
					for(z=0,b1=temp->bf;z<temp->num;++z,++b1)
					{
						if(b1->rname!=NULL) Rfree(b1->rname);
						if(b1->ename!=NULL) Rfree(b1->ename);
					}
					Rfree(temp->bf);
					temp->num=0;
				}
			}
		}
		Rfree(HoldButtons);
		--numHoldButtons;
		HoldButtons=Rmalloc(numHoldButtons*sizeof(MaintainButton));
		for(x=0,temp1=HoldButtons,temp=new_part;x<numHoldButtons;++x,++temp,
			++temp1)
		{
			if(temp->progname!=NULL)
			{
				temp1->progname=stralloc(temp->progname);
				Rfree(temp->progname);
			} else temp1->progname=NULL;
			if(temp->modulename!=NULL)
			{
				temp1->modulename=stralloc(temp->modulename);
				Rfree(temp->modulename);
			} else temp1->modulename=NULL;
			if(temp->filename!=NULL)
			{
				temp1->filename=stralloc(temp->filename);
				Rfree(temp->filename);
			} else temp1->filename=NULL;
			if(temp->keyname!=NULL)
			{
				temp1->keyname=stralloc(temp->keyname);
				Rfree(temp->keyname);
			} else temp1->keyname=NULL;
			if(temp->listname!=NULL)
			{
				temp1->listname=stralloc(temp->listname);
				Rfree(temp->listname);
			} else temp1->listname=NULL;
			if(temp->buttonname!=NULL)
			{
				temp1->buttonname=stralloc(temp->buttonname);
				Rfree(temp->buttonname);
			} else temp1->buttonname=NULL;
			if(temp->expression!=NULL)
			{
				temp1->expression=stralloc(temp->expression);
				Rfree(temp->expression);
			} else temp1->expression=NULL;
			temp1->type=temp->type;
			temp1->evalargs=temp->evalargs;
			if(temp->args!=NULL)
			{
				temp1->args=APPlibNEW();
				for(z=0;z<temp->args->numlibs;++z) 
					addAPPlib(temp1->args,temp->args->libs[z]);
				freeapplib(temp->args);
			} else temp1->args=NULL;
			if(temp->bf!=NULL)
			{
				temp1->num=temp->num;
				temp1->bf=Rmalloc(temp->num*sizeof(ButtonField));
				for(z=0,b1=temp->bf,b2=temp1->bf;z<temp->num;++z,++b1,++b2)
				{
					if(b1->rname!=NULL)
					{
						b2->rname=stralloc(b1->rname);
						Rfree(b1->rname);
					} else b2->rname=NULL;
					if(b1->ename!=NULL)
					{
						b2->ename=stralloc(b1->ename);
						Rfree(b1->ename);
					} else b2->ename=NULL;
				}
				Rfree(temp->bf);
				temp->num=0;
			} else {
				temp1->num=0;
				temp1->bf=NULL;
			}
		}
		Rfree(new_part);
	} else {
		temp=HoldButtons;
		if(temp!=NULL)
		{
			if(temp->progname!=NULL) Rfree(temp->progname);
			if(temp->modulename!=NULL) Rfree(temp->modulename);
			if(temp->filename!=NULL) Rfree(temp->filename);
			if(temp->keyname!=NULL) Rfree(temp->keyname);
			if(temp->listname!=NULL) Rfree(temp->listname);
			if(temp->buttonname!=NULL) Rfree(temp->buttonname);
			if(temp->expression!=NULL) Rfree(temp->expression);
			if(temp->args!=NULL) freeapplib(temp->args);
			if(temp->bf!=NULL)
			{
				for(z=0,b1=temp->bf;z<temp->num;++z,++b1)
				{
					if(b1->rname!=NULL) Rfree(b1->rname);
					if(b1->ename!=NULL) Rfree(b1->ename);
				}
				Rfree(temp->bf);
				temp->num=0;
			}
			Rfree(HoldButtons);
			numHoldButtons=0;
		}
	}
	makebuttonlist();
	if(value>=buttonsavl->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(mtnrsrc,"BUTTONS AVAILABLE",value,buttonsavl))
		updatersrc(mtnrsrc,"BUTTONS AVAILABLE");
} 
/*
static void printmtn1(RDA_PFILE *fp,MaintainMaster *d,int *lines,int *pages)
{
	int x,y,z;
	MaintainButton *button;
	ButtonField *btnfld;
	RDAsearch *tempsupport;
	RDAsubord *tempsubord;
	RDAconfld *tempconfld;
	PowerWrite *Write=NULL;
	PowerWriteField *WriteField=NULL;
	MTNAction *Action=NULL; 
	Actionstruct *ActionStruct=NULL;
	char *temp=NULL;

	temp=stralloc("XPERT MAINTAIN MASTER DEFINITION"); 
	printheader(fp,lines,pages,temp,d->module);
	if(temp!=NULL) Rfree(temp);

	RDA_fprintf(fp,"START MAINTAIN MASTER DEFINITION : Name: [%s]\r\n",
		(d->name!=NULL ? d->name:""));
	++*lines;
	checkheader(fp,lines,pages,d->module);

	RDA_fprintf(fp,"  Description: \r\n");
	++*lines;
	checkheader(fp,lines,pages,d->module);

	temp=Rmalloc(10+(d->desc!=NULL?RDAstrlen(d->desc):0));
	sprintf(temp,"    [%s]\r\n",(d->desc!=NULL ? d->desc:""));
	fixmtnoutput(fp,temp,lines,pages,d->module);
	if(temp!=NULL) Rfree(temp);

	RDA_fprintf(fp,"  Style: [%d][%s] Main File: [%s]\r\n",d->style,style_names[d->style],d->mainfile);
	++*lines;
	checkheader(fp,lines,pages,d->module);

	RDA_fprintf(fp,"  File Type: [%d][%s] Start With: [%d][%s] Start Where: [%d][%s]\r\n",
		d->file_type,filetype_names[d->file_type],
		d->start_with,startwith_names[d->start_with],
		d->start_where,startwhere_names[d->start_where]);
	++*lines;
	checkheader(fp,lines,pages,d->module);
	RDA_fprintf(fp,"  Inherit Resource Values: [%d] [%s]\r\n",d->inheritrsrcs,(d->inheritrsrcs==TRUE?"TRUE":"FALSE"));
	++*lines;
	checkheader(fp,lines,pages,d->module);
	RDA_fprintf(fp,"  Browse Screen: [%s]\r\n",(d->browse_screen!=NULL ? d->browse_screen:""));
	++*lines;
	checkheader(fp,lines,pages,d->module);
	RDA_fprintf(fp,"  Define List: [%s]\r\n",(d->define_list!=NULL ? d->define_list:""));
	++*lines;
	checkheader(fp,lines,pages,d->module);
	RDA_fprintf(fp,"  Search Browse: [%s]\r\n",(d->search_browse!=NULL ? d->search_browse:""));
	++*lines;
	checkheader(fp,lines,pages,d->module);
	RDA_fprintf(fp,"  Maintain Screen: [%s]\r\n",(d->maintain_screen!=NULL ? d->maintain_screen:""));
	++*lines;
	checkheader(fp,lines,pages,d->module);
	RDA_fprintf(fp,"  Use Audit Trail: [%s] Reportname: [%s]\r\n",
		(d->Use_Audit_Trail==TRUE?"TRUE":"FALSE"),
		(d->Audit_Trail!=NULL?d->Audit_Trail:""));
	++*lines;
	checkheader(fp,lines,pages,d->module);
	RDA_fprintf(fp,"  Save Type: [%d] Level: [%d]\r\n",
		d->save_type,d->level);
	++*lines;
	checkheader(fp,lines,pages,d->module);
	if(d->s!=NULL)
	{
		RDA_fprintf(fp,"  Supporting Files: [%d]\r\n",d->num);
		++*lines;
		checkheader(fp,lines,pages,d->module);
		for(x=0,tempsupport=d->s;x<d->num;++x,++tempsupport)
		{
			RDA_fprintf(fp,"    Module: [%s]  Filename: [%s]\r\n",
				(tempsupport->module!=NULL?tempsupport->module:""),
				(tempsupport->filename!=NULL?tempsupport->filename:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			RDA_fprintf(fp,"    Key Name: [%s]  Occurrence: [%d]\r\n",
				(tempsupport->subkeyname!=NULL?tempsupport->subkeyname:""),
				tempsupport->occurrence);
			++*lines;
			checkheader(fp,lines,pages,d->module);
			RDA_fprintf(fp,"    Connect Type: [%d][%s]\r\n",
				tempsupport->ctype,contypemtn[tempsupport->ctype]);
			++*lines;
			checkheader(fp,lines,pages,d->module);
			if(tempsupport->browse==TRUE)
			{
				RDA_fprintf(fp,"      Browse Button Name: [%s]\r\n",
					(tempsupport->button_name!=NULL?tempsupport->button_name:""));
				++*lines;
				checkheader(fp,lines,pages,d->module);
				RDA_fprintf(fp,"      Browse Screen: [%s]\r\n",
					(tempsupport->browse_screen!=NULL?tempsupport->browse_screen:""));
				++*lines;
				checkheader(fp,lines,pages,d->module);
				RDA_fprintf(fp,"      Define List Screen: [%s]\r\n",
					(tempsupport->define_list!=NULL?tempsupport->define_list:""));
				++*lines;
				checkheader(fp,lines,pages,d->module);
				RDA_fprintf(fp,"      Search Browse Screen: [%s]\r\n",
					(tempsupport->search_browse!=NULL?tempsupport->search_browse:""));
				++*lines;
				checkheader(fp,lines,pages,d->module);
			}
			RDA_fprintf(fp,"      Connect Module: [%s]  Filename: [%s]\r\n",
				(tempsupport->cmodule!=NULL?tempsupport->cmodule:""),
				(tempsupport->cfilename!=NULL?tempsupport->cfilename:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			RDA_fprintf(fp,"        Expression: [%s]\r\n",
				(tempsupport->expression!=NULL?tempsupport->expression:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			for(z=0,tempconfld=tempsupport->confld;z<tempsupport->num;++z,++tempconfld)
			{
				RDA_fprintf(fp,"          Field: [%s] connects to: [%s]  Inherit [%s]\r\n",
					(tempconfld->fldname!=NULL?tempconfld->fldname:""),
					(tempconfld->conname!=NULL?tempconfld->conname:""),
					(tempconfld->inherit==TRUE?"True":"False"));
				++*lines;
				checkheader(fp,lines,pages,d->module);
			}
		}
	}
	if(d->subord!=NULL)
	{
		RDA_fprintf(fp,"  Subordinate: [%d]\r\n",d->subnum);
		++*lines;
		checkheader(fp,lines,pages,d->module);
		for(x=0,tempsubord=d->subord;x<d->subnum;++x,++tempsubord)
		{
			RDA_fprintf(fp,"    Module: [%s]  Filename: [%s]  Key Name: [%s]\r\n",
				(tempsubord->module!=NULL?tempsubord->module:""),
				(tempsubord->filename!=NULL?tempsubord->filename:""),
				(tempsubord->subkeyname!=NULL?tempsubord->subkeyname:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			RDA_fprintf(fp,"      Connect Module: [%s]  Filename: [%s]\r\n",
				(tempsubord->cmodule!=NULL?tempsubord->cmodule:""),
				(tempsubord->cfilename!=NULL?tempsubord->cfilename:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			RDA_fprintf(fp,"      Expression: [%s]\r\n",
				(tempsubord->expression!=NULL?tempsubord->expression:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			for(y=0,tempconfld=tempsubord->confld;y<tempsubord->num;++y,++tempconfld)
			{
				RDA_fprintf(fp,"        Fieldname: [%s] connects to: [%s]\r\n",
					(tempconfld->fldname!=NULL?tempconfld->fldname:""),
					(tempconfld->conname!=NULL?tempconfld->conname:""));
				++*lines;
				checkheader(fp,lines,pages,d->module);
			}
			if(tempsubord->UsePowerWrite==TRUE)
			{
			RDA_fprintf(fp,"  Use Subordinate Power Writes: [%s]\r\n",(tempsubord->UsePowerWrite==TRUE?"TRUE":"FALSE"));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			if(tempsubord->Writes!=NULL && tempsubord->numWrites>0)
			{
				RDA_fprintf(fp,"      Subordinate Writes: [%d]\r\n",tempsubord->numWrites);
				++*lines;
				checkheader(fp,lines,pages,d->module);
				for(y=0,Write=tempsubord->Writes;y<tempsubord->numWrites;++y,++Write)
				{
					RDA_fprintf(fp,"        Write Module/File: [%s][%s]\r\n",
						(Write->module!=NULL?Write->module:""),
						(Write->filename!=NULL?Write->filename:""));
					++*lines;
					checkheader(fp,lines,pages,d->module);
					RDA_fprintf(fp,"          Write Expression: [%s]\r\n",
						(Write->expression!=NULL?Write->expression:""));
					++*lines;
					checkheader(fp,lines,pages,d->module);
					if(Write->fields!=NULL)
					{
						for(z=0,WriteField=Write->fields;z<Write->numflds;++z,++WriteField)
						{
							RDA_fprintf(fp,"            Write to Field: [%s] Copy Type [%d][%s]\r\n",
								(WriteField->to_name!=NULL?WriteField->to_name:""),
								WriteField->copy_type,
								(WriteField->copy_type==0?"Copy Variable":"Evaluate Expression"));
							++*lines;
							checkheader(fp,lines,pages,d->module);
							RDA_fprintf(fp,"              from Field: [%s]\r\n",
								(WriteField->from_name!=NULL?WriteField->from_name:""));
							++*lines;
							checkheader(fp,lines,pages,d->module);

						}
					}
				}
			}
			}
		}
	}
	if(d->Actions!=NULL)
	{
		RDA_fprintf(fp,"  Actions: [%d]\r\n",d->numActions);
		++*lines;
		checkheader(fp,lines,pages,d->module);
		for(x=0,Action=d->Actions;x<d->numActions;++x,++Action)
		{
			RDA_fprintf(fp,"    Name: [%s]  Number of Resources: [%d]\r\n",
				(Action->name!=NULL?Action->name:""),
				Action->num);
			++*lines;
			checkheader(fp,lines,pages,d->module);
			if(Action->a!=NULL)
			{
				RDA_fprintf(fp,"      Action Resources: [%d]\r\n",Action->num);
				++*lines;
				checkheader(fp,lines,pages,d->module);
				for(y=0,ActionStruct=Action->a;y<Action->num;++y,++ActionStruct)
				{
					RDA_fprintf(fp,"          Resource Name: [%s]\r\n",
						(ActionStruct->to_name!=NULL?ActionStruct->to_name:""));
					++*lines;
					checkheader(fp,lines,pages,d->module);
					RDA_fprintf(fp,"            Expression: [%s]\r\n",
						(ActionStruct->expression!=NULL?ActionStruct->expression:""));
					++*lines;
					checkheader(fp,lines,pages,d->module);

					RDA_fprintf(fp,"            Action Type: [%d] [%s] \r\n",
						ActionStruct->typelist,ActionTypeList[ActionStruct->typelist]);
					++*lines;
					checkheader(fp,lines,pages,d->module);
				}
			}
		}
	}
	if(d->save_warning!=NULL)
	{
		RDA_fprintf(fp,"    Save Warning: [%s]\r\n",
			(d->save_warning!=NULL?d->save_warning:""));
		++*lines;
		checkheader(fp,lines,pages,d->module);
		if(d->save_warning_desc!=NULL)
		{
			RDA_fprintf(fp,"    Save Warning Desc: [%s]\r\n",
				(d->save_warning_desc!=NULL?d->save_warning_desc:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
		}
	}
	if(d->save_error!=NULL)
	{
		RDA_fprintf(fp,"    Save Error: [%s]\r\n",
			(d->save_error!=NULL?d->save_error:""));
		++*lines;
		checkheader(fp,lines,pages,d->module);
		if(d->save_error!=NULL)
		{
			RDA_fprintf(fp,"    Save Error Desc: [%s]\r\n",
				(d->save_error_desc!=NULL?d->save_error_desc:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
		}
	}
	if(d->UsePowerWrite==FALSE)
	{
		RDA_fprintf(fp,"  Use Power Writes [FALSE]\r\n");
		++*lines;
		checkheader(fp,lines,pages,d->module);
	} else {
		RDA_fprintf(fp,"  Use Power Writes [TRUE]\r\n");
		++*lines;
		checkheader(fp,lines,pages,d->module);
		if(d->PrevWrites!=NULL && d->numPrevWrites>0)
		{
			RDA_fprintf(fp,"    Previous Power Writes: [%d]\r\n",d->numPrevWrites);
			++*lines;
			checkheader(fp,lines,pages,d->module);
			for(y=0,Write=d->PrevWrites;y<d->numPrevWrites;++y,++Write)
			{
				RDA_fprintf(fp,"      Write Module/File: [%s][%s]\r\n",
					(Write->module!=NULL?Write->module:""),
					(Write->filename!=NULL?Write->filename:""));
				++*lines;
				checkheader(fp,lines,pages,d->module);
				RDA_fprintf(fp,"        Write Expression: [%s]\r\n",
					(Write->expression!=NULL?Write->expression:""));
				++*lines;
				checkheader(fp,lines,pages,d->module);
				if(Write->fields!=NULL)
				{
					for(z=0,WriteField=Write->fields;z<Write->numflds;++z,++WriteField)
					{
						RDA_fprintf(fp,"          Write to Field: [%s] Copy Type [%d][%s]\r\n",
							(WriteField->to_name!=NULL?WriteField->to_name:""),
							WriteField->copy_type,
							(WriteField->copy_type==0?"Copy Variable":"Evaluate Expression"));
						++*lines;
						checkheader(fp,lines,pages,d->module);
						RDA_fprintf(fp,"                from Field: [%s]\r\n",
							(WriteField->from_name!=NULL?WriteField->from_name:""));
						++*lines;
						checkheader(fp,lines,pages,d->module);
					}
				}
			}
		}
		if(d->NewWrites!=NULL && d->numNewWrites>0)
		{
			RDA_fprintf(fp,"    New Power Writes: [%d]\r\n",d->numNewWrites);
			++*lines;
			checkheader(fp,lines,pages,d->module);
			for(y=0,Write=d->NewWrites;y<d->numNewWrites;++y,++Write)
			{
				RDA_fprintf(fp,"      Write Module/File: [%s][%s]\r\n",
					(Write->module!=NULL?Write->module:""),
					(Write->filename!=NULL?Write->filename:""));
				++*lines;
				checkheader(fp,lines,pages,d->module);
				RDA_fprintf(fp,"        Write Expression: [%s]\r\n",
					(Write->expression!=NULL?Write->expression:""));
				++*lines;
				checkheader(fp,lines,pages,d->module);
				if(Write->fields!=NULL)
				{
					for(z=0,WriteField=Write->fields;z<Write->numflds;++z,++WriteField)
					{
						RDA_fprintf(fp,"          Write to Field: [%s] Copy Type [%d][%s]\r\n",
							(WriteField->to_name!=NULL?WriteField->to_name:""),
							WriteField->copy_type,
							(WriteField->copy_type==0?"Copy Variable":"Evaluate Expression"));
						++*lines;
						checkheader(fp,lines,pages,d->module);
						RDA_fprintf(fp,"                from Field: [%s]\r\n",
							(WriteField->from_name!=NULL?WriteField->from_name:""));
						++*lines;
						checkheader(fp,lines,pages,d->module);
					}
				}
			}
		}
	}
	if(d->buttons!=NULL)
	{
		for(y=0,button=d->buttons;y<d->numbuttons;++y,++button)
		{
			RDA_fprintf(fp,"   Program [%s]\r\n",
				(button->progname!=NULL ? button->progname:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			RDA_fprintf(fp,"      List Name: [%s]\r\n",
				(button->listname!=NULL ? button->listname:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			RDA_fprintf(fp,"      Button Name: [%s]\r\n",
				(button->buttonname!=NULL ? button->buttonname:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			RDA_fprintf(fp,"      Button Type: [%s]\r\n",
				(btntypes->libs[button->type]!=NULL?btntypes->libs[button->type]:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			RDA_fprintf(fp,"      Module [%s] Filename: [%s] Keyname: [%s]\r\n",
				(button->modulename!=NULL ? button->modulename:""),
				(button->filename!=NULL ? button->filename:""),
				(button->keyname!=NULL ? button->keyname:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			if(button->bf!=NULL)
			{
				for(z=0,btnfld=button->bf;z<button->num;++z,++btnfld)
				{
					RDA_fprintf(fp,"        Field Name: [%s]\r\n",
						(btnfld->rname!=NULL ? btnfld->rname:""));
					++*lines;
					checkheader(fp,lines,pages,d->module);
					RDA_fprintf(fp,"          Environment Name: [%s]\r\n",
						(btnfld->ename!=NULL ? btnfld->ename:""));
					++*lines;
					checkheader(fp,lines,pages,d->module);
				}
			}
		}
	}
	RDA_fprintf(fp,"END MAINTAIN MASTER DEFINITION [%s] \r\n",
		(d->name!=NULL ? d->name:""));
	++*lines;
}
*/
static void enable_powerwrite(RDArsrc *r)
{
	char bv=FALSE;

	readwidget(r,"USE POWER WRITE");
	FINDRSCGETCHAR(r,"USE POWER WRITE",&bv);
	if(bv==FALSE)
	{
		FINDRSCSETEDITABLE(r,"PREV WRITE LIST",FALSE);
		FINDRSCSETEDITABLE(r,"ADD PREV WRITE ABOVE",FALSE);
		FINDRSCSETEDITABLE(r,"ADD PREV WRITE BELOW",FALSE);
		FINDRSCSETEDITABLE(r,"SELECT PREV WRITE",FALSE);
		FINDRSCSETEDITABLE(r,"COPY PREV WRITE",FALSE);
		FINDRSCSETEDITABLE(r,"DELETE PREV WRITE",FALSE);
		FINDRSCSETEDITABLE(r,"NEW WRITE LIST",FALSE);
		FINDRSCSETEDITABLE(r,"ADD NEW WRITE ABOVE",FALSE);
		FINDRSCSETEDITABLE(r,"ADD NEW WRITE BELOW",FALSE);
		FINDRSCSETEDITABLE(r,"SELECT NEW WRITE",FALSE);
		FINDRSCSETEDITABLE(r,"COPY NEW WRITE",FALSE);
		FINDRSCSETEDITABLE(r,"DELETE NEW WRITE",FALSE);
		FINDRSCSETSENSITIVE(r,"PREV WRITE LIST",FALSE);
		FINDRSCSETSENSITIVE(r,"ADD PREV WRITE ABOVE",FALSE);
		FINDRSCSETSENSITIVE(r,"ADD PREV WRITE BELOW",FALSE);
		FINDRSCSETSENSITIVE(r,"SELECT PREV WRITE",FALSE);
		FINDRSCSETSENSITIVE(r,"COPY PREV WRITE",FALSE);
		FINDRSCSETSENSITIVE(r,"DELETE PREV WRITE",FALSE);
		FINDRSCSETSENSITIVE(r,"NEW WRITE LIST",FALSE);
		FINDRSCSETSENSITIVE(r,"ADD NEW WRITE ABOVE",FALSE);
		FINDRSCSETSENSITIVE(r,"ADD NEW WRITE BELOW",FALSE);
		FINDRSCSETSENSITIVE(r,"SELECT NEW WRITE",FALSE);
		FINDRSCSETSENSITIVE(r,"COPY NEW WRITE",FALSE);
		FINDRSCSETSENSITIVE(r,"DELETE NEW WRITE",FALSE);
	} else {
		FINDRSCSETSENSITIVE(r,"PREV WRITE LIST",TRUE);
		FINDRSCSETSENSITIVE(r,"ADD PREV WRITE ABOVE",TRUE);
		FINDRSCSETSENSITIVE(r,"ADD PREV WRITE BELOW",TRUE);
		FINDRSCSETSENSITIVE(r,"SELECT PREV WRITE",TRUE);
		FINDRSCSETSENSITIVE(r,"COPY PREV WRITE",TRUE);
		FINDRSCSETSENSITIVE(r,"DELETE PREV WRITE",TRUE);
		FINDRSCSETSENSITIVE(r,"NEW WRITE LIST",TRUE);
		FINDRSCSETSENSITIVE(r,"ADD NEW WRITE ABOVE",TRUE);
		FINDRSCSETSENSITIVE(r,"ADD NEW WRITE BELOW",TRUE);
		FINDRSCSETSENSITIVE(r,"SELECT NEW WRITE",TRUE);
		FINDRSCSETSENSITIVE(r,"COPY NEW WRITE",TRUE);
		FINDRSCSETSENSITIVE(r,"DELETE NEW WRITE",TRUE);
		FINDRSCSETEDITABLE(r,"PREV WRITE LIST",TRUE);
		FINDRSCSETEDITABLE(r,"ADD PREV WRITE ABOVE",TRUE);
		FINDRSCSETEDITABLE(r,"ADD PREV WRITE BELOW",TRUE);
		FINDRSCSETEDITABLE(r,"SELECT PREV WRITE",TRUE);
		FINDRSCSETEDITABLE(r,"COPY PREV WRITE",TRUE);
		FINDRSCSETEDITABLE(r,"DELETE PREV WRITE",TRUE);
		FINDRSCSETEDITABLE(r,"NEW WRITE LIST",TRUE);
		FINDRSCSETEDITABLE(r,"ADD NEW WRITE ABOVE",TRUE);
		FINDRSCSETEDITABLE(r,"ADD NEW WRITE BELOW",TRUE);
		FINDRSCSETEDITABLE(r,"SELECT NEW WRITE",TRUE);
		FINDRSCSETEDITABLE(r,"COPY NEW WRITE",TRUE);
		FINDRSCSETEDITABLE(r,"DELETE NEW WRITE",TRUE);
	}
}
static void set_audit_trail(RDArsrc *r)
{
	char a_trail=FALSE;

	readwidget(r,"USE AUDIT TRAIL");
	FINDRSCGETCHAR(r,"USE AUDIT TRAIL",&a_trail);
	if(a_trail==TRUE)
	{
		FINDRSCSETSENSITIVE(r,"AUDIT TRAIL",TRUE);
		FINDRSCSETEDITABLE(r,"AUDIT TRAIL",TRUE);
	} else {
		FINDRSCSETEDITABLE(r,"AUDIT TRAIL",FALSE);
		FINDRSCSETSENSITIVE(r,"AUDIT TRAIL",FALSE);
	}
}
/*
static void quitdfmtn(RDArsrc *rsrc)
{
	if(rsrc!=NULL) free_rsrc(rsrc);
	if(mtntdefaults!=NULL) FreeRDAdefault(mtntdefaults);
	if(btntypes!=NULL) freeapplib(btntypes);
}
*/
/*
static MaintainMaster *ReportMaintainMasterNew(char *module,char *name,RDAreport *report)
{
	char *temp=NULL;
	MaintainMaster *MTNMSTR=NULL;

	temp=Rmalloc(RDAstrlen(name)+5);
	sprintf(temp,"MTN %s",(name!=NULL?name:""));
	MTNMSTR=MaintainMasterNew(module,temp);
	if(temp!=NULL) Rfree(temp);

	MTNMSTR->desc=stralloc(report->desc);
	MTNMSTR->style=2;
	MTNMSTR->mainfile=stralloc("SORTFILE");
	MTNMSTR->browse_screen=Rmalloc(RDAstrlen(name)+8);
	sprintf(MTNMSTR->browse_screen,"%s BROWSE",(name!=NULL?name:""));
	MTNMSTR->define_list=Rmalloc(RDAstrlen(name)+13);
	sprintf(MTNMSTR->define_list,"%s DEFINE LIST",(name!=NULL?name:""));
	MTNMSTR->search_browse=Rmalloc(RDAstrlen(name)+15);
	sprintf(MTNMSTR->search_browse,"%s SEARCH BROWSE",(name!=NULL?name:""));
	MTNMSTR->maintain_screen=Rmalloc(RDAstrlen(name)+17);
	sprintf(MTNMSTR->maintain_screen,"%s MAINTAIN SCREEN",(name!=NULL?name:""));
	MTNMSTR->file_type=1;
	MTNMSTR->start_with=1;
	MTNMSTR->start_where=0;
	MTNMSTR->passparent=NULL;
	return(MTNMSTR);



	MTNMSTR=Rmalloc(sizeof(MaintainMaster));
	MTNMSTR->module=stralloc(module);
	MTNMSTR->name=Rmalloc(RDAstrlen(name)+5);
	sprintf(MTNMSTR->name,"MTN %s",(name!=NULL?name:""));
	MTNMSTR->desc=stralloc(report->desc);
	MTNMSTR->style=2;
	MTNMSTR->mainfile=stralloc("SORTFILE");
	MTNMSTR->browse_screen=Rmalloc(RDAstrlen(name)+8);
	sprintf(MTNMSTR->browse_screen,"%s BROWSE",(name!=NULL?name:""));
	MTNMSTR->define_list=Rmalloc(RDAstrlen(name)+13);
	sprintf(MTNMSTR->define_list,"%s DEFINE LIST",(name!=NULL?name:""));
	MTNMSTR->search_browse=Rmalloc(RDAstrlen(name)+15);
	sprintf(MTNMSTR->search_browse,"%s SEARCH BROWSE",(name!=NULL?name:""));
	MTNMSTR->maintain_screen=Rmalloc(RDAstrlen(name)+17);
	sprintf(MTNMSTR->maintain_screen,"%s MAINTAIN SCREEN",(name!=NULL?name:""));
	MTNMSTR->file_type=1;
	MTNMSTR->start_with=1;
	MTNMSTR->start_where=0;
	MTNMSTR->num=0;
	MTNMSTR->s=NULL;
	MTNMSTR->subnum=0;
	MTNMSTR->subord=NULL;
	MTNMSTR->passkey=NULL;
	MTNMSTR->args=NULL;
	MTNMSTR->numlists=NULL;
	MTNMSTR->lists=NULL;
	MTNMSTR->save_type=0;
	MTNMSTR->save_expression=NULL;
	MTNMSTR->numbuttons=0;
	MTNMSTR->buttons=NULL;
	MTNMSTR->runfunc=NULL;
	MTNMSTR->save_error=NULL;
	MTNMSTR->save_warning=NULL;
	MTNMSTR->save_error_desc=NULL;
	MTNMSTR->save_warning_desc=NULL;
	MTNMSTR->inheritrsrcs=FALSE;
	MTNMSTR->UsePowerWrite=FALSE;
	MTNMSTR->numPrevWrites=0;
	MTNMSTR->PrevWrites=NULL;
	MTNMSTR->numNewWrites=0;
	MTNMSTR->NewWrites=NULL;
	MTNMSTR->Use_Audit_Trail=FALSE;
	MTNMSTR->Audit_Trail=NULL;
	MTNMSTR->mbl_utlmstr=NULL;
	MTNMSTR->editable=TRUE;
	MTNMSTR->level=0;
	MTNMSTR->WindowCount=0;
	MTNMSTR->SubFunc=NULL;
	MTNMSTR->SubFuncArgs=NULL;
	MTNMSTR->numActions=0;
	MTNMSTR->Actions=NULL;
	MTNMSTR->mtnrsrc=NULL;
	MTNMSTR->passparent=NULL;
	return(MTNMSTR);
}
*/
static void maintainscreen(RDArsrc *mainrsrc,char *modulx,char *name,
	char *desc,int style,char *mainfile,char *browse_screen,
	char *define_list,char *search_browse,char *maintain_screen,
	int file_type,int start_with,int startwhere,
	int numbuttons,MaintainButton *buttons,
	int numsupport,RDAsearch *s,int numsubord,RDAsubord *sub,
	char *save_e,char *save_ed,char *save_w,char *save_wd,
	short inheritrsrcs,char Use_Audit_Trail,char *Audit_Trail,
	char UsePowerWrite,int numPrevWrites,PowerWrite *PrevWrites,
	int numNewWrites,PowerWrite *NewWrites,
	int numActions,MTNAction *Actions,RDAreport *report)
{
	int x=0,z=0,y=0;
	RDArsrc *mtnrsrc=NULL;
	MaintainButton *b1,*b2;
	ButtonField *bf1,*bf2;
	RDAsearch *tempsupp1,*tempsupp;
	RDAsubord *tempsub1,*tempsub;
	RDAconfld *confld1,*confld2;
	char tempinheritrsrcs=FALSE;
	PowerWrite *W1=NULL,*W2=NULL;
	PowerWriteField *WF1=NULL,*WF2=NULL;
	MTNAction *action1=NULL,*action2=NULL;
	Actionstruct *actstruct1=NULL,*actstruct2=NULL;

	btntypes=makebtntypeslist();
	tmpnumPrevWrites=0;
	tempPrevWrites=NULL;
	if(PrevWrites!=NULL)
	{
		tmpnumPrevWrites=numPrevWrites;
		tempPrevWrites=Rmalloc(tmpnumPrevWrites*sizeof(PowerWrite));
		for(x=0,W1=PrevWrites,W2=tempPrevWrites;x<numPrevWrites;
			++x,++W1,++W2)
		{
			if(W1->module!=NULL) W2->module=stralloc(W1->module);
				else W2->module=NULL;
			if(W1->filename!=NULL) W2->filename=stralloc(W1->filename);
				else W2->filename=NULL;
			if(W1->expression!=NULL) W2->expression=stralloc(W1->expression);
				else W2->expression=NULL;
			W2->fields=NULL;
			W2->numflds=0;
			if(W1->fields!=NULL)
			{
				W2->numflds=W1->numflds;
				W2->fields=Rmalloc(W2->numflds*sizeof(PowerWriteField));
				for(z=0,WF1=W1->fields,WF2=W2->fields;z<W2->numflds;++z,++WF1,++WF2)
				{
					if(WF1->to_name!=NULL) WF2->to_name=stralloc(WF1->to_name);
						else WF1->to_name=NULL;
					WF2->copy_type=WF1->copy_type;
					if(WF1->from_name!=NULL) WF2->from_name=stralloc(WF1->from_name);
						else WF1->from_name=NULL;
				}
			} else {
				W2->fields=NULL;
				W2->numflds=0;
			}
		}
	}
	tmpnumNewWrites=0;
	tempNewWrites=NULL;
	if(NewWrites!=NULL)
	{
		tmpnumNewWrites=numNewWrites;
		tempNewWrites=Rmalloc(tmpnumNewWrites*sizeof(PowerWrite));
		for(x=0,W1=NewWrites,W2=tempNewWrites;x<numNewWrites;
			++x,++W1,++W2)
		{
			if(W1->module!=NULL) W2->module=stralloc(W1->module);
				else W2->module=NULL;
			if(W1->filename!=NULL) W2->filename=stralloc(W1->filename);
				else W2->filename=NULL;
			if(W1->expression!=NULL) W2->expression=stralloc(W1->expression);
				else W2->expression=NULL;
			W2->fields=NULL;
			W2->numflds=0;
			if(W1->fields!=NULL)
			{
				W2->numflds=W1->numflds;
				W2->fields=Rmalloc(W2->numflds*sizeof(PowerWriteField));
				for(z=0,WF1=W1->fields,WF2=W2->fields;z<W2->numflds;++z,++WF1,++WF2)
				{
					if(WF1->to_name!=NULL) WF2->to_name=stralloc(WF1->to_name);
						else WF1->to_name=NULL;
					WF2->copy_type=WF1->copy_type;
					if(WF1->from_name!=NULL) WF2->from_name=stralloc(WF1->from_name);
						else WF1->from_name=NULL;
				}
			} else {
				W2->fields=NULL;
				W2->numflds=0;
			}
		}
	}
	if(s!=NULL)
	{
		tmpnumsupp=numsupport;
		tempsupport=Rmalloc(tmpnumsupp*sizeof(RDAsearch));
		for(x=0,tempsupp1=tempsupport,tempsupp=s;x<tmpnumsupp;++x,++tempsupp1,++tempsupp)
		{
			if(tempsupp->module!=NULL)
			{
				tempsupp1->module=stralloc(tempsupp->module);
			} else tempsupp1->module=NULL;
			if(tempsupp->filename!=NULL)
			{
				tempsupp1->filename=stralloc(tempsupp->filename);
			} else tempsupp1->filename=NULL;
			tempsupp1->ctype=tempsupp->ctype;
			if(tempsupp->cmodule!=NULL)
			{
				tempsupp1->cmodule=stralloc(tempsupp->cmodule);
			} else tempsupp1->cmodule=NULL;
			if(tempsupp->cfilename!=NULL)
			{
				tempsupp1->cfilename=stralloc(tempsupp->cfilename);
			} else tempsupp1->cfilename=NULL;
			tempsupp1->num=tempsupp->num;
			if(tempsupp->confld!=NULL)
			{
				tempsupp1->confld=Rmalloc(tempsupp->num*sizeof(RDAconfld));
				for(z=0,confld1=tempsupp->confld,confld2=tempsupp1->confld;
					z<tempsupp->num;++z,++confld1,++confld2)
				{
					if(confld1->fldname!=NULL)
					{
						confld2->fldname=stralloc(confld1->fldname);
					} else confld2->fldname=NULL;
					if(confld1->conname!=NULL)
					{
						confld2->conname=stralloc(confld1->conname);
					} else confld2->conname=NULL;
					confld2->inherit=confld1->inherit;
				}
			}
			if(tempsupp->subkeyname!=NULL)
			{
				tempsupp1->subkeyname=stralloc(tempsupp->subkeyname);
			} else tempsupp1->subkeyname=NULL;
			if(tempsupp->expression!=NULL)
			{
				tempsupp1->expression=stralloc(tempsupp->expression);
			} else tempsupp1->expression=NULL;
			tempsupp1->browse=tempsupp->browse;
			if(tempsupp->button_name!=NULL)
			{
				tempsupp1->button_name=stralloc(tempsupp->button_name);
			} else tempsupp1->button_name=NULL;
			if(tempsupp->browse_screen!=NULL)
			{
				tempsupp1->browse_screen=stralloc(tempsupp->browse_screen);
			} else tempsupp1->browse_screen=NULL;
			if(tempsupp->define_list!=NULL)
			{
				tempsupp1->define_list=stralloc(tempsupp->define_list);
			} else tempsupp1->define_list=NULL;
			if(tempsupp->search_browse!=NULL)
			{
				tempsupp1->search_browse=stralloc(tempsupp->search_browse);
			} else tempsupp1->search_browse=NULL;
			tempsupp1->smart_browse=tempsupp->smart_browse;
			tempsupp1->range=tempsupp->range;
			tempsupp1->num_writes=tempsupp->num_writes;
		}
	} else {
		tempsupport=NULL;
		tmpnumsupp=0;
	}
	if(sub!=NULL)
	{
		tmpnumsub=numsubord;
		tempsubord=Rmalloc(tmpnumsub*sizeof(RDAsubord));
		for(x=0,tempsub1=tempsubord,tempsub=sub;x<tmpnumsub;++x,++tempsub1,++tempsub)
		{
			if(tempsub->module!=NULL)
			{
				tempsub1->module=stralloc(tempsub->module);
			} else tempsub1->module=NULL;
			if(tempsub->filename!=NULL)
			{
				tempsub1->filename=stralloc(tempsub->filename);
			} else tempsub1->filename=NULL;
			tempsub1->dtype=tempsub->dtype;
			if(tempsub->cmodule!=NULL)
			{
				tempsub1->cmodule=stralloc(tempsub->cmodule);
			} else tempsub1->cmodule=NULL;
			if(tempsub->cfilename!=NULL)
			{
				tempsub1->cfilename=stralloc(tempsub->cfilename);
			} else tempsub1->cfilename=NULL;
			tempsub1->num=tempsub->num;
			if(tempsub->confld!=NULL)
			{
				tempsub1->confld=Rmalloc(tempsub->num*sizeof(RDAconfld));
				for(z=0,confld1=tempsub->confld,confld2=tempsub1->confld;
					z<tempsub->num;++z,++confld1,++confld2)
				{
					if(confld1->fldname!=NULL)
					{
						confld2->fldname=stralloc(confld1->fldname);
					} else confld2->fldname=NULL;
					if(confld1->conname!=NULL)
					{
						confld2->conname=stralloc(confld1->conname);
					} else confld2->conname=NULL;
				}
			}
			if(tempsub->subkeyname!=NULL)
			{
				tempsub1->subkeyname=stralloc(tempsub->subkeyname);
			} else tempsub1->subkeyname=NULL;
			if(tempsub->expression!=NULL)
			{
				tempsub1->expression=stralloc(tempsub->expression);
			} else tempsub1->expression=NULL;
			tempsub1->UsePowerWrite=tempsub->UsePowerWrite;
			tempsub1->numWrites=0;
			tempsub1->Writes=NULL;
			if(tempsub->Writes!=NULL)
			{
				tempsub1->numWrites=tempsub->numWrites;
				tempsub1->Writes=Rmalloc(tempsub1->numWrites*sizeof(PowerWrite));
				for(y=0,W1=tempsub->Writes,W2=tempsub1->Writes;y<tempsub1->numWrites;++y,++W1,++W2)
				{
					if(W1->module!=NULL) W2->module=stralloc(W1->module);
						else W2->module=NULL;
					if(W1->filename!=NULL) W2->filename=stralloc(W1->filename);
						else W2->filename=NULL;
					if(W1->expression!=NULL) W2->expression=stralloc(W1->expression);
						else W2->expression=NULL;
					W2->fields=NULL;
					W2->numflds=0;
					if(W1->fields!=NULL)
					{
						W2->numflds=W1->numflds;
						W2->fields=Rmalloc(W2->numflds*sizeof(PowerWriteField));
						for(z=0,WF1=W1->fields,WF2=W2->fields;z<W2->numflds;++z,++WF1,++WF2)
						{
							if(WF1->to_name!=NULL) WF2->to_name=stralloc(WF1->to_name);
								else WF1->to_name=NULL;
							WF2->copy_type=WF1->copy_type;
							if(WF1->from_name!=NULL) WF2->from_name=stralloc(WF1->from_name);
								else WF1->from_name=NULL;
						}
					} else {
						W2->fields=NULL;
						W2->numflds=0;
					}
				}
			} else {
				tempsub1->numWrites=0;
				tempsub1->Writes=NULL;
			}
		}
	} else {
		tempsubord=NULL;
		tmpnumsub=0;
	}
	if(buttons!=NULL)
	{
		numHoldButtons=numbuttons;
		HoldButtons=Rmalloc(numHoldButtons*sizeof(MaintainButton));
		for(x=0,b1=buttons,b2=HoldButtons;x<numbuttons;++x,++b1,++b2)
		{
			if(b1->progname!=NULL) b2->progname=stralloc(b1->progname);
				else b2->progname=NULL;
			if(b1->modulename!=NULL) b2->modulename=stralloc(b1->modulename);
				else b2->modulename=NULL;
			if(b1->filename!=NULL) b2->filename=stralloc(b1->filename);
				else b2->filename=NULL;
			if(b1->keyname!=NULL) b2->keyname=stralloc(b1->keyname);
				else b2->keyname=NULL;
			if(b1->listname!=NULL) b2->listname=stralloc(b1->listname);
				else b2->listname=NULL;
			if(b1->buttonname!=NULL) b2->buttonname=stralloc(b1->buttonname);
				else b2->buttonname=NULL;
			if(b1->expression!=NULL) b2->expression=stralloc(b1->expression);
				else b2->expression=NULL;
			b2->type=b1->type;
			b2->evalargs=b1->evalargs;
			if(b1->args!=NULL)
			{
				b2->args=APPlibNEW();
				for(z=0;z<b1->args->numlibs;++z) addAPPlib(b2->args,b1->args->libs[z]);
			} else b2->args=NULL;
			if(b1->bf!=NULL)
			{
				b2->num=b1->num;
				b2->bf=Rmalloc(b2->num*sizeof(ButtonField));
				for(z=0,bf1=b1->bf,bf2=b2->bf;z<b2->num;++z,++bf1,++bf2)
				{
					if(bf1->rname!=NULL) bf2->rname=stralloc(bf1->rname);
						else bf2->rname=NULL;
					if(bf1->ename!=NULL) bf2->ename=stralloc(bf1->ename);
						else bf2->ename=NULL;
				}
			} else {
				b2->bf=NULL;
				b2->num=0;
			}
		}
	} else {
		numHoldButtons=0;
		HoldButtons=NULL;
	}
	if(Actions!=NULL)
	{
		tmpnumActions=numActions;
		tempActions=Rmalloc(numActions*sizeof(MTNAction));
		for(x=0,action1=Actions,action2=tempActions;x<numActions;++x,++action1,++action2)
		{
			if(!isEMPTY(action1->name)) action2->name=stralloc(action1->name);
				else action2->name=NULL;
			if(action1->a!=NULL)
			{
				action2->num=action1->num;
				action2->a=Rmalloc(action1->num*sizeof(Actionstruct));
				for(y=0,actstruct1=action1->a,actstruct2=action2->a;y<action1->num;++y,++actstruct1,++actstruct2)
				{
					if(!isEMPTY(actstruct1->to_name)) 
					{
						actstruct2->to_name=stralloc(actstruct1->to_name);
					} else actstruct2->to_name=NULL;
					actstruct2->typelist=actstruct1->typelist;
					if(!isEMPTY(actstruct1->expression))
					{
						actstruct2->expression=stralloc(actstruct1->expression);
					} else actstruct2->expression=NULL;
				}		
			} else {
				action2->num=0;
				action2->a=NULL;
			}
		}
	}
	mtnrsrc=RDArsrcNEW(module,"EDIT REPORT MAINTAIN MASTER");
        if(styles!=NULL) freeapplib(styles);
        styles=APPlibNEW();
        for(x=0;x<3;++x) addAPPlib(styles,style_names[x]);
	if(style>=styles->numlibs) style=0;
	addlstrsrc(mtnrsrc,"STYLES",&style,FALSE,changestyles,styles->numlibs,&styles->libs,NULL);
	tempinheritrsrcs=(char)inheritrsrcs;
	addstdrsrc(mtnrsrc,"INHERIT RESOURCE VALUES",BOOLNS,1,&tempinheritrsrcs,FALSE);
	addstdrsrc(mtnrsrc,"MODULE",VARIABLETEXT,0,(modulx!=NULL ? modulx:""),FALSE);
	addstdrsrc(mtnrsrc,"NAME",VARIABLETEXT,0,(name!=NULL ? name:""),FALSE);
	addsctrsrc(mtnrsrc,"DESCRIPTION",0,(desc!=NULL ? desc:""),FALSE);
	addstdrsrc(mtnrsrc,"USE AUDIT TRAIL",BOOLNS,1,&Use_Audit_Trail,TRUE);
	FINDRSCSETFUNC(mtnrsrc,"USE AUDIT TRAIL",set_audit_trail,NULL);
	addstdrsrc(mtnrsrc,"AUDIT TRAIL",VARIABLETEXT,0,(Audit_Trail!=NULL ? Audit_Trail:""),TRUE);
	if(ftypes!=NULL) freeapplib(ftypes);
	ftypes=APPlibNEW();
        for(x=0;x<2;++x) addAPPlib(ftypes,filetype_names[x]);
	if(file_type>=ftypes->numlibs) file_type=0;
	addlstrsrc(mtnrsrc,"FILE TYPES",&file_type,FALSE,NULL,ftypes->numlibs,&ftypes->libs,NULL);
	if(starttypes!=NULL) freeapplib(starttypes);
	starttypes=APPlibNEW();
        for(x=0;x<3;++x) addAPPlib(starttypes,startwith_names[x]);
	addstdrsrc(mtnrsrc,"MAINFILE",VARIABLETEXT,0,(mainfile!=NULL ? mainfile:""),FALSE);
	if(start_with>=starttypes->numlibs) start_with=0;
	addlstrsrc(mtnrsrc,"START TYPES",&start_with,TRUE,NULL,starttypes->numlibs,&starttypes->libs,NULL);
	if(startwheres!=NULL) freeapplib(startwheres);
	startwheres=APPlibNEW();
        for(x=0;x<2;++x) addAPPlib(startwheres,startwhere_names[x]);
	if(startwhere>=startwheres->numlibs) startwhere=0;
	addlstrsrc(mtnrsrc,"START WHERES",&startwhere,TRUE,NULL,startwheres->numlibs,&startwheres->libs,NULL);
	addstdrsrc(mtnrsrc,"BROWSE SCREEN",VARIABLETEXT,0,browse_screen,FALSE);
	addstdrsrc(mtnrsrc,"DEFINE LIST",VARIABLETEXT,0,define_list,FALSE);
	addstdrsrc(mtnrsrc,"SEARCH BROWSE",VARIABLETEXT,0,search_browse,FALSE);
	addstdrsrc(mtnrsrc,"MAINTAIN SCREEN",VARIABLETEXT,0,maintain_screen,FALSE);
	makebuttonlist();
	addlstrsrc(mtnrsrc,"BUTTONS AVAILABLE",0,TRUE,NULL,buttonsavl->numlibs,&buttonsavl->libs,NULL);
	addbtnrsrc(mtnrsrc,"ADD BUTTON ABOVE",TRUE,add_button_above,mainrsrc);
	addbtnrsrc(mtnrsrc,"ADD BUTTON BELOW",TRUE,add_button_below,mainrsrc);
	addbtnrsrc(mtnrsrc,"SELECT BUTTON",TRUE,select_button,mainrsrc);
	addbtnrsrc(mtnrsrc,"DELETE BUTTON",TRUE,delete_button,NULL);
	getsupports();
	addlstrsrc(mtnrsrc,"SUPPORTING FILES",0,TRUE,NULL,suppavlmtn->numlibs,&suppavlmtn->libs,NULL);
	addbtnrsrc(mtnrsrc,"ADD SUPPORTING ABOVE",TRUE,suppaddabove,mtnrsrc);
	addbtnrsrc(mtnrsrc,"ADD SUPPORTING BELOW",TRUE,suppaddbelow,mtnrsrc);
	addbtnrsrc(mtnrsrc,"DELETE SUPPORTING",TRUE,suppdelete,NULL);
	addbtnrsrc(mtnrsrc,"SELECT SUPPORTING",TRUE,suppedit,mtnrsrc);
	addsctrsrc(mtnrsrc,"SAVE ERROR",0,save_e,TRUE);
	addbtnrsrc(mtnrsrc,"LOAD VALUE SAVE ERROR",TRUE,loadvaluemtn,"SAVE ERROR");
	addbtnrsrc(mtnrsrc,"LOAD OPERAND SAVE ERROR",TRUE,loadoperand,"SAVE ERROR");
	addbtnrsrc(mtnrsrc,"LOAD GROUPER SAVE ERROR",TRUE,loadgrouper,"SAVE ERROR");
	addbtnrsrc(mtnrsrc,"LOAD CONTROL SAVE ERROR",TRUE,loadcontrol,"SAVE ERROR");
	addsctrsrc(mtnrsrc,"SAVE ERROR DESCRIPTION",0,save_ed,TRUE);
	addbtnrsrc(mtnrsrc,"LOAD VALUE SAVE ERROR DESCRIPTION",TRUE,loadvaluemtn,"SAVE ERROR DESCRIPTION");
	addbtnrsrc(mtnrsrc,"LOAD OPERAND SAVE ERROR DESCRIPTION",TRUE,loadoperand,"SAVE ERROR DESCRIPTION");
	addbtnrsrc(mtnrsrc,"LOAD GROUPER SAVE ERROR DESCRIPTION",TRUE,loadgrouper,"SAVE ERROR DESCRIPTION");
	addbtnrsrc(mtnrsrc,"LOAD CONTROL SAVE ERROR DESCRIPTION",TRUE,loadcontrol,"SAVE ERROR DESCRIPTION");
	addsctrsrc(mtnrsrc,"SAVE WARNING",0,save_w,TRUE);
	addbtnrsrc(mtnrsrc,"LOAD VALUE SAVE WARNING",TRUE,loadvaluemtn,"SAVE WARNING");
	addbtnrsrc(mtnrsrc,"LOAD OPERAND SAVE WARNING",TRUE,loadoperand,"SAVE WARNING");
	addbtnrsrc(mtnrsrc,"LOAD GROUPER SAVE WARNING",TRUE,loadgrouper,"SAVE WARNING");
	addbtnrsrc(mtnrsrc,"LOAD CONTROL SAVE WARNING",TRUE,loadcontrol,"SAVE WARNING");
	addsctrsrc(mtnrsrc,"SAVE WARNING DESCRIPTION",0,save_wd,TRUE);
	addbtnrsrc(mtnrsrc,"LOAD VALUE SAVE WARNING DESCRIPTION",TRUE,loadvaluemtn,"SAVE WARNING DESCRIPTION");
	addbtnrsrc(mtnrsrc,"LOAD OPERAND SAVE WARNING DESCRIPTION",TRUE,loadoperand,"SAVE WARNING DESCRIPTION");
	addbtnrsrc(mtnrsrc,"LOAD GROUPER SAVE WARNING DESCRIPTION",TRUE,loadgrouper,"SAVE WARNING DESCRIPTION");
	addbtnrsrc(mtnrsrc,"LOAD CONTROL SAVE WARNING DESCRIPTION",TRUE,loadcontrol,"SAVE WARNING DESCRIPTION");
	getsubords();
	addlstrsrc(mtnrsrc,"SUBORDINATE FILES",0,TRUE,NULL,subavlmtn->numlibs,&subavlmtn->libs,NULL);
	addbtnrsrc(mtnrsrc,"ADD SUBORDINATE ABOVE",TRUE,subaddabove,mtnrsrc);
	addbtnrsrc(mtnrsrc,"ADD SUBORDINATE BELOW",TRUE,subaddbelow,mtnrsrc);
	addbtnrsrc(mtnrsrc,"DELETE SUBORDINATE",TRUE,subdelete,NULL);
	addbtnrsrc(mtnrsrc,"SELECT SUBORDINATE",TRUE,subedit,mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE",TRUE,savemaintain,mainrsrc);
	addbtnrsrc(mtnrsrc,"QUIT",TRUE,quitmaintaintest,mainrsrc);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addstdrsrc(mtnrsrc,"USE POWER WRITE",BOOLNS,1,&UsePowerWrite,TRUE);
	FINDRSCSETFUNC(mtnrsrc,"USE POWER WRITE",enable_powerwrite,NULL);
	if(prevwrtavl!=NULL) freeapplib(prevwrtavl);
	prevwrtavl=APPlibNEW();
	getPrevWrites();
	addlstrsrc(mtnrsrc,"PREV WRITE LIST",0,TRUE,NULL,prevwrtavl->numlibs,&prevwrtavl->libs,NULL);
	addbtnrsrc(mtnrsrc,"ADD PREV WRITE ABOVE",TRUE,prevwrtaddabove,NULL);
	addbtnrsrc(mtnrsrc,"ADD PREV WRITE BELOW",TRUE,prevwrtaddbelow,NULL);
	addbtnrsrc(mtnrsrc,"DELETE PREV WRITE",TRUE,prevwrtdelete,NULL);
	addbtnrsrc(mtnrsrc,"COPY PREV WRITE",TRUE,prevwrtcopy,NULL);
	addbtnrsrc(mtnrsrc,"SELECT PREV WRITE",TRUE,prevwrtedit,NULL);
	if(newwrtavl!=NULL) freeapplib(newwrtavl);
	newwrtavl=APPlibNEW();
	getNewWrites();
	addlstrsrc(mtnrsrc,"NEW WRITE LIST",0,TRUE,NULL,newwrtavl->numlibs,&newwrtavl->libs,NULL);
	addbtnrsrc(mtnrsrc,"ADD NEW WRITE ABOVE",TRUE,newwrtaddabove,NULL);
	addbtnrsrc(mtnrsrc,"ADD NEW WRITE BELOW",TRUE,newwrtaddbelow,NULL);
	addbtnrsrc(mtnrsrc,"DELETE NEW WRITE",TRUE,newwrtdelete,NULL);
	addbtnrsrc(mtnrsrc,"COPY NEW WRITE",TRUE,newwrtcopy,NULL);
	addbtnrsrc(mtnrsrc,"SELECT NEW WRITE",TRUE,newwrtedit,NULL);
	actstructavl=APPlibNEW();
	getActions();
	addlstrsrc(mtnrsrc,"ACTION LIST",0,TRUE,NULL,actstructavl->numlibs,&actstructavl->libs,NULL);
	addbtnrsrc(mtnrsrc,"ADD ACTION ABOVE",TRUE,actionaddabove,NULL);
	addbtnrsrc(mtnrsrc,"ADD ACTION BELOW",TRUE,actionaddbelow,NULL);
	addbtnrsrc(mtnrsrc,"DELETE ACTION",TRUE,actiondelete,NULL);
	addbtnrsrc(mtnrsrc,"COPY ACTION",TRUE,actioncopy,NULL);
	addbtnrsrc(mtnrsrc,"SELECT ACTION",TRUE,actionedit,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	mtntdefaults=GetDefaults(mtnrsrc);
	file_defs=APPlibNEW();
	filersrc=RDArsrcNEW("UTILITIES","UTILITY MASTER SELECT FILE");
	x=0;
	addlstrsrc(filersrc,"FILE LIST",&x,TRUE,NULL,file_defs->numlibs,&file_defs->libs,NULL);
	addrfkwrsrc(filersrc,"QUIT",TRUE,NULL_RSCS,NULL);
	addbtnrsrc(filersrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(filersrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	changestyles(mtnrsrc);
	enable_powerwrite(mtnrsrc);
	set_audit_trail(mtnrsrc);
	APPmakescrn(mtnrsrc,TRUE,quitmaintain,mainrsrc,FALSE);
	MTNMSTR=report->MTNMSTR;
}
static void procexcpscrn(RDArsrc *mainrsrc,RDAreport *report)
{
	char *rptmodule=NULL;
	char *rptname=NULL;

	readwidget(mainrsrc,"REPORT NAME");
	readwidget(mainrsrc,"REPORT MODULE");
	FINDRSCGETSTRING(mainrsrc,"REPORT MODULE",&rptmodule);
	FINDRSCGETSTRING(mainrsrc,"REPORT NAME",&rptname);
	if(report->MTNMSTR==NULL)
	{
		report->MTNMSTR=ReportMaintainMasterNew(rptmodule,rptname,report);
	}
	maintainscreen(mainrsrc,report->MTNMSTR->module,
		report->MTNMSTR->name,report->MTNMSTR->desc,
		report->MTNMSTR->style,report->MTNMSTR->mainfile,
		report->MTNMSTR->browse_screen,report->MTNMSTR->define_list,
		report->MTNMSTR->search_browse,report->MTNMSTR->maintain_screen,
		report->MTNMSTR->file_type,report->MTNMSTR->start_with,
		report->MTNMSTR->start_where,report->MTNMSTR->numbuttons,
		report->MTNMSTR->buttons,report->MTNMSTR->num,
		report->MTNMSTR->s,report->MTNMSTR->subnum,
		report->MTNMSTR->subord,report->MTNMSTR->save_expression,
		report->MTNMSTR->save_error_desc,report->MTNMSTR->save_warning,
		report->MTNMSTR->save_warning_desc,
		report->MTNMSTR->inheritrsrcs,
		report->MTNMSTR->Use_Audit_Trail,report->MTNMSTR->Audit_Trail,
		report->MTNMSTR->UsePowerWrite,report->MTNMSTR->numPrevWrites,
		report->MTNMSTR->PrevWrites,report->MTNMSTR->numNewWrites,
		report->MTNMSTR->NewWrites,report->MTNMSTR->numActions,
		report->MTNMSTR->Actions,report);
}
APPlib *loadfilefieldlistA(RDArsrc *rsrc,char *modx,char *filx)
{
	int z=0,y=0;
	char *temp=NULL;
	NRDfield *fields=NULL,*f=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part=NULL;
	short engine=0,nofields=0,nokeys=0;
	APPlib *tmp=NULL;

	tmp=APPlibNEW();
	if(!RDAstrcmp(modx,"RPTGEN") && !RDAstrcmp(filx,"SORTFILE"))
	{
		makefieldvallistrpt(&tmp,TRUE,FALSE,TRUE,FALSE);
	} else {
		if(!GETBIN(modx,filx,&engine,&nofields,&fields,&nokeys,&keys)) 
		{
			if(fields!=NULL)
			{
				for(y=0,f=fields;y<nofields;++y,++f)
				{
					if(temp!=NULL)
					{
						temp=Rrealloc(temp,RDAstrlen(f->name)+RDAstrlen(modx)+RDAstrlen(filx)+7);
					} else {
						temp=Rmalloc(RDAstrlen(f->name)+RDAstrlen(modx)+RDAstrlen(filx)+7);
					}
					sprintf(temp,"[%s][%s][%s]",modx,filx,f->name);
					addAPPlib(tmp,temp);
					if(f->name!=NULL) Rfree(f->name);
				}
				Rfree(fields);
			}
			if(keys!=NULL)
			{
				for(y=0,keyx=keys;y<nokeys;++y,++keyx)
				{
					if(keyx->name!=NULL) Rfree(keyx->name);
					if(keyx->part!=NULL)
					{
						for(z=0,part=keyx->part;z<keyx->numparts;++z,++part)
						{
							if(part->name!=NULL) Rfree(part->name);
						}
						Rfree(keyx->part);
					}
				}
					Rfree(keys);
			}
		}
	}
	return(tmp);
}
