/* makehtmlcode.c - Generate HTML & CGI Script for speed of Web Development*/
#ifndef WIN32
#define __NAM__ "dfwww.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dfwww.exe"
#endif
#include <app.hpp>
#include <mix.hpp>
/*CPP_OPART dfwww1 */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */

APPlib *scr_libs=NULL,*scr_defs=NULL,*dir_libs=NULL,*RDA_APIs=NULL;
APPlib *APIavl=NULL;
static void getscreenlist(RDArsrc *);
char *url=NULL,*prog_path=NULL,*prevlink=NULL;
RDArsrc *mrsrc=NULL;
extern void HTMLCreateWidgets(FILE *,RDAscrn *,char),HTMLsetrscnum(RDAscrn *);
int RDAapi=0,APIselected=0;
static void rdaapifunc(RDArsrc *);

static void makescrncode(RDAscrn *scrn,FILE *fp,char *dirname,char *libname,char *scrnname,char *cgifile)
{
	if(fp!=NULL)
	{	
		fprintf(fp,"<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n");
		fprintf(fp,"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\"\n");
		fprintf(fp,"	\"DTD/xhtml1-1-transitional.dtd\">\n");
		fprintf(fp,"<html xmlns=\"http://www.w3.org/1999/xhtml\">\n");
		fprintf(fp,"<META NAME=\"Generator\" CONTENT=\"XPERT HTML INTERFACE\"\n");
		fprintf(fp,"<HEAD>\n");
		fprintf(fp,"<TITLE>%s</TITLE>\n",(scrnname!=NULL ? scrnname:""));
		fprintf(fp,"</HEAD>\n");
		fprintf(fp,"<BODY bgcolor=\"#C0C0C0\" text=\"#000000\" link=\"#660066\" alink=\"#9999CC\" vlink=\"#666633\">\n");

		fprintf(fp,"<B><FONT SIZE=10><P ALIGN=\"CENTER\"><A HREF=\"%s/index.htm\"><IMG SRC=\"Logo.gif\" ALT=\"%s\"></A>%s</P>\n",(url!=NULL ? url:""),(url!=NULL ? url:""),(scrnname!=NULL ? scrnname:""));
		fprintf(fp,"</B></FONT><FONT SIZE=4>\n");
#ifdef WIN32
		fprintf(fp,"<FORM METHOD=\"POST\"	ACTION=\"%s/%s\">\n",(url!=NULL ? url:""),(cgifile!=NULL ? cgifile:""));
#endif
#ifndef WIN32
		fprintf(fp,"<FORM METHOD=\"POST\"	ACTION=\"%s/%s\">\n",(url!=NULL ? url:""),(cgifile!=NULL ? cgifile:""));
#endif
		fprintf(fp,"<INPUT TYPE=\"hidden\" NAME=\"LOGIN\" VALUE=\"\">\n");
		fprintf(fp,"<INPUT TYPE=\"hidden\" NAME=\"PASSWORD\" VALUE=\"\">\n");
		fprintf(fp,"</B></FONT>\n");
		fprintf(fp,"<FONT SIZE=4>\n");
		HTMLCreateWidgets(fp,scrn,FALSE);
		fprintf(fp,"</FONT>\n");
		fprintf(fp,"</FORM>\n");
		fprintf(fp,"<HR>\n");
		fprintf(fp,"<h5><font face=\"Arial\" size=\"1\">Send mail to <A HREF=\"mailto:info@rdasys.com\">info@rdasys.com</A> with<BR>questions or comments about this web site.<br><P>Copyright © 2004 RDA Systems, Inc.<br></h5></BODY></HTML>\n");
	}

}
static void makescriptcode(RDAscrn *scrn,FILE *fp,char *dirname,char *libname,
	char *scrnname,char *cgifile)
{
	int x=0;
	RDAwdgt *wdgt=NULL;
	char *dashes=NULL;

	if(fp!=NULL)
	{	
		fprintf(fp,"#!/usr/bin/perl\n");
#ifndef WIN32
		fprintf(fp,"push(@INC,\"%s/cgi-bin/\");\n",CURRENTDIRECTORY);
#endif
#ifdef WIN32
		fprintf(fp,"push(@INC,\"%s\\cgi-bin\\\");\n",CURRENTDIRECTORY);
#endif
		fprintf(fp,"do \"cgi-lib.pl\" || die \"Fatal Error:  Can't load cgi library, cgi-lib.pl!\";\n");
		fprintf(fp,"&ReadParse;\n");

		fprintf(fp,"print \"Content-type: text/html\\n\\n\";\n");
		fprintf(fp,"print \"<HTML>\";\n");
		fprintf(fp,"print \"<HEAD><TITLE>%s</TITLE>\";\n",scrnname);
		fprintf(fp,"print \"</HEAD><BODY>\";\n");
		fprintf(fp,"print \"<H2 ALIGN=\"CENTER\">%s</H2>\";\n",scrnname);
		fprintf(fp,"print \"<HR>\";\n");
		fprintf(fp,"$error=$ENV{'SAVE ERROR'};\n");
		fprintf(fp,"$warning=$ENV{'SAVE WARNING'};\n");
/* EMPTY SCREEN OR ERRORS  */
		fprintf(fp,"if(($#in<0) || ($error>0) || ($warning>0)\n");
		fprintf(fp,"{\n");
		fprintf(fp,"\t$error=0;\n");
		fprintf(fp,"\t$warning=0;\n");
		fprintf(fp,"\tprint \"<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\";\n");
		fprintf(fp,"\tprint \"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\"\";\n");
		fprintf(fp,"\tprint \"	\"DTD/xhtml1-1-transitional.dtd\">\";\n");
		fprintf(fp,"\tprint \"<html xmlns=\"http://www.w3.org/1999/xhtml\">\";\n");
		fprintf(fp,"\tprint \"<META NAME=\"Generator\" CONTENT=\"XPERT HTML INTERFACE\"\";\n");
		fprintf(fp,"\tprint \"<HEAD>\";\n");
		fprintf(fp,"\tprint \"<TITLE>%s</TITLE>\";\n",(scrnname!=NULL ? scrnname:""));
		fprintf(fp,"\tprint \"</HEAD>\";\n");
		fprintf(fp,"\tprint \"<BODY bgcolor=\"#C0C0C0\" text=\"#000000\" link=\"#660066\" alink=\"#9999CC\" vlink=\"#666633\">\";\n");

		fprintf(fp,"\tprint \"<B><FONT SIZE=10><P ALIGN=\"CENTER\"><A HREF=\"%s/index.htm\"><IMG SRC=\"Logo.gif\" ALT=\"%s\"></A>%s</P>\";\n",(url!=NULL ? url:""),(url!=NULL ? url:""),(scrnname!=NULL ? scrnname:""));
		fprintf(fp,"\tprint \"</B></FONT><FONT SIZE=4>\";\n");
#ifdef WIN32
		fprintf(fp,"\tprint \"<FORM METHOD=\"POST\"	ACTION=\"%s/%s\">\";\n",(url!=NULL ? url:""),(cgifile!=NULL ? cgifile:""));
#endif
#ifndef WIN32
		fprintf(fp,"\tprint \"<FORM METHOD=\"POST\"	ACTION=\"%s/%s\">\";\n",(url!=NULL ? url:""),(cgifile!=NULL ? cgifile:""));
#endif
		fprintf(fp,"\tprint \"<INPUT TYPE=\"hidden\" NAME=\"LOGIN\" VALUE=\"\">\";\n");
		fprintf(fp,"\tprint \"<INPUT TYPE=\"hidden\" NAME=\"PASSWORD\" VALUE=\"\">\";\n");
		fprintf(fp,"\tprint \"</B></FONT>\";\n");
		fprintf(fp,"\tprint \"<FONT SIZE=4>\";\n");
		HTMLCreateWidgets(fp,scrn,TRUE);
		fprintf(fp,"\tprint \"</FONT>\";\n");
		fprintf(fp,"\tprint \"</FORM>\";\n");
		fprintf(fp,"\n");
		fprintf(fp,"\n");
		fprintf(fp,"} else {\n");
/* HAS SOMETHING */
		fprintf(fp,"\n");

		fprintf(fp,"\t$ENV{'LOGNAME'}=$in{'LOGIN'};\n");
		fprintf(fp,"\t$ENV{'LOGNAME'}=$in{'LOGIN'};\n");

/* Widget Values */
		for(x=0,wdgt=scrn->wdgts;x<scrn->numwdgts;++x,++wdgt)
		{
			if(wdgt->resource_num>(-1))
			{
				dashes=adddashes(wdgt->resource_name);
				switch(wdgt->type)
				{
					case 0: /* standard resource */
					case 8: /* standard resource */
					case 9: /* toggle */
					case 15: /* custom type */
					case 16: /* expenditure type */
					case 17: /* revenue */
					case 18: /* balance sheet */
					case 19: /* beginning balance */
						fprintf(fp,"\t$ENV{'%s'}=$in{'%s'};\n",(dashes!=NULL ? dashes:""),(dashes!=NULL ? dashes:""));
						break;
					default:
						break;
				}
				if(dashes!=NULL) Rfree(dashes);
			}
		}
		fprintf(fp,"\tchdir \"%s\";\n",CURRENTDIRECTORY);
		fprintf(fp,"\tprint \"<HR>\";\n");
		fprintf(fp,"\tprint \"<PRE>\";\n");
		switch(RDAapi)
		{
			default:
			case 0: /* doadd */
#ifdef WIN32
				fprintf(fp,"\texec '%s\\wwwdoadd.exe','%s','%s';\n",(prog_path!=NULL ? prog_path:""),(libname!=NULL ? libname:""),APIavl->libs[APIselected]);
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
				fprintf(fp,"\texec '%s/wwwdoadd.lnx','%s','%s';\n",(prog_path!=NULL ? prog_path:""),(libname!=NULL ? libname:""),APIavl->libs[APIselected]);
#endif
#ifdef LINUX
				fprintf(fp,"\texec '%s/wwwdoadd.lnx','%s','%s';\n",(prog_path!=NULL ? prog_path:""),(libname!=NULL ? libname:""),APIavl->libs[APIselected]);
#endif
#ifdef RISC6000
				fprintf(fp,"\texec '%s/wwwdoadd.rsc','%s','%s';\n",(prog_path!=NULL ? prog_path:""),(libname!=NULL ? libname:""),APIavl->libs[APIselected]);
#endif
#ifdef UNIXWARE7
				fprintf(fp,"\texec '%s/wwwdoadd.unw','%s','%s';\n",(prog_path!=NULL ? prog_path:""),(libname!=NULL ? libname:""),APIavl->libs[APIselected]);
#endif
#ifdef SCO50P
				fprintf(fp,"\texec '%s/wwwdoadd.sco','%s','%s';\n",(prog_path!=NULL ? prog_path:""),(libname!=NULL ? libname:""),APIavl->libs[APIselected]);
#endif
				break;
			case 1: /* doreport */
#ifdef WIN32
				fprintf(fp,"\texec '%s/wwwdorpt.exe','%s','%s';\n",(prog_path!=NULL ? prog_path:""),(libname!=NULL ? libname:""),APIavl->libs[APIselected]);
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
				fprintf(fp,"\texec '%s/wwwdorpt.lnx','%s','%s';\n",(prog_path!=NULL ? prog_path:""),(libname!=NULL ? libname:""),APIavl->libs[APIselected]);
#endif
#ifdef LINUX
				fprintf(fp,"\texec '%s/wwwdorpt.lnx','%s','%s';\n",(prog_path!=NULL ? prog_path:""),(libname!=NULL ? libname:""),APIavl->libs[APIselected]);
#endif
#ifdef RISC6000
				fprintf(fp,"\texec '%s/wwwdorpt.rsc','%s','%s';\n",(prog_path!=NULL ? prog_path:""),(libname!=NULL ? libname:""),APIavl->libs[APIselected]);
#endif
#ifdef UNIXWARE7
				fprintf(fp,"\texec '%s/wwwdorpt.unw','%s','%s';\n",(prog_path!=NULL ? prog_path:""),(libname!=NULL ? libname:""),APIavl->libs[APIselected]);
#endif
#ifdef SCO50P
				fprintf(fp,"\texec '%s/wwwdorpt.sco','%s','%s';\n",(prog_path!=NULL ? prog_path:""),(libname!=NULL ? libname:""),APIavl->libs[APIselected]);
#endif
				break;
			case 2: /* domenu */
				break;
		}
		fprintf(fp,"\tprint \"</PRE>\";\n");
		fprintf(fp,"\tprint \"<P>\";\n");
		fprintf(fp,"\tprint \"<HR>\";\n");
		fprintf(fp,"\tprint \"<P>\";\n");
		fprintf(fp,"\t$error=$ENV{'SAVE ERROR'};\n");
		fprintf(fp,"\tunless ($error <= 0)\n");
		fprintf(fp,"\t{\n");
		fprintf(fp,"\t\tprint \"<H2 ALIGN=\"CENTER\">Error(s) Reported</H2>\";\n");
		fprintf(fp,"\t\tprint \"<P>\";\n");
		fprintf(fp,"\t\tprint \"<PRE>\";\n");
		fprintf(fp,"\t\tprint \"$error\";\n");
		fprintf(fp,"\t\tprint \"</PRE>\";\n");
		fprintf(fp,"\t\tprint \"<P>\";\n");
		fprintf(fp,"\t}\n");
		fprintf(fp,"\t$warning=$ENV{'SAVE WARNING'};\n");
		fprintf(fp,"\tunless ($warning <= 0)\n");
		fprintf(fp,"\t{\n");
		fprintf(fp,"\t\tprint \"<H2 ALIGN=\"CENTER\">Warning(s) Reported</H2>\";\n");
		fprintf(fp,"\t\tprint \"<P>\";\n");
		fprintf(fp,"\t\tprint \"<PRE>\";\n");
		fprintf(fp,"\t\tprint \"$warning\";\n");
		fprintf(fp,"\t\tprint \"</PRE>\";\n");
		fprintf(fp,"\t\tprint \"<P>\";\n");
		fprintf(fp,"\t}\n");
		fprintf(fp,"}\n");
		fprintf(fp,"print \"<HR>\";\n");
		fprintf(fp,"print \"<h5><font face=\"Arial\" size=\"1\">Send mail to <A HREF=\"mailto:info@rdasys.com\">info@rdasys.com</A> with<BR>questions or comments about this web site.<br><P>Copyright © 2004 RDA Systems, Inc.<br></h5></BODY></HTML>\";\n");
	}
}
static void quit_printRDAscrncode(RDArsrc *prsrc)
{
        if(diaggui)
        {
                prterr("DIAG quit_printRDAscrncode Quitting Print RDA Screen Code");
        }
        free_rsrc(prsrc);
}
static void printRDAscrncode(RDArsrc *prsrc,RDAscrn *scrn)
{
	char *htmlpath=NULL,*htmlfile=NULL,*cgipath=NULL,*cgifile=NULL;
	char *outdevice=NULL,*cgidevice=NULL;
	FILE *fp=NULL;
	char *dirname=NULL,*libname=NULL,*scrnname=NULL;
	int l=0,s=0;

	if(scrn!=NULL)
	{
		readwidget(prsrc,"HTML PATH");
		readwidget(prsrc,"HTML FILENAME");
		readwidget(prsrc,"CGI PATH");
		readwidget(prsrc,"CGI FILENAME");
		FINDRSCGETSTRING(prsrc,"HTML PATH",&htmlpath);
		FINDRSCGETSTRING(prsrc,"HTML FILENAME",&htmlfile);
		FINDRSCGETSTRING(prsrc,"CGI PATH",&cgipath);
		FINDRSCGETSTRING(prsrc,"CGI FILENAME",&cgifile);
		if(FINDRSCGETSTRING(mrsrc,"DIRECTORY",&dirname))
		{
			return;
		}
		if(FINDRSCGETINT(mrsrc,"LIBRARY LIST",&l))
		{
			return;
		}
		if(FINDRSCGETINT(mrsrc,"SCREEN LIST",&s))
		{
			return;
		}
		outdevice=Rmalloc(RDAstrlen(htmlpath)+RDAstrlen(htmlfile)+5);
#ifndef WIN32
		sprintf(outdevice,"%s/%s",(htmlpath!=NULL ? htmlpath:""),
			(htmlfile!=NULL ? htmlfile:""));
#endif
#ifdef WIN32
		sprintf(outdevice,"%s\\%s",(htmlpath!=NULL ? htmlpath:""),
			(htmlfile!=NULL ? htmlfile:""));
#endif
		cgidevice=Rmalloc(RDAstrlen(cgipath)+RDAstrlen(cgifile)+5);
#ifndef WIN32
		sprintf(cgidevice,"%s/%s",(cgipath!=NULL ? cgipath:""),
			(cgifile!=NULL ? cgifile:""));
#endif
#ifdef WIN32
		sprintf(cgidevice,"%s\\%s",(cgipath!=NULL ? cgipath:""),
			(cgifile!=NULL ? cgifile:""));
#endif
		if(outdevice!=NULL)
		{
			unlink(outdevice);
			fp=fopen(outdevice,"a+b");
                	if(fp!=NULL)
                	{
				libname=stralloc(scr_libs->libs[l]);
				scrnname=stralloc(scr_defs->libs[s]);
				stoupper(scrnname);
				stoupper(scrn->name);
				makescrncode(scrn,fp,dirname,libname,scrnname,cgifile);
				fclose(fp);
			} else {
				ERRORDIALOG("HTML DEVICE COMMUNICATIONS FAILED!","The output device failed to open and will not receive output.",NULL,FALSE);
				prterr("Error PrintRDAscrn popen call for output device [%s] failed.",(outdevice!=NULL?outdevice:""));
			}
		} else {
			ERRORDIALOG("HTML DEVICE FAILURE!","There was no output device specified.",NULL,FALSE);
		}
		if(cgidevice!=NULL)
		{
			unlink(cgidevice);
			fp=fopen(cgidevice,"a+b");
                	if(fp!=NULL)
                	{
				libname=stralloc(scr_libs->libs[l]);
				scrnname=stralloc(scr_defs->libs[s]);
				stoupper(scrnname);
				stoupper(scrn->name);
				makescriptcode(scrn,fp,dirname,libname,scrnname,cgifile);
				fclose(fp);
#ifndef WIN32
				chmod(cgidevice,(mode_t)MODE_RWXRWX___);
#endif
			} else {
				ERRORDIALOG("CGI DEVICE COMMUNICATIONS FAILED!","The output device failed to open and will not receive output.",NULL,FALSE);
				prterr("Error PrintRDAscrn popen call for output device [%s] failed.",(outdevice!=NULL?outdevice:""));
			}
		} else {
			ERRORDIALOG("CGI DEVICE FAILURE!","There was no output device specified.",NULL,FALSE);
		}


	} else {
		ERRORDIALOG("SCREEN ERROR!","The attempt to retrieve the screen definition failed.",NULL,FALSE);
		prterr("SCREEN RETRIVAL ERROR: The attempt to retrieve the screen definition failed.",NULL,FALSE);
	}
	if(dirname!=NULL) Rfree(dirname);
	if(libname!=NULL) Rfree(libname);
	if(scrnname!=NULL) Rfree(scrnname);
	if(outdevice!=NULL) Rfree(outdevice);
	if(cgidevice!=NULL) Rfree(cgidevice);
}
static void GetDevicePrintRDAscrncode(RDAscrn *scrn)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL,*dashes=NULL;

	prsrc=RDArsrcNEW("UTILITIES","WWW PRINT SCREEN");
	defaultprinter=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+20);
	sprintf(defaultprinter,"%s",CURRENTDIRECTORY);
	addstdrsrc(prsrc,"HTML PATH",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);

	dashes=adddashes(scrn->name);
	defaultprinter=Rmalloc(RDAstrlen(dashes)+20);
#ifndef WIN32
	sprintf(defaultprinter,"html/%s.htm",dashes);
#endif
#ifdef WIN32
	sprintf(defaultprinter,"html\\%s.htm",dashes);
#endif
	addstdrsrc(prsrc,"HTML FILENAME",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);

	defaultprinter=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+20);
	sprintf(defaultprinter,"%s",CURRENTDIRECTORY);
	addstdrsrc(prsrc,"CGI PATH",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);

	defaultprinter=Rmalloc(RDAstrlen(dashes)+20);
#ifndef WIN32
	sprintf(defaultprinter,"cgi-bin/%s.cgi",dashes);
#endif
#ifdef WIN32
	sprintf(defaultprinter,"cgi-bin\\%s.cgi",dashes);
#endif
	addstdrsrc(prsrc,"CGI FILENAME",VARIABLETEXT,0,defaultprinter,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	if(dashes!=NULL) Rfree(dashes);

	addrfkwrsrc(prsrc,"PRINT",TRUE,printRDAscrncode,scrn);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_printRDAscrncode,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PRINT SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		free_rsrc(prsrc);
	}
}
static RDAscrn *okscreen(char *dirname,char *libname,char *scrnname)
{
	RDAscrn *scrn=NULL;
	char *dirx=NULL,*libx=NULL;

	if(RDAstrcmp(libname,"Directory Contains No Libraries"))
	{
		if(scrn!=NULL) free_scrn(scrn); 
		scrn=RDAscrnNEW(libname,scrnname);
		dirx=Rmalloc(RDAstrlen(dirname)+2);
		sprintf(dirx,"%s/",dirname);
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(libname)+6);
		sprintf(libx,"%s/%s.SCN",dirx,libname);
		if(dirx!=NULL) Rfree(dirx);
		if(getscrnbin(libx,scrn,FALSE))
		{
			prterr("ERROR: Cannot Read Screen Definition.  Error in permissions or screen [%s][%s] doesn't exist.  Check the permissions, and retry.",libname,scrnname);
			if(scrn!=NULL) free_scrn(scrn);
			if(libx!=NULL) Rfree(libx);
			return(NULL);
		} else {
			if(libx!=NULL) Rfree(libx);
			return(scrn);
		}
	} else { 
		return(NULL);
	}
}
void okscreencb(RDArsrc *mainrsrc)
{
	RDAscrn *scrn=NULL;
	int l=0,s=0;
	char *libname=NULL,*scrnname=NULL,*dirname=NULL;

	readwidget(mrsrc,"URL");
	readwidget(mrsrc,"PROGRAM PATH");
	readwidget(mrsrc,"PREVIOUS LINK");
	FINDRSCGETSTRING(mrsrc,"URL",&url);
	FINDRSCGETSTRING(mrsrc,"PROGRAM PATH",&prog_path);
	FINDRSCGETSTRING(mrsrc,"PREVIOUS LINK",&prevlink);
	FINDRSCGETINT(mrsrc,"RDA API",&RDAapi);
	FINDRSCGETINT(mrsrc,"API AVAILABLE",&APIselected);
	if(FINDRSCGETSTRING(mrsrc,"DIRECTORY",&dirname))
	{
		return;
	}
	if(FINDRSCGETINT(mrsrc,"LIBRARY LIST",&l))
	{
		return;
	} else {
		libname=stralloc(scr_libs->libs[l]);
	}
	if(FINDRSCGETINT(mrsrc,"SCREEN LIST",&s))
	{
		return;
	} else {
		scrnname=stralloc(scr_defs->libs[s]);
	}
	scrn=okscreen(dirname,libname,scrnname);
	if(scrn!=NULL)
	{
		HTMLsetrscnum(scrn);
		GetDevicePrintRDAscrncode(scrn);
	}
	if(dirname!=NULL) Rfree(dirname);
	if(libname!=NULL) Rfree(libname);
	if(scrnname!=NULL) Rfree(scrnname);
}
static void doliblist(RDArsrc *mainrsrc)
{
	int l=0,s=0;
	char *libx=NULL;
	char *dirx=NULL;

	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirx)) return;
	if(FINDRSCGETINT(mainrsrc,"LIBRARY LIST",&l)) return;
	FINDRSCGETINT(mainrsrc,"SCREEN LIST",&s);
	freeapplib(scr_defs);
	if(!RDAstrcmp(scr_libs->libs[l],"Directory Contains No Libraries"))
	{
		scr_defs=APPlibNEW();
		addAPPlib(scr_defs,"Contains No Screen Definitions");
		s=0;
	} else {
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(scr_libs->libs[l])+6);
		sprintf(libx,"%s/%s.SCN",dirx,scr_libs->libs[l]);
		scr_defs=getlibnames(libx,FALSE);
		if(scr_defs==NULL)
		{
			scr_defs=APPlibNEW();
			addAPPlib(scr_defs,"Contains No Screen Definitions");
			s=0;
		}
	}
	if(dirx!=NULL) Rfree(dirx);
	if(libx!=NULL) Rfree(libx);
	if(s>=scr_defs->numlibs) s=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"SCREEN LIST",s,scr_defs))
	{
		updatersrc(mainrsrc,"SCREEN LIST");
	}
	rdaapifunc(mainrsrc);
}
void getrptentries(RDArsrc *mainrsrc)
{
	int x,selm=0;
	char *tmp=NULL,*dirx=NULL,*temp=NULL,*modname=NULL;

	if(FINDRSCGETINT(mainrsrc,"LIBRARY LIST",&selm)) return;
	if(APIavl!=NULL) freeapplib(APIavl);
	APIavl=APPlibNEW();
	modname=scr_libs->libs[selm];	
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modname)+3);
#ifndef WIN32
	sprintf(dirx,"%s/%s/",CURRENTDIRECTORY,modname);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\%s\\",CURRENTDIRECTORY,modname);
#endif
	for(x=0;findfile(dirx,"*.RPT",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		temp=stripdashes(tmp);
		addAPPlib(APIavl,temp);
		if(temp!=NULL) Rfree(temp);
		temp=NULL;
	}
	if(APIavl->numlibs<1)
	{
		addAPPlib(APIavl,"No Report Definitions Available");
	}
	if(dirx!=NULL) Rfree(dirx);
	if(tmp!=NULL) Rfree(tmp);
	SORTAPPlib(APIavl);
	if(selm>=APIavl->numlibs) selm=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"API AVAILABLE",selm,APIavl))
		updatersrc(mainrsrc,"API AVAILABLE");
}
static void DOADDdoliblist1(RDArsrc *mainrsrc)
{
	int selected=0,selm=0;
	char *libx=NULL;
	char *dirx=NULL;

	readwidget(mainrsrc,"DIRECTORY");
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirx)) return;
	if(FINDRSCGETINT(mainrsrc,"LIBRARY LIST",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"API AVAILABLE",&selm)) return;
	if(APIavl!=NULL) freeapplib(APIavl);
	libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(scr_libs->libs[selected])+6);
#ifndef WIN32
	sprintf(libx,"%s/%s.ADD",dirx,scr_libs->libs[selected]);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.ADD",dirx,scr_libs->libs[selected]);
#endif
	APIavl=getlibnames(libx,FALSE);
	if(dirx!=NULL) Rfree(dirx);
	if(libx!=NULL) Rfree(libx);
	if(APIavl==NULL)
	{
		APIavl=APPlibNEW();
		addAPPlib(APIavl,"No Power Add Definitions");
	}
	if(selm>=APIavl->numlibs) selm=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"API AVAILABLE",selm,APIavl))
		updatersrc(mainrsrc,"API AVAILABLE");
}
static void rdaapifunc(RDArsrc *mainrsrc)
{
	FINDRSCGETINT(mainrsrc,"RDA API",&RDAapi);
	switch(RDAapi)
	{
		default:
		case 0: /* doadd */
			DOADDdoliblist1(mainrsrc);
			break;
		case 1: /* doreport */
			getrptentries(mainrsrc);
			break;
		case 2: /* domenu */
			if(APIavl!=NULL) freeapplib(APIavl);
			APIavl=APPlibNEW();
			addAPPlib(APIavl,"Not Applicable");
			if(!FINDRSCLISTAPPlib(mainrsrc,"API AVAILABLE",0,APIavl)) updatersrc(mainrsrc,"API AVAILABLE");
			break;
	}
}
static void getscreenlist(RDArsrc *mainrsrc)
{
	int l=0,x=0;
	char *dirname=NULL;
	char *tmp=NULL,*dirx=NULL;

	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirname))
	{
		return;
	}
	if(FINDRSCGETINT(mainrsrc,"LIBRARY LIST",&l))
	{
		l=0;
	}
	FINDRSCGETINT(mainrsrc,"RDA API",&RDAapi);
	if(scr_libs!=NULL) freeapplib(scr_libs);
	scr_libs=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(dirname)+2);
	sprintf(dirx,"%s/",dirname);
	for(x=0;findfile(dirx,"*.SCN",&tmp,(int)x+1);++x)
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(scr_libs,tmp);
	}
	if(scr_libs->numlibs<1)
	{
		addAPPlib(scr_libs,"Directory Contains No Libraries");
	} else {
		SORTAPPlib(scr_libs);
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	if(l>=scr_libs->numlibs) l=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"LIBRARY LIST",l,scr_libs))
	{
		updatersrc(mainrsrc,"LIBRARY LIST");
	}
	doliblist(mainrsrc);
	rdaapifunc(mainrsrc);
	if(dirname!=NULL) Rfree(dirname);
}
static void setdirlist(RDArsrc *mainrsrc)
{
	int x=0;
	char *directory=NULL;
	char inlist=FALSE;

	readwidget(mainrsrc,"DIRECTORY");
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	inlist=FALSE;
	for(x=0;x<dir_libs->numlibs;++x)
	{
		if(!RDAstrcmp(dir_libs->libs[x],directory))
		{
			inlist=TRUE;
			break;
		}
	}
	if(inlist!=TRUE)
	{
		addAPPlib(dir_libs,directory);
		x=dir_libs->numlibs-1;
	}
	if(directory!=NULL) Rfree(directory);
	FINDRSCLISTAPPlib(mainrsrc,"DIRECTORY LIST",x,dir_libs);
	updatersrc(mainrsrc,"DIRECTORY LIST");
	getscreenlist(mainrsrc);
}
static void getdir(RDArsrc *mainrsrc)
{
	int s=0;
	char *dirstr=NULL;

	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirstr)) return;
	if(FINDRSCGETINT(mainrsrc,"DIRECTORY LIST",&s)) return;
	if(RDAstrcmp(dirstr,dir_libs->libs[s]))
	{
		if(FINDRSCSETSTRING(mainrsrc,"DIRECTORY",dir_libs->libs[s])) return;
		updatersrc(mainrsrc,"DIRECTORY");
	}
	if(dirstr!=NULL) Rfree(dirstr);
	getscreenlist(mainrsrc);
}
void quitscrncode(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	if(scr_libs!=NULL) freeapplib(scr_libs);
	if(scr_defs!=NULL) freeapplib(scr_defs);
	if(dir_libs!=NULL) freeapplib(dir_libs);
	if(RDA_APIs!=NULL) freeapplib(RDA_APIs);
	if(url!=NULL) Rfree(url);
	if(prog_path!=NULL) Rfree(prog_path);
	if(prevlink!=NULL) Rfree(prevlink);
	ShutdownSubsystems();
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int d=0,l=0,s=0,x=0;
	char *defdir=NULL;

	if(InitializeSubsystems(argc,argv,"UTILITIES","DEFINE WWW"))
	{
		ShutdownSubsystems();
		exit(1);
	}
	mrsrc=RDArsrcNEW("UTILITIES","DEFINE WWW");
	scr_libs=APPlibNEW();
	scr_defs=APPlibNEW();
	dir_libs=APPlibNEW();
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(USERLOGIN)+2);
#ifndef WIN32
	sprintf(defdir,"%s/%s",CURRENTDIRECTORY,USERLOGIN);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\%s",CURRENTDIRECTORY,USERLOGIN);
#endif
	addAPPlib(dir_libs,defdir);
	if(defdir!=NULL) Rfree(defdir); 
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(defdir,"%s/site",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\site",CURRENTDIRECTORY);
#endif
	addAPPlib(dir_libs,defdir);
	if(defdir!=NULL) Rfree(defdir); 
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+5);
#ifndef WIN32
	sprintf(defdir,"%s/rda",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\rda",CURRENTDIRECTORY);
#endif
	addAPPlib(dir_libs,defdir);
	if(defdir!=NULL) Rfree(defdir); 
	addlstrsrc(mrsrc,"DIRECTORY LIST",0,TRUE,getdir,dir_libs->numlibs,&dir_libs->libs,NULL);
	addstdrsrc(mrsrc,"DIRECTORY",VARIABLETEXT,0,dir_libs->libs[0],TRUE);
	if(defdir!=NULL) Rfree(defdir);
	FINDRSCLISTAPPlib(mrsrc,"DIRECTORY LIST",0,dir_libs);
	FINDRSCSETFUNC(mrsrc,"DIRECTORY",setdirlist,NULL);
	addlstrsrc(mrsrc,"LIBRARY LIST",0,TRUE,doliblist,scr_libs->numlibs,&scr_libs->libs,NULL);
	addlstrsrc(mrsrc,"SCREEN LIST",0,TRUE,NULL,scr_defs->numlibs,&scr_defs->libs,NULL);
	if(argc>1)
	{
		for(d=0;d<dir_libs->numlibs;++d)
		{
			if(!RDAstrcmp(argv[1],dir_libs->libs[d])) break;
		}
		if(d>=dir_libs->numlibs)
		{
			addAPPlib(dir_libs,argv[1]);
		}
		FINDRSCLISTAPPlib(mrsrc,"DIRECTORY LIST",d,dir_libs);
		FINDRSCSETSTRING(mrsrc,"DIRECTORY",argv[1]);
	}
	getscreenlist(mrsrc);
	if(argc>2)
	{
		for(l=0;l<scr_libs->numlibs;++l)
		{
			if(!RDAstrcmp(argv[2],scr_libs->libs[l])) break;
		}
		if(l>=scr_libs->numlibs)
		{
			l=0;
		}
	}
	FINDRSCLISTAPPlib(mrsrc,"LIBRARY LIST",l,scr_libs);
	doliblist(mrsrc);
	if(argc>3)
	{
		for(s=0;s<scr_defs->numlibs;++s)
		{
			if(!RDAstrcmp(argv[3],scr_defs->libs[s])) break;
		}
		if(s>=scr_defs->numlibs)
		{
			s=0;
		}
	}
	FINDRSCLISTAPPlib(mrsrc,"SCREEN LIST",s,scr_defs);

	url=stralloc("http://64.0.0.103:80");
	addstdrsrc(mrsrc,"URL",VARIABLETEXT,0,url,TRUE);
	prog_path=stralloc(CURRENTDIRECTORY);
	addstdrsrc(mrsrc,"PROGRAM PATH",VARIABLETEXT,0,prog_path,TRUE);
	prevlink=stralloc("index");
	addstdrsrc(mrsrc,"PREVIOUS LINK",VARIABLETEXT,0,prevlink,TRUE);
	RDA_APIs=APPlibNEW();
	addAPPlib(RDA_APIs,"doadd");
	addAPPlib(RDA_APIs,"doreport");
	addAPPlib(RDA_APIs,"domenu");
	addlstrsrc(mrsrc,"RDA API",0,TRUE,rdaapifunc,RDA_APIs->numlibs,&RDA_APIs->libs,NULL);
	if(APIavl!=NULL) freeapplib(APIavl); 
	APIavl=APPlibNEW();
	addlstrsrc(mrsrc,"API AVAILABLE",&x,TRUE,NULL,
		APIavl->numlibs,&APIavl->libs,NULL);
	DOADDdoliblist1(mrsrc);
	addrfcbrsrc(mrsrc,"SELECT",TRUE,okscreencb,NULL);
	addrfexrsrc(mrsrc,"QUIT",TRUE,quitscrncode,NULL);
	addrfcbrsrc(mrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mrsrc,TRUE,quitscrncode,NULL,TRUE);
}
