/* mkmscfnc.c - Make Code misc functions */
#include <rptgen.hpp>

#define MAX_SIZE	100

void xaddERRlist(APPlib **lst,char *string,char *emptystring,int line,char *file)
{
	APPlib *list=NULL;

	list=*lst;
	if(list!=NULL)
	{
		if(list->numlibs==1)
		{
			if(!RDAstrcmp(list->libs[0],emptystring))
			{
				freeapplib(*lst);
				*lst=APPlibNEW();
				list=*lst;
			}
		}
	} else {
		*lst=APPlibNEW();
		list=*lst;
	}
	addAPPlib(*lst,string);
}
APPlib *xbreak_expression2(char *expression,int *overall_length,int line,char *file)
{
	int str_length=0,x=0,y=0;
	APPlib *new_applib=NULL;
	char new_part[MAX_SIZE+1];
	char *temp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG break_expression2 breaking up string at line %d file %s",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	new_applib=APPlibNEW();
	if(!isEMPTY(expression))
	{
		str_length=RDAstrlen(expression);
		memset(new_part,0,MAX_SIZE+1);
		for(x=0,y=0;x<str_length;++x)
		{
			if(y>=MAX_SIZE)
			{
				temp=Rmalloc(MAX_SIZE+3);
				sprintf(temp,"\"%s\"",new_part);
				addAPPlib(new_applib,temp);
				if(temp!=NULL) Rfree(temp);
				memset(new_part,0,MAX_SIZE+1);
				*overall_length+=y;
				y=0;
			}
			if(expression[x]=='\"')
			{
				if((y+2)>MAX_SIZE)
				{
					temp=Rmalloc(MAX_SIZE+3);
					sprintf(temp,"\"%s\"",new_part);
					addAPPlib(new_applib,temp);
					if(temp!=NULL) Rfree(temp);
					memset(new_part,0,MAX_SIZE+1);
					*overall_length+=y;
					y=0;
				}
				new_part[y]='\\';
				++y;
				new_part[y]='"';
				++y;
			}else if(expression[x]=='\\')
			{
				if((y+2)>MAX_SIZE)
				{
					temp=Rmalloc(MAX_SIZE+3);
					sprintf(temp,"\"%s\"",new_part);
					addAPPlib(new_applib,temp);
					if(temp!=NULL) Rfree(temp);
					memset(new_part,0,MAX_SIZE+1);
					*overall_length+=y;
					y=0;
				}
				new_part[y]='\\';
				++y;
				new_part[y]='\\';
				++y;
			} else if(expression[x]=='\n')
			{
				if((y+2)>MAX_SIZE)
				{
					temp=Rmalloc(MAX_SIZE+3);
					sprintf(temp,"\"%s\"",new_part);
					addAPPlib(new_applib,temp);
					if(temp!=NULL) Rfree(temp);
					if(temp!=NULL) Rfree(temp);
					memset(new_part,0,MAX_SIZE+1);
					*overall_length+=y;
					y=0;
				}
				new_part[y]='\\';
				++y;
				new_part[y]='n';
				++y;
			} else {
				if((y+1)>MAX_SIZE)
				{
					temp=Rmalloc(MAX_SIZE+3);
					sprintf(temp,"\"%s\"",new_part);
					addAPPlib(new_applib,temp);
					if(temp!=NULL) Rfree(temp);
					memset(new_part,0,MAX_SIZE+1);
					*overall_length+=y;
					y=0;
				}
				new_part[y]=expression[x];
				++y;
			}
		}
		if(y>0)
		{
			temp=Rmalloc(MAX_SIZE+3);
			sprintf(temp,"\"%s\"",new_part);
			addAPPlib(new_applib,temp);
			if(temp!=NULL) Rfree(temp);
			memset(new_part,0,MAX_SIZE+1);
			*overall_length+=y;
			y=0;
		}
	} else {
		addAPPlib(new_applib,"\"\"");
		*overall_length+=3;
	}
	return(new_applib);
}
char *PAD_STRING(char padchar,int length)
{
	int x=0;
	char *ret_string=NULL;

	ret_string=Rmalloc(length+1);
	for(x=0;x<length;++x)
	{
		ret_string[x]=padchar;
	}
	return(ret_string);
}
void free_pvals(RDApval *pvals,int numpvals)
{
	RDApval *pval=NULL;
	int y=0;

	if(pvals!=NULL)
	{
		for(y=0,pval=pvals;y<numpvals;++y,++pval)
		{
			if(pval->name!=NULL) Rfree(pval->name);
			if(pval->fieldname!=NULL) Rfree(pval->fieldname);
		}
		Rfree(pvals);
	}
}
void free_pvars(RDApvar *pvars,int numpvars)
{
	RDApvar *pvar=NULL;
	int y=0;

	if(pvars!=NULL)
	{
		for(y=0,pvar=pvars;y<numpvars;++y,++pvar)
		{
			if(pvar->name!=NULL) Rfree(pvar->name);
			if(pvar->command!=NULL) Rfree(pvar->command);
		}
		Rfree(pvars);
	}
}

void xaddRDApvar(RDApdev *p,char *name,char *command,short chgsize,short pitch,
	short points,short type,int line,char *file)
{
	RDApvar *a=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG addRDApvar at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(p!=NULL)
	{
		if(p->pvar!=NULL)
		{
			p->pvar=Rrealloc(p->pvar,(p->num+1)*sizeof(RDApvar));
		} else {
			p->num=0;
			p->pvar=Rmalloc(sizeof(RDApvar));
		}
		a=p->pvar+p->num;
		a->name=stralloc(name);
		a->command=stralloc(command);
		a->type=type;
		a->chgsize=chgsize;
		a->points=points;
		a->pitch=pitch;
		++p->num;
	} else {
		prterr("Error RDApdev at line [%d] program [%s] is NULLL.",line,file);
	}
}
int xaddRDATableValue(RDATABLE *T,double a,double b,double r,
	int line,char *file)
{
	TABLEVALUE *v=NULL;
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec)
	{
		prterr("DIAG addRDATableValue at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(T!=NULL)
	{
		if(T->values!=NULL)
		{
			T->values=Rrealloc(T->values,(T->num+1)*sizeof(TABLEVALUE));
		} else {
			T->values=Rmalloc((T->num+1)*sizeof(TABLEVALUE));
		}
		v=T->values+(T->num);
		v->amount=a;
		v->base=b;
		v->rate=r;
		return(++T->num);	
	} else {
		prterr("Error: RDATable is NULL at line [%d] program [%s].",line,file);
		return(-1);
	}
}
void Establish_Install_Date(char *module)
{
	RDAGenericSetup *gsv=NULL;
	char *temp1=NULL,*libx=NULL;

	if(isEMPTY(module)) return;
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif

	gsv=RDAGenericSetupNew(module,"COMPILE DATE");
	if(gsv!=NULL)
	{
		getRDAGenericSetupbin(libx,gsv);
		gsv->type=16;
		gsv->length=10;
		gsv->desc=stralloc("Compile Date of RDA's Latest Update.");
		gsv->label=stralloc("Compile Date:");
		gsv->value.string_value=stralloc(VERSION_DATE);

		if(writeRDAGenericSetupbin(libx,gsv))
		{
			if(temp1!=NULL) Rfree(temp1);
			if(libx!=NULL) Rfree(libx);
			temp1=Rmalloc(12+6+110+1);
			sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [%s] GSV [COMPILE DATE], Can Not Save Generic Setup Variable!",module);
			prterr(temp1);
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew(module,"INSTALLATION DATE");
	if(gsv!=NULL)
	{
		getRDAGenericSetupbin(libx,gsv);
		gsv->type=16;
		gsv->length=10;
		gsv->desc=stralloc("Installation Date of RDA's Latest Update.");
		gsv->label=stralloc("Installation Date:");
		gsv->value.string_value=stralloc(CURRENT_DATE10);

		if(writeRDAGenericSetupbin(libx,gsv))
		{
			if(temp1!=NULL) Rfree(temp1);
			if(libx!=NULL) Rfree(libx);
			temp1=Rmalloc(17+6+110+1);
			sprintf(temp1,"GENERIC SETUP VARIABLE WRITE ERROR: Module [%s] GSV [INSTALLATION DATE], Can Not Save Generic Setup Variable!",module);
			prterr(temp1);
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(libx!=NULL) Rfree(libx);
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
}
