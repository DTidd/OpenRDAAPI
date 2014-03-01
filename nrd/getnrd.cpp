#include <nrdp.hpp>

short xgetnrd(char *module,char *name,int line,char *file)
{
	short x,test=0;
	short y,version=FALSE;
	short nrdno;
	BIN *bin;
	NRDbuffer *NRDtmp;
	NRDfield *field;
	NRDkey *key;
	NRDpart *part;
	char *libname=NULL,*dashes=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field) { prterr("DIAG getnrd Reading File Definition [%s] [%s] at line [%d] program [%s].",module,name,line,file); }
#endif /* USE_RDA_DIAGNOSTICS */
	dashes=adddashes(module);
	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(dashes)+10);
#ifndef WIN32
	sprintf(libname,"%s/rda/%s.FIL",CURRENTDIRECTORY,dashes);
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,dashes);
#endif
	if(dashes!=NULL) Rfree(dashes);
	bin=getlibbin(libname,name,TRUE);
	if(bin==NULL)
	{
		prterr("Error can't read File Definition [%s] [%s] in Binary Library [%s] at line [%d] program [%s].",module,name,libname,line,file);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	x=BINgetshort(bin);
	if(x!=(-1701) && x!=(-1700) && x!=(-1699))
	{
		prterr("Error invalid version in File Definition [%s] [%s] in Binary Library [%s] at line [%d] program [%s].",module,name,libname,line,file);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	if(x==(-1699))
	{
		version=2;
	} else if(x==(-1700))
	{
		version=TRUE;
	} else version=FALSE;
	if(libname!=NULL) Rfree(libname);
	for(x=0;x<no_buffers;++x) if(NRDbuf[x].not_used) break;
	if(x>=no_buffers)
	{
		x=no_buffers;
		if(NRDbuf==NULL)
		{
			NRDbuf=Rmalloc(sizeof(NRDbuffer));
			++no_buffers;
		} else {
			NRDbuf=Rrealloc(NRDbuf,sizeof(NRDbuffer)*++no_buffers);
		}
	}
	nrdno=x;
	if(x==0) NRDtmp=NRDbuf;
		else NRDtmp=NRDbuf+x;
	NRDtmp->not_used=0;
	NRDtmp->db_engine=BINgetshort(bin);
	if(version>1)
	{
		test=BINgetshort(bin);
		if(test) NRDtmp->servername=BINgetstring(bin);
			else NRDtmp->servername=NULL;
	} else {
		NRDtmp->servername=NULL;
	}
	NRDtmp->modname=stralloc(module);
	NRDtmp->name=stralloc(name);
	NRDtmp->nofields=BINgetshort(bin);
	NRDtmp->field=Rmalloc(NRDtmp->nofields*sizeof(NRDfield));
	NRDtmp->nokeys=BINgetshort(bin);
	NRDtmp->key=Rmalloc(NRDtmp->nokeys*sizeof(NRDkey));
	NRDtmp->record_data=NULL;
	NRDtmp->dlen=0;
	NRDtmp->currec=0;
#ifdef USE_BERKELEY_BTREE
	NRDtmp->dbp=NULL;
	NRDtmp->recno=0;
	NRDtmp->lockerID=0;
	memset(&NRDtmp->lock,0,sizeof(DB_LOCK));
#endif
#ifdef USE_MYSQL
	NRDtmp->db=NULL;
	NRDtmp->result=NULL;
#endif
#ifdef USE_MYISAM
	NRDtmp->myinfo=NULL;
	NRDtmp->record_data2=NULL;
	NRDtmp->dlen2=0;
	NRDtmp->target=NULL;
	NRDtmp->tlen=0;
#endif
	NRDtmp->dtanum=NULL;
	NRDtmp->filenum=(-1);
	for(x=0,field=NRDtmp->field;x<NRDtmp->nofields;++x,++field)
	{
		field->name=BINgetstring(bin);
		field->type=BINgetshort(bin);
		if(version)
		{
			field->len=BINgetint(bin);
		} else field->len=BINgetshort(bin);
		switch(field->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT: /* variable length field type */
				field->data.string_value=NULL;
				field->dlen=0;
				break;
			case PLAINTEXT: /* plain text field type */
			case ZIPCODE:
			case DATES:
			case TIMES:
			case SOCSECNUM:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case OPTIONALFIELDS:
			case PHONE:
			case CUSTOMTYPE:
				field->data.string_value=NULL;
				field->dlen=0;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				field->data.float_value=Rmalloc(sizeof(double));
				*field->data.float_value=0;
				field->dlen=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				field->data.short_value=Rmalloc(sizeof(short));
				*field->data.short_value=0;
				field->dlen=sizeof(short);
				break;
			case LONGV:
			case SLONGV:
				field->data.integer_value=Rmalloc(sizeof(int));
				*field->data.integer_value=0;
				field->dlen=sizeof(int);
				break;
			case BOOLNS:
			case CHARACTERS:
				field->data.string_value=Rmalloc(1);
				*field->data.string_value=0;
				field->dlen=1;
				break;
			default:
				prterr("Error Field Type [%d] is invalid for Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",field->type,field->name,module,name,line,file);
				break;
		}
	}
	for(x=0,key=NRDtmp->key;x<NRDtmp->nokeys;++x,++key)
	{
		key->name=BINgetstring(bin);
		key->numparts=BINgetshort(bin);
		key->fileno=nrdno;
		key->length=0;
		key->part=Rmalloc(sizeof(NRDpart)*key->numparts);
#ifdef USE_BERKELEY_BTREE
		key->dbp=NULL;
#endif /* USE_BERKELEY_BTREE */
		for(y=0,part=key->part;y<key->numparts;++y,++part)
		{
			part->offset=BINgetshort(bin);
			part->name=BINgetstring(bin);
			switch(NRDtmp->db_engine)
			{
				case NRDRdaEngine:
					break;
#ifdef INFORMIX_ISAM
				case NRDInformixIsamEngine:
					break;
#endif
#ifdef USE_MYISAM
				case NRDMyIsamEngine:
					break;
#endif
#ifdef USE_MYSQL
				case NRDMySQLXpertEngine:
					break;
				case NRDMySQLEngine:
				case NRDMySQLAutoEngine:
					break;
#endif
#ifdef USE_BERKELEY_BTREE
				case NRDBerkeleyBtree:
					break;
#endif
#ifdef USE_CTREE
				case NRDCtreeEngine:
				case NRDLocalCtreeEngine:
					break;
#endif
				default:
					break;
			}
		}
	}
	BINfree(bin);
	return(nrdno);
}

short xputnrd(char *module,char *name,short dbengine,short nofields,NRDfield *field,short nokeys,NRDkey *key,char *serverx,int line,char *file)
{
	short y,x;
	NRDpart *segx;
	BIN *bin;
	char *libname=NULL,*dashes=NULL;
	NRDfield *f;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("DIAG putnrd Writing File Definition [%s] [%s] Engine [%d] #Fields [%d] #Keys [%d] for Module [%s] File [%s] at line [%d] program [%s].",module,name,dbengine,nofields,nokeys,module,name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	bin=BINnew();
	BINaddshort(bin,-1699);
	BINaddshort(bin,dbengine);
	if(!isEMPTY(serverx))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,serverx);
	} else BINaddshort(bin,FALSE);
	BINaddshort(bin,nofields);
	BINaddshort(bin,nokeys);
	for(x=0,f=field;x<nofields;++x,++f)
	{
		BINaddstring(bin,f->name);
		BINaddshort(bin,f->type);
		BINaddint(bin,f->len);
	}
	for(x=0;x<nokeys;++x,++key)
	{
		BINaddstring(bin,key->name);
		BINaddshort(bin,key->numparts);
		for(y=0,segx=key->part;y<key->numparts;++y,++segx)
		{
			BINaddshort(bin,segx->offset);
			BINaddstring(bin,segx->name);
		}
	}
	dashes=adddashes(module);
	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(dashes)+10);
#ifndef WIN32
	sprintf(libname,"%s/rda/%s.FIL",CURRENTDIRECTORY,dashes);
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,dashes);
#endif
	if(dashes!=NULL) Rfree(dashes);
	if(writelibbin(libname,bin,name))
	{
		prterr("Error can't write File Definition [%s] [%s] into Binary Library [%s] at line [%d] program [%s].",module,name,libname,line,file);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	if(libname!=NULL) Rfree(libname);
	return(0);
}
