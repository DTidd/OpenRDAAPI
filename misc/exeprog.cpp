/* exeprog.c - Execute Programs */
#include <misc.hpp>

extern char **environ;
void (*UI_OpenWindow)(char *)=NULL;
void (*UI_OpenWindowWait)(char *)=NULL;
char *GetWebIDGSV();

char *RDAPercentEncode(char *s)
{
	char *n=NULL,*temp1=NULL;
	int x=0,count=0,num=0;

	if(RDAstrstr(s,"#"))
	{
		for(num=0,temp1=s;*temp1;++temp1) if(*temp1=='#') ++num;
		n=Rmalloc(RDAstrlen(s)+(num*3)+1);
		for(count=0,temp1=s;*temp1;++temp1)
		{
			if(*temp1=='#') 
			{ 
				sprintf(&n[count],"%s","%23");
				count+=3;	
			} else {
				n[count++]=*temp1;
			}
		}
	} else {
		n=stralloc(s);
	}
	return(n);
}

static void signal_cleanup(int sig,siginfo_t *siginfo,void *context)
{
	/* This should call any global clean up processes */
	prterr("SIGTERM Signal Received.");
}

static void process_reaper(int sig,siginfo_t *siginfo,void *context)
{
	int status;
	while (waitpid( 0, &status, WNOHANG) > 0) ;
}

short xExecuteProgram(char *name,APPlib *args,APPlib *envp,short style,APPlib *remove_env,int line,char *file)
{
	int pid=0,x=0,count=0;
	char *progname=NULL,**temp=NULL,*windowData=NULL,*temp1=NULL;
	char *temp2=NULL;
	struct sigaction act;
	APPlib *applibx=NULL;

	errno=0;
	if(diagexeprog)
	{
		prterr("DIAG ExecuteProgram Executing Program [%s] at line [%d] program [%s].",name,line,file);
		SEEAPPlib(args);
		SEEAPPlib(envp);
		SEEAPPlib(remove_env);
	}
	if(name==NULL)
	{
		prterr("Error Cannot Execute Program is NULL at line [%d] program [%s].",line,file);
		return(-1);
	}
	applibx=APPlibNEW();
	if((RDA_NOGUI && UI_OpenWindow==NULL) || (!RDA_NOGUI && UI_OpenWindow!=NULL))
	{
		if(style==TRUE)
		{
			progname=stralloc(name);
		} else {
		
			progname=(char *)Rmalloc(RDAstrlen(name)+5);
#ifndef WIN32
			sprintf(progname,"%s.lnx",name);
#else
			sprintf(progname,"%s.exe",name);
#endif
		}
	} else {
		if(style==TRUE)
		{
			progname=Rmalloc(RDAstrlen(DOCUMENTROOT)+RDAstrlen(name)+4);
			sprintf(progname,"%s/%s",DOCUMENTROOT,name);
		} else {
			progname=Rmalloc(RDAstrlen(DOCUMENTROOT)+RDAstrlen(name)+10);
#ifndef WIN32
			sprintf(progname,"%s/%s.lnx",DOCUMENTROOT,name);
#else
			sprintf(progname,"%s/%s.exe",DOCUMENTROOT,name);
#endif
		}
	}
	addAPPlib(applibx,progname);
	if(UI_OpenWindow!=NULL && !RDA_NOGUI)
	{
		windowData=stralloc(progname);
		count=RDAstrlen(progname);
	}
	if(args!=NULL)
	{
		for(x=0;x<args->numlibs;++x)
		{
			addAPPlib(applibx,args->libs[x]);
			if(UI_OpenWindow!=NULL && !RDA_NOGUI)
			{
				temp2=RDAPercentEncode(args->libs[x]);
				if(x==0)
				{
					temp1=Rmalloc(8+RDAstrlen(temp2));
					sprintf(temp1,"?arg%d=%s",(x+1),temp2);
				} else {
					temp1=Rmalloc(8+RDAstrlen(temp2));
					sprintf(temp1,"&arg%d=%s",(x+1),temp2);
				}
				if(temp2!=NULL) Rfree(temp2);
				windowData=Rrealloc(windowData,(count+RDAstrlen(temp1)+1));
				memcpy(&windowData[count],temp1,RDAstrlen(temp1)+1);
				count+=RDAstrlen(temp1);
				if(temp1!=NULL) Rfree(temp1);
			}
		}
	}
	addAPPlib(applibx,NULL);
	if(UI_OpenWindow!=NULL && !RDA_NOGUI)
	{
		if(envp!=NULL)
		{
			for(x=0;x<envp->numlibs;++x)
			{
				RDA_PutEnv(envp->libs[x]);
			}
		}
		UI_OpenWindow(windowData);
		if(progname!=NULL) Rfree(progname);
		if(applibx!=NULL) freeapplib(applibx);
		if(windowData!=NULL) Rfree(windowData);
		return(0);
	} else {

		memset (&act, '\0', sizeof(act));
		act.sa_flags = SA_SIGINFO;
	
		/* Set Signal Handler for SIGTERM */
		act.sa_sigaction = &signal_cleanup;
		if (sigaction(SIGTERM, &act, NULL) < 0) {
			fprintf(stderr,"Setting SIGACTION handler for SIGTERM FAILED!\n");
			return 1;
		}
	 
		/* Set Signal Handler/Reaper for SIGCHLD */
		act.sa_sigaction = &process_reaper;
		if (sigaction(SIGCHLD,&act, NULL) < 0) {
			fprintf(stdout,"Setting SIGACTION handler for SIGCHLD FAILED!\n");
		}
 
		pid=fork();
		if (pid == (pid_t) 0) 
		{
			for(temp=environ;*temp!=(char *)0;++temp)
			{
				putenv(*temp);
			}
			if(remove_env!=NULL)
			{
				for(x=0;x<remove_env->numlibs;++x)
				{
					RDA_UnSetEnv(remove_env->libs[x]);
				}
			}
			if(envp!=NULL)
			{
				for(x=0;x<envp->numlibs;++x)
				{
					RDA_PutEnv(envp->libs[x]);
				}
			}
/*lint -e605 */
			pid=execvp(progname,applibx->libs);
/*lint +e605 */
			prterr("Error Child Couldn't execvp (Returned %d) to Execute Command [%s] System Error [%d] [%s] at line [%d] program [%s].",pid,(progname!=NULL ? progname:"NULL"),errno,errname(),line,file);
			if(progname!=NULL) Rfree(progname);
			if(applibx!=NULL) freeapplib(applibx);
			std::exit;

		}else if(pid == (pid_t) -1)
		{
			prterr("Error Couldn't Fork to Execute Command [%s] System Error [%d] [%s] at line [%d] program [%s].",progname,errno,errname(),line,file);
			if(progname!=NULL) Rfree(progname);
			if(applibx!=NULL) freeapplib(applibx);
			return(-1);

		}else{
			if(progname!=NULL) Rfree(progname);
			if(applibx!=NULL) freeapplib(applibx);
			if(errno!=0)
			{
				prterr("Error from ExecuteProgram [%d] [%s].",errno,errname());
			}
			return(errno);
		}
	}

}
short xExecute2Program(char *name,APPlib *args,APPlib *envp,char add_ext,char quoteargs,APPlib *remove_env,int line,char *file)
{
	int pid=0,x=0,child_state=0,count=0;
	char *progname=NULL,**temp=NULL,*windowData=NULL,*temp1=NULL;
	char *temp2=NULL;
	pid_t rc_pid;
	struct sigaction act;
 
	APPlib *applibx=NULL;

	errno=0;
	if(diagexeprog)
	{
		prterr("DIAG ExecuteProgram Executing Program [%s] at line [%d] program [%s].",name,line,file);
		SEEAPPlib(args);
		SEEAPPlib(envp);
		SEEAPPlib(remove_env);
	}
	if(name==NULL)
	{
		prterr("Error Cannot Execute Program is NULL at line [%d] program [%s].",line,file);
		return(-1);
	}
	applibx=APPlibNEW();
	if((RDA_NOGUI && UI_OpenWindowWait==NULL) || (!RDA_NOGUI && UI_OpenWindowWait!=NULL))
	{
		if(add_ext==FALSE)
		{
			progname=stralloc(name);
		} else {
		
			progname=(char *)Rmalloc(RDAstrlen(name)+5);
#ifndef WIN32
			sprintf(progname,"%s.lnx",name);
#else
			sprintf(progname,"%s.exe",name);
#endif
		}
	} else {
		if(add_ext==FALSE)
		{
			progname=Rmalloc(RDAstrlen(DOCUMENTROOT)+RDAstrlen(name)+4);
			sprintf(progname,"%s/%s",DOCUMENTROOT,name);
		} else {
			progname=Rmalloc(RDAstrlen(DOCUMENTROOT)+RDAstrlen(name)+10);
#ifndef WIN32
			sprintf(progname,"%s/%s.lnx",DOCUMENTROOT,name);
#else
			sprintf(progname,"%s/%s.exe",DOCUMENTROOT,name);
#endif
		}
	}
	addAPPlib(applibx,progname);
	if(UI_OpenWindowWait!=NULL && !RDA_NOGUI)
	{
		windowData=stralloc(progname);
		count=RDAstrlen(progname);
	}
	if(args!=NULL)
	{
		for(x=0;x<args->numlibs;++x)
		{
			addAPPlib(applibx,args->libs[x]);
			if(UI_OpenWindowWait!=NULL && !RDA_NOGUI)
			{
				temp2=RDAPercentEncode(args->libs[x]);
				if(x==0)
				{
					temp1=Rmalloc(8+RDAstrlen(temp2));
					sprintf(temp1,"?arg%d=%s",(x+1),temp2);
				} else {
					temp1=Rmalloc(8+RDAstrlen(temp2));
					sprintf(temp1,"&arg%d=%s",(x+1),temp2);
				}
				windowData=Rrealloc(windowData,(count+RDAstrlen(temp1)+1));
				memcpy(&windowData[count],temp1,RDAstrlen(temp1)+1);
				count+=RDAstrlen(temp1);
				if(temp1!=NULL) Rfree(temp1);
				if(temp2!=NULL) Rfree(temp2);
			}
		}
	}
	addAPPlib(applibx,NULL);

	if(UI_OpenWindowWait!=NULL && !RDA_NOGUI)
	{
		if(envp!=NULL)
		{
		for(x=0;x<envp->numlibs;++x)
		{
			RDA_PutEnv(envp->libs[x]);
		}
		}
		UI_OpenWindowWait(windowData);
		if(progname!=NULL) Rfree(progname);
		if(applibx!=NULL) freeapplib(applibx);
		if(windowData!=NULL) Rfree(windowData);
		return(0);
	} else {


		memset (&act, '\0', sizeof(act));
		act.sa_flags = SA_SIGINFO;
	
		/* Set Signal Handler for SIGTERM */
		act.sa_sigaction = &signal_cleanup;
		if (sigaction(SIGTERM,&act,NULL)<0) {
			fprintf(stderr,"Setting SIGACTION handler for SIGTERM FAILED!\n");
			return 1;
		}
	 
 		/* Unset Signal Handler for SIGCHLD for Successful wait() */
		memset (&act, '\0', sizeof(act));
		if (sigaction(SIGCHLD, &act, NULL) < 0) {
			fprintf(stdout,"Setting SIGACTION handler for SIGCHLD FAILED!\n");
		}

		pid = fork ();
		if (pid == (pid_t) 0) {

			for(temp=environ;*temp!=(char *)0;++temp)
			{
				putenv(*temp);
			}

			if(remove_env!=NULL)
			{
				for(x=0;x<remove_env->numlibs;++x)
				{
					RDA_UnSetEnv(remove_env->libs[x]);
				}
			}

			if(envp!=NULL)
			{
				for(x=0;x<envp->numlibs;++x)
				{
					putenv(envp->libs[x]);
				}
			}
/*lint -e605 */
			pid=execvp(progname,applibx->libs);
/*lint +e605 */
			prterr("Error Child Couldn't execvp (Returned %d) to Execute Command [%s] System Error [%d] [%s] at line [%d] program [%s].",pid,(progname!=NULL ? progname:"NULL"),errno,errname(),line,file);
			if(progname!=NULL) Rfree(progname);
			if(applibx!=NULL) freeapplib(applibx);
			std::exit;

		}else if(pid == (pid_t) -1){

			act.sa_sigaction = &process_reaper;
			if (sigaction(SIGCHLD, &act, NULL) < 0) {
				prterr("Setting SIGACTION handler for SIGCHLD FAILED!\n");
			}
			prterr("Error Couldn't Fork to Execute Command [%s] System Error [%d] [%s] at line [%d] program [%s].",progname,errno,errname(),line,file);
			if(progname!=NULL) Rfree(progname);
			if(applibx!=NULL) freeapplib(applibx);
			return(-1);

		}else{

			rc_pid=waitpid(pid,&child_state,0);
			if(rc_pid > 0)
			{
				if(WIFEXITED(child_state)) {
					if(((WEXITSTATUS(child_state))!=0)||(diagexeprog)) 
					{
						prterr("Child exited with exit status [%d] [%d] [%s] at line [%d] program [%s].",WEXITSTATUS(child_state),errno,errname(),line,file);
					}
				} else if (WIFSIGNALED(child_state)) {
					prterr("Child exited via signal [%d] [%d] [%s] at line [%d] program [%s].",WTERMSIG(child_state),errno,errname(),line,file);
				} else if (WIFSTOPPED(child_state)) {
					prterr("Child [%d] stopped by signal %d\n",pid,WSTOPSIG(child_state));
				} else if (WIFCONTINUED(child_state)) {
					fprintf(stdout,"continued\n");
					prterr("Child [%d] Continued. \n",pid);
				}

			} else {
			  /* if no PID returned, then an error */
				if(errno == ECHILD) 
				{
					prterr("No children exist, fork/exec failed  [%d] [%s] at line [%d] program [%s].",errno,errname(),line,file);
				} else {
					prterr("Unexpected error, No children exist, fork/exec failed  [%d] [%s] at line [%d] program [%s].",errno,errname(),line,file);
				}
			}

			act.sa_sigaction = &process_reaper;
			if(sigaction(SIGCHLD, &act, NULL) < 0) {
				prterr("Setting SIGACTION handler for SIGCHLD FAILED!\n");
			}
		}

		if(progname!=NULL) Rfree(progname);
		if(applibx!=NULL) freeapplib(applibx);
		if(errno!=0)
		{
			prterr("Error from Execute2Program [%d] [%s].",errno,errname());
		}
		if (WIFEXITED(child_state))
		{
			return(WEXITSTATUS(child_state));
		} else { 
			return(errno); 
		}
	}
}
