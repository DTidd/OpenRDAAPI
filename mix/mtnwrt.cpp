/* mtnwrt.c - Maintain Master Write Functions */
#include<mix.hpp>

static void CpyRDArsrcStp(RDArsrc *to,RDArsrc *from)
{
	RDAScrolledList *l1;
	RDAvirtual *v1;
	int x;

	if(from!=NULL && to!=NULL)
	{
		if(from->virflds!=NULL)
		{
			for(x=0,v1=from->virflds;x<from->num;++x,++v1)
			{
				addSCRNvirtual(to,v1->name,v1->type,v1->len,
					v1->expression,v1->when);
			}
		}
		if(from->lists!=NULL)
		{
			for(x=0,l1=from->lists;x<from->numlists;++x,++l1)
			{
				addSCRNScrolledList(to,l1->module,l1->name,
					l1->vtype,l1->numincvirs,l1->incvir,
					l1->type,l1->select_formula,
					l1->special_function,l1->num,
					l1->initfld,l1->ffield,l1->occurrence,
					l1->ffinfo,l1->contype,l1->conmod,
					l1->confil,l1->confld,l1->con_occurrence,
					l1->list,l1->format_formula,
					l1->unformat_formula,l1->reformat_formula,l1->desc,l1->timing);
			}
		}
	} else {
		prterr("Error:  One RDArsrc or the other isn't allocated.");
	}
}
short GetFileNumber(char *module,char *filename,int numP_WRITES,PowerWrite *P_WRITES,MaintainMaster *MTN,
	RDAsubord *subord,short *closefile)
{
	int x;
	RDAsearch *supfil=NULL;
	short fileno=(-1);
	PowerWrite *Write=NULL;

	if(!isEMPTY(module) && !isEMPTY(filename))
	{
		if(MTN!=NULL)
		{
			if(!RDAstrcmp(MTN->module,module) && 
				!RDAstrcmp(MTN->mainfile,filename) &&
				(MTN->passkey->fileno!=(-1)))
			{
				*closefile=0;
				return(MTN->passkey->fileno);
			}
			if(MTN->s!=NULL)
			{
				for(x=0,supfil=MTN->s;x<MTN->num;++x,++supfil)
				{
					if(!RDAstrcmp(supfil->module,module) && 
						!RDAstrcmp(supfil->filename,filename) && 
						(supfil->fileno!=(-1))) break;
				}
				if(x<MTN->num)
				{
					fileno=supfil->fileno;
					*closefile=0;
					return(fileno);
				}
			}
			if(subord!=NULL)
			{
				if(!RDAstrcmp(subord->module,module) &&
					!RDAstrcmp(subord->filename,filename) &&
					(subord->fileno!=(-1))) 
				{
					*closefile=2;
					return(subord->fileno);
				}
			}
			if(P_WRITES!=NULL)
			{
				for(x=0,Write=P_WRITES;x<=numP_WRITES;++x,++Write)
				{
					if(!RDAstrcmp(Write->module,module) && 
						!RDAstrcmp(Write->filename,filename) &&
						(Write->fileno!=(-1))) 
					{
						*closefile=3;
						return(Write->fileno);
					}
				}
			}
			fileno=OPNNRDsec(module,filename,TRUE,TRUE,FALSE);
			*closefile=1;
			return(fileno);
		}
	} else {
		prterr("Error Module and Filename are empty/NULL.");
	}
	return(fileno);
}
char xMaintainMasterPowerWrite(MaintainMaster *MTN,RDAsubord *subord,
	int numP_WRITES,PowerWrite *P_WRITES,int line,char *file)
{
	PowerWrite *Write=NULL;
	PowerWriteField *WriteField=NULL;
	NRDfield *field=NULL;
	int a,x;
	char dowrite=FALSE,*trnfld=NULL,*fieldname=NULL;
	RDATData *prev=NULL,*orig=NULL;
	RDArsrc *tmprsrc=NULL;
	short cf=FALSE;
	char *string_value=NULL;
	char update=FALSE;
	
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG MaintainMasterPowerWrite at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(P_WRITES!=NULL)
	{
		tmprsrc=RDArsrcNEW(MTN->module,NULL);
/* Slow's everything down considerably */
/* Am considering making something that will copy the virtuals from the 
   original RDArsrc into the tmprsrc, avoiding searching the *.VIR for all
   appropriation computations */
/* RADDOG */
		GET_MAINTAIN_INCVARS(tmprsrc,MTN);
/*
		GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
*/
		CpyRDArsrcStp(MTN->mtnrsrc,tmprsrc);			
		if(subord!=NULL)
		{
			addDFincvir(tmprsrc,subord->module,subord->filename,NULL,subord->fileno);
		}
		for(a=0,Write=P_WRITES;a<numP_WRITES;
			++a,++Write)
		{
			Write->fileno=GetFileNumber(Write->module,Write->filename,a,P_WRITES,MTN,subord,&cf);
			if(cf==0) Write->closefile=0;
			else if(cf==1) Write->closefile=1;
			else if(cf==2) Write->closefile=2;
			else if(cf==3) Write->closefile=3;
			if(Write->closefile==1)
			{
				addDFincvir(tmprsrc,Write->module,Write->filename,NULL,Write->fileno);
			}
			if(Write->fileno!=(-1))
			{
				if(RDAstrcmp(MTN->module,Write->module) ||
					RDAstrcmp(MTN->mainfile,Write->filename) ||
					subord!=NULL)
				{
				if(Write->closefile==0 || Write->closefile==3) 
				{
					ZERNRD(Write->fileno);
				}
				if(Write->fields!=NULL)
				{
					for(x=0,WriteField=Write->fields;x<Write->numflds;++x,++WriteField)
					{
						fieldname=stripfieldname(WriteField->to_name);
						if(!isEMPTY(fieldname))
						{
							field=FLDNRD(Write->fileno,fieldname);
						}
						if(!WriteField->copy_type)
						{
							if(!RDAstrcmp(WriteField->from_name,"[NEXT TRANSACTION NO]"))
							{
								trnfld=stralloc(fieldname);
								if(fieldname!=NULL) Rfree(fieldname);
							} else {
								if(fieldname!=NULL) Rfree(fieldname);
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
										case PHONE:
										case CUSTOMTYPE:
										case ZIPCODE:
										case EXPENDITURE:
										case REVENUE:
										case BALANCESHEET:
										case BEGINNINGBALANCE:
										case CHARACTERS:
										case OPTIONALFIELDS:
											string_value=EVALUATEstr(WriteField->from_name,SCRNvirtualSubData,tmprsrc);
											if(field->data.string_value!=NULL) Rfree(field->data.string_value);
											field->data.string_value=stralloc(string_value);
											if(string_value!=NULL) Rfree(string_value);
											if(field->data.string_value!=NULL)
											{
												field->dlen=RDAstrlen(field->data.string_value)+1;
												if(RDAstrlen(field->data.string_value)>field->len && field->len>0) field->data.string_value[field->len]=0;
											} else field->dlen=0;
											break;
										case BOOLNS:
											*field->data.string_value=EVALUATEbol(WriteField->from_name,SCRNvirtualSubData,tmprsrc);
											break;
										case SHORTV:
										case SSHORTV:
											*field->data.short_value=EVALUATEsht(WriteField->from_name,SCRNvirtualSubData,tmprsrc);
											break;
										case SLONGV:
										case SCROLLEDLIST:
										case LONGV:
											*field->data.integer_value=EVALUATEint(WriteField->from_name,SCRNvirtualSubData,tmprsrc);
											break;
										case DOUBLEV:
										case SDOUBLEV:
										case DOLLARS:
										case DOLLARS_NOCENTS:
										case DECIMALV:
										case SDECIMALV:
											*field->data.float_value=EVALUATEdbl(WriteField->from_name,SCRNvirtualSubData,tmprsrc);
											if(field->type==DOLLARS || field->type==DOLLARS_NOCENTS ||field->type==DOUBLEV || field->type==SDOUBLEV)
											{
												*field->data.float_value=round(*field->data.float_value);
											}
											break;
										default:
											prterr("Error unrecognized field type [%d] for field [%s].",field->type,(field->name!=NULL ? field->name:""));
									}
								}
							}
						} else {
							if(fieldname!=NULL) Rfree(fieldname);
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
									case PHONE:
									case CUSTOMTYPE:
									case ZIPCODE:
									case EXPENDITURE:
									case REVENUE:
									case BALANCESHEET:
									case BEGINNINGBALANCE:
									case CHARACTERS:
									case OPTIONALFIELDS:
										string_value=EVALUATEstr(WriteField->from_name,SCRNvirtualSubData,tmprsrc);
										if(field->data.string_value!=NULL) Rfree(field->data.string_value);
										field->data.string_value=stralloc(string_value);
										if(string_value!=NULL) Rfree(string_value);
										if(field->data.string_value!=NULL)
										{
											field->dlen=RDAstrlen(field->data.string_value)+1;
											if(RDAstrlen(field->data.string_value)>field->len && field->len>0) field->data.string_value[field->len]=0;
										} else field->dlen=0;
										break;
									case BOOLNS:
										*field->data.string_value=EVALUATEbol(WriteField->from_name,SCRNvirtualSubData,tmprsrc);
										break;
									case SHORTV:
									case SSHORTV:
										*field->data.short_value=EVALUATEsht(WriteField->from_name,SCRNvirtualSubData,tmprsrc);
										break;
									case SLONGV:
									case SCROLLEDLIST:
									case LONGV:
										*field->data.integer_value=EVALUATEint(WriteField->from_name,SCRNvirtualSubData,tmprsrc);
										break;
									case DOUBLEV:
									case SDOUBLEV:
									case DOLLARS:
									case DOLLARS_NOCENTS:
									case DECIMALV:
									case SDECIMALV:
										*field->data.float_value=EVALUATEdbl(WriteField->from_name,SCRNvirtualSubData,tmprsrc);
										if(field->type==DOLLARS || field->type==DOLLARS_NOCENTS ||field->type==DOUBLEV || field->type==SDOUBLEV)
										{
											*field->data.float_value=round(*field->data.float_value);
										}
										break;
									default:
										prterr("Error unrecognized field type [%d] for field [%s].",field->type,(field->name!=NULL ? field->name:""));
								}
							}
						}
					}
					if(trnfld!=NULL)
					{
						if(isEMPTY(Write->expression))
						{
							dowrite=TRUE;
						} else dowrite=EVALUATEbol(Write->expression,SCRNvirtualSubData,tmprsrc);
						if(dowrite==TRUE)
						{
							ADVWRTTRANSsec(Write->fileno,1,trnfld,NULL,SCRNvirtualSubData,tmprsrc);	
							if(Write->closefile==0 && subord!=NULL) 
							{
								update=TRUE;
							}
						}
						Rfree(trnfld);
						trnfld=NULL;
					} else if(ADVEQLNRDsec(Write->fileno,1,SCRNvirtualSubData,tmprsrc))
					{
						if(isEMPTY(Write->expression))
						{
							dowrite=TRUE;
						} else dowrite=EVALUATEbol(Write->expression,SCRNvirtualSubData,tmprsrc);
						if(dowrite==TRUE)
						{
							ADVWRTTRANSsec(Write->fileno,0,NULL,NULL,SCRNvirtualSubData,tmprsrc);	
							if(Write->closefile==0 && subord!=NULL) 
							{
								update=TRUE;
							}
						}
					} else {
/* SJSCHANGE - moved from up-above. */
						if(isEMPTY(Write->expression))
						{
							dowrite=TRUE;
						} else dowrite=EVALUATEbol(Write->expression,SCRNvirtualSubData,tmprsrc);
						if(dowrite==TRUE)
						{
						prev=RDATDataNEW(Write->fileno);
						
						for(x=0,WriteField=Write->fields;x<Write->numflds;++x,++WriteField)
						{
							fieldname=stripfieldname(WriteField->to_name);
							field=FLDNRD(Write->fileno,fieldname);
							if(!WriteField->copy_type)
							{
								if(!RDAstrcmp(WriteField->from_name,"[NEXT TRANSACTION NO]"))
								{
									trnfld=stralloc(fieldname);
									if(fieldname!=NULL) Rfree(fieldname);
								} else {
									if(fieldname!=NULL) Rfree(fieldname);
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
											case PHONE:
											case CUSTOMTYPE:
											case ZIPCODE:
											case EXPENDITURE:
											case REVENUE:
											case BALANCESHEET:
											case BEGINNINGBALANCE:
											case CHARACTERS:
											case OPTIONALFIELDS:
												string_value=EVALUATEstr(WriteField->from_name,SCRNvirtualSubData,tmprsrc);
												if(field->data.string_value!=NULL) Rfree(field->data.string_value);
												field->data.string_value=stralloc(string_value);
												if(string_value!=NULL) Rfree(string_value);
												if(field->data.string_value!=NULL)
												{
													field->dlen=RDAstrlen(field->data.string_value)+1;
													if(RDAstrlen(field->data.string_value)>field->len && field->len>0) field->data.string_value[field->len]=0;
												} else field->dlen=0;
												break;
											case BOOLNS:
												*field->data.string_value=EVALUATEbol(WriteField->from_name,SCRNvirtualSubData,tmprsrc);
												break;
											case SHORTV:
											case SSHORTV:
												*field->data.short_value=EVALUATEsht(WriteField->from_name,SCRNvirtualSubData,tmprsrc);
												break;
											case SLONGV:
											case SCROLLEDLIST:
											case LONGV:
												*field->data.integer_value=EVALUATEint(WriteField->from_name,SCRNvirtualSubData,tmprsrc);
												break;
											case DOUBLEV:
											case SDOUBLEV:
											case DOLLARS:
											case DOLLARS_NOCENTS:
											case DECIMALV:
											case SDECIMALV:
												*field->data.float_value=EVALUATEdbl(WriteField->from_name,SCRNvirtualSubData,tmprsrc);
												if(field->type==DOLLARS || field->type==DOLLARS_NOCENTS ||field->type==DOUBLEV || field->type==SDOUBLEV)
												{
													*field->data.float_value=round(*field->data.float_value);
												}
												break;
											default:
												prterr("Error unrecognized field type [%d] for field [%s].",field->type,(field->name!=NULL ? field->name:""));
										}
									} else {
									}
								}
							} else {		
								if(fieldname!=NULL) Rfree(fieldname);
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
										case PHONE:
										case CUSTOMTYPE:
										case ZIPCODE:
										case EXPENDITURE:
										case REVENUE:
										case BALANCESHEET:
										case BEGINNINGBALANCE:
										case CHARACTERS:
										case OPTIONALFIELDS:
											string_value=EVALUATEstr(WriteField->from_name,SCRNvirtualSubData,tmprsrc);
											if(field->data.string_value!=NULL) Rfree(field->data.string_value);
											field->data.string_value=stralloc(string_value);
											if(string_value!=NULL) Rfree(string_value);
											if(field->data.string_value!=NULL)
											{
												field->dlen=RDAstrlen(field->data.string_value)+1;
												if(RDAstrlen(field->data.string_value)>field->len && field->len>0) field->data.string_value[field->len]=0;
											} else field->dlen=0;
											break;
										case BOOLNS:
											*field->data.string_value=EVALUATEbol(WriteField->from_name,SCRNvirtualSubData,tmprsrc);
											break;
										case SHORTV:
										case SSHORTV:
											*field->data.short_value=EVALUATEsht(WriteField->from_name,SCRNvirtualSubData,tmprsrc);
											break;
										case SLONGV:
										case SCROLLEDLIST:
										case LONGV:
											*field->data.integer_value=EVALUATEint(WriteField->from_name,SCRNvirtualSubData,tmprsrc);
											break;
										case DOUBLEV:
										case SDOUBLEV:
										case DOLLARS:
										case DOLLARS_NOCENTS:
										case DECIMALV:
										case SDECIMALV:
											*field->data.float_value=EVALUATEdbl(WriteField->from_name,SCRNvirtualSubData,tmprsrc);
											if(field->type==DOLLARS || field->type==DOLLARS_NOCENTS ||field->type==DOUBLEV || field->type==SDOUBLEV)
											{
												*field->data.float_value=round(*field->data.float_value);
											}
											break;
										default:
											prterr("Error unrecognized field type [%d] for field [%s].",field->type,(field->name!=NULL ? field->name:""));
									}
								} else {
								}
							}
						}
						if(!isEMPTY(trnfld))
						{
							ADVWRTTRANSsec(Write->fileno,1,trnfld,prev,SCRNvirtualSubData,tmprsrc);	
						} else {
							ADVWRTTRANSsec(Write->fileno,0,NULL,prev,SCRNvirtualSubData,tmprsrc);	
						}
						if(Write->closefile==0 && subord!=NULL) 
						{
							update=TRUE;
						}
						if(prev!=NULL) FreeRDATData(prev);
						}
					}	
				} else {
					prterr("Error PowerWriteField's are missing...");
				}
				} else { /* when it's the main file */
					for(x=0,WriteField=Write->fields;x<Write->numflds;++x,++WriteField)
					{
						if(!WriteField->copy_type)
						{
							if(!RDAstrcmp(WriteField->from_name,"[NEXT TRANSACTION NO]"))
							{
								fieldname=stripfieldname(WriteField->to_name);
								trnfld=stralloc(fieldname);
								if(fieldname!=NULL) Rfree(fieldname);
							}
						}
					}
					if(isEMPTY(Write->expression))
					{
						dowrite=TRUE;
					} else dowrite=EVALUATEbol(Write->expression,SCRNvirtualSubData,tmprsrc);
					if(dowrite==TRUE)
					{
					if(isEMPTY(trnfld))
					{
						if(prev!=NULL) FreeRDATData(prev);
						orig=RDATDataNEW(Write->fileno);
						if(!ADVEQLNRDsec(Write->fileno,1,SCRNvirtualSubData,tmprsrc))
						{
							prev=RDATDataNEW(Write->fileno);
							RDATData2File(Write->fileno,orig);
						}
						ADVWRTTRANSsec(Write->fileno,0,NULL,prev,SCRNvirtualSubData,tmprsrc);	
					} else {
						ADVWRTTRANSsec(Write->fileno,1,trnfld,prev,SCRNvirtualSubData,tmprsrc);	
					}
					}
					if(prev!=NULL) FreeRDATData(prev);
					if(orig!=NULL) FreeRDATData(orig);
				}
			} else {
				prterr("Error PowerWrite File not found [%s][%s].",(Write->module!=NULL ? Write->module:""),(Write->filename!=NULL ? Write->filename:""));
			}	
		}
		for(x=0,Write=P_WRITES;x<numP_WRITES;++x,
			++Write)
		{
			if(Write->fileno!=(-1) && 
				(Write->closefile==1)) 
			{
				CLSNRD(Write->fileno);
			}
		}
		if(tmprsrc!=NULL) free_rsrc(tmprsrc);
	}
	return(update);
}
