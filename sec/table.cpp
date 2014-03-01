/* table.c - RDATable Functions */
#include<sec.hpp>

RDATABLE *Internal_RDATABLES=NULL;
int Internal_num_RDATABLES=0;

RDATABLE *xRDATABLENEW(char *mod,char *name,int line,char *file)
{
	RDATABLE *tmp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtable)
	{
		prterr("DIAG RDATABLENEW at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(RDATABLE));
	if(!isEMPTY(mod)) tmp->module=stralloc(mod);
		else tmp->module=NULL;
	if(!isEMPTY(name)) tmp->name=stralloc(name);
		else tmp->name=NULL;
	tmp->title1=NULL;
	tmp->title2=NULL;
	tmp->desc=NULL;
	tmp->num=0;
	tmp->inclusive=FALSE;
	tmp->values=NULL;
	return(tmp);
}
void xFreeRDATABLE(RDATABLE *t,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagtable)
	{
		prterr("DIAG FreeRDATABLE at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(t!=NULL)
	{
		if(t->module!=NULL) Rfree(t->module);
		if(t->name!=NULL) Rfree(t->name);
		if(t->title1!=NULL) Rfree(t->title1);
		if(t->title2!=NULL) Rfree(t->title2);
		if(t->desc!=NULL) Rfree(t->desc);
		if(t->values!=NULL) Rfree(t->values);
		Rfree(t);
	}
}
short xgetRDATablebin(char *libname,RDATABLE *table,int line,char *file)
{
	short x,test=FALSE,version=0;
	BIN *bin;
	int z;
	TABLEVALUE *v=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtable)
	{
		prterr("DIAG getRDATablebin Attempting to Read RDATable Binary [%s] from RDATable Library [%s] at line [%d] program [%s].",table->name,libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=getlibbin(libname,table->name,FALSE);
	if(bin==NULL) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagtable)
		{
			prterr("Error Attempting to Read RDATable Binary [%s] from RDATable Library [%s] Failed at line [%d] program [%s].",table->name,libname,line,file); 
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(-1);
	}
	x=BINgetshort(bin);
	if(x!=(-1701) && x!=(-1702))
	{
		prterr("Error Invalid version or bad binary for RDATable [%s] in RDATable Binary Library [%s] at line [%d] program [%s].",table->name,libname,line,file);
		BINfree(bin);
		return(-1);
	}
	if(x==(-1702)) version=1;
	else version=0;
	if(table->module!=NULL) Rfree(table->module);
	if(table->name!=NULL) Rfree(table->name);
	test=BINgetshort(bin);
	if(test) table->module=BINgetstring(bin);
		else table->module=NULL;
	test=BINgetshort(bin);
	if(test) table->name=BINgetstring(bin);
		else table->name=NULL;
	test=BINgetshort(bin);
	if(test) table->desc=BINgetstring(bin);
		else table->desc=NULL;
	test=BINgetshort(bin);
	if(test) table->title1=BINgetstring(bin);
		else table->title1=NULL;
	test=BINgetshort(bin);
	if(test) table->title2=BINgetstring(bin);
		else table->title2=NULL;
	if(version>0)
	{
		test=BINgetshort(bin);
		if(test) table->inclusive=TRUE;
			else table->inclusive=FALSE;
	} else table->inclusive=FALSE;
	table->num=BINgetint(bin);
	if(table->num>0)
	{
		table->values=Rmalloc(table->num*sizeof(TABLEVALUE));
		for(z=0,v=table->values;z<table->num;++z,++v)
		{
			v->amount=BINgetdouble(bin);
			v->base=BINgetdouble(bin);
			v->rate=BINgetdouble(bin);
		}
	} else {
		table->num=0;
		table->values=NULL;
	}
	BINfree(bin);
	return(0);
}
short xADVwriteRDATablebin(char *modx,RDATABLE *table,int line,char *file)
{
	char *libname=NULL;
	short x=0;

	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+20);
#ifndef WIN32
	sprintf(libname,"%s/rda/%s.TBL",CURRENTDIRECTORY,(modx!=NULL ? modx:""));
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\%s.TBL",CURRENTDIRECTORY,(modx!=NULL ? modx:""));
#endif
	x=xwriteRDATablebin(libname,table,line,file);
	if(libname!=NULL) Rfree(libname);
	return(x);
}
short xwriteRDATablebin(char *libname,RDATABLE *table,int line,char *file)
{
	BIN *bin;
	int x;
	TABLEVALUE *v=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtable)
	{
		prterr("DIAG writeRDATablebin Writing RDATable [%s] into RDATable Binary Library [%s] at line [%d] program [%s].",(table->name!=NULL ? table->name:""),libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=BINnew();
	BINaddshort(bin,-1702);	/*type identifier*/
	if(!isEMPTY(table->module))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,table->module);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(table->name))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,table->name);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(table->desc))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,table->desc);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(table->title1))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,table->title1);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(table->title2))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,table->title2);
	} else BINaddshort(bin,FALSE);
	BINaddshort(bin,(table->inclusive ? TRUE:FALSE));
	if(table->values!=NULL && table->num>0)
	{
		BINaddint(bin,table->num);
		for(x=0,v=table->values;x<table->num;++x,++v)
		{
			BINadddouble(bin,v->amount);
			BINadddouble(bin,v->base);
			BINadddouble(bin,v->rate);
		}
	} else {
		BINaddint(bin,0);
	}
	if(writelibbin(libname,bin,table->name))
	{
		prterr("Error Can't write RDATable binary [%s] into RDATable library [%s] at line [%d] program [%s].",(table->name!=NULL ? table->name:""),libname,line,file);
		return(-1);
	}
	return(0);
}
short xgetRDATabledesc(char *libname,char *name,char **desc,char **title1,
	char **title2,int line,char *file)
{
	short x,test=FALSE;
	BIN *bin;
	char *temp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtable)
	{
		prterr("DIAG getRDATabledesc Attempting to Read RDATable description  for [%s] from RDATable Library [%s] at line [%d] program [%s].",name,libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=getlibbin(libname,name,FALSE);
	if(bin==NULL) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagtable)
		{
			prterr("Error Attempting to Read RDATable Binary [%s] from RDATable Library [%s] Failed at line [%d] program [%s].",name,libname,line,file); 
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(-1);
	}
	x=BINgetshort(bin);
	if(x!=(-1701) && x!=(-1702))
	{
		prterr("Error Invalid version or bad binary for RDATable [%s] in RDATable Binary Library [%s] at line [%d] program [%s].",name,libname,line,file);
		BINfree(bin);
		return(-1);
	}
	test=BINgetshort(bin);
	if(test) 
	{
		temp=BINgetstring(bin);
		if(temp!=NULL) Rfree(temp);
	}
	test=BINgetshort(bin);
	if(test) 
	{
		temp=BINgetstring(bin);
		if(temp!=NULL) Rfree(temp);
	}
	if(*desc!=NULL) Rfree(*desc);
	test=BINgetshort(bin);
	if(test) *desc=BINgetstring(bin);
		else *desc=NULL;
	if(*title1!=NULL) Rfree(*title1);
	test=BINgetshort(bin);
	if(test) *title1=BINgetstring(bin);
		else *title1=NULL;
	if(*title2!=NULL) Rfree(*title2);
	test=BINgetshort(bin);
	if(test) *title2=BINgetstring(bin);
		else *title2=NULL;
	BINfree(bin);
	return(0);
}
double xComputeRDATable(RDATABLE *t,short which,double search_value,
	void (*SubFunc)(...),void *args,int line,char *file)
{
	double value=0,toamt=0;
	int x;
	TABLEVALUE *v,*v1;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtable)
	{
		prterr("DIAG ComputeRDATAble [%f] at line [%d] program [%s].",search_value,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(t!=NULL)
	{
		for(x=0,v=t->values;x<t->num;++x,++v)
		{
			if(((t->inclusive==FALSE) && (search_value<v->amount)) || ((t->inclusive==TRUE) && (search_value<=v->amount)))
			{
				switch(which)
				{
					case RDATABLEBASE:
						value=v->base;
#ifdef USE_RDA_DIAGNOSTICS
						if(diagtable)
						{
							prterr("DIAG ComputeRDATable Returning Base [%f] ",v->base);
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
						break;
					case RDATABLERATE:
						value=v->rate;
#ifdef USE_RDA_DIAGNOSTICS
						if(diagtable)
						{
							prterr("DIAG ComputeRDATable Returning Rate [%f] ",v->rate);
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
						break;
					default:
					case RDATABLECOMP:
						if(x) 
						{
							v1=t->values+(x-1);
							toamt=v1->amount;
						} else toamt=0.0;
						value=(v->base+((search_value-toamt)*v->rate));
#ifdef USE_RDA_DIAGNOSTICS
						if(diagtable)
						{
							prterr("DIAG ComputeRDATable Returning Computation [%f]",value);
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
						break;
				}
				break;
			}
		}
	}
	return(value);
}
double xCOMPUTERDATABLE(char *expression,void (*SubFunc)(...),void *args,
	short which,int line,char *file)
{
	RDATABLE *t=NULL;
	int x=0;
	char *module=NULL,*name=NULL,*libname=NULL,*amtstring=NULL;
	double amt=0.0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtable)
	{
		prterr("DIAG COMPUTERDATABLE at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(isEMPTY(expression))
	{
		prterr("No [Module][Table Name] to use......");
		return(0);
	} else {
		module=stripmodulename(expression);
		name=stripfilename(expression);
		amtstring=stripfieldname(expression);
		if(!isEMPTY(amtstring))
		{
			amt=atof(amtstring);
		} else amt=0;
	}
	if(isEMPTY(module) || isEMPTY(name))
	{
		prterr("Error Module or Table name is NULL.");
		if(module!=NULL) Rfree(module);
		if(name!=NULL) Rfree(name);
		if(amtstring!=NULL) Rfree(amtstring);
		return(0);
	}
	if(Internal_RDATABLES!=NULL)
	{
		for(x=0,t=Internal_RDATABLES;x<Internal_num_RDATABLES;++x,++t)
		{
			if(!RDAstrcmp(t->module,module) && !RDAstrcmp(t->name,name)) break;
		}
		if(x>=Internal_num_RDATABLES)
		{
			x=0;
			t=NULL;
		}
	}
	if(t==NULL)
	{
		if(Internal_RDATABLES!=NULL)
		{
			Internal_RDATABLES=Rrealloc(Internal_RDATABLES,(Internal_num_RDATABLES+1)*sizeof(RDATABLE));
		} else {
			Internal_RDATABLES=Rmalloc(sizeof(RDATABLE));
			Internal_num_RDATABLES=0;
		}
		t=Internal_RDATABLES+Internal_num_RDATABLES;
		if(!isEMPTY(module)) t->module=stralloc(module);
			else t->module=NULL;
		if(!isEMPTY(name)) t->name=stralloc(name);
			else t->name=NULL;
		t->title1=NULL;
		t->title2=NULL;
		t->desc=NULL;
		t->num=0;
		t->values=NULL;
		libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
#ifndef WIN32
		sprintf(libname,"%s/rda/%s.TBL",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\rda\\%s.TBL",CURRENTDIRECTORY,module);
#endif
		getRDATablebin(libname,t);
		++Internal_num_RDATABLES;
	}
	if(libname!=NULL) Rfree(libname);
	if(module!=NULL) Rfree(module);
	if(name!=NULL) Rfree(name);
	if(amtstring!=NULL) Rfree(amtstring);
	return(xComputeRDATable(t,which,amt,SubFunc,args,__LINE__,__FILE__));
}
void xFreeInternalRDATables(int line,char *file)
{
	RDATABLE *t;
	int y;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtable)
	{
		prterr("DIAG FreeInternalRDATables at line [%d] program [%s].",
			line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(Internal_RDATABLES!=NULL)
	{
		for(y=0,t=Internal_RDATABLES;y<Internal_num_RDATABLES;++y,++t)
		{
			if(t->module!=NULL) Rfree(t->module);
			if(t->name!=NULL) Rfree(t->name);
			if(t->title1!=NULL) Rfree(t->title1);
			if(t->title2!=NULL) Rfree(t->title2);
			if(t->desc!=NULL) Rfree(t->desc);
			if(t->values!=NULL) Rfree(t->values);
		}
		Rfree(Internal_RDATABLES);
		Internal_num_RDATABLES=0;
	}
}
