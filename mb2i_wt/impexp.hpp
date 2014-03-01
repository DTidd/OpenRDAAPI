#ifndef IMPEXP_H
#define IMPEXP_H

#include <cstdio>
#include <cstdlib>
#include <olh.hpp>
#include <mix.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <gui.hpp>
#include <nrd.hpp>
#include <misc.hpp>

struct IEfields
{
	char *name;
	short type;
	short start;
	unsigned length;
	char *expression;
	char use_quotes;
	char *value_expression;
};
typedef struct IEfields IEfield;

struct IEfiles
{
	char *name;
	char *module;
	char *file;
	char *ifile;
	char *desc;
	char *fdel;
	char *rdel;
	int which;
	int type;
	int num;
	char force_quotes;
	char carriage_return;
	char line_feed;
	char skip_dups;
	IEfield *fields;
	short range;
	short ask_b4_execute;
	char *expression;
	RangeScreen *rs;
	short fileno;
	short MainLoop;
};
typedef struct IEfiles IEfile;

#define getIMPORTbin(a,b)	xgetIMPORTbin(a,b,__LINE__,__FILE__)
short xgetIMPORTbin(char *,IEfile *,int,char *);
#define getIMPORTdesc(a,b)	xgetIMPORTdesc(a,b,__LINE__,__FILE__)
char *xgetIMPORTdesc(char *,char *,int,char *);

#define IMPORTstpNEW(a)	xIMPEXPdefNEW(a,NULL,NULL,NULL,NULL,NULL,NULL,0,0,0,0,0,0,0,0,NULL,0,NULL,__LINE__,__FILE__)
#define IMPEXPdefNEW(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r)	xIMPEXPdefNEW(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,__LINE__,__FILE__)
IEfile *xIMPEXPdefNEW(char *,char *,char *,char *,char *,char *,char *,int,int,char,char,char,char,short,short,char *,int,IEfield *,int,char *);

#define free_import(a)	{xfree_import(a,__LINE__,__FILE__);a=NULL;}
void xfree_import(IEfile *,int,char *);

#define writeIMPORTbin(a,b)	xwriteIMPORTbin(a,b,__LINE__,__FILE__)
short xwriteIMPORTbin(char *,IEfile *,int,char *);

#define EXECUTEimport(a,b)	xEXECUTEimport(a,b,__LINE__,__FILE__)
void xEXECUTEimport(char *,char *,int,char *);

#define ComputeDelimiter(a)	xComputeDelimiter(a,__LINE__,__FILE__)
char *xComputeDelimiter(char *,int,char *);
#define readDelimitedline(a,b,c)	xreadDelimitedline(a,b,c,__LINE__,__FILE__)
char *xreadDelimitedline(FILE *,char *,long *,int,char *);
char *READIEfield(char **,char *,short,short *);
#endif
