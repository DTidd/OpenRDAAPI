#include<sec.hpp>
#include <cstdio>
#include <cstdlib>

#ifdef WIN32
#include<io.h>
#endif

void xFreeDFvirtual(DFvirtual *d,int line,char *file)
{
	DFincvir *i;
	int x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual)
	{
		prterr("DIAG FreeDFvirtual at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(d!=NULL)
	{
		if(d->incvir!=NULL)
		{
			for(x=0,i=d->incvir;x<d->num;++x,++i)
			{
				if(i->module!=NULL) Rfree(i->module);
				if(i->file!=NULL) Rfree(i->file);
				if(i->keyname!=NULL) Rfree(i->keyname);
			}
			Rfree(d->incvir);
			d->num=0;
		} 
		if(d->module!=NULL) Rfree(d->module);
		if(d->name!=NULL) Rfree(d->name);
		if(d->expression!=NULL) Rfree(d->expression);
		if(d->desc!=NULL) Rfree(d->desc);
		Rfree(d);
		d=NULL;
	}
}
DFvirtual *DFvirtualNEW(char *module,char *name)
{
	DFvirtual *d=NULL;

	d=Rmalloc(sizeof(DFvirtual));
	d->vtype=0;
	d->num=0;
	d->incvir=NULL;
	if(name!=NULL) d->name=stralloc(name);
		else d->name=NULL;
	if(module!=NULL) d->module=stralloc(module);
		else d->module=NULL;
	d->type=0;
	d->avail2rsrc=FALSE;
	d->length=0;
	d->when=0;
	d->desc=NULL;
	d->expression=NULL;
	return(d);
}
DFvirtual *xgetDFvirtual(char *module,char *name,int line,char *file)
{
	BIN *mainbin=NULL;
	char *fname=NULL,*fname1=NULL,*temp=NULL;
	short x,test=0,version=FALSE;
	int y;
	DFvirtual *d;
	DFincvir *i;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field)
	{
		prterr("DIAG Getting Virtual Field [%s] [%s] from Binary Library [%s.VIR] at line [%d] file [%s].",module,name,module,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	fname=Rmalloc(RDAstrlen(module)+10+RDAstrlen(CURRENTDIRECTORY));
	fname1=adddashes(module);
#ifndef WIN32
	sprintf(fname,"%s/rda/%s.VIR",CURRENTDIRECTORY,fname1);
#endif
#ifdef WIN32
	sprintf(fname,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,fname1);
#endif
	if(fname1!=NULL) Rfree(fname1);
	if(access(fname,04))
	{
		prterr("Error in permissions, user cannot read the Virtual Field binary library [%s] at line [%d] program [%s].",fname,line,file);
		if(fname!=NULL) Rfree(fname);
		return(NULL);
	}
	mainbin=getlibbin(fname,name,TRUE);
	if(mainbin==NULL)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagvirtual || diagvirtual_field)
		{
			prterr("Error Virtual Field [%s] not found in [%s.VIR] or Library [%s.VIR] doesn't exist at line [%d] program [%s].",name,module,module,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		if(fname!=NULL) Rfree(fname);
		return(NULL);
	}
	x=BINgetshort(mainbin);
	if(fname!=NULL) Rfree(fname);
	if(x!=(-1701) && x!=(-1700) && x!=(-1699))
	{
		prterr("Error Invalid version or bad binary in Library [%s.VIR] at line [%d] program [%s].",module,line,file);
		BINfree(mainbin);
		return(NULL);
	}
	if(x==(-1699)) version=2;
		else if(x==(-1700)) version=TRUE;
		else version=FALSE;
	d=DFvirtualNEW(module,name);
	test=BINgetshort(mainbin);
	if(test)
	{
		temp=BINgetstring(mainbin);
		if(temp!=NULL) Rfree(temp);
	} 
	d->type=BINgetshort(mainbin);
	if(version>1) d->length=BINgetint(mainbin);
		else d->length=BINgetshort(mainbin);
	d->when=BINgetshort(mainbin);
	test=BINgetshort(mainbin);
	if(test) d->desc=BINgetstring(mainbin);
		else d->desc=NULL;
	if(version)
	{
		d->avail2rsrc=BINgetshort(mainbin);
	} else d->avail2rsrc=TRUE;
	test=BINgetshort(mainbin);
	if(test) d->expression=BINgetstring(mainbin);
		else d->expression=NULL;
	d->vtype=BINgetint(mainbin);
	d->num=BINgetshort(mainbin);
	if(d->num>0)
	{
		d->incvir=Rmalloc(d->num*sizeof(DFincvir));
		for(y=0,i=d->incvir;y<d->num;++y,++i)
		{
			test=BINgetshort(mainbin);
			if(test) i->module=BINgetstring(mainbin);
				else i->module=NULL;
			test=BINgetshort(mainbin);
			if(test) i->file=BINgetstring(mainbin);
				else i->file=NULL;
			i->keyname=NULL;
		}
	}
	if(mainbin!=NULL) BINfree(mainbin);
	return(d);
}
short xwriteDFvirtual(char *module,DFvirtual *d,int line,char *file)
{
	int y,fd=0;
	BIN *bin;
	char *fname=NULL,*fname1=NULL;
	DFincvir *i;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field)
	{
		prterr("DIAG writeDFvirtual Writing Virtual Field Defintion [%s] [%s] to Binary Library at line [%d] file [%s].",(module!=NULL ? module:""),(d->name!=NULL ? d->name:""),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	fname=Rmalloc(RDAstrlen(module)+10+RDAstrlen(CURRENTDIRECTORY));
	fname1=adddashes(module);
#ifndef WIN32
	sprintf(fname,"%s/rda/%s.VIR",CURRENTDIRECTORY,fname1);
#endif
#ifdef WIN32
	sprintf(fname,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,fname1);
#endif
	if(fname1!=NULL) Rfree(fname1);
#ifndef WIN32
	fd=open(fname,O_RDONLY,0666);
#endif
#ifdef WIN32
	fd=open(fname,O_RDONLY|O_BINARY,0666);
#endif
	if(fd!=(-1))
	{
		if(close(fd)==(-1)) {prterr("Error Can't close file(%s).",fname);}
		if(access(fname,02))
		{
			prterr("Error in permissions, user can't write report file [%s] at line [%d] program [%s].",fname,line,file);
			if(fname!=NULL) Rfree(fname);
			return(-1);
		}
	}
	bin=BINnew();
	BINaddshort(bin,-1699);	/*type identifier*/
	if(!isEMPTY(d->name))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,d->name);
	} else BINaddshort(bin,FALSE);
	BINaddshort(bin,d->type);
	BINaddint(bin,d->length);
	BINaddshort(bin,d->when);
	if(!isEMPTY(d->desc))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,d->desc);
	} else BINaddshort(bin,FALSE);
	BINaddshort(bin,d->avail2rsrc);
	if(!isEMPTY(d->expression))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,d->expression);
	} else BINaddshort(bin,FALSE);
	BINaddint(bin,d->vtype);
	BINaddshort(bin,d->num);
	if(d->num>0)
	{
		for(y=0,i=d->incvir;y<d->num;++y,++i)
		{
			if(!isEMPTY(i->module))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,i->module);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(i->file))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,i->file);
			} else BINaddshort(bin,FALSE);
		}
	}
	if(writelibbin(fname,bin,d->name))
	{
		prterr("Error Can't write Default binary [%s] into library [%s] at line [%d] program [%s].",(d->name!=NULL ? d->name:""),fname,line,file);
		if(fname!=NULL) Rfree(fname);
		return(-1);
	}
	if(fname!=NULL) Rfree(fname);
	return(0);
}
char *xgetvirtualdesc(char *module,char *name,int line,char *file)
{
	BIN *mainbin=NULL;
	char *temp=NULL,*fname=NULL,*fname1=NULL,*hold=NULL;
	short x,test=0,version=FALSE;
	DFvirtual *d=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field)
	{
		prterr("DIAG getvirtualdesc Getting Virtual Field Description [%s] [%s] from binary at line [%d] program [%s].",module,name,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	fname=Rmalloc(RDAstrlen(module)+10+RDAstrlen(CURRENTDIRECTORY));
	fname1=adddashes(module);
#ifndef WIN32
	sprintf(fname,"%s/rda/%s.VIR",CURRENTDIRECTORY,fname1);
#endif
#ifdef WIN32
	sprintf(fname,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,fname1);
#endif
	if(fname1!=NULL) Rfree(fname1);
	if(access(fname,04))
	{
		prterr("Error in permissions, user cannot read the Virtual Field Binary Library [%s] at line [%d] program [%s].",fname,line,file);
		return(NULL);
	}
	mainbin=getlibbin(fname,name,TRUE);
	if(mainbin==NULL)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagvirtual || diagvirtual_field)
		{
			prterr("Error Virtual Field [%s] not found in [%s.VIR] or Library [%s.VIR] doesn't exist at line [%d] program [%s].",name,module,module,line,file);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(NULL);
	}
	x=BINgetshort(mainbin);
	if(x!=(-1701) && x!=(-1700) && x!=(-1699))
	{
		prterr("Error Invalid version or bad binary in Library [%s.VIR] at line [%d] program [%s].",module,line,file);
		BINfree(mainbin);
		return(NULL);
	}
	if(x==(-1699)) version=2;
		else if(x==(-1700)) version=TRUE;
		else version=FALSE;
	d=DFvirtualNEW(module,name);
	test=BINgetshort(mainbin);
	if(test)
	{
		temp=BINgetstring(mainbin);
		if(temp!=NULL) Rfree(temp);
	} 
	d->type=BINgetshort(mainbin);
	if(version>1) d->length=BINgetint(mainbin);
		else d->length=BINgetshort(mainbin);
	d->when=BINgetshort(mainbin);
	test=BINgetshort(mainbin);
	if(test) hold=BINgetstring(mainbin);
		else hold=NULL;
	BINfree(mainbin);
	if(fname!=NULL) Rfree(fname);
	FreeDFvirtual(d);
	return(hold);
}
int xFINDVIRTUAL(RDArsrc *r,char *name,int line,char *file)
{
	int x;
	RDAvirtual *v;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field)
	{
		prterr("DIAG FINDVIRTUAL Find Virtual Field [%s] on Screen [%s] [%s] at line [%d] program [%s].",name,r->module,r->screen,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(r->virflds!=NULL)
	{
		for(x=0,v=r->virflds;x<r->num;++x,++v)
		{
			if(!RDAstrcmp(v->name,name)) return(x);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field)
	{
		prterr("Error Virtual Field  [%s] is not available on Screen [%s] [%s] at line [%d] program [%s].",name,r->module,r->screen,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(-1);
}
int xFINDSCROLLLIST(RDArsrc *r,char *name,int line,char *file)
{
	int x;
	RDAScrolledList *list=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field)
	{
		prterr("DIAG FINDSCROLLLIST Find Scrolled List [%s] on Screen [%s] [%s] at line [%d] program [%s].",name,r->module,r->screen,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(r->lists!=NULL)
	{
		for(x=0,list=r->lists;x<r->numlists;++x,++list)
		{
			if(!RDAstrcmp(list->name,name)) return(x);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field)
	{
		prterr("Error Scrolled List  [%s] is not available on Screen [%s] [%s] at line [%d] program [%s].",name,r->module,r->screen,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(-1);
}
