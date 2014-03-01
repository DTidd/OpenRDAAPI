/* evaluat2.c - Additional Evaluation Functions for Translations */
#include<evaluate.hpp>
#include<cctype>

#define OP	1
#define KEYWORD	2
#define NUMBER 4
#define FIELDVALUE 5

char *xstripmodulename(char *rname,int line,char *file)
{
	char *tmp=NULL;
	char *endx;

#ifdef USE_RDA_DIAGNOSTICS

	if(diagmix)
	{
		prterr("DIAG stripmodulename from [%s] at line [%d] program [%s].",rname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	rname=strchr(rname,'[');
	if(rname!=NULL)
	{
		endx=strchr(++rname,']');
		if(endx!=NULL) *endx=0;
		tmp=stralloc(rname);
		if(endx!=NULL) *endx=']';
	}
	return(tmp);
}
char *xstripfilename(char *rname,int line,char *file)
{
	char *tmp=NULL;
#ifdef GOOFY_PARSING
/* used to Parse [] things within the field/file/module names */
/* not used because it's too slow, though in a pinch dev can */
	char *temp=NULL;
	short start=0,count=0;
#else
	char *end;
#endif

#ifdef USE_RDA_DIAGNOSTICS

	if(diagmix)
	{
		prterr("DIAG stripfilename from [%s] at line [%d] program [%s].",rname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifndef GOOFY_PARSING
	rname=strchr(rname,'[');
	if(rname!=NULL)
	{
		rname=strchr(++rname,'[');
		if(rname!=NULL)
		{
			end=strchr(++rname,']');
			if(end!=NULL) *end=0;
			tmp=stralloc(rname);
			if(end!=NULL) *end=']';
		}
	}
#else 
	temp=rname;
	while(*temp)
	{
		if(*temp=='[') 
		{
			++start;
		} else if(*temp==']')
		{
			--start;
			if(start==0) 
			{
				++count;	
				++temp;
			}

		}
		if(count==1)
		{
			tmp=stralloc(++temp);	
			if(*tmp=='[') 	
			{
				temp=tmp;
				while(*temp)
				{
					if(*temp=='[') ++start;	
					++temp;
				}
				temp=tmp;
				for(count=0;count<(start+1);++count)
				{
					temp=strchr(++temp,']');
				}
				if(temp!=NULL) *temp=0;
					
			} else {
				start=RDAstrlen(tmp);
				tmp[start-1]=0;
			}
			break;
		}
		++temp;
	}
#endif
#ifdef USE_RDA_DIAGNOSTICS

	if(diagmix)
	{
		prterr("DIAG stripfilename returning [%s] at line [%d] program [%s].",(tmp!=NULL ? tmp:""),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(tmp);
}
char *xstripfieldname(char *rname,int line,char *file)
{
	char *tmp=NULL;
#ifdef GOOFY_PARSING
/* used to Parse [] things within the field/file/module names */
/* not used because it's too slow, though in a pinch dev can */
	char *temp=NULL;
	short start=0,count=0;
#else
	char *end;
#endif

#ifdef USE_RDA_DIAGNOSTICS

	if(diagmix)
	{
		prterr("DIAG stripfieldname [%s] at line [%d] program [%s].",rname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
#ifndef GOOFY_PARSING
	rname=strchr(rname,'[');
	if(rname!=NULL)
	{
		rname=strchr(++rname,'[');
		if(rname!=NULL)
		{
			rname=strchr(++rname,'[');
			if(rname!=NULL)
			{
				end=strchr(++rname,']');
				if(end!=NULL) *end=0;
				tmp=stralloc(rname);
				if(end!=NULL) *end=']';
			}
		}
	}
#else 
	temp=rname;
	while(*temp)
	{
		if(*temp=='[') 
		{
			++start;
		} else if(*temp==']')
		{
			--start;
			if(start==0) 
			{
				++count;	
				++temp;
			}

		}
		if(count==2)
		{
			tmp=stralloc(++temp);	
			if(*tmp=='[') 	
			{
				temp=tmp;
				while(*temp)
				{
					if(*temp=='[') ++start;	
					++temp;
				}
				temp=tmp;
				for(count=0;count<(start+1);++count)
				{
					temp=strchr(++temp,']');
				}
				if(temp!=NULL) *temp=0;
					
			} else {
				start=RDAstrlen(tmp);
				tmp[start-1]=0;
			}
			break;
		}
		++temp;
	}
#endif
#ifdef USE_RDA_DIAGNOSTICS

	if(diagmix)
	{
		prterr("DIAG stripfieldname retuning [%s] at line [%d] program [%s].",(tmp!=NULL ? tmp:""),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(tmp);
}
void subfielddata(char **tmp,short *fileno)
{
	char *fieldname=NULL;
	NRDfield *field;
	short start=1,length=0,str_length=0;
	char *temp=NULL,*gn;

#ifdef USE_RDA_DIAGNOSTICS

	if(diageval)
	{
		prterr("DIAG subfielddata Replaces [%s] with a value using file [%d] [%s].",(*tmp!=NULL ? *tmp:""),*fileno,FILENAME(*fileno));
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	fieldname=stripfieldname(*tmp);
	if(fieldname!=NULL)
	{
		field=FLDNRDsec(*fileno,fieldname,TRUE,FALSE,FALSE);
	} else field=NULL;
	if(field!=NULL)
	{
		memset(stemp,0,101);
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
				if(RDAstrlen(*tmp)>(RDAstrlen(fieldname)+2))
				{
					temp=*tmp+(RDAstrlen(fieldname)+2);
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
				str_length=(field->data.string_value!=NULL ? RDAstrlen(field->data.string_value):0);
				if(length==0) length=str_length;
				if(RDAstrlen(*tmp)<(length+3))
				{
					*tmp=Rrealloc(*tmp,length+3);
				}
				if(field->data.string_value!=NULL && str_length>length)
				{
					temp=stralloc(field->data.string_value);
					gn=temp+(start-1);
					if(RDAstrlen(gn)>(start-1+length))
						temp[start-1+length]=0;
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"\"%s\"",&temp[start-1]);
					if(temp!=NULL) Rfree(temp);
					temp=NULL;
				} else {
					memset(*tmp,0,RDAstrlen(*tmp)+1);
					sprintf(*tmp,"\"%s\"",(field->data.string_value!=NULL ? field->data.string_value:""));
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
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
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
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case SDECIMALV:
				if(*field->data.float_value<0)
				{
					sprintf(stemp,"(%.08f)",*field->data.float_value);
				} else {
					sprintf(stemp,"%.08f",*field->data.float_value);
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
				prterr("Error Field Type [%d] is invalid for Field [%s] in Module [%s] File [%s].",field->type,field->name,MODULENAME(*fileno),FILENAME(*fileno)); 
				break;
		}
	} else {
		prterr("Error Field [%s] is not found unable to substitute.",*tmp);
		if(*tmp!=NULL) Rfree(*tmp);
		*tmp=NULL;
	}
	if(fieldname!=NULL) Rfree(fieldname);
#ifdef USE_RDA_DIAGNOSTICS

	if(diageval)
	{
		prterr("DIAG subfielddata Returning Value [%s].",(*tmp!=NULL ? *tmp:""));
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
}
void total_subkeyword(char *s)
{
	if(!RDAstrcmp(s,"if") || !RDAstrcmp(s,"IF")) 
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"IF\t");
	} else if(!RDAstrcmp(s,"then") || !RDAstrcmp(s,"THEN")) 
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"\tTHEN\t");
	} else if(!RDAstrcmp(s,"else") || !RDAstrcmp(s,"ELSE")) 
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"\tELSE\t");
	} else if(!RDAstrcmp(s,"fi") || !RDAstrcmp(s,"FI")) 
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"\tFI");
	}
}
void presortsubkeyword(char *s)
{
	char c=0;

	if(!RDAstrcmp(s,"if") || !RDAstrcmp(s,"IF")) 
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"IF\t");
	} else if(!RDAstrcmp(s,"then") || !RDAstrcmp(s,"THEN")) 
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"\tTHEN\t");
	} else if(!RDAstrcmp(s,"else") || !RDAstrcmp(s,"ELSE")) 
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"\tELSE\t");
	} else if(!RDAstrcmp(s,"fi") || !RDAstrcmp(s,"FI")) 
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"\tFI");
	} else if(!RDAstrcmp(s,"and") || !RDAstrcmp(s,"AND")) 
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"&");
	} else if(!RDAstrcmp(s,"not") || !RDAstrcmp(s,"NOT"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"!");
	} else if(!RDAstrcmp(s,"or") || !RDAstrcmp(s,"OR")) 
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"|");
	} else if(!RDAstrcmp(s,"C") || !RDAstrcmp(s,"c"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"n");
	} else if(!RDAstrcmp(s,"NC") || !RDAstrcmp(s,"nc"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"o");
	} else if(!RDAstrcmp(s,"append") || !RDAstrcmp(s,"APPEND")) 
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"+");
	} else if(!RDAstrcmp(s,"true") || !RDAstrcmp(s,"TRUE")) 
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"1");
	} else if(!RDAstrcmp(s,"false") || !RDAstrcmp(s,"FALSE")) 
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"0");
	} else if(!RDAstrcmp(s,"return_value")) 
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"RETURN_VALUE");
	} else if(!RDAstrcmp(s,"MOD") || !RDAstrcmp(s,"mod")) 
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"%");
	} else if(!RDAstrcmp(s,"spell") || !RDAstrcmp(s,"SPELL"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"?");
	} else if(!RDAstrcmp(s,"roundup") || !RDAstrcmp(s,"ROUNDUP"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"{");
	} else if(!RDAstrcmp(s,"round") || !RDAstrcmp(s,"ROUND"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"}");
	} else if(!RDAstrcmp(s,"string_to_value") ||
		!RDAstrcmp(s,"STRING_TO_VALUE"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,":");
	} else if(!RDAstrcmp(s,"DECIMAL_TO_STRING") || 
		!RDAstrcmp(s,"decimal_to_string"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,";");
	} else if(!RDAstrcmp(s,"DATE_TO_VALUE") ||
		!RDAstrcmp(s,"date_to_value"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"`");
	} else if(!RDAstrcmp(s,"VALUE_TO_TIME") ||
		!RDAstrcmp(s,"value_to_time"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"~");
	} else if(!RDAstrcmp(s,"EVALUATE_BOOLEAN") ||
		!RDAstrcmp(s,"evaluate_boolean"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"a");
	} else if(!RDAstrcmp(s,"EVALUATE_SHORT") ||
		!RDAstrcmp(s,"evaluate_short"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"b");
	} else if(!RDAstrcmp(s,"EVALUATE_INTEGER") ||
		!RDAstrcmp(s,"evaluate_integer"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"c");
	} else if(!RDAstrcmp(s,"EVALUATE_DOUBLE") ||
		!RDAstrcmp(s,"evaluate_double"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"d");
	} else if(!RDAstrcmp(s,"EVALUATE_STRING") ||
		!RDAstrcmp(s,"evaluate_string"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"e");
	} else if(!RDAstrcmp(s,"BOOLEAN_TO_STRING") ||
		!RDAstrcmp(s,"boolean_to_string"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"f");
	} else if(!RDAstrcmp(s,"SHORT_TO_STRING") ||
		!RDAstrcmp(s,"short_to_string"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"g");
	} else if(!RDAstrcmp(s,"INTEGER_TO_STRING") ||
		!RDAstrcmp(s,"integer_to_string"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"h");
	} else if(!RDAstrcmp(s,"FLOAT_TO_STRING") ||
		!RDAstrcmp(s,"float_to_string"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"i");
	} else if(!RDAstrcmp(s,"MERGE_ACCOUNT") ||
		!RDAstrcmp(s,"merge_account"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"j");
	} else if(!RDAstrcmp(s,"STRING_TO_UPPER") ||
		!RDAstrcmp(s,"string_to_upper"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"k");
	} else if(!RDAstrcmp(s,"STRING_TO_LOWER") ||
		!RDAstrcmp(s,"string_to_lower"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"l");
	} else if(!RDAstrcmp(s,"COMPUTE_TABLE_BASE") ||
		!RDAstrcmp(s,"compute_table_base"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"m");
	} else if(!RDAstrcmp(s,"COMPUTE_TABLE_RATE") ||
		!RDAstrcmp(s,"compute_table_rate"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"n");
	} else if(!RDAstrcmp(s,"COMPUTE_MATRIX_ROWCOLUMN") ||
		!RDAstrcmp(s,"compute_matrix_rowcolumn"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"o");
	} else if(!RDAstrcmp(s,"COMPUTE_MATRIX_LINEARLY") ||
		!RDAstrcmp(s,"compute_matrix_linearly"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"p");
	} else if(!RDAstrcmp(s,"COMPUTE_TABLE") ||
		!RDAstrcmp(s,"compute_table"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"q");
	} else if(!RDAstrcmp(s,"STRING_LENGTH") ||
		!RDAstrcmp(s,"string_length"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"r");
	} else if(!RDAstrcmp(s,"DOLLARS_TO_STRING") ||
		!RDAstrcmp(s,"dollars_to_string"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"s");
	} else if(!RDAstrcmp(s,"DOLLARSNS_TO_STRING") ||
		!RDAstrcmp(s,"dollarsns_to_string"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"t");
	} else if(!RDAstrcmp(s,"DATE_TO_YEARDAY") ||
		!RDAstrcmp(s,"date_to_yearday"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"u");
	} else if(!RDAstrcmp(s,"DECIMAL_TO_STRING") ||
		!RDAstrcmp(s,"decimal_to_string"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"v");
	} else if(!RDAstrcmp(s,"GENERIC_SETUP_VARIABLE") ||
		!RDAstrcmp(s,"generic_setup_variable"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"w");
	} else if(!RDAstrcmp(s,"INTEGER_TO_ENCRYPTEDA") ||
		!RDAstrcmp(s,"integer_to_encrypteda"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"x");
	} else if(!RDAstrcmp(s,"GENERIC_COUNTER") ||
		!RDAstrcmp(s,"generic_counter"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"y");
	} else if(!RDAstrcmp(s,"ADD_DASHES") ||
		!RDAstrcmp(s,"add_dashes"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"z");
	} else if(!RDAstrcmp(s,"MD_FIVE_SUM") ||
		!RDAstrcmp(s,"md_five_sum"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"_");
	} else if(!RDAstrcmp(s,"READ_FILE") ||
		!RDAstrcmp(s,"read_file"))
	{
		memset(s,0,RDAstrlen(s)+1);
		c=(-1);
		memset(s,c,1);
	} else if(!RDAstrcmp(s,"BASE_SIXTYFOUR_ENCODE") ||
		!RDAstrcmp(s,"base_sixtyfour_encode"))
	{
		memset(s,0,RDAstrlen(s)+1);
		c=(-2);
		memset(s,c,1);
	} else if(!RDAstrcmp(s,"BASE_SIXTYFOUR_DECODE") ||
		!RDAstrcmp(s,"base_sixtyfour_decode"))
	{
		memset(s,0,RDAstrlen(s)+1);
		c=(-3);
		memset(s,c,1);
	} else if(!RDAstrcmp(s,"READ_FILE_HEX") ||
		!RDAstrcmp(s,"read_file_hex"))
	{
		memset(s,0,RDAstrlen(s)+1);
		c=(-4);
		memset(s,c,1);
	} else if(!RDAstrcmp(s,"HEX_TO_BASE_SIXTYFOUR_ENCODE") ||
		!RDAstrcmp(s,"hex_to_base_sixtyfour_encode"))
	{
		memset(s,0,RDAstrlen(s)+1);
		c=(-5);
		memset(s,c,1);
	} else if(!RDAstrcmp(s,"BASE_SIXTYFOUR_DECODE_TO_HEX") ||
		!RDAstrcmp(s,"base_sixtyfour_decode_to_hex"))
	{
		memset(s,0,RDAstrlen(s)+1);
		c=(-6);
		memset(s,c,1);
	} else if(!RDAstrcmp(s,"STRING_TO_HEX") ||
		!RDAstrcmp(s,"string_to_hex"))
	{
		memset(s,0,RDAstrlen(s)+1);
		c=(-7);
		memset(s,c,1);
	} else if(!RDAstrcmp(s,"HEX_TO_STRING") ||
		!RDAstrcmp(s,"hex_to_string"))
	{
		memset(s,0,RDAstrlen(s)+1);
		c=(-8);
		memset(s,c,1);
	} else if(!RDAstrcmp(s,"DOLLARS_TO_TEXT") ||
		!RDAstrcmp(s,"dollars_to_text"))
	{
		memset(s,0,RDAstrlen(s)+1);
		c=(-9);
		memset(s,c,1);
	} else if(!RDAstrcmp(s,"BASE_SIXTYFOUR_WRITE_FILE") ||
		!RDAstrcmp(s,"base_sixtyfour_write_file"))
	{
		memset(s,0,RDAstrlen(s)+1);
		c=(-10);
		memset(s,c,1);
	} else if(!RDAstrcmp(s,"BASE_SIXTYFOUR_READ_FILE") ||
		!RDAstrcmp(s,"base_sixtyfour_read_file"))
	{
		memset(s,0,RDAstrlen(s)+1);
		c=(-11);
		memset(s,c,1);
	} else if(!RDAstrcmp(s,"WT_BCRYPT_VALUE") || 
		!RDAstrcmp(s,"wt_bcrypt_value"))
	{
		memset(s,0,RDAstrlen(s)+1);
		c=(-12);
		memset(s,c,1);
	}
}
void subkeyword(char *s)
{
	char c=0;

	if(!RDAstrcmp(s,"and") || !RDAstrcmp(s,"AND")) 
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"&");
	} else if(!RDAstrcmp(s,"not") || !RDAstrcmp(s,"NOT"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"!");
	} else if(!RDAstrcmp(s,"or") || !RDAstrcmp(s,"OR")) 
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"|");
	} else if(!RDAstrcmp(s,"C") || !RDAstrcmp(s,"c"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"n");
	} else if(!RDAstrcmp(s,"NC") || !RDAstrcmp(s,"nc"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"o");
	} else if(!RDAstrcmp(s,"append") || !RDAstrcmp(s,"APPEND")) 
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"+");
	} else if(!RDAstrcmp(s,"true") || !RDAstrcmp(s,"TRUE")) 
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"1");
	} else if(!RDAstrcmp(s,"false") || !RDAstrcmp(s,"FALSE")) 
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"0");
	} else if(!RDAstrcmp(s,"return_value")) 
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"RETURN_VALUE");
	} else if(!RDAstrcmp(s,"MOD") || !RDAstrcmp(s,"mod")) 
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"%");
	} else if(!RDAstrcmp(s,"spell") || !RDAstrcmp(s,"SPELL"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"?");
	} else if(!RDAstrcmp(s,"roundup") || !RDAstrcmp(s,"ROUNDUP"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"{");
	} else if(!RDAstrcmp(s,"round") || !RDAstrcmp(s,"ROUND"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"}");
	} else if(!RDAstrcmp(s,"string_to_value") ||
		!RDAstrcmp(s,"STRING_TO_VALUE"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,":");
	} else if(!RDAstrcmp(s,"DECIMAL_TO_STRING") || 
		!RDAstrcmp(s,"decimal_to_string"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,";");
	} else if(!RDAstrcmp(s,"DATE_TO_VALUE") ||
		!RDAstrcmp(s,"date_to_value"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"`");
	} else if(!RDAstrcmp(s,"VALUE_TO_TIME") ||
		!RDAstrcmp(s,"value_to_time"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"~");
	} else if(!RDAstrcmp(s,"EVALUATE_BOOLEAN") ||
		!RDAstrcmp(s,"evaluate_boolean"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"a");
	} else if(!RDAstrcmp(s,"EVALUATE_SHORT") ||
		!RDAstrcmp(s,"evaluate_short"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"b");
	} else if(!RDAstrcmp(s,"EVALUATE_INTEGER") ||
		!RDAstrcmp(s,"evaluate_integer"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"c");
	} else if(!RDAstrcmp(s,"EVALUATE_DOUBLE") ||
		!RDAstrcmp(s,"evaluate_double"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"d");
	} else if(!RDAstrcmp(s,"EVALUATE_STRING") ||
		!RDAstrcmp(s,"evaluate_string"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"e");
	} else if(!RDAstrcmp(s,"BOOLEAN_TO_STRING") ||
		!RDAstrcmp(s,"boolean_to_string"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"f");
	} else if(!RDAstrcmp(s,"SHORT_TO_STRING") ||
		!RDAstrcmp(s,"short_to_string"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"g");
	} else if(!RDAstrcmp(s,"INTEGER_TO_STRING") ||
		!RDAstrcmp(s,"integer_to_string"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"h");
	} else if(!RDAstrcmp(s,"FLOAT_TO_STRING") ||
		!RDAstrcmp(s,"float_to_string"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"i");
	} else if(!RDAstrcmp(s,"MERGE_ACCOUNT") ||
		!RDAstrcmp(s,"merge_account"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"j");
	} else if(!RDAstrcmp(s,"STRING_TO_UPPER") ||
		!RDAstrcmp(s,"string_to_upper"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"k");
	} else if(!RDAstrcmp(s,"STRING_TO_LOWER") ||
		!RDAstrcmp(s,"string_to_lower"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"l");
	} else if(!RDAstrcmp(s,"COMPUTE_TABLE_BASE") ||
		!RDAstrcmp(s,"compute_table_base"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"m");
	} else if(!RDAstrcmp(s,"COMPUTE_TABLE_RATE") ||
		!RDAstrcmp(s,"compute_table_rate"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"n");
	} else if(!RDAstrcmp(s,"COMPUTE_MATRIX_ROWCOLUMN") ||
		!RDAstrcmp(s,"compute_matrix_rowcolumn"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"o");
	} else if(!RDAstrcmp(s,"COMPUTE_MATRIX_LINEARLY") ||
		!RDAstrcmp(s,"compute_matrix_linearly"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"p");
	} else if(!RDAstrcmp(s,"COMPUTE_TABLE") ||
		!RDAstrcmp(s,"compute_table"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"q");
	} else if(!RDAstrcmp(s,"STRING_LENGTH") ||
		!RDAstrcmp(s,"string_length"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"r");
	} else if(!RDAstrcmp(s,"DOLLARS_TO_STRING") ||
		!RDAstrcmp(s,"dollars_to_string"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"s");
	} else if(!RDAstrcmp(s,"DOLLARSNS_TO_STRING") ||
		!RDAstrcmp(s,"dollarsns_to_string"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"t");
	} else if(!RDAstrcmp(s,"DATE_TO_YEARDAY") ||
		!RDAstrcmp(s,"date_to_yearday"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"u");
	} else if(!RDAstrcmp(s,"DECIMAL_TO_STRING") ||
		!RDAstrcmp(s,"decimal_to_string"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"v");
	} else if(!RDAstrcmp(s,"GENERIC_SETUP_VARIABLE") ||
		!RDAstrcmp(s,"generic_setup_variable"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"w");
	} else if(!RDAstrcmp(s,"GENERIC_COUNTER") ||
		!RDAstrcmp(s,"generic_counter"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"y");
	} else if(!RDAstrcmp(s,"INTEGER_TO_ENCRYPTEDA") ||
		!RDAstrcmp(s,"integer_to_encrypteda"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"x");
	} else if(!RDAstrcmp(s,"ADD_DASHES") ||
		!RDAstrcmp(s,"add_dashes"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"z");
	} else if(!RDAstrcmp(s,"MD_FIVE_SUM") ||
		!RDAstrcmp(s,"md_five_sum"))
	{
		memset(s,0,RDAstrlen(s)+1);
		strcpy(s,"_");
	} else if(!RDAstrcmp(s,"READ_FILE") ||
		!RDAstrcmp(s,"read_file"))
	{
		memset(s,0,RDAstrlen(s)+1);
		c=(-1);
		memset(s,c,1);
	} else if(!RDAstrcmp(s,"BASE_SIXTYFOUR_ENCODE") ||
		!RDAstrcmp(s,"base_sixtyfour_encode"))
	{
		memset(s,0,RDAstrlen(s)+1);
		c=(-2);
		memset(s,c,1);
	} else if(!RDAstrcmp(s,"BASE_SIXTYFOUR_DECODE") ||
		!RDAstrcmp(s,"base_sixtyfour_decode"))
	{
		memset(s,0,RDAstrlen(s)+1);
		c=(-3);
		memset(s,c,1);
	} else if(!RDAstrcmp(s,"READ_FILE_HEX") ||
		!RDAstrcmp(s,"read_file_hex"))
	{
		memset(s,0,RDAstrlen(s)+1);
		c=(-4);
		memset(s,c,1);
	} else if(!RDAstrcmp(s,"HEX_TO_BASE_SIXTYFOUR_ENCODE") ||
		!RDAstrcmp(s,"hex_to_base_sixtyfour_encode"))
	{
		memset(s,0,RDAstrlen(s)+1);
		c=(-5);
		memset(s,c,1);
	} else if(!RDAstrcmp(s,"BASE_SIXTYFOUR_DECODE_TO_HEX") ||
		!RDAstrcmp(s,"base_sixtyfour_decode_to_hex"))
	{
		memset(s,0,RDAstrlen(s)+1);
		c=(-6);
		memset(s,c,1);
	} else if(!RDAstrcmp(s,"ASCII_TO_HEX") ||
		!RDAstrcmp(s,"ascii_to_hex"))
	{
		memset(s,0,RDAstrlen(s)+1);
		c=(-7);
		memset(s,c,1);
	} else if(!RDAstrcmp(s,"HEX_TO_ASCII") ||
		!RDAstrcmp(s,"hex_to_ascii"))
	{
		memset(s,0,RDAstrlen(s)+1);
		c=(-8);
		memset(s,c,1);
	} else if(!RDAstrcmp(s,"DOLLARS_TO_TEXT") ||
		!RDAstrcmp(s,"dollars_to_text"))
	{
		memset(s,0,RDAstrlen(s)+1);
		c=(-9);
		memset(s,c,1);
	
	} else if(!RDAstrcmp(s,"BASE_SIXTYFOUR_WRITE_FILE") ||
		!RDAstrcmp(s,"base_sixtyfour_write_file"))
	{
		memset(s,0,RDAstrlen(s)+1);
		c=(-10);
		memset(s,c,1);
	} else if(!RDAstrcmp(s,"BASE_SIXTYFOUR_READ_FILE") ||
		!RDAstrcmp(s,"base_sixtyfour_read_file"))
	{
		memset(s,0,RDAstrlen(s)+1);
		c=(-11);
		memset(s,c,1);
	} else if(!RDAstrcmp(s,"WT_BCRYPT_VALUE") || 
		!RDAstrcmp(s,"wt_bcrypt_value"))
	{
		memset(s,0,RDAstrlen(s)+1);
		c=(-12);
		memset(s,c,1);
	}
}
short get_info(char **prog,int *tok_type,char **holdvarx,void (*subfieldvalfunc)(...),void *arg,short dowhich)
{
	char *temp,*temp2;
	short allvalues=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
/*
	if(diageval)
	{
		prterr("DIAG get_info reading token type [%d], dowhich [%d] ,*prog: \n",*tok_type,dowhich);
		temp2=*prog;
		while(TRUE)
		{
			if(RDAstrlen(temp2)<256)
			{
				prterr("%s\n",temp2);
				break;
			} else {
				prterr("%.256s\n",temp2);
			}
			temp2=temp2+256;
		}
		prterr("     ,*holdvarx: \n");
		temp2=*holdvarx;
		while(TRUE)
		{
			if(RDAstrlen(temp2)<256)
			{
				prterr("%s\n",temp2);
				break;
			} else {
				prterr("%.256s\n",temp2);
			}
			temp2=temp2+256;
		}
	}
*/
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	*tok_type=0;
	temp=*holdvarx;
	if(**prog=='\0' || *prog==NULL)
	{
		*temp='\0';
		return(-1);
	}
	while((iswhite(**prog) && !allvalues) || **prog=='#' || allvalues) 
/* REMOVING COMMENTS BETWEEN THE # SIGNS, DCT DOESN'T RECOMMEND USING... */
	{
		if(**prog=='#') 
		{
			if(allvalues) allvalues=FALSE;
				else allvalues=TRUE;
		}
		++(*prog);
	}
	if(is_in(**prog,func) || **prog<0) /* OPERATORS AND MATHMATICAL FUNCTIONS */
	{
		*tok_type=OP;
		*temp=**prog;
		++temp;++(*prog);
		*temp=0;
		return(0);
	} else if(**prog=='\"') /* STRINGS */ 
	{
		*temp=**prog;
		++temp;++(*prog);
		while(**prog!='\0') 
		{
			*temp=**prog;
			temp2=*prog;
			++temp;
			--temp2;
			if((**prog=='\"') && (*temp2!='\\'))
			{
				*temp=0;
				++(*prog);
				return(0);
			}
			++(*prog);
		}
	} else if(**prog=='[') /* VARIABLE RESOURCES */
	{	
		*temp=**prog;
		allvalues=TRUE;
		while((allvalues || is_in(**prog,"[]")) && **prog!='\0') 
		{
			if(**prog==']') allvalues=FALSE;
			else if(**prog=='[') allvalues=TRUE;
			*temp=**prog;
			++temp;++(*prog);
		}
		allvalues=FALSE;
		*temp=0;
		*tok_type=FIELDVALUE;
		if(dowhich==TRUE)
		{
			if(subfieldvalfunc!=NULL)
			{
/*lint -e746 */
				subfieldvalfunc(holdvarx,arg);
/*lint +e746 */
				if(**holdvarx=='\"')
				{
					sub_quotes(holdvarx);
				}
/* To avoid changing all SubData() look for leading and trailing quotes here*/
/* holdvarx will have a quote starting and ending this string when it's a 
   string as opposed to */
			}	
		}
		return(0);
	} else if(isalpha(**prog) || **prog=='_') /* KEY WORDS */
	{
		while((isalpha(**prog) || **prog=='_') && **prog!='\0') 
		{
			*temp=**prog;
			++temp;++(*prog);
		}
		*temp=0;
		*tok_type=KEYWORD;
		if(dowhich==TRUE)
		{
			subkeyword(*holdvarx);
		} else if(dowhich==FALSE)
		{
			presortsubkeyword(*holdvarx);
		} else if(dowhich==(-1))
		{
			total_subkeyword(*holdvarx);
		} else {
			total_subkeyword(*holdvarx);
			prterr("Error translate type error..... executing total_subkeyword to ensure correctness.");
		}
		return(0);
	} else if(isdigit(**prog)) /* NUMERICAL VALUES */
	{
		while(!isdelim(**prog) && **prog!='\0') 
		{
			*temp=**prog;
			++temp;++(*prog);
		}
		*tok_type=NUMBER;
		*temp=0;
		return(0);
	}
	++(*prog); /* something unrecognized */ 
	return(1);
}
char *xPP_translate(char *expression,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS

	if(diageval)
	{
		prterr("PP_translate at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(xtranslate(expression,NULL,NULL,-1,__LINE__,__FILE__));
}
static void EVALunpad(char *string)
{
	int x;

	if(!isEMPTY(string))
	{
		x=RDAstrlen(string)-1;
		for(string+=x;x>=0 && (*string==' ' || *string=='\r' || *string=='\n');--string,--x) *string=0;
	}
}
char *xtranslate(char *expression,void (*subfieldvalfunc)(...),void *arg,short dowhich,int line,char *file)
{
	char *tmp=NULL;
	int count=0,tok_type=0;
	char *holdvarx=NULL,*prog=NULL;
	unsigned len=0,len_express=0;

#ifdef USE_RDA_DIAGNOSTICS

	char *temp=NULL;
	if(diageval)
	{
		prterr("DIAG Translate Expression at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	EVALunpad(expression);
	if(!isEMPTY(expression))
	{
#ifdef USE_RDA_DIAGNOSTICS

		if(diageval)
		{
			temp=expression;
			prterr("Expression:\n");
			while(TRUE)
			{
				if(RDAstrlen(temp)<256)
				{
					prterr("%s\n",temp);
					break;
				} else { 
					prterr("%.256s\n",temp);
				}
				temp=temp+256;
			}
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
/* substitute expressions from datafields so the value of the expression may
   be used in the computation of another virtual field only do this the first
   time thru */
		for(prog=expression;*prog;++prog) 
			if(*prog=='\n' || *prog=='\r' || *prog=='\t') *prog=' ';
		prog=expression;
		len_express=RDAstrlen(expression);
		holdvarx=Rmalloc(len_express+1);
		while(TRUE)
		{
			if(holdvarx==NULL)
			{
				holdvarx=Rmalloc(len_express+1);
				memset(holdvarx,0,len_express+1);
/*
			} else {
				memset(holdvarx,0,RDAstrlen(holdvarx)+1);
*/
			}
			if(get_info(&prog,&tok_type,&holdvarx,subfieldvalfunc,arg,dowhich)==(-1)) break;
			if(!isEMPTY(holdvarx))
			{
#ifdef USE_RDA_DIAGNOSTICS

				if(diageval)
				{
					prterr("DIAG get_info Returning [%s] ",holdvarx);
				}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
/* possible to place this part with a stralloccat function, uncertain as to
   any performance gain that it would make */
				len=RDAstrlen(holdvarx);
				if(tmp!=NULL)
				{
					tmp=Rrealloc(tmp,count+len+2);
				} else {
					tmp=Rmalloc(len+1);
					count=0;
				}
				memcpy(&tmp[count],holdvarx,len+1);
				count+=len;
				tmp[count]=0;
/* stralloccat could replace the preceeding code up to the previous coment */
			} else len=0;
			if(tok_type==FIELDVALUE)
			{
				if(holdvarx!=NULL) Rfree(holdvarx);
				holdvarx=Rmalloc(len_express+1);
			}
		}
		if(holdvarx!=NULL) Rfree(holdvarx);
	}
#ifdef USE_RDA_DIAGNOSTICS

	if(diageval || diageval_field)
	{
		prterr("DIAG Expression After Translated.");
		if(!isEMPTY(tmp))
		{
			temp=tmp;
			while(TRUE)
			{
				if(RDAstrlen(temp)<256)
				{
					prterr("%s\n",temp);
					break;
				} else { 
					prterr("%.256s\n",temp);
				}
				temp=temp+256;
			}
		} else prterr("(NULL)\n");
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(tmp);
}
