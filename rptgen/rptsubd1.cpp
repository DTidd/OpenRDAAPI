#include<rptgen.hpp>
#include<mix.hpp>

short VIRTUALMTNReportGenSubData1(char **tmp,HoldReport *h,char *modulename)
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
                prterr("DIAG VIRTUALMTNReportGenSubData1(): Substituting for [%s].",*tmp);
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
					if(RDAstrlen(*tmp)<2)
					{
						*tmp=Rrealloc(*tmp,2);
					}
					sprintf(*tmp,"%d",(*v->value.string_value ? 1:0));
					break;
				case CHARACTERS:
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
static short RPTMTNExecuteSubData(short (*func)(...),char **tmp,RDArsrc *r,HoldReport *h,char *modulename)
{
	short retval=FALSE;
	
/*lint -e746*/
	if(h!=NULL)
	{
		retval=func(tmp,h,modulename);
	} else if(r!=NULL)
	{
		retval=func(tmp,r,modulename);
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
			prterr("DIAG RPTMTNExecuteSubData Returning [%s].",*tmp);
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	}
	return(retval);
}
void RPTMTNSubData(char **tmp,MTNPassableStruct *PSTRUCT)
{
	MaintainMaster *MTNMASTER=NULL;
	char *modulename=NULL;
	HoldReport *h=NULL;
	RDArsrc *rsrc=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagvirtual || diageval)
	{
		prterr("DIAG RPTMTNGvirtualSubData Substituting for [%s].",*tmp);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(PSTRUCT!=NULL)
	{
		MTNMASTER=PSTRUCT->MTNMASTER;
		if(MTNMASTER!=NULL)
		{
			h=(HoldReport *)MTNMASTER->passparent;
			if(h!=NULL)
			{
				rsrc=PSTRUCT->rsrc;
				modulename=stripmodulename(*tmp);
				if(!isEMPTY(modulename))
				{
					if(RPTMTNExecuteSubData(SORTReportGenSubData1,tmp,rsrc,h,modulename)==0)
					{
#ifdef USE_RDA_DIAGNOSTICS
						if(diagvirtual|| diageval)
						{
							prterr("DIAG RPTMTNSubData Returning [%s].",*tmp);
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
						if(modulename!=NULL) Rfree(modulename);
						return;
					} else if(RPTMTNExecuteSubData(VIRTUALMTNReportGenSubData1,tmp,rsrc,h,modulename)==0)
					{
#ifdef USE_RDA_DIAGNOSTICS
						if(diagvirtual|| diageval)
						{
							prterr("DIAG RPTMTNSubData Returning [%s].",*tmp);
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
						if(modulename!=NULL) Rfree(modulename);
						return;
					} else if(RPTMTNExecuteSubData(FIELDReportGenSubData1,tmp,rsrc,h,modulename)==0)
					{
#ifdef USE_RDA_DIAGNOSTICS
						if(diagvirtual|| diageval)
						{
							prterr("DIAG RPTMTNSubData Returning [%s].",*tmp);
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
						if(modulename!=NULL) Rfree(modulename);
						return;
					} else if(GLOBALSubData(tmp,modulename)==0)
					{
#ifdef USE_RDA_DIAGNOSTICS
						if(diagvirtual|| diageval)
						{
							prterr("DIAG RPTMTNSubData Returning [%s].",*tmp);
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
						if(modulename!=NULL) Rfree(modulename);
						return;
					} else if(RPTMTNExecuteSubData(SCRNvirtualFIELDSubData,tmp,rsrc,NULL,modulename)==0)
					{
#ifdef USE_RDA_DIAGNOSTICS
						if(diagvirtual|| diageval)
						{
							prterr("DIAG RPTMTNSubData Returning [%s].",*tmp);
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
						if(modulename!=NULL) Rfree(modulename);
						return;
					} else if(RPTMTNExecuteSubData(SCRNvirtualVIRTUALSubData,tmp,rsrc,NULL,modulename)==0)
					{
#ifdef USE_RDA_DIAGNOSTICS
						if(diagvirtual|| diageval)
						{
							prterr("DIAG RPTMTNSubData Returning [%s].",*tmp);
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
						if(modulename!=NULL) Rfree(modulename);
						return;
					} else if(ScrolledListSubData(tmp,rsrc)==0)
					{
#ifdef USE_RDA_DIAGNOSTICS
						if(diagvirtual|| diageval)
						{
							prterr("DIAG SCRNvirtualSubData Returning [%s].",*tmp);
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
						if(modulename!=NULL) Rfree(modulename);
						return;
					} else {
						prterr("Error SCRNvirtualSubData [%s] not found.",*tmp);
						if(*tmp!=NULL) Rfree(*tmp);
						*tmp=stralloc("\"\"");
#ifdef USE_RDA_DIAGNOSTICS
						if(diagvirtual || diageval)
						{
								prterr("DIAG SCRNvirtualSubData Returning [%s].",*tmp);
						}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
						if(modulename!=NULL) Rfree(modulename);
						return;
					}
				} else {
					prterr("Error SCRNvirtualSubData [%s] not found.",*tmp);
					if(*tmp!=NULL) Rfree(*tmp);
					*tmp=stralloc("\"\"");
#ifdef USE_RDA_DIAGNOSTICS
					if(diagvirtual || diageval)
					{
						prterr("DIAG SCRNvirtualSubData Returning [%s].",*tmp);
					}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
					if(modulename!=NULL) Rfree(modulename);
				}
			}
		}
	}
}
