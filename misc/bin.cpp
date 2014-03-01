/*Binary Builder*/
#include <misc.hpp>

void xSeeBIN(BIN *b,int line,char *file)
{
	memset(errspace,0,4096);
	prterr("SeeBIN at line [%d] program [%s].",line,file);
	memset(errspace,0,4096);
	prterr("\talloc [%d] size [%d] read [%d]",b->alloc,b->size,b->read);
}

BIN *xBINnew(int size,int line,char *file)
{
	BIN *tmp;

	if(diagbin)
	{
		prterr("DIAG BINnew Creating / Allocating New Binary Structure at line [%d] program [%s].",line,file);
	}
	tmp=(BIN *)Rmalloc(sizeof(BIN));
	tmp->alloc=size;/* add to buffer in big chunks to save time.*/
	tmp->data=(char *)Rmalloc(tmp->alloc);
	tmp->size=sizeof(tmp->size);
	return(tmp);
}

int xBINlen(BIN *bin,int line,char *file)
{
	if(diagbin)
	{
		prterr("DIAG BINlen Returning Size of Binary [%d] at line [%d] program [%s].",bin->size,line,file);
	}
	return(bin->size);
}

void xreverse_memory(char *x,int size,int line,char *file)
{
	int y;
	char *z;
	char w;

	if(diagbin) 
	{ 
		prterr("DIAG Reversing Memory at line [%d] program [%s].",line,file); 
	}
	for(y=size/2,z=x+size-1;y>0;--y,++x,--z)
	{
		w=(*z);
		*z=(*x);
		*x=w;
	}
}

void xBINadd(BIN *bin,void *data,int size,int line,char *file)
{
	if(diagbin) 
	{ 
		prterr("DIAG BINadd Adding Data to Binary Sizeof [%d] at line [%d] program [%s].",size,line,file); 
	}
	if(bin->size+size>bin->alloc)
	{
		bin->alloc+=1024+size;
		bin->data=(char *)Rrealloc(bin->data,bin->alloc);
	}
	memset(bin->data+bin->size,0,size);
	memcpy(bin->data+bin->size,data,size);
	bin->size+=size;
}
void xBINaddbin(BIN *bin,BIN *tmpbin,char r,int line,char *file)
{
	if(diagbin) { prterr("DIAG BINaddbin Adding a Binary to a Binary at line [%d] program [%s].",line,file); }
	memcpy(tmpbin->data,&tmpbin->size,sizeof(tmpbin->size));
	if(r)
	{
		reverse_memory((char *)tmpbin->data,sizeof(tmpbin->size));
	}
	BINadd(bin,tmpbin->data,BINlen(tmpbin));
}

void xBINaddshort(BIN *bin,short x,char r,int line,char *file)
{
	if(diagbin) { prterr("DIAG BINaddshort Adding a Short Integer [%d] to a Binary at line [%d] program [%s].",x,line,file); }
	if(r)
	{
		reverse_memory((char *)&x,sizeof(x));
	}
	BINadd(bin,&x,sizeof(x));
}
void xBINaddint(BIN *bin,int x,char r,int line,char *file)
{
	if(diagbin) { prterr("DIAG BINaddint Adding an Integer [%d] to a Binary at line [%d] program [%s].",x,line,file); }
	if(r)
	{
		reverse_memory((char *)&x,sizeof(x));
	}
	BINadd(bin,&x,sizeof(x));
}
void xBINadddouble(BIN *bin,double x,char r,int line,char *file)
{
	if(diagbin) { prterr("DIAG BINadddouble Adding an Double [%f] to a Binary at line [%d] program [%s].",x,line,file); }
	if(r)
	{
		reverse_memory((char *)&x,sizeof(x));
	}
	BINadd(bin,&x,sizeof(x));
}
void xBINaddstringfixed(BIN *bin,char *x,int len,int line,char *file)
{
	if(diagbin) { prterr("DIAG BINaddstringfixed Adding a String [%s] to a Binary at line [%d] program [%s].",x,line,file); }
	BINadd(bin,x,len);
}

void xBINaddstring(BIN *bin,char *x,int line,char *file)
{
	if(diagbin) { prterr("DIAG BINaddstring Adding a String [%s] to a Binary at line [%d] program [%s].",x,line,file); }
	BINadd(bin,x,RDAstrlen(x)+1);
}

void xBINaddlong(BIN *bin,long x,char r,char m32,int line,char *file)
{
	int v=0;

	if(diagbin) { prterr("DIAG BINaddlong Adding a Long  [%ld] to a Binary at line [%d] program [%s].",x,line,file); }
	if(r)
	{
		reverse_memory((char *)&x,sizeof(x));
	}
	if(m32)
	{
		v=x;
		BINadd(bin,&v,sizeof(int));
	} else {
		BINadd(bin,&x,sizeof(x));
	}
}
void xBINaddulong(BIN *bin,unsigned long x,char r,int line,char *file)
{
	if(diagbin) { prterr("DIAG BINaddulong Adding an Unsigned Long [%ld] to a Binary at line [%d] program [%s].",x,line,file); }
	if(r)
	{
		reverse_memory((char *)&x,sizeof(x));
	}
	BINadd(bin,&x,sizeof(x));
}

int xBINwrite(int fd,BIN *bin,char r,int line,char *file)
{
	int err;

	if(diagbin) { prterr("DIAG BINwrite Writing Binary to File Pointer [%d] at line [%d] program [%s].",fd,line,file); }
	memcpy(bin->data,&bin->size,sizeof(bin->size));
	if(r)
	{
		reverse_memory((char *)bin->data,sizeof(bin->size));
	}
	err=write(fd,bin->data,bin->size);
	if(err!=(-1)) if(err!=bin->size)
	{
		prterr("Error Wrote %d of %d.",err,bin->size);
		if(err) if(lseek(fd,-(long)err,1)==(-1))
		{
			prterr("Error in lseek %s",errname());
		}
		err=(-1);
		errno=ENOSPC;
	}
	if(diagbin) 
	{ 
		if(err!=(-1))
		{
			prterr("DIAG Successful Write in BINwrite at line [%d] program [%s].",line,file); 
		} else {
			prterr("DIAG Errors Writing in BINwrite at line [%d] program [%s].",line,file); 
		}
	}
	return(err);
}
void xBINclear(BIN *bin,int line,char *file)
{
	if(diagbin) { prterr("DIAG BINclear Clearing Data Space at line [%d] program [%s].",line,file); }
	if(bin!=NULL)
	{
		if(bin->data!=NULL)
		{
			memset(bin->data,0,bin->alloc);
		}
		bin->size=sizeof(bin->size);
		bin->read=sizeof(bin->alloc);
	}
}

void xBINfree(BIN *bin,int line,char *file)
{
	if(diagbin) { prterr("DIAG BINfree Freeing Binary at line [%d] program [%s].",line,file); }
	if(bin!=NULL)
	{
		if(bin->data!=NULL) Rfree(bin->data);
		Rfree(bin);
	}
}

BIN *xBINread(int fd,char r,int line,char *file)
{
	BIN *tmp;
	int x;
	int y;

	if(diagbin) { prterr("DIAG BINread Reading Binary from File Pointer [%d] at line [%d] program [%s].",fd,line,file); }
	tmp=(BIN *)Rmalloc(sizeof(BIN));
	if(read(fd,&tmp->alloc,sizeof(tmp->alloc))!=sizeof(tmp->alloc))
	{
		prterr("Error can't read BIN Structure, %s from File Pointer [%d] at line [%d] program [%s].",errname(),fd,line,file);
		Rfree(tmp);
		return(NULL);
	}
	if(r)
	{
		reverse_memory((char *)&tmp->alloc,sizeof(tmp->alloc));
	}
	tmp->size=tmp->alloc;
	if(diagallocx && diagmisc)
	{
		tmp->data=(char *)xRmalloc(tmp->alloc,line,file);
	} else {
		tmp->data=(char *)Rmalloc(tmp->alloc);
	}
	if(tmp->data==NULL)
	{
		prterr("Error [%s] (Allocation) in BINread at line [%d] program [%s].",errname(),line,file);
	}
	x=tmp->alloc-sizeof(tmp->alloc);
	if(x>0)
	{
		y=read(fd,tmp->data+sizeof(tmp->alloc),x);
		if(y!=x)
		{
			prterr("Error can't read BIN structure (%d of %d), %s. Errors attempting BINread from line [%d] program [%s].",y,x,errname(),line,file);
			Rfree(tmp->data);
			Rfree(tmp);
			return(NULL);
		}
	}
	tmp->read=sizeof(tmp->alloc);
	if(diagbin) { prterr("DIAG Successful Read of Binary using BINread at line [%d] program [%s].",line,file); }
	return(tmp);
}

void xBINget(BIN *bin,void *data,int size,int line,char *file)
{
	if(diagbin) { prterr("DIAG BINget Attempting to Read [%d] Bytes from Binary. at line [%d] program [%s].",size,line,file); }
	if(bin->read+size>bin->size)
	{
		bin->read=bin->size;
		prterr("Error BIN overrun in BINget at line [%d] program [%s].",line,file);
		memset(data,0,size);
		return;
	}
	memcpy(data,bin->data+bin->read,size);
	bin->read+=size;
	if(diagbin) { prterr("DIAG BINget Successfully Read [%d] Bytes from Binary at line [%d] program [%s].",size,line,file); }
}
BIN *xBINgetbin(BIN *bin,char r,int line,char *file)
{
	BIN *tmp;

	if(diagbin) { prterr("DIAG BINgetbin Attempting to Read Binary Subfile from Binary at line [%d] program [%s].",line,file); }
	if(diagallocx && diagmisc)
	{
		tmp=(BIN *)xRmalloc(sizeof(BIN),line,file);
	} else {
		tmp=(BIN *)Rmalloc(sizeof(BIN));
	}
	BINget(bin,&tmp->alloc,sizeof(tmp->alloc));
	if(r)
	{
		reverse_memory((char *)&tmp->alloc,sizeof(tmp->alloc));
	}
	tmp->size=tmp->alloc;
	tmp->data=(char *)Rmalloc(tmp->alloc);
	if(tmp->data==NULL)
	{
		prterr("Errors in Allocation %s from line [%d] program [%s].",errname(),line,file);
	}
	BINget(bin,tmp->data+sizeof(tmp->alloc),tmp->alloc-sizeof(tmp->alloc));
	tmp->read=sizeof(tmp->alloc);
	if(diagbin) { prterr("DIAG Successfully Read Binary from Binary at line [%d] program [%s].",line,file); }
	return(tmp);
}

short xBINgetshort(BIN *bin,char r,int line,char *file)
{
	short x;
	if(diagbin) { prterr("DIAG BINgetshort Attempting to Read Short from Binary at line [%d] program [%s].",line,file); }
	BINget(bin,&x,sizeof(short));
	if(r)
	{
		reverse_memory((char *)&x,sizeof(x));
	}
	if(diagbin) { prterr("DIAG Read Short Value [%d] from Binary at line [%d] program [%s].",x,line,file); }
	return(x);
}
int xBINgetint(BIN *bin,char r,int line,char *file)
{
	int x;
	if(diagbin) { prterr("DIAG BINgetint Attempting to Read Integer from Binary at line [%d] program [%s].",line,file); }
	BINget(bin,&x,sizeof(int));
	if(r)
	{
		reverse_memory((char *)&x,sizeof(x));
	}
	if(diagbin) { prterr("DIAG Successfully Read Integer [%d] from Binary at line [%d] program [%s].",x,line,file); }
	return(x);
}
double xBINgetdouble(BIN *bin,char r,int line,char *file)
{
	double x;
	if(diagbin) { prterr("DIAG BINgetdouble Attempting to Read Double from Binary at line [%d] program [%s].",line,file); }
	BINget(bin,&x,sizeof(double));
	if(r)
	{
		reverse_memory((char *)&x,sizeof(x));
	}
	if(diagbin) { prterr("DIAG Successfully Read double value [%f] from binary at line [%d] program [%s].",x,line,file); }
	return(x);
}
int xBINgetstrlen(BIN *bin,int line,char *file)
{
	if(bin->read+1>bin->size)
	{
		prterr("Error BIN overrun in BINgetstrlen at line [%d] program [%s].",line,file);
		return(0);
	}
	return(RDAstrlen(bin->data+bin->read));
}
char *xBINgetstring(BIN *bin,int line,char *file)
{
	char *temp;

	if(diagbin) { prterr("DIAG BINgetstring Attempting to Read String from Binary at line [%d] program [%s].",line,file); }
	if(bin->read+1>bin->size)
	{
		bin->read=bin->size;
		prterr("Error BIN overrun in BINgetstring at line [%d] program [%s].",line,file);
		return(NULL);
	}
	if(diagmisc && diagallocx)
	{
		temp=xstralloc(bin->data+bin->read,line,file);
	} else {
		temp=stralloc(bin->data+bin->read);
	}
	bin->read+=RDAstrlen(temp)+1;
	if(diagbin) { prterr("DIAG Successfully Read String [%s] from Binary at line [%d] program [%s].",temp,line,file); }
	return(temp);
}

long xBINgetlong(BIN *bin,char r,char m32,int line,char *file)
{
	int v=0;
	long x;
	if(diagbin) { prterr("DIAG BINgetlong Attempting to Read Long from Binary at line [%d] program [%s].",line,file); }
	if(m32)
	{
		BINget(bin,&v,sizeof(int));
		if(r)
		{
			reverse_memory((char *)&v,sizeof(v));
		}
		x=v;	
	} else {
		BINget(bin,&x,sizeof(long));
		if(r)
		{
			reverse_memory((char *)&x,sizeof(x));
		}
	}
	if(diagbin) { prterr("DIAG Successfully Read Long [%ld] from Binary at line [%d] program [%s].",x,line,file); }
	return(x);
}
unsigned long xBINgetulong(BIN *bin,char r,int line,char *file)
{
	unsigned long x;
	if(diagbin) { prterr("DIAG BINgetulong Attempting to Read Unsigned Long from Binary at line [%d] program [%s].",line,file); }
	BINget(bin,&x,sizeof(unsigned long));
	if(r)
	{
		reverse_memory((char *)&x,sizeof(x));
	}
	if(diagbin) { prterr("DIAG Successfully Read Unsigned Long [%u] from Binary at line [%d] program [%s].",x,line,file); }
	return x;
}

BIN *BINmemread(char *mem)
{
	BIN *tmp;
	int x;

	tmp=(BIN *)Rmalloc(sizeof(BIN));
	memcpy(&tmp->alloc,mem,sizeof(tmp->alloc));
#ifdef RS6000
	reverse_memory((char *)&tmp->alloc,sizeof(tmp->alloc));
#endif
	tmp->size=tmp->alloc;
	tmp->data=(char *)Rmalloc(tmp->alloc);
	x=tmp->alloc-sizeof(tmp->alloc);
	if(x>0) memcpy(tmp->data+sizeof(tmp->alloc),mem+sizeof(tmp->alloc),x);
	tmp->read=sizeof(tmp->alloc);
	return(tmp);
}

void BINmemwrite(BIN *bin,char *mem)
{
	memcpy(bin->data,&bin->size,sizeof(bin->size));
#ifdef RS6000
	reverse_memory((char *)bin->data,sizeof(bin->size));
#endif
	memcpy(mem,bin->data,bin->size);
}
short xBINreadReUse(BIN *tmp,int fd,char r,int line,char *file)
{
	int y;
	int asize=0;

	if(diagbin) { prterr("DIAG BINreadReUse Reading Binary from File Pointer [%d] at line [%d] program [%s].",fd,line,file); }
	BINclear(tmp);
	if(read(fd,&asize,sizeof(asize))!=sizeof(asize))
	{
		prterr("Error can't read BIN Structure, %s from File Pointer [%d] at line [%d] program [%s].",errname(),fd,line,file);
		BINclear(tmp);
		return(-1);
	}
	if(r)
	{
		reverse_memory((char *)&asize,sizeof(asize));
	}
	if((tmp->size+asize)>tmp->alloc)
	{
		if(diagallocx && diagmisc)
		{
			if(tmp->data!=NULL) 
			{
				tmp->data=(char *)xRrealloc(tmp->data,tmp->alloc+asize+1,
					line,file);
			} else {
				tmp->data=(char *)xRmalloc(tmp->alloc+asize+1,line,file);
				tmp->alloc=0;
			}
			tmp->alloc=tmp->alloc+asize+1;
		} else {
			if(tmp->data!=NULL) 
			{
				tmp->data=(char *)Rrealloc(tmp->data,tmp->alloc+asize+1);
			} else {
				tmp->data=(char *)Rmalloc(asize+1);
				tmp->alloc=0;
			}
			tmp->alloc=tmp->alloc+asize+1;
		}
		memset(tmp->data,0,tmp->alloc);
	}
	if(tmp->data==NULL)
	{
		prterr("Error [%s] (Allocation) in BINread at line [%d] program [%s].",errname(),line,file);
	}
	y=read(fd,tmp->data+tmp->size,asize);
	tmp->size+=(asize);
	if(y!=asize)
	{
		prterr("Error can't read BIN structure (%d of %d), %s. Errors attempting BINread from line [%d] program [%s].",y,asize,errname(),line,file);
		BINclear(tmp);
		return(-1);
	}
	if(diagbin) { prterr("DIAG Successful Read of Binary using BINread at line [%d] program [%s].",line,file); }
	return(0);
}
