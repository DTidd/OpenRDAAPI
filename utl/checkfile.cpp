/* checkfile.c - Check File Specifications */
#ifndef WIN32
#define __NAM__ "checkfile.lnx"
#endif
#ifdef WIN32
#define __NAM__ "checkfile.exe"
#endif
#include <app.hpp>
#ifdef WIN32
#include<cstdlib>
#include<io.h>
#endif
#include <mix.hpp>
#include<mknrd.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char RangeMod=FALSE;
char *FromMod=NULL,*ToMod=NULL;
RDA_PFILE *fp=NULL;
int cf_errors=0;

static void doexit(RDArsrc *r)
{
	if(r!=NULL)
	{
		killwindow(r);
		free_rsrc(r);
	}
	if(FromMod!=NULL) Rfree(FromMod);
	if(ToMod!=NULL) Rfree(ToMod);
	ShutdownSubsystems();
	std::exit;
}
static void CHECKFILE(char *mod,char *fil)
{
	NRDfield *field=NULL,*fields=NULL;
	NRDkey *key=NULL,*keyx=NULL;
	NRDpart *part=NULL;
	short fileno=(-1),numflds=0,x=0,y=0;

	fileno=OPNNRD(mod,fil);
	if(fileno!=(-1))
	{
		key=KEYPOINTER(fileno);
		if(key!=NULL)
		{
			keyx=key+0;
			for(y=0,part=keyx->part;y<keyx->numparts;++y,++part)
			{
				field=FLDNRD(fileno,part->name);
				if(field!=NULL)
				{
				if(field->type==VARIABLETEXT || field->type==SCROLLEDTEXT) 
				{
					RDA_fprintf(fp,"Module: [%s] File: [%s] Key Field: [%s] Type: [%s] Length: [%d].\r\n",mod,fil,(field->name!=NULL ? field->name:""),fieldtypedesc[field->type],field->len);
					++cf_errors;
				}
				} else {
					RDA_fprintf(fp,"Module: [%s] File: [%s] Key Part: [%s] Not Even Defined in Field List.\r\n",mod,fil,(part->name!=NULL ? part->name:""));
					++cf_errors;
				}
			}
		}
		fields=FLDPOINTER(fileno);
		numflds=NUMFLDS(fileno);
		for(x=0,field=fields;x<numflds;++x,++field)
		{
			if(field->len==0)
			{
				RDA_fprintf(fp,"Module: [%s] File: [%s] Normal Field: [%s] Type: [%s] Length: [%d].\r\n",mod,fil,(field->name!=NULL ? field->name:""),fieldtypedesc[field->type],field->len);
				++cf_errors;
			}
		}
		CLSNRD(fileno);
	} else {
		RDA_fprintf(fp,"ERROR Module: [%s] File: [%s] failed to open.\r\n",mod,fil);
		++cf_errors;
	}
}
static void okfunction(RDArsrc *r)
{
	int y=0,z=0;
	char *tmp=NULL,*dirx=NULL,*libx=NULL;
	APPlib *modlst=NULL,*filelst=NULL;

	readallwidgets(r);
	FINDRSCGETCHAR(r,"RANGE MODULE",&RangeMod);
	FINDRSCGETSTRING(r,"FROM MODULE",&FromMod);
	FINDRSCGETSTRING(r,"TO MODULE",&ToMod);
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	modlst=APPlibNEW();
	for(z=0;findfile(dirx,"*.FIL",&tmp,(int)z+1);++z) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		if(RDAstrcmp(tmp,"RPTGEN") && RDAstrcmp(tmp,"OLHELP") && RDAstrcmp(tmp,"CONVERSION") && RDAstrcmp(tmp,"MBGUICONV")) 
		{
			if(!RangeMod || ((RDAstrcmp(FromMod,tmp)<=0) && RDAstrcmp(tmp,ToMod)<=0))
			{
				addAPPlib(modlst,tmp);
			}
		}
	}
	if(dirx!=NULL) Rfree(dirx);
	if(modlst->numlibs<1)
	{
		prterr("Error:  No modules found (*.FIL's).");
	} else {
#ifndef WIN32
		fp=RDA_popen("rdaout.lnx","w");
#endif
#ifdef WIN32
		fp=RDA_popen("rdaout.exe","w");
#endif
		SORTAPPlib(modlst);
		for(z=0;z<modlst->numlibs;++z)
		{
			libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlst->libs[z])+10);
#ifndef WIN32
			sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modlst->libs[z]);
#endif
#ifdef WIN32
			sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modlst->libs[z]);
#endif
			filelst=getlibnames(libx,TRUE);
			SORTAPPlib(filelst);
			if(libx!=NULL) Rfree(libx);
			if(filelst->numlibs>0)
			{
				for(y=0;y<filelst->numlibs;++y)
				{
/*
					RDA_fprintf(fp,"Testing [%s] [%s]\r\n",modlst->libs[z],filelst->libs[y]);TRACE;
*/
					CHECKFILE(modlst->libs[z],filelst->libs[y]);
				}
			}
			if(libx!=NULL) Rfree(libx);
			if(filelst!=NULL) freeapplib(filelst);
		}
		if(cf_errors==0)
		{
			RDA_fprintf(fp,"Everything checks out, no cf_errors were found!\r\n");
		}
		if(fp!=NULL) RDA_pclose(fp);
	}
	if(modlst!=NULL) freeapplib(modlst);
	doexit(r);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDArsrc *mainrsrc=NULL;

	RDA_SOFTWARE_TYPE=RDA_ALL_APP;
	if(InitializeSubsystems(argc,argv,"DATABASE","CHECK FILE")) 
	{
		return;
	}
	mainrsrc=RDArsrcNEW("DATABASE","CHECK FILE");
	addstdrsrc(mainrsrc,"RANGE MODULE",BOOLNS,1,&RangeMod,TRUE);
	addstdrsrc(mainrsrc,"FROM MODULE",PLAINTEXT,15,NULL,TRUE);
	addstdrsrc(mainrsrc,"TO MODULE",PLAINTEXT,15,NULL,TRUE);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,okfunction,NULL);
	addrfcbrsrc(mainrsrc,"QUIT",TRUE,doexit,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,doexit,NULL,TRUE);
}
