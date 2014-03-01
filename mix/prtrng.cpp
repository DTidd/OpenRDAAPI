#include<mix.hpp>
char *xgetrangedesc(RangeScreen *rs,int line,char *file)
{
	FileRangeStruct *frs=NULL;
	int y;
	short count=0;
	char *desc=NULL,*to_str=NULL,*from_str=NULL;
	char stemp1[101],atleastone=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG getrangedesc Get File Range Description off Screen [%s] [%s] at line [%d] program [%s].",rs->module,rs->screen,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(rs!=NULL)
	{
	if(rs->frs!=NULL)
	{
	desc=Rmalloc(RDAstrlen(rs->module)+RDAstrlen(rs->screen)+50);
	sprintf(desc,"\r\nUSER DEFINED CRITERIA FOR MODULE: %s SCREEN: %s\r\n\r\n",rs->module,rs->screen);
	count=(RDAstrlen(rs->module)+RDAstrlen(rs->screen)+49);
	for(y=0,frs=rs->frs;y<rs->num;++y,++frs)
	{
		switch(frs->field_type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case PLAINTEXT:
			case CHARACTERS:
			case SOCSECNUM:
			case PHONE:
			case CUSTOMTYPE:
			case DATES:
			case TIMES:
			case ZIPCODE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				if(frs->range)
				{
					if(desc!=NULL)
					{
						desc=Rrealloc(desc,RDAstrlen(desc)+(frs->to.string_value!=NULL ? RDAstrlen(frs->to.string_value):0)+(frs->from.string_value!=NULL ? RDAstrlen(frs->from.string_value):0)+RDAstrlen(frs->fieldname)+29);
					} else desc=Rmalloc(RDAstrlen(desc)+(frs->to.string_value!=NULL ? RDAstrlen(frs->to.string_value):0)+(frs->from.string_value!=NULL ? RDAstrlen(frs->from.string_value):0)+RDAstrlen(frs->fieldname)+29);
					unpad(frs->from.string_value);
					unpad(frs->to.string_value);
					sprintf(&desc[count],"Range on [%s] from [%s] to [%s].\r\n",frs->fieldname,(frs->from.string_value!=NULL ? frs->from.string_value:""),(frs->to.string_value!=NULL ? frs->to.string_value:""));
					count+=(frs->to.string_value!=NULL ? RDAstrlen(frs->to.string_value):0)+(frs->from.string_value!=NULL ? RDAstrlen(frs->from.string_value):0)+RDAstrlen(frs->fieldname)+28;
					atleastone=TRUE;
				}
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				if(frs->range)
				{
					if(desc!=NULL)
					{
						desc=Rrealloc(desc,RDAstrlen(desc)+((frs->field_len)*2)+RDAstrlen(frs->fieldname)+29);
					} else desc=Rmalloc(((frs->field_len)*2)+RDAstrlen(frs->fieldname)+29);
					sprintf(&desc[count],"Range on [%s] from [%*d] to [%*d].\r\n",frs->fieldname,frs->field_len,*frs->from.integer_value,frs->field_len,*frs->to.integer_value);
					count+=((frs->field_len)*2)+RDAstrlen(frs->fieldname)+28;
					atleastone=TRUE;
				}
				break;
			case SHORTV:
			case SSHORTV:
				if(frs->range)
				{
					if(desc!=NULL)
					{
						desc=Rrealloc(desc,RDAstrlen(desc)+((frs->field_len)*2)+RDAstrlen(frs->fieldname)+29);
					} else desc=Rmalloc(((frs->field_len)*2)+RDAstrlen(frs->fieldname)+29);
					sprintf(&desc[count],"Range on [%s] from [%*d] to [%*d].\r\n",frs->fieldname,frs->field_len,*frs->from.short_value,frs->field_len,*frs->to.short_value);
					count+=((frs->field_len)*2)+RDAstrlen(frs->fieldname)+28;
					atleastone=TRUE;
				}
				break;
			case DOLLARS:
				if(frs->range)
				{
					if(desc!=NULL)
					{
						desc=Rrealloc(desc,RDAstrlen(desc)+((frs->field_len)*2)+RDAstrlen(frs->fieldname)+29);
					} else desc=Rmalloc(((frs->field_len)*2)+RDAstrlen(frs->fieldname)+29);
					to_str=famt(*frs->to.float_value,frs->field_len);
					from_str=famt(*frs->from.float_value,frs->field_len);
					sprintf(&desc[count],"Range on [%s] from [%s] to [%s].\r\n",frs->fieldname,(from_str!=NULL?from_str:""),(to_str!=NULL?to_str:""));
					if(to_str!=NULL) Rfree(to_str);
					if(from_str!=NULL) Rfree(from_str);
					count+=((frs->field_len)*2)+RDAstrlen(frs->fieldname)+28;
					atleastone=TRUE;
				}
				break;
			case DOLLARS_NOCENTS:
				if(frs->range)
				{
					if(desc!=NULL)
					{
						desc=Rrealloc(desc,RDAstrlen(desc)+((frs->field_len)*2)+RDAstrlen(frs->fieldname)+29);
					} else desc=Rmalloc(((frs->field_len)*2)+RDAstrlen(frs->fieldname)+29);
					to_str=famtncents(*frs->to.float_value*100,frs->field_len,' ');
					from_str=famtncents(*frs->from.float_value*100,frs->field_len,' ');
					sprintf(&desc[count],"Range on [%s] from [%s] to [%s].\r\n",frs->fieldname,(from_str!=NULL?from_str:""),(to_str!=NULL?to_str:""));
					if(to_str!=NULL) Rfree(to_str);
					if(from_str!=NULL) Rfree(from_str);
					count+=((frs->field_len)*2)+RDAstrlen(frs->fieldname)+28;
					atleastone=TRUE;
				}
				break;
			case DOUBLEV:
			case SDOUBLEV:
				if(frs->range)
				{
					if(desc!=NULL)
					{
						desc=Rrealloc(desc,RDAstrlen(desc)+((frs->field_len)*2)+RDAstrlen(frs->fieldname)+29);
					} else desc=Rmalloc(((frs->field_len)*2)+RDAstrlen(frs->fieldname)+29);
					sprintf(&desc[count],"Range on [%s] from [%*.0f] to [%*.0f].\r\n",frs->fieldname,frs->field_len,*frs->from.float_value,frs->field_len,*frs->to.float_value);
					count+=((frs->field_len)*2)+RDAstrlen(frs->fieldname)+28;
					atleastone=TRUE;
				}
				break;
			case DECIMALV:
			case SDECIMALV:
				if(frs->range)
				{
					memset(stemp,0,101);
					memset(stemp1,0,101);
					sprintf(stemp,"%*f",frs->field_len,*frs->from.float_value);
					sprintf(stemp1,"%*f",frs->field_len,*frs->to.float_value);
					if(desc!=NULL)
					{
						desc=Rrealloc(desc,RDAstrlen(desc)+RDAstrlen(stemp)+RDAstrlen(stemp1)+RDAstrlen(frs->fieldname)+29);
					} else desc=Rmalloc(RDAstrlen(stemp)+RDAstrlen(stemp1)+RDAstrlen(frs->fieldname)+29);
					sprintf(&desc[count],"Range on [%s] from [%s] to [%s].\r\n",frs->fieldname,stemp,stemp1);
					count+=((frs->field_len)*2)+RDAstrlen(frs->fieldname)+28;
					atleastone=TRUE;
				}
				break;
			case BUTTONS:
				break;
			case BOOLNS:
				if(!*frs->from.string_value || !*frs->to.string_value)
				{
					if(desc!=NULL)
					{
						desc=Rrealloc(desc,(RDAstrlen(desc)+RDAstrlen(frs->fieldname)+36));
					} else desc=Rmalloc((RDAstrlen(frs->fieldname)+36));
					sprintf(&desc[count],"Select [%s] TRUE [%3s] FALSE [%3s].\r\n",(frs->fieldname!=NULL ? frs->fieldname:""),(*frs->from.string_value ? "Yes":"No"),(*frs->to.string_value ? "Yes":"No"));
					count+=RDAstrlen(frs->fieldname)+35;
					atleastone=TRUE;
				}
				break;
			default:
				prterr("Error field type [%d] is invalid for field [%s].",frs->field_type,frs->fieldname);
				break;
		}
	}
	}
	}
	if(atleastone==FALSE)
	{
		desc=Rrealloc(desc,(RDAstrlen(desc)+13));
		sprintf(&desc[count],"SELECT ALL\r\n");
		count+=12;
	}
	return desc;
}
