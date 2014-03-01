/* dfchg.h - Define Change Include File */
#ifndef DFCHG_H
#define DFCHG_H
#include <mix.hpp>

struct CHGfields
{
	char keypart;
	char *fieldname;
};
typedef struct CHGfields CHGfield;

struct CHGfiles
{
	char *module;
	char *filename;
	char *keyname;
	char *expression;
	int num;
	RDAsearch *supfil;
	int numflds;
	CHGfield *chgflds;
	short fileno;
	short keyno;
	RDATData *prev;
	void *parent;
	DBsort *sort;
};
typedef struct CHGfiles CHGfile;

struct CHGstructs
{
	char *module;
	char *name;
	char *desc;
	char *fieldname;
	short type;
	unsigned len;
	char simple_from;
	char simple_to;
	union rda_field_types from;
	union rda_field_types to;
	unsigned from_dlen;
	unsigned to_dlen;
	char range;
	char *range_screen;
	char *expression;
	int numfiles;
	CHGfile *chgfiles;
	void (*SubFunc)();
	void *SubFuncArgs;
	RDArsrc *rsrc;
};
typedef struct CHGstructs CHGstruct;

#endif /* DFCHG_H */

#define CHGstructNEW(a,b)	xCHGstructNEW(a,b,__LINE__,__FILE__)
CHGstruct *xCHGstructNEW(char *,char *,int,char *);
#define FreeCHGstruct(a)	xFreeCHGstruct(a,__LINE__,__FILE__)
void xFreeCHGstruct(CHGstruct *,int,char *);
#define getCHGstruct(a,b)	xgetCHGstruct(a,b,__LINE__,__FILE__)
short xgetCHGstruct(char *,CHGstruct *,int,char *);
#define writeCHGstruct(a,b)	xwriteCHGstruct(a,b,__LINE__,__FILE__)
short xwriteCHGstruct(char *,CHGstruct *,int,char *);
#define getCHGstructdesc(a,b)	xgetCHGstructdesc(a,b,__LINE__,__FILE__)
char *xgetCHGstructdesc(char *,char *,int,char *);
