#include<misc.hpp>

#define LOOP_COUNT	10000
#define LOOP_WAIT	100000
int xRopenfile(char *libname,int flags,int line,char *file)
{
	int fd=(-1);
	int count=0,ocount=0;

	if(diagblb)
	{
		prterr("DIAG Ropenfile at line [%d] program [%s].",line,file);
	}
	fd=open(libname,flags,0666);
	if(fd==(-1))
	{
		if(errno!=(ENOENT))
		{
		ocount=0;
		while(ocount<LOOP_COUNT)
		{
			fd=open(libname,flags,0666);
			if(fd==(-1))
			{
				count=0;
				while(count<LOOP_WAIT) ++count;
			}
			++ocount;
		}
		}
	}
	return(fd);
} 
BIN *xgetlibbin(char *libname,char *name,short showerror,int line,char *file)
{
	short x,numheads;
	int fd=(-1);
	BIN *mainbin,*bin;
	BINhead head;

	if(diagblb)
	{
		prterr("DIAG Getting Binary [%s] from Binary Library [%s] at line [%d] program [%s].",name,libname,line,file);
	}
	fd=Ropenfile(libname,O_RDONLY);
	if(fd==(-1))
	{
		if(showerror)
		{
			prterr("Error [%s] can't open Binary Library [%s] at line [%d] program [%s].",errname(),libname,line,file);
		}
		return(NULL);
	}
	if(access(libname,04))
	{
		if(close(fd)==(-1)) 
		{ 
			prterr("Error can't close Binary Library [%s] at line [%d] program [%s].",libname,line,file);
		}
		if(showerror)
		{
			prterr("Error in permissions, user can't READ Binary Library [%s] at line [%d] program [%s].",libname,line,file);
		}
		return(NULL);
	}
	mainbin=BINread(fd);
	if(mainbin==NULL)
	{
		prterr("Error can't read Binary Library [%s] at line [%d] program [%s].",libname,line,file);
		if(close(fd)==(-1)) {prterr("Error can't close Binary Library [%s] at line [%d] program [%s].",libname,line,file);}
		return(NULL);
	}
	x=BINgetshort(mainbin);
	if(x!=(-1701))
	{
		prterr("Error invalid version in binary library [%s] at line [%d] program [%s].  Library may be corrupted.",libname,line,file);
		if(close(fd)==(-1)) {prterr("Error can't close binary library [%s] at line [%d] program [%s].",libname,line,file);}
		return(NULL);
	}
	numheads=BINgetint(mainbin);
	if(diagblb)
	{
		prterr("DIAG Binary Library [%s] contains [%d] entries at line [%d] program [%s].",libname,numheads,line,file);
	}
	for(x=0;x<numheads;++x)
	{
		head.name=BINgetstring(mainbin);
		head.position=BINgetlong(mainbin);
		head.length=BINgetlong(mainbin);
		if(diagblb)
		{
			prterr("DIAG Library [%s] Binary # [%d] Name [%s] Position [%ld] Length [%ld] Searching for binary [%s].",libname,x,head.name,head.position,head.length,name);
		}
		if(!RDAstrcmp(head.name,name))
		{
			if(diagblb)
			{
				prterr("DIAG Attempting to Seek Position [%ld] Binary Library [%s].",head.position,libname);
			}
			mainbin->read=head.position+sizeof(int);
			bin=BINgetbin(mainbin);
			BINfree(mainbin);
			if(diagblb)
			{
				prterr("DIAG Found Binary [%s] in Binary Library [%s] returning sucessfully to line [%d] program [%s].",name,libname,line,file);
			}
			if(head.name!=NULL) Rfree(head.name);
			head.position=(head.length=0);
			if(close(fd)==(-1))
			{
				prterr("Error can't close Binary Library [%s] at line [%d] program [%s].",libname,line,file);
				return(NULL);
			}
			return(bin);
		}
		if(head.name!=NULL) Rfree(head.name);
		head.position=(head.length=0);
	}
	if(diagblb) 
	{ 
		prterr("Error Binary [%s] not found in Binary Library [%s] at line [%d] program [%s].",name,libname,line,file); 
	}
	BINfree(mainbin);
	if(close(fd)==(-1)) { prterr("Error can't close Binary Library [%s] at line [%d] program [%s].",libname,line,file); }
	return(NULL);
}
long xreadlibrary(BINlib *binlib,char *temp,short *add,BIN *bin,char *name,short errorx,int line,char *file)
{
	short x=0;
	int fd=(-1);
	BINhead *head=NULL;
	long new_pos=0;
	BIN *mainbin=NULL;

	if(diagblb) 
	{ 
		prterr("DIAG readlibrary Attempting to read existing Binary Library [%s] at line [%d] program [%s].",temp,line,file); 
	}
	fd=Ropenfile(temp,O_RDONLY);
	if(fd!=(-1))
	{
		mainbin=BINread(fd);
		if(mainbin==NULL)
		{
			prterr("Error can't read Binary Library [%s] at line [%d] program [%s].",temp,line,file);
			if(close(fd)==(-1)) {prterr("Error can't close Binary Libary [%s] at line [%d] program [%s].",temp,line,file);}
			if(unlink(temp)==(-1)) if(errno!=ENOENT) { prterr("Error [%s] can't remove Binary Library [%s] at line [%d] program [%s].",errname(),temp,line,file); }
			return(0);
		}
		if(close(fd)==(-1))
		{
			prterr("Error can't close binary library [%s].",temp);
			return(-1);
		}
		if(*add)
		{
			if(access(temp,02))
			{
				prterr("Error in permissions, user can't WRITE Binary Library [%s] at line [%d] program [%s].",temp,line,file);
				return(-1);
			}
			if(unlink(temp)==(-1)) if(errno!=ENOENT)
			{
				prterr("Error [%s] can't remove binary library [%s].",errname(),temp);
				return(-1);
			}
		}
		x=BINgetshort(mainbin);
		if(x!=(-1701))
		{
			prterr("Error invalid version or bad binary library [%s].  File nmay be corrupted at line [%d] program [%s].",temp,line,file);
			return(-1);
		}
		binlib->numlibs=BINgetint(mainbin);
		binlib->header=(BINhead *)Rmalloc((binlib->numlibs+1)*sizeof(BINhead));
		for(x=0,head=binlib->header;x<binlib->numlibs;++x,++head)
		{
			head->name=BINgetstring(mainbin);
			head->position=BINgetlong(mainbin);
			head->length=BINgetlong(mainbin);
			new_pos+=RDAstrlen(head->name)+1;
#ifdef RDA_64BITS
			new_pos+=sizeof(int);
			new_pos+=sizeof(int);
#else
			new_pos+=sizeof(long);
			new_pos+=sizeof(long);
#endif /* RDA_64BITS */
			if(diagblb)
			{
				prterr("DIAG Reading Header Name [%s] Position [%ld] Length [%ld] from Binary Library [%s].",head->name,head->position,head->length,temp);
			}
			if(!RDAstrcmp(head->name,name))
			{
				if(*add==1)
				{
					*add=0;
					if(diagblb) 
					{ 
						prterr("DIAG Header [%s] already exists in Binary Library [%s] at line [%d] program [%s], Rewriting Binary.",name,temp,line,file); 
					}
				} else if(*add==(-1))
				{
					new_pos-=(RDAstrlen(head->name)+1);
#ifdef RDA_64BITS
					new_pos-=sizeof(int);
					new_pos-=sizeof(int);
#else
					new_pos-=sizeof(long);
					new_pos-=sizeof(long);
#endif /* RDA_64BITS */
					head->length=(-1);
				}
			}
		}
		for(x=0,head=binlib->header;x<binlib->numlibs;++x,++head)
		{
			head->bin=BINgetbin(mainbin);
			if(diagblb)
			{
				prterr("DIAG Just Extracted Binary [%s] from Binary Library [%s] at line [%d] program [%s].",head->name,temp,line,file);
			}
			if(!*add && !RDAstrcmp(head->name,name))
			{
				if(diagblb) 
				{
					prterr("Replacing Binary [%s] with New Binary in Binary Library [%s] at line [%d] program [%s].",name,temp,line,file); 
				}
				BINfree(head->bin);
				if(head->length==(-1))
				{
					Rfree(head->name);
					head->name=NULL;
					head->position=0;
					head->bin=NULL;
				} else {
					head->bin=bin;
					head->length=BINlen(bin);
					head->position=0;
				}
			}
		}
		BINfree(mainbin);
		if(diagblb)
		{
			SEEBINLIB(binlib);
		}
		return(new_pos);
	}
	if(errorx)
	{
		prterr("Error couldn't open binary library [%s] at line [%d] program [%s].",temp,line,file);
	}
	return(0);
}
void xfreebinlib(BINlib *binlib,int line,char *file)
{
	short x;
	BINhead *head;

	if(diagblb)
	{
		prterr("DIAG freebinlib Free Binary Library Table of Contents at line [%d] program [%s].",line,file);
	}
	if(binlib!=NULL)
	{
		if(binlib->header!=NULL)
		{
			for(x=0,head=binlib->header;x<binlib->numlibs;++x,
				++head)
			{
				if(head->name!=NULL) Rfree(head->name);
				if(head->bin!=NULL) BINfree(head->bin);
			}
			Rfree(binlib->header);
		}
		Rfree(binlib);
		binlib=NULL;
	}
}
short xwritelibbin(char *libname,BIN *bin,char *binname,int line,char *file)
{
	short add=1;
	short x;
	int fd;
	BIN *mainbin;
	BINhead *head;
	long new_pos=sizeof(short)+sizeof(int),pos=0;
	BINlib *binlib;
	struct flock lck;

	if(diagblb)
	{
		prterr("DIAG writelibbin Writing Binary [%s] into Binary Library [%s] add [%d] at line [%d] program [%s].",binname,libname,add,line,file);
	}
	binlib=(BINlib *)Rmalloc(sizeof(BINlib));
	binlib->numlibs=0;
	pos=readlibrary(binlib,libname,&add,bin,binname,FALSE);
	if(pos==(-1))
	{
		pos=0;
		if(bin==NULL)
		{
			freebinlib(binlib);
			return(-1);
		}
	}
	fd=open(libname,O_WRONLY | O_CREAT | O_EXCL,0666);
	if(fd==(-1))
	{
		fd=open(libname,O_WRONLY | O_CREAT,0666);
		if(fd==(-1))
		{
			prterr("Error [%s] can't open binary library [%s] at line [%d] program [%s].",errname(),libname,line,file);
			freebinlib(binlib);
			return(-1);
		}
	}
	lck.l_type=F_WRLCK;
	lck.l_whence=0;
	lck.l_start=0;
	lck.l_len=0;
	fcntl(fd,F_GETLK,&lck);
	if(lck.l_type==F_UNLCK)
	{
		lck.l_type=F_WRLCK;
		lck.l_whence=0;
		lck.l_start=0;
		lck.l_len=0;
		if(fcntl(fd,F_SETLKW,&lck))
		{
			prterr("Error [%s] cannot lock binary library [%s] failed at line [%d] program [%s].",errname(),libname,line,file);
			freebinlib(binlib);
			return(-1);
		}
	} else {
		lck.l_type=F_WRLCK;
		lck.l_whence=0;
		lck.l_start=0;
		lck.l_len=0;
		if(fcntl(fd,F_SETLKW,&lck))
		{
			prterr("Error [%s] cannot lock binary library [%s] failed at line [%d] program [%s].",errname(),libname,line,file);
			freebinlib(binlib);
			return(-1);
		}
	}
	if(pos==0) binlib->header=(BINhead *)Rmalloc(sizeof(BINhead));
	new_pos+=pos;
	if(add>0)
	{
		head=binlib->header+binlib->numlibs;
		++binlib->numlibs;
		head->name=stralloc(binname);
#ifdef RDA_64BITS
		new_pos+=((2*sizeof(int))+RDAstrlen(head->name)+1);
#else
		new_pos+=((2*sizeof(long))+RDAstrlen(head->name)+1);
#endif /* RDA_64BITS */
		head->position=0;
		head->length=BINlen(bin);
		head->bin=bin;
		if(diagblb)
		{
			prterr("DIAG Adding Header Name [%s] Position [%ld] Length [%ld] to Binary Library [%s].",head->name,head->position,head->length,libname);
		}
	}
	for(x=0,head=binlib->header;x<binlib->numlibs;++x,++head)
	{
		if(head->name!=NULL)
		{
			if(diagblb)
			{
				prterr("DIAG Assigning Position [%d] to Header [%s] in Binary Library [%s].",new_pos,head->name,libname);
			}
			head->position=new_pos;
			new_pos+=BINlen(head->bin);
		}
	}
	mainbin=BINnew();
	BINaddshort(mainbin,(-1701));
	BINaddint(mainbin,binlib->numlibs);
	for(x=0,head=binlib->header;x<binlib->numlibs;++x,++head)
	{
		if(head->length>0 && head->name!=NULL)
		{
			if(diagblb)
			{
				prterr("DIAG Writing Header for Binary [%s] Position [%ld] Length [%ld] into Binary Library [%s].",head->name,head->position,head->length,libname);
			}
			BINaddstring(mainbin,head->name);
			BINaddlong(mainbin,head->position);
			BINaddlong(mainbin,head->length);
		}
	}
	for(x=0,head=binlib->header;x<binlib->numlibs;++x,++head)
	{
		if(head->length>0 && head->name!=NULL)
		{
			if(diagblb)
			{
				prterr("DIAG Writing Binary [%s] into Binary Library [%s].",head->name,libname);
			}
			BINaddbin(mainbin,head->bin);
		}
	}
	BINwrite(fd,mainbin);
	freebinlib(binlib);
	BINfree(mainbin);
	lck.l_type=F_UNLCK;
	lck.l_whence=0;
	lck.l_start=0;
	lck.l_len=0;
	if(fcntl(fd,F_SETLK,&lck))
	{
		prterr("Error [%s] cannot unlock binary library [%s] at line [%d] program [%s].",errname(),libname,line,file);
		freebinlib(binlib);
		return(-1);
	}
	if(close(fd)==(-1))
	{
		prterr("Error can't close binary library [%s] at line [%d] program [%s].",libname,line,file);
		return(-1);
	}
	return(0);
}
short xdeletelibbin(char *libname,char *name,int line,char *file)
{
	short x;
	int fd;
	BIN *mainbin;
	BINhead *head;
	long new_pos=sizeof(short)+sizeof(int),pos=0;
	BINlib *binlib;
	short add=(-1);

	if(diagblb)
	{
		prterr("DIAG deletelibbin Deleting Binary [%s] from Binary Library [%s] Add [%d] at line [%d] program [%s].",name,libname,add,line,file);
	}
	if(access(libname,02))
	{
		prterr("Error in permissions, user can't WRITE Binary Library [%s] at line [%d] program [%s].",libname,line,file);
		return(-1);
	}
	binlib=(BINlib *)Rmalloc(sizeof(BINlib));
	binlib->numlibs=0;
	pos=readlibrary(binlib,libname,&add,NULL,name,TRUE);
	if(pos==(-1))
	{
		prterr("Error cannot read existing Binary Library [%s] at line [%d] program [%s].",libname,line,file);
		freebinlib(binlib);
		return(-1);
	} else if(pos==0) 
	{
		/* removed the only binary in the binary library with the unlink */
		/* no reason to create a 0 byte binary library */
		return(0);
	}
	fd=open(libname,O_WRONLY | O_CREAT | O_EXCL,0666);
	if(fd==(-1))
	{
		prterr("Error [%s] can't open binary library [%s] at line [%d] program [%s].",errname(),libname,line,file);
		freebinlib(binlib);
		return(-1);
	}
	for(x=0,head=binlib->header;x<binlib->numlibs;++x,++head)
	{
		if(head->name!=NULL && head->length>0)
		{
#ifdef RDA_64BITS
			new_pos+=RDAstrlen(head->name)+1+sizeof(int)+sizeof(int);
#else
			new_pos+=RDAstrlen(head->name)+1+sizeof(long)+sizeof(long);
#endif /* RDA_64BITS */
		}
	}
	for(x=0,head=binlib->header;x<binlib->numlibs;++x,++head)
	{
		if(head->name!=NULL && head->length>0)
		{
			if(diagblb)
			{
				prterr("DIAG Assigning Position [%d] to Header [%s] in Binary Library [%s].",new_pos,head->name,libname);
			}
			head->position=new_pos;
			new_pos+=BINlen(head->bin);
		} else head->position=0;
	}
	mainbin=BINnew();
	BINaddshort(mainbin,(-1701));
	BINaddint(mainbin,(binlib->numlibs-1));
	for(x=0,head=binlib->header;x<binlib->numlibs;++x,++head)
	{
		if(head->length>0 && head->name!=NULL && head->position!=0)
		{
			if(diagblb)
			{
				prterr("DIAG Writing Header for Binary [%s] Position [%ld] Length [%ld] into Binary Library [%s].",head->name,head->position,head->length,libname);
			}
			BINaddstring(mainbin,head->name);
			BINaddlong(mainbin,head->position);
			BINaddlong(mainbin,head->length);
		}
	}
	for(x=0,head=binlib->header;x<binlib->numlibs;++x,++head)
	{
		if(head->name!=NULL && head->length>0 && head->position!=0)
		{
			if(diagblb)
			{
				prterr("DIAG Writing Binary [%s] Size [%d] into Binary Library [%s].",head->name,BINlen(head->bin),libname);
			}
			BINaddbin(mainbin,head->bin);
		}
	}
	BINwrite(fd,mainbin);
	freebinlib(binlib);
	BINfree(mainbin);
	if(close(fd)==(-1))
	{
		prterr("Error can't close binary library [%s] at line [%d] program [%s].",libname,line,file);
		return(-1);
	}
	return(0);
}
APPlib *xgetlibnames(char *libname,short sortlist,short errorx,int line,char *file)
{
	short x;
	BINhead *head;
	BINlib *binlib;
	APPlib *applib;
	long pos=0;
	short add=0;

	if(diagblb)
	{
		prterr("DIAG getlibnames Attempting to Read Contents of Binary Library [%s] at line [%d] program [%s].",libname,line,file);
	}
/*
	if(access(libname,04))
*/
	if(access(libname,00)==0)  /* check for existance of file */
	{
		if(access(libname,04)) /* check for read permissions */
		{
			prterr("Error in permissions, user can't READ Binary Library [%s] FAILED(errno=[%d]) at line [%d] program [%s].",libname,errno,line,file);
			return(NULL);
		}
	} else {
		if(diagblb)
		{
			prterr("Error getlibnames Attempt to READ Contents of Binary Library [%s] FAILED(errno=[%d]) because file doesn't exist at line [%d] program [%s].",libname,errno,line,file);
		}
		return(NULL);
	}
	binlib=(BINlib *)Rmalloc(sizeof(BINlib));
	binlib->numlibs=0;
	pos=readlibrary(binlib,libname,&add,NULL,NULL,errorx);
	if(pos==(-1) || binlib->numlibs==0)
	{
		if(errorx)
		{
			prterr("Error cannot read existing Binary Library [%s] at line [%d] program [%s].",libname,line,file);
		}
		freebinlib(binlib);
		return(NULL);
	}
	applib=APPlibNEW();
	for(x=0,head=binlib->header;x<binlib->numlibs;++x,++head)
	{
		addAPPlib(applib,head->name);
	}
	freebinlib(binlib);
	if(sortlist) SORTAPPlib(applib);
	return(applib);
}
void xSEEBINLIB(BINlib *binlib,int line,char *file)
{
	int x;
	BINhead *head;
	long new_pos=sizeof(short)+sizeof(int);

	prterr("DIAG SEEBINLIB Showing Contents and Positions of a Binary Library at line [%d] program [%s].",line,file);
/*lint -e740*/
	prterr("Number Library Entries [%d]",binlib->numlibs);
/*lint +e740*/
	for(x=0,head=binlib->header;x<binlib->numlibs;++x,++head)
	{
/*lint -e740*/
		prterr("Offset [%ld] Name of Binary [%s] Position [%ld] Length [%ld]",new_pos,head->name,head->position,head->length);
/*lint +e740*/
#ifdef RDA_64BITS
		new_pos+=RDAstrlen(head->name)+1+sizeof(int)+sizeof(int);
#else
		new_pos+=RDAstrlen(head->name)+1+sizeof(long)+sizeof(long);
#endif /* RDA_64BITS */
	}
}
