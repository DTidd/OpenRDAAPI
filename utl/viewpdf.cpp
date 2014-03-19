#include <stdarg.h>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctime>
#ifndef WIN32
#include <libps/pslib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pwd.h>
#include <uuid/uuid.h>
#include <assert.h>
#endif

#ifdef WIN32
#include <libps\pslib.h>
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <rpc.h>
#include <rpcdce.h>
#endif


#ifdef MEMORY_DEBUGGING
#include </usr/include/libps/pslib-mp.h>
#endif

#include "ierrors.h"
#include "iapi.h"



/* Example of using GS DLL as a ps2pdf converter.  */
#include <cstdio>
#if defined(_WIN32) && !defined(_Windows)
# define _Windows
#endif
#ifdef _Windows
/* add this source to a project with gsdll32.dll, or compile it directly with:
 *   cl -D_Windows -Isrc -Febin\ps2pdf.exe ps2pdf.c bin\gsdll32.lib
 */
//# include <windows.h>
//# define GSDLLEXPORT __declspec(dllimport)
#endif

#ifdef WIN32
#define TRACE   fprintf(stderr,"FILE: [%s] LINE: [%d] DATE: [%s]\n",__FILE__,__LINE__,__DATE__);FlushFileBuffers(stderr);
#else
#define TRACE   fprintf(stderr,"FILE: [%s] LINE: [%d] DATE: [%s]\n",__FILE__,__LINE__,__DATE__);fflush(stderr);
#endif

#define BUFSIZE 65535
#define PIPE_TIMEOUT 5000
char *VERSION_DATE=__VER__;

//#define CAPTURE_INPUT 1
#ifdef CAPTURE_INPUT
char diagfilename[1024];
FILE *diagfp=NULL;
#endif /* CAPTURE_INPUT */

struct RDA_PS_VARIABLES
{
	PSDoc *ps;
	int psfont;
	char *string;
	char *cmdstr;
	int esc_cmd;
	int rda_exec_cmd;
	/* RDA PS PRT VARIABLES */
	float textx;
	float texty;
	float lmargin;
	int *font_size;
	int *font_height;
	int formfeed; 
	int pagesizedef;
	int landscape;
	int psimage[10000];
	int pstmpl[10000];
	int rdapclborder;
	//  These variables were globals.
	int font_array[150];
	int line_space[4];
	int font_options[150];
	int font_set;
	// These are needed for for ghostscript to use the right page.  Does not allow for changing pagesize mid input at the moment.
	int pagewidth;
	int pageheight;

	char *named_pipe;

	char *input;
	char *output;
	FILE *outputfp;


	int pushpopx;
	int pushpopy;
	int nlcounter;
	int pgcounter;
	int pgopen;
	int printstyle;


	int prepagecount;
	int prepagecmd[150];
	char *prepagecmdstr[150];

	int viewfile;
	int removefile;
	int debug;

};
typedef struct RDA_PS_VARIABLES RDA_PS_VARS;


int ProcessPrePageCmds(RDA_PS_VARS *);
int ParseInput(int, char **,RDA_PS_VARS *,int);
int LaunchPDFViewer(RDA_PS_VARS *);


struct MYGSVARIABLES
{
	int use_gslib;
	int code;
	int code1;
	int exit_code;
	char *command;
	size_t bytes_read;
	FILE *fp;
	int readsize;
	char *gsargv[14];
	int gsargc;
	void *minst;

};
typedef struct MYGSVARIABLES MYGSVARS;


MYGSVARS *gsvars=NULL;
int gswritecount=0;


//void errorhandler(PSDoc *p, int error, const char *str, void *data) {
//	fprintf(stderr, "PSLib: %s  Code:%d\n",str,error);
//}


int FileExists(char *pathname,char *filename)
{
#ifdef WIN32
	return (GetFileAttributes(filename) == INVALID_FILE_ATTRIBUTES) ? 0 : -1;
#else
	return access(filename, R_OK);
#endif
/*
	struct stat *buf;
	if(pathname==NULL) {
		if((stat(filename,buf))==0) {
			return (0);
		}else{
			return (-1);
		}
	//}else{ 
		// This needs to stat() the path to make sure it is a 
		// directory then chdir() to it.  Then in should stat()
		// the filename and return error status of that command.
		// It needs to return to the previous CWD before returning status.
	}
*/
}

int DecryptDataFromFile(char *filename,char *password,char **filedata,size_t *size)
{
	FILE *fp=NULL;
	int bytes_read=0,bufsize=65535,x=0;
	size_t readsize=0,memsize=65535;
	char inputarray[65535];
	char *temp1;

#ifndef WIN32
	if((password==NULL)||(strlen(password)==0)) {
		return(1);
	}
	strlen(filename);
	x=((strlen("echo \"%s\" | gpg --batch --no-tty --status-fd 2 --passphrase-fd 0 --output - --decrypt %s"))+(strlen(password))+(strlen(filename))+10);
	temp1=(char *)malloc(x);
	memset(temp1,0,x);
	sprintf(temp1,"echo \"%s\" | gpg --batch --no-tty --status-fd 2 --passphrase-fd 0 --output - --decrypt %s",password,filename);
	fp=popen(temp1,"r");
	if(fp!=NULL)
	{
		*filedata=(char *)realloc(*filedata,((sizeof(char*))*(*filedata,readsize+memsize+1)));
		if(*filedata==NULL) {
			fprintf(stderr,"(char *)realloc failed  at line [%d]\n",__LINE__);
		}
		while(!feof(fp)) {
			memset(inputarray,0,bufsize+1);
	        	bytes_read=fread(inputarray,1,bufsize,fp);
			if((bytes_read==0)&&(ferror(fp)!=0)) {
				fprintf(stderr,"fread failed at with error [%d] at line [%d]\n",ferror(fp),__LINE__);
			}else{
				readsize=(bytes_read+readsize);
				if(readsize>memsize)
				{
					*filedata=(char *)realloc(*filedata,((sizeof(char*))*(*filedata,readsize+memsize+1)));
					memsize=(memsize+readsize);
				}
				if(*filedata==NULL) {
					fprintf(stderr,"(char *)realloc failed  at line [%d]\n",__LINE__);
				}
				memcpy(*filedata+(readsize-bytes_read),inputarray,bytes_read);
			}
		}
		memset(inputarray,0,bufsize);
		pclose(fp);
		memset(*filedata+(readsize),0,1);
	}else{
		fprintf(stderr,"Can not open gpg pipe for file %s\n",filename);
		if(temp1!=NULL) free(temp1);
		return 1;
	}
	if(temp1!=NULL) free(temp1);
	*size=readsize;
#endif
	return 0;
}

#ifdef WIN32
int bufferwrite(FILE *fp,const char *value,int size,int maxbuf)
{
	int x=0;
	if(maxbuf<1) { maxbuf=4096; }
    	while(1) { 
		if((size-x)<maxbuf) {
			fwrite(value,1,(size-x),fp);
			break;
		}else{
			fwrite(value,1,maxbuf,fp);
			if(ferror(fp)==-1) { return (1); }
		}
		value += maxbuf;
		x=(x+maxbuf);
		//printf ("size [%d]  x [%d]\n",size,x); 
	}
	return (0);
}

int win32_exec_handles(HANDLE **procinfo,HANDLE *stdioh_0,HANDLE *stdioh_1,HANDLE *stdioh_2,char *option,int wait)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    SECURITY_ATTRIBUTES sa;
    DWORD threadID;
    char msg[600];
    BOOL ok;
    HANDLE hProcess;
    char cmdline[1024];

    hProcess = GetCurrentProcess();

    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags   = STARTF_USESTDHANDLES;
    si.hStdInput = INVALID_HANDLE_VALUE;

    ZeroMemory(&sa, sizeof(SECURITY_ATTRIBUTES));
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = FALSE;
    //CreatePipe(&Out.pipe, &h, &sa, 0);

    if(stdioh_0!=INVALID_HANDLE_VALUE) {
    	DuplicateHandle(hProcess,stdioh_0,hProcess,&si.hStdInput,0,TRUE,DUPLICATE_SAME_ACCESS | DUPLICATE_CLOSE_SOURCE);
    }else{
    	si.hStdInput=INVALID_HANDLE_VALUE;
    }

    /* create a non-inheritible pipe. */
    if(stdioh_1!=INVALID_HANDLE_VALUE) {
    	DuplicateHandle(hProcess,stdioh_1,hProcess,&si.hStdOutput,0,TRUE,DUPLICATE_SAME_ACCESS | DUPLICATE_CLOSE_SOURCE);
    }else{
    	si.hStdOutput=INVALID_HANDLE_VALUE;
    }

    /* Same as above, but for the error side. */
    if(stdioh_2!=INVALID_HANDLE_VALUE) {
    	DuplicateHandle(hProcess,stdioh_2,hProcess,&si.hStdError,0,TRUE, DUPLICATE_SAME_ACCESS | DUPLICATE_CLOSE_SOURCE);
    }else{
    	si.hStdError=INVALID_HANDLE_VALUE;
    }
    
    /* base command line */
    strcpy(cmdline,option);

    ok = CreateProcess(
	    NULL,	    /* Module name. */
	    cmdline,	    /* Command line. */
	    NULL,	    /* Process handle not inheritable. */
	    NULL,	    /* Thread handle not inheritable. */
	    TRUE,	    /* yes, inherit handles. */
	    DETACHED_PROCESS, /* No console for you. */
	    NULL,	    /* Use parent's environment block. */
	    NULL,	    /* Use parent's starting directory. */
	    &si,	    /* Pointer to STARTUPINFO structure. */
	    &pi);	    /* Pointer to PROCESS_INFORMATION structure. */

    if (!ok) {
	fprintf(stderr,"Tried to launch: \"%s\", but got error [%u]: ",cmdline,GetLastError());
	return 2;
    }

    /* close our references to the write handles that have now been inherited. */
    CloseHandle(si.hStdOutput);
    //CloseHandle(si.hStdError);

    //WaitForInputIdle(pi.hProcess, 5000);
    CloseHandle(pi.hThread);

    /* block waiting for the process to end. */
    if(wait) {
	    WaitForSingleObject(pi.hProcess,INFINITE);
    }else{
	    DuplicateHandle(hProcess,pi.hProcess,hProcess,procinfo,0,TRUE,DUPLICATE_SAME_ACCESS);
    }
    CloseHandle(pi.hProcess);
    return (0);
}

PHANDLE win32_popen_h(HANDLE **procinfo,char *cmdline,char *mode)
{
	SECURITY_ATTRIBUTES sa;
	HANDLE *stdioh_0,*stdioh_1,*stdioh_2;
	PHANDLE readpipe,writepipe;

	ZeroMemory(&sa, sizeof(SECURITY_ATTRIBUTES));
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = FALSE;

	stdioh_0=INVALID_HANDLE_VALUE;
	stdioh_1=INVALID_HANDLE_VALUE;
	stdioh_2=INVALID_HANDLE_VALUE;
	readpipe=INVALID_HANDLE_VALUE;
	writepipe=INVALID_HANDLE_VALUE;

	/* To avoid having to pass procinfo, custom named pipes could be used
	   instead of anonymous pipes which contained the process id as part
           of the pipe filename.  This could be retrieved by _get_osfhandle()
           and then use the handle to retrieve the filename.  Then parse the
           process id from the filename.  This would allow for a standard 
           replacement of the popen function in windows without custom commands.
           An example filename would be \\\\Pipe\\UUID__CHILDID.  
	   On Vista/2008 or later use GetFinalPathNameByHandle() for get filename
           For XP, http://msdn.microsoft.com/en-us/library/aa366789(VS.85).aspx.
        */

	if((CreatePipe(&readpipe,&writepipe,&sa,0))==0) {
		fprintf(stderr,"Create Pipe Error [%u]\n",GetLastError());
		return INVALID_HANDLE_VALUE;
	}
	//CreatePipe(&readpipe,&writepipe,&sa,65535);

	// Shall we redirect stderr to stdout ? 
  	// if ((err2out = strstr("2>&1",cmd)) != NULL) 
	
	if((!strcmp("w",mode))||(!strcmp("wb",mode))) {
		win32_exec_handles(procinfo,readpipe,stdioh_1,stdioh_2,cmdline,0);
    		CloseHandle(readpipe);
		return(writepipe);
	}else{
		win32_exec_handles(procinfo,stdioh_0,writepipe,stdioh_2,cmdline,0);
    		CloseHandle(writepipe);
		return(readpipe);
	}
}

int win32_pclose_h(HANDLE **procinfo,PHANDLE *pipe)
{
    	CloseHandle(pipe);
	if(pipe!=INVALID_HANDLE_VALUE) {
		WaitForSingleObject(*procinfo,INFINITE);
    		CloseHandle(*procinfo);
	}
	return (0);
}

FILE* win32_popen(HANDLE **procinfo,char *cmdline,char *mode)
{
	FILE *fp=NULL;
	int fdval=0;
	HANDLE temphand;
	temphand=INVALID_HANDLE_VALUE; 

    	temphand=win32_popen_h(procinfo,cmdline,mode);
	if(temphand!=INVALID_HANDLE_VALUE) {
	 	fdval=_open_osfhandle(temphand,0);
    		fp=_fdopen(fdval,mode);
		CloseHandle(temphand);
		return (fp);
	}
	return (NULL);
}

int win32_pclose(HANDLE **procinfo,FILE *fp)
{
	fclose(fp);
	if(*procinfo!=INVALID_HANDLE_VALUE) {
		WaitForSingleObject(*procinfo, INFINITE);
    		CloseHandle(*procinfo);
	}
	return (0);
}

int win32_pclose_nowait(HANDLE **procinfo,FILE *fp)
{
	fclose(fp);
	if(*procinfo!=INVALID_HANDLE_VALUE) {
    		CloseHandle(*procinfo);
	}
	return (0);
}
#endif
void ParseCmdLine(int argc, char *argv[],RDA_PS_VARS *myvars) {
	// This function needs to parse the command line
	// and set all relevant values to the myvars structure
	// or execute any needed ExecEscCmd commands.

	int i;
	char *mytemp=NULL;
#ifndef WIN32
	uuid_t out;
#else
	UUID myuuid;
	RPC_CSTR myuuidstr;
#endif
	//struct stat myfst;
	for (i=1;i<argc;i++) {
		//if(substr)	
		//sprintf(variable,"V%d[%s]\n",i,argv[i]);

		// option -pt# is for page type
		// option -ps# is for print style
#ifdef USE_RDA_DIAGNOSTICS
		if(myvars->debug) { fprintf(stderr,"Arg [%s]  strlen [%d]\n",argv[i],strlen(argv[i])); }
#endif /* USE_RDA_DIAGNOSTICS */
		
		if((strncmp(argv[i],"-i",2))==0) {
			//if((strncmp(mystr2+((strlen(mystr2)) - 4),".eps",4))==0) {

			if((strlen(argv[i]))==2) {
				++i;
				myvars->input=(char *)malloc(sizeof(char*)*((strlen(argv[i]))+1));
				memset(myvars->input,0,(strlen(argv[i]))+1);
				sprintf(myvars->input,"%s",argv[i]);
			} else {
				myvars->input=(char *)malloc(sizeof(char*)*((strlen(argv[i]))+1));
				memset(myvars->input,0,(strlen(argv[i]))+1);
				strncpy(myvars->input,argv[i]+2,(strlen(argv[i]))-2);
			}

		}else if((strncmp(argv[i],"-o",2))==0) {
			if((strlen(argv[i]))==2) {
				++i;
				myvars->output=(char *)malloc(sizeof(char*)*((strlen(argv[i]))+1));
				memset(myvars->output,0,(strlen(argv[i]))+1);
				sprintf(myvars->output,"%s",argv[i]);
			} else {
				myvars->output=(char *)malloc(sizeof(char*)*((strlen(argv[i]))+1));
				memset(myvars->output,0,(strlen(argv[i]))+1);
				strncpy(myvars->output,argv[i]+2,(strlen(argv[i]))-2);
			}
		}else if((strncmp(argv[i],"-wp",2))==0) {
#ifdef WIN32
			fprintf(stderr,"Current Var named_pipe value [%s].\n",myvars->named_pipe);
			if(myvars->named_pipe==NULL) {
				if((strlen(argv[i]))==3) {
					++i;
					myvars->named_pipe=(char *)malloc(sizeof(char*)*((strlen(argv[i]))+1));
					memset(myvars->named_pipe,0,(strlen(argv[i]))+1);
					strcat (myvars->named_pipe,argv[i]);
				} else {
					myvars->named_pipe=(char *)malloc(sizeof(char*)*((strlen(argv[i]))+1));
					memset(myvars->named_pipe,0,(strlen(argv[i]))+1);
					strcat (myvars->named_pipe,argv[i]+3);
				}
				fprintf(stderr,"Setting Win32 Named Pipe To [%s]\n",myvars->named_pipe);
			}else{
				fprintf(stderr,"Win32 Named Pipe Already Set To [%s]\n",myvars->named_pipe);
			}
			fprintf(stderr,"CMD Var named_pipe value [%s].\n",myvars->named_pipe);
#endif
		}else if((strncmp(argv[i],"-d",2))==0) {
			if((strlen(argv[i]))==2) {
				++i;
				myvars->debug=atoi(argv[i]);
			} else {
				// Need to fix to allow -d5 to work using temp string and atoi...
				myvars->debug=1;
			}
#ifdef USE_RDA_DIAGNOSTICS
			if((myvars->debug==0)&&((getenv("VIEWPDF_DEBUG"))!=NULL)) {
				myvars->debug=1;
			}
#endif /* USE_RDA_DIAGNOSTICS */
		}else if((strcmp(argv[i],"-pt1"))==0) {
			myvars->pagesizedef=1;
			fprintf(stderr,"Arg Found [%s]\n",argv[i]);
		} else if((strcmp(argv[i],"-pt2"))==0) {
			myvars->pagesizedef=2;
			fprintf(stderr,"Arg Found[%s]\n",argv[i]);
		} else if((strcmp(argv[i],"-pt3"))==0) {
			myvars->pagesizedef=3;
			fprintf(stderr,"Arg Found[%s]\n",argv[i]);
		} else if((strcmp(argv[i],"-pt4"))==0) {
			myvars->pagesizedef=4;
			fprintf(stderr,"Arg Found[%s]\n",argv[i]);
		} else if((strcmp(argv[i],"-pt5"))==0) {
			myvars->pagesizedef=5;
			fprintf(stderr,"Arg Found[%s]\n",argv[i]);
		} else if((strcmp(argv[i],"-pt6"))==0) {
			myvars->pagesizedef=6;
			fprintf(stderr,"Arg Found[%s]\n",argv[i]);
		} else if((strcmp(argv[i],"-pt7"))==0) {
			myvars->pagesizedef=7;
			fprintf(stderr,"Arg Found[%s]\n",argv[i]);
		} else if((strcmp(argv[i],"-pt8"))==0) {
			myvars->pagesizedef=8;
			fprintf(stderr,"Arg Found[%s]\n",argv[i]);
		} else if((strcmp(argv[i],"-ps1"))==0) {
			myvars->printstyle=1;
			fprintf(stderr,"Arg Found[%s]\n",argv[i]);
		} else if((strcmp(argv[i],"-ps2"))==0) {
			myvars->printstyle=2;
			fprintf(stderr,"Arg Found[%s]\n",argv[i]);
		} else if((strcmp(argv[i],"--dont-view"))==0) {
			myvars->viewfile=0;
			fprintf(stderr,"Arg Found[%s]\n",argv[i]);
		} else if((strcmp(argv[i],"--dont-delete"))==0) {
			myvars->removefile=0;
			fprintf(stderr,"Arg Found[%s]\n",argv[i]);
		} else if(((strcmp(argv[i],"-version"))==0)||((strcmp(argv[i],"--version"))==0)) {
			fprintf(stderr,"ViewPDF built on [%s]\n",__VER__);
#ifdef WIN32
			fprintf(stdout,"ViewPDF built on [%s]\n",__VER__);
#endif
		}
	}
				
	mytemp=getenv("VIEWPDF_INPUT_FILENAME");
	if(mytemp!=NULL) {
		myvars->input=(char *)malloc(sizeof(char*)*((strlen(mytemp))+1));
		memset(myvars->input,0,(strlen(mytemp))+1);
		sprintf(myvars->input,"%s",mytemp);
	}

	mytemp=getenv("VIEWPDF_OUTPUT_FILENAME");
	if(mytemp!=NULL) {
		myvars->output=(char *)malloc(sizeof(char*)*((strlen(mytemp))+1));
		memset(myvars->output,0,(strlen(mytemp))+1);
		sprintf(myvars->output,"%s",mytemp);
	}

	mytemp=getenv("VIEWPDF_DONT_VIEW");
	if(mytemp!=NULL) {
		if((strcmp(mytemp,"TRUE"))==0) {
			myvars->viewfile=0;
		}
	}

	mytemp=getenv("VIEWPDF_DONT_DELETE");
	if(mytemp!=NULL) {
		if((strcmp(mytemp,"TRUE"))==0) {
			myvars->removefile=0;
		}
	}



	if((myvars->output!=NULL)&&((strcmp(myvars->output,"-"))==0)){
		myvars->viewfile=0;
		myvars->removefile=0;
	}


	if((strstr(argv[0],"viewpdf"))!=NULL && myvars->output==NULL) {

#ifdef USE_RDA_DIAGNOSTICS
		if(myvars->debug)  { fprintf(stderr,"Program Type is ViewPDF [%d].\n",i); }
#endif /* USE_RDA_DIAGNOSTICS */

#ifndef WIN32
		uuid_generate_time(out);
		myvars->output=(char *)malloc(sizeof(char *)*100);
		memset(myvars->output,0,100);
		uuid_unparse_lower(out,myvars->output);

		sprintf(myvars->output,"%s.pdf",myvars->output);
#else
		UuidCreateSequential(&myuuid);
		UuidToStringA(&myuuid,&myuuidstr);
		myvars->output=(char *)malloc(sizeof(char *)*100);
		memset(myvars->output,0,100);
		sprintf(myvars->output,"%s.pdf",myuuidstr);
#endif
		

#ifdef USE_RDA_DIAGNOSTICS
	}else if(myvars->debug){
		fprintf(stderr,"Program Type is TXT2PS.\n");
#endif /* USE_RDA_DIAGNOSTICS */
	}
	

}
void ExecEscCmd(int rda_exec_cmd,RDA_PS_VARS *myvars) {
	//This should be a if then else list of defined PS Print commands	
	//
	//fprintf(stderr, "CMD: [%d] STR [%s]\n",myvars->rda_exec_cmd,cmdstr);
	
	/*
	int psimage, pstmpl;
	int my_exec_cmd, my_psfont, my_font_size;
	char *my_cmdstr, *my_string; 
	float my_textx = 0.0, my_texty =0.0;
	PSDoc *my_ps;
	*/
	
	char *mystr1=NULL,*mystr2=NULL,*mystr3=NULL,*mystr4=NULL,*mystr5=NULL;
	float myfloat;
	char temparray[65535];
	size_t size=0;
#ifndef WIN32
	struct passwd *passwd;
#endif

	myfloat=0.0;

	//fprintf(stderr,"rdaexeccmd [%d] cmdstr [%s] string [%s]\n",rda_exec_cmd,myvars->cmdstr,myvars->string);
	
	//myvars->rda_exec_cmd=myvars->rda_exec_cmd;
	//float tx = 0.0, ty = 0.0, *varp=NULL;
	
	// Examples of using structure passed variables
	// myvars->texty=0
	// myvars.textx=0
	
		/*
		if((textx_pt = (char *)malloc(sizeof(float)))==NULL) 
		{ puts("Unable to allocate memory");
		  exit(1);
		}
		if((texty_pt = (char *)malloc(sizeof(float)))==NULL) 
		{ puts("Unable to allocate memory");
		  exit(1);
		}
		*/
#ifdef USE_RDA_DIAGNOSTICS
	if(myvars->debug) { fprintf(stderr,"Command [%d] NL [%d] .\n",rda_exec_cmd,myvars->nlcounter); }
#endif /* USE_RDA_DIAGNOSTICS */
	switch (rda_exec_cmd) {
	case 001:
			//NORMAL
			//fprintf(stderr,"CMD001 [%d] STR[%s]\n",myvars->rda_exec_cmd,cmdstr);
			//psfont = PS_findfont(ps, "Helvetica", "", 0);
			//psfont = PS_findfont(ps,"Courier","",1);

			//if(strlen(string)==0 && font_array[0] != font_array[1] || line_space[0] != 12) {
			//	
			//	myfloat=PS_stringwidth(ps,string,font_array[0],12);
			//	PS_show_xy(ps,string,*textx_pt,*texty_pt);
			//	*textx_pt=(*textx_pt + myfloat);
			//}
			
		/*
			if(((strlen(myvars->string)!=0) && (myvars->font_array[0] == myvars->font_array[1]))) {
				
				myfloat=PS_stringwidth(myvars->ps,myvars->string,myvars->font_array[0],myvars->line_space[0]);
				PS_show_xy(myvars->ps,myvars->string,myvars->textx,myvars->texty);
				myvars->textx=(myvars->textx + myfloat);
				sprintf(myvars->string, "%c",0);
			}
		*/		
				
			//printf("font1=%d\n",myvars->font_array[1]);
			PS_setfont(myvars->ps,myvars->font_array[1],12.0);
			PS_set_value(myvars->ps,"charspacing",0.0);
			myvars->font_array[0]=myvars->font_array[1];
			myvars->line_space[0]=12;
			myvars->font_set=1;
			myvars->printstyle=1;
			break;
	case 002:
			//CONDENSED
			//fprintf(stderr,"CMD002 [%d] STR[%s]\n",myvars->rda_exec_cmd,cmdstr);
			//psfont = PS_findfont(ps,"Helvetica","",0);
			//psfont = PS_findfont(ps,"Courier","",1);
			//if(strlen(string)==0 && font_array[0] != font_array[1] || line_space[0] != 9) {
			//	
			//	myfloat=PS_stringwidth(ps,string,font_array[0],12);
			//	PS_show_xy(ps,string,*textx_pt,*texty_pt);
			//	*textx_pt=(*textx_pt + myfloat);
			//}
				
			//if(strlen(myvars->string)==0 && myvars->font_array[0] != myvars->font_array[1] || myvars->line_space[0] != 9) {

		/*
			if(((strlen(myvars->string)!=0) && (myvars->font_array[0] == myvars->font_array[1]))) {
				
				myfloat=PS_stringwidth(myvars->ps,myvars->string,myvars->font_array[0],myvars->line_space[0]);
				PS_show_xy(myvars->ps,myvars->string,myvars->textx,myvars->texty);
				myvars->textx=(myvars->textx + myfloat);
				sprintf(myvars->string, "%c",0);
			}
		
		*/
			PS_setfont(myvars->ps,myvars->font_array[1],7.0);
			PS_set_value(myvars->ps,"charspacing",0.11);
			myvars->line_space[0]=9;
			myvars->font_set=1;
			myvars->printstyle=2;
				
			break;
	case 050:
			// Preload/Embedd all commonly used fonts.
			myvars->font_array[1] = PS_findfont(myvars->ps,"c0419bt_","",1);
			//myvars->font_array[1] = PS_findfont(myvars->ps,"Courier","",0);
				
		//myvars->font_array[1] = PS_findfont(myvars->ps,"c0583bt_","",1);
	//myvars->font_array[1] = PS_findfont(myvars->ps,"Courier","",0);
			//myvars->font_array[1] = PS_findfont(myvars->ps,"Courier_New_Bold","",1);
			//myvars->font_array[1] = PS_findfont(myvars->ps,"Courier_New","",1);
			
			if(myvars->font_array[1]==0) { fprintf(stderr,"Could not load font Courier.\n"); }
			
			//font_array[2] = PS_findfont(ps,"CourierNew-Bold","",0);

			// These need to be handled with a switch for forms usage.
		//	myvars->font_array[21] = PS_findfont(myvars->ps,"Times_New_Roman","",1);
	
			myvars->font_array[22] = PS_findfont(myvars->ps,"Arial","",1);
			myvars->font_array[23] = PS_findfont(myvars->ps,"Arial_Bold","",1);
			if(myvars->font_array[22]==0) { fprintf(stderr,"Could not load font Arial.\n"); }
			if(myvars->font_array[23]==0) { fprintf(stderr,"Could not load font Arial Bold.\n"); }

			// Needed for barcodes.  Commenting out because of memory leak in PSLIB on fonts not found.
			//myvars->font_array[10] = PS_findfont(myvars->ps,"MRVCode39extSA","",1);
			
			
			//myvars->font_array[11] = PS_findfont(myvars->ps,"MRVCode39extSA","",1);
			//if(myvars->font_array[10]==0) { printf("Could not load font MRVCode39extSA.\n"); }
			myvars->font_array[0] = myvars->font_array[1];
			if(myvars->font_array[0]==0) { fprintf(stderr,"Default Font Invalid.\n"); }
			break;
	case 051:
			// Multi option command to load specific fonts
			// to be used to load less used fonts.
			switch(atoi(myvars->cmdstr)) {
				case 100:
					myvars->font_array[100] = PS_findfont(myvars->ps,"VeraMono","",1);
					myvars->font_options[100]=0;
					if(myvars->font_array[100]==0) { fprintf(stderr,"Could not load font VeraMono.\n"); }
					break;
				case 101:
					myvars->font_array[101] = PS_findfont(myvars->ps,"c0419bt_","",1);
					myvars->font_options[101]=0;
					if(myvars->font_array[101]==0) { fprintf(stderr,"Could not load font Courier.\n"); }
					break;
			}
			break;
	case 003:
			fprintf(stderr,"Unknown Command 003.\n");
			break;
	case 80:
			//DEF_PARAMS
			//PS_set_parameter(myvars->ps,"warning","true");
			PS_set_parameter(myvars->ps,"warning","false");
			PS_set_parameter(myvars->ps,"imageencoding","hex");

//			PS_set_parameter(myvars->ps,"BoundingBox","???");
//			PS_set_parameter(myvars->ps,"Orientation","???");
//			
			//PS_set_parameter(myvars->ps,"imageencoding","85");
#ifndef WIN32
			PS_set_parameter(myvars->ps,"SearchPath","./fonts");
			PS_set_parameter(myvars->ps,"SearchPath","./forms");
			PS_set_parameter(myvars->ps,"SearchPath","/rda/fonts");
			PS_set_parameter(myvars->ps,"SearchPath","/rda/forms");
#else
			PS_set_parameter(myvars->ps,"SearchPath",".\\fonts");
			PS_set_parameter(myvars->ps,"SearchPath",".\\forms");
			PS_set_parameter(myvars->ps,"SearchPath","\\rda\\fonts");
			PS_set_parameter(myvars->ps,"SearchPath","\\rda\\forms");
#endif
			//PS_set_value(ps,"charspacing",-0.5);
			break;
	case 81:
			//PS_HEADER
			// These need to be set from the command line and from the environment
			// I do not think they can be set after the first page is started...
			// Title/Subject needs to capture REPORT TITLE and OPTIONAL REPORT TITLE variables.
			// Creator needs to be some variation of Xpert and txt2ps...
			// Keywords should include report titles and a date at the minimum.  Maybe module info too?


			//mystr1=(char *)malloc((strlen(getenv("CYBER_PRODUCT")))+3);
			//mystr2=(char *)malloc(512);
			mystr1=getenv("CYBER_PRODUCT");
			if(mystr1==NULL) { 
				mystr1=(char *)malloc(sizeof(char*)*10);
				memset(mystr1,0,10);
				sprintf(mystr1,"UTILITIES");
			}
			
			mystr2=getenv("CYBER_PROCESS");
			if(mystr2==NULL) { 
				mystr2=(char *)malloc(sizeof(char*)*20);
				memset(mystr2,0,20);
				sprintf(mystr2,"TXT2PS CREATE PDF");
			}
			
			mystr3=getenv("REPORT_TITLE");
			if(mystr3==NULL) { 
				mystr3=(char *)malloc(sizeof(char*)*20);
				memset(mystr3,0,20);
				sprintf(mystr3," ");
			}
			
			mystr4=getenv("REPORT_DESCRIPTION");
			if(mystr4==NULL) { 
				mystr4=(char *)malloc(sizeof(char*)*20);
				memset(mystr4,0,20);
				sprintf(mystr4," ");
			}
		
			mystr5=(char *)malloc(sizeof(char*)*((strlen(mystr1))+(strlen(mystr2))+(strlen(mystr3))+(strlen(mystr4))+30));
			memset(mystr5,0,(strlen(mystr1))+(strlen(mystr2))+(strlen(mystr3))+(strlen(mystr4))+30);
			
			sprintf(mystr5,"%s - %s -- %s",mystr1,mystr2,mystr3);
			PS_set_info(myvars->ps,"Title",mystr5);
			
			sprintf(mystr5,"%s",mystr4);
			PS_set_info(myvars->ps,"Subject",mystr5);
			
			sprintf(mystr5,"%s - %s -- %s    %s",mystr1,mystr2,mystr3,mystr4);
			PS_set_info(myvars->ps,"Keywords",mystr5);

#ifndef WIN32
			//passwd=(char *)malloc(sizeof(passwd));
			if(NULL==(passwd=getpwuid(getuid()))) {
				fprintf(stderr,"getpwuid failed.\n");
				//mystr5=(char *)malloc(sizeof(char*)*20);
				mystr5=(char *)realloc(mystr5,sizeof(char*)*20);
				memset(mystr5,0,20);
				sprintf(mystr5,"%s","testuser");
			}else{
				//mystr5=(char *)malloc(sizeof(char*)*((strlen(passwd->pw_name))+3));
				mystr5=(char *)realloc(mystr5,sizeof(char*)*((strlen(passwd->pw_name))+3));
				memset(mystr5,0,(strlen(passwd->pw_name))+3);
				sprintf(mystr5,"%s",passwd->pw_name);
			}
			PS_set_info(myvars->ps,"Author",mystr5);

			// The creator shoudld include the compile date of txt2ps for support purposes...
#ifndef WIN32	
			PS_set_info(myvars->ps,"Creator","OpenRDA (txt2ps.lnx)");
#else
			PS_set_info(myvars->ps,"Creator","OpenRDA (txt2ps.exe)");
			PS_set_info(myvars->ps,"Application","OpenRDA (viewpdf.exe)");
#endif
			//if((strcmp(mystr1,"UTILITIES"))==0) { free(mystr1); }
			if((strcmp(mystr2,"TXT2PS CREATE PDF"))==0) { free(mystr2); }
			if((strcmp(mystr3," "))==0) { free(mystr3); }
			if((strcmp(mystr4," "))==0) { free(mystr4); }
			free(mystr5);
			//free(passwd); 
			//if(passwd!=NULL) { free(passwd); }
#else
			PS_set_info(myvars->ps,"Author","Robert Goley");
#endif
			break;
	case 90:
			//  Handle pre page start issues with Esc Commands consistently.
			//  This option was replaced by the Parse and Process pre page cmd functions.
				if(myvars->pagesizedef == 0) {
					ExecEscCmd(101,myvars);
				}else if(myvars->pagesizedef == 1) {
					ExecEscCmd(101,myvars);
				}else if(myvars->pagesizedef == 2) {
					ExecEscCmd(102,myvars);
				}else if(myvars->pagesizedef == 3) {
					ExecEscCmd(103,myvars);
				}else if(myvars->pagesizedef == 4) {
					ExecEscCmd(104,myvars);
				}else if(myvars->pagesizedef == 5) {
					ExecEscCmd(105,myvars);
				}else if(myvars->pagesizedef == 6) {
					ExecEscCmd(106,myvars);
				}else if(myvars->pagesizedef == 7) {
					ExecEscCmd(107,myvars);
				}else if(myvars->pagesizedef == 8) {
					ExecEscCmd(108,myvars);
				}
				
				if(myvars->printstyle == 1) {
					ExecEscCmd(001,myvars);
				} else if(myvars->printstyle == 2) {
					ExecEscCmd(002,myvars);
				}


			break;
	case 101:
			//NEW_PAGE_8.5x11_PORTRAIT  (LETTER)

		//	fprintf(stderr,"P [%s]",PS_list_parameters);
		//	fprintf(stderr,"V [%s]",PS_list_values);
		//	fprintf(stderr,"R [%s]",PS_list_resources);
			
			PS_begin_page(myvars->ps,612,792);
			myvars->pagewidth=612;
			myvars->pageheight=792;
			myvars->pgcounter++;
			myvars->pgopen=1;
			
			//printf(" In function Before: Pointers for texttx and textty %ul %ul\n", (unsigned long) textx_pt,texty_pt);
			//printf(" In function Before: Values for texttx and textty %5.2f %5.2f\n", *textx_pt,*texty_pt);
			
			if(myvars->rdapclborder) {
				 myvars->textx = 18.0; 
				 myvars->texty = 748.0; 
				 myvars->lmargin = 18.0; 
			} else {
				 myvars->textx = 18.0; 
				 myvars->texty = 765.0; 
			}
			 //*textx_pt = 18.0; 
			 //*texty_pt = 765.0; 

			//printf(" In function After: Pointers for texttx and textty %ul %ul\n", (unsigned long) textx_pt,texty_pt);
			//printf(" In function After: Values for texttx and textty %5.2f %5.2f\n", *textx_pt,*texty_pt);
			ProcessPrePageCmds(myvars);
			break;
	case 102:
			//NEW_PAGE_8.5x11_LANDSCAPE  (LETTER)
			
			PS_begin_page(myvars->ps,792,612);
			myvars->pagewidth=792;
			myvars->pageheight=612;
			myvars->pgcounter++;
			myvars->pgopen=1;
			if(myvars->rdapclborder) {
				 myvars->textx = 18.0; 
				 myvars->texty = 568.0; 
				 myvars->lmargin = 18.0; 
			} else {
				 myvars->textx = 18.0; 
				 myvars->texty = 585.0; 
			}
			ProcessPrePageCmds(myvars);
			break;
	case 103:
			//NEW_PAGE_8.5x14_PORTRAIT  (LEGAL)
			PS_begin_page(myvars->ps,612,1008);
			myvars->pagewidth=612;
			myvars->pageheight=1008;
			myvars->pgcounter++;
			myvars->pgopen=1;
			if(myvars->rdapclborder) {
				 myvars->textx = 18.0; 
				 myvars->texty = 964.0; 
				 myvars->lmargin = 18.0; 
			} else {
				 myvars->textx = 18.0; 
				 myvars->texty = 981.0; 
			}
			ProcessPrePageCmds(myvars);
			break;
	case 104:
			//NEW_PAGE_8.5x14_LANDSCAPE  (LEGAL)
			
			PS_begin_page(myvars->ps,1008,612);
			myvars->pagewidth=1008;
			myvars->pageheight=612;
			myvars->pgcounter++;
			myvars->pgopen=1;
			if(myvars->rdapclborder) {
				 myvars->textx = 18.0; 
				 myvars->texty = 568.0; 
				 myvars->lmargin = 18.0; 
			} else {
				 myvars->textx = 18.0; 
				 myvars->texty = 585.0; 
			}
			ProcessPrePageCmds(myvars);
			break;
	case 105:
			//NEW_PAGE_11x14_PORTRAIT  (POSTER)
			PS_begin_page(myvars->ps,792,1008);
			myvars->pagewidth=792;
			myvars->pageheight=1008;
			myvars->pgcounter++;
			myvars->pgopen=1;
			if(myvars->rdapclborder) {
				 myvars->textx = 18.0; 
				 myvars->texty = 964.0; 
				 myvars->lmargin = 18.0; 
			} else {
				 myvars->textx = 18.0; 
				 myvars->texty = 981.0; 
			}
			ProcessPrePageCmds(myvars);
			break;
	case 106:
			//NEW_PAGE_11x14_LANDSCAPE  (POSTER)
			PS_begin_page(myvars->ps,1008,792);
			myvars->pagewidth=1008;
			myvars->pageheight=792;
			myvars->pgcounter++;
			myvars->pgopen=1;
			if(myvars->rdapclborder) {
				 myvars->textx = 18.0; 
				 myvars->texty = 748.0; 
				 myvars->lmargin = 18.0; 
			} else {
				 myvars->textx = 18.0; 
				 myvars->texty = 765.0; 
			}
			ProcessPrePageCmds(myvars);
			break;
	case 107:
			//NEW_PAGE_11x17_PORTRAIT  (LEDGER)
			PS_begin_page(myvars->ps,792,1224);
			myvars->pagewidth=792;
			myvars->pageheight=1224;
			myvars->pgcounter++;
			myvars->pgopen=1;
			if(myvars->rdapclborder) {
				 myvars->textx = 18.0; 
				 myvars->texty = 1180.0; 
				 myvars->lmargin = 18.0; 
			} else {
				 myvars->textx = 18.0; 
				 myvars->texty = 1197.0; 
			}
			ProcessPrePageCmds(myvars);
			break;
	case 108:
			//NEW_PAGE_11x17_LANDSCAPE  (LEDGER)
			PS_begin_page(myvars->ps,1224,792);
			myvars->pagewidth=1224;
			myvars->pageheight=792;
			myvars->pgcounter++;
			myvars->pgopen=1;
			if(myvars->rdapclborder) {
				 myvars->textx = 18.0; 
				 myvars->texty = 748.0; 
				 myvars->lmargin = 18.0; 
			} else {
				 myvars->textx = 18.0; 
				 myvars->texty = 765.0; 
			}
			ProcessPrePageCmds(myvars);
			break;
	case 110:
			//PS_ENDPAGE
			PS_end_page(myvars->ps);
			myvars->pgopen=0;
			break;
	case 111:
			//PS_BEGIN_FILE
			//PS_boot();
			//PS_mp_init();
			myvars->ps = PS_new();
			break;
	case 112:
			//PS_ENDFILE
			PS_close(myvars->ps);
			break;
	case 113:
			//PS_ENDFILE
			PS_delete(myvars->ps);
			break;
	case 115:
			//SET_FONT
			myvars->font_array[12] = PS_findfont(myvars->ps, "Helvetica", "", 0);
			myvars->font_array[0] = myvars->font_array[12];
			break;
	case 120:
			//SET_FONTSIZE
			PS_setfont(myvars->ps,myvars->font_array[10],12.0);
			myvars->font_array[11] = myvars->font_array[0];
			myvars->font_array[0] = myvars->font_array[10];
			break;
	case 121:
			// This function will print the current 
			// string and compute the length of that
			// string and increment textx.  It will
			// then change the default font in the array 
			// and send the pslib command to do the same.
			// It will need to clear the string variable.

			//PS_show_xy(ps,string,*textx_pt,*texty_pt);
			myvars->font_array[10] = PS_findfont(myvars->ps,"MRVCode39extSA","",1);
			PS_setfont(myvars->ps,myvars->font_array[10],12.0);
			myvars->font_array[11] = myvars->font_array[0];
			myvars->font_array[0] = myvars->font_array[10];
			
			break;
	case 200:
			//IMP_PS_IMAGE
			myvars->psimage[0] = PS_open_image_file(myvars->ps,"png","letter.ps", NULL, 0);
			break;
	case 201:
			//IMP_PS_IMAGE
			myvars->psimage[0] = PS_open_image_file(myvars->ps,"jpg","letter.ps", NULL, 0);
			break;
	case 202:
			//IMP_PS_IMAGE
			myvars->psimage[0] = PS_open_image_file(myvars->ps,"eps",myvars->cmdstr, NULL, 0);
			break;
	case 220:
			//Create a page template using an image to 
			//create smaller more efficient output
			//using known psimage id.  PORTRAIT
			//pstmpl=PS_begin_template(ps,"float width","float height")
			//myvars->pstmpl=PS_begin_template(myvars->ps,596,842);
			myvars->pstmpl[0]=PS_begin_template(myvars->ps,596,842);
			PS_place_image(myvars->ps, myvars->psimage[0], 0, 0, 1.0);
			PS_end_template(myvars->ps);
			break;
	case 221:
			//Create a page template using an image to 
			//create smaller more efficient output
			//using new psimage from file PORTRAIT
			myvars->psimage[0] = PS_open_image_file(myvars->ps,"eps","/tmp/letter.ps",NULL,0);
			myvars->pstmpl[1]=PS_begin_template(myvars->ps,612,792);
			PS_place_image(myvars->ps,myvars->psimage[0],0,0,1.0);
			PS_end_template(myvars->ps);
			break;
	case 222:
			PS_place_image(myvars->ps,myvars->pstmpl[0],0,0,1.0);
			break;
	case 240:
			// This function moves the cursor to a fixed or relative horizontal position 
			// based on an equivalency with PCL dot size.

			if(myvars->cmdstr==NULL) { break; }
			ExecEscCmd(301,myvars);
			sprintf(myvars->string, "%c",0);
			switch (myvars->cmdstr[0]){
			case '-':
					memset(temparray,0,32);
					strcpy(temparray,myvars->cmdstr+1);
					myfloat=(atof(temparray));
					myvars->textx=(myvars->textx - (myfloat * 0.244));
					break;
			case '+':
					memset(temparray,0,32);
					strcpy(temparray,myvars->cmdstr+1);
					myfloat=(atof(temparray));
					myvars->textx=(myvars->textx + (myfloat * 0.244));
					break;
			default:
					myvars->pagewidth;
					myfloat=(atof(myvars->cmdstr));
					myvars->textx=(myfloat * 0.244);
					break;
			}

			if(myvars->cmdstr==NULL) { fprintf(stderr,"FUNC 240   cmdstr [%s]  myfloat [%8.4f]  temparray [%s]\n",myvars->cmdstr,myfloat,temparray); }
			break;
	case 241:
			// This function moves the cursor to a fixed or relative vertical posistion 
			// based on an equivalency with PCL dot size.
                        //myvars->pageheight=792;
			
			if(myvars->cmdstr==NULL) { break; }
			ExecEscCmd(301,myvars);
			sprintf(myvars->string, "%c",0);
			switch (myvars->cmdstr[0]){
			case '-':
					memset(temparray,0,32);
					strcpy(temparray,myvars->cmdstr+1);
					myfloat=(atof(temparray));
					myvars->textx=(myvars->texty + (myfloat * 0.2428));
					break;
			case '+':
					memset(temparray,0,32);
					strcpy(temparray,myvars->cmdstr+1);
					myfloat=(atof(temparray));
					myvars->texty=(myvars->texty - (myfloat * 0.2428));
					break;
			default:
					myfloat=(atof(myvars->cmdstr));
					myvars->texty=(myvars->pageheight - (myfloat * 0.2428));
					break;
			}

			if(myvars->cmdstr==NULL) { fprintf(stderr,"FUNC 241   cmdstr [%s]  myfloat [%8.4f]  temparray [%s]\n",myvars->cmdstr,myfloat,temparray); }
			break;
	case 242:
			// This function moves the cursor to a fixed or relative horizontal position 
			// based on postscript signed float values.

			if(myvars->cmdstr==NULL) { break; }
			switch (myvars->cmdstr[0]){
			case '-':
					memset(temparray,0,32);
					strcpy(temparray,myvars->cmdstr+1);
					myfloat=(atof(temparray));
					myvars->textx=(myvars->textx - myfloat);
					break;
			case '+':
					memset(temparray,0,32);
					strcpy(temparray,myvars->cmdstr+1);
					myfloat=(atof(temparray));
					myvars->textx=(myvars->textx + myfloat);
					break;
			default:
					myfloat=(atof(myvars->cmdstr));
					myvars->textx=myfloat;
					break;
			}

			break;
	case 243:
			// This function moves the cursor to a fixed or relative vertical posistion 
			// based on postscript signed float values.

			if(myvars->cmdstr==NULL) { break; }
			switch (myvars->cmdstr[0]){
			case '-':
					memset(temparray,0,32);
					strcpy(temparray,myvars->cmdstr+1);
					myfloat=(atof(temparray));
					myvars->texty=(myvars->textx - myfloat);
					break;
			case '+':
					memset(temparray,0,32);
					strcpy(temparray,myvars->cmdstr+1);
					myfloat=(atof(temparray));
					myvars->texty=(myvars->textx + myfloat);
					break;
			default:
					myvars->pagewidth;
					myfloat=(atof(myvars->cmdstr));
					myvars->texty=(myvars->pageheight - myfloat);
					break;
			}

			break;
	case 250:
			// Load Image Like Similar TO Load PCL MACRO.
			// May want to free the previously loaded images before loading the same number again
			if(strlen(myvars->cmdstr)==0) { break; } 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"Loading MACRO from [%s]  \n",mystr1); }
#endif /* USE_RDA_DIAGNOSTICS */
			//printf("pgopen=[%d]  \n",myvars->pgopen);
			//if(myvars->pgopen==0) {	ExecEscCmd(90,myvars);	}
			//printf("pgopen=[%d]  \n",myvars->pgopen);
			mystr1=strtok(myvars->cmdstr,":");
			mystr2=strtok(NULL,":");
			if((strncmp(mystr2+((strlen(mystr2)) - 4),".eps",4))==0) {
				myvars->psimage[(atoi(mystr1))]=PS_open_image_file(myvars->ps,"eps",mystr2,NULL,(atoi(mystr1)));
			} else if((strncmp(mystr2+((strlen(mystr2)) - 3),".ps",3))==0) {
				myvars->psimage[(atoi(mystr1))]=PS_open_image_file(myvars->ps,"eps",mystr2,NULL,(atoi(mystr1)));
			} else if((strncmp(mystr2+((strlen(mystr2)) - 4),".png",4))==0) {
				myvars->psimage[(atoi(mystr1))]=PS_open_image_file(myvars->ps,"png",mystr2,NULL,(atoi(mystr1)));
			} else if((strncmp(mystr2+((strlen(mystr2)) - 4),".jpg",4))==0) {
				myvars->psimage[(atoi(mystr1))]=PS_open_image_file(myvars->ps,"jpeg",mystr2,NULL,(atoi(mystr1)));
			} else if((strncmp(mystr2+((strlen(mystr2)) - 5),".jpeg",5))==0) {
				myvars->psimage[(atoi(mystr1))]=PS_open_image_file(myvars->ps,"jpeg",mystr2,NULL,(atoi(mystr1)));
			} else if((strncmp(mystr2+((strlen(mystr2)) - 4),".tif",4))==0) {
				myvars->psimage[(atoi(mystr1))]=PS_open_image_file(myvars->ps,"tiff",mystr2,NULL,(atoi(mystr1)));
			} else if((strncmp(mystr2+((strlen(mystr2)) - 5),".tiff",5))==0) {
				myvars->psimage[(atoi(mystr1))]=PS_open_image_file(myvars->ps,"tiff",mystr2,NULL,(atoi(mystr1)));
			}
			//printf("str1=[%s]  str2=[%s] str3=[%s] str4=[%s] str5=[%s] \n",mystr1,mystr2,mystr3,mystr4,mystr5);
			//printf("psimage=%d\n",myvars->psimage[(atoi(mystr1))]);
			break;
	case 251:
			// Print Image Like PCL Call MACRO 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"Using MACRO...\n"); }
#endif /* USE_RDA_DIAGNOSTICS */
			if(strlen(myvars->cmdstr)==0) { break; } 
			//printf("pgopen=[%d]  \n",myvars->pgopen);
			//if(myvars->pgopen==0) {	ExecEscCmd(90,myvars);	}
			//printf("pgopen=[%d]  \n",myvars->pgopen);
			mystr1=strtok(myvars->cmdstr,":");
			mystr2=strtok(NULL,":");
			mystr3=strtok(NULL,":");
			//printf("pgopen=[%d]  str1=[%s]  str2=[%s] str3=[%s] str4=[%s] str5=[%s] \n",myvars->pgopen,mystr1,mystr2,mystr3,mystr4,mystr5);
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"Using MACRO [%d]  \n",(atoi(mystr1))); }
#endif /* USE_RDA_DIAGNOSTICS */
			PS_place_image(myvars->ps,myvars->psimage[(atoi(mystr1))],(atoi(mystr2)),(atoi(mystr3)),1.0);
			break;
	case 252:
			//PRT_PS_IMAGE
			PS_place_image(myvars->ps,myvars->psimage[0],0,0,1.0);
			break;
	case 253:
			//PRT_PS_IMAGE
			//printf("psimage=%d\n",myvars->psimage[0]);
			//printf("cmdstr=%s\n",myvars->cmdstr);
			mystr1=strtok(myvars->cmdstr,":");
			mystr2=strtok(NULL,":");
			mystr3=strtok(NULL,":");
			mystr4=strtok(NULL,":");
			mystr5=strtok(NULL,":");
			//printf("str1=[%s]  str2=[%s] str3=[%s] str4=[%s] str5=[%s] \n",mystr1,mystr2,mystr3,mystr4,mystr5);
			
			if(myvars->psimage[0]==0) {
				myvars->psimage[0] = PS_open_image_file(myvars->ps,"eps","/tmp/letter.ps",NULL,0);
			} 
			PS_place_image(myvars->ps,myvars->psimage[0],0,0,1.0);
			//printf("psimage=%d\n",myvars->psimage[0]);
			break;
	case 260:
			// Load Image Like Similar TO Load PCL MACRO.
			if(strlen(myvars->cmdstr)==0) { break; } 

			mystr1=strtok(myvars->cmdstr,":");
			mystr2=strtok(NULL,":");
			mystr3=strtok(NULL,":");
			mystr4=strtok(NULL,":");

			DecryptDataFromFile(mystr2,mystr3,&mystr4,&size);
			if(size!=0) {
				myvars->psimage[(atoi(mystr1))]=PS_open_image(myvars->ps,"eps",NULL,mystr4,size,0,0,0,0,NULL);
			}
			if(mystr4!=NULL) free(mystr4);
			//fprintf(stderr,"PS_open_image result [%d]\n",myvars->psimage[(atoi(mystr1))]);

			//printf("str1=[%s]  str2=[%s] str3=[%s] str4=[%s] str5=[%s] \n",mystr1,mystr2,mystr3,mystr4,mystr5);
			//printf("psimage=%d\n",myvars->psimage[(atoi(mystr1))]);
			break;
	case 300:
			PS_stringwidth(myvars->ps,myvars->string,myvars->psfont,12);
			//fprintf(stderr,"PRT_TEXT_XY STRLEN=%s\n",(PS_stringwidth(ps,string,psfont,12)));
			//PRT_TEXT_XY
			PS_show_xy(myvars->ps,myvars->string,300,300);
			break;
	case 301:
			//fprintf(stderr,"PRT_TEXT_XY STRLEN=%d\n",(PS_stringwidth(ps,string,psfont,7)));
			//PRT_TEXT_XY
			//fprintf(stderr,"X[%f} Y[%f] MY_X[%d] MY_Y[%d]",*textx_pt,*texty_pt,my_textx,my_texty);
			//PS_show_xy(ps,string,&textx,&texty);
			//fprintf(stderr,"StringWidth=%5.2f\n",PS_stringwidth(ps,string,font_array[0],12));
			
			PS_show_xy(myvars->ps,myvars->string,myvars->textx,myvars->texty);
			
			//PS_show_xy(myvars->ps,myvars->string,myvars->*textx_pt,myvars->*texty_pt);


/* TODO: Test case for switching fonts on same line only works with one text line.
 
			myfloat=PS_stringwidth(ps,string,font_array[0],12);
			*textx_pt=(*textx_pt + myfloat);
			
			// Calculate the size of one normal char
			fprintf(stderr,"CharWidth=%5.2f\n",PS_stringwidth(ps,"1",font_array[0],12));
			

			
			font_array[10] = PS_findfont(ps,"MRVCode39extSA","",1);
			PS_setfont(ps,font_array[10],12.0);
			font_array[11] = font_array[0];
			font_array[0] = font_array[10];
			fprintf(stderr,"CharWidth=%5.2f\n",PS_stringwidth(ps,"1",font_array[0],12));
			PS_show_xy(ps,string,*textx_pt,*texty_pt);
			
			font_array[0] = font_array[11];
			PS_setfont(ps,font_array[0],12.0);
			myfloat=PS_stringwidth(ps,string,font_array[0],12);
			*textx_pt=(*textx_pt + myfloat + (PS_stringwidth(ps,"1",font_array[0],12))*2);
			
			PS_show_xy(ps,string,*textx_pt,*texty_pt);
			*textx_pt=(*textx_pt - myfloat);
			*textx_pt=(*textx_pt - myfloat);
*/
			break;
	case 310:
 			// STUB* Variable Name: [RESET PRINTER] 
			// Not sure if this should be implemented or ignored yet.
			// It may need to delete all macros or fonts for compatibility.  It is not used much though...
			break;
	case 311:
 			// STUB* Variable Name: [SIGNATURE DIMM 1] 
			// This is slowly going away.  Probably can be ignored.
			break;
	case 312:
			// STUB* Variable Name: [SIGNATURE DIMM A] 
			// This is slowly going away.  Probably can be ignored.
			break;
	case 313:
			// STUB* Variable Name: [PUSH CURSOR] 
			// PCL Push equiv command
		//	if(myvars->pgopen==0) {
				// This needs to take the page size into account instead of just LETTER/PORTRAIT
				// May not be needed now that prepage commands are processed post page start.
		//		 myvars->pushpopx=18.0; 
		//		 myvars->pushpopy=748.0; 
		//	} else {
				myvars->pushpopx=myvars->textx;
				myvars->pushpopy=myvars->texty;
		//	}
			break;
	case 314:
			// STUB* Variable Name: [POP CURSOR] 
			// PCL Pop equiv command

			// This section added to address X/Y coordinate hopping to ensure 
			// text for previous coordinates prints at correct location when 
			// POP is at the end of a line.  May need to change, testing needed.
			ExecEscCmd(301,myvars);
			sprintf(myvars->string, "%c",0);
			//
			
			myvars->textx=myvars->pushpopx;
			myvars->texty=myvars->pushpopy;
			break;
	case 315:
			// Variable Name: [NORMAL TEXT] 
			//if(myvars->pgopen==0) {	ExecEscCmd(90,myvars);	}
			if(myvars->pgopen==0) {
				myvars->printstyle=1;
			} else {	
				ExecEscCmd(001,myvars);
			}
				
			break;
	case 316:
			// Variable Name: [COMPRESSED TEXT] 
			//if(myvars->pgopen==0) {	ExecEscCmd(90,myvars);	}
			if(myvars->pgopen==0) {
				myvars->printstyle=2;
			} else {	
				ExecEscCmd(002,myvars);
			}
			break;
	case 317:
			// STUB* Variable Name: [ELITE PRINT] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: ELITE PRINT Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 318:
			// STUB* Variable Name: [PAGE_SIZE_LETTER] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: PAGE_SIZE_LETTER Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			if(myvars->landscape==1){
				myvars->pagesizedef=2;
			}else{
				myvars->pagesizedef=1;
			}

			break;
	case 319:
			// STUB* Variable Name: [PAGE_SIZE_LEGAL] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: PAGE_SIZE_LEGAL Not Implemented [%d].\n",myvars->rda_exec_cmd);}
#endif /* USE_RDA_DIAGNOSTICS */
			if(myvars->landscape==1){
				myvars->pagesizedef=4;
			}else{
				myvars->pagesizedef=3;
			}
			break;
	case 320:
			// STUB* Variable Name: [PAGE_SIZE_LEDGER] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: PAGE_SIZE_LEDGER Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			if(myvars->landscape==1){
				myvars->pagesizedef=8;
			}else{
				myvars->pagesizedef=7;
			}
			break;
	case 321:
			// STUB* Variable Name: [PAGE_SIZE_POSTER] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: PAGE_SIZE_POSTER Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			if(myvars->landscape==1){
				myvars->pagesizedef=6;
			}else{
				myvars->pagesizedef=5;
			}
			break;
	case 330:
			// STUB* Variable Name: [PORTRAIT] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: PORTRAIT Partially Implemented for Letter Sized Paper [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			myvars->landscape=0;
			switch (myvars->pagesizedef) {
				case 2:
				case 4:
				case 6:
				case 8:
					--myvars->pagesizedef;
					// Hopefully, this will work for all defined page types.  
					break;
				default:
					// This should ignore any other page size changes for non landscape page sizes.
					break;
			}
			break;
	case 331:
			// STUB* Variable Name: [LANDSCAPE] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: LANDSCAPE Partially Implemented for Letter Sized Paper [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			myvars->landscape=1;
			switch (myvars->pagesizedef) {
				case 0:
					++myvars->pagesizedef;
					++myvars->pagesizedef;
					break;
				case 1:
				case 3:
				case 5:
				case 7:
					++myvars->pagesizedef;
					// Hopefully, this will work for all defined page types.  
					break;
				default:
					// This should ignore any other page size changes for non portrait page sizes.
					break;
			}
			break;
	case 332:
			// STUB* Variable Name: [SIMPLEX] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: SIMPLEX Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 333:
			// STUB* Variable Name: [DUPLEX LONG] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: DUPLEX LONG Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 334:
			// STUB* Variable Name: [DUPLEX SHORT] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: DUPLEX SHORT Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 335:
			// STUB* Variable Name: [REVERSE PORTRAIT] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: REVERSE PORTRAIT Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 336:
			// STUB* Variable Name: [REVERSE LANDSCAPE] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: REVERSE LANDSCAPE Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 337:
			// STUB* Variable Name: [ASCII SQUARE] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: ASCII SQUARE Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 338:
			// STUB* Variable Name: [ESCAPE] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: ESCAPE Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 339:
			// STUB* Variable Name: [HALF LINE FEED] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: HALF LINE FEED [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			//fprintf(stderr,"pgopen [%d],  nlcounter [%d],  texty [%5.2f]\n",myvars->pgopen,myvars->nlcounter,myvars->texty);
			if(myvars->nlcounter==1) {
				if(myvars->printstyle == 1) {
					ExecEscCmd(001,myvars);
				} else if(myvars->printstyle == 2) {
					ExecEscCmd(002,myvars);
				}
				myvars->texty=(myvars->texty - 6);
			}else{
				ExecEscCmd(301,myvars);
				sprintf(myvars->string, "%c",0);
				myvars->texty=(myvars->texty - 6);
			}
			//myvars->texty=(myvars->texty - 6);

			// Not sure what the actual postscript pixel value is for a half line feed.  Testing...
			//myvars->texty=(myvars->texty - 9);
			
			//myvars->texty=(myvars->texty - 12);
			
			//myvars->texty=(myvars->texty - myvars->line_space[0]);

			break;
	case 340:
			// STUB* Variable Name: [VENTREAS] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: VENTREAS Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 341:
			// STUB* Variable Name: [PAYREG] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: PAYREG Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 342:
			// STUB* Variable Name: [POCERT] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: POCERT Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 343:
			// STUB* Variable Name: [PAYCKRG] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: PAYCKRG Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 344:
			// STUB* Variable Name: [VENCKRG] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: VENCKRG Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 345:
			// STUB* Variable Name: [SIGNATURE LOCK] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: SIGNATURE LOCK Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 346:
			// STUB* Variable Name: [SIGNATURE UNLOCK] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: SIGNATURE UNLOCK Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 347:
			// STUB* Variable Name: [POAUDIT] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: POAUDIT Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 348:
			// STUB* Variable Name: [SIGNATURE FILE] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: SIGNATURE FILE Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 349:
			// * Variable Name: [UNLOAD_SOFTFONTS] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: UNLOAD_SOFTFONTS Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 350:
			// * Variable Name: [RESET FONT] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: RESET FONT Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 351:
			// * Variable Name: [LOAD VENPMT SIGNATURE] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: LOAD VENPMT SIGNATURE Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			// This will load all available rda-ven[123456789].dat files as separate 
			// signature images if the files exist. File checks will be done with 
			// the stat() function.The image numbers will correspond with the file 
			// number.  The passed string will be XY coordinates for the image placement.
			
			
			// This section may be irrelevant now that code 260 was added.  
			// This part of the code was added to the calling program
			// due to the need to pass password credentials to decrypt.
			if(!FileExists(NULL,"rda-ven1.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-ven2.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-ven3.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-ven4.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-ven5.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-ven6.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-ven7.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-ven8.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-ven9.dat")) { TRACE; }
			break;
	case 352:
			// * Variable Name: [LOAD PAYROLL SIGNATURE] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: LOAD PAYROLL SIGNATURE Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			// This will load all available rda-pay[123456789].dat files as separate 
			// signature images if the files exist. File checks will be done with 
			// the stat() function.The image numbers will correspond with the file 
			// number.  The passed string will be XY coordinates for the image placement.
			// This will need to pass the password as well...
			
			
			// This section may be irrelevant now that code 260 was added.  
			// This part of the code was added to the calling program
			// due to the need to pass password credentials to decrypt.
			if(!FileExists(NULL,"rda-pay1.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-pay2.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-pay3.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-pay4.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-pay5.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-pay6.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-pay7.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-pay8.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-pay9.dat")) { TRACE; }

			break;
	case 353:
			// * Variable Name: [VENPMT SIGNATURE] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: VENPMT SIGNATURE Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			// This will call a general function with one argument only.  The argument will 
			// be a series of single digit numbers.  This is the number of the signature
			// loaded in the correct order to print the signature.  If not corresponding 
			// XY coordinates are set, the signature will not be printed.
			break;
	case 354:
			// * Variable Name: [PAYROLL SIGNATURE] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: PAYROLL SIGNATURE Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			// This will call a general function with one argument only.  The argument will 
			// be a series of single digit numbers.  This is the number of the signature
			// loaded in the correct order to print the signature.  If not corresponding 
			// XY coordinates are set, the signature will not be printed.
			break;
	case 355:
			// * Variable Name: [DIRECT DEPOSIT] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: DIRECT DEPOSIT Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 356:
			// * Variable Name: [PAYROLL CHECK] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: PAYROLL CHECK Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 357:
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 358:
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 359:
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: Not Implemented [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 360:
			// STUB* Variable Name: [NEG Q LINE FEED] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: NEG Q LINE FEED [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			//fprintf(stderr,"pgopen [%d],  nlcounter [%d],  texty [%5.2f]\n",myvars->pgopen,myvars->nlcounter,myvars->texty);
			if(myvars->nlcounter==1) {
				if(myvars->printstyle == 1) {
					ExecEscCmd(001,myvars);
				} else if(myvars->printstyle == 2) {
					ExecEscCmd(002,myvars);
				}
				myvars->texty=(myvars->texty + 3);
			}else{
				ExecEscCmd(301,myvars);
				sprintf(myvars->string, "%c",0);
				myvars->texty=(myvars->texty + 3);
			}
			break;
	case 361:
			// STUB* Variable Name: [POS Q LINE FEED] 
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STUB: POS Q LINE FEED [%d].\n",myvars->rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			//fprintf(stderr,"pgopen [%d],  nlcounter [%d],  texty [%5.2f]\n",myvars->pgopen,myvars->nlcounter,myvars->texty);
			if(myvars->nlcounter==1) {
				if(myvars->printstyle == 1) {
					ExecEscCmd(001,myvars);
				} else if(myvars->printstyle == 2) {
					ExecEscCmd(002,myvars);
				}
				myvars->texty=(myvars->texty - 3);
			}else{
				ExecEscCmd(301,myvars);
				sprintf(myvars->string, "%c",0);
				myvars->texty=(myvars->texty - 3);
			}
			break;
	case 399:
			//fprintf(stderr,"%s",cmdstr[4],cmdstr[5]);
			
			//fprintf(stderr,"STRLEN[%d]  %c\n",strlen(cmdstr),cmdstr[3]);
			break;
	default: 
			if(myvars->debug) { fprintf(stderr,"Unknown Command [%d].\n",myvars->rda_exec_cmd); }
			break;
	}

	// Free resources only used in this function
	mystr1=NULL;
	mystr2=NULL;
	mystr3=NULL;
	mystr4=NULL;
	mystr5=NULL;
	//if(mystr1!=NULL) free(mystr1);
	//if(mystr2!=NULL) free(mystr2);  
	//if(mystr3!=NULL) free(mystr3);
	//if(mystr4!=NULL) free(mystr4);
	//if(mystr5!=NULL) free(mystr5);
	
}

size_t writeproc(PSDoc *p, void *data, size_t size) { return fwrite(data, 1, size, stdout); }

size_t gswriteproc(PSDoc *p, void *data, size_t size) { 
	size_t mysize=0;
	int myerror=0;
	unsigned int diff=0;
	unsigned int s=0;
	void *mydata=NULL;
	int bufsize=65535;
	
	//fprintf(stderr,"test3 [%d]  test4[%d]\n",fwrite(data,1,size,test3),fwrite(data,1,strlen(data),test4));
	if(size>65535) {
		//mydata=(char *)malloc(sizeof(char*)*(bufsize+1));
		mydata=malloc(bufsize+1);
		memset(mydata,0,bufsize+1);
		while(mysize<(strlen((char *)data))) {
			diff=(size - mysize);
			if(diff<bufsize) {
				break;
			}else{
				memcpy(mydata,(char *)data+mysize,bufsize);
				s=strlen((char *)mydata);
				mysize=(mysize+s);
				myerror=gsapi_run_string_continue(gsvars->minst,(const char *)mydata,s, 0, &gsvars->exit_code);
			}
			//fprintf(stderr,"strncpy size [%d]",strlen(memcpy(mydata,data+mysize,bufsize)));;
			//fprintf(stderr,"fwrite2 [%d] size [%d]  mysize [%d]  mydata [%d]\n",myerror,size,mysize,strlen(mydata));

			//myerror=gsapi_run_string_continue(gsvars->minst,mydata,strlen(mydata), 0, &gsvars->exit_code);
				
			//mysize=(mysize+(strlen(mydata)));
		}
		//fprintf(stderr,"size [%d]  bufsize  [%d]  mysize  [%d]  diff [%d]\n",size,bufsize,mysize,diff);
		memset(mydata,0,bufsize+1);
		memcpy(mydata,(char *)data+mysize,diff);
		mysize=(mysize+diff);
		myerror=gsapi_run_string_continue(gsvars->minst,(const char *)mydata,diff, 0, &gsvars->exit_code);
		free(mydata);
		
	}else{
		myerror=gsapi_run_string_continue(gsvars->minst,(const char *)data,size,0,&gsvars->exit_code);
	}
	return (size);
}
	

//#define LEFT_BORDER 50

#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	FILE *fp;
	FILE *MYFP;
	struct stat fst;
	size_t bytes_read;
	int n=0;
	int x=0,y=0;
	int readsize=0;
	char *tempstr;
	char inputarray[65535];
	RDA_PS_VARS *myvars=NULL;
	char cwd[256];

#ifdef WIN32
	BOOL fConnected;
	//LPTSTR lpszPipename = "\\\\.\\pipe\\SamplePipe";
	LPTSTR lpszPipename=NULL;
	CHAR chRequest[BUFSIZE];
	DWORD cbBytesRead;
	BOOL fSuccess;
	HANDLE hPipe;

	char *win32username=NULL;
	char usernametemp[101];
	
	FreeConsole(); 

	win32username=getenv("USERNAME");
	if(win32username!=NULL) {
		fclose(stderr);
		memset(usernametemp,0,101);
		sprintf(usernametemp,"%s.ERR",win32username);
		freopen(usernametemp, "a+b", stderr);
	}
#endif

#ifdef USE_CPU_PROFILER
	ProfilerStart("viewpdf.prof");
#endif

	myvars=(RDA_PS_VARS *)malloc(sizeof(RDA_PS_VARS));
	gsvars=(MYGSVARS *)malloc(sizeof(MYGSVARS));

	// This section sets program defaults 
	// These can be overriden by command lines options
	// or by the options passed to the command interpreter
	myvars->ps=NULL;
	myvars->psfont=0;
	myvars->string=NULL;
	myvars->cmdstr=NULL;
	myvars->string=(char *)malloc(sizeof(char*)*512);
	myvars->cmdstr=(char *)malloc(sizeof(char*)*512);
	memset(myvars->string,0,512);
	memset(myvars->cmdstr,0,512);
	myvars->esc_cmd=0;
	myvars->rda_exec_cmd=0;
	myvars->textx=0.0;
	myvars->texty=0.0;
	myvars->font_size=0;
	myvars->font_height=0;
	myvars->formfeed=0;
	myvars->pagesizedef=0;
	myvars->landscape=0;
	myvars->viewfile=1;
	myvars->removefile=1;
	for(x=0;x<10000;x++) { myvars->psimage[x]=0; }
	for(x=0;x<10000;x++) { myvars->pstmpl[x]=0; }
	myvars->rdapclborder=1;
	for(x=0;x<150;x++) { myvars->font_array[x]=0; }
	for(x=0;x<4;x++) { myvars->line_space[x]=0; }
	for(x=0;x<150;x++) { myvars->font_options[x]=0; }
	myvars->font_set=0;
	// These are needed for for ghostscript to use the right page.  Does not allow for changing pagesize mid input at the moment.
	// Not used anymore, might be used later to handle mtnmst browse printing...
	myvars->pagewidth=0;
	myvars->pageheight=0;

	
#ifdef WIN32
	myvars->named_pipe=NULL;
	myvars->named_pipe=getenv("VIEWPDF_PIPE");
	if(myvars->debug) { fprintf(stderr,"VIEWPDF_PIPE ENV [%s]\n",myvars->named_pipe); }
#endif
	myvars->input=NULL;
	myvars->output=NULL;
	
	myvars->pushpopx=0;
	myvars->pushpopy=0;
	myvars->nlcounter=0;
	myvars->pgcounter=0;
	myvars->pgopen=0;
	myvars->printstyle=1;
	myvars->prepagecount=0;

	for(x=0;x<150;x++) { 
		myvars->prepagecmd[x]=0; 
		myvars->prepagecmdstr[x]=NULL;
		myvars->prepagecmdstr[x]=(char *)malloc(sizeof(char *)*512);
		memset(myvars->prepagecmdstr[x],0,512);
	}
	
	// Assert below here
	//myvars->cmdstr=NULL;
	//myvars->psfont=NULL;
	//assert(myvars->ps!=NULL);
	//assert(myvars->cmdstr!=NULL);


	//assert(myvars->cmdstr!=NULL);
	gsvars->use_gslib=0;
	
			/*
			if((textx = (char *)malloc(sizeof(float)))==NULL) 
			{ puts("Unable to allocate memory");
			  exit(1);
			}
			if((texty = (char *)malloc(sizeof(float)))==NULL) 
			{ puts("Unable to allocate memory");
			  exit(1);
			}
			*/
	
	tempstr=getenv("RDA_DATA_DIR");
	if(tempstr!=NULL)
	{
		chdir(tempstr);	
		tempstr==NULL;
	}

	ParseCmdLine(argc,argv,myvars);
	
#ifdef CAPTURE_INPUT
	memset(diagfilename,0,1024);
	sprintf(diagfilename,"%s-diag.txt",myvars->output);

	if(NULL==(diagfp=fopen(diagfilename,"w+b"))) {
		fprintf(stderr, "Cannot open diagnostic file\n");
		std::exit(1);
	}
#endif /* CAPTURE_INPUT */

#ifdef WIN32
	if(myvars->named_pipe!=NULL) {
		fprintf(stderr,"VIEWPDF_PIPE SIZE [%d]\n",strlen(myvars->named_pipe));
		lpszPipename=(LPTSTR)(char *)malloc((strlen(myvars->named_pipe))+15);
		sprintf(lpszPipename,"\\\\.\\pipe\\%s",myvars->named_pipe);
	}
#endif

	PS_boot();
	myvars->ps = PS_new();
	

/*  Added for file read */

#ifdef USE_RDA_DIAGNOSTICS
	if(myvars->debug) { 
		fprintf(stderr,"Var input value [%s].\n",myvars->input);
		fprintf(stderr,"Var named_pipe value [%s].\n",myvars->named_pipe);
	}
#endif /* USE_RDA_DIAGNOSTICS */

	if(myvars->input==NULL&&myvars->named_pipe==NULL) {
		fprintf(stderr,"input is stdin.\n");
		fp=stdin;

#ifdef WIN32
	}else if(myvars->input==NULL&&myvars->named_pipe!=NULL) {
		fprintf(stderr,"input is named pipe [%s].\n",myvars->named_pipe);
		hPipe=CreateNamedPipe(lpszPipename,PIPE_ACCESS_DUPLEX,PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,PIPE_UNLIMITED_INSTANCES,BUFSIZE,BUFSIZE,PIPE_TIMEOUT,NULL); 
		if (hPipe == INVALID_HANDLE_VALUE) {
			fprintf(stderr,"INVALID PIPE HANDLE\n");
			std::exit(1);
		}

		fConnected = ConnectNamedPipe(hPipe, NULL) ?
			TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
		
#endif
		/*
#ifdef WIN32
	}else if(myvars->input==NULL&&myvars->input==NULL&&myvars->named_pipe!=NULL) {
		fprintf(stderr,"input is named pipe [%s].\n",myvars->named_pipe);
		if(NULL==(fp=fopen(myvars->input,"rb"))) {
			fprintf(stderr, "Cannot open input file\n");
			exit(1);
		}
		str=(char *)malloc(sizeof(char *)*100000000);
#endif
*/
	} else {
		fprintf(stderr,"Open [%s] for input.\n",myvars->input);
		if(NULL==(fp=fopen(myvars->input,"r"))) {
			fprintf(stderr, "Cannot open input file\n");
			std::exit(1);
		}
		//stat(myvars->input, &fst);
		//text = (char *)malloc(sizeof(char *)*(fst.st_size+1));
		//str = text;
	}

	if(myvars->output==NULL) {
		if (0 > PS_open_fp(myvars->ps,stdout)) {
			fprintf(stderr,"Cannot open PS file descriptor for stdout.\n");
			std::exit(1);
		}
	}else{
		if(((strncmp(myvars->output+((strlen(myvars->output)) - 4),".pdf",4))==0)||((strcmp(myvars->output,"-"))==0)) { 


/*    using ps2pdf via a pipe code....
 *
			tempstr=(char *)malloc((strlen(myvars->output)+250));
			//sprintf(tempstr,"ps2pdf -dPDFA -dCompatibilityLevel=1.4 -dProcessColorModel=/DeviceGray -dPDFSETTINGS=/printer -dDEVICEWIDTHPOINTS=%d -dDEVICEHEIGHTPOINTS=%d - \"%s\"",myvars->pagewidth,myvars->pageheight,myvars->output);
			sprintf(tempstr,"ps2pdf -dPDFA -dCompatibilityLevel=1.4 -dProcessColorModel=/DeviceGray -dPDFSETTINGS=/printer - \"%s\"",myvars->output);
			fprintf(stderr,"GSCMD [%s]\n",tempstr);
			MYFP = popen(tempstr,"w");
			if (0 > PS_open_fp(myvars->ps,MYFP)) {
				fprintf(stderr,"Cannot open Ghostscript file descriptor\n");
				free(tempstr);
				exit(1);
			}
*/

		// May need to use -dUseCropBox option to correctly set page data size.  
		// /CropBox is the element used by pslib to setup page size.
		
		gsvars->use_gslib=1;

		gsvars->gsargv[0] = "ps2pdf";	// actual value doesn't matter 
		gsvars->gsargv[1] = "-q";
		gsvars->gsargv[2] = "-dNOPAUSE";
		gsvars->gsargv[3] = "-dBATCH";
		gsvars->gsargv[4] = "-dSAFER";
		gsvars->gsargv[5] = "-sDEVICE=pdfwrite";
		gsvars->gsargv[6] = "-dPDFA";
		gsvars->gsargv[7] = "-dCompatibilityLevel=1.4";
		gsvars->gsargv[8] = "-dProcessColorModel=/DeviceGray";
		gsvars->gsargv[9] = "-dPDFSETTINGS=/printer";
		

		//gsvars->gsargv[10] = "-sOutputFile=/tmp/test.pdf";
		//

		//if((strcmp(myvars->output,"-"))==0) { 
		//	gsvars->gsargv[10]=(char *)malloc(sizeof(char *)*1024);
		//	sprintf(gsvars->gsargv[10],"-sOutputFile=%%stdout%%",myvars->output);
		//}else{
			gsvars->gsargv[10]=(char *)malloc(sizeof(char *)*1024);
			sprintf(gsvars->gsargv[10],"-sOutputFile=%s",myvars->output);
		//}
#ifdef USE_RDA_DIAGNOSTICS
		if(myvars->debug) { fprintf(stderr,"gs output file [%s]\n",gsvars->gsargv[10]); }
#endif /* USE_RDA_DIAGNOSTICS */
		gsvars->gsargv[11] = "-c";
		gsvars->gsargv[12] = ".setpdfwrite";
		//gsvars->gsargv[12] = "-f";
		//gsvars->gsargv[13] = "/tmp/test.ps";
		gsvars->gsargc=13;

		gsvars->code = gsapi_new_instance(&gsvars->minst, NULL);
		if (gsvars->code < 0) {
			if(fp!=NULL) { fclose(fp); }
			std::exit(1);
		}
		gsvars->code = gsapi_init_with_args(gsvars->minst, gsvars->gsargc, gsvars->gsargv);

		if (gsvars->code == 0) {
			gsapi_run_string_begin(gsvars->minst, 0, &gsvars->exit_code);
		}

		if (0 > PS_open_mem(myvars->ps,gswriteproc)) {
			fprintf(stderr,"Cannot open PostScript Doc in Memory [%s]\n",myvars->output);
			std::exit(1);
		}
			
		}else if((strncmp(myvars->output+((strlen(myvars->output)) - 3),".ps",3))==0) { 
			/* Current output code does not write more than 2GB. */
			if (0 > PS_open_file(myvars->ps,myvars->output)) {
				fprintf(stderr,"Cannot open PostScript file [%s]\n",myvars->output);
				std::exit(1);
			}

			/* Sample Code to open using 64 bit largefile option....
			if(NULL==(myvars->outputfp=fopen(myvars->output,"wb"))) {
				if (0 > PS_open_fp(myvars->ps,myvars->outputfp)) {
					fprintf(stderr,"Cannot open PS file descriptor for output.\n");
					exit(1);
				}
			}
			*/
		}
	}

	ExecEscCmd(80,myvars);
	ExecEscCmd(81,myvars);
	ExecEscCmd(050,myvars);
#ifdef USE_RDA_DIAGNOSTICS
	if(myvars->debug) {
		fprintf(stderr,"font_array[0]==%d\n",myvars->font_array[0]);
		fprintf(stderr,"font_array[1]==%d\n",myvars->font_array[1]);
		fprintf(stderr,"font_array[10]==%d\n",myvars->font_array[10]);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	myvars->debug=0;

	myvars->textx = 0.0;
	myvars->texty = 0.0;
	
	//printf(" In Main Before: Pointers for texttx and textty %ul %ul\n", (unsigned long) myvars->textx,myvars->texty);
	
	//printf(" In Main Before: Pointers for texttx and textty %ul %ul\n", (unsigned long) myvars->&textx,myvars->&texty);
	//fprintf(stderr,"\nBEFORE X[%5.2f] Y[%5.2f]\n",textx,texty);
	
	// TODO: command parser to default first page and page size defaults
	// needs to set pagesizedef to 0 for not set, 1 for set(Letter), 
	// other page sizes and types need to be set here and called on the NL
	
#ifdef USE_RDA_DIAGNOSTICS
	if(myvars->debug) { fprintf(stderr,"BEFORE SWITCH X[%5.2f] Y[%5.2f]\n",myvars->textx,myvars->texty); }
#endif /* USE_RDA_DIAGNOSTICS */
	
	readsize=0;
	if(myvars->named_pipe!=NULL) {
		memset(inputarray,0,BUFSIZE);
#ifdef WIN32
		while(1) {
			fSuccess=ReadFile(hPipe,inputarray,BUFSIZE,&cbBytesRead,NULL); 
#ifdef CAPTURE_INPUT
			fwrite(inputarray,1,cbBytesRead,diagfp);
#endif /* CAPTURE_INPUT */
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"bytes read [%d] strlen [%d]\n",readsize,(strlen(inputarray))); }
#endif /* USE_RDA_DIAGNOSTICS */
			inputarray[cbBytesRead] = '\0';
			if (! fSuccess || cbBytesRead == 0) { break; }
			for(x=0;x<cbBytesRead;x++) {
				n=inputarray[x];	
				ParseInput(argc,argv,myvars,n);
			}
			readsize=(cbBytesRead+readsize);
		}
			fprintf(stderr,"bytes read [%d] \n",readsize); 

#ifdef CAPTURE_INPUT
		fclose(diagfp);
#endif /* CAPTURE_INPUT */
		ExecEscCmd(110,myvars);
		ExecEscCmd(112,myvars);
		ExecEscCmd(113,myvars);
#endif
		//fwrite(chRequest,1,cbBytesRead,outfile);
		//printf("Size: [%d]  Data: [%s]\n",cbBytesRead,chRequest);

	//}else if(myvars->input!=NULL) {	
	}else{	
		while(!feof(fp)) {
			memset(inputarray,0,65535);
	        	bytes_read=fread(inputarray,1,65534,fp);
#ifdef CAPTURE_INPUT
			fwrite(inputarray,1,bytes_read,diagfp);
#endif /* CAPTURE_INPUT */
			if((fp==stdin)&&(bytes_read==0)) {
				//fprintf(stderr,"bytes read [%d] strlen [%d]  y [%d]\n",readsize,(strlen(inputarray)),y);
				y++;
				if(y>2000000) { break; }
			}else if(fp==stdin) {
				y=0;
			}
			for(x=0;x<bytes_read;x++) {
				n=inputarray[x];	
				ParseInput(argc,argv,myvars,n);
			}
			readsize=(bytes_read+readsize);
			//fprintf(stderr,"bytes read [%d] str [%d] strlen [%d]\n",readsize,n,(strlen(str)));
		}
		fclose(fp);
#ifdef CAPTURE_INPUT
		fclose(diagfp);
#endif /* CAPTURE_INPUT */
		ExecEscCmd(110,myvars);
		ExecEscCmd(112,myvars);
		ExecEscCmd(113,myvars);
	}

	
/* Functions for closing down and freeing memory go here.  */
	
	// PS_close_image(myvars->ps,myvars->psimage[0]);
 
 
	
/*
	psimage = PS_open_image_file(ps, "eps", "DMV.ps", NULL, 0);
	PS_place_image(ps, psimage, 0, 0, 1.0);
*/

//	if(string!=NULL) free(string);
//	if(cmdstr!=NULL) free(cmdstr);
//	if(text!=NULL) free(text);
//	if(str!=NULL) free(str);
	PS_shutdown();
	if(gsvars->use_gslib) {
		gsapi_run_string_end(gsvars->minst, 0, &gsvars->exit_code);
		gsvars->code1 = gsapi_exit(gsvars->minst);
		if ((gsvars->code == 0) || (gsvars->code == e_Quit))
		gsvars->code = gsvars->code1;
	
		gsapi_delete_instance(gsvars->minst);
	
#ifdef USE_RDA_DIAGNOSTICS
	        if ((myvars->debug)&&((gsvars->code == 0) || (gsvars->code == e_Quit))) {
			//return 0;
			fprintf(stderr,"gslib exited normally.\n"); 
		}else if(myvars->debug){
			//return 1;
			fprintf(stderr,"gslib exited in error.\n");
		}
#endif /* USE_RDA_DIAGNOSTICS */
		free(gsvars->gsargv[10]); 	
		if(gsvars!=NULL) free(gsvars);
	}
	
	if(((strstr(argv[0],"viewpdf"))!=NULL)&&myvars->debug) {
		fprintf(stderr,"Program Type is ViewPDF.\n");
		LaunchPDFViewer(myvars);
	}else if((strstr(argv[0],"viewpdf"))!=NULL) {
		LaunchPDFViewer(myvars);
#ifdef USE_RDA_DIAGNOSTICS
	}else if(myvars->debug){
		fprintf(stderr,"Program Type is TXT2PS.\n");
#endif /* USE_RDA_DIAGNOSTICS */
	}
	
	n=0;
	if(myvars->cmdstr!=NULL) free(myvars->cmdstr);
	free(myvars->input);
	free(myvars->output);
	free(myvars->string);

	for(x=0;x<150;x++) { 
		free(myvars->prepagecmdstr[x]);
	}
	if(myvars!=NULL) free(myvars);
// 	fprintf(stderr,"tempstr [%s]\n",tempstr);
/*
	if((strlen(tempstr))!=0) {
			free(tempstr);
	//		pclose(MYFP);
	}
*/
#ifdef WIN32
	FlushFileBuffers(stderr);
	//*stderr=*savestderr;
	//if(mystderr!=NULL) { fclose(mystderr); }
#endif
#ifdef USE_CPU_PROFILER
	ProfilerStop();
#endif
	std::exit;
}

int ParsePrePageCmds(int argc, char **argv,RDA_PS_VARS *myvars)
{
//	fprintf(stderr,"PROCESS PAGE [%d] COUNT [%d]  \n",(myvars->pgcounter),(myvars->prepagecount));
	if((myvars->pgopen==0)||(myvars->formfeed==1)) {
		

	// Just need a switch case statement here for cmds to process before page is opened.
	// Could likely use return code from this command to decide whether to execute
	// command in the calling function.  Make things simple...
		
		switch (myvars->rda_exec_cmd) {
		case 0:
		case 315:
		case 316:
		case 318:
		case 319:
		case 320:
		case 321:
		case 330:
		case 331:
		
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"RUN PRE PAGE CMD [%d]  [%s]\n",myvars->rda_exec_cmd,myvars->cmdstr); }
#endif /* USE_RDA_DIAGNOSTICS */
			ExecEscCmd(myvars->rda_exec_cmd,myvars);
			return(1);
			break;
		default: 
			myvars->prepagecount++;
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"STORE PRE PAGE COUNT [%d] CMD [%d]  [%s]\n",myvars->prepagecount,myvars->rda_exec_cmd,myvars->cmdstr); }
#endif /* USE_RDA_DIAGNOSTICS */

//			fprintf(stderr,"PRE PAGE CMD [%s]\n",myvars->prepagecmd[(myvars->prepagecount)]);
			myvars->prepagecmd[(myvars->prepagecount)]=myvars->rda_exec_cmd;
		//	myvars->prepagecmdstr[(myvars->prepagecount)]=(char *)malloc((strlen(myvars->cmdstr))+1);
			sprintf(myvars->prepagecmdstr[(myvars->prepagecount)],"%s",myvars->cmdstr);
			return(0);
			break;
		}
		
//
//	} else if (myvars->pgopen==1) {
//		fprintf(stderr,"POST PAGE CMD [%d]  [%s]\n",myvars->rda_exec_cmd,myvars->cmdstr);
//	}else{  
//		fprintf(stderr,"WIERD PAGE CMD [%d]  [%s]\n",myvars->rda_exec_cmd,myvars->cmdstr);
//
	}


}
int ProcessPrePageCmds(RDA_PS_VARS *myvars)
{
	int i=0;
	char *tempstr;
//	fprintf(stderr,"PROCESS PAGE [%d] COUNT [%d] PGOPEN [%d] FF [%d]  \n",(myvars->pgcounter),(myvars->prepagecount),(myvars->pgopen),(myvars->formfeed));
	while(1){
		if((myvars->prepagecmd[(i)]!=0)) {
#ifdef USE_RDA_DIAGNOSTICS
			if(myvars->debug) { fprintf(stderr,"PROCESS PRE PAGE CMD [%d] [%s] \n",myvars->prepagecmd[(i)],myvars->prepagecmdstr[(i)]); }
#endif /* USE_RDA_DIAGNOSTICS */

			tempstr=(char *)malloc(sizeof(char *)*((strlen(myvars->prepagecmdstr[(i)]))+1));
			sprintf(tempstr,"%s",myvars->prepagecmdstr[(i)]);

			
//			sprintf(myvars->cmdstr,"%c",0);
			sprintf(myvars->cmdstr,"%s",(myvars->prepagecmdstr[(i)]));
			ExecEscCmd(myvars->prepagecmd[(i)],myvars);
			sprintf(myvars->cmdstr,"%s",tempstr);


			
			myvars->prepagecmd[(i)]=0;
			//sprintf(myvars->prepagecmdstr[(i)],"%c",0);
			
			//sprintf(tempstr,"%c",0);
			free(tempstr);
	
	/*
		}else{
			fprintf(stderr,"PROCESS ZERO OR NULL CMD \n");
	*/
		}
		
		if(myvars->prepagecount==0) { break; }
		if(i==myvars->prepagecount) { break; }
		i++;
	}
	myvars->prepagecount=0;
		
	return(0);
}

int LaunchPDFViewer(RDA_PS_VARS *myvars)
{
	int execstatus=0;
	char *mytemp=NULL;
	char *myenv1=NULL,*myenv2=NULL,*myenv3=NULL;

#ifdef WIN32
	DWORD bufsize=0;
#endif

	if(myvars->viewfile) {
		mytemp=(char *)malloc(sizeof(char *)*512);
#ifndef WIN32
		myenv1=getenv("CYBER_PRODUCT");
		myenv2=getenv("CYBER_PROCESS");
		myenv3=getenv("REPORT_TITLE");
		sprintf(mytemp,"acroread -tempFileTitle \"%s - %s - %s\" -openInNewWindow \"%s\"",(myenv1!=NULL ? myenv1:"UTILITIES"),(myenv2!=NULL ? myenv2:"ViewPDF"),(myenv3!=NULL ? myenv3:myvars->output),myvars->output);
		if(myvars->debug) {
			fprintf(stderr,"CMD:  [%s]\n",mytemp);
		}else{
			fprintf(stderr,"Output File: %s\n",myvars->output);
		}
	
	 
		execstatus=system(mytemp);
		if(myvars->debug) { fprintf(stderr,"Exit Code: %d\n",WEXITSTATUS(execstatus)); }
		if(myvars->removefile) {
			unlink(myvars->output);
		}
	
#else
	/*
	 * This should read this variable from the registry and then
	 * set mytemp with the command and the filename to use with the exec.
	 * Need to make sure it allows waiting on acrobat to finish as well.  
	 *
[HKEY_CLASSES_ROOT\acrobat\shell\open\command]
@="\"C:\\Program Files\\Adobe\\Reader 9.0\\Reader\\AcroRd32.exe\" /u \"%1\""
	 *
	*/
		myenv1=(char *)malloc(sizeof(char *)*512);
		GetCurrentDirectoryA(&bufsize,myenv1);
		sprintf(mytemp,"%s\\%s",myenv1,myvars->output);
		fprintf(stderr,"Output File: %s\\%s\n",myenv1,myvars->output);
		ShellExecute(NULL,"open",mytemp,NULL,NULL,SW_SHOWNORMAL);
		Sleep(10000);
		while(1) {
			//  Could possibly use MoveFileEx instead if file needs kept.
			//  May also need to stat() to make sure file exists to 
			//  prevent an infinite loop.
			if((unlink(mytemp))==0) { break; }
			Sleep(10000);
		}
		free(myenv1);
#endif
		free(mytemp);
	}
	return(0);
}
int ParseInput(int argc, char **argv,RDA_PS_VARS *myvars,int n)
{
		switch (n) {
			// 
			// This section may need to be redone to handle extra command parsing on NLs 
			// so that page and font instructions passed via commands will have appropriate
			// variables set before starting the first page.  Likely will be handled by doing 
			// command parsing/setting until the first valid line of printable text is parsed.
			// After the first printable line of text is found, it will then launch all
			// of the commands currently being called before this loop is entered.
			//
			case 10:
				
#ifdef USE_RDA_DIAGNOSTICS
				if(myvars->debug) { fprintf(stderr, "NL [%d]  pagesizedef [%d]   landscape [%d]\n",myvars->nlcounter,myvars->pagesizedef,myvars->landscape); }
#endif /* USE_RDA_DIAGNOSTICS */
				if(myvars->formfeed==1) {
					myvars->formfeed=0;
					myvars->nlcounter=0;
					ExecEscCmd(110,myvars);
				}
				//fprintf(stderr,"BEFORE NL X[%5.2f] Y[%5.2f]\n",myvars->textx,myvars->texty);
				myvars->nlcounter++;
				//fprintf(stderr, "Case: NL [%d] \n",n);

				// TODO:  If the New page has not been called using a ESC
				// sequence yet, do so now. Default Portrait or 
				// the default set by command line options

				//ExecEscCmd(101,cmdstr,ps,string,psfont,font_size,&textx,&texty,line_space);
					
				//fprintf(stderr, "font_set=[%d] NL Count=[%d]\n",myvars->font_set,myvars->nlcounter);
				
				//fprintf(stderr, "pagesizedef [%d] \n",myvars->pagesizedef);

				if((myvars->pgopen==1)&&(myvars->nlcounter==1)&&(strlen(myvars->string)!=0)) { 
					// This should only apply for lines with only printable text
					ExecEscCmd(301,myvars);

					myvars->texty=(myvars->texty - myvars->line_space[0]);
					sprintf(myvars->string, "%c",0);
					myvars->textx=myvars->lmargin;
				}else if((myvars->pgopen==0)&&(myvars->nlcounter==1)&&(strlen(myvars->string)==0)) { 
					// This should only apply for first lines with only Esc commands
					if(myvars->pagesizedef == 0) {
						ExecEscCmd(101,myvars);
					}else if(myvars->pagesizedef == 1) {
						ExecEscCmd(101,myvars);
					}else if(myvars->pagesizedef == 2) {
						ExecEscCmd(102,myvars);
					}else if(myvars->pagesizedef == 3) {
						ExecEscCmd(103,myvars);
					}else if(myvars->pagesizedef == 4) {
						ExecEscCmd(104,myvars);
					}else if(myvars->pagesizedef == 5) {
						ExecEscCmd(105,myvars);
					}else if(myvars->pagesizedef == 6) {
						ExecEscCmd(106,myvars);
					}else if(myvars->pagesizedef == 7) {
						ExecEscCmd(107,myvars);
					}else if(myvars->pagesizedef == 8) {
						ExecEscCmd(108,myvars);
					}
					
					if(myvars->printstyle == 1) {
						ExecEscCmd(001,myvars);
					} else if(myvars->printstyle == 2) {
						ExecEscCmd(002,myvars);
					}

					myvars->texty=(myvars->texty - myvars->line_space[0]);
					sprintf(myvars->string, "%c",0);
					
				}else if((myvars->pgopen==0)&&(myvars->nlcounter==1)&&(strlen(myvars->string)!=0)) { 
					// This should only apply for lines with only printable text
					if(myvars->pagesizedef == 0) {
						ExecEscCmd(101,myvars);
					}else if(myvars->pagesizedef == 1) {
						ExecEscCmd(101,myvars);
					}else if(myvars->pagesizedef == 2) {
						ExecEscCmd(102,myvars);
					}else if(myvars->pagesizedef == 3) {
						ExecEscCmd(103,myvars);
					}else if(myvars->pagesizedef == 4) {
						ExecEscCmd(104,myvars);
					}else if(myvars->pagesizedef == 5) {
						ExecEscCmd(105,myvars);
					}else if(myvars->pagesizedef == 6) {
						ExecEscCmd(106,myvars);
					}else if(myvars->pagesizedef == 7) {
						ExecEscCmd(107,myvars);
					}else if(myvars->pagesizedef == 8) {
						ExecEscCmd(108,myvars);
					}
					
					if(myvars->printstyle == 1) {
						ExecEscCmd(001,myvars);
					} else if(myvars->printstyle == 2) {
						ExecEscCmd(002,myvars);
					}
					
					ExecEscCmd(301,myvars);

					myvars->texty=(myvars->texty - myvars->line_space[0]);
					sprintf(myvars->string, "%c",0);
					myvars->textx=myvars->lmargin;
				}else if((myvars->pgopen==1)&&(myvars->nlcounter==1)&&(strlen(myvars->string)==0)) { 
					// This should only apply for lines with only printable text
					myvars->textx=myvars->lmargin;
				} else {
					ExecEscCmd(301,myvars);
					myvars->texty=(myvars->texty - myvars->line_space[0]);
					sprintf(myvars->string, "%c",0);
					myvars->textx=myvars->lmargin;
				}
					


/*


				
				if(myvars->formfeed==1 && strlen(myvars->string)!=0) {
					
					myvars->formfeed=0;
					myvars->nlcounter=0;
					ExecEscCmd(110,myvars);

					// Need some way to change this per page and rest of job
					// with a Esc CMD before calling the new page.  Until then
					// you will have to change page sizes before calling the formfeed 
					// for the end of the previous page.
					if(myvars->pagesizedef == 0) {
						ExecEscCmd(101,myvars);
					}else if(myvars->pagesizedef == 1) {
						ExecEscCmd(101,myvars);
					}else if(myvars->pagesizedef == 2) {
						ExecEscCmd(102,myvars);
					}else if(myvars->pagesizedef == 3) {
						ExecEscCmd(103,myvars);
					}else if(myvars->pagesizedef == 4) {
						ExecEscCmd(104,myvars);
					}else if(myvars->pagesizedef == 5) {
						ExecEscCmd(105,myvars);
					}else if(myvars->pagesizedef == 6) {
						ExecEscCmd(106,myvars);
					}else if(myvars->pagesizedef == 7) {
						ExecEscCmd(107,myvars);
					}else if(myvars->pagesizedef == 8) {
						ExecEscCmd(108,myvars);
					}	
					
					//fprintf(stderr, "FF=TRUE, NL STRING: [%s]\n",string);
				
					// needs to use defpagesize
					// setting formfeed to default tru from the start
					// should set the first page size correctly.
					//ExecEscCmd(101,myvars);
				
					// This should set the default font instead of just NORMAL
					if(myvars->printstyle == 1) {
						ExecEscCmd(001,myvars);
					} else if(myvars->printstyle == 2) {
						ExecEscCmd(002,myvars);
					}
				
					// TODO: This should be handled automatically somehow.
					//
					//ExecEscCmd(120,cmdstr,ps,string,psfont,font_size,NULL,NULL,line_space);
				//}else if(myvars->formfeed==2 && strlen(myvars->string)!=0) {
				}else if(myvars->formfeed==2) {
					//fprintf(stderr,"NL STRING: [%s] FF=[%d] strlen [%d]\n",string,myvars->formfeed);
				
					
					if(myvars->pagesizedef == 0) {
						ExecEscCmd(101,myvars);
					}else if(myvars->pagesizedef == 1) {
						ExecEscCmd(101,myvars);
					}else if(myvars->pagesizedef == 2) {
						ExecEscCmd(102,myvars);
					}else if(myvars->pagesizedef == 3) {
						ExecEscCmd(103,myvars);
					}else if(myvars->pagesizedef == 4) {
						ExecEscCmd(104,myvars);
					}else if(myvars->pagesizedef == 5) {
						ExecEscCmd(105,myvars);
					}else if(myvars->pagesizedef == 6) {
						ExecEscCmd(106,myvars);
					}else if(myvars->pagesizedef == 7) {
						ExecEscCmd(107,myvars);
					}else if(myvars->pagesizedef == 8) {
						ExecEscCmd(108,myvars);
					}
					
					
					myvars->formfeed=0;
					if((myvars->font_set == 0) && (myvars->printstyle == 0)) {
						ExecEscCmd(001,myvars);
					}else if((myvars->font_set == 0) && (myvars->printstyle == 1)) {
						ExecEscCmd(001,myvars);
					} else if((myvars->font_set == 0) && (myvars->printstyle == 2)) {
						ExecEscCmd(002,myvars);
					}
					
				} 
				
				//else {
				//	fprintf(stderr, "FF=FALSE, NL STRING: [%s]\n",string);
				//}
				
				if(strlen(myvars->string)!=0) {
					//PrtCmd();

				
					if(myvars->rdapclborder) {
						 myvars->textx = 18.0; 
					} else {
						 myvars->textx = 18.0; 
					}
			/
					ExecEscCmd(301,myvars);
				
					//fprintf(stderr, "NL STRING: [%s]\n",string);
					//Need to increment textx and texty
					//based on current font size
					//fprintf(stderr,"BEFORE NL X[%5.2f] Y[%5.2f]\n",textx,texty);
				
					//fprintf(stderr,"AFTER NL X[%5.2f] Y[%5.2f]\n",myvars->textx,myvars->texty);
				}
				myvars->texty=(myvars->texty - myvars->line_space[0]);
				sprintf(myvars->string, "%c",0);
*/
				break;
			case 12:
				
				//fprintf(stderr, "Case: FF [%d] \n",n);
				//
				//Close Page
				myvars->formfeed=1;
				//ExecEscCmd(110,cmdstr,ps,string,psfont,font_size,&textx,&texty,line_space);
				//
				//New Page Based on last defined page size (can be changed mid stream by esc cmd)
				//if(pgsize=1) PORTRAIT LETTER
				//fprintf(stderr,"BEFORE FF X[%5.2f] Y[%5.2f]\n",textx,texty);
				//ExecEscCmd(101,cmdstr,ps,string,psfont,font_size,&textx,&texty,line_space);
				//fprintf(stderr,"AFTER FF X[%5.2f] Y[%5.2f]\n",textx,texty);
				//  Need some way to track last font used and 
				//  set it as the starting font for next page.
				//ExecEscCmd(001,cmdstr,ps,string,psfont,font_size,NULL,NULL,line_space);
				//textx=18;
				//texty=759;
				//elif(pgsize=2) LANDSCAPE LETTER
				//ExecEscCmd(101,cmdstr,ps,string,psfont,font_size,cmdstr,ps,string,psfont,font_size,&textx,&texty,line_spacetextx,cmdstr,ps,string,psfont,font_size,&textx,&texty,line_spacetexty,line_space);
				break;
			case 13:
				
				//fprintf(stderr, "Case: CR [%d] \n",n);
				break;
			case 27:
				
				//if((myvars->esc_cmd)&&(myvars->pgopen==0)&&(myvars->nlcounter==0)) {
				if((myvars->esc_cmd)&&(myvars->pgopen==0)) {
					// Set page type and font type
					// Page type should be first command or this will need to be edited.

					/*
					if(myvars->pagesizedef == 0) {
						ExecEscCmd(101,myvars);
					}else if(myvars->pagesizedef == 1) {
						ExecEscCmd(101,myvars);
					}else if(myvars->pagesizedef == 2) {
						ExecEscCmd(102,myvars);
					}else if(myvars->pagesizedef == 3) {
						ExecEscCmd(103,myvars);
					}else if(myvars->pagesizedef == 4) {
						ExecEscCmd(104,myvars);
					}else if(myvars->pagesizedef == 5) {
						ExecEscCmd(105,myvars);
					}else if(myvars->pagesizedef == 6) {
						ExecEscCmd(106,myvars);
					}else if(myvars->pagesizedef == 7) {
						ExecEscCmd(107,myvars);
					}else if(myvars->pagesizedef == 8) {
						ExecEscCmd(108,myvars);
					}
					if((myvars->font_set == 0) && (myvars->printstyle == 0)) {
						ExecEscCmd(001,myvars);
					}else if((myvars->font_set == 0) && (myvars->printstyle == 1)) {
						ExecEscCmd(001,myvars);
					} else if((myvars->font_set == 0) && (myvars->printstyle == 2)) {
						ExecEscCmd(002,myvars);
					}
					*/

					ParsePrePageCmds(argc,argv,myvars);

					
					// Run exec cmd
					myvars->esc_cmd=0;
					//ExecEscCmd(myvars->rda_exec_cmd,myvars);
					myvars->rda_exec_cmd=0;
				}else if(myvars->esc_cmd) {
					//ParsePrePageCmds(argc,argv,myvars);
					// execute command string function
					// Close ESC Quote and exec function
					//myvars->rda_exec_cmd=111;
					//fprintf(stderr,"Execute #%d  CMDSTR[%s]\n",myvars->rda_exec_cmd,cmdstr);
//					fprintf(stderr,"NORMAL CMD [%d]  [%s]\n",myvars->rda_exec_cmd,myvars->cmdstr);
					myvars->esc_cmd=0;
					if(myvars->formfeed==1) {
						ParsePrePageCmds(argc,argv,myvars);
					}else{
						ExecEscCmd(myvars->rda_exec_cmd,myvars);
					}
					myvars->rda_exec_cmd=0;
				} else {
					//ParsePrePageCmds(argc,argv,myvars);
					// Begin ESC Quoted CMD 
					//ExecEscCmd() call here
					
					// Open CMD ESC

					memset(myvars->cmdstr,0,512);
					//sprintf(myvars->cmdstr, "%c",0);
					myvars->esc_cmd=1;
				}
				break;
			default: 
				
				//fprintf(stderr, "CaseU: [%d] [%c]\n",n,n);
				if(myvars->esc_cmd) {
					//fprintf(stderr,"PRE CMDSTR [%s]\n",myvars->cmdstr);
					sprintf(myvars->cmdstr, "%s%c",myvars->cmdstr,n);
					//fprintf(stderr,"POST CMDSTR [%s]\n",myvars->cmdstr);
				} else {
					
					if(myvars->formfeed==1) {
						ExecEscCmd(301,myvars);
						myvars->texty=(myvars->texty - myvars->line_space[0]);
						sprintf(myvars->string, "%c",0);

						myvars->formfeed=0;
						myvars->nlcounter=0;
						ExecEscCmd(110,myvars);
					}
					sprintf(myvars->string, "%s%c",(myvars->string!=NULL ? myvars->string:""),n);
				}

				if (myvars->esc_cmd && strlen(myvars->cmdstr)==3 && myvars->rda_exec_cmd==0) {

					//fprintf(stderr, "CMDSTR [%s] RDACMD: [%d]\n",myvars->cmdstr,myvars->rda_exec_cmd);
					myvars->rda_exec_cmd=atoi(myvars->cmdstr);

					sprintf(myvars->cmdstr, "%c",0);

					//fprintf(stderr, "CMDSTR [%s] RDACMD: [%d]\n",myvars->cmdstr,myvars->rda_exec_cmd);

				}

				break;

		}
		//if(bytes_read==50) { break;}

		return(0);

}
