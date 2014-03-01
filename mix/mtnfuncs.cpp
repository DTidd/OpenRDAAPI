/* mtnfuncs.c - Maintain Master Functions */
#include<mix.hpp>

void xGET_BROWSE_SUPPORTING(RDArsrc *brsrc,RDArsrc *srsrc,RDArsrc *drsrc,
	MaintainMaster *MTNMASTER,void (*SubFunc)(...),void *args,int line,char *file)
{
	RDAsearch *f;
	int x,y;
	NRDkey *key;
	NRDpart *part;
	RDAconfld *cf=NULL;
	short getfile=FALSE,w=0;
	char doit=FALSE;

	if(MTNMASTER!=NULL)
	{
		addDFincvir(drsrc,MTNMASTER->module,MTNMASTER->mainfile,NULL,MTNMASTER->passkey->fileno);
		addDFincvir(srsrc,MTNMASTER->module,MTNMASTER->mainfile,NULL,MTNMASTER->passkey->fileno);
		if(MTNMASTER->s!=NULL)
		{
			for(x=0,f=MTNMASTER->s;x<MTNMASTER->num;++x,++f)
			{
				if(f->fileno!=(-1) && f->skip_widget==FALSE)
				{
					ZERNRD(f->fileno);
					addDFincvir(srsrc,f->module,f->filename,NULL,f->fileno);
					addDFincvir(drsrc,f->module,f->filename,NULL,f->fileno);
					addDFincvir(brsrc,f->module,f->filename,NULL,f->fileno);

					if(f->confld!=NULL)
					{
						for(w=0,cf=f->confld;w<f->num;++w,++cf)
						{
							if(isEMPTY(cf->conname)) break;
						}
						if(w<f->num) doit=FALSE;
							else doit=TRUE;
					} else doit=FALSE;
					if(doit==TRUE)
					{
						key=KEYNUM(f->fileno,f->keyno);
						if(key!=NULL)
						{
							getfile=FALSE;
							for(y=0,part=key->part;(y<key->numparts && !getfile);++y,++part)
							{
								getfile=COPYFIELD(MTNMASTER->passkey->fileno,f->fileno,part->name);
							}
							if(y>=key->numparts)
							{
								if(ADVEQLNRDsec(f->fileno,f->keyno,SubFunc,args))
								{
									ZERNRD(f->fileno);
								} else {
									file2rsrc(f->fileno,brsrc,FALSE);
									filerecord2rsrc(f->fileno,brsrc);
								}
							}
						}
					}
				}
			}
		}
	}
}
void xGET_MAINTAIN_INCVARS(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER,int line,char *file)
{
	RDAsearch *s;
	int x;

	if(MTNMASTER!=NULL)
	{
        	addDFincvir(mtnrsrc,MTNMASTER->module,MTNMASTER->mainfile,NULL,MTNMASTER->passkey->fileno);
		if(MTNMASTER->s!=NULL)
		{
			for(x=0,s=MTNMASTER->s;x<MTNMASTER->num;++x,++s)
			{
				addDFincvir(mtnrsrc,s->module,s->filename,NULL,s->fileno);
   			}
		}
	}
}
void xSET_MAINTAIN_FILE2RSRCS(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER,int line,char *file)
{
	RDAsearch *s;
	int x;

	if(MTNMASTER!=NULL)
	{
		if(MTNMASTER->s!=NULL)
		{
			for(x=0,s=MTNMASTER->s;x<MTNMASTER->num;++x,++s)
			{
				if(s->ctype!=2 && s->skip_widget==FALSE) file2rsrc(s->fileno,mtnrsrc,FALSE);
			}
		}
	}
}
static char CompareKeyFields(short sub_file,MaintainMaster *MTNMASTER,RDAsubord *subord)
{
	RDAconfld *tempconfld;
	NRDfield *subfld,*mainfld;
	int x;
	char *filename=NULL,*fieldname=NULL,break_out=FALSE;
	char *mainfilename=NULL;

	mainfilename=stralloc(FILENAME(MTNMASTER->passkey->fileno));
	for(x=0,tempconfld=subord->confld+x;x<subord->num;x++,tempconfld++)
	{
		subfld=FLDNRDsec(sub_file,tempconfld->fldname,TRUE,FALSE,FALSE);
		if(subfld!=NULL)
		{
			filename=stripmodulename(tempconfld->conname);
			fieldname=stripfilename(tempconfld->conname);
			if(fieldname!=NULL && RDAstrlen(fieldname)>0)
 			{
				mainfld=NULL;
				if(!RDAstrcmp(filename,mainfilename))
				{
					mainfld=FLDNRDsec(MTNMASTER->passkey->fileno,fieldname,TRUE,FALSE,FALSE);
				} else {
/* linking to other file fields values goes here, not implemented yet */
				}
				if(mainfld!=NULL)
				{
					switch(subfld->type)
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
							if(!RDAstrcmp(subfld->data.string_value,mainfld->data.string_value))
							{
								break_out=FALSE;
							} else {
								break_out=TRUE;
							}
							break;
						case SHORTV:
						case SSHORTV:
							if(*subfld->data.short_value==*mainfld->data.short_value)
							{
								break_out=FALSE;
							} else {
								break_out=TRUE;
							}
							break;
						case SCROLLEDLIST:
						case LONGV:
						case SLONGV:
							if(*subfld->data.integer_value==*mainfld->data.integer_value)
							{
								break_out=FALSE;
							} else {
								break_out=TRUE;
							}
							break;
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DECIMALV:
						case DOUBLEV:
						case SDOUBLEV:
						case SDECIMALV:
							if(*subfld->data.float_value==*mainfld->data.float_value)
							{
								break_out=FALSE;
							} else {
								break_out=TRUE;
							}
							break;
						case BOOLNS:
							if(*subfld->data.string_value==*mainfld->data.string_value)
							{
								break_out=FALSE;
							} else {
								break_out=TRUE;
							}
							break;
						case CHARACTERS:
							if(*subfld->data.string_value==*mainfld->data.string_value)
							{
								break_out=FALSE;
							} else {
								break_out=TRUE;
							}
							break;
						default:
							prterr("Error field type [%d] is invalid for field [%s].",mainfld->type,mainfld->name);
							break_out=TRUE;
							break;
					}
					if(break_out==TRUE)
					{
						if(mainfilename!=NULL) Rfree(mainfilename);
						return(FALSE);
					}
				}
			} else {
				if(mainfilename!=NULL) Rfree(mainfilename);
				return(FALSE);
			}
		} else {
			if(mainfilename!=NULL) Rfree(mainfilename);
			return(FALSE);
		}
	}
	if(mainfilename!=NULL) Rfree(mainfilename);
	return(TRUE);
}
char xDELETE_SUBORDINATES(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER,void (*SubFunc)(...),void *args,int line,char *file)
{
	RDAsubord *subord;
	RDAconfld *tempconfld;
	char deleteflag=FALSE,subdeleteflag=FALSE,*errdesc=NULL;
	char *filename=NULL,*fieldname=NULL;
	char *mainmodule,*mainfilename;
	short sub_file=(-1);
	short ef=0;
	short x=0,y=0,keyno=0;
	RDATData *prev_sub=NULL;
	RDArsrc *tmprsrc=NULL;
	NRDfield *mainfld,*subfld;
	char update=FALSE;

	if(MTNMASTER->subord!=NULL)
	{
		FINDFLDGETCHAR(MTNMASTER->passkey->fileno,"DELETEFLAG",&deleteflag);
		if(deleteflag)
		{
			mainmodule=stralloc(MODULENAME(MTNMASTER->passkey->fileno));
			mainfilename=stralloc(FILENAME(MTNMASTER->passkey->fileno));
			for(x=0,subord=MTNMASTER->subord;x<MTNMASTER->subnum;++x,++subord)
			{
				tmprsrc=RDArsrcNEW(MTNMASTER->module,NULL);
				copyDFincvir(mtnrsrc,tmprsrc);
				copyRDAvirtual(mtnrsrc,tmprsrc);
				sub_file=OPNNRDsec(subord->module,subord->filename,TRUE,TRUE,FALSE);
/* SJSCHANGE */
				subord->fileno=sub_file;
				addDFincvir(tmprsrc,subord->module,subord->filename,NULL,sub_file);
/*
				ADVGET_SCREEN_VIRTUALS(tmprsrc,0,TRUE,FALSE);
				GET_ALL_SCREEN_VIRTUALS(tmprsrc,0);
*/
				if(sub_file!=(-1))
				{
					ZERNRD(sub_file);
					keyno=KEYNUMBER(sub_file,subord->subkeyname);
					if(keyno==(-1))
					{
						CLSNRD(sub_file);
						if(tmprsrc!=NULL) free_rsrc(tmprsrc);
						errdesc=Rmalloc(550+RDAstrlen(subord->subkeyname)+RDAstrlen(subord->filename)+RDAstrlen(subord->module));
						sprintf(errdesc,"The keyname [%s] is missing for the [%s][%s] file.  Therefore, the associated records cannot be deleted for this file.  You will need to add the key if it is supposed to exist and bring up the main record in deleted form and resave as deleted, or select the correct key for the subordinate file to connect correctly and then bring up this main record in the deleted form and resave as deleted, or manually set the records to delete.",subord->subkeyname,subord->module,subord->module);
						ERRORDIALOG("MISSING KEY NAME!",errdesc,NULL,FALSE);
						prterr(errdesc);                                                        
						if(errdesc!=NULL) Rfree(errdesc);
					} else {
						for(y=0,tempconfld=subord->confld;y<(subord->num);++y,++tempconfld)
						{
							subfld=FLDNRDsec(sub_file,tempconfld->fldname,TRUE,FALSE,FALSE);
							if(subfld!=NULL)
							{
								filename=stripmodulename(tempconfld->conname);
								fieldname=stripfilename(tempconfld->conname);
								if(fieldname!=NULL && RDAstrlen(fieldname)>0)
 								{
									mainfld=NULL;
									if(!RDAstrcmp(filename,mainfilename))
									{
										mainfld=FLDNRDsec(MTNMASTER->passkey->fileno,fieldname,TRUE,FALSE,FALSE);
									} else {
/* linking to other fields values goes here, not implemented yet */
									}
									if(mainfld!=NULL)
									{
										switch(subfld->type)
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
											if(mainfld->type==SCROLLEDTEXT || mainfld->type==VARIABLETEXT ||
	   											mainfld->type==DATES || mainfld->type==TIMES ||
	   											mainfld->type==SOCSECNUM || mainfld->type==PLAINTEXT ||
	   											mainfld->type==PHONE || mainfld->type==CUSTOMTYPE ||
												mainfld->type==ZIPCODE || mainfld->type==EXPENDITURE ||
	   											mainfld->type==REVENUE || mainfld->type==BALANCESHEET ||
	   											mainfld->type==BEGINNINGBALANCE || mainfld->type==OPTIONALFIELDS)
											{
												if(!isEMPTY(mainfld->data.string_value))
												{
													QUICKALLOC(subfld->data.string_value,subfld->dlen,mainfld->dlen);
													memcpy(subfld->data.string_value,mainfld->data.string_value,mainfld->dlen);
													if(subfld->dlen>subfld->len && subfld->len>0)
														subfld->data.string_value[subfld->len]=0;
												} else if(subfld->dlen>0)
												{
													memset(subfld->data.string_value,0,subfld->dlen);
												} else {
													subfld->data.string_value=NULL;
													subfld->dlen=0;
												}
											} else {
												prterr("Error field types don't match between the Main File Field [%s][%s][%s] type [%d] and the Subordinate File Field [%s][%s][%s] Type [%d].",
													mainmodule,mainfilename,
													mainfld->name,mainfld->type,
													subord->module,subord->filename,
													subfld->name,subfld->type);
											}
											break;
										case SHORTV:
										case SSHORTV:
											if(mainfld->type==SHORTV || mainfld->type==SSHORTV)
											{
												*subfld->data.short_value=*mainfld->data.short_value;
											} else {
												prterr("Error field types don't match between the Main File Field [%s][%s][%s] type [%d] and the Subordinate File Field [%s][%s][%s] Type [%d].",
													mainmodule,mainfilename,
													mainfld->name,mainfld->type,
													subord->module,subord->filename,
													subfld->name,subfld->type);
											}
											break;
										case SCROLLEDLIST:
										case LONGV:
										case SLONGV:
											if(mainfld->type==LONGV || mainfld->type==SLONGV ||
												mainfld->type==SCROLLEDLIST)
											{
												*subfld->data.integer_value=*mainfld->data.integer_value;
											} else {
												prterr("Error field types don't match between the Main File Field [%s][%s][%s] type [%d] and the Subordinate File Field [%s][%s][%s] Type [%d].",
													mainmodule,mainfilename,
													mainfld->name,mainfld->type,
													subord->module,subord->filename,
													subfld->name,subfld->type);
											}
											break;
										case DOLLARS:
										case DOLLARS_NOCENTS:
										case DECIMALV:
										case DOUBLEV:
										case SDOUBLEV:
										case SDECIMALV:
											if(mainfld->type==DOLLARS || mainfld->type==DOLLARS_NOCENTS ||
												mainfld->type==DECIMALV || mainfld->type==SDECIMALV ||
												mainfld->type==DOUBLEV || mainfld->type==SDOUBLEV)
											{
												*subfld->data.float_value=*mainfld->data.float_value;
											} else {
												prterr("Error field types don't match between the Main File Field [%s][%s][%s] type [%d] and the Subordinate File Field [%s][%s][%s] Type [%d].",
													mainmodule,mainfilename,
													mainfld->name,mainfld->type,
													subord->module,subord->filename,
													subfld->name,subfld->type);
											}
											break;
										case BOOLNS:
											if(mainfld->type==BOOLNS)
											{
												*subfld->data.string_value=*mainfld->data.string_value;
											} else {
												prterr("Error field types don't match between the Main File Field [%s][%s][%s] type [%d] and the Subordinate File Field [%s][%s][%s] Type [%d].",
													mainmodule,mainfilename,
													mainfld->name,mainfld->type,
													subord->module,subord->filename,
													subfld->name,subfld->type);
											}
											break;
										case CHARACTERS:
											if(mainfld->type==CHARACTERS)
											{
												*subfld->data.string_value=*mainfld->data.string_value;
											} else {
												prterr("Error field types don't match between the Main File Field [%s][%s][%s] type [%d] and the Subordinate File Field [%s][%s][%s] Type [%d].",
													mainmodule,mainfilename,
													mainfld->name,mainfld->type,
													subord->module,subord->filename,
													subfld->name,subfld->type);
											}
											break;
										default:
											prterr("Error field type [%d] is invalid for field [%s].",mainfld->type,mainfld->name);
											break;
										}
									} else {
										prterr("Error Field [%s] in file [%d] [%s] [%s] not found.",fieldname,MTNMASTER->passkey->fileno,mainmodule,mainfilename);
									}
								} else {
/* Virtual Fields comparisons to go here */
								}
								if(filename!=NULL) Rfree(filename);
								if(fieldname!=NULL) Rfree(fieldname);

							}
						}
						ef=ADVGTENRDsec(sub_file,keyno,SCRNvirtualSubData,tmprsrc);
						while(!ef)
						{
							if(!CompareKeyFields(sub_file,MTNMASTER,subord))
							{
								break;
							}
							if(!isEMPTY(subord->expression))
							{
								if(EVALUATEbol(subord->expression,SCRNvirtualSubData,tmprsrc))
								{
									if(subord->UsePowerWrite==TRUE)
									{
										update=DeleteSubordinatePowerWrite(MTNMASTER,subord,subord->numWrites,subord->Writes);
											
									} else {
										LOCNRD(sub_file);
										prev_sub=RDATDataNEW(sub_file);
										FINDFLDSETCHAR(sub_file,"DELETEFLAG",deleteflag);
										ADVWRTTRANSsec(sub_file,0,NULL,prev_sub,SCRNvirtualSubData,tmprsrc);
										UNLNRD(sub_file);
										if(prev_sub!=NULL) FreeRDATData(prev_sub);
									}
								}
							} else {
								FINDFLDGETCHAR(sub_file,"DELETEFLAG",&subdeleteflag);
								if(!subdeleteflag)
								{
									if(subord->UsePowerWrite==TRUE)
									{
										update=DeleteSubordinatePowerWrite(MTNMASTER,subord,subord->numWrites,subord->Writes);
									} else {
										LOCNRD(sub_file);
										prev_sub=RDATDataNEW(sub_file);
										FINDFLDSETCHAR(sub_file,"DELETEFLAG",deleteflag);
										ADVWRTTRANSsec(sub_file,0,NULL,prev_sub,SCRNvirtualSubData,tmprsrc);
										UNLNRD(sub_file);
										if(prev_sub!=NULL) FreeRDATData(prev_sub);
									}
								}
							}
							ef=ADVNXTNRDsec(sub_file,keyno,SCRNvirtualSubData,tmprsrc);
						}
						CLSNRD(sub_file);
					}
				}
				if(tmprsrc!=NULL) free_rsrc(tmprsrc);
			}
			if(mainmodule!=NULL) Rfree(mainmodule);
			if(mainfilename!=NULL) Rfree(mainfilename);
		}
	}
	return(update);
}
char xSetSupportingFiles(RDArsrc *mrsrc,MaintainMaster *MTNMASTER,void (*SubFunc)(...),void *args,short update_rsrcs,short display_diags,int line,char *file)
{
	int x,y;
	short mainfileno=(-1),fileno=(-1);
	RDAsearch *s;
	NRDkey *key;
	NRDpart *part;
	RDAconfld *confld;
	short keyno=(-1),getfile=FALSE;
	char *confieldname=NULL,*temp=NULL,*temp1=NULL,skipit=FALSE;
/*
	char deleteflag=FALSE;
*/

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG xSetSupportingFiles Update Resources [%s] Display Diagnostics [%s] at line [%d] program [%s].",
			(update_rsrcs==TRUE?"TRUE":"FALSE"),
			(display_diags==TRUE?"TRUE":"FALSE"),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(MTNMASTER!=NULL)
	{
	if(MTNMASTER->s!=NULL)
	{
	for(x=0,s=MTNMASTER->s;x<MTNMASTER->num;++x,++s)
	{
		skipit=FALSE;
		if(!RDAstrcmp(s->cmodule,MTNMASTER->module) && !RDAstrcmp(s->cfilename,MTNMASTER->mainfile) && !RDAstrcmp(s->module,MTNMASTER->module) && !RDAstrcmp(s->filename,MTNMASTER->mainfile)) skipit=TRUE;
		if(s->ctype!=2 && skipit==FALSE)
		{
		if(!RDAstrcmp(s->cfilename,"SORTFILE") && MTNMASTER->file_type)
		{
			mainfileno=MTNMASTER->passkey->fileno;
		} else {
			mainfileno=SCRNFILENUMBER(mrsrc,s->cmodule,s->cfilename,1);
		}
		fileno=SCRNFILENUMBER(mrsrc,s->module,s->filename,s->occurrence);
		if(fileno!=(-1) && fileno!=MTNMASTER->passkey->fileno)
		{
			keyno=KEYNUMBER(fileno,s->subkeyname);
			if(keyno!=(-1))
			{
				key=KEYNUM(fileno,keyno);
				if(key!=NULL)
				{
					getfile=FALSE;
					if(s->confld!=NULL)
					{
						if(key->numparts==s->num)
						{
							for(y=0,confld=s->confld,part=key->part;(y<key->numparts && !getfile);
								++y,++confld,++part)
							{
								if(!RDAstrcmp(s->cfilename,"SORTFILE") && MTNMASTER->file_type)
								{
									confieldname=stralloc(confld->conname);
								} else {
									confieldname=stripfilename(confld->conname);
								}
								getfile=FIELDCOPY(mainfileno,confieldname,fileno,part->name);
								if(confieldname!=NULL) Rfree(confieldname);
							}
							if(!getfile)
							{
								if(y>=key->numparts)
								{
									if(ADVEQLNRDsec(fileno,1,SubFunc,args))
									{
										KEYNRD(fileno,keyno);
/* SJSTEST - we tried this to fix loading of maintain master supportings from BROWSE buttons in the supporting definitions but timing problem with multiple keys, when only one key is set at a time the others get ZERO'ed and you can never load the keys from the browse.
										ZERNRD(fileno);
*/
										if(s->skip_widget==FALSE)
										{	
										if(update_rsrcs==TRUE) updatefilerecord2rsrc(fileno,mrsrc);
										else filerecord2rsrc(fileno,mrsrc); 
										}
										if(s->ctype<1 && display_diags)
										{
										for(y=0,part=key->part;(y<key->numparts);++y,++part)
										{
											if(y+1<key->numparts)
											{
												temp1=Rmalloc(3+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
												sprintf(temp1,"%s, ",(isEMPTY(part->name)?"NULL":part->name));
											} else {
												temp1=Rmalloc(5+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
												sprintf(temp1,"and %s",(isEMPTY(part->name)?"NULL":part->name));
											}
											temp=stralloccat(temp,temp1);
											if(temp1!=NULL) Rfree(temp1);	
										}
										temp1=Rmalloc(207+RDAstrlen(temp)+RDAstrlen(s->module)+RDAstrlen(s->filename));
										sprintf(temp1,"There are invalid values for the key part(s) %s.  A corresponding record does not exist in the Supporting file of [%s][%s].  Valid values for these fields must be entered before saving this record.",(temp==NULL?"":temp),(s->module==NULL?"":s->module),(s->filename==NULL?"":s->filename));
										if(temp!=NULL) Rfree(temp);

/*
										WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID KEY VALUES!",(temp1==NULL?"":temp1),NULL,NULL,TRUE,NULL);
*/
										ERRORDIALOG("INVALID FIELD VALUE!",(temp1!=NULL?temp1:""),NULL,FALSE);
										if(temp1!=NULL) Rfree(temp1);
										return(FALSE);
										}
									} else {
										if(s->skip_widget==FALSE)
										{	
										if(update_rsrcs==TRUE) updatefilerecord2rsrc(fileno,mrsrc);
										else filerecord2rsrc(fileno,mrsrc); 
										}
										if(s->ctype<1 && display_diags)
										{
										if(!isEMPTY(s->expression))
										{
										if(!EVALUATEbol(s->expression,SubFunc,args))
										{
											for(y=0,part=key->part;(y<key->numparts);++y,++part)
											{
												if(y+1<key->numparts)
												{
													temp1=Rmalloc(3+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
													sprintf(temp1,"%s, ",(isEMPTY(part->name)?"NULL":part->name));
												} else {
													temp1=Rmalloc(5+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
													sprintf(temp1,"and %s",(isEMPTY(part->name)?"NULL":part->name));
												}
												temp=stralloccat(temp,temp1);
												if(temp1!=NULL) Rfree(temp1);	
											}
											temp1=Rmalloc(242+RDAstrlen(temp)+RDAstrlen(s->module)+RDAstrlen(s->filename));
											sprintf(temp1,
												"The values for the key part(s) %s have a corresponding record in the Supporting file of [%s][%s] but the Expression Evaluates to FALSE.  Therefore, the values are invalid.  Valid key values must be entered before saving this record.",
												(temp==NULL?"":temp),
												(s->module==NULL?"":s->module),
												(s->filename==NULL?"":s->filename));
											if(temp!=NULL) Rfree(temp);
/*
											WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID KEY VALUES!",(temp1==NULL?"":temp1),NULL,NULL,TRUE,NULL);
*/
											ERRORDIALOG("INVALID FIELD VALUE!",(temp1!=NULL?temp1:""),NULL,FALSE);
											if(temp1!=NULL) Rfree(temp1);

											if(s->skip_widget==FALSE)
											{
																					if(update_rsrcs==TRUE) updatefilerecord2rsrc(fileno,mrsrc);
																					else filerecord2rsrc(fileno,mrsrc);
											}
											return(FALSE);
										}
										}
/*
										FINDFLDGETCHAR(fileno,"DELETEFLAG",&deleteflag);
										if(deleteflag)
										{
											for(y=0,part=key->part;(y<key->numparts);++y,++part)
											{
												if(y+1<key->numparts)
												{
													temp1=Rmalloc(3+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
													sprintf(temp1,"%s, ",(isEMPTY(part->name)?"NULL":part->name));
												} else {
													temp1=Rmalloc(5+(isEMPTY(part->name)?4:RDAstrlen(part->name)));
													sprintf(temp1,"and %s",(isEMPTY(part->name)?"NULL":part->name));
												}
												temp=stralloccat(temp,temp1);
												if(temp1!=NULL) Rfree(temp1);	
											}
											temp1=Rmalloc(222+RDAstrlen(temp)+RDAstrlen(s->module)+RDAstrlen(s->filename));
											sprintf(temp1,"The values for the key part(s) %s have a corresponding record in the Supporting file of [%s][%s] but it is in a deleted state.  Therefore, the values are invalid.  Valid key values must be entered before saving this record.",
	(temp==NULL?"":temp),
	(s->module==NULL?"":s->module),
	(s->filename==NULL?"":s->filename));
											if(temp!=NULL) Rfree(temp);
											WARNINGDIALOG("WARNING DIALOG SCREEN","DELETED KEY VALUES!",(temp1==NULL?"":temp1),NULL,NULL,TRUE,NULL);
											if(temp1!=NULL) Rfree(temp1);
																					if(update_rsrcs==TRUE) updatefilerecord2rsrc(fileno,mrsrc);
											else filerecord2rsrc(fileno,mrsrc);
											return(FALSE);
										}
*/
										}
									}
								} else {
									if(confieldname!=NULL) Rfree(confieldname);
									if(s->skip_widget==FALSE)
									{
									if(update_rsrcs==TRUE) updatefilerecord2rsrc(fileno,mrsrc);
									else filerecord2rsrc(fileno,mrsrc); 
									}
									if(s->ctype<1 && display_diags)
									{
									if(update_rsrcs==TRUE) updatefilerecord2rsrc(fileno,mrsrc);
									else filerecord2rsrc(fileno,mrsrc);
									return(FALSE);
									}
								}
							} else {
								if(s->skip_widget==FALSE)
								{
								if(update_rsrcs==TRUE) updatefilerecord2rsrc(fileno,mrsrc);
								else filerecord2rsrc(fileno,mrsrc);
								}
								if(s->ctype<1 && display_diags)
								{
								part=key->part+y;
								temp1=Rmalloc(231+RDAstrlen(s->module)+RDAstrlen(s->filename)+RDAstrlen(s->cmodule)+RDAstrlen(s->cfilename));
								sprintf(temp1,
									"Could not copy the Field Values of the for the Connect File [%s][%s] to the Supporting File [%s][%s].  The definition is therefore invalid, please have it corrected in the Maintain Master Definition so that a record can be saved.",
									(s->cmodule!=NULL?s->cmodule:""),
									(s->cfilename!=NULL?s->cfilename:""),
									(s->module!=NULL?s->module:""),
									(s->filename!=NULL?s->filename:""));
								if(confieldname!=NULL) Rfree(confieldname);
/*
								WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID KEY VALUES!",(temp1==NULL?"":temp1),NULL,NULL,TRUE,NULL);
*/
								ERRORDIALOG("INVALID FIELD VALUE!",(temp1!=NULL?temp1:""),NULL,FALSE);
								if(temp1!=NULL) Rfree(temp1);
								return(FALSE);
								}
							}
						} else {
							if(s->skip_widget==FALSE)
							{
							if(update_rsrcs==TRUE) updatefilerecord2rsrc(fileno,mrsrc);
							else filerecord2rsrc(fileno,mrsrc); 
							}
							if(s->ctype<1 && display_diags)
							{
							temp1=Rmalloc(300+RDAstrlen(s->subkeyname)+RDAstrlen(s->module)+RDAstrlen(s->filename)+RDAstrlen(s->cmodule)+RDAstrlen(s->cfilename));
							sprintf(temp1,"The number of Key Parts in the [%s] key in the Supporting file of [%s][%s] do not match the number of defined connect fields to the Connect file of [%s][%s].",
								(s->subkeyname!=NULL?s->subkeyname:""),
								(s->module!=NULL?s->module:""),
								(s->filename!=NULL?s->filename:""),
								(s->cmodule!=NULL?s->cmodule:""),
								(s->cfilename!=NULL?s->cfilename:""));
							if(confieldname!=NULL) Rfree(confieldname);
/*
							WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID NUMBER OF KEYS DEFINED!",(temp1==NULL?"":temp1),NULL,NULL,TRUE,NULL);
*/
							ERRORDIALOG("INVALID NUMBER OF KEYS DEFINED!",(temp1!=NULL?temp1:""),NULL,FALSE);
							if(temp1!=NULL) Rfree(temp1);
							return(FALSE);
							}
						}
					} else {
						if(s->skip_widget==FALSE)
						{
						if(update_rsrcs==TRUE) updatefilerecord2rsrc(fileno,mrsrc);
						else filerecord2rsrc(fileno,mrsrc); 
						}
						if(s->ctype<1 && display_diags)
						{
						temp1=Rmalloc(290+RDAstrlen(s->module)+RDAstrlen(s->filename)+RDAstrlen(s->cmodule)+RDAstrlen(s->cfilename));
						sprintf(temp1,
							"There are no Connect Fields Values defined to connect the file of [%s][%s] to the Supporting file of [%s][%s].  Please have this corrected in the Maintain Master definition so that records can be saved to this file.",
							(s->cmodule!=NULL?s->cmodule:""),
							(s->cfilename!=NULL?s->cfilename:""),
							(s->module!=NULL?s->module:""),
							(s->filename!=NULL?s->filename:""));
						if(confieldname!=NULL) Rfree(confieldname);
/*
						WARNINGDIALOG("WARNING DIALOG SCREEN","CANNOT CONNECT SUPPORTING FILE!",(temp1=NULL?"":temp1),NULL,NULL,TRUE,NULL);
*/
						ERRORDIALOG("INVALID NUMBER OF KEYS DEFINED!",(temp1!=NULL?temp1:""),NULL,FALSE);
						if(temp1!=NULL) Rfree(temp1);
						return(FALSE);
						}
					}
				} else {
					if(s->skip_widget==FALSE)
					{
					if(update_rsrcs==TRUE) updatefilerecord2rsrc(fileno,mrsrc);
					else filerecord2rsrc(fileno,mrsrc); 
					}
					if(s->ctype<1 && display_diags)
					{
					temp1=Rmalloc(300+RDAstrlen(s->module)+RDAstrlen(s->filename)+RDAstrlen(s->subkeyname));
					sprintf(temp1,"Cannot get the Supporting file's key of [%s] for the Supporting File of [%s][%s].  Please have this corrected in the Maintain Master definition so that records can be saved to this file.",
						(s->subkeyname!=NULL?s->subkeyname:""),
						(s->module!=NULL?s->module:""),
						(s->filename!=NULL?s->filename:""));
					if(confieldname!=NULL) Rfree(confieldname);
/*
					WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID SUPPORTING KEYNAME!",(temp1==NULL?"":temp1),NULL,NULL,TRUE,NULL);
*/
					ERRORDIALOG("INVALID SUPPORT KEYNAME!",(temp1!=NULL?temp1:""),NULL,FALSE);
					if(temp1!=NULL) Rfree(temp1);
					return(FALSE);
					}
				}
			} else {
				if(s->skip_widget==FALSE)
				{
				if(update_rsrcs==TRUE) updatefilerecord2rsrc(fileno,mrsrc);
				else filerecord2rsrc(fileno,mrsrc); 
				}
				if(s->ctype<1 && display_diags)
				{
				temp1=Rmalloc(300+RDAstrlen(s->module)+RDAstrlen(s->filename)+RDAstrlen(s->subkeyname));
				sprintf(temp1,
					"The Supporting file of [%s][%s] has an Invalid Supporting KeyName of [%s] defined.  Please have this corrected in the Maintain Master definition so that records can be saved to this file.",
					(s->module!=NULL?s->module:""),
					(s->filename!=NULL?s->filename:""),
					(s->subkeyname!=NULL?s->subkeyname:""));
				if(confieldname!=NULL) Rfree(confieldname);
/*
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID SUPPORTING KEYNAME!",(temp1==NULL?"":temp1),NULL,NULL,TRUE,NULL);
*/
				ERRORDIALOG("INVALID SUPPORTING KEYNAME!",(temp1!=NULL?temp1:""),NULL,FALSE);
				if(temp1!=NULL) Rfree(temp1);
				return(FALSE);
				}
			}
		} else {
			if(s->skip_widget==FALSE)
			{
			if(update_rsrcs==TRUE) updatefilerecord2rsrc(fileno,mrsrc);
			else filerecord2rsrc(fileno,mrsrc); 
			}
			if(s->ctype<1 && display_diags)
			{
			temp1=Rmalloc(300+RDAstrlen(s->module)+RDAstrlen(s->filename));
			sprintf(temp1,
				"Cannot find the Supporting file of [%s][%s].  Please have this corrected in the Maintain Master definition so that records can be saved to this file.",
				(s->module!=NULL?s->module:""),
				(s->filename!=NULL?s->filename:""));
			if(confieldname!=NULL) Rfree(confieldname);
/*
			WARNINGDIALOG("WARNING DIALOG SCREEN","CANNOT FIND SUPPORTING FILE!",(temp1==NULL?"":temp1),NULL,NULL,TRUE,NULL);
*/
			ERRORDIALOG("CANNOT FIND SUPPORTING FILE!",(temp1!=NULL?temp1:""),NULL,FALSE);
			if(temp1!=NULL) Rfree(temp1);
			return(FALSE);
			}
		}
		if(update_rsrcs==TRUE) updatefilerecord2rsrc(fileno,mrsrc);
		else filerecord2rsrc(fileno,mrsrc); 
		}
	}
	}
	}
	return(TRUE);
}
char xSAVE_CHECK(RDArsrc *mrsrc,MaintainMaster *MTNMASTER,void (*SubFunc)(...),void *args,int line,char *file)
{
	char deleteflag=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG SaveCheck at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	FINDFLDGETCHAR(MTNMASTER->passkey->fileno,"DELETEFLAG",&deleteflag);
	if(!deleteflag)
	{
		return(SetSupportingFiles(mrsrc,MTNMASTER,SubFunc,args,(MTNMASTER->mbl_utlmstr==NULL?TRUE:FALSE),TRUE));
	} else {
		return(TRUE);
	}
}
void xGET_SUPPORTING(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER,void (*SubFunc)(...),void *args,short update_rsrcs,short display_diags,int line,char *file)
{
	if(update_rsrcs==TRUE && MTNMASTER->mbl_utlmstr==NULL)
	{
		SetSupportingFiles(mtnrsrc,MTNMASTER,SubFunc,args,TRUE,display_diags);
	} else {
		SetSupportingFiles(mtnrsrc,MTNMASTER,SubFunc,args,FALSE,display_diags);
	}
}
void xGET_SUPPORTING_NONSAVE(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER,void (*SubFunc)(...),void *args,short update_rsrcs,short display_diags,int line,char *file)
{
	SetSupportingFiles(mtnrsrc,MTNMASTER,SubFunc,args,update_rsrcs,display_diags);
}
void xSET_MTNMASTER(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER,void (*SubFunc)(...),void *args,int line,char *file)
{
	readallwidgets(mtnrsrc);
	IncVirRsrcstoFile(mtnrsrc);
/* Don't need to call this because this function only called from non RDAScrolledList resources.
	ReadRDAScrolledLists(mtnrsrc);
	if(!MTNMASTER->file_type)
	{
		rsrc2filerecord(MTNMASTER->passkey->fileno,mtnrsrc);
	} else {
		rsrc2singlefilerecord(MTNMASTER->passkey->fileno,mtnrsrc);
	}
*/
	GET_SUPPORTING(mtnrsrc,MTNMASTER,SubFunc,args,TRUE,FALSE);
	MSTADVupdateSCRNvirtuals(mtnrsrc,SubFunc,args,MTNMASTER);
}
void xSET_KEYFUNC(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER,RDATData *prevmain,
	void (*SubFunc)(...),void *args,short inheritrsrcs,
	void (*NEfunc)(...),void *funcarg1,void (*BUEQLfunc)(...),void *funcarg2,
	void (*AUEQLfunc)(...),void *funcarg3,int line,char *file)
{
	readallwidgets(mtnrsrc);
	ReadRDAScrolledLists(mtnrsrc);
	if(!MTNMASTER->file_type)
	{
		rsrc2filerecord(MTNMASTER->passkey->fileno,mtnrsrc);
	} else {
		rsrc2singlefilerecord(MTNMASTER->passkey->fileno,mtnrsrc);
	}
	if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,1,SubFunc,args))
	{
		KEYNRD(MTNMASTER->passkey->fileno,1);
		ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
		if(inheritrsrcs) 
		{
			if(!MTNMASTER->file_type)
			{
				updatefilersrc(MTNMASTER->passkey->fileno,mtnrsrc);
				rsrc2filerecord(MTNMASTER->passkey->fileno,mtnrsrc);
			} else {
				updatesinglefilersrc(MTNMASTER->passkey->fileno,mtnrsrc);
				rsrc2singlefilerecord(MTNMASTER->passkey->fileno,mtnrsrc);
			}
		} else {
			updatefilerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
		}
		if(prevmain!=NULL) ClearRDATData(prevmain);
		if(NEfunc!=NULL)
		{
/*lint -e746 */
			NEfunc(mtnrsrc,MTNMASTER,SubFunc,args,funcarg1);
/*lint +e746 */
		}
		GET_SUPPORTING(mtnrsrc,MTNMASTER,SubFunc,args,TRUE,FALSE);
	} else {
		if(prevmain!=NULL) GetRDATData(MTNMASTER->passkey->fileno,prevmain);
		if(BUEQLfunc!=NULL)
		{
/*lint -e746 */
			BUEQLfunc(mtnrsrc,MTNMASTER,SubFunc,args,funcarg2);
/*lint +e746 */
		}
		if(AUEQLfunc!=NULL)
		{
/*lint -e746 */
			AUEQLfunc(mtnrsrc,MTNMASTER,SubFunc,args,funcarg3);
/*lint +e746 */
		} else { 
			if(!MTNMASTER->file_type)
			{
				updatefilerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
			} else {
				updatesinglerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
			}
		} 
		GET_SUPPORTING(mtnrsrc,MTNMASTER,SubFunc,args,TRUE,TRUE);
	}
	MSTADVupdateSCRNvirtuals(mtnrsrc,SubFunc,args,MTNMASTER);
}
void xQUIT_RECORD_TEST(RDArsrc *mtnrsrc,short filenum,RDATData *previous,
	void (*readscreen)(...),void (*save_record)(...),void (*quit_record)(...),void *args,
	int line,char *file)
{
	int x=(-1);
	RDArmem *member;
	char test=FALSE;
	char *desc=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG QuitRecordTest at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if((x=FINDRSC(mtnrsrc,"SAVE"))!=(-1))
	{
		member=mtnrsrc->rscs+x;
		test=(char)IsWidgetDisplayed(member);
	}
	if(!test)
	{
		if((x=FINDRSC(mtnrsrc,"SAVE NO UPDATE"))!=(-1))
		{
			member=mtnrsrc->rscs+x;
			test=(char)IsWidgetDisplayed(member);
		}
	}
	if(test)
	{
		if(readscreen!=NULL)
		{
/*lint -e746 */
			readscreen(mtnrsrc,args);
/*lint +e746 */
		}
		if(QUITvfy(filenum,previous))
		{
			desc=QUITvfydesc(filenum,previous);
			ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN",
				"CHANGES NOT SAVED!",desc,save_record,quit_record,
				FALSE,2,mtnrsrc,args,NULL);
			if(desc!=NULL) Rfree(desc);
		} else {
/*lint -e746 */
			quit_record(mtnrsrc,args);
/*lint +e746 */
		}
	} else  {
/*lint -e746 */
		quit_record(mtnrsrc,args);
/*lint +e746 */
	}
}
short xReturnSupportingFileno(MaintainMaster *m,char *mod,char *fil,short occurrence,
	int line,char *file)
{
	short count=0;
	int x=0;
	RDAsearch *i=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG ReturnSupportingFileno for Module [%s] File [%s] Occurrence [%d] at line [%d] program [%s].",mod,fil,occurrence,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(m!=NULL)
	{
		if(m->s!=NULL)
		{
			for(x=0,i=m->s;x<m->num;++x,++i)
			{
				if(!RDAstrcmp(i->module,mod) && !RDAstrcmp(i->filename,fil))
				{
					++count;
					if(count==occurrence) return(i->fileno);
				}
			}
		}
		return(-1);
	}
	return(-1);
}
short xSetSupportingWritable(MaintainMaster *m,char *mod,char *fil,short occurrence,
	short writable,char *editable,int line,char *file)
{
	short count=0;
	int x=0;
	RDAsearch *i=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG SetSupportingWritable for Module [%s] File [%s] Occurance [%d] Writable [%s] at line [%d] program [%s].",mod,fil,occurrence,(writable==TRUE?"TRUE":"FALSE"),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(m!=NULL)
	{
		if(m->s!=NULL)
		{
			for(x=0,i=m->s;x<m->num;++x,++i)
			{
				if(!RDAstrcmp(i->module,mod) && !RDAstrcmp(i->filename,fil))
				{
					++count;
					if(count==occurrence)
					{
						i->writable=writable;
						return(0);
					}
				}
			}
		}
		*editable=FALSE;
		prterr("Error Cannot find the File [%s][%s] with Occurence [%4d] in the Maintain Master Definition.",mod,fil,occurrence);
		return(-1);
	}
	*editable=FALSE;
	prterr("Error Maintain Master Definition not available.");
	return(-1);
}
short xAPPReturnSupportingFileno(MaintainMaster *m,char *mod,char *fil,
	short occurrence,int line,char *file)
{
	short fileno=(-1);
	char *desc=NULL;

	fileno=ReturnSupportingFileno(m,mod,fil,occurrence);
	if(fileno==(-1))
	{
		desc=Rmalloc(86+RDAstrlen(mod)+RDAstrlen(fil)+5);
		sprintf(desc,"Cannot find the File [%s][%s] of Occurrence [%4d] in the Maintain Master Definition.\n",mod,fil,occurrence);
		ERRORDIALOG("CANNOT FIND FILE!",desc,ShutdownOnError,TRUE);
		if(desc!=NULL) Rfree(desc);
		RDAAPPMAINLOOP();
	}
	return(fileno);
}
void ApplyActionExpression(RDArmem *rec,char *expression,void (*SubFunc)(...),void *SubFuncArgs)
{

	char *temp1=NULL;
	int len=0;

	switch(rec->field_type)
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
			temp1=EVALUATEstr(expression,SubFunc,SubFuncArgs);
			
			if(!isEMPTY(temp1))
			{
				len=RDAstrlen(temp1);
				QUICKALLOC(rec->value.string_value,rec->dlen,len+1);
				memcpy(rec->value.string_value,temp1,len+1);
			} else if(rec->dlen>0)
			{
				memset(rec->value.string_value,0,rec->dlen);
			} else {
				rec->value.string_value=NULL;
				rec->dlen=0;
			}
			break;
		case SHORTV:
		case SSHORTV:
			*rec->value.short_value=EVALUATEsht(expression,SubFunc,SubFuncArgs);
			break;
		case SCROLLEDLIST:
		case LONGV:
		case SLONGV:
			*rec->value.integer_value=EVALUATEint(expression,SubFunc,SubFuncArgs);
			break;
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case DECIMALV:
		case DOUBLEV:
		case SDOUBLEV:
		case SDECIMALV:
			*rec->value.float_value=EVALUATEdbl(expression,SubFunc,SubFuncArgs);
			break;
		case BOOLNS:
		case CHARACTERS:
			*rec->value.string_value=EVALUATEbol(expression,SubFunc,SubFuncArgs);
			break;
		default:
			prterr("Error field type [%d] is invalid for field [%s].",rec->field_type,rec->rscrname);
			break;
	}
	if(IsWidgetDisplayed(rec)) 
	{
		updatersrc(rec->parent,rec->rscrname);
	}
}
void ApplyActionStructure(RDArmem *member,MTNAction *a,void (*SubFunc)(...),void *SubFuncArgs,short override_expression)
{
	Actionstruct *actstruct=NULL;
	int y,x=0;
	short value=0;
	RDArmem *mem=NULL;
	RDArsrc *parent=NULL;

	if(a->a!=NULL)
	{
		parent=(RDArsrc *)member->parent;
		if(member->field_type==LONGV || member->field_type==SLONGV)
		{
			value=(short)*member->value.integer_value;
		} else if(member->field_type==SHORTV || member->field_type==SSHORTV)
		{
			value=*member->value.short_value;
		} else if(member->field_type==DOUBLEV || member->field_type==SDOUBLEV ||
			member->field_type==DECIMALV || member->field_type==SDECIMALV)
		{
			value=(short)*member->value.float_value;
		}
		for(y=0,actstruct=a->a;y<a->num;++y,++actstruct)
		{
			x=FINDRSC(parent,actstruct->to_name);
			if(x!=(-1))
			{
				mem=parent->rscs+x;
			}
			if(mem!=NULL)
			{
			switch(actstruct->typelist)
			{
				default:
				case 0: /* Expression Only */
					if(!override_expression)
					{
						ApplyActionExpression(mem,actstruct->expression,SubFunc,SubFuncArgs);
					}
					break;
				case 1: /* Expression & Set Acct Type */
					if(!override_expression)
					{
						ApplyActionExpression(mem,actstruct->expression,SubFunc,SubFuncArgs);
					}
					FINDRSCSETACCTTYPE(member->parent,actstruct->to_name,value);				
					break;
				case 2: /* Expression & Set Def Type */
					if(!override_expression)
					{
						ApplyActionExpression(mem,actstruct->expression,SubFunc,SubFuncArgs);
					}
					FINDRSCSETDEFTYPE(member->parent,actstruct->to_name,value);				
					break;
				case 3: /* Set Acct Type Only */
					FINDRSCSETACCTTYPE(member->parent,actstruct->to_name,value);				
					break;
				case 4: /* Set Def Type Only */
					FINDRSCSETDEFTYPE(member->parent,actstruct->to_name,value);				
					break;
			}
			}
		}
	}
}
void xDoMTNAction(RDArmem *member,MaintainMaster *m,void (*SubFunc)(...),
	void *SubFuncArgs,short override_expression,int line,char *file)
{
	MTNAction *a=NULL;
	int x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		if(member!=NULL)
		{
			prterr("DIAG DoMTNAction for Resource [%s] at Line [%d] Program [%s].",(member->rscrname!=NULL ? member->rscrname:""),line,file);
		} else {
			prterr("DIAG DoMTNAction for NULL RDArmem at Line [%d] Program [%s].",line,file);
		}
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(m!=NULL && member!=NULL)
	{
		if(m->Actions!=NULL)
		{
			for(x=0,a=m->Actions;x<m->numActions;++x,++a)
			{
				if(!RDAstrcmp(a->name,member->rscrname))
				{
					ApplyActionStructure(member,a,SubFunc,SubFuncArgs,override_expression);
					break;
				}
			}
		}
	}
}
void xActionrsrc2filerecord(short fileno,RDArmem *rmem,short type,MaintainMaster *m,void (*SubFunc)(...),void *SubFuncArgs,int line,char *file)
{
	int x;
	NRDfield *field=NULL;
	RDArmem *res;
	char *filename=NULL,*fieldname=NULL;
	RDArsrc *rsrc=NULL;
	
	if(!TESTFILENO(fileno,line,file)) return;
	rsrc=(RDArsrc *)rmem->parent;
	filename=FILENAME(fileno);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG rsrc2filerecord Setting field structures in Module [%s] File [%d] [%s] from values in Screen [%s] [%s]'s Resources at line [%d] program [%s].",MODULENAME(fileno),fileno,filename,rsrc->module,rsrc->screen,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(filename!=NULL)
	{
	for(x=0,res=rsrc->rscs;x<rsrc->numrscs;++x,++res)
	{
		if(!type)
		{
			fieldname=gfname(filename,res->rscrname);
			if(fieldname!=NULL) 
			{
/*
				field=FLDNRDsec(fileno,fieldname,TRUE,TRUE,FALSE);
*/
				field=FLDNRD(fileno,fieldname);
				Rfree(fieldname);
			} else field=NULL;
		} else {
/*
			field=FLDNRDsec(fileno,res->rscrname,TRUE,TRUE,FALSE);
*/
			field=FLDNRD(fileno,res->rscrname);
		}
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
				case OPTIONALFIELDS:
					if(!isEMPTY(res->value.string_value))
					{
						unpad(res->value.string_value);
						QUICKALLOC(field->data.string_value,field->dlen,res->dlen);
						memcpy(field->data.string_value,res->value.string_value,res->dlen);
						if(field->dlen>field->len && field->len>0)
						{
							field->data.string_value[field->len]=0;
						}
					} else if(field->dlen>0)
					{
						memset(field->data.string_value,0,field->dlen);
					} else {
						field->data.string_value=NULL;
						field->dlen=0;
					}
					break;
				case SHORTV:
				case SSHORTV:
					*field->data.short_value=
						*res->value.short_value;
					break;
				case LONGV:
				case SLONGV:
				case SCROLLEDLIST:
					*field->data.integer_value=
						*res->value.integer_value;
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					*field->data.float_value=
						*res->value.float_value;
					break;
				case BOOLNS:
				case CHARACTERS:
					*field->data.string_value=
						*res->value.string_value;
					break;
				default:
					prterr("Error Field type [%d] is invalid for [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",field->type,field->name,MODULENAME(fileno),fileno,filename,line,file); 
					break;
			}
		}
	}
	for(x=0,res=rsrc->rscs;x<rsrc->numrscs;++x,++res)
	{
		if(!RDAstrcmp(rmem->rscrname,res->rscrname))
		{
			if(m->Actions!=NULL && m->numActions>0)
			{
				DoMTNAction(res,m,SubFunc,SubFuncArgs,FALSE);
			}
		}
	}
	}
}
void xApplyActions(RDArsrc *r,MaintainMaster *m,void (*SubFunc)(...),
	void *SubFuncArgs,short override_expression,int line,char *file)
{
	int y,x=0;
	MTNAction *action=NULL;
	RDArmem *mem=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG ApplyActions at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(m->Actions!=NULL)
	{
		for(x=0,action=m->Actions;x<m->numActions;++x,++action)
		{
			if(!isEMPTY(action->name))
			{
				y=FINDRSC(r,action->name);
				if(y!=(-1))
				{
					mem=r->rscs+y;
					DoMTNAction(mem,m,SubFunc,SubFuncArgs,override_expression);
				}
			}
		}
	}
}
void xRMEMSET_MASTER(RDArmem *member,MaintainMaster *MTNMASTER,void (*SubFunc)(...),void *args,int line,char *file)
{
	RDArsrc *mtnrsrc=NULL;
	
	mtnrsrc=(RDArsrc *)member->parent;
	readallwidgets(mtnrsrc);
	IncVirRsrcstoFile(mtnrsrc);
	if(MTNMASTER!=NULL)
	{
		if(!MTNMASTER->file_type)
		{
			Actionrsrc2filerecord(MTNMASTER->passkey->fileno,member,MTNMASTER,SubFunc,args);
		} else {
			Actionrsrc2singlefilerecord(MTNMASTER->passkey->fileno,member,MTNMASTER,SubFunc,args);
		}
		GET_SUPPORTING_NONSAVE(mtnrsrc,MTNMASTER,SubFunc,args,TRUE,FALSE);
		MSTADVupdateSCRNvirtuals(mtnrsrc,SubFunc,args,MTNMASTER);
	}
}
void xRMEMSET_KEYFUNC(RDArmem *member,MaintainMaster *MTNMASTER,RDATData *prevmain,
	void (*SubFunc)(...),void *args,short inheritrsrcs,
	void (*NEfunc)(...),void *funcarg1,void (*BUEQLfunc)(...),void *funcarg2,
	void (*AUEQLfunc)(...),void *funcarg3,int line,char *file)
{
	RDArsrc *mtnrsrc=NULL;

	mtnrsrc=(RDArsrc *)member->parent;
	readallwidgets(mtnrsrc);
	if(!MTNMASTER->file_type)
	{
		Actionrsrc2filerecord(MTNMASTER->passkey->fileno,member,MTNMASTER,SubFunc,args);
	} else {
		Actionrsrc2singlefilerecord(MTNMASTER->passkey->fileno,member,MTNMASTER,SubFunc,args);
	}
	if(ADVEQLNRDsec(MTNMASTER->passkey->fileno,1,SubFunc,args))
	{
		KEYNRD(MTNMASTER->passkey->fileno,1);
		ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
		if(inheritrsrcs) 
		{
			if(!MTNMASTER->file_type)
			{
				updatefilersrc(MTNMASTER->passkey->fileno,mtnrsrc);
				rsrc2filerecord(MTNMASTER->passkey->fileno,mtnrsrc);
			} else {
				updatesinglefilersrc(MTNMASTER->passkey->fileno,mtnrsrc);
				rsrc2singlefilerecord(MTNMASTER->passkey->fileno,mtnrsrc);
			}
		} else {
			updatefilerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
		}
/* Added this line to execute actions after failing to obtain record */
		DoMTNAction(member,MTNMASTER,SubFunc,args,FALSE);
		if(prevmain!=NULL) ClearRDATData(prevmain);
		if(NEfunc!=NULL)
		{
/*lint -e746 */
			NEfunc(mtnrsrc,MTNMASTER,SubFunc,args,funcarg1);
/*lint +e746 */
		}
		GET_SUPPORTING(mtnrsrc,MTNMASTER,SubFunc,args,TRUE,FALSE);
	} else {
		if(prevmain!=NULL) GetRDATData(MTNMASTER->passkey->fileno,prevmain);
		if(BUEQLfunc!=NULL)
		{
/*lint -e746 */
			BUEQLfunc(mtnrsrc,MTNMASTER,SubFunc,args,funcarg2);
/*lint +e746 */
		}
		if(AUEQLfunc!=NULL)
		{
/*lint -e746 */
			AUEQLfunc(mtnrsrc,MTNMASTER,SubFunc,args,funcarg3);
/*lint +e746 */
		} else {
			if(!MTNMASTER->file_type)
			{
				updatefilerecord2rsrc(MTNMASTER->passkey->fileno,mtnrsrc);
				rsrc2filerecord(MTNMASTER->passkey->fileno,mtnrsrc);
			} else {
				updatesinglefilersrc(MTNMASTER->passkey->fileno,mtnrsrc);
				rsrc2singlefilerecord(MTNMASTER->passkey->fileno,mtnrsrc);
			}
		}
		GET_SUPPORTING(mtnrsrc,MTNMASTER,SubFunc,args,TRUE,TRUE);
	}
	MSTADVupdateSCRNvirtuals(mtnrsrc,SubFunc,args,MTNMASTER);
}
