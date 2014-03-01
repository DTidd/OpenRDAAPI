/* dfchgcm.c - Define / Do Change Common */
#include<dfchg.hpp>

CHGstruct *xCHGstructNEW(char *module,char *name,int line,char *file)
{
	CHGstruct *tmp=NULL;

	if(diagapps)
	{
		prterr("DIAG CHGstructNEW at line [%d] program [%s].",line,file);
	}
	tmp=Rmalloc(sizeof(CHGstruct));
	if(!isEMPTY(module)) tmp->module=stralloc(module);
		else tmp->module=NULL;
	if(!isEMPTY(name)) tmp->name=stralloc(name);
		else tmp->name=NULL;
	tmp->desc=NULL;
	tmp->fieldname=NULL;
	tmp->range=FALSE;
	tmp->range_screen=NULL;
	tmp->expression=NULL;
	tmp->numfiles=0;
	tmp->chgfiles=NULL;
	tmp->SubFunc=NULL;
	tmp->SubFuncArgs=NULL;
	tmp->rsrc=NULL;
	tmp->type=0;
	tmp->len=0;
	tmp->simple_from=FALSE;
	tmp->simple_to=FALSE;
	return(tmp);
}
void xFreeCHGstruct(CHGstruct *c,int line,char *file)
{
	CHGfile *chgfile=NULL;
	CHGfield *chgfield=NULL;
	RDAsearch *s;
	RDAconfld *e;
	int x,y,z;

	if(diagapps)
	{
		prterr("DIAG FreeCHGstruct at line [%d] program [%s].",line,file);
	}
	if(c!=NULL)
	{
		if(c->module!=NULL) Rfree(c->module);
		if(c->name!=NULL) Rfree(c->name);
		if(c->desc!=NULL) Rfree(c->desc);
		if(c->fieldname!=NULL) Rfree(c->fieldname);
		if(c->range_screen!=NULL) Rfree(c->range_screen);
		if(c->expression!=NULL) Rfree(c->expression);
		if(c->chgfiles!=NULL)
		{
			for(x=0,chgfile=c->chgfiles;x<c->numfiles;++x,++chgfile)
			{
				if(chgfile->module!=NULL) Rfree(chgfile->module);
				if(chgfile->filename!=NULL) Rfree(chgfile->filename);
				if(chgfile->keyname!=NULL) Rfree(chgfile->keyname);
				if(chgfile->expression!=NULL) Rfree(chgfile->expression);
				if(chgfile->supfil!=NULL)
				{
					for(y=0,s=chgfile->supfil;y<chgfile->num;++y,++s)
					{
						if(s->module!=NULL) Rfree(s->module);
						if(s->filename!=NULL) Rfree(s->filename);
						if(s->cmodule!=NULL) Rfree(s->cmodule);
						if(s->cfilename!=NULL) Rfree(s->cfilename);
						if(s->subkeyname!=NULL) Rfree(s->subkeyname);
						if(s->confld!=NULL)
						{
							for(z=0,e=s->confld;z<s->num;++z,++e)
							{
								if(e->conname!=NULL) Rfree(e->conname);
								if(e->fldname!=NULL) Rfree(e->fldname);
							}			
							Rfree(s->confld);
						}
						if(s->expression!=NULL) Rfree(s->expression);
					}
					Rfree(chgfile->supfil);
				}
				if(chgfile->chgflds!=NULL)
				{
					for(y=0,chgfield=chgfile->chgflds;y<chgfile->numflds;++y,++chgfield)
					{
						if(chgfield->fieldname!=NULL)
							Rfree(chgfield->fieldname);
					}
					Rfree(chgfile->chgflds);
				}
				if(chgfile->prev!=NULL) FreeRDATData(chgfile->prev);
			}
			Rfree(c->chgfiles);
		}
		Rfree(c);
	}
}
short xgetCHGstruct(char *libname,CHGstruct *c,int line,char *file)
{
	CHGfile *chgfile=NULL;
	CHGfield *chgfield=NULL;
	RDAsearch *s;
	RDAconfld *e;
	int z,y,w;
	short x,version=FALSE,test=0;
	BIN *bin=NULL;

	if(diagapps)
	{
		prterr("DIAG getCHGstruct at line [%d] program [%s].",line,file);
	}
	bin=getlibbin(libname,c->name,FALSE);
	if(bin==NULL)
	{
		if(diagapps)	
		{
			prterr("Error Attempting to Read Change Structure [%s] from Change Library [%s] Failed at line [%d] program [%s].",c->name,libname,line,file);
		}
		return(-1);
	}
	x=BINgetshort(bin);
	if(x!=(-1701) && x!=(-1700))
	{
		prterr("Error Invalid version or bad binary for Change Structure [%s] in Change Library [%s] at line [%d] program [%s].",c->name,libname,line,file);
		BINfree(bin);
		return(-1);
	}
	if(x==(-1700)) version=TRUE;
	else version=FALSE;
	test=BINgetshort(bin);
	if(c->module!=NULL) Rfree(c->module);
	if(test) c->module=BINgetstring(bin);
		else c->module=NULL;
	test=BINgetshort(bin);
	if(c->name!=NULL) Rfree(c->name);
	if(test) c->name=BINgetstring(bin);
		else c->name=NULL;
	test=BINgetshort(bin);
	if(test) c->desc=BINgetstring(bin);
		else c->desc=NULL;
	test=BINgetshort(bin);
	if(test) c->fieldname=BINgetstring(bin);
		else c->fieldname=NULL;
	if(version)
	{
		c->type=BINgetshort(bin);
		c->len=BINgetint(bin);
	} else {
		c->type=0;
		c->len=0;
	}
	test=BINgetshort(bin);
	if(test) c->range=TRUE;
		else c->range=FALSE;
	test=BINgetshort(bin);
	if(test) c->range_screen=BINgetstring(bin);
		else c->range_screen=NULL;
	test=BINgetshort(bin);
	if(test) c->expression=BINgetstring(bin);
		else c->expression=NULL;
	c->simple_from=FALSE;
	c->simple_to=FALSE;
	c->rsrc=NULL;
	c->numfiles=BINgetint(bin);
	if(c->numfiles>0)
	{
		c->chgfiles=Rmalloc(c->numfiles*sizeof(CHGfile));
		for(y=0,chgfile=c->chgfiles;y<c->numfiles;++y,++chgfile)
		{
			test=BINgetshort(bin);
			if(test) chgfile->module=BINgetstring(bin);
				else chgfile->module=NULL;
			test=BINgetshort(bin);
			if(test) chgfile->filename=BINgetstring(bin);
				else chgfile->filename=NULL;
			test=BINgetshort(bin);
			if(test) chgfile->keyname=BINgetstring(bin);
				else chgfile->keyname=NULL;
			test=BINgetshort(bin);
			if(test) chgfile->expression=BINgetstring(bin);
				else chgfile->expression=NULL;
			chgfile->num=BINgetint(bin);
			chgfile->sort=NULL;
			if(chgfile->num>0)
			{
				chgfile->supfil=Rmalloc(chgfile->num*sizeof(RDAsearch));
				for(z=0,s=chgfile->supfil;z<chgfile->num;++z,++s)
				{
					test=BINgetshort(bin);
					if(test) s->module=BINgetstring(bin);
						else s->module=NULL;
					test=BINgetshort(bin);
					if(test) s->filename=BINgetstring(bin);
						else s->filename=NULL;
					s->ctype=BINgetint(bin);
					test=BINgetshort(bin);
					if(test) s->cmodule=BINgetstring(bin);
						else s->cmodule=NULL;
					test=BINgetshort(bin);
					if(test) s->cfilename=BINgetstring(bin);
						else s->cfilename=NULL;
					s->num=BINgetshort(bin);
					if(s->num>0)
					{
						s->confld=Rmalloc(s->num*sizeof(RDAconfld));
						for(w=0,e=s->confld;w<s->num;++w,++e)
						{
							test=BINgetshort(bin);
							if(test) e->fldname=BINgetstring(bin);
								else e->fldname=NULL;
							test=BINgetshort(bin);
							if(test) e->conname=BINgetstring(bin);
								else e->conname=NULL;
						}
					} else s->confld=NULL;
					test=BINgetshort(bin);
					if(test) s->subkeyname=BINgetstring(bin);
						else s->subkeyname=NULL;
					test=BINgetshort(bin);
					if(test) s->range=TRUE;
						else s->range=FALSE;
					s->num_writes=0;
					test=BINgetshort(bin);
					if(test) s->expression=BINgetstring(bin);
						else s->expression=NULL;
					s->fileno=(-1);
					s->keyno=(-1);
					s->writable=(-1);
					s->occurrence=(-1);
				}
			} else chgfile->supfil=NULL;
			chgfile->numflds=BINgetint(bin);
			if(chgfile->numflds>0)
			{
				chgfile->chgflds=Rmalloc(chgfile->numflds*sizeof(CHGfield));
				for(z=0,chgfield=chgfile->chgflds;z<chgfile->numflds;++z,++chgfield)
				{
					test=BINgetshort(bin);
					if(test) chgfield->fieldname=BINgetstring(bin);
						else chgfield=NULL;
					test=BINgetshort(bin);
					if(test) chgfield->keypart=TRUE;
						else chgfield->keypart=FALSE;
				}
			} else chgfile->chgflds=NULL;
			chgfile->prev=NULL;
			chgfile->fileno=(-1);
			chgfile->keyno=(-1);
			chgfile->parent=c;
		}
	} else c->chgfiles=NULL;
	BINfree(bin);
	return(0);
}
short xwriteCHGstruct(char *libname,CHGstruct *c,int line,char *file)
{
	CHGfile *chgfile=NULL;
	CHGfield *chgfield=NULL;
	RDAsearch *s;
	RDAconfld *e;
	int z,y,w;
	BIN *bin=NULL;

	if(diagapps)
	{
		prterr("DIAG writeCHGstruct at line [%d] program [%s].",line,file);
	}
	bin=BINnew();
	BINaddshort(bin,-1700);
	if(c->module!=NULL)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,c->module);
	} else BINaddshort(bin,FALSE);
	if(c->name!=NULL)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,c->name);
	} else BINaddshort(bin,FALSE);
	if(c->desc!=NULL)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,c->desc);
	} else BINaddshort(bin,FALSE);
	if(c->fieldname!=NULL)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,c->fieldname);
	} else BINaddshort(bin,FALSE);
	BINaddshort(bin,c->type);
	BINaddint(bin,c->len);
	BINaddshort(bin,(c->range ? TRUE:FALSE));
	if(c->range_screen!=NULL)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,c->range_screen);
	} else BINaddshort(bin,FALSE);
	if(c->expression!=NULL)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,c->expression);
	} else BINaddshort(bin,FALSE);
	BINaddint(bin,c->numfiles);
	if(c->numfiles>0)
	{
		for(y=0,chgfile=c->chgfiles;y<c->numfiles;++y,++chgfile)
		{
			if(chgfile->module!=NULL)
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,chgfile->module);
			} else BINaddshort(bin,FALSE);
			if(chgfile->filename!=NULL) 
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,chgfile->filename);
			} else BINaddshort(bin,FALSE);
			if(chgfile->keyname!=NULL)
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,chgfile->keyname);
			} else BINaddshort(bin,FALSE);
			if(chgfile->expression!=NULL)
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,chgfile->expression);
			} else BINaddshort(bin,FALSE);
			BINaddint(bin,chgfile->num);
			if(chgfile->num>0)
			{
				for(z=0,s=chgfile->supfil;z<chgfile->num;++z,++s)
				{
					if(s->module!=NULL)
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,s->module);
					} else BINaddshort(bin,FALSE);
					if(s->filename!=NULL)
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,s->filename);
					} else BINaddshort(bin,FALSE);
					BINaddint(bin,s->ctype);
					if(s->cmodule!=NULL)
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,s->cmodule);
					} else BINaddshort(bin,FALSE);
					if(s->cfilename!=NULL)
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,s->cfilename);
					} else BINaddshort(bin,FALSE);
					BINaddshort(bin,s->num);
					if(s->num>0)
					{
	
						for(w=0,e=s->confld;w<s->num;++w,++e)
						{
							if(e->fldname!=NULL)
							{
								BINaddshort(bin,TRUE);
								BINaddstring(bin,e->fldname);
							} else BINaddshort(bin,FALSE);
							if(e->conname!=NULL)
							{
								BINaddshort(bin,TRUE);
								BINaddstring(bin,e->conname);
							} else BINaddshort(bin,FALSE);
						}
					}
					if(s->subkeyname!=NULL)
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,s->subkeyname);
					} else BINaddshort(bin,FALSE);
					BINaddshort(bin,(s->range==TRUE ? TRUE:FALSE));
					if(s->expression!=NULL)
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,s->expression);
					} else BINaddshort(bin,FALSE);
				}
			}
			BINaddint(bin,chgfile->numflds);
			if(chgfile->numflds>0)
			{
				for(z=0,chgfield=chgfile->chgflds;z<chgfile->numflds;++z,++chgfield)
				{
					if(chgfield->fieldname!=NULL)
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,chgfield->fieldname);
					} else BINaddshort(bin,FALSE);
					BINaddshort(bin,(chgfield->keypart ? TRUE:FALSE));
				}
			} 
		}
	}
	if(writelibbin(libname,bin,c->name))
	{
		prterr("Error Can't write Change Structure [%s] into Change Library [%s] at line [%d] program [%s].",(c->name!=NULL ? c->name:""),libname,line,file);
		return(-1);
	}
	return(0);
}
char *xgetCHGstructdesc(char *libname,char *name,int line,char *file)
{
	char *temp=NULL,*tmp=NULL;
	short x,test=0;
	BIN *bin=NULL;

	if(diagapps)
	{
		prterr("DIAG getCHGstruct at line [%d] program [%s].",line,file);
	}
	bin=getlibbin(libname,name,FALSE);
	if(bin==NULL)
	{
		if(diagapps)	
		{
			prterr("Error Attempting to Read Change Structure [%s] from Change Library [%s] Failed at line [%d] program [%s].",name,libname,line,file);
		}
		return(NULL);
	}
	x=BINgetshort(bin);
	if(x!=(-1701) && x!=(-1700))
	{
		prterr("Error Invalid version or bad binary for Change Structure [%s] in Change Library [%s] at line [%d] program [%s].",name,libname,line,file);
		BINfree(bin);
		return(NULL);
	}
	test=BINgetshort(bin);
	if(test) temp=BINgetstring(bin);
		else temp=NULL;
	if(temp!=NULL) Rfree(temp);
	test=BINgetshort(bin);
	if(test) temp=BINgetstring(bin);
		else temp=NULL;
	if(temp!=NULL) Rfree(temp);
	test=BINgetshort(bin);
	if(test) tmp=BINgetstring(bin);
	BINfree(bin);
	return(tmp);
}
