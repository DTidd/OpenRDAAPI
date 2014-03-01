/******************************************************************************
                       MAKE VIRTUAL FIELD FUNCTIONS HEADER
                                    mkvir.h
******************************************************************************/
#ifndef MKVIRFLD_H
#define MKVIRFLD_H
#include <mix.hpp>

#define GetFieldLength(a,b,c) xGetFieldLength(a,b,c,__LINE__,__FILE__)
int xGetFieldLength(char *,char *,char *,int,char *);

#define ReturnFUNDvirtual(a,b,c,d) xReturnFUNDvirtual(a,b,c,d,__LINE__,__FILE__)
char *xReturnFUNDvirtual(short,char *,char *,short *,int,char *);
#define ReturnFUNDSUFFIXvirtual(a,b,c,d) xReturnFUNDSUFFIXvirtual(a,b,c,d,__LINE__,__FILE__)
char *xReturnFUNDSUFFIXvirtual(short,char *,char *,short *,int,char *);
#define CreateFUNDSUFFIXvirtual(a,b,c,d,e) xCreateFUNDSUFFIXvirtual(a,b,c,d,e,__LINE__,__FILE__)
void xCreateFUNDSUFFIXvirtual(char *,char *,short,char *,char *,int,char *);
void xCreateCREATECASHSUFFIXvirtual(char *,char *,short,char *,char *,int,char *);
#define ReturnCLEARINGCASHSUFFIXvirtual(a,b,c,d) xReturnCLEARINGCASHSUFFIXvirtual(a,b,c,d,__LINE__,__FILE__)
char *xReturnCLEARINGCASHSUFFIXvirtual(short,char *,char *,short *,int,char *);
#define CreateCLEARINGCASHSUFFIXvirtual(a,b,c,d,e) xCreateCLEARINGCASHSUFFIXvirtual(a,b,c,d,e,__LINE__,__FILE__)

#define CreateFUNDvirtual(a,b,c,d,e) xCreateFUNDvirtual(a,b,c,d,e,__LINE__,__FILE__)
void xCreateFUNDvirtual(char *,char *,short,char *,char *,int,char *);
#define ReturnPROJECTvirtual(a,b,c,d) xReturnPROJECTvirtual(a,b,c,d,__LINE__,__FILE__)
char *xReturnPROJECTvirtual(short,char *,char *,short *,int,char *);
#define CreatePROJECTvirtual(a,b,c,d,e) xCreatePROJECTvirtual(a,b,c,d,e,__LINE__,__FILE__)
void xCreatePROJECTvirtual(char *,char *,short,char *,char *,int,char *);
#define ReturnBEGBALvirtual(a,b,c,d) xReturnBEGBALvirtual(a,b,c,d,__LINE__,__FILE__)
char *xReturnBEGBALvirtual(short,char *,char *,short *,int,char *);

#define CreateBEGBALvirtual(a,b,c,d,e) xCreateBEGBALvirtual(a,b,c,d,e,__LINE__,__FILE__)
void xCreateBEGBALvirtual(char *,char *,short,char *,char *,int,char *);

#define CreateBEGBALs(a) xCreateBEGBALs(a,__LINE__,__FILE__)
void xCreateBEGBALs(char *,int,char *);
#define ReturnACCMINUS1virtual(a,b,c,d) xReturnACCMINUS1virtual(a,b,c,d,__LINE__,__FILE__)
char *xReturnACCMINUS1virtual(short,char *,char *,short *,int,char *);

#define CreateACCMINUS1virtual(a,b,c,d,e) xCreateACCMINUS1virtual(a,b,c,d,e,__LINE__,__FILE__)
void xCreateACCMINUS1virtual(char *,char *,short,char *,char *,int,char *);
#define CreateCOAs(a) xCreateCOAs(a,__LINE__,__FILE__)
void xCreateCOAs(char *,int,char *);
#define CreateBUDPREPCOAs(a) xCreateBUDPREPCOAs(a,__LINE__,__FILE__)
void xCreateBUDPREPCOAs(char *,int,char *);
#endif /* MKVIRFLD_H */
