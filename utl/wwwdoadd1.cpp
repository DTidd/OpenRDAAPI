/* runadd.c - Run Power Add */
/*COMPMEM 36864 */
/*lint -library */
#include <cstdio>


#include <mix.hpp>
#include <rptgen.hpp>
#include <powadd.hpp>

static void doexit(PowerAdd *);
static void save_check(RDArsrc *,PowerAdd *,short);
static void save_record_now(RDArsrc *,PowerAdd *);
static void save_record(RDArsrc *,PowerAdd *,short);
static void quit_record(RDArsrc *,PowerAdd *);
static void quit_test(RDArsrc *,PowerAdd *);
static void readscreen(RDArsrc *,PowerAdd *);
static void resetdefs(RDArsrc *,PowerAdd *);
static void SETFILES(RDArsrc *,PowerAdd *); 
static void SETNEWIDANDFILES(RDArsrc *,AddEditWdgt *);
void setpadddeftype(RDArmem *,PowerAdd *);
void setpaddacctype(RDArmem *,PowerAdd *);
void doadd(int,void (*)(...),RDArsrc *,void *); 

static void Set_Skip_Widgets(PowerAdd *p)
{
	RDAaddsub *addsub=NULL,*addsub1=NULL;
	int x,y;
	short count=0;

	if(p->subord!=NULL)
	{
		for(x=0,addsub=p->subord;x<p->subnum;++x,++addsub)
		{
			count=0;
			for(y=0,addsub1=p->subord;y<p->subnum;++y,++addsub1)
			{
				if(!RDAstrcmp(addsub->module,addsub1->module) && 
					!RDAstrcmp(addsub->filename,addsub1->filename))
				{
					 ++count;
					if(count>1) addsub1->skip_widget=TRUE;
				}
			}
		}
	}
}
static void PP_translate_POWERADD(PowerAdd *p)
{
	AddEditWdgt *AE=NULL;
	int x,y;
	char *temp=NULL;
	AddWrite *AW=NULL;
	AddWriteField *AWF=NULL;
	MaintainButton *BTN=NULL;
	RDAaddsub *addsub=NULL;

	if(p!=NULL)
	{
		if(p->subord!=NULL)
		{
			for(x=0,addsub=p->subord;x<p->subnum;++x,++addsub)
			{
				if(!isEMPTY(addsub->expression))
				{
					temp=PP_translate(addsub->expression);
					Rfree(addsub->expression);
					if(temp!=NULL)
					{
						addsub->expression=stralloc(temp);
						Rfree(temp);
					}
				}
			}
		}
		if(p->EditWdgts!=NULL)
		{
			for(x=0,AE=p->EditWdgts;x<p->numwdgts;++x,++AE)
			{
				if(!isEMPTY(AE->expression))
				{
					temp=PP_translate(AE->expression);
					Rfree(AE->expression);
					if(temp!=NULL)
					{
						AE->expression=stralloc(temp);
						Rfree(temp);
					}
				}
				if(!isEMPTY(AE->mgt_expression))
				{
					temp=PP_translate(AE->mgt_expression);
					Rfree(AE->mgt_expression);
					if(temp!=NULL)
					{
						AE->mgt_expression=stralloc(temp);
						Rfree(temp);
					}
				}
				if(!isEMPTY(AE->override_expression))
				{
					temp=PP_translate(AE->override_expression);
					Rfree(AE->override_expression);
					if(temp!=NULL)
					{
						AE->override_expression=stralloc(temp);
						Rfree(temp);
					}
				}
			}
		}
		if(!isEMPTY(p->save_error))
		{
			temp=PP_translate(p->save_error);
			Rfree(p->save_error);
			if(temp!=NULL)
			{
				p->save_error=stralloc(temp);
				Rfree(temp);
			}
		}
		if(!isEMPTY(p->save_error_desc))
		{
			temp=PP_translate(p->save_error_desc);
			Rfree(p->save_error_desc);
			if(temp!=NULL)
			{
				p->save_error_desc=stralloc(temp);
				Rfree(temp);
			}
		}
		if(!isEMPTY(p->save_warning))
		{
			temp=PP_translate(p->save_warning);
			Rfree(p->save_warning);
			if(temp!=NULL)
			{
				p->save_warning=stralloc(temp);
				Rfree(temp);
			}
		}
		if(!isEMPTY(p->save_warning_desc))
		{
			temp=PP_translate(p->save_warning_desc);
			Rfree(p->save_warning_desc);
			if(temp!=NULL)
			{
				p->save_warning_desc=stralloc(temp);
				Rfree(temp);
			}
		}
		if(!isEMPTY(p->quit_error))
		{
			temp=PP_translate(p->quit_error);
			Rfree(p->quit_error);
			if(temp!=NULL)
			{
				p->quit_error=stralloc(temp);
				Rfree(temp);
			}
		}
		if(!isEMPTY(p->save_expression))
		{
			temp=PP_translate(p->save_expression);
			Rfree(p->save_expression);
			if(temp!=NULL)
			{
				p->save_expression=stralloc(temp);
				Rfree(temp);
			}
		}
		if(!isEMPTY(p->quit_error_desc))
		{
			temp=PP_translate(p->quit_error_desc);
			Rfree(p->quit_error_desc);
			if(temp!=NULL)
			{
				p->quit_error_desc=stralloc(temp);
				Rfree(temp);
			}
		}
		if(!isEMPTY(p->quit_warning))
		{
			temp=PP_translate(p->quit_warning);
			Rfree(p->quit_warning);
			if(temp!=NULL)
			{
				p->quit_warning=stralloc(temp);
				Rfree(temp);
			}
		}
		if(!isEMPTY(p->quit_warning_desc))
		{
			temp=PP_translate(p->quit_warning_desc);
			Rfree(p->quit_warning_desc);
			if(temp!=NULL)
			{
				p->quit_warning_desc=stralloc(temp);
				Rfree(temp);
			}
		}
		if(p->Writes!=NULL)
		{
			for(x=0,AW=p->Writes;x<p->numWrites;++x,++AW)
			{
				if(!isEMPTY(AW->expression))
				{
					temp=PP_translate(AW->expression);
					Rfree(AW->expression);
					if(temp!=NULL) 
					{
						AW->expression=stralloc(temp);
						Rfree(temp);
					}	
					
				}
				if(AW->fields!=NULL)
				{
					for(y=0,AWF=AW->fields;y<AW->numflds;++y,++AWF)
					{
						if(AWF->copy_type==1)
						{
						if(!isEMPTY(AWF->from_name))
						{
							temp=PP_translate(AWF->from_name);
							Rfree(AWF->from_name);
							if(temp!=NULL)
							{
								AWF->from_name=stralloc(temp);
								Rfree(temp);
							}
						}
						}
					}
				}
			}
		}
		if(p->buttons!=NULL)
		{
			for(x=0,BTN=p->buttons;x<p->numbuttons;++x,++BTN)
			{
				if(!isEMPTY(BTN->expression))
				{
					temp=PP_translate(BTN->expression);
					Rfree(BTN->expression);
					if(temp!=NULL) 
					{
						BTN->expression=stralloc(temp);
						Rfree(temp);
					}	
					
				}
			}
		}
	}
}
static void doexit(PowerAdd *POWERADD)
{
	int level=0; 

	if(POWERADD!=NULL)
	{
		level=POWERADD->level;
		CLOSE_POWERADD_FILES(POWERADD);
		if(POWERADD!=NULL) FreePowerAdd(POWERADD);
		if(level==0)
		{
		 	ShutdownSubsystems();
		}
	}
}
#ifdef XXXX_GLOBAL
short POWERADDFIELDSubData(char **tmp,RDArsrc *r,char *modulename)
{
	char *filename=NULL,*fieldname=NULL;
	NRDfield *field=NULL;
	short start=1,length=0,fileno=(-1),occurrence=1,str_length=0;
	char *temp=NULL,*gn=NULL;

	if(diagvirtual || diageval)
	{
		prterr("DIAG POWERADDFIELDSubData Expression Evaluator for Virtual Fields on Screen [%s] [%s] Before Evaluating [%s].",r->module,r->screen,*tmp);
	}
	memset(stemp,0,101);
	filename=stripfilename(*tmp);
	fieldname=stripfieldname(*tmp);
	if(fieldname!=NULL)
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+RDAstrlen(filename)+RDAstrlen(fieldname)+6))
		{
			temp=*tmp+(RDAstrlen(modulename)+RDAstrlen(filename)+RDAstrlen(fieldname)+7);
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				occurrence=Ratoi(gn);
				} else occurrence=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		fileno=SCRNFILENUMBER(r,modulename,filename,occurrence);
		if(fileno!=(-1))
		{
			field=FLDNRDsec(fileno,fieldname,TRUE,FALSE,FALSE);
		} else field=NULL;
		if(field!=NULL)
		{
			switch(field->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
					temp=stralloc(field->data.string_value);
					str_length=(temp!=NULL?RDAstrlen(temp):0);
					if(length<1) length=str_length;
					if(RDAstrlen(*tmp)<(length+3))
					{
						*tmp=Rrealloc(*tmp,length+3);
					}
					if(!isEMPTY(temp) && str_length>length)
					{
						gn=temp+(start-1);
						if(RDAstrlen(gn)>(length))
							temp[start-1+length]=0;
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
					} else if(start<=str_length && !isEMPTY(temp))
					{
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
					} else {
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"\"");
					}
					if(temp!=NULL) Rfree(temp);
					temp=NULL;
					break;
				case SHORTV:
				case SSHORTV:
					if(*field->data.short_value<0)
					{
						sprintf(stemp,"(%d)",*field->data.short_value);
					} else {
						sprintf(stemp,"%d",*field->data.short_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<str_length)
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case SLONGV:
				case SCROLLEDLIST:
				case LONGV:
					if(*field->data.integer_value<0)
					{
						sprintf(stemp,"(%d)",*field->data.integer_value);
					} else {
						sprintf(stemp,"%d",*field->data.integer_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					if(*field->data.float_value<0)
					{
						sprintf(stemp,"(%f)",*field->data.float_value);
					} else {
						sprintf(stemp,"%f",*field->data.float_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case BOOLNS:
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"%d",(*field->data.string_value ? 1:0));
					break;
				case CHARACTERS:
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"%c",*field->data.string_value);
					break;
				default:
					prterr("Error Field type [%d] is invalid for [%s] file [%s].",field->type,field->name,FILENAME(fileno)); 
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=stralloc("\"\"");
					if(fieldname!=NULL) Rfree(fieldname);
					if(filename!=NULL) Rfree(filename);
					return(1);
			}
		} else {
			prterr("Error Field [%s] not found.",*tmp);
			if(*tmp!=NULL) Rfree(*tmp);
			*tmp=stralloc("\"\"");
			if(filename!=NULL) Rfree(filename);
			return(1);
		}
		if(fieldname!=NULL) Rfree(fieldname);
		if(filename!=NULL) Rfree(filename);
	} else {
		if(filename!=NULL) Rfree(filename);
		return(1);
	}
	return(0);
}
short POWERADDVIRTUALSubData(char **tmp,PowerAdd *POWERADD,char *modulename)
{
	RDArsrc *r=NULL;
	ADDPassableStruct *PSTRUCT=NULL;
	RDAvirtual *v=NULL;
	short start=1,length=0,str_length=0;
	char *temp=NULL,*gn=NULL;
	int y=0;

	if(POWERADD!=NULL)
	{
		PSTRUCT=(ADDPassableStruct *)POWERADD->passable;
	} else {
		return(1);
	}
	r=PSTRUCT->r;
	if(diagvirtual || diageval)
	{
		prterr("DIAG POWERADDVIRTUALSubData Expression Evaluator for Virtual Fields on Screen [%s] [%s] Before Evaluating [%s].",r->module,r->screen,*tmp);
	}
	if(r->virtual!=NULL)
	{
		for(y=0,v=r->virtual;y<r->num;++y,++v)
		{
			if(!RDAstrcmp(modulename,v->name)) break;
		}
		if(y<r->num)
		{
			switch(v->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					if(!v->computed) 
					{
						v->computed=TRUE;
						temp=PP_EVALUATEstr(v->expression,POWERADD->SubFunc,POWERADD);
						if(v->value.string_value!=NULL) 
							Rfree(v->value.string_value);
						v->value.string_value=temp;
						temp=NULL;
						if(v->value.string_value!=NULL)
						{
							v->dlen=RDAstrlen(v->value.string_value)+1;
							if(v->dlen>v->len && v->len>0)
							{
								v->value.string_value[v->len]=0;
							}
						}
					}
					if(RDAstrlen(*tmp)>(RDAstrlen(v->name)+2))
					{
						temp=*tmp+(RDAstrlen(v->name)+3);
						gn=strtok(temp,",]");
						if(gn!=NULL)
						{
							if(!isEMPTY(gn))
							{
							start=Ratoi(gn);
							} else start=1;
							gn=strtok(NULL,"]");
							if(!isEMPTY(gn))
							{
							length=Ratoi(gn);
							} else length=0;
						}
						if(start<1) start=1;

					}
					if(length==0) length=v->dlen;
					if(RDAstrlen(*tmp)<(length+3))
					{
						*tmp=Rrealloc(*tmp,length+3);
					}
					if((RDAstrlen(*tmp)>(RDAstrlen(v->name)+2)) && v->value.string_value!=NULL)
					{
						temp=stralloc(v->value.string_value);
						gn=temp+(start-1);
						if(RDAstrlen(gn)>(start-1+length))
							temp[start-1+length]=0;
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
						if(temp!=NULL) Rfree(temp);
						temp=NULL;
					} else {
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"%s\"",(v->value.string_value!=NULL ? v->value.string_value:""));
					}
					break;
				case SHORTV:
				case SSHORTV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.short_value=PP_EVALUATEsht(v->expression,POWERADD->SubFunc,POWERADD);
					}
					if(*v->value.short_value<0)
					{
						sprintf(stemp,"(%d)",*v->value.short_value);
					} else {
						sprintf(stemp,"%d",*v->value.short_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.integer_value=PP_EVALUATEint(v->expression,POWERADD->SubFunc,POWERADD);
					}
					if(*v->value.integer_value<0)
					{
						sprintf(stemp,"(%d)",*v->value.integer_value);
					} else {
						sprintf(stemp,"%d",*v->value.integer_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.float_value=PP_EVALUATEdbl(v->expression,POWERADD->SubFunc,POWERADD);
						if(v->type==DOLLARS || v->type==DOLLARS_NOCENTS ||
							v->type==DOUBLEV || v->type==SDOUBLEV)
						{
							*v->value.float_value=round(*v->value.float_value);
						}
					}
					if(*v->value.float_value<0)
					{
						sprintf(stemp,"(%f)",*v->value.float_value);
					} else {
						sprintf(stemp,"%f",*v->value.float_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case BOOLNS:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.string_value=PP_EVALUATEbol(v->expression,POWERADD->SubFunc,POWERADD);
					}
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"%d",(*v->value.string_value ? 1:0));
					break;
				case CHARACTERS:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.string_value=PP_EVALUATEbol(v->expression,POWERADD->SubFunc,POWERADD);
					}
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"%c",*v->value.string_value);
					break;
				default:
					prterr("Error Field type [%d] is invalid for virtual field [%s].",v->type,v->name); 
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=stralloc("\"\"");
					return(1);
			}
		} else {
			return(1);
/*
			if(ScrolledListSubData(tmp,r)==(-1))
			{
				prterr("Error Field [%s] not found.",*tmp);
				if(*tmp!=NULL) Rfree(*tmp);
				*tmp=stralloc("\"\"");
			}
*/
		}
	} else {
		return(1);
	} 
	return(0);
}
#ifdef XXXX_GLOBAL_OLD
short POWERADDFIELDSubData(char **tmp,RDArsrc *r,char *modulename)
{
	char *filename=NULL,*fieldname=NULL;
	NRDfield *field=NULL;
	short start=1,length=0,fileno=(-1),occurrence=1,str_length=0;
	char *temp=NULL,*gn=NULL;

	if(diagvirtual || diageval)
	{
		prterr("DIAG POWERADDFIELDSubData Expression Evaluator for Virtual Fields on Screen [%s] [%s] Before Evaluating [%s].",r->module,r->screen,*tmp);
	}
	memset(stemp,0,101);
	filename=stripfilename(*tmp);
	if(filename!=NULL)
	{
		fieldname=stripfieldname(*tmp);
		if(fieldname!=NULL)
		{
			if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+RDAstrlen(filename)+RDAstrlen(fieldname)+6))
			{
				temp=*tmp+(RDAstrlen(modulename)+RDAstrlen(filename)+RDAstrlen(fieldname)+7);
				gn=strtok(temp,",]");
				if(gn!=NULL)
				{
					if(!isEMPTY(gn))
					{
					occurrence=Ratoi(gn);
					} else occurrence=1;
					gn=strtok(NULL,",]");
					if(!isEMPTY(gn))
					{
					start=Ratoi(gn);
					} else start=1;
					gn=strtok(NULL,",]");
					if(!isEMPTY(gn))
					{
					length=Ratoi(gn);
					} else length=0;
				}
				if(start<1) start=1;
			}
			fileno=SCRNFILENUMBER(r,modulename,filename,occurrence);
			if(fileno!=(-1))
			{
				field=FLDNRDsec(fileno,fieldname,TRUE,FALSE,FALSE);
			} else field=NULL;
			if(field!=NULL)
			{
				switch(field->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case ZIPCODE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
						temp=stralloc(field->data.string_value);
						str_length=(temp!=NULL?RDAstrlen(temp):0);
						if(length<1) length=str_length;
						if(RDAstrlen(*tmp)<(length+3))
						{
							*tmp=Rrealloc(*tmp,length+3);
						}
						if(!isEMPTY(temp) && str_length>length)
						{
							gn=temp+(start-1);
							if(RDAstrlen(gn)>(length))
								temp[start-1+length]=0;
							memset(*tmp,0,RDAstrlen(*tmp)+1);
							sprintf(*tmp,"\"%s\"",&temp[start-1]);
						} else if(start<=str_length && !isEMPTY(temp))
						{
							memset(*tmp,0,RDAstrlen(*tmp)+1);
							sprintf(*tmp,"\"%s\"",&temp[start-1]);
						} else {
							memset(*tmp,0,RDAstrlen(*tmp)+1);
							sprintf(*tmp,"\"\"");
						}
						if(temp!=NULL) Rfree(temp);
						temp=NULL;
						break;
					case SHORTV:
					case SSHORTV:
						if(*field->data.short_value<0)
						{
							sprintf(stemp,"(%d)",*field->data.short_value);
						} else {
							sprintf(stemp,"%d",*field->data.short_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<str_length)
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						break;
					case SLONGV:
					case SCROLLEDLIST:
					case LONGV:
						if(*field->data.integer_value<0)
						{
							sprintf(stemp,"(%d)",*field->data.integer_value);
						} else {
							sprintf(stemp,"%d",*field->data.integer_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						if(*field->data.float_value<0)
						{
							sprintf(stemp,"(%f)",*field->data.float_value);
						} else {
							sprintf(stemp,"%f",*field->data.float_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						break;
					case BOOLNS:
						if(RDAstrlen(*tmp)<2)
						{
							*tmp=Rrealloc(*tmp,2);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"%d",(*field->data.string_value ? 1:0));
						break;
					case CHARACTERS:
						if(RDAstrlen(*tmp)<2)
						{
							*tmp=Rrealloc(*tmp,2);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"%c",*field->data.string_value);
						break;
					default:
						prterr("Error Field type [%d] is invalid for [%s] file [%s].",field->type,field->name,FILENAME(fileno)); 
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("\"\"");
						return(1);
				}
			} else {
				if(fieldname!=NULL) Rfree(fieldname);
				if(filename!=NULL) Rfree(filename);
				return(1);
			}
		} else {
			if(filename!=NULL) Rfree(filename);
			return(1);
		}
	} else {
		return(1);
	}
	return(0);
}
short POWERADDVIRTUALSubData(char **tmp,PowerAdd *POWERADD,char *modulename)
{
	RDArsrc *r=NULL;
	ADDPassableStruct *PSTRUCT=NULL;
	RDAvirtual *v=NULL;
	short start=1,length=0,str_length=0;
	char *temp=NULL,*gn=NULL;
	int y=0;

	if(POWERADD!=NULL)
	{
		PSTRUCT=(ADDPassableStruct *)POWERADD->passable;
	} else {
		return(1);
	}
	r=PSTRUCT->r;
	if(diagvirtual || diageval)
	{
		prterr("DIAG POWERADDVIRTUALSubData Expression Evaluator for Virtual Fields on Screen [%s] [%s] Before Evaluating [%s].",r->module,r->screen,*tmp);
	}
	if(r->virtual!=NULL && r->num>0)
	{
		for(y=0,v=r->virtual;y<r->num;++y,++v)
		{
			if(!RDAstrcmp(modulename,v->name)) break;
		}
		if(y<r->num)
		{
			switch(v->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					if(!v->computed) 
					{
						v->computed=TRUE;
						temp=PP_EVALUATEstr(v->expression,POWERADD->SubFunc,POWERADD);
						if(v->value.string_value!=NULL) 
						{
							Rfree(v->value.string_value);
						}
						v->value.string_value=temp;
						temp=NULL;
						if(v->value.string_value!=NULL)
						{
							v->dlen=RDAstrlen(v->value.string_value)+1;
							if(v->dlen>v->len && v->len>0)
							{
								v->value.string_value[v->len]=0;
							}
						}
					}
					if(RDAstrlen(*tmp)>(RDAstrlen(v->name)+2))
					{
						temp=*tmp+(RDAstrlen(v->name)+3);
						gn=strtok(temp,",]");
						if(gn!=NULL)
						{
							if(!isEMPTY(gn))
							{
							start=Ratoi(gn);
							} else start=1;
							gn=strtok(NULL,"]");
							if(!isEMPTY(gn))
							{
							length=Ratoi(gn);
							} else length=0;
						}
						if(start<1) start=1;

					}
					if(length==0) length=v->dlen;
					if(RDAstrlen(*tmp)<(length+3))
					{
						*tmp=Rrealloc(*tmp,length+3);
					}
					if((RDAstrlen(*tmp)>(RDAstrlen(v->name)+2)) && v->value.string_value!=NULL)
					{
						temp=stralloc(v->value.string_value);
						gn=temp+(start-1);
						if(RDAstrlen(gn)>(start-1+length))
							temp[start-1+length]=0;
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
						if(temp!=NULL) Rfree(temp);
						temp=NULL;
					} else {
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"%s\"",(v->value.string_value!=NULL ? v->value.string_value:""));
					}
					break;
				case SHORTV:
				case SSHORTV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.short_value=PP_EVALUATEsht(v->expression,POWERADD->SubFunc,POWERADD);
					}
					if(*v->value.short_value<0)
					{
						sprintf(stemp,"(%d)",*v->value.short_value);
					} else {
						sprintf(stemp,"%d",*v->value.short_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.integer_value=PP_EVALUATEint(v->expression,POWERADD->SubFunc,POWERADD);
					}
					if(*v->value.integer_value<0)
					{
						sprintf(stemp,"(%d)",*v->value.integer_value);
					} else {
						sprintf(stemp,"%d",*v->value.integer_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.float_value=PP_EVALUATEdbl(v->expression,POWERADD->SubFunc,POWERADD);
						if(v->type==DOLLARS || v->type==DOLLARS_NOCENTS ||
							v->type==DOUBLEV || v->type==SDOUBLEV)
						{
							*v->value.float_value=round(*v->value.float_value);
						}
					}
					if(*v->value.float_value<0)
					{
						sprintf(stemp,"(%f)",*v->value.float_value);
					} else {
						sprintf(stemp,"%f",*v->value.float_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case BOOLNS:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.string_value=PP_EVALUATEbol(v->expression,POWERADD->SubFunc,POWERADD);
					}
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"%d",(*v->value.string_value ? 1:0));
					break;
				case CHARACTERS:
					if(!v->computed) 
					{
						v->computed=TRUE;
						*v->value.string_value=PP_EVALUATEbol(v->expression,POWERADD->SubFunc,POWERADD);
					}
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"%c",*v->value.string_value);
					break;
				default:
					prterr("Error Field type [%d] is invalid for virtual field [%s].",v->type,v->name); 
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=stralloc("\"\"");
					return(1);
			}
		} else {
			return(1);
		}
	} else {
		return(1);
	}
	return(0);
}
#endif /* ifdef XXXX_GLOBAL_OLD */ 
void POWERADDSubData(char **tmp,PowerAdd *POWERADD)
{
	RDArsrc *r=NULL;
	char *modulename=NULL;
	ADDPassableStruct *PSTRUCT=NULL;

	if(POWERADD!=NULL)
	{
		PSTRUCT=(ADDPassableStruct *)POWERADD->passable;
	}
	r=PSTRUCT->r;
	if(diagvirtual || diageval)
	{
		prterr("DIAG POWERADDSubData Expression Evaluator for Virtual Fields on Screen [%s] [%s] Before Evaluating [%s].",r->module,r->screen,*tmp);
	}
	modulename=stripmodulename(*tmp);
	if(modulename!=NULL)
	{
		if(POWERADDFIELDSubData(tmp,r,modulename)==0)
		{
			if(diagvirtual|| diageval)
			{
				prterr("DIAG POWERADDSubData Returning [%s].",*tmp);
			}
			if(modulename!=NULL) Rfree(modulename);
			return;
		} else {
			if(diagvirtual|| diageval)
			{
				prterr("DIAG POWERADDFIELDSubData Didn't Find Substitution for [%s].",*tmp);
			}
		}
		if(GLOBALSubData(tmp,modulename)==0)
		{
			if(diagvirtual|| diageval)
			{
				prterr("DIAG POWERADDSubData Returning [%s].",*tmp);
			}
			if(modulename!=NULL) Rfree(modulename);
			return;
		} else {
			if(diagvirtual|| diageval)
			{
				prterr("DIAG GLOBALSubData Didn't Find Substitution for [%s].",*tmp);
			}
		}
		if(POWERADDVIRTUALSubData(tmp,POWERADD,modulename)==0)
		{
			if(diagvirtual|| diageval)
			{
				prterr("DIAG POWERADDSubData Returning [%s].",*tmp);
			}
			if(modulename!=NULL) Rfree(modulename);
			return;
		} else {
			if(diagvirtual|| diageval)
			{
				prterr("DIAG POWERADDVIRTUALSubData Didn't Find Substitution for [%s].",*tmp);
			}
		}
		if(AddEditWdgtSubData(tmp,POWERADD)==0)
		{
			if(diagvirtual|| diageval)
			{
				prterr("DIAG POWERADDSubData Returning [%s].",*tmp);
			}
			if(modulename!=NULL) Rfree(modulename);
			return;
		} else {
			if(diagvirtual|| diageval)
			{
				prterr("DIAG AddEditWdgtSubData Didn't Find Substitution for [%s].",*tmp);
			}
		}
		if(ScrolledListSubData(tmp,r)!=(-1))
		{
			if(diagvirtual|| diageval)
			{
				prterr("DIAG POWERADDSubData Returning [%s].",*tmp);
			}
			if(modulename!=NULL) Rfree(modulename);
			return;
		} else {
			if(diagvirtual|| diageval)
			{
				prterr("DIAG ScrolledListSubData Didn't Find Substitution for [%s].",*tmp);
			}
		}
		prterr("Error POWERADDSubData [%s] not found.",*tmp);
		if(*tmp!=NULL) Rfree(*tmp);
		*tmp=stralloc("\"\"");
		if(diagvirtual || diageval)
		{
			prterr("DIAG POWERADDSubData Returning [%s].",*tmp);
		}
		if(modulename!=NULL) Rfree(modulename);
		return;
	} else {
		prterr("Error POWERADDSubData [%s] not found.",*tmp);
		if(*tmp!=NULL) Rfree(*tmp);
		*tmp=stralloc("\"\"");
		if(diagvirtual || diageval)
		{
			prterr("DIAG POWERADDSubData Returning [%s].",*tmp);
		}
		if(modulename!=NULL) Rfree(modulename);
	}
	if(diagvirtual || diageval)
	{
		prterr("DIAG POWERADDSubData for Screen [%s] [%s] After Evaluating [%s].",r->module,r->screen,*tmp);
	}
}
/*
void POWERADDSubDataGUI(char *tmp,PowerAdd *POWERADD)
{
	POWERADDSubData(&tmp,POWERADD);
}
*/
#endif /* ifdef XXXX_GLOBAL */
#ifndef XXXX_GLOBAL
void POWERADDSubData(char **tmp,PowerAdd *POWERADD)
{
	RDArsrc *r=NULL;
	char *modulename=NULL,*filename=NULL,*fieldname=NULL;
	NRDfield *field=NULL;
	short start=1,length=0,fileno=(-1),occurrence=1,str_length=0;
	char *temp=NULL,*gn=NULL;
	int y=0;
	RDAvirtual *v=NULL;
#ifdef XXXX_GLOBAL_INT 
	RDAfinmgt *fmgt=NULL;
	RDApayroll *pay=NULL;
	RDAappmgt *appmgt=NULL;
	RDApptmgt *pptmgt=NULL;
	RDApurord *purord=NULL;
#endif /* XXXX_GLOBAL_INT */
	ADDPassableStruct *PSTRUCT=NULL;

	if(POWERADD!=NULL)
	{
		PSTRUCT=(ADDPassableStruct *)POWERADD->passable;
	}
	r=PSTRUCT->r;
	if(diagvirtual)
	{
		prterr("DIAG POWERADDSubData Expression Evaluator for Virtual Fields on Screen [%s] [%s] Before Evaluating [%s].",r->module,r->screen,*tmp);
	}
	modulename=stripmodulename(*tmp);
	if(modulename!=NULL)
	{
/* BREAK_HERE fields */
	memset(stemp,0,101);
	filename=stripfilename(*tmp);
	fieldname=stripfieldname(*tmp);
	if(fieldname!=NULL)
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+RDAstrlen(filename)+RDAstrlen(fieldname)+6))
		{
			temp=*tmp+(RDAstrlen(modulename)+RDAstrlen(filename)+RDAstrlen(fieldname)+7);
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				occurrence=Ratoi(gn);
				} else occurrence=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		fileno=SCRNFILENUMBER(r,modulename,filename,occurrence);
		if(fileno!=(-1))
		{
			field=FLDNRDsec(fileno,fieldname,TRUE,FALSE,FALSE);
		} else field=NULL;
		if(field!=NULL)
		{
			switch(field->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
					temp=stralloc(field->data.string_value);
					str_length=(temp!=NULL?RDAstrlen(temp):0);
					if(length<1) length=str_length;
					if(RDAstrlen(*tmp)<(length+3))
					{
						*tmp=Rrealloc(*tmp,length+3);
					}
					if(!isEMPTY(temp) && str_length>length)
					{
						gn=temp+(start-1);
						if(RDAstrlen(gn)>(length))
							temp[start-1+length]=0;
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
					} else if(start<=str_length && !isEMPTY(temp))
					{
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
					} else {
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"\"");
					}
					if(temp!=NULL) Rfree(temp);
					temp=NULL;
					break;
				case SHORTV:
				case SSHORTV:
					if(*field->data.short_value<0)
					{
						sprintf(stemp,"(%d)",*field->data.short_value);
					} else {
						sprintf(stemp,"%d",*field->data.short_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<str_length)
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case SLONGV:
				case SCROLLEDLIST:
				case LONGV:
					if(*field->data.integer_value<0)
					{
						sprintf(stemp,"(%d)",*field->data.integer_value);
					} else {
						sprintf(stemp,"%d",*field->data.integer_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					if(*field->data.float_value<0)
					{
						sprintf(stemp,"(%f)",*field->data.float_value);
					} else {
						sprintf(stemp,"%f",*field->data.float_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case BOOLNS:
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"%d",(*field->data.string_value ? 1:0));
					break;
				case CHARACTERS:
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"%c",*field->data.string_value);
					break;
				default:
					prterr("Error Field type [%d] is invalid for [%s] file [%s].",field->type,field->name,FILENAME(fileno)); 
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=stralloc("\"\"");
					break;
			}
		} else {
			prterr("Error Field [%s] not found.",*tmp);
			if(*tmp!=NULL) Rfree(*tmp);
			*tmp=stralloc("\"\"");
		}
		if(fieldname!=NULL) Rfree(fieldname);
		if(filename!=NULL) Rfree(filename);
/* BREAK_HERE Globals */
	} else if(GLOBALSubData(tmp,modulename)==0)
	{
#ifdef XXXX_GLOBAL_INT
	} else if(!RDAstrcmp(modulename,"LOGIN IDENTIFICATION"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=stralloc(USERLOGIN);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	} else if(!RDAstrcmp(modulename,"STARTING FISCAL MONTH"))
	{
		memset(stemp,0,101);
		fmgt=RDAfinmgtNEW();
		getfinmgtbin(fmgt);
		sprintf(stemp,"%d",fmgt->month);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_finmgt(fmgt);
		fmgt=NULL;
	} else if(!RDAstrcmp(modulename,"PAY BALANCE OF CONTRACT"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(Pay_Balance_of_Contract ? TRUE:FALSE));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
	} else if(!RDAstrcmp(modulename,"DUMP DEFERRED PAY BALANCE"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",(Dump_Deferred_Pay_Balance ? TRUE:FALSE));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
	} else if(!RDAstrcmp(modulename,"PURORD SETUP ADMIN FEE"))
	{
		purord=RDApurordNEW();
		getpurordbin(purord);
		if(RDAstrlen(*tmp)<(RDAstrlen(purord->admin_fee)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(purord->admin_fee)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+3);
		sprintf(*tmp,"\"%s\"",(purord->admin_fee!=NULL ? 
			purord->admin_fee:""));
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP USE TAXES"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%d",purord->Use_Taxes);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP USE TAX1"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%d",purord->Use_Tax1);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP SEPERATE TAX1 VENDOR"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%d",purord->Sep_Tax1_Vendor);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP TAX1 RATE"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%f",purord->Tax1_Rate);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP DEFAULT TAX1 VENDOR"))
	{
		purord=RDApurordNEW();
		getpurordbin(purord);
		if(RDAstrlen(*tmp)<(RDAstrlen(purord->Default_Tax1_Vendor)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(purord->Default_Tax1_Vendor)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+3);
		sprintf(*tmp,"\"%s\"",(purord->Default_Tax1_Vendor!=NULL ? purord->Default_Tax1_Vendor:""));
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP DEFAULT TAX1 ADDRESS"))
	{
		purord=RDApurordNEW();
		getpurordbin(purord);
		if(RDAstrlen(*tmp)<(RDAstrlen(purord->Default_Tax1_Address)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(purord->Default_Tax1_Address)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+3);
		sprintf(*tmp,"\"%s\"",(purord->Default_Tax1_Address!=NULL ? purord->Default_Tax1_Address:""));
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP USE TAX2"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%d",purord->Use_Tax2);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP SEPERATE TAX2 VENDOR"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%d",purord->Sep_Tax2_Vendor);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP TAX2 RATE"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%f",purord->Tax2_Rate);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP DEFAULT TAX2 VENDOR"))
	{
		purord=RDApurordNEW();
		getpurordbin(purord);
		if(RDAstrlen(*tmp)<(RDAstrlen(purord->Default_Tax2_Vendor)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(purord->Default_Tax2_Vendor)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+3);
		sprintf(*tmp,"\"%s\"",(purord->Default_Tax2_Vendor!=NULL ? purord->Default_Tax2_Vendor:""));
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP DEFAULT TAX2 ADDRESS"))
	{
		purord=RDApurordNEW();
		getpurordbin(purord);
		if(RDAstrlen(*tmp)<(RDAstrlen(purord->Default_Tax2_Address)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(purord->Default_Tax2_Address)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+3);
		sprintf(*tmp,"\"%s\"",(purord->Default_Tax2_Address!=NULL ? purord->Default_Tax2_Address:""));
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP USE TAX3"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%d",purord->Use_Tax3);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP SEPERATE TAX3 VENDOR"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%d",purord->Sep_Tax3_Vendor);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP TAX3 RATE"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%f",purord->Tax3_Rate);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP DEFAULT TAX3 VENDOR"))
	{
		purord=RDApurordNEW();
		getpurordbin(purord);
		if(RDAstrlen(*tmp)<(RDAstrlen(purord->Default_Tax3_Vendor)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(purord->Default_Tax3_Vendor)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+3);
		sprintf(*tmp,"\"%s\"",(purord->Default_Tax3_Vendor!=NULL ? purord->Default_Tax3_Vendor:""));
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP DEFAULT TAX3 ADDRESS"))
	{
		purord=RDApurordNEW();
		getpurordbin(purord);
		if(RDAstrlen(*tmp)<(RDAstrlen(purord->Default_Tax3_Address)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(purord->Default_Tax3_Address)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+3);
		sprintf(*tmp,"\"%s\"",(purord->Default_Tax3_Address!=NULL ? purord->Default_Tax3_Address:""));
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP USE TAX4"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%d",purord->Use_Tax4);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP SEPERATE TAX4 VENDOR"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%d",purord->Sep_Tax4_Vendor);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP TAX4 RATE"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%f",purord->Tax4_Rate);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP DEFAULT TAX4 VENDOR"))
	{
		purord=RDApurordNEW();
		getpurordbin(purord);
		if(RDAstrlen(*tmp)<(RDAstrlen(purord->Default_Tax4_Vendor)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(purord->Default_Tax4_Vendor)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+3);
		sprintf(*tmp,"\"%s\"",(purord->Default_Tax4_Vendor!=NULL ? purord->Default_Tax4_Vendor:""));
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP DEFAULT TAX4 ADDRESS"))
	{
		purord=RDApurordNEW();
		getpurordbin(purord);
		if(RDAstrlen(*tmp)<(RDAstrlen(purord->Default_Tax4_Address)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(purord->Default_Tax4_Address)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+3);
		sprintf(*tmp,"\"%s\"",(purord->Default_Tax4_Address!=NULL ? purord->Default_Tax4_Address:""));
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP USE TAX5"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%d",purord->Use_Tax5);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP SEPERATE TAX5 VENDOR"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%d",purord->Sep_Tax5_Vendor);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP TAX5 RATE"))
	{
		memset(stemp,0,101);
		purord=RDApurordNEW();
		getpurordbin(purord);
		sprintf(stemp,"%f",purord->Tax5_Rate);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"PURORD SETUP DEFAULT TAX5 VENDOR"))
	{
		purord=RDApurordNEW();
		getpurordbin(purord);
		if(RDAstrlen(*tmp)<(RDAstrlen(purord->Default_Tax5_Vendor)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(purord->Default_Tax5_Vendor)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+3);
		sprintf(*tmp,"\"%s\"",(purord->Default_Tax5_Vendor!=NULL ? purord->Default_Tax5_Vendor:""));
		free_purord(purord);
		purord=NULL;
	} else if(!RDAstrcmp(modulename,"USE PERSONNEL INTERVIEW"))
	{
		memset(stemp,0,101);
		appmgt=RDAappmgtNEW();
		getappmgtbin(appmgt);
		sprintf(stemp,"%d",appmgt->use_personnel_interview);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_appmgt(appmgt);
		appmgt=NULL;
	} else if(!RDAstrcmp(modulename,"USE ADMINISTRATIVE INTERVIEW"))
	{
		memset(stemp,0,101);
		appmgt=RDAappmgtNEW();
		getappmgtbin(appmgt);
		sprintf(stemp,"%d",appmgt->use_administrative_interview);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
		free_appmgt(appmgt);
		appmgt=NULL;
	} else if(!RDAstrcmp(modulename,"USE PAYROLL FUND"))
	{
		memset(stemp,0,101);
		pay=RDApayrollNEW();
		getpayrollbin(pay);
		sprintf(stemp,"%d",(pay->use_payroll_fund ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		free_payroll(pay);
		pay=NULL;
	} else if(!RDAstrcmp(modulename,"PAYSEL AUDIT"))
	{
		memset(stemp,0,101);
		pay=RDApayrollNEW();
		getpayrollbin(pay);
		sprintf(stemp,"%d",(pay->paysel_audit ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		free_payroll(pay);
		pay=NULL;
	} else if(!RDAstrcmp(modulename,"PAYNET AUDIT"))
	{
		memset(stemp,0,101);
		pay=RDApayrollNEW();
		getpayrollbin(pay);
		sprintf(stemp,"%d",(pay->paynet_audit ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		free_payroll(pay);
		pay=NULL;
	} else if(!RDAstrcmp(modulename,"PAYMTS AUDIT"))
	{
		memset(stemp,0,101);
		pay=RDApayrollNEW();
		getpayrollbin(pay);
		sprintf(stemp,"%d",(pay->paymts_audit ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		free_payroll(pay);
		pay=NULL;
	} else if(!RDAstrcmp(modulename,"SUBSEL AUDIT"))
	{
		memset(stemp,0,101);
		pay=RDApayrollNEW();
		getpayrollbin(pay);
		sprintf(stemp,"%d",(pay->subsel_audit ? 1:0));
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		free_payroll(pay);
		pay=NULL;
	} else if(!RDAstrcmp(modulename,"DIRECT DEPOSIT TYPE"))
	{
		memset(stemp,0,101);
		pay=RDApayrollNEW();
		getpayrollbin(pay);
		sprintf(stemp,"%d",pay->direct_deposit_type);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		sprintf(*tmp,"%s",stemp);
		free_payroll(pay);
		pay=NULL;
	} else if(!RDAstrcmp(modulename,"EIN NUMBER"))
	{
		pay=RDApayrollNEW();
		getpayrollbin(pay);
		if(!isEMPTY(pay->ein))
		{
			if(RDAstrlen(*tmp)<(RDAstrlen(pay->ein)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(pay->ein)+3);
			}
			sprintf(*tmp,"\"%s\"",pay->ein);
		} else sprintf(*tmp,"\"\"");
		free_payroll(pay);
		pay=NULL;
	} else if(!RDAstrcmp(modulename,"PPTMGT COMMISSIONER"))
	{
		pptmgt=RDApptmgtNEW();
		getpptmgtbin(pptmgt);
		if(!isEMPTY(pptmgt->commissioner))
		{
			if(RDAstrlen(*tmp)<(RDAstrlen(pptmgt->commissioner)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(pptmgt->commissioner)+3);
			}
			sprintf(*tmp,"\"%s\"",pptmgt->commissioner);
		} else sprintf(*tmp,"\"\"");
		free_pptmgt(pptmgt);
		pptmgt=NULL;
	} else if(!RDAstrcmp(modulename,"PPTMGT COMMISSIONER PHONE"))
	{
		pptmgt=RDApptmgtNEW();
		getpptmgtbin(pptmgt);
		if(!isEMPTY(pptmgt->comm_phone))
		{
			if(RDAstrlen(*tmp)<(RDAstrlen(pptmgt->comm_phone)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(pptmgt->comm_phone)+3);
			}
			sprintf(*tmp,"\"%s\"",pptmgt->comm_phone);
		} else sprintf(*tmp,"\"\"");
		free_pptmgt(pptmgt);
		pptmgt=NULL;
	} else if(!RDAstrcmp(modulename,"PPTMGT COMMISSIONER ADDRESS 1"))
	{
		pptmgt=RDApptmgtNEW();
		getpptmgtbin(pptmgt);
		if(!isEMPTY(pptmgt->comm_addr1))
		{
			if(RDAstrlen(*tmp)<(RDAstrlen(pptmgt->comm_addr1)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(pptmgt->comm_addr1)+3);
			}
			sprintf(*tmp,"\"%s\"",pptmgt->comm_addr1);
		} else sprintf(*tmp,"\"\"");
		free_pptmgt(pptmgt);
		pptmgt=NULL;
	} else if(!RDAstrcmp(modulename,"PPTMGT COMMISSIONER ADDRESS 2"))
	{
		pptmgt=RDApptmgtNEW();
		getpptmgtbin(pptmgt);
		if(!isEMPTY(pptmgt->comm_addr2))
		{
			if(RDAstrlen(*tmp)<(RDAstrlen(pptmgt->comm_addr2)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(pptmgt->comm_addr2)+3);
			}
			sprintf(*tmp,"\"%s\"",pptmgt->comm_addr2);
		} else sprintf(*tmp,"\"\"");
		free_pptmgt(pptmgt);
		pptmgt=NULL;
	} else if(!RDAstrcmp(modulename,"PPTMGT COMMISSIONER CITY"))
	{
		pptmgt=RDApptmgtNEW();
		getpptmgtbin(pptmgt);
		if(!isEMPTY(pptmgt->comm_city))
		{
			if(RDAstrlen(*tmp)<(RDAstrlen(pptmgt->comm_city)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(pptmgt->comm_city)+3);
			}
			sprintf(*tmp,"\"%s\"",pptmgt->comm_city);
		} else sprintf(*tmp,"\"\"");
		free_pptmgt(pptmgt);
		pptmgt=NULL;
	} else if(!RDAstrcmp(modulename,"PPTMGT COMMISSIONER STATE"))
	{
		pptmgt=RDApptmgtNEW();
		getpptmgtbin(pptmgt);
		if(!isEMPTY(pptmgt->comm_state))
		{
			if(RDAstrlen(*tmp)<(RDAstrlen(pptmgt->comm_state)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(pptmgt->comm_state)+3);
			}
			sprintf(*tmp,"\"%s\"",pptmgt->comm_state);
		} else sprintf(*tmp,"\"\"");
		free_pptmgt(pptmgt);
		pptmgt=NULL;
	} else if(!RDAstrcmp(modulename,"PPTMGT COMMISSIONER ZIP"))
	{
		pptmgt=RDApptmgtNEW();
		getpptmgtbin(pptmgt);
		if(!isEMPTY(pptmgt->comm_zip))
		{
			if(RDAstrlen(*tmp)<(RDAstrlen(pptmgt->comm_zip)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(pptmgt->comm_zip)+3);
			}
			sprintf(*tmp,"\"%s\"",pptmgt->comm_zip);
		} else sprintf(*tmp,"\"\"");
		free_pptmgt(pptmgt);
		pptmgt=NULL;
	} else if(!RDAstrcmp(modulename,"PPTMGT TREASURER"))
	{
		pptmgt=RDApptmgtNEW();
		getpptmgtbin(pptmgt);
		if(!isEMPTY(pptmgt->treasurer))
		{
			if(RDAstrlen(*tmp)<(RDAstrlen(pptmgt->treasurer)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(pptmgt->treasurer)+3);
			}
			sprintf(*tmp,"\"%s\"",pptmgt->treasurer);
		} else sprintf(*tmp,"\"\"");
		free_pptmgt(pptmgt);
		pptmgt=NULL;
	} else if(!RDAstrcmp(modulename,"PPTMGT TREASURER PHONE"))
	{
		pptmgt=RDApptmgtNEW();
		getpptmgtbin(pptmgt);
		if(!isEMPTY(pptmgt->treas_phone))
		{
			if(RDAstrlen(*tmp)<(RDAstrlen(pptmgt->treas_phone)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(pptmgt->treas_phone)+3);
			}
			sprintf(*tmp,"\"%s\"",pptmgt->treas_phone);
		} else sprintf(*tmp,"\"\"");
		free_pptmgt(pptmgt);
		pptmgt=NULL;
	} else if(!RDAstrcmp(modulename,"PPTMGT TREASURER ADDRESS 1"))
	{
		pptmgt=RDApptmgtNEW();
		getpptmgtbin(pptmgt);
		if(!isEMPTY(pptmgt->treas_addr1))
		{
			if(RDAstrlen(*tmp)<(RDAstrlen(pptmgt->treas_addr1)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(pptmgt->treas_addr1)+3);
			}
			sprintf(*tmp,"\"%s\"",pptmgt->treas_addr1);
		} else sprintf(*tmp,"\"\"");
		free_pptmgt(pptmgt);
		pptmgt=NULL;
	} else if(!RDAstrcmp(modulename,"PPTMGT TREASURER ADDRESS 2"))
	{
		pptmgt=RDApptmgtNEW();
		getpptmgtbin(pptmgt);
		if(!isEMPTY(pptmgt->treas_addr2))
		{
			if(RDAstrlen(*tmp)<(RDAstrlen(pptmgt->treas_addr2)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(pptmgt->treas_addr2)+3);
			}
			sprintf(*tmp,"\"%s\"",pptmgt->treas_addr2);
		} else sprintf(*tmp,"\"\"");
		free_pptmgt(pptmgt);
		pptmgt=NULL;
	} else if(!RDAstrcmp(modulename,"PPTMGT TREASURER CITY"))
	{
		pptmgt=RDApptmgtNEW();
		getpptmgtbin(pptmgt);
		if(!isEMPTY(pptmgt->treas_city))
		{
			if(RDAstrlen(*tmp)<(RDAstrlen(pptmgt->treas_city)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(pptmgt->treas_city)+3);
			}
			sprintf(*tmp,"\"%s\"",pptmgt->treas_city);
		} else sprintf(*tmp,"\"\"");
		free_pptmgt(pptmgt);
		pptmgt=NULL;
	} else if(!RDAstrcmp(modulename,"PPTMGT TREASURER STATE"))
	{
		pptmgt=RDApptmgtNEW();
		getpptmgtbin(pptmgt);
		if(!isEMPTY(pptmgt->treas_state))
		{
			if(RDAstrlen(*tmp)<(RDAstrlen(pptmgt->treas_state)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(pptmgt->treas_state)+3);
			}
			sprintf(*tmp,"\"%s\"",pptmgt->treas_state);
		} else sprintf(*tmp,"\"\"");
		free_pptmgt(pptmgt);
		pptmgt=NULL;
	} else if(!RDAstrcmp(modulename,"PPTMGT TREASURER ZIP"))
	{
		pptmgt=RDApptmgtNEW();
		getpptmgtbin(pptmgt);
		if(!isEMPTY(pptmgt->treas_zip))
		{
			if(RDAstrlen(*tmp)<(RDAstrlen(pptmgt->treas_zip)))
			{
				*tmp=Rrealloc(*tmp,RDAstrlen(pptmgt->treas_zip)+3);
			}
			sprintf(*tmp,"\"%s\"",pptmgt->treas_zip);
		} else sprintf(*tmp,"\"\"");
		free_pptmgt(pptmgt);
		pptmgt=NULL;
	} else if(!RDAstrcmp(modulename,"RLSTMGT COMMISSIONER"))
	{
	} else if(!RDAstrcmp(modulename,"RLSTMGT COMMISSIONER PHONE"))
	{
	} else if(!RDAstrcmp(modulename,"RLSTMGT COMMISSIONER ADDRESS 1"))
	{
	} else if(!RDAstrcmp(modulename,"RLSTMGT COMMISSIONER ADDRESS 2"))
	{
	} else if(!RDAstrcmp(modulename,"RLSTMGT COMMISSIONER CITY"))
	{
	} else if(!RDAstrcmp(modulename,"RLSTMGT COMMISSIONER STATE"))
	{
	} else if(!RDAstrcmp(modulename,"RLSTMGT COMMISSIONER ZIP"))
	{
	} else if(!RDAstrcmp(modulename,"RLSTMGT TREASURER"))
	{
	} else if(!RDAstrcmp(modulename,"RLSTMGT TREASURER PHONE"))
	{
	} else if(!RDAstrcmp(modulename,"RLSTMGT TREASURER ADDRESS 1"))
	{
	} else if(!RDAstrcmp(modulename,"RLSTMGT TREASURER ADDRESS 2"))
	{
	} else if(!RDAstrcmp(modulename,"RLSTMGT TREASURER CITY"))
	{
	} else if(!RDAstrcmp(modulename,"RLSTMGT TREASURER STATE"))
	{
	} else if(!RDAstrcmp(modulename,"RLSTMGT TREASURER ZIP"))
	{
	} else if(!RDAstrcmp(modulename,"SYSTEM DATE"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=GETCURRENTDATE();
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	} else if(!RDAstrcmp(modulename,"SYSTEM DATE-YYYY"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=GETCURRENTDATE10();
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	} else if(!RDAstrcmp(modulename,"SYSTEM TIME"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=GETCURRENTTIME();
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	} else if(!RDAstrcmp(modulename,"CURRENT DIRECTORY"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
#ifdef WIN32
		temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+2);
		sprintf(temp,"%s\\",CURRENTDIRECTORY);
#endif
#ifndef WIN32
		temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+2);
		sprintf(temp,"%s/",CURRENTDIRECTORY);
#endif
/*
		temp=stralloc(CURRENTDIRECTORY);
*/
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	} else if(!RDAstrcmp(modulename,"SYSTEM TYPE"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
#if defined(WIN32)
		temp=stralloc("WIN32");
#elif defined(UNIXWARE7)
		temp=stralloc("UNIXWARE7");
#elif defined(UNIXWARE21)
		temp=stralloc("UNIXWARE21");
#elif defined(SCO50P)
		temp=stralloc("SCO5OP");
#elif defined(SCO50)
		temp=stralloc("SCO5O");
#elif defined(SC0386)
		temp=stralloc("SCO386");
#elif defined(RS6000)
		temp=stralloc("RS6000");
#elif defined(LINUX)
		temp=stralloc("LINUX");
#elif defined(LINUX2_2) || defined(UBUNTU_OS)
		temp=stralloc("LINUX2_2");
#elif defined(SUNX86)
		temp=stralloc("SUNX86");
#else
		temp=stralloc("UNKNOWN");
#endif
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	} else if(!RDAstrcmp(modulename,"ORGANIZATION NAME"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=stralloc(XPERT_SETUP->company_name);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	} else if(!RDAstrcmp(modulename,"ORGANIZATION ADDRESS #1"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=stralloc(XPERT_SETUP->addr1);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	} else if(!RDAstrcmp(modulename,"ORGANIZATION ADDRESS #2"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=stralloc(XPERT_SETUP->addr2);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	} else if(!RDAstrcmp(modulename,"ORGANIZATION CITY"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=stralloc(XPERT_SETUP->city);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	} else if(!RDAstrcmp(modulename,"ORGANIZATION STATE"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=stralloc(XPERT_SETUP->state);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if((start<=str_length) && !isEMPTY(temp))
		{
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	} else if(!RDAstrcmp(modulename,"ORGANIZATION ZIP"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=1;
			}
			if(start<1) start=1;
		}
		temp=stralloc(XPERT_SETUP->zip);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL; 
	} else if(!RDAstrcmp(modulename,"ORGANIZATION FAX"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=stralloc(XPERT_SETUP->fax);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	} else if(!RDAstrcmp(modulename,"USE CENTURY THRESHOLD"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",XPERT_SETUP->use_century_threshold);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
	} else if(!RDAstrcmp(modulename,"CENTURY THRESHOLD"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",XPERT_SETUP->century_threshold);
		if(RDAstrlen(*tmp)<(RDAstrlen(stemp)))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(stemp)+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"%s",stemp);
	} else if(!RDAstrcmp(modulename,"ORGANIZATION PHONE"))
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
		{
			temp=*tmp+RDAstrlen(modulename)+3;
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
				start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
				length=Ratoi(gn);
				} else length=0;
			}
			if(start<1) start=1;
		}
		temp=stralloc(XPERT_SETUP->phone);
		str_length=(temp!=NULL ? RDAstrlen(temp):0);
		if(length==0) length=str_length;
		if(RDAstrlen(*tmp)<(length+3))
		{
			*tmp=Rrealloc(*tmp,length+3);
		}
		if(!isEMPTY(temp) && str_length>length)
		{
			gn=temp+(start-1);
			if(RDAstrlen(gn)>(length)) temp[start-1+length]=0;
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else if(start<=str_length && !isEMPTY(temp))
		{
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"%s\"",&temp[start-1]);
		} else {
			memset(*tmp,0,RDAstrlen(*tmp)+1);
			sprintf(*tmp,"\"\"");
		}
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
#endif /* ifdef XXXX_GLOBAL_INT */
/* BREAK_HERE Edit Widgets */
	} else if(!AddEditWdgtSubData(tmp,POWERADD))
	{
/* OK */
	} else {/* virtual field */
/* BREAK_HERE Virtuals */
		if(r->virtual!=NULL)
		{
			for(y=0,v=r->virtual;y<r->num;++y,++v)
			{
				if(!RDAstrcmp(modulename,v->name)) break;
			}
			if(y<r->num)
			{
				switch(v->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case ZIPCODE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						if(!v->computed) 
						{
							v->computed=TRUE;
							temp=PP_EVALUATEstr(v->expression,POWERADD->SubFunc,POWERADD);
							if(v->value.string_value!=NULL) 
								Rfree(v->value.string_value);
							v->value.string_value=temp;
							temp=NULL;
							if(v->value.string_value!=NULL)
							{
								v->dlen=RDAstrlen(v->value.string_value)+1;
								if(v->dlen>v->len && v->len>0)
								{
									v->value.string_value[v->len]=0;
								}
							}
						}
						if(RDAstrlen(*tmp)>(RDAstrlen(v->name)+2))
						{
							temp=*tmp+(RDAstrlen(v->name)+3);
							gn=strtok(temp,",]");
							if(gn!=NULL)
							{
								if(!isEMPTY(gn))
								{
								start=Ratoi(gn);
								} else start=1;
								gn=strtok(NULL,"]");
								if(!isEMPTY(gn))
								{
								length=Ratoi(gn);
								} else length=0;
							}
							if(start<1) start=1;

						}
						if(length==0) length=v->dlen;
						if(RDAstrlen(*tmp)<(length+3))
						{
							*tmp=Rrealloc(*tmp,length+3);
						}
						if((RDAstrlen(*tmp)>(RDAstrlen(v->name)+2)) && v->value.string_value!=NULL)
						{
							temp=stralloc(v->value.string_value);
							gn=temp+(start-1);
							if(RDAstrlen(gn)>(start-1+length))
								temp[start-1+length]=0;
							memset(*tmp,0,RDAstrlen(*tmp)+1);
							sprintf(*tmp,"\"%s\"",&temp[start-1]);
							if(temp!=NULL) Rfree(temp);
							temp=NULL;
						} else {
							memset(*tmp,0,RDAstrlen(*tmp)+1);
							sprintf(*tmp,"\"%s\"",(v->value.string_value!=NULL ? v->value.string_value:""));
						}
						break;
					case SHORTV:
					case SSHORTV:
						if(!v->computed) 
						{
							v->computed=TRUE;
							*v->value.short_value=PP_EVALUATEsht(v->expression,POWERADD->SubFunc,POWERADD);
						}
						if(*v->value.short_value<0)
						{
							sprintf(stemp,"(%d)",*v->value.short_value);
						} else {
							sprintf(stemp,"%d",*v->value.short_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						if(!v->computed) 
						{
							v->computed=TRUE;
							*v->value.integer_value=PP_EVALUATEint(v->expression,POWERADD->SubFunc,POWERADD);
						}
						if(*v->value.integer_value<0)
						{
							sprintf(stemp,"(%d)",*v->value.integer_value);
						} else {
							sprintf(stemp,"%d",*v->value.integer_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						if(!v->computed) 
						{
							v->computed=TRUE;
							*v->value.float_value=PP_EVALUATEdbl(v->expression,POWERADD->SubFunc,POWERADD);
							if(v->type==DOLLARS || v->type==DOLLARS_NOCENTS ||
								v->type==DOUBLEV || v->type==SDOUBLEV)
							{
								*v->value.float_value=round(*v->value.float_value);
							}
						}
						if(*v->value.float_value<0)
						{
							sprintf(stemp,"(%f)",*v->value.float_value);
						} else {
							sprintf(stemp,"%f",*v->value.float_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<RDAstrlen(stemp))
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						memcpy(*tmp,stemp,str_length+1);
						break;
					case BOOLNS:
						if(!v->computed) 
						{
							v->computed=TRUE;
							*v->value.string_value=PP_EVALUATEbol(v->expression,POWERADD->SubFunc,POWERADD);
						}
						if(RDAstrlen(*tmp)<2)
						{
							*tmp=Rrealloc(*tmp,2);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"%d",(*v->value.string_value ? 1:0));
						break;
					case CHARACTERS:
						if(!v->computed) 
						{
							v->computed=TRUE;
							*v->value.string_value=PP_EVALUATEbol(v->expression,POWERADD->SubFunc,POWERADD);
						}
						if(RDAstrlen(*tmp)<2)
						{
							*tmp=Rrealloc(*tmp,2);
						}
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"%c",*v->value.string_value);
						break;
					default:
						prterr("Error Field type [%d] is invalid for virtual field [%s].",v->type,v->name); 
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("\"\"");
						break;
				}
			} else {
				if(ScrolledListSubData(tmp,r)==(-1))
				{
					prterr("Error Field [%s] not found.",*tmp);
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=stralloc("\"\"");
				}
			}
		} else {
/* BREAK_HERE ScrolledList */
			if(ScrolledListSubData(tmp,r)==(-1))
			{
				prterr("Error Field [%s] not found.",*tmp);
				if(*tmp!=NULL) Rfree(*tmp);
				*tmp=stralloc("\"\"");
			}
		}
	}
	Rfree(modulename);
	}
	if(diagvirtual)
	{
		prterr("DIAG POWERADDSubData for Screen [%s] [%s] After Evaluating [%s].",r->module,r->screen,*tmp);
	}
}
#endif /* ifndef XXXX_GLOBAL */
short AddEditWdgtSubData(char **tmp,PowerAdd *POWERADD)
{
	AddEditWdgt *EditWdgt=NULL;
	char *xname=NULL,*temp=NULL,*gn;
	int length=0,x,str_length=0,start=1;

	if(POWERADD!=NULL)
	{
		xname=stripmodulename(*tmp);
		for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;
			++x,++EditWdgt)
		{
			if(!RDAstrcmp(EditWdgt->name,xname)) break;
		}
		if(x<POWERADD->numwdgts)
		{
			if(RDAstrlen(*tmp)>(RDAstrlen(xname)+2))
			{
				temp=*tmp+(RDAstrlen(xname)+3);
				gn=strtok(temp,",]");
				if(gn!=NULL)
				{
					if(!isEMPTY(gn))
					{
					start=Ratoi(gn);
					} else start=1;
					gn=strtok(NULL,",]");
					if(!isEMPTY(gn))
					{
					length=Ratoi(gn);
					} else length=0;
				}
				if(start<1) start=1;
			}
			switch(EditWdgt->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
					temp=stralloc(EditWdgt->data.string_value);
					str_length=(temp!=NULL?RDAstrlen(temp):0);
					if(length<1) length=str_length;
					if(RDAstrlen(*tmp)<(length+3))
					{
						*tmp=Rrealloc(*tmp,length+3);
					}
					if(!isEMPTY(temp) && str_length>length)
					{
						gn=temp+(start-1);
						if(RDAstrlen(gn)>(length))
							temp[start-1+length]=0;
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
					} else if(start<=str_length && !isEMPTY(temp))
					{
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
					} else {
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"\"");
					}
					if(temp!=NULL) Rfree(temp);
					temp=NULL;
					break;
				case SHORTV:
				case SSHORTV:
					if(*EditWdgt->data.short_value<0)
					{
						sprintf(stemp,"(%d)",*EditWdgt->data.short_value);
					} else {
						sprintf(stemp,"%d",*EditWdgt->data.short_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<str_length)
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case SLONGV:
				case SCROLLEDLIST:
				case LONGV:
					if(*EditWdgt->data.integer_value<0)
					{
						sprintf(stemp,"(%d)",*EditWdgt->data.integer_value);
					} else {
						sprintf(stemp,"%d",*EditWdgt->data.integer_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					if(*EditWdgt->data.float_value<0)
					{
						sprintf(stemp,"(%f)",*EditWdgt->data.float_value);
					} else {
						sprintf(stemp,"%f",*EditWdgt->data.float_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<RDAstrlen(stemp))
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case BOOLNS:
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"%d",(*EditWdgt->data.string_value ? 1:0));
					break;
				case CHARACTERS:
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"%c",*EditWdgt->data.string_value);
					break;
				default:
					prterr("Error Field type [%d] is invalid for [%s].",EditWdgt->type,EditWdgt->name); 
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=stralloc("\"\"");
					return(-1);
			}
			if(xname!=NULL) Rfree(xname);
			return(0);
		}
		if(xname!=NULL) Rfree(xname);
	} else {
		prterr("No POWERADD Structure.");TRACE;
	}
	return(-1);
}
short DOADD_GUIEVAL(char *string,RDArsrc *r,PowerAdd *POWERADD)
{
	if(!isEMPTY(string))
	{
		return(EVALUATEbol(string,POWERADDSubData,(void *)POWERADD));
	} else {
		return(TRUE);
	}
}
char *DOADD_GUIEVALSTR(char *string,RDArsrc *r,void *SubFuncArgs,PowerAdd *POWERADD)
{
	char *tmp=NULL;

	if(!isEMPTY(string))
	{
		tmp=PP_EVALUATEstr(string,POWERADDSubData,(void *)POWERADD);
	}
	return(tmp);
}

void READEditWdgts(PowerAdd *POWERADD)
{
	char *temp=NULL,*dashes=NULL;
	int x=0;
	AddEditWdgt *EditWdgt=NULL;

	if(POWERADD!=NULL)
	{
		for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;
			++x,++EditWdgt)
		{
			if(!isEMPTY(EditWdgt->name))
			{
				dashes=adddashes(EditWdgt->name);
				temp=RDA_GetEnv(dashes);
				if(dashes!=NULL) Rfree(dashes);
			}
			if(!isEMPTY(temp))
			{
				switch(EditWdgt->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case ZIPCODE:
					case SOCSECNUM:
					case DATES:
					case TIMES:
					case EXPENDITURE:
					case BALANCESHEET:
					case REVENUE:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						if(EditWdgt->data.string_value!=NULL) Rfree(EditWdgt->data.string_value);
						EditWdgt->data.string_value=stralloc(temp);
						EditWdgt->dlen=RDAstrlen(EditWdgt->data.string_value)+1;
						break;
					case DECIMALV:
					case SDECIMALV:
					case DOLLARS_NOCENTS:
					case DOLLARS:
					case DOUBLEV:
					case SDOUBLEV:
						if(!isEMPTY(temp))
						{
						*EditWdgt->data.float_value=atof(temp);
						} else *EditWdgt->data.float_value=0;
						break;
					case SHORTV:
					case SSHORTV:
						if(!isEMPTY(temp))
						{
						*EditWdgt->data.short_value=Ratoi(temp);
						} else *EditWdgt->data.short_value=0;
						break;
					case LONGV:
					case SLONGV:
					case SCROLLEDLIST:
						if(!isEMPTY(temp))
						{
						*EditWdgt->data.integer_value=Ratoi(temp);
						} else *EditWdgt->data.integer_value=0;
						break;
					case BOOLNS:
					case CHARACTERS:
						*EditWdgt->data.string_value=*temp;
						break;
					case BUTTONS:
						prterr("Huh?....");
						break;
					default:
						prterr("Error Invalid Field Type [%d].",EditWdgt->type);
						break;
				}
				EditWdgt->etype=(-1);
			}
		}
	}
}
void setpadddeftype(RDArmem *member,PowerAdd *POWERADD)
{
	RDArmem *mem=NULL;
	short x;
	ADDPassableStruct *PSTRUCT=NULL;
	RDArsrc *rsrc=NULL;
	
	rsrc=(RDArsrc *)member->parent;
	if(POWERADD!=NULL)
	{
		PSTRUCT=(ADDPassableStruct *)POWERADD->passable;
		if(PSTRUCT!=NULL)
		{
			if(member!=NULL && rsrc!=NULL)
			{	
				readwidget(rsrc,member->rscrname);
				if(rsrc->rscs!=NULL)
				{
					for(x=0,mem=rsrc->rscs;x<rsrc->numrscs;++x,++mem)
					{
						if(mem->field_type==EXPENDITURE || mem->field_type==REVENUE || mem->field_type==BALANCESHEET || mem->field_type==BEGINNINGBALANCE)
						{
							switch(mem->field_type)
							{
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
									mem->items=*member->value.short_value;
									if(mem->dlen>0)
									{
										memset(mem->value.string_value,0,mem->dlen);
									} else {
										mem->value.string_value=NULL;
										mem->dlen=0;
									}
									updatemember(mem);
									break;
								default:
									prterr("Error Resource [%s] has the wrong field type [%d].",mem->rscrname,mem->field_type);
									break;
							}
						}
					}
				}
			}
			SET_POWERADD(rsrc,POWERADD,POWERADDSubData,(void *)POWERADD);
		} else {
			prterr("Error PSTRUCT (ADDPassableStruct) is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	} else {
		prterr("Error POWERADD is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}

}
void setpaddacctype(RDArmem *member,PowerAdd *POWERADD)
{
	RDArmem *mem=NULL;
	short x;
	ADDPassableStruct *PSTRUCT=NULL;
	RDArsrc *rsrc=NULL;

	rsrc=(RDArsrc *)member->parent;
	if(POWERADD!=NULL)
	{
		PSTRUCT=(ADDPassableStruct *)POWERADD->passable;
		if(PSTRUCT!=NULL)
		{
			if(rsrc!=NULL && member!=NULL)
			{
				if(!isEMPTY(member->rscrname))
				{
					readwidget(rsrc,member->rscrname);
					if(rsrc->rscs!=NULL)
					{
						for(x=0,mem=rsrc->rscs;x<rsrc->numrscs;++x,++mem)
						{
							if(!isEMPTY(mem->rscrname))
							{
								if((mem->field_type==EXPENDITURE || mem->field_type==REVENUE || mem->field_type==BALANCESHEET || mem->field_type==BEGINNINGBALANCE) && (RDAstrstr(mem->rscrname,"ACCOUNT CODE")!=NULL))
								{
									switch(mem->field_type)
									{
										case EXPENDITURE:
										case REVENUE:
										case BALANCESHEET:
										case BEGINNINGBALANCE:
											switch(*member->value.short_value)
											{
												case EXP_ACCT:
													mem->field_type=EXPENDITURE;
													if(mem->dlen>0)
													{
														memset(mem->value.string_value,0,mem->dlen);
													} else {
														mem->value.string_value=NULL;
														mem->dlen=0;
													}
													updatemember(mem);
													break;
												case REV_ACCT:
													mem->field_type=REVENUE;
													if(mem->dlen>0)
													{
														memset(mem->value.string_value,0,mem->dlen);
													} else {
														mem->value.string_value=NULL;
														mem->dlen=0;
													}
													updatemember(mem);
													break;
												case BAL_ACCT:
													mem->field_type=BALANCESHEET;
													if(mem->dlen>0)
													{
														memset(mem->value.string_value,0,mem->dlen);
													} else {
														mem->value.string_value=NULL;
														mem->dlen=0;
													}
													updatemember(mem);
													break;
												case BEG_ACCT:
													mem->field_type=BEGINNINGBALANCE;
													if(mem->dlen>0)
													{
														memset(mem->value.string_value,0,mem->dlen);
													} else {
														mem->value.string_value=NULL;
														mem->dlen=0;
													}
													updatemember(mem);
													break;
												default:
													prterr("Error Resource [%s] has the wrong acctype [%d].",member->rscrname,*member->value.short_value);
													return;
											}
											break;
										default:
											prterr("Error Resource [%s] has the wrong field type [%d].",mem->rscrname,mem->field_type);
											break;
									}
								}
							}
						}
					}
				}
			}
			SET_POWERADD(rsrc,POWERADD,POWERADDSubData,(void *)POWERADD);
		} else {
			prterr("Error PSTRUCT (ADDPassableStruct) is NULL at line [%d] program [%s].",__LINE__,__FILE__);
		}
	} else {
		prterr("Error POWERADD is NULL at line [%d] program [%s].",__LINE__,__FILE__);
	}
}
static void add_definelist(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	if(IsScreenDisplayed(POWERADD->ADD_DEFINE_LIST)) return;
	POWERADDcomputeallSCRNvirtuals(POWERADD->ADD_DEFINE_LIST,POWERADD,POWERADDSubData,(void *)POWERADD);
	if(ADVmakescrn(POWERADD->ADD_DEFINE_LIST,TRUE))
	{
		prterr("Error cannot create screen [%s][%s].",(POWERADD->ADD_DEFINE_LIST->module!=NULL?POWERADD->ADD_DEFINE_LIST->module:""),(POWERADD->ADD_DEFINE_LIST->screen!=NULL?POWERADD->ADD_DEFINE_LIST->screen:""));
	}
}
static void makedflist(PowerAdd *POWERADD)
{
	RDArmem *len_member=NULL,*pos_member=NULL;
	int x,cur=1,length=0,lcount=0;
	char *s=NULL,*hold=NULL,*temp=NULL;
	AddEditWdgt *EditWdgt=NULL;
	char strtemp[1024];
	
	
	if(POWERADD!=NULL)
	{
	if(POWERADD->PREVIOUSADD!=NULL)
	{
		if(POWERADD->PREVIOUSADD->numlibs==1 && !RDAstrcmp(POWERADD->PREVIOUSADD->libs[0],
			"Nothing Added this Session."))
		{
			freeapplib(POWERADD->PREVIOUSADD);
			POWERADD->PREVIOUSADD=APPlibNEW();
		}
	} else POWERADD->PREVIOUSADD=APPlibNEW();
	if(POWERADD->ADD_DEFINE_LIST!=NULL)
	{
		for(x=0,length=0;x<(POWERADD->ADD_DEFINE_LIST->numrscs-5);x+=2)
		{
			pos_member=POWERADD->ADD_DEFINE_LIST->rscs+x;
			len_member=POWERADD->ADD_DEFINE_LIST->rscs+(x+1);
			if(*pos_member->value.short_value>0 && *len_member->value.short_value>0)
			{
				length+=(*len_member->value.short_value+1);
			}		
		}	
	}
	hold=Rmalloc(length+1);
	lcount=0;
	cur=0;
	while(cur<(POWERADD->ADD_DEFINE_LIST->numrscs-5))
	{
		for(x=0;x<(POWERADD->ADD_DEFINE_LIST->numrscs-5);x+=2)
		{
			pos_member=POWERADD->ADD_DEFINE_LIST->rscs+x;
			len_member=POWERADD->ADD_DEFINE_LIST->rscs+(x+1);
			if(*pos_member->value.short_value>0)
			{
			if(*pos_member->value.short_value==cur &&
				*len_member->value.short_value>0)
			{
				s=stralloc(pos_member->rscrname);
				s[RDAstrlen(s)-9]=0;
				EditWdgt=FINDEDITWDGT(POWERADD,s);		
				if(EditWdgt!=NULL)
				{
					switch(EditWdgt->type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
						case PLAINTEXT:
						case PHONE:
						case CUSTOMTYPE:
						case ZIPCODE:
						case SOCSECNUM:
						case DATES:
						case TIMES:
						case EXPENDITURE:
						case BALANCESHEET:
						case REVENUE:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
							if(*len_member->value.short_value<RDAstrlen(EditWdgt->data.string_value))
							{
								sprintf(&hold[lcount],"%.*s ",*len_member->value.short_value,(EditWdgt->data.string_value!=NULL ? EditWdgt->data.string_value:""));
							} else {
								sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,(EditWdgt->data.string_value!=NULL ? EditWdgt->data.string_value:""));
							}
							break;
						case DECIMALV:
							memset(strtemp,0,1024);
							sprintf(strtemp,"%*f",*len_member->value.short_value,*EditWdgt->data.float_value);
							strtemp[*len_member->value.short_value]=0;
							sprintf(&hold[lcount],"%s ",strtemp);
							break;

						case SDECIMALV:
							memset(strtemp,0,1024);
							if(*EditWdgt->data.float_value>=0)
							{
								sprintf(strtemp,"%*f ",*len_member->value.short_value,*EditWdgt->data.float_value);
							} else {
								sprintf(strtemp,"%*f-",*len_member->value.short_value,(*EditWdgt->data.float_value)*(-1));
							}
							strtemp[*len_member->value.short_value]=0;
							sprintf(&hold[lcount],"%s ",strtemp);
							break;
						case SHORTV:
							memset(strtemp,0,1024);
							sprintf(strtemp,"%*d",*len_member->value.short_value,*EditWdgt->data.short_value);
							strtemp[*len_member->value.short_value]=0;
							sprintf(&hold[lcount],"%s ",strtemp);
							break;
						case SSHORTV:
							memset(strtemp,0,1024);
							if(*EditWdgt->data.short_value>=0)
							{ 
								sprintf(strtemp,"%*d ",*len_member->value.short_value,*EditWdgt->data.short_value);
							} else {
								sprintf(strtemp,"%*d-",*len_member->value.short_value,(*EditWdgt->data.short_value)*(-1));
							}
							strtemp[*len_member->value.short_value]=0;
							sprintf(&hold[lcount],"%s ",strtemp);
							break;
						case LONGV:
							memset(strtemp,0,1024);
							sprintf(strtemp,"%*d",*len_member->value.short_value,*EditWdgt->data.integer_value);
							strtemp[*len_member->value.short_value]=0;
							sprintf(&hold[lcount],"%s ",strtemp);
							break;
						case SLONGV:
							memset(strtemp,0,1024);
							if(*EditWdgt->data.integer_value>=0)
							{
								sprintf(strtemp,"%*d ",*len_member->value.short_value,*EditWdgt->data.integer_value);
							} else {
								sprintf(strtemp,"%*d-",*len_member->value.short_value,(*EditWdgt->data.integer_value)*(-1));
							}
							strtemp[*len_member->value.short_value]=0;
							sprintf(&hold[lcount],"%s ",strtemp);
							break;
						case DOLLARS_NOCENTS:
							break;
						case DOLLARS:
							temp=famt(*EditWdgt->data.float_value,*len_member->value.short_value);
							sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,temp);
							if(temp!=NULL) Rfree(temp);
							break;
						case DOUBLEV:
							memset(strtemp,0,1024);
							sprintf(strtemp,"%*.0f",*len_member->value.short_value,*EditWdgt->data.float_value);
							strtemp[*len_member->value.short_value]=0;
							sprintf(&hold[lcount],"%s ",strtemp);
							break;
						case SDOUBLEV:
							memset(strtemp,0,1024);
							if(*EditWdgt->data.float_value>=0)
							{ 
								sprintf(strtemp,"%*.0f ",*len_member->value.short_value,*EditWdgt->data.float_value);
							} else {
								sprintf(strtemp,"%*.0f-",*len_member->value.short_value,(*EditWdgt->data.float_value)*(-1));
							}
							strtemp[*len_member->value.short_value]=0;
							sprintf(&hold[lcount],"%s ",strtemp);
							break;
						case BOOLNS:
							sprintf(&hold[lcount],"%c ",(*EditWdgt->data.string_value  ? 'Y':'N'));
							break;
						case CHARACTERS:
							sprintf(&hold[lcount],"%c ",*EditWdgt->data.string_value);
							break;
						case BUTTONS:
						case SCROLLEDLIST:
							prterr("Huh?....");
							break;
						default:
							prterr("Error Invalid Field Type [%d].",EditWdgt->type);
							break;
					}
					lcount+=(*len_member->value.short_value+1);
				} else {
					prterr("Error AddEditWdgt [%s] not found.",s);
				}
				if(s!=NULL) Rfree(s);
			}
			}
		}
		++cur;
	}
	addAPPlib(POWERADD->PREVIOUSADD,hold);
	if(hold!=NULL) Rfree(hold);
	}
}
static void savelistcb(RDArsrc *define_list,RDArsrc *add_rsrc)
{
	if(IsScreenDisplayed(define_list)) NULL_RSCS(define_list);
}
void RUNPOWERADD(char *module,char *poweradd,int level,
	void (*quitfunc)(...),RDArsrc *fromrsrc,void *fromarg,
	short APPmainLoop,int line,char *file)
{
	char *warnmessage=NULL;
	RDArsrc *add_rsrc=NULL;
	AddEditWdgt *EditWdgt=NULL;
	char *temp=NULL;
	int x=0;
	short count=0,flen=0;
	PowerAdd *POWERADD=NULL;
	ADDPassableStruct *PSTRUCT=NULL;

	if(diagadd)
	{
		prterr("DIAG RUNPOWERADD(%s,%s) called at line [%d] program [%s].",module,poweradd,line,file);
	}
	POWERADD=PowerAddNew(module,poweradd);
	POWERADD->APPmainLoop=APPmainLoop;
	PSTRUCT=Rmalloc(sizeof(ADDPassableStruct));
	PSTRUCT->r=add_rsrc;
	PSTRUCT->quitfunc=quitfunc;
	PSTRUCT->fromrsrc=fromrsrc;
	PSTRUCT->fromarg=fromarg;

	if(findPowerAdd(POWERADD))
	{
		warnmessage=Rmalloc(300+RDAstrlen(module)+RDAstrlen(poweradd));
		sprintf(warnmessage,"Error Power Add Definition not found, this process cannot be executed without it's definition [%s] in the [%s.ADD] library.",poweradd,module);
		ERRORDIALOG("MAINTAIN POWERADD NOT FOUND.",warnmessage,NULL,(level==0?TRUE:FALSE)); 
		prterr("%s",warnmessage);
		if(warnmessage!=NULL) Rfree(warnmessage);
		doexit(POWERADD);
		return;
	} else {
		if(!OPEN_POWERADD_FILES(POWERADD))
		{
			doexit(POWERADD);
			prterr("Error cannot open files for this power add..");
			return;
		}
		Set_Skip_Widgets(POWERADD);

		AllocateEditWdgts(POWERADD);
		PP_translate_POWERADD(POWERADD);

		add_rsrc=RDArsrcNEW(POWERADD->module,POWERADD->addscreen);
		POWERADD->ADD_DEFINE_LIST=RDArsrcNEW(POWERADD->module,POWERADD->define_list);
		PSTRUCT->r=add_rsrc;
		add_rsrc->SubFunc=POWERADDSubData;
		add_rsrc->SubFuncArgs=POWERADD;

		POWERADD->SubFunc=POWERADDSubData;
		POWERADD->SubFuncArgs=POWERADD;
		POWERADD->addrsrc=add_rsrc;
		POWERADD->passable=(void *)PSTRUCT;
		POWERADD->level=level;

		POWERADD->PREVIOUSADD=APPlibNEW();
		addAPPlib(POWERADD->PREVIOUSADD,"Nothing Added this Session.");
		addlstrsrc(add_rsrc,"PREVIOUS ADD'S",&x,TRUE,NULL,POWERADD->PREVIOUSADD->numlibs,
			&POWERADD->PREVIOUSADD->libs,NULL);
		GET_POWERADD_INCVARS(add_rsrc,POWERADD);
		if(POWERADD->EditWdgts!=NULL)
		{
			for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;
				++x,++EditWdgt)
			{
				if(!isEMPTY(EditWdgt->name))
				{
					++count;
					temp=Rmalloc(RDAstrlen(EditWdgt->name)+10);
					sprintf(temp,"%s POSITION",EditWdgt->name);
					addstdrsrc(POWERADD->ADD_DEFINE_LIST,temp,SHORTV,3,&count,TRUE);
					sprintf(temp,"%s LENGTH",EditWdgt->name);
					flen=EditWdgt->len;
					addstdrsrc(POWERADD->ADD_DEFINE_LIST,temp,SHORTV,3,&flen,TRUE);
					if(temp!=NULL) Rfree(temp);
					makefieldrsrc(add_rsrc,EditWdgt->name,EditWdgt->type,
						EditWdgt->len,TRUE);
					if(EditWdgt->id_mgt==TRUE)
					{
						FINDRSCSETFUNC(add_rsrc,EditWdgt->name,SETNEWIDANDFILES,EditWdgt);
					} else {
						FINDRSCSETFUNC(add_rsrc,EditWdgt->name,SETFILES,POWERADD);
					}
				}
			}
		}
		addbtnrsrc(POWERADD->ADD_DEFINE_LIST,"DEFAULTS",TRUE,SaveDefaultsUsedOnly,NULL);
		addrfkwrsrc(POWERADD->ADD_DEFINE_LIST,"SELECT",TRUE,savelistcb,add_rsrc);
		addrfkwrsrc(POWERADD->ADD_DEFINE_LIST,"QUIT",TRUE,NULL_RSCS,NULL);
		addbtnrsrc(POWERADD->ADD_DEFINE_LIST,"HELP",TRUE,screenhelp,NULL);
		addbtnrsrc(POWERADD->ADD_DEFINE_LIST,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		GET_POWERADD_SCREEN_VIRTUALS(add_rsrc,0,FALSE,TRUE,POWERADD);
		SET_POWERADD_FILE2RSRCS(add_rsrc,POWERADD);

		ADVSCRNvirtual2rsrc(add_rsrc,POWERADDSubData,(void *)POWERADD);

		PSTRUCT->r=add_rsrc;
		POWERADD->passable=(void *)PSTRUCT;

		DefaultScreens(add_rsrc);
		GetAddEditWdgts(add_rsrc,POWERADD);
		ReadRDAScrolledLists(add_rsrc);
		READEditWdgts(POWERADD);
		SetAddEditWdgts(add_rsrc,POWERADD);
		DefaultScreens(POWERADD->ADD_DEFINE_LIST);
		GET_POWERADD_SUPPORTING(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD,FALSE,FALSE);
		DefaultAddEditWdgts(add_rsrc,POWERADD,0,POWERADDSubData,(void *)POWERADD);
		SetAddEditWdgts(add_rsrc,POWERADD);
		addbtnrsrc(add_rsrc,"SAVE",TRUE,save_record_now,POWERADD);
		addbtnrsrc(add_rsrc,"DEFAULTS",TRUE,SaveDefaults,POWERADD);
		addbtnrsrc(add_rsrc,"RESET DEFAULTS",TRUE,resetdefs,POWERADD);
		addbtnrsrc(add_rsrc,"DEFINE LIST",TRUE,add_definelist,POWERADD);
		addrfcbrsrc(add_rsrc,"QUIT",TRUE,quit_test,POWERADD);
		addrfcbrsrc(add_rsrc,"HELP",TRUE,screenhelp,POWERADD);
		addrfcbrsrc(add_rsrc,"PRINT RESOURCES",TRUE,printrsrcs,POWERADD);
		MakePowerAddButtonsADV(add_rsrc,POWERADD,updateallPowerAdd);
		POWERADDcomputeallSCRNvirtuals(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD);
		add_rsrc->SubFunc=POWERADDSubData;
		add_rsrc->SubFuncArgs=POWERADD;
		PSTRUCT->r=add_rsrc;
		POWERADD->passable=(void *)PSTRUCT;
		SetAddEditWdgts(add_rsrc,POWERADD);
		xmakescrn(add_rsrc,TRUE,DOADD_GUIEVAL,(void *)POWERADD,addoswidgets,DOADD_GUIEVALSTR,(void *)POWERADD,POWERADDSubData,POWERADD,NULL,__LINE__,__FILE__);
		SetAddEditWdgts(add_rsrc,POWERADD);
		if(!APPmainLoop)
		{
			APPmainLoop=TRUE;
			POWERADD->APPmainLoop=TRUE;
		}
	}
}
static void resetdefs(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	AddEditWdgt *EditWdgt=NULL;
	int x=0;
	char *name=NULL;

	if(POWERADD!=NULL)
	{
	if(POWERADD->EditWdgts!=NULL)
	{
		for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++EditWdgt)
		{
			if(EditWdgt!=NULL)
			{
				if(EditWdgt->id_mgt==TRUE)
				{
					UndoASSIGNID(POWERADD,EditWdgt,POWERADDSubData,POWERADD);
				}
			}
		}
	}
	}
	EditWdgt=NULL;
	ADVDefaultResetScreen(add_rsrc,TRUE,&name);
	if(POWERADD!=NULL)
	{
		for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;
			++x,++EditWdgt)
		{
			if(EditWdgt->etype==(-1))
			{
				switch(EditWdgt->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case ZIPCODE:
					case SOCSECNUM:
					case DATES:
					case TIMES:
					case EXPENDITURE:
					case BALANCESHEET:
					case REVENUE:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						FINDRSCSETSTRING(add_rsrc,EditWdgt->name,EditWdgt->data.string_value);
						break;
					case DECIMALV:
					case SDECIMALV:
					case DOLLARS_NOCENTS:
					case DOLLARS:
					case DOUBLEV:
					case SDOUBLEV:
						FINDRSCSETDOUBLE(add_rsrc,EditWdgt->name,*EditWdgt->data.float_value);
						break;
					case SHORTV:
					case SSHORTV:
						FINDRSCSETSHORT(add_rsrc,EditWdgt->name,*EditWdgt->data.short_value);
						break;
					case LONGV:
					case SLONGV:
					case SCROLLEDLIST:
						FINDRSCSETINT(add_rsrc,EditWdgt->name,*EditWdgt->data.integer_value);
						break;
					case BOOLNS:
					case CHARACTERS:
						FINDRSCSETCHAR(add_rsrc,EditWdgt->name,*EditWdgt->data.string_value);
						break;
					case BUTTONS:
						prterr("Huh?....");
						break;
					default:
						prterr("Error Invalid Field Type [%d].",EditWdgt->type);
						break;
				}
			}
		}
	}
	updateallrsrc(add_rsrc);
	GetAddEditWdgts(add_rsrc,POWERADD);
	DefaultAddEditWdgts(add_rsrc,POWERADD,1,POWERADDSubData,(void *)POWERADD);
	SetAddEditWdgts(add_rsrc,POWERADD);
	updateallrsrc(add_rsrc);
	SET_POWERADD(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD);
	updateallrsrc(add_rsrc);
	if(!isEMPTY(name)) FINDRSCSETINPUTFOCUS(add_rsrc,name);
	if(name!=NULL) Rfree(name);
}
static void SETNEWIDANDFILES(RDArsrc *add_rsrc,AddEditWdgt *EditWdgt)
{	
	char *value=NULL;
	PowerAdd *POWERADD=NULL;

	if(EditWdgt!=NULL)
	{
	POWERADD=(PowerAdd *)EditWdgt->parent;
	if(POWERADD!=NULL)
	{
	readwidget(add_rsrc,EditWdgt->name);
	switch(EditWdgt->type)
	{
		case SCROLLEDTEXT:
		case VARIABLETEXT:
		case PLAINTEXT:
		case PHONE:
		case CUSTOMTYPE:
		case ZIPCODE:
		case SOCSECNUM:
		case DATES:
		case TIMES:
		case EXPENDITURE:
		case BALANCESHEET:
		case REVENUE:
		case BEGINNINGBALANCE:
		case OPTIONALFIELDS:
			FINDRSCGETSTRING(add_rsrc,EditWdgt->name,&value);
			break;
		case DECIMALV:
		case SDECIMALV:
		case DOLLARS_NOCENTS:
		case DOLLARS:
		case DOUBLEV:
		case SDOUBLEV:
		case SHORTV:
		case SSHORTV:
		case LONGV:
		case SLONGV:
		case SCROLLEDLIST:
		case BOOLNS:
		case CHARACTERS:
		case BUTTONS:
		default:
			prterr("Error: SETNEWIDANDFILES(): Managed Edit Widget must be a String Type Field, name [%s]",
				(EditWdgt->name!=NULL?EditWdgt->name:""));
			break;
	}
	SET_POWERADD_MGT(add_rsrc,POWERADD,value,POWERADDSubData,(void *)POWERADD);
	if(value!=NULL) Rfree(value);
	}
	}
}
static void SETFILES(RDArsrc *add_rsrc,PowerAdd *POWERADD) 
{
	SET_POWERADD(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD);
}
static void readscreen(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	readallwidgets(add_rsrc);
	ReadRDAScrolledLists(add_rsrc);
	GetAddEditWdgts(add_rsrc,POWERADD);
}
void updateallPowerAdd(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	if(POWERADD!=NULL)
	{
		if(add_rsrc!=NULL)
		{
			readallwidgets(add_rsrc);
       			GetAddEditWdgts(add_rsrc,POWERADD);
			GET_POWERADD_SUPPORTING(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD,TRUE,FALSE);
			DefaultAddEditWdgts(add_rsrc,POWERADD,2,POWERADDSubData,(void *)POWERADD);
			SetAddEditWdgts(add_rsrc,POWERADD);
			xcomputeandupdateallSCRNvirtuals(add_rsrc,POWERADDSubData,(void *)POWERADD,FALSE,FALSE,NULL,POWERADD,__LINE__,__FILE__);
			updateallrsrc(add_rsrc);
			SetAllEditableAndSensitive(add_rsrc);
		}
	}
}
static void run_prequit_buttons(RDArsrc *add_rsrc,PowerAdd *POWERADD,short update)
{
	int x;
	MaintainButton *button=NULL;
	char do_btn=FALSE;

	if(POWERADD!=NULL)
	{
		for(x=0,button=POWERADD->buttons;x<POWERADD->numbuttons;++x,++button)
		{
			if(button->type==3)
			{
				do_btn=FALSE;
				if(!isEMPTY(button->expression))
				{
					do_btn=PP_EVALUATEbol(button->expression,POWERADDSubData,(void *)POWERADD);
	/*
					if(do_btn==FALSE) return;
	*/
				} else do_btn=TRUE;
				if(do_btn==TRUE)
				{
					button->func=updateallPowerAdd;
					PowerAddButtonFunction(add_rsrc,button);
				}
			}
		}
	}
}
static void run_presave_buttons(RDArsrc *add_rsrc,PowerAdd *POWERADD,short update)
{
	int x;
	MaintainButton *button=NULL;
	char do_btn=FALSE;

	if(POWERADD!=NULL)
	{
		for(x=0,button=POWERADD->buttons;x<POWERADD->numbuttons;++x,++button)
		{
			if(button->type==1)
			{
				do_btn=FALSE;
				if(!isEMPTY(button->expression))
				{
					do_btn=PP_EVALUATEbol(button->expression,POWERADDSubData,(void *)POWERADD);
				}  else do_btn=TRUE;
				if(do_btn==TRUE)
				{
					button->func=updateallPowerAdd;
					PowerAddButtonFunction(add_rsrc,button);
				}
			}
		}
	}
}
static void run_postsave_buttons(RDArsrc *add_rsrc,PowerAdd *POWERADD,short update)
{
	int x;
	MaintainButton *button=NULL;
	char do_btn=FALSE;

	if(POWERADD!=NULL)
	{
		for(x=0,button=POWERADD->buttons;x<POWERADD->numbuttons;++x,++button)
		{
			do_btn=FALSE;
			if(button->type==2)
			{
				if(!isEMPTY(button->expression))
				{
					do_btn=PP_EVALUATEbol(button->expression,POWERADDSubData,(void *)POWERADD);
				} else do_btn=TRUE;
				if(do_btn==TRUE)
				{
					button->func=updateallPowerAdd;
					PowerAddButtonFunction(add_rsrc,button);
				}
			}
		}
	}
}
static void save_record_warn(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	if(POWERADD!=NULL)
	{
		readscreen(add_rsrc,POWERADD);
		run_presave_buttons(add_rsrc,POWERADD,TRUE);
		save_record(add_rsrc,POWERADD,TRUE);
		run_postsave_buttons(add_rsrc,POWERADD,TRUE);
	}
}
static void save_record_now(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	readscreen(add_rsrc,POWERADD);
	save_check(add_rsrc,POWERADD,TRUE);
}
static void save_check(RDArsrc *add_rsrc,PowerAdd *POWERADD,short update)
{
	char *desc=NULL;

	if(POWERADD!=NULL)
	{
		if(!SAVE_POWERADD_CHECK(add_rsrc,POWERADD,POWERADDSubData,(void *)POWERADD))
		{
			return;
		}
		if(!isEMPTY(POWERADD->save_expression))
		{
			if(PP_EVALUATEbol(POWERADD->save_expression,POWERADDSubData,(void *)POWERADD))
			{
				desc=PP_EVALUATEstr(POWERADD->save_error_desc,POWERADDSubData,POWERADD);
				ERRORDIALOG("SAVE ERROR",desc,NULL,FALSE);
				if(desc!=NULL) Rfree(desc);
				return;
			}
		}
		if(!isEMPTY(POWERADD->save_warning))
		{
			if(PP_EVALUATEbol(POWERADD->save_warning,POWERADDSubData,(void *)POWERADD))
			{
				desc=PP_EVALUATEstr(POWERADD->save_warning_desc,
					POWERADDSubData,(void *)POWERADD);
				ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","SAVE WARNING",
					desc,save_record_warn,NULL,FALSE,2,add_rsrc,(void *)POWERADD,NULL);
				if(desc!=NULL) Rfree(desc);
				return;
			}
		}
		run_presave_buttons(add_rsrc,POWERADD,update);
		save_record(add_rsrc,POWERADD,update);
		run_postsave_buttons(add_rsrc,POWERADD,update);
		if(POWERADD->save_type==2 || POWERADD->save_type==4)
		{
			resetdefs(add_rsrc,POWERADD);
		} else if(POWERADD->save_type==3 || POWERADD->save_type==5)
		{
			quit_test(add_rsrc,POWERADD);
		}
	}
}
static void save_record(RDArsrc *add_rsrc,PowerAdd *POWERADD,short update_list)
{
	AddWrite *Write=NULL,*pWrite=NULL;
	AddWriteField *WriteField=NULL;
	NRDfield *field=NULL;
	RDAaddsub *subord=NULL;
	int a,x,b;
	char dowrite=FALSE,*trnfld=NULL,*fieldname=NULL;
	RDATData *prev=NULL;
	RDArsrc *tmprsrc=NULL,*holdrsrc=NULL;
	void *holdarg=NULL;
	AddEditWdgt *AE;
	RDArmem *mem=NULL;
	ADDPassableStruct *PSTRUCT=NULL;
/*
	ADDPassableStruct *PX=NULL;
*/

	if(POWERADD!=NULL)
	{
		PSTRUCT=(ADDPassableStruct *)POWERADD->passable;
	}
	holdrsrc=PSTRUCT->r;
	holdarg=PSTRUCT->arg;
	if(POWERADD->Writes!=NULL)
	{
		if(POWERADD->save_type==0 || POWERADD->save_type>3)
		{
			tmprsrc=RDArsrcNEW(POWERADD->module,NULL);
/*
			PX=Rmalloc(sizeof(ADDPassableStruct));
*/
/* This code segment is causing performance problems when a large number of 
   EZ VIRTUAL FIELDS are defind in the module, e.g. FINMGT 400+ in ADD RECEIPTS 
*/
			GET_POWERADD_INCVARS(tmprsrc,POWERADD);
			GET_POWERADD_SCREEN_VIRTUALS(tmprsrc,0,TRUE,FALSE,POWERADD);
/* END This code segment ... */
/*
			GET_POWERADD_SCREEN_VIRTUALS(tmprsrc,0,TRUE,TRUE,POWERADD);
			GET_POWERADD_SCREEN_VIRTUALS(tmprsrc,0,FALSE,TRUE,POWERADD);
*/
		} else {
			tmprsrc=add_rsrc;
		}
		holdrsrc=PSTRUCT->r;
		holdarg=PSTRUCT->arg;
		PSTRUCT->r=tmprsrc;
		PSTRUCT->arg=POWERADD;
		for(a=0,Write=POWERADD->Writes;a<POWERADD->numWrites;
			++a,++Write)
		{
			if(isEMPTY(Write->expression)) dowrite=TRUE;
				else dowrite=PP_EVALUATEbol(Write->expression,POWERADDSubData,(void *)POWERADD);
			if(dowrite==TRUE)
			{
			if(POWERADD->save_type==0 || POWERADD->save_type>3)
			{
			Write->closefile=FALSE;
			if(POWERADD->subord!=NULL)
			{
				for(x=0,subord=POWERADD->subord;x<POWERADD->subnum;++x,++subord)
				{
					if(!RDAstrcmp(subord->module,Write->module) && !RDAstrcmp(subord->filename,Write->filename)) break;
				}
				if(x<POWERADD->subnum)
				{
					Write->fileno=subord->fileno;
					Write->closefile=FALSE;
					Write->zerofile=FALSE;
				}
			}
			if(Write->fileno==(-1))
			{
				for(b=0,pWrite=POWERADD->Writes;b<a;++b,++pWrite)
				{
					if(!RDAstrcmp(pWrite->module,Write->module) && !RDAstrcmp(pWrite->filename,Write->filename)) break;
				}
				if(a<b)
				{
					Write->fileno=pWrite->fileno;
					Write->closefile=FALSE;
					Write->zerofile=TRUE;
				}
			}
			if(Write->fileno==(-1))
			{
				Write->fileno=OPNNRDsec(Write->module,Write->filename,TRUE,TRUE,FALSE);
				addDFincvir(tmprsrc,Write->module,Write->filename,NULL,Write->fileno);
				Write->closefile=TRUE;
				Write->zerofile=TRUE;
			}
			}
			if(Write->fileno!=(-1))
			{
				if(Write->zerofile==TRUE) ZERNRD(Write->fileno);
				if(Write->fields!=NULL)
				{
					for(x=0,WriteField=Write->fields;x<Write->numflds;++x,++WriteField)
					{
						fieldname=stripfieldname(WriteField->to_name);
						if(!isEMPTY(fieldname))
						{
							field=FLDNRD(Write->fileno,fieldname);
						}
						if(!WriteField->copy_type)
						{
							if(!RDAstrcmp(WriteField->from_name,"[NEXT TRANSACTION NO]"))
							{
								trnfld=stralloc(fieldname);
								if(fieldname!=NULL) Rfree(fieldname);
							} else {
								if(fieldname!=NULL) Rfree(fieldname);
								if(field!=NULL)
								{
					
									switch(field->type)
									{
										case SCROLLEDTEXT:
										case VARIABLETEXT:
										case DATES:
										case TIMES:
										case SOCSECNUM:
										case PLAINTEXT:
										case PHONE:
										case CUSTOMTYPE:
										case ZIPCODE:
										case EXPENDITURE:
										case REVENUE:
										case BALANCESHEET:
										case BEGINNINGBALANCE:
										case CHARACTERS:
										case OPTIONALFIELDS:
											if(field->data.string_value!=NULL) Rfree(field->data.string_value);
											field->data.string_value=PP_EVALUATEstr(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											if(field->data.string_value!=NULL)
											{
												field->dlen=RDAstrlen(field->data.string_value)+1;
												if(RDAstrlen(field->data.string_value)>field->len && field->len>0) field->data.string_value[field->len]=0;
											} else field->dlen=0;
											break;
										case BOOLNS:
											*field->data.string_value=PP_EVALUATEbol(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											break;
										case SHORTV:
										case SSHORTV:
											*field->data.short_value=PP_EVALUATEsht(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											break;
										case SLONGV:
										case SCROLLEDLIST:
										case LONGV:
											*field->data.integer_value=PP_EVALUATEint(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											break;
										case DOUBLEV:
										case SDOUBLEV:
										case DOLLARS:
										case DOLLARS_NOCENTS:
										case DECIMALV:
										case SDECIMALV:
											*field->data.float_value=PP_EVALUATEdbl(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											if(field->type==DOLLARS || field->type==DOLLARS_NOCENTS ||field->type==DOUBLEV || field->type==SDOUBLEV)
											{
												*field->data.float_value=round(*field->data.float_value);
											}
											break;
										default:
											prterr("Error unrecognized field type [%d] for field [%s].",field->type,(field->name!=NULL ? field->name:""));
									}
								}
							}
						} else {
							if(fieldname!=NULL) Rfree(fieldname);
							if(field!=NULL)
							{
								switch(field->type)
								{
									case SCROLLEDTEXT:
									case VARIABLETEXT:
									case DATES:
									case TIMES:
									case SOCSECNUM:
									case PLAINTEXT:
									case PHONE:
									case CUSTOMTYPE:
									case ZIPCODE:
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case CHARACTERS:
									case OPTIONALFIELDS:
										if(field->data.string_value!=NULL) Rfree(field->data.string_value);
										field->data.string_value=PP_EVALUATEstr(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
										if(field->data.string_value!=NULL)
										{
											field->dlen=RDAstrlen(field->data.string_value)+1;
											if(RDAstrlen(field->data.string_value)>field->len && field->len>0) field->data.string_value[field->len]=0;
										} else field->dlen=0;
										break;
									case BOOLNS:
										*field->data.string_value=PP_EVALUATEbol(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
										break;
									case SHORTV:
									case SSHORTV:
										*field->data.short_value=PP_EVALUATEsht(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
										break;
									case SLONGV:
									case SCROLLEDLIST:
									case LONGV:
										*field->data.integer_value=PP_EVALUATEint(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
										break;
									case DOUBLEV:
									case SDOUBLEV:
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case SDECIMALV:
										*field->data.float_value=PP_EVALUATEdbl(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
										if(field->type==DOLLARS || field->type==DOLLARS_NOCENTS ||field->type==DOUBLEV || field->type==SDOUBLEV)
										{
											*field->data.float_value=round(*field->data.float_value);
										}
										break;
									default:
										prterr("Error unrecognized field type [%d] for field [%s].",field->type,(field->name!=NULL ? field->name:""));
								}
							}
						}
					}
					LOCNRDFILE(Write->fileno);
					if(trnfld!=NULL)
					{
						ADVWRTTRANSsec(Write->fileno,1,trnfld,NULL,POWERADDSubData,(void *)POWERADD);	
						Rfree(trnfld);
						trnfld=NULL;
					} else if(ADVEQLNRDsec(Write->fileno,1,POWERADDSubData,(void *)POWERADD))
					{
						ADVWRTTRANSsec(Write->fileno,0,NULL,NULL,POWERADDSubData,(void *)POWERADD);	
					} else {
						prev=RDATDataNEW(Write->fileno);
						
						for(x=0,WriteField=Write->fields;x<Write->numflds;++x,++WriteField)
						{
							fieldname=stripfieldname(WriteField->to_name);
							field=FLDNRD(Write->fileno,fieldname);
							if(!WriteField->copy_type)
							{
								if(!RDAstrcmp(WriteField->from_name,"[NEXT TRANSACTION NO]"))
								{
									trnfld=stralloc(fieldname);
									if(fieldname!=NULL) Rfree(fieldname);
								} else {
									if(fieldname!=NULL) Rfree(fieldname);
									if(field!=NULL)
									{
						
										switch(field->type)
										{
											case SCROLLEDTEXT:
											case VARIABLETEXT:
											case DATES:
											case TIMES:
											case SOCSECNUM:
											case PLAINTEXT:
											case PHONE:
											case CUSTOMTYPE:
											case ZIPCODE:
											case EXPENDITURE:
											case REVENUE:
											case BALANCESHEET:
											case BEGINNINGBALANCE:
											case CHARACTERS:
											case OPTIONALFIELDS:
												if(field->data.string_value!=NULL) Rfree(field->data.string_value);
												field->data.string_value=PP_EVALUATEstr(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
												if(field->data.string_value!=NULL)
												{
													field->dlen=RDAstrlen(field->data.string_value)+1;
													if(RDAstrlen(field->data.string_value)>field->len && field->len>0) field->data.string_value[field->len]=0;
												} else field->dlen=0;
												break;
											case BOOLNS:
												*field->data.string_value=PP_EVALUATEbol(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
												break;
											case SHORTV:
											case SSHORTV:
												*field->data.short_value=PP_EVALUATEsht(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
												break;
											case SLONGV:
											case SCROLLEDLIST:
											case LONGV:
												*field->data.integer_value=PP_EVALUATEint(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
												break;
											case DOUBLEV:
											case SDOUBLEV:
											case DOLLARS:
											case DOLLARS_NOCENTS:
											case DECIMALV:
											case SDECIMALV:
												*field->data.float_value=PP_EVALUATEdbl(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
												if(field->type==DOLLARS || field->type==DOLLARS_NOCENTS ||field->type==DOUBLEV || field->type==SDOUBLEV)
												{
													*field->data.float_value=round(*field->data.float_value);
												}
												break;
											default:
												prterr("Error unrecognized field type [%d] for field [%s].",field->type,(field->name!=NULL ? field->name:""));
										}
									} else {
									}
								}
							} else {		
								if(fieldname!=NULL) Rfree(fieldname);
								if(field!=NULL)
								{
									switch(field->type)
									{
										case SCROLLEDTEXT:
										case VARIABLETEXT:
										case DATES:
										case TIMES:
										case SOCSECNUM:
										case PLAINTEXT:
										case PHONE:
										case CUSTOMTYPE:
										case ZIPCODE:
										case EXPENDITURE:
										case REVENUE:
										case BALANCESHEET:
										case BEGINNINGBALANCE:
										case CHARACTERS:
										case OPTIONALFIELDS:
											if(field->data.string_value!=NULL) Rfree(field->data.string_value);
											field->data.string_value=PP_EVALUATEstr(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											if(field->data.string_value!=NULL)
											{
												field->dlen=RDAstrlen(field->data.string_value)+1;
												if(RDAstrlen(field->data.string_value)>field->len && field->len>0) field->data.string_value[field->len]=0;
											} else field->dlen=0;
											break;
										case BOOLNS:
											*field->data.string_value=PP_EVALUATEbol(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											break;
										case SHORTV:
										case SSHORTV:
											*field->data.short_value=PP_EVALUATEsht(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											break;
										case SLONGV:
										case SCROLLEDLIST:
										case LONGV:
											*field->data.integer_value=PP_EVALUATEint(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											break;
										case DOUBLEV:
										case SDOUBLEV:
										case DOLLARS:
										case DOLLARS_NOCENTS:
										case DECIMALV:
										case SDECIMALV:
											*field->data.float_value=PP_EVALUATEdbl(WriteField->from_name,POWERADDSubData,(void *)POWERADD);
											if(field->type==DOLLARS || field->type==DOLLARS_NOCENTS ||field->type==DOUBLEV || field->type==SDOUBLEV)
											{
												*field->data.float_value=round(*field->data.float_value);
											}
											break;
										default:
											prterr("Error unrecognized field type [%d] for field [%s].",field->type,(field->name!=NULL ? field->name:""));
									}
								} else {
								}
							}
						}
						ADVWRTTRANSsec(Write->fileno,0,NULL,prev,POWERADDSubData,(void *)POWERADD);	
						if(prev!=NULL) FreeRDATData(prev);
					}	
				} else {
					prterr("Error AddWriteField's are missing...");
				}
			} else {
				prterr("Error AddWrite File not found [%s][%s].",(Write->module!=NULL ? Write->module:""),(Write->filename!=NULL ? Write->filename:""));
			}	
			}
		}
	}
	DefaultAddEditWdgts(add_rsrc,POWERADD,4,POWERADDSubData,(void *)POWERADD);
	SetAddEditWdgts(add_rsrc,POWERADD);
	updateallrsrc(add_rsrc);
	if(POWERADD->EditWdgts!=NULL)
	{
		for(x=0,AE=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++AE)
		{
			if(AE->id_mgt==TRUE) 
			{
				REMOVENEWID(POWERADD,AE,POWERADDSubData,(void *)POWERADD);
			}
		}
		if(POWERADD->save_type==0 || POWERADD->save_type>3)
		{
			if(POWERADD->Writes!=NULL)
			{
				for(x=0,Write=POWERADD->Writes;x<POWERADD->numWrites;++x,
					++Write)
				{
					if(Write->fileno!=(-1) && Write->closefile==TRUE) 
						CLSNRD(Write->fileno);
				}
			}
		}
	}
/*
	if(PX!=NULL) Rfree(PX);
*/
	PSTRUCT->r=holdrsrc;
	PSTRUCT->arg=holdarg;
	if(POWERADD->save_type==0 || POWERADD->save_type>3)
	{
		if(tmprsrc!=NULL) free_rsrc(tmprsrc);
	}
	POWERADDADVupdateSCRNvirtuals(add_rsrc,POWERADDSubData,(void *)POWERADD,POWERADD);
	makedflist(POWERADD);
	FINDRSCLISTAPPlib(add_rsrc,"PREVIOUS ADD'S",(POWERADD->PREVIOUSADD->numlibs-1),
		POWERADD->PREVIOUSADD);
	updatersrc(add_rsrc,"PREVIOUS ADD'S");
	++POWERADD->WRITE_COUNTER;
	x=FINDRSC(add_rsrc,"SAVE");
	if(x!=(-1))
	{
		mem=add_rsrc->rscs+x;
		if(!isEMPTY(mem->transversal_expression))
		{
			ComputeNewTransversalADV(add_rsrc,add_rsrc->EvalStr,add_rsrc->EvalStrArgs,add_rsrc->SubFunc,add_rsrc->SubFuncArgs,mem->transversal_expression);
		}
	}
}
static void quit_record_warn(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	run_prequit_buttons(add_rsrc,POWERADD,FALSE);
	quit_record(add_rsrc,POWERADD);
}
static void quit_test(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	char *desc=NULL;
	ADDPassableStruct *PSTRUCT=NULL;

	if(POWERADD!=NULL)
	{
	PSTRUCT=(ADDPassableStruct *)POWERADD->passable;
	if(!isEMPTY(POWERADD->quit_error))
	{
		if(PP_EVALUATEbol(POWERADD->quit_error,POWERADDSubData,(void *)POWERADD))
		{
			desc=PP_EVALUATEstr(POWERADD->quit_error_desc,POWERADDSubData,(void *)POWERADD);
			ERRORDIALOG("QUIT ERROR",desc,NULL,FALSE);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	if(!isEMPTY(POWERADD->quit_warning))
	{
		if(PP_EVALUATEbol(POWERADD->quit_warning,POWERADDSubData,(void *)POWERADD))
		{
			desc=PP_EVALUATEstr(POWERADD->quit_warning_desc,POWERADDSubData,(void *)POWERADD);
			ADVWARNINGDIALOG("QUIT WARNING DIALOG SCREEN","QUIT WARNING",
				desc,quit_record_warn,NULL,FALSE,2,add_rsrc,POWERADD,NULL);
			if(desc!=NULL) Rfree(desc);
			return;
		}
	}
	run_prequit_buttons(add_rsrc,POWERADD,FALSE);
	if(POWERADD->level!=0)
	{
/*lint -e746 */
		if(PSTRUCT->quitfunc!=NULL) PSTRUCT->quitfunc(PSTRUCT->fromrsrc,PSTRUCT->fromarg);
/*lint +e746 */
	}
	quit_record(add_rsrc,POWERADD);
	}
}
static void quit_record(RDArsrc *add_rsrc,PowerAdd *POWERADD)
{
	APPlib *args=NULL;
	AddEditWdgt *EditWdgt=NULL;
	int x=0;
	int level=0;
	ADDPassableStruct *PSTRUCT=NULL;

	if(POWERADD!=NULL)
	{
		PSTRUCT=(ADDPassableStruct *)POWERADD->passable;
	}
	level=POWERADD->level;
	if(POWERADD!=NULL)
	{
	if(POWERADD->EditWdgts!=NULL)
	{
		for(x=0,EditWdgt=POWERADD->EditWdgts;x<POWERADD->numwdgts;++x,++EditWdgt)
		{
			if(EditWdgt->id_mgt==TRUE)
			{
					UndoASSIGNID(POWERADD,EditWdgt,POWERADDSubData,(void *)POWERADD);
			}
		}
	}
	}
	if(add_rsrc!=NULL)
	{
		killwindow(add_rsrc);
		free_rsrc(add_rsrc); 
	}
	if(PSTRUCT!=NULL) Rfree(PSTRUCT);
	if(POWERADD!=NULL)
	{
		if(POWERADD->PREVIOUSADD!=NULL) freeapplib(POWERADD->PREVIOUSADD);
		if(POWERADD->ADD_DEFINE_LIST!=NULL) free_rsrc(POWERADD->ADD_DEFINE_LIST);
		CLOSE_POWERADD_FILES(POWERADD);
		if((POWERADD->WRITE_COUNTER>0) && !isEMPTY(POWERADD->reportname))
		{
			args=APPlibNEW();
			addAPPlib(args,POWERADD->module);
			addAPPlib(args,POWERADD->reportname);
			ExecuteProgram("doreport",args);
			if(args!=NULL) freeapplib(args);
		}
		FreeEditWdgts(POWERADD);
		FreePowerAdd(POWERADD);
	}
	if(level==0)
	{
		ShutdownSubsystems();
		std::exit;
	}
}
void xGET_POWERADD_SCREEN_VIRTUALS(RDArsrc *r,short type,short force,
	short includelists,PowerAdd *POWERADD,int line,char *file)
{
	char *libx=NULL,*tmpx=NULL,addvf=FALSE,temp[501];
	char *tx=NULL;
	APPlib *filenames=NULL,*ScrolledLists=NULL;
	int x,y,z,g,vc=0,lc=0;
	DFvirtual *d=NULL;
	DFincvir *i,*a;
	RDAwdgt *wdgt;
	int count=0,got_it=0;
	int mi=(-1);
/*
	RDArmem *mem=NULL;
*/
	RDAvirtual *v,*v1;
	RDAScrolledList *list=NULL,*LIST=NULL;

	if(diagvirtual || diagvirtual_field)
	{
		prterr("DIAG GET_SCREEN_VIRTUALS Creating Virtual Fields used on Custom Screen [%s] [%s] at line [%d] program [%s].",r->module,r->screen,line,file);
	}
	if(!force)
	{
		if(r->scn==NULL && !isEMPTY(r->screen))
		{
			r->scn=RDAscrnNEW(r->module,r->screen);
			if(findscrnbin(r->scn)) 
			{
				prterr("Error couldn't read Screen Binary [%s] [%s] from Binary Library [%s.SCN]",r->module,(r->screen!=NULL ? r->screen:""),r->module);
				free_scrn(r->scn);
				r->scn=NULL;
				return;
			} 
		} else {
			r->scn=NULL;
		}
	}
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(r->module)+10);
	tmpx=adddashes(r->module);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,tmpx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,tmpx);
#endif
	filenames=getunsortedlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	if(tmpx!=NULL) Rfree(tmpx);
	tmpx=NULL;
	libx=NULL;
	if(includelists && (force || (!force && r->scn!=NULL)))
	{
		libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(r->module)+10);
		tmpx=adddashes(r->module);
#ifndef WIN32
		sprintf(libx,"%s/rda/%s.LST",CURRENTDIRECTORY,tmpx);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\rda\\%s.LST",CURRENTDIRECTORY,tmpx);
#endif
		ScrolledLists=getunsortedlibnames(libx,FALSE);
		if(ScrolledLists==NULL) ScrolledLists=APPlibNEW();
	} else {
		if(ScrolledLists==NULL) ScrolledLists=APPlibNEW();
	}
	if(filenames==NULL) filenames=APPlibNEW();
	if(!force && r->scn!=NULL)
	{
		if(r->scn->wdgts!=NULL)
		{
			for(g=0,wdgt=r->scn->wdgts;g<r->scn->numwdgts;++g,++wdgt)
			{
				if((!wdgt->type || wdgt->type==8 || wdgt->type==9)
					&& wdgt->resource_name!=NULL)
				{
/* GETTING VIRTUAL FIELDS USED ON SCREEN */
					for(y=0;y<filenames->numlibs;++y)
					{
						if(!type) /* regular screen */
						{
							if(!RDAstrcmp(wdgt->resource_name,
								filenames->libs[y])) break;
						} else if(type==1) /* Define List */
						{
							memset(temp,0,501);
							sprintf(temp,"%s LENGTH",filenames->libs[y]);
							if(!RDAstrcmp(wdgt->resource_name,temp)) break;
							memset(temp,0,501);
							sprintf(temp,"%s POSITION",filenames->libs[y]);
							if(!RDAstrcmp(wdgt->resource_name,temp)) break; 
						} else if(type==2) /* Range Screen */
						{
							if(wdgt->type==8)
							{
								memset(temp,0,501);
								sprintf(temp,"SELECT [%s] TRUE",
									filenames->libs[y]);
								if(!RDAstrcmp(wdgt->resource_name,temp)) break;
								memset(temp,0,501);
								sprintf(temp,"SELECT [%s] FALSE",
									filenames->libs[y]);
								if(!RDAstrcmp(wdgt->resource_name,temp)) break; 
							} else {
								memset(temp,0,501);
								sprintf(temp,"FROM [%s]",filenames->libs[y]);
								if(!RDAstrcmp(wdgt->resource_name,temp)) break;
								memset(temp,0,501);
								sprintf(temp,"TO [%s]",filenames->libs[y]);
								if(!RDAstrcmp(wdgt->resource_name,temp)) break; 
							}
						}
					}
					if(y<filenames->numlibs)
					{
						mi=FINDRSC(r,filenames->libs[y]);
						if(mi==(-1))
						{
						d=getDFvirtual(r->module,filenames->libs[y]);
						if(d!=NULL)
						{
							addvf=d->avail2rsrc;
							if(d->incvir!=NULL && d->avail2rsrc &&
								!d->vtype)
							{
								for(z=0,a=d->incvir;z<d->num;++z,++a)
								{
									for(x=0,i=r->incvir;x<r->numvir;
										++x,++i)
									{
										if(!RDAstrcmp(i->module,a->module)
											&& !RDAstrcmp(i->file,a->file)) 
											break;
									}
									if(x>=r->numvir) 
									{
										addvf=FALSE;
										break;
									}
								}
							} else if(d->incvir!=NULL && d->avail2rsrc &&
								d->vtype)
							{
								for(z=0,a=d->incvir;z<d->num;++z,++a)
								{
									for(x=0,i=r->incvir;x<r->numvir;
										++x,++i)
									{
										if(!RDAstrcmp(i->module,a->module)
											&& !RDAstrcmp(i->file,a->file)) 
											break;
									}
									if(x<r->numvir)
									{
										addvf=TRUE;
										break;
									}
								}
								if(z>=d->num) addvf=FALSE;
							}
							if(addvf)
							{
								addSCRNvirtual(r,d->name,d->type,d->length,
									d->expression,d->when);
								++count;
							}
							FreeDFvirtual(d);
							d=NULL;
						}
						}
					}
				} else if(((wdgt->type==7) && wdgt->resource_name!=NULL)
					&& !type && includelists)
				{
/* GETTING SCROLLED LISTS USED ON SCREEN */
					for(y=0;y<ScrolledLists->numlibs;++y)
					{
						if(!RDAstrcmp(wdgt->resource_name,
							ScrolledLists->libs[y])) break;
					}
					if(y<ScrolledLists->numlibs)
					{
						list=RDAScrolledListNew(r->module,
							wdgt->resource_name);
						if(getScrolledListbin(libx,list)!=(-1))
						{
							addvf=TRUE;
							if(list->incvir!=NULL && !list->vtype)
							{
								for(z=0,a=list->incvir;z<list->numincvirs;
									++z,++a)
								{
									for(x=0,i=r->incvir;x<r->numvir;
										++x,++i)
									{
										if(!RDAstrcmp(i->module,a->module)
											&& !RDAstrcmp(i->file,a->file)) 
											break;
									}
									if(x>=r->numvir) 
									{
										addvf=FALSE;
										break;
									}
								}
							} else if(list->incvir!=NULL && list->vtype)
							{
								for(z=0,a=list->incvir;z<list->numincvirs;++z,++a)
								{
									for(x=0,i=r->incvir;x<r->numvir;
										++x,++i)
									{
										if(!RDAstrcmp(i->module,a->module)
											&& !RDAstrcmp(i->file,a->file)) 
											break;
									}
									if(x<r->numvir)
									{
										addvf=TRUE;
										break;
									}
								}
								if(z>=list->numincvirs) addvf=FALSE;
							}
							if(addvf)
							{
								addSCRNScrolledList(r,list->module,
									list->name,list->vtype,
									list->numincvirs,list->incvir,
									list->type,list->select_formula,
									list->special_function,list->num,list->initfld,
									list->ffield,list->occurrence,list->ffinfo,list->contype,
									list->conmod,list->confil,
									list->confld,list->con_occurrence,list->list,
									list->format_formula,
									list->unformat_formula,
									list->reformat_formula,
									list->desc,list->timing);
								++count;
							}
						}
						if(list!=NULL) FreeRDAScrolledList(list);
					}
				}
			}
		}
	} else { /* FORCED */
		if(includelists)
		{
			if(r->scn==NULL && !isEMPTY(r->module) && !isEMPTY(r->screen))
			{
				r->scn=RDAscrnNEW(r->module,r->screen);
				if(findscrnbin(r->scn))
				{ 
					free_scrn(r->scn);
					r->scn=NULL;
				}
			} 
			if(r->scn!=NULL)
			{
			
/* GETTING ALL SCROLLED LISTS AVAILABLE */
			if(r->scn->wdgts!=NULL)
			{
				for(g=0,wdgt=r->scn->wdgts;g<r->scn->numwdgts;++g,++wdgt)
				{
					if(((wdgt->type==7) && wdgt->resource_name!=NULL)
						&& includelists)
					{
/* 	GETTING SCROLLED LISTS USED ON SCREEN */
						for(y=0;y<ScrolledLists->numlibs;++y)
						{
							if(!RDAstrcmp(wdgt->resource_name,
								ScrolledLists->libs[y])) break;
						}
						if(y<ScrolledLists->numlibs)
						{
							list=RDAScrolledListNew(r->module,
								wdgt->resource_name);
							if(getScrolledListbin(libx,list)!=(-1))
							{
								addvf=TRUE;
								if(list->incvir!=NULL && !list->vtype)
								{
									for(z=0,a=list->incvir;z<list->numincvirs;
										++z,++a)
									{
										for(x=0,i=r->incvir;x<r->numvir;
											++x,++i)
										{
											if(!RDAstrcmp(i->module,a->module)
												&& !RDAstrcmp(i->file,a->file)) 
												break;
										}
										if(x>=r->numvir) 
										{
											addvf=FALSE;
											break;
										}
									}
								} else if(list->incvir!=NULL && list->vtype)
								{
									for(z=0,a=list->incvir;z<list->numincvirs;++z,++a)
									{
										for(x=0,i=r->incvir;x<r->numvir;
											++x,++i)
										{
											if(!RDAstrcmp(i->module,a->module)
												&& !RDAstrcmp(i->file,a->file)) 
												break;
										}
										if(x<r->numvir)
										{
											addvf=TRUE;
											break;
										}
									}
									if(z>=list->numincvirs) addvf=FALSE;
								}
								if(addvf)
								{
									addSCRNScrolledList(r,list->module,
										list->name,list->vtype,
										list->numincvirs,list->incvir,
										list->type,list->select_formula,
										list->special_function,list->num,list->initfld,
										list->ffield,list->occurrence,list->ffinfo,list->contype,
										list->conmod,list->confil,
										list->confld,list->con_occurrence,list->list,
										list->format_formula,
										list->unformat_formula,
										list->reformat_formula,
										list->desc,list->timing);
									++count;
								}
							}
							if(list!=NULL) FreeRDAScrolledList(list);
						}
					}
				}
			}
			}
		}
/* GETTING ALL VIRTUAL FIELDS AVAILABLE */
		for(y=0;y<filenames->numlibs;++y)
		{
			mi=FINDRSC(r,filenames->libs[y]);
			if(mi==(-1))
			{
			d=getDFvirtual(r->module,filenames->libs[y]);
			if(d!=NULL)
			{
				addvf=d->avail2rsrc;
				if(d->incvir!=NULL && d->avail2rsrc && !d->vtype)
				{
					for(z=0,a=d->incvir;z<d->num;++z,++a)
					{
						for(x=0,i=r->incvir;x<r->numvir;++x,++i)
						{
							if(!RDAstrcmp(i->module,a->module) && 
								!RDAstrcmp(i->file,a->file)) break;
						}
						if(x>=r->numvir) 
						{
							addvf=FALSE;
							break;
						}
					}
				} else if(d->incvir!=NULL && d->avail2rsrc &&
					d->vtype)
				{
					for(z=0,a=d->incvir;z<d->num;++z,++a)
					{
						for(x=0,i=r->incvir;x<r->numvir;
							++x,++i)
						{
							if(!RDAstrcmp(i->module,a->module)
								&& !RDAstrcmp(i->file,a->file)) 
								break;
						}
						if(x<r->numvir)
						{
							addvf=TRUE;
							break;
						}
					}
					if(z>=d->num) addvf=FALSE;
				}
				if(addvf)
				{
					addSCRNvirtual(r,d->name,d->type,d->length,
						d->expression,d->when);
					++count;
				}
				FreeDFvirtual(d);
				d=NULL;
			}
			}
		}
	} 
/* GETTING VIRTUAL FIELDS USED IN OTHER VIRTUAL FIELDS */
	if(!force && count>0)
	{
	vc=0;
	while(TRUE)
	{
		if(vc>=r->num) break;
		for(x=0;x<filenames->numlibs;++x)
		{
			v=r->virtual+vc;
			if(v->expression!=NULL && RDAstrcmp(v->name,filenames->libs[x]))
			{
				got_it=FINDVIRTUAL(r,filenames->libs[x]);
				if(got_it==(-1))
				{
					tx=RDAstrstr(v->expression,filenames->libs[x]); 
					if(!isEMPTY(tx))
					{
						for(y=0,v1=r->virtual;y<r->num;++y,++v1)
						{
							if(!RDAstrcmp(v1->name,
								filenames->libs[x])) break;
						}	
						if(y>=r->num)
						{
							mi=FINDRSC(r,filenames->libs[x]);
							if(mi==(-1))
							{
							d=getDFvirtual(r->module,filenames->libs[x]);
							if(d!=NULL)
							{
								ADVaddSCRNvirtual(r,d->name,d->type,d->length,
									d->expression,d->when,FALSE);
								FreeDFvirtual(d);
								d=NULL;
							}
							}
						}
					}
				}
			}
		}
		if(includelists)
		{
			for(y=0;y<ScrolledLists->numlibs;++y)
			{
				v=r->virtual+vc;
				if(v->expression!=NULL)
				{
					tx=RDAstrstr(v->expression,ScrolledLists->libs[y]); 
					if(!isEMPTY(tx))
					{
						list=RDAScrolledListNew(r->module,
							ScrolledLists->libs[y]);
						if(getScrolledListbin(libx,list)!=(-1))
						{
							addSCRNScrolledList(r,list->module,
								list->name,list->vtype,list->numincvirs,list->incvir,
								list->type,list->select_formula,
								list->special_function,list->num,list->initfld,
								list->ffield,list->occurrence,list->ffinfo,
								list->contype,list->conmod,
								list->confil,list->confld,list->con_occurrence,list->list,
								list->format_formula,
								list->unformat_formula,
								list->reformat_formula,
								list->desc,list->timing);
						}
						if(list!=NULL) FreeRDAScrolledList(list);
					}
				}
			}
			++vc;
		}
	}
	if(includelists)
	{
		lc=0;
		while(TRUE)
		{
			if(lc>=r->numlists) break;
			for(x=0;x<filenames->numlibs;++x)
			{
				LIST=r->lists+lc;
				if(!isEMPTY(LIST->select_formula))
				{
					tx=RDAstrstr(LIST->select_formula,filenames->libs[x]); 
					if(!isEMPTY(tx))
					{
						for(y=0,v1=r->virtual;y<r->num;++y,++v1)
						{
							if(!RDAstrcmp(v1->name,
								filenames->libs[x])) break;
						}	
						if(y>=r->num)
						{
							mi=FINDRSC(r,filenames->libs[x]);
							if(mi==(-1))
							{
							d=getDFvirtual(r->module,filenames->libs[x]);
							if(d!=NULL)
							{
								ADVaddSCRNvirtual(r,d->name,d->type,d->length,
									d->expression,d->when,FALSE);
								FreeDFvirtual(d);
								d=NULL;
							}
							}
						}
					}
				}
				if(!isEMPTY(LIST->format_formula))
				{
					tx=RDAstrstr(LIST->format_formula,filenames->libs[x]); 
					if(!isEMPTY(tx))
					{
						for(y=0,v1=r->virtual;y<r->num;++y,++v1)
						{
							if(!RDAstrcmp(v1->name,
								filenames->libs[x])) break;
						}	
						if(y>=r->num)
						{
							mi=FINDRSC(r,filenames->libs[x]);
							if(mi==(-1))
							{
							d=getDFvirtual(r->module,filenames->libs[x]);
							if(d!=NULL)
							{
								ADVaddSCRNvirtual(r,d->name,d->type,d->length,
									d->expression,d->when,FALSE);
								FreeDFvirtual(d);
								d=NULL;
							}
							}
						}
					}
				}
			}
			for(y=0;y<ScrolledLists->numlibs;++y)
			{
				LIST=r->lists+lc;
				if(!isEMPTY(LIST->select_formula) && 
					RDAstrcmp(LIST->name,ScrolledLists->libs[y]))
				{
					tx=RDAstrstr(LIST->select_formula,
						ScrolledLists->libs[y]); 
					if(!isEMPTY(tx))
					{
						list=RDAScrolledListNew(r->module,
							ScrolledLists->libs[y]);
						if(getScrolledListbin(libx,list)!=(-1))
						{
							addSCRNScrolledList(r,list->module,
								list->name,list->vtype,list->numincvirs,list->incvir,
								list->type,list->select_formula,
								list->special_function,
								list->num,list->initfld,list->ffield,
								list->occurrence,list->ffinfo,
								list->contype,list->conmod,
								list->confil,list->confld,list->con_occurrence,
								list->list,list->format_formula,
								list->unformat_formula,
								list->reformat_formula,
								list->desc,list->timing);
						}
						if(list!=NULL) FreeRDAScrolledList(list);
					}
				}
/*
				if(!isEMPTY(LIST->format_formula) &&
					RDAstrcmp(LIST->name,ScrolledLists->libs[y]))
				{	
*/
				if(!isEMPTY(LIST->format_formula))
				{
				if(RDAstrcmp(LIST->name,ScrolledLists->libs[y]))
				{
					tx=RDAstrstr(LIST->select_formula,ScrolledLists->libs[y]); 
					if(!isEMPTY(tx))
					{
						list=RDAScrolledListNew(r->module,
							ScrolledLists->libs[y]);
						if(getScrolledListbin(libx,list)!=(-1))
						{
							addSCRNScrolledList(r,list->module,
								list->name,list->vtype,list->numincvirs,list->incvir,
								list->type,list->select_formula,
								list->special_function,
								list->num,list->initfld,list->ffield,
								list->occurrence,list->ffinfo,
								list->contype,list->conmod,
								list->confil,list->confld,list->con_occurrence,list->list,
								list->format_formula,
								list->unformat_formula,
								list->reformat_formula,
								list->desc,list->timing);
						}
						if(list!=NULL) FreeRDAScrolledList(list);
					}
				}
				}
			}
			++lc;
		}
	}		
	}
	freeapplib(filenames);
	filenames=NULL;
	if(includelists)
	{
		if(r->lists!=NULL)
		{
			for(x=0,list=r->lists;x<r->numlists;++x,++list)
			{
				if(list->incvir!=NULL)
				{
				for(y=0,i=list->incvir;y<list->numincvirs;++y,++i)
				{
					for(z=0,a=r->incvir;z<r->numvir;++z,++a)
					{
						if(!RDAstrcmp(i->module,a->module)
							&& !RDAstrcmp(i->file,a->file)) break;
					}
					if(z<r->numvir)
					{
						i->fileno=a->fileno;
					}
				}
				if(!list->type)
				{
					if(list->ffinfo!=NULL)
					{
						for(z=0,a=r->incvir;z<r->numvir;++z,++a)
						{
							if(!RDAstrcmp(list->ffinfo->module,a->module)
								&& !RDAstrcmp(list->ffinfo->file,a->file)) break;
						}
						if(z<r->numvir)
						{
							list->ffinfo->fileno=a->fileno;
						}
					}
				}
				}
			}
		}
		freeapplib(ScrolledLists);
		ScrolledLists=NULL;
		if(libx!=NULL) Rfree(libx);
		if(tmpx!=NULL) Rfree(tmpx);
		tmpx=NULL;
		libx=NULL;
	}
}
