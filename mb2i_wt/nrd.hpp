
/******************************************************************************
			NEW RDA DATABASE HEADER 
				nrd.h
	purpose: Contains macros and function prototypes for the rda data base
		 manager interface.
		 These functions and associated macros are defined public for 
		 application use.					
*******************************************************************************/

#ifndef NRD_H
#define NRD_H


#ifndef WIN32
#include <values.h>
#endif

#include<misc.hpp>


extern char UseRamDisk;
extern char NONTRADITIONAL_EXECUTION;
extern char *supported_engines[11];
#ifdef USE_CTREE
extern char USE_ODBC;
#endif /* USE_CTREE */
/*Standard macro's*/
#define NRDRdaEngine	0
#define NRDInformixIsamEngine 1
#define NRDMyIsamEngine 2
#define NRDMySQLXpertEngine	3
#define NRDMySQLEngine 4
#define NRDCtreeEngine	5
#define NRDLocalCtreeEngine 6
#define NRDBerkeleyBtree	7
#define NRDBerkeleyBtreeRPC	8
#define NRDBerkeleyBtreeMySQL	9
#define NRDMySQLAutoEngine	10

extern char *fieldtypedesc[];

/* Ctree Local Database Initialization variables */
extern int DBUFFctree;
extern int IBUFFctree;
extern int MAXFILESctree;
extern int NODESECTSctree;

#ifdef USE_MYSQL
extern char *MySQL_Username,*MySQL_Password,*MySQL_Database;
extern char RDA_CollateType;
#else
#ifdef USE_MYISAM
extern char RDA_CollateType;
#endif
#endif


/*structure for fields*/
struct NRDfields
{
	char *name;/*name of field*/
	short type;/*type of field*/
	unsigned len;/*max length of field, -1 if variable*/
	union rda_field_types data;
	unsigned dlen;
#ifdef INFORMIX_ISAM
	short offset;
#endif
};
typedef struct NRDfields NRDfield;
/*structure for parts */
struct NRDparts
{
	short offset; /* offset of field in data file */
	char *name; /* name of element used as key */
};
typedef struct NRDparts NRDpart;

/*structure for keys*/
struct NRDkeys
{
	char *name; /* name of key */
	short numparts; /* number of elements in the key */
	NRDpart *part;/* element or part structure */
	short length;
	short fileno;
#ifdef INFORMIX_ISAM
	struct keydesc KeyDesc;
#endif
#ifdef USE_BERKELEY_BTREE
	DB *dbp;
#endif
};
typedef struct NRDkeys NRDkey;

struct DBsorts
{
	short fileno;
	char *module;
	char *filename;
	char *servername;
	short engine;
	short nofields;
	NRDfield *fields;
	short nokeys;
	NRDkey *keys;
};
typedef struct DBsorts DBsort;

#define VARRECORDS(x)	xVARRECORDS(x,__LINE__,__FILE__)
short xVARRECORDS(short,int,char *);
short TESTFILENO(short,int,char *);
#define MODULENAME(x)	xMODULENAME(x,__LINE__,__FILE__)
char *xMODULENAME(short,int,char *);
#define FILENAME(x)	xFILENAME(x,__LINE__,__FILE__)
#define KEYNAME(x,y)	xKEYNAME(x,y,__LINE__,__FILE__)
char *xFILENAME(short,int,char *);
char *xKEYNAME(short,short,int,char *);
short FILENUMBER(char *,char *);
#define KEYNUMBER(x,y)	xKEYNUMBER(x,y,__LINE__,__FILE__)
short xKEYNUMBER(short,char *,int,char *);
#define KEYNUMBERBYNAME(x,y,z)	xKEYNUMBERBYNAME(x,y,z,__LINE__,__FILE__)
short xKEYNUMBERBYNAME(char *,char *,char *,int,char *);
#define FINDFLDSETVALUE(a,b,c)	xFINDFLDSETVALUE(a,b,c,__LINE__,__FILE__)
short xFINDFLDSETVALUE(short,char *,void *,int,char *);
#define FINDFLDGETVALUE(a,b,c)	xFINDFLDGETVALUE(a,b,c,__LINE__,__FILE__)
short xFINDFLDGETVALUE(short,char *,void *,int,char *);
#define FINDFLDGETSTRING(a,b,c)	xFINDFLDGETSTRING(a,b,c,__LINE__,__FILE__)
short xFINDFLDGETSTRING(short,char *,char **,int,char *);
#define GETNRDKEY(x,y,z)	xGETNRDKEY(x,y,z,__LINE__,__FILE__)
void *xGETNRDKEY(short,short,short *,int,char *);
#define DUMPfile(x)	xDUMPfile(x,__LINE__,__FILE__)
void xDUMPfile(short,int,char *);
#define SEENRDFIELD(x)	xSEENRDFIELD(x,__LINE__,__FILE__)
void xSEENRDFIELD(NRDfield *,int,char *);
#define SEENRDRECORD(x)	xSEENRDRECORD(x,__LINE__,__FILE__)
void xSEENRDRECORD(short,int,char *);
#define FIELDCOPY(w,x,y,z)	xFIELDCOPY(w,x,y,z,__LINE__,__FILE__)
short xFIELDCOPY(short,char *,short,char *,int,char *);
#define COPYFIELD(x,y,z)	xCOPYFIELD(x,y,z,__LINE__,__FILE__)
short xCOPYFIELD(short,short,char *,int,char *);
#define CRTNRD(a,b,c,d,e,f,g,h)	xCRTNRD(a,b,c,d,e,f,g,h,__LINE__,__FILE__)
short xCRTNRD(char *,short,char *,short,NRDfield *,short,NRDkey *,char *,int,char *);
	/* Creates new data base file, remove old copy if exists, field & key
		information must exist prior to calling.
		1st arg module name
		2nd arg data base engine, 0=rda.
		3rd arg name,
		4th arg number of fields.
		5th arg pointer to field structures.
		6th arg number of keys.
		7th arg pointer to key structures.
		8th arg pointer to string for server name.
		returns file reference number, if less 0, error*/
#define OPNNRD(x,y)	xOPNNRD(x,y,__LINE__,__FILE__)
short xOPNNRD(char *,char *,int,char *);
	/* Open data base file, loads field & key information.
		1st arg name of module 
		2nd arg name of file
		returns file reference number, if less 0, error*/
#define OPNEXCLCKNRD(x,y)	xOPNEXCLCKNRD(x,y,__LINE__,__FILE__)
short xOPNEXCLCKNRD(char *,char *,int,char *);
	/* Open data base file, loads field & key information.
		1st arg name of module 
		2nd arg name of file
		returns file reference number, if less 0, error*/
#define CLSNRD(x)	{xCLSNRD(x,__LINE__,__FILE__);x=(-1);}
short xCLSNRD(short,int,char *);
	/* Close data base file.
		1st arg file reference number*/
#define EQLNRDKEY(a,b,c)	xEQLNRDKEY(a,b,c,__LINE__,__FILE__)
short xEQLNRDKEY(short,short,void *,int,char *);
#define EQLNRD(a,b)	xEQLNRD(a,b,__LINE__,__FILE__)
short xEQLNRD(short,short,int,char *);
	/* Find record equal to loaded key.
		1st arg file reference number.
		2nd arg key file number.
		returns 0 if found and loaded, 1 if not found, <0 if error.*/
#define BEGNRD(a)	xBEGNRD(a,__LINE__,__FILE__)
short xBEGNRD(short,int,char *);
#define SEQNRD(a)	xSEQNRD(a,__LINE__,__FILE__)
short xSEQNRD(short,int,char *);
#define PRVNRD(x,y)	xPRVNRD(x,y,__LINE__,__FILE__)
short xPRVNRD(short,short,int,char *);
#define LSTNRD(x,y)	xLSTNRD(x,y,__LINE__,__FILE__)
short xLSTNRD(short,short,int,char *);
#define GTENRD(x,y)	xGTENRD(x,y,__LINE__,__FILE__)
short xGTENRD(short,short,int,char *);
	/* Find record equal to or greater than loaded key.
		1st arg file reference number.
		2nd arg key file number.
		returns 0 if found and loaded, 1 if not found, <0 if error.*/
#define LTENRD(x,y)	xLTENRD(x,y,__LINE__,__FILE__)
short xLTENRD(short,short,int,char *);
	/* Find record equal to or less than loaded key.
		1st arg file reference number.
		2nd arg key file number.
		returns 0 if found and loaded, 1 if not found, <0 if error.*/
#define FRSNRD(x,y)	xFRSNRD(x,y,__LINE__,__FILE__)
short xFRSNRD(short,short,int,char *);
	/* Find first record in sorted order.
		1st arg file reference number.
		2nd arg key file number.
		returns 0 if found and loaded, 1 if not found, <0 if error.*/
#define NXTNRD(x,y)	xNXTNRD(x,y,__LINE__,__FILE__)
short xNXTNRD(short,short,int,char *);
	/* Find next record in sorted order.
		1st arg file reference number.
		2nd arg key file number.
		returns 0 if found and loaded, 1 if not found, <0 if error.*/
#define FORNRD(x,y)	xFORNRD(x,y,__LINE__,__FILE__)
short xFORNRD(short,short (*)(short));
	/* Loops through all data in physical order calling function for
		each record.
		1st arg file reference number.
		2nd arg function to call for each record in file, should return 1 if
			to continue looping, 0 to abort. Arg to function is 1 for 1st
			record, 2 for other records, & 0 for clean-up at end.
		returns 1 if completed, 0 if aborted, <0 if error*/
#define SETNRD(a,b,c,d)	xSETNRD(a,b,c,d,__LINE__,__FILE__)
short xSETNRD(short,short,short,short (*)(short),int,char *);
	/* Loops through data in sorted order, selecting only range of records
		specified by key level, calling function for each record.
		1st arg file reference number.
		2nd arg key file number.
		3rd arg key level number, if 0 don't compare any fields, 1 select only
			fields that have 1st key the same, etc.
		4th arg function to call for each record in file, should return 1 if
			to continue looping, 0 to abort. Arg to function is 1 for 1st
			record, 2 for other records, & 0 for clean-up at end.
		returns 1 if completed, 0 if aborted, <0 if error*/
#define SIZNRD(x)	xSIZNRD(x,__LINE__,__FILE__)
long xSIZNRD(short,int,char *);
	/* Calculates size of data base file.
		1st arg file reference number.
		returns number of valid records, or <0 if error*/
#define WRTNRD(a,b,c)	xWRTNRD(a,b,c,__LINE__,__FILE__)
short xWRTNRD(short,short,char *,int,char *);
	/* Writes current record out to data file.
		1st arg file reference number.
		2nd arg is 1 if in add mode (for activity), 0 for overwrite.
		returns 0 if completed, <0 if error.*/
#define RESNRD(x)	xRESNRD(x,__LINE__,__FILE__)
short xRESNRD(short,int,char *);
#define RBDNRD(x)	xRBDNRD(x,__LINE__,__FILE__)
short xRBDNRD(short,int,char *);
	/* General purpose utility. i.e. removes deleted records, packs file,
		remakes (or verify's) keys.
		1st arg file reference number.
		returns 0 if completed, <0 if error.*/
#define KEYNRD(x,y)	xKEYNRD(x,y,__LINE__,__FILE__)
void xKEYNRD(short,short,int,char *);
	/* Zero's out all of record except key parts.
		1st arg file reference number.
		2nd arg key file number.*/
#define ZERNRD(x)	xZERNRD(x,__LINE__,__FILE__)
void xZERNRD(short,int,char *);
	/* Zero's out all of record except key parts.
		1st arg file reference number.*/
#define KEYNUM(x,y)	xKEYNUM(x,y,__LINE__,__FILE__)
NRDkey *xKEYNUM(short,short,int,char *);
#define FLDNUM(x,y)	xFLDNUM(x,y,__LINE__,__FILE__)
NRDfield *xFLDNUM(short,short,int,char *);
#define FLDNRD(x,y)	xFLDNRD(x,y,__LINE__,__FILE__)
NRDfield *xFLDNRD(short,char *,int,char *);
	/* Retreive field address of current record & file.
		1st arg file reference number.
		2nd arg is the fieldname searching for.
		returns pointer to field, NULL if field not found.*/
#define GETBIN(a,b,c,d,e,f,g)	xGETBIN(a,b,c,d,e,f,g,0,NULL,__LINE__,__FILE__)
#define ADVGETBIN(a,b,c,d,e,f,g,h)	xGETBIN(a,b,c,d,e,f,g,1,h,__LINE__,__FILE__)
short xGETBIN(char *,char *,short *,short *,NRDfield **,short *,NRDkey **,short,char **,int,char *);
#define NUMFLDS(x)	xNUMFLDS(x,__LINE__,__FILE__)
short xNUMFLDS(short,int,char *);
	/* Returns the number of fields in a file */
#define INITNRD()	xINITNRD(__LINE__,__FILE__)
short xINITNRD(int,char *);
	/* This function is used to initialize the database */
#define MAKEPASSABLEKEYS(x,y,z)	xMAKEPASSABLEKEYS(x,y,z,__LINE__,__FILE__)
#define MAKEPASSABLEKEYSAPPlib(x,y,z)	xMAKEPASSABLEKEYSAPPlib(x,y,z,__LINE__,__FILE__)
char *xMAKEPASSABLEKEYS(short,short,void *,int,char *);
APPlib *xMAKEPASSABLEKEYSAPPlib(short,short,void *,int,char *);
#define CONVERTPASSABLEKEYS(x,y,z,a)	xCONVERTPASSABLEKEYS(x,y,z,a,__LINE__,__FILE__)
short xCONVERTPASSABLEKEYS(short,short,short,char **,int,char *);
#define SETFIELDWITHARRAYKEYS(a,b,c,d)	xSETFIELDWITHARRAYKEYS(a,b,c,d,__LINE__,__FILE__)
short xSETFIELDWITHARRAYKEYS(short,short,short,char **,int,char *);
#define FreeNRDfield(a)	xFreeNRDfield(a,__LINE__,__FILE__)
void xFreeNRDfield(NRDfield *,int,char *);
#define NRD2NRD(a,b)	xNRD2NRD(a,b,__LINE__,__FILE__)
short xNRD2NRD(NRDfield *,NRDfield *,int,char *);
#define COMPARENRDfield(a,b)	xCOMPARENRDfield(a,b,__LINE__,__FILE__)
short xCOMPARENRDfield(NRDfield *,NRDfield *,int,char *);
#define DELETENRD(a,b,c,d)	xDELETENRD(a,b,c,d,__LINE__,__FILE__)
short xDELETENRD(char *,char *,short,char *,int,char *);
#define DELNRD(x)	xDELNRD(x,__LINE__,__FILE__)
short xDELNRD(short,int,char *);
#define LOCNRDFILE(x)	xLOCNRDFILE(x,__LINE__,__FILE__)
short xLOCNRDFILE(short,int,char *);
#define UNLNRDFILE(x)	xUNLNRDFILE(x,__LINE__,__FILE__)
short xUNLNRDFILE(short,int,char *);
#define LOCNRD(x)	xLOCNRD(x,__LINE__,__FILE__)
short xLOCNRD(short,int,char *);
#define UNLNRD(x)	xUNLNRD(x,__LINE__,__FILE__)
short xUNLNRD(short,int,char *);
#define RCKNRD(x)	xRCKNRD(x,__LINE__,__FILE__)
void xRCKNRD(short,int,char *);
#define DBsortNEW(a,b,c)	xDBsortNEW(a,b,c,__LINE__,__FILE__)
DBsort *xDBsortNEW(char *,char *,short,int,char *);
#define addDBfield(a,b,c,d)	xaddDBfield(a,b,c,d,__LINE__,__FILE__)
short xaddDBfield(DBsort *,char *,short,unsigned,int,char *);
#define addDBfilefield(a,b,c)	xaddDBfilefield(a,b,c,__LINE__,__FILE__)
short xaddDBfilefield(DBsort *,short,char *,int,char *);
#define addDBkeypart(a,b,c)	xaddDBkeypart(a,b,c,__LINE__,__FILE__)
short xaddDBkeypart(DBsort *,short,char *,int,char *);
#define addDBfieldNoDuplicates(a,b,c,d)	xaddDBfieldNoDuplicates(a,b,c,d,__LINE__,__FILE__)
short xaddDBfieldNoDuplicates(DBsort *,char *,short,unsigned,int,char *);
#define addDBkeypartNoDuplicates(a,b,c)	xaddDBkeypartNoDuplicates(a,b,c,__LINE__,__FILE__)
short xaddDBkeypartNoDuplicates(DBsort *,short,char *,int,char *);
#define addDBkey(a,b)	xaddDBkey(a,b,__LINE__,__FILE__)
short xaddDBkey(DBsort *,char *,int,char *);
#define ADVFreeDBsort(a,b)	{xFreeDBsort(a,b,__LINE__,__FILE__);a=NULL;}
#define FreeDBsort(a)	{xFreeDBsort(a,TRUE,__LINE__,__FILE__);a=NULL;}
void xFreeDBsort(DBsort *,short,int,char *);
#define OPNDBsort(a)	xOPNDBsort(a,__LINE__,__FILE__)
short xOPNDBsort(DBsort *,int,char *);
#define CLSDBsort(a)	xCLSDBsort(a,__LINE__,__FILE__)
void xCLSDBsort(DBsort *,int,char *);
#define FLDPOINTER(a)	xFLDPOINTER(a,__LINE__,__FILE__)
NRDfield *xFLDPOINTER(short,int,char *);
#define KEYPOINTER(a)	xKEYPOINTER(a,__LINE__,__FILE__)
NRDkey *xKEYPOINTER(short,int,char *);
#define NUMKEYS(a)	xNUMKEYS(a,__LINE__,__FILE__)
short xNUMKEYS(short,int,char *);
#define SetTrnModeField(a,b)	xSetTrnModeField(a,b,__LINE__,__FILE__)
void xSetTrnModeField(short,char *,int,char *);
#define FINDFLDSETSHORT(a,b,c)	xFINDFLDSETSHORT(a,b,c,__LINE__,__FILE__)
short xFINDFLDSETSHORT(short,char *,short,int,char *);
#define FINDFLDSETCHAR(a,b,c)	xFINDFLDSETCHAR(a,b,c,__LINE__,__FILE__)
short xFINDFLDSETCHAR(short,char *,char,int,char *);
#define FINDFLDSETINT(a,b,c)	xFINDFLDSETINT(a,b,c,__LINE__,__FILE__)
short xFINDFLDSETINT(short,char *,int,int,char *);
#define FINDFLDSETDOUBLE(a,b,c)	xFINDFLDSETDOUBLE(a,b,c,__LINE__,__FILE__)
short xFINDFLDSETDOUBLE(short,char *,double,int,char *);
#define FINDFLDSETSTRING(a,b,c)	xFINDFLDSETSTRING(a,b,c,__LINE__,__FILE__)
short xFINDFLDSETSTRING(short,char *,char *,int,char *);
#define FINDFLDGETCHAR(a,b,c)	xFINDFLDGETCHAR(a,b,c,__LINE__,__FILE__)
short xFINDFLDGETCHAR(short,char *,char *,int,char *);
#define FINDFLDGETSHORT(a,b,c)	xFINDFLDGETSHORT(a,b,c,__LINE__,__FILE__)
short xFINDFLDGETSHORT(short,char *,short *,int,char *);
#define FINDFLDGETINT(a,b,c)	xFINDFLDGETINT(a,b,c,__LINE__,__FILE__)
short xFINDFLDGETINT(short,char *,int *,int,char *);
#define FINDFLDGETDOUBLE(a,b,c)	xFINDFLDGETDOUBLE(a,b,c,__LINE__,__FILE__)
short xFINDFLDGETDOUBLE(short,char *,double *,int,char *);
#define FreeNRDfieldvalue(a)	xFreeNRDfieldvalue(a,__LINE__,__FILE__)
void xFreeNRDfieldvalue(NRDfield *,int,char *);
#define ZeroNRDfield(a)	xZeroNRDfield(a,__LINE__,__FILE__)
void xZeroNRDfield(NRDfield *,int,char *);
#define EXITNRD()	xEXITNRD(__LINE__,__FILE__)
void xEXITNRD(int,char *);
NRDfield *DBsortFIELD(DBsort *,char *);
short IsKeyPart(NRDkey *,char *);

#ifdef INFORMIX_ISAM
#ifdef LINUX2_2
#ifdef LINUX_SUSE_6_1
void __bzero(void *,int);
#endif
#endif
#endif
struct RDATNRDDatas
{
	int num;
	NRDfield *field;
};
typedef struct RDATNRDDatas RDATNRDData;
#define RDATNRDDataNEW(a)	xRDATNRDDataNEW(a,__LINE__,__FILE__)
RDATNRDData *xRDATNRDDataNEW(short,int,char *);
#define GetRDATNRDData(a,b)	xGetRDATNRDData(a,b,__LINE__,__FILE__)
void xGetRDATNRDData(short,RDATNRDData *,int,char *);
#define FreeRDATNRDData(a)	{xFreeRDATNRDData(a,__LINE__,__FILE__);a=NULL;}
void xFreeRDATNRDData(RDATNRDData *,int,char *);
#define RecordNRDfield(a,b)	xRecordNRDfield(a,b,__LINE__,__FILE__)
void xRecordNRDfield(NRDfield *,NRDfield *,int,char *);
#define ClearRDATNRDData(a)	xClearRDATNRDData(a,__LINE__,__FILE__)
void xClearRDATNRDData(RDATNRDData *,int,char *);
#define DATABASE_ENGINE(a)	xDATABASE_ENGINE(a,__LINE__,__FILE__)
short xDATABASE_ENGINE(short,int,char *);

#endif /*NRD_H*/

