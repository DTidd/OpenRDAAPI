#include <misc.hpp>
#include <rdadta.hpp>

#define MAXALLOC(buf,max,new)   {if(max<new){max=new;buf=Rrealloc(buf,max);}}

#define FLDtype(x,y)	(x)->fdef=(y)

static void DUMPfld(FLD fld)
{
	union ptrs p;
	short x;

	if(fld==NULL)
	{
		prterr("Error received NULL field");
		return;
	}
/*lint -e788 */
	switch(fld->fdef->type)
	{
		case ftchar :
			prterr("\"%.*s\"(%d);",fld->dlen,fld->data,fld->dlen);
			break;
		case ftshort :
			prterr("[");
			for(x=0,p.c=fld->data;x<fld->dlen;++x,++p.s)
				prterr("%d,",*p.s);
			prterr("](%d);",fld->dlen);
			break;
		default :
			prterr("Type not coded for dump");
			break;
	}
	DUMPNL();
/*lint +e788 */
}

static void FLDfree(FLD fld)
{
	if(fld==NULL)
	{
		prterr("Error received NULL field");
		return;
	}
	if(fld->data!=NULL) Rfree(fld->data);
	fld->max=0;
	memset(fld,0,sizeof(struct FLDs));
}

static void FLDcopy(FLD to,FLD from)
{
	if(to==NULL || from==NULL)
	{
		prterr("Error received NULL field");
		return;
	}
	to->fdef=from->fdef;
	to->dlen=from->dlen;
	if(from->data!=NULL)
	{
		MAXALLOC(to->data,to->max,from->max);
		memcpy(to->data,from->data,from->max);
	} else if(to->data!=NULL)
	{
		Rfree(to->data);
		to->max=0;
	}
}

static int charcmp(char *from,char *to,short x)
{
	if(from[x]>to[x]) return(1);
	if(from[x]<to[x]) return(-1);
	return(0);
}

static int shortcmp(short *from,short *to,short x)
{
	if(from[x]>to[x]) return(1);
	if(from[x]<to[x]) return(-1);
	return(0);
}

static int intcmp(int *from,int *to,short x)
{
	if(from[x]>to[x]) return(1);
	if(from[x]<to[x]) return(-1);
	return(0);
}

static int longcmp(long *from,long *to,short x)
{
	if(from[x]>to[x]) return(1);
	if(from[x]<to[x]) return(-1);
	return(0);
}

static int doublecmp(double *from,double *to,short x)
{
	if(from[x]>to[x]) return(1);
	if(from[x]<to[x]) return(-1);
	return(0);
}

static short FLDcmp(FLD from,FLD to)
{
	short x;
	int cmp=0;

	if(to->fdef!=from->fdef)
	{
		prterr("Error trying to compare different types %s to %s",
			from->fdef->name,to->fdef->name);
		return(0);
	}
	switch(from->fdef->type)
	{
		case ftchar :
			for(x=0;x<from->dlen && x<to->dlen;++x)
			{
				cmp=charcmp(from->data,to->data,x);
				if(cmp) return(cmp);
			}
			break;
		case ftshort :
			for(x=0;x<from->dlen && x<to->dlen;++x)
			{
				cmp=shortcmp((short *)from->data,(short *)to->data,x);
				if(cmp) return(cmp);
			}
			break;
		case ftint :
			for(x=0;x<from->dlen && x<to->dlen;++x)
			{
				cmp=intcmp((int *)from->data,(int *)to->data,x);
				if(cmp) return(cmp);
			}
			break;
		case ftlong :
			for(x=0;x<from->dlen && x<to->dlen;++x)
			{
				cmp=longcmp((long *)from->data,(long *)to->data,x);
				if(cmp) return(cmp);
			}
			break;
		case ftdouble :
			for(x=0;x<from->dlen && x<to->dlen;++x)
			{
				cmp=doublecmp((double *)from->data,(double *)to->data,x);
				if(cmp) return(cmp);
			}
			break;
		default:
			prterr("Error unknown field type %d-%s & %s",from->fdef->type,
				from->fdef->name,to->fdef->name);
			return(0);
	}
	if(!cmp && from->dlen!=to->dlen) cmp=(from->dlen>to->dlen?1:-1);
	return(cmp);
}

static void FLDwrite(FLD fld,BIN *bin)
{
	short x;
	union ptrs p;

	switch(fld->fdef->type)
	{
		case ftchar :
			BINadd(bin,fld->data,fld->dlen);
			break;
		case ftshort :
			for(x=0,p.c=fld->data;x<fld->dlen;++x,++p.s) BINaddshort(bin,*p.s);
			break;
		case ftint :
			for(x=0,p.c=fld->data;x<fld->dlen;++x,++p.i) BINaddint(bin,*p.i);
			break;
		case ftlong :
			for(x=0,p.c=fld->data;x<fld->dlen;++x,++p.l) BINaddlongNormal(bin,*p.l);
			break;
		case ftdouble :
			for(x=0,p.c=fld->data;x<fld->dlen;++x,++p.d) BINadddouble(bin,*p.d);
			break;
		default:
			prterr("Error unknown field type %d-%s",fld->fdef->type,
				fld->fdef->name);
			break;
	}
}
static void FLDread(FLD fld,BIN *bin)
{
	short z;
	union ptrs p;

	fld->dlen=fld->fdef->len;
	switch(fld->fdef->type)
	{
		case ftchar :
			if(fld->dlen<1)
			{
				fld->dlen=BINgetstrlen(bin)+1;
				MAXALLOC(fld->data,fld->max,fld->dlen);
				BINget(bin,fld->data,fld->dlen);
			} else {
				MAXALLOC(fld->data,fld->max,fld->dlen);
				BINget(bin,fld->data,fld->dlen);
			}
			break;
		case ftshort :
			if(fld->dlen<1)
			{
				z=0;
				do
				{
					MAXALLOC(fld->data,fld->max,(z+1)*sizeof(short));
					p.c=fld->data;
					p.s+=z;
					*p.s=BINgetshort(bin);
					++z;
				} while(*p.s);
				fld->dlen=z;
			} else {
				MAXALLOC(fld->data,fld->max,fld->dlen*sizeof(short));
				for(z=0,p.c=fld->data;z<fld->dlen;++p.s,++z)
					*p.s=BINgetshort(bin);
			}
			break;
		case ftint :
			if(fld->dlen<1)
			{
				z=0;
				do
				{
					MAXALLOC(fld->data,fld->max,(z+1)*sizeof(int));
					p.c=fld->data;
					p.i+=z;
					*p.i=BINgetint(bin);
					++z;
				} while(*p.i);
				fld->dlen=z;
			} else {
				MAXALLOC(fld->data,fld->max,fld->dlen*sizeof(int));
				for(z=0,p.c=fld->data;z<fld->dlen;++p.i,++z)
					*p.i=BINgetint(bin);
			}
			break;
		case ftlong :
			if(fld->dlen<1)
			{
				z=0;
				do
				{
					MAXALLOC(fld->data,fld->max,(z+1)*sizeof(long));
					p.c=fld->data;
					p.l+=z;
					*p.l=BINgetlongNormal(bin);
					++z;
				} while(*p.l);
				fld->dlen=z;
			} else {
				MAXALLOC(fld->data,fld->max,fld->dlen*sizeof(long));
				for(z=0,p.c=fld->data;z<fld->dlen;++p.l,++z)
					*p.l=BINgetlongNormal(bin);
			}
			break;
		case ftdouble :
			if(fld->dlen<1)
			{
				z=0;
				do
				{
					MAXALLOC(fld->data,fld->max,(z+1)*sizeof(double));
					p.c=fld->data;
					p.d+=z;
					*p.d=BINgetdouble(bin);
					++z;
				} while(*p.d);
				fld->dlen=z;
			} else {
				MAXALLOC(fld->data,fld->max,fld->dlen*sizeof(double));
				for(z=0,p.c=fld->data;z<fld->dlen;++p.d,++z)
					*p.d=BINgetdouble(bin);
			}
			break;
		default:
			prterr("Error unknown field type %d-%s",fld->fdef->type,
				fld->fdef->name);
			break;
	}
}

static void FDEFbinadd(FDEF fdef,BIN *bin)
{
	BINaddstring(bin,fdef->name);
	BINaddint(bin,(int)fdef->type);
	BINaddlongNormal(bin,fdef->len);
}

static void FDEFbinget(FDEF fdef,BIN *bin)
{
	fdef->name=BINgetstring(bin);
/*lint -e64 */
	fdef->type=(ftype)BINgetint(bin);
/*lint +e64 */
	fdef->len=BINgetlongNormal(bin);
}

static void FLDzero(FLD fld)
{
	fld->dlen=fld->fdef->len;
	if(fld->dlen<1) fld->dlen=1;
	switch(fld->fdef->type)
	{
		case ftchar :
			MAXALLOC(fld->data,fld->max,fld->dlen);
			memset(fld->data,0,fld->dlen);
			break;
		case ftshort :
			MAXALLOC(fld->data,fld->max,fld->dlen*sizeof(short));
			memset(fld->data,0,fld->dlen*sizeof(short));
			break;
		case ftint :
			MAXALLOC(fld->data,fld->max,fld->dlen*sizeof(int));
			memset(fld->data,0,fld->dlen*sizeof(int));
			break;
		case ftlong :
			MAXALLOC(fld->data,fld->max,fld->dlen*sizeof(long));
			memset(fld->data,0,fld->dlen*sizeof(long));
			break;
		case ftdouble :
			MAXALLOC(fld->data,fld->max,fld->dlen*sizeof(double));
			memset(fld->data,0,fld->dlen*sizeof(double));
			break;
		default:
			prterr("Error unknown field type %d-%s",fld->fdef->type,
				fld->fdef->name);
			break;
	}
}
short xFLDNAME(DTA dta,char *name,int line,char *file)
{
	short x;

	if(diagrdadta)
	{
		prterr("DIAG FLDNAME Find Field [%s] in data file [%s] at line [%d] program [%s].",name,dta->name,line,file);
	}
	for(x=0;x<dta->nofields;++x) if(!RDAstrcmp(dta->fdef[x].name,name)) return(x);
	prterr("Error field name [%s] not found in file [%s] at line [%d] program [%s].",name,dta->name,line,file);
	return(-1);
}
void *xDATDTA(DTA dta,short no,int line,char *file)
{
	if(diagrdadta)
	{
		prterr("DIAG DATDTA returning pointer data of Field # [%d,%s] in File [%s] at line [%d] program [%s].",no,dta->fld[no].fdef->name,dta->name,line,file);
		DUMPfld(dta->fld+no);
	}
	return(dta->fld[no].data);
}
static void FLDset(FLD fld,void *data)
{
	union ptrs p;

	if(diagrdadta)
	{
		prterr("DIAG FLDset (%s) original value:",fld->fdef->name);
		DUMPfld(fld);
	}
	fld->dlen=fld->fdef->len;
	switch(fld->fdef->type)
	{
		case ftchar :
			if(fld->dlen<1) fld->dlen=RDAstrlen((char *)data)+1;
			MAXALLOC(fld->data,fld->max,fld->dlen);
			memcpy(fld->data,data,fld->dlen);
			break;
		case ftshort :
			if(fld->dlen<1) for(fld->dlen=1,p.c=data;*p.s!=0;++fld->dlen) ++p.s;
			MAXALLOC(fld->data,fld->max,fld->dlen*sizeof(short));
			memcpy(fld->data,data,fld->dlen*sizeof(short));
			break;
		case ftint :
			if(fld->dlen<1) for(fld->dlen=1,p.c=data;*p.i!=0;++fld->dlen) ++p.i;
			MAXALLOC(fld->data,fld->max,fld->dlen*sizeof(int));
			memcpy(fld->data,data,fld->dlen*sizeof(int));
			break;
		case ftlong :
			if(fld->dlen<1) for(fld->dlen=1,p.c=data;*p.l!=0;++fld->dlen) ++p.l;
			MAXALLOC(fld->data,fld->max,fld->dlen*sizeof(long));
			memcpy(fld->data,data,fld->dlen*sizeof(long));
			break;
		case ftdouble :
			if(fld->dlen<1) for(fld->dlen=1,p.c=data;*p.d!=0;++fld->dlen) ++p.d;
			MAXALLOC(fld->data,fld->max,fld->dlen*sizeof(double));
			memcpy(fld->data,data,fld->dlen*sizeof(double));
			break;
		default:
			prterr("Error unknown field type %d-%s",fld->fdef->type,
				fld->fdef->name);
			break;
	}
	if(diagrdadta)
	{
		prterr("DIAG FLDset (%s) new value:",fld->fdef->name);
		DUMPfld(fld);
	}
}

void xSETDTA(DTA dta,short no,void *valx,int line,char *file)
{
	if(diagrdadta)
	{
		prterr("DIAG SETDTA Setting value of Field [%d] in File [%s] at line [%d] program [%s].",no,dta->name,line,file);
	}
	if(valx==NULL) FLDzero(dta->fld+no); else FLDset(dta->fld+no,valx);
}

static char CRTKEY(DTA dta)
{
	short y,z;
	KEY key;
	BIN *bin;
	VF vf;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(dta->keyfilename)+5);
	sprintf(temp,"%s/%s",CURRENTDIRECTORY,dta->keyfilename);
	if(VFcreat(dta->keyfilename)) 
	{
		if(temp!=NULL) Rfree(temp);
		return(TRUE);
	}
	vf=VFopen(temp);
	if(vf==NULL)
	{
		unlink(temp);
		if(temp!=NULL) Rfree(temp);
		return(TRUE);
	}
	bin=BINnew();
	BINaddshort(bin,dta->nokeys);
	for(y=0,key=dta->key;y<dta->nokeys;++y,++key)
	{
		BINaddstring(bin,key->keyname);
		BINaddshort(bin,key->noparts);
		for(z=0;z<key->noparts;++z)
		{
			BINaddshort(bin,key->fieldno[z]);
			BINadd(bin,&key->asc_dec[z],1);
		}
		key->branch=1;BINaddshort(bin,key->branch);/*not currently used*/
	}
	if(VFwrite(vf,(long)0,bin)<0)
	{
		VFclose(vf);
		BINfree(bin);
		unlink(temp);
		if(temp!=NULL) Rfree(temp);
		return(TRUE);
	}
	if(bin!=NULL) BINfree(bin);
	for(y=0;y<dta->nokeys;++y)
	{
		bin=BINnew();
		BINaddlongNormal(bin,(long)0);
		BINaddlongNormal(bin,(long)0);
		if(VFwrite(vf,(long)0,bin)<0)
		{
			VFclose(vf);
			BINfree(bin);
			unlink(temp);
			if(temp!=NULL) Rfree(temp);
			return(TRUE);
		}
		if(bin!=NULL) BINfree(bin);
	}
	if(temp!=NULL) Rfree(temp);
	VFclose(vf);
	return(FALSE);
}

char xCRTDTA(DTA dta,int line,char *file)
{
	short y;
	BIN *bin;
	VF vf;
	char *temp=NULL;

	if(diagrdadta)
	{
		prterr("DIAG CRTDTA Creating Data File [%s] at line [%d] program [%s].",dta->name,line,file);
	}
	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(dta->filename)+5);
	sprintf(temp,"%s/%s",CURRENTDIRECTORY,dta->filename);
	if(VFcreat(temp)) 
	{
		if(temp!=NULL) Rfree(temp);
		return(TRUE);
	}
	vf=VFopen(temp);
	if(vf==NULL)
	{
		unlink(temp);
		if(temp!=NULL) Rfree(temp);
		return(TRUE);
	}
	bin=BINnew();
	BINaddstring(bin,dta->name);
	BINaddshort(bin,dta->nofields);
	BINaddstring(bin,dta->keyfilename);
	for(y=0;y<dta->nofields;++y) FDEFbinadd(dta->fdef+y,bin);
	if(VFwrite(vf,(long)0,bin)<0)
	{
		VFclose(vf);
		unlink(temp);
		BINfree(bin);
		if(temp!=NULL) Rfree(temp);
		return(TRUE);
	}
	BINfree(bin);
	VFclose(vf);
	if(CRTKEY(dta))
	{
		unlink(temp);
		if(temp!=NULL) Rfree(temp);
		return(TRUE);
	}
	/*free items allocated by create functinos*/
	CLSDTA(dta);
	if(temp!=NULL) Rfree(temp);
	return(FALSE);
}

DTA xNEWDTA(char *name,char *filename,char *keyfilename,int line,char *file)
{
	DTA dta;

	if(diagrdadta)
	{
		prterr("DIAG NEWDTA Creating RDA Database Structure [%s] Filename [%s] Keyfilename [%s] at line [%d] program [%s].",name,filename,keyfilename,line,file); 
	}
	dta=Rmalloc(sizeof(struct DTAs));
	if(dta==NULL) return(NULL);
	memset(dta,0,sizeof(struct DTAs));
	dta->name=stralloc(name);
	dta->filename=stralloc(filename);
	dta->keyfilename=stralloc(keyfilename);
	return(dta);
}

short xNEWFLD(DTA dta,char *fieldname,ftype type,long length,int line,char *file)
{
	short x;
	FDEF fdef;

	if(diagrdadta)
	{
		prterr("DIAG NEWFLD Adding New Field [%s] Type [%d] Length [%d] to Datafile [%s] at line [%d] program [%s].",fieldname,type,length,dta->name,line,file);
	}
	if(dta==NULL) return(-1);
	x=dta->nofields++;
	dta->fdef=Rrealloc(dta->fdef,sizeof(struct FDEFs)*dta->nofields);
	fdef=dta->fdef+x;
	memset(fdef,0,sizeof(struct FDEFs));
	fdef->name=stralloc(fieldname);
	fdef->type=type;
	fdef->len=length;
	return(x);
}

short xNEWKEY(DTA dta,char *keyname,short branch,int line,char *file)
{
	short x;
	KEY key;

	if(diagrdadta)
	{
		prterr("DIAG NEWKEY Adding New Key [%s] Branch Factor [%d] to Datafile [%s] at line [%d] program [%s].",keyname,branch,dta->name,line,file);
	}
	if(dta==NULL) return(-1);
	x=dta->nokeys++;
	dta->key=Rrealloc(dta->key,sizeof(struct KEYs)*dta->nokeys);
	key=dta->key+x;
	memset(key,0,sizeof(struct KEYs));
	key->keyno=x;
	key->keyname=stralloc(keyname);
	key->branch=branch;/*not used*/
	key->noparts=0;
	return(x);
}

short xNEWELE(KEY key,short field_no,char asc_dec,int line,char *file)
{
	short x;

	if(diagrdadta)
	{
		prterr("DIAG NEWELE Adding Key Element Field Number [%d] to Key [%s] at line [%d] program [%s].",field_no,key->keyname,line,file);
	}
	x=key->noparts++;
	key->fieldno=Rrealloc(key->fieldno,sizeof(short)*key->noparts);
	key->fieldno[x]=field_no;
	key->asc_dec=Rrealloc(key->asc_dec,key->noparts);
	key->asc_dec[x]=asc_dec;
	return(x);
}

static void DUMPkey(KEY key)
{
	short x,y;

	DUMPINT(key->keyno);
	DUMPSTR(key->keyname);
	DUMPLONG(key->firstkey);
	DUMPNL();
	DUMPINT(key->noparts);
	for(x=0;x<key->noparts;++x)
	{
		DUMPINT(x);
		DUMPINT(key->fieldno[x]);
	}
	DUMPNL();
	DUMPLONG(key->currec);
	DUMPINT(key->parent);
	DUMPNL();
	DUMPLONG(key->childless);
	DUMPLONG(key->childgreat);
	DUMPLONG(key->dtarecs);
	DUMPNL();
	for(y=0;y<key->noparts;++y) DUMPfld((key->fld)+y);
	DUMPNL();
}

static short CMPKEY(KEY key,FLD dtafld,FLD keyfld)
{
	short x;
	short cmp=0;

	for(x=0;x<key->noparts && !cmp;++x,++keyfld)
	{
		cmp=FLDcmp(dtafld+key->fieldno[x],keyfld);
		if(key->asc_dec[x]) cmp*=(-1);/*invert for reverse order*/
	}
	return(cmp);
}

static char WFLKEY(KEY key)
{
	long x;
	BIN *bin;

	bin=BINnew();
	BINaddlongNormal(bin,key->firstkey);
	if(key->ver) BINaddlongNormal(bin,key->no_nodes);
	x=VFwrite(key->dta->keyvf,key->keyno+2,bin);
	if(x<0)
	{
		prterr("Error can't write first key for %d in %s",key->keyno,key->dta->keyfilename);
		if(bin!=NULL) BINfree(bin);
		return(TRUE);
	}
	if(bin!=NULL) BINfree(bin);
	return(FALSE);
}

static char RFLKEY(KEY key)
{
	BIN *bin;

	if(key==NULL)
	{
		prterr("Error received NULL key pointer");
		return(TRUE);
	}
	bin=VFread(key->dta->keyvf,key->keyno+2);
	if(bin==NULL)
	{
		prterr("Error can't read first key for %d in %s",key->keyno,key->dta->keyfilename);
		return(TRUE);
	}
	key->firstkey=BINgetlongNormal(bin);
	if(BINlen(bin)>sizeof(long)+sizeof(short))
	{
		key->no_nodes=BINgetlongNormal(bin);
		key->ver=1;
	}
	BINfree(bin);
	return(FALSE);
}

static char READKEY(KEY key,long recno)
{
	BIN *bin;
	short y,z;

	key->currec=0;
	if(recno==0)/*read first key*/
	{
		key->level=0;
		key->partial=0;
		key->last_part=0;
		if(RFLKEY(key)) return(TRUE);
		if(key->firstkey==0) return(FALSE);/*no key's!*/
		recno=key->firstkey;
	}
	bin=VFread(key->dta->keyvf,recno);
	if(bin==NULL)
	{
		prterr("Error can't read key record %d in %s",key->keyno,key->dta->keyfilename);
		return(TRUE);
	}
	key->currec=recno;
	key->parent=BINgetlongNormal(bin);
	key->no_bch=BINgetshort(bin);/*not used*/
	key->childless=BINgetlongNormal(bin);
	key->dtarecs=BINgetlongNormal(bin);
	for(y=0;y<key->noparts;++y) FLDread(key->fld+y,bin);
	key->childgreat=BINgetlongNormal(bin);
	key->last_part=key->partial;
	z=0;
	if(key->childless) ++z;
	if(key->childgreat) ++z;
	if(z==1) key->partial=1; else key->partial=0;
	BINfree(bin);
	return(FALSE);
}

static long WRITEKEY(KEY key,long recno)
{
	BIN *bin;
	short y;

	bin=BINnew();
	BINaddlongNormal(bin,key->parent);
	BINaddshort(bin,key->no_bch);/*not used*/
	BINaddlongNormal(bin,key->childless);
	BINaddlongNormal(bin,key->dtarecs);
	for(y=0;y<key->noparts;++y) FLDwrite(key->fld+y,bin);
	BINaddlongNormal(bin,key->childgreat);
	recno=VFwrite(key->dta->keyvf,recno,bin);
	BINfree(bin);
	return(recno);
}

static void SETKEY(KEY key,FLD fld)
{
	short y;

	for(y=0;y<key->noparts;++y,++fld)
		FLDcopy(fld,key->dta->fld+key->fieldno[y]);
}

static char EQLKEY(KEY key)
{
	long nextrec;
	short cmp;

	if(READKEY(key,(long)0)) return(TRUE);
	if(key->firstkey==0) return(TRUE);/*no key's*/
	do
	{
		++key->level;
		cmp=CMPKEY(key,key->dta->fld,key->fld);
		if(!cmp)
		{
			if(key->dtarecs<=0) return(TRUE);/*record is deleted!*/
			/*record not deleted and found.*/
			key->dta->cur=key->dtarecs;
			return(FALSE);
		}
		nextrec=(cmp<0?key->childless:key->childgreat);
		if(nextrec<=0) break;
		if(READKEY(key,nextrec)) return(TRUE);
	} while(TRUE);
	return(TRUE);
}

static char UPKEY(KEY key)
{
	long lastrec;

	do
	{
		if(key->childgreat>0)
		{
			if(READKEY(key,key->childgreat)) return(TRUE);
			while(key->childless>0)
				if(READKEY(key,key->childless)) return(TRUE);
		} else {
			do
			{
				if(key->parent<=0) return(TRUE);/*no parent!*/
				lastrec=key->currec;
				if(READKEY(key,key->parent)) return(TRUE);
				if(lastrec==key->childless) break;
			} while(TRUE);
		}
		if(key->dtarecs>0) break;
		/*key deleted, do upkey again*/
	} while(TRUE);
	key->dta->cur=key->dtarecs;
	return(FALSE);
}

static char DWKEY(KEY key)
{
	long lastrec;

	do
	{
		if(key->childless>0)
		{
			if(READKEY(key,key->childless)) return(TRUE);
			while(key->childgreat>0)
				if(READKEY(key,key->childgreat)) return(TRUE);
		} else {
			do
			{
				if(key->parent<=0) return(TRUE);/*no parent!*/
				lastrec=key->currec;
				if(READKEY(key,key->parent)) return(TRUE);
				if(lastrec==key->childgreat) break;
			} while(TRUE);
		}
		if(key->dtarecs>0) break;
		/*key deleted, do dwkey again*/
	} while(TRUE);
	key->dta->cur=key->dtarecs;
	return(FALSE);
}

static char FRSKEY(KEY key)
{
	if(READKEY(key,(long)0)) return(TRUE);
	if(key->firstkey==0) return(TRUE);/*no key's*/
	do
	{
		++key->level;
		if(key->childless<=0)
		{
			/*no more less children*/
			if(key->dtarecs<=0)
			{
				/*first key deleted, do upkey*/
				return(UPKEY(key));
			}
			/*return first key*/
			key->dta->cur=key->dtarecs;
			return(FALSE);
		}
		if(READKEY(key,key->childless)) return(TRUE);
	} while(TRUE);
}

static char LSTKEY(KEY key)
{
	if(READKEY(key,(long)0)) return(TRUE);
	if(key->firstkey==0) return(TRUE);/*no key's*/
	do
	{
		++key->level;
		if(key->childgreat<=0)
		{
			/*no more great children*/
			if(key->dtarecs<=0)
			{
				/*last key deleted, do dwkey*/
				return(DWKEY(key));
			}
			/*return last key*/
			key->dta->cur=key->dtarecs;
			return(FALSE);
		}
		if(READKEY(key,key->childgreat)) return(TRUE);
	} while(TRUE);
}

static char NXTKEY(KEY key)
{
	short cmp;

	if(READKEY(key,(long)0)) return(TRUE);
	if(key->firstkey==0) return(TRUE);/*no key's*/
	do
	{
		++key->level;
		cmp=CMPKEY(key,key->dta->fld,key->fld);
		if(!cmp) return(UPKEY(key));
		if(cmp<0)
		{
			if(key->childless<=0)
			{
				if(key->dtarecs<=0) return(UPKEY(key));
				key->dta->cur=key->dtarecs;
				return(FALSE);
			}
			if(READKEY(key,key->childless)) return(TRUE);
		} else {
			if(key->childgreat<=0) return(UPKEY(key));
			if(READKEY(key,key->childgreat)) return(TRUE);
		}
	} while(TRUE);
}

static char PRVKEY(KEY key)
{
	short cmp;

	if(READKEY(key,(long)0)) return(TRUE);
	if(key->firstkey==0) return(TRUE);/*no key's*/
	do
	{
		++key->level;
		cmp=CMPKEY(key,key->dta->fld,key->fld);
		if(!cmp) return(DWKEY(key));
		if(cmp<0)
		{
			if(key->childless<=0) return(DWKEY(key));
			if(READKEY(key,key->childless)) return(TRUE);
		} else {
			if(key->childgreat<=0)
			{
				if(key->dtarecs<=0) return(DWKEY(key));
				key->dta->cur=key->dtarecs;
				return(FALSE);
			}
			if(READKEY(key,key->childgreat)) return(TRUE);
		}
	} while(TRUE);
}

static char GTEKEY(KEY key)
{
	short cmp;

	if(READKEY(key,(long)0)) return(-1);
	if(key->firstkey==0) return(0);/*no key's*/
	do
	{
		++key->level;
		cmp=CMPKEY(key,key->dta->fld,key->fld);
		if(!cmp)
		{
			key->dta->cur=key->dtarecs;
			return(FALSE);/*found equal!*/
		}
		if(cmp<0)
		{
			if(key->childless<=0)
			{
				if(key->dtarecs<=0) return(UPKEY(key));
				key->dta->cur=key->dtarecs;
				return(FALSE);
			}
			if(READKEY(key,key->childless)) return(TRUE);
		} else {
			if(key->childgreat<=0) return(UPKEY(key));
			if(READKEY(key,key->childgreat)) return(TRUE);
		}
	} while(TRUE);
}

static char LTEKEY(KEY key)
{
	short cmp;

	if(READKEY(key,(long)0)) return(-1);
	if(key->firstkey==0) return(0);/*no key's*/
	do
	{
		++key->level;
		cmp=CMPKEY(key,key->dta->fld,key->fld);
		if(!cmp)
		{
			key->dta->cur=key->dtarecs;
			return(FALSE);/*found equal!*/
		}
		if(cmp<0)
		{
			if(key->childless<=0) return(DWKEY(key));
			if(READKEY(key,key->childless)) return(TRUE);
		} else {
			if(key->childgreat<=0)
			{
				if(key->dtarecs<=0) return(DWKEY(key));
				key->dta->cur=key->dtarecs;
				return(FALSE);
			}
			if(READKEY(key,key->childgreat)) return(TRUE);
		}
	} while(TRUE);
}

static void ZEROKEY(KEY key)
{
	short w;

	key->parent=0;
	key->no_bch=0;/*not used*/
	key->childless=0;
	key->childgreat=0;
	key->dtarecs=0;
	for(w=0;w<key->noparts;++w) FLDzero((key->fld)+w);
}

static char ADDKEY(KEY key)
{
	long pkey;
	long ppkey;
	long pppkey;
	long ckey;
	short cmp;
	short pcmp;

	if(key->firstkey==0)
	{
		/*no key records for key*/
		/*zero record*/
		ZEROKEY(key);
		key->dtarecs=key->dta->cur;
		SETKEY(key,key->fld);/*set key value for 1st position*/
		key->firstkey=WRITEKEY(key,key->firstkey);
		key->no_nodes=1;
		if(key->firstkey<0) return(TRUE);
		return(WFLKEY(key));
	}
	/*find where to add key from current*/
	cmp=CMPKEY(key,key->dta->fld,key->fld);
	if(!cmp)
	{
		if(key->dtarecs>0)
		{
			prterr("Error key already on file for key %d in %s",key->keyno,key->dta->filename);
			return(TRUE);
		}
		/*record was deleted, undelete*/
		key->dtarecs=key->dta->cur;
		WRITEKEY(key,key->currec);
		return(FALSE);
	}
	if(cmp>0)
	{
		if(key->childgreat!=0)
		{
			prterr("Error, found non-zero child");
			return(TRUE);/*not possible!*/
		}
	} else {
		if(key->childless!=0)
		{
			prterr("Error, found non-zero child");
			return(TRUE);/*not possible!*/
		}
	}
	if(key->last_part && !key->partial)
	{
		/*re-balance parent-parent node*/
		pkey=key->currec;
		ppkey=key->parent;
		if(READKEY(key,ppkey)) return(TRUE);
		pppkey=key->parent;
		pcmp=(pkey==key->childless?(-1):1);
		if(pcmp==cmp)
		{
			/*straight line, make pkey papa*/
			/*write new record*/
			/*zero record*/
			ZEROKEY(key);
			key->parent=pkey;
			key->dtarecs=key->dta->cur;
			SETKEY(key,key->fld);
			ckey=WRITEKEY(key,(long)0);
			if(ckey<0) return(TRUE);
			/*move pkey*/
			if(READKEY(key,pkey)) return(TRUE);
			key->parent=pppkey;
			if(cmp<0)
			{
				key->childless=ckey;
				key->childgreat=ppkey;
			} else {
				key->childless=ppkey;
				key->childgreat=ckey;
			}
			if(WRITEKEY(key,pkey)<0) return(TRUE);
			/*move ppkey*/
			if(READKEY(key,ppkey)) return(TRUE);
			key->parent=pkey;
			key->childless=0;
			key->childgreat=0;
			if(WRITEKEY(key,ppkey)<0) return(TRUE);
			/*fix pppkey*/
			if(pppkey)
			{
				if(READKEY(key,pppkey)) return(TRUE);
				if(ppkey==key->childless)
				{
					key->childless=pkey;
				} else {
					key->childgreat=pkey;
				}
				if(WRITEKEY(key,pppkey)<0) return(TRUE);
			} else {
				key->firstkey=pkey;
			}
		} else {
			/*curved line, make ckey papa*/
			/*write new record*/
			/*zero record*/
			ZEROKEY(key);
			key->parent=pppkey;
			key->dtarecs=key->dta->cur;
			SETKEY(key,key->fld);
			if(pcmp<0)
			{
				key->childless=pkey;
				key->childgreat=ppkey;
			} else {
				key->childless=ppkey;
				key->childgreat=pkey;
			}
			ckey=WRITEKEY(key,(long)0);
			if(ckey<0) return(TRUE);
			/*move ppkey*/
			if(READKEY(key,ppkey)) return(TRUE);
			key->parent=ckey;
			key->childless=0;
			key->childgreat=0;
			if(WRITEKEY(key,ppkey)<0) return(TRUE);
			/*move pkey*/
			if(READKEY(key,pkey)) return(TRUE);
			key->parent=ckey;
			key->childless=0;
			key->childgreat=0;
			if(WRITEKEY(key,pkey)<0) return(TRUE);
			/*fix pppkey*/
			if(pppkey)
			{
				if(READKEY(key,pppkey)) return(TRUE);
				if(ppkey==key->childless)
				{
					key->childless=ckey;
				} else {
					key->childgreat=ckey;
				}
				if(WRITEKEY(key,pppkey)<0) return(TRUE);
			} else {
				key->firstkey=ckey;
			}
		}
	} else {
		/*rebalance not required, add at current position*/
		pkey=key->currec;
		/*zero record*/
		ZEROKEY(key);
		key->parent=pkey;
		key->dtarecs=key->dta->cur;
		SETKEY(key,key->fld);
		ckey=WRITEKEY(key,(long)0);
		if(ckey<0) return(TRUE);
		if(READKEY(key,pkey)) return(TRUE);
		if(cmp>0)
		{
			key->childgreat=ckey;
		} else {
			key->childless=ckey;
		}
		if(WRITEKEY(key,pkey)<0) return(TRUE);
	}
	++key->no_nodes;
	return(WFLKEY(key));
}

static void DELKEY(KEY key)
{
	if(!EQLKEY(key))
	{
		key->dtarecs=(-1);
		WRITEKEY(key,key->currec);
	}
	return;
}

void xDUMPdta(DTA dta,int line,char *file)
{
	short x;

	if(diagrdadta)
	{
		prterr("DIAG DUMPdta Dumping Datafile [%s] at line [%d] program [%s].",dta->name,line,file);
	}
	DUMPSTR(dta->name);
	DUMPSTR(dta->filename);
	DUMPINT(dta->nofields);
	DUMPINT(dta->nokeys);
	DUMPSTR(dta->keyfilename);
	DUMPNL();
	for(x=0;x<dta->nokeys;++x) DUMPkey(dta->key+x);
	VFprint(dta->vf);
	VFprint(dta->keyvf);
}

DTA xOPNDTA(char *filename,int line,char *file)
{
	DTA dta;
	short y,z,w;
	KEY key;
	BIN *bin;
	char *temp=NULL,*temp1=NULL;

	if(diagrdadta)
	{
		prterr("DIAG OPNDTA Opening RDA Database file [%s] at line [%d] program [%s].",filename,line,file);
	}
	dta=Rmalloc(sizeof(struct DTAs));
	if(dta==NULL) return(NULL);
	memset(dta,0,sizeof(struct DTAs));
	temp1=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(filename)+5);
	sprintf(temp1,"%s/%s",CURRENTDIRECTORY,filename);
	dta->vf=VFopen(temp1);
	if(dta->vf==NULL)
	{
		if(temp1!=NULL) Rfree(temp1);
		Rfree(dta);
		return(NULL);
	}
	bin=VFread(dta->vf,(long)1);
	if(temp1!=NULL) Rfree(temp1);
	if(bin==NULL)
	{
		prterr("Error [%s] in reading resource for [%s].",errname(),filename);
		CLSDTA(dta);
		return(NULL);
	}
	dta->filename=stralloc(filename);
	dta->name=BINgetstring(bin);
	dta->nofields=BINgetshort(bin);
	dta->keyfilename=BINgetstring(bin);
#ifndef WIN32
	temp=fixunixname(dta->keyfilename);
#endif
#ifdef WIN32
	temp=fixdosname(dta->keyfilename);
#endif
	if(dta->keyfilename!=NULL) Rfree(dta->keyfilename);
	dta->keyfilename=stralloc(temp);
	if(temp!=NULL) Rfree(temp);
	dta->fdef=Rmalloc(sizeof(struct FDEFs)*dta->nofields);
	memset(dta->fdef,0,sizeof(struct FDEFs)*dta->nofields);
	dta->fld=Rmalloc(sizeof(struct FLDs)*dta->nofields);
	memset(dta->fld,0,sizeof(struct FLDs)*dta->nofields);
	for(y=0;y<dta->nofields;++y)
	{
		FDEFbinget(dta->fdef+y,bin);
		FLDtype(dta->fld+y,dta->fdef+y);
		FLDzero(dta->fld+y);
	}
	BINfree(bin);
	temp1=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(dta->keyfilename)+5);
	sprintf(temp1,"%s/%s",CURRENTDIRECTORY,dta->keyfilename);
	dta->keyvf=VFopen(temp1);
	if(temp1!=NULL) Rfree(temp1);
	if(dta->keyvf==NULL)
	{
		CLSDTA(dta);
		return(NULL);
	}
	bin=VFread(dta->keyvf,(long)1);
	if(bin==NULL)
	{
		prterr("Error in reading resource for (%s)%s",dta->keyfilename,
			errname());
		CLSDTA(dta);
		return(NULL);
	}
	dta->nokeys=BINgetshort(bin);
	dta->key=Rmalloc(dta->nokeys*sizeof(struct KEYs));
	memset(dta->key,0,dta->nokeys*sizeof(struct KEYs));
	for(y=0,key=dta->key;y<dta->nokeys;++y,++key)
	{
		key->keyno=y;
		key->dta=dta;
		key->keyname=BINgetstring(bin);
		key->noparts=BINgetshort(bin);
		key->fieldno=Rmalloc(sizeof(short)*key->noparts);
		key->asc_dec=Rmalloc(key->noparts);
		for(z=0;z<key->noparts;++z)
		{
			key->fieldno[z]=BINgetshort(bin);
			BINget(bin,&key->asc_dec[z],1);
		}
		key->branch=BINgetshort(bin);/*not used*/
		key->fld=Rmalloc(sizeof(struct FLDs)*key->noparts);
		memset(key->fld,0,sizeof(struct FLDs)*key->noparts);
		for(w=0;w<key->noparts;++w)
			FLDcopy((key->fld)+w,dta->fld+key->fieldno[w]);
	}
	BINfree(bin);
	if(diagrdadta)
	{
		prterr("DIAG OPNDTA:");
		DUMPdta(dta);
	}
	return(dta);
}

long xSIZDTA(DTA dta,int line,char *file)
{
	if(diagrdadta)
	{
		prterr("DIAG SIZDTA Find Number Records in Datafile [%s] at line [%d] program [%s].",dta->name,line,file);
	}
	if(RFLKEY(dta->key)) return(-1);
	return(dta->key->no_nodes);
}

KEY xKEYBYNAME(DTA dta,char *keyname,int line,char *file)
{
	short x;
	KEY key;

	if(diagrdadta)
	{
		prterr("DIAG KEYBYNAME Find Key [%s] in Datafile [%s] at line [%d] program [%s].",keyname,dta->name,line,file);
	}
	for(x=0,key=dta->key;x<dta->nokeys;++x,++key)
		if(!RDAstrcmp(key->keyname,keyname)) return(key);
	prterr("Error key name [%s] not found in datafile [%s] at line [%d] program [%s].",keyname,dta->name,line,file);
	return(NULL);
}

static char SAVEDTA(DTA dta,long recno)/*move to data buffer*/
{
	short y;
	BIN *bin;

	bin=BINnew();
	for(y=0;y<dta->nofields;++y) FLDwrite(dta->fld+y,bin);
	recno=VFwrite(dta->vf,recno,bin);
	if(recno<0)
	{
		BINfree(bin);
		return(FALSE);
	}
	BINfree(bin);
	dta->cur=recno;
	return(FALSE);
}

static void LOADDTA(DTA dta,BIN *bin,long recno)
{
	short y;
	FLD fld;

	for(y=0,fld=dta->fld;y<dta->nofields;++y,++fld) FLDread(fld,bin);
	/*set current record number*/
	dta->cur=recno;
}

char xREADDTA(DTA dta,long recno,int line,char *file)
{
	BIN *bin;

	if(diagrdadta)
	{
		prterr("DIAG READDTA Reading Record Number [%d] in Datafile [%s] at line [%d] program [%s].",recno,dta->name,line,file);
	}
	if(recno<0)
	{
		prterr("Error trying to read record number [%d] from datafile [%s] at line [%d] program [%s].",recno,dta->name,line,file);
		return(FALSE);/*deleted key's.*/
	}
	bin=VFread(dta->vf,recno);
	if(bin==NULL) return(TRUE);
	LOADDTA(dta,bin,recno);
	BINfree(bin);
	return(FALSE);
}

char xFRSDTA(KEY key,int line,char *file)
{
	if(diagrdadta)
	{
		prterr("DIAG FRSDTA Find First Record in Datafile [%s] using Key [%s] at line [%d] program [%s].",key->dta->name,key->keyname,line,file);
	}
	if(FRSKEY(key)) return(TRUE);/*no records*/
	if(key->dta->cur<=0) return(TRUE);/*deleted record*/
	return(READDTA(key->dta,key->dta->cur));
}

char xLSTDTA(KEY key,int line,char *file)
{
	if(diagrdadta)
	{
		prterr("DIAG LSTDTA Find Last Record in Datafile [%s] using Key [%s] at line [%d] program [%s].",key->dta->name,key->keyname,line,file);
	}
	if(LSTKEY(key)) return(TRUE);/*no records*/
	if(key->dta->cur<=0) return(TRUE);/*deleted record*/
	return(READDTA(key->dta,key->dta->cur));
}

char xNXTDTA(KEY key,int line,char *file)
{
	if(diagrdadta)
	{
		prterr("DIAG NXTDTA Find Next Record in Datafile [%s] using Key [%s] at line [%d] program [%s].",key->dta->name,key->keyname,line,file);
	}
	if(NXTKEY(key)) return(TRUE);
	if(key->dta->cur<=0) return(TRUE);/*deleted record*/
	return(READDTA(key->dta,key->dta->cur));
}

char xPRVDTA(KEY key,int line,char *file)
{
	if(diagrdadta)
	{
		prterr("DIAG NXTDTA Find Previous Record in Datafile [%s] using Key [%s] at line [%d] program [%s].",key->dta->name,key->keyname,line,file);
	}
	if(PRVKEY(key)) return(TRUE);
	if(key->dta->cur<=0) return(TRUE);/*deleted record*/
	return(READDTA(key->dta,key->dta->cur));
}

char xEQLDTA(KEY key,int line,char *file)
{
	if(diagrdadta)
	{
		prterr("DIAG EQLDTA Find Equal Record in Datafile [%s] using Key [%s] at line [%d] program [%s].",key->dta->name,key->keyname,line,file);
	}
	if(EQLKEY(key)) return(TRUE);
	if(key->dta->cur<=0) return(TRUE);/*deleted record*/
	return(READDTA(key->dta,key->dta->cur));
}

char xWRTDTA(DTA dta,int line,char *file)
{
	short y,z=0;

	if(diagrdadta)
	{
		prterr("DIAG WRTDTA Write Record in Datafile [%s] at line [%d] program [%s].",dta->name,line,file);
	}
	LOCDTA(dta,0,TRUE);
	for(y=0,z=0;y<dta->nokeys;++y) if(!EQLKEY(dta->key+y)) ++z;
	if(z>0 && z<dta->nokeys)
	{
		prterr("Error trying to add duplicate key in datafile [%s] using WRTDTA at line [%d] program [%s]",dta->name,line,file);
		UNLDTA(dta,0);
		return(TRUE);
	}
	if(z)
	{
		if(SAVEDTA(dta,dta->cur))
		{
			UNLDTA(dta,0);
			return(TRUE);
		}
	} else {
		/*add to data file*/
		if(SAVEDTA(dta,(long)0))
		{
			UNLDTA(dta,0);
			return(TRUE);
		}
		for(y=0;y<dta->nokeys;++y) if(ADDKEY(dta->key+y))
		{
			prterr("Error adding record [%d] to key [%d] in datafile [%s] using WRTDTA at line [%d] program [%s].",dta->cur,y,dta->name,line,file);
		}
	}
	UNLDTA(dta,0);
	return(FALSE);
}

char xGTEDTA(KEY key,int line,char *file)
{
	if(diagrdadta)
	{
		prterr("DIAG GTEDTA Find Record Greater Than or Equal To in Datafile [%s] using Key [%s] at line [%d] program [%s].",key->dta->name,key->keyname,line,file);
	}
	if(GTEKEY(key)) return(TRUE);
	if(key->dta->cur<=0) return(TRUE);/*deleted record*/
	return(READDTA(key->dta,key->dta->cur));
}

char xLTEDTA(KEY key,int line,char *file)
{
	if(diagrdadta)
	{
		prterr("DIAG LTEDTA Find Record Less Than or Equal To in Datafile [%s] using Key [%s] at line [%d] program [%s].",key->dta->name,key->keyname,line,file);
	}
	if(LTEKEY(key)) return(TRUE);
	if(key->dta->cur<=0) return(TRUE);/*deleted record*/
	return(READDTA(key->dta,key->dta->cur));
}

void xDTAFLDZERO(DTA dta,short no,int line,char *file)
{
	if(diagrdadta)
	{
		prterr("DIAG DTAFLDZERO Zeroing Field [%d] in Datafile [%s] at line [%d] program [%s].",no,dta->name,line,file);
	}
	FLDzero(dta->fld+no);
}

void xCLSDTA(DTA dta,int line,char *file)
{
	short x;
	short z;
	KEY key;

	if(diagrdadta)
	{
		prterr("DIAG CLSDTA Close Datafile [%s] at line [%d] program [%s].",dta->name,line,file);
	}
	if(dta->vf!=NULL) VFclose(dta->vf);
	if(dta->keyvf!=NULL) VFclose(dta->keyvf);
	if(dta->key!=NULL)
	{
		for(x=0,key=dta->key;x<dta->nokeys;++x,++key)
		{
			if(key->keyname) Rfree(key->keyname);
			if(key->fieldno) Rfree(key->fieldno);
			if(key->asc_dec) Rfree(key->asc_dec);
			if(key->fld!=NULL)
			{
				for(z=0;z<key->noparts;++z) FLDfree((key->fld)+z);
/* Raddog Added This Rfree */
				Rfree(key->fld);
			}
		}
		Rfree(dta->key);
	}
	for(x=0;x<dta->nofields;++x)
	{
		if(dta->fdef!=NULL) if(dta->fdef[x].name!=NULL)
			Rfree(dta->fdef[x].name);
		if(dta->fld!=NULL) FLDfree(dta->fld+x);
	}
	if(dta->fld) Rfree(dta->fld);
/* Raddog Added This Rfree */
	if(dta->fdef!=NULL) Rfree(dta->fdef);
	if(dta->keyfilename!=NULL) Rfree(dta->keyfilename);
	if(dta->filename!=NULL) Rfree(dta->filename);
	if(dta->name!=NULL) Rfree(dta->name);
	Rfree(dta);
}

char xLOCDTA(DTA dta,long recno,char block,int line,char *file)
{
	if(diagrdadta)
	{
		prterr("DIAG LOCDTA Locking Database Record [%d] in Datafile [%s] Block [%s] at line [%d] program [%s].",recno,dta->name,(char *)(block ? "True":"False"),line,file);
	}
	return(VFlock(dta->vf,recno,block));
}

void xUNLDTA(DTA dta,long recno,int line,char *file)
{
	if(diagrdadta)
	{
		prterr("DIAG UNLDTA Unlocking Database Record [%d] in Datafile [%s] at line [%d] program [%s].",recno,dta->name,line,file);
	}
	VFunlock(dta->vf,recno);
}

void xDELDTA(DTA dta,long recno,int line,char *file)
{
	short y;

	if(diagrdadta)
	{
		prterr("DIAG DELDTA Deleting record no [%d] in Datafile [%s] at line [%d] program [%s].",recno,dta->name,line,file);
	}
	if(recno<1) return;
	/*find data record*/
	if(READDTA(dta,recno)) return;
	/*remove data record*/
	VFdelrec(dta->vf,recno);
	for(y=0;y<dta->nokeys;++y) DELKEY(dta->key+y);
}

static char RKYleaf(KEY key,int fd,long left,long right,long parent,short pset)
{
	long y;
	long x;
	long recno;

	if(right<left) return(FALSE);
	y=(right+left)/2;
	ZEROKEY(key);
	key->parent=parent;
	if(Rseek(fd,sizeof(long)*y,SEEK_SET)) return(TRUE);
	if(Rread(fd,&x,sizeof(long))) return(TRUE);
	if(READDTA(key->dta,x)) return(TRUE);
	key->dtarecs=key->dta->cur;
	SETKEY(key,key->fld);/*set key value for 1st position*/
	recno=WRITEKEY(key,(long)0);
	if(recno<0) return(TRUE);
	if(parent)
	{
		/*add as child to parent*/
		if(READKEY(key,parent)) return(TRUE);
		if(pset) key->childgreat=recno; else key->childless=recno;
		if(WRITEKEY(key,parent)<0) return(TRUE);
		++key->no_nodes;
	} else {
		key->no_nodes=1;
	}
	if(RKYleaf(key,fd,left,y-1,recno,0)) return(TRUE);
	if(RKYleaf(key,fd,y+1,right,recno,1)) return(TRUE);
	if(!parent)
	{
		/*is first key record*/
		key->firstkey=recno;
		if(WFLKEY(key)) return(TRUE);
	}
	return(FALSE);
}

static long RKYread(KEY key,int fd)
{
	long count=0;

	if(FRSKEY(key)) return(-1);
	do
	{
		if(Rwrite(fd,&key->dtarecs,sizeof(long))) return(-1);
		++count;
	} while(!UPKEY(key));
	return(count);
}

static char RKYdel(KEY key,long recno)
{
	long less,great;

	if(READKEY(key,recno)) return(TRUE);
	less=key->childless;
	great=key->childgreat;
	if(less) if(RKYdel(key,less))
	{
		prterr("Error could not delete key, key file is not corupted.");
		return(TRUE);
	}
	if(great) if(RKYdel(key,great))
	{
		prterr("Error could not delete key, key file is not corupted.");
		return(TRUE);
	}
	if(recno==0)
	{
		if(READKEY(key,recno)) return(TRUE);
		recno=key->currec;
		key->firstkey=0;
		key->no_nodes=0;
		if(WFLKEY(key)) return(TRUE);
	}
	VFdelrec(key->dta->keyvf,recno);
	return(FALSE);
}

static char *RTempFile(int *fd)
{
	char *fname;

	fname=Rmalloc(RDAstrlen(USERLOGIN)+11);
	sprintf(fname,"RKY.%s%d",USERLOGIN,RGETPID());
	*fd=Rcreat(fname);
	if(*fd>=0) Rclose(*fd);
	*fd=Ropen(fname);
	if(*fd>=0) return(fname);
	Rfree(fname);
	return(NULL);
}

char xRKYDTA(KEY key,int line,char *file)
{
	long count;
	char *fname;
	int fd;
	char r;

	if(diagrdadta)
	{
		prterr("DIAG RKYDTA Rebuild Node using Key [%s] in Datafile [%s] at line [%d] program [%s].",key->keyname,key->dta->name,line,file);
	}
	LOCDTA(key->dta,(long)0,TRUE);
	if(LOCDTA(key->dta,(long)-1,FALSE))
	{
		UNLDTA(key->dta,(long)0);
		return(TRUE);/*can't lock whole file*/
	}
	r=RFLKEY(key);
	if(!r || key->firstkey!=0)
	{
		fname=RTempFile(&fd);
		if(fd>=0)
		{
			count=RKYread(key,fd);
			if(count>=0)
			{
				/*delete all key records*/
				r=RKYdel(key,(long)0);
				if(!r) r=RKYleaf(key,fd,(long)0,count-1,(long)0,0);
			} else r=TRUE;
			Rclose(fd);
			unlink(fname);
			Rfree(fname);
		} else r=TRUE;
	}
	UNLDTA(key->dta,(long)-1);
	UNLDTA(key->dta,(long)0);
	return(r);
}

char xRBDDTA(DTA dta,int line,char *file)
{
	long *count;
	char *fname;
	int fd;
	char r=false;
	short y;
	long x;
	char *temp=NULL;

	if(diagrdadta)
	{
		prterr("DIAG RBDDTA Rebuild Datafile [%s] at line [%d] program [%s].",dta->name,line,file);
	}
	LOCDTA(dta,(long)0,TRUE);
	if(LOCDTA(dta,(long)-1,FALSE))
	{
		UNLDTA(dta,(long)0);
		return(TRUE);/*can't lock whole file*/
	}
	count=Rmalloc(sizeof(long)*dta->nokeys);
	memset(count,0,sizeof(long)*dta->nokeys);
	fname=RTempFile(&fd);
	if(fd>=0)
	{
		for(y=0;y<dta->nokeys && !r;++y)
		{
			r=RFLKEY(dta->key+y);
			if(!r || dta->key[y].firstkey!=0)
			{
				count[y]=RKYread(dta->key+y,fd);
				if(count[y]<0) r=TRUE;
			}
		}
		if(!r)
		{
			VFclose(dta->keyvf);
			unlink(dta->keyfilename);
			r=CRTKEY(dta);
			if(!r)
			{
				temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(dta->keyfilename)+5);
				sprintf(temp,"%s/%s",CURRENTDIRECTORY,dta->keyfilename);
				dta->keyvf=VFopen(temp);
				if(temp!=NULL) Rfree(temp);
				if(dta->keyvf!=NULL)
				{
					for(y=0,x=0;y<dta->nokeys && !r;x+=count[y++])
						r=RKYleaf(dta->key+y,fd,x,x+count[y]-1,(long)0,0);
				} else r=TRUE;
			}
		}
		close(fd);
		unlink(fname);
		Rfree(fname);
	} else r=TRUE;
	Rfree(count);
	UNLDTA(dta,(long)-1);
	UNLDTA(dta,(long)0);
	return(r);
}

static DTA resdta;

static char RESloop(long recno,BIN *bin)
{
	short y;

	if(recno==1) return(FALSE);
	LOADDTA(resdta,bin,recno);
	for(y=0;y<resdta->nokeys;++y)
	{
		/*find where to add*/
		EQLKEY(resdta->key+y);
		if(ADDKEY(resdta->key+y))
		{
			prterr("Error adding record %d to key %d in %s",resdta->cur,y,resdta->name);
			return(TRUE);
		}
	}
	return(FALSE);
}

char xRESDTA(char *filename,int line,char *file)
{
	char *temp=NULL;

	if(diagrdadta)
	{
		prterr("DIAG RESDTA Restructure Datafile [%s] at line [%d] program [%s].",filename,line,file);
	}
	if(VFrestructure(filename)) return(TRUE);
	resdta=OPNDTA(filename);
	if(resdta==NULL) return(TRUE);
	VFclose(resdta->keyvf);
	unlink(resdta->keyfilename);
	CRTKEY(resdta);
	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(resdta->keyfilename)+5);
	sprintf(temp,"%s/%s",CURRENTDIRECTORY,resdta->keyfilename);
	resdta->keyvf=VFopen(temp);
	if(temp!=NULL) Rfree(temp);
	if(resdta->keyvf==NULL)
	{
		CLSDTA(resdta);
		return(TRUE);
	}
	/*write all keys*/
	if(VFloop(resdta->vf,RESloop))
	{
		CLSDTA(resdta);
		return(TRUE);
	}
	CLSDTA(resdta);
	return(FALSE);
}

#define RBDFACTOR z*z*4

void xRCKDTA(DTA dta,int line,char *file)
{
	short y,x,z;
	long t;
	KEY key;

	if(diagrdadta)
	{
		prterr("DIAG RCKDTA Datafile [%s] at line [%d] program [%s].",dta->name,line,file);
	}
	for(y=0,x=0,key=dta->key;y<dta->nokeys;++y,++key)
	{
		/*figure power of two*/
		for(t=key->no_nodes,z=1;t;++z) t/=2;
		if(key->level>RBDFACTOR) ++x;/*need to rebuild*/
	}
	if(!x) return;
	RBDDTA(dta);
}

char xBEGDTA(DTA dta,int line,char *file)
{
	long rec;

	if(diagrdadta)
	{
		prterr("DIAG BEGDTA Find First Physical Record in Datafile [%s] at line [%d] program [%s].",dta->name,line,file); 
	}
	/*get number of records*/
	READDTA(dta,(long)2);
	for(rec=2;rec<=dta->vf->numrec;++rec)
	{
		if(!READDTA(dta,rec)) return(FALSE);
	}
	/*no records, or errors*/
	return(TRUE);
}

char xSEQDTA(DTA dta,int line,char *file)
{
	long rec;
	
	if(diagrdadta)
	{
		prterr("DIAG SEQDTA Find Next Physical Record in Datafile [%s] at line [%d] program [%s].",dta->name,line,file); 
	}
	rec=dta->cur;
	for(rec=dta->cur+1;rec<=dta->vf->numrec;++rec)
	{
		if(!READDTA(dta,rec)) return(FALSE);
	}
	/*no more records, or errors*/
	return(TRUE);
}
