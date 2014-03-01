/* dochg5.c - Actual Functions that manage the change */
#include<dfchg.hpp>
extern CHGstruct *CHGSTRUCT;

short Write_Change(CHGfile *c)
{
	NRDfield *field=NULL,*fields=NULL,*wfields=NULL,*wfield=NULL;
	NRDkey *key=NULL;
	NRDpart *part=NULL;
	int x,y;
	CHGfield *fld=NULL;
	char write2file=FALSE,changedkeypart=FALSE,deleterecord=FALSE;
	char *temp=NULL;
	short numflds=0;

	if(diagapps)
	{
		prterr("DIAG Write_Change...");
	}
	if(c!=NULL)
	{
		if(c->chgflds!=NULL)
		{
			numflds=NUMFLDS(c->fileno);
			fields=FLDPOINTER(c->fileno);
			wfields=FLDPOINTER(c->sort->fileno);
			for(x=0,field=fields,wfield=wfields;x<numflds;
				++x,++field,++wfield)
			{
				NRD2NRD(field,wfield);
			}
			for(x=0,fld=c->chgflds;x<c->numflds;++x,++fld)
			{
				field=FLDNRD(c->sort->fileno,fld->fieldname);
				if(field!=NULL)
				{
					if(CHGSTRUCT->simple_from==TRUE)
					{
						switch(CHGSTRUCT->type)
						{	
							case VARIABLETEXT:
							case PLAINTEXT:
							case SCROLLEDTEXT:
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case ZIPCODE:
							case PHONE:
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								if(!RDAstrcmp(CHGSTRUCT->from.string_value,field->data.string_value))
								{
									if(fld->keypart==TRUE) changedkeypart=TRUE;
									write2file=TRUE;
									if(CHGSTRUCT->simple_to==FALSE)
									{ 
										temp=PP_EVALUATEstr(CHGSTRUCT->expression,CHGSTRUCT->SubFunc,CHGSTRUCT->SubFuncArgs);
										if(field->data.string_value!=NULL) Rfree(field->data.string_value);
										field->dlen=0;
										if(temp!=NULL)
										{
											field->data.string_value=stralloc(temp);
											field->dlen=strlen(field->data.string_value)+1;
											Rfree(temp);
										}
									} else if(!isEMPTY(CHGSTRUCT->to.string_value))
									{
										QUICKALLOC(field->data.string_value,field->dlen,CHGSTRUCT->to_dlen);
										memcpy(field->data.string_value,CHGSTRUCT->to.string_value,CHGSTRUCT->to_dlen);
									} else if(field->data.string_value!=NULL)
									{
										memset(field->data.string_value,0,field->dlen);
									}	
								}
								break;
							case BOOLNS:
							case CHARACTERS:
								if(*CHGSTRUCT->from.string_value==*field->data.string_value)
								{
									if(fld->keypart==TRUE) changedkeypart=TRUE;
									write2file=TRUE;
									if(CHGSTRUCT->simple_to==FALSE)
									{ 
										*field->data.string_value=PP_EVALUATEbol(CHGSTRUCT->expression,CHGSTRUCT->SubFunc,CHGSTRUCT->SubFuncArgs);
									} else {
										*field->data.string_value=*CHGSTRUCT->to.string_value;
									}
								}
								break;
							case DECIMALV:
							case SDECIMALV:
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DOUBLEV:
							case SDOUBLEV:
								if(*CHGSTRUCT->from.float_value==*field->data.float_value)
								{
									if(fld->keypart==TRUE) changedkeypart=TRUE;
									write2file=TRUE;
									if(CHGSTRUCT->simple_to==FALSE)
									{ 
										*field->data.float_value=PP_EVALUATEdbl(CHGSTRUCT->expression,CHGSTRUCT->SubFunc,CHGSTRUCT->SubFuncArgs);
									} else {
										*field->data.float_value=*CHGSTRUCT->to.float_value;
									}
								}
								break;
							case SHORTV:
							case SSHORTV:
								if(*CHGSTRUCT->from.short_value==*field->data.short_value)
								{
									if(fld->keypart==TRUE) changedkeypart=TRUE;
									write2file=TRUE;
									if(CHGSTRUCT->simple_to==FALSE)
									{ 
										*field->data.short_value=PP_EVALUATEsht(CHGSTRUCT->expression,CHGSTRUCT->SubFunc,CHGSTRUCT->SubFuncArgs);
									} else {
										*field->data.short_value=*CHGSTRUCT->to.short_value;
									}
								}
								break;
							case LONGV:
							case SLONGV:
							case SCROLLEDLIST:
								if(*CHGSTRUCT->from.integer_value==*field->data.integer_value)
								{
									if(fld->keypart==TRUE) changedkeypart=TRUE;
									write2file=TRUE;
									if(CHGSTRUCT->simple_to==FALSE)
									{ 
										*field->data.integer_value=PP_EVALUATEint(CHGSTRUCT->expression,CHGSTRUCT->SubFunc,CHGSTRUCT->SubFuncArgs);
									} else {
										*field->data.integer_value=*CHGSTRUCT->to.integer_value;
									}
								}
								break;
							default:
								prterr("Error invalid field type [%d].",CHGSTRUCT->type);
								break;
						}
					} else {
						write2file=TRUE;
						switch(CHGSTRUCT->type)
						{	
							case VARIABLETEXT:
							case PLAINTEXT:
							case SCROLLEDTEXT:
							case DATES:
							case TIMES:
							case SOCSECNUM:
							case ZIPCODE:
							case PHONE:
							case CUSTOMTYPE:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
								if(CHGSTRUCT->simple_to==FALSE)
								{ 
									if(fld->keypart==TRUE) changedkeypart=TRUE;
									temp=PP_EVALUATEstr(CHGSTRUCT->expression,CHGSTRUCT->SubFunc,CHGSTRUCT->SubFuncArgs);
									if(field->data.string_value!=NULL) Rfree(field->data.string_value);
									field->dlen=0;
									if(temp!=NULL)
									{
										field->data.string_value=stralloc(temp);
										field->dlen=strlen(field->data.string_value)+1;
										Rfree(temp);
									}
								} else if(!isEMPTY(CHGSTRUCT->to.string_value))
								{
									if(fld->keypart==TRUE) changedkeypart=TRUE;
									QUICKALLOC(field->data.string_value,field->dlen,CHGSTRUCT->to_dlen);
									memcpy(field->data.string_value,CHGSTRUCT->to.string_value,CHGSTRUCT->to_dlen);
								} else if(field->data.string_value!=NULL)
								{
									if(fld->keypart==TRUE) changedkeypart=TRUE;
									memset(field->data.string_value,0,field->dlen);
								}	
								break;
							case BOOLNS:
							case CHARACTERS:
								if(CHGSTRUCT->simple_to==FALSE)
								{ 
									if(fld->keypart==TRUE) changedkeypart=TRUE;
									*field->data.string_value=PP_EVALUATEbol(CHGSTRUCT->expression,CHGSTRUCT->SubFunc,CHGSTRUCT->SubFuncArgs);
								} else {
									if(fld->keypart==TRUE) changedkeypart=TRUE;
									*field->data.string_value=*CHGSTRUCT->to.string_value;
								}
								break;
							case DECIMALV:
							case SDECIMALV:
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DOUBLEV:
							case SDOUBLEV:
								if(CHGSTRUCT->simple_to==FALSE)
								{ 
									if(fld->keypart==TRUE) changedkeypart=TRUE;
									*field->data.float_value=PP_EVALUATEdbl(CHGSTRUCT->expression,CHGSTRUCT->SubFunc,CHGSTRUCT->SubFuncArgs);
								} else {
									if(fld->keypart==TRUE) changedkeypart=TRUE;
									*field->data.float_value=*CHGSTRUCT->to.float_value;
								}
								break;
							case SHORTV:
							case SSHORTV:
								if(CHGSTRUCT->simple_to==FALSE)
								{ 
									if(fld->keypart==TRUE) changedkeypart=TRUE;
									*field->data.short_value=PP_EVALUATEsht(CHGSTRUCT->expression,CHGSTRUCT->SubFunc,CHGSTRUCT->SubFuncArgs);
								} else {
									if(fld->keypart==TRUE) changedkeypart=TRUE;
									*field->data.short_value=*CHGSTRUCT->to.short_value;
								}
								break;
							case LONGV:
							case SLONGV:
							case SCROLLEDLIST:
								if(CHGSTRUCT->simple_to==FALSE)
								{ 
									if(fld->keypart==TRUE) changedkeypart=TRUE;
									*field->data.integer_value=PP_EVALUATEint(CHGSTRUCT->expression,CHGSTRUCT->SubFunc,CHGSTRUCT->SubFuncArgs);
								} else {
									if(fld->keypart==TRUE) changedkeypart=TRUE;
									*field->data.integer_value=*CHGSTRUCT->to.integer_value;
								}
								break;
							default:
								prterr("Error invalid field type [%d].",CHGSTRUCT->type);
								break;
						}
					}
				}
			}		
			if(write2file==TRUE)
			{
				if(changedkeypart==TRUE)/*avoiding when possible*/
				{
					key=KEYNUM(c->fileno,c->keyno);
					if(key!=NULL)
					{
						deleterecord=FALSE;	
						for(y=0,part=key->part;y<key->numparts;++y,++part)	
						{
							if(deleterecord==TRUE) break;
							wfield=FLDNRD(c->sort->fileno,part->name);
							field=FLDNRD(c->fileno,part->name);
							if(wfield!=NULL && field!=NULL)
							{
								if(COMPARENRDfield(wfield,field))
								{
									deleterecord=TRUE;
								}
							} else {
								prterr("Error Oooppps.... call raddog...");
							}
						}
					} else {
						prterr("Error no key found!@#$@!$#@..");
					}
				}
				if(deleterecord==TRUE)
				{
					FINDFLDSETCHAR(c->fileno,"DELETEFLAG",TRUE);
					ADVWRTTRANSsec(c->fileno,0,NULL,c->prev,CHGSTRUCT->SubFunc,CHGSTRUCT->SubFuncArgs);
					WRTNRD(c->sort->fileno,0,NULL);
				} else {
					numflds=NUMFLDS(c->sort->fileno);
					wfields=FLDPOINTER(c->fileno);
					fields=FLDPOINTER(c->sort->fileno);
					for(x=0,field=fields,wfield=wfields;x<numflds;
						++x,++field,++wfield)
					{
						NRD2NRD(field,wfield);
					}
					ADVWRTTRANSsec(c->fileno,0,NULL,c->prev,CHGSTRUCT->SubFunc,CHGSTRUCT->SubFuncArgs);
				}
				return(TRUE);
			}
		}		
	}
	return(FALSE);
}
