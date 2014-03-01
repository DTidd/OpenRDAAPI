#include <cstdio>
#include <mix.hpp>
#include <mkmsc.hpp>

extern char *module;
#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */

#if defined(LINUX2_2) || defined(UBUNTU_OS)
void SCRIPT_FTP_RDASYS_COM_kdelnk()
{
	FILE *fp=NULL;
	char *temp=NULL;

	if(!RDAstrcmp(USERLOGIN,"root")) return;
	temp=Rmalloc(RDAstrlen(USERLOGIN)+RDAstrlen("FTP_RDASYS_COM.kdelnk")+98);
	sprintf(temp,"/home/%s/KDesktop",USERLOGIN);
	if(access(temp,00))  /* check for existance of file */
	{
		if(temp!=NULL) Rfree(temp);
		temp=Rmalloc(RDAstrlen(USERLOGIN)+RDAstrlen("FTP_RDASYS_COM.kdelnk")+98);
		sprintf(temp,"/home/%s/Desktop/FTP_RDASYS_COM.kdelnk",USERLOGIN);
	} else {
		if(temp!=NULL) Rfree(temp);
		temp=Rmalloc(RDAstrlen(USERLOGIN)+RDAstrlen("FTP_RDASYS_COM.kdelnk")+98);
		sprintf(temp,"/home/%s/KDesktop/FTP_RDASYS_COM.kdelnk",USERLOGIN);
	}
	unlink(temp);
	fp=fopen(temp,"w+b");

	fprintf(fp,"# KDE Config File\n");
	fprintf(fp,"[KDE Desktop Entry]\n");
	fprintf(fp,"Name=FTP URL\n");
	fprintf(fp,"MiniIcon=ftp.xpm\n");
	fprintf(fp,"Name[sv]=FTP-URL\n");
	fprintf(fp,"URL=ftp://66.135.38.166/pub/\n");
	fprintf(fp,"Icon=ftp.xpm\n");
	fprintf(fp,"Type=Link\n");
	fclose(fp);

	chmod(temp,(mode_t)MODE_RWXRWX___);

	if(temp!=NULL) Rfree(temp);
}
void SCRIPT_WWW_RDASYS_COM_kdelnk()
{
	FILE *fp=NULL;
	char *temp=NULL;

	if(!RDAstrcmp(USERLOGIN,"root")) return;
	temp=Rmalloc(RDAstrlen(USERLOGIN)+RDAstrlen("WWW_RDASYS_COM.kdelnk")+98);
	sprintf(temp,"/home/%s/KDesktop",USERLOGIN);
	if(access(temp,00))  /* check for existance of file */
	{
		if(temp!=NULL) Rfree(temp);
		temp=Rmalloc(RDAstrlen(USERLOGIN)+RDAstrlen("WWW_RDASYS_COM.kdelnk")+98);
		sprintf(temp,"/home/%s/Desktop/WWW_RDASYS_COM.kdelnk",USERLOGIN);
	} else {
		if(temp!=NULL) Rfree(temp);
		temp=Rmalloc(RDAstrlen(USERLOGIN)+RDAstrlen("WWW_RDASYS_COM.kdelnk")+98);
		sprintf(temp,"/home/%s/KDesktop/WWW_RDASYS_COM.kdelnk",USERLOGIN);
	}
	unlink(temp);
	fp=fopen(temp,"w+b");

	fprintf(fp,"# KDE Config File\n");
	fprintf(fp,"[KDE Desktop Entry]\n");
	fprintf(fp,"Name=World Wide Web URL\n");
	fprintf(fp,"MiniIcon=www.xpm\n");
	fprintf(fp,"URL=http://66.135.38.166\n");
	fprintf(fp,"Icon=www.xpm\n");
	fprintf(fp,"Type=Link\n");
	fclose(fp);

	chmod(temp,(mode_t)MODE_RWXRWX___);

	if(temp!=NULL) Rfree(temp);
}
#endif /* LINUX2_2 || UBUNTU_OS */
#define MOTIF_SILVER
void SCRIPT_RDA_XDefaults()
{
	FILE *fp=NULL;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("RDA_XDefaults")+15);
#ifdef WIN32
	sprintf(temp,"%s\\scripts\\RDA_XDefaults",CURRENTDIRECTORY);
#endif
#ifndef WIN32
	sprintf(temp,"%s/scripts/RDA_XDefaults",CURRENTDIRECTORY);
#endif
	unlink(temp);
	fp=fopen(temp,"w+b");
	if(temp!=NULL) Rfree(temp);
#ifdef OLD_DEFAULT
	fprintf(fp,"!\n");
	fprintf(fp,"!\n");
	fprintf(fp,"!         ***BACKGROUND COLORS***\n");
	fprintf(fp,"XPERT*Background:   Cyan4\n");
	fprintf(fp,"XPERT*SCROLLEDWINDOW*Background:   Cyan4\n");
	fprintf(fp,"XPERT*SCROLLEDWINDOW*Foreground: black\n");
	fprintf(fp,"XPERT*SCROLLEDWINDOW*XmText*background: linen\n");
	fprintf(fp,"XPERT*SCROLLEDWINDOW*XmLabel*foreground: black\n");
	fprintf(fp,"XPERT*SCROLLEDWINDOW*armColor:     firebrick\n");
	fprintf(fp,"XPERT*SCROLLEDWINDOW*HighlightColor:    firebrick\n");
	fprintf(fp,"XPERT*XmLabel*foreground: white\n");
	fprintf(fp,"XPERT*HighlightColor:    firebrick\n");
	fprintf(fp,"XPERT*XmScrolledWindow*background: linen\n");
	fprintf(fp,"XPERT*XmText*background: linen\n");
	fprintf(fp,"XPERT*armColor:     firebrick\n");
	fprintf(fp,"!XPERT*selectColor:  red\n");
	fprintf(fp,"XPERT*troughColor:  cadet blue\n");
	fprintf(fp,"aixterm*ActiveShadowrColor:   black\n");
	fprintf(fp,"XPERT*XmScrollBar*foreground: honeydew\n");
	fprintf(fp,"XPERT*XmToggleButton*selectColor: red\n");
	fprintf(fp,"!         ***BUTTONS***\n");
	fprintf(fp,"XPERT*Foreground:     Black\n");
	fprintf(fp,"XPERT*ADD*Background:	lightblue\n");
	fprintf(fp,"XPERT*DELETE*Background:	red\n");
	fprintf(fp,"XPERT*DELETE_FIELD*Background:	red\n");
	fprintf(fp,"XPERT*DELETE_FILE*Background:	red\n");
	fprintf(fp,"XPERT*DELETE_KEY*Background:	red\n");
	fprintf(fp,"!XPERT*ERROR_DIALOG_SCREEN*Background:	red\n");
	fprintf(fp,"XPERT*EXIT*Background:	yellow\n");
	fprintf(fp,"XPERT*HELP*Background:	goldenrod\n");
	fprintf(fp,"XPERT*CYBERSUPPORT*Background:	goldenrod\n");
	fprintf(fp,"XPERT*QUIT*Background:	yellow\n");
	fprintf(fp,"XPERT*RESOURCE_HELP_SCREEN*Background:	goldenrod\n");
	fprintf(fp,"XPERT*RESOURCE_HELP_SCREEN*SCROLLEDWINDOW*Background:	white\n");
	fprintf(fp,"XPERT*MANAGE_SCANS*SAVE*Background:	red\n");
	fprintf(fp,"XPERT*SAVE*Background:	green\n");
	fprintf(fp,"XPERT*SCREEN_HELP*Background:	goldenrod\n");
	fprintf(fp,"XPERT*SCREEN_HELP*SCROLLEDWINDOW*background:	white\n");
	fprintf(fp,"XPERT*SELECT*Background:	green\n");
	fprintf(fp,"XPERT*XmPushButton*background:     BlanchedAlmond\n");
	fprintf(fp,"!\n");
	fprintf(fp,"!         ***DIALOG MESSAGES***\n");
	fprintf(fp,"!\n");
	fprintf(fp,"XPERT*ERROR_DIALOG_SCREEN*Background: red\n");
	fprintf(fp,"XPERT*ERROR_DIALOG_SCREEN*XmScrolledWindow*background: white\n");
	fprintf(fp,"XPERT*ERROR_DIALOG_SCREEN*XmText*background: white\n");
	fprintf(fp,"XPERT*SAVE_WARNING_DIALOG_SCREEN*Background: dodger blue\n");
	fprintf(fp,"XPERT*SAVE_WARNING_DIALOG_SCREEN*XmScrolledWindow*background: white\n");
	fprintf(fp,"XPERT*SAVE_WARNING_DIALOG_SCREEN*XmText*background: white\n");
	fprintf(fp,"XPERT*WARNING_DIALOG_SCREEN*Background: dark salmon\n");
	fprintf(fp,"XPERT*WARNING_DIALOG_SCREEN*XmScrolledWindow*background: white\n");
	fprintf(fp,"XPERT*WARNING_DIALOG_SCREEN*XmText*background: white\n");
	fprintf(fp,"!\n");
	fprintf(fp,"!         ***HELP SCREENS***\n");
	fprintf(fp,"!\n");
	fprintf(fp,"XPERT*RESOURCE_HELP_SCREEN*XmScrolledWindow*background: white\n");
	fprintf(fp,"XPERT*RESOURCE_HELP_SCREEN*background: gold\n");
	fprintf(fp,"XPERT*RESOURCE_HELP_SCREEN*XmLabel*Foreground:  black\n");
	fprintf(fp,"XPERT*RESOURCE_HELP_SCREEN*XmPushButton*background: LightGoldenrod1\n");
	fprintf(fp,"XPERT*SCREEN_HELP*XmScrolledWindow*background: white\n");
	fprintf(fp,"XPERT*SCREEN_HELP*background: gold\n");
	fprintf(fp,"XPERT*SCREEN_HELP*XmPushButton*background: LightGoldenrod1\n");
	fprintf(fp,"XPERT*SCREEN_HELP*XmLabel*Foreground: black\n");
	fprintf(fp,"!\n");
	fprintf(fp,"!\n");
	fprintf(fp,"!\n");
	fprintf(fp,"!\n");
	fprintf(fp,"!         ***FONTS***\n");
	fprintf(fp,"!\n");
	fprintf(fp,"XPERT*font:    fixed\n");
	fprintf(fp,"XPERT*fontList:     fixed\n");
	fprintf(fp,"XPERT*fontsel:  fixed\n");
	fprintf(fp,"!XPERT*TEXT_BROWSER*font: 10x18\n");
	fprintf(fp,"!XPERT*TEXT_BROWSER*fontList:  10x18\n");
	fprintf(fp,"!XPERT*TEXT_BROWSER*fontsel:   10x18\n");
	fprintf(fp,"!XPERT*TEXT_BROWSER*font: ROM14.500\n");
	fprintf(fp,"!XPERT*TEXT_BROWSER*fontList: ROM14.500\n");
	fprintf(fp,"!XPERT*TEXT_BROWSER*fontsel:  ROM14.500 \n");
	fprintf(fp,"!XPERT*fontList:     ROM14.500\n");
	fprintf(fp,"!XPERT*fontSet:	ROM14.500\n");
	fprintf(fp,"!	xtb*fontList:  fixed\n");
	fprintf(fp,"!	xtb*fontSet:   fixed\n");
	fprintf(fp,"!xtb*fontList:	ROM14.500\n");
	fprintf(fp,"!xtb*fontSet:	ROM14.500\n");
	fprintf(fp,"xtb*background:	white\n");
#endif
#ifdef MOTIF_SILVER
	fprintf(fp,"!\n");
	fprintf(fp,"!\n");
	fprintf(fp,"!         ***BACKGROUND COLORS***\n");
	fprintf(fp,"XPERT*Background:    light grey\n");
	fprintf(fp,"XPERT*SCROLLEDWINDOW*Background:   light grey\n");
	fprintf(fp,"XPERT*SCROLLEDWINDOW*Foreground: black\n");
	fprintf(fp,"XPERT*SCROLLEDWINDOW*XmText*background: linen\n");
	fprintf(fp,"XPERT*SCROLLEDWINDOW*XmText*foreground: black\n");
	fprintf(fp,"XPERT*SCROLLEDWINDOW*XmLabel*foreground: black\n");
	fprintf(fp,"XPERT*SCROLLEDWINDOW*armColor:     light blue\n");
	fprintf(fp,"XPERT*SCROLLEDWINDOW*HighlightColor:    blue\n");
	fprintf(fp,"XPERT*SCROOLEDWINDOW*XmScrolledWindow*background: linen\n");
	fprintf(fp,"XPERT*SCROLLEDWINDOW*XmPushButton*background:     BlanchedAlmond\n");
	fprintf(fp,"XPERT*SCROLLEDWINDOW*troughColor:  linen\n");
	fprintf(fp,"!XPERT*XmLabel*foreground: black\n");
	fprintf(fp,"XPERT*HighlightColor:    blue\n");
	fprintf(fp,"XPERT*XmScrolledWindow*background: linen\n");
	fprintf(fp,"XPERT*XmScrolledWindow*foreground: black\n");
	fprintf(fp,"XPERT*XmScrolledWindow*HighlightColor: blue\n");
	fprintf(fp,"XPERT*XmText*background: linen\n");
	fprintf(fp,"XPERT*XmText*foreground: black\n");
	fprintf(fp,"XPERT*XmLabel*foreground: black\n");
	fprintf(fp,"XPERT*armColor:     White\n");
	fprintf(fp,"!XPERT*selectColor:  black\n");
	fprintf(fp,"XPERT*troughColor:  Grey\n");
	fprintf(fp,"aixterm*ActiveShadowrColor:   black\n");
	fprintf(fp,"XPERT*XmScrollBar*foreground: linen\n");
	fprintf(fp,"XPERT*XmScrollBar*background: linen\n");
	fprintf(fp,"XPERT*XmToggleButton*selectColor: red\n");
	fprintf(fp,"XPERT*XmToggleButton*background: light grey\n");
	fprintf(fp,"!         ***BUTTONS***\n");
	fprintf(fp,"XPERT*Foreground:     Black\n");
	fprintf(fp,"XPERT*ADD*Background:	lightblue\n");
	fprintf(fp,"XPERT*DELETE*Background:	red\n");
	fprintf(fp,"XPERT*DELETE_FIELD*Background:	red\n");
	fprintf(fp,"XPERT*DELETE_FILE*Background:	red\n");
	fprintf(fp,"XPERT*DELETE_KEY*Background:	red\n");
	fprintf(fp,"!XPERT*ERROR_DIALOG_SCREEN*Background:	red\n");
	fprintf(fp,"XPERT*EXIT*Background:	yellow\n");
	fprintf(fp,"XPERT*HELP*Background:	goldenrod\n");
	fprintf(fp,"XPERT*QUIT*Background:	yellow\n");
	fprintf(fp,"XPERT*RESOURCE_HELP_SCREEN*Background:	goldenrod\n");
	fprintf(fp,"XPERT*RESOURCE_HELP_SCREEN*SCROLLEDWINDOW*Background:	white\n");
	fprintf(fp,"XPERT*MANAGE_SCANS*SAVE*Background:	red\n");
	fprintf(fp,"XPERT*SAVE*Background:	green\n");
	fprintf(fp,"XPERT*SCREEN_HELP*Background:	goldenrod\n");
	fprintf(fp,"XPERT*SCREEN_HELP*SCROLLEDWINDOW*background:	white\n");
	fprintf(fp,"XPERT*SELECT*Background:	green\n");
	fprintf(fp,"XPERT*RUN_REPORT*Background:	green\n");
	fprintf(fp,"XPERT*XmPushButton*background:     BlanchedAlmond\n");
	fprintf(fp,"!\n");
	fprintf(fp,"!         ***DIALOG MESSAGES***\n");
	fprintf(fp,"!\n");
	fprintf(fp,"XPERT*ERROR_DIALOG_SCREEN*Background: red\n");
	fprintf(fp,"XPERT*ERROR_DIALOG_SCREEN*XmScrolledWindow*background: white\n");
	fprintf(fp,"XPERT*ERROR_DIALOG_SCREEN*XmText*background: white\n");
	fprintf(fp,"XPERT*SAVE_WARNING_DIALOG_SCREEN*Background: dodger blue\n");
	fprintf(fp,"XPERT*SAVE_WARNING_DIALOG_SCREEN*XmScrolledWindow*background: white\n");
	fprintf(fp,"XPERT*SAVE_WARNING_DIALOG_SCREEN*XmText*background: white\n");
	fprintf(fp,"XPERT*WARNING_DIALOG_SCREEN*Background: dark salmon\n");
	fprintf(fp,"XPERT*WARNING_DIALOG_SCREEN*XmScrolledWindow*background: white\n");
	fprintf(fp,"XPERT*WARNING_DIALOG_SCREEN*XmText*background: white\n");
	fprintf(fp,"!\n");
	fprintf(fp,"!         ***HELP SCREENS***\n");
	fprintf(fp,"!\n");
	fprintf(fp,"XPERT*RESOURCE_HELP_SCREEN*XmScrolledWindow*background: white\n");
	fprintf(fp,"XPERT*RESOURCE_HELP_SCREEN*background: gold\n");
	fprintf(fp,"XPERT*RESOURCE_HELP_SCREEN*XmLabel*Foreground:  black\n");
	fprintf(fp,"XPERT*RESOURCE_HELP_SCREEN*XmPushButton*background: LightGoldenrod1\n");
	fprintf(fp,"XPERT*SCREEN_HELP*XmScrolledWindow*background: white\n");
	fprintf(fp,"XPERT*SCREEN_HELP*background: gold\n");
	fprintf(fp,"XPERT*SCREEN_HELP*XmPushButton*background: LightGoldenrod1\n");
	fprintf(fp,"XPERT*SCREEN_HELP*XmLabel*Foreground: black\n");
	fprintf(fp,"!\n");
	fprintf(fp,"!\n");
	fprintf(fp,"!\n");
	fprintf(fp,"!\n");
	fprintf(fp,"!         ***FONTS***\n");
	fprintf(fp,"!\n");
	fprintf(fp,"XPERT*font:    fixed\n");
	fprintf(fp,"XPERT*fontList:     fixed\n");
	fprintf(fp,"XPERT*fontsel:  fixed\n");
	fprintf(fp,"!XPERT*TEXT_BROWSER*font: 10x18\n");
	fprintf(fp,"!XPERT*TEXT_BROWSER*fontList:  10x18\n");
	fprintf(fp,"!XPERT*TEXT_BROWSER*fontsel:   10x18\n");
	fprintf(fp,"!XPERT*TEXT_BROWSER*font: ROM14.500\n");
	fprintf(fp,"!XPERT*TEXT_BROWSER*fontList: ROM14.500\n");
	fprintf(fp,"!XPERT*TEXT_BROWSER*fontsel:  ROM14.500 \n");
	fprintf(fp,"!XPERT*fontList:     ROM14.500\n");
	fprintf(fp,"!XPERT*fontSet:	ROM14.500\n");
	fprintf(fp,"!	xtb*fontList:  fixed\n");
	fprintf(fp,"!	xtb*fontSet:   fixed\n");
	fprintf(fp,"!xtb*fontList:	ROM14.500\n");
	fprintf(fp,"!xtb*fontSet:	ROM14.500\n");
	fprintf(fp,"xtb*background:	white\n");
#endif
	fclose(fp);
}

void SCRIPT_megs2()
{
	FILE *fp=NULL;
	char *temp=NULL;

	if(XPERT_SETUP->ASP) return;
	temp=Rmalloc(300);
#ifdef WIN32
	sprintf(temp,"rda\\rdabin\\megs2");
	unlink(temp);
	fp=fopen(temp,"w+b");
#endif
#ifndef WIN32
	sprintf(temp,"/rda/rdabin/megs2");
	unlink(temp);
	fp=fopen(temp,"w+");
#endif
	if(fp!=NULL)
	{
	if(temp!=NULL) Rfree(temp);
	fprintf(fp,"!\n");
	fprintf(fp,"!\n");
	fprintf(fp,"!         ***BACKGROUND COLORS***\n");
	fprintf(fp,"XPERT*Background:   Cyan4\n");
	fprintf(fp,"XPERT*SCROLLEDWINDOW*Background:   Cyan4\n");
	fprintf(fp,"XPERT*SCROLLEDWINDOW*Foreground: black\n");
	fprintf(fp,"XPERT*SCROLLEDWINDOW*XmText*background: linen\n");
	fprintf(fp,"XPERT*SCROLLEDWINDOW*XmLabel*foreground: black\n");
	fprintf(fp,"XPERT*SCROLLEDWINDOW*armColor:     firebrick\n");
	fprintf(fp,"XPERT*SCROLLEDWINDOW*HighlightColor:    firebrick\n");
	fprintf(fp,"XPERT*XmLabel*foreground: white\n");
	fprintf(fp,"XPERT*HighlightColor:    firebrick\n");
	fprintf(fp,"XPERT*XmScrolledWindow*background: linen\n");
	fprintf(fp,"XPERT*XmText*background: linen\n");
	fprintf(fp,"XPERT*armColor:     firebrick\n");
	fprintf(fp,"!XPERT*selectColor:  red\n");
	fprintf(fp,"XPERT*troughColor:  cadet blue\n");
	fprintf(fp,"aixterm*ActiveShadowrColor:   black\n");
	fprintf(fp,"XPERT*XmScrollBar*foreground: honeydew\n");
	fprintf(fp,"XPERT*XmToggleButton*selectColor: red\n");
	fprintf(fp,"!         ***BUTTONS***\n");
	fprintf(fp,"XPERT*Foreground:     Black\n");
	fprintf(fp,"XPERT*ADD*Background:	lightblue\n");
	fprintf(fp,"XPERT*DELETE*Background:	red\n");
	fprintf(fp,"XPERT*DELETE_FIELD*Background:	red\n");
	fprintf(fp,"XPERT*DELETE_FILE*Background:	red\n");
	fprintf(fp,"XPERT*DELETE_KEY*Background:	red\n");
	fprintf(fp,"!XPERT*ERROR_DIALOG_SCREEN*Background:	red\n");
	fprintf(fp,"XPERT*EXIT*Background:	yellow\n");
	fprintf(fp,"XPERT*HELP*Background:	goldenrod\n");
	fprintf(fp,"XPERT*CYBERSUPPORT*Background:	goldenrod\n");
	fprintf(fp,"XPERT*QUIT*Background:	yellow\n");
	fprintf(fp,"XPERT*RESOURCE_HELP_SCREEN*Background:	goldenrod\n");
	fprintf(fp,"XPERT*RESOURCE_HELP_SCREEN*SCROLLEDWINDOW*Background:	white\n");
	fprintf(fp,"XPERT*MANAGE_SCANS*SAVE*Background:	red\n");
	fprintf(fp,"XPERT*SAVE*Background:	green\n");
	fprintf(fp,"XPERT*SCREEN_HELP*Background:	goldenrod\n");
	fprintf(fp,"XPERT*SCREEN_HELP*SCROLLEDWINDOW*background:	white\n");
	fprintf(fp,"XPERT*SELECT*Background:	green\n");
	fprintf(fp,"XPERT*XmPushButton*background:     BlanchedAlmond\n");
	fprintf(fp,"!\n");
	fprintf(fp,"!         ***DIALOG MESSAGES***\n");
	fprintf(fp,"!\n");
	fprintf(fp,"XPERT*ERROR_DIALOG_SCREEN*Background: red\n");
	fprintf(fp,"XPERT*ERROR_DIALOG_SCREEN*XmScrolledWindow*background: white\n");
	fprintf(fp,"XPERT*ERROR_DIALOG_SCREEN*XmText*background: white\n");
	fprintf(fp,"XPERT*SAVE_WARNING_DIALOG_SCREEN*Background: dodger blue\n");
	fprintf(fp,"XPERT*SAVE_WARNING_DIALOG_SCREEN*XmScrolledWindow*background: white\n");
	fprintf(fp,"XPERT*SAVE_WARNING_DIALOG_SCREEN*XmText*background: white\n");
	fprintf(fp,"XPERT*WARNING_DIALOG_SCREEN*Background: dark salmon\n");
	fprintf(fp,"XPERT*WARNING_DIALOG_SCREEN*XmScrolledWindow*background: white\n");
	fprintf(fp,"XPERT*WARNING_DIALOG_SCREEN*XmText*background: white\n");
	fprintf(fp,"!\n");
	fprintf(fp,"!         ***HELP SCREENS***\n");
	fprintf(fp,"!\n");
	fprintf(fp,"XPERT*RESOURCE_HELP_SCREEN*XmScrolledWindow*background: white\n");
	fprintf(fp,"XPERT*RESOURCE_HELP_SCREEN*background: gold\n");
	fprintf(fp,"XPERT*RESOURCE_HELP_SCREEN*XmLabel*Foreground:  black\n");
	fprintf(fp,"XPERT*RESOURCE_HELP_SCREEN*XmPushButton*background: LightGoldenrod1\n");
	fprintf(fp,"XPERT*SCREEN_HELP*XmScrolledWindow*background: white\n");
	fprintf(fp,"XPERT*SCREEN_HELP*background: gold\n");
	fprintf(fp,"XPERT*SCREEN_HELP*XmPushButton*background: LightGoldenrod1\n");
	fprintf(fp,"XPERT*SCREEN_HELP*XmLabel*Foreground: black\n");
	fprintf(fp,"!\n");
	fprintf(fp,"!\n");
	fprintf(fp,"!\n");
	fprintf(fp,"!\n");
	fprintf(fp,"!         ***FONTS***\n");
	fprintf(fp,"!\n");
	fprintf(fp,"XPERT*font:    fixed\n");
	fprintf(fp,"XPERT*fontList:     fixed\n");
	fprintf(fp,"XPERT*fontsel:  fixed\n");
	fprintf(fp,"!XPERT*TEXT_BROWSER*font: 10x18\n");
	fprintf(fp,"!XPERT*TEXT_BROWSER*fontList:  10x18\n");
	fprintf(fp,"!XPERT*TEXT_BROWSER*fontsel:   10x18\n");
	fprintf(fp,"!XPERT*TEXT_BROWSER*font: ROM14.500\n");
	fprintf(fp,"!XPERT*TEXT_BROWSER*fontList: ROM14.500\n");
	fprintf(fp,"!XPERT*TEXT_BROWSER*fontsel:  ROM14.500 \n");
	fprintf(fp,"!XPERT*fontList:     ROM14.500\n");
	fprintf(fp,"!XPERT*fontSet:	ROM14.500\n");
	fprintf(fp,"!	xtb*fontList:  fixed\n");
	fprintf(fp,"!	xtb*fontSet:   fixed\n");
	fprintf(fp,"!xtb*fontList:	ROM14.500\n");
	fprintf(fp,"!xtb*fontSet:	ROM14.500\n");
	fprintf(fp,"xtb*background:	white\n");
	fclose(fp);
	}
}

#ifndef WIN32
void SCRIPT_call_mozilla_lnx()
{
	FILE *fp=NULL;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("call_mozilla.lnx")+15);
	sprintf(temp,"%s/call_mozilla.lnx",CURRENTDIRECTORY);
	unlink(temp);
	fp=fopen(temp,"w+b");
	fprintf(fp,"/usr/bin/mozilla $*\n");
	fclose(fp);
	chmod(temp,(mode_t)MODE_RWXRWX___);
	if(temp!=NULL) Rfree(temp);
}
void SCRIPT_call_opera_lnx()
{
	FILE *fp=NULL;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("call_opera.lnx")+15);
	sprintf(temp,"%s/call_opera.lnx",CURRENTDIRECTORY);
	unlink(temp);
	fp=fopen(temp,"w+b");
	fprintf(fp,"/usr/bin/opera $*\n");
	fclose(fp);
	chmod(temp,(mode_t)MODE_RWXRWX___);
	if(temp!=NULL) Rfree(temp);
}
void SCRIPT_call_acroread_lnx()
{
	FILE *fp=NULL;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("call_acroread.lnx")+15);
	sprintf(temp,"%s/call_acroread.lnx",CURRENTDIRECTORY);
	unlink(temp);
	fp=fopen(temp,"w+b");

	fprintf(fp,"acroread $1\n");
	fclose(fp);
	chmod(temp,(mode_t)MODE_RWXRWX___);
	if(temp!=NULL) Rfree(temp);
}
void SCRIPT_kghostview_lnx()
{
	FILE *fp=NULL;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("kghostview.lnx")+15);
	sprintf(temp,"%s/kghostview.lnx",CURRENTDIRECTORY);
	unlink(temp);
	fp=fopen(temp,"w+b");

	fprintf(fp,"/usr/bin/kghostview $*\n");
	fclose(fp);
	chmod(temp,(mode_t)MODE_RWXRWX___);
	if(temp!=NULL) Rfree(temp);
}
void SCRIPT_scalc()
{
	FILE *fp=NULL;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("soffice.lnx")+15);
	sprintf(temp,"%s/scalc.lnx",CURRENTDIRECTORY);
	unlink(temp);
	fp=fopen(temp,"w+b");

	fprintf(fp,"#!/bin/sh\n");
	fprintf(fp,"soffice -calc \"$@\"\n");
	fclose(fp);
	chmod(temp,(mode_t)MODE_RWXRWX___);
	if(temp!=NULL) Rfree(temp);
}
void SCRIPT_perdmgimp()
{
	FILE *fp=NULL;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("perdmgimp.lnx")+15);
	sprintf(temp,"%s/perdmgimp.lnx",CURRENTDIRECTORY);
	unlink(temp);
	fp=fopen(temp,"w+b");

	fprintf(fp,"#!/bin/sh\n");
	fprintf(fp,"perdmgimp.lnx 2>/tmp/$LOGNAME.imp.log\n");
	fclose(fp);
	chmod(temp,(mode_t)MODE_RWXRWX___);
	if(temp!=NULL) Rfree(temp);
}
void SCRIPT_soffice()
{
	FILE *fp=NULL;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("soffice.lnx")+15);
	sprintf(temp,"%s/soffice.lnx",CURRENTDIRECTORY);
	unlink(temp);
	fp=fopen(temp,"w+b");

	fprintf(fp,"#!/bin/sh\n");
	fprintf(fp,"soffice \"$@\"\n");
	fclose(fp);
	chmod(temp,(mode_t)MODE_RWXRWX___);
	if(temp!=NULL) Rfree(temp);
}
#endif
#ifdef WIN32
void SCRIPT_call_netscape_bat()
{
	FILE *fp=NULL;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("call_netscape.bat")+15);
	sprintf(temp,"%s\\call_netscape.bat",CURRENTDIRECTORY);
	unlink(temp);
	fp=fopen(temp,"w+b");

	fprintf(fp,"NETSCAPE.EXE \"%1\"\n");
	fclose(fp);
	if(temp!=NULL) Rfree(temp);

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("call_mozilla.bat")+15);
	sprintf(temp,"%s\\call_mozilla.bat",CURRENTDIRECTORY);
	unlink(temp);
	fp=fopen(temp,"w+b");

	fprintf(fp,"NETSCAPE.EXE \"%1\"\n");
	fclose(fp);
	if(temp!=NULL) Rfree(temp);
	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("call_explorer.bat")+15);
	sprintf(temp,"%s\\call_explorer.bat",CURRENTDIRECTORY);
	unlink(temp);
	fp=fopen(temp,"w+b");

	fprintf(fp,"EXPLORER.EXE \"%1\"\n");
	fclose(fp);
}
void SCRIPT_call_acroread_bat()
{
	FILE *fp=NULL;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("call_acroread.bat")+15);
	sprintf(temp,"%s\\call_acroread.bat",CURRENTDIRECTORY);
	unlink(temp);
	fp=fopen(temp,"w+b");

	fprintf(fp,"\"c:\\Program Files\\Adobe\\Acrobat 4.0\\Reader\\ACRORD32.EXE\" \"%1\"\n");
	fclose(fp);
	if(temp!=NULL) Rfree(temp);
}
#endif
#ifndef WIN32
void SCRIPT_chgdirmtnmst_lnx()
{
	FILE *fp=NULL;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen("chgdirmtnmst.lnx")+15);
	sprintf(temp,"chgdirmtnmst.lnx");
	unlink(temp);
	fp=fopen(temp,"w+b");
	fprintf(fp,"cd \"$3\"\n");
	fprintf(fp,"mtnmst.lnx \"$1\" \"$2\"\n");
	fclose(fp);
	chmod(temp,(mode_t)MODE_RWXRWX___);
	if(temp!=NULL) Rfree(temp);
}
#endif
void SCRIPT_wrkgrpmtnmst_lnx()
{
	FILE *fp=NULL;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("wrkgrpmtnmst.lnx")+15);
#ifdef WIN32
	sprintf(temp,"%s\\wrkgrpmtnmst.bat",CURRENTDIRECTORY);
	unlink(temp);
	fp=fopen(temp,"w+b");
	fprintf(fp,"set WORK_GROUP=\"$3\"\n");
	fprintf(fp,"mtnmst.exe \"$1\" \"$2\"\n");
	fclose(fp);
#endif
#ifndef WIN32
	sprintf(temp,"%s/wrkgrpmtnmst.lnx",CURRENTDIRECTORY);
	unlink(temp);
	fp=fopen(temp,"w+b");
	fprintf(fp,"export WORK_GROUP=\"$3\"\n");
	fprintf(fp,"mtnmst.lnx \"$1\" \"$2\"\n");
	fclose(fp);
	chmod(temp,(mode_t)MODE_RWXRWX___);
#endif
	if(temp!=NULL) Rfree(temp);
}
void SCRIPT_setRDAenv()
{
	FILE *fp=NULL;
	char *temp=NULL;

	if(XPERT_SETUP->ASP) return;
	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("setRDAenv")+20);
#ifdef WIN32
	sprintf(temp,"%s\\scripts\\setRDAenv",CURRENTDIRECTORY);
#endif
#ifndef WIN32
	sprintf(temp,"%s/scripts/setRDAenv",CURRENTDIRECTORY);
#endif
	unlink(temp);
	fp=fopen(temp,"w+b");
	fprintf(fp,"#!/bin/sh\n");
	fprintf(fp,". /etc/profile.rda\n");
	fprintf(fp,"USER=`id -un`\n");
	fprintf(fp,"export USER\n");
	fprintf(fp,"PROG=\"$1\"\n");
	fprintf(fp,"shift\n");
	fprintf(fp,"exec $PROG \"$@\"\n");
	fclose(fp);
#ifndef WIN32
	chmod(temp,(mode_t)MODE_RWXRWX___);
#endif
	if(temp!=NULL) Rfree(temp);
}
static void MNU_SETUPRDACOLORS()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("SETUPRDACOLORS","xterm -ls -e setuprdacolors.lnx");
	if(menu!=NULL)
	{
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.MNU",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.MNU",CURRENTDIRECTORY,module);
#endif
		if(writemenubin(defdir,menu))
		{
			ERRORDIALOG("Cannot Save Menu","This menu may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(menu!=NULL) free_menu(menu);
	}
}
void SCRIPT_setuprdacolors_lnx()
{
	FILE *fp=NULL;
	char *temp=NULL;

#ifndef WIN32
	MNU_SETUPRDACOLORS();
	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("setuprdacolors.lnx")+15);
	sprintf(temp,"%s/setuprdacolors.lnx",CURRENTDIRECTORY);
	unlink(temp);
	fp=fopen(temp,"w+b");

	fprintf(fp,"#!/bin/sh\n");
	fprintf(fp,"if [ `id -u` != 0 ]\n");
	fprintf(fp,"then\n");
	fprintf(fp,"	echo \"Switch To ROOT User.....\"\n");
	fprintf(fp,"	su - -c \"$PWD\"/setuprdacolors.lnx\n");
	fprintf(fp,"	sleep 30\n");
	fprintf(fp,"fi\n");
	fprintf(fp,"if [ `id -u` != 0 ]\n");
	fprintf(fp,"then\n");
	fprintf(fp,"	exit 0\n");
	fprintf(fp,"fi\n");
	fprintf(fp,"cd /\n");
	fprintf(fp,"FILENAME=`date +/root/setuprdacolors.%%Y%%m%%d%%H%%M.tgz`\n");
	fprintf(fp,"XRDBFILES=`grep -l 'xrdb -merge' ./rda/rdabin/*`\n");
	fprintf(fp,"tar cvzf \"$FILENAME\" ./etc/X11/app-defaults/Mwm ./rda/rdabin/megs* $XRDBFILES\n");
	fprintf(fp,"uudecode -o /etc/X11/app-defaults/Mwm <<EOF \n");
	fprintf(fp,"begin-base64 644 Mwm\n");
	fprintf(fp,"IQohIEBPUEVOR1JPVVBfQ09QWVJJR0hUQAohIENPUFlSSUdIVCBOT1RJQ0UK\n");
	fprintf(fp,"ISBDb3B5cmlnaHQgKGMpIDE5OTAsIDE5OTEsIDE5OTIsIDE5OTMgT3BlbiBT\n");
	fprintf(fp,"b2Z0d2FyZSBGb3VuZGF0aW9uLCBJbmMuCiEgQ29weXJpZ2h0IChjKSAxOTk2\n");
	fprintf(fp,"LCAxOTk3LCAxOTk4LCAxOTk5LCAyMDAwIFRoZSBPcGVuIEdyb3VwCiEgQUxM\n");
	fprintf(fp,"IFJJR0hUUyBSRVNFUlZFRCAoTU9USUYpLiBTZWUgdGhlIGZpbGUgbmFtZWQg\n");
	fprintf(fp,"Q09QWVJJR0hULk1PVElGIGZvcgohIHRoZSBmdWxsIGNvcHlyaWdodCB0ZXh0\n");
	fprintf(fp,"LgohIAohIFRoaXMgc29mdHdhcmUgaXMgc3ViamVjdCB0byBhbiBvcGVuIGxp\n");
	fprintf(fp,"Y2Vuc2UuIEl0IG1heSBvbmx5IGJlCiEgdXNlZCBvbiwgd2l0aCBvciBmb3Ig\n");
	fprintf(fp,"b3BlcmF0aW5nIHN5c3RlbXMgd2hpY2ggYXJlIHRoZW1zZWx2ZXMgb3Blbgoh\n");
	fprintf(fp,"IHNvdXJjZSBzeXN0ZW1zLiBZb3UgbXVzdCBjb250YWN0IFRoZSBPcGVuIEdy\n");
	fprintf(fp,"b3VwIGZvciBhIGxpY2Vuc2UKISBhbGxvd2luZyBkaXN0cmlidXRpb24gYW5k\n");
	fprintf(fp,"IHN1YmxpY2Vuc2luZyBvZiB0aGlzIHNvZnR3YXJlIG9uLCB3aXRoLAohIG9y\n");
	fprintf(fp,"IGZvciBvcGVyYXRpbmcgc3lzdGVtcyB3aGljaCBhcmUgbm90IE9wZW4gU291\n");
	fprintf(fp,"cmNlIHByb2dyYW1zLgohIAohIFNlZSBodHRwOi8vd3d3Lm9wZW5ncm91cC5v\n");
	fprintf(fp,"cmcvb3Blbm1vdGlmL2xpY2Vuc2UgZm9yIGZ1bGwKISBkZXRhaWxzIG9mIHRo\n");
	fprintf(fp,"ZSBsaWNlbnNlIGFncmVlbWVudC4gQW55IHVzZSwgcmVwcm9kdWN0aW9uLCBv\n");
	fprintf(fp,"cgohIGRpc3RyaWJ1dGlvbiBvZiB0aGUgcHJvZ3JhbSBjb25zdGl0dXRlcyBy\n");
	fprintf(fp,"ZWNpcGllbnQncyBhY2NlcHRhbmNlIG9mCiEgdGhpcyBhZ3JlZW1lbnQuCiEg\n");
	fprintf(fp,"CiEgRVhDRVBUIEFTIEVYUFJFU1NMWSBTRVQgRk9SVEggSU4gVEhJUyBBR1JF\n");
	fprintf(fp,"RU1FTlQsIFRIRSBQUk9HUkFNIElTCiEgUFJPVklERUQgT04gQU4gIkFTIElT\n");
	fprintf(fp,"IiBCQVNJUywgV0lUSE9VVCBXQVJSQU5USUVTIE9SIENPTkRJVElPTlMgT0Yg\n");
	fprintf(fp,"QU5ZCiEgS0lORCwgRUlUSEVSIEVYUFJFU1MgT1IgSU1QTElFRCBJTkNMVURJ\n");
	fprintf(fp,"TkcsIFdJVEhPVVQgTElNSVRBVElPTiwgQU5ZCiEgV0FSUkFOVElFUyBPUiBD\n");
	fprintf(fp,"T05ESVRJT05TIE9GIFRJVExFLCBOT04tSU5GUklOR0VNRU5ULCBNRVJDSEFO\n");
	fprintf(fp,"VEFCSUxJVFkKISBPUiBGSVRORVNTIEZPUiBBIFBBUlRJQ1VMQVIgUFVSUE9T\n");
	fprintf(fp,"RQohIAohIEVYQ0VQVCBBUyBFWFBSRVNTTFkgU0VUIEZPUlRIIElOIFRISVMg\n");
	fprintf(fp,"QUdSRUVNRU5ULCBORUlUSEVSIFJFQ0lQSUVOVAohIE5PUiBBTlkgQ09OVFJJ\n");
	fprintf(fp,"QlVUT1JTIFNIQUxMIEhBVkUgQU5ZIExJQUJJTElUWSBGT1IgQU5ZIERJUkVD\n");
	fprintf(fp,"VCwKISBJTkRJUkVDVCwgSU5DSURFTlRBTCwgU1BFQ0lBTCwgRVhFTVBMQVJZ\n");
	fprintf(fp,"LCBPUiBDT05TRVFVRU5USUFMCiEgREFNQUdFUyAoSU5DTFVESU5HIFdJVEhP\n");
	fprintf(fp,"VVQgTElNSVRBVElPTiBMT1NUIFBST0ZJVFMpLCBIT1dFVkVSIENBVVNFRAoh\n");
	fprintf(fp,"IEFORCBPTiBBTlkgVEhFT1JZIE9GIExJQUJJTElUWSwgV0hFVEhFUiBJTiBD\n");
	fprintf(fp,"T05UUkFDVCwgU1RSSUNUCiEgTElBQklMSVRZLCBPUiBUT1JUIChJTkNMVURJ\n");
	fprintf(fp,"TkcgTkVHTElHRU5DRSBPUiBPVEhFUldJU0UpIEFSSVNJTkcgSU4KISBBTlkg\n");
	fprintf(fp,"V0FZIE9VVCBPRiBUSEUgVVNFIE9SIERJU1RSSUJVVElPTiBPRiBUSEUgUFJP\n");
	fprintf(fp,"R1JBTSBPUiBUSEUKISBFWEVSQ0lTRSBPRiBBTlkgUklHSFRTIEdSQU5URUQg\n");
	fprintf(fp,"SEVSRVVOREVSLCBFVkVOIElGIEFEVklTRUQgT0YgVEhFCiEgUE9TU0lCSUxJ\n");
	fprintf(fp,"VFkgT0YgU1VDSCBEQU1BR0VTLgohCiEKISBISVNUT1JZCiEKISAgICRYQ29u\n");
	fprintf(fp,"c29ydGl1bTogTXdtLmFkIC9tYWluLzkgMTk5NS8wNy8xNCAwOToyMTo1MCBk\n");
	fprintf(fp,"cmsgJAohCiEgIGFwcC1kZWZhdWx0cyBSRVNPVVJDRSBTUEVDSUZJQ0FUSU9O\n");
	fprintf(fp,"UyBGT1IgTXdtCiEJCShtd20gaGFzIHByZWNlZGVuY2Ugb3ZlciBNd20pCiEK\n");
	fprintf(fp,"CiEhCiEhIE5PVEU6IEluIE13bSAxLjEuMSBhbmQgbmV3ZXIsIHRoZSByZXNv\n");
	fprintf(fp,"dXJjZXMgJ2J1dHRvbkJpbmRpbmdzJyBhbmQKISEJICdrZXlCaW5kaW5ncycg\n");
	fprintf(fp,"c2hvdWxkIG5vdCBiZSBkZWZpbmVkIGluIHRoZSBNd20gYXBwLWRlZmF1bHRz\n");
	fprintf(fp,"IGZpbGUuCiEhCSBUaGUgZGVmYXVsdHMgYXJlIHNldCBpbnRlcm5hbGx5IGlu\n");
	fprintf(fp,"IE13bSB0byAnRGVmYXVsdEJ1dHRvbkJpbmRpbmdzJyBhbmQKISEJICdEZWZh\n");
	fprintf(fp,"dWx0S2V5QmluZGluZ3MnIHJlc3BlY3RpdmVseS4KISEKISEJIFVzZXJzIGFu\n");
	fprintf(fp,"ZCBzeXN0ZW0gYWRtaW4gbmVlZCBvbmx5IGRlZmluZSB0aGUgZGVmYXVsdCBi\n");
	fprintf(fp,"dXR0b24gYW5kIGtleQohIQkgYmluZGluZ3MgaW4gdGhlIGFwcHJvcHJpYXRl\n");
	fprintf(fp,"IG13bSBjb25maWcgZmlsZSAoaWUuIC5td21yYyBvcgohIQkgc3lzdGVtLm13\n");
	fprintf(fp,"bXJjKS4KISEKISEJIFRoZSAnd2luZG93TWVudScgcmVzb3VyY2UgZGVmYXVs\n");
	fprintf(fp,"dHMgdG8gJ0RlZmF1bHRXaW5kb3dNZW51JywgYW5kCiEhCSBpbiBNd20gMS4y\n");
	fprintf(fp,"IHRoZXJlIGlzIGEgZGVmYXVsdCByb290IG1lbnUgdGhhdCBpcyBkZWZpbmVk\n");
	fprintf(fp,"IGFzCiEhCSAnRGVmYXVsdFJvb3RNZW51Jy4KISEKCiFNd20qbW92ZVRocmVz\n");
	fprintf(fp,"aG9sZDoJCTMKCiEKISBjb21wb25lbnQgYXBwZWFyYW5jZSByZXNvdXJjZXMg\n");
	fprintf(fp,"CiEKCiEhCiEhIE5PVEU6IEluIE13bSAxLjIsIHRoZSByZXNvdXJjZXMgJ3Jl\n");
	fprintf(fp,"c2l6ZUJvcmRlcldpZHRoJyBhbmQgJ2ZyYW1lQm9yZGVyV2lkdGgnCiEhCSBh\n");
	fprintf(fp,"cmUgY2FsY3VsYXRlZCBkeW5hbWljYWxseSBkZXBlbmRpbmcgb24gdGhlIHNp\n");
	fprintf(fp,"emUgYW5kIHJlc29sdXRpb24gb2YKISEJIHRoZSBzY3JlZW4uICBJbiBnZW5l\n");
	fprintf(fp,"cmFsLCB5b3Ugc2hvdWxkIG5vdCBoYXZlIHRvIHNldCB0aGVzZSByZXNvdXJj\n");
	fprintf(fp,"ZXMKISEJIGluIHRoZSBNd20gYXBwLWRlZmF1bHRzIGZpbGUuCiEhCgohIEZP\n");
	fprintf(fp,"TlQgc3R1ZmYKTXdtKmZvbnRMaXN0OgkJCXZhcmlhYmxlCgohIFVzZSBzbWFs\n");
	fprintf(fp,"bGVyIGZpeGVkIGZvbnQgZm9yIGljb25zCk13bSppY29uKmZvbnRMaXN0OgkJ\n");
	fprintf(fp,"Zml4ZWQKCiEhIENsaWVudCBkZWNvcmF0aW9ucyBmb3IgY29tbW9uIGNsaWVu\n");
	fprintf(fp,"dHMgLSBTQU1QTEVTCiFNd20qWENsb2NrKmNsaWVudERlY29yYXRpb246CQli\n");
	fprintf(fp,"b3JkZXIKIU13bSpYTG9hZCpjbGllbnREZWNvcmF0aW9uOgkJYm9yZGVyCiFN\n");
	fprintf(fp,"d20qWEJpZmYqY2xpZW50RGVjb3JhdGlvbjoJCWJvcmRlcgoKIQohICBFTkQg\n");
	fprintf(fp,"T0YgUkVTT1VSQ0UgU1BFQ0lGSUNBVElPTlMKIQohIEFkZGVkIGJ5IFJEQSBT\n");
	fprintf(fp,"eXN0ZW1zIEluYy4KCk13bSpjbGllbnRBdXRvUGxhY2U6CUZhbHNlCk13bSpw\n");
	fprintf(fp,"b3NpdGlvbklzRnJhbWU6IAlGYWxzZQpNd20qaW50ZXJhY3RpdmVQbGFjZW1l\n");
	fprintf(fp,"bnQ6IEZhbHNlCgpNd20qRm9yZWdyb3VuZDoJYmxhY2sgIApNd20qQmFja2dy\n");
	fprintf(fp,"b3VuZDoJbGlnaHRncmV5Ck13bSphY3RpdmVGb3JlZ3JvdW5kOgkjOGU5MzAw\n");
	fprintf(fp,"Ck13bSphY3RpdmVCYWNrZ3JvdW5kOgkjNGYyZDdmCgo=\n");
	fprintf(fp,"====\n");
	fprintf(fp,"EOF\n");
	fprintf(fp,"for xxx in `echo \"$XRDBFILES\"`\n");
	fprintf(fp,"do\n");
	fprintf(fp,"	grep -v 'xrdb -merge' <\"$xxx\" >\"$xxx\".new\n");
	fprintf(fp,"	mv -v \"$xxx\".new \"$xxx\"\n");
	fprintf(fp,"done\n");
	fprintf(fp,"# rm -fv /rda/rdabin/megs*\n");
	fprintf(fp," \n");
	fprintf(fp,"if [ -d /rda/xpert ]\n");
	fprintf(fp,"then\n");
	fprintf(fp,"	chown rda:xpert /rda/rdabin/*\n");
	fprintf(fp,"	chmod 770 /rda/rdabin/*\n");
	fprintf(fp,"elif [ -d /rda/mbgui ]\n");
	fprintf(fp,"then\n");
	fprintf(fp,"	chown rda:mbgui /rda/rdabin/*\n");
	fprintf(fp,"	chmod 770 /rda/rdabin/*\n");
	fprintf(fp,"else\n");
	fprintf(fp,"	echo \"Group No found.  Permissions Not Reset.\"\n");
	fprintf(fp,"fi\n");
	fclose(fp);
	chmod(temp,(mode_t)MODE_RWXRWX___);
	if(temp!=NULL) Rfree(temp);
#endif
}
void SCRIPT_openoffice()
{
#ifndef WIN32
	SCRIPT_soffice();
	SCRIPT_scalc();
#endif
}
void SCRIPT_runscript()
{
#ifndef WIN32
	FILE *fp=NULL;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("runscript.lnx")+15);
	sprintf(temp,"%s/runscript.lnx",CURRENTDIRECTORY);
	unlink(temp);
	fp=fopen(temp,"w+b");

	fprintf(fp,"#!/bin/sh\n");
	fprintf(fp,"\"$2\"\n");
	fclose(fp);
	chmod(temp,(mode_t)MODE_RWXRWX___);
	if(temp!=NULL) Rfree(temp);
#endif
}
void SCRIPT_Runscript()
{
#ifndef WIN32
	SCRIPT_runscript();
	SCRIPT_perdmgimp();
#endif
}
