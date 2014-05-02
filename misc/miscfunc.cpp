/*miscfunc.c - misc. functions for utilities*/
#include <misc.hpp>

//#define USING_BOOST_FILESYSTEM 1
//#define BOOST_FILESYSTEM_VERSION 2
#ifdef USING_BOOST_FILESYSTEM
#include <boost/filesystem.hpp>
#endif

using namespace std;

char *bolstring[2]={"False","True"};
char *CALENDAR_MONTHS[12]={"JANUARY","FEBRUARY","MARCH","APRIL","MAY","JUNE","JULY","AUGUST","SEPTEMBER","OCTOBER","NOVEMBER","DECEMBER"};
short RDA_NOGUI=FALSE;

double stripfamt(char *data1)
{
	int x,y=0;
	char *data2=NULL;
	double amount;
	short neg_flag=FALSE;

	data2=(char *)Rmalloc(RDAstrlen(data1)+1);
	memset(data2,0,sizeof(data2));
	if(RDAstrstr(data1,"-")) neg_flag=TRUE;
	for(x=0;data1[x];++x)
	{
		if(isdigit(data1[x]))
		{
			data2[y]=data1[x];
			++y;
		}
	}
	if(!isEMPTY(data2))
	{
		amount=atof(data2);
	} else {
		amount=0;
	}
	if(neg_flag==TRUE)
	{
		amount*=(-1);
	}
	if(data2!=NULL) Rfree(data2); 
	return amount;
}
void stoupper(char *str)
{
	if(!isEMPTY(str)) for(;*str;++str) *str=toupper(*str);
}
void stolower(char *str)
{
	if(!isEMPTY(str)) for(;*str;++str) *str=tolower(*str);
}
void pad(char *data,int length)
{
	int x,start=RDAstrlen(data);

	if(!isEMPTY(data))
	{
		data+=start;	
		for(x=start;x<length;++x,++data)
		{
			*data=' ';
		}
	} else {
		memset(data,' ',length);
	}
}
void xunpad(char *data,int line,char *file)
{
	int x;

	if(diagmisc)
	{
		prterr("DIAG unpad of %p at line [%d] program [%s].",data,line,file);
	}
	if(isEMPTY(data)) return;
	x=RDAstrlen(data)-1;
	for(data+=x;x>=0 && *data==' ';--data,--x) *data=0;
	if(diagmisc)
	{
		prterr("DIAG unpad of %p at line [%d] program [%s].",data,line,file);
	}
}
char *stripdashes(char *data)
{
	short x;
	char *tmp=NULL;

	if(data==NULL) return(NULL);
	if(!isEMPTY(data))
	{
		if(data!=NULL) tmp=stralloc(data);
			else return(NULL);
		for(x=0;tmp[x];++x) if(tmp[x]=='_') tmp[x]=' ';
	}
	return tmp;
}
char *adddashes(char *data)
{
	short x;
	char *tmp=NULL;

	if(data==NULL) return(NULL);
	if(!isEMPTY(data))
	{

		tmp=stralloc(data);
		for(x=0;tmp[x];++x) if(tmp[x]==' ')  tmp[x]='_';
	}
	return tmp;
}
short patmatch(char *pattern,char *data)
{
	do
	{
		if(*pattern=='*')
		{
			++pattern;
			if(!*pattern) return TRUE;
			do
			{
				if(patmatch(pattern,data)) return TRUE;
				if(!*data) break;
				++data;
			} while(TRUE);
			return FALSE;
		}
		if(*pattern!='?') if(toupper((int)*pattern)!=toupper((int)*data))
			return FALSE;
		++pattern;
		if(!*data) break;
		++data;
	} while(TRUE);
	return TRUE;
}
short findfile(char *drv,char *pattern,char **tempname,int number)
{
	char drive[512];
	DIR *od;
	struct dirent *dp;

	if(drv[0])
	{
		if(drv[0]=='/') strcpy(drive,drv); else sprintf(drive,"./%s",drv);
	} else strcpy(drive,".");
	od=opendir(drive);
	if(od!=NULL)
	{
		dp=readdir(od);
		while(dp!=NULL)
		{
			if(*tempname!=NULL)
			{
				*tempname=(char *)Rrealloc(*tempname,RDAstrlen(dp->d_name)+1);
			} else {
				*tempname=(char *)Rmalloc(RDAstrlen(dp->d_name)+1);
			}
			strcpy(*tempname,dp->d_name);
			if(patmatch(pattern,*tempname)) if(--number==0)
			{
				closedir(od);
				return TRUE;
			}
			dp=readdir(od);
		}
		closedir(od);
	}
	return FALSE;
}
short vals(char *data,int rlen)
{
	char temp,*temp1=NULL;
	short num;

	temp1=stralloc(data);
	temp=data[rlen];
	if(temp=='-') temp1[rlen]=0;
	if(!isEMPTY(temp1))
	{
		num=Ratoi(temp1);
	} else num=0;
	if(temp=='-') num*=(-1);
	if(temp1!=NULL) Rfree(temp1);
	return(num);
}
int val(char *data,int rlen)
{
	char temp,*temp1=NULL;
	int num;

	temp1=stralloc(data);
	temp=data[rlen];
	if(temp=='-') temp1[rlen]=0;
	if(!isEMPTY(temp1))
	{
	num=Ratoi(temp1);
	} else num=0;
	if(temp=='-') num*=(-1);
	if(temp1!=NULL) Rfree(temp1);
	return(num);
}
double valf(char *data,int rlen)
{
	char temp,*temp1;
	double num;

	temp1=stralloc(data);
	temp=data[rlen];
	if(temp=='-') temp1[rlen]=0;
	if(!isEMPTY(temp1))
	{
	num=atof(temp1);
	} else num=0;
	if(temp=='-') num*=(-1);
	if(temp1!=NULL) Rfree(temp1);
	return(num);
}
long vall(char *data,int rlen)
{
	char temp,*temp1=NULL;
	long num;

	temp1=stralloc(data);
	temp=data[rlen];
	if(temp=='-') temp1[rlen]=0;
	num=atol(temp1);
	if(temp=='-') num*=(-1);
	if(temp1!=NULL) Rfree(temp1);
	return(num);
}
int xreadline(FILE *fd,char *buffer,int maxlen,int line,char *file)
{
	int x,y=1;
	unsigned maxlenus;

	if(diagmisc && !diagallocx)
	{
		prterr("DIAG readline at line [%d] program [%s].",line,file);
	}
	maxlenus=maxlen;
	x=fread(buffer,y,maxlenus,fd);
	if(x<0) x=0;
	buffer[x]=0;
	if(!x) return(-1);
	for(y=0;y<x;++y)
	{
		if(buffer[y]=='\n' || buffer[y]=='\r')
		{
			if(y+1<x)
			if((buffer[y+1]=='\n' || buffer[y+1]=='\r') && buffer[y]!=buffer[y+1])
			{
				buffer[y]=0;
				++y;
			}
			buffer[y]=0;
			++y;
			break;
		}
	}
	if(y<x) fseek(fd,(long)y-x,1);
	return((int)RDAstrlen(buffer));
}
#ifdef USING_BOOST_FILESYSTEM_TEST
int xCountLines(char *filename,int line,char *file)
{
	int x=0;
	try {
		std::ifstream (filename); 
		x=std::count(std::istreambuf_iterator<char>(inFile),std::istreambuf_iterator<char>(), '\n');
	} catch (std::exception e) {
		std::cout <<"Error reading file [" <<filename <<"]  "<< e.what() << std::endl;
		return(0);
	}
	return(x);
}


int xCountLines(char *filename,int line,char *file)
{
	boost::filesystem::path file(filename); 

	if (boost::filesystem::exists(file))    
	{
		if (boost::filesystem::is_regular_file(file))   
		{
			std::cout << file << " size is " << boost::filesystem::file_size(file) << '\n';
		}else{
			std::cout << file << "exists, but is neither a regular file nor a directory\n";
		}
	}else{
		std::cout << file << "does not exist\n";
	}
	return 0;	
}
#else
int xCountLines(char *filename,int line,char *file)
{
	FILE *p;
	char buffer[512],*temp=NULL;
	int count=0;

	temp=(char *)Rmalloc((filename!=NULL ?RDAstrlen(filename):0)+7);
	sprintf(temp,"wc -l %s",(filename!=NULL ? filename:""));
	p=popen(temp,"r");
	if(p!=NULL)
	{
		fgets(buffer,sizeof(buffer),p);
		if(!isEMPTY(buffer))
		{
		count=Ratoi(buffer);
		} else count=0;
		pclose(p);
	} else {
		prterr("Error couldn't call \"wc -l %s\" at line [%d] program [%s].",filename,line,file);
	}
	if(temp!=NULL) Rfree(temp);
	return(count);
}
#endif
int xLinesInTextFile(FILE *fd,int sizeline,int line,char *file)
{
	char *temp=NULL;
	int count=0,len=0,tmpsizeline=0;

	if(sizeline<1) tmpsizeline=1023;
		else tmpsizeline=sizeline;
	temp=(char *)Rmalloc(sizeline+1);
	while((len=readline(fd,temp,tmpsizeline))!=(-1)) 
	{
		if(diagmisc && !diagallocx)
		{
			prterr("DIAG readline read [%d] bytes.",len);
		}
		++count;
	}
	fseek(fd,0L,SEEK_SET);
	if(temp!=NULL) Rfree(temp);
	if(diagmisc && !diagallocx)
	{
		prterr("DIAG LinesInTextfile returns count of [%d] at line [%d] program [%s].",count,line,file);
	}
	return(count);
}
int xisEMPTY(char *d,int line,char *file)
{
	if(diagmisc && !diagallocx)
	{
		prterr("DIAG xisEMPTY testing pointer is empty at line [%d] program [%s].",line,file);
	}
	if(d==NULL)
	{
		return(TRUE);
	}
	if(strlen(d)==0) return(TRUE);
	else return(FALSE);
}
int xRDAstrlen(char *s,int line,char *file)
{
	if(diagmisc && !diagallocx)
	{
		prterr("DIAG RDAstrlen at line [%d] program [%s].",line,file);
	}
	if(!isEMPTY(s))
	{
		return(strlen(s));
	}
	return(0);
}
int xRDAstrcmp(char *s,char *t,int line,char *file)
{
	if(diagmisc && !diagallocx)
	{
		prterr("DIAG RDAstrcmp at line [%d] program [%s].",line,file);
	}
	if(isEMPTY(s) && isEMPTY(t))
	{
		return(0);
	} else if(!isEMPTY(s) && !isEMPTY(t))
	{
		return(strcmp(s,t));
	}
	return(-1);
}
char *xRDAstrstr(char *s,char *t,int line,char *file)
{
	if(diagmisc && !diagallocx)
	{
		prterr("DIAG RDAstrstr at line [%d] program [%s].",line,file);
	}
	if(isEMPTY(s) && isEMPTY(t))
	{
		if(t==NULL)
		{
			return(NULL);
		} else if(s==NULL)
		{
			return(NULL);
		} else {
			return(s);
		}
	} else if(!isEMPTY(s) && !isEMPTY(t))
	{
		return(strstr(s,t));
	} else if(!isEMPTY(t))
	{
		if(t==NULL)
		{
			return(NULL);
		} else {
			return(s);
		}
	}
	return(NULL);
}
char *fixdosname(char *name)
{
	int str_length=0,x=0,y=0;
	char *new_name=NULL;

	if(!isEMPTY(name))
	{
		str_length=RDAstrlen(name);
		for(x=0,y=0;x<str_length;++x)
		{
			if(name[x]=='/') y+=2;
			else if(name[x]=='\\') y+=2;
			else ++y;
		}
		new_name=(char *)Rmalloc(y+1);
		memset(new_name,0,y+1);
		for(x=0,y=0;x<str_length;++x)
		{
			if(name[x]=='/')
			{
				new_name[y]='\\';
				++y;
			} else if(name[x]=='\\')
			{
				new_name[y]='\\';
				++y;
			} else {
				new_name[y]=name[x];
				++y;
			}
		}
	}
	return(new_name);
}
char *fixunixname(char *name)
{
	int str_length=0,x;
	char *new_name=NULL;

	if(!isEMPTY(name))
	{
		str_length=RDAstrlen(name);
		new_name=(char *)Rmalloc(str_length+1);
		memset(new_name,0,str_length+1);
		for(x=0;x<str_length;++x)
		{
			if(name[x]=='\\')
			{
				new_name[x]='/';
			} else {
				new_name[x]=name[x];
			}
		}
	}
	return(new_name);
}
int xRatoi(char *x,int line,char *file)
{
	if(diagmisc)
	{
		prterr("DIAG Ratoi at line [%d] program [%s].",line,file);
	}
	if(!isEMPTY(x)) return(atoi(x));
		else return(0);
}

#ifdef USING_BOOST_FILESYSTEM
int xRDAMoveFile(char *fromfile,char *tofile,short type,int line,char *file)
{
	char *absfromfile=NULL,*abstofile=NULL;

	if(type==0)
	{
		absfromfile=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(fromfile)+2);
		sprintf(absfromfile,"%s/%s",CURRENTDIRECTORY,fromfile);
		abstofile=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(tofile)+2);
		sprintf(abstofile,"%s/%s",CURRENTDIRECTORY,tofile);
	} else {
		absfromfile=stralloc(fromfile);
		abstofile=stralloc(tofile);
	}

	boost::filesystem::path from (fromfile); 
	boost::filesystem::path to (tofile); 
	if (boost::filesystem::exists(from))   
	{
		try {
			boost::filesystem::rename(from,to);
		} catch( boost::filesystem::filesystem_error& e ) {
			if(diagmisc)
			{
				std::cout << "Error:  rename() failed.  Using Copy() instead. [" << e.what() <<"].   Called from [" <<file <<"]  line [" <<line <<"]."   <<std::endl ;
			}
			try {
				boost::filesystem::copy_file(fromfile,tofile,boost::filesystem::copy_option::overwrite_if_exists);
			} catch( boost::filesystem::filesystem_error& e ) {
				if(diagmisc)
				{
					std::cout << "Error:  Copy() failed for from[" <<from <<"]  to [" <<to <<"].  [" << e.what() <<"].   Called from  [" <<file <<"]  line [" <<line <<"]." <<std::endl ;
				}
				if(absfromfile!=NULL) Rfree(absfromfile);
				if(abstofile!=NULL) Rfree(abstofile);
				return(2);
			}

		}
	}else{
		if(diagmisc)
		{
			std::cout << "Error:  File from[" <<from <<"] does not exist." <<"  Called from [" <<file <<"]  line [" <<line <<"]." <<std::endl;
		}
		if(absfromfile!=NULL) Rfree(absfromfile);
		if(abstofile!=NULL) Rfree(abstofile);
		return(1);
	}
	if(absfromfile!=NULL) Rfree(absfromfile);
	if(abstofile!=NULL) Rfree(abstofile);
	return(0);
}

int xRDACopyFile(char *fromfile,char *tofile,short type,int line,char *file)
{
	char *absfromfile=NULL,*abstofile=NULL;

	if(type==0)
	{
		absfromfile=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(fromfile)+2);
		sprintf(absfromfile,"%s/%s",CURRENTDIRECTORY,fromfile);
		abstofile=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(tofile)+2);
		sprintf(abstofile,"%s/%s",CURRENTDIRECTORY,tofile);
	} else {
		absfromfile=stralloc(fromfile);
		abstofile=stralloc(tofile);
	}

	boost::filesystem::path from (absfromfile); 
	boost::filesystem::path to (abstofile); 

	if (boost::filesystem::exists(from))   
	{
		try {
			boost::filesystem::copy_file(fromfile,tofile,boost::filesystem::copy_option::overwrite_if_exists);
		} catch( boost::filesystem::filesystem_error& e ) {
			if(diagmisc)
			{
				std::cout << "Error:  Copy() failed for from[" <<from <<"]  to [" <<to <<"].  [" << e.what() <<"].   Called from [" <<file <<"]  line [" <<line <<"]."  <<std::endl ;
			}
			if(absfromfile!=NULL) Rfree(absfromfile);
			if(abstofile!=NULL) Rfree(abstofile);
			return(2);
		}
	}else{
		if(diagmisc)
		{
			std::cout << "Error:  File from[" <<from <<"] does not exist." <<"  Called from [" <<file <<"]  line [" <<line <<"]." <<std::endl;
		}
		if(absfromfile!=NULL) Rfree(absfromfile);
		if(abstofile!=NULL) Rfree(abstofile);
		return(1);
	}
	if(absfromfile!=NULL) Rfree(absfromfile);
	if(abstofile!=NULL) Rfree(abstofile);
	return(0);
}
#else

int xRDAMoveFile(char *fromfile,char *tofile,short type,int line,char *file)
{
	FILE *from=NULL,*to=NULL;
	char buffer[4096],*absfromfile=NULL,*abstofile=NULL;
	int x=0,y=0;

	if(type==0)
	{
		absfromfile=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(fromfile)+2);
		sprintf(absfromfile,"%s/%s",CURRENTDIRECTORY,fromfile);
		abstofile=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(tofile)+2);
		sprintf(abstofile,"%s/%s",CURRENTDIRECTORY,tofile);
	} else {
		absfromfile=stralloc(fromfile);
		abstofile=stralloc(tofile);
	}

	from=fopen(absfromfile,"r+b");
	to=fopen(abstofile,"w+b");
	if(from!=NULL && to!=NULL)
	{
		while(!feof(from)) 
		{
			x=fread(buffer,1,4096,from);
			if((ferror(from))!=0)
			{
				prterr("Error:  Reading %d bytes of %d.\n",y,x,abstofile);
				if(x>0)
				{
					y=fwrite(buffer,1,x,to);
					if((ferror(to))!=0)
					{
						prterr("Error:  Copying %d bytes of %d to [%s].\n",y,x,abstofile);
						fclose(from);
						fclose(to);
						if(absfromfile!=NULL) Rfree(absfromfile);
						if(abstofile!=NULL) Rfree(abstofile);
						return (5);
					}
				}
				fclose(from);
				fclose(to);
				if(absfromfile!=NULL) Rfree(absfromfile);
				if(abstofile!=NULL) Rfree(abstofile);
				return (3);
				break;
			}
			y=fwrite(buffer,1,x,to);
			if((ferror(to))!=0)
			{
				prterr("Error:  Copying %d bytes of %d to [%s].\n",y,x,abstofile);
				fclose(from);
				fclose(to);
				if(absfromfile!=NULL) Rfree(absfromfile);
				if(abstofile!=NULL) Rfree(abstofile);
				return (4);
				break;
			}
		}
		fclose(from);
		fclose(to);
		if((unlink(absfromfile))==0)
		{
			if(absfromfile!=NULL) Rfree(absfromfile);
			if(abstofile!=NULL) Rfree(abstofile);
			return (0);
		} else {
			prterr("Error:  Could not remove file [%s].\n",absfromfile);
			if(absfromfile!=NULL) Rfree(absfromfile);
			if(abstofile!=NULL) Rfree(abstofile);
			return (6);
		}
	} else if(from==NULL)
	{
		prterr("Error:  Couldn't open from file [%s] for copying.\n",absfromfile);
		fclose(to);
		if(absfromfile!=NULL) Rfree(absfromfile);
		if(abstofile!=NULL) Rfree(abstofile);
		return (7);
	} else if(to==NULL)
	{
		prterr("Error:  Couldn't open to file [%s] for copying.\n",abstofile);
		fclose(from);
		if(absfromfile!=NULL) Rfree(absfromfile);
		if(abstofile!=NULL) Rfree(abstofile);
		return (8);
	}

}
int xRDACopyFile(char *fromfile,char *tofile,short type,int line,char *file)
{
	FILE *from=NULL,*to=NULL;
	char buffer[4096],*absfromfile=NULL,*abstofile=NULL;
	int x=0,y=0;

	if(type==0)
	{
		absfromfile=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(fromfile)+2);
		sprintf(absfromfile,"%s/%s",CURRENTDIRECTORY,fromfile);
		abstofile=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(tofile)+2);
		sprintf(abstofile,"%s/%s",CURRENTDIRECTORY,tofile);
	} else {
		absfromfile=stralloc(fromfile);
		abstofile=stralloc(tofile);
	}
	from=fopen(absfromfile,"r+b");
	to=fopen(abstofile,"w+b");
	if(from!=NULL && to!=NULL)
	{
		while(!feof(from)) 
		{
			x=fread(buffer,1,4096,from);
			if((ferror(from))!=0)
			{
				prterr("Error:  Reading %d bytes of %d.\n",y,x,abstofile);
				if(x>0)
				{
					y=fwrite(buffer,1,x,to);
					if((ferror(to))!=0)
					{
						prterr("Error:  Copying %d bytes of %d to [%s].\n",y,x,abstofile);
						fclose(from);
						fclose(to);
						if(absfromfile!=NULL) Rfree(absfromfile);
						if(abstofile!=NULL) Rfree(abstofile);
						return (5);
					}
				}
				fclose(from);
				fclose(to);
				if(absfromfile!=NULL) Rfree(absfromfile);
				if(abstofile!=NULL) Rfree(abstofile);
				return (3);
			}
			y=fwrite(buffer,1,x,to);
			if((ferror(to))!=0)
			{
				prterr("Error:  Copying %d bytes of %d to [%s].\n",y,x,abstofile);
				fclose(from);
				fclose(to);
				if(absfromfile!=NULL) Rfree(absfromfile);
				if(abstofile!=NULL) Rfree(abstofile);
				return (4);
			}
		}
		
		fclose(from);
		fclose(to);
		if(absfromfile!=NULL) Rfree(absfromfile);
		if(abstofile!=NULL) Rfree(abstofile);
		return(0);
	} else if(from==NULL)
	{
		prterr("Error:  Couldn't open from file [%s] for copying.\n",absfromfile);
		fclose(to);
		if(absfromfile!=NULL) Rfree(absfromfile);
		if(abstofile!=NULL) Rfree(abstofile);
		return(1);
	} else if(to==NULL)
	{
		prterr("Error:  Couldn't open to file [%s] for copying.\n",abstofile);
		fclose(from);
		if(absfromfile!=NULL) Rfree(absfromfile);
		if(abstofile!=NULL) Rfree(abstofile);
		return(2);
	}

}
#endif

int xFileExists(char *filename,short type,int line,char *file)
{
	int r=0;
	char *absfilename=NULL;
#ifdef WIN32
	struct stat buffer;   
#endif

	if(diagmisc)
	{
		prterr("DIAG FileExists [%s] Relative Directory [%s] at line [%d] program [%s].",(filename!=NULL ? filename:""),(type==FALSE ? "Yes":"No"),line,file);
	}
	if(type==0)
	{
		absfilename=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(filename)+2);
		sprintf(absfilename,"%s/%s",CURRENTDIRECTORY,(filename!=NULL ? filename:""));
	} else absfilename=stralloc(filename);
#ifdef WIN32
	r=(stat (filename, &buffer) == 0);
	/* return (GetFileAttributes(filename) == INVALID_FILE_ATTRIBUTES) ? -1 : 0; */
#else
	r=(access(filename, R_OK) == 0);
#endif
	if(absfilename!=NULL) Rfree(absfilename);
	return(r);
}


#ifdef WIN32
DWORD RunSilent(const char* strCommand)
{
	STARTUPINFO StartupInfo;
	PROCESS_INFORMATION ProcessInfo;
	char Args[512];
	char *pEnvCMD = NULL;
	char *pDefaultCMD = "CMD.EXE";
	ULONG rc;
	
	memset(&StartupInfo, 0, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(STARTUPINFO);
	StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartupInfo.wShowWindow = SW_HIDE;

	Args[0] = 0;

	pEnvCMD = RDA_GetEnv("COMSPEC");

	if(pEnvCMD){		
		strcpy(Args, pEnvCMD);
	} else{
		strcpy(Args, pDefaultCMD);
	}

	/* "/c" option - Do the command then terminate the command window */
	strcat(Args, " /c "); 
	/*the application you would like to run from the command window */
	strcat(Args, strCommand);  

	if (!CreateProcess( NULL, Args, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, 
		NULL, 
		NULL,
		&StartupInfo,
		&ProcessInfo))
	{
		return GetLastError();		
	}

	WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
	if(!GetExitCodeProcess(ProcessInfo.hProcess, &rc))
		rc = 0;

	CloseHandle(ProcessInfo.hThread);
	CloseHandle(ProcessInfo.hProcess);

	return rc;
	
}

/*------------------------------------------------------------------------------
  Globals for the Routines pt_popen() / pt_pclose()
------------------------------------------------------------------------------*/
static HANDLE my_pipein[2], my_pipeout[2], my_pipeerr[2];
static char   my_popenmode = ' ';

static int
my_pipe(HANDLE *readwrite)
{
  SECURITY_ATTRIBUTES sa;

  sa.nLength = sizeof(sa);          /* Length in bytes */
  sa.bInheritHandle = 1;            /* the child must inherit these handles */
  sa.lpSecurityDescriptor = NULL;

  if (! CreatePipe (&readwrite[0],&readwrite[1],&sa,1 << 13))
  {
    errno = -1; /* EMFILE; que? */
    return -1;
  }

  return 0;
}

/*------------------------------------------------------------------------------
  Replacement for 'popen()' under WIN32.
  NOTE: if cmd contains '2>&1', we connect the standard error file handle
    to the standard output file handle.
------------------------------------------------------------------------------*/
FILE *
pt_popen(const char *cmd, const char *mode)
{
  FILE *fptr = (FILE *)0;
  PROCESS_INFORMATION piProcInfo;
  STARTUPINFO siStartInfo;
  int success, redirect_error = 0;
  char cmd_buff[2048];
  char *err2out;

  const char *shell_cmd = RDA_GetEnv("COMSPEC");
  if (! shell_cmd) shell_cmd = "cmd";
  strcpy(cmd_buff,shell_cmd);
  strcat(cmd_buff," /c ");
  strcat(cmd_buff,cmd);

  my_pipein[0]   = INVALID_HANDLE_VALUE;
  my_pipein[1]   = INVALID_HANDLE_VALUE;
  my_pipeout[0]  = INVALID_HANDLE_VALUE;
  my_pipeout[1]  = INVALID_HANDLE_VALUE;
  my_pipeerr[0]  = INVALID_HANDLE_VALUE;
  my_pipeerr[1]  = INVALID_HANDLE_VALUE;

  if (!mode || !*mode)
    goto finito;

  my_popenmode = *mode;
  if (my_popenmode != 'r' && my_popenmode != 'w')
    goto finito;

  /*
   * Shall we redirect stderr to stdout ? */
  if ((err2out = strstr("2>&1",cmd)) != NULL) {
     /* this option doesn't apply to win32 shells, so we clear it out! */
     strncpy(err2out,"    ",4);
     redirect_error = 1;
  }

  /*
   * Create the Pipes... */
  if (my_pipe(my_pipein)  == -1 ||
      my_pipe(my_pipeout) == -1)
    goto finito;
  if (!redirect_error && my_pipe(my_pipeerr) == -1)
    goto finito;

  /*
   * Now create the child process */
  ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
  siStartInfo.cb           = sizeof(STARTUPINFO);
  siStartInfo.hStdInput    = my_pipein[0];
  siStartInfo.hStdOutput   = my_pipeout[1];
  if (redirect_error)
    siStartInfo.hStdError  = my_pipeout[1];
  else
    siStartInfo.hStdError  = my_pipeerr[1];
  siStartInfo.dwFlags    = STARTF_USESTDHANDLES;

  success = CreateProcess(NULL,
     (LPTSTR)cmd_buff, 
     NULL,   
     NULL,    
     TRUE,
     DETACHED_PROCESS, 
     NULL, 
     NULL,
     &siStartInfo,
     &piProcInfo);

  if (!success)
    goto finito;

  /*
   * These handles listen to the Child process */
  CloseHandle(my_pipein[0]);  my_pipein[0]  = INVALID_HANDLE_VALUE;
  CloseHandle(my_pipeout[1]); my_pipeout[1] = INVALID_HANDLE_VALUE;
  CloseHandle(my_pipeerr[1]); my_pipeerr[1] = INVALID_HANDLE_VALUE;

  if (my_popenmode == 'r')
    fptr = _fdopen(_open_osfhandle((long)my_pipeout[0],_O_BINARY),"r");
  else
    fptr = _fdopen(_open_osfhandle((long)my_pipein[1],_O_BINARY),"w");

finito:
  if (!fptr)
  {
    if (my_pipein[0]  != INVALID_HANDLE_VALUE)
      CloseHandle(my_pipein[0]);
    if (my_pipein[1]  != INVALID_HANDLE_VALUE)
      CloseHandle(my_pipein[1]);
    if (my_pipeout[0] != INVALID_HANDLE_VALUE)
      CloseHandle(my_pipeout[0]);
    if (my_pipeout[1] != INVALID_HANDLE_VALUE)
      CloseHandle(my_pipeout[1]);
    if (my_pipeerr[0] != INVALID_HANDLE_VALUE)
      CloseHandle(my_pipeerr[0]);
    if (my_pipeerr[1] != INVALID_HANDLE_VALUE)
      CloseHandle(my_pipeerr[1]);
  }
  return fptr;

}

/*------------------------------------------------------------------------------
  Replacement for 'pclose()' under WIN32
------------------------------------------------------------------------------*/
int
pt_pclose(FILE *fle)
{
  if (fle)
  {
    (void)fclose(fle);

    CloseHandle(my_pipeerr[0]);
    if (my_popenmode == 'r')
      CloseHandle(my_pipein[1]);
    else
     CloseHandle(my_pipeout[0]);
    return 0;
  }
  return -1;
}
#endif /* WIN32 */
/* 
int (*UI_RDA_SetEnv)(char *,char *,int,char *);
int (*UI_RDA_UnSetEnv)(char *,int,char *);
int (*UI_RDA_PutEnv)(char *,int,char *);
*/
int xRDA_SetEnv(char *name, char *value,int line,char *file)
{
#ifdef WIN32
	int ret1=0;
	char *temp=NULL;
#endif

	if(RDA_NOGUI==TRUE || UI_RDA_SetEnv==NULL)
	{
#ifdef WIN32
	temp=Rmalloc((RDAstrlen(name))+(RDAstrlen(value))+3);
	
	strcat(temp,name);
	strcat(temp,"=");
	strcat(temp,value);
	ret1=_putenv(temp);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmisc)
	{
		if(ret1==0)
		{
			prterr("RDA_UnSetEnv:  _putenv returned zero.  Name [%s] at line [%d] program [%s].",name,line,file);
		} else {
			prterr("RDA_UnSetEnv:  Error _putenv returned non zero.  Name [%s] at line [%d] program [%s].",name,line,file);
		}
	}
#endif /* USE_RDA_DIAGNOSTICS */
	Rfree(temp);
	return(ret1);
#else
	return(setenv(name,value,TRUE));
#endif
	} else {
		return(UI_RDA_SetEnv(name,value,line,file));
	}
}
int xRDA_UnSetEnv(char *name,int line,char *file)
{
#ifdef WIN32
	int ret1=0,ret2=0;
	char temp[1024];
	memset(temp,0,1024);
#endif

	if(RDA_NOGUI==TRUE || UI_RDA_UnSetEnv==NULL)
	{
#ifdef WIN32
	strcat(temp,name);
	strcat(temp,"=");

	ret1=_putenv(temp);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmisc)
	{
		if(ret1==0)
		{
			prterr("RDA_UnSetEnv:  _putenv returned zero.  Name [%s] at line [%d] program [%s]",name,line,file);
		}else{
			prterr("RDA_UnSetEnv:  Error _putenv returned non zero.  Name [%s] at line [%d] program [%s]",name,line,file);
		}
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(ret1);
#else
	return(unsetenv(name));
#endif
	} else {
		return(UI_RDA_UnSetEnv(name,line,file));
	}
}
int xRDA_PutEnv(char *nameplusvalue,int line,char *file)
{
#ifdef WIN32
	int ret1=0;
#endif

	if(RDA_NOGUI==TRUE || UI_RDA_UnSetEnv==NULL)
	{
#ifdef WIN32
	ret1=_putenv(nameplusvalue);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmisc)
	{
		if(ret1==0)
		{
			prterr("RDA_PutEnv:  _putenv returned zero.  Name+Value [%s] at line [%d] program [%s]",nameplusvalue,line,file);
		}else{
			prterr("RDA_PutEnv:  Error _putenv returned non zero.  Name [%s] at line [%d] program [%s]",nameplusvalue,line,file);
		}
	}
#endif /* USE_RDA_DIAGNOSTICS */

	return(ret1);
#else
	return(putenv(nameplusvalue));
#endif
	} else {
		return(UI_RDA_PutEnv(nameplusvalue,line,file));
	}
}
char *xRDA_GetEnv(char *v,int line,char *file)
{
	char *temp1=NULL;

	if(diagmisc)
	{
		prterr("DIAG RDA_GetEnv [%s] at line [%d] program [%s].",(v!=NULL ? v:""),line,file);
	}
	if(RDA_NOGUI==TRUE || UI_RDA_GetEnv==NULL)
	{
		return(getenv(v));
	} else {
		return(UI_RDA_GetEnv(v,line,file));
	}
}
