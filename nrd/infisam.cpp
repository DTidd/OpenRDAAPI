/*lint -library*/
/*lint -e525*/
#include <nrdp.hpp>

#if(defined(INFORMIX_ISAM))
#define FLUSH_ME_

#ifdef LINUX2_2
#ifdef LINUX_SUSE6_1
void __bzero(void *s, int n)
{
	bzero(s,n);
}
#endif 
#endif 

void assignIsam_fields(NRDbuffer *NRDtmp)
{
	short pos=0,x,length=0;
	NRDfield *field;
#ifdef INFORMIX_ISAM
	char cstring[2];
#endif

	if(diagnrd) { prterr("DIAG assignIsam_fields Assigning Current C-tree Record in Module [%s] File [%s] to Database Engine.",NRDtmp->modname,NRDtmp->name); }
	for(x=0,field=NRDtmp->field;x<NRDtmp->nofields;++x,++field)
	{
		if(diagnrd)
		{
			prterr("Field [%d] name [%s] type [%d] [%s] len [%d] pos [%d].",x,field->name,field->type,fieldtypedesc[field->type],field->len,pos);
		}
		switch(field->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT: /* variable length field type */
				if(NRDtmp->db_engine==NRDInformixIsamEngine)
				{
#ifdef INFORMIX_ISAM
					QUICKALLOC(field->data.string_value,field->dlen,field->len+1);
					memset(field->data.string_value,0,field->dlen);
					ldchar(&NRDtmp->record_data[pos],field->len,field->data.string_value);
					pos+=(field->len);
#endif
				} else {
					length=(short)RDAstrlen(&NRDtmp->record_data[pos]);	
					if(length)
					{
						QUICKALLOC(field->data.string_value,field->dlen,length+1);
						memset(field->data.string_value,0,field->dlen);
						memcpy(field->data.string_value,&NRDtmp->record_data[pos],length+1);
						unpad(field->data.string_value);
					} else if(field->data.string_value!=NULL)
					{
						memset(field->data.string_value,0,field->dlen);
					}
					pos+=(length+1);
				}
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
				if(NRDtmp->db_engine==NRDInformixIsamEngine)
				{
#ifdef INFORMIX_ISAM
					QUICKALLOC(field->data.string_value,field->dlen,field->len+1);
					memset(field->data.string_value,0,field->dlen);
					ldchar(&NRDtmp->record_data[pos],field->len,field->data.string_value);
					pos+=field->len;
#endif
				} else {
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
							memcpy(field->data.string_value,&NRDtmp->record_data[pos],length+1);
							unpad(field->data.string_value);
						} else if(field->data.string_value!=NULL)
						{
							memset(field->data.string_value,0,field->dlen);
						}
						pos+=(length+1);
					}
				}
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				if(NRDtmp->db_engine==NRDInformixIsamEngine)
				{
#ifdef INFORMIX_ISAM
					*field->data.float_value=lddbl(&NRDtmp->record_data[pos]);
#endif
				} else {
					memcpy(field->data.float_value,&NRDtmp->record_data[pos],sizeof(double));
				}
				pos+=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				if(NRDtmp->db_engine==NRDInformixIsamEngine)
				{
#ifdef INFORMIX_ISAM
					*field->data.short_value=ldint(&NRDtmp->record_data[pos]);
#endif
				} else {
					memcpy(field->data.short_value,&NRDtmp->record_data[pos],sizeof(short));
				}
				pos+=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				if(NRDtmp->db_engine==NRDInformixIsamEngine)
				{
#ifdef INFORMIX_ISAM
					*field->data.integer_value=ldlong(&NRDtmp->record_data[pos]);
#endif
				} else {
					memcpy(field->data.integer_value,&NRDtmp->record_data[pos],sizeof(int));
				}
				pos+=sizeof(int);
				break;
			case BOOLNS:
			case CHARACTERS:
				if(NRDtmp->db_engine==NRDInformixIsamEngine)
				{
#ifdef INFORMIX_ISAM
					ldchar(&NRDtmp->record_data[pos],1,cstring);
					*field->data.string_value=cstring[0];
#endif
				} else {
					if(field->type==BOOLNS)
					{
						if(NRDtmp->record_data[pos]) *field->data.string_value=TRUE;
							else *field->data.string_value=FALSE;
					} else *field->data.string_value=NRDtmp->record_data[pos];
				}
				pos+=1;
				break;
			default:
				prterr("Error Field Type [%d] is invalid for Field [%s] in Module [%s] File [%s].",field->type,field->name,NRDtmp->modname,NRDtmp->name);
				break;
		}
	}
	if(diagnrd)
	{
		prterr("DIAG All Fields Assigned in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
}
int xIsamRecordLength(NRDbuffer *NRDtmp,short varls,int line,char *file)
{
	int x,length=0;
	NRDfield *field;

	if(diaginformix_isam || diaginformix_isam)
	{
		prterr("DIAG IsamRecordLength at line [%d] program [%s].",line,file);
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
		prterr("DIAG IsamRecordLength Returning [%d] at line [%d] program [%s].",length,line,file);
	}
	return(length);
}
void assignNRDRecordData(NRDbuffer *NRDtmp,short doall)
{
	int x,y,length=0,tlen=0;
	NRDfield *field;
	NRDkey *key;
	NRDpart *part;
#ifdef INFORMIX_ISAM
	char cstring[2];
	unsigned short blah=0;
#endif

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
					if(NRDtmp->db_engine==NRDInformixIsamEngine)
					{
#ifdef INFORMIX_ISAM
						if(field->len>0) /* testing to catch variable lengths */
						{
							stchar((field->data.string_value!=NULL ? field->data.string_value:""),&NRDtmp->record_data[length],field->len);
							if(diaginformix_isam)
							{
								prterr("DIAG position %d",length);
							}
							length+=field->len;
						} else {
							if(field->data.string_value!=NULL)
							{
								tlen=RDAstrlen(field->data.string_value);
							} else tlen=0;
							stchar((field->data.string_value!=NULL ? field->data.string_value:""),&NRDtmp->record_data[length],field->len);
							if(diaginformix_isam)
							{
								prterr("DIAG position %d",length);
							}
							length+=tlen+1;
						}
#endif
					} else {
						sprintf(&NRDtmp->record_data[length],"%s",(field->data.string_value!=NULL ? field->data.string_value:""));
						if(diaginformix_isam)
						{
							prterr("DIAG position %d",length);
						}
						if(field->data.string_value!=NULL)
						{
							length+=RDAstrlen(field->data.string_value)+1;
						} else length+=1;
					}
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
						if(NRDtmp->db_engine==NRDInformixIsamEngine)
						{
#ifdef INFORMIX_ISAM
							stchar((field->data.string_value!=NULL ?
								field->data.string_value:""),
								&NRDtmp->record_data[length],field->len);
#endif
						} else {
							sprintf(&NRDtmp->record_data[length],"%-*s",field->len,(field->data.string_value!=NULL ? field->data.string_value:""));
						}
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
						if(NRDtmp->db_engine==NRDInformixIsamEngine)
						{
#ifdef INFORMIX_ISAM
							stchar((field->data.string_value!=NULL ? 
								field->data.string_value:""),
								&NRDtmp->record_data[length],tlen);
#endif
						} else {
							sprintf(&NRDtmp->record_data[length],"%s",
								(field->data.string_value!=NULL ? 
								field->data.string_value:""));
						}
						if(diaginformix_isam)
						{
							prterr("DIAG position %d",length);
						}
						length+=tlen+1;
					}
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					if(NRDtmp->db_engine==NRDInformixIsamEngine)
					{
#ifdef INFORMIX_ISAM
						stdbl(*field->data.float_value,&NRDtmp->record_data[length]);
#endif
					} else {
						memcpy(&NRDtmp->record_data[length],
							field->data.float_value,sizeof(double));
					}
					if(diaginformix_isam)
					{
						prterr("DIAG position %d",length);
					}
					length+=sizeof(double);
					break;
				case SHORTV:
				case SSHORTV:
					if(NRDtmp->db_engine==NRDInformixIsamEngine)
					{
#ifdef INFORMIX_ISAM
						blah=*field->data.short_value;
						stint(blah,&NRDtmp->record_data[length]);
#endif
					} else {
						memcpy(&NRDtmp->record_data[length],
							field->data.short_value,sizeof(short));
					}
					if(diaginformix_isam)
					{
						prterr("DIAG position %d",length);
					}
					length+=sizeof(short);
					break;
				case LONGV:
				case SLONGV:
					if(NRDtmp->db_engine==NRDInformixIsamEngine)
					{
#ifdef INFORMIX_ISAM
						stlong(*field->data.integer_value,&NRDtmp->record_data[length]);
#endif
					} else {
						memcpy(&NRDtmp->record_data[length],
							field->data.integer_value,sizeof(int));
					}
					if(diaginformix_isam)
					{
						prterr("DIAG position %d",length);
					}
					length+=sizeof(int);
					break;
				case BOOLNS:
				case CHARACTERS:
					if(NRDtmp->db_engine==NRDInformixIsamEngine)
					{
#ifdef INFORMIX_ISAM
						cstring[0]=0;
						cstring[1]=0;
						cstring[0]=*field->data.string_value;
						stchar(cstring,&NRDtmp->record_data[length],1);
#endif
					} else {
						memcpy(&NRDtmp->record_data[length],
							field->data.string_value,1);
					}
					if(diaginformix_isam)
					{
						prterr("DIAG position %d",length);
					}
					length+=1;
					break;
				default:
					prterr("Unknown field type %d for %s in assignNRDRecordData",field->type,field->name);
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
							if(NRDtmp->db_engine==NRDInformixIsamEngine)
							{
#ifdef INFORMIX_ISAM
								stchar((field->data.string_value!=NULL ? field->data.string_value:""),
									&NRDtmp->record_data[length],field->len);
#endif
							} else {
								sprintf(&NRDtmp->record_data[length],"%s",(field->data.string_value!=NULL ? field->data.string_value:""));
							}
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
							if(NRDtmp->db_engine==NRDInformixIsamEngine)
							{
#ifdef INFORMIX_ISAM
								stchar((field->data.string_value!=NULL ? field->data.string_value:""),
										&NRDtmp->record_data[length],field->len);
#endif
							} else {
								if(field->len>0) /* testing to catch variable lengths */
								{
									sprintf(&NRDtmp->record_data[length],"%-*s",field->len,(field->data.string_value!=NULL ? field->data.string_value:""));
								} else {
									sprintf(&NRDtmp->record_data[length],"%s",(field->data.string_value!=NULL ? field->data.string_value:""));
								}
							}
							break;
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case SDECIMALV:
							if(NRDtmp->db_engine==NRDInformixIsamEngine)
							{
#ifdef INFORMIX_ISAM
								stdbl(*field->data.float_value,&NRDtmp->record_data[length]);
#endif
							} else {
								memcpy(&NRDtmp->record_data[length],field->data.float_value,sizeof(double));
							}
							break;
						case SHORTV:
						case SSHORTV:
							if(NRDtmp->db_engine==NRDInformixIsamEngine)
							{
#ifdef INFORMIX_ISAM
								blah=*field->data.short_value;
								stint(blah,&NRDtmp->record_data[length]);
#endif
							} else {
								memcpy(&NRDtmp->record_data[length],field->data.short_value,sizeof(short));
							}
							break;
						case LONGV:
						case SLONGV:
							if(NRDtmp->db_engine==NRDInformixIsamEngine)
							{
#ifdef INFORMIX_ISAM
								stlong(*field->data.integer_value,&NRDtmp->record_data[length]);
#endif
							} else {
								memcpy(&NRDtmp->record_data[length],field->data.integer_value,sizeof(int));
							}
							break;
						case BOOLNS:
						case CHARACTERS:
							if(NRDtmp->db_engine==NRDInformixIsamEngine)
							{
#ifdef INFORMIX_ISAM
								cstring[0]=0;
								cstring[1]=0;
								cstring[0]=*field->data.string_value;
								stchar(cstring,&NRDtmp->record_data[length],1);
#endif
							} else {
								memcpy(&NRDtmp->record_data[length],field->data.string_value,1);
							}
							break;
						default:
							prterr("Unknown field type %d for %s in assignNRDRecordData",field->type,field->name);
							break;
					}
				}
			}
		}
	}
}
#endif
#ifdef INFORMIX_ISAM
short CRTNRDisam(NRDbuffer *NRDtmp)
{
	int reclen=0,x=0,y=0,z=0;
	NRDfield *field=NULL;
	NRDkey *key;
	NRDpart *part;
	char *d=NULL,*cx=NULL;

	key=NRDtmp->key;
	if(key!=NULL)
	{
		key->KeyDesc.k_flags=ISNODUPS;
		key->KeyDesc.k_nparts=key->numparts;
		key->KeyDesc.k_rootnode=0;
		key->KeyDesc.k_len=0;
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			for(y=0,field=NRDtmp->field;y<NRDtmp->nofields;++y,++field)
			{
				if(!RDAstrcmp(part->name,field->name)) break;
			}
			if(y<NRDtmp->nofields)
			{
				key->KeyDesc.k_part[x].kp_start=part->offset;
				switch(field->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT: /* variable length field type */
						if(field->len>0) 
						{
							key->KeyDesc.k_part[x].kp_leng=field->len;
						} else {
							prterr("Error Variable Length Field [%s]  must have a defined maximum in [%s] [%s].",field->name,NRDtmp->modname,NRDtmp->name);
							return(-1);
						}
						key->KeyDesc.k_part[x].kp_type=CHARTYPE;
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
							key->KeyDesc.k_part[x].kp_leng=field->len;
						} else {
							prterr("Error Variable Length Field [%s]  must have a defined maximum in [%s] [%s].",field->name,NRDtmp->modname,NRDtmp->name);
							return(-1);
						}
						key->KeyDesc.k_part[x].kp_type=CHARTYPE;
						break;
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case SDECIMALV:
						key->KeyDesc.k_part[x].kp_leng=sizeof(double);
						key->KeyDesc.k_part[x].kp_type=DOUBLETYPE;
						break;
					case SHORTV:
					case SSHORTV:
						key->KeyDesc.k_part[x].kp_leng=sizeof(short);
						key->KeyDesc.k_part[x].kp_type=INTTYPE;
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						key->KeyDesc.k_part[x].kp_leng=sizeof(int);
						key->KeyDesc.k_part[x].kp_type=LONGTYPE;
						break;
					case BOOLNS:
					case CHARACTERS:
						key->KeyDesc.k_part[x].kp_leng=1;
						key->KeyDesc.k_part[x].kp_type=CHARTYPE;
						break;
					default:
						prterr("Error Field type %d for field %s\n",
							field->type,field->name);
						break; 
				}
			} else {
				prterr("Error Invalid File definition, part [%s] not a field. How did you get here?",part->name);
			}
			key->KeyDesc.k_len+=key->KeyDesc.k_part[x].kp_leng;
		}
		if(key->numparts>8 || key->KeyDesc.k_len>120)
		{
			prterr("Error Invalid INFORMIX C-Isam Key Definition [%d] # Parts (8 Maximum) and [%s] Byte Key Length (120 Bytes Maximum).",key->numparts,key->KeyDesc.k_len);
			return(-1);
		}
	}
	reclen=IsamRecordLength(NRDtmp);
	QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,reclen);
	memset(NRDtmp->record_data,0,NRDtmp->dlen);
	cx=stralloc(CURRENTDIRECTORY);
	d=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(NRDtmp->modname)+2);
	sprintf(d,"%s/%s",CURRENTDIRECTORY,NRDtmp->modname);
	if(RCHGCWD(d)==(-1))
	{
		prterr("Error Unable to RCHGCWD to [%s]",d);
	}
	if(d!=NULL) Rfree(d);
#ifdef NO_VARIABLETEXT_FIELDS
	if(checkvariable(NRDtmp))
	{
		isreclen=ADVIsamRecordLength(NRDtmp,FALSE);
		NRDtmp->filenum=isbuild(NRDtmp->name,reclen,&key->KeyDesc,
			ISINOUT+ISEXCLLOCK+ISVARLEN+ISNOLOG);
	} else {
		NRDtmp->filenum=isbuild(NRDtmp->name,reclen,&key->KeyDesc,
			ISINOUT+ISEXCLLOCK+ISFIXLEN+ISNOLOG);
	}
#else
		NRDtmp->filenum=isbuild(NRDtmp->name,reclen,&key->KeyDesc,
			ISINOUT+ISEXCLLOCK+ISFIXLEN+ISNOLOG);
#endif
	RCHGCWD(cx);
	if(cx!=NULL) Rfree(cx);
	if(NRDtmp->filenum<0)
	{
		prterr("Error [%d] CRTNRDisam for [%s] [%s].",iserrno,NRDtmp->modname,
			NRDtmp->name);
		return(-1);
	}
	if(NRDtmp->nokeys>1 && NRDtmp->key!=NULL)
	{
		for(z=1,key=NRDtmp->key+1;z<NRDtmp->nokeys;++z,++key)
		{
			key->KeyDesc.k_flags=ISNODUPS;
			key->KeyDesc.k_rootnode=0;
			key->KeyDesc.k_nparts=key->numparts;
			key->KeyDesc.k_len=0;
			for(x=0,part=key->part;x<key->numparts;++x,++part)
			{
				for(y=0,field=NRDtmp->field;y<NRDtmp->nofields;++y,++field)
				{
					if(!RDAstrcmp(part->name,field->name)) break;
				}
				if(y<NRDtmp->nofields)
				{
					key->KeyDesc.k_part[x].kp_start=part->offset;
					switch(field->type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT: /* variable length field type */
							if(field->len>0) 
							{
								key->KeyDesc.k_part[x].kp_leng=field->len;
							} else {
								prterr("Error Variable Length Field [%s]  must have a defined maximum in [%s] [%s].",field->name,NRDtmp->modname,NRDtmp->name);
								return(-1);
							}
							key->KeyDesc.k_part[x].kp_type=CHARTYPE;
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
								key->KeyDesc.k_part[x].kp_leng=field->len;
							} else {
								prterr("Error Variable Length Field [%s]  must have a defined maximum in [%s] [%s].",field->name,NRDtmp->modname,NRDtmp->name);
								return(-1);
							}
							key->KeyDesc.k_part[x].kp_type=CHARTYPE;
							break;
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case SDECIMALV:
							key->KeyDesc.k_part[x].kp_leng=sizeof(double);
							key->KeyDesc.k_part[x].kp_type=DOUBLETYPE;
							break;
						case SHORTV:
						case SSHORTV:
							key->KeyDesc.k_part[x].kp_leng=sizeof(short);
							key->KeyDesc.k_part[x].kp_type=INTTYPE;
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							key->KeyDesc.k_part[x].kp_leng=sizeof(int);
							key->KeyDesc.k_part[x].kp_type=LONGTYPE;
							break;
						case BOOLNS:
						case CHARACTERS:
							key->KeyDesc.k_part[x].kp_leng=1;
							key->KeyDesc.k_part[x].kp_type=CHARTYPE;
							break;
						default:
							prterr("Error Field type %d for field %s\n",
								field->type,field->name);
							break; 
					}
				} else {
					prterr("Error Invalid File definition, part [%s] not a field. How did you get here?",part->name);
				}
				key->KeyDesc.k_len+=key->KeyDesc.k_part[x].kp_leng;
			}
			if(key->numparts>8 || key->KeyDesc.k_len>120)
			{
				prterr("Error Invalid INFORMIX C-Isam Key Definition [%d] # Parts (8 Maximum) and [%s] Byte Key Length (120 Bytes Maximum).",key->numparts,key->KeyDesc.k_len);
				if(isclose(NRDtmp->filenum)<0)
				{
					prterr("Error [%d] isclose for [%s] [%s].",iserrno,
					(NRDtmp->modname!=NULL ? NRDtmp->modname:""),
					(NRDtmp->name!=NULL ? NRDtmp->name:""));
				}
				return(-1);
			} else if(isaddindex(NRDtmp->filenum,&key->KeyDesc))
			{
				prterr("Error [%d] CRTNRDisam with isaddindex.",iserrno);
				if(isclose(NRDtmp->filenum)<0)
				{
					prterr("Error [%d] isclose for [%s] [%s].",iserrno,
					(NRDtmp->modname!=NULL ? NRDtmp->modname:""),
					(NRDtmp->name!=NULL ? NRDtmp->name:""));
				}
				return(-1);
			}
		}
	}
	if(isclose(NRDtmp->filenum)<0)
	{
		prterr("Error [%d] isclose for [%s] [%s].",iserrno,
			(NRDtmp->modname!=NULL ? NRDtmp->modname:""),
			(NRDtmp->name!=NULL ? NRDtmp->name:""));
	}
	return(0);
}
short OPNNRDisam(NRDbuffer *NRDtmp)
{
	int reclen=0;
	NRDkey *key;
	NRDpart *part;
	NRDfield *field;
	int y,x,z;
	char *cx=NULL,*d=NULL;

	cx=stralloc(CURRENTDIRECTORY);
	d=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(NRDtmp->modname)+2);
	sprintf(d,"%s/%s",CURRENTDIRECTORY,NRDtmp->modname);
	if(RCHGCWD(d)==(-1))
	{
		prterr("Error Unable to RCHGCWD to [%s]",d);
	}
	if(d!=NULL) Rfree(d);
	NRDtmp->filenum=0;
#ifdef NO_VARIABLETEXT_FIELDS
	if(checkvariable(NRDtmp))
	{
		NRDtmp->filenum=isopen(NRDtmp->name,ISINOUT+ISMANULOCK+ISVARLEN);
	} else {
		NRDtmp->filenum=isopen(NRDtmp->name,ISINOUT+ISMANULOCK);
	}
#else
	NRDtmp->filenum=isopen(NRDtmp->name,ISINOUT+ISMANULOCK);
#endif
	RCHGCWD(cx);
	if(cx!=NULL) Rfree(cx);
	if(NRDtmp->filenum<0)
	{
		prterr("Error [%d] OPNNRDisam for [%s] [%s].",iserrno,
			NRDtmp->modname,NRDtmp->name);
		return(-1);
	}
	reclen=IsamRecordLength(NRDtmp);
	QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,reclen);
	memset(NRDtmp->record_data,0,NRDtmp->dlen);
	if(NRDtmp->key!=NULL)
	{
		for(z=0,key=NRDtmp->key;z<NRDtmp->nokeys;++z,++key)
		{
			key->KeyDesc.k_flags=ISNODUPS;
			key->KeyDesc.k_rootnode=0;
			key->KeyDesc.k_nparts=key->numparts;
			key->KeyDesc.k_len=0;
			for(x=0,part=key->part;x<key->numparts;++x,++part)
			{
				for(y=0,field=NRDtmp->field;y<NRDtmp->nofields;++y,++field)
				{
					if(!RDAstrcmp(part->name,field->name)) break;
				}
				if(y<NRDtmp->nofields)
				{
					key->KeyDesc.k_part[x].kp_start=part->offset;
					switch(field->type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT: /* variable length field type */
							key->KeyDesc.k_part[x].kp_leng=field->len;
							key->KeyDesc.k_part[x].kp_type=CHARTYPE;
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
							key->KeyDesc.k_part[x].kp_leng=field->len;
							key->KeyDesc.k_part[x].kp_type=CHARTYPE;
							break;
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case SDECIMALV:
							key->KeyDesc.k_part[x].kp_leng=sizeof(double);
							key->KeyDesc.k_part[x].kp_type=DOUBLETYPE;
							break;
						case SHORTV:
						case SSHORTV:
							key->KeyDesc.k_part[x].kp_leng=sizeof(short);
							key->KeyDesc.k_part[x].kp_type=INTTYPE;
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							key->KeyDesc.k_part[x].kp_leng=sizeof(int);
							key->KeyDesc.k_part[x].kp_type=LONGTYPE;
							break;
						case BOOLNS:
						case CHARACTERS:
							key->KeyDesc.k_part[x].kp_leng=1;
							key->KeyDesc.k_part[x].kp_type=CHARTYPE;
							break;
						default:
							prterr("Error Field type %d for field %s\n",
								field->type,field->name);
							break; 
					}
				} else {
					prterr("Error Invalid File definition. How did you get here?");
				}
				key->KeyDesc.k_len+=key->KeyDesc.k_part[x].kp_leng;
			}
		}
	}
	return(0);
}
short OPNEXCLCKNRDisam(NRDbuffer *NRDtmp)
{
	int reclen=0;
	NRDkey *key;
	NRDpart *part;
	NRDfield *field;
	int y,x,z;
	char *cx=NULL,*d=NULL;

	cx=stralloc(CURRENTDIRECTORY);
	d=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(NRDtmp->modname)+2);
	sprintf(d,"%s/%s",CURRENTDIRECTORY,NRDtmp->modname);
	if(RCHGCWD(d)==(-1))
	{
		prterr("Error Unable to RCHGCWD to [%s]",d);
	}
	if(d!=NULL) Rfree(d);
	NRDtmp->filenum=0;
#ifdef NO_VARIABLETEXT_FIELDS
	if(checkvariable(NRDtmp))
	{
		NRDtmp->filenum=isopen(NRDtmp->name,ISEXCLLOCK+ISINOUT+ISVARLEN);
	} else {
		NRDtmp->filenum=isopen(NRDtmp->name,ISEXCLLOCK+ISINOUT);
	}
#else
	NRDtmp->filenum=isopen(NRDtmp->name,ISEXCLLOCK+ISINOUT);
#endif
	RCHGCWD(cx);
	if(cx!=NULL) Rfree(cx);
	if(NRDtmp->filenum<0)
	{
		prterr("Error [%d] OPNEXCLCKNRDisam for [%s] [%s].",iserrno,
			NRDtmp->modname,NRDtmp->name);
		return(-1);
	}
	reclen=IsamRecordLength(NRDtmp);
	QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,reclen);
	memset(NRDtmp->record_data,0,NRDtmp->dlen);
	if(NRDtmp->key!=NULL)
	{
		for(z=0,key=NRDtmp->key;z<NRDtmp->nokeys;++z,++key)
		{
			key->KeyDesc.k_flags=ISNODUPS;
			key->KeyDesc.k_rootnode=0;
			key->KeyDesc.k_nparts=key->numparts;
			key->KeyDesc.k_len=0;
			for(x=0,part=key->part;x<key->numparts;++x,++part)
			{
				for(y=0,field=NRDtmp->field;y<NRDtmp->nofields;++y,++field)
				{
					if(!RDAstrcmp(part->name,field->name)) break;
				}
				if(y<NRDtmp->nofields)
				{
					key->KeyDesc.k_part[x].kp_start=part->offset;
					switch(field->type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT: /* variable length field type */
							key->KeyDesc.k_part[x].kp_leng=field->len;
							key->KeyDesc.k_part[x].kp_type=CHARTYPE;
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
							key->KeyDesc.k_part[x].kp_leng=field->len;
							key->KeyDesc.k_part[x].kp_type=CHARTYPE;
							break;
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case SDECIMALV:
							key->KeyDesc.k_part[x].kp_leng=sizeof(double);
							key->KeyDesc.k_part[x].kp_type=DOUBLETYPE;
							break;
						case SHORTV:
						case SSHORTV:
							key->KeyDesc.k_part[x].kp_leng=sizeof(short);
							key->KeyDesc.k_part[x].kp_type=INTTYPE;
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							key->KeyDesc.k_part[x].kp_leng=sizeof(int);
							key->KeyDesc.k_part[x].kp_type=LONGTYPE;
							break;
						case BOOLNS:
						case CHARACTERS:
							key->KeyDesc.k_part[x].kp_leng=1;
							key->KeyDesc.k_part[x].kp_type=CHARTYPE;
							break;
						default:
							prterr("Error Field type %d for field %s\n",
								field->type,field->name);
							break; 
					}
				} else {
					prterr("Error Invalid File definition. How did you get here?");
				}
				key->KeyDesc.k_len+=key->KeyDesc.k_part[x].kp_leng;
			}
		}
	}
	return(0);
}
short CLSNRDisam(short fileno)
{
	NRDbuffer *NRDtmp=NULL;

	NRDtmp=NRDbuf+fileno;
	if(isclose(NRDtmp->filenum)==(-1))
	{
		prterr("Error [%d] CLSNRDisam for [%s] [%s].",iserrno,
			(NRDtmp->modname!=NULL ? NRDtmp->modname:""),
			(NRDtmp->name!=NULL ? NRDtmp->name:""));
		return(-1);
	} 
	return(0);
}
short LOCNRDisam(short fileno)
{
	NRDbuffer *NRDtmp=NULL;

	NRDtmp=NRDbuf+fileno;
	if(isread(NRDtmp->filenum,NRDtmp->record_data,ISLCKW)==(-1))
	{
		prterr("Error [%d] UNLNRDisam for [%s] [%s].",iserrno,NRDtmp->modname,
			NRDtmp->name);
		return(-1);
	} 
	return(0);
}
short UNLNRDisam(short fileno)
{
	NRDbuffer *NRDtmp=NULL;

	NRDtmp=NRDbuf+fileno;
	if(isrelease(NRDtmp->filenum)==(-1))
	{
		prterr("Error [%d] UNLNRDisam for [%s] [%s].",iserrno,NRDtmp->modname,
			NRDtmp->name);
		return(-1);
	} 
	return(0);
}
#define LOCK_WAIT	1000
short LOCNRDFILEisam(short fileno)
{
	NRDbuffer *NRDtmp=NULL;
	short retval=FALSE;
	int x=0;

	NRDtmp=NRDbuf+fileno;
	while(TRUE)
	{
		retval=islock(NRDtmp->filenum);
		if(retval!=(-1))
		{
			break;
		} else if((iserrno!=(-107)) && (iserrno!=(107)))
		{
			prterr("Error [%d] LOCNRDFILEisam for [%s] [%s].",iserrno,NRDtmp->modname,NRDtmp->name);
			return(-1);
		} 
		x=0;
		while(x<LOCK_WAIT) ++x;
	}
	return(0);
}
short UNLNRDFILEisam(short fileno)
{
	NRDbuffer *NRDtmp=NULL;

	NRDtmp=NRDbuf+fileno;
	if(isunlock(NRDtmp->filenum)==(-1))
	{
		prterr("Error [%d] UNLNRDisam for [%s] [%s].",iserrno,NRDtmp->modname,
			NRDtmp->name);
		return(-1);
	} 
	return(0);
}
short INITNRDisam(void)
{
	return(0);
}
short EQLNRDKEYisam(short fileno,short keyno,void *target)
{
	NRDbuffer *NRDtmp=NULL;
	struct keydesc KeyDesc;
	
	NRDtmp=NRDbuf+fileno;
	KeyDesc.k_nparts=0;
	if(target!=NULL) isrecnum=*(long *)target;
		else isrecnum=0;
	if(isstart(NRDtmp->filenum,&KeyDesc,0,NULL,ISEQUAL)<0)
	{
		if(iserrno!=110 && iserrno!=111)/* Not End of file or no rec */
		{
			prterr("Error [%d] with isstart in EQLNRDKEYisam.",iserrno);
		}
		return(-1);
	}
	if(isread(NRDtmp->filenum,NRDtmp->record_data,ISEQUAL)<0)
	{
		prterr("Error [%d] with isread in EQLNRDKEYisam.",iserrno);
		return(-1);
	} else {
		NRDtmp->currec=isrecnum;
		assignIsam_fields(NRDtmp);
	}
	return(0);
}
short EQLNRDisam(short fileno,short keyno)
{
	NRDbuffer *NRDtmp=NULL;
	NRDkey *key;
	short ikeyno=1;
	
	NRDtmp=NRDbuf+fileno;
	if(keyno>0 && keyno<=NRDtmp->nokeys)
	{
		ikeyno=keyno;
	} else ikeyno=1;
	key=NRDtmp->key+(ikeyno-1);
	assignNRDRecordData(NRDtmp,FALSE);
	if(isstart(NRDtmp->filenum,&key->KeyDesc,0,NRDtmp->record_data,ISEQUAL)<0)
	{
		if(iserrno!=110 && iserrno!=111)/* Not End of file or no rec */
		{
			prterr("Error [%d] with isstart in EQLNRDisam.",iserrno);
		}
		return(-1);
	}
	if(isread(NRDtmp->filenum,NRDtmp->record_data,ISEQUAL)<0)
	{
		if(iserrno!=110 && iserrno!=111)/* Not End of file or no rec */
		{
			prterr("Error [%d] with isread in EQLNRDisam.",iserrno);
		}
		return(-1);
	} else {
		NRDtmp->currec=isrecnum;
		assignIsam_fields(NRDtmp);
	}
	return(0);
}
short GTENRDisam(short fileno,short keyno)
{
	NRDbuffer *NRDtmp=NULL;
	NRDkey *key;
	short ikeyno=1;
	
	NRDtmp=NRDbuf+fileno;
	if(keyno>0 && keyno<=NRDtmp->nokeys)
	{
		ikeyno=keyno;
	} else ikeyno=1;
	key=NRDtmp->key+(ikeyno-1);
	assignNRDRecordData(NRDtmp,FALSE);
	if(isstart(NRDtmp->filenum,&key->KeyDesc,0,NRDtmp->record_data,ISGTEQ)<0)
	{
		if(iserrno!=110 && iserrno!=111)/* Not End of file or no rec */
		{
			prterr("Error [%d] with isstart in GTENRDisam.",iserrno);
		}
		return(-1);
	}
	if(isread(NRDtmp->filenum,NRDtmp->record_data,ISGTEQ)<0)
	{
		if(iserrno!=110 && iserrno!=111)/* Not End of file or no rec */
		{
			prterr("Error [%d] with isread in GTENRDisam.",iserrno);
		}
		return(-1);
	} else {
		NRDtmp->currec=isrecnum;
		assignIsam_fields(NRDtmp);
	}
	return(0);
}
short PRVNRDisam(short fileno,short keyno)
{
	NRDbuffer *NRDtmp=NULL;
	NRDkey *key;
	short ikeyno=1;
	int mode=ISPREV;
	
	NRDtmp=NRDbuf+fileno;
	if(keyno>0 && keyno<=NRDtmp->nokeys)
	{
		ikeyno=keyno;
	} else ikeyno=1;
	key=NRDtmp->key+(ikeyno-1);
	assignNRDRecordData(NRDtmp,FALSE);
	if(!isstart(NRDtmp->filenum,&key->KeyDesc,0,NRDtmp->record_data,ISEQUAL))
	{
		mode=ISEQUAL;
	} else if(!isstart(NRDtmp->filenum,&key->KeyDesc,0,NRDtmp->record_data,ISGTEQ))
	{
		mode=ISPREV;
	} else {
		if(iserrno!=110 && iserrno!=111)/* Not End of file or no rec */
		{
			prterr("Error [%d] with isstart in PRVNRDisam.",iserrno);
		}
		return(-1);
	}
	if(isread(NRDtmp->filenum,NRDtmp->record_data,mode)<0)
	{
		if(iserrno!=110 && iserrno!=111)/* Not End of file or no rec */
		{
			prterr("Error [%d] with isread in PRVNRDisam.",iserrno);
		}
		return(-1);
	}
	mode=ISPREV;
	if(isread(NRDtmp->filenum,NRDtmp->record_data,mode)<0)
	{
		if(iserrno!=110 && iserrno!=111)/* Not End of file or no rec */
		{
			prterr("Error [%d] with isread in PRVNRDisam.",iserrno);
		}
		return(-1);
	} else {
		NRDtmp->currec=isrecnum;
		assignIsam_fields(NRDtmp);
	}
	return(0);
}
short LSTNRDisam(short fileno,short keyno)
{
	NRDbuffer *NRDtmp=NULL;
	NRDkey *key=NULL;
	short ikeyno=1;
	
	NRDtmp=NRDbuf+fileno;
	if(keyno>0 && keyno<=NRDtmp->nokeys)
	{
		ikeyno=keyno;
	} else ikeyno=1;
	key=NRDtmp->key+(ikeyno-1);
	isrecnum=0;
	if(isstart(NRDtmp->filenum,&key->KeyDesc,0,NRDtmp->record_data,ISLAST)<0)
	{
		if(iserrno!=110 && iserrno!=111)/* Not End of file or no rec */
		{
			prterr("Error [%d] with isstart in LSTNRDisam.",iserrno);
		}
		return(-1);
	}
	if(isread(NRDtmp->filenum,NRDtmp->record_data,ISLAST)<0)
	{
		prterr("Error [%d] with isread in LSTNRDisam.",iserrno);
		return(-1);
	} else {
		NRDtmp->currec=isrecnum;
		assignIsam_fields(NRDtmp);
	}
	return(0);
}
short BEGNRDisam(short fileno)
{
	NRDbuffer *NRDtmp=NULL;
	struct keydesc KeyDesc;
	
	NRDtmp=NRDbuf+fileno;
	KeyDesc.k_nparts=0;
	isrecnum=0;
	if(isstart(NRDtmp->filenum,&KeyDesc,0,NULL,ISFIRST)<0)
	{
		if(iserrno!=110 && iserrno!=111)/* Not End of file or no rec */
		{
			prterr("Error [%d] with isstart in BEGNRDisam.",iserrno);
		}
		return(-1);
	}
	if(isread(NRDtmp->filenum,NRDtmp->record_data,ISFIRST)<0)
	{
		if(iserrno!=110 && iserrno!=111)/*Not End of file or no rec*/
		{
			prterr("Error [%d] with isread in BEGNRDisam.",iserrno);
		}
		return(-1);
	} else {
		NRDtmp->currec=isrecnum;
		assignIsam_fields(NRDtmp);
	}
	return(0);
}
short SEQNRDisam(short fileno)
{
	NRDbuffer *NRDtmp=NULL;
	struct keydesc KeyDesc;
	
	NRDtmp=NRDbuf+fileno;
	KeyDesc.k_nparts=0;
	assignNRDRecordData(NRDtmp,FALSE);
	isrecnum=NRDtmp->currec;
	if(isstart(NRDtmp->filenum,&KeyDesc,0,NULL,ISEQUAL)<0)
	{
		if(iserrno!=110 && iserrno!=111)/* Not End of file or no rec */
		{
			prterr("Error [%d] with isstart in SEQNRDisam.",iserrno);
		}
		return(-1);
	}
	if(isread(NRDtmp->filenum,NRDtmp->record_data,ISEQUAL)<0)
	{
		if(iserrno!=110 && iserrno!=111)/* Not End of file or no rec */
		{
			prterr("Error [%d] with isread(ISCURR) in SEQNRDisam.",iserrno);
		}
		return(-1);
	}
	if(isread(NRDtmp->filenum,NRDtmp->record_data,ISNEXT)<0)
	{
		if(iserrno!=110 && iserrno!=111)/* Not End of file or no rec */
		{
			prterr("Error [%d] with isread in SEQNRDisam.",iserrno);
		}
		return(-1);
	} else {
		NRDtmp->currec=isrecnum;
		assignIsam_fields(NRDtmp);
	}
	return(0);
}
short LTENRDisam(short fileno,short keyno)
{
	NRDbuffer *NRDtmp=NULL;
	NRDkey *key;
	short ikeyno=1;
	int mode=ISEQUAL;
	
	NRDtmp=NRDbuf+fileno;
	if(keyno>0 && keyno<=NRDtmp->nokeys)
	{
		ikeyno=keyno;
	} else ikeyno=1;
	key=NRDtmp->key+(ikeyno-1);
	assignNRDRecordData(NRDtmp,FALSE);
	if(!isstart(NRDtmp->filenum,&key->KeyDesc,0,NRDtmp->record_data,ISEQUAL))
	{
		mode=ISEQUAL;
	} else if(!isstart(NRDtmp->filenum,&key->KeyDesc,0,NRDtmp->record_data,
		ISGTEQ))
	{
		mode=ISGTEQ;
		if(isread(NRDtmp->filenum,NRDtmp->record_data,mode)<0)
		{
			if(iserrno!=110 && iserrno!=111)/* Not End of file or no rec */
			{
				prterr("Error [%d] with isread in LTENRDisam.",iserrno);
			}
			return(-1);
		}
		mode=ISPREV;
	} else if(!isstart(NRDtmp->filenum,&key->KeyDesc,0,NRDtmp->record_data,ISLAST))
	{
		mode=ISLAST;
	} else {
		if(iserrno!=110 && iserrno!=111)/* Not End of file or no rec */
		{
			prterr("Error [%d] with isstart in LSTNRDisam.",iserrno);
		}
		return(-1);
	}
	if(!isread(NRDtmp->filenum,NRDtmp->record_data,mode))
	{
		NRDtmp->currec=isrecnum;
		assignIsam_fields(NRDtmp);
		return(0);
	} else if(iserrno==101 && iserrno==100 && diaginformix_isam)
	{
		prterr("DIAG Record not found.");
		return(1);
	}
	prterr("Error [%d] from isread in LTENRDisam.",iserrno);
	return(-1);
}
short FRSNRDisam(short fileno,short keyno)
{
	NRDbuffer *NRDtmp=NULL;
	NRDkey *key;
	short ikeyno=1;
	
	if(diaginformix_isam)
	{
		prterr("DIAG FRSNRDisam.");
	}
	NRDtmp=NRDbuf+fileno;
	if(keyno>0 && keyno<=NRDtmp->nokeys)
	{
		ikeyno=keyno;
	} else ikeyno=1;
	key=NRDtmp->key+(ikeyno-1);
	memset(NRDtmp->record_data,0,NRDtmp->dlen);
	if(isstart(NRDtmp->filenum,&key->KeyDesc,0,NRDtmp->record_data,ISFIRST)<0)
	{
		if(iserrno!=110 && iserrno!=111)/* Not End of file or no rec */
		{
			prterr("Error [%d] with isstart in FRSNRDisam.",iserrno);
		}
		return(-1);
	}
	if(isread(NRDtmp->filenum,NRDtmp->record_data,ISFIRST)<0)
	{
		if(iserrno!=110 && iserrno!=111)/* Not End of file or no rec */
		{
			prterr("Error [%d] with isread in FRSNRDisam.",iserrno);
		}
		return(-1);
	} else {
		NRDtmp->currec=isrecnum;
		assignIsam_fields(NRDtmp);
	}
	return(0);
}
short NXTNRDisam(short fileno,short keyno)
{
	NRDbuffer *NRDtmp=NULL;
	NRDkey *key=NULL;
	short ikeyno=1;
	int mode=ISEQUAL;
	
	NRDtmp=NRDbuf+fileno;
	if(keyno>0 && keyno<=NRDtmp->nokeys)
	{
		ikeyno=keyno;
	} else ikeyno=1;
	key=(NRDtmp->key+(ikeyno-1));
	assignNRDRecordData(NRDtmp,FALSE);
	if(!isstart(NRDtmp->filenum,&key->KeyDesc,0,NRDtmp->record_data,ISEQUAL))
	{
		mode=ISEQUAL;
		if(isread(NRDtmp->filenum,NRDtmp->record_data,mode)<0)
		{
			if(iserrno!=110 && iserrno!=111)/* Not End of file or no rec */
			{
				prterr("Error [%d] with isread in NXTNRDisam.",iserrno);
			}
			return(-1);
		}
		mode=ISNEXT;
	} else if(!isstart(NRDtmp->filenum,&key->KeyDesc,0,NRDtmp->record_data,
			ISGTEQ))
	{
		mode=ISGTEQ;
	} else {
		if(iserrno!=110 && iserrno!=111)/* Not End of file or no rec */
		{
			prterr("Error [%d] with isread in NXTNRDisam.",iserrno);
		}
		return(-1);
	}
	if(isread(NRDtmp->filenum,NRDtmp->record_data,mode)<0)
	{
		if(iserrno!=110 && iserrno!=111)/* Not End of file or no rec */
		{
			prterr("Error [%d] with isread in NXTNRDisam.",iserrno);
		}
		return(-1);
	} else {	
		NRDtmp->currec=isrecnum;
		assignIsam_fields(NRDtmp);
	}
	return(0);
}
long SIZNRDisam(short fileno)
{
	NRDbuffer *NRDtmp=NULL;
	struct dictinfo FileInfo;
	long leng=0;

	NRDtmp=NRDbuf+fileno;
	if(isindexinfo(NRDtmp->filenum,&FileInfo,0)<0)
	{
		prterr("Error [%d] from isindexinfo in SIZNRDisam.",iserrno);
	} else leng=FileInfo.di_nrecords;
	return(leng);
}
short DELNRDisam(short fileno)
{
	NRDbuffer *NRDtmp=NULL;

	NRDtmp=NRDbuf+fileno;
	if(isdelcurr(NRDtmp->filenum)==(-1))
	{
		prterr("Error [%d] UNLNRDisam for [%s] [%s].",iserrno,NRDtmp->modname,
			NRDtmp->name);
		return(-1);
	} 
	return(0);
}
short WRTNRDisam(short fileno,short trnmode,char *fieldname)
{
	NRDbuffer *NRDtmp=NULL;
	NRDfield *field;
	short return_value=0;
	int length=0;
	char *temp=NULL;
	NRDkey *key=NULL;

	if(diaginformix_isam)
	{
		prterr("DIAG WRTNRDisam Writing Informix Isam Database Record in Module [%s] File [%s] Trnmode [%d] Fieldname [%s].",MODULENAME(fileno),FILENAME(fileno),trnmode,(fieldname!=NULL ? fieldname:""));
	}
	NRDtmp=NRDbuf+fileno;
	length=IsamRecordLength(NRDtmp);
	if(diaginformix_isam)
	{
		prterr("DIAG Length of data record [%d]",length);
	}
	if(!isEMPTY(fieldname))
	{
		field=xFLDNRD(fileno,fieldname,__LINE__,__FILE__);
		if(field->type!=SCROLLEDLIST && field->type!=LONGV && 
			field->type!=SLONGV && field->type!=SHORTV &&
			field->type!=SSHORTV && field->type!=DOUBLEV &&
			field->type!=SDOUBLEV) field=NULL;
	} else field=NULL;
	isreclen=length;
	if(trnmode && field!=NULL)/*get next tran num automatically*/
	{
		SetTrnModeField(fileno,fieldname);
		assignNRDRecordData(NRDtmp,TRUE);
		if(!islock(NRDtmp->filenum))
		{
			return_value=iswrcurr(NRDtmp->filenum,NRDtmp->record_data);
			if(return_value<0)
			{
				prterr("Error [%d] from iswrcurr in WRTNRDisam.",iserrno);
			} else {
				NRDtmp->currec=isrecnum;
				isrelease(NRDtmp->filenum);
			}
			if(isunlock(NRDtmp->filenum)<0)
			{
				prterr("Error [%d] in isunlock of WRTNRDisam.",iserrno);
			}
		} else {
			prterr("Error [%d] in islock of WRTNRDisam.",iserrno);
			return_value=(-1);
		}
	} else {
		assignNRDRecordData(NRDtmp,TRUE);
		temp=Rmalloc(NRDtmp->dlen);
		memcpy(temp,NRDtmp->record_data,NRDtmp->dlen);
		key=NRDtmp->key;
		if(!isstart(NRDtmp->filenum,&key->KeyDesc,0,temp,ISEQUAL))
		{
			if(isread(NRDtmp->filenum,temp,ISEQUAL+ISLCKW)<0)
			{
				if(temp!=NULL) Rfree(temp);
				if(!islock(NRDtmp->filenum))
				{
					return_value=iswrcurr(NRDtmp->filenum,NRDtmp->record_data);
					if(return_value<0)
					{
						prterr("Error [%d] from iswrcurr in WRTNRDisam.",iserrno);
					} else {
						NRDtmp->currec=isrecnum;
						isrelease(NRDtmp->filenum);
					}
					if(isunlock(NRDtmp->filenum)<0)
					{
						prterr("Error [%d] in isunlock of WRTNRDisam.",iserrno);
					}
				} else {
					prterr("Error [%d] in islock of WRTNRDisam.",iserrno);
					return_value=(-1);
				}
			} else {
				if(temp!=NULL) Rfree(temp);
				if(!islock(NRDtmp->filenum))
				{
					
					return_value=isrewcurr(NRDtmp->filenum,NRDtmp->record_data);
					if(return_value<0)
					{
						prterr("Error [%d] from isrewcurr in WRTNRDisam.",iserrno);
					} else {
						NRDtmp->currec=isrecnum;
						isrelease(NRDtmp->filenum);
					}
#ifdef FLUSH_ME_
					isflush(NRDtmp->filenum);
#endif /* FLUSH_ME_ */
					if(isunlock(NRDtmp->filenum)<0)
					{
						prterr("Error [%d] in isunlock of WRTNRDisam.",iserrno);
					}
				} else {
					prterr("Error [%d] in islock of WRTNRDisam.",iserrno);
					return_value=(-1);
				}
			}
		} else {
			if(temp!=NULL) Rfree(temp);
			if(!islock(NRDtmp->filenum))
			{
				return_value=iswrcurr(NRDtmp->filenum,NRDtmp->record_data);
				if(return_value<0)
				{
					prterr("Error [%d] from iswrcurr in WRTNRDisam.",iserrno);
				} else {
					NRDtmp->currec=isrecnum;
					isrelease(NRDtmp->filenum);
				}
#ifdef FLUSH_ME_
				isflush(NRDtmp->filenum);
#endif /* FLUSH_ME_ */
				if(isunlock(NRDtmp->filenum)<0)
				{
					prterr("Error [%d] in isunlock of WRTNRDisam.",iserrno);
				}
			} else {
				prterr("Error [%d] in islock of WRTNRDisam.",iserrno);
				return_value=(-1);
			}
		}
	}
	return(return_value);
}
short RBDNRDisam(char *modx,char *filx)
{
	char *cmd=NULL;
	short x=FALSE;

	cmd=Rmalloc(RDAstrlen(modx)+RDAstrlen(filx)+14);
	sprintf(cmd,"bcheck -y ./%s/%s",modx,filx);
	x=system_command(cmd);
	if(cmd!=NULL) Rfree(cmd);
	return(x);
}	
short DELETENRDisam(char *modx,char *filx)
{
	char *cx=NULL,*d=NULL;
	short return_value=0;

	cx=stralloc(CURRENTDIRECTORY);
	d=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modx)+2);
	sprintf(d,"%s/%s",CURRENTDIRECTORY,modx);
	if(RCHGCWD(d)==(-1))
	{
		prterr("Error Unable to RCHGCWD to [%s]",d);
	}
	if(d!=NULL) Rfree(d);
	if(iserase(filx))
	{
		prterr("Error [%d] on DELETENRDisam for [%s] [%s].",iserrno,
			modx,filx);
		return_value=(-1);
	}
	RCHGCWD(cx);
	if(cx!=NULL) Rfree(cx);
	return(return_value);
}
#endif
/*lint +e525*/
