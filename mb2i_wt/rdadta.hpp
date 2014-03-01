/*rdadta.h - header file for rda data base engine*/
#ifndef RDADTA_H
#define RDADTA_H

#include<misc.hpp>

#ifndef WIN32
#include <fcntl.h>
#endif


#ifdef WIN32
#include<io.h>
#include<direct.h>
#endif

enum ftypes { ftchar=1,ftshort=2,ftint=3,ftlong=4,ftdouble=5};
typedef enum ftypes ftype;

typedef struct KEYs *KEY;

struct FDEFs
{
	char *name;
	ftype type;
	long len;/*defined length*/
};
typedef struct FDEFs *FDEF;

struct FLDs
{
	FDEF fdef;
	char *data;
	long dlen;/*actual length, same as defined, except when variable*/
	long max;/*max space in data field*/
};
typedef struct FLDs *FLD;

struct DTAs
{
	char *name;
	VF vf;
	long cur;/* current record number*/
	char *filename;
	short nofields;
	FDEF fdef;
	FLD fld;
	FLD oldfld;
	VF keyvf;
	short nokeys;
	KEY key;
	char *keyfilename;
};
typedef struct DTAs *DTA;

struct KEYs
{
	short keyno;
	char *keyname;
	short noparts;
	short *fieldno;
	char *asc_dec;
	long firstkey;/*record number, zero if none.*/
	short branch;
	DTA dta;
	long currec;/*current key record number*/
	short curset;/*current key set number*/
	long parent;
	short no_bch;/*not used*/
	long childless;
	long childgreat;
	long dtarecs;/*zero is not used, -1 is deleted, other is data rec number.*/
	FLD fld;
	long no_nodes;
	long level;
	char partial;
	char last_part;
	char ver;
};

union ptrs
{
	char *c;
	short *s;
	int *i;
	long *l;
	double *d;
};



/*Creation functions*/

#define CRTDTA(a)	xCRTDTA(a,__LINE__,__FILE__)
char xCRTDTA(DTA,int,char *);
/*after new and add, creates actual file, will need to open
after using to use data file. Returns TRUE on error, and does not free dta
structure, can use close, or create again.*/

#define NEWDTA(a,b,c)	xNEWDTA(a,b,c,__LINE__,__FILE__)
DTA xNEWDTA(char *,char *,char *,int,char *);
/*creates a blank DTA record to add fields and keys to. 1st arg name, 2nd file name, 3rd key file name.*/

#define NEWFLD(a,b,c,d)	xNEWFLD(a,b,c,d,__LINE__,__FILE__)
short xNEWFLD(DTA,char *,ftype,long,int,char *);
/*adds field to data file, returns field number. 2nd arg field name, 2nd field type, 3rd length.*/

#define NEWKEY(a,b,c)	xNEWKEY(a,b,c,__LINE__,__FILE__)
short xNEWKEY(DTA,char *,short,int,char *);
/*adds key to data file, returns key number. 2nd arg key name, 3rd branch factor.*/

#define NEWELE(a,b,c)	xNEWELE(a,b,c,__LINE__,__FILE__)
short xNEWELE(KEY,short,char,int,char *);
/*adds key part, returns key part number. 2nd field no, 3rd ascending(FALSE)/decending(TRUE).*/


/*Data Functions*/

#define OPNDTA(a)	xOPNDTA(a,__LINE__,__FILE__)
DTA xOPNDTA(char *,int,char *);
/*open file, reads field structure, returns NULL if problem. 1st arg is file name.*/

#define SIZDTA(a)	xSIZDTA(a,__LINE__,__FILE__)
long xSIZDTA(DTA,int,char *);
	/*returns the number of key records on file*/

#define KEYBYNUM(a,b)	xKEYBYNUM(a,b,__LINE__,__FILE__)
KEY xKEYBYNUM(DTA,short,int,char *);
/*returns key structure of number's key, returns -1 if none. 2nd arg is key number.*/

#define KEYBYNAME(a,b)	xKEYBYNAME(a,b,__LINE__,__FILE__)
KEY xKEYBYNAME(DTA,char *,int,char *);
/*returns key structure of key named, returns -1 if none. 2nd arg is key name.*/

#define FRSDTA(a)	xFRSDTA(a,__LINE__,__FILE__)
char xFRSDTA(KEY,int,char *);
/*reads first record in key order, returns TRUE if none.*/

#define LSTDTA(a)	xLSTDTA(a,__LINE__,__FILE__)
char xLSTDTA(KEY,int,char *);
/*reads last record in key order, returns TRUE if none.*/

#define NXTDTA(a)	xNXTDTA(a,__LINE__,__FILE__)
char xNXTDTA(KEY,int,char *);
/*reads next record in key order, returns TRUE if none.*/

#define PRVDTA(a)	xPRVDTA(a,__LINE__,__FILE__)
char xPRVDTA(KEY,int,char *);
/*reads preveous record in key order, returns TRUE if none.*/

#define READDTA(a,b)	xREADDTA(a,b,__LINE__,__FILE__)
char xREADDTA(DTA,long,int,char *);
/*reads record number, returns TRUE if error.*/

#define EQLDTA(a)	xEQLDTA(a,__LINE__,__FILE__)
char xEQLDTA(KEY,int,char *);
/*finds record that maches key, returns TRUE if none.*/

#define WRTDTA(a)	xWRTDTA(a,__LINE__,__FILE__)
char xWRTDTA(DTA,int,char *);
/*add's or replaces's data based on key, returns TRUE on error.*/

#define GTEDTA(a)	xGTEDTA(a,__LINE__,__FILE__)
char xGTEDTA(KEY,int,char *);
/*find greater or equal record, returns TRUE on error.*/

#define LTEDTA(a)	xLTEDTA(a,__LINE__,__FILE__)
char xLTEDTA(KEY,int,char *);
/*find lesser or equal record, returns TRUE on error.*/

#define DTAFLDZERO(a,b)	xDTAFLDZERO(a,b,__LINE__,__FILE__)
void xDTAFLDZERO(DTA,short,int,char *);
/*zero's field number*/

#define CLSDTA(a)	xCLSDTA(a,__LINE__,__FILE__)
void xCLSDTA(DTA,int,char *);
/*closes and free's memory for data files.*/

#define LOCDTA(a,b,c)	xLOCDTA(a,b,c,__LINE__,__FILE__)
char xLOCDTA(DTA,long,char,int,char *);
/*locks record number, see VFlock for rec no desc.
3rd arguement is block, if true, will wait until lock available before
returning. Returns TRUE on error, or lock unavailable.*/

#define UNLDTA(a,b)	xUNLDTA(a,b,__LINE__,__FILE__)
void xUNLDTA(DTA,long,int,char *);
/*unlocks record number, see VFlock for rec no desc.*/

#define FLDNAME(a,b)	xFLDNAME(a,b,__LINE__,__FILE__)
short xFLDNAME(DTA,char *,int,char *);
/*returns field number based on name, -1 if error. 2nd arg is field name.*/

#define DATDTA(a,b)	xDATDTA(a,b,__LINE__,__FILE__)
void *xDATDTA(DTA,short,int,char *);
/*returns pointer to field number, NULL if field number
out of bounds. 2nd arg is field number.*/

#define SETDTA(a,b,c)	xSETDTA(a,b,c,__LINE__,__FILE__)
void xSETDTA(DTA,short,void *,int,char *);
/*sets a field in a data base to pointer. 2nd arg is field number, 3rd is pointer to data to copy.*/

#define DELDTA(a,b)	xDELDTA(a,b,__LINE__,__FILE__)
void xDELDTA(DTA,long,int,char *);/*deletes the specified record number and keys.*/


/*Maintenance Functions*/

#define RKYDTA(a)	xRKYDTA(a,__LINE__,__FILE__)
char xRKYDTA(KEY,int,char *);
/*rebuilds key number, returns true or false? 2nd arg is key file number.*/

#define RBDDTA(a)	xRBDDTA(a,__LINE__,__FILE__)
char xRBDDTA(DTA,int,char *);
/*rebuilds all key files and packs data records, removes deleted.*/

#define RESDTA(a)	xRESDTA(a,__LINE__,__FILE__)
char xRESDTA(char *,int,char *);
/*uses banks in data field to recreate data file and keys.*/

#define RCKDTA(a)	xRCKDTA(a,__LINE__,__FILE__)
void xRCKDTA(DTA,int,char *);
/*checks to see if any keys need to be rebuild.*/

#define BEGDTA(a)	xBEGDTA(a,__LINE__,__FILE__)
char xBEGDTA(DTA,int,char *);
/*finds first record in physical order*/

#define SEQDTA(a)	xSEQDTA(a,__LINE__,__FILE__)
char xSEQDTA(DTA,int,char *);
/*finds next record in physical order*/


/* todo future: endless loops in key searches*/
/* todo future: array of string's?*/
/* todo future: error checking on dta, fld, etc.*/
#define DUMPdta(a)	xDUMPdta(a,__LINE__,__FILE__)
void xDUMPdta(DTA,int,char *);

extern char diagrdakey;
#endif
