/*lint -library*/
/*lint -e525*/
#ifdef USE_MYISAM
#include <nrdp.hpp>
/* define MyISAMSTORE  */

#ifdef USE_MARIADB
#include "mysql/private/myisam.h"
#include "mysql/private/myisam/myisamdef.h"
#include "mysql/private/my_base.h"
#else
#include "myisam.h"
#include "myisamdef.h"
#include "my_base.h"
#endif
#ifdef WIN32
char **environ=NULL;
#endif
HA_KEYSEG glob_segs[16][16];
ulong key_cache_size=IO_SIZE*16;
uint key_cache_block_size= KEY_CACHE_BLOCK_SIZE;
#define RD_HEADER	1

#ifdef _MY_NOTES_
/* mi_rkey movements */
  HA_READ_KEY_EXACT,              /* Find first record else error */
  HA_READ_KEY_OR_NEXT,            /* Record or next record */
  HA_READ_KEY_OR_PREV,            /* Record or previous */
  HA_READ_AFTER_KEY,              /* Find next rec. after key-record */
  HA_READ_BEFORE_KEY,             /* Find next rec. before key-record */

 #endif /* MY_NOTES */

void assignMyIsam_fields(NRDbuffer *NRDtmp)
{
	short pos=1,x;
	int length=0;
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG assignIsam_fields Assigning Current MyIsam Record in Module [%s] File [%s] to Database Engine.",NRDtmp->modname,NRDtmp->name); }
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
					if(field->len>field->dlen)
					{
						/* prterr("\nlen greater [%d] than  dlen [%d] \nstring.value [%s]\n\n",field->len,field->dlen,field->data.string_value); */
						QUICKALLOC(field->data.string_value,field->dlen,field->len+1);
						memset(field->data.string_value,0,field->dlen);
						/* prterr("\nlen  [%d]  dlen [%d]\nstring.value [%s]\n\n",field->len,field->dlen,field->data.string_value); */
					}else{
						QUICKALLOC(field->data.string_value,field->dlen,field->len+1);
						memset(field->data.string_value,0,field->dlen);
						/* prterr("\ndlen greater [%d] than  len [%d] \nstring.value [%s]\n\n",field->dlen,field->len,field->data.string_value); */
					}
					memcpy(field->data.string_value,&NRDtmp->record_data[pos],field->len);
					field->data.string_value[field->len]=0;
					unpad(field->data.string_value);
					pos+=field->len;
				} else {
					length=0;
#ifdef MyISAMSTORE
					longget(length,&NRDtmp->record_data[pos]);
#endif /* MyISAMSTORE */
#ifndef MyISAMSTORE
					memcpy(&length,&NRDtmp->record_data[pos],sizeof(int));
#endif /* MyISAMSTORE */
					pos+=4;
					if(length)
					{
						QUICKALLOC(field->data.string_value,field->dlen,length+1);
						memset(field->data.string_value,0,field->dlen);
						memcpy(field->data.string_value,&NRDtmp->record_data[pos],length);
						unpad(field->data.string_value);
						pos+=length;
					} else if(field->data.string_value!=NULL)
					{
						memset(field->data.string_value,0,field->dlen);
					}
				}
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
#ifdef MyISAMSTORE
				doubleget(*field->data.float_value,&NRDtmp->record_data[pos]);
#endif /* MyISAMSTORE */
#ifndef MyISAMSTORE
				memcpy(field->data.float_value,&NRDtmp->record_data[pos],sizeof(double));
#endif /* MyISAMSTORE */
				pos+=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
#ifdef MyISAMSTORE
				shortget(*field->data.short_value,&NRDtmp->record_data[pos]);
#endif /* MyISAMSTORE */
#ifndef MyISAMSTORE
				memcpy(field->data.short_value,&NRDtmp->record_data[pos],sizeof(short));
#endif /* MyISAMSTORE */
				pos+=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
#ifdef MyISAMSTORE
				longget(*field->data.integer_value,&NRDtmp->record_data[pos]);
#endif /* MyISAMSTORE */
#ifndef MyISAMSTORE
				memcpy(field->data.integer_value,&NRDtmp->record_data[pos],sizeof(int));
#endif /* MyISAMSTORE */
				pos+=sizeof(int);
				break;
			case BOOLNS:
				if(NRDtmp->record_data[pos]=='1') *field->data.string_value=TRUE;
					else *field->data.string_value=FALSE;
					
				pos+=1;
				break;
			case CHARACTERS:
				*field->data.string_value=NRDtmp->record_data[pos];
				pos+=1;
				break;
			default:
				prterr("Error Field Type [%d] is invalid for Field [%s] in Module [%s] File [%s].",field->type,field->name,NRDtmp->modname,NRDtmp->name);
				break;
		}
		if(diagmyisam) 
		{
			SEENRDFIELD(field);TRACE;
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG All Fields Assigned in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
}
#define MyIsamKeyLength(a,b)	xMyIsamKeyLength(a,b,__LINE__,__FILE__)
int xMyIsamKeyLength(NRDbuffer *NRDtmp,short keyno,int line,char *file)
{
	int x,length=0;
	NRDfield *field=NULL;
	NRDpart *part=NULL;
	NRDkey *key=NULL;
	short ikeyno=1;

	if(diagmyisam)
	{
		prterr("DIAG MyIsamKeyLength at line [%d] program [%s].",line,file);
	}
	length=0;
	if(NRDtmp!=NULL)
	{
		if(keyno>0 && keyno<=NRDtmp->nokeys)
		{
			ikeyno=keyno;
		} else ikeyno=1;
		key=NRDtmp->key+(ikeyno-1);	
		if(NRDtmp->varfile==FALSE) return(key->length);
		if(key!=NULL)
		{
			for(x=0,part=key->part;x<key->numparts;++x,++part)
			{
				field=FINDFLD(NRDtmp,part->name);
				switch(field->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT: /* variable length field type */
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
						length+=field->len;
						if(field->len==0) 
						{
							length+=field->dlen-1+4;
							NRDtmp->varfile=TRUE;
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
	if(diagmyisam)
	{
		prterr("DIAG MyIsamKeyLength Returning [%d] at line [%d] program [%s].",length,line,file);
	}
	return(length);
}
int xMyIsamRecordLength(NRDbuffer *NRDtmp,short varls,int line,char *file)
{
	int x,length=RD_HEADER;
	NRDfield *field;

	if(diagmyisam)
	{
		prterr("DIAG MyIsamRecordLength at line [%d] program [%s].",line,file);
		varls=0;
	}
	if(NRDtmp->varfile==FALSE && NRDtmp->dlen>0) return(NRDtmp->dlen-1);
	length=RD_HEADER*2;
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
						if(field->len==0)
						{
							length+=field->dlen-1+4;
							NRDtmp->varfile=TRUE;
						} else length+=field->len;
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
	if(diagmyisam)
	{
		prterr("DIAG MyIsamRecordLength Returning [%d] at line [%d] program [%s].",length,line,file);
	}
	return(length);
}
void assignMyIsamRecordData(NRDbuffer *NRDtmp,short doall)
{
	int x,y,length=0,tlen=0;
	NRDfield *field;
	NRDkey *key;
	NRDpart *part;

	length=ADVMyIsamRecordLength(NRDtmp,TRUE);
	QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,length+1);
	memset(NRDtmp->record_data,0,NRDtmp->dlen);
	length=RD_HEADER;
	if(length>0)
	{
		NRDtmp->record_data[0]=250;
	}
	if(doall)
	{
		for(x=0,field=NRDtmp->field;x<NRDtmp->nofields;++x,++field)
		{
			if(diagmyisam) 
			{
				SEENRDFIELD(field);TRACE;
			}
			switch(field->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT: /* variable length field type */
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
					tlen=RDAstrlen(field->data.string_value);
					if(field->len>0) /* testing to catch variable lengths */
					{
						sprintf(&NRDtmp->record_data[length],"%-*s",field->len,
							(field->data.string_value!=NULL ? 
							field->data.string_value:""));
						if(diagmyisam)
						{
							prterr("DIAG position %d",length);
						}
						length+=field->len;
					} else {
#ifdef MyISAMSTORE
						longstore(&NRDtmp->record_data[length],tlen);
#endif /* MyISAMSTORE */
#ifndef MyISAMSTORE
						memcpy(&NRDtmp->record_data[length],&tlen,sizeof(int));
#endif /* MyISAMSTORE */
						length+=4;
						sprintf(&NRDtmp->record_data[length],"%-s",(field->data.string_value!=NULL ? field->data.string_value:""));
						if(diagmyisam)
						{
							prterr("DIAG position %d",length);
						}
						length+=tlen;
					}
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
#ifdef MyISAMSTORE
					doublestore(&NRDtmp->record_data[length],*field->data.float_value);
#endif /* MyISAMSTORE */
#ifndef MyISAMSTORE
					memcpy(&NRDtmp->record_data[length],field->data.float_value,sizeof(double));
#endif /* MyISAMSTORE */
					if(diagmyisam)
					{
						prterr("DIAG position %d",length);
					}
					length+=sizeof(double);
					break;
				case SHORTV:
				case SSHORTV:
#ifdef MyISAMSTORE
					shortstore(&NRDtmp->record_data[length],*field->data.short_value);
#endif /* MyISAMSTORE */
#ifndef MyISAMSTORE
					memcpy(&NRDtmp->record_data[length],field->data.short_value,sizeof(short));
#endif /* MyISAMSTORE */
					if(diagmyisam)
					{
						prterr("DIAG position %d",length);
					}
					length+=sizeof(short);
					break;
				case LONGV:
				case SLONGV:
#ifdef MyISAMSTORE
					longstore(&NRDtmp->record_data[length],*field->data.integer_value);
#endif /* MyISAMSTORE */
#ifndef MyISAMSTORE
					memcpy(&NRDtmp->record_data[length],field->data.integer_value,sizeof(int));
#endif /* MyISAMSTORE */
					if(diagmyisam)
					{
						prterr("DIAG position %d",length);
					}
					length+=sizeof(int);
					break;
				case BOOLNS:
					if(*field->data.string_value) NRDtmp->record_data[length]='1';
						else NRDtmp->record_data[length]='0';
					if(diagmyisam)
					{
						prterr("DIAG position %d",length);
					}
					length+=1;
					break;
				case CHARACTERS:
					memcpy(&NRDtmp->record_data[length],
						field->data.string_value,1);
					if(diagmyisam)
					{
						prterr("DIAG position %d",length);
					}
					length+=1;
					break;
				default:
					prterr("Unknown field type %d for %s in assignMyIsamRecordData",field->type,field->name);
					break;
			}
		}
	} else {
		key=NRDtmp->key;
		if(key!=NULL)
		{
			for(y=0,part=key->part;y<key->numparts;++y,++part)
			{
				length=part->offset+RD_HEADER;
				field=FINDFLD(NRDtmp,part->name);
				if(field!=NULL)
				{
					switch(field->type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT: /* variable length field type */
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
							tlen=RDAstrlen(field->data.string_value);
							if(field->len>0) /* testing to catch variable lengths */
							{
								sprintf(&NRDtmp->record_data[length],"%-*s",field->len,
									(field->data.string_value!=NULL ? 
									field->data.string_value:""));
								if(diagmyisam)
								{
									prterr("DIAG position %d",length);
								}
								length+=field->len;
							} else {
#ifdef MyISAMSTORE
								longstore(&NRDtmp->record_data[length],tlen);
#endif /* MyISAMSTORE */
#ifndef MyISAMSTORE
								memcpy(&NRDtmp->record_data[length],&tlen,sizeof(int));
#endif /* MyISAMSTORE */
								length+=4;
								sprintf(&NRDtmp->record_data[length],"%-s",(field->data.string_value!=NULL ? field->data.string_value:""));
								if(diagmyisam)
								{
									prterr("DIAG position %d",length);
								}
								length+=tlen;
							}
							break;
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case SDECIMALV:
#ifdef MyISAMSTORE
							doublestore(&NRDtmp->record_data[length],*field->data.float_value);
#endif /* MyISAMSTORE */
#ifndef MyISAMSTORE
							memcpy(&NRDtmp->record_data[length],field->data.float_value,sizeof(double));
#endif /* MyISAMSTORE */
							break;
						case SHORTV:
						case SSHORTV:
#ifdef MyISAMSTORE
							shortstore(&NRDtmp->record_data[length],*field->data.short_value);
#endif /* MyISAMSTORE */
#ifndef MyISAMSTORE
							memcpy(&NRDtmp->record_data[length],field->data.short_value,sizeof(short));
#endif /* MyISAMSTORE */
							break;
						case LONGV:
						case SLONGV:
#ifdef MyISAMSTORE
							longstore(&NRDtmp->record_data[length],*field->data.integer_value);
#endif /* MyISAMSTORE */
#ifndef MyISAMSTORE
							memcpy(&NRDtmp->record_data[length],field->data.integer_value,sizeof(int));
#endif /* MyISAMSTORE */
							break;
						case BOOLNS:
						case CHARACTERS:
							memcpy(&NRDtmp->record_data[length],field->data.string_value,1);
							break;
						default:
							prterr("Unknown field type %d for %s in assignMyIsamRecordData",field->type,field->name);
							break;
					}
				}
			}
		}
	}
}
short CRTNRDmyisam(NRDbuffer *NRDtmp)
{
	MI_CREATE_INFO create_info;
	int create_flag=0;
	short r=FALSE;
	char *name=NULL;
	int x=0,y=0;
	NRDfield *field;
	NRDkey *key;
	NRDpart *part;
	MI_COLUMNDEF *recinfo=NULL,*FILErecinfo=NULL;
	MI_KEYDEF *keyinfo=NULL,*FILEkeyinfo=NULL;
	HA_KEYSEG *ks=NULL;
	CHARSET_INFO *RDA_default_charset_info;
	
	if(RDA_CollateType==TRUE)
	{
		RDA_default_charset_info=&my_charset_utf8_bin;
	} else {
		RDA_default_charset_info=default_charset_info;
	}
	memset((char *)&create_info,0,sizeof(create_info));
	create_info.max_rows=(ulong)0;

#ifndef WIN32
	memset(stemp,0,101);
	sprintf(stemp,"%d",RGETPID());
	if(UseRamDisk==TRUE && (!RDAstrcmp(NRDtmp->modname,"RPTGEN") || (RDAstrstr(NRDtmp->name,stemp)!=NULL)))
	{
		name=Rmalloc((RDAstrlen(NRDtmp->modname)+RDAstrlen(NRDtmp->name)+14));	
		sprintf(name,"/dev/shm/%s/%s",NRDtmp->modname,NRDtmp->name);
	} else {
#ifndef WT_FASTCGI
		name=Rmalloc((RDAstrlen(NRDtmp->modname)+RDAstrlen(NRDtmp->name)+5));	
		sprintf(name,"%s/%s",NRDtmp->modname,NRDtmp->name);
#else
		name=Rmalloc((RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(NRDtmp->modname)+RDAstrlen(NRDtmp->name)+8));	
		sprintf(name,"%s/%s/%s",CURRENTDIRECTORY,NRDtmp->modname,NRDtmp->name);
#endif
	}
#endif
#ifdef WIN32
	name=Rmalloc((RDAstrlen(NRDtmp->modname)+RDAstrlen(NRDtmp->name)+5));	
	sprintf(name,"%s\\%s",NRDtmp->modname,NRDtmp->name);
#endif
	
	if(diagmyisam)
	{
		prterr("MI_INFO for [%s][%s].",NRDtmp->modname,NRDtmp->name);
		prterr("\tnofields [%d] ",NRDtmp->nofields);TRACE;
	}
	FILErecinfo=Rmalloc((NRDtmp->nofields+1)*sizeof(MI_COLUMNDEF));
	memset(FILErecinfo,0,((NRDtmp->nofields+1)*sizeof(MI_COLUMNDEF)));
	recinfo=FILErecinfo;
	recinfo->type=FIELD_NORMAL;
	recinfo->null_bit=0;
	recinfo->null_pos=0;
	recinfo->length=1;
	for(x=0,field=NRDtmp->field;x<NRDtmp->nofields;++x,++field)
	{
		recinfo=FILErecinfo+(x+1);
		if(diagmyisam)
		{
			prterr("\tfields [%s] ",field->name);TRACE;
		}
		recinfo->type=FIELD_NORMAL;
		recinfo->null_bit=0;
		recinfo->null_pos=0;
		switch(field->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT: /* variable length field type */
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
					recinfo->length=field->len;
				} else {
					recinfo->length=field->len+4;
					recinfo->type=FIELD_VARCHAR;
				}
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				recinfo->length=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				recinfo->length=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				recinfo->length=sizeof(int);
				break;
			case BOOLNS:
			case CHARACTERS:
				recinfo->length=1;
				break;
			default:
				prterr("Error Field type %d for field %s\n",
					field->type,field->name);
				break; 
		}
	}
	memset(&glob_segs[0],0,(16*16*sizeof(HA_KEYSEG)));
	FILEkeyinfo=Rmalloc((NRDtmp->nokeys*sizeof(MI_KEYDEF)));
	memset(FILEkeyinfo,0,(NRDtmp->nokeys*sizeof(MI_KEYDEF)));
	key=NRDtmp->key;
	for(x=0,key=NRDtmp->key,keyinfo=FILEkeyinfo;x<NRDtmp->nokeys;++x,++key,++keyinfo)
	{
		key->length=MyIsamKeyLength(NRDtmp,x+1);
		keyinfo->keysegs=key->numparts;
		keyinfo->seg=&glob_segs[x][0];
		keyinfo->key_alg=HA_KEY_ALG_BTREE;
		keyinfo->flag=0;
/*
		keyinfo->flag=(HA_NOSAME | HA_UNIQUE_CHECK );
*/
/* Define Segments */
		keyinfo->block_length=0;
		for(y=0,part=key->part;y<key->numparts;++y,++part)
		{
			field=FINDFLD(NRDtmp,part->name);
			if(diagmyisam)
			{
				prterr("\tKeypart fields [%s] ",field->name);TRACE;
			}
			ks=&glob_segs[x][y];
			ks->flag=0;
			ks->start=part->offset+RD_HEADER;
			ks->null_bit=0;
			ks->null_pos=0;
			ks->charset=RDA_default_charset_info;
			switch(field->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT: /* variable length field type */
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
						if(RDA_CollateType==TRUE)
						{
							ks->type=HA_KEYTYPE_BINARY;
						} else {
							ks->type=HA_KEYTYPE_TEXT;
						}
						ks->length=field->len;
					} else {
						ks->length=field->len+4;
						ks->type=HA_KEYTYPE_VARTEXT1;
						ks->flag=HA_VAR_LENGTH_PART;
					}
					ks->language=RDA_default_charset_info->number;
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					ks->type=HA_KEYTYPE_DOUBLE;
					ks->length=sizeof(double);
					if(y==0)
					{
						ks->null_bit=0;
						ks->null_pos=0;
					}
					break;
				case SHORTV:
				case SSHORTV:
					ks->type=HA_KEYTYPE_SHORT_INT;
					ks->length=sizeof(short);
					if(y==0)
					{
						ks->null_bit=0;
						ks->null_pos=0;
					}
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					ks->type=HA_KEYTYPE_LONG_INT;
					ks->length=sizeof(int);
					if(y==0)
					{
						ks->null_bit=0;
						ks->null_pos=0;
					}
					break;
				case BOOLNS:
					if(RDA_CollateType==TRUE)
					{
						ks->type=HA_KEYTYPE_BINARY;
					} else {
						ks->type=HA_KEYTYPE_TEXT;
					}
					ks->language=RDA_default_charset_info->number;
					ks->length=1;
					break;
				case CHARACTERS:
					if(RDA_CollateType==TRUE)
					{
						ks->type=HA_KEYTYPE_BINARY;
					} else {
						ks->type=HA_KEYTYPE_TEXT;
					}
					ks->language=RDA_default_charset_info->number;
					ks->length=1;
					break;
				default:
					prterr("Error Field type %d for field %s\n",
						field->type,field->name);
					break; 
			}
/*
			ks->type=HA_KEYTYPE_BINARY;
*/
			if(diagmyisam)
			{
				prterr("\tKeypart %d fields [%s] start [%d] length [%d] type [%d] flag [%d] offset [%d] ",y,field->name,ks->start,ks->length,ks->type,ks->flag,part->offset+RD_HEADER);TRACE;
			}
		}
	}
	DELETENRDmyisam(NRDtmp->modname,NRDtmp->name);
	if(mi_create(name,NRDtmp->nokeys,FILEkeyinfo,(NRDtmp->nofields+1),FILErecinfo,0,(MI_UNIQUEDEF*)0,&create_info,create_flag))
	{
		prterr("Error:  CRTNRDmyisam failed to produce database.");
		r=(-1);
	}
	if(name!=NULL) Rfree(name);
	if(FILEkeyinfo!=NULL) Rfree(FILEkeyinfo);
	if(FILErecinfo!=NULL) Rfree(FILErecinfo);
	return(r);
}
short OPNNRDmyisam(NRDbuffer *NRDtmp)
{
	char *name=NULL;
	short r=FALSE,x=0;
	NRDkey *key=NULL;
	uint length=0;

#ifndef WIN32
	memset(stemp,0,101);
	sprintf(stemp,"%d",RGETPID());
	if(UseRamDisk==TRUE && (!RDAstrcmp(NRDtmp->modname,"RPTGEN") || (RDAstrstr(NRDtmp->name,stemp)!=NULL)))
	{
		name=Rmalloc((RDAstrlen(NRDtmp->modname)+RDAstrlen(NRDtmp->name)+14));	
		sprintf(name,"/dev/shm/%s/%s",NRDtmp->modname,NRDtmp->name);
	} else {
#ifndef WT_FASTCGI
		name=Rmalloc((RDAstrlen(NRDtmp->modname)+RDAstrlen(NRDtmp->name)+5));	
		sprintf(name,"%s/%s",NRDtmp->modname,NRDtmp->name);
#else 
		name=Rmalloc((RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(NRDtmp->modname)+RDAstrlen(NRDtmp->name)+8));	
		sprintf(name,"%s/%s/%s",CURRENTDIRECTORY,NRDtmp->modname,NRDtmp->name);
#endif
	}
#endif
#ifdef WIN32
	name=Rmalloc((RDAstrlen(NRDtmp->modname)+RDAstrlen(NRDtmp->name)+5));	
	sprintf(name,"%s\\%s",NRDtmp->modname,NRDtmp->name);
#endif
	if(!(NRDtmp->myinfo=mi_open(name,O_RDWR,HA_OPEN_WAIT_IF_LOCKED)))
	{
		prterr("Error:  mi_open failed to open database with %d.",my_errno);
		r=(-1);
	}
	if(name!=NULL) Rfree(name);
	if(!r) 
	{
		mi_extra(NRDtmp->myinfo,HA_EXTRA_WRITE_CACHE,0);
		mi_extra(NRDtmp->myinfo,HA_EXTRA_QUICK,0);
		NRDtmp->varfile=(-1);
		length=ADVMyIsamRecordLength(NRDtmp,TRUE);
		QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,length+1);
		memset(NRDtmp->record_data,0,NRDtmp->dlen);
		NRDtmp->tlen=0;
		NRDtmp->target=NULL;
		for(x=0,key=NRDtmp->key;x<NRDtmp->nokeys;++x,++key)
		{
			key->length=MyIsamKeyLength(NRDtmp,x+1);
			QUICKALLOC(NRDtmp->target,NRDtmp->tlen,key->length+1);
		}
	}
	return(r);
}
short OPNEXCLCKNRDmyisam(NRDbuffer *NRDtmp)
{
	char *name=NULL;
	short r=FALSE,x=0;
	NRDkey *key=NULL;
	uint length=0;

#ifndef WIN32
	memset(stemp,0,101);
	sprintf(stemp,"%d",RGETPID());
	if(UseRamDisk==TRUE && (!RDAstrcmp(NRDtmp->modname,"RPTGEN") || (RDAstrstr(NRDtmp->name,stemp)!=NULL)))
	{
		name=Rmalloc((RDAstrlen(NRDtmp->modname)+RDAstrlen(NRDtmp->name)+14));	
		sprintf(name,"/dev/shm/%s/%s",NRDtmp->modname,NRDtmp->name);
	} else {
#ifndef WT_FASTCGI
		name=Rmalloc((RDAstrlen(NRDtmp->modname)+RDAstrlen(NRDtmp->name)+5));	
		sprintf(name,"%s/%s",NRDtmp->modname,NRDtmp->name);
#else
		name=Rmalloc((RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(NRDtmp->modname)+RDAstrlen(NRDtmp->name)+8));	
		sprintf(name,"%s/%s/%s",CURRENTDIRECTORY,NRDtmp->modname,NRDtmp->name);
#endif
	}
#endif
#ifdef WIN32
	name=Rmalloc((RDAstrlen(NRDtmp->modname)+RDAstrlen(NRDtmp->name)+5));	
	sprintf(name,"%s\\%s",NRDtmp->modname,NRDtmp->name);
#endif

	if(!(NRDtmp->myinfo=mi_open(name,2,HA_OPEN_ABORT_IF_LOCKED)))
	{
		prterr("Error:  mi_open failed to open database.");
		r=(-1);
	}
	if(name!=NULL) Rfree(name);
	if(!r) 
	{
		mi_extra(NRDtmp->myinfo,HA_EXTRA_WRITE_CACHE,0);
		mi_extra(NRDtmp->myinfo,HA_EXTRA_QUICK,0);
		NRDtmp->varfile=(-1);
		length=ADVMyIsamRecordLength(NRDtmp,TRUE);
		QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,length+1);
		memset(NRDtmp->record_data,0,NRDtmp->dlen);
		for(x=0,key=NRDtmp->key;x<NRDtmp->nokeys;++x,++key)
		{
			key->length=MyIsamKeyLength(NRDtmp,x+1);
		}
	}
	return(r);
}
short CLSNRDmyisam(short fileno)
{
	NRDbuffer *NRDtmp=NULL;
	int error=0;

	NRDtmp=NRDbuf+fileno;
	if(mi_lock_database(NRDtmp->myinfo,F_UNLCK))
	{
		prterr("Error: mi_lock_database Unable to obtain lock.");
	} 
	error=mi_close(NRDtmp->myinfo);
	if(error) 
	{
		prterr("Error: mi_close failed with [%d] [%d] for Module [%s] File [%s].",error,my_errno,NRDtmp->modname,NRDtmp->name);
		return(-1);
	}
	return(0);
}
short LOCNRDmyisam(short fileno)
{
	NRDbuffer *NRDtmp=NULL;

	NRDtmp=NRDbuf+fileno;
	return(0);
}
short UNLNRDmyisam(short fileno)
{
	NRDbuffer *NRDtmp=NULL;

	NRDtmp=NRDbuf+fileno;
	return(0);
}
#define LOCK_WAIT	1000
short LOCNRDFILEmyisam(short fileno)
{
	NRDbuffer *NRDtmp=NULL;

	NRDtmp=NRDbuf+fileno;
	if(mi_lock_database(NRDtmp->myinfo,F_RDLCK))
	{
		prterr("Error: mi_lock_database Unable to obtain lock.");
		return(-1);
	} 
	return(0);
}
short UNLNRDFILEmyisam(short fileno)
{
	NRDbuffer *NRDtmp=NULL;

	NRDtmp=NRDbuf+fileno;
	if(mi_lock_database(NRDtmp->myinfo,F_UNLCK))
	{
		prterr("Error: mi_lock_database Unable to obtain lock.");
		return(-1);
	} 
	return(0);
}
void EXITNRDmyisam(void)
{
	end_key_cache(dflt_key_cache,1);
}
short INITNRDmyisam(void)
{
	my_set_max_open_files(2048);
	MY_INIT("XPERT");
#ifdef USE_MARIADB
    	init_key_cache(dflt_key_cache,key_cache_block_size,key_cache_size,0,0,0);
#else
    	init_key_cache(dflt_key_cache,key_cache_block_size,key_cache_size,0,0);
#endif
	return(0);
}
short EQLNRDKEYmyisam(short fileno,short keyno,void *target)
{
	NRDbuffer *NRDtmp=NULL;
	NRDkey *key=NULL;
	short ikeyno=1;
	short r=FALSE;
	uint key_len=0;
	
	if(diagmyisam)
	{
		prterr("DIAG EQLNRDKEYmyisam... ");
	}
	NRDtmp=NRDbuf+fileno;
	if(keyno>0 && keyno<=NRDtmp->nokeys)
	{
		ikeyno=keyno;
	} else ikeyno=1;
	key=NRDtmp->key+(ikeyno-1);
/*
	if(!isEMPTY(target))
*/
	if(target!=NULL)
	{
		r=mi_rkey(NRDtmp->myinfo,NRDtmp->record_data,ikeyno-1,(char *)target,key_len,HA_READ_KEY_EXACT);
		if(r)
		{
			if(diagmyisam)
			{
				prterr("DIAG mi_rkey: failed to find target with [%d].",my_errno);
			}
		} else {
			assignMyIsam_fields(NRDtmp);
		}
	} else {
		if(diagmyisam)
		{
			prterr("DIAG mi_rkey: failed to find target with no target(NULL).");
		}
		r=(-1);
	}
	return(r);
}
short EQLNRDmyisam(short fileno,short keyno)
{
	NRDbuffer *NRDtmp=NULL;
	short r=FALSE;
	short ikeyno=1;
	NRDkey *key=NULL;

	if(diagmyisam)
	{
		prterr("DIAG EQLNRDmyisam... ");
	}
	NRDtmp=NRDbuf+fileno;
	if(keyno>0 && keyno<=NRDtmp->nokeys)
	{
		ikeyno=keyno;
	} else ikeyno=1;
	key=NRDtmp->key+(ikeyno-1);
	MyIsam2target(NRDtmp,key,&r);
	return(EQLNRDKEYmyisam(fileno,keyno,NRDtmp->target));
}
short GTENRDmyisam(short fileno,short keyno)
{
	NRDbuffer *NRDtmp=NULL;
	short r=FALSE;
	short ikeyno=1;
	NRDkey *key=NULL;

	if(diagmyisam)
	{
		prterr("DIAG GTENRDmyisam... ");
	}
	NRDtmp=NRDbuf+fileno;
	if(keyno>0 && keyno<=NRDtmp->nokeys)
	{
		ikeyno=keyno;
	} else ikeyno=1;
	key=NRDtmp->key+(ikeyno-1);
	MyIsam2target(NRDtmp,key,&r);
	r=mi_rkey(NRDtmp->myinfo,NRDtmp->record_data,ikeyno-1,(char *)NRDtmp->target,0,HA_READ_KEY_OR_NEXT);
	if(r)
	{
		if(diagmyisam)
		{
			prterr("DIAG mi_rkey failed to find target.");
		}
	} else {
		assignMyIsam_fields(NRDtmp);
	}
	return(r);
}
short PRVNRDmyisam(short fileno,short keyno)
{
	NRDbuffer *NRDtmp=NULL;
	short r=FALSE;
	short ikeyno=1;
	NRDkey *key=NULL;

	if(diagmyisam)
	{
		prterr("DIAG PRVNRDmyisam... ");
	}
	NRDtmp=NRDbuf+fileno;
	if(keyno>0 && keyno<=NRDtmp->nokeys)
	{
		ikeyno=keyno;
	} else ikeyno=1;
	key=NRDtmp->key+(ikeyno-1);
	MyIsam2target(NRDtmp,key,&r);
	r=mi_rkey(NRDtmp->myinfo,NRDtmp->record_data,ikeyno-1,(char *)NRDtmp->target,0,HA_READ_BEFORE_KEY);
	if(r)
	{
		if(diagmyisam)
		{
			prterr("DIAG mi_rkey failed to find target.");
		}
	} else {
		assignMyIsam_fields(NRDtmp);
	}
	return(r);
}
short LSTNRDmyisam(short fileno,short keyno)
{
	NRDbuffer *NRDtmp=NULL;
	short ikeyno=1;
	short r=FALSE;
	
	if(diagmyisam)
	{
		prterr("DIAG LSTNRDmyisam... ");
	}
	NRDtmp=NRDbuf+fileno;
	assignMyIsamRecordData(NRDtmp,FALSE);
	if(keyno>0 && keyno<=NRDtmp->nokeys)
	{
		ikeyno=keyno;
	} else ikeyno=1;
	if(mi_rlast(NRDtmp->myinfo,NRDtmp->record_data,ikeyno-1))
	{
		if(diagmyisam)
		{
			prterr("DIAG mi_rlast: failed to find target.");
		}
		r=(-1);
	} else {
		assignMyIsam_fields(NRDtmp);
	}
	return(r);
}
short BEGNRDmyisam(short fileno)
{
#ifdef PHYSICAL_MOVEMENT
	NRDbuffer *NRDtmp=NULL;
	int r=FALSE;
	short rs=0;
	
	if(diagmyisam)
	{
		prterr("DIAG BEGNRDmyisam... ");
	}
	NRDtmp=NRDbuf+fileno;
	assignMyIsamRecordData(NRDtmp,FALSE);
	r=mi_rrnd(NRDtmp->myinfo,NRDtmp->record_data,HA_POS_ERROR);
	while(r!=HA_ERR_END_OF_FILE)
	{
		r=mi_rrnd(NRDtmp->myinfo,NRDtmp->record_data,HA_OFFSET_ERROR);
	}
	if(r)
	{
		if(diagmyisam)
		{
			prterr("DIAG mi_rrnd: failed to find target with %d %d.",r,my_errno);
		}
		rs=(-1);
	} else {
		assignMyIsam_fields(NRDtmp);
		rs=0;
	}
	return(r);
#else
	return(FRSNRDmyisam(fileno,1));
#endif
}
short SEQNRDmyisam(short fileno)
{
#ifdef PHYSICAL_MOVEMENT
	NRDbuffer *NRDtmp=NULL;
	int r=FALSE;
	short rs=0;
	
	if(diagmyisam)
	{
		prterr("DIAG SEQNRDmyisam... ");
	}
	NRDtmp=NRDbuf+fileno;
	assignMyIsamRecordData(NRDtmp,FALSE);
	r=mi_rrnd(NRDtmp->myinfo,NRDtmp->record_data,HA_OFFSET_ERROR);
	while(r!=HA_ERR_END_OF_FILE)
	{
		r=mi_rrnd(NRDtmp->myinfo,NRDtmp->record_data,HA_OFFSET_ERROR);
	}
	if(r)
	{
		if(diagmyisam)
		{
			prterr("DIAG mi_rrnd: failed to find target with %d %d.",r,my_errno);
		}
		rs=(-1);
	} else {
		assignMyIsam_fields(NRDtmp);
		rs=0;
	}
	return(r);
#else
	return(NXTNRDmyisam(fileno,1));
#endif
}
short LTENRDmyisam(short fileno,short keyno)
{
	NRDbuffer *NRDtmp=NULL;
	short r=FALSE;
	short ikeyno=1;
	NRDkey *key=NULL;

	if(diagmyisam)
	{
		prterr("DIAG LTENRDmyisam... ");
	}
	NRDtmp=NRDbuf+fileno;
	if(keyno>0 && keyno<=NRDtmp->nokeys)
	{
		ikeyno=keyno;
	} else ikeyno=1;
	key=NRDtmp->key+(ikeyno-1);
	MyIsam2target(NRDtmp,key,&r);
	r=mi_rkey(NRDtmp->myinfo,NRDtmp->record_data,ikeyno-1,(char *)NRDtmp->target,0,HA_READ_KEY_OR_PREV);
	if(r)
	{
		if(diagmyisam)
		{
			prterr("DIAG mi_rkey failed to find target.");
		}
	} else {
		assignMyIsam_fields(NRDtmp);
	}
	return(r);
}
short FRSNRDmyisam(short fileno,short keyno)
{
	NRDbuffer *NRDtmp=NULL;
	short ikeyno=1;
	short r=FALSE;
	
	if(diagmyisam)
	{
		prterr("DIAG FRSNRDmyisam... ");
	}
	NRDtmp=NRDbuf+fileno;
	if(keyno>0 && keyno<=NRDtmp->nokeys)
	{
		ikeyno=keyno;
	} else ikeyno=1;
	ZERNRD(fileno);
	assignMyIsamRecordData(NRDtmp,FALSE);
	r=mi_rfirst(NRDtmp->myinfo,NRDtmp->record_data,ikeyno-1);
	if(r)
	{
		if(diagmyisam)
		{
			prterr("DIAG mi_rfirst: failed to find target with %d %d.",r,my_errno);
		}
	} else {
		assignMyIsam_fields(NRDtmp);
	}
	return(r);
}
short NXTNRDmyisam(short fileno,short keyno)
{
	NRDbuffer *NRDtmp=NULL;
	short r=FALSE;
	short ikeyno=1;
	NRDkey *key=NULL;

	if(diagmyisam)
	{
		prterr("DIAG NXTNRDmyisam... ");
	}
	NRDtmp=NRDbuf+fileno;
	if(keyno>0 && keyno<=NRDtmp->nokeys)
	{
		ikeyno=keyno;
	} else ikeyno=1;
	key=NRDtmp->key+(ikeyno-1);
	MyIsam2target(NRDtmp,key,&r);
	r=mi_rkey(NRDtmp->myinfo,NRDtmp->record_data,ikeyno-1,(char *)NRDtmp->target,0,HA_READ_AFTER_KEY);
	if(r)
	{
		if(diagmyisam)
		{
			prterr("DIAG mi_rkey failed to find target.");
		}
	} else {
		assignMyIsam_fields(NRDtmp);
	}
	return(r);
}
long SIZNRDmyisam(short fileno)
{
	NRDbuffer *NRDtmp=NULL;
	long leng=0;
	MI_ISAMINFO myisaminfo;

	if(diagmyisam)
	{
		prterr("DIAG SIZNRDmyisam... ");
	}
	NRDtmp=NRDbuf+fileno;
	if(NRDtmp->myinfo!=NULL)
	{
		memset(&myisaminfo,0,sizeof(MI_ISAMINFO));
		mi_status(NRDtmp->myinfo,&myisaminfo,HA_STATUS_VARIABLE | HA_STATUS_CONST);
		leng=(ulong)myisaminfo.records;
	}
	return(leng);
}
short DELNRDmyisam(short fileno)
{
	NRDbuffer *NRDtmp=NULL;
	short error=0;

	if(diagmyisam)
	{
		prterr("DIAG DELNRDmyisam... ");
	}
	NRDtmp=NRDbuf+fileno;
	assignMyIsamRecordData(NRDtmp,TRUE);
	error=mi_delete(NRDtmp->myinfo,NRDtmp->record_data);
	if(diagmyisam)
	{
		if(error!=0)
		{
			prterr("Error: mi_delete has failed with [%d] [%d].",error,my_errno);
		}
	}
	return(error);
}
short WRTNRDmyisam(short fileno,short trnmode,char *fieldname)
{
	NRDbuffer *NRDtmp=NULL;
	NRDfield *field;
	short return_value=0,tl=0;
	int length=0,error=0;
	NRDkey *key=NULL;

	if(diagmyisam)
	{
		prterr("DIAG WRTNRDisam Writing MyIsam Database Record in Module [%s] File [%s] Trnmode [%d] Fieldname [%s].",MODULENAME(fileno),FILENAME(fileno),trnmode,(fieldname!=NULL ? fieldname:""));
		SEENRDRECORD(fileno);TRACE;
	}
	NRDtmp=NRDbuf+fileno;
	if(diagmyisam)
	{
		length=MyIsamRecordLength(NRDtmp);
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
	if(trnmode && field!=NULL)/*get next tran num automatically*/
	{
		SetTrnModeField(fileno,fieldname);
		assignMyIsamRecordData(NRDtmp,TRUE);

		error=mi_write(NRDtmp->myinfo,NRDtmp->record_data);
		if(error)
		{
			prterr("Error: mi_write failed to create record error[%d] myerrno [%d].",error,my_errno);
		}
	} else {
		assignMyIsamRecordData(NRDtmp,TRUE);
		key=NRDtmp->key;
		MyIsam2target(NRDtmp,key,&tl);
		QUICKALLOC(NRDtmp->record_data2,NRDtmp->dlen2,NRDtmp->dlen);
		memset(NRDtmp->record_data2,0,NRDtmp->dlen2);
		memcpy(NRDtmp->record_data2,NRDtmp->record_data,NRDtmp->dlen);
		error=mi_rkey(NRDtmp->myinfo,NRDtmp->record_data,0,(char *)NRDtmp->target,0,HA_READ_KEY_EXACT);
		if(error==HA_ERR_KEY_NOT_FOUND)
		{
			my_errno=0;
			error=mi_write(NRDtmp->myinfo,NRDtmp->record_data2);
			if(error)
			{
				prterr("Error: mi_write failed to create record error[%d] myerrno [%d].",error,my_errno);
			}
		} else {
			my_errno=0;
			error=mi_update(NRDtmp->myinfo,NRDtmp->record_data,NRDtmp->record_data2);
			if(error)
			{
				prterr("Error: mi_update failed to create record error[%d] myerrno [%d].",error,my_errno);
			}
		}
	}
	return(return_value);
}
short RBDNRDmyisam(char *modx,char *filx)
{
	char *cmd=NULL;
	short x=FALSE;

	if(diagmyisam)
	{
		prterr("DIAG RBDNRDmyisam... ");
	}
	cmd=Rmalloc(RDAstrlen(modx)+RDAstrlen(filx)+23);
#ifndef WIN32
	sprintf(cmd,"myisamchk -B -v -o %s/%s",modx,filx);
#else
	sprintf(cmd,"myisamchk -B -v -o %s\\%s",modx,filx);
#endif
	x=system_command(cmd);
	if(cmd!=NULL) Rfree(cmd);
	return(x);
}	
short RESNRDmyisam(char *modx,char *filx)
{
	char *cmd=NULL;
	short x=FALSE;

	if(diagmyisam)
	{
		prterr("DIAG RESNRDmyisam... ");
	}
	cmd=Rmalloc(RDAstrlen(modx)+RDAstrlen(filx)+23);
#ifndef WIN32
	sprintf(cmd,"myisamchk -B -v -o %s/%s",modx,filx);
#else
	sprintf(cmd,"myisamchk -B -v -o %s\\%s",modx,filx);
#endif
	x=system_command(cmd);
	if(cmd!=NULL) Rfree(cmd);
	return(x);
}	
short DELETENRDmyisam(char *module,char *filename)
{
	char *libx=NULL,*tmp=NULL;

	if(diagmyisam)
	{
		prterr("DIAG DELETENRDmyisam... ");
	}
#ifndef WIN32
	memset(stemp,0,101);
	sprintf(stemp,"%d",RGETPID());
	if(UseRamDisk==TRUE && (!RDAstrcmp(module,"RPTGEN") || (RDAstrstr(filename,stemp)!=NULL)))
	{
		libx=Rmalloc((RDAstrlen(module)+RDAstrlen(filename)+14));	
		sprintf(libx,"/dev/shm/%s/%s",module,filename);
	} else {
#ifndef WT_FASTCGI
		libx=Rmalloc((RDAstrlen(module)+RDAstrlen(filename)+5));	
		sprintf(libx,"%s/%s",module,filename);
#else
		libx=Rmalloc((RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+RDAstrlen(filename)+8));	
		sprintf(libx,"%s/%s/%s",CURRENTDIRECTORY,module,filename);
#endif
	}
#endif
#ifdef WIN32
	libx=Rmalloc(RDAstrlen(filename)+RDAstrlen(CURRENTDIRECTORY)+
		RDAstrlen(module)+3);
	sprintf(libx,"%s\\%s\\%s",CURRENTDIRECTORY,module,filename);
#endif
	tmp=Rmalloc(RDAstrlen(libx)+5);
	sprintf(tmp,"%s.MYD",libx);
	if(unlink(tmp)==(-1))
	{
		if(errno!=ENOENT)
		{ 	
			prterr("Error [%s] can't remove file [%s].",errname(),tmp);
			if(libx!=NULL) Rfree(libx);
			if(tmp!=NULL) Rfree(tmp);
			return(-1);
		}
	}
	memset(tmp,0,RDAstrlen(libx)+5);
	sprintf(tmp,"%s.MYI",libx);
	if(unlink(tmp)==(-1))
	{
		if(errno!=ENOENT)
		{
			prterr("Error [%s] can't remove file [%s].",errname(),tmp);
			if(libx!=NULL) Rfree(libx);
			if(tmp!=NULL) Rfree(tmp);
			return(-1);
		}
	}
	if(libx!=NULL) Rfree(libx);
	if(tmp!=NULL) Rfree(tmp);
	return(0);
}
void *xMyIsam2target(NRDbuffer *NRDtmp,NRDkey *key,short *pos,int line,char *file)
{
	short x;
	NRDpart *part;
	NRDfield *field=NULL;
	int tlen=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) 
	{
		prterr("DIAG MyIsam2target at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	*pos=0;
	if(NRDtmp->varfile)
	{
		key->length=MyIsamKeyLength(NRDtmp,1);
	}
	QUICKALLOC(NRDtmp->target,NRDtmp->tlen,key->length+1);
	memset(NRDtmp->target,0,NRDtmp->tlen);
	for(x=0,part=key->part;x<key->numparts;++x,++part)
	{
		field=FINDFLD(NRDtmp,part->name);
		if(field!=NULL)
		{
/* increment allocation space for target by field length */
/* write field data into target */
			switch(field->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT: /* variable length field type */
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT: /* plain text field type */
				case ZIPCODE:
				case PHONE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case CUSTOMTYPE:
					if(field->len>0)
					{
						unpad(field->data.string_value);
						sprintf(&NRDtmp->target[*pos],"%-*s",field->len,(field->data.string_value!=NULL ? field->data.string_value:""));
						*pos+=field->len;
						NRDtmp->target[*pos]=0;
					} else {
						tlen=RDAstrlen(field->data.string_value);
						unpad(field->data.string_value);
#ifdef MyISAMSTORE
						longstore(&NRDtmp->target[*pos],tlen);
#endif /* MyISAMSTORE */
#ifndef MyISAMSTORE
						memcpy(&NRDtmp->target[*pos],&tlen,sizeof(int));
#endif /* MyISAMSTORE */
						sprintf(&NRDtmp->target[*pos+4],"%-s",(field->data.string_value!=NULL ? field->data.string_value:""));
						*pos+=RDAstrlen(field->data.string_value)+4;
						NRDtmp->target[*pos]=0;
					}
					break;
				case CHARACTERS:
					memcpy(&NRDtmp->target[*pos],field->data.string_value,1);
					*pos+=1;
					break;
				case BOOLNS:
					if(*field->data.string_value==TRUE) NRDtmp->target[*pos]='1';
						else NRDtmp->target[*pos]='0';
					*pos+=1;
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
#ifdef MyISAMSTORE
					doublestore(&NRDtmp->target[*pos],*field->data.float_value);
#endif /* MyISAMSTORE */
#ifndef MyISAMSTORE
					memcpy(&NRDtmp->target[*pos],field->data.float_value,sizeof(double));
#endif /* MyISAMSTORE */
					*pos+=sizeof(double);
					break;
				case SHORTV:
				case SSHORTV:
#ifdef MyISAMSTORE
					shortstore(&NRDtmp->target[*pos],*field->data.short_value);
#endif /* MyISAMSTORE */
#ifndef MyISAMSTORE
					memcpy(&NRDtmp->target[*pos],field->data.short_value,sizeof(short));
#endif /* MyISAMSTORE */
					*pos+=sizeof(short);
					break;
				case LONGV:
				case SLONGV:
#ifdef MyISAMSTORE
					longstore(&NRDtmp->target[*pos],*field->data.integer_value);
#endif /* MyISAMSTORE */
#ifndef MyISAMSTORE
					memcpy(&NRDtmp->target[*pos],field->data.integer_value,sizeof(int));
#endif /* MyISAMSTORE */
					*pos+=sizeof(int);
					break;
				default:
					break;
			}
		}
	}
	return(NRDtmp->target);
}
#endif
/*lint +e525*/
