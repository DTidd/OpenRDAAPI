/******************************************************************************
                          MAKE MISC FUNCTIONS HEADER
                                    mkmsc.h
******************************************************************************/
#ifndef MKMSCFNC_H
#define MKMSCFNC_H
#include <rptgen.hpp>

#if defined(SCO50) || defined(SCO50P)
extern int mkdir(const char *, mode_t);
extern int chmod(const char *, mode_t);
#endif

#define addERRlist(a,b) xaddERRlist(a,b,"No Errors Detected",__LINE__,__FILE__)
#define ADVaddERRlist(a,b,c) xaddERRlist(a,b,c,__LINE__,__FILE__)
int xaddERRlist(APPlib **,char *,char *,int,char *);

#define break_expression2(a,b)	xbreak_expression2(a,b,__LINE__,__FILE__)
APPlib *xbreak_expression2(char *,int *,int,char *);

void free_pvals(RDApval *,int);
void free_pvars(RDApvar *,int);
#define GetSBC(a,b,c)	xGetSBC(a,b,c,__LINE__,__FILE__)
char *xGetSBC(short,char *,short,int,char *);
#define ADVaddRDApvar(a,b,c,d,e,f,g)	xaddRDApvar(a,b,c,d,e,f,g,__LINE__,__FILE__)
#define addRDApvar(a,b,c,d,e,f)	xaddRDApvar(a,b,c,d,e,f,0,__LINE__,__FILE__)
void xaddRDApvar(RDApdev *,char *,char *,short,short,short,short,int,char *);
#define addRDATableValue(a,b,c,d)	xaddRDATableValue(a,b,c,d,__LINE__,__FILE__)
int xaddRDATableValue(RDATABLE *,double,double,double,int,char *);
void Establish_Install_Date(char *);

#endif /* MKMSCFNC_H */
