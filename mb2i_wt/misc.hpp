// misc.h - header for the Misc Library 
#ifndef MISC_H
#define MISC_H
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cfloat>
#include <cstdarg>
#include <cstddef>
#include <csignal>
#include <cstring>
#include <ctime>
#include <cmath>
#include <cerrno>
#include <limits.h>
#ifdef USING_BOOST_FILESYSTEM
#include <boost/filesystem.hpp>
#endif

extern char *(*UI_GetAppRoot)(void);
extern char *(*UI_GetDocRoot)(void);
extern char *(*UI_RDA_GetEnv)(char *,int,char *);
extern int (*UI_RDA_SetEnv)(char *,char *,int,char *);
extern int (*UI_RDA_UnSetEnv)(char *,int,char *);
extern int (*UI_RDA_PutEnv)(char *,int,char *);
extern void (*UI_OpenWindow)(char *);
extern void (*UI_OpenWindowWait)(char *);
extern char *(*BCryptValue)(char *);

#define RDA_SetEnv(a,b)	xRDA_SetEnv(a,b,__LINE__,__FILE__)
int xRDA_SetEnv(char *, char *,int,char *);
#define RDA_UnSetEnv(a)	xRDA_UnSetEnv(a,__LINE__,__FILE__)
int xRDA_UnSetEnv(char *,int,char *);
#define RDA_PutEnv(a)	xRDA_PutEnv(a,__LINE__,__FILE__)
int xRDA_PutEnv(char *,int,char *);
#define RDA_GetEnv(a)	xRDA_GetEnv(a,__LINE__,__FILE__)
char *xRDA_GetEnv(char *,int,char *);

#ifndef WIN32
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#endif

#ifndef WIN32
#include <sys/types.h>
#include <sys/stat.h>
#endif 

#ifdef WIN32
#include<io.h>
#include<windows.h>
#include <direct.h>
#include<windef.h>
#include<winbase.h>
#include <sys/locking.h>
#include<process.h>
#endif

using namespace std;

extern char stemp[];
extern char RDA_CURRENCY_SYMBOL;
extern short RDA_NOGUI;

#ifndef TRUE
#define TRUE	1
#endif
#define true	TRUE
#ifndef FALSE
#define FALSE	0
#endif
#define false	FALSE
#define NE(a)	(sizeof(a)/sizeof(a[0]))

#define ISNULL(str) (str!=NULL?str:"NULL")
#ifndef WIN32
#define DUMPSTR(x)  fprintf(RDA_STDERR,"%s:(%d)[%s];",#x,(int)x,ISNULL(x))
#define DUMPADDR(x) fprintf(RDA_STDERR,"%s:%d;",#x,(int)x)
#define DUMPINT(x)  fprintf(RDA_STDERR,"%s:%d;",#x,(int)x)
#define DUMPLONG(x) fprintf(RDA_STDERR,"%s:%d;",#x,(long)x)
#define DUMPDBL(x) fprintf(RDA_STDERR,"%s:%f;",#x,(double)x)
#define DUMPBOL(x)  fprintf(RDA_STDERR,"%s:(%d)%s;",#x,(long)x,(x?"TRUE":"FALSE"))
#define DUMPNL()	{fprintf(RDA_STDERR,"\n");fflush(RDA_STDERR);}
#endif
#ifdef WIN32
#define DUMPSTR(x)  fprintf(RDA_STDERR,"%s:(%d)[%s];",#x,(int)x,ISNULL(x))
#define DUMPADDR(x) fprintf(RDA_STDERR,"%s:%d;",#x,(int)x)
#define DUMPINT(x)  fprintf(RDA_STDERR,"%s:%d;",#x,(int)x)
#define DUMPLONG(x) fprintf(RDA_STDERR,"%s:%d;",#x,(long)x)
#define DUMPDBL(x) fprintf(RDA_STDERR,"%s:%f;",#x,(double)x)
#define DUMPBOL(x)  fprintf(RDA_STDERR,"%s:(%d)%s;",#x,(long)x,(x?"TRUE":"FALSE"))
#define DUMPNL()	{fprintf(RDA_STDERR,"\n");fflush(RDA_STDERR);}
#endif

#include <b64encode.hpp>
#include <b64decode.hpp>

extern char *CALENDAR_MONTHS[];
extern char *bolstring[2];

extern char AlreadyInitialized;
extern FILE * RDA_STDERR;
void initrdadiag(void);

extern char diagmisc;
extern char diagalloc;
extern char diagallocx;
extern char diaggui;
extern char diagnrd;
extern char diagcss;
extern char diagmix;
extern char diagbin;
extern char diagmyisam;
extern char diagmysql;
extern char diagproc;
extern char diagblb;
extern char diagolh;
extern char diagsec;
extern char diagtrans;
extern char diagrptgen;
extern char diageval;
extern char diagrf;
extern char diagvf;
extern char diagrdadta;
extern char diagbadalloc;
extern char diagimport;
extern char diagvirtual;
extern char diagexeprog;
extern char diagrptgen_slteval;
extern char diagrptgen_select;
extern char diagrptgen_sltall;
extern char diagrptgen_sltrec;
extern char diagrptgen_outall;
extern char diagrptgen_outexp;
extern char diagrptgen_sltrec;
extern char diagrptgen_outall;
extern char diagrptgen_outexp;
extern char diagrptgen_output;
extern char diagrptgen_outline;
extern char diagrptgen_field;
extern char diaggui_field;
extern char diagnrd_field;
extern char diageval_field;
extern char diagsec_field;
extern char diagvirtual_field;
extern char diagimport_field;
extern char diagolh_field;
extern char diagmix_field;
extern char diagywin;
extern char diagapps;
extern char diagtable;
extern char diagmatrix;
extern char diaglist;
extern char diaginformix_isam;
extern char diaginformix_standard;
extern char diagstandard_sql;
extern char diagibm_db2;
extern char diagadd;
extern char diagprt;
extern char diaglic;

extern char *USERLOGIN;
extern char *VERSION_DATE;
extern char *PROGRAM_NAME;
extern int RDA_SOFTWARE_TYPE;

extern char *DOCUMENTROOT;
extern char *CURRENTDIRECTORY;
extern char *CURRENT_DATE;
extern char *CURRENT_DATE10;
extern char *CURRENT_TIME;

void SETCWD(void);
int RCHGCWD(char *);
void FREECWD(void);
char *GETCURRENTDATE(void);
char *GETCURRENTDATE10(void);
char *GETCURRENTTIME(void);

#define RDA_ALL_APP		(-1)
#define RDA_XPERT_APP		0
#define RDA_XPERT_LITE_APP	1
#define RDA_MBGUI_APP 		2
#define RDA_XPERT_AND_LITE_APP	3

/*FIELD TYPES*/
#define VARIABLETEXT	0
#define PLAINTEXT		1
#define DOLLARS			2
#define PHONE			3
#define CUSTOMTYPE		4
#define ZIPCODE			5
#define DECIMALV		6
#define SHORTV			7
#define LONGV			8
#define DOUBLEV			9
#define BOOLNS			10
#define CHARACTERS		11
#define BUTTONS			12
#define SCROLLEDLIST		13
#define SCROLLEDTEXT		14
#define SOCSECNUM		15
#define DATES			16
#define TIMES			17
#define SSHORTV			18
#define SLONGV			19
#define SDECIMALV		20
#define SDOUBLEV		21
#define EXPENDITURE		22
#define REVENUE 		23
#define BALANCESHEET		24
#define BEGINNINGBALANCE	25
#define OPTIONALFIELDS		26
#define DOLLARS_NOCENTS		27
#define PROGRESS_BAR		28

union rda_field_types
{
	short *short_value;
	int *integer_value;
	double *float_value;
	char *string_value;
};

#define EXP_ACCT 0 
#define REV_ACCT 1
#define BAL_ACCT 2
#define BEG_ACCT 3
#define CUSTOM_ACCT 4
int RGETPID(void);
int RGETPPID(void);
void SETUSERLOGIN(void);

extern char errspace[];

#define prterr	prtflderr(__FILE__,__LINE__,__DATE__);prtopterr
	/* use above macro as prterr(x) where x is some printf style string*/

void prtflderr(char *,int,char *);
	/* adds location data to static data space for future output.
		1st arg file name.
		2nd arg line number.
		3rd arg data.*/
void prtopterr(char *,...);
	/* prints printf style error message to RDA_STDERR.
		1st arg format string.
		optional rest of args based on format.*/
char *errname(void);


#ifndef WIN32
#define TRACE	prterr("TRACE");
#else
#define TRACE	xWIN32TRACE(__LINE__,__FILE__);
#endif

#define Rmalloc(x)	xRmalloc(x,__LINE__,__FILE__)
/* Rmalloc has one argument, the length of memory in bytes to allocate. 
	This argument is an unsigned. */
#define Rrealloc(x,y)	xRrealloc(x,y,__LINE__,__FILE__)
/* Rrealloc has two arguments, the pointer to the memory to reallocate,
	followed by the new length of memory requested.  The pointer is 
	a void * , the length is an unsigned */
#define Rfree(x)	{xRfree(x,__LINE__,__FILE__);x=NULL;}
/* Rfree has one argument, the pointer to the memory to free.  This argument
	is a void * */
#define Rcalloc(x,y)	xRcalloc(x,y,__LINE__,__FILE__)
/* Rcalloc has two arguments, the number and length to an array of pointers,
	both arguments are unsigned */
#define CHECKALLOC()	checkallocdiag(__LINE__,__FILE__)
/* CHECKALLOC has no arguments, and tests memory allocated for damage and 
	overruns. This is used to test for memory corruption */

#define stralloc(a)	xstralloc(a,__LINE__,__FILE__)
char *xstralloc(char *,int,char *);
#define stralloccat(a,b)	xstralloccat(a,b,__LINE__,__FILE__)
	/* mallocs space for passed string and copies it.
		1st arg string to alloc.
		returns malloced string, or NULL if error or passed NULL.*/
char *xstralloccat(char *,char *,int,char *);
short checkallocdiag(int,char *);
void *diagRrealloc(void *,unsigned,int,char *);
void *diagRmalloc(unsigned,int,char *);
void diagRfree(void *,int,char *);
void *xRmalloc(unsigned,int,char *);
void *xRrealloc(void *,unsigned,int,char *);
void xRfree(void *,int,char *);
void *xRcalloc(unsigned,unsigned,int,char *);
#define QUICKALLOC(RDA_buf,max,new)	{if(max<new) {max=new;if(RDA_buf!=NULL) RDA_buf=Rrealloc(RDA_buf,max); else RDA_buf=Rmalloc(max);}}

struct applibs
{
	int numlibs;
	char **libs;
};
typedef struct applibs APPlib;

#define freeapplib(x)		{xfreeapplib(x,__LINE__,__FILE__);x=NULL;}
/* freeapplib has one argument, the pointer to a APPlib structure to free */
#define addAPPlib(x,y)		xaddAPPlib(x,y,__LINE__,__FILE__)
/* addAPPlib has two arguments, the first a pointer to an APPlib structure,
	the second a character string to add as an element of the APPlib 
	structure */
#define addAPPlibNoDuplicates(x,y)	xaddAPPlibNoDuplicates(x,y,__LINE__,__FILE__)
#define APPlibNEW()		xAPPlibNEW(__LINE__,__FILE__)
/* APPlibNEW has no arguments, and creates an initial APPlib structure.  The 
	APPlib structure is a group of char pointers (strings) */
#define SORTAPPlib(x)	xSORTAPPlib(x,__LINE__,__FILE__)

void xfreeapplib(APPlib *,int,char *);
int xaddAPPlib(APPlib *,char *,int,char *);
int xaddAPPlibNoDuplicates(APPlib *,char *,int,char *);
APPlib *xAPPlibNEW(int,char *);
void xSORTAPPlib(APPlib *,int,char *);
#define SEEAPPlib(a)	xSEEAPPlib(a,__LINE__,__FILE__)
void xSEEAPPlib(APPlib *,int,char *);
#define FINDAPPLIBELEMENT(a,b)	xFINDAPPLIBELEMENT(a,b,__LINE__,__FILE__)
int xFINDAPPLIBELEMENT(APPlib *,char *,int,char *);
#define CopyAPPlib(a)	xCopyAPPlib(a,__LINE__,__FILE__)
APPlib *xCopyAPPlib(APPlib *,int,char *);

char *floatamt(double,int);	/*Regular Float Format function w/neg*/
char *famt(double,int);		/*Regular Dollar Format function*/
char *GUIfamt(double,int);		/*Regular Dollar Format function*/
char *GUIfamtncents(double,int,char);	/*Dollar No Cents Format function*/
char *GUIfloatamt(double,int);	/*Regular Float Format function w/neg*/
char *GUIsintamt(long,int);	/*Integer with negitive field*/
char *famtncents(double,int,char);	/*Dollar No Cents Format function*/
char *famtncomma(double,int,char);	/*Dollar No Comma Format function*/
char *famtncnc(double,int,char);	/*Dollar No Comma No Cents Format func*/
char *famtnbk(double,int,char);	/*Dollar Brackets For Neg Format func*/
char *famtnbknc(double,int,char);	/*Dollar Brackets For Neg & No Cents*/
char *famtnbkncomma(double,int,char);/*Dollar Brackets For Neg & No Comma*/
char *famtnbkncnc(double,int,char);	/*Dollar Brackets For Neg,No Cents, & No Comma*/
char *famtpad(double,int,char);	/*Dollar with pad chars*/
char *ufloatamt(double,int);
char *sintamt(long,int);	/*Integer with negitive field*/
char *famt_inttoencrypted1(double,int,char); /* */
double stripfamt(char *);
double GUIstripfamt(char *);
double convneg(char *);
int convintneg(char *);
short convshortneg(char *);
void stoupper(char *);
void stolower(char *);
#define unpad(a)	xunpad(a,__LINE__,__FILE__)
void xunpad(char *,int,char *);
void pad(char *,int);
char *stripdashes(char *);
char *adddashes(char *);
short system_command(char *);
short pattern_match(char *,char *);
short findfile(char *,char *,char **,int);
short vals(char *,int);
double valf(char *,int);
long vall(char *,int);
int val(char *,int);
double Rint(double);
double round(double);
double RDAroundup(double);
char *spell(double);
#define LinesInTextFile(a,b)	xLinesInTextFile(a,b,__LINE__,__FILE__)
int xLinesInTextFile(FILE *,int,int,char *);
#define CountLines(a)	xCountLines(a,__LINE__,__FILE__)
int xCountLines(char *filename,int,char *);
#define readline(a,b,c)	xreadline(a,b,c,__LINE__,__FILE__)
int xreadline(FILE *,char *,int,int,char *);
#define isEMPTY(x)	xisEMPTY(x,__LINE__,__FILE__)
/* Checks for a string pointer null, then if its length is zero*/
int xisEMPTY(char *,int,char *);
short patmatch(char *,char *);
#define RDAstrlen(a)	xRDAstrlen(a,__LINE__,__FILE__)
int xRDAstrlen(char *,int,char *);
#define RDAstrcmp(a,b)	xRDAstrcmp(a,b,__LINE__,__FILE__)
int xRDAstrcmp(char *,char *,int,char *);
char *xRDAstrstr(char *,char *,int,char *);
#define RDAstrstr(a,b)	xRDAstrstr(a,b,__LINE__,__FILE__)
char *fixdosname(char *);
char *fixunixname(char *);
char *xRDAmkdir(char *,int,int *,int,int,char *);  /* returns NULL if no problem */
#define RDAmkdir(a,b,c) xRDAmkdir(a,b,c,FALSE,__LINE__,__FILE__)
#define AbsRDAmkdir(a,b,c) xRDAmkdir(a,b,c,TRUE__LINE__,__FILE__)
char *xRDAcreatepath(char *,int,char **,int,int,char *);  /* returns NULL if no problem */
#define RDAcreatepath(a,b,c) xRDAcreatepath(a,b,c,FALSE,__LINE__,__FILE__)
#define AbsRDAcreatepath(a,b,c) xRDAcreatepath(a,b,c,TRUE,__LINE__,__FILE__)
char *xRDAchown(char *,char *,int *,int,int,char *);  /* returns NULL if no problem */
#define RDAchown(a,b,c) xRDAchown(a,b,c,FALSE,__LINE__,__FILE__)
#define AbsRDAchown(a,b,c) xRDAchown(a,b,c,TRUE,__LINE__,__FILE__)
char *xRDAchmod(char *,int,int *,int,int,char *);  /* returns NULL if no problem */
#define RDAchmod(a,b,c) xRDAchmod(a,b,c,FALSE,__LINE__,__FILE__)
#define RDAchmod(a,b,c) xRDAchmod(a,b,c,TRUE,__LINE__,__FILE__)
#define Ratoi(x)	xRatoi(x,__LINE__,__FILE__)
int xRatoi(char *,int,char *);
#define RDACopyFile(a,b)	xRDACopyFile(a,b,FALSE,__LINE__,__FILE__)
#define AbsRDACopyFile(a,b)	xRDACopyFile(a,b,TRUE,__LINE__,__FILE__)
int xRDACopyFile(char *,char *,short,int,char *);
#define RDAMoveFile(a,b)	xRDAMoveFile(a,b,FALSE,__LINE__,__FILE__)
#define AbsRDAMoveFile(a,b)	xRDAMoveFile(a,b,TRUE,__LINE__,__FILE__)
int xRDAMoveFile(char *,char *,short,int,char *);
#define FileExists(a)	xFileExists(a,FALSE,__LINE__,__FILE__)
#define AbsFileExists(a)	xFileExists(a,TRUE,__LINE__,__FILE__)
int xFileExists(char *,short,int,char *);
short CheckDevLicense(char *);
char *READargument(char **,short *);

#define ADVExecuteProgramB(a,b,c,d)	xExecuteProgram(a,b,c,d,__LINE__,__FILE__)
#define ADVExecuteProgram(a,b,c)	xExecuteProgram(a,b,c,0,__LINE__,__FILE__)
#define ExecuteProgram(a,b)	xExecuteProgram(a,b,NULL,0,__LINE__,__FILE__)
short xExecuteProgram(char *,APPlib *,APPlib *,short,int,char *);

#define ADVExecute2Program(a,b,c)	xExecute2Program(a,b,c,TRUE,TRUE,__LINE__,__FILE__)
#define ADV2Execute2Program(a,b,c,d)	xExecute2Program(a,b,c,d,TRUE,__LINE__,__FILE__)
#define ADV3Execute2Program(a,b,c,d,e)	xExecute2Program(a,b,c,d,e,__LINE__,__FILE__)
#define Execute2Program(a,b)	xExecute2Program(a,b,NULL,TRUE,TRUE,__LINE__,__FILE__)
short xExecute2Program(char *,APPlib *,APPlib *,char,char,int,char *);
struct SENDMAIL_VARIABLES 
{
	char *from_name;
	char *from_addr;
	char *subject;
	char *body;
	char *tolist;
	char *cclist;
	char *bcclist;
	char *fname[10];	
	char *mtype[10];	
	char *dname[10];	
	char *fdata[10];	
	int ignerr;
};
#define RunVMimeSendmail(a)	xRunVMimeSendmail(a,__LINE__,__FILE__)
typedef struct SENDMAIL_VARIABLES MAIL_VARS;
short xRunVMimeSendmail(MAIL_VARS *,int,char *);


struct BINs
{
	char *data;
	int size; /*current write position*/
	int read; /*current read position*/
	int alloc; /*space allocated in unit*/
};

typedef struct BINs BIN;

#define reverse_memory(a,b)	xreverse_memory(a,b,__LINE__,__FILE__)
void xreverse_memory(char *,int,int,char *);

#define BINnewSIZE(size)	xBINnew(size,__LINE__,__FILE__)
#define BINnew()	xBINnew(1024,__LINE__,__FILE__)
#define BINclear(a)	xBINclear(a,__LINE__,__FILE__)
#define BINfree(a)	{xBINfree(a,__LINE__,__FILE__);a=NULL;}
#define BINclose(a)	xBINclose(a,__LINE__,__FILE__)
BIN *xBINnew(int,int,char *);
#define BINlen(a)	xBINlen(a,__LINE__,__FILE__)
int xBINlen(BIN *,int,char *);
void xBINadd(BIN *,void *,int,int,char *);
void xBINget(BIN *,void *,int,int,char *);

void xBINaddbin(BIN *,BIN *,char,int,char *);
void xBINaddshort(BIN *,short,char,int,char *);
void xBINaddint(BIN *,int,char,int,char *);
void xBINaddlong(BIN *,long,char,char,int,char *);
void xBINaddulong(BIN *,unsigned long,char,int,char *);
void xBINadddouble(BIN *,double,char,int,char *);
short xBINgetshort(BIN *,char,int,char *);
int xBINgetint(BIN *,char,int,char *);
long xBINgetlong(BIN *,char,char,int,char *);
unsigned long xBINgetulong(BIN *,char,int,char *);
double xBINgetdouble(BIN *,char,int,char *);
BIN *xBINgetbin(BIN *,char,int,char *);
BIN *xBINread(int,char,int,char *);
int xBINwrite(int,BIN *,char,int,char *);
short xBINreadReUse(BIN *,int,char,int,char *);


void xBINaddstringfixed(BIN *,char *,int,int,char *);
void xBINaddstring(BIN *,char *,int,char *);
int xBINgetstrlen(BIN *,int,char *);
char *xBINgetstring(BIN *,int,char *);
void xBINclose(BIN *,int,char *);
void xBINclear(BIN *,int,char *);
void xBINfree(BIN *,int,char *);
BIN *BINmemread(char *);
void BINmemwrite(BIN *,char *);
#define SeeBIN(a)	xSeeBIN(a,__LINE__,__FILE__)
void xSeeBIN(BIN *,int,char *);
#define BINaddstringfixed(a,b,c)	xBINaddstringfixed(a,b,c,__LINE__,__FILE__)
#define BINaddstring(a,b)	xBINaddstring(a,b,__LINE__,__FILE__)
#define BINgetstrlen(a)	xBINgetstrlen(a,__LINE__,__FILE__)
#define BINgetstring(a)	xBINgetstring(a,__LINE__,__FILE__)
#define BINadd(a,b,c)	xBINadd(a,b,c,__LINE__,__FILE__)
#define BINget(a,b,c)	xBINget(a,b,c,__LINE__,__FILE__)

#ifdef RS6000
#define BINread(a)	xBINread(a,TRUE,__LINE__,__FILE__)
#define BINwrite(a,b)	xBINwrite(a,b,TRUE,__LINE__,__FILE__)
#define BINreadReUse(a,b)	xBINreadReUse(a,b,TRUE,__LINE__,__FILE__)
#define BINaddbin(a,b)	xBINaddbin(a,b,TRUE,__LINE__,__FILE__)
#define BINaddshort(a,b)	xBINaddshort(a,b,TRUE,__LINE__,__FILE__)
#define BINaddint(a,b)	xBINaddint(a,b,TRUE,__LINE__,__FILE__)
#define BINaddlong(a,b)	xBINaddlong(a,b,TRUE,FALSE,__LINE__,__FILE__)
#define BINaddlongNormal(a,b)	xBINaddlong(a,b,TRUE,FALSE,__LINE__,__FILE__)
#define BINaddulong(a,b)	xBINaddulong(a,b,TRUE,__LINE__,__FILE__)
#define BINadddouble(a,b)	xBINadddouble(a,b,TRUE,__LINE__,__FILE__)
#define BINgetbin(a)	xBINgetbin(a,TRUE,__LINE__,__FILE__)
#define BINgetshort(a)	xBINgetshort(a,TRUE,__LINE__,__FILE__)
#define BINgetint(a)	xBINgetint(a,TRUE,__LINE__,__FILE__)
#define BINgetlong(a)	xBINgetlong(a,TRUE,FALSE,__LINE__,__FILE__)
#define BINgetlongNormal(a)	xBINgetlong(a,TRUE,FALSE,__LINE__,__FILE__)
#define BINgetulong(a)	xBINgetulong(a,TRUE,__LINE__,__FILE__)
#define BINgetdouble(a)	xBINgetdouble(a,TRUE,__LINE__,__FILE__)
#endif

#ifndef RS6000
#define BINread(a)	xBINread(a,FALSE,__LINE__,__FILE__)
#define BINwrite(a,b)	xBINwrite(a,b,FALSE,__LINE__,__FILE__)
#define BINreadReUse(a,b)	xBINreadReUse(a,b,FALSE,__LINE__,__FILE__)
#define BINaddbin(a,b)	xBINaddbin(a,b,FALSE,__LINE__,__FILE__)
#define BINaddshort(a,b)	xBINaddshort(a,b,FALSE,__LINE__,__FILE__)
#define BINaddint(a,b)	xBINaddint(a,b,FALSE,__LINE__,__FILE__)
#ifdef RDA_64BITS
#define BINaddlong(a,b)	xBINaddlong(a,b,FALSE,TRUE,__LINE__,__FILE__)
#define BINaddlongNormal(a,b)	xBINaddlong(a,b,FALSE,FALSE,__LINE__,__FILE__)
#define BINgetlong(a)	xBINgetlong(a,FALSE,TRUE,__LINE__,__FILE__)
#define BINgetlongNormal(a)	xBINgetlong(a,FALSE,FALSE,__LINE__,__FILE__)
#else
#define BINaddlong(a,b)	xBINaddlong(a,b,FALSE,FALSE,__LINE__,__FILE__)
#define BINaddlongNormal(a,b)	xBINaddlong(a,b,FALSE,FALSE,__LINE__,__FILE__)
#define BINgetlong(a)	xBINgetlong(a,FALSE,FALSE,__LINE__,__FILE__)
#define BINgetlongNormal(a)	xBINgetlong(a,FALSE,FALSE,__LINE__,__FILE__)
#endif /* RDA_64BITS */
#define BINaddulong(a,b)	xBINaddulong(a,b,FALSE,__LINE__,__FILE__)
#define BINadddouble(a,b)	xBINadddouble(a,b,FALSE,__LINE__,__FILE__)
#define BINgetbin(a)	xBINgetbin(a,FALSE,__LINE__,__FILE__)
#define BINgetshort(a)	xBINgetshort(a,FALSE,__LINE__,__FILE__)
#define BINgetint(a)	xBINgetint(a,FALSE,__LINE__,__FILE__)
#define BINgetulong(a)	xBINgetulong(a,FALSE,__LINE__,__FILE__)
#define BINgetdouble(a)	xBINgetdouble(a,FALSE,__LINE__,__FILE__)
#endif /* NOT RS6000 */

#ifdef RDA_64BITS
#define ADVBINaddlong(a,b,c)	xBINaddlong(a,b,c,TRUE,__LINE__,__FILE__)
#define ADVBINgetlong(a,b)	xBINgetlong(a,b,TRUE,__LINE__,__FILE__)
#else
#define ADVBINaddlong(a,b,c)	xBINaddlong(a,b,c,FALSE,__LINE__,__FILE__)
#define ADVBINgetlong(a,b)	xBINgetlong(a,b,FALSE,__LINE__,__FILE__)
#endif /* RDA_64BITS */
#define ADVBINread(a,b)	xBINread(a,b,__LINE__,__FILE__)
#define ADVBINwrite(a,b,c)	xBINwrite(a,b,c,__LINE__,__FILE__)
#define ADVBINreadReUse(a,b,c)	xBINreadReUse(a,b,c,__LINE__,__FILE__)
#define ADVBINaddbin(a,b,c)	xBINaddbin(a,b,c,__LINE__,__FILE__)
#define ADVBINaddshort(a,b,c)	xBINaddshort(a,b,c,__LINE__,__FILE__)
#define ADVBINaddint(a,b,c)	xBINaddint(a,b,c,__LINE__,__FILE__)
#define ADVBINaddulong(a,b,c)	xBINaddulong(a,b,c,__LINE__,__FILE__)
#define ADVBINadddouble(a,b,c)	xBINadddouble(a,b,c,__LINE__,__FILE__)
#define ADVBINgetbin(a,b)	xBINgetbin(a,b,__LINE__,__FILE__)
#define ADVBINgetshort(a,b)	xBINgetshort(a,b,__LINE__,__FILE__)
#define ADVBINgetint(a,b)	xBINgetint(a,b,__LINE__,__FILE__)
#define ADVBINgetulong(a,b)	xBINgetulong(a,b,__LINE__,__FILE__)
#define ADVBINgetdouble(a,b)	xBINgetdouble(a,b,__LINE__,__FILE__)


struct binheads
{
	char *name;
	long position;
	long length;
	BIN *bin;
};
typedef struct binheads BINhead;
struct binlibs 
{
	int numlibs;
	BINhead *header;
};
typedef struct binlibs BINlib;
#define getlibbin(x,y,z)		xgetlibbin(x,y,z,__LINE__,__FILE__)
/* getlibbin has two arguments, the first a character string (char *) used
	to get the full name of the binary library, the second a character
	string (char *) used to name a specific binary structure within the
	binary to retrieve. This function returns a pointer to a binary
	(BIN *) structure */
#define writelibbin(x,y,z)	xwritelibbin(x,y,z,__LINE__,__FILE__)
/* writelibbin has three arguments, the first the full name of the binary to
	be written to, the second a pointer to a BIN * binary structure, the
	third the name (char * charcter string) of the BIN * structure. */
#define deletelibbin(x,y)	xdeletelibbin(x,y,__LINE__,__FILE__)
/* deletelibbin has two arguments, the first the full name of the binary 
	library that currently contains the binary to delete, the second 
	a character string reflecting the name of the BIN * structure to 
	delete. */
#define getunsortedlibnames(x,y)	xgetlibnames(x,FALSE,y,__LINE__,__FILE__)
#define getlibnames(x,y)		xgetlibnames(x,TRUE,y,__LINE__,__FILE__)
/* getlibnames has one argument the full name of the binary library that the
	names of the binaries is requested from.  This function returns an
	APPlib structure containing the names of the binarys contained in 
	a specific library. */ 
#define SEEBINLIB(x)	xSEEBINLIB(x,__LINE__,__FILE__)
/* SEEBINLIB is a diagnostic function used to display the contents of a 
	BINLIB structure */

void xSEEBINLIB(BINlib *,int,char *);
#define freebinlib(a)	{xfreebinlib(a,__LINE__,__FILE__);a=NULL;}
void xfreebinlib(BINlib *,int,char *);
BIN *xgetlibbin(char *,char *,short,int,char *);
#define readlibrary(a,b,c,d,e,f)	xreadlibrary(a,b,c,d,e,f,__LINE__,__FILE__)
long xreadlibrary(BINlib *,char *,short *,BIN *,char *,short,int,char *);
short xwritelibbin(char *,BIN *,char *,int,char *);
short xdeletelibbin(char *,char *,int,char *);
APPlib *xgetlibnames(char *,short,short,int,char *);
#define Ropenfile(a,b)	xRopenfile(a,b,__LINE__,__FILE__)
int xRopenfile(char *,int,int,char *);

int decode_base64_to_file(char *,char *);
char *encode_base64_from_file(char *);

#ifdef WIN32
void RotateErrorLog();
int xWIN32TRACE(int,char *);
FILE* pt_popen(char *,char *);
int pt_pclose(FILE *);
unsigned long RunSilent(char*);
#define popen pt_popen
#define pclose pt_pclose
#define system RunSilent
#endif

/*rfile.h - header file for rda file manipulation functions*/
#ifdef WIN32
#define F_SETLK		1701
#define F_SETLKW	1700
#define F_WRLCK		1699
#define F_UNLCK		1698
#endif
#define Rcreat(x)	xRcreat(x,__FILE__,__LINE__)
#define Ropen(x)	xRopen(x,__FILE__,__LINE__)
#define Rclose(x)	xRclose(x,__FILE__,__LINE__)
#define Rvread(x,y,z)	xRvread(x,y,z,__FILE__,__LINE__)
#define Rread(x,y,z)	xRread(x,y,z,__FILE__,__LINE__)
#define Rreadlong(x,y)	xRreadlong(x,y,__FILE__,__LINE__)
#define Rseek(x,y,z)	xRseek(x,y,z,__FILE__,__LINE__)
#define Rlock(x,y,z,w,u)	xRlock(x,y,z,w,u,__FILE__,__LINE__)
#define Rwrite(x,y,z)	xRwrite(x,y,z,__FILE__,__LINE__)
#define Rwritelong(x,y)	xRwritelong(x,y,__FILE__,__LINE__)

int xRcreat(char *,char *,int line);
int xRopen(char *,char *,int);
char xRseek(int,long,int,char *,int);
char xRlock(int,short,int,long,long,char *,int);
int xRvread(int,void *,int,char *,int);
char xRread(int,void *,int,char *,int);
char xRreadlong(int,long *,char *,int);
char xRwrite(int,void *,int,char *,int);
char xRwritelong(int,long,char *,int);
void xRclose(int,char *,int);

/*vfile.h - header file for rda variable length file functions*/

struct VFLCKs
{
	long recno;
	short c;
};
typedef struct VFLCKs *VFLCK;

struct VFs
{
	int fd;
	long offrec;/*offset of offset record*/
	long maxrec;/*maximum number of records, increment by 100*/
	long numrec;/*number of records(in offset record & on file)*/
	long recoff;/*current record offset*/
	long reclen;/*current record length*/
	long offfree;/*offset of free record*/
	long maxfree;/*maximum number of free records*/
	long numfree;/*number of free records*/
	long curfree;/*current position to search free records for*/
	char *filename;
	long nolock;
	VFLCK lck;
	long maxlock;
	long VFmaximum;
};
typedef struct VFs *VF;

#define ADVVFcreat(a,b)	xVFcreat(a,b,__LINE__,__FILE__)
#define VFcreat(a)	xVFcreat(a,0,__LINE__,__FILE__)
char xVFcreat(char *,short,int,char *);
	/*create's a variable length file. returns true on error.*/
#define VFopen(a)	xVFopen(a,__LINE__,__FILE__)
VF xVFopen(char *,int,char *);
	/*open's a variable length file. returns NULL on error.*/
#define VFread(a,b)	xVFread(a,b,__LINE__,__FILE__)
BIN *xVFread(VF,long,int,char *);
	/*read's recno, returns bin, or NULL on error.*/
#define VFwrite(a,b,c)	xVFwrite(a,b,c,__LINE__,__FILE__)
long xVFwrite(VF,long,BIN *,int,char *);
	/*write's bin to recno. if recno zero, then it
		will add record, returning record number. returns -1 if fail.*/
#define VFdelrec(a,b)	xVFdelrec(a,b,__LINE__,__FILE__)
void xVFdelrec(VF,long,int,char *);
	/*marks record as deleted, free's space used, and unlocks 
	it if it was locked.*/
#define VFclose(a)	xVFclose(a,__LINE__,__FILE__)
void xVFclose(VF,int,char *);/*closes vf and free's memory used by.*/
#define VFlock(a,b,c)	xVFlock(a,b,c,__LINE__,__FILE__)
char xVFlock(VF,long,char,int,char *);
/*locks a record, if the record number is zero, it
locks the control portion of the file, if it is less than zero it locks the
entire file, otherwise it locks the defined record. The third arguement is to
a boolean to wait till lock is free. Returns true on error, or if lock can not
be obtained.*/

#define VFunlock(a,b)	xVFunlock(a,b,__LINE__,__FILE__)
void xVFunlock(VF,long,int,char *);
/*unlocks specified portion of file, see lock for record number definition.*/
#define VFrestructure(a)	xVFrestructure(a,__LINE__,__FILE__)
char xVFrestructure(char *,int,char *);/*restructures file name*/

#define VFloop(a,b)	xVFloop(a,b,__LINE__,__FILE__)
char xVFloop(VF,char (*)(long,BIN *),int,char *);
	/*loop through variable file records*/
#define VFprint(a)	xVFprint(a,__LINE__,__FILE__)
void xVFprint(VF,int,char *);/*prints VF file layout*/
#define VFreadReUse(a,b,c)	xVFreadReUse(a,b,c,__LINE__,__FILE__)
short xVFreadReUse(BIN *,VF,long,int,char *);
	/* read's recno reusing existing BIN, returns 0  or -1 on error.*/

struct RDA_PFILES
{
	FILE *pfile;
	char *name;
	char *perm;
	char wrote_something;
#ifdef WIN32
	HANDLE lph;
	char *jobinfo;
	DWORD jobno;
	HDC hdc;
#endif
};
typedef struct RDA_PFILES RDA_PFILE;

#define RDA_PFILE_NEW(a,b)	xRDA_PFILE_NEW(a,b,__LINE__,__FILE__)
RDA_PFILE *xRDA_PFILE_NEW(char *,char *,int,char *);
#define RDA_popen(a,b)	xRDA_popen(a,b,__LINE__,__FILE__)
RDA_PFILE *xRDA_popen(char *,char *,int,char *);
#define RDA_pclose(a)	xRDA_pclose(a,__LINE__,__FILE__)
void xRDA_pclose(RDA_PFILE *,int,char *);
int RDA_fprintf(RDA_PFILE *,char *,...);
#define RDA_fflush(a) xRDA_fflush(a,__LINE__,__FILE__)
void xRDA_fflush(RDA_PFILE *,int,char *);
int RDA_snprintf(char *,size_t,char *,...);

#endif  // MISC_H
