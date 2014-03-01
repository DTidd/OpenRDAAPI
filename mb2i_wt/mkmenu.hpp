/******************************************************************************
				MAKE MENU FUNCTIONS HEADER
					mkmenu.h
******************************************************************************/
#ifndef MKMENU_H
#define MKMENU_H
#include<mix.hpp>

#define ConvMenu(a,b)	xConvMenu(a,b,__LINE__,__FILE__)
void xConvMenu(RDAmenu *,char *,int,char *);
#endif /* MKMENU_H */
