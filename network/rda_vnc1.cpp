/* rda_vnc_rc.c - Program to start VNC Sessions at rc */
#ifndef WIN32
#define __NAM__ "rda_vnc1.lnx"
#endif
#ifdef WIN32
#define __NAM__ "rda_vnc1.exe"
#endif
#include <app.h>
#include<applib.h>
#include<exeprog.h>
#include<gcwd.h>
#include<pwd.h>
#include<miscfunc.h>
/*ADDLIB misc */

static void ManageVNCservers(int Cmd,char *user,int disnum)
{
	int status=0;
	char *od=NULL;
	struct passwd *p;

	od=stralloc(CURRENTDIRECTORY);
	p=getpwnam(user);
	status=setreuid(p->pw_uid,p->pw_uid);
	if(status!=(-1))
	{
		setenv("HOME",p->pw_dir,TRUE);
		setenv("LOGNAME",user,TRUE);
		setenv("USER",user,TRUE);
		setenv("USERNAME",user,TRUE);
		memset(stemp,0,101);
		sprintf(stemp,"%s/.bashrc",p->pw_dir);
		setenv("BASH_ENV",user,TRUE);
		memset(stemp,0,101);
		sprintf(stemp,"%s",(p->pw_dir!=NULL ? p->pw_dir:""));
		RCHGCWD(stemp);	
		memset(stemp,0,101);
		switch(Cmd)
		{
			default:
			case 0: /* Start VNC Servers */
				sprintf(stemp,"/usr/X11R6/bin/vncserver :%d -ac",disnum);
				break;
			case 1: /* Keep Alive VNC Servers */
				sprintf(stemp,"/tmp/.X%d-lock",disnum);
				unlink(stemp);
				sprintf(stemp,"/usr/X11R6/bin/vncserver :%d -ac",disnum);
				break;
			case 2: /* Stop VNC Servers */
				sprintf(stemp,"/tmp/.X%d-lock",disnum);
				unlink(stemp);
				sprintf(stemp,"/tmp/.X11-unix/X%d",disnum);
				unlink(stemp);
				sprintf(stemp,"./.vnc/%s%d.log","*",disnum);
				unlink(stemp);
				sprintf(stemp,"/usr/X11R6/bin/vncserver -kill :%d",disnum);
				break;
			case 3: /* test */
				sprintf(stemp,"/usr/bin/env > /tmp/%s.env",user);
				break;
		}
		system_command(stemp);
	} else {
		prterr("Error:  Can't setuid....");TRACE;
	}
	RCHGCWD(od);	
	if(od!=NULL) Rfree(od);
}
int main(int argc,char **argv)
{
	int which=0,displaynum=0;

	initrdadiag();
	SETUSERLOGIN();
	SETCWD();
	if(argc<4)
	{
		prterr("Error:  Not enough arguments...");
		return(-1);
	}
	which=atoi(argv[1]);
	displaynum=atoi(argv[3]);
	ManageVNCservers(which,argv[2],displaynum);
	exit(0);
}
