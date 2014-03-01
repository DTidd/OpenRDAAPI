#include <nrdp.hpp>
#ifdef UNIXWARE7
#include <errno.h>
#endif
#ifdef UNIXWARE21
#include <errno.h>
#endif

short xOPNNRD(char *module,char *name,int line,char *program)
{
	short x,nrdno;
	NRDbuffer *NRDtmp;
#ifdef INFORMIX_ISAM
	short a=0;
#endif

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field) { prterr("DIAG OPNNRD Opening File Definition [%s] [%s] at line [%d] program [%s].",module,name,line,program); }
#endif /* USE_RDA_DIAGNOSTICS */
	x=getnrd(module,name);
	if(x<0)
	{
		prterr("Error can't read File Definition [%s] [%s] from Binary Library [%s.FIL] at line [%d] program [%s].",module,name,module,line,program);
		return(-1);
	}
	NRDtmp=NRDbuf+x;
	nrdno=x;
	switch(NRDtmp->db_engine)
	{
		case NRDRdaEngine:
			NRDtmp->dtanum=OPNNRDrda(NRDtmp);
			if(NRDtmp->dtanum==NULL)
			{
				freenrd(NRDtmp);
				prterr("Error couldn't open Actual RDA Database File [%s] [%s] at line [%d] program [%s].  Verify the Data File [%s.dat] and Index File [%s.idx] are available.",module,name,line,program,name,name);
				return(-1); 
			}
			break;
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:
			a=OPNNRDisam(NRDtmp);
			if(a==(-1))
			{
				freenrd(NRDtmp);
				prterr("Error couldn't open Actual Informix-OnLine Database File [%s] [%s] at line [%d] program [%s].  Verify the Data File [%s.dat] and Index File [%s.idx] are available.",module,name,line,program,name,name);
				return(-1);
			}
			break;
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine:
			if(OPNNRDmyisam(NRDtmp))
			{
				freenrd(NRDtmp);
				prterr("Error couldn't open Actual MYSQL Embedded Database Table [%s] [%s] at line [%d] program [%s].  Verify the Data File [%s.MYD] is available.",module,name,line,program,name);
				return(-1);
			}
			break;
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine:
			if(!OPNNRDmysqlxpert(NRDtmp))
			{
				freenrd(NRDtmp);
				prterr("Error couldn't open Actual MYSQL Server Database Table [%s] [%s] at line [%d] program [%s].  Verify the Data File [%s.MYD] is available.",module,name,line,program,name);
				return(-1);
			}
			break;
		case NRDMySQLEngine:
		case NRDMySQLAutoEngine:
			if(!OPNNRDmysql(NRDtmp))
			{
				freenrd(NRDtmp);
				prterr("Error couldn't open Actual MYSQL Server Database Table [%s] [%s] at line [%d] program [%s].  Verify the Data File [%s.MYD] is available.",module,name,line,program,name);
				return(-1);
			}
			break;
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree:
			NRDtmp->dbp=OPNNRDberkeley(NRDtmp);
			if(NRDtmp->dbp==NULL)
			{
				freenrd(NRDtmp);
				prterr("Error couldn't open Actual Berkeley Btree Database File [%s] [%s] at line [%d] program [%s].  Verify the Data File [%s.dat] and Index File [%s.idx] are available.",module,name,line,program,name,name);
				return(-1);
			}
			NRDtmp->varfile=VARRECORDS(nrdno);
			break;
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine:
		case NRDLocalCtreeEngine:
			NRDtmp->filenum=OPNNRDctree(NRDtmp,FALSE);
			if(NRDtmp->filenum==(-1))
			{
				freenrd(NRDtmp);
				prterr("Error couldn't open Actual C-tree Database File [%s] [%s] at line [%d] program [%s].  Verify the Data File [%s.dat] and Index File [%s.idx] are available.",module,name,line,program,name,name);
				return(-1);
			}
			NRDtmp->varfile=VARRECORDS(nrdno);
			break;
#endif
		default:
			prterr("Error Unknown Database Engine [%d] for File Definition [%s] [%s] at line [%d] program [%s].",NRDtmp->db_engine,module,name,line,program);
			return(-1);
	}
	return nrdno;
}
short xOPNEXCLCKNRD(char *module,char *name,int line,char *program)
{
	short x,nrdno;
	NRDbuffer *NRDtmp;
#ifdef INFORMIX_ISAM
	short a=0;
#endif

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field) { prterr("DIAG OPNNRD Opening File Definition [%s] [%s] at line [%d] program [%s].",module,name,line,program); }
#endif /* USE_RDA_DIAGNOSTICS */
	x=getnrd(module,name);
	if(x<0)
	{
		prterr("Error can't read File Definition [%s] [%s] from Binary Library [%s.FIL] at line [%d] program [%s].",module,name,module,line,program);
		return(-1);
	}
	NRDtmp=NRDbuf+x;
	nrdno=x;
	switch(NRDtmp->db_engine)
	{
		case NRDRdaEngine:
			NRDtmp->dtanum=OPNNRDrda(NRDtmp);
			if(NRDtmp->dtanum==NULL)
			{
				freenrd(NRDtmp);
				prterr("Error couldn't open Actual RDA Database File [%s] [%s] at line [%d] program [%s].  Verify the Data File [%s.dat] and Index File [%s.idx] are available.",module,name,line,program,name,name);
				return(-1); 
			}
			break;
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:
			a=OPNEXCLCKNRDisam(NRDtmp);
			if(a==(-1))
			{
				freenrd(NRDtmp);
				prterr("Error couldn't open Actual Informix-OnLine Database File [%s] [%s] at line [%d] program [%s].  Verify the Data File [%s.dat] and Index File [%s.idx] are available.",module,name,line,program,name,name);
				return(-1);
			}
			break;
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine:
			if(OPNEXCLCKNRDmyisam(NRDtmp))
			{
				freenrd(NRDtmp);
				prterr("Error couldn't open Actual MYSQL Embedded Database Table [%s] [%s] at line [%d] program [%s].  Verify the Data File [%s.dat] is available.",module,name,line,program,name);
				return(-1);
			}
			break;
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine:
			if((OPNNRDmysqlxpert(NRDtmp)==NULL))
			{
				prterr("Error: OPNNRDmysqlxpert.");
			}
			break;
		case NRDMySQLEngine:
		case NRDMySQLAutoEngine:
			if((OPNNRDmysql(NRDtmp)==NULL))
			{
				prterr("Error: OPNNRDmysql.");
			}
			break;
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine:
		case NRDLocalCtreeEngine:
			NRDtmp->filenum=OPNNRDctree(NRDtmp,TRUE);
			if(NRDtmp->filenum==(-1))
			{
				freenrd(NRDtmp);
				prterr("Error couldn't open Actual C-tree Database File [%s] [%s] at line [%d] program [%s].  Verify the Data File [%s.dat] and Index File [%s.idx] are available.",module,name,line,program,name,name);
				return(-1);
			}
			NRDtmp->varfile=VARRECORDS(nrdno);
			break;
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree:
			NRDtmp->dbp=OPNNRDberkeley(NRDtmp);
			if(NRDtmp->dbp==NULL)
			{
				freenrd(NRDtmp);
				prterr("Error couldn't open Actual Berkeley Btree Database File [%s] [%s] at line [%d] program [%s].  Verify the Data File [%s.dat] and Index File [%s.idx] are available.",module,name,line,program,name,name);
				return(-1);
			}
			NRDtmp->varfile=VARRECORDS(nrdno);
			break;
#endif
		default:
			prterr("Error Unknown Database Engine [%d] for File Definition [%s] [%s] at line [%d] program [%s].",NRDtmp->db_engine,module,name,line,program);
			return(-1);
	}
	return nrdno;
}
DTA OPNNRDrda(NRDbuffer *NRDtmp)
{
	DTA tmp=NULL;
	char *filename=NULL;
	char *dashes=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG OPNNRDrda Opening File Definition [%s] [%s] using RDA Engine.",NRDtmp->modname,NRDtmp->name); }
#endif /* USE_RDA_DIAGNOSTICS */
	dashes=Rmalloc(RDAstrlen(NRDtmp->name)+8+RDAstrlen(NRDtmp->modname));
#ifndef WIN32
	sprintf(dashes,"./%s/%s.dat",NRDtmp->modname,NRDtmp->name);
#endif
#ifdef WIN32
	sprintf(dashes,".\\%s\\%s.dat",NRDtmp->modname,NRDtmp->name);
#endif
	filename=adddashes(dashes);
	tmp=OPNDTA(filename);
	if(filename!=NULL) Rfree(filename);
	if(dashes!=NULL) Rfree(dashes);
	return(tmp);
}
#ifdef USE_CTREE
short OPNNRDctree(NRDbuffer *NRDtmp,short exclusive)
{
	short new_file;
	char *filename=NULL;
	char *dashes=NULL;
	int length=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG OPNNRDctree Opening File [%s] using C-tree Engine.",NRDtmp->name); }
#endif /* USE_RDA_DIAGNOSTICS */
	dashes=Rmalloc(RDAstrlen(NRDtmp->name)+9+RDAstrlen(NRDtmp->modname));
#ifndef WIN32
	sprintf(dashes,"./%s/%s.dat",NRDtmp->modname,NRDtmp->name);
#endif
#ifdef WIN32
	sprintf(dashes,".\\%s\\%s.dat",NRDtmp->modname,NRDtmp->name);
#endif
	if(access(dashes,02))
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagnrd)
		{
			prterr("Error OPNNRDctree file [%s] doesn't exist or security access denied!",dashes);
		}
#endif /* USE_RDA_DIAGNOSTICS */
		if(dashes!=NULL) Rfree(dashes);
		return(-1);
	}
	RecordNRDconnect(NRDtmp->servername,NRDtmp->db_engine);
	filename=adddashes(dashes);
	if(checkvariable(NRDtmp))
	{
		if(NRDtmp->db_engine==NRDCtreeEngine)
		{
			if(exclusive==TRUE)
			{
				new_file=OPNRFILX((-1),filename,EXCLUSIVE|PERMANENT|CHECKLOCK|DUPCHANEL|VLENGTH,NULL);
			} else {
				new_file=OPNRFILX((-1),filename,SHARED|PERMANENT|CHECKLOCK|DUPCHANEL|VLENGTH,NULL);
			}
		} else {
			if(exclusive==TRUE)
			{
				new_file=OPNRFILX((-1),filename,EXCLUSIVE|PERMANENT|CHECKLOCK|DUPCHANEL|VLENGTH,NULL);
			} else {
				new_file=OPNRFILX((-1),filename,SHARED|PERMANENT|CHECKLOCK|DUPCHANEL|VLENGTH,NULL);
			}
		}
	} else {
		if(NRDtmp->db_engine==NRDCtreeEngine)
		{
			if(exclusive==TRUE)
			{
				new_file=OPNRFILX((-1),filename,EXCLUSIVE|PERMANENT|CHECKLOCK|DUPCHANEL,NULL);
			} else {
				new_file=OPNRFILX((-1),filename,SHARED|PERMANENT|CHECKLOCK|DUPCHANEL,NULL);
			}
		} else {
			if(exclusive==TRUE)
			{
				new_file=OPNRFILX((-1),filename,EXCLUSIVE|PERMANENT|CHECKLOCK|DUPCHANEL,NULL);
			} else {
				new_file=OPNRFILX((-1),filename,SHARED|PERMANENT|CHECKLOCK|DUPCHANEL,NULL);
			}
		}
	}
	if(new_file<0)
	{
		prterr("Error OPNRFILX %s %s",ctdb_errors(errno),filename);
	} else { 
		SETCURI(new_file,0,NULL,0);
		length=CTREERecordLength(NRDtmp,TRUE);
		QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,length+1);
		memset(NRDtmp->record_data,0,NRDtmp->dlen);
	}
	if(filename!=NULL) Rfree(filename);
	if(dashes!=NULL) Rfree(dashes);
	return(new_file);
} 
#endif
short xCLSNRD(short fileno,int line,char *file)
{
	short value=(-1);
	NRDbuffer *NRDtmp;

	if(!TESTFILENO(fileno,line,file)) return(-1);
	NRDtmp=NRDbuf+fileno;
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd || diagnrd_field) { prterr("DIAG Closing file [%s] at line [%d] file [%s].",NRDtmp->name,line,file); }
#endif /* USE_RDA_DIAGNOSTICS */
	switch(NRDtmp->db_engine)
	{
		case NRDRdaEngine:
			value=CLSNRDrda(fileno);
			break;
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:
			value=CLSNRDisam(fileno);
			break;
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine:
			value=CLSNRDmyisam(fileno);
			break;
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine:
		case NRDMySQLEngine:
		case NRDMySQLAutoEngine:
			value=CLSNRDmysql(fileno);
			break;
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine:
		case NRDLocalCtreeEngine:
			value=CLSNRDctree(fileno);
			break;
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree:
			value=CLSNRDberkeley(fileno);
			break;
#endif
		default:
			prterr("Unknown engine type %d in CLSNRD",
				NRDtmp->db_engine);
			value=(-1);
			break;
	}
	freenrd(NRDtmp);
	return value;
}
short CLSNRDrda(short fileno)
{
	NRDbuffer *NRDtmp;

	NRDtmp=NRDbuf+fileno;
	CLSDTA(NRDtmp->dtanum);
	return(0);
}
#ifdef USE_CTREE
short CLSNRDctree(short fileno)
{
	int x=0;
	NRDbuffer *NRDtmp;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG Closing file [%s] using C-tree Engine.",NRDbuf[fileno].name); }
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	RecordNRDconnect(NRDtmp->servername,NRDtmp->db_engine);
	x=CLRFIL(NRDbuf[fileno].filenum);
	if(x) 
	{ 
		prterr("Error CRLFIL %s Module [%s] File [%s]",ctdb_errors(errno),NRDtmp->modname,NRDtmp->name); 
	}
	return(x);
}
#endif
