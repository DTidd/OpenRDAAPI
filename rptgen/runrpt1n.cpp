/* runrpt1n.c - Run Report Functions Additional Program #2 */
#include<rptgen.hpp>

short FIELDReportGenSubData1(char **tmp,HoldReport *h,char *modulename)
{
	char *filename=NULL,*fieldname=NULL;
	NRDfield *field=NULL;
	short tmpfileno=1,start=1,length=0,fileno=(-1),str_length=0;
	char *temp=NULL,*gn=NULL,doall=TRUE;
	RDArunrpt *rrpt=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diageval)
	{
		prterr("DIAG FIELDRepoortSubData1(): Substituting for [%s].",*tmp);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	rrpt=h->rrpt;
	filename=stripfilename(*tmp);
	if(filename!=NULL)
	{
		fieldname=stripfieldname(*tmp);
		if(fieldname!=NULL)
		{
			memset(stemp,0,101);
			temp=Rmalloc(RDAstrlen(modulename)+RDAstrlen(filename)+RDAstrlen(fieldname)+7);
			sprintf(temp,"[%s][%s][%s]",modulename,filename,fieldname);
			field=FLDNRD(rrpt->sortno,temp);
			if(temp!=NULL) Rfree(temp);
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
					case OPTIONALFIELDS:
						if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+RDAstrlen(filename)+RDAstrlen(fieldname)+6))
						{
							temp=*tmp+(RDAstrlen(modulename)+RDAstrlen(filename)+RDAstrlen(fieldname)+7);
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
							doall=FALSE;
						}
						str_length=(field->data.string_value!=NULL ? RDAstrlen(field->data.string_value):0);
						if(length==0) length=str_length;
						if(RDAstrlen(*tmp)<(length+3))
						{
							*tmp=Rrealloc(*tmp,length+3);
						}
						if(doall)
						{
							sprintf(*tmp,"\"%s\"",(field->data.string_value!=NULL ? field->data.string_value:""));
						} else if(field->data.string_value!=NULL && (start-1)<str_length && str_length>=length)
						{
							temp=stralloc(field->data.string_value);
							gn=temp+(start-1);
							if(RDAstrlen(gn)>(length))
								temp[start-1+length]=0;
							sprintf(*tmp,"\"%s\"",&temp[start-1]);
							if(temp!=NULL) Rfree(temp);
							temp=NULL;
						} else if((start-1)<str_length && !isEMPTY(field->data.string_value))
						{
							sprintf(*tmp,"\"%s\"",&field->data.string_value[start-1]);
						} else {
							sprintf(*tmp,"\"\"");
						}
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
						if(RDAstrlen(*tmp)<str_length)
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memcpy(*tmp,stemp,str_length+1);
						break;
					case DECIMALV:
					case SDECIMALV:
						if(*field->data.float_value<0)
						{
							sprintf(stemp,"(%f)",*field->data.float_value);
						} else {
							sprintf(stemp,"%f",*field->data.float_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<str_length)
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memcpy(*tmp,stemp,str_length+1);
						break;
					case SDOUBLEV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DOUBLEV:
						if(*field->data.float_value<0)
						{
							sprintf(stemp,"(%.0f)",*field->data.float_value);
						} else {
							sprintf(stemp,"%.0f",*field->data.float_value);
						}
						str_length=RDAstrlen(stemp);
						if(RDAstrlen(*tmp)<str_length)
						{
							*tmp=Rrealloc(*tmp,str_length+1);
						}
						memcpy(*tmp,stemp,str_length+1);
						break;
					case BOOLNS:
						if(RDAstrlen(*tmp)<2)
						{
							*tmp=Rrealloc(*tmp,2);
						}
						sprintf(*tmp,"%d",(*field->data.string_value ? 1:0));
						break;
					case CHARACTERS:
						if(RDAstrlen(*tmp)<2)
						{
							*tmp=Rrealloc(*tmp,2);
						}
						sprintf(*tmp,"%c",*field->data.string_value);
						break;
					default:
						prterr("Error field type %d is invalid for %s.",
							field->type,field->name); 
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=NULL;
						break;
				}
			} else {
				if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+RDAstrlen(filename)+RDAstrlen(fieldname)+6))
				{
					temp=*tmp+(RDAstrlen(modulename)+RDAstrlen(filename)+RDAstrlen(fieldname)+7);
					gn=strtok(temp,",]");
					if(gn!=NULL)
					{
						if(!isEMPTY(gn))
						{
							tmpfileno=Ratoi(gn);
						} else tmpfileno=1;
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
						if(tmpfileno<1) tmpfileno=1;
					}
					if(start<1) start=1;
					doall=FALSE;
				}
				fileno=REPORTFILENUMBER(rrpt,modulename,filename,tmpfileno,TRUE);
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
						case OPTIONALFIELDS:
							str_length=(field->data.string_value!=NULL ? RDAstrlen(field->data.string_value):0);
							if(length==0) length=str_length;
							if(RDAstrlen(*tmp)<(length+3))
							{
								*tmp=Rrealloc(*tmp,length+3);
							}
							if(doall)
							{
								sprintf(*tmp,"\"%s\"",(field->data.string_value!=NULL ? field->data.string_value:""));
							} else if(field->data.string_value!=NULL && (start-1)<str_length && str_length>length)
							{
								temp=stralloc(field->data.string_value);
								gn=temp+(start-1);
								if(RDAstrlen(gn)>(start-1+length))
									temp[start-1+length]=0;
								sprintf(*tmp,"\"%s\"",&temp[start-1]);
								if(temp!=NULL) Rfree(temp);
								temp=NULL;
							} else if((start-1)<str_length && 
								!isEMPTY(field->data.string_value))
							{
								sprintf(*tmp,"\"%s\"",&field->data.string_value[start-1]);
							} else {
								sprintf(*tmp,"\"\"");
							}
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
							if(RDAstrlen(*tmp)<str_length)
							{
								*tmp=Rrealloc(*tmp,str_length+1);
							}
							memcpy(*tmp,stemp,str_length+1);
							break;
						case DECIMALV:
						case SDECIMALV:
							if(*field->data.float_value<0)
							{
								sprintf(stemp,"(%f)",*field->data.float_value);
							} else {
								sprintf(stemp,"%f",*field->data.float_value);
							}
							str_length=RDAstrlen(stemp);
							if(RDAstrlen(*tmp)<str_length)
							{
								*tmp=Rrealloc(*tmp,str_length+1);
							}
							memcpy(*tmp,stemp,str_length+1);
							break;
						case SDOUBLEV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DOUBLEV:
							if(*field->data.float_value<0)
							{
								sprintf(stemp,"(%.0f)",*field->data.float_value);
							} else {
								sprintf(stemp,"%.0f",*field->data.float_value);
							}
							str_length=RDAstrlen(stemp);
							if(RDAstrlen(*tmp)<str_length)
							{
								*tmp=Rrealloc(*tmp,str_length+1);
							}
							memcpy(*tmp,stemp,str_length+1);
							break;
						case BOOLNS:
							if(RDAstrlen(*tmp)<2)
							{
								*tmp=Rrealloc(*tmp,2);
							}
							sprintf(*tmp,"%d",(*field->data.string_value ? 1:0));
							break;
						case CHARACTERS:
							if(RDAstrlen(*tmp)<2)
							{
								*tmp=Rrealloc(*tmp,2);
							}
							sprintf(*tmp,"%c",*field->data.string_value);
							break;
						default:
							prterr("Error field type %d is invalid for %s.",field->type,field->name); 
							if(*tmp!=NULL) Rfree(*tmp);
							*tmp=NULL;
							break;
					}
				} else {
					if(fieldname!=NULL) Rfree(fieldname);
					if(filename!=NULL) Rfree(filename);
					return(1);
				}
			}
			if(fieldname!=NULL) Rfree(fieldname);
			if(filename!=NULL) Rfree(filename);
		} else {
			if(filename!=NULL) Rfree(filename);
			return(1);
		}
	} else {
		return(1);
	}
	return(0);
}
short SORTReportGenSubData1(char **tmp,HoldReport *h,char *modulename)
{
	NRDfield *field=NULL;
	short start=1,length=0,str_length=0;
	char *temp=NULL,*gn=NULL,doall=TRUE;
	RDArunrpt *rrpt=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diageval)
        {
                prterr("DIAG SORTReportGenSubData1(): Substituting for [%s].",*tmp);
        }
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	rrpt=h->rrpt;
	if(rrpt==NULL) return(1);
	if(isEMPTY(modulename)) return(1);
	temp=Rmalloc(RDAstrlen(modulename)+3);
	if(!RDAstrcmp(modulename,"DELETEFLAG"))
	{
		sprintf(temp,"%s",modulename);
	} else {
		sprintf(temp,"[%s]",modulename);
	}
	field=FLDNRD(rrpt->sortno,temp);
	if(temp!=NULL) Rfree(temp);
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
			case OPTIONALFIELDS:
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
						gn=strtok(NULL,"]");
						if(!isEMPTY(gn))
						{
							length=Ratoi(gn);
						} else length=0;
					}
					if(start<1) start=1;
					doall=FALSE;
				}
				str_length=(field->data.string_value!=NULL ? RDAstrlen(field->data.string_value):0);
				if(length==0) length=str_length;
				if(RDAstrlen(*tmp)<(length+3))
				{
					*tmp=Rrealloc(*tmp,length+3);
				}
				if(doall)
				{
					sprintf(*tmp,"\"%s\"",(field->data.string_value!=NULL ? field->data.string_value:""));
				} else if(!isEMPTY(field->data.string_value) && (start-1)<str_length && str_length>length)
				{
					temp=stralloc(field->data.string_value);
					gn=temp+(start-1);
					if(RDAstrlen(gn)>(length))
						temp[start-1+length]=0;
					sprintf(*tmp,"\"%s\"",&temp[start-1]);
					if(temp!=NULL) Rfree(temp);
					temp=NULL;
				} else if(!isEMPTY(field->data.string_value) &&
					(start-1)<str_length)
				{
					sprintf(*tmp,"\"%s\"",&field->data.string_value[start-1]);
				} else {
					sprintf(*tmp,"\"\"");
				}
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
				if(RDAstrlen(*tmp)<str_length)
				{
					*tmp=Rrealloc(*tmp,str_length+1);
				}
				memcpy(*tmp,stemp,str_length+1);
				break;
			case DECIMALV:
			case SDECIMALV:
				if(*field->data.float_value<0)
				{
					sprintf(stemp,"(%f)",*field->data.float_value);
				} else {
					sprintf(stemp,"%f",*field->data.float_value);
				}
				str_length=RDAstrlen(stemp);
				if(RDAstrlen(*tmp)<str_length)
				{
					*tmp=Rrealloc(*tmp,str_length+1);
				}
				memcpy(*tmp,stemp,str_length+1);
				break;
			case SDOUBLEV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DOUBLEV:
				if(*field->data.float_value<0)
				{
					sprintf(stemp,"(%.0f)",*field->data.float_value);
				} else {
					sprintf(stemp,"%.0f",*field->data.float_value);
				}
				str_length=RDAstrlen(stemp);
				if(RDAstrlen(*tmp)<str_length)
				{
					*tmp=Rrealloc(*tmp,str_length+1);
				}
				memcpy(*tmp,stemp,str_length+1);
				break;
			case BOOLNS:
				if(RDAstrlen(*tmp)<2)
				{
					*tmp=Rrealloc(*tmp,2);
				}
				sprintf(*tmp,"%d",(*field->data.string_value ? 1:0));
				break;
			case CHARACTERS:
				if(RDAstrlen(*tmp)<2)
				{
					*tmp=Rrealloc(*tmp,2);
				}
				sprintf(*tmp,"%c",*field->data.string_value);
				break;
			default:
				prterr("Error field type %d is invalid for %s.",field->type,field->name); 
				if(*tmp!=NULL) Rfree(*tmp);
				break;
		}
	} else {
		return(1);
	}
	return(0);
}
short ACCUMReportGenSubData1(char **tmp,HoldReport *h,char *modulename)
{
	char *filename=NULL;
	RDAaccum *a=NULL;
	RDAaccval *accval=NULL;
	int y=0;
	RDAreport *rpt=NULL;
        char *temp=NULL,*gn=NULL,doall=TRUE;
        short start=1,length=0,str_length=0,blevel=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diageval)
        {
                prterr("DIAG ACCUMReportGenSubData1(): Substituting for [%s].",*tmp);
        }
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(isEMPTY(modulename)) return(1);
	rpt=h->rpt;
	if(rpt->accum!=NULL && rpt->numaccums!=0)
	{
		for(y=0,a=rpt->accum;y<rpt->numaccums;++y,++a)
		{
			if(!RDAstrcmp(modulename,a->name)) break;
		}
		if(y<rpt->numaccums)
		{
			blevel=rpt->currbrk;
			if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+2))
			{
				filename=(*tmp)+(RDAstrlen(modulename)+2);
				for(y=0,accval=a->accvals;y<a->num;++y,++accval)
				{
					if(!RDAstrcmp(filename,accval->breakname)) break;
				} 
				if(y<a->num) blevel=y;
				filename=NULL;
			}
			accval=a->accvals+blevel;
			switch(a->fieldtype)
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
					switch(a->atype)
					{
						case 2: /* FIRST OCCURRENCE */
						case 3: /* LAST OCCURRENCE */
						case 6: /* PREVIOUS OCCURRENCE */
							if(RDAstrlen(*tmp)>(RDAstrlen(a->name)+2))
							{
								temp=*tmp+(RDAstrlen(a->name)+3);
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
								doall=FALSE;
								if(start<1) start=1;
							}
							str_length=(accval->prev_value.string_value!=NULL ? RDAstrlen(accval->prev_value.string_value):0);
							if(length==0) length=str_length;
							if(RDAstrlen(*tmp)<(length+3))
							{
								*tmp=Rrealloc(*tmp,length+3);
							}
							if(doall)
							{
								sprintf(*tmp,"\"%s\"",(accval->prev_value.string_value!=NULL ? accval->prev_value.string_value:""));
							} else if(!isEMPTY(accval->prev_value.string_value) && (start-1)<str_length && str_length>length)
							{
								temp=stralloc(accval->prev_value.string_value);
								gn=temp+(start-1);
								if(RDAstrlen(gn)>(start-1+length))
									temp[start-1+length]=0;
								sprintf(*tmp,"\"%s\"",&temp[start-1]);
								if(temp!=NULL) Rfree(temp);
								temp=NULL;
							} else if((start-1)<str_length && !isEMPTY(accval->prev_value.string_value))
							{
								sprintf(*tmp,"\"%s\"",&accval->prev_value.string_value[start-1]);
							} else {
								sprintf(*tmp,"\"\"");
							}
							break;
						case 0: /* AVERAGE */
						case 4: /* MAXIMUM */
						case 5: /* MINIMUM */
						case 7: /* SUMMATION */
							if(RDAstrlen(*tmp)>(RDAstrlen(a->name)+2))
							{
								temp=*tmp+(RDAstrlen(a->name)+3);
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
								doall=FALSE;
								if(start<1) start=1;
							}
							str_length=(accval->value.string_value!=NULL ? RDAstrlen(accval->value.string_value):0);
							if(length==0) length=str_length;
							if(RDAstrlen(*tmp)<(length+3))
							{
								*tmp=Rrealloc(*tmp,length+3);
							}
							if(doall)
							{
								sprintf(*tmp,"\"%s\"",(accval->value.string_value!=NULL ? accval->value.string_value:""));
							} else if(!isEMPTY(accval->value.string_value) && (start-1)<str_length && str_length>length)
							{
								temp=stralloc(accval->value.string_value);
								gn=temp+(start-1);
								if(RDAstrlen(gn)>(start-1+length))
									temp[start-1+length]=0;
								sprintf(*tmp,"\"%s\"",&temp[start-1]);
								if(temp!=NULL) Rfree(temp);
								temp=NULL;
							} else if((start-1)<str_length && !isEMPTY(accval->value.string_value))
							{
								sprintf(*tmp,"\"%s\"",&accval->value.string_value[start-1]);
							} else {
								sprintf(*tmp,"\"\"");
							}
							break;
						case 1: /* FREQUENCY */
							sprintf(stemp,"%.0f",*accval->value.float_value);
							str_length=RDAstrlen(stemp);
							if(RDAstrlen(*tmp)<str_length)
							{
								*tmp=Rrealloc(*tmp,str_length+1);
							}
							memcpy(*tmp,stemp,str_length+1);
							break;
						default:
							prterr("Error unrecognized accumulator type.");
							break;
					}
					break;
				case SSHORTV:
				case SLONGV:
				case SDOUBLEV:
				case SHORTV:
				case SCROLLEDLIST:
				case LONGV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DOUBLEV:
				case CHARACTERS:
				case BOOLNS:
					switch(a->atype)
					{
						case 2: /* FIRST OCCURRENCE */
						case 3: /* LAST OCCURRENCE */
						case 6: /* PREVIOUS OCCURRENCE */
							if(*accval->prev_value.float_value<0)
							{
								sprintf(stemp,"(%.0f)",*accval->prev_value.float_value);
							} else {
								sprintf(stemp,"%.0f",*accval->prev_value.float_value);
							}
							break;
						case 0: /* AVERAGE */
						case 4: /* MAXIMUM */
						case 5: /* MINIMUM */
						case 7: /* SUMMATION */
						case 1: /* FREQUENCY */
							if(*accval->value.float_value<0)
							{
								sprintf(stemp,"(%.0f)",*accval->value.float_value);
							} else {
								sprintf(stemp,"%.0f",*accval->value.float_value);
							}
							break;
						default:
							prterr("Error unrecognized accumulator type.");
							memset(stemp,0,101);
							break;
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<str_length)
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memcpy(*tmp,stemp,str_length+1);
					break;
				case SDECIMALV:
				case DECIMALV:
					switch(a->atype)
					{
						case 2: /* FIRST OCCURRENCE */
						case 3: /* LAST OCCURRENCE */
						case 6: /* PREVIOUS OCCURRENCE */
							if(*accval->prev_value.float_value<0)
							{
								sprintf(stemp,"(%f)",*accval->prev_value.float_value);
							} else {
								sprintf(stemp,"%f",*accval->prev_value.float_value);
							}
							break;
						case 0: /* AVERAGE */
						case 4: /* MAXIMUM */
						case 5: /* MINIMUM */
						case 7: /* SUMMATION */
						case 1: /* FREQUENCY */
							if(*accval->value.float_value<0)
							{
								sprintf(stemp,"(%f)",*accval->value.float_value);
							} else {
								sprintf(stemp,"%f",*accval->value.float_value);
							}
							break;
						default:
							prterr("Error unrecognized accumulator type.");
							memset(stemp,0,101);
							break;
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<str_length)
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memcpy(*tmp,stemp,str_length+1);
					break;
				default:
					prterr("Error field type %d is invalid for %s.",a->fieldtype,a->name); 
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=NULL;
					break;
			}
		} else {
			return(1);
		}
	} else {
		return(1);
	}
	return(0);
}
short VIRTUALReportGenSubData1(char **tmp,HoldReport *h,char *modulename)
{
	char *dashname=NULL,*tempvalue=NULL;
	char *tempchar=" ";
	int y=0;
	RDAvirtual *v=NULL;
	RDAreport *rpt=NULL;
	RDArunrpt *rrpt=NULL;
        char *temp=NULL,*gn=NULL,doall=TRUE;
        short start=1,length=0,str_length=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diageval)
        {
                prterr("DIAG VIRTUALReportGenSubData1(): Substituting for [%s].",*tmp);
        }
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	rpt=h->rpt;
	rrpt=h->rrpt;
	if(isEMPTY(modulename)) return(1);
	if(rpt->virflds!=NULL && rpt->numvirtuals!=0)
	{
		for(y=0,v=rpt->virflds;y<rpt->numvirtuals;++y,++v)
		{
			if(!RDAstrcmp(modulename,v->name)) break;
		}
		if(y<rpt->numvirtuals)
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
						if(v->range==2)
						{
							if(rrpt->rsrsrc!=NULL)
							{
								FINDRSCGETSTRING(rrpt->rsrsrc,v->name,&v->value.string_value);
							}
						} else if(v->range==3)
						{
							dashname=adddashes(v->name);
							tempvalue=RDA_GetEnv(dashname);
							if(dashname!=NULL) Rfree(dashname);
							if(!isEMPTY(tempvalue))
							{
								if(v->value.string_value!=NULL) Rfree(v->value.string_value);
								v->value.string_value=stralloc(tempvalue);
								v->dlen=RDAstrlen(v->value.string_value)+1;
							} else if(v->value.string_value!=NULL)
							{
								memset(v->value.string_value,0,v->dlen);
							} else {
								v->value.string_value=NULL;
								v->dlen=0;
							}
						}
						if(!isEMPTY(v->expression))
						{
							temp=PP_EVALUATEstr(v->expression,ReportGenSubData1,h);
							if(v->value.string_value!=NULL) Rfree(v->value.string_value);
							v->value.string_value=stralloc(temp);
							if(temp!=NULL) Rfree(temp);
						}
						if(v->value.string_value!=NULL)
						{
							v->dlen=RDAstrlen(v->value.string_value)+1;
							if(v->dlen>v->len && v->len>0)
							{
								v->value.string_value[v->len]=0;
							}
						} else v->dlen=0;
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
						doall=FALSE;
					}
					str_length=(v->value.string_value!=NULL?RDAstrlen(v->value.string_value):0);
					if(length==0) length=str_length;
					if(RDAstrlen(*tmp)<(length+3))
					{
						*tmp=Rrealloc(*tmp,length+3);
					}
					if(doall)
					{
						sprintf(*tmp,"\"%s\"",(v->value.string_value!=NULL ? v->value.string_value:""));
					} else if(!isEMPTY(v->value.string_value) && (start-1)<str_length && str_length<length)
					{
						temp=stralloc(v->value.string_value);
						gn=temp+(start-1);
						if(RDAstrlen(gn)>(length))
						{
							temp[start-1+length]=0;
						}
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
						if(temp!=NULL) Rfree(temp);
						temp=NULL;
					} else if((start-1)<str_length && !isEMPTY(v->value.string_value))
					{
						sprintf(*tmp,"\"%s\"",&v->value.string_value[start-1]);
					} else {
						sprintf(*tmp,"\"\"");
					}
					if(!RDAstrcmp(v->name,"ERROR DESCRIPTION") && !isEMPTY(v->value.string_value))
					{
						if(rrpt->errorlist->numlibs==1 && !RDAstrcmp(rrpt->errorlist->libs[0],"No Errors Encountered")) 
						{
							freeapplib(rrpt->errorlist);
							rrpt->errorlist=APPlibNEW();
						}
						addAPPlib(rrpt->errorlist,v->value.string_value);
					}
					break;
				case SHORTV:
				case SSHORTV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						if(v->range==2)
						{
							if(rrpt->rsrsrc!=NULL)
							{
								FINDRSCGETSHORT(rrpt->rsrsrc,v->name,v->value.short_value);
							}
						} else if(v->range==3)
						{
							dashname=adddashes(v->name);
							tempvalue=RDA_GetEnv(dashname);
							if(dashname!=NULL) Rfree(dashname);
							if(v->dlen==0)
							{
								v->value.short_value=Rmalloc(sizeof(short));
								v->dlen=sizeof(short);
							}
							if(!isEMPTY(tempvalue))
							{
								*v->value.short_value=Ratoi(tempvalue);
							} else {
								*v->value.short_value=0;
							}
						}
						if(!isEMPTY(v->expression))
						{
							*v->value.short_value=PP_EVALUATEsht(v->expression,ReportGenSubData1,h);
						}
					}
					if(*v->value.short_value<0)
					{
						sprintf(stemp,"(%d)",*v->value.short_value);
					} else {
						sprintf(stemp,"%d",*v->value.short_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<str_length)
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memcpy(*tmp,stemp,str_length+1);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						if(v->range==2)
						{
							if(rrpt->rsrsrc!=NULL)
							{
								FINDRSCGETINT(rrpt->rsrsrc,v->name,v->value.integer_value);
							}
						} else if(v->range==3)
						{
							dashname=adddashes(v->name);
							tempvalue=RDA_GetEnv(dashname);
							if(dashname!=NULL) Rfree(dashname);
							if(v->dlen==0)
							{
								v->value.integer_value=Rmalloc(sizeof(int));
								v->dlen=sizeof(int);
							}
							if(!isEMPTY(tempvalue))
							{
								*v->value.integer_value=Ratoi(tempvalue);
							} else {
								*v->value.integer_value=0;
							}
						}
						if(!isEMPTY(v->expression))
						{
							*v->value.integer_value=PP_EVALUATEint(v->expression,ReportGenSubData1,h);
						}
					}
					if(*v->value.integer_value<0)
					{
						sprintf(stemp,"(%d)",*v->value.integer_value);
					} else {
						sprintf(stemp,"%d",*v->value.integer_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<str_length)
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memcpy(*tmp,stemp,str_length+1);
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DOUBLEV:
				case SDOUBLEV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						if(v->range==2)
						{
							if(rrpt->rsrsrc!=NULL)
							{
								FINDRSCGETDOUBLE(rrpt->rsrsrc,v->name,v->value.float_value);
							}
						} else if(v->range==3)
						{
							dashname=adddashes(v->name);
							tempvalue=RDA_GetEnv(dashname);
							if(dashname!=NULL) Rfree(dashname);
							if(v->dlen==0)
							{
								v->value.float_value=Rmalloc(sizeof(double));
								v->dlen=sizeof(double);
							}
							if(!isEMPTY(tempvalue))
							{
								*v->value.float_value=atof(tempvalue);
							} else {
								*v->value.float_value=0;
							}
						}
						if(!isEMPTY(v->expression))
						{
							*v->value.float_value=PP_EVALUATEdbl(v->expression,ReportGenSubData1,h);
							if(v->type==DOLLARS || v->type==DOUBLEV ||
								v->type==SDOUBLEV ||
								v->type==DOLLARS_NOCENTS)
							{
								*v->value.float_value=round(*v->value.float_value);
							}
						}
					}
					if(*v->value.float_value<0)
					{
						sprintf(stemp,"(%.0f)",*v->value.float_value);
					} else {
						sprintf(stemp,"%.0f",*v->value.float_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<str_length)
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
					memcpy(*tmp,stemp,str_length+1);
					break;
				case DECIMALV:
				case SDECIMALV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						if(v->range==2)
						{
							if(rrpt->rsrsrc!=NULL)
							{
								FINDRSCGETDOUBLE(rrpt->rsrsrc,v->name,v->value.float_value);
							}
						} else if(v->range==3)
						{
							dashname=adddashes(v->name);
							tempvalue=RDA_GetEnv(dashname);
							if(dashname!=NULL) Rfree(dashname);
							if(v->dlen==0)
							{
								v->value.float_value=Rmalloc(sizeof(double));
								v->dlen=sizeof(double);
							}
							if(!isEMPTY(tempvalue))
							{
								*v->value.float_value=atof(tempvalue);
							} else {
								*v->value.float_value=0;
							}
						}
						if(!isEMPTY(v->expression))
						{
							*v->value.float_value=PP_EVALUATEdbl(v->expression,ReportGenSubData1,h);
							if(v->type==DOLLARS || v->type==DOUBLEV ||
								v->type==SDOUBLEV || 
								v->type==DOLLARS_NOCENTS)
							{
								*v->value.float_value=round(*v->value.float_value);
							}
						}
					}
					if(*v->value.float_value<0)
					{
						sprintf(stemp,"(%f)",*v->value.float_value);
					} else {
						sprintf(stemp,"%f",*v->value.float_value);
					}
					str_length=RDAstrlen(stemp);
					if(RDAstrlen(*tmp)<str_length)
					{
						*tmp=Rrealloc(*tmp,str_length+1);
					}
							memcpy(*tmp,stemp,str_length+1);
					break;
				case BOOLNS:
					if(!v->computed) 
					{
						v->computed=TRUE;
						if(v->range==2)
						{
							if(rrpt->rsrsrc!=NULL)
							{
								FINDRSCGETCHAR(rrpt->rsrsrc,v->name,v->value.string_value);
							}
						} else if(v->range==3)
						{
							dashname=adddashes(v->name);
							tempvalue=RDA_GetEnv(dashname);
							if(dashname!=NULL) Rfree(dashname);
							if(v->dlen==0)
							{
								v->value.string_value=Rmalloc(1);
								v->dlen=1;
							}
							if(!isEMPTY(tempvalue))
							{
								if(Ratoi(tempvalue))
								{
									*v->value.string_value=TRUE;
								} else {
									*v->value.string_value=FALSE;
								}
							} else {
								*v->value.string_value=FALSE;
							}
						}
						if(!isEMPTY(v->expression))
						{
							*v->value.string_value=PP_EVALUATEbol(v->expression,ReportGenSubData1,h);
						}
					}
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					sprintf(*tmp,"%d",(*v->value.string_value ? 1:0));
					break;
				case CHARACTERS:
					if(!v->computed) 
					{
						v->computed=TRUE;
						if(v->range==2)
						{
							if(rrpt->rsrsrc!=NULL)
							{
								FINDRSCGETCHAR(rrpt->rsrsrc,v->name,v->value.string_value);
							}
						} else if(v->range==3)
						{
							dashname=adddashes(v->name);
							tempvalue=RDA_GetEnv(dashname);
							if(dashname!=NULL) Rfree(dashname);
							if(v->dlen==0)
							{
								v->value.string_value=Rmalloc(1);
								v->dlen=1;
							}
							if(!isEMPTY(tempvalue))
							{
								*v->value.string_value=*tempvalue;
							} else {
								*v->value.string_value=*tempchar;
							}
						}
						if(!isEMPTY(v->expression))
						{
							*v->value.string_value=PP_EVALUATEbol(v->expression,ReportGenSubData1,h);
						}
					}
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					sprintf(stemp,"%c",*v->value.string_value);
					break;
				default:
					prterr("Error field type %d is invalid for %s.",v->type,v->name); 
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=NULL;
					break;
			}
		} else {
                	return(1);
		}
	} else {
               	return(1);
	}
	return(0);
}
short RPTGLOBALReportGenSubData1(char **tmp,HoldReport *h,char *modulename)
{
	short str_length=0;
	RDArunrpt *rrpt=NULL;
	RDAreport *rpt=NULL;
	char *temp1=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diageval)
        {
                prterr("DIAG RPTGLOBALReportGenSubData1(): Substituting for [%s].",*tmp);
        }
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	rrpt=h->rrpt;
	rpt=h->rpt;
	if(isEMPTY(modulename)) return(1);
	if(!RDAstrcmp(modulename,"BODY COUNT"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",rrpt->body_count);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
	if(!RDAstrcmp(modulename,"BREAK LEVEL"))
	{
		str_length=RDAstrlen(rrpt->cur_brk_name);
		if(RDAstrlen(*tmp)<(str_length+2))
		{
			*tmp=Rrealloc(*tmp,str_length+3);
		}
		sprintf(*tmp,"\"%s\"",(rrpt->cur_brk_name!=NULL?rrpt->cur_brk_name:""));
		return(0);
	} 
	if(!RDAstrcmp(modulename,"PREVIOUS BREAK LEVEL 2"))
	{
		str_length=RDAstrlen(rrpt->prev_brk_name2);
		if(RDAstrlen(*tmp)<(str_length+2))
		{
			*tmp=Rrealloc(*tmp,str_length+3);
		}
		sprintf(*tmp,"\"%s\"",(rrpt->prev_brk_name2!=NULL?rrpt->prev_brk_name2:""));
		return(0);
	} 
	if(!RDAstrcmp(modulename,"PREVIOUS BREAK LEVEL"))
	{
		str_length=RDAstrlen(rrpt->prev_brk_name);
		if(RDAstrlen(*tmp)<(str_length+2))
		{
			*tmp=Rrealloc(*tmp,str_length+3);
		}
		sprintf(*tmp,"\"%s\"",(rrpt->prev_brk_name!=NULL?rrpt->prev_brk_name:""));
		return(0);
	}
	if(!RDAstrcmp(modulename,"PAGE NO"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",rrpt->pageno);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
	if(!RDAstrcmp(modulename,"PAGE COUNTER"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",rrpt->page_count);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
	if(!RDAstrcmp(modulename,"NUMBER OF LINES"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",rrpt->numlines);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
	if(!RDAstrcmp(modulename,"BODY COUNT LIMIT"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",rpt->display->body_count);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
	if(!RDAstrcmp(modulename,"BODY COUNT TYPE"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",rpt->display->body_count_type);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
	if(!RDAstrcmp(modulename,"PAGE LENGTH LIMIT"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%f",rpt->display->page_length);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
	if(!RDAstrcmp(modulename,"PAGE WIDTH LIMIT"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%f",rpt->display->page_width);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
	if(!RDAstrcmp(modulename,"STARTING PAGE NO"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",rpt->display->starting_page);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
	if(!RDAstrcmp(modulename,"DEVICE NAME"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%s",rpt->display->set_lpp);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
	if(!RDAstrcmp(modulename,"ABLE TO APPROVE"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",Users_Able_to_Approve());
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
	if(!RDAstrcmp(modulename,"APPROVAL LIMIT"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%f",Users_Approval_Limit());
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
	if(!RDAstrcmp(modulename,"USERS DEPARTMENT"))
	{
		memset(stemp,0,101);
		temp1=Users_Department();
		sprintf(stemp,"%s",(temp1!=NULL?temp1:""));
		if(temp1!=NULL) Rfree(temp1);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
/*
	if(!RDAstrcmp(modulename,"DEFAULT PRINTER"))
	{
		memset(stemp,0,101);
		temp1=DefaultPrinter();
		sprintf(stemp,"%s",(temp1!=NULL?temp1:""));
		if(temp1!=NULL) Rfree(temp1);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
*/
	if(!RDAstrcmp(modulename,"TOTAL COUNTER"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%ld",rrpt->total_count);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CURRENT FONT PITCH"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",rrpt->curpitch);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CURRENT FONT POINTS"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",rrpt->curpoints);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
	if(!RDAstrcmp(modulename,"FONT PITCH"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",rrpt->pitch);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
	if(!RDAstrcmp(modulename,"FONT POINTS"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%d",rrpt->points);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
	if(!RDAstrcmp(modulename,"CURRENT FONT"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%s",rrpt->curfont);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
	if(!RDAstrcmp(modulename,"LONGEST FONT"))
	{
		memset(stemp,0,101);
		sprintf(stemp,"%s",rrpt->longfont);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memcpy(*tmp,stemp,str_length+1);
		return(0);
	}
	return(1);
}
static short ExecuteReportGenSubData1(short (*func)(...),char **tmp,HoldReport *h,char *modulename)
{
	short retval=FALSE;

/*lint -e746*/
	if(h!=NULL)
	{
		retval=func(tmp,h,modulename);
	} else {
		retval=func(tmp,modulename);
	}
/*lint +e746*/
	if(retval<1)
	{
		if(retval==(-1))
		{
			prterr("Error Fatal Error encountered substitution aborted.....");
		}
#ifdef USE_RDA_DIAGNOSTICS
		if(diagrptgen || diageval)
		{
			prterr("DIAG ExecuteSubData Returning [%s].",*tmp);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	}
	return(retval);
}
void ReportGenSubData1(char **tmp,HoldReport *h)
{
	char *modulename=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen || diageval)
	{
		prterr("DIAG ReportGenSubData1 Substituting for [%s].",*tmp);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	modulename=stripmodulename(*tmp);
	if(!isEMPTY(modulename))
	{
		if(ExecuteReportGenSubData1(FIELDReportGenSubData1,tmp,h,modulename)<1)
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diagrptgen || diageval)
			{
				prterr("DIAG ReportGenSubData1 Returning [%s].",*tmp);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			if(modulename!=NULL) Rfree(modulename);
			return;
		} else if(ExecuteReportGenSubData1(SORTReportGenSubData1,tmp,h,modulename)<1)
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diagrptgen || diageval)
			{
				prterr("DIAG ReportGenSubData1 Returning [%s].",*tmp);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			if(modulename!=NULL) Rfree(modulename);
			return;
		} else if(ExecuteReportGenSubData1(ACCUMReportGenSubData1,tmp,h,modulename)<1)
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diagrptgen || diageval)
			{
				prterr("DIAG ReportGenSubData1 Returning [%s].",*tmp);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			if(modulename!=NULL) Rfree(modulename);
			return;
		} else if(ExecuteReportGenSubData1(VIRTUALReportGenSubData1,tmp,h,modulename)<1)
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diagrptgen || diageval)
			{
				prterr("DIAG ReportGenSubData1 Returning [%s].",*tmp);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			if(modulename!=NULL) Rfree(modulename);
			return;
		} else if(ExecuteReportGenSubData1(RPTGLOBALReportGenSubData1,tmp,h,modulename)<1)
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diagrptgen || diageval)
			{
				prterr("DIAG ReportGenSubData1 Returning [%s].",*tmp);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			if(modulename!=NULL) Rfree(modulename);
			return;
/*
		} else if(ExecuteReportGenSubData1(GLOBALSubData,tmp,NULL,modulename)==0)
*/
		} else if(GLOBALSubData(tmp,modulename)==0)
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diagrptgen || diageval)
			{
				prterr("DIAG ReportGenSubData1 Returning [%s].",*tmp);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			if(modulename!=NULL) Rfree(modulename);
			return;
		} else {
			prterr("Error ReportGenSubData1 [%s] not found.",*tmp);
			if(*tmp!=NULL) Rfree(*tmp);
			*tmp=stralloc("\"\"");
#ifdef USE_RDA_DIAGNOSTICS
			if(diagrptgen || diageval)
			{
				prterr("DIAG ReportGenSubData1 Returning [%s].",*tmp);
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
			if(modulename!=NULL) Rfree(modulename);
			return;
		}
	} else {
		prterr("Error ReportGenSubData1 [%s] not found.",*tmp);
		if(*tmp!=NULL) Rfree(*tmp);
		*tmp=stralloc("\"\"");
#ifdef USE_RDA_DIAGNOSTICS
		if(diagrptgen || diageval)
		{
			prterr("DIAG ReportGenSubData1 Returning [%s].",*tmp);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		if(modulename!=NULL) Rfree(modulename);
	}
}
void SetFieldValue1(NRDfield *fld,char *tmp,RDAreport *rpt,RDArunrpt *rrpt)
{
	char *modulename=NULL;
	char *dashname=NULL,*tempvalue=NULL;
	char *tempchar=" ";
	unsigned l=0;
	NRDfield *field;
	RDAaccum *a;
	RDAaccval *accval;
	HoldReport *h=NULL;
	int y;
	RDAvirtual *v;
	char *temp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG SetFieldValue1 for Field [%s].",fld->name);
		SEENRDFIELD(fld); 
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	modulename=stripmodulename(tmp);
	memset(stemp,0,101);
	field=FLDNRD(rrpt->sortno,tmp);
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
			case OPTIONALFIELDS:
				switch(fld->type)
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
						if(!isEMPTY(field->data.string_value))
						{
							QUICKALLOC(fld->data.string_value,fld->dlen,field->dlen);
							memcpy(fld->data.string_value,field->data.string_value,field->dlen);
							if(fld->dlen>fld->len && fld->len>0)
								fld->data.string_value[fld->len]=0;
						} else if(fld->dlen>0)
						{
							memset(fld->data.string_value,0,fld->dlen);
						} else {
							fld->data.string_value=NULL;
							fld->dlen=0;
						}
						break;
					default:
						prterr("Error Field type [%d][%s] in Field [%s] not corresponding to Field type [%d][%s] from Field [%s] and cannot be copied.",
							fld->type,fieldtypedesc[fld->type],(fld->name==NULL?"":fld->name),
							field->type,fieldtypedesc[field->type],(tmp==NULL?"":tmp));
						break;
				}
				break;
			case SHORTV:
			case SSHORTV:
				switch(fld->type)
				{
					case SHORTV:
					case SSHORTV:
						*fld->data.short_value=*field->data.short_value;
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						*fld->data.integer_value=*field->data.short_value;
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						*fld->data.float_value=*field->data.short_value;
						break;
					case BOOLNS:
					case CHARACTERS:
						*fld->data.string_value=*field->data.short_value;
						break;
					default:
						prterr("Error Field type [%d][%s] in Field [%s] not corresponding to Field type [%d][%s] from Field [%s] and cannot be copied.",
							fld->type,fieldtypedesc[fld->type],(fld->name==NULL?"":fld->name),
							field->type,fieldtypedesc[field->type],(tmp==NULL?"":tmp));
/*
						prterr("Error Field type [%d] not corresponding to field type [%d] and cannot be copied.",fld->type,field->type);
*/
						break;
				}
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				switch(fld->type)
				{
					case SHORTV:
					case SSHORTV:
						*fld->data.short_value=(short)*field->data.integer_value;
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						*fld->data.integer_value=*field->data.integer_value;
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						*fld->data.float_value=*field->data.integer_value;
						break;
					case BOOLNS:
					case CHARACTERS:
						*fld->data.string_value=*field->data.integer_value;
						break;
					default:
						prterr("Error Field type [%d][%s] in Field [%s] not corresponding to Field type [%d][%s] from Field [%s] and cannot be copied.",
							fld->type,fieldtypedesc[fld->type],(fld->name==NULL?"":fld->name),
							field->type,fieldtypedesc[field->type],(tmp==NULL?"":tmp));
/*
						prterr("Error Field type [%d] not corresponding to field type [%d] and cannot be copied.",fld->type,field->type);
*/
						break;
				}
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				switch(fld->type)
				{
					case SHORTV:
					case SSHORTV:
						*fld->data.short_value=(short)*field->data.float_value;
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						*fld->data.integer_value=(int)*field->data.float_value;
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						*fld->data.float_value=*field->data.float_value;
						break;
					case BOOLNS:
					case CHARACTERS:
						if(*field->data.float_value)
						{
							*fld->data.string_value=TRUE;
						} else {
							*fld->data.string_value=FALSE;
						}
						break;
					default:
						prterr("Error Field type [%d][%s] in Field [%s] not corresponding to Field type [%d][%s] from Field [%s] and cannot be copied.",
							fld->type,fieldtypedesc[fld->type],(fld->name==NULL?"":fld->name),
							field->type,fieldtypedesc[field->type],(tmp==NULL?"":tmp));
/*
						prterr("Error Field type [%d] not corresponding to field type [%d] and cannot be copied.",fld->type,field->type);
*/
						break;
				}
				break;
			case BOOLNS:
			case CHARACTERS:
				switch(fld->type)
				{
					case SHORTV:
					case SSHORTV:
						*fld->data.short_value=*field->data.string_value;
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						*fld->data.integer_value=*field->data.string_value;
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						*fld->data.float_value=*field->data.string_value;
						break;
					case BOOLNS:
					case CHARACTERS:
						*fld->data.string_value=*field->data.string_value;
						break;
					default:
						prterr("Error Field type [%d][%s] in Field [%s] not corresponding to Field type [%d][%s] from Field [%s] and cannot be copied.",
							fld->type,fieldtypedesc[fld->type],(fld->name==NULL?"":fld->name),
							field->type,fieldtypedesc[field->type],(tmp==NULL?"":tmp));
/*
						prterr("Error Field type [%d] not corresponding to field type [%d] and cannot be copied.",fld->type,field->type);
*/
						break;
				}
				break;
			default:
				prterr("Error field type %d is invalid for %s.",
					field->type,field->name); 
				break;
		}
	} else if(!RDAstrcmp(modulename,"ORGANIZATION NAME"))
	{
		if(tmp!=NULL) Rfree(tmp);
		switch(fld->type)
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
				if(XPERT_SETUP->company_name!=NULL)
				{
					l=RDAstrlen(XPERT_SETUP->company_name);
					QUICKALLOC(fld->data.string_value,fld->dlen,l+1);
					memcpy(fld->data.string_value,XPERT_SETUP->company_name,l+1);
					if(fld->dlen>fld->len && fld->len>0)
						fld->data.string_value[fld->len]=0;
				} else if(fld->dlen>0)
				{
					memset(fld->data.string_value,0,fld->dlen);
				} else {
					fld->data.string_value=NULL;
					fld->dlen=0;
				}
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"ORGANIZATION ADDRESS #1"))
	{
		if(tmp!=NULL) Rfree(tmp);
		switch(fld->type)
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
				if(XPERT_SETUP->addr1!=NULL)
				{
					l=RDAstrlen(XPERT_SETUP->addr1);
					QUICKALLOC(fld->data.string_value,fld->dlen,l+1);
					memcpy(fld->data.string_value,XPERT_SETUP->addr1,l+1);
					if(fld->dlen>fld->len && fld->len>0)
						fld->data.string_value[fld->len]=0;
				} else if(fld->dlen>0)
				{
					memset(fld->data.string_value,0,fld->dlen);
				} else {
					fld->data.string_value=NULL;
					fld->dlen=0;
				}
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"ORGANIZATION ADDRESS #2"))
	{
		if(tmp!=NULL) Rfree(tmp);
		switch(fld->type)
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
				if(XPERT_SETUP->addr2!=NULL)
				{
					l=RDAstrlen(XPERT_SETUP->addr2);
					QUICKALLOC(fld->data.string_value,fld->dlen,l+1);
					memcpy(fld->data.string_value,XPERT_SETUP->addr2,l+1);
					if(fld->dlen>fld->len && fld->len>0)
						fld->data.string_value[fld->len]=0;
				} else if(fld->dlen>0)
				{
					memset(fld->data.string_value,0,fld->dlen);
				} else {
					fld->data.string_value=NULL;
					fld->dlen=0;
				}
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"ORGANIZATION CITY"))
	{
		if(tmp!=NULL) Rfree(tmp);
		switch(fld->type)
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
				if(XPERT_SETUP->city!=NULL)
				{
					l=RDAstrlen(XPERT_SETUP->city);
					QUICKALLOC(fld->data.string_value,fld->dlen,l+1);
					memcpy(fld->data.string_value,XPERT_SETUP->city,l+1);
					if(fld->dlen>fld->len && fld->len>0)
						fld->data.string_value[fld->len]=0;
				} else if(fld->dlen>0)
				{
					memset(fld->data.string_value,0,fld->dlen);
				} else {
					fld->data.string_value=NULL;
					fld->dlen=0;
				}
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"ORGANIZATION STATE"))
	{
		if(tmp!=NULL) Rfree(tmp);
		switch(fld->type)
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
				if(XPERT_SETUP->state!=NULL)
				{
					l=RDAstrlen(XPERT_SETUP->state);
					QUICKALLOC(fld->data.string_value,fld->dlen,l+1);
					memcpy(fld->data.string_value,XPERT_SETUP->state,l+1);
					if(fld->dlen>fld->len && fld->len>0)
						fld->data.string_value[fld->len]=0;
				} else if(fld->dlen>0)
				{
					memset(fld->data.string_value,0,fld->dlen);
				} else {
					fld->data.string_value=NULL;
					fld->dlen=0;
				}
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"ORGANIZATION ZIP"))
	{
		if(tmp!=NULL) Rfree(tmp);
		switch(fld->type)
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
				if(XPERT_SETUP->zip!=NULL)
				{
					l=RDAstrlen(XPERT_SETUP->zip);
					QUICKALLOC(fld->data.string_value,fld->dlen,l+1);
					memcpy(fld->data.string_value,XPERT_SETUP->zip,l+1);
					if(fld->dlen>fld->len && fld->len>0)
						fld->data.string_value[fld->len]=0;
				} else if(fld->dlen>0)
				{
					memset(fld->data.string_value,0,fld->dlen);
				} else {
					fld->data.string_value=NULL;
					fld->dlen=0;
				}
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"ORGANIZATION FAX"))
	{
		if(tmp!=NULL) Rfree(tmp);
		switch(fld->type)
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
				if(XPERT_SETUP->fax!=NULL)
				{
					l=RDAstrlen(XPERT_SETUP->fax);
					QUICKALLOC(fld->data.string_value,fld->dlen,l+1);
					memcpy(fld->data.string_value,XPERT_SETUP->fax,l+1);
					if(fld->dlen>fld->len && fld->len>0)
						fld->data.string_value[fld->len]=0;
				} else if(fld->dlen>0)
				{
					memset(fld->data.string_value,0,fld->dlen);
				} else {
					fld->data.string_value=NULL;
					fld->dlen=0;
				}
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"ORGANIZATION PHONE"))
	{
		if(tmp!=NULL) Rfree(tmp);
		switch(fld->type)
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
				if(XPERT_SETUP->phone!=NULL)
				{
					l=RDAstrlen(XPERT_SETUP->phone);
					QUICKALLOC(fld->data.string_value,fld->dlen,l+1);
					memcpy(fld->data.string_value,XPERT_SETUP->phone,l+1);
					if(fld->dlen>fld->len && fld->len>0)
						fld->data.string_value[fld->len]=0;
				} else if(fld->dlen>0)
				{
					memset(fld->data.string_value,0,fld->dlen);
				} else {
					fld->data.string_value=NULL;
					fld->dlen=0;
				}
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"BODY COUNT"))
	{
		switch(fld->type)
		{
			case SHORTV:
			case SSHORTV:
				*fld->data.short_value=rrpt->body_count;
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				*fld->data.integer_value=rrpt->body_count;
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				*fld->data.float_value=rrpt->body_count;
				break;
			case BOOLNS:
			case CHARACTERS:
				*fld->data.string_value=rrpt->body_count;
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"PAGE COUNTER"))
	{
		switch(fld->type)
		{
			case SHORTV:
			case SSHORTV:
				*fld->data.short_value=rrpt->page_count;
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				*fld->data.integer_value=rrpt->page_count;
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				*fld->data.float_value=rrpt->page_count;
				break;
			case BOOLNS:
			case CHARACTERS:
				*fld->data.string_value=rrpt->page_count;
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"NUMBER OF LINES"))
	{
		switch(fld->type)
		{
			case SHORTV:
			case SSHORTV:
				*fld->data.short_value=rrpt->numlines;
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				*fld->data.integer_value=rrpt->numlines;
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				*fld->data.float_value=rrpt->numlines;
				break;
			case BOOLNS:
			case CHARACTERS:
				*fld->data.string_value=rrpt->numlines;
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
 	} else if(!RDAstrcmp(modulename,"BODY COUNT LIMIT"))
	{
		switch(fld->type)
		{
			case SHORTV:
			case SSHORTV:
				*fld->data.short_value=rpt->display->body_count;
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				*fld->data.integer_value=rpt->display->body_count;
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				*fld->data.float_value=rpt->display->body_count;
				break;
			case BOOLNS:
			case CHARACTERS:
				*fld->data.string_value=rpt->display->body_count;
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"BODY COUNT TYPE"))
	{
		switch(fld->type)
		{
			case SHORTV:
			case SSHORTV:
				*fld->data.short_value=rpt->display->body_count_type;
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				*fld->data.integer_value=rpt->display->body_count_type;
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				*fld->data.float_value=rpt->display->body_count_type;
				break;
			case BOOLNS:
			case CHARACTERS:
				*fld->data.string_value=rpt->display->body_count_type;
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"PAGE LENGTH LIMIT"))
	{
		switch(fld->type)
		{
			case SHORTV:
			case SSHORTV:
				*fld->data.short_value=(short)rpt->display->page_length;
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				*fld->data.integer_value=(int)rpt->display->page_length;
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				*fld->data.float_value=rpt->display->page_length;
				break;
			case BOOLNS:
			case CHARACTERS:
				if(rpt->display->page_length)
				{
					*fld->data.string_value=TRUE;
				} else {
					*fld->data.string_value=FALSE;
				}
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"PAGE WIDTH LIMIT"))
	{
		switch(fld->type)
		{
			case SHORTV:
			case SSHORTV:
				*fld->data.short_value=(short)rpt->display->page_width;
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				*fld->data.integer_value=(int)rpt->display->page_width;
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				*fld->data.float_value=rpt->display->page_width;
				break;
			case BOOLNS:
			case CHARACTERS:
				if(rpt->display->page_width)
				{
					*fld->data.string_value=TRUE;
				} else {
					*fld->data.string_value=FALSE;
				}
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"STARTING PAGE NO"))
	{
		switch(fld->type)
		{
			case SHORTV:
			case SSHORTV:
				*fld->data.short_value=rpt->display->starting_page;
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				*fld->data.integer_value=rpt->display->starting_page;
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				*fld->data.float_value=rpt->display->starting_page;
				break;
			case BOOLNS:
			case CHARACTERS:
				*fld->data.string_value=rpt->display->starting_page;
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"DEVICE NAME"))
	{
		if(tmp!=NULL) Rfree(tmp);
		switch(fld->type)
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
				if(rpt->display->set_lpp!=NULL)
				{
					l=RDAstrlen(rpt->display->set_lpp);
					QUICKALLOC(fld->data.string_value,fld->dlen,l+1);
					memcpy(fld->data.string_value,rpt->display->set_lpp,l+1);
					if(fld->dlen>fld->len && fld->len>0)
						fld->data.string_value[fld->len]=0;
				} else if(fld->dlen>0)
				{
					memset(fld->data.string_value,0,fld->dlen);
				} else {
					fld->data.string_value=NULL;
					fld->dlen=0;
				}
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"BREAK LEVEL"))
	{
		if(tmp!=NULL) Rfree(tmp);
		switch(fld->type)
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
				if(rrpt->cur_brk_name!=NULL)
				{
					l=RDAstrlen(rrpt->cur_brk_name);
					QUICKALLOC(fld->data.string_value,fld->dlen,l+1);
					memcpy(fld->data.string_value,rrpt->cur_brk_name,l+1);
					if(fld->dlen>fld->len && fld->len>0)
						fld->data.string_value[fld->len]=0;
				} else if(fld->dlen>0)
				{
					memset(fld->data.string_value,0,fld->dlen);
				} else {
					fld->data.string_value=NULL;
					fld->dlen=0;
				}
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"PAGE NO"))
	{
		switch(fld->type)
		{
			case SHORTV:
			case SSHORTV:
				*fld->data.short_value=rrpt->pageno;
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				*fld->data.integer_value=rrpt->pageno;
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				*fld->data.float_value=rrpt->pageno;
				break;
			case BOOLNS:
			case CHARACTERS:
				*fld->data.string_value=rrpt->pageno;
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"SYSTEM DATE"))
	{
		switch(fld->type)
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
				temp=GETCURRENTDATE();
				if(temp!=NULL)
				{
					l=RDAstrlen(temp);
					QUICKALLOC(fld->data.string_value,fld->dlen,l+1);
					memcpy(fld->data.string_value,temp,l+1);
					if(fld->dlen>fld->len && fld->len>0)
						fld->data.string_value[fld->len]=0;
				} else if(fld->dlen>0)
				{
					memset(fld->data.string_value,0,fld->dlen);
				} else {
					fld->data.string_value=NULL;
					fld->dlen=0;
				}
				if(temp!=NULL) Rfree(temp);
				temp=NULL;
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"SYSTEM DATE-YYYY"))
	{
		switch(fld->type)
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
				temp=GETCURRENTDATE10();
				if(temp!=NULL)
				{
					l=RDAstrlen(temp);
					QUICKALLOC(fld->data.string_value,fld->dlen,l+1);
					memcpy(fld->data.string_value,temp,l+1);
					if(fld->dlen>fld->len && fld->len>0)
						fld->data.string_value[fld->len]=0;
				} else if(fld->dlen>0)
				{
					memset(fld->data.string_value,0,fld->dlen);
				} else {
					fld->data.string_value=NULL;
					fld->dlen=0;
				}
				if(temp!=NULL) Rfree(temp);
				temp=NULL;
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"SYSTEM TIME"))
	{
		switch(fld->type)
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
				temp=GETCURRENTTIME();
				if(temp!=NULL)
				{
					l=RDAstrlen(temp);
					QUICKALLOC(fld->data.string_value,fld->dlen,l+1);
					memcpy(fld->data.string_value,temp,l+1);
					if(fld->dlen>fld->len && fld->len>0)
						fld->data.string_value[fld->len]=0;
				} else if(fld->dlen>0)
				{
					memset(fld->data.string_value,0,fld->dlen);
				} else {
					fld->data.string_value=NULL;
					fld->dlen=0;
				}
				if(temp!=NULL) Rfree(temp);
				temp=NULL;
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"CURRENT DIRECTORY"))
	{
		switch(fld->type)
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
				temp=stralloc(CURRENTDIRECTORY);
				if(temp!=NULL)
				{
					l=RDAstrlen(temp);
					QUICKALLOC(fld->data.string_value,fld->dlen,l+1);
					memcpy(fld->data.string_value,temp,l+1);
					if(fld->dlen>fld->len && fld->len>0)
						fld->data.string_value[fld->len]=0;
				} else if(fld->dlen>0)
				{
					memset(fld->data.string_value,0,fld->dlen);
				} else {
					fld->data.string_value=NULL;
					fld->dlen=0;
				}
				if(temp!=NULL) Rfree(temp);
				temp=NULL;
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"SYSTEM TYPE"))
	{
		switch(fld->type)
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
#else
				temp=stralloc("UNKNOWN");
#endif
				if(temp!=NULL)
				{
					l=RDAstrlen(temp);
					QUICKALLOC(fld->data.string_value,fld->dlen,l+1);
					memcpy(fld->data.string_value,temp,l+1);
					if(fld->dlen>fld->len && fld->len>0)
						fld->data.string_value[fld->len]=0;
				} else if(fld->dlen>0)
				{
					memset(fld->data.string_value,0,fld->dlen);
				} else {
					fld->data.string_value=NULL;
					fld->dlen=0;
				}
				if(temp!=NULL) Rfree(temp);
				temp=NULL;
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"ACCOUNT MANAGER"))
	{
		switch(fld->type)
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
				temp=RDA_ReturnAccountManager();
				if(temp!=NULL)
				{
					l=RDAstrlen(temp);
					QUICKALLOC(fld->data.string_value,fld->dlen,l+1);
					memcpy(fld->data.string_value,temp,l+1);
					if(fld->dlen>fld->len && fld->len>0)
						fld->data.string_value[fld->len]=0;
				} else if(fld->dlen>0)
				{
					memset(fld->data.string_value,0,fld->dlen);
				} else {
					fld->data.string_value=NULL;
					fld->dlen=0;
				}
				if(temp!=NULL) Rfree(temp);
				temp=NULL;
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"ACCOUNT GRANT IDENTIFICATION"))
	{
		switch(fld->type)
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
				temp=RDA_ReturnAccountGrantIdentification();
				if(temp!=NULL)
				{
					l=RDAstrlen(temp);
					QUICKALLOC(fld->data.string_value,fld->dlen,l+1);
					memcpy(fld->data.string_value,temp,l+1);
					if(fld->dlen>fld->len && fld->len>0)
						fld->data.string_value[fld->len]=0;
				} else if(fld->dlen>0)
				{
					memset(fld->data.string_value,0,fld->dlen);
				} else {
					fld->data.string_value=NULL;
					fld->dlen=0;
				}
				if(temp!=NULL) Rfree(temp);
				temp=NULL;
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"ACCOUNT REFERENCE TYPE"))
	{
		switch(fld->type)
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
				temp=RDA_ReturnAccountReferenceType();
				if(temp!=NULL)
				{
					l=RDAstrlen(temp);
					QUICKALLOC(fld->data.string_value,fld->dlen,l+1);
					memcpy(fld->data.string_value,temp,l+1);
					if(fld->dlen>fld->len && fld->len>0)
						fld->data.string_value[fld->len]=0;
				} else if(fld->dlen>0)
				{
					memset(fld->data.string_value,0,fld->dlen);
				} else {
					fld->data.string_value=NULL;
					fld->dlen=0;
				}
				if(temp!=NULL) Rfree(temp);
				temp=NULL;
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"LOGIN IDENTIFICATION"))
	{
		switch(fld->type)
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
				if(USERLOGIN!=NULL)
				{
					l=RDAstrlen(USERLOGIN);
					QUICKALLOC(fld->data.string_value,fld->dlen,l+1);
					memcpy(fld->data.string_value,USERLOGIN,l+1);
					if(fld->dlen>fld->len && fld->len>0)
						fld->data.string_value[fld->len]=0;
				} else if(fld->dlen>0)
				{
					memset(fld->data.string_value,0,fld->dlen);
				} else {
					fld->data.string_value=NULL;
					fld->dlen=0;
				}
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"TOTAL COUNTER"))
	{
		switch(fld->type)
		{
			case SHORTV:
			case SSHORTV:
				*fld->data.short_value=rrpt->total_count;
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				*fld->data.integer_value=rrpt->total_count;
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				*fld->data.float_value=rrpt->total_count;
				break;
			case BOOLNS:
			case CHARACTERS:
				*fld->data.string_value=rrpt->total_count;
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"CURRENT FONT PITCH"))
	{
		switch(fld->type)
		{
			case SHORTV:
			case SSHORTV:
				*fld->data.short_value=rrpt->curpitch;
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				*fld->data.integer_value=rrpt->curpitch;
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				*fld->data.float_value=rrpt->curpitch;
				break;
			case BOOLNS:
			case CHARACTERS:
				*fld->data.string_value=rrpt->curpitch;
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"CURRENT FONT POINTS"))
	{
		switch(fld->type)
		{
			case SHORTV:
			case SSHORTV:
				*fld->data.short_value=rrpt->curpoints;
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				*fld->data.integer_value=rrpt->curpoints;
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				*fld->data.float_value=rrpt->curpoints;
				break;
			case BOOLNS:
			case CHARACTERS:
				*fld->data.string_value=rrpt->curpoints;
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"FONT PITCH"))
	{
		switch(fld->type)
		{
			case SHORTV:
			case SSHORTV:
				*fld->data.short_value=rrpt->pitch;
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				*fld->data.integer_value=rrpt->pitch;
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				*fld->data.float_value=rrpt->pitch;
				break;
			case BOOLNS:
			case CHARACTERS:
				*fld->data.string_value=rrpt->pitch;
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"FONT POINTS"))
	{
		switch(fld->type)
		{
			case SHORTV:
			case SSHORTV:
				*fld->data.short_value=rrpt->points;
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				*fld->data.integer_value=rrpt->points;
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				*fld->data.float_value=rrpt->points;
				break;
			case BOOLNS:
			case CHARACTERS:
				*fld->data.string_value=rrpt->points;
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"CURRENT FONT"))
	{
		if(tmp!=NULL) Rfree(tmp);
		switch(fld->type)
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
				if(rrpt->curfont!=NULL)
				{
					l=RDAstrlen(rrpt->curfont);
					QUICKALLOC(fld->data.string_value,fld->dlen,l+1);
					memcpy(fld->data.string_value,rrpt->curfont,l+1);
					if(fld->dlen>fld->len && fld->len>0)
						fld->data.string_value[fld->len]=0;
				} else if(fld->dlen>0)
				{
					memset(fld->data.string_value,0,fld->dlen);
				} else {
					fld->data.string_value=NULL;
					fld->dlen=0;
				}
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else if(!RDAstrcmp(modulename,"LONGEST FONT"))
	{
		if(tmp!=NULL) Rfree(tmp);
		switch(fld->type)
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
				if(rrpt->longfont!=NULL)
				{
					l=RDAstrlen(rrpt->longfont);
					QUICKALLOC(fld->data.string_value,fld->dlen,l+1);
					memcpy(fld->data.string_value,rrpt->longfont,l+1);
					if(fld->dlen>fld->len && fld->len>0)
						fld->data.string_value[fld->len]=0;
				} else if(fld->dlen>0)
				{
					memset(fld->data.string_value,0,fld->dlen);
				} else {
					fld->data.string_value=NULL;
					fld->dlen=0;
				}
				break;
			default:
				prterr("Error cannot copy field [%s].",fld->name);
				break;
		}
	} else { 
	/* accumulators */
		for(y=0,a=rpt->accum;y<rpt->numaccums;++y,++a)
		{
			if(!RDAstrcmp(modulename,a->name)) break;
		}
		if(y<rpt->numaccums)
		{
			accval=a->accvals+rpt->currbrk;
			switch(fld->type)
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
					switch(a->fieldtype)
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
							switch(a->atype)
							{
								case 2: /* FIRST OCCURRENCE */
								case 3: /* LAST OCCURRENCE */
								case 6: /* PREVIOUS OCCURRENCE */
									if(!isEMPTY(accval->prev_value.string_value))
									{
										QUICKALLOC(fld->data.string_value,fld->dlen,accval->pdlen);
										memcpy(fld->data.string_value,accval->prev_value.string_value,accval->pdlen);
										if(fld->dlen>fld->len && fld->len>0)
											fld->data.string_value[fld->len]=0;
									} else if(fld->dlen>0)
									{
										memset(fld->data.string_value,0,fld->dlen);
									} else {
										fld->data.string_value=NULL;
										fld->dlen=0;
									}
									break;
								case 0: /* AVERAGE */
								case 4: /* MAXIMUM */
								case 5: /* MINIMUM */
								case 7: /* SUMMATION */
									if(!isEMPTY(accval->value.string_value))
									{
										QUICKALLOC(fld->data.string_value,fld->dlen,accval->dlen);
										memcpy(fld->data.string_value,accval->value.string_value,accval->dlen);
										if(fld->dlen>fld->len && fld->len>0)
											fld->data.string_value[fld->len]=0;
									} else if(fld->dlen>0)
									{
										memset(fld->data.string_value,0,fld->dlen);
									} else {
										fld->data.string_value=NULL;
										fld->dlen=0;
									}
									break;
								case 1: /* FREQUENCY */
									*fld->data.float_value=*accval->value.float_value;
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						default:
							prterr("Error cannot copy accumulator [%s] type [%d] [%s] to field [%s].",a->name,a->fieldtype,standardfieldtypes[a->fieldtype],fld->name); 
							break;
					}
					break;
				case SHORTV:
				case SSHORTV:
					switch(a->fieldtype)
					{
						case SHORTV:
						case SSHORTV:
						case SLONGV:
						case SCROLLEDLIST:
						case LONGV:
						case SDOUBLEV:
						case SDECIMALV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case CHARACTERS:
						case BOOLNS:
							switch(a->atype)
							{
								case 2: /* FIRST OCCURRENCE */
								case 3: /* LAST OCCURRENCE */
								case 6: /* PREVIOUS OCCURRENCE */
									*fld->data.short_value=(short)*accval->prev_value.float_value;
									break;
								case 0: /* AVERAGE */
								case 4: /* MAXIMUM */
								case 5: /* MINIMUM */
								case 7: /* SUMMATION */
								case 1: /* FREQUENCY */
									*fld->data.short_value=(short)*accval->value.float_value;
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						default:
							prterr("Error cannot copy accumulator [%s] type [%d] [%s] to field [%s].",a->name,a->fieldtype,standardfieldtypes[a->fieldtype],fld->name); 
							break;
					}
					break;
				case SLONGV:
				case SCROLLEDLIST:
				case LONGV:
					switch(a->fieldtype)
					{
						case SHORTV:
						case SSHORTV:
						case SLONGV:
						case SCROLLEDLIST:
						case LONGV:
						case SDOUBLEV:
						case SDECIMALV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case CHARACTERS:
						case BOOLNS:
							switch(a->atype)
							{
								case 2: /* FIRST OCCURRENCE */
								case 3: /* LAST OCCURRENCE */
								case 6: /* PREVIOUS OCCURRENCE */
									*fld->data.integer_value=(int)*accval->prev_value.float_value;
									break;
								case 0: /* AVERAGE */
								case 4: /* MAXIMUM */
								case 5: /* MINIMUM */
								case 7: /* SUMMATION */
								case 1: /* FREQUENCY */
									*fld->data.integer_value=(int)*accval->value.float_value;
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						default:
							prterr("Error cannot copy accumulator [%s] type [%d] [%s] to field [%s].",a->name,a->fieldtype,standardfieldtypes[a->fieldtype],fld->name); 
							break;
					}
					break;
				case CHARACTERS:
				case BOOLNS:
					switch(a->fieldtype)
					{
						case SHORTV:
						case SSHORTV:
						case SLONGV:
						case SCROLLEDLIST:
						case LONGV:
						case SDOUBLEV:
						case SDECIMALV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case CHARACTERS:
						case BOOLNS:
							switch(a->atype)
							{
								case 2: /* FIRST OCCURRENCE */
								case 3: /* LAST OCCURRENCE */
								case 6: /* PREVIOUS OCCURRENCE */
									if(*accval->prev_value.float_value)
									{
										*fld->data.string_value=TRUE;
									} else *fld->data.string_value=FALSE;
									break;
								case 0: /* AVERAGE */
								case 4: /* MAXIMUM */
								case 5: /* MINIMUM */
								case 7: /* SUMMATION */
								case 1: /* FREQUENCY */
									*fld->data.string_value=(short)*accval->value.float_value;
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						default:
							prterr("Error cannot copy accumulator [%s] type [%d] [%s] to field [%s].",a->name,a->fieldtype,standardfieldtypes[a->fieldtype],fld->name); 
							break;
					}
					break;
				case SDOUBLEV:
				case SDECIMALV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
					switch(a->fieldtype)
					{
						case SHORTV:
						case SSHORTV:
						case SLONGV:
						case SCROLLEDLIST:
						case LONGV:
						case SDOUBLEV:
						case SDECIMALV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case CHARACTERS:
						case BOOLNS:
							switch(a->atype)
							{
								case 2: /* FIRST OCCURRENCE */
								case 3: /* LAST OCCURRENCE */
								case 6: /* PREVIOUS OCCURRENCE */
									*fld->data.float_value=*accval->prev_value.float_value;
									break;
								case 0: /* AVERAGE */
								case 4: /* MAXIMUM */
								case 5: /* MINIMUM */
								case 7: /* SUMMATION */
								case 1: /* FREQUENCY */
									*fld->data.float_value=*accval->value.float_value;
									break;
								default:
									prterr("Error unrecognized accumulator type.");
									break;
							}
							break;
						default:
							prterr("Error cannot copy accumulator [%s] type [%d] [%s] to field [%s].",a->name,a->fieldtype,standardfieldtypes[a->fieldtype],fld->name); 
							break;
					}
					break;
				default:
					prterr("Error field type %d is invalid for %s.",
						a->fieldtype,a->name); 
					break;
			}
		} else {
	/* virtual field */
			for(y=0,v=rpt->virflds;y<rpt->numvirtuals;++y,++v)
			{
				if(!RDAstrcmp(modulename,v->name)) break;
			}
			if(y<rpt->numvirtuals)
			{
				if(!v->computed) 
				{
					h=HoldReportNew(rrpt,rpt);
					v->computed=TRUE;
					tempvalue=NULL;
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
							if(v->range==2)
							{
								if(rrpt->rsrsrc!=NULL)
								{
								FINDRSCGETSTRING(rrpt->rsrsrc,v->name,
									&v->value.string_value);
								}
							} else if(v->range==3)
							{
								dashname=adddashes(v->name);
								tempvalue=RDA_GetEnv(dashname);
								if(dashname!=NULL) Rfree(dashname);
								if(!isEMPTY(tempvalue))
								{
									if(v->value.string_value!=NULL) Rfree(v->value.string_value);
									v->value.string_value=stralloc(tempvalue);
									v->dlen=RDAstrlen(v->value.string_value)+1;
									
								} else if(v->value.string_value!=NULL)
								{
									memset(v->value.string_value,0,v->dlen);
								} else {
									v->value.string_value=NULL;
									v->dlen=0;
								}
							}
							if(!isEMPTY(v->expression))
							{
								temp=PP_EVALUATEstr(v->expression,
									ReportGenSubData1,h);
								if(v->value.string_value!=NULL) 
									Rfree(v->value.string_value);
								v->value.string_value=stralloc(temp);
								if(temp!=NULL) Rfree(temp);
							}
							if(v->value.string_value!=NULL)
							{
								v->dlen=RDAstrlen(v->value.string_value)+1;
								if(v->dlen>v->len && v->len>0)
								{
									v->value.string_value[v->len]=0;
								}
							} else v->dlen=0;
							break;
						case SHORTV:
						case SSHORTV:
							if(v->range==2)
							{
								if(rrpt->rsrsrc!=NULL)
								{
								FINDRSCGETSHORT(rrpt->rsrsrc,v->name,
									v->value.short_value);
								}
							} else if(v->range==3)
							{
								dashname=adddashes(v->name);
								tempvalue=RDA_GetEnv(dashname);
								if(dashname!=NULL) Rfree(dashname);
								if(v->dlen==0)
								{
									v->value.short_value=Rmalloc(sizeof(short));
									v->dlen=sizeof(short);
								}
								if(!isEMPTY(tempvalue))
								{
									*v->value.short_value=Ratoi(tempvalue);
								} else {
									*v->value.short_value=0;
								}
							}
							if(!isEMPTY(v->expression))
							{
								*v->value.short_value=PP_EVALUATEsht(v->expression,
									ReportGenSubData1,h);
							}
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							if(v->range==2)
							{
								if(rrpt->rsrsrc!=NULL)
								{
								FINDRSCGETINT(rrpt->rsrsrc,v->name,
									v->value.integer_value);
								}
							} else if(v->range==3)
							{
								dashname=adddashes(v->name);
								tempvalue=RDA_GetEnv(dashname);
								if(dashname!=NULL) Rfree(dashname);
								if(v->dlen==0)
								{
									v->value.integer_value=Rmalloc(sizeof(int));
									v->dlen=sizeof(int);
								}
								if(!isEMPTY(tempvalue))
								{
									*v->value.integer_value=Ratoi(tempvalue);
								} else {
									*v->value.integer_value=0;
								}
							}
							if(!isEMPTY(v->expression))
							{
								*v->value.integer_value=PP_EVALUATEint(v->expression,ReportGenSubData1,h);
							}
							break;
						case BOOLNS:
							if(v->range==2)
							{
								if(rrpt->rsrsrc!=NULL)
								{
								FINDRSCGETCHAR(rrpt->rsrsrc,v->name,
									v->value.string_value);
								}
							} else if(v->range==3)
							{
								dashname=adddashes(v->name);
								tempvalue=RDA_GetEnv(dashname);
								if(dashname!=NULL) Rfree(dashname);
								if(v->dlen==0)
								{
									v->value.string_value=Rmalloc(1);
									v->dlen=1;
								}
								if(!isEMPTY(tempvalue))
								{
									if(Ratoi(tempvalue))
									{
										*v->value.string_value=TRUE;
									} else {
										*v->value.string_value=FALSE;
									}
								} else {
									*v->value.string_value=FALSE;
								}
							}
							if(!isEMPTY(v->expression))
							{
								*v->value.string_value=PP_EVALUATEbol(v->expression,ReportGenSubData1,h);
							}
							break;
						case CHARACTERS:
							if(v->range==2)
							{
								if(rrpt->rsrsrc!=NULL)
								{
								FINDRSCGETCHAR(rrpt->rsrsrc,v->name,
									v->value.string_value);
								}
							} else if(v->range==3)
							{
								dashname=adddashes(v->name);
								tempvalue=RDA_GetEnv(dashname);
								if(dashname!=NULL) Rfree(dashname);
								if(v->dlen==0)
								{
									v->value.string_value=Rmalloc(1);
									v->dlen=1;
								}
								if(!isEMPTY(tempvalue))
								{
									*v->value.string_value=*tempvalue;
								} else {
									*v->value.string_value=*tempchar;
								}
							}
							if(!isEMPTY(v->expression))
							{
								*v->value.string_value=PP_EVALUATEbol(v->expression,ReportGenSubData1,h);
							}
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							if(v->range==2)
							{
								if(rrpt->rsrsrc!=NULL)
								{
								FINDRSCGETDOUBLE(rrpt->rsrsrc,v->name,
									v->value.float_value);
								}
							} else if(v->range==3)
							{
								dashname=adddashes(v->name);
								tempvalue=RDA_GetEnv(dashname);
								if(dashname!=NULL) Rfree(dashname);
								if(v->dlen==0)
								{
									v->value.float_value=Rmalloc(sizeof(double));
									v->dlen=sizeof(double);
								}
								if(!isEMPTY(tempvalue))
								{
									*v->value.float_value=atof(tempvalue);
								} else {
									*v->value.float_value=0;
								}
							}
							if(!isEMPTY(v->expression))
							{
								*v->value.float_value=PP_EVALUATEdbl(v->expression,ReportGenSubData1,h);
								if(v->type==DOLLARS || v->type==DOUBLEV ||
									v->type==SDOUBLEV ||
									v->type==DOLLARS_NOCENTS)
								{
									*v->value.float_value=round(*v->value.float_value);
								}
							}
							break;
						default:
							prterr("Error unrecognized field type.");
							break;
					}
					if(h!=NULL) Rfree(h);
					h=NULL;
				}
				switch(fld->type)
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
								if(!isEMPTY(v->value.string_value))
								{
									QUICKALLOC(fld->data.string_value,fld->dlen,v->dlen);
									memcpy(fld->data.string_value,v->value.string_value,v->dlen);
									if(fld->dlen>fld->len && fld->len>0)
										fld->data.string_value[fld->len]=0;
								} else if(fld->dlen>0)
								{
									memset(fld->data.string_value,0,fld->dlen);
								} else {
									fld->data.string_value=NULL;
									fld->dlen=0;
								}
								break;
							default:
								prterr("Error cannot copy field.");
								break;
						}
						break;
					case SHORTV:
					case SSHORTV:
						switch(v->type)
						{
							case SHORTV:
							case SSHORTV:
								*fld->data.short_value=
									*v->value.short_value;
								break;
							case SLONGV:
							case SCROLLEDLIST:
							case LONGV:
								*fld->data.short_value=
									(short)*v->value.integer_value;
								break;
							case SDOUBLEV:
							case SDECIMALV:
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
								*fld->data.short_value=
									(short)*v->value.float_value;
								break;
							case CHARACTERS:
							case BOOLNS:
								*fld->data.short_value=
									*v->value.string_value;
								break;
							default:
								prterr("Error cannot copy field.");
								break;
						}
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						switch(v->type)
						{
							case SHORTV:
							case SSHORTV:
								*fld->data.integer_value=
									*v->value.short_value;
								break;
							case SCROLLEDLIST:
							case LONGV:
							case SLONGV:
								*fld->data.integer_value=
									*v->value.integer_value;
								break;
							case SDOUBLEV:
							case SDECIMALV:
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
								*fld->data.integer_value=
									(int)*v->value.float_value;
								break;
							case CHARACTERS:
							case BOOLNS:
								*fld->data.integer_value=
									*v->value.string_value;
								break;
							default:
								prterr("Error cannot copy field.");
								break;
						}
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						switch(v->type)
						{
							case SHORTV:
							case SSHORTV:
								*fld->data.float_value=
									*v->value.short_value;
								break;
							case SLONGV:
							case SCROLLEDLIST:
							case LONGV:
								*fld->data.float_value=
									*v->value.integer_value;
								break;
							case SDOUBLEV:
							case SDECIMALV:
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
								*fld->data.float_value=
									*v->value.float_value;
								break;
							case CHARACTERS:
							case BOOLNS:
								*fld->data.float_value=
									*v->value.string_value;
								break;
							default:
								prterr("Error cannot copy field.");
								break;
						}
						break;
					case BOOLNS:
					case CHARACTERS:
						switch(v->type)
						{
							case SHORTV:
							case SSHORTV:
								*fld->data.string_value=
									*v->value.short_value;
								break;
							case SLONGV:
							case SCROLLEDLIST:
							case LONGV:
								*fld->data.string_value=
									*v->value.integer_value;
								break;
							case SDOUBLEV:
							case SDECIMALV:
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DECIMALV:
							case DOUBLEV:
								*fld->data.string_value=
									(short)*v->value.float_value;
								break;
							case CHARACTERS:
							case BOOLNS:
								*fld->data.string_value=
									*v->value.string_value;
								break;
							default:
								prterr("Error cannot copy field.");
								break;
						}
						break;
					default:
						prterr("Error field type %d is invalid for %s.",v->type,v->name); 
						break;
				}
			} else {
				prterr("Error Field [%s] not found.",tmp);
			}
		}
	}
	if(modulename!=NULL) Rfree(modulename);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG SetFieldValue1 Returning Field [%s].",fld->name);
		SEENRDFIELD(fld); 
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
}
