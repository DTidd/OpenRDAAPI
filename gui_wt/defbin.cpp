#include <guip.hpp>
#include <gui.hpp>

char *LastGroupDefault=NULL;

RDAdefault *xRDAdefaultNEW(char *module,char *name,int line,char *file)
{
	RDAdefault *tmp;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG RDAdefaultNEW Allocating Default Screen Structure for Module [%s] Name [%s] at line [%d] program [%s].",module,name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(RDAdefault));
	if(module!=NULL)
	{
		tmp->module=stralloc(module);
	} else tmp->module=NULL;
	if(name!=NULL)
	{
		tmp->name=stralloc(name);
	} else tmp->name=NULL;
	tmp->input_focus=NULL;
	tmp->num=0;
	tmp->field=NULL;
	return(tmp);
}
void xFreeRDAdefault(RDAdefault *d,int line,char *file)
{
	int x;
	DEFfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG FreeRDAdefault Freeing Default Screen [%s] [%s] at line [%d] program [%s].",d->module,d->name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(d!=NULL)
	{
		if(d->module!=NULL) 
		{
			Rfree(d->module);
		}
		if(d->name!=NULL) Rfree(d->name);
		if(d->input_focus!=NULL) Rfree(d->input_focus);
		if(d->field!=NULL)
		{
			for(x=0,field=d->field+x;x<d->num;++x,++field)
			{
				if(field->name!=NULL) Rfree(field->name);
				switch(field->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case ZIPCODE:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case BOOLNS:
					case CHARACTERS:
						if(field->data.string_value!=NULL) 
							Rfree(field->data.string_value);
						break;
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case SDECIMALV:
						if(field->data.float_value!=NULL) 
							Rfree(field->data.float_value);
						break;
					case SHORTV:
					case SSHORTV:
						if(field->data.short_value!=NULL) 
							Rfree(field->data.short_value);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						if(field->data.integer_value!=NULL) 
							Rfree(field->data.integer_value);
						break;
					default:
						prterr("Error Field Type [%d] is invalid for Default Screen Fields [%s].",field->type,field->name);
						break;
				}
			}
			Rfree(d->field);
		}
		Rfree(d);
	}
}
short xwritedefaultbin(char *libname,RDAdefault *d,int line,char *file)
{
	short x,test;
	BIN *bin;
	DEFfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG writedefaultbin Writing Default Binary [%s] [%s] into Default Screen's Binary Library [%s] at line [%d] file [%s].",(d->module!=NULL ? d->module:""),(d->name!=NULL ? d->name:""),libname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	bin=BINnew();
	BINaddshort(bin,-1699);	/*type identifier*/
	if(d->name!=NULL) 
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,d->name);
	} else BINaddshort(bin,FALSE);
	if(d->input_focus!=NULL)
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,d->input_focus);
	} else BINaddshort(bin,FALSE);
	BINaddint(bin,d->num);
	for(x=0,field=d->field;x<d->num;++x,++field)
	{
		if(field->name!=NULL) 
		{
			BINaddshort(bin,TRUE);
			BINaddstring(bin,field->name);
		} else BINaddshort(bin,FALSE);
		BINaddshort(bin,field->type);
		BINaddshort(bin,field->reset);
		switch(field->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				if(!isEMPTY(field->data.string_value))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,field->data.string_value);
				} else BINaddshort(bin,FALSE);
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				BINadddouble(bin,*field->data.float_value);
				break;
			case BOOLNS:
			case CHARACTERS:
				test=*field->data.string_value;
				BINaddshort(bin,test);
				break;
			case SHORTV:
			case SSHORTV:
				BINaddshort(bin,*field->data.short_value);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				BINaddint(bin,*field->data.integer_value);
				break;
			default:
				prterr("Error Field Type [%d] is invalid for Default Screen Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
				break;
		}
	}
	if(writelibbin(libname,bin,d->name))
	{
		prterr("Error can't write Default Screen Binary [%s] [%s] into Binary Library [%s] at line [%d] program [%s].",(d->module!=NULL ? d->module:""),(d->name!=NULL ? d->name:""),libname,line,file);
		return(-1);
	}
	return(0);
}
RDAdefault *xfinddefaultbin(char *module,char *name,int line,char *file)
{
	RDAdefault *d=NULL;
	char *libname=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field) { prterr("DIAG finddefaultbin Looking for Default Screen Binary [%s] [%s] using finddefaultbin at line [%d] program [%s].",module,name,line,file); }
#endif /* USE_RDA_DIAGNOSTICS */
	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+RDAstrlen(USERLOGIN)+7);
#ifndef WIN32
	sprintf(libname,"%s/%s/%s.DEF",CURRENTDIRECTORY,USERLOGIN,module);
#endif
#ifdef WIN32
	sprintf(libname,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,USERLOGIN,module);
#endif
	d=RDAdefaultNEW(module,name);
	if(!getdefaultbin(libname,name,d,FALSE)) 
	{
		if(libname!=NULL) Rfree(libname);
		return(d);
	}
	if(!isEMPTY(RDA_GROUP))
	{
		libname=Rrealloc((void *)libname,RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+RDAstrlen(RDA_GROUP)+11);
#ifndef WIN32
		sprintf(libname,"%s/%s/%s.DEF",CURRENTDIRECTORY,RDA_GROUP,module);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\%s\\%s.DEF",CURRENTDIRECTORY,RDA_GROUP,module);
#endif
		if(!getdefaultbin(libname,name,d,FALSE)) 
		{
			if(libname!=NULL) Rfree(libname);
			return(d);
		}
	}
	
	libname=Rrealloc((void *)libname,RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+11);
#ifndef WIN32
	sprintf(libname,"%s/site/%s.DEF",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libname,"%s\\site\\%s.DEF",CURRENTDIRECTORY,module);
#endif
	if(!getdefaultbin(libname,name,d,FALSE)) 
	{
		if(libname!=NULL) Rfree(libname);
		return(d);
	}
	libname=Rrealloc((void *)libname,RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
#ifndef WIN32
	sprintf(libname,"%s/rda/%s.DEF",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\%s.DEF",CURRENTDIRECTORY,module);
#endif
	if(!getdefaultbin(libname,name,d,FALSE)) 
	{
		if(libname!=NULL) Rfree(libname);
		return(d);
	}
	if(libname!=NULL) Rfree(libname);
	if(d!=NULL) FreeRDAdefault(d);
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("Error can't find Default Screen Binary for Screen [%s] [%s] at line [%d] program [%s].",module,name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(NULL);
}
short xgetdefaultbin(char *libname,char *name,RDAdefault *d,short showerror,int line,char *file)
{
	short x,test,version=FALSE;
	BIN *bin;
	DEFfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG getdefaultbin Reading Default Screen Binary [%s] [%s] from Binary Library [%s] at line [%d] program [%s].",d->module,name,libname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	bin=getlibbin(libname,name,showerror);
	if(bin==NULL) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diaggui || diaggui_field)
		{
			prterr("Error Default Screen Binary [%s] [%s] not found in Binary Library [%s] at line [%d] program [%s].",d->module,d->name,libname,line,file);
		}
#endif /* USE_RDA_DIAGNOSTICS */
		return(-1);
	}
	x=BINgetshort(bin);
	if(x!=(-1699) && x!=(-1701) && x!=(-1700))
	{
		prterr("Error invalid version or bad Default Screen Binary [%s] [%s] from Binary Library [%s] at line [%d] program [%s].",d->module,name,libname,line,file);
		BINfree(bin);
		return(-1);
	}
	if(x==(-1699)) version=2;
	else if(x==(-1700)) version=TRUE;
	else version=FALSE;
	test=BINgetshort(bin);
	if(d->name!=NULL) Rfree(d->name);
	if(test) d->name=BINgetstring(bin);
		else d->name=NULL;
	if(version)
	{
		test=BINgetshort(bin);
		if(test) d->input_focus=BINgetstring(bin);
			else d->input_focus=NULL;
	} else d->input_focus=NULL; 
	d->num=BINgetint(bin);
	d->field=Rmalloc(d->num*sizeof(DEFfield));
	for(x=0,field=d->field;x<d->num;++x,++field)
	{
		test=BINgetshort(bin);
		if(test) field->name=BINgetstring(bin);
			else field->name=NULL;
		field->type=BINgetshort(bin);
		if(version>1)
		{
			test=BINgetshort(bin);
			if(test) field->reset=TRUE;
				else field->reset=FALSE;
		} else field->reset=TRUE;
		switch(field->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				test=BINgetshort(bin);
				if(test) field->data.string_value=BINgetstring(bin);
					else field->data.string_value=NULL;
				break;
			case BOOLNS:
			case CHARACTERS:
				field->data.string_value=Rmalloc(1);
				*field->data.string_value=BINgetshort(bin);
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				field->data.float_value=Rmalloc(sizeof(double));
				*field->data.float_value=BINgetdouble(bin);
				break;
			case SHORTV:
			case SSHORTV:
				field->data.short_value=Rmalloc(sizeof(short));
				*field->data.short_value=BINgetshort(bin);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				field->data.integer_value=Rmalloc(sizeof(int));
				*field->data.integer_value=BINgetint(bin);
				break;
			case BUTTONS:
				break;
			default:
				prterr("Error Field Type [%d] is invalid for Default Screen Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
				break;
		}
	}
	BINfree(bin);
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field) SEEDEFAULTS(d);
#endif /* USE_RDA_DIAGNOSTICS */
	return(0);
}
void xSEEDEFAULTS(RDAdefault *d,int line,char *file)
{
	int x=0;
	DEFfield *field=NULL;

	prterr("DIAG SEEDEFAULTS Showing Default Screen [%s] [%s] at line [%d] program [%s].",d->module,d->name,line,file);
	prterr("Resource to receive Input Focus [%s] ",(d->input_focus!=NULL ?
		d->input_focus:""));
	for(x=0,field=d->field;x<d->num;++x,++field)
	{
		prterr("FIELD [%d] Name [%s] Reset [%s] Type [%d] [%s]\n",x,field->name,(field->reset ? "True":"False"),field->type,standardfieldtypes[field->type]);
		switch(field->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				prterr("\t Value [%s]",(field->data.string_value!=NULL ? field->data.string_value:"NULL"));
				break;
			case BOOLNS:
				prterr("\t Value [%d] [%s]",*field->data.string_value,(*field->data.string_value ? "Yes":"No"));
				break;
			case CHARACTERS:
				prterr("\t Value [%c]",*field->data.string_value);
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				prterr("\t Value [%f]",*field->data.float_value);
				break;
			case SHORTV:
			case SSHORTV:
				prterr("\t Value [%d]",*field->data.short_value);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				prterr("\t Value [%d]",*field->data.integer_value);
				break;
			case BUTTONS:
				break;
			default:
				prterr("field type [%d] not found for field [%s] in SEEDEFAULTS.",field->type,field->name);
				break;
		}
		prterr("\n\n");
	}
	prterr("END RDAdefault [%s][%s]\n",d->module,d->name);
}
DEFfield *xFINDFIELDDEFAULT(RDAdefault *d,char *name,int line,char *file)
{
	DEFfield *field;
	int x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDFIELDDEFAULT Find Default for Field [%s] on Screen [%s] [%s] at line [%d] program [%s].",name,d->module,d->name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(d!=NULL)
	{
		for(x=0,field=d->field;x<d->num;++x,++field)
		{
			if(!RDAstrcmp(field->name,name)) break;
		}
		if(x<d->num) return(field);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("Error FINDFIELDDEFAULT not found for Field [%s] at line [%d] program [%s].",name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(NULL);
}
short xDefaultScreens(RDArsrc *rsrc,short reset,char **name,short which,int line,char *file)
{
	RDArmem *member=NULL;
	RDAdefault *d=NULL;
	int x;
	DEFfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG DefaultScreen Attempting to Set Screen Defaults for Screen [%s] [%s] at line [%d] program [%s].",rsrc->module,rsrc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	d=finddefaultbin(rsrc->module,rsrc->screen);
	if(d!=NULL)
	{
		if(RDAstrstr(rsrc->screen," DEFINE LIST"))
		{
			for(x=0,member=rsrc->rscs;x<rsrc->numrscs;++x,++member)
			{
				if(member->field_type==SHORTV) *member->value.short_value=0;
			}
		}
		if(d->input_focus!=NULL) 
			FINDRSCSETINPUTFOCUS(rsrc,d->input_focus);
		if(name!=NULL)
		{
			if(*name!=NULL) Rfree(*name);
		}
		if(which==TRUE) *name=stralloc(d->input_focus);
		for(x=0,field=d->field;x<d->num;++x,++field)
		{
			if((field->name!=NULL) && (!reset || (reset 
				&& field->reset)))
			{
			switch(field->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case ZIPCODE:
				case PHONE:
				case CUSTOMTYPE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					FINDRSCSETSTRING(rsrc,field->name,
						field->data.string_value);
					break;
				case BOOLNS:
				case CHARACTERS:
					FINDRSCSETCHAR(rsrc,field->name,
						*field->data.string_value);
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					FINDRSCSETDOUBLE(rsrc,field->name,
						*field->data.float_value);
					break;
				case SHORTV:
				case SSHORTV:
					FINDRSCSETSHORT(rsrc,field->name,
						*field->data.short_value);
					break;
/*
				case SCROLLEDLIST:
					y=FINDRSC(rsrc,field->name);
					if(y>0)
					{
						member=rsrc->rscs+y;
						if(*field->data.integer_value<member->items)
						{
							*member->value.integer_value=*field->data.integer_value;
						} 
					}	
					break;
*/
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					FINDRSCSETINT(rsrc,field->name,
						*field->data.integer_value);
					break;
				case BUTTONS:
					break;
				default:
					prterr("Error Field Type [%d] is invalid for Default Screen Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
					break;
			}
			}
		}
		FreeRDAdefault(d);
		return(0);
#ifdef USE_RDA_DIAGNOSTICS
	} else if(diaggui || diaggui_field)
	{
		prterr("Error Default Screen [%s] [%s] failed at line [%d] program [%s].",rsrc->module,rsrc->screen,line,file);
#endif /* USE_RDA_DIAGNOSTICS */
	}
	return(-1);
}
RDAdefault *xGetDefaults(RDArsrc *rsrc,int line,char *file)
{
	RDAdefault *d=NULL;
	int x;
	DEFfield *field;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG GetDefaults Getting Default Screen [%s] [%s] at line [%d] program [%s].",rsrc->module,rsrc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	d=RDAdefaultNEW(rsrc->module,rsrc->screen);
	for(x=0,member=rsrc->rscs;x<rsrc->numrscs;++x,++member)
	{
		if(member->field_type!=BUTTONS && RDAstrcmp(member->rscrname,"PREVIOUS ADD'S"))
		{
			if(d->field!=NULL)
			{
				d->field=Rrealloc((void *)d->field,
					((d->num+1)*sizeof(DEFfield)));
			} else {
				d->field=Rmalloc((d->num+1)*sizeof(DEFfield));
			}
			field=d->field+d->num;
			if(member->rscrname!=NULL) field->name=stralloc(member->rscrname);
				else field->name=NULL;
			field->type=member->field_type;
			field->reset=TRUE;
			switch(field->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case ZIPCODE:
				case PHONE:
				case CUSTOMTYPE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					if(member->value.string_value!=NULL)
					{
						field->data.string_value=
							stralloc(member->value.string_value);
					} else field->data.string_value=NULL;
					break;
				case BOOLNS:
					field->data.string_value=Rmalloc(1);
					if(member->value.string_value!=NULL)
					{
						*field->data.string_value=*member->value.string_value;
					} else *field->data.string_value=0;
					break;
				case CHARACTERS:
					field->data.string_value=Rmalloc(1);
					if(member->value.string_value!=NULL)
					{
						*field->data.string_value=*member->value.string_value;
					} else *field->data.string_value=0;
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					field->data.float_value=Rmalloc(sizeof(double));
					*field->data.float_value=*member->value.float_value;
					break;
				case SHORTV:
				case SSHORTV:
					field->data.short_value=Rmalloc(sizeof(short));
					*field->data.short_value=*member->value.short_value;
					break;
				case SCROLLEDLIST:
					field->data.integer_value=Rmalloc(sizeof(int));
					*field->data.integer_value=*member->value.integer_value;
					field->items=member->items;
					break;
				case LONGV:
				case SLONGV:
					field->data.integer_value=Rmalloc(sizeof(int));
					*field->data.integer_value=*member->value.integer_value;
					break;
				case BUTTONS:
					break;
				default:
					prterr("Error Field Type [%d] is invalid for Default Screen Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
					break;
			}
			++d->num;
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field) SEEDEFAULTS(d);
#endif /* USE_RDA_DIAGNOSTICS */
	return(d);
}
