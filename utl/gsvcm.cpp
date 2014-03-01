/* gsvcm.c - Generic Setup Variables Common Functions */
#include <mix.hpp>

void xGetAllSetupVariables(char *module,int line,char *file)
{
	APPlib *nlist=NULL;
	int x=0;
	char *libname=NULL;
	RDAGenericSetup *t=NULL;

	if(diagapps)
	{
		prterr("DIAG GetAllSetupVariables at line [%d] program [%s].",line,file);
	}
	if(!isEMPTY(module))
	{
		libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
#ifndef WIN32
		sprintf(libname,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif
		nlist=getlibnames(libname,FALSE);
		if(nlist!=NULL)
		{
			if(nlist->numlibs>0 && nlist->libs!=NULL)
			{
				if(Internal_RDAGenericSetup!=NULL)
				{
					Internal_RDAGenericSetup=Rrealloc(Internal_RDAGenericSetup,(Internal_num_RDAGenericSetup+nlist->numlibs)*sizeof(RDAGenericSetup));
				} else {
					Internal_RDAGenericSetup=Rmalloc(nlist->numlibs*sizeof(RDAGenericSetup));
					Internal_num_RDAGenericSetup=0;
				}
				for(x=0;x<nlist->numlibs;++x)
				{
					t=Internal_RDAGenericSetup+Internal_num_RDAGenericSetup;
					if(!isEMPTY(module)) t->module=stralloc(module);
						else t->module=NULL;
					if(!isEMPTY(nlist->libs[x])) t->name=stralloc(nlist->libs[x]);
						else t->name=NULL;
					t->type=(-1);
					t->length=0;
					t->desc=NULL;
					t->label=NULL;	
					getRDAGenericSetupbin(libname,t);
					++Internal_num_RDAGenericSetup;
				}
			}
		}
		if(libname!=NULL) Rfree(libname);
	}
}
void xRDAGenericSetup2rsrc(RDArsrc *r,int line,char *file)
{
	int x;
	RDAGenericSetup *g=NULL;

	if(diagapps)
	{
		prterr("DIAG RDAGenericSetup2rsrc at line [%d] program [%s].",line,file);
	}
	if(Internal_RDAGenericSetup!=NULL && r!=NULL)
	{
		for(x=0,g=Internal_RDAGenericSetup;x<Internal_num_RDAGenericSetup;++x,++g)
		{
			switch(g->type)
			{
				case SCROLLEDTEXT:
					addsctrsrc(r,g->name,g->length,g->value.string_value,TRUE);
					break;
				case VARIABLETEXT:
				case PLAINTEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					addstdrsrc(r,g->name,g->type,g->length,g->value.string_value,TRUE);
					break;
				case SHORTV:
				case SSHORTV:
					addstdrsrc(r,g->name,g->type,g->length,g->value.short_value,TRUE);
					break;
				case LONGV:
				case SLONGV:
					addstdrsrc(r,g->name,g->type,g->length,g->value.integer_value,TRUE);
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					addstdrsrc(r,g->name,g->type,g->length,g->value.float_value,TRUE);
					break;
				case BOOLNS:
					addstdrsrc(r,g->name,g->type,g->length,g->value.string_value,TRUE);
					break;
				case CHARACTERS:
					addstdrsrc(r,g->name,g->type,g->length,g->value.string_value,TRUE);
					break;
				default:
					prterr("Error Unrecognized Field type [%d] for resource [%s] not added to Screen [%s] [%s].",g->type,g->name,r->module,r->screen); 
					break;
			}
			if(!RDAstrcmp(g->name,"BINARY VERSION NUMBER"))
			{
				FINDRSCSETEDITABLE(r,g->name,FALSE);
			}
		}
	} else {
		prterr("Error Function Failed either for no Generic Setup Variables available or RDArsrc Structure not available at line [%s] program [%s].",line,file);
	}
}
void xrsrc2RDAGenericSetup(RDArsrc *r,int line,char *file)
{
	int x;
	RDAGenericSetup *g=NULL;
	char t=0;

	if(diagapps)
	{
		prterr("DIAG RDAGenericSetup2rsrc at line [%d] program [%s].",line,file);
	}
	if(Internal_RDAGenericSetup!=NULL && r!=NULL)
	{
		for(x=0,g=Internal_RDAGenericSetup;x<Internal_num_RDAGenericSetup;++x,++g)
		{
			switch(g->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case PLAINTEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					FINDRSCGETSTRING(r,g->name,&g->value.string_value);
					break;
				case SHORTV:
				case SSHORTV:
					FINDRSCGETSHORT(r,g->name,g->value.short_value);
					break;
				case LONGV:
				case SLONGV:
					FINDRSCGETINT(r,g->name,g->value.integer_value);
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					FINDRSCGETDOUBLE(r,g->name,g->value.float_value);
					break;
				case BOOLNS:
				case CHARACTERS:
					FINDRSCGETCHAR(r,g->name,&t);
					*g->value.string_value=t;
					break;
				default:
					prterr("Error Unrecognized Field type [%d] for resource [%s] not added to Screen [%s] [%s].",g->type,g->name,r->module,r->screen); 
					break;
			}
		}
	} else {
		prterr("Error Function Failed either for no Generic Setup Variables available or RDArsrc Structure not available at line [%s] program [%s].",line,file);
	}
}
void xWriteAllSetupVariables(char *module,int line,char *file)
{
	int x=0;
	char *libname=NULL;
	RDAGenericSetup *t=NULL;

	if(diagapps)
	{
		prterr("DIAG WriteAllSetupVariables at line [%d] program [%s].",line,file);
	}
	if(!isEMPTY(module))
	{
		libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
#ifndef WIN32
		sprintf(libname,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif
		if(Internal_RDAGenericSetup!=NULL)
		{
			for(x=0,t=Internal_RDAGenericSetup;x<Internal_num_RDAGenericSetup;++x,++t)
			{
				if(!RDAstrcmp(t->module,module))
				{
					writeRDAGenericSetupbin(libname,t);
				}
			}
		}
	}
}
