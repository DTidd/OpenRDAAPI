/* bsfunc.c - Balance Sheet Account Functions for Packaging */
#include <mkmsc.hpp>

char *xGetSBC(short fileno,char *reftype,short which,int line,char *file)
{
	char *sbc=NULL,*ref=NULL;
	char delflag=FALSE,std=FALSE;
	short ef=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG GetSBC at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(fileno!=(-1))
	{
		ZERNRD(fileno);
		ef=FRSNRD(fileno,1);
		while(!ef)
		{
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			if(!delflag)
			{
				FINDFLDGETCHAR(fileno,"STANDARD",&std);
				FINDFLDGETSTRING(fileno,"REFERENCE TYPE",&ref);
				if(!RDAstrcmp(ref,reftype))
				{
					if(which==FALSE || (which==TRUE && std==TRUE))
					{
						FINDFLDGETSTRING(fileno,"BALANCE SHEET",&sbc);
						if(!isEMPTY(sbc)) break;
					} 
				}
			}
			ef=NXTNRD(fileno,1);
		}
	}
	if(ref!=NULL) Rfree(ref);
	if(isEMPTY(sbc))
	{
		if(!RDAstrcmp(reftype,"CASH")) 
		{
			sbc=stralloc("101");
		} else if(!RDAstrcmp(reftype,"PRE-PAID ITEMS")) 
		{
			sbc=stralloc("181");
		} else if(!RDAstrcmp(reftype,"REVENUE")) 
		{
			sbc=stralloc("302");
		} else if(!RDAstrcmp(reftype,"EXPENDITURE")) 
		{
			sbc=stralloc("602");
		} else if(!RDAstrcmp(reftype,"ACCOUNTS PAYABLE")) 
		{
			sbc=stralloc("421");
		} else if(!RDAstrcmp(reftype,"ACCOUNTS RECEIVABLE")) 
		{
			sbc=stralloc("121");
		} else if(!RDAstrcmp(reftype,"APPROPRIATION")) 
		{
			sbc=stralloc("601");
		} else if(!RDAstrcmp(reftype,"DEPOSITS PAYABLE")) 
		{
			sbc=stralloc("491");
		} else if(!RDAstrcmp(reftype,"DISCOUNTS ALLOWED")) 
		{
			sbc=stralloc("481");
		} else if(!RDAstrcmp(reftype,"ENCUMBRANCE")) 
		{
			sbc=stralloc("603");
		} else if(!RDAstrcmp(reftype,"ESTIMATED REVENUE")) 
		{
			sbc=stralloc("301");
		} else if(!RDAstrcmp(reftype,"BUDGETARY FD BAL-RESERVED ENCUMBRANCES")) 
		{
			sbc=stralloc("753");
		} else if(!RDAstrcmp(reftype,"FUND BALANCE-UNRESERVED")) 
		{
			sbc=stralloc("762");
		} else if(!RDAstrcmp(reftype,"FUND BALANCE-RESERVED")) 
		{
			sbc=stralloc("763");
		} else if(!RDAstrcmp(reftype,"FUND BALANCE-RESERVED FOR ENCUMBRANCES")) 
		{
			sbc=stralloc("764");
		} else if(!RDAstrcmp(reftype,"FUND BALANCE-UNRESERVED, DESIGNATED")) 
		{
			sbc=stralloc("765");
		} else if(!RDAstrcmp(reftype,"FUND BALANCE-BUDGETARY CONTROL")) 
		{
			sbc=stralloc("770");
		} else if(!RDAstrcmp(reftype,"SALARIES / BENEFITS PAYABLE")) 
		{
			sbc=stralloc("461");
		} else if(!RDAstrcmp(reftype,"DEFERRED REVENUE"))
		{
			sbc=stralloc("499");
		} else if(!RDAstrcmp(reftype,"DUE TO"))
		{
			sbc=stralloc("431");
		} else if(!RDAstrcmp(reftype,"DUE FROM"))
		{
			sbc=stralloc("131");
		} else if(!RDAstrcmp(reftype,"TRANSFER CONTROLS"))
		{
			sbc=stralloc("999");
		}
	}
	return(sbc);
}
