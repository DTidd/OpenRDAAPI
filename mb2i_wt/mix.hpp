
/******************************************************************************
				MIX FUNCTIONS HEADER
					mix.h
******************************************************************************/
#ifndef MIX_H
#define MIX_H
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <rdalic.hpp>
#include <olh.hpp>
#include <trans.hpp>
#include <evaluate.hpp>
#include <sec.hpp>
#include <nrd.hpp>
#include <gui.hpp>
#include <misc.hpp>

#ifdef UNIXWARE7
extern int toupper(int);
extern int tolower(int);
#endif
#ifdef SCO50
extern int toupper(int);
extern int tolower(int);
#endif
#ifdef SCO50P
extern int toupper(int);
extern int tolower(int);
#endif
#ifdef WIN32 
#include<winbase.h>
#endif

extern char *VENPMT_SIGNATURE_FILE,*VENPMT_PASSWORD;
extern char *PAYROLL_SIGNATURE_FILE,*PAYROLL_PASSWORD;
extern char *PURORD_SIGNATURE_FILE,*PURORD_PASSWORD;
extern int diagnostic_selected,diagnostic_read,diagnostic_total;
extern short suspend_diagnostic,abort_diagnostic;
extern short seconds_2_increment;
extern time_t dstart_time,delapsed_time,destimated_time,destimated_completion;
extern time_t dcurrent_time,dremaining_time,est_duration;
extern char *diagvariable;

struct PassKeys
{
	char *keyname;
	short fileno;
	short keyno;
	int numflds;
	NRDfield *fields;
};
typedef struct PassKeys PassKey;

struct filerangestruct
{
	char *filename;
	char *fieldname;
	short field_type;
	short field_len;
	short visible;
	char range;
	union rda_field_types from;
	union rda_field_types to;
};
typedef struct filerangestruct FileRangeStruct;

struct RangeScreens
{
	RDArsrc *rsrc;
	char *module;
	char *screen;
	int num;
	FileRangeStruct *frs;
};
typedef struct RangeScreens RangeScreen;

struct RDAbrowses 
{
	short fileno;
	short keyno;
	int num;
	void **key;
	char **string;
	void *keyvalue;
	void (*listfunc)(...);
};
typedef struct RDAbrowses RDAbrowse;


struct makebrowselists 
{
	short (*breakfunc)(...);
	short (*selectfunc)(...);
	short fileno;
	short keyno;
	RangeScreen *rs;
	RDArsrc *definelist;
	RDAbrowse *list;
	RDArsrc *mainrsrc;
	RDArsrc *searchrsrc;
	void (*initfunc)(...);
	void (*okfunction)(...);
	void (*new_record)(...);
	void (*doexit)(...);
	void *funcargs;
	short file_type;
	short quit_type;
	int start_where;
	int start_with;
	short modal_b;
	char *query;
	PassKey *passkey;
};
typedef struct makebrowselists MakeBrowseList;

struct browse_functions
{
	RDArsrc *mainrsrc;
	RDAbrowse *list;
	void (*okfunction)(...);
};
typedef struct browse_functions Browse_Function;

struct define_lists
{
	RDArsrc *definelist;
};
typedef struct define_lists Define_List;

struct browse_exits 
{
	void (*doexit)(...);
	short nofields;
	NRDfield *fields;
	short nokeys;
	NRDkey *keys;
	MakeBrowseList *b;
	Define_List *l;
	APPlib *keysavl;
	void *funcargs;
	Browse_Function *bf;
};
typedef struct browse_exits Browse_Exit;

/* WarningDialog Variables */
struct WarningDialogArgs
{
	RDArsrc *rsrc;
	void (*contfunc)(...);
	void (*cancfunc)(...);
	char type;
	void *arglist1;
	void *arglist2;
	void *arglist3;
};
typedef struct WarningDialogArgs WarningDialogArg;

struct PowerWriteFields
{
	char *to_name;
	short copy_type;
	char *from_name;
};
typedef struct PowerWriteFields PowerWriteField;

struct PowerWrites
{
	short fileno; /* Runtime - Actual fileno handle */
	char *module;
	char *filename;
	char *expression;
	int numflds;
	PowerWriteField *fields;
	char closefile; /* Runtime - used to denote when a special open was done to make the file available */
	char zerofile; /* Runtime - used to denote when it is necessary to ZERNRD() a file record in memory before loading vaules into its fields */
};
typedef struct PowerWrites PowerWrite;

struct RDAsearchs
{
	char *module;
	char *filename;
	int ctype;
	char *cmodule;
	char *cfilename;
	short num;
	RDAconfld *confld;
	char *subkeyname;
	short range;
	short num_writes;
	char *expression;
	short fileno;
	short keyno;
	short occurrence;
	short writable;
	char *skip_detail;
	char browse;
	char *button_name;
	char *browse_screen;
	char *define_list;
	char *search_browse;
	char smart_browse;
	MakeBrowseList *BL;
	void *parent;
	RDArsrc *parent_rsrc;
	short wc_eval;
	char *wc_expression;
	short wc_skip;
	char *skip_detail_error;
	char skip_widget;
	char key_fastbreak;
};
typedef struct RDAsearchs RDAsearch;

struct RDAsubords
{
	char *module;
	char *filename;
	int dtype;
	char *cmodule;
	char *cfilename;
	short num;
	RDAconfld *confld;
	char *subkeyname;
	char *expression;
	short fileno;
	short keyno;
	short occurrence;
	short writable;
	char UsePowerWrite;
	int numWrites;
	PowerWrite *Writes; 
};
typedef struct RDAsubords RDAsubord;

struct RDAaddsubs
{
	char *module;
	char *filename;
	int dtype;
	char *cmodule;
	char *cfilename;
	short num;
	RDAconfld *confld;
	char *subkeyname;
	char *expression;
	short fileno;
	short keyno;
	short occurrence;
	short writable;
	char browse;
	char *button_name;
	char *browse_screen;
	char *define_list;
	char *search_browse;
	char smart_browse;
	MakeBrowseList *BL;
	void *parent;
	RDArsrc *parent_rsrc;
	char skip_widget;
};
typedef struct RDAaddsubs RDAaddsub;

struct ButtonFields
{
	char *rname;
	char *ename;
};
typedef struct ButtonFields ButtonField;

struct MaintainButtons
{
	char *progname;
	char *listname;
	char *buttonname;
	char *modulename;
	char *filename;
	char *keyname;
	char *expression;
	short type;
	int num;
	ButtonField *bf;	
	char evalargs;
	APPlib *args;
	void *parent;
	void (*func)(...);
};
typedef struct MaintainButtons MaintainButton;

struct MaintainLists
{
	char *module;
	char *name;
};
typedef struct MaintainLists MaintainList;

struct Actionstructs
{
	char *to_name; /* resource the actions act upon */
	char *expression; /* Expression to set to to_name resource's value using compute to field type of the to_name resource accordingly..... */
	int typelist; /* 0-expression Only, 
			 1-Set Account Type, 
			 2-Set Definition Type
			 3-Set Account Type No Expression Used 
			 4-Set Definition Type No Expression Used */
};
typedef struct Actionstructs Actionstruct;

struct MTNActions
{
	char *name; /* name of resource that actions are associated with */
	int num; /* number of actions the callback for this resource will execute */
	Actionstruct *a; /* Array of ActionStructures to use when this resource is changed in it's callback */
};
typedef struct MTNActions MTNAction;

struct MaintainMasters
{
	char *module; /* module name process and screen belong */
	char *name; /* name of process */
	char *desc; /* description of master */
	int style; /* 0 - hard coded, 1 - utility master, 2 - special master */
	char *mainfile; /* filename this maintain master belongs */
	char *browse_screen; /* name of browse screen */
	char *define_list; /* name of define list screen */
	char *search_browse; /* name of search browse screen */
	char *maintain_screen; /* name of maintain master screen */
	int file_type; /* 0 - normal file, 1 - special (sort file) */ 
	int start_with; /* 0 - Maintain Screen, 1 - Browse Screen */
	int start_where; /* 0-top of browse, 1-bottom of browse */
	int num; /* number of include files */
	RDAsearch *s; /* include files */
	int subnum; /* number of subordinate files */
	RDAsubord *subord; /* subordinate files */
	PassKey *passkey; /* inherited information, key name and values */
	void *args;
	int numlists;
	MaintainList *lists;
	int save_type;
	char *save_expression;
	int numbuttons;
	MaintainButton *buttons;
	APPlib *runfunc;
	char *save_error;
	char *save_warning;
	char *save_error_desc;
	char *save_warning_desc;
	short inheritrsrcs;
	char UsePowerWrite;
	int numPrevWrites;
	PowerWrite *PrevWrites;
	int numNewWrites;
	PowerWrite *NewWrites;
	char Use_Audit_Trail;
	char *Audit_Trail;
	MakeBrowseList *mbl_utlmstr;
	char editable;
	int level;
	int WindowCount;
	void (*SubFunc)(...);
	void *SubFuncArgs;
	int numActions;
	MTNAction *Actions;
	RDArsrc *mtnrsrc;
	void *passparent;		/* used to pass around different values like, HoldReport structure for the embedded Process Exceptions Maintain Master. */
	short APPmainLoop;
};
typedef struct MaintainMasters MaintainMaster;

struct AddWriteFields
{
	char *to_name;
	short copy_type;
	char *from_name;
};
typedef struct AddWriteFields AddWriteField;

struct AddWrites
{
	short fileno;
	char *module;
	char *filename;
	char *expression;
	int numflds;
	AddWriteField *fields;
	char closefile;
	char zerofile;
};
typedef struct AddWrites AddWrite;

struct RDAeditflds
{
	char *fldname;
	char *conname;
	short type;
	unsigned len;
	union rda_field_types data;
	unsigned dlen;
};
typedef struct RDAeditflds RDAeditfld;

struct AddEditWdgts
{
	char *name;
	short type;
	unsigned len;
	union rda_field_types data;
	unsigned dlen;
	int etype;
	char *expression;
	char id_mgt;
	char wrote_id;
	char *module;
	char *filename;
	char *subkeyname;
	short num;
	RDAeditfld *confld;
	char *mgt_expression;
	char *assignname;
	char *id;
	char *override_expression;
	short fileno;
	short wcount;
	void *parent;
	char setaccttype;
	APPlib *acctrsc;
	char setdeftype;
	APPlib *defrsc;
	char loadvirtual;
};
typedef struct AddEditWdgts AddEditWdgt;

struct PowerAdds 
{
	char *module; /* module name process and screen belong */
	char *name; /* name of process */
	char *desc; /* description of Add Process */
	char *define_list; /* name of define list screen */
	char *addscreen; /* name of add screen */
	char *reportname; /* Audit Trail Report Name */
	int subnum; /* number of subordinate files */
	RDAaddsub *subord; /* subordinate files */
	void *args;
	char *save_expression;
	int numbuttons;
	MaintainButton *buttons;
	int save_type; 
			/* 0 - Power Save (Power Save will use the tmprsrc and
			       seperately gather the required RDAvirtuals), 
			   1 - Fast Save will add the write files to the 
			       addDFincvir list of the Power Add screen and 
			       not make a tmprsrc when saving */
	char *save_error; /* this might be redundant and not used 
				it uses the save_expression above */
	char *save_warning;
	char *save_error_desc;
	char *save_warning_desc;
	char *quit_error;
	char *quit_warning;
	char *quit_error_desc;
	char *quit_warning_desc;
	int numWrites;
	AddWrite *Writes;
	int numwdgts;
	AddEditWdgt *EditWdgts;
	void (*SubFunc)(...);
	void *SubFuncArgs;
	RDArsrc *addrsrc;
	void *passable;
	int level;
	APPlib *PREVIOUSADD;
	RDArsrc *ADD_DEFINE_LIST;
	long WRITE_COUNTER;
	short APPmainLoop;
	int AddType;
	void *TableWidget;
	int table_row;
	int table_col;
	char UsingRow[101];
};
typedef struct PowerAdds PowerAdd;

struct RDADosCopys
{
	char *device_spec;
	char *from_filename;
	char *to_filename;
	void (*quitfunc)(...);
	void *quitfuncargs;
	RDArsrc *doscopyrsrc;
	RDArsrc *parentrsrc;
};
typedef struct RDADosCopys RDADosCopy;

struct RDApvars
{
	char *name;
	char *command;
	short chgsize;
	short pitch;
	short points;
	int type;
};
typedef struct RDApvars RDApvar;
struct RDApdevs
{
	char *name;
	char formfeed;
	char line_58;
	char active;
	double page_length;
	int num;
	RDApvar *pvar;
};
typedef struct RDApdevs RDApdev;

struct MTNPassableStructs
{
	RDArsrc *rsrc;
	MaintainMaster *MTNMASTER;
	RDATData *previous;
	void (*quitfunc)(...);
	RDArsrc *fromrsrc;
	void *fromargs;
	void *args;
};
typedef struct MTNPassableStructs MTNPassableStruct;


#define PowerExecuteListFunc(a,b,c,d)   xPowerExecuteListFunc(a,b,c,d,__LINE__,__FILE__)
void xPowerExecuteListFunc(RDArsrc *,int,void *,MTNPassableStruct *,int,char *);


#define MakePowerMasterButtons(a,b,c)     xMakePowerMasterButtons(a,b,c,__LINE__,__FILE__)
void xMakePowerMasterButtons(RDArsrc *,MTNPassableStruct *,void (*)(...),int,char *);


void xWARNINGDIALOG(char *,char *,char *,void (*contfunc)(...),void (*cancfunc)(...),char,char,void *,void *,void *,int,char *);
#define WARNINGDIALOG(s,h,m,f,c,i,x) xWARNINGDIALOG(s,h,m,f,c,i,1,x,NULL,NULL,__LINE__,__FILE__)
#define ADVWARNINGDIALOG(s,h,m,f,c,i,t,x,y,z) xWARNINGDIALOG(s,h,m,f,c,i,t,x,y,z,__LINE__,__FILE__)
/* WARNINGDIALOG creates a warning dialog, arguments are: warning name,
        warning message, function to run when continue button is precessed,
	a function to run when cancel button is pressed,
        ,either TRUE or FALSE to not place a Cancel Button in the window,
	,number of arguments (arg_type) to pass to the two functions
        ,void *arglist1 for passing of argument 1 to the function 
        ,void *arglist2 for passing of argument 2 to the function 
        and void *arglist3 for passing of argument 3 to the function */
#define InitializeSubsystems(a,b,x,y)	xInitializeSubsystems(a,b,x,y,__LINE__,__FILE__)
/* InitializeSubsystems, Initializes Help Gui Database Security and 
	Transaction Processing Subsytems */
short xInitializeSubsystems(int,char **,char *,char *,int,char *);
#define ShutdownSubsystems()	xShutdownSubsystems(__LINE__,__FILE__)
/* ShutdownSubsystems Stops the Help Gui Database Security and Transaction 
	Processing Subsystems */
void xShutdownSubsystems(int,char *);
char *gfname(char *,char *);
short xisfilersrc(short,char *,char *,short);
#define fields2file(x,y)	{xfields2file(x,y,__LINE__,__FILE__);y=NULL;}
short xfields2file(short,NRDfield *,int,char *);
#define Rfreetmpfld(x,y)	{xRfreetmpfld(x,y,__LINE__,__FILE__);x=NULL;}
short xRfreetmpfld(NRDfield *,short,int,char *);
#define readsinglefilewidgets(x,y)	xreadfilewidgets(x,y,1,__LINE__,__FILE__)
#define readfilewidgets(x,y)	xreadfilewidgets(x,y,0,__LINE__,__FILE__)
void xreadfilewidgets(short,RDArsrc *,short,int,char *);
/* xreadfilewidgets reads all resources on a screen that come from the
  file's fields*/
#define singleupdatefilersrcs(x,y)	xupdateeditfilersrcs(x,1,y,1,0,1,0,1,__LINE__,__FILE__)
#define updatefilersrcs(x,y) xupdateeditfilersrcs(x,1,y,0,0,1,1,__LINE__,__FILE__)
#define editsinglefilersrcsnokeys(x,y,z,e) xupdateeditfilersrcs(x,y,z,1,1,e,0,__LINE__,__FILE__)
#define updateeditfilersrcsnokeys(x,y,z,e) xupdateeditfilersrcs(x,y,z,0,1,e,0,__LINE__,__FILE__)
#define updatesingleeditfilersrcs(x,y,e) xupdateeditfilersrcs(x,1,y,1,1,e,1,__LINE__,__FILE__)
#define updateeditfilersrcs(x,y,e) xupdateeditfilersrcs(x,1,y,0,1,e,1,__LINE__,__FILE__)
void xupdateeditfilersrcs(short,short,RDArsrc *,short,short,short,short,int,char *);
/* xupdateallfilewidgets updates all resources on a screen that come from the
  file's fields*/
#define singlefile2rsrc(x,y,z)	xfile2rsrc(x,y,z,1,NULL,NULL,__LINE__,__FILE__)
#define ADVsinglefile2rsrc(x,y,z,a,b)	xfile2rsrc(x,y,z,1,a,b,__LINE__,__FILE__)
#define file2rsrc(x,y,z)	xfile2rsrc(x,y,z,0,NULL,NULL,__LINE__,__FILE__)
#define ADVfile2rsrc(x,y,z,a,b)	xfile2rsrc(x,y,z,0,a,b,__LINE__,__FILE__)
/* file2rsrc Creates a resource for each element (field) contained in the 
	specified file */
short xfile2rsrc(short,RDArsrc *,char,short,void (*)(...),void *,int,char *);
#define singlefile2rangersrc(x,y)	xfile2rangersrc(x,y,1,__LINE__,__FILE__);
#define file2rangersrc(x,y)	xfile2rangersrc(x,y,0,__LINE__,__FILE__)
/* file2rangersrc Creates a set of resources used for ranging (querying) upon 
	elements (fields) within the specified file */
short xfile2rangersrc(short,RDArsrc *,short,int,char *);
#define ADVfile2range_editableandsensitive(a,b,c,d,e)	xfile2range_editableandsensitive(a,b,c,d,e,__LINE__,__FILE__)
#define singlefile2range_editableandsensitive(a,b,c,d)	xfile2range_editableandsensitive(a,b,TRUE,c,d,__LINE__,__FILE__)
#define file2range_editableandsensitive(a,b,c,d)	xfile2range_editableandsensitive(a,b,FALSE,c,d,__LINE__,__FILE__)
short xfile2range_editableandsensitive(short,RDArsrc *,short,short,short,int,char *);
#define singlefilerecord2rsrc(x,y)	xfilerecord2rsrc(x,y,1,__LINE__,__FILE__)
#define filerecord2rsrc(x,y)	xfilerecord2rsrc(x,y,0,__LINE__,__FILE__)
/* fiilerecord2rsrc Copies the contents of the current record to the 
	previously created resource for each element in a specified record */
void xfilerecord2rsrc(short,RDArsrc *,short,int,char *);
#define rsrc2singlefilerecord(x,y)	xrsrc2filerecord(x,y,1,__LINE__,__FILE__)
#define Actionrsrc2singlefilerecord(x,y,a,b,c)	xActionrsrc2filerecord(x,y,1,a,b,c,__LINE__,__FILE__)
#define rsrc2filerecord(x,y)	xrsrc2filerecord(x,y,0,__LINE__,__FILE__)
#define Actionrsrc2filerecord(x,y,a,b,c)	xActionrsrc2filerecord(x,y,0,a,b,c,__LINE__,__FILE__)
/* rsrc2filerecord Copies the contents (values) of the resources corresponding 
	to each element in the specified record */
void xrsrc2filerecord(short,RDArsrc *,short,int,char *);
void xActionrsrc2filerecord(short,RDArmem *,short,MaintainMaster *,void (*)(...),void *,int,char *);
#define ADVPbrowse(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)	xbrowse(a,b,c,d,e,PASSKEYinitfunc,PASSKEYselectfunction,NULL,f,g,h,i,j,k,l,m,n,o,p,0,1,__LINE__,__FILE__)
#define START_ADVPbrowse(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q)	xbrowse(a,b,c,d,e,PASSKEYinitfunc,PASSKEYselectfunction,NULL,f,g,h,i,j,k,l,m,n,o,p,q,1,__LINE__,__FILE__)
#define START_ADV2Pbrowse(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r)	xbrowse(a,b,c,d,e,PASSKEYinitfunc,PASSKEYselectfunction,NULL,f,g,h,i,j,k,l,m,n,o,p,q,r,__LINE__,__FILE__)
#ifdef XXX

ADVPbrowse makes xbrowse work like it was the following function:

MakeBrowseList *ADVPbrowse(short fileno,short keyno,RDArsrc *mainrsrc,
	RDArsrc *searchrsrc,RDArsrc *definelist,void (*doexit)(...),
	void (*okfunction)(...),void (*new_record)(...),void *funcargs,short modal_b,
	void (*listfunc)(...),short add_type,short select_type,short quit_type,
	short file_type,PassKey *passkey) 

#endif

#define ADVbrowse(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q)	xbrowse(a,b,c,d,e,f,g,NULL,h,i,j,k,l,m,n,o,p,q,NULL,0,1,__LINE__,__FILE__)
#define START_ADVbrowse(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r)	xbrowse(a,b,c,d,e,f,g,NULL,h,i,j,k,l,m,n,o,p,q,NULL,r,1,__LINE__,__FILE__)
#define START_ADV2browse(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s)	xbrowse(a,b,c,d,e,f,g,NULL,h,i,j,k,l,m,n,o,p,q,NULL,r,s,__LINE__,__FILE__)

#define ADVSbrowse(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r)	xbrowse(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,NULL,0,1,__LINE__,__FILE__)
#define START_ADVSbrowse(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s)	xbrowse(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,NULL,s,1,__LINE__,__FILE__)
#define START_ADV2Sbrowse(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t)	xbrowse(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,NULL,s,t,__LINE__,__FILE__)
#ifdef XXX

ADVbrowse makes xbrowse work like it was the following function:

MakeBrowseList *ADVbrowse(short fileno,short keyno,RDArsrc *mainrsrc,
	RDArsrc *searchrsrc,RDArsrc *definelist,void (*initfunc)(...),
	short (*breakfunc)(...),void (*doexit)(...),void (*okfunction)(...),
	void (*new_record)(...),void *funcargs,short modal_b,
	void (*listfunc)(...),short add_type,short select_type,short quit_type,
	short file_type) 

ADVSbrowse makes xbrowse work like it was the following function:

MakeBrowseList *ADVSbrowse(short fileno,short keyno,RDArsrc *mainrsrc,
	RDArsrc *searchrsrc,RDArsrc *definelist,void (*initfunc)(...),
	short (*breakfunc)(...),short (*selectfunc)(...),
	void (*doexit)(...),void (*okfunction)(...),
	void (*new_record)(...),void *funcargs,short modal_b,
	void (*listfunc)(...),short add_type,short select_type,short quit_type,
	short file_type) 
#endif
#define browse(a,b,c,d,e,f,g,h,i,j,k)	xbrowse(a,b,c,d,e,f,g,NULL,h,i,j,k,FALSE,NULL,0,0,2,0,NULL,0,1,__LINE__,__FILE__)

#define Sbrowse(a,b,c,d,e,f,g,h,i,j,k,l)	xSbrowse(a,b,c,d,e,f,g,h,i,j,k,l,FALSE,NULL,0,0,2,0,NULL,__LINE__,__FILE__)
#ifdef XXX

browse makes xbrowse work like it was written using the following:

MakeBrowseList *browse(short fileno,short keyno,RDArsrc *mainrsrc,
	RDArsrc *searchrsrc,RDArsrc *definelist,void (*initfunc)(...),
	short (*breakfunc)(...),short (*selectfunc)(...),void (*doexit)(...),
	void (*okfunction)(...),void (*new_record)(...),void *funcargs)

browse makes xSbrowse work like it was written using the following:

MakeBrowseList *Sbrowse(short fileno,short keyno,RDArsrc *mainrsrc,
	RDArsrc *searchrsrc,RDArsrc *definelist,void (*initfunc)(...),
	short (*breakfunc)(...),short (*selectfunc)(...),void (*doexit)(...),
	void (*okfunction)(...),void (*new_record)(...),void *funcargs)
#endif
/* browse - Creates a browse on the specified data file */
MakeBrowseList *xbrowse(short,short,RDArsrc *,RDArsrc *,RDArsrc *,void (*)(...),
	short (*)(...),short (*)(...),void (*)(...),void (*)(...),void (*)(...),void *,
	short,void (*)(...),short,short,short,short,PassKey *,int,int,int,char *);

#define getfilerangedesc(a,b)	xgetfilerangedesc(a,b,__LINE__,__FILE__)
/* getfilerangedesc - Returns a char * describing the values specified in the 
	file2rangersrc resources */
char *xgetfilerangedesc(short,RangeScreen *,int,char *);
#define testfilerangersrc(a,b)	xtestfilerangersrc(a,b,__LINE__,__FILE__)
/* testfilerangersrc - Tests the current record for compliance to a range 
	specified in the file2rangersrc resources for the specified file */
short xtestfilerangersrc(short,RangeScreen *,int,char *);
#define FreeFileRangeStruct(a)	{xFreeFileRangeStruct(a,__LINE__,__FILE__);a=NULL;}
void xFreeFileRangeStruct(FileRangeStruct *,int,char *);
APPlib *mk_define_list(RDArsrc *,short,NRDfield *,short,NRDkey *,RDArsrc *,short);
#define makefieldrsrc(a,b,c,d,e)	xmakefieldrsrc(a,b,c,d,e,FALSE)
#define RMEMmakefieldrsrc(a,b,c,d,e)	xmakefieldrsrc(a,b,c,d,e,TRUE)
void xmakefieldrsrc(RDArsrc *,char *,short,unsigned,char,short);
short alreadyrsrc(char *,RDArsrc *);
#define setfile2rsrccallbacks(a,b,c)	xsetfile2rsrccallbacks(a,b,c,__LINE__,__FILE__)
void xsetfile2rsrccallbacks(short,RDArsrc *,short,int,char *);
void setdeftypefrom(RDArsrc *,RDArmem *);
void setdeftypeto(RDArsrc *,RDArmem *);
void setacctypefrom(RDArsrc *,RDArmem *);
void setacctypeto(RDArsrc *,RDArmem *);
void setdeftype(RDArmem *,short);
void setacctype(RDArmem *,short);
RangeScreen *RangeScreenNew(RDArsrc *);
#define FreeRangeScreen(a)	{xFreeRangeScreen(a,__LINE__,__FILE__);a=NULL;}
void xFreeRangeScreen(RangeScreen *,int,char *);
#define ADVReadRangeScreen(a,b,c)	xReadRangeScreen(a,b,c,__LINE__,__FILE__)
#define ReadSingleFileRangeScreen(a,b)	xReadRangeScreen(a,b,1,__LINE__,__FILE__)
#define ReadRangeScreen(a,b)	xReadRangeScreen(a,b,0,__LINE__,__FILE__)
void xReadRangeScreen(short,RangeScreen *,short,int,char *);
short update_diagnostic(RDArsrc *,short);
void resumediagnostic(RDArsrc *);
void suspenddiagnostic(RDArsrc *);
void stop_diagnostic(RDArsrc *);
#define diagscrn(a,b,c,d,e)	xdiagscrn(a,b,c,d,e,0);
#define odiagscrn(a,b,c,d,e)	xdiagscrn((-1),a,b,c,d,e);
RDArsrc *xdiagscrn(short,char *,char *,char *,void (*)(...),int);
#define makebrowselist(x)	xmakebrowselist(x,NULL)
void xmakebrowselist(MakeBrowseList *,RDArsrc *);
#define virtual2rangersrc(a,b)	xvirtual2rangersrc(a,b,__LINE__,__FILE__)
short xvirtual2rangersrc(RDAvirtual *,RDArsrc *,int,char *);
void ReadVirtualRangeStruct(RDAvirtual *,RangeScreen *);
#define ADVtestvirtualrangersrc(a,b,c,d)	xtestvirtualrangersrc(a,b,c,d,__LINE__,__FILE__)
#define testvirtualrangersrc(a,b)	xtestvirtualrangersrc(a,b,NULL,NULL,__LINE__,__FILE__)
short xtestvirtualrangersrc(RDAvirtual *,RangeScreen *,void (*)(...),void *,int,char *);
#define RETURN_BROWSE_EXIT(a)	xRETURN_BROWSE_EXIT(a,__LINE__,__FILE__)
Browse_Exit *xRETURN_BROWSE_EXIT(MakeBrowseList *,int,char *);
#define FREE_BROWSE_EXIT(a)	{xFREE_BROWSE_EXIT(a,__LINE__,__FILE__);a=NULL;}
short xFREE_BROWSE_EXIT(MakeBrowseList *,int,char *);
void SaveDefaultsUsedOnly(RDArsrc *);
#define unique_name(...)	xunique_name(NULL,__LINE__,__FILE__)
#define ADVunique_name(a)	xunique_name(a,__LINE__,__FILE__)
char *xunique_name(char *,int,char *);
#define MakeNRDlist(a,b,c,d,e,f)	xMakeNRDlist(a,b,c,d,e,f,NULL,NULL,__LINE__,__FILE__)
#define ADVMakeNRDlist(a,b,c,d,e,f,g,h)	xMakeNRDlist(a,b,c,d,e,f,g,h,__LINE__,__FILE__)
APPlib *xMakeNRDlist(short,char *,char *,void (*)(short,APPlib *),short,
	void (*)(...),void *,int,char *);
/* builds a list from a nrdsec file. arguements: file number, module name,
   process description, sub-function, key number to use.  Sub function should
   accept a short (file number), and a APPlib * (list to add to).*/
#define MakeNRDlistAllSTRING(a,b,c,d,e)	xMakeNRDlistAllSTRING(a,b,c,d,e,NULL,NULL,__LINE__,__FILE__)
#define ADVMakeNRDlistAllSTRING(a,b,c,d,e,f,g)	xMakeNRDlistAllSTRING(a,b,c,d,e,f,g,__LINE__,__FILE__)
APPlib *xMakeNRDlistAllSTRING(short,char *,char *,short,char *,void (*)(...),void *,
	int,char *);
/* biulds a list from a nrdsec file. arguements: file number, module name,
	process description, key number to use, field name.*/
int FINDFLDLISTELEMENT(short,char *,APPlib *,char *);
     /* checks a list passed to it for a string value, if not found a warning 
        dialog is poped up for the user to know that an invalid value is 
        stored in the record.  arguments:  file number(supporing file with 
        values on list), fieldname(in main file), list, value.*/
#define APPOPNNRDEDITABLE(a,b,c)	xAPPOPNNRDEDITABLE(a,b,c,__LINE__,__FILE__)
short xAPPOPNNRDEDITABLE(char *,char *,char *,int,char *);
#define APPOPNNRD(a,b,c,d)	xAPPOPNNRD(a,b,c,d,__LINE__,__FILE__)
short xAPPOPNNRD(char *,char *,short,short,int,char *);
#define APPmakescrn(a,b,c,d,e)	xAPPmakescrn(a,b,c,d,e,__LINE__,__FILE__) 
short xAPPmakescrn(RDArsrc *,short,void (*)(...),void *,short,int,char *);
#define nonlstmakefld(a,b,c,d)	xnonlstmakefld(a,b,c,d,FALSE,__LINE__,__FILE__)
#define ADVnonlstmakefld(a,b,c,d,e)	xnonlstmakefld(a,b,c,d,e,__LINE__,__FILE__)
void xnonlstmakefld(RDArsrc *,short,NRDfield *,short,short,int,char *);
/* adds field to screen resources for fields that are not scrolled lists.
	arguements: screen resource, file number, field pointer(fldx), editable.
	editable can be a short or char, compiler will convert to short*/
#define updatebrowse(a,b,c,d,e)	xupdatebrowse(a,b,c,d,e,__LINE__,__FILE__)
void xupdatebrowse(short,MakeBrowseList *,short,char *,RDArsrc *,int,char *);
/* update's the browse list for maintains. Arguements: update_list, mbl pointer,
	file number, module name, RDArsrc for maintain screen.*/

#define COMPARE_RSRCS(a,b,c)	xCOMPARE_RSRCS(a,b,c,__LINE__,__FILE__)
short xCOMPARE_RSRCS(RDArsrc *,RDAdefault *,short,int,char *);
#define updatefilersrc(a,b)	xupdatefilersrc(a,b,0,__LINE__,__FILE__)
#define updatesinglefilersrc(a,b)	xupdatefilersrc(a,b,1,__LINE__,__FILE__)
void xupdatefilersrc(short,RDArsrc *,short,int,char *);
#define seteditfilersrc(a,b,c,d,e)	xseteditfilersrc(a,b,c,0,d,e,__LINE__,__FILE__)
#define seteditsinglefilersrc(a,b,c,d,e)	xseteditfilersrc(a,b,c,1,d,e,__LINE__,__FILE__)
void xseteditfilersrc(short,short,RDArsrc *,short,short,short,int,char *);
#define updatesinglerecord2rsrc(x,y)	xupdatefilerecord2rsrc(x,y,1,__LINE__,__FILE__)
#define updatefilerecord2rsrc(x,y)	xupdatefilerecord2rsrc(x,y,0,__LINE__,__FILE__)
/* updatefilerecord2rsrc Copies and updates the contents of the current record
	to the previously created resource for each element in a specified record */
void xupdatefilerecord2rsrc(short,RDArsrc *,short,int,char *);
void AddFileFields(APPlib *,short);

#define PassKeyNew()	xPassKeyNew(__LINE__,__FILE__)
PassKey *xPassKeyNew(int,char *);
#define FreePassKey(a)	{xFreePassKey(a,__LINE__,__FILE__);a=NULL;}
void xFreePassKey(PassKey *,int,char *);
#define ApplyPassKey(a)	xApplyPassKey(a,__LINE__,__FILE__)
void xApplyPassKey(PassKey *,int,char *);
#define addPassKeyField(a,b)	xaddPassKeyField(a,b,__LINE__,__FILE__)
short xaddPassKeyField(PassKey *,NRDfield *,int,char *);
#define READPassKey(a)	xREADPassKey(a,__LINE__,__FILE__)
PassKey *xREADPassKey(short,int,char *);
void PASSKEYinitfunc(MakeBrowseList *);
short PASSKEYselectfunction(MakeBrowseList *);
#define ApplySingleFilePassKey2Rsrc(a,b)	xApplyPassKey2Rsrc(a,b,TRUE,__LINE__,__FILE__)
#define ApplyPassKey2Rsrc(a,b)	xApplyPassKey2Rsrc(a,b,FALSE,__LINE__,__FILE__)
void xApplyPassKey2Rsrc(RDArsrc *,PassKey *,short,int,char *);
#define File2PassKeyAPPlib(a,b,c)	xFile2PassKeyAPPlib(a,b,c,__LINE__,__FILE__)
APPlib *xFile2PassKeyAPPlib(char *,short,short,int,char *);
#define SingleFileRsrc2PassKeyApplib(a,b,c,d)	xRsrc2PassKeyAPPlib(a,b,c,d,TRUE,__LINE__,__FILE__)
#define Rsrc2PassKeyAPPlib(a,b,c,d)	xRsrc2PassKeyAPPlib(a,b,c,d,FALSE,__LINE__,__FILE__)
#define ADVRsrc2PassKeyAPPlib(a,b,c,d,e)	xRsrc2PassKeyAPPlib(a,b,c,d,e,__LINE__,__FILE__)
APPlib *xRsrc2PassKeyAPPlib(RDArsrc *,char *,short,short,short,int,char *);
#define MTNSCRN_EXECUTEPROGRAM(a,b,c,d,e,f)	xMTNSCRN_EXECUTEPROGRAM(a,b,c,d,e,FALSE,f,NULL,__LINE__,__FILE__)
#define SINGLEFILEMTNSCRN_EXECUTEPROGRAM(a,b,c,d,e,f)	xMTNSCRN_EXECUTEPROGRAM(a,b,c,d,e,TRUE,f,NULL,__LINE__,__FILE__)
#define ADVMTNSCRN_EXECUTEPROGRAM(a,b,c,d,e,f,g,h)	xMTNSCRN_EXECUTEPROGRAM(a,b,c,d,e,f,g,h,__LINE__,__FILE__)
void xMTNSCRN_EXECUTEPROGRAM(char *,RDArsrc *,char *,short,short,short,APPlib *,void (*)(...),int,char *);
#define BROWSESCRN_EXECUTEPROGRAM(a,b,c,d,e,f)	xBROWSESCRN_EXECUTEPROGRAM(a,b,c,d,e,FALSE,f,NULL,__LINE__,__FILE__)
#define SINGLEFILEBROWSESCRN_EXECUTEPROGRAM(a,b,c,d,e,f)	xBROWSESCRN_EXECUTEPROGRAM(a,b,c,d,e,TRUE,f,NULL,__LINE__,__FILE__)
#define ADVBROWSESCRN_EXECUTEPROGRAM(a,b,c,d,e,f,g,h)	xBROWSESCRN_EXECUTEPROGRAM(a,b,c,d,e,f,g,h,__LINE__,__FILE__)
void xBROWSESCRN_EXECUTEPROGRAM(char *,void *,char *,short,short,short,APPlib *,void (*)(...),int,char *);

#define RDAScrolledListNew(a,b)	xRDAScrolledListNew(a,b,__LINE__,__FILE__)
RDAScrolledList *xRDAScrolledListNew(char *,char *,int,char *);
#define getScrolledListbin(a,b)	xgetScrolledListbin(a,b,__LINE__,__FILE__)
short xgetScrolledListbin(char *,RDAScrolledList *,int,char *);
#define getScrolledListdesc(m,n) xgetScrolledListdesc(m,n,__LINE__,__FILE__)
char *xgetScrolledListdesc(char *,char *,int,char *);
#define writeScrolledListbin(a,b)	xwriteScrolledListbin(a,b,__LINE__,__FILE__)
short xwriteScrolledListbin(char *,RDAScrolledList *,int,char *);

#define MaintainMasterNew(a,b)	xMaintainMasterNew(a,b,__LINE__,__FILE__)
MaintainMaster *xMaintainMasterNew(char *,char *,int line,char *file);
#define FreeMaintainMaster(a)	{xFreeMaintainMaster(a,__LINE__,__FILE__);a=NULL;}
void xFreeMaintainMaster(MaintainMaster *,int,char *);
#define getMaintainMaster(a,b)	xgetMaintainMaster(a,b,NULL,__LINE__,__FILE__)
#define getMaintainMasterADV(a,b,c)	xgetMaintainMaster(a,b,c,__LINE__,__FILE__)
short xgetMaintainMaster(char *,MaintainMaster *,BIN **,int,char *);
#define writeMaintainMaster(a,b)	xwriteMaintainMaster(a,b,NULL,__LINE__,__FILE__)
#define writeMaintainMasterADV(a,b,c)	xwriteMaintainMaster(a,b,c,__LINE__,__FILE__)
short xwriteMaintainMaster(char *,MaintainMaster *,BIN **,int,char *);
#define getMaintainMasterdesc(a,b)	xgetMaintainMasterdesc(a,b,__LINE__,__FILE__)
char *xgetMaintainMasterdesc(char *,char *,int,char *);
#define QUITvfy(a,b)	xQUITvfy(a,b,__LINE__,__FILE__)
short xQUITvfy(short,RDATData *,int,char *);
#define QUITvfydesc(a,b)	xQUITvfydesc(a,b,__LINE__,__FILE__)
char *xQUITvfydesc(short,RDATData *,int,char *);
#define findMaintainMaster(a)	xfindMaintainMaster(a,__LINE__,__FILE__)
short xfindMaintainMaster(MaintainMaster *,int,char *);
#define MakeRunFunction(a,b)	xMakeRunFunction(a,b,__LINE__,__FILE__)
void xMakeRunFunction(RDArsrc *,MaintainMaster *,int,char *);
#define MakeMasterButtons(a,b)	xMakeMasterButtons(a,b,__LINE__,__FILE__)
void xMakeMasterButtons(RDArsrc *,MaintainMaster *,int,char *);
void MasterButtonFunction(RDArsrc *,MaintainButton *);
#define MaintainButton2PassKey(a,b)	xMaintainButton2PassKey(a,b,__LINE__,__FILE__)
APPlib *xMaintainButton2PassKey(MaintainButton *,short,int,char *);
#define MaintainButton2ClearPassKey(a,b)	xMaintainButton2ClearPassKey(a,b,__LINE__,__FILE__)
APPlib *xMaintainButton2ClearPassKey(MaintainButton *,short,int,char *);
#define ExecuteListFunc(a,b,c)	xExecuteListFunc(a,b,c,__LINE__,__FILE__)
void xExecuteListFunc(int,void *,MaintainMaster *,int,char *);
int xGETCURRENTYEAR(int,int,char *);
#define GETCURRENTCALENDARYEAR() xGETCURRENTYEAR((-1),__LINE__,__FILE__)
#define GETCURRENTFISCALYEAR(a) xGETCURRENTYEAR(a,__LINE__,__FILE__)
#define OPEN_MASTER_FILES(a)	xOPEN_MASTER_FILES(a,__LINE__,__FILE__)
short xOPEN_MASTER_FILES(MaintainMaster *,int,char *);
#define CLOSE_MASTER_FILES(a)	xCLOSE_MASTER_FILES(a,__LINE__,__FILE__)
void xCLOSE_MASTER_FILES(MaintainMaster *,int,char *);
#define SetRDArsrcFiles(a,b)	xSetRDArsrcFiles(a,b,__LINE__,__FILE__)
void xSetRDArsrcFiles(short,RDArsrc *,int,char *);
#define GET_BROWSE_SUPPORTING(b,s,d,m,f,a) xGET_BROWSE_SUPPORTING(b,s,d,m,f,a,__LINE__,__FILE__)
void xGET_BROWSE_SUPPORTING(RDArsrc *,RDArsrc *,RDArsrc *,MaintainMaster *,void (*)(...),void *,int,char *);
#define GET_MAINTAIN_INCVARS(r,m) xGET_MAINTAIN_INCVARS(r,m,__LINE__,__FILE__)
void xGET_MAINTAIN_INCVARS(RDArsrc *,MaintainMaster *,int,char *);
#define DELETE_SUBORDINATES(r,m,f,a) xDELETE_SUBORDINATES(r,m,f,a,__LINE__,__FILE__)
char xDELETE_SUBORDINATES(RDArsrc *,MaintainMaster *,void (*)(...),void *,int,char *);
#define SetSupportingFiles(r,m,f,a,u,d) xSetSupportingFiles(r,m,f,a,u,d,__LINE__,__FILE__)
char xSetSupportingFiles(RDArsrc *,MaintainMaster *,void (*)(...),void *,short,short,int,char *);
#define SAVE_CHECK(r,m,f,a) xSAVE_CHECK(r,m,f,a,__LINE__,__FILE__)
char xSAVE_CHECK(RDArsrc *,MaintainMaster *,void (*)(...),void *,int,char *);
#define GET_SUPPORTING(r,m,s,a,u,d) xGET_SUPPORTING(r,m,s,a,u,d,__LINE__,__FILE__)
void xGET_SUPPORTING(RDArsrc *,MaintainMaster *,void (*)(...),void *,short,short,int,char *);
#define GET_SUPPORTING_NONSAVE(r,m,s,a,u,d) xGET_SUPPORTING_NONSAVE(r,m,s,a,u,d,__LINE__,__FILE__)
void xGET_SUPPORTING_NONSAVE(RDArsrc *,MaintainMaster *,void (*)(...),void *,short,short,int,char *);
#define SET_MTNMASTER(r,m,s,a) xSET_MTNMASTER(r,m,s,a,__LINE__,__FILE__)
#define RMEMSET_MASTER(r,m,s,a) xRMEMSET_MASTER(r,m,s,a,__LINE__,__FILE__)
void xSET_MTNMASTER(RDArsrc *,MaintainMaster *,void (*)(...),void *,int,char *);
void xRMEMSET_MASTER(RDArmem *,MaintainMaster *,void (*)(...),void *,int,char *);
#define SET_MAINTAIN_FILE2RSRCS(r,m) xSET_MAINTAIN_FILE2RSRCS(r,m,__LINE__,__FILE__)
void xSET_MAINTAIN_FILE2RSRCS(RDArsrc *,MaintainMaster *,int,char *);
#define SET_KEYFUNC(r,m,p,f,a,i) xSET_KEYFUNC(r,m,p,f,a,i,NULL,NULL,NULL,NULL,NULL,NULL,__LINE__,__FILE__)
#define RMEMSET_KEYFUNC(r,m,p,f,a,i) xRMEMSET_KEYFUNC(r,m,p,f,a,i,NULL,NULL,NULL,NULL,NULL,NULL,__LINE__,__FILE__)
#define ADVSET_KEYFUNC(r,m,p,f,a,i,n)	xSET_KEYFUNC(r,m,p,f,a,i,n,NULL,NULL,NULL,NULL,NULL,NULL,__LINE__,__FILE__)
#define ADVRMEMSET_KEYFUNC(r,m,p,f,a,i,n)	xRMEMSET_KEYFUNC(r,m,p,f,a,i,n,NULL,NULL,NULL,NULL,NULL,NULL,__LINE__,__FILE__)
#define ADV2SET_KEYFUNC(r,m,p,f,a,i,n,e,z)	xSET_KEYFUNC(r,m,p,f,a,i,n,z,NULL,NULL,e,z,__LINE__,__FILE__)
#define ADV2RMEMSET_KEYFUNC(r,m,p,f,a,i,n,e,z)	xRMEMSET_KEYFUNC(r,m,p,f,a,i,n,z,NULL,NULL,e,z,__LINE__,__FILE__)
#define ADV3SET_KEYFUNC(r,m,p,f,a,i,n,x,b,y,e,z)	xSET_KEYFUNC(r,m,p,f,a,i,n,x,b,y,e,z,__LINE__,__FILE__)
#define ADV3RMEMSET_KEYFUNC(r,m,p,f,a,i,n,x,b,y,e,z)	xRMEMSET_KEYFUNC(r,m,p,f,a,i,n,x,b,y,e,z,__LINE__,__FILE__)
#define ADV4SET_KEYFUNC(r,m,p,f,a,n,x,b,y,e,z)	xSET_KEYFUNC(r,m,p,f,a,m->inheritrsrcs,n,x,b,y,e,z,__LINE__,__FILE__)
#define ADV4RMEMSET_KEYFUNC(r,m,p,f,a,n,x,b,y,e,z)	xRMEMSET_KEYFUNC(r,m,p,f,a,m->inheritrsrcs,n,x,b,y,e,z,__LINE__,__FILE__)
void xSET_KEYFUNC(RDArsrc *,MaintainMaster *,RDATData *,void (*)(...),void *,short,void (*)(...),void *,void (*)(...),void *,void (*)(...),void *,int,char *);
void xRMEMSET_KEYFUNC(RDArmem *,MaintainMaster *,RDATData *,void (*)(...),void *,short,void (*)(...),void *,void (*)(...),void *,void (*)(...),void *,int,char *);
#define QUIT_RECORD_TEST(r,f,p,g,s,q,m) xQUIT_RECORD_TEST(r,f,p,g,s,q,m,__LINE__,__FILE__)
void xQUIT_RECORD_TEST(RDArsrc *,short,RDATData *,void (*)(...),void (*)(...),void (*)(...),void *,int,char *);


/* VIRFLD STUFF */
#define SHOWvirtual(a)	xSHOWvirtual(a,__LINE__,__FILE__)
void xSHOWvirtual(RDAvirtual *,int,char *);
#define computevirtual(a,b,c)	xcomputevirtual(a,b,c,__LINE__,__FILE__)
void xcomputevirtual(RDAvirtual *,void (*)(...),void *,int,char *);
#define addDFincvir(a,b,c,d,e)	xaddDFincvir(a,b,c,d,e,__LINE__,__FILE__)
short xaddDFincvir(RDArsrc *,char *,char *,char *,short,int,char *);
#define copyDFincvir(f,t)	xcopyDFincvir(f,t,__LINE__,__FILE__)
RDArsrc *xcopyDFincvir(RDArsrc *,RDArsrc *,int,char *);
#define copyRDAvirtual(a,b)	xcopyRDAvirtual(a,b,__LINE__,__FILE__)
RDArsrc *xcopyRDAvirtual(RDArsrc *,RDArsrc *,int,char *);
#define ADVaddSCRNvirtual(a,b,c,d,e,f,g)	xaddSCRNvirtual(a,b,c,d,e,f,g,__LINE__,__FILE__)
short SCRNExecuteSubData(short (*)(...),char **,RDArsrc *,char *);
short SCRNvirtualVIRTUALSubData(char **,RDArsrc *,char *);
short SCRNvirtualFIELDSubData(char **,RDArsrc *,char *);
#define addSCRNvirtual(a,b,c,d,e,f)	xaddSCRNvirtual(a,b,c,d,e,f,TRUE,__LINE__,__FILE__)
int xaddSCRNvirtual(RDArsrc *,char *,int,unsigned,char *,short,short,int,char *);
#define ADV3GET_SCREEN_VIRTUALS(a,b,c,d,e,f)	xGET_SCREEN_VIRTUALS(a,b,c,d,e,f,__LINE__,__FILE__)
#define ADVGET_SCREEN_VIRTUALS(a,b,c,d)	xGET_SCREEN_VIRTUALS(a,b,c,d,TRUE,TRUE,__LINE__,__FILE__)
#define ADV2GET_SCREEN_VIRTUALS(a,b,c,d,e)	xGET_SCREEN_VIRTUALS(a,b,c,d,TRUE,e,__LINE__,__FILE__)
#define GET_ALL_SCREEN_VIRTUALS(a,b)	xGET_SCREEN_VIRTUALS(a,b,TRUE,TRUE,TRUE,TRUE,__LINE__,__FILE__)
#define GET_SCREEN_VIRTUALS(a,b)	xGET_SCREEN_VIRTUALS(a,b,FALSE,TRUE,TRUE,TRUE,__LINE__,__FILE__)
void xGET_SCREEN_VIRTUALS(RDArsrc *,short,short,short,short,short,int,char *);
#define SCRNvirtual2rangersrc(a)	xSCRNvirtual2rangersrc(a,SCRNvirtualSubData,a,__LINE__,__FILE__)
#define ADVSCRNvirtual2rangersrc(a,b,c)	xSCRNvirtual2rangersrc(a,b,c,__LINE__,__FILE__)
short xSCRNvirtual2rangersrc(RDArsrc *,void(*)(...),void *,int,char *);
#define ADVSCRNvirtual2rsrc(a,b,c)	xSCRNvirtual2rsrc(a,b,c,__LINE__,__FILE__)
#define SCRNvirtual2rsrc(a)	xSCRNvirtual2rsrc(a,SCRNvirtualSubData,a,__LINE__,__FILE__)
void xSCRNvirtual2rsrc(RDArsrc *,void (*)(...),void *,int,char *);
#define CLEAR_SCREEN_VIRTUAL(a)	xCLEAR_SCREEN_VIRTUAL(a,__LINE__,__FILE__)
void xCLEAR_SCREEN_VIRTUAL(RDArsrc *,int,char *);
#define COMPUTE_SCREEN_VIRTUAL(a,b,c,d)	xCOMPUTE_SCREEN_VIRTUAL(a,b,c,d,__LINE__,__FILE__)
short xCOMPUTE_SCREEN_VIRTUAL(RDArsrc *,char *,void (*)(...),void *,
	int,char *);
short SCRNFILENUMBER(RDArsrc *,char *,char *,short);
void SCRNvirtualSubData(char **,RDArsrc *);
short ScrolledListSubData(char **,RDArsrc *);
#define CreateRDAScrolledList(a,b,c,d)	xCreateRDAScrolledList(a,b,c,d,__LINE__,__FILE__)
void xCreateRDAScrolledList(RDArsrc *,RDAScrolledList *,void (*)(...),void *,int,char *);
void ComputeSCRNcallback(RDArsrc *,MaintainMaster *);
void DefaultComputeSCRNcallback(RDArsrc *);
#define IncVirRsrcstoFile(a)	xIncVirRsrcstoFile(a,__LINE__,__FILE__)
void xIncVirRsrcstoFile(RDArsrc *r,int,char *);
#define SetVirFieldFuncs(a)	xSetVirFieldFuncs(a,NULL,NULL,FALSE,NULL,NULL,__LINE__,__FILE__)
#define MSTSetVirFieldFuncs(a,b)	xSetVirFieldFuncs(a,NULL,NULL,FALSE,b,NULL,__LINE__,__FILE__)
#define ADVSetVirFieldFuncs(a,b,c)	xSetVirFieldFuncs(a,b,c,FALSE,NULL,NULL,__LINE__,__FILE__)
#define MSTADVSetVirFieldFuncs(a,b,c,d)	xSetVirFieldFuncs(a,b,c,FALSE,d,NULL,__LINE__,__FILE__)
#define RMEMSetVirFieldFuncs(a,b,c,d)	xSetVirFieldFuncs(a,b,c,d,NULL,NULL,__LINE__,__FILE__)
#define MSTRMEMSetVirFieldFuncs(a,b,c,d,e)	xSetVirFieldFuncs(a,b,c,d,e,NULL,__LINE__,__FILE__)
void xSetVirFieldFuncs(RDArsrc *,void (*)(...),void *,short,MaintainMaster *,PowerAdd *,int,char *);
#define SetListValue(f,r,l,u,s,a)	xSetListValue(f,r,l,u,s,a,__LINE__,__FILE__)
void xSetListValue(short,RDArsrc *,RDAScrolledList *,short,void (*)(...),void *,int,char *);
#define SensitiveMSTcomputeandupdateallSCRNvirtuals(a,b,c,d,e,f) xcomputeandupdateallSCRNvirtuals(a,b,c,d,e,f,NULL,__LINE__,__FILE__)
#define SensitivePOWERADDcomputeandupdateallSCRNvirtuals(a,b,c,d,e,f) xcomputeandupdateallSCRNvirtuals(a,b,c,d,e,NULL,f,__LINE__,__FILE__)
#define SensitiveADVcomputeandupdateallSCRNvirtuals(a,b,c,d,e) xcomputeandupdateallSCRNvirtuals(a,b,c,d,e,NULL,NULL,__LINE__,__FILE__)
#define MSTADVcomputeandupdateallSCRNvirtuals(a,b,c,d,f) xcomputeandupdateallSCRNvirtuals(a,b,c,d,TRUE,f,NULL,__LINE__,__FILE__)
#define POWERADDADVcomputeandupdateallSCRNvirtuals(a,b,c,d,f) xcomputeandupdateallSCRNvirtuals(a,b,c,d,TRUE,NULL,f,__LINE__,__FILE__)
#define ADVcomputeandupdateallSCRNvirtuals(a,b,c,d) xcomputeandupdateallSCRNvirtuals(a,b,c,d,TRUE,NULL,NULL,__LINE__,__FILE__)
#define MSTcomputeandupdateallSCRNvirtuals(a,b,f)	xcomputeandupdateallSCRNvirtuals(a,SCRNvirtualSubData,a,b,TRUE,f,NULL,__LINE__,__FILE__)
#define POWERADDcomputeandupdateallSCRNvirtuals(a,b,c,d,f)	xcomputeandupdateallSCRNvirtuals(a,c,d,b,TRUE,NULL,f,__LINE__,__FILE__)
#define computeandupdateallSCRNvirtuals(a,b)	xcomputeandupdateallSCRNvirtuals(a,SCRNvirtualSubData,a,b,TRUE,NULL,NULL,__LINE__,__FILE__)
#define MSTcomputeallSCRNvirtuals(a,f) xcomputeandupdateallSCRNvirtuals(a,SCRNvirtualSubData,a,0,TRUE,f,NULL,__LINE__,__FILE__)
#define POWERADDcomputeallSCRNvirtuals(a,f,b,c) xcomputeandupdateallSCRNvirtuals(a,b,c,0,TRUE,NULL,f,__LINE__,__FILE__)
#define computeallSCRNvirtuals(a) xcomputeandupdateallSCRNvirtuals(a,SCRNvirtualSubData,a,0,TRUE,NULL,NULL,__LINE__,__FILE__)
void xcomputeandupdateallSCRNvirtuals(RDArsrc *,void (*)(...),void *,short,short,MaintainMaster *,PowerAdd *,int,char *);
#define updateSCRNvirtuals(a)	xupdateSCRNvirtuals(a,NULL,NULL,NULL,NULL,__LINE__,__FILE__)
#define MSTupdateSCRNvirtuals(a,b)	xupdateSCRNvirtuals(a,NULL,NULL,b,NULL,__LINE__,__FILE__)
#define POWERADDupdateSCRNvirtuals(a,b)	xupdateSCRNvirtuals(a,NULL,NULL,NULL,b,__LINE__,__FILE__)
#define ADVupdateSCRNvirtuals(a,b,c)	xupdateSCRNvirtuals(a,b,c,NULL,NULL,__LINE__,__FILE__)
#define MSTADVupdateSCRNvirtuals(a,b,c,d)	xupdateSCRNvirtuals(a,b,c,d,NULL,__LINE__,__FILE__)
#define POWERADDADVupdateSCRNvirtuals(a,b,c,d)	xupdateSCRNvirtuals(a,b,c,NULL,d,__LINE__,__FILE__)
void xupdateSCRNvirtuals(RDArsrc *,void (*)(...),void *,MaintainMaster *,PowerAdd *,int,char *);
/* VIRTUAL STUFF ABOVE */
char *GUIEvalStr(char *,void (*)(...),void *);
short GUIEVAL(char *,RDArsrc *);
void addoswidgets(RDArsrc *);
#define ADVmakescrn(a,b)	xmakescrn(a,b,GUIEVAL,NULL,addoswidgets,GUIEvalStr,NULL,SCRNvirtualSubData,a,NULL,__LINE__,__FILE__)
#define RDAmakescrn(a,b,c)	xmakescrn(a,b,c,NULL,addoswidgets,GUIEvalStr,SCRNvirtualSubData,SCRNvirtualSubData,a,NULL,__LINE__,__FILE__)

#define addSCRNScrolledList(a,b,c,x,y,z,d,e,f,g,q,r,s,t,h,i,j,k,l,u,m,n,o,p,v) xaddSCRNScrolledList(a,b,c,x,y,z,d,e,f,g,q,r,s,t,h,i,j,k,l,u,m,n,o,p,v,__LINE__,__FILE__)
int xaddSCRNScrolledList(RDArsrc *,char *,char *,int,int,DFincvir *,int,char *,char *,short,RDAconfld *,char *,short,DFincvir *,int,char *,char *,char *,short,APPlib *,char *,char *,char *,char *,int,int,char *);
#define addSCRNLoadValues(a,b,c,d,e,f,g) xaddSCRNLoadValues(a,b,c,d,e,f,g,NULL,__LINE__,__FILE__)
#define addSCRNLoadValuesADV(a,b,c,d,e,f,g,h) xaddSCRNLoadValues(a,b,c,d,e,f,g,h,__LINE__,__FILE__)
int xaddSCRNLoadValues(RDArsrc *,char *,char *,int,DFincvir *,char *,char *,APPlib *,int,char *);
void FreeRDAvirtual(RDAvirtual *v);
void RDALoadValueCB(RDArsrc *,RDALoadValue *);
void RDALoadOperatorCB(RDArsrc *,RDALoadValue *);
void RDALoadGrouperCB(RDArsrc *,RDALoadValue *);
void RDALoadControlCB(RDArsrc *,RDALoadValue *);
#define ReadRDAScrolledLists(a) xReadRDAScrolledLists(a,__LINE__,__FILE__)
void xReadRDAScrolledLists(RDArsrc *,int,char *);
#define ReadRDAScrolledListsKeysOnly(a,b)	xReadRDAScrolledListsKeysOnly(a,b,__LINE__,__FILE__)
void xReadRDAScrolledListsKeysOnly(RDArsrc *,short,int,char *);
#define ReadRDAScrolledListsNoPassKeys(a,b)	xReadRDAScrolledListsNoPassKeys(a,b,__LINE__,__FILE__)
void xReadRDAScrolledListsNoPassKeys(RDArsrc *,PassKey *,int,char *);
#define SEERDAScrolledList(s,f,d) xSEERDAScrolledList(s,f,d,__LINE__,__FILE__)
#define SEESList(s) xSEERDAScrolledList(s,RDA_STDERR,FALSE,__LINE__,__FILE__)
void xSEERDAScrolledList(RDAScrolledList *,RDA_PFILE *,char,int,char *);
#define SEERDAScrolledList_detail(s,f,d,p,n) xSEERDAScrolledList_detail(s,f,d,p,n,__LINE__,__FILE__)
void xSEERDAScrolledList_detail(RDAScrolledList *,RDA_PFILE *,char,int *,int *,int,char *);
#define ADVSetRDAScrolledList(a,b,c)	xADVSetRDAScrolledList(a,b,c,__LINE__,__FILE__)
void xADVSetRDAScrolledList(RDArsrc *,RDAScrolledList *,short,int,char *);
#define GET_PREVIOUS(r,m,p,s,a,k)	xGET_PREVIOUS(r,m,p,s,a,(-1),k,m->inheritrsrcs,NULL,NULL,NULL,NULL,NULL,NULL,__LINE__,__FILE__)
#define GET_PREVIOUSNM(r,p,s,a,f,k,i)	xGET_PREVIOUS(r,NULL,p,s,a,f,k,i,NULL,NULL,NULL,NULL,NULL,NULL,__LINE__,__FILE__)
#define ADVGET_PREVIOUS(r,m,p,s,a,k,n,g,z)	xGET_PREVIOUS(r,m,p,s,a,(-1),k,m->inheritrsrcs,n,z,NULL,NULL,g,z,__LINE__,__FILE__)
#define ADVGET_PREVIOUSNM(r,p,s,a,f,k,n,g,z)	xGET_PREVIOUS(r,NULL,p,s,a,f,k,i,n,z,NULL,NULL,g,z,__LINE__,__FILE__)
#define ADV2GET_PREVIOUS(r,m,p,s,a,k,n,x,g,y,h,z)	xGET_PREVIOUS(r,m,p,s,a,(-1),k,m->inheritrsrcs,n,x,g,y,h,z,__LINE__,__FILE__)
#define ADV2GET_PREVIOUSNM(r,p,s,a,f,k,i,n,x,g,y,h,z)	xGET_PREVIOUS(r,NULL,p,s,a,k,i,n,x,g,y,h,z,__LINE__,__FILE__)
#define ADVIRGET_PREVIOUS(r,m,p,s,a,k,i,n,x,g,y,h,z)	xGET_PREVIOUS(r,m,p,s,a,(-1),k,i,n,x,g,y,h,z,__LINE__,__FILE__)
#define ADVIRGET_PREVIOUSNM(r,m,p,s,a,f,k,i,n,x,g,y,h,z)	xGET_PREVIOUS(r,m,p,s,a,f,k,i,n,x,g,y,h,z,__LINE__,__FILE__)
void xGET_PREVIOUS(RDArsrc *,MaintainMaster *,RDATData *,void (*)(...),void *,
	short,short,short,void (*)(...),void *,void (*)(...),void *,void (*)(...),void *,
	int,char *);
#define GET_NEXT(r,m,p,s,a,k)	xGET_NEXT(r,m,p,s,a,(-1),k,m->inheritrsrcs,NULL,NULL,NULL,NULL,NULL,NULL,__LINE__,__FILE__)
#define GET_NEXTNM(r,p,s,a,f,k,i)	xGET_NEXT(r,NULL,p,s,a,f,k,i,NULL,NULL,NULL,NULL,NULL,NULL,__LINE__,__FILE__)
#define ADVGET_NEXT(r,m,p,s,a,k,n,g,z)	xGET_NEXT(r,m,p,s,a,(-1),k,m->inheritrsrcs,n,z,NULL,NULL,g,z,__LINE__,__FILE__)
#define ADVGET_NEXTNM(r,p,s,a,f,k,n,g,z)	xGET_NEXT(r,NULL,p,s,a,f,k,i,n,z,NULL,NULL,g,z,__LINE__,__FILE__)
#define ADV2GET_NEXT(r,m,p,s,a,k,n,x,g,y,h,z)	xGET_NEXT(r,m,p,s,a,(-1),k,m->inheritrsrcs,n,x,g,y,h,z,__LINE__,__FILE__)
#define ADV2GET_NEXTNM(r,p,s,a,f,k,i,n,x,g,y,h,z)	xGET_NEXT(r,NULL,p,s,a,k,i,n,x,g,y,h,z,__LINE__,__FILE__)
#define ADVIRGET_NEXT(r,m,p,s,a,k,i,n,x,g,y,h,z)	xGET_NEXT(r,m,p,s,a,(-1),k,i,n,x,g,y,h,z,__LINE__,__FILE__)
#define ADVIRGET_NEXTNM(r,m,p,s,a,f,k,i,n,x,g,y,h,z)	xGET_NEXT(r,m,p,s,a,f,k,i,n,x,g,y,h,z,__LINE__,__FILE__)
void xGET_NEXT(RDArsrc *,MaintainMaster *,RDATData *,void (*)(...),void *,
	short,short,short,void (*)(...),void *,void (*)(...),void *,void (*)(...),void *,
	int,char *);

#define GET_RANGESCREEN_EXPRESSION_VIRTUALS(a)	xGET_RANGESCREEN_EXPRESSION_VIRTUALS(a,__LINE__,__FILE__)
void xGET_RANGESCREEN_EXPRESSION_VIRTUALS(RDArsrc *,int,char *);
#define ReturnSupportingFileno(a,b,c,d)	xReturnSupportingFileno(a,b,c,d,__LINE__,__FILE__)
short xReturnSupportingFileno(MaintainMaster *,char *,char *,short,int,char *);
#define SetRDAScrolledListEditable(a,b,c,d,e)	xSetRDAScrolledListEditable(a,b,c,d,e,__LINE__,__FILE__)
void xSetRDAScrolledListEditable(RDArsrc *,short,short,short,short,int,char *);
#define SetSupportingWritable(a,b,c,d,e,f)	xSetSupportingWritable(a,b,c,d,e,f,__LINE__,__FILE__)
short xSetSupportingWritable(MaintainMaster *,char *,char *,short,short,char *,int,char *);
#define APPReturnSupportingFileno(a,b,c,d)	xAPPReturnSupportingFileno(a,b,c,d,__LINE__,__FILE__)
short xAPPReturnSupportingFileno(MaintainMaster *,char *,char *,short,
	int,char *);
void ShutdownOnError(void);
#define ADVMakePrevNextButtons(a,b,c,d,e,f,g)	xMakePrevNextButtons(a,b,c,d,e,f,g,__LINE__,__FILE__)
#define MakePrevNextButtons(a,b,c,d)	xMakePrevNextButtons(a,b,c,getpreviouscb,d,getnextcb,d,__LINE__,__FILE__)
#define ADVFMakePrevNextButtons(a,b,c,d,e,f)	xMakePrevNextButtons(a,KEYPOINTER(b),NUMKEYS(b),c,d,e,f,__LINE__,__FILE__)
void xMakePrevNextButtons(RDArsrc *,NRDkey *,short,void (*)(...),void *,
	void (*)(...),void *,int,char *);
#define FreeBrowseExit(a)	xFreeBrowseExit(a,__LINE__,__FILE__)
void xFreeBrowseExit(Browse_Exit *,int,char *);
#define GETVirtualsFromExpression(a,b)	xGETVirtualsFromExpression(a,b,__LINE__,__FILE__)
void xGETVirtualsFromExpression(RDArsrc *,char *,int,char *);

#define setfilekeyFROMrangersrc(f,k,r)	xsetfilekeyFROMrangersrc(f,k,r,__LINE__,__FILE__)
short xsetfilekeyFROMrangersrc(short,short,RangeScreen *,int,char *);
#define testfilekeyTOrangersrc(f,k,r)	xtestfilekeyTOrangersrc(f,k,r,__LINE__,__FILE__)
short xtestfilekeyTOrangersrc(short,short,RangeScreen *,int,char *);

#define GTENRDRNG(f,k,r)	xGTENRDRNG(f,k,r,0,NULL,NULL,__LINE__,__FILE__)
#define ADVGTENRDRNG(f,k,r,s,a)	xGTENRDRNG(f,k,r,0,s,a,__LINE__,__FILE__)
#define GTENRDRNGsec(f,k,r)	xGTENRDRNG(f,k,r,1,NULL,NULL,__LINE__,__FILE__)
#define ADVGTENRDRNGsec(f,k,r,s,a)	xGTENRDRNG(f,k,r,1,s,a,__LINE__,__FILE__)
short xGTENRDRNG(short,short,RangeScreen *,short,void (*)(...),void *,int,char *);

#define NXTNRDRNG(f,k,r)	xNXTNRDRNG(f,k,r,0,NULL,NULL,__LINE__,__FILE__)
#define ADVNXTNRDRNG(f,k,r,s,a)	xNXTNRDRNG(f,k,r,0,s,a,__LINE__,__FILE__)
#define NXTNRDRNGsec(f,k,r)	xNXTNRDRNG(f,k,r,1,NULL,NULL,__LINE__,__FILE__)
#define ADVNXTNRDRNGsec(f,k,r,s,a)	xNXTNRDRNG(f,k,r,1,s,a,__LINE__,__FILE__)
short xNXTNRDRNG(short,short,RangeScreen *,short,void (*)(...),void *,int,char *);

void SetRangersrcsensitive(RDArsrc *);
void setrangesensitive(RDArmem *);
void setrangesensitivecb(RDArmem *);
#define DeleteSubordinatePowerWrite(a,b,c,d)	xMaintainMasterPowerWrite(a,b,c,d,__LINE__,__FILE__)
#define MaintainMasterPowerWrite(a,b,c)	xMaintainMasterPowerWrite(a,NULL,b,c,__LINE__,__FILE__)
char xMaintainMasterPowerWrite(MaintainMaster *,RDAsubord *,int,PowerWrite *,int,char *);

void POWERADDComputeSCRNcallback(RDArsrc *,PowerAdd *);
#define POWERADDSetVirFieldFuncs(a,b)	xSetVirFieldFuncs(a,NULL,NULL,FALSE,NULL,b,__LINE__,__FILE__)
#define POWERADDADVSetVirFieldFuncs(a,b,c,d)	xSetVirFieldFuncs(a,b,c,FALSE,NULL,d,__LINE__,__FILE__)
#define POWERADDRMEMSetVirFieldFuncs(a,b,c,d,e)	xSetVirFieldFuncs(a,b,c,d,NULL,e,__LINE__,__FILE__)

char *make_definelist_string(RDArsrc *,short,char **);
void printbrowse(RDArsrc *,MakeBrowseList *);

char *GETASSIGNNEXTNEWID(PowerAdd *,AddEditWdgt *,char *,short,void (*)(...),void *);
#define ASSIGNNEWID(a,b,c,d,e)	xASSIGNNEWID(a,b,c,d,e,__LINE__,__FILE__)
void xASSIGNNEWID(PowerAdd *,AddEditWdgt *,char *,void (*)(...),void *,int,char *);
void UndoASSIGNID(PowerAdd *,AddEditWdgt *,void (*)(...),void *);
#define REMOVENEWID(a,b,c,d)	xREMOVENEWID(a,b,c,d,__LINE__,__FILE__)
void xREMOVENEWID(PowerAdd *,AddEditWdgt *,void (*)(...),void *,int,char *);

#define SetPowerAddSupportingFiles(a,b,c,d,e,f)	xSetPowerAddSupportingFiles(a,b,c,d,e,f,__LINE__,__FILE__)
char xSetPowerAddSupportingFiles(RDArsrc *,PowerAdd *,void (*)(...),void *,short,short,int,char *);
#define DoMTNAction(a,b,c,d,e)	xDoMTNAction(a,b,c,d,e,__LINE__,__FILE__)
void xDoMTNAction(RDArmem *,MaintainMaster *,void (*)(...),void *,short,int,char *);
#define ApplyActions(a,b,c,d,e)	xApplyActions(a,b,c,d,e,__LINE__,__FILE__)
void xApplyActions(RDArsrc *,MaintainMaster *,void (*)(...),void *,short,int,char *);
#define ScrolledListDFincvir(a,b,c,d,e)	xScrolledListDFincvir(a,b,c,d,e,__LINE__,__FILE__)
void xScrolledListDFincvir(RDAScrolledList *,char *,char *,char *,short,int,char *);
#define DFvirtualDFincvir(a,b,c,d,e)	xDFvirtualDFincvir(a,b,c,d,e,__LINE__,__FILE__)
void xDFvirtualDFincvir(DFvirtual *,char *,char *,char *,short,int,char *);
void SetRDAsearchOccurrence(MaintainMaster *);
#define GLOBALSubData(a,b)	xGLOBALSubData(a,b,__LINE__,__FILE__)
short xGLOBALSubData(char **,char *,int,char *);
void PP_translate_GUI(char **);
void PP_translate_GUIALL(RDArsrc *);


#define RDALoadValueNEW(a,b)	xRDALoadValueNEW(a,b,__LINE__,__FILE__)
RDALoadValue *xRDALoadValueNEW(char *,char *,int,char *);
#define getRDALoadValuebin(a,b)	xgetRDALoadValuebin(a,b,__LINE__,__FILE__)
short xgetRDALoadValuebin(char *,RDALoadValue *,int,char *);
#define getRDALoadValuedesc(m,n) xgetRDALoadValuedesc(m,n,__LINE__,__FILE__)
char *xgetRDALoadValuedesc(char *,char *,int,char *);
#define writeRDALoadValuebin(a,b)	xwriteRDALoadValuebin(a,b,__LINE__,__FILE__)
short xwriteRDALoadValuebin(char *,RDALoadValue *,int,char *);
#define SEERDALoadValue(s,f,d) xSEERDALoadValue(s,f,d,__LINE__,__FILE__)
#define SEERDARLV(s,f) xSEERDALoadValue(s,f,FALSE,__LINE__,__FILE__)
void xSEERDALoadValue(RDALoadValue *,RDA_PFILE *,char,int,char *);
#define SEERDALoadValue_detail(s,f,d,p,n) xSEERDALoadValue_detail(s,f,d,p,n,__LINE__,__FILE__)
void xSEERDALoadValue_detail(RDALoadValue *,RDA_PFILE *,char,int *,int *,int,char *);
#define RDALoadValueDFincvir(a,b,c,d,e)	xRDALoadValueDFincvir(a,b,c,d,e,__LINE__,__FILE__)
void xRDALoadValueDFincvir(RDALoadValue *,char *,char *,char *,short,int,char *);
#define COPY_FILE(a,b,c) xCOPY_FILE(a,b,c,__LINE__,__FILE__)
int xCOPY_FILE(char *,char *,char *,int,char *);
#define FORMAT_MEDIA(a) xFORMAT_MEDIA(a,__LINE__,__FILE__)
int xFORMAT_MEDIA(char *,int,char *);
void do_dos_copy(RDArsrc *,RDADosCopy *);
int run_dos_copy(RDArsrc *,char *,char *,char *,char *,short,APPlib *,void (*)(...),void *,char *);
void do_dos_format(RDArsrc *,RDADosCopy *);

#define RDApdevNEW(x)	xRDApdevNEW(x,__LINE__,__FILE__)
RDApdev *xRDApdevNEW(char *,int,char *);
#define free_pdev(x)	{xfree_pdev(x,__LINE__,__FILE__);x=NULL;}
void xfree_pdev(RDApdev *,int,char *);
#define writepdevbin(x)	xwritepdevbin(x,__LINE__,__FILE__)
short xwritepdevbin(RDApdev *,int,char *);
short getpdevbin(RDApdev *);

void SelectFuncDeviceName(RDArsrc *,RDArsrc *);
void SelectFuncOutputDevice(RDArsrc *,RDArsrc *);
void SelectFuncDevice(RDArsrc *,RDArsrc *);
void LoadDeviceWindow(RDArsrc *,void (*)(...));
void LoadDeviceWindowAutoArchive(RDArsrc *,void (*)(...));
char *PRT_Text_File(char *);
void *PRT_Binary_File(char *,int *);
#define SelectPrintType(a,b,c)	xSelectPrintType(a,b,c,__LINE__,__FILE__)
void xSelectPrintType(char *,char,RDA_PFILE *,int,char *);
#define SelectPrintTypeByName(a,b,c)	xSelectPrintTypeByName(a,b,c,__LINE__,__FILE__)
void xSelectPrintTypeByName(char *,char *,RDA_PFILE *,int,char *);
void quitSelectDevice(RDArsrc *);
extern APPlib *MYout_devs;
void BrowseLoadValue(RDArsrc *,char *);
APPlib *GetFieldsAvailable(char *,void (*)(...),void *);
APPlib *GetProcessesAvailable(char *,void (*)(...),void *);
APPlib *FindFilesAvailable(char *,void (*)(...),void *);
void MakeBrowseListTotals(RDArsrc *,short,RDArsrc *);
void ZeroBrowseListTotals(RDArsrc *,short,RDArsrc *);
void IncrementBrowseListTotals(RDArsrc *,short,RDArsrc *);
#define  getrangedesc(a)	xgetrangedesc(a,__LINE__,__FILE__)
char *xgetrangedesc(RangeScreen *,int,char *);
char *MakeArchiveName(char *,char *);
#define AddSupportRequest(a,b,c)	xAddSupportRequest(a,b,c,__LINE__,__FILE__)
void xAddSupportRequest(char *,char *,char *,int,char *);
#define GuessCustomFieldType(a,b)	xGuessCustomFieldType(a,b,__LINE__,__FILE__)
short xGuessCustomFieldType(char *,char *,int,char *);
void CheckDiskSpace(void);
#define APPlib2SupportRequest(a,b,c)	xAPPlib2SupportRequest(a,b,c,__LINE__,__FILE__)
void xAPPlib2SupportRequest(char *,char *,APPlib *,int,char *);
void ResetGroupDefaults(RDArsrc *);
short DoPROCsecurity(char *,char *);
void RDAUsersConfig(char *);
void DOHTTP(int,char **);
char *WorkStationIPAddr(void);
void setrangeto2fromcb(RDArmem *);
void browselistcsv(RDArsrc *,MakeBrowseList *);
short download_blob_data(char *,char *,char *,char *,char *,char *);
char *OpenRDA_AutoComplete(char *,char *,char *,char *);
void OpenRDA_CaptureAutoComplete(char *,char *,char *,char *);
short DoInitializeSecurity(void);
#define LongestAccountCode(a)	xLongestAccountCode(a,__LINE__,__FILE__)
short xLongestAccountCode(short type,int line,char *file);

#endif /* MIX_H */
