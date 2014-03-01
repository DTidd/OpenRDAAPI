#include<mix.hpp>

short xsetfilekeyFROMrangersrc(short fileno,short keyno,RangeScreen *rs,int line,char *file)
{
	NRDfield *field=NULL;
	NRDkey *keyx=NULL;
	NRDpart *part=NULL;
	short x,y;
	FileRangeStruct *frs=NULL;
	char *filename=NULL;
	short return_value=FALSE;
	unsigned h=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG Setting Module [%s] File [%d] [%s] for key parts to range values Using Range Screen [%s] [%s] [%s] at line [%d] program [%s].",MODULENAME(fileno),fileno,FILENAME(fileno),KEYNAME(fileno,keyno),rs->module,rs->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	if((keyx=KEYNUM(fileno,keyno))==NULL)
	{
		prterr("Error cannot get key parts for file %d [%s] %d [%s].",
			fileno,FILENAME(fileno),keyno,KEYNAME(fileno,keyno));
		return(-1);
	}	
	if(rs!=NULL && rs->frs!=NULL)
	{
	if(keyx->part!=NULL)
	{
		for(x=0,part=keyx->part;x<keyx->numparts;++x,++part)
		{
		field=FLDNRD(fileno,part->name);
		if(field!=NULL)
		{
			filename=FILENAME(fileno);
			for(y=0,frs=rs->frs;y<rs->num;++y,++frs)
			{
				if(!RDAstrcmp(frs->filename,filename) &&
					!RDAstrcmp(frs->fieldname,field->name)) break;
			}
			if(y<rs->num)
			{
				switch(field->type)
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
						if(!isEMPTY(frs->from.string_value)) 
						{
							h=RDAstrlen(frs->from.string_value);
							QUICKALLOC(field->data.string_value,field->dlen,h+1);
							memcpy(field->data.string_value,frs->from.string_value,h+1);
							if(field->dlen>field->len && field->len>0)
							{
								field->data.string_value[field->len]=0;
							}
							return_value=TRUE;

						} else {
							return(return_value);
						}
						} else {
							return(return_value);
						}
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						if(frs->range)
						{
							*field->data.integer_value=*frs->from.integer_value;
							return_value=TRUE;
						} else {
							return(return_value);
						}
						break;
					case SHORTV:
					case SSHORTV:
						if(frs->range)
						{
							*field->data.short_value=*frs->from.short_value;
							return_value=TRUE;
						} else {
							return(return_value);
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
							*field->data.float_value=*frs->from.float_value;
							return_value=TRUE;
						} else {
							return(return_value);
						}
						break;
					case BUTTONS:
						break;
					case BOOLNS:
						if(frs->range)
						{
							if(field->data.string_value==NULL)
							{
								field->data.string_value=Rmalloc(1);
								field->dlen=1;
							}
							*field->data.string_value=*frs->from.string_value;
							return_value=TRUE;
						} else {
							return(return_value);
						}
						break;
					default:
						prterr("Error field type [%d] is invalid for field [%s].",field->type,field->name);
						break;
				}
			} else {
				return(return_value);
			}
		} else {
			return(return_value);
		}
		}
	}
	} else {
		prterr("Error No Range information available in Screen [%s] [%s].",rs->module,rs->screen);
		return(return_value);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG setfilekeyFROMrangersrc returning [%d] ",(return_value));
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(return_value);
}
short xtestfilekeyTOrangersrc(short fileno,short keyno,RangeScreen *rs,int line,char *file)
{
	NRDfield *field=NULL;
	NRDkey *keyx=NULL;
	NRDpart *part=NULL;
	short x,y;
	FileRangeStruct *frs=NULL;
	short return_value=FALSE;
	char *filename=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG Testing Module [%s] File [%d] [%s] for key parts to range values Using Range Screen [%s] [%s] [%s] at line [%d] program [%s].",MODULENAME(fileno),fileno,FILENAME(fileno),KEYNAME(fileno,keyno),rs->module,rs->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	if((keyx=KEYNUM(fileno,keyno))==NULL)
	{
		prterr("Error cannot get key parts for file %d [%s] %d [%s].",
				fileno,FILENAME(fileno),keyno,KEYNAME(fileno,keyno));
		return(-1);
	}	
	if(rs!=NULL && rs->frs!=NULL)
	{
	if(keyx->part!=NULL)
	{
		for(x=0,part=keyx->part;x<keyx->numparts;++x,++part)
		{
		field=FLDNRD(fileno,part->name);
		if(field!=NULL)
		{
			filename=FILENAME(fileno);
			for(y=0,frs=rs->frs;y<rs->num;++y,++frs)
			{
				if(!RDAstrcmp(frs->filename,filename) &&
					!RDAstrcmp(frs->fieldname,field->name)) break;
			}
			if(y<rs->num)
			{
				switch(field->type)
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
						if(frs->to.string_value!=NULL) 
						{
							if(RDAstrcmp(field->data.string_value,frs->to.string_value)>0)
							{
								return(TRUE);
							} else {
								return_value=FALSE;
							}
						} else {
							return(FALSE);
						}
						} else {
							return(FALSE);
						}
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						if(frs->range)
						{
							if(*field->data.integer_value>*frs->to.integer_value)
							{
								return(TRUE);
							} else {
								return_value=FALSE;
							}
						} else {
							return(FALSE);
						}
						break;
					case SHORTV:
					case SSHORTV:
						if(frs->range)
						{
							if(*field->data.short_value>*frs->to.short_value)
							{
								return(TRUE);
							} else {
								return_value=FALSE;
							}
						} else {
							return(FALSE);
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
							if(*field->data.float_value>*frs->to.float_value)
							{
								return(TRUE);
							} else {
								return_value=FALSE;
							}
						} else {
							return(FALSE);
						}
						break;
					case BUTTONS:
						break;
					case BOOLNS:
						if(frs->range)
						{
							if((!*field->data.string_value && *frs->to.string_value)) 
							{
								return(TRUE);		
							} else {
								return_value=FALSE;
							}
						} else {
							return(FALSE);
						}
						break;
					default:
						prterr("Error field type [%d] is invalid for field [%s].",field->type,field->name);
						break;
				}
			} else {
				return(FALSE);
			}
		} else {
			return(TRUE);
		}
		}
	} else {
		return(FALSE);
	}
	} else {
		return(FALSE);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG testfilekeyTOrangersrc() returning [%d] ",return_value);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(return_value);
}
short xGTENRDRNG(short fileno,short keyno,RangeScreen *rs,short type,void (*SubFunc)(...),void *args,int line,char *file)
{
	short x,y;

#ifdef USE_RDA_DIAGNOSTICS
        if(diagsec || diagsec_field)
        {
                prterr("DIAG xGTENRDRNG() Get Record Greater Than or Equal To using Range Screen for file [%d] [%s] keyno [%d] at line [%d] program [%s].",fileno,FILENAME(fileno),keyno,line,file);
        }
#endif /* USE_RDA_DIAGNOSTICS */
	y=setfilekeyFROMrangersrc(fileno,keyno,rs);
	if(y)
	{
		if(type==0) /* Non-Security Flavor Needed */
		{
			x=GTENRD(fileno,keyno);
		} else { /* Security Flavor Needed */
			x=ADVGTENRDsec(fileno,keyno,SubFunc,args);
		}
	} else {
		if(type==0)
		{
			x=BEGNRD(fileno);
		} else {
			x=ADVBEGNRDsec(fileno,SubFunc,args);
		}
	}

	return(x);
}
short xNXTNRDRNG(short fileno,short keyno,RangeScreen *rs,short type,void (*SubFunc)(...),void *args,int line,char *file)
{
	short x;

#ifdef USE_RDA_DIAGNOSTICS
        if(diagsec || diagsec_field)
        {
                prterr("DIAG xNXTNRDRNG() Get Next Record using Range Screen for file [%d] [%s] keyno [%d] at line [%d] program [%s].",fileno,FILENAME(fileno),keyno,line,file);
        }
#endif /* USE_RDA_DIAGNOSTICS */
	if(type==0) /* Non-Security Flavor Needed */
	{
		x=NXTNRD(fileno,keyno);
	} else { /* Security Flavor Needed */
		x=ADVNXTNRDsec(fileno,keyno,SubFunc,args);
	}
	if(!x)
	{
		return(testfilekeyTOrangersrc(fileno,keyno,rs));
	} else {
		return(x);
	}
}
short xGuessCustomFieldType(char *mod,char *name,int line,char *file)
{
	short x=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG GuessCustomFieldType for Module [%s] Resource [%s] at line [%d] Program [%s].",(mod!=NULL ? mod:""),(name!=NULL ? name:""),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(RDAstrstr(name,"YYYY-MM")) x=0; 
	else if(RDAstrstr(name,"EIN")) x=1; 
	else if((RDAstrstr(name,"PO IDENTIFICATION")) || (RDAstrstr(name,"PO ID"))) x=2; 
	else if(RDAstrstr(name,"BUSINESS IDENTIFICATION")) x=3;
	else if((!RDAstrcmp(mod,"OCCTAX")) && (RDAstrstr(name,"LICENSE NUMBER"))) x=4;
	else if(RDAstrstr(name,"PROPERTY IDENTIFICATION")) x=5;
	else if(RDAstrstr(name,"REAL ESTATE IDENTIFICATION")) x=6;
	else if(RDAstrstr(name,"PERSONNEL IDENTIFICATION")) x=7;
	else if(RDAstrstr(name,"MISC IDENTIFICATION")) x=8;	
	else if(RDAstrstr(name,"CUSTOMER IDENTIFICATION")) x=22;	
	else if(RDAstrstr(name,"OWNER IDENTIFICATION")) x=10;	
	else if(RDAstrstr(name,"REQUISITION ID")) x=11;	
	else if(RDAstrstr(name,"RECEIPT IDENTIFICATION")) x=12;	
	else if(RDAstrstr(name,"POSITION IDENTIFICATION")) x=13;	
	else if(RDAstrstr(name,"CUSTOMER")) x=9;	
	else if(RDAstrstr(name,"CHECK PAYEE")) x=20;	
	else if(RDAstrstr(name,"PERMIT IDENTIFICATION")) x=21;	
	else if(RDAstrcmp(name,"PROBLEM ID")) x=23;	
	else if((RDAstrstr(name,"LATITUDE")) || (RDAstrstr(name,"LONGITUDE"))) x=24;
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG Returning GuessCustomFieldType [%d] for Module [%s] Resource [%s] at line [%d] Program [%s].",x,(mod!=NULL ? mod:""),(name!=NULL ? name:""),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(x);
}
