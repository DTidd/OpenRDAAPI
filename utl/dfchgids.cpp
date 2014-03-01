/* dfchgids.c - Define Change File Fields*/
#ifndef WIN32
#define __NAM__ "dfchgids.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dfchgids.exe"
#endif
#include <app.hpp>





#include <mix.hpp>
#include <ldval.hpp>
#include <doheader.hpp>
/*CPP_OPART dfmtnsup */
/*CPP_OPART dfmtnsub */
/*CPP_OPART doheader */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="UTILITIES";
RDArsrc *mainrsrc=NULL,*mtnrsrc=NULL,*dfscnrsrc=NULL,*filersrc=NULL;
APPlib *modlst=NULL,*chgfldavl=NULL,*styles=NULL,*ftypes=NULL,*starttypes=NULL;
APPlib *dir_libs=NULL,*scr_defs=NULL,*file_defs=NULL,*diravl=NULL;
APPlib *buttonsavl=NULL,*fieldsavl=NULL,*btnfldavl=NULL,*keysavl=NULL;
APPlib *filelist=NULL,*tmpargs=NULL,*modlstx=NULL;
ChangeField *MTNMSTR=NULL;
static void savemaintain(RDArsrc *,RDArsrc *);
static void quitmaintaintest(RDArsrc *,RDArsrc *),setvaluesx(RDArsrc *);
static void quitmaintain(RDArsrc *,RDArsrc *);
static void printmtnfp(RDArsrc *,RDArsrc *);
static void printmtn(RDArsrc *,FILE *);
static void fixmtnoutput(FILE *,char *,int *,int *,char *);
static void checkheader(FILE *,int *,int *,char *);
static void printheader(FILE *,int *,int *,char *,char *);
static void printdefinition(RDArsrc *,void (*)(...));
static void printfp(RDArsrc *,void (*)(...),RDArsrc *);
static void printmodulefp(RDArsrc *prsrc,RDArsrc *rsrc);
static void printmodule(RDArsrc *,FILE *);
static RDAdefault *mtntdefaults=NULL,*buttondefaults=NULL;
static RDAdefault *btnflddefaults=NULL,*argdefaults=NULL;
static char changedargs=FALSE,changedbtnfld=FALSE,changedbutton=FALSE;
char changedvalues=FALSE;
static char *snames[4]={"BROWSE SCREEN","DEFINE LIST","SEARCH BROWSE","MAINTAIN SCREEN"}; 
static int which_screen=0,numHoldButtons=0,numHoldButtonField=0;
static char *style_names[3]={"Hard Coded","Utility Master","Special"};
static char *filetype_names[2]={"Normal File","Special File"};
static char *startwith_names[2]={"Maintain Screen","Browse Screen"};
static void doliblist1(RDArsrc *),getscreenlist(RDArsrc *);
static MaintainButton *HoldButtons=NULL;
static ButtonField *HoldButtonField=NULL;
static void dochgfld(RDArsrc *);
RDAsearch *tempsupport=NULL;
int tmpnumsupp=0;
RDAsubord *tempsubord=NULL;
int tmpnumsub=0;
APPlib *suppavl=NULL,*subavl=NULL,*ctypes=NULL;
APPlib *filelst=NULL,*cmodlst=NULL,*subkeylist=NULL,*cnflds=NULL;
APPlib *econflds=NULL;
char *contype[3]={
	"Equal To and Check on Save",
	"Equal To Only",
	"Include File Only",
};
extern void savesupp(RDArsrc *),suppedit(RDArsrc *,RDArsrc *),getsupports(void);
extern void suppaddbelow(RDArsrc *,RDArsrc *),suppaddabove(RDArsrc *,RDArsrc *);
extern void suppdelete(RDArsrc *);
extern void savesub(RDArsrc *),subedit(RDArsrc *,RDArsrc *),getsubords(void);
extern void subaddbelow(RDArsrc *,RDArsrc *),subaddabove(RDArsrc *,RDArsrc *);
extern void subdelete(RDArsrc *);
static void makebuttonlist(void);
static void makebtnfldlist(void);
static void makespacebuttons(int);
APPlib *makefieldvallist2(RDAsearch *,int,int,short,char *,char *);

APPlib *makefieldvallist2(RDAsearch *search,int numsearchs,int number,short dofields,char *submodname,char *subfilename)
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
static void addholdvalues(APPlib *tmp,char *modulex,char *filex)
{
	NRDfield *f,*fields=NULL;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short z,engine=0,nofields=0,nokeys=0;
	int y;
	char *temp=NULL;

	if(diagrptgen)
	{
		prterr("DIAG addholdvalues Module [%s] File [%s] ",modulex,filex);
	}
	if(!GETBIN(modulex,filex,&engine,&nofields,&fields,
		&nokeys,&keys)) 
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
							RDAstrlen(modulex)+
							RDAstrlen(filex)+7);
					} else 	{
						temp=Rmalloc(RDAstrlen(f->name)+RDAstrlen(modulex)+
							RDAstrlen(filex)+7);
					}
					sprintf(temp,"[%s][%s][%s]",modulex,
						filex,f->name);
					addAPPlib(tmp,temp);
					if(f->name!=NULL) Rfree(f->name);
				}
				Rfree(fields);
			}
		}
	}
	if(temp!=NULL) Rfree(temp);
}
static APPlib *makefieldvallist()
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
	sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,MTNMSTR->module);
	virflist=getlibnames(libx,FALSE);
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
	if(tmprsrc!=NULL) free_rsrc(tmprsrc);
	return(tmp);
}
static void loadvalue(RDArsrc *r,char *fieldname)
{
	int x;
	LoadValue *load;

	ldvalrsrc=RDArsrcNEW("RPTGEN","LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	loadlist=makefieldvallist();
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

	if(modlstx!=NULL) freeapplib(modlstx);
	modlstx=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
	for(x=0;findfile(dirx,"*.FIL",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(modlstx,tmp);
	}
	if(modlstx->numlibs<1)
	{
		addAPPlib(modlstx,"No Modules Available");
	} else {
		SORTAPPlib(modlstx);
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
	sprintf(libname,"%s/rda/%s.FIL",CURRENTDIRECTORY,modx);
	file_defs=getlibnames(libname,TRUE);
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
	sprintf(libx,"%s/%s.SCN",dirx,modulx);
	scr_defs=getlibnames(libx,FALSE);
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
	RDAconfld *confld1;
	RDAsearch *tempsupp;
	RDAsubord *tempsub;

	if(suppavl!=NULL) freeapplib(suppavl);
	if(subavl!=NULL) freeapplib(subavl);
	if(ctypes!=NULL) freeapplib(ctypes);
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
				for(z=0,confld1=tempsupp->confld;z<tempsupp->num;
					++z,++confld1)
				{
					if(confld1->fldname!=NULL) Rfree(confld1->fldname);
					if(confld1->conname!=NULL) Rfree(confld1->conname);
				}
				Rfree(tempsupp->confld);
			}
			if(tempsupp->subkeyname!=NULL) Rfree(tempsupp->subkeyname);
			if(tempsupp->expression!=NULL) Rfree(tempsupp->expression);
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
				for(z=0,confld1=tempsub->confld;z<tempsub->num;
					++z,++confld1)
				{
					if(confld1->fldname!=NULL) Rfree(confld1->fldname);
					if(confld1->conname!=NULL) Rfree(confld1->conname);
				}
				Rfree(tempsub->confld);
			}
			if(tempsub->subkeyname!=NULL) Rfree(tempsub->subkeyname);
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
	if(styles!=NULL) freeapplib(styles);
	if(ftypes!=NULL) freeapplib(ftypes);
	if(starttypes!=NULL) freeapplib(starttypes);
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
	char inheritrsrcs=FALSE;
	int x,y;
	MaintainButton *b1,*b2;
	ButtonField *bf1,*bf2;

	readallwidgets(mtnrsrc);
	if(FINDRSCGETSTRING(mtnrsrc,"DIRECTORY",&directory)) return;
	FINDRSCGETSTRING(mtnrsrc,"MODULE",&MTNMSTR->module);
	libname=Rmalloc(RDAstrlen(directory)+RDAstrlen(MTNMSTR->module)+6);
	sprintf(libname,"%s/%s.MTN",directory,MTNMSTR->module);
	FINDRSCGETSTRING(mtnrsrc,"NAME",&MTNMSTR->name);
	if(!isEMPTY(MTNMSTR->name))
	{
		FINDRSCGETSTRING(mtnrsrc,"DESCRIPTION",&MTNMSTR->desc);
		FINDRSCGETINT(mtnrsrc,"STYLES",&MTNMSTR->style);
		FINDRSCGETSTRING(mtnrsrc,"MAINFILE",&MTNMSTR->mainfile);
		FINDRSCGETSTRING(mtnrsrc,"BROWSE SCREEN",&MTNMSTR->browse_screen);
		FINDRSCGETSTRING(mtnrsrc,"DEFINE LIST",&MTNMSTR->define_list);
		FINDRSCGETSTRING(mtnrsrc,"SEARCH BROWSE",&MTNMSTR->search_browse);
		FINDRSCGETSTRING(mtnrsrc,"MAINTAIN SCREEN",&MTNMSTR->maintain_screen);
		FINDRSCGETINT(mtnrsrc,"FILE TYPES",&MTNMSTR->file_type);
		FINDRSCGETINT(mtnrsrc,"START TYPES",&MTNMSTR->start_with);
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
				if(b1->args!=NULL)
				{
					b2->args=APPlibNEW();
					for(y=0;y<b1->args->numlibs;++y) 
						addAPPlib(b2->args,b1->args->libs[y]);
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
		savesupp(mtnrsrc);
		savesub(mtnrsrc);
		if(writeChangeField(libname,MTNMSTR))
		{
			ERRORDIALOG("Cannot Write Maintain Master","The user cannot write the Maintain Master Definition.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
		}
		FreeChangeField(MTNMSTR);
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
	if(modlstx!=NULL) freeapplib(modlstx);
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
	button->modulename=stralloc(modlstx->libs[selmod]);
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
static void changemodlist(RDArsrc *mainrsrc)
{
	int selected=0,selectedf=0;
	char *libx=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	FINDRSCGETINT(mainrsrc,"FILE LIST",&selectedf);
	if(filelist!=NULL) { freeapplib(filelist); filelist=NULL; }
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlstx->libs[selected])+10);
	sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modlstx->libs[selected]);
	filelist=getlibnames(libx,TRUE);
	if(libx!=NULL) Rfree(libx);
	if(filelist==NULL)
	{
		filelist=APPlibNEW();
		addAPPlib(filelist,"No Files Defined");
	}
	if(selectedf>=filelist->numlibs) selectedf=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"FILE LIST",selectedf,filelist))
		updatersrc(mainrsrc,"FILE LIST");
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
	if(keysavl!=NULL) { freeapplib(keysavl); keysavl=NULL; }
	keysavl=APPlibNEW();
	if(!GETBIN(modlstx->libs[selected],filelist->libs[selectedf],&engine,
		&nofields,&fieldx,&nokeys,&keyx))
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
	if(keysavl->numlibs<1)
	{
		addAPPlib(keysavl,"No Keys Defined");
	}
	if(selectedk>=keysavl->numlibs) selectedk=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"KEYS AVAILABLE",selectedk,keysavl))
		updatersrc(mainrsrc,"KEYS AVAILABLE");
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

	changedargs=FALSE;
	argrsrc=RDArsrcNEW(module,"MAINTAIN ARGUMENT SCREEN");
	addstdrsrc(argrsrc,"ARGUMENT",VARIABLETEXT,0,argument,TRUE);
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
	char *progname,char *listname,char *buttonname,int num,ButtonField *bf,
	void (*savefunc)(...),void (*quitfunc)(...),char *modulename,char *filename,
	char *keyname,APPlib *args)
{
	RDArsrc *buttonrsrc=NULL;
	int x,y;
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
				addAPPlib(fieldsavl,field->name);
				if(field->name!=NULL) Rfree(field->name);
			}
			Rfree(fieldx);
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
	makebtnfldlist();
	addlstrsrc(buttonrsrc,"FIELDS AVAILABLE",0,TRUE,NULL,btnfldavl->numlibs,
		&btnfldavl->libs,NULL);
	makemodlistx();
	for(x=0;x<modlstx->numlibs;++x)
	{
		if(!RDAstrcmp(modlstx->libs[x],modulename)) break;
	} 
	if(x>=modlstx->numlibs) x=0;
	addlstrsrc(buttonrsrc,"MODULE LIST",&x,TRUE,changemodlist,modlstx->numlibs,
		&modlstx->libs,NULL);
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
		FINDRSCSETINT(buttonrsrc,"FILE LIST",x);
	keysavl=APPlibNEW();
	addlstrsrc(buttonrsrc,"KEYS AVAILABLE",&x,TRUE,NULL,keysavl->numlibs,
		&keysavl->libs,NULL);
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
	addlstrsrc(buttonrsrc,"ARGUMENT LIST",&x,TRUE,NULL,tmpargs->numlibs,
		&tmpargs->libs,NULL);
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
				RDAstrlen(button->buttonname)+64;
			s=Rmalloc(l);
			sprintf(s,"Program: [%s] Module: [%s] File: [%s] Keyname: [%s] List: [%s] Button: [%s]",
				(button->progname!=NULL ? button->progname:""),
				(button->modulename!=NULL ? button->modulename:""),
				(button->filename!=NULL ? button->filename:""),
				(button->keyname!=NULL ? button->keyname:""),
				(button->listname!=NULL ? button->listname:""),
				(button->buttonname!=NULL ? button->buttonname:""));
			addAPPlib(buttonsavl,s);
			Rfree(s);
		}
	}
}
static void add_button_above(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	int x;

	if(FINDRSCGETINT(mtnrsrc,"BUTTONS AVAILABLE",&x)) return;
	if(!RDAstrcmp(buttonsavl->libs[x],"No Buttons Available"))
	{
		buttonscreen(mtnrsrc,mainrsrc,x,NULL,NULL,NULL,0,NULL,save_button,
			quitbuttontest,NULL,NULL,NULL,NULL);
	} else {
		buttonscreen(mtnrsrc,mainrsrc,x,NULL,NULL,NULL,0,NULL,
			save_button_above,quitbuttonabovetest,NULL,NULL,NULL,NULL);
	}
}
static void add_button_below(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	int x;

	if(FINDRSCGETINT(mtnrsrc,"BUTTONS AVAILABLE",&x)) return;
	if(!RDAstrcmp(buttonsavl->libs[x],"No Buttons Available"))
	{
		buttonscreen(mtnrsrc,mainrsrc,x,NULL,NULL,NULL,0,NULL,save_button,
			quitbuttontest,NULL,NULL,NULL,NULL);
	} else {
		buttonscreen(mtnrsrc,mainrsrc,++x,NULL,NULL,NULL,0,NULL,
			save_button_below,quitbuttonbelowtest,NULL,NULL,NULL,NULL);
	}
}
static void select_button(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	int x;
	MaintainButton *b=NULL;

	if(FINDRSCGETINT(mtnrsrc,"BUTTONS AVAILABLE",&x)) return;
	if(!RDAstrcmp(buttonsavl->libs[x],"No Buttons Available"))
	{
		buttonscreen(mtnrsrc,mainrsrc,x,NULL,NULL,NULL,0,NULL,save_button,
			quitbuttontest,NULL,NULL,NULL,NULL);
	} else {
		b=HoldButtons+x;
		buttonscreen(mtnrsrc,mainrsrc,x,b->progname,b->listname,
			b->buttonname,b->num,b->bf,save_button,quitbuttontest,
			b->modulename,b->filename,b->keyname,b->args);
	}
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
				++y;
			} else {
				if(temp->progname!=NULL) Rfree(temp->progname);
				if(temp->modulename!=NULL) Rfree(temp->modulename);
				if(temp->filename!=NULL) Rfree(temp->filename);
				if(temp->keyname!=NULL) Rfree(temp->keyname);
				if(temp->listname!=NULL) Rfree(temp->listname);
				if(temp->buttonname!=NULL) Rfree(temp->buttonname);
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
static void fixmtnoutput(FILE *fp,char *data,int *lines,int *pages,char *modname)
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
			fprintf(fp,"%.*s\n",x+1,data);
			++*lines;
			fixmtnoutput(fp,data+x+1,lines,pages,modname);
		} else {
			fprintf(fp,"%.*s\n",length,data);
			++*lines;
			if(temp!=NULL || temp1!=NULL) fixmtnoutput(fp,data+length+1,lines,pages,modname);
		}
	} 
}
static void printmtn1(FILE *fp,ChangeField *d,int *lines,int *pages)
{
	int y,z;
	MaintainButton *button;
	ButtonField *btnfld;
	RDAsearch *tempsupport;
	RDAsubord *tempsubord;
	RDAconfld *tempconfld;
	char *temp=NULL;

	temp=Rmalloc(26+(d->name!=NULL?RDAstrlen(d->name):0)+(d->desc!=NULL?RDAstrlen(d->desc):0));
	sprintf(temp,"Name: [%s] Description: [%s]\n",(d->name!=NULL ? d->name:""),(d->desc!=NULL ? d->desc:""));
	fixmtnoutput(fp,temp,lines,pages,d->module);
	if(temp!=NULL) Rfree(temp);
	fprintf(fp,"   Style: [%d] [%s] Main File: [%s]\n",d->style,style_names[d->style],d->mainfile);
	++*lines;
	checkheader(fp,lines,pages,d->module);
	fprintf(fp,"   File Type: [%d] [%s] Start: [%d] [%s]\n",d->file_type,filetype_names[d->file_type],d->start_with,startwith_names[d->start_with]);
	++*lines;
	checkheader(fp,lines,pages,d->module);
	fprintf(fp,"   Inherit Resource Values: [%d] [%s]\n",d->inheritrsrcs,(d->inheritrsrcs==TRUE?"TRUE":"FALSE"));
	++*lines;
	checkheader(fp,lines,pages,d->module);
	fprintf(fp,"   Browse Screen: [%s]\n",(d->browse_screen!=NULL ? d->browse_screen:""));
	++*lines;
	checkheader(fp,lines,pages,d->module);
	fprintf(fp,"   Define List: [%s]\n",(d->define_list!=NULL ? d->define_list:""));
	++*lines;
	checkheader(fp,lines,pages,d->module);
	fprintf(fp,"   Search Browse: [%s]\n",(d->search_browse!=NULL ? d->search_browse:""));
	++*lines;
	checkheader(fp,lines,pages,d->module);
	fprintf(fp,"   Maintain Screen: [%s]\n",(d->maintain_screen!=NULL ? d->maintain_screen:""));
	++*lines;
	checkheader(fp,lines,pages,d->module);
	if(d->s!=NULL)
	{
		for(y=0,tempsupport=d->s;y<d->num;++y,++tempsupport)
		{
			fprintf(fp,"   Module [%s]  Filename [%s]\n",
				(tempsupport->module!=NULL?tempsupport->module:""),
				(tempsupport->filename!=NULL?tempsupport->filename:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			fprintf(fp,"      Key Name [%s]  Occurence [%d]\n",
				(tempsupport->subkeyname!=NULL?tempsupport->subkeyname:""),
				tempsupport->occurrence);
			++*lines;
			checkheader(fp,lines,pages,d->module);
			fprintf(fp,"     Connect Type [%d]:[%s]\n",
				tempsupport->ctype,contype[tempsupport->ctype]);
			++*lines;
			checkheader(fp,lines,pages,d->module);
			fprintf(fp,"     Connect Module [%s]  Filename [%s]\n",
				(tempsupport->cmodule!=NULL?tempsupport->cmodule:""),
				(tempsupport->cfilename!=NULL?tempsupport->cfilename:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			fprintf(fp,"     Expression [%s]\n",
				(tempsupport->expression!=NULL?tempsupport->expression:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			for(z=0,tempconfld=tempsupport->confld;z<tempsupport->num;++z,++tempconfld)
			{
				fprintf(fp,"      Fieldname [%s]  Connectname [%s]\n",
					(tempconfld->fldname!=NULL?tempconfld->fldname:""),
					(tempconfld->conname!=NULL?tempconfld->conname:""));
				++*lines;
				checkheader(fp,lines,pages,d->module);
			}
		}
	}
	if(d->subord!=NULL)
	{
		for(y=0,tempsubord=d->subord;y<d->subnum;++y,++tempsubord)
		{
			fprintf(fp,"   Module [%s]  Filename [%s]  Key Name [%s]\n",
				(tempsubord->module!=NULL?tempsubord->module:""),
				(tempsubord->filename!=NULL?tempsubord->filename:""),
				(tempsubord->subkeyname!=NULL?tempsubord->subkeyname:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			fprintf(fp,"     Connect Module [%s]  Filename [%s]\n",
				(tempsubord->cmodule!=NULL?tempsubord->cmodule:""),
				(tempsubord->cfilename!=NULL?tempsubord->cfilename:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			fprintf(fp,"     Expression [%s]\n",
				(tempsubord->expression!=NULL?tempsubord->expression:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			for(z=0,tempconfld=tempsubord->confld;z<tempsubord->num;++z,++tempconfld)
			{
				fprintf(fp,"      Fieldname [%s]  Connectname [%s]\n",
					(tempconfld->fldname!=NULL?tempconfld->fldname:""),
					(tempconfld->conname!=NULL?tempconfld->conname:""));
				++*lines;
				checkheader(fp,lines,pages,d->module);
			}
		}
	}
	if(d->buttons!=NULL)
	{
		for(y=0,button=d->buttons;y<d->numbuttons;++y,++button)
		{
			fprintf(fp,"   Program [%s]\n",
				(button->progname!=NULL ? button->progname:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			fprintf(fp,"      List Name: [%s]\n",
				(button->listname!=NULL ? button->listname:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			fprintf(fp,"      Button Name: [%s]\n",
				(button->buttonname!=NULL ? button->buttonname:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			fprintf(fp,"      Module [%s] Filename: [%s] Keyname: [%s]\n",
				(button->modulename!=NULL ? button->modulename:""),
				(button->filename!=NULL ? button->filename:""),
				(button->keyname!=NULL ? button->keyname:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			if(button->bf!=NULL)
			{
				for(z=0,btnfld=button->bf;z<button->num;++z,++btnfld)
				{
					fprintf(fp,"      Field Name: [%s]\n",
						(btnfld->rname!=NULL ? btnfld->rname:""));
					++*lines;
					checkheader(fp,lines,pages,d->module);
					fprintf(fp,"      Environment Name: [%s]\n",
						(btnfld->ename!=NULL ? btnfld->ename:""));
					++*lines;
					checkheader(fp,lines,pages,d->module);
				}
			}
		}
	}
	fprintf(fp,"\n");
	++*lines;
}
static void printmtn(RDArsrc *rsrc,FILE *fp)
{
	int lines=0,pages=0;
	char *temp=NULL;

	temp=stralloc("XPERT MAINTAIN MASTER DEFINITION"); 
	printheader(fp,&lines,&pages,temp,MTNMSTR->module);
	if(temp!=NULL) Rfree(temp);
	printmtn1(fp,MTNMSTR,&lines,&pages);
	fprintf(fp,"\f\n");
}
static void quit_print_definition(RDArsrc *prsrc)
{
	if(prsrc!=NULL) Rfree(prsrc);
}
static void checkheader(FILE *fp,int *lines,int *pages,char *modname)
{
	char *temp=NULL;

	if(doheaders)
	{
		if(*lines>57)
		{
			fprintf(fp,"\f");
			temp=stralloc("XPERT MAINTAIN MASTER DEFINITION"); 
			printheader(fp,lines,pages,temp,modname);
			if(temp!=NULL) Rfree(temp);
		}
	}
}
static void printheader(FILE *fp,int *lines,int *pages,char *header,char *modname)
{
	char *date=NULL,*timex=NULL;
	int lead_ws=0,tail_ws=0;
	unsigned line_length=79;

	date=GETCURRENTDATE();
	timex=GETCURRENTTIME();
	fprintf(fp,"\n");
	*lines=1;
	lead_ws=(int)(RDAroundup((double)(line_length-(2+RDAstrlen(header)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(2+RDAstrlen(header)))/2)-14));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(header)+2+14+lead_ws+tail_ws)>line_length) header[(line_length-14-lead_ws-tail_ws)]=0;
	fprintf(fp,"%*s %s %*sPage: %*d\n",
		lead_ws,"",header,tail_ws,"",8,++*pages);
	++*lines;
	lead_ws=(int)(RDAroundup((double)(line_length-(10+RDAstrlen(modname)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(10+RDAstrlen(modname)))/2)-14));
	if(tail_ws<0) tail_ws=0;
	fprintf(fp,"%*s Module: %s %*sDate: %s\n",
		lead_ws,"",(modname!=NULL?modname:""),tail_ws,"",date);
	++*lines;
	lead_ws=(int)(RDAroundup(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)-14));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(USERLOGIN)+14+15+lead_ws+tail_ws)>line_length)
	{
		fprintf(fp,"%*s %s %*sTime: %s\n",
			(int)(RDAroundup(((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2))),"",
			USERLOGIN,
			(int)(Rint((((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2)-14))),"",
			timex);
		++*lines;
	} else {
		fprintf(fp,"%*s Executed By: %s %*sTime: %s\n",
			lead_ws,"",USERLOGIN,tail_ws,"",timex);
		++*lines;
	}
	fprintf(fp,"\n");
	++*lines;
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
}
static void printdefinition(RDArsrc *mtn,void (*printfunc)(...))
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL;

	prsrc=RDArsrcNEW(module,"PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
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
	char *outdevice=NULL,*temp=NULL;
	FILE *fp=NULL;

	readwidget(prsrc,"DEVICE");
        FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
	if(outdevice!=NULL)
	{
		fp=popen(outdevice,"w");
		if(fp!=NULL)
		{
/*lint -e746 */
			pfunc(rsrc,fp);
/*lint +e746 */
			pclose(fp);
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
static void maintainscreen(RDArsrc *mainrsrc,char *modulx,char *name,
	char *desc,int style,char *mainfile,char *browse_screen,
	char *define_list,char *search_browse,char *maintain_screen,
	int file_type,int start_with,int numbuttons,MaintainButton *buttons,
	int numsupport,RDAsearch *s,int numsubord,RDAsubord *sub,
	char *save_e,char *save_ed,char *save_w,char *save_wd,short inheritrsrcs)
{
	int x=0,z=0;
	RDArsrc *mtnrsrc=NULL;
	char *defdir=NULL;
	MaintainButton *b1,*b2;
	ButtonField *bf1,*bf2;
	RDAsearch *tempsupp1,*tempsupp;
	RDAsubord *tempsub1,*tempsub;
	RDAconfld *confld1,*confld2;
	char tempinheritrsrcs=FALSE;

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
		tempsubord=Rmalloc(tmpnumsub*sizeof(RDAsearch));
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
	addstdrsrc(mtnrsrc,"MODULE",VARIABLETEXT,0,(modulx!=NULL ? modulx:""),
		TRUE);
	addstdrsrc(mtnrsrc,"NAME",VARIABLETEXT,0,(name!=NULL ? name:""),
		TRUE);
	addsctrsrc(mtnrsrc,"DESCRIPTION",0,(desc!=NULL ? desc:""),TRUE);
	if(ftypes!=NULL) freeapplib(ftypes);
	ftypes=APPlibNEW();
        for(x=0;x<2;++x) addAPPlib(ftypes,filetype_names[x]);
/*
	addAPPlib(ftypes,"Normal File");
	addAPPlib(ftypes,"Special File");
*/
	if(file_type>=ftypes->numlibs) file_type=0;
	addlstrsrc(mtnrsrc,"FILE TYPES",&file_type,TRUE,NULL,
		ftypes->numlibs,&ftypes->libs,NULL);
	if(starttypes!=NULL) freeapplib(starttypes);
	starttypes=APPlibNEW();
	addAPPlib(starttypes,"Maintain Screen");
	addAPPlib(starttypes,"Browse Screen");
	addbtnrsrc(mtnrsrc,"SELECT MAINFILE",TRUE,getfile,NULL);
	addstdrsrc(mtnrsrc,"MAINFILE",VARIABLETEXT,0,(mainfile!=NULL ? mainfile:""),
		TRUE);
	if(start_with>=starttypes->numlibs) start_with=0;
	addlstrsrc(mtnrsrc,"START TYPES",&start_with,TRUE,NULL,
		starttypes->numlibs,&starttypes->libs,NULL);
	addbtnrsrc(mtnrsrc,"SELECT BROWSE SCREEN",TRUE,getscreenbs,dfscnrsrc);
	addstdrsrc(mtnrsrc,"BROWSE SCREEN",VARIABLETEXT,0,browse_screen,TRUE);
	addbtnrsrc(mtnrsrc,"SELECT DEFINE LIST",TRUE,getscreends,dfscnrsrc);
	addstdrsrc(mtnrsrc,"DEFINE LIST",VARIABLETEXT,0,define_list,TRUE);
	addbtnrsrc(mtnrsrc,"SELECT SEARCH BROWSE",TRUE,getscreensb,dfscnrsrc);
	addstdrsrc(mtnrsrc,"SEARCH BROWSE",VARIABLETEXT,0,search_browse,TRUE);
	addbtnrsrc(mtnrsrc,"SELECT MAINTAIN SCREEN",TRUE,getscreenms,dfscnrsrc);
	addstdrsrc(mtnrsrc,"MAINTAIN SCREEN",VARIABLETEXT,0,maintain_screen,TRUE);
	makebuttonlist();
	addlstrsrc(mtnrsrc,"BUTTONS AVAILABLE",0,TRUE,NULL,buttonsavl->numlibs,
		&buttonsavl->libs,NULL);
	addbtnrsrc(mtnrsrc,"ADD BUTTON ABOVE",TRUE,add_button_above,mainrsrc);
	addbtnrsrc(mtnrsrc,"ADD BUTTON BELOW",TRUE,add_button_below,mainrsrc);
	addbtnrsrc(mtnrsrc,"SELECT BUTTON",TRUE,select_button,mainrsrc);
	addbtnrsrc(mtnrsrc,"DELETE BUTTON",TRUE,delete_button,NULL);
	getsupports();
	addlstrsrc(mtnrsrc,"SUPPORTING FILES",0,TRUE,NULL,suppavl->numlibs,
		&suppavl->libs,NULL);
	addbtnrsrc(mtnrsrc,"ADD SUPPORTING ABOVE",TRUE,suppaddabove,mtnrsrc);
	addbtnrsrc(mtnrsrc,"ADD SUPPORTING BELOW",TRUE,suppaddbelow,mtnrsrc);
	addbtnrsrc(mtnrsrc,"DELETE SUPPORTING",TRUE,suppdelete,NULL);
	addbtnrsrc(mtnrsrc,"SELECT SUPPORTING",TRUE,suppedit,mtnrsrc);
	addsctrsrc(mtnrsrc,"SAVE ERROR",0,save_e,TRUE);
	addbtnrsrc(mtnrsrc,"LOAD VALUE SAVE ERROR",TRUE,loadvalue,
		"SAVE ERROR");
	addbtnrsrc(mtnrsrc,"LOAD OPERAND SAVE ERROR",TRUE,loadoperand,
		"SAVE ERROR");
	addbtnrsrc(mtnrsrc,"LOAD GROUPER SAVE ERROR",TRUE,loadgrouper,
		"SAVE ERROR");
	addbtnrsrc(mtnrsrc,"LOAD CONTROL SAVE ERROR",TRUE,loadcontrol,
		"SAVE ERROR");
	addsctrsrc(mtnrsrc,"SAVE ERROR DESCRIPTION",0,save_ed,TRUE);
	addbtnrsrc(mtnrsrc,"LOAD VALUE SAVE ERROR DESCRIPTION",TRUE,loadvalue,
		"SAVE ERROR DESCRIPTION");
	addbtnrsrc(mtnrsrc,"LOAD OPERAND SAVE ERROR DESCRIPTION",TRUE,
		loadoperand,"SAVE ERROR DESCRIPTION");
	addbtnrsrc(mtnrsrc,"LOAD GROUPER SAVE ERROR DESCRIPTION",TRUE,
		loadgrouper,"SAVE ERROR DESCRIPTION");
	addbtnrsrc(mtnrsrc,"LOAD CONTROL SAVE ERROR DESCRIPTION",TRUE,
		loadcontrol,"SAVE ERROR DESCRIPTION");
	addsctrsrc(mtnrsrc,"SAVE WARNING",0,save_w,TRUE);
	addbtnrsrc(mtnrsrc,"LOAD VALUE SAVE WARNING",TRUE,loadvalue,
		"SAVE WARNING");
	addbtnrsrc(mtnrsrc,"LOAD OPERAND SAVE WARNING",TRUE,
		loadoperand,"SAVE WARNING");
	addbtnrsrc(mtnrsrc,"LOAD GROUPER SAVE WARNING",TRUE,
		loadgrouper,"SAVE WARNING");
	addbtnrsrc(mtnrsrc,"LOAD CONTROL SAVE WARNING",TRUE,
		loadcontrol,"SAVE WARNING");
	addsctrsrc(mtnrsrc,"SAVE WARNING DESCRIPTION",0,save_wd,TRUE);
	addbtnrsrc(mtnrsrc,"LOAD VALUE SAVE WARNING DESCRIPTION",TRUE,loadvalue,
		"SAVE WARNING DESCRIPTION");
	addbtnrsrc(mtnrsrc,"LOAD OPERAND SAVE WARNING DESCRIPTION",TRUE,
		loadoperand,"SAVE WARNING DESCRIPTION");
	addbtnrsrc(mtnrsrc,"LOAD GROUPER SAVE WARNING DESCRIPTION",TRUE,
		loadgrouper,"SAVE WARNING DESCRIPTION");
	addbtnrsrc(mtnrsrc,"LOAD CONTROL SAVE WARNING DESCRIPTION",TRUE,
		loadcontrol,"SAVE WARNING DESCRIPTION");
	getsubords();
	addlstrsrc(mtnrsrc,"SUBORDINATE FILES",0,TRUE,NULL,subavl->numlibs,
		&subavl->libs,NULL);
	addbtnrsrc(mtnrsrc,"ADD SUBORDINATE ABOVE",TRUE,subaddabove,mtnrsrc);
	addbtnrsrc(mtnrsrc,"ADD SUBORDINATE BELOW",TRUE,subaddbelow,mtnrsrc);
	addbtnrsrc(mtnrsrc,"DELETE SUBORDINATE",TRUE,subdelete,NULL);
	addbtnrsrc(mtnrsrc,"SELECT SUBORDINATE",TRUE,subedit,mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE",TRUE,savemaintain,mainrsrc);
	addbtnrsrc(mtnrsrc,"QUIT",TRUE,quitmaintaintest,mainrsrc);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
/*lint -e611 */
	addbtnrsrc(mtnrsrc,"PRINT DEFINITION",TRUE,printdefinition,(void *)printmtnfp);
/*lint +e611 */
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	mtntdefaults=GetDefaults(mtnrsrc);
	file_defs=APPlibNEW();
	filersrc=RDArsrcNEW("UTILITIES","UTILITY MASTER SELECT FILE");
	x=0;
	addlstrsrc(filersrc,"FILE LIST",&x,TRUE,NULL,file_defs->numlibs,
		&file_defs->libs,NULL);
	addrfkwrsrc(filersrc,"SELECT",TRUE,okselectfile,mtnrsrc);
	addrfkwrsrc(filersrc,"QUIT",TRUE,NULL_RSCS,NULL);
	addbtnrsrc(filersrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(filersrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	changestyles(mtnrsrc);
	dir_libs=APPlibNEW();
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(USERLOGIN)+2);
	sprintf(defdir,"%s/%s",CURRENTDIRECTORY,USERLOGIN);
	addAPPlib(dir_libs,defdir);
	if(defdir!=NULL) Rfree(defdir); 
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
	sprintf(defdir,"%s/site",CURRENTDIRECTORY);
	addAPPlib(dir_libs,defdir);
	if(defdir!=NULL) Rfree(defdir);
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+5);
	sprintf(defdir,"%s/rda",CURRENTDIRECTORY);
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
	addrfkwrsrc(dfscnrsrc,"SELECT",TRUE,okselectscreen,mtnrsrc);
	addrfkwrsrc(dfscnrsrc,"QUIT",TRUE,NULL_RSCS,NULL);
	addbtnrsrc(dfscnrsrc,"HELP",TRUE,screenhelp,dfscnrsrc);
	addbtnrsrc(dfscnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,dfscnrsrc);
	APPmakescrn(mtnrsrc,TRUE,quitmaintain,mainrsrc,FALSE);
}
static void okchgfld(RDArsrc *mainrsrc)
{
	int selected=0,selectedv=0;
	char *libx=NULL,*directory=NULL,*tmpx=NULL;

	changedvalues=FALSE;changedbtnfld=FALSE;changedbutton=FALSE;
	changedargs=FALSE;
	readwidget(mainrsrc,"DIRECTORY");
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"CHANGE FIELDS AVAILABLE",&selectedv)) return;
	if(MTNMSTR!=NULL) FreeChangeField(MTNMSTR);
	if(RDAstrcmp(chgfldavl->libs[selectedv],"No Change Field Definintions")) 
	{
		libx=Rmalloc(RDAstrlen(directory)+RDAstrlen(modlst->libs[selected])+6);
		tmpx=adddashes(modlst->libs[selected]);
		sprintf(libx,"%s/%s.MTN",directory,tmpx);
		MTNMSTR=ChangeFieldNew(modlst->libs[selected],chgfldavl->libs[selectedv]);
		if(tmpx!=NULL) Rfree(tmpx);
		if(getChangeField(libx,MTNMSTR))
		{
			ERRORDIALOG("Cannot Read Maintain Master","Error in permissions, the user cannot read the maintain master definition.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
			if(libx!=NULL) Rfree(libx);
			if(MTNMSTR!=NULL) FreeChangeField(MTNMSTR);
			return;
		}
		if(libx!=NULL) Rfree(libx);
	}
	if(MTNMSTR==NULL)
	{
		MTNMSTR=ChangeFieldNew(modlst->libs[selected],NULL);
	}
	maintainscreen(mainrsrc,MTNMSTR->module,MTNMSTR->name,MTNMSTR->desc,
		MTNMSTR->style,MTNMSTR->mainfile,MTNMSTR->browse_screen,
		MTNMSTR->define_list,MTNMSTR->search_browse,MTNMSTR->maintain_screen,
		MTNMSTR->file_type,MTNMSTR->start_with,MTNMSTR->numbuttons,
		MTNMSTR->buttons,MTNMSTR->num,MTNMSTR->s,
		MTNMSTR->subnum,MTNMSTR->subord,MTNMSTR->save_expression,
		MTNMSTR->save_error_desc,MTNMSTR->save_warning,
		MTNMSTR->save_warning_desc,MTNMSTR->inheritrsrcs);
}
static void newchgfld(RDArsrc *mainrsrc)
{
	int selected=0;

	changedvalues=FALSE;changedbtnfld=FALSE;changedbutton=FALSE;
	changedargs=FALSE;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(MTNMSTR!=NULL) FreeChangeField(MTNMSTR);
	MTNMSTR=ChangeFieldNew(modlst->libs[selected],NULL);
	maintainscreen(mainrsrc,MTNMSTR->module,MTNMSTR->name,MTNMSTR->desc,
		MTNMSTR->style,MTNMSTR->mainfile,MTNMSTR->browse_screen,
		MTNMSTR->define_list,MTNMSTR->search_browse,MTNMSTR->maintain_screen,
		MTNMSTR->file_type,MTNMSTR->start_with,MTNMSTR->numbuttons,
		MTNMSTR->buttons,MTNMSTR->num,MTNMSTR->s,
		MTNMSTR->subnum,MTNMSTR->subord,
		MTNMSTR->save_expression,
		MTNMSTR->save_error_desc,MTNMSTR->save_warning,
		MTNMSTR->save_warning_desc,MTNMSTR->inheritrsrcs);
}
static void quitdfchgfld(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	if(modlst!=NULL) freeapplib(modlst);
	if(chgfldavl!=NULL) freeapplib(chgfldavl);
	if(mtntdefaults!=NULL) FreeRDAdefault(mtntdefaults);
	if(diravl!=NULL) freeapplib(diravl);
	ShutdownSubsystems();
}
static void deletechgfld(RDArsrc *mainrsrc)
{
	int s,m=0;
	char *libx=NULL,*modulename=NULL;
	char *directory=NULL;

	readallwidgets(mainrsrc);
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&m)) return;
	if(FINDRSCGETINT(mainrsrc,"CHANGE FIELDS AVAILABLE",&s)) return;
	modulename=adddashes(modlst->libs[m]);	
	libx=Rmalloc(RDAstrlen(directory)+RDAstrlen(modulename)+6);
	sprintf(libx,"%s/%s.CHG",directory,modulename);
	if(deletelibbin(libx,chgfldavl->libs[s]))
	{
		ERRORDIALOG("Cannot Delete Change Field Definition!","The user cannot delete this Change Field Definition.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
	} else doliblist1(mainrsrc);
	if(libx!=NULL) Rfree(libx);
	if(modulename!=NULL) Rfree(modulename);
	if(directory!=NULL) Rfree(directory);
}
static void deletechgfldtest(RDArsrc *mainrsrc)
{
	int s=0;

	if(!RDAstrcmp(chgfldavl->libs[s],"No Change Field Definintions")) return;
	WARNINGDIALOG("DELETE WARNING DIALOG SCREEN","DELETE CHANGE FIELD DEFINITION!","Are you sure you want to delete this change field definition?",deletechgfld,NULL,FALSE,mainrsrc);
}
static void setvaluesx(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;
	char *tmpx=NULL,*temp=NULL,*libx=NULL,*directory=NULL;

	readwidget(mainrsrc,"DIRECTORY");
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	libx=Rmalloc(RDAstrlen(directory)+RDAstrlen(modlst->libs[selectedm])+6);
	tmpx=adddashes(modlst->libs[selectedm]);
	sprintf(libx,"%s/%s.CHG",directory,tmpx);
	if(tmpx!=NULL) Rfree(tmpx);
	if(FINDRSCGETINT(mainrsrc,"CHANGE FIELDS AVAILABLE",&selected)) return;
	if(!RDAstrcmp(chgfldavl->libs[selected],"No Change Field Definintions"))
	{
		temp=NULL;
	} else {
		temp=getChangeFielddesc(libx,chgfldavl->libs[selected]);
	}
	if(libx!=NULL) Rfree(libx);
	if(directory!=NULL) Rfree(directory);
	if(!FINDRSCSETSTRING(mainrsrc,"DESCRIPTION",temp))
		updatersrc(mainrsrc,"DESCRIPTION");
	if(temp!=NULL) Rfree(temp);
}
static void printmodule(RDArsrc *mainrsrc,FILE *fp)
{
	int x=0,selected=0,lines=0,pages=0;
	char *tmpx=NULL,*libx=NULL,*directory=NULL,*temp=NULL;
	ChangeField *d=NULL;
	APPlib *chgfldnames=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	libx=Rmalloc(RDAstrlen(directory)+RDAstrlen(modlst->libs[selected])+6);
	tmpx=adddashes(modlst->libs[selected]);
	sprintf(libx,"%s/%s.CHG",directory,tmpx);
	if(directory!=NULL) Rfree(directory);
	chgfldnames=getlibnames(libx,FALSE);
	if(chgfldnames!=NULL)
	{
		temp=stralloc("XPERT CHANGE FIELD DEFINITION"); 
		printheader(fp,&lines,&pages,temp,tmpx);
		if(temp!=NULL) Rfree(temp);
		for(x=0;x<chgfldnames->numlibs;++x)
		{
			d=ChangeFieldNew(modlst->libs[selected],chgfldnames->libs[x]);
			if(!getChangeField(libx,d))
			{
				printmtn1(fp,d,&lines,&pages);
				fprintf(fp,"\f\n");
			}
			if(d!=NULL) FreeChangeField(d);
		}
	} else {
		prterr("Error No Change Field Available.");
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
	if(FINDRSCGETINT(mainrsrc,"CHANGE FIELDS AVAILABLE",&selm)) return;
	if(chgfldavl!=NULL) freeapplib(chgfldavl);
	libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(modlst->libs[selected])+6);
	sprintf(libx,"%s/%s.CHG",dirx,modlst->libs[selected]);
	chgfldavl=getlibnames(libx,FALSE);
	if(dirx!=NULL) Rfree(dirx);
	if(libx!=NULL) Rfree(libx);
	if(chgfldavl==NULL)
	{
		chgfldavl=APPlibNEW();
		addAPPlib(chgfldavl,"No Change Field Definintions");
	}
	if(selm>=chgfldavl->numlibs) selm=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"CHANGE FIELDS AVAILABLE",selm,chgfldavl))
		updatersrc(mainrsrc,"CHANGE FIELDS AVAILABLE");
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
	if(modlst!=NULL) freeapplib(modlst);
	modlst=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(directory)+2);
	sprintf(dirx,"%s/",directory);
	for(x=0;findfile(dirx,"*.MTN",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(modlst,tmp);
	}
	if(modlst->numlibs<1)
	{
		addAPPlib(modlst,"Directory Contains No Libraries");
	} else {
		SORTAPPlib(modlst);
	}
	if(tmp!=NULL) Rfree(tmp);
	if(directory!=NULL) Rfree(directory);
	if(dirx!=NULL) Rfree(dirx);
	if(selectedx>=modlst->numlibs) selectedx=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"MODULE LIST",selectedx,modlst))
		updatersrc(mainrsrc,"MODULE LIST");
	doliblist1(mainrsrc);
}
static void dochgfld(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;
	APPlib *args=NULL;

	if(chgfldavl->numlibs==1 && !RDAstrcmp(chgfldavl->libs[0],"No Change Field Definintions")) return; 
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(mainrsrc,"CHANGE FIELDS AVAILABLE",&selected)) return;
	args=APPlibNEW();
	addAPPlib(args,modlst->libs[selectedm]);
	addAPPlib(args,chgfldavl->libs[selected]);
	ExecuteProgram("dochgfld",args);
	if(args!=NULL) freeapplib(args);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0;
	RDArsrc *mainrsrc=NULL;
	char *defdir=NULL;

	if(InitializeSubsystems(argc,argv,module,"DEFINE CHANGE FIELDS")) 
	{
		return;
	}
	DoHeaders();
	mainrsrc=RDArsrcNEW(module,"DEFINE CHANGE FIELDS");
	diravl=APPlibNEW();
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(USERLOGIN)+2);
	sprintf(defdir,"%s/%s",CURRENTDIRECTORY,USERLOGIN);
	addAPPlib(diravl,defdir);
	if(defdir!=NULL) Rfree(defdir);
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
	sprintf(defdir,"%s/site",CURRENTDIRECTORY);
	addAPPlib(diravl,defdir);
	if(defdir!=NULL) Rfree(defdir); 
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+5);
	sprintf(defdir,"%s/rda",CURRENTDIRECTORY);
	addAPPlib(diravl,defdir);
	if(defdir!=NULL) Rfree(defdir);
	addlstrsrc(mainrsrc,"DIRECTORY LIST",0,TRUE,getdir1,diravl->numlibs,&diravl->libs,NULL);
	addstdrsrc(mainrsrc,"DIRECTORY",VARIABLETEXT,0,diravl->libs[0],TRUE);
	if(defdir!=NULL) Rfree(defdir);
	FINDRSCLISTAPPlib(mainrsrc,"DIRECTORY LIST",0,diravl);
	FINDRSCSETFUNC(mainrsrc,"DIRECTORY",setdirlist1,NULL);
	x=0;
	if(modlst!=NULL) freeapplib(modlst); 
	modlst=APPlibNEW();
	addlstrsrc(mainrsrc,"MODULE LIST",&x,TRUE,doliblist1,
		modlst->numlibs,&modlst->libs,NULL);
	x=0;
	if(chgfldavl!=NULL) freeapplib(chgfldavl); 
	chgfldavl=APPlibNEW();
	addlstrsrc(mainrsrc,"CHANGE FIELDS AVAILABLE",&x,TRUE,setvaluesx,
		chgfldavl->numlibs,&chgfldavl->libs,NULL);
	addsctrsrc(mainrsrc,"DESCRIPTION",0,NULL,FALSE);
	getdir1(mainrsrc);
	addbtnrsrc(mainrsrc,"RUN CHANGE FIELDS",TRUE,dochgfld,NULL);
	addbtnrsrc(mainrsrc,"ADD",TRUE,newchgfld,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,okchgfld,NULL);
/*lint -e611 */
	addbtnrsrc(mainrsrc,"PRINT",TRUE,printdefinition,(void *)printmodulefp);
/*lint +e611 */
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitdfchgfld,NULL);
	addbtnrsrc(mainrsrc,"DELETE",TRUE,deletechgfldtest,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,quitdfchgfld,NULL,TRUE);
}
