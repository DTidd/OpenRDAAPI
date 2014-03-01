/* mkezfnc.c - Functions used in packaging of EZRPT Definitions */
#include <rptgen.hpp>

short xaddEZincvir(RDAezrpt *EZ,char *module,char *filx,char *keyname,
	int line,char *file)
{
	DFincvir *d=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG addEZincvir at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(EZ!=NULL)
	{
		if(EZ->incvir!=NULL)
		{
			EZ->incvir=Rrealloc(EZ->incvir,(EZ->num+1)*sizeof(DFincvir));					
		} else {
			EZ->incvir=Rmalloc(sizeof(DFincvir));
			EZ->num=0;
		}	
		d=EZ->incvir+EZ->num;
		d->module=stralloc(module);
		d->file=stralloc(filx);
		d->keyname=stralloc(keyname);
		d->v=NULL;
		d->fileno=(-1);
		return(++EZ->num);
	} else {
		prterr("Error:  RDAezrpt is NULL....");
		return(-1);
	}
}
void addEZsortFROMTODOUBLE(RDAezsrt *s,double from,double to)
{
	if(s!=NULL)
	{
		s->from.float_value=Rmalloc(sizeof(double));
		s->to.float_value=Rmalloc(sizeof(double));
		*s->from.float_value=from;
		*s->to.float_value=to;	
	} else {
		prterr("Error: RDAezsrt is NULL.");
	}
}
void addEZsortFROMTOINT(RDAezsrt *s,int from,int to)
{
	if(s!=NULL)
	{
		s->from.integer_value=Rmalloc(sizeof(int));
		s->to.integer_value=Rmalloc(sizeof(int));
		*s->from.integer_value=from;
		*s->to.integer_value=to;
	} else {
		prterr("Error: RDAezsrt is NULL.");
	}
}
void addEZsortFROMTOSHORT(RDAezsrt *s,short from,short to)
{
	if(s!=NULL)
	{
		s->from.short_value=Rmalloc(sizeof(short));
		s->to.short_value=Rmalloc(sizeof(short));
		*s->from.short_value=from;
		*s->to.short_value=to;
	} else {
		prterr("Error: RDAezsrt is NULL.");
	}
}
void addEZsortFROMTOBOOLNS(RDAezsrt *s,char from,char to)
{
	if(s!=NULL)
	{
		s->from.string_value=Rmalloc(1);
		s->to.string_value=Rmalloc(1);
		*s->from.string_value=from;
		*s->to.string_value=to;
	} else {
		prterr("Error: RDAezsrt is NULL.");
	}
}
void addEZsortFROMTOSTRING(RDAezsrt *s,char *from,char *to)
{
	if(s!=NULL)
	{
		s->from.string_value=stralloc(from);
		s->to.string_value=stralloc(to);
	} else {
		prterr("Error: RDAezsrt is NULL.");
	}
}
short xaddEZsort(RDAezrpt *EZ,char *name,short total,short start,
	short length,short type,char range,int line,char *file)
{
	RDAezsrt *s=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG addEZsort at line [%d] program [%s].",line,file);	
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(EZ!=NULL)
	{
		if(EZ->sort!=NULL)
		{
			EZ->sort=Rrealloc(EZ->sort,(EZ->numsrt+1)*sizeof(RDAezsrt));
		} else {
			EZ->sort=Rmalloc(sizeof(RDAezsrt));
			EZ->numsrt=0;
		}
		s=EZ->sort+EZ->numsrt;
		s->name=stralloc(name);
		s->total=total;
		s->start=start;
		s->type=type;
		s->length=length;
		s->range=range;
		return(++EZ->numsrt);
	} else {
		prterr("Error:  RDAezrpt is NULL...");
		return(-1);
	}
}
short xaddEZdetail(RDAezrpt *EZ,char *title1,char *title2,short start,
	short length,short type,char range,short fieldtype,char *fieldname,
	char *expression,int line,char *file)
{
	RDAezdtl *d=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG addEZdetail at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(EZ!=NULL)
	{
		if(EZ->detail!=NULL)
		{
			EZ->detail=Rrealloc(EZ->detail,(EZ->numdtl+1)*sizeof(RDAezdtl));
		} else {
			EZ->detail=Rmalloc(sizeof(RDAezdtl));
			EZ->numdtl=0;
		}
		d=EZ->detail+EZ->numdtl;
		d->title1=stralloc(title1);
		d->title2=stralloc(title2);
		d->start=start;
		d->length=length;
		d->type=type;
		d->range=range;
		d->fieldtype=fieldtype;
		d->fieldname=stralloc(fieldname);
		d->expression=stralloc(expression);
		return(++EZ->numdtl);
	} else {
		prterr("Error: RDAezrpt is NULL...");
		return(-1);
	}
}
void xSetReportDefaultEngine(RDAreport *tmp,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG SetReportDefaultEngine at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifdef USE_MYISAM
	tmp->engine_type=NRDMyIsamEngine;
#else
#if !defined(USE_CTREE) && !defined(INFORMIX_ISAM)
	tmp->engine_type=NRDRdaEngine;
#endif /* if !defined(USE_CTREE) && !defined(INFORMIX_ISAM) */
#if defined(USE_CTREE) && defined(INFORMIX_ISAM)
	if(tmp->numsorts>11)
	{
		tmp->engine_type=NRDRdaEngine;
	} else if(tmp->numsorts>7)
	{
		tmp->engine_type=NRDLocalCtreeEngine;
	} else {
		tmp->engine_type=NRDInformixIsamEngine;
	}
#endif /* if defined(USE_CTREE) && defined(INFORMIX_ISAM) */
#if defined(USE_CTREE) && !defined(INFORMIX_ISAM)
	if(tmp->numsorts>11)
	{
		tmp->engine_type=NRDRdaEngine;
	} else {
		tmp->engine_type=NRDLocalCtreeEngine;
	}
#endif /* if defined(USE_CTREE) && !defined(INFORMIX_ISAM) */
#if !defined(USE_CTREE) && defined(INFORMIX_ISAM)
	if(tmp->numsorts>7)
	{
		tmp->engine_type=NRDRdaEngine;
	} else {
		tmp->engine_type=NRDInformixIsamEngine;
	}
#endif /* if !defined(USE_CTREE) && defined(INFORMIX_ISAM) */
#endif /* USE_MYISAM */
}

