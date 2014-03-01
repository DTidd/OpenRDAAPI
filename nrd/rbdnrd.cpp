#include <nrdp.hpp>
#ifdef UNIXWARE21
#include <errno.h>
#endif
#ifdef UNIXWARE7
#include <errno.h>
#endif

short xRBDNRD(short fileno,int line,char *file)
{
	NRDbuffer *NRDtmp;
	short ef=FALSE;
	char *modx=NULL,*filx=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG RBDNRD Rebuilding Module [%s] File [%d] [%s] at line [%d] file [%s].",MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG Rebuiding file [%s].",NRDbuf[fileno].name); }
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	switch(NRDbuf[fileno].db_engine)
	{
		case NRDRdaEngine:	
			ef=RBDNRDrda(NRDtmp);
			CLSNRD(fileno);
			return(ef);
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:
			modx=stralloc(NRDtmp->modname);
			filx=stralloc(NRDtmp->name);
			CLSNRD(ef);
			ef=RBDNRDisam(modx,filx);
			if(modx!=NULL) Rfree(modx);
			if(filx!=NULL) Rfree(filx);
			return(ef);
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine:
			modx=stralloc(NRDtmp->modname);
			filx=stralloc(NRDtmp->name);
			CLSNRD(ef);
			ef=RBDNRDmyisam(modx,filx);
			if(modx!=NULL) Rfree(modx);
			if(filx!=NULL) Rfree(filx);
			return(ef);
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine:
		case NRDMySQLEngine:
		case NRDMySQLAutoEngine:
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree:
			return(ef);
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine:	
		case NRDLocalCtreeEngine:
			ef=RBDNRDctree(NRDtmp);
			return(ef);
#endif
		default:
			prterr("Error Unknown Database Engine [%d] for Module [%s] File [%s] at line [%d] program [%s].",NRDbuf[fileno].db_engine,MODULENAME(fileno),FILENAME(fileno),line,file);
			return(-1);
	}
}
short RBDNRDrda(NRDbuffer *NRDtmp)
{
	short retval=FALSE;

	retval=RBDDTA(NRDtmp->dtanum);
	return(retval);
}
#ifdef USE_CTREE
short RBDNRDctree(NRDbuffer *NRDtmp)
{
	short x,y,return_value=0;
	NRDfield *field;
	NRDkey *key;
	NRDpart *part;
	IFIL filedef;
	IIDX *index;
	ISEG *element;
	char *fullname=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) 
	{
		prterr("DIAG RBDNRDctree Rebuiding file [%s] using C-tree Engine.",
			NRDtmp->name);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	RecordNRDconnect(NRDtmp->servername,NRDtmp->db_engine);
	fullname=Rmalloc(RDAstrlen(NRDtmp->name)+RDAstrlen(NRDtmp->modname)+4);
#ifndef WIN32
	sprintf(fullname,"./%s/%s",NRDtmp->modname,NRDtmp->name);
#endif
#ifdef WIN32
	sprintf(fullname,".\\%s\\%s",NRDtmp->modname,NRDtmp->name);
#endif
	filedef.pfilnam=adddashes(fullname);
	if(fullname!=NULL) Rfree(fullname);
	filedef.dxtdsiz=4096;
	if(checkvariable(NRDtmp))
	{
		filedef.dfilmod=(EXCLUSIVE | CHECKLOCK | VLENGTH | DUPCHANEL);
	} else {
		filedef.dfilmod=(EXCLUSIVE | CHECKLOCK | ctFIXED | DUPCHANEL);
	}
	filedef.ixtdsiz=4096;
	filedef.dreclen=getdreclen(NRDtmp);
	filedef.ifilmod=EXCLUSIVE|DUPCHANEL;
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
			switch(field->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
					filedef.ifilmod=EXCLUSIVE|DUPCHANEL|VLENGTH;
					element->segmode=VARSEG;
					element->slength=get_length(NRDtmp,part->name);
					if(element->slength<=0) 
					{
/* variable length offsets may need to be the variable length field number */
						prterr("Error variable length key %s with no maximum",field->name);
						element->slength=0;
					}
					break;
				case ZIPCODE:
				case DATES:
				case TIMES:
				case SOCSECNUM:
				case PHONE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case CUSTOMTYPE:
				case PLAINTEXT:
					element->segmode=REGSEG;
					element->slength=get_length(NRDtmp,part->name);
					if(element->slength<=0) 
					{
						filedef.ifilmod=EXCLUSIVE|DUPCHANEL|VLENGTH;
						element->segmode=VARSEG;
/* variable length offsets may need to be the variable length field number */
						prterr("Error variable length key %s with no maximum",field->name);
						element->slength=0;
					}
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DOUBLEV:
				case SDOUBLEV:
				case DECIMALV:
				case SDECIMALV:
					element->slength=sizeof(double);
					element->segmode=FLTSEG;
					break;
				case SHORTV:
				case SSHORTV:
					element->slength=sizeof(short);
					element->segmode=SGNSEG;
					break;
				case LONGV:
				case SLONGV:
					element->slength=sizeof(int);
					element->segmode=SGNSEG;
					break;
				case BOOLNS:
				case CHARACTERS:
					element->slength=1;
					element->segmode=REGSEG;
					break;
				default:
					prterr("Unknown field type %d for %s in CRTNRDctree",field->type,field->name);
					break;
			}
			index->ikeylen+=element->slength;
			key->length+=element->slength;
		}
		while(index->ikeylen>filedef.ixtdsiz) filedef.ixtdsiz+=4096;
	}
	return_value=CLRFIL(NRDtmp->filenum);
	if(return_value)
	{
		prterr("Error CLRFIL %s %s",ctdb_errors(errno),NRDtmp->name);
/*
		prterr("Error CLRFIL %s %s",ctdb_errors(isam_err),NRDtmp->name);
		prterr("Error CLRFIL %s %s",ctdb_errors(errno,uerr_cod),NRDtmp->name);
*/
		return(-1);
	}
	filedef.dfilno=(-1);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) seeifil(&filedef);
#endif /* USE_RDA_DIAGNOSTICS */
	return_value=RBLIFILX(&filedef,NULL,NULL,OPF_ALL|GPF_READ|GPF_WRITE|GPF_DEF|GPF_DELETE,"XPERT",NULL);
	if(return_value)
	{
		prterr("Error RBLIFILX %s %s",ctdb_errors(errno),NRDtmp->name);
/*
		prterr("Error RBLIFILX %s %s",ctdb_errors(isam_err),NRDtmp->name);
		prterr("Error RBLIFILX %s %s",ctdb_errors(errno,uerr_cod),NRDtmp->name);
*/
		return(-1);
	}
	for(x=0,index=filedef.ix;x<filedef.dnumidx;++x,++index)
	{
		Rfree(index->ridxnam);
		Rfree(index->seg);
	}
	Rfree(filedef.rfstfld); 
	Rfree(filedef.rlstfld);
	Rfree(filedef.pfilnam);
	NRDtmp->filenum=filedef.tfilno;
	return(filedef.tfilno); /* data file number if everything ok */
}
#endif
short xRESNRD(short fileno,int line,char *file)
{
	NRDbuffer *NRDtmp;
	short ef=FALSE;
	char *modx=NULL,*filx=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG RESNRD Rebuilding Node in Module [%s] File Definition [%d] [%s] at line [%d] program [%s].",MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG Rebuiding file [%s].",NRDbuf[fileno].name); }
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	switch(NRDbuf[fileno].db_engine)
	{
		case NRDRdaEngine:	
			if(RESNRDrda(NRDtmp)) return(-1);
			return(fileno);
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:
			return(fileno);
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine:
			modx=stralloc(NRDtmp->modname);
			filx=stralloc(NRDtmp->name);
			CLSNRD(ef);
			ef=RESNRDmyisam(modx,filx);
			if(modx!=NULL) Rfree(modx);
			if(filx!=NULL) Rfree(filx);
			return(ef);
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine:
		case NRDMySQLEngine:
		case NRDMySQLAutoEngine:
			return(fileno);
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree:
			return(fileno);
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine:	
		case NRDLocalCtreeEngine:
			prterr("Error C-tree Database Engine can only rebuild the database. Rebuilding C-tree File at line [%d] file [%s].",line,file);
			return(RBDNRDctree(NRDtmp));
#endif
		default:
			prterr("Error Unknown Database Engine [%d] for Module [%s] File [%s] at line [%d] program [%s].",NRDbuf[fileno].db_engine,MODULENAME(fileno),FILENAME(fileno),line,file);
			return(-1);
	}
}
short RESNRDrda(NRDbuffer *NRDtmp)
{
	short tmp=FALSE;
	char *filename=NULL;
	char *dashes=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG RESNRDrda Rebuilding Node in Module [%s] File [%s] using RDA Engine.",NRDtmp->modname,NRDtmp->name); }
#endif /* USE_RDA_DIAGNOSTICS */
	dashes=Rmalloc(RDAstrlen(NRDtmp->name)+8+RDAstrlen(NRDtmp->modname));
#ifndef WIN32
	sprintf(dashes,"./%s/%s.dat",NRDtmp->modname,NRDtmp->name);
#endif
#ifdef WIN32
	sprintf(dashes,".\\%s\\%s.dat",NRDtmp->modname,NRDtmp->name);
#endif
	filename=adddashes(dashes);
	CLSDTA(NRDtmp->dtanum);
	tmp=RESDTA(filename);
	NRDtmp->dtanum=OPNDTA(filename);
	if(filename!=NULL) Rfree(filename);
	if(dashes!=NULL) Rfree(dashes);
	return(tmp);
}
