/* Make Import Code Funcitons */
#include <impexp.hpp>

short xaddIMPEXPfield(IEfile *IMPEXP,char *name,short type,short start,unsigned length,char *expression,char use_quotes,char *value_expression,int line,char *file)
{
	IEfield *nf=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG addIMPEXPfield() at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(IMPEXP!=NULL)
	{
		if(IMPEXP->fields!=NULL)
		{
			IMPEXP->fields=Rrealloc(IMPEXP->fields,(IMPEXP->num+1)*sizeof(IEfield));
		} else {
			IMPEXP->fields=Rmalloc(sizeof(IEfield));
			IMPEXP->num=0;
		}
		nf=IMPEXP->fields+IMPEXP->num;
		if(!isEMPTY(name))
		{
			nf->name=stralloc(name);
		} else nf->name=NULL;
		nf->type=type;
		nf->start=start;
		nf->length=length;
		if(!isEMPTY(expression))
		{
			nf->expression=stralloc(expression);
		} else nf->expression=NULL;
		nf->use_quotes=use_quotes;
		if(!isEMPTY(value_expression))
		{
			nf->value_expression=stralloc(value_expression);
		} else nf->value_expression=NULL;
		return(++IMPEXP->num);
	} else {
		return(-1);
	}
}
