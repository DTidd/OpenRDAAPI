/* doadd.c - Power Add */
/*lint -library */
#ifndef WIN32
#define __NAM__ "doadd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "doadd.exe"
#endif
#include <app.hpp>


#include <rptgen.hpp>
#include <powadd.hpp>
/*ADDLIB add */
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	if(argc<3) std::exit;
	if(!RDAstrcmp(argv[1],"-s") || !RDAstrcmp(argv[1],"-S"))
	{
		SIMPLE_SCREENS=TRUE;
		if(InitializeSubsystems(argc,argv,argv[2],argv[3]))
		{
			return;
		}
		RUNPOWERADD(argv[2],argv[3],0,NULL,NULL,NULL);
	} else {
		if(InitializeSubsystems(argc,argv,argv[1],argv[2]))
		{
			return;
		}
		RUNPOWERADD(argv[1],argv[2],0,NULL,NULL,NULL);
	}
}
