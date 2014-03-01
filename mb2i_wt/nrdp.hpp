
/******************************************************************************
			NEW RDA DATABASE PRIVATE HEADER
				 nrdp.h
	purpose: Contains macros and function prototypes for the rda data
		 base manager interface that are private from applications.
*******************************************************************************/

#ifndef NRDP_H
#define NRDP_H

#include <nrdp1.hpp>
#include <rdadta.hpp>
#include <nrd.hpp>
#include <misc.hpp>

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <limits.h>

#ifndef WIN32
#include <unistd.h>
#include <fcntl.h>
#endif

#ifdef USE_MYISAM
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
#endif

#ifdef WIN32
#include<direct.h>
#include<io.h>
#endif
#ifdef USE_CTREE
#include "ctstdr.h"
#include "ctoptn.h"
#include "ctaerr.h"
#include "cterrc.h"
#include "ctdecl.h"
#include "ctifil.h"
#endif
#ifdef INFORMIX_ISAM
#include <isam.h>
#endif
#ifdef USE_BERKELEY_BTREE
#include <db.h>
#endif

struct NRDservers
{
	char *name;
	short engine;
};
typedef struct NRDservers NRDserver;

struct NRDconnects
{
	int cur;
	int num;
	NRDserver *s;
};
typedef struct NRDconnects NRDconnect;

extern NRDconnect dbcons;

#define getnrd(x,y)	xgetnrd(x,y,__LINE__,__FILE__)
#define putnrd(a,b,c,d,e,f,g,h)	xputnrd(a,b,c,d,e,f,g,h,__LINE__,__FILE__)
#ifdef USE_CTREE
#ifdef XXXXX
extern pCTGVAR ctWNGV;
#endif
#endif

/* this structure needs to be made dynamic at runtime */

#define FreeNRDconnect()	xFreeNRDconnect(__LINE__,__FILE__)
void xFreeNRDconnect(int,char *);
#define RecordNRDconnect(a,b)	xRecordNRDconnect(a,b,__LINE__,__FILE__)
short xRecordNRDconnect(char *,short,int,char *);

#if (defined(USE_CTREE) || defined(INFORMIX_ISAM))
void assignNRDRecordData(NRDbuffer *,short);
#define IsamRecordLength(a)	xIsamRecordLength(a,TRUE,__LINE__,__FILE__)
#define ADVIsamRecordLength(a,b)	xIsamRecordLength(a,b,__LINE__,__FILE__)
int xIsamRecordLength(NRDbuffer *,short,int,char *);
void assignCtree_fields(NRDbuffer *);
void assignIsam_fields(NRDbuffer *);
#endif
/*--------------------------------------------------------------------------
These will be used for NRDCtreeEngine 
--------------------------------------------------------------------------*/
#if (defined(USE_CTREE) || defined(USE_BERKELEY_BTREE))
#define CTREERecordLength(a,b)	xCTREERecordLength(a,b,__LINE__,__FILE__)
int xCTREERecordLength(NRDbuffer *,short,int,char *);
void assignCTREERecordData(NRDbuffer *,short);
#endif
#ifdef USE_CTREE
short checkvariable(NRDbuffer *);
void seeifil(IFIL *);
short INITNRDctree(void);
short CRTNRDctree(NRDbuffer *);
short OPNNRDctree(NRDbuffer *,short);
short CLSNRDctree(short);
short EQLNRDKEYctree(short,short,void *);
short EQLNRDctree(short,short);
short LTENRDctree(short,short);
short GTENRDctree(short,short);
short FRSNRDctree(short,short);
short NXTNRDctree(short,short);
long SIZNRDctree(short);
short WRTNRDctree(short,short,char *);
short RBDNRDctree(NRDbuffer *);
short PRVNRDctree(short,short);
short LSTNRDctree(short,short);
short LOCNRDctree(short);
short UNLNRDctree(short);
short LOCNRDFILEctree(short);
short UNLNRDFILEctree(short);
short DELNRDctree(short);
short BEGNRDctree(short);
short SEQNRDctree(short);
#define ShowDODA(a,b,c)	xShowDODA(a,b,c,__LINE__,__FILE__)
void xShowDODA(int,DATOBJ *,short,int,char *);
unsigned getdreclen(NRDbuffer *);
short ctdbnum(short);
char *ctdb_errors(short);
#endif /* ifdef USE_CTREE */
#define NRDtmp2target(a,b,c)	xNRDtmp2target(a,b,c,__LINE__,__FILE__)
void *xNRDtmp2target(NRDbuffer *,NRDkey *,short *,int,char *);
#define maketarget(x,y,z)	xmaketarget(x,y,z,__LINE__,__FILE__)
void *xmaketarget(short,short,short *,int,char *);
#define target2NRDbuf(a,b,c)	xtarget2NRDbuf(a,b,c,__LINE__,__FILE__)
void xtarget2NRDbuf(short,short,void *,int,char *);
#define setrdakeys(a,b)	xsetrdakeys(a,b,__LINE__,__FILE__)
short DELETENRDrda(char *,char *);
DTA CRTNRDrda(NRDbuffer *);
DTA OPNNRDrda(NRDbuffer *);
short CLSNRDrda(short);
short EQLNRDKEYrda(short,void *);
short EQLNRDrda(short,short);
short GTENRDrda(short,short);
short PRVNRDrda(short,short);
short LSTNRDrda(short,short);
short BEGNRDrda(short);
short SEQNRDrda(short);
short LTENRDrda(short,short);
short RESNRDrda(NRDbuffer *);
short FRSNRDrda(short,short);
short NXTNRDrda(short,short);
long SIZNRDrda(short);
short WRTNRDrda(short,short,char *);
short RBDNRDrda(NRDbuffer *);
short LOCNRDFILErda(short);
short UNLNRDFILErda(short);
short LOCNRDrda(short);
short UNLNRDrda(short);
short get_length(NRDbuffer *,char *);
#define freenrd(a)	{xfreenrd(a,__LINE__,__FILE__);a=NULL;}
void xfreenrd(NRDbuffer *,int line,char *file);
void assign_rdafields(NRDbuffer *);
short xputnrd(char *,char *,short,short,NRDfield *,short,NRDkey *,char *,int,char *);
short xgetnrd(char *,char *,int,char *);
short DELNRDrda(short);
#define FINDFLD(x,y)	xFINDFLD(x,y,__LINE__,__FILE__)
NRDfield *xFINDFLD(NRDbuffer *,char *,int,char *);
	/* Retreive field address of current record & file.
		1st arg NRDbuffer structure (for the data file) using.
		2nd arg is the fieldname searching for.
		returns pointer to field, NULL if field not found.*/
#ifdef INFORMIX_ISAM
short DELETENRDisam(char *,char *);
short INITNRDisam(void);
short CRTNRDisam(NRDbuffer *);
short OPNNRDisam(NRDbuffer *);
short OPNEXCLCKNRDisam(NRDbuffer *);
short CLSNRDisam(short);
short DELNRDisam(short);
short EQLNRDKEYisam(short,short,void *);
short EQLNRDisam(short,short);
short GTENRDisam(short,short);
short PRVNRDisam(short,short);
short LSTNRDisam(short,short);
short BEGNRDisam(short);
short SEQNRDisam(short);
short LTENRDisam(short,short);
short FRSNRDisam(short,short);
short NXTNRDisam(short,short);
long SIZNRDisam(short);
short WRTNRDisam(short,short,char *);
short RBDNRDisam(char *,char *);
short LOCNRDFILEisam(short);
short UNLNRDFILEisam(short);
short LOCNRDisam(short);
short UNLNRDisam(short);
#endif /* INFORMIX_ISAM */

#ifdef USE_BERKELEY_BTREE
int MakeKeyDBT(DB *,const DBT *,const DBT *,DBT *);
short CRTNRDberkeley(NRDbuffer *);
short LOCNRDFILEberkeley(short);
short LOCNRDberkeley(short);
short UNLNRDFILEberkeley(short);
short UNLNRDberkeley(short);
DB *OPNNRDberkeley(NRDbuffer *);
short CLSNRDberkeley(short);
short WRTNRDberkeley(short,short,char *);
short DELNRDberkeley(short);
short EQLNRDKEYberkeley(short,short,void *);
short EQLNRDberkeley(short,short);
short GTENRDberkeley(short,short);
short PRVNRDberkeley(short,short);
short LSTNRDberkeley(short,short);
short BEGNRDberkeley(short);
short SEQNRDberkeley(short);
short LTENRDberkeley(short,short);
short FRSNRDberkeley(short,short);
short NXTNRDberkeley(short,short);
long SIZNRDberkeley(short);
void EXITNRDberkeley(void);
short INITNRDberkeley(void);
#endif /* USE_BERKELEY_BTREE */

#ifdef USE_MYSQL
short LOCNRDFILEmysqlxpert(short);
short LOCNRDmysqlxpert(short);
short UNLNRDFILEmysqlxpert(short);
short UNLNRDmysqlxpert(short);
MYSQL *OPNNRDmysqlxpert(NRDbuffer *);
short CLSNRDmysqlxpert(short);
short WRTNRDmysqlxpert(short,short,char *);
short CRTNRDmysql(NRDbuffer *);
short CRTNRDmysqlxpert(NRDbuffer *);
short LOCNRDFILEmysql(short);
short LOCNRDmysql(short);
short UNLNRDFILEmysql(short);
short UNLNRDmysql(short);
MYSQL *OPNNRDmysql(NRDbuffer *);
short CLSNRDmysql(short);
short WRTNRDmysql(short,short,char *);
short DELNRDmysql(short);
short EQLNRDKEYmysql(short,short,void *);
short EQLNRDmysql(short,short);
short GTENRDmysql(short,short);
short PRVNRDmysql(short,short);
short LSTNRDmysql(short,short);
short BEGNRDmysql(short);
short SEQNRDmysql(short);
short LTENRDmysql(short,short);
short FRSNRDmysql(short,short);
short NXTNRDmysql(short,short);
long SIZNRDmysql(short);
void EXITNRDmysql(void);
void INITNRDmysql(void);
short DELETENRDmysql(char *,char *,char *);
#endif /* USE_MYSQL */
#ifdef USE_MYISAM 
short CRTNRDmyisam(NRDbuffer *);
short LOCNRDFILEmyisam(short);
short LOCNRDmyisam(short);
short UNLNRDFILEmyisam(short);
short UNLNRDmyisam(short);
short OPNNRDmyisam(NRDbuffer *);
short OPNEXCLCKNRDmyisam(NRDbuffer *);
short CLSNRDmyisam(short);
short WRTNRDmyisam(short,short,char *);
short DELNRDmyisam(short);
short EQLNRDKEYmyisam(short,short,void *);
short EQLNRDmyisam(short,short);
short GTENRDmyisam(short,short);
short PRVNRDmyisam(short,short);
short LSTNRDmyisam(short,short);
short BEGNRDmyisam(short);
short SEQNRDmyisam(short);
short LTENRDmyisam(short,short);
short FRSNRDmyisam(short,short);
short NXTNRDmyisam(short,short);
long SIZNRDmyisam(short);
void EXITNRDmyisam(void);
short INITNRDmyisam(void);
short RBDNRDmyisam(char *,char *);
short RESNRDmyisam(char *,char *);
short DELETENRDmyisam(char *,char *);
#define MyIsamRecordLength(a)	xMyIsamRecordLength(a,TRUE,__LINE__,__FILE__)
#define ADVMyIsamRecordLength(a,b)	xMyIsamRecordLength(a,b,__LINE__,__FILE__)
void assignMyIsam_fields(NRDbuffer *);
#define MyIsam2target(a,b,c)	xMyIsam2target(a,b,c,__LINE__,__FILE__)
void *xMyIsam2target(NRDbuffer *,NRDkey *,short *,int,char *);
#endif /* USE_MYISAM */
#endif /*NRDP_H*/

