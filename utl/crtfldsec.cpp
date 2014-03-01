/* crtfieldsec.c - Create Field Security */
/*lint -library */
#ifndef WIN32
#define __NAM__ "crtfldsec.lnx"
#endif
#ifdef WIN32
#define __NAM__ "crtfldsec.exe"
#endif
#include <app.hpp>
#include <mix.hpp>
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
	RDArsrc *r=NULL;
	char *userid=NULL,*module=NULL,*tmp=NULL,*filename=NULL;
	APPlib *files=NULL;
	short fileno=(-1),x=0;
	RDATData *prev=NULL;

	if(InitializeSubsystems(argc,argv,"SECURITY","CREATE FLDSEC")) 
	{
		return;
	}
	tmp=RDA_GetEnv("USER_IDENTIFICATION");
	if(!isEMPTY(tmp)) userid=stralloc(tmp);
	tmp=RDA_GetEnv("MODULE");
	if(!isEMPTY(tmp)) module=stralloc(tmp);
	tmp=RDA_GetEnv("FILE");
	if(!isEMPTY(tmp)) filename=stralloc(tmp);
	if(!isEMPTY(userid) && !isEMPTY(module) && !isEMPTY(filename))
	{
		memset(stemp,0,101);
		sprintf(stemp,"\"[%s][%s]\"",module,filename);
		files=GetFieldsAvailable(stemp,SCRNvirtualSubData,r);
		if(files!=NULL)
		{
			fileno=OPNNRD("SECURITY","FLDSEC");
			if(fileno!=(-1))
			{
				for(x=0;x<files->numlibs;++x)
				{
					ZERNRD(fileno);
					FINDFLDSETSTRING(fileno,"USER IDENTIFICATION",userid);
					FINDFLDSETSTRING(fileno,"MODULE NAME",module);
					FINDFLDSETSTRING(fileno,"FILE NAME",filename);
					FINDFLDSETSTRING(fileno,"FIELD NAME",files->libs[x]);
					if(EQLNRD(fileno,1))
					{
						prev=NULL;
					} else prev=RDATDataNEW(fileno);
					FINDFLDSETCHAR(fileno,"READ FIELD",TRUE);
					FINDFLDSETCHAR(fileno,"WRITE FIELD",TRUE);
					FINDFLDSETCHAR(fileno,"PRINT FIELD",TRUE);
					WRTTRANS(fileno,0,NULL,prev);
					if(prev!=NULL) FreeRDATData(prev);
				}
				CLSNRD(fileno);
			}
			freeapplib(files);
		}
	}
	if(userid!=NULL) Rfree(userid);
	if(module!=NULL) Rfree(module);
	if(filename!=NULL) Rfree(filename);
	ShutdownSubsystems();
	std::exit;
}
