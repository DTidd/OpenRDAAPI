/*rptgen.h - Report Generator Private Header */
#ifndef RPTPH_D
#define RPTPH_D
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <ctime>
#include <math.h>
#include <fcntl.h>

#ifdef SCO386
#include <sys/stat.h>
#endif
#ifdef RS6000
#include <sys/stat.h>
#endif

#include <xpert.hpp>
#include <mix.hpp>
#include <olh.hpp>
#include <trans.hpp>
#include <evaluate.hpp>
#include <sec.hpp>
#include <nrd.hpp>
#include <gui.hpp>
#include <misc.hpp>

short makesortfile(RDArunrpt *,RDAreport *);
void ReportGenSubData(char **,HoldReport *);
void ReportGenSubData1(char **,HoldReport *);
short compute_virtual_field(RDArunrpt *,RDAreport *,char *,void (*)(...),void *);
void compute_remaining_virtuals(RDArunrpt *,RDAreport *);
void SetFieldValue(NRDfield *,RDAreport *,RDArunrpt *);
short writesortrecord(RDArunrpt *,RDAreport *);
short evaluate_records(RDArunrpt *,RDAreport *);
short READsubfiles(RUNfile *,RDArunrpt *,RDAreport *);
#define READRecords(x,y)	xREADRecords(x,y,__LINE__,__FILE__)
short xREADRecords(RDArunrpt *,RDAreport *,int,char *);
void FREEsubfiles(RUNfile *);
void FREEReportfiles(RDArunrpt *);
short addReportFile(RDArunrpt *,char *,char *,short,short,short);
short OPNsubfiles(RUNfile *,RDArunrpt *);
short OPNReportfiles(RDArunrpt *);
#define CLSReportfiles(a,b)	xCLSReportfiles(a,b,__LINE__,__FILE__)
void xCLSReportfiles(RDArunrpt *,short,int,char *);
short fieldexists(int,NRDfield *,char *);
short addfield(RDArunrpt *,char *,short,short);
short addsubfile(RDAsearch *,RUNfile *,short);
short definefiles(RDAreport *,RDArunrpt *);
short SHOWrunfiles(RDArunrpt *);
#define testform(x,y,z)	xtestform(x,y,z,__LINE__,__FILE__)
void xtestform(RDArunrpt *,RDAreport *,short,int,char *);
short getlinecomponent(char **,char **,RDArunrpt *,RDAreport *,RDAimage *);
void print_font(RDArunrpt *,RDAreport *,char *,char **);
#define interpretline(x,y,z)	xinterpretline(x,y,z,__LINE__,__FILE__)
void xinterpretline(RDArunrpt *,RDAreport *,RDAimage *,int,char *);
void print_line(RDArunrpt *,RDAreport *,RDAimage *);
void print_section(RDArunrpt *,RDAreport *,char *);
#define ReportBackEnd(x,y)	xReportBackEnd(x,y,__LINE__,__FILE__)
void xReportBackEnd(RDArunrpt *,RDAreport *,int,char *);
void quitreport(HoldReport *);
void quitreport1(HoldReport *);
void executereport(HoldReport *);
void executereport1(HoldReport *);
void SetFieldValue1(NRDfield *,char *,RDAreport *,RDArunrpt *);
#define CLSReportfiles(a,b)	xCLSReportfiles(a,b,__LINE__,__FILE__)
void xCLSReportfiles(RDArunrpt *,short,int,char *);
#define FREEsubfiles(x)	xFREEsubfiles(x,__LINE__,__FILE__)
void xFREEsubfiles(RUNfile *,int,char *);
#define FREEReportfiles(x)	xFREEReportfiles(x,__LINE__,__FILE__)
void xFREEReportfiles(RDArunrpt *,int,char *);
short PrintLineSubData(char **,char *,HoldReport *);
HoldReport *HoldReportNew(RDArunrpt *,RDAreport *);
#define REPORTFILENUMBER(a,b,c,d,e)	xREPORTFILENUMBER(a,b,c,d,e,__LINE__,__FILE__)
short xREPORTFILENUMBER(RDArunrpt *,char *,char *,short,short,int,char *);
#define PageFooterSize(x,y)	xPageFooterSize(x,y,__LINE__,__FILE__)
int xPageFooterSize(RDArunrpt *,RDAreport *,int,char *);
#define addsortfield(a,b,c,d,e)	xaddsortfield(a,b,c,d,e,__LINE__,__FILE__)
short xaddsortfield(RDArunrpt *,RDAreport *,char *,char *,int,int,char *);
void process_exceptions(RDArunrpt *,RDAreport *);
void ReadVirtualRangeStruct(RDAvirtual *,RangeScreen *);
void runprtscreen(HoldReport *);
void setvirtualrangecallbacks(RDAreport *,RDArsrc *);
void setvirtualdeftype(RDArmem *);
void setvirtualdeftypefrom(RDArmem *);
void setvirtualacctype(RDArmem *);
void setvirtualdeftypeto(RDArmem *);
void setvirtualacctypefrom(RDArmem *);
void setvirtualacctypeto(RDArmem *);
#define addvirtualline(a,b,c,d,e,f)	xaddvirtualline(a,b,c,d,e,f,__LINE__,__FILE__)
int xaddvirtualline(RDAreport *,char *,int,short,char *,short,int,char *);
#define addcontrolline(a,b,c)	xaddcontrolline(a,b,c,__LINE__,__FILE__)
int xaddcontrolline(RDAreport *,char *,short,int,char *);
#define addsortline(a,b,c)	xaddsortline(a,b,c,__LINE__,__FILE__)
int xaddsortline(RDAreport *,int,char *,int,char *);
#define addsearchline(a,b,c,d,e,f,g,h,i,j)	xaddsearchline(a,b,c,d,e,f,g,h,i,NULL,j,__LINE__,__FILE__)
int xaddsearchline(RDAreport *,char *,char *,int,char *,char *,short,RDAconfld *,char *,char *,short,int,char *);
#define addaccumline(a,b,c,d,e)	xaddaccumline(a,b,c,d,e,__LINE__,__FILE__)
int xaddaccumline(RDAreport *,char *,int,char *,short,int,char *);
#define addimageline(a,b,c,d,e,f,g)	xaddimageline(a,b,c,d,e,f,g,__LINE__,__FILE__)
int xaddimageline(RDAreport *,char *,char *,short,char *,int,RDApval *,int,char *);
void makepageheader(RDAreport *,RDAezrpt *);
#define establishconflds(a,b,c,d,e,f)	xestablishconflds(a,b,c,d,e,f,__LINE__,__FILE__)
void xestablishconflds(RDAreport *,char *,char *,char *,char *,char *,int,char *);
#endif
