#ifndef FIXASS_H 
#define FIXASS_H
#include <cstdio>
#include <cstdlib>
#include <mix.hpp>

#define UpdateTotalItems(f,i,q,p,d,r,s,a) xUpdateTotalItems(f,i,q,p,d,r,s,a,__LINE__,__FILE__)
void xUpdateTotalItems(short,char *,int,double,double,double,void (*)(),void *,int,char *);

#endif /* INVCTL_H */
