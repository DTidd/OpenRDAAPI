
/*****************************************************************************
			SECURITY HEADER 
			    sec.h
	purpose: Contains macros and function prototypes for security 
*****************************************************************************/

#ifndef SECH_P
#define SECH_P
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <misc.hpp>
#include <evaluate.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <gui.hpp>

struct SECfields
{
	char *name;
	char read;
	char write;
	char print;
};
typedef struct SECfields SECfield;

struct SECfiles
{
#ifdef RADDOG
	char active;
#endif
	char *file;
	char fieldsecurity;
	char read;
	char write;
	char print;
	char can_delete;
	char *expression;
	char *p_expression;
	int num;
	SECfield *fields;
};
typedef struct SECfiles SECfile;

struct SECmodules
{
	char *module;
	char allprocs;
	char allfiles;
	char execfunc;
	char read;
	char write;
	char print;
	char can_delete;
	int num;
	SECfile *files;
}; 
typedef struct SECmodules SECmodule;

extern short MODSEC_FILENO;
extern short PROCSEC_FILENO;
extern short FILESEC_FILENO;
extern short FLDSEC_FILENO;
extern short FLDSEC_FILENO;
extern short SEC_USERS_FILENO;
extern short SEC_TOKEN_FILENO;
extern short SEC_FINEACC;
extern short SEC_FINRACC;
extern short SEC_FINBACC;
extern short DO_SECURITY_FILENO;
extern SECmodule *SECMOD;
extern short numSECMOD;
extern short ARCHWORK_FILENO;
extern short ARCHUSER_FILENO;
extern char *SECURITY_USERLOGIN;

SECmodule *GetMODSEC(char *);
SECmodule *FINDMODSEC(char *);
SECfile *GetFILESEC(SECmodule *,char *);
SECfile *FINDFILESEC(SECmodule *,char *);
#define FIELDsec(a,b,c)	xFIELDsecurity(a,b,c,TRUE,FALSE,TRUE,__LINE__,__FILE__)
#define FIELDsecurity(a,b,c,d,e,f)	xFIELDsecurity(a,b,c,d,e,f,__LINE__,__FILE__)
short xFIELDsecurity(char *,char *,char *,char,char,char,int,char *);
SECfield *GetFIELDSEC(SECmodule *,SECfile *,char *);
SECfield *FINDFIELDSEC(SECmodule *,SECfile *,char *);

#define MODsecurity(a,b,c,d,e,f,g)	xMODsecurity(a,b,c,d,e,f,g,__LINE__,__FILE__)
/* MODsecurity tests module security for a specific user. */
#define PROCsecurity(a,b)	xPROCsecurity(a,b,__LINE__,__FILE__)
/* PROCsecurity tests process security for a specific user */
#define FILEsecurity(a,b,c,d,e)	xFILEsecurity(a,b,c,d,e,__LINE__,__FILE__)
/* FILEsecurity tests file security for a specific user */
#define ADV2RECORDsec(x,y,z,w)	xRECORDsec(x,y,z,w,__LINE__,__FILE__)
#define ADVRECORDsec(x,y,z)	xRECORDsec(x,y,z,0,__LINE__,__FILE__)
#define RECORDsec(x)	xRECORDsec(x,NULL,NULL,0,__LINE__,__FILE__)
/* RECORDsec tests record level security */
#define ADV2RECORDsecDesc(x,y,z,a)	xRECORDsecDesc(x,y,z,a,__LINE__,__FILE__)
#define ADVRECORDsecDesc(x,y,z)	xRECORDsecDesc(x,y,z,0,__LINE__,__FILE__)
#define RECORDsecDesc(x)	xRECORDsecDesc(x,NULL,NULL,0,__LINE__,__FILE__)
/* RECORDsecDesc returns a character string descibing why RECORDsec Failed */
#define OPNNRDsec(a,b,c,d,e)	xOPNNRDsec(a,b,c,d,e,__LINE__,__FILE__)
/* OPNNRDsec Open File With Security */
#define EQLNRDKEYsec(x,y,z)	xEQLNRDKEYsec(x,y,z,NULL,NULL,__LINE__,__FILE__)
#define ADVEQLNRDKEYsec(x,y,z,a,b)	xEQLNRDKEYsec(x,y,z,a,b,__LINE__,__FILE__)
/* EQLNRDKEYsec Get Equal record using specified key using security */
#define EQLNRDsec(x,y)	xEQLNRDsec(x,y,NULL,NULL,__LINE__,__FILE__)
#define ADVEQLNRDsec(x,y,z,a)	xEQLNRDsec(x,y,z,a,__LINE__,__FILE__)
/* EQLNRDsec Get Equal record using security */
#define GTENRDsec(x,y)	xGTENRDsec(x,y,NULL,NULL,__LINE__,__FILE__)
#define ADVGTENRDsec(x,y,z,a)	xGTENRDsec(x,y,z,a,__LINE__,__FILE__)
/* GTENRDsec Get Record Greater than or equal to using security */
#define PRVNRDsec(x,y)	xPRVNRDsec(x,y,NULL,NULL,__LINE__,__FILE__)
#define ADVPRVNRDsec(x,y,z,a)	xPRVNRDsec(x,y,z,a,__LINE__,__FILE__)
#define LSTNRDsec(x,y)	xLSTNRDsec(x,y,NULL,NULL,__LINE__,__FILE__)
#define ADVLSTNRDsec(x,y,z,a)	xLSTNRDsec(x,y,z,a,__LINE__,__FILE__)
#define BEGNRDsec(a)	xBEGNRDsec(a,NULL,NULL,__LINE__,__FILE__)
#define ADVBEGNRDsec(a,b,c)	xBEGNRDsec(a,b,c,__LINE__,__FILE__)
#define SEQNRDsec(a)	xSEQNRDsec(a,NULL,NULL,__LINE__,__FILE__)
#define ADVSEQNRDsec(a,b,c)	xSEQNRDsec(a,b,c,__LINE__,__FILE__)
#define LTENRDsec(x,y)	xLTENRDsec(x,y,NULL,NULL,__LINE__,__FILE__)
#define ADVLTENRDsec(x,y,z,a)	xLTENRDsec(x,y,z,a,__LINE__,__FILE__)
/* LTENRDsec Get Record Less than or equal to using security */
#define FRSNRDsec(x,y)	xFRSNRDsec(x,y,NULL,NULL,__LINE__,__FILE__)
#define ADVFRSNRDsec(x,y,z,a)	xFRSNRDsec(x,y,z,a,__LINE__,__FILE__)
/* FRSNRDsec Get First Record in sorted sequence using security */
#define NXTNRDsec(x,y)	xNXTNRDsec(x,y,NULL,NULL,__LINE__,__FILE__)
#define ADVNXTNRDsec(x,y,z,a)	xNXTNRDsec(x,y,z,a,__LINE__,__FILE__)
/* NXTNRDsec Get Next Record in sorted sequence using security */
#define WRTNRDsec(x,y,z)	xWRTNRDsec(x,y,z,NULL,NULL,__LINE__,__FILE__)
#define ADVWRTNRDsec(x,y,z,a,b)	xWRTNRDsec(x,y,z,a,b,__LINE__,__FILE__)
/* WRTNRDsec Write Record using security */
#define InitializeSecurity()	xInitializeSecurity(__LINE__,__FILE__)
/* InitializeSecurity Initialize the Security Subsystems and open appropriate 
	files */
#define ShutdownSecurity()	xShutdownSecurity(__LINE__,__FILE__)
/* ShutdownSecurity Shutdown Security and close files associated with security */
#define FIELDscrn(x,y,z)	xFIELDscrn(x,y,z,__LINE__,__FILE__)

short xMODsecurity(char *,short *,short *,char,char,char,char,int,char *);
short xPROCsecurity(char *,char *,int,char *);
short xFILEsecurity(char *,char *,char,char,char,int,char *);
short xRECORDsec(short,void (*)(...),void *,short,int,char *);
char *xRECORDsecDesc(short,void (*)(...),void *,short,int,char *);
short xOPNNRDsec(char *,char *,char,char,char,int,char *);
short xEQLNRDKEYsec(short,short,void *,void (*)(...),void *,int,char *);
short xEQLNRDsec(short,short,void (*)(...),void *,int,char *);
short xGTENRDsec(short,short,void (*)(...),void *,int,char *);
short xPRVNRDsec(short,short,void (*)(...),void *,int,char *);
short xLSTNRDsec(short,short,void (*)(...),void *,int,char *);
short xBEGNRDsec(short,void (*)(...),void *,int,char *);
short xSEQNRDsec(short,void (*)(...),void *,int,char *);
short xFIELDsec(char *,char *,char *,int,char *);
short xFIELDscrn(short,char *,short *,int,char *);
short xWRTNRDsec(short,short,char *,void (*)(...),void *,int,char *);
short xNXTNRDsec(short,short,void (*)(...),void *,int,char *);
short xFRSNRDsec(short,short,void (*)(...),void *,int,char *);
short xLTENRDsec(short,short,void (*)(...),void *,int,char *);
short xInitializeSecurity(int,char *);
void xShutdownSecurity(int,char *);
#define ADVWRTNRDtranssec(a,b,c,d,e)	xWRTNRDtranssec(a,b,c,d,e,__LINE__,__FILE__)
#define WRTNRDtranssec(a,b,c)	xWRTNRDtranssec(a,b,c,NULL,NULL,__LINE__,__FILE__)
short xWRTNRDtranssec(short,short,char *,void (*)(...),void *,int,char *);
char *DefaultPrinter(void);
#define Users_Department()	xUsers_Department(__LINE__,__FILE__)
char *xUsers_Department(int,char *);
char *Users_Scan_Directory(void);
short Users_DomenuStyle(void);
double Users_Approval_Limit(void);
char Users_Able_to_Approve(void);
#define FLDNUMsec(a,b,c,d,e)	xFLDNUMsec(a,b,c,d,e,__LINE__,__FILE__)
NRDfield *xFLDNUMsec(short,short,short,short,short,int,char *);
#define FLDNRDsec(a,b,c,d,e)	xFLDNRDsec(a,b,c,d,e,__LINE__,__FILE__)
NRDfield *xFLDNRDsec(short,char *,short,short,short,int,char *);
#define ADVWRTTRANSsec(a,b,c,d,e,f)	xWRTTRANSsec(a,b,c,d,e,f,__LINE__,__FILE__)
#define WRTTRANSsec(a,b,c,d)	xWRTTRANSsec(a,b,c,d,NULL,NULL,__LINE__,__FILE__)
short xWRTTRANSsec(short,short,char *,RDATData *,void (*)(...),void *,int,char *);

/* VIRTUAL STUFF */
struct DFvirtuals
{
	int vtype;
	short num;
	DFincvir *incvir;
	char *module;
	char *name;
	short avail2rsrc;
	short type;
	short length;
	short when;
	char *expression;
	char *desc;
};
typedef struct DFvirtuals DFvirtual;

#define FINDVIRTUAL(a,b)	xFINDVIRTUAL(a,b,__LINE__,__FILE__)
int xFINDVIRTUAL(RDArsrc *,char *,int,char *);
#define FINDSCROLLLIST(a,b)	xFINDSCROLLLIST(a,b,__LINE__,__FILE__)
int xFINDSCROLLLIST(RDArsrc *,char *,int,char *);
#define FreeDFvirtual(a)	{xFreeDFvirtual(a,__LINE__,__FILE__);a=NULL;}
void xFreeDFvirtual(DFvirtual *,int,char *);
DFvirtual *DFvirtualNEW(char *,char *);
#define getDFvirtual(a,b)	xgetDFvirtual(a,b,__LINE__,__FILE__)
DFvirtual *xgetDFvirtual(char *,char *,int,char *);
#define writeDFvirtual(a,b)	xwriteDFvirtual(a,b,__LINE__,__FILE__)
short xwriteDFvirtual(char *,DFvirtual *,int,char *);
#define getvirtualdesc(a,b)	xgetvirtualdesc(a,b,__LINE__,__FILE__)
char *xgetvirtualdesc(char *,char *,int,char *);
/* VIRTUAL STUFF ABOVE */
/****************************************************************************/
/* THE FOLLOWING WAS ADDED FOR RDATABLES */
struct TABLEVALUES
{
	double amount;
	double base;
	double rate;
};
typedef struct TABLEVALUES TABLEVALUE;

struct RDATABLEs
{
	char *module;  /* Module Table Def Belongs to */
	char *name;    /* Name of the Table Definition */
	char *title1;   /* Title #1 of Table */
	char *title2;  /* Title #2 of Table */
	char *desc;    /* Table Description */
	char inclusive;
	int num; /* number of records in the table */
	TABLEVALUE *values;
};
typedef struct RDATABLEs RDATABLE;

#define RDATABLENEW(a,b)	xRDATABLENEW(a,b,__LINE__,__FILE__)
RDATABLE *xRDATABLENEW(char *,char *,int,char *);
#define FreeRDATABLE(a)	{xFreeRDATABLE(a,__LINE__,__FILE__);a=NULL;}
void xFreeRDATABLE(RDATABLE *,int,char *);
#define getRDATablebin(a,b)	xgetRDATablebin(a,b,__LINE__,__FILE__)
short xgetRDATablebin(char *,RDATABLE *,int,char *);
#define writeRDATablebin(a,b)	xwriteRDATablebin(a,b,__LINE__,__FILE__)
short xwriteRDATablebin(char *,RDATABLE *,int,char *);
#define getRDATabledesc(a,b,c,d,e)	xgetRDATabledesc(a,b,c,d,e,__LINE__,__FILE__)
short xgetRDATabledesc(char *,char *,char **,char **,char **,int,char *);
#define RDATABLEBASE	0
#define RDATABLERATE	1
#define RDATABLECOMP	2
#define ComputeRDATableBase(a,b,c,d)	xComputeRDATable(a,RDATABLEBASE,b,c,d,__LINE__,__FILE__)
#define ComputeRDATableRate(a,b,c,d)	xComputeRDATable(a,RDATABLERATE,b,c,d,__LINE__,__FILE__)
double xComputeRDATable(RDATABLE *,short,double,void (*)(...),void *,int,char *);
extern RDATABLE *Internal_RDATABLES;
extern int Internal_num_RDATABLES;
#define COMPUTERDATABLE(a,b,c,d)	xCOMPUTERDATABLE(a,b,c,d,__LINE__,__FILE__)
double xCOMPUTERDATABLE(char *,void (*)(...),void *,short,int,char *);
#define FreeInternalRDATables()	xFreeInternalRDATables(__LINE__,__FILE__)
void xFreeInternalRDATables(int,char *);

/* THE PREVIOUS WAS ADDED FOR RDATABLES */
/****************************************************************************/
/****************************************************************************/
/* THE FOLLOWING WAS ADDED FOR RDAMATRIX */

struct MATRIXcells
{
	char *name;
	short type; /* Field Type */
	unsigned len; /* length of data element */
	short displayed;
};
typedef struct MATRIXcells MATRIXcell;
struct MATRIXvalues
{
	union rda_field_types data; /* Union of pointer to the data part */ 
	unsigned dlen; /* length of allocated data space..... */
};
typedef struct MATRIXvalues MATRIXvalue;
struct MATRIXmems
{
	short sensitive;
	int num;
	MATRIXvalue *values;
};
typedef struct MATRIXmems MATRIXmem;
struct MATRIXrecs
{
	int num; /* number of members in a record */
	MATRIXmem *mems; /* pointer to the structure of members */
};
typedef struct MATRIXrecs MATRIXrec;
struct AXISmems
{
	char *title1;
	char *title2;
	union rda_field_types data; /* Union of pointer to the data part */ 
	unsigned dlen;
};
typedef struct AXISmems AXISmem;
struct MATRIXAXISS
{
	char *title1;
	char *title2;
	char *desc;
	int columns;
	int axis_type; /* 0 - Equal To */
		   /* 1 - Greater Than */
		   /* 2 - Greater Than or Equal To */
		   /* 3 - Less Than */
		   /* 4 - Less Than or Equal To */
		   /* 5 - No Value Only */
	short type; /* Field Type */
	unsigned len; /* length of data element */
	int contype; /* connect type */
	char *conresource; /* connect resource */
	short print_title1;
	short print_title2;
	int num;
	AXISmem *mem;
};
typedef struct MATRIXAXISS MATRIXAXIS;

struct RDAMATRIXs
{
	char *module;  /* Module Matrix Def Belongs to */
	char *name;    /* Name of the Matrix Definition */
	char *title1;   /* Title #1 of Matrix */
	char *title2;  /* Title #2 of Matrix */
	char *desc;    /* Matrix Description */
	int display_cell;
	int matrix_buttons;
	int cell_config; 
	int locate_method; /* 0 - By Axis' Values */
			   /* 1 - Linearly by cells / Regardless of Axis */
	MATRIXAXIS xaxis;
	MATRIXAXIS yaxis;
	int numdefs; /* number of values in a cell */
	MATRIXcell *defs; /* Definitions of cell values */
	int num; /* number of records in the matrix */
	MATRIXrec *recs; /* pointer to the array of record structures */
	int which; /* which way to use includes, 0-ALL, 1-One of Many */
	int numinc; /* number include files */
	DFincvir *incvir; /* array of include files */
};
typedef struct RDAMATRIXs RDAMATRIX;

#define RDAMATRIXNEW(a,b)	xRDAMATRIXNEW(a,b,__LINE__,__FILE__)
RDAMATRIX *xRDAMATRIXNEW(char *,char *,int,char *);
#define FreeRDAMATRIX(a)	{xFreeRDAMATRIX(a,__LINE__,__FILE__);a=NULL;}
void xFreeRDAMATRIX(RDAMATRIX *,int,char *);
#define getRDAMatrixbin(a,b)	xgetRDAMatrixbin(a,b,__LINE__,__FILE__)
short xgetRDAMatrixbin(char *,RDAMATRIX *,int,char *);
#define writeRDAMatrixbin(a,b)	xwriteRDAMatrixbin(a,b,__LINE__,__FILE__)
short xwriteRDAMatrixbin(char *,RDAMATRIX *,int,char *);
#define getRDAMatrixdesc(a,b,c,d,e)	xgetRDAMatrixdesc(a,b,c,d,e,__LINE__,__FILE__)
short xgetRDAMatrixdesc(char *,char *,char **,char **,char **,int,char *);
extern RDAMATRIX *Internal_RDAMATRIXES;
extern int Internal_num_RDAMATRIXES;
#define GetInternalMatrix(a,b)	xGetInternalMatrix(a,b,__LINE__,__FILE__)
RDAMATRIX *xGetInternalMatrix(char *,char *,int,char *);
#define Free_Internal_RDAMATRIXES()	xFree_Internal_RDAMATRIXES(__LINE__,__FILE__)
void xFree_Internal_RDAMATRIXES(int,char *);

struct MATRIX_RUNTIMES
{
	char *module;
	char *name;
	int rangetype;
	char *rangename;
	NRDfield *from_col;
	NRDfield *to_col;
	NRDfield *from_row;
	NRDfield *to_row;
	NRDfield *data;
	RDAMATRIX *m;
};
typedef struct MATRIX_RUNTIMES MATRIX_RUNTIME;

#define MATRIX_RUNTIME_NEW(a,b)	xMATRIX_RUNTIME_NEW(a,b,__LINE__,__FILE__)
MATRIX_RUNTIME *xMATRIX_RUNTIME_NEW(char *,char *,int,char *);
#define FreeMATRIX_RUNTIME(a)	{xFreeMATRIX_RUNTIME(a,TRUE,__LINE__,__FILE__);a=NULL;}
#define FreeMATRIX_RUNTIME_EVAL(a,b)	{xFreeMATRIX_RUNTIME(a,b,__LINE__,__FILE__);a=NULL;}
void xFreeMATRIX_RUNTIME(MATRIX_RUNTIME *,short,int,char *);
#define COMPUTE_MATRIX_LINEARLY(a)	xCOMPUTE_MATRIX_LINEARLY(a,__LINE__,__FILE__)
NRDfield *xCOMPUTE_MATRIX_LINEARLY(char *,int,char *);
#define COMPUTE_MATRIX_ROWCOLUMN(a)	xCOMPUTE_MATRIX_ROWCOLUMN(a,__LINE__,__FILE__)
NRDfield *xCOMPUTE_MATRIX_ROWCOLUMN(char *,int,char *);

/* THE PREVIOUS WAS ADDED FOR RDAMATRIX */
/****************************************************************************/
struct RDAGenericSetups
{
	char *module;
	char *name;
	short type;
	short length;
	union rda_field_types value;
	char *desc;
	char *label;
};
typedef struct RDAGenericSetups RDAGenericSetup;

extern RDAGenericSetup *Internal_RDAGenericSetup;
extern int Internal_num_RDAGenericSetup;

#define RDAGenericSetupNew(a,b)	xRDAGenericSetupNew(a,b,__LINE__,__FILE__)
RDAGenericSetup *xRDAGenericSetupNew(char *,char *,int,char *);
#define FreeRDAGenericSetup(a)	xFreeRDAGenericSetup(a,__LINE__,__FILE__)
void xFreeRDAGenericSetup(RDAGenericSetup *,int,char *);
#define getRDAGenericSetupbin(a,b)	xgetRDAGenericSetupbin(a,b,__LINE__,__FILE__)
short xgetRDAGenericSetupbin(char *,RDAGenericSetup *,int,char *);
#define writeRDAGenericSetupbin(a,b)	xwriteRDAGenericSetupbin(a,b,__LINE__,__FILE__)
short xwriteRDAGenericSetupbin(char *,RDAGenericSetup *,int,char *);
#define getRDAGenericSetupdesc(a,b,c)	xgetRDAGenericSetupdesc(a,b,c,__LINE__,__FILE__)
short xgetRDAGenericSetupdesc(char *,char *,char **,int,char *);
#define FreeInternalGenericSetup()	xFreeInternalGenericSetup(__LINE__,__FILE__)
void xFreeInternalGenericSetup(int,char *);
#define COMPUTEGenericSetup(a,b,c)	xCOMPUTEGenericSetup(a,b,c,__LINE__,__FILE__)
NRDfield *xCOMPUTEGenericSetup(char *,void (*)(...),void *,int,char *);
#define ComputeRDAGenericSetup(a,b,c)	xComputeRDAGenericSetup(a,b,c,__LINE__,__FILE__)
NRDfield *xComputeRDAGenericSetup(RDAGenericSetup *,void (*)(...),void *,int,char *);
char *DefaultMediaOutDevice(void);
#define RDA_ReturnAccountManager()	RDA_ReturnAccountCodeStuff(0)
#define RDA_ReturnAccountGrantIdentification()	RDA_ReturnAccountCodeStuff(1)
#define RDA_ReturnAccountReferenceType()	RDA_ReturnAccountCodeStuff(2)
#define RDA_ReturnCurrentSubsidiaryAccount()	RDA_ReturnAccountCodeStuff(3)
char *RDA_ReturnAccountCodeStuff(short);
#define ADVwriteRDATablebin(a,b)	xADVwriteRDATablebin(a,b,__LINE__,__FILE__)
short xADVwriteRDATablebin(char *,RDATABLE *,int,char *);
char *Users_Phone(void);
char *Users_Phone_Extension(void);
char *Users_Password(void);
char *Users_Login(void);
char *Users_Email(void);
void SET_SECURITY_USERLOGIN(void);
char *Department_Supervisor(char *);
char Users_BrowseListLabel();
char *Users_Supervisor(void);
char *Users_SiteID(void);
char *UsersVenpmtSignatureFilename(void);
char *UsersVenpmtSignaturePassword(void);
char *UsersPayrollSignatureFilename(void);
char *UsersPayrollSignaturePassword(void);
char *UsersPurordSignatureFilename(void);
char *UsersPurordSignaturePassword(void);
short UsersPushButtonStyle(void);
char UsersAllowPostActivity(void);
char UsersAllowMultipleEntries(void);
char UsersAllowBeginningBalances(void);
char UsersWorkstationValidated(void);
char *UsersWorkstationLogin(void);
char *UsersWorkstationPassword(void);
char *UsersWorkstationDocuments(void);
char UsersIgnoreRestrictedProcessConstraints(void);
char UsersDisengageWorkFlowNavigation(void);
char UsersAutoComplete(void);
char UsersJournalEntryApprove(void);
char UsersVoucherApprove(void);
char AdminEdits(void);
char SubmitRequisitions(void);
char ApproveRequisitions(void);
char *Users_LocationID(void);
char UsersDisplayPurgeFunctions(void);
char *Users_Name(void);

#ifdef USE_MYSQL
void INITNRD_MYSQL_GSVS(void);
#endif /* USE_MYSQL */

#endif
