/* dfchgsup - Define Search Files section of Define Changes */
#include <mix.hpp>
#include <dfchg.hpp>
#include <ldval.hpp>

extern APPlib *makefieldvallist3(RDAsearch *,int,short,char *,char *);
extern char *module,changedvalues;
static RDAdefault *msuppdefaults=NULL,*econdefaults=NULL;
static char changesupports=FALSE;
static RDAconfld *holdconfld=NULL;
extern RDAsearch *holdsupfils;
extern int numsupfils;
extern APPlib *modulelist,*supfilsavl,*filelstx,*cmodlst,*subkeylist,*econflds;
extern APPlib *modlstx,*filelst;
APPlib *cnflds=NULL;
APPlib *ctypes;
extern char *contype[];
static void setconfldlist(RDArsrc *,APPlib *,char *);
static int holdnum=0;
static int ptrue=TRUE,pfalse=FALSE;
static void loadvalue(RDArsrc *,char *);
void supportingedit(RDArsrc *),supportingdelete(RDArsrc *);
void supportingabove(RDArsrc *),supportingbelow(RDArsrc *);
void getsupports(void);
static void changectype(RDArsrc *);
static void dofilelist(RDArsrc *,int *);
static void domodulelist(RDArsrc *,int *);

static void setconfldlist(RDArsrc *rsrc,APPlib *list,char *fieldvalue)
{
	int x=(-1);
	char *warndesc=NULL;

	if(!isEMPTY(fieldvalue))
	{
		if((x=FINDAPPLIBELEMENT(list,fieldvalue))==(-1))
		{
			warndesc=Rmalloc(RDAstrlen(fieldvalue)+250);
			sprintf(warndesc,"The value of [%s] is an Invalid Virtual/File Field Value.  It doesn't exist in this Change Definitions resources.",(fieldvalue!=NULL ? fieldvalue:""));
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID CONNECT VALUE",warndesc,NULL,NULL,FALSE,NULL);
			Rfree(warndesc);
			x=0;
		}
		FINDRSCSETINT(rsrc,"CONNECT FIELDS",x);
		updatersrc(rsrc,"CONNECT FIELDS");
	} 
}
static void loadvalue(RDArsrc *r,char *fieldname)
{
	int x,selected=(-1),y=0;
	int selectedm=0,selectedf=0;
	short z=TRUE;
	LoadValue *load;
	RDArsrc *ldvalrsrc1=NULL;
	char *modname=NULL,*filename=NULL;

	ldvalrsrc1=RDArsrcNEW("RPTGEN","LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc1,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	FINDRSCGETINT(r,"NUMBER",&selected);
	FINDRSCGETINT(r,"MODULE",&selectedm);
	FINDRSCGETINT(r,"FILENAME",&selectedf);
	if(RDAstrcmp(modlstx->libs[selectedm],"Contains No Database Libraries"))
	{
		modname=stralloc(modulelist->libs[selectedm]);
	}
	if(RDAstrcmp(filelstx->libs[selectedf],"Contains No File Definitions"))
	{
		filename=stralloc(filelstx->libs[selectedf]);
	}
	y=selected-1;
	loadlist=makefieldvallist3(holdsupfils,y,z,modname,filename);
	if(modname!=NULL) Rfree(modname);
	if(filename!=NULL) Rfree(filename);
	if(loadlist->numlibs<1)
	{
		addAPPlib(loadlist,"Load List Unavailable");
	}
	x=0;
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

	if(supfilsavl!=NULL) freeapplib(supfilsavl);
	supfilsavl=APPlibNEW();
	if(holdsupfils!=NULL)
	{
		for(x=0,s=holdsupfils;x<numsupfils;++x,++s)
		{
			len=RDAstrlen(s->module)+RDAstrlen(s->filename)+7+RDAstrlen(s->cmodule)+RDAstrlen(s->cfilename)+RDAstrlen(s->subkeyname)+RDAstrlen(contype[s->ctype]);
			if(tmp!=NULL) 
			{
				tmp=Rrealloc(tmp,len);
			} else {
				tmp=Rmalloc(len);
			}
			memset(tmp,0,len);
			sprintf(tmp,"%s %s %s %s %s %s",(s->module!=NULL ? s->module:""),(s->filename!=NULL ? s->filename:""),contype[s->ctype],(s->cmodule!=NULL ? s->cmodule:""),(s->cfilename!=NULL ? s->cfilename:""),(s->subkeyname!=NULL ? s->subkeyname:""));

			addAPPlib(supfilsavl,tmp);
		}
	}
	if(supfilsavl->numlibs<1)
	{
		addAPPlib(supfilsavl,"No Files Defined");
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
	if(modulelist!=NULL) freeapplib(modulelist);
	if(filelstx!=NULL) freeapplib(filelstx);
	if(cmodlst!=NULL) freeapplib(cmodlst);
	if(cnflds!=NULL) freeapplib(cnflds);
	if(ctypes!=NULL) freeapplib(ctypes);
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

	++numsupfils;
	new_fields=Rmalloc(numsupfils*sizeof(RDAsearch));
	y=0;
	for(x=0,temp=holdsupfils;x<(numsupfils-1);++x,++temp)
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
			temp1->range=FALSE;
			temp1->expression=NULL;
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
		temp1->range=temp->range;
		temp1->num_writes=temp->num_writes;
		++y;
	}
	Rfree(holdsupfils);
	holdsupfils=Rmalloc(numsupfils*sizeof(RDAsearch));
	for(x=0,temp1=holdsupfils,temp=new_fields;x<numsupfils;++x,
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
		temp1->range=temp->range;
		temp1->num_writes=temp->num_writes;
	}
	Rfree(new_fields);
}
static void savemsupp(RDArsrc *msupprsrc,RDArsrc *supprsrc)
{
	int value=0,cmod=0,selectedm=0,selectedf=0,selectsubkey=0;
	RDAsearch *s;
	int x;
	RDAconfld *cf1,*cf2;
	char rngval=FALSE;

	if(FINDRSCGETINT(supprsrc,"SUPPORTING FILES",&value)) return;
	if(holdsupfils==NULL)
	{
		holdsupfils=Rmalloc(sizeof(RDAsearch));
		numsupfils=1;
	}
	s=holdsupfils+value;
	readallwidgets(msupprsrc);
	FINDRSCGETINT(msupprsrc,"MODULE",&selectedm);
	FINDRSCGETINT(msupprsrc,"FILENAME",&selectedf);
	s->module=stralloc(modulelist->libs[selectedm]);
	s->filename=stralloc(filelstx->libs[selectedf]);
	FINDRSCGETINT(msupprsrc,"CONNECT TYPES",&s->ctype);
	FINDRSCGETINT(msupprsrc,"CONNECT FILENAME",&cmod);
        if(!isEMPTY(cmodlst->libs[cmod]))
        {
                s->cmodule=stripmodulename(cmodlst->libs[cmod]);
                s->cfilename=stripfilename(cmodlst->libs[cmod]);
        } else {
                s->cmodule=NULL;
                s->cfilename=NULL;
        }
	FINDRSCGETSTRING(msupprsrc,"EXPRESSION",&s->expression);
	FINDRSCGETINT(msupprsrc,"KEYNAME",&selectsubkey);
	FINDRSCGETCHAR(msupprsrc,"RANGE",&rngval);
	s->range=rngval;
	s->subkeyname=stralloc(subkeylist->libs[selectsubkey]);
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
	getsupports();
	if(!FINDRSCLISTAPPlib(supprsrc,"SUPPORTING FILES",value,supfilsavl))
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

	if(modulelist!=NULL) freeapplib(modulelist);
	modulelist=APPlibNEW();
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
		addAPPlib(modulelist,tmp);
	}
	if(modulelist->numlibs<1)
	{
		addAPPlib(modulelist,"Contains No Database Libraries");
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	SORTAPPlib(modulelist);
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
		sprintf(temp,"Field [%s] connects to %s",(cf1->fldname!=NULL ? cf1->fldname:""),(cf1->conname!=NULL ? cf1->conname:""));
		addAPPlib(cnflds,temp);
	}
	if(cnflds->numlibs<1)
	{
		addAPPlib(cnflds,"No Connecting Fields");
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
	if(selectedm>=modulelist->numlibs) 
	{
		selectedm=0;
	}
	if(selectedf>=filelstx->numlibs)
	{
		selectedf=0;
	}
	if(!GETBIN(modulelist->libs[selectedm],filelstx->libs[selectedf],&engine,&nofields,&fields,&nokeys,&keys)) 
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
								if(!RDAstrcmp(cf->fldname,
									f->name))
								{
									cf->conname=Rmalloc(RDAstrlen(modulename)+RDAstrlen(filename)+RDAstrlen(f->name)+7);
									sprintf(cf->conname,"[%s][%s][%s]",modulename,filename,f->name);
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
			}
			if(filename!=NULL) Rfree(filename);
			if(modulename!=NULL) Rfree(modulename);
		}
	}
	getconflds();
	if(!FINDRSCLISTAPPlib(msupprsrc,"CONNECT FIELDNAME",0,cnflds))
		updatersrc(msupprsrc,"CONNECT FIELDNAME");
}
static void quitecon(RDArsrc *econrsrc,RDArsrc *msupprsrc)
{
	if(econrsrc!=NULL)
	{
		killwindow(econrsrc);
		free_rsrc(econrsrc);
	}
	if(econflds!=NULL) freeapplib(econflds);
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
				c->conname=stralloc(econflds->libs[selected]);
				getconflds();
				if(!FINDRSCLISTAPPlib(msupprsrc,"CONNECT FIELDNAME",
					selhold,cnflds))
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
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGE CHANGES NOT SAVED!","Changes have been made to this CONNECT FIELD!\nDo you want to Save these changes?",saveecon,quitecon,FALSE,2,econrsrc,msupprsrc,NULL);
	} else {
		quitecon(econrsrc,msupprsrc);
	}
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
/*
						temp=Rrealloc(temp,RDAstrlen(f->name)+
							RDAstrlen(filex)+7);
*/
						temp=Rrealloc(temp,RDAstrlen(f->name)+
							RDAstrlen(modulex)+
							RDAstrlen(filex)+7);
					} else {
/*
						temp=Rmalloc(RDAstrlen(f->name)+
							RDAstrlen(filex)+7);
*/
						temp=Rmalloc(RDAstrlen(f->name)+RDAstrlen(modulex)+
							RDAstrlen(filex)+7);
					}
/*
					sprintf(temp,"[%s][%s]",
						filex,(f->name==NULL?"":f->name));
*/
					sprintf(temp,"[%s][%s][%s]",modulex,
						filex,(f->name==NULL?"":f->name));
					addAPPlib(tmp,temp);
					if(f->name!=NULL) Rfree(f->name);
				}
				Rfree(fields);
			}
		}
	}
	if(temp!=NULL) Rfree(temp);
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
	readwidget(supprsrc,"FILENAME");
	FINDRSCGETINT(supprsrc,"MODULE",&x);
	curmod=stralloc(modlstx->libs[x]);	
	FINDRSCGETINT(supprsrc,"FILENAME",&x);
	curfile=stralloc(filelst->libs[x]);
	if(!isEMPTY(curfile) && !isEMPTY(curmod))
	{
#ifdef ADDING_VIRTUALS
		addDFincvir(tmprsrc,curmod,curfile,NULL,dumb);
#endif /* Adding Virtuals */
		addholdvalues(tmp,curmod,curfile);
	}
	if(curfile!=NULL) Rfree(curfile);
	for(x=0,s=holdsupfils;x<numsupfils;++x,++s)
	{
#ifdef ADDING_VIRTUALS
		addDFincvir(tmprsrc,s->module,s->filename,NULL,dumb);
#endif /* Adding Virtuals */
		addholdvalues(tmp,s->module,s->filename);
	}
#ifdef ADDING_VIRTUALS
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(curmod)+17);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,(curmod!=NULL ? curmod:""));
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,(curmod!=NULL ? curmod:""));
#endif
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
	char *fldname=NULL,*cfldname=NULL;
	RDAconfld *c;
	RDArsrc *econrsrc=NULL;

	readallwidgets(msupprsrc);
	if(FINDRSCGETINT(msupprsrc,"CONNECT FIELDNAME",&selectfld)) return;
	if(holdconfld!=NULL && selectfld<holdnum)
	{
		c=holdconfld+selectfld;
		fldname=stralloc(c->fldname);
		cfldname=stralloc(c->conname);
	} 
	econrsrc=RDArsrcNEW(module,"EDIT CHANGE FILE CONNECT FIELD");
	addstdrsrc(econrsrc,"FIELD NAME",VARIABLETEXT,0,
		(fldname!=NULL ? fldname:""),FALSE);
	if(fldname!=NULL) Rfree(fldname);
	if(econflds!=NULL) freeapplib(econflds);
	econflds=APPlibNEW();
	x=0;
	econflds=makefieldvallist(supprsrc);
	if(econflds->numlibs<1)
	{
		addAPPlib(econflds,"No Connecting Fields Available");
	}
	addlstrsrc(econrsrc,"CONNECT FIELDS",&x,TRUE,NULL,econflds->numlibs,
		&econflds->libs,NULL);
	addrfcbrsrc(econrsrc,"SAVE",TRUE,saveecon,msupprsrc);
	addrfcbrsrc(econrsrc,"QUIT",TRUE,quitecontest,msupprsrc);
	addbtnrsrc(econrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(econrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(econrsrc,TRUE,quitecon,msupprsrc,FALSE);
	setconfldlist(econrsrc,econflds,cfldname);
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
	if(subkeylist!=NULL) freeapplib(subkeylist);
	subkeylist=APPlibNEW();
	if(selectedm>=modulelist->numlibs) 
	{
		selectedm=0;
	}
	if(selectedf>=filelstx->numlibs)
	{
		selectedf=0;
	}
	if(!GETBIN(modulelist->libs[selectedm],filelstx->libs[selectedf],&engine,&nofields,&fields,&nokeys,&keys)) 
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
	if(!FINDRSCLISTAPPlib(msupprsrc,"KEYNAME",0,subkeylist))
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
	if(filelstx!=NULL) freeapplib(filelstx);
	if(selected>=modulelist->numlibs) 
	{
		selected=0;
	}
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modulelist->libs[selected])+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modulelist->libs[selected]);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modulelist->libs[selected]);
#endif
	filelstx=getlibnames(libx,TRUE);
	if(libx!=NULL) Rfree(libx);
	if(filelstx==NULL)
	{
		filelstx=APPlibNEW();
		addAPPlib(filelstx,"Contains No File Definitions");
	}
	if(selectedf>=filelstx->numlibs) selectedf=0;
	if(!FINDRSCLISTAPPlib(msupprsrc,"FILENAME",selectedf,filelstx))
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
void SupportScreen(RDArsrc *supprsrc,int num,
	char *modulex,char *filename,int ctype,char *cmodule,char *cfilename,
	char *subkeyname,int number,char *expression,RDAconfld *confld,
	char range_sup,void (*savefunc)(...),void (*quitfunctest)(...))
{
	int x=0,y=0,selmod=0,selfile=0;
	char *temp=NULL,*tmod=NULL,*curfile=NULL,*curmod=NULL;
	RDAsearch *s;
	RDAconfld *cf,*cf1;
	NRDfield *fields=NULL,*f;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short nofields=0,nokeys=0,engine=0;
	RDArsrc *msupprsrc=NULL;

	msupprsrc=RDArsrcNEW(module,"DEFINE CHANGE MAINTAIN SUPPORTING FILE");
	addstdrsrc(msupprsrc,"NUMBER",LONGV,8,&num,FALSE);
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
		FINDRSCGETINT(supprsrc,"MODULE",&x);
		tmod=stralloc(modlstx->libs[x]);
		for(x=0;x<modulelist->numlibs;++x)
		{
			if(!RDAstrcmp(modulelist->libs[x],tmod)) break;
		}
		if(tmod!=NULL) Rfree(tmod);
		if(x>=modulelist->numlibs) x=0;
	}
	addlstrsrc(msupprsrc,"MODULE",&x,TRUE,domodulelist,modulelist->numlibs,
		&modulelist->libs,&ptrue);
	if(filelstx!=NULL) freeapplib(filelstx);
	filelstx=APPlibNEW();
	addlstrsrc(msupprsrc,"FILENAME",&x,TRUE,dofilelist,filelstx->numlibs,
		&filelstx->libs,&ptrue);
        if(ctypes!=NULL) freeapplib(ctypes);
        ctypes=APPlibNEW();
	for(x=0;x<3;++x) addAPPlib(ctypes,contype[x]);
	x=0;
	addlstrsrc(msupprsrc,"CONNECT TYPES",&ctype,TRUE,changectype,ctypes->numlibs,
		&ctypes->libs,NULL);
	if(cmodlst!=NULL) freeapplib(cmodlst);
	cmodlst=APPlibNEW();
	readwidget(supprsrc,"FILENAME");
	FINDRSCGETINT(supprsrc,"FILENAME",&x);
	curfile=stralloc(filelst->libs[x]);
	readwidget(supprsrc,"MODULE");
	FINDRSCGETINT(supprsrc,"MODULE",&x);
	curmod=stralloc(modlstx->libs[x]);
	if(!isEMPTY(curfile) && !isEMPTY(curmod))
	{
		if(temp!=NULL)
		{
			temp=Rrealloc(temp,RDAstrlen(curmod)+RDAstrlen(curfile)+5);
		} else {
			temp=Rmalloc(RDAstrlen(curmod)+RDAstrlen(curfile)+5);
		}
		sprintf(temp,"[%s][%s]",curmod,curfile);
		addAPPlib(cmodlst,temp);
	}
	if(curmod!=NULL) Rfree(curmod);
	if(curfile!=NULL) Rfree(curfile);
	if(num>1)
	{
		for(x=0,s=holdsupfils;x<(num-1);++x,++s)
		{
			if(temp!=NULL)
			{
				temp=Rrealloc(temp,RDAstrlen(s->module)+RDAstrlen(s->filename)+5);
			} else {
				temp=Rmalloc(RDAstrlen(s->module)+RDAstrlen(s->filename)+5);
			}
			sprintf(temp,"[%s][%s]",(s->module!=NULL ? s->module:""),
				(s->filename!=NULL ? s->filename:""));
			addAPPlib(cmodlst,temp);
		}
	}
	if(temp!=NULL) Rfree(temp);
	if(cmodlst->numlibs<1)
	{
		addAPPlib(cmodlst,"No Files Available");
	}
	for(x=0,s=holdsupfils;x<(num-1);++x,++s)
	{
		if(!RDAstrcmp(s->module,cmodule) && !RDAstrcmp(s->filename,cfilename)) break;
	}
	if(x>=(num-1)) x=0;
	addlstrsrc(msupprsrc,"CONNECT FILENAME",&x,TRUE,setconflds,cmodlst->numlibs,&cmodlst->libs,NULL);
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
			sprintf(temp,"Field [%s] connects to %s",(cf->fldname!=NULL ? cf->fldname:""),(cf->conname!=NULL ? cf->conname:""));
			addAPPlib(cnflds,temp);
		}
	}
	subkeylist=APPlibNEW();
	x=0;
	addlstrsrc(msupprsrc,"KEYNAME",&x,TRUE,setconflds,subkeylist->numlibs,&subkeylist->libs,NULL);
	selmod=0;
	domodulelist(msupprsrc,&pfalse);
	if(modulex!=NULL)
	{
		for(selmod=0;selmod<modulelist->numlibs;++selmod)
		{
			if(!RDAstrcmp(modulelist->libs[selmod],modulex)) break;
		}
	}
	if(selmod>=modulelist->numlibs)
	{
		FINDRSCGETINT(supprsrc,"MODULE",&x);
		tmod=stralloc(modulelist->libs[x]);
		for(selmod=0;selmod<modulelist->numlibs;++selmod)
		{
			if(!RDAstrcmp(modulelist->libs[selmod],tmod)) break;
		}
		if(tmod!=NULL) Rfree(tmod);
		if(selmod>=modulelist->numlibs) selmod=0;
	}
	dofilelist(msupprsrc,&pfalse); 
	selfile=0;
	if(filename!=NULL)
	{
		for(selfile=0;selfile<filelstx->numlibs;++selfile)
		{
			if(!RDAstrcmp(filelstx->libs[selfile],filename)) break;
		}
	}
	if(selfile>=filelstx->numlibs)
	{
		selfile=0;
	}
	if(!FINDRSCLISTAPPlib(msupprsrc,"FILENAME",selfile,filelstx))
	{
		updatersrc(msupprsrc,"FILENAME");
	}
	if(!GETBIN(modulelist->libs[selmod],filelstx->libs[selfile],&engine,&nofields,&fields,&nokeys,&keys)) 
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
			if(!RDAstrcmp(subkeylist->libs[x],subkeyname)) break;
		}
	}
	if(x>=subkeylist->numlibs)
	{
		x=0;
	}
	if(!FINDRSCLISTAPPlib(msupprsrc,"KEYNAME",x,subkeylist))
	{
		updatersrc(msupprsrc,"KEYNAME");
	}
	if(cnflds->numlibs<1)
	{
		addAPPlib(cnflds,"No Connecting Fields");
		if(ctype!=2)
		{
			setconflds(msupprsrc);
		}
	}
	x=0;
	addlstrsrc(msupprsrc,"CONNECT FIELDNAME",&x,TRUE,NULL,cnflds->numlibs,
		&cnflds->libs,NULL);
	addbtnrsrc(msupprsrc,"SELECT",TRUE,editconnect,supprsrc);
	addstdrsrc(msupprsrc,"RANGE",BOOLNS,1,&range_sup,TRUE);
	addsctrsrc(msupprsrc,"EXPRESSION",0,expression,TRUE);
	addbtnrsrc(msupprsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
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
	addrfcbrsrc(msupprsrc,"SAVE",TRUE,savefunc,supprsrc);
	addrfcbrsrc(msupprsrc,"QUIT",TRUE,quitfunctest,supprsrc);
	addbtnrsrc(msupprsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(msupprsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
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
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGE CHANGES NOT SAVED!","Changes have been made to this SUPPORTING FILE DEFININTION!\nDo you want to Save these changes?",savemsupp,quitmsupp,FALSE,2,msupprsrc,supprsrc,NULL);
	} else {
		quitmsupp(msupprsrc,supprsrc);
	}
}
static void quitmsuppabovetest(RDArsrc *msupprsrc,RDArsrc *supprsrc)
{
	readallwidgets(msupprsrc);
	if(COMPARE_RSRCS(msupprsrc,msuppdefaults,2) || changesupports)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGE CHANGES NOT SAVED!","Changes have been made to this SUPPORTING FILE DEFININTION!\nDo you want to Save these changes?",savemsuppabove,quitmsupp,FALSE,2,msupprsrc,supprsrc,NULL);
	} else {
		quitmsupp(msupprsrc,supprsrc);
	}
}
static void quitmsuppbelowtest(RDArsrc *msupprsrc,RDArsrc *supprsrc)
{
	readallwidgets(msupprsrc);
	if(COMPARE_RSRCS(msupprsrc,msuppdefaults,2) || changesupports)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGE CHANGES NOT SAVED!","Changes have been made to this SUPPORTING FILE DEFININTION!\nDo you want to Save these changes?",savemsuppbelow,quitmsupp,FALSE,2,msupprsrc,supprsrc,NULL);
	} else {
		quitmsupp(msupprsrc,supprsrc);
	}
}
void supportingabove(RDArsrc *supprsrc)
{
	int selected=0,selectedm=0;

	if(FINDRSCGETINT(supprsrc,"SUPPORTING FILES",&selected)) return;
	if(selected==0 && !RDAstrcmp(supfilsavl->libs[selected],"No Files Defined"))
		return;
	readwidget(supprsrc,"MODULE");
	FINDRSCGETINT(supprsrc,"MODULE",&selectedm);
	SupportScreen(supprsrc,++selected,modlstx->libs[selectedm],NULL,0,NULL,NULL,NULL,0,NULL,NULL,FALSE,savemsuppabove,quitmsuppabovetest);
}
void supportingbelow(RDArsrc *supprsrc)
{
	int selected=0,selectedm=0;

	if(FINDRSCGETINT(supprsrc,"SUPPORTING FILES",&selected)) return;
	if(selected==0 && !RDAstrcmp(supfilsavl->libs[selected],"No Files Defined"))
		return;
	selected+=2;
	readwidget(supprsrc,"MODULE");
	FINDRSCGETINT(supprsrc,"MODULE",&selectedm);
	SupportScreen(supprsrc,selected,modlstx->libs[selectedm],NULL,0,NULL,NULL,NULL,0,NULL,NULL,FALSE,savemsuppbelow,quitmsuppbelowtest);
}
void supportingdelete(RDArsrc *supprsrc)
{
	short x,y,z;
	RDAsearch *temp,*temp1,*new_fields;
	int value;
	RDAconfld *confld1,*confld2;

	readallwidgets(supprsrc);
	if(FINDRSCGETINT(supprsrc,"SUPPORTING FILES",&value)) return;
	if(numsupfils<1) return;
	if(numsupfils>1)
	{
		new_fields=Rmalloc((numsupfils-1)*sizeof(RDAsearch));
		y=0;
		for(x=0,temp=holdsupfils;x<numsupfils;++x,++temp)
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
			}	
		}
		Rfree(holdsupfils);
		--numsupfils;
		holdsupfils=Rmalloc(numsupfils*sizeof(RDAsearch));
		for(x=0,temp1=holdsupfils,temp=new_fields;x<numsupfils;++x,
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
			temp1->range=temp->range;
			temp1->num_writes=temp->num_writes;
		}
		Rfree(new_fields);
	} else if(holdsupfils!=NULL)
	{
		for(x=0,temp=holdsupfils;x<numsupfils;++x,++temp)
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
		}
		Rfree(holdsupfils);
		numsupfils=0;
	}
	getsupports();
	if(value>=supfilsavl->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(supprsrc,"SUPPORTING FILES",value,supfilsavl))
		updatersrc(supprsrc,"SUPPORTING FILES");
	changesupports=TRUE;
}
void supportingedit(RDArsrc *supprsrc)
{
	int selected=0,selectedm=0;
	RDAsearch *s;

	if(FINDRSCGETINT(supprsrc,"SUPPORTING FILES",&selected)) return;
	s=holdsupfils+selected;
	if(s!=NULL)
	{
		SupportScreen(supprsrc,++selected,s->module,s->filename,s->ctype,s->cmodule,s->cfilename,s->subkeyname,s->num,s->expression,s->confld,s->range,savemsupp,quitmsupptest);
	} else {
		readwidget(supprsrc,"MODULE");
		FINDRSCGETINT(supprsrc,"MODULE",&selectedm);
		SupportScreen(supprsrc,++selected,modlstx->libs[selectedm],
			NULL,0,NULL,NULL,NULL,0,NULL,NULL,FALSE,savemsupp,
			quitmsupptest);
	}
}
