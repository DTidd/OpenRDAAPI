#ifndef RLSTMGT_H
#define RLSTMGT_H
#include <mix.hpp>
#include <olh.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <nrd.hpp>
#include <gui.hpp>
#include <misc.hpp>

extern char *TranTypeFields[];
#define MakeRealEstateField(a)	xMakeRealEstateField(a,__LINE__,__FILE__)
char *xMakeRealEstateField(short,int,char *);
#define UpdateRealEstateTransactions(a,b,c,d) xUpdateRealEstate((-1),a,b,c,(-1),d,NULL,NULL,__LINE__,__FILE__)
#define UpdateRealEstatePayments(a,b,c,d,e) xUpdateRealEstate(a,b,(-1),c,d,e,NULL,NULL,__LINE__,__FILE__)
#define ADVUpdateRealEstateTransactions(a,b,c,d,e,f) xUpdateRealEstate((-1),a,b,c,(-1),d,e,f,__LINE__,__FILE__)
#define ADVUpdateRealEstatePayments(a,b,c,d,e,f,g) xUpdateRealEstate(a,b,(-1),c,d,e,f,g,__LINE__,__FILE__)
void xUpdateRealEstate(short,short,short,short,short,double,void (*)(),void *,int,char *);
#endif /* RLSTMGT_H */
