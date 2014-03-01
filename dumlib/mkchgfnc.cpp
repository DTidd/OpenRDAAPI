/* mkchgcd.c - Change Database Files Make Functions */
#include<mkchg.hpp>

short xaddChangeFileSupportingRDAconfld(RDAsearch *sup,char *fldname,char *conname,char inherit,int line,char *file)
{
	RDAconfld *c=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG addChangeSubordinateRDAconfld at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(sup->confld!=NULL)
	{
		sup->confld=Rrealloc(sup->confld,(sup->num+1)*sizeof(RDAconfld));
	} else {
		sup->confld=Rmalloc(sizeof(RDAconfld));
		sup->num=0;
	}
	c=sup->confld+sup->num;
	c->fldname=stralloc(fldname);
	c->conname=stralloc(conname);
	c->inherit=inherit;
	return(++sup->num);
}
int xaddChangeFileSupporting(CHGfile *chgfil,char *module,
	char *filename,int ctype,short numconflds,RDAconfld *conflds,
	char *cmodule,char *cfilename,char *subkeyname,short range,
	char *expression,char browse,char *button_name,char *browse_screen,
	char *define_list,char *search_browse,
	int line,char *file)
{
	RDAsearch *subord=NULL;
	RDAconfld *confld1=NULL,*confld2=NULL;
	int x=0;

	if(chgfil!=NULL)
	{
		if(chgfil->supfil!=NULL)
		{
			chgfil->supfil=Rrealloc(chgfil->supfil,(chgfil->num+1)*sizeof(RDAsearch));
		} else {
			chgfil->supfil=Rmalloc(sizeof(RDAsearch));
			chgfil->num=0;
		}
		subord=chgfil->supfil+chgfil->num;
		if(!isEMPTY(module))
		{
			subord->module=stralloc(module);
		} else subord->module=NULL;
		if(!isEMPTY(filename))
		{
			subord->filename=stralloc(filename);
		} else subord->filename=NULL;
		if(!isEMPTY(cmodule))
		{
			subord->cmodule=stralloc(cmodule);
		} else subord->cmodule=NULL;
		if(!isEMPTY(cfilename))
		{
			subord->cfilename=stralloc(cfilename);
		} else subord->cfilename=NULL;
		if(!isEMPTY(subkeyname))
		{
			subord->subkeyname=stralloc(subkeyname);
		} else subord->subkeyname=NULL;
		subord->range=range;
		subord->ctype=ctype;
		subord->browse=browse;
		if(!isEMPTY(button_name))
		{
			subord->button_name=stralloc(button_name);
		} else subord->button_name=NULL;
		if(!isEMPTY(browse_screen))
		{
			subord->browse_screen=stralloc(browse_screen);
		} else subord->browse_screen=NULL;
		if(!isEMPTY(define_list))
		{
			subord->define_list=stralloc(define_list);
		} else subord->define_list=NULL;
		if(!isEMPTY(search_browse))
		{
			subord->search_browse=stralloc(search_browse);
		} else subord->search_browse=NULL;
		if(!isEMPTY(expression))
		{
			subord->expression=stralloc(expression);
		} else subord->expression=NULL;
		if(conflds!=NULL && numconflds>0)
		{		
			subord->confld=Rmalloc(numconflds*sizeof(RDAconfld));
			for(x=0,confld1=conflds,confld2=subord->confld;x<numconflds;++x,++confld1,++confld2)
			{
				if(!isEMPTY(confld1->fldname))
				{
					confld2->fldname=stralloc(confld1->fldname);
				} else confld2->fldname=NULL;
				if(!isEMPTY(confld1->conname))
				{
					confld2->conname=stralloc(confld1->conname);
				} else confld2->conname=NULL;
				confld2->inherit=confld1->inherit;
			}
			subord->num=numconflds;
		} else {
			subord->confld=NULL;
			subord->num=0;
		}
		return(++chgfil->num);
	} else {
		return(-1);
	}
}


int xaddChangeField(CHGfile *chgfile,char keypart,char *fieldname,int line,char *file)
{
	CHGfield *chgfld=NULL;

	if(chgfile!=NULL)
	{
		if(chgfile->chgflds!=NULL)
		{
			chgfile->chgflds=Rrealloc(chgfile->chgflds,(chgfile->numflds+1)*sizeof(CHGfield));
		} else {
			chgfile->chgflds=Rmalloc(sizeof(CHGfield));
			chgfile->numflds=0;
		}
		chgfld=chgfile->chgflds+chgfile->numflds;
		chgfld->keypart=keypart;
		if(!isEMPTY(fieldname))
		{
			chgfld->fieldname=stralloc(fieldname);
		} else chgfld->fieldname=NULL;
		return(++chgfile->numflds);
	} else {
		return(-1);
	}
}

int xaddChangeFile(CHGstruct *CHGDEF,
	char *module,char *filename,char *keyname,char *expression,
	int numflds,CHGfield *chgflds,int num,RDAsearch *supfils,
	int line,char *file)
{
	CHGfile *chgfile=NULL;
	CHGfield *chgfld1=NULL,*chgfld2=NULL;
	RDAsearch *supfil1=NULL,*supfil2=NULL;
	RDAconfld *confld1=NULL,*confld2=NULL;
	int x=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG xaddChangeFile(%s,module=%s,filename=%s,keyname=%s,expression=%s,numflds(bf)=%d,%s,line %d,file %s.",
			(CHGDEF!=NULL?"NOT NULL CHGDEF":"NULL CHGDEF"),
			(module!=NULL?module:""),
			(filename!=NULL?filename:""),
			(keyname!=NULL?keyname:""),
			(expression!=NULL?expression:""),
			numflds,(chgflds!=NULL?"NOT NULL chgflds":"NULL chgflds"),
			line,(file!=NULL?file:""));
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(CHGDEF!=NULL)
	{
		if(CHGDEF->chgfiles!=NULL && CHGDEF->numfiles>0)

		{
			CHGDEF->chgfiles=Rrealloc(CHGDEF->chgfiles,(CHGDEF->numfiles+1)*sizeof(CHGfile));
		} else {
			CHGDEF->chgfiles=Rmalloc(sizeof(CHGfile));
			CHGDEF->numfiles=0;
		}
		chgfile=CHGDEF->chgfiles+CHGDEF->numfiles;
		if(!isEMPTY(module))
		{
			chgfile->module=stralloc(module);
		} else chgfile->module=NULL;
		if(!isEMPTY(filename))
		{
			chgfile->filename=stralloc(filename);
		} else chgfile->filename=NULL;
		if(!isEMPTY(keyname))
		{
			chgfile->keyname=stralloc(keyname);
		} else chgfile->keyname=NULL;
		if(!isEMPTY(expression))
		{
			chgfile->expression=stralloc(expression);
		} else chgfile->expression=NULL;
		if(chgflds!=NULL && numflds>0)
		{
			chgfile->chgflds=Rmalloc(numflds*sizeof(CHGfile));
			for(x=0,chgfld1=chgflds,chgfld2=chgfile->chgflds;x<numflds;++x,++chgfld1,++chgfld2)
			{
				chgfld2->keypart=chgfld1->keypart;
				if(!isEMPTY(chgfld1->fieldname))
				{
					chgfld2->fieldname=stralloc(chgfld1->fieldname);
				} else chgfld2->fieldname=NULL;
			}
			chgfile->numflds=numflds;
		} else {
			chgfile->chgflds=NULL;
			chgfile->numflds=0;
		}
		if(supfils!=NULL && num>0)
		{
			chgfile->supfil=Rmalloc(num*sizeof(RDAsearch));
			for(x=0,supfil1=supfils,supfil2=chgfile->supfil;x<num;++x,++supfil1,++supfil2)
			{
				if(!isEMPTY(supfil1->module))
				{
					supfil2->module=stralloc(supfil1->module);
				} else supfil2->module=NULL;
				if(!isEMPTY(supfil1->filename))
				{
					supfil2->filename=stralloc(supfil1->filename);
				} else supfil2->filename=NULL;
				if(!isEMPTY(supfil1->subkeyname))
				{
					supfil2->subkeyname=stralloc(supfil1->subkeyname);
				} else supfil2->subkeyname=NULL;
				supfil2->ctype=supfil1->ctype;
				supfil2->browse=supfil1->browse;
				if(!isEMPTY(supfil1->button_name))
				{
					supfil2->button_name=stralloc(supfil1->button_name);
				} else supfil2->button_name=NULL;
				if(!isEMPTY(supfil1->browse_screen))
				{
					supfil2->browse_screen=stralloc(supfil1->browse_screen);
				} else supfil2->browse_screen=NULL;
				if(!isEMPTY(supfil1->define_list))
				{
					supfil2->define_list=stralloc(supfil1->define_list);
				} else supfil2->define_list=NULL;
				if(!isEMPTY(supfil1->search_browse))
				{
					supfil2->search_browse=stralloc(supfil1->search_browse);
				} else supfil2->search_browse=NULL;
				if(!isEMPTY(supfil1->expression))
				{
					supfil2->expression=stralloc(supfil1->expression);
				} else supfil2->expression=NULL;
				if(supfil1->confld!=NULL && supfil1->num>0)
				{		
					supfil2->confld=Rmalloc(supfil1->num*sizeof(RDAconfld));
					for(x=0,confld1=supfil1->confld,confld2=supfil2->confld;x<supfil1->num;++x,++confld1,++confld2)
					{
						if(!isEMPTY(confld1->fldname))
						{
							confld2->fldname=stralloc(confld1->fldname);
						} else confld2->fldname=NULL;
						if(!isEMPTY(confld1->conname))
						{
							confld2->conname=stralloc(confld1->conname);
						} else confld2->conname=NULL;
						confld2->inherit=confld1->inherit;
					}
					supfil2->num=supfil1->num;
				} else {
					supfil2->confld=NULL;
					supfil2->num=0;
				}
			}
			chgfile->num=num;
		} else {
			chgfile->supfil=NULL;
			chgfile->num=0;
		}
		chgfile->fileno=(-1);
		chgfile->keyno=0;
		chgfile->prev=NULL;
		chgfile->parent=NULL;
		chgfile->sort=NULL;
		return(++CHGDEF->numfiles);
	} else {
		return(-1);
	}
}


CHGstruct *xRDACHGstructNEW(char *module,char *name,char *desc,
	char *fieldname,short type,unsigned len,
	char range,char *range_screen,char *expression,
	int numfiles,CHGfile *chgfiles,
	int line,char *file)

{
	CHGstruct *CHGDEF=NULL;
	CHGfile *chgfile=NULL;
	int x=0;

	CHGDEF=CHGstructNEW(module,name);
	if(CHGDEF!=NULL)
	{
		if(!isEMPTY(desc))
		{
			CHGDEF->desc=stralloc(desc);
		} else CHGDEF->desc=NULL;
		if(!isEMPTY(fieldname))
		{
			CHGDEF->fieldname=stralloc(fieldname);
		} else CHGDEF->fieldname=NULL;
		CHGDEF->type=type;
		CHGDEF->len=len;
		CHGDEF->range=range;
		if(!isEMPTY(expression))
		{
			CHGDEF->expression=stralloc(expression);
		} else CHGDEF->expression=NULL;
		if(!isEMPTY(range_screen))
		{
			CHGDEF->range_screen=stralloc(range_screen);
		} else CHGDEF->range_screen=NULL;
		if(chgfiles!=NULL && numfiles>0)
		{
			for(x=0,chgfile=chgfiles;x<numfiles;++x,++chgfile)
			{
				addChangeFile(CHGDEF,chgfile->module,
					chgfile->filename,chgfile->keyname,
					chgfile->expression,chgfile->numflds,
					chgfile->chgflds,chgfile->num,
					chgfile->supfil);
			}
		} else {
			CHGDEF->chgfiles=NULL;
			CHGDEF->numfiles=0;
		}
		return(CHGDEF);
	} else {
		return(NULL);
	}
}
