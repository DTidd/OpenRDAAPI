/* DATABASE's - Make (BACKUP DATA) Screen */
#include <mkmsc.hpp>

extern char *module;
extern APPlib *errorlist;
#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)
#define MODE_RWXRWX___	(S_IRWXU|S_IRWXG)
#endif /* Not LINUX && Not LINUX2_2 && Not UBUNTU_OS */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___	(00770)
#endif /* LINUX || LINUX2_2 || UBUNTU_OS*/

static char *cdrw_bus=NULL,cdrw_blank=FALSE;
static short cdrw_speed=1;

static void BACKUPSCRIPT()
{
	FILE *fp;
	char *name=NULL;
	int fd=(-1);
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
	char *libx=NULL,*cdrw_bus=NULL,blankit=FALSE;
	RDAGenericSetup *gsv=NULL;
	short cdrw_speed=1;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("UTILITIES")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif
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
			blankit=*gsv->value.short_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) FreeRDAGenericSetup(gsv);
#endif

	name=Rmalloc(12);
#ifdef WIN32
	sprintf(name,"backup.bat");
#endif
#ifdef RS6000
	sprintf(name,"backup.rsc");
#endif
#ifdef SCO50
	sprintf(name,"backup.sco");
#endif
#ifdef SCO50P
	sprintf(name,"backup.sco");
#endif
#ifdef UNIXWARE21
	sprintf(name,"backup.unw");
#endif
#ifdef UNIXWARE7
	sprintf(name,"backup.unw");
#endif
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
	sprintf(name,"backup.lnx");
#endif
#ifdef SUNX86
	sprintf(name,"backup.sun");
#endif
	unlink(name);
	fp=fopen(name,"w+");
	if(fp!=NULL)
	{
#ifdef WIN32
		//fprintf(fp,"zip.exe -r mbgui *");
		fprintf(fp,"7zg.exe a -t7z -mmt=on -ssw %s -xr-!*.exe -xr-!*.dll .",(adddashes(XPERT_SETUP->company_name)));
#endif
#ifdef RS6000
		fprintf(fp,"#!/bin/sh\n");
		fprintf(fp,"rm backup.log\n");
		fprintf(fp,"rm restore.log\n");
		fprintf(fp,"echo \"Backing up to Device: $*\"\n");
		fprintf(fp,"find . -print | cpio -ocBv > $* 2>backup.log\n");
		fprintf(fp,"echo \"Your Backup is complete, would you like to view the log file?\"\n");
		fprintf(fp,"read response\n");
		fprintf(fp,"if [ \"$response\" = \"\" ]\n");
		fprintf(fp,"then\n");
		fprintf(fp,"\tresponse=\"y\"\n");
		fprintf(fp,"fi\n");
		fprintf(fp,"case $response in\n");
		fprintf(fp,"\ty*|Y*) rdaout.rsc ./backup.log ;;\n");
		fprintf(fp,"\t*);;\n");
		fprintf(fp,"esac\n");
#endif
#ifdef SCO50
		fprintf(fp,"#!/bin/sh\n");
		fprintf(fp,"rm backup.log\n");
		fprintf(fp,"rm restore.log\n");
		fprintf(fp,"echo \"Backing up to Device: $*\"\n");
		fprintf(fp,"find . -print | cpio -ocBv -O $* 2>&1 | tee backup.log\n");
		fprintf(fp,"echo \"Your Backup is complete, would you like to view the log file?\"\n");
		fprintf(fp,"read response\n");
		fprintf(fp,"if [ \"$response\" = \"\" ]\n");
		fprintf(fp,"then\n");
		fprintf(fp,"\tresponse=\"y\"\n");
		fprintf(fp,"fi\n");
		fprintf(fp,"case $response in\n");
		fprintf(fp,"\ty*|Y*) rdaout.unw ./backup.log ;;\n");
		fprintf(fp,"\t*);;\n");
		fprintf(fp,"esac\n");
#endif
#ifdef SCO50P
		fprintf(fp,"#!/bin/sh\n");
		fprintf(fp,"rm backup.log\n");
		fprintf(fp,"rm restore.log\n");
		fprintf(fp,"echo \"Backing up to Device: $*\"\n");
		fprintf(fp,"find . -print | cpio -ocBv -O $* 2>&1 | tee backup.log\n");
		fprintf(fp,"echo \"Your Backup is complete, would you like to view the log file?\"\n");
		fprintf(fp,"read response\n");
		fprintf(fp,"if [ \"$response\" = \"\" ]\n");
		fprintf(fp,"then\n");
		fprintf(fp,"\tresponse=\"y\"\n");
		fprintf(fp,"fi\n");
		fprintf(fp,"case $response in\n");
		fprintf(fp,"\ty*|Y*) rdaout.unw ./backup.log ;;\n");
		fprintf(fp,"\t*);;\n");
		fprintf(fp,"esac\n");
#endif
#ifdef UNIXWARE21
		fprintf(fp,"#!/bin/sh\n");
		fprintf(fp,"rm backup.log\n");
		fprintf(fp,"rm restore.log\n");
		fprintf(fp,"echo \"Backing up to Device: $*\"\n");
		fprintf(fp,"find . -print | cpio -oBv -Hodc -O $* 2>&1 | tee backup.log\n");
		fprintf(fp,"echo \"Your Backup is complete, would you like to view the log file?\"\n");
		fprintf(fp,"read response\n");
		fprintf(fp,"if [ \"$response\" = \"\" ]\n");
		fprintf(fp,"then\n");
		fprintf(fp,"\tresponse=\"y\"\n");
		fprintf(fp,"fi\n");
		fprintf(fp,"case $response in\n");
		fprintf(fp,"\ty*|Y*) rdaout.unw ./backup.log ;;\n");
		fprintf(fp,"\t*);;\n");
		fprintf(fp,"esac\n");
#endif
#ifdef UNIXWARE7
		fprintf(fp,"#!/bin/sh\n");
		fprintf(fp,"rm backup.log\n");
		fprintf(fp,"rm restore.log\n");
		fprintf(fp,"echo \"Backing up to Device: $*\"\n");
		fprintf(fp,"find . -print | cpio -ocBv -O $* 2>&1 | tee backup.log\n");
		fprintf(fp,"echo \"Your Backup is complete, would you like to view the log file?\"\n");
		fprintf(fp,"read response\n");
		fprintf(fp,"if [ \"$response\" = \"\" ]\n");
		fprintf(fp,"then\n");
		fprintf(fp,"\tresponse=\"y\"\n");
		fprintf(fp,"fi\n");
		fprintf(fp,"case $response in\n");
		fprintf(fp,"\ty*|Y*) rdaout.unw ./backup.log ;;\n");
		fprintf(fp,"\t*);;\n");
		fprintf(fp,"esac\n");
#endif
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
		fprintf(fp,"#!/bin/bash\n");
		fprintf(fp,"## Backup script supporting CD-R/CD-RW/DVD-R/DVD-RW/DVD+R/DVD+RW Media.\n");
		fprintf(fp,"DisplayResults()\n");
		fprintf(fp,"{\n");
		fprintf(fp,"	echo \"\"\n");
		fprintf(fp,"	rdaout.lnx ./backup.log\n");
		fprintf(fp,"}\n");
		fprintf(fp,"Finished()\n");
		fprintf(fp,"{\n");
		fprintf(fp,"	rm -f /tmp/cdrecord.ERR ./$DATEFILE ./\"$FILENAME\"\".md5\"\n");
		fprintf(fp,"	exit 0\n");
		fprintf(fp,"}\n");
		fprintf(fp,"Verify_Cdrom()\n");
		fprintf(fp,"{\n");
		fprintf(fp," 	echo \"Verifying CD Contents Now....\" | tee -a ./backup.log\n");
		fprintf(fp,"	sleep 10\n");
		fprintf(fp,"	if [ -x /rda/xpgms/xorriso.lnx ]\n");
		fprintf(fp,"	then\n");
		fprintf(fp,"		xorriso.lnx -md5 all -indev $BUS -check_md5_r sorry / --	\n");
		fprintf(fp,"		VERIFY_STATUS=$?\n");
		fprintf(fp,"\n");
		fprintf(fp,"		xorriso.lnx --outdev $BUS --eject all -- 2>&1\n");
		fprintf(fp,"\n");
		fprintf(fp,"		if [ $VERIFY_STATUS == 0 ]\n");
		fprintf(fp,"		then\n");
		fprintf(fp,"			echo \"\" | tee -a ./backup.log\n");
		fprintf(fp,"			echo \"Backup Created Successfully!\" | tee -a ./backup.log\n");
		fprintf(fp,"		else\n");
		fprintf(fp,"			echo \"\" | tee -a ./backup.log\n");
		fprintf(fp,"			echo \"ERROR:  BACKUP FAILED.  Failed To Verify Backup Disc.  Your Data Was Not Backed Up.\" | tee -a ./backup.log\n");
		fprintf(fp,"			ok.lnx ERRORDIALOG \"BACKUP DATA\"  \"ERROR:  BACKUP FAILED.  Failed To Verify Backup Disc.  Your Data Was Not Backed Up.\"\n");
		fprintf(fp,"		fi	\n");
		fprintf(fp,"	else\n");
		fprintf(fp,"		N=0\n");
		fprintf(fp,"		until [ $N == 15 ]\n");
		fprintf(fp,"		do\n");
		fprintf(fp,"			((++N))\n");
		fprintf(fp,"			mount /cdrom\n");
		fprintf(fp,"			sleep 2\n");
		fprintf(fp,"			if [ -f /cdrom/\"$FILENAME\".md5 ]\n");
		fprintf(fp,"			then \n");
		fprintf(fp,"				echo \"CDROM Read on try $N.\"\n");
		fprintf(fp,"				break\n");
		fprintf(fp,"			fi\n");
		fprintf(fp,"		done\n");
		fprintf(fp,"		\n");
		fprintf(fp,"		sleep 10\n");
		fprintf(fp,"		ORIG_MD5=`cat ./\"$FILENAME\"\".md5\"`\n");
		fprintf(fp,"		BACKUP_MD5=`md5sum /cdrom/\"$FILENAME\"\".7z\" | awk '{print $1}'`\n");
		fprintf(fp," \n");
		fprintf(fp,"		if [ \"$ORIG_MD5\" == \"$BACKUP_MD5\" ]\n");
		fprintf(fp,"		then\n");
		fprintf(fp,"			echo \"MD5SUMS Match\"\n");
		fprintf(fp,"		else\n");
		fprintf(fp,"			echo \"MD5SUMS Don't Match\"\n");
		fprintf(fp,"		fi\n");
		fprintf(fp,"\n");
		fprintf(fp,"		if [ -f /cdrom/$DATEFILE -a -f /cdrom/\"$FILENAME\"\".7z\" -a \"$ORIG_MD5\" == \"$BACKUP_MD5\" ]\n");
		fprintf(fp,"		then\n");
		fprintf(fp,"			echo \"\" >>./backup.log\n");
		fprintf(fp,"			echo \"Backup Created Successfully!\" >>./backup.log\n");
		fprintf(fp,"			else\n");
		fprintf(fp,"			echo \"\" >>./backup.log\n");
		fprintf(fp,"			cat /tmp/cdrecord.ERR >>./backup.log\n");
		fprintf(fp,"			echo \"\" >>./backup.log\n");
		fprintf(fp,"			echo \"ERROR:  Could not Verify CDROM Backup.  Your Backup may not have worked!\" | tee -a ./backup.log\n");
		fprintf(fp,"		fi\n");
		fprintf(fp,"		export LANG=C\n");
		fprintf(fp,"		./fixcdrecordlog.lnx backup.log >backup2.log\n");
		fprintf(fp,"		mv backup2.log backup.log\n");
		fprintf(fp,"		umount /cdrom\n");
		fprintf(fp,"		eject /cdrom\n");
		fprintf(fp,"	fi\n");
		fprintf(fp,"}\n");
		fprintf(fp,"Tape_Backup()\n");
		fprintf(fp,"{\n");
		fprintf(fp,"	echo \"====================================================\" >>./backup.log\n");
		fprintf(fp,"	echo \"Tape Backup\" >>./backup.log\n");
		fprintf(fp,"	echo \"Date: `date`\" >>./backup.log\n");
		fprintf(fp,"	echo \"Output Diagnostics:\" >>./backup.log\n");
		fprintf(fp,"	echo \"Backup Device: $DEVICE\" >>./backup.log\n");
		fprintf(fp,"	echo \"\" >>./backup.log \n");
		fprintf(fp,"	find . -print | afio -ov $DEVICE 2>&1 | tee -a backup.log\n");
		fprintf(fp,"	DisplayResults\n");
		fprintf(fp,"	Finished\n");
		fprintf(fp,"} \n");
		fprintf(fp,"Cdrom_Backup()\n");
		fprintf(fp,"{\n");
		fprintf(fp,"	DATEFILE=`date +BACKUP-%%m-%%d-%%Y.txt`\n");
		fprintf(fp,"	date | sed -e '1,$s/$/\015/' >$DATEFILE\n");
		fprintf(fp,"	echo \"-------------------------------------------------------------------\" >./backup.log\n");
		fprintf(fp,"	echo \"CDROM Backup\" >>./backup.log\n");
		fprintf(fp,"	echo \"Date: `date`\" >>./backup.log\n");
		fprintf(fp,"	echo \"Output Diagnostics:\" >>./backup.log\n");
		fprintf(fp,"	echo \"-------------------------------------------------------------------\" >>./backup.log\n");
		fprintf(fp,"	echo \"\" >>./backup.log\n");
		fprintf(fp,"\n");
		fprintf(fp,"	if [ -x /rda/xpgms/xorriso.lnx ]\n");
		fprintf(fp,"	then\n");
		fprintf(fp,"		if [ \"${BUS:0:5}\" != \"/dev/\" ]\n");
		fprintf(fp,"		then\n");
		fprintf(fp,"			AUTODEV=`xorriso.lnx --devices 2>/dev/null| grep '0  -dev' | awk '{print $3}'`\n");
		fprintf(fp,"			AUTODEV=\"${AUTODEV//\\\"/}\"\n");
		fprintf(fp,"			if [ \"${AUTODEV:0:5}\" = \"/dev/\" ]\n");
		fprintf(fp,"			then\n");
		fprintf(fp,"				BUS=\"$AUTODEV\"\n");
		fprintf(fp,"			else\n");
		fprintf(fp,"				echo \"Invalid Device Specified, Could Not Auto Detect Correct Device.  Exiting...\" | tee -a ./backup.log\n");
		fprintf(fp,"				ok.lnx ERRORDIALOG \"BACKUP DATA\"  \"Invalid Device Specified, Could Not Auto Detect Correct Device.  Exiting...\"\n");
		fprintf(fp,"				exit 1\n");
		fprintf(fp,"			fi\n");
		fprintf(fp,"		fi\n");
		fprintf(fp,"		\n");
		fprintf(fp,"		echo \"Detecting Drive Status...\"\n");
		fprintf(fp,"		XORRISO_INFO=$(xorriso.lnx --outdev $BUS -- 2>&1)\n");
		fprintf(fp,"		echo \"$XORRISO_INFO\" >>./backup.log\n");
		fprintf(fp,"		MEDIA_ERR_UNSUITABLE=$(echo \"$XORRISO_INFO\" | grep -c 'Disc status unsuitable for writing')\n");
		fprintf(fp,"		MEDIA_ERR_RECOGNIZABLE=$(echo \"$XORRISO_INFO\" | grep -c 'Media current: is not recognizable')\n");
		fprintf(fp,"		MEDIA_ERR_NOT_PRESENT=$(echo \"$XORRISO_INFO\" | grep -c 'Media status : is not present')\n");
		fprintf(fp,"		MEDIA_WRITTEN_CLOSED=$(echo \"$XORRISO_INFO\" | grep -c 'Media status : is written , is closed')\n");
		fprintf(fp,"\n");
		fprintf(fp,"		if [ \"$MEDIA_ERR_NOT_PRESENT\" = \"1\" ]\n");
		fprintf(fp,"		then\n");
		fprintf(fp,"			echo \"No Optical Media Found In Drive.  Exiting...\" | tee -a ./backup.log\n");
		fprintf(fp,"			ok.lnx ERRORDIALOG \"BACKUP DATA\"  \"No Optical Media Found In Drive.  Exiting...\"\n");
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
		fprintf(fp,"			echo \"Could Not Detect Optical Media Type.  Exiting...\" | tee -a ./backup.log\n");
		fprintf(fp,"			ok.lnx ERRORDIALOG \"BACKUP DATA\"  \"Could Not Detect Optical Media Type.  Exiting...\"\n");
		fprintf(fp,"			exit 1\n");
		fprintf(fp,"		fi\n");
		fprintf(fp,"\n");
		fprintf(fp,"		if [ \"$MEDIA_WRITTEN_CLOSED\" = \"1\" ]\n");
		fprintf(fp,"		then\n");
		fprintf(fp,"			if [ \"$MEDIA\" = \"CD-R\" -o \"$MEDIA\" = \"DVD+R\" -o \"$MEDIA\" = \"DVD-R\" ]\n");
		fprintf(fp,"			then\n");
		fprintf(fp,"				echo \"$MEDIA Media In Drive Is Not Writable.  Exiting...\" | tee -a ./backup.log\n");
		fprintf(fp,"				ok.lnx ERRORDIALOG \"BACKUP DATA\"  \"$MEDIA Media In Drive Is Not Writable.  Exiting...\"\n");
		fprintf(fp,"			exit 1\n");
		fprintf(fp,"		fi\n");
		fprintf(fp,"\n");
		fprintf(fp,"		elif [ \"$MEDIA_ERR_UNSUITABLE=\" = \"1\" ]\n");
		fprintf(fp,"		then\n");
		fprintf(fp,"			if [ \"$MEDIA\" = \"CD-R\" -o \"$MEDIA\" = \"DVD+R\" -o \"$MEDIA\" = \"DVD-R\" ]\n");
		fprintf(fp,"			then\n");
		fprintf(fp,"				echo \"$MEDIA Media In Drive Is Not Suitable for Writing.  Exiting...\" | tee -a ./backup.log\n");
		fprintf(fp,"				ok.lnx ERRORDIALOG \"BACKUP DATA\"  \"$MEDIA Media In Drive Is Not Suitable for Writing.  Exiting...\"\n");
		fprintf(fp,"			exit 1\n");
		fprintf(fp,"		fi\n");
		fprintf(fp,"		fi\n");
		fprintf(fp,"\n");
		fprintf(fp,"		echo \"MEDIA_ERR_UNSUITABLE=$MEDIA_ERR_UNSUITABLE\"\n");
		fprintf(fp,"		echo \"MEDIA_ERR_RECOGNIZABLE=$MEDIA_ERR_RECOGNIZABLE\"\n");
		fprintf(fp,"		echo \"MEDIA_ERR_NOT_PRESENT=$MEDIA_ERR_NOT_PRESENT\"\n");
		fprintf(fp,"		echo \"MEDIA_WRITTEN_CLOSED=$MEDIA_WRITTEN_CLOSED\"\n");
		fprintf(fp,"		echo \"MEDIA_TYPE=$MEDIA\"\n");
		fprintf(fp,"		echo \"\"\n");
		fprintf(fp,"		echo \"-------------------------------------------------------------------\" >>./backup.log\n");
		fprintf(fp,"	fi\n");
		fprintf(fp,"\n");
		fprintf(fp,"	TOTAL_CPUS=`cat /proc/cpuinfo | grep -c 'processor'`\n");
		fprintf(fp,"	TOTAL_MEM=`cat /proc/meminfo | grep 'MemTotal:' | awk '{print$2}'`\n");
		fprintf(fp,"\n");
		fprintf(fp,"	rm -fv \"$FILENAME\"\".7z\"\n");
		fprintf(fp,"	if [ $TOTAL_CPUS -ge 8 -a $TOTAL_MEM -gt $((8*1000*1000)) ]\n");
		fprintf(fp,"	then\n");
		fprintf(fp,"		7za.lnx a -m0=lzma2 -mx=9 -mmt=8 -t7z -bd -pb3a1fe7e9d9 \"$FILENAME\"\".7z\" . | tee -a ./backup.log\n");
		fprintf(fp,"	elif [ $TOTAL_CPUS -ge 4 -a $TOTAL_MEM -gt $((4*1000*1000)) ]\n");
		fprintf(fp,"	then\n");
		fprintf(fp,"		7za.lnx a -m0=lzma2 -mx=9 -mmt=4 -t7z -bd -pb3a1fe7e9d9 \"$FILENAME\"\".7z\" . | tee -a ./backup.log\n");
		fprintf(fp,"	else\n");
		fprintf(fp,"		7za.lnx a -t7z -bd -mmt=on -pb3a1fe7e9d9 \"$FILENAME\"\".7z\" . | tee -a ./backup.log\n");
		fprintf(fp,"	fi\n");
		fprintf(fp,"\n");
		fprintf(fp,"	md5sum ./\"$FILENAME\"\".7z\" | awk '{print $1}' >\"$FILENAME\"\".md5\"\n");
		fprintf(fp,"	echo \"-------------------------------------------------------------------\" >>./backup.log\n");
		fprintf(fp,"\n");
		fprintf(fp,"\n");
		fprintf(fp,"	if [ \"$BLANK_CDROM\" = \"TRUE\" ]\n");
		fprintf(fp,"	then\n");
		fprintf(fp,"		if [ -x /rda/xpgms/xorriso.lnx ]\n");
		fprintf(fp,"		then\n");
		fprintf(fp,"			echo \"Blanking $MEDIA Media....\" | tee -a ./backup.log\n");
		fprintf(fp,"			xorriso.lnx --outdev $BUS -blank all -- 2>&1 | tee -a ./backup.log\n");
		fprintf(fp,"			echo \"Finished Blanking $MEDIA Media.\" | tee -a ./backup.log\n");
		fprintf(fp,"		else\n");
		fprintf(fp,"			echo \"Blanking CDRW Media....\" | tee -a ./backup.log\n");
		fprintf(fp,"			cdrecord blank=fast dev=$BUS 2>&1 | tee -a ./backup.log\n");
		fprintf(fp,"			echo \"Finished Blanking CDRW Media.\" | tee -a ./backup.log\n");
		fprintf(fp,"		fi\n");
		fprintf(fp,"		echo \"-------------------------------------------------------------------\" >>./backup.log\n");
		fprintf(fp,"	fi\n");
		fprintf(fp,"	echo \"Waiting on Burn Process to start....\"\n");
		fprintf(fp,"	\n");
		fprintf(fp,"	if [ -x /rda/xpgms/xorriso.lnx ]\n");
		fprintf(fp,"	then\n");
		fprintf(fp,"		sleep 2\n");
		fprintf(fp,"		VOLID=`date +OPENRDA_%%Y%%m%%d`\n");
		fprintf(fp,"		if [ \"${MEDIA:0:2}\" = \"CD\" ]\n");
		fprintf(fp,"		then\n");
		fprintf(fp,"			xorriso.lnx -md5 all -joliet on -close on -fs 50m -speed \"$SPEED\"c -volid \"$VOLID\" -outdev $BUS -blank as_needed -add \"$FILENAME\"\".7z\" $DATEFILE \"$FILENAME\"\".md5\" -- 2>&1 | tee -a ./backup.log\n");
		fprintf(fp,"		elif [ \"${MEDIA:0:3}\" = \"DVD\" ]\n");
		fprintf(fp,"		then\n");
		fprintf(fp,"			xorriso.lnx -md5 all -joliet on -close on -fs 50m -speed \"$SPEED\"d -volid \"$VOLID\" -outdev $BUS -blank as_needed -add \"$FILENAME\"\".7z\" $DATEFILE \"$FILENAME\"\".md5\" -- 2>&1 | tee -a ./backup.log\n");
		fprintf(fp,"		else\n");
		fprintf(fp,"			xorriso.lnx -md5 all -joliet on -close on -fs 50m -volid \"$VOLID\" -outdev $BUS -blank as_needed -add \"$FILENAME\"\".7z\" $DATEFILE \"$FILENAME\"\".md5\" -- 2>&1 | tee -a ./backup.log\n");
		fprintf(fp,"		fi\n");
		fprintf(fp,"	else\n");
		fprintf(fp,"		sleep 10\n");
		fprintf(fp,"		mkisofs -R -J -v -l ./\"$FILENAME\"\".7z\" ./$DATEFILE ./\"$FILENAME\"\".md5\" | cdrecord -v speed=$SPEED dev=$BUS - 2>/tmp/cdrecord.ERR | tee -a ./backup.log\n");
		fprintf(fp,"	fi\n");
		fprintf(fp,"	echo \"-------------------------------------------------------------------\" >>./backup.log\n");
		fprintf(fp,"	Verify_Cdrom $@\n");
		fprintf(fp,"	DisplayResults\n");
		fprintf(fp,"	Finished\n");
		fprintf(fp,"}\n");
		fprintf(fp,"if [ -f backup.log ]\n");
		fprintf(fp,"then\n");
		fprintf(fp,"	rm -fv backup.log\n");
		fprintf(fp,"fi\n");
		fprintf(fp,"\n");
		fprintf(fp,"if [ $# = \"2\" ]\n");
		fprintf(fp,"then\n");
		fprintf(fp,"	FILENAME=$1\n");
		fprintf(fp,"	if [ \"$2\" = \"cdrom\" ]\n");
		fprintf(fp,"	then\n");
		fprintf(fp,"		## Set the Defaults since none were specified.\n");
		fprintf(fp,"		## The make should set these with either the GSV values or :\n");
		fprintf(fp,"		## cdrom, 0,0,0 24  <-- Default values.\n");
		fprintf(fp,"		BUS=\"%s\"\n",(cdrw_bus!=NULL ? cdrw_bus:"0,0,0"));
		fprintf(fp,"		SPEED=\"%d\"\n",cdrw_speed);
		fprintf(fp,"		BLANK_CDROM=\"FALSE\"\n");
		fprintf(fp,"		DEVICE=\"cdrom\"\n");
		fprintf(fp,"	 	Cdrom_Backup \"$@\"\n");
		fprintf(fp,"	else\n");
		fprintf(fp,"		DEVICE=\"$2\"\n");
		fprintf(fp,"		Tape_Backup\n");
		fprintf(fp,"	fi\n");
		fprintf(fp,"elif [ $# -lt 2 ]\n");
		fprintf(fp,"then\n");
		fprintf(fp,"	echo \"ERROR: Device Specification cannot be BLANK.  Valid values are cdrom for CD Writers or Tape Device (i.e cdrom, /dev/ht0)\" >>./backup.ERR\n");
		fprintf(fp,"	rdaout.lnx backup.ERR\n");
		fprintf(fp,"else\n");
		fprintf(fp,"	FILENAME=\"$1\"\n");
		fprintf(fp,"	DEVICE=\"cdrom\"\n");
		fprintf(fp,"	BUS=\"$3\"\n");
		fprintf(fp,"	SPEED=\"$4\"\n");
		fprintf(fp,"	BLANK_CDROM=\"$5\"\n");
		fprintf(fp,"	Cdrom_Backup \"$@\"\n");
		fprintf(fp,"fi \n");
#endif
		fclose(fp);
#ifndef WIN32
		fd=chmod(name,(mode_t)MODE_RWXRWX___);
		if(fd==(-1))
		{
			prterr("Error:  Couldn't set Mode for [%s].",(name!=NULL ? name:""));
		}
#endif
	}
	if(name!=NULL) Rfree(name);
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
	if(cdrw_bus!=NULL) Rfree(cdrw_bus);
#endif
}
static void BCKPDATA()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("DATABASE","BACKUP DATA");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
#ifndef WIN32
		ADVaddwdgt(scrn,5,"","Drive Specification:","","",0,0,0,NULL,NULL,NULL,NULL);
#endif 
#ifdef WIN32
		ADVaddwdgt(scrn,5,"","Zip Filename:","","",0,0,0,NULL,NULL,NULL,NULL);
#endif
		ADVaddwdgt(scrn,0,"DRIVE SPEC","Drive Spec","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,9,"BLANK CDRW","Blank CDRW Disc","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,25,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DEFAULTS","Save Defaults","","",0,0,17,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(11+8+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [DATABASE] Screen [BACKUP DATA], Can Not Save Maintain Master!");
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
static void BACKUP_DATA_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("BACKUP DATA","dobackup.EXT");
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
void BACKUPDATA()
{
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("UTILITIES")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif

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
		} else {
			cdrw_speed=1;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("UTILITIES","CDRW DEFAULT BLANK");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			cdrw_blank=*gsv->value.string_value;
		} else {
			cdrw_blank=FALSE;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	BACKUPSCRIPT();
	BCKPDATA();
	BACKUP_DATA_MENU();
}
