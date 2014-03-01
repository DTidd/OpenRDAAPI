/******************************************************************************
				ON-LINE HELP HEADER
					olh.h
*******************************************************************************/

#ifndef OLH_H
#define OLH_H
#include <cstdio>
#include <cstdlib>

#ifndef WIN32
#include <fcntl.h>
#include<strings.h>
#endif 

#include <xpert.hpp>
#include <evaluate.hpp>
#include <sec.hpp>
#include <nrd.hpp>
#include <gui.hpp>
#include <misc.hpp>

/* Define Common reusable value strings like web urls etc */
#define RDA_WEBSITE_DOMAIN "openrda.com"
#define RDA_WEBSITE_BASE_URL "http://openrda.com"
#define RDA_WEBSITE_BLOG_URL "http://openrda.com/blog"

#define ADVINITHELP(a,b)	xINITHELP(a,b,__LINE__,__FILE__)
#define INITHELP()	xINITHELP(NULL,NULL,__LINE__,__FILE__)
extern short (*HELPEXPRESSFUNC)(...);
extern void (*HELPADDOSFUNC)(...);
/* INITHELP Initailizes the help system and opens the Note Related Data Files */
#define EXITHELP()	xEXITHELP(__LINE__,__FILE__)
/* EXITHELP Shuts down help and closes Note Related Files */
#define RDAhelpNEW()	xRDAhelpNEW(__LINE__,__FILE__)
/* RDAhelpNEW allocates memory for a new RDAhelp Structure and returns a 
	pointer to it */
#define SEERDAHELP(x)	xSEERDAHELP(x,__LINE__,__FILE__)
/* SEERDAHELP is a diagnostic program used to print the contents of an RDAhelp 
	Structure to standard error (RDA_STDERR).  It requires 1 argument the 
	pointer to the RDAhelp Structure (RDAhelp *) */
#define FINDRHELP(x,y)	xFINDRHELP(x,y,__LINE__,__FILE__)
/* FINDRHELP is used to Find a Help for a Specific Help Resource contained in 
	an RDAhelp Structure.  It requires a pointer to the RDAhelp Structure 
	and the name of the resource (char *) */
#define free_help(x)	{xfree_help(x,__LINE__,__FILE__);x=NULL;}
/* free_help Free's the memory allocated in an RDAhelp Structure */
#define findhelpbin(x,y)	xfindhelpbin(x,y,__LINE__,__FILE__)
/* findhelpbin Searchs a specific help binary library for the specified help 
	binary (screen) and returns a pointer to it (RDAhelp *) */
#define gethelpbin(x,y,z,a)	xgethelpbin(x,y,z,a,__LINE__,__FILE__)
/* gethelpbin attempts to read a help binary from a help binary library */
#define writehelpbin(x,y)	xwritehelpbin(x,y,__LINE__,__FILE__)
/* writehelpbin writes the specified help binary to the specified help binary 
	library */
#define addlstrsrc(r,n,v,e,w,x,z,a)	addrsrc(r,0,n,SCROLLEDLIST,0,v,e,w,x,z,a)
/* addlstrsrc - Add List Resource Element with resource help */
#define addsctrsrc(r,n,l,v,e)	addrsrc(r,0,n,SCROLLEDTEXT,l,v,e,NULL,0,NULL,NULL)
/* addsctrsrc - Add Scrolled Text Resource Element with resource help */
#define addstdrsrc(r,n,t,l,v,e)	addrsrc(r,0,n,t,l,v,e,NULL,0,NULL,NULL)
/* addstdrsrc - Add Standard Resource with resource help */
#define addbtnrsrc(r,n,e,f,g)	addrsrc(r,1,n,BUTTONS,0,NULL,e,f,0,NULL,g)
/* addbtnrsrc - Add Button Resource with resource help */
#define addrfcbrsrc(r,n,e,f,g)	addrsrc(r,2,n,BUTTONS,0,NULL,e,f,0,NULL,g)
/* addrfcbrsrc Add Run-function (same as button now!) callback button resource */
#define addrfexrsrc(r,n,e,f,g)	addrsrc(r,3,n,BUTTONS,0,NULL,e,f,0,NULL,g)
/* addrfexrsrc - Add Button resource with help that will exit the program upon completion of executing the specified function */
#define addrfkwrsrc(r,n,e,f,g)	addrsrc(r,4,n,BUTTONS,0,NULL,e,f,0,NULL,g)
/* addrfkwrsrc - Add Button resource with help that will kill it's 
	corresponding window upon completion of it's specified function */
#define addrsrc(a,b,c,d,e,f,g,h,i,j,k)	xaddrsrc(a,b,c,d,e,f,g,h,i,j,k,resourcehelp,FALSE,__LINE__,__FILE__)
#define RMEMaddlstrsrc(r,n,v,e,w,x,z,a)	RMEMaddrsrc(r,0,n,SCROLLEDLIST,0,v,e,w,x,z,a)
#define RMEMaddsctrsrc(r,n,l,v,e)	RMEMaddrsrc(r,0,n,SCROLLEDTEXT,l,v,e,NULL,0,NULL,NULL)
#define RMEMaddstdrsrc(r,n,t,l,v,e)	RMEMaddrsrc(r,0,n,t,l,v,e,NULL,0,NULL,NULL)
#define RMEMaddbtnrsrc(r,n,e,f,g)	RMEMaddrsrc(r,1,n,BUTTONS,0,NULL,e,f,0,NULL,g)
#define RMEMaddrfcbrsrc(r,n,e,f,g)	RMEMaddrsrc(r,2,n,BUTTONS,0,NULL,e,f,0,NULL,g)
#define RMEMaddrfexrsrc(r,n,e,f,g)	RMEMaddrsrc(r,3,n,BUTTONS,0,NULL,e,f,0,NULL,g)
#define RMEMaddrfkwrsrc(r,n,e,f,g)	RMEMaddrsrc(r,4,n,BUTTONS,0,NULL,e,f,0,NULL,g)
#define RMEMaddrsrc(a,b,c,d,e,f,g,h,i,j,k)	xaddrsrc(a,b,c,d,e,f,g,h,i,j,k,resourcehelp,TRUE,__LINE__,__FILE__)

extern short SCNNOTES_FILENO;
extern short USCNNOTES_FILENO;
extern short RSCNOTES_FILENO;
extern short URSCNOTES_FILENO;
extern char WRITE_SCNNOTES;
extern char WRITE_USCNNOTES;
extern char WRITE_RSCNOTES;
extern char WRITE_URSCNOTES;

struct RDArhlps
{
	char *name;
	char *desc;
	char *label;
};
typedef struct RDArhlps RDArhlp;

struct RDAhelps 
{
	char *name;
	char *desc;
	int numrschs;
	RDArhlp *rhlp;
};
typedef struct RDAhelps RDAhelp;

struct WriteRsrcHelps
{
	RDArmem *member;
	RDArsrc *rsrc;
};
typedef struct WriteRsrcHelps WriteRsrcHelp;

struct WriteScnHelps
{
	RDArsrc *rsrcx;
	RDArsrc *helprsrc;
};
typedef struct WriteScnHelps WriteScnHelp;

struct SearchHelps
{
	RDArsrc *r;
	char **connecttype;
};
typedef struct SearchHelps SearchHelp;

struct PrintSections
{
	RDArsrc *prsrc;
	RDArsrc *hrsrc;
	RDArsrc *rsrc;
};
typedef struct PrintSections PrintSection;

void xINITHELP(short (*)(...),void (*)(...),int,char *);
void xEXITHELP(int,char *);
RDAhelp *xRDAhelpNEW(int,char *);
void xSEERDAHELP(RDAhelp *,int,char *);
RDArhlp *xFINDRHELP(RDAhelp *,char *,int,char *);
void xfree_help(RDAhelp *,int,char *);
RDAhelp *xfindhelpbin(char *,char *,int,char *);
short xgethelpbin(char *,char *,RDAhelp *,short,int,char *);
short xwritehelpbin(char *,RDAhelp *,int,char *);
void resourcehelp(RDArsrc *r,RDArmem *);
void screenhelp(RDArsrc *);
void SaveDefaults(RDArsrc *);
void printrsrcs(RDArsrc *,RDArsrc *);
#define OpenRDAWiki(a)	xOpenRDAWiki(a,__LINE__,__FILE__)
void xOpenRDAWiki(char *,int,char *);

#endif
