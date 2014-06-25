#include <nrdp.hpp>

#ifdef UNIXWARE7
#include <errno.h>
#endif
#ifdef UNIXWARE21
#include <errno.h>
#endif
#ifdef USE_CTREE
#include<ctport.h>
#endif

char UseRamDisk=FALSE;

short no_buffers=0; /* represents the number of NRDbuffers */
NRDbuffer *NRDbuf=NULL; /* dynamically arrayed structure for all files(buffers) */

short xCRTNRD(char *module,short dbengine,char *name,short nofields,NRDfield *field,short nokeys,NRDkey *key,char *serverx,int line,char *file)
{
	short nrdno=0,x=0;
	NRDbuffer *NRDtmp;
	DTA dtanum=NULL;
	short return_value=(-1);

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field)
	{
		prterr("DIAG CRTNRD Creating Physical Datafile for Module [%s] File [%s] Engine [%d] # Fields [%d] # Keys [%d] at line [%d] program [%s].",module,name,dbengine,nofields,nokeys,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(putnrd(module,name,dbengine,nofields,field,nokeys,key,serverx))
	{
		prterr("Error creating File Definition [%s] [%s] at line [%d] program [%s].  Check permissions and existance of Binary Library [%s.FIL].",module,name,line,file,module);
		return(-1);
	}
	x=getnrd(module,name);
	if(x<0)
	{
		prterr("Error can't create File Definition [%s] [%s] at line [%d] program [%s].  Verify Binary Library [%s.RPT].",module,name,line,file,module);
		return(-1);
	}
	NRDtmp=NRDbuf+x; 
	nrdno=x;
	switch(NRDtmp->db_engine)
	{
		case NRDRdaEngine:
			dtanum=CRTNRDrda(NRDtmp);
			if(dtanum==NULL)
			{
				prterr("Error can't create Actual Data File [%s] [%s] using RDA Database at line [%d] program [%s].",module,name,line,file);
				freenrd(NRDtmp);
				return(-1);
			}
			break;
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:
			x=CRTNRDisam(NRDtmp);
			if(x<0)
			{
				prterr("Error can't create Actual Data File [%s] [%s] using Informix-OnLine Database at line [%d] program [%s].",module,name,line,file);
				freenrd(NRDtmp);
				return(-1);
			}
			break;
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine:
			x=CRTNRDmyisam(NRDtmp);
			if(x<0)
			{
				prterr("Error can't create Actual Data File [%s] [%s] using MyISAM Database at line [%d] program [%s].",module,name,line,file);
				freenrd(NRDtmp);
				return(-1);
			}
			break;
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine:
			x=CRTNRDmysqlxpert(NRDtmp);
			if(x<0)
			{
				prterr("Error can't create Actual Data File [%s] [%s] using MySQL (Xpert) Database at line [%d] program [%s].",module,name,line,file);
				freenrd(NRDtmp);
				return(-1);
			}
			break;
		case NRDMySQLEngine:
		case NRDMySQLAutoEngine:
			x=CRTNRDmysql(NRDtmp);
			if(x<0)
			{
				prterr("Error can't create Actual Data File [%s] [%s] using MySQL Database at line [%d] program [%s].",module,name,line,file);
				freenrd(NRDtmp);
				return(-1);
			}
			break;
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree:
			x=CRTNRDberkeley(NRDtmp);
			if(x<0)
			{
				prterr("Error can't create Actual Data File [%s] [%s] using Berkeley Database at line [%d] program [%s].",module,name,line,file);
				freenrd(NRDtmp);
				return(-1);
			}
			break;
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine:
		case NRDLocalCtreeEngine:
			x=CRTNRDctree(NRDtmp);
			if(x<0)
			{
				prterr("Error [%d] [%s] can't create Actual Data File [%s] [%s] using C-tree Database at line [%d] program [%s].",x,ctdb_errors(errno),module,name,line,file);
				freenrd(NRDtmp);
				return(-1);
			}
			NRDtmp->varfile=VARRECORDS(nrdno);
			break;
#endif
		default: 
			prterr("Error Unknown Database Engine [%d] for Module [%s] File [%s] at line [%d] program [%s].  Can't create datafile.",NRDtmp->db_engine,module,name,line,file);
			freenrd(NRDtmp);
			return(-1);
	}
	freenrd(NRDtmp);
	return_value=OPNNRD(module,name);
	return(return_value);
}
short checkvariable(NRDbuffer *NRDtmp)
{
	NRDfield *field;
	int x;

	if(NRDtmp->field)
	{
		for(x=0,field=NRDtmp->field;x<NRDtmp->nofields;++x,++field)
		{
			if(field->type==VARIABLETEXT || field->type==SCROLLEDTEXT || field->len==0) 
			{
				return(TRUE);
			}
		}
	}
	return(FALSE);
}
DTA CRTNRDrda(NRDbuffer *NRDtmp)
{
	short x,y,z,branch_factor=1;
	DTA tmp=NULL;
	KEY rdakey;
	NRDfield *field;
	NRDkey *key;
	NRDpart *part;
	char *fullname=NULL,*filename=NULL,*keyfile=NULL;
	ftype type;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG CRTNRDrda Creating RDA Database Data & Index Files for [%s] [%s].",NRDtmp->modname,NRDtmp->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	fullname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(NRDtmp->name)+RDAstrlen(NRDtmp->modname)+3);
#ifndef WIN32
	sprintf(fullname,"%s/%s/%s",CURRENTDIRECTORY,NRDtmp->modname,
		NRDtmp->name);
#endif
#ifdef WIN32
	sprintf(fullname,"%s\\%s\\%s",CURRENTDIRECTORY,NRDtmp->modname,
		NRDtmp->name);
#endif
	filename=Rmalloc(RDAstrlen(NRDtmp->name)+RDAstrlen(NRDtmp->modname)+6);
#ifndef WIN32
	sprintf(filename,"%s/%s.dat",NRDtmp->modname,NRDtmp->name);
#endif
#ifdef WIN32
	sprintf(filename,"%s\\%s.dat",NRDtmp->modname,NRDtmp->name);
#endif

	keyfile=Rmalloc(RDAstrlen(NRDtmp->name)+RDAstrlen(NRDtmp->modname)+6);
#ifndef WIN32
	sprintf(keyfile,"%s/%s.idx",NRDtmp->modname,NRDtmp->name);
#endif
#ifdef WIN32
	sprintf(keyfile,"%s\\%s.idx",NRDtmp->modname,NRDtmp->name);
#endif
	tmp=NEWDTA(fullname,filename,keyfile);
	if(fullname!=NULL) Rfree(fullname);
	if(filename!=NULL) Rfree(filename);
	if(keyfile!=NULL) Rfree(keyfile);
	for(x=0,field=NRDtmp->field;x<NRDtmp->nofields;++x,++field)
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
/*lint -e64 */
				/* type=1; */
/*lint +e64 */
				type=ftchar;
				NEWFLD(tmp,field->name,type,0);
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
/*lint -e64 */
				/* type=5; */
/*lint +e64 */
				type=ftdouble;
				NEWFLD(tmp,field->name,type,1);
				break;
			case SHORTV:
			case SSHORTV:
/*lint -e64 */
				/* type=2; */
/*lint +e64 */
				type=ftshort;
				NEWFLD(tmp,field->name,type,1);
				break;
			case LONGV:
			case SLONGV:
/*lint -e64 */
				/* type=3; */
/*lint +e64 */
				type=ftint;
				NEWFLD(tmp,field->name,type,1);
				break;
			case BOOLNS:
			case CHARACTERS:
/*lint -e64 */
				/* type=1; */
/*lint +e64 */
				type=ftchar;
				if(field->len==0) field->len=1;
				NEWFLD(tmp,field->name,type,1);
				break;
			default:
				prterr("Error Field Type [%d] is invalid for Field [%s] in Module [%s] File [%s].",field->type,field->name,NRDtmp->modname,NRDtmp->name);
				break;
		}
	}
	for(x=0,key=NRDtmp->key;x<NRDtmp->nokeys;++x,++key)
	{
		NEWKEY(tmp,key->name,branch_factor);
		rdakey=tmp->key+x;
		for(y=0,part=key->part;y<key->numparts;++y,++part)
		{
			for(z=0,field=NRDtmp->field;z<NRDtmp->nofields;++z,++field)
			{
				if(!RDAstrcmp(field->name,part->name)) break;
			} 
			if(z<NRDtmp->nofields)
			{
				NEWELE(rdakey,z,FALSE);
			} else {
				prterr("Error Key part [%s] not found in field list for File Definition [%s] [%s].",part->name,NRDtmp->modname,NRDtmp->name);
			}
		}
	}
	if(CRTDTA(tmp)) 
	{
		CLSDTA(tmp);
		tmp=NULL;
	}
	return(tmp);
}
#ifdef USE_CTREE
void xShowDODA(int c,DATOBJ *doda,short which,int line,char *file)
{
	int x;
	DATOBJ *d=NULL;

	if(doda!=NULL)
	{
		prterr("DODA has %d objects at line [%d] program [%s].\n",c,line,file);
		for(x=0,d=doda;x<c;++x,++d)
		{
			if(which)
			{
				prterr("\tName: %s Type: %d Length: %d Offset %d\n",
					(d->fsymb!=NULL ? d->fsymb:""),d->ftype,d->flen,
					*(short *)d->fadr); 
			} else {
				prterr("\tName: %s Type: %d Length: %d \n",
					(d->fsymb!=NULL ? d->fsymb:""),d->ftype,d->flen);
			}
		}
	} else {
		prterr("DODA is NULL at line [%d] program [%s].\n",line,file);
	}
}
static void MakeDODA(NRDbuffer *NRDtmp)
{
	DATOBJ *d=NULL,*doda=NULL;
	int x=0,pos=0;
	NRDfield *f=NULL;

	if(NRDtmp!=NULL)
	{
		if(NRDtmp->field!=NULL)
		{
			doda=Rmalloc((NRDtmp->nofields)*sizeof(DATOBJ));
			memset(doda,0,(NRDtmp->nofields)*sizeof(DATOBJ));
			pos=0;
			for(x=0,d=doda,f=NRDtmp->field;x<NRDtmp->nofields;++x,++d,++f)
			{
				d->fsymb=f->name;
				switch(f->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
						d->ftype=CT_STRING;
						d->flen=f->len;
						pos=(f->len+1);
						break;
					case ZIPCODE:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PHONE:
					case CUSTOMTYPE:
					case PLAINTEXT:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						if(f->len>0)
						{
							d->ftype=CT_FSTRING;
							d->flen=f->len;
							pos+=f->len;
						} else {
							d->ftype=CT_STRING;
							d->flen=f->len;
							pos+=f->len+1;
						}
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DOUBLEV:
					case SDOUBLEV:
					case DECIMALV:
					case SDECIMALV:
						d->ftype=CT_DFLOAT;
						d->flen=sizeof(double);
						pos+=d->flen;
						break;
					case SHORTV:
					case SSHORTV:
						d->ftype=CT_INT2;
						d->flen=sizeof(short);
						pos+=d->flen;
						break;
					case LONGV:
					case SLONGV:
						d->ftype=CT_INT4;
						d->flen=sizeof(int);
						pos+=d->flen;
						break;
					case BOOLNS:
						d->ftype=CT_CHAR;
						d->flen=1;
						pos+=d->flen;
						break;
					case CHARACTERS:
						d->ftype=CT_CHAR;
						d->flen=1;
						pos+=d->flen;
						break;
					default:
						prterr("Error Field Type [%d] is invalid for Field [%s] in Module [%s] File [%s].",f->type,f->name,NRDtmp->modname,NRDtmp->name);
						break;
				}
			}
			if(NRDtmp->record_data!=NULL) Rfree(NRDtmp->record_data);
			NRDtmp->record_data=Rmalloc(pos+1);
			pos=0;
			for(x=0,d=doda,f=NRDtmp->field;x<NRDtmp->nofields;++x,++d,++f)
			{
				d->fsymb=f->name;
				d->fadr=NRDtmp->record_data+pos;
				switch(f->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
						d->ftype=CT_STRING;
						d->flen=f->len;
						pos=(f->len+1);
						break;
					case ZIPCODE:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PHONE:
					case CUSTOMTYPE:
					case PLAINTEXT:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						if(f->len>0)
						{
							d->ftype=CT_FSTRING;
							d->flen=f->len;
							pos+=f->len;
						} else {
							d->ftype=CT_STRING;
							d->flen=f->len;
							pos+=f->len+1;
						}
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DOUBLEV:
					case SDOUBLEV:
					case DECIMALV:
					case SDECIMALV:
						d->ftype=CT_DFLOAT;
						d->flen=sizeof(double);
						pos+=d->flen;
						break;
					case SHORTV:
					case SSHORTV:
						d->ftype=CT_INT2;
						d->flen=sizeof(short);
						pos+=d->flen;
						break;
					case LONGV:
					case SLONGV:
						d->ftype=CT_INT4;
						d->flen=sizeof(int);
						pos+=d->flen;
						break;
					case BOOLNS:
						d->ftype=CT_CHAR;
						d->flen=1;
						pos+=d->flen;
						break;
					case CHARACTERS:
						d->ftype=CT_CHAR;
						d->flen=1;
						pos+=d->flen;
						break;
					default:
						prterr("Error Field Type [%d] is invalid for Field [%s] in Module [%s] File [%s].",f->type,f->name,NRDtmp->modname,NRDtmp->name);
						break;
				}
			}
			x=PUTDODA(NRDtmp->filenum,doda,NRDtmp->nofields);
			if(x)
			{
				prterr("Error PUTDODA [%d] for file [%s][%s].",x,NRDtmp->modname,NRDtmp->name);
			} else {
				PUTHDR(NRDtmp->filenum,(LONG)2,ctALIGNhdr);
			}
			if(doda!=NULL) Rfree(doda);
		}
	}
}

short CRTNRDctree(NRDbuffer *NRDtmp)
{
	short x,y,return_value=0;
	NRDfield *field=NULL;
	NRDkey *key;
	NRDpart *part;
	IFIL filedef;
	IIDX *index;
	ISEG *element;
	char *fullname=NULL,*odir=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG CRTNRDctree Creating C-tree Data and Index Files for Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	RecordNRDconnect(NRDtmp->servername,NRDtmp->db_engine);
	fullname=Rmalloc(RDAstrlen(NRDtmp->name)+RDAstrlen(NRDtmp->modname)+4);
#ifdef XXXX
#ifndef WIN32
	sprintf(fullname,"./%s/%s",NRDtmp->modname,NRDtmp->name);
#endif
#ifdef WIN32
	sprintf(fullname,".\\%s\\%s",NRDtmp->modname,NRDtmp->name);
#endif
#endif /* XXXX */
#ifndef WIN32
	sprintf(fullname,"%s",NRDtmp->name);
#endif
#ifdef WIN32
	sprintf(fullname,"%s",NRDtmp->name);
#endif
	filedef.pfilnam=adddashes(fullname);
	if(fullname!=NULL) Rfree(fullname);
	filedef.dxtdsiz=4096;
#ifdef XXXX
	if(checkvariable(NRDtmp))
	{
		filedef.dfilmod=(EXCLUSIVE|CHECKLOCK|VLENGTH);
	} else {
		filedef.dfilmod=(EXCLUSIVE|CHECKLOCK|ctFIXED);
	}
	if(NRDtmp->db_engine==NRDCtreeEngine)
	{
		filedef.ifilmod=EXCLUSIVE|DUPCHANEL;
	} else {
		filedef.ifilmod=EXCLUSIVE;
	}
#endif /* XXXX */
	if(checkvariable(NRDtmp))
	{
		filedef.dfilmod=(SHARED|CHECKLOCK|VLENGTH);
	} else {
		filedef.dfilmod=(SHARED|CHECKLOCK|ctFIXED);
	}
	if(NRDtmp->db_engine==NRDCtreeEngine)
	{
		filedef.ifilmod=SHARED|DUPCHANEL;
	} else {
		filedef.ifilmod=SHARED;
	}
	filedef.ixtdsiz=4096;
	filedef.dreclen=getdreclen(NRDtmp);
	while(filedef.dreclen>filedef.dxtdsiz) filedef.dxtdsiz+=4096;
	filedef.dnumidx=NRDtmp->nokeys;
	field=NRDtmp->field+0;
	filedef.rfstfld=stralloc(field->name);
	field=NRDtmp->field+(NRDtmp->nofields-1);
	filedef.rlstfld=stralloc(field->name);
	filedef.ix=Rmalloc(NRDtmp->nokeys*sizeof(IIDX));
	for(x=0,index=filedef.ix+x,key=NRDtmp->key+x;x<NRDtmp->nokeys;++x,++index,++key)
	{
		index->inumseg=key->numparts;
		index->ikeytyp=0;
		index->ikeydup=0;
		index->inulkey=0;
		index->iempchr=32;
		index->ikeylen=0;
		index->ridxnam=stralloc(key->name);
/* create ISEG structures */
		key->length=0;
		index->seg=Rmalloc(key->numparts*sizeof(ISEG));
		for(y=0,part=key->part+y,element=index->seg+y;y<key->numparts;++y,++part,++element)
		{
			element->soffset=part->offset;
			field=xFINDFLD(NRDtmp,part->name,__LINE__,__FILE__);
			if(field!=NULL)
			{
				switch(field->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
						filedef.ifilmod=SHARED|DUPCHANEL|VLENGTH;
						element->segmode=VARSEG;
						element->slength=get_length(NRDtmp,part->name);
						if(element->slength==0)
						{
							prterr("Error FIeld [%s] has 0 length...",part->name);
						}
						index->ikeylen+=element->slength;
						key->length+=element->slength;
						break;
					case ZIPCODE:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PHONE:
					case CUSTOMTYPE:
					case PLAINTEXT:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						element->segmode=REGSEG;
						element->slength=get_length(NRDtmp,part->name);
						if(element->slength<=0) 
						{
							element->segmode=VARSEG;
							element->slength=0;
							filedef.ifilmod=SHARED|DUPCHANEL|VLENGTH;
							prterr("Error FIeld [%s] has 0 length...",part->name);
						}
						index->ikeylen+=element->slength;
						key->length+=element->slength;
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DOUBLEV:
					case SDOUBLEV:
					case DECIMALV:
					case SDECIMALV:
						element->slength=sizeof(double);
						element->segmode=FLTSEG;
						index->ikeylen+=element->slength;
						key->length+=element->slength;
						break;
					case SHORTV:
					case SSHORTV:
						element->slength=sizeof(short);
						element->segmode=SGNSEG; 
						index->ikeylen+=element->slength;
						key->length+=element->slength;
						break;
					case LONGV:
					case SLONGV:
						element->slength=sizeof(int);
						element->segmode=SGNSEG;
						index->ikeylen+=element->slength;
						key->length+=element->slength;
						break;
					case BOOLNS:
					case CHARACTERS:
						element->slength=1;
						element->segmode=REGSEG;
						index->ikeylen+=element->slength;
						key->length+=element->slength;
						break;
					default:
						prterr("Error Field Type [%d] is invalid for Field [%s] in Module [%s] File [%s].",field->type,field->name,NRDtmp->modname,NRDtmp->name);
						break;
				}
			} else {
				prterr("Error Key Field Part [%s] in key [%s] does not exist as a valid field in the file definition [%s][%s].",(part->name!=NULL?part->name:""),(key->name!=NULL?key->name:""),(NRDtmp->modname!=NULL?NRDtmp->modname:""),(NRDtmp->name!=NULL?NRDtmp->name
:""));
				return(-1);
			}
		}
		while(index->ikeylen>filedef.ixtdsiz) filedef.ixtdsiz+=4096;
	}
	filedef.dfilno=(-1);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) seeifil(&filedef);
#endif /* USE_RDA_DIAGNOSTICS */
	odir=stralloc(CURRENTDIRECTORY);
#ifdef WIN32
	sprintf(stemp,"%s\\%s",CURRENTDIRECTORY,NRDtmp->modname);
	RCHGCWD(stemp);
#endif /* WIN32 */
#ifndef WIN32
	sprintf(stemp,"%s/%s",CURRENTDIRECTORY,NRDtmp->modname);
	RCHGCWD(stemp);
#endif /* Not WIN32 */
	if(NRDtmp->db_engine==NRDCtreeEngine)
	{
		return_value=CREIFILX(&filedef,NULL,NULL,OPF_ALL|GPF_READ|GPF_WRITE|GPF_DEF|GPF_DELETE,"XPERT",NULL);
	} else {
		return_value=CREIFILX(&filedef,NULL,NULL,OPF_ALL|GPF_READ|GPF_WRITE|GPF_DEF|GPF_DELETE,"XPERT",NULL);
	}
	RCHGCWD(odir);
	if(odir!=NULL) Rfree(odir);
	if(return_value)
	{
		prterr("Error CREIFILX %s %s",ctdb_errors(errno),NRDtmp->name);
		return_value=(-1);
	} else {
		return_value=filedef.tfilno;
	}
	if(return_value>=0)
	{
		NRDtmp->filenum=return_value;
		if(USE_ODBC)
		{
			MakeDODA(NRDtmp);
		}
		CLRFIL(return_value);	
	}
	if(USE_ODBC)
	{
	if(NRDtmp->db_engine==NRDCtreeEngine)
	{
		return_value=PUTIFILX(&filedef,NULL,NULL,NULL);
		if(return_value)
		{
			prterr("Error PUTIFILX %s %s",ctdb_errors(errno),NRDtmp->name);
		} 
	} else {
		return_value=PUTIFILX(&filedef,NULL,NULL,NULL);
		if(return_value)
		{
			prterr("Error PUTIFILX %s %s",ctdb_errors(errno),NRDtmp->name);
		} 
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) seeifil(&filedef);
#endif /* USE_RDA_DIAGNOSTICS */
	}
	for(x=0,index=filedef.ix;x<filedef.dnumidx;++x,++index)
	{
		Rfree(index->ridxnam);
		Rfree(index->seg);
	}
	Rfree(filedef.rfstfld); 
	Rfree(filedef.rlstfld);
	Rfree(filedef.pfilnam);
	return(return_value); /* data file number if everything ok */
}
#endif
unsigned getdreclen(NRDbuffer *NRDtmp)
{
	NRDfield *field;
	short x;
	int length=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG getdreclen Determining Data Record Length for Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,field=NRDtmp->field+x;x<NRDtmp->nofields;++x,++field)
	{
		switch(field->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
				break;
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
 				length+=field->len;
				break;
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case SDECIMALV:
				length+=sizeof(double);
				break;
			case SHORTV:
			case SSHORTV:
				length+=sizeof(short);
				break;
			case LONGV:
			case SLONGV:
				length+=sizeof(int);
				break;
			case BOOLNS:
			case CHARACTERS:
				length+=field->len;
				break;
			default:
				prterr("Error Field Type [%d] is invalid for Field [%s] in Module [%s] File [%s].",field->type,field->name,NRDtmp->modname,NRDtmp->name);
				break;
		}
	}
	++length;
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG getdreclen Returning File Length [%d] for Module [%s] File [%s].",length,NRDtmp->modname,NRDtmp->name); }
#endif /* USE_RDA_DIAGNOSTICS */
	return(length);
}
