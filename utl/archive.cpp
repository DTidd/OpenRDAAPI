#ifndef WIN32
#define __NAM__ "archive.lnx"
#endif
#ifdef WIN32
#define __NAM__ "archive.exe"
#endif
#include <app.hpp>
#include <cstdlib>
#if (!defined(UNIXWARE21) && !defined(UNIXWARE7))
#include <sys/stat.h>
#include <fcntl.h>
#else 
#include <sys/stat.h>
#endif
#include <mix.hpp>
#include <gui.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

#define PIPEBLOCKSIZE 512

#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS) && !defined(WIN32)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */
#ifdef WIN32
#define MODE_RWXRWX___ (00770)
#endif /* ifdef WIN32 */

static char standalone=FALSE;
short archwork=(-1),archsub=(-1);
static short a_fileno=(-1);
static char *module="ARCHIVE";

#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	short return_value=TRUE;
	char *filename=NULL,*text=NULL,e=FALSE;
	RDArsrc *mainrsrc=NULL;
	char *temp=NULL;

	if (InitializeSubsystems(argc, argv, module, "AUTO ARCHIVE"))
	{ 
		return;
	}
	if(argc>1)
	{
		if(RDAstrcmp(argv[1],"-s")) 
		{
			filename=argv[1];
		} else { 
			standalone=TRUE;
			if(!isEMPTY(argv[2])) filename=argv[2];
		}
	}
	a_fileno=OPNNRDsec("ARCHIVE","ARCHIVES",TRUE,TRUE,FALSE);
	if(a_fileno!=(-1))
	{
		sprintf(stemp,"%s.zip",(drvspc!=NULL ? drvspc:""));
		tempstr=stralloc(stemp);
		ArchiveBackup(tempstr);
		if(tempstr!=NULL) Rfree(tempstr);

		CLSNRD(a_fileno);
	}
	ShutdownSubsystems();
}

