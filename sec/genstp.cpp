/* genstp.c - Generic Setup Variable Implementation */
#include<sec.hpp>

RDAGenericSetup *Internal_RDAGenericSetup=NULL;
int Internal_num_RDAGenericSetup=0;

RDAGenericSetup *xRDAGenericSetupNew(char *module,char *name,int line,char *file)
{
	RDAGenericSetup *s=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec)
	{
		prterr("DIAG RDAGenericSetupNew at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	s=Rmalloc(sizeof(RDAGenericSetup));
	s->module=stralloc(module);
	s->name=stralloc(name);
	s->type=(-1);
	s->length=0;
	s->desc=NULL;
	s->label=NULL;	
	return(s);
}
void xFreeRDAGenericSetup(RDAGenericSetup *g,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec)
	{
		prterr("DIAG FreeRDAGenericSetup at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(g!=NULL)
	{
		if(g->type!=(-1))
		{
			switch(g->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case BOOLNS:
				case CHARACTERS:
				case PHONE:
				case CUSTOMTYPE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case ZIPCODE:
					if(g->value.string_value!=NULL) 
						Rfree(g->value.string_value);
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					if(g->value.float_value!=NULL) 
						Rfree(g->value.float_value);
					break;
				case SHORTV:
				case SSHORTV:
					if(g->value.short_value!=NULL) 
						Rfree(g->value.short_value);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					if(g->value.integer_value!=NULL) 
						Rfree(g->value.integer_value);
					break;
				case BUTTONS:
					prterr("Error call RaddogTiddog cause this is weird....");
					break;
				default:
					prterr("Error field type [%s] invalid for Setup Variable [%s] at line [%d] program [%s].",g->type,(g->name!=NULL ? g->name:""),line,file);
					break;
			}
		}
		if(g->module!=NULL) Rfree(g->module);
		if(g->name!=NULL) Rfree(g->name);
		if(g->desc!=NULL) Rfree(g->desc);
		if(g->label!=NULL) Rfree(g->label);
	}
}
short xgetRDAGenericSetupbin(char *libname,RDAGenericSetup *g,int line,char *file)
{
	short x,test=FALSE;
	BIN *bin;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec)
	{
		prterr("DIAG getRDAGenericSetupbin Attempting to Read RDAGenericSetup Binary [%s] from RDAGenericSetup Library [%s] at line [%d] program [%s].",(g->name!=NULL ? g->name:""),(libname!=NULL ? libname:""),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=getlibbin(libname,g->name,FALSE);
	if(bin==NULL) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec)
		{
			prterr("Error Attempting to Read RDAGenericSetup Binary [%s] from RDAGenericSetup Library [%s] Failed at line [%d] program [%s].",(g->name!=NULL ? g->name:""),(libname!=NULL ? libname:""),line,file); 
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(-1);
	}
	x=BINgetshort(bin);
	if(x!=(-1701))
	{
		prterr("Error Invalid version or bad binary for RDAGenericSetup [%s] in RDAGenericSetup Binary Library [%s] at line [%d] program [%s].",g->name,libname,line,file);
		BINfree(bin);
		return(-1);
	}
	if(g->module!=NULL) Rfree(g->module);
	if(g->name!=NULL) Rfree(g->name);
	test=BINgetshort(bin);
	if(test) g->module=BINgetstring(bin);
		else g->module=NULL;
	test=BINgetshort(bin);
	if(test) g->name=BINgetstring(bin);
		else g->name=NULL;
	test=BINgetshort(bin);
	if(test) g->desc=BINgetstring(bin);
		else g->desc=NULL;
	test=BINgetshort(bin);
	if(test) g->label=BINgetstring(bin);
		else g->label=NULL;
	g->type=BINgetshort(bin);
	g->length=BINgetshort(bin);
	if(g->type!=(-1))
	{
		switch(g->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case ZIPCODE:
				test=BINgetshort(bin);
				if(test) g->value.string_value=BINgetstring(bin);
					else g->value.string_value=NULL;
				break;
			case BOOLNS:
			case CHARACTERS:
				g->value.string_value=Rmalloc(1);
				*g->value.string_value=0;
				test=BINgetshort(bin);
				if(g->type==BOOLNS)
				{
					if(test) *g->value.string_value=TRUE;
				} else *g->value.string_value=test;
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				g->value.float_value=Rmalloc(sizeof(double));
				*g->value.float_value=0;
				*g->value.float_value=BINgetdouble(bin);
				break;
			case SHORTV:
			case SSHORTV:
				g->value.short_value=Rmalloc(sizeof(short));
				*g->value.short_value=0;
				*g->value.short_value=BINgetshort(bin);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				g->value.integer_value=Rmalloc(sizeof(int));
				*g->value.integer_value=0;
				*g->value.integer_value=BINgetint(bin);
				break;
			case BUTTONS:
				prterr("Error call RaddogTiddog cause this is weird....");
				break;
			default:
				prterr("Error field type [%s] invalid for Setup Variable [%s] at line [%d] program [%s].",g->type,(g->name!=NULL ? g->name:""),line,file);
				break;
		}
	}
	BINfree(bin);
	return(0);
}
short xwriteRDAGenericSetupbin(char *libname,RDAGenericSetup *gs,int line,char *file)
{
	BIN *bin;
	double Zero=0.0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec)
	{
		prterr("DIAG writeRDAGenericSetupbin Writing RDAGenericSetup [%s] into RDAGenericSetup Binary Library [%s] at line [%d] program [%s].",(gs->name!=NULL ? gs->name:""),libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=BINnew();
	BINaddshort(bin,-1701);	/*type identifier*/
	if(!isEMPTY(gs->module))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,gs->module);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(gs->name))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,gs->name);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(gs->desc))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,gs->desc);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(gs->label))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,gs->label);
	} else BINaddshort(bin,FALSE);
	BINaddshort(bin,gs->type);
	BINaddshort(bin,gs->length);
	if(gs->type!=(-1))
	{
		switch(gs->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case ZIPCODE:
				if(!isEMPTY(gs->value.string_value))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,gs->value.string_value);
				} else BINaddshort(bin,FALSE);
				break;
			case BOOLNS:
			case CHARACTERS:
				if(gs->value.string_value!=NULL) 
				{
					BINaddshort(bin,*gs->value.string_value);
				} else BINaddshort(bin,FALSE);
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				if(gs->value.float_value!=NULL)
				{
					BINadddouble(bin,*gs->value.float_value);
				} else {
					BINadddouble(bin,Zero);
				}
				break;
			case SHORTV:
			case SSHORTV:
				if(gs->value.short_value!=NULL)
				{
					BINaddshort(bin,*gs->value.short_value);
				} else {
					BINaddshort(bin,0);
				}
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				if(gs->value.integer_value!=NULL)
				{
					BINaddint(bin,*gs->value.integer_value);
				} else {
					BINaddint(bin,0);
				}
				break;
			case BUTTONS:
				prterr("Error call RaddogTiddog cause this is weird....");
				break;
			default:
				prterr("Error field type [%s] invalid for Setup Variable [%s] at line [%d] program [%s].",gs->type,(gs->name!=NULL ? gs->name:""),line,file);
				break;
		}
	}
	if(writelibbin(libname,bin,gs->name))
	{
		prterr("Error Can't write RDAGenericSetup binary [%s] into RDAGenericSetup library [%s] at line [%d] program [%s].",(gs->name!=NULL ? gs->name:""),libname,line,file);
		return(-1);
	}
	return(0);
}
short xgetRDAGenericSetupdesc(char *libname,char *name,char **desc,int line,char *file)
{
	short x,test=FALSE;
	BIN *bin;
	char *temp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec)
	{
		prterr("DIAG getRDAGenericSetupdesc Attempting to Read RDAGenericSetup description  for [%s] from RDAGenericSetup Library [%s] at line [%d] program [%s].",name,libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=getlibbin(libname,name,FALSE);
	if(bin==NULL) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagsec)
		{
			prterr("Error Attempting to Read RDAGenericSetup Binary [%s] from RDAGenericSetup Library [%s] Failed at line [%d] program [%s].",name,libname,line,file); 
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		return(-1);
	}
	x=BINgetshort(bin);
	if(x!=(-1701))
	{
		prterr("Error Invalid version or bad binary for RDAGenericSetup [%s] in RDAGenericSetup Binary Library [%s] at line [%d] program [%s].",name,libname,line,file);
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
	BINfree(bin);
	return(0);
}
void xFreeInternalGenericSetup(int line,char *file)
{
	RDAGenericSetup *g=NULL;
	int x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec)
	{
		prterr("DIAG FreeInternalGenericSetup at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(Internal_RDAGenericSetup!=NULL)
	{
		for(x=0,g=Internal_RDAGenericSetup;x<Internal_num_RDAGenericSetup;++x,++g)
		{
			if(g->type!=(-1))
			{
				switch(g->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case BOOLNS:
					case CHARACTERS:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case ZIPCODE:
						if(g->value.string_value!=NULL)
							Rfree(g->value.string_value);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						if(g->value.float_value!=NULL)
							Rfree(g->value.float_value);
						break;
					case SHORTV:
					case SSHORTV:
						if(g->value.short_value!=NULL)
							Rfree(g->value.short_value);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						if(g->value.integer_value!=NULL) 
							Rfree(g->value.integer_value);
						break;
					case BUTTONS:
						prterr("Error call RaddogTiddog cause this is weird....");
						break;
					default:
						prterr("Error field type [%s] invalid for Setup Variable [%s] at line [%d] program [%s].",g->type,(g->name!=NULL ? g->name:""),line,file);
						break;
				}
			}
			if(g->module!=NULL) Rfree(g->module);
			if(g->name!=NULL) Rfree(g->name);
			if(g->desc!=NULL) Rfree(g->desc);
			if(g->label!=NULL) Rfree(g->label);
		}
		Rfree(Internal_RDAGenericSetup);
	}
	Internal_RDAGenericSetup=NULL;
	Internal_num_RDAGenericSetup=0;
}
NRDfield *xComputeRDAGenericSetup(RDAGenericSetup *t,void (*SubFunc)(...),
	void *args,int line,char *file)
{
	NRDfield *field=NULL;

	if(t!=NULL)
	{
		if(t->type!=(-1))
		{
			field=Rmalloc(sizeof(NRDfield));
			field->name=stralloc(t->name);
			field->type=t->type;
			field->len=t->length;
			switch(t->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case ZIPCODE:
					if(!isEMPTY(t->value.string_value))
					{
						field->data.string_value=stralloc(t->value.string_value);
					} else field->data.string_value=NULL;
					break;
				case BOOLNS:
				case CHARACTERS:
					field->data.string_value=Rmalloc(1);
					*field->data.string_value=0;
					*field->data.string_value=*t->value.string_value;
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					field->data.float_value=Rmalloc(sizeof(double));
					*field->data.float_value=0;
					*field->data.float_value=*t->value.float_value;
					break;
				case SHORTV:
				case SSHORTV:
					field->data.short_value=Rmalloc(sizeof(short));
					*field->data.short_value=0;
					*field->data.short_value=*t->value.short_value;
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					field->data.integer_value=Rmalloc(sizeof(int));
					*field->data.integer_value=0;
					*field->data.integer_value=*t->value.integer_value;
					break;
				case BUTTONS:
					prterr("Error call RaddogTiddog cause this is weird....");
					break;
				default:
					prterr("Error field type [%s] invalid for Setup Variable [%s] at line [%d] program [%s].",t->type,(t->name!=NULL ? t->name:""),line,file);
					break;
			}
		} else {
			prterr("Error Generic Setup Variable type of (-1) is invalid for [%s][%s].... Please return to setup and correct.",(t->module!=NULL ? t->module:""),(t->name!=NULL ? t->name:""));
		}
	}
	return(field);
}
NRDfield *xCOMPUTEGenericSetup(char *expression,void (*SubFunc)(...),void *args,
	int line,char *file)
{
	RDAGenericSetup *t=NULL;
	int x=0;
	char *module=NULL,*name=NULL,*libname=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagsec)
	{
		prterr("DIAG COMPUTEGenericSetup at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(isEMPTY(expression))
	{
		prterr("No [Module][Generic Setup Variable] to use......");
		return(0);
	} else {
		module=stripmodulename(expression);
		name=stripfilename(expression);
	}
	if(isEMPTY(module) || isEMPTY(name))
	{
		prterr("Error Module or Generic Setup Variable name is NULL.");
		if(module!=NULL) Rfree(module);
		if(name!=NULL) Rfree(name);
		return(0);
	}
	if(Internal_RDAGenericSetup!=NULL)
	{
		for(x=0,t=Internal_RDAGenericSetup;x<Internal_num_RDAGenericSetup;++x,++t)
		{
			if(!RDAstrcmp(t->module,module) && !RDAstrcmp(t->name,name)) break;
		}
		if(x>=Internal_num_RDAGenericSetup)
		{
			x=0;
			t=NULL;
		}
	}
	if(t==NULL)
	{
		if(Internal_RDAGenericSetup!=NULL)
		{
			Internal_RDAGenericSetup=Rrealloc(Internal_RDAGenericSetup,(Internal_num_RDAGenericSetup+1)*sizeof(RDAGenericSetup));
		} else {
			Internal_RDAGenericSetup=Rmalloc(sizeof(RDAGenericSetup));
			Internal_num_RDAGenericSetup=0;
		}
		t=Internal_RDAGenericSetup+Internal_num_RDAGenericSetup;
		if(!isEMPTY(module)) t->module=stralloc(module);
			else t->module=NULL;
		if(!isEMPTY(name)) t->name=stralloc(name);
			else t->name=NULL;
		t->type=(-1);
		t->length=0;
		t->desc=NULL;
		t->label=NULL;	
		libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
#ifndef WIN32
		sprintf(libname,"%s/rda/%s.GSV",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,module);
#endif
		getRDAGenericSetupbin(libname,t);
		++Internal_num_RDAGenericSetup;
	}
	if(libname!=NULL) Rfree(libname);
	if(module!=NULL) Rfree(module);
	if(name!=NULL) Rfree(name);
	return(xComputeRDAGenericSetup(t,SubFunc,args,__LINE__,__FILE__));
}
#ifdef USE_MYSQL
void INITNRD_MYSQL_GSVS()
{
	RDAGenericSetup *gsv=NULL;
	char *libx=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("DATABASE")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"DATABASE");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"DATABASE");
#endif

	gsv=RDAGenericSetupNew("DATABASE","MYSQL DATABASE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			MySQL_Database=stralloc(gsv->value.string_value);
		} else {
			MySQL_Database=NULL;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("DATABASE","MYSQL USERNAME");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			MySQL_Username=stralloc(gsv->value.string_value);
		} else {
			MySQL_Username=NULL;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("DATABASE","MYSQL PASSWORD");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			MySQL_Password=stralloc(gsv->value.string_value);
		} else {
			MySQL_Password=NULL;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("DATABASE","MYSQL CASE SENSITIVE COLLATION");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			RDA_CollateType=*gsv->value.string_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
}
#endif /* USE_MYSQL */
