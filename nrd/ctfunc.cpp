/*lint -library*/
/*lint -e525*/
#include <nrdp.hpp>

void assignCtree_fields(NRDbuffer *NRDtmp)
{
	short x,length=0;
	int pos=0;
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG assignCtree_fields Assigning Current C-tree Record in Module [%s] File [%s] to Database Engine.",NRDtmp->modname,NRDtmp->name); }
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,field=NRDtmp->field;x<NRDtmp->nofields;++x,++field)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagnrd)
		{
			prterr("Field [%d] name [%s] type [%d] [%s] len [%d] pos [%d].",x,field->name,field->type,fieldtypedesc[field->type],field->len,pos);
		}
#endif /* USE_RDA_DIAGNOSTICS */
		switch(field->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT: /* variable length field type */
				length=(short)RDAstrlen(&NRDtmp->record_data[pos]);	
				if(length)
				{
					QUICKALLOC(field->data.string_value,field->dlen,length+1);
					memset(field->data.string_value,0,field->dlen);
					memcpy(field->data.string_value,&NRDtmp->record_data[pos],
						length);
					unpad(field->data.string_value);
				} else if(field->data.string_value!=NULL)
				{
					memset(field->data.string_value,0,field->dlen);
				}
				pos+=(length+1);
				break;
			case PLAINTEXT: /* plain text field type */
			case ZIPCODE:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PHONE:
			case CUSTOMTYPE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
				if(field->len>0)
				{
					QUICKALLOC(field->data.string_value,field->dlen,field->len+1);
					memset(field->data.string_value,0,field->dlen);
					memcpy(field->data.string_value,&NRDtmp->record_data[pos],field->len);
					field->data.string_value[field->len]=0;
					unpad(field->data.string_value);
					pos+=field->len;
				} else {
					length=(short)RDAstrlen(&NRDtmp->record_data[pos]);	
					if(length)
					{
						QUICKALLOC(field->data.string_value,field->dlen,length+1);
						memset(field->data.string_value,0,field->dlen);
						memcpy(field->data.string_value,&NRDtmp->record_data[pos],
							length);
						unpad(field->data.string_value);
					} else if(field->data.string_value!=NULL)
					{
						memset(field->data.string_value,0,field->dlen);
					}
					pos+=(length+1);
				}
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
#ifdef RS6000
#ifdef REVMEM
				reverse_memory((char *)&NRDtmp->record_data[pos],sizeof(double));
#endif
#endif
				memcpy(field->data.float_value,&NRDtmp->record_data[pos],sizeof(double));
				pos+=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
#ifdef RS6000
#ifdef REVMEM
				reverse_memory((char *)&NRDtmp->record_data[pos],sizeof(short));
#endif
#endif
				memcpy(field->data.short_value,&NRDtmp->record_data[pos],sizeof(short));
				pos+=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
#ifdef RS6000
#ifdef REVMEM
				reverse_memory((char *)&NRDtmp->record_data[pos],sizeof(int));
#endif
#endif
				memcpy(field->data.integer_value,&NRDtmp->record_data[pos],sizeof(int));
				pos+=sizeof(int);
				break;
			case BOOLNS:
			case CHARACTERS:
				if(field->type==BOOLNS)
				{
					if(NRDtmp->record_data[pos]) *field->data.string_value=TRUE;
						else *field->data.string_value=FALSE;
				} else *field->data.string_value=NRDtmp->record_data[pos];
				pos+=1;
				break;
			default:
				prterr("Error Field Type [%d] is invalid for Field [%s] in Module [%s] File [%s].",field->type,field->name,NRDtmp->modname,NRDtmp->name);
				break;
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG All Fields Assigned in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
}
#if (defined(USE_CTREE) || defined(USE_BERKELEY_BTREE))
int xCTREERecordLength(NRDbuffer *NRDtmp,short varls,int line,char *file)
{
	int x,length=0;
	NRDfield *field;

	if(diaginformix_isam || diaginformix_isam)
	{
		prterr("DIAG CTREERecordLength at line [%d] program [%s].",line,file);
	}
	length=0;
	if(NRDtmp!=NULL)
	{
		if(NRDtmp->field!=NULL)
		{
			for(x=0,field=NRDtmp->field;x<NRDtmp->nofields;++x,++field)
			{
/* get lengths of variable length fields and fixed fields seperate */
				switch(field->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT: /* variable length field type */
						if(varls)
						{
							if(field->len>0)
							{
								length+=field->len;
							} else {
								length+=RDAstrlen(field->data.string_value)+1;
							}
						}
						break;
					case PLAINTEXT: /* plain text field type */
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case ZIPCODE:
					case CUSTOMTYPE:
					case PHONE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						if(field->len>0)
						{
							length+=field->len;
						} else if(varls)
						{
							length+=RDAstrlen(field->data.string_value)+1;
						}
						break;
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case SDECIMALV:
						length+=sizeof(double);
						break;
					case SHORTV:
					case SSHORTV:
						length+=sizeof(short);
						break;
					case LONGV:
					case SLONGV:
						length+=sizeof(int);
						break;
					case BOOLNS:
					case CHARACTERS:
						length+=1;
						break;
					default:
						prterr("Error Field type %d for field %s\n",
							field->type,field->name);
						break; 
			
				}
			}
		}
	}
	length+=1;
	if(diaginformix_isam || diaginformix_isam)
	{
		prterr("DIAG CTREERecordLength Returning [%d] at line [%d] program [%s].",length,line,file);
	}
	return(length);
}
void assignCTREERecordData(NRDbuffer *NRDtmp,short doall)
{
	int x,y,length=0,tlen=0;
	NRDfield *field;
	NRDkey *key;
	NRDpart *part;

	memset(NRDtmp->record_data,0,NRDtmp->dlen);
	if(doall)
	{
		length=0;
		for(x=0,field=NRDtmp->field;x<NRDtmp->nofields;++x,++field)
		{
			switch(field->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT: /* variable length field type */
					if(field->data.string_value!=NULL)
					unpad(field->data.string_value);
					sprintf(&NRDtmp->record_data[length],"%s",(field->data.string_value!=NULL ? field->data.string_value:""));
					if(diaginformix_isam)
					{
						prterr("DIAG position %d",length);
					}
					if(field->data.string_value!=NULL)
					{
						length+=RDAstrlen(field->data.string_value)+1;
					} else length+=1;
					break;
				case PLAINTEXT: /* plain text field type */
				case ZIPCODE:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case CUSTOMTYPE:
				case PHONE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					if(field->data.string_value!=NULL)
						unpad(field->data.string_value);
					if(field->len>0) /* testing to catch variable lengths */
					{
						sprintf(&NRDtmp->record_data[length],"%-*s",field->len,(field->data.string_value!=NULL ? field->data.string_value:""));
						if(diaginformix_isam)
						{
							prterr("DIAG position %d",length);
						}
						length+=field->len;
					} else {
						if(field->data.string_value!=NULL)
						{
							tlen=RDAstrlen(field->data.string_value)+1;
						} else tlen=0;
						sprintf(&NRDtmp->record_data[length],"%s",
							(field->data.string_value!=NULL ? 
							field->data.string_value:""));
						length+=tlen+1;
					}
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					memcpy(&NRDtmp->record_data[length],
						field->data.float_value,sizeof(double));
#ifdef RS6000
#ifdef REVMEM
					reverse_memory((char *)&NRDtmp->record_data[s],sizeof(double));
#endif
#endif
					length+=sizeof(double);
					break;
				case SHORTV:
				case SSHORTV:
					memcpy(&NRDtmp->record_data[length],
						field->data.short_value,sizeof(short));
#ifdef RS6000
#ifdef REVMEM
					reverse_memory((char *)&NRDtmp->record_data[s],sizeof(short));
#endif
#endif
					length+=sizeof(short);
					break;
				case LONGV:
				case SLONGV:
					memcpy(&NRDtmp->record_data[length],
						field->data.integer_value,sizeof(int));
#ifdef RS6000
#ifdef REVMEM
					reverse_memory((char *)&NRDtmp->record_data[s],sizeof(int));
#endif
#endif
					length+=sizeof(int);
					break;
				case BOOLNS:
				case CHARACTERS:
					memcpy(&NRDtmp->record_data[length],
						field->data.string_value,1);
					length+=1;
					break;
				default:
					prterr("Unknown field type %d for %s in assignCTREERecordData",field->type,field->name);
					break;
			}
		}
	} else {
		key=NRDtmp->key;
		if(key!=NULL)
		{
			for(y=0,part=key->part;y<key->numparts;++y,++part)
			{
				length=part->offset;
				field=FINDFLD(NRDtmp,part->name);
				if(field!=NULL)
				{
					switch(field->type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT: /* variable length field type */
							if(field->data.string_value!=NULL)
								unpad(field->data.string_value);
							sprintf(&NRDtmp->record_data[length],"%s",(field->data.string_value!=NULL ? field->data.string_value:""));
							break;
						case PLAINTEXT: /* plain text field type */
						case ZIPCODE:
						case DATES:
						case TIMES:
						case SOCSECNUM:
						case CUSTOMTYPE:
						case PHONE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
							if(field->data.string_value!=NULL)
								unpad(field->data.string_value);
							if(field->len>0) /* testing to catch variable lengths */
							{
								sprintf(&NRDtmp->record_data[length],"%-*s",field->len,(field->data.string_value!=NULL ? field->data.string_value:""));
							} else {
								sprintf(&NRDtmp->record_data[length],"%s",(field->data.string_value!=NULL ? field->data.string_value:""));
							}
							break;
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case SDECIMALV:
							memcpy(&NRDtmp->record_data[length],field->data.float_value,sizeof(double));
#ifdef RS6000
#ifdef REVMEM
							reverse_memory((char *)&NRDtmp->record_data[length],sizeof(double));
#endif
#endif
							break;
						case SHORTV:
						case SSHORTV:
							memcpy(&NRDtmp->record_data[length],field->data.short_value,sizeof(short));
#ifdef RS6000
#ifdef REVMEM
							reverse_memory((char *)&NRDtmp->record_data[length],sizeof(short));
#endif
#endif
							break;
						case LONGV:
						case SLONGV:
							memcpy(&NRDtmp->record_data[length],field->data.integer_value,sizeof(int));
#ifdef RS6000
#ifdef REVMEM
							reverse_memory((char *)&NRDtmp->record_data[length],sizeof(int));
#endif
#endif
							break;
						case BOOLNS:
						case CHARACTERS:
							memcpy(&NRDtmp->record_data[length],field->data.string_value,1);
							break;
						default:
							prterr("Unknown field type %d for %s in assignCTREERecordData",field->type,field->name);
							break;
					}
				}
			}
		}
	}
}
#endif
