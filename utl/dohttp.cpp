/* dohttp.c - Execute Browser */
/*lint -library */
#ifndef WIN32
#define __NAM__ "dohttp.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dohttp.exe"
#endif
#include <app.hpp>


#include <rptgen.hpp>
#include <powadd.hpp>
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
	char *url=NULL,delflag=FALSE,*prog_name=NULL; 
	short fileno=(-1),gsv_type=(-1),u_type=(-1),x=0;
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;
	APPlib *args=NULL;

	if(InitializeSubsystems(argc,argv,"UTILITIES","DOHTTP"))
	{
		return;
	}
	if(argc>1) 
	{
		url=stralloc(argv[1]);
	} else {
		url=stralloc("http://www.openrda.com");
	}
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("UTILITIES")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif

	gsv=RDAGenericSetupNew("UTILITIES","BROWSER TYPE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			gsv_type=*gsv->value.short_value;
		} else {
			gsv_type=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	fileno=OPNNRD("SECURITY","USERS");
	if(fileno!=(-1))
	{
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"USER IDENTIFICATION",USERLOGIN);
		if(EQLNRD(fileno,1)) 
		{
			KEYNRD(fileno,1);
		} else {
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			if(!delflag)
			{
				FINDFLDGETSHORT(fileno,"BROWSER TYPE",&u_type);
			}
		}
		CLSNRD(fileno);
	}
	switch(u_type)
	{
		default:
		case (-1):
			break;
#ifndef WIN32
		case 0: /* OPERA */
			prog_name=stralloc("/usr/bin/opera");
			break;
		case 1: /* MOZILLA */
			prog_name=stralloc("/usr/bin/mozilla");
			break;
		case 2: /* FIREFOX */ 
			prog_name=stralloc("/usr/bin/firefox");
			break;
#endif
#ifdef WIN32
		case 0: /* NETSCAPE */
/*
			prog_name=stralloc("C:\\Program Files\\Netscape\\Netscape\\NETSCP.EXE");
*/
			prog_name=stralloc("NETSCP.EXE");
			break;
		case 1: /* MOZILLA */
			prog_name=stralloc("MOZILLA.EXE");
			break;
		case 2: /* EXPLORER */ 
			prog_name=stralloc("C:\\windows\\EXPLORER.EXE");
			break;
		case 3: /* FIREFOX */ 
			prog_name=stralloc("C:\\Program Files\\Mozilla Firefox\\FIREFOX.EXE");
			break;
#endif
	}
	if(isEMPTY(prog_name))
	{
		switch(gsv_type)
		{
			default:
			case (-1):
				break;
#ifndef WIN32
			case 0: /* OPERA */
				prog_name=stralloc("/usr/bin/opera");
				break;
			case 1: /* MOZILLA */
				prog_name=stralloc("/usr/bin/mozilla");
				break;
			case 2: /* FIREFOX */ 
				prog_name=stralloc("/usr/bin/firefox");
				break;
#endif
#ifdef WIN32
			case 0: /* NETSCAPE */
				prog_name=stralloc("C:\\Program Files\\Netscape\\Netscape\\NETSCP.EXE");
				break;
			case 1: /* MOZILLA */
				prog_name=stralloc("MOZILLA.EXE");
				break;
			case 2: /* EXPLORER */ 
				prog_name=stralloc("EXPLORER.EXE");
				break;
			case 3: /* FIREFOX */ 
				prog_name=stralloc("FIREFOX.EXE");
				break;
#endif
		}
	}
	if(isEMPTY(prog_name))
	{
		prterr("Error:  dohttp was unable to dispatch browser.");
	} else {
		args=APPlibNEW();
		for(x=1;x<argc;++x)
		{
			addAPPlib(args,argv[x]);
		}
		ADVExecuteProgramB(prog_name,args,NULL,TRUE);
		if(args!=NULL) freeapplib(args);
	}
	if(prog_name!=NULL) Rfree(prog_name);
	ShutdownSubsystems();
	std::exit;
}
