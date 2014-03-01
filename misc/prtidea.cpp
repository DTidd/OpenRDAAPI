#include<misc.hpp>
#include<gui.hpp>
#ifdef WIN32
struct OutRDAs
{
	WORD osize;
	char o[4096];
};
typedef struct OutRDAs OutRDA;
typedef struct stat Statb;
#endif /* ifdef WIN32 */

#define __DRIVING_OUTPUT_DATA_DIRECTORY__

#ifdef WIN32
BOOL IsPrinterHandle(HANDLE hPrinter)
{
	DWORD cbNeeded;
	DWORD Error;
	BOOL bRet=FALSE;
	LPPRINTER_INFO_2 pPrinter;
	DWORD cbBuf;
	HANDLE hMem=NULL;

	if(!GetPrinter(hPrinter,2,(LPBYTE)NULL,cbBuf,&cbNeeded))
	{
		Error=GetLastError();
		if(Error==ERROR_INSUFFICIENT_BUFFER)
		{
			hMem=GlobalAlloc(GHND,cbNeeded);
			if(!hMem) return(bRet);
			pPrinter=(LPPRINTER_INFO_2)GlobalLock(hMem);
			cbBuf=cbNeeded;		
			if(GetPrinter(hPrinter,2,(LPBYTE)pPrinter,cbBuf,&cbNeeded))
			{
				bRet=TRUE;
				GlobalUnlock(hMem);
				GlobalFree(hMem);
			} else {
				SetLastError(ERROR_INVALID_PRINTER_NAME);
			}
		} else if(Error==ERROR_INVALID_HANDLE)
		{
			SetLastError(ERROR_INVALID_PRINTER_NAME);
		}	
	}
	return(bRet);
}
#endif /* ifdef WIN32 */

RDA_PFILE *xRDA_PFILE_NEW(char *name,char *perm,int line,char *file)
{
	RDA_PFILE *r=NULL;
	char *temp1=NULL;
#ifndef WIN32
#ifdef __PREVIOUSLY_USED_CODE__
	int fds[2]; 
	pid_t pid; 
#endif /* __PREVIOUSLY_USED_CODE__ */
	char *pname=NULL,*oname=NULL;
#endif
#ifdef WIN32
	char tname[501];
	DOC_INFO_1 DocInfo;
	DWORD len,x=0,y=0;
	UUID myuuid;
	RPC_CSTR myuuidstr;
	APPlib *args=NULL;
#endif /* ifdef WIN32 */

	diagmisc=1;
	diagprt=1;

	if(diagprt)
	{
		prterr("DIAG RDA_PFILE_NEW name [%s] perm [%s] at line [%d] program [%s].",(name!=NULL ? name:""),(perm!=NULL ? perm:""),line,file);
	}
	r=Rmalloc(sizeof(RDA_PFILE));
	r->wrote_something=FALSE;
#ifndef WIN32
	temp1=RDA_GetEnv("VIEWPDF_OUTPUT_FILENAME");
	if((!strncmp(name,"viewpdf",7) || !strncmp(name,"VIEWPDF",7)) && (temp1==NULL) && (RDAstrlen(name)<12))
	{
		TRACE;
		if((diagprt)&&(temp1!=NULL))
		{
			prterr("DIAG:  VIEWPDF_OUTPUT_FILENAME: [%s]",temp1);
		TRACE;
		}
#ifdef __PREVIOUSLY_USED_CODE__
		pipe (fds); 
		pid = fork (); 
		if (pid == (pid_t) 0) {
			close (fds[1]); 
			dup2 (fds[0], STDIN_FILENO); 
			oname=Rmalloc(RDAstrlen(USERLOGIN)+25);
			sprintf(oname,"%s%d.pdf",USERLOGIN,RGETPID());
#ifdef __DRIVING_OUTPUT_DATA_DIRECTORY__
			pname=Rmalloc(RDAstrlen(DOCUMENTROOT)+RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(oname)+128);
			sprintf(pname,"%s/viewpdf.lnx --dont-view --dont-delete -o %s/%s",DOCUMENTROOT,CURRENTDIRECTORY,oname);
#else 
			pname=Rmalloc(RDAstrlen(DOCUMENTROOT)+RDAstrlen(DOCUMENTROOT)+RDAstrlen(oname)+128);
			sprintf(pname,"%s/viewpdf.lnx --dont-view --dont-delete -o %s/%s",DOCUMENTROOT,DOCUMENTROOT,oname);
#endif
			r->name=stralloc(oname);

			execl("/bin/sh","sh","-c",pname,NULL);
		}else{
			r->pfile=fdopen(fds[1],"w"); 
		}
#else 
		oname=Rmalloc(RDAstrlen(USERLOGIN)+25);
		sprintf(oname,"%s%d.pdf",USERLOGIN,RGETPID());
		r->name=stralloc(oname);
#ifdef __DRIVING_OUTPUT_DATA_DIRECTORY__
		pname=Rmalloc(RDAstrlen(DOCUMENTROOT)+RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(oname)+128);
		sprintf(pname,"%s/viewpdf.lnx --dont-view --dont-delete -o %s/%s",DOCUMENTROOT,CURRENTDIRECTORY,oname);
#else 
		pname=Rmalloc(RDAstrlen(DOCUMENTROOT)+RDAstrlen(DOCUMENTROOT)+RDAstrlen(oname)+128);
		sprintf(pname,"%s/viewpdf.lnx --dont-view --dont-delete -o %s/%s",DOCUMENTROOT,DOCUMENTROOT,oname);
#endif
		r->pfile=popen(pname,perm);
#endif /* __PREVIOUSLY_USED_CODE__ */
		TRACE;
	}else if((!strncmp(name,"viewpdf",7) || !strncmp(name,"VIEWPDF",7)) && (temp1!=NULL))
	{
		TRACE;
		pname=Rmalloc(RDAstrlen(DOCUMENTROOT)+RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(temp1)+128);
		sprintf(pname,"%s/viewpdf.lnx --dont-view --dont-delete -o %s/%s",DOCUMENTROOT,CURRENTDIRECTORY,temp1);
		r->pfile=popen(pname,perm);
		r->name=stralloc(name);
	}else{
		TRACE;
		pname=Rmalloc(RDAstrlen(DOCUMENTROOT)+RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(temp1)+128);
		sprintf(pname,"%s/%s",DOCUMENTROOT,name);
		r->pfile=popen(pname,perm);
		r->name=stralloc(name);
	}
	if(r->pfile==NULL)
	{
		prterr("Error popen failed for name [%s] perm [%s] at line [%d] program [%s].",(name!=NULL ? name:""),(perm!=NULL ? perm:""),line,file);
	}
#endif /* ifndef WIN32 */
#ifdef WIN32
	r->pfile=NULL;
	if(!strncmp(name,"rdaout",6) || !strncmp(name,"RDAOUT",6))
	{
		sprintf(tname,"rdaout.%d",RGETPID());
		r->pfile=fopen(tname,"wbc");
		if(r->pfile==NULL)
		{
			prterr("Error fopen failed for temprorary file [%s] at line [%d] program [%s].",(tname!=NULL ? tname:""),line,file);
		}
	} else if(!strncmp(name,"wordpad",7) || !strncmp(name,"WORDPAD",7))
	{
		sprintf(tname,"wordpad.%d",RGETPID());
		r->pfile=fopen(tname,"wbc");
		if(r->pfile==NULL)
		{
			prterr("Error fopen failed for temprorary file [%s] at line [%d] program [%s].",(tname!=NULL ? tname:""),line,file);
		}
	} else if(!strncmp(name,"notepad",7) || !strncmp(name,"NOTEPAD",7))
	{
		sprintf(tname,"notepad.%d",RGETPID());
		r->pfile=fopen(tname,"wbc");
		if(r->pfile==NULL)
		{
			prterr("Error fopen failed for temprorary file [%s] at line [%d] program [%s].",(tname!=NULL ? tname:""),line,file);
		}
	} else if(!strncmp(name,"viewpdf",7) || !strncmp(name,"VIEWPDF",7))
	{
		memset(tname,0,501);
		UuidCreateSequential(&myuuid);
		UuidToStringA(&myuuid,&myuuidstr);
		sprintf(tname,"\\\\.\\pipe\\%s",myuuidstr);
		_putenv_s("VIEWPDF_PIPE",myuuidstr);
		args=APPlibNEW();
		Execute2Program("viewpdf",args);
		if(args!=NULL) freeapplib(args);

		for(x=0;x<240;x++) {
			r->lph=CreateFile(tname,GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
			if(r->lph == INVALID_HANDLE_VALUE) {
				Sleep(250);
				prterr("CreateFile failed for Named Pipe client [%d]\n",x);
			}else{
				prterr("CreateFile sucessful [%d]\n",x);
				break;
			}
		}
	} else {
		DocInfo.pDocName="Xpert Tools";
		DocInfo.pOutputFile=NULL;
		DocInfo.pDatatype="RAW";
		if(diagprt)
		{
			prterr("DIAG Preparing to call OpenPrinter for [%s].",(name!=NULL ? name:""));
		}
		if(!OpenPrinter(name,&r->lph,NULL))
		{
			x=GetLastError();
			prterr("Error OpenPrinter Failed....Error#: [%d]",x);
		}
		if(r->lph==NULL) 
		{	
			x=GetLastError();
			prterr("Error OpenPrinter didn't get handle! Error#: [%d]",x);
		}
		r->jobno=StartDocPrinter(r->lph,1,(LPSTR)&DocInfo);
		if(r->jobno==0)
		{
			x=GetLastError();
			prterr("Error StartDocPrinter failed to start spooled print job with error [%d].",x);
		}
		if(!StartPagePrinter(r->lph))
		{
			x=GetLastError();
			prterr("Error StartPagePrinter failed to start spooled page with error [%d].",x);
		}
	}
	r->name=stralloc(name);
#endif /* ifdef WIN32 */
	r->perm=stralloc(perm);

	diagmisc=0;
	diagprt=0;

	return(r);
}
RDA_PFILE *xRDA_popen(char *name,char *perm,int line,char *file)
{
	RDA_PFILE *r=NULL;

	if(diagprt)
	{
		prterr("DIAG RDA_popen at line [%d] progrma [%s].",line,file);
	}
	r=RDA_PFILE_NEW(name,perm);
	r->wrote_something=FALSE;
	return(r);
}
/* RDA_fprintf - Replacement for fprintf to manage cross-platforms */
#ifdef RS6000
#endif
#if defined(SCO50) || defined(SCO50P)
#endif
#ifdef UNIXWARE7
#endif
#ifdef UNIXWARE21
#endif
#ifdef LINUX
#endif
#ifdef LINUX2_2
#endif
#ifdef SUNX86
#endif
#ifdef WIN32
int RDA_fprintf(RDA_PFILE *p,char *fmt,...)
{
	va_list args;
	int x=0,y=0;
	int len=0;
	static char buffer[4096],tempx[4096];
	char temp[2];
	DWORD cbWritten;
	OutRDA o;
	DWORD dwWrite;

	if(p==NULL) return;
	if(!strncmp(p->name,"rdaout",6) || !strncmp(p->name,"RDAOUT",6))
	{
		if(p->pfile!=NULL)
		{
			va_start(args,fmt);
			x=vfprintf(p->pfile,fmt,args);
			va_end(args);
			fflush(p->pfile);
		}
	} else if(!strncmp(p->name,"wordpad",7) || !strncmp(p->name,"WORDPAD",7))
	{
		if(p->pfile!=NULL)
		{
			va_start(args,fmt);
			x=vfprintf(p->pfile,fmt,args);
			va_end(args);
			fflush(p->pfile);
		}
	} else if(!strncmp(p->name,"notepad",7) || !strncmp(p->name,"NOTEPAD",7))
	{
		if(p->pfile!=NULL)
		{
			va_start(args,fmt);
			x=vfprintf(p->pfile,fmt,args);
			va_end(args);
			fflush(p->pfile);
		}
	} else if(!strncmp(p->name,"viewpdf",7) || !strncmp(p->name,"VIEWPDF",7))
	{
		va_start(args,fmt);
		memset(buffer,0,4096);
		x=vsprintf(buffer,fmt,args);
		va_end(args);

		if(!WriteFile(p->lph,buffer,(strlen(buffer)),&dwWrite,NULL))
		{
			prterr("WriteFile failed for Named Pipe client\n");
		}else{
			prterr("WriteFile succeeded for Named Pipe client [%d]\n",dwWrite);
		}
	} else 
	{
		va_start(args,fmt);
		memset(buffer,0,4096);
		x=vsprintf(buffer,fmt,args);
		va_end(args);
		if(x)
		{
			if(strchr(buffer,'\f'))
			{
				memcpy(tempx,buffer,4096);
				for(y=0;y<4096 && x<RDAstrlen(buffer);++y) 
				{
					if(tempx[y]=='\f') 
					{
						tempx[y+1]=0;
						break;
					}	
				}
				if(y<4096)
				{
					if(!WritePrinter(p->lph,tempx,y+1,&len))
					{
						x=GetLastError();
						prterr("Error WritePrinter failed to send buffer data error [%d].",x);
					}
					if(!EndPagePrinter(p->lph))
					{
						x=GetLastError();
						prterr("Error EndPagePrinter failed...Error# [%d]",x);	
					}
					if(!StartPagePrinter(p->lph))
					{
						x=GetLastError();
						prterr("Error StartPagePrinter failed to start spooled page with error [%d].",x);
					}
					if(RDAstrlen(buffer)>(y+1))
					{
						if(!WritePrinter(p->lph,&buffer[y+1],RDAstrlen(buffer)-y-1,&len))
						{
							x=GetLastError();
							prterr("Error WritePrinter failed to send buffer data error [%d].",x);
						}
					}
				} else {
					if(!WritePrinter(p->lph,buffer,y,&len))
					{
						x=GetLastError();
						prterr("Error WritePrinter failed to send buffer data error [%d].",x);
					}
				}
			} else {
				if(!WritePrinter(p->lph,buffer,x,&len))
				{
					x=GetLastError();
					prterr("Error WritePrinter failed to send buffer data error [%d].",x);
				}
			}
		}
	}
	return(x);
}
#endif /* ifdef WIN32 */

#ifndef WIN32
int RDA_fprintf(RDA_PFILE *p,char *fmt,...)
{
	va_list args;
	int x=0;

	if(p==NULL) return(-1);
	if(p->pfile!=NULL)
	{
		p->wrote_something=TRUE;
		va_start(args,fmt);
		x=vfprintf(p->pfile,fmt,args);
		va_end(args);
	}
	return(x);
}
#endif /* ifndef WIN32 */
/* Close Printer or Temporary File and dump to spooler */
void xRDA_pclose(RDA_PFILE *p,int line,char *file)
{
	char tempx[512];
#ifdef WIN32
	APPlib *args=NULL;
	char filename[101];
	int x=0;
#endif /* ifdef WIN32 */
	if(diagprt)
	{
		prterr("DIAG RDA_pclose at line [%d] program [%s].",line,file);
	}
	if(p==NULL) return;
	if(p!=NULL)
	{
#ifdef WIN32
		if(p->pfile!=NULL) 
		{
			fflush(p->pfile);
			fclose(p->pfile);
		}
		_flushall();
		if(!strncmp(p->name,"rdaout",6) || !strncmp(p->name,"RDAOUT",6))
		{
			memset(filename,0,101);
			sprintf(filename,"rdaout.%d",RGETPID());
			args=APPlibNEW();
			addAPPlib(args,filename);
			Execute2Program("rdaout",args);
			if(args!=NULL) freeapplib(args);
			unlink(filename);
		} else if(!strncmp(p->name,"wordpad",7) || !strncmp(p->name,"WORDPAD",7))
		{
			memset(filename,0,101);
			sprintf(filename,"wordpad.%d",RGETPID());
			args=APPlibNEW();
			addAPPlib(args,filename);
			Execute2Program("wordpad",args);
			if(args!=NULL) freeapplib(args);
			unlink(filename);
		} else if(!strncmp(p->name,"notepad",7) || !strncmp(p->name,"NOTEPAD",7))
		{
			memset(filename,0,101);
			sprintf(filename,"notepad.%d",RGETPID());
			args=APPlibNEW();
			addAPPlib(args,filename);
			Execute2Program("notepad",args);
			if(args!=NULL) freeapplib(args);
			unlink(filename);
		} else if(!strncmp(p->name,"viewpdf",7) || !strncmp(p->name,"VIEWPDF",7))
		{
			CloseHandle(p->lph);
			Sleep(1000);
		} else
		{
			if(!EndPagePrinter(p->lph))
			{
				x=GetLastError();
				prterr("Error EndPagePrinter failed...Error# [%d]",x);	
			}
			if(!EndDocPrinter(p->lph))
			{
				x=GetLastError();
				prterr("Error EndDocPrinter failed...Error# [%d]",x);	
			}
			if(!ClosePrinter(p->lph))
			{
				x=GetLastError();
				prterr("Error ClosePrinter failed...Error# [%d]",x);	
			}
		}
#endif /* ifdef WIN32 */
#ifndef WIN32
		if(p->pfile!=NULL) pclose(p->pfile);
#endif /* ifndef WIN32 */
#ifdef __DRIVING_OUTPUT_DATA_DIRECTORY__
		if(p->wrote_something)
		{
			DisplayFile(p->name);
		}
		sprintf(tempx,"%s/%s",CURRENTDIRECTORY,p->name);
		unlink(tempx);
#else
		ADV2Execute2Program(p->name,NULL,NULL,TRUE);
		sprintf(tempx,"%s/%s",DOCUMENTROOT,p->name);
		unlink(tempx);
#endif
		if(p->name!=NULL) Rfree(p->name);
		if(p->perm!=NULL) Rfree(p->perm);
		Rfree(p);
	}
}
void xRDA_fflush(RDA_PFILE *p,int line,char *file)
{
	if(diagprt)
	{
		prterr("DIAG RDA_fflush at line [%d] program [%s].",line,file);
	}
	if(p!=NULL)
	{
		if(p->pfile!=NULL) 
		{
			fflush(p->pfile);
		}
	}
}
/* RDA_snprintf - Replacement for fprintf to manage cross-platforms */
int RDA_snprintf(char *str, size_t size, char *fmt,...)
{
	va_list args;
	int x=0;

	if(str==NULL) 
	{
		prterr("Output string pointer is NULL.  Plase make sure that the output output string has been allocated.");
		return(-1);
	}
	va_start(args,fmt);
#ifndef WIN32
	x=vsnprintf(str,size,fmt,args);
#else
	x=_snprintf(str,size,fmt,args);
#endif
	va_end(args);
	return(x);
}
