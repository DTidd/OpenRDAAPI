#include <rptgen.hpp>

void ReportGenSubData(char **tmp,HoldReport *h)
{
	char *modulename=NULL,*filename=NULL,*fieldname=NULL;
	NRDfield *field;
	short start=1,length=0,fileno=(-1),file_occurrance=1;
	int str_length=0;
	char *temp=NULL,*gn=NULL,doall=TRUE;
	char *dashname=NULL,*tempvalue=NULL;
	char *tempchar=" ";
	int y=0;
	RDAvirtual *v=NULL;
	RDAreport *rpt=NULL;
	RDArunrpt *rrpt=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG ReportGenSubData Before *tmp [%s].",*tmp);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	rpt=h->rpt;
	rrpt=h->rrpt;
	modulename=stripmodulename(*tmp);
	if(modulename!=NULL)
	{
	memset(stemp,0,101);
	filename=stripfilename(*tmp);
	fieldname=stripfieldname(*tmp);
	if(fieldname!=NULL)
	{
		if(RDAstrlen(*tmp)>(RDAstrlen(modulename)+RDAstrlen(filename)+
			RDAstrlen(fieldname)+6))
		{
			temp=*tmp+(RDAstrlen(modulename)+RDAstrlen(filename)+RDAstrlen(fieldname)+7);
			gn=strtok(temp,",]");
			if(gn!=NULL)
			{
				if(!isEMPTY(gn))
				{
					file_occurrance=Ratoi(gn);
				} else file_occurrance=1;
				gn=strtok(NULL,",]");
				if(!isEMPTY(gn))
				{
					start=Ratoi(gn);
				} else start=1;
				gn=strtok(NULL,"]");
				if(!isEMPTY(gn))
				{
					length=Ratoi(gn);
				} else length=0;
				if(file_occurrance<1) file_occurrance=1;
			}
			if(start<1) start=1;
			doall=FALSE;
		}
		if(DO_SECURITY_FILENO!=(-1))
		{
			if(!RDAstrcmp(modulename,MODULENAME(DO_SECURITY_FILENO)) &&
				!RDAstrcmp(filename,FILENAME(DO_SECURITY_FILENO)))
			{
				fileno=DO_SECURITY_FILENO;
			} else fileno=REPORTFILENUMBER(rrpt,modulename,filename,file_occurrance,FALSE);
		} else fileno=REPORTFILENUMBER(rrpt,modulename,filename,file_occurrance,FALSE);
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
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
						if(temp!=NULL) Rfree(temp);
						temp=NULL;
					} else if((start-1)<str_length && !isEMPTY(field->data.string_value))
					{
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"%s\"",&field->data.string_value[start-1]);
					} else {
						memset(*tmp,0,RDAstrlen(*tmp)+1);
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
					if(RDAstrlen(*tmp)<str_length)
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
					if(RDAstrlen(*tmp)<str_length)
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
					prterr("Error field type [%d] is invalid for field [%s].",field->type,field->name); 
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=stralloc("\"\"");
					break;
			}
		} else {
#ifdef USE_RDA_DIAGNOSTICS
			if(diagrptgen)
			{
				prterr("Error Field [%s] not Found in File [%s] [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno)); 
			}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		}
		if(fieldname!=NULL) Rfree(fieldname);
		if(filename!=NULL) Rfree(filename);
	} else if(!RDAstrcmp(modulename,"WRITE COUNT"))
	{
		sprintf(stemp,"%d",rrpt->write_count);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		memcpy(*tmp,stemp,str_length+1);
	} else if(!RDAstrcmp(modulename,"PRIMARY FILE NUMBER"))
	{
		sprintf(stemp,"%d",rrpt->primary_file_no);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		memcpy(*tmp,stemp,str_length+1);
	} else if(!RDAstrcmp(modulename,"BODY COUNT"))
	{
		sprintf(stemp,"%d",rrpt->body_count);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		memcpy(*tmp,stemp,str_length+1);
	} else if(!RDAstrcmp(modulename,"PAGE COUNTER"))
	{
		sprintf(stemp,"%d",rrpt->page_count);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		memcpy(*tmp,stemp,str_length+1);
	} else if(!RDAstrcmp(modulename,"NUMBER OF LINES"))
	{
		sprintf(stemp,"%d",rrpt->numlines);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		memcpy(*tmp,stemp,str_length+1);
	} else if(!RDAstrcmp(modulename,"BODY COUNT LIMIT"))
	{
		sprintf(stemp,"%d",rpt->display->body_count);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		memcpy(*tmp,stemp,str_length+1);
	} else if(!RDAstrcmp(modulename,"BODY COUNT TYPE"))
	{
		sprintf(stemp,"%d",rpt->display->body_count_type);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		memcpy(*tmp,stemp,str_length+1);
	} else if(!RDAstrcmp(modulename,"PAGE LENGTH LIMIT"))
	{
		sprintf(stemp,"(%f)",rpt->display->page_length);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		memcpy(*tmp,stemp,str_length+1);
	} else if(!RDAstrcmp(modulename,"PAGE WIDTH LIMIT"))
	{
		sprintf(stemp,"(%f)",rpt->display->page_width);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		memcpy(*tmp,stemp,str_length+1);
	} else if(!RDAstrcmp(modulename,"STARTING PAGE NO"))
	{
		sprintf(stemp,"%d",rpt->display->starting_page);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		memcpy(*tmp,stemp,str_length+1);
	} else if(!RDAstrcmp(modulename,"DEVICE NAME"))
	{
		if(RDAstrlen(*tmp)<(RDAstrlen(rpt->display->set_lpp)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(rpt->display->set_lpp)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"\"%s\"",(rpt->display->set_lpp!=NULL ? 
			rpt->display->set_lpp:""));
	} else if(!RDAstrcmp(modulename,"BREAK LEVEL"))
	{
		if(RDAstrlen(*tmp)<(RDAstrlen(rrpt->cur_brk_name)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(rrpt->cur_brk_name)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"\"%s\"",(rrpt->cur_brk_name!=NULL ? 
			rrpt->cur_brk_name:""));
	} else if(!RDAstrcmp(modulename,"PREVIOUS BREAK LEVEL 2"))
	{
		if(RDAstrlen(*tmp)<(RDAstrlen(rrpt->prev_brk_name2)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(rrpt->prev_brk_name2)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"\"%s\"",(rrpt->prev_brk_name2!=NULL ? 
			rrpt->prev_brk_name2:""));
	} else if(!RDAstrcmp(modulename,"PREVIOUS BREAK LEVEL"))
	{
		if(RDAstrlen(*tmp)<(RDAstrlen(rrpt->prev_brk_name)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(rrpt->prev_brk_name)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"\"%s\"",(rrpt->prev_brk_name!=NULL ? 
			rrpt->prev_brk_name:""));
	} else if(!RDAstrcmp(modulename,"PAGE NO"))
	{
		sprintf(stemp,"%d",rrpt->pageno);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		memcpy(*tmp,stemp,str_length+1);
	} else if(!RDAstrcmp(modulename,"ERRORS ENCOUNTERED"))
	{
		sprintf(stemp,"%ld",rrpt->errors_encountered);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		memcpy(*tmp,stemp,str_length+1);
	} else if(!RDAstrcmp(modulename,"RECORD COUNTER"))
	{
		sprintf(stemp,"%ld",rrpt->record_count);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		memcpy(*tmp,stemp,str_length+1);
	} else if(!RDAstrcmp(modulename,"TOTAL COUNTER"))
	{
		sprintf(stemp,"%ld",rrpt->total_count);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		memcpy(*tmp,stemp,str_length+1);
	} else if(!RDAstrcmp(modulename,"CURRENT FONT PITCH"))
	{
		sprintf(stemp,"%d",rrpt->curpitch);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		memcpy(*tmp,stemp,str_length+1);
	} else if(!RDAstrcmp(modulename,"CURRENT FONT POINTS"))
	{
		sprintf(stemp,"%d",rrpt->curpoints);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		memcpy(*tmp,stemp,str_length+1);
	} else if(!RDAstrcmp(modulename,"FONT PITCH"))
	{
		sprintf(stemp,"%d",rrpt->pitch);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		memcpy(*tmp,stemp,str_length+1);
	} else if(!RDAstrcmp(modulename,"FONT POINTS"))
	{
		sprintf(stemp,"%d",rrpt->points);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		memcpy(*tmp,stemp,str_length+1);
	} else if(!RDAstrcmp(modulename,"CURRENT FONT"))
	{
		if(RDAstrlen(*tmp)<(RDAstrlen(rrpt->curfont)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(rrpt->curfont)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"\"%s\"",(rrpt->curfont !=NULL ? 
			rrpt->curfont:""));
	} else if(!RDAstrcmp(modulename,"LONGEST FONT"))
	{
		if(RDAstrlen(*tmp)<(RDAstrlen(rrpt->longfont)+2))
		{
			*tmp=Rrealloc(*tmp,RDAstrlen(rrpt->longfont)+3);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		sprintf(*tmp,"\"%s\"",(rrpt->longfont !=NULL ? 
			rrpt->longfont:""));
	} else if(!RDAstrcmp(modulename,"SUBORDINATE WRITE COUNT"))
	{
		sprintf(stemp,"%d",rrpt->cur_runfile->write_count);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		memcpy(*tmp,stemp,str_length+1);
	} else if(!RDAstrcmp(modulename,"DETAIL COUNT"))
	{
		sprintf(stemp,"%d",rrpt->detail_count);
		str_length=RDAstrlen(stemp);
		if(RDAstrlen(*tmp)<str_length)
		{
			*tmp=Rrealloc(*tmp,str_length+1);
		}
		memset(*tmp,0,RDAstrlen(*tmp)+1);
		memcpy(*tmp,stemp,str_length+1);
	} else if(GLOBALSubData(tmp,modulename)==0)
	{
/* OK, found in Global Virtuals */
	} else if(rpt->virflds!=NULL)
	{
		/* virtual field */
		for(y=0,v=rpt->virflds;y<rpt->numvirtuals;++y,++v)
		{
			if(!RDAstrcmp(modulename,v->name)) break;
		}
		if(y<rpt->numvirtuals)
		{
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
					if(!v->computed) 
					{
						v->computed=TRUE;
						if((v->range==2 || v->range==4) && !v->when)
						{
							if(rrpt->rsrsrc!=NULL)
							{
								FINDRSCGETSTRING(rrpt->rsrsrc,v->name,&v->value.string_value);
							}
						} else if(v->range==3 && !v->when)
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
						if(v->expression!=NULL)
						{
							temp=PP_EVALUATEstr(v->expression,ReportGenSubData,h);
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
					str_length=(v->value.string_value!=NULL ?
						RDAstrlen(v->value.string_value):0);
					if(length==0) length=str_length;
					if(RDAstrlen(*tmp)<(length+3))
					{
						*tmp=Rrealloc(*tmp,length+3);
					}
					if(doall)
					{
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"%s\"",(v->value.string_value!=NULL ? v->value.string_value:""));
					} else if(!isEMPTY(v->value.string_value) && (start-1)<str_length && str_length>length)
					{
						temp=stralloc(v->value.string_value);
						gn=temp+(start-1);
						if(RDAstrlen(gn)>(length))
							temp[start-1+length]=0;
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"%s\"",&temp[start-1]);
						if(temp!=NULL) Rfree(temp);
						temp=NULL;
					} else if((start-1)<str_length && !isEMPTY(v->value.string_value))
					{
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"%s\"",&v->value.string_value[start-1]);
						temp=NULL;
					} else {
						memset(*tmp,0,RDAstrlen(*tmp)+1);
						sprintf(*tmp,"\"\"");
					}
					break;
				case SHORTV:
				case SSHORTV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						if((v->range==2 || v->range==4) && !v->when)
						{
							if(rrpt->rsrsrc!=NULL)
							{
								FINDRSCGETSHORT(rrpt->rsrsrc,v->name,v->value.short_value);
							}
						} else if(v->range==3 && !v->when)
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
						if(v->range!=4)
						{
						if(v->expression!=NULL)
						{
						*v->value.short_value=PP_EVALUATEsht(v->expression,ReportGenSubData,h);
						}
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
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					if(!v->computed) 
					{
						v->computed=TRUE;
						if((v->range==2 || v->range==4) && !v->when)
						{
							if(rrpt->rsrsrc!=NULL)
							{
								FINDRSCGETINT(rrpt->rsrsrc,v->name,v->value.integer_value);
							}
						} else if(v->range==3 && !v->when)
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
						if(v->range!=4)
						{
						if(v->expression!=NULL)
						{
						*v->value.integer_value=PP_EVALUATEint(v->expression,ReportGenSubData,h);
						}
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
						if((v->range==2 || v->range==4) && !v->when)
						{
							if(rrpt->rsrsrc!=NULL)
							{
								FINDRSCGETDOUBLE(rrpt->rsrsrc,v->name,v->value.float_value);
							}
						} else if(v->range==3 && !v->when)
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
						if(v->range!=4)
						{
						if(v->expression!=NULL)
						{
						*v->value.float_value=PP_EVALUATEdbl(v->expression,ReportGenSubData,h);
						if(v->type==DOLLARS || v->type==DOLLARS_NOCENTS ||
							v->type==DOUBLEV ||
							v->type==SDOUBLEV)
						{
							*v->value.float_value=round(*v->value.float_value);
						}
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
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					memcpy(*tmp,stemp,str_length+1);
					break;
				case BOOLNS:
					if(!v->computed) 
					{
						v->computed=TRUE;
						if((v->range==2 || v->range==4) && !v->when)
						{
							if(rrpt->rsrsrc!=NULL)
							{
								FINDRSCGETCHAR(rrpt->rsrsrc,v->name,v->value.string_value);
							}
						} else if(v->range==3 && !v->when)
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
						if(v->range!=4)
						{
						if(v->expression!=NULL)
						{
						*v->value.string_value=PP_EVALUATEbol(v->expression,ReportGenSubData,h);
						}
						}
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
						if((v->range==2 || v->range==4) && !v->when)
						{
							if(rrpt->rsrsrc!=NULL)
							{
							FINDRSCGETCHAR(rrpt->rsrsrc,v->name,v->value.string_value);
							}
						} else if(v->range==3 && !v->when)
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
						if(v->range!=4)
						{
						if(v->expression!=NULL)
						{
						*v->value.string_value=PP_EVALUATEbol(v->expression,ReportGenSubData,h);
						}
						}
					}
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"%c",*v->value.string_value);
					break;
				default:
					prterr("Error field type [%d] is invalid for virtual field [%s].",v->type,v->name); 
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=NULL;
					break;
			}
/*
			if(value!=NULL) Rfree(value);
*/
		} else {
			if(ScrolledListSubData(tmp,rrpt->rsrsrc)==(-1))
			{
				if(SCRNvirtualVIRTUALSubData(tmp,rrpt->rsrsrc,modulename)!=0)
				{
					prterr("Error Field [%s] not found.",*tmp);
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=stralloc("\"\"");
				}
			}
		}
	} else {
		if(ScrolledListSubData(tmp,rrpt->rsrsrc)==(-1))
		{
			if(SCRNvirtualVIRTUALSubData(tmp,rrpt->rsrsrc,modulename)!=0)
			{
				prterr("Error Field [%s] not found.",*tmp);
				if(*tmp!=NULL) Rfree(*tmp);
				*tmp=stralloc("\"\"");
			}
		}
	}
	Rfree(modulename);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagrptgen)
	{
		prterr("DIAG ReportGenSubData After *tmp [%s].",*tmp);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
}
