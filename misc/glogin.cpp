/* glogin.c - Manages the User's Login */
#include <misc.hpp>

char *USERLOGIN=NULL;

void SETUSERLOGIN()
{
	if(RDA_NOGUI)
	{
		USERLOGIN=getenv("LOGNAME");
	}
	if(isEMPTY(USERLOGIN))
	{
		USERLOGIN=stralloc("GUEST");
	}
}
