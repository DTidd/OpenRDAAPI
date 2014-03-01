/******************************************************************************
                       Make GA Uniform Chart of Accounts
                                    mkgag.h
******************************************************************************/
#ifndef MKGAG_H
#define MKGAG_H
#include <mix.hpp>

#define isGAUCOA(a,b) xisGAUCOA(a,b,__LINE__,__FILE__)
char xisGAUCOA(int,RDAacct *,int,char *);

#define CreateGAUCOAs(a) xCreateGAUCOAs(a,__LINE__,__FILE__)
void xCreateGAUCOAs(char *,int,char *);

#endif /* MKGAG_H */
