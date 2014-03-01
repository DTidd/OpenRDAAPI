/* dfrptsch - Define Search Files section of Define Reports */
#include <cstdio>





#include <mix.hpp>
#include <rptgen.hpp>
#include <ldval.hpp>

extern RDAreport *report;
extern RDArsrc *rptrsrc;
extern char *module,*rptmodule;
extern char *contype[];
extern char changedreport;
static RDAdefault *schdefaults=NULL,*msrchdefaults=NULL,*econdefaults=NULL;
static char changedsearches=FALSE;
static RDAconfld *holdconfld=NULL;
static APPlib *ctypes=NULL,*modlst=NULL,*schavl=NULL;
static RDAsearch *tempsearch=NULL;
static int tmpnum=0;
static APPlib *filelst=NULL,*cmodlst=NULL,*subkeylist=NULL,*cnflds=NULL;
static APPlib *econflds=NULL;
static int holdnum=0;
static int ptrue=TRUE,pfalse=FALSE;
extern void loadvalue(RDArsrc *,char *);

static void getsearchs()
{
	int x,len=0;
	RDAsearch *s;
	char *tmp=NULL;

	if(schavl!=NULL) freeapplib(schavl);
	schavl=APPlibNEW();
	if(tempsearch!=NULL)
	{
		for(x=0,s=tempsearch;x<tmpnum;++x,++s)
		{
			len=RDAstrlen(s->module)+RDAstrlen(s->filename)+RDAstrlen(s->cmodule)+RDAstrlen(s->cfilename)+RDAstrlen(s->subkeyname)+RDAstrlen(contype[s->ctype])+5+10;
			if(tmp!=NULL) 
			{
				tmp=Rrealloc(tmp,len);
			} else {
				tmp=Rmalloc(len);
			}
			memset(tmp,0,len);
			sprintf(tmp,"[%5d] %s %s %s %s %s %s",
				(x+1),
				(s->module!=NULL ? s->module:""),
				(s->filename!=NULL ? s->filename:""),
				contype[s->ctype],
				(s->cmodule!=NULL ? s->cmodule:""),
				(s->cfilename!=NULL ? s->cfilename:""),
				(s->subkeyname!=NULL ? s->subkeyname:""));
			addAPPlib(schavl,tmp);
		}
	}
	if(tmp!=NULL) Rfree(tmp);
	if(schavl->numlibs<1)
	{
		addAPPlib(schavl,"No Files Defined");
	}
}
static void quitmsrch(RDArsrc *msrchrsrc,RDArsrc *schrsrc)
{
	int x;
	RDAconfld *cf;

	if(msrchrsrc!=NULL)
	{
		killwindow(msrchrsrc);
		free_rsrc(msrchrsrc);
	}
	if(ctypes!=NULL) freeapplib(ctypes);
	if(modlst!=NULL) freeapplib(modlst);
	if(filelst!=NULL) freeapplib(filelst);
	if(cmodlst!=NULL) freeapplib(cmodlst);
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
	if(msrchdefaults!=NULL) FreeRDAdefault(msrchdefaults);
}
static void makespacesearch(int pos)
{
	short x,y,z;
	RDAsearch *temp,*temp1,*new_fields;
	RDAconfld *confld1,*confld2;

	++tmpnum;
	new_fields=Rmalloc(tmpnum*sizeof(RDAsearch));
	y=0;
	for(x=0,temp=tempsearch;x<(tmpnum-1);++x,++temp)
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
			temp1->skip_detail=NULL;
			temp1->num_writes=1;
			temp1->wc_eval=FALSE;
			temp1->wc_expression=NULL;
			temp1->wc_skip=FALSE;
			temp1->skip_detail_error=NULL;
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
		if(temp->skip_detail!=NULL)
		{
			temp1->skip_detail=stralloc(temp->skip_detail);
			Rfree(temp->skip_detail);
		} else temp1->skip_detail=NULL;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		temp1->range=temp->range;
		temp1->num_writes=temp->num_writes;
		temp1->wc_eval=temp->wc_eval;
		if(temp->wc_expression!=NULL)
		{
			temp1->wc_expression=stralloc(temp->wc_expression);
			Rfree(temp->wc_expression);
		} else temp1->wc_expression=NULL;
		temp1->wc_skip=temp->wc_skip;
		if(temp->skip_detail_error!=NULL)
		{
			temp1->skip_detail_error=stralloc(temp->skip_detail_error);
			Rfree(temp->skip_detail_error);
		} else temp1->skip_detail_error=NULL;
		++y;
	}
	Rfree(tempsearch);
	tempsearch=Rmalloc(tmpnum*sizeof(RDAsearch));
	for(x=0,temp1=tempsearch,temp=new_fields;x<tmpnum;++x,
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
		if(temp->skip_detail!=NULL)
		{
			temp1->skip_detail=stralloc(temp->skip_detail);
			Rfree(temp->skip_detail);
		} else temp1->skip_detail=NULL;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		temp1->range=temp->range;
		temp1->num_writes=temp->num_writes;
		temp1->wc_eval=temp->wc_eval;
		if(temp->wc_expression!=NULL)
		{
			temp1->wc_expression=stralloc(temp->wc_expression);
			Rfree(temp->wc_expression);
		} else temp1->wc_expression=NULL;
		temp1->wc_skip=temp->wc_skip;
		if(temp->skip_detail_error!=NULL)
		{
			temp1->skip_detail_error=stralloc(temp->skip_detail_error);
			Rfree(temp->skip_detail_error);
		} else temp1->skip_detail_error=NULL;
	}
	Rfree(new_fields);
}
static void savemsrch(RDArsrc *msrchrsrc,RDArsrc *schrsrc)
{
	int value=0,cmod=0,selectedm=0,selectedf=0,selectsubkey=0;
	RDAsearch *s;
	char range=FALSE;
	char wc_eval=FALSE,wc_skip=FALSE;
	int x;
	RDAconfld *cf1,*cf2;

	if(FINDRSCGETINT(schrsrc,"SEARCH FILES",&value)) return;
	if(tempsearch==NULL)
	{
		tempsearch=Rmalloc(sizeof(RDAsearch));
		tmpnum=1;
	}
	s=tempsearch+value;
	readallwidgets(msrchrsrc);
	FINDRSCGETINT(msrchrsrc,"MODULE",&selectedm);
	FINDRSCGETINT(msrchrsrc,"FILENAME",&selectedf);
	s->module=stralloc(modlst->libs[selectedm]);
	s->filename=stralloc(filelst->libs[selectedf]);
	FINDRSCGETINT(msrchrsrc,"CONNECT TYPES",&s->ctype);
	FINDRSCGETINT(msrchrsrc,"CONNECT FILENAME",&cmod);
	FINDRSCGETCHAR(msrchrsrc,"RANGE FILE",&range);
	s->range=(short)range;
	FINDRSCGETSHORT(msrchrsrc,"NUMBER WRITES",&s->num_writes);
	if(RDAstrcmp(cmodlst->libs[cmod],"               ") && s->ctype)
	{
		s->cmodule=stripmodulename(cmodlst->libs[cmod]);
		s->cfilename=stripfilename(cmodlst->libs[cmod]);
	} else {
		s->cmodule=NULL;
		s->cfilename=NULL;
	}
	FINDRSCGETSTRING(msrchrsrc,"EXPRESSION",&s->expression);
	FINDRSCGETSTRING(msrchrsrc,"SKIP DETAIL",&s->skip_detail);
	FINDRSCGETCHAR(msrchrsrc,"EVALUATE NUMBER WRITES",&wc_eval);
	s->wc_eval=(short)wc_eval;
	FINDRSCGETSTRING(msrchrsrc,"NUMBER WRITES EXPRESSION",&s->wc_expression);
	FINDRSCGETCHAR(msrchrsrc,"WRITE COUNT SKIP",&wc_skip);
	s->wc_skip=(short)wc_skip;
	FINDRSCGETSTRING(msrchrsrc,"SKIP DETAIL ERROR",&s->skip_detail_error);
	FINDRSCGETINT(msrchrsrc,"KEYNAME",&selectsubkey);
	s->subkeyname=stralloc(subkeylist->libs[selectsubkey]);
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
	getsearchs();
	if(!FINDRSCLISTAPPlib(schrsrc,"SEARCH FILES",value,schavl))
		updatersrc(schrsrc,"SEARCH FILES");
	if(COMPARE_RSRCS(msrchrsrc,msrchdefaults,2))
	{
		changedsearches=TRUE;
	}
	quitmsrch(msrchrsrc,schrsrc);
}
static void savemsrchabove(RDArsrc *msrchrsrc,RDArsrc *schrsrc)
{
	int x;

	if(FINDRSCGETINT(schrsrc,"SEARCH FILES",&x)) return;
	makespacesearch(x);
	savemsrch(msrchrsrc,schrsrc);
}
static void savemsrchbelow(RDArsrc *msrchrsrc,RDArsrc *schrsrc)
{
	int x;

	if(FINDRSCGETINT(schrsrc,"SEARCH FILES",&x)) return;
	makespacesearch(++x);
	FINDRSCSETINT(schrsrc,"SEARCH FILES",x);
	savemsrch(msrchrsrc,schrsrc);
}
static void getmodulelist()
{
	int x;
	char *tmp=NULL,*dirx=NULL;

	if(modlst!=NULL) freeapplib(modlst);
	modlst=APPlibNEW();
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
		addAPPlib(modlst,tmp);
	}
	if(modlst->numlibs<1)
	{
		addAPPlib(modlst,"Contains No Database Libraries");
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	SORTAPPlib(modlst);
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
			temp=Rrealloc(temp,RDAstrlen(cf1->fldname)+RDAstrlen(cf1->conname)+5+30);
		} else {
			temp=Rmalloc(RDAstrlen(cf1->fldname)+RDAstrlen(cf1->conname)+5+30);
		}
		sprintf(temp,"[%5d] Field [%s] connects to [%s]",(x+1),(cf1->fldname!=NULL ? cf1->fldname:""),(cf1->conname!=NULL ? cf1->conname:""));
		addAPPlib(cnflds,temp);
	}
	if(temp!=NULL) Rfree(temp);
	if(cnflds->numlibs<1)
	{
		addAPPlib(cnflds,"No Connecting Fields");
	}
}
static void setconflds(RDArsrc *msrchrsrc)
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
	if(FINDRSCGETINT(msrchrsrc,"MODULE",&selectedm)) return;
	if(FINDRSCGETINT(msrchrsrc,"FILENAME",&selectedf)) return;
	if(FINDRSCGETINT(msrchrsrc,"KEYNAME",&selectedkey)) return;
	if(selectedm>=modlst->numlibs) 
	{
		selectedm=0;
	}
	if(selectedf>=filelst->numlibs)
	{
		selectedf=0;
	}
	if(!GETBIN(modlst->libs[selectedm],filelst->libs[selectedf],&engine,&nofields,&fields,&nokeys,&keys)) 
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
			if(FINDRSCGETINT(msrchrsrc,"KEYNAME",&selectedkey)) return;
			if(FINDRSCGETINT(msrchrsrc,"CONNECT FILENAME",&selcf)) return;
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
			}
			if(filename!=NULL) Rfree(filename);
			if(modulename!=NULL) Rfree(modulename);
		}
	}
	getconflds();
	if(!FINDRSCLISTAPPlib(msrchrsrc,"CONNECT FIELDNAME",0,cnflds))
		updatersrc(msrchrsrc,"CONNECT FIELDNAME");
}
static void setcontypes(RDArsrc *msrchrsrc)
{
	int x=0;
	
	FINDRSCGETINT(msrchrsrc,"CONNECT TYPES",&x);
	if(!RDAstrcmp(ctypes->libs[x],"Detail") || !RDAstrcmp(ctypes->libs[x],"Detail Break"))
	{
		FINDRSCSETINT(msrchrsrc,"CONNECT FILENAME",(cmodlst->numlibs-1));
		updatersrc(msrchrsrc,"CONNECT FILENAME");
	}
	setconflds(msrchrsrc);
}
static void quitecon(RDArsrc *econrsrc,RDArsrc *msrchrsrc)
{
	if(econrsrc!=NULL)
	{
		killwindow(econrsrc);
		free_rsrc(econrsrc);
	}
	if(econflds!=NULL) freeapplib(econflds);
	if(econdefaults!=NULL) FreeRDAdefault(econdefaults);
}
static void saveecon(RDArsrc *econrsrc,RDArsrc *msrchrsrc)
{
	int selected=0,selhold=0;
	char *fldname=NULL;
	RDAconfld *c;

	if(FINDRSCGETINT(msrchrsrc,"CONNECT FIELDNAME",&selhold)) return;
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
				if(!FINDRSCLISTAPPlib(msrchrsrc,"CONNECT FIELDNAME",
					selhold,cnflds))
				{
					updatersrc(msrchrsrc,"CONNECT FIELDNAME");
				}
			}
		}
		Rfree(fldname);
	}
	if(COMPARE_RSRCS(econrsrc,econdefaults,2))
	{
		changedsearches=TRUE;
	}
	quitecon(econrsrc,msrchrsrc);
}
static void quitecontest(RDArsrc *econrsrc,RDArsrc *msrchrsrc)
{
	readallwidgets(econrsrc);
	if(COMPARE_RSRCS(econrsrc,econdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","REPORT CHANGES NOT SAVED!","Changes have been made to this CONNECT FIELD!\nDo you want to Save these changes?",saveecon,quitecon,FALSE,2,econrsrc,msrchrsrc,NULL);
	} else {
		quitecon(econrsrc,msrchrsrc);
	}
}
static APPlib *makeconnectlist(RDArsrc *msrchrsrc) 
{
	APPlib *tmp=NULL;
	int x,y,z,cmod=0;
	RDAvirtual *v;
	NRDfield *fields=NULL,*f;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short q,nofields=0,nokeys=0,engine=0;
	char *mtmp=NULL,*temp=NULL,*modx=NULL,*filex=NULL;

	tmp=APPlibNEW();
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
			addAPPlib(tmp,temp);
		}
	}
	FINDRSCGETINT(msrchrsrc,"CONNECT FILENAME",&cmod);
	mtmp=stralloc(cmodlst->libs[cmod]);
	unpad(mtmp);
	if(isEMPTY(mtmp)) q=FALSE;
		else q=TRUE;
	if(mtmp!=NULL) Rfree(mtmp);
	if(q)
	{
		filex=stripfilename(cmodlst->libs[cmod]);
		if(filex!=NULL) modx=stripmodulename(cmodlst->libs[cmod]);
		if(!GETBIN(modx,filex,&engine,&nofields,&fields,
			&nokeys,&keys)) 
		{
			if(fields!=NULL)
			{
				for(y=0,f=fields;y<nofields;++y,++f)
				{
					if(temp!=NULL)
					{
						temp=Rrealloc(temp,
							RDAstrlen(f->name)+
							RDAstrlen(filex)+5);
					} else {
						temp=Rmalloc(RDAstrlen(f->name)
							+RDAstrlen(filex)+5);
					}
					sprintf(temp,"[%s][%s]",filex,
						(f->name!=NULL ? f->name:""));
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
							if(part->name!=NULL) 
							{
								Rfree(part->name);
							}
						}
						Rfree(keyx->part);
					}
				}
				Rfree(keys);
			}
		}
		if(modx!=NULL) Rfree(modx);
		if(filex!=NULL) Rfree(filex);
	}
	if(temp!=NULL) Rfree(temp);
	SORTAPPlib(tmp);
	return(tmp);
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
		sprintf(warndesc,"The value of [%s] is an Invalid Virtual/File Field Value.  It doesn't exist in this report's Virtual Fields or the connected files.  The List for this field is being set to the value [%s].",(fieldvalue==NULL?"":fieldvalue),list->libs[0
]);
		WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID FIELD VALUE!",warndesc,NULL,NULL,FALSE,NULL);
		Rfree(warndesc);
		x=0;
	}
	FINDRSCSETINT(rsrc,"CONNECT FIELDS",x);
	updatersrc(rsrc,"CONNECT FIELDS");
	}
}
static void editconnect(RDArsrc *msrchrsrc)
{
	int x,selectfld=0;
	char *fldname=NULL,*cfldname=NULL;
	RDAconfld *c;
	RDArsrc *econrsrc=NULL;

	readallwidgets(msrchrsrc);
	if(FINDRSCGETINT(msrchrsrc,"CONNECT FIELDNAME",&selectfld)) return;
	if(holdconfld!=NULL && selectfld<holdnum)
	{
		c=holdconfld+selectfld;
		fldname=stralloc(c->fldname);
		cfldname=stralloc(c->conname);
	} 
	econrsrc=RDArsrcNEW(module,"EDIT CONNECT FIELD");
	addstdrsrc(econrsrc,"FIELD NAME",VARIABLETEXT,0,
		(fldname!=NULL ? fldname:""),FALSE);
	if(fldname!=NULL) Rfree(fldname);
	if(econflds!=NULL) freeapplib(econflds);
	x=0;
	econflds=makeconnectlist(msrchrsrc);
	if(econflds->numlibs<1)
	{
		addAPPlib(econflds,"No Connecting Fields Available");
	}
	addlstrsrc(econrsrc,"CONNECT FIELDS",&x,TRUE,NULL,econflds->numlibs,
		&econflds->libs,NULL);
	addrfcbrsrc(econrsrc,"SAVE",TRUE,saveecon,msrchrsrc);
	addrfcbrsrc(econrsrc,"QUIT",TRUE,quitecontest,msrchrsrc);
	addbtnrsrc(econrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(econrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(econrsrc,TRUE,quitecon,msrchrsrc,FALSE);
	setconfldlist(econrsrc,econflds,cfldname);
	if(cfldname!=NULL) Rfree(cfldname);
	econdefaults=GetDefaults(econrsrc);
}
static void dofilelist(RDArsrc *msrchrsrc,int *passfunc)
{
	int x,y,z,selectedm,selectedf;
	NRDfield *fields=NULL,*f;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short nofields=0,nokeys=0,engine=0;

	if(FINDRSCGETINT(msrchrsrc,"MODULE",&selectedm)) return;
	if(FINDRSCGETINT(msrchrsrc,"FILENAME",&selectedf)) return;
	if(subkeylist!=NULL) freeapplib(subkeylist);
	subkeylist=APPlibNEW();
	if(selectedm>=modlst->numlibs) 
	{
		selectedm=0;
	}
	if(selectedf>=filelst->numlibs)
	{
		selectedf=0;
	}
	if(!GETBIN(modlst->libs[selectedm],filelst->libs[selectedf],&engine,&nofields,&fields,&nokeys,&keys)) 
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
	if(!FINDRSCLISTAPPlib(msrchrsrc,"KEYNAME",x,subkeylist))
	{
		updatersrc(msrchrsrc,"KEYNAME");
		if(*passfunc) setconflds(msrchrsrc);
	}
}
static void domodulelist(RDArsrc *msrchrsrc,int *passfunc)
{
	int selected=0,selectedf=0;
	char *libx=NULL;

	readwidget(msrchrsrc,"MODULE");
	readwidget(msrchrsrc,"FILENAME");
	if(FINDRSCGETINT(msrchrsrc,"MODULE",&selected)) return;
	if(FINDRSCGETINT(msrchrsrc,"FILENAME",&selectedf)) return;
	if(filelst!=NULL) freeapplib(filelst);
	if(selected>=modlst->numlibs) 
	{
		selected=0;
	}
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlst->libs[selected])+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modlst->libs[selected]);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modlst->libs[selected]);
#endif
	filelst=getlibnames(libx,TRUE);
	if(libx!=NULL) Rfree(libx);
	if(filelst==NULL)
	{
		filelst=APPlibNEW();
		addAPPlib(filelst,"Contains No File Definitions");
	}
	if(selectedf>=filelst->numlibs) selectedf=0;
	if(!FINDRSCLISTAPPlib(msrchrsrc,"FILENAME",selectedf,filelst))
	{
		updatersrc(msrchrsrc,"FILENAME");
		dofilelist(msrchrsrc,passfunc);
	}
}
static void set_wceval(RDArsrc *msrchrsrc)
{
	char wc_eval=FALSE;

	readwidget(msrchrsrc,"EVALUATE NUMBER WRITES");
	FINDRSCGETCHAR(msrchrsrc,"EVALUATE NUMBER WRITES",&wc_eval);
	if(wc_eval==TRUE)
	{
		FINDRSCSETSENSITIVE(msrchrsrc,"NUMBER WRITES",FALSE);
		FINDRSCSETEDITABLE(msrchrsrc,"NUMBER WRITES",FALSE);
		FINDRSCSETSENSITIVE(msrchrsrc,"NUMBER WRITES EXPRESSION",TRUE);
		FINDRSCSETEDITABLE(msrchrsrc,"NUMBER WRITES EXPRESSION",TRUE);
		FINDRSCSETSENSITIVE(msrchrsrc,"LOAD VALUE NUMBER WRITES",TRUE);
		FINDRSCSETEDITABLE(msrchrsrc,"LOAD VALUE NUMBER WRITES",TRUE);
		FINDRSCSETSENSITIVE(msrchrsrc,"LOAD OPERATOR NUMBER WRITES",TRUE);
		FINDRSCSETEDITABLE(msrchrsrc,"LOAD OPERATOR NUMBER WRITES",TRUE);
		FINDRSCSETSENSITIVE(msrchrsrc,"LOAD GROUPER NUMBER WRITES",TRUE);
		FINDRSCSETEDITABLE(msrchrsrc,"LOAD GROUPER NUMBER WRITES",TRUE);
		FINDRSCSETSENSITIVE(msrchrsrc,"LOAD CONTROL NUMBER WRITES",TRUE);
		FINDRSCSETEDITABLE(msrchrsrc,"LOAD CONTROL NUMBER WRITES",TRUE);
	} else {
		FINDRSCSETSENSITIVE(msrchrsrc,"NUMBER WRITES",TRUE);
		FINDRSCSETEDITABLE(msrchrsrc,"NUMBER WRITES",TRUE);
		FINDRSCSETSENSITIVE(msrchrsrc,"NUMBER WRITES EXPRESSION",FALSE);
		FINDRSCSETEDITABLE(msrchrsrc,"NUMBER WRITES EXPRESSION",FALSE);
		FINDRSCSETSENSITIVE(msrchrsrc,"LOAD VALUE NUMBER WRITES",FALSE);
		FINDRSCSETEDITABLE(msrchrsrc,"LOAD VALUE NUMBER WRITES",FALSE);
		FINDRSCSETSENSITIVE(msrchrsrc,"LOAD OPERATOR NUMBER WRITES",FALSE);
		FINDRSCSETEDITABLE(msrchrsrc,"LOAD OPERATOR NUMBER WRITES",FALSE);
		FINDRSCSETSENSITIVE(msrchrsrc,"LOAD GROUPER NUMBER WRITES",FALSE);
		FINDRSCSETEDITABLE(msrchrsrc,"LOAD GROUPER NUMBER WRITES",FALSE);
		FINDRSCSETSENSITIVE(msrchrsrc,"LOAD CONTROL NUMBER WRITES",FALSE);
		FINDRSCSETEDITABLE(msrchrsrc,"LOAD CONTROL NUMBER WRITES",FALSE);
	}
}
static void searchscreen(RDArsrc *schrsrc,int num,char *modulex,char *filename,
	int ctype,char *cmodule,char *cfilename,char *subkeyname,int number,
	RDAconfld *confld,short range,short num_writes,char *expression,
	char *skip_detail,short wc_eval,char *wc_expression,
	short wc_skip,char *skip_detail_error,
	void (*savefunc)(...),void (*quitfunctest)(...))
{
	int x=0,y=0,selmod=0,selfile=0;
	char *temp=NULL,*tmod=NULL,rangev=FALSE;
	char wceval=FALSE,wcskip=FALSE;
	RDAsearch *s;
	RDAconfld *cf,*cf1;
	NRDfield *fields=NULL,*f;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short nofields=0,nokeys=0,engine=0;
	RDArsrc *msrchrsrc=NULL;

	msrchrsrc=RDArsrcNEW(module,"MAINTAIN SEARCH FILE");
	addstdrsrc(msrchrsrc,"NUMBER",LONGV,8,&num,FALSE);
	if(range) rangev=TRUE;
	if(wc_eval) wceval=TRUE;
	if(wc_skip) wcskip=TRUE;
	getmodulelist();
	if(modulex!=NULL)
	{
		for(x=0;x<modlst->numlibs;++x)
		{
			if(!RDAstrcmp(modlst->libs[x],modulex)) break;
		}
	}
	if(x>=modlst->numlibs)
	{
		FINDRSCGETSTRING(rptrsrc,"REPORT MODULE",&tmod);
		for(x=0;x<modlst->numlibs;++x)
		{
			if(!RDAstrcmp(modlst->libs[x],tmod)) break;
		}
		if(tmod!=NULL) Rfree(tmod);
		if(x>=modlst->numlibs) x=0;
	}
	addlstrsrc(msrchrsrc,"MODULE",&x,TRUE,domodulelist,modlst->numlibs,
		&modlst->libs,&ptrue);
	if(filelst!=NULL) freeapplib(filelst);
	filelst=APPlibNEW();
	addlstrsrc(msrchrsrc,"FILENAME",&x,TRUE,dofilelist,filelst->numlibs,
		&filelst->libs,&ptrue);
	if(ctypes!=NULL) freeapplib(ctypes);
	ctypes=APPlibNEW();
	for(x=0;x<20;++x) addAPPlib(ctypes,contype[x]);
	x=0;
	addlstrsrc(msrchrsrc,"CONNECT TYPES",&ctype,TRUE,setcontypes,
		ctypes->numlibs,&ctypes->libs,NULL);
	addstdrsrc(msrchrsrc,"RANGE FILE",BOOLNS,1,&rangev,TRUE);
	addstdrsrc(msrchrsrc,"NUMBER WRITES",SHORTV,2,&num_writes,TRUE);
	if(cmodlst!=NULL) freeapplib(cmodlst);
	cmodlst=APPlibNEW();
/*
	if(num>1)
*/
	if(RDAstrcmp(ctypes->libs[ctype],"Detail") && RDAstrcmp(ctypes->libs[ctype],"Detail Break"))
	{
		for(x=0,s=tempsearch;x<(num-1);++x,++s)
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
	addAPPlib(cmodlst,"                     ");
/*
	} else {
		addAPPlib(cmodlst,"                     ");
	}
*/
	for(x=0,s=tempsearch;x<(num-1);++x,++s)
	{
		if(!RDAstrcmp(s->module,cmodule) && !RDAstrcmp(s->filename,cfilename)) break;
	}
	if(x>=(num-1)) x=0;
	addlstrsrc(msrchrsrc,"CONNECT FILENAME",&x,TRUE,setconflds,cmodlst->numlibs,
		&cmodlst->libs,NULL);
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
				temp=Rrealloc(temp,RDAstrlen(cf->fldname)+RDAstrlen(cf->conname)+5+30);
			} else {
				temp=Rmalloc(RDAstrlen(cf->fldname)+RDAstrlen(cf->conname)+5+30);
			}
			cf1->fldname=stralloc(cf->fldname);
			cf1->conname=stralloc(cf->conname);
			sprintf(temp,"[%5d] Field [%s] connects to [%s]",(x+1),(cf->fldname!=NULL ? cf->fldname:""),(cf->conname!=NULL ? cf->conname:""));
			addAPPlib(cnflds,temp);
		}
	}
	if(subkeylist!=NULL) freeapplib(subkeylist);
	subkeylist=APPlibNEW();
	x=0;
	addlstrsrc(msrchrsrc,"KEYNAME",&x,TRUE,setconflds,subkeylist->numlibs,
		&subkeylist->libs,NULL);
	selmod=0;
	domodulelist(msrchrsrc,&pfalse);
	if(modulex!=NULL)
	{
		for(selmod=0;selmod<modlst->numlibs;++selmod)
		{
			if(!RDAstrcmp(modlst->libs[selmod],modulex)) break;
		}
	}
	if(selmod>=modlst->numlibs)
	{
		FINDRSCGETSTRING(rptrsrc,"REPORT MODULE",&tmod);
		for(selmod=0;selmod<modlst->numlibs;++selmod)
		{
			if(!RDAstrcmp(modlst->libs[selmod],tmod)) break;
		}
		if(tmod!=NULL) Rfree(tmod);
		if(selmod>=modlst->numlibs) selmod=0;
	}
	dofilelist(msrchrsrc,&pfalse); 
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
	if(!FINDRSCLISTAPPlib(msrchrsrc,"FILENAME",selfile,filelst))
	{
		updatersrc(msrchrsrc,"FILENAME");
	}
	if(subkeylist!=NULL) freeapplib(subkeylist);
	subkeylist=APPlibNEW();
	if(!GETBIN(modlst->libs[selmod],filelst->libs[selfile],&engine,&nofields,&fields,&nokeys,&keys)) 
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
	if(!FINDRSCLISTAPPlib(msrchrsrc,"KEYNAME",x,subkeylist))
	{
		updatersrc(msrchrsrc,"KEYNAME");
	}
	if(cnflds->numlibs<1)
	{
		addAPPlib(cnflds,"No Connecting Fields");
		setconflds(msrchrsrc);
	}
	x=0;
	addlstrsrc(msrchrsrc,"CONNECT FIELDNAME",&x,TRUE,NULL,cnflds->numlibs,
		&cnflds->libs,NULL);
	addsctrsrc(msrchrsrc,"EXPRESSION",0,expression,TRUE);
	addbtnrsrc(msrchrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(msrchrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(msrchrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(msrchrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addsctrsrc(msrchrsrc,"SKIP DETAIL",0,skip_detail,TRUE);
	addbtnrsrc(msrchrsrc,"LOAD VALUE SKIP DETAIL",TRUE,loadvalue,"SKIP DETAIL");
	addbtnrsrc(msrchrsrc,"LOAD OPERATOR SKIP DETAIL",TRUE,loadoperand,"SKIP DETAIL");
	addbtnrsrc(msrchrsrc,"LOAD GROUPER SKIP DETAIL",TRUE,loadgrouper,"SKIP DETAIL");
	addbtnrsrc(msrchrsrc,"LOAD CONTROL SKIP DETAIL",TRUE,loadcontrol,"SKIP DETAIL");
	addstdrsrc(msrchrsrc,"EVALUATE NUMBER WRITES",BOOLNS,1,&wceval,TRUE);
	FINDRSCSETFUNC(msrchrsrc,"EVALUATE NUMBER WRITES",set_wceval,NULL);
	addsctrsrc(msrchrsrc,"NUMBER WRITES EXPRESSION",0,wc_expression,TRUE);
	addbtnrsrc(msrchrsrc,"LOAD VALUE NUMBER WRITES",TRUE,loadvalue,"NUMBER WRITES EXPRESSION");
	addbtnrsrc(msrchrsrc,"LOAD OPERATOR NUMBER WRITES",TRUE,loadoperand,"NUMBER WRITES EXPRESSION");
	addbtnrsrc(msrchrsrc,"LOAD GROUPER NUMBER WRITES",TRUE,loadgrouper,"NUMBER WRITES EXPRESSION");
	addbtnrsrc(msrchrsrc,"LOAD CONTROL NUMBER WRITES",TRUE,loadcontrol,"NUMBER WRITES EXPRESSION");
	addstdrsrc(msrchrsrc,"WRITE COUNT SKIP",BOOLNS,1,&wcskip,TRUE);
	addsctrsrc(msrchrsrc,"SKIP DETAIL ERROR",0,skip_detail_error,TRUE);
	addbtnrsrc(msrchrsrc,"LOAD VALUE SKIP DETAIL ERROR",TRUE,loadvalue,"SKIP DETAIL ERROR");
	addbtnrsrc(msrchrsrc,"LOAD OPERATOR SKIP DETAIL ERROR",TRUE,loadoperand,"SKIP DETAIL ERROR");
	addbtnrsrc(msrchrsrc,"LOAD GROUPER SKIP DETAIL ERROR",TRUE,loadgrouper,"SKIP DETAIL ERROR");
	addbtnrsrc(msrchrsrc,"LOAD CONTROL SKIP DETAIL ERROR",TRUE,loadcontrol,"SKIP DETAIL ERROR");
	addbtnrsrc(msrchrsrc,"SELECT",TRUE,editconnect,NULL);
	addrfcbrsrc(msrchrsrc,"SAVE",TRUE,savefunc,schrsrc);
	addrfcbrsrc(msrchrsrc,"QUIT",TRUE,quitfunctest,schrsrc);
	addbtnrsrc(msrchrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(msrchrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	if(temp!=NULL) Rfree(temp);
	set_wceval(msrchrsrc);
	msrchdefaults=GetDefaults(msrchrsrc);
	APPmakescrn(msrchrsrc,TRUE,quitmsrch,schrsrc,FALSE);
}
static void quitmsrchtest(RDArsrc *msrchrsrc,RDArsrc *schrsrc)
{
	readallwidgets(msrchrsrc);
	if(COMPARE_RSRCS(msrchrsrc,msrchdefaults,2) || changedsearches)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","REPORT CHANGES NOT SAVED!","Changes have been made to this SEARCH FILE DEFININTION!\nDo you want to Save these changes?",savemsrch,quitmsrch,FALSE,2,msrchrsrc,schrsrc,NULL);
	} else {
		quitmsrch(msrchrsrc,schrsrc);
	}
}
static void quitmsrchabovetest(RDArsrc *msrchrsrc,RDArsrc *schrsrc)
{
	readallwidgets(msrchrsrc);
	if(COMPARE_RSRCS(msrchrsrc,msrchdefaults,2) || changedsearches)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","REPORT CHANGES NOT SAVED!","Changes have been made to this SEARCH FILE DEFININTION!\nDo you want to Save these changes?",savemsrchabove,quitmsrch,FALSE,2,msrchrsrc,schrsrc,NULL);
	} else {
		quitmsrch(msrchrsrc,schrsrc);
	}
}
static void quitmsrchbelowtest(RDArsrc *msrchrsrc,RDArsrc *schrsrc)
{
	readallwidgets(msrchrsrc);
	if(COMPARE_RSRCS(msrchrsrc,msrchdefaults,2) || changedsearches)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","REPORT CHANGES NOT SAVED!","Changes have been made to this SEARCH FILE DEFININTION!\nDo you want to Save these changes?",savemsrchbelow,quitmsrch,FALSE,2,msrchrsrc,schrsrc,NULL);
	} else {
		quitmsrch(msrchrsrc,schrsrc);
	}
}
static void schaddabove(RDArsrc *schrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(schrsrc,"SEARCH FILES",&selected)) return;
	if(selected==0 && !RDAstrcmp(schavl->libs[selected],"No Files Defined"))
		return;
	searchscreen(schrsrc,++selected,rptmodule,NULL,1,NULL,NULL,NULL,0,
		NULL,0,1,NULL,NULL,0,NULL,0,NULL,
		savemsrchabove,quitmsrchabovetest);
}
static void schaddbelow(RDArsrc *schrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(schrsrc,"SEARCH FILES",&selected)) return;
	if(selected==0 && !RDAstrcmp(schavl->libs[selected],"No Files Defined"))
		return;
	selected+=2;
	searchscreen(schrsrc,selected,rptmodule,NULL,1,NULL,NULL,NULL,0,
		NULL,0,1,NULL,NULL,0,NULL,0,NULL,
		savemsrchbelow,quitmsrchbelowtest);
}
static void copysch(RDArsrc *schrsrc)
{
	int selected=0;
	RDAsearch *s=NULL;

	if(FINDRSCGETINT(schrsrc,"SEARCH FILES",&selected)) return;
	if(selected==0 && !RDAstrcmp(schavl->libs[selected],"No Files Defined"))
		return;
	if(tempsearch!=NULL && tmpnum>selected) s=tempsearch+selected;
	selected+=2;
	if(s!=NULL)
	{
		searchscreen(schrsrc,selected,s->module,s->filename,s->ctype,s->cmodule,s->cfilename,s->subkeyname,s->num,s->confld,s->range,s->num_writes,s->expression,s->skip_detail,s->wc_eval,s->wc_expression,s->wc_skip,s->skip_detail_error,savemsrchbelow,quitmsrchbelowtest);
	} else {
		searchscreen(schrsrc,selected,rptmodule,NULL,0,NULL,NULL,NULL,0,NULL,0,1,NULL,NULL,0,NULL,0,NULL,savemsrchbelow,quitmsrchbelowtest);
	}
}
static void schdelete(RDArsrc *schrsrc)
{
	short x,y,z;
	RDAsearch *temp,*temp1,*new_fields;
	int value;
	RDAconfld *confld1,*confld2;

	readallwidgets(schrsrc);
	if(FINDRSCGETINT(schrsrc,"SEARCH FILES",&value)) return;
	if(tmpnum<1) return;
	if(tmpnum>1)
	{
		new_fields=Rmalloc((tmpnum-1)*sizeof(RDAsearch));
		y=0;
		for(x=0,temp=tempsearch;x<tmpnum;++x,++temp)
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
				if(temp->skip_detail!=NULL)
				{
					temp1->skip_detail=stralloc(temp->skip_detail);
					Rfree(temp->skip_detail);
				} else temp1->skip_detail=NULL;
				if(temp->expression!=NULL)
				{
					temp1->expression=stralloc(temp->expression);
					Rfree(temp->expression);
				} else temp1->expression=NULL;
				temp1->range=temp->range;
				temp1->num_writes=temp->num_writes;
				temp1->wc_eval=temp->wc_eval;
				if(temp->wc_expression!=NULL)
				{
					temp1->wc_expression=stralloc(temp->wc_expression);
					Rfree(temp->wc_expression);
				} else temp1->wc_expression=NULL;
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
				if(temp->skip_detail!=NULL) Rfree(temp->skip_detail);
				if(temp->wc_expression!=NULL) Rfree(temp->wc_expression);
			}	
		}
		Rfree(tempsearch);
		--tmpnum;
		tempsearch=Rmalloc(tmpnum*sizeof(RDAsearch));
		for(x=0,temp1=tempsearch,temp=new_fields;x<tmpnum;++x,
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
			if(temp->skip_detail!=NULL)
			{
				temp1->skip_detail=stralloc(temp->skip_detail);
				Rfree(temp->skip_detail);
			} else temp1->skip_detail=NULL;
			if(temp->expression!=NULL)
			{
				temp1->expression=stralloc(temp->expression);
			} else temp1->expression=NULL;
			temp1->range=temp->range;
			temp1->num_writes=temp->num_writes;
			temp1->wc_eval=temp->wc_eval;
			if(temp->wc_expression!=NULL)
			{
				temp1->wc_expression=stralloc(temp->wc_expression);
				Rfree(temp->wc_expression);
			} else temp1->wc_expression=NULL;
		}
		Rfree(new_fields);
	} else if(tempsearch!=NULL)
	{
		for(x=0,temp=tempsearch;x<tmpnum;++x,++temp)
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
			if(temp->skip_detail!=NULL) Rfree(temp->skip_detail);
			if(temp->wc_expression!=NULL) Rfree(temp->wc_expression);
		}
		Rfree(tempsearch);
		tmpnum=0;
	}
	getsearchs();
	if(value>=schavl->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(schrsrc,"SEARCH FILES",value,schavl))
		updatersrc(schrsrc,"SEARCH FILES");
	changedsearches=TRUE;
}
static void schedit(RDArsrc *schrsrc)
{
	int selected=0;
	RDAsearch *s=NULL;

	if(FINDRSCGETINT(schrsrc,"SEARCH FILES",&selected)) return;
	if(tempsearch!=NULL && tmpnum>selected) s=tempsearch+selected;
	if(s!=NULL)
	{
		searchscreen(schrsrc,++selected,s->module,s->filename,s->ctype,s->cmodule,s->cfilename,s->subkeyname,s->num,s->confld,s->range,s->num_writes,s->expression,s->skip_detail,s->wc_eval,s->wc_expression,s->wc_skip,s->skip_detail_error,savemsrch,quitmsrchtest);
	} else {
		searchscreen(schrsrc,++selected,rptmodule,NULL,0,NULL,NULL,NULL,0,NULL,0,1,NULL,NULL,0,NULL,0,NULL,savemsrch,quitmsrchtest);
	}
}
static void quitsch(RDArsrc *schrsrc)
{
	RDAconfld *confld1;
	RDAsearch *temp;
	int x,z;

	if(schrsrc!=NULL)
	{
		killwindow(schrsrc);
		free_rsrc(schrsrc);
	}
	if(schavl!=NULL) freeapplib(schavl);
	if(tempsearch!=NULL)
	{
		for(x=0,temp=tempsearch;x<tmpnum;++x,++temp)
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
			if(temp->skip_detail!=NULL) Rfree(temp->skip_detail);
			if(temp->wc_expression!=NULL) Rfree(temp->wc_expression);
			if(temp->skip_detail_error!=NULL) Rfree(temp->skip_detail_error);
		}
		Rfree(tempsearch);
		tmpnum=0;
	}
	if(changedsearches)
	{
		changedreport=TRUE;
	}
	if(schdefaults!=NULL) FreeRDAdefault(schdefaults);
}
static void quitschclr(RDArsrc *schrsrc)
{
	changedsearches=FALSE;
	quitsch(schrsrc);
}
static void savesch(RDArsrc *schrsrc)
{
	int x=0,z=0;
	RDAsearch *temp1,*temp;
	RDAconfld *confld1,*confld2;

	readallwidgets(schrsrc);
	if(report->search!=NULL)
	{
		for(x=0,temp=report->search;x<report->numsearchs;++x,++temp)
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
			if(temp->skip_detail!=NULL) Rfree(temp->skip_detail);
			if(temp->wc_expression!=NULL) Rfree(temp->wc_expression);
			if(temp->skip_detail_error!=NULL) Rfree(temp->skip_detail_error);
		}
		Rfree(report->search);
		report->search=NULL;
		report->numsearchs=0;
	}
	if(tempsearch!=NULL)
	{
		report->numsearchs=tmpnum;
		report->search=Rmalloc(tmpnum*sizeof(RDAsearch));
		for(x=0,temp=tempsearch,temp1=report->search;x<tmpnum;++x,++temp,
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
				}
				Rfree(temp->confld);
			}
			if(temp->subkeyname!=NULL)
			{
				temp1->subkeyname=stralloc(temp->subkeyname);
				Rfree(temp->subkeyname);
			} else temp1->subkeyname=NULL;
			if(temp->skip_detail!=NULL)
			{
				temp1->skip_detail=stralloc(temp->skip_detail);
				Rfree(temp->skip_detail);
			} else temp1->skip_detail=NULL;
			if(temp->expression!=NULL)
			{
				temp1->expression=stralloc(temp->expression);
				Rfree(temp->expression);
			} else temp1->expression=NULL;
			temp1->range=temp->range;
			temp1->num_writes=temp->num_writes;
			temp1->wc_eval=temp->wc_eval;
			if(temp->wc_expression!=NULL)
			{
				temp1->wc_expression=stralloc(temp->wc_expression);
				Rfree(temp->wc_expression);
			} else temp1->wc_expression=NULL;
			temp1->wc_skip=temp->wc_skip;
			if(temp->skip_detail_error!=NULL)
			{
				temp1->skip_detail_error=stralloc(temp->skip_detail_error);
				Rfree(temp->skip_detail_error);
			} else temp1->skip_detail_error=NULL;
		}
		Rfree(tempsearch);
		tmpnum=0;
	}
	if(COMPARE_RSRCS(schrsrc,schdefaults,1))
	{
		changedsearches=TRUE;
	}
	quitsch(schrsrc);
}
static void quitschtest(RDArsrc *schrsrc)
{
	readallwidgets(schrsrc);
	if(COMPARE_RSRCS(schrsrc,schdefaults,0) || changedsearches)
	{
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","REPORT CHANGES NOT SAVED!","Changes have been made to the SEARCH FILE DEFININTIONS!\nDo you want to Save these changes?",savesch,quitschclr,FALSE,schrsrc);
	} else {
		quitsch(schrsrc);
	}
}
void RPTsearch(RDArsrc *rptrsrc)
{
	int x=0,z=0;
	RDAsearch *temp1,*temp;
	RDAconfld *confld1,*confld2;
	RDArsrc *schrsrc=NULL;

	if(report->search!=NULL)
	{
		tmpnum=report->numsearchs;
		tempsearch=Rmalloc(tmpnum*sizeof(RDAsearch));
		for(x=0,temp1=tempsearch,temp=report->search;x<tmpnum;++x,++temp1,
			++temp)
		{
			if(temp->module!=NULL)
			{
				temp1->module=stralloc(temp->module);
			} else temp1->module=NULL;
			if(temp->filename!=NULL)
			{
				temp1->filename=stralloc(temp->filename);
			} else temp1->filename=NULL;
			temp1->ctype=temp->ctype;
			if(temp->cmodule!=NULL)
			{
				temp1->cmodule=stralloc(temp->cmodule);
			} else temp1->cmodule=NULL;
			if(temp->cfilename!=NULL)
			{
				temp1->cfilename=stralloc(temp->cfilename);
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
					} else confld2->fldname=NULL;
					if(confld1->conname!=NULL)
					{
						confld2->conname=stralloc(confld1->conname);
					} else confld2->conname=NULL;
				}
			}
			if(temp->subkeyname!=NULL)
			{
				temp1->subkeyname=stralloc(temp->subkeyname);
			} else temp1->subkeyname=NULL;
			if(temp->skip_detail!=NULL)
			{
				temp1->skip_detail=stralloc(temp->skip_detail);
			} else temp1->skip_detail=NULL;
			if(temp->expression!=NULL)
			{
				temp1->expression=stralloc(temp->expression);
			} else temp1->expression=NULL;
			temp1->range=temp->range;
			temp1->num_writes=temp->num_writes;
			temp1->wc_eval=temp->wc_eval;
			if(temp->wc_expression!=NULL)
			{
				temp1->wc_expression=stralloc(temp->wc_expression);
			} else temp1->wc_expression=NULL;
			temp1->wc_skip=temp->wc_skip;
			if(temp->skip_detail_error!=NULL)
			{
				temp1->skip_detail_error=stralloc(temp->skip_detail_error);
			} else temp1->skip_detail_error=NULL;
		}
	} else {
		tempsearch=NULL;
		tmpnum=0;
	}
	schrsrc=RDArsrcNEW(module,"DEFINE SEARCH FILES");
	getsearchs();
	x=0;
	addlstrsrc(schrsrc,"SEARCH FILES",&x,TRUE,NULL,schavl->numlibs,
		&schavl->libs,NULL);
	addbtnrsrc(schrsrc,"ADD ABOVE",TRUE,schaddabove,NULL);
	addbtnrsrc(schrsrc,"ADD BELOW",TRUE,schaddbelow,NULL);
	addbtnrsrc(schrsrc,"DELETE",TRUE,schdelete,NULL);
	addbtnrsrc(schrsrc,"SELECT",TRUE,schedit,NULL);
	addbtnrsrc(schrsrc,"COPY",TRUE,copysch,NULL);
	addrfcbrsrc(schrsrc,"SAVE",TRUE,savesch,NULL);
	addrfcbrsrc(schrsrc,"QUIT",TRUE,quitschtest,NULL);
	addbtnrsrc(schrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(schrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	changedsearches=FALSE;
	schdefaults=GetDefaults(schrsrc);
	APPmakescrn(schrsrc,TRUE,quitsch,NULL,FALSE);
}
