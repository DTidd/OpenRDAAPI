/* mknrd.c - Make Datafiles */
#include<mknrd.hpp>
#include<nrdp1.hpp>

char xCompareDATABASE(DBsort *sort,int line,char *file)
{
	NRDkey *skey=NULL,*fkey=NULL;
	NRDpart *spart=NULL,*fpart=NULL;
	NRDbuffer *NRDtmp=NULL;
	NRDfield *ff=NULL,*sf=NULL;
	int x=0,y=0,z=0;
	short fp=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG CompareDATABASE at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */

/* Checks to make sure the .dat and .idx exist, if they don't then the 
   file is created. 
*/
	fp=OPNNRD(sort->module,sort->filename);
	if(fp==(-1))
	{
		return(FALSE);
	}
	CLSNRD(fp);

	NRDtmp=NRDtmpGETBIN(sort->module,sort->filename);
	if(NRDtmp!=NULL)
	{
		if(sort!=NULL)
		{
/* Compaire keys by name and parts, if they aren't exactly the same 
   the datafile is changed to use the new keys.
*/
			if(sort->keys!=NULL && sort->nokeys>0)
			{
			if(NRDtmp->key!=NULL && NRDtmp->nokeys>0)
			{
				if(sort->nokeys!=NRDtmp->nokeys)
				{
					return(FALSE);
				} else {
					for(x=0,skey=sort->keys;x<sort->nokeys;++x,++skey)
					{
						for(y=0,fkey=NRDtmp->key;y<NRDtmp->nokeys;++y,++fkey)
						{
							if(!RDAstrcmp(skey->name,fkey->name)) break;
						}
						if(y>=NRDtmp->nokeys)
						{
							return(FALSE);
						} else if(skey->numparts!=fkey->numparts)
						{
							return(FALSE);
						} else {
							for(z=0,fpart=fkey->part,spart=skey->part;z<skey->numparts;++z,++fpart,++spart)
							{
								if(RDAstrcmp(fpart->name,spart->name))
								{
									return(FALSE);
								}
							}
						}
					}
				}
			} else {
				return(FALSE);
			}
			} else {
				return(TRUE);
			}
/* Compaire fields by name, if the sort file has names that aren't in the 
   existing datafile definition or it exist but the sort fields are larger 
   then the present ones then the datafile is changed. 
*/
			if(NRDtmp->field!=NULL)
			{
				if(sort->fields!=NULL)
				{
					for(x=0,sf=sort->fields;x<sort->nofields;++x,++sf)
					{
						for(y=0,ff=NRDtmp->field;y<NRDtmp->nofields;++y,++ff)
						{
							if(!RDAstrcmp(sf->name,ff->name)) break;
						}
						if(y>=NRDtmp->nofields)
						{
							return(FALSE);
						} else if(sf->len>ff->len)
						{
							return(FALSE);
						} else if(sf->type!=ff->type)
						{
							switch(sf->type)
							{
								case SCROLLEDTEXT:
								case VARIABLETEXT:
									break;
								case PLAINTEXT:
								case PHONE:
								case CUSTOMTYPE:
								case ZIPCODE:
								case SOCSECNUM:
								case DATES:
								case TIMES:
								case EXPENDITURE:
								case REVENUE:
								case BALANCESHEET:
								case BEGINNINGBALANCE:
								case OPTIONALFIELDS:
								case BOOLNS:
								case CHARACTERS:
									break;
								case DECIMALV:
								case DOLLARS:
								case DOLLARS_NOCENTS:
								case DOUBLEV:
								case SDECIMALV:
								case SDOUBLEV:
								case SHORTV:
								case SSHORTV:
								case LONGV:
								case SLONGV:
									return(FALSE);
								case BUTTONS:
								case SCROLLEDLIST:
									break;
								default:
									prterr("Error unrecognized field type.");
									break;
							}
							
						}
					}
				} else {
					return(TRUE);
				}
			} else {
				return(TRUE);
			}
		} else {
			return(TRUE);
		}
	} else {
		return(FALSE);
	}

	fp=OPNEXCLCKNRD(sort->module,sort->filename);
	if(fp==(-1))
	{
		prterr("Error: Can't get Exclusive Lock on database file [%s][%s], please get all users out of processes that are using this datafile before attempting to make it again",sort->module,sort->filename);
		return(TRUE);
	}
	CLSNRD(fp);

	return(TRUE);
}
void xMergeDATABASE(DBsort *sort,int line,char *file)
{
	NRDbuffer *NRDtmp=NULL;
	NRDfield *f=NULL,*sf=NULL,*field=NULL;
	NRDkey *key=NULL;
	NRDpart *part=NULL;
	int x,y,z=0,keylength=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG MergeDATABASE at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDtmpGETBIN(sort->module,sort->filename);
	if(NRDtmp!=NULL)
	{
		if(NRDtmp->field!=NULL)
		{
			for(x=0,f=NRDtmp->field;x<NRDtmp->nofields;++x,++f)
			{
				sf=DBsortFIELD(sort,f->name);
				if(sf!=NULL)
				{
					if(sf->len<f->len)
					{
						sf->len=f->len;
					}
					if(sf->type!=f->type)
					{
						switch(sf->type)
						{
							case SCROLLEDTEXT:
							case VARIABLETEXT:
								sf->type=f->type;
								break;
							case PLAINTEXT:
							case PHONE:
							case CUSTOMTYPE:
							case ZIPCODE:
							case SOCSECNUM:
							case DATES:
							case TIMES:
							case EXPENDITURE:
							case REVENUE:
							case BALANCESHEET:
							case BEGINNINGBALANCE:
							case OPTIONALFIELDS:
							case BOOLNS:
							case CHARACTERS:
								sf->type=f->type;
								break;
							case DECIMALV:
							case SDECIMALV:
							case DOLLARS:
							case DOLLARS_NOCENTS:
							case DOUBLEV:
							case SDOUBLEV:
								if(f->type==DECIMALV || f->type==SDECIMALV || 
									f->type==DOLLARS || f->type==DOLLARS_NOCENTS 
									|| f->type==DOUBLEV || f->type==SDOUBLEV)
								{
									/* use RDA standard */
								} else sf->type=f->type;
								break;
							case SHORTV:
							case SSHORTV:
								if(f->type==DECIMALV || f->type==SDECIMALV || 
									f->type==DOLLARS || f->type==DOLLARS_NOCENTS 
									|| f->type==DOUBLEV || f->type==SDOUBLEV ||
									f->type==LONGV || f->type==SLONGV)
								{
									sf->type=f->type;
								} else {
									/* use RDA standard */
								}
								break;
							case LONGV:
							case SLONGV:
								if(f->type==DECIMALV || f->type==SDECIMALV || 
									f->type==DOLLARS || f->type==DOLLARS_NOCENTS 
									|| f->type==DOUBLEV || f->type==SDOUBLEV)
								{
									sf->type=f->type;
								} else {
									/* use RDA standard */
								}
								break;
							case BUTTONS:
							case SCROLLEDLIST:
								sf->type=f->type;
								break;
							default:
								prterr("Error unrecognized field type.");
								break;
						}
							
					} else sf->type=f->type;
				} else {
					addDBfieldNoDuplicates(sort,f->name,f->type,f->len);
				}
			}
		}
		freenrd(NRDtmp);
	}
	if(sort->engine==NRDCtreeEngine)
	{
		for(x=0,key=sort->keys;x<sort->nokeys;++x,++key)
		{
			for(z=0,part=key->part;z<key->numparts;++z,++part)
			{
				for(y=0,field=sort->fields;y<sort->nofields;++y,++field)
				{
				if(!RDAstrcmp(field->name,part->name))
				{
					if(field->len<1)
					{
#ifdef USE_MYISAM
						sort->engine=NRDMyIsamEngine;
#else
						sort->engine=NRDRdaEngine;
#endif
					}
				}
				}
			}
		}
	} else if(sort->engine==NRDInformixIsamEngine)
	{
		for(x=0,key=sort->keys;x<sort->nokeys;++x,++key)
		{
			if(key->numparts>8)
			{
#ifdef USE_MYISAM
				sort->engine=NRDMyIsamEngine;
#else
				sort->engine=NRDRdaEngine;
#endif
			}
			keylength=0;
			for(z=0,part=key->part;z<key->numparts;++z,++part)
			{
				for(y=0,field=sort->fields;y<sort->nofields;++y,++field)
				{
				if(!RDAstrcmp(field->name,part->name))
				{
					switch(field->type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
#ifdef USE_MYISAM
							sort->engine=NRDMyIsamEngine;
#else
							sort->engine=NRDRdaEngine;
#endif
							break;
						case PLAINTEXT:
						case PHONE:
						case CUSTOMTYPE:
						case ZIPCODE:
						case SOCSECNUM:
						case DATES:
						case TIMES:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
							keylength+=field->len;
							break;
						case DECIMALV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DOUBLEV:
						case SDECIMALV:
						case SDOUBLEV:
							keylength+=sizeof(double);
							break;
						case SHORTV:
						case SSHORTV:
							keylength+=sizeof(short);
							break;
						case LONGV:
						case SLONGV:
							keylength+=sizeof(int);
							break;
						case BOOLNS:
						case CHARACTERS:
							keylength+=1;
							break;
						case BUTTONS:
						case SCROLLEDLIST:
							break;
						default:
							prterr("Error unrecognized field type.");
							break;
					}
				}
				}
			}
		}
		if(keylength>120)
		{
#ifdef USE_MYISAM
			sort->engine=NRDMyIsamEngine;
#else
			sort->engine=NRDRdaEngine;
#endif
		}
		for(x=0,field=sort->fields;x<sort->nofields;++x,++field)
		{
			if(field->len<1)
			{
#ifdef USE_MYISAM
				sort->engine=NRDMyIsamEngine;
#else
				sort->engine=NRDRdaEngine;
#endif
			}
		}
	}
}

short xCHANGE_DATABASE_FILE(char *modulename,short engine,char *filename,
	short nofield,NRDfield *fields,short nokeys,NRDkey *keys,
	char *servery,int line,char *file)
{
	short x,y=0,tmp_nofields=0,tmp_engine=0;
	short tmpfileno=0,ef=0,s=0;
	NRDfield *fld=NULL,*fieldx=NULL;
	char *tmp=NULL,*tmp2=NULL;
	char *libx=NULL;
	char *e=NULL,*serverx=NULL;
	RDArsrc *tmprsrc=NULL;
	NRDbuffer *NRDtmp=NULL;
	char def_server=FALSE;
	char def_server2=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagapps)
	{
		prterr("DIAG CHANGE_DATABASE_FILE at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(engine==NRDCtreeEngine)
	{
		if(isEMPTY(servery))
		{
			tmp2=RDA_GetEnv("CTREE_DEFAULT_SERVER");
			servery=stralloc(tmp2);
			def_server=TRUE;
		}
	}
	if((x=OPNNRD(modulename,filename))!=(-1))
	{
	CLSNRD(x);
	if((x=OPNEXCLCKNRD(modulename,filename))==(-1))
	{
		if(!RDA_NOGUI) ERRORDIALOG("OPEN FILE ERROR!","Couldn't open original file, file could be opended and/or locked by other processes.  Conversion failed.",NULL,FALSE);
		prterr("Error - Couldn't open original file [%s], file could be opened and/or locked by another process, conversion failed.",filename);
		return(-1);
	} else {
		tmp=Rmalloc(10);
		sprintf(tmp,"%d",RGETPID());	
		y=DELETENRD(modulename,tmp,0,servery);
		if(y!=(-1))
		{
			if(def_server==TRUE)
			{
				tmpfileno=CRTNRD(modulename,engine,tmp,nofield,fields,nokeys,keys,NULL);
			} else {
				tmpfileno=CRTNRD(modulename,engine,tmp,nofield,fields,nokeys,keys,servery);
			}
			if(tmpfileno!=(-1))
			{
				ef=BEGNRD(x);
				e=Rmalloc(RDAstrlen(tmp)+RDAstrlen(modulename)+27);
				sprintf(e,"Writing Temp File %s Module %s",tmp,modulename);
				if(!RDA_NOGUI)
				{
				tmprsrc=diagscrn(x,"DIAGNOSTIC SCREEN","DATABASE",e,NULL);
				}
				if(e!=NULL) Rfree(e);
				if(!RDA_NOGUI)
				{
				if(tmprsrc!=NULL)
				{
					if(!ADVmakescrn(tmprsrc,TRUE))
					{
						ForceWindowUpdate(tmprsrc);
					} else {
						e=Rmalloc(130+RDAstrlen(tmprsrc->module)+RDAstrlen(tmprsrc->screen));
						sprintf(e,"The Make Screen function failed for the custom screen [%s] [%s]. Check to see the screen is available. If it is, call your installer.",tmprsrc->module,tmprsrc->screen);
                				WARNINGDIALOG("WARNING DIALOG SCREEN","MAKESCRN FAILED",e,NULL,NULL,FALSE,NULL);
						prterr("Error %s.",e);
                				if(e!=NULL) Rfree(e);
					}
				}
				}
				fieldx=FLDPOINTER(x);
				tmp_nofields=NUMFLDS(x);
				NRDtmp=NRDbuf+x;
				tmp_engine=NRDtmp->db_engine;
				if(!isEMPTY(NRDtmp->servername))
				{
					serverx=stralloc(NRDtmp->servername);
				} else {
					tmp2=RDA_GetEnv("CTREE_DEFAULT_SERVER");
					serverx=stralloc(tmp2);
					def_server2=TRUE;
				}
				while(!ef)
				{
					for(s=0,fld=fieldx;s<tmp_nofields;++s,++fld)
					{
						COPYFIELD(x,tmpfileno,
							fld->name);
					}
					WRTNRD(tmpfileno,0,NULL);
					ef=SEQNRD(x);
					if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
				}
				CLSNRD(x);
				if(!RDA_NOGUI)
				{
				if(tmprsrc!=NULL)
				{
					killwindow(tmprsrc);
					free_rsrc(tmprsrc);
				}
				}
				if(def_server2==TRUE)
				{
					y=DELETENRD(modulename,filename,tmp_engine,NULL);
				} else {
					y=DELETENRD(modulename,filename,tmp_engine,serverx);
				}
				if(y!=(-1))
				{
					if(def_server==TRUE)
					{
						x=CRTNRD(modulename,engine,filename,nofield,fields,nokeys,keys,NULL);
					} else {
						x=CRTNRD(modulename,engine,filename,nofield,fields,nokeys,keys,servery);
					}
					if(x!=(-1))
					{
						if(!RDA_NOGUI)
						{
						e=Rmalloc(RDAstrlen(filename)+RDAstrlen(modulename)+21);
						sprintf(e,"Saving File %s Module %s",filename,modulename);
						tmprsrc=diagscrn(tmpfileno,"DIAGNOSTIC SCREEN","DATABASE",e,NULL);
						if(e!=NULL) Rfree(e);
						}
						if(!RDA_NOGUI)
						{
						if(tmprsrc!=NULL)
						{
							if(!ADVmakescrn(tmprsrc,TRUE))
							{
								ForceWindowUpdate(tmprsrc);
							}
						}
						}
						fieldx=FLDPOINTER(tmpfileno);
						tmp_nofields=NUMFLDS(tmpfileno);
						ef=BEGNRD(tmpfileno);
						while(!ef)
						{
							for(s=0,fld=fieldx;s<tmp_nofields;++s,++fld)
							{
								COPYFIELD(tmpfileno,x,
									fld->name);
							}
							WRTNRD(x,0,NULL);
							ef=SEQNRD(tmpfileno);
							if(!RDA_NOGUI)
							{
							if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
							}
						}
						CLSNRD(tmpfileno);		
						if(!RDA_NOGUI)
						{
						if(tmprsrc!=NULL)
						{
							killwindow(tmprsrc);
							free_rsrc(tmprsrc);
						}
						}
						if(def_server==TRUE)
						{
							DELETENRD(modulename,tmp,engine,NULL);
						} else {
							DELETENRD(modulename,tmp,engine,servery);
						}
						libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modulename)+10);
#ifndef WIN32
						sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modulename);
#endif
#ifdef WIN32
						sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modulename);
#endif
						deletelibbin(libx,tmp);
						if(libx!=NULL) Rfree(libx);
						if(serverx!=NULL) Rfree(serverx);
						if(tmp!=NULL) Rfree(tmp);
						return(x);
					} else {
						prterr("Error - Cannot create file [%s]. Data Saved in temporary file [%d] [%s]. Call your installer.",filename,tmpfileno,FILENAME(tmpfileno));
						CLSNRD(tmpfileno);
						if(tmp!=NULL) Rfree(tmp);
						if(serverx!=NULL) Rfree(serverx);
						return(-1);
					}
				} else {
					if(!RDA_NOGUI) ERRORDIALOG("DELETE FILE ERROR!","Couldn't delete original file created, file could be opended and/or locked by other processes.  Conversion failed.",NULL,FALSE);
					prterr("Error - Couldn't delete original file [%s], conversion failed.",filename);
					if(tmp!=NULL) Rfree(tmp);
					if(serverx!=NULL) Rfree(serverx);
					return(-1);
				}
			} else {
				if(!RDA_NOGUI) ERRORDIALOG("CREATE FILE ERROR!","Couldn't create temporary file.  Conversion failed.",NULL,FALSE);
				prterr("Error - Couldn't create temporary file [%s] conversion failed.",tmp);
				if(tmp!=NULL) Rfree(tmp);
				if(serverx!=NULL) Rfree(serverx);
				return(-1);
			}
		} else {
			if(!RDA_NOGUI) ERRORDIALOG("DELETE FILE ERROR!","Couldn't initially delete temporary file created by previous processing, file could be opended and/or locked by other processes.  Conversion failed.",NULL,FALSE);
			prterr("Error - Couldn't initially delete temporary file [%s] created by previous processing, conversion failed.",tmp);
			if(tmp!=NULL) Rfree(tmp);
			if(serverx!=NULL) Rfree(serverx);
			return(-1);
		}
	}
	} else {
		if(tmp!=NULL) Rfree(tmp);
		if(serverx!=NULL) Rfree(serverx);
		if(def_server==TRUE)
		{
			DELETENRD(modulename,filename,engine,NULL);
			return(CRTNRD(modulename,engine,filename,nofield,fields,nokeys,keys,NULL));
		} else {
			DELETENRD(modulename,filename,engine,servery);
			return(CRTNRD(modulename,engine,filename,nofield,fields,nokeys,keys,servery));
		}
	}	
}
void xSetupFileDefinition(DBsort *sort,int line,char *file)
{
	short z=0,x=0,y=0;
	int offset=0 /* ,start=0 */;
	short engine=0,offcount=0;
	NRDfield *field=NULL,*OFIELDS=NULL,*field1=NULL;
	NRDpart *part=NULL;
	NRDkey *key=NULL;
	APPlib *nonkeyfld=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG SetupFileDefinition at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if((sort->engine==NRDMyIsamEngine) || (sort->engine==NRDMySQLXpertEngine) ||
		(sort->engine==NRDMySQLEngine) || (sort->engine==NRDMySQLAutoEngine))
	{
		nonkeyfld=APPlibNEW();
		OFIELDS=Rmalloc(sort->nofields*sizeof(NRDfield));
		y=0;
		key=sort->keys+0;
		for(x=0,field=sort->fields;x<sort->nofields;++x,++field)
		{
			if(IsKeyPart(key,field->name))
			{
				field1=OFIELDS+y;
				field1->name=stralloc(field->name);
				field1->type=field->type;
				field1->len=field->len;
				++y;
			} else {
				addAPPlib(nonkeyfld,field->name);
			}
		}
		SORTAPPlib(nonkeyfld);
		for(z=0;z<nonkeyfld->numlibs;++z)
		{
			for(x=0,field=sort->fields;x<sort->nofields;++x,++field)
			{
				if(!RDAstrcmp(field->name,nonkeyfld->libs[z]) && field->type!=VARIABLETEXT && field->type!=SCROLLEDTEXT && field->len>0)
				{
					field1=OFIELDS+y;
					field1->name=stralloc(field->name);
					field1->type=field->type;
					field1->len=field->len;
					++y;
					break;
				}
			}
		}
		for(z=0;z<nonkeyfld->numlibs;++z)
		{
			for(x=0,field=sort->fields;x<sort->nofields;++x,++field)
			{
				if(!RDAstrcmp(field->name,nonkeyfld->libs[z]) && (field->type==VARIABLETEXT || field->type==SCROLLEDTEXT || field->len<1))
				{
					field1=OFIELDS+y;
					field1->name=stralloc(field->name);
					field1->type=field->type;
					field1->len=field->len;
					++y;
					break;
				}
			}
		}
		if(nonkeyfld!=NULL) freeapplib(nonkeyfld);
	} else {
		OFIELDS=Rmalloc(sort->nofields*sizeof(NRDfield));
		y=0;
		for(x=0,field=sort->fields;x<sort->nofields;++x,++field)
		{
			if(field->type!=VARIABLETEXT && field->type!=SCROLLEDTEXT
			 	&& field->len>0)
			{
				field1=OFIELDS+y;
				field1->name=stralloc(field->name);
				field1->type=field->type;
				field1->len=field->len;
				++y;
			}
		}
		for(x=0,field=sort->fields;x<sort->nofields;++x,++field)
		{
			if(field->type==VARIABLETEXT || field->type==SCROLLEDTEXT
			 	|| field->len<1)
			{
				field1=OFIELDS+y;
				field1->name=stralloc(field->name);
				field1->type=field->type;
				field1->len=field->len;
				++y;
			}
		}
	}
	for(x=0,field=sort->fields;x<sort->nofields;++x,++field)
	{
		if(field->name!=NULL) Rfree(field->name);
	}
	Rfree(sort->fields);
	sort->fields=Rmalloc(sort->nofields*sizeof(NRDfield));
	for(x=0,field1=OFIELDS,field=sort->fields;x<sort->nofields;++x,++field,++field1)
	{
		field->name=stralloc(field1->name);
		if(field1->name!=NULL) Rfree(field1->name);
		field->type=field1->type;
		field->len=field1->len;
	}
	Rfree(OFIELDS);
	for(x=0,key=sort->keys;x<sort->nokeys;++x,++key)
	{
		for(z=0,part=key->part;z<key->numparts;++z,++part)
		{
			offset=0;
			offcount=(-1);
			for(y=0,field=sort->fields;y<sort->nofields;++y,++field)
			{
				if(field->type==SCROLLEDTEXT || field->type==VARIABLETEXT || field->len<1)
				{
					++offcount;
				}
				if(!RDAstrcmp(field->name,part->name))
				{
					if(engine==NRDMyIsamEngine)
					{
						part->offset=offset;
					} else if(engine==NRDCtreeEngine || engine==NRDLocalCtreeEngine)
					{
						if(field->type==SCROLLEDTEXT || field->type==VARIABLETEXT || field->len<1)
						{
							part->offset=offcount;
						} else part->offset=offset;
					} else part->offset=offset;
					break;
				}
				switch(field->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
						if(engine==NRDMyIsamEngine)
						{
							offset+=field->len+4;
						} else if(engine==NRDInformixIsamEngine)
						{ 
							offset+=field->len;
						} else offset+=field->len+1;
						break;
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case ZIPCODE:
					case SOCSECNUM:
					case DATES:
					case TIMES:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						if(engine==NRDMyIsamEngine && field->len==0)
						{
							offset+=field->len+4;
						} else offset+=field->len;
						break;
					case DECIMALV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DOUBLEV:
					case SDECIMALV:
					case SDOUBLEV:
						offset+=sizeof(double);
						break;
					case SHORTV:
					case SSHORTV:
						offset+=sizeof(short);
						break;
					case LONGV:
					case SLONGV:
					case SCROLLEDLIST:
						offset+=sizeof(int);
						break;
					case BOOLNS:
					case CHARACTERS:
						offset+=1;
						break;
					case BUTTONS:
						break;
					default:
						prterr("Error unrecognized field type.");
						break;
				}
			}
		}
	}
}
