/******************************************************************************
				DOHEADERS FUNCTION HEADER
					doheader.h
******************************************************************************/

#ifndef DOHEADERS_H
#define DOHEADERS_H
#include <cstdio>
#include <mix.hpp>

extern char doheaders;

short xDoHeaders(int line,char *file);
#define DoHeaders() xDoHeaders(__LINE__,__FILE__)

#endif /* DOHEADERS_H */
