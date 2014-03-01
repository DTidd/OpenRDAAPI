#include <nrdp.hpp>

char *fieldtypedesc[]=
{
	"Variable Text Fields",
	"Plain Text Fields",
	"Formatted Dollars",
	"Phone Numbers",
	"Custom Types",
	"Zip Codes",
	"Unsigned Decimal Value",
	"Unsigned Short Integer",
	"Unsigned Long Integer",
	"Unsigned Float Value",
	"Boolean Value",
	"Single Character",
	"Buttons",
	"ScrolledList",
	"ScrolledText",
	"Social Security #'s",
	"Dates",
	"Times",
	"Signed Short Integer",
	"Signed Long Integer",
	"Signed Decimal Value",
	"Signed Float Value",
	"Expenditure Account",
	"Revenue Account",
	"Balance Sheet Account",
	"Beginning Balance Account",
	"Optional Field",
	"Formatted Dollars-No Cents",
};

short xWRTNRD(short fileno,short trnmode,char *fieldname,int line,char *file)
{
	short retval=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("DIAG WRTNRD Writing record to Module [%s] File [%d] [%s] Trnmode [%d] Fieldname [%s] at line [%d] program [%s].",MODULENAME(fileno),fileno,FILENAME(fileno),trnmode,(fieldname!=NULL ? fieldname:""),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	LOCNRDFILE(fileno);
	switch(NRDbuf[fileno].db_engine)
	{
		case NRDRdaEngine:	
			retval=WRTNRDrda(fileno,trnmode,fieldname);
			break;
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:
			retval=WRTNRDisam(fileno,trnmode,fieldname);
			break;
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine:
			retval=WRTNRDmyisam(fileno,trnmode,fieldname);
			break;
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine:
			retval=WRTNRDmysqlxpert(fileno,trnmode,fieldname);
			break;
		case NRDMySQLEngine:
		case NRDMySQLAutoEngine:
			retval=WRTNRDmysql(fileno,trnmode,fieldname);
			break;
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree:
			retval=WRTNRDberkeley(fileno,trnmode,fieldname);
			break;
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine:	
		case NRDLocalCtreeEngine:
			retval=WRTNRDctree(fileno,trnmode,fieldname);
			break;
#endif
		default:
			prterr("Error Unknown Database Engine [%d] in Module [%s] File [%s] at line [%d] program [%s].",NRDbuf[fileno].db_engine,MODULENAME(fileno),FILENAME(fileno),line,file);
			retval=(-1);
			break;
	}
	if(!retval) RCKNRD(fileno);
	UNLNRDFILE(fileno);
	return(retval);
}
void xSetTrnModeField(short fileno,char *fieldname,int line,char *file)
{
	short return_value=0,comp_val,y,z;
	NRDfield *field,*holdfields,*fldx,*fldx1;
	NRDkey *key;
	NRDpart *part;
	union rda_field_types value;
	NRDbuffer *NRDtmp=NULL;
	short keyno=(-1);

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("DIAG SetTrnModeField for File [%d] [%s] [%s] using Field [%s] at line [%d] program [%s].",fileno,MODULENAME(fileno),FILENAME(fileno),fieldname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	
	for(z=0,key=NRDtmp->key;z<NRDtmp->nokeys;++z,++key)
	{
		part=key->part+(key->numparts-1);
		if(!RDAstrcmp(part->name,fieldname))
		{
			keyno=(z+1);
			break;
		}
	}
	if(keyno==(-1) || keyno>NRDtmp->nokeys) keyno=1;
	field=FLDNRD(fileno,fieldname);
/* et temporary repository for current_record_data */
	holdfields=Rmalloc(NRDtmp->nofields*sizeof(NRDfield));
	for(y=0,fldx=holdfields,fldx1=NRDtmp->field;y<NRDtmp->nofields;++y,++fldx,++fldx1)
	{
		fldx->type=fldx1->type;
		fldx->name=stralloc(fldx1->name);
		fldx->len=fldx1->len;
		switch(fldx1->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT: 
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
			case PLAINTEXT: /* plain text field type */
				if(fldx1->data.string_value!=NULL)
				{
					QUICKALLOC(fldx->data.string_value,fldx->dlen,fldx1->dlen);
					memcpy(fldx->data.string_value,fldx1->data.string_value,fldx1->dlen);
				} else if(fldx->dlen>0)
				{
					memset(fldx->data.string_value,0,fldx->dlen);
				} else {
					fldx->data.string_value=NULL;
					fldx->dlen=0;
				}
				break;
			case BOOLNS:
			case CHARACTERS:
				if(fldx->data.string_value==NULL) 
				{
					fldx->data.string_value=Rmalloc(1);
					fldx->dlen=1;
				}
				*fldx->data.string_value=*fldx1->data.string_value;
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				if(fldx->data.float_value==NULL) 
				{
					fldx->data.float_value=Rmalloc(sizeof(double));
					fldx->dlen=sizeof(double);
				}
				*fldx->data.float_value=*fldx1->data.float_value;
				break;
			case SHORTV:
			case SSHORTV:
				if(fldx->data.short_value==NULL) 
				{
					fldx->data.short_value=Rmalloc(sizeof(short));
					fldx->dlen=sizeof(short);
				}
				*fldx->data.short_value=*fldx1->data.short_value;
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				if(fldx->data.integer_value==NULL) 
				{
					fldx->data.integer_value=Rmalloc(sizeof(int));
					fldx->dlen=sizeof(int);
				}
				*fldx->data.integer_value=*fldx1->data.integer_value;
				break;
			default:
				prterr("Error Field Type [%d] is invalid for Field [%s] in Module [%s] File [%s].",fldx1->type,fldx1->name,MODULENAME(fileno),FILENAME(fileno));
				break;
		}
	}
/* set tran num to max value for incremental type fields */
	switch(field->type)
	{
		case SCROLLEDLIST:
		case LONGV:	/* 2,147,483,647 */
		case SLONGV:	/* 2,147,483,647 */
			value.integer_value=Rmalloc(sizeof(int));
			*value.integer_value=(INT_MAX)-1;
			*field->data.integer_value=*value.integer_value;
			break;
		case SHORTV: /* 32K biggest value */
		case SSHORTV: /* 32K biggest value */
			value.short_value=Rmalloc(sizeof(short));
			*value.short_value=(SHRT_MAX)-1;
			*field->data.short_value=*value.short_value;
			break;
		case DOUBLEV: /* 15 whatever's */
		case SDOUBLEV:
			value.float_value=Rmalloc(sizeof(double));
#ifndef WIN32
			*value.float_value=(MAXDOUBLE)-1;
#endif
#ifdef WIN32
			*value.float_value=(_I8_MAX)-1;
#endif
			*field->data.float_value=*value.float_value;
			break;
		default:
			prterr("Error Field Type [%d] is invalid for use as Transaction Mode for Field [%s] in Module [%s] File [%s].",field->type,field->name,MODULENAME(fileno),FILENAME(fileno));
			break;
	}
	return_value=LTENRD(fileno,keyno);
	NRDtmp=NRDbuf+fileno;
	field=xFLDNRD(fileno,fieldname,__LINE__,__FILE__);
	if(field==NULL)
	{
		prterr("Fieldname %s not found",fieldname);
		return;
	}
	key=NRDtmp->key+(keyno-1);
	comp_val=FALSE;
	for(y=0,part=key->part;y<key->numparts;++y,++part)
	{
		if(comp_val) break;
		for(z=0,fldx=NRDtmp->field,fldx1=holdfields;z<NRDtmp->nofields;++z,++fldx,++fldx1)
		{
			if(!RDAstrcmp(fldx->name,part->name)) break;
		} 
		if(z<NRDtmp->nofields)
		{
			if(RDAstrcmp(fldx->name,field->name))
			{
				switch(fldx->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT: 
					case PLAINTEXT:
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
						if(RDAstrcmp(fldx->data.string_value,fldx1->data.string_value)) comp_val=TRUE;
						break;
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case SDECIMALV:
						if(*fldx->data.float_value!=*fldx1->data.float_value) comp_val=TRUE;
						break;
					case SHORTV:
					case SSHORTV:
						if(*fldx->data.short_value!=*fldx1->data.short_value) comp_val=TRUE;
						break;
					case LONGV:
					case SLONGV:
						if(*fldx->data.integer_value!=*fldx1->data.integer_value) comp_val=TRUE;
						break;
					case BOOLNS:
					case CHARACTERS:
						if(*fldx->data.string_value!=*fldx1->data.string_value) comp_val=TRUE;
						break;
					default:
						prterr("Error Field Type [%d] is invalid for Field [%s] in Module [%s] File [%s].",field->type,field->name,MODULENAME(fileno),FILENAME(fileno));
						break;
				}
			}
		} else {
			prterr("ERROR - Key part [%s] not found.",part->name);
		}
	}
	if(!return_value && !comp_val)
	{
		switch(field->type)
		{
			case SCROLLEDLIST:
			case LONGV:	/* 2,147,483,647 max signed int */
			case SLONGV:	/* 2,147,483,647 max signed int */
/*lint -e644 */
				*value.integer_value=*field->data.integer_value;
/*lint +e644 */
				++*value.integer_value;
				break;
			case SHORTV: /* 32K biggest value signed short */
			case SSHORTV: /* 32K biggest value signed short */
/*lint -e644 */
				*value.short_value=*field->data.short_value;
/*lint +e644 */
				++*value.short_value;
				break;
			case DOUBLEV: /* 15 significant digits */
			case SDOUBLEV: /* 15 significant digits */
/*lint -e644 */
				*value.float_value=*field->data.float_value;
/*lint +e644 */
				++*value.float_value;
				break;
			default:
				prterr("Error Field Type [%d] is invalid for use as Transaction Mode for Field [%s] in Module [%s] File [%s].",field->type,field->name,MODULENAME(fileno),FILENAME(fileno));
				return;
		}
	} else { /* no other records found */
		switch(field->type)
		{
			case SCROLLEDLIST:
			case LONGV:	/* 2,147,483,647 max signed int */
			case SLONGV:	/* 2,147,483,647 max signed int */
/*lint -e644 */
				*value.integer_value=1;
/*lint +e644 */
				break;
			case SHORTV: /* 32K biggest value signed short */
			case SSHORTV: /* 32K biggest value signed short */
/*lint -e644 */
				*value.short_value=1;
/*lint +e644 */
				break;
			case DOUBLEV: /* 15 significant digits */
			case SDOUBLEV: /* 15 significant digits */
/*lint -e644 */
				*value.float_value=1;
/*lint +e644 */
				break;
			default:
				prterr("Error Field Type [%d] is invalid for use as Transaction Mode for Field [%s] in Module [%s] File [%s].",field->type,field->name,MODULENAME(fileno),FILENAME(fileno));
				return;
		}
	}
	for(y=0,fldx1=holdfields,fldx=NRDtmp->field;y<NRDtmp->nofields;++y,++fldx,++fldx1)
	{
		switch(fldx->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT: 
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
			case PLAINTEXT: /* plain text field type */
				if(fldx1->data.string_value!=NULL)
				{
					QUICKALLOC(fldx->data.string_value,fldx->dlen,fldx1->dlen);
					memcpy(fldx->data.string_value,fldx1->data.string_value,fldx1->dlen);
				} else if(fldx->dlen>0)
				{
					memset(fldx->data.string_value,0,fldx->dlen);
				} else {
					fldx->data.string_value=NULL;
					fldx->dlen=0;
				}
				if(fldx1->data.string_value!=NULL) 
					Rfree(fldx1->data.string_value);
				fldx1->dlen=0;
				break;
			case BOOLNS:
			case CHARACTERS:
				*fldx->data.string_value=*fldx1->data.string_value;
				fldx->dlen=1;
				if(fldx1->data.string_value!=NULL) Rfree(fldx1->data.string_value);
				fldx1->data.string_value=0;
				fldx1->dlen=0;
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				*fldx->data.float_value=*fldx1->data.float_value;
				fldx->dlen=sizeof(double);
				if(fldx1->data.float_value!=NULL) Rfree(fldx1->data.float_value);
				fldx1->data.float_value=NULL;
				fldx1->dlen=0;
				break;
			case SHORTV:
			case SSHORTV:
				*fldx->data.short_value=*fldx1->data.short_value;
				fldx->dlen=sizeof(short);
				if(fldx1->data.short_value!=NULL) Rfree(fldx1->data.short_value);
				fldx1->data.short_value=NULL;
				fldx1->dlen=0;
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				*fldx->data.integer_value=*fldx1->data.integer_value;
				fldx->dlen=sizeof(int);
				if(fldx1->data.integer_value!=NULL) Rfree(fldx1->data.integer_value);
				fldx1->data.integer_value=NULL;
				fldx1->dlen=0;
				break;
			default:
				prterr("Error Field Type [%d] is invalid for Field [%s] in Module [%s] File [%s].",field->type,field->name,MODULENAME(fileno),FILENAME(fileno));
				break;
		}
		if(fldx1->name!=NULL) Rfree(fldx1->name);
	}
	Rfree(holdfields);
	field=xFLDNRD(fileno,fieldname,__LINE__,__FILE__);
	switch(field->type)
	{
		case SCROLLEDLIST:
		case LONGV:	/* 2,147,483,647 max signed int */
		case SLONGV:	/* 2,147,483,647 max signed int */
#ifdef USE_RDA_DIAGNOSTICS
			if(diagnrd || diagnrd_field)
			{
				prterr("DIAG Next Transaction # [%d].",*value.integer_value);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			*field->data.integer_value=*value.integer_value;
			break;
		case SHORTV: /* 32K biggest value signed short */
		case SSHORTV: /* 32K biggest value signed short */
#ifdef USE_RDA_DIAGNOSTICS
			if(diagnrd || diagnrd_field)
			{
				prterr("DIAG Next Transaction # [%d].",*value.short_value);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			*field->data.short_value=*value.short_value;
			break;
		case DOUBLEV: /* 15 significant digits */
		case SDOUBLEV: /* 15 significant digits */
#ifdef USE_RDA_DIAGNOSTICS
			if(diagnrd || diagnrd_field)
			{
				prterr("DIAG Next Transaction # [%f].",*value.float_value);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			*field->data.float_value=*value.float_value;
			break;
		default:
			prterr("Error Field Type [%d] is invalid for use as Transaction Mode for Field [%s] in Module [%s] File [%s].",field->type,field->name,MODULENAME(fileno),FILENAME(fileno));
			return;
	}
	switch(field->type)
	{
		case SCROLLEDLIST:
		case LONGV:	/* 2,147,483,647 */
		case SLONGV:	/* 2,147,483,647 */
			if(value.integer_value!=NULL) Rfree(value.integer_value);
			break;
		case SHORTV: /* 32K biggest value */
		case SSHORTV: /* 32K biggest value */
			if(value.short_value!=NULL) Rfree(value.short_value);
			break;
		case DOUBLEV: /* 15 whatever's */
		case SDOUBLEV:
			if(value.float_value!=NULL) Rfree(value.float_value);
			break;
		default:
			break;
	}
}
short WRTNRDrda(short fileno,short trnmode,char *fieldname)
{
	NRDbuffer *NRDtmp;
	NRDfield *field;
	short return_value=0;
	int x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG WRTNRDrda Writing RDA Database Record in Module [%s] File [%s] Trnmode [%d] Fieldname [%s].",MODULENAME(fileno),FILENAME(fileno),trnmode,(fieldname!=NULL ? fieldname:""));
	}
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
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
	}
	for(x=0,field=NRDtmp->field;x<NRDtmp->nofields;++x,++field)
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
				SETDTA(NRDtmp->dtanum,x,field->data.string_value);
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				SETDTA(NRDtmp->dtanum,x,field->data.float_value);
				break;
			case SHORTV:
			case SSHORTV:
				SETDTA(NRDtmp->dtanum,x,field->data.short_value);
				break;
			case LONGV:
			case SLONGV:
				SETDTA(NRDtmp->dtanum,x,field->data.integer_value);
				break;
			case BOOLNS:
			case CHARACTERS:
				SETDTA(NRDtmp->dtanum,x,field->data.string_value);
				break;
			default:
				prterr("Error Field Type [%d] is invalid for Field [%s] in Module [%s] File [%s].",field->type,field->name,MODULENAME(fileno),FILENAME(fileno));
				break;
		}
	} 
	return_value=WRTDTA(NRDtmp->dtanum);
	if(return_value)
	{
		prterr("Error WRTDTA Failed Attempting to Write in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	} else {
#ifdef USE_RDA_DIAGNOSTICS
		if(diagnrd) { prterr("DIAG WRTDTA exiting OK after Writing in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name); }
#endif /* USE_RDA_DIAGNOSTICS */
	}
	return(return_value);
}
