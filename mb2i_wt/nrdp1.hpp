
/******************************************************************************
			NEW RDA DATABASE PRIVATE HEADER
				 nrdp.h
	purpose: Contains macros and function prototypes for the rda data
		 base manager interface that are private from applications.
*******************************************************************************/

#ifndef NRDP1_H
#define NRDP1_H

#include <cstdio>
#include <cstdlib>

#ifdef USE_MYISAM
#include "limits.h"
#include "my_global.h"
#ifndef WIN32
#include "config.h"
#endif 
#include "my_sys.h"
#include "m_ctype.h"
#include "myisam.h"
#endif
#ifdef USE_MYSQL
#include <mysql.h>
#endif


#ifndef WIN32
#include <unistd.h>
#include <fcntl.h>
#endif

#include <rdadta.hpp>
#include <nrd.hpp>
#include <misc.hpp>

struct NRDbuffers
{
	char not_used;
	char db_engine;
	char *servername;
	char *modname;
	char *name; /* name of the datafile */
	short nofields;
	NRDfield *field;
	short nokeys;
	NRDkey *key;
	char *record_data; /* records data contents */
	int dlen;
	short filenum; /* database filenum */
	DTA dtanum;
	long currec;
	char varfile;
#ifdef USE_BERKELEY_BTREE
	db_recno_t recno;
	DB_ENV *dbenv;
	DB *dbp;
	u_int32_t lockerID;
	DB_LOCK lock;
#endif
#ifdef USE_MYSQL
	MYSQL *db;
	MYSQL_RES *result;
	MYSQL_ROW row;
#endif
#ifdef USE_MYISAM
	MI_INFO *myinfo;
	char *record_data2; /* records data contents */
	int dlen2;
	char *target;
	int tlen;
#endif
};
typedef struct NRDbuffers NRDbuffer;

extern short no_buffers; /* represents the number of NRDbuffers */
extern NRDbuffer *NRDbuf; /* dynamically arrayed structure for all files(buffers) */
#define NRDtmpGETBIN(a,b)	xNRDtmpGETBIN(a,b,__LINE__,__FILE__)
NRDbuffer *xNRDtmpGETBIN(char *,char *,int,char *);
#define freenrd(a)	{xfreenrd(a,__LINE__,__FILE__);a=NULL;}
void xfreenrd(NRDbuffer *,int,char *);
#define SEENRDbuf()	xSEENRDbuf(__LINE__,__FILE__)
void xSEENRDbuf(int,char *);

#endif /*NRDP1_H*/

