/* dfadd.c - Define Power Add */
#ifndef WIN32
#define __NAM__ "dfadd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dfadd.exe"
#endif
#include <app.hpp>





#include <ldval.hpp>
#include <doheader.hpp>
#include <powadd.hpp>

/*CPP_OPART dfaddsub */
/*CPP_OPART dfaddwgt */
/*CPP_OPART dfaddwrt */
/*CPP_OPART dfaddscn */
/*ADDLIB mklib */
/*ADDLIB add */
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */



char *module="UTILITIES";
RDArsrc *mainrsrc=NULL,*mtnrsrc=NULL,*dfscnrsrc=NULL,*filersrc=NULL;
APPlib *modlst=NULL,*poweraddavl=NULL;
APPlib *dir_libs=NULL,*scr_defs=NULL,*file_defs=NULL,*diravl=NULL;
APPlib *buttonsavl=NULL,*fieldsavl=NULL,*btnfldavl=NULL,*keysavl=NULL;
APPlib *btntypes=NULL,*savetypes=NULL;
APPlib *filelist=NULL,*tmpargs=NULL,*modlstx=NULL;
APPlib *ExpTypes=NULL;
APPlib *fieldtypes=NULL;
APPlib *ConTypes=NULL,*BrowseTypes=NULL;
PowerAdd *POWERADD=NULL;
static void savemaintain(RDArsrc *,RDArsrc *);
static void quitpoweraddtest(RDArsrc *,RDArsrc *),setvaluesx(RDArsrc *);
static void quitpoweradd(RDArsrc *,RDArsrc *);
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
static char *snames[2]={"DEFINE LIST","ADD SCREEN"}; 
static int which_screen=0,numHoldButtons=0,numHoldButtonField=0;
static void doliblist1(RDArsrc *),getscreenlist(RDArsrc *);
static MaintainButton *HoldButtons=NULL;
static ButtonField *HoldButtonField=NULL;
static void dopoweradds(RDArsrc *);
static void dopoweradd(RDArsrc *);
RDAaddsub *tempsubord=NULL;
AddWrite *tempWrites=NULL;
AddEditWdgt *tempEditWdgts=NULL;
int tmpnumsub=0,tmpnumEditWdgts=0,tmpnumWrites=0;
APPlib *wrtavl=NULL,*wdgtavl=NULL,*subavl=NULL;
APPlib *filelst=NULL,*cmodlst=NULL,*subkeylist=NULL,*cnflds=NULL;
APPlib *econflds=NULL;
extern void savewdgt(void),wdgtedit(RDArsrc *,RDArsrc *),getWidgets(void);
extern void wdgtaddbelow(RDArsrc *,RDArsrc *),wdgtaddabove(RDArsrc *,RDArsrc *);
extern void wdgtdelete(RDArsrc *),wdgtcopy(RDArsrc *),wdgtwizard(RDArsrc *,RDArsrc *);
extern void wdgtmoveup(RDArsrc *),wdgtmovedown(RDArsrc *);
extern void savewrt(void),wrtcopy(RDArsrc *),wrtedit(RDArsrc *),getWrites(void);
extern void wrtaddbelow(RDArsrc *),wrtaddabove(RDArsrc *);
extern void wrtdelete(RDArsrc *),subcopy(RDArsrc *,RDArsrc *);
extern void wrtmoveup(RDArsrc *),wrtmovedown(RDArsrc *);
extern void savesub(void),subedit(RDArsrc *,RDArsrc *),getsubords(void);
extern void subaddbelow(RDArsrc *,RDArsrc *),subaddabove(RDArsrc *,RDArsrc *);
extern void subdelete(RDArsrc *);
extern void makeDefineListScreen(PowerAdd *);
extern void makePowerAddScreen(PowerAdd *);
extern void makeDefaultMenu(PowerAdd *);
static void makebuttonlist(void);
static APPlib *makebtntypeslist(void);
static void makebtnfldlist(void);
static void makespacebuttons(int);
APPlib *makefieldvallist2(RDAaddsub *,int,short,char *,char *);
APPlib *makefieldvallist(void);
void loadvalue(RDArsrc *,char *);
short use_wizard=TRUE;
char *orgmodulename=NULL;
char *orgname=NULL;

#ifdef SJSDIAG
extern void SEEEditWdgts(PowerAdd *);
#endif /* SJSDIAG */

APPlib *makefieldvallist2(RDAaddsub *search,int number,short dofields,char *submodname,char *subfilename)
{
	APPlib *tmp=NULL;
	int x,y,z;
	NRDfield *fields=NULL,*f;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short nofields=0,nokeys=0,engine=0;
	char *temp=NULL;
	RDAaddsub *s;
	AddEditWdgt *EditWdgt=NULL;

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
	if(POWERADD->EditWdgts!=NULL)
	{
		for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;
			++x,++EditWdgt)
		{
			temp=Rmalloc(RDAstrlen(EditWdgt->name)+3);
			sprintf(temp,"[%s]",EditWdgt->name);
			addAPPlibNoDuplicates(tmp,temp);
			if(temp!=NULL) Rfree(temp);
		}
	}
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
					addAPPlibNoDuplicates(tmp,temp);
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
	RDAaddsub *temp=NULL;
	AddEditWdgt *EditWdgt=NULL;

	tmp=APPlibNEW();
	tmprsrc=RDArsrcNEW(module,"TEMP");
	if(POWERADD->subord!=NULL)
	{
		for(x=0,temp=POWERADD->subord;x<POWERADD->subnum;++x,++temp)
		{
			addDFincvir(tmprsrc,temp->module,temp->filename,NULL,dumb);
		}
	}
	for(x=0,i=tmprsrc->incvir;x<tmprsrc->numvir;++x,++i)
	{
		addholdvalues(tmp,i->module,i->file);
	}
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(POWERADD->module)+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,POWERADD->module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,POWERADD->module);
#endif
	virflist=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	libx=NULL;
	if(virflist!=NULL)
	{
		for(y=0;y<virflist->numlibs;++y)
		{
			d=getDFvirtual(POWERADD->module,virflist->libs[y]);
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
	if(POWERADD->EditWdgts!=NULL)
	{
		for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;
			++x,++EditWdgt)
		{
			libx=Rmalloc(RDAstrlen(EditWdgt->name)+3);
			sprintf(libx,"[%s]",EditWdgt->name);
			addAPPlibNoDuplicates(tmp,libx);
			if(libx!=NULL) Rfree(libx);
		}
	}
	loadglobals(tmp);
	addAPPlibNoDuplicates(tmp,"[NEXT TRANSACTION NO]");
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
		if(x>=loadlist->numlibs) x=0;
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
#ifndef WIN32
	sprintf(libx,"%s/%s.SCN",dirx,modulx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.SCN",dirx,modulx);
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
static void getscreends(RDArsrc *mtnrsrc,RDArsrc *dfscnrsrc)
{
	which_screen=0;
	getscreen(mtnrsrc,dfscnrsrc);
}
static void getscreenms(RDArsrc *mtnrsrc,RDArsrc *dfscnrsrc)
{
	which_screen=1;
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
static void quitpoweradd(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	MaintainButton *b1;
	ButtonField *bf1;
	int x,y,z;
	RDAconfld *confld1;
	RDAeditfld *aconfld1;
	RDAaddsub *tempsub;
	AddWrite *Write=NULL;
	AddWriteField *WF=NULL;
	AddEditWdgt *EditWdgt=NULL;

	if(tempWrites!=NULL)
	{
		for(x=0,Write=tempWrites;x<tmpnumWrites;++x,++Write)
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
		Rfree(tempWrites);
		tempWrites=NULL;
		tmpnumWrites=0;
	} 
	if(tempEditWdgts!=NULL)
	{
		for(x=0,EditWdgt=tempEditWdgts;x<tmpnumEditWdgts;++x,++EditWdgt)
		{
			if(EditWdgt->name!=NULL) Rfree(EditWdgt->name);
			if(EditWdgt->expression!=NULL) Rfree(EditWdgt->expression);
			if(EditWdgt->module!=NULL) Rfree(EditWdgt->module);
			if(EditWdgt->filename!=NULL) Rfree(EditWdgt->filename);
			if(EditWdgt->subkeyname!=NULL) Rfree(EditWdgt->subkeyname);
			if(EditWdgt->acctrsc!=NULL) freeapplib(EditWdgt->acctrsc);
			if(EditWdgt->defrsc!=NULL) freeapplib(EditWdgt->defrsc);
			if(tempEditWdgts->confld!=NULL)
			{
				for(z=0,aconfld1=tempEditWdgts->confld;z<tempEditWdgts->num;
					++z,++aconfld1)
				{
					if(aconfld1->fldname!=NULL) Rfree(aconfld1->fldname);
					if(aconfld1->conname!=NULL) Rfree(aconfld1->conname);
				}
				Rfree(tempEditWdgts->confld);
			}
			tempEditWdgts->num=0;
			if(EditWdgt->mgt_expression!=NULL) Rfree(EditWdgt->mgt_expression);
			if(EditWdgt->assignname!=NULL) Rfree(EditWdgt->assignname);
			if(EditWdgt->id!=NULL) Rfree(EditWdgt->id);
			if(EditWdgt->override_expression!=NULL) Rfree(EditWdgt->override_expression);
		}
		Rfree(tempEditWdgts);
		tempEditWdgts=NULL;
		tmpnumEditWdgts=0;
	}
	if(tempsubord!=NULL)
	{
		for(x=0,tempsub=tempsubord;x<tmpnumsub;++x,++tempsub)
		{
			if(tempsub->module!=NULL) Rfree(tempsub->module);
			if(tempsub->filename!=NULL) Rfree(tempsub->filename);
			if(tempsub->subkeyname!=NULL) Rfree(tempsub->subkeyname);
			if(tempsub->cmodule!=NULL) Rfree(tempsub->cmodule);
			if(tempsub->cfilename!=NULL) Rfree(tempsub->cfilename);
			if(tempsub->browse_screen!=NULL) Rfree(tempsub->browse_screen);
			if(tempsub->button_name!=NULL) Rfree(tempsub->button_name);
			if(tempsub->define_list!=NULL) Rfree(tempsub->define_list);
			if(tempsub->search_browse!=NULL) Rfree(tempsub->search_browse);
			if(tempsub->expression!=NULL) Rfree(tempsub->expression);
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
			tempsub->BL=NULL;
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
			b1->type=0;
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
	if(dir_libs!=NULL) freeapplib(dir_libs);
	if(scr_defs!=NULL) freeapplib(scr_defs);
	if(file_defs!=NULL) freeapplib(file_defs);
	if(buttonsavl!=NULL) freeapplib(buttonsavl);
	if(subavl!=NULL) freeapplib(subavl);
	if(wrtavl!=NULL) freeapplib(wrtavl);
	if(wdgtavl!=NULL) freeapplib(wdgtavl);
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
static void quitpoweraddtest(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	readallwidgets(mtnrsrc);
	if(COMPARE_RSRCS(mtnrsrc,mtntdefaults,2) || changedvalues)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","There are changes to the POWER ADD DEFINITION.\nDo you want to Save these changes?",savemaintain,quitpoweradd,FALSE,2,mtnrsrc,mainrsrc,NULL);
	} else {
		quitpoweradd(mtnrsrc,mainrsrc);
	}
}
static void savemaintain(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	char *libname=NULL;
	char *directory=NULL,def_define_list=FALSE,def_add_screen=FALSE;
	char def_menu=FALSE;
	int x,y;
	MaintainButton *b1,*b2;
	ButtonField *bf1,*bf2;

	readallwidgets(mtnrsrc);
	FINDRSCGETCHAR(mtnrsrc,"DEFAULT DEFINE LIST",&def_define_list);
	FINDRSCGETCHAR(mtnrsrc,"DEFAULT ADD SCREEN",&def_add_screen);
	FINDRSCGETCHAR(mtnrsrc,"DEFAULT MENU",&def_menu);
	FINDRSCGETSTRING(mtnrsrc,"MODULE",&POWERADD->module);
	FINDRSCGETSTRING(mtnrsrc,"NAME",&POWERADD->name);
	if(FINDRSCGETSTRING(mtnrsrc,"DIRECTORY",&directory)) return;
	libname=Rmalloc(RDAstrlen(directory)+RDAstrlen(POWERADD->module)+6);
#ifndef WIN32
	sprintf(libname,"%s/%s.ADD",directory,POWERADD->module);
#endif
#ifdef WIN32
	sprintf(libname,"%s\\%s.ADD",directory,POWERADD->module);
#endif
	if(!isEMPTY(POWERADD->name))
	{
		FINDRSCGETSTRING(mtnrsrc,"DESCRIPTION",&POWERADD->desc);
		FINDRSCGETSTRING(mtnrsrc,"DEFINE LIST",&POWERADD->define_list);
		FINDRSCGETSTRING(mtnrsrc,"ADD SCREEN",&POWERADD->addscreen);
		FINDRSCGETSTRING(mtnrsrc,"SAVE ERROR",&POWERADD->save_expression);
		FINDRSCGETINT(mtnrsrc,"SAVE TYPE",&POWERADD->save_type);
		FINDRSCGETSTRING(mtnrsrc,"SAVE ERROR DESCRIPTION",
			&POWERADD->save_error_desc);
		FINDRSCGETSTRING(mtnrsrc,"SAVE WARNING",&POWERADD->save_warning);
		FINDRSCGETSTRING(mtnrsrc,"SAVE WARNING DESCRIPTION",
			&POWERADD->save_warning_desc);
		FINDRSCGETSTRING(mtnrsrc,"QUIT ERROR",&POWERADD->quit_error);
		FINDRSCGETSTRING(mtnrsrc,"QUIT ERROR DESCRIPTION",&POWERADD->quit_error_desc);
		FINDRSCGETSTRING(mtnrsrc,"QUIT WARNING",&POWERADD->quit_warning);
		FINDRSCGETSTRING(mtnrsrc,"QUIT WARNING DESCRIPTION",&POWERADD->quit_warning_desc);
		FINDRSCGETSTRING(mtnrsrc,"REPORTNAME",&POWERADD->reportname);
		if(POWERADD->buttons!=NULL)
		{
			for(x=0,b1=POWERADD->buttons;x<POWERADD->numbuttons;++x,++b1)
			{
				if(b1->progname!=NULL) Rfree(b1->progname);
				if(b1->modulename!=NULL) Rfree(b1->modulename);
				if(b1->filename!=NULL) Rfree(b1->filename);
				if(b1->keyname!=NULL) Rfree(b1->keyname);
				if(b1->listname!=NULL) Rfree(b1->listname);
				if(b1->buttonname!=NULL) Rfree(b1->buttonname);
				if(b1->expression!=NULL) Rfree(b1->expression);
				b1->type=0;
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
			Rfree(POWERADD->buttons);
			POWERADD->numbuttons=0;
		}
		if(HoldButtons!=NULL)
		{
			POWERADD->numbuttons=numHoldButtons;
			POWERADD->buttons=Rmalloc(numHoldButtons*sizeof(MaintainButton));
			for(x=0,b1=HoldButtons,b2=POWERADD->buttons;x<numHoldButtons;
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
		savesub();
		savewdgt();
#ifdef SJSDIAG
		SEEEditWdgts(POWERADD);
#endif /* SJSDIAG */
		savewrt();
		if(writePowerAdd(libname,POWERADD))
		{
			ERRORDIALOG("Cannot Write Power Add","The user cannot write the Power Add Definition.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
		}
		if(def_define_list==TRUE) makeDefineListScreen(POWERADD);
		if(def_add_screen==TRUE) makePowerAddScreen(POWERADD);
		if(def_menu==TRUE) makeDefaultMenu(POWERADD);
		FreePowerAdd(POWERADD);
		POWERADD=NULL;
	}
	if(libname!=NULL) Rfree(libname);
	if(directory!=NULL) Rfree(directory);
	doliblist1(mainrsrc);
	quitpoweradd(mtnrsrc,mainrsrc);
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
	if(!x)
	{
		for(x=0;x<fieldsavl->numlibs;++x)
		{
			if(strncmp(fieldsavl->libs[x],"GENERIC_SETUP_VARIABLE",22)) break;
		}
		if(x>=fieldsavl->numlibs) x=0;
	}	
	addlstrsrc(btnfldrsrc,"FIELD LIST",&x,TRUE,setenvname,
		fieldsavl->numlibs,&fieldsavl->libs,NULL);
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
		HoldButtons->type=0;
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
	addAPPlib(btntypes,"Button Wait for Completion");
	addAPPlib(btntypes,"Changed Value");
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
			l=RDAstrlen(button->rname)+RDAstrlen(button->ename)+34;
			s=Rmalloc(l);
			sprintf(s,"%2d Field: [%s] Environment: [%s]",(x+1),
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
static void changemodlist(RDArsrc *mainrsrc)
{
	int selected=0,selectedf=0;
	char *libx=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	FINDRSCGETINT(mainrsrc,"FILE LIST",&selectedf);
	if(filelist!=NULL) freeapplib(filelist); 
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlstx->libs[selected])+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modlstx->libs[selected]);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modlstx->libs[selected]);
#endif
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
		addbtnrsrc(argrsrc,"LOAD VALUE",TRUE,loadvalue,"ARGUMENT");
		addbtnrsrc(argrsrc,"LOAD OPERATOR",TRUE,loadoperand,"ARGUMENT");
		addbtnrsrc(argrsrc,"LOAD GROUPER",TRUE,loadgrouper,"ARGUMENT");
		addbtnrsrc(argrsrc,"LOAD CONTROL",TRUE,loadcontrol,"ARGUMENT");
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
	int x,seltype=0;
	ButtonField *b1,*b2;

	if(fieldsavl!=NULL) freeapplib(fieldsavl);
	fieldsavl=makefieldvallist();
	tmpargs=APPlibNEW();
	if(args!=NULL)
	{
		for(x=0;x<args->numlibs;++x) addAPPlib(tmpargs,args->libs[x]);
	}
	if(tmpargs->numlibs<1)
	{
		addAPPlib(tmpargs,"No Arguments");
	}
	if(fieldsavl->numlibs<1)
	{
		addAPPlib(fieldsavl,"No Fields Defined");
	}
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
	addstdrsrc(buttonrsrc,"PROGRAM NAME",VARIABLETEXT,0,(progname!=NULL?progname:""),TRUE);
	addstdrsrc(buttonrsrc,"LIST NAME",VARIABLETEXT,0,(listname!=NULL?listname:""),TRUE);
	addstdrsrc(buttonrsrc,"BUTTON NAME",VARIABLETEXT,0,(buttonname!=NULL?buttonname:""),TRUE);
	addsctrsrc(buttonrsrc,"EXPRESSION",0,(expression!=NULL?expression:""),TRUE);
	addbtnrsrc(buttonrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
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
			temp1->evalargs=FALSE;
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
				RDAstrlen(button->buttonname)+RDAstrlen(btntypes->libs[button->type])+84;
			s=Rmalloc(l);
			sprintf(s,"%2d Button: [%s] Program: [%s] Module: [%s] File: [%s] Keyname: [%s] List: [%s] Type: [%s]",(x+1),
				(button->buttonname!=NULL?button->buttonname:""),
				(button->progname!=NULL?button->progname:""),
				(button->modulename!=NULL?button->modulename:""),
				(button->filename!=NULL?button->filename:""),
				(button->keyname!=NULL?button->keyname:""),
				(button->listname!=NULL?button->listname:""),
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
		buttonscreen(mtnrsrc,mainrsrc,++x,b->progname,b->listname,
			b->buttonname,b->expression,b->type,b->num,
			b->bf,save_button_below,quitbuttonbelowtest,
			b->modulename,b->filename,b->keyname,b->evalargs,b->args);
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
static void printmtn1(RDA_PFILE *fp,PowerAdd *d,int *lines,int *pages)
{
	int x,y;
	MaintainButton *button=NULL;
	ButtonField *btnfld=NULL;
	RDAaddsub *tempsubord=NULL;
	RDAconfld *tempconfld=NULL;
	RDAeditfld *aconfld1=NULL;
	AddEditWdgt *EditWdgt=NULL;
	AddWrite *Write=NULL;
	AddWriteField *WriteField=NULL;
	char *temp=NULL;

	temp=stralloc("XPERT POWER ADD DEFINITION"); 
	printheader(fp,lines,pages,temp,d->module);
	if(temp!=NULL) Rfree(temp);

/*
	temp=Rmalloc(26+(d->name!=NULL?RDAstrlen(d->name):0)+(d->desc!=NULL?RDAstrlen(d->desc):0));
	sprintf(temp,"Name: [%s] Description: [%s]\r\n",(d->name!=NULL ? d->name:""),(d->desc!=NULL ? d->desc:""));
	fixmtnoutput(fp,temp,lines,pages,d->module);
	if(temp!=NULL) Rfree(temp);
*/
	RDA_fprintf(fp,"START POWER ADD DEFINITION : Name: [%s]\r\n",
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

	RDA_fprintf(fp,"  Define List: [%s]\r\n",(d->define_list!=NULL ? d->define_list:""));
	++*lines;
	checkheader(fp,lines,pages,d->module);
	RDA_fprintf(fp,"  Add Screen: [%s]\r\n",(d->addscreen!=NULL ? d->addscreen:""));
	++*lines;
	checkheader(fp,lines,pages,d->module);
	RDA_fprintf(fp,"  Audit Trail Report: [%s]\r\n",(d->reportname!=NULL ? d->reportname:""));
	++*lines;
	checkheader(fp,lines,pages,d->module);
	RDA_fprintf(fp,"  Save Type: [%d][%s] Level: [%d]\r\n",
		d->save_type,savetypes->libs[d->save_type],d->level);
	++*lines;
	checkheader(fp,lines,pages,d->module);
	if(d->EditWdgts!=NULL)
	{
		RDA_fprintf(fp,"  Edit Widgets: [%d]\r\n",d->numwdgts);
		++*lines;
		checkheader(fp,lines,pages,d->module);
		for(x=0,EditWdgt=d->EditWdgts;x<d->numwdgts;++x,++EditWdgt)
		{
			RDA_fprintf(fp,"    Name: [%s]  Length: [%d]\r\n",
				(EditWdgt->name!=NULL?EditWdgt->name:""),
				EditWdgt->len);
			++*lines;
			checkheader(fp,lines,pages,d->module);
			RDA_fprintf(fp,"      Field Type: [%d][%s] Expression Type: [%d][%s]\r\n",
				EditWdgt->type,fieldtypes->libs[EditWdgt->type],
				EditWdgt->etype,ExpTypes->libs[EditWdgt->etype]);
			++*lines;
			checkheader(fp,lines,pages,d->module);
			RDA_fprintf(fp,"      Default Expression: [%s]\r\n",
				(EditWdgt->expression!=NULL?EditWdgt->expression:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			RDA_fprintf(fp,"      Managed ID: [%s]\r\n",
				(EditWdgt->id_mgt==TRUE?"TRUE":"FALSE"));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			if(EditWdgt->id_mgt==TRUE)
			{
				RDA_fprintf(fp,"        Managed Field: [%s]\r\n",
					(EditWdgt->id!=NULL?EditWdgt->id:""));
				++*lines;
				checkheader(fp,lines,pages,d->module);
				RDA_fprintf(fp,"        Managed Assign: [%s]\r\n",
					(EditWdgt->assignname!=NULL?EditWdgt->assignname:""));
				++*lines;
				checkheader(fp,lines,pages,d->module);
				RDA_fprintf(fp,"        Managed Module/File/Key: [%s][%s][%s]\r\n",
					(EditWdgt->module!=NULL?EditWdgt->module:""),
					(EditWdgt->filename!=NULL?EditWdgt->filename:""),
					(EditWdgt->subkeyname!=NULL?EditWdgt->subkeyname:""));
				++*lines;
				checkheader(fp,lines,pages,d->module);
				if(tempEditWdgts->confld!=NULL)
				{
					for(y=0,aconfld1=tempEditWdgts->confld;y<tempEditWdgts->num;++y,++aconfld1)
					{
						RDA_fprintf(fp,"          Field: [%s] connects to [%s]\r\n",
							(aconfld1->fldname!=NULL?aconfld1->fldname:""),
							(aconfld1->conname!=NULL?aconfld1->conname:""));
						++*lines;
						checkheader(fp,lines,pages,d->module);
					}
				}
				RDA_fprintf(fp,"        Managed Expression: [%s]\r\n",
					(EditWdgt->mgt_expression!=NULL?EditWdgt->mgt_expression:""));
				++*lines;
				checkheader(fp,lines,pages,d->module);
				RDA_fprintf(fp,"        Override Expression: [%s]\r\n",
					(EditWdgt->override_expression!=NULL?EditWdgt->override_expression:""));
				++*lines;
				checkheader(fp,lines,pages,d->module);
			}
			if(EditWdgt->setaccttype==TRUE && EditWdgt->acctrsc!=NULL)
			{
				for(y=0;y<EditWdgt->acctrsc->numlibs;++y)
				{
					RDA_fprintf(fp,"      Set Account Type: [%s]\r\n",
						EditWdgt->acctrsc->libs[y]);
					++*lines;
					checkheader(fp,lines,pages,d->module);
				}
			}
			if(EditWdgt->setdeftype==TRUE && EditWdgt->defrsc!=NULL)
			{
				for(y=0;y<EditWdgt->defrsc->numlibs;++y)
				{
					RDA_fprintf(fp,"      Set Definition Type: [%s]\r\n",
						EditWdgt->defrsc->libs[y]);
					++*lines;
					checkheader(fp,lines,pages,d->module);
				}
			}
		}
	}
	if(d->subord!=NULL)
	{
		RDA_fprintf(fp,"  Subordinates: [%d]\r\n",d->subnum);
		++*lines;
		checkheader(fp,lines,pages,d->module);
		for(x=0,tempsubord=d->subord;x<d->subnum;++x,++tempsubord)
		{
			RDA_fprintf(fp,"    Module: [%s]  Filename: [%s]\r\n",
				(tempsubord->module!=NULL?tempsubord->module:""),
				(tempsubord->filename!=NULL?tempsubord->filename:"")),
			++*lines;
			checkheader(fp,lines,pages,d->module);
			RDA_fprintf(fp,"    Key Name: [%s] Occurance: [%d]\r\n",
				(tempsubord->subkeyname!=NULL?tempsubord->subkeyname:""),
				tempsubord->occurrence);
			++*lines;
			checkheader(fp,lines,pages,d->module);
			RDA_fprintf(fp,"    Connect Type: [%d][%s]\r\n",
				tempsubord->dtype,
				ConTypes->libs[tempsubord->dtype]);
			++*lines;
			checkheader(fp,lines,pages,d->module);
			if(tempsubord->browse==TRUE)
			{
				RDA_fprintf(fp,"      Browse Button Name: [%s] Smart [%s]\r\n",
					(tempsubord->button_name!=NULL?tempsubord->button_name:""),
					(tempsubord->smart_browse==TRUE?"TRUE":"FALSE"));
				++*lines;
				checkheader(fp,lines,pages,d->module);
				RDA_fprintf(fp,"      Browse Screen: [%s]\r\n",
					(tempsubord->browse_screen!=NULL?tempsubord->browse_screen:""));
				++*lines;
				checkheader(fp,lines,pages,d->module);
				RDA_fprintf(fp,"      Define List Screen: [%s]\r\n",
					(tempsubord->define_list!=NULL?tempsubord->define_list:""));
				++*lines;
				checkheader(fp,lines,pages,d->module);
				RDA_fprintf(fp,"      Search Browse Screen: [%s]\r\n",
					(tempsubord->search_browse!=NULL?tempsubord->search_browse:""));
				++*lines;
				checkheader(fp,lines,pages,d->module);
			}
/*
			RDA_fprintf(fp,"    Connect Module: [%s]  Filename: [%s]\r\n",
				(tempsubord->cmodule!=NULL?tempsubord->cmodule:""),
				(tempsubord->cfilename!=NULL?tempsubord->cfilename:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
*/
			RDA_fprintf(fp,"    Expression: [%s]\r\n",
				(tempsubord->expression!=NULL?tempsubord->expression:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			for(y=0,tempconfld=tempsubord->confld;y<tempsubord->num;++y,++tempconfld)
			{
				RDA_fprintf(fp,"      Field: [%s]  connects to: [%s]\r\n",
					(tempconfld->fldname!=NULL?tempconfld->fldname:""),
					(tempconfld->conname!=NULL?tempconfld->conname:""));
				++*lines;
				checkheader(fp,lines,pages,d->module);
			}
			checkheader(fp,lines,pages,d->module);
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
	if(d->save_expression!=NULL)
	{
		RDA_fprintf(fp,"    Save Error: [%s]\r\n",
			(d->save_expression!=NULL?d->save_expression:""));
		++*lines;
		if(d->save_error_desc!=NULL)
		{
			RDA_fprintf(fp,"    Save Error Desc: [%s]\r\n",
				(d->save_error_desc!=NULL?d->save_error_desc:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
		}
	}
	if(d->quit_warning!=NULL)
	{
		RDA_fprintf(fp,"    Quit Warning: [%s]\r\n",
			(d->quit_warning!=NULL?d->quit_warning:""));
		++*lines;
		checkheader(fp,lines,pages,d->module);
		if(d->quit_warning_desc!=NULL)
		{
			RDA_fprintf(fp,"    Quit Warning Desc: [%s]\r\n",
				(d->quit_warning_desc!=NULL?d->quit_warning_desc:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
		}
	}
	if(d->quit_error!=NULL)
	{
		RDA_fprintf(fp,"    Quit Error: [%s]\r\n",
			(d->quit_error!=NULL?d->quit_error:""));
		++*lines;
		if(d->quit_error_desc!=NULL)
		{
			RDA_fprintf(fp,"    Quit Error Desc: [%s]\r\n",
				(d->quit_error_desc!=NULL?d->quit_error_desc:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
		}
	}
	if(d->Writes!=NULL)
	{
		RDA_fprintf(fp,"  Power Writes: [%d]\r\n",d->numWrites);
		++*lines;
		checkheader(fp,lines,pages,d->module);
		for(x=0,Write=d->Writes;x<d->numWrites;++x,++Write)
		{
			RDA_fprintf(fp,"    Write Module/File: [%s][%s]\r\n",
				(Write->module!=NULL?Write->module:""),
				(Write->filename!=NULL?Write->filename:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			RDA_fprintf(fp,"    Write Expression: [%s]\r\n",
				(Write->expression!=NULL?Write->expression:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			if(Write->fields!=NULL)
			{
				for(y=0,WriteField=Write->fields;y<Write->numflds;++y,++WriteField)
				{
					RDA_fprintf(fp,"      Write to Field: [%s] Copy Type [%d][%s]\r\n",
						(WriteField->to_name!=NULL?WriteField->to_name:""),
						WriteField->copy_type,(WriteField->copy_type==0?"Copy Variable":"Evaluate Expression"));
					++*lines;
					checkheader(fp,lines,pages,d->module);
					RDA_fprintf(fp,"            from Field: [%s]\r\n",
						(WriteField->from_name!=NULL?WriteField->from_name:""));
					++*lines;
					checkheader(fp,lines,pages,d->module);
				}	
			}
		}
	}
	if(d->buttons!=NULL)
	{
		RDA_fprintf(fp,"  Buttons: [%d]\r\n",d->numbuttons);
		++*lines;
		checkheader(fp,lines,pages,d->module);
		for(x=0,button=d->buttons;x<d->numbuttons;++x,++button)
		{
			RDA_fprintf(fp,"    Program: [%s]\r\n",
				(button->progname!=NULL ? button->progname:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			RDA_fprintf(fp,"    List Name: [%s]\r\n",
				(button->listname!=NULL ? button->listname:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			RDA_fprintf(fp,"    Button Name: [%s]\r\n",
				(button->buttonname!=NULL ? button->buttonname:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			RDA_fprintf(fp,"    Button Type: [%s]\r\n",
				(btntypes->libs[button->type]!=NULL?btntypes->libs[button->type]:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			RDA_fprintf(fp,"    Module: [%s] Filename: [%s] Keyname: [%s]\r\n",
				(button->modulename!=NULL ? button->modulename:""),
				(button->filename!=NULL ? button->filename:""),
				(button->keyname!=NULL ? button->keyname:""));
			++*lines;
			checkheader(fp,lines,pages,d->module);
			if(button->bf!=NULL)
			{
				for(y=0,btnfld=button->bf;y<button->num;++y,++btnfld)
				{
					RDA_fprintf(fp,"      Field Name: [%s]\r\n",
						(btnfld->rname!=NULL ? btnfld->rname:""));
					++*lines;
					checkheader(fp,lines,pages,d->module);
					RDA_fprintf(fp,"        Environment Name: [%s]\r\n",
						(btnfld->ename!=NULL ? btnfld->ename:""));
					++*lines;
					checkheader(fp,lines,pages,d->module);
				}
			}
		}
	}
	RDA_fprintf(fp,"END POWER ADD DEFINITION [%s]\r\n",
		(d->name!=NULL?d->name:""));
	++*lines;
}
static void printmtn(RDArsrc *rsrc,RDA_PFILE *fp)
{
	int lines=0,pages=0;
/*
	char *temp=NULL;

	temp=stralloc("XPERT POWER ADD DEFINITION"); 
	printheader(fp,&lines,&pages,temp,POWERADD->module);
	if(temp!=NULL) Rfree(temp);
*/
	printmtn1(fp,POWERADD,&lines,&pages);
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
			temp=stralloc("XPERT POWER ADD DEFINITION"); 
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
	if(name!=NULL) Rfree(name);
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
	char *desc,
	char *define_list,char *addscreen,
	int numbuttons,MaintainButton *buttons,
	int numsubord,RDAaddsub *sub,
	char *save_e,char *save_ed,char *save_w,char *save_wd,
	char *reportname,char *quit_e,char *quit_ed,char *quit_w,char *quit_wd,
	int numWrites,AddWrite *Write,int numwdgts,
	AddEditWdgt *EditWdgt,int save_type)
{
	int w=0,x=0,z=0;
	RDArsrc *mtnrsrc=NULL;
	char *defdir=NULL;
	MaintainButton *b1,*b2;
	ButtonField *bf1,*bf2;
	RDAaddsub *tempsub1,*tempsub;
	RDAconfld *confld1,*confld2;
	RDAeditfld *aconfld1,*aconfld2;
	AddWrite *w1=NULL,*w2=NULL;
	AddWriteField *wf1=NULL,*wf2=NULL;
	AddEditWdgt *ew1=NULL,*ew2=NULL;
	
	if(Write!=NULL)
	{
		tmpnumWrites=numWrites;
		tempWrites=Rmalloc(tmpnumWrites*sizeof(AddWrite));
		for(x=0,w1=Write,w2=tempWrites;x<numWrites;++x,++w1,++w2)
		{
			if(w1->module!=NULL) w2->module=stralloc(w1->module);
				else w2->module=NULL;
			if(w1->filename!=NULL) w2->filename=stralloc(w1->filename);
				else w2->filename=NULL;
			if(w1->expression!=NULL) w2->expression=stralloc(w1->expression);
				else w2->expression=NULL;
			w2->numflds=w1->numflds;
			if(w1->fields!=NULL)
			{
				w2->fields=Rmalloc(w2->numflds*sizeof(AddWriteField));
				for(z=0,wf1=w1->fields,wf2=w2->fields;z<w1->numflds;++z,++wf1,++wf2)
				{
					if(wf1->to_name!=NULL) wf2->to_name=stralloc(wf1->to_name);
						else wf2->to_name=NULL;
					wf2->copy_type=wf1->copy_type;
					if(wf1->from_name!=NULL) wf2->from_name=stralloc(wf1->from_name);
						else wf2->from_name=NULL;
				}	
			} else {
				w2->numflds=0;
				w2->fields=NULL;
			}
		}
	} else {
		tmpnumWrites=0;
		tempWrites=NULL;
	}
	if(EditWdgt!=NULL)
	{
		tmpnumEditWdgts=numwdgts;
		tempEditWdgts=Rmalloc(numwdgts*sizeof(AddEditWdgt));
		for(x=0,ew1=EditWdgt,ew2=tempEditWdgts;x<numwdgts;++x,++ew1,++ew2)
		{
			if(ew1->name!=NULL) ew2->name=stralloc(ew1->name);
				else ew2->name=NULL;
			ew2->type=ew1->type;
			ew2->len=ew1->len;
			ew2->etype=ew1->etype;
			ew2->id_mgt=ew1->id_mgt;
			ew2->setaccttype=ew1->setaccttype;
			ew2->setdeftype=ew1->setdeftype;
			ew2->loadvirtual=ew1->loadvirtual;
			if(ew1->acctrsc!=NULL)
			{
				ew2->acctrsc=APPlibNEW();
				for(w=0;w<ew1->acctrsc->numlibs;++w)
				{
					addAPPlib(ew2->acctrsc,ew1->acctrsc->libs[w]);
				}
				freeapplib(ew1->acctrsc);
			} else ew2->acctrsc=NULL;
			if(ew1->defrsc!=NULL)
			{
				ew2->defrsc=APPlibNEW();
				for(w=0;w<ew1->defrsc->numlibs;++w)
				{
					addAPPlib(ew2->defrsc,ew1->defrsc->libs[w]);
				}
				freeapplib(ew1->defrsc);
			} else ew2->defrsc=NULL;
			if(ew1->module!=NULL) ew2->module=stralloc(ew1->module);
				else ew2->module=NULL;
			if(ew1->filename!=NULL) ew2->filename=stralloc(ew1->filename);
				else ew2->filename=NULL;
			if(ew1->subkeyname!=NULL) ew2->subkeyname=stralloc(ew1->subkeyname);
				else ew2->subkeyname=NULL;
			if(ew1->confld!=NULL)
			{
				ew2->num=ew1->num;
				ew2->confld=Rmalloc(ew1->num*sizeof(RDAeditfld));
				for(z=0,aconfld1=ew1->confld,aconfld2=ew2->confld;
					z<ew1->num;++z,++aconfld1,++aconfld2)
				{
					if(aconfld1->fldname!=NULL)
					{
						aconfld2->fldname=stralloc(aconfld1->fldname);
					} else aconfld2->fldname=NULL;
					if(aconfld1->conname!=NULL)
					{
						aconfld2->conname=stralloc(aconfld1->conname);
					} else aconfld2->conname=NULL;
				}
			} else {
				ew2->num=0;
			}
			if(ew1->mgt_expression!=NULL) ew2->mgt_expression=stralloc(ew1->mgt_expression);
				else ew2->mgt_expression=NULL;
			if(ew1->assignname!=NULL) ew2->assignname=stralloc(ew1->assignname);
				else ew2->assignname=NULL;
			if(ew1->id!=NULL) ew2->id=stralloc(ew1->id);
				else ew2->id=NULL;
			ew2->fileno=ew1->fileno;
			if(ew1->expression!=NULL) ew2->expression=stralloc(ew1->expression);
				else ew2->expression=NULL;
			ew2->loadvirtual=ew1->loadvirtual;
			if(ew1->override_expression!=NULL) ew2->override_expression=stralloc(ew1->override_expression);
				else ew2->override_expression=NULL;
		}
	} else {
		tmpnumEditWdgts=0;
		tempEditWdgts=NULL;
	}
	if(sub!=NULL)
	{
		tmpnumsub=numsubord;
		tempsubord=Rmalloc(tmpnumsub*sizeof(RDAaddsub));
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
			if(tempsub->subkeyname!=NULL)
			{
				tempsub1->subkeyname=stralloc(tempsub->subkeyname);
			} else tempsub1->subkeyname=NULL;
			tempsub1->dtype=tempsub->dtype;
			tempsub1->browse=tempsub->browse;
			if(tempsub->expression!=NULL)
			{
				tempsub1->expression=stralloc(tempsub->expression);
			} else tempsub1->expression=NULL;
			if(tempsub->browse_screen!=NULL)
			{
				tempsub1->browse_screen=stralloc(tempsub->browse_screen);
			} else tempsub1->browse_screen=NULL;
			if(tempsub->button_name!=NULL)
			{
				tempsub1->button_name=stralloc(tempsub->button_name);
			} else tempsub1->button_name=NULL;
			if(tempsub->define_list!=NULL)
			{
				tempsub1->define_list=stralloc(tempsub->define_list);
			} else tempsub1->define_list=NULL;
			if(tempsub->search_browse!=NULL)
			{
				tempsub1->search_browse=stralloc(tempsub->search_browse);
			} else tempsub1->search_browse=NULL;
			tempsub1->smart_browse=tempsub->smart_browse;
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
					confld2->inherit=confld1->inherit;
				}
			}
			if(tempsub->expression!=NULL)
			{
				tempsub1->expression=stralloc(tempsub->expression);
			} else tempsub1->expression=NULL;
			tempsub->BL=NULL;
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
	mtnrsrc=RDArsrcNEW(module,"EDIT POWER ADD");
	dfscnrsrc=RDArsrcNEW("UTILITIES","UTILITY MASTER SELECT SCREEN");
	FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&defdir);
	addstdrsrc(mtnrsrc,"DIRECTORY",VARIABLETEXT,0,defdir,TRUE);
	if(defdir!=NULL) Rfree(defdir);
	addstdrsrc(mtnrsrc,"MODULE",VARIABLETEXT,0,(modulx!=NULL ? modulx:""),
		TRUE);
	addstdrsrc(mtnrsrc,"NAME",VARIABLETEXT,0,(name!=NULL ? name:""),
		TRUE);
	addstdrsrc(mtnrsrc,"REPORTNAME",VARIABLETEXT,0,(reportname!=NULL ? reportname:""),TRUE);
	addsctrsrc(mtnrsrc,"DESCRIPTION",0,(desc!=NULL ? desc:""),TRUE);
	addbtnrsrc(mtnrsrc,"SELECT DEFINE LIST",TRUE,getscreends,dfscnrsrc);
	addstdrsrc(mtnrsrc,"DEFINE LIST",VARIABLETEXT,0,define_list,TRUE);
	addbtnrsrc(mtnrsrc,"DFSCN DEFINE LIST",TRUE,dfscn_screen,"DEFINE LIST");
	addstdrsrc(mtnrsrc,"DEFAULT DEFINE LIST",BOOLNS,1,&use_wizard,TRUE);
	addstdrsrc(mtnrsrc,"DEFAULT ADD SCREEN",BOOLNS,1,&use_wizard,TRUE);
	addbtnrsrc(mtnrsrc,"SELECT ADD SCREEN",TRUE,getscreenms,dfscnrsrc);
	addstdrsrc(mtnrsrc,"ADD SCREEN",VARIABLETEXT,0,addscreen,TRUE);
	addbtnrsrc(mtnrsrc,"DFSCN ADD SCREEN",TRUE,dfscn_screen,"ADD SCREEN");
	addstdrsrc(mtnrsrc,"DEFAULT MENU",BOOLNS,1,&use_wizard,TRUE);
	makebuttonlist();
	addlstrsrc(mtnrsrc,"BUTTONS AVAILABLE",0,TRUE,NULL,buttonsavl->numlibs,
		&buttonsavl->libs,NULL);
	addbtnrsrc(mtnrsrc,"ADD BUTTON ABOVE",TRUE,add_button_above,mainrsrc);
	addbtnrsrc(mtnrsrc,"ADD BUTTON BELOW",TRUE,add_button_below,mainrsrc);
	addbtnrsrc(mtnrsrc,"SELECT BUTTON",TRUE,select_button,mainrsrc);
	addbtnrsrc(mtnrsrc,"COPY BUTTON",TRUE,copy_button,mainrsrc);
	addbtnrsrc(mtnrsrc,"DELETE BUTTON",TRUE,delete_button,NULL);
	addlstrsrc(mtnrsrc,"SAVE TYPE",&save_type,TRUE,NULL,savetypes->numlibs,
		&savetypes->libs,NULL);
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
	addsctrsrc(mtnrsrc,"QUIT ERROR",0,quit_e,TRUE);
	addbtnrsrc(mtnrsrc,"LOAD VALUE QUIT ERROR",TRUE,loadvalue,"QUIT ERROR");
	addbtnrsrc(mtnrsrc,"LOAD OPERAND QUIT ERROR",TRUE,loadoperand,"QUIT ERROR");
	addbtnrsrc(mtnrsrc,"LOAD GROUPER QUIT ERROR",TRUE,loadgrouper,"QUIT ERROR");
	addbtnrsrc(mtnrsrc,"LOAD CONTROL QUIT ERROR",TRUE,loadcontrol,"QUIT ERROR");
	addsctrsrc(mtnrsrc,"QUIT ERROR DESCRIPTION",0,quit_ed,TRUE);
	addbtnrsrc(mtnrsrc,"LOAD VALUE QUIT ERROR DESCRIPTION",TRUE,loadvalue,
		"QUIT ERROR DESCRIPTION");
	addbtnrsrc(mtnrsrc,"LOAD OPERAND QUIT ERROR DESCRIPTION",TRUE,loadoperand,
		"QUIT ERROR DESCRIPTION");
	addbtnrsrc(mtnrsrc,"LOAD GROUPER QUIT ERROR DESCRIPTION",TRUE,loadgrouper,
		"QUIT ERROR DESCRIPTION");
	addbtnrsrc(mtnrsrc,"LOAD CONTROL QUIT ERROR DESCRIPTION",TRUE,loadcontrol,
		"QUIT ERROR DESCRIPTION");
	addsctrsrc(mtnrsrc,"QUIT WARNING",0,quit_w,TRUE);
	addbtnrsrc(mtnrsrc,"LOAD VALUE QUIT WARNING",TRUE,loadvalue,"QUIT WARNING");
	addbtnrsrc(mtnrsrc,"LOAD OPERAND QUIT WARNING",TRUE,loadoperand,"QUIT WARNING");
	addbtnrsrc(mtnrsrc,"LOAD GROUPER QUIT WARNING",TRUE,loadgrouper,"QUIT WARNING");
	addbtnrsrc(mtnrsrc,"LOAD CONTROL QUIT WARNING",TRUE,loadcontrol,"QUIT WARNING");
	addsctrsrc(mtnrsrc,"QUIT WARNING DESCRIPTION",0,quit_wd,TRUE);
	addbtnrsrc(mtnrsrc,"LOAD VALUE QUIT WARNING DESCRIPTION",TRUE,loadvalue,"QUIT WARNING DESCRIPTION");
	addbtnrsrc(mtnrsrc,"LOAD OPERAND QUIT WARNING DESCRIPTION",TRUE,loadoperand,"QUIT WARNING DESCRIPTION");
	addbtnrsrc(mtnrsrc,"LOAD GROUPER QUIT WARNING DESCRIPTION",TRUE,loadgrouper,"QUIT WARNING DESCRIPTION");
	addbtnrsrc(mtnrsrc,"LOAD CONTROL QUIT WARNING DESCRIPTION",TRUE,loadcontrol,"QUIT WARNING DESCRIPTION");
	getsubords();
	addlstrsrc(mtnrsrc,"SUPPORTING FILES",0,TRUE,NULL,subavl->numlibs,
		&subavl->libs,NULL);
	addbtnrsrc(mtnrsrc,"ADD SUPPORTING ABOVE",TRUE,subaddabove,mtnrsrc);
	addbtnrsrc(mtnrsrc,"ADD SUPPORTING BELOW",TRUE,subaddbelow,mtnrsrc);
	addbtnrsrc(mtnrsrc,"DELETE SUPPORTING",TRUE,subdelete,NULL);
	addbtnrsrc(mtnrsrc,"SELECT SUPPORTING",TRUE,subedit,mtnrsrc);
	addbtnrsrc(mtnrsrc,"COPY SUPPORTING",TRUE,subcopy,mtnrsrc);
	if(wrtavl!=NULL) freeapplib(wrtavl);
	wrtavl=APPlibNEW();
	getWrites();
	addlstrsrc(mtnrsrc,"WRITE LIST",0,TRUE,NULL,wrtavl->numlibs,
		&wrtavl->libs,NULL);
	addbtnrsrc(mtnrsrc,"MOVE WRITE UP",TRUE,wrtmoveup,NULL);
	addbtnrsrc(mtnrsrc,"MOVE WRITE DOWN",TRUE,wrtmovedown,NULL);
	addbtnrsrc(mtnrsrc,"ADD WRITE ABOVE",TRUE,wrtaddabove,NULL);
	addbtnrsrc(mtnrsrc,"ADD WRITE BELOW",TRUE,wrtaddbelow,NULL);
	addbtnrsrc(mtnrsrc,"DELETE WRITE",TRUE,wrtdelete,NULL);
	addbtnrsrc(mtnrsrc,"COPY WRITE",TRUE,wrtcopy,NULL);
	addbtnrsrc(mtnrsrc,"SELECT WRITE",TRUE,wrtedit,NULL);
	if(wdgtavl!=NULL) freeapplib(wdgtavl);
	wdgtavl=APPlibNEW();
	getWidgets();
	addlstrsrc(mtnrsrc,"EDIT WIDGETS",0,TRUE,NULL,wdgtavl->numlibs,
		&wdgtavl->libs,NULL);
	addbtnrsrc(mtnrsrc,"WIZARD",TRUE,wdgtwizard,mtnrsrc);
	if(use_wizard==FALSE)
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"WIZARD",FALSE);
	}
	addbtnrsrc(mtnrsrc,"ADD WIDGET ABOVE",TRUE,wdgtaddabove,mtnrsrc);
	addbtnrsrc(mtnrsrc,"ADD WIDGET BELOW",TRUE,wdgtaddbelow,mtnrsrc);
	addbtnrsrc(mtnrsrc,"MOVE WIDGET UP",TRUE,wdgtmoveup,NULL);
	addbtnrsrc(mtnrsrc,"MOVE WIDGET DOWN",TRUE,wdgtmovedown,NULL);
	addbtnrsrc(mtnrsrc,"DELETE WIDGET",TRUE,wdgtdelete,NULL);
	addbtnrsrc(mtnrsrc,"COPY WIDGET",TRUE,wdgtcopy,NULL);
	addbtnrsrc(mtnrsrc,"SELECT WIDGET",TRUE,wdgtedit,mtnrsrc);
	addbtnrsrc(mtnrsrc,"DFSCN",TRUE,dfscn,NULL);
	addbtnrsrc(mtnrsrc,"DFMENU",TRUE,dfmenu,NULL);
	addbtnrsrc(mtnrsrc,"DFLIST",TRUE,dflist,NULL);
	addbtnrsrc(mtnrsrc,"DFVIR",TRUE,dfvir,NULL);
	addbtnrsrc(mtnrsrc,"DFLOAD",TRUE,dfload,NULL);
	addbtnrsrc(mtnrsrc,"SAVE",TRUE,savemaintain,mainrsrc);
	addbtnrsrc(mtnrsrc,"QUIT",TRUE,quitpoweraddtest,mainrsrc);
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
	addrfkwrsrc(dfscnrsrc,"SELECT",TRUE,okselectscreen,mtnrsrc);
	addrfkwrsrc(dfscnrsrc,"QUIT",TRUE,NULL_RSCS,NULL);
	addbtnrsrc(dfscnrsrc,"HELP",TRUE,screenhelp,dfscnrsrc);
	addbtnrsrc(dfscnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,dfscnrsrc);
	APPmakescrn(mtnrsrc,TRUE,quitpoweradd,mainrsrc,FALSE);
}
static void okpoweradd(RDArsrc *mainrsrc)
{
	int selected=0,selectedv=0;
	char *libx=NULL,*directory=NULL,*tmpx=NULL;

	changedvalues=FALSE;changedbtnfld=FALSE;changedbutton=FALSE;
	changedargs=FALSE;
	readwidget(mainrsrc,"DIRECTORY");
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"POWER ADDS AVAILABLE",&selectedv)) return;
	if(POWERADD!=NULL) FreePowerAdd(POWERADD);
	if(RDAstrcmp(poweraddavl->libs[selectedv],"No Power Add Definitions")) 
	{
		libx=Rmalloc(RDAstrlen(directory)+RDAstrlen(modlst->libs[selected])+6);
		tmpx=adddashes(modlst->libs[selected]);
#ifndef WIN32
		sprintf(libx,"%s/%s.ADD",directory,tmpx);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\%s.ADD",directory,tmpx);
#endif
		POWERADD=PowerAddNew(modlst->libs[selected],poweraddavl->libs[selectedv]);
		if(tmpx!=NULL) Rfree(tmpx);
		if(getPowerAdd(libx,POWERADD))
		{
			ERRORDIALOG("Cannot Read Power Add","Error in permissions, the user cannot read the power add definition.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
			if(libx!=NULL) Rfree(libx);
			if(POWERADD!=NULL) FreePowerAdd(POWERADD);
			return;
		}
		if(libx!=NULL) Rfree(libx);
	}
	if(POWERADD==NULL)
	{
		POWERADD=PowerAddNew(modlst->libs[selected],NULL);
	}
#ifdef SJSDIAG
	SEEEditWdgts(POWERADD);
#endif /* SJSDIAG */
	use_wizard=FALSE;
	maintainscreen(mainrsrc,POWERADD->module,POWERADD->name,POWERADD->desc,
		POWERADD->define_list,POWERADD->addscreen,
		POWERADD->numbuttons,POWERADD->buttons,
		POWERADD->subnum,POWERADD->subord,
		POWERADD->save_expression,
		POWERADD->save_error_desc,POWERADD->save_warning,
		POWERADD->save_warning_desc,POWERADD->reportname,
		POWERADD->quit_error,POWERADD->quit_error_desc,
		POWERADD->quit_warning,POWERADD->quit_warning_desc,
		POWERADD->numWrites,POWERADD->Writes,POWERADD->numwdgts,
		POWERADD->EditWdgts,POWERADD->save_type);
}
static void newpoweradd(RDArsrc *mainrsrc)
{
	int selected=0;

	changedvalues=FALSE;changedbtnfld=FALSE;changedbutton=FALSE;
	changedargs=FALSE;
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(POWERADD!=NULL) FreePowerAdd(POWERADD);
	POWERADD=PowerAddNew(modlst->libs[selected],NULL);
	POWERADD->save_type=1;
	use_wizard=TRUE;
	maintainscreen(mainrsrc,POWERADD->module,POWERADD->name,POWERADD->desc,
		POWERADD->define_list,POWERADD->addscreen,
		POWERADD->numbuttons,POWERADD->buttons,
		POWERADD->subnum,POWERADD->subord,
		POWERADD->save_expression,
		POWERADD->save_error_desc,POWERADD->save_warning,
		POWERADD->save_warning_desc,POWERADD->reportname,
		POWERADD->quit_error,POWERADD->quit_error_desc,
		POWERADD->quit_warning,POWERADD->quit_warning_desc,
		POWERADD->numWrites,POWERADD->Writes,POWERADD->numwdgts,
		POWERADD->EditWdgts,POWERADD->save_type);
}
static void quitdfmtn(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	if(modlst!=NULL) freeapplib(modlst);
	if(poweraddavl!=NULL) freeapplib(poweraddavl);
	if(mtntdefaults!=NULL) FreeRDAdefault(mtntdefaults);
	if(diravl!=NULL) freeapplib(diravl);
	if(ExpTypes!=NULL) freeapplib(ExpTypes);
	if(fieldtypes!=NULL) freeapplib(fieldtypes);
	if(ConTypes!=NULL) freeapplib(ConTypes);
	if(BrowseTypes!=NULL) freeapplib(BrowseTypes);
	if(savetypes!=NULL) freeapplib(savetypes);
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
	if(FINDRSCGETINT(mainrsrc,"POWER ADDS AVAILABLE",&s)) return;
	modulename=adddashes(modlst->libs[m]);	
	libx=Rmalloc(RDAstrlen(directory)+RDAstrlen(modulename)+6);
#ifndef WIN32
	sprintf(libx,"%s/%s.ADD",directory,modulename);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.ADD",directory,modulename);
#endif
	if(deletelibbin(libx,poweraddavl->libs[s]))
	{
		ERRORDIALOG("Cannot Delete Power Add","The user cannot delete this power add.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
	} else doliblist1(mainrsrc);
	if(libx!=NULL) Rfree(libx);
	if(modulename!=NULL) Rfree(modulename);
	if(directory!=NULL) Rfree(directory);
}
static void deletemtntest(RDArsrc *mainrsrc)
{
	int s=0;

	if(!RDAstrcmp(poweraddavl->libs[s],"No Power Add Definitions")) return;
	WARNINGDIALOG("DELETE WARNING DIALOG SCREEN","DELETE POWER ADD!","Are you sure you want to delete this power add?",deletemtn,NULL,FALSE,mainrsrc);
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
	sprintf(libx,"%s/%s.ADD",directory,tmpx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.ADD",directory,tmpx);
#endif
	if(tmpx!=NULL) Rfree(tmpx);
	if(FINDRSCGETINT(mainrsrc,"POWER ADDS AVAILABLE",&selected)) return;
	if(!RDAstrcmp(poweraddavl->libs[selected],"No Power Add Definitions"))
	{
		temp=NULL;
	} else {
		temp=getPowerAdddesc(libx,poweraddavl->libs[selected]);
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
/*
	char *temp=NULL;
*/
	int lines=0,pages=0;
	PowerAdd *d=NULL;
	APPlib *mtntnames=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	libx=Rmalloc(RDAstrlen(directory)+RDAstrlen(modlst->libs[selected])+6);
	tmpx=adddashes(modlst->libs[selected]);
#ifndef WIN32
	sprintf(libx,"%s/%s.ADD",directory,tmpx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.ADD",directory,tmpx);
#endif
	if(directory!=NULL) Rfree(directory);
	mtntnames=getlibnames(libx,FALSE);
	if(mtntnames!=NULL)
	{
/*
		temp=stralloc("XPERT POWER ADD DEFINITION"); 
		printheader(fp,&lines,&pages,temp,tmpx);
		if(temp!=NULL) Rfree(temp);
*/
		for(x=0;x<mtntnames->numlibs;++x)
		{
			d=PowerAddNew(modlst->libs[selected],mtntnames->libs[x]);
			if(!getPowerAdd(libx,d))
			{
				printmtn1(fp,d,&lines,&pages);
				RDA_fprintf(fp,"\f\r\n");
			}
			if(d!=NULL) FreePowerAdd(d);
		}
	} else {
		prterr("Error No Power Add Available.");
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
	if(FINDRSCGETINT(mainrsrc,"POWER ADDS AVAILABLE",&selm)) return;
	if(poweraddavl!=NULL) freeapplib(poweraddavl);
	libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(modlst->libs[selected])+6);
#ifndef WIN32
	sprintf(libx,"%s/%s.ADD",dirx,modlst->libs[selected]);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.ADD",dirx,modlst->libs[selected]);
#endif
	poweraddavl=getlibnames(libx,FALSE);
	if(dirx!=NULL) Rfree(dirx);
	if(libx!=NULL) Rfree(libx);
	if(poweraddavl==NULL)
	{
		poweraddavl=APPlibNEW();
		addAPPlib(poweraddavl,"No Power Add Definitions");
	}
	if(selm>=poweraddavl->numlibs) selm=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"POWER ADDS AVAILABLE",selm,poweraddavl))
		updatersrc(mainrsrc,"POWER ADDS AVAILABLE");
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
	for(x=0;findfile(dirx,"*.ADD",&tmp,(int)x+1);++x) 
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
static void dopoweradds(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;
	APPlib *args=NULL;

	if(poweraddavl->numlibs==1 && !RDAstrcmp(poweraddavl->libs[0],"No Power Add Definitions")) return; 
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(mainrsrc,"POWER ADDS AVAILABLE",&selected)) return;
	args=APPlibNEW();
	addAPPlib(args,modlst->libs[selectedm]);
	addAPPlib(args,poweraddavl->libs[selected]);
	ExecuteProgram("doadds",args);
	if(args!=NULL) freeapplib(args);
}
static void dopoweradd(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;
	APPlib *args=NULL;

	if(poweraddavl->numlibs==1 && !RDAstrcmp(poweraddavl->libs[0],"No Power Add Definitions")) return; 
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(mainrsrc,"POWER ADDS AVAILABLE",&selected)) return;
	args=APPlibNEW();
	addAPPlib(args,modlst->libs[selectedm]);
	addAPPlib(args,poweraddavl->libs[selected]);
	ExecuteProgram("doadd",args);
	if(args!=NULL) freeapplib(args);
}
APPlib *makeetypes(void)
{
	if(ExpTypes!=NULL) freeapplib(ExpTypes);
	ExpTypes=APPlibNEW();
        addAPPlib(ExpTypes,"Initial Default Computation");
        addAPPlib(ExpTypes,"Default-Reset Computation");
        addAPPlib(ExpTypes,"Changed Value Computation");
        addAPPlib(ExpTypes,"Initial / Reset Default Computation");
        addAPPlib(ExpTypes,"Write Only Computation");
        addAPPlib(ExpTypes,"Environment / Change Value");
	return(ExpTypes);
}
APPlib *makefldtypes(void)
{
	int x=0;

        if(fieldtypes!=NULL) freeapplib(fieldtypes);
        fieldtypes=APPlibNEW();
        for(x=0;x<28;++x)
        {
                addAPPlib(fieldtypes,standardfieldtypes[x]);
        }
	return(fieldtypes);
}
APPlib *makeConTypes(void)
{
        if(ConTypes!=NULL) freeapplib(ConTypes);
        ConTypes=APPlibNEW();
        addAPPlib(ConTypes,"Equal to and Save Check");
        addAPPlib(ConTypes,"Equal to");
        addAPPlib(ConTypes,"Greater Than or Equal to and Save Check");
        addAPPlib(ConTypes,"Greater Than or Equal to");
        addAPPlib(ConTypes,"Greater Than and Save Check");
        addAPPlib(ConTypes,"Greater Than");
        addAPPlib(ConTypes,"Less Than or Equal to and Save Check");
        addAPPlib(ConTypes,"Less Than or Equal to");
        addAPPlib(ConTypes,"Less Than and Save Check");
        addAPPlib(ConTypes,"Less Than");
	return(ConTypes);
}
APPlib *makesavetypes(void)
{
	if(savetypes!=NULL) freeapplib(savetypes);
	savetypes=APPlibNEW();
	addAPPlib(savetypes,"Power Save");
	addAPPlib(savetypes,"Fast Save");
	addAPPlib(savetypes,"Fast Save & Reset");
	addAPPlib(savetypes,"Fast Save & Quit");
	addAPPlib(savetypes,"Power Save & Reset");
	addAPPlib(savetypes,"Power Save & Quit");
	return(savetypes);
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

	if(InitializeSubsystems(argc,argv,module,"DEFINE POWER ADD")) 
	{
		return;
	}
	DoHeaders();
	mainrsrc=RDArsrcNEW(module,"DEFINE POWER ADDS");
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
	if(modlst!=NULL) freeapplib(modlst); 
	modlst=APPlibNEW();
	addlstrsrc(mainrsrc,"MODULE LIST",&x,TRUE,doliblist1,
		modlst->numlibs,&modlst->libs,NULL);
	x=0;
	if(poweraddavl!=NULL) freeapplib(poweraddavl); 
	poweraddavl=APPlibNEW();
	addlstrsrc(mainrsrc,"POWER ADDS AVAILABLE",&x,TRUE,setvaluesx,
		poweraddavl->numlibs,&poweraddavl->libs,NULL);
	setdirlist1(mainrsrc);
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
			m = 0;
			FINDRSCSETINT(mainrsrc,"MODULE LIST",m);
		}
		doliblist1(mainrsrc);
	}
	if(argc>2 && poweraddavl!=NULL)
	{
		for(s=0;s<poweraddavl->numlibs;++s)
		{
			if(!RDAstrcmp(poweraddavl->libs[s],argv[2])) break;
		}
		if(s<poweraddavl->numlibs)
		{
			FINDRSCSETINT(mainrsrc,"POWER ADDS AVAILABLE",s);
		}
		else
		{
			s = 0;
			FINDRSCSETINT(mainrsrc,"POWER ADDS AVAILABLE",s);
		}
		setvaluesx(mainrsrc);
	}
	addsctrsrc(mainrsrc,"DESCRIPTION",0,NULL,FALSE);
	addbtnrsrc(mainrsrc,"RUN POWER ADD",TRUE,dopoweradd,NULL);
	addbtnrsrc(mainrsrc,"MULTIPOWERADD",TRUE,dopoweradds,NULL);
	addbtnrsrc(mainrsrc,"ADD",TRUE,newpoweradd,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,okpoweradd,NULL);
/*lint -e611 */
	addbtnrsrc(mainrsrc,"PRINT",TRUE,printdefinition,(void *)printmodulefp);
/*lint +e611 */
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitdfmtn,NULL);
	addbtnrsrc(mainrsrc,"DELETE",TRUE,deletemtntest,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	ExpTypes=makeetypes();
	fieldtypes=makefldtypes();
	ConTypes=makeConTypes();
	BrowseTypes=APPlibNEW();
	addAPPlib(BrowseTypes,"Standard Browse");
	addAPPlib(BrowseTypes,"Smart Browse");
	addAPPlib(BrowseTypes,"Search Browse");
	addAPPlib(BrowseTypes,"Lexical Pattern Match");
	savetypes=makesavetypes();
	btntypes=makebtntypeslist();
	FINDRSCSETINPUTFOCUS(mainrsrc,"MODULE LIST");
	APPmakescrn(mainrsrc,TRUE,quitdfmtn,NULL,TRUE);
}
