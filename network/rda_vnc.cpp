/* rda_vnc_rc.c - Program to start VNC Sessions at rc */
#ifndef WIN32
#define __NAM__ "rda_vnc.lnx"
#endif
#ifdef WIN32
#define __NAM__ "rda_vnc.exe"
#endif
#include <app.h>
#include<applib.h>
#include<exeprog.h>
#include<gcwd.h>
#include<pwd.h>
#include<miscfunc.h>
/*ADDLIB misc */

char *ufile=NULL;
APPlib *RDAusers=NULL;
int which=0; /* 0-start, 1-keep alive, 2-kill */

static void GetUsers()
{
	FILE *fp=NULL;
	char buffer[5122];
	int x=0,y=0;

	if(RDAusers!=NULL) freeapplib(RDAusers);
	RDAusers=APPlibNEW();
	fp=fopen(ufile,"r");
	if(fp!=NULL)
	{
		while(fgets(buffer,512,fp))
		{
			x=RDAstrlen(buffer);
			for(y=0;y<x;++y)
			{
				if(buffer[y]=='#' && y==0)
				{
					++y;
					buffer[y+1]=0;
					++y;
					break;
				} else if(buffer[y]=='#')
				{
					buffer[y]=0;
					++y;
					break;
				} else if(buffer[y]=='\n' || buffer[y]=='\r')
				{
					if(y+1<x)
					if((buffer[y+1]=='\n' || buffer[y+1]=='\r') && buffer[y]!=buffer[y+1])
					{
						buffer[y]=0;
						++y;
					}
					buffer[y]=0;
					++y;
					break;
				}
			}
			addAPPlib(RDAusers,buffer);
			memset(buffer,0,512);
		}
	}
}
static char FindRunningServer(char *usr)
{
	FILE *fp=NULL;
	APPlib *vncs=NULL;
	char buffer[512],*temp=NULL;
	int x=0,y=0;

	vncs=APPlibNEW();
	fp=popen("ps aef | grep -l Xvnc","r");
	if(fp!=NULL)
	{
		while(fgets(buffer,512,fp))
		{
			x=RDAstrlen(buffer);
			for(y=0;y<x;++y)
			{
				if(buffer[y]=='\n' || buffer[y]=='\r')
				{
					if(y+1<x)
					if((buffer[y+1]=='\n' || buffer[y+1]=='\r') && buffer[y]!=buffer[y+1])
					{
						buffer[y]=0;
						++y;
					}
					buffer[y]=0;
					++y;
					break;
				}
			}
			addAPPlib(vncs,buffer);
			memset(buffer,0,512);
		}
		pclose(fp);
	}
	if(vncs->numlibs>0)
	{
		for(x=0;x<vncs->numlibs;++x)
		{
			temp=strchr(vncs->libs[x],' ');
			if(temp!=NULL) *temp=0;
			if(!RDAstrcmp(temp,usr))	
			{
				freeapplib(vncs);
				return(TRUE);
			}
		}
	}
	freeapplib(vncs);
	return(FALSE);
}
static void ManageVNCservers(int Cmd)
{
	int x=0;
	char *t=NULL;
	APPlib *args=NULL;

	if(RDAusers->numlibs>0)
	{
		for(x=0;x<RDAusers->numlibs;++x)
		{
			switch(Cmd)
			{
				default:
				case 0: /* Start VNC Servers */
					break;
				case 1: /* Keep Alive VNC Servers */
					if(FindRunningServer(RDAusers->libs[x]))
					{
						sprintf(RDAusers->libs[x],"#");
					}
					break;
				case 2: /* Stop VNC Servers */
					break;
				case 3: /* test */
					break;
			}
			t=RDAusers->libs[x];
			if(t[0]!='#')
			{
				args=APPlibNEW();
				memset(stemp,0,101);
				sprintf(stemp,"%d",Cmd);
				addAPPlib(args,stemp);
				addAPPlib(args,RDAusers->libs[x]);
				memset(stemp,0,101);
				sprintf(stemp,"%d",(x+50));
				addAPPlib(args,stemp);
				Execute2Program("rda_vnc1",args);
				if(args!=NULL) freeapplib(args);
			}
		}
	}
}
int main(int argc,char **argv)
{
	char *temp=NULL;

	initrdadiag();
	SETUSERLOGIN();
	SETCWD();
	if(argc<2) 
	{
		which=1;
	} else if(!isEMPTY(argv[1]))
	{
		temp = Rmalloc(strlen(argv[1])+1);
		sprintf(temp,argv[1]);
		stolower(temp);
		if(!RDAstrcmp(temp,"start")) which=0;
		else if(!RDAstrcmp(temp,"stop")) which=2;
		else if(!RDAstrcmp(temp,"test")) which=3;
		else which=1; /* keepalive */
		if(temp!=NULL) Rfree(temp);
	}
	if(argc<3)
	{
		ufile=stralloc("rdausers");
	} else ufile=stralloc(argv[2]);
	GetUsers();
	ManageVNCservers(which);
	if(RDAusers!=NULL) freeapplib(RDAusers);
	if(ufile!=NULL) Rfree(ufile);
	exit(0);
}
