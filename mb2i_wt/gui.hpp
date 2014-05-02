
/******************************************************************************
			GRAPHICAL USER INTERFACE HEADER  
			 	    gui.hpp
	purpose: Contains macros and function prototypes for creating and
			manipulating resources dealing with the gui.
******************************************************************************/
#ifndef GUI_HPP
#define GUI_HPP
#include <misc.hpp>

#include <Wt/WWidget>
#include <Wt/WCssStyleSheet>
#include <Wt/WApplication>
#include <Wt/WServer>
#include <Wt/WToolBar>
#include <Wt/WPopupMenu>
#include <Wt/WMenuItem>

using namespace Wt;
using namespace Wt::Auth;

#define USING_WT
extern APPlib *CMAIN_ARGS,*OpenRDA_Cookies;
extern Wt::WContainerWidget *QN2IT_PARENT;
extern short QN2IT_PARENT_TYPE,MODULE_GROUP;
extern char *LastGroupDefault;

extern char SIMPLE_SCREENS,SKIP_DOMENU_TRANSVERSAL,CENTER_WINDOWS;
extern Wt::WApplication *RDAMAINWIDGET;
extern std::string RDA_Session;
extern char *membertypes[5];
extern char *wdgttypes[34];
extern char *standardfieldtypes[29];
extern char *accttypes[5];
extern int WindowCount;
extern char Skip_OpenRDA_Cookies;
extern int RDA_UserAgent;
extern int RDA_FONTSIZE;
extern short GUI_INTERFACE;
extern char USE_SPEECH_SYNTHESIZER,USE_BROWSELISTLABEL;
extern char *RDA_GROUP;
extern short PUSHBUTTON_STYLE;
extern char GUIstemp[1024];
extern short USE_DIAGNOSTIC_SCREENS;

struct RDAaccparts
{
	char *name;
	short length;
	char *definition;
	char default_zero;
	char force_zero;
	char last_dimension;
};
typedef struct RDAaccparts RDAaccpart;

struct RDAaccts
{
	char *name;
	short num;
	RDAaccpart *acct;
};
typedef struct RDAaccts RDAacct;

struct RDAacctmstrs
{
	short type;
	int num;
	RDAacct *codetype;
};
typedef struct RDAacctmstrs RDAacctmstr;

extern RDAacctmstr *EXPENDITURE_ACCOUNT;
extern RDAacctmstr *REVENUE_ACCOUNT;
extern RDAacctmstr *BALANCE_SHEET_ACCOUNT;
extern RDAacctmstr *BEGINNING_BALANCE_ACCOUNT;
extern RDAacctmstr *CUSTOM_INPUTS_ACCOUNT;

struct DFincvirs
{
	char *module;
	char *file;
	char *keyname;
	short fileno;
	void *v;
};
typedef struct DFincvirs DFincvir;

struct RDAvirtuals
{
	int when; /* 0 - select time, 1 - output time */
	char *name;
	int type;
	unsigned len;
	char *expression;
	union rda_field_types value;
	char computed;
	char range;
	unsigned dlen;
};
typedef struct RDAvirtuals RDAvirtual;

#define INITGUI(x,y,d)	xINITGUI(x,y,d,__LINE__,__FILE__)
/* INITGUI has 3 arguments, the first being the number of command line 
	arguments, the second an arrary of character pointers reflecting 
	those arguments  */
#define RDAAPPMAINLOOP()	xRDAAPPMAINLOOP(__LINE__,__FILE__)
/* RDAAPPMAINLOOP has no arguments, and is used similar to an XtAppMainLoop 
	in the Xwindow system, it get's and dispatches events */
#define FINDRSCSHOWVALUE(x,y)	xFINDRSCSHOWVALUE(x,y,__LINE__,__FILE__)
/* SEEMEMBER has one argument, a (RDArmem *) pointer to an RDA Member Structure
	allocated somewhere in memory */
#define SEEMEMBER(x)	xSEEMEMBER(x,__LINE__,__FILE__)
/* SEEMEMBER has one argument, a (RDArmem *) pointer to an RDA Member Structure
	allocated somewhere in memory */
#define SEERSRCS(x)	xSEERSRCS(x,__LINE__,__FILE__)
/* SEERSRCS has one argument, a (RDArsrc *) pointer to an RDA Resource Structure
	allocated somewhere in memory */
#define SEESCRN(x)	xSEESCRN(x,__LINE__,__FILE__)
/* SEESCRN has one argument, a (RDAscrn *) pointer to an RDA Screen Structure
	allocated somewhere in memory */
#define FINDRSCSETVALUE(x,y,z)	xFINDRSCSETVALUE(x,y,z,__LINE__,__FILE__)
/* FINDRSCSETVALUE has three arguments and is used to locate a specific 
	resource member (RDArmem *) in an RDA Resource Structure (RDArsrc *)
	and set it's value.  The first argument is the pointer to the 
	RDA Resource Structure (RDArsrc *).  The second the name (char *) of 
	the particular (RDArmem *) structure.  The third the value a (void *)
	pointer.  The value is a (void *) so it takes all values. */ 
#define FINDRSCSETEDITABLE(x,y,z)	xFINDRSCSETEDITABLE(x,y,z,__LINE__,__FILE__)
#define FINDRSCSETDEFTYPE(x,y,z)	xFINDRSCSETDEFTYPE(x,y,z,__LINE__,__FILE__)
#define FINDRSCGETDEFTYPE(x,y,z)	xFINDRSCGETDEFTYPE(x,y,z,__LINE__,__FILE__)
#define FINDRSCSETACCTTYPE(x,y,z)	xFINDRSCSETACCTTYPE(x,y,z,__LINE__,__FILE__)
#define FINDRSCGETACCTTYPE(x,y,z)	xFINDRSCGETACCTTYPE(x,y,z,__LINE__,__FILE__)
#define FINDRSCGETVALUE(x,y,z)	xFINDRSCGETVALUE(x,y,z,__LINE__,__FILE__)
/* FINDRSCGETVALUE has three arguments, the RDA Resource Structure (RDArsrc *)
	allocated in memory, the second the name (char *) of the (RDArmem *)
	resource member, and finally a pointer to a variable to recieve the
	member's value.  This pointer is a (void *) to accept all types. */
#define FINDRSCGETSTRING(x,y,z)	xFINDRSCGETSTRING(x,y,z,__LINE__,__FILE__)
/* FINDRSCGETSTRING has three arguments, the RDA Resource Structure (RDArsrc *)
	allocated in memory. the second the name (char *) of the (RDArmem *)
	resource member, and finally a pointer to a character pointer (char **)
	to receive the string value. */
#define FINDRSCSETLIST(a,b,c,d,e)	xFINDRSCSETLIST(a,b,c,d,e,__LINE__,__FILE__)
/* FINDRSCSETLIST finds a resource member in an RDA Resource Structure and
	sets the position, number of elements, and an array of char pointers
	to the particular resource member for scrolling lists. The first argument
	is a pointer to an RDA Resource Structure (RDArsrc *) allocated in memory,
	the second a name (char *) of the (RDArmem *) resource member, the 
	position (int), number of elements in list (int), and the array of list
	elements  (char **). */
#define FINDRSCLISTAPPlib(a,b,c,d)	xFINDRSCLISTAPPlib(a,b,c,d,__LINE__,__FILE__)
/* FINDRSCLSTAPPlib is similar to FINDRSCSETLIST but uses an APPlib structure
	for the number of elements and the it's contents the character array.
	This function has four arguments, a RDA Resource Structure (RDArsrc *) 
	allocated in memory, the name (char *) of the list element, the 
	position (int), and a pointer to an APPlib structure in memory. */
#define ADVFINDRSCSETFUNC(a,b,c,d,e)	xFINDRSCSETFUNC(a,b,c,d,e,__LINE__,__FILE__)
#define FINDRSCSETFUNC(a,b,c,d)	xFINDRSCSETFUNC(a,b,c,d,FALSE,__LINE__,__FILE__)
/* FINDRSCSETFUNC is used to find a specific resource element (RDArmem *) 
	and add a function and arguments to it. This function will be executed
	when pressing return in the resource, toggling, or clicking a button.
	The arguments are values allocated in memory, passed to this function.
	This function has 4 args, the furst a pointer to an RDA Resource 
	structure (RDArsrc *) allocated in memory, the name a (char *) of the
	element (RDArmem *), the function (void (*)()), and the arguments 
	(void *). */
#define FINDRSCSETCURSOR(a,b,c)	xFINDRSCSETCURSOR(a,b,c,__LINE__,__FILE__)

/* Add Widget Easy Functions */
#define addstdwdgt(x,y)		addwdgt(x,0,y,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL)
#define addnewline(x)		addwdgt(x,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL)
#define addendline(x)		addwdgt(x,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL)
#define addnewbox(x)		addwdgt(x,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL)
#define addendbox(x)		addwdgt(x,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL)
#define addlabel(x,y,p,z)	addwdgt(x,5,NULL,y,p,z,0,0,0,NULL,NULL,NULL)
#define addbutton(w,x,y,p,z)	addwdgt(w,6,x,y,p,z,0,0,0,NULL,NULL,NULL)
#define addscrlist(w,x,z)	addwdgt(w,7,x,NULL,NULL,NULL,z,0,0,NULL,NULL,NULL)
#define addscrtext(w,x,z,y,a)	addwdgt(w,8,x,NULL,NULL,NULL,z,y,a,NULL,NULL,NULL)
#define addtogbtn(x,y,z,p,w)	addwdgt(x,9,y,z,p,w,0,0,0,NULL,NULL,NULL) 

#define addwdgt(a,b,c,d,p,w,e,f,g,h,i,j)	xaddwdgt(a,b,c,d,p,w,e,f,g,h,i,j,NULL,__LINE__,__FILE__)
#define ADVaddwdgt(a,b,c,d,p,w,e,f,g,h,i,j,l)	xaddwdgt(a,b,c,d,p,w,e,f,g,h,i,j,l,__LINE__,__FILE__)

/* Add Resource Easy Functions */
#define addlstrsrcnh(r,n,v,e,w,x,z,a)	addrsrcnh(r,0,n,SCROLLEDLIST,0,v,e,w,x,z,a)
#define addsctrsrcnh(r,n,l,v,e)	addrsrcnh(r,0,n,SCROLLEDTEXT,l,v,e,NULL,0,NULL,NULL)
#define addstdrsrcnh(r,n,t,l,v,e)	addrsrcnh(r,0,n,t,l,v,e,NULL,0,NULL,NULL)
#define addbtnrsrcnh(r,n,e,f,g)	addrsrcnh(r,1,n,BUTTONS,0,NULL,e,f,0,NULL,g)
#define addrfcbrsrcnh(r,n,e,f,g)	addrsrcnh(r,2,n,BUTTONS,0,NULL,e,f,0,NULL,g)
#define addrfexrsrcnh(r,n,e,f,g)	addrsrcnh(r,3,n,BUTTONS,0,NULL,e,f,0,NULL,g)
#define addrfkwrsrcnh(r,n,e,f,g)	addrsrcnh(r,4,n,BUTTONS,0,NULL,e,f,0,NULL,g)
#define addrsrcnh(a,b,c,d,e,f,g,h,i,j,k)	xaddrsrc(a,b,c,d,e,f,g,h,i,j,k,NULL,0,__LINE__,__FILE__)
#define RDAscrnNEW(x,y)	xRDAscrnNEW(x,y,__LINE__,__FILE__)
/* RDAscrnNEW Initailly allocates memory for an RDAscrn Structure.  It has two 
	arguments, the Module Name and Screen Name, both are char *'s */
#define RDArsrcNEW(x,y)	xRDArsrcNEW(x,y,__LINE__,__FILE__)
/* RDArsrcNEW Initailly allocates memory for an RDArsrc Structure.  It has two 
	arguments, the Module Name and Screen Name, both are char *'s */
#define FINDRSC(x,y)	xFINDRSC(x,y,__LINE__,__FILE__)
/* FINDRSC Find Resource in an RDArsrc Structure.  FINDRSC Locates and returns 
	the sequence number of the RDArmem Structure containing the specified 
	field name.  The FINDRSC function needs two arguments, the RDArsrc * 
	and a (char *) representing the resource name */
#define free_scrn(x)	{xfree_scrn(x,__LINE__,__FILE__);x=NULL;}
/* free_scrn free's the memory allocated with an RDAscrn Structure.  It has 1 
	argument the pointer to the RDAscrn Structure (RDAscrn *) */
#define free_rsrc(x)	{xfree_rsrc(x,__LINE__,__FILE__);x=NULL;}
/* free_rsrc free's the memory allocated for an RDArsrc Structure.  It has 1
	 argument the pointer to an RDArsrc Structure (RDArsrc *) */
#define updatersrc(x,y)	xupdatersrc(x,y,__LINE__,__FILE__)
/* updatersrc has two arguments, a pointer to an RDArsrc Structure and the 
	resource name to update (char *).  This function updates the Motif 
	value with the value of the specified resource member. */
#define updateallrsrc(x)	xupdateallrsrc(x,__LINE__,__FILE__)
/* updateallrsrc Updates the Motif values with the values in the RDArsrc 
	Structure.  updateallrsrc has 1 arg, a pointer to an RDArsrc Structure 
	(RDArsrc *) */
#define readwidget(x,y)		xreadwidget(x,y,__LINE__,__FILE__)
/* readwidget reads a widgets current Motif value.  readwidget has two 
	arguments a pointer to an RDArsrc Structure (RDArsrc *) and a resource 
	name (char *) */
#define readallwidgets(x)	xreadallwidgets(x,FALSE,FALSE,__LINE__,__FILE__)
#define readallwidgetsADV(x,y,z)	xreadallwidgets(x,y,z,__LINE__,__FILE__)
/* reads all widgets current Motif Values for the resource elements in an 
	RDArsrc Structure.  readallwidgets has 1 arg a pointer to an RDArsrc 
	Structure (RDArsrc *) */
#define writescrnbin(x,y)	xwritescrnbin(x,y,__LINE__,__FILE__)
/* writescrnbin Writes a screen binary for a user's screen definition, the 
	first argument is the library name, followed by the RDAscrn structure 
	to write */
#define ERRORDIALOG(x,y,z,a)	xERRORDIALOG(x,y,z,a,__LINE__,__FILE__)
/* ERRORDIALOG creates an error dialog, arguments are: error name, desc, 
	function to run void (*)(), and either TRUE or FALSE to do an exit(0) 
	after executing the function (previous argment) */

#define FINDRSCGETVISABLE(x,y,z) xFINDRSCGETVISABLE(x,y,z,__LINE__,__FILE__)
#define MEMBERGETVISABLE(x) xMEMBERGETVISABLE(x,__LINE__,__FILE__)
#define FINDRSCGETCURSORPOSITION(x,y)	xFINDRSCGETCURSORPOSITION(x,y,__LINE__,__FILE__)

/*---------------------------------------------------------------------------
RDAwdgts types 
{
	0	-	Standard Resource type 
	1	-	New Line 
	2	-	End Line
	3	-	New Box
	4	-	End Box
	5	-	Label
	6	- 	Button
	7	-	Scrolled List
	8	-	Scrolled Text
	9	-	Toggle Button (Boolean)	
	10 	- 	Frame
	11	-	Seperator
	12	- 	New ScrolledWindow
	13	-	End ScrolledWindow	
	14	-	OptionalScreen
	15	-	Custom Type (Same as Standard Resource Type, except for rtype.)
	16	-	Expenditure Type (Same as Standard Resource Type, except for rtype.)
	17	-	Revenue Type (Same as Standard Resource Type, except for rtype.)
	18	-	Balance Sheet Type (Same as Standard Resource Type, except for rtype.)
	19	-	Beginning Balance Type (Same as Standard Resource Type, except for rtype.)
	20	-	New Tab Container
	21	-	End Tab Container
	22	-	Progress Bar
	23	-	New Tab Bar
	24	-	New Popup Menu
	25	-	End Popup Menu
	26	-	New ToolBar
	27	-	End ToolBar
	28 	-	New Table
	29	-	End Table
	30	-	New Table Header
	31	-	End Table Header
	

}
---------------------------------------------------------------------------*/
/* RDAwdgt Structure - Contains the user's config for a specific resource */
struct RDAwdgts 
{
	short type;   
	char *resource_name;
	int resource_num;
	char *label;
	char *pixmap;
	char *XHTML_Label;
	short rows;
	short cols;
	int rtype;
	char *expression;
	char *editable_expression;
	char *sensitive_expression;
	char *transversal_expression;
	char *required_expression;
	short bootstrap;
	short vertical;
	short horzontal;
};
typedef struct RDAwdgts RDAwdgt;

/* RDAscrn Structure - Contains the user's screen configuration structure */
struct RDAscrns
{
	char *module;
	char *name;
	int numwdgts;
	RDAwdgt *wdgts;
};
typedef struct RDAscrns RDAscrn;

struct RDAconflds
{
	char *fldname;
	char *conname;
	char inherit;
};
typedef struct RDAconflds RDAconfld;

struct RDAScrolledLists
{
	char *module; /* module list is available to */
	char *name; /* name of scrolled list */
	int vtype; /* all or one or more */
	short numincvirs; /* number of files */
	DFincvir *incvir; /* list of files */
	int type; /* 0-from file, 1-parsed list, 2-special 3-computed list*/
	char *select_formula; /* select formula from file */
	char *special_function; /* used only with type 2 */
	short num; /* number of initial conditions */
	RDAconfld *initfld; /* array of initial conditions */
	char *ffield; /* Field from the ffinfo incvir structure that's read */
	short occurrence; /* occurrence of ffinfo */
	DFincvir *ffinfo; /* from file info */
	int contype; /*Connect Types:   0-To Normal Field,
					1-To Special Field,
					2-To RDAvirtual */
	char *conmod; /* connecting module name */
		      /* connecting module without confil & confld when contype
			 is 1 or 2 */
	char *confil; /* connecting file name */
	char *confld; /* connecting field name */
	short con_occurrence; /* connect file occurrence */
	APPlib *dlist; /* list of displayed elements */
	APPlib *list; /* list of elements that can be established */
	char *format_formula; /* from_file uses this expression EVALUATEstr, to
	                         format the elements in the list, special may also
				 use this....*/
	char *unformat_formula; /* used to undo what format_formula did */
	char *desc; /* description of master */
	int timing; /* 0-Initial Only, 1-Initial & Update Virtuals, 2-Update only */
	char *reformat_formula; /* used to evaluate new list value after list made */
	void (*SubFunc)(...);
	void *args;
};
typedef struct RDAScrolledLists RDAScrolledList;

struct RDALoadValues
{
	char *module; /* module list is available to */
	char *name; /* name of scrolled list */
	int type; /* not used yet*/
	short numincvirs; /* number of files */
	DFincvir *incvir; /* list of files */
	char *conname; /* Widget to Connect to and Update */
	APPlib *dlist; /* list of displayed elements */
	char *desc; /* description of master */
	APPlib *mlist; /* list of manual elements */
	void (*SubFunc)(...);
	void *args;
};
typedef struct RDALoadValues RDALoadValue;

/* RDArmem Structure - A structure containing the definition of 1 resource on 
	a resource structure (RDArsrc *) */

/* IF YOU ADD TO THIS STRUCTURE, REMEMBER TO NULL the NEW VARIABLES 
   IN mix/optscrn.c */

struct RDArmems
{
	short type;
	char *rscrname;
	short field_type;
	unsigned field_length;
	union rda_field_types value;
	unsigned dlen;
	char editable;
	Wt::WWidget *w;
	void (*function)(...);
	int items;
	char ***list;
	void *arglist;
	void *parent;
	void (*helpfunction)(...);
	short sensitive;
	short callbacktype;
/* Add this new field */
	int rtype;
	short rows;
	short cols;
	short user_editable;
	char *editable_expression;
	short user_sensitive;
	char *sensitive_expression;
	short function_type; /* 0 for RDArsrc first arg, 1 for RDArmem */
	char app_update;
	char *transversal_expression;
	char *required_expression;
	char *validate_expression;
	char *definition;
	char *label;
	char *XHTML_Label;
	Wt::WToolBar *toolbar_w;
	Wt::WPopupMenu *popup_w;
	int popup_id;
	Wt::WMenuItem *menuitem;
	Wt::WTable *Table;
	Wt::WTableCell *tCell;
	int table_row;
	int table_row_span;
	int table_col;
	int table_col_span;
	void (*table_function)(...);
	void *table_arglist;
	short table_function_type; /* 0 for RDArsrc first arg, 1 for RDArmem */
	Wt::WPushButton *pfd;
	Wt::WValidator *validobject;
	Wt::WStandardItemModel *wSIM;
	short bootstrap;
	short vertical;
	short horzontal;
};
typedef struct RDArmems RDArmem;

/* RDArsrc Structure - A structure containing the Resource's available for a 
	window */
struct RDArsrcs
{
	short (*EvalFunc)(...);
	void *EvalFuncArgs;
	char *module;
	char *screen;
	Wt::WDialog *primary;
	Wt::WContainerWidget *swidget;
	int numrscs;
	RDArmem *rscs;
	int numvir;
	DFincvir *incvir;
	int num;
	RDAvirtual *virflds;
	RDAscrn *scn;
	char *input_focus;
	int numlists;
	RDAScrolledList *lists;
	int numloads;
	RDALoadValue *loads;
	char *(*EvalStr)(...);
	void *EvalStrArgs;
	void (*SubFunc)(...);
	void *SubFuncArgs;
	void (*SetFunc)(...);
	void *SetFuncArgs;
	char modal;
	Wt::WToolBar *window_toolbar;
	char has_tabbar;
	char has_large_table;
};
typedef struct RDArsrcs RDArsrc;

/* ErrorDialog Variables */
struct ErrorDialogArgs
{
	void (*function)(...);
	RDArsrc *rsrc;
	char exit;
};
typedef struct ErrorDialogArgs ErrorDialogArg;

void xINITGUI(int,char **,char *,int,char *);
void xEXITGUI(int,char *);
int xRDAAPPMAINLOOP(int,char *);
short xFINDRSCSHOWVALUE(RDArsrc *,char *,int,char *);
void xSEEMEMBER(RDArmem *,int,char *);
void xSEERSRCS(RDArsrc *,int,char *);
void xSEESCRN(RDAscrn *,int,char *);
RDAscrn *xRDAscrnNEW(char *,char *,int,char *);
RDArsrc *xRDArsrcNEW(char *,char *,int,char *);
short xFINDRSC(RDArsrc *,char *,int,char *);
#define FINDWDGT(a,b)	xFINDWDGT(a,b,__LINE__,__FILE__)
short xFINDWDGT(RDAscrn *,char *,int,char *);
short xFINDRSCSETVALUE(RDArsrc *,char *,void *,int,char *);
short xFINDRSCSETEDITABLE(RDArsrc *,char *,short,int,char *);
short xFINDRSCSETDEFTYPE(RDArsrc *,char *,short,int,char *);
short xFINDRSCSETACCTTYPE(RDArsrc *,char *,short,int,char *);
short xFINDRSCGETDEFTYPE(RDArsrc *,char *,short *,int,char *);
short xFINDRSCGETACCTTYPE(RDArsrc *,char *,short *,int,char *);
short xFINDRSCGETVALUE(RDArsrc *,char *,void *,int,char *);
short xFINDRSCGETSTRING(RDArsrc *,char *,char **,int,char *);
short xFINDRSCSETLIST(RDArsrc *,char *,int,int,char ***,int,char *);
short xFINDRSCLISTAPPlib(RDArsrc *,char *,int,APPlib *,int,char *);
short xFINDRSCSETFUNC(RDArsrc *,char *,void (*)(...),void *,short,int,char *);
int xFINDRSCGETCURSORPOSITION(RDArsrc *,char *,int,char *);
short xMEMBERGETVISABLE(RDArmem *,int,char *);
short xFINDRSCGETVISABLE(RDArsrc *,char *,short *,int,char *);
short xFINDRSCSETCURSOR(RDArsrc *,char *,int,int,char *);
short xwritescrnbin(char *,RDAscrn *,int,char *);
short xupdatersrc(RDArsrc *,char *,int,char *);
#define updatemember(a)	xupdatemember(a,__LINE__,__FILE__)
short xupdatemember(RDArmem *,int,char *);
void xupdateallrsrc(RDArsrc *,int,char *);
void xreadwidget(RDArsrc *,char *,int,char *);
#define readmember(a)	xreadmember(a,__LINE__,__FILE__)
void xreadmember(RDArmem *,int,char *);
void xreadallwidgets(RDArsrc *,short,short,int,char *);
void xfree_scrn(RDAscrn *,int,char *);
void xfree_rsrc(RDArsrc *,int,char *);
void xaddwdgt(RDAscrn *,short,char *,char *,char *,char *,short,short,int,char *,char *,char *,char *,int,char *);
void xaddrsrc(RDArsrc *,short,char *,short,unsigned,void *,char,void (*)(...),short,char ***,void *,void (*)(...),short,int,char *);
#define getscrnbin(a,b,c)	xgetscrnbin(a,b,c,__LINE__,__FILE__)
short xgetscrnbin(char *,RDAscrn *,short,int,char *);
#define makescrn(a,b)	xmakescrn(a,b,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,__LINE__,__FILE__)
short xmakescrn(RDArsrc  *,short,short (*)(...),void *,void (*)(RDArsrc *),char *(*)(...),void *,void (*)(...),void *,RDArsrc  *,int,char *);
void xERRORDIALOG(char *,char *,void (*function)(...),char,int,char *);

struct DEFfields
{
	char *name;/*name of field*/
	short type;/*type of field*/
	short reset; /* reset default value */
	union rda_field_types data;
	int items;
};
typedef struct DEFfields DEFfield;

struct RDAdefaults
{
	char *module;
	char *name;
	char *input_focus;
	int num;
	DEFfield *field;
};
typedef struct RDAdefaults RDAdefault;

struct Hold2Rsrcs
{
	RDArsrc *rsrc1;
	RDArsrc *rsrc2;
	APPlib *lib;
	void *v;
};
typedef struct Hold2Rsrcs Hold2Rsrc;

#define RDAdefaultNEW(x,y)	xRDAdefaultNEW(x,y,__LINE__,__FILE__)
RDAdefault *xRDAdefaultNEW(char *,char *,int,char *);
#define FreeRDAdefault(x)	{xFreeRDAdefault(x,__LINE__,__FILE__);x=NULL;}
void xFreeRDAdefault(RDAdefault *,int,char *);
#define writedefaultbin(x,y)	xwritedefaultbin(x,y,__LINE__,__FILE__)
short xwritedefaultbin(char *,RDAdefault *,int,char *);
#define SEEDEFAULTS(x)	xSEEDEFAULTS(x,__LINE__,__FILE__)
void xSEEDEFAULTS(RDAdefault *,int,char *);
#define getdefaultbin(a,b,c,d)	xgetdefaultbin(a,b,c,d,__LINE__,__FILE__)
short xgetdefaultbin(char *,char *,RDAdefault *,short,int,char *);
#define GetDefaults(a)	xGetDefaults(a,__LINE__,__FILE__)
RDAdefault *xGetDefaults(RDArsrc *,int,char *);
#define finddefaultbin(a,b)	xfinddefaultbin(a,b,__LINE__,__FILE__)
RDAdefault *xfinddefaultbin(char *,char *,int,char *);
#define ADVDefaultResetScreen(a,b,c)	xDefaultScreens(a,b,c,TRUE,__LINE__,__FILE__)
#define DefaultResetScreen(a,b)	xDefaultScreens(a,b,NULL,FALSE,__LINE__,__FILE__)
#define DefaultScreens(a)	xDefaultScreens(a,FALSE,NULL,FALSE,__LINE__,__FILE__)
short xDefaultScreens(RDArsrc *,short,char **,short,int,char *);
#define RDAacctNEW(x)	xRDAacctNEW(x,__LINE__,__FILE__)
RDAacctmstr *xRDAacctNEW(short,int,char *);

#define FreeRDAacctmstr(x)	{xFreeRDAacctmstr(x,__LINE__,__FILE__);x=NULL;}
void xFreeRDAacctmstr(RDAacctmstr *,int,char *);
#define FreeRDAacctmstr2(x)	xFreeRDAacctmstr2(x,__LINE__,__FILE__)
void xFreeRDAacctmstr2(RDAacctmstr **,int,char *);
#define FreeRDAacct(x)	xFreeRDAacct(x,__LINE__,__FILE__)
void xFreeRDAacct(RDAacct **,int,char *);
#define FreeRDAaccpart(x)	xFreeRDAaccpart(x,__LINE__,__FILE__)
void xFreeRDAaccpart(RDAaccpart **,int,char *);
#define getacctbin(x)	xgetacctbin(x,__LINE__,__FILE__)
short xgetacctbin(RDAacctmstr *,int,char *);
#define writeacctbin(x)	xwriteacctbin(x,__LINE__,__FILE__)
short xwriteacctbin(RDAacctmstr *,int,char *);
#define EXITGUI()	xEXITGUI(__LINE__,__FILE__)
void xEXITGUI(int,char *);
#define killwindow(x)	xkillwindow(x,__LINE__,__FILE__)
void xkillwindow(RDArsrc *,int,char *);
#define MEMBERSETSENSITIVE(a,b)	xMEMBERSETSENSITIVE(a,b,0,__LINE__,__FILE__)
#define ADVMEMBERSETSENSITIVE(a,b,c)	xMEMBERSETSENSITIVE(a,b,c,__LINE__,__FILE__)
short xMEMBERSETSENSITIVE(RDArmem *,short,short,int,char *);
#define FINDRSCSETSENSITIVE(a,b,c)	xFINDRSCSETSENSITIVE(a,b,c,__LINE__,__FILE__)
short xFINDRSCSETSENSITIVE(RDArsrc *,char *,short,int,char *);
#define FINDRSCSETSENSITIVE(a,b,c) xFINDRSCSETSENSITIVE(a,b,c,__LINE__,__FILE__) 
short xMEMBERGETEDITABLE(RDArmem *,short *,int,char *);
#define MEMBERGETEDITABLE(a,b)  xMEMBERGETEDITABLE(a,b,__LINE__,__FILE__)
short xFINDRSCGETEDITABLE(RDArsrc *,char *,short *,int,char *);
#define FINDRSCGETEDITABLE(a,b,c)  xFINDRSCGETEDITABLE(a,b,c,__LINE__,__FILE__)
short xMEMBERGETSENSITIVE(RDArmem *,short *,int,char *);
#define MEMBERGETSENSITIVE(a,b)  xMEMBERGETSENSITIVE(a,b,__LINE__,__FILE__)
short xFINDRSCGETSENSITIVE(RDArsrc *,char *,short *,int,char *);
#define FINDRSCGETSENSITIVE(a,b,c)  xFINDRSCGETSENSITIVE(a,b,c,__LINE__,__FILE__)
short checkforinterrupt(RDArsrc *);
#define ForceWindowUpdate(a)	xForceWindowUpdate(a,__LINE__,__FILE__)
void xForceWindowUpdate(RDArsrc *,int,char *);

#define TimeoutCursors(a,b)	xTimeoutCursors(a,b,__LINE__,__FILE__)
void xTimeoutCursors(RDArsrc *,int,int,char *);
#define findscrnbin(a)	xfindscrnbin(a,__LINE__,__FILE__)
short xfindscrnbin(RDAscrn *,int,char *);
void NULL_RSCS(RDArsrc *);
#define FINDRSCSETINPUTFOCUS(a,b)	xFINDRSCSETINPUTFOCUS(a,b,__LINE__,__FILE__)
short xFINDRSCSETINPUTFOCUS(RDArsrc *,char *,int,char *);
#define DeActivateLine(a,b)	xDeActivateLine(a,b,__LINE__,__FILE__)
int xDeActivateLine(RDAscrn *,int,int,char *);
#define DeActivateBox(a,b)	xDeActivateBox(a,b,__LINE__,__FILE__)
int xDeActivateBox(RDAscrn *,int,int,char *);
#define DeActivatePopup(a,b)	xDeActivatePopup(a,b,__LINE__,__FILE__)
int xDeActivatePopup(RDAscrn *,int,int,char *);
#define DeActivateToolbar(a,b)	xDeActivateToolbar(a,b,__LINE__,__FILE__)
int xDeActivateToolbar(RDAscrn *,int,int,char *);
#define DeActivateScrolledWindow(a,b)	xDeActivateScrolledWindow(a,b,__LINE__,__FILE__)
int xDeActivateScrolledWindow(RDAscrn *,int,int,char *);
#define DeActivateTabContainer(a,b)	xDeActivateTabContainer(a,b,__LINE__,__FILE__)
int xDeActivateTabContainer(RDAscrn *,int,int,char *);
#define ADVMEMBERSETEDITABLE(a,b,c)	xMEMBERSETEDITABLE(a,b,c,__LINE__,__FILE__)
#define MEMBERSETEDITABLE(a,b)	xMEMBERSETEDITABLE(a,b,FALSE,__LINE__,__FILE__)
short xMEMBERSETEDITABLE(RDArmem *,short,short,int,char *);
#define FINDFIELDDEFAULT(a,b)	xFINDFIELDDEFAULT(a,b,__LINE__,__FILE__)
DEFfield *xFINDFIELDDEFAULT(RDAdefault *,char *,int,char *);
short xRSRCCOPY(RDArsrc *,char *,RDArsrc *,char *,int,char *);
#define COPYRSRC(a,b,c) xRSRCCOPY(a,c,b,c,__LINE__,__FILE__)
#define COPYRSRC2RSRC(a,b,c) xRSRCCOPY(a,b,a,c,__LINE__,__FILE__)

#define FINDRSCSETSTRING(x,y,z)	xFINDRSCSETSTRING(x,y,z,__LINE__,__FILE__)
short xFINDRSCSETSTRING(RDArsrc *,char *,char *,int,char *);
#define FINDRSCSETCHAR(x,y,z)	xFINDRSCSETCHAR(x,y,z,__LINE__,__FILE__)
short xFINDRSCSETCHAR(RDArsrc *,char *,char,int,char *);
#define FINDRSCSETSHORT(x,y,z)	xFINDRSCSETSHORT(x,y,z,__LINE__,__FILE__)
short xFINDRSCSETSHORT(RDArsrc *,char *,short,int,char *);
#define FINDRSCSETINT(x,y,z)	xFINDRSCSETINT(x,y,z,__LINE__,__FILE__)
short xFINDRSCSETINT(RDArsrc *,char *,int,int,char *);
#define FINDRSCSETDOUBLE(x,y,z)	xFINDRSCSETDOUBLE(x,y,z,__LINE__,__FILE__)
short xFINDRSCSETDOUBLE(RDArsrc *,char *,double,int,char *);

#define FINDRSCGETCHAR(x,y,z)	xFINDRSCGETCHAR(x,y,z,__LINE__,__FILE__)
short xFINDRSCGETCHAR(RDArsrc *,char *,char *,int,char *);
#define FINDRSCGETSHORT(x,y,z)	xFINDRSCGETSHORT(x,y,z,__LINE__,__FILE__)
short xFINDRSCGETSHORT(RDArsrc *,char *,short *,int,char *);
#define FINDRSCGETINT(x,y,z)	xFINDRSCGETINT(x,y,z,__LINE__,__FILE__)
short xFINDRSCGETINT(RDArsrc *,char *,int *,int,char *);
#define FINDRSCGETDOUBLE(x,y,z)	xFINDRSCGETDOUBLE(x,y,z,__LINE__,__FILE__)
short xFINDRSCGETDOUBLE(RDArsrc *,char *,double *,int,char *);
#define FINDRSCSETCALLBACKTYPE(x,y,z)	xFINDRSCSETCALLBACKTYPE(x,y,z,__LINE__,__FILE__)
short xFINDRSCSETCALLBACKTYPE(RDArsrc *,char *,short,int,char *);
#define MergeImage(a,b,c,d)	xMergeImage(a,b,c,d,__LINE__,__FILE__)
char *xMergeImage(char *,char *,short,short,int,char *);
void SAVE_RSRC_BINARY(RDArsrc *);

#define IsScreenDisplayed(x)	((x->swidget!=NULL) || (x->primary!=NULL))
#define IsWidgetDisplayed(x)	(x->w!=NULL)

short checkdiff(RDArmem *,RDArmem *);
char *fullacctdef(RDAacct *); /* Pass RDAacct structure to get the full account str*/
char *fullacctlabel(RDAacct *); /* Pass RDAacct structure to get the full account label*/
#define FreeRDAScrolledList(a)	{xFreeRDAScrolledList(a,__LINE__,__FILE__);a=NULL;}
void xFreeRDAScrolledList(RDAScrolledList *,int,char *);
#define SetAllEditableAndSensitive(a)	xSetAllEditableAndSensitive(a,__LINE__,__FILE__)
void xSetAllEditableAndSensitive(RDArsrc *,int,char *);
#define SetAllListEditableAndSensitive(a)	xSetAllListEditableAndSensitive(a,__LINE__,__FILE__)
void xSetAllListEditableAndSensitive(RDArsrc *,int,char *);
#define ExecuteRDArmemFunction(a)	xExecuteRDArmemFunction(a,__LINE__,__FILE__)
void xExecuteRDArmemFunction(RDArmem *,int,char *);
#define ComputeNewTransversal(a,b,c,d,e)	xComputeNewTransversal(a,b,NULL,c,d,e,__LINE__,__FILE__)
#define ComputeNewTransversalADV(a,b,c,d,e,f)	xComputeNewTransversal(a,b,c,d,e,f,__LINE__,__FILE__)
void xComputeNewTransversal(RDArsrc *,char *(*)(...),void *,void (*)(...),void *,char *,int,char *);
void xSETTITLE(RDArsrc *,char *,int,char *);
#define SETTITLE(a,b)	xSETTITLE(a,b,__LINE__,__FILE__)
extern void (*PP_translate_GUIFUNC)(char **);
extern void (*PP_translate_GUIFUNCALL)(RDArsrc *);
extern int (*CustomTableFunction)(...);
extern char *(*RDA_AutoComplete)(char *,char *,char *,char *);
extern void (*RDA_CaptureAutoComplete)(char *,char *,char *,char *);
void TableCopied(WTable *);
void TablePasted(WTable *);


#define FreeRDALoadValue(a)	{xFreeRDALoadValue(a,__LINE__,__FILE__);a=NULL;}
void xFreeRDALoadValue(RDALoadValue *,int,char *);
#define SetAllLoadsEditableAndSensitive(a)	xSetAllLoadsEditableAndSensitive(a,__LINE__,__FILE__)
void xSetAllLoadsEditableAndSensitive(RDArsrc *,int,char *);
#define MEMBERSETDEFTYPE(a,b)	xMEMBERSETDEFTYPE(a,b,__LINE__,__FILE__)
short xMEMBERSETDEFTYPE(RDArmem *,short,int,char *);
#define MEMBERSETACCTTYPE(a,b)	xMEMBERSETACCTTYPE(a,b,__LINE__,__FILE__)
short xMEMBERSETACCTTYPE(RDArmem *,short,int,char *);
extern RDArmem *NoLosingFocus;
void SetupStyleSheet(Wt::WTextEdit *);
#define FINDRSCGETTABLECOLUMN(a,b)	xFINDRSCGETTABLECOLUMN(a,b,__LINE__,__FILE__)
short xFINDRSCGETTABLECOLUMN(RDArsrc *,char *,int,char *);
#define FINDRSCGETTABLEROW(a,b)	xFINDRSCGETTABLEROW(a,b,__LINE__,__FILE__)
short xFINDRSCGETTABLEROW(RDArsrc *,char *,int,char *);
#define FINDRSCSETTABLEFUNC(a,b,c,d,e)	xFINDRSCSETTABLEFUNC(a,b,c,d,e,__LINE__,__FILE__)
short xFINDRSCSETTABLEFUNC(RDArsrc *,char *,void (*)(...),void *,short,int,char *);
#define FINDRSCSETINTPUTFOCUS(a,b)	xFINDRSCSETINPUTFOCUS(a,b,__LINE__,__FILE__)
short xFINDRSCSETINPUTFOCUS(RDArsrc *,char *,int,char *);

#define OpenRDAWiki(a)	xOpenRDAWiki(a,__LINE__,__FILE__)
void xOpenRDAWiki(char *,int,char *);
void WikiSupport(RDArsrc *);
void ClearRootContainer(void);
void RDArsrcKeyPressed(RDArsrc *,Wt::WKeyEvent &);
void RDArmemKeyPressed(RDArmem *,Wt::WKeyEvent &);
char *CreateRegExpString(char *);

/* RDAmenu Structure - Contains the name and command to run for a menu item */
struct RDAmenus
{
	char *name;
	char *command;
};
typedef struct RDAmenus RDAmenu;

#define RDAmenuNEW(x,y)	xRDAmenuNEW(x,y,__LINE__,__FILE__)
/* RDAmenuNEW Initailly allocates memory for an RDAmenu Structure.  It has two 
	arguments, the Module Name and Item Name, both are char *'s */
#define free_menu(x)	{xfree_menu(x,__LINE__,__FILE__);x=NULL;}
/* free_menu free's the memory allocated for an RDAmenu Structure.  It has 1 
	argument, the pointer to the RDAmenu Structure (RDAmenu *) */
#define writemenubin(x,y)	xwritemenubin(x,y,__LINE__,__FILE__)
/* writemenubin, Writes a menu structure to a menu binary library, first 
	argument menu binary library, second RDAmenu structure */ 
RDAmenu *xRDAmenuNEW(char *,char *,int,char *);
short xwritemenubin(char *,RDAmenu *,int,char *);
void xfree_menu(RDAmenu *,int,char *);
#define getmenubin(a,b)	xgetmenubin(a,b,__LINE__,__FILE__)
short xgetmenubin(char *,RDAmenu *,int,char *);
double GUIstripfamt(char *);
int WT_RDA_PutEnv(char *,int,char *);
int WT_RDA_UnSetEnv(char *,int,char *);
int WT_RDA_SetEnv(char *,char *,int,char *);
char *WT_RDA_GetEnv(char *,int,char *);
char *WT_GetDocRoot(void);
char *WT_GetAppRoot(void);
void GUI_OpenWindow(char *);
void GUI_OpenWindowWait(char *);
char *WtPassWdHash(char *);
char *DetermineMimeType(char *);
void DisplayFile(char *);
void DisplayRelativeFile(char *);
#define UTF8famt(a,b)	xUTF8famt(a,b,__LINE__,__FILE__)
char *xUTF8famt(double,int,int,char *);
void GUI_OpenURL(char *);
char *RDA_EncodeWhiteSpace(char *);

#endif /* GUI_HPP */
