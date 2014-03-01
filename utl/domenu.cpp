/* menu.c - Program to execute a specific menu */
#ifndef __NAM__
#ifndef WIN32
#define __NAM__ "domenu.lnx"
#endif
#ifdef WIN32
#define __NAM__ "domenu.exe"
#endif
#endif

#include <app.hpp>
#include <mix.hpp>
#include <curl/curl.h>
#include <curl/easy.h>
#include <trans.hpp>
#include <gui.hpp>
/*ADDLIB curl */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */


static APPlib *AVLcommands=NULL;
static char KEEP_ALIVE=TRUE;
char use_proxy=FALSE,*http_proxy=NULL,cfu=FALSE;
int http_proxy_port=1080;

struct MemoryStruct {
	char *memory;
	int size;
};

static int WriteMemoryCallback(void *ptr,int size,int nmemb,void *data)
{
	int realsize=size*nmemb;
	struct MemoryStruct *mem=(struct MemoryStruct *)data;

	mem->memory=(char *)Rrealloc(mem->memory,mem->size+realsize+1);
	if(mem->memory)
	{
		memcpy(&(mem->memory[mem->size]),ptr,realsize);
		mem->size+=realsize;
		mem->memory[mem->size]=0;
	}
	return(realsize);
}
static void RunRDAUpdate(RDArsrc *mainrsrc,char *m)
{
	GUI_OpenURL("http://wiki.openrda.com/wiki/index.php/Category:PROGRAM_UPDATES");
}
static void CheckRDAUpdates(RDArsrc *mainrsrc,char *m)
{
	char *temp1=NULL,name[256],*temp=NULL,mdate[11],*date1=NULL,*libx=NULL;
	CURL *RDAcurl=NULL;
	struct MemoryStruct chunk;
	RDAwdgt *wdgt=NULL;
	int x=0;
	CURLcode res=0;
	RDAGenericSetup *gsv=NULL;


	if(isEMPTY(m)) return;
	temp1=Rmalloc(RDAstrlen(m)+15);
	sprintf(temp1,"DUE FOR %s UPDATE",m);
	chunk.memory=NULL;
	chunk.size=0;
	x=FINDWDGT(mainrsrc->scn,temp1);
	if(x!=(-1))
	{
		wdgt=mainrsrc->scn->wdgts+x;
		addbtnrsrc(mainrsrc,temp1,cfu,RunRDAUpdate,m);
		if(cfu)
		{
			RDAcurl=curl_easy_init();
			memset(name,0,256);
			if(XPERT_SETUP->software_type<2)
			{
#ifdef WIN32
				sprintf(name,"http://66.135.38.166/checkupdates/xpert/winxp_mysql-%s.lst",m);
#else
#ifndef RDA_64BITS
				sprintf(name,"http://66.135.38.166/checkupdates/xpert/lnx_mysql-%s.lst",m);
#endif 
#ifdef RDA_64BITS
				sprintf(name,"http://66.135.38.166/checkupdates/xpert/lnx_mysql64-%s.lst",m);
#endif 
#endif
			} else {
#ifdef WIN32
				sprintf(name,"http://66.135.38.166/checkupdates/mbgui/winxp_mysql-%s.lst",m);
#else
#ifndef RDA_64BITS
				sprintf(name,"http://66.135.38.166/checkupdates/mbgui/lnx_mysql-%s.lst",m);
#endif 
#ifdef RDA_64BITS
				sprintf(name,"http://66.135.38.166/checkupdates/mbgui/lnx_mysql64-%s.lst",m);
#endif 
#endif
			}
			curl_easy_setopt(RDAcurl,CURLOPT_URL,name);
			if(use_proxy)
			{
				sprintf(stemp,"%s:%d",(http_proxy!=NULL ? http_proxy:""),http_proxy_port);
				curl_easy_setopt(RDAcurl,CURLOPT_PROXY,stemp);
			}
			curl_easy_setopt(RDAcurl,CURLOPT_VERBOSE,FALSE);
			curl_easy_setopt(RDAcurl,CURLOPT_WRITEFUNCTION,WriteMemoryCallback);
			curl_easy_setopt(RDAcurl,CURLOPT_FILE,(void *)&chunk);
			curl_easy_setopt(RDAcurl,CURLOPT_USERAGENT,"libcurl-agent/1.0");
			curl_easy_setopt(RDAcurl,CURLOPT_TIMEOUT,5);
			res=curl_easy_perform(RDAcurl);
			curl_easy_cleanup(RDAcurl);
			if(CURLE_OK!=res)
			{
				FINDRSCSETSENSITIVE(mainrsrc,temp1,FALSE);
			} else {
				temp=RDAstrstr(chunk.memory,".7z ");
				if(temp!=NULL)
				{
					temp+=4;
					memset(stemp,0,101);
					sprintf(stemp,"%.10s %s Update is Available!  Schedule your installation.",temp,m);
					if(wdgt->label!=NULL) Rfree(wdgt->label);
					wdgt->label=stralloc(stemp);
					memset(mdate,0,11);
					libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(m)+11);
#ifndef WIN32
					sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,m);
#endif
#ifdef WIN32
					sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,m);
#endif
					gsv=RDAGenericSetupNew(m,"COMPILE DATE");
					if(gsv!=NULL)
					{
						if(!getRDAGenericSetupbin(libx,gsv))
						{
							date1=gsv->value.string_value;
							sprintf(mdate,"%.4s-%.2s-%.2s",&date1[6],&date1[0],&date1[3]);
						}
					}
					if(gsv!=NULL) FreeRDAGenericSetup(gsv);
					if(libx!=NULL) Rfree(libx);
					if(strncmp(temp,mdate,10)<=0)
					{
						FINDRSCSETSENSITIVE(mainrsrc,temp1,FALSE);
					}
				} else {
					FINDRSCSETSENSITIVE(mainrsrc,temp1,FALSE);
				}
			}
		} else {
			FINDRSCSETSENSITIVE(mainrsrc,temp1,FALSE);
		}
	}
	if(chunk.memory) Rfree(chunk.memory);
	if(temp1!=NULL) Rfree(temp1);
}
void quitmenu(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	if(AVLcommands!=NULL) freeapplib(AVLcommands);
	ShutdownSubsystems();
}
void runcommand(RDArsrc *mainrsrc,char *command)
{
	short x,is_menu=FALSE;
	char *errdesc=NULL,*cmd=NULL,*tmp=NULL;
	int offset=0;
	char *srchstr=".EXT";
#if defined(RS6000)
	char *repstr=".rsc";
#elif defined(LINUX)
	char *repstr=".lnx";
#elif defined(LINUX2_2) || defined(UBUNTU_OS)
	char *repstr=".lnx";
#elif defined(UNIXWARE7)
	char *repstr=".unw";
#elif defined(UNIXWARE21)
	char *repstr=".unw";
#elif defined(SCO386)
	char *repstr=".sco";
#elif defined(SCO50)
	char *repstr=".sco";
#elif defined(SCO50P)
	char *repstr=".sco";
#elif defined(INT386)
	char *repstr=".unx";
#elif defined(SUNX86)
	char *repstr=".sun";
#elif defined(WIN32)
	char *repstr=".exe";
	char *repstr1=".bat";
#else 
	char *repstr=".EXT";
#endif

#ifndef WIN32
	cmd=Rmalloc(RDAstrlen(command)+3);
	sprintf(cmd,"%s &",command);
#endif
#ifdef WIN32
	cmd=stralloc(command);
#endif
	tmp=RDAstrstr(cmd,srchstr);
	if(tmp!=NULL)
	{
		offset=((int)(tmp-cmd));
		for(x=0;x<(RDAstrlen(srchstr));++x)
		{
			cmd[x+offset]=repstr[x];
		}
		
	}
	tmp=RDAstrstr(cmd,"domenu");
	if(tmp!=NULL) is_menu=TRUE;
	x=system_command(cmd);
	if(cmd!=NULL) Rfree(cmd);
	if(x)
	{
#ifdef WIN32
		cmd=stralloc(command);
		tmp=RDAstrstr(cmd,srchstr);
		if(tmp!=NULL)
		{
			offset=((int)(tmp-cmd));
			for(x=0;x<(RDAstrlen(srchstr));++x)
			{
				cmd[x+offset]=repstr1[x];
			}
			
		}
		x=system_command(cmd);
		if(cmd!=NULL) Rfree(cmd);
		if(x)
		{
			errdesc=Rmalloc(RDAstrlen(command)+87);
			sprintf(errdesc,"The command [%s] won't execute. Check your syntax and/or try executing it at the command prompt.",command);
			ERRORDIALOG("COMMAND FAILED",errdesc,NULL,FALSE);
			if(errdesc!=NULL) Rfree(errdesc);
		} else {
			if(is_menu && XPERT_SETUP->SimpleMenu && (RDAstrcmp(mainrsrc->screen,"MAIN MENU") || KEEP_ALIVE==FALSE))
			{
				killwindow(mainrsrc);
				quitmenu(mainrsrc);
				std::exit;
			}
		}
#endif
#ifndef WIN32
		errdesc=Rmalloc(RDAstrlen(command)+97);
		sprintf(errdesc,"The command [%s] won't execute. Check your syntax and/or try executing it at the command prompt.",command);
		ERRORDIALOG("COMMAND FAILED",errdesc,NULL,FALSE);
		if(errdesc!=NULL) Rfree(errdesc);
#endif
	} else {
		if(is_menu && XPERT_SETUP->SimpleMenu && (RDAstrcmp(mainrsrc->screen,"MAIN MENU") || KEEP_ALIVE==FALSE))
		{
			killwindow(mainrsrc);
			quitmenu(mainrsrc);
			std::exit;
		}
	}
}
char *ReturnProcessName(char *command)
{
	char *temp=NULL,*progname=NULL,*pname=NULL,*m=NULL;
	short Texit=FALSE;

	temp=command;
	progname=READargument(&temp,&Texit);
	if(!Texit)
	{
		m=READargument(&temp,&Texit);
	}
	if(!Texit)
	{
		pname=READargument(&temp,&Texit);
	}
	if(progname!=NULL) Rfree(progname);
	if(m!=NULL) Rfree(m);
	return(pname);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	char *screen=NULL,*dirx=NULL,*master=NULL;
	char *tmp=NULL,*mod=NULL;
	short allfuncs=TRUE,allfiles=TRUE,style=0;
	int x=0,y=0,z=0;
	APPlib *menuitms=NULL,*menulib=NULL,*names=NULL;
	APPlib *dir_libs=NULL,*AVLnames=NULL,*commands=NULL;
	RDAmenu *menu=NULL;
	RDArsrc *mainrsrc=NULL;
	RDAwdgt *wdgt=NULL;
	char *temp=NULL,*tc=NULL;
	int tc_count=0;
	char *libx=NULL,*focus_resource=NULL;
	RDAGenericSetup *gsv=NULL;


	if(argc<2) 
	{
		tmp="UTILITIES";
#ifdef WIN32
		RotateErrorLog();
#endif
	}
	else if(!isEMPTY(argv[1]))
	{
		tmp=argv[1];
	}
	else
	{	
		tmp="UTILITIES";
	}
	if(InitializeSubsystems(argc,argv,tmp,(argc<2 ? "MAIN MENU":argv[2]))) 
	{
		return;
	}
	if((!RDAstrcmp(tmp,"UTILITIES")) && argc<2)
	{
		CheckDiskSpace();
	}
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("UTILITIES")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif

	KEEP_ALIVE=TRUE;
	gsv=RDAGenericSetupNew("UTILITIES","KEEP ALIVE MAIN MENU");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			KEEP_ALIVE=*gsv->value.string_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("UTILITIES","SKIP DOMENU TRANSVERSALS");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			SKIP_DOMENU_TRANSVERSAL=*gsv->value.string_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	curl_global_init(CURL_GLOBAL_DEFAULT);	
	gsv=RDAGenericSetupNew("UTILITIES","CHECK FOR UPDATES");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			cfu=*gsv->value.string_value;
		} else {
			cfu=FALSE;
		}
	}
	if(cfu)
	{
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
		gsv=RDAGenericSetupNew("UTILITIES","FTP USE PROXY");
		if(gsv!=NULL)
		{
			if(!getRDAGenericSetupbin(libx,gsv))
			{
				use_proxy=*gsv->value.string_value;
			} else {
				use_proxy=FALSE;
			}
		}
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
		gsv=RDAGenericSetupNew("UTILITIES","FTP PROXY");
		if(gsv!=NULL)
		{
			if(!getRDAGenericSetupbin(libx,gsv))
			{
				http_proxy=stralloc(gsv->value.string_value);
			} else {
				http_proxy=NULL;
			}
		}
		if(gsv!=NULL) FreeRDAGenericSetup(gsv);
		gsv=RDAGenericSetupNew("UTILITIES","FTP PROXY PORT");
		if(gsv!=NULL)
		{
			if(!getRDAGenericSetupbin(libx,gsv))
			{
				http_proxy_port=*gsv->value.integer_value;
			} else {
				http_proxy_port=1080;
			}
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	tmp=NULL;
	if(argc>3 && !isEMPTY(argv[3]) && !isEMPTY(argv[1]))
	{
		focus_resource=stralloc(argv[3]);
	}
	if(argc>2 && !isEMPTY(argv[2]) && !isEMPTY(argv[1]))
	{
		mod=stralloc(argv[1]);
		screen=stralloc(argv[2]);
	} else {
		mod=stralloc("UTILITIES");
		screen=stralloc("MAIN MENU");
	}
	style=Users_DomenuStyle();
	mainrsrc=RDArsrcNEW(mod,screen);
	if(mainrsrc->scn==NULL)
	{
		mainrsrc->scn=RDAscrnNEW(mod,screen);
		if(findscrnbin(mainrsrc->scn)) 
		{
			prterr("Error couldn't read Screen Binary [%s] [%s] from Binary Library [%s.SCN]",mainrsrc->module,(mainrsrc->screen!=NULL ? mainrsrc->screen:""),mainrsrc->module);
			free_scrn(mainrsrc->scn);
			mainrsrc->scn=NULL;
			free_rsrc(mainrsrc);
			Rfree(mod);
			Rfree(screen);
			return;
		} 
	}
	if(mod!=NULL) { Rfree(mod); mod=NULL; }
	dir_libs=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(USERLOGIN)+3);
#ifndef WIN32
	sprintf(dirx,"%s/%s/",CURRENTDIRECTORY,USERLOGIN);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\%s\\",CURRENTDIRECTORY,USERLOGIN);
#endif
	addAPPlib(dir_libs,dirx);
	if(dirx!=NULL) { Rfree(dirx); dirx=NULL; }
	if(!isEMPTY(RDA_GROUP))
	{
		dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(RDA_GROUP)+7);
#ifndef WIN32
		sprintf(dirx,"%s/%s/",CURRENTDIRECTORY,RDA_GROUP);
#endif
#ifdef WIN32
		sprintf(dirx,"%s\\%s\\",CURRENTDIRECTORY,RDA_GROUP);
#endif
		addAPPlib(dir_libs,dirx);
		if(dirx!=NULL) { Rfree(dirx); dirx=NULL; }
	}
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+7);
#ifndef WIN32
	sprintf(dirx,"%s/site/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\site\\",CURRENTDIRECTORY);
#endif
	addAPPlib(dir_libs,dirx);
	if(dirx!=NULL) Rfree(dirx);
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	addAPPlib(dir_libs,dirx);
	if(dirx!=NULL) { Rfree(dirx); dirx=NULL; }
	menulib=APPlibNEW();
	commands=APPlibNEW();
	names=APPlibNEW();
	for(z=0;z<dir_libs->numlibs;++z)
	{
		for(x=0;findfile(dir_libs->libs[z],"*.MNU",&tmp,(int)x+1);++x)
		{
			mod=Rmalloc(RDAstrlen(tmp)-3);
			strncpy(mod,tmp,(RDAstrlen(tmp)-4));
			mod[RDAstrlen(tmp)-4]=0;
			if(MODsecurity(mod,&allfuncs,&allfiles,TRUE,FALSE,FALSE,FALSE))
			{
				addAPPlib(menulib,tmp);
			}
			if(mod!=NULL) { Rfree(mod); mod=NULL; }
			if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
		}
		for(x=0;x<menulib->numlibs;++x)
		{
			if(master!=NULL)
			{
				master=Rrealloc(master,RDAstrlen(dir_libs->libs[z])+RDAstrlen(menulib->libs[x])+1);
			} else {
				master=Rmalloc(RDAstrlen(dir_libs->libs[z])+RDAstrlen(menulib->libs[x])+1);
			}
			sprintf(master,"%s%s",dir_libs->libs[z],menulib->libs[x]);
			menuitms=getunsortedlibnames(master,FALSE);
			if(menuitms!=NULL)
			{
				for(y=0;y<menuitms->numlibs;++y)
				{
					if(menu!=NULL) { free_menu(menu); menu=NULL; }
					mod=Rmalloc(RDAstrlen(menulib->libs[x])-3);
					strncpy(mod,menulib->libs[x],(RDAstrlen(menulib->libs[x])-4));
					mod[RDAstrlen(menulib->libs[x])-4]=0;
					MODsecurity(mod,&allfuncs,&allfiles,TRUE,FALSE,FALSE,FALSE);
					menu=RDAmenuNEW(menuitms->libs[y],NULL);
					getmenubin(master,menu);
					if(!allfuncs)
					{
						if(!strncmp(menu->command,"doadd",5) || !strncmp(menu->command,"DOADD",5))
						{
							tc_count=0;
							tc=menu->command;
							for(;*tc;++tc) if(*tc=='\"') ++tc_count;
							if(tc_count>2)
							{
								tmp=strchr(menu->command,'\"');
								++tmp;
								--tc_count;

								while(tc_count>1)
								{
									tmp=strchr(tmp,'\"');
									++tmp;
									--tc_count;
								}
								if(tmp!=NULL)
								{
									++tmp;
									if(*tmp)
									{
										temp=stralloc(tmp);
										temp[RDAstrlen(tmp)-1]=0;
										allfiles=PROCsecurity(mod,temp);
										if(temp!=NULL) Rfree(temp);
									}
								}
							} else if(tc_count<2)
							{
								for(tc_count=RDAstrlen(menu->command);tc_count>0;--tc_count)
								{
									tmp=menu->command+(tc_count-1);
									if(*tmp==' ') break;
								}
								tmp=menu->command+(tc_count-1);
								temp=stralloc(tmp);
							
								temp[RDAstrlen(tmp)-1]=0;
								allfiles=PROCsecurity(mod,temp);
								if(temp!=NULL) Rfree(temp);
							} else {
								tmp=strchr(menu->command,'\"');
								if(tmp!=NULL)
								{
									++tmp;
									if(*tmp)
									{
										temp=stralloc(tmp);
										temp[RDAstrlen(tmp)-1]=0;
										allfiles=PROCsecurity(mod,temp);
										if(temp!=NULL) Rfree(temp);
									}
								}
							}
							if(tmp==NULL && !RDAstrcmp(menu->command,"domenu.EXT"))
							{
								allfiles=PROCsecurity(mod,"MAIN MENU");
							}
						} else if(!strncmp(menu->command,"doreport",8) || !strncmp(menu->command,"DOREPORT",8))
						{
							tc_count=0;
							tc=menu->command;
							for(;*tc;++tc) if(*tc=='\"') ++tc_count;
							if(tc_count>2)
							{
								tmp=strchr(menu->command,'\"');
								++tmp;
								--tc_count;
	
								while(tc_count>1)
								{
									tmp=strchr(tmp,'\"');
									++tmp;
									--tc_count;
								}
							} else if(tc_count>0)
							{
								tmp=strchr(menu->command,'\"');
							} else {
								tmp=strchr(menu->command,' ');
								if(tmp!=NULL)
								{
									++tmp;
									tmp=strchr(tmp,' ');
								}
							}
							if(tmp==NULL)
							{
								tmp=menu->command;
							}
							if(tmp!=NULL)
							{
								++tmp;
								if(*tmp)
								{
								temp=stralloc(tmp);
							
								if(tc_count) temp[RDAstrlen(tmp)-1]=0;
								allfiles=PROCsecurity(mod,temp);
								if(temp!=NULL) Rfree(temp);
								}
								tmp=NULL;
							} else if(!RDAstrcmp(menu->command,"domenu.EXT"))
							{
								allfiles=PROCsecurity(mod,"MAIN MENU");
							}
						} else if(!strncmp(menu->command,"mtnmst",6) || !strncmp(menu->command,"MTNMST",6))
						{
							tc_count=0;
							tc=menu->command;
							for(;*tc;++tc) if(*tc=='\"') ++tc_count;
							if(tc_count>2)
							{
								tmp=strchr(menu->command,'\"');
								++tmp;
								--tc_count;
	
								while(tc_count>1)
								{
									tmp=strchr(tmp,'\"');
									++tmp;
									--tc_count;
								}
							} else {
								tmp=strchr(menu->command,'\"');
							}
							if(tmp!=NULL)
							{
								++tmp;
								if(*tmp)
								{
								temp=stralloc(tmp);
							
								temp[RDAstrlen(tmp)-1]=0;
								allfiles=PROCsecurity(mod,temp);
								if(temp!=NULL) Rfree(temp);
								}
							} else if(!RDAstrcmp(menu->command,"domenu.EXT"))
							{
								allfiles=PROCsecurity(mod,"MAIN MENU");
							}
						} else if(!strncmp(menu->command,"domenu",6) || !strncmp(menu->command,"DOMENU",6))
						{
							tmp=ReturnProcessName(menu->command);
							if(tmp!=NULL)
							{
								allfiles=PROCsecurity(mod,tmp);
							} else if(!RDAstrcmp(menu->command,"domenu.EXT"))
							{
								allfiles=PROCsecurity(mod,"MAIN MENU");
							}
							if(tmp!=NULL) Rfree(tmp);
						} else if(!strncmp(menu->command,"rdaout",6) || 
							!strncmp(menu->command,"dfmatrix",8) ||
							!strncmp(menu->command,"dftable",7))
						{
							allfiles=PROCsecurity(mod,menu->name);
						} else {
							allfiles=PROCsecurity(mod,menu->name);
						}
					}
					if(allfuncs==TRUE || allfiles==TRUE)
					{
						if(addAPPlibNoDuplicates(names,menu->name)!=(-1))
						{
							addAPPlib(commands,menu->command);
						}
					}
				}
			}
			if(menuitms!=NULL) { freeapplib(menuitms); menuitms=NULL; }
			if(master!=NULL) { Rfree(master); master=NULL; }
		}
	}
	AVLnames=APPlibNEW();
	AVLcommands=APPlibNEW();
	if(XPERT_SETUP->FINMGT==TRUE) CheckRDAUpdates(mainrsrc,"FINMGT");
	if(XPERT_SETUP->VENPMT==TRUE) CheckRDAUpdates(mainrsrc,"VENPMT");
	if(XPERT_SETUP->VENPMT==TRUE) CheckRDAUpdates(mainrsrc,"VENSAFE");
	if(XPERT_SETUP->PURORD==TRUE) CheckRDAUpdates(mainrsrc,"PURORD");
	if(XPERT_SETUP->BNKREC==TRUE) CheckRDAUpdates(mainrsrc,"BNKREC");
	if(XPERT_SETUP->FIXASS==TRUE) CheckRDAUpdates(mainrsrc,"FIXASS");
	if(XPERT_SETUP->PRSNNL==TRUE) CheckRDAUpdates(mainrsrc,"PRSNNL");
	if(XPERT_SETUP->PAYROLL==TRUE) CheckRDAUpdates(mainrsrc,"PAYROLL");
	if(XPERT_SETUP->POSTRK==TRUE) CheckRDAUpdates(mainrsrc,"POSTRK");
	if(XPERT_SETUP->BFTMGT==TRUE) CheckRDAUpdates(mainrsrc,"BFTMGT");
	if(XPERT_SETUP->APPMGT==TRUE) CheckRDAUpdates(mainrsrc,"APPMGT");
	if(XPERT_SETUP->EMPABS==TRUE) CheckRDAUpdates(mainrsrc,"EMPABS");
	if(XPERT_SETUP->LVEMGT==TRUE) CheckRDAUpdates(mainrsrc,"LVEMGT");
	if(XPERT_SETUP->SUBMGT==TRUE) CheckRDAUpdates(mainrsrc,"SUBMGT");
	if(XPERT_SETUP->LVSIMP==TRUE) CheckRDAUpdates(mainrsrc,"LVSIMP");
	if(XPERT_SETUP->INVCTL==TRUE) CheckRDAUpdates(mainrsrc,"INVCTL");
	if(XPERT_SETUP->RLVEMGT==TRUE) CheckRDAUpdates(mainrsrc,"RLVEMGT");
	if(XPERT_SETUP->REQMGT==TRUE) CheckRDAUpdates(mainrsrc,"REQMGT");
	if(XPERT_SETUP->RLSTMGT==TRUE) CheckRDAUpdates(mainrsrc,"RLSTMGT");
	if(XPERT_SETUP->CSHDWR==TRUE) CheckRDAUpdates(mainrsrc,"CSHDWR");
	if(XPERT_SETUP->DMVREG==TRUE) CheckRDAUpdates(mainrsrc,"DMVREG");
	if(XPERT_SETUP->PROPERTY==TRUE) CheckRDAUpdates(mainrsrc,"PROPERTY");
	if(XPERT_SETUP->MISCBILL==TRUE) CheckRDAUpdates(mainrsrc,"MISCBILL");
	if(XPERT_SETUP->CATALOGUE==TRUE) CheckRDAUpdates(mainrsrc,"CATALOGUE");
	if(XPERT_SETUP->BLDPRMT==TRUE) CheckRDAUpdates(mainrsrc,"BLDPRMT");
	if(XPERT_SETUP->ARCHIVE==TRUE) CheckRDAUpdates(mainrsrc,"ARCHIVE");
	if(XPERT_SETUP->BUDPREP==TRUE) CheckRDAUpdates(mainrsrc,"BUDPREP");
	if(XPERT_SETUP->UTLBLL==TRUE) CheckRDAUpdates(mainrsrc,"UTLBLL");
	if(XPERT_SETUP->OCCTAX==TRUE) CheckRDAUpdates(mainrsrc,"OCCTAX");
	if(XPERT_SETUP->FINMGT==TRUE) CheckRDAUpdates(mainrsrc,"UTILITIES");
	for(z=0,wdgt=mainrsrc->scn->wdgts;z<mainrsrc->scn->numwdgts;++z,++wdgt)
	{
		for(x=0;x<names->numlibs;++x)
		{
			if(!RDAstrcmp(wdgt->resource_name,names->libs[x])) break;
		}
		if(x<names->numlibs)
		{
			addAPPlib(AVLnames,names->libs[x]);
			addAPPlib(AVLcommands,commands->libs[x]);
/* Recent Change 08-27-2002 */
		} else if(RDAstrstr(wdgt->resource_name,"DUE FOR ") && RDAstrstr(wdgt->resource_name," UPDATE"))
		{
		} else if(RDAstrcmp(wdgt->resource_name,"HELP") &&
			RDAstrcmp(wdgt->resource_name,"QUIT") &&
			RDAstrcmp(wdgt->resource_name,"PRINT RESOURCES") && style==0 && (allfiles || allfuncs))
		{
			addbtnrsrc(mainrsrc,wdgt->resource_name,FALSE,NULL,NULL);
			FINDRSCSETSENSITIVE(mainrsrc,wdgt->resource_name,FALSE);
			FINDRSCSETEDITABLE(mainrsrc,wdgt->resource_name,FALSE);
/* Recent Change 08-27-2002 */
		}
	}
	for(x=0;x<AVLnames->numlibs;++x)
	{
		addbtnrsrc(mainrsrc,AVLnames->libs[x],TRUE,runcommand,
			AVLcommands->libs[x]);
	}
	if(dir_libs!=NULL) freeapplib(dir_libs);
	if(menulib!=NULL) freeapplib(menulib);
	if(names!=NULL) freeapplib(names);
	if(AVLnames!=NULL) freeapplib(AVLnames);
	if(commands!=NULL) freeapplib(commands);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitmenu,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	FINDRSCSETINPUTFOCUS(mainrsrc,focus_resource);
	APPmakescrn(mainrsrc,TRUE,quitmenu,NULL,TRUE);
	if(focus_resource!=NULL) Rfree(focus_resource);
}
