
/******************************************************************************
			TRANSACTION PROCESSING HEADER
				trans.h 
	purpose: Contains the macros and functions for transaction processing
		 and log processing. 
*******************************************************************************/

#ifndef TRANSH_P
#define TRANSH_P
#include <nrd.hpp>
#include <misc.hpp>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#ifndef WIN32
#include <unistd.h>
#endif
#ifdef WIN32
#include<io.h>
#include<direct.h>
#endif
#include <sys/types.h>

extern FILE *trans;
extern char *TRNFILENAME;
extern char dotrans;
extern char *trectype[2];
extern short DATLOG_FILENO,DATTRN_FILENO;

struct RDATDatas
{
	int num;
	NRDfield *field;
};
typedef struct RDATDatas RDATData;

#define INITTRANS()	xINITTRANS(__LINE__,__FILE__)
/* INITTRANS - Initializes Transaction Processing */
void xINITTRANS(int,char *);
#define OPNLOG(x,y)	xOPNLOG(x,y,__LINE__,__FILE__)
/* OPNLOG - Call's OPNLOGFILE for set's up for recording transactions for the 
	current process */
void xOPNLOG(char *,char *,int,char *);
#define CLSLOG()	xCLSLOG(__LINE__,__FILE__)
/* CLSLOG Closes the log file associated with the current process */
void xCLSLOG(int,char *);
#define WRTNRDtrans(x,y,z)	xWRTNRDtrans(x,y,z,__LINE__,__FILE__)
/* WRTNRDtrans - Records the changes in the current database record to the 
	transaction and calls WRTNRD to write the database record to the file*/
short xWRTNRDtrans(short,short,char *,int,char *);
void cpfldinfo(NRDfield *,NRDfield *);
/* cpfldinfo copies field information from one NRDfield structure to another */
char *getkeyinfo(short,short);
/* getkeyinfo returns key field information in a format to be written in a 
	transaction processing file */

#define RDATDataNEW(a)	xRDATDataNEW(a,__LINE__,__FILE__)
RDATData *xRDATDataNEW(short,int,char *);
#define GetRDATData(a,b)	xGetRDATData(a,b,__LINE__,__FILE__)
void xGetRDATData(short,RDATData *,int,char *);
#define FreeRDATData(a)	{xFreeRDATData(a,__LINE__,__FILE__);a=NULL;}
void xFreeRDATData(RDATData *,int,char *);
#define RecordNRDfield(a,b)	xRecordNRDfield(a,b,__LINE__,__FILE__)
void xRecordNRDfield(NRDfield *,NRDfield *,int,char *);
#define ClearRDATData(a)	xClearRDATData(a,__LINE__,__FILE__)
void xClearRDATData(RDATData *,int,char *);
void WriteDATTRNLog(short,NRDfield *,NRDfield *);
#define WRTTRANS(a,b,c,d)	xWRTTRANS(a,b,c,d,__LINE__,__FILE__)
short xWRTTRANS(short,short,char *,RDATData *,int,char *);
#define RDATData2File(a,b)	xRDATData2File(a,b,__LINE__,__FILE__)
void xRDATData2File(short,RDATData *,int,char *);
int getnexttran(char *,char *,int);

#define RDATDataFIELD(a,b)	xRDATDataFIELD(a,b,__LINE__,__FILE__)
NRDfield *xRDATDataFIELD(RDATData *,char *,int,char *);
#define FINDRDATDataSETSHORT(a,b,c)	xFINDRDATDataSETSHORT(a,b,c,__LINE__,__FILE__)
short xFINDRDATDataSETSHORT(RDATData *,char *,short,int,char *);
#define FINDRDATDataSETCHAR(a,b,c)	xFINDRDATDataSETCHAR(a,b,c,__LINE__,__FILE__)
short xFINDRDATDataSETCHAR(RDATData *,char *,char,int,char *);
#define FINDRDATDataSETINT(a,b,c)	xFINDRDATDataSETINT(a,b,c,__LINE__,__FILE__)
short xFINDRDATDataSETINT(RDATData *,char *,int,int,char *);
#define FINDRDATDataSETDOUBLE(a,b,c)	xFINDRDATDataSETDOUBLE(a,b,c,__LINE__,__FILE__)
short xFINDRDATDataSETDOUBLE(RDATData *,char *,double,int,char *);
#define FINDRDATDataSETSTRING(a,b,c)	xFINDRDATDataSETSTRING(a,b,c,__LINE__,__FILE__)
short xFINDRDATDataSETSTRING(RDATData *,char *,char *,int,char *);
#define FINDRDATDataGETCHAR(a,b,c)	xFINDRDATDataGETCHAR(a,b,c,__LINE__,__FILE__)
short xFINDRDATDataGETCHAR(RDATData *,char *,char *,int,char *);
#define FINDRDATDataGETSHORT(a,b,c)	xFINDRDATDataGETSHORT(a,b,c,__LINE__,__FILE__)
short xFINDRDATDataGETSHORT(RDATData *,char *,short *,int,char *);
#define FINDRDATDataGETINT(a,b,c)	xFINDRDATDataGETINT(a,b,c,__LINE__,__FILE__)
short xFINDRDATDataGETINT(RDATData *,char *,int *,int,char *);
#define FINDRDATDataGETDOUBLE(a,b,c)	xFINDRDATDataGETDOUBLE(a,b,c,__LINE__,__FILE__)
short xFINDRDATDataGETDOUBLE(RDATData *,char *,double *,int,char *);
#define FINDRDATDataGETSTRING(a,b,c)	xFINDRDATDataGETSTRING(a,b,c,__LINE__,__FILE__)
short xFINDRDATDataGETSTRING(RDATData *,char *,char **,int,char *);

char *READFIELD(FILE *,short *);
/* READFIELD used to read a field from a transaction processing text file */

#endif 
