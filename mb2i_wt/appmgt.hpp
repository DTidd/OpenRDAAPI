/******************************************************************************
				    APPMGT FUNCTIONS HEADER
					       appmgt.h
******************************************************************************/

#ifndef APPMGT_H
#define APPMGT_H
#include <mix.hpp>

struct RDAappmgts
{
	char use_personnel_interview;
	char use_administrative_interview;
};
typedef struct RDAappmgts RDAappmgt;

#define getappmgtbin(x)	xgetappmgtbin(x,__LINE__,__FILE__)
short xgetappmgtbin(RDAappmgt *,int,char *);
#define writeappmgtbin(x)	xwriteappmgtbin(x,__LINE__,__FILE__)
short xwriteappmgtbin(RDAappmgt *,int,char *);
#define RDAappmgtNEW()	xRDAappmgtNEW(__LINE__,__FILE__)
RDAappmgt *xRDAappmgtNEW(int,char *);
#define free_appmgt(x)	{xfree_appmgt(x,__LINE__,__FILE__);x=NULL;}
void xfree_appmgt(RDAappmgt *,int,char *);
#define GetAppMgtSetup(a)	xGetAppMgtSetup(a,__LINE__,__FILE__)
short xGetAppMgtSetup(RDAappmgt *,int,char *);
#endif /* APPMGT_H */
