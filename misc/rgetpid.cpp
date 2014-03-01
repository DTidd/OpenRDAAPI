/* gets the process id num */
#include <misc.hpp>
#include <unistd.h>

int RGETPID() 
{
	int PROSID=0;

	PROSID=(int)getpid();
	return(PROSID);	
}
