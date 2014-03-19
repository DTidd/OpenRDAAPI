/* mtnmst.c - Maintain Master Browse */
/*lint -library */
#ifndef WIN32
#define __NAM__ "mtnmst.lnx"
#endif
#ifdef WIN32
#define __NAM__ "mtnmst.exe"
#endif
#include <app.hpp>
#include <mix.hpp>
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
	char temp[512];
	if(!RDAstrcmp(argv[1],"-s") || !RDAstrcmp(argv[1],"-S"))
	{
		SIMPLE_SCREENS=TRUE;
		if(InitializeSubsystems(argc,argv,argv[2],argv[3]))
		{
			ShutdownSubsystems();
			return(1);
		}
		if(argc<3) 
		{
			ShutdownSubsystems();
			return(1);
		}
		if(!RDAstrcmp(argv[2],"ARCHIVE") && (!RDAstrcmp(argv[3],"MTN ARCHIVES") || !RDAstrcmp(argv[3],"MTN ARCHIVES.SEARCH") || !RDAstrcmp(argv[3],"MTN ARCHIVES.BROWSE")) && argc>4) 
		{
			WT_RDA_SetEnv("WORK_GROUP",argv[4],__LINE__,__FILE__);
		}
		RUNMAINTAIN(argv[2],argv[3],0,NULL,NULL,NULL);
	} else {
		if(InitializeSubsystems(argc,argv,argv[1],argv[2]))
		{
			return;
		}
		if(!RDAstrcmp(argv[1],"ARCHIVE") && (!RDAstrcmp(argv[2],"MTN ARCHIVES") || !RDAstrcmp(argv[2],"MTN ARCHIVES.SEARCH") || !RDAstrcmp(argv[2],"MTN ARCHIVES.BROWSE")) && argc>3) 
		{
			WT_RDA_SetEnv("WORK_GROUP",argv[3],__LINE__,__FILE__);
		}
		RUNMAINTAIN(argv[1],argv[2],0,NULL,NULL,NULL);
	}
}
