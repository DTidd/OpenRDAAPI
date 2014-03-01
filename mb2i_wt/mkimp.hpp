/******************************************************************************
                       MAKE IMPORT/EXPORT FUNCTIONS HEADER
                                    mkimp.h
******************************************************************************/
#ifndef MKIMPEXP_H
#define MKIMPEXP_H
#include <mix.hpp>
#include <impexp.hpp>

#define addIMPEXPfield(a,b,c,d,e,f,g)	xaddIMPEXPfield(a,b,c,d,e,f,g,NULL,__LINE__,__FILE__)
#define addIMPEXPfieldADV(a,b,c,d,e,f,g,h)	xaddIMPEXPfield(a,b,c,d,e,f,g,h,__LINE__,__FILE__)
short xaddIMPEXPfield(IEfile *,char *,short,short,unsigned,char *,char,char *,int,char *);

#endif /* MKIMPEXP_H */
