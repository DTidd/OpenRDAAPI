/* dfmtnsup - Define Search Files section of Define Maintain Masters */
#include <cstdio>





#include <mix.hpp>
#include <rptgen.hpp>
#include <ldval.hpp>

extern APPlib *makefieldvallist2mtn(RDAsearch *,int,int,short,char *,char *);
extern MaintainMaster *MTNMSTR;
extern RDArsrc *mtnrsrc;
extern char *module,changedvalues;
static RDAdefault *msuppdefaults=NULL,*econdefaults=NULL;
static char changesupports=FALSE;
static RDAconfld*holdconfld=NULL;
extern RDAsearch *tempsupport;
extern int tmpnumsupp;
extern APPlib *modlstxmtn,*suppavlmtn,*filelstmtn,*cmodlstmtn,*subkeylistmtn,*cnfldsmtn,*econfldsmtn;
extern APPlib *ctypes,*BrowseTypes;
extern char *contypemtn[];
extern void setconfldlist(RDArsrc *,APPlib *,char *);
extern void addholdvaluessup(APPlib *,char *,char *);
extern void makeSearchBrowseScreen(MaintainMaster *,RDAsearch *);
extern void makeBrowseScreen(MaintainMaster *,RDAsearch *);
extern void makeDefineListScreen(MaintainMaster *,RDAsearch *);
static int holdnum=0;
static int ptrue=TRUE,pfalse=FALSE;
static void loadvalue3mtn(RDArsrc *,char *);
void savesupp(RDArsrc *,MaintainMaster *),suppedit(RDArsrc *,RDArsrc *),suppdelete(RDArsrc *);
void suppaddabove(RDArsrc *,RDArsrc *),suppaddbelow(RDArsrc *,RDArsrc *);
void getsupports(void);
static void changectype(RDArsrc *);

static void loadvalue3mtn(RDArsrc *r,char *fieldname)
{
	int x,selected=(-1);
	int selectedm=0,selectedf=0;
	LoadValue *load;
	RDArsrc *ldvalrsrc1=NULL;
	char *modname=NULL,*filename=NULL;

	ldvalrsrc1=RDArsrcNEW("RPTGEN","LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc1,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	FINDRSCGETINT(r,"NUMBER",&selected);
	FINDRSCGETINT(r,"MODULE",&selectedm);
	FINDRSCGETINT(r,"FILENAME",&selectedf);
	if(RDAstrcmp(modlstxmtn->libs[selectedm],"Contains No Database Libraries"))
	{
		modname=stralloc(modlstxmtn->libs[selectedm]);
	}
	if(RDAstrcmp(filelstmtn->libs[selectedf],"Contains No File Definitions"))
	{
		filename=stralloc(filelstmtn->libs[selectedf]);
	}
	loadlist=makefieldvallist2mtn(tempsupport,tmpnumsupp,(selected-1),TRUE,modname,filename);
	if(modname!=NULL) Rfree(modname);
	if(filename!=NULL) Rfree(filename);
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
	addlstrsrc(ldvalrsrc1,"LOAD LIST",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addrfkwrsrc(ldvalrsrc1,"SELECT",TRUE,okldval,load);
	addrfkwrsrc(ldvalrsrc1,"QUIT",TRUE,quitldval,load);
	addbtnrsrc(ldvalrsrc1,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ldvalrsrc1,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(ldvalrsrc1,TRUE,quitldval,load,FALSE);
}
void getsupports(void)
{
	int x,len=0;
	RDAsearch *s=NULL;
	char *tmp=NULL;

	if(suppavlmtn!=NULL) freeapplib(suppavlmtn);
	suppavlmtn=APPlibNEW();
	if(tempsupport!=NULL)
	{
		for(x=0,s=tempsupport;x<tmpnumsupp;++x,++s)
		{
/*
			len=RDAstrlen(s->module)+RDAstrlen(s->filename)+7+RDAstrlen(s->cmodule)+RDAstrlen(s->cfilename)+RDAstrlen(s->subkeyname)+RDAstrlen(contypemtn[s->ctype]);
*/
			len=(RDAstrlen(s->module)+RDAstrlen(s->filename)+(s->browse==TRUE?6:0)+3)+10;
			if(tmp!=NULL) 
			{
				tmp=Rrealloc(tmp,len);
			} else {
				tmp=Rmalloc(len);
			}
			memset(tmp,0,len);
			sprintf(tmp,"%2d %s %s %s",x+1,
				(s->module!=NULL?s->module:""),
				(s->filename!=NULL?s->filename:""),
				(s->browse==TRUE?"Browse":""));
			addAPPlib(suppavlmtn,tmp);
		}
	}
	if(suppavlmtn->numlibs<1)
	{
		addAPPlib(suppavlmtn,"No Files Defined");
	}
}
static void quitmsupp(RDArsrc *msupprsrc,RDArsrc *supprsrc)
{
	int x;
	RDAconfld *cf;

	if(msupprsrc!=NULL)
	{
		killwindow(msupprsrc);
		free_rsrc(msupprsrc);
	}
	if(modlstxmtn!=NULL) freeapplib(modlstxmtn);
	if(filelstmtn!=NULL) freeapplib(filelstmtn);
	if(cmodlstmtn!=NULL) freeapplib(cmodlstmtn);
	if(cnfldsmtn!=NULL) freeapplib(cnfldsmtn);
	if(ctypes!=NULL) freeapplib(ctypes);
	if(BrowseTypes!=NULL) freeapplib(BrowseTypes);
	if(holdconfld!=NULL)
	{
		for(x=0,cf=holdconfld;x<holdnum;++x,++cf)
		{
			if(cf->fldname!=NULL) Rfree(cf->fldname);
			if(cf->conname!=NULL) Rfree(cf->conname);
		}
		Rfree(holdconfld);
		holdnum=0;
	}
	if(changesupports) changedvalues=TRUE;
	if(msuppdefaults!=NULL) FreeRDAdefault(msuppdefaults);
}
static void makespacesupport(int pos)
{
	short x,y,z;
	RDAsearch *temp,*temp1,*new_fields;
	RDAconfld *confld1,*confld2;

	++tmpnumsupp;
	new_fields=Rmalloc(tmpnumsupp*sizeof(RDAsearch));
	y=0;
	for(x=0,temp=tempsupport;x<(tmpnumsupp-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->module=NULL;
			temp1->filename=NULL;
			temp1->ctype=0;
			temp1->num=0;
			temp1->confld=NULL;
			temp1->cmodule=NULL;
			temp1->cfilename=NULL;
			temp1->subkeyname=NULL;
			temp1->expression=NULL;
			temp1->browse=FALSE;
			temp1->button_name=NULL;
			temp1->define_list=NULL;
			temp1->search_browse=NULL;
			temp1->smart_browse=FALSE;
			temp1->range=FALSE;
			temp1->num_writes=1;
			++y;
			temp1=new_fields+y;
		}
		if(temp->module!=NULL)
		{
			temp1->module=stralloc(temp->module);
			Rfree(temp->module);
		} else temp1->module=NULL;
		if(temp->filename!=NULL)
		{
			temp1->filename=stralloc(temp->filename);
			Rfree(temp->filename);
		} else temp1->filename=NULL;
		temp1->ctype=temp->ctype;
		temp1->num=temp->num;
		if(temp->confld!=NULL)
		{
			temp1->confld=Rmalloc(temp->num*sizeof(RDAconfld));
			for(z=0,confld1=temp->confld,confld2=temp1->confld;
				z<temp->num;++z,++confld1,++confld2)
			{
				if(confld1->fldname!=NULL)
				{
					confld2->fldname=stralloc(confld1->fldname);
					Rfree(confld1->fldname);
				} else confld2->fldname=NULL;
				if(confld1->conname!=NULL)
				{
					confld2->conname=stralloc(confld1->conname);
					Rfree(confld1->conname);
				} else confld2->conname=NULL;
				confld2->inherit=confld1->inherit;
			}
			Rfree(temp->confld);
		}
		if(temp->cmodule!=NULL)
		{
			temp1->cmodule=stralloc(temp->cmodule);
			Rfree(temp->cmodule);
		} else temp1->cmodule=NULL;
		if(temp->cfilename!=NULL)
		{
			temp1->cfilename=stralloc(temp->cfilename);
			Rfree(temp->cfilename);
		} else temp1->cfilename=NULL;
		if(temp->subkeyname!=NULL)
		{
			temp1->subkeyname=stralloc(temp->subkeyname);
			Rfree(temp->subkeyname);
		} else temp1->subkeyname=NULL;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		temp1->browse=temp->browse;
		if(temp->button_name!=NULL)
		{
			temp1->button_name=stralloc(temp->button_name);
			Rfree(temp->button_name);
		} else temp1->button_name=NULL;
		if(temp->browse_screen!=NULL)
		{
			temp1->browse_screen=stralloc(temp->browse_screen);
			Rfree(temp->browse_screen);
		} else temp1->browse_screen=NULL;
		if(temp->define_list!=NULL)
		{
			temp1->define_list=stralloc(temp->define_list);
			Rfree(temp->define_list);
		} else temp1->define_list=NULL;
		if(temp->search_browse!=NULL)
		{
			temp1->search_browse=stralloc(temp->search_browse);
			Rfree(temp->search_browse);
		} else temp1->search_browse=NULL;
		temp1->smart_browse=temp->smart_browse;
		temp1->range=temp->range;
		temp1->num_writes=temp->num_writes;
		++y;
	}
	Rfree(tempsupport);
	tempsupport=Rmalloc(tmpnumsupp*sizeof(RDAsearch));
	for(x=0,temp1=tempsupport,temp=new_fields;x<tmpnumsupp;++x,
		++temp,++temp1)
	{
		if(temp->module!=NULL)
		{
			temp1->module=stralloc(temp->module);
			Rfree(temp->module);
		} else temp1->module=NULL;
		if(temp->filename!=NULL)
		{
			temp1->filename=stralloc(temp->filename);
			Rfree(temp->filename);
		} else temp1->filename=NULL;
		temp1->ctype=temp->ctype;
		if(temp->cmodule!=NULL)
		{
			temp1->cmodule=stralloc(temp->cmodule);
			Rfree(temp->cmodule);
		} else temp1->cmodule=NULL;
		if(temp->cfilename!=NULL)
		{
			temp1->cfilename=stralloc(temp->cfilename);
			Rfree(temp->cfilename);
		} else temp1->cfilename=NULL;
		temp1->num=temp->num;
		if(temp->confld!=NULL)
		{
			temp1->confld=Rmalloc(temp->num*sizeof(RDAconfld));
			for(z=0,confld1=temp->confld,confld2=temp1->confld;
				z<temp->num;++z,++confld1,++confld2)
			{
				if(confld1->fldname!=NULL)
				{
					confld2->fldname=stralloc(confld1->fldname);
					Rfree(confld1->fldname);
				} else confld2->fldname=NULL;
				if(confld1->conname!=NULL)
				{
					confld2->conname=stralloc(confld1->conname);
					Rfree(confld1->conname);
				} else confld2->conname=NULL;
				confld2->inherit=confld1->inherit;
			}
			Rfree(temp->confld);
		}
		if(temp->subkeyname!=NULL)
		{
			temp1->subkeyname=stralloc(temp->subkeyname);
			Rfree(temp->subkeyname);
		} else temp1->subkeyname=NULL;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		temp1->browse=temp->browse;
		if(temp->button_name!=NULL)
		{
			temp1->button_name=stralloc(temp->button_name);
			Rfree(temp->button_name);
		} else temp1->button_name=NULL;
		if(temp->browse_screen!=NULL)
		{
			temp1->browse_screen=stralloc(temp->browse_screen);
			Rfree(temp->browse_screen);
		} else temp1->browse_screen=NULL;
		if(temp->define_list!=NULL)
		{
			temp1->define_list=stralloc(temp->define_list);
			Rfree(temp->define_list);
		} else temp1->define_list=NULL;
		if(temp->search_browse!=NULL)
		{
			temp1->search_browse=stralloc(temp->search_browse);
			Rfree(temp->search_browse);
		} else temp1->search_browse=NULL;
		temp1->smart_browse=temp->smart_browse;
		temp1->range=temp->range;
		temp1->num_writes=temp->num_writes;
	}
	Rfree(new_fields);
}
static void savemsupp(RDArsrc *msupprsrc,RDArsrc *supprsrc)
{
	int value=0,cmod=0,selectedm=0,selectedf=0,selectsubkey=0;
	RDAsearch *s;
	int x=0;
	RDAconfld *cf1,*cf2;
	char def_browse_screen=FALSE,def_search_browse=FALSE,def_define_list=FALSE;

	if(FINDRSCGETINT(supprsrc,"SUPPORTING FILES",&value)) return;
	if(tempsupport==NULL)
	{
		tempsupport=Rmalloc(sizeof(RDAsearch));
		tmpnumsupp=1;
	}
	s=tempsupport+value;
	readallwidgets(msupprsrc);
	FINDRSCGETINT(msupprsrc,"MODULE",&selectedm);
	FINDRSCGETINT(msupprsrc,"FILENAME",&selectedf);
	FINDRSCGETCHAR(msupprsrc,"DEFAULT BROWSE SCREEN",&def_browse_screen);
	FINDRSCGETCHAR(msupprsrc,"DEFAULT SEARCH BROWSE",&def_search_browse);
	FINDRSCGETCHAR(msupprsrc,"DEFAULT DEFINE LIST",&def_define_list);
	s->module=stralloc(modlstxmtn->libs[selectedm]);
	s->filename=stralloc(filelstmtn->libs[selectedf]);
	FINDRSCGETINT(msupprsrc,"CONNECT TYPES",&s->ctype);
	FINDRSCGETINT(msupprsrc,"CONNECT FILENAME",&cmod);
        if(!isEMPTY(cmodlstmtn->libs[cmod]))
        {
                s->cmodule=stripmodulename(cmodlstmtn->libs[cmod]);
                s->cfilename=stripfilename(cmodlstmtn->libs[cmod]);
        } else {
                s->cmodule=NULL;
                s->cfilename=NULL;
        }
	FINDRSCGETSTRING(msupprsrc,"EXPRESSION",&s->expression);
	FINDRSCGETCHAR(msupprsrc,"BROWSE",&s->browse);
	FINDRSCGETSTRING(msupprsrc,"BROWSE SCREEN",&s->browse_screen);
	FINDRSCGETSTRING(msupprsrc,"BUTTON NAME",&s->button_name);
	FINDRSCGETSTRING(msupprsrc,"DEFINE LIST",&s->define_list);
	FINDRSCGETSTRING(msupprsrc,"SEARCH BROWSE",&s->search_browse);
	FINDRSCGETINT(msupprsrc,"BROWSE TYPES",&x);
	s->smart_browse=x;
	FINDRSCGETINT(msupprsrc,"KEYNAME",&selectsubkey);
	s->subkeyname=stralloc(subkeylistmtn->libs[selectsubkey]);
	if(s->ctype!=2)
	{
		FINDRSCGETSTRING(msupprsrc,"EXPRESSION",&s->expression);
		if(holdconfld!=NULL)
		{
			s->num=holdnum;
			s->confld=Rmalloc(holdnum*sizeof(RDAconfld));
			for(x=0,cf1=holdconfld,cf2=s->confld;x<holdnum;++x,++cf1,++cf2)
			{
				if(cf1->fldname!=NULL)
				{
					cf2->fldname=stralloc(cf1->fldname);
					Rfree(cf1->fldname);
				} else cf2->fldname=NULL;
				if(cf1->conname!=NULL)
				{
					cf2->conname=stralloc(cf1->conname);
					Rfree(cf1->conname);
				} else cf2->conname=NULL;
				cf2->inherit=cf1->inherit;
			}
			Rfree(holdconfld);
			holdconfld=NULL;
			holdnum=0;
		}
	} else {
		s->expression=NULL;
		if(holdconfld!=NULL)
		{
			for(x=0,cf1=holdconfld;x<holdnum;++x,++cf1)
			{
				if(cf1->fldname!=NULL) Rfree(cf1->fldname);
				if(cf1->conname!=NULL) Rfree(cf1->conname);
			}
			Rfree(holdconfld);
			holdconfld=NULL;
			holdnum=0;
		}
		if(s->confld!=NULL)
		{
			for(x=0,cf1=s->confld;x<s->num;++x,++cf1)
			{
				if(cf1->fldname!=NULL) Rfree(cf1->fldname);
				if(cf1->conname!=NULL) Rfree(cf1->conname);
			}
			Rfree(s->confld);
			s->confld=NULL;
			s->num=0;
		}
	}
	if(def_search_browse) makeSearchBrowseScreen(MTNMSTR,s);
	if(def_browse_screen) makeBrowseScreen(MTNMSTR,s);
	if(def_define_list) makeDefineListScreen(MTNMSTR,s);
	getsupports();
	if(!FINDRSCLISTAPPlib(supprsrc,"SUPPORTING FILES",value,suppavlmtn))
		updatersrc(supprsrc,"SUPPORTING FILES");
	if(COMPARE_RSRCS(msupprsrc,msuppdefaults,2))
	{
		changesupports=TRUE;
	}
	quitmsupp(msupprsrc,supprsrc);
}
static void savemsuppabove(RDArsrc *msupprsrc,RDArsrc *supprsrc)
{
	int x;

	if(FINDRSCGETINT(supprsrc,"SUPPORTING FILES",&x)) return;
	makespacesupport(x);
	savemsupp(msupprsrc,supprsrc);
}
static void savemsuppbelow(RDArsrc *msupprsrc,RDArsrc *supprsrc)
{
	int x;

	if(FINDRSCGETINT(supprsrc,"SUPPORTING FILES",&x)) return;
	makespacesupport(++x);
	FINDRSCSETINT(supprsrc,"SUPPORTING FILES",x);
	savemsupp(msupprsrc,supprsrc);
}
static void getmodulelist()
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
		addAPPlib(modlstxmtn,"Contains No Database Libraries");
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	SORTAPPlib(modlstxmtn);
}
static void getconflds()
{
	int x;
	RDAconfld *cf1;
	char *temp=NULL;

	if(cnfldsmtn!=NULL) freeapplib(cnfldsmtn);
	cnfldsmtn=APPlibNEW();
	for(x=0,cf1=holdconfld;x<holdnum;++x,++cf1)
	{
		if(temp!=NULL)
		{
			temp=Rrealloc(temp,RDAstrlen(cf1->fldname)+RDAstrlen(cf1->conname)+28);
		} else {
			temp=Rmalloc(RDAstrlen(cf1->fldname)+RDAstrlen(cf1->conname)+28);
		}
		sprintf(temp,"%2d Field [%s] connects to %s",x+1,(cf1->fldname!=NULL ? cf1->fldname:""),(cf1->conname!=NULL ? cf1->conname:""));
		addAPPlib(cnfldsmtn,temp);
	}
	if(cnfldsmtn->numlibs<1)
	{
		addAPPlib(cnfldsmtn,"No Connecting Fields");
	}
}
static void setconflds(RDArsrc *msupprsrc)
{
	int y,z,selectedm=0,selectedf=0,selectedkey=0,selcf=0;
	NRDfield *fields=NULL,*f;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short nofields=0,nokeys=0,engine=0;
	RDAconfld *cf;
	char *modulename=NULL,*filename=NULL;

	if(holdconfld!=NULL)
	{
		for(z=0,cf=holdconfld;z<holdnum;++z,++cf)
		{
			if(cf->fldname!=NULL) Rfree(cf->fldname);
			if(cf->conname!=NULL) Rfree(cf->conname);
		}
		Rfree(holdconfld);
		holdconfld=NULL;
		holdnum=0;
	}
	if(FINDRSCGETINT(msupprsrc,"MODULE",&selectedm)) return;
	if(FINDRSCGETINT(msupprsrc,"FILENAME",&selectedf)) return;
	if(FINDRSCGETINT(msupprsrc,"KEYNAME",&selectedkey)) return;
	if(selectedm>=modlstxmtn->numlibs) 
	{
		selectedm=0;
	}
	if(selectedf>=filelstmtn->numlibs)
	{
		selectedf=0;
	}
	if(!GETBIN(modlstxmtn->libs[selectedm],filelstmtn->libs[selectedf],&engine,&nofields,&fields,&nokeys,&keys)) 
	{
		if(fields!=NULL)
		{
			for(y=0,f=fields;y<nofields;++y,++f)
			{
				if(f->name!=NULL) Rfree(f->name);
			}
			Rfree(fields);
			fields=NULL;
		}
		if(keys!=NULL)
		{
			for(y=0,keyx=keys;y<nokeys;++y,++keyx)
			{
				if(keyx->part!=NULL && !RDAstrcmp(keyx->name,subkeylistmtn->libs[selectedkey]))
				{
					holdnum=keyx->numparts;
					holdconfld=Rmalloc(keyx->numparts*sizeof(RDAconfld));
					for(z=0,cf=holdconfld,part=keyx->part;z<keyx->numparts;
						++z,++cf,++part)
					{
						if(part->name!=NULL) 
						{
							cf->fldname=stralloc(part->name);
							Rfree(part->name);
						} else cf->fldname=NULL;
					}
					Rfree(keyx->part);
				}
				if(keyx->name!=NULL) Rfree(keyx->name);
			}
			Rfree(keys);
			keys=NULL;
		}
		if(holdconfld!=NULL)
		{
			if(FINDRSCGETINT(msupprsrc,"KEYNAME",&selectedkey)) return;
			if(FINDRSCGETINT(msupprsrc,"CONNECT FILENAME",&selcf)) return;
			filename=stripfilename(cmodlstmtn->libs[selcf]);
			if(filename!=NULL) modulename=stripmodulename(cmodlstmtn->libs[selcf]);
			if(modulename!=NULL && filename!=NULL)
			{
/*
				if(RDAstrcmp(filename,"SORTFILE"))
				{
*/
				if(!GETBIN(modulename,filename,&engine,&nofields,&fields,&nokeys,&keys)) 
				{
					if(fields!=NULL)
					{
						for(y=0,f=fields;y<nofields;++y,++f)
						{
							for(z=0,cf=holdconfld;z<holdnum;++z,++cf)
							{
								if(!RDAstrcmp(cf->fldname,f->name))
								{
									cf->conname=Rmalloc(RDAstrlen(filename)+RDAstrlen(f->name)+5);
									sprintf(cf->conname,"[%s][%s]",filename,f->name);
								}
							}
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
								for(z=0,part=keyx->part;z<keyx->numparts;
									++z,++part)
								{
									if(part->name!=NULL) Rfree(part->name);
								}
								Rfree(keyx->part);
							}
						}
						Rfree(keys);
					}
				}
/*
				}
*/
			}
			if(filename!=NULL) Rfree(filename);
			if(modulename!=NULL) Rfree(modulename);
		}
	}
	getconflds();
	if(!FINDRSCLISTAPPlib(msupprsrc,"CONNECT FIELDNAME",0,cnfldsmtn))
	{
		updatersrc(msupprsrc,"CONNECT FIELDNAME");
	}
}
static void quitecon(RDArsrc *econrsrc,RDArsrc *msupprsrc)
{
	if(econrsrc!=NULL)
	{
		killwindow(econrsrc);
		free_rsrc(econrsrc);
	}
	if(econfldsmtn!=NULL) freeapplib(econfldsmtn);
	if(econdefaults!=NULL) FreeRDAdefault(econdefaults);
}
static void saveecon(RDArsrc *econrsrc,RDArsrc *msupprsrc)
{
	int selected=0,selhold=0;
	char *fldname=NULL;
	RDAconfld *c;

	if(FINDRSCGETINT(msupprsrc,"CONNECT FIELDNAME",&selhold)) return;
	if(FINDRSCGETINT(econrsrc,"CONNECT FIELDS",&selected)) return;
	if(FINDRSCGETSTRING(econrsrc,"FIELD NAME",&fldname)) return;
	if(fldname!=NULL)
	{
		if(holdconfld!=NULL)
		{
			c=holdconfld+selhold;
			if(!RDAstrcmp(c->fldname,fldname))
			{
				readwidget(econrsrc,"INHERIT");
				FINDRSCGETCHAR(econrsrc,"INHERIT",&c->inherit);
				c->conname=stralloc(econfldsmtn->libs[selected]);
				getconflds();
				if(!FINDRSCLISTAPPlib(msupprsrc,"CONNECT FIELDNAME",
					selhold,cnfldsmtn))
				{
					updatersrc(msupprsrc,"CONNECT FIELDNAME");
				}
			}
		}
	}
	if(COMPARE_RSRCS(econrsrc,econdefaults,2))
	{
		changesupports=TRUE;
	}
	quitecon(econrsrc,msupprsrc);
}
static void quitecontest(RDArsrc *econrsrc,RDArsrc *msupprsrc)
{
	readallwidgets(econrsrc);
	if(COMPARE_RSRCS(econrsrc,econdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","MASTER CHANGES NOT SAVED!","Changes have been made to this CONNECT FIELD!\nDo you want to Save these changes?",saveecon,quitecon,FALSE,2,econrsrc,msupprsrc,NULL);
	} else {
		quitecon(econrsrc,msupprsrc);
	}
}
static APPlib *makefieldvallist(RDArsrc *supprsrc)
{
#ifdef ADDING_VIRTUALS
	APPlib *virflist=NULL;
	char addvf=FALSE,*libx=NULL;
	int y,z;
	DFincvir *i,*a;
	DFvirtual *d=NULL;
	short dumb=0;
	RDArsrc *tmprsrc=NULL;
#endif /* Adding Virtuals */
	char *curfile=NULL,*curmod=NULL;
	APPlib *tmp=NULL;
	int x;
	RDAsearch *s;

	tmp=APPlibNEW();
#ifdef ADDING_VIRTUALS
	tmprsrc=RDArsrcNEW(module,"TEMP");
#endif /* Adding Virtuals */
	readwidget(supprsrc,"MAINFILE");
	FINDRSCGETSTRING(supprsrc,"MODULE",&curmod);
	FINDRSCGETSTRING(supprsrc,"MAINFILE",&curfile);
	if(!isEMPTY(curfile) && !isEMPTY(curmod))
	{
#ifdef ADDING_VIRTUALS
		addDFincvir(tmprsrc,curmod,curfile,NULL,dumb);
#endif /* Adding Virtuals */
		addholdvaluessup(tmp,curmod,curfile);
	}
	if(curfile!=NULL) Rfree(curfile);
	for(x=0,s=tempsupport;x<tmpnumsupp;++x,++s)
	{
#ifdef ADDING_VIRTUALS
		addDFincvir(tmprsrc,s->module,s->filename,NULL,dumb);
#endif /* Adding Virtuals */
		addholdvaluessup(tmp,s->module,s->filename);
	}
#ifdef ADDING_VIRTUALS
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(curmod)+17);
	sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,(curmod!=NULL ? curmod:""));
	virflist=getlibnames(libx,FALSE);
	if(curmod!=NULL) Rfree(curmod);
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
#endif /* Adding Virtuals */
	return(tmp);
}
static void editconnect(RDArsrc *msupprsrc,RDArsrc *supprsrc)
{
	int x,selectfld=0;
	char inherit=FALSE,*fldname=NULL,*cfldname=NULL;
	RDAconfld *c;
	RDArsrc *econrsrc=NULL;

	readallwidgets(msupprsrc);
	if(FINDRSCGETINT(msupprsrc,"CONNECT FIELDNAME",&selectfld)) return;
	if(holdconfld!=NULL && selectfld<holdnum)
	{
		c=holdconfld+selectfld;
		fldname=stralloc(c->fldname);
		cfldname=stralloc(c->conname);
		inherit=c->inherit;
	} 
	econrsrc=RDArsrcNEW(module,"EDIT SUPPORTING CONNECT FIELD");
	addstdrsrc(econrsrc,"FIELD NAME",VARIABLETEXT,0,
		(fldname!=NULL ? fldname:""),FALSE);
	if(fldname!=NULL) Rfree(fldname);
	if(econfldsmtn!=NULL) freeapplib(econfldsmtn);
	econfldsmtn=APPlibNEW();
	x=0;
	econfldsmtn=makefieldvallist(supprsrc);
	if(econfldsmtn->numlibs<1)
	{
		addAPPlib(econfldsmtn,"No Connecting Fields Available");
	}
	addlstrsrc(econrsrc,"CONNECT FIELDS",&x,TRUE,NULL,econfldsmtn->numlibs,&econfldsmtn->libs,NULL);
	addstdrsrc(econrsrc,"INHERIT",BOOLNS,1,&inherit,TRUE);
	addrfcbrsrc(econrsrc,"SAVE",TRUE,saveecon,msupprsrc);
	addrfcbrsrc(econrsrc,"QUIT",TRUE,quitecontest,msupprsrc);
	addbtnrsrc(econrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(econrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(econrsrc,TRUE,quitecon,msupprsrc,FALSE);
	setconfldlist(econrsrc,econfldsmtn,cfldname);
	if(cfldname!=NULL) Rfree(cfldname);
	econdefaults=GetDefaults(econrsrc);
}
static void dofilelist(RDArsrc *msupprsrc,int *passfunc)
{
	int y,z,selectedm,selectedf;
	NRDfield *fields=NULL,*f;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short nofields=0,nokeys=0,engine=0;

	if(FINDRSCGETINT(msupprsrc,"MODULE",&selectedm)) return;
	if(FINDRSCGETINT(msupprsrc,"FILENAME",&selectedf)) return;
	if(subkeylistmtn!=NULL) freeapplib(subkeylistmtn);
	subkeylistmtn=APPlibNEW();
	if(selectedm>=modlstxmtn->numlibs) 
	{
		selectedm=0;
	}
	if(selectedf>=filelstmtn->numlibs)
	{
		selectedf=0;
	}
	if(!GETBIN(modlstxmtn->libs[selectedm],filelstmtn->libs[selectedf],&engine,&nofields,&fields,&nokeys,&keys)) 
	{
		if(fields!=NULL)
		{
			for(y=0,f=fields;y<nofields;++y,++f)
			{
				if(f->name!=NULL) Rfree(f->name);
			}
			Rfree(fields);
		}
		if(keys!=NULL)
		{
			for(y=0,keyx=keys;y<nokeys;++y,++keyx)
			{
				addAPPlib(subkeylistmtn,keyx->name);
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
	if(subkeylistmtn->numlibs<1)
	{
		addAPPlib(subkeylistmtn,"No Keys Available");
	}
	if(!FINDRSCLISTAPPlib(msupprsrc,"KEYNAME",0,subkeylistmtn))
	{
		updatersrc(msupprsrc,"KEYNAME");
		if(*passfunc) setconflds(msupprsrc);
	}
}
static void domodulelist(RDArsrc *msupprsrc,int *passfunc)
{
	int selected=0,selectedf=0;
	char *libx=NULL;

	readwidget(msupprsrc,"MODULE");
	readwidget(msupprsrc,"FILENAME");
	if(FINDRSCGETINT(msupprsrc,"MODULE",&selected)) return;
	if(FINDRSCGETINT(msupprsrc,"FILENAME",&selectedf)) return;
	if(filelstmtn!=NULL) freeapplib(filelstmtn);
	if(selected>=modlstxmtn->numlibs) 
	{
		selected=0;
	}
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlstxmtn->libs[selected])+10);
	sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modlstxmtn->libs[selected]);
	filelstmtn=getlibnames(libx,TRUE);
	if(libx!=NULL) Rfree(libx);
	if(filelstmtn==NULL)
	{
		filelstmtn=APPlibNEW();
		addAPPlib(filelstmtn,"Contains No File Definitions");
	}
	if(selectedf>=filelstmtn->numlibs) selectedf=0;
	if(!FINDRSCLISTAPPlib(msupprsrc,"FILENAME",selectedf,filelstmtn))
	{
		updatersrc(msupprsrc,"FILENAME");
		dofilelist(msupprsrc,passfunc);
	}
}
static void changectype(RDArsrc *msupprsrc)
{
	int ctype=0;

	FINDRSCGETINT(msupprsrc,"CONNECT TYPES",&ctype);
	if(ctype==2)
	{
		FINDRSCSETSENSITIVE(msupprsrc,"CONNECT FILENAME",FALSE);
		FINDRSCSETEDITABLE(msupprsrc,"CONNECT FILENAME",FALSE);
		FINDRSCSETSENSITIVE(msupprsrc,"CONNECT FIELDNAME",FALSE);
		FINDRSCSETEDITABLE(msupprsrc,"CONNECT FIELDNAME",FALSE);
		FINDRSCSETSENSITIVE(msupprsrc,"SELECT",FALSE);
		FINDRSCSETSENSITIVE(msupprsrc,"EXPRESSION",FALSE);
		FINDRSCSETSENSITIVE(msupprsrc,"LOAD VALUE",FALSE);
		FINDRSCSETSENSITIVE(msupprsrc,"LOAD OPERATOR",FALSE);
		FINDRSCSETSENSITIVE(msupprsrc,"LOAD GROUPER",FALSE);
		FINDRSCSETSENSITIVE(msupprsrc,"LOAD CONTROL",FALSE);
	} else {
		FINDRSCSETSENSITIVE(msupprsrc,"CONNECT FILENAME",TRUE);
		FINDRSCSETEDITABLE(msupprsrc,"CONNECT FILENAME",TRUE);
		FINDRSCSETSENSITIVE(msupprsrc,"CONNECT FIELDNAME",TRUE);
		FINDRSCSETEDITABLE(msupprsrc,"CONNECT FIELDNAME",TRUE);
		FINDRSCSETSENSITIVE(msupprsrc,"SELECT",TRUE);
		FINDRSCSETSENSITIVE(msupprsrc,"EXPRESSION",TRUE);
		FINDRSCSETSENSITIVE(msupprsrc,"LOAD VALUE",TRUE);
		FINDRSCSETSENSITIVE(msupprsrc,"LOAD OPERATOR",TRUE);
		FINDRSCSETSENSITIVE(msupprsrc,"LOAD GROUPER",TRUE);
		FINDRSCSETSENSITIVE(msupprsrc,"LOAD CONTROL",TRUE);
	}
}
static void setbrowsesensitive(RDArsrc *msupprsrc)
{
	char brs=FALSE;

	readwidget(msupprsrc,"BROWSE");
	FINDRSCGETCHAR(msupprsrc,"BROWSE",&brs);
	if(brs)
	{
		FINDRSCSETSENSITIVE(msupprsrc,"BROWSE SCREEN",TRUE);
		FINDRSCSETSENSITIVE(msupprsrc,"DEFINE LIST",TRUE);
		FINDRSCSETSENSITIVE(msupprsrc,"SEARCH BROWSE",TRUE);
		FINDRSCSETEDITABLE(msupprsrc,"BROWSE SCREEN",TRUE);
		FINDRSCSETEDITABLE(msupprsrc,"DEFINE LIST",TRUE);
		FINDRSCSETEDITABLE(msupprsrc,"SEARCH BROWSE",TRUE);
		FINDRSCSETSENSITIVE(msupprsrc,"BUTTON NAME",TRUE);
		FINDRSCSETEDITABLE(msupprsrc,"BUTTON NAME",TRUE);
		FINDRSCSETSENSITIVE(msupprsrc,"BROWSE TYPES",TRUE);
		FINDRSCSETEDITABLE(msupprsrc,"BROWSE TYPES",TRUE);
		FINDRSCSETSENSITIVE(msupprsrc,"DEFAULT BROWSE SCREEN",TRUE);
		FINDRSCSETEDITABLE(msupprsrc,"DEFAULT BROWSE SCREEN",TRUE);
		FINDRSCSETSENSITIVE(msupprsrc,"DEFAULT DEFINE LIST",TRUE);
		FINDRSCSETEDITABLE(msupprsrc,"DEFAULT DEFINE LIST",TRUE);
		FINDRSCSETSENSITIVE(msupprsrc,"DEFAULT SEARCH BROWSE",TRUE);
		FINDRSCSETEDITABLE(msupprsrc,"DEFAULT SEARCH BROWSE",TRUE);
	} else {
		FINDRSCSETSENSITIVE(msupprsrc,"BROWSE SCREEN",FALSE);
		FINDRSCSETSENSITIVE(msupprsrc,"DEFINE LIST",FALSE);
		FINDRSCSETSENSITIVE(msupprsrc,"SEARCH BROWSE",FALSE);
		FINDRSCSETEDITABLE(msupprsrc,"BROWSE SCREEN",FALSE);
		FINDRSCSETEDITABLE(msupprsrc,"DEFINE LIST",FALSE);
		FINDRSCSETEDITABLE(msupprsrc,"SEARCH BROWSE",FALSE);
		FINDRSCSETSENSITIVE(msupprsrc,"BUTTON NAME",FALSE);
		FINDRSCSETEDITABLE(msupprsrc,"BUTTON NAME",FALSE);
		FINDRSCSETSENSITIVE(msupprsrc,"BROWSE TYPES",FALSE);
		FINDRSCSETEDITABLE(msupprsrc,"BROWSE TYPES",FALSE);
		FINDRSCSETEDITABLE(msupprsrc,"DEFAULT BROWSE SCREEN",FALSE);
		FINDRSCSETSENSITIVE(msupprsrc,"DEFAULT BROWSE SCREEN",FALSE);
		FINDRSCSETEDITABLE(msupprsrc,"DEFAULT DEFINE LIST",FALSE);
		FINDRSCSETSENSITIVE(msupprsrc,"DEFAULT DEFINE LIST",FALSE);
		FINDRSCSETEDITABLE(msupprsrc,"DEFAULT SEARCH BROWSE",FALSE);
		FINDRSCSETSENSITIVE(msupprsrc,"DEFAULT SEARCH BROWSE",FALSE);
	}
}
static void supportscreen(RDArsrc *supprsrc,int num,
	char *modulex,char *filename,int ctype,char *cmodule,char *cfilename,
	char *subkeyname,int number,char *expression,
	char browse,char *button_name,char *browse_screen, 
	char *define_list,char *search_browse,char smart_browse,
	RDAconfld *confld,void (*savefunc)(...),void (*quitfunctest)(...))
{
	int x=0,y=0,selmod=0,selfile=0;
	char *temp=NULL,*tmod=NULL,*curfile=NULL,*curmod=NULL;
	char make_default=FALSE;
	RDAsearch *s;
	RDAconfld *cf,*cf1;
	NRDfield *fields=NULL,*f;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short nofields=0,nokeys=0,engine=0;
	RDArsrc *msupprsrc=NULL;

	msupprsrc=RDArsrcNEW(module,"MAINTAIN SUPPORTING FILE");
	addstdrsrc(msupprsrc,"NUMBER",LONGV,8,&num,FALSE);
	getmodulelist();
	if(modulex!=NULL)
	{
		for(x=0;x<modlstxmtn->numlibs;++x)
		{
			if(!RDAstrcmp(modlstxmtn->libs[x],modulex)) break;
		}
	}
	if(x>=modlstxmtn->numlibs)
	{
		FINDRSCGETSTRING(mtnrsrc,"MODULE",&tmod);
		for(x=0;x<modlstxmtn->numlibs;++x)
		{
			if(!RDAstrcmp(modlstxmtn->libs[x],tmod)) break;
		}
		if(tmod!=NULL) Rfree(tmod);
		if(x>=modlstxmtn->numlibs) x=0;
	}
	addlstrsrc(msupprsrc,"MODULE",&x,TRUE,domodulelist,modlstxmtn->numlibs,
		&modlstxmtn->libs,&ptrue);
	if(filelstmtn!=NULL) freeapplib(filelstmtn);
	filelstmtn=APPlibNEW();
	addlstrsrc(msupprsrc,"FILENAME",&x,TRUE,dofilelist,filelstmtn->numlibs,
		&filelstmtn->libs,&ptrue);
        if(ctypes!=NULL) freeapplib(ctypes);
        ctypes=APPlibNEW();
	for(x=0;x<3;++x) addAPPlib(ctypes,contypemtn[x]);
	x=0;
	addlstrsrc(msupprsrc,"CONNECT TYPES",&ctype,TRUE,changectype,ctypes->numlibs,&ctypes->libs,NULL);
	if(cmodlstmtn!=NULL) freeapplib(cmodlstmtn);
	cmodlstmtn=APPlibNEW();
	readwidget(supprsrc,"MAINFILE");
	FINDRSCGETSTRING(supprsrc,"MAINFILE",&curfile);
	if(!RDAstrcmp(curfile,"SORTFILE"))
	{
		curmod=stralloc("RPTGEN");
	} else {
		readwidget(supprsrc,"MODULE");
		FINDRSCGETSTRING(supprsrc,"MODULE",&curmod);
	}
	if(!isEMPTY(curfile) && !isEMPTY(curmod))
	{
		if(temp!=NULL)
		{
			temp=Rrealloc(temp,RDAstrlen(curmod)+RDAstrlen(curfile)+5);
		} else {
			temp=Rmalloc(RDAstrlen(curmod)+RDAstrlen(curfile)+5);
		}
		sprintf(temp,"[%s][%s]",curmod,curfile);
		addAPPlib(cmodlstmtn,temp);
	}
	if(curmod!=NULL) Rfree(curmod);
	if(curfile!=NULL) Rfree(curfile);
	if(num>1)
	{
		for(x=0,s=tempsupport;x<(num-1);++x,++s)
		{
			if(temp!=NULL)
			{
				temp=Rrealloc(temp,RDAstrlen(s->module)+RDAstrlen(s->filename)+12);
			} else {
				temp=Rmalloc(RDAstrlen(s->module)+RDAstrlen(s->filename)+12);
			}
			sprintf(temp,"%2d [%s][%s]",x+1,(s->module!=NULL ? s->module:""),
				(s->filename!=NULL ? s->filename:""));
			addAPPlib(cmodlstmtn,temp);
		}
	}
	if(temp!=NULL) Rfree(temp);
	if(cmodlstmtn->numlibs<1)
	{
		addAPPlib(cmodlstmtn,"No Files Available");
	}
	for(x=0,s=tempsupport;x<(num-1);++x,++s)
	{
		if(!RDAstrcmp(s->module,cmodule) && !RDAstrcmp(s->filename,cfilename)) break;
	}
	if(x>=(num-1)) x=0;
	addlstrsrc(msupprsrc,"CONNECT FILENAME",&x,TRUE,setconflds,cmodlstmtn->numlibs,&cmodlstmtn->libs,NULL);
	if(cnfldsmtn!=NULL) freeapplib(cnfldsmtn);
	cnfldsmtn=APPlibNEW();
	if(confld!=NULL && number)
	{
		holdnum=number;
		holdconfld=Rmalloc(number*sizeof(RDAconfld));
		for(x=0,cf1=holdconfld,cf=confld;x<number;++x,++cf,++cf1)
		{
			if(temp!=NULL)
			{
				temp=Rrealloc(temp,RDAstrlen(cf->fldname)+RDAstrlen(cf->conname)+29);
			} else {
				temp=Rmalloc(RDAstrlen(cf->fldname)+RDAstrlen(cf->conname)+29);
			}
			cf1->fldname=stralloc(cf->fldname);
			cf1->conname=stralloc(cf->conname);
			cf1->inherit=cf->inherit;
			sprintf(temp,"%2d Field [%s] connects to %s",x+1,(cf->fldname!=NULL ? cf->fldname:""),(cf->conname!=NULL ? cf->conname:""));
			addAPPlib(cnfldsmtn,temp);
		}
	}
	if(subkeylistmtn!=NULL) freeapplib(subkeylistmtn);
	subkeylistmtn=APPlibNEW();
	x=0;
	addlstrsrc(msupprsrc,"KEYNAME",&x,TRUE,setconflds,subkeylistmtn->numlibs,&subkeylistmtn->libs,NULL);
	selmod=0;
	domodulelist(msupprsrc,&pfalse);
	if(modulex!=NULL)
	{
		for(selmod=0;selmod<modlstxmtn->numlibs;++selmod)
		{
			if(!RDAstrcmp(modlstxmtn->libs[selmod],modulex)) break;
		}
	}
	if(selmod>=modlstxmtn->numlibs)
	{
		FINDRSCGETSTRING(mtnrsrc,"MODULE",&tmod);
		for(selmod=0;selmod<modlstxmtn->numlibs;++selmod)
		{
			if(!RDAstrcmp(modlstxmtn->libs[selmod],tmod)) break;
		}
		if(tmod!=NULL) Rfree(tmod);
		if(selmod>=modlstxmtn->numlibs) selmod=0;
	}
	dofilelist(msupprsrc,&pfalse); 
	selfile=0;
	if(filename!=NULL)
	{
		for(selfile=0;selfile<filelstmtn->numlibs;++selfile)
		{
			if(!RDAstrcmp(filelstmtn->libs[selfile],filename)) break;
		}
	}
	if(selfile>=filelstmtn->numlibs)
	{
		selfile=0;
	}
	if(!FINDRSCLISTAPPlib(msupprsrc,"FILENAME",selfile,filelstmtn))
	{
		updatersrc(msupprsrc,"FILENAME");
	}
	if(!GETBIN(modlstxmtn->libs[selmod],filelstmtn->libs[selfile],&engine,&nofields,&fields,&nokeys,&keys)) 
	{
		if(subkeylistmtn!=NULL) freeapplib(subkeylistmtn);
		subkeylistmtn=APPlibNEW();
		if(fields!=NULL)
		{
			for(x=0,f=fields;x<nofields;++x,++f)
			{
				if(f->name!=NULL) Rfree(f->name);
			}
			Rfree(fields);
		}
		if(keys!=NULL)
		{
			for(x=0,keyx=keys;x<nokeys;++x,++keyx)
			{
				addAPPlib(subkeylistmtn,keyx->name);
				if(keyx->name!=NULL) Rfree(keyx->name);
				if(keyx->part!=NULL)
				{
					for(y=0,part=keyx->part;y<keyx->numparts;++y,++part)
					{
						if(part->name!=NULL) Rfree(part->name);
					}
					Rfree(keyx->part);
				}
			}
			Rfree(keys);
		}
	}
	if(subkeylistmtn->numlibs<1)
	{
		addAPPlib(subkeylistmtn,"No Keys Available");
	}
	if(subkeyname!=NULL)
	{
		for(x=0;x<subkeylistmtn->numlibs;++x)
		{
			if(!RDAstrcmp(subkeylistmtn->libs[x],subkeyname)) break;
		}
	}
	if(x>=subkeylistmtn->numlibs)
	{
		x=0;
	}
	if(!FINDRSCLISTAPPlib(msupprsrc,"KEYNAME",x,subkeylistmtn))
	{
		updatersrc(msupprsrc,"KEYNAME");
	}
	if(cnfldsmtn->numlibs<1)
	{
		addAPPlib(cnfldsmtn,"No Connecting Fields");
		setconflds(msupprsrc);
#ifdef XXXX
		if(ctype!=2)
		{
			setconflds(msupprsrc);
		}
#endif /* XXXX */
	}
	x=0;
	addlstrsrc(msupprsrc,"CONNECT FIELDNAME",&x,TRUE,NULL,cnfldsmtn->numlibs,
		&cnfldsmtn->libs,NULL);
	addbtnrsrc(msupprsrc,"SELECT",TRUE,editconnect,supprsrc);
	addsctrsrc(msupprsrc,"EXPRESSION",0,expression,TRUE);
	addbtnrsrc(msupprsrc,"LOAD VALUE",TRUE,loadvalue3mtn,"EXPRESSION");
	addbtnrsrc(msupprsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(msupprsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(msupprsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	if(ctype==2)
	{
		FINDRSCSETSENSITIVE(msupprsrc,"CONNECT FILENAME",FALSE);
		FINDRSCSETEDITABLE(msupprsrc,"CONNECT FILENAME",FALSE);
		FINDRSCSETSENSITIVE(msupprsrc,"CONNECT FIELDNAME",FALSE);
		FINDRSCSETEDITABLE(msupprsrc,"CONNECT FIELDNAME",FALSE);
		FINDRSCSETSENSITIVE(msupprsrc,"SELECT",FALSE);
		FINDRSCSETSENSITIVE(msupprsrc,"EXPRESSION",FALSE);
		FINDRSCSETSENSITIVE(msupprsrc,"LOAD VALUE",FALSE);
		FINDRSCSETSENSITIVE(msupprsrc,"LOAD OPERATOR",FALSE);
		FINDRSCSETSENSITIVE(msupprsrc,"LOAD GROUPER",FALSE);
		FINDRSCSETSENSITIVE(msupprsrc,"LOAD CONTROL",FALSE);
	} else {
		FINDRSCSETSENSITIVE(msupprsrc,"CONNECT FILENAME",TRUE);
		FINDRSCSETEDITABLE(msupprsrc,"CONNECT FILENAME",TRUE);
		FINDRSCSETSENSITIVE(msupprsrc,"CONNECT FIELDNAME",TRUE);
		FINDRSCSETEDITABLE(msupprsrc,"CONNECT FIELDNAME",TRUE);
		FINDRSCSETSENSITIVE(msupprsrc,"SELECT",TRUE);
		FINDRSCSETSENSITIVE(msupprsrc,"EXPRESSION",TRUE);
		FINDRSCSETSENSITIVE(msupprsrc,"LOAD VALUE",TRUE);
		FINDRSCSETSENSITIVE(msupprsrc,"LOAD OPERATOR",TRUE);
		FINDRSCSETSENSITIVE(msupprsrc,"LOAD GROUPER",TRUE);
		FINDRSCSETSENSITIVE(msupprsrc,"LOAD CONTROL",TRUE);
	}

	addstdrsrc(msupprsrc,"BROWSE",BOOLNS,1,&browse,TRUE);
	FINDRSCSETFUNC(msupprsrc,"BROWSE",setbrowsesensitive,NULL);
	addstdrsrc(msupprsrc,"BUTTON NAME",VARIABLETEXT,0,button_name,TRUE);
	addstdrsrc(msupprsrc,"BROWSE SCREEN",VARIABLETEXT,0,browse_screen,TRUE);
	addstdrsrc(msupprsrc,"DEFINE LIST",VARIABLETEXT,0,define_list,TRUE);
	addstdrsrc(msupprsrc,"SEARCH BROWSE",VARIABLETEXT,0,search_browse,TRUE);
	addstdrsrc(msupprsrc,"DEFAULT BROWSE SCREEN",BOOLNS,1,&make_default,TRUE);
	addstdrsrc(msupprsrc,"DEFAULT DEFINE LIST",BOOLNS,1,&make_default,TRUE);
	addstdrsrc(msupprsrc,"DEFAULT SEARCH BROWSE",BOOLNS,1,&make_default,TRUE);
	BrowseTypes=APPlibNEW();
	addAPPlib(BrowseTypes,"Standard Browse");
	addAPPlib(BrowseTypes,"Smart Browse");
	addAPPlib(BrowseTypes,"Search Browse");
	addAPPlib(BrowseTypes,"Lexical Pattern Match");
	x=smart_browse;
	addlstrsrc(msupprsrc,"BROWSE TYPES",&x,TRUE,NULL,BrowseTypes->numlibs,&BrowseTypes->libs,NULL);

	addrfcbrsrc(msupprsrc,"SAVE",TRUE,savefunc,supprsrc);
	addrfcbrsrc(msupprsrc,"QUIT",TRUE,quitfunctest,supprsrc);
	addbtnrsrc(msupprsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(msupprsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	setbrowsesensitive(msupprsrc);
	if(temp!=NULL) Rfree(temp);
	msuppdefaults=GetDefaults(msupprsrc);
	changesupports=FALSE;
	APPmakescrn(msupprsrc,TRUE,quitmsupp,supprsrc,FALSE);
}
static void quitmsupptest(RDArsrc *msupprsrc,RDArsrc *supprsrc)
{
	readallwidgets(msupprsrc);
	if(COMPARE_RSRCS(msupprsrc,msuppdefaults,2) || changesupports)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","MASTER CHANGES NOT SAVED!","Changes have been made to this SUPPORTING FILE DEFININTION!\nDo you want to Save these changes?",savemsupp,quitmsupp,FALSE,2,msupprsrc,supprsrc,NULL);
	} else {
		quitmsupp(msupprsrc,supprsrc);
	}
}
static void quitmsuppabovetest(RDArsrc *msupprsrc,RDArsrc *supprsrc)
{
	readallwidgets(msupprsrc);
	if(COMPARE_RSRCS(msupprsrc,msuppdefaults,2) || changesupports)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","MASTER CHANGES NOT SAVED!","Changes have been made to this SUPPORTING FILE DEFININTION!\nDo you want to Save these changes?",savemsuppabove,quitmsupp,FALSE,2,msupprsrc,supprsrc,NULL);
	} else {
		quitmsupp(msupprsrc,supprsrc);
	}
}
static void quitmsuppbelowtest(RDArsrc *msupprsrc,RDArsrc *supprsrc)
{
	readallwidgets(msupprsrc);
	if(COMPARE_RSRCS(msupprsrc,msuppdefaults,2) || changesupports)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","MASTER CHANGES NOT SAVED!","Changes have been made to this SUPPORTING FILE DEFININTION!\nDo you want to Save these changes?",savemsuppbelow,quitmsupp,FALSE,2,msupprsrc,supprsrc,NULL);
	} else {
		quitmsupp(msupprsrc,supprsrc);
	}
}
void suppaddabove(RDArsrc *supprsrc,RDArsrc *mtnrsrc)
{
	int selected=0;
	char *modx=NULL;

	if(FINDRSCGETINT(supprsrc,"SUPPORTING FILES",&selected)) return;
	if(selected==0 && !RDAstrcmp(suppavlmtn->libs[selected],"No Files Defined"))
		return;
	readwidget(mtnrsrc,"MODULE");
	FINDRSCGETSTRING(mtnrsrc,"MODULE",&modx);
	supportscreen(supprsrc,++selected,modx,NULL,0,NULL,NULL,NULL,0,
		NULL,FALSE,NULL,NULL,NULL,NULL,FALSE,
		NULL,savemsuppabove,quitmsuppabovetest);
	if(modx!=NULL) Rfree(modx);
}
void suppaddbelow(RDArsrc *supprsrc,RDArsrc *mtnrsrc)
{
	int selected=0;
	char *modx=NULL;

	if(FINDRSCGETINT(supprsrc,"SUPPORTING FILES",&selected)) return;
	if(selected==0 && !RDAstrcmp(suppavlmtn->libs[selected],"No Files Defined"))
		return;
	selected+=2;
	readwidget(mtnrsrc,"MODULE");
	FINDRSCGETSTRING(mtnrsrc,"MODULE",&modx);
	supportscreen(supprsrc,selected,modx,NULL,0,NULL,NULL,
		NULL,0,NULL,FALSE,NULL,NULL,NULL,NULL,FALSE,
		NULL,savemsuppbelow,quitmsuppbelowtest);
	if(modx!=NULL) Rfree(modx);
}
void suppdelete(RDArsrc *supprsrc)
{
	short x,y,z;
	RDAsearch *temp,*temp1,*new_fields;
	int value;
	RDAconfld *confld1,*confld2;

	readallwidgets(supprsrc);
	if(FINDRSCGETINT(supprsrc,"SUPPORTING FILES",&value)) return;
	if(tmpnumsupp<1) return;
	if(tmpnumsupp>1)
	{
		new_fields=Rmalloc((tmpnumsupp-1)*sizeof(RDAsearch));
		y=0;
		for(x=0,temp=tempsupport;x<tmpnumsupp;++x,++temp)
		{
			temp1=new_fields+y;
			if(x!=value)
			{
				if(temp->module!=NULL)
				{
					temp1->module=stralloc(temp->module);
					Rfree(temp->module);
				} else temp1->module=NULL;
				if(temp->filename!=NULL)
				{
					temp1->filename=stralloc(temp->filename);
					Rfree(temp->filename);
				} else temp1->filename=NULL;
				temp1->ctype=temp->ctype;
				temp1->num=temp->num;
				if(temp->confld!=NULL)
				{
					temp1->confld=Rmalloc(temp->num*sizeof(RDAconfld));
					for(z=0,confld1=temp->confld,confld2=temp1->confld;
						z<temp->num;++z,++confld1,++confld2)
					{
						if(confld1->fldname!=NULL)
						{
							confld2->fldname=stralloc(confld1->fldname);
							Rfree(confld1->fldname);
						} else confld2->fldname=NULL;
						if(confld1->conname!=NULL)
						{
							confld2->conname=stralloc(confld1->conname);
							Rfree(confld1->conname);
						} else confld2->conname=NULL;
						confld2->inherit=confld1->inherit;
					}
					Rfree(temp->confld);
				} else temp1->confld=NULL;
				if(temp->cmodule!=NULL)
				{
					temp1->cmodule=stralloc(temp->cmodule);
					Rfree(temp->cmodule);
				} else temp1->cmodule=NULL;
				if(temp->cfilename!=NULL)
				{
					temp1->cfilename=stralloc(temp->cfilename);
					Rfree(temp->cfilename);
				} else temp1->cfilename=NULL;
				if(temp->subkeyname!=NULL)
				{
					temp1->subkeyname=stralloc(temp->subkeyname);
					Rfree(temp->subkeyname);
				} else temp1->subkeyname=NULL;
				if(temp->expression!=NULL)
				{
					temp1->expression=stralloc(temp->expression);
					Rfree(temp->expression);
				} else temp1->expression=NULL;
				temp1->browse=temp->browse;
				if(temp->button_name!=NULL)
				{
					temp1->button_name=stralloc(temp->button_name);
					Rfree(temp->button_name);
				} else temp1->button_name=NULL;
				if(temp->browse_screen!=NULL)
				{
					temp1->browse_screen=stralloc(temp->browse_screen);
					Rfree(temp->browse_screen);
				} else temp1->browse_screen=NULL;
				if(temp->define_list!=NULL)
				{
					temp1->define_list=stralloc(temp->define_list);
					Rfree(temp->define_list);
				} else temp1->define_list=NULL;
				if(temp->search_browse!=NULL)
				{
					temp1->search_browse=stralloc(temp->search_browse);
					Rfree(temp->search_browse);
				} else temp1->search_browse=NULL;
				temp1->smart_browse=temp->smart_browse;
				temp1->range=temp->range;
				temp1->num_writes=temp->num_writes;
				++y;
			} else {
				if(temp->module!=NULL) Rfree(temp->module);
				if(temp->filename!=NULL) Rfree(temp->filename);
				if(temp->confld!=NULL)
				{
					for(z=0,confld1=temp->confld;z<temp->num;++z,++confld1)
					{
						if(confld1->fldname!=NULL) Rfree(confld1->fldname);
						if(confld1->conname!=NULL) Rfree(confld1->conname);
					}
					Rfree(temp->confld);
				}
				if(temp->cmodule!=NULL) Rfree(temp->cmodule);
				if(temp->cfilename!=NULL) Rfree(temp->cfilename);
				if(temp->subkeyname!=NULL) Rfree(temp->subkeyname);
				if(temp->expression!=NULL) Rfree(temp->expression);
				if(temp->button_name!=NULL) Rfree(temp->button_name);
				if(temp->browse_screen!=NULL) Rfree(temp->browse_screen);
				if(temp->define_list!=NULL) Rfree(temp->define_list);
				if(temp->search_browse!=NULL) Rfree(temp->search_browse);
			}	
		}
		Rfree(tempsupport);
		--tmpnumsupp;
		tempsupport=Rmalloc(tmpnumsupp*sizeof(RDAsearch));
		for(x=0,temp1=tempsupport,temp=new_fields;x<tmpnumsupp;++x,
			++temp,++temp1)
		{
			if(temp->module!=NULL)
			{
				temp1->module=stralloc(temp->module);
				Rfree(temp->module);
			} else temp1->module=NULL;
			if(temp->filename!=NULL)
			{
				temp1->filename=stralloc(temp->filename);
				Rfree(temp->filename);
			} else temp1->filename=NULL;
			temp1->ctype=temp->ctype;
			temp1->num=temp->num;
			if(temp->confld!=NULL)
			{
				temp1->confld=Rmalloc(temp->num*sizeof(RDAconfld));
				for(z=0,confld1=temp->confld,confld2=temp1->confld;
					z<temp->num;++z,++confld1,++confld2)
				{
					if(confld1->fldname!=NULL)
					{
						confld2->fldname=stralloc(confld1->fldname);
						Rfree(confld1->fldname);
					} else confld2->fldname=NULL;
					if(confld1->conname!=NULL)
					{
						confld2->conname=stralloc(confld1->conname);
						Rfree(confld1->conname);
					} else confld2->conname=NULL;
					confld2->inherit=confld1->inherit;
				}
				Rfree(temp->confld);
			} else temp1->confld=NULL;
			if(temp->cmodule!=NULL)
			{
				temp1->cmodule=stralloc(temp->cmodule);
				Rfree(temp->cmodule);
			} else temp1->cmodule=NULL;
			if(temp->cfilename!=NULL)
			{
				temp1->cfilename=stralloc(temp->cfilename);
				Rfree(temp->cfilename);
			} else temp1->cfilename=NULL;
			if(temp->subkeyname!=NULL)
			{
				temp1->subkeyname=stralloc(temp->subkeyname);
				Rfree(temp->subkeyname);
			} else temp1->subkeyname=NULL;
			if(temp->expression!=NULL)
			{
				temp1->expression=stralloc(temp->expression);
			} else temp1->expression=NULL;
			temp1->browse=temp->browse;
			if(temp->button_name!=NULL)
			{
				temp1->button_name=stralloc(temp->button_name);
			} else temp1->button_name=NULL;
			if(temp->browse_screen!=NULL)
			{
				temp1->browse_screen=stralloc(temp->browse_screen);
			} else temp1->browse_screen=NULL;
			if(temp->define_list!=NULL)
			{
				temp1->define_list=stralloc(temp->define_list);
			} else temp1->define_list=NULL;
			if(temp->search_browse!=NULL)
			{
				temp1->search_browse=stralloc(temp->search_browse);
			} else temp1->search_browse=NULL;
			temp1->smart_browse=temp->smart_browse;
			temp1->range=temp->range;
			temp1->num_writes=temp->num_writes;
		}
		Rfree(new_fields);
	} else if(tempsupport!=NULL)
	{
		for(x=0,temp=tempsupport;x<tmpnumsupp;++x,++temp)
		{
			if(temp->module!=NULL) Rfree(temp->module);
			if(temp->filename!=NULL) Rfree(temp->filename);
			if(temp->cmodule!=NULL) Rfree(temp->cmodule);
			if(temp->cfilename!=NULL) Rfree(temp->cfilename);
			if(temp->confld!=NULL)
			{
				for(z=0,confld1=temp->confld;z<temp->num;
					++z,++confld1)
				{
					if(confld1->fldname!=NULL) Rfree(confld1->fldname);
					if(confld1->conname!=NULL) Rfree(confld1->conname);
				}
				Rfree(temp->confld);
			}
			if(temp->subkeyname!=NULL) Rfree(temp->subkeyname);
			if(temp->expression!=NULL) Rfree(temp->expression);
			if(temp->button_name!=NULL) Rfree(temp->button_name);
			if(temp->browse_screen!=NULL) Rfree(temp->browse_screen);
			if(temp->define_list!=NULL) Rfree(temp->define_list);
			if(temp->search_browse!=NULL) Rfree(temp->search_browse);
		}
		Rfree(tempsupport);
		tmpnumsupp=0;
	}
	getsupports();
	if(value>=suppavlmtn->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(supprsrc,"SUPPORTING FILES",value,suppavlmtn))
		updatersrc(supprsrc,"SUPPORTING FILES");
	changesupports=TRUE;
}
void suppedit(RDArsrc *supprsrc,RDArsrc *mtnrsrc)
{
	int selected=0;
	RDAsearch *s;
	char *modx=NULL;

	if(FINDRSCGETINT(supprsrc,"SUPPORTING FILES",&selected)) return;
	if(tempsupport==NULL)
	{
		readwidget(mtnrsrc,"MODULE");
		FINDRSCGETSTRING(mtnrsrc,"MODULE",&modx);
		supportscreen(supprsrc,++selected,modx,NULL,0,NULL,NULL,
			NULL,0,NULL,FALSE,NULL,NULL,NULL,NULL,FALSE,
			NULL,savemsupp,quitmsupptest);
		if(modx!=NULL) Rfree(modx);
	} else {
		s=tempsupport+selected;
		if(s!=NULL)
		{
			supportscreen(supprsrc,++selected,s->module,s->filename,s->ctype,s->cmodule,s->cfilename,s->subkeyname,s->num,s->expression,s->browse,s->button_name,s->browse_screen,s->define_list,s->search_browse,s->smart_browse,s->confld,savemsupp,quitmsupptest);
		} else {
			readwidget(mtnrsrc,"MODULE");
			FINDRSCGETSTRING(mtnrsrc,"MODULE",&modx);
			supportscreen(supprsrc,++selected,modx,NULL,0,NULL,
				NULL,NULL,0,NULL,FALSE,NULL,NULL,NULL,NULL,FALSE,
				NULL,savemsupp,quitmsupptest);
			if(modx!=NULL) Rfree(modx);
		}
	}
}
void savesupp(RDArsrc *mtnrsrc,MaintainMaster *MTNMSTR)
{
	int x=0,z=0;
	RDAsearch *temp1,*temp;
	RDAconfld *confld1,*confld2;

	if(MTNMSTR->s!=NULL)
	{
		for(x=0,temp=MTNMSTR->s;x<MTNMSTR->num;++x,++temp)
		{
			if(temp->module!=NULL) Rfree(temp->module);
			if(temp->filename!=NULL) Rfree(temp->filename);
			if(temp->cmodule!=NULL) Rfree(temp->cmodule);
			if(temp->cfilename!=NULL) Rfree(temp->cfilename);
			if(temp->confld!=NULL)
			{
				for(z=0,confld1=temp->confld;z<temp->num;++z,++confld1)
				{
					if(confld1->fldname!=NULL) Rfree(confld1->fldname);
					if(confld1->conname!=NULL) Rfree(confld1->conname);
				}
				Rfree(temp->confld);
			}
			if(temp->subkeyname!=NULL) Rfree(temp->subkeyname);
			if(temp->expression!=NULL) Rfree(temp->expression);
			if(temp->button_name!=NULL) Rfree(temp->button_name);
			if(temp->browse_screen!=NULL) Rfree(temp->browse_screen);
			if(temp->define_list!=NULL) Rfree(temp->define_list);
			if(temp->search_browse!=NULL) Rfree(temp->search_browse);
		}
		Rfree(MTNMSTR->s);
		MTNMSTR->s=NULL;
		MTNMSTR->num=0;
	}
	if(tempsupport!=NULL)
	{
		MTNMSTR->num=tmpnumsupp;
		MTNMSTR->s=Rmalloc(tmpnumsupp*sizeof(RDAsearch));
		for(x=0,temp=tempsupport,temp1=MTNMSTR->s;x<tmpnumsupp;++x,++temp,
			++temp1)
		{
			if(temp->module!=NULL)
			{
				temp1->module=stralloc(temp->module);
				Rfree(temp->module);
			} else temp1->module=NULL;
			if(temp->filename!=NULL)
			{
				temp1->filename=stralloc(temp->filename);
				Rfree(temp->filename);
			} else temp1->filename=NULL;
			temp1->ctype=temp->ctype;
			if(temp->cmodule!=NULL)
			{
				temp1->cmodule=stralloc(temp->cmodule);
				Rfree(temp->cmodule);
			} else temp1->cmodule=NULL;
			if(temp->cfilename!=NULL)
			{
				temp1->cfilename=stralloc(temp->cfilename);
				Rfree(temp->cfilename);
			} else temp1->cfilename=NULL;
			temp1->num=temp->num;
			if(temp->confld!=NULL)
			{
				temp1->confld=Rmalloc(temp->num*sizeof(RDAconfld));
				for(z=0,confld1=temp->confld,confld2=temp1->confld;
					z<temp->num;++z,++confld2,++confld1)
				{
					if(confld1->fldname!=NULL)
					{
						confld2->fldname=stralloc(confld1->fldname);
						Rfree(confld1->fldname);
					} else confld2->fldname=NULL;
					if(confld1->conname!=NULL)
					{
						confld2->conname=stralloc(confld1->conname);
						Rfree(confld1->conname);
					} else confld2->conname=NULL;
					confld2->inherit=confld1->inherit;
				}
				Rfree(temp->confld);
			}
			if(temp->subkeyname!=NULL)
			{
				temp1->subkeyname=stralloc(temp->subkeyname);
				Rfree(temp->subkeyname);
			} else temp1->subkeyname=NULL;
			if(temp->expression!=NULL)
			{
				temp1->expression=stralloc(temp->expression);
			} else temp1->expression=NULL;
			temp1->browse=temp->browse;
			if(temp->button_name!=NULL)
			{
				temp1->button_name=stralloc(temp->button_name);
			} else temp1->button_name=NULL;
			if(temp->browse_screen!=NULL)
			{
				temp1->browse_screen=stralloc(temp->browse_screen);
			} else temp1->browse_screen=NULL;
			if(temp->define_list!=NULL)
			{
				temp1->define_list=stralloc(temp->define_list);
			} else temp1->define_list=NULL;
			if(temp->search_browse!=NULL)
			{
				temp1->search_browse=stralloc(temp->search_browse);
			} else temp1->search_browse=NULL;
			temp1->smart_browse=temp->smart_browse;
			temp1->range=temp->range;
			temp1->num_writes=temp->num_writes;
		}
		Rfree(tempsupport);
		tmpnumsupp=0;
		SetRDAsearchOccurrence(MTNMSTR);
	}
}
void suppcopy(RDArsrc *supprsrc,RDArsrc *mtnrsrc)
{
	int selected=0;
	RDAsearch *s;
	char *modx=NULL;

	if(FINDRSCGETINT(supprsrc,"SUPPORTING FILES",&selected)) return;
	if(tempsupport==NULL)
	{
		readwidget(mtnrsrc,"MODULE");
		FINDRSCGETSTRING(mtnrsrc,"MODULE",&modx);
		supportscreen(supprsrc,++selected,modx,NULL,0,NULL,NULL,
			NULL,0,NULL,FALSE,NULL,NULL,NULL,NULL,FALSE,
			NULL,savemsupp,quitmsupptest);
		if(modx!=NULL) Rfree(modx);
	} else {
		s=tempsupport+selected;
		++selected;
		if(s!=NULL)
		{
			supportscreen(supprsrc,++selected,s->module,s->filename,s->ctype,s->cmodule,s->cfilename,s->subkeyname,s->num,s->expression,s->browse,s->button_name,s->browse_screen,s->define_list,s->search_browse,s->smart_browse,s->confld,savemsuppbelow,quitmsuppbelowtest);
		} else {
			readwidget(mtnrsrc,"MODULE");
			FINDRSCGETSTRING(mtnrsrc,"MODULE",&modx);
			supportscreen(supprsrc,++selected,modx,NULL,0,NULL,
				NULL,NULL,0,NULL,FALSE,NULL,NULL,NULL,NULL,FALSE,
				NULL,savemsuppbelow,quitmsuppbelowtest);
			if(modx!=NULL) Rfree(modx);
		}
	}
}
