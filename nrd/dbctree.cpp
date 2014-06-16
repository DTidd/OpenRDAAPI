/* dbctree.c - C-tree DataBase Functions */
#include <nrdp.hpp>
char RDA_CollateType=TRUE;

/*lint -e537*/
/*lint +e537*/
#ifdef USE_CTREE
#ifndef LINUX2_2
pCTGVAR ctWNGV=NULL;
#endif
#ifdef LINUX2_2
extern pCTGVAR ctWNGV;
#endif
#endif

char NONTRADITIONAL_EXECUTION=FALSE;
NRDconnect dbcons;
int IBUFFctree=32;
int DBUFFctree=32;
int MAXFILESctree=200;
int NODESECTSctree=16;
#ifdef USE_CTREE
char USE_ODBC=FALSE;
#endif /* USE_CTREE */

char *supported_engines[]={"RDA Database","Informix Isam","MySQL MyIsam","MySQL Server(ODBC)","MySQL Server","Faircom's Ctree","Faircom's Ctree Local","DB Berkeley Btree","DB Berkeley Btree RPC","DB Berkeley Btree / MySQL","MySQL Server (Auto)"};

/************************* RESTRICTION ************************/
/* All Variable Length Records Must be at the end of the file.*/
/**************************************************************/ 
short IsKeyPart(NRDkey *key,char *name)
{
	int x=0;
	NRDpart *part=NULL;

	if(key!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			if(!RDAstrcmp(part->name,name)) return(TRUE);
		}
	}
	return(FALSE);
}
void xSEENRDbuf(int line,char *file)
{
	NRDbuffer *NRDtmp=NULL;
	short fileno=(-1);

	prterr("DIAG Displaying Contents of the NRDbuf at line [%d] program [%s].",line,file);
	if(NRDbuf!=NULL)
	{
		for(fileno=0;fileno<no_buffers;++fileno)
		{
			NRDtmp=NRDbuf+fileno;
			if(NRDtmp->not_used)
			{
				prterr("\t%4d Not Used.\n",fileno);
			} else {
				prterr("\t%4d %s %s.\n",fileno,MODULENAME(fileno),FILENAME(fileno));
			}
		}
	} else {
		prterr("DIAG NRDbuf is NULL.");
	}
	prterr("\n\n");
}
short xDATABASE_ENGINE(short fileno,int line,char *file)
{
	if(!TESTFILENO(fileno,line,file)) return((short)-1);
	return(NRDbuf[fileno].db_engine);
}
short xVARRECORDS(short fileno,int line,char *file)
{
	short x,numflds;
	NRDfield *field,*fields=NULL;

	if(!TESTFILENO(fileno,line,file)) return((short)-1);
	numflds=NUMFLDS(fileno);
	fields=FLDPOINTER(fileno);
	if(fields!=NULL)
	{
		for(x=0,field=fields;x<numflds;++x,++field)
		{
			if(field->type==VARIABLETEXT || field->type==SCROLLEDTEXT || field->len<1) 
				return((short)TRUE);
		}
	}
	return((short)FALSE);
}
void xDUMPfile(short fileno,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field) 
	{ 
		prterr("DIAG DUMPfile Dumping Module [%s] Datafile [%d] [%s] at line [%d] program [%s].",MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return;
	switch(NRDbuf[fileno].db_engine)
	{
		case NRDRdaEngine: 
			DUMPdta(NRDbuf[fileno].dtanum);
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
			prterr("Error Unknown Database Engine [%d] for Database Module [%s] File [%d] [%s] at line [%d] program [%s].",NRDbuf[fileno].db_engine,MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
			break;
	}
}
void xSEENRDFIELD(NRDfield *field,int line,char *file)
{
	prterr("FIELD [%s] Type [%d] [%s] Length [%d] dlen [%d] ",field->name,field->type,fieldtypedesc[field->type],field->len,field->dlen);
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
		case CUSTOMTYPE:
		case EXPENDITURE:
		case REVENUE:
		case BALANCESHEET:
		case BEGINNINGBALANCE:
		case OPTIONALFIELDS:
			prterr("[%s]",(field->data.string_value!=NULL ? field->data.string_value:"NULL"));
			break;
		case BOOLNS:
			prterr("[%d] [%s]",*field->data.string_value,(*field->data.string_value ? "Yes":"No"));
			break;
		case CHARACTERS:
			prterr("[%c]",*field->data.string_value);
			break;
		case DECIMALV:
		case DOUBLEV:
		case SDOUBLEV:
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case SDECIMALV:
			prterr("[%f]",*field->data.float_value);
			break;
		case SHORTV:
		case SSHORTV:
			prterr("[%d]",*field->data.short_value);
			break;
		case LONGV:
		case SLONGV:
			prterr("[%d]",*field->data.integer_value);
			break;
		default:
			prterr(" at line [%d] program [%s]\n",line,file);
			prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
			return;
	}
	prterr(" at line [%d] program [%s]\n",line,file);
}
void xSEENRDRECORD(short fileno,int line,char *file)
{
	short x,numflds;
	NRDfield *field;

	prterr("DIAG SEENRDRECORD Showing Current Record for Module [%s] File [%d] [%s] at line [%d] program [%s].",MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
	if(!TESTFILENO(fileno,line,file)) return;
	numflds=NUMFLDS(fileno);
	for(x=0;x<numflds;++x)
	{
		field=FLDNUM(fileno,x);
		if(field!=NULL)
		{
			SEENRDFIELD(field);
		}
	}
	prterr("END RECORD\n");
}
static NRDbuffer *tTESTFILENO(short fileno,int line,char *file)
{
	NRDbuffer *NRDtmp;
	if(fileno>=no_buffers || fileno<0)
	{
		prterr("Error File Number [%d] Out of Range at line [%d] program [%s].",fileno,line,file);
		return (NRDbuffer *)NULL;
	}
	NRDtmp=NRDbuf+fileno;
	if(NRDtmp->not_used)
	{
		prterr("Error File Number [%d] Not Used at line [%d] program [%s].",fileno,line,file);
		return (NRDbuffer *)NULL;
	}
	return(NRDtmp);
}

short TESTFILENO(short fileno,int line,char *file)
{
	if(tTESTFILENO(fileno,line,file)==NULL) return FALSE;
	return(TRUE);
}
char *xMODULENAME(short fileno,int line,char *file)
{
	NRDbuffer *NRDtmp;
	NRDtmp=tTESTFILENO(fileno,line,file);
	if(NRDtmp==NULL) return(NULL);
	return(NRDtmp->modname);
}
static NRDbuffer *tFILENUMBER(char *module,char *filename)
{
	NRDbuffer *NRDtmp;
	int y;

	for(y=0,NRDtmp=NRDbuf;y<no_buffers;++y,++NRDtmp)
	{
		if(!RDAstrcmp(filename,NRDtmp->name) && !RDAstrcmp(module,NRDtmp->modname)) return(NRDtmp);
	}
	return(NULL);
}
short FILENUMBER(char *module,char *filename)
{
	NRDbuffer *NRDtmp;
	short y;

	for(y=0,NRDtmp=NRDbuf;y<no_buffers;++y,++NRDtmp)
	{
		if(!RDAstrcmp(filename,NRDtmp->name) && !RDAstrcmp(module,NRDtmp->modname)) return(y);
	}
	return(-1);
}
short xKEYNUMBER(short fileno,char *keyname,int line,char *file)
{
	NRDbuffer *NRDtmp;
	NRDkey *key;
	short x;

	NRDtmp=tTESTFILENO(fileno,line,file);
	if(NRDtmp==NULL) return(-1);
	for(x=0,key=NRDtmp->key;x<NRDtmp->nokeys;++x,++key)
	{
		if(!RDAstrcmp(key->name,keyname)) return(++x);
	}
	return(-1);
}
short xKEYNUMBERBYNAME(char *module,char *filename,char *keyname,
	int line,char *file)
{
	NRDbuffer *NRDtmp;
	NRDkey *key;
	short x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("DIAG KEYNUMBERBYNAME Returning Key Number for Module [%s] Filename [%s] Keyname [%s] at line [%d] program [%s].",module,filename,keyname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=tFILENUMBER(module,filename);
	if(NRDtmp==NULL) return(-1);
	for(x=0,key=NRDtmp->key;x<NRDtmp->nokeys;++x,++key)
	{
		if(!RDAstrcmp(key->name,keyname)) return(++x);
	}
	return(-1);
}
char *xFILENAME(short fileno,int line,char *file)
{
	NRDbuffer *NRDtmp;

	NRDtmp=tTESTFILENO(fileno,line,file);
	if(NRDtmp==NULL) return(NULL);
	return(NRDtmp->name);
}
NRDkey *xKEYNUM(short fileno,short keyno,int line,char *file)
{
	NRDbuffer *NRDtmp;

	NRDtmp=tTESTFILENO(fileno,line,file);
	if(NRDtmp==NULL) return(NULL);
	if((NRDtmp->nokeys+1)<keyno || keyno<1) 
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s] at line [%d] program [%s].",keyno,MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
		return(NULL);
	}
	return(NRDtmp->key+(keyno-1));
}
char *xKEYNAME(short fileno,short keyno,int line,char *file)
{
	NRDkey *key;

	key=xKEYNUM(fileno,keyno,line,file);
	return(key->name);
}
NRDfield *xFLDNUM(short fileno,short fieldno,int line,char *file)
{
	NRDfield *field;
	NRDbuffer *NRDtmp;

	NRDtmp=tTESTFILENO(fileno,line,file);
	if(NRDtmp==NULL) return(NULL);

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("DIAG FLDNUM Find Field [%d] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldno,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(fieldno<0 || fieldno>NRDtmp->nofields)
	{
		prterr("Error Field Number [%d] is Out of Bounds for Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldno,MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
		return(NULL);
	}
	field=NRDtmp->field+fieldno;
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{ 
		prterr("DIAG FLDNUM Returning Field [%s] Type [%d] Length [%d] in Module [%s] File [%d] [%s] to line [%d] program [%s].",field->name,field->type,field->len,NRDbuf[fileno].modname,fileno,NRDbuf[fileno].name,line,file); 
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(field);
}
short get_length(NRDbuffer *NRDtmp,char *fieldname)
{
	short x;
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG get_lenth Getting Length of Field [%s] from Module [%s] File [%s].",fieldname,NRDtmp->modname,NRDtmp->name); }
#endif /* USE_RDA_DIAGNOSTICS */
	if(NRDtmp->field!=NULL)
	{
		for(x=0,field=NRDtmp->field+x;x<NRDtmp->nofields;++x,++field)
		{
			if(!RDAstrcmp(field->name,fieldname)) return field->len;
		}
	}
	return(0);
}
short xNUMFLDS(short fileno,int line,char *file)
{
	NRDbuffer *NRDtmp;

	NRDtmp=tTESTFILENO(fileno,line,file);
	if(NRDtmp==NULL) return(-1);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field) { prterr("DIAG NUMFLDS Getting Number of Fields [%d] in Module [%s] File [%d] [%s] at line [%d] program [%s].",NRDtmp->nofields,NRDtmp->modname,fileno,NRDtmp->name,line,file); }
#endif /* USE_RDA_DIAGNOSTICS */
	return(NRDtmp->nofields);
}
NRDfield *xFINDFLD(NRDbuffer *NRDtmp,char *fieldname,int line,char *file)
{
	short x;
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("DIAG FINDFLD Find Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",fieldname,NRDtmp->modname,NRDtmp->name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,field=NRDtmp->field;x<NRDtmp->nofields;++x,++field)
	{
		if(!RDAstrcmp(field->name,fieldname))
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diagnrd || diagnrd_field)
			{ 
				prterr("DIAG FINDFLD Returning Field [%s] Type [%d] [%s] Length [%d] in Module [%s] File [%s] to line [%d] program [%s].",field->name,field->type,fieldtypedesc[field->type],field->len,NRDtmp->modname,NRDtmp->name,line,file);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			return(field);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("Error Can't Find Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",fieldname,NRDtmp->modname,NRDtmp->name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(NULL);
}
NRDfield *xFLDNRD(short fileno,char *fieldname,int line,char *file)
{
	NRDbuffer *NRDtmp;

	NRDtmp=tTESTFILENO(fileno,line,file);
	if(NRDtmp==NULL) return(NULL);
	return(xFINDFLD(NRDtmp,fieldname,line,file));
}
short xNUMKEYS(short fileno,int line,char *file)
{
	NRDbuffer *NRDtmp;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("DIAG NUMKEYS Returning the Number of Keys Available for Module [%s] File [%d] [%s] at line [%d] program [%s].",MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=tTESTFILENO(fileno,line,file);
	if(NRDtmp==NULL) return(-1);
	return(NRDtmp->nokeys);
}
NRDkey *xKEYPOINTER(short fileno,int line,char *file)
{
	NRDbuffer *NRDtmp;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG KEYPOINTER Returning a pointer to the Key Structures for of Module [%s] File [%d] [%s] at line [%d] program [%s].",MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=tTESTFILENO(fileno,line,file);
	if(NRDtmp==NULL) return(NULL);
	return(NRDtmp->key);
}
NRDfield *xFLDPOINTER(short fileno,int line,char *file)
{
	NRDbuffer *NRDtmp;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG FLDPOINTER Returning a pointer to the array of fields of Module [%s] File [%d] [%s] at line [%d] program [%s].",MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=tTESTFILENO(fileno,line,file);
	if(NRDtmp==NULL) return(NULL);
	return(NRDtmp->field);
}
void xFreeNRDfieldvalue(NRDfield *field,int line,char *file)
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
		case CUSTOMTYPE:
		case EXPENDITURE:
		case REVENUE:
		case BALANCESHEET:
		case BEGINNINGBALANCE:
		case OPTIONALFIELDS:
		case CHARACTERS:
		case BOOLNS:
			if(field->data.string_value!=NULL) Rfree(field->data.string_value);
			break;
		case DECIMALV:
		case DOUBLEV:
		case SDOUBLEV:
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case SDECIMALV:
			if(field->data.float_value!=NULL) Rfree(field->data.float_value);
			break;
		case SHORTV:
		case SSHORTV:
			if(field->data.short_value!=NULL) Rfree(field->data.short_value);
			break;
		case SCROLLEDLIST:
		case LONGV:
		case SLONGV:
			if(field->data.integer_value!=NULL) Rfree(field->data.integer_value);
			break;
		default:
			prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
			break;
	}
	field->dlen=0;
}
void xFreeNRDfield(NRDfield *field,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG FreeNRDfield at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	Rfree(field->name);
	FreeNRDfieldvalue(field);
}
void xfreenrd(NRDbuffer *NRDtmp,int line,char *file)
{
	short x,y;
	NRDfield *field;
	NRDkey *key;
	NRDpart *p;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{ 
		prterr("DIAG freenrd Freeing Database Engine for Module [%s] File [%s] at line [%d] program [%s].",(NRDtmp->modname!=NULL?NRDtmp->modname:""),(NRDtmp->name!=NULL?NRDtmp->name:""),line,file); 
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(NRDtmp->key!=NULL)
	{
		for(x=0,key=NRDtmp->key+x;x<NRDtmp->nokeys;++x,++key)
		{
#ifdef USE_MYISAM
	/*
			if(key->target!=NULL) Rfree(key->target);
		*/
#endif
			if(key->part!=NULL)
			{
				for(y=0,p=key->part;y<key->numparts;++y,++p)
				{
					if(p->name!=NULL) Rfree(p->name);
				}
				Rfree(key->part);
			}
			Rfree(key->name);
		}
		Rfree(NRDtmp->key);
		NRDtmp->key=NULL;
		NRDtmp->nokeys=0;
	}
	if(NRDtmp->field!=NULL)
	{
		for(x=0,field=NRDtmp->field+x;x<NRDtmp->nofields;++x,++field) 
			FreeNRDfield(field);
		Rfree(NRDtmp->field);
		NRDtmp->field=NULL;
		NRDtmp->nofields=0;
	}
#ifdef USE_MYISAM
	NRDtmp->myinfo=NULL;
	if(NRDtmp->record_data2!=NULL) Rfree(NRDtmp->record_data2);
	NRDtmp->dlen2=0;
	if(NRDtmp->target!=NULL) Rfree(NRDtmp->target);
	NRDtmp->tlen=0;
#endif
	if(NRDtmp->name!=NULL) Rfree(NRDtmp->name);
	NRDtmp->name=NULL;
	if(NRDtmp->modname!=NULL) Rfree(NRDtmp->modname);
	if(NRDtmp->servername!=NULL) Rfree(NRDtmp->servername);
	NRDtmp->modname=NULL;
	if(NRDtmp->record_data!=NULL) Rfree(NRDtmp->record_data);
	NRDtmp->dlen=0;
	NRDtmp->record_data=NULL;
	NRDtmp->not_used=1;/*TRUE*/
	NRDtmp->dtanum=NULL;
#ifdef USE_BERKELEY_BTREE
	NRDtmp->dbp=NULL;
#endif
#ifdef USE_MYISAM
	NRDtmp->dlen=0;
	NRDtmp->record_data=NULL;
#endif 
	NRDtmp->currec=0;
	NRDtmp->filenum=(-1);
	NRDtmp->db_engine=0;	
}
#ifdef USE_CTREE
void EXITNRDctree()
{
	ULONG ct[8];

	if(RDA_GetEnv("CTREE_IOPERFORMANCE")!=NULL)
	{
		IOPERFORMANCE(ct);
		prterr("DIAG Faircom Server IOPERFORMANCE Statics.");
		prterr("\tData Buffer Requests  =[%ld]\n",ct[0]);
		prterr("\tData Buffer Hits      =[%ld]\n",ct[1]);
		prterr("\tIndex Buffer Requests =[%ld]\n",ct[2]);
		prterr("\tIndex Buffer Hits     =[%ld]\n",ct[3]);
		prterr("\tNumber Read Ops       =[%ld]\n",ct[4]);
		prterr("\tNumber Bytes Read     =[%ld]\n",ct[5]);
		prterr("\tNumber of Write Ops   =[%ld]\n",ct[6]);
		prterr("\tNumber Bytes Written  =[%ld]\n",ct[7]);
	}
}
#endif
#ifdef INFORMIX_ISAM
#ifdef UNIXWARE7
extern int iscleanup(void);
#endif
void EXITNRDisam()
{
	short x=0;

	x=iscleanup();
	if(x==(-1))
	{
		prterr("Error [%d] EXITNRDisam.",iserrno); 
	}
}
#endif
void xEXITNRD(int line,char *filx)
{
	NRDbuffer *NRDtmp;
	int x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG EXITNRD Shutting down the Database Engine at line [%d] program [%s].",line,filx);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	FreeNRDconnect();
	if(NRDbuf!=NULL)
	{
		for(x=0;x<no_buffers;++x) 
		{
			NRDtmp=NRDbuf+x;
			freenrd(NRDtmp);
		}
		Rfree(NRDbuf);
		NRDbuf=NULL;
		no_buffers=0;
	}
#ifdef USE_BERKELEY_BTREE
	EXITNRDberkeley();
#endif
#ifdef USE_CTREE
	EXITNRDctree();
#endif
#ifdef INFORMIX_ISAM
	EXITNRDisam();
#endif
#ifdef USE_MYSQL
	EXITNRDmysql();
#endif
}
short xINITNRD(int line,char *filx)
{
	short retval=FALSE;
	char *temp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG INITNRD Initializing Database Engine at line [%d] program [%s].",line,filx); }
#endif /* USE_RDA_DIAGNOSTICS */
	if(CURRENTDIRECTORY==NULL) SETCWD();
	if(USERLOGIN==NULL) SETUSERLOGIN();
	temp=RDA_GetEnv("XPERT_CDROM_EXECUTION");
	if(!isEMPTY(temp)) NONTRADITIONAL_EXECUTION=TRUE;
	temp=RDA_GetEnv("USERAMDISK");
	if(!isEMPTY(temp)) UseRamDisk=TRUE;
	if(!AlreadyInitialized)
	{
		NRDbuf=NULL;	
		no_buffers=0;
		dbcons.s=NULL;
		dbcons.num=0;
#ifdef USE_BERKELEY_BTREE
		retval=INITNRDberkeley();
		if(retval)
		{
			prterr("Error INITNRDberkeley Failed to Initialize the Berkeley Btree Engine.");
		}
#endif
#ifdef USE_MYISAM
		INITNRDmyisam();
#endif
#ifdef USE_MYSQL
		INITNRDmysql();
#endif
#ifdef USE_CTREE
#ifdef XXXXX
		ctWNGV=NULL;
#endif
		retval=INITNRDctree();
		if(retval) 
		{
			prterr("Error INITNRDctree Failed to Initialize the Faircom Engines.");
		}
#endif
#ifdef INFORMIX_ISAM
		retval=INITNRDisam();
		if(retval) 
		{
			prterr("Error INITNRDisam Failed to Initialize the Informix Engines.");
		}
#endif
	}
	return(retval);
}
#ifdef USE_CTREE
short INITNRDctree()
{
	char *tmp=NULL,*ostype=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG Initializing C-tree Isam Files."); }
#endif /* USE_RDA_DIAGNOSTICS */
	tmp=RDA_GetEnv("USE_ODBC");
	if(tmp!=NULL)
	{
		stoupper(tmp);
		if(!RDAstrcmp(tmp,"FALSE")) USE_ODBC=FALSE;
		else USE_ODBC=TRUE;
	} else  USE_ODBC=FALSE;
	tmp=RDA_GetEnv("IDXBUFF_CTREE");
#ifdef WIN32
	ostype=__OSTYPE__;
#endif
	if(tmp!=NULL)
	{
		IBUFFctree=Ratoi(tmp);
		if(IBUFFctree<3) 
		{
			IBUFFctree=32;
		}
	} else {
		if(RDAstrcmp(ostype,"MSWINDOWS_98"))
		{
			IBUFFctree=32;
		} else IBUFFctree=32;
	}
	tmp=NULL;
	tmp=RDA_GetEnv("DATBUFF_CTREE");
	if(tmp!=NULL)
	{
		DBUFFctree=Ratoi(tmp);
		if(DBUFFctree<3) 
		{
			DBUFFctree=32;
		}
	} else {
		if(RDAstrcmp(ostype,"MSWINDOWS_98"))
		{
			DBUFFctree=32;
		} else DBUFFctree=32;
	}
	tmp=NULL;
	tmp=RDA_GetEnv("MAXFILES_CTREE");
	if(tmp!=NULL)
	{
		MAXFILESctree=Ratoi(tmp);
		if(MAXFILESctree<1) 
		{
			MAXFILESctree=200;
		}
	} else {
		if(RDAstrcmp(ostype,"MSWINDOWS_98"))
		{
			MAXFILESctree=200;
		} else MAXFILESctree=150; /* 180 */
	}
	tmp=NULL;
	tmp=RDA_GetEnv("NODESECTS_CTREE");
	if(tmp!=NULL)
	{
		NODESECTSctree=Ratoi(tmp);
		if(NODESECTSctree<1) 
		{
			NODESECTSctree=32;
		}
	} else {
		NODESECTSctree=32;
	}
	return(0);
}
#endif
short xEQLNRDKEY(short fileno,short keyno,void *target,int line,char *file)
{
	long recno=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field) 
	{
		prterr("DIAG EQLNRDKEY Searching for Equal Key in Module [%s] File [%d] [%s] with Keyno [%d] at line [%d] file [%s].",MODULENAME(fileno),fileno,NRDbuf[fileno].name,keyno,line,file); 
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	switch(NRDbuf[fileno].db_engine)
	{
		case NRDRdaEngine:	
			recno=*(long *)target;
			if(recno!=0) return(EQLNRDKEYrda(fileno,target));
				else return(-1);
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine: return(EQLNRDKEYisam(fileno,keyno,target));
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine:
			    return(EQLNRDKEYmyisam(fileno,keyno,target));
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine:
		case NRDMySQLEngine: 
		case NRDMySQLAutoEngine:
			    return(EQLNRDKEYmysql(fileno,keyno,target));
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree: return(EQLNRDKEYberkeley(fileno,keyno,target));
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine: return(EQLNRDKEYctree(fileno,keyno,target));
		case NRDLocalCtreeEngine: return(EQLNRDKEYctree(fileno,keyno,target));
#endif
		default:
			prterr("Error Unknown Database Engine [%d] for Module [%s] File [%d] [%s] at line [%d] program [%s].",NRDbuf[fileno].db_engine,NRDbuf[fileno].modname,fileno,FILENAME(fileno),line,file);
			return(-1);
	}
}
short EQLNRDKEYrda(short fileno,void *target)
{
	short return_value=0;
	NRDbuffer *NRDtmp;
	DTA y;

	NRDtmp=NRDbuf+fileno;
	return_value=READDTA(NRDtmp->dtanum,*(long *)target);
	y=NRDtmp->dtanum;
	if(!return_value) 
	{
		NRDtmp->currec=y->cur;
		assign_rdafields(NRDtmp);
	}
	return(return_value);
}
short xEQLNRD(short fileno,short keyno,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field) { prterr("DIAG EQLNRD Searching for Equal Key in Module [%s] File [%d] [%s] with Keyno [%d] at line [%d] file [%s].",MODULENAME(fileno),fileno,NRDbuf[fileno].name,keyno,line,file); }
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	switch(NRDbuf[fileno].db_engine)
	{
		case NRDRdaEngine: return(EQLNRDrda(fileno,keyno));
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:	return(EQLNRDisam(fileno,keyno));
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine:
			  return(EQLNRDmyisam(fileno,keyno));
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine:
		case NRDMySQLEngine: 
		case NRDMySQLAutoEngine:
			  return(EQLNRDmysql(fileno,keyno));
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree: return(EQLNRDberkeley(fileno,keyno));
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine: return(EQLNRDctree(fileno,keyno));
		case NRDLocalCtreeEngine: return(EQLNRDctree(fileno,keyno));
#endif
		default:
			prterr("Error Unknown Database Engine [%d] for Module [%s] File [%s] at line [%d] program [%s].",NRDbuf[fileno].db_engine,NRDbuf[fileno].modname,NRDbuf[fileno].name,line,file);
			return(-1);
	}
	if(diagnrd)
	{
		SEENRDRECORD(fileno);TRACE;
	}
}
void xsetrdakeys(short fileno,short keyno,int line,char *file)
{
	short x,y;
	NRDkey *key;
	NRDpart *part;
	NRDfield *field;
	NRDbuffer *NRDtmp;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG setrdakeys Setting Keys for RDA Database Module [%s] File [%d] [%s] using Keyno [%d] at line [%d] program [%s].",MODULENAME(fileno),fileno,FILENAME(fileno),keyno,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	key=NRDtmp->key+(keyno-1);
	for(x=0,part=key->part;x<key->numparts;++x,++part)
	{
		for(y=0,field=NRDtmp->field;y<NRDtmp->nofields;++y,++field)
		{
			if(!RDAstrcmp(part->name,field->name)) break;
		}
		if(y<NRDtmp->nofields)
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
				case CUSTOMTYPE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case CHARACTERS:
				case BOOLNS:
					SETDTA(NRDbuf[fileno].dtanum,y,field->data.string_value);
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					SETDTA(NRDbuf[fileno].dtanum,y,field->data.float_value);
					break;
				case SHORTV:
				case SSHORTV:
					SETDTA(NRDbuf[fileno].dtanum,y,field->data.short_value);
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					SETDTA(NRDbuf[fileno].dtanum,y,field->data.integer_value);
					break;
				default:
					prterr("Error Field Type [%d] is invalid for Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",field->type,field->name,NRDbuf[fileno].modname,NRDbuf[fileno].name,line,file); 
					break;
			}
		} else {
			prterr("Error Field [%s] Not Found in Module [%s] File [%s] at line [%d] program [%s].",part->name,NRDbuf[fileno].modname,NRDbuf[fileno].name,line,file); 
		}
	}
}
short EQLNRDrda(short fileno,short keyno)
{
	short return_value=0;
	DTA y;
	KEY x;
	NRDbuffer *NRDtmp;

	NRDtmp=NRDbuf+fileno;
	setrdakeys(fileno,keyno);
	y=NRDtmp->dtanum;
	x=y->key+(keyno-1);
	return_value=EQLDTA(x);
	if(!return_value) 
	{
		NRDtmp->currec=y->cur;
		assign_rdafields(NRDtmp);
	}
	return(return_value);
}
void *xNRDtmp2target(NRDbuffer *NRDtmp,NRDkey *key,short *pos,int line,char *file)
{
	short x,lf=0;
	char *target=NULL;
	NRDpart *part;
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) 
	{
		prterr("DIAG NRDtmp2target at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	*pos=0;
#ifdef USE_MYISAM
	if(NRDtmp->db_engine==NRDMyIsamEngine)
	{
		MyIsam2target(NRDtmp,key,pos);
		target=Rmalloc(*pos+1);
		memcpy(target,NRDtmp->target,*pos);
	} else {
#endif /* USE_MYISAM */
		for(x=0;x<key->numparts;++x)
		{
			part=key->part+x;
			field=FINDFLD(NRDtmp,part->name);
			if(field!=NULL)
			{
/* increment allocation space for target by field length */
/* write field data into target */
				switch(field->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT: /* variable length field type */
						if(field->len>0)
						{
							unpad(field->data.string_value);
							if(target!=NULL)
							{
								target=Rrealloc(target,*pos+field->len+3);
							} else target=Rmalloc(field->len+2);
							sprintf(&target[*pos],"%-*s",field->len,(field->data.string_value!=NULL ? field->data.string_value:""));
							*pos+=field->len;
						} else {
							if(field->data.string_value!=NULL)
							{
								unpad(field->data.string_value);
								lf=RDAstrlen(field->data.string_value);
								target=Rrealloc(target,*pos+lf+3);
							} else {
								target=Rrealloc(target,*pos+3);
								lf=0;
							}
							sprintf(&target[*pos],"%s",(field->data.string_value!=NULL ? field->data.string_value:""));
							if(field->data.string_value!=NULL)
							{
								*pos+=lf;
							}
						}
						target[*pos]=0;
						break;
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
						if(target!=NULL)
						{
							target=Rrealloc(target,*pos+field->len+2);
						} else target=Rmalloc(field->len+1);
						unpad(field->data.string_value);
						sprintf(&target[*pos],"%-*s",field->len,(field->data.string_value!=NULL ? field->data.string_value:""));
						*pos+=field->len;
						target[*pos]=0;
						break;
					case CHARACTERS:
						if(target!=NULL)
						{
							target=Rrealloc(target,*pos+field->len);
						} else target=Rmalloc(field->len);
						memcpy(&target[*pos],field->data.string_value,1);
						*pos+=1;
						break;
					case BOOLNS:
						if(target!=NULL)
						{
							target=Rrealloc(target,*pos+field->len);
						} else target=Rmalloc(field->len);
						if(*field->data.string_value==TRUE) target[*pos]='1';
							else target[*pos]='0';
						*pos+=1;
						break;
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case SDECIMALV:
						if(target!=NULL)
						{
							target=Rrealloc(target,*pos+sizeof(double)+1);
						} else target=Rmalloc(sizeof(double));
						memcpy(&target[*pos],field->data.float_value,sizeof(double));
						*pos+=sizeof(double);
						break;
					case SHORTV:
					case SSHORTV:
						if(target!=NULL)
						{
							target=Rrealloc(target,*pos+sizeof(short)+1);
						} else target=Rmalloc(sizeof(short));
						memcpy(&target[*pos],field->data.short_value,sizeof(short));
						*pos+=sizeof(short);
						break;
					case LONGV:
					case SLONGV:
						if(target!=NULL)
						{
							target=Rrealloc(target,*pos+sizeof(int)+1);
						} else target=Rmalloc(sizeof(int));
						memcpy(&target[*pos],field->data.integer_value,sizeof(int));
						*pos+=sizeof(int);
						break;
					default:
						break;
				}
			}
		}
#ifdef USE_MYISAM
	}
#endif /* USE_MYISAM */
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(target);
}
void *xmaketarget(short fileno,short keyno,short *pos,int line,char *file)
{
	NRDkey *key;
	NRDbuffer *NRDtmp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) 
	{
		prterr("DIAG maketarget Creating Target for Current Record in Module [%s] File [%d] [%s] Using Keyno [%d] at line [%d] program [%s].",MODULENAME(fileno),fileno,FILENAME(fileno),keyno,line,file); 
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(NULL);
	NRDtmp=NRDbuf+fileno;
	if(keyno<1) keyno=1;
	key=NRDtmp->key+(keyno-1);
	*pos=0;
	return(xNRDtmp2target(NRDtmp,key,pos,line,file));
}
void assign_rdafields(NRDbuffer *NRDtmp)
{
	short x;
	unsigned hold_length=0;
	NRDfield *field;
	void *data=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) 
	{ 
		prterr("DIAG assign_rdafields Assigning Current RDA Database Record for Module [%s] File [%s] to Database Field Structures.",NRDtmp->modname,NRDtmp->name); 
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,field=NRDtmp->field;x<NRDtmp->nofields;++x,++field)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagnrd)
		{
			prterr("DIAG Database Field Number [%d] Name [%s] Type [%d] [%s] Length [%d].",x,field->name,field->type,fieldtypedesc[field->type],field->len);
		}
#endif /* USE_RDA_DIAGNOSTICS */
		data=DATDTA(NRDtmp->dtanum,x);
		if(data!=NULL)
		{
			switch(field->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT: 
				case PLAINTEXT:
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
					if(!isEMPTY((char *)data))
					{
						hold_length=RDAstrlen((char *)data);
						QUICKALLOC(field->data.string_value,field->dlen,hold_length+1);
						memcpy(field->data.string_value,(char *)data,
							hold_length+1);
						if(field->len>0 && field->dlen>field->len)
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
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					if(field->data.float_value==NULL)
					{
						field->data.float_value=Rmalloc(sizeof(double));
						field->dlen=sizeof(double);
					}
					*field->data.float_value=*(double *)data;
					break;
				case SHORTV:
				case SSHORTV:
					if(field->data.short_value==NULL)
					{
						field->data.short_value=Rmalloc(sizeof(short));
						field->dlen=sizeof(short);
					}
					*field->data.short_value=*(short *)data;
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					if(field->data.integer_value==NULL)
					{
						field->data.integer_value=Rmalloc(sizeof(int));
						field->dlen=sizeof(int);
					}
					*field->data.integer_value=*(int *)data;
					break;
				case BOOLNS:
				case CHARACTERS:
					if(field->data.string_value==NULL)
					{
						field->data.string_value=Rmalloc(1);
						field->dlen=1;
					}
					*field->data.string_value=*(char *)data;
					break;
				default:
					prterr("Error Field Type [%d] is invalid in Field [%s] in Module [%s] File [%s].",field->type,field->name,NRDtmp->modname,NRDtmp->name);
					break;
			}
		} else {
			prterr("Error DATDTA RDA Database Returning Field Number [%d] Field Name [%s] in Module [%s] File [%s] is out of bounds.",x,field->name,NRDtmp->modname,NRDtmp->name);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG assign_rdafields All Fields Assigned for Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
}
#if (defined(USE_CTREE) || defined(INFORMIX_ISAM))
#endif
#ifdef USE_CTREE
short EQLNRDKEYctree(short fileno,short keyno,void *target)
{
	short ctree_keyno;
	VRLEN varlen;
	NRDbuffer *NRDtmp;
	LONG return_value=0;
	short tlen=0;
	void *tmptarget=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG EQLNRDKEYctree Searching for Equal Key in Module [%s] File [%d] [%s] Keyno [%d] using C-tree Database Engine.",MODULENAME(fileno),fileno,FILENAME(fileno),keyno);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	RecordNRDconnect(NRDtmp->servername,NRDtmp->db_engine);
	if((NRDbuf[fileno].nokeys+1)<keyno || keyno<1) 
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s].  Using Keyno [1] instead.",keyno,MODULENAME(fileno),fileno,FILENAME(fileno));
		keyno=1;
	}
	ctree_keyno=NRDtmp->filenum+keyno;
	tmptarget=maketarget(fileno,keyno,&tlen);
	if(target==NULL)
	{
		target=Rmalloc(tlen+1);
	}
	memcpy(tmptarget,target,tlen);
	return_value=EQLREC(ctree_keyno,tmptarget,NRDtmp->record_data);
	if(tmptarget!=NULL) Rfree(tmptarget);
	if(!return_value)
	{
		if(NRDtmp->varfile==TRUE)
		{
			varlen=GETVLEN(NRDtmp->filenum);
			QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,varlen+1);
			return_value=REDVREC(NRDtmp->filenum,
				NRDtmp->record_data,varlen);
		}
		assignCtree_fields(NRDtmp);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagnrd) { xSEENRDRECORD(fileno,__LINE__,__FILE__);prterr("Exiting EQLNRDctree OK for Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name); }
#endif /* USE_RDA_DIAGNOSTICS */
		return(0);
	}
	if(return_value!=101)
	{
		prterr("Error in C-tree Database [%s] for Module [%s] File [%s].",ctdb_errors(errno),NRDtmp->modname,NRDtmp->name);
		return(-1);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG Specified Key Value not found in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(1);
}
short EQLNRDctree(short fileno,short keyno)
{
	short return_value=0;
	NRDbuffer *NRDtmp;
	void *target=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG EQLNRDctree Searching for equal key  with C-tree D-Base Engine."); }
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	RecordNRDconnect(NRDtmp->servername,NRDtmp->db_engine);
	if((NRDtmp->nokeys+1)<keyno || keyno<1)
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s].  Using Keyno [1] instead.",keyno,MODULENAME(fileno),fileno,FILENAME(fileno));
		keyno=1;
	}
	target=maketarget(fileno,keyno,&return_value);
	return_value=EQLNRDKEYctree(fileno,keyno,target);
	if(target!=NULL) Rfree(target);
	return(return_value);
}
#endif
short xGTENRD(short fileno,short keyno,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field) 
	{ 
		prterr("DIAG GTENRD Get Record Greater Than or Equal to in Module [%s] File [%d] [%s] Keyno [%d] at line [%d] program [%s].",MODULENAME(fileno),fileno,FILENAME(fileno),keyno,line,file); 
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	switch(NRDbuf[fileno].db_engine)
	{
		case NRDRdaEngine:	return(GTENRDrda(fileno,keyno));
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:	return(GTENRDisam(fileno,keyno));
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine:
			return(GTENRDmyisam(fileno,keyno));
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine:
		case NRDMySQLEngine:
		case NRDMySQLAutoEngine:
			return(GTENRDmysql(fileno,keyno));
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree:		return(GTENRDberkeley(fileno,keyno));
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine:	return(GTENRDctree(fileno,keyno));
		case NRDLocalCtreeEngine:	return(GTENRDctree(fileno,keyno));
#endif
		default:
			prterr("Error Unknown Database Engine [%d] for Module [%s] File [%d] [%s] at line [%d] program [%s]",NRDbuf[fileno].db_engine,MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
			return(-1);
	}
}
short GTENRDrda(short fileno,short keyno)
{
	short return_value=0;
	NRDbuffer *NRDtmp;
	DTA y;
	KEY x;

	setrdakeys(fileno,keyno);
	NRDtmp=NRDbuf+fileno;
	y=NRDtmp->dtanum;
	x=y->key+(keyno-1);
	return_value=GTEDTA(x);
	if(!return_value) 
	{
		NRDtmp->currec=y->cur;
		assign_rdafields(NRDtmp);
	}
	return(return_value);
}
short xPRVNRD(short fileno,short keyno,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field) 
	{ 
		prterr("DIAG PRVNRD Get Record Previous to Equal to in Module [%s] File [%d] [%s] Keyno [%d] at line [%d] program [%s].",MODULENAME(fileno),fileno,FILENAME(fileno),keyno,line,file); 
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	switch(NRDbuf[fileno].db_engine)
	{
		case NRDRdaEngine:	return(PRVNRDrda(fileno,keyno));
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:	return(PRVNRDisam(fileno,keyno));
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine: 
			return(PRVNRDmyisam(fileno,keyno));
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine:
		case NRDMySQLEngine:
		case NRDMySQLAutoEngine:
			return(PRVNRDmysql(fileno,keyno));
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree:		return(PRVNRDberkeley(fileno,keyno));
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine:	return(PRVNRDctree(fileno,keyno));
		case NRDLocalCtreeEngine:	return(PRVNRDctree(fileno,keyno));
#endif
		default:
			prterr("Error Unknown Database Engine [%d] for Module [%s] File [%d] [%s] at line [%d] program [%s]",NRDbuf[fileno].db_engine,MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
			return(-1);
	}
}
short PRVNRDrda(short fileno,short keyno)
{
	short return_value=0;
	NRDbuffer *NRDtmp;
	DTA y;
	KEY x;

	setrdakeys(fileno,keyno);
	NRDtmp=NRDbuf+fileno;
	y=NRDtmp->dtanum;
	x=y->key+(keyno-1);
	return_value=PRVDTA(x);
	if(!return_value) 
	{
		NRDtmp->currec=y->cur;
		assign_rdafields(NRDtmp);
	}
	return(return_value);
}
#ifdef USE_CTREE
short PRVNRDctree(short fileno,short keyno)
{
	short ctree_keyno;
	short x=0;
	VRLEN varlen;
	NRDbuffer *NRDtmp;
	LONG return_value=0;
	void *target=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG Searching for last key with C-tree engine for file [%d] [%s] keyno [%d].",fileno,FILENAME(fileno),keyno); }
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	RecordNRDconnect(NRDtmp->servername,NRDtmp->db_engine);
/* check key exists, if not use 1st key. All indexes have 1 key */
	if(NRDtmp->record_data==NULL)
	{
		varlen=getdreclen(NRDtmp);
		QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,varlen);
	}
	if((NRDbuf[fileno].nokeys+1)<keyno || keyno<1)
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s].  Using Keyno [1] instead.",keyno,MODULENAME(fileno),fileno,FILENAME(fileno));
		keyno=1;
	}
	ctree_keyno=NRDtmp->filenum+keyno;
	assignCTREERecordData(NRDtmp,FALSE);
	target=maketarget(fileno,keyno,&x);
/*
	return_value=EQLREC(ctree_keyno,target,NRDtmp->record_data);
*/
	return_value=LTREC(ctree_keyno,target,NRDtmp->record_data);
	if(target!=NULL) Rfree(target);
/*
	if(return_value)
	{
		return_value=LSTREC(ctree_keyno,NRDtmp->record_data);
	} else {
		return_value=PRVREC(ctree_keyno,NRDtmp->record_data);
	}
*/
	if(!return_value)
	{
		if(NRDtmp->varfile==TRUE)
		{
			varlen=GETVLEN(NRDtmp->filenum);
			QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,varlen+1);
			memset(NRDtmp->record_data,0,NRDtmp->dlen);
			return_value=REDVREC(NRDtmp->filenum,
				NRDtmp->record_data,varlen);
		}
		assignCtree_fields(NRDtmp);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagnrd) { xSEENRDRECORD(fileno,__LINE__,__FILE__);prterr("Exiting PRVNRDctree OK for Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name); }
#endif /* USE_RDA_DIAGNOSTICS */
		return(0);
	}
	if(return_value!=101)
	{
		prterr("Error in C-tree Database [%s] for Module [%s] File [%s].",ctdb_errors(errno),NRDtmp->modname,NRDtmp->name);
		return(-1);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG Specified Key Value not found in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(1);
}
#endif
short xLSTNRD(short fileno,short keyno,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field) 
	{ 
		prterr("DIAG LSTNRD Get Record Less Than Equal to in Module [%s] File [%d] [%s] Keyno [%d] at line [%d] program [%s].",MODULENAME(fileno),fileno,FILENAME(fileno),keyno,line,file); 
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	switch(NRDbuf[fileno].db_engine)
	{
		case NRDRdaEngine:	return(LSTNRDrda(fileno,keyno));
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:	return(LSTNRDisam(fileno,keyno));
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine: 
			return(LSTNRDmyisam(fileno,keyno));
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine:
		case NRDMySQLEngine:
		case NRDMySQLAutoEngine:
			return(LSTNRDmysql(fileno,keyno));
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree:		return(LSTNRDberkeley(fileno,keyno));
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine:	return(LSTNRDctree(fileno,keyno));
		case NRDLocalCtreeEngine:	return(LSTNRDctree(fileno,keyno));
#endif
		default:
			prterr("Error Unknown Database Engine [%d] for Module [%s] File [%d] [%s] at line [%d] program [%s]",NRDbuf[fileno].db_engine,MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
			return(-1);
	}
}
short LSTNRDrda(short fileno,short keyno)
{
	short return_value=0;
	NRDbuffer *NRDtmp;
	DTA y;
	KEY x;

	setrdakeys(fileno,keyno);
	NRDtmp=NRDbuf+fileno;
	y=NRDtmp->dtanum;
	x=y->key+(keyno-1);
	return_value=LSTDTA(x);
	if(!return_value) 
	{
		NRDtmp->currec=y->cur;
		assign_rdafields(NRDtmp);
	}
	return(return_value);
}
#ifdef USE_CTREE
short LSTNRDctree(short fileno,short keyno)
{
	short ctree_keyno;
	VRLEN varlen;
	NRDbuffer *NRDtmp;
	LONG return_value=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG Searching for last key with C-tree engine for file [%d] [%s] keyno [%d].",fileno,FILENAME(fileno),keyno); }
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	RecordNRDconnect(NRDtmp->servername,NRDtmp->db_engine);
	SETCURI(NRDtmp->filenum,return_value,NULL,0);
	if((NRDbuf[fileno].nokeys+1)<keyno || keyno<1)
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s].  Using Keyno [1] instead.",keyno,MODULENAME(fileno),fileno,FILENAME(fileno));
		keyno=1;
	}
	if(NRDtmp->record_data==NULL)
	{
		varlen=getdreclen(NRDtmp);
		QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,varlen);
	}
	ctree_keyno=NRDtmp->filenum+keyno;
	assignCTREERecordData(NRDtmp,FALSE);
	return_value=LSTREC(ctree_keyno,NRDtmp->record_data);
	if(!return_value)
	{
		if(NRDtmp->varfile==TRUE)
		{
			varlen=GETVLEN(NRDtmp->filenum);
			QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,varlen+1);
			memset(NRDtmp->record_data,0,NRDtmp->dlen);
			return_value=REDVREC(NRDtmp->filenum,
				NRDtmp->record_data,varlen);
		}
		assignCtree_fields(NRDtmp);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagnrd) { xSEENRDRECORD(fileno,__LINE__,__FILE__);prterr("Exiting LSTNRDctree OK for Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name); }
#endif /* USE_RDA_DIAGNOSTICS */
		return(0);
	}
	if(return_value!=101)
	{
		prterr("Error in C-tree Database [%s] for Module [%s] File [%s].",ctdb_errors(errno),NRDtmp->modname,NRDtmp->name);
		return(-1);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG Specified Key Value not found in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(1);
}
#endif
#ifdef USE_CTREE
short GTENRDctree(short fileno,short keyno)
{
	short ctree_keyno,x=0;
	VRLEN varlen;
	NRDbuffer *NRDtmp;
	LONG return_value=0;
	void *target=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG Searching for Greater than or equal key with C-tree engine for file [%d] [%s] keyno [%d].",fileno,FILENAME(fileno),keyno); }
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	RecordNRDconnect(NRDtmp->servername,NRDtmp->db_engine);
	SETCURI(NRDtmp->filenum,return_value,NULL,0);
/* check key exists, if not use 1st key. All indexes have 1 key */
	if((NRDbuf[fileno].nokeys+1)<keyno || keyno<1)
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s].  Using Keyno [1] instead.",keyno,MODULENAME(fileno),fileno,FILENAME(fileno));
		keyno=1;
	}
	if(NRDtmp->record_data==NULL)
	{
		varlen=getdreclen(NRDtmp);
		QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,varlen);
	}
	ctree_keyno=NRDtmp->filenum+keyno;
	target=maketarget(fileno,keyno,&x);
	return_value=GTEREC(ctree_keyno,target,NRDtmp->record_data);
	if(target!=NULL) Rfree(target);
	if(!return_value)
	{
		if(NRDtmp->varfile==TRUE)
		{
			varlen=GETVLEN(NRDtmp->filenum);
			QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,varlen+1);
			memset(NRDtmp->record_data,0,NRDtmp->dlen);
			return_value=REDVREC(NRDtmp->filenum,
				NRDtmp->record_data,varlen);
		}
		assignCtree_fields(NRDtmp);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagnrd) { xSEENRDRECORD(fileno,__LINE__,__FILE__);prterr("Exiting GTENRDctree OK for Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name); }
#endif /* USE_RDA_DIAGNOSTICS */
		return(0);
	}
	if(return_value!=101)
	{
		prterr("Error in C-tree Database [%s] for Module [%s] File [%s].",ctdb_errors(errno),NRDtmp->modname,NRDtmp->name);
		return(-1);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG Specified Key Value not found in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(1);
}
#endif
short xLTENRD(short fileno,short keyno,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field) { prterr("DIAG LTENRD Searching for Less Than or Equal To in Module [%s] File [%d] [%s] using Keyno [%d] at line [%d] program [%s].",MODULENAME(fileno),fileno,NRDbuf[fileno].name,keyno,line,file); } 
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	switch(NRDbuf[fileno].db_engine)
	{
		case NRDRdaEngine:	return(LTENRDrda(fileno,keyno));
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:	return(LTENRDisam(fileno,keyno));
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine:
			return(LTENRDmyisam(fileno,keyno));
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine:
		case NRDMySQLEngine: 
		case NRDMySQLAutoEngine:
			return(LTENRDmysql(fileno,keyno));
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree:		return(LTENRDberkeley(fileno,keyno));
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine:	return(LTENRDctree(fileno,keyno));
		case NRDLocalCtreeEngine:	return(LTENRDctree(fileno,keyno));
#endif
		default:
			prterr("Error Unknown Database Engine [%d] for Module [%s] File [%d] [%s] at line [%d] program [%s]",NRDbuf[fileno].db_engine,MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
			return(-1);
	}
}
short LTENRDrda(short fileno,short keyno)
{
	short retval=0;
	NRDbuffer *NRDtmp;
	DTA y;
	KEY x;

	setrdakeys(fileno,keyno);
	NRDtmp=NRDbuf+fileno;
	y=NRDbuf[fileno].dtanum;
	x=y->key+(keyno-1);
	retval=LTEDTA(x);
	if(!retval) 
	{
		NRDtmp->currec=y->cur;
		assign_rdafields(NRDtmp);
	}
	return(retval);
}
#ifdef USE_CTREE
short LTENRDctree(short fileno,short keyno)
{
	short ctree_keyno=0,x=0;
	VRLEN varlen;
	NRDbuffer *NRDtmp;
	LONG return_value=0;
	void *target=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG LTENRDctree Searching for Less Than or Equal To in Module [%s] File [%d] [%s] using C-tree Database Engine.",MODULENAME(fileno),fileno,FILENAME(fileno)); }
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	RecordNRDconnect(NRDtmp->servername,NRDtmp->db_engine);
/* check key exists, if not use 1st key. All indexes have 1 key */
	if((NRDbuf[fileno].nokeys+1)<keyno || keyno<1)
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s].  Using Keyno [1] instead.",keyno,MODULENAME(fileno),fileno,FILENAME(fileno));
		keyno=1;
	}
	if(NRDtmp->record_data==NULL)
	{
		varlen=getdreclen(NRDtmp);
		QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,varlen);
	}
	ctree_keyno=NRDtmp->filenum+keyno;
	target=maketarget(fileno,keyno,&x);
	return_value=LTEREC(ctree_keyno,target,NRDtmp->record_data);
	if(target!=NULL) Rfree(target);
	if(!return_value)
	{
		if(NRDtmp->varfile==TRUE)
		{
			varlen=GETVLEN(NRDtmp->filenum);
			QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,varlen+1);
			memset(NRDtmp->record_data,0,NRDtmp->dlen);
			return_value=REDVREC(NRDtmp->filenum,
				NRDtmp->record_data,varlen);
		}
		assignCtree_fields(NRDtmp);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagnrd) { xSEENRDRECORD(fileno,__LINE__,__FILE__);prterr("Exiting LTENRDctree OK for Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name); }
#endif /* USE_RDA_DIAGNOSTICS */
		return(0);
	}
	if(return_value!=101)
	{
		prterr("Error in C-tree Database [%s] for Module [%s] File [%s].",ctdb_errors(errno),NRDtmp->modname,NRDtmp->name);
		return(-1);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG Specified Key Value not found in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(1);
}
#endif
short xFRSNRD(short fileno,short keyno,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field) { prterr("DIAG FRSNRD Searching for First Record in Module [%s] File [%d] [%s] Keyno [%d] at line [%d] program [%s].",MODULENAME(fileno),fileno,NRDbuf[fileno].name,keyno,line,file); }
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	switch(NRDbuf[fileno].db_engine)
	{
		case NRDRdaEngine:	
			return(FRSNRDrda(fileno,keyno));
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:	
			return(FRSNRDisam(fileno,keyno));
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine:
			return(FRSNRDmyisam(fileno,keyno));
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine: 
		case NRDMySQLEngine: 
		case NRDMySQLAutoEngine:
			return(FRSNRDmysql(fileno,keyno));
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree:		
			return(FRSNRDberkeley(fileno,keyno));
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine:	
			return(FRSNRDctree(fileno,keyno));
		case NRDLocalCtreeEngine:	
			return(FRSNRDctree(fileno,keyno));
#endif
		default:
			prterr("Error Unknown Database Engine [%d] for Module [%s] File [%d] [%s] at line [%d] program [%s]",NRDbuf[fileno].db_engine,MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
			return(-1);
	}
}
short FRSNRDrda(short fileno,short keyno)
{
	short retval=0;
	NRDbuffer *NRDtmp;
	DTA y;
	KEY x;

	NRDtmp=NRDbuf+fileno;
	y=NRDtmp->dtanum;
	x=y->key+(keyno-1);
	retval=FRSDTA(x);
	if(!retval) 
	{
		NRDtmp->currec=y->cur;
		assign_rdafields(NRDtmp);
	}
	return(retval);
}
#ifdef USE_CTREE
short FRSNRDctree(short fileno,short keyno)
{
	short ctree_keyno;
	LONG return_value=0;
	VRLEN varlen;
	NRDbuffer *NRDtmp;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG Searching for first record in sorted sequence using C-tree Engine."); }
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	RecordNRDconnect(NRDtmp->servername,NRDtmp->db_engine);
/* set current position in data file to 0 */
	SETCURI(NRDtmp->filenum,return_value,NULL,0);
	if((NRDbuf[fileno].nokeys+1)<keyno || keyno<1)
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s].  Using Keyno [1] instead.",keyno,MODULENAME(fileno),fileno,FILENAME(fileno));
		keyno=1;
	}
	if(NRDtmp->record_data==NULL)
	{
		varlen=getdreclen(NRDtmp);
		QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,varlen);
	}
	ctree_keyno=NRDtmp->filenum+keyno;
	return_value=FRSREC(ctree_keyno,NRDtmp->record_data);
	if(!return_value)
	{
		if(NRDtmp->varfile==TRUE)
		{
			varlen=GETVLEN(NRDtmp->filenum);
			QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,varlen+1);
			memset(NRDtmp->record_data,0,NRDtmp->dlen);
			return_value=REDVREC(NRDtmp->filenum,
				NRDtmp->record_data,varlen);
		}
		assignCtree_fields(NRDtmp);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagnrd) { xSEENRDRECORD(fileno,__LINE__,__FILE__);prterr("Exiting FRSNRDctree OK for Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name); }
#endif /* USE_RDA_DIAGNOSTICS */
		return(0);
	}
	if(return_value!=101)
	{
		prterr("Error in C-tree Database [%s] for Module [%s] File [%s].",ctdb_errors(errno),NRDtmp->modname,NRDtmp->name);
		return(-1);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG Specified Key Value not found in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(1);
}
#endif
short xNXTNRD(short fileno,short keyno,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field) { prterr("DIAG NXTNRD Searching for Next Record in Sorted Sequence in Module [%s] File [%d] [%s] Keyno [%d] at line [%d] program [%s] .",MODULENAME(fileno),fileno,NRDbuf[fileno].name,keyno,line,file); }
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	switch(NRDbuf[fileno].db_engine)
	{
		case NRDRdaEngine:	return(NXTNRDrda(fileno,keyno));
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:	return(NXTNRDisam(fileno,keyno));
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine:
			return(NXTNRDmyisam(fileno,keyno));
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine:
		case NRDMySQLEngine:
		case NRDMySQLAutoEngine:
			return(NXTNRDmysql(fileno,keyno));
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree:		return(NXTNRDberkeley(fileno,keyno));
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine:	return(NXTNRDctree(fileno,keyno));
		case NRDLocalCtreeEngine:	return(NXTNRDctree(fileno,keyno));
#endif
		default:
			prterr("Error Unknown Database Engine [%d] for Module [%s] File [%d] [%s] at line [%d] program [%s]",NRDbuf[fileno].db_engine,MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
			return(-1);
	}
}
short NXTNRDrda(short fileno,short keyno)
{
	short retval=0;
	NRDbuffer *NRDtmp;
	DTA y;
	KEY x;

	setrdakeys(fileno,keyno);
	NRDtmp=NRDbuf+fileno;
	y=NRDbuf[fileno].dtanum;
	x=y->key+(keyno-1);
	retval=NXTDTA(x);
	if(!retval) 
	{
		NRDtmp->currec=y->cur;
		assign_rdafields(NRDtmp);
	}
	return(retval);
}
#ifdef USE_CTREE
short NXTNRDctree(short fileno,short keyno)
{
	short ctree_keyno;
	LONG return_value=0;
	VRLEN varlen;
	NRDbuffer *NRDtmp;
	short x=0;
	void *target=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG Searching for next record in sorted sequence using C-tree Engine."); }
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	RecordNRDconnect(NRDtmp->servername,NRDtmp->db_engine);
	if((NRDbuf[fileno].nokeys+1)<keyno || keyno<1)
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s].  Using Keyno [1] instead.",keyno,MODULENAME(fileno),fileno,FILENAME(fileno));
		keyno=1;
	}
	if(NRDtmp->record_data==NULL)
	{
		varlen=getdreclen(NRDtmp);
		QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,varlen);
	}
	ctree_keyno=NRDtmp->filenum+keyno;
	assignCTREERecordData(NRDtmp,FALSE);
	target=maketarget(fileno,keyno,&x);
/*
	return_value=EQLREC(ctree_keyno,target,NRDtmp->record_data);
*/
	return_value=GTREC(ctree_keyno,target,NRDtmp->record_data);
	if(target!=NULL) Rfree(target);
/*
	if(return_value)
	{
		return_value=FRSREC(ctree_keyno,NRDtmp->record_data);
	} else {
		return_value=NXTREC(ctree_keyno,NRDtmp->record_data);
	}
*/
	if(!return_value)
	{
		if(NRDtmp->varfile==TRUE)
		{
			varlen=GETVLEN(NRDtmp->filenum);
			QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,varlen+1);
			memset(NRDtmp->record_data,0,NRDtmp->dlen);
			return_value=REDVREC(NRDtmp->filenum,NRDtmp->record_data,varlen);
		}
		assignCtree_fields(NRDtmp);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagnrd) { xSEENRDRECORD(fileno,__LINE__,__FILE__);prterr("Exiting NXTNRDctree OK for Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name); }
#endif /* USE_RDA_DIAGNOSTICS */
		return(0);
	}
	if(return_value!=101)
	{
		prterr("Error in C-tree Database [%s] for Module [%s] File [%s].",ctdb_errors(errno),NRDtmp->modname,NRDtmp->name);
		return(-1);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG Specified Key Value not found in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(1);
}
#endif
long xSIZNRD(short fileno,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field) { prterr("DIAG SIZNRD Getting Number of Records in Module [%s] File [%d] [%s] at line [%d] program [%s].",MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file); }
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	switch(NRDbuf[fileno].db_engine)
	{
		case NRDRdaEngine:	return(SIZNRDrda(fileno));
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:	return(SIZNRDisam(fileno));
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine: 
			return(SIZNRDmyisam(fileno));
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine: 
		case NRDMySQLEngine: 
		case NRDMySQLAutoEngine:
			return(SIZNRDmysql(fileno));
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree:		return(SIZNRDberkeley(fileno));
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine:	return(SIZNRDctree(fileno));
		case NRDLocalCtreeEngine:	return(SIZNRDctree(fileno));
#endif
		default:
			prterr("Error Unknown Database Engine [%d] for Module [%s] File [%d] [%s] at line [%d] program [%s]",NRDbuf[fileno].db_engine,MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
			return(-1);
	}
}
long SIZNRDrda(short fileno)
{
	NRDbuffer *NRDtmp;
	long tmp=0;

	NRDtmp=NRDbuf+fileno;
	tmp=SIZDTA(NRDtmp->dtanum);
	return(tmp);
}
#ifdef USE_CTREE
long SIZNRDctree(short fileno)
{
	long return_value;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG Getting number of keys for file [%d] [%s] using C-tree Engine.",fileno,NRDbuf[fileno].name); }
#endif /* USE_RDA_DIAGNOSTICS */
	RecordNRDconnect(NRDbuf[fileno].servername,NRDbuf[fileno].db_engine);
	return_value=DATENT(NRDbuf[fileno].filenum);
	if(!return_value) 
	{
		/* Had error here but since the uerr_cod can't be accessed without causing a memoryfault we can't establish weither the return value of ZERO represents the number of records is zero or that the DATENT() function failed.*/
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) 
	{ 
		prterr("DIAG SIZNRD for C-tree Database file [%d] [%s][%s] returing size of [%d].",fileno,NRDbuf[fileno].modname,NRDbuf[fileno].name,return_value);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(return_value);

#ifdef XXXXX
	if(!return_value && uerr_cod!=0) 
	{
		prterr("Error in C-tree Database [%s] for Module [%s] File [%d] [%s].",ctdb_errors(errno),NRDbuf[fileno].modname,fileno,NRDbuf[fileno].name);
		return(0);	
	} else {
#ifdef USE_RDA_DIAGNOSTICS
		if(diagnrd)
		{
			prterr("DIAG SIZNRD Returning [%ld] records in Module [%s] File [%d] [%s].",return_value,NRDbuf[fileno].modname,fileno,NRDbuf[fileno].name); 
		}
#endif /* USE_RDA_DIAGNOSTICS */
		return(return_value);
	}
#endif /* ifdef XXXXX */
}
#endif
void xZeroNRDfield(NRDfield *field,int line,char *file)
{
	switch(field->type)
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
			if(field->dlen>0)
			{
				memset(field->data.string_value,0,field->dlen);
			} else {
				field->data.string_value=NULL;
				field->dlen=0;
			}
			break;
		case CHARACTERS:
		case BOOLNS:
			*field->data.string_value=0;
			break;
		case DOUBLEV:
		case SDOUBLEV:
		case DECIMALV:
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case SDECIMALV:
			*field->data.float_value=0;
			break;
		case SHORTV:
		case SSHORTV:
			*field->data.short_value=0;
			break;
		case SCROLLEDLIST:
		case LONGV:
		case SLONGV:
			*field->data.integer_value=0;
			break;
		default:
			prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
			break;
	}
}
void xKEYNRD(short fileno,short keyno,int line,char *file)
{
	short x,y;
	short dont;
	NRDkey *key;
	NRDpart *part;
	NRDfield *field;
	NRDbuffer *NRDtmp;
#ifdef USE_CTREE
	int return_value=0;
#endif

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field) { prterr("DIAG KEYNRD Zeroing All but Key [%d]'s Fields in Module [%s] File [%d] [%s] at line [%d] program [%s].",keyno,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file); }
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return;
	NRDtmp=NRDbuf+fileno;
	switch(NRDbuf[fileno].db_engine)
	{
		case NRDRdaEngine:	
			NRDtmp->currec=0;
			break;
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:
			NRDtmp->currec=0;
			break;
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine:
			break;
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine:
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
			SETCURI(NRDtmp->filenum,return_value,NULL,0);
			break;
#endif
		default:
			prterr("Error Unknown Database Engine [%d] for Module [%s] File [%d] [%s] at line [%d] program [%s]",NRDbuf[fileno].db_engine,MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
			break;
	}
	if((NRDbuf[fileno].nokeys+1)<keyno || keyno<1)
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s].  Using Keyno [1] instead.",keyno,MODULENAME(fileno),fileno,FILENAME(fileno));
		keyno=1;
	}
	for(x=0,field=NRDtmp->field+x;x<NRDtmp->nofields;++x,++field)
	{
		key=NRDtmp->key+(keyno-1);   /*key=NRDtmp->key+(keyno-1);*/
		dont=FALSE;
		for(y=0;y<key->numparts;++y)
		{
			part=key->part+y;
			if(!RDAstrcmp(part->name,field->name))
			{
				dont=TRUE;
				break;
			}
		}
		if(!dont)
		{
			ZeroNRDfield(field);
		}
	}
}
void xZERNRD(short fileno,int line,char *file)
{
	short x;
	NRDfield *field;
	NRDbuffer *NRDtmp;
#ifdef USE_CTREE
	LONG return_value=0;
#endif

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field) { prterr("DIAG ZERNRD Zeroing All fields in Module [%s] File [%d] [%s] at line [%d] program [%s].",MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file); }
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return;
	NRDtmp=NRDbuf+fileno;
	switch(NRDbuf[fileno].db_engine)
	{
		case NRDRdaEngine:	
			NRDtmp->currec=0;
			break;
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:
			NRDtmp->currec=0;
			break;
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine:
			break;
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine:
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
			SETCURI(NRDtmp->filenum,return_value,NULL,0);
			break;
#endif
		default:
			prterr("Error Unknown Database Engine [%d] for Module [%s] File [%d] [%s] at line [%d] program [%s]",NRDbuf[fileno].db_engine,MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
			break;
	}
	if(NRDtmp->field!=NULL)
	{
	for(x=0,field=NRDtmp->field+x;x<NRDtmp->nofields;++x,++field)
	{
		ZeroNRDfield(field);
	}
	}
}
/*---------------------------------------------------------------------------
	FINDFLDSETVALUE	Locates the specified field in current record of the
			selected file and set's it's appropriate value.
---------------------------------------------------------------------------*/
short xFINDFLDSETVALUE(short fileno,char *fieldname,void *value,int line,char *file)
{
	NRDfield *field;
	unsigned h=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("DIAG FINDFLDSETVALUE Setting value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	field=xFLDNRD(fileno,fieldname,__LINE__,__FILE__);
	if(field!=NULL)
	{
		switch(field->type)
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
				if(!isEMPTY(value))
				{
					h=RDAstrlen((char *)value);
					QUICKALLOC(field->data.string_value,field->dlen,h+1);
					memcpy(field->data.string_value,(char *)value,h+1);
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
				if(!RDAstrcmp(USERLOGIN,"sjs") ||
					!RDAstrcmp(USERLOGIN,"bld"))
				{
					prterr("WARNING FINDFLDSETVALUE at line [%d] program [%s] should be replaced with FINDFLDSETSTRING.",line,file);
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDSETVALUE Found Field [%s] in Module [%s] File [%s] and set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case BOOLNS:
			case CHARACTERS:
				if(field->data.string_value==NULL)
				{
					field->data.string_value=Rmalloc(1);
					field->dlen=1;
				}
				*field->data.string_value=*(char *)value;
				if(!RDAstrcmp(USERLOGIN,"sjs") ||
					!RDAstrcmp(USERLOGIN,"bld"))
				{
					prterr("WARNING FINDFLDSETVALUE at line [%d] program [%s] should be replaced with FINDFLDSETCHAR.",line,file);
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDSETVALUE Found Field [%s] in Module [%s] File [%s] and set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				*field->data.float_value=*(double *)value;
				if(!RDAstrcmp(USERLOGIN,"sjs") ||
					!RDAstrcmp(USERLOGIN,"bld"))
				{
					prterr("WARNING FINDFLDSETVALUE at line [%d] program [%s] should be replaced with FINDFLDSETDOUBLE.",line,file);
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDSETVALUE Found Field [%s] in Module [%s] File [%s] and set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SHORTV:
			case SSHORTV:
				*field->data.short_value=*(short *)value;
				if(!RDAstrcmp(USERLOGIN,"sjs") ||
					!RDAstrcmp(USERLOGIN,"bld"))
				{
					prterr("WARNING FINDFLDSETVALUE at line [%d] program [%s] should be replaced with FINDFLDSETSHORT.",line,file);
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDSETVALUE Found Field [%s] in Module [%s] File [%s] and set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case LONGV:
			case SLONGV:
				*field->data.integer_value=*(int *)value;
				if(!RDAstrcmp(USERLOGIN,"sjs") ||
					!RDAstrcmp(USERLOGIN,"bld"))
				{
					prterr("WARNING FINDFLDSETVALUE at line [%d] program [%s] should be replaced with FINDFLDSETINT.",line,file);
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDSETVALUE Found Field [%s] in Module [%s] File [%s] and set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			default:
				prterr("Error Field Type [%d] is invalid in Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",field->type,field->name,MODULENAME(fileno),FILENAME(fileno),line,file);
				break;
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("Error FINDFLDSETVALUE Field [%s] not found in Module [%s] File [%s] couldn't set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDFLDSETSTRING(short fileno,char *fieldname,char *value,int line,char *file)
{
	NRDfield *field;
	unsigned h=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("DIAG FINDFLDSETSTRING Setting value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	field=xFLDNRD(fileno,fieldname,__LINE__,__FILE__);
	if(field!=NULL)
	{
		switch(field->type)
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
				if(!isEMPTY(value))
				{
					h=RDAstrlen((char *)value);
					QUICKALLOC(field->data.string_value,field->dlen,h+1);
					memcpy(field->data.string_value,(char *)value,h+1);
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
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDSETSTRING Found Field [%s] in Module [%s] File [%s] and set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case BOOLNS:
			case CHARACTERS:
				prterr("Error the FINDFLDSETSTRING at line [%d] program [%s] should be replaced with FINDFLDSETCHAR.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				prterr("Error the FINDFLDSETSTRING at line [%d] program [%s] should be replaced with FINDFLDSETDOUBLE.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case SHORTV:
			case SSHORTV:
				prterr("Error the FINDFLDSETSTRING at line [%d] program [%s] should be replaced with FINDFLDSETSHORT.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case LONGV:
			case SLONGV:
				prterr("Error the FINDFLDSETSTRING at line [%d] program [%s] should be replaced with FINDFLDSETINT.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			default:
				prterr("Error Field Type [%d] is invalid in Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",field->type,field->name,MODULENAME(fileno),FILENAME(fileno),line,file);
				break;
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("Error FINDFLDSETSTRING Field [%s] not found in Module [%s] File [%s] couldn't set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDFLDSETCHAR(short fileno,char *fieldname,char value,int line,char *file)
{
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("DIAG FINDFLDSETCHAR Setting value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	field=xFLDNRD(fileno,fieldname,__LINE__,__FILE__);
	if(field!=NULL)
	{
		switch(field->type)
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
				prterr("Error the FINDFLDSETCHAR at line [%d] program [%s] should be replaced with FINDFLDSETSTRING.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				if(field->data.string_value==NULL)
				{
					field->data.string_value=Rmalloc(1);
					field->dlen=1;
				}
				*field->data.string_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDSETCHAR Found Field [%s] in Module [%s] File [%s] and set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				*field->data.float_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDSETCHAR Found Field [%s] in Module [%s] File [%s] and set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SHORTV:
			case SSHORTV:
				*field->data.short_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDSETCHAR Found Field [%s] in Module [%s] File [%s] and set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case LONGV:
			case SLONGV:
				*field->data.integer_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDSETCHAR Found Field [%s] in Module [%s] File [%s] and set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			default:
				prterr("Error Field Type [%d] is invalid in Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",field->type,field->name,MODULENAME(fileno),FILENAME(fileno),line,file);
				break;
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("Error FINDFLDSETCHAR Field [%s] not found in Module [%s] File [%s] couldn't set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDFLDSETSHORT(short fileno,char *fieldname,short value,int line,char *file)
{
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("DIAG FINDFLDSETSHORT Setting value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	field=xFLDNRD(fileno,fieldname,__LINE__,__FILE__);
	if(field!=NULL)
	{
		switch(field->type)
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
				prterr("Error the FINDFLDSETSHORT at line [%d] program [%s] should be replaced with FINDFLDSETSTRING.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				if(value<128 && value>-127)
				{
					*field->data.string_value=value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diagnrd || diagnrd_field)
					{
						prterr("DIAG FINDFLDSETSHORT Found Field [%s] in Module [%s] File [%s] and set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in Precision the FINDFLDSETSHORT at line [%d] program [%s] should be replaced with FINDFLDSETCHAR.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				*field->data.float_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDSETSHORT Found Field [%s] in Module [%s] File [%s] and set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SHORTV:
			case SSHORTV:
				*field->data.short_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDSETSHORT Found Field [%s] in Module [%s] File [%s] and set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case LONGV:
			case SLONGV:
				*field->data.integer_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDSETSHORT Found Field [%s] in Module [%s] File [%s] and set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			default:
				prterr("Error Field Type [%d] is invalid in Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",field->type,field->name,MODULENAME(fileno),FILENAME(fileno),line,file);
				break;
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("Error FINDFLDSETSHORT Field [%s] not found in Module [%s] File [%s] couldn't set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDFLDSETINT(short fileno,char *fieldname,int value,
	int line,char *file)
{
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("DIAG FINDFLDSETINT Setting value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	field=xFLDNRD(fileno,fieldname,__LINE__,__FILE__);
	if(field!=NULL)
	{
		switch(field->type)
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
				prterr("Error the FINDFLDSETINT at line [%d] program [%s] should be replaced with FINDFLDSETSTRING.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				if(field->data.string_value==NULL)
				{
					field->data.string_value=Rmalloc(1);
					field->dlen=1;
					*field->data.string_value=0;
				}
				if(value>-127 && value<128)
				{
					*field->data.string_value=value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diagnrd || diagnrd_field)
					{
						prterr("DIAG FINDFLDSETINT Found Field [%s] in Module [%s] File [%s] and set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
						SEENRDFIELD(field);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in Precision the FINDFLDSETINT at line [%d] program [%s] should be replaced with FINDFLDSETCHAR.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				*field->data.float_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDSETINT Found Field [%s] in Module [%s] File [%s] and set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SHORTV:
			case SSHORTV:
				if(value<SHRT_MAX)
				{
					*field->data.short_value=value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diagnrd || diagnrd_field)
					{
						prterr("DIAG FINDFLDSETINT Found Field [%s] in Module [%s] File [%s] and set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
						SEENRDFIELD(field);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in Precision the FINDFLDSETINT at line [%d] program [%s] should be replaced with FINDFLDSETSHORT.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case LONGV:
			case SLONGV:
				*field->data.integer_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDSETINT Found Field [%s] in Module [%s] File [%s] and set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			default:
				prterr("Error Field Type [%d] is invalid in Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",field->type,field->name,MODULENAME(fileno),FILENAME(fileno),line,file);
				break;
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("Error FINDFLDSETINT Field [%s] not found in Module [%s] File [%s] couldn't set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDFLDSETDOUBLE(short fileno,char *fieldname,double value,int line,char *file)
{
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("DIAG FINDFLDSETDOUBLE Setting value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	field=xFLDNRD(fileno,fieldname,__LINE__,__FILE__);
	if(field!=NULL)
	{
		switch(field->type)
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
				prterr("Error the FINDFLDSETDOUBLE at line [%d] program [%s] should be replaced with FINDFLDSETSTRING.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				if(field->data.string_value==NULL)
				{
					field->data.string_value=Rmalloc(1);
					field->dlen=1;
					*field->data.string_value=0;
				}
				if(value>-127 && value<128)
				{
/*lint -e524*/
					*field->data.string_value=value;
/*lint +e524*/
#ifdef USE_RDA_DIAGNOSTICS
					if(diagnrd || diagnrd_field)
					{
						prterr("DIAG FINDFLDSETDOUBLE Found Field [%s] in Module [%s] File [%s] and set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
						SEENRDFIELD(field);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in Precision the FINDFLDSETDOUBLE at line [%d] program [%s] should be replaced with FINDFLDSETCHAR.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				*field->data.float_value=value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDSETDOUBLE Found Field [%s] in Module [%s] File [%s] and set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SHORTV:
			case SSHORTV:
				if(value<SHRT_MAX)
				{
/*lint -e524*/
					*field->data.short_value=value;
/*lint +e524*/
#ifdef USE_RDA_DIAGNOSTICS
					if(diagnrd || diagnrd_field)
					{
						prterr("DIAG FINDFLDSETDOUBLE Found Field [%s] in Module [%s] File [%s] and set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
						SEENRDFIELD(field);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in Precision the FINDFLDSETDOUBLE at line [%d] program [%s] should be replaced with FINDFLDSETSHORT.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case LONGV:
			case SLONGV:
				if(value<INT_MAX)
				{
/*lint -e524*/
					*field->data.integer_value=value;
/*lint +e524*/
#ifdef USE_RDA_DIAGNOSTICS
					if(diagnrd || diagnrd_field)
					{
						prterr("DIAG FINDFLDSETDOUBLE Found Field [%s] in Module [%s] File [%s] and set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
						SEENRDFIELD(field);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in Precision the FINDFLDSETDOUBLE at line [%d] program [%s] should be replaced with FINDFLDSETINT.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			default:
				prterr("Error Field Type [%d] is invalid in Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",field->type,field->name,MODULENAME(fileno),FILENAME(fileno),line,file);
				break;
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("Error FINDFLDSETDOUBLE Field [%s] not found in Module [%s] File [%s] couldn't set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDFLDGETSTRING(short fileno,char *fieldname,char **value,int line,char *file)
{
	NRDfield *field;
	short h=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("DIAG FINDFLDGETSTRING Getting string value of [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",MODULENAME(fileno),fieldname,fileno,NRDbuf[fileno].name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(FALSE);
	field=xFLDNRD(fileno,fieldname,__LINE__,__FILE__);
	if(field!=NULL)
	{
		switch(field->type)
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
				if(field->data.string_value!=NULL)
				{
					h=RDAstrlen(*value);
					if(field->dlen>h || *value==NULL)
					{
						*value=Rrealloc(*value,field->dlen+1);
					}
					memcpy(*value,field->data.string_value,field->dlen);
				} else {
					if(*value!=NULL) Rfree(*value);
					*value=NULL;
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDGETSTRING Found Field [%s] in Module [%s] File [%s] and set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				prterr("Error the FINDFLDGETSTRING at line [%d] program [%s] should be replaced with FINDFLDGETDOUBLE.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case SHORTV:
			case SSHORTV:
				prterr("Error the FINDFLDGETSTRING at line [%d] program [%s] should be replaced with FINDFLDGETSHORT.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				prterr("Error the FINDFLDGETSTRING at line [%d] program [%s] should be replaced with FINDFLDGETINT.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				prterr("Error the FINDFLDGETSTRING at line [%d] program [%s] should be replaced with FINDFLDGETCHAR.",line,file);
				SEENRDFIELD(field);TRACE;
				break;
			default :
				prterr("Error Field Type [%d] is invalid in Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",field->type,field->name,MODULENAME(fileno),FILENAME(fileno),line,file);
				return(-1);
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("Error FINDFLDGETSTRING Field [%s] not found in Module [%s] File [%s] couldn't set value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
/*---------------------------------------------------------------------------
	FINDFLDGETVALUE	Locates the specified field in current record of the
			selected file and get's it's numerical value.
---------------------------------------------------------------------------*/
short xFINDFLDGETVALUE(short fileno,char *fieldname,void *value,int line,char *file)
{
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("DIAG FINDFLDGETVALUE Get Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(FALSE);
	field=xFLDNRD(fileno,fieldname,__LINE__,__FILE__);
	if(field!=NULL)
	{
		switch(field->type)
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
				prterr("Error Field Type [%d] for Field [%s] in Module [%s] File [%s] requires library function FINDFLDGETSTRING instead of FINDFLDGETVALUE at line [%d] program [%s].",field->type,fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
				break;
			case BOOLNS:
			case CHARACTERS:
				*(char *)value=*field->data.string_value;
				if(!RDAstrcmp(USERLOGIN,"sjs") ||
					!RDAstrcmp(USERLOGIN,"bld"))
				{
					prterr("WARNING FINDFLDGETVALUE in line [%d] program [%s] should be replaced with FINDFLDGETCHAR.",line,file);
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDGETVALUE Read Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				*(double *)value=*field->data.float_value;
				if(!RDAstrcmp(USERLOGIN,"sjs") ||
					!RDAstrcmp(USERLOGIN,"bld"))
				{
					prterr("WARNING FINDFLDGETVALUE in line [%d] program [%s] should be replaced with FINDFLDGETDOUBLE.",line,file);
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDGETVALUE Read Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SHORTV:
			case SSHORTV:
				*(short *)value=*field->data.short_value;
				if(!RDAstrcmp(USERLOGIN,"sjs") ||
					!RDAstrcmp(USERLOGIN,"bld"))
				{
					prterr("WARNING FINDFLDGETVALUE in line [%d] program [%s] should be replaced with FINDFLDGETSHORT.",line,file);
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDGETVALUE Read Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				*(int *)value=*field->data.integer_value;
				if(!RDAstrcmp(USERLOGIN,"sjs") ||
					!RDAstrcmp(USERLOGIN,"bld"))
				{
					prterr("WARNING FINDFLDGETVALUE in line [%d] program [%s] should be replaced with FINDFLDGETINT.",line,file);
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDGETVALUE Read Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			default :
				prterr("Error Field Type [%d] is invalid in Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",field->type,field->name,MODULENAME(fileno),FILENAME(fileno),line,file);
				return(-1);
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("Error FINDFLDGETVALUE Field [%s] not found in Module [%s] File [%s] couldn't read value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDFLDGETCHAR(short fileno,char *fieldname,char *value,
	int line,char *file)
{
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("DIAG FINDFLDGETCHAR Get Character Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(FALSE);
	field=xFLDNRD(fileno,fieldname,__LINE__,__FILE__);
	if(field!=NULL)
	{
		switch(field->type)
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
				prterr("Error Field Type [%d] for Field [%s] in Module [%s] File [%s] requires library function FINDFLDGETSTRING instead of FINDFLDGETVALUE at line [%d] program [%s].",field->type,fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				*(char *)value=*(char *)field->data.string_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDGETCHAR Read Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				if(*field->data.float_value>-127 &&
					*field->data.float_value<128)
				{
					*(char *)value=*(char *)field->data.float_value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diagnrd || diagnrd_field)
					{
						prterr("DIAG FINDFLDGETCHAR Read Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
						SEENRDFIELD(field);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in precision the FINDFLDGETCHAR at line [%d] program [%s] should be replaced with FINDFLDGETDOUBLE.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case SHORTV:
			case SSHORTV:
				if(*field->data.short_value>-127 &&
					*field->data.short_value<128)
				{
					*(char *)value=*(char *)field->data.short_value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diagnrd || diagnrd_field)
					{
						prterr("DIAG FINDFLDGETCHAR Read Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
						SEENRDFIELD(field);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in precision the FINDFLDGETCHAR at line [%d] program [%s] should be replaced with FINDFLDGETSHORT.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				if(*field->data.integer_value>-127 &&
					*field->data.integer_value<128)
				{
					*(char *)value=*(char *)field->data.integer_value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diagnrd || diagnrd_field)
					{
						prterr("DIAG FINDFLDGETCHAR Read Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
						SEENRDFIELD(field);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in precision the FINDFLDGETCHAR at line [%d] program [%s] should be replaced with FINDFLDGETINT.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			default :
				prterr("Error Field Type [%d] is invalid in Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",field->type,field->name,MODULENAME(fileno),FILENAME(fileno),line,file);
				return(-1);
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("Error FINDFLDGETCHAR Field [%s] not found in Module [%s] File [%s] couldn't read value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDFLDGETSHORT(short fileno,char *fieldname,short *value,
	int line,char *file)
{
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("DIAG FINDFLDGETSHORT Get Short Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(FALSE);
	field=xFLDNRD(fileno,fieldname,__LINE__,__FILE__);
	if(field!=NULL)
	{
		switch(field->type)
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
				prterr("Error Field Type [%d] for Field [%s] in Module [%s] File [%s] requires library function FINDFLDGETSTRING instead of FINDFLDGETVALUE at line [%d] program [%s].",field->type,fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				*value=*field->data.string_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDGETSHORT Read Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				if(*field->data.float_value<SHRT_MAX)
				{
/*lint -e524*/
					*value=*field->data.float_value;
/*lint +e524*/
#ifdef USE_RDA_DIAGNOSTICS
					if(diagnrd || diagnrd_field)
					{
						prterr("DIAG FINDFLDGETSHORT Read Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
						SEENRDFIELD(field);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in precision the FINDFLDGETSHORT at line [%d] program [%s] should be replaced with FINDFLDGETDOUBLE.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case SHORTV:
			case SSHORTV:
				*value=*field->data.short_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDGETSHORT Read Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				if(*field->data.integer_value<SHRT_MAX)
				{
/*lint -e524*/
					*value=*field->data.integer_value;
/*lint +e524*/
#ifdef USE_RDA_DIAGNOSTICS
					if(diagnrd || diagnrd_field)
					{
						prterr("DIAG FINDFLDGETSHORT Read Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
						SEENRDFIELD(field);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in precision the FINDFLDGETSHORT at line [%d] program [%s] should be replaced with FINDFLDGETINT.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			default :
				prterr("Error Field Type [%d] is invalid in Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",field->type,field->name,MODULENAME(fileno),FILENAME(fileno),line,file);
				return(-1);
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("Error FINDFLDGETSHORT Field [%s] not found in Module [%s] File [%s] couldn't read value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDFLDGETINT(short fileno,char *fieldname,int *value,int line,char *file)
{
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("DIAG FINDFLDGETINT Get Integer Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(FALSE);
	field=xFLDNRD(fileno,fieldname,__LINE__,__FILE__);
	if(field!=NULL)
	{
		switch(field->type)
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
				prterr("Error Field Type [%d] for Field [%s] in Module [%s] File [%s] requires library function FINDFLDGETSTRING instead of FINDFLDGETVALUE at line [%d] program [%s].",field->type,fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				*value=*field->data.string_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDGETINT Read Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				if(*field->data.float_value<INT_MAX)
				{
/*lint -e524*/
					*value=*field->data.float_value;
/*lint +e524*/
#ifdef USE_RDA_DIAGNOSTICS
					if(diagnrd || diagnrd_field)
					{
						prterr("DIAG FINDFLDGETINT Read Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
						SEENRDFIELD(field);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
				} else {
					prterr("Error in precision the FINDFLDGETINT at line [%d] program [%s] should be replaced with FINDFLDGETDOUBLE.",line,file);
					SEENRDFIELD(field);TRACE;
				}
				break;
			case SHORTV:
			case SSHORTV:
				*value=*field->data.short_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDGETINT Read Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				*value=*field->data.integer_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDGETINT Read Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			default :
				prterr("Error Field Type [%d] is invalid in Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",field->type,field->name,MODULENAME(fileno),FILENAME(fileno),line,file);
				return(-1);
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("Error FINDFLDGETINT Field [%s] not found in Module [%s] File [%s] couldn't read value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
short xFINDFLDGETDOUBLE(short fileno,char *fieldname,double *value,int line,char *file)
{
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("DIAG FINDFLDGETDOUBLE Get Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(FALSE);
	field=xFLDNRD(fileno,fieldname,__LINE__,__FILE__);
	if(field!=NULL)
	{
		switch(field->type)
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
				prterr("Error Field Type [%d] for Field [%s] in Module [%s] File [%s] requires library function FINDFLDGETSTRING instead of FINDFLDGETVALUE at line [%d] program [%s].",field->type,fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
				SEENRDFIELD(field);TRACE;
				break;
			case BOOLNS:
			case CHARACTERS:
				*value=*field->data.string_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDGETDOUBLE Read Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case DOUBLEV:
			case SDOUBLEV:
			case DECIMALV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				*value=*field->data.float_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDGETDOUBLE Read Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				
				break;
			case SHORTV:
			case SSHORTV:
				*value=*field->data.short_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDGETDOUBLE Read Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SCROLLEDLIST:
			case LONGV:
			case SLONGV:
				*value=*field->data.integer_value;
#ifdef USE_RDA_DIAGNOSTICS
				if(diagnrd || diagnrd_field)
				{
					prterr("DIAG FINDFLDGETDOUBLE Read Value for Field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname,MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file);
					SEENRDFIELD(field);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			default :
				prterr("Error Field Type [%d] is invalid in Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",field->type,field->name,MODULENAME(fileno),FILENAME(fileno),line,file);
				return(-1);
		}
		return FALSE;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("Error FINDFLDGETDOUBLE Field [%s] not found in Module [%s] File [%s] couldn't read value at line [%d] program [%s].",fieldname,MODULENAME(fileno),FILENAME(fileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return TRUE;
}
/* GETNRDKEY function gets the specified key for the current record */ 
void *xGETNRDKEY(short fileno,short keyno,short *length,int line,char *file)
{
	long *recno;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field) 
	{ 
		prterr("DIAG GETNRDKEY Getting Key for Current record in Module [%s] File [%d] [%s] Keyno [%d] at line [%d] program [%s].",MODULENAME(fileno),fileno,NRDbuf[fileno].name,keyno,line,file); 
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(NULL);
	switch(NRDbuf[fileno].db_engine)
	{
		case NRDRdaEngine:	
			*length=sizeof(long);
			recno=Rmalloc(sizeof(long));
			*recno=0;
			*recno=NRDbuf[fileno].currec;
			return(recno);
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:	
			*length=sizeof(long);
			recno=Rmalloc(sizeof(long));
			*recno=0;
			*recno=NRDbuf[fileno].currec;
			return(recno);
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine:
			return(maketarget(fileno,keyno,length));
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine:
		case NRDMySQLEngine:
		case NRDMySQLAutoEngine:
			return(maketarget(fileno,keyno,length));
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree:
			return(maketarget(fileno,keyno,length));
			break;
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine:	
		case NRDLocalCtreeEngine:	
			return(maketarget(fileno,keyno,length));
#endif
		default:
			prterr("Error Unknown Database Engine [%d] for Module [%s] File [%d] [%s] at line [%d] program [%s]",NRDbuf[fileno].db_engine,MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
			return(NULL);
	}
}
short AcceptableFields(short fieldtype1,short fieldtype2)
{
	short return_value=FALSE;

	switch(fieldtype1)
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
		case PLAINTEXT:
			if(fieldtype2==SCROLLEDTEXT || fieldtype2==DATES ||
				fieldtype2==VARIABLETEXT ||
				fieldtype2==EXPENDITURE ||
				fieldtype2==REVENUE ||
				fieldtype2==BALANCESHEET ||
				fieldtype2==BEGINNINGBALANCE ||
				fieldtype2==OPTIONALFIELDS ||
				fieldtype2==TIMES || fieldtype2==PHONE ||
				fieldtype2==SOCSECNUM ||
				fieldtype2==ZIPCODE || fieldtype2==PLAINTEXT 
				|| fieldtype2==CUSTOMTYPE) return_value=TRUE;
			break;
		case BOOLNS:
		case CHARACTERS:
			if(fieldtype2==BOOLNS || fieldtype2==CHARACTERS) return_value=TRUE;
			break;
		case DOUBLEV:
		case SDOUBLEV:
		case DECIMALV:
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case SDECIMALV:
			if(fieldtype2==DOUBLEV || fieldtype2==DECIMALV ||
				fieldtype2==DOLLARS || fieldtype2==SDOUBLEV ||
				fieldtype2==DOLLARS_NOCENTS ||
				fieldtype2==SDECIMALV) return_value=TRUE;
			break;
		case SHORTV:
		case SSHORTV:
			if(fieldtype2==SHORTV || fieldtype2==SSHORTV) return_value=TRUE;
			break;
		case SCROLLEDLIST:
		case LONGV:
		case SLONGV:
			if(fieldtype2==LONGV || fieldtype2==SLONGV
				 || fieldtype2==SCROLLEDLIST) return_value=TRUE;
			break;
		default :
			prterr("Error field type [%d].",fieldtype1);
			break;
	}
	return(return_value);
}
short xCOPYFIELD(short ffileno,short tfileno,char *fieldname,int line,
	char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("DIAG COPYFIELD Copying Field [%s] from file [%d] [%s] to file [%d] [%s] at line [%d] program [%s].",(fieldname!=NULL ? fieldname:"Null"),ffileno,FILENAME(ffileno),tfileno,FILENAME(tfileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(FIELDCOPY(ffileno,fieldname,tfileno,fieldname));
}
short xSETFIELDWITHARRAYKEYS(short fileno,short keyno,short nofields,char **argvals,int line,char *file)
{
	short x,sht_value;
	unsigned q=0;
	int int_value;
	double dbl_value;
	NRDkey *key;
	NRDpart *part;
	NRDfield *field;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG SETFIELDWITHARRAYKEYS for Module [%s] File [%d] [%s] Key [%d] at line [%d] program [%s].",MODULENAME(fileno),fileno,FILENAME(fileno),keyno,line,file); }
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	key=NRDbuf[fileno].key+(keyno-1);
	for(x=0;(x<key->numparts && x<nofields);++x)
	{
		part=key->part+x;
		field=xFLDNRD(fileno,part->name,__LINE__,__FILE__);
		if(field!=NULL)
		{
			switch(field->type)
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
				case PLAINTEXT:
					q=RDAstrlen(argvals[x]);
					if(q>0)
					{
						QUICKALLOC(field->data.string_value,field->dlen,q+1);
						memcpy(field->data.string_value,argvals[x],q+1);
						if(field->dlen>field->len && field->len>0)
							field->data.string_value[field->len]=0;
					} else if(field->dlen>0)
					{
						memset(field->data.string_value,0,field->dlen);
					} else {
						field->data.string_value=NULL;
						field->dlen=0;
					}
					break;
				case BOOLNS:
				case CHARACTERS:
					if(field->data.string_value==NULL)
					{
						field->data.string_value=Rmalloc(1);
						field->dlen=1;
					}
					*field->data.string_value=argvals[x][0];
					break;
				case DOUBLEV:
				case SDOUBLEV:
				case DECIMALV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					if(!isEMPTY(argvals[x]))
					{
					dbl_value=atof(argvals[x]);
					} else dbl_value=0;
					*field->data.float_value=dbl_value;
					break;
				case SHORTV:
				case SSHORTV:
					if(!isEMPTY(argvals[x]))
					{	
					sht_value=(short)Ratoi(argvals[x]);
					} else sht_value=0;
					*field->data.short_value=sht_value;
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					if(!isEMPTY(argvals[x]))
					{
					int_value=Ratoi(argvals[x]);
					} else int_value=0;
					*field->data.integer_value=int_value;
					break;
				default:
					prterr("Error Field Type [%d] is invalid in Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",field->type,field->name,MODULENAME(fileno),FILENAME(fileno),line,file);

					break;
			}
		}
	}
	return(0);
}
short xCONVERTPASSABLEKEYS(short fileno,short keyno,short noargs,char **argvals,int line,char *file)
{
	short x,sht_value;
	unsigned q=0;
	int int_value;
	double dbl_value;
	NRDkey *key;
	NRDpart *part;
	NRDfield *field;
	APPlib *args=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG CONVERTPASSABLEKEYS Convert Passable Keys for Module [%s] File [%d] [%s] Key [%d] at line [%d] program [%s].",MODULENAME(fileno),fileno,FILENAME(fileno),keyno,line,file); }
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	args=APPlibNEW();
	for(x=0;x<(noargs-1);++x) addAPPlib(args,argvals[x+1]);
	key=NRDbuf[fileno].key+(keyno-1);
	for(x=0;(x<key->numparts && x<args->numlibs);++x)
	{
		part=key->part+x;
		field=xFLDNRD(fileno,part->name,__LINE__,__FILE__);
		if(field!=NULL)
		{
			switch(field->type)
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
				case PLAINTEXT:
					q=RDAstrlen(args->libs[x]);
					if(q>0)
					{
						QUICKALLOC(field->data.string_value,field->dlen,q+1);
						memcpy(field->data.string_value,args->libs[x],q);
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
				case BOOLNS:
				case CHARACTERS:
					if(field->data.string_value==NULL)
					{
						field->data.string_value=Rmalloc(1);
						*field->data.string_value=0;
						field->dlen=1;
					}
					*field->data.string_value=args->libs[x][0];
					break;
				case DOUBLEV:
				case SDOUBLEV:
				case DECIMALV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					if(!isEMPTY(args->libs[x]))
					{
					dbl_value=atof(args->libs[x]);
					} else dbl_value=0;
					*field->data.float_value=dbl_value;
					break;
				case SHORTV:
				case SSHORTV:
					if(!isEMPTY(args->libs[x]))
					{
					sht_value=(short)Ratoi(args->libs[x]);
					} else sht_value=0;
					*field->data.short_value=sht_value;
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					if(!isEMPTY(args->libs[x]))
					{
					int_value=Ratoi(args->libs[x]);
					} else int_value=0;
					*field->data.integer_value=int_value;
					break;
				default :
					prterr("Error Field Type [%d] is invalid in Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",field->type,field->name,MODULENAME(fileno),FILENAME(fileno),line,file);
					break;
			}
		}
	}
	if(args!=NULL) freeapplib(args);
	return(0);
}
char *xMAKEPASSABLEKEYS(short fileno,short keyno,void *keyvalue,int line,char *file)
{
	int pos=0;
	short x,lf=0;
	char *target=NULL;
	NRDkey *key;
	NRDpart *part;
	NRDfield *field=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG MAKEPASSABLEKEYS Creating Passable Keys for Module [%s] File [%d] [%s] Key [%d] at line [%d] program [%s].",MODULENAME(fileno),fileno,FILENAME(fileno),keyno,line,file); }
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(NULL);
	if(!EQLNRDKEY(fileno,keyno,keyvalue))
	{
		key=NRDbuf[fileno].key+(keyno-1);
		for(x=0;x<key->numparts;++x)
		{
			part=key->part+x;
			if(part!=NULL && part->name!=NULL) 
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
						if(field->data.string_value!=NULL)
						{
							lf=RDAstrlen(field->data.string_value);
						} else lf=0;
						target=Rrealloc(target,pos+lf+4);
						sprintf(&target[pos],"\"%s\" ",
							(field->data.string_value!=NULL ? 
							field->data.string_value:""));
						pos+=(lf+3);
						break;
					case CHARACTERS:
					case BOOLNS:
						target=Rrealloc(target,pos+5);
						if(field->type==BOOLNS)
						{
							sprintf(&target[pos],"\"%c\" ",*field->data.string_value);
						} else sprintf(&target[pos],"\"%1d\" ",*field->data.string_value);
						pos+=4;
						break;
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case SDECIMALV:
						target=Rrealloc(target,pos+field->len+4);
						sprintf(&target[pos],"\"%*f\" ",field->len,
							*field->data.float_value);
						pos+=field->len+3;
						break;
					case SHORTV:
					case SSHORTV:
						target=Rrealloc(target,pos+field->len+4);
						sprintf(&target[pos],"\"%*d\" ",field->len,
							*field->data.short_value);
						pos+=field->len+3;
						break;
					case LONGV:
					case SLONGV:
						target=Rrealloc(target,pos+field->len+4);
						sprintf(&target[pos],"\"%*d\" ",field->len,
							*field->data.integer_value);
						pos+=field->len+3;
						break;
					default:
						prterr("Error Field Type [%d] is invalid in Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",field->type,field->name,MODULENAME(fileno),FILENAME(fileno),line,file);
						break;
				}
			}
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG MAKEPASSABLEKEYS Returning Target Value [%s] for Module [%s] File [%s] Key [%s] to line [%d] program [%s].",target,MODULENAME(fileno),FILENAME(fileno),KEYNAME(fileno,keyno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return target;
}
short DELETENRDrda(char *module,char *filename)
{
	char *libx=NULL,*tmp=NULL;

	if(isEMPTY(module) || isEMPTY(filename)) return(-1);
	libx=Rmalloc(RDAstrlen(filename)+RDAstrlen(CURRENTDIRECTORY)+
		RDAstrlen(module)+3);
#ifndef WIN32
	sprintf(libx,"%s/%s/%s",CURRENTDIRECTORY,(module!=NULL ? module:""),(filename!=NULL ? filename:""));
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s\\%s",CURRENTDIRECTORY,(module!=NULL ? module:""),(filename!=NULL ? filename:""));
#endif
	tmp=Rmalloc(RDAstrlen(libx)+5);
	sprintf(tmp,"%s.dat",libx);
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
	sprintf(tmp,"%s.idx",libx);
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
short xDELETENRD(char *module,char *filename,short engine,char *serverx,int line,char *file)
{
	short return_value=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("DIAG DELETENRD Deleting Data File [%s][%s] on server [%s] at line [%d] program [%s].",module,filename,(serverx!=NULL ? serverx:""),line,file);
	} 
#endif /* USE_RDA_DIAGNOSTICS */
	switch(engine)
	{
		default:
		case NRDRdaEngine:
			return_value=DELETENRDrda(module,filename);
			break;
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:
			return_value=DELETENRDisam(module,filename);
			break;
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine:
			return_value=DELETENRDmyisam(module,filename);
			break;
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine:
		case NRDMySQLEngine:
		case NRDMySQLAutoEngine:
			return_value=DELETENRDmysql(serverx,module,filename);
			break;
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree:
			return_value=DELETENRDrda(module,filename);
			break;
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine:
		case NRDLocalCtreeEngine:
			return_value=DELETENRDrda(module,filename);
			break;
#endif
	}
	return(return_value);
}
APPlib *xMAKEPASSABLEKEYSAPPlib(short fileno,short keyno,void *keyvalue,
	int line,char *file)
{
	short x;
	char *target=NULL;
	NRDkey *key;
	NRDpart *part;
	NRDfield *field=NULL;
	APPlib *tmp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG MAKEPASSABLEKEYSAPPlib for Module [%s] File [%d] [%s] Key [%d] at line [%d] program [%s].",MODULENAME(fileno),fileno,FILENAME(fileno),keyno,line,file); }
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(NULL);
	tmp=APPlibNEW();
	if(!EQLNRDKEY(fileno,keyno,keyvalue))
	{
		key=NRDbuf[fileno].key+(keyno-1);
		for(x=0;x<key->numparts;++x)
		{
			part=key->part+x;
			if(part!=NULL && part->name!=NULL) 
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
						target=Rrealloc(target,RDAstrlen(field->data.string_value)+1);
						sprintf(target,"%s",field->data.string_value);
						break;
					case CHARACTERS:
					case BOOLNS:
						target=Rrealloc(target,2);
						if(field->type==BOOLNS)
						{
							sprintf(target,"%c",*field->data.string_value);
						} else sprintf(target,"%1d",*field->data.string_value);
						break;
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case SDECIMALV:
						target=Rrealloc(target,field->len+1);
						sprintf(target,"%*f",field->len,
							*field->data.float_value);
						break;
					case SHORTV:
					case SSHORTV:
						target=Rrealloc(target,field->len+1);
						sprintf(target,"%*d",field->len,
							*field->data.short_value);
						break;
					case LONGV:
					case SLONGV:
						target=Rrealloc(target,field->len+1);
						sprintf(target,"%*d",field->len,
							*field->data.integer_value);
						break;
					default:
						prterr("Error Field Type [%d] is invalid in Field [%s] in Module [%s] File [%s] at line [%d] program [%s].",field->type,field->name,MODULENAME(fileno),FILENAME(fileno),line,file);
						break;
				}
				addAPPlib(tmp,target);
			}
		}
	}
	if(target!=NULL) Rfree(target);
	return tmp;
}
short xFIELDCOPY(short ffileno,char *fieldname1,short tfileno,char *fieldname2,int line,char *file)
{
	NRDfield *ffield=NULL;
	NRDfield *tfield=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("DIAG FIELDCOPY Copying Field [%s] from Module [%s] File [%d] [%s] to field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",
			(fieldname1!=NULL ? fieldname1:"Null"),
			MODULENAME(ffileno),ffileno,FILENAME(ffileno),
			(fieldname2!=NULL ? fieldname2:"Null"),
			MODULENAME(tfileno),tfileno,FILENAME(tfileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(ffileno,line,file)) return(-1);
	if(!TESTFILENO(tfileno,line,file)) return(-1);
	ffield=FLDNRD(ffileno,fieldname1);
	tfield=FLDNRD(tfileno,fieldname2);
	if(ffield!=NULL && tfield!=NULL)
	{
		NRD2NRD(ffield,tfield);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagnrd || diagnrd_field)
		{
			prterr("DIAG FIELDCOPY Successful copy of field [%s] from Module [%s] File [%d] [%s] to field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname1,MODULENAME(ffileno),ffileno,FILENAME(ffileno),fieldname2,MODULENAME(tfileno),tfileno
,FILENAME(tfileno),line,file);
		}
#endif /* USE_RDA_DIAGNOSTICS */
		return(0);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("Error FIELDCOPY has failed trying to copy of field [%s] from Module [%s] File [%d] [%s] to field [%s] in Module [%s] File [%d] [%s] at line [%d] program [%s].",fieldname1,MODULENAME(ffileno),ffileno,FILENAME(ffileno),fieldname2,
			MODULENAME(tfileno),tfileno,FILENAME(tfileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(-1);
}
short xDELNRD(short fileno,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field) 
	{ 
		prterr("DIAG DELNRD Deleting Current Record in Module [%s] File [%d] [%s] at line [%d] program [%s].",MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file); 
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	switch(NRDbuf[fileno].db_engine)
	{
		case NRDRdaEngine:	return(DELNRDrda(fileno));
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:	return(DELNRDisam(fileno));
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine:
			return(DELNRDmyisam(fileno));
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine:
		case NRDMySQLEngine:
		case NRDMySQLAutoEngine:
			return(DELNRDmysql(fileno));
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree:		return(DELNRDberkeley(fileno));
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine:	return(DELNRDctree(fileno));
		case NRDLocalCtreeEngine:	return(DELNRDctree(fileno));
#endif
		default:
			prterr("Error Unknown Database Engine [%d] for Module [%s] File [%d] [%s] at line [%d] program [%s]",NRDbuf[fileno].db_engine,MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
			return(-1);
	}
}
short DELNRDrda(short fileno)
{
	NRDbuffer *NRDtmp;

	NRDtmp=NRDbuf+fileno;
	DELDTA(NRDtmp->dtanum,NRDtmp->currec);
	return(FALSE); /* to be done */
}
#ifdef USE_CTREE
short DELNRDctree(short fileno)
{
	NRDbuffer *NRDtmp;
	short return_value=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG DELNRDctree Deleting Current Record in Module [%s] File [%d] [%s].",MODULENAME(fileno),fileno,FILENAME(fileno)); }
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	RecordNRDconnect(NRDbuf[fileno].servername,NRDbuf[fileno].db_engine);
	if(NRDtmp->varfile==TRUE)
	{
		LKISAM(ENABLE);
		return_value=DELVREC(NRDtmp->filenum);
		LKISAM(FREE);
		return(return_value);
	} else {
		LKISAM(ENABLE);
		return_value=DELREC(NRDtmp->filenum);
		LKISAM(FREE);
		return(return_value);
	}
} 
#endif
short xGETBIN(char *module,char *name,short *engine,short *nofields,NRDfield **fields,short *nokeys,NRDkey **keys,short getserver,char **serverx,int line,char *file)
{
	short y,version,x,test;
	BIN *bin;
	NRDfield *fld;
	NRDkey *key;
	NRDpart *part;
	char *temp=NULL,*libname=NULL,*dashes=NULL;
	char *tmp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field) { prterr("DIAG GETBIN Reading Database Engine Definition for Module [%s] File [%s] at line [%d] program [%s].",module,name,line,file); }
#endif /* USE_RDA_DIAGNOSTICS */
	if(NONTRADITIONAL_EXECUTION==FALSE || RDAstrcmp(module,"RPTGEN"))
	{
		dashes=adddashes(module);
		libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(dashes)+10);
#ifndef WIN32
		sprintf(libname,"%s/rda/%s.FIL",CURRENTDIRECTORY,dashes);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,dashes);
#endif
		if(dashes!=NULL) Rfree(dashes);		
	} else {
		dashes=adddashes(module);
		libname=Rmalloc(RDAstrlen(dashes)+40);
#ifndef WIN32
		sprintf(libname,"/tmp/%s.FIL",dashes);
#endif
#ifdef WIN32
		sprintf(libname,"c:\\temp\\%s.FIL",dashes);
#endif
		if(dashes!=NULL) Rfree(dashes);		
	}
	bin=getlibbin(libname,name,TRUE);
	if(bin==NULL)
	{
		prterr("Error can't read Database Definition [%s] [%s] from Binary Library [%s] at line [%d] program [%s].",module,name,libname,line,file);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	if(libname!=NULL) Rfree(libname);
	x=BINgetshort(bin);
	if(x!=(-1701) && x!=(-1700) && x!=(-1699))
	{
		prterr("Error invalid version or bad binary for Module [%s] File [%s] at line [%d] program [%s].",module,name,line,file);
		return(-1);
	}
	if(x==(-1699)) version=2;
	else if(x==(-1700)) version=TRUE;
	else version=FALSE;
	*engine=BINgetshort(bin);
	if(version>1)
	{
		test=BINgetshort(bin);
		if(getserver)
		{
			if(*serverx!=NULL) Rfree(*serverx);
			*serverx=NULL;
		}
		if(test) 
		{
			if(getserver)
			{
				*serverx=BINgetstring(bin);
				if(isEMPTY(*serverx))
				{
					if(*serverx!=NULL) Rfree(*serverx);
					tmp=RDA_GetEnv("CTREE_DEFAULT_SERVER");
					if(isEMPTY(tmp))
					{
						*serverx=NULL;
					} else {
						*serverx=stralloc(tmp);
					}
					tmp=NULL;
				}
			} else {
				temp=BINgetstring(bin);
				if(temp!=NULL) Rfree(temp);
			}
		} else {
		}
	} else {
		if(getserver)
		{
			if(*serverx!=NULL) Rfree(*serverx);
			*serverx=NULL;
		}
	}
	*nofields=BINgetshort(bin);
	if(*fields!=NULL) *fields=Rrealloc(*fields,*nofields*sizeof(NRDfield));
		else *fields=Rmalloc(*nofields*sizeof(NRDfield));
	*nokeys=BINgetshort(bin);
	if(*keys!=NULL) *keys=Rrealloc(*keys,*nokeys*sizeof(NRDkey));
		else *keys=Rmalloc(*nokeys*sizeof(NRDkey));
	for(x=0,fld=*fields;x<*nofields;++x,++fld)
	{
		fld->name=BINgetstring(bin);
		fld->type=BINgetshort(bin);
		if(version) fld->len=BINgetint(bin);
			else fld->len=BINgetshort(bin);
	}
	for(x=0,key=*keys;x<*nokeys;++x,++key)
	{
		key->name=BINgetstring(bin);
		key->numparts=BINgetshort(bin);
		key->part=Rmalloc(sizeof(NRDpart)*key->numparts);
		for(y=0,part=key->part;y<key->numparts;++y,++part)
		{
			part->offset=BINgetshort(bin);
			part->name=BINgetstring(bin);
		}
	}
	BINfree(bin);
	return(0);
}
NRDbuffer *xNRDtmpGETBIN(char *module,char *name,int line,char *file)
{
	NRDbuffer *tmp=NULL;
	short engine=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG NRDtmpGETBIN at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(NRDbuffer));
	tmp->modname=stralloc(module);
	tmp->name=stralloc(name);
	tmp->field=NULL;
	tmp->nofields=0;
	tmp->key=NULL;
	tmp->nokeys=0;
	tmp->servername=NULL;
	if(ADVGETBIN(tmp->modname,tmp->name,&engine,&tmp->nofields,&tmp->field,
		&tmp->nokeys,&tmp->key,&tmp->servername))
	{
		if(tmp!=NULL)
		{
			if(tmp->modname!=NULL) Rfree(tmp->modname);
			if(tmp->name!=NULL) Rfree(tmp->name);
			Rfree(tmp);
			return(NULL);
		}
	}
	tmp->db_engine=engine;
	tmp->dtanum=NULL;
	tmp->currec=0;
	tmp->record_data=NULL;
#ifdef USE_BERKELEY_BTREE
	tmp->dbp=NULL;
#endif
	tmp->dlen=0;
	return(tmp);
}
void xRCKNRD(short fileno,int line,char *file)
{
/* toggling locks for multi-user mode */
	UNLNRDFILE(fileno);
	LOCNRDFILE(fileno);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field) 
	{ 
		prterr("DIAG RCKNRD Rebuild Node on Module [%s] File [%d] [%s].",MODULENAME(fileno),fileno,FILENAME(fileno));
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return;
	switch(NRDbuf[fileno].db_engine)
	{
		case NRDRdaEngine: 
			RCKDTA(NRDbuf[fileno].dtanum);
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
			prterr("Error Unknown Database Engine [%d] for Module [%s] File [%d] [%s] at line [%d] program [%s]",NRDbuf[fileno].db_engine,MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
			break;
	}
/* toggling locks for multi-user mode */
	UNLNRDFILE(fileno);
	LOCNRDFILE(fileno);
}
short xBEGNRD(short fileno,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field) { prterr("DIAG BEGNRD Searching for First Physical Record in Module [%s] File [%d] [%s] at line [%d] program [%s].",MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file); }
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	switch(NRDbuf[fileno].db_engine)
	{
		case NRDRdaEngine:	return(BEGNRDrda(fileno));
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:	return(BEGNRDisam(fileno));
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine:
			return(BEGNRDmyisam(fileno));
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine:
		case NRDMySQLEngine:
		case NRDMySQLAutoEngine:
			return(BEGNRDmysql(fileno));
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree:		return(BEGNRDberkeley(fileno));
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine:	return(BEGNRDctree(fileno));
		case NRDLocalCtreeEngine:	return(BEGNRDctree(fileno));
#endif
		default:
			prterr("Error Unknown Database Engine [%d] for Module [%s] File [%d] [%s] at line [%d] program [%s]",NRDbuf[fileno].db_engine,MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
			return(-1);
	}
}
short BEGNRDrda(short fileno)
{
	short retval=0;
	NRDbuffer *NRDtmp;
	DTA y;

	NRDtmp=NRDbuf+fileno;
	y=NRDbuf[fileno].dtanum;
	retval=BEGDTA(y);
	if(!retval) 
	{
		NRDtmp->currec=y->cur;
		assign_rdafields(NRDtmp);
	}
	return(retval);
}
#ifdef USE_CTREE
short BEGNRDctree(short fileno)
{
	LONG return_value=0;
	VRLEN varlen;
	NRDbuffer *NRDtmp;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG Searching for first record in sorted sequence using C-tree Engine."); }
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	RecordNRDconnect(NRDtmp->servername,NRDtmp->db_engine);
/* set current position in data file to 0 */
	SETCURI(NRDtmp->filenum,return_value,NULL,0);
	if(NRDtmp->record_data==NULL)
	{
		varlen=getdreclen(NRDtmp);
		QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,varlen);
	}
	return_value=FRSREC(NRDtmp->filenum,NRDtmp->record_data);
	if(!return_value)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagnrd) { prterr("DIAG Found key in search"); }
#endif /* USE_RDA_DIAGNOSTICS */
		if(NRDtmp->varfile==TRUE)
		{
			varlen=GETVLEN(NRDtmp->filenum);
			QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,varlen+1);
			return_value=REDVREC(NRDtmp->filenum,
				NRDtmp->record_data,varlen);
		}
		if(return_value)
		{
			prterr("Error in C-tree Database [%s] for Module [%s] File [%d] [%s].",ctdb_errors(errno),NRDtmp->modname,fileno,NRDtmp->name);
			return(-1);
		} 
		assignCtree_fields(NRDtmp);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagnrd)
		{ 
			xSEENRDRECORD(fileno,__LINE__,__FILE__);
			prterr("Exiting FRSNRDctree OK.");
		}
#endif /* USE_RDA_DIAGNOSTICS */
		return(0);
	} else if(return_value!=(101))
	{
		prterr("Error in C-tree Database [%s] for Module [%s] File [%d] [%s].",ctdb_errors(errno),NRDtmp->modname,fileno,NRDtmp->name);
		return(-1);
	} else return(1);
}
#endif
short xSEQNRD(short fileno,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field) { prterr("DIAG SEQNRD Searching for Next Physical Record in Module [%s] File [%d] [%s] at line [%d] program [%s].",MODULENAME(fileno),fileno,NRDbuf[fileno].name,line,file); }
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	switch(NRDbuf[fileno].db_engine)
	{
		case NRDRdaEngine:	return(SEQNRDrda(fileno));
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:	return(SEQNRDisam(fileno));
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine:
			return(SEQNRDmyisam(fileno));
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine:
		case NRDMySQLEngine:
		case NRDMySQLAutoEngine:
			return(SEQNRDmysql(fileno));
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree:		return(SEQNRDberkeley(fileno));
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine:	return(SEQNRDctree(fileno));
		case NRDLocalCtreeEngine:	return(SEQNRDctree(fileno));
#endif
		default:
			prterr("Error Unknown Database Engine [%d] for Module [%s] File [%d] [%s] at line [%d] program [%s]",NRDbuf[fileno].db_engine,MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
			return(-1);
	}
}
short SEQNRDrda(short fileno)
{
	short retval=0;
	NRDbuffer *NRDtmp;
	DTA y;

	NRDtmp=NRDbuf+fileno;
	y=NRDbuf[fileno].dtanum;
	retval=SEQDTA(y);
	if(!retval) 
	{
		NRDtmp->currec=y->cur;
		assign_rdafields(NRDtmp);
	}
	return(retval);
}
#ifdef USE_CTREE
short SEQNRDctree(short fileno)
{
	LONG return_value=0;
	VRLEN varlen;
	NRDbuffer *NRDtmp;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG Searching for first record in sorted sequence using C-tree Engine."); }
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	RecordNRDconnect(NRDtmp->servername,NRDtmp->db_engine);
	return_value=NXTREC(NRDtmp->filenum,NRDtmp->record_data);
	if(!return_value)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagnrd) { prterr("DIAG Found key in search"); }
#endif /* USE_RDA_DIAGNOSTICS */
		if(NRDtmp->varfile==TRUE)
		{
			varlen=GETVLEN(NRDtmp->filenum);
			QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,varlen+1);
			return_value=REDVREC(NRDtmp->filenum,
				NRDtmp->record_data,varlen);
		}
		if(return_value)
		{
			prterr("Error in C-tree Database [%s] for Module [%s] File [%d] [%s].",ctdb_errors(errno),NRDtmp->modname,fileno,NRDtmp->name);
			return(-1);
		} 
		assignCtree_fields(NRDtmp);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagnrd)
		{ 
			xSEENRDRECORD(fileno,__LINE__,__FILE__);
			prterr("Exiting FRSNRDctree OK.");
		}
#endif /* USE_RDA_DIAGNOSTICS */
		return(0);
	} else if(return_value!=(101))
	{
		prterr("Error in C-tree Database [%s] for Module [%s] File [%d] [%s].",ctdb_errors(errno),NRDtmp->modname,fileno,NRDtmp->name);
		return(-1);
	} else return(1);
}
#endif
short xNRD2NRD(NRDfield *fld,NRDfield *field,int line,char *file)
{
	double hold=0.0;
	long hold1=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("DIAG NRD2NRD Copying Field [%s]'s value to Field [%s] at line [%d] program [%s].",fld->name,field->name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
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
					if(fld->data.string_value!=NULL && fld->dlen>0)
					{
						QUICKALLOC(field->data.string_value,field->dlen,
							fld->dlen);
						memcpy(field->data.string_value,fld->data.string_value,fld->dlen);
						if(field->dlen>field->len && field->len>0)
							field->data.string_value[field->len]=0;
					} else if(field->dlen>0)
					{
						memset(field->data.string_value,0,field->dlen);
					} else {
						field->data.string_value=NULL;
						field->dlen=0;
					}
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
					prterr("Error Incompatiable Field Type [%d] in Fields [%s] and Type [%d] in [%s] at line [%d] program [%s].",fld->type,fld->name,field->type,field->name,line,file);
					return(-1);
				default:
					prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",fld->type,fld->name,line,file);
					return(-1);
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
					prterr("Error Incompatiable Field Type [%d] in Fields [%s] and Type [%d] in [%s] at line [%d] program [%s].",fld->type,fld->name,field->type,field->name,line,file);
					return(-1);
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
/*lint -e524 */
					*field->data.string_value=*fld->data.float_value;
/*lint +e524 */
					break;
				case SHORTV:
				case SSHORTV:
/*lint -e524 */
					*field->data.string_value=*fld->data.short_value;
/*lint +e524 */
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
/*lint -e524 */
					*field->data.string_value=*fld->data.integer_value;
/*lint +e524 */
					break;
				case CHARACTERS:
				case BOOLNS:
/*lint -e524 */
					*field->data.string_value=*fld->data.string_value;
/*lint +e524 */
					break;
				default:
					prterr("Error Incompatible Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",fld->type,fld->name,line,file);
					return(-1);
			}
			break;
		case DOLLARS:
		case DOLLARS_NOCENTS:
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
					prterr("Error Incompatiable Field Type [%d] in Fields [%s] and Type [%d] in [%s] at line [%d] program [%s].",fld->type,fld->name,field->type,field->name,line,file);
					return(-1);
				case CHARACTERS:
				case BOOLNS:
					*field->data.float_value=*fld->data.string_value;
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					if(fld->type==DOLLARS && field->type==DOLLARS_NOCENTS)
					{
						hold1=(*fld->data.float_value/100);
						if(hold1<1 && hold1>(-1))
						{
							*field->data.float_value=0;
						} else *field->data.float_value=hold1;
					} else if(fld->type==DOLLARS_NOCENTS &&
						field->type==DOLLARS)
					{
						hold=*fld->data.float_value;
						if(hold<1 && hold>(-1))
						{
							*field->data.float_value=0;
						} else *field->data.float_value=hold*100;
					} else {
						hold=*fld->data.float_value;
						if(hold<1 && hold>(-1))
						{
							*field->data.float_value=0;
						} else *field->data.float_value=hold;
					}
					break;
				case SHORTV:
				case SSHORTV:
					hold=*fld->data.short_value;
					if(hold<1 && hold>(-1))
					{
						*field->data.float_value=0;
					} else *field->data.float_value=hold;
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					hold=*fld->data.integer_value;
					if(hold<1 && hold>(-1))
					{
						*field->data.float_value=0;
					} else *field->data.float_value=hold;
					break;
				default:
					prterr("Error Incompatible Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",fld->type,fld->name,line,file);
					return(-1);
			}
			break;
		case DECIMALV:
		case DOUBLEV:
		case SDOUBLEV:
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
					prterr("Error Incompatiable Field Type [%d] in Fields [%s] and Type [%d] in [%s] at line [%d] program [%s].",fld->type,fld->name,field->type,field->name,line,file);
					return(-1);
				case CHARACTERS:
				case BOOLNS:
					*field->data.float_value=*fld->data.string_value;
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
					if(fld->type==DOLLARS && field->type==DOLLARS_NOCENTS)
					{
						hold=*fld->data.float_value/100;
						if(hold==0)
						{
							*field->data.float_value=0.0;
						} else *field->data.float_value=hold;
					} else if(fld->type==DOLLARS_NOCENTS &&
						field->type==DOLLARS)
					{
						hold=*fld->data.float_value*100;
						if(hold==0)
						{
							*field->data.float_value=0.0;
						} else *field->data.float_value=hold;
					} else {
						hold=*fld->data.float_value;
						if(hold==0)
						{
							*field->data.float_value=0.0;
						} else *field->data.float_value=hold;
					}
					break;
				case SHORTV:
				case SSHORTV:
					hold=*fld->data.short_value;
					if(hold==0)
					{
						*field->data.float_value=0.0;
					} else *field->data.float_value=hold;
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					hold=*fld->data.integer_value;
					if(hold==0)
					{
						*field->data.float_value=0.0;
					} else *field->data.float_value=hold;
					break;
				default:
					prterr("Error Incompatible Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",fld->type,fld->name,line,file);
					return(-1);
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
					prterr("Error Incompatiable Field Type [%d] in Fields [%s] and Type [%d] in [%s] at line [%d] program [%s].",fld->type,fld->name,field->type,field->name,line,file);
					return(-1);
				case CHARACTERS:
				case BOOLNS:
					*field->data.short_value=*fld->data.string_value;
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
/*lint -e524 */
					*field->data.short_value=*fld->data.float_value;
/*lint +e524 */
					break;
				case SHORTV:
				case SSHORTV:
					*field->data.short_value=*fld->data.short_value;
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
/*lint -e524 */
					*field->data.short_value=*fld->data.integer_value;
/*lint +e524 */
					break;
				default:
					prterr("Error Incompatible Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",fld->type,fld->name,line,file);
					return(-1);
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
					prterr("Error Incompatiable Field Type [%d] in Fields [%s] and Type [%d] in [%s] at line [%d] program [%s].",fld->type,fld->name,field->type,field->name,line,file);
					return(-1);
				case CHARACTERS:
				case BOOLNS:
					*field->data.integer_value=*fld->data.string_value;
					break;
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case SDECIMALV:
/*lint -e524 */
					*field->data.integer_value=*fld->data.float_value;
/*lint +e524 */
					break;
				case SHORTV:
				case SSHORTV:
					*field->data.integer_value=*fld->data.short_value;
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					*field->data.integer_value=*fld->data.integer_value;
					break;
				default:
					prterr("Error Incompatible Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",fld->type,fld->name,line,file);
					return(-1);
			}
			break;
		default:
			prterr("Error Field Type [%d] is invalid in Field [%s] at line [%d] program [%s].",field->type,field->name,line,file);
			return(-1);
	}
	return(0);
}
