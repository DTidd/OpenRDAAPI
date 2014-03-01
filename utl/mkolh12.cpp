/* OLHELP's - Make (SENDMYVNC) Power Add */
#include <cstdio>
#include <mkadd.hpp>
#include <mkmsc.hpp>
#include <mkdef.hpp>

#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */

extern char *module;
extern APPlib *errorlist;

static void ADD_SENDMYVNC()
{
	int x=0;
	PowerAdd *add=NULL;
	char *defdir=NULL;
	APPlib *args=NULL,*args2=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;
	char *temp5=NULL,*temp6=NULL;
	char *temp7=NULL,*temp8=NULL;
	char *temp9=NULL;

/* ADD DESCRIPTION */
	temp1=Rmalloc(41+1);
	sprintf(temp1,"%s",
		"Poweradd used as a wrapper for sendmyvnc.");


/* SAVE EXPRESSION */
	temp2=Rmalloc(3+1);
	sprintf(temp2,"%s",
		"");


/* SAVE ERROR DESCRIPTION */
	temp3=Rmalloc(3+1);
	sprintf(temp3,"%s",
		"");


/* SAVE WARNING */
	temp4=Rmalloc(3+1);
	sprintf(temp4,"%s",
		"");


/* SAVE WARNING DESCRIPTION */
	temp5=Rmalloc(3+1);
	sprintf(temp5,"%s",
		"");


/* QUIT ERROR */
	temp6=Rmalloc(3+1);
	sprintf(temp6,"%s",
		"");


/* QUIT ERROR DESCRIPTION */
	temp7=Rmalloc(3+1);
	sprintf(temp7,"%s",
		"");


/* QUIT WARNING */
	temp8=Rmalloc(3+1);
	sprintf(temp8,"%s",
		"");


/* QUIT WARNING DESCRIPTION */
	temp9=Rmalloc(3+1);
	sprintf(temp9,"%s",
		"");
	add=RDAPowerAddNEW(module,"SENDMYVNC",temp1,"SENDMYVNC","SENDMYVNC DEFINE LIST","",3,temp2,temp3,temp4,temp5,temp6,temp7,temp8,temp9,0,NULL,0,NULL,0,NULL,0,NULL);
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
	if(temp5!=NULL) Rfree(temp5);
	if(temp6!=NULL) Rfree(temp6);
	if(temp7!=NULL) Rfree(temp7);
	if(temp8!=NULL) Rfree(temp8);
	if(temp9!=NULL) Rfree(temp9);
	if(add!=NULL)
	{

/* POWERADD BUTTONS  */

/* SENDMYVNC sendmyvnc BUTTON  */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		
		args=APPlibNEW();
		temp2=Rmalloc(108+1);
		sprintf(temp2,"%s",
			"IF([STYLE]=0) THEN RETURN_VALUE=(INTEGER_TO_STRING([LISTENING VIEWER])) \nELSE RETURN_VALUE=[HOST NAME] FI");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		temp2=Rmalloc(101+1);
		sprintf(temp2,"%s",
			"IF([STYLE]=1) THEN RETURN_VALUE=(INTEGER_TO_STRING([LISTENING VIEWER])) \nELSE RETURN_VALUE=\"\" FI");
		addAPPlib(args,temp2);
		if(temp2!=NULL) Rfree(temp2);
		x=addPowerAddButton(add,"sendmyvnc","SendMyVNc","SENDMYVNC","OLHELP","RSCNOT","RSCNOT KEY",temp1,1,1,args,0,NULL);
		if(args!=NULL) freeapplib(args);
		if(temp1!=NULL) Rfree(temp1);


/* POWERADD EDIT WIDGETS  */


/* STYLE EDIT WIDGET */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"STYLE",7,1,0,temp1,0,"","","",0,NULL,temp2,"","STYLE",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* HOST NAME EDIT WIDGET */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"HOST NAME",1,40,0,temp1,0,"","","",0,NULL,temp2,"","HOST NAME",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

/* LISTENING VIEWER EDIT WIDGET */
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		x=addPowerAddEditWdgts(add,"LISTENING VIEWER",8,5,0,temp1,0,"","","",0,NULL,temp2,"","LISTENING VIEWER",temp3,0,NULL,0,NULL);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.ADD",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.ADD",CURRENTDIRECTORY,module);
#endif
		if(writePowerAdd(defdir,add))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(9+6+89+1);
			sprintf(temp1,"POWER ADD WRITE ERROR: Module [OLHELP] Power Add [SENDMYVNC], Can Not Save Power Add!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(args!=NULL) freeapplib(args);
		if(args2!=NULL) freeapplib(args2);
		if(add!=NULL) FreePowerAdd(add);
	}
}
static void MNU_OLHELP_SENDMYVNC()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("OLHELP SENDMYVNC","doadd.EXT OLHELP \"SENDMYVNC\"");
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
static void SCN_SENDMYVNC()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("OLHELP","SENDMYVNC");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Style:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"SENDMYVNC STYLES","Style:","","",2,0,0,NULL,NULL,NULL,"IF([STYLE]=0) THEN RETURN_VALUE=\"LISTENING VIEWER\" ELSE RETURN_VALUE=\"HOST NAME\" FI");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Host Name:","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(11+1);
		sprintf(temp3,"%s",
			"([STYLE]=1)");
		temp4=Rmalloc(83+1);
		sprintf(temp4,"%s",
			"\"LISTENING VIEWER\"");
		ADVaddwdgt(scrn,0,"HOST NAME","Host Name:","","",0,40,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Listening Viewer:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"LISTENING VIEWER","Listening Viewer:","","",0,5,0,NULL,NULL,NULL,"\"SAVE\"");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(3+1);
		sprintf(temp1,"%s",
			"");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(64+1);
		sprintf(temp3,"%s",
			"([LISTENING VIEWER]!=0) AND (([STYLE]=0) OR ([HOST NAME]!=\"\"))");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,6,"SAVE","Send","","",0,0,32,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(9+6+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [OLHELP] Screen [SENDMYVNC], Can Not Save Maintain Master!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
}
static void SCN_SENDMYVNC_DEFINE_LIST()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("OLHELP","SENDMYVNC DEFINE LIST");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Enter a position and length for each of the following:","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,5,"","Host Name:","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,5,"","Listening Viewer:","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,5,"","Style:","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,0,"HOST NAME POSITION","Host Name Position","","",0,3,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,0,"HOST NAME LENGTH","Host Name Length","","",0,3,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,0,"LISTENING VIEWER POSITION","Listening Viewer Position","","",0,3,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,0,"LISTENING VIEWER LENGTH","Listening Viewer Length","","",0,3,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,0,"STYLE POSITION","Style Position","","",0,3,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,0,"STYLE LENGTH","Style Length","","",0,3,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,15,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
	ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);

	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
	sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
	if(writescrnbin(defdir,scrn))
	{
		if(temp1!=NULL) Rfree(temp1);
		temp1=Rmalloc(21+6+89+1);
		sprintf(temp1,"SCREEN WRITE ERROR: Module [OLHELP] Screen [SENDMYVNC DEFINE LIST], Can Not Save Maintain Master!");
		prterr(temp1);
		if(errorlist!=NULL)
		{
			addERRlist(&errorlist,temp1);
		}
		if(temp1!=NULL) Rfree(temp1);
	}
	if(defdir!=NULL) Rfree(defdir);
	if(scrn!=NULL) free_scrn(scrn);
	}
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
}
static void CREATE_SENDMYVNC_SCRIPT()
{
	FILE *fp=NULL;
	char *temp=NULL;
#ifndef WIN32
	int fd=0;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("sendmyvnc.lnx")+15);
	sprintf(temp,"%s/sendmyvnc.lnx",CURRENTDIRECTORY);
	unlink(temp);
	fp=fopen(temp,"w+b");

	fprintf(fp,"#!/bin/bash\n");
	fprintf(fp,"\n");
	fprintf(fp,"FindDisplayNumber()\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	DA=( `echo ${DISPLAY//:/ }` )\n");
	fprintf(fp,"\n");
	fprintf(fp,"	if [ ${#DA[@]} == 1 ]\n");
	fprintf(fp,"	then\n");
	fprintf(fp,"		echo \"DISPLAY has no IP/hostname\"\n");
	fprintf(fp,"		X=( `echo ${DA[0]//./ }` )\n");
	fprintf(fp,"	\n");
	fprintf(fp,"		DISPVALS[0]=''\n");
	fprintf(fp,"		DISPVALS[1]=\"${X[0]}\"\n");
	fprintf(fp,"		DISPVALS[2]=\"${X[1]}\"\n");
	fprintf(fp,"		unset X\n");
	fprintf(fp,"		\n");
	fprintf(fp,"	elif [ ${#DA[@]} == 2 ]\n");
	fprintf(fp,"	then\n");
	fprintf(fp,"		echo \"DISPLAY has IP/hostname\"\n");
	fprintf(fp,"		X=( `echo ${DA[1]//./ }` )\n");
	fprintf(fp,"	\n");
	fprintf(fp,"		DISPVALS[0]=\"${DA[0]}\"\n");
	fprintf(fp,"		DISPVALS[1]=\"${X[0]}\"\n");
	fprintf(fp,"		DISPVALS[2]=\"${X[1]}\"\n");
	fprintf(fp,"		unset X\n");
	fprintf(fp,"	else\n");
	fprintf(fp,"		echo \"Error with DISPLAY variable.  Could not parse.\"\n");
	fprintf(fp,"		exit 1\n");
	fprintf(fp,"	fi\n");
	fprintf(fp,"\n");
	fprintf(fp,"	echo [${DISPVALS[0]}]     [${DISPVALS[1]}]     [${DISPVALS[2]}]\n");
	fprintf(fp,"	echo \"VNC_NUMBER=${DISPVALS[1]}\"	\n");
	fprintf(fp,"}\n");
	fprintf(fp,"\n");
	fprintf(fp,"DetectSendingPrograms()\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	# This function should search for th vncconnect utility path and for the x11vnc path including for a x11vnc.lnx to be distributed by RDA\n");
	fprintf(fp,"	X11LOC=( \"/rda/xpgms/x11vnc.lnx\" \"/usr/local/bin/x11vnc\" \"/usr/bin/x11vnc\" \"/usr/bin/X11/x11vnc\" $(which -a x11vnc.lnx)  $(which -a x11vnc) )\n");
	fprintf(fp,"	for xxx in ${X11LOC[@]}\n");
	fprintf(fp,"	do\n");
	fprintf(fp,"		if [ -x \"$xxx\" ]\n");
	fprintf(fp,"		then\n");
	fprintf(fp,"			X11VNC=\"$xxx\"\n");
	fprintf(fp,"			break;\n");
	fprintf(fp,"		fi\n");
	fprintf(fp,"	done\n");
	fprintf(fp,"\n");
	fprintf(fp,"	VNCCONNLOC=( \"/rda/xpgms/vncconnect.lnx\" \"/usr/local/bin/vncconnect\" \"/usr/bin/vncconnect\" \"/usr/bin/X11/vncconnect\" $(which -a vncconnect.lnx)  $(which -a vncconnect) )\n");
	fprintf(fp,"	for xxx in ${VNCCONNLOC[@]}\n");
	fprintf(fp,"	do\n");
	fprintf(fp,"		if [ -x \"$xxx\" ]\n");
	fprintf(fp,"		then\n");
	fprintf(fp,"			VNCCONN=\"$xxx\"\n");
	fprintf(fp,"			break;\n");
	fprintf(fp,"		fi\n");
	fprintf(fp,"	done\n");
	fprintf(fp,"\n");
	fprintf(fp,"	VNCCONFIG=( \"/rda/xpgms/vncconfig.lnx\" \"/opt/TigerVNC/bin/vncconfig\" \"/usr/local/bin/vncconfig\" \"/usr/bin/vncconfig\" \"/usr/bin/X11/vncconfig\" $(which -a vncconfig.lnx)  $(which -a vncconfig) )\n");
	fprintf(fp,"	for xxx in ${VNCCONFIG[@]}\n");
	fprintf(fp,"	do\n");
	fprintf(fp,"		if [ -x \"$xxx\" ]\n");
	fprintf(fp,"		then\n");
	fprintf(fp,"			VNCCONFIG=\"$xxx\"\n");
	fprintf(fp,"			break;\n");
	fprintf(fp,"		fi\n");
	fprintf(fp,"	done\n");
	fprintf(fp,"}\n");
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	fprintf(fp,"SendVNCSession()\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	ELF32=`file /bin/bash | grep -c 'ELF 32-bit'`\n");
	fprintf(fp,"	ELF64=`file /bin/bash | grep -c 'ELF 64-bit'`\n");
	fprintf(fp,"	X11_XVNC=`xdpyinfo | grep 'vendor string' | grep -c 'AT&T'`	\n");
	fprintf(fp,"	X11_XF86=`xdpyinfo | grep 'vendor string' | grep -c 'The XFree86 Project'`	\n");
	fprintf(fp,"	X11_XORG=`xdpyinfo | grep 'vendor string' | grep -c 'AT&T'`	\n");
	fprintf(fp,"	X11_XTEST=`xdpyinfo | grep -c 'XTEST'`\n");
	fprintf(fp,"	X11_VNC=`xdpyinfo | grep -c 'VNC-EXTENSION'`\n");
	fprintf(fp,"	X11_SHM=`xdpyinfo | grep -c 'MIT-SHM'`\n");
	fprintf(fp,"	X11_RANDR=`xdpyinfo | grep -c 'RANDR'`\n");
	fprintf(fp,"	X11_DAMAGE=`xdpyinfo | grep -c 'DAMAGE'`\n");
	fprintf(fp,"\n");
	fprintf(fp,"	if [ $X11_XORG -gt 0 -a $X11_VNC -gt 0 ]\n");
	fprintf(fp,"	then\n");
	fprintf(fp,"		IS_TIGERVNC=1\n");
	fprintf(fp,"		IS_XORG=0\n");
	fprintf(fp,"	elif [ $X11_XORG -gt 0 -a $X11_VNC == 0 ]\n");
	fprintf(fp,"	then\n");
	fprintf(fp,"		IS_TIGERVNC=0\n");
	fprintf(fp,"		IS_XORG=1\n");
	fprintf(fp,"	else\n");
	fprintf(fp,"		IS_TIGERVNC=0\n");
	fprintf(fp,"		IS_XORG=0\n");
	fprintf(fp,"	fi\n");
	fprintf(fp,"\n");
	fprintf(fp,"	if [ $X11_XVNC -gt 0 -a $IS_TIGERVNC == 0 -a $IS_XORG == 0 ]\n");
	fprintf(fp,"	then\n");
	fprintf(fp,"		IS_XVNC=1\n");
	fprintf(fp,"	else\n");
	fprintf(fp,"		IS_XVNC=0\n");
	fprintf(fp,"	fi\n");
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	fprintf(fp,"	if [ $ELF32 -gt 0 ]\n");
	fprintf(fp,"	then\n");
	fprintf(fp,"		if [ $X11_XVNC -gt 0 ]\n");
	fprintf(fp,"		then\n");
	fprintf(fp,"			echo \"METHOD=XVNC\"	\n");
	fprintf(fp,"			$VNCCONN -display 127.0.0.1:${DISPVALS[1]} ${LISTEN_IP[0]}:${LISTEN_PORT[0]}\n");
	fprintf(fp,"		elif [ $X11_XF86 -gt 0 -o $X11_XORG -gt 0 ]\n");
	fprintf(fp,"		then\n");
	fprintf(fp,"			echo \"METHOD=X11VNC\"	\n");
	fprintf(fp,"			\"$X11VNC\" -connect ${LISTEN_IP[0]}:${LISTEN_PORT[0]} 	\n");
	fprintf(fp,"			if [ \"$?\" -gt 0 ]\n");
	fprintf(fp,"			then\n");
	fprintf(fp,"				\"$X11VNC\" -noshm -connect ${LISTEN_IP[0]}:${LISTEN_PORT[0]} 	\n");
	fprintf(fp,"			fi\n");
	fprintf(fp,"		elif [ $X11_XTEST -gt 0 ]\n");
	fprintf(fp,"		then\n");
	fprintf(fp,"			echo \"METHOD=X11VNC\"	\n");
	fprintf(fp,"			\"$X11VNC\" -noshm -connect ${LISTEN_IP[0]}:${LISTEN_PORT[0]}\n");
	fprintf(fp,"			if [ \"$?\" -gt 0 ]\n");
	fprintf(fp,"			then\n");
	fprintf(fp,"				\"$X11VNC\" -connect ${LISTEN_IP[0]}:${LISTEN_PORT[0]} 	\n");
	fprintf(fp,"			fi\n");
	fprintf(fp,"		else\n");
	fprintf(fp,"			echo \"Unknown X11 Server type.  Should not occur...  Trying X11VNC method...\"\n");
	fprintf(fp,"			\"$X11VNC\" -noshm -connect ${LISTEN_IP[0]}:${LISTEN_PORT[0]}\n");
	fprintf(fp,"			if [ \"$?\" -gt 0 ]\n");
	fprintf(fp,"			then\n");
	fprintf(fp,"				\"$X11VNC\" -connect ${LISTEN_IP[0]}:${LISTEN_PORT[0]} 	\n");
	fprintf(fp,"			fi\n");
	fprintf(fp,"		fi\n");
	fprintf(fp,"	else\n");
	fprintf(fp,"		if [ $IS_XVNC == 1 ]\n");
	fprintf(fp,"		then\n");
	fprintf(fp,"			$VNCCONN -display 127.0.0.1:${DISPVALS[1]} ${LISTEN_IP[0]}:${LISTEN_PORT[0]}\n");
	fprintf(fp,"		else\n");
	fprintf(fp,"			VERSION=( `x11vnc -version` )\n");
	fprintf(fp,"			if [ \"${VERSION[1]}\" = \"0.9.13\" ]\n");
	fprintf(fp,"			then\n");
	fprintf(fp,"				if [ $X11_SHM -gt 0 ]\n");
	fprintf(fp,"				then\n");
	fprintf(fp,"					\"$X11VNC\" -nopw -bg -rfbport 0 -ping 20 -readtimeout 120 -nomodtweak -connect_or_exit ${LISTEN_IP[0]}:${LISTEN_PORT[0]}\n");
	fprintf(fp,"				else\n");
	fprintf(fp,"					\"$X11VNC\" -noshm -nopw -bg -rfbport 0 -ping 20 -readtimeout 120 -nomodtweak -connect_or_exit ${LISTEN_IP[0]}:${LISTEN_PORT[0]}\n");
	fprintf(fp,"				fi\n");
	fprintf(fp,"			else\n");
	fprintf(fp,"				if [ $X11_SHM -gt 0 ]\n");
	fprintf(fp,"				then\n");
	fprintf(fp,"					\"$X11VNC\" -nopw -bg -connect_or_exit ${LISTEN_IP[0]}:${LISTEN_PORT[0]}\n");
	fprintf(fp,"				else\n");
	fprintf(fp,"					\"$X11VNC\" -noshm -nopw -bg -connect_or_exit ${LISTEN_IP[0]}:${LISTEN_PORT[0]}\n");
	fprintf(fp,"				fi\n");
	fprintf(fp,"			fi\n");
	fprintf(fp,"		fi\n");
	fprintf(fp,"	fi\n");
	fprintf(fp,"\n");
	fprintf(fp,"}\n");
	fprintf(fp,"\n");
	fprintf(fp,"FindDisplayNumber \n");
	fprintf(fp,"DetectSendingPrograms\n");
	fprintf(fp,"if [ \"$#\" == 1 ]\n");
	fprintf(fp,"then\n");
	fprintf(fp,"	LISTEN_IP[0]=\"support-srv.rdasys.com\"\n");
	fprintf(fp,"	if [ ${#1} == 2 ]\n");
	fprintf(fp,"	then\n");
	fprintf(fp,"		LISTEN_PORT[0]=\"55\"\"$1\"\n");
	fprintf(fp,"	elif [ ${#1} -gt 2 ]\n");
	fprintf(fp,"	then\n");
	fprintf(fp,"		LISTEN_PORT[0]=\"$1\"\n");
	fprintf(fp,"	else \n");
	fprintf(fp,"		echo \"Invalid Arguments.\"\n");
	fprintf(fp,"	fi\n");
	fprintf(fp,"elif [ \"$#\" == 2 ]\n");
	fprintf(fp,"then\n");
	fprintf(fp,"	LISTEN_IP[0]=\"$1\"\n");
	fprintf(fp,"	if [ ${#2} == 2 ]\n");
	fprintf(fp,"	then\n");
	fprintf(fp,"		LISTEN_PORT[0]=\"55\"\"$2\"\n");
	fprintf(fp,"	elif [ ${#2} -gt 2 ]\n");
	fprintf(fp,"	then\n");
	fprintf(fp,"		LISTEN_PORT[0]=\"$2\"\n");
	fprintf(fp,"	else \n");
	fprintf(fp,"		echo \"Invalid Arguments.\"\n");
	fprintf(fp,"	fi\n");
	fprintf(fp,"else\n");
	fprintf(fp,"	echo \"You Must Enter Arguments To This Command.\"\n");
	fprintf(fp,"	exit 1\n");
	fprintf(fp,"fi\n");
	fprintf(fp,"\n");
	fprintf(fp,"SendVNCSession\n");

	fclose(fp);
	fd=chmod(temp,(mode_t)MODE_RWXRWX___);
	if(fd==(-1))
	{
		prterr("Error:  Couldn't set Mode for [%s].",(temp!=NULL ? temp:""));
	}
#endif

}

static void LST_SENDMYVNC_STYLES()
{
	RDAScrolledList *list=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	list=RDAScrolledListNew("OLHELP","SENDMYVNC STYLES");
	if(list!=NULL)
	{
		list->vtype=0;
		list->type=1;
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->select_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->special_function=stralloc("");
		list->num=0;
		list->initfld=NULL;
		list->ffield=stralloc("");
		list->ffinfo=Rmalloc(sizeof(DFincvir));
		list->ffinfo->module=stralloc("");
		list->ffinfo->file=stralloc("");
		list->ffinfo->keyname=stralloc("");
		list->contype=1;
		list->conmod=stralloc("STYLE");
		list->confil=stralloc("");
		list->confld=stralloc("");
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->format_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->reformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
	temp1=Rmalloc(3+1);
	sprintf(temp1,"%s",
		"");
		list->unformat_formula=stralloc(temp1);
		if(temp1!=NULL) Rfree(temp1);
		list->desc=stralloc("");
		list->timing=0;
		list->list=APPlibNEW();
		addAPPlib(list->list,"Listening Viewer Only");
		addAPPlib(list->list,"Dynamic Host & Listening Viewer");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.LST",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.LST",CURRENTDIRECTORY,module);
#endif
		if(writeScrolledListbin(defdir,list))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+6+100+1);
			sprintf(temp1,"SCROLLED LIST WRITE ERROR: Module [OLHELP] RDAScrolledList [SENDMYVNC STYLES], Can Not Save Scrolled List!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(list!=NULL) FreeRDAScrolledList(list);
	}
}
static void DEF_SENDMYVNC()
{
	RDAdefault *def=NULL;
	char *defdir=NULL;
	char *temp1=NULL;

	def=RDAdefaultNEW("OLHELP","SENDMYVNC");
	if(def!=NULL)
	{
		def->input_focus=stralloc("LISTENING VIEWER");
		addDefaultFieldINT(def,"PREVIOUS ADD'S",TRUE,13,0);
		addDefaultFieldSHORT(def,"STYLE",TRUE,7,0);
		addDefaultFieldSTRING(def,"HOST NAME",TRUE,1,"");
		addDefaultFieldINT(def,"LISTENING VIEWER",TRUE,8,0);
		addDefaultFieldINT(def,"SENDMYVNC STYLES",TRUE,13,0);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.DEF",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.DEF",CURRENTDIRECTORY,module);
#endif
		if(writedefaultbin(defdir,def))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(9+6+100+1);
			sprintf(temp1,"SCREEN DEFAULT WRITE ERROR: Module [OLHELP] Screen [SENDMYVNC], Can Not Save Screen Defaults!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(def!=NULL) FreeRDAdefault(def);
	}
}
void SENDMYVNC()
{
	ADD_SENDMYVNC();
	MNU_OLHELP_SENDMYVNC();
	SCN_SENDMYVNC();
	SCN_SENDMYVNC_DEFINE_LIST();
	CREATE_SENDMYVNC_SCRIPT();
	LST_SENDMYVNC_STYLES();
	DEF_SENDMYVNC();
}
