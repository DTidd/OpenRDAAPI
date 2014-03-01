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

static void SCRIPT_fixcdrecordlog_lnx(void);

static void SCRIPT_fixcdrecordlog_lnx()
{
	FILE *fp=NULL;
	char *name=NULL;
        char *temp=NULL;
        int fd=(-1);

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("fixcdrecordlog.lnx")+15);
#ifdef WIN32
	sprintf(temp,"fixcdrecordlog.lnx");
#endif
#ifndef WIN32
	sprintf(temp,"fixcdrecordlog.lnx");
#endif
	unlink(temp);
	fp=fopen(temp,"w+b");
	if(temp!=NULL) Rfree(temp);
	fprintf(fp,"#!/usr/bin/perl\n");
	fprintf(fp,"# Formats cdrecord output to properly viewable text for rdaout.\n");
	fprintf(fp,"$input = \"$ARGV[0]\";\n");
	fprintf(fp,"open(INPUT, $input) || die \"Can't open $input for read\";\n");
	fprintf(fp,"while (<INPUT>) {        \n");
	fprintf(fp,"    chomp ($_);		\n");
	fprintf(fp,"    $_ =~ s/(.)/ord $1 > 177 ? '' : $1/ge;\n");
	fprintf(fp,"    $_ =~ s/Track/\\nTrack/g;\n");
	fprintf(fp,"    print(\"$_\\n\");\n");
	fprintf(fp,"}\n");
	fprintf(fp,"    close(ARGV);        \n");
	fprintf(fp,"close(INPUT);		\n");
	fprintf(fp,"\n");
	fclose(fp);

	name=Rmalloc(18);
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
        sprintf(name,"fixcdrecordlog.lnx");
#endif

#ifndef WIN32
                fd=chmod(name,(mode_t)MODE_RWXRWX___);
                if(fd==(-1))
                {
                        prterr("Error:  Couldn't set Mode for [%s].",(name!=NULL ? name:""));
                }
#endif

}

int SCRIPTfixcdrecordlog()
{
	SCRIPT_fixcdrecordlog_lnx();
}
