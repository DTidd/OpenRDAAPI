#ifndef RDALIC_H
#define RDALIC_H

#include <xpert.hpp>
#include <mix.hpp>
#include <sec.hpp>
#include <trans.hpp>
#include <gui.hpp>
#include <nrd.hpp>
#include <misc.hpp>

struct RDAlicenses
{
	short used;
	int process_count;
	char *display;
};
typedef struct RDAlicenses RDAlicense;

#define INITRDALIC()    xINITRDALIC(__LINE__,__FILE__)
int xINITRDALIC(int,char *);

#define ShutdownRDALIC()    xShutdownRDALIC(__LINE__,__FILE__)
int xShutdownRDALIC(int,char *);

#define getRDAlicvfile()    xgetRDAlicvfile(__LINE__,__FILE__)
VF xgetRDAlicvfile(int,char *);

#define getRDAlicbin(x)    xgetRDAlicbin(x,__LINE__,__FILE__)
RDAlicense *xgetRDAlicbin(BIN *,int,char *);

#define writeRDAlicbin(x)    xwriteRDAlicbin(x,__LINE__,__FILE__)
BIN *xwriteRDAlicbin(RDAlicense *,int,char *);

#define RDAlicNEW()    xRDAlicNEW(__LINE__,__FILE__)
RDAlicense *xRDAlicNEW(int,char *);

#define Free_RDAlic(x)    {xFree_RDAlic(x,__LINE__,__FILE__);x=NULL;}
void xFree_RDAlic(RDAlicense *,int,char *);

#endif
