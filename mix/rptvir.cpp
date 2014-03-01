#include<mix.hpp>

short xvirtual2rangersrc(RDAvirtual *field,RDArsrc *rsrc,int line,char *file)
{
	char *name=NULL;
	char svalue=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field)
	{
		prterr("DIAG virtual2rangersrc Create Resource in Screen [%s] [%s] using Virtual Field [%s] at line [%d] program [%s].",rsrc->module,rsrc->screen,field->name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(field!=NULL)
	{
		if(field->type!=BOOLNS)
		{
			name=Rmalloc(RDAstrlen(field->name)+12);
			sprintf(name,"RANGE ON [%s]",field->name);
			if(!alreadyrsrc(name,rsrc))
			{
				makefieldrsrc(rsrc,name,BOOLNS,1,TRUE);
				ADVFINDRSCSETFUNC(rsrc,name,setrangesensitivecb,NULL,TRUE);
			}
			Rfree(name);
			name=Rmalloc(RDAstrlen(field->name)+8);
			sprintf(name,"FROM [%s]",field->name);
			if(!alreadyrsrc(name,rsrc))
			{
				makefieldrsrc(rsrc,name,field->type,
					field->len,TRUE);
				ADVFINDRSCSETFUNC(rsrc,name,setrangeto2fromcb,NULL,TRUE);
			}
			Rfree(name);
			name=Rmalloc(RDAstrlen(field->name)+6);
			sprintf(name,"TO [%s]",field->name);
			if(!alreadyrsrc(name,rsrc))
			{
				makefieldrsrc(rsrc,name,field->type,
					field->len,TRUE);
			}
			Rfree(name);
		} else {
			name=Rmalloc(RDAstrlen(field->name)+15);
			sprintf(name,"SELECT [%s] TRUE",field->name);
			if(!alreadyrsrc(name,rsrc))
			{
				makefieldrsrc(rsrc,name,field->type,
			 		field->len,TRUE);
				svalue=TRUE;
				FINDRSCSETCHAR(rsrc,name,svalue);
			}
			Rfree(name);
			name=Rmalloc(RDAstrlen(field->name)+16);
			sprintf(name,"SELECT [%s] FALSE",field->name);
			if(!alreadyrsrc(name,rsrc))
			{
				makefieldrsrc(rsrc,name,field->type,
					field->len,TRUE);
				FINDRSCSETCHAR(rsrc,name,svalue);
			}	
			Rfree(name);
		}
	}
	return(0);
}
void ReadVirtualRangeStruct(RDAvirtual *field,RangeScreen *rs)
{
	int x;
	char *fieldname=NULL;
	FileRangeStruct *frs=NULL;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field)
	{
		prterr("DIAG ReadVirtualRangeStruct Reading Range Values from Screen [%s] [%s] for Virtual Field [%s].",rs->rsrc->module,rs->rsrc->screen,field->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(field!=NULL)
	{
		frs=NULL;
		if(rs->frs!=NULL)
		{
			rs->frs=Rrealloc(rs->frs,(rs->num+1)*
				sizeof(FileRangeStruct));
		} else {
			rs->frs=Rmalloc(sizeof(FileRangeStruct));
			rs->num=0;
		}
		frs=rs->frs+rs->num;
		frs->visible=FALSE;
		frs->filename=NULL;
		frs->fieldname=stralloc(field->name);
		frs->range=FALSE;
		++rs->num;
		frs->field_type=field->type;
		frs->field_len=field->len;
		if(field->type!=BOOLNS)
		{
			fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+12);
			sprintf(fieldname,"RANGE ON [%s]",field->name);
			x=FINDRSC(rs->rsrc,fieldname);
			if(x!=(-1)) 
			{
				member=rs->rsrc->rscs+x;
				frs->range=*member->value.string_value;
				if(member->w!=NULL) frs->visible=TRUE;
			} else frs->range=FALSE;
			if(fieldname!=NULL) Rfree(fieldname);
		}
		switch(field->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case PLAINTEXT:
			case SOCSECNUM:
			case PHONE:
			case CUSTOMTYPE:
			case ZIPCODE:
			case DATES:
			case TIMES:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				frs->from.string_value=NULL;
				frs->to.string_value=NULL;
				if(frs->range)
				{
					fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+8);
					sprintf(fieldname,"FROM [%s]",field->name);
					x=FINDRSC(rs->rsrc,fieldname);
					if(x!=(-1))
					{
						member=rs->rsrc->rscs+x;
						if(member->value.string_value!=NULL && RDAstrlen(member->value.string_value)>0)
						{
							frs->from.string_value=
								stralloc(member->value.string_value);
						}
						if(member->w!=NULL) frs->visible=TRUE;
					}
					fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+6);
					sprintf(fieldname,"TO [%s]",field->name);
					x=FINDRSC(rs->rsrc,fieldname);
					if(x!=(-1))
					{
						member=rs->rsrc->rscs+x;
						if(member->value.string_value!=NULL && RDAstrlen(member->value.string_value)>0)
						{
							frs->to.string_value=
								stralloc(member->value.string_value);
						}
						if(member->w!=NULL) frs->visible=TRUE;
					}
				}
				break;
			case LONGV:
			case SLONGV:
				frs->from.integer_value=Rmalloc(sizeof(int));
				*frs->from.integer_value=0;
				frs->to.integer_value=Rmalloc(sizeof(int));
				*frs->to.integer_value=0;
				if(frs->range)
				{
					fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+8);
					sprintf(fieldname,"FROM [%s]",field->name);
					x=FINDRSC(rs->rsrc,fieldname);
					if(x!=(-1))
					{
						member=rs->rsrc->rscs+x;
						*frs->from.integer_value=*member->value.integer_value;
						if(member->w!=NULL) frs->visible=TRUE;
					}
					fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+
						6);
					sprintf(fieldname,"TO [%s]",field->name);
					x=FINDRSC(rs->rsrc,fieldname);
					if(x!=(-1))
					{
						member=rs->rsrc->rscs+x;
						*frs->to.integer_value=
							*member->value.integer_value;
						if(member->w!=NULL) frs->visible=TRUE;
					}
				}
				break;
			case SHORTV:
			case SSHORTV:
				frs->from.short_value=Rmalloc(sizeof(short));
				*frs->from.short_value=0;
				frs->to.short_value=Rmalloc(sizeof(short));
				*frs->to.short_value=0;
				if(frs->range)
				{
					fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+8);
					sprintf(fieldname,"FROM [%s]",field->name);
					x=FINDRSC(rs->rsrc,fieldname);
					if(x!=(-1))
					{
						member=rs->rsrc->rscs+x;
						*frs->from.short_value=
							*member->value.short_value;
						if(member->w!=NULL) frs->visible=TRUE;
					}
					fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+6);
					sprintf(fieldname,"TO [%s]",field->name);
					x=FINDRSC(rs->rsrc,fieldname);
					if(x!=(-1))
					{
						member=rs->rsrc->rscs+x;
						*frs->to.short_value=
							*member->value.short_value;
						if(member->w!=NULL) frs->visible=TRUE;
					}
				}
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case SDECIMALV:
				frs->from.float_value=Rmalloc(sizeof(double));
				*frs->from.float_value=0;
				frs->to.float_value=Rmalloc(sizeof(double));
				*frs->to.float_value=0;
				if(frs->range)
				{
					fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+8);
					sprintf(fieldname,"FROM [%s]",field->name);
					x=FINDRSC(rs->rsrc,fieldname);
					if(x!=(-1))
					{
						member=rs->rsrc->rscs+x;
						*frs->from.float_value=
							*member->value.float_value;
						if(member->w!=NULL) frs->visible=TRUE;
					}
					fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+6);
					sprintf(fieldname,"TO [%s]",field->name);
					x=FINDRSC(rs->rsrc,fieldname);
					if(x!=(-1))
					{
						member=rs->rsrc->rscs+x;
						*frs->to.float_value=
							*member->value.float_value;
						if(member->w!=NULL) frs->visible=TRUE;
					}
				}
				break;
			case BOOLNS:
				frs->from.string_value=Rmalloc(1);
				*frs->from.string_value=TRUE;
				frs->to.string_value=Rmalloc(1);
				*frs->to.string_value=TRUE;
				frs->visible=FALSE;
				fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+15);
				sprintf(fieldname,"SELECT [%s] TRUE",field->name);
				x=FINDRSC(rs->rsrc,fieldname);
				if(x!=(-1))
				{
					member=rs->rsrc->rscs+x;
					*frs->from.string_value=
						*member->value.string_value;
					if(member->w!=NULL) frs->visible=TRUE;
				}
				fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+16);
				sprintf(fieldname,"SELECT [%s] FALSE",field->name);
				x=FINDRSC(rs->rsrc,fieldname);
				if(x!=(-1))
				{
					member=rs->rsrc->rscs+x;
					*frs->to.string_value=
						*member->value.string_value;
					if(member->w!=NULL) frs->visible=TRUE;
				}
				break;
			case CHARACTERS:
				frs->from.string_value=Rmalloc(1);
				*frs->from.string_value=0;
				frs->to.string_value=Rmalloc(1);
				*frs->to.string_value=0;
				if(frs->range)
				{
					fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+
						8);
					sprintf(fieldname,"FROM [%s]",field->name);
					x=FINDRSC(rs->rsrc,fieldname);
					if(x!=(-1))
					{
						member=rs->rsrc->rscs+x;
						*frs->from.string_value=
							*member->value.string_value;
						if(member->w!=NULL) frs->visible=TRUE;
					}
					fieldname=Rrealloc(fieldname,RDAstrlen(field->name)+6);
					sprintf(fieldname,"TO [%s]",field->name);
					x=FINDRSC(rs->rsrc,fieldname);
					if(x!=(-1))
					{
						member=rs->rsrc->rscs+x;
						*frs->to.string_value=
							*member->value.string_value;
						if(member->w!=NULL) frs->visible=TRUE;
					}
				}
				break;
			case SCROLLEDLIST:
			case BUTTONS:
				prterr("Error cannot range on this field type.");
				break;
			default:
				prterr("Error Field Type [%d] is invalid for virtual field [%s].",field->type,field->name);
				break;
		}
		if(fieldname!=NULL) Rfree(fieldname);
	}
}
short xtestvirtualrangersrc(RDAvirtual *v,RangeScreen *rs,
	void (*SubData)(...),void *args,int line,char *file)
{
	FileRangeStruct *frs=NULL;
	short return_value=FALSE;
	char *tempdateadj=NULL;
	char *tempdatetoadj=NULL,*tempdatefradj=NULL;
	int y;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field)
	{
		prterr("DIAG Testing Virtual Field [%s] using Range Screen [%s] [%s] at line [%d] program [%s].",v->name,rs->rsrc->module,rs->rsrc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(rs->frs!=NULL && rs!=NULL)
	{
		for(y=0,frs=rs->frs;y<rs->num;++y,++frs)
		{
			if(frs->filename==NULL &&
				!RDAstrcmp(frs->fieldname,v->name))
			{
				break;
			}
		}
		if(y<rs->num)
		{
			if(SubData!=NULL)
			{
				if(!v->computed)
				{
					computevirtual(v,SubData,args);
				}
			}
			switch(v->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case PLAINTEXT:
				case CHARACTERS:
				case SOCSECNUM:
				case PHONE:
				case CUSTOMTYPE:
				case TIMES:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					if(frs->range)
					{
						unpad(v->value.string_value);
						unpad(frs->from.string_value);
						unpad(frs->to.string_value);
						if(RDAstrcmp(v->value.string_value,
							frs->from.string_value)<0)
						{
							return_value=TRUE;
						}
						if(RDAstrcmp(v->value.string_value,							frs->to.string_value)>0) 
						{
							return_value=TRUE;
						}
					}
					break;
				case DATES:
					if(frs->range)
					{
						unpad(v->value.string_value);
						if(!isEMPTY(v->value.string_value))
						{
							if(RDAstrlen(v->value.string_value)==10)
							{
								tempdateadj=Rmalloc(9);
								sprintf(tempdateadj,"%c%c%c%c%c%c%c%c",v->value.string_value[6],v->value.string_value[7],v->value.string_value[8],v->value.string_value[9],v->value.string_value[0],v->value.string_value[1],v->value.string_value[3],v->value.string_value[4]);
							} else if(RDAstrlen(v->value.string_value)==8)
							{
								tempdateadj=Rmalloc(9);
								sprintf(tempdateadj,"%c%c%c%c%c%c%c%c",'1','9',v->value.string_value[6],v->value.string_value[7],v->value.string_value[0],v->value.string_value[1],v->value.string_value[3],v->value.string_value[4]);

							} 
						}
						if(!isEMPTY(frs->from.string_value))
						{
							if(RDAstrlen(frs->from.string_value)==10)
							{
								tempdatefradj=Rmalloc(9);
								sprintf(tempdatefradj,"%c%c%c%c%c%c%c%c",frs->from.string_value[6],frs->from.string_value[7],frs->from.string_value[8],frs->from.string_value[9],frs->from.string_value[0],frs->from.string_value[1],frs->from.string_value[3],frs->from.string_value[4]);
							} else if(RDAstrlen(frs->to.string_value)==8)
							{
								tempdatefradj=Rmalloc(9);
								sprintf(tempdatefradj,"%c%c%c%c%c%c%c%c",'1','9',frs->from.string_value[6],frs->from.string_value[7],frs->from.string_value[0],frs->from.string_value[1],frs->from.string_value[3],frs->from.string_value[4]);
							}
						}
						if(!isEMPTY(frs->to.string_value))
						{
							if(RDAstrlen(frs->to.string_value)==10)
							{
								tempdatetoadj=Rmalloc(9);
								sprintf(tempdatetoadj,"%c%c%c%c%c%c%c%c",frs->to.string_value[6],frs->to.string_value[7],frs->to.string_value[8],frs->to.string_value[9],frs->to.string_value[0],frs->to.string_value[1],frs->to.string_value[3],frs->to.string_value[4]);
							} else if(RDAstrlen(frs->to.string_value)==8)
							{
								tempdatetoadj=Rmalloc(9);
								sprintf(tempdatetoadj,"%c%c%c%c%c%c%c%c",'1','9',frs->to.string_value[6],frs->to.string_value[7],frs->to.string_value[0],frs->to.string_value[1],frs->to.string_value[3],frs->to.string_value[4]);
							}
						}
						if(RDAstrcmp(tempdateadj,tempdatefradj)<0)
						{
							return_value=TRUE;
						}
						if(RDAstrcmp(tempdateadj,tempdatetoadj)>0)
						{
							return_value=TRUE;
						}
						if(tempdateadj!=NULL) Rfree(tempdateadj);
						if(tempdatefradj!=NULL) Rfree(tempdatefradj);
						if(tempdatetoadj!=NULL) Rfree(tempdatetoadj);

					}
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					if(frs->range)
					{
						if(*v->value.integer_value<*frs->from.integer_value || 
							*v->value.integer_value>*frs->to.integer_value) 
							return_value=TRUE;
					}
					break;
				case SHORTV:
				case SSHORTV:
					if(frs->range)
					{
						if(*v->value.short_value<*frs->from.short_value || 
							*v->value.short_value>*frs->to.short_value) 
							return_value=TRUE;
					}
					break;
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case SDECIMALV:
					if(frs->range)
					{
						if(*v->value.float_value<*frs->from.float_value || 
							*v->value.float_value>*frs->to.float_value) 
							return_value=TRUE;
					}
					break;
				case BUTTONS:
					break;
				case BOOLNS:
					if((*v->value.string_value && !*frs->from.string_value)) 
						return_value=TRUE;
					if((!*v->value.string_value && !*frs->to.string_value)) 
						return_value=TRUE;		
					break;
				default:
					prterr("Error Field Type [%d] is invalid for virtual field [%s].",v->type,v->name);
					break;
			}
		}
	}	
#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diagvirtual_field)
	{
		prterr("DIAG testvirtualrangersrc Returning [%d] for virtual field [%s].",return_value,v->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return return_value;
}
