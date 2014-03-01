//__pragma(optimize("", off))
/* ARCHIVE's - Make (ARCHIVES) Database File */
#include <cstdio>
#include <mix.hpp>
#include <mknrd.hpp>
#include <mkmsc.hpp>

extern char *module;
extern APPlib *errorlist;


void DB_ARCHPROG()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	sort=DBsortNEW("ARCHIVE","ARCHPROG",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"PROGRAM ID",1,15);
		addDBfield(sort,"PROGRAM NAME",1,40);
		addDBfield(sort,"EXECUTABLE NAME",1,80);
		addDBkey(sort,"ARCHPROG KEY");
		addDBkeypart(sort,1,"PROGRAM ID");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(8+7+77+1);
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [ARCHIVE] Screen [ARCHPROG], Can Not Save Database File!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
}
static void AddARCHPROG(short fileno,char *id,char *name,char *e)
{
	ZERNRD(fileno);
	FINDFLDSETSTRING(fileno,"PROGRAM ID",id);
	FINDFLDSETSTRING(fileno,"PROGRAM NAME",name);
	FINDFLDSETSTRING(fileno,"EXECUTABLE NAME",e);
	WRTNRD(fileno,0,NULL);
}
void MAKE_BACKUPFOLDER()
{
	short fileno=(-1);

	fileno=OPNNRD("ARCHIVE","ARCHWORK");
	if(fileno!=(-1))
	{
		ZERNRD(fileno);
		FINDFLDSETCHAR(fileno,"DELETEFLAG",FALSE);
		FINDFLDSETSTRING(fileno,"WORK GROUP","BACKUPS");
		if(XPERT_SETUP->software_type<2)
		{
			FINDFLDSETSTRING(fileno,"BACKUPS","XPERT BACKUPS");
		} else {
			FINDFLDSETSTRING(fileno,"BACKUPS","MBGUI BACKUPS");
		}
		FINDFLDSETSTRING(fileno,"USER IDENTIFICATION","RDA");
		FINDFLDSETSTRING(fileno,"DIRECTORY","BACKUPS");
		WRTNRD(fileno,0,NULL);
		CLSNRD(fileno);
	}
}
void MAKE_ARCHPROG()
{
	short fileno=(-1);
#ifdef WIN32
	int x=0,y=0;
	char myvalue[512];
#endif

	fileno=OPNNRD("ARCHIVE","ARCHPROG");
	if(fileno!=(-1))
	{
#ifdef WIN32
		AddARCHPROG(fileno,"gnuplot","GNUPLOT","gnuplot.exe");
		AddARCHPROG(fileno,"default","Windows Default Display Method","start");
		AddARCHPROG(fileno,"dohttp","Route to Default Browser","dohttp.exe");
		AddARCHPROG(fileno,"rdaout","RDA Text Browser","rdaout.exe");

		memset(myvalue,0,512);
		if(ReadRegKey("SOFTWARE\\Classes\\Applications\\swriter.exe\\shell\\open\\command",NULL,&myvalue))
		{
			prterr("Writer found using key 1.\n");
		}else if(ReadRegKey("SOFTWARE\\Classes\\opendocument.WriterDocument.1\\shell\\open\\command",NULL,&myvalue))
		{
			prterr("Writer found using key 2.\n");
		}else if(ReadRegKey("SOFTWARE\\Classes\\opendocument.WriterGlobalDocument.1\\shell\\open\\command",NULL,&myvalue))
		{
			prterr("Writer found using key 3.\n");
		}else if(ReadRegKey("SOFTWARE\\Classes\\OpenOffice.org.Doc\\shell\\open\\command",NULL,&myvalue))
		{
			prterr("Writer found using key 4.\n");
		}else{
			prterr("Writer Not found...\n");
		}
		//prterr("myvalue [%s].\n",myvalue);
		if((myvalue[0]=='"')&&((strstr(myvalue,".exe\"")!=NULL)||(strstr(myvalue,".EXE\"")!=NULL)))
		{
			for(x=0;x<512;x++)
			{
				
				if((myvalue[x]=='\\')&&(myvalue[(x+1)]=='\\'))
				{
					//prterr("Found Double backslashes: %d",x);
					for(y=x;y<(512-x);y++)
					{
						myvalue[y]=myvalue[(y+1)];
					}
				}
				if(myvalue[x]=='"')
				{
					//prterr("Found Double Quotes: %d",x);
					myvalue[x]='\0';
				}

			}
			strcpy(stemp,myvalue+1);
			//prterr("Program path: [%s]",stemp);
			AddARCHPROG(fileno,"swriter","OpenOffice / LibreOffice Writer",stemp);
			memset(myvalue,0,512);
			y=0;
			for(x=0;x<101;x++)
			{
				myvalue[x]=stemp[x];
				if(myvalue[x]=='\\')
				{
					y=x;	
				}
			}
			myvalue[(y+1)]='s';
			myvalue[(y+2)]='o';
			myvalue[(y+3)]='f';
			myvalue[(y+4)]='f';
			myvalue[(y+5)]='i';
			myvalue[(y+6)]='c';
			myvalue[(y+7)]='e';
			myvalue[(y+8)]='.';
			myvalue[(y+9)]='e';
			myvalue[(y+10)]='x';
			myvalue[(y+11)]='e';
			myvalue[(y+12)]='\0';

			AddARCHPROG(fileno,"soffice","OpenOffice / LibreOffice",myvalue);
		}

		memset(myvalue,0,512);
		memset(stemp,0,101);
		if(ReadRegKey("SOFTWARE\\Classes\\opendocument.CalcDocument.1\\shell\\open\\command",NULL,&myvalue))
		{
			prterr("Calc found using key 1.\n");
		}else if(ReadRegKey("SOFTWARE\\Classes\\opendocument.CalcTemplate.1\\shell\\open\\command",NULL,&myvalue))
		{
			prterr("Calc found using key 2.\n");
		}else if(ReadRegKey("SOFTWARE\\Classes\\OpenOffice.org.Xls\\shell\\new\\command",NULL,&myvalue))
		{
			prterr("Calc found using key 3.\n");
		}else{
			prterr("Calc Not found...\n");
		}
		//prterr("myvalue [%s].\n",myvalue);
		if((myvalue[0]=='"')&&((strstr(myvalue,".exe\"")!=NULL)||(strstr(myvalue,".EXE\"")!=NULL)))
		{
			for(x=0;x<512;x++)
			{
				
				if((myvalue[x]=='\\')&&(myvalue[(x+1)]=='\\'))
				{
					//prterr("Found Double backslashes: %d",x);
					for(y=x;y<(512-x);y++)
					{
						myvalue[y]=myvalue[(y+1)];
					}
				}
				if(myvalue[x]=='"')
				{
					//prterr("Found Double Quotes: %d",x);
					myvalue[x]='\0';
				}

			}
			strcpy(stemp,myvalue+1);
			//prterr("Program path: [%s]",stemp);
			AddARCHPROG(fileno,"scalc","OpenOffice / LibreOffice Calc",stemp);
		}


#endif
#ifndef WIN32
		AddARCHPROG(fileno,"dohttp","Route to Default Browser","dohttp.lnx");
		AddARCHPROG(fileno,"gnuplot","GNUPLOT","gnuplot");
		AddARCHPROG(fileno,"soffice","OpenOffice / Default","soffice.lnx");
		AddARCHPROG(fileno,"scalc","OpenOffice / Calc","scalc.lnx");
		AddARCHPROG(fileno,"mozilla","Mozilla Browser","/usr/bin/mozilla");
		AddARCHPROG(fileno,"firefox","Mozilla Firefox Browser","/usr/bin/firefox");
		AddARCHPROG(fileno,"netscape","Netscape Browser","/usr/bin/netscape");
		AddARCHPROG(fileno,"adobe acrobat","Adobe Acrobat Reader","/usr/bin/acroread");
		AddARCHPROG(fileno,"rdaout","RDA Text Browser","rdaout.lnx");
		AddARCHPROG(fileno,"ark","KDE Archiving Tool","/usr/bin/ark");
		AddARCHPROG(fileno,"gmplayer","Movie Player","/usr/bin/gmplayer");
		AddARCHPROG(fileno,"mplayer","Media Player","/usr/bin/mplayer");
#endif
		CLSNRD(fileno);
	}
}
void DB_ARCHIVES()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	sort=DBsortNEW("ARCHIVE","ARCHIVES",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"WORK GROUP",1,15);
		addDBfield(sort,"SUBJECT",1,15);
		addDBfield(sort,"CLASS",7,4);
		addDBfield(sort,"ARCHIVE NAME",1,60);
		addDBfield(sort,"TYPE",1,15);
		addDBfield(sort,"FILENAME",1,80);
		addDBfield(sort,"OCR NAME",1,80);
		addDBfield(sort,"DATE",16,10);
		addDBfield(sort,"TIME",17,8);
		addDBfield(sort,"SOURCE USER",1,15);
		addDBfield(sort,"SOURCE",7,1);
		addDBfield(sort,"DESCRIPTION",14,800);
		addDBfield(sort,"KEY WORDS",14,800);
		addDBkey(sort,"ARCHIVE KEY 1");
		addDBkeypart(sort,1,"WORK GROUP");
		addDBkeypart(sort,1,"ARCHIVE NAME");
		addDBkey(sort,"ARCHIVE KEY 2");
		addDBkeypart(sort,2,"ARCHIVE NAME");
		addDBkeypart(sort,2,"WORK GROUP");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(8+7+77+1);
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVES], Can Not Save Database File!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
}
void DB_ARCHTYPE()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	sort=DBsortNEW("ARCHIVE","ARCHTYPE",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"TYPE",1,15);
		addDBfield(sort,"DESCRIPTION",1,40);
		addDBfield(sort,"DISPLAYABLE",10,1);
		addDBfield(sort,"PROGRAM ID",1,15);
		addDBfield(sort,"LOCAL",10,1);
		addDBkey(sort,"ARCHTYPE KEY");
		addDBkeypart(sort,1,"TYPE");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(8+7+77+1);
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [ARCHIVE] Screen [ARCHTYPE], Can Not Save Database File!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
}
void DB_ARCHUSER()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	sort=DBsortNEW("ARCHIVE","ARCHUSER",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"WORK GROUP",1,15);
		addDBfield(sort,"USER IDENTIFICATION",1,15);
		addDBfield(sort,"READ",10,1);
		addDBfield(sort,"WRITE",10,1);
		addDBkey(sort,"ARCHUSER KEY 1");
		addDBkeypart(sort,1,"WORK GROUP");
		addDBkeypart(sort,1,"USER IDENTIFICATION");
		addDBkey(sort,"ARCHUSER KEY 2");
		addDBkeypart(sort,2,"USER IDENTIFICATION");
		addDBkeypart(sort,2,"WORK GROUP");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(8+7+77+1);
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [ARCHIVE] Screen [ARCHUSER], Can Not Save Database File!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
}
void DB_ARCHWORK()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	sort=DBsortNEW("ARCHIVE","ARCHWORK",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"WORK GROUP",1,15);
		addDBfield(sort,"USER IDENTIFICATION",1,15);
		addDBfield(sort,"NAME",1,40);
		addDBfield(sort,"DIRECTORY",1,80);
		addDBkey(sort,"ARCHWORK KEY 1");
		addDBkeypart(sort,1,"WORK GROUP");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(8+7+77+1);
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [ARCHIVE] Screen [ARCHWORK], Can Not Save Database File!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
}
void MakeARCHTYPE()
{
	short fileno=(-1);

	fileno=OPNNRD("ARCHIVE","ARCHTYPE");
	if(fileno!=(-1))
	{
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","lnx");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","Linux Executable");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",FALSE);
		FINDFLDSETSTRING(fileno,"PROGRAM ID","");
		WRTNRD(fileno,0,NULL);
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","exe");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","Microsoft Executable");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",FALSE);
		FINDFLDSETSTRING(fileno,"PROGRAM ID","");
		WRTNRD(fileno,0,NULL);
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","txt");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","Text File (ascii)");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
		FINDFLDSETCHAR(fileno,"LOCAL",FALSE);
		FINDFLDSETSTRING(fileno,"PROGRAM ID","rdaout");
		WRTNRD(fileno,0,NULL);
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","mp4");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","MP4");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
		FINDFLDSETCHAR(fileno,"LOCAL",FALSE);
#ifndef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","mplayer");
#endif
#ifdef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","dohttp");
#endif
		WRTNRD(fileno,0,NULL);
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","pdf");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","Adobe Portable Document Format");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
		FINDFLDSETCHAR(fileno,"LOCAL",FALSE);
#ifndef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","adobe acrobat");
#endif
#ifdef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","dohttp");
#endif
		WRTNRD(fileno,0,NULL);
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","zip");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","Zip Archive");
		FINDFLDSETCHAR(fileno,"LOCAL",FALSE);
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
#ifndef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","ark");
#endif
#ifdef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","winzip");
#endif
		WRTNRD(fileno,0,NULL);
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","tgz");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","Tarball");
		FINDFLDSETCHAR(fileno,"LOCAL",FALSE);
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
#ifndef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","ark");
#endif
#ifdef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","winzip");
#endif
		WRTNRD(fileno,0,NULL);
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","dia");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","DIA Diagram Format");
		FINDFLDSETCHAR(fileno,"LOCAL",FALSE);
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
		FINDFLDSETSTRING(fileno,"PROGRAM ID","");
		WRTNRD(fileno,0,NULL);

		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","doc");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","Microsoft Word Document");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
		FINDFLDSETCHAR(fileno,"LOCAL",FALSE);

#ifndef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","soffice");
#endif
#ifdef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","dohttp");
#endif
		WRTNRD(fileno,0,NULL);
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","odp");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","Open Office Presentation");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
		FINDFLDSETCHAR(fileno,"LOCAL",FALSE);
#ifndef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","soffice");
#endif
#ifdef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","dohttp");
#endif
		WRTNRD(fileno,0,NULL);
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","odt");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","Open Office Text");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
		FINDFLDSETCHAR(fileno,"LOCAL",FALSE);
#ifndef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","soffice");
#endif
#ifdef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","dohttp");
#endif
		WRTNRD(fileno,0,NULL);
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","ott");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","Open Office Text Template");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
		FINDFLDSETCHAR(fileno,"LOCAL",FALSE);
#ifndef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","soffice");
#endif
#ifdef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","dohttp");
#endif
		WRTNRD(fileno,0,NULL);
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","ods");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","Open Document Spreadsheet");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
		FINDFLDSETCHAR(fileno,"LOCAL",FALSE);
#ifndef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","soffice");
#endif
#ifdef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","dohttp");
#endif
		WRTNRD(fileno,0,NULL);
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","ots");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","Open Document Spreadsheet Template");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
		FINDFLDSETCHAR(fileno,"LOCAL",FALSE);
#ifndef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","soffice");
#endif
#ifdef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","dohttp");
#endif
		WRTNRD(fileno,0,NULL);
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","otp");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","Open Document Presentation Template");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
		FINDFLDSETCHAR(fileno,"LOCAL",FALSE);
#ifndef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","soffice");
#endif
#ifdef WIN32
		FINDFLDSETCHAR(fileno,"MOZILLA",FALSE);
		FINDFLDSETSTRING(fileno,"PROGRAM ID","dohttp");
#endif
		WRTNRD(fileno,0,NULL);
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","odg");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","Open Document Drawing");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
		FINDFLDSETCHAR(fileno,"LOCAL",FALSE);
#ifndef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","soffice");
#endif
#ifdef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","dohttp");
#endif
		WRTNRD(fileno,0,NULL);

		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","otg");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","Open Document Drawing Template");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
		FINDFLDSETCHAR(fileno,"LOCAL",FALSE);
#ifndef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","soffice");
#endif
#ifdef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","dohttp");
#endif
		WRTNRD(fileno,0,NULL);


		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","ppt");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","Power Point Presentation");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
		FINDFLDSETCHAR(fileno,"LOCAL",FALSE);
#ifndef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","soffice");
#endif
#ifdef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","dohttp");
#endif
		WRTNRD(fileno,0,NULL);
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","sxc");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","Open Office Spreadsheet");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
		FINDFLDSETCHAR(fileno,"LOCAL",FALSE);
#ifndef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","soffice");
#endif
#ifdef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","dohttp");
#endif
		WRTNRD(fileno,0,NULL);
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","sxw");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","Open Office Document");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
		FINDFLDSETCHAR(fileno,"LOCAL",FALSE);
#ifndef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","soffice");
#endif
#ifdef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","dohttp");
#endif
		WRTNRD(fileno,0,NULL);






		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","jpeg");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","JPEG Image Format");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
		if(GUI_INTERFACE==1) FINDFLDSETCHAR(fileno,"LOCAL",TRUE);
			else FINDFLDSETCHAR(fileno,"LOCAL",FALSE);
#ifndef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","dohttp");
#endif
#ifdef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","dohttp");
#endif
		WRTNRD(fileno,0,NULL);

		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","sxw");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","Open Office Document");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
#ifndef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","soffice");
#endif
#ifdef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","dohttp");
#endif
		WRTNRD(fileno,0,NULL);
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","csv");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","Delimited File/Spreadsheet");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
#ifndef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","scalc");
#endif
#ifdef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","dohttp");
#endif
		WRTNRD(fileno,0,NULL);
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","tsv");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","Delimited File/Spreadsheet");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
#ifndef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","scalc");
#endif
#ifdef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","dohttp");
#endif
		WRTNRD(fileno,0,NULL);
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","xls");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","Excel Spreadsheet");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
#ifndef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","soffice");
#endif
#ifdef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","dohttp");
#endif
		WRTNRD(fileno,0,NULL);
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","sxc");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","Open Office Spreadsheet");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
#ifndef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","soffice");
#endif
#ifdef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","dohttp");
#endif
		WRTNRD(fileno,0,NULL);
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","html");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","Hypertext Markup Language");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
		FINDFLDSETSTRING(fileno,"PROGRAM ID","dohttp");
		WRTNRD(fileno,0,NULL);
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","xml");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","XML Markup Language");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
		FINDFLDSETSTRING(fileno,"PROGRAM ID","dohttp");
		WRTNRD(fileno,0,NULL);
		ZERNRD(fileno);
		FINDFLDSETSTRING(fileno,"TYPE","gpl");
		FINDFLDSETSTRING(fileno,"DESCRIPTION","GNUPLOT");
		FINDFLDSETCHAR(fileno,"DISPLAYABLE",TRUE);
		FINDFLDSETCHAR(fileno,"LOCAL",FALSE);

#ifndef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","gnuplot");
#endif
#ifdef WIN32
		FINDFLDSETSTRING(fileno,"PROGRAM ID","gnuplot");
#endif
		WRTNRD(fileno,0,NULL);
		CLSNRD(fileno);
	}
}
void DB_ARCHCLS()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	sort=DBsortNEW("ARCHIVE","ARCHCLS",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"CLASS",8,4);
		addDBfield(sort,"NAME",1,40);
		addDBfield(sort,"DESCRIPTION",14,800);
		addDBkey(sort,"ARCHCLS KEY");
		addDBkeypart(sort,1,"CLASS");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(7+7+77+1);
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [ARCHIVE] Screen [ARCHCLS], Can Not Save Database File!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
}
void DB_ARCHSUB()
{
	DBsort *sort=NULL;
	short x=(-1);
	char same=FALSE;
	char *temp1=NULL;

	sort=DBsortNEW("ARCHIVE","ARCHSUB",0);
	if(sort!=NULL)
	{
		addDBfield(sort,"DELETEFLAG",10,1);
		addDBfield(sort,"SUBJECT",1,15);
		addDBfield(sort,"TITLE",1,40);
		addDBfield(sort,"DESCRIPTION",14,800);
		addDBkey(sort,"ARCHSUB KEY");
		addDBkeypart(sort,1,"SUBJECT");

		same=CompareDATABASE(sort);
		if(same==FALSE)
		{
			MergeDATABASE(sort);
			SetupFileDefinition(sort);
			x=CHANGE_DATABASE_FILE(sort->module,sort->engine,sort->filename,sort->nofields,sort->fields,sort->nokeys,sort->keys,sort->servername);
			if(x==(-1))
			{
				if(temp1!=NULL) Rfree(temp1);
				temp1=Rmalloc(7+7+77+1);
				sprintf(temp1,"DATABASE FILE WRITE ERROR: Module [ARCHIVE] Screen [ARCHSUB], Can Not Save Database File!");
				prterr(temp1);
				if(errorlist!=NULL)
				{
					addERRlist(&errorlist,temp1);
				}
				if(temp1!=NULL) Rfree(temp1);
			} else {
				CLSNRD(x);
			}
		}
		if(sort!=NULL) ADVFreeDBsort(sort,FALSE);
	}
}
static void AddARCHCLS(short fileno,int c,char *n,char *desc)
{
	ZERNRD(fileno);
	FINDFLDSETINT(fileno,"CLASS",c);
	FINDFLDSETSTRING(fileno,"NAME",n);
	FINDFLDSETSTRING(fileno,"DESCRIPTION",desc);
	WRTNRD(fileno,0,NULL);	
}
void CreateARCHCLS()
{
	short fileno=(-1);
	int xclass=0;

	fileno=OPNNRD("ARCHIVE","ARCHCLS");
	if(fileno!=(-1))
	{
		AddARCHCLS(fileno,0,"N/A","Not Applicable/Not Classified");
		AddARCHCLS(fileno,++xclass,"REPORT","Reports");
		AddARCHCLS(fileno,++xclass,"DOCUMENTS","Documents");
		AddARCHCLS(fileno,++xclass,"SPREADSHEETS","Spreadsheets");
		AddARCHCLS(fileno,++xclass,"LETTERS","Letters");
		AddARCHCLS(fileno,++xclass,"MANUALS","Manuals");
		AddARCHCLS(fileno,++xclass,"MEMORANDUMS","Memos");
		AddARCHCLS(fileno,++xclass,"MINUTES","Minutes");
		AddARCHCLS(fileno,++xclass,"MISC","Miscellaneous");
		AddARCHCLS(fileno,++xclass,"NOTES","Notes");
		AddARCHCLS(fileno,++xclass,"FAX","Faxes");
		AddARCHCLS(fileno,++xclass,"LICENSE","License");
		AddARCHCLS(fileno,++xclass,"CERTIFICATION","Certification");
		AddARCHCLS(fileno,++xclass,"INTERVIEW","Interviews");
		AddARCHCLS(fileno,++xclass,"ORDINANCE","Ordinances");
		AddARCHCLS(fileno,++xclass,"PAPER","Papers");
		AddARCHCLS(fileno,++xclass,"POLICIES","Policies");
		AddARCHCLS(fileno,++xclass,"PROCEDURES","Procedures");
		AddARCHCLS(fileno,++xclass,"CONTRACTS","Contracts");
		AddARCHCLS(fileno,++xclass,"SPEECH","Speech");
		AddARCHCLS(fileno,++xclass,"CIRCULARS","Circulars");
		AddARCHCLS(fileno,++xclass,"CYCLES","Cycles");
		AddARCHCLS(fileno,++xclass,"PROPOSALS","Proposals");
		AddARCHCLS(fileno,++xclass,"HANDOUTS","Handouts");
		AddARCHCLS(fileno,++xclass,"PICTURES","Pictures");
		AddARCHCLS(fileno,++xclass,"MOVIES","Movies");
		AddARCHCLS(fileno,++xclass,"PRESENTATIONS","Presentations / Slide Shows");
		AddARCHCLS(fileno,++xclass,"VIDEO","Videos");
		AddARCHCLS(fileno,++xclass,"DRAWINGS","Drawings");
		AddARCHCLS(fileno,++xclass,"FLYER","Flyers");
		AddARCHCLS(fileno,++xclass,"BRIEF","Briefs");
		AddARCHCLS(fileno,++xclass,"PAMPHLET","Pamphlets");
		AddARCHCLS(fileno,++xclass,"POSTER","Posters");
		AddARCHCLS(fileno,++xclass,"PRESS ARTICLE","Press Articles");
		AddARCHCLS(fileno,++xclass,"PRESS STATEMENT","Press Statements");
		AddARCHCLS(fileno,++xclass,"PROGRAM","Programme");
		AddARCHCLS(fileno,++xclass,"TRANSCRIPTS","Transcripts");
		AddARCHCLS(fileno,++xclass,"TRIAL PROCEEDINGS","Trial Proceedings");
		AddARCHCLS(fileno,++xclass,"AFFIDAVIT","Affidavits");
		AddARCHCLS(fileno,++xclass,"BIOGRAPHICAL DOCUMENT","Biographical Documents");
		AddARCHCLS(fileno,++xclass,"BACKUP","Backups");
		AddARCHCLS(fileno,++xclass,"ZIP / TARBALL","Zips / Tarballs");
		CLSNRD(fileno);
	}
}
static void AddARCHSUB(short fileno,char *s,char *t,char *d)
{
	ZERNRD(fileno);
	FINDFLDSETSTRING(fileno,"SUBJECT",s);
	FINDFLDSETSTRING(fileno,"TITLE",t);
	FINDFLDSETSTRING(fileno,"DESCRIPTION",d);
	WRTNRD(fileno,0,NULL);	
}
void CreateARCHSUB()
{
	short fileno=(-1);
	
	fileno=OPNNRD("ARCHIVE","ARCHSUB");
	if(fileno!=(-1))
	{
		AddARCHSUB(fileno,"","",NULL);
		AddARCHSUB(fileno,"ACADEMIC","Acedemic",NULL);
		AddARCHSUB(fileno,"ADMINISTRATIVE","Administrative",NULL);
		AddARCHSUB(fileno,"FINANCE","Financial",NULL);
		AddARCHSUB(fileno,"A/P","Accounts Payable",NULL);
		AddARCHSUB(fileno,"PURCHASING","Purchasing / Requisition",NULL);
		AddARCHSUB(fileno,"PERSONNEL","Personnel",NULL);
		AddARCHSUB(fileno,"PAYROLL","Payroll",NULL);
		AddARCHSUB(fileno,"BUDGET","Budgetary / Preparation",NULL);
		AddARCHSUB(fileno,"CERTIFICATION","Certification / Licensure",NULL);
		AddARCHSUB(fileno,"INSTRUCTIONAL","Instructional",NULL);
		AddARCHSUB(fileno,"UTILITIES","Utilities",NULL);
		AddARCHSUB(fileno,"PLANNING","Planning",NULL);
		AddARCHSUB(fileno,"TREASURER","Treasurer",NULL);
		AddARCHSUB(fileno,"COMMISSIONER","Commissioner",NULL);
		AddARCHSUB(fileno,"CLAIM","Claims",NULL);
		AddARCHSUB(fileno,"MARKETING","Marketing",NULL);
		AddARCHSUB(fileno,"SALES","Sales",NULL);
		AddARCHSUB(fileno,"TRAINING","Training",NULL);
		AddARCHSUB(fileno,"CONSULTING","Consulting",NULL);
		AddARCHSUB(fileno,"AUDIT","Audit",NULL);
		AddARCHSUB(fileno,"SOCIAL SERVICES","Social Services",NULL);

		
		CLSNRD(fileno);
	}
}
static void SCN_ARCHIVE_MENU()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;
	char *libx=NULL,*org_logo=NULL;
	RDAGenericSetup *gsv=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("UTILITIES")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif

	gsv=RDAGenericSetupNew("UTILITIES","ORGANIZATION LOGO");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			org_logo=stralloc(gsv->value.string_value);
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);


	scrn=RDAscrnNEW("ARCHIVE","ARCHIVE MENU");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","F.A.S.T.","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"",XPERT_SETUP->company_name,org_logo,"",0,0,(org_logo!=NULL ? 1:0),NULL,NULL,NULL,NULL);
		if(org_logo!=NULL) Rfree(org_logo);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,3,NULL,NULL,NULL,NULL);
		memset(stemp,0,101);
		sprintf(stemp,"Installed: %s",CURRENT_DATE10);
		ADVaddwdgt(scrn,5,"",stemp,"","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,3,NULL,NULL,NULL,NULL);
		memset(stemp,0,101);
		sprintf(stemp,"Compiled: %s",VERSION_DATE);
		ADVaddwdgt(scrn,5,"",stemp,"","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DUE FOR ARCHIVE UPDATE","Due For Update","","",0,0,45,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"((GENERIC_SETUP_VARIABLE(\"[ARCHIVE][ARCHIVE STYLE]\"))=2)",NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE MANAGE SCANS","Manage Scans","","",0,0,21,NULL,NULL,"([ARCHIVE SETUP]=TRUE) AND ((GENERIC_SETUP_VARIABLE(\"[ARCHIVE][ARCHIVE STYLE]\"))=2)",NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"((GENERIC_SETUP_VARIABLE(\"[ARCHIVE][ARCHIVE STYLE]\"))=2)",NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE ADD ARCHIVES","Build Archives","","",0,0,30,NULL,NULL,"([ARCHIVE SETUP]=TRUE)",NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE MTN ARCHWORK","Maintain Work Groups/Folders","","",0,0,31,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE MTN ARCHIVES.SEARCH","Query Archives","","",0,0,12,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE MTN ARCHIVES","Maintain Archives","","",0,0,19,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE EMAILARCHIVE","Email Archives (Records & Documents)","","",0,0,20,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE REPORT SUBMENU","Report Submenu","","",0,0,7,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE MAINTENANCE SUBMENU","Maintenance Submenu","","",0,0,34,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE SETUP SUBMENU","Setup Submenu","","",0,0,35,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"MAIN MENU","Main Menu","","",0,0,38,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(12+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVE MENU], Can Not Save Maintain Master!");
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
static void SCN_ARCHIVE_DOCK_WINDOW()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","ARCHIVE DOCK WINDOW");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(58+1);
		sprintf(temp1,"%s",
			"((GENERIC_SETUP_VARIABLE(\"[ARCHIVE][ARCHIVE STYLE]\"))=2)");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,6,"Manage Scans","Manage Scans","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(58+1);
		sprintf(temp1,"%s",
			"((GENERIC_SETUP_VARIABLE(\"[ARCHIVE][ARCHIVE STYLE]\"))>0)");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,6,"Add Work Group","Add Work Group","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(58+1);
		sprintf(temp1,"%s",
			"((GENERIC_SETUP_VARIABLE(\"[ARCHIVE][ARCHIVE STYLE]\"))=2)");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,6,"Add Archives","Build Server Archives","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(358+1);
		sprintf(temp1,"%s",
			"((GENERIC_SETUP_VARIABLE(\"[ARCHIVE][ARCHIVE STYLE]\"))=2) AND ([WORKSTATION VALIDATED]=TRUE)");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,6,"Add Workstation Archives","Build Workstation Archives","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"Maintain Work Groups/Folders","Work Groups/Folders","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"Query Archives","Query Archives","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"Maintain Archives","Maintain Archives","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(58+1);
		sprintf(temp1,"%s",
			"((GENERIC_SETUP_VARIABLE(\"[ARCHIVE][ARCHIVE STYLE]\"))>0)");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,6,"Copy Archives","Copy / Move Archives","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		temp1=Rmalloc(58+1);
		sprintf(temp1,"%s",
			"((GENERIC_SETUP_VARIABLE(\"[ARCHIVE][ARCHIVE STYLE]\"))>0)");
		temp2=Rmalloc(3+1);
		sprintf(temp2,"%s",
			"");
		temp3=Rmalloc(3+1);
		sprintf(temp3,"%s",
			"");
		temp4=Rmalloc(3+1);
		sprintf(temp4,"%s",
			"");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,temp1,temp2,temp3,temp4);
		if(temp1!=NULL) Rfree(temp1);
		if(temp2!=NULL) Rfree(temp2);
		if(temp3!=NULL) Rfree(temp3);
		if(temp4!=NULL) Rfree(temp4);
		ADVaddwdgt(scrn,6,"Email Archives","Email Reports/Documents","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(19+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVE DOCK WINDOW], Can Not Save Maintain Master!");
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
static void MNU_ARCHIVE_EZRPT()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("ARCHIVE EZRPT","ezrpt.EXT ARCHIVE");
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
static void MNU_ARCHIVE_MENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("ARCHIVE MENU","domenu.EXT ARCHIVE \"ARCHIVE MENU\"");
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
static void SCN_ARCHIVE_MAINTENANCE_SUBMENU()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","ARCHIVE MAINTENANCE SUBMENU");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","F.A.S.T. Maintenance Submenu","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"((GENERIC_SETUP_VARIABLE(\"[ARCHIVE][ARCHIVE STYLE]\"))>0)",NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE ADD WORK GROUP","Add Work Group / Folder","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE ADD WORK GROUP USERS","Add Work Group Users","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE COPYARCHIVE","Copy/Move Archived Documents","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE COPYGROUPUSER","Copy Work Group Users","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE MTN ARCHWORK","Maintain Work Groups/Folders","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE MTN ARCHUSER","Maintain Archive Users","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE AUTOWRKGRP","Create Work Group(s)/User(s)","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE AUTOWRKGRPDEPT","Create Department Level Work Groups/Users","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE MENU","F.A.S.T.","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"MAIN MENU","Main Menu","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(27+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVE MAINTENANCE SUBMENU], Can Not Save Maintain Master!");
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
static void SCN_ARCHIVE_REPORT_SUBMENU()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","ARCHIVE REPORT SUBMENU");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","F.A.S.T. Report Submenu","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE RPTARCHIVE","Report of Archived Documents","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE WORK GROUPS","Report of Work Groups","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE WORKGROUPUSER","Report of Work Group Users","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE EZRPT","EZ-Report Writer","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE MENU","F.A.S.T.","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"MAIN MENU","Main Menu","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(22+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVE REPORT SUBMENU], Can Not Save Maintain Master!");
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
static void SCN_ARCHIVE_SETUP_SUBMENU()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("ARCHIVE","ARCHIVE SETUP SUBMENU");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,1,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","F.A.S.T. Setup Submenu","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE MTN ARCHTYPE","Maintain Types","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE MTN ARCHCLS","Maintain Classes","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE MTN ARCHSUB","Maintain Subjects","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE MTN ARCHPROG","Maintain Viewing Programs","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE ADD ARCHPROG","Add Viewing Programs","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE PURGEARCHIVE","Purge Archives (Records & Documents)","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE DOGSV","Generic Setup Variables","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ARCHIVE MENU","F.A.S.T.","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"MAIN MENU","Main Menu","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,0,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(21+7+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [ARCHIVE] Screen [ARCHIVE SETUP SUBMENU], Can Not Save Maintain Master!");
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
static void MNU_ARCHIVE_MAINTENANCE_SUBMENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("ARCHIVE MAINTENANCE SUBMENU","domenu.EXT ARCHIVE \"ARCHIVE MAINTENANCE SUBMENU\"");
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
static void MNU_ARCHIVE_REPORT_SUBMENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("ARCHIVE REPORT SUBMENU","domenu.EXT ARCHIVE \"ARCHIVE REPORT SUBMENU\"");
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
static void MNU_ARCHIVE_SETUP_SUBMENU()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("ARCHIVE SETUP SUBMENU","domenu.EXT ARCHIVE \"ARCHIVE SETUP SUBMENU\"");
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
void ARCHIVEMENU()
{
	SCN_ARCHIVE_MENU();
	MNU_ARCHIVE_MENU();
	SCN_ARCHIVE_DOCK_WINDOW();
	SCN_ARCHIVE_MAINTENANCE_SUBMENU();
	SCN_ARCHIVE_REPORT_SUBMENU();
	SCN_ARCHIVE_SETUP_SUBMENU();
	MNU_ARCHIVE_MAINTENANCE_SUBMENU();
	MNU_ARCHIVE_REPORT_SUBMENU();
	MNU_ARCHIVE_SETUP_SUBMENU();
	MNU_ARCHIVE_EZRPT();
}
#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */

static void SCRIPT_scandocs_lnx()
{
	FILE *fp=NULL;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("scandocs.lnx")+15);
#ifdef WIN32
	sprintf(temp,"%s\\scandocs.lnx",CURRENTDIRECTORY);
#endif
#ifndef WIN32
	sprintf(temp,"%s/scandocs.lnx",CURRENTDIRECTORY);
#endif
	unlink(temp);
	fp=fopen(temp,"w+b");

	fprintf(fp,"#!/bin/sh\n");
	fprintf(fp,"PASS=0\n");
	fprintf(fp,"PNM_TEMPFILE=$$.tempfile.pnm\n");
	fprintf(fp,"PS_TEMPFILE=$$.tempfile.ps\n");
	fprintf(fp,"PDF_TEMPFILE=$$.rdascan.pdf\n");
	fprintf(fp,"TEMPDIR=$$.tempdir\n");
	fprintf(fp,"SCANNER_DEV=\"hp:/dev/usb/scanner0\"\n");
	fprintf(fp,"echo $ADD_SCAN_DIRECTORY\n");
	fprintf(fp,"if [ ! -d $TEMPDIR ]\n");
	fprintf(fp,"then\n");
	fprintf(fp,"	mkdir $TEMPDIR\n");
	fprintf(fp,"fi\n");
	fprintf(fp,"cd $TEMPDIR\n");
	fprintf(fp,"until [ \"$PAGE\" = \"last\" 2>/dev/null ]\n");
	fprintf(fp,"do\n");
	fprintf(fp,"	PASS=`expr $PASS + 1`\n");
	fprintf(fp,"	tput clear\n");
	fprintf(fp,"	echo \"Please Insert Page Number $PASS And Then Press Enter\"\n");
	fprintf(fp,"	echo \"\"\n");
	fprintf(fp,"	echo \"\"\n");
	fprintf(fp,"	echo \"If This Is The End Of The Batch Type: \\\"last\\\" And Then Press Enter.\"\n");
	fprintf(fp,"	echo \"\"\n");
	fprintf(fp,"	read response\n");
	fprintf(fp,"	\n");
	fprintf(fp,"	case $response in\n");
	fprintf(fp,"	    LA*|La*) PAGE=last ;;\n");
	fprintf(fp,"	    la*|lA*) PAGE=last ;;\n");
	fprintf(fp,"	    *)	PAGE=$PASS ;;\n");
	fprintf(fp,"	esac\n");
	fprintf(fp,"	export PAGE	\n");
	fprintf(fp,"	if [ \"$PAGE\" = \"last\" 2>/dev/null ]\n");
	fprintf(fp,"	then\n");
	fprintf(fp,"		echo \"\"	\n");
	fprintf(fp,"		echo \"\"	\n");
	fprintf(fp,"	else\n");
	fprintf(fp,"		scanimage -d hp:/dev/usb/scanner0 --resolution 150 --speed Extra Fast --batch=file\"$PASS\".pnm --batch-count=1 --batch-start=$PASS  -v --mode color \n");
	fprintf(fp,"	fi\n");
	fprintf(fp,"done\n");
	fprintf(fp,"ls file1.pnm >/dev/null 2>&1\n");
	fprintf(fp,"if [ \"$?\" == \"0\" ]\n");
	fprintf(fp,"then\n");
	fprintf(fp,"	echo \"Creating PDF File For The Archive Module.\"	\n");
	fprintf(fp,"	echo \"This May Take A Couple Of Minutes.\"	\n");
	fprintf(fp,"	for xxx in `ls -A file*.pnm`\n");
	fprintf(fp,"	do\n");
	fprintf(fp,"		dd <$xxx >>$PNM_TEMPFILE 2>/dev/null\n");
	fprintf(fp,"		rm $xxx\n");
	fprintf(fp,"	done\n");
	fprintf(fp,"	pnmtops < $PNM_TEMPFILE 2>/dev/null | ps2pdf - $PDF_TEMPFILE\n");
	fprintf(fp,"	mv $PDF_TEMPFILE ../$ADD_SCAN_DIRECTORY/\n");
	fprintf(fp,"	\n");
	fprintf(fp,"	if [ \"$?\" == 0 ]\n");
	fprintf(fp,"	then\n");
	fprintf(fp,"		echo \"Finshed Creating PDF File.\"\n");
	fprintf(fp,"	fi\n");
	fprintf(fp,"else\n");
	fprintf(fp,"	echo \"No Pages Scanned.  Exiting Now.\"\n");
	fprintf(fp,"fi	\n");
	fprintf(fp,"cd ..\n");
	fprintf(fp,"rm -fr $TEMPDIR\n");
	fprintf(fp,"sleep 3 \n");
	fclose(fp);
#ifndef WIN32
	chmod(temp,(mode_t)MODE_RWXRWX___);
#endif
	if(temp!=NULL) Rfree(temp);
}
static void MNU_ARCHIVE_SCANDOCS()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;
	FILE *fp=NULL;
	char *temp=NULL;
#ifndef WIN32	
	char *dsp2use=NULL;
#endif

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("scandocs.lnx")+15);
#ifdef WIN32
	sprintf(temp,"%s\\scandoc.lnx",CURRENTDIRECTORY);
#endif
#ifndef WIN32
	sprintf(temp,"%s/scandoc.lnx",CURRENTDIRECTORY);
#endif
	unlink(temp);
	fp=fopen(temp,"w+b");

	fprintf(fp,"#!/bin/sh\n");

#ifndef WIN32	
	dsp2use=RDA_GetEnv("DISPLAY");
#endif
#ifdef WIN32
	sprintf(stemp,"scandocs.bat");
#endif
#ifdef LINUX
	sprintf(stemp,"/usr/X11R6/bin/xterm -T \"SCAN DOCUMENTS\" -fn 10x24 -e scandocs.lnx",(dsp2use!=NULL ? dsp2use:"")); 
#endif
#if defined(LINUX2_2) || defined(UBUNTU_OS)
	sprintf(stemp,"/usr/X11R6/bin/xterm -T \"SCAN DOCUMENTS\" -fn 10x24 -e scandocs.lnx",(dsp2use!=NULL ? dsp2use:"")); 
#endif
#ifdef RS6000
	sprintf(stemp,"/usr/bin/X11/aixterm -ls -fn pc8x16 -T \"SCAN DOCUMENTS\" -d %s -e scandocs.rsc",(dsp2use!=NULL ? dsp2use:"")); 
#endif
#ifdef SCO50
	sprintf(stemp,"/usr/bin/X11/scoterm -ls -fn pc8x16 -T \"SCAN DOCUMENTS\" -d %s -e scandocs.lnx",(dsp2use!=NULL ? dsp2use:"")); 
#endif
#ifdef SCO50P
	sprintf(stemp,"/usr/bin/X11/scoterm -ls -fn pc8x16 -T \"SCAN DOCUMENTS\" -d %s -e scandocs.lnx",(dsp2use!=NULL ? dsp2use:"")); 
#endif
#ifdef UNIXWARE21
	sprintf(stemp,"/usr/bin/X11/xterm -ls -fn pc8x16 -T \"SCAN DOCUMENTS\" -d %s -e scandocs.unw",(dsp2use!=NULL ? dsp2use:"")); 
#endif
#ifdef UNIXWARE7
	sprintf(stemp,"/usr/bin/X11/xterm -ls -fn pc8x16 -T \"SCAN DOCUMENTS\" -d %s -e scandocs.unw",(dsp2use!=NULL ? dsp2use:"")); 
#endif
#ifdef SUNX86
	sprintf(stemp,"/usr/X/bin/xterm -T \"SCAN DOCUMENTS\" -e scandocs.sun",(dsp2use!=NULL ? dsp2use:"")); 
#endif
	fprintf(fp,"%s\n",stemp);

	menu=RDAmenuNEW("ARCHIVE SCANDOCS",stemp);
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
	fclose(fp);
#ifndef WIN32
	chmod(temp,(mode_t)MODE_RWXRWX___);
#endif
	if(temp!=NULL) Rfree(temp);
}
void SCANSCRIPT()
{
	SCRIPT_scandocs_lnx();
	MNU_ARCHIVE_SCANDOCS();
}
static void SCRIPT_sendattach_1_lnx()
{
	FILE *fp=NULL;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("sendattach-1.lnx")+15);
#ifdef WIN32
	sprintf(temp,"%s\\sendattach-1.lnx",CURRENTDIRECTORY);
#endif
#ifndef WIN32
	sprintf(temp,"%s/sendattach-1.lnx",CURRENTDIRECTORY);
#endif
	unlink(temp);
	fp=fopen(temp,"w+b");
	fprintf(fp,"#!/bin/sh\n");
	fprintf(fp,"#Usage: \n");
	fprintf(fp,"#   $1=From Name (Steven J. Saunders)\n");
	fprintf(fp,"#   $2=From email address (sjsaunders@rdasys.com)\n");
	fprintf(fp,"#   $3=Subject \n");
	fprintf(fp,"#   $4=Body\n");
	fprintf(fp,"#   $5=email to list (support@rdasys.com)\n");
	fprintf(fp,"#   $6=Filename to be sent as attachment\n");
	fprintf(fp,"#   $7=Attachment name in email\n");
	fprintf(fp,"export VMIME_FROM_NAME=\"$1\"\n");
	fprintf(fp,"export VMIME_FROM_ADDR=\"$2\"\n");
	fprintf(fp,"export VMIME_SUBJECT=\"$3\"\n");
	fprintf(fp,"export VMIME_BODY=\"$4\"\n");
	fprintf(fp,"export VMIME_TO=\"$5\"\n");
	fprintf(fp,"export VMIME_FNAME1=\"$6\"\n");
	fprintf(fp,"export VMIME_DNAME1=\"$7\"\n");
	fprintf(fp,"vmime-sendmail.lnx\n");

	fclose(fp);
#ifndef WIN32
	chmod(temp,(mode_t)MODE_RWXRWX___);
#endif
	if(temp!=NULL) Rfree(temp);
}
static void SCRIPT_sendattach_3_lnx()
{
	FILE *fp=NULL;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("sendattach-3.lnx")+15);
#ifdef WIN32
	sprintf(temp,"%s\\sendattach-3.lnx",CURRENTDIRECTORY);
#endif
#ifndef WIN32
	sprintf(temp,"%s/sendattach-3.lnx",CURRENTDIRECTORY);
#endif
	unlink(temp);
	fp=fopen(temp,"w+b");
	fprintf(fp,"#!/bin/sh\n");
	fprintf(fp,"#Usage: \n");
	fprintf(fp,"#   $1=From Name (Steven J. Saunders)\n");
	fprintf(fp,"#   $2=From email address (sjsaunders@rdasys.com)\n");
	fprintf(fp,"#   $3=Subject \n");
	fprintf(fp,"#   $4=Body\n");
	fprintf(fp,"#   $5=email to list (support@rdasys.com)\n");
	fprintf(fp,"#   $xxx=Filename to be sent as attachment\n");
	fprintf(fp,"#   $xxx=Attachment name in email\n");
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	fprintf(fp,"N=1\n");
	fprintf(fp,"for xxx in `echo $FILENAMES_TO_EMAIL`\n");
	fprintf(fp,"do\n");
	fprintf(fp,"	if [ $N = 1 ]\n");
	fprintf(fp,"	then	\n");
	fprintf(fp,"		export VMIME_FNAME1=\"$xxx\"\n");
	fprintf(fp,"	elif [ $N = 2 ]\n");
	fprintf(fp,"	then	\n");
	fprintf(fp,"		export VMIME_FNAME2=\"$xxx\"\n");
	fprintf(fp,"	elif [ $N = 3 ]\n");
	fprintf(fp,"	then	\n");
	fprintf(fp,"		export VMIME_FNAME3=\"$xxx\"\n");
	fprintf(fp,"	elif [ $N = 4 ]\n");
	fprintf(fp,"	then	\n");
	fprintf(fp,"		export VMIME_FNAME4=\"$xxx\"\n");
	fprintf(fp,"	elif [ $N = 5 ]\n");
	fprintf(fp,"	then	\n");
	fprintf(fp,"		export VMIME_FNAME5=\"$xxx\"\n");
	fprintf(fp,"	elif [ $N = 6 ]\n");
	fprintf(fp,"	then	\n");
	fprintf(fp,"		export VMIME_FNAME6=\"$xxx\"\n");
	fprintf(fp,"	elif [ $N = 7  ]\n");
	fprintf(fp,"	then	\n");
	fprintf(fp,"		export VMIME_FNAME7=\"$xxx\"\n");
	fprintf(fp,"	elif [ $N = 8 ]\n");
	fprintf(fp,"	then	\n");
	fprintf(fp,"		export VMIME_FNAME8=\"$xxx\"\n");
	fprintf(fp,"	elif [ $N = 9 ]\n");
	fprintf(fp,"		export VMIME_FNAME9=\"$xxx\"\n");
	fprintf(fp,"	fi\n");
	fprintf(fp,"	((++N))\n");
	fprintf(fp,"done\n");
	fprintf(fp,"export VMIME_FROM_NAME=\"$1\"\n");
	fprintf(fp,"export VMIME_FROM_ADDR=\"$2\"\n");
	fprintf(fp,"export VMIME_SUBJECT=\"$3\"\n");
	fprintf(fp,"export VMIME_BODY=\"$4\"\n");
	fprintf(fp,"export VMIME_TO=\"$5\"\n");
	fprintf(fp,"vmime-sendmail.lnx\n");

	fclose(fp);
#ifndef WIN32
	chmod(temp,(mode_t)MODE_RWXRWX___);
#endif
	if(temp!=NULL) Rfree(temp);
}
void SENDATTACHED()
{
	SCRIPT_sendattach_1_lnx();
	SCRIPT_sendattach_3_lnx();
}
#ifdef USING_QT
static void addMenuItem(short fileno,char *md,char *itemnumber1,char *arg1,
char *arg2,char *arg3,char *arg4,char *arg5,char *dflag,char *dd_list1,
char *en1,char *en2,char *en3,char *en4,char *en5,char *name,char * parent1,
char *program,char *rank1,char *style1,char *ev1,char *ev2,char *ev3,
char *ev4,char *ev5,char *vis)
{
	char delflag=FALSE;
	int itemnumber=atoi(itemnumber1);
	short dd_list=atoi(dd_list1);
	int parent=atoi(parent1);
	int rank=atoi(rank1);
	short style=atoi(style1);

	if(!RDAstrcmp(dflag,"Y")) delflag=TRUE;
	ZERNRD(fileno);
	FINDFLDSETCHAR(fileno,"DELETEFLAG",delflag);
	FINDFLDSETSTRING(fileno,"MODULE NAME",md);
	FINDFLDSETINT(fileno,"ITEM NUMBER",itemnumber);
	FINDFLDSETSTRING(fileno,"ARGUMENT 1",arg1);
	FINDFLDSETSTRING(fileno,"ARGUMENT 2",arg2);
	FINDFLDSETSTRING(fileno,"ARGUMENT 3",arg3);
	FINDFLDSETSTRING(fileno,"ARGUMENT 4",arg4);
	FINDFLDSETSTRING(fileno,"ARGUMENT 5",arg5);
	FINDFLDSETSHORT(fileno,"DROP DOWN LIST",dd_list);
	FINDFLDSETSTRING(fileno,"ENVIRONMENT NAME 1",en1);
	FINDFLDSETSTRING(fileno,"ENVIRONMENT NAME 2",en2);
	FINDFLDSETSTRING(fileno,"ENVIRONMENT NAME 3",en3);
	FINDFLDSETSTRING(fileno,"ENVIRONMENT NAME 4",en4);
	FINDFLDSETSTRING(fileno,"ENVIRONMENT NAME 5",en5);
	FINDFLDSETSTRING(fileno,"NAME",name);
	FINDFLDSETINT(fileno,"PARENT",parent);
	FINDFLDSETSTRING(fileno,"PROGRAM",program);
	FINDFLDSETSHORT(fileno,"RANK",rank);
	FINDFLDSETSHORT(fileno,"STYLE",style);
	FINDFLDSETSTRING(fileno,"ENVIRONMENT VALUE 1",ev1);
	FINDFLDSETSTRING(fileno,"ENVIRONMENT VALUE 2",ev2);
	FINDFLDSETSTRING(fileno,"ENVIRONMENT VALUE 3",ev3);
	FINDFLDSETSTRING(fileno,"ENVIRONMENT VALUE 4",ev4);
	FINDFLDSETSTRING(fileno,"ENVIRONMENT VALUE 5",ev5);
	FINDFLDSETSTRING(fileno,"VISIBLE EXPRESSION",vis);
	WRTNRD(fileno,0,NULL);
}
void CREATE_MENUITEM()
{
	short fileno=(-1),ef=FALSE;
	char custom=FALSE,*mod=NULL,*mod1=NULL;

	fileno=OPNNRD("UTILITIES","MENUITEM");
	if(fileno!=(-1))
	{
		mod=stralloc("ARCHIVE");
		FINDFLDSETSTRING(fileno,"MODULE NAME",mod);
		ef=FRSNRDsec(fileno,1);
		while(!ef)
		{
			FINDFLDGETSTRING(fileno,"MODULE NAME",&mod1);
			if(RDAstrcmp(mod,mod1)) break;
			FINDFLDGETCHAR(fileno,"CUSTOM",&custom);
			if(!custom) DELNRD(fileno);
			ef=NXTNRDsec(fileno,1);
		}
		if(mod!=NULL) Rfree(mod);
		if(mod1!=NULL) Rfree(mod1);
		addMenuItem(fileno,"ARCHIVE","00001000","ARCHIVE","MANAGE SCANS","","","","N","05","","","","","","Manage Scans","00000000","RUNPOWERADD","0000","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00001050","ARCHIVE","ADD WORK GROUP","","","","N","05","","","","","","Add Work Group","00000000","RUNPOWERADD","0000","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00001100","ARCHIVE","ADD ARCHIVE","","","","N","05","","","","","","Add Archives","00000000","RUNPOWERADD","0000","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00001110","ARCHIVE","ADD WORKSTATION ARCHIVE","","","","N","05","","","","","","Add Workstation Archives","00000000","RUNPOWERADD","0000","00","","","","","","([WORKSTATION VALIDATED]=TRUE)");
		addMenuItem(fileno,"ARCHIVE","00001200","ARCHIVE","MTN ARCHWORK","","","","N","05","","","","","","Maintain Work Groups/Folders","00000000","RUNMAINTAIN","0000","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00001300","ARCHIVE","MTN ARCHIVES.SEARCH","","","","N","05","","","","","","Query Archives","00000000","RUNMAINTAIN","0000","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00001400","ARCHIVE","MTN ARCHIVES","","","","N","05","","","","","","Maintain Archives","00000000","RUNMAINTAIN","0000","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00001500","ARCHIVE","EMAILARCHIVE","","","","N","05","","","","","","Email Archives","00000000","RUNREPORT","0000","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00002000","ARCHIVE","MTN ARCHIVES.SEARCH","","","","N","00","","","","","","Archives","00000000","RUNMAINTAIN","0020","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00002010","ARCHIVE","MTN ARCHWORK.SEARCH","","","","N","00","","","","","","Work Groups","00000000","RUNMAINTAIN","0010","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00002020","ARCHIVE","MTN ARCHUSER.SEARCH","","","","N","00","","","","","","Work Group Users","00000000","RUNMAINTAIN","0015","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00002500","ARCHIVE","RPTARCHIVE","","","","N","01","","","","","","Report of Archived Documents","00000000","RUNREPORT","0010","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00002600","ARCHIVE","WORK GROUPS","","","","N","01","","","","","","Report of Work Groups","00000000","RUNREPORT","0020","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00002700","ARCHIVE","WORKGROUPUSER","","","","N","01","","","","","","Report of Work Group Users","00000000","RUNREPORT","0030","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00003000","ARCHIVE","ADD WORK GROUP","","","","N","02","","","","","","Add Work Group","00000000","RUNPOWERADD","0010","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00003100","ARCHIVE","ADD WORK GROUP USERS","","","","N","02","","","","","","Add Work Group Users","00000000","RUNPOWERADD","0020","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00003150","ARCHIVE","COPYGROUPUSER","","","","N","02","","","","","","Copy Work Group Users","00000000","RUNREPORT","0025","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00003200","ARCHIVE","COPYARCHIVE","","","","N","02","","","","","","Copy Archive","00000000","RUNREPORT","0030","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00003300","ARCHIVE","MTN ARCHWORK","","","","N","02","","","","","","Maintain Work Groups","00000000","RUNMAINTAIN","0040","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00003400","ARCHIVE","MTN ARCHUSER","","","","N","02","","","","","","Maintain Work Groups Users","00000000","RUNMAINTAIN","0050","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00003500","ARCHIVE","AUTOWRKGRP","","","","N","02","","","","","","Create Work Groups and Users","00000000","RUNREPORT","0060","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00003600","ARCHIVE","AUTOWRKGRPDEPT","","","","N","02","","","","","","Create Department Level Work Groups and Users","00000000","RUNREPORT","0070","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00004000","ARCHIVE","MTN ARCHTYPE","","","","N","03","","","","","","Archive Types","00000000","RUNMAINTAIN","0010","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00004100","ARCHIVE","MTN ARCHCLS","","","","N","03","","","","","","Archive Classes","00000000","RUNMAINTAIN","0020","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00004200","ARCHIVE","MTN ARCHSUB","","","","N","03","","","","","","Archive Subjects","00000000","RUNMAINTAIN","0030","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00004300","ARCHIVE","MTN ARCHPROG","","","","N","03","","","","","","Archive Viewing Programs","00000000","RUNMAINTAIN","0040","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00004400","ARCHIVE","ADD ARCHPROG","","","","N","03","","","","","","Add Archive Viewing Program","00000000","RUNPOWERADD","0050","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00004500","ARCHIVE","PURGEARCHIVE","","","","N","03","","","","","","Purge Archives","00000000","RUNREPORT","0080","00","","","","","","");
		addMenuItem(fileno,"ARCHIVE","00004900","ARCHIVE","","","","","N","03","","","","","","Generic Setup Variables","00000000","DOGSV","0090","00","","","","","","");
		CLSNRD(fileno);
	}
}
#endif /* USING_QT */
