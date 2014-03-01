/******************************************************************************
			PURCHASE ORDER FUNCTIONS HEADER
				purord.h
******************************************************************************/

#ifndef PURORD_H
#define PURORD_H
#include <mix.hpp>
#include <venpmt.hpp>

struct RDApurords
{
	char use_fiscal_months;
/*
	char Use_Taxes;
	char Use_Tax1;
	char Sep_Tax1_Vendor;
	char *Default_Tax1_Vendor;
	char *Default_Tax1_Address;
	double Tax1_Rate;
	char Use_Tax2;
	char Sep_Tax2_Vendor;
	char *Default_Tax2_Vendor;
	char *Default_Tax2_Address;
	double Tax2_Rate;
	char Use_Tax3;
	char Sep_Tax3_Vendor;
	char *Default_Tax3_Vendor;
	char *Default_Tax3_Address;
	double Tax3_Rate;
	char Use_Tax4;
	char Sep_Tax4_Vendor;
	char *Default_Tax4_Vendor;
	char *Default_Tax4_Address;
	double Tax4_Rate;
	char Use_Tax5;
	char Sep_Tax5_Vendor;
	char *Default_Tax5_Vendor;
	char *Default_Tax5_Address;
	double Tax5_Rate;
	char *admin_fee;
*/
};
typedef struct RDApurords RDApurord;


#define updateoutstanding(f,lf,y,p,l,a) xupdateoutstanding(f,lf,y,p,l,0.0,0.0,0.0,0.0,a,TRUE,0,NULL,NULL,NULL,__LINE__,__FILE__)
#define ADVupdateoutstanding(f,lf,y,p,l,a,b,c) xupdateoutstanding(f,lf,y,p,l,0.0,0.0,0.0,0.0,a,TRUE,0,b,c,NULL,__LINE__,__FILE__)
#define ADV2updateoutstanding(f,lf,y,p,l,a,s,b,c) xupdateoutstanding(f,lf,y,p,l,0.0,0.0,0.0,0.0,a,0,s,b,c,NULL,__LINE__,__FILE__)
#define ADV3updateoutstanding(f,lf,y,p,l,cost,salestax,a,s,b,c,venpmt)	xupdateoutstanding(f,lf,y,p,l,cost,salestax,0.0,0.0,a,s,b,c,venpmt,__LINE__,__FILE__)
#define ADV4updateoutstanding(f,lf,y,p,l,cost,salestax,adminfee,prorate,a,t,s,b,c,venpmt) xupdateoutstanding(f,lf,y,p,l,cost,salestax,adminfee,prorate,a,t,s,b,c,venpmt,__LINE__,__FILE__)
short xupdateoutstanding(short,short,int,char *,int,double,double,double,double,double,short,short,void (*)(...),void *,RDAvenpmt *,int,char *);
/* xupdateoutstanding function update the encumbered outstanding amount for
	the Purchase Order Master.
	args:	(f)->file number.
		(y)->Year Identification Key Value.
		(p)->PO Identification Key Value.
		(a)->amount of the adjustment.
		(t)->type of update (0=normal,other=disencumbrance).
*/
#define CheckPOID(a,b)	xCheckPOID(a,b,NULL,NULL,__LINE__,__FILE__)
#define ADVCheckPOID(a,b,c,d)	xCheckPOID(a,b,c,d,__LINE__,__FILE__)
short xCheckPOID(short,char *,void (*)(...),void *,int,char *);
#define GetAssignNextPOID(a,b)	xGetAssignNextPOID(a,b,NULL,NULL,__LINE__,__FILE__)
#define ADVGetAssignNextPOID(a,b,c,d)	xGetAssignNextPOID(a,b,c,d,__LINE__,__FILE__)
char *xGetAssignNextPOID(short,char *,void (*)(...),void *,int,char *);
#define UndoIDifnoPO(a,b,c)	xUndoIDifnoPO(a,b,c,NULL,NULL,__LINE__,__FILE__)
#define ADVUndoIDifnoPO(a,b,c,d,e)	xUndoIDifnoPO(a,b,c,d,e,__LINE__,__FILE__)
void xUndoIDifnoPO(short,short,char *,void (*)(...),void *,int,char *);
#define RemovePOID(a,b)	xRemovePOID(a,b,NULL,NULL,__LINE__,__FILE__)
#define ADVRemovePOID(a,b,c,d)	xRemovePOID(a,b,c,d,__LINE__,__FILE__)
void xRemovePOID(short,char *,void (*)(...),void *,int,char *);
#define AssignPOID(a,b)	xAssignPOID(a,b,NULL,NULL,__LINE__,__FILE__)
#define ADVAssignPOID(a,b,c,d)	xAssignPOID(a,b,c,d,__LINE__,__FILE__)
void xAssignPOID(short,char *,void (*)(...),void *,int,char *);
#define getpurordbin(x)	xgetpurordbin(x,__LINE__,__FILE__)
short xgetpurordbin(RDApurord *,int,char *);
#define writepurordbin(x)	xwritepurordbin(x,__LINE__,__FILE__)
short xwritepurordbin(RDApurord *,int,char *);
#define RDApurordNEW()	xRDApurordNEW(__LINE__,__FILE__)
RDApurord *xRDApurordNEW(int,char *);
#define free_purord(x)	{xfree_purord(x,__LINE__,__FILE__);x=NULL;}
void xfree_purord(RDApurord *,int,char *);
#define GetPurOrdSetup(a,b,c)	xGetPurOrdSetup(a,b,c,__LINE__,__FILE__)
short xGetPurOrdSetup(RDApurord *,void (*)(...),void *,int,char *);

#endif /* PURORD_H */
