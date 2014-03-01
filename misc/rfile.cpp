#include<misc.hpp>

int xRcreat(char *filename,char *source,int line)
{
	int fd;

	if(diagrf)
	{
		prterr("DIAG Rcreat Create File [%s] at line [%d] program [%s].",filename,line,source);
	}
	fd=creat(filename,0666);
#ifndef WIN32
	if(fd<0)
	{
		prterr("Error [%s] can't create file [%s] at line [%d] program [%s]",errname(),filename,line,source);
	} else fcntl(fd,F_SETFD,1);
#endif
#ifdef WIN32
	if(fd<0)
	{
		prterr("Error [%s] can't create file [%s] at line [%d] program [%s].",errname(),filename,line,source);
	}
#endif
	return(fd);
}

int xRopen(char *filename,char *source,int line)
{
	int fd;
#ifdef WIN32
	char *file2=NULL;
#endif

	if(diagrf)
	{
		prterr("DIAG Ropen Open File [%s] at line [%d] program [%s].",filename,line,source);
	}
#ifndef WIN32
	fd=open(filename,O_RDWR);
	if(fd<0)
	{
		prterr("Error [%s] can't open file [%s] at line [%d] program [%s].",errname(),filename,line,source);
	} else fcntl(fd,F_SETFD,1);
#endif
#ifdef WIN32
	if(!isEMPTY(filename))
	{
		if(filename[0]=='.') 
		{
			file2=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(filename)+15);
			if(RDAstrlen(filename)>1)
			{
				sprintf(file2,"%s%s",CURRENTDIRECTORY,&filename[1]);
			} else {
				if(file2!=NULL) Rfree(file2);
			}
		} else file2=stralloc(filename);
	} else file2=stralloc(filename);
	fd=open(file2,O_RDWR|O_BINARY);
	if(file2!=NULL) Rfree(file2);
	if(fd<0)
	{
		prterr("Error [%s] can't open file [%s] at line [%d] program [%s].",errname(),filename,line,source);
	}
#endif
	return(fd);
}

char xRseek(int fd,long pos,int mode,char *source,int line)
{
	long x;

	if(diagrf)
	{
		prterr("DIAG Rseek Seeking position [%d] in File Number [%d] Mode [%d] at line [%d] program [%s].",pos,fd,mode,line,source);
	}
	x=lseek(fd,pos,mode);
	if(mode==SEEK_SET)
	{
		if(pos==x) return(FALSE);
	} else {
		if(x>=0) return(FALSE);
	}
	prterr("Error [%s] seeking position [%d] file number [%d] mode [%d] at line [%d] program [%s].",errname(),pos,fd,mode,line,source);
	return(TRUE);
}

char xRlock(int cmd,short lock,int fd,long pos,long len,char *source,int line)
{
#ifndef WIN32
	struct flock lck;
#endif
#ifdef WIN32
#ifdef XXXX
	long lseek_length=0;
#endif /* XXXX */
#endif

	if(diagrf)
	{
		prterr("DIAG Rlock Locking Record cmd [%d] lock [%d] file number [%d] pos [%d] length [%d] at line [%d] program [%s].",cmd,lock,fd,pos,len,line,source);
	}
#ifndef WIN32
	lck.l_type=lock;
	lck.l_whence=0;
	lck.l_start=pos;
	lck.l_len=len;
	if(fcntl(fd,cmd,&lck)!=(-1)) 
	{
		if(diagrf)
		{
			prterr("DIAG achieved fcntl lock at line [%d] program [%s].",line,source);
		}
		return(FALSE);
	}
	if(cmd==F_SETLKW)/*sleeping lock */
	{
		prterr("Error [%s] locking cms [%d] lock [%d] pos [%d] len [%d] at line [%d] program [%s].",errname(),cmd,lock,pos,len,line,source);
	}
	return(TRUE);
#endif
#ifdef WIN32
#ifdef XXXX
	lseek_length=lseek(fd,pos,SEEK_SET);
	if(_locking(fd,_LK_LOCK,len)==(-1))
	{
		prterr("Error couldn't achieve _locking at line [%d] program [%s].",line,source); 
		return(TRUE);
	} else {
		return(0);
	}
#endif /* XXXX */
	return(0);
#endif
}

int xRvread(int fd,void *buf,int len,char *source,int line)
{
	int x;

	if(diagrf)
	{
		prterr("DIAG Rvread at line [%d] program [%s].",line,source);
	}
	x=read(fd,buf,len);
	if(x>=0) return(x);
	prterr("Error [%s] reading file at line [%d] program [%s]",errname(),line,source);
	return(x);
}

char xRread(int fd,void *buf,int len,char *source,int line)
{
	int x;

	if(diagrf)
	{
		prterr("DIAG Rread file [%d] length [%d] at line [%d] program [%s].",fd,len,line,source);
	}
	x=xRvread(fd,buf,len,source,line);
	if(x<0) return(TRUE);
	if(x==len) return(FALSE);
	prterr("Error partially read file [%d] got [%d] of [%d] at line [%d] program [%s].",fd,x,len,line,source);
	return(TRUE);
}

char xRreadlong(int fd,long *buf,char *source,int line)
{
	if(diagrf)
	{
		prterr("DIAG Rreadlong fd [%d] at line [%d] program [%s].",fd,line,source);
	}
	if(xRread(fd,buf,sizeof(long),source,line)) return(TRUE);
#ifdef RS6000
	reverse_memory((char *)buf,sizeof(long));
#endif
	return(FALSE);
}

char xRwrite(int fd,void *buf,int len,char *source,int line)
{
	int x;

	if(diagrf)
	{
		prterr("DIAG Rwrite Write to fd [%d] length [%d] at line [%d] program [%s].",fd,len,line,source);
	}
	x=write(fd,buf,len);
	if(x==len) return(FALSE);
	if(x<0)
	{
		prterr("Error [%s] writing at line [%d] program [%s].",errname(),line,source);
	} else {
		prterr("Error partial write did [%d] of [%d] at line [%d] program [%s].",x,len,line,source);
	}
	return(TRUE);
}

char xRwritelong(int fd,long buf,char *source,int line)
{
	if(diagrf)
	{
		prterr("DIAG Rwritelong File [%d] buf [%d] at line [%d] program [%s].",fd,buf,line,source);
	}
#ifdef RS6000
	reverse_memory((char *)&buf,sizeof(long));
#endif
	return(xRwrite(fd,&buf,sizeof(long),source,line));
}

void xRclose(int fd,char *source,int line)
{
	if(diagrf)
	{
		prterr("DIAG Rclose File [%d] at line [%d] program [%s].",fd,line,source);
	}
#ifdef WIN32
/*
	if(_commit(fd)==(-1))
	{
		prterr("Error [%s] commit file at line [%d] program [%s].",errname(),line,source);
	}
*/
#endif /* ifdef WIN32 */
	if(close(fd)>=0) return;
	prterr("Error [%s] closing file at line [%d] program [%s].",errname(),line,source);
}

