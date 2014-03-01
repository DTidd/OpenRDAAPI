/* dfchg.c - Define Change */
#ifndef WIN32
#define __NAM__ "dfchg.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dfchg.exe"
#endif
#include <app.hpp>
#include <mix.hpp>
#include <ldval.hpp>
#include <doheader.hpp>
#include <dfchg.hpp>
/*CPP_OPART dfchgfl */
/*CPP_OPART dfchgsup */
/*ADDLIB chglib */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="UTILITIES";
RDArsrc *mainrsrc=NULL,*mtnrsrc=NULL,*dfscnrsrc=NULL,*filersrc=NULL;
APPlib *modlst=NULL,*chgstructavl=NULL;
APPlib *dir_libs=NULL,*scr_defs=NULL,*file_defs=NULL,*diravl=NULL;
APPlib *fieldsavl=NULL,*keysavl=NULL,*ftypes=NULL,*filelstx=NULL;
APPlib *filelist=NULL,*keylst=NULL,*tmpargs=NULL,*modlstx=NULL;
APPlib *modulelist=NULL;
CHGstruct *CHGSTRUCT=NULL;
extern RDAsearch *holdsupfils;
static void savemaintain(RDArsrc *,RDArsrc *);
static void quitchgstructtest(RDArsrc *,RDArsrc *),setvaluesx(RDArsrc *);
static void quitchgstruct(RDArsrc *,RDArsrc *);
static void printmtnfp(RDArsrc *,RDArsrc *);
static void printmtn(RDArsrc *,RDA_PFILE *);
static void fixmtnoutput(RDA_PFILE *,char *,int *,int *,char *);
static void checkheader(RDA_PFILE *,int *,int *,char *);
static void printheader(RDA_PFILE *,int *,int *,char *,char *);
static void printdefinition(RDArsrc *,void (*)(...));
static void printfp(RDArsrc *,void (*)(...),RDArsrc *);
static void printmodulefp(RDArsrc *prsrc,RDArsrc *rsrc);
static void printmodule(RDArsrc *,RDA_PFILE *);
static RDAdefault *mtntdefaults=NULL;
char changedvalues=FALSE;
static char *snames[1]={"RANGE SCREEN"}; 
static int which_screen=0;
static void doliblist1(RDArsrc *),getscreenlist(RDArsrc *);
static void dochgstruct(RDArsrc *);
CHGfile *tempchgfile=NULL;
int numchgfile=0;
APPlib *chgfilesavl=NULL;
APPlib *filelst=NULL,*cmodlst=NULL,*subkeylist=NULL,*chgflds=NULL;
APPlib *econflds=NULL,*supfilsavl=NULL;
char *contype[3]={
	"Equal To and Check on Save",
	"Equal To Only",
	"Include File Only",
};
extern void savechg(void),chgedit(RDArsrc *,RDArsrc *),getchgfiles(void);
extern void DFchgcopy(RDArsrc *,RDArsrc *);
extern void chgaddbelow(RDArsrc *,RDArsrc *),chgaddabove(RDArsrc *,RDArsrc *);
extern void chgdelete(RDArsrc *);
APPlib *makefieldvallist3(RDAsearch *,int,short,char *,char *);
APPlib *makefieldvallist2(CHGfile *,int,short,char *,char *);
void loadvalue(RDArsrc *,char *);

APPlib *makefieldvallist3(RDAsearch *slist,int nbr,short dofields,char *submodname,char *subfilename)
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
	if(slist!=NULL)
	{
	for(x=0,s=slist;x<nbr;++x,++s)
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
APPlib *makefieldvallist2(CHGfile *ch,int nbr,short dofields,char *submodname,char *subfilename)
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
	if(ch!=NULL)
	{
	for(x=0,s=ch->supfil;(x<nbr && x<ch->num);++x,++s)
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
APPlib *makefieldvallist()
{
	APPlib *tmp=NULL,*virflist=NULL;
	char addvf=FALSE,*libx=NULL;
	int x,y,z;
	DFincvir *i,*a;
	DFvirtual *d=NULL;
	short dumb=0;
	RDArsrc *tmprsrc=NULL;
	RDAsearch *s;
	CHGfile *temp=NULL;

	tmp=APPlibNEW();
	tmprsrc=RDArsrcNEW(module,"TEMP");
	if(CHGSTRUCT->chgfiles!=NULL)
	{
		for(x=0,temp=CHGSTRUCT->chgfiles;x<CHGSTRUCT->numfiles;++x,++temp)
		{
			addDFincvir(tmprsrc,temp->module,temp->filename,NULL,dumb);
			if(temp->supfil!=NULL)
			{
				for(y=0,s=temp->supfil;y<temp->num;++y,++s)
				{
					addDFincvir(tmprsrc,s->module,s->filename,
						NULL,dumb);
				}
			}
		}
	}
	for(x=0,i=tmprsrc->incvir;x<tmprsrc->numvir;++x,++i)
	{
		addholdvalues(tmp,i->module,i->file);
	}
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(CHGSTRUCT->module)+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,CHGSTRUCT->module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,CHGSTRUCT->module);
#endif
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
	SORTAPPlib(tmp);
	return(tmp);
}
void loadvalue(RDArsrc *r,char *fieldname)
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
static void okselectfile(RDArsrc *filersrc,RDArsrc *mrsrc)
{
	int f=0;

	if(FINDRSCGETINT(filersrc,"FILE LIST",&f)) return;
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
#ifdef WIN32
	sprintf(libx,"%s\\%s.CHG",dirx,modulx);
#endif
#ifndef WIN32
	sprintf(libx,"%s/%s.CHG",dirx,modulx);
#endif
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
static void getscreenms(RDArsrc *mtnrsrc,RDArsrc *dfscnrsrc)
{
	which_screen=0;
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
static void quitchgstruct(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	int x,y,z;
	RDAconfld *confld1;
	RDAsearch *s;
	CHGfield *chgfield;
	CHGfile *tempchg;

	if(tempchgfile!=NULL)
	{
		for(x=0,tempchg=tempchgfile;x<numchgfile;++x,++tempchg)
		{
			if(tempchg->module!=NULL) Rfree(tempchg->module);
			if(tempchg->filename!=NULL) Rfree(tempchg->filename);
			if(tempchg->keyname!=NULL) Rfree(tempchg->keyname);
			if(tempchg->expression!=NULL) Rfree(tempchg->expression);
			if(tempchg->supfil!=NULL)
			{
				for(y=0,s=tempchg->supfil;y<tempchg->num;++y,++s)
				{
					if(s->module!=NULL) Rfree(s->module);
					if(s->filename!=NULL) Rfree(s->filename);
					if(s->cmodule!=NULL) Rfree(s->cmodule);
					if(s->cfilename!=NULL) Rfree(s->cfilename);
					if(s->subkeyname!=NULL) Rfree(s->subkeyname);
					if(s->expression!=NULL) Rfree(s->expression);
					if(s->confld!=NULL)
					{
						for(z=0,confld1=s->confld;z<s->num;++z,++confld1)
						{
							if(confld1->fldname!=NULL) Rfree(confld1->fldname);
							if(confld1->conname!=NULL) Rfree(confld1->conname);
						}
						Rfree(s->confld);
					}
				}
				Rfree(tempchg->supfil);
			}
			if(tempchg->chgflds!=NULL)
			{
				for(y=0,chgfield=tempchg->chgflds;y<tempchg->numflds;++y,++chgfield)
				{
					if(chgfield->fieldname!=NULL) Rfree(chgfield->fieldname);
				}
				Rfree(tempchg->chgflds);
			}
		}
		Rfree(tempchgfile);
		numchgfile=0;
	}
	if(dir_libs!=NULL) freeapplib(dir_libs);
	if(scr_defs!=NULL) freeapplib(scr_defs);
	if(file_defs!=NULL) freeapplib(file_defs);
	if(ftypes!=NULL) freeapplib(ftypes);
	if(chgfilesavl!=NULL) freeapplib(chgfilesavl);
	changedvalues=FALSE;
	if(CHGSTRUCT!=NULL) FreeCHGstruct(CHGSTRUCT);
	CHGSTRUCT=NULL;
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
	if(dfscnrsrc!=NULL) free_rsrc(dfscnrsrc);
	if(filersrc!=NULL) free_rsrc(filersrc);
	if(mtntdefaults!=NULL) FreeRDAdefault(mtntdefaults);
}
static void quitchgstructtest(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	readallwidgets(mtnrsrc);
	if(COMPARE_RSRCS(mtnrsrc,mtntdefaults,2) || changedvalues)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","There are changes to the CHANGE DEFINITION.\nDo you want to Save these changes?",savemaintain,quitchgstruct,FALSE,2,mtnrsrc,mainrsrc,NULL);
	} else {
		quitchgstruct(mtnrsrc,mainrsrc);
	}
}
static void savemaintain(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	char *libname=NULL;
	char *directory=NULL;
	int x;

	readallwidgets(mtnrsrc);
	if(FINDRSCGETSTRING(mtnrsrc,"DIRECTORY",&directory)) return;
	FINDRSCGETSTRING(mtnrsrc,"MODULE",&CHGSTRUCT->module);
	libname=Rmalloc(RDAstrlen(directory)+RDAstrlen(CHGSTRUCT->module)+6);
#ifndef WIN32
	sprintf(libname,"%s/%s.CHG",directory,CHGSTRUCT->module);
#endif
#ifdef WIN32
	sprintf(libname,"%s\\%s.CHG",directory,CHGSTRUCT->module);
#endif
	FINDRSCGETSTRING(mtnrsrc,"NAME",&CHGSTRUCT->name);
	if(!isEMPTY(CHGSTRUCT->name))
	{
		FINDRSCGETSTRING(mtnrsrc,"DESCRIPTION",&CHGSTRUCT->desc);
		FINDRSCGETSTRING(mtnrsrc,"FIELD NAME",&CHGSTRUCT->fieldname);
		FINDRSCGETINT(mtnrsrc,"FIELD TYPES",&x);
		CHGSTRUCT->type=x;
		FINDRSCGETINT(mtnrsrc,"FIELD LENGTH",&x);
		CHGSTRUCT->len=x;
		FINDRSCGETSTRING(mtnrsrc,"EXPRESSION",&CHGSTRUCT->expression);
		FINDRSCGETCHAR(mtnrsrc,"RANGE",&CHGSTRUCT->range);
		FINDRSCGETSTRING(mtnrsrc,"RANGE SCREEN",&CHGSTRUCT->range_screen);
		savechg();
		if(writeCHGstruct(libname,CHGSTRUCT))
		{
			ERRORDIALOG("Cannot Write Change","The user cannot write the Change Definition.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
		}
		FreeCHGstruct(CHGSTRUCT);
		CHGSTRUCT=NULL;
	}
	if(libname!=NULL) Rfree(libname);
	if(directory!=NULL) Rfree(directory);
	doliblist1(mainrsrc);
	quitchgstruct(mtnrsrc,mainrsrc);
}
static void fixmtnoutput(RDA_PFILE *fp,char *data,int *lines,int *pages,char *modname)
{
	unsigned y=0,x=0,length=0,line_length=79;
	char *temp=NULL,*temp1=NULL;

	if(!isEMPTY(data))
	{
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
			RDA_fprintf(fp,"%.*s\n",x+1,data);
			checkheader(fp,lines,pages,modname);
			fixmtnoutput(fp,data+x+1,lines,pages,modname);
		} else {
			RDA_fprintf(fp,"%.*s\n",length,data);
			if(temp!=NULL || temp1!=NULL) fixmtnoutput(fp,data+length+1,lines,pages,modname);
			checkheader(fp,lines,pages,modname);
		}
	} 
}
static void printmtn1(RDA_PFILE *fp,CHGstruct *d,int *lines,int *pages)
{
	char *temp=NULL;
	CHGfile *chgfile=NULL;
	RDAsearch *supfil=NULL;
	RDAconfld *e=NULL;
	CHGfield *chgfld=NULL;
	int y=0,z=0,p=0;

	if(fp!=NULL)
	{
		RDA_fprintf(fp,"Module: [%s] Name: [%s]",
			(d->module!=NULL?d->module:""),
			(d->name!=NULL?d->name:""));
		checkheader(fp,lines,pages,d->module);
		RDA_fprintf(fp,"  Field Name: [%s]",
			(d->fieldname!=NULL ? d->fieldname:""));
		checkheader(fp,lines,pages,d->module);
		RDA_fprintf(fp,"  Type: [%d] Length: [%d]",
			d->type,d->range);
		checkheader(fp,lines,pages,d->module);
		RDA_fprintf(fp,"  Range: [%s] Range Screen: [%s]",
			(d->range==TRUE?"TRUE":"FALSE"),
			(d->range_screen!=NULL?d->range_screen:""));
		checkheader(fp,lines,pages,d->module);
		if(!isEMPTY(d->expression))
		{
			temp=Rmalloc(17+RDAstrlen(d->expression));
			sprintf(temp,"  Expression: [%s]",d->expression);
			fixmtnoutput(fp,temp,lines,pages,d->module);
			if(temp!=NULL) Rfree(temp);
		} else {
			RDA_fprintf(fp,"  Expression: []");
			checkheader(fp,lines,pages,d->module);
		}
		RDA_fprintf(fp,"    Number of Files: [%d]",d->numfiles);
		checkheader(fp,lines,pages,d->module);
		if(d->numfiles>0 && d->chgfiles!=NULL)
		{
			for(y=0,chgfile=d->chgfiles;y<d->numfiles;++y,++chgfile)
			{
				RDA_fprintf(fp,"      Module: [%s] File: [%s] Key Name: [%s]",
					(chgfile->module!=NULL?chgfile->module:""),
					(chgfile->filename!=NULL?chgfile->filename:""),
					(chgfile->keyname!=NULL?chgfile->keyname:""));
				checkheader(fp,lines,pages,d->module);
				if(!isEMPTY(chgfile->expression))
				{
					temp=Rmalloc(19+RDAstrlen(chgfile->expression));
					sprintf(temp,"      Expression: [%s]",chgfile->expression);
					fixmtnoutput(fp,temp,lines,pages,d->module);
					if(temp!=NULL) Rfree(temp);
				} else {
					RDA_fprintf(fp,"      Expression: []");
					checkheader(fp,lines,pages,d->module);
				}
				RDA_fprintf(fp,"        Number of Supporting Files: [%d]",chgfile->num);
				checkheader(fp,lines,pages,d->module);
				if(chgfile->num>0 && chgfile->supfil!=NULL)
				{
					for(z=0,supfil=chgfile->supfil;z<chgfile->num;++z,++supfil)
					{
						RDA_fprintf(fp,"          Module: [%s] File: [%s] Key Name: [%s]",
							(supfil->module!=NULL?supfil->module:""),
							(supfil->filename!=NULL?supfil->filename:""),
							(supfil->subkeyname!=NULL?supfil->subkeyname:""));
						RDA_fprintf(fp,"          Connect Type: [%d][%s]",supfil->ctype,contype[supfil->ctype]);
						checkheader(fp,lines,pages,d->module);
						RDA_fprintf(fp,"          Connect Module: [%s] Connect File: [%s]",
							(supfil->cmodule!=NULL?supfil->cmodule:""),
							(supfil->cfilename!=NULL?supfil->cfilename:""));
						checkheader(fp,lines,pages,d->module);
						RDA_fprintf(fp,"            Number of Connect Fields: [%d]",supfil->num);
						checkheader(fp,lines,pages,d->module);
						if(supfil->num>0 && supfil->confld!=NULL)
						{
							for(p=0,e=supfil->confld;p<supfil->num;++p,++e)
							{
								RDA_fprintf(fp,"              Field Name: [%s]",
									(e->fldname!=NULL?e->fldname:""));
								checkheader(fp,lines,pages,d->module);
								RDA_fprintf(fp,"              Connect Name: [%s]",
									(e->conname!=NULL?e->conname:""));
								checkheader(fp,lines,pages,d->module);
							}
						}
						RDA_fprintf(fp,"          Range: [%s]",
							(supfil->range==TRUE?"TRUE":"FALSE"));
						checkheader(fp,lines,pages,d->module);
						if(!isEMPTY(supfil->expression))
						{
							temp=Rmalloc(21+RDAstrlen(supfil->expression));
							sprintf(temp,"          Expression: [%s]",supfil->expression);
							fixmtnoutput(fp,temp,lines,pages,d->module);
							if(temp!=NULL) Rfree(temp);
						} else {
							RDA_fprintf(fp,"          Expression: []");
							checkheader(fp,lines,pages,d->module);
						}
					}
				}
				RDA_fprintf(fp,"        Number of Change Fields: [%d]",chgfile->numflds);
				checkheader(fp,lines,pages,d->module);
				if(chgfile->numflds>0 && chgfile->chgflds!=NULL)
				{
					for(z=0,chgfld=chgfile->chgflds;z<chgfile->numflds;++z,++chgfld)
					{
						RDA_fprintf(fp,"          Field Name: [%s] Key Part: [%s]",
							(chgfld->fieldname!=NULL?chgfld->fieldname:""),
							(chgfld->keypart!=TRUE?"TRUE":"FALSE"));
						checkheader(fp,lines,pages,d->module);
					}
				}
			}
		}
	}
}
static void printmtn(RDArsrc *rsrc,RDA_PFILE *fp)
{
	int lines=1,pages=1;
	char *temp=NULL;

	temp=stralloc("XPERT CHANGE DEFINITION"); 
	printheader(fp,&lines,&pages,temp,CHGSTRUCT->module);
	if(temp!=NULL) Rfree(temp);
	printmtn1(fp,CHGSTRUCT,&lines,&pages);
	RDA_fprintf(fp,"\f\n");
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
		if(*pages==1 && *lines==1)
		{
			RDA_fprintf(fp,"\f"); *lines=1; *pages=*pages+1;
			temp=stralloc("XPERT CHANGE DEFINITION"); 
			printheader(fp,lines,pages,temp,modname);
			if(temp!=NULL) Rfree(temp);
		}
		RDA_fprintf(fp,"\n"); ++*lines;
		if(*lines>60)
		{
			RDA_fprintf(fp,"\f"); *lines=1; *pages=*pages+1;
			temp=stralloc("XPERT CHANGE DEFINITION"); 
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
	RDA_fprintf(fp,"\n");
	*lines=1;
	lead_ws=(int)(RDAroundup((double)(line_length-(2+RDAstrlen(header)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(2+RDAstrlen(header)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(header)+2+14+lead_ws+tail_ws)>line_length) header[(line_length-14-lead_ws-tail_ws)]=0;
	RDA_fprintf(fp,"%*s %s %*sPage: %*d\n",
		lead_ws,"",header,tail_ws,"",10,*pages);
	++*lines;
	lead_ws=(int)(RDAroundup((double)(line_length-(10+RDAstrlen(modname)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(10+RDAstrlen(modname)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	RDA_fprintf(fp,"%*s Module: %s %*sDate: %s\n",
		lead_ws,"",(modname!=NULL?modname:""),tail_ws,"",date);
	++*lines;
	lead_ws=(int)(RDAroundup(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(USERLOGIN)+16+15+lead_ws+tail_ws)>line_length)
	{
		RDA_fprintf(fp,"%*s %s %*sTime:   %s\n",
			(int)(RDAroundup(((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2))),"",
			USERLOGIN,
			(int)(Rint((((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2)-16))),"",
			timex);
		++*lines;
	} else {
		RDA_fprintf(fp,"%*s Executed By: %s %*sTime:   %s\n",
			lead_ws,"",USERLOGIN,tail_ws,"",timex);
		++*lines;
	}
	RDA_fprintf(fp,"\n");
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
static void set_range(RDArsrc *mainrsrc)
{
	char range=FALSE;

	readwidget(mainrsrc,"RANGE");
	FINDRSCGETCHAR(mainrsrc,"RANGE",&range);
	if(range)
	{
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE SCREEN",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"RANGE SCREEN",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"SELECT RANGE SCREEN",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"SELECT RANGE SCREEN",TRUE);
	} else {
		FINDRSCSETEDITABLE(mainrsrc,"RANGE SCREEN",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"RANGE SCREEN",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"SELECT RANGE SCREEN",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"SELECT RANGE SCREEN",FALSE);
	}
}
static void dfscn(RDArsrc *r)
{
	ExecuteProgram("dfscn",NULL);
}
static void dfmenu(RDArsrc *r)
{
	ExecuteProgram("dfmenu",NULL);
}
static void maintainscreen(RDArsrc *mainrsrc,char *modulx,char *name,
	char *desc,char *fieldname,short type,unsigned length,
	char range,char *rangescreen,
	char *expression,int numfiles,CHGfile *fls)
{
	int field_type=0,x=0,y=0,z=0;
	RDArsrc *mtnrsrc=NULL;
	char *defdir=NULL;
	CHGfile *tempchg1,*tempchg;
	CHGfield *fld1=NULL,*fld2=NULL;
	RDAsearch *s1,*s2;
	RDAconfld *confld1,*confld2;

	if(fls!=NULL)
	{
		numchgfile=numfiles;
		tempchgfile=Rmalloc(numchgfile*sizeof(CHGfile));
		for(x=0,tempchg1=tempchgfile,tempchg=fls;x<numchgfile;++x,++tempchg1,++tempchg)
		{
			if(tempchg->module!=NULL)
			{
				tempchg1->module=stralloc(tempchg->module);
			} else tempchg1->module=NULL;
			if(tempchg->filename!=NULL)
			{
				tempchg1->filename=stralloc(tempchg->filename);
			} else tempchg1->filename=NULL;
			if(tempchg->keyname!=NULL)
			{
				tempchg1->keyname=stralloc(tempchg->keyname);
			} else tempchg1->keyname=NULL;
			if(tempchg->expression!=NULL)
			{
				tempchg1->expression=stralloc(tempchg->expression);
			} else tempchg1->expression=NULL;
			if(tempchg->supfil!=NULL)
			{
				tempchg1->num=tempchg->num;
				tempchg1->supfil=Rmalloc(tempchg1->num*sizeof(RDAsearch));
				for(y=0,s1=tempchg->supfil,s2=tempchg1->supfil;
					y<tempchg1->num;++y,++s1,++s2)
				{
					if(s1->module!=NULL)
					{
						s2->module=stralloc(s1->module);
					} else s2->module=NULL;
					if(s1->filename!=NULL) 
					{
						s2->filename=stralloc(s1->filename);
					} else s2->filename=NULL;
					s2->ctype=s1->ctype;
					if(s1->cmodule!=NULL)
					{
						s2->cmodule=stralloc(s1->cmodule);
					} else s2->cmodule=NULL;
					if(s1->cfilename!=NULL)
					{
						s2->cfilename=stralloc(s1->cfilename);
					} else s2->cfilename=NULL;
					if(s1->confld!=NULL)
					{
						s2->num=s1->num;
						s2->confld=Rmalloc(s2->num*sizeof(RDAconfld));
						for(z=0,confld1=s1->confld,confld2=s2->confld;z<s2->num;++z,++confld1,++confld2)
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
					} else {
						s2->confld=NULL;
						s2->num=0;
					}
					if(s1->subkeyname!=NULL)
					{
						s2->subkeyname=stralloc(s1->subkeyname);
					} else s2->subkeyname=NULL;
					s2->range=s1->range;
					if(s1->expression!=NULL)
					{
						s2->expression=stralloc(s1->expression);
					} else s2->expression=NULL;
					s2->num_writes=0;
					s2->fileno=(-1);
					s2->keyno=(-1);
					s2->occurrence=(-1);
					s2->writable=(-1);
				}
			} else {
				tempchg1->supfil=NULL;
				tempchg1->num=0;
			}
			if(tempchg->chgflds!=NULL)
			{
				tempchg1->numflds=tempchg->numflds;
				tempchg1->chgflds=Rmalloc(tempchg->numflds*sizeof(CHGfield));
				for(y=0,fld1=tempchg->chgflds,fld2=tempchg1->chgflds;y<tempchg1->numflds;++y,++fld1,++fld2)
				{
					fld2->keypart=fld1->keypart;
					if(fld1->fieldname!=NULL)
					{
						fld2->fieldname=stralloc(fld1->fieldname);
					} else fld2->fieldname=NULL;
				}
			} else {
				tempchg1->chgflds=NULL;
				tempchg1->numflds=0;
			}
			tempchg1->prev=NULL;
			tempchg1->parent=NULL;
			tempchg1->fileno=(-1);
			tempchg1->keyno=(-1);
		}
	} else {
		tempchgfile=NULL;
		numchgfile=0;
	}
	mtnrsrc=RDArsrcNEW(module,"MAINTAIN CHANGE");
	dfscnrsrc=RDArsrcNEW("UTILITIES","UTILITY MASTER SELECT SCREEN");
	FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&defdir);
	addstdrsrc(mtnrsrc,"DIRECTORY",VARIABLETEXT,0,defdir,TRUE);
	if(defdir!=NULL) Rfree(defdir);
	addstdrsrc(mtnrsrc,"MODULE",VARIABLETEXT,0,(modulx!=NULL ? modulx:""),
		TRUE);
	addstdrsrc(mtnrsrc,"NAME",VARIABLETEXT,0,(name!=NULL ? name:""),
		TRUE);
	addstdrsrc(mtnrsrc,"FIELD NAME",VARIABLETEXT,0,(fieldname!=NULL ? fieldname:""),TRUE);
	if(ftypes!=NULL) freeapplib(ftypes);
	ftypes=APPlibNEW();
	for(x=0;x<28;++x)
	{
		addAPPlib(ftypes,standardfieldtypes[x]);
	}
	field_type=type;
	addlstrsrc(mtnrsrc,"FIELD TYPES",&field_type,TRUE,NULL,ftypes->numlibs,
		&ftypes->libs,NULL);
	addstdrsrc(mtnrsrc,"FIELD LENGTH",LONGV,4,&length,TRUE);
	addsctrsrc(mtnrsrc,"DESCRIPTION",0,(desc!=NULL ? desc:""),TRUE);
	addstdrsrc(mtnrsrc,"RANGE",BOOLNS,1,&range,TRUE);
	FINDRSCSETFUNC(mtnrsrc,"RANGE",set_range,NULL);
	addbtnrsrc(mtnrsrc,"SELECT RANGE SCREEN",TRUE,getscreenms,dfscnrsrc);
	addstdrsrc(mtnrsrc,"RANGE SCREEN",VARIABLETEXT,0,rangescreen,TRUE);
	addsctrsrc(mtnrsrc,"EXPRESSION",0,(expression!=NULL ? expression:""),TRUE);
	addbtnrsrc(mtnrsrc,"LOAD VALUE EXPRESSION",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(mtnrsrc,"LOAD OPERAND EXPRESSION",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(mtnrsrc,"LOAD GROUPER EXPRESSION",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(mtnrsrc,"LOAD CONTROL EXPRESSION",TRUE,loadcontrol,"EXPRESSION");
	if(chgfilesavl!=NULL) freeapplib(chgfilesavl);
	chgfilesavl=APPlibNEW();
	getchgfiles();
	addlstrsrc(mtnrsrc,"CHANGE FILES",0,TRUE,NULL,chgfilesavl->numlibs,
		&chgfilesavl->libs,NULL);
	addbtnrsrc(mtnrsrc,"ADD CHANGE ABOVE",TRUE,chgaddabove,mtnrsrc);
	addbtnrsrc(mtnrsrc,"ADD CHANGE BELOW",TRUE,chgaddbelow,mtnrsrc);
	addbtnrsrc(mtnrsrc,"COPY CHANGE",TRUE,DFchgcopy,mtnrsrc);
	addbtnrsrc(mtnrsrc,"DELETE CHANGE",TRUE,chgdelete,NULL);
	addbtnrsrc(mtnrsrc,"SELECT CHANGE",TRUE,chgedit,mtnrsrc);
	addbtnrsrc(mtnrsrc,"SAVE",TRUE,savemaintain,mainrsrc);
	addbtnrsrc(mtnrsrc,"QUIT",TRUE,quitchgstructtest,mainrsrc);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
/*lint -e611 */
	addbtnrsrc(mtnrsrc,"DFSCN",TRUE,dfscn,NULL);
	addbtnrsrc(mtnrsrc,"DFMENU",TRUE,dfmenu,NULL);
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
	addrfkwrsrc(dfscnrsrc,"SELECT",TRUE,okselectscreen,mtnrsrc);
	addrfkwrsrc(dfscnrsrc,"QUIT",TRUE,NULL_RSCS,NULL);
	addbtnrsrc(dfscnrsrc,"HELP",TRUE,screenhelp,dfscnrsrc);
	addbtnrsrc(dfscnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,dfscnrsrc);
	set_range(mtnrsrc);
	APPmakescrn(mtnrsrc,TRUE,quitchgstruct,mainrsrc,FALSE);
}
static void okchgstruct(RDArsrc *mainrsrc)
{
	int selected=0,selectedv=0;
	char *libx=NULL,*directory=NULL,*tmpx=NULL;

	changedvalues=FALSE;
	readwidget(mainrsrc,"DIRECTORY");
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"CHANGES AVAILABLE",&selectedv)) return;
	if(CHGSTRUCT!=NULL) FreeCHGstruct(CHGSTRUCT);
	if(RDAstrcmp(chgstructavl->libs[selectedv],"No Change Definitions")) 
	{
		libx=Rmalloc(RDAstrlen(directory)+RDAstrlen(modlst->libs[selected])+6);
		tmpx=adddashes(modlst->libs[selected]);
#ifndef WIN32
		sprintf(libx,"%s/%s.CHG",directory,tmpx);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\%s.CHG",directory,tmpx);
#endif
		CHGSTRUCT=CHGstructNEW(modlst->libs[selected],chgstructavl->libs[selectedv]);
		if(tmpx!=NULL) Rfree(tmpx);
		if(getCHGstruct(libx,CHGSTRUCT))
		{
			ERRORDIALOG("Cannot Read Change","Error in permissions, the user cannot read the power add definition.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
			if(libx!=NULL) Rfree(libx);
			if(CHGSTRUCT!=NULL) FreeCHGstruct(CHGSTRUCT);
			return;
		}
		if(libx!=NULL) Rfree(libx);
	}
	if(CHGSTRUCT==NULL)
	{
		CHGSTRUCT=CHGstructNEW(modlst->libs[selected],NULL);
	}
	maintainscreen(mainrsrc,CHGSTRUCT->module,CHGSTRUCT->name,CHGSTRUCT->desc,
		CHGSTRUCT->fieldname,CHGSTRUCT->type,CHGSTRUCT->len,
		CHGSTRUCT->range,CHGSTRUCT->range_screen,
		CHGSTRUCT->expression,CHGSTRUCT->numfiles,CHGSTRUCT->chgfiles);
}
static void newchgstruct(RDArsrc *mainrsrc)
{
	int selected=0;

	changedvalues=FALSE;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(CHGSTRUCT!=NULL) FreeCHGstruct(CHGSTRUCT);
	CHGSTRUCT=CHGstructNEW(modlst->libs[selected],NULL);
	maintainscreen(mainrsrc,CHGSTRUCT->module,CHGSTRUCT->name,CHGSTRUCT->desc,
		CHGSTRUCT->fieldname,CHGSTRUCT->type,CHGSTRUCT->len,
		CHGSTRUCT->range,CHGSTRUCT->range_screen,
		CHGSTRUCT->expression,CHGSTRUCT->numfiles,CHGSTRUCT->chgfiles);
}
static void quitdfmtn(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	if(modlst!=NULL) freeapplib(modlst);
	if(chgstructavl!=NULL) freeapplib(chgstructavl);
	if(mtntdefaults!=NULL) FreeRDAdefault(mtntdefaults);
	if(diravl!=NULL) freeapplib(diravl);
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
	if(FINDRSCGETINT(mainrsrc,"CHANGES AVAILABLE",&s)) return;
	modulename=adddashes(modlst->libs[m]);	
	libx=Rmalloc(RDAstrlen(directory)+RDAstrlen(modulename)+6);
#ifndef WIN32
	sprintf(libx,"%s/%s.CHG",directory,modulename);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.CHG",directory,modulename);
#endif
	if(deletelibbin(libx,chgstructavl->libs[s]))
	{
		ERRORDIALOG("Cannot Delete Change","The user cannot delete this power add.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
	} else doliblist1(mainrsrc);
	if(libx!=NULL) Rfree(libx);
	if(modulename!=NULL) Rfree(modulename);
	if(directory!=NULL) Rfree(directory);
}
static void deletemtntest(RDArsrc *mainrsrc)
{
	int s=0;

	if(!RDAstrcmp(chgstructavl->libs[s],"No Change Definitions")) return;
	WARNINGDIALOG("DELETE WARNING DIALOG SCREEN","DELETE CHANGE!","Are you sure you want to delete this power add?",deletemtn,NULL,FALSE,mainrsrc);
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
#ifndef WIN32
	sprintf(libx,"%s/%s.CHG",directory,tmpx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.CHG",directory,tmpx);
#endif
	if(tmpx!=NULL) Rfree(tmpx);
	if(FINDRSCGETINT(mainrsrc,"CHANGES AVAILABLE",&selected)) return;
	if(!RDAstrcmp(chgstructavl->libs[selected],"No Change Definitions"))
	{
		temp=NULL;
	} else {
		temp=getCHGstructdesc(libx,chgstructavl->libs[selected]);
	}
	if(libx!=NULL) Rfree(libx);
	if(directory!=NULL) Rfree(directory);
	if(!FINDRSCSETSTRING(mainrsrc,"DESCRIPTION",temp))
		updatersrc(mainrsrc,"DESCRIPTION");
	if(temp!=NULL) Rfree(temp);
}
static void printmodule(RDArsrc *mainrsrc,RDA_PFILE *fp)
{
	int x=0,selected=0,lines=1,pages=1;
	char *tmpx=NULL,*libx=NULL,*directory=NULL,*temp=NULL;
	CHGstruct *d=NULL;
	APPlib *mtntnames=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	libx=Rmalloc(RDAstrlen(directory)+RDAstrlen(modlst->libs[selected])+6);
	tmpx=adddashes(modlst->libs[selected]);
#ifndef WIN32
	sprintf(libx,"%s/%s.CHG",directory,tmpx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.CHG",directory,tmpx);
#endif
	if(directory!=NULL) Rfree(directory);
	mtntnames=getlibnames(libx,FALSE);
	if(mtntnames!=NULL)
	{
		temp=stralloc("XPERT CHANGE DEFINITION"); 
		printheader(fp,&lines,&pages,temp,tmpx);
		if(temp!=NULL) Rfree(temp);
		for(x=0;x<mtntnames->numlibs;++x)
		{
			d=CHGstructNEW(modlst->libs[selected],mtntnames->libs[x]);
			if(!getCHGstruct(libx,d))
			{
				printmtn1(fp,d,&lines,&pages);
				RDA_fprintf(fp,"\f\n");
			}
			if(d!=NULL) FreeCHGstruct(d);
		}
	} else {
		prterr("Error No Change Available.");
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
	if(FINDRSCGETINT(mainrsrc,"CHANGES AVAILABLE",&selm)) return;
	if(chgstructavl!=NULL) freeapplib(chgstructavl);
	libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(modlst->libs[selected])+6);
#ifndef WIN32
	sprintf(libx,"%s/%s.CHG",dirx,modlst->libs[selected]);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.CHG",dirx,modlst->libs[selected]);
#endif
	chgstructavl=getlibnames(libx,FALSE);
	if(dirx!=NULL) Rfree(dirx);
	if(libx!=NULL) Rfree(libx);
	if(chgstructavl==NULL)
	{
		chgstructavl=APPlibNEW();
		addAPPlib(chgstructavl,"No Change Definitions");
	}
	if(selm>=chgstructavl->numlibs) selm=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"CHANGES AVAILABLE",selm,chgstructavl))
		updatersrc(mainrsrc,"CHANGES AVAILABLE");
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
#ifndef WIN32
	sprintf(dirx,"%s/",directory);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\",directory);
#endif
	for(x=0;findfile(dirx,"*.CHG",&tmp,(int)x+1);++x) 
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
static void dochgstruct(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;
	APPlib *args=NULL,*envx=NULL;
	int dv=0;
	char *date=NULL;

	if(chgstructavl->numlibs==1 && !RDAstrcmp(chgstructavl->libs[0],"No Change Definitions")) return; 
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(mainrsrc,"CHANGES AVAILABLE",&selected)) return;
	args=APPlibNEW();
	addAPPlib(args,modlst->libs[selectedm]);
	addAPPlib(args,chgstructavl->libs[selected]);
	envx=APPlibNEW();
	memset(stemp,0,101);
	date=GETCURRENTDATE10();
	sprintf(stemp,"%.4s%.2s%.2s",&date[6],&date[0],&date[3]);
	dv=((atoi(stemp))-1225);
	if(date!=NULL) Rfree(date);
	sprintf(stemp,"DEV_LICENSE=%d",dv);	
	addAPPlib(envx,stemp);
	ADVExecuteProgram("dochg",args,envx);
	if(envx!=NULL) freeapplib(envx);
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

	if(InitializeSubsystems(argc,argv,module,"DEFINE CHANGE")) 
	{
		return;
	}
	DoHeaders();
	mainrsrc=RDArsrcNEW(module,"DEFINE CHANGES");
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
	addstdrsrc(mainrsrc,"DIRECTORY",VARIABLETEXT,0,diravl->libs[0],TRUE);
	if(defdir!=NULL) Rfree(defdir);
	FINDRSCLISTAPPlib(mainrsrc,"DIRECTORY LIST",w,diravl);
	FINDRSCSETFUNC(mainrsrc,"DIRECTORY",setdirlist1,NULL);
	x=0;
	if(modlst!=NULL) freeapplib(modlst); 
	modlst=APPlibNEW();
	addlstrsrc(mainrsrc,"MODULE LIST",&m,TRUE,doliblist1,
		modlst->numlibs,&modlst->libs,NULL);
	x=0;
	if(chgstructavl!=NULL) freeapplib(chgstructavl); 
	chgstructavl=APPlibNEW();
	addlstrsrc(mainrsrc,"CHANGES AVAILABLE",&s,TRUE,setvaluesx,
		chgstructavl->numlibs,&chgstructavl->libs,NULL);
	addsctrsrc(mainrsrc,"DESCRIPTION",0,NULL,FALSE);
	getdir1(mainrsrc);
	doliblist1(mainrsrc);
	if(argc>1 && modlst!=NULL)
	{
		for(m=0;m<modlst->numlibs;++m)
		{
			if(!RDAstrcmp(modlst->libs[m],argv[1])) break;
		}
		if(m<modlst->numlibs)
		{
			FINDRSCSETINT(mainrsrc,"MODULE LIST",m);
		}
		else
		{
			m=0;
			FINDRSCSETINT(mainrsrc,"MODULE LIST",m);
		}
		getdir1(mainrsrc);
	}
	if(argc>2 && chgstructavl!=NULL)
	{
		for(s=0;s<chgstructavl->numlibs;++s)
		{
			if(!RDAstrcmp(chgstructavl->libs[s],argv[2])) break;
		}
		if(s<chgstructavl->numlibs)
		{
			FINDRSCSETINT(mainrsrc,"CHANGES AVAILABLE",s);
		}
		else
		{
			s=0;
			FINDRSCSETINT(mainrsrc,"CHANGES AVAILABLE",s);
		}
	}
	addbtnrsrc(mainrsrc,"RUN CHANGE",TRUE,dochgstruct,NULL);
	addbtnrsrc(mainrsrc,"ADD",TRUE,newchgstruct,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,okchgstruct,NULL);
/*lint -e611 */
	addbtnrsrc(mainrsrc,"PRINT",TRUE,printdefinition,(void *)printmodulefp);
/*lint +e611 */
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitdfmtn,NULL);
	addbtnrsrc(mainrsrc,"DELETE",TRUE,deletemtntest,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	FINDRSCSETINPUTFOCUS(mainrsrc,"MODULE LIST");
	APPmakescrn(mainrsrc,TRUE,quitdfmtn,NULL,TRUE);
}
