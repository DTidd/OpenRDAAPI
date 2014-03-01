/*vfile.c - rda variable file functions*/
#include<misc.hpp>

#define MAXALLOC(buf,max,new)   {if(max<new){max=new;buf=Rrealloc(buf,max);}}

static void VFaddfree(VF,long,long);
#define VFKEY_SIZE	100	
#define VFDAT_SIZE	100

static char VFbank(int fd,long size)
{
	char *bank;
	long x;
	long y;

	if(size>32000) x=32000; else x=size;
	bank=Rmalloc(x);
	memset(bank,254,x);
	for(x=0;x<size;x+=32000)
	{
		if(x+32000>size) y=size-x; else y=32000;
		if(Rwrite(fd,bank,y))
		{
			Rfree(bank);
			return(TRUE);
		}
	}
	Rfree(bank);
	return(FALSE);
}

static char xVFwl(char *filename,int fd,long x)
{
	if(Rwritelong(fd,x))
	{
		Rclose(fd);
		unlink(filename);
		return(TRUE);
	}
	return(FALSE);
}

char xVFcreat(char *filename,short style,int line,char *file)
{
	int fd;
	short y;
	char c;
	long VFsize;

	if(diagvf) { prterr("DIAG VFcreate creating variable length file [%s] at line [%d] program [%s].",filename,line,file); }
	fd=Rcreat(filename);
	if(fd<0) return(TRUE);

	/*write version*/
	if(!style)
	{
		if(xVFwl(filename,fd,-1706)) return(TRUE);
		VFsize=VFDAT_SIZE;
	} else {
		if(xVFwl(filename,fd,-1705)) return(TRUE);
		VFsize=VFKEY_SIZE;
	}
	/*offset record offset*/
	if(xVFwl(filename,fd,sizeof(long)*4+8)) return(TRUE);

	/*free record offset*/
	/*offset room=1 long number of recs, VFsize rec off/len, + bank*/
/* MAXREC */
	if(xVFwl(filename,fd,sizeof(long)*((VFsize*2)+7)+17)) return(TRUE);

	/*next record offset*/
	if(xVFwl(filename,fd,sizeof(long)*((VFsize*4)+10)+26)) return(TRUE);

	if(VFbank(fd,(long)8))
	{
		Rclose(fd);
		unlink(filename);
		return(TRUE);
	}
	/*initial offset record*/
	c=0;
	if(Rwrite(fd,&c,1))
	{
		Rclose(fd);
		unlink(filename);
		return(TRUE);
	}

	if(xVFwl(filename,fd,(long)-1)) return(TRUE);
	if(xVFwl(filename,fd,(long)VFsize)) return(TRUE);

	for(y=0;y<(2*VFsize+1);++y) 
	{
		if(xVFwl(filename,fd,(long)0)) return(TRUE);
	}

	/*bank after offset*/
	if(VFbank(fd,(long)8))
	{
		Rclose(fd);
		unlink(filename);
		return(TRUE);
	}
	/*initial free record*/
	c=0;
	if(Rwrite(fd,&c,1))
	{
		Rclose(fd);
		unlink(filename);
		return(TRUE);
	}

	if(xVFwl(filename,fd,(long)-2)) return(TRUE);
	if(xVFwl(filename,fd,(long)VFsize)) return(TRUE);

	for(y=0;y<(2*VFsize+1);++y)
	{
		if(xVFwl(filename,fd,(long)0)) return(TRUE);
	}
	if(VFbank(fd,(long)8))
	{
		Rclose(fd);
		unlink(filename);
		return(TRUE);
	}
	Rclose(fd);
	if(diagvf) { prterr("DIAG VFcreat Successfully created variable length file [%s] at line [%d] program [%s].",filename,line,file); }
	return(FALSE);
}

VF xVFopen(char *filename,int line,char *file)
{
	VF vf;
	long x;

	if(diagvf) { prterr("DIAG VFopen open variable length file [%s] at line [%d] program [%s].",filename,line,file); }
	vf=Rmalloc(sizeof(struct VFs));
	memset(vf,0,sizeof(struct VFs));
	vf->fd=Ropen(filename);
	if(vf->fd<0)
	{
		Rfree(vf);
		return(NULL);
	}
	if(Rreadlong(vf->fd,&x))
	{
		Rfree(vf);
		return(NULL);
	}
	if(x!=(-1706) && x!=(-1705))
	{
		prterr("Error wrong file version number [%d] in variable length file [%s] in VFopen at line [%d] program [%s].",x,filename,line,file);
		Rfree(vf);
		return(NULL);
	}
	if(x==(-1705)) vf->VFmaximum=VFKEY_SIZE;
		else vf->VFmaximum=VFDAT_SIZE;
	vf->filename=stralloc(filename);
	if(diagvf)
	{
		prterr("DIAG VFopen Successful open of file [%s] file pinter (fd) [%d] at line [%d] program [%s].",filename,vf->fd,line,file);
	}
	return(vf);
}

static char VFfindrec(VF vf,long recno)
{
	if(diagvf) { prterr("DIAG VFfindrec finding record number [%d] in datafile [%s].",recno,vf->filename); }
	if(recno<0) return(TRUE);
	/*read offset record offset*/
	if(Rseek(vf->fd,sizeof(long),SEEK_SET)) return(TRUE);
	if(Rreadlong(vf->fd,&vf->offrec)) return(TRUE);
	/*read offset record size & number of records*/
	if(Rseek(vf->fd,vf->offrec+sizeof(long)+1,SEEK_SET)) return(TRUE);
	if(Rreadlong(vf->fd,&vf->maxrec)) return(TRUE);
	if(Rreadlong(vf->fd,&vf->numrec)) return(TRUE);
	if(recno==0) return(FALSE);
	if(recno>vf->numrec)
	{
		if(diagvf)
		{
			prterr("Error trying to read record number [%d] in variable file [%s] in VFfindrec.",recno,vf->filename);
		}
		return(TRUE);/*no such record*/
	}
	/*read recno offset & length*/
	if(Rseek(vf->fd,vf->offrec+sizeof(long)*(recno*2+1)+1,SEEK_SET))
		return(TRUE);
	if(Rreadlong(vf->fd,&vf->recoff)) return(TRUE);
	if(Rreadlong(vf->fd,&vf->reclen)) return(TRUE);
	return(FALSE);
}

BIN *xVFread(VF vf,long recno,int line,char *file)
{
	BIN *bin;
	long x;

	if(diagvf) { prterr("DIAG VFread reading record number [%d] in variable length file [%s] at line [%d] program [%s].",recno,vf->filename,line,file); }
	if(recno<1)
	{
		prterr("Error invalid record number [%d] in variable file [%s] in VFread at line [%d] program [%s].",recno,vf->filename,line,file);
		return(NULL);
	}
	if(VFfindrec(vf,recno)) return(NULL);
	if(vf->recoff<0) return(NULL);/*deleted record*/
	/*read actual record*/
	if(Rseek(vf->fd,vf->recoff+1,SEEK_SET)) return(NULL);
	if(Rreadlong(vf->fd,&x)) return(NULL);
	if(x!=recno)
	{
		prterr("Error record number [%d] corrupted in variable file [%s] in VFread at line [%d] program [%s].",recno,vf->filename,line,file);
		return(NULL);
	}
	bin=BINread(vf->fd);
	if(BINlen(bin)+sizeof(long)+1!=vf->reclen)
	{
		prterr("Error length of binary does not match record in variable file [%s] in VFread at line [%d] program [%s].",vf->filename,line,file);
	}
	return(bin);
}

static char VFmovedata(VF vf,long fromoff,long tooff,long size)
{
	char *temp=NULL;
	long x;

	if(diagvf) { prterr("DIAG VFmovedata moving data from [%d] to [%d] sizeof [%d] in datafile [%s]",fromoff,tooff,size,vf->filename); }
	temp=Rmalloc(32000);
	while(size>0)
	{
		if(size>32000) x=32000; else x=size;
		if(Rseek(vf->fd,fromoff,SEEK_SET))
		{
			Rfree(temp);
			return(TRUE);
		}
		if(Rread(vf->fd,temp,x))
		{
			Rfree(temp);
			return(TRUE);
		}
		if(Rseek(vf->fd,tooff,SEEK_SET))
		{
			Rfree(temp);
			return(TRUE);
		}
		if(Rwrite(vf->fd,temp,x))
		{
			Rfree(temp);
			return(TRUE);
		}
		size-=32000;
		tooff+=32000;
		fromoff+=32000;
	}
	if(temp!=NULL) Rfree(temp);
	return(FALSE);
}

long ly[2000];

static char VFpackfree(VF vf)
{
	long lastrec;
	long newsize;
	short x;
	long z;
	short y;
	short w;
	long ly1,ly2;

	if(diagvf) { prterr("DIAG VFpackfree pack free records in datafile [%s]",vf->filename); }
	/*sort and pack*/
	if(Rseek(vf->fd,vf->offfree+sizeof(long)*3+1,SEEK_SET)) return(TRUE);
	for(z=0,newsize=1,y=0;z<vf->numfree;++z)
	{
		/*read chunk*/
		if(Rreadlong(vf->fd,&ly1)) return(TRUE);
		if(Rreadlong(vf->fd,&ly2)) return(TRUE);
		if(ly2>0) /*could be empty*/
		{
			/*find where this record goes.*/
			for(x=0;x<y;++x) if(ly1<ly[x*2])
			{
				if(ly1+ly2==ly[x*2])
				{
					/*record comes right before existing record*/
					ly[x*2]=ly1;
					ly[x*2+1]+=ly2;
					break;
				}
				if(x>0) if(ly1==ly[(x-1)*2]+ly[(x-1)*2+1])
				{
					/*record comes right after previous record*/
					ly[(x-1)*2+1]+=ly2;
					break;
				}
				/*add new*/
				for(w=y;w>x;--w)
				{
					ly[w*2+1]=ly[w*2-1];
					ly[w*2]=ly[w*2-2];
				}
				ly[x*2]=ly1;
				ly[x*2+1]=ly2;
				++y;
				break;
			}
			if(x>=y)
			{
				if(y>0) if(ly1==ly[(y-1)*2]+ly[(y-1)*2+1])
				{
					/*record comes right after last record*/
					ly[(y-1)*2+1]+=ly2;
					ly2=0;
				}
				if(ly2>0)
				{
					ly[y*2]=ly1;
					ly[y*2+1]=ly2;
					++y;
				}
			}
			if(y>=vf->VFmaximum)
			{
				/*buffer full write 75% recs*/
				if(Rseek(vf->fd,vf->offfree+sizeof(long)*(newsize*2+1)+1,SEEK_SET))
					return(TRUE);
				for(x=0;x<(vf->VFmaximum*3/2);++x) if(Rwritelong(vf->fd,ly[x])) return(TRUE);
				newsize+=vf->VFmaximum*3/4;
				y-=(vf->VFmaximum*3/4);
				/*move top 25% down*/
				for(x=0;x<(vf->VFmaximum*1/2);++x) ly[x]=ly[x+(vf->VFmaximum*3/2)];
				/*move file position back*/
				if(Rseek(vf->fd,vf->offfree+sizeof(long)*(z*2+5)+1,SEEK_SET))
					return(TRUE);
			}
		}
	}
	/*write rest of ly */
	if(Rseek(vf->fd,vf->offfree+sizeof(long)*(newsize*2+1)+1,SEEK_SET))
		return(TRUE);
	for(x=0;x<y*2;++x) if(Rwritelong(vf->fd,ly[x])) return(TRUE);
	newsize+=y;
	vf->numfree=newsize-1;
	/*write zero's*/
	for(;newsize<=vf->maxfree;++newsize) if(Rwritelong(vf->fd,(long)0))
		return(TRUE);
	/*write new number of free records*/
	if(Rseek(vf->fd,vf->offfree+sizeof(long)*2+1,SEEK_SET)) return(TRUE);
	if(Rwritelong(vf->fd,vf->numfree)) return(TRUE);
	/*if less than 75% left, need vf->VFmaximum more*/
	if(vf->numfree+(3/4*vf->VFmaximum)>vf->maxfree)
	{
		/*get last offset*/
		if(Rseek(vf->fd,sizeof(long)*3,SEEK_SET)) return(TRUE);
		if(Rreadlong(vf->fd,&lastrec)) return(TRUE);
		/*write new last offset*/
/* MAXREC */
		vf->maxfree+=vf->VFmaximum;
		if(Rseek(vf->fd,sizeof(long)*3,SEEK_SET)) return(TRUE);
		if(Rwritelong(vf->fd,lastrec+sizeof(long)*(3+vf->maxfree*2)+9))
			return(TRUE);
		/*copy record*/
		if(VFmovedata(vf,vf->offfree,lastrec,sizeof(long)*(vf->maxfree*2-((2*vf->VFmaximum)-3))+1))
			return(-1);
		/*fill new with zero's*/
		for(y=0;y<(vf->VFmaximum*2);++y) if(Rwritelong(vf->fd,(long)0)) return(TRUE);
		/*write bank*/
		if(VFbank(vf->fd,(long)8)) return(-1);
		/*write new offset*/
		if(Rseek(vf->fd,sizeof(long)*2,SEEK_SET)) return(TRUE);
		if(Rwritelong(vf->fd,lastrec)) return(TRUE);
		/*write new number of free records & max free records*/
		if(Rseek(vf->fd,lastrec+sizeof(long)+1,SEEK_SET)) return(TRUE);
		if(Rwritelong(vf->fd,vf->maxfree)) return(TRUE);
		/*have to do this here so that it will not get stuck in endless loop*/
		VFaddfree(vf,vf->offfree,sizeof(long)*(vf->maxfree*2-((2*vf->VFmaximum-3)))+9);
		vf->offfree=lastrec;
	}
	return(FALSE);
}

static char VFgetfreerec(VF vf)
{
	if(diagvf) { prterr("DIAG VFgetfreerec getting free record offset in datafile [%s].",vf->filename); }
	/*read offset of free record*/
	if(Rseek(vf->fd,sizeof(long)*2,SEEK_SET)) return(TRUE);
	if(Rreadlong(vf->fd,&vf->offfree)) return(TRUE);
	/*read max and length of free record*/
	if(Rseek(vf->fd,vf->offfree+sizeof(long)+1,SEEK_SET)) return(TRUE);
	if(Rreadlong(vf->fd,&vf->maxfree)) return(TRUE);
	if(Rreadlong(vf->fd,&vf->numfree)) return(TRUE);
	if(vf->numfree+3>vf->maxfree)/*will always need atleast 2 more*/
		if(VFpackfree(vf)) return(TRUE);
	return(FALSE);
}

static long VFgetfree(VF vf,long size)
{
	long getrec;
	long y;
	long ly1=0,ly2=0;

	if(diagvf) { prterr("DIAG VFgetfree getting a record of size [%d] in datafile [%s]",size,vf->filename); }
	if(VFgetfreerec(vf)) return(-1);
	/*search for chunk that is large enough*/
	/*calculate number to search*/
	if(Rseek(vf->fd,vf->offfree+sizeof(long)*3+1,SEEK_SET)) return(-1);
	for(y=0;y<vf->numfree;++y)
	{
		if(Rreadlong(vf->fd,&ly1)) return(-1);
		if(Rreadlong(vf->fd,&ly2)) return(-1);
		/*look for greater than or equal to size*/
		if(ly2>=size) break;
	}
	if(y>=vf->numfree)
	{
		/*none big enough, use next record*/
		/*read next record*/
		if(Rseek(vf->fd,sizeof(long)*3,SEEK_SET)) return(-1);
		if(Rreadlong(vf->fd,&getrec)) return(-1);
		/*write new next record*/
		if(Rseek(vf->fd,sizeof(long)*3,SEEK_SET)) return(-1);
		if(Rwritelong(vf->fd,getrec+size)) return(-1);
	} else {
		/*use current position*/
		getrec=ly1;
		/*write new offset for old free section*/
		if(Rseek(vf->fd,vf->offfree+sizeof(long)*(2*y+3)+1,SEEK_SET))
			return(-1);
		if(Rwritelong(vf->fd,getrec+size)) return(-1);
		/*write size, as old size less newsize, could be zero*/
		if(Rwritelong(vf->fd,ly2-size)) return(-1);
	}
	if(diagvf) { prterr("DIAG got record offset [%d] of [%d] in datafile [%s].",getrec,size,vf->filename); }
	return(getrec);
}

static void VFaddfree(VF vf,long off,long size)
{
	if(diagvf) { prterr("DIAG VFaddfree adding a record of size [%d] at [%d] in datafile [%s]",size,off,vf->filename); }
	if(VFgetfreerec(vf)) return;/*error!*/
	/*set last offset & length*/
	if(Rseek(vf->fd,vf->offfree+sizeof(long)*(vf->numfree*2+3)+1,SEEK_SET))
		return;
	if(Rwritelong(vf->fd,off)) return;
	if(Rwritelong(vf->fd,size)) return;
	/*write bank record*/
	if(Rseek(vf->fd,off,SEEK_SET)) return;
	if(VFbank(vf->fd,size)) return;
	/*write new number of free records*/
	++vf->numfree;
	if(Rseek(vf->fd,vf->offfree+sizeof(long)*2+1,SEEK_SET)) return;
	if(Rwritelong(vf->fd,vf->numfree)) return;
}

static void VFincrec(VF vf)
{
	long newrec;
	long x;

	if(diagvf) { prterr("DIAG VFincrec increasing record offset record in datafile [%s].",vf->filename); }
	/*increase size of offset record, by vf->VFmaximum records*/
	newrec=VFgetfree(vf,sizeof(long)*(((2*vf->VFmaximum)+3)+vf->maxrec*2)+9);
	if(newrec<0) return;
	/*move data*/
	if(VFmovedata(vf,vf->offrec,newrec,sizeof(long)*(3+vf->maxrec*2)+1)) return;
	/*zero area*/
	for(x=0;x<(vf->VFmaximum*2);++x) if(Rwritelong(vf->fd,(long)0)) return;
	if(VFbank(vf->fd,(long)8)) return;
	/*write new offset*/
	if(Rseek(vf->fd,sizeof(long),SEEK_SET)) return;
	if(Rwritelong(vf->fd,newrec)) return;
	VFaddfree(vf,vf->offrec,sizeof(long)*(3+vf->maxrec*2)+9);
	vf->offrec=newrec;
	/*write new max*/
/* MAXREC */
	vf->maxrec+=vf->VFmaximum;
	if(Rseek(vf->fd,vf->offrec+sizeof(long)+1,SEEK_SET)) return;
	if(Rwritelong(vf->fd,vf->maxrec)) return;
	if(VFgetfreerec(vf)) return;
	if(VFpackfree(vf)) return;
	if(diagvf) { prterr("DIAG finished increasing record max to [%d] in datafile [%s].",vf->maxrec,vf->filename); }
}

static long aVFwrite(VF vf,long recno,BIN *bin,int line,char *file)
{
	long newrec;
	long len;
	long x;
	char c;

	if(diagvf) { prterr("DIAG aVFwrite write record [%d] in variable length file [%s] at line [%d] program [%s].",recno,vf->filename,line,file); }
	len=BINlen(bin)+sizeof(long)+1;
	if(recno==0)
	{
		/*add record*/
		/*increase number of records*/
		if(VFfindrec(vf,(long)0)) return(-1);
		++vf->numrec;
		if(vf->numrec+1>vf->maxrec) VFincrec(vf);
		recno=vf->numrec;
		/*get new record*/
		newrec=VFgetfree(vf,len+8);
		if(newrec<0) return(-1);
		/*write data*/
		if(Rseek(vf->fd,newrec,SEEK_SET)) return(-1);
		c=1;
		if(Rwrite(vf->fd,&c,1)) return(-1);
		if(Rwritelong(vf->fd,recno)) return(-1);
		if(BINwrite(vf->fd,bin)<0)
		{
			prterr("Error aVFwrite can't write offset [%d] in file [%s] at line [%d] program [%s].",newrec,vf->filename,line,file);
			return(-1);
		}
		/*bank*/
		if(VFbank(vf->fd,(long)8)) return(-1);
		/*write new record offset and length*/
		if(Rseek(vf->fd,vf->offrec+sizeof(long)*(recno*2+1)+1,SEEK_SET))
			return(-1);
		if(Rwritelong(vf->fd,newrec)) return(-1);
		if(Rwritelong(vf->fd,len)) return(-1);
		/*write new number of records*/
		if(Rseek(vf->fd,vf->offrec+sizeof(long)*2+1,SEEK_SET)) return(-1);
		if(Rwritelong(vf->fd,vf->numrec)) return(-1);
	} else {
		/*get old record offset and size*/
		if(VFfindrec(vf,recno)) return(-1);
		if(vf->recoff<0)
		{
			/*deleted record*/
			/*get new record*/
			newrec=VFgetfree(vf,len+8);
			if(newrec<0) return(-1);
			/*write data*/
			if(Rseek(vf->fd,newrec,SEEK_SET)) return(-1);
			c=1;
			if(Rwrite(vf->fd,&c,1)) return(-1);
			if(Rwritelong(vf->fd,recno)) return(-1);
			if(BINwrite(vf->fd,bin)<0)
			{
				prterr("Error aVFwrite can't write offset [%d] in file [%s] at line [%d] program [%s].",newrec,vf->filename,line,file);
				return(-1);
			}
			/*bank*/
			if(VFbank(vf->fd,(long)8)) return(-1);
			/*write new record offset and length*/
			if(Rseek(vf->fd,vf->offrec+sizeof(long)*(recno*2+1)+1,SEEK_SET))
				return(-1);
			if(Rwritelong(vf->fd,newrec)) return(-1);
			if(Rwritelong(vf->fd,len)) return(-1);
		} else if(vf->reclen<len)
		{
			/*need to move record*/
			newrec=VFgetfree(vf,len+8);
			if(newrec<0) return(-1);
			/*write data*/
			if(Rseek(vf->fd,newrec,SEEK_SET)) return(-1);
			c=1;
			if(Rwrite(vf->fd,&c,1)) return(-1);
			if(Rwritelong(vf->fd,recno)) return(-1);
			if(BINwrite(vf->fd,bin)<0)
			{
				prterr("Error aVFwrite can't write offset [%d] in datafile [%s] at line [%d] program [%s].",newrec,vf->filename,line,file);
				return(-1);
			}
			/*bank*/
			if(VFbank(vf->fd,(long)8)) return(-1);
			/*write new record offset and length*/
			if(Rseek(vf->fd,vf->offrec+sizeof(long)*(recno*2+1)+1,SEEK_SET))
				return(-1);
			if(Rwritelong(vf->fd,newrec)) return(-1);
			if(Rwritelong(vf->fd,len)) return(-1);
			/*move locks*/
			for(x=0;x<vf->nolock;++x) if(vf->lck[x].recno==recno)
			{
				/*unlock*/
				Rlock(F_SETLKW,F_UNLCK,vf->fd,vf->recoff,vf->reclen);
				/*lock new*/
				Rlock(F_SETLKW,F_WRLCK,vf->fd,newrec,len);
				break;
			}
			/*free old record*/
			VFaddfree(vf,vf->recoff,vf->reclen+8);
		} else {
			/*re-write record in same place*/
			if(Rseek(vf->fd,vf->recoff+sizeof(long)+1,SEEK_SET)) return(-1);
			if(BINwrite(vf->fd,bin)<0)
			{
				prterr("Error aVFwrite can't write offset [%d] in file [%s] at line [%d] program [%s].",vf->recoff,vf->filename,line,file);
				return(-1);
			}
			if(vf->reclen>len)
			{
				/*size not the same*/
				/*write new record length*/
				if(Rseek(vf->fd,vf->offrec+sizeof(long)*(recno*2+2)+1,SEEK_SET))
					return(-1);
				if(Rwritelong(vf->fd,len)) return(-1);
				/*remove excess lock*/
				for(x=0;x<vf->nolock;++x) if(vf->lck[x].recno==recno)
				{
					/*unlock*/
					Rlock(F_SETLKW,F_UNLCK,vf->fd,vf->recoff+len,vf->reclen-len);
					break;
				}
				/*add excess to free space*/
				VFaddfree(vf,vf->recoff+len+8,vf->reclen-len);
			}
		}
	}
	return(recno);/*everything ok*/
}

long xVFwrite(VF vf,long recno,BIN *bin,int line,char *file)
{
	long newrec;

	if(diagvf)
	{
		prterr("DIAG VFwrite Writing Record [%d] to variable length file [%s] at line [%d] program [%s].",recno,vf->filename,line,file);
	}
	VFlock(vf,0,TRUE);/*control*/
	if(recno>0) VFlock(vf,recno,TRUE);
	newrec=aVFwrite(vf,recno,bin,__LINE__,__FILE__);
	if(recno>0) VFunlock(vf,recno);
	VFunlock(vf,0);/*control*/
	return(newrec);
}

void xVFdelrec(VF vf,long recno,int line,char *file)
{
	long x;

	if(diagvf)
	{
		prterr("DIAG Vfdelrec Deleting Record [%d] in Variable File [%s] at line [%d] program [%s].",recno,vf->filename,line,file);
	}
	if(recno<1) return;
	if(VFfindrec(vf,recno)) return;
	if(vf->recoff<0) return;/*already deleted*/
	/*remove locks*/
	for(x=0;x<vf->nolock;++x) if(vf->lck[x].recno==recno)
	{
		if(vf->lck[x].c>0) vf->lck[x].c=1;
		VFunlock(vf,recno);
		break;
	}
	/*write deleted offset*/
	if(Rseek(vf->fd,vf->offrec+sizeof(long)*(recno*2+1)+1,SEEK_SET)) return;
	if(Rwritelong(vf->fd,(long)-1)) return;
	if(Rwritelong(vf->fd,(long)0)) return;
	/*free record*/
	VFaddfree(vf,vf->recoff,vf->reclen+8);
}

void xVFclose(VF vf,int line,char *file)
{
	if(diagvf) { prterr("DIAG VFclose closing file [%s] at line [%d] program [%s].",vf->filename,line,file); }
	if(vf->lck!=NULL) Rfree(vf->lck);
	Rclose(vf->fd);
	if(vf->filename!=NULL) Rfree(vf->filename);
	Rfree(vf);
}

char xVFlock(VF vf,long recno,char block,int line,char *file)
{
	long x;

	if(diagvf) 
	{ 
		prterr("DIAG VFlock locking record number [%d] in file [%s] block [%d] at line [%d] program [%s].",recno,vf->filename,(char *)(block ? "True":"False"),line,file); }
	for(x=0;x<vf->nolock;++x) if(vf->lck[x].recno==recno)
	{
		if(vf->lck[x].c>0)
		{
			++vf->lck[x].c;
			return(FALSE);
		}
		break;
	}
	if(recno>0)
	{
		/*lock record*/
		if(VFfindrec(vf,recno)) return(-1);
		if(vf->recoff<0) return(TRUE);/*record deleted*/
		if(Rlock((block?F_SETLKW:F_SETLK),F_WRLCK,vf->fd,vf->recoff,vf->reclen))
			return(TRUE);
	} else if(recno==0)
	{
		/*lock control*/
		if(Rlock((block?F_SETLKW:F_SETLK),F_WRLCK,vf->fd,(long)sizeof(long),sizeof(long)*3))
			return(TRUE);
	} else {
		/*lock all*/
		if(Rlock((block?F_SETLKW:F_SETLK),F_WRLCK,vf->fd,sizeof(long)*4,(long)0))
			return(TRUE);
	}
	if(x>=vf->nolock)
	{
		x=vf->nolock++;
		MAXALLOC(vf->lck,vf->maxlock,vf->nolock*sizeof(struct VFLCKs));
		vf->lck[x].recno=recno;
	}
	vf->lck[x].c=1;
	return(FALSE);
}

void xVFunlock(VF vf,long recno,int line,char *file)
{
	long x;

	if(diagvf) { prterr("DIAG VFunlock unlocking record number [%d] in file [%s] at line [%d] program [%s].",recno,vf->filename,line,file); }
	for(x=0;x<vf->nolock;++x) if(vf->lck[x].recno==recno) break;
	if(x>=vf->nolock) return;/*record not locked!*/
	if(vf->lck[x].c>1)
	{
		--vf->lck[x].c;
		return;
	}
	if(recno>0)
	{
		/*unlock record*/
		if(VFfindrec(vf,recno)) return;
		if(vf->recoff<0) return;/*record deleted*/
		if(Rlock(F_SETLKW,F_UNLCK,vf->fd,vf->recoff,vf->reclen)) return;
	} else if(recno==0)
	{
		/*lock control*/
		if(Rlock(F_SETLKW,F_UNLCK,vf->fd,(long)sizeof(long),sizeof(long)*3)) return;
	} else {
		/*lock all*/
		if(Rlock(F_SETLKW,F_UNLCK,vf->fd,sizeof(long)*4,(long)0)) return;
	}
	for(++x;x<vf->nolock;++x) vf->lck[x-1]=vf->lck[x];
	--vf->nolock;
}

char xVFrestructure(char *filename,int line,char *file)
{
	char *fname;
	VF vf;
	int fd;
	char *buf;
	int len;
	char *t;
	char r;
	long recno;
	char bank[8];
	int x;
	BIN *bin;
	char c;

	if(diagvf) 
	{ 
		prterr("DIAG VFrestructure restructuring variable length file [%s] at line [%d] program [%s].",filename,line,file); 
	}
	memset(bank,254,8);
	fd=Ropen(filename);
	if(fd<0) return(TRUE);
	fname=Rmalloc(RDAstrlen(USERLOGIN)+11);
	sprintf(fname,"RES.%s%d",USERLOGIN,RGETPID());
	if(VFcreat(fname))
	{
		Rclose(fd);
		Rfree(fname);
		return(TRUE);
	}
	vf=VFopen(fname);
	if(vf==NULL)
	{
		Rclose(fd);
		Rfree(fname);
		return(TRUE);
	}
	/*set first position past control section*/
	r=Rseek(fd,sizeof(long)*4,SEEK_SET);
	if(!r)
	{
		buf=Rmalloc(512);
		if(diagvf) { prterr("DIAG looking for end of bank in datafile [%s].",vf->filename); }
		len=Rvread(fd,buf,512);
		if(len<0)
		{
			r=TRUE;
		} else while(len>0)
		{
			/*find beginning of next record*/
			for(x=0,t=buf;x<len;++x,++t) if(*t!=(char)254) break;
			if(x>=len)
			{
				/*read off end of buffer, read another section*/
				len=Rvread(fd,buf,512);
				if(len<0)
				{
					r=TRUE;
					break;
				}
				continue;
			}
			r=Rseek(fd,x-len,SEEK_CUR);
			if(r) break;
			/*read record type*/
			r=Rread(fd,&c,1);
			if(r) break;
			/*read record number*/
			r=Rreadlong(fd,&recno);
			if(r) break;
			if(diagvf) { prterr("DIAG found record [%d] type [%d].",recno,c); }
			if(c!=1 || recno<1)
			{
				/*old control record, or bad record number, skip*/
				len=Rvread(fd,buf,512);
				if(len<0)
				{
					r=TRUE;
					break;
				}
				while(len>0)
				{
					for(x=0,t=buf;x<len-8;++x,++t) if(*t==(char)254)
						if(!memcmp(t,bank,8)) break;/*found bank*/
					if(x<len-8) break;
					memcpy(buf,buf+len-8,8);
					r=Rseek(fd,-8,SEEK_CUR);
					len=Rvread(fd,buf+8,512-8);
					if(len<0)
					{
						r=TRUE;
						break;
					}
					if(len<=8)
					{
						/*end of file*/
						x=0;
						len=0;
						break;
					}
					len+=8;
				}
				if(r) break;
				/*set pointer to begining of bank*/
				r=Rseek(fd,x-len,SEEK_CUR);
				if(r) break;
			} else {
				/*good record*/
				r=VFfindrec(vf,0);
				if(r) break;
				/*increase size of record offsets if needed*/
				while(recno>=vf->maxrec)
				{
					VFincrec(vf);
				}
				if(recno>vf->numrec)
				{
					r=Rseek(vf->fd,vf->offrec+sizeof(long)*(vf->numrec*2+3)+1,
						SEEK_SET);
					if(r) break;
					/*add deleted records*/
					while(recno>vf->numrec)
					{
						r=Rwritelong(vf->fd,(long)-1);
						if(r) break;
						r=Rwritelong(vf->fd,(long)0);
						if(r) break;
						++vf->numrec;
					}
					/*write new number of records*/
					if(r) break;
					r=Rseek(vf->fd,vf->offrec+sizeof(long)*2+1,SEEK_SET);
					if(r) break;
					r=Rwritelong(vf->fd,vf->numrec);
					if(r) break;
				}
				if(r) break;
				bin=BINread(fd);
				if(VFwrite(vf,recno,bin)<0)
				{
					r=TRUE;
					break;
				}
			}
			len=Rvread(fd,buf,512);
			if(len<0)
			{
				r=TRUE;
				break;
			}
		}
		Rfree(buf);
	}
	VFclose(vf);
	if(!r)
	{
		/*new temporary file has been created, move to old file*/
		unlink(filename);
#ifndef WIN32
		link(fname,filename);
#endif
#ifdef WIN32
		rename(fname,filename);
#endif
	}
	unlink(fname);
	Rclose(fd);
	Rfree(fname);
	return(r);
}

char xVFloop(VF vf,char (*subfunc)(long,BIN *),int line,char *file)
{
	long x;
	BIN *bin;

	if(diagvf) { prterr("DIAG VFloop looping in variable file [%s] at line [%d] program [%s].",vf->filename,line,file); }
	if(VFfindrec(vf,0)) return(TRUE);
	for(x=0;x<vf->numrec;++x)
	{
		bin=VFread(vf,x+1);
		if(bin!=NULL) if(subfunc(x+1,bin)) return(TRUE);
	}
	return(FALSE);
}

void xVFprint(VF vf,int line,char *file)
{
	long y;
	long x;
	long z;

	if(diagvf)
	{
		prterr("DIAG VFprint Dumping Variable Length File [%s] at line [%d] program [%s].",vf->filename,line,file);
	}
	DUMPSTR(vf->filename);DUMPNL();
	if(Rseek(vf->fd,0,SEEK_SET)) return;	
	if(Rreadlong(vf->fd,&x)) return;
	fprintf(RDA_STDERR,"Version=%d\n",x);DUMPNL();
	if(VFfindrec(vf,(long)0)) return;
	DUMPINT(vf->offrec);DUMPINT(vf->maxrec);DUMPINT(vf->numrec);DUMPNL()
/*
	if(Rseek(vf->fd,vf->offrec+sizeof(long)*3+1,SEEK_SET)) return;
	for(y=0;y<vf->numrec;++y)
	{
		if(Rreadlong(vf->fd,&x)) return;
		if(Rreadlong(vf->fd,&z)) return;
		fprintf(RDA_STDERR,"#%d:offset=%d length=%d",y,x,z);DUMPNL();
	}
*/
	if(VFgetfreerec(vf)) return;
	DUMPINT(vf->offfree);DUMPINT(vf->maxfree);DUMPINT(vf->numfree);DUMPNL();
	if(Rseek(vf->fd,vf->offfree+sizeof(long)*3+1,SEEK_SET)) return;
	for(y=0;y<vf->numfree;++y)
	{
		if(Rreadlong(vf->fd,&x)) return;
		if(Rreadlong(vf->fd,&z)) return;
		fprintf(RDA_STDERR,"#%d:offset=%d length=%d",y,x,z);DUMPNL();
	}
}
short xVFreadReUse(BIN *bin,VF vf,long recno,int line,char *file)
{
	long x;

	if(diagvf) { prterr("DIAG VFreadReUse reading record number [%d] in variable length file [%s] at line [%d] program [%s].",recno,vf->filename,line,file); }
	if(recno<1)
	{
		prterr("Error invalid record number [%d] in variable file [%s] in VFread at line [%d] program [%s].",recno,vf->filename,line,file);
		return(-1);
	}
	if(VFfindrec(vf,recno)) return(-1);
	if(vf->recoff<0) return(-1);/*deleted record*/
	/*read actual record*/
	if(Rseek(vf->fd,vf->recoff+1,SEEK_SET)) return(-1);
	if(Rreadlong(vf->fd,&x)) return(-1);
	if(x!=recno)
	{
		prterr("Error record number [%d] corrupted in variable file [%s] in VFread at line [%d] program [%s].",recno,vf->filename,line,file);
		return(-1);
	}
	if(bin==NULL) bin=BINnew();
	BINclear(bin);
	BINreadReUse(bin,vf->fd);
	if(BINlen(bin)+1!=vf->reclen)
	{
		prterr("Error length of binary does not match record in variable file [%s] in VFreadReUse at line [%d] program [%s].",vf->filename,line,file);
		return(-1);
	}
	return(0);
}
