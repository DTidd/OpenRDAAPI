#ifdef WIN32
#include <windows.h>
#include <stdio.h>
#pragma comment (lib, "user32.lib")
#pragma comment (lib, "kernel32.lib")

#define TRACE fprintf(stdout,"File [[%s],  Line [%d]\n",__FILE__,__LINE__);


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
	printf("Tried to launch: \"%s\", but got error [%u]: ",cmdline,GetLastError());
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
		printf("Create Pipe Error [%u]\n",GetLastError());
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
	 	fdval=_open_osfhandle(temphand);
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
#endif /* (WIN32) */
