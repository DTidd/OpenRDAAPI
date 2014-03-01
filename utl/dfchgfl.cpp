/* dfchgfl - Define Change Files section of Define Change */
#include <dfchg.hpp>
#include <ldval.hpp>

extern void supportingabove(RDArsrc *),supportingbelow(RDArsrc *);
extern void supportingedit(RDArsrc *),supportingdelete(RDArsrc *);
extern APPlib *makefieldvallist2(CHGfile *,int,short,char *,char *);
extern CHGstruct *CHGSTRUCT;
extern RDArsrc *mtnrsrc;
extern char *module,changedvalues;
static RDAdefault *mchgfiledefaults=NULL;
static char changedchgfiles=FALSE;
CHGfield *holdchgfields=NULL;
RDAsearch *holdsupfils=NULL;
extern CHGfile *tempchgfile;
extern int numchgfile;
extern APPlib *modlstx,*chgfilesavl,*keylst,*filelst,*chgflds,*supfilsavl;
static APPlib *fieldlst=NULL;
int numchgfields=0,numsupfils=0;
static void loadvalue(RDArsrc *,char *);
void savechg(void),chgedit(RDArsrc *,RDArsrc *),chgdelete(RDArsrc *);
void DFchgcopy(RDArsrc *,RDArsrc *);
void chgaddabove(RDArsrc *,RDArsrc *),chgaddbelow(RDArsrc *,RDArsrc *);
void getchgfiles(void);
static void dofilelist(RDArsrc *);
static void domodulelist(RDArsrc *);

static void getchgfields()
{
	int x;
	CHGfield *f;

	if(chgflds!=NULL) freeapplib(chgflds);
	chgflds=APPlibNEW();
	if(holdchgfields!=NULL)
	{
		for(x=0,f=holdchgfields;x<numchgfields;++x,++f)
		{
			addAPPlib(chgflds,f->fieldname);
		}		
	}
	if(chgflds->numlibs<1)
	{
		addAPPlib(chgflds,"No Change Fields Defined");
	}
}
static void makespacechgfield(int pos)
{
	int x=0,y=0;
	CHGfield *temp=NULL,*temp1=NULL,*new_fields=NULL;

	++numchgfields;
	new_fields=Rmalloc(numchgfields*sizeof(CHGfield));
	y=0;
	for(x=0,temp=holdchgfields;x<(numchgfields-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->fieldname=NULL;
			temp1->keypart=FALSE;
			++y;
			temp1=new_fields+y;
		}
		if(temp->fieldname!=NULL)
		{
			temp1->fieldname=stralloc(temp->fieldname);
			Rfree(temp->fieldname);
		} else temp1->fieldname=NULL;
		temp1->keypart=temp->keypart;
	}
	Rfree(holdchgfields);
	holdchgfields=Rmalloc(numchgfields*sizeof(CHGfield));
	for(x=0,temp=new_fields,temp1=holdchgfields;x<numchgfields;
		++x,++temp,++temp1)
	{
		if(temp->fieldname!=NULL)
		{
			temp1->fieldname=stralloc(temp->fieldname);
			Rfree(temp->fieldname);
		} else temp1->fieldname=NULL;
		temp1->keypart=temp->keypart;
	}
	Rfree(new_fields);
}
static void quitfield(RDArsrc *chgfldrsrc,RDArsrc *mainrsrc)
{
	if(fieldlst!=NULL) freeapplib(fieldlst);
	if(chgfldrsrc!=NULL)
	{
		killwindow(chgfldrsrc);
		free_rsrc(chgfldrsrc);
	}
}
static void savefield(RDArsrc *chgfldrsrc,RDArsrc *mainrsrc)
{
	int x,value=0;
	CHGfield *f;
	
	readwidget(chgfldrsrc,"FIELD NAMES");
	FINDRSCGETINT(mainrsrc,"CHANGE FIELDS",&value);
	if(holdchgfields==NULL)
	{
		holdchgfields=Rmalloc(sizeof(CHGfield));
		numchgfields=1;
		f=holdchgfields;
		f->fieldname=NULL;
		f->keypart=0;	
	}
	f=holdchgfields+value;
	if(fieldlst->numlibs>0)
	{
		FINDRSCGETINT(chgfldrsrc,"FIELD NAMES",&x);
		f->fieldname=stralloc(fieldlst->libs[x]);
	} else if(f->fieldname!=NULL) Rfree(f->fieldname);
	getchgfields();
	if(!FINDRSCLISTAPPlib(mainrsrc,"CHANGE FIELDS",value,chgflds))
		updatersrc(mainrsrc,"CHANGE FIELDS");
	quitfield(chgfldrsrc,mainrsrc);
}
static void quitfieldbelow(RDArsrc *chgfldrsrc,RDArsrc *mainrsrc)
{
	quitfield(chgfldrsrc,mainrsrc);
}
static void savefieldbelow(RDArsrc *chgfldrsrc,RDArsrc *mainrsrc)
{
	int x=0;

	if(FINDRSCGETINT(mainrsrc,"CHANGE FIELDS",&x)) return;
	makespacechgfield(++x);
	FINDRSCSETINT(mainrsrc,"CHANGE FIELDS",x);
	savefield(chgfldrsrc,mainrsrc);
}
static void quitfieldabove(RDArsrc *chgfldrsrc,RDArsrc *mainrsrc)
{
	quitfield(chgfldrsrc,mainrsrc);
}
static void savefieldabove(RDArsrc *chgfldrsrc,RDArsrc *mainrsrc)
{
	int x=0;

	if(FINDRSCGETINT(mainrsrc,"CHANGE FIELDS",&x)) return;
	makespacechgfield(x);
	savefield(chgfldrsrc,mainrsrc);
}
static void chgfieldscreen(RDArsrc *rsrc,int num,char *fieldname,
	void (*savefunc)(...),void (*quitfunc)(...))
{
	RDArsrc *chgfldrsrc=NULL;
	int x,y,z,selectedm,selectedf;
	NRDfield *fields=NULL,*f;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part=NULL;
	short nofields=0,nokeys=0,engine=0;

	chgfldrsrc=RDArsrcNEW(module,"MAINTAIN CHANGE FIELD");
	if(FINDRSCGETINT(rsrc,"MODULE",&selectedm)) return;
	if(FINDRSCGETINT(rsrc,"FILENAME",&selectedf)) return;
	if(fieldlst!=NULL) freeapplib(fieldlst);
	fieldlst=APPlibNEW();
	if(selectedm>=modlstx->numlibs) selectedm=0;
	if(selectedf>=filelst->numlibs) selectedf=0;
	if(!GETBIN(modlstx->libs[selectedm],filelst->libs[selectedf],&engine,&nofields,&fields,&nokeys,&keys))
	{
		if(fields!=NULL)
		{
			for(y=0,f=fields;y<nofields;++y,++f)
			{
				if(f->name!=NULL) 
				{
					addAPPlib(fieldlst,f->name);
					Rfree(f->name);
				}
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
	if(fieldlst->numlibs<1)
	{
		addAPPlib(keylst,"No Fields Available");
	}
	x=0;
	if(fieldname!=NULL)
	{
		for(x=0;x<fieldlst->numlibs;++x)
		{
			if(!RDAstrcmp(fieldlst->libs[x],fieldname)) break;
		}
		if(x>=fieldlst->numlibs) x=0;
	} 
	addstdrsrc(chgfldrsrc,"NUMBER",LONGV,8,&num,FALSE);
	addlstrsrc(chgfldrsrc,"FIELD NAMES",&x,TRUE,NULL,fieldlst->numlibs,
		&fieldlst->libs,NULL);
	addbtnrsrc(chgfldrsrc,"SAVE",TRUE,savefunc,rsrc);
	addbtnrsrc(chgfldrsrc,"QUIT",TRUE,quitfunc,rsrc);
	addbtnrsrc(chgfldrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(chgfldrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(chgfldrsrc,TRUE,quitfield,rsrc,FALSE);
}
static void fieldaddabove(RDArsrc *mainrsrc)
{
	int selected=0;
	
	if(FINDRSCGETINT(mainrsrc,"CHANGE FIELDS",&selected)) return;
	if(selected==0 && !RDAstrcmp(chgflds->libs[selected],"No Change Fields Defined")) return;
	chgfieldscreen(mainrsrc,++selected,NULL,savefieldabove,quitfieldabove);
}
static void fieldaddbelow(RDArsrc *mainrsrc)
{
	int selected=0;
	
	if(FINDRSCGETINT(mainrsrc,"CHANGE FIELDS",&selected)) return;
	if(selected==0 && !RDAstrcmp(chgflds->libs[selected],"No Change Fields Defined")) return;
	selected+=2;
	chgfieldscreen(mainrsrc,++selected,NULL,savefieldbelow,quitfieldbelow);
}
static void fieldedit(RDArsrc *mainrsrc)
{
	int selected=0;
	CHGfield *f=NULL;
	
	if(FINDRSCGETINT(mainrsrc,"CHANGE FIELDS",&selected)) return;
	f=holdchgfields+selected;
	if(f!=NULL)
	{
		chgfieldscreen(mainrsrc,++selected,f->fieldname,
			savefield,quitfield);
	} else {
		chgfieldscreen(mainrsrc,++selected,NULL,savefield,quitfield);
	}	
}
static void fielddelete(RDArsrc *mainrsrc)
{
	int x,y;
	int value=0;
	CHGfield *new_fields=NULL,*fld1=NULL,*fld2=NULL;

	readallwidgets(mainrsrc);
	if(FINDRSCGETINT(mainrsrc,"CHANGE FIELDS",&value)) return;
	if(numchgfields<1) return;
	if(numchgfields>1)
	{
		new_fields=Rmalloc((numchgfields-1)*sizeof(CHGfield));
		y=0;
		for(x=0,fld1=holdchgfields;x<numchgfields;++x,	
			++fld1)
		{
			fld2=new_fields+y;
			if(x!=value)
			{
				if(fld1->fieldname!=NULL)
				{
					fld2->fieldname=stralloc(fld1->fieldname);
					Rfree(fld1->fieldname);
				} else fld2->fieldname=NULL;
				fld2->keypart=fld1->keypart;
				++y;
			} else {
				if(fld1->fieldname!=NULL) Rfree(fld1->fieldname);
			}
		}
		Rfree(holdchgfields);
		--numchgfields;
		holdchgfields=Rmalloc(numchgfields*sizeof(CHGfield));
		for(x=0,fld1=new_fields,fld2=holdchgfields;x<numchgfields;
			++x,++fld1,++fld2)
		{
			if(fld1->fieldname!=NULL)
			{
				fld2->fieldname=stralloc(fld1->fieldname);
				Rfree(fld1->fieldname);
			} else fld2->fieldname=NULL;
			fld2->keypart=fld1->keypart;
		}
		Rfree(new_fields);
	} else if(holdchgfields!=NULL)
	{
		for(x=0,fld1=holdchgfields;x<numchgfields;++x,++fld1)
		{
			if(fld1->fieldname!=NULL) Rfree(fld1->fieldname);
		}
		Rfree(holdchgfields);
		numchgfields=0;
	}
	getchgfields();
	if(value>=chgflds->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"CHANGE FIELDS",value,chgflds))
		updatersrc(mainrsrc,"CHANGE FIELDS");
	changedchgfiles=TRUE;
}
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
	loadlist=makefieldvallist2(tempchgfile,numchgfile,TRUE,modname,filename);
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
void getchgfiles()
{
	int x,len=0;
	CHGfile *subs;
	char *tmp=NULL;

	if(chgfilesavl!=NULL) freeapplib(chgfilesavl);
	chgfilesavl=APPlibNEW();
	if(tempchgfile!=NULL)
	{
		for(x=0,subs=tempchgfile;x<numchgfile;++x,++subs)
		{
			len=RDAstrlen(subs->module)+RDAstrlen(subs->filename)+15;
			if(tmp!=NULL) 
			{
				tmp=Rrealloc(tmp,len);
			} else {
				tmp=Rmalloc(len);
			}
			memset(tmp,0,len);
			sprintf(tmp,"%s %s %s",
				(subs->module!=NULL?subs->module:""),
				(subs->filename!=NULL?subs->filename:""),
				(subs->expression!=NULL ? "Conditional":""));
			addAPPlib(chgfilesavl,tmp);
		}
	}
	if(chgfilesavl->numlibs<1)
	{
		addAPPlib(chgfilesavl,"No Change Files Defined");
	}
}
static void quitmsubo(RDArsrc *msuborsrc,RDArsrc *chgrsrc)
{
	int x,y;
	CHGfield *chgfield;
	RDAsearch *s;
	RDAconfld *c;

	if(msuborsrc!=NULL)
	{
		killwindow(msuborsrc);
		free_rsrc(msuborsrc);
	}
	if(modlstx!=NULL) freeapplib(modlstx);
	if(filelst!=NULL) freeapplib(filelst);
	if(chgflds!=NULL) freeapplib(chgflds);
	if(supfilsavl!=NULL) freeapplib(supfilsavl);
	if(holdsupfils!=NULL)
	{
		for(x=0,s=holdsupfils;x<numsupfils;++x,++s)
		{
			if(s->module!=NULL) Rfree(s->module);
			if(s->filename!=NULL) Rfree(s->filename);
			if(s->cmodule!=NULL) Rfree(s->cmodule);
			if(s->cfilename!=NULL) Rfree(s->cfilename);
			if(s->confld!=NULL)
			{
				for(y=0,c=s->confld;y<s->num;++y,++c)
				{
					if(c->conname!=NULL) Rfree(c->conname);	
					if(c->fldname!=NULL) Rfree(c->fldname);
				}
				Rfree(s->confld);
			}
			if(s->subkeyname!=NULL) Rfree(s->subkeyname);
			if(s->expression!=NULL) Rfree(s->expression);
		}
		Rfree(holdsupfils);
		numsupfils=0;
	}
	if(holdchgfields!=NULL)
	{
		for(x=0,chgfield=holdchgfields;x<numchgfields;++x,++chgfield)
		{
			if(chgfield->fieldname!=NULL) Rfree(chgfield->fieldname);
		}
		Rfree(holdchgfields);
		numchgfields=0;
	}
	if(changedchgfiles) changedvalues=TRUE;
	if(mchgfiledefaults!=NULL) FreeRDAdefault(mchgfiledefaults);
}
static void makespacechgfile(int pos)
{
	int x,y,z,w;
	CHGfile *temp,*temp1,*new_fields;
	CHGfield *fld1,*fld2;
	RDAsearch *s1=NULL,*s2=NULL;
	RDAconfld *c1=NULL,*c2=NULL;

	++numchgfile;
	new_fields=Rmalloc(numchgfile*sizeof(CHGfile));
	y=0;
	for(x=0,temp=tempchgfile;x<(numchgfile-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->module=NULL;
			temp1->filename=NULL;
			temp1->keyname=NULL;
			temp1->expression=NULL;
			temp1->num=0;
			temp1->supfil=NULL;
			temp1->numflds=0;
			temp1->chgflds=NULL;
			temp1->fileno=(-1);
			temp1->keyno=(-1);
			temp1->prev=NULL;
			temp1->parent=NULL;
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
		if(temp->keyname!=NULL)
		{
			temp1->keyname=stralloc(temp->keyname);
			Rfree(temp->keyname);
		} else temp1->keyname=NULL;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		if(temp->supfil!=NULL)
		{
			temp1->num=temp->num;
			temp1->supfil=Rmalloc(temp1->num*sizeof(RDAsearch));
			for(z=0,s1=temp->supfil,s2=temp1->supfil;z<temp1->num;++z,++s1,++s2)
			{
				if(s1->module!=NULL)
				{
					s2->module=stralloc(s1->module);
					Rfree(s1->module);
				} else s2->module=NULL;
				if(s1->filename!=NULL)
				{
					s2->filename=stralloc(s1->filename);
					Rfree(s1->filename);
				} else s2->filename=NULL;
				s2->ctype=s1->ctype;
				if(s1->cmodule!=NULL)
				{
					s2->cmodule=stralloc(s1->cmodule);
					Rfree(s1->cmodule);
				} else s2->cmodule=NULL;
				if(s1->cfilename!=NULL)
				{
					s2->cfilename=stralloc(s1->cfilename);
					Rfree(s1->cfilename);
				} else s2->cfilename=NULL;
				if(s1->confld!=NULL)
				{
					s2->num=s1->num;
					s2->confld=Rmalloc(s2->num*sizeof(RDAconfld));
					for(w=0,c1=s1->confld,c2=s2->confld;
						w<s2->num;++w,++c1,++c2)
					{
						if(c1->conname!=NULL)
						{
							c2->conname=stralloc(c1->conname);
							Rfree(c1->conname);
						} else c2->conname=NULL;
						if(c1->fldname!=NULL)
						{
							c2->fldname=stralloc(c1->fldname);
							Rfree(c1->fldname);
						} else c2->fldname=NULL;
					}
					Rfree(s1->confld);
				} else {
					s2->confld=NULL;
					s2->num=0;
				}
				if(s1->subkeyname!=NULL)
				{
					s2->subkeyname=stralloc(s1->subkeyname);
					Rfree(s1->subkeyname);
				} else s2->subkeyname=NULL;
				s2->range=s1->range;
				s2->num_writes=s1->num_writes;
				if(s1->expression!=NULL)
				{
					s2->expression=stralloc(s1->expression);
					Rfree(s1->expression);
				} else s2->expression=NULL;
				s2->fileno=s1->fileno;
				s2->keyno=s1->keyno;
				s2->occurrence=s1->occurrence;
				s2->writable=s1->writable;
			}
			Rfree(temp->supfil);
		} else {
			temp1->num=0;
			temp1->supfil=NULL;
		}
		if(temp->chgflds!=NULL)
		{
			temp1->numflds=temp->numflds;
			temp1->chgflds=Rmalloc(temp1->numflds*sizeof(CHGfield));
			for(z=0,fld1=temp->chgflds,fld2=temp1->chgflds;z<temp1->numflds;++z,++fld1,++fld2)
			{
				if(fld1->fieldname!=NULL)
				{
					fld2->fieldname=stralloc(fld1->fieldname);
					Rfree(fld1->fieldname);
				} else fld2->fieldname=NULL;
				fld2->keypart=fld1->keypart;
			}
			Rfree(temp->chgflds);
		} else {
			temp1->chgflds=NULL;
			temp1->numflds=0;
		}
		temp1->fileno=temp->fileno;
		temp1->keyno=temp->keyno;
		temp1->prev=temp->prev;
		temp1->parent=temp->parent;
		++y;
	}
	Rfree(tempchgfile);
	tempchgfile=Rmalloc(numchgfile*sizeof(CHGfile));
	for(x=0,temp1=tempchgfile,temp=new_fields;x<numchgfile;++x,
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
		if(temp->keyname!=NULL)
		{
			temp1->keyname=stralloc(temp->keyname);
			Rfree(temp->keyname);
		} else temp1->keyname=NULL;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		if(temp->supfil!=NULL)
		{
			temp1->num=temp->num;
			temp1->supfil=Rmalloc(temp1->num*sizeof(RDAsearch));
			for(z=0,s1=temp->supfil,s2=temp1->supfil;z<temp1->num;++z,++s1,++s2)
			{
				if(s1->module!=NULL)
				{
					s2->module=stralloc(s1->module);
					Rfree(s1->module);
				} else s2->module=NULL;
				if(s1->filename!=NULL)
				{
					s2->filename=stralloc(s1->filename);
					Rfree(s1->filename);
				} else s2->filename=NULL;
				s2->ctype=s1->ctype;
				if(s1->cmodule!=NULL)
				{
					s2->cmodule=stralloc(s1->cmodule);
					Rfree(s1->cmodule);
				} else s2->cmodule=NULL;
				if(s1->cfilename!=NULL)
				{
					s2->cfilename=stralloc(s1->cfilename);
					Rfree(s1->cfilename);
				} else s2->cfilename=NULL;
				if(s1->confld!=NULL)
				{
					s2->num=s1->num;
					s2->confld=Rmalloc(s2->num*sizeof(RDAconfld));
					for(w=0,c1=s1->confld,c2=s2->confld;
						w<s2->num;++w,++c1,++c2)
					{
						if(c1->conname!=NULL)
						{
							c2->conname=stralloc(c1->conname);
							Rfree(c1->conname);
						} else c2->conname=NULL;
						if(c1->fldname!=NULL)
						{
							c2->fldname=stralloc(c1->fldname);
							Rfree(c1->fldname);
						} else c2->fldname=NULL;
					}
					Rfree(s1->confld);
				} else {
					s2->confld=NULL;
					s2->num=0;
				}
				if(s1->subkeyname!=NULL)
				{
					s2->subkeyname=stralloc(s1->subkeyname);
					Rfree(s1->subkeyname);
				} else s2->subkeyname=NULL;
				s2->range=s1->range;
				s2->num_writes=s1->num_writes;
				if(s1->expression!=NULL)
				{
					s2->expression=stralloc(s1->expression);
					Rfree(s1->expression);
				} else s2->expression=NULL;
				s2->fileno=s1->fileno;
				s2->keyno=s1->keyno;
				s2->occurrence=s1->occurrence;
				s2->writable=s1->writable;
			}
			Rfree(temp->supfil);
		} else {
			temp1->num=0;
			temp1->supfil=NULL;
		}
		if(temp->chgflds!=NULL)
		{
			temp1->numflds=temp->numflds;
			temp1->chgflds=Rmalloc(temp1->numflds*sizeof(CHGfield));
			for(z=0,fld1=temp->chgflds,fld2=temp1->chgflds;z<temp1->numflds;++z,++fld1,++fld2)
			{
				if(fld1->fieldname!=NULL)
				{
					fld2->fieldname=stralloc(fld1->fieldname);
					Rfree(fld1->fieldname);
				} else fld2->fieldname=NULL;
				fld2->keypart=fld1->keypart;
			}
			Rfree(temp->chgflds);
		} else {
			temp1->chgflds=NULL;
			temp1->numflds=0;
		}
		temp1->fileno=temp->fileno;
		temp1->keyno=temp->keyno;
		temp1->prev=temp->prev;
		temp1->parent=temp->parent;
	}
	Rfree(new_fields);
}
static void savemsubo(RDArsrc *msuborsrc,RDArsrc *chgrsrc)
{
	int value=0,selectedk=0,selectedm=0,selectedf=0;
	CHGfile *chgfile;
	int x,y;
	CHGfield *cf1,*cf2;
	RDAsearch *s1=NULL,*s2=NULL;
	RDAconfld *c1=NULL,*c2=NULL;

	if(FINDRSCGETINT(chgrsrc,"CHANGE FILES",&value)) return;
	if(tempchgfile==NULL)
	{
		tempchgfile=Rmalloc(sizeof(CHGfile));
		numchgfile=1;
	}
	chgfile=tempchgfile+value;
	readallwidgets(msuborsrc);
	FINDRSCGETINT(msuborsrc,"MODULE",&selectedm);
	FINDRSCGETINT(msuborsrc,"FILENAME",&selectedf);
	FINDRSCGETINT(msuborsrc,"KEYNAME",&selectedk);
	chgfile->module=stralloc(modlstx->libs[selectedm]);
	chgfile->filename=stralloc(filelst->libs[selectedf]);
	chgfile->keyname=stralloc(keylst->libs[selectedk]);
	chgfile->fileno=(-1);
	chgfile->keyno=(-1);
	chgfile->prev=NULL;
	chgfile->parent=NULL;
	FINDRSCGETSTRING(msuborsrc,"EXPRESSION",&chgfile->expression);
	if(holdsupfils!=NULL)
	{
		chgfile->num=numsupfils;
		chgfile->supfil=Rmalloc(numsupfils*sizeof(RDAsearch));
		for(x=0,s1=holdsupfils,s2=chgfile->supfil;x<chgfile->num;++x,++s1,++s2)
		{
			if(s1->module!=NULL)
			{
				s2->module=stralloc(s1->module);
				Rfree(s1->module);
			} else s2->module=NULL;
			if(s1->filename!=NULL)
			{
				s2->filename=stralloc(s1->filename);
				Rfree(s1->filename);
			} else s2->filename=NULL;
			s2->ctype=s1->ctype;
			if(s1->cmodule!=NULL)
			{
				s2->cmodule=stralloc(s1->cmodule);
				Rfree(s1->cmodule);
			} else s2->cmodule=NULL;
			if(s1->cfilename!=NULL)
			{
				s2->cfilename=stralloc(s1->cfilename);
				Rfree(s1->cfilename);
			} else s2->cfilename=NULL;	
			if(s1->confld!=NULL)
			{
				s2->num=s1->num;
				s2->confld=Rmalloc(s2->num*sizeof(RDAconfld));
				for(y=0,c1=s1->confld,c2=s2->confld;y<s2->num;++y,++c1,++c2)
				{
					if(c1->fldname!=NULL)
					{
						c2->fldname=stralloc(c1->fldname);
						Rfree(c1->fldname);
					}
					if(c1->conname!=NULL)
					{
						c2->conname=stralloc(c1->conname);
						Rfree(c1->conname);
					} else c2->conname=NULL;
				}
				Rfree(s1->confld);
				s1->num=0;
			} else {
				s2->confld=NULL;
				s2->num=0;
			}
			if(s1->subkeyname!=NULL)
			{
				s2->subkeyname=stralloc(s1->subkeyname);
				Rfree(s1->subkeyname);
			} else s2->subkeyname=NULL;
			s2->range=s1->range;
			s2->num_writes=s1->num_writes;
			if(s1->expression!=NULL)
			{
				s2->expression=stralloc(s1->expression);
				Rfree(s1->expression);
			} else s2->expression=NULL;
			s2->fileno=s1->fileno;
			s2->keyno=s1->keyno;
			s2->occurrence=s1->occurrence;
			s2->writable=s1->writable;
		}	
		Rfree(holdsupfils);
		numsupfils=0;
	} else {
		chgfile->supfil=NULL;
		chgfile->num=0;
	}
	if(holdchgfields!=NULL)
	{
		chgfile->numflds=numchgfields;
		chgfile->chgflds=Rmalloc((numchgfields)*sizeof(CHGfield));
		for(x=0,cf1=holdchgfields,cf2=chgfile->chgflds;x<numchgfields;++x,++cf1,++cf2)
		{
			if(cf1->fieldname!=NULL)
			{
				cf2->fieldname=stralloc(cf1->fieldname);
				Rfree(cf1->fieldname);
			} else cf2->fieldname=NULL;
			cf2->keypart=cf1->keypart;
		}
		Rfree(holdchgfields);
		holdchgfields=NULL;
		numchgfields=0;
	} else {
		chgfile->numflds=0;
		chgfile->chgflds=NULL;
	}
	getchgfiles();
	if(!FINDRSCLISTAPPlib(chgrsrc,"CHANGE FILES",value,chgfilesavl))
		updatersrc(chgrsrc,"CHANGE FILES");
	if(COMPARE_RSRCS(msuborsrc,mchgfiledefaults,2))
	{
		changedchgfiles=TRUE;
	}
	quitmsubo(msuborsrc,chgrsrc);
}
static void savemsuboabove(RDArsrc *msuborsrc,RDArsrc *chgrsrc)
{
	int x=0;

	if(FINDRSCGETINT(chgrsrc,"CHANGE FILES",&x)) return;
	makespacechgfile(x);
	savemsubo(msuborsrc,chgrsrc);
}
static void savemsubobelow(RDArsrc *msuborsrc,RDArsrc *chgrsrc)
{
	int x=0;

	if(FINDRSCGETINT(chgrsrc,"CHANGE FILES",&x)) return;
	makespacechgfile(++x);
	FINDRSCSETINT(chgrsrc,"CHANGE FILES",x);
	savemsubo(msuborsrc,chgrsrc);
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
static void dofilelist(RDArsrc *rsrc)
{
	int x,y,z,selectedm,selectedf;
	NRDfield *fields=NULL,*f;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part=NULL;
	short nofields=0,nokeys=0,engine=0;

	if(FINDRSCGETINT(rsrc,"MODULE",&selectedm)) return;
	if(FINDRSCGETINT(rsrc,"FILENAME",&selectedf)) return;
	if(keylst!=NULL) freeapplib(keylst);
	keylst=APPlibNEW();
	if(selectedm>=modlstx->numlibs) selectedm=0;
	if(selectedf>=filelst->numlibs) selectedf=0;
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
				addAPPlib(keylst,keyx->name);
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
	if(keylst->numlibs<1)
	{
		addAPPlib(keylst,"No Keys Available");
	}
	x=0;
	if(!FINDRSCLISTAPPlib(rsrc,"KEYNAME",x,keylst))
	{
		updatersrc(rsrc,"KEYNAME");
	}
}
static void domodulelist(RDArsrc *msuborsrc)
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
		dofilelist(msuborsrc);
	}
}
static void chgfilescreen(RDArsrc *chgrsrc,int num,char *modulex,char *filename,
	char *keyname,char *expression,int numx,RDAsearch *supfil,int numflds,
	CHGfield *chgfldx,void (*savefunc)(...),void (*quitfunctest)(...))
{
	int x=0,y,selmod=0,selfile=0;
	char *temp=NULL,*tmod=NULL,*curmod=NULL;
	CHGfield *cf,*cf1;
	RDArsrc *msuborsrc=NULL;
	RDAsearch *s1=NULL,*s2=NULL;
	RDAconfld *c1=NULL,*c2=NULL;

	if(supfilsavl!=NULL) freeapplib(supfilsavl);
	supfilsavl=APPlibNEW();
	if(supfil!=NULL)
	{
		numsupfils=numx;
		holdsupfils=Rmalloc(numx*sizeof(RDAsearch));
		for(x=0,s1=supfil,s2=holdsupfils;x<numsupfils;++x,++s1,++s2)
		{
			temp=Rmalloc(strlen(s1->module)+strlen(s1->filename)+3);
			sprintf(temp,"%s %s",(s1->module!=NULL ? s1->module:""),(s1->filename!=NULL ? s1->filename:""));
			addAPPlib(supfilsavl,temp);
			if(temp!=NULL) Rfree(temp);
			if(s1->module!=NULL) 
			{
				s2->module=stralloc(s1->module);
			}
			if(s1->filename!=NULL)
			{
				s2->filename=stralloc(s1->filename);
			}
			s1->ctype=s2->ctype;
			if(s1->cmodule!=NULL)
			{
				s2->cmodule=stralloc(s1->cmodule);
			}
			if(s1->cfilename!=NULL)
			{
				s2->cfilename=stralloc(s1->cfilename);
			}
			if(s1->confld!=NULL)
			{
				s2->num=s1->num;
				s2->confld=Rmalloc(s2->num*sizeof(RDAconfld));
				for(y=0,c1=s1->confld,c2=s2->confld;y<s1->num;
					++y,++c1,++c2)
				{
					if(c1->fldname!=NULL)
						c2->fldname=stralloc(c1->fldname);
					if(c1->conname!=NULL)
						c2->conname=stralloc(c1->conname);
				}
			} else {
				s2->confld=NULL;
				s2->num=0;
			}
			if(s1->subkeyname!=NULL)
			{
				s2->subkeyname=stralloc(s1->subkeyname);
			}
			s2->range=s1->range;
			s2->num_writes=s1->num_writes;
			s2->fileno=s1->fileno;
			s2->keyno=s1->keyno;
			s2->occurrence=s1->occurrence;
			s2->writable=s1->writable;
			if(s1->expression!=NULL)
				s2->expression=stralloc(s1->expression);
		}
	} else {
		holdsupfils=NULL;
		numsupfils=0;
	}
	msuborsrc=RDArsrcNEW(module,"EDIT CHANGE FILE");
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
		&modlstx->libs,NULL);
	if(filelst!=NULL) freeapplib(filelst);
	filelst=APPlibNEW();
	addlstrsrc(msuborsrc,"FILENAME",&x,TRUE,dofilelist,filelst->numlibs,
		&filelst->libs,NULL);
	if(keylst!=NULL) freeapplib(keylst);
	keylst=APPlibNEW();
	addlstrsrc(msuborsrc,"KEYNAME",&x,TRUE,NULL,keylst->numlibs,&keylst->libs,
		NULL);
	readwidget(chgrsrc,"MODULE");
	FINDRSCGETSTRING(chgrsrc,"MODULE",&curmod);
	if(chgflds!=NULL) freeapplib(chgflds);
	chgflds=APPlibNEW();
	if(chgfldx!=NULL && numflds)
	{
		numchgfields=numflds;
		holdchgfields=Rmalloc(numflds*sizeof(CHGfield));
		for(x=0,cf1=holdchgfields,cf=chgfldx;x<numflds;++x,++cf,++cf1)
		{
			if(temp!=NULL)
			{
				temp=Rrealloc(temp,RDAstrlen(cf->fieldname)+20);
			} else {
				temp=Rmalloc(RDAstrlen(cf->fieldname)+20);
			}
			cf1->fieldname=stralloc(cf->fieldname);
			cf1->keypart=cf->keypart;
			sprintf(temp,"Field [%s] [%s]",(cf->fieldname!=NULL ? cf->fieldname:""),(cf->keypart==TRUE ? "Keypart":"Regular"));
			addAPPlib(chgflds,temp);
		}
	}
	selmod=0;
	domodulelist(msuborsrc);
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
	dofilelist(msuborsrc); 
	if(chgflds->numlibs<1)
	{
		addAPPlib(chgflds,"No Change Fields");
	}
	x=0;
	addlstrsrc(msuborsrc,"CHANGE FIELDS",0,TRUE,NULL,chgflds->numlibs,
		&chgflds->libs,NULL);
	addbtnrsrc(msuborsrc,"ADD FIELD ABOVE",TRUE,fieldaddabove,NULL);
	addbtnrsrc(msuborsrc,"ADD FIELD BELOW",TRUE,fieldaddbelow,NULL);
	addbtnrsrc(msuborsrc,"SELECT FIELD",TRUE,fieldedit,NULL);
	addbtnrsrc(msuborsrc,"DELETE FIELD",TRUE,fielddelete,NULL);
	if(supfilsavl->numlibs<1)
	{
		addAPPlib(supfilsavl,"No Supporting Files");
	}
	addlstrsrc(msuborsrc,"SUPPORTING FILES",0,TRUE,NULL,supfilsavl->numlibs,
		&supfilsavl->libs,NULL);
	addbtnrsrc(msuborsrc,"ADD SUPPORTING ABOVE",TRUE,supportingabove,NULL);
	addbtnrsrc(msuborsrc,"ADD SUPPORTING BELOW",TRUE,supportingbelow,NULL);
	addbtnrsrc(msuborsrc,"SELECT SUPPORTING",TRUE,supportingedit,NULL);
	addbtnrsrc(msuborsrc,"DELETE SUPPORTING",TRUE,supportingdelete,NULL);
	addsctrsrc(msuborsrc,"EXPRESSION",0,expression,TRUE);
	addbtnrsrc(msuborsrc,"LOAD VALUE",TRUE,loadvalue,"EXPRESSION");
	addbtnrsrc(msuborsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(msuborsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(msuborsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");
	addrfcbrsrc(msuborsrc,"SAVE",TRUE,savefunc,chgrsrc);
	addrfcbrsrc(msuborsrc,"QUIT",TRUE,quitfunctest,chgrsrc);
	addbtnrsrc(msuborsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(msuborsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	if(temp!=NULL) Rfree(temp);
	mchgfiledefaults=GetDefaults(msuborsrc);
	changedchgfiles=FALSE;
	APPmakescrn(msuborsrc,TRUE,quitmsubo,chgrsrc,FALSE);
}
static void quitmsubotest(RDArsrc *msuborsrc,RDArsrc *chgrsrc)
{
	readallwidgets(msuborsrc);
	if(COMPARE_RSRCS(msuborsrc,mchgfiledefaults,2) || changedchgfiles)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","MASTER CHANGES NOT SAVED!","Changes have been made to this SUPPORTING FILE DEFININTION!\nDo you want to Save these changes?",savemsubo,quitmsubo,FALSE,2,msuborsrc,chgrsrc,NULL);
	} else {
		quitmsubo(msuborsrc,chgrsrc);
	}
}
static void quitmsuboabovetest(RDArsrc *msuborsrc,RDArsrc *chgrsrc)
{
	readallwidgets(msuborsrc);
	if(COMPARE_RSRCS(msuborsrc,mchgfiledefaults,2) || changedchgfiles)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","MASTER CHANGES NOT SAVED!","Changes have been made to this SURORDINATE FILE DEFININTION!\nDo you want to Save these changes?",savemsuboabove,quitmsubo,FALSE,2,msuborsrc,chgrsrc,NULL);
	} else {
		quitmsubo(msuborsrc,chgrsrc);
	}
}
static void quitmsubobelowtest(RDArsrc *msuborsrc,RDArsrc *chgrsrc)
{
	readallwidgets(msuborsrc);
	if(COMPARE_RSRCS(msuborsrc,mchgfiledefaults,2) || changedchgfiles)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","MASTER CHANGES NOT SAVED!","Changes have been made to this SUPPORTING FILE DEFININTION!\nDo you want to Save these changes?",savemsubobelow,quitmsubo,FALSE,2,msuborsrc,chgrsrc,NULL);
	} else {
		quitmsubo(msuborsrc,chgrsrc);
	}
}
void chgaddabove(RDArsrc *chgrsrc,RDArsrc *mtnrsrc)
{
	int selected=0;
	char *modx=NULL;

	if(FINDRSCGETINT(chgrsrc,"CHANGE FILES",&selected)) return;
	if(selected==0 && !RDAstrcmp(chgfilesavl->libs[selected],"No Change Files Defined"))
		return;
	readwidget(mtnrsrc,"MODULE");
	FINDRSCGETSTRING(mtnrsrc,"MODULE",&modx);
	chgfilescreen(chgrsrc,++selected,modx,NULL,NULL,NULL,0,NULL,0,NULL,
		savemsuboabove,quitmsuboabovetest);
	if(modx!=NULL) Rfree(modx);
}
void chgaddbelow(RDArsrc *chgrsrc,RDArsrc *mtnrsrc)
{
	int selected=0;
	char *modx=NULL;

	if(FINDRSCGETINT(chgrsrc,"CHANGE FILES",&selected)) return;
	if(selected==0 && !RDAstrcmp(chgfilesavl->libs[selected],"No Change Files Defined"))
		return;
	selected+=2;
	readwidget(mtnrsrc,"MODULE");
	FINDRSCGETSTRING(mtnrsrc,"MODULE",&modx);
	chgfilescreen(chgrsrc,selected,modx,NULL,NULL,NULL,0,NULL,0,NULL,
		savemsubobelow,quitmsubobelowtest);
	if(modx!=NULL) Rfree(modx);
}
void chgdelete(RDArsrc *chgrsrc)
{
	int x,y,z,w;
	CHGfile *temp,*temp1,*new_fields;
	int value;
	RDAsearch *s1=NULL,*s2=NULL;
	RDAconfld *c1=NULL,*c2=NULL;
	CHGfield *fld1,*fld2;

	readallwidgets(chgrsrc);
	if(FINDRSCGETINT(chgrsrc,"CHANGE FILES",&value)) return;
	if(numchgfile<1) return;
	if(numchgfile>1)
	{
		new_fields=Rmalloc((numchgfile-1)*sizeof(CHGfile));
		y=0;
		for(x=0,temp=tempchgfile;x<numchgfile;++x,++temp)
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
				if(temp->keyname!=NULL)
				{
					temp1->keyname=stralloc(temp->keyname);
					Rfree(temp->keyname);
				} else temp1->keyname=NULL;
				if(temp->expression!=NULL)
				{
					temp1->expression=stralloc(temp->expression);
					Rfree(temp->expression);
				} else temp1->expression=NULL;
				if(temp->supfil!=NULL)
				{
					temp1->num=temp->num;
					temp1->supfil=Rmalloc(temp1->num*sizeof(RDAsearch));
					for(z=0,s1=temp->supfil,s2=temp1->supfil;
						z<temp1->num;++z,++s1,++s2)
					{
						if(s1->module!=NULL)
						{
							s2->module=stralloc(s1->module);
							Rfree(s1->module);
						} else s2->module=NULL;
						if(s1->filename!=NULL)
						{
							s2->filename=stralloc(s1->filename);
							Rfree(s1->filename);
						} else s2->filename=NULL;
						s2->ctype=s1->ctype;
						if(s1->cmodule!=NULL)
						{
							s2->cmodule=stralloc(s1->cmodule);
							Rfree(s1->cmodule);
						} else s2->cmodule=NULL;
						if(s1->cfilename!=NULL)
						{
							s2->cfilename=stralloc(s1->cfilename);
							Rfree(s1->cfilename);
						} else s2->cfilename=NULL;
						if(s1->confld!=NULL)
						{
							s2->num=s1->num;
							s2->confld=Rmalloc(s2->num*sizeof(RDAconfld));
							for(w=0,c1=s1->confld,c2=s2->confld;w<s1->num;++w,++c1,++c2)
							{
								if(c1->fldname!=NULL)
								{
									c2->fldname=stralloc(c1->fldname);
									Rfree(c1->fldname);
								} else c2->fldname=NULL;
								if(c1->conname!=NULL)
								{
									c2->conname=stralloc(c1->conname);
									Rfree(c1->conname);
								} else c2->conname=NULL;
							}
							Rfree(s1->confld);
						} else {
							s2->confld=NULL;
							s2->num=0;
						}
						if(s1->subkeyname!=NULL)
						{
							s2->subkeyname=stralloc(s1->subkeyname);
							Rfree(s1->subkeyname);
						} else s2->subkeyname=NULL;
						s2->range=s1->range;
						s2->num_writes=s1->num_writes;
						if(s1->expression!=NULL)
						{
							s2->expression=stralloc(s1->expression);
							Rfree(s1->expression);
						} else s2->expression=NULL;
						s2->fileno=s1->fileno;
						s2->keyno=s1->keyno;
						s2->occurrence=s1->occurrence;
						s2->writable=s1->writable;
					}
					Rfree(temp->supfil);
				} else {
					temp1->supfil=NULL;
					temp1->num=0;
				}
				if(temp->chgflds!=NULL)
				{
					temp1->numflds=temp->numflds;
					temp1->chgflds=Rmalloc(temp1->numflds*sizeof(CHGfield));
					for(z=0,fld1=temp->chgflds,fld2=temp1->chgflds;z<temp1->numflds;++z,++fld1,++fld2)
					{
						fld2->keypart=fld1->keypart;
						if(fld1->fieldname!=NULL)
						{
							fld2->fieldname=stralloc(fld1->fieldname);
							Rfree(fld1->fieldname);
						} else fld2->fieldname=NULL;
					}
					Rfree(temp->chgflds);
				} else {
					temp1->chgflds=NULL;
					temp1->numflds=0;
				}
				temp1->fileno=temp->fileno;
				temp1->keyno=temp->keyno;
				temp1->prev=temp->prev;
				temp1->parent=temp->parent;
				++y;
			} else {
				if(temp->module!=NULL) Rfree(temp->module);
				if(temp->filename!=NULL) Rfree(temp->filename);
				if(temp->keyname!=NULL) Rfree(temp->keyname);
				if(temp->expression!=NULL) Rfree(temp->expression);
				if(temp->supfil!=NULL)
				{
					for(z=0,s1=temp->supfil;z<temp->num;++z,++s1)
					{
						if(s1->module!=NULL) Rfree(s1->module);
						if(s1->filename!=NULL) Rfree(s1->filename);
						if(s1->cmodule!=NULL) Rfree(s1->cmodule);
						if(s1->cfilename!=NULL) Rfree(s1->cfilename);
						if(s1->confld!=NULL)
						{
							for(w=0,c1=s1->confld;w<s1->num;++w,++c1)
							{
								if(c1->fldname!=NULL) Rfree(c1->fldname);
								if(c1->conname!=NULL) Rfree(c1->conname);
							}
							Rfree(s1->confld);
						} 
						if(s1->subkeyname!=NULL) Rfree(s1->subkeyname);
						if(s1->expression!=NULL) Rfree(s1->expression);
					}
					Rfree(temp->supfil);
				} 
				if(temp->chgflds!=NULL)
				{
					for(z=0,fld1=temp->chgflds;z<temp->numflds;
						++z,++fld1)
					{
						if(fld1->fieldname!=NULL) 
							Rfree(fld1->fieldname);
					}
					Rfree(temp->chgflds);
				}
			}	
		}
		Rfree(tempchgfile);
		--numchgfile;
		tempchgfile=Rmalloc(numchgfile*sizeof(CHGfile));
		for(x=0,temp1=tempchgfile,temp=new_fields;x<numchgfile;++x,
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
			if(temp->keyname!=NULL)
			{
				temp1->keyname=stralloc(temp->keyname);
				Rfree(temp->keyname);
			} else temp1->keyname=NULL;
			if(temp->expression!=NULL)
			{
				temp1->expression=stralloc(temp->expression);
				Rfree(temp->expression);
			} else temp1->expression=NULL;
			if(temp->supfil!=NULL)
			{
				temp1->num=temp->num;
				temp1->supfil=Rmalloc(temp1->num*sizeof(RDAsearch));
				for(z=0,s1=temp->supfil,s2=temp1->supfil;
					z<temp1->num;++z,++s1,++s2)
				{
					if(s1->module!=NULL)
					{
						s2->module=stralloc(s1->module);
						Rfree(s1->module);
					} else s2->module=NULL;
					if(s1->filename!=NULL)
					{
						s2->filename=stralloc(s1->filename);
						Rfree(s1->filename);
					} else s2->filename=NULL;
					s2->ctype=s1->ctype;
					if(s1->cmodule!=NULL)
					{
						s2->cmodule=stralloc(s1->cmodule);
						Rfree(s1->cmodule);
					} else s2->cmodule=NULL;
					if(s1->cfilename!=NULL)
					{
						s2->cfilename=stralloc(s1->cfilename);
						Rfree(s1->cfilename);
					} else s2->cfilename=NULL;
					if(s1->confld!=NULL)
					{
						s2->num=s1->num;
						s2->confld=Rmalloc(s2->num*sizeof(RDAconfld));
						for(w=0,c1=s1->confld,c2=s2->confld;w<s1->num;++w,++c1,++c2)
						{
							if(c1->fldname!=NULL)
							{
								c2->fldname=stralloc(c1->fldname);
								Rfree(c1->fldname);
							} else c2->fldname=NULL;
							if(c1->conname!=NULL)
							{
								c2->conname=stralloc(c1->conname);
								Rfree(c1->conname);
							} else c2->conname=NULL;
						}
						Rfree(s1->confld);
					} else {
						s2->confld=NULL;
						s2->num=0;
					}
					if(s1->subkeyname!=NULL)
					{
						s2->subkeyname=stralloc(s1->subkeyname);
						Rfree(s1->subkeyname);
					} else s2->subkeyname=NULL;
					s2->range=s1->range;
					s2->num_writes=s1->num_writes;
					if(s1->expression!=NULL)
					{
						s2->expression=stralloc(s1->expression);
						Rfree(s1->expression);
					} else s2->expression=NULL;
					s2->fileno=s1->fileno;
					s2->keyno=s1->keyno;
					s2->occurrence=s1->occurrence;
					s2->writable=s1->writable;
				}
				Rfree(temp->supfil);
			} else {
				temp1->supfil=NULL;
				temp1->num=0;
			}
			if(temp->chgflds!=NULL)
			{
				temp1->numflds=temp->numflds;
				temp1->chgflds=Rmalloc(temp1->numflds*sizeof(CHGfield));
				for(z=0,fld1=temp->chgflds,fld2=temp1->chgflds;z<temp1->numflds;++z,++fld1,++fld2)
				{
					fld2->keypart=fld1->keypart;
					if(fld1->fieldname!=NULL)
					{
						fld2->fieldname=stralloc(fld1->fieldname);
						Rfree(fld1->fieldname);
					} else fld2->fieldname=NULL;
				}
				Rfree(temp->chgflds);
			} else {
				temp1->chgflds=NULL;
				temp1->numflds=0;
			}
			temp1->fileno=temp->fileno;
			temp1->keyno=temp->keyno;
			temp1->prev=temp->prev;
			temp1->parent=temp->parent;
		}
		Rfree(new_fields);
	} else if(tempchgfile!=NULL)
	{
		for(x=0,temp=tempchgfile;x<numchgfile;++x,++temp)
		{
			if(temp->module!=NULL) Rfree(temp->module);
			if(temp->filename!=NULL) Rfree(temp->filename);
			if(temp->keyname!=NULL) Rfree(temp->keyname);
			if(temp->expression!=NULL) Rfree(temp->expression);
			if(temp->supfil!=NULL)
			{
				for(z=0,s1=temp->supfil;z<temp->num;++z,++s1)
				{
					if(s1->module!=NULL) Rfree(s1->module);
					if(s1->filename!=NULL) Rfree(s1->filename);
					if(s1->cmodule!=NULL) Rfree(s1->cmodule);
					if(s1->cfilename!=NULL) Rfree(s1->cfilename);
					if(s1->confld!=NULL)
					{
						for(w=0,c1=s1->confld;w<s1->num;++w,++c1)
						{
							if(c1->fldname!=NULL) Rfree(c1->fldname);
							if(c1->conname!=NULL) Rfree(c1->conname);
						}
						Rfree(s1->confld);
					} 
					if(s1->subkeyname!=NULL) Rfree(s1->subkeyname);
					if(s1->expression!=NULL) Rfree(s1->expression);
				}
				Rfree(temp->supfil);
			} 
			if(temp->chgflds!=NULL)
			{
				for(z=0,fld1=temp->chgflds;z<temp->numflds;
					++z,++fld1)
				{
					if(fld1->fieldname!=NULL) 
						Rfree(fld1->fieldname);
				}
				Rfree(temp->chgflds);
			}
		}
		Rfree(tempchgfile);
		numchgfile=0;
	}
	getchgfiles();
	if(value>=chgfilesavl->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(chgrsrc,"CHANGE FILES",value,chgfilesavl))
		updatersrc(chgrsrc,"CHANGE FILES");
	changedchgfiles=TRUE;
}
void DFchgcopy(RDArsrc *chgrsrc,RDArsrc *mtnrsrc)
{
	int selected=0;
	CHGfile *subs;
	char *modx=NULL;

	if(FINDRSCGETINT(chgrsrc,"CHANGE FILES",&selected)) return;
	subs=tempchgfile+selected;
	selected+=2;
	if(subs!=NULL)
	{
		chgfilescreen(chgrsrc,selected,subs->module,subs->filename,
			subs->keyname,subs->expression,subs->num,subs->supfil,
			subs->numflds,subs->chgflds,savemsubobelow,quitmsubobelowtest);
	} else {
		readwidget(mtnrsrc,"MODULE");
		FINDRSCGETSTRING(mtnrsrc,"MODULE",&modx);
		chgfilescreen(chgrsrc,selected,modx,NULL,NULL,NULL,0,NULL,0,NULL,
			savemsubobelow,quitmsubobelowtest);
		if(modx!=NULL) Rfree(modx);
	}
}
void chgedit(RDArsrc *chgrsrc,RDArsrc *mtnrsrc)
{
	int selected=0;
	CHGfile *subs;
	char *modx=NULL;

	if(FINDRSCGETINT(chgrsrc,"CHANGE FILES",&selected)) return;
	subs=tempchgfile+selected;
	if(subs!=NULL)
	{
		chgfilescreen(chgrsrc,++selected,subs->module,subs->filename,
			subs->keyname,subs->expression,subs->num,subs->supfil,
			subs->numflds,subs->chgflds,savemsubo,quitmsubotest);
	} else {
		readwidget(mtnrsrc,"MODULE");
		FINDRSCGETSTRING(mtnrsrc,"MODULE",&modx);
		chgfilescreen(chgrsrc,++selected,modx,NULL,NULL,NULL,0,NULL,0,NULL,
			savemsubo,quitmsubotest);
		if(modx!=NULL) Rfree(modx);
	}
}
void savechg(void)
{
	int x,z,w;
	CHGfile *temp,*temp1;
	RDAsearch *s1=NULL,*s2=NULL;
	RDAconfld *c1=NULL,*c2=NULL;
	CHGfield *fld1,*fld2;

	if(CHGSTRUCT->chgfiles!=NULL)
	{
		for(x=0,temp=CHGSTRUCT->chgfiles;x<CHGSTRUCT->numfiles;++x,++temp)
		{
			if(temp->module!=NULL) Rfree(temp->module);
			if(temp->filename!=NULL) Rfree(temp->filename);
			if(temp->keyname!=NULL) Rfree(temp->keyname);
			if(temp->expression!=NULL) Rfree(temp->expression);
			if(temp->supfil!=NULL)
			{
				for(z=0,s1=temp->supfil;z<temp->num;++z,++s1)
				{
					if(s1->module!=NULL) Rfree(s1->module);
					if(s1->filename!=NULL) Rfree(s1->filename);
					if(s1->cmodule!=NULL) Rfree(s1->cmodule);
					if(s1->cfilename!=NULL) Rfree(s1->cfilename);
					if(s1->confld!=NULL)
					{
						for(w=0,c1=s1->confld;w<s1->num;++w,++c1)
						{
							if(c1->fldname!=NULL) Rfree(c1->fldname);
							if(c1->conname!=NULL) Rfree(c1->conname);
						}
						Rfree(s1->confld);
					} 
					if(s1->subkeyname!=NULL) Rfree(s1->subkeyname);
					if(s1->expression!=NULL) Rfree(s1->expression);
				}
				Rfree(temp->supfil);
			} 
			if(temp->chgflds!=NULL)
			{
				for(z=0,fld1=temp->chgflds;z<temp->numflds;
					++z,++fld1)
				{
					if(fld1->fieldname!=NULL) 
						Rfree(fld1->fieldname);
				}
				Rfree(temp->chgflds);
			}
		}
		Rfree(CHGSTRUCT->chgfiles);
		CHGSTRUCT->chgfiles=NULL;
		CHGSTRUCT->numfiles=0;
	}
	if(tempchgfile!=NULL)
	{
		CHGSTRUCT->numfiles=numchgfile;
		CHGSTRUCT->chgfiles=Rmalloc(numchgfile*sizeof(CHGfile));
		for(x=0,temp=tempchgfile,temp1=CHGSTRUCT->chgfiles;x<numchgfile;++x,++temp,
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
			if(temp->keyname!=NULL)
			{
				temp1->keyname=stralloc(temp->keyname);
				Rfree(temp->keyname);
			} else temp1->keyname=NULL;
			if(temp->expression!=NULL)
			{
				temp1->expression=stralloc(temp->expression);
				Rfree(temp->expression);
			} else temp1->expression=NULL;
			if(temp->supfil!=NULL)
			{
				temp1->num=temp->num;
				temp1->supfil=Rmalloc(temp1->num*sizeof(RDAsearch));
				for(z=0,s1=temp->supfil,s2=temp1->supfil;
					z<temp1->num;++z,++s1,++s2)
				{
					if(s1->module!=NULL)
					{
						s2->module=stralloc(s1->module);
						Rfree(s1->module);
					} else s2->module=NULL;
					if(s1->filename!=NULL)
					{
						s2->filename=stralloc(s1->filename);
						Rfree(s1->filename);
					} else s2->filename=NULL;
					s2->ctype=s1->ctype;
					if(s1->cmodule!=NULL)
					{
						s2->cmodule=stralloc(s1->cmodule);
						Rfree(s1->cmodule);
					} else s2->cmodule=NULL;
					if(s1->cfilename!=NULL)
					{
						s2->cfilename=stralloc(s1->cfilename);
						Rfree(s1->cfilename);
					} else s2->cfilename=NULL;
					if(s1->confld!=NULL)
					{
						s2->num=s1->num;
						s2->confld=Rmalloc(s2->num*sizeof(RDAconfld));
						for(w=0,c1=s1->confld,c2=s2->confld;w<s1->num;++w,++c1,++c2)
						{
							if(c1->fldname!=NULL)
							{
								c2->fldname=stralloc(c1->fldname);
								Rfree(c1->fldname);
							} else c2->fldname=NULL;
							if(c1->conname!=NULL)
							{
								c2->conname=stralloc(c1->conname);
								Rfree(c1->conname);
							} else c2->conname=NULL;
						}
						Rfree(s1->confld);
					} else {
						s2->confld=NULL;
						s2->num=0;
					}
					if(s1->subkeyname!=NULL)
					{
						s2->subkeyname=stralloc(s1->subkeyname);
						Rfree(s1->subkeyname);
					} else s2->subkeyname=NULL;
					s2->range=s1->range;
					s2->num_writes=s1->num_writes;
					if(s1->expression!=NULL)
					{
						s2->expression=stralloc(s1->expression);
						Rfree(s1->expression);
					} else s2->expression=NULL;
					s2->fileno=s1->fileno;
					s2->keyno=s1->keyno;
					s2->occurrence=s1->occurrence;
					s2->writable=s1->writable;
				}
				Rfree(temp->supfil);
			} else {
				temp1->supfil=NULL;
				temp1->num=0;
			}
			if(temp->chgflds!=NULL)
			{
				temp1->numflds=temp->numflds;
				temp1->chgflds=Rmalloc(temp1->numflds*sizeof(CHGfield));
				for(z=0,fld1=temp->chgflds,fld2=temp1->chgflds;z<temp1->numflds;++z,++fld1,++fld2)
				{
					fld2->keypart=fld1->keypart;
					if(fld1->fieldname!=NULL)
					{
						fld2->fieldname=stralloc(fld1->fieldname);
						Rfree(fld1->fieldname);
					} else fld2->fieldname=NULL;
				}
				Rfree(temp->chgflds);
			} else {
				temp1->chgflds=NULL;
				temp1->numflds=0;
			}
			temp1->fileno=temp->fileno;
			temp1->keyno=temp->keyno;
			temp1->prev=temp->prev;
			temp1->parent=temp->parent;
		}
		Rfree(tempchgfile);
		numchgfile=0;
	}
}
