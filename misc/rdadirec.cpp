#include<misc.hpp>

char *xRDAcreatepath(char *dirname,int filemode,char **newdirname,int type,int line,char *file)
{
	int x=0,y=0;
	char *tempdir=NULL;
	char *tempdir2=NULL;
	char *return_value=NULL;
	int value=0;
	char firstfound=FALSE;
	char lastseperator=FALSE;

	char *absdirname=NULL;

	if(type==0)
	{
		absdirname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(dirname)+2);
		sprintf(absdirname,"%s/%s",CURRENTDIRECTORY,dirname);
	} else {
		absdirname=stralloc(dirname);
	}


	if(diagmisc && !diagallocx)
	{
		prterr("DIAG RDAcreatepath: Making [%s] Directory, line=[%d], program=[%s]",(absdirname!=NULL?absdirname:""),line,file);
	}
	tempdir=(char *)Rmalloc(RDAstrlen(absdirname)+5);
	for(y=0,x=0;x<(RDAstrlen(absdirname));++x)
	{
		if(x==0)
		{
			if((absdirname[x]!='\\') && (absdirname[x]!='/') &&
				(absdirname[x]!='.')
				)
			{
				tempdir[y]='.';
				y++;
				tempdir[y]='/';
				y++;
			}
			tempdir[y]=absdirname[x];
			y++;
			lastseperator=FALSE;
		} else if(absdirname[x]=='\\' || absdirname[x]=='/')
		{
			if(lastseperator==FALSE)
			{
				tempdir[y]='/';
				y++;
			}
			lastseperator=TRUE;
		} else {
			lastseperator=FALSE;
			tempdir[y]=absdirname[x];
			y++;
		} 
	}
	if(lastseperator==FALSE)
	{
		tempdir[y]='/';
		y++;
	}
	tempdir[y]=0;
	tempdir2=(char *)Rmalloc(RDAstrlen(tempdir)+1);
	for(x=0;x<RDAstrlen(tempdir);++x)
	{
		tempdir2[x]=0;
	}
	if(tempdir!=NULL)
	{
		firstfound=FALSE;
		for(x=0;x<RDAstrlen(tempdir);++x)
		{
			tempdir2[x]=tempdir[x];
			if((tempdir[x]=='\\') || (tempdir[x]=='/') || (x==(RDAstrlen(tempdir))-1))
			{
				if(firstfound==TRUE)
				{
					prterr("DIAG RDAcreatepath: calling RDAmkdir(%s), line=[%d], program=[%s]",(tempdir2!=NULL?tempdir2:""),line,file);
					return_value=RDAmkdir(tempdir2,filemode,&value);
					if(return_value!=NULL)
					{
						if(value!=EEXIST)
						{
							*newdirname=stralloc(tempdir);
							if(tempdir!=NULL) Rfree(tempdir);
							if(tempdir2!=NULL) Rfree(tempdir2);
							if(absdirname!=NULL) Rfree(absdirname);
							return(return_value);
						}
					}
				}
				firstfound=TRUE;
			}
		}
	}
	*newdirname=stralloc(tempdir);
	if(tempdir!=NULL) Rfree(tempdir);
	if(tempdir2!=NULL) Rfree(tempdir2);
	if(absdirname!=NULL) Rfree(absdirname);
	return(NULL);
}
char *xRDAmkdir(char *dirname,int filemode,int *value,int type,int line,char *file)
{
	char *temp1=NULL;
	int fd=(-1);
	char *absdirname=NULL;

	if(type==0)
	{
		absdirname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(dirname)+2);
		sprintf(absdirname,"%s/%s",CURRENTDIRECTORY,dirname);
	} else {
		absdirname=stralloc(dirname);
	}


	if(diagmisc && !diagallocx)
	{
		prterr("DIAG RDAmkdir: Making [%s] Directory, line=[%d], program=[%s]",(absdirname!=NULL?absdirname:""),line,file);
		
	}

	fd=mkdir(absdirname,(mode_t)filemode);
	if(fd)
	{
		temp1=stralloc(errname());
		if(diagmisc && !diagallocx)
		{
			prterr("DIAG RDAmkdir: Can't Create Directory [%s], %s\n",(absdirname!=NULL?absdirname:""),(temp1!=NULL?temp1:""));
		}
		switch(errno)
		{
			case EEXIST: 
				if(temp1!=NULL) Rfree(temp1);
				if(absdirname!=NULL) Rfree(absdirname);
				*value=EEXIST;
				return(NULL);
			case ENOENT: 
				*value=ENOENT;
				if(absdirname!=NULL) Rfree(absdirname);
				return(temp1);
			default :
				if(absdirname!=NULL) Rfree(absdirname);
				*value=errno;
				return(temp1);
		}
	} else {
		if(absdirname!=NULL) Rfree(absdirname);
		return(NULL);
	}
}
char *xRDAchown(char *filename,char *fileowner,int *value,int type,int line,char *file)
{
	int fd=(-1);
	uid_t temp_uid;
	gid_t temp_gid;
	char *temp1=NULL;
	char *abspathname=NULL;

	if(type==0)
	{
		abspathname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(filename)+2);
		sprintf(abspathname,"%s/%s",CURRENTDIRECTORY,filename);
	} else {
		abspathname=stralloc(filename);
	}

	if(diagmisc && !diagallocx)
	{
		prterr("DIAG RDAchown: Changing owner and group of File/Directory [%s] to [%s], line=[%d], program=[%s]",(filename!=NULL?filename:""),(fileowner!=NULL?fileowner:""),line,file);
	}
	temp_uid=getuid();
	temp_gid=getgid();
	fd=chown(abspathname,temp_uid,temp_gid);
	if(fd)
	{
		temp1=stralloc(errname());
		if(diagmisc && !diagallocx)
		{
			prterr("DIAG RDAchown: Can't Chown Owner/Group of File/Directory [%s], %s\n",(abspathname!=NULL?abspathname:""),(temp1!=NULL?temp1:""));
		}
		switch(errno)
		{
			default :
				if(abspathname!=NULL) Rfree(abspathname);
				*value=errno;
				return(temp1);
		}
	}
	if(abspathname!=NULL) Rfree(abspathname);
	return(NULL);
}
char *xRDAchmod(char *filename,int filemode,int *value,int type,int line,char *file)
{
	int fd=(-1);
	char *temp1=NULL;
	char *abspathname=NULL;

	if(type==0)
	{
		abspathname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(filename)+2);
		sprintf(abspathname,"%s/%s",CURRENTDIRECTORY,filename);
	} else {
		abspathname=stralloc(filename);
	}


	if(diagmisc && !diagallocx)
	{
		prterr("DIAG RDAchmod: Changing mode of File/Directory [%s] to [%d], line=[%d], program=[%s]\n",(abspathname!=NULL?abspathname:""),filemode,line,file);
	}

	fd=chmod(abspathname,(mode_t)filemode);
	if(fd)
	{
		temp1=stralloc(errname());
		if(diagmisc && !diagallocx)
		{
			prterr("DIAG RDAchmod: Can't Change Mode of File/Directory [%s], %s\n",(abspathname!=NULL?abspathname:""),(temp1!=NULL?temp1:""));
		}
		switch(errno)
		{
			default :
				*value=errno;
				return(temp1);
		}
	}
	if(abspathname!=NULL) Rfree(abspathname);
	return(NULL);
}
#ifdef NOT_READY_YET
char *xRDAmkdir(char *dirname,int filemode,int *value,int line,char *file)
{
	char *temp1=NULL;
	int fd=(-1);

	if(diagmisc && !diagallocx)
	{
		prterr("DIAG RDAmkdir: Making [%s] Directory, line=[%d], program=[%s]",(dirname!=NULL?dirname:""),line,file);
		
	}

	fd=mkdir(dirname,(mode_t)filemode);
	if(fd)
	{
		temp1=stralloc(errname());
		if(diagmisc && !diagallocx)
		{
			prterr("DIAG RDAmkdir: Can't Create Directory [%s], %s\n",(dirname!=NULL?dirname:""),(temp1!=NULL?temp1:""));
		}
		switch(errno)
		{
			case EEXIST: 
				if(temp1!=NULL) Rfree(temp1);
				*value=EEXIST;
				return(NULL);
			case ENOENT: 
				*value=ENOENT;
				return(temp1);
			default :
				*value=errno;
				return(temp1);
		}
	} else {
		return(NULL);
	}
}
char *xRDAchown(char *filename,char *fileowner,int *value,int line,char *file)
{
	int fd=(-1);
	uid_t temp_uid;
	gid_t temp_gid;
	char *temp1=NULL;

	if(diagmisc && !diagallocx)
	{
		prterr("DIAG RDAchown: Changing owner and group of File/Directory [%s] to [%s], line=[%d], program=[%s]",(filename!=NULL?filename:""),(fileowner!=NULL?fileowner:""),line,file);
	}
	temp_uid=getuid();
	temp_gid=getgid();
	fd=chown(filename,temp_uid,temp_gid);
	if(fd)
	{
		temp1=stralloc(errname());
		if(diagmisc && !diagallocx)
		{
			prterr("DIAG RDAchown: Can't Chown Owner/Group of File/Directory [%s], %s\n",(filename!=NULL?filename:""),(temp1!=NULL?temp1:""));
		}
		switch(errno)
		{
			default :
				*value=errno;
				return(temp1);
		}
	}
	return(NULL);
}
char *xRDAchmod(char *filename,int filemode,int *value,int line,char *file)
{
	int fd=(-1);
	char *temp1=NULL;

	if(diagmisc && !diagallocx)
	{
		prterr("DIAG RDAchmod: Changing mode of File/Directory [%s] to [%d], line=[%d], program=[%s]\n",(filename!=NULL?filename:""),filemode,line,file);
	}

	fd=chmod(filename,(mode_t)filemode);
	if(fd)
	{
		temp1=stralloc(errname());
		if(diagmisc && !diagallocx)
		{
			prterr("DIAG RDAchmod: Can't Change Mode of File/Directory [%s], %s\n",(filename!=NULL?filename:""),(temp1!=NULL?temp1:""));
		}
		switch(errno)
		{
			default :
				*value=errno;
				return(temp1);
		}
	}
	return(NULL);
}
#endif
