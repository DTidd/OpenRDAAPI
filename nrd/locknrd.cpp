#include <nrdp.hpp>

short xLOCNRDFILE(short fileno,int line,char *file)
{
	if(!TESTFILENO(fileno,line,file)) return(-1);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG LOCNRDFILE Locking File [%d] [%s][%s] at line [%d] program [%s].",fileno,MODULENAME(fileno),FILENAME(fileno),line,file); 
	}
#endif /* USE_RDA_DIAGNOSTICS */
	switch(NRDbuf[fileno].db_engine)
	{
		case NRDRdaEngine:	return(LOCNRDFILErda(fileno));
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:	return(LOCNRDFILEisam(fileno));
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine: return(LOCNRDmyisam(fileno));
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine: return(LOCNRDmysqlxpert(fileno));
		case NRDMySQLEngine: return(LOCNRDmysql(fileno));
		case NRDMySQLAutoEngine: return(LOCNRDmysql(fileno));
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree:	return(LOCNRDFILEberkeley(fileno));
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine:	return(LOCNRDFILEctree(fileno));
		case NRDLocalCtreeEngine:	return(LOCNRDFILEctree(fileno));
#endif
		default:
			prterr("Error Unknown Database Engine [%d] for Module [%s] File [%d] [%s] at line [%d] program [%s].",NRDbuf[fileno].db_engine,MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
			return(-1);
	}
}
short LOCNRDFILErda(short fileno)
{
	NRDbuffer *NRDtmp;

	NRDtmp=NRDbuf+fileno;
	if(LOCDTA(NRDtmp->dtanum,0,TRUE))
	{
		prterr("Error RDA Database couldn't lock File [%d] [%s][%s].",fileno,NRDtmp->modname,NRDtmp->name);
		return(-1);
	}
	return(0);
}

short xLOCNRD(short fileno,int line,char *file)
{
	if(!TESTFILENO(fileno,line,file)) return(-1);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG LOCNRD Locking current record in Module [%s] File [%d] [%s] at line [%d] program [%s].",MODULENAME(fileno),fileno,FILENAME(fileno),line,file); 
	}
#endif /* USE_RDA_DIAGNOSTICS */
	switch(NRDbuf[fileno].db_engine)
	{
		case NRDRdaEngine:	return(LOCNRDrda(fileno));
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:	return(LOCNRDisam(fileno));
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine: return(LOCNRDmyisam(fileno));
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine: return(LOCNRDmysqlxpert(fileno));
		case NRDMySQLEngine: return(LOCNRDmysql(fileno));
		case NRDMySQLAutoEngine: return(LOCNRDmysql(fileno));
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree:	return(LOCNRDberkeley(fileno));
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine:	return(LOCNRDctree(fileno));
		case NRDLocalCtreeEngine:	return(LOCNRDctree(fileno));
#endif
		default:
			prterr("Error Unknown Database Engine [%d] for Module [%s] File [%d] [%s] at line [%d] program [%s].",NRDbuf[fileno].db_engine,MODULENAME(fileno),fileno,FILENAME(fileno),line,file);
			return(-1);
	}
}
short LOCNRDrda(short fileno)
{
	NRDbuffer *NRDtmp;

	NRDtmp=NRDbuf+fileno;
	if(LOCDTA(NRDtmp->dtanum,NRDtmp->currec,TRUE))
	{
		prterr("Error RDA Database couldn't lock current record in Module [%s] File [%d] [%s].",NRDtmp->modname,fileno,NRDtmp->name);
		return(-1);
	}
	return(0);
}
#ifdef USE_CTREE
short LOCNRDFILEctree(short fileno)
{
	NRDbuffer *NRDtmp=NULL;

	NRDtmp=NRDbuf+fileno;
	RecordNRDconnect(NRDtmp->servername,NRDtmp->db_engine);
	LKISAM(ENABLE_BLK);
	return(0);
}
short LOCNRDctree(short fileno)
{
	short x;
	NRDbuffer *NRDtmp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG Locking current record using C-tree Engine.");
	}
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	RecordNRDconnect(NRDtmp->servername,NRDtmp->db_engine);
	x=LOKREC(NRDbuf[fileno].filenum,3,GETVLEN(NRDbuf[fileno].filenum));
	if(x)
	{
		prterr("C-tree lock error code %d on lock of %s",x,
			NRDbuf[fileno].name);
	}
	return(0);
}
#endif
short xUNLNRDFILE(short fileno,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG Unlocking File [%d] [%s] at line [%d] file [%s].",fileno,FILENAME(fileno),line,file); }
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	switch(NRDbuf[fileno].db_engine)
	{
		case NRDRdaEngine:	return(UNLNRDFILErda(fileno));
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:	return(UNLNRDFILEisam(fileno));
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine: return(UNLNRDFILEmyisam(fileno));
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine: return(UNLNRDFILEmysqlxpert(fileno));
		case NRDMySQLEngine: return(UNLNRDFILEmysql(fileno));
		case NRDMySQLAutoEngine: return(UNLNRDFILEmysql(fileno));
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree:	return(UNLNRDFILEberkeley(fileno));
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine:	return(UNLNRDFILEctree(fileno));
		case NRDLocalCtreeEngine:	return(UNLNRDFILEctree(fileno));
#endif
		default:
			prterr("Unknown engine type %d in LOCNRDFILE",
				NRDbuf[fileno].db_engine);
			return(-1);
	}
}
short UNLNRDFILErda(short fileno)
{
	NRDbuffer *NRDtmp;

	NRDtmp=NRDbuf+fileno;
	UNLDTA(NRDtmp->dtanum,0);
	return(0);
}
short xUNLNRD(short fileno,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG Unlocking current record in file [%d] [%s] at line [%d] file [%s].",fileno,FILENAME(fileno),line,file); }
#endif /* USE_RDA_DIAGNOSTICS */
	if(!TESTFILENO(fileno,line,file)) return(-1);
	switch(NRDbuf[fileno].db_engine)
	{
		case NRDRdaEngine:	return(UNLNRDrda(fileno));
#ifdef INFORMIX_ISAM
		case NRDInformixIsamEngine:	return(UNLNRDisam(fileno));
#endif
#ifdef USE_BERKELEY_BTREE
		case NRDBerkeleyBtree:	return(UNLNRDberkeley(fileno));
#endif
#ifdef USE_MYISAM
		case NRDMyIsamEngine: return(UNLNRDmyisam(fileno));
#endif
#ifdef USE_MYSQL
		case NRDMySQLXpertEngine: return(UNLNRDmysqlxpert(fileno));
		case NRDMySQLEngine: return(UNLNRDmysql(fileno));
		case NRDMySQLAutoEngine: return(UNLNRDmysql(fileno));
#endif
#ifdef USE_CTREE
		case NRDCtreeEngine:	return(UNLNRDctree(fileno));
		case NRDLocalCtreeEngine:	return(UNLNRDctree(fileno));
#endif
		default:
			prterr("Unknown engine type %d in LOCNRD",
				NRDbuf[fileno].db_engine);
			return(-1);
	}
}
short UNLNRDrda(short fileno)
{
	NRDbuffer *NRDtmp;

	NRDtmp=NRDbuf+fileno;
	UNLDTA(NRDtmp->dtanum,NRDtmp->currec);
	return(0);
}
#ifdef USE_CTREE
short UNLNRDFILEctree(short fileno)
{
	NRDbuffer *NRDtmp=NULL;

	NRDtmp=NRDbuf+fileno;
	RecordNRDconnect(NRDtmp->servername,NRDtmp->db_engine);
	LKISAM(FREE);
	return(0);
}
short UNLNRDctree(short fileno)
{
	short x;
	NRDbuffer *NRDtmp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd) { prterr("DIAG Unlocking current record using C-tree Engine."); }
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	RecordNRDconnect(NRDtmp->servername,NRDtmp->db_engine);
	x=LOKREC(NRDbuf[fileno].filenum,0,GETVLEN(NRDbuf[fileno].filenum));
	if(x)
	{
		prterr("C-tree lock error code %d on unlock of %s",x,
			NRDbuf[fileno].name);
	}
	return(0);
}
#endif
