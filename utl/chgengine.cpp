/* dfnrd.c - Define Datafiles */
#ifndef WIN32
#define __NAM__ "chgengine.lnx"
#endif
#ifdef WIN32
#define __NAM__ "chgengine.exe"
#endif
#include <app.hpp>
#ifdef WIN32
#include<cstdlib>
#include<io.h>
#endif
#include<mknrd.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

APPlib *dbengines=NULL;
char RangeMod=FALSE,DoSame=FALSE;
char *FromMod=NULL,*ToMod=NULL;

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
}
static void okfunction(RDArsrc *r)
{
	int engine=0,y=0,z=0;
	short enginex=0;
	char *tmp=NULL,*dirx=NULL,*libx=NULL;
	APPlib *modlst=NULL,*filelst=NULL;
	DBsort *sort=NULL;
	short x=(-1),numfields=0,numkeys=0,w=0;
	char *tempstr=NULL,*serverx=NULL;
	NRDkey *key=NULL,*keys=NULL;
	NRDpart *part=NULL;
	NRDfield *field=NULL,*fields=NULL;

	readallwidgets(r);
	FINDRSCGETCHAR(r,"CHANGE ALL",&DoSame);
	FINDRSCGETCHAR(r,"RANGE MODULE",&RangeMod);
	FINDRSCGETSTRING(r,"FROM MODULE",&FromMod);
	FINDRSCGETSTRING(r,"TO MODULE",&ToMod);
	FINDRSCGETINT(r,"DATABASE ENGINE",&engine);
	for(x=0;x<NE(supported_engines);++x)
	{
		if(!RDAstrcmp(supported_engines[x],dbengines->libs[engine]))
			break;
	}
	engine=x;

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
		if(RDAstrcmp(tmp,"OLHELP") && RDAstrcmp(tmp,"SECURITY") && RDAstrcmp(tmp,"RPTGEN") && RDAstrcmp(tmp,"CONVERSION") && RDAstrcmp(tmp,"MBGUICONV")) 
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
					sort=DBsortNEW(modlst->libs[z],filelst->libs[y],0);
					if(sort!=NULL)
					{
						if(!ADVGETBIN(modlst->libs[z],filelst->libs[y],&enginex,&numfields,&fields,&numkeys,&keys,&serverx))
						{
							if(enginex!=engine || DoSame==TRUE)
							{
							for(x=0,field=fields;x<numfields;++x,++field)
							{
								addDBfield(sort,field->name,field->type,field->len);
							}	
							for(x=0,key=keys;x<numkeys;++x,++key)
							{
								addDBkey(sort,key->name);
								for(w=0,part=key->part;w<key->numparts;++w,++part)
								{
									addDBkeypart(sort,(x+1),part->name);
								}
							}

							MergeDATABASE(sort);
							SetupFileDefinition(sort);
							sort->engine=engine;
							x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
							if(x==(-1))
							{
								tempstr=Rmalloc(171+sizeof(sort->module)+sizeof(sort->filename));
								sprintf(tempstr,"The Database File Changes may not be saved to the specified library: [%s][%s].  Check your permissions and re-try.  Call your installer for further instructions.",sort->module,sort->filename);
								prterr("ERROR: Cannot Save Database File, [%s]",tempstr);
								ERRORDIALOG("Cannot Save Database File",tempstr,NULL,FALSE);
								if(tempstr!=NULL) Rfree(tempstr);
							} else {
								CLSNRD(x);
							}
							}
							if(serverx!=NULL) Rfree(serverx);
							if(fields!=NULL)
							{
								for(x=0,field=fields;x<numfields;++x,++field)
								{
									FreeNRDfield(field);
								}
								Rfree(fields);
							}
							if(keys!=NULL)
							{
								for(x=0,key=keys;x<numkeys;++x,++key)
								{
									if(key->name!=NULL) Rfree(key->name);
									if(key->part!=NULL)
									{
										for(w=0,part=key->part;w<key->numparts;++w,++part)
										{
											if(part->name!=NULL) Rfree(part->name);
										}
										Rfree(key->part);
									}
								}
								Rfree(keys);
							}
						}
						if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
					}
				}
			}
			if(libx!=NULL) Rfree(libx);
			if(filelst!=NULL) freeapplib(filelst);
		}
	}
	if(modlst!=NULL) freeapplib(modlst);
	doexit(r);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
#endif
{
	RDArsrc *mainrsrc=NULL;
	int x=0;

	RDA_SOFTWARE_TYPE=RDA_ALL_APP;
	if(InitializeSubsystems(argc,argv,"DATABASE","CHANGE ENGINE")) 
	{
		return;
	}
	if(dbengines!=NULL) freeapplib(dbengines);
	dbengines=APPlibNEW();
	addAPPlib(dbengines,supported_engines[0]);
#ifdef INFORMIX_ISAM
	addAPPlib(dbengines,supported_engines[1]);
#endif
#ifdef USE_MYISAM
	addAPPlib(dbengines,supported_engines[2]);
#endif
#ifdef USE_MYSQL
	addAPPlib(dbengines,supported_engines[3]);
	addAPPlib(dbengines,supported_engines[4]);
#endif
#ifdef USE_CTREE
	addAPPlib(dbengines,supported_engines[5]);
	addAPPlib(dbengines,supported_engines[6]);
#endif
#ifdef USE_BERKELEY_BTREE
	addAPPlib(dbengines,supported_engines[7]);
#endif
	mainrsrc=RDArsrcNEW("DATABASE","CHANGE ENGINE");
	addstdrsrc(mainrsrc,"CHANGE ALL",BOOLNS,1,&DoSame,TRUE);
	addlstrsrc(mainrsrc,"DATABASE ENGINE",&x,TRUE,NULL,dbengines->numlibs,
		&dbengines->libs,NULL);
	addstdrsrc(mainrsrc,"RANGE MODULE",BOOLNS,1,&RangeMod,TRUE);
	addstdrsrc(mainrsrc,"FROM MODULE",PLAINTEXT,15,NULL,TRUE);
	addstdrsrc(mainrsrc,"TO MODULE",PLAINTEXT,15,NULL,TRUE);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,okfunction,NULL);
	addrfcbrsrc(mainrsrc,"QUIT",TRUE,doexit,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,doexit,NULL,TRUE);
}
