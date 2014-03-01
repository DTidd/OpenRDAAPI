/* dfaddsub - Define Supporting Files section of Define Power Add */
#include <cstdio>





#include <powadd.hpp>
#include <mkadd.hpp>
#include <rptgen.hpp>
#include <ldval.hpp>

extern void makeSearchBrowseScreen(PowerAdd *,RDAaddsub *);
extern void makeBrowseScreen(PowerAdd *,RDAaddsub *);
extern void makeBrowseDefineListScreen(PowerAdd *,RDAaddsub *);
extern APPlib *makefieldvallist2(RDAaddsub *,int,short,char *,char *);
extern APPlib *makefieldvallist(void);
extern PowerAdd *POWERADD;
extern RDArsrc *mtnrsrc;
extern char *module,changedvalues;
static void setconfldlist(RDArsrc *,APPlib *,char *);
static RDAdefault *msubsdefaults=NULL,*econdefaults=NULL;
static char changedsubords=FALSE;
static RDAconfld *holdconfld=NULL;
extern RDAaddsub *tempsubord;
extern int tmpnumsub;
extern APPlib *modlstx,*subavl,*filelst,*cnflds,*econflds;
extern APPlib *ConTypes,*BrowseTypes;
static APPlib *subkeylist=NULL;
static int holdnum=0;
static int ptrue=TRUE,pfalse=FALSE;
static void loadvalue(RDArsrc *,char *);
void savesub(void),subedit(RDArsrc *,RDArsrc *),subdelete(RDArsrc *);
void subaddabove(RDArsrc *,RDArsrc *),subaddbelow(RDArsrc *,RDArsrc *);
void getsubords(void);

static void loadvalue(RDArsrc *r,char *fieldname)
{
	int x,selectedm=0,selectedf=0;
	LoadValue *load;
	RDArsrc *ldvalrsrc1=NULL;
	char *modname=NULL,*filename=NULL;

	ldvalrsrc1=RDArsrcNEW("RPTGEN","LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc1,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	FINDRSCGETINT(r,"MODULE",&selectedm);
	FINDRSCGETINT(r,"FILENAME",&selectedf);
	if(RDAstrcmp(modlstx->libs[selectedm],"Contains No Database Libraries"))
	{
		modname=stralloc(modlstx->libs[selectedm]);
	}
	if(RDAstrcmp(filelst->libs[selectedf],"Contains No File Definitions"))
	{
		filename=stralloc(filelst->libs[selectedf]);
	}
	loadlist=makefieldvallist2(tempsubord,tmpnumsub,TRUE,modname,filename);
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
void getsubords()
{
	int x,len=0;
	RDAaddsub *subs;
	char *tmp=NULL;

	if(subavl!=NULL) freeapplib(subavl);
	subavl=APPlibNEW();
	if(tempsubord!=NULL)
	{
		for(x=0,subs=tempsubord;x<tmpnumsub;++x,++subs)
		{
			len=RDAstrlen(subs->module)+RDAstrlen(subs->filename)+18;
			if(tmp!=NULL) 
			{
				tmp=Rrealloc(tmp,len);
			} else {
				tmp=Rmalloc(len);
			}
			memset(tmp,0,len);
			sprintf(tmp,"%2d %s %s %s",(x+1),
				(subs->module!=NULL?subs->module:""),
				(subs->filename!=NULL?subs->filename:""),
				(subs->browse==TRUE ? "Browse":""));
			addAPPlib(subavl,tmp);
		}
	}
	if(subavl->numlibs<1)
	{
		addAPPlib(subavl,"No Files Defined");
	}
}
static void quitmsubo(RDArsrc *msuborsrc,RDArsrc *subrsrc)
{
	int x;
	RDAconfld *cf;

	if(msuborsrc!=NULL)
	{
		killwindow(msuborsrc);
		free_rsrc(msuborsrc);
	}
	if(modlstx!=NULL) freeapplib(modlstx);
	if(filelst!=NULL) freeapplib(filelst);
	if(cnflds!=NULL) freeapplib(cnflds);
	if(subkeylist!=NULL) freeapplib(subkeylist);
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
	if(changedsubords) changedvalues=TRUE;
	if(msubsdefaults!=NULL) FreeRDAdefault(msubsdefaults);
}
static void makespacesubord(int pos)
{
	short x,y,z;
	RDAaddsub *temp,*temp1,*new_fields;
	RDAconfld *confld1,*confld2;

	++tmpnumsub;
	new_fields=Rmalloc(tmpnumsub*sizeof(RDAaddsub));
	y=0;
	for(x=0,temp=tempsubord;x<(tmpnumsub-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->module=NULL;
			temp1->filename=NULL;
			temp1->subkeyname=NULL;
			temp1->dtype=0;
			temp1->num=0;
			temp1->confld=NULL;
			temp1->browse=FALSE;
			temp1->browse_screen=NULL;
			temp1->expression=NULL;
			temp1->button_name=NULL;
			temp1->define_list=NULL;
			temp1->search_browse=NULL;
			temp1->smart_browse=FALSE;
			temp1->BL=NULL;
			++y;
			temp1=new_fields+y;
		}
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
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
		if(temp->subkeyname!=NULL)
		{
			temp1->subkeyname=stralloc(temp->subkeyname);
			Rfree(temp->subkeyname);
		} else temp1->subkeyname=NULL;
		temp1->dtype=temp->dtype;
		temp1->browse=temp->browse;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
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
		temp1->num=temp->num;
		temp1->BL=NULL;
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
		++y;
	}
	Rfree(tempsubord);
	tempsubord=Rmalloc(tmpnumsub*sizeof(RDAaddsub));
	for(x=0,temp1=tempsubord,temp=new_fields;x<tmpnumsub;++x,
		++temp,++temp1)
	{
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
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
		if(temp->subkeyname!=NULL)
		{
			temp1->subkeyname=stralloc(temp->subkeyname);
			Rfree(temp->subkeyname);
		} else temp1->subkeyname=NULL;
		temp1->num=temp->num;
		temp1->dtype=temp->dtype;
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
		temp1->BL=NULL;
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
	}
	Rfree(new_fields);
}
static void savemsubo(RDArsrc *msuborsrc,RDArsrc *subrsrc)
{
	int value=0,selectedm=0,selectedf=0,selectsubkey=0;
	RDAaddsub *subs;
	int x=0;
	RDAconfld *cf1,*cf2;
	char def_browse_screen=FALSE,def_define_list=FALSE,def_search_browse=FALSE;

	if(FINDRSCGETINT(subrsrc,"SUPPORTING FILES",&value)) return;
	if(tempsubord==NULL)
	{
		tempsubord=Rmalloc(sizeof(RDAaddsub));
		tmpnumsub=1;
	}
	subs=tempsubord+value;
	readallwidgets(msuborsrc);
	FINDRSCGETINT(msuborsrc,"MODULE",&selectedm);
	FINDRSCGETINT(msuborsrc,"FILENAME",&selectedf);
	subs->module=stralloc(modlstx->libs[selectedm]);
	subs->filename=stralloc(filelst->libs[selectedf]);
	FINDRSCGETSTRING(msuborsrc,"EXPRESSION",&subs->expression);
	FINDRSCGETCHAR(msuborsrc,"BROWSE",&subs->browse);
	FINDRSCGETSTRING(msuborsrc,"BROWSE SCREEN",&subs->browse_screen);
	FINDRSCGETSTRING(msuborsrc,"BUTTON NAME",&subs->button_name);
	FINDRSCGETSTRING(msuborsrc,"DEFINE LIST",&subs->define_list);
	FINDRSCGETSTRING(msuborsrc,"SEARCH BROWSE",&subs->search_browse);
	FINDRSCGETCHAR(msuborsrc,"DEFAULT BROWSE SCREEN",&def_browse_screen);
	FINDRSCGETCHAR(msuborsrc,"DEFAULT SEARCH BROWSE",&def_search_browse);
	FINDRSCGETCHAR(msuborsrc,"DEFAULT DEFINE LIST",&def_define_list);
	FINDRSCGETINT(msuborsrc,"BROWSE TYPES",&x);
	subs->smart_browse=x;
	FINDRSCGETINT(msuborsrc,"CONNECT TYPES",&subs->dtype);
	FINDRSCGETINT(msuborsrc,"KEYNAME",&selectsubkey);
	subs->subkeyname=stralloc(subkeylist->libs[selectsubkey]);
	if(holdconfld!=NULL)
	{
		subs->num=holdnum;
		subs->confld=Rmalloc((holdnum)*sizeof(RDAconfld));
		for(x=0,cf1=holdconfld,cf2=subs->confld;x<holdnum;++x,++cf1,++cf2)
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
	if(def_search_browse) makeSearchBrowseScreen(POWERADD,subs);
	if(def_browse_screen) makeBrowseScreen(POWERADD,subs);
	if(def_define_list) makeBrowseDefineListScreen(POWERADD,subs);
	getsubords();
	if(!FINDRSCLISTAPPlib(subrsrc,"SUPPORTING FILES",value,subavl))
		updatersrc(subrsrc,"SUPPORTING FILES");
	if(COMPARE_RSRCS(msuborsrc,msubsdefaults,2))
	{
		changedsubords=TRUE;
	}
	quitmsubo(msuborsrc,subrsrc);
}
static void savemsuboabove(RDArsrc *msuborsrc,RDArsrc *subrsrc)
{
	int x;

	if(FINDRSCGETINT(subrsrc,"SUPPORTING FILES",&x)) return;
	makespacesubord(x);
	savemsubo(msuborsrc,subrsrc);
}
static void savemsubobelow(RDArsrc *msuborsrc,RDArsrc *subrsrc)
{
	int x;

	if(FINDRSCGETINT(subrsrc,"SUPPORTING FILES",&x)) return;
	makespacesubord(++x);
	FINDRSCSETINT(subrsrc,"SUPPORTING FILES",x);
	savemsubo(msuborsrc,subrsrc);
}
static void getmodulelist()
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
		addAPPlib(modlstx,"Contains No Database Libraries");
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	SORTAPPlib(modlstx);
}
static void getconflds()
{
	int x;
	RDAconfld *cf1;
	char *temp=NULL;

	if(cnflds!=NULL) freeapplib(cnflds);
	cnflds=APPlibNEW();
	for(x=0,cf1=holdconfld;x<holdnum;++x,++cf1)
	{
		if(temp!=NULL)
		{
			temp=Rrealloc(temp,RDAstrlen(cf1->fldname)+RDAstrlen(cf1->conname)+22);
		} else {
			temp=Rmalloc(RDAstrlen(cf1->fldname)+RDAstrlen(cf1->conname)+22);
		}
		sprintf(temp,"Field [%s] connects to %s",(cf1->fldname!=NULL?cf1->fldname:""),(cf1->conname!=NULL?cf1->conname:""));
		addAPPlib(cnflds,temp);
	}
	if(cnflds->numlibs<1)
	{
		addAPPlib(cnflds,"No Connecting Fields");
	}
}
static void setconflds(RDArsrc *msuborsrc)
{
	int y,z,selectedm=0,selectedf=0,selectedkey=0;
/*
	int selcf=0;
*/
	NRDfield *fields=NULL,*f;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short nofields=0,nokeys=0,engine=0;
	RDAconfld *cf;
/*
	char *modulename=NULL,*filename=NULL;
*/

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
	if(FINDRSCGETINT(msuborsrc,"MODULE",&selectedm)) return;
	if(FINDRSCGETINT(msuborsrc,"FILENAME",&selectedf)) return;
	if(FINDRSCGETINT(msuborsrc,"KEYNAME",&selectedkey)) return;
	if(selectedm>=modlstx->numlibs) 
	{
		selectedm=0;
	}
	if(selectedf>=filelst->numlibs)
	{
		selectedf=0;
	}
	if(!GETBIN(modlstx->libs[selectedm],filelst->libs[selectedf],&engine,&nofields,&fields,&nokeys,&keys)) 
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
				if(keyx->part!=NULL && !RDAstrcmp(keyx->name,subkeylist->libs[selectedkey]))
				{
					holdnum=keyx->numparts;
					holdconfld=Rmalloc(holdnum*sizeof(RDAconfld));
					for(z=0,cf=holdconfld,part=keyx->part;z<keyx->numparts;++z,++cf,++part)
                                        {
						if(part->name!=NULL)
						{
							cf->fldname=stralloc(part->name);
							Rfree(part->name);
						}  else cf->fldname=NULL;
					}
					Rfree(keyx->part);
				}
			}
			Rfree(keys);
			keys=NULL;
		}
/*
		if(holdconfld!=NULL)
		{
			if(FINDRSCGETINT(msuborsrc,"KEYNAME",&selectedkey)) return;
			if(FINDRSCGETINT(msuborsrc,"CONNECT FILENAME",&selcf)) return;
			filename=stripfilename(cmodlst->libs[selcf]);
			if(filename!=NULL) modulename=stripmodulename(cmodlst->libs[selcf]);
			if(modulename!=NULL && filename!=NULL)
			{
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
			if(filename!=NULL) Rfree(filename);
			if(modulename!=NULL) Rfree(modulename);
		}
*/
	}
	getconflds();
	if(!FINDRSCLISTAPPlib(msuborsrc,"CONNECT FIELDNAME",0,cnflds))
		updatersrc(msuborsrc,"CONNECT FIELDNAME");
}
static void quitecon(RDArsrc *econrsrc,RDArsrc *msuborsrc)
{
	if(econrsrc!=NULL)
	{
		killwindow(econrsrc);
		free_rsrc(econrsrc);
	}
	if(econflds!=NULL) freeapplib(econflds);
	if(econdefaults!=NULL) FreeRDAdefault(econdefaults);
}
static void saveecon(RDArsrc *econrsrc,RDArsrc *msuborsrc)
{
	int selected=0,selhold=0;
	char *fldname=NULL;
	RDAconfld *c;

	if(FINDRSCGETINT(msuborsrc,"CONNECT FIELDNAME",&selhold)) return;
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
				c->conname=stralloc(econflds->libs[selected]);
				getconflds();
				if(!FINDRSCLISTAPPlib(msuborsrc,"CONNECT FIELDNAME",
					selhold,cnflds))
				{
					updatersrc(msuborsrc,"CONNECT FIELDNAME");
				}
			}
		}
	}
	if(COMPARE_RSRCS(econrsrc,econdefaults,2))
	{
		changedsubords=TRUE;
	}
	quitecon(econrsrc,msuborsrc);
}
static void quitecontest(RDArsrc *econrsrc,RDArsrc *msuborsrc)
{
	readallwidgets(econrsrc);
	if(COMPARE_RSRCS(econrsrc,econdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","MASTER CHANGES NOT SAVED!","Changes have been made to this CONNECT FIELD!\nDo you want to Save these changes?",saveecon,quitecon,FALSE,2,econrsrc,msuborsrc,NULL);
	} else {
		quitecon(econrsrc,msuborsrc);
	}
}
static void setconfldlist(RDArsrc *rsrc,APPlib *list,char *fieldvalue)
{
	int x=(-1);
	char *warndesc=NULL;

	if(!isEMPTY(fieldvalue))
	{
		if((x=FINDAPPLIBELEMENT(list,fieldvalue))==(-1))
		{
			warndesc=Rmalloc(RDAstrlen(fieldvalue)+250);
			sprintf(warndesc,"The value of [%s] is an Invalid Virtual/File Field Value.  It doesn't exist in this report's Virtual Fields or the connected files.  The List for this field is being set to the value [%s].",(fieldvalue==NULL?"":fieldvalue),list->libs[
0]);
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID FIELD VALUE!",warndesc,NULL,NULL,FALSE,NULL);
			Rfree(warndesc);
			x=0;
		}
		FINDRSCSETINT(rsrc,"CONNECT FIELDS",x);
		updatersrc(rsrc,"CONNECT FIELDS");
	}
}
static void editconnect(RDArsrc *msuborsrc,RDArsrc *subrsrc)
{
	int x,selectfld=0;
	char inherit=FALSE,*fldname=NULL,*cfldname=NULL;
	RDAconfld *c;
	RDArsrc *econrsrc=NULL;

	readallwidgets(msuborsrc);
	if(FINDRSCGETINT(msuborsrc,"CONNECT FIELDNAME",&selectfld)) return;
	if(holdconfld!=NULL && selectfld<holdnum)
	{
		c=holdconfld+selectfld;
		fldname=stralloc(c->fldname);
		cfldname=stralloc(c->conname);
		inherit=c->inherit;
	} 
	econrsrc=RDArsrcNEW(module,"POWERADD EDIT CONNECT FIELD");
	addstdrsrc(econrsrc,"FIELD NAME",VARIABLETEXT,0,
		(fldname!=NULL ? fldname:""),FALSE);
	if(fldname!=NULL) Rfree(fldname);
	if(econflds!=NULL) freeapplib(econflds);
	econflds=APPlibNEW();
	x=0;
	econflds=makefieldvallist();
	if(econflds->numlibs<1)
	{
		addAPPlib(econflds,"No Connecting Fields Available");
	}
	if(!x)
	{
		for(x=0;x<econflds->numlibs;++x)
		{
			if(strncmp(econflds->libs[x],"GENERIC_SETUP_VARIABLE",22)) break;
		}
		if(x>=econflds->numlibs) x=0;
	}
	addlstrsrc(econrsrc,"CONNECT FIELDS",&x,TRUE,NULL,econflds->numlibs,
		&econflds->libs,NULL);
	addstdrsrc(econrsrc,"INHERIT",BOOLNS,1,&inherit,TRUE);
	addrfcbrsrc(econrsrc,"SAVE",TRUE,saveecon,msuborsrc);
	addrfcbrsrc(econrsrc,"QUIT",TRUE,quitecontest,msuborsrc);
	addbtnrsrc(econrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(econrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(econrsrc,TRUE,quitecon,msuborsrc,FALSE);
	setconfldlist(econrsrc,econflds,cfldname);
	if(cfldname!=NULL) Rfree(cfldname);
	econdefaults=GetDefaults(econrsrc);
}
/*
static void dofilelist(RDArsrc *msuborsrc,int *passfunc)
{
	if(*passfunc) setconflds(msuborsrc);
}
*/
static void dofilelist(RDArsrc *msuborsrc,int *passfunc)
{
	int x,y,z,selectedm,selectedf;
	NRDfield *fields=NULL,*f;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short nofields=0,nokeys=0,engine=0;

	if(FINDRSCGETINT(msuborsrc,"MODULE",&selectedm)) return;
	if(FINDRSCGETINT(msuborsrc,"FILENAME",&selectedf)) return;
	if(subkeylist!=NULL) freeapplib(subkeylist);
	subkeylist=APPlibNEW();
	if(selectedm>=modlstx->numlibs)
	{
		selectedm=0;
	}
	if(selectedf>=filelst->numlibs)
	{
		selectedf=0;
	}
	if(!GETBIN(modlstx->libs[selectedm],filelst->libs[selectedf],&engine,&nofields,&fields,&nokeys,&keys))
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
				addAPPlib(subkeylist,keyx->name);
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
	if(subkeylist->numlibs<1)
	{
		addAPPlib(subkeylist,"No Keys Available");
	}
	x=0;
	if(!FINDRSCLISTAPPlib(msuborsrc,"KEYNAME",x,subkeylist))
	{
		updatersrc(msuborsrc,"KEYNAME");
		if(*passfunc) setconflds(msuborsrc);
	}
}
static void domodulelist(RDArsrc *msuborsrc,int *passfunc)
{
	int selected=0,selectedf=0;
	char *libx=NULL;

	readwidget(msuborsrc,"MODULE");
	readwidget(msuborsrc,"FILENAME");
	if(FINDRSCGETINT(msuborsrc,"MODULE",&selected)) return;
	if(FINDRSCGETINT(msuborsrc,"FILENAME",&selectedf)) return;
	if(filelst!=NULL) freeapplib(filelst);
	if(selected>=modlstx->numlibs) 
	{
		selected=0;
	}
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlstx->libs[selected])+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modlstx->libs[selected]);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modlstx->libs[selected]);
#endif
	filelst=getlibnames(libx,TRUE);
	if(libx!=NULL) Rfree(libx);
	if(filelst==NULL)
	{
		filelst=APPlibNEW();
		addAPPlib(filelst,"Contains No File Definitions");
	}
	if(selectedf>=filelst->numlibs) selectedf=0;
	if(!FINDRSCLISTAPPlib(msuborsrc,"FILENAME",selectedf,filelst))
	{
		updatersrc(msuborsrc,"FILENAME");
		dofilelist(msuborsrc,passfunc);
	}
}
static void setbrowsesensitive(RDArsrc *msuborsrc)
{
	char brs=FALSE;

	readwidget(msuborsrc,"BROWSE");
	FINDRSCGETCHAR(msuborsrc,"BROWSE",&brs);
	if(brs)
	{
		FINDRSCSETSENSITIVE(msuborsrc,"BROWSE SCREEN",TRUE);
		FINDRSCSETSENSITIVE(msuborsrc,"DEFINE LIST",TRUE);
		FINDRSCSETSENSITIVE(msuborsrc,"SEARCH BROWSE",TRUE);
		FINDRSCSETEDITABLE(msuborsrc,"BROWSE SCREEN",TRUE);
		FINDRSCSETEDITABLE(msuborsrc,"DEFINE LIST",TRUE);
		FINDRSCSETEDITABLE(msuborsrc,"SEARCH BROWSE",TRUE);
		FINDRSCSETSENSITIVE(msuborsrc,"BUTTON NAME",TRUE);
		FINDRSCSETEDITABLE(msuborsrc,"BUTTON NAME",TRUE);
		FINDRSCSETSENSITIVE(msuborsrc,"BROWSE TYPES",TRUE);
		FINDRSCSETEDITABLE(msuborsrc,"BROWSE TYPES",TRUE);
		FINDRSCSETSENSITIVE(msuborsrc,"DEFAULT BROWSE SCREEN",TRUE);
		FINDRSCSETEDITABLE(msuborsrc,"DEFAULT BROWSE SCREEN",TRUE);
		FINDRSCSETSENSITIVE(msuborsrc,"DEFAULT DEFINE LIST",TRUE);
		FINDRSCSETEDITABLE(msuborsrc,"DEFAULT DEFINE LIST",TRUE);
		FINDRSCSETSENSITIVE(msuborsrc,"DEFAULT SEARCH BROWSE",TRUE);
		FINDRSCSETEDITABLE(msuborsrc,"DEFAULT SEARCH BROWSE",TRUE);
	} else {
		FINDRSCSETSENSITIVE(msuborsrc,"BROWSE SCREEN",FALSE);
		FINDRSCSETSENSITIVE(msuborsrc,"DEFINE LIST",FALSE);
		FINDRSCSETSENSITIVE(msuborsrc,"SEARCH BROWSE",FALSE);
		FINDRSCSETEDITABLE(msuborsrc,"BROWSE SCREEN",FALSE);
		FINDRSCSETEDITABLE(msuborsrc,"DEFINE LIST",FALSE);
		FINDRSCSETEDITABLE(msuborsrc,"SEARCH BROWSE",FALSE);
		FINDRSCSETSENSITIVE(msuborsrc,"BUTTON NAME",FALSE);
		FINDRSCSETEDITABLE(msuborsrc,"BUTTON NAME",FALSE);
		FINDRSCSETSENSITIVE(msuborsrc,"BROWSE TYPES",FALSE);
		FINDRSCSETEDITABLE(msuborsrc,"BROWSE TYPES",FALSE);
		FINDRSCSETSENSITIVE(msuborsrc,"DEFAULT BROWSE SCREEN",FALSE);
		FINDRSCSETEDITABLE(msuborsrc,"DEFAULT BROWSE SCREEN",FALSE);
		FINDRSCSETSENSITIVE(msuborsrc,"DEFAULT DEFINE LIST",FALSE);
		FINDRSCSETEDITABLE(msuborsrc,"DEFAULT DEFINE LIST",FALSE);
		FINDRSCSETSENSITIVE(msuborsrc,"DEFAULT SEARCH BROWSE",FALSE);
		FINDRSCSETEDITABLE(msuborsrc,"DEFAULT SEARCH BROWSE",FALSE);
	}
}
static void subordscreen(RDArsrc *subrsrc,int num,char *modulex,char *filename,
	char *subkeyname,int dtype,int number,char *expression,char browse,
	char *button_name,char *browse_screen,char *define_list,
	char *search_browse,char smart_browse,RDAconfld *confld,
	void (*savefunc)(...),void (*quitfunctest)(...))
{
	int x=0,y=0,selmod=0,selfile=0;
	char *temp=NULL,*tmod=NULL,*curmod=NULL,make_default=FALSE;
	NRDfield *fields=NULL,*f;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short nofields=0,nokeys=0,engine=0;
/*
	RDAaddsub *subs;
*/
	RDAconfld *cf,*cf1;
	RDArsrc *msuborsrc=NULL;

	msuborsrc=RDArsrcNEW(module,"POWER ADD SUPPORTING FILE");
	addstdrsrc(msuborsrc,"NUMBER",LONGV,8,&num,FALSE);
	getmodulelist();
	if(modulex!=NULL)
	{
		for(x=0;x<modlstx->numlibs;++x)
		{
			if(!RDAstrcmp(modlstx->libs[x],modulex)) break;
		}
	}
	if(x>=modlstx->numlibs)
	{
		FINDRSCGETSTRING(mtnrsrc,"MODULE",&tmod);
		for(x=0;x<modlstx->numlibs;++x)
		{
			if(!RDAstrcmp(modlstx->libs[x],tmod)) break;
		}
		if(tmod!=NULL) Rfree(tmod);
		if(x>=modlstx->numlibs) x=0;
	}
	addlstrsrc(msuborsrc,"MODULE",&x,TRUE,domodulelist,modlstx->numlibs,
		&modlstx->libs,&ptrue);
	if(filelst!=NULL) freeapplib(filelst);
	filelst=APPlibNEW();
	addlstrsrc(msuborsrc,"FILENAME",&x,TRUE,dofilelist,filelst->numlibs,
		&filelst->libs,&ptrue);
	readwidget(subrsrc,"MODULE");
	FINDRSCGETSTRING(subrsrc,"MODULE",&curmod);
	if(cnflds!=NULL) freeapplib(cnflds);
	cnflds=APPlibNEW();
	if(confld!=NULL && number)
	{
		holdnum=number;
		holdconfld=Rmalloc(number*sizeof(RDAconfld));
		for(x=0,cf1=holdconfld,cf=confld;x<number;++x,++cf,++cf1)
		{
			if(temp!=NULL)
			{
				temp=Rrealloc(temp,RDAstrlen(cf->fldname)+RDAstrlen(cf->conname)+24);
			} else {
				temp=Rmalloc(RDAstrlen(cf->fldname)+RDAstrlen(cf->conname)+24);
			}
			cf1->fldname=stralloc(cf->fldname);
			cf1->conname=stralloc(cf->conname);
			cf1->inherit=cf->inherit;
			sprintf(temp,"Field [%s] connects to %s",(cf->fldname!=NULL ? cf->fldname:""),(cf->conname!=NULL ? cf->conname:""));
			addAPPlib(cnflds,temp);
		}
	}
	if(subkeylist!=NULL) freeapplib(subkeylist);
	subkeylist=APPlibNEW();
	x=0;
	addlstrsrc(msuborsrc,"KEYNAME",&x,TRUE,setconflds,subkeylist->numlibs,
		&subkeylist->libs,NULL);
	selmod=0;
	domodulelist(msuborsrc,&pfalse);
	if(modulex!=NULL)
	{
		for(selmod=0;selmod<modlstx->numlibs;++selmod)
		{
			if(!RDAstrcmp(modlstx->libs[selmod],modulex)) break;
		}
	}
	if(selmod>=modlstx->numlibs)
	{
		FINDRSCGETSTRING(mtnrsrc,"MODULE",&tmod);
		for(selmod=0;selmod<modlstx->numlibs;++selmod)
		{
			if(!RDAstrcmp(modlstx->libs[selmod],tmod)) break;
		}
		if(tmod!=NULL) Rfree(tmod);
		if(selmod>=modlstx->numlibs) selmod=0;
	}
	dofilelist(msuborsrc,&pfalse); 
	selfile=0;
	if(filename!=NULL)
	{
		for(selfile=0;selfile<filelst->numlibs;++selfile)
		{
			if(!RDAstrcmp(filelst->libs[selfile],filename)) break;
		}
	}
	if(selfile>=filelst->numlibs)
	{
		selfile=0;
	}
	if(!FINDRSCLISTAPPlib(msuborsrc,"FILENAME",selfile,filelst))
	{
		updatersrc(msuborsrc,"FILENAME");
	}                
	if(subkeylist!=NULL) freeapplib(subkeylist);
	subkeylist=APPlibNEW();
	if(!GETBIN(modlstx->libs[selmod],filelst->libs[selfile],&engine,&nofields,&fields,&nokeys,&keys))
	{
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
				addAPPlib(subkeylist,keyx->name);
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
	if(subkeylist->numlibs<1)
	{
		addAPPlib(subkeylist,"No Keys Available");
	}
	if(subkeyname!=NULL)
	{
		for(x=0;x<subkeylist->numlibs;++x)
		{
			if(!RDAstrcmp(subkeylist->libs[x],subkeyname))
			{
				break;
			}
		}
	}
	if(x>=subkeylist->numlibs)
	{
		x=0;
	}
	if(!FINDRSCLISTAPPlib(msuborsrc,"KEYNAME",x,subkeylist))
	{
		updatersrc(msuborsrc,"KEYNAME");
	}
	if(cnflds->numlibs<1)
	{
		addAPPlib(cnflds,"No Connecting Fields");
		setconflds(msuborsrc);
	}
	x=0;
	addlstrsrc(msuborsrc,"CONNECT TYPES",&dtype,TRUE,NULL,ConTypes->numlibs,&ConTypes->libs,NULL);
	addlstrsrc(msuborsrc,"CONNECT FIELDNAME",&x,TRUE,NULL,cnflds->numlibs,
		&cnflds->libs,NULL);
	addbtnrsrc(msuborsrc,"SELECT",TRUE,editconnect,subrsrc);
	addsctrsrc(msuborsrc,"EXPRESSION",0,expression,TRUE);
	addbtnrsrc(msuborsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(msuborsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(msuborsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(msuborsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");

	addstdrsrc(msuborsrc,"BROWSE",BOOLNS,1,&browse,TRUE);
	FINDRSCSETFUNC(msuborsrc,"BROWSE",setbrowsesensitive,NULL);
	addstdrsrc(msuborsrc,"BUTTON NAME",VARIABLETEXT,0,button_name,TRUE);
	addstdrsrc(msuborsrc,"BROWSE SCREEN",VARIABLETEXT,0,browse_screen,TRUE);
	addstdrsrc(msuborsrc,"DEFINE LIST",VARIABLETEXT,0,define_list,TRUE);
	addstdrsrc(msuborsrc,"SEARCH BROWSE",VARIABLETEXT,0,search_browse,TRUE);
	addstdrsrc(msuborsrc,"DEFAULT BROWSE SCREEN",BOOLNS,1,&make_default,TRUE);
	addstdrsrc(msuborsrc,"DEFAULT DEFINE LIST",BOOLNS,1,&make_default,TRUE);
	addstdrsrc(msuborsrc,"DEFAULT SEARCH BROWSE",BOOLNS,1,&make_default,TRUE);

	x=smart_browse;
	addlstrsrc(msuborsrc,"BROWSE TYPES",&x,TRUE,NULL,BrowseTypes->numlibs,&BrowseTypes->libs,NULL);
	addrfcbrsrc(msuborsrc,"SAVE",TRUE,savefunc,subrsrc);
	addrfcbrsrc(msuborsrc,"QUIT",TRUE,quitfunctest,subrsrc);
	addbtnrsrc(msuborsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(msuborsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	setbrowsesensitive(msuborsrc);
	if(temp!=NULL) Rfree(temp);
	msubsdefaults=GetDefaults(msuborsrc);
	changedsubords=FALSE;
	APPmakescrn(msuborsrc,TRUE,quitmsubo,subrsrc,FALSE);
}
static void quitmsubotest(RDArsrc *msuborsrc,RDArsrc *subrsrc)
{
	readallwidgets(msuborsrc);
	if(COMPARE_RSRCS(msuborsrc,msubsdefaults,2) || changedsubords)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","MASTER CHANGES NOT SAVED!","Changes have been made to this SUPPORTING FILE DEFININTION!\nDo you want to Save these changes?",savemsubo,quitmsubo,FALSE,2,msuborsrc,subrsrc,NULL);
	} else {
		quitmsubo(msuborsrc,subrsrc);
	}
}
static void quitmsuboabovetest(RDArsrc *msuborsrc,RDArsrc *subrsrc)
{
	readallwidgets(msuborsrc);
	if(COMPARE_RSRCS(msuborsrc,msubsdefaults,2) || changedsubords)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","MASTER CHANGES NOT SAVED!","Changes have been made to this SURORDINATE FILE DEFININTION!\nDo you want to Save these changes?",savemsuboabove,quitmsubo,FALSE,2,msuborsrc,subrsrc,NULL);
	} else {
		quitmsubo(msuborsrc,subrsrc);
	}
}
static void quitmsubobelowtest(RDArsrc *msuborsrc,RDArsrc *subrsrc)
{
	readallwidgets(msuborsrc);
	if(COMPARE_RSRCS(msuborsrc,msubsdefaults,2) || changedsubords)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","MASTER CHANGES NOT SAVED!","Changes have been made to this SUPPORTING FILE DEFININTION!\nDo you want to Save these changes?",savemsubobelow,quitmsubo,FALSE,2,msuborsrc,subrsrc,NULL);
	} else {
		quitmsubo(msuborsrc,subrsrc);
	}
}
void subaddabove(RDArsrc *subrsrc,RDArsrc *mtnrsrc)
{
	int selected=0;
	char *modx=NULL;

	if(FINDRSCGETINT(subrsrc,"SUPPORTING FILES",&selected)) return;
	if(selected==0 && !RDAstrcmp(subavl->libs[selected],"No Files Defined"))
		return;
	readwidget(mtnrsrc,"MODULE");
	FINDRSCGETSTRING(mtnrsrc,"MODULE",&modx);
	subordscreen(subrsrc,++selected,modx,NULL,NULL,0,0,NULL,
		0,NULL,NULL,NULL,NULL,FALSE,
		NULL,savemsuboabove,quitmsuboabovetest);
	if(modx!=NULL) Rfree(modx);
}
void subaddbelow(RDArsrc *subrsrc,RDArsrc *mtnrsrc)
{
	int selected=0;
	char *modx=NULL;

	if(FINDRSCGETINT(subrsrc,"SUPPORTING FILES",&selected)) return;
	if(selected==0 && !RDAstrcmp(subavl->libs[selected],"No Files Defined"))
		return;
	selected+=2;
	readwidget(mtnrsrc,"MODULE");
	FINDRSCGETSTRING(mtnrsrc,"MODULE",&modx);
	subordscreen(subrsrc,selected,modx,NULL,NULL,0,0,NULL,
		0,NULL,NULL,NULL,NULL,FALSE,
		NULL,savemsubobelow,quitmsubobelowtest);
	if(modx!=NULL) Rfree(modx);
}
void subdelete(RDArsrc *subrsrc)
{
	short x,y,z;
	RDAaddsub *temp,*temp1,*new_fields;
	int value;
	RDAconfld *confld1,*confld2;

	readallwidgets(subrsrc);
	if(FINDRSCGETINT(subrsrc,"SUPPORTING FILES",&value)) return;
	if(tmpnumsub<1) return;
	if(tmpnumsub>1)
	{
		new_fields=Rmalloc((tmpnumsub-1)*sizeof(RDAaddsub));
		y=0;
		for(x=0,temp=tempsubord;x<tmpnumsub;++x,++temp)
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
				if(temp->subkeyname!=NULL)
				{
					temp1->subkeyname=stralloc(temp->subkeyname);
					Rfree(temp->subkeyname);
				} else temp1->subkeyname=NULL;
				temp1->dtype=temp->dtype;
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
				if(temp->expression!=NULL)
				{
					temp1->expression=stralloc(temp->expression);
					Rfree(temp->expression);
				} else temp1->expression=NULL;
				temp1->BL=NULL;
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
				temp1->BL=NULL;
				++y;
			} else {
				if(temp->module!=NULL) Rfree(temp->module);
				if(temp->filename!=NULL) Rfree(temp->filename);
				if(temp->subkeyname!=NULL) Rfree(temp->subkeyname);
				if(temp->browse_screen!=NULL) Rfree(temp->browse_screen);
				if(temp->button_name!=NULL) Rfree(temp->button_name);
				if(temp->define_list!=NULL) Rfree(temp->define_list);
				if(temp->search_browse!=NULL) Rfree(temp->search_browse);
				if(temp->expression!=NULL) Rfree(temp->expression);
				if(temp->confld!=NULL)
				{
					for(z=0,confld1=temp->confld;z<temp->num;++z,++confld1)
					{
						if(confld1->fldname!=NULL) Rfree(confld1->fldname);
						if(confld1->conname!=NULL) Rfree(confld1->conname);
					}
					Rfree(temp->confld);
				}
			}	
		}
		Rfree(tempsubord);
		--tmpnumsub;
		tempsubord=Rmalloc(tmpnumsub*sizeof(RDAaddsub));
		for(x=0,temp1=tempsubord,temp=new_fields;x<tmpnumsub;++x,
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
			if(temp->subkeyname!=NULL)
			{
				temp1->subkeyname=stralloc(temp->subkeyname);
				Rfree(temp->subkeyname);
			} else temp1->subkeyname=NULL;
			temp1->dtype=temp->dtype;
			temp1->browse=temp->browse;
			if(temp->expression!=NULL)
			{
				temp1->expression=stralloc(temp->expression);
				Rfree(temp->expression);
			} else temp1->expression=NULL;
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
			temp1->BL=NULL;
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
			temp1->BL=NULL;
		}
		Rfree(new_fields);
	} else if(tempsubord!=NULL)
	{
		for(x=0,temp=tempsubord;x<tmpnumsub;++x,++temp)
		{
			if(temp->module!=NULL) Rfree(temp->module);
			if(temp->filename!=NULL) Rfree(temp->filename);
			if(temp->subkeyname!=NULL) Rfree(temp->subkeyname);
			if(temp->browse_screen!=NULL) Rfree(temp->browse_screen);
			if(temp->button_name!=NULL) Rfree(temp->button_name);
			if(temp->define_list!=NULL) Rfree(temp->define_list);
			if(temp->expression!=NULL) Rfree(temp->expression);
			if(temp->search_browse!=NULL) Rfree(temp->search_browse);
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
		}
		Rfree(tempsubord);
		tmpnumsub=0;
	}
	getsubords();
	if(value>=subavl->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(subrsrc,"SUPPORTING FILES",value,subavl))
		updatersrc(subrsrc,"SUPPORTING FILES");
	changedsubords=TRUE;
}
void subedit(RDArsrc *subrsrc,RDArsrc *mtnrsrc)
{
	int selected=0;
	RDAaddsub *subs;
	char *modx=NULL;

	if(FINDRSCGETINT(subrsrc,"SUPPORTING FILES",&selected)) return;
	subs=tempsubord+selected;
	if(subs!=NULL)
	{
		subordscreen(subrsrc,++selected,subs->module,subs->filename,subs->subkeyname,subs->dtype,subs->num,subs->expression,subs->browse,subs->button_name,subs->browse_screen,subs->define_list,subs->search_browse,subs->smart_browse,subs->confld,savemsubo,quitmsubotest);
	} else {
		readwidget(mtnrsrc,"MODULE");
		FINDRSCGETSTRING(mtnrsrc,"MODULE",&modx);
		subordscreen(subrsrc,++selected,modx,NULL,NULL,0,0,NULL,
			0,NULL,NULL,NULL,NULL,FALSE,
			NULL,savemsubo,quitmsubotest);
		if(modx!=NULL) Rfree(modx);
	}
}
void savesub(void)
{
	int x=0,z=0;
	RDAaddsub *temp1,*temp;
	RDAconfld *confld1,*confld2;

	if(POWERADD->subord!=NULL)
	{
		for(x=0,temp=POWERADD->subord;x<POWERADD->subnum;++x,++temp)
		{
			if(temp->module!=NULL) Rfree(temp->module);
			if(temp->filename!=NULL) Rfree(temp->filename);
			if(temp->subkeyname!=NULL) Rfree(temp->subkeyname);
			if(temp->browse_screen!=NULL) Rfree(temp->browse_screen);
			if(temp->button_name!=NULL) Rfree(temp->button_name);
			if(temp->define_list!=NULL) Rfree(temp->define_list);
			if(temp->search_browse!=NULL) Rfree(temp->search_browse);
			if(temp->confld!=NULL)
			{
				for(z=0,confld1=temp->confld;z<temp->num;++z,++confld1)
				{
					if(confld1->fldname!=NULL) Rfree(confld1->fldname);
					if(confld1->conname!=NULL) Rfree(confld1->conname);
				}
				Rfree(temp->confld);
			}
			if(temp->expression!=NULL) Rfree(temp->expression);
			temp->BL=NULL;
		}
		Rfree(POWERADD->subord);
		POWERADD->subord=NULL;
		POWERADD->subnum=0;
	}
	if(tempsubord!=NULL)
	{
		POWERADD->subnum=tmpnumsub;
		POWERADD->subord=Rmalloc(tmpnumsub*sizeof(RDAaddsub));
		for(x=0,temp=tempsubord,temp1=POWERADD->subord;x<tmpnumsub;++x,++temp,
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
			if(temp->subkeyname!=NULL)
			{
				temp1->subkeyname=stralloc(temp->subkeyname);
				Rfree(temp->subkeyname);
			} else temp1->subkeyname=NULL;
			temp1->dtype=temp->dtype;
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
			if(temp->expression!=NULL)
			{
				temp1->expression=stralloc(temp->expression);
				Rfree(temp->expression);
			} else temp1->expression=NULL;
			temp1->BL=NULL;
		}
		Rfree(tempsubord);
		tmpnumsub=0;
	}
}
void create_default_supporting(RDArsrc *mtnrsrc,short fileno,char *m,APPlib *filesavl)
{
	int x,y=0,z=0;
	short sfileno=(-1);
	NRDkey *key=NULL;
	NRDpart *part=NULL;
	NRDfield *field=NULL;
	RDAaddsub *temp1,*temp;
	RDAconfld *confld1,*confld2;
	char *tempx=NULL,*tempy=NULL,*tempb=NULL,*tempd=NULL,*temps=NULL;
	
	if(POWERADD->subord!=NULL)
	{
		for(x=0,temp=POWERADD->subord;x<POWERADD->subnum;++x,++temp)
		{
			if(temp->module!=NULL) Rfree(temp->module);
			if(temp->filename!=NULL) Rfree(temp->filename);
			if(temp->subkeyname!=NULL) Rfree(temp->subkeyname);
			if(temp->browse_screen!=NULL) Rfree(temp->browse_screen);
			if(temp->button_name!=NULL) Rfree(temp->button_name);
			if(temp->define_list!=NULL) Rfree(temp->define_list);
			if(temp->search_browse!=NULL) Rfree(temp->search_browse);
			if(temp->confld!=NULL)
			{
				for(z=0,confld1=temp->confld;z<temp->num;++z,++confld1)
				{
					if(confld1->fldname!=NULL) Rfree(confld1->fldname);
					if(confld1->conname!=NULL) Rfree(confld1->conname);
				}
				Rfree(temp->confld);
			}
			if(temp->expression!=NULL) Rfree(temp->expression);
			temp->BL=NULL;
		}
		Rfree(POWERADD->subord);
		POWERADD->subord=NULL;
		POWERADD->subnum=0;
	}
	if(tempsubord!=NULL)
	{
		for(x=0,temp=tempsubord;x<tmpnumsub;++x,++temp)
		{
			if(temp->module!=NULL) Rfree(temp->module);
			if(temp->filename!=NULL) Rfree(temp->filename);
			if(temp->subkeyname!=NULL) Rfree(temp->subkeyname);
			if(temp->browse_screen!=NULL) Rfree(temp->browse_screen);
			if(temp->button_name!=NULL) Rfree(temp->button_name);
			if(temp->define_list!=NULL) Rfree(temp->define_list);
			if(temp->search_browse!=NULL) Rfree(temp->search_browse);
			if(temp->confld!=NULL)
			{
				for(z=0,confld1=temp->confld;z<temp->num;++z,++confld1)
				{
					if(confld1->fldname!=NULL) Rfree(confld1->fldname);
					if(confld1->conname!=NULL) Rfree(confld1->conname);
				}
				Rfree(temp->confld);
			}
			if(temp->expression!=NULL) Rfree(temp->expression);
			temp->BL=NULL;
		}
		Rfree(tempsubord);
		tempsubord=NULL;
		tmpnumsub=0;
	}
	if(filesavl!=NULL)
	{
		for(x=0;x<filesavl->numlibs;++x)
		{
			if(RDAstrcmp(filesavl->libs[x],FILENAME(fileno)))
			{
				sfileno=OPNNRD(m,filesavl->libs[x]);
				if(sfileno!=(-1))
				{
					key=KEYPOINTER(sfileno);
					if(key!=NULL)
					{
						for(y=0,part=key->part;y<key->numparts;++y,++part)
						{
							field=FLDNRD(fileno,part->name);
							if(field==NULL) break;
							
						}
						if(y<key->numparts)
						{
						} else {
							tempx=Rmalloc(RDAstrlen(MODULENAME(sfileno))+RDAstrlen(FILENAME(sfileno))+41);
							sprintf(tempx,"([%s][%s][DELETEFLAG]=FALSE)",MODULENAME(sfileno),FILENAME(sfileno));
							tempy=Rmalloc(RDAstrlen(FILENAME(sfileno))+6);
							sprintf(tempy,"LOAD %s",FILENAME(sfileno));
							tempb=Rmalloc(101);
							sprintf(tempb,"SELECT %s BROWSE",FILENAME(sfileno));	
							tempd=Rmalloc(101);
							sprintf(tempd,"SELECT %s DEFINE LIST",FILENAME(sfileno));	
							temps=Rmalloc(101);
							sprintf(temps,"SELECT %s SEARCH BROWSE",FILENAME(sfileno));	
							z=addPowerAddSupportingADV(POWERADD,MODULENAME(sfileno),FILENAME(sfileno),0,0,NULL,key->name,tempx,1,tempy,tempb,tempd,temps,0);
							if(tempx!=NULL) Rfree(tempx);
							if(tempy!=NULL) Rfree(tempy);
							if(tempb!=NULL) Rfree(tempb);
							if(tempd!=NULL) Rfree(tempd);
							if(temps!=NULL) Rfree(temps);
							if(z!=(-1))
							{
								temp1=POWERADD->subord+(z-1);
								for(y=0,part=key->part;y<key->numparts;++y,++part)
								{
									tempx=Rmalloc(RDAstrlen(part->name)+16);
									sprintf(tempx,"[ADD %s]",part->name);
									if((y+1)<key->numparts)
									{
										addPowerAddSupportingConfld(temp1,part->name,tempx,TRUE);
									} else {
										addPowerAddSupportingConfld(temp1,part->name,tempx,0);
									}
									if(tempx!=NULL) Rfree(tempx);
								}
							}
						}
					}
				}
			}
		}
	}
	if(POWERADD->subord!=NULL)
	{
		tmpnumsub=POWERADD->subnum;
		tempsubord=Rmalloc(tmpnumsub*sizeof(RDAaddsub));
		for(x=0,temp1=tempsubord,temp=POWERADD->subord;x<tmpnumsub;++x,++temp,
			++temp1)
		{
			if(temp->module!=NULL)
			{
				temp1->module=stralloc(temp->module);
			} else temp1->module=NULL;
			if(temp->filename!=NULL)
			{
				temp1->filename=stralloc(temp->filename);
			} else temp1->filename=NULL;
			if(temp->subkeyname!=NULL)
			{
				temp1->subkeyname=stralloc(temp->subkeyname);
			} else temp1->subkeyname=NULL;
			temp1->dtype=temp->dtype;
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
					} else confld2->fldname=NULL;
					if(confld1->conname!=NULL)
					{
						confld2->conname=stralloc(confld1->conname);
					} else confld2->conname=NULL;
					confld2->inherit=confld1->inherit;
				}
			}
			if(temp->expression!=NULL)
			{
				temp1->expression=stralloc(temp->expression);
			} else temp1->expression=NULL;
			temp1->BL=NULL;
		}
	}
	if(POWERADD->subord!=NULL)
	{
		for(x=0,temp=POWERADD->subord;x<POWERADD->subnum;++x,++temp)
		{
			if(temp->browse==TRUE)
			{
				makeSearchBrowseScreen(POWERADD,temp);
				makeBrowseScreen(POWERADD,temp);
				makeBrowseDefineListScreen(POWERADD,temp);
			}
		}
	}
	getsubords();
	x=0;
	if(!FINDRSCLISTAPPlib(mtnrsrc,"SUPPORTING FILES",x,subavl))
		updatersrc(mtnrsrc,"SUPPORTING FILES");
	changedvalues=TRUE;
}
void subcopy(RDArsrc *subrsrc,RDArsrc *mtnrsrc)
{
	int selected=0;
	RDAaddsub *subs;
	char *modx=NULL;

	if(FINDRSCGETINT(subrsrc,"SUPPORTING FILES",&selected)) return;
	subs=tempsubord+selected;
	++selected;
	if(subs!=NULL)
	{
		subordscreen(subrsrc,++selected,subs->module,subs->filename,subs->subkeyname,subs->dtype,subs->num,subs->expression,subs->browse,subs->button_name,subs->browse_screen,subs->define_list,subs->search_browse,subs->smart_browse,subs->confld,savemsubobelow,quitmsubobelowtest);
	} else {
		readwidget(mtnrsrc,"MODULE");
		FINDRSCGETSTRING(mtnrsrc,"MODULE",&modx);
		subordscreen(subrsrc,++selected,modx,NULL,NULL,0,0,NULL,
			0,NULL,NULL,NULL,NULL,FALSE,
			NULL,savemsubobelow,quitmsubobelowtest);
		if(modx!=NULL) Rfree(modx);
	}
}
