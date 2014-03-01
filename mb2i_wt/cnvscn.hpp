#ifndef CNVSCN_H
#define CNVSCN_H
#include<mix.hpp>

struct RDAcnvscns
{
	int set_which;
	int match_type;
	char pat_scn;
	char *scn_pattern;
	char rng_scn;
	char *from_scn;
	char *to_scn;
	char all_dirs;
	char *directory;
	char all_libs;
	char *library;
	RDAwdgt from;
	RDAwdgt to;
};
typedef struct RDAcnvscns RDAcnvscn;

#define RDAcnvscnNEW()	xRDAcnvscnNEW(__LINE__,__FILE__)
RDAcnvscn *xRDAcnvscnNEW(int,char *);
#define FreeRDAcnvscn(a)	xFreeRDAcnvscn(a,__LINE__,__FILE__)
void xFreeRDAcnvscn(RDAcnvscn *,int,char *);
#define ADVExecute_RDAcnvscn(a,b)	xExecute_RDAcnvscn(a,b,__LINE__,__FILE__)
#define Execute_RDAcnvscn(a)	xExecute_RDAcnvscn(a,TRUE,__LINE__,__FILE__)
void xExecute_RDAcnvscn(RDAcnvscn *,short,int,char *);

#endif /* CNVSCN_H */
