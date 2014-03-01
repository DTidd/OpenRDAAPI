#ifndef PPTMGT_H
#define PPTMGT_H
#include <mix.hpp>
#include <olh.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <nrd.hpp>
#include <gui.hpp>
#include <misc.hpp>

struct PPTclass
{
	char *name;
	char *desc;
};
typedef struct PPTclass PPTclas;

struct RDApptmgts
{
	char *commissioner;
	char *comm_addr1;
	char *comm_addr2;
	char *comm_city;
	char *comm_state;
	char *comm_zip;
	char *comm_phone;
	char *treasurer;
	char *treas_addr1;
	char *treas_addr2;
	char *treas_city;
	char *treas_state;
	char *treas_zip;
	char *treas_phone;
	int num;
	PPTclas *classes;
};
typedef struct RDApptmgts RDApptmgt;

extern char *PPTMGTTranTypeFields[];

#define MakePersonalPropertyField(a,b,c)	xMakePersonalPropertyField(a,b,c,__LINE__,__FILE__)
char *xMakePersonalPropertyField(char *,short,RDApptmgt *,int,char *);
#define UpdatePersonalPropertyTransactions(a,b,c,d) xUpdatePersonalProperty((-1),a,b,c,(-1),d,NULL,NULL,NULL,__LINE__,__FILE__)
#define UpdatePersonalPropertyPayments(a,b,c,d,e,f) xUpdatePersonalProperty(a,b,c,d,e,f,NULL,NULL,NULL,__LINE__,__FILE__)
#define ADVUpdatePersonalPropertyTransactions(a,b,c,d,e,f,g) xUpdatePersonalProperty((-1),a,b,c,(-1),d,e,f,g,__LINE__,__FILE__)
#define ADVUpdatePersonalPropertyPayments(a,b,c,d,e,f,g,h) xUpdatePersonalProperty(a,b,c,d,e,f,g,h,NULL,__LINE__,__FILE__)
void xUpdatePersonalProperty(short,short,short,short,short,double,void (*)(),void *,RDApptmgt *,int,char *);

#define getpptmgtbin(x)	xgetpptmgtbin(x,__LINE__,__FILE__)
short xgetpptmgtbin(RDApptmgt *,int,char *);
#define writepptmgtbin(x)	xwritepptmgtbin(x,__LINE__,__FILE__)
short xwritepptmgtbin(RDApptmgt *,int,char *);
#define RDApptmgtNEW()	xRDApptmgtNEW(__LINE__,__FILE__)
RDApptmgt *xRDApptmgtNEW(int,char *);
#define free_pptmgt(x)	{xfree_pptmgt(x,__LINE__,__FILE__);x=NULL;}
void xfree_pptmgt(RDApptmgt *,int,char *);
#define GetPPTMGTSetup(a,b,c)	xGetPPTMGTSetup(a,b,c,__LINE__,__FILE__)
short xGetPPTMGTSetup(RDApptmgt *,void (*)(),void *,int,char *);

#endif /* PPTMGT_H */
