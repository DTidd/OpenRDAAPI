/* dfaddwgt - Define Edit Widgets section of Define Power Add */
#include <cstdio>





#include <powadd.hpp>
#include <rptgen.hpp>
#include <ldval.hpp>



extern void create_default_writes(RDArsrc *,short);
extern void create_default_supporting(RDArsrc *,short,char *,APPlib *);
extern PowerAdd *POWERADD;
extern char *module,changedvalues;
static RDAeditfld *holdconfld=NULL;
static int holdnum=0;
static RDAdefault *msubsdefaults=NULL;
static RDAdefault *acctdefaults=NULL;
static RDAdefault *defdefaults=NULL;
static char changedacct=FALSE;
static char changeddef=FALSE;
static char changedEditWdgtss=FALSE;
extern AddEditWdgt *tempEditWdgts;
extern int tmpnumEditWdgts;
extern APPlib *wdgtavl;
extern APPlib *ExpTypes;
extern APPlib *fieldtypes;
static APPlib *ACCTrsc=NULL,*DEFrsc=NULL,*filesavl=NULL,*modsavl2=NULL;
static APPlib *wdgtstyle=NULL;
extern void loadvalue(RDArsrc *,char *);
extern APPlib *makefieldvallist(void);
void savewdgt(void),wdgtedit(RDArsrc *,RDArsrc *),wdgtdelete(RDArsrc *);
void SEEEditWdgts(PowerAdd *);
void wdgtaddabove(RDArsrc *,RDArsrc *),wdgtaddbelow(RDArsrc *,RDArsrc *);
void wdgtmovedown(RDArsrc *),wdgtmoveup(RDArsrc *),wdgtmove(RDArsrc *,short);
void wdgtcopy(RDArsrc *);
void getWidgets(void);
static APPlib *modlstx=NULL,*filelst=NULL,*fieldlst=NULL,*idfieldlst=NULL;
static APPlib *cnflds=NULL,*econflds=NULL,*btypes=NULL;
static int ptrue=TRUE,pfalse=FALSE;
/*
static APPlib *ConTypes=NULL;
*/
static APPlib *subkeylist=NULL;
static RDAdefault *econdefaults=NULL;
static APPlib *ezviravl=NULL,*mod_list=NULL;
static int virmodpointer=0;
static void makeezvirlisttest(RDArsrc *);
static void makeezvirlist(RDArsrc *);


void getWidgets()
{
	int x,len=0;
	AddEditWdgt *subs;
	char *tmp=NULL;

	if(wdgtavl!=NULL) freeapplib(wdgtavl);
	wdgtavl=APPlibNEW();
	if(tempEditWdgts!=NULL)
	{
		for(x=0,subs=tempEditWdgts;x<tmpnumEditWdgts;++x,++subs)
		{
			len=RDAstrlen(subs->name)+RDAstrlen(subs->expression)+12;
			if(tmp!=NULL) 
			{
				tmp=Rrealloc(tmp,len);
			} else {
				tmp=Rmalloc(len);
			}
			memset(tmp,0,len);
			if(RDAstrlen(subs->expression)<100)
			{
				sprintf(tmp,"%3d %s %s",(x+1),
					(subs->name!=NULL?subs->name:""),
					(subs->expression!=NULL?subs->expression:""));
			} else {
				sprintf(tmp,"%3d %s %.100s",(x+1),
					(subs->name!=NULL?subs->name:""),
					(subs->expression!=NULL?subs->expression:""));
			}
			addAPPlib(wdgtavl,tmp);
		}
	}
	if(wdgtavl->numlibs<1)
	{
		addAPPlib(wdgtavl,"No Edit Widgets Defined");
	}
}
static void makespaceEditWdgts(int pos)
{
	short x,y;
	int z;
	AddEditWdgt *temp,*temp1,*new_fields;
	RDAeditfld *confld1,*confld2;

	++tmpnumEditWdgts;
	new_fields=Rmalloc(tmpnumEditWdgts*sizeof(AddEditWdgt));
	y=0;
	for(x=0,temp=tempEditWdgts;x<(tmpnumEditWdgts-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->name=NULL;
			temp1->type=0;
			temp1->len=0;
			temp1->etype=0;
			temp1->expression=NULL;
			temp1->id_mgt=FALSE;
			temp1->module=NULL;
			temp1->filename=NULL;
			temp1->subkeyname=NULL;
			temp1->confld=NULL;
			temp1->mgt_expression=NULL;
			temp1->assignname=NULL;
			temp1->id=NULL;
			temp1->override_expression=NULL;
			temp1->fileno=(-1);
			temp1->setaccttype=FALSE;
			temp1->acctrsc=NULL;
			temp1->setdeftype=FALSE;
			temp1->defrsc=NULL;
			temp1->loadvirtual=FALSE;
			++y;
			temp1=new_fields+y;
		}
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->type=temp->type;
		temp1->len=temp->len;
		temp1->etype=temp->etype;
		temp1->id_mgt=temp->id_mgt;
		temp1->loadvirtual=temp->loadvirtual;
		temp1->setaccttype=temp->setaccttype;
		temp1->setdeftype=temp->setdeftype;
		if(temp->acctrsc!=NULL)
		{
			temp1->acctrsc=APPlibNEW();
			for(z=0;z<temp->acctrsc->numlibs;++z)
			{
				addAPPlib(temp1->acctrsc,temp->acctrsc->libs[z]);
			}
			freeapplib(temp->acctrsc);
		} else temp1->acctrsc=NULL;
		if(temp->defrsc!=NULL)
		{
			temp1->defrsc=APPlibNEW();
			for(z=0;z<temp->defrsc->numlibs;++z)
			{
				addAPPlib(temp1->defrsc,temp->defrsc->libs[z]);
			}
			freeapplib(temp->defrsc);
		} else temp1->defrsc=NULL;
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
		if(temp->confld!=NULL && temp->num>0)
		{
			temp1->confld=Rmalloc(temp->num*sizeof(RDAeditfld));
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
		temp1->num=temp->num;
		if(temp->mgt_expression!=NULL)
		{
			temp1->mgt_expression=stralloc(temp->mgt_expression);
			Rfree(temp->mgt_expression);
		} else temp1->mgt_expression=NULL;
		if(temp->assignname!=NULL)
		{
			temp1->assignname=stralloc(temp->assignname);
			Rfree(temp->assignname);
		} else temp1->assignname=NULL;
		if(temp->id!=NULL)
		{
			temp1->id=stralloc(temp->id);
			Rfree(temp->id);
		} else temp1->id=NULL;
		if(temp->override_expression!=NULL)
		{
			temp1->override_expression=stralloc(temp->override_expression);
			Rfree(temp->override_expression);
		} else temp1->override_expression=NULL;
		temp1->fileno=temp->fileno;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		temp1->loadvirtual=temp->loadvirtual;
		++y;
	}
	Rfree(tempEditWdgts);
	tempEditWdgts=Rmalloc(tmpnumEditWdgts*sizeof(AddEditWdgt));
	for(x=0,temp1=tempEditWdgts,temp=new_fields;x<tmpnumEditWdgts;++x,
		++temp,++temp1)
	{
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->type=temp->type;
		temp1->len=temp->len;
		temp1->loadvirtual=temp->loadvirtual;
		temp1->etype=temp->etype;
		temp1->id_mgt=temp->id_mgt;
		temp1->setaccttype=temp->setaccttype;
		temp1->setdeftype=temp->setdeftype;
		if(temp->acctrsc!=NULL)
		{
			temp1->acctrsc=APPlibNEW();
			for(z=0;z<temp->acctrsc->numlibs;++z)
			{
				addAPPlib(temp1->acctrsc,temp->acctrsc->libs[z]);
			}
			freeapplib(temp->acctrsc);
		} else temp1->acctrsc=NULL;
		if(temp->defrsc!=NULL)
		{
			temp1->defrsc=APPlibNEW();
			for(z=0;z<temp->defrsc->numlibs;++z)
			{
				addAPPlib(temp1->defrsc,temp->defrsc->libs[z]);
			}
			freeapplib(temp->defrsc);
		} else temp1->defrsc=NULL;
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
		if(temp->confld!=NULL && temp->num>0)
		{
			temp1->confld=Rmalloc(temp->num*sizeof(RDAeditfld));
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
			temp1->num=temp->num;
		} else {
			temp1->num=0;
		}
		if(temp->mgt_expression!=NULL)
		{
			temp1->mgt_expression=stralloc(temp->mgt_expression);
			Rfree(temp->mgt_expression);
		} else temp1->mgt_expression=NULL;
		if(temp->assignname!=NULL)
		{
			temp1->assignname=stralloc(temp->assignname);
			Rfree(temp->assignname);
		} else temp1->assignname=NULL;
		if(temp->id!=NULL)
		{
			temp1->id=stralloc(temp->id);
			Rfree(temp->id);
		} else temp1->id=NULL;
		if(temp->override_expression!=NULL)
		{
			temp1->override_expression=stralloc(temp->override_expression);
			Rfree(temp->override_expression);
		} else temp1->override_expression=NULL;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		temp1->loadvirtual=temp->loadvirtual;
	}
	Rfree(new_fields);
}
static void quitmsubo(RDArsrc *mrsrc,RDArsrc *subrsrc)
{
	int x; 
	RDAeditfld *cf;

	if(mrsrc!=NULL) 
	{
		killwindow(mrsrc);
		free_rsrc(mrsrc);
	}
	if(msubsdefaults!=NULL) FreeRDAdefault(msubsdefaults);
	if(modlstx!=NULL) freeapplib(modlstx);
	if(btypes!=NULL) freeapplib(btypes);
	if(filelst!=NULL) freeapplib(filelst);
	if(subkeylist!=NULL) freeapplib(subkeylist);
	if(fieldlst!=NULL) freeapplib(fieldlst);
	if(idfieldlst!=NULL) freeapplib(idfieldlst);
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
	RDAeditfld *cf1;
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
	RDAeditfld *cf;
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
					holdconfld=Rmalloc(holdnum*sizeof(RDAeditfld));
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
	RDAeditfld *c;

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
		changedEditWdgtss=TRUE;
	}
	quitecon(econrsrc,msuborsrc);
}
static void quitecontest(RDArsrc *econrsrc,RDArsrc *msuborsrc)
{
	readallwidgets(econrsrc);
	if(COMPARE_RSRCS(econrsrc,econdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","MASTER CHANGES NOT SAVED!","Changes have been made to this CONNECTFIELD!\nDo you want to Save these changes?",saveecon,quitecon,FALSE,2,econrsrc,msuborsrc,NULL);
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
	char *fldname=NULL,*cfldname=NULL;
	RDAeditfld *c;
	RDArsrc *econrsrc=NULL;

	readallwidgets(msuborsrc);
	if(FINDRSCGETINT(msuborsrc,"CONNECT FIELDNAME",&selectfld)) return;
	if(holdconfld!=NULL && selectfld<holdnum)
	{
		c=holdconfld+selectfld;
		fldname=stralloc(c->fldname);
		cfldname=stralloc(c->conname);
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
	addlstrsrc(econrsrc,"CONNECT FIELDS",&x,TRUE,NULL,econflds->numlibs,
		&econflds->libs,NULL);
	addrfcbrsrc(econrsrc,"SAVE",TRUE,saveecon,msuborsrc);
	addrfcbrsrc(econrsrc,"QUIT",TRUE,quitecontest,msuborsrc);
	addbtnrsrc(econrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(econrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(econrsrc,TRUE,quitecon,msuborsrc,FALSE);
	setconfldlist(econrsrc,econflds,cfldname);
	if(cfldname!=NULL) Rfree(cfldname);
		econdefaults=GetDefaults(econrsrc);
}
static void dofieldlist(RDArsrc *msuborsrc)
{
	int selectedm=0,selectedf=0,selectedfld=0,selectedidfld=0;
	int y,z;
	NRDfield *fields=NULL,*f;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short nofields=0,nokeys=0,engine=0;
/*
	char *temp=NULL;
*/

	if(FINDRSCGETINT(msuborsrc,"MODULE",&selectedm)) return;
	if(FINDRSCGETINT(msuborsrc,"FILENAME",&selectedf)) return;
	if(FINDRSCGETINT(msuborsrc,"ASSIGN FIELDNAME",&selectedfld)) return;
	if(FINDRSCGETINT(msuborsrc,"MANAGED FIELDNAME",&selectedidfld)) return;
	if(fieldlst!=NULL) freeapplib(fieldlst);
	fieldlst=APPlibNEW();
	if(idfieldlst!=NULL) freeapplib(idfieldlst);
	idfieldlst=APPlibNEW();
	if(!GETBIN(modlstx->libs[selectedm],filelst->libs[selectedf],&engine,&nofields,&fields,&nokeys,&keys))
	{
		if(fields!=NULL)
		{
			for(y=0,f=fields;y<nofields;++y,++f)
			{
/*
				if(temp!=NULL)
				{
					temp=Rrealloc(temp,RDAstrlen(f->name)+1);
				} else {
					temp=Rmalloc(RDAstrlen(f->name)+1);
				}
				sprintf(temp,"%s",f->name);
*/
				if(f->type==BOOLNS)
				{
					addAPPlibNoDuplicates(fieldlst,f->name);
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
						if(y==0)
						{
							addAPPlibNoDuplicates(idfieldlst,part->name);
						}
						if(part->name!=NULL) Rfree(part->name);
					}
					Rfree(keyx->part);
				}
			}
			Rfree(keys);
		}
	}
	if(fieldlst->numlibs<1)
	{
		addAPPlib(fieldlst,"Contains No Fields");
	}
	SORTAPPlib(fieldlst);
	if(selectedfld>=fieldlst->numlibs) selectedfld=0;
	if(!FINDRSCLISTAPPlib(msuborsrc,"ASSIGN FIELDNAME",selectedfld,fieldlst))
	{
		updatersrc(msuborsrc,"ASSIGN FIELDNAME");
	}
	if(idfieldlst->numlibs<1)
	{
		addAPPlib(idfieldlst,"Contains No Fields");
	}
	SORTAPPlib(idfieldlst);
	if(selectedidfld>=idfieldlst->numlibs) selectedidfld=0;
	if(!FINDRSCLISTAPPlib(msuborsrc,"MANAGED FIELDNAME",selectedidfld,idfieldlst))
	{
		updatersrc(msuborsrc,"MANAGED FIELDNAME");
	}
}
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
	dofieldlist(msuborsrc);
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
static void savemsubo(RDArsrc *mrsrc,RDArsrc *subrsrc)
{
	int value=0,type=0,len=0,btype=0;
	AddEditWdgt *EditWdgt=NULL;
	int selected=0;
	RDAeditfld *cf1,*cf2;
	int x;

	if(FINDRSCGETINT(subrsrc,"EDIT WIDGETS",&value)) return;
	readallwidgets(mrsrc);
	EditWdgt=tempEditWdgts+value;
	FINDRSCGETSTRING(mrsrc,"NAME",&EditWdgt->name);
	FINDRSCGETINT(mrsrc,"FIELD TYPES",&type);
	FINDRSCGETINT(mrsrc,"FIELD LENGTH",&len);
	FINDRSCGETCHAR(mrsrc,"SET ACCOUNT TYPE",&EditWdgt->setaccttype);
	if(EditWdgt->setaccttype==TRUE)
	{
		if(EditWdgt->acctrsc!=NULL) freeapplib(EditWdgt->acctrsc);
		if(ACCTrsc!=NULL)
		{
			if(ACCTrsc->numlibs>0)
			{
				EditWdgt->acctrsc=APPlibNEW();
				for(x=0;x<ACCTrsc->numlibs;++x)
					addAPPlib(EditWdgt->acctrsc,ACCTrsc->libs[x]);
			}
		}
	} else {
		if(EditWdgt->acctrsc!=NULL) freeapplib(EditWdgt->acctrsc);
	}
	FINDRSCGETCHAR(mrsrc,"SET DEFINITION TYPE",&EditWdgt->setdeftype);
	if(EditWdgt->setdeftype==TRUE)
	{
		if(EditWdgt->defrsc!=NULL) freeapplib(EditWdgt->defrsc);
		if(DEFrsc!=NULL)
		{
			if(DEFrsc->numlibs>0)
			{
				EditWdgt->defrsc=APPlibNEW();
				for(x=0;x<DEFrsc->numlibs;++x)
					addAPPlib(EditWdgt->defrsc,DEFrsc->libs[x]);
			}
		}
	} else {
		if(EditWdgt->defrsc!=NULL) freeapplib(EditWdgt->defrsc);
	}
	FINDRSCGETCHAR(mrsrc,"MANAGED",&EditWdgt->id_mgt);
	FINDRSCGETINT(mrsrc,"EXPRESSION TYPES",&EditWdgt->etype);
	FINDRSCGETINT(mrsrc,"BUILD TYPES",&btype);
	EditWdgt->loadvirtual=btype;
	if(EditWdgt->id_mgt)
	{
		FINDRSCGETSTRING(mrsrc,"EXPRESSION",&EditWdgt->expression);
		FINDRSCGETINT(mrsrc,"MODULE",&selected);
		EditWdgt->module=stralloc(modlstx->libs[selected]);
		FINDRSCGETINT(mrsrc,"FILENAME",&selected);
		EditWdgt->filename=stralloc(filelst->libs[selected]);
		FINDRSCGETINT(mrsrc,"KEYNAME",&selected);
		EditWdgt->subkeyname=stralloc(subkeylist->libs[selected]);
		if(holdconfld!=NULL)
		{
			EditWdgt->num=holdnum;
			EditWdgt->confld=Rmalloc((holdnum)*sizeof(RDAeditfld));
			for(x=0,cf1=holdconfld,cf2=EditWdgt->confld;x<holdnum;++x,++cf1,++cf2)
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
		FINDRSCGETSTRING(mrsrc,"MANAGED EXPRESSION",&EditWdgt->mgt_expression);
		FINDRSCGETINT(mrsrc,"ASSIGN FIELDNAME",&selected);
		EditWdgt->assignname=stralloc(fieldlst->libs[selected]);
		FINDRSCGETINT(mrsrc,"MANAGED FIELDNAME",&selected);
		EditWdgt->id=stralloc(idfieldlst->libs[selected]);
		FINDRSCGETSTRING(mrsrc,"OVERRIDE EXPRESSION",&EditWdgt->override_expression);
	} else {
		FINDRSCGETSTRING(mrsrc,"EXPRESSION",&EditWdgt->expression);
		if(EditWdgt->module!=NULL) Rfree(EditWdgt->module);
		if(EditWdgt->filename!=NULL) Rfree(EditWdgt->filename);
		if(EditWdgt->subkeyname!=NULL) Rfree(EditWdgt->subkeyname);
		if(EditWdgt->confld!=NULL) 
		{
			for(x=0,cf1=EditWdgt->confld;x<EditWdgt->num;++x,++cf1)
			{
				if(cf1->fldname!=NULL)
				{
					Rfree(cf1->fldname);
				}
				if(cf1->conname!=NULL)
				{
					Rfree(cf1->conname);
				} 
			}
			Rfree(EditWdgt->confld);
			EditWdgt->confld=NULL;
			EditWdgt->num=0;
		}
		if(EditWdgt->mgt_expression!=NULL) Rfree(EditWdgt->mgt_expression);
		if(EditWdgt->assignname!=NULL) Rfree(EditWdgt->assignname);
		if(EditWdgt->id!=NULL) Rfree(EditWdgt->id);
		if(EditWdgt->override_expression!=NULL) Rfree(EditWdgt->override_expression);
	}
	EditWdgt->len=len;
	EditWdgt->type=type;	
	getWidgets();
	if(!FINDRSCLISTAPPlib(subrsrc,"EDIT WIDGETS",value,wdgtavl))
		updatersrc(subrsrc,"EDIT WIDGETS");
	if(COMPARE_RSRCS(mrsrc,msubsdefaults,2))
	{
		changedvalues=TRUE;
	}	
	quitmsubo(mrsrc,subrsrc);
}
static void savemsuboabove(RDArsrc *mrsrc,RDArsrc *subrsrc)
{
	int x;

	if(FINDRSCGETINT(subrsrc,"EDIT WIDGETS",&x)) return;
	makespaceEditWdgts(x);
	savemsubo(mrsrc,subrsrc);
}
static void savemsubobelow(RDArsrc *mrsrc,RDArsrc *subrsrc)
{
	int x;

	if(FINDRSCGETINT(subrsrc,"EDIT WIDGETS",&x)) return;
	makespaceEditWdgts(++x);
	FINDRSCSETINT(subrsrc,"EDIT WIDGETS",x);
	savemsubo(mrsrc,subrsrc);
}
static void setmgtflag(RDArsrc *parent)
{
	char mgtd=FALSE;
	
	readwidget(parent,"MANAGED");
	FINDRSCGETCHAR(parent,"MANAGED",&mgtd);
	if(mgtd==TRUE)
	{
		FINDRSCSETSENSITIVE(parent,"MODULE",TRUE);
		FINDRSCSETEDITABLE(parent,"MODULE",TRUE);
		FINDRSCSETSENSITIVE(parent,"FILENAME",TRUE);
		FINDRSCSETEDITABLE(parent,"FILENAME",TRUE);
		FINDRSCSETSENSITIVE(parent,"KEYNAME",TRUE);
		FINDRSCSETEDITABLE(parent,"KEYNAME",TRUE);
		FINDRSCSETSENSITIVE(parent,"MANAGED EXPRESSION",TRUE);
		FINDRSCSETEDITABLE(parent,"MANAGED EXPRESSION",TRUE);
		FINDRSCSETSENSITIVE(parent,"OVERRIDE EXPRESSION",TRUE);
		FINDRSCSETEDITABLE(parent,"OVERRIDE EXPRESSION",TRUE);
		FINDRSCSETSENSITIVE(parent,"ASSIGN FIELDNAME",TRUE);
		FINDRSCSETEDITABLE(parent,"ASSIGN FIELDNAME",TRUE);
		FINDRSCSETSENSITIVE(parent,"MANAGED FIELDNAME",TRUE);
		FINDRSCSETEDITABLE(parent,"MANAGED FIELDNAME",TRUE);
		FINDRSCSETSENSITIVE(parent,"CONNECT FIELDNAME",TRUE);
		FINDRSCSETEDITABLE(parent,"CONNECT FIELDNAME",TRUE);
		FINDRSCSETSENSITIVE(parent,"MANAGED LOAD VALUE",TRUE);
		FINDRSCSETEDITABLE(parent,"MANAGED LOAD VALUE",TRUE);
		FINDRSCSETSENSITIVE(parent,"MANAGED LOAD OPERATOR",TRUE);
		FINDRSCSETEDITABLE(parent,"MANAGED LOAD OPERATOR",TRUE);
		FINDRSCSETSENSITIVE(parent,"MANAGED LOAD GROUPER",TRUE);
		FINDRSCSETEDITABLE(parent,"MANAGED LOAD GROUPER",TRUE);
		FINDRSCSETSENSITIVE(parent,"MANAGED LOAD CONTROL",TRUE);
		FINDRSCSETEDITABLE(parent,"MANAGED LOAD CONTROL",TRUE);
		FINDRSCSETSENSITIVE(parent,"OVERRIDE LOAD VALUE",TRUE);
		FINDRSCSETEDITABLE(parent,"OVERRIDE LOAD VALUE",TRUE);
		FINDRSCSETSENSITIVE(parent,"OVERRIDE LOAD OPERATOR",TRUE);
		FINDRSCSETEDITABLE(parent,"OVERRIDE LOAD OPERATOR",TRUE);
		FINDRSCSETSENSITIVE(parent,"OVERRIDE LOAD GROUPER",TRUE);
		FINDRSCSETEDITABLE(parent,"OVERRIDE LOAD GROUPER",TRUE);
		FINDRSCSETSENSITIVE(parent,"OVERRIDE LOAD CONTROL",TRUE);
		FINDRSCSETEDITABLE(parent,"OVERRIDE LOAD CONTROL",TRUE);
		FINDRSCSETSENSITIVE(parent,"SELECT",TRUE);
		FINDRSCSETEDITABLE(parent,"SELECT",TRUE);
	} else {
		FINDRSCSETSENSITIVE(parent,"MODULE",FALSE);
		FINDRSCSETEDITABLE(parent,"MODULE",FALSE);
		FINDRSCSETSENSITIVE(parent,"FILENAME",FALSE);
		FINDRSCSETEDITABLE(parent,"FILENAME",FALSE);
		FINDRSCSETSENSITIVE(parent,"KEYNAME",FALSE);
		FINDRSCSETEDITABLE(parent,"KEYNAME",FALSE);
		FINDRSCSETSENSITIVE(parent,"MANAGED EXPRESSION",FALSE);
		FINDRSCSETEDITABLE(parent,"MANAGED EXPRESSION",FALSE);
		FINDRSCSETSENSITIVE(parent,"OVERRIDE EXPRESSION",FALSE);
		FINDRSCSETEDITABLE(parent,"OVERRIDE EXPRESSION",FALSE);
		FINDRSCSETSENSITIVE(parent,"ASSIGN FIELDNAME",FALSE);
		FINDRSCSETEDITABLE(parent,"ASSIGN FIELDNAME",FALSE);
		FINDRSCSETSENSITIVE(parent,"MANAGED FIELDNAME",FALSE);
		FINDRSCSETEDITABLE(parent,"MANAGED FIELDNAME",FALSE);
		FINDRSCSETSENSITIVE(parent,"CONNECT FIELDNAME",FALSE);
		FINDRSCSETEDITABLE(parent,"CONNECT FIELDNAME",FALSE);
		FINDRSCSETSENSITIVE(parent,"MANAGED LOAD VALUE",FALSE);
		FINDRSCSETEDITABLE(parent,"MANAGED LOAD VALUE",FALSE);
		FINDRSCSETSENSITIVE(parent,"MANAGED LOAD OPERATOR",FALSE);
		FINDRSCSETEDITABLE(parent,"MANAGED LOAD OPERATOR",FALSE);
		FINDRSCSETSENSITIVE(parent,"MANAGED LOAD GROUPER",FALSE);
		FINDRSCSETEDITABLE(parent,"MANAGED LOAD GROUPER",FALSE);
		FINDRSCSETSENSITIVE(parent,"MANAGED LOAD CONTROL",FALSE);
		FINDRSCSETEDITABLE(parent,"MANAGED LOAD CONTROL",FALSE);
		FINDRSCSETSENSITIVE(parent,"OVERRIDE LOAD VALUE",FALSE);
		FINDRSCSETEDITABLE(parent,"OVERRIDE LOAD VALUE",FALSE);
		FINDRSCSETSENSITIVE(parent,"OVERRIDE LOAD OPERATOR",FALSE);
		FINDRSCSETEDITABLE(parent,"OVERRIDE LOAD OPERATOR",FALSE);
		FINDRSCSETSENSITIVE(parent,"OVERRIDE LOAD GROUPER",FALSE);
		FINDRSCSETEDITABLE(parent,"OVERRIDE LOAD GROUPER",FALSE);
		FINDRSCSETSENSITIVE(parent,"OVERRIDE LOAD CONTROL",FALSE);
		FINDRSCSETEDITABLE(parent,"OVERRIDE LOAD CONTROL",FALSE);
		FINDRSCSETSENSITIVE(parent,"SELECT",FALSE);
		FINDRSCSETEDITABLE(parent,"SELECT",FALSE);
	}
}
static void setacctfunc(RDArsrc *r)
{
	char t=FALSE;

	readwidget(r,"SET ACCOUNT TYPE");
	FINDRSCGETCHAR(r,"SET ACCOUNT TYPE",&t);
	if(t==FALSE)
	{
		FINDRSCSETEDITABLE(r,"ACCOUNT RESOURCES",FALSE);
		FINDRSCSETSENSITIVE(r,"ACCOUNT RESOURCES",FALSE);
		FINDRSCSETEDITABLE(r,"ADD ACCOUNT TYPE ABOVE",FALSE);
		FINDRSCSETSENSITIVE(r,"ADD ACCOUNT TYPE ABOVE",FALSE);
		FINDRSCSETEDITABLE(r,"ADD ACCOUNT TYPE BELOW",FALSE);
		FINDRSCSETSENSITIVE(r,"ADD ACCOUNT TYPE BELOW",FALSE);
		FINDRSCSETEDITABLE(r,"EDIT ACCOUNT TYPE",FALSE);
		FINDRSCSETSENSITIVE(r,"EDIT ACCOUNT TYPE",FALSE);
		FINDRSCSETEDITABLE(r,"DELETE ACCOUNT TYPE",FALSE);
		FINDRSCSETSENSITIVE(r,"DELETE ACCOUNT TYPE",FALSE);
	} else {
		FINDRSCSETSENSITIVE(r,"ACCOUNT RESOURCES",TRUE);
		FINDRSCSETEDITABLE(r,"ACCOUNT RESOURCES",TRUE);
		FINDRSCSETEDITABLE(r,"ADD ACCOUNT TYPE ABOVE",TRUE);
		FINDRSCSETSENSITIVE(r,"ADD ACCOUNT TYPE ABOVE",TRUE);
		FINDRSCSETEDITABLE(r,"ADD ACCOUNT TYPE BELOW",TRUE);
		FINDRSCSETSENSITIVE(r,"ADD ACCOUNT TYPE BELOW",TRUE);
		FINDRSCSETEDITABLE(r,"EDIT ACCOUNT TYPE",TRUE);
		FINDRSCSETSENSITIVE(r,"EDIT ACCOUNT TYPE",TRUE);
		FINDRSCSETEDITABLE(r,"DELETE ACCOUNT TYPE",TRUE);
		FINDRSCSETSENSITIVE(r,"DELETE ACCOUNT TYPE",TRUE);
	}
}
static void setdeffunc(RDArsrc *r)
{
	char t=FALSE;

	readwidget(r,"SET DEFINITION TYPE");
	FINDRSCGETCHAR(r,"SET DEFINITION TYPE",&t);
	if(t==FALSE)
	{
		FINDRSCSETEDITABLE(r,"DEFINITION TYPE RESOURCES",FALSE);
		FINDRSCSETSENSITIVE(r,"DEFINITION TYPE RESOURCES",FALSE);
		FINDRSCSETEDITABLE(r,"ADD DEFINITION TYPE ABOVE",FALSE);
		FINDRSCSETSENSITIVE(r,"ADD DEFINITION TYPE ABOVE",FALSE);
		FINDRSCSETEDITABLE(r,"ADD DEFINITION TYPE BELOW",FALSE);
		FINDRSCSETSENSITIVE(r,"ADD DEFINITION TYPE BELOW",FALSE);
		FINDRSCSETEDITABLE(r,"EDIT DEFINITION TYPE",FALSE);
		FINDRSCSETSENSITIVE(r,"EDIT DEFINITION TYPE",FALSE);
		FINDRSCSETEDITABLE(r,"DELETE DEFINITION TYPE",FALSE);
		FINDRSCSETSENSITIVE(r,"DELETE DEFINITION TYPE",FALSE);
	} else {
		FINDRSCSETSENSITIVE(r,"DEFINITION TYPE RESOURCES",TRUE);
		FINDRSCSETEDITABLE(r,"DEFINITION TYPE RESOURCES",TRUE);
		FINDRSCSETEDITABLE(r,"ADD DEFINITION TYPE ABOVE",TRUE);
		FINDRSCSETSENSITIVE(r,"ADD DEFINITION TYPE ABOVE",TRUE);
		FINDRSCSETEDITABLE(r,"ADD DEFINITION TYPE BELOW",TRUE);
		FINDRSCSETSENSITIVE(r,"ADD DEFINITION TYPE BELOW",TRUE);
		FINDRSCSETEDITABLE(r,"EDIT DEFINITION TYPE",TRUE);
		FINDRSCSETSENSITIVE(r,"EDIT DEFINITION TYPE",TRUE);
		FINDRSCSETEDITABLE(r,"DELETE DEFINITION TYPE",TRUE);
		FINDRSCSETSENSITIVE(r,"DELETE DEFINITION TYPE",TRUE);
	}
}
/* BEGINNING OF ACCT SECTION */
static void quit_acct(RDArsrc *acct_rsrc,RDArsrc *meditwgtrsrc)
{
	if(acctdefaults!=NULL) FreeRDAdefault(acctdefaults);
	if(acct_rsrc!=NULL) 
	{
		killwindow(acct_rsrc);
		free_rsrc(acct_rsrc);
	}
	if(changedacct) changedEditWdgtss=TRUE;
}
static void quit_acctcls(RDArsrc *acct_rsrc,RDArsrc *meditwgtrsrc)
{
	changedacct=FALSE;
	quit_acct(acct_rsrc,meditwgtrsrc);
}
static void save_acct(RDArsrc *acct_rsrc,RDArsrc *meditwgtrsrc)
{
	int x,selected=0;
	char *temp=NULL;
	APPlib *a=NULL;

	readallwidgets(acct_rsrc);
	if(FINDRSCGETINT(meditwgtrsrc,"ACCOUNT RESOURCES",&selected)) return;
	FINDRSCGETSTRING(acct_rsrc,"RESOURCE",&temp);
	if(temp!=NULL)
	{
		a=ACCTrsc;
		ACCTrsc=APPlibNEW();
		for(x=0;x<a->numlibs;++x)
		{
			if(x==selected) addAPPlib(ACCTrsc,temp);
				else addAPPlib(ACCTrsc,a->libs[x]);
		}
		if(temp!=NULL) Rfree(temp);
		if(a!=NULL) freeapplib(a);
	}
	if(selected>=ACCTrsc->numlibs) selected=0;
	if(!FINDRSCLISTAPPlib(meditwgtrsrc,"ACCOUNT RESOURCES",selected,ACCTrsc))
		updatersrc(meditwgtrsrc,"ACCOUNT RESOURCES");
	if(COMPARE_RSRCS(acct_rsrc,acctdefaults,2))
	{
		changedacct=TRUE;
	}
	quit_acct(acct_rsrc,meditwgtrsrc);
}
static void save_acct_above(RDArsrc *acct_rsrc,RDArsrc *meditwgtrsrc)
{
	int x,selected=0;
	char *temp=NULL;
	APPlib *a=NULL;

	readallwidgets(acct_rsrc);
	if(FINDRSCGETINT(meditwgtrsrc,"ACCOUNT RESOURCES",&selected)) return;
	FINDRSCGETSTRING(acct_rsrc,"RESOURCE",&temp);
	if(temp!=NULL)
	{
		a=ACCTrsc;
		ACCTrsc=APPlibNEW();
		for(x=0;x<a->numlibs;++x)
		{
			if(x==selected) addAPPlib(ACCTrsc,temp);
			addAPPlib(ACCTrsc,a->libs[x]);
		}
		if(temp!=NULL) Rfree(temp);
		if(a!=NULL) freeapplib(a);
	}
	if(selected>=ACCTrsc->numlibs) selected=0;
	if(!FINDRSCLISTAPPlib(meditwgtrsrc,"ACCOUNT RESOURCES",selected,ACCTrsc))
		updatersrc(meditwgtrsrc,"ACCOUNT RESOURCES");
	if(COMPARE_RSRCS(acct_rsrc,acctdefaults,2))
	{
		changedacct=TRUE;
	}
	quit_acct(acct_rsrc,meditwgtrsrc);
}
static void save_acct_below(RDArsrc *acct_rsrc,RDArsrc *meditwgtrsrc)
{
	int x,selected=0;
	char *temp=NULL;
	APPlib *a=NULL;

	readallwidgets(acct_rsrc);
	if(FINDRSCGETINT(meditwgtrsrc,"ACCOUNT RESOURCES",&selected)) return;
	FINDRSCGETSTRING(acct_rsrc,"RESOURCE",&temp);
	if(temp!=NULL)
	{
		a=ACCTrsc;
		ACCTrsc=APPlibNEW();
		for(x=0;x<a->numlibs;++x)
		{
			addAPPlib(ACCTrsc,a->libs[x]);
			if(x==selected) addAPPlib(ACCTrsc,temp);
		}
		if(temp!=NULL) Rfree(temp);
		if(a!=NULL) freeapplib(a);
		if(COMPARE_RSRCS(acct_rsrc,acctdefaults,2))
		{
			changedacct=TRUE;
		}
	}
	selected+=1;
	if(selected>=ACCTrsc->numlibs) selected=0;
	if(!FINDRSCLISTAPPlib(meditwgtrsrc,"ACCOUNT RESOURCES",selected,ACCTrsc))
		updatersrc(meditwgtrsrc,"ACCOUNT RESOURCES");
	quit_acct(acct_rsrc,meditwgtrsrc);
}
static void quit_acct_test(RDArsrc *acct_rsrc,RDArsrc *meditwgtrsrc)
{
	readallwidgets(acct_rsrc);
	if(COMPARE_RSRCS(acct_rsrc,acctdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made, and are about to be aborted!\nDo you want to save these changes?",save_acct,quit_acctcls,FALSE,2,acct_rsrc,meditwgtrsrc,NULL);
	} else {
		quit_acct(acct_rsrc,meditwgtrsrc);
	}
}
static void quit_acct_above_test(RDArsrc *acct_rsrc,RDArsrc *meditwgtrsrc)
{
	readallwidgets(acct_rsrc);
	if(COMPARE_RSRCS(acct_rsrc,acctdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made, and are about to be aborted!\nDo you want to save these changes?",save_acct_above,quit_acctcls,FALSE,2,acct_rsrc,meditwgtrsrc,NULL);
	} else {
		quit_acct(acct_rsrc,meditwgtrsrc);
	}
}
static void quit_acct_below_test(RDArsrc *acct_rsrc,RDArsrc *meditwgtrsrc)
{
	readallwidgets(acct_rsrc);
	if(COMPARE_RSRCS(acct_rsrc,acctdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made, and are about to be aborted!\nDo you want to save these changes?",save_acct_below,quit_acctcls,FALSE,2,acct_rsrc,meditwgtrsrc,NULL);
	} else {
		quit_acct(acct_rsrc,meditwgtrsrc);
	}
}
static void acctresourcescreen(RDArsrc *r,char *argument,
	void (*savefunc)(...),void (*quitfunc)(...))
{
	RDArsrc *acct_rsrc=NULL;

	acct_rsrc=RDArsrcNEW(module,"MAINTAIN RESOURCE SCREEN");
	addstdrsrc(acct_rsrc,"RESOURCE",VARIABLETEXT,0,(argument!=NULL?argument:""),TRUE);
	addbtnrsrc(acct_rsrc,"SAVE",TRUE,savefunc,r);
	addbtnrsrc(acct_rsrc,"QUIT",TRUE,quitfunc,r);
	addbtnrsrc(acct_rsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(acct_rsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	acctdefaults=GetDefaults(acct_rsrc);
	APPmakescrn(acct_rsrc,TRUE,quitfunc,r,FALSE);
}
static void add_acct_above(RDArsrc *meditwgtrsrc)
{
	int x;

	if(FINDRSCGETINT(meditwgtrsrc,"ACCOUNT RESOURCES",&x)) return;
	if(!RDAstrcmp(ACCTrsc->libs[x],"No Resources"))
	{
		acctresourcescreen(meditwgtrsrc,NULL,save_acct,quit_acct_test);
	} else {
		acctresourcescreen(meditwgtrsrc,NULL,save_acct_above,
			quit_acct_above_test);
	}
}
static void add_acct_below(RDArsrc *meditwgtrsrc)
{
	int x;

	if(FINDRSCGETINT(meditwgtrsrc,"ACCOUNT RESOURCES",&x)) return;
	if(!RDAstrcmp(ACCTrsc->libs[x],"No Resources"))
	{
		acctresourcescreen(meditwgtrsrc,NULL,save_acct,quit_acct_test);
	} else {
		acctresourcescreen(meditwgtrsrc,NULL,save_acct_below,
			quit_acct_below_test);
	}
}
static void select_acct(RDArsrc *meditwgtrsrc)
{
	int x;

	if(FINDRSCGETINT(meditwgtrsrc,"ACCOUNT RESOURCES",&x)) return;
	if(!RDAstrcmp(ACCTrsc->libs[x],"No Resources"))
	{
		acctresourcescreen(meditwgtrsrc,NULL,save_acct,quit_acct_test);
	} else {
		acctresourcescreen(meditwgtrsrc,ACCTrsc->libs[x],save_acct,quit_acct_test);
	}
}
static void delete_acct(RDArsrc *meditwgtrsrc)
{
	int selected=0,x=0;
	APPlib *a=NULL;

	FINDRSCGETINT(meditwgtrsrc,"ACCOUNT RESOURCES",&selected);
	if(ACCTrsc->numlibs>0 && RDAstrcmp(ACCTrsc->libs[1],"No Resources"))
	{
		a=ACCTrsc;
		ACCTrsc=APPlibNEW();
		for(x=0;x<a->numlibs;++x)
		{
			if(x!=selected)
			{
				addAPPlib(ACCTrsc,a->libs[x]);
			}
		}
		freeapplib(a);
	} else {
		freeapplib(ACCTrsc);
		ACCTrsc=APPlibNEW();
	}
	if(ACCTrsc->numlibs<1)
	{
		addAPPlib(ACCTrsc,"No Resources");
	}
	if(selected>=ACCTrsc->numlibs) selected=0;
	FINDRSCLISTAPPlib(meditwgtrsrc,"ACCOUNT RESOURCES",selected,ACCTrsc);
	updatersrc(meditwgtrsrc,"ACCOUNT RESOURCES");
}
/* END OF ACCT SECTION */
/* BEGINNING OF DEFINITION TYPE SECTION */
static void quit_deftype(RDArsrc *deftype_rsrc,RDArsrc *meditwgtrsrc)
{
	if(defdefaults!=NULL) FreeRDAdefault(defdefaults);
	if(deftype_rsrc!=NULL) 
	{
		killwindow(deftype_rsrc);
		free_rsrc(deftype_rsrc);
	}
	if(changeddef) changedEditWdgtss=TRUE;
}
static void quit_deftypecls(RDArsrc *deftype_rsrc,RDArsrc *meditwgtrsrc)
{
	changeddef=FALSE;
	quit_deftype(deftype_rsrc,meditwgtrsrc);
}
static void save_deftype(RDArsrc *deftype_rsrc,RDArsrc *meditwgtrsrc)
{
	int x,selected=0;
	char *temp=NULL;
	APPlib *a=NULL;

	readallwidgets(deftype_rsrc);
	if(FINDRSCGETINT(meditwgtrsrc,"DEFINITION TYPE RESOURCES",&selected)) return;
	FINDRSCGETSTRING(deftype_rsrc,"RESOURCE",&temp);
	if(temp!=NULL)
	{
		a=DEFrsc;
		DEFrsc=APPlibNEW();
		for(x=0;x<a->numlibs;++x)
		{
			if(x==selected) addAPPlib(DEFrsc,temp);
				else addAPPlib(DEFrsc,a->libs[x]);
		}
		if(temp!=NULL) Rfree(temp);
		if(a!=NULL) freeapplib(a);
	}
	if(selected>=DEFrsc->numlibs) selected=0;
	if(!FINDRSCLISTAPPlib(meditwgtrsrc,"DEFINITION TYPE RESOURCES",selected,DEFrsc))
		updatersrc(meditwgtrsrc,"DEFINITION TYPE RESOURCES");
	if(COMPARE_RSRCS(deftype_rsrc,defdefaults,2))
	{
		changeddef=TRUE;
	}
	quit_deftype(deftype_rsrc,meditwgtrsrc);
}
static void save_deftype_above(RDArsrc *deftype_rsrc,RDArsrc *meditwgtrsrc)
{
	int x,selected=0;
	char *temp=NULL;
	APPlib *a=NULL;

	readallwidgets(deftype_rsrc);
	if(FINDRSCGETINT(meditwgtrsrc,"DEFINITION TYPE RESOURCES",&selected)) return;
	FINDRSCGETSTRING(deftype_rsrc,"RESOURCE",&temp);
	if(temp!=NULL)
	{
		a=DEFrsc;
		DEFrsc=APPlibNEW();
		for(x=0;x<a->numlibs;++x)
		{
			if(x==selected) addAPPlib(DEFrsc,temp);
			addAPPlib(DEFrsc,a->libs[x]);
		}
		if(temp!=NULL) Rfree(temp);
		if(a!=NULL) freeapplib(a);
	}
	if(selected>=DEFrsc->numlibs) selected=0;
	if(!FINDRSCLISTAPPlib(meditwgtrsrc,"DEFINITION TYPE RESOURCES",selected,DEFrsc))
		updatersrc(meditwgtrsrc,"DEFINITION TYPE RESOURCES");
	if(COMPARE_RSRCS(deftype_rsrc,defdefaults,2))
	{
		changeddef=TRUE;
	}
	quit_deftype(deftype_rsrc,meditwgtrsrc);
}
static void save_deftype_below(RDArsrc *deftype_rsrc,RDArsrc *meditwgtrsrc)
{
	int x,selected=0;
	char *temp=NULL;
	APPlib *a=NULL;

	readallwidgets(deftype_rsrc);
	if(FINDRSCGETINT(meditwgtrsrc,"DEFINITION TYPE RESOURCES",&selected)) return;
	FINDRSCGETSTRING(deftype_rsrc,"RESOURCE",&temp);
	if(temp!=NULL)
	{
		a=DEFrsc;
		DEFrsc=APPlibNEW();
		for(x=0;x<a->numlibs;++x)
		{
			addAPPlib(DEFrsc,a->libs[x]);
			if(x==selected) addAPPlib(DEFrsc,temp);
		}
		if(temp!=NULL) Rfree(temp);
		if(a!=NULL) freeapplib(a);
		if(COMPARE_RSRCS(deftype_rsrc,defdefaults,2))
		{
			changeddef=TRUE;
		}
	}
	selected+=1;
	if(selected>=DEFrsc->numlibs) selected=0;
	if(!FINDRSCLISTAPPlib(meditwgtrsrc,"DEFINITION TYPE RESOURCES",selected,DEFrsc))
		updatersrc(meditwgtrsrc,"DEFINITION TYPE RESOURCES");
	quit_deftype(deftype_rsrc,meditwgtrsrc);
}
static void quit_deftype_test(RDArsrc *deftype_rsrc,RDArsrc *meditwgtrsrc)
{
	readallwidgets(deftype_rsrc);
	if(COMPARE_RSRCS(deftype_rsrc,defdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made, and are about to be aborted!\nDo you want to save these changes?",save_deftype,quit_deftypecls,FALSE,2,deftype_rsrc,meditwgtrsrc,NULL);
	} else {
		quit_deftype(deftype_rsrc,meditwgtrsrc);
	}
}
static void quit_deftype_above_test(RDArsrc *deftype_rsrc,RDArsrc *meditwgtrsrc)
{
	readallwidgets(deftype_rsrc);
	if(COMPARE_RSRCS(deftype_rsrc,defdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made, and are about to be aborted!\nDo you want to save these changes?",save_deftype_above,quit_deftypecls,FALSE,2,deftype_rsrc,meditwgtrsrc,NULL);
	} else {
		quit_deftype(deftype_rsrc,meditwgtrsrc);
	}
}
static void quit_deftype_below_test(RDArsrc *deftype_rsrc,RDArsrc *meditwgtrsrc)
{
	readallwidgets(deftype_rsrc);
	if(COMPARE_RSRCS(deftype_rsrc,defdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made, and are about to be aborted!\nDo you want to save these changes?",save_deftype_below,quit_deftypecls,FALSE,2,deftype_rsrc,meditwgtrsrc,NULL);
	} else {
		quit_deftype(deftype_rsrc,meditwgtrsrc);
	}
}
static void deftyperesourcescreen(RDArsrc *r,char *argument,
	void (*savefunc)(...),void (*quitfunc)(...))
{
	RDArsrc *deftype_rsrc=NULL;

	deftype_rsrc=RDArsrcNEW(module,"MAINTAIN RESOURCE SCREEN");
	addstdrsrc(deftype_rsrc,"RESOURCE",VARIABLETEXT,0,(argument!=NULL?argument:""),TRUE);
	addbtnrsrc(deftype_rsrc,"SAVE",TRUE,savefunc,r);
	addbtnrsrc(deftype_rsrc,"QUIT",TRUE,quitfunc,r);
	addbtnrsrc(deftype_rsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(deftype_rsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	defdefaults=GetDefaults(deftype_rsrc);
	APPmakescrn(deftype_rsrc,TRUE,quitfunc,r,FALSE);
}
static void add_deftype_above(RDArsrc *meditwgtrsrc)
{
	int x;

	if(FINDRSCGETINT(meditwgtrsrc,"DEFINITION TYPE RESOURCES",&x)) return;
	if(!RDAstrcmp(DEFrsc->libs[x],"No Resources"))
	{
		deftyperesourcescreen(meditwgtrsrc,NULL,save_deftype,quit_deftype_test);
	} else {
		deftyperesourcescreen(meditwgtrsrc,NULL,save_deftype_above,
			quit_deftype_above_test);
	}
}
static void add_deftype_below(RDArsrc *meditwgtrsrc)
{
	int x;

	if(FINDRSCGETINT(meditwgtrsrc,"DEFINITION TYPE RESOURCES",&x)) return;
	if(!RDAstrcmp(DEFrsc->libs[x],"No Resources"))
	{
		deftyperesourcescreen(meditwgtrsrc,NULL,save_deftype,quit_deftype_test);
	} else {
		deftyperesourcescreen(meditwgtrsrc,NULL,save_deftype_below,
			quit_deftype_below_test);
	}
}
static void select_deftype(RDArsrc *meditwgtrsrc)
{
	int x;

	if(FINDRSCGETINT(meditwgtrsrc,"DEFINITION TYPE RESOURCES",&x)) return;
	if(!RDAstrcmp(DEFrsc->libs[x],"No Resources"))
	{
		deftyperesourcescreen(meditwgtrsrc,NULL,save_deftype,quit_deftype_test);
	} else {
		deftyperesourcescreen(meditwgtrsrc,DEFrsc->libs[x],save_deftype,quit_deftype_test);
	}
}
static void delete_deftype(RDArsrc *meditwgtrsrc)
{
	int selected=0,x=0;
	APPlib *a=NULL;

	FINDRSCGETINT(meditwgtrsrc,"DEFINITION TYPE RESOURCES",&selected);
	if(DEFrsc->numlibs>0 && RDAstrcmp(DEFrsc->libs[1],"No Resources"))
	{
		a=DEFrsc;
		DEFrsc=APPlibNEW();
		for(x=0;x<a->numlibs;++x)
		{
			if(x!=selected)
			{
				addAPPlib(DEFrsc,a->libs[x]);
			}
		}
		freeapplib(a);
	} else {
		freeapplib(DEFrsc);
		DEFrsc=APPlibNEW();
	}
	if(DEFrsc->numlibs<1)
	{
		addAPPlib(DEFrsc,"No Resources");
	}
	if(selected>=DEFrsc->numlibs) selected=0;
	FINDRSCLISTAPPlib(meditwgtrsrc,"DEFINITION TYPE RESOURCES",selected,DEFrsc);
	updatersrc(meditwgtrsrc,"DEFINITION TYPE RESOURCES");
}
/* END OF DEFINITION TYPE SECTION */
static void makeezvirlisttest(RDArsrc *r)
{
	readwidget(r,"MODULE LIST");
	FINDRSCGETINT(r,"MODULE LIST",&virmodpointer);
	if(ezviravl!=NULL) { freeapplib(ezviravl); ezviravl=NULL; }
	makeezvirlist(r);
}
static void makeezvirlist(RDArsrc *r)
{
	APPlib *virflist=NULL;
    	DFvirtual *d=NULL;
	RDAaddsub *tmpsch=NULL;
	DFincvir *a=NULL;
	char *holdmodule=NULL;
	char *tmpx=NULL,addezvf=FALSE,*libx=NULL;
	int x,y,z;
	short addedtolist=FALSE;
	RDArsrc *tmprsrc=NULL;
	char *e=NULL;

	holdmodule=stralloc(mod_list->libs[virmodpointer]);
	if(virflist!=NULL) { freeapplib(virflist); virflist=NULL; }
	if(ezviravl==NULL) 
	{
		if(ezviravl!=NULL) { freeapplib(ezviravl); ezviravl=NULL; }
		ezviravl=APPlibNEW(); 
		if(POWERADD->subord==NULL) 
		{
			WARNINGDIALOG("WARNING DIALOG SCREEN","NO SUPPORTING FILES SELECTED!","There has been no supporting files defined for this power add.  In order to load an EZ-VIRTUAL FIELD this must be done first.",NULL,NULL,TRUE,NULL);
		} else {
			libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(holdmodule)+10);
			tmpx=adddashes(holdmodule);
#ifndef WIN32
			sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,tmpx);
#endif
#ifdef WIN32
			sprintf(libx,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,tmpx);
#endif
			virflist=getlibnames(libx,FALSE);
			if(libx!=NULL) { Rfree(libx); libx=NULL; }
			if(virflist!=NULL)
			{
				e=Rmalloc(RDAstrlen(holdmodule)+27);
				sprintf(e,"Collecting Valid %s Virtuals",holdmodule);
#ifdef RS6000
				tmprsrc=odiagscrn("DIAGNOSTIC SCREEN","RPTGEN",e,NULL,virflist->numlibs);
#else
				tmprsrc=NULL;
				tmprsrc=odiagscrn("DIAGNOSTIC SCREEN","RPTGEN",e,NULL,virflist->numlibs);
#endif
				if(e!=NULL) Rfree(e);
				e=NULL;
				if(tmprsrc!=NULL)
				{
					if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
						ForceWindowUpdate(tmprsrc);
				}
				for(y=0;y<virflist->numlibs;++y)
				{
					addedtolist=FALSE;
					d=getDFvirtual(holdmodule,virflist->libs[y]);
					if(d!=NULL)
					{
						addezvf=FALSE;
						if(d->incvir!=NULL)
						{
						if(!RDAstrcmp(d->incvir->module,holdmodule) &&
						  d->incvir->file==NULL)
						{
							addezvf=TRUE;
						} else {
							for(z=0,a=d->incvir;z<d->num;++z,++a)
							{
								if(d->vtype==1) /*One of Many Available*/
								{
									for(x=0,tmpsch=POWERADD->subord;x<POWERADD->subnum;++x,++tmpsch)
									{
										if(!RDAstrcmp(tmpsch->module,a->module) && 
											!RDAstrcmp(tmpsch->filename,a->file)) break;
									}
									if(x<POWERADD->subnum)
									{
										addezvf=TRUE;
										break;
									} 
									
								} else if(d->vtype==0) /*All Available*/
								{
									for(x=0,tmpsch=POWERADD->subord;x<POWERADD->subnum;++x,++tmpsch)
									{
										if(!RDAstrcmp(tmpsch->module,a->module) &&
											!RDAstrcmp(tmpsch->filename,a->file)) break;
									}
									if(x>=POWERADD->subnum)
									{
										addezvf=FALSE;
										break;
									} else {
										addezvf=TRUE;
									}
								}
							}
						}
						}
						if(addezvf==TRUE)
						{
							addedtolist=TRUE;
							addAPPlibNoDuplicates(ezviravl,d->name);
						}
					}
					FreeDFvirtual(d);
					if(tmprsrc!=NULL) update_diagnostic(tmprsrc,
								(addedtolist ? TRUE:FALSE));
				}
			}
		}
		if(tmprsrc!=NULL)
		{
			killwindow(tmprsrc);
			free_rsrc(tmprsrc); 
		}
		if(holdmodule!=NULL) Rfree(holdmodule);
		if(tmpx!=NULL) Rfree(tmpx);
		if(libx!=NULL) Rfree(libx);
		if(ezviravl->numlibs<1)
		{
			addAPPlib(ezviravl,"No EZ-Virtual Fields Defined");
		}
	}
	if(virflist!=NULL) { freeapplib(virflist); virflist=NULL; }
	if(!FINDRSCLISTAPPlib(r,"VIRTUALS AVAILABLE",0,ezviravl))
	{
		updatersrc(r,"VIRTUALS AVAILABLE");
	}
}
static void changevdesc(RDArsrc *r)
{
	char *temp=NULL;
	int selectedm=0,selectedn=0;

	if(FINDRSCGETINT(r,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(r,"VIRTUALS AVAILABLE",&selectedn)) return;
	if(ezviravl!=NULL)
	{
	if(ezviravl->libs!=NULL)
	{
	if(!RDAstrcmp(ezviravl->libs[selectedn],"No EZ-Virtual Fields Defined"))
	{
		temp=NULL;
	} else {
		temp=getvirtualdesc(mod_list->libs[selectedm],
			ezviravl->libs[selectedn]);
	}
	if(!FINDRSCSETSTRING(r,"DESCRIPTION",temp))
		updatersrc(r,"DESCRIPTION");
	if(temp!=NULL) Rfree(temp);
	}
	}
}
static void quitlvir(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
}
static void oklvir(RDArsrc *r,RDArsrc *ersrc)
{
	DFvirtual *d=NULL;
	int x=0,selectedm=0,selectedv=0,etypes=2;

	if(FINDRSCGETINT(r,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(r,"VIRTUALS AVAILABLE",&selectedv)) return;
	d=getDFvirtual(mod_list->libs[selectedm],ezviravl->libs[selectedv]);
	if(d!=NULL)
	{
		FINDRSCSETSTRING(ersrc,"NAME",d->name);
		x=(int)d->type;
		FINDRSCSETINT(ersrc,"FIELD TYPES",x);
		x=(int)d->when;
		FINDRSCSETSHORT(ersrc,"LENGTH",d->length);
		FINDRSCSETSTRING(ersrc,"EXPRESSION",d->expression);
		FINDRSCSETCHAR(ersrc,"LOADVIRTUAL",TRUE);
		FINDRSCSETINT(ersrc,"EXPRESSION TYPES",etypes);
		updateallrsrc(ersrc);
		FreeDFvirtual(d); 
	} else {
		WARNINGDIALOG("WARNING DIALOG SCREEN","COULD NOT LOAD EZ-VIRTUAL!","The attemp to load the EZ-VIRTUAL FIELD has failed.  Make sure that it is defined.  If so call your installer.",NULL,NULL,TRUE,NULL);
	}
	quitlvir(r);
}
static void makemodlist()
{
	char *dirx=NULL;
	char *tmp=NULL;
	int x;

	if(mod_list!=NULL) { freeapplib(mod_list); mod_list=NULL; }
	mod_list=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	for(x=0;findfile(dirx,"*.VIR",&tmp,(int)x+1);++x)
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(mod_list,tmp);
	}
	if(mod_list->numlibs<1)
	{
		addAPPlib(mod_list,"Contains No Database Libraries");
	} else {
		SORTAPPlib(mod_list);
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	if(x>=mod_list->numlibs) 
	{
		x=0;
		virmodpointer=0;
	} else {
		virmodpointer=x;
	}
}
static void wdgtloadvirtual(RDArsrc *ersrc)
{
	int x,free_ezviravl=FALSE;
	RDArsrc *lvirrsrc=NULL;

	lvirrsrc=RDArsrcNEW(module,"LOAD EZ VIRTUAL FIELD SCREEN");
	if(mod_list==NULL) makemodlist();
	addlstrsrc(lvirrsrc,"MODULE LIST",&virmodpointer,TRUE,makeezvirlisttest,mod_list->numlibs,&mod_list->libs,NULL);
	x=0;
	if(ezviravl==NULL) /* This has to be initialized to deal with addlstrsrc */ 
	{
		ezviravl=APPlibNEW();
		free_ezviravl=TRUE;
	}
	addlstrsrc(lvirrsrc,"VIRTUALS AVAILABLE",&x,TRUE,changevdesc,ezviravl->numlibs,&ezviravl->libs,NULL);
	
/* free_ezviravl is setting up makeezvirlist when it was created just to setup addlstrsrc otherwise it doesn't create the list each time  */
	if(free_ezviravl) { freeapplib(ezviravl);ezviravl=NULL; }
	makeezvirlist(lvirrsrc);
	addsctrsrc(lvirrsrc,"DESCRIPTION",0,NULL,FALSE);
	changevdesc(lvirrsrc);
	addrfkwrsrc(lvirrsrc,"SELECT",TRUE,oklvir,ersrc);
	addrfkwrsrc(lvirrsrc,"QUIT",TRUE,quitlvir,NULL);
	addbtnrsrc(lvirrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(lvirrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(lvirrsrc,TRUE,quitlvir,NULL,FALSE);
}
static void EditWdgtsscreen(RDArsrc *parent,int selnum,
	char *name,int type,int len,int etype,char *expression,char mgtd,
	char *mod,char *fil,char *subkeyname,int numconfld,RDAeditfld *confld,
	char *mgt_expression,char *assignname,char *id,char *override_expression,
	char setaccttype,APPlib *acctrsc,char setdeftype,APPlib *defrsc,
	char lvirtual,void (*savefunc)(...),void (*quitfunctest)(...))
{
	RDArsrc *meditwgtrsrc=NULL;
	int w=0,x=0,y=0,selmod=0,selfile=0,selfield=0,selidfield=0;
	int btype=0;
	char *curmod=NULL,*temp=NULL;
	NRDfield *fields=NULL,*f;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short nofields=0,nokeys=0,engine=0;
	RDAeditfld *cf1,*cf2;

	meditwgtrsrc=RDArsrcNEW(module,"MAINTAIN ADD EDIT WIDGET");
	addstdrsrc(meditwgtrsrc,"NUMBER",LONGV,8,&selnum,FALSE);
	addbtnrsrc(meditwgtrsrc,"LOAD VIRTUAL",TRUE,wdgtloadvirtual,NULL);
	btype=lvirtual;
	if(btypes==NULL)
	{
		btypes=APPlibNEW();
		addAPPlib(btypes,"N/A");
		addAPPlib(btypes,"Load Virtual");
		addAPPlib(btypes,"Replicate");
		addAPPlib(btypes,"Replicate & Copy");
	}
	addlstrsrc(meditwgtrsrc,"BUILD TYPES",&btype,TRUE,NULL,btypes->numlibs,
		&btypes->libs,NULL);
	addstdrsrc(meditwgtrsrc,"NAME",VARIABLETEXT,0,(name!=NULL?name:""),TRUE);
	addstdrsrc(meditwgtrsrc,"MANAGED",BOOLNS,1,&mgtd,TRUE);
	FINDRSCSETFUNC(meditwgtrsrc,"MANAGED",setmgtflag,NULL);
	getmodulelist();
	if(mod!=NULL)
	{
		for(x=0;x<modlstx->numlibs;++x)
		{
			if(!RDAstrcmp(modlstx->libs[x],mod)) break;
		}
	}
	if(x>=modlstx->numlibs || mod==NULL)
	{
		for(x=0;x<modlstx->numlibs;++x)
		{
			if(!RDAstrcmp(modlstx->libs[x],POWERADD->module)) break;
		}
		if(x>=modlstx->numlibs) x=0;
	}
	addlstrsrc(meditwgtrsrc,"MODULE",&x,TRUE,domodulelist,modlstx->numlibs,
		&modlstx->libs,&ptrue);
	if(filelst!=NULL) freeapplib(filelst);
	filelst=APPlibNEW();
	addlstrsrc(meditwgtrsrc,"FILENAME",&x,TRUE,dofilelist,filelst->numlibs,
		&filelst->libs,&ptrue);
	if(fieldlst!=NULL) freeapplib(fieldlst);
	fieldlst=APPlibNEW();
	addlstrsrc(meditwgtrsrc,"ASSIGN FIELDNAME",&x,TRUE,NULL,fieldlst->numlibs,
		&fieldlst->libs,NULL);
	if(idfieldlst!=NULL) freeapplib(idfieldlst);
	idfieldlst=APPlibNEW();
	addlstrsrc(meditwgtrsrc,"MANAGED FIELDNAME",&x,TRUE,NULL,idfieldlst->numlibs,
		&idfieldlst->libs,NULL);
	readwidget(parent,"MODULE");
	FINDRSCGETSTRING(parent,"MODULE",&curmod);
	if(cnflds!=NULL) freeapplib(cnflds);
	cnflds=APPlibNEW();
	if(confld!=NULL && numconfld)
	{
		holdnum=numconfld;
		holdconfld=Rmalloc(numconfld*sizeof(RDAeditfld));
		for(x=0,cf1=holdconfld,cf2=confld;x<numconfld;++x,++cf1,++cf2)
		{
			if(temp!=NULL)
			{
				temp=Rrealloc(temp,RDAstrlen(cf2->fldname)+RDAstrlen(cf2->conname)+24);
			} else {
				temp=Rmalloc(RDAstrlen(cf2->fldname)+RDAstrlen(cf2->conname)+24);
			}
			cf1->fldname=stralloc(cf2->fldname);
			cf1->conname=stralloc(cf2->conname);
			sprintf(temp,"Field [%s] connects to %s",
				(cf1->fldname!=NULL?cf1->fldname:""),(cf1->conname!=NULL?cf1->conname:""));
			addAPPlib(cnflds,temp);
		}
	}
	if(subkeylist!=NULL) freeapplib(subkeylist);
	subkeylist=APPlibNEW();
	x=0;
	addlstrsrc(meditwgtrsrc,"KEYNAME",&x,TRUE,setconflds,subkeylist->numlibs,
		&subkeylist->libs,NULL);
	selmod=0;
	domodulelist(meditwgtrsrc,&pfalse);
	if(mod!=NULL)
	{
		for(selmod=0;selmod<modlstx->numlibs;++selmod)
		{
			if(!RDAstrcmp(modlstx->libs[selmod],mod)) break;
		}
	}
	if(selmod>=modlstx->numlibs || mod==NULL)
	{
		for(selmod=0;selmod<modlstx->numlibs;++selmod)
		{
			if(!RDAstrcmp(modlstx->libs[selmod],POWERADD->module)) break;
		}
		if(selmod>=modlstx->numlibs) selmod=0;
	}
	dofilelist(meditwgtrsrc,&pfalse);
	selfile=0;
	if(fil!=NULL)
	{
		for(selfile=0;selfile<filelst->numlibs;++selfile)
		{
			if(!RDAstrcmp(filelst->libs[selfile],fil)) break;
		}
	}
	if(selfile>=filelst->numlibs)
	{
		selfile=0;
	}
	if(!FINDRSCLISTAPPlib(meditwgtrsrc,"FILENAME",selfile,filelst))
	{
		updatersrc(meditwgtrsrc,"FILENAME");
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
	if(!FINDRSCLISTAPPlib(meditwgtrsrc,"KEYNAME",x,subkeylist))
	{
		updatersrc(meditwgtrsrc,"KEYNAME");
	}
	if(cnflds->numlibs<1)
	{
		addAPPlib(cnflds,"No Connecting Fields");
		setconflds(meditwgtrsrc);
	}

	dofieldlist(meditwgtrsrc);
	selfield=0;
	if(assignname!=NULL)
	{
		for(selfield=0;selfield<fieldlst->numlibs;++selfield)
		{
			if(!RDAstrcmp(fieldlst->libs[selfield],assignname)) break;
		}
	}
	if(selfield>=fieldlst->numlibs)
	{
		selfield=0;
	}
	if(!FINDRSCLISTAPPlib(meditwgtrsrc,"ASSIGN FIELDNAME",selfield,fieldlst))
	{
		updatersrc(meditwgtrsrc,"ASSIGN FIELDNAME");
	}
	if(id!=NULL)
	{
		for(selidfield=0;selidfield<idfieldlst->numlibs;++selidfield)
		{
			if(!RDAstrcmp(idfieldlst->libs[selidfield],id)) break;
		}
	}
	if(selidfield>=idfieldlst->numlibs)
	{
		selidfield=0;
	}
	if(!FINDRSCLISTAPPlib(meditwgtrsrc,"MANAGED FIELDNAME",selidfield,idfieldlst))
	{
		updatersrc(meditwgtrsrc,"MANAGED FIELDNAME");
	}
/*
	x=0;
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
	addlstrsrc(meditwgtrsrc,"CONNECT TYPES",&dtype,TRUE,NULL,ConTypes->numlibs,&ConTypes->libs,NULL);
*/
	addlstrsrc(meditwgtrsrc,"CONNECT FIELDNAME",&x,TRUE,NULL,cnflds->numlibs,
		&cnflds->libs,NULL);
/*
	addstdrsrc(meditwgtrsrc,"ASSIGN FIELDNAME",VARIABLETEXT,0,
		(assignname!=NULL?assignname:""),TRUE);
*/
	addbtnrsrc(meditwgtrsrc,"SELECT",TRUE,editconnect,parent);
	addsctrsrc(meditwgtrsrc,"MANAGED EXPRESSION",0,
		(mgt_expression!=NULL?mgt_expression:""),TRUE);
	addbtnrsrc(meditwgtrsrc,"MANAGED LOAD VALUE",TRUE,loadvalue,"MANAGED EXPRESSION");
	addbtnrsrc(meditwgtrsrc,"MANAGED LOAD OPERATOR",TRUE,loadoperand,"MANAGED EXPRESSION");
	addbtnrsrc(meditwgtrsrc,"MANAGED LOAD GROUPER",TRUE,loadgrouper,"MANAGED EXPRESSION");
	addbtnrsrc(meditwgtrsrc,"MANAGED LOAD CONTROL",TRUE,loadcontrol,"MANAGED EXPRESSION");
	addsctrsrc(meditwgtrsrc,"OVERRIDE EXPRESSION",0,
		(override_expression!=NULL?override_expression:""),TRUE);
	addbtnrsrc(meditwgtrsrc,"OVERRIDE LOAD VALUE",TRUE,loadvalue,"OVERRIDE EXPRESSION");
	addbtnrsrc(meditwgtrsrc,"OVERRIDE LOAD OPERATOR",TRUE,loadoperand,"OVERRIDE EXPRESSION");
	addbtnrsrc(meditwgtrsrc,"OVERRIDE LOAD GROUPER",TRUE,loadgrouper,"OVERRIDE EXPRESSION");
	addbtnrsrc(meditwgtrsrc,"OVERRIDE LOAD CONTROL",TRUE,loadcontrol,"OVERRIDE EXPRESSION");



	addlstrsrc(meditwgtrsrc,"EXPRESSION TYPES",&etype,TRUE,NULL,ExpTypes->numlibs,&ExpTypes->libs,NULL);
	addlstrsrc(meditwgtrsrc,"FIELD TYPES",&type,TRUE,NULL,fieldtypes->numlibs,&fieldtypes->libs,NULL);
	addstdrsrc(meditwgtrsrc,"FIELD LENGTH",LONGV,5,&len,TRUE);
	addsctrsrc(meditwgtrsrc,"EXPRESSION",0,(expression!=NULL?expression:""),TRUE);
	addbtnrsrc(meditwgtrsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(meditwgtrsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(meditwgtrsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(meditwgtrsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	setmgtflag(meditwgtrsrc);

	addstdrsrc(meditwgtrsrc,"SET ACCOUNT TYPE",BOOLNS,1,&setaccttype,TRUE);
	if(ACCTrsc!=NULL) freeapplib(ACCTrsc);
	ACCTrsc=APPlibNEW();
	if(acctrsc!=NULL)
	{
		for(w=0;w<acctrsc->numlibs;++w)
		{
			addAPPlib(ACCTrsc,acctrsc->libs[w]);
		}
	} else {
		addAPPlib(ACCTrsc,"No Resources"); 
	}
	addlstrsrc(meditwgtrsrc,"ACCOUNT RESOURCES",0,TRUE,NULL,ACCTrsc->numlibs,&ACCTrsc->libs,NULL);
	FINDRSCSETFUNC(meditwgtrsrc,"SET ACCOUNT TYPE",setacctfunc,NULL);
	addbtnrsrc(meditwgtrsrc,"ADD ACCOUNT TYPE ABOVE",TRUE,add_acct_above,NULL);
	addbtnrsrc(meditwgtrsrc,"ADD ACCOUNT TYPE BELOW",TRUE,add_acct_below,NULL);
	addbtnrsrc(meditwgtrsrc,"EDIT ACCOUNT TYPE",TRUE,select_acct,NULL);
	addbtnrsrc(meditwgtrsrc,"DELETE ACCOUNT TYPE",TRUE,delete_acct,NULL);
	addstdrsrc(meditwgtrsrc,"SET DEFINITION TYPE",BOOLNS,1,&setdeftype,TRUE);
	if(DEFrsc!=NULL) freeapplib(DEFrsc);
	DEFrsc=APPlibNEW();
	if(defrsc!=NULL)
	{
		for(w=0;w<defrsc->numlibs;++w)
		{
			addAPPlib(DEFrsc,defrsc->libs[w]);
		}
	} else {
		addAPPlib(DEFrsc,"No Resources");
	}
	FINDRSCSETFUNC(meditwgtrsrc,"SET DEFINITION TYPE",setdeffunc,NULL);
	addlstrsrc(meditwgtrsrc,"DEFINITION TYPE RESOURCES",0,TRUE,NULL,DEFrsc->numlibs,&DEFrsc->libs,NULL);
	addbtnrsrc(meditwgtrsrc,"ADD DEFINITION TYPE ABOVE",TRUE,add_deftype_above,NULL);
	addbtnrsrc(meditwgtrsrc,"ADD DEFINITION TYPE BELOW",TRUE,add_deftype_below,NULL);
	addbtnrsrc(meditwgtrsrc,"EDIT DEFINITION TYPE",TRUE,select_deftype,NULL);
	addbtnrsrc(meditwgtrsrc,"DELETE DEFINITION TYPE",TRUE,delete_deftype,NULL);

	setacctfunc(meditwgtrsrc);
	setdeffunc(meditwgtrsrc);
	addrfcbrsrc(meditwgtrsrc,"SAVE",TRUE,savefunc,parent);
	addrfcbrsrc(meditwgtrsrc,"QUIT",TRUE,quitfunctest,parent);
	addbtnrsrc(meditwgtrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(meditwgtrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	msubsdefaults=GetDefaults(meditwgtrsrc);
	changedEditWdgtss=FALSE;
	APPmakescrn(meditwgtrsrc,TRUE,quitmsubo,parent,FALSE);
}
static void quitmsubotest(RDArsrc *meditwgtrsrc,RDArsrc *subrsrc)
{
	readallwidgets(meditwgtrsrc);
	if(COMPARE_RSRCS(meditwgtrsrc,msubsdefaults,2) || changedEditWdgtss)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","MASTER CHANGES NOT SAVED!","Changes have been made to this SUPPORTING FILE DEFININTION!\nDo you want to Save these changes?",savemsubo,quitmsubo,FALSE,2,meditwgtrsrc,subrsrc,NULL);
	} else {
		quitmsubo(meditwgtrsrc,subrsrc);
	}
}
static void quitmsuboabovetest(RDArsrc *meditwgtrsrc,RDArsrc *subrsrc)
{
	readallwidgets(meditwgtrsrc);
	if(COMPARE_RSRCS(meditwgtrsrc,msubsdefaults,2) || changedEditWdgtss)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","MASTER CHANGES NOT SAVED!","Changes have been made to this SURORDINATE FILE DEFININTION!\nDo you want to Save these changes?",savemsuboabove,quitmsubo,FALSE,2,meditwgtrsrc,subrsrc,NULL);
	} else {
		quitmsubo(meditwgtrsrc,subrsrc);
	}
}
static void quitmsubobelowtest(RDArsrc *meditwgtrsrc,RDArsrc *subrsrc)
{
	readallwidgets(meditwgtrsrc);
	if(COMPARE_RSRCS(meditwgtrsrc,msubsdefaults,2) || changedEditWdgtss)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","MASTER CHANGES NOT SAVED!","Changes have been made to this SUPPORTING FILE DEFININTION!\nDo you want to Save these changes?",savemsubobelow,quitmsubo,FALSE,2,meditwgtrsrc,subrsrc,NULL);
	} else {
		quitmsubo(meditwgtrsrc,subrsrc);
	}
}
void wdgtaddabove(RDArsrc *subrsrc,RDArsrc *mtnrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(subrsrc,"EDIT WIDGETS",&selected)) return;
	if(selected==0 && !RDAstrcmp(wdgtavl->libs[selected],"No Edit Widgets Defined"))
		return;
	EditWdgtsscreen(subrsrc,++selected,NULL,0,0,0,
		NULL,FALSE,NULL,NULL,NULL,0,NULL,NULL,NULL,NULL,NULL,
		FALSE,NULL,FALSE,NULL,FALSE,
		savemsuboabove,quitmsuboabovetest);
}
void wdgtaddbelow(RDArsrc *subrsrc,RDArsrc *mtnrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(subrsrc,"EDIT WIDGETS",&selected)) return;
	if(selected==0 && !RDAstrcmp(wdgtavl->libs[selected],"No Edit Widgets Defined"))
		return;
	selected+=2;
	EditWdgtsscreen(subrsrc,selected,NULL,0,0,0,
		NULL,FALSE,NULL,NULL,NULL,0,NULL,NULL,NULL,NULL,NULL,
		FALSE,NULL,FALSE,NULL,FALSE,
		savemsubobelow,quitmsubobelowtest);
}
void wdgtmove(RDArsrc *subrsrc,short direction)
{
	/* A direction of 0 means up, 1 means down */
	short z;
	AddEditWdgt *temp,*temp1,*temp2;
	int value,w;
	RDAeditfld *confld=NULL,*confld1=NULL;

	readallwidgets(subrsrc);
	if(FINDRSCGETINT(subrsrc,"EDIT WIDGETS",&value)) return;
	if(tmpnumEditWdgts<1) return;
	temp=tempEditWdgts+value;
	if(direction)
	{
		if((temp==NULL) || (value >= (tmpnumEditWdgts-1))) return;
		value++;
	} else {
		if((temp==NULL) || (value <=0)) return;
		value--;
	}
	temp2=tempEditWdgts+value;
	temp1=Rmalloc(sizeof(AddEditWdgt));

	/* STEP 1 */
	if(temp->name!=NULL)
	{
		temp1->name=stralloc(temp->name);
		Rfree(temp->name);
	} else {
		temp1->name=NULL;
	}
	temp1->type=temp->type;
	temp1->len=temp->len;
	temp1->etype=temp->etype;
	temp1->id_mgt=temp->id_mgt;
	temp1->setaccttype=temp->setaccttype;
	temp1->loadvirtual=temp->loadvirtual;
	temp1->setdeftype=temp->setdeftype;
	if(temp->acctrsc!=NULL)
	{
		temp1->acctrsc=APPlibNEW();
		for(w=0;w<temp->acctrsc->numlibs;++w)
		{
			addAPPlib(temp1->acctrsc,temp->acctrsc->libs[w]);
		}
		freeapplib(temp->acctrsc);
	} else temp1->acctrsc=NULL;
	if(temp->defrsc!=NULL)
	{
		temp1->defrsc=APPlibNEW();
		for(w=0;w<temp->defrsc->numlibs;++w)
		{
			addAPPlib(temp1->defrsc,temp->defrsc->libs[w]);
		}
		freeapplib(temp->defrsc);
	} else temp1->defrsc=NULL;
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
	if(temp->confld!=NULL)
	{
		temp1->num=temp->num;
		temp1->confld=Rmalloc(sizeof(RDAeditfld)*temp->num);
		for(z=0,confld1=temp1->confld,confld=temp->confld;z<temp->num;++z,++confld,++confld1)
		{
			if(confld->fldname!=NULL)
			{
				confld1->fldname=stralloc(confld->fldname);
				Rfree(confld->fldname);
			} else confld1->fldname=NULL;
			if(confld->conname!=NULL)
			{
				confld1->conname=stralloc(confld->conname);
				Rfree(confld->conname);
			} else confld1->conname=NULL;
		}
		Rfree(temp->confld);
	} else {
		temp1->num=0;
	}
	if(temp->mgt_expression!=NULL)
	{
		temp1->mgt_expression=stralloc(temp->mgt_expression);
		Rfree(temp->mgt_expression);
	} else temp1->mgt_expression=NULL;
	if(temp->assignname!=NULL)
	{
		temp1->assignname=stralloc(temp->assignname);
		Rfree(temp->assignname);
	} else temp1->assignname=NULL;
	if(temp->id!=NULL)
	{
		temp1->id=stralloc(temp->id);
		Rfree(temp->id);
	} else temp1->id=NULL;
	if(temp->override_expression!=NULL)
	{
		temp1->override_expression=stralloc(temp->override_expression);
		Rfree(temp->override_expression);
	} else temp1->override_expression=NULL;
	temp1->fileno=temp->fileno;
	if(temp->expression!=NULL)
	{
		temp1->expression=stralloc(temp->expression);
		Rfree(temp->expression);
	} else temp1->expression=NULL;
	temp1->loadvirtual=temp->loadvirtual;

	/* STEP 2 */
	if(temp2->name!=NULL)
	{
		temp->name=stralloc(temp2->name);
		Rfree(temp2->name);
	} else {
		temp->name=NULL;
	}
	temp->type=temp2->type;
	temp->len=temp2->len;
	temp->etype=temp2->etype;
	temp->id_mgt=temp2->id_mgt;
	temp->setaccttype=temp2->setaccttype;
	temp->loadvirtual=temp2->loadvirtual;
	temp->setdeftype=temp2->setdeftype;
	if(temp2->acctrsc!=NULL)
	{
		temp->acctrsc=APPlibNEW();
		for(w=0;w<temp2->acctrsc->numlibs;++w)
		{
			addAPPlib(temp->acctrsc,temp2->acctrsc->libs[w]);
		}
		freeapplib(temp2->acctrsc);
	} else temp->acctrsc=NULL;
	if(temp2->defrsc!=NULL)
	{
		temp->defrsc=APPlibNEW();
		for(w=0;w<temp2->defrsc->numlibs;++w)
		{
			addAPPlib(temp->defrsc,temp2->defrsc->libs[w]);
		}
		freeapplib(temp2->defrsc);
	} else temp->defrsc=NULL;
	if(temp2->module!=NULL)
	{
		temp->module=stralloc(temp2->module);
		Rfree(temp2->module);
	} else temp->module=NULL;
	if(temp2->filename!=NULL)
	{
		temp->filename=stralloc(temp2->filename);
		Rfree(temp2->filename);
	} else temp->filename=NULL;
	if(temp2->subkeyname!=NULL)
	{
		temp->subkeyname=stralloc(temp2->subkeyname);
		Rfree(temp2->subkeyname);
	} else temp->subkeyname=NULL;
	if(temp2->confld!=NULL)
	{
		temp->num=temp2->num;
		temp->confld=Rmalloc(sizeof(RDAeditfld)*temp2->num);
		for(z=0,confld1=temp->confld,confld=temp2->confld;z<temp2->num;++z,++confld,++confld1)
		{
			if(confld->fldname!=NULL)
			{
				confld1->fldname=stralloc(confld->fldname);
				Rfree(confld->fldname);
			} else confld1->fldname=NULL;
			if(confld->conname!=NULL)
			{
				confld1->conname=stralloc(confld->conname);
				Rfree(confld->conname);
			} else confld1->conname=NULL;
		}
		Rfree(temp2->confld);
	} else {
		temp->num=0;
	}
	if(temp2->mgt_expression!=NULL)
	{
		temp->mgt_expression=stralloc(temp2->mgt_expression);
		Rfree(temp2->mgt_expression);
	} else temp->mgt_expression=NULL;
	if(temp2->assignname!=NULL)
	{
		temp->assignname=stralloc(temp2->assignname);
		Rfree(temp2->assignname);
	} else temp->assignname=NULL;
	if(temp2->id!=NULL)
	{
		temp->id=stralloc(temp2->id);
		Rfree(temp2->id);
	} else temp->id=NULL;
	if(temp2->override_expression!=NULL)
	{
		temp->override_expression=stralloc(temp2->override_expression);
		Rfree(temp2->override_expression);
	} else temp->override_expression=NULL;
	temp->fileno=temp2->fileno;
	if(temp2->expression!=NULL)
	{
		temp->expression=stralloc(temp2->expression);
		Rfree(temp2->expression);
	} else temp->expression=NULL;
	temp->loadvirtual=temp2->loadvirtual;

	/* STEP 3 */
	if(temp1->name!=NULL)
	{
		temp2->name=stralloc(temp1->name);
		Rfree(temp1->name);
	} else {
		temp2->name=NULL;
	}
	temp2->type=temp1->type;
	temp2->len=temp1->len;
	temp2->etype=temp1->etype;
	temp2->id_mgt=temp1->id_mgt;
	temp2->setaccttype=temp1->setaccttype;
	temp2->loadvirtual=temp1->loadvirtual;
	temp2->setdeftype=temp1->setdeftype;
	if(temp1->acctrsc!=NULL)
	{
		temp2->acctrsc=APPlibNEW();
		for(w=0;w<temp1->acctrsc->numlibs;++w)
		{
			addAPPlib(temp2->acctrsc,temp1->acctrsc->libs[w]);
		}
		freeapplib(temp1->acctrsc);
	} else temp2->acctrsc=NULL;
	if(temp1->defrsc!=NULL)
	{
		temp2->defrsc=APPlibNEW();
		for(w=0;w<temp1->defrsc->numlibs;++w)
		{
			addAPPlib(temp2->defrsc,temp1->defrsc->libs[w]);
		}
		freeapplib(temp1->defrsc);
	} else temp2->defrsc=NULL;
	if(temp1->module!=NULL)
	{
		temp2->module=stralloc(temp1->module);
		Rfree(temp1->module);
	} else temp2->module=NULL;
	if(temp1->filename!=NULL)
	{
		temp2->filename=stralloc(temp1->filename);
		Rfree(temp1->filename);
	} else temp2->filename=NULL;
	if(temp1->subkeyname!=NULL)
	{
		temp2->subkeyname=stralloc(temp1->subkeyname);
		Rfree(temp1->subkeyname);
	} else temp2->subkeyname=NULL;
	if(temp1->confld!=NULL)
	{
		temp2->num=temp1->num;
		temp2->confld=Rmalloc(sizeof(RDAeditfld)*temp1->num);
		for(z=0,confld1=temp2->confld,confld=temp1->confld;z<temp1->num;++z,++confld,++confld1)
		{
			if(confld->fldname!=NULL)
			{
				confld1->fldname=stralloc(confld->fldname);
				Rfree(confld->fldname);
			} else confld1->fldname=NULL;
			if(confld->conname!=NULL)
			{
				confld1->conname=stralloc(confld->conname);
				Rfree(confld->conname);
			} else confld1->conname=NULL;
		}
		Rfree(temp1->confld);
	} else {
		temp2->num=0;
	}
	if(temp1->mgt_expression!=NULL)
	{
		temp2->mgt_expression=stralloc(temp1->mgt_expression);
		Rfree(temp1->mgt_expression);
	} else temp2->mgt_expression=NULL;
	if(temp1->assignname!=NULL)
	{
		temp2->assignname=stralloc(temp1->assignname);
		Rfree(temp1->assignname);
	} else temp2->assignname=NULL;
	if(temp1->id!=NULL)
	{
		temp2->id=stralloc(temp1->id);
		Rfree(temp1->id);
	} else temp2->id=NULL;
	if(temp1->override_expression!=NULL)
	{
		temp2->override_expression=stralloc(temp1->override_expression);
		Rfree(temp1->override_expression);
	} else temp2->override_expression=NULL;
	temp2->fileno=temp1->fileno;
	if(temp1->expression!=NULL)
	{
		temp2->expression=stralloc(temp1->expression);
		Rfree(temp1->expression);
	} else temp2->expression=NULL;
	temp2->loadvirtual=temp1->loadvirtual;

	Rfree(temp1);
	getWidgets();
	if(value>=wdgtavl->numlibs) value=(wdgtavl->numlibs-1);
	if(!FINDRSCLISTAPPlib(subrsrc,"EDIT WIDGETS",value,wdgtavl))
		updatersrc(subrsrc,"EDIT WIDGETS");
	changedEditWdgtss=TRUE;
}
void wdgtmoveup(RDArsrc *subrsrc)
{
	wdgtmove(subrsrc,0);
}
void wdgtmovedown(RDArsrc *subrsrc)
{
	wdgtmove(subrsrc,1);
}
void wdgtdelete(RDArsrc *subrsrc)
{
	short x,y,z;
	AddEditWdgt *temp,*temp1,*new_fields;
	int value,w;
	RDAeditfld *confld=NULL,*confld1=NULL;

	readallwidgets(subrsrc);
	if(FINDRSCGETINT(subrsrc,"EDIT WIDGETS",&value)) return;
	if(tmpnumEditWdgts<1) return;
	if(tmpnumEditWdgts>1)
	{
		new_fields=Rmalloc((tmpnumEditWdgts-1)*sizeof(AddEditWdgt));
		y=0;
		for(x=0,temp=tempEditWdgts;x<tmpnumEditWdgts;++x,++temp)
		{
			temp1=new_fields+y;
			if(x!=value)
			{
				if(temp->name!=NULL)
				{
					temp1->name=stralloc(temp->name);
					Rfree(temp->name);
				} else {
					temp1->name=NULL;
				}
				temp1->type=temp->type;
				temp1->len=temp->len;
				temp1->etype=temp->etype;
				temp1->id_mgt=temp->id_mgt;
				temp1->setaccttype=temp->setaccttype;
				temp1->loadvirtual=temp->loadvirtual;
				temp1->setdeftype=temp->setdeftype;
				if(temp->acctrsc!=NULL)
				{
					temp1->acctrsc=APPlibNEW();
					for(w=0;w<temp->acctrsc->numlibs;++w)
					{
						addAPPlib(temp1->acctrsc,temp->acctrsc->libs[w]);
					}
					freeapplib(temp->acctrsc);
				} else temp1->acctrsc=NULL;
				if(temp->defrsc!=NULL)
				{
					temp1->defrsc=APPlibNEW();
					for(w=0;w<temp->defrsc->numlibs;++w)
					{
						addAPPlib(temp1->defrsc,temp->defrsc->libs[w]);
					}
					freeapplib(temp->defrsc);
				} else temp1->defrsc=NULL;
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
				if(temp->confld!=NULL)
				{
					temp1->num=temp->num;
					temp1->confld=Rmalloc(sizeof(RDAeditfld)*temp->num);
					for(z=0,confld1=temp1->confld,confld=temp->confld;z<temp->num;++z,++confld,++confld1)
					{
						if(confld->fldname!=NULL)
						{
							confld1->fldname=stralloc(confld->fldname);
							Rfree(confld->fldname);
						} else confld1->fldname=NULL;
						if(confld->conname!=NULL)
						{
							confld1->conname=stralloc(confld->conname);
							Rfree(confld->conname);
						} else confld1->conname=NULL;
					}
					Rfree(temp->confld);
				} else {
					temp1->num=0;
				}
				if(temp->mgt_expression!=NULL)
				{
					temp1->mgt_expression=stralloc(temp->mgt_expression);
					Rfree(temp->mgt_expression);
				} else temp1->mgt_expression=NULL;
				if(temp->assignname!=NULL)
				{
					temp1->assignname=stralloc(temp->assignname);
					Rfree(temp->assignname);
				} else temp1->assignname=NULL;
				if(temp->id!=NULL)
				{
					temp1->id=stralloc(temp->id);
					Rfree(temp->id);
				} else temp1->id=NULL;
				if(temp->override_expression!=NULL)
				{
					temp1->override_expression=stralloc(temp->override_expression);
					Rfree(temp->override_expression);
				} else temp1->override_expression=NULL;
				temp1->fileno=temp->fileno;
				if(temp->expression!=NULL)
				{
					temp1->expression=stralloc(temp->expression);
					Rfree(temp->expression);
				} else temp1->expression=NULL;
				temp1->loadvirtual=temp->loadvirtual;
				++y;
			} else {
				if(temp->name!=NULL) Rfree(temp->name);
				if(temp->expression!=NULL) 
					Rfree(temp->expression);
				if(temp->module!=NULL) Rfree(temp->module);
				if(temp->filename!=NULL) Rfree(temp->filename);
				if(temp->subkeyname!=NULL) Rfree(temp->subkeyname);
				if(temp->acctrsc!=NULL) freeapplib(temp->acctrsc);
				if(temp->defrsc!=NULL) freeapplib(temp->defrsc);
				if(temp->confld!=NULL)
				{
					for(z=0,confld=temp->confld;z<temp->num;++z,++confld)
					{
						if(confld->fldname!=NULL) Rfree(confld->fldname);
						if(confld->conname!=NULL) Rfree(confld->conname);
					}
					Rfree(temp->confld);
				}
				temp->num=0;
				if(temp->mgt_expression!=NULL) Rfree(temp->mgt_expression);
				if(temp->assignname!=NULL) Rfree(temp->assignname);
				if(temp->id!=NULL) Rfree(temp->id);
				if(temp->override_expression!=NULL) Rfree(temp->override_expression);
			}	
		}
		Rfree(tempEditWdgts);
		--tmpnumEditWdgts;
		tempEditWdgts=Rmalloc(tmpnumEditWdgts*sizeof(AddEditWdgt));
		for(x=0,temp1=tempEditWdgts,temp=new_fields;x<tmpnumEditWdgts;++x,
			++temp,++temp1)
		{
			if(temp->name!=NULL)
			{
				temp1->name=stralloc(temp->name);
				Rfree(temp->name);
			} else temp1->name=NULL;
			temp1->type=temp->type;
			temp1->len=temp->len;
			temp1->etype=temp->etype;
			temp1->id_mgt=temp->id_mgt;
			temp1->setaccttype=temp->setaccttype;
			temp1->loadvirtual=temp->loadvirtual;
			temp1->setdeftype=temp->setdeftype;
			if(temp->acctrsc!=NULL)
			{
				temp1->acctrsc=APPlibNEW();
				for(w=0;w<temp->acctrsc->numlibs;++w)
				{
					addAPPlib(temp1->acctrsc,temp->acctrsc->libs[w]);
				}
				freeapplib(temp->acctrsc);
			} else temp1->acctrsc=NULL;
			if(temp->defrsc!=NULL)
			{
				temp1->defrsc=APPlibNEW();
				for(w=0;w<temp->defrsc->numlibs;++w)
				{
					addAPPlib(temp1->defrsc,temp->defrsc->libs[w]);
				}
				freeapplib(temp->defrsc);
			} else temp1->defrsc=NULL;
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
			if(temp->confld!=NULL)
			{
				temp1->num=temp->num;
				temp1->confld=Rmalloc(sizeof(RDAeditfld)*temp->num);
				for(z=0,confld1=temp1->confld,confld=temp->confld;z<temp->num;++z,++confld,++confld1)
				{
					if(confld->fldname!=NULL)
					{
						confld1->fldname=stralloc(confld->fldname);
						Rfree(confld->fldname);
					} else confld1->fldname=NULL;
					if(confld->conname!=NULL)
					{
						confld1->conname=stralloc(confld->conname);
						Rfree(confld->conname);
					} else confld1->conname=NULL;
				}
				Rfree(temp->confld);
			} else {
				temp->num=0;
			}
			if(temp->mgt_expression!=NULL)
			{
				temp1->mgt_expression=stralloc(temp->mgt_expression);
				Rfree(temp->mgt_expression);
			} else temp1->mgt_expression=NULL;
			if(temp->assignname!=NULL)
			{
				temp1->assignname=stralloc(temp->assignname);
				Rfree(temp->assignname);
			} else temp1->assignname=NULL;
			if(temp->id!=NULL)
			{
				temp1->id=stralloc(temp->id);
				Rfree(temp->id);
			} else temp1->id=NULL;
			if(temp->override_expression!=NULL)
			{
				temp1->override_expression=stralloc(temp->override_expression);
				Rfree(temp->override_expression);
			} else temp1->override_expression=NULL;
			temp1->fileno=temp->fileno;
			if(temp->expression!=NULL)
			{
				temp1->expression=stralloc(temp->expression);
				Rfree(temp->expression);
			} else temp1->expression=NULL;
			temp1->loadvirtual=temp->loadvirtual;
		}
		Rfree(new_fields);
	} else if(tempEditWdgts!=NULL)
	{
		for(x=0,temp=tempEditWdgts;x<tmpnumEditWdgts;++x,++temp)
		{
			if(temp->name!=NULL) Rfree(temp->name);
			if(temp->expression!=NULL) Rfree(temp->expression);
			if(temp->module!=NULL) Rfree(temp->module);
			if(temp->filename!=NULL) Rfree(temp->filename);
			if(temp->subkeyname!=NULL) Rfree(temp->subkeyname);
			if(temp->acctrsc!=NULL) freeapplib(temp->acctrsc);
			if(temp->defrsc!=NULL) freeapplib(temp->defrsc);
			if(temp->confld!=NULL)
			{
				for(z=0,confld=temp->confld;z<temp->num;++z,++confld)
				{
					if(confld->fldname!=NULL) Rfree(confld->fldname);
					if(confld->conname!=NULL) Rfree(confld->conname);
				}
				Rfree(temp->confld);
			}
			temp->num=0;
			if(temp->id!=NULL) Rfree(temp->id);
			if(temp->assignname!=NULL) Rfree(temp->assignname);
			if(temp->mgt_expression!=NULL) Rfree(temp->mgt_expression);
			if(temp->override_expression!=NULL) Rfree(temp->override_expression);
		}
		Rfree(tempEditWdgts);
		tmpnumEditWdgts=0;
	}
	getWidgets();
	if(value>=wdgtavl->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(subrsrc,"EDIT WIDGETS",value,wdgtavl))
		updatersrc(subrsrc,"EDIT WIDGETS");
	changedEditWdgtss=TRUE;
}
void wdgtcopy(RDArsrc *mtnrsrc)
{
	int selected=0;
	AddEditWdgt *subs=NULL;

	if(FINDRSCGETINT(mtnrsrc,"EDIT WIDGETS",&selected)) return;
	if(selected==0 && !RDAstrcmp(wdgtavl->libs[selected],"No Edit Widgets Defined")) return;
	if(tempEditWdgts==NULL) return;
	subs=tempEditWdgts+selected;
	selected+=2;
	if(subs!=NULL)
	{
		EditWdgtsscreen(mtnrsrc,selected,subs->name,subs->type,
			subs->len,subs->etype,subs->expression,
			subs->id_mgt,subs->module,subs->filename,
			subs->subkeyname,subs->num,subs->confld,
			subs->mgt_expression,subs->assignname,subs->id,
			subs->override_expression,subs->setaccttype,
			subs->acctrsc,subs->setdeftype,subs->defrsc,
			subs->loadvirtual,savemsubobelow,quitmsubobelowtest);
	} else {
		EditWdgtsscreen(mtnrsrc,selected,NULL,0,0,0,
			NULL,FALSE,NULL,NULL,NULL,0,NULL,NULL,NULL,NULL,NULL,
			FALSE,NULL,FALSE,NULL,FALSE,
			savemsubobelow,quitmsubobelowtest);
	}
}
void wdgtedit(RDArsrc *subrsrc,RDArsrc *mtnrsrc)
{
	int selected=0;
	AddEditWdgt *subs=NULL;

	if(FINDRSCGETINT(subrsrc,"EDIT WIDGETS",&selected)) return;
	if(tempEditWdgts==NULL)
	{
		tempEditWdgts=Rmalloc(sizeof(AddEditWdgt));
		subs=tempEditWdgts;
		subs->name=NULL;
		subs->type=0;
		subs->len=0;
		subs->etype=0;	
		subs->expression=NULL;
		subs->id_mgt=FALSE;
		subs->module=NULL;
		subs->filename=NULL;
		subs->fileno=(-1);
		subs->setaccttype=FALSE;
		subs->setdeftype=FALSE;
		subs->defrsc=NULL;
		subs->acctrsc=NULL;
		subs->loadvirtual=FALSE;
		selected=0;
		tmpnumEditWdgts=1;
	}
	subs=tempEditWdgts+selected;
	if(subs!=NULL)
	{
		EditWdgtsscreen(subrsrc,++selected,subs->name,subs->type,subs->len,subs->etype,subs->expression,
		subs->id_mgt,subs->module,subs->filename,subs->subkeyname,
		subs->num,subs->confld,
		subs->mgt_expression,subs->assignname,subs->id,
		subs->override_expression,subs->setaccttype,subs->acctrsc,
		subs->setdeftype,subs->defrsc,subs->loadvirtual,savemsubo,quitmsubotest);
	} else {
		EditWdgtsscreen(subrsrc,++selected,NULL,0,0,0,
			NULL,FALSE,NULL,NULL,NULL,0,NULL,NULL,NULL,NULL,NULL,
			FALSE,NULL,FALSE,NULL,FALSE,
			savemsubo,quitmsubotest);
	}
}
void savewdgt(void)
{
	int x=0,z=0,y=0;
	AddEditWdgt *temp1,*temp;
	RDAeditfld *confld1,*confld2;

	if(POWERADD->EditWdgts!=NULL)
	{
		for(x=0,temp=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++temp)
		{
			if(temp->name!=NULL) Rfree(temp->name);
			if(temp->expression!=NULL) Rfree(temp->expression);
			if(temp->module!=NULL) Rfree(temp->module);
			if(temp->filename!=NULL) Rfree(temp->filename);
			if(temp->subkeyname!=NULL) Rfree(temp->subkeyname);
			if(temp->acctrsc!=NULL) freeapplib(temp->acctrsc);
			if(temp->defrsc!=NULL) freeapplib(temp->defrsc);
			if(temp->confld!=NULL)
			{
				for(z=0,confld1=temp->confld;z<temp->num;++z,++confld1)
				{
					if(confld1->fldname!=NULL) Rfree(confld1->fldname);
					if(confld1->conname!=NULL) Rfree(confld1->conname);
				}
				Rfree(temp->confld);
			}
			temp->num=0;
			if(temp->assignname!=NULL) Rfree(temp->assignname);
			if(temp->mgt_expression!=NULL) Rfree(temp->mgt_expression);
			if(temp->id!=NULL) Rfree(temp->id);
			if(temp->override_expression!=NULL) Rfree(temp->override_expression);
		}
		Rfree(POWERADD->EditWdgts);
		POWERADD->EditWdgts=NULL;
		POWERADD->numwdgts=0;
	}
	if(tempEditWdgts!=NULL)
	{
		POWERADD->numwdgts=tmpnumEditWdgts;
		POWERADD->EditWdgts=Rmalloc(tmpnumEditWdgts*sizeof(AddEditWdgt));
		for(x=0,temp=tempEditWdgts,temp1=POWERADD->EditWdgts;x<tmpnumEditWdgts;++x,++temp,
			++temp1)
		{
			if(temp->name!=NULL)
			{
				temp1->name=stralloc(temp->name);
				Rfree(temp->name);
			} else temp1->name=NULL;
			temp1->type=temp->type;
			temp1->len=temp->len;
			temp1->loadvirtual=temp->loadvirtual;
			temp1->etype=temp->etype;
			temp1->id_mgt=temp->id_mgt;
			temp1->setaccttype=temp->setaccttype;
			temp1->setdeftype=temp->setdeftype;
			temp1->loadvirtual=temp->loadvirtual;
			if(temp->acctrsc!=NULL)
			{
				temp1->acctrsc=APPlibNEW();
				for(y=0;y<temp->acctrsc->numlibs;++y)
				{
					addAPPlib(temp1->acctrsc,temp->acctrsc->libs[y]);
				}
				freeapplib(temp->acctrsc);
			} else temp1->acctrsc=NULL;
			if(temp->defrsc!=NULL)
			{
				temp1->defrsc=APPlibNEW();
				for(y=0;y<temp->defrsc->numlibs;++y)
				{
					addAPPlib(temp1->defrsc,temp->defrsc->libs[y]);
				}
				freeapplib(temp->defrsc);
			} else temp1->defrsc=NULL;
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
			if(temp->confld!=NULL)
			{
				temp1->num=temp->num;
				temp1->confld=Rmalloc(temp->num*sizeof(RDAeditfld));
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
			} else {
				temp1->num=0;
			}
			if(temp->assignname!=NULL)
			{
				temp1->assignname=stralloc(temp->assignname);
				Rfree(temp->assignname);
			} else temp1->assignname=NULL;
			if(temp->id!=NULL)
			{
				temp1->id=stralloc(temp->id);
				Rfree(temp->id);
			} else temp1->id=NULL;
			if(temp->mgt_expression!=NULL)
			{
				temp1->mgt_expression=stralloc(temp->mgt_expression);
				Rfree(temp->mgt_expression);
			} else temp1->mgt_expression=NULL;
			if(temp->override_expression!=NULL)
			{
				temp1->override_expression=stralloc(temp->override_expression);
				Rfree(temp->override_expression);
			} else temp1->override_expression=NULL;
			temp1->fileno=temp->fileno;
			if(temp->expression!=NULL)
			{
				temp1->expression=stralloc(temp->expression);
				Rfree(temp->expression);
			} else temp1->expression=NULL;
			temp1->loadvirtual=temp->loadvirtual;
		}
		Rfree(tempEditWdgts);
		tempEditWdgts=NULL;
		tmpnumEditWdgts=0;
	}
}
void SEEEditWdgts(PowerAdd *POWERADD)
{
	RDAeditfld *confld=NULL;
	int x=0,z=0;
	AddEditWdgt *EditWdgt=NULL;
	
	for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++EditWdgt)
	{
		prterr("EditWdgt->name=[%s], Type=[%d], Length=[%d], E_Type=[%d], ID_MANAGED=[%s] LoadVirtual [%d]\n",
			(EditWdgt->name!=NULL?EditWdgt->name:""),
			EditWdgt->type,EditWdgt->len,EditWdgt->etype,(EditWdgt->id_mgt==TRUE?"TRUE":"FALSE"),EditWdgt->loadvirtual);
		prterr("\tExpression=[%s]\n",
			(EditWdgt->expression!=NULL?EditWdgt->expression:""));
		if(EditWdgt->id_mgt==TRUE)
		{
			prterr("\tManaged Module=[%s], Managed File=[%s], Managed Key=[%s]\n",
				(EditWdgt->module!=NULL?EditWdgt->module:""),
				(EditWdgt->filename!=NULL?EditWdgt->filename:""),
				(EditWdgt->subkeyname!=NULL?EditWdgt->subkeyname:""));
			prterr("\tAssign Name=[%s], Managed ID=[%s]\n",
				(EditWdgt->assignname!=NULL?EditWdgt->assignname:""),
				(EditWdgt->id!=NULL?EditWdgt->id:""));
			if(EditWdgt->confld!=NULL)
			{
				for(z=0,confld=EditWdgt->confld;z<EditWdgt->num;++z,++confld)
				{
					prterr("\t\tField Name=[%s], Connect Field=[%s]\n",
						(confld->fldname!=NULL?confld->fldname:""),
						(confld->conname!=NULL?confld->conname:""));
				}
			}
			prterr("\tManaged Expression=[%s]\n",
				(EditWdgt->mgt_expression!=NULL?EditWdgt->mgt_expression:""));
			prterr("\t Override Expression=[%s]\n",
				(EditWdgt->override_expression!=NULL?EditWdgt->override_expression:""));
			prterr("acctrsc ");SEEAPPlib(EditWdgt->acctrsc);TRACE;
			prterr("defrsc ");SEEAPPlib(EditWdgt->defrsc);TRACE;
		}
	}
}
static int dofilelist3(RDArsrc *incrsrc,char *modname,char *filename)
{
	int x=0,selectedf=0;
	char *libx=NULL;

	if(filesavl!=NULL) freeapplib(filesavl); 
	if(modname!=NULL)
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modname)+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modname);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modname);
#endif
	} else {
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modsavl2->libs[0])+10);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modsavl2->libs[0]);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modsavl2->libs[0]);
#endif
	}
	filesavl=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	if(filesavl==NULL)
	{
		filesavl=APPlibNEW();
		addAPPlib(filesavl,"No Database Files");
	}
	if(filename!=NULL)
	{
		if((x=FINDAPPLIBELEMENT(filesavl,filename))==(-1))
		{
			selectedf=0;
		} else {
			selectedf=x;
		}
	} else {
		selectedf=0;
	}
	if(incrsrc!=NULL)
	{
		if(!FINDRSCLISTAPPlib(incrsrc,"FILE LIST",selectedf,filesavl))
		{
			updatersrc(incrsrc,"FILE LIST");
		}
	}
	return(selectedf);
}
static void dofilelistcb3(RDArsrc *incrsrc)
{
	int selected=0,selectedf=0;
	char *temp=NULL,*temp2=NULL;

	if(FINDRSCGETINT(incrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETINT(incrsrc,"FILE LIST",&selectedf)) return; 
	temp=stralloc(modsavl2->libs[selected]);
	temp2=stralloc(filesavl->libs[selectedf]);
	dofilelist3(incrsrc,temp,temp2);
	if(temp!=NULL) Rfree(temp);
	if(temp2!=NULL) Rfree(temp2);
}
static int getfilemodules3(char *modname)
{
	int x,selectedm=0;
	char *tmp=NULL,*dirx=NULL;
	
	if(modsavl2!=NULL) freeapplib(modsavl2); 
	modsavl2=APPlibNEW();
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
		addAPPlib(modsavl2,tmp);
	}
	if(modsavl2->numlibs<1)
	{
		addAPPlib(modsavl2,"No Database Files");
		selectedm=0;
	} else {
		SORTAPPlib(modsavl2);
		if(selectedm>=modsavl2->numlibs) selectedm=0;
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	if(modname!=NULL)
	{
		if((x=FINDAPPLIBELEMENT(modsavl2,modname))==(-1))
		{
			selectedm=0;
		} else {
			selectedm=x;
		}
	} else {
		selectedm=0;
	}
	return(selectedm);
}
void quitwizard(RDArsrc *incrsrc)
{
	if(modsavl2!=NULL) freeapplib(modsavl2);
	if(filesavl!=NULL) freeapplib(filesavl);
	if(wdgtstyle!=NULL) freeapplib(wdgtstyle);
	if(incrsrc!=NULL)
	{
		killwindow(incrsrc);
		free_rsrc(incrsrc);
	}
}
static char IsAKeyPart(short fileno,char *fldname)
{
	int x;
	NRDkey *key=NULL;
	NRDpart *part=NULL;

	if(!isEMPTY(fldname))
	{
		key=KEYPOINTER(fileno);
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			if(!RDAstrcmp(part->name,fldname)) return(TRUE);
		}
	}
	return(FALSE);
}
static int addEditWdgt(char *name,short type,unsigned len,
	int etype,char *expression,char id_mgt,char *module,char *filename,
	char *subkeyname,
	int numconflds,RDAeditfld *conflds,char *mgt_expression,
	char *assignname,char *id,char *override_expression,
	char setaccttype,APPlib *acctrscs,char setdeftype,APPlib *defrscs,char lvirtual)
{
	AddEditWdgt *EditWdgt=NULL;
	RDAeditfld *confld1=NULL,*confld2=NULL;
	int x=0;

	if(tempEditWdgts!=NULL)
	{
		tempEditWdgts=Rrealloc(tempEditWdgts,(tmpnumEditWdgts+1)*sizeof(AddEditWdgt));
	} else {
		tempEditWdgts=Rmalloc(sizeof(AddEditWdgt));
		tmpnumEditWdgts=0;
	}
	EditWdgt=tempEditWdgts+tmpnumEditWdgts;
	if(!isEMPTY(name))
	{
		EditWdgt->name=stralloc(name);
	} else EditWdgt->name=NULL;
	EditWdgt->type=type;
	EditWdgt->len=len;
	EditWdgt->etype=etype;
	if(!isEMPTY(expression))
	{
		EditWdgt->expression=stralloc(expression);
	} else EditWdgt->expression=NULL;
	EditWdgt->id_mgt=id_mgt;
	EditWdgt->loadvirtual=lvirtual;
	if(id_mgt==TRUE)
	{
		if(!isEMPTY(module))
		{
			EditWdgt->module=stralloc(module);
		} else EditWdgt->module=NULL;
		if(!isEMPTY(filename))
		{
			EditWdgt->filename=stralloc(filename);
		} else EditWdgt->filename=NULL;
		if(!isEMPTY(subkeyname))
		{
			EditWdgt->subkeyname=stralloc(subkeyname);
		} else EditWdgt->subkeyname=NULL;
		if(conflds!=NULL && numconflds>0)
		{		
			EditWdgt->confld=Rmalloc(numconflds*sizeof(RDAeditfld));
			for(x=0,confld1=conflds,confld2=EditWdgt->confld;x<numconflds;++x,++confld1,++confld2)
			{
			if(!isEMPTY(confld1->fldname))
			{
				confld2->fldname=stralloc(confld1->fldname);
			} else confld2->fldname=NULL;
			if(!isEMPTY(confld1->conname))
			{
				confld2->conname=stralloc(confld1->conname);
			} else confld2->conname=NULL;
			}
			EditWdgt->num=numconflds;
		} else {
			EditWdgt->confld=NULL;
			EditWdgt->num=0;
		}
		if(!isEMPTY(mgt_expression))
		{
			EditWdgt->mgt_expression=stralloc(mgt_expression);
		} else EditWdgt->mgt_expression=NULL;
		if(!isEMPTY(assignname))
		{
			EditWdgt->assignname=stralloc(assignname);
		} else EditWdgt->assignname=NULL;
		if(!isEMPTY(id))
		{
			EditWdgt->id=stralloc(id);
		} else EditWdgt->id=NULL;
		if(!isEMPTY(override_expression))
		{
			EditWdgt->override_expression=stralloc(override_expression);
		} else EditWdgt->override_expression=NULL;
	} else {
		EditWdgt->module=NULL;
		EditWdgt->subkeyname=NULL;
		EditWdgt->filename=NULL;
		EditWdgt->override_expression=NULL;
		EditWdgt->id=NULL;
		EditWdgt->assignname=NULL;
		EditWdgt->mgt_expression=NULL;
		EditWdgt->confld=NULL;
		EditWdgt->num=0;
	}
	if(setaccttype==TRUE)
	{
		EditWdgt->setaccttype=TRUE;
		if(acctrscs!=NULL)
		{
			EditWdgt->acctrsc=APPlibNEW();
			for(x=0;x<acctrscs->numlibs;++x)
			{
				addAPPlib(EditWdgt->acctrsc,acctrscs->libs[x]);
			}
		} else {
			EditWdgt->acctrsc=NULL;
			EditWdgt->setaccttype=FALSE;
		}
	} else {
		EditWdgt->acctrsc=NULL;
		EditWdgt->setaccttype=FALSE;
	}
	if(setdeftype==TRUE)
	{
		EditWdgt->setdeftype=TRUE;
		if(defrscs!=NULL)
		{
			EditWdgt->defrsc=APPlibNEW();
			for(x=0;x<defrscs->numlibs;++x)
			{
				addAPPlib(EditWdgt->defrsc,defrscs->libs[x]);
			}
		} else {
			EditWdgt->defrsc=NULL;
			EditWdgt->setdeftype=FALSE;
		}
	} else {
		EditWdgt->defrsc=NULL;
		EditWdgt->setdeftype=FALSE;
	}
	return(++tmpnumEditWdgts);
}
void savewizard(RDArsrc *incrsrc,RDArsrc *mtnrsrc)
{
	int selmodule=0,selfilename=0,style=0,x=0,y=0,z=0,value=0;
	char *expression=NULL,*temp2=NULL,*temp3=NULL;
	short fileno=(-1),noflds=0,exp_len=0;
	RDAeditfld *aconfld1,*confld1=NULL,*confld2=NULL;
	NRDfield *field=NULL,*fields=NULL;
	APPlib *key_fields=NULL,*reg_fields=NULL;
	AddEditWdgt *EditWdgt=NULL,*temp=NULL,*temp1=NULL;
	char c_supporting=FALSE,c_writes=FALSE;
	

	if(FINDRSCGETINT(mtnrsrc,"EDIT WIDGETS",&value)) return;
	readallwidgets(incrsrc);
	FINDRSCGETINT(incrsrc,"MODULE LIST",&selmodule);
	FINDRSCGETINT(incrsrc,"FILE LIST",&selfilename);
	FINDRSCGETINT(incrsrc,"WIDGET STYLE",&style);
	FINDRSCGETCHAR(incrsrc,"CREATE SUPPORTING FILES",&c_supporting);
	FINDRSCGETCHAR(incrsrc,"CREATE POWER WRITES",&c_writes);
	fileno=OPNNRD(modsavl2->libs[selmodule],filesavl->libs[selfilename]);
	if(fileno!=(-1))
	{		
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
		noflds=NUMFLDS(fileno);
		fields=FLDPOINTER(fileno);
		if(fields!=NULL)
		{
			key_fields=APPlibNEW();
			reg_fields=APPlibNEW();
			for(x=0,field=fields;x<noflds;++x,++field)
			{
				if(RDAstrcmp(field->name,"DELETEFLAG"))
				{
					if(IsAKeyPart(fileno,field->name))
					{
						addAPPlib(key_fields,field->name);
					} else {
						addAPPlib(reg_fields,field->name);
					}
				}
			}
			if(expression!=NULL) Rfree(expression);
			for(x=0;x<key_fields->numlibs;++x)
			{
				field=FLDNRD(fileno,key_fields->libs[x]);
				if(field!=NULL)
				{
					expression=NULL;
					temp2=Rmalloc(RDAstrlen(field->name)+10);
					sprintf(temp2,"ADD %s",(field->name!=NULL ? field->name:""));
					addEditWdgt(temp2,field->type,field->len,3,expression,0,"","","",0,NULL,NULL,"",temp2,NULL,0,NULL,0,NULL,FALSE);
					if(temp2!=NULL) Rfree(temp2);
				}
			}
			if(style==1)
			{
				temp3=stralloc("IF(");
				exp_len=RDAstrlen(temp3);
				for(x=0;x<key_fields->numlibs;++x)
				{
					if(x>0)
					{
						temp3=Rrealloc(temp3,(exp_len+5));
						sprintf(&temp3[exp_len]," OR ");
						exp_len=RDAstrlen(temp3);
					}
					temp3=Rrealloc(temp3,(exp_len+(RDAstrlen(key_fields->libs[x])*2)+19));
					sprintf(&temp3[exp_len],"([ADD %s]!=[LAST %s])",key_fields->libs[x],key_fields->libs[x]);
					exp_len=RDAstrlen(temp3);
				}
				temp3=Rrealloc(temp3,(exp_len+21));
				sprintf(&temp3[exp_len],") THEN RETURN_VALUE=");
				exp_len=RDAstrlen(temp3);
			} else {
				temp3=NULL;
			}
			for(x=0;x<reg_fields->numlibs;++x)
			{
				field=FLDNRD(fileno,reg_fields->libs[x]);
				if(field!=NULL)
				{
					if(style==1)
					{
						expression=Rmalloc(exp_len+250);
						sprintf(expression,"%s[%s][%s][%s] ELSE RETURN_VALUE=[ADD %s] FI",(temp3!=NULL ? temp3:""),MODULENAME(fileno),FILENAME(fileno),reg_fields->libs[x],reg_fields->libs[x]);
					} else {
						expression=NULL;
					}
					temp2=Rmalloc(RDAstrlen(field->name)+10);
					sprintf(temp2,"ADD %s",(field->name!=NULL ? field->name:""));
					addEditWdgt(temp2,field->type,field->len,2,expression,0,"","","",0,NULL,NULL,"",temp2,NULL,0,NULL,0,NULL,FALSE);
					if(temp2!=NULL) Rfree(temp2);
				}
				if(expression!=NULL) Rfree(expression);
			}
			temp2=stralloc("ADD SESSION COUNTER");
			expression=Rmalloc(RDAstrlen(temp2)+10);
			sprintf(expression,"([%s]+1)",(temp2!=NULL ? temp2:""));
			addEditWdgt(temp2,LONGV,6,4,expression,0,"","","",0,NULL,NULL,"",temp2,NULL,0,NULL,0,NULL,FALSE);
			if(expression!=NULL) Rfree(expression);
			if(temp2!=NULL) Rfree(temp2);
			for(x=0;x<key_fields->numlibs;++x)
			{
				field=FLDNRD(fileno,key_fields->libs[x]);
				if(field!=NULL)
				{
					expression=Rmalloc(RDAstrlen(field->name)+10);
					sprintf(expression,"[ADD %s]",(field->name!=NULL ? field->name:""));
					temp2=Rmalloc(RDAstrlen(field->name)+10);
					sprintf(temp2,"LAST %s",(field->name!=NULL ? field->name:""));
					addEditWdgt(temp2,field->type,field->len,2,expression,0,"","","",0,NULL,NULL,"",temp2,NULL,0,NULL,0,NULL,FALSE);
					if(expression!=NULL) Rfree(expression);
					if(temp2!=NULL) Rfree(temp2);
				}
			}
			if(key_fields!=NULL) freeapplib(key_fields);
			if(reg_fields!=NULL) freeapplib(reg_fields);
		}
		changedvalues=TRUE;
		getWidgets();
		if(!FINDRSCLISTAPPlib(mtnrsrc,"EDIT WIDGETS",value,wdgtavl))
			updatersrc(mtnrsrc,"EDIT WIDGETS");
		if(POWERADD->EditWdgts!=NULL)
		{
			for(x=0,temp=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++temp)
			{
				if(temp->name!=NULL) Rfree(temp->name);
				if(temp->expression!=NULL) Rfree(temp->expression);
				if(temp->module!=NULL) Rfree(temp->module);
				if(temp->filename!=NULL) Rfree(temp->filename);
				if(temp->subkeyname!=NULL) Rfree(temp->subkeyname);
				if(temp->acctrsc!=NULL) freeapplib(temp->acctrsc);
				if(temp->defrsc!=NULL) freeapplib(temp->defrsc);
				if(temp->confld!=NULL)
				{
					for(z=0,confld1=temp->confld;z<temp->num;++z,++confld1)
					{
						if(confld1->fldname!=NULL) Rfree(confld1->fldname);
						if(confld1->conname!=NULL) Rfree(confld1->conname);
					}
					Rfree(temp->confld);
				}
				temp->num=0;
				if(temp->assignname!=NULL) Rfree(temp->assignname);
				if(temp->mgt_expression!=NULL) Rfree(temp->mgt_expression);
				if(temp->id!=NULL) Rfree(temp->id);
				if(temp->override_expression!=NULL) Rfree(temp->override_expression);
			}
			Rfree(POWERADD->EditWdgts);
			POWERADD->EditWdgts=NULL;
			POWERADD->numwdgts=0;
		}
		if(tempEditWdgts!=NULL)
		{
			POWERADD->numwdgts=tmpnumEditWdgts;
			POWERADD->EditWdgts=Rmalloc(tmpnumEditWdgts*sizeof(AddEditWdgt));
			for(x=0,temp=tempEditWdgts,temp1=POWERADD->EditWdgts;x<tmpnumEditWdgts;++x,++temp,
				++temp1)
			{
				if(temp->name!=NULL)
				{
					temp1->name=stralloc(temp->name);
				} else temp1->name=NULL;
				temp1->type=temp->type;
				temp1->len=temp->len;
				temp1->etype=temp->etype;
				temp1->id_mgt=temp->id_mgt;
				temp1->setaccttype=temp->setaccttype;
				temp1->setdeftype=temp->setdeftype;
				if(temp->acctrsc!=NULL)
				{
					temp1->acctrsc=APPlibNEW();
					for(y=0;y<temp->acctrsc->numlibs;++y)
					{
						addAPPlib(temp1->acctrsc,temp->acctrsc->libs[y]);
					}
				} else temp1->acctrsc=NULL;
				if(temp->defrsc!=NULL)
				{
					temp1->defrsc=APPlibNEW();
					for(y=0;y<temp->defrsc->numlibs;++y)
					{
						addAPPlib(temp1->defrsc,temp->defrsc->libs[y]);
					}
				} else temp1->defrsc=NULL;
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
				if(temp->confld!=NULL)
				{
					temp1->num=temp->num;
					temp1->confld=Rmalloc(temp->num*sizeof(RDAeditfld));
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
					}
				} else {
					temp1->num=0;
				}
				if(temp->assignname!=NULL)
				{
					temp1->assignname=stralloc(temp->assignname);
				} else temp1->assignname=NULL;
				if(temp->id!=NULL)
				{
					temp1->id=stralloc(temp->id);
				} else temp1->id=NULL;
				if(temp->mgt_expression!=NULL)
				{
					temp1->mgt_expression=stralloc(temp->mgt_expression);
				} else temp1->mgt_expression=NULL;
				if(temp->override_expression!=NULL)
				{
					temp1->override_expression=stralloc(temp->override_expression);
				} else temp1->override_expression=NULL;
				temp1->fileno=temp->fileno;
				if(temp->expression!=NULL)
				{
					temp1->expression=stralloc(temp->expression);
				} else temp1->expression=NULL;
				temp1->loadvirtual=temp->loadvirtual;
			}
		}
		if(c_supporting)
		{
			create_default_supporting(mtnrsrc,fileno,modsavl2->libs[selmodule],filesavl);
		}
		if(c_writes)
		{
			create_default_writes(mtnrsrc,fileno);
		}
		CLSNRD(fileno);
	} else {
		prterr("Error:  Couldn't open file... wizard aborted without contributing to the power add.");
	}
	quitwizard(incrsrc);
}
void wdgtwizard(RDArsrc *subrsrc,RDArsrc *mtnrsrc)
{
	int x=0,y=0;
	RDArsrc *incrsrc=NULL;
	char c_writes=TRUE,c_supporting=TRUE;

	readwidget(mtnrsrc,"MODULE");
	readwidget(mtnrsrc,"NAME");
	FINDRSCGETSTRING(mtnrsrc,"MODULE",&POWERADD->module);
	FINDRSCGETSTRING(mtnrsrc,"NAME",&POWERADD->name);
	incrsrc=RDArsrcNEW(module,"POWER ADD WIZARD");
	x=getfilemodules3(POWERADD->module);
	y=dofilelist3(incrsrc,POWERADD->module,NULL);
	addlstrsrc(incrsrc,"MODULE LIST",&x,TRUE,dofilelistcb3,
		modsavl2->numlibs,&modsavl2->libs,NULL);
	addlstrsrc(incrsrc,"FILE LIST",&y,TRUE,NULL,
		filesavl->numlibs,&filesavl->libs,NULL);
	x=0;
	wdgtstyle=APPlibNEW();
	addAPPlib(wdgtstyle,"Blank");
	addAPPlib(wdgtstyle,"Add/Change");
	addlstrsrc(incrsrc,"WIDGET STYLE",&x,TRUE,NULL,wdgtstyle->numlibs,&wdgtstyle->libs,NULL);
	addstdrsrc(incrsrc,"CREATE SUPPORTING FILES",BOOLNS,1,&c_supporting,TRUE);
	addstdrsrc(incrsrc,"CREATE POWER WRITES",BOOLNS,1,&c_writes,TRUE);
	addrfcbrsrc(incrsrc,"SELECT",TRUE,savewizard,subrsrc);
	addrfcbrsrc(incrsrc,"QUIT",TRUE,quitwizard,NULL);
	addbtnrsrc(incrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(incrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(incrsrc,TRUE,quitwizard,NULL,FALSE);
}
