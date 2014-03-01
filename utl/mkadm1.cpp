/* UTILITIES's - Make (SENDMYVNC SCRIPT) */
#include <mkmsc.hpp>

extern char *module;
#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)
#define MODE_RWXRWXR_X	(S_IRWXU|S_IRWXG)
#endif /* Not LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWXR_X	(00770)
#endif /* LINUX || LINUX2_2 || UBUNTU_OS*/

static void SENDMYVNC_ADM_SCRIPT()
{
	FILE *fp;
	char *name=NULL;
	int fd=(-1);

	name=Rmalloc(12);
#ifdef WIN32
	sprintf(name,"sendmyvnc.bat");
#endif
#ifdef RS6000
	sprintf(name,"sendmyvnc.rsc");
#endif
#ifdef SCO50
	sprintf(name,"sendmyvnc.sco");
#endif
#ifdef SCO50P
	sprintf(name,"sendmyvnc.sco");
#endif
#ifdef UNIXWARE21
	sprintf(name,"sendmyvnc.unw");
#endif
#ifdef UNIXWARE7
	sprintf(name,"sendmyvnc.unw");
#endif
#ifdef LINUX
	sprintf(name,"./scripts/sendmyvnc");
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
	sprintf(name,"./scripts/sendmyvnc");
#endif
	fp=fopen(name,"w+");
	if(fp!=NULL)
	{
#ifdef UNIXWARE7
#endif
#ifdef LINUX
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
		fprintf(fp,"CheckString()\n");
		fprintf(fp,"{\n");
		fprintf(fp,"	if [ $(echo $1 | cut -c\"$START_NUMBER\"'-'\"$CUT_NUMBER\") == \":\" 2>/dev/null ]\n");
		fprintf(fp,"	then\n");
		fprintf(fp,"		LOOP_FINISHED=1\n");
		fprintf(fp,"	else\n");
		fprintf(fp,"		START_NUMBER=`expr $START_NUMBER + 1`\n");
		fprintf(fp,"		CUT_NUMBER=`expr $CUT_NUMBER + 1`\n");
		fprintf(fp,"	fi\n");
		fprintf(fp,"}\n");
		fprintf(fp,"\n");
		fprintf(fp,"\n");
		fprintf(fp,"for xxx in `echo $DISPLAY | awk '{print $1}'`\n");
		fprintf(fp,"do\n");
		fprintf(fp,"	TEST1=0\n");
		fprintf(fp,"	TEST2=0\n");
		fprintf(fp,"	TEST3=0\n");
		fprintf(fp,"	if [ $(echo $xxx | cut -c0-1) == \":\" ]\n");
		fprintf(fp,"	then\n");
		fprintf(fp,"		if [ $(echo $xxx | cut -c3-3) == \".\" ]\n");
		fprintf(fp,"		then\n");
		fprintf(fp,"			VNC_NUMBER=`echo $xxx | cut -c2-2`\n");
		fprintf(fp,"		else\n");
		fprintf(fp,"			VNC_NUMBER=`echo $xxx | cut -c2-3`\n");
		fprintf(fp,"		fi\n");
		fprintf(fp,"	else	\n");
		fprintf(fp,"		TEST1=1\n");
		fprintf(fp,"	fi\n");
		fprintf(fp,"	\n");
		fprintf(fp,"	if [ $(echo $xxx | cut -c0-9) == \"127.0.0.1\" ]\n");
		fprintf(fp,"	then\n");
		fprintf(fp,"		if [ $(echo $xxx | cut -c12-12) == \".\" ]\n");
		fprintf(fp,"		then\n");
		fprintf(fp,"			VNC_NUMBER=`echo $xxx | cut -c11-11`\n");
		fprintf(fp,"		else\n");
		fprintf(fp,"			VNC_NUMBER=`echo $xxx | cut -c11-12`\n");
		fprintf(fp,"		fi\n");
		fprintf(fp,"	else	\n");
		fprintf(fp,"		TEST2=1\n");
		fprintf(fp,"	fi\n");
		fprintf(fp,"	\n");
		fprintf(fp,"	if [ $(echo $xxx | cut -c0-9) == \"localhost\" ]\n");
		fprintf(fp,"	then\n");
		fprintf(fp,"		if [ $(echo $xxx | cut -c12-12) == \".\" ]\n");
		fprintf(fp,"		then\n");
		fprintf(fp,"			VNC_NUMBER=`echo $xxx | cut -c11-11`\n");
		fprintf(fp,"		else\n");
		fprintf(fp,"			VNC_NUMBER=`echo $xxx | cut -c11-12`\n");
		fprintf(fp,"		fi\n");
		fprintf(fp,"	else	\n");
		fprintf(fp,"		TEST3=1\n");
		fprintf(fp,"	fi\n");
		fprintf(fp,"\n");
		fprintf(fp,"	if [ $TEST1 == 1 -a $TEST2 == 1 -a $TEST3 == 1 ]\n");
		fprintf(fp,"	then\n");
		fprintf(fp,"\n");
		fprintf(fp,"		START_NUMBER=0\n");
		fprintf(fp,"		CUT_NUMBER=0\n");
		fprintf(fp,"		LOOP_FINISHED=0\n");
		fprintf(fp,"		\n");
		fprintf(fp,"		until [ $LOOP_FINISHED == 1 ]\n");
		fprintf(fp,"		do\n");
		fprintf(fp,"			CheckString $DISPLAY\n");
		fprintf(fp,"		done	\n");
		fprintf(fp,"		\n");
		fprintf(fp,"		\n");
		fprintf(fp,"		VNC_START=`expr $CUT_NUMBER + 1`\n");
		fprintf(fp,"		VNC_CUT=`expr $CUT_NUMBER + 2`\n");
		fprintf(fp,"\n");
		fprintf(fp,"\n");
		fprintf(fp,"		if [ $(echo $xxx | cut -c\"$VNC_CUT\"'-'\"$VNC_CUT\") == \".\" ]\n");
		fprintf(fp,"		then	\n");
		fprintf(fp,"			VNC_NUMBER=`echo $xxx | cut -c\"$VNC_START\"'-'\"$VNC_START\"`\n");
		fprintf(fp,"		else\n");
		fprintf(fp,"			VNC_NUMBER=`echo $xxx | cut -c\"$VNC_START\"'-'\"$VNC_CUT\"`\n");
		fprintf(fp,"		fi\n");
		fprintf(fp,"		\n");
		fprintf(fp,"	fi\n");
		fprintf(fp,"	\n");
		fprintf(fp,"	echo \"VNC_NUMBER=$VNC_NUMBER\"	\n");
		fprintf(fp,"\n");
		fprintf(fp,"	vncconnect -display 127.0.0.1:$VNC_NUMBER 162.39.251.140:55$1\n");
		fprintf(fp,"done\n");
#endif
#ifdef WIN32
#endif
#ifdef RS6000
#endif
#ifdef SCO50
#endif
#ifdef SCO50P
#endif
#ifdef UNIXWARE21
#endif
		fclose(fp);
#ifndef WIN32
		fd=chmod(name,(mode_t)MODE_RWXRWXR_X);
		if(fd==(-1))
		{
			prterr("Error:  Couldn't set Mode for [%s].",(name!=NULL ? name:""));
		}
#endif
	}
	if(name!=NULL) Rfree(name);
}
static void SENDMYVNC_ADM_DATA_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("SENDMYVNC SCRIPT","sendmyvnc.EXT");
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
void SENDMYVNC_SCRIPT()
{
	SENDMYVNC_ADM_SCRIPT();
	SENDMYVNC_ADM_DATA_MENU();
}
