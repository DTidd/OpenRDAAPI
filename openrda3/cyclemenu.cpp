#include <misc.hpp>



#include <gui.hpp>
#include <guip.hpp>
#include <nrd.hpp>
#include <nrdp1.hpp>
#include <olh.hpp>
#include <mix.hpp>
#include <rptgen.hpp>



extern RDArsrc *DockRSRC;
extern char *CURRENT_MODULE;
extern short MENUITEM_FILENO;
extern void ExecuteOptionCycle(int);

static int GetExecuteNumber(char *n)
{
	RDArmem *mem=NULL;
	int x=0,en=(-1);
	short ef=NULL;
	char delflag=FALSE,*m=NULL,*name=NULL;
	
	x=FINDRSC(DockRSRC,n);
	if(x>(-1))
	{
		mem=DockRSRC->rscs+x;
		en=mem->items;
	}
	if(en==(-1))
	{
		ZERNRD(MENUITEM_FILENO);
		FINDFLDSETSTRING(MENUITEM_FILENO,"MODULE NAME",CURRENT_MODULE);
		ef=GTENRD(MENUITEM_FILENO,1);
		while(!ef)
		{
			FINDFLDGETSTRING(MENUITEM_FILENO,"MODULE NAME",&m);
			if(RDAstrcmp(m,CURRENT_MODULE)) break;
			FINDFLDGETCHAR(MENUITEM_FILENO,"DELETEFLAG",&delflag);
			if(!delflag)
			{
				FINDFLDGETSTRING(MENUITEM_FILENO,"NAME",&name);
				if(!RDAstrcmp(name,n))
				{
					FINDFLDGETINT(MENUITEM_FILENO,"ITEM NUMBER",&en);
					break;
				}
			}
			ef=NXTNRD(MENUITEM_FILENO,1);
		}
		ZERNRD(MENUITEM_FILENO);
	}
	if(name!=NULL) Rfree(name);
	if(m!=NULL) Rfree(m);
	return(en);
}

