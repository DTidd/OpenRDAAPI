/* dbsort.c - Functions for managing sort files within Applications */
#include <nrdp.hpp>

DBsort *xDBsortNEW(char *module,char *filename,short engine,
	int line,char *file)
{
	DBsort *tmp=NULL;
	char *serverx=NULL,*engtype=NULL;
	short x=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG DBsortNEW creating a new sort file in Module [%s] filename [%s] engine [%d] at line [%d] program [%s]",module,filename,engine,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	serverx=RDA_GetEnv("DBsortSERVER");
	engtype=RDA_GetEnv("DBsortENGINE");
	tmp=Rmalloc(sizeof(DBsort));
	if(module!=NULL) tmp->module=stralloc(module);
		else tmp->module=NULL;
	if(filename!=NULL) tmp->filename=stralloc(filename);
		else tmp->filename=NULL;
	if(!isEMPTY(engtype))
	{
		for(x=0;x<NE(supported_engines);++x)
		{
			if(!RDAstrcmp(engtype,supported_engines[x])) break;
		}
		if(x<NE(supported_engines))
		{
			tmp->engine=x;
		} else {
#if !defined(WIN32) && !defined(LINUX) && !defined(SUNX86)
#ifdef USE_MYISAM
			tmp->engine=NRDMyIsamEngine;
#else
#ifdef INFORMIX_ISAM
			tmp->engine=NRDInformixIsamEngine;
#else
			tmp->engine=engine;
#endif
#endif /* USE_MYISAM */
#endif
#if defined(WIN32) || defined(LINUX) || defined(SUNX86)
#ifdef USE_MYISAM
			tmp->engine=NRDMyIsamEngine;
#else
#ifdef USE_CTREE
			tmp->engine=NRDLocalCtreeEngine;
#else
			tmp->engine=engine;
#endif
#endif /* USE_MYISAM */
#endif
		}
	} else {
#if !defined(WIN32) && !defined(LINUX) && !defined(SUNX86)
#ifdef USE_MYISAM
		tmp->engine=NRDMyIsamEngine;
#else
#ifdef INFORMIX_ISAM
		tmp->engine=NRDInformixIsamEngine;
#else
		tmp->engine=engine;
#endif
#endif /* USE_MYISAM */
#endif
#if defined(WIN32) || defined(LINUX) || defined(SUNX86)
#ifdef USE_MYISAM
		tmp->engine=NRDMyIsamEngine;
#else
#ifdef USE_CTREE
		tmp->engine=NRDLocalCtreeEngine;
#else
		tmp->engine=engine;
#endif
#endif /* USE_MYISAM */
#endif
	}
	if(!isEMPTY(serverx)) tmp->servername=stralloc(serverx);
		else tmp->servername=NULL;	
	tmp->nofields=0;
	tmp->fields=NULL;
	tmp->nokeys=0;
	tmp->keys=NULL;
	return(tmp);
}
short xaddDBfilefield(DBsort *dbsrt,short filenx,char *fieldname,int line,
	char *file)
{
	NRDfield *field=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG addDBfilefield Adding Field [%s] File [%d] to Sort File [%s] [%s] at line [%d] program [%s].",fieldname,filenx,dbsrt->module,dbsrt->filename,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(filenx,line,file)) return(-1);
	field=FLDNRD(filenx,fieldname);
	if(field!=NULL) 
	{
		return(xaddDBfield(dbsrt,fieldname,field->type,field->len,line,file));
	} else {
		prterr("Error Field [%s] not found at line [%d] program [%s].",fieldname,line,file);
		return(-1);
	}
}
short xaddDBfield(DBsort *dbsrt,char *name,short type,unsigned len,
	int line,char *file)
{
	NRDfield *f;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG addDBfield Adding Field [%s] Type [%d] [%s] Length [%d] to Sort File [%s] [%s] at line [%d] program [%s].",name,type,fieldtypedesc[type],len,dbsrt->module,dbsrt->filename,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(dbsrt->fields!=NULL)
	{
		dbsrt->fields=Rrealloc(dbsrt->fields,
			(dbsrt->nofields+1)*sizeof(NRDfield));
	} else {
		dbsrt->fields=Rmalloc(sizeof(NRDfield));
		dbsrt->nofields=0;
	}
	f=dbsrt->fields+dbsrt->nofields;
	if(name!=NULL) f->name=stralloc(name);
		else f->name=NULL;
	f->type=type;
	f->len=len;
	return(++dbsrt->nofields);
}
short xaddDBfieldNoDuplicates(DBsort *dbsrt,char *name,short type,
	unsigned len,int line,char *file)
{
	short x;
	NRDfield *f;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG addDBfieldNoDuplicates Adding Field [%s] Type [%d] [%s] Length [%d] to Sort File [%s] [%s] at line [%d] program [%s].",name,type,fieldtypedesc[type],len,dbsrt->module,dbsrt->filename,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,f=dbsrt->fields;x<dbsrt->nofields;++x,++f)
	{
		if(!RDAstrcmp(f->name,name)) return(x);
	}
	return(addDBfield(dbsrt,name,type,len));
}
short xaddDBkeypart(DBsort *dbsrt,short keyno,char *fname,int line,char *file)
{
	short kval=0;
	NRDkey *key;
	NRDpart *part;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG addDBkeypart Adding Key Part [%s] to Sort File [%s] [%s] Key Number [%d] at line [%d] program [%s].",fname,dbsrt->module,dbsrt->filename,keyno,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	kval=keyno-1;
	if(kval<0) kval=0;
	key=dbsrt->keys+kval;
	if(key->part!=NULL)
	{
		key->part=Rrealloc(key->part,(key->numparts+1)*sizeof(NRDpart));
	} else {
		key->part=Rmalloc(sizeof(NRDpart));
		key->numparts=0;
	}
	part=key->part+key->numparts;
	if(fname!=NULL) part->name=stralloc(fname);
		else part->name=NULL;
	return(++key->numparts);
}
short xaddDBkeypartNoDuplicates(DBsort *dbsrt,short keyno,char *fname,
	int line,char *file)
{
	short kval=0,x;
	NRDkey *key;
	NRDpart *part;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG addDBkeypartNoDuplicates Adding Key Part [%s] to Sort File [%s] [%s] Key Number [%d] at line [%d] program [%s].",fname,dbsrt->module,dbsrt->filename,keyno,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	kval=keyno-1;
	if(kval<0) kval=0;
	key=dbsrt->keys+kval;
	for(x=0,part=key->part;x<key->numparts;++x,++part)
	{
		if(!RDAstrcmp(part->name,fname)) return(x);
	}
	return(addDBkeypart(dbsrt,keyno,fname));
}
short xaddDBkey(DBsort *dbsrt,char *name,int line,char *file)
{
	NRDkey *key;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG addDBkey Adding Key [%s] to Sort File [%s] [%s] at line [%d] program [%s].",name,dbsrt->module,dbsrt->filename,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(dbsrt->keys!=NULL)
	{
		dbsrt->keys=Rrealloc(dbsrt->keys,(dbsrt->nokeys+1)*sizeof(NRDkey));
	} else {
		dbsrt->keys=Rmalloc(sizeof(NRDkey));
		dbsrt->nokeys=0;
	}
	key=dbsrt->keys+dbsrt->nokeys;
	if(name!=NULL) key->name=stralloc(name);
		else key->name=NULL;
	key->numparts=0;
	key->part=NULL;
	++dbsrt->nokeys;
	return(dbsrt->nokeys);
}
void xFreeDBsort(DBsort *dbsrt,short remove_datafile,int line,char *file)
{
	NRDfield *f;
	NRDkey *key;
	NRDpart *part;
	int x,y;
	char *libx=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG FreeDBsort Freeing Sort File at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(dbsrt!=NULL)
	{
		if(remove_datafile)
		{
			DELETENRD(dbsrt->module,dbsrt->filename,dbsrt->engine,dbsrt->servername);
			libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(dbsrt->module)+10);
#ifndef WIN32
			sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,dbsrt->module);
#endif
#ifdef WIN32
			sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,dbsrt->module);
#endif
			deletelibbin(libx,dbsrt->filename);
			if(libx!=NULL) Rfree(libx);
		}
		if(dbsrt->module!=NULL) Rfree(dbsrt->module);
		if(dbsrt->filename!=NULL) Rfree(dbsrt->filename);
		if(dbsrt->servername!=NULL) Rfree(dbsrt->servername);
		if(dbsrt->fields!=NULL)
		{
			for(x=0,f=dbsrt->fields;x<dbsrt->nofields;++x,++f)
			{
				if(f->name!=NULL) Rfree(f->name);
			}
			Rfree(dbsrt->fields);
		}
		if(dbsrt->keys!=NULL)
		{
			for(x=0,key=dbsrt->keys;x<dbsrt->nokeys;++x,++key)
			{
				if(key->name!=NULL) Rfree(key->name);
				if(key->part!=NULL)
				{
					for(y=0,part=key->part;y<key->numparts;++y,++part)
					{
						if(part->name!=NULL) Rfree(part->name);
					}
					Rfree(key->part);
				}
			}
			Rfree(dbsrt->keys);
		}
		Rfree(dbsrt);
		dbsrt=NULL;
	}
}
short xOPNDBsort(DBsort *dbsrt,int line,char *file)
{
	short z=0,x=0,y=0;
	int offset=0;
	short offcount=0;
	NRDfield *field=NULL,*OFIELDS=NULL,*field1=NULL;
	NRDpart *part=NULL;
	NRDkey *key=NULL;
	char *fname=NULL;
	APPlib *nonkeyfld=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG OPNDBsort Opening Sort File [%s] [%s] at line [%d] program [%s].",dbsrt->module,dbsrt->filename,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	OFIELDS=Rmalloc(dbsrt->nofields*sizeof(NRDfield));
	y=0;
	if((dbsrt->engine==NRDMyIsamEngine) || (dbsrt->engine==NRDMySQLXpertEngine) || (dbsrt->engine==NRDMySQLEngine))
	{
		nonkeyfld=APPlibNEW();
		OFIELDS=Rmalloc(dbsrt->nofields*sizeof(NRDfield));
		y=0;
		key=dbsrt->keys+0;
		for(x=0,field=dbsrt->fields;x<dbsrt->nofields;++x,++field)
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
			for(x=0,field=dbsrt->fields;x<dbsrt->nofields;++x,++field)
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
			for(x=0,field=dbsrt->fields;x<dbsrt->nofields;++x,++field)
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
		for(x=0,field=dbsrt->fields;x<dbsrt->nofields;++x,++field)
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
		for(x=0,field=dbsrt->fields;x<dbsrt->nofields;++x,++field)
		{
			if(field->type==VARIABLETEXT || field->type==SCROLLEDTEXT
			 	|| field->len==0)
			{
				field1=OFIELDS+y;
				field1->name=stralloc(field->name);
				field1->type=field->type;
				field1->len=field->len;
				++y;
			}
		}
	}
	for(x=0,field=dbsrt->fields;x<dbsrt->nofields;++x,++field)
	{
		if(field->name!=NULL) Rfree(field->name);
	}
	Rfree(dbsrt->fields);
	dbsrt->fields=Rmalloc(dbsrt->nofields*sizeof(NRDfield));
	for(x=0,field1=OFIELDS,field=dbsrt->fields;x<dbsrt->nofields;
		++x,++field,++field1)
	{
		field->name=stralloc(field1->name);
		if(field1->name!=NULL) Rfree(field1->name);
		field->type=field1->type;
		field->len=field1->len;
	}
	Rfree(OFIELDS);
	for(x=0,key=dbsrt->keys;x<dbsrt->nokeys;++x,++key)
	{
		for(z=0,part=key->part;z<key->numparts;++z,++part)
		{
			offset=0;
			offcount=(-1);
			for(y=0,field=dbsrt->fields;y<dbsrt->nofields;++y,++field)
			{
				if(field->type==SCROLLEDTEXT || field->type==VARIABLETEXT || field->len<1) 
				{
					++offcount;	
				}
				if(!RDAstrcmp(field->name,part->name))
				{
					if(dbsrt->engine==NRDMyIsamEngine)
					{
						part->offset=offset;
					} else if(dbsrt->engine==NRDCtreeEngine || dbsrt->engine==NRDLocalCtreeEngine)
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
						if(dbsrt->engine==NRDMyIsamEngine)
						{
							offset+=field->len+4;
						} else if(dbsrt->engine==NRDInformixIsamEngine)
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
						if(dbsrt->engine==NRDMyIsamEngine && field->len==0)
						{
							offset+=field->len+4;
						} else offset+=field->len;
						break;
					case DECIMALV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DOUBLEV:
					case SDECIMALV:
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
						prterr("Error Field Type [%d] is invalid for Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",field->type,field->name,dbsrt->module,dbsrt->filename,line,file);
						break;
				}
			}
		}
	}
#ifndef WIN32
	if(UseRamDisk==FALSE)
	{
		fname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(dbsrt->module)+
			RDAstrlen(dbsrt->filename)+10);
		sprintf(fname,"%s/%s/%s.dat",CURRENTDIRECTORY,dbsrt->module,(dbsrt->filename!=NULL ? dbsrt->filename:""));
		if(!access(fname,02))
		{
			DELETENRD(dbsrt->module,dbsrt->filename,dbsrt->engine,dbsrt->servername);
		}
		if(fname!=NULL) Rfree(fname);
		fname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(dbsrt->module)+
			RDAstrlen(dbsrt->filename)+10);
		sprintf(fname,"%s/%s/%s.MYD",CURRENTDIRECTORY,dbsrt->module,(dbsrt->filename!=NULL ? dbsrt->filename:""));
		if(!access(fname,02))
		{
			DELETENRD(dbsrt->module,dbsrt->filename,dbsrt->engine,dbsrt->servername);
		}
		if(fname!=NULL) Rfree(fname);
	} else {
		fname=Rmalloc(RDAstrlen("/dev/shm")+RDAstrlen(dbsrt->module)+
			RDAstrlen(dbsrt->filename)+10);
		sprintf(fname,"%s/%s/%s.MYD","/dev/shm",dbsrt->module,(dbsrt->filename!=NULL ? dbsrt->filename:""));
		if(!access(fname,02))
		{
			DELETENRD(dbsrt->module,dbsrt->filename,dbsrt->engine,dbsrt->servername);
		}
		if(fname!=NULL) Rfree(fname);
	}
#endif
#ifndef WIN32
	fname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(dbsrt->module)+
		RDAstrlen(dbsrt->filename)+10);
	sprintf(fname,"%s\\%s\\%s.dat",CURRENTDIRECTORY,dbsrt->module,(dbsrt->filename!=NULL ? dbsrt->filename:""));
	if(!access(fname,02))
	{
		DELETENRD(dbsrt->module,dbsrt->filename,dbsrt->engine,dbsrt->servername);
	}
	if(fname!=NULL) Rfree(fname);
	fname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(dbsrt->module)+
		RDAstrlen(dbsrt->filename)+10);
	sprintf(fname,"%s\\%s\\%s.MYD",CURRENTDIRECTORY,dbsrt->module,(dbsrt->filename!=NULL ? dbsrt->filename:""));
	if(!access(fname,02))
	{
		DELETENRD(dbsrt->module,dbsrt->filename,dbsrt->engine,dbsrt->servername);
	}
	if(fname!=NULL) Rfree(fname);
#endif
	dbsrt->fileno=CRTNRD(dbsrt->module,dbsrt->engine,dbsrt->filename,
		dbsrt->nofields,dbsrt->fields,dbsrt->nokeys,dbsrt->keys,
		dbsrt->servername);
	if(dbsrt->fileno==(-1))
	{
		prterr("Error Creating Sort File [%s] in Module [%s] at line [%d] program [%s].",dbsrt->filename,dbsrt->module,line,file);
	}
	return(dbsrt->fileno);
}
void xCLSDBsort(DBsort *dbsrt,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG CLSDBsort Close Sort File [%s] [%s] at line [%d] program [%s].",dbsrt->module,dbsrt->filename,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(dbsrt->fileno!=(-1))
	{
		CLSNRD(dbsrt->fileno);
	}
}
NRDfield *DBsortFIELD(DBsort *sort,char *name)
{
	int x;
	NRDfield *f=NULL;

	if(sort!=NULL)
	{
		if(sort->fields!=NULL)
		{
			for(x=0,f=sort->fields;x<sort->nofields;++x,++f)
			{
				if(!RDAstrcmp(f->name,name)) break;
			}
			if(x>=sort->nofields) return(NULL);
		}
	}
	return(f);
}
