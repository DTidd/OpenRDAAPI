/* dfmtn.c - Define Maintain Masters */
#ifndef WIN32
#define __NAM__ "dfmtn.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dfmtn.exe"
#endif
#include <app.hpp>





#include <mix.hpp>
#include <ldval.hpp>
#include <doheader.hpp>
/*CPP_OPART dfmtnsup */
/*CPP_OPART dfmtnsub */
/*CPP_OPART dfmtnnew */
/*CPP_OPART dfmtnprv */
/*CPP_OPART dfmtnswt */
/*CPP_OPART dfmtnact */
/*CPP_OPART dfmtnscn */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="UTILITIES";
extern APPlib *holdapplib;
RDArsrc *mainrsrc=NULL,*mtnrsrc=NULL,*dfscnrsrc=NULL,*filersrc=NULL;
APPlib *actstructavl=NULL,*btntypes=NULL,*startwheres=NULL;
APPlib *modlstmtn=NULL,*mtnavl=NULL,*styles=NULL,*ftypes=NULL,*starttypes=NULL;
APPlib *dir_libs=NULL,*scr_defs=NULL,*file_defs=NULL,*diravl=NULL;
APPlib *buttonsavl=NULL,*fieldsavlmtn=NULL,*btnfldavl=NULL,*keysavlmtn=NULL;
APPlib *filelistmtn=NULL,*tmpargs=NULL,*modlstxmtn=NULL;
MaintainMaster *MTNMSTR=NULL;
void addholdvalues(APPlib *,char *,char *);
void addholdvaluessup(APPlib *,char *,char *);
void addholdvaluessub(APPlib *,char *,char *);
void addholdvaluesact(APPlib *,char *,char *);
static void savemaintain(RDArsrc *,RDArsrc *);
static void quitmaintaintest(RDArsrc *,RDArsrc *),setvaluesx(RDArsrc *);
static void quitmaintain(RDArsrc *,RDArsrc *);
static void printmtnfp(RDArsrc *,RDArsrc *);
static void printmtn(RDArsrc *,RDA_PFILE *);
static void fixmtnoutput(RDA_PFILE *,char *,int *,int *,char *);
static void checkheader(RDA_PFILE *,int *,int *,char *);
static void printheader(RDA_PFILE *,int *,int *,char *,char *);
static void printdefinition(RDArsrc *,void (*)(...));
static void printfp(RDArsrc *,void (*)(...),RDArsrc *);
static void printmodulefp(RDArsrc *prsrc,RDArsrc *rsrc);
static void printmodule(RDArsrc *,RDA_PFILE *);
static RDAdefault *mtntdefaults=NULL,*buttondefaults=NULL;
static RDAdefault *btnflddefaults=NULL,*argdefaults=NULL;
static char changedargs=FALSE,changedbtnfld=FALSE,changedbutton=FALSE;
char changedvalues=FALSE;
static char *snames[4]={
	"BROWSE SCREEN",
	"DEFINE LIST",
	"SEARCH BROWSE",
	"MAINTAIN SCREEN"}; 
static int which_screen=0,numHoldButtons=0,numHoldButtonField=0;
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
	"Search Browse Screen"};
static char *startwhere_names[2]={
	"Top of Browse",
	"Bottom of Browse"};
static void doliblist1(RDArsrc *),getscreenlist(RDArsrc *);
static MaintainButton *HoldButtons=NULL;
static ButtonField *HoldButtonField=NULL;
static void domtnmst(RDArsrc *);
PowerWrite *tempNewWrites=NULL,*tempPrevWrites=NULL;
int tmpnumActions=0,tmpnumNewWrites=0,tmpnumPrevWrites=0;
MTNAction *tempActions=NULL;
APPlib *newwrtavl=NULL,*prevwrtavl=NULL;
RDAsearch *tempsupport=NULL;
int tmpnumsupp=0;
RDAsubord *tempsubord=NULL;
int tmpnumsub=0;
APPlib *suppavlmtn=NULL,*subavlmtn=NULL,*ctypes=NULL,*BrowseTypes=NULL;
APPlib *filelstmtn=NULL,*cmodlstmtn=NULL,*subkeylistmtn=NULL,*cnfldsmtn=NULL;
void loadvaluemtn(RDArsrc *,char *);
APPlib *makefieldvallistmtn(MaintainMaster *);
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

extern void makeSearchBrowseScreen(MaintainMaster *,RDAsearch *);
extern void makeBrowseScreen(MaintainMaster *,RDAsearch *);
extern void makeDefineListScreen(MaintainMaster *,RDAsearch *);
extern void makeMaintainScreen(MaintainMaster *);
extern void makeDefaultMenu(MaintainMaster *);

extern void saveaction(RDArsrc *,MaintainMaster *);
extern void savesupp(RDArsrc *,MaintainMaster *);
extern void savenewwrt(RDArsrc *,MaintainMaster *);
extern void saveprevwrt(RDArsrc *,MaintainMaster *);
extern void savesub(RDArsrc *,MaintainMaster *);
extern void newgetprev(RDArsrc *);

extern void getActions(void);
extern void actionedit(RDArsrc *);
extern void actioncopy(RDArsrc *),actionaddabove(RDArsrc *);
extern void actionaddbelow(RDArsrc *),actiondelete(RDArsrc *);
extern void suppedit(RDArsrc *,RDArsrc *);
extern void suppcopy(RDArsrc *,RDArsrc *);
extern void getsupports(void);
extern void suppaddbelow(RDArsrc *,RDArsrc *),suppaddabove(RDArsrc *,RDArsrc *);
extern void newwrtcopy(RDArsrc *),newwrtedit(RDArsrc *);
extern void getNewWrites(void);
extern void newwrtaddbelow(RDArsrc *),newwrtaddabove(RDArsrc *);
extern void newwrtmoveup(RDArsrc *),newwrtmovedown(RDArsrc *);
extern void newwrtdelete(RDArsrc *);
extern void prevwrtcopy(RDArsrc *),prevwrtedit(RDArsrc *);
extern void getPrevWrites(void);
extern void prevwrtaddbelow(RDArsrc *),prevwrtaddabove(RDArsrc *);
extern void prevwrtmoveup(RDArsrc *),prevwrtmovedown(RDArsrc *);
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
APPlib *makefieldvallist2mtn(RDAsearch *,int,int,short,char *,char *);
APPlib *loadfilefieldlistA(RDArsrc *,char *,char *);

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
	SORTAPPlib(tmp);
	return(tmp);
}
void addholdvalues(APPlib *tmp,char *modulex,char *filex)
{
	NRDfield *f=NULL,*fields=NULL;
	NRDkey *keys=NULL,*keyx=NULL;
	NRDpart *part=NULL;
	short z,engine=0,nofields=0,nokeys=0;
	int y;
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
						temp=Rrealloc(temp,RDAstrlen(f->name)+
							RDAstrlen(modulex)+
							RDAstrlen(filex)+7);
					} else 	{
						temp=Rmalloc(RDAstrlen(f->name)+RDAstrlen(modulex)+
							RDAstrlen(filex)+7);
					}
					sprintf(temp,"[%s][%s][%s]",modulex,filex,f->name);
					addAPPlibNoDuplicates(tmp,temp);
					if(f->name!=NULL) Rfree(f->name);
				}
				Rfree(fields);
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
	if(temp!=NULL) Rfree(temp);
}
void addholdvaluessub(APPlib *tmp,char *modulex,char *filex)
{
	NRDfield *f,*fields=NULL;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short z,engine=0,nofields=0,nokeys=0;
	int y;
	char *temp=NULL;

	if(diagrptgen)
	{
		prterr("DIAG addholdvaluessub Module [%s] File [%s] ",modulex,filex);
	}
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
	if(temp!=NULL) Rfree(temp);
}
void addholdvaluesact(APPlib *tmp,char *modulex,char *filex)
{
	NRDfield *f,*fields=NULL;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short z,engine=0,nofields=0,nokeys=0;
	int y;
	char *temp=NULL;

	if(diagrptgen)
	{
		prterr("DIAG addholdvaluessub Module [%s] File [%s] ",modulex,filex);
	}
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

	tmp=APPlibNEW();
	tmprsrc=RDArsrcNEW(module,"TEMP");
	addDFincvir(tmprsrc,MTNMSTR->module,MTNMSTR->mainfile,NULL,dumb);
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
	loadlist=makefieldvallistmtn(MTNMSTR);
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
static void okselectfile(RDArsrc *filersrc,RDArsrc *mrsrc)
{
	int f=0;

	if(FINDRSCGETINT(filersrc,"FILE LIST",&f)) return;
	if(!FINDRSCSETSTRING(mrsrc,"MAINFILE",file_defs->libs[f]))
		updatersrc(mrsrc,"MAINFILE");
	NULL_RSCS(filersrc);
}
static void doliblist(RDArsrc *dfscnrsrc,RDArsrc *mtnrsrc)
{
	char *libx=NULL,*modulx=NULL;
	char *dirx=NULL;
	int sscn=0;

	readwidget(dfscnrsrc,"DIRECTORY");
	if(FINDRSCGETSTRING(dfscnrsrc,"DIRECTORY",&dirx)) return;
	FINDRSCGETINT(dfscnrsrc,"SCREEN LIST",&sscn);
	readwidget(mtnrsrc,"MODULE");
	if(FINDRSCGETSTRING(mtnrsrc,"MODULE",&modulx)) return;
	if(scr_defs!=NULL) freeapplib(scr_defs);
	libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(modulx)+6);
#ifndef WIN32
	sprintf(libx,"%s/%s.SCN",dirx,modulx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.SCN",dirx,modulx);
#endif
	if(!isEMPTY(modulx)) scr_defs=getlibnames(libx,FALSE);
	if(dirx!=NULL) Rfree(dirx);
	if(libx!=NULL) Rfree(libx);
	if(modulx!=NULL) Rfree(modulx);
	if(scr_defs==NULL)
	{
		scr_defs=APPlibNEW();
		addAPPlib(scr_defs,"Contains No Screen Definitions");
	}
	if(sscn>=scr_defs->numlibs) sscn=0;
	if(!FINDRSCLISTAPPlib(dfscnrsrc,"SCREEN LIST",sscn,scr_defs))
		updatersrc(dfscnrsrc,"SCREEN LIST");
}
static void getdir(RDArsrc *dfscnrsrc,RDArsrc *mtnrsrc)
{
	int s=0;
	char *dirstr=NULL;

	if(FINDRSCGETSTRING(dfscnrsrc,"DIRECTORY",&dirstr)) return;
	if(FINDRSCGETINT(dfscnrsrc,"DIRECTORY LIST",&s)) return;
	if(RDAstrcmp(dirstr,dir_libs->libs[s]))
	{
		if(FINDRSCSETSTRING(dfscnrsrc,"DIRECTORY",dir_libs->libs[s])) return;
		updatersrc(dfscnrsrc,"DIRECTORY");
	}
	if(dirstr!=NULL) Rfree(dirstr);
	doliblist(dfscnrsrc,mtnrsrc);
}
static void setdirlist(RDArsrc *dfscnrsrc,RDArsrc *mtnrsrc)
{
	int x=0;
	char *directory=NULL;

	readwidget(dfscnrsrc,"DIRECTORY");
	if(FINDRSCGETSTRING(dfscnrsrc,"DIRECTORY",&directory)) return;
	for(x=0;x<dir_libs->numlibs;++x)
	{
		if(!RDAstrcmp(dir_libs->libs[x],directory))
			break;
	}
	if(x>=dir_libs->numlibs)
	{
		addAPPlib(dir_libs,directory);
		x=dir_libs->numlibs-1;
	}
	if(directory!=NULL) Rfree(directory);
	FINDRSCLISTAPPlib(dfscnrsrc,"DIRECTORY LIST",x,dir_libs);
	updatersrc(dfscnrsrc,"DIRECTORY LIST");
	doliblist(dfscnrsrc,mtnrsrc);
}
static void getscreen(RDArsrc *mtnrsrc,RDArsrc *dfscnrsrc)
{
	doliblist(dfscnrsrc,mtnrsrc);
	APPmakescrn(dfscnrsrc,TRUE,NULL,NULL,FALSE);
}
static void getscreenbs(RDArsrc *mtnrsrc,RDArsrc *dfscnrsrc)
{
	which_screen=0;
	getscreen(mtnrsrc,dfscnrsrc);
}
static void getscreends(RDArsrc *mtnrsrc,RDArsrc *dfscnrsrc)
{
	which_screen=1;
	getscreen(mtnrsrc,dfscnrsrc);
}
static void getscreensb(RDArsrc *mtnrsrc,RDArsrc *dfscnrsrc)
{
	which_screen=2;
	getscreen(mtnrsrc,dfscnrsrc);
}
static void getscreenms(RDArsrc *mtnrsrc,RDArsrc *dfscnrsrc)
{
	which_screen=3;
	getscreen(mtnrsrc,dfscnrsrc);
}
static void okselectscreen(RDArsrc *dfscnrsrc,RDArsrc *mtnrsrc)
{
	int scrnum=0;

	if(FINDRSCGETINT(dfscnrsrc,"SCREEN LIST",&scrnum)) return;
	if(!FINDRSCSETSTRING(mtnrsrc,snames[which_screen],scr_defs->libs[scrnum]))
		updatersrc(mtnrsrc,snames[which_screen]);
	NULL_RSCS(dfscnrsrc);
}
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

	if(holdapplib!=NULL) freeapplib(holdapplib);
	if(suppavlmtn!=NULL) freeapplib(suppavlmtn);
	if(subavlmtn!=NULL) freeapplib(subavlmtn);
	if(ctypes!=NULL) freeapplib(ctypes);
	if(BrowseTypes!=NULL) freeapplib(BrowseTypes);
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
	if(dir_libs!=NULL) freeapplib(dir_libs);
	if(scr_defs!=NULL) freeapplib(scr_defs);
	if(file_defs!=NULL) freeapplib(file_defs);
	if(buttonsavl!=NULL) freeapplib(buttonsavl);
	changedvalues=FALSE;
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
	if(dfscnrsrc!=NULL) free_rsrc(dfscnrsrc);
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
	if(style_selected>1)
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
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD VALUE SAVE WARNING DESCRIPTION",
			TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD OPERAND SAVE WARNING DESCRIPTION",
			TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD GROUPER SAVE WARNING DESCRIPTION",
			TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"LOAD CONTROL SAVE WARNING DESCRIPTION",
			TRUE);
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
	char *libname=NULL;
	char *directory=NULL;
	char inheritrsrcs=FALSE,def_search_browse=FALSE,def_browse_screen=FALSE;
	char def_define_list=FALSE,def_maintain_screen=FALSE,def_menu;
	int x,y;
	MaintainButton *b1,*b2;
	ButtonField *bf1,*bf2;

	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"MODULE",&MTNMSTR->module);
	FINDRSCGETSTRING(mtnrsrc,"NAME",&MTNMSTR->name);
	FINDRSCGETCHAR(mtnrsrc,"DEFAULT SEARCH BROWSE",&def_search_browse);
	FINDRSCGETCHAR(mtnrsrc,"DEFAULT BROWSE SCREEN",&def_browse_screen);
	FINDRSCGETCHAR(mtnrsrc,"DEFAULT DEFINE LIST",&def_define_list);
	FINDRSCGETCHAR(mtnrsrc,"DEFAULT MAINTAIN SCREEN",&def_maintain_screen);
	FINDRSCGETCHAR(mtnrsrc,"DEFAULT MENU",&def_menu);
	if(FINDRSCGETSTRING(mtnrsrc,"DIRECTORY",&directory)) return;
	libname=Rmalloc(RDAstrlen(directory)+RDAstrlen(MTNMSTR->module)+6);
#ifndef WIN32
	sprintf(libname,"%s/%s.MTN",directory,MTNMSTR->module);
#endif
#ifdef WIN32
	sprintf(libname,"%s\\%s.MTN",directory,MTNMSTR->module);
#endif
	FINDRSCGETSTRING(mtnrsrc,"NAME",&MTNMSTR->name);
	if(!isEMPTY(MTNMSTR->name))
	{
		FINDRSCGETCHAR(mtnrsrc,"USE POWER WRITE",&MTNMSTR->UsePowerWrite);
		FINDRSCGETCHAR(mtnrsrc,"USE AUDIT TRAIL",&MTNMSTR->Use_Audit_Trail);
		FINDRSCGETSTRING(mtnrsrc,"AUDIT TRAIL",&MTNMSTR->Audit_Trail);
		FINDRSCGETSTRING(mtnrsrc,"DESCRIPTION",&MTNMSTR->desc);
		FINDRSCGETINT(mtnrsrc,"STYLES",&MTNMSTR->style);
		FINDRSCGETSTRING(mtnrsrc,"MAINFILE",&MTNMSTR->mainfile);
		FINDRSCGETSTRING(mtnrsrc,"BROWSE SCREEN",&MTNMSTR->browse_screen);
		FINDRSCGETSTRING(mtnrsrc,"DEFINE LIST",&MTNMSTR->define_list);
		FINDRSCGETSTRING(mtnrsrc,"SEARCH BROWSE",&MTNMSTR->search_browse);
		FINDRSCGETSTRING(mtnrsrc,"MAINTAIN SCREEN",&MTNMSTR->maintain_screen);
		FINDRSCGETINT(mtnrsrc,"FILE TYPES",&MTNMSTR->file_type);
		FINDRSCGETINT(mtnrsrc,"START TYPES",&MTNMSTR->start_with);
		FINDRSCGETINT(mtnrsrc,"START WHERES",&MTNMSTR->start_where);
		FINDRSCGETSTRING(mtnrsrc,"SAVE ERROR",&MTNMSTR->save_expression);
		FINDRSCGETSTRING(mtnrsrc,"SAVE ERROR DESCRIPTION",
			&MTNMSTR->save_error_desc);
		FINDRSCGETSTRING(mtnrsrc,"SAVE WARNING",&MTNMSTR->save_warning);
		FINDRSCGETSTRING(mtnrsrc,"SAVE WARNING DESCRIPTION",
			&MTNMSTR->save_warning_desc);
		FINDRSCGETCHAR(mtnrsrc,"INHERIT RESOURCE VALUES",&inheritrsrcs);
		MTNMSTR->inheritrsrcs=(short)inheritrsrcs;
		if(MTNMSTR->buttons!=NULL)
		{
			for(x=0,b1=MTNMSTR->buttons;x<MTNMSTR->numbuttons;++x,++b1)
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
			Rfree(MTNMSTR->buttons);
			MTNMSTR->numbuttons=0;
		}
		if(HoldButtons!=NULL)
		{
			MTNMSTR->numbuttons=numHoldButtons;
			MTNMSTR->buttons=Rmalloc(numHoldButtons*sizeof(MaintainButton));
			for(x=0,b1=HoldButtons,b2=MTNMSTR->buttons;x<numHoldButtons;
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
		savesupp(mtnrsrc,MTNMSTR);
		savesub(mtnrsrc,MTNMSTR);
		savenewwrt(mtnrsrc,MTNMSTR);
		saveprevwrt(mtnrsrc,MTNMSTR);
		saveaction(mtnrsrc,MTNMSTR);
		if(writeMaintainMaster(libname,MTNMSTR))
		{
			ERRORDIALOG("Cannot Write Maintain Master","The user cannot write the Maintain Master Definition.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
		}
		if(def_search_browse && !isEMPTY(MTNMSTR->mainfile) && !isEMPTY(MTNMSTR->search_browse)) makeSearchBrowseScreen(MTNMSTR,NULL);
		if(def_browse_screen && !isEMPTY(MTNMSTR->browse_screen)) makeBrowseScreen(MTNMSTR,NULL);
		if(def_define_list && !isEMPTY(MTNMSTR->mainfile) && !isEMPTY(MTNMSTR->define_list)) makeDefineListScreen(MTNMSTR,NULL);
		if(def_maintain_screen && !isEMPTY(MTNMSTR->mainfile) && !isEMPTY(MTNMSTR->maintain_screen)) makeMaintainScreen(MTNMSTR);
		if(def_menu && !isEMPTY(MTNMSTR->module) && !isEMPTY(MTNMSTR->name)) makeDefaultMenu(MTNMSTR);
		FreeMaintainMaster(MTNMSTR);
		MTNMSTR=NULL;
	}
	if(libname!=NULL) Rfree(libname);
	if(directory!=NULL) Rfree(directory);
	doliblist1(mainrsrc);
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
	p->rname=stralloc(fieldsavlmtn->libs[selected]);
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
	temp=adddashes(fieldsavlmtn->libs[x]);
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
	for(x=0;x<fieldsavlmtn->numlibs;++x)
	{
		if(!RDAstrcmp(rname,fieldsavlmtn->libs[x])) break;
	}
	if(x>=fieldsavlmtn->numlibs) x=0;
	addlstrsrc(btnfldrsrc,"FIELD LIST",&x,TRUE,setenvname,fieldsavlmtn->numlibs,
		&fieldsavlmtn->libs,NULL);
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
static void copy_btnfld(RDArsrc *buttonrsrc)
{
	int x;
	ButtonField *b=NULL;

	if(FINDRSCGETINT(buttonrsrc,"FIELDS AVAILABLE",&x)) return;
	if(!RDAstrcmp(btnfldavl->libs[x],"No Fields Defined"))
	{
		buttonfieldscreen(buttonrsrc,x,NULL,NULL,savebtnfld,quitbtnfldtest);
	} else {
		b=HoldButtonField+x;
		buttonfieldscreen(buttonrsrc,++x,b->rname,b->ename,savebtnfldbelow,quitbtnfldbelowtest);
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
	if(fieldsavlmtn!=NULL) freeapplib(fieldsavlmtn);
	if(filelistmtn!=NULL) freeapplib(filelistmtn);
	if(modlstxmtn!=NULL) freeapplib(modlstxmtn);
	if(keysavlmtn!=NULL) freeapplib(keysavlmtn);
	if(buttondefaults!=NULL) FreeRDAdefault(buttondefaults);
	fieldsavlmtn=NULL;
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
	button->filename=stralloc(filelistmtn->libs[selfil]);
	button->keyname=stralloc(keysavlmtn->libs[selkey]);
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
			l=RDAstrlen(button->rname)+RDAstrlen(button->ename)+47;
			s=Rmalloc(l);
			sprintf(s,"%2d Field: [%s] Environment: [%s]",x+1,
				(button->rname!=NULL ? button->rname:""),
				(button->ename!=NULL ? button->ename:""));
			addAPPlib(btnfldavl,s);
			Rfree(s);
		}
	}
}
static void changefilelistmtn(RDArsrc *mainrsrc)
{
	int selected=0,selectedf=0,selectedk=0;
	short x,y,engine=0,nokeys=0,nofields=0;
	NRDfield *fieldx=NULL,*field;
	NRDkey *keyx=NULL,*key;
	NRDpart *part;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"FILE LIST",&selectedf)) return;
	FINDRSCGETINT(mainrsrc,"KEYS AVAILABLE",&selectedk);
	if(keysavlmtn!=NULL) freeapplib(keysavlmtn); 
	keysavlmtn=APPlibNEW();
	if(!GETBIN(modlstxmtn->libs[selected],filelistmtn->libs[selectedf],&engine,
		&nofields,&fieldx,&nokeys,&keyx))
	{
		if(keyx!=NULL)
		{
			for(x=0,key=keyx;x<nokeys;++x,++key)
			{
				addAPPlib(keysavlmtn,key->name);
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
	if(keysavlmtn->numlibs<1)
	{
		addAPPlib(keysavlmtn,"No Keys Defined");
	}
	if(selectedk>=keysavlmtn->numlibs) selectedk=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"KEYS AVAILABLE",selectedk,keysavlmtn))
		updatersrc(mainrsrc,"KEYS AVAILABLE");
}
static void changemodlist(RDArsrc *mainrsrc)
{
	int selected=0,selectedf=0;
	char *libx=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	FINDRSCGETINT(mainrsrc,"FILE LIST",&selectedf);
	if(filelistmtn!=NULL) freeapplib(filelistmtn); 
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlstxmtn->libs[selected])+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modlstxmtn->libs[selected]);
#endif 
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modlstxmtn->libs[selected]);
#endif
	if(!isEMPTY(modlstxmtn->libs[selected])) filelistmtn=getlibnames(libx,TRUE);
	if(libx!=NULL) Rfree(libx);
	if(filelistmtn==NULL)
	{
		filelistmtn=APPlibNEW();
		addAPPlib(filelistmtn,"No Files Defined");
	}
	if(selectedf>=filelistmtn->numlibs) selectedf=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"FILE LIST",selectedf,filelistmtn))
		updatersrc(mainrsrc,"FILE LIST");
	changefilelistmtn(mainrsrc);
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
		argumentscreen(buttonrsrc,NULL,save_arg_above,
			quit_arg_above_test);
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
		argumentscreen(buttonrsrc,NULL,save_arg_below,
			quit_arg_below_test);
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
static void copy_arg(RDArsrc *buttonrsrc)
{
	int x;

	if(FINDRSCGETINT(buttonrsrc,"ARGUMENT LIST",&x)) return;
	if(!RDAstrcmp(tmpargs->libs[x],"No Arguments"))
	{
		argumentscreen(buttonrsrc,NULL,save_arg,quit_arg_test);
	} else {
		argumentscreen(buttonrsrc,tmpargs->libs[x],save_arg_below,
			quit_arg_below_test);
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
	if(fieldsavlmtn!=NULL) freeapplib(fieldsavlmtn);
	fieldsavlmtn=APPlibNEW();
	tmpargs=APPlibNEW();
	if(args!=NULL)
	{
		for(x=0;x<args->numlibs;++x) addAPPlib(tmpargs,args->libs[x]);
	}
	if(tmpargs->numlibs<1)
	{
		addAPPlib(tmpargs,"No Arguments");
	}
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
				addAPPlib(fieldsavlmtn,field->name);
				if(field->name!=NULL) Rfree(field->name);
			}
			Rfree(fieldx);
		}
	}
	if(fieldsavlmtn->numlibs<1)
	{
		addAPPlib(fieldsavlmtn,"No Fields Defined");
	}
	if(modx!=NULL) Rfree(modx);
	if(filx!=NULL) Rfree(filx);
	if(bf!=NULL)
	{
		numHoldButtonField=num;
		HoldButtonField=Rmalloc(num*sizeof(ButtonField));
		for(x=0,b1=bf,b2=HoldButtonField;x<num;++x,++b1,++b2)
		{
			if(b1->rname!=NULL) b2->rname=stralloc(b1->rname);
				else b2->rname=NULL;
			if(b1->ename!=NULL) b2->ename=stralloc(b1->ename);
				else b2->ename=NULL;
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
	addlstrsrc(buttonrsrc,"FIELDS AVAILABLE",0,TRUE,NULL,btnfldavl->numlibs,
		&btnfldavl->libs,NULL);
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
	if(filelistmtn!=NULL) freeapplib(filelistmtn);
	filelistmtn=APPlibNEW();
	addlstrsrc(buttonrsrc,"FILE LIST",&x,TRUE,changefilelistmtn,filelistmtn->numlibs,&filelistmtn->libs,NULL);
	changemodlist(buttonrsrc);
	for(x=0;x<filelistmtn->numlibs;++x)
	{
		if(!RDAstrcmp(filelistmtn->libs[x],filename)) break;
	}
	if(x<filelistmtn->numlibs)
		FINDRSCSETINT(buttonrsrc,"FILE LIST",x);
	keysavlmtn=APPlibNEW();
	addlstrsrc(buttonrsrc,"KEYS AVAILABLE",&x,TRUE,NULL,keysavlmtn->numlibs,
		&keysavlmtn->libs,NULL);
	changefilelistmtn(buttonrsrc);
	for(x=0;x<keysavlmtn->numlibs;++x)
	{
		if(!RDAstrcmp(keyname,keysavlmtn->libs[x])) break;
	}
	if(x<keysavlmtn->numlibs)
	{
		FINDRSCSETINT(buttonrsrc,"KEYS AVAILABLE",x);
	}
	addbtnrsrc(buttonrsrc,"ADD BUTTON ABOVE",TRUE,add_btnfld_above,NULL);
	addbtnrsrc(buttonrsrc,"ADD BUTTON BELOW",TRUE,add_btnfld_below,NULL);
	addbtnrsrc(buttonrsrc,"SELECT BUTTON",TRUE,select_btnfld,NULL);
	addbtnrsrc(buttonrsrc,"COPY BUTTON",TRUE,copy_btnfld,NULL);
	addbtnrsrc(buttonrsrc,"DELETE BUTTON",TRUE,delete_btnfld,NULL);
	x=0;
	addlstrsrc(buttonrsrc,"ARGUMENT LIST",&x,TRUE,NULL,tmpargs->numlibs,
		&tmpargs->libs,NULL);
	addbtnrsrc(buttonrsrc,"ADD ARGUMENT ABOVE",TRUE,add_arg_above,NULL);
	addbtnrsrc(buttonrsrc,"ADD ARGUMENT BELOW",TRUE,add_arg_below,NULL);
	addbtnrsrc(buttonrsrc,"SELECT ARGUMENT",TRUE,select_arg,NULL);
	addbtnrsrc(buttonrsrc,"COPY ARGUMENT",TRUE,copy_arg,NULL);
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
	char *s=NULL,*temp=NULL;

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
				RDAstrlen(btntypes->libs[button->type])+84;
			s=Rmalloc(l);
			sprintf(s,"%2d Button: [%s] Program: [%s] Module: [%s] File: [%s] Keyname: [%s] List: [%s] Type: [%s]",x+1,
				(button->buttonname!=NULL?button->buttonname:""),
				(button->progname!=NULL?button->progname:""),
				(button->modulename!=NULL?button->modulename:""),
				(button->filename!=NULL?button->filename:""),
				(button->keyname!=NULL?button->keyname:""),
				(button->listname!=NULL?button->listname:""),
				(btntypes->libs[button->type]!=NULL?btntypes->libs[button->type]:""));
			if(RDAstrlen(s)>120)
			{
				temp=s+120;
				*temp=0;
			}
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
static void copy_button(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
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
	}
	if(!RDAstrcmp(buttonsavl->libs[x],"No Buttons Available"))
	{
		buttonscreen(mtnrsrc,mainrsrc,x,NULL,NULL,NULL,NULL,0,0,NULL,
			save_button,quitbuttontest,modulx,NULL,NULL,FALSE,NULL);
	} else {
		b=HoldButtons+x;
		buttonscreen(mtnrsrc,mainrsrc,x,b->progname,b->listname,
			b->buttonname,b->expression,b->type,b->num,
			b->bf,save_button_below,quitbuttonbelowtest,
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
static void fixmtnoutput(RDA_PFILE *fp,char *data,int *lines,int *pages,char *modname)
{
	unsigned y=0,x=0,length=0,line_length=79;
	char *temp=NULL,*temp1=NULL;

	if(!isEMPTY(data))
	{
		checkheader(fp,lines,pages,modname);
		temp=strchr(data,'\n');
		temp1=strchr(data,'\r');
		if(temp==NULL && temp1==NULL) length=RDAstrlen(data);
		else if(temp==NULL) length=temp1-data;
		else if(temp1==NULL) length=temp-data;
		else if(temp<temp1) length=temp-data;
		else length=temp1-data;
		for(x=0,y=0;x<length;++x,++y)
		{
			if(data[x]=='\t')
			{
				if(y+9>line_length) break;
				y+=7;
			}
			if(y+2>line_length) break;
		}
		if(x<length)
		{
			RDA_fprintf(fp,"%.*s\r\n",x+1,data);
			++*lines;
			fixmtnoutput(fp,data+x+1,lines,pages,modname);
		} else {
			RDA_fprintf(fp,"%.*s\r\n",length,data);
			++*lines;
			if(temp!=NULL || temp1!=NULL) fixmtnoutput(fp,data+length+1,lines,pages,modname);
		}
	} 
}
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
				RDA_fprintf(fp,"      Browse Button Name: [%s] Smart: [%s]\r\n",
					(tempsupport->button_name!=NULL?tempsupport->button_name:""),
					(tempsupport->smart_browse==TRUE?"TRUE":"FALSE"));
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
static void printmtn(RDArsrc *rsrc,RDA_PFILE *fp)
{
	int lines=0,pages=0;
/*
	char *temp=NULL;

	temp=stralloc("XPERT MAINTAIN MASTER DEFINITION"); 
	printheader(fp,&lines,&pages,temp,MTNMSTR->module);
	if(temp!=NULL) Rfree(temp);
*/
	printmtn1(fp,MTNMSTR,&lines,&pages);
	RDA_fprintf(fp,"\f\r\n");
}
static void quit_print_definition(RDArsrc *prsrc)
{
	if(prsrc!=NULL) Rfree(prsrc);
}
static void checkheader(RDA_PFILE *fp,int *lines,int *pages,char *modname)
{
	char *temp=NULL;

	if(doheaders)
	{
		if(*lines>57)
		{
			RDA_fprintf(fp,"\f");
			temp=stralloc("XPERT MAINTAIN MASTER DEFINITION"); 
			printheader(fp,lines,pages,temp,modname);
			if(temp!=NULL) Rfree(temp);
		}
	}
}
static void printheader(RDA_PFILE *fp,int *lines,int *pages,char *header,char *modname)
{
	char *date=NULL,*timex=NULL;
	int lead_ws=0,tail_ws=0;
	unsigned line_length=79;

	date=GETCURRENTDATE10();
	timex=GETCURRENTTIME();
	RDA_fprintf(fp,"\r\n");
	*lines=1;
	lead_ws=(int)(RDAroundup((double)(line_length-(2+RDAstrlen(header)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(2+RDAstrlen(header)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(header)+2+16+lead_ws+tail_ws)>line_length) header[(line_length-16-lead_ws-tail_ws)]=0;
	RDA_fprintf(fp,"%*s %s %*sPage: %*d\r\n",
		lead_ws,"",header,tail_ws,"",10,++*pages);
	++*lines;
	lead_ws=(int)(RDAroundup((double)(line_length-(10+RDAstrlen(modname)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(10+RDAstrlen(modname)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	RDA_fprintf(fp,"%*s Module: %s %*sDate: %s\r\n",
		lead_ws,"",(modname!=NULL?modname:""),tail_ws,"",date);
	++*lines;
	lead_ws=(int)(RDAroundup(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(USERLOGIN)+16+15+lead_ws+tail_ws)>line_length)
	{
		RDA_fprintf(fp,"%*s %s %*sTime:   %s\r\n",
			(int)(RDAroundup(((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2))),"",
			USERLOGIN,
			(int)(Rint((((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2)-16))),"",
			timex);
		++*lines;
	} else {
		RDA_fprintf(fp,"%*s Executed By: %s %*sTime:   %s\r\n",
			lead_ws,"",USERLOGIN,tail_ws,"",timex);
		++*lines;
	}
	RDA_fprintf(fp,"\r\n");
	++*lines;
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
}
static void printdefinition(RDArsrc *mtn,void (*printfunc)(...))
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL,normal_print=TRUE;

	prsrc=RDArsrcNEW(module,"PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addbtnrsrc(prsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	addstdrsrc(prsrc,"NORMAL PRINT",BOOLNS,1,&normal_print,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printfunc,mtn);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_print_definition,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PRINT SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		quit_print_definition(prsrc);
	}
}
static void printfp(RDArsrc *prsrc,void (*pfunc)(...),RDArsrc *rsrc)
{
	char *outdevice=NULL,*temp=NULL,print_style=TRUE;
	RDA_PFILE *fp=NULL;

	readwidget(prsrc,"DEVICE");
        FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
	readwidget(prsrc,"NORMAL PRINT");
	FINDRSCGETCHAR(prsrc,"NORMAL PRINT",&print_style);
	if(outdevice!=NULL)
	{
		fp=RDA_popen(outdevice,"w");
		if(fp!=NULL)
		{
			SelectPrintType(outdevice,print_style,fp);
/*lint -e746 */
			pfunc(rsrc,fp);
/*lint +e746 */
			RDA_pclose(fp);
		} else {
			temp=Rmalloc(RDAstrlen(outdevice)+70);
			sprintf(temp,"The output attempt to the DEVICE [%s] failed when trying to open pipe.",outdevice);
			WARNINGDIALOG("WARNING DIALOG SCREEN","DEVICE COMMUNICATIONS FAILED!",temp,NULL,NULL,TRUE,NULL);
			prterr(temp);
			if(temp!=NULL) Rfree(temp);
		}
		if(outdevice!=NULL) Rfree(outdevice);
	} else {
		ERRORDIALOG("Output Device Failure","There was no output device specified.",NULL,FALSE);
	}
}
static void printmodulefp(RDArsrc *prsrc,RDArsrc *rsrc)
{
	printfp(prsrc,printmodule,rsrc);
}
static void printmtnfp(RDArsrc *prsrc,RDArsrc *rsrc)
{
	printfp(prsrc,printmtn,rsrc);
}
static void enable_powerwrite(RDArsrc *r)
{
	char bv=FALSE;

	readwidget(r,"USE POWER WRITE");
	FINDRSCGETCHAR(r,"USE POWER WRITE",&bv);
	if(bv==FALSE)
	{
		FINDRSCSETEDITABLE(r,"PREV WRITE LIST",FALSE);
		FINDRSCSETEDITABLE(r,"MOVE PREV WRITE UP",FALSE);
		FINDRSCSETEDITABLE(r,"MOVE PREV WRITE DOWN",FALSE);
		FINDRSCSETEDITABLE(r,"MOVE NEW WRITE UP",FALSE);
		FINDRSCSETEDITABLE(r,"MOVE NEW WRITE DOWN",FALSE);
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
		FINDRSCSETEDITABLE(r,"OBTAIN PREVIOUS",FALSE);
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
		FINDRSCSETSENSITIVE(r,"OBTAIN PREVIOUS",FALSE);
		FINDRSCSETSENSITIVE(r,"MOVE PREV WRITE UP",FALSE);
		FINDRSCSETSENSITIVE(r,"MOVE PREV WRITE DOWN",FALSE);
		FINDRSCSETSENSITIVE(r,"MOVE NEW WRITE UP",FALSE);
		FINDRSCSETSENSITIVE(r,"MOVE NEW WRITE DOWN",FALSE);
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
		FINDRSCSETSENSITIVE(r,"OBTAIN PREVIOUS",TRUE);
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
		FINDRSCSETEDITABLE(r,"OBTAIN PREVIOUS",TRUE);
		FINDRSCSETEDITABLE(r,"MOVE PREV WRITE UP",TRUE);
		FINDRSCSETEDITABLE(r,"MOVE PREV WRITE DOWN",TRUE);
		FINDRSCSETEDITABLE(r,"MOVE NEW WRITE UP",TRUE);
		FINDRSCSETEDITABLE(r,"MOVE NEW WRITE DOWN",TRUE);
		FINDRSCSETSENSITIVE(r,"MOVE PREV WRITE UP",TRUE);
		FINDRSCSETSENSITIVE(r,"MOVE PREV WRITE DOWN",TRUE);
		FINDRSCSETSENSITIVE(r,"MOVE NEW WRITE UP",TRUE);
		FINDRSCSETSENSITIVE(r,"MOVE NEW WRITE DOWN",TRUE);
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
static void dfscn(RDArsrc *r)
{
	APPlib *args=NULL;
	char *m=NULL;
	
	readwidget(r,"MODULE");
	FINDRSCGETSTRING(r,"MODULE",&m);
	args=APPlibNEW();
	if(!isEMPTY(m)) addAPPlib(args,m);
	ExecuteProgram("dfscn",args);
	if(m!=NULL) Rfree(m);
	if(args!=NULL) freeapplib(args);
}
static void dfscn_screen(RDArsrc *r,char *screen)
{
	APPlib *args=NULL;
	char *m=NULL;
	char *s=NULL;
	
	readwidget(r,"MODULE");
	FINDRSCGETSTRING(r,"MODULE",&m);
	args=APPlibNEW();
	if(!isEMPTY(m)) addAPPlib(args,m);
	readwidget(r,screen);
	FINDRSCGETSTRING(r,screen,&s);
	if(!isEMPTY(s)) addAPPlib(args,s);
	ExecuteProgram("dfscn",args);
	if(m!=NULL) Rfree(m);
	if(args!=NULL) freeapplib(args);
}
static void dfvir(RDArsrc *r)
{
	APPlib *args=NULL;
	char *m=NULL;
	
	readwidget(r,"MODULE");
	FINDRSCGETSTRING(r,"MODULE",&m);
	args=APPlibNEW();
	if(!isEMPTY(m)) addAPPlib(args,m);
	ExecuteProgram("dfvir",args);
	if(m!=NULL) Rfree(m);
	if(args!=NULL) freeapplib(args);
}
static void dfmenu(RDArsrc *r)
{
	APPlib *args=NULL;
	char *m=NULL,*name=NULL;
	
	readwidget(r,"MODULE");
	FINDRSCGETSTRING(r,"MODULE",&m);
	readwidget(r,"NAME");
	FINDRSCGETSTRING(r,"NAME",&name);
	args=APPlibNEW();
	if(!isEMPTY(m)) 
	{
		addAPPlib(args,m);
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
	
	readwidget(r,"MODULE");
	FINDRSCGETSTRING(r,"MODULE",&m);
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
	
	readwidget(r,"MODULE");
	FINDRSCGETSTRING(r,"MODULE",&m);
	args=APPlibNEW();
	if(!isEMPTY(m)) addAPPlib(args,m);
	ExecuteProgram("dfload",args);
	if(m!=NULL) Rfree(m);
	if(args!=NULL) freeapplib(args);
}
static void maintainscreen(RDArsrc *mainrsrc,char *modulx,char *name,
	char *desc,int style,char *mainfile,char *browse_screen,
	char *define_list,char *search_browse,char *maintain_screen,
	int file_type,int start_with,int startwhere,int numbuttons,MaintainButton *buttons,
	int numsupport,RDAsearch *s,int numsubord,RDAsubord *sub,
	char *save_e,char *save_ed,char *save_w,char *save_wd,
	short inheritrsrcs,char Use_Audit_Trail,char *Audit_Trail,
	char UsePowerWrite,int numPrevWrites,PowerWrite *PrevWrites,
	int numNewWrites,PowerWrite *NewWrites,int numActions,MTNAction *Actions)
{
	int x=0,z=0,y=0;
	RDArsrc *mtnrsrc=NULL;
	char *defdir=NULL;
	MaintainButton *b1,*b2;
	ButtonField *bf1,*bf2;
	RDAsearch *tempsupp1,*tempsupp;
	RDAsubord *tempsub1,*tempsub;
	RDAconfld *confld1,*confld2;
	char tempinheritrsrcs=FALSE,make_default=FALSE;
	PowerWrite *W1=NULL,*W2=NULL;
	PowerWriteField *WF1=NULL,*WF2=NULL;
	MTNAction *action1=NULL,*action2=NULL;
	Actionstruct *actstruct1=NULL,*actstruct2=NULL;

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
	mtnrsrc=RDArsrcNEW(module,"EDIT MAINTAIN MASTER");
        if(styles!=NULL) freeapplib(styles);
        styles=APPlibNEW();
        for(x=0;x<3;++x) addAPPlib(styles,style_names[x]);
/*
	if(styles!=NULL) freeapplib(styles);
        styles=APPlibNEW();
	addAPPlib(styles,"Hard Coded");
	addAPPlib(styles,"Utility Master");
	addAPPlib(styles,"Special");
*/
	if(style>=styles->numlibs) style=0;
	addlstrsrc(mtnrsrc,"STYLES",&style,TRUE,changestyles,styles->numlibs,
		&styles->libs,NULL);
	tempinheritrsrcs=(char)inheritrsrcs;
	addstdrsrc(mtnrsrc,"INHERIT RESOURCE VALUES",BOOLNS,1,&tempinheritrsrcs,TRUE);
	dfscnrsrc=RDArsrcNEW("UTILITIES","UTILITY MASTER SELECT SCREEN");
	FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&defdir);
	addstdrsrc(mtnrsrc,"DIRECTORY",VARIABLETEXT,0,defdir,TRUE);
	if(defdir!=NULL) Rfree(defdir);
	addstdrsrc(mtnrsrc,"MODULE",VARIABLETEXT,0,(modulx!=NULL ? modulx:""),TRUE);
	addstdrsrc(mtnrsrc,"NAME",VARIABLETEXT,0,(name!=NULL ? name:""),TRUE);
	addsctrsrc(mtnrsrc,"DESCRIPTION",0,(desc!=NULL ? desc:""),TRUE);
	addstdrsrc(mtnrsrc,"USE AUDIT TRAIL",BOOLNS,1,&Use_Audit_Trail,TRUE);
	FINDRSCSETFUNC(mtnrsrc,"USE AUDIT TRAIL",set_audit_trail,NULL);
	addstdrsrc(mtnrsrc,"AUDIT TRAIL",VARIABLETEXT,0,(Audit_Trail!=NULL ? Audit_Trail:""),TRUE);
	if(ftypes!=NULL) freeapplib(ftypes);
	ftypes=APPlibNEW();
        for(x=0;x<2;++x) addAPPlib(ftypes,filetype_names[x]);
/*
	addAPPlib(ftypes,"Normal File");
	addAPPlib(ftypes,"Special File");
*/
	if(file_type>=ftypes->numlibs) file_type=0;
	addlstrsrc(mtnrsrc,"FILE TYPES",&file_type,TRUE,NULL,ftypes->numlibs,&ftypes->libs,NULL);
	if(starttypes!=NULL) freeapplib(starttypes);
	starttypes=APPlibNEW();
        for(x=0;x<3;++x) addAPPlib(starttypes,startwith_names[x]);
/*
	addAPPlib(starttypes,"Maintain Screen");
	addAPPlib(starttypes,"Browse Screen");
*/
	addbtnrsrc(mtnrsrc,"SELECT MAINFILE",TRUE,getfile,NULL);
	addstdrsrc(mtnrsrc,"MAINFILE",VARIABLETEXT,0,(mainfile!=NULL ? mainfile:""),
		TRUE);
	if(start_with>=starttypes->numlibs) start_with=0;
	addlstrsrc(mtnrsrc,"START TYPES",&start_with,TRUE,NULL,
		starttypes->numlibs,&starttypes->libs,NULL);
	if(startwheres!=NULL) freeapplib(startwheres);
	startwheres=APPlibNEW();
        for(x=0;x<2;++x) addAPPlib(startwheres,startwhere_names[x]);
/*
	addAPPlib(startwheres,"Top of Browse");
	addAPPlib(startwheres,"Bottom of Browse");
*/
	if(startwhere>=startwheres->numlibs) startwhere=0;
	addlstrsrc(mtnrsrc,"START WHERES",&startwhere,TRUE,NULL,
		startwheres->numlibs,&startwheres->libs,NULL);
	addbtnrsrc(mtnrsrc,"SELECT BROWSE SCREEN",TRUE,getscreenbs,dfscnrsrc);
	addstdrsrc(mtnrsrc,"DEFAULT BROWSE SCREEN",BOOLNS,1,&make_default,TRUE);
	addstdrsrc(mtnrsrc,"DEFAULT DEFINE LIST",BOOLNS,1,&make_default,TRUE);
	addstdrsrc(mtnrsrc,"DEFAULT SEARCH BROWSE",BOOLNS,1,&make_default,TRUE);
	addstdrsrc(mtnrsrc,"DEFAULT MAINTAIN SCREEN",BOOLNS,1,&make_default,TRUE);
	addstdrsrc(mtnrsrc,"DEFAULT MENU",BOOLNS,1,&make_default,TRUE);
	addstdrsrc(mtnrsrc,"BROWSE SCREEN",VARIABLETEXT,0,browse_screen,TRUE);
	addbtnrsrc(mtnrsrc,"SELECT DEFINE LIST",TRUE,getscreends,dfscnrsrc);
	addstdrsrc(mtnrsrc,"DEFINE LIST",VARIABLETEXT,0,define_list,TRUE);
	addbtnrsrc(mtnrsrc,"SELECT SEARCH BROWSE",TRUE,getscreensb,dfscnrsrc);
	addstdrsrc(mtnrsrc,"SEARCH BROWSE",VARIABLETEXT,0,search_browse,TRUE);
	addbtnrsrc(mtnrsrc,"SELECT MAINTAIN SCREEN",TRUE,getscreenms,dfscnrsrc);
	addstdrsrc(mtnrsrc,"MAINTAIN SCREEN",VARIABLETEXT,0,maintain_screen,TRUE);
	addbtnrsrc(mtnrsrc,"DFSCN BROWSE SCREEN",TRUE,dfscn_screen,"BROWSE SCREEN");
	addbtnrsrc(mtnrsrc,"DFSCN DEFINE LIST",TRUE,dfscn_screen,"DEFINE LIST");
	addbtnrsrc(mtnrsrc,"DFSCN SEARCH BROWSE",TRUE,dfscn_screen,"SEARCH BROWSE");
	addbtnrsrc(mtnrsrc,"DFSCN MAINTAIN SCREEN",TRUE,dfscn_screen,"MAINTAIN SCREEN");
	makebuttonlist();
	addlstrsrc(mtnrsrc,"BUTTONS AVAILABLE",0,TRUE,NULL,buttonsavl->numlibs,&buttonsavl->libs,NULL);
	addbtnrsrc(mtnrsrc,"ADD BUTTON ABOVE",TRUE,add_button_above,mainrsrc);
	addbtnrsrc(mtnrsrc,"ADD BUTTON BELOW",TRUE,add_button_below,mainrsrc);
	addbtnrsrc(mtnrsrc,"SELECT BUTTON",TRUE,select_button,mainrsrc);
	addbtnrsrc(mtnrsrc,"DELETE BUTTON",TRUE,delete_button,NULL);
	addbtnrsrc(mtnrsrc,"COPY BUTTON",TRUE,copy_button,mainrsrc);
	getsupports();
	addlstrsrc(mtnrsrc,"SUPPORTING FILES",0,TRUE,NULL,suppavlmtn->numlibs,&suppavlmtn->libs,NULL);
	addbtnrsrc(mtnrsrc,"ADD SUPPORTING ABOVE",TRUE,suppaddabove,mtnrsrc);
	addbtnrsrc(mtnrsrc,"ADD SUPPORTING BELOW",TRUE,suppaddbelow,mtnrsrc);
	addbtnrsrc(mtnrsrc,"DELETE SUPPORTING",TRUE,suppdelete,NULL);
	addbtnrsrc(mtnrsrc,"SELECT SUPPORTING",TRUE,suppedit,mtnrsrc);
	addbtnrsrc(mtnrsrc,"COPY SUPPORTING",TRUE,suppcopy,mtnrsrc);
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
	addbtnrsrc(mtnrsrc,"MOVE PREV WRITE UP",TRUE,prevwrtmoveup,NULL);
	addbtnrsrc(mtnrsrc,"MOVE PREV WRITE DOWN",TRUE,prevwrtmovedown,NULL);
	addbtnrsrc(mtnrsrc,"DELETE PREV WRITE",TRUE,prevwrtdelete,NULL);
	addbtnrsrc(mtnrsrc,"COPY PREV WRITE",TRUE,prevwrtcopy,NULL);
	addbtnrsrc(mtnrsrc,"SELECT PREV WRITE",TRUE,prevwrtedit,NULL);
	if(newwrtavl!=NULL) freeapplib(newwrtavl);
	newwrtavl=APPlibNEW();
	getNewWrites();
	addlstrsrc(mtnrsrc,"NEW WRITE LIST",0,TRUE,NULL,newwrtavl->numlibs,&newwrtavl->libs,NULL);
	addbtnrsrc(mtnrsrc,"ADD NEW WRITE ABOVE",TRUE,newwrtaddabove,NULL);
	addbtnrsrc(mtnrsrc,"ADD NEW WRITE BELOW",TRUE,newwrtaddbelow,NULL);
	addbtnrsrc(mtnrsrc,"MOVE NEW WRITE UP",TRUE,newwrtmoveup,NULL);
	addbtnrsrc(mtnrsrc,"MOVE NEW WRITE DOWN",TRUE,newwrtmovedown,NULL);
	addbtnrsrc(mtnrsrc,"DELETE NEW WRITE",TRUE,newwrtdelete,NULL);
	addbtnrsrc(mtnrsrc,"COPY NEW WRITE",TRUE,newwrtcopy,NULL);
	addbtnrsrc(mtnrsrc,"SELECT NEW WRITE",TRUE,newwrtedit,NULL);
	addbtnrsrc(mtnrsrc,"OBTAIN PREVIOUS",TRUE,newgetprev,NULL);
	actstructavl=APPlibNEW();
	getActions();
	addlstrsrc(mtnrsrc,"ACTION LIST",0,TRUE,NULL,actstructavl->numlibs,&actstructavl->libs,NULL);
	addbtnrsrc(mtnrsrc,"ADD ACTION ABOVE",TRUE,actionaddabove,NULL);
	addbtnrsrc(mtnrsrc,"ADD ACTION BELOW",TRUE,actionaddbelow,NULL);
	addbtnrsrc(mtnrsrc,"DELETE ACTION",TRUE,actiondelete,NULL);
	addbtnrsrc(mtnrsrc,"COPY ACTION",TRUE,actioncopy,NULL);
	addbtnrsrc(mtnrsrc,"SELECT ACTION",TRUE,actionedit,NULL);
	
	addbtnrsrc(mtnrsrc,"DFSCN",TRUE,dfscn,NULL);
	addbtnrsrc(mtnrsrc,"DFMENU",TRUE,dfmenu,NULL);
	addbtnrsrc(mtnrsrc,"DFLIST",TRUE,dflist,NULL);
	addbtnrsrc(mtnrsrc,"DFVIR",TRUE,dfvir,NULL);
	addbtnrsrc(mtnrsrc,"DFLOAD",TRUE,dfload,NULL);
/*lint -e611 */
	addbtnrsrc(mtnrsrc,"PRINT DEFINITION",TRUE,printdefinition,(void *)printmtnfp);
/*lint +e611 */
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	mtntdefaults=GetDefaults(mtnrsrc);
	file_defs=APPlibNEW();
	filersrc=RDArsrcNEW("UTILITIES","UTILITY MASTER SELECT FILE");
	x=0;
	addlstrsrc(filersrc,"FILE LIST",&x,TRUE,NULL,file_defs->numlibs,&file_defs->libs,NULL);
	addrfkwrsrc(filersrc,"SELECT",TRUE,okselectfile,mtnrsrc);
	addrfkwrsrc(filersrc,"QUIT",TRUE,NULL_RSCS,NULL);
	addbtnrsrc(filersrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(filersrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	changestyles(mtnrsrc);
	enable_powerwrite(mtnrsrc);
	dir_libs=APPlibNEW();
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(USERLOGIN)+2);
#ifndef WIN32
	sprintf(defdir,"%s/%s",CURRENTDIRECTORY,USERLOGIN);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\%s",CURRENTDIRECTORY,USERLOGIN);
#endif
	addAPPlib(dir_libs,defdir);
	if(defdir!=NULL) Rfree(defdir); 
	if(!isEMPTY(RDA_GROUP))
	{
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(RDA_GROUP)+2);
#ifndef WIN32
	sprintf(defdir,"%s/%s",CURRENTDIRECTORY,RDA_GROUP);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\%s",CURRENTDIRECTORY,RDA_GROUP);
#endif
	addAPPlib(dir_libs,defdir);
	if(defdir!=NULL) Rfree(defdir); 
	}
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(defdir,"%s/site",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\site",CURRENTDIRECTORY);
#endif
	addAPPlib(dir_libs,defdir);
	if(defdir!=NULL) Rfree(defdir);
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+5);
#ifndef WIN32
	sprintf(defdir,"%s/rda",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\rda",CURRENTDIRECTORY);
#endif
	addAPPlib(dir_libs,defdir);
	if(defdir!=NULL) Rfree(defdir);
	addlstrsrc(dfscnrsrc,"DIRECTORY LIST",0,TRUE,getdir,dir_libs->numlibs,&dir_libs->libs,mtnrsrc);
	addstdrsrc(dfscnrsrc,"DIRECTORY",VARIABLETEXT,0,dir_libs->libs[0],TRUE);
	FINDRSCSETFUNC(dfscnrsrc,"DIRECTORY",setdirlist,mtnrsrc);
	scr_defs=APPlibNEW();
	if(defdir!=NULL) Rfree(defdir);
	doliblist(dfscnrsrc,mtnrsrc);
	if(scr_defs->numlibs<1)
	{
		addAPPlib(scr_defs,"Contains No Screen Definitions");
	}
	addlstrsrc(dfscnrsrc,"SCREEN LIST",0,TRUE,NULL,scr_defs->numlibs,&scr_defs->libs,NULL);
	set_audit_trail(mtnrsrc);
	addrfkwrsrc(dfscnrsrc,"SELECT",TRUE,okselectscreen,mtnrsrc);
	addrfkwrsrc(dfscnrsrc,"QUIT",TRUE,NULL_RSCS,NULL);
	addbtnrsrc(dfscnrsrc,"HELP",TRUE,screenhelp,dfscnrsrc);
	addbtnrsrc(dfscnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,dfscnrsrc);
	APPmakescrn(mtnrsrc,TRUE,quitmaintain,mainrsrc,FALSE);
}
static void okmaintain(RDArsrc *mainrsrc)
{
	int selected=0,selectedv=0;
	char *libx=NULL,*directory=NULL,*tmpx=NULL;

	changedvalues=FALSE;changedbtnfld=FALSE;changedbutton=FALSE;
	changedargs=FALSE;
	readwidget(mainrsrc,"DIRECTORY");
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"MAINTAINS AVAILABLE",&selectedv)) return;
	if(MTNMSTR!=NULL) FreeMaintainMaster(MTNMSTR);
	if(RDAstrcmp(mtnavl->libs[selectedv],"No Maintain Master Definitions")) 
	{
		libx=Rmalloc(RDAstrlen(directory)+RDAstrlen(modlstmtn->libs[selected])+6);
		tmpx=adddashes(modlstmtn->libs[selected]);
#ifndef WIN32
		sprintf(libx,"%s/%s.MTN",directory,tmpx);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\%s.MTN",directory,tmpx);
#endif 
		MTNMSTR=MaintainMasterNew(modlstmtn->libs[selected],mtnavl->libs[selectedv]);
		if(tmpx!=NULL) Rfree(tmpx);
		if(getMaintainMaster(libx,MTNMSTR))
		{
			ERRORDIALOG("Cannot Read Maintain Master","Error in permissions, the user cannot read the maintain master definition.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
			if(libx!=NULL) Rfree(libx);
			if(MTNMSTR!=NULL) FreeMaintainMaster(MTNMSTR);
			return;
		}
		if(libx!=NULL) Rfree(libx);
	}
	if(directory!=NULL) Rfree(directory);
	if(MTNMSTR==NULL)
	{
		MTNMSTR=MaintainMasterNew(modlstmtn->libs[selected],NULL);
	}
	maintainscreen(mainrsrc,MTNMSTR->module,MTNMSTR->name,MTNMSTR->desc,
		MTNMSTR->style,MTNMSTR->mainfile,MTNMSTR->browse_screen,
		MTNMSTR->define_list,MTNMSTR->search_browse,MTNMSTR->maintain_screen,
		MTNMSTR->file_type,MTNMSTR->start_with,MTNMSTR->start_where,MTNMSTR->numbuttons,
		MTNMSTR->buttons,MTNMSTR->num,MTNMSTR->s,
		MTNMSTR->subnum,MTNMSTR->subord,MTNMSTR->save_expression,
		MTNMSTR->save_error_desc,MTNMSTR->save_warning,
		MTNMSTR->save_warning_desc,MTNMSTR->inheritrsrcs,
		MTNMSTR->Use_Audit_Trail,MTNMSTR->Audit_Trail,
		MTNMSTR->UsePowerWrite,MTNMSTR->numPrevWrites,
		MTNMSTR->PrevWrites,MTNMSTR->numNewWrites,MTNMSTR->NewWrites,
		MTNMSTR->numActions,MTNMSTR->Actions);
}
static void newmaintain(RDArsrc *mainrsrc)
{
	int selected=0;

	changedvalues=FALSE;changedbtnfld=FALSE;changedbutton=FALSE;
	changedargs=FALSE;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(MTNMSTR!=NULL) FreeMaintainMaster(MTNMSTR);
	MTNMSTR=MaintainMasterNew(modlstmtn->libs[selected],NULL);
	MTNMSTR->style=1;
	MTNMSTR->start_where=1;
	maintainscreen(mainrsrc,MTNMSTR->module,MTNMSTR->name,MTNMSTR->desc,
		MTNMSTR->style,MTNMSTR->mainfile,MTNMSTR->browse_screen,
		MTNMSTR->define_list,MTNMSTR->search_browse,MTNMSTR->maintain_screen,
		MTNMSTR->file_type,MTNMSTR->start_with,MTNMSTR->start_where,MTNMSTR->numbuttons,
		MTNMSTR->buttons,MTNMSTR->num,MTNMSTR->s,
		MTNMSTR->subnum,MTNMSTR->subord,
		MTNMSTR->save_expression,
		MTNMSTR->save_error_desc,MTNMSTR->save_warning,
		MTNMSTR->save_warning_desc,MTNMSTR->inheritrsrcs,
		MTNMSTR->Use_Audit_Trail,MTNMSTR->Audit_Trail,
		MTNMSTR->UsePowerWrite,MTNMSTR->numPrevWrites,
		MTNMSTR->PrevWrites,MTNMSTR->numNewWrites,MTNMSTR->NewWrites,
		MTNMSTR->numActions,MTNMSTR->Actions);
}
static void quitdfmtn(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	if(modlstmtn!=NULL) freeapplib(modlstmtn);
	if(mtnavl!=NULL) freeapplib(mtnavl);
	if(mtntdefaults!=NULL) FreeRDAdefault(mtntdefaults);
	if(diravl!=NULL) freeapplib(diravl);
	if(btntypes!=NULL) freeapplib(btntypes);
	ShutdownSubsystems();
}
static void deletemtn(RDArsrc *mainrsrc)
{
	int s,m=0;
	char *libx=NULL,*modulename=NULL;
	char *directory=NULL;

	readallwidgets(mainrsrc);
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&m)) return;
	if(FINDRSCGETINT(mainrsrc,"MAINTAINS AVAILABLE",&s)) return;
	modulename=adddashes(modlstmtn->libs[m]);	
	libx=Rmalloc(RDAstrlen(directory)+RDAstrlen(modulename)+6);
#ifndef WIN32
	sprintf(libx,"%s/%s.MTN",directory,modulename);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.MTN",directory,modulename);
#endif
	if(deletelibbin(libx,mtnavl->libs[s]))
	{
		ERRORDIALOG("Cannot Delete Maintain Master","The user cannot delete this maintain master.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
	} else doliblist1(mainrsrc);
	if(libx!=NULL) Rfree(libx);
	if(modulename!=NULL) Rfree(modulename);
	if(directory!=NULL) Rfree(directory);
}
static void deletemtntest(RDArsrc *mainrsrc)
{
	int s=0;

	if(!RDAstrcmp(mtnavl->libs[s],"No Maintain Master Definitions")) return;
	WARNINGDIALOG("DELETE WARNING DIALOG SCREEN","DELETE MAINTAIN MASTER!","Are you sure you want to delete this maintain master?",deletemtn,NULL,FALSE,mainrsrc);
}
static void setvaluesx(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;
	char *tmpx=NULL,*temp=NULL,*libx=NULL,*directory=NULL;

	readwidget(mainrsrc,"DIRECTORY");
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	libx=Rmalloc(RDAstrlen(directory)+RDAstrlen(modlstmtn->libs[selectedm])+6);
	tmpx=adddashes(modlstmtn->libs[selectedm]);
#ifndef WIN32
	sprintf(libx,"%s/%s.MTN",directory,tmpx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.MTN",directory,tmpx);
#endif
	if(tmpx!=NULL) Rfree(tmpx);
	if(FINDRSCGETINT(mainrsrc,"MAINTAINS AVAILABLE",&selected)) return;
	if(!RDAstrcmp(mtnavl->libs[selected],"No Maintain Master Definitions"))
	{
		temp=NULL;
	} else {
		temp=getMaintainMasterdesc(libx,mtnavl->libs[selected]);
	}
	if(libx!=NULL) Rfree(libx);
	if(directory!=NULL) Rfree(directory);
	if(!FINDRSCSETSTRING(mainrsrc,"DESCRIPTION",temp))
		updatersrc(mainrsrc,"DESCRIPTION");
	if(temp!=NULL) Rfree(temp);
}
static void printmodule(RDArsrc *mainrsrc,RDA_PFILE *fp)
{
	int x=0,selected=0;
	char *tmpx=NULL,*libx=NULL,*directory=NULL;
	MaintainMaster *d=NULL;
	APPlib *mtntnames=NULL;
/*
	char *temp=NULL;
*/
	int lines=0,pages=0;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	libx=Rmalloc(RDAstrlen(directory)+RDAstrlen(modlstmtn->libs[selected])+6);
	tmpx=adddashes(modlstmtn->libs[selected]);
#ifndef WIN32
	sprintf(libx,"%s/%s.MTN",directory,tmpx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.MTN",directory,tmpx);
#endif
	if(directory!=NULL) Rfree(directory);
	if(!isEMPTY(tmpx)) mtntnames=getlibnames(libx,FALSE);
	if(mtntnames!=NULL)
	{
/*
		temp=stralloc("XPERT MAINTAIN MASTER DEFINITION"); 
		printheader(fp,&lines,&pages,temp,tmpx);
		if(temp!=NULL) Rfree(temp);
*/
		for(x=0;x<mtntnames->numlibs;++x)
		{
			d=MaintainMasterNew(modlstmtn->libs[selected],mtntnames->libs[x]);
			if(!getMaintainMaster(libx,d))
			{
				printmtn1(fp,d,&lines,&pages);
				RDA_fprintf(fp,"\f\r\n");
			}
			if(d!=NULL) FreeMaintainMaster(d);
		}
	} else {
		prterr("Error No Maintain Master Available.");
	}
	if(libx!=NULL) Rfree(libx);
	if(tmpx!=NULL) Rfree(tmpx);
}
static void doliblist1(RDArsrc *mainrsrc)
{
	int selected=0,selm=0;
	char *libx=NULL;
	char *dirx=NULL;

	readwidget(mainrsrc,"DIRECTORY");
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirx)) return;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"MAINTAINS AVAILABLE",&selm)) return;
	if(mtnavl!=NULL) freeapplib(mtnavl);
	libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(modlstmtn->libs[selected])+6);
#ifndef WIN32
	sprintf(libx,"%s/%s.MTN",dirx,modlstmtn->libs[selected]);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.MTN",dirx,modlstmtn->libs[selected]);
#endif
	if(!isEMPTY(modlstmtn->libs[selected])) mtnavl=getlibnames(libx,FALSE);
	if(dirx!=NULL) Rfree(dirx);
	if(libx!=NULL) Rfree(libx);
	if(mtnavl==NULL)
	{
		mtnavl=APPlibNEW();
		addAPPlib(mtnavl,"No Maintain Master Definitions");
	}
	if(selm>=mtnavl->numlibs) selm=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"MAINTAINS AVAILABLE",selm,mtnavl))
		updatersrc(mainrsrc,"MAINTAINS AVAILABLE");
	setvaluesx(mainrsrc);
}
static void getdir1(RDArsrc *mainrsrc)
{
	int s=0;
	char *dirstr=NULL;

	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirstr)) return;
	if(FINDRSCGETINT(mainrsrc,"DIRECTORY LIST",&s)) return;
	if(RDAstrcmp(dirstr,diravl->libs[s]))
	{
		if(FINDRSCSETSTRING(mainrsrc,"DIRECTORY",diravl->libs[s])) return;
		updatersrc(mainrsrc,"DIRECTORY");
	}
	if(dirstr!=NULL) Rfree(dirstr);
	getscreenlist(mainrsrc);
}
static void setdirlist1(RDArsrc *mainrsrc)
{
	int x=0;
	char *directory=NULL;

	readwidget(mainrsrc,"DIRECTORY");
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	for(x=0;x<diravl->numlibs;++x)
	{
		if(!RDAstrcmp(diravl->libs[x],directory))
			break;
	}
	if(x>=diravl->numlibs)
	{
		addAPPlib(diravl,directory);
		x=diravl->numlibs-1;
	}
	if(directory!=NULL) Rfree(directory);
	FINDRSCLISTAPPlib(mainrsrc,"DIRECTORY LIST",x,diravl);
	updatersrc(mainrsrc,"DIRECTORY LIST");
	getscreenlist(mainrsrc);
}
static void getscreenlist(RDArsrc *mainrsrc)
{
	int x,selectedx=0;
	char *tmp=NULL,*directory=NULL,*dirx=NULL;

	readwidget(mainrsrc,"DIRECTORY");
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedx)) selectedx=0;
	if(modlstmtn!=NULL) freeapplib(modlstmtn);
	modlstmtn=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(directory)+2);
#ifndef WIN32
	sprintf(dirx,"%s/",directory);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\",directory);
#endif
	for(x=0;findfile(dirx,"*.MTN",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(modlstmtn,tmp);
	}
	if(modlstmtn->numlibs<1)
	{
		addAPPlib(modlstmtn,"Directory Contains No Libraries");
	} else {
		SORTAPPlib(modlstmtn);
	}
	if(tmp!=NULL) Rfree(tmp);
	if(directory!=NULL) Rfree(directory);
	if(dirx!=NULL) Rfree(dirx);
	if(selectedx>=modlstmtn->numlibs) selectedx=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"MODULE LIST",selectedx,modlstmtn))
		updatersrc(mainrsrc,"MODULE LIST");
	doliblist1(mainrsrc);
}
static void domtnmst(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;
	APPlib *args=NULL;

	if(mtnavl->numlibs==1 && !RDAstrcmp(mtnavl->libs[0],"No Maintain Master Definitions")) return; 
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(mainrsrc,"MAINTAINS AVAILABLE",&selected)) return;
	args=APPlibNEW();
	addAPPlib(args,modlstmtn->libs[selectedm]);
	addAPPlib(args,mtnavl->libs[selected]);
	ExecuteProgram("mtnmst",args);
	if(args!=NULL) freeapplib(args);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0,w=2,m=0,s=0;
	RDArsrc *mainrsrc=NULL;
	char *defdir=NULL;

	if(InitializeSubsystems(argc,argv,module,"DEFINE MAINTAIN MASTERS")) 
	{
		return;
	}
	DoHeaders();
	mainrsrc=RDArsrcNEW(module,"DEFINE MAINTAIN MASTERS");
	diravl=APPlibNEW();
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(USERLOGIN)+2);
#ifndef WIN32
	sprintf(defdir,"%s/%s",CURRENTDIRECTORY,USERLOGIN);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\%s",CURRENTDIRECTORY,USERLOGIN);
#endif
	addAPPlib(diravl,defdir);
	if(defdir!=NULL) Rfree(defdir);
	if(!isEMPTY(RDA_GROUP))
	{
	w=3;
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(RDA_GROUP)+2);
#ifndef WIN32
	sprintf(defdir,"%s/%s",CURRENTDIRECTORY,RDA_GROUP);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\%s",CURRENTDIRECTORY,RDA_GROUP);
#endif
	addAPPlib(diravl,defdir);
	if(defdir!=NULL) Rfree(defdir);
	}
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(defdir,"%s/site",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\site",CURRENTDIRECTORY);
#endif
	addAPPlib(diravl,defdir);
	if(defdir!=NULL) Rfree(defdir); 
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+5);
#ifndef WIN32
	sprintf(defdir,"%s/rda",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\rda",CURRENTDIRECTORY);
#endif
	addAPPlib(diravl,defdir);
	if(defdir!=NULL) Rfree(defdir);
	addlstrsrc(mainrsrc,"DIRECTORY LIST",&w,TRUE,getdir1,diravl->numlibs,&diravl->libs,NULL);
	addstdrsrc(mainrsrc,"DIRECTORY",VARIABLETEXT,0,diravl->libs[w],TRUE);
	if(defdir!=NULL) Rfree(defdir);
	FINDRSCLISTAPPlib(mainrsrc,"DIRECTORY LIST",w,diravl);
	FINDRSCSETFUNC(mainrsrc,"DIRECTORY",setdirlist1,NULL);
	x=0;
	if(modlstmtn!=NULL) freeapplib(modlstmtn); 
	modlstmtn=APPlibNEW();
	addlstrsrc(mainrsrc,"MODULE LIST",&x,TRUE,doliblist1,
		modlstmtn->numlibs,&modlstmtn->libs,NULL);
	x=0;
	if(mtnavl!=NULL) freeapplib(mtnavl); 
	mtnavl=APPlibNEW();
	addlstrsrc(mainrsrc,"MAINTAINS AVAILABLE",&x,TRUE,setvaluesx,
		mtnavl->numlibs,&mtnavl->libs,NULL);
	setdirlist1(mainrsrc);
	if(argc>1 && modlstmtn!=NULL)
	{
		for(m=0;m<modlstmtn->numlibs;++m)
		{
			if(!RDAstrcmp(modlstmtn->libs[m],argv[1])) break;
		}
		if(m<modlstmtn->numlibs)
		{
			FINDRSCSETINT(mainrsrc,"MODULE LIST",m);
		}
		else
		{
			m=0;
			FINDRSCSETINT(mainrsrc,"MODULE LIST",m);
		}
		doliblist1(mainrsrc);
	}
	if(argc>2 && mtnavl!=NULL)
	{
		for(s=0;s<mtnavl->numlibs;++s)
		{
			if(!RDAstrcmp(mtnavl->libs[s],argv[2])) break;
		}
		if(s<mtnavl->numlibs)
		{
			FINDRSCSETINT(mainrsrc,"MAINTAINS AVAILABLE",s);
		}
		else
		{
			s=0;
			FINDRSCSETINT(mainrsrc,"MAINTAINS AVAILABLE",s);
		}
		setvaluesx(mainrsrc);
	}
	addsctrsrc(mainrsrc,"DESCRIPTION",0,NULL,FALSE);
	addbtnrsrc(mainrsrc,"RUN MAINTAIN MASTER",TRUE,domtnmst,NULL);
	addbtnrsrc(mainrsrc,"ADD",TRUE,newmaintain,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,okmaintain,NULL);
/*lint -e611 */
	addbtnrsrc(mainrsrc,"PRINT",TRUE,printdefinition,(void *)printmodulefp);
/*lint +e611 */
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitdfmtn,NULL);
	addbtnrsrc(mainrsrc,"DELETE",TRUE,deletemtntest,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	btntypes=makebtntypeslist();
	FINDRSCSETINPUTFOCUS(mainrsrc,"MODULE LIST");
	APPmakescrn(mainrsrc,TRUE,quitdfmtn,NULL,TRUE);
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
	return(tmp);
}
