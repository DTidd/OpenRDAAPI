/*lint -library*/
/*lint -e525*/
#include <nrdp.hpp>

#if(defined(USE_BERKELEY_BTREE))
DB_ENV *BerkeleyENV=NULL;
#define KEYNAME2

#ifndef WIN32
#define USE_ENVIRONMENT
#define USE_DIRECTORY
#define USE_LOCKING
#endif /* NOT WIN32 */
#ifdef WIN32
#endif /* WIN32 */
#ifdef XXXXX


#define USE_LOCKING
#define USE_ENVIRONMENT
#define USE_SETCACHE
#define USE_RDA_ALLOC
#define USE_WRITECURSOR
#define USE_RECNUM
#define DEFAULT_BLOCKSIZE	32768
#define USE_SETPAGESIZE /* Use Defaults, they are faster */
#define USE_ERRFILE
#define USE_BTREESTAT
#endif

#ifdef USE_RDA_ALLOC
void *Bmalloc(unsigned size)
{
	return(xRmalloc(size,0,"BERKELEY DB-4.1.25"));
}
void *Brealloc(void *a,unsigned size)
{
	return(xRrealloc(a,size,0,"BERKELEY DB-4.1.25"));
}
void Bfree(void *a)
{
	xRfree(a,0,"BERKELEY DB-4.1.25");
}
#endif /* USE_RDA_ALLOC */
short LOCNRDFILEberkeley(short fileno)
{
	NRDbuffer *NRDtmp;

	NRDtmp=NRDbuf+fileno;
	return(0);
}
short LOCNRDberkeley(short fileno)
{
	NRDbuffer *NRDtmp;

	NRDtmp=NRDbuf+fileno;
	return(0);
}
short UNLNRDFILEberkeley(short fileno)
{
	NRDbuffer *NRDtmp;

	NRDtmp=NRDbuf+fileno;

	return(0);
}
short UNLNRDberkeley(short fileno)
{
	NRDbuffer *NRDtmp;

	NRDtmp=NRDbuf+fileno;
	return(0);
}
short DELNRDberkeley(short fileno)
{
	NRDbuffer *NRDtmp;
	NRDkey *key=NULL;
	LONG return_value=0;
	short tlen=0;
	void *tmptarget=NULL;
	DBT k;

	if(diagnrd)
	{
		prterr("DIAG DELNRDberkeley Delete Current Record in Module [%s] File [%d] [%s] using Berkeley Btree Database Engine.",MODULENAME(fileno),fileno,FILENAME(fileno));
	}
	NRDtmp=NRDbuf+fileno;
	key=NRDtmp->key;
	tmptarget=NRDtmp2target(NRDtmp,key,&tlen);
	memset(&k,0,sizeof(DBT));
	k.data=tmptarget;
	k.size=tlen;
	return_value=NRDtmp->dbp->del(NRDtmp->dbp,NULL,&k,0);
	if(tmptarget!=NULL) Rfree(tmptarget);
	if(return_value)
	{
		if(diagnrd)
		{
			prterr("Error: %d %s Specified Key Value not found in Module [%s] File [%s].",return_value,db_strerror(return_value),NRDtmp->modname,NRDtmp->name);
		}
		return(1);
	}
	return(0);
}
short BEGNRDberkeley(short fileno)
{
	return(FRSNRDberkeley(fileno,1));
}
short SEQNRDberkeley(short fileno)
{
	return(NXTNRDberkeley(fileno,1));
}
short GTENRDberkeley(short fileno,short keyno)
{
	NRDbuffer *NRDtmp;
	NRDkey *key=NULL;
	LONG return_value=0;
	DBC *dbc=NULL;
	DBT k,d;
	short tlen=0,x=0;
	void *tmptarget=NULL;

	if(diagnrd)
	{
		prterr("DIAG NXTNRDberkeley Searching for Equal Key in Module [%s] File [%d] [%s] Keyno [%d] using Berkeley Btree Database Engine.",MODULENAME(fileno),fileno,FILENAME(fileno),keyno);
	}
	NRDtmp=NRDbuf+fileno;
	if((NRDbuf[fileno].nokeys+1)<keyno || keyno<1) 
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s].  Using Keyno [1] instead.",keyno,MODULENAME(fileno),fileno,FILENAME(fileno));
		keyno=1;
	}
	key=NRDtmp->key+(keyno-1);
	x=key->dbp->cursor(key->dbp,NULL,&dbc,0);
	if(x!=0)
	{
		prterr("Error: key->dbp->cursor() %d %s Unable to create cursor/pointer for the key [%s].",x,db_strerror(x),key->name);
		return(-1);
	} else {
		tmptarget=NRDtmp2target(NRDtmp,key,&tlen);
		memset(&k,0,sizeof(DBT));
		memset(&d,0,sizeof(DBT));
		k.data=tmptarget;
		k.size=tlen;
		return_value=dbc->c_get(dbc,&k,&d,DB_SET_RANGE);
		memset(&d,0,sizeof(DBT));
		if(!return_value)
		{
			return_value=dbc->c_get(dbc,&k,&d,DB_CURRENT);
		} else {
			return_value=dbc->c_get(dbc,&k,&d,DB_NEXT);
		}
		if(tmptarget!=NULL) Rfree(tmptarget);
		if(!return_value)
		{
			QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,d.size+1);
			memset(NRDtmp->record_data,0,NRDtmp->dlen);
			memcpy(NRDtmp->record_data,d.data,d.size);
			assignCtree_fields(NRDtmp);
			if(dbc!=NULL)
			{
				dbc->c_close(dbc);
				dbc=NULL;
			}
			if(diagnrd) { xSEENRDRECORD(fileno,__LINE__,__FILE__);prterr("Exiting NXTNRDberkeley OK for Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name); }
			return(0);
		}
	}
	if(dbc!=NULL)
	{
		dbc->c_close(dbc);
		dbc=NULL;
	}
	if(diagnrd)
	{
		prterr("DIAG Specified Key Value not found in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
	return(1);
}
short PRVNRDberkeley(short fileno,short keyno)
{
	NRDbuffer *NRDtmp;
	NRDkey *key=NULL;
	LONG return_value=0;
	short tlen=0,x=0;
	void *tmptarget=NULL;
	DBC *dbc=NULL;
	DBT k,d;

	if(diagnrd)
	{
		prterr("DIAG PRVNRDberkeley Searching for Equal Key in Module [%s] File [%d] [%s] Keyno [%d] using Berkeley Btree Database Engine.",MODULENAME(fileno),fileno,FILENAME(fileno),keyno);
	}
	NRDtmp=NRDbuf+fileno;
	if((NRDbuf[fileno].nokeys+1)<keyno || keyno<1) 
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s].  Using Keyno [1] instead.",keyno,MODULENAME(fileno),fileno,FILENAME(fileno));
		keyno=1;
	}
	key=NRDtmp->key+(keyno-1);
	x=key->dbp->cursor(key->dbp,NULL,&dbc,0);
	if(x!=0)
	{
		prterr("Error: key->dbp->cursor() %d %s Unable to create cursor/pointer for the key [%s].",x,db_strerror(x),key->name);
		return(-1);
	} else {
		tmptarget=NRDtmp2target(NRDtmp,key,&tlen);
		memset(&k,0,sizeof(DBT));
		memset(&d,0,sizeof(DBT));
		k.data=tmptarget;
		k.size=tlen;
		return_value=dbc->c_get(dbc,&k,&d,DB_SET_RANGE);
		memset(&d,0,sizeof(DBT));
		return_value=dbc->c_get(dbc,&k,&d,DB_PREV);
		if(tmptarget!=NULL) Rfree(tmptarget);
		if(!return_value)
		{
			QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,d.size+1);
			memset(NRDtmp->record_data,0,NRDtmp->dlen);
			memcpy(NRDtmp->record_data,d.data,d.size);
			assignCtree_fields(NRDtmp);
			if(dbc!=NULL)
			{
				dbc->c_close(dbc);
				dbc=NULL;
			}
			if(diagnrd) { xSEENRDRECORD(fileno,__LINE__,__FILE__);prterr("Exiting NXTNRDberkeley OK for Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name); }
			return(0);
		}
	}
	if(dbc!=NULL)
	{
		dbc->c_close(dbc);
		dbc=NULL;
	}
	if(diagnrd)
	{
		prterr("DIAG Specified Key Value not found in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
	return(1);
}
short LSTNRDberkeley(short fileno,short keyno)
{
	NRDbuffer *NRDtmp;
	NRDkey *key=NULL;
	LONG return_value=0;
	short tlen=0,x=0;
	void *tmptarget=NULL;
	DBC *dbc=NULL;
	DBT k,d;

	if(diagnrd)
	{
		prterr("DIAG LSTNRDberkeley Searching for Equal Key in Module [%s] File [%d] [%s] Keyno [%d] using Berkeley Btree Database Engine.",MODULENAME(fileno),fileno,FILENAME(fileno),keyno);
	}
	NRDtmp=NRDbuf+fileno;
	if((NRDbuf[fileno].nokeys+1)<keyno || keyno<1) 
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s].  Using Keyno [1] instead.",keyno,MODULENAME(fileno),fileno,FILENAME(fileno));
		keyno=1;
	}
	key=NRDtmp->key+(keyno-1);
	x=key->dbp->cursor(key->dbp,NULL,&dbc,0);
	if(x!=0)
	{
		prterr("Error: key->dbp->cursor() %d %s Unable to create cursor/pointer for the key [%s].",x,db_strerror(x),key->name);
		return(-1);
	} else {
		tmptarget=NRDtmp2target(NRDtmp,key,&tlen);
		memset(&k,0,sizeof(DBT));
		memset(&d,0,sizeof(DBT));
		k.data=tmptarget;
		k.size=tlen;
		return_value=dbc->c_get(dbc,&k,&d,DB_SET_RANGE);
		memset(&d,0,sizeof(DBT));
		if(!return_value)
		{
			return_value=dbc->c_get(dbc,&k,&d,DB_PREV);
		} else {
			return_value=dbc->c_get(dbc,&k,&d,DB_LAST);
		}
		if(tmptarget!=NULL) Rfree(tmptarget);
		if(!return_value)
		{
			QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,d.size+1);
			memset(NRDtmp->record_data,0,NRDtmp->dlen);
			memcpy(NRDtmp->record_data,d.data,d.size);
			assignCtree_fields(NRDtmp);
			if(dbc!=NULL)
			{
				dbc->c_close(dbc);
				dbc=NULL;
			}
			if(diagnrd) { xSEENRDRECORD(fileno,__LINE__,__FILE__);prterr("Exiting NXTNRDberkeley OK for Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name); }
			return(0);
		}
	}
	if(dbc!=NULL)
	{
		dbc->c_close(dbc);
		dbc=NULL;
	}
	if(diagnrd)
	{
		prterr("DIAG Specified Key Value not found in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
	return(1);
}
short LTENRDberkeley(short fileno,short keyno)
{
	NRDbuffer *NRDtmp;
	NRDkey *key=NULL;
	LONG return_value=0;
	short tlen=0,x=0;
	void *tmptarget=NULL;
	DBC *dbc=NULL;
	DBT k,d;

	if(diagnrd)
	{
		prterr("DIAG LTENRDberkeley Searching for Equal Key in Module [%s] File [%d] [%s] Keyno [%d] using Berkeley Btree Database Engine.",MODULENAME(fileno),fileno,FILENAME(fileno),keyno);
	}
	NRDtmp=NRDbuf+fileno;
	if((NRDbuf[fileno].nokeys+1)<keyno || keyno<1) 
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s].  Using Keyno [1] instead.",keyno,MODULENAME(fileno),fileno,FILENAME(fileno));
		keyno=1;
	}
	key=NRDtmp->key+(keyno-1);
	x=key->dbp->cursor(key->dbp,NULL,&dbc,0);
	if(x!=0)
	{
		prterr("Error: key->dbp->cursor() %d %s Unable to create cursor/pointer for the key [%s].",x,db_strerror(x),key->name);
		return(-1);
	} else {
		tmptarget=NRDtmp2target(NRDtmp,key,&tlen);
		memset(&k,0,sizeof(DBT));
		memset(&d,0,sizeof(DBT));
		k.data=tmptarget;
		k.size=tlen;
		return_value=dbc->c_get(dbc,&k,&d,DB_SET_RANGE);
		memset(&d,0,sizeof(DBT));
		if(!return_value)
		{
			return_value=dbc->c_get(dbc,&k,&d,DB_CURRENT);
		} else {
			return_value=dbc->c_get(dbc,&k,&d,DB_PREV);
		}
		if(tmptarget!=NULL) Rfree(tmptarget);
		if(!return_value)
		{
			QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,d.size+1);
			memset(NRDtmp->record_data,0,NRDtmp->dlen);
			memcpy(NRDtmp->record_data,d.data,d.size);
			assignCtree_fields(NRDtmp);
			if(dbc!=NULL)
			{
				dbc->c_close(dbc);
				dbc=NULL;
			}
			if(diagnrd) { xSEENRDRECORD(fileno,__LINE__,__FILE__);prterr("Exiting NXTNRDberkeley OK for Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name); }
			return(0);
		}
	}
	if(dbc!=NULL)
	{
		dbc->c_close(dbc);
		dbc=NULL;
	}
	if(diagnrd)
	{
		prterr("DIAG Specified Key Value not found in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
	return(1);
}
short FRSNRDberkeley(short fileno,short keyno)
{
	NRDbuffer *NRDtmp;
	NRDkey *key=NULL;
	LONG return_value=0;
	short tlen=0,x=0;
	void *tmptarget=NULL;
	DBC *dbc=NULL;
	DBT k,d;

	if(diagnrd)
	{
		prterr("DIAG FRSNRDberkeley Searching for Equal Key in Module [%s] File [%d] [%s] Keyno [%d] using Berkeley Btree Database Engine.",MODULENAME(fileno),fileno,FILENAME(fileno),keyno);
	}
	NRDtmp=NRDbuf+fileno;
	if((NRDbuf[fileno].nokeys+1)<keyno || keyno<1) 
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s].  Using Keyno [1] instead.",keyno,MODULENAME(fileno),fileno,FILENAME(fileno));
		keyno=1;
	}
	key=NRDtmp->key+(keyno-1);
	x=key->dbp->cursor(key->dbp,NULL,&dbc,0);
	if(x!=0)
	{
		prterr("Error: key->dbp->cursor() %d %s Unable to create cursor/pointer for the key [%s].",x,db_strerror(x),key->name);
		return(-1);
	} else {
		tmptarget=NRDtmp2target(NRDtmp,key,&tlen);
		memset(&k,0,sizeof(DBT));
		memset(&d,0,sizeof(DBT));
		k.data=tmptarget;
		k.size=tlen;
		return_value=dbc->c_get(dbc,&k,&d,DB_SET_RANGE);
		memset(&d,0,sizeof(DBT));
		return_value=dbc->c_get(dbc,&k,&d,DB_FIRST);
		if(tmptarget!=NULL) Rfree(tmptarget);
		if(!return_value)
		{
			QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,d.size+1);
			memset(NRDtmp->record_data,0,NRDtmp->dlen);
			memcpy(NRDtmp->record_data,d.data,d.size);
			assignCtree_fields(NRDtmp);
			if(dbc!=NULL)
			{
				dbc->c_close(dbc);
				dbc=NULL;
			}
			if(diagnrd) { xSEENRDRECORD(fileno,__LINE__,__FILE__);prterr("Exiting NXTNRDberkeley OK for Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name); }
			return(0);
		}
	}
	if(dbc!=NULL)
	{
		dbc->c_close(dbc);
		dbc=NULL;
	}
	if(diagnrd)
	{
		prterr("DIAG Specified Key Value not found in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
	return(1);
}
short NXTNRDberkeley(short fileno,short keyno)
{
	NRDbuffer *NRDtmp;
	NRDkey *key=NULL;
	LONG return_value=0;
	short tlen=0,x=0;
	void *tmptarget=NULL;
	DBC *dbc=NULL;
	DBT k,d;

	if(diagnrd)
	{
		prterr("DIAG NXTNRDberkeley Searching for Equal Key in Module [%s] File [%d] [%s] Keyno [%d] using Berkeley Btree Database Engine.",MODULENAME(fileno),fileno,FILENAME(fileno),keyno);
	}
	NRDtmp=NRDbuf+fileno;
	if((NRDbuf[fileno].nokeys+1)<keyno || keyno<1) 
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s].  Using Keyno [1] instead.",keyno,MODULENAME(fileno),fileno,FILENAME(fileno));
		keyno=1;
	}
	key=NRDtmp->key+(keyno-1);
	x=key->dbp->cursor(key->dbp,NULL,&dbc,0);
	if(x!=0)
	{
		prterr("Error: key->dbp->cursor() %d %s Unable to create cursor/pointer for the key [%s].",x,db_strerror(x),key->name);
		return(-1);
	} else {
		tmptarget=NRDtmp2target(NRDtmp,key,&tlen);
		memset(&k,0,sizeof(DBT));
		memset(&d,0,sizeof(DBT));
		k.data=tmptarget;
		k.size=tlen;
		return_value=dbc->c_get(dbc,&k,&d,DB_SET_RANGE);
		memset(&d,0,sizeof(DBT));
		return_value=dbc->c_get(dbc,&k,&d,DB_NEXT);
		if(tmptarget!=NULL) Rfree(tmptarget);
		if(!return_value)
		{
			QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,d.size+1);
			memset(NRDtmp->record_data,0,NRDtmp->dlen);
			memcpy(NRDtmp->record_data,d.data,d.size);
			assignCtree_fields(NRDtmp);
			if(dbc!=NULL)
			{
				dbc->c_close(dbc);
				dbc=NULL;
			}
			if(diagnrd) { xSEENRDRECORD(fileno,__LINE__,__FILE__);prterr("Exiting NXTNRDberkeley OK for Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name); }
			return(0);
		}
	}
	if(dbc!=NULL)
	{
		dbc->c_close(dbc);
		dbc=NULL;
	}
	if(diagnrd)
	{
		prterr("DIAG Specified Key Value not found in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
	return(1);
}
long SIZNRDberkeley(short fileno)
{
	long tmp=0;
#ifdef USE_BTREESTAT
	NRDbuffer *NRDtmp;
	DB_BTREE_STAT *s;

	NRDtmp=NRDbuf+fileno;
	if(NRDtmp->dbp->stat(NRDtmp->dbp,&s,DB_FAST_STAT))
	{
		prterr("Error:  Attempting to gather Berkeley Btree statistics for [%s][%s].",NRDtmp->modname,NRDtmp->name);
	} else {
		tmp=s->bt_ndata;
	}
#endif /* USE_BTREESTAT */
	return(tmp);
}

short EQLNRDKEYberkeley(short fileno,short keyno,void *target)
{
	NRDbuffer *NRDtmp;
	NRDkey *key=NULL;
	LONG return_value=0;
	short tlen=0,x=0;
	void *tmptarget=NULL;
	DBC *dbc=NULL;
	DBT k,d;

	if(diagnrd)
	{
		prterr("DIAG EQLNRDKEYberkeley Searching for Equal Key in Module [%s] File [%d] [%s] Keyno [%d] using Berkeley Btree Database Engine.",MODULENAME(fileno),fileno,FILENAME(fileno),keyno);
	}
	NRDtmp=NRDbuf+fileno;
	if((NRDbuf[fileno].nokeys+1)<keyno || keyno<1) 
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s].  Using Keyno [1] instead.",keyno,MODULENAME(fileno),fileno,FILENAME(fileno));
		keyno=1;
	}
	key=NRDtmp->key+(keyno-1);
	x=key->dbp->cursor(key->dbp,NULL,&dbc,0);
	if(x!=0)
	{
		prterr("Error: key->dbp->cursor() %d %s Unable to create cursor/pointer for the key [%s].",x,db_strerror(x),key->name);
		return(-1);
	} else {
		tmptarget=NRDtmp2target(NRDtmp,key,&tlen);
		if(target==NULL)
		{
			target=Rmalloc(tlen+1);
		}
		memcpy(tmptarget,target,tlen);
		memset(&k,0,sizeof(DBT));
		memset(&d,0,sizeof(DBT));
		k.data=tmptarget;
		k.size=tlen;
		return_value=dbc->c_get(dbc,&k,&d,DB_SET_RANGE);
		if(!return_value)
		{
			memset(&d,0,sizeof(DBT));
			return_value=dbc->c_get(dbc,&k,&d,DB_CURRENT);
		}
		if(tmptarget!=NULL) Rfree(tmptarget);
		if(!return_value)
		{
			QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,d.size+1);
			memset(NRDtmp->record_data,0,NRDtmp->dlen);
			memcpy(NRDtmp->record_data,d.data,d.size);
			assignCtree_fields(NRDtmp);
			if(dbc!=NULL)
			{
				dbc->c_close(dbc);
				dbc=NULL;
			}
			if(diagnrd) { xSEENRDRECORD(fileno,__LINE__,__FILE__);prterr("Exiting EQLNRDberkeley OK for Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name); }
			return(0);
		}
	}
	if(dbc!=NULL)
	{
		dbc->c_close(dbc);
		dbc=NULL;
	}
	if(diagnrd)
	{
		prterr("DIAG Specified Key Value not found in Module [%s] File [%s].",NRDtmp->modname,NRDtmp->name);
	}
	return(1);
}
short EQLNRDberkeley(short fileno,short keyno)
{
	short return_value=0;
	NRDbuffer *NRDtmp;
	NRDkey *key=NULL;
	void *target=NULL;

	if(diagnrd) { prterr("DIAG EQLNRDberkeley Searching for equal key  with Berkeley Btree D-Base Engine."); }
	NRDtmp=NRDbuf+fileno;
	if((NRDtmp->nokeys+1)<keyno || keyno<1)
	{
		prterr("Error Keyno [%d] Out of Bounds for Module [%s] File [%d] [%s].  Using Keyno [1] instead.",keyno,MODULENAME(fileno),fileno,FILENAME(fileno));
		keyno=1;
	}
	key=NRDtmp->key+(keyno-1);
	target=NRDtmp2target(NRDtmp,key,&return_value);
	return_value=EQLNRDKEYberkeley(fileno,keyno,target);
	if(target!=NULL) Rfree(target);
	return(return_value);
}

short WRTNRDberkeley(short fileno,short trnmode,char *fieldname)
{
	NRDbuffer *NRDtmp;
	unsigned length=0;
	short return_value=0,useit=0;
	NRDfield *field;
	NRDkey *key;
	void *target=NULL;
	DBT k,d;
#ifdef USE_LOCKING
#ifdef USE_WRITECURSOR
	DBC *dbc=NULL;
#endif /* USE_WRITECURSOR */
#endif /* USE_LOCKING */

	if(diagnrd)
	{
		prterr("DIAG Writing current record to file [%s] trnmode [%d] fieldname [%s] using Berkeley Btree Engine.",NRDbuf[fileno].name,trnmode,(fieldname!=NULL ? fieldname:""));
	}
	NRDtmp=NRDbuf+fileno;
	key=NRDtmp->key;
	length=CTREERecordLength(NRDtmp,TRUE);
	if(diagnrd)
	{
		prterr("DIAG Length of data record [%d]",length);
	}
	QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,length+1);
	memset(NRDtmp->record_data,0,NRDtmp->dlen);
	if(fieldname!=NULL && RDAstrlen(fieldname)>0)
	{
		field=xFLDNRD(fileno,fieldname,__LINE__,__FILE__);
		if(field->type!=SCROLLEDLIST && field->type!=LONGV && 
			field->type!=SLONGV && field->type!=SHORTV &&
			field->type!=SSHORTV && field->type!=DOUBLEV &&
			field->type!=SDOUBLEV) field=NULL;
	} else field=NULL;
	if(trnmode && field!=NULL)/*get next tran num automatically*/
	{
		SetTrnModeField(fileno,fieldname);
	}
#ifdef USE_LOCKING
#ifdef USE_WRITECURSOR
	if(key->dbp->cursor(key->dbp,NULL,&dbc,DB_WRITECURSOR))
	{
		prterr("Error: dbp->cursor() Unable to create cursor/pointer for the key [%s].",key->name);
		return(-1);
	}
#endif /* USE_WRITECURSOR */
#endif /* USE_LOCKING */
	length=CTREERecordLength(NRDtmp,TRUE);
	QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,length+1);
	assignCTREERecordData(NRDtmp,TRUE);
	target=NRDtmp2target(NRDtmp,key,&useit);
	memset(&k,0,sizeof(DBT));
	k.data=target;
	k.size=useit+1;
	memset(&d,0,sizeof(DBT));
	d.data=NRDtmp->record_data;
	d.size=length+1;
	return_value=NRDtmp->dbp->put(NRDtmp->dbp,NULL,&k,&d,0);
	if(target!=NULL) Rfree(target);
#ifdef USE_LOCKING
#ifdef USE_WRITECURSOR
	if(dbc!=NULL)
	{
		dbc->c_close(dbc);
		dbc=NULL;
	}
#endif /* USE_WRITECURSOR */
#endif /* USE_LOCKING */
	switch(return_value)
	{
		default:
		case 0:
			if(diagnrd) { prterr("DIAG exiting wrtnrd OK."); }
			break;
		case DB_LOCK_DEADLOCK:
			prterr("Error %d: (DB_LOCK_DEADLOCK) Attempting to Write Berkeley Btree record in Module [%s] File [%s].",return_value,NRDtmp->modname,NRDtmp->name);
			break;
		case EACCES:
			prterr("Error %d: (READ-ONLY) Attempting to Write Berkeley Btree record in Module [%s] File [%s].",return_value,NRDtmp->modname,NRDtmp->name);
			break;
		case ENOSPC:
			prterr("Error %d: (BTREE Exceeded Maximum Depth 255) Attempting to Write Berkeley Btree record in Module [%s] File [%s].",return_value,NRDtmp->modname,NRDtmp->name);
			break;
		case EINVAL:
			prterr("Error %d: (Invalid Value) Attempting to Write Berkeley Btree record in Module [%s] File [%s].",return_value,NRDtmp->modname,NRDtmp->name);
			break;
	}
	return(return_value);
}
short CLSNRDberkeley(short fileno)
{
	int x=0;
#ifdef REMOVE_ENVIRONMENT
	char *temp=NULL;
#endif /* REMOVE_ENVIRONMENT */
	NRDbuffer *NRDtmp;
	NRDkey *key=NULL;

	if(diagnrd) { prterr("DIAG Closing file [%s] using Berkeley Engine.",NRDbuf[fileno].name); }
	NRDtmp=NRDbuf+fileno;
	for(x=1;x<NRDtmp->nokeys;++x)
	{
		key=NRDtmp->key+x;
		key->dbp->sync(key->dbp,0);
		key->dbp->close(key->dbp,0);
		key->dbp=NULL;
	}
	NRDtmp->dbp->sync(NRDtmp->dbp,0);
	NRDtmp->dbp->close(NRDtmp->dbp,0);
#ifdef USE_ENVIRONMENT
#ifdef USE_LOCKING
	BerkeleyENV->lock_id_free(BerkeleyENV,NRDtmp->lockerID);
#endif
#endif /* USE_ENVIRONMENT */
	return(0);
}
DB *OPNNRDberkeley(NRDbuffer *NRDtmp)
{
	short x=0,r=FALSE,y=0;
	char *filename=NULL;
	NRDkey *key=NULL;
	DB *db=NULL;
	int len=0;
#ifdef USE_SETPAGESIZE
	short lenx=0,min_c=0;
	int block=DEFAULT_BLOCKSIZE;
	char *target=NULL;
#endif /* USE_SETPAGESIZE */

	if(diagnrd) { prterr("DIAG Open file [%s] using Berkeley Engine.",NRDtmp->name); }

	NRDtmp->dbp=NULL;
	if(db_create(&NRDtmp->dbp,BerkeleyENV,0))
	{
		prterr("Error:  db_create failed.");
		r=(-1);
	}
#ifdef USE_RDA_ALLOC
	NRDtmp->dbp->set_alloc(NRDtmp->dbp,Bmalloc,Brealloc,Bfree);
#endif /* USE_RDA_ALLOC */
	filename=Rmalloc(RDAstrlen(NRDtmp->modname)+RDAstrlen(NRDtmp->name)+25);
#ifndef WIN32
	sprintf(filename,"%s/%s.dat",NRDtmp->modname,NRDtmp->name);
#endif
#ifdef WIN32
	sprintf(filename,"%s\\%s.dat",NRDtmp->modname,NRDtmp->name);
#endif
#ifdef USE_SETPAGESIZE
	len=CTREERecordLength(NRDtmp,TRUE);
	key=NRDtmp->key;
	target=NRDtmp2target(NRDtmp,key,&lenx);
	if(target!=NULL) Rfree(target);
	while((2*(len+lenx))>block)
	{
		block+=DEFAULT_BLOCKSIZE;
	}
	min_c=(block/(((len+lenx))));
	if(min_c<2) min_c=2;
	prterr("block [%d] min_c [%d] key len [%d] data len [%d] ",block,min_c,lenx,len);TRACE;
	NRDtmp->dbp->set_pagesize(NRDtmp->dbp,block);
	NRDtmp->dbp->set_bt_minkey(NRDtmp->dbp,min_c);
#endif /* USE_SETPAGESIZE */
#ifdef USE_ERRFILE
	NRDtmp->dbp->set_errfile(NRDtmp->dbp,RDA_STDERR);
	NRDtmp->dbp->set_errpfx(NRDtmp->dbp,PROGRAM_NAME);
#endif /* USE_ERRFILE */

#ifdef USE_RECNUM
	NRDtmp->dbp->set_flags(NRDtmp->dbp,DB_RECNUM);
#endif /* USE_RECNUM */
	y=NRDtmp->dbp->open(NRDtmp->dbp,NULL,filename,NRDtmp->name,DB_BTREE,0,0664);
	if(y!=0)
	{
		prterr("Error: %d %s NRDtmp->dbp->open failed creating the database %s.",y,db_strerror(y),filename);
		r=(-1);
	}

	if(filename!=NULL) Rfree(filename);
	NRDtmp->dbp->app_private=NRDtmp;
#ifdef USE_ENVIRONMENT
#ifdef USE_LOCKING
	
	BerkeleyENV->lock_id(BerkeleyENV,&NRDtmp->lockerID);
#endif
#endif /* USE_ENVIRONMENT */
#ifdef KEYNAME1
	filename=Rmalloc(RDAstrlen(NRDtmp->modname)+RDAstrlen(NRDtmp->name)+25);
#ifndef WIN32
	sprintf(filename,"%s/%s.idx",NRDtmp->modname,NRDtmp->name);
#endif
#ifdef WIN32
	sprintf(filename,"%s\\%s.idx",NRDtmp->modname,NRDtmp->name);
#endif
#endif /* KEYNAME1 */
	key=NRDtmp->key;
	key->dbp=NRDtmp->dbp;
	for(x=1;x<NRDtmp->nokeys;++x)
	{
#ifdef KEYNAME2
		if(filename!=NULL) Rfree(filename);
		filename=Rmalloc(RDAstrlen(NRDtmp->modname)+RDAstrlen(NRDtmp->name)+25);
#ifndef WIN32
		sprintf(filename,"%s/%s.id%d",NRDtmp->modname,NRDtmp->name,x);
#endif
#ifdef WIN32
		sprintf(filename,"%s\\%s.id%d",NRDtmp->modname,NRDtmp->name,x);
#endif
#endif /* KEYNAME2 */
		key=NRDtmp->key+x;
		key->dbp=NULL;
		if(db_create(&key->dbp,BerkeleyENV,0))
		{
			prterr("Error:  db_create failed.");
			r=(-1);
		}
#ifdef USE_RDA_ALLOC
		key->dbp->set_alloc(key->dbp,Bmalloc,Brealloc,Bfree);
#endif /* USE_RDA_ALLOC */
#ifdef USE_ERRFILE
		key->dbp->set_errfile(key->dbp,RDA_STDERR);
		key->dbp->set_errpfx(key->dbp,PROGRAM_NAME);
#endif /* USE_ERRFILE */

		y=key->dbp->open(key->dbp,NULL,filename,key->name,DB_BTREE,0,0664);
		if(y!=0)
		{
			prterr("Error: %d %s key->dbp->open failed creating the index %s for %s.",y,db_strerror(y),filename,key->name);
			r=(-1);
		}
		key->dbp->app_private=key;
		if(NRDtmp->dbp->associate(NRDtmp->dbp,NULL,key->dbp,MakeKeyDBT,0))
		{
			prterr("Error: key->dbp->associate failed creating the index %s for %s.",filename,key->name);
			r=(-1);
		}
	}
	len=CTREERecordLength(NRDtmp,TRUE);
	QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,len+1);
	memset(NRDtmp->record_data,0,NRDtmp->dlen);
	if(filename!=NULL) Rfree(filename);
	if(!r) db=NRDtmp->dbp;
	return(db);
}
int MakeKeyDBT(DB *d,const DBT *key,const DBT *data,DBT *newkey)
{
	NRDbuffer *NRDtmp=NULL;
	NRDkey *keyx=NULL;
	void *target=NULL;
	short len=0;

	if(d->app_private==NULL) 
	{
		prterr("Error:  MakeKeyDBT couldn't obtain handles...app_private is NULL.");
		return(-1);
	}
	keyx=(NRDkey *)d->app_private;
	NRDtmp=NRDbuf+(keyx->fileno); 
	target=NRDtmp2target(NRDtmp,keyx,&len);
	newkey->flags=DB_DBT_APPMALLOC;
	newkey->data=target;
	newkey->size=len;	
	return(0);
}
short CRTNRDberkeley(NRDbuffer *NRDtmp)
{
	short x=0,r=FALSE,y=0;
	char *filename=NULL;
	NRDkey *key=NULL;
#ifdef USE_SETPAGESIZE
	int len=0,min_c=0;
	short lenx=0;
	int block=DEFAULT_BLOCKSIZE;
	char *target=NULL;
#endif /* USE_SETPAGESIZE */

	if(db_create(&NRDtmp->dbp,BerkeleyENV,0))
	{
		prterr("Error:  db_create failed.");
		r=(-1);
	}
	filename=Rmalloc(RDAstrlen(NRDtmp->modname)+RDAstrlen(NRDtmp->name)+25);
#ifndef WIN32
	sprintf(filename,"%s/%s.dat",NRDtmp->modname,NRDtmp->name);
#endif
#ifdef WIN32
	sprintf(filename,"%s\\%s.dat",NRDtmp->modname,NRDtmp->name);
#endif
#ifdef USE_RDA_ALLOC
	NRDtmp->dbp->set_alloc(NRDtmp->dbp,Bmalloc,Brealloc,Bfree);
#endif /* USE_RDA_ALLOC */
#ifdef USE_SETPAGESIZE
	len=CTREERecordLength(NRDtmp,TRUE);
	key=NRDtmp->key;
	target=NRDtmp2target(NRDtmp,key,&lenx);
	if(target!=NULL) Rfree(target);
	while((2*(len+lenx))>block)
	{
		block+=DEFAULT_BLOCKSIZE;
	}
	min_c=(block/(((len+lenx))));
	if(min_c<2) min_c=2;
	prterr("block [%d] min_c [%d] key len [%d] data len [%d] ",block,min_c,lenx,len);TRACE;
	NRDtmp->dbp->set_pagesize(NRDtmp->dbp,block);
	NRDtmp->dbp->set_bt_minkey(NRDtmp->dbp,min_c);
#endif /* USE_SETPAGESIZE */
#ifdef USE_ERRFILE
	NRDtmp->dbp->set_errfile(NRDtmp->dbp,RDA_STDERR);
	NRDtmp->dbp->set_errpfx(NRDtmp->dbp,PROGRAM_NAME);
#endif /* USE_ERRFILE */
#ifdef USE_RECNUM
	NRDtmp->dbp->set_flags(NRDtmp->dbp,DB_RECNUM);
#endif /* USE_RECNUM */
	y=NRDtmp->dbp->open(NRDtmp->dbp,NULL,filename,NRDtmp->name,DB_BTREE,DB_CREATE,0664);
	if(y!=0)
	{
		prterr("Error: %d %s NRDtmp->dbp->open failed creating the database %s.",y,db_strerror(y),filename);
		r=(-1);
	}
	NRDtmp->dbp->app_private=NRDtmp;
	if(filename!=NULL) Rfree(filename);
#ifdef KEYNAME1
	filename=Rmalloc(RDAstrlen(NRDtmp->modname)+RDAstrlen(NRDtmp->name)+25);
#ifndef WIN32
	sprintf(filename,"%s/%s.idx",NRDtmp->modname,NRDtmp->name);
#endif
#ifdef WIN32
	sprintf(filename,"%s\\%s.idx",NRDtmp->modname,NRDtmp->name);
#endif
#endif /* KEYNAME1 */
	key=NRDtmp->key;
	key->dbp=NRDtmp->dbp;
	for(x=1;x<NRDtmp->nokeys;++x)
	{
#ifdef KEYNAME2
		if(filename!=NULL) Rfree(filename);
		filename=Rmalloc(RDAstrlen(NRDtmp->modname)+RDAstrlen(NRDtmp->name)+25);
#ifndef WIN32
		sprintf(filename,"%s/%s.id%d",NRDtmp->modname,NRDtmp->name,x);
#endif
#ifdef WIN32
		sprintf(filename,"%s\\%s.id%d",NRDtmp->modname,NRDtmp->name,x);
#endif
#endif /* KEYNAME2 */
		key=NRDtmp->key+x;
		if(db_create(&key->dbp,BerkeleyENV,0))
		{
			prterr("Error:  db_create failed.");
			r=(-1);
		}
#ifdef USE_DUPS
		key->dbp->set_flags(key->dbp,DB_DUP | DB_DUPSORT);
#endif
#ifdef USE_RDA_ALLOC
		key->dbp->set_alloc(key->dbp,Bmalloc,Brealloc,Bfree);
#endif /* USE_RDA_ALLOC */
#ifdef USE_ERRFILE
		key->dbp->set_errfile(key->dbp,RDA_STDERR);
		key->dbp->set_errpfx(key->dbp,PROGRAM_NAME);
#endif /* USE_ERRFILE */
		y=key->dbp->open(key->dbp,NULL,filename,key->name,DB_BTREE,DB_CREATE ,0664);
		if(y!=0)
		{
			prterr("Error: %d %s key->dbp->open failed creating the index %s for %s.",y,db_strerror(y),filename,key->name);
			r=(-1);
		}
		key->dbp->app_private=key;
		if(NRDtmp->dbp->associate(NRDtmp->dbp,NULL,key->dbp,MakeKeyDBT,0))
		{
			prterr("Error: key->dbp->associate failed creating the index %s for %s.",filename,key->name);
			r=(-1);
		}
	}
	if(filename!=NULL) Rfree(filename);
	for(x=1;x<NRDtmp->nokeys;++x)
	{
		key=NRDtmp->key+1;
		key->dbp->close(key->dbp,0);
	}
	NRDtmp->dbp->close(NRDtmp->dbp,0);
	NRDtmp->dbp=NULL;
	return(r);
}
void EXITNRDberkeley()
{
#ifdef USE_ENVIRONMENT
	char *temp=NULL;

	if(BerkeleyENV!=NULL)
	{
		BerkeleyENV->close(BerkeleyENV,0);
		BerkeleyENV=NULL;
	}
	if(db_env_create(&BerkeleyENV,0))
	{
		prterr("Error:  db_env_create Unable to initialize Berkeley Environment.");
		BerkeleyENV=NULL;
		return;
	} else {
		TRACE;
#ifdef USE_DIRECTORY
		temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+25);
#ifndef WIN32
		sprintf(temp,"%s/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
		sprintf(temp,"%s\\",CURRENTDIRECTORY);
#endif
		BerkeleyENV->set_data_dir(BerkeleyENV,temp);
		BerkeleyENV->set_lg_dir(BerkeleyENV,temp);
		BerkeleyENV->set_tmp_dir(BerkeleyENV,temp);
#endif /* USE_DIRECTORY */
		BerkeleyENV->remove(BerkeleyENV,temp,0);
		BerkeleyENV=NULL;
	}
#endif /* USE_ENVIRONMENT */
}
short INITNRDberkeley()
{
#ifdef USE_ENVIRONMENT
	char *temp=NULL;
	int e=0;

	if(db_env_create(&BerkeleyENV,0))
	{
		prterr("Error:  db_env_create Unable to initialize Berkeley Environment.");
		BerkeleyENV=NULL;
		return(-1);
	} else {
		TRACE;
#ifdef USE_DIRECTORY
		temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+25);
#ifndef WIN32
		sprintf(temp,"%s/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
		sprintf(temp,"%s\\",CURRENTDIRECTORY);
#endif
		BerkeleyENV->set_data_dir(BerkeleyENV,temp);
		BerkeleyENV->set_lg_dir(BerkeleyENV,temp);
		BerkeleyENV->set_tmp_dir(BerkeleyENV,temp);
#endif /* USE_DIRECTORY */
#ifdef USE_ERRFILE
		BerkeleyENV->set_errfile(BerkeleyENV,RDA_STDERR);
		BerkeleyENV->set_errpfx(BerkeleyENV,PROGRAM_NAME);
#endif /* USE_ERRFILE */
		TRACE;
#ifdef USE_SETCACHE
		if(BerkeleyENV->set_cachesize(BerkeleyENV,0,5*1024*1024,0))
		{
			prterr("Error: BerkeleyENV->set_cachesize failed.");
			BerkeleyENV->close(BerkeleyENV,0);
			BerkeleyENV=NULL;
			return(-1);
		}
		TRACE;
#endif /* USE_SETCACHE */
#ifdef USE_RDA_ALLOC
		BerkeleyENV->set_alloc(BerkeleyENV,Bmalloc,Brealloc,Bfree);
#endif /* USE_RDA_ALLOC */
#ifndef USE_LOCKING
		TRACE;
/*
		BerkeleyENV->set_flags(BerkeleyENV,DB_NOLOCKING,TRUE);
*/
		TRACE;
#endif
#ifdef USE_LOCKING
		TRACE;
		BerkeleyENV->set_lk_detect(BerkeleyENV,DB_LOCK_DEFAULT);
		TRACE;
#ifdef USE_NOMMAP
		BerkeleyENV->set_flags(BerkeleyENV,DB_NOMMAP,TRUE);
#endif /* USE_NOMMAP */
		TRACE;
#endif
		TRACE;
		e=BerkeleyENV->open(BerkeleyENV,NULL,DB_JOINENV,0664);
		TRACE;
		prterr("ENV->open [%d] ",e);TRACE;
		if(e!=0)
		{
#ifndef USE_LOCKING
#ifndef WIN32
			e=BerkeleyENV->open(BerkeleyENV,NULL,DB_CREATE ,0);
#endif 
#ifdef WIN32
			e=BerkeleyENV->open(BerkeleyENV,NULL,DB_CREATE ,0);
#endif
#endif
#ifdef USE_LOCKING
			e=BerkeleyENV->open(BerkeleyENV,NULL,DB_CREATE | DB_INIT_CDB ,0664);
#endif
		}
		if(temp!=NULL) Rfree(temp);
		prterr("ENV->open [%d] ",e);TRACE;
		switch(e)
		{
			default:
			case 0:
				return(0);
			case EAGAIN:
				prterr("Error EAGAIN: (Shared memory was unavailable) BerkeleyENV->open  Unable to open BerkeleyENV error %d.",e);
				BerkeleyENV->close(BerkeleyENV,0);
				BerkeleyENV=NULL;
				return(-1);
			case EINVAL:
				prterr("Error EINVAL: (Invalid Flag or Parameter) BerkeleyENV->open  Unable to open BerkeleyENV error %d.",e);
				BerkeleyENV->close(BerkeleyENV,0);
				BerkeleyENV=NULL;
				return(-1);
			case ENOSPC:
				prterr("Error ENOSPC: (Previous Berkeley Environment) BerkeleyENV->open  Unable to open BerkeleyENV error %d.",e);
				BerkeleyENV->close(BerkeleyENV,0);
				BerkeleyENV=NULL;
				return(-1);
		}
	}
#endif /* USE_ENVIRONMENT */
}
#endif /* USE_BERKELEY_BTREE */
/*lint +e525*/
