/* passarg.c - Functions for Passing Arguments Back and Forth */
#include<mix.hpp>

PassKey *xPassKeyNew(int line,char *file)
{
	PassKey *tmp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG PassKeyNew at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(PassKey));
	tmp->keyname=NULL;
	tmp->fileno=(-1);
	tmp->keyno=0;
	tmp->numflds=0;
	tmp->fields=NULL;
	return(tmp);
}
short xaddPassKeyField(PassKey *p,NRDfield *field,int line,char *file)
{
	NRDfield *f;
	char *value=NULL,*dashname=NULL;
	char *temp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG addPassKeyField at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(field->name!=NULL) 
	{
		dashname=adddashes(field->name);
		temp=RDA_GetEnv(dashname);
		value=stralloc(temp);
		RDA_UnSetEnv(dashname);
		if(dashname!=NULL) Rfree(dashname);
	}
	if(value!=NULL)
	{
		if(p->fields!=NULL)
		{
			p->fields=Rrealloc(p->fields,(p->numflds+1)*sizeof(NRDfield));
		} else {
			p->fields=Rmalloc(sizeof(NRDfield));
			p->numflds=0;
		}
		f=p->fields+p->numflds;
		if(!isEMPTY(field->name)) f->name=stralloc(field->name);
			else f->name=NULL;
		f->len=field->len;
		f->dlen=0;
		f->type=field->type;
		switch(field->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case PLAINTEXT:
			case ZIPCODE:
			case PHONE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case CUSTOMTYPE:
				if(!isEMPTY(value))
				{
					f->data.string_value=stralloc(value);
					f->dlen=RDAstrlen(f->data.string_value)+1;
				} else f->data.string_value=NULL;
				break;
			case CHARACTERS:
				f->data.string_value=Rmalloc(1);
				if(!isEMPTY(value))
				{
					*f->data.string_value=Ratoi(value);
				} else *f->data.string_value=0;
				f->dlen=1;
				break;
			case BOOLNS:
				f->data.string_value=Rmalloc(1);
				if(!isEMPTY(value))
				{
					if(Ratoi(value)!=0) *f->data.string_value=TRUE;
						else *f->data.string_value=0;
				} else *f->data.string_value=0;
				f->dlen=1;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				f->data.float_value=Rmalloc(sizeof(double));
				if(!isEMPTY(value))
				{
				*f->data.float_value=atof(value);
				} else *f->data.float_value=0;
				f->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				f->data.short_value=Rmalloc(sizeof(short));
				if(!isEMPTY(value))
				{
				*f->data.short_value=Ratoi(value);
				} else *f->data.short_value=0;
				f->dlen=sizeof(short);
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				f->data.integer_value=Rmalloc(sizeof(int));
				if(!isEMPTY(value))
				{
				*f->data.integer_value=Ratoi(value);
				} else *f->data.integer_value=0;
				f->dlen=sizeof(int);
				break;
			default:
				prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
				break;
		}
		if(value!=NULL) Rfree(value);
		return(++p->numflds);
	}
	if(value!=NULL) Rfree(value);
	return(-1);
}
void xFreePassKey(PassKey *p,int line,char *file)
{
	int x;
	NRDfield *f;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG FreePassKey for [%s] at line [%d] program [%s].",p->keyname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(p!=NULL)
	{
		if(p->keyname!=NULL) Rfree(p->keyname);
		if(p->fields!=NULL)
		{
			for(x=0,f=p->fields;x<p->numflds;++x,++f) FreeNRDfield(f);
			Rfree(p->fields);
		}
		Rfree(p);
	}
}
PassKey *xREADPassKey(short fileno,int line,char *file)
{
	PassKey *tmp=NULL;
	char *temp=NULL;
	int x;
	NRDkey *keys=NULL,*key;
	NRDfield *field=NULL;
	NRDpart *part;

	tmp=PassKeyNew();
	temp=RDA_GetEnv("KEYNAME");
	if(!isEMPTY(temp)) tmp->keyname=stralloc(temp);
		else tmp->keyname=stralloc(KEYNAME(fileno,1));
	RDA_UnSetEnv("KEYNAME");
	tmp->fileno=fileno;
	tmp->keyno=KEYNUMBER(fileno,tmp->keyname);
	if(tmp->keyno<1) tmp->keyno=1;
	keys=KEYPOINTER(fileno);
	if(keys!=NULL)
	{
		key=keys+(tmp->keyno-1);
		if(key!=NULL)
		{
			if(key->part!=NULL)
			{
				for(x=0,part=key->part;x<key->numparts;++x,++part)
				{
					if(!isEMPTY(part->name))
					{
						field=FLDNRD(fileno,part->name);
					} else field=NULL;
					if(field!=NULL) 
					{
						addPassKeyField(tmp,field);
					}
				}
			}
		}
	}
	return(tmp);
}
void xApplyPassKey(PassKey *p,int line,char *file)
{
	int x;
	NRDfield *f,*field=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG ApplyPassKey at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(p->fields!=NULL)
	{
		for(x=0,f=p->fields;x<p->numflds;++x,++f)
		{
			field=FLDNRD(p->fileno,f->name);
			if(field!=NULL)
			{
				NRD2NRD(f,field);
			} else {
				prterr("Error Field [%s] not found in Module [%s] File [%s] at line [%d] program [%s].",f->name,MODULENAME(p->fileno),FILENAME(p->fileno),line,file);
			}
		}
	}
}
void PASSKEYinitfunc(MakeBrowseList *blist)
{
	int x;
	NRDfield *field=NULL,*fld=NULL;

	if(blist->passkey!=NULL)
	{
		if(blist->passkey->fields!=NULL)
		{
			for(x=0,field=blist->passkey->fields;
				x<blist->passkey->numflds;++x,++field)
			{
				fld=FLDNRD(blist->fileno,field->name);
				if(fld!=NULL)
				{
					NRD2NRD(field,fld);
				} else {
					prterr("Error Field [%s] not found in PASSKEYinitfunc.",
						field->name);
				}
			}
		}
	}
}
short PASSKEYselectfunction(MakeBrowseList *blist)
{
	int x;
	NRDfield *field=NULL,*fld=NULL;

	if(blist->passkey!=NULL)
	{
		if(blist->passkey->fields!=NULL)
		{
			for(x=0,field=blist->passkey->fields;
				x<blist->passkey->numflds;++x,++field)
			{
				fld=FLDNRD(blist->fileno,field->name);
				if(fld!=NULL)
				{
					switch(field->type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
						case DATES:
						case TIMES:
						case SOCSECNUM:
						case PLAINTEXT:
						case ZIPCODE:
						case PHONE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case CUSTOMTYPE:
							switch(fld->type)
							{
								case SCROLLEDTEXT:
								case VARIABLETEXT:
								case DATES:
								case TIMES:
								case SOCSECNUM:
								case PLAINTEXT:
								case ZIPCODE:
								case PHONE:
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case CUSTOMTYPE:
									if(RDAstrcmp(fld->data.string_value,field->data.string_value)) return(FALSE);
									break;
								case CHARACTERS:
								case BOOLNS:
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case SDECIMALV:
								case SHORTV:
								case SSHORTV:
								case SCROLLEDLIST:
								case LONGV:
								case SLONGV:
									prterr("Error Incompatiable Field Type [%d] in Field [%s] between the PassKey and File [%s][%s].",fld->type,fld->name,MODULENAME(blist->fileno),FILENAME(blist->fileno));
									break;
								default:
									prterr("Error Field Type [%d] is invalid in Field [%s].",fld->type,fld->name);
									break;
							}
							break;
						case CHARACTERS:
						case BOOLNS:
							switch(fld->type)
							{
								case SCROLLEDTEXT:
								case VARIABLETEXT:
								case DATES:
								case TIMES:
								case SOCSECNUM:
								case PLAINTEXT:
								case ZIPCODE:
								case PHONE:
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case CUSTOMTYPE:
									prterr("Error Incompatiable Field Type [%d] in Field [%s] between the PassKey and File [%s][%s].",fld->type,fld->name,MODULENAME(blist->fileno),FILENAME(blist->fileno));
									break;
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case SDECIMALV:
									if(*fld->data.float_value!=
										*field->data.string_value) 
										return(FALSE);
									break;
								case SHORTV:
								case SSHORTV:
									if(*fld->data.short_value!=
										*field->data.string_value) 
										return(FALSE);
									break;
								case SCROLLEDLIST:
								case LONGV:
								case SLONGV:
									if(*fld->data.integer_value!=
										*field->data.string_value) 
										return(FALSE);
									break;
								case CHARACTERS:
								case BOOLNS:
									if(*fld->data.string_value!=
										*field->data.string_value) 
										return(FALSE);
									break;
								default:
									prterr("Error Incompatible Field Type [%d] is invalid in Field [%s].",fld->type,fld->name);
									break;
							}
							break;
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case SDECIMALV:
							switch(fld->type)
							{
								case SCROLLEDTEXT:
								case VARIABLETEXT:
								case DATES:
								case TIMES:
								case SOCSECNUM:
								case PLAINTEXT:
								case ZIPCODE:
								case PHONE:
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case CUSTOMTYPE:
									prterr("Error Incompatiable Field Type [%d] in Field [%s] between the PassKey and File [%s][%s].",fld->type,fld->name,MODULENAME(blist->fileno),FILENAME(blist->fileno));
									break;
								case CHARACTERS:
								case BOOLNS:
									if(*fld->data.string_value!=
										*field->data.float_value)
										return(FALSE);
									break;
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case SDECIMALV:
									if(*fld->data.float_value!=
										*field->data.float_value)
										return(FALSE);
									break;
								case SHORTV:
								case SSHORTV:
									if(*fld->data.short_value!=
										*field->data.float_value)
										return(FALSE);
									break;
								case SCROLLEDLIST:
								case LONGV:
								case SLONGV:
									if(*fld->data.integer_value!=
										*field->data.float_value)
										return(FALSE);
									break;
								default:
									prterr("Error Incompatible Field Type [%d] is invalid in Field [%s].",fld->type,fld->name);
									break;
							}
							break;
						case SHORTV:
						case SSHORTV:
							switch(fld->type)
							{
								case SCROLLEDTEXT:
								case VARIABLETEXT:
								case DATES:
								case TIMES:
								case SOCSECNUM:
								case PLAINTEXT:
								case ZIPCODE:
								case PHONE:
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case CUSTOMTYPE:
									prterr("Error Incompatiable Field Type [%d] in Field [%s] between the PassKey and File [%s][%s].",fld->type,fld->name,MODULENAME(blist->fileno),FILENAME(blist->fileno));
									break;
								case CHARACTERS:
								case BOOLNS:
									if(*fld->data.string_value!=
										*field->data.short_value)
										return(FALSE);
									break;
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case SDECIMALV:
									if(*fld->data.float_value!=
										*field->data.short_value)
										return(FALSE);
									break;
								case SHORTV:
								case SSHORTV:
									if(*fld->data.short_value!=
										*field->data.short_value)
										return(FALSE);
									break;
								case SCROLLEDLIST:
								case LONGV:
								case SLONGV:
									if(*fld->data.integer_value!=
										*field->data.short_value)
										return(FALSE);
									break;
								default:
									prterr("Error Incompatible Field Type [%d] is invalid in Field [%s].",fld->type,fld->name);
									break;
							}
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							switch(fld->type)
							{
								case SCROLLEDTEXT:
								case VARIABLETEXT:
								case DATES:
								case TIMES:
								case SOCSECNUM:
								case PLAINTEXT:
								case ZIPCODE:
								case PHONE:
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case CUSTOMTYPE:
									prterr("Error Incompatiable Field Type [%d] in Field [%s] between the PassKey and File [%s][%s].",fld->type,fld->name,MODULENAME(blist->fileno),FILENAME(blist->fileno));
									break;
								case CHARACTERS:
								case BOOLNS:
									if(*fld->data.string_value!=
										*field->data.integer_value)
										return(FALSE);
									break;
								case DECIMALV:
								case DOUBLEV:
								case SDOUBLEV:
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case SDECIMALV:
									if(*fld->data.float_value!=
										*field->data.integer_value)
										return(FALSE);
									break;
								case SHORTV:
								case SSHORTV:
									if(*fld->data.short_value!=
										*field->data.integer_value)
										return(FALSE);
									break;
								case SCROLLEDLIST:
								case LONGV:
								case SLONGV:
									if(*fld->data.integer_value!=
										*field->data.integer_value)
										return(FALSE);
									break;
								default:
									prterr("Error Incompatible Field Type [%d] is invalid in Field [%s].",fld->type,fld->name);
									break;
							}
							break;
						default:
							prterr("Error Field Type [%d] is invalid in Field [%s].",field->type,field->name);
							break;
					}
				} else {
					prterr("Error Field [%s] not found in PASSKEYselectfunction.",field->name);
				}
			}
		}
	}
	return(TRUE);
}
void xApplyPassKey2Rsrc(RDArsrc *r,PassKey *p,short type,int line,char *file)
{
	int x;
	NRDfield *field;
	char *name=NULL,*filename=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG ApplyPassKey2Rsrc at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(p->fields!=NULL)
	{
		filename=FILENAME(p->fileno);
		for(x=0,field=p->fields;x<p->numflds;++x,++field)
		{
			if(!type)
			{
				if(name!=NULL) 
				{ 
					name=Rrealloc(name,RDAstrlen(field->name)+5+RDAstrlen(filename));
				} else name=Rmalloc(RDAstrlen(field->name)+5+RDAstrlen(filename));
				sprintf(name,"[%s][%s]",filename,field->name);
			} else {
				if(name!=NULL) 
				{ 
					name=Rrealloc(name,RDAstrlen(field->name)+3);
				} else name=Rmalloc(RDAstrlen(field->name)+3);
				sprintf(name,"%s",field->name);
			}
			switch(field->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case ZIPCODE:
				case PHONE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case CUSTOMTYPE:
					FINDRSCSETSTRING(r,name,field->data.string_value);
					break;
				case CHARACTERS:
				case BOOLNS:
					FINDRSCSETCHAR(r,name,*field->data.string_value);
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					FINDRSCSETDOUBLE(r,name,*field->data.float_value);
					break;
				case SHORTV:
				case SSHORTV:
					FINDRSCSETSHORT(r,name,*field->data.short_value);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					FINDRSCSETINT(r,name,*field->data.integer_value);
					break;
				default:
					prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
					break;
			}
		}
	}
}
APPlib *xFile2PassKeyAPPlib(char *keyname,short fileno,short keyno,
	int line,char *file)
{
	APPlib *tmp=NULL;
	char *temp=NULL,*mod=NULL,*fil=NULL,*kname=NULL,*dashname=NULL;
	short x=0;
	NRDfield *field;
	NRDkey *key;
	NRDpart *part;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG File2PassKeyAPPlib for Keyname [%s] from fileno [%d] keyno [%d] at line [%d] program [%s].",keyname,fileno,keyno,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	tmp=APPlibNEW();
	mod=MODULENAME(fileno);
	fil=FILENAME(fileno);
	temp=Rmalloc(13+RDAstrlen(mod));
	sprintf(temp,"FROM_MODULE=%s",mod);
	addAPPlib(tmp,temp);
	if(temp!=NULL) Rfree(temp);
	temp=Rmalloc(15+RDAstrlen(fil));
	sprintf(temp,"FROM_FILENAME=%s",fil);
	addAPPlib(tmp,temp);
	if(temp!=NULL) Rfree(temp);
	kname=KEYNAME(fileno,keyno);
	temp=Rmalloc(14+RDAstrlen(kname));
	sprintf(temp,"FROM_KEYNAME=%s",kname);
	addAPPlib(tmp,temp);
	if(temp!=NULL) Rfree(temp);
	temp=Rmalloc(9+RDAstrlen(keyname));
	sprintf(temp,"KEYNAME=%s",keyname);
	addAPPlib(tmp,temp);
	if(temp!=NULL) Rfree(temp);
	key=KEYNUM(fileno,keyno);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			if(!isEMPTY(part->name))
			{
				field=FLDNRD(fileno,part->name);
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
					case ZIPCODE:
					case PHONE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case CUSTOMTYPE:
						temp=Rmalloc(RDAstrlen(field->name)+2+RDAstrlen(field->data.string_value));
						dashname=adddashes(field->name);
						sprintf(temp,"%s=%s",dashname,(field->data.string_value!=NULL ? field->data.string_value:""));
						addAPPlib(tmp,temp);
						if(dashname!=NULL) Rfree(dashname);
						if(temp!=NULL) Rfree(temp);
						break;
					case CHARACTERS:
						sprintf(stemp,"%c",*field->data.string_value);
						temp=Rmalloc(RDAstrlen(field->name)+2+RDAstrlen(stemp));
						dashname=adddashes(field->name);
						sprintf(temp,"%s=%s",dashname,stemp);
						addAPPlib(tmp,temp);
						if(dashname!=NULL) Rfree(dashname);
						if(temp!=NULL) Rfree(temp);
						break;
					case BOOLNS:
						sprintf(stemp,"%d",(*field->data.string_value ? 
							1:0));
						temp=Rmalloc(RDAstrlen(field->name)+2+RDAstrlen(stemp));
						dashname=adddashes(field->name);
						sprintf(temp,"%s=%s",dashname,stemp);
						addAPPlib(tmp,temp);
						if(dashname!=NULL) Rfree(dashname);
						if(temp!=NULL) Rfree(temp);
						break;
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case SDECIMALV:
						sprintf(stemp,"%f",*field->data.float_value);
						temp=Rmalloc(RDAstrlen(field->name)+2+RDAstrlen(stemp));
						dashname=adddashes(field->name);
						sprintf(temp,"%s=%s",dashname,stemp);
						addAPPlib(tmp,temp);
						if(dashname!=NULL) Rfree(dashname);
						if(temp!=NULL) Rfree(temp);
						break;
					case SHORTV:
					case SSHORTV:
						sprintf(stemp,"%d",*field->data.short_value);
						temp=Rmalloc(RDAstrlen(field->name)+2+RDAstrlen(stemp));
						dashname=adddashes(field->name);
						sprintf(temp,"%s=%s",dashname,stemp);
						addAPPlib(tmp,temp);
						if(dashname!=NULL) Rfree(dashname);
						if(temp!=NULL) Rfree(temp);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						sprintf(stemp,"%d",*field->data.integer_value);
						dashname=adddashes(field->name);
						temp=Rmalloc(RDAstrlen(dashname)+2+RDAstrlen(stemp));
						sprintf(temp,"%s=%s",dashname,stemp);
						addAPPlib(tmp,temp);
						if(temp!=NULL) Rfree(temp);
						if(dashname!=NULL) Rfree(dashname);
						break;
					default:
						prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
						break;
				}
			}
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG File2PassKeyAPPlib displaying APPlib Structure at line [%d] program [%s].",line,file);
		SEEAPPlib(tmp);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(tmp);
}
APPlib *xRsrc2PassKeyAPPlib(RDArsrc *mtnrsrc,char *keyname,short fileno,
	short keyno,short type,int line,char *file)
{
	APPlib *tmp=NULL;
	char *temp=NULL,*mod=NULL,*fil=NULL,*kname=NULL,*dashname=NULL;
	char *rsrcname=NULL;
	short x=0;
	NRDfield *field;
	NRDkey *key;
	NRDpart *part;
	RDArmem *rsc=NULL;
	int y;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG Rsrc2PassKeyAPPlib for Keyname [%s] from fileno [%d] keyno [%d] at line [%d] program [%s].",keyname,fileno,keyno,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	tmp=APPlibNEW();
	mod=MODULENAME(fileno);
	fil=FILENAME(fileno);
	temp=Rmalloc(13+RDAstrlen(mod));
	sprintf(temp,"FROM_MODULE=%s",mod);
	addAPPlib(tmp,temp);
	if(temp!=NULL) Rfree(temp);
	temp=Rmalloc(15+RDAstrlen(fil));
	sprintf(temp,"FROM_FILENAME=%s",fil);
	addAPPlib(tmp,temp);
	if(temp!=NULL) Rfree(temp);
	kname=KEYNAME(fileno,keyno);
	temp=Rmalloc(14+RDAstrlen(kname));
	sprintf(temp,"FROM_KEYNAME=%s",kname);
	addAPPlib(tmp,temp);
	if(temp!=NULL) Rfree(temp);
	temp=Rmalloc(9+RDAstrlen(keyname));
	sprintf(temp,"KEYNAME=%s",keyname);
	addAPPlib(tmp,temp);
	if(temp!=NULL) Rfree(temp);
	key=KEYNUM(fileno,keyno);
	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			if(!isEMPTY(part->name))
			{
				field=FLDNRD(fileno,part->name);
			} else field=NULL;
			if(field!=NULL) 
			{
				if(!type)
				{
					rsrcname=Rmalloc(RDAstrlen(fil)+RDAstrlen(field->name)+5);
					sprintf(rsrcname,"[%s][%s]",fil,field->name);
				} else rsrcname=stralloc(field->name);
				readwidget(mtnrsrc,rsrcname);
				y=FINDRSC(mtnrsrc,rsrcname);
				if(y!=(-1))
				{
					rsc=mtnrsrc->rscs+y;
					switch(field->type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
						case DATES:
						case TIMES:
						case SOCSECNUM:
						case PLAINTEXT:
						case ZIPCODE:
						case PHONE:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
						case CUSTOMTYPE:
							temp=Rmalloc(RDAstrlen(field->name)+2+RDAstrlen(rsc->value.string_value));
							dashname=adddashes(field->name);
							sprintf(temp,"%s=%s",dashname,rsc->value.string_value);
							addAPPlib(tmp,temp);
							if(dashname!=NULL) Rfree(dashname);
							if(temp!=NULL) Rfree(temp);
							break;
						case CHARACTERS:
							sprintf(stemp,"%c",*rsc->value.string_value);
							temp=Rmalloc(RDAstrlen(field->name)+2+RDAstrlen(stemp));
							dashname=adddashes(field->name);
							sprintf(temp,"%s=%s",dashname,stemp);
							addAPPlib(tmp,temp);
							if(dashname!=NULL) Rfree(dashname);
							if(temp!=NULL) Rfree(temp);
							break;
						case BOOLNS:
							sprintf(stemp,"%d",(*rsc->value.string_value ? 
								1:0));
							temp=Rmalloc(RDAstrlen(field->name)+2+RDAstrlen(stemp));
							dashname=adddashes(field->name);
							sprintf(temp,"%s=%s",dashname,stemp);
							addAPPlib(tmp,temp);
							if(dashname!=NULL) Rfree(dashname);
							if(temp!=NULL) Rfree(temp);
							break;
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case SDECIMALV:
							sprintf(stemp,"%f",*rsc->value.float_value);
							temp=Rmalloc(RDAstrlen(field->name)+2+RDAstrlen(stemp));
							dashname=adddashes(field->name);
							sprintf(temp,"%s=%s",dashname,stemp);
							addAPPlib(tmp,temp);
							if(dashname!=NULL) Rfree(dashname);
							if(temp!=NULL) Rfree(temp);
							break;
						case SHORTV:
						case SSHORTV:
							sprintf(stemp,"%d",*rsc->value.short_value);
							temp=Rmalloc(RDAstrlen(field->name)+2+RDAstrlen(stemp));
							dashname=adddashes(field->name);
							sprintf(temp,"%s=%s",dashname,stemp);
							addAPPlib(tmp,temp);
							if(dashname!=NULL) Rfree(dashname);
							if(temp!=NULL) Rfree(temp);
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							sprintf(stemp,"%d",*rsc->value.integer_value);
							dashname=adddashes(field->name);
							temp=Rmalloc(RDAstrlen(dashname)+2+RDAstrlen(stemp));
							sprintf(temp,"%s=%s",dashname,stemp);
							addAPPlib(tmp,temp);
							if(temp!=NULL) Rfree(temp);
							if(dashname!=NULL) Rfree(dashname);
							break;
						default:
							prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
							break;
					}
				}
				if(rsrcname!=NULL) Rfree(rsrcname);
			}
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG Rsrc2PassKeyAPPlib displaying APPlib Structure at line [%d] program [%s].",line,file);
		SEEAPPlib(tmp);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(tmp);
}
void xMTNSCRN_EXECUTEPROGRAM(char *progx,RDArsrc *mtnrsrc,char *keyname,
	short fileno,short keyno,short type,APPlib *args,void (*func)(...),
	int line,char *file)
{
	APPlib *envpx=NULL;

	envpx=ADVRsrc2PassKeyAPPlib(mtnrsrc,keyname,fileno,keyno,type);
	if(func!=NULL)
	{
/*lint -e746 */
		func(mtnrsrc,keyname,fileno,keyno,type,envpx);
/*lint +e746 */
	}
	ADVExecuteProgram(progx,args,envpx);
	if(envpx!=NULL) freeapplib(envpx);
}
void xBROWSESCRN_EXECUTEPROGRAM(char *progx,void *targetkey,char *keyname,
	short fileno,short keyno,short type,APPlib *args,void (*func)(...),
	int line,char *file)
{
	APPlib *envpx=NULL;


	ZERNRD(fileno);
	if(EQLNRDKEYsec(fileno,keyno,targetkey))
	{
		KEYNRD(fileno,keyno);
	}
	envpx=File2PassKeyAPPlib(keyname,fileno,keyno);
	if(func!=NULL)
	{
/*lint -e746 */
		func(keyname,fileno,keyno,type,envpx);
/*lint +e746 */
	}
	ADVExecuteProgram(progx,args,envpx);
	if(envpx!=NULL) freeapplib(envpx);
}
void MakeBrowseListTotals(RDArsrc *r,short fileno,RDArsrc *d)
{
	char *name=NULL;
	short nofields=0,x=0;
	NRDfield *fields=NULL,*fldx=NULL;
	RDAvirtual *v=NULL;

	if(r->scn!=NULL)
	{
		nofields=NUMFLDS(fileno);
		fields=FLDPOINTER(fileno);
		if(fields!=NULL)
		{
			for(x=0,fldx=fields;x<nofields;++x,++fldx)
			{
				switch(fldx->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case ZIPCODE:
						break;
					case BOOLNS:
					case CHARACTERS:
						name=Rmalloc(RDAstrlen(fldx->name)+12);
						sprintf(name,"%s TOTAL",(fldx->name!=NULL ? fldx->name:""));
						if((FINDWDGT(r->scn,name))!=(-1))
						{
							if(!alreadyrsrc(name,r))
							{
								makefieldrsrc(r,name,LONGV,8,FALSE);
							}
						}
						if(name!=NULL) Rfree(name);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						name=Rmalloc(RDAstrlen(fldx->name)+12);
						sprintf(name,"%s TOTAL",(fldx->name!=NULL ? fldx->name:""));
						if((FINDWDGT(r->scn,name))!=(-1))
						{
							if(!alreadyrsrc(name,r))
							{
								makefieldrsrc(r,name,fldx->type,fldx->len,FALSE);
							}
						}
						if(name!=NULL) Rfree(name);
						break;
					case SHORTV:
					case SSHORTV:
						name=Rmalloc(RDAstrlen(fldx->name)+12);
						sprintf(name,"%s TOTAL",(fldx->name!=NULL ? fldx->name:""));
						if((FINDWDGT(r->scn,name))!=(-1))
						{
							if(!alreadyrsrc(name,r))
							{
								makefieldrsrc(r,name,fldx->type,fldx->len,FALSE);
							}
						}
						if(name!=NULL) Rfree(name);
						break;
					case LONGV:
					case SLONGV:
					case SCROLLEDLIST:
						name=Rmalloc(RDAstrlen(fldx->name)+12);
						sprintf(name,"%s TOTAL",(fldx->name!=NULL ? fldx->name:""));
						if((FINDWDGT(r->scn,name))!=(-1))
						{
							if(!alreadyrsrc(name,r))
							{
								makefieldrsrc(r,name,fldx->type,fldx->len,FALSE);
							}
						}
						if(name!=NULL) Rfree(name);
						break;
					default:
					case BUTTONS:
						break;
				}
			}
		}
		if(d!=NULL)
		{
		if(d->virflds!=NULL)
		{
			for(x=0,v=d->virflds;x<d->num;++x,++v)
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
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case ZIPCODE:
						break;
					case BOOLNS:
					case CHARACTERS:
						name=Rmalloc(RDAstrlen(v->name)+12);
						sprintf(name,"%s TOTAL",(v->name!=NULL ? v->name:""));
						if((FINDWDGT(r->scn,name))!=(-1))
						{
							if(!alreadyrsrc(name,r))
							{
								makefieldrsrc(r,name,LONGV,8,FALSE);
							}
						}
						if(name!=NULL) Rfree(name);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						name=Rmalloc(RDAstrlen(v->name)+12);
						sprintf(name,"%s TOTAL",(v->name!=NULL ? v->name:""));
						if((FINDWDGT(r->scn,name))!=(-1))
						{
							if(!alreadyrsrc(name,r))
							{
								makefieldrsrc(r,name,v->type,v->len,FALSE);
							}
						}
						if(name!=NULL) Rfree(name);
						break;
					case SHORTV:
					case SSHORTV:
						name=Rmalloc(RDAstrlen(v->name)+12);
						sprintf(name,"%s TOTAL",(v->name!=NULL ? v->name:""));
						if((FINDWDGT(r->scn,name))!=(-1))
						{
							if(!alreadyrsrc(name,r))
							{
								makefieldrsrc(r,name,v->type,v->len,FALSE);
							}
						}
						if(name!=NULL) Rfree(name);
						break;
					case LONGV:
					case SLONGV:
					case SCROLLEDLIST:
						name=Rmalloc(RDAstrlen(v->name)+12);
						sprintf(name,"%s TOTAL",(v->name!=NULL ? v->name:""));
						if((FINDWDGT(r->scn,name))!=(-1))
						{
							if(!alreadyrsrc(name,r))
							{
								makefieldrsrc(r,name,v->type,v->len,FALSE);
							}
						}
						if(name!=NULL) Rfree(name);
						break;
					default:
					case BUTTONS:
						break;
				}
			}
		}
		}
	} else {
		/* Screen unavailable */
	}
}
void ZeroBrowseListTotals(RDArsrc *r,short fileno,RDArsrc *d)
{
	char *name=NULL;
	short nofields=0,x=0;
	NRDfield *fields=NULL,*fldx=NULL;
	double dzero=0.0;
	int izero=0;
	short szero=0,rs=0;
	RDAvirtual *v=NULL;

	nofields=NUMFLDS(fileno);
	fields=FLDPOINTER(fileno);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			switch(fldx->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case ZIPCODE:
					break;
				case BOOLNS:
				case CHARACTERS:
					name=Rmalloc(RDAstrlen(fldx->name)+12);
					sprintf(name,"%s TOTAL",(fldx->name!=NULL ? fldx->name:""));
					rs=FINDRSC(r,name);
					if(rs!=(-1))
					{
						FINDRSCSETINT(r,name,izero);
					}
					if(name!=NULL) Rfree(name);
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					name=Rmalloc(RDAstrlen(fldx->name)+12);
					sprintf(name,"%s TOTAL",(fldx->name!=NULL ? fldx->name:""));
					rs=FINDRSC(r,name);
					if(rs!=(-1))
					{
						FINDRSCSETDOUBLE(r,name,dzero);
					}
					if(name!=NULL) Rfree(name);
					break;
				case SHORTV:
				case SSHORTV:
					name=Rmalloc(RDAstrlen(fldx->name)+12);
					sprintf(name,"%s TOTAL",(fldx->name!=NULL ? fldx->name:""));
					rs=FINDRSC(r,name);
					if(rs!=(-1))
					{
						FINDRSCSETSHORT(r,name,szero);
					}
					if(name!=NULL) Rfree(name);
					break;
				case LONGV:
				case SLONGV:
				case SCROLLEDLIST:
					name=Rmalloc(RDAstrlen(fldx->name)+12);
					sprintf(name,"%s TOTAL",(fldx->name!=NULL ? fldx->name:""));
					rs=FINDRSC(r,name);
					if(rs!=(-1))
					{
						FINDRSCSETINT(r,name,izero);
					}
					if(name!=NULL) Rfree(name);
					break;
				default:
				case BUTTONS:
					break;
			}
		}
	}
	if(d!=NULL)
	{
	if(d->virflds!=NULL)
	{
		for(x=0,v=d->virflds;x<d->num;++x,++v)
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
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case ZIPCODE:
					break;
				case BOOLNS:
				case CHARACTERS:
					name=Rmalloc(RDAstrlen(v->name)+12);
					sprintf(name,"%s TOTAL",(v->name!=NULL ? v->name:""));
					rs=FINDRSC(r,name);
					if(rs!=(-1))
					{
						FINDRSCSETINT(r,name,izero);
					}
					if(name!=NULL) Rfree(name);
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					name=Rmalloc(RDAstrlen(v->name)+12);
					sprintf(name,"%s TOTAL",(v->name!=NULL ? v->name:""));
					rs=FINDRSC(r,name);
					if(rs!=(-1))
					{
						FINDRSCSETDOUBLE(r,name,dzero);
					}
					if(name!=NULL) Rfree(name);
					break;
				case SHORTV:
				case SSHORTV:
					name=Rmalloc(RDAstrlen(v->name)+12);
					sprintf(name,"%s TOTAL",(v->name!=NULL ? v->name:""));
					rs=FINDRSC(r,name);
					if(rs!=(-1))
					{
						FINDRSCSETSHORT(r,name,szero);
					}
					if(name!=NULL) Rfree(name);
					break;
				case LONGV:
				case SLONGV:
				case SCROLLEDLIST:
					name=Rmalloc(RDAstrlen(v->name)+12);
					sprintf(name,"%s TOTAL",(v->name!=NULL ? v->name:""));
					rs=FINDRSC(r,name);
					if(rs!=(-1))
					{
						FINDRSCSETINT(r,name,izero);
					}
					if(name!=NULL) Rfree(name);
					break;
				default:
				case BUTTONS:
					break;
			}
		}
	}
	}
}
void IncrementBrowseListTotals(RDArsrc *r,short fileno,RDArsrc *d)
{
	char *name=NULL;
	short nofields=0,x=0,y=0,rs=0;
	NRDfield *fields=NULL,*fldx=NULL;
	RDArmem *member=NULL;
	RDAvirtual *v=NULL;

	nofields=NUMFLDS(fileno);
	fields=FLDPOINTER(fileno);
	if(fields!=NULL)
	{
		for(x=0,fldx=fields;x<nofields;++x,++fldx)
		{
			name=Rmalloc(RDAstrlen(fldx->name)+12);
			sprintf(name,"%s TOTAL",(fldx->name!=NULL ? fldx->name:""));
			rs=FINDRSC(r,name);
			if(rs!=(-1))
			{
				switch(fldx->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case ZIPCODE:
						break;
					case BOOLNS:
					case CHARACTERS:
						y=FINDRSC(r,name);
						member=r->rscs+y;
						if(RDAstrcmp(fldx->name,"DELETEFLAG"))
						{
							*member->value.integer_value+=(*fldx->data.string_value ? 1:0);
						} else *member->value.integer_value+=(*fldx->data.string_value ? 0:1);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						y=FINDRSC(r,name);
						member=r->rscs+y;
						*member->value.float_value+=*fldx->data.float_value;
						break;
					case SHORTV:
					case SSHORTV:
						y=FINDRSC(r,name);
						member=r->rscs+y;
						*member->value.short_value+=*fldx->data.short_value;
						break;
					case LONGV:
					case SLONGV:
					case SCROLLEDLIST:
						y=FINDRSC(r,name);
						member=r->rscs+y;
						*member->value.integer_value+=*fldx->data.integer_value;
						break;
					default:
					case BUTTONS:
						break;
				}
				updatersrc(r,name);
			}
			if(name!=NULL) Rfree(name);
		}
	}
	if(d!=NULL)
	{
		if(d->virflds!=NULL)
		{
			for(x=0,v=d->virflds;x<d->num;++x,++v)
			{
			name=Rmalloc(RDAstrlen(v->name)+12);
			sprintf(name,"%s TOTAL",(v->name!=NULL ? v->name:""));
			rs=FINDRSC(r,name);
			if(rs!=(-1))
			{
				v->computed=FALSE;
				computevirtual(v,SCRNvirtualSubData,d);
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
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case ZIPCODE:
						break;
					case BOOLNS:
					case CHARACTERS:
						y=FINDRSC(r,name);
						member=r->rscs+y;
						*member->value.integer_value+=(*v->value.string_value ? 1:0);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						y=FINDRSC(r,name);
						member=r->rscs+y;
						*member->value.float_value+=*v->value.float_value;
						break;
					case SHORTV:
					case SSHORTV:
						y=FINDRSC(r,name);
						member=r->rscs+y;
						*member->value.short_value+=*v->value.short_value;
						break;
					case LONGV:
					case SLONGV:
					case SCROLLEDLIST:
						y=FINDRSC(r,name);
						member=r->rscs+y;
						*member->value.integer_value+=*v->value.integer_value;
						break;
					default:
					case BUTTONS:
						break;
				}
				updatersrc(r,name);
			}
			if(name!=NULL) Rfree(name);
			}
		}
	}
}
