#ifndef MISCBILL_H
#define MISCBILL_H
#include <cstdio>
#include <cstdlib>
#include <mix.hpp>
#include <gui.hpp>
#include <olh.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <sec.hpp>
#include <misc.hpp>


extern char *BillTypeFields[];
#define MakeMiscBillingField(a)	xMakeMiscBillingField(a,__LINE__,__FILE__)
char *xMakeMiscBillingField(short,int,char *);
#define UpdateMiscBillingTransactions(a,b,c,d) xUpdateMiscBilling((-1),a,b,(-1),c,d,NULL,NULL,__LINE__,__FILE__)
#define UpdateMiscBillingPayments(a,b,c,d,e) xUpdateMiscBilling(a,b,c,d,0.0,e,NULL,NULL,__LINE__,__FILE__)
#define ADVUpdateMiscBillingTransactions(a,b,c,d,e,f) xUpdateMiscBilling((-1),a,b,(-1),c,d,e,f,__LINE__,__FILE__)
#define ADVUpdateMiscBillingPayments(a,b,c,d,e,f,g) xUpdateMiscBilling(a,b,c,d,0.0,e,f,g,__LINE__,__FILE__)
void xUpdateMiscBilling(short,short,short,short,double,double,void (*)(),void *,int,char *);
#endif /* MISCBILL_H */
