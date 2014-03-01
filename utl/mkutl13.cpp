#include<mkmsc.hpp>

#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX && LINUX2_2 */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX || LINUX2_2 || UBUNTU_OS */

void LOADCDROM()
{
	char *name=NULL,*ext=NULL;
	FILE *fp=NULL;
	int fd=(-1);

#ifdef WIN32
	name=stralloc("loadcdrom.bat");
	ext=stralloc(".exe");
#endif 
#ifdef RS6000
	name=stralloc("loadcdrom.rsc");
	ext=stralloc(".rsc");
#endif
#ifdef LINUX
	name=stralloc("loadcdrom.lnx");
	ext=stralloc(".lnx");
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
	name=stralloc("loadcdrom.lnx");
	ext=stralloc(".lnx");
#endif
#ifdef SCO50P
	name=stralloc("loadcdrom.sco");
	ext=stralloc(".sco");
#endif
#ifdef SCO50
	name=stralloc("loadcdrom.sco");
	ext=stralloc(".sco");
#endif
#ifdef UNIXWARE21
	name=stralloc("loadcdrom.unw");
	ext=stralloc(".unw");
#endif
#ifdef UNIXWARE7
	name=stralloc("loadcdrom.unw");
	ext=stralloc(".unw");
#endif
#ifdef SUNX86
	name=stralloc("loadcdrom.sun");
	ext=stralloc(".lnx");
#endif

	fp=fopen(name,"w+");
	if(fp!=NULL)
	{
		fprintf(fp,"%s - Property of RDA Systems, Inc\n",name); 
		fprintf(fp,"\n"); 
		fprintf(fp,"# This script was written to aid in loading cdroms.  This isn't a fix-all\n");
		fprintf(fp,"# and is expecting standards setup the installation process to ensure\n"); 
		fprintf(fp,"# successful execution.  Please obtain verify that you have created a\n"); 
		fprintf(fp,"# mountable cdrom file system on your unix computer, create a known mount\n"); 
		fprintf(fp,"# point.  Set the environment variables into the /etc/profile script\n"); 
		fprintf(fp,"# executed during login.  The environment variables are:\n");
		fprintf(fp,"#\n");
		fprintf(fp,"# RDA_PROGRAMS	: Set this equal to the directory containing the RDA programs\n");
		fprintf(fp,"# RDA_CDROM	: Set this equal to the cdrom mountable filesystem name\n");
		fprintf(fp,"# RDA_GROUP	: Set this equal to the group name to own the programs\n");
		fprintf(fp,"#\n"); 
		fprintf(fp,"# NO RUNNING RDA PROGRAM XPERT OR MBGUI WILL GET LOADED!!!!\n"); 
		fprintf(fp,"#\n"); 
		fprintf(fp,"# The remainder of these comments are to describe the intended functionality\n");
		fprintf(fp,"\n"); 
		fprintf(fp,"\n"); 
		fprintf(fp,"# Obtain User's Login\n");
		fprintf(fp,"logname=`who am i`\n");
		fprintf(fp,"\n"); 
		fprintf(fp,"# Obtain root authority\n");
		fprintf(fp,"su - -c\n");
		fprintf(fp,"\n"); 
		fprintf(fp,"# Get Current Working Directory\n");
		fprintf(fp,"curwdir=`pwd`\n");
		fprintf(fp,"\n"); 
		fprintf(fp,"# if directory in $RDA_PROGRAMS environment variable ?\n");
		fprintf(fp,"if [ $RDA_PROGRAMS == \"\" ]\n");
		fprintf(fp,"then\n");
		fprintf(fp," cd /xpgms\n");
		fprintf(fp,"else\n");
		fprintf(fp,"# if no environment variable.... \n");
		fprintf(fp," cd $RDA_PROGRAMS\n");
		fprintf(fp,"fi\n");
		fprintf(fp,"\n"); 
		fprintf(fp,"# if cdrom filesystem name in $RDA_CDROM environment variable ?\n");
		fprintf(fp,"if [ $RDA_CDROM == \"\" ]\n");
		fprintf(fp,"then \n");
		fprintf(fp,"# use this if no cd-rom filesystem\n");
		fprintf(fp," rdacdrom=\"/rdacdrom\"\n");
		fprintf(fp,"else \n");
		fprintf(fp," rdacdrom=$RDA_CDROM\n");
		fprintf(fp,"fi\n");
		fprintf(fp,"\n"); 
		fprintf(fp,"# if group name in $RDA_GROUP environment variable ?\n");
		fprintf(fp,"if [ $RDA_GROUP == \"\" ]\n");
		fprintf(fp,"then \n");
		fprintf(fp,"# use this if no group name\n");
		fprintf(fp," groupname=\"staff\"\n");
		fprintf(fp,"else \n");
		fprintf(fp," groupname=$RDA_GROUP\n");
		fprintf(fp,"fi\n");
		fprintf(fp,"\n"); 
		fprintf(fp,"# mount pre-installed file system\n");
		fprintf(fp,"mount $rdacdrom\n");
		fprintf(fp,"\n"); 
		fprintf(fp,"# copy programs from mounted diretory to programs directory\n");
		fprintf(fp,"cp $rdacdrom/* .\n");
		fprintf(fp,"\n"); 
		fprintf(fp,"# unmount cd-rom filesystem so that cdrom can be removed\n");
		fprintf(fp,"umount $rdacdrom\n");
		fprintf(fp,"\n"); 
		fprintf(fp,"# Return to Current Working Directory\n");
		fprintf(fp,"cd $curwdir\n");
		fprintf(fp,"\n"); 
		fprintf(fp,"# Fix Permissions, Ownership, and Group\n"); 
		fprintf(fp,"chmod ugo+rwx *%s\n",ext); 
		fprintf(fp,"if [ $logname == \"\" ]\n");
		fprintf(fp,"then\n");
		fprintf(fp," chown $logname *%s\n",ext); 
		fprintf(fp,"fi\n");
		fprintf(fp,"chgrp $groupname *%s\n",ext); 
		fclose(fp);
#ifndef WIN32
		fd=chmod(name,(mode_t)MODE_RWXRWX___);
		if(fd==(-1))
		{
			prterr("Error: Unable to set script as executable!");
		}
#endif /* Not WIN32 */
	}
	if(name!=NULL) Rfree(name);
}
