#include <misc.hpp>
#define SIGWORD	0x33BBAA55
int alloccount=0;
int freecount=0;
#define ALLOC_EXTRA	1

struct Rmds
{
	char *ptr;
	int size;
	int line;
	char *file;
	struct Rmds *next;
} *Rmd=NULL;

char *xstralloc(char *d,int line,char *file)
{
	char *temp;

	if(diagmisc && !diagallocx)
	{
		prterr("DIAG xstralloc String Allocated at line [%d] program [%s].",line,file);	
	}
	if(isEMPTY(d)) return(NULL);
	if(diagallocx && diagmisc)
	{		
		temp=(char *)xRmalloc(RDAstrlen(d)+1,line,file);
	} else {
		temp=(char *)Rmalloc(RDAstrlen(d)+1);
	}
	strcpy(temp,d);
	return(temp);
}
char *xstralloccat(char *base,char *add,int line,char *file)
{
	if(diagmisc && !diagallocx)
	{
		prterr("DIAG xstralloccat String Allocated at line [%d] program [%s].",line,file);	
	}
	if(base!=NULL)
	{
		if(diagallocx && diagmisc)
		{		
			base=(char *)xRrealloc(base,RDAstrlen(base)+RDAstrlen(add)+1,line,file);
		} else {
			base=(char *)Rrealloc(base,RDAstrlen(base)+RDAstrlen(add)+1);
		}
		if(base!=NULL) strcat(base,add);
	} else {
		if(diagallocx && diagmisc)
		{		
			base=xstralloc(add,line,file);
		} else {
			base=stralloc(add);
		}
	}
	return(base);
}
short checkallocdiag(int line,char *file)
{
	struct Rmds *md;
	char *tmp;
	short retval=FALSE;

	for(md=Rmd;md!=NULL;md=md->next) if(md->ptr!=NULL)
	{
		tmp=md->ptr-sizeof(unsigned);
		if(*(unsigned *)tmp!=(unsigned)SIGWORD)
		{
			prterr("Error ALLOC ptr=%p head damaged alloc=%d,%s cur=%d,%s\n",md->ptr,md->line,md->file,line,file);
			retval=(-1);
		}
		tmp+=md->size+sizeof(unsigned);
		if(*(unsigned *)tmp!=(unsigned)SIGWORD)
		{
			prterr("Error ALLOC ptr=%p foot damaged alloc=%d,%s cur=%d,%s\n",md->ptr,md->line,md->file,line,file);
			retval=(-1);
		}
	}
	return(retval);
}

void *diagRmalloc(unsigned size,int line,char *file)
{
	char *tmp;
	struct Rmds *md;

	checkallocdiag(line,file);
	for(md=Rmd;md!=NULL;md=md->next) if(md->ptr==NULL) break;
	if(md==NULL)
	{
		md=(struct Rmds *)malloc(sizeof(struct Rmds));
		md->next=Rmd;
		Rmd=md;
	}
	md->size=size;
	md->line=line;
	md->file=file;
	md->ptr=(char *)malloc(size+sizeof(unsigned)*2);
	if(md->ptr==NULL || size==0)
	{
		prterr("Error in Allocation size=%u%s at line [%d] program [%s].",size,errname(),line,file);
	}
	tmp=md->ptr;
	md->ptr+=sizeof(unsigned);
	*(unsigned *)tmp=(unsigned)SIGWORD;
	tmp+=sizeof(unsigned)+size;
	*(unsigned *)tmp=(unsigned)SIGWORD;
	return(md->ptr);
}
void *xRmalloc(unsigned size,int line,char *file)
{
	void *tmp=NULL;
	unsigned a_size=0;

	a_size=size+ALLOC_EXTRA;
	if(diagalloc)
	{
		prterr("DIAG xRmalloc(size=%u,line=%d,file=%s)",size,line,file);
		tmp=diagRmalloc(a_size,line,file);
	} else if(diagbadalloc)
	{
		tmp=diagRmalloc(a_size,line,file);
	} else tmp=malloc(a_size);
	if(tmp==NULL)
	{
		prterr("Error in Allocation size=%u%s at line [%d] program [%s].",size,errname(),line,file);

	}
	memset(tmp,0,a_size);
	if(diagallocx)
	{
/*lint -e740*/
		prterr("%p ALLOC %s %d %d",tmp,file,line,size);fflush(RDA_STDERR);
/*lint +e740*/
	}
	if(diagalloc)
	{
		prterr("DIAG return(%p) to Rmalloc at line [%d] program [%s].",tmp,line,file);
	}
	return(tmp);
}
void diagRfree(void *tmp,int line,char *file)
{
	char *adjptr;
	struct Rmds *md;

	checkallocdiag(line,file);
	for(md=Rmd;md!=NULL;md=md->next) if((void *)md->ptr==tmp) break;
	if(md==NULL)
	{
		prterr("Error in diagRfree freeing pointer not allocated with RDA at line [%d] program [%s].",line,file);
		free(tmp);
		return;
	}
	adjptr=(char *)tmp;
	adjptr-=sizeof(unsigned);
	md->ptr=NULL;
	free(adjptr);
}
void diagDCTfree(void *tmp,int line,char *file)
{
	free(((char *)tmp)-sizeof(unsigned));
}

void xRfree(void *tmp,int line,char *file)
{
	if(tmp==NULL)
	{
		prterr("Error freeing null pointer at line [%d] program [%s].",line,file);
		return;
	}
	if(diagalloc)
	{
		prterr("DIAG xRfree(addr=%p,line=%d,file=%s)",tmp,line,file);
		diagRfree(tmp,line,file);
	} else if(diagbadalloc)
	{
		diagRfree(tmp,line,file);
	} else free(tmp);
	if(diagallocx)
	{
/*lint -e644 */
/*lint -e740*/
		prterr("%p FREE %s %d",tmp,file,line);
/*lint +e740*/
/*lint +e644 */
/*lint -e740*/
		fflush(RDA_STDERR);
/*lint +e740*/
	}
}

void *diagRrealloc(void *tmp,unsigned size,int line,char *file)
{
	void *tmp1;
	struct Rmds *md;

	checkallocdiag(line,file);
	if(tmp==NULL)
	{
/*
		prterr("Error in ALLOC reallocation of NULL pointer at line [%d] program [%s].",line,file);
*/
		return(xRmalloc(size,line,file));
	}
	for(md=Rmd;md!=NULL;md=md->next) if((void *)md->ptr==tmp) break;
	if(md==NULL)
	{
		prterr("Error in ALLOC reallocation of non RDA pointer at line [%d] program [%s].",line,file);
		tmp1=realloc(tmp,size);
	} else {
		tmp1=malloc(size+sizeof(unsigned)*2);
		if(tmp1!=NULL)
		{
			tmp=tmp1;
			*(unsigned *)tmp1=(unsigned)SIGWORD;
			tmp1+=sizeof(unsigned)+(unsigned)size;
			*(unsigned *)tmp1=(unsigned)SIGWORD;
			tmp1=tmp;
			tmp1+=sizeof(unsigned);
			memset(tmp1,0,size);
			memcpy(tmp1,md->ptr,(size>md->size?md->size:size));
		}
		md->ptr-=sizeof(unsigned);
		free(md->ptr);
		md->ptr=(char *)tmp1;
		md->size=size;
	}
	return(tmp1);
}
void *xRrealloc(void *tmp,unsigned size,int line, char *file)
{
	unsigned a_size=0;

	a_size=size+ALLOC_EXTRA;
	if(diagallocx) if(tmp!=NULL)
	{
/*lint -e740*/
		prterr("%p FREE %s %d",tmp,file,line);
/*lint +e740*/
	}
	if(diagalloc)
	{
		prterr("DIAG xRrealloc(addr=%p,size=%u,line=%d,file=%s)",tmp,size,line,file);
		tmp=diagRrealloc(tmp,a_size,line,file);
	} else if(diagbadalloc)
	{
		tmp=diagRrealloc(tmp,a_size,line,file);
	} else {
		if(tmp!=NULL) tmp=realloc(tmp,a_size);
			else tmp=malloc(a_size);
	}
	if(diagallocx)
	{
/*lint -e740*/
		prterr("%p ALLOC %s %d %d",tmp,file,line,size);
/*lint +e740*/
	}
	if(tmp==NULL)
	{
		prterr("Error in Allocation Size %u %s at line [%d] program [%s].",size,errname(),line,file);
	}
	if(diagalloc)
	{
		prterr("DIAG return(%p) to Rrealloc at line [%d] program [%s].",tmp,line,file);
	}
	return(tmp);
}

void *xRcalloc(unsigned nelem,unsigned elsize,int line,char *file)
{
	void *tmp;
	if(diagalloc)
	{
		prterr("DIAG xRcalloc(addr,nelem=%u,elsize=%u,line=%d,file=%s)",nelem,elsize,line,file);
	}
	tmp=xRmalloc(nelem*elsize,line,file);
	if(tmp==NULL)
	{
		prterr("Error in Allocation Size=%ux%u%s at line [%d] program [%s].",
			nelem,elsize,errname(),line,file);
	}
	if(diagalloc)
	{
		prterr("DIAG return(%p) to Rcalloc at line [%d] program [%s].",tmp,line,file);
	}
	return(tmp);
}
