/*rptgen.h - Report Generator Private Header */
#ifndef RPTH_D
#define RPTH_D

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <math.h>
#include <fcntl.h>
#include <ctime>


#ifdef SCO386
#include <sys/stat.h>
#endif
#ifdef RS6000
#include <sys/stat.h>
#endif


#include <mix.hpp>
#include <fin.hpp>
#include <pay.hpp>
#include <venpmt.hpp>
#include <appmgt.hpp>
#include <xpert.hpp>
#include <purord.hpp>
#include <evaluate.hpp>
#include <olh.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <nrd.hpp>
#include <gui.hpp>
#include <misc.hpp>

#define FIELDTYPE	0
#define DEVICECONTROL	1
#define ASCENDING		0	/* Ascending Sequence */
#define ASCENDLOWER		1	/* Ascending Sequence Lower Case Translation */
#define ASCENDUPPER		2	/* Ascending Sequence Upper Case Translation */
#define DESCENDING		3	/* Descending Sequence */
#define DESCENDLOWER	4	/* Descending Sequence Lower Case Translation */
#define DESCENDUPPER	5	/* Descending Sequence Upper Case Translation */

struct RDAprints
{
	short type; 
	short length;
	short numdecimals;
	short fieldtype;
};
typedef struct RDAprints RDAprint;

struct RDAsorts
{
	int type;
	char *name;
};
typedef struct RDAsorts RDAsort;
struct RDAcontrols
{
	char *name;
	short fieldtype;
	union rda_field_types value;
	unsigned dlen;
};
typedef struct RDAcontrols RDAcontrol;
struct RDAaccvals
{
	char *breakname;
	int count;
	union rda_field_types value;
	unsigned dlen;
	union rda_field_types prev_value;
	unsigned pdlen;
};
typedef struct RDAaccvals RDAaccval;
struct RDAaccums
{
	char *name;
	int atype;
	char *accname;
	short fieldtype;
	char displayed;
	unsigned len;
	int num;
	RDAaccval *accvals;
};
typedef struct RDAaccums RDAaccum;
struct RDAdisplays 
{
	double page_width;
	double page_length;
	short date_order; /* date_order will be used as print test page boolean */
	short first_page_hdr;
	short last_page_ftr;
	short device;
	short body_count;
	char body_count_type;
	short test_pages;
	short starting_page;
	char *set_lpp;
	int num;
	RDApvar *pvars;
	char dumprinter;
	short dumprinter_type;
	char extra_formfeed;
	char count_invisible;
	char doscopy;
	char *device_spec;
	char set_lpp_eval;
};
typedef struct RDAdisplays RDAdisplay;
struct RDApvals
{
	short symbol_type;
	char *name;
	int type;
	char *fieldname;
};
typedef struct RDApvals RDApval;
struct RDAimages
{
	short type;
	char *breakname;
	char *line;
	short test;
	char *expression;
	int numpvals;
	RDApval *pvals;
};
typedef struct RDAimages RDAimage;

struct RDAreports 
{
	char type;
	char *name;
	char *title;
	char *desc;
	short engine_type;
	short stop_on_error;
	short process_exceptions;
	short use_print_screen;
	char *server_name;
	char manual_form;
	char report_completion;
	short ask_b4_execute;
	short chain_report;
	char *chain_module;
	char *chain_name;
	short range_screen;
	short diag_breakable;
	short remove_file;
	char *remove_name;
	short removesort;
	int numvirtuals;
	RDAvirtual *virflds;
	int numsearchs;
	RDAsearch *search;
	char *select;
	int numsorts;
	RDAsort *sort;	
	int numcontrols;
	RDAcontrol *control;
	int numaccums;
	RDAaccum *accum;
	RDAdisplay *display;
	int numimages;
	RDAimage *image;
	int currbrk;
	int reporttype;
	short no_empty_error;
	short no_wc_diag;
	short writecount_only;
	short write_all_detail_levels;
	MaintainMaster *MTNMSTR; /* Process Exceptions Maintain */
};
typedef struct RDAreports RDAreport;

struct RDAezsrts
{
	char *name;
	short total;
	short start;
	short length;
	short type;
	char range;
	union rda_field_types from;
	union rda_field_types to;
};
typedef struct RDAezsrts RDAezsrt;

struct RDAezdtls
{
	char *title1;
	char *title2;
	short start;
	short length;
	short type;
	char range;
	short fieldtype;
	char *fieldname;
	char *expression;
};
typedef struct RDAezdtls RDAezdtl;

struct RDAezrpts
{
	char *name;
	char *title;
	char *desc;
	char *majmod;
	char *majfil;
	char *devicename;
	char *printsize;
	short print;
	short account_report;
	char skip_finmgt;
	short num;
	short alignment_test;
	double page_length;
	DFincvir *incvir;
	int numsrt;
	RDAezsrt *sort;
	int numdtl;
	RDAezdtl *detail;
	char range_screen;
};
typedef struct RDAezrpts RDAezrpt;

struct RUNfiles
{
	int type;
	char *module;
	char *filename;
	char *keyname;
	short fileno;
	short keyno;
	int num_sup;
	void *sup_detail;
	int num;
	void *child;
	short range;
	short num_writes;
	char *expression;
	char *skip_detail;
	short wc_eval;
	char *wc_expression;
	short wc_skip;
	char *skip_detail_error;
	short wrote_first;
	short write_count;
	void *parent;
};
typedef struct RUNfiles RUNfile;
struct Reportfiles
{
	char *module;
	char *filename;
	short fileno;
	short range;
};
typedef struct Reportfiles ReportFile;

struct RDArunrpts
{
	char *module;
	short exit_status;
	short fatal_errors;
	int numfiles;
	ReportFile *files;
	int numprims;
	RUNfile *prims;
	char *sortserver;
	char *sortfile;
	short sortno;
	short errors_encountered;
	APPlib *errorlist;
	short nofields;
	char **fnames;
	NRDfield *fields;
	short nokeys;
	NRDkey *keys;
	int page_count;
	int numlines;
	int horizontal_count;
	int numhorizontals;
	int pageno;
	int page_width;
	int body_count;
	char *curfont;
	short curpitch;
	short curpoints;
	short pitch;
	short points;
	char *longfont;
	FILE *fp;
	RDA_PFILE *pfile;
	int numofiles;
	ReportFile *ofiles;
	RDArsrc *rsrsrc;
	RDArsrc *prtrsrc;
	RDArsrc *alignrsrc;
	RDArsrc *diagrsrc;
	RDArsrc *exception;
	RDArsrc *mainrsrc;
	RDArsrc *definelist;
	RDArsrc *searchrsrc;
	RDArsrc *askb4exec;
	RDArsrc *quitfuncrsrc;
	void (*quitfunc)(...);
	void *quitfuncArgs;
	MakeBrowseList *mbl;
	short APPmainLoop;
	long record_count;
	long total_count;
	short forced;
	short startline;
	short primary_file_no;
	int screen_count;
	RangeScreen *RScreen;
	short write_count;
	char *cur_brk_name;
	char *prev_brk_name;
	char *prev_brk_name2;
	char range_page;
	int from_page;
	int to_page;
	short do_process_exceptions;
	APPlib *procexcept_types;
	RUNfile *cur_runfile;
	short detail_count;
	APPlib *dumprttypes;
	int level;
	void (*ProcDataLine)(...);
	char no_pe_accums;
	char archive;
	FILE *fp_pdf;
	char print_range_criteria;
	char *range_desc;
	void *TableWidget;
	RDArmem *lastwidget;
	int table_row;
	char UsingRow[101];
	int page;
	int total_pages;
};
typedef struct RDArunrpts RDArunrpt;

struct HoldReports
{
	RDArunrpt *rrpt;
	RDAreport *rpt;
	RDArsrc *mtnrsrc;
};
typedef struct HoldReports HoldReport;

struct EZfields
{
	char *name;
	short type;
	short len;
	char *wholename;
};
typedef struct EZfields EZfield;

#define RDAreportNEW(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o)	xRDAreportNEW(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,NULL,FALSE,FALSE,FALSE,FALSE,__LINE__,__FILE__)
#define RDAreportNEWADV(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)	xRDAreportNEW(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,FALSE,FALSE,FALSE,FALSE,__LINE__,__FILE__)
#define RDAreportNEWADV2(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r)	xRDAreportNEW(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,FALSE,FALSE,__LINE__,__FILE__)
#define RDAreportNEWADV3(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t)	xRDAreportNEW(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,__LINE__,__FILE__)
RDAreport *xRDAreportNEW(char *,char *,char *,short,char *,short,short,
	short,short,short,char *,char *,short,short,short,char *,short,short,
	short,short,int,char *);
short makesortfile(RDArunrpt *,RDAreport *);
short FIELDReportGenSubData1(char **,HoldReport *,char *);
short VIRTUALReportGenSubData1(char **,HoldReport *,char *);
short SORTReportGenSubData1(char **,HoldReport *,char *);
short ACCUMReportGenSubData1(char **,HoldReport *,char *);
void ReportGenSubData(char **,HoldReport *);
void ReportGenSubData1(char **,HoldReport *);
void RPTMTNSubData(char **,MTNPassableStruct *);
short compute_virtual_field(RDArunrpt *,RDAreport *,char *,void (*)(...),void *);
void compute_remaining_virtuals(RDArunrpt *,RDAreport *);
void SetFieldValue(NRDfield *,RDAreport *,RDArunrpt *);
short writesortrecord(RDArunrpt *,RDAreport *,RUNfile *);
short evaluate_records(RDArunrpt *,RDAreport *,RUNfile *);
short READsubfiles(RUNfile *,RDArunrpt *,RDAreport *);
#define READRecords(x,y)	xREADRecords(x,y,__LINE__,__FILE__)
short xREADRecords(RDArunrpt *,RDAreport *,int,char *);
#define RDArunrptNEW(x,y,z,a,b)	xRDArunrptNEW(x,y,z,a,b,NULL,NULL,NULL,1,__LINE__,__FILE__)
#define RDArunrptNEWADV(x,y,z,a,b,c,d,e)	xRDArunrptNEW(x,y,z,a,b,c,d,e,1,__LINE__,__FILE__)
#define RDArunrptNEWADV2(x,y,z,a,b,c,d,e,f)	xRDArunrptNEW(x,y,z,a,b,c,d,e,f,__LINE__,__FILE__)
RDArunrpt *xRDArunrptNEW(char *,char *,char *,short,short,void (*)(...),RDArsrc *,void *,int,int,char *);
#define FreeRDAreport(x)	{xFreeRDAreport(x,__LINE__,__FILE__);x=NULL;}
void xFreeRDAreport(RDAreport *,int,char *);
void FREEReportfiles(RDArunrpt *);
#define FreeRDArunrpt(x)	{xFreeRDArunrpt(x,__LINE__,__FILE__);x=NULL;}
void xFreeRDArunrpt(RDArunrpt *,int,char *);
#define GETRDAreportEXE(x,y)	xGETRDAreport(x,y,FALSE,__LINE__,__FILE__)
#define GETRDAreport(x,y)	xGETRDAreport(x,y,TRUE,__LINE__,__FILE__)
RDAreport *xGETRDAreport(char *,char *,short,int,char *);
#define WRTRDAreport(x,y)	xWRTRDAreport(x,y,__LINE__,__FILE__)
short xWRTRDAreport(char *,RDAreport *,int,char *);
short addReportFile(RDArunrpt *,char *,char *,short,short,short);
short OPNsubfiles(RUNfile *,RDArunrpt *);
short OPNReportfiles(RDArunrpt *);
#define CLSReportfiles(a,b)	xCLSReportfiles(a,b,__LINE__,__FILE__)
void xCLSReportfiles(RDArunrpt *,short,int,char *);
short fieldexists(int,NRDfield *,char *);
short addfield(RDArunrpt *,char *,short,short);
short addsubfile(RDAsearch *,RUNfile *,int);
short definefiles(RDAreport *,RDArunrpt *);
short SHOWrunfiles(RDArunrpt *);
void process_dataline(RDArunrpt *,RDAreport *,RDAimage *,short);
#define RUNREPORT(x,y,z,a,b,c,d)	xRUNREPORT(x,y,z,a,b,c,d,NULL,NULL,NULL,0,(void (*)(...))process_dataline,__LINE__,__FILE__)
#define RUNREPORTADV(x,y,z,a,b,c,d,e,f,g)	xRUNREPORT(x,y,z,a,b,c,d,e,f,g,0,(void (*)(...))process_dataline,__LINE__,__FILE__)
#define RUNREPORTADV2(x,y,z,a,b,c,d,e,f,g,h)	xRUNREPORT(x,y,z,a,b,c,d,e,f,g,h,(void (*)(...))process_dataline,__LINE__,__FILE__)
#define RUNREPORTADV3(x,y,z,a,b,c,d,e,f,g,h,i)	xRUNREPORT(x,y,z,a,b,c,d,e,f,g,h,i,__LINE__,__FILE__)
void xRUNREPORT(char *,char *,char *,char *,short,short,RDArsrc *,void (*)(...),RDArsrc *,void *,int,void (*)(...),int,char *);
#define testform(x,y,z,a)	xtestform(x,y,z,a,__LINE__,__FILE__)
short xtestform(RDArunrpt *,RDAreport *,short,short,int,char *);
short getlinecomponent(char **,char **,RDArunrpt *,RDAreport *,RDAimage *,short);
void print_font(RDArunrpt *,RDAreport *,char *,char **);
#define interpretline(x,y,z,a,b)	xinterpretline(x,y,z,a,b,__LINE__,__FILE__)
short xinterpretline(RDArunrpt *,RDAreport *,RDAimage *,short,short,int,char *);
#define print_line(a,b,c,d,e)	xprint_line(a,b,c,d,e,__LINE__,__FILE__)
short xprint_line(RDArunrpt *,RDAreport *,RDAimage *,short,short,int,char *);
#define print_section(a,b,c,d)	xprint_section(a,b,c,d,__LINE__,__FILE__)
short xprint_section(RDArunrpt *,RDAreport *,char *,short,int,char *);
#define ReportBackEnd(x,y)	xReportBackEnd(x,y,__LINE__,__FILE__)
void xReportBackEnd(RDArunrpt *,RDAreport *,int,char *);
void quitreport(RDArsrc *,HoldReport *);
void quitreport1(RDArsrc *,HoldReport *);
void executereport(RDArsrc *,HoldReport *);
void executereport1(RDArsrc *,HoldReport *);
void SetFieldValue1(NRDfield *,char *,RDAreport *,RDArunrpt *);
#define CLSReportfiles(a,b)	xCLSReportfiles(a,b,__LINE__,__FILE__)
void xCLSReportfiles(RDArunrpt *,short,int,char *);
#ifdef XXX
#define FREEsubfiles(x)	{xFREEsubfiles(x,__LINE__,__FILE__);x=NULL;}
#endif
#define FREEsubfiles(x)	xFREEsubfiles(x,__LINE__,__FILE__)
void xFREEsubfiles(RUNfile *,int,char *);
#define FREEReportfiles(x)	xFREEReportfiles(x,__LINE__,__FILE__)
void xFREEReportfiles(RDArunrpt *,int,char *);
#define SEERDAreport(x,y)	xSEERDAreport(x,y,__LINE__,__FILE__)
void xSEERDAreport(RDAreport *,short,int,char *);
void printreport(RDArsrc *,RDAreport *);
short PrintLineSubData(char **,char *,HoldReport *);
#define HoldReportNew(a,b)	xHoldReportNew(a,b,__LINE__,__FILE__)
HoldReport *xHoldReportNew(RDArunrpt *,RDAreport *,int,char *);
#define REPORTFILENUMBER(a,b,c,d,e)	xREPORTFILENUMBER(a,b,c,d,e,__LINE__,__FILE__)
short xREPORTFILENUMBER(RDArunrpt *,char *,char *,short,short,int,char *);
#define PageFooterSize(x,y)	xPageFooterSize(x,y,__LINE__,__FILE__)
int xPageFooterSize(RDArunrpt *,RDAreport *,int,char *);
#define addsortfield(a,b,c,d,e)	xaddsortfield(a,b,c,d,e,__LINE__,__FILE__)
short xaddsortfield(RDArunrpt *,RDAreport *,char *,char *,int,int,char *);
char *getreporttitle(char *,char *);
char *getreportdesc(char *,char *);
void process_exceptions(RDArunrpt *,RDAreport *);
void process_exceptions2(HoldReport *);
void runprtscreen(HoldReport *);
void setvirtualrangecallbacks(RDAreport *,RDArsrc *);
void setvirtualdeftype(RDArsrc *,RDArmem *);
void setvirtualdeftypefrom(RDArsrc *,RDArmem *);
void setvirtualacctype(RDArsrc *,RDArmem *);
void setvirtualdeftypeto(RDArsrc *,RDArmem *);
void setvirtualacctypefrom(RDArsrc *,RDArmem *);
void setvirtualacctypeto(RDArsrc *,RDArmem *);
RDAezrpt *RDAezrptNEW(char *);
#define FreeRDAezrpt(a)	{xFreeRDAezrpt(a,__LINE__,__FILE__);a=NULL;}
void xFreeRDAezrpt(RDAezrpt *,int line,char *file);
#define GETRDAezrpt(a,b)	xGETRDAezrpt(a,b,__LINE__,__FILE__)
RDAezrpt *xGETRDAezrpt(char *,char *,int,char *);
#define WRTRDAezrpt(a,b)	xWRTRDAezrpt(a,b,__LINE__,__FILE__)
short xWRTRDAezrpt(char *,RDAezrpt *,int,char *);
#define getezrpttitle(a,b)	xgetezrpttitle(a,b,__LINE__,__FILE__)
char *xgetezrpttitle(char *,char *,int,char *);
#define getezrptdesc(a,b)	xgetezrptdesc(a,b,__LINE__,__FILE__)
char *xgetezrptdesc(char *,char *,int,char *);
#define addvirtualline(a,b,c,d,e,f)	xaddvirtualline(a,b,c,d,e,f,0,0,__LINE__,__FILE__)
#define addvirtuallineADV(a,b,c,d,e,f,g)	xaddvirtualline(a,b,c,d,e,f,g,0,__LINE__,__FILE__)
#define addvirtuallineADV1(a,b,c,d,e,f,g,h)	xaddvirtualline(a,b,c,d,e,f,g,h,__LINE__,__FILE__)
int xaddvirtualline(RDAreport *,char *,int,short,char *,short,short,char,int,char *);
#define addcontrolline(a,b,c)	xaddcontrolline(a,b,c,__LINE__,__FILE__)
int xaddcontrolline(RDAreport *,char *,short,int,char *);
#define addsortline(a,b,c)	xaddsortline(a,b,c,__LINE__,__FILE__)
int xaddsortline(RDAreport *,int,char *,int,char *);
#define addsearchfile(a,b,c,d,e,f,g,h,i,j)		xaddsearchline(a,b,c,d,e,f,0,NULL,g,h,i,NULL,j,0,NULL,0,NULL,__LINE__,__FILE__)
#define addsearchfileADV(a,b,c,d,e,f,g,h,i,j,k)		xaddsearchline(a,b,c,d,e,f,0,NULL,g,h,i,j,k,0,NULL,0,NULL,__LINE__,__FILE__)
#define addsearchfileADV2(a,b,c,d,e,f,g,h,i,j,k,l,m)		xaddsearchline(a,b,c,d,e,f,0,NULL,g,h,i,j,k,l,m,0,NULL,__LINE__,__FILE__)
#define addsearchline(a,b,c,d,e,f,g,h,i,j,k)		xaddsearchline(a,b,c,d,e,f,g,h,i,j,k,NULL,1,0,NULL,0,NULL,__LINE__,__FILE__)
#define addsearchlineADV(a,b,c,d,e,f,g,h,i,j,k,l,m)	xaddsearchline(a,b,c,d,e,f,g,h,i,j,k,l,m,0,NULL,0,NULL,__LINE__,__FILE__)
#define addsearchlineADV2(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o)	xaddsearchline(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,0,NULL,__LINE__,__FILE__)
#define addsearchlineADV3(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q)	xaddsearchline(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,__LINE__,__FILE__)
int xaddsearchline(RDAreport *,char *,char *,int,char *,char *,short,RDAconfld *,char *,short,char *,char *,short,short,char *,short,char *,int,char *);
#define addaccumline(a,b,c,d,e)	xaddaccumline(a,b,c,d,e,__LINE__,__FILE__)
int xaddaccumline(RDAreport *,char *,int,char *,short,int,char *);
#define addRDAimage(a,b,c,d,e)	xaddimageline(a,b,c,d,e,0,NULL,__LINE__,__FILE__)
#define addimageline(a,b,c,d,e,f,g)	xaddimageline(a,b,c,d,e,f,g,__LINE__,__FILE__)
int xaddimageline(RDAreport *,char *,char *,short,char *,int,RDApval *,int,char *);
void makepageheader(RDAreport *,RDAezrpt *);
#define EZRPTtoRPT(a,b,c,d)	xEZRPTtoRPT(a,b,c,d,__LINE__,__FILE__)
RDAreport *xEZRPTtoRPT(char *,RDAezrpt *,EZfield *,int,int,char *);
#define establishconflds(a,b,c,d,e,f,g,h,i)	xestablishconflds(a,b,c,d,e,f,g,h,i,__LINE__,__FILE__)
void xestablishconflds(RDAreport *,char *,char *,char *,char *,char *,EZfield *,int,short,int,char *);
void print_test_pages(RDArsrc *,HoldReport *);
#define establishCOAconflds(a,b,c,d,e,f,g,h,i)	xestablishCOAconflds(a,b,c,d,e,f,g,h,i,__LINE__,__FILE__)
void xestablishCOAconflds(RDAreport *,char *,char *,char *,char *,char *,short,EZfield *,int,int,char *);
#define process_controls(a,b,c,d)	xprocess_controls(a,b,c,d,__LINE__,__FILE__)
void xprocess_controls(RDArunrpt *,RDAreport *,int,short,int,char *);
#define MAKEPAGEHEADER(a,b,c,d)	xMAKEPAGEHEADER(a,b,c,d,__LINE__,__FILE__)
void xMAKEPAGEHEADER(RDAreport *,char *,char *,int,short,char *);
#define adddisplay(a,b,c,d,e,f,g,h,i)	xadddisplay(a,b,c,d,e,f,g,FALSE,h,i,0,NULL,FALSE,0,1,FALSE,NULL,__LINE__,__FILE__)
#define adddisplayADV(a,b,c,d,e,f,g,h,i,j,k)	xadddisplay(a,b,c,d,e,f,g,FALSE,h,i,j,k,FALSE,0,1,FALSE,NULL,__LINE__,__FILE__)
#define adddisplayADV2(a,b,c,d,e,f,g,h,i,j,k,l,m)	xadddisplay(a,b,c,d,e,f,g,FALSE,h,i,j,k,l,m,1,FALSE,NULL,__LINE__,__FILE__)
#define adddisplayADV3(a,b,c,d,e,f,g,h,i,j,k,l,m,n)	xadddisplay(a,b,c,d,e,f,g,FALSE,h,i,j,k,l,m,n,FALSE,NULL,__LINE__,__FILE__)
#define adddisplayADV4(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)	xadddisplay(a,b,c,d,e,f,g,FALSE,h,i,j,k,l,m,n,o,p,__LINE__,__FILE__)
#define adddisplayADV5(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q)	xadddisplay(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,__LINE__,__FILE__)
void xadddisplay(RDAreport *,double,double,short,short,short,char *,char,short,
	short,int,RDApvar *,char,short,short,char,char *,int,char *);
#define addsearchconfld(a,b,c)	xaddsearchconfld(a,b,c,__LINE__,__FILE__)
int xaddsearchconfld(RDAsearch *,char *,char *,int,char *);
#define addimagepval(a,b,c,d,e)	xaddimagepval(a,b,c,d,e,__LINE__,__FILE__)
int xaddimagepval(RDAimage *,short,char *,int,char *,int,char *);
void xexit_cleanly(RDArunrpt *,RDAreport *,short,char *,char *,short,char,int,char *);
#define exit_cleanly(a,b,c,d,e,f,g)	xexit_cleanly(a,b,c,d,e,f,g,__LINE__,__FILE__)
void makeEZRPTRangeSCreen(char *,RDAezrpt *);
short RPTGEN_GUIEVAL(char *,RDArsrc *,HoldReport *);
char *RPTGEN_GUIEVALSTR(char *,RDArsrc *,void *,HoldReport *);
void RDAreportSetFunc(HoldReport *);
void dump_csv(RDArsrc *,HoldReport *);
#define ReportMaintainMasterNew(x,y,z)	xReportMaintainMasterNew(x,y,z,__LINE__,__FILE__)
MaintainMaster *xReportMaintainMasterNew(char *,char *,RDAreport *,int,char *);
#endif
