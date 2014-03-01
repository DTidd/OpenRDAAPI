/* syscom.c - allows calling of a system command */
#include <misc.hpp>
extern char **environ;

char *READargument(char **temp,short *Texit)
{
	int count=0;
	char *tmp=NULL,quotes=FALSE,tread[2];
	char *sometemp=NULL;
	char t=0;

	sometemp=*temp;
	t=sometemp[0];
	if(t==' ') 
	{
		++(*temp);
		++sometemp;
	}
	if(!*Texit && !isEMPTY(sometemp))
	{
		while(TRUE)
		{
			memset(tread,0,2);
			tread[0]=(*sometemp);
			if((tread[0]=='\0'))
			{
				if(diagtrans) { prterr("DIAG READFIELD reached end of file."); }
				*Texit=TRUE;
				break;
			}
			if(!quotes && tread[0]=='\"') 
			{
				quotes=TRUE;
			} else if(quotes && strchr("\"",tread[0]))
			{
				quotes=FALSE;
				++(*temp);
				++sometemp;
				break;
			} else if(!quotes && strchr("\t\n",tread[0]) && count>0) 
			{
				break;
			} else if(!quotes && tread[0]==' ' && count>0)
			{
				break;
			} else if(!quotes && strchr("\t\n",tread[0]))
			{
			} else {
				if(tmp!=NULL)
				{
					tmp=(char *)Rrealloc(tmp,count+2);
				} else tmp=(char *)Rmalloc(2);
				tmp[count]=tread[0];
				tmp[count+1]='\0';
				++count;
			}
			++(*temp);
			++sometemp;
		}
	}
	if(diagtrans)
	{
		prterr("DIAG READFIELD returning [%s].",(tmp!=NULL ? tmp:"NULL"));
	}
	return(tmp);
}
short system_command(char *command)
{
	int pid=0,status=0;
	char *argv[4];

	if(isEMPTY(command)) return(-1);
	signal(SIGCHLD,SIG_DFL);
	pid=fork();
	if(pid==(-1)) return(-1);
	if(pid==0)
	{
		argv[0]="sh";
		argv[1]="-c";
		argv[2]=command;
		argv[3]=0;
		execve("/bin/sh",argv,environ);
		std::exit;
	}
	while(TRUE)
	{
		if(waitpid(pid,&status,0)==(-1)) 
		{
			if(errno != EINTR) return(-1);
		} else return(status);
	}
}
