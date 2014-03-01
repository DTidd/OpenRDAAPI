#include <cstdio>
#include <mix.hpp>
#include <mkmsc.hpp>

extern char *module;
extern APPlib *errorlist;

#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */

static void SCRIPT_MoveMSMTPRC_lnx()
{
	FILE *fp=NULL;
	char *name=NULL;
        char *temp=NULL;
        int fd=(-1);

	temp=Rmalloc(RDAstrlen(DOCUMENTROOT)+RDAstrlen("MoveMSMTPRC.lnx")+15);
#ifdef WIN32
	sprintf(temp,"%s\\MoveMSMTPRC.lnx",DOCUMENTROOT);
#endif
#ifndef WIN32
	sprintf(temp,"%s/MoveMSMTPRC.lnx",DOCUMENTROOT);
#endif
	unlink(temp);
	fp=fopen(temp,"w+b");
	if(temp!=NULL) Rfree(temp);
		fprintf(fp,"#!/bin/sh\n");
		fprintf(fp,"MoveMSMTPRC()\n");
		fprintf(fp,"{\n");
		fprintf(fp,"	for xxx in `ls %s/*/msmtprc`\n",CURRENTDIRECTORY);
		fprintf(fp,"	do\n");
		fprintf(fp,"		XPERT_USER=`awk '/logfile/ {print $2}' \"$xxx\" | awk -F '/' '{print $2}'`\n");
		fprintf(fp,"		USERHOME=`awk -F':' -v UID=\"$XPERT_USER\"  '$1 == UID { print $6 }' /etc/passwd`\n");
		fprintf(fp,"	\n");
		fprintf(fp,"		if [ -d \"$USERHOME\" ]\n");
		fprintf(fp,"		then\n");
		fprintf(fp,"			mv \"$xxx\" \"$USERHOME\"/.msmtprc\n");
		fprintf(fp,"			chown \"$XPERT_USER\" \"$USERHOME\"/.msmtprc\n");
		fprintf(fp,"			chgrp \"$GROUP\" \"$USERHOME\"/.msmtprc\n");
		fprintf(fp,"			chmod 600 \"$USERHOME\"/.msmtprc\n");
		fprintf(fp,"		else\n");
		fprintf(fp,"			echo \"Users Home Directory Not Known.\"\n");
		fprintf(fp,"		fi\n");
		fprintf(fp,"	\n");
		fprintf(fp,"	done\n");
		fprintf(fp,"}\n");
		fprintf(fp,"\n");
		fprintf(fp,"if [ `id -u` == 0 ]\n");
		fprintf(fp,"then\n");
		fprintf(fp,"	MoveMSMTPRC\n");
		fprintf(fp,"elif [ \"$1\" = needroot ]\n");
		fprintf(fp,"then\n");
		fprintf(fp,"	GROUP=`id -gn`\n");
		fprintf(fp,"	export GROUP\n");
		fprintf(fp,"	echo \"You Must Enter Root's Password To Update Email Information.\"\n");
		fprintf(fp,"	su -c \"%s/MoveMSMTPRC.lnx\"\n",DOCUMENTROOT);
		fprintf(fp,"	sleep 3\n");
		fprintf(fp,"elif [ \"$1\" = root ]\n");
		fprintf(fp,"then\n");
		fprintf(fp,"	/usr/bin/X11/xterm -ls -e MoveMSMTPRC.lnx needroot\n");
		fprintf(fp,"elif [ \"$1\" = user ]\n");
		fprintf(fp,"then\n");
		fprintf(fp,"	XPERT_USER=\"$USER_IDENTIFICATION\"\n");
		fprintf(fp,"	USERHOME=`awk -F':' -v UID=\"$XPERT_USER\"  '$1 == UID { print $6 }' /etc/passwd`\n");
		fprintf(fp,"\n");
		fprintf(fp,"	if [ -d \"$USERHOME\" ]\n");
		fprintf(fp,"	then\n");
		fprintf(fp,"		mv \"$XPERT_USER\"/msmtprc \"$USERHOME\"/.msmtprc\n");
		fprintf(fp,"		chmod 600 \"$USERHOME\"/.msmtprc\n");
		fprintf(fp,"	else\n");
		fprintf(fp,"		echo \"Users Home Directory Not Known.\"\n");
		fprintf(fp,"	fi\n");
		fprintf(fp,"	\n");
		fprintf(fp,"fi\n");
	name=Rmalloc(512);
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
        sprintf(name,"%s/MoveMSMTPRC.lnx",DOCUMENTROOT);
#endif

#ifndef WIN32
                fd=chmod(name,(mode_t)MODE_RWXRWX___);
                if(fd==(-1))
                {
                        prterr("Error:  Couldn't set Mode for [%s].",(name!=NULL ? name:""));
                }
#endif

}

void MAKE_SCRIPTMoveMSMTPRC()
{
	SCRIPT_MoveMSMTPRC_lnx();
}
