#include <mix.hpp>
#include <mkmsc.hpp>

extern char *module;
extern APPlib *errorlist;

static void MNU_UTILITIES_DOCRONTAB()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("UTILITIES DOCRONTAB","docrontab.EXT");
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
static void SCN_ADD_CRONTAB()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","ADD CRONTAB");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Do Which:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"PACKAGES","Packages","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Hour of Day:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"HOUR OF DAY","Hour of Day","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Minute of Hour:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"MINUTE","Minute","","",0,2,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Day of Month:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"DAY OF MONTH","Day of Month","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Month of Year:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"MONTH OF YEAR","Month","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Day of Week:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"DAY OF WEEK","Day of Week","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Command:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"COMMAND","Command","","",0,60,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(11+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [ADD CRONTAB], Can Not Save Maintain Master!");
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
static void SCN_MANAGE_CRONTABS()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","MANAGE CRONTAB");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Crontab's Defined:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"CRONTABS","Crontabs","","",10,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ADD","Add","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DELETE","Delete","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(15+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [MANAGE CRONTAB], Can Not Save Maintain Master!");
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
#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */

static void SCRIPT_rdacroncd_lnx(char *cd_bus,short cd_speed,short blank_cdrw)
{
	FILE *fp=NULL;
	char *temp=NULL;
#ifndef WIN32
	int fd=0;
#endif

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("rdacroncd.lnx")+15);
#ifdef WIN32
	sprintf(temp,"%s\\rdacroncd.lnx",CURRENTDIRECTORY);
#endif
#ifndef WIN32
	sprintf(temp,"%s/rdacroncd.lnx",CURRENTDIRECTORY);
#endif
	unlink(temp);
	fp=fopen(temp,"w+b");
	if(fp==NULL) { 
		prterr("Error opening file rdacroncd.lnx");
		return ;
	}

	fprintf(fp,"#!/bin/sh\n");
	fprintf(fp,"## Script that runs via cron and makes a MBGUI data backup cdrom.\n");
	fprintf(fp,"## These need to be filled in when the script is created\n");
	fprintf(fp,"#-------------------------------------------------------------------\n");
	fprintf(fp,"BUS=%s\n",cd_bus);
	fprintf(fp,"SPEED=%d\n",cd_speed);
	fprintf(fp,"DATA_DIR=%s\n",CURRENTDIRECTORY);
	fprintf(fp,"FILENAME=%s\n",(adddashes(XPERT_SETUP->company_name)));
	fprintf(fp,"BLANK_CDRW=%s\n",(blank_cdrw==0 ? "FALSE":"TRUE"));  
	fprintf(fp,"#-------------------------------------------------------------------\n");
	fprintf(fp,"Verify_Cdrom()\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	sleep 10\n");
	fprintf(fp,"	if [ -x /rda/xpgms/xorriso.lnx ]\n");
	fprintf(fp,"	then\n");
	fprintf(fp,"		xorriso.lnx -md5 all -indev $BUS -check_md5_r sorry / -- >>./rdacroncd.log 2>&1\n");
	fprintf(fp,"		VERIFY_STATUS=$?\n");
	fprintf(fp,"\n");
	fprintf(fp,"		xorriso.lnx --outdev $BUS --eject all -- >>./rdacroncd.log 2>&1\n");
	fprintf(fp,"\n");
	fprintf(fp,"		if [ $VERIFY_STATUS == 0 ]\n");
	fprintf(fp,"		then\n");
	fprintf(fp,"			echo \"\" >>./rdacroncd.log\n");
	fprintf(fp,"			echo \"Backup Created Successfully!\" >>./rdacroncd.log\n");
	fprintf(fp,"		else\n");
	fprintf(fp,"			echo \"\" >>./rdacroncd.log\n");
	fprintf(fp,"			echo \"ERROR:  BACKUP FAILED.  Failed To Verify Backup Disc.  Your Data Was Not Backed Up.\" >>./rdacroncd.log\n");
	fprintf(fp,"		fi	\n");
	fprintf(fp,"	else\n");
	fprintf(fp,"		mount /cdrom\n");
	fprintf(fp,"		sleep 20\n");
	fprintf(fp,"		ORIG_MD5=`cat ./\"$FILENAME\"\".md5\"`\n");
	fprintf(fp,"		BACKUP_MD5=`md5sum /cdrom/\"$FILENAME\"\".7z\" | awk '{print $1}'`\n");
	fprintf(fp,"	 \n");
	fprintf(fp,"		if [ \"$ORIG_MD5\" == \"$BACKUP_MD5\" ]\n");
	fprintf(fp,"		then\n");
	fprintf(fp,"			echo \"MD5SUMS Match\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"		else\n");
	fprintf(fp,"			echo \"MD5SUMS Don't Match\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"		fi\n");
	fprintf(fp,"	\n");
	fprintf(fp,"		if [ -f /cdrom/\"$DATEFILE\" -a -f /cdrom/\"$FILENAME\"\".7z\" -a \"$ORIG_MD5\" == \"$BACKUP_MD5\" ]\n");
	fprintf(fp,"		then\n");
	fprintf(fp," 		echo \"\" >>./rdacroncd.log\n");
	fprintf(fp,"		echo \"Backup Verified Successfully.  No Errors Encountered.\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"		else\n");
	fprintf(fp,"		echo \"\" >>./rdacroncd.log\n");
	fprintf(fp,"		echo \"ERROR:  Could not Verify CDROM Backup.  Your Backup may not have worked!\" >>./rdacroncd.log\n");
	fprintf(fp,"		echo \"Please Check Your Backup or Contact Support.\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"		cat /tmp/cdrecord.ERR >>./rdacroncd.log\n");
	fprintf(fp,"		fi\n");
	fprintf(fp,"		export LANG=C\n");
	fprintf(fp,"		$DATA_DIR/fixcdrecordlog.lnx rdacroncd.log >backup3.log\n");
	fprintf(fp,"		mv backup3.log rdacroncd.log\n");
	fprintf(fp,"		umount /cdrom\n");
	fprintf(fp,"		eject /cdrom\n");
	fprintf(fp,"	fi\n");
	fprintf(fp,"	rm -f /tmp/cdrecord.ERR \"$DATEFILE\" \"$FILENAME\"\".md5\"\n");
	fprintf(fp,"}\n");
	fprintf(fp,"\n");
	fprintf(fp,"PATH=\"/sbin:/usr/sbin:/bin:/usr/bin:/usr/bin/X11:/usr/local/sbin:/usr/local/bin:/rda/xpgms:/rda/rdabin\"\n");
	fprintf(fp,"export PATH\n");
	fprintf(fp,"echo \"-------------------------------------------------------------------\" >$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"echo \"Backup Executed by Nightly CRON\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"echo \"Date: `date`\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"echo \"Output Diagnostics: \" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"echo \"-------------------------------------------------------------------\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"cd \"$DATA_DIR\"\n");
	fprintf(fp,"if [ \"$?\" == 0 ]\n");
	fprintf(fp,"then\n");
	fprintf(fp,"	if [ -x /rda/xpgms/xorriso.lnx ]\n");
	fprintf(fp,"	then\n");
	fprintf(fp,"		if [ \"${BUS:0:5}\" != \"/dev/\" ]\n");
	fprintf(fp,"		then\n");
	fprintf(fp,"			AUTODEV=`xorriso.lnx --devices 2>/dev/null| grep '0  -dev' | awk '{print $3}'`\n");
	fprintf(fp,"			AUTODEV=\"${AUTODEV//\\\"/}\"\n");
	fprintf(fp,"			AUTODEV=\"${AUTODEV//\\'/}\"\n");
	fprintf(fp,"			if [ \"${AUTODEV:0:5}\" = \"/dev/\" ]\n");
	fprintf(fp,"			then\n");
	fprintf(fp,"				BUS=\"$AUTODEV\"\n");
	fprintf(fp,"			else\n");
	fprintf(fp,"				echo \"Invalid Device Specified, Could Not Auto Detect Correct Device.  Exiting...\" | tee -a ./backup.log\n");
	fprintf(fp,"				exit 1\n");
	fprintf(fp,"			fi\n");
	fprintf(fp,"		fi\n");
	fprintf(fp,"		\n");
	fprintf(fp,"		echo \"Detecting Drive Status...\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"		XORRISO_INFO=$(xorriso.lnx --outdev $BUS -- 2>&1)\n");
	fprintf(fp,"		echo \"$XORRISO_INFO\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"		MEDIA_ERR_UNSUITABLE=$(echo \"$XORRISO_INFO\" | grep -c 'Disc status unsuitable for writing')\n");
	fprintf(fp,"		MEDIA_ERR_RECOGNIZABLE=$(echo \"$XORRISO_INFO\" | grep -c 'Media current: is not recognizable')\n");
	fprintf(fp,"		MEDIA_ERR_NOT_PRESENT=$(echo \"$XORRISO_INFO\" | grep -c 'Media status : is not present')\n");
	fprintf(fp,"		MEDIA_WRITTEN_CLOSED=$(echo \"$XORRISO_INFO\" | grep -c 'Media status : is written , is closed')\n");
	fprintf(fp,"\n");
	fprintf(fp,"		if [ \"$MEDIA_ERR_NOT_PRESENT\" = \"1\" ]\n");
	fprintf(fp,"		then\n");
	fprintf(fp,"			echo \"No Optical Media Found In Drive.  Exiting...\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"			exit 1\n");
	fprintf(fp,"		fi\n");
	fprintf(fp,"\n");
	fprintf(fp,"		if [ \"$(echo \"$XORRISO_INFO\" | grep -c 'Media current: CD-RW')\" = \"1\" ]\n");
	fprintf(fp,"		then\n");
	fprintf(fp,"			MEDIA=\"CD-RW\"\n");
	fprintf(fp,"		elif [ \"$(echo \"$XORRISO_INFO\" | grep -c 'Media current: CD-R')\" = \"1\" ]\n");
	fprintf(fp,"		then\n");
	fprintf(fp,"			MEDIA=\"CD-R\"\n");
	fprintf(fp,"		elif [ \"$(echo \"$XORRISO_INFO\" | grep -c 'Media current: DVD+RW')\" = \"1\" ]\n");
	fprintf(fp,"		then\n");
	fprintf(fp,"			MEDIA=\"DVD+RW\"\n");
	fprintf(fp,"		elif [ \"$(echo \"$XORRISO_INFO\" | grep -c 'Media current: DVD+R')\" = \"1\" ]\n");
	fprintf(fp,"		then\n");
	fprintf(fp,"			MEDIA=\"DVD+R\"\n");
	fprintf(fp,"		elif [ \"$(echo \"$XORRISO_INFO\" | grep -c 'Media current: DVD-RW')\" = \"1\" ]\n");
	fprintf(fp,"		then\n");
	fprintf(fp,"			MEDIA=\"DVD-RW\"\n");
	fprintf(fp,"		elif [ \"$(echo \"$XORRISO_INFO\" | grep -c 'Media current: DVD-R')\" = \"1\" ]\n");
	fprintf(fp,"		then\n");
	fprintf(fp,"			MEDIA=\"DVD-R\"\n");
	fprintf(fp,"		else\n");
	fprintf(fp,"			MEDIA=\"UNKNOWN\"\n");
	fprintf(fp,"			echo \"Could Not Detect Optical Media Type.  Exiting...\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"			exit 1\n");
	fprintf(fp,"		fi\n");
	fprintf(fp,"\n");
	fprintf(fp,"		if [ \"$MEDIA_WRITTEN_CLOSED\" = \"1\" ]\n");
	fprintf(fp,"		then\n");
	fprintf(fp,"			if [ \"$MEDIA\" = \"CD-R\" -o \"$MEDIA\" = \"DVD+R\" -o \"$MEDIA\" = \"DVD-R\" ]\n");
	fprintf(fp,"			then\n");
	fprintf(fp,"				echo \"$MEDIA Media In Drive Is Not Writable.  Exiting...\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"			exit 1\n");
	fprintf(fp,"		fi\n");
	fprintf(fp,"\n");
	fprintf(fp,"		elif [ \"$MEDIA_ERR_UNSUITABLE=\" = \"1\" ]\n");
	fprintf(fp,"		then\n");
	fprintf(fp,"			if [ \"$MEDIA\" = \"CD-R\" -o \"$MEDIA\" = \"DVD+R\" -o \"$MEDIA\" = \"DVD-R\" ]\n");
	fprintf(fp,"			then\n");
	fprintf(fp,"				echo \"$MEDIA Media In Drive Is Not Suitable for Writing.  Exiting...\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"			exit 1\n");
	fprintf(fp,"		fi\n");
	fprintf(fp,"		fi\n");
	fprintf(fp,"\n");
	fprintf(fp,"		echo \"MEDIA_ERR_UNSUITABLE=$MEDIA_ERR_UNSUITABLE\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"		echo \"MEDIA_ERR_RECOGNIZABLE=$MEDIA_ERR_RECOGNIZABLE\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"		echo \"MEDIA_ERR_NOT_PRESENT=$MEDIA_ERR_NOT_PRESENT\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"		echo \"MEDIA_WRITTEN_CLOSED=$MEDIA_WRITTEN_CLOSED\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"		echo \"MEDIA_TYPE=$MEDIA\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"		echo \"\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"		echo \"-------------------------------------------------------------------\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"	fi\n");
	fprintf(fp,"\n");
	fprintf(fp,"	TOTAL_CPUS=`cat /proc/cpuinfo | grep -c 'processor'`\n");
	fprintf(fp,"	TOTAL_MEM=`cat /proc/meminfo | grep 'MemTotal:' | awk '{print$2}'`\n");
	fprintf(fp,"\n");
	fprintf(fp,"	rm -fv \"$FILENAME\"\".7z\" >>$DATA_DIR/rdacroncd.log 2>&1\n");
	fprintf(fp,"	if [ $TOTAL_CPUS -ge 8 -a $TOTAL_MEM -gt $((8*1000*1000)) ]\n");
	fprintf(fp,"	then\n");
	fprintf(fp,"		7za.lnx a -m0=lzma2 -mx=9 -mmt=8 -t7z -bd -pb3a1fe7e9d9 \"$FILENAME\"\".7z\" . >>$DATA_DIR/rdacroncd.log 2>&1\n");
	fprintf(fp,"	elif [ $TOTAL_CPUS -ge 4 -a $TOTAL_MEM -gt $((4*1000*1000)) ]\n");
	fprintf(fp,"	then\n");
	fprintf(fp,"		7za.lnx a -m0=lzma2 -mx=9 -mmt=4 -t7z -bd -pb3a1fe7e9d9 \"$FILENAME\"\".7z\" . >>$DATA_DIR/rdacroncd.log 2>&1\n");
	fprintf(fp,"	else\n");
	fprintf(fp,"		7za.lnx a -t7z -bd -mmt=on -pb3a1fe7e9d9 \"$FILENAME\"\".7z\" . >>$DATA_DIR/rdacroncd.log 2>&1\n");
	fprintf(fp,"	fi\n");
	fprintf(fp,"	ZIP_ERROR=\"$?\"\n");
	fprintf(fp,"	if [ \"$ZIP_ERROR\" == 0 ]\n");
	fprintf(fp,"	then\n");
	fprintf(fp,"		echo \"Zip File Backup Created With 0 Errors.\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"	else\n");
	fprintf(fp,"		echo \"Zip File Backup Created But Exited With Error Code: $ZIP_ERROR\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"	fi\n");
	fprintf(fp,"\n");
	fprintf(fp,"	md5sum ./\"$FILENAME\"\".7z\" | awk '{print $1}' >\"$FILENAME\"\".md5\" \n");
	fprintf(fp,"\n");
	fprintf(fp,"	DATEFILE=`date +BACKUP-%%m-%%d-%%Y.txt`\n");
	fprintf(fp,"	date | sed -e '1,$s/$//' >\"$DATEFILE\"\n");
	fprintf(fp,"\n");
	fprintf(fp,"	if [ \"$BLANK_CDRW\" == \"TRUE\" ]\n");
	fprintf(fp,"	then\n");
	fprintf(fp,"		echo \"-------------------------------------------------------------------\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"		if [ -x /rda/xpgms/xorriso.lnx ]\n");
	fprintf(fp,"		then\n");
	fprintf(fp,"			echo \"Blanking $MEDIA Media....\" >>$DATA_DIR/rdacroncd.log \n");
	fprintf(fp,"			xorriso.lnx --outdev $BUS -blank all -- >>$DATA_DIR/rdacroncd.log 2>&1 \n");
	fprintf(fp,"			echo \"Finished Blanking $MEDIA Media.\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"		else\n");
	fprintf(fp,"			echo \"Blanking CDRW Media....\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"			cdrecord dev=$BUS blank=fast 2>&1 >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"			echo \"Finished Blanking CDRW Media.\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"		fi\n");
	fprintf(fp,"		echo \"\"\n");
	fprintf(fp,"		echo \"\"\n");
	fprintf(fp,"		echo \"-------------------------------------------------------------------\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"	fi\n");
	fprintf(fp,"	\n");
	fprintf(fp,"	if [ -x /rda/xpgms/xorriso.lnx ]\n");
	fprintf(fp,"	then\n");
	fprintf(fp,"		sleep 2\n");
	fprintf(fp,"		VOLID=`date +OPENRDA_%%Y%%m%%d`\n");
	fprintf(fp,"		if [ \"${MEDIA:0:2}\" = \"CD\" ]\n");
	fprintf(fp,"		then\n");
	fprintf(fp,"			xorriso.lnx -md5 all -joliet on -close on -fs 50m -speed \"$SPEED\"c -volid \"$VOLID\" -outdev $BUS -blank as_needed -add \"$FILENAME\"\".7z\" $DATEFILE \"$FILENAME\"\".md5\" -- >>$DATA_DIR/rdacroncd.log 2>&1\n");
	fprintf(fp,"		elif [ \"${MEDIA:0:3}\" = \"DVD\" ]\n");
	fprintf(fp,"		then\n");
	fprintf(fp,"			xorriso.lnx -md5 all -joliet on -close on -fs 50m -speed \"$SPEED\"d -volid \"$VOLID\" -outdev $BUS -blank as_needed -add \"$FILENAME\"\".7z\" $DATEFILE \"$FILENAME\"\".md5\" -- >>$DATA_DIR/rdacroncd.log 2>&1\n");
	fprintf(fp,"		else\n");
	fprintf(fp,"			xorriso.lnx -md5 all -joliet on -close on -fs 50m -volid \"$VOLID\" -outdev $BUS -blank as_needed -add \"$FILENAME\"\".7z\" $DATEFILE \"$FILENAME\"\".md5\" -- >>$DATA_DIR/rdacroncd.log 2>&1\n");
	fprintf(fp,"		fi\n");
	fprintf(fp,"	else\n");
	fprintf(fp,"		sleep 10\n");
	fprintf(fp,"		mkisofs -R -J -v -l ./\"$FILENAME\"\".7z\" ./\"$DATEFILE\" ./\"$FILENAME\"\".md5\" | cdrecord -v speed=$SPEED dev=$BUS - 2>/tmp/cdrecord.ERR | tee -a rdacroncd.log\n");
	fprintf(fp,"	fi\n");
	fprintf(fp,"	sleep 5\n");
	fprintf(fp,"	Verify_Cdrom\n");
	fprintf(fp,"\n");
	fprintf(fp,"else\n");
	fprintf(fp,"	echo \"The Data Directory Specified Does Not Exist or Is Not Accessible\" >>$DATA_DIR/rdacroncd.log\n");
	fprintf(fp,"	exit 1\n");
	fprintf(fp,"fi\n");

	fclose(fp);
#ifndef WIN32
	fd=chmod(temp,(mode_t)MODE_RWXRWX___);
	if(fd==(-1))
	{
		prterr("Error:  Couldn't set Mode for [%s].",(temp!=NULL ? temp:""));
	}
#endif
	if(temp!=NULL) Rfree(temp);
}
static void SCRIPT_rdacrontape_lnx(char *tape_dev)
{
	FILE *fp=NULL;
	char *temp=NULL;
#ifndef WIN32
	int fd=0;
#endif

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("rdacrontape.lnx")+15);
#ifdef WIN32
	sprintf(temp,"%s\\rdacrontape.lnx",CURRENTDIRECTORY);
#endif
#ifndef WIN32
	sprintf(temp,"%s/rdacrontape.lnx",CURRENTDIRECTORY);
#endif
	unlink(temp);
	fp=fopen(temp,"w+b");

	fprintf(fp,"## Script that runs via cron and makes a MBGUI data backup tape.\n");
	fprintf(fp,"## These need to be filled in when the script is created\n");
	fprintf(fp,"DATA_DIR=%s\n",CURRENTDIRECTORY);
	fprintf(fp,"cd $DATA_DIR\n");
	fprintf(fp,"echo \"====================================================\" >./rdacrontape.log\n");
	fprintf(fp,"echo \"Tape Backup\" >>./rdacrontape.log\n");
	fprintf(fp,"echo \"Date: `date`\" >>./rdacrontape.log\n");
	fprintf(fp,"echo \"Output Diagnostics:\" >>./rdacrontape.log\n");
	fprintf(fp,"echo \"====================================================\" >>./rdacrontape.log\n");
	fprintf(fp,"echo \"\" >>./rdacrontape.log \n");
	fprintf(fp,"find . -print | afio -ov %s 2>&1 | tee -a rdacrontape.log\n",(tape_dev!=NULL ? tape_dev:""));
	fclose(fp);
#ifndef WIN32
	fd=chmod(temp,(mode_t)MODE_RWXRWX___);
	if(fd==(-1))
	{
		prterr("Error:  Couldn't set Mode for [%s].",(temp!=NULL ? temp:""));
	}
#endif
	if(temp!=NULL) Rfree(temp);
}
void docrontab()
{
	char *libx=NULL,*cron_tape=NULL,*cdrw_bus=NULL;
	RDAGenericSetup *gsv=NULL;
	short cdrw_speed=1, blank_cdrw=0;


	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("UTILITIES")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif

	gsv=RDAGenericSetupNew("UTILITIES","CRON TAPE DEVICE");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			cron_tape=stralloc(gsv->value.string_value);
		} else {
			cron_tape=NULL;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("UTILITIES","CDRW BUS");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			cdrw_bus=stralloc(gsv->value.string_value);
		} else {
			cdrw_bus=NULL;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("UTILITIES","CDRW SPEED");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			cdrw_speed=*gsv->value.short_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("UTILITIES","CDRW DEFAULT BLANK");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			blank_cdrw=*gsv->value.short_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) FreeRDAGenericSetup(gsv);
	SCRIPT_rdacroncd_lnx(cdrw_bus,cdrw_speed,blank_cdrw);
	SCRIPT_rdacrontape_lnx(cron_tape);
	MNU_UTILITIES_DOCRONTAB();
	SCN_ADD_CRONTAB();
	SCN_MANAGE_CRONTABS();
	if(cron_tape!=NULL) Rfree(cron_tape);
	if(cdrw_bus!=NULL) Rfree(cdrw_bus);
}
