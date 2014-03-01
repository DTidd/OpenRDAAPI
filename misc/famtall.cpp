/* famtall.c-all famt related functions */
#include <misc.hpp>

/* GUIfamt- format regular dollar string */
char *GUIfamt(double amount,int len)
{
	char *tmp=NULL,*tmp1=NULL,*temp=NULL;
	char negflag=FALSE;
	
	tmp=famt(amount,len);
	if(tmp==NULL) return(NULL);
	unpad(tmp);
	tmp1=Rmalloc(RDAstrlen(tmp)+2);
	temp=RDAstrstr(tmp,"-");
	if(temp!=NULL)
	{
		negflag=TRUE;
		*temp=0;
	}
	sprintf(tmp1,"%c%s",(negflag ? '-':' '),tmp);
	if(tmp!=NULL) Rfree(tmp);
	return(tmp1);
}
char *GUIfamtncents(double amount,int len,char padchar)
{
	char *tmp=NULL,*tmp1=NULL,*temp=NULL;
	char negflag=FALSE;
	
	tmp=famtncents(amount,len,padchar);
	if(tmp==NULL) return(NULL);
	unpad(tmp);
	tmp1=Rmalloc(RDAstrlen(tmp)+2);
	temp=RDAstrstr(tmp,"-");
	if(temp!=NULL)
	{
		negflag=TRUE;
		*temp=0;
	}
	sprintf(tmp1,"%c%s",(negflag ? '-':' '),tmp);
	if(tmp!=NULL) Rfree(tmp);
	return(tmp1);
}
char *GUIuintamt(long amount,int len)
{
	char *tmp=NULL,*tmp1=NULL,*temp=NULL;
	char negflag=FALSE;
	
	tmp=uintamt(amount,len);
	if(tmp==NULL) return(NULL);
	unpad(tmp);
	tmp1=Rmalloc(RDAstrlen(tmp)+2);
	temp=RDAstrstr(tmp,"-");
	if(temp!=NULL)
	{
		negflag=TRUE;
		*temp=0;
	}
	sprintf(tmp1,"%c%s",(negflag ? '-':' '),tmp);
	if(tmp!=NULL) Rfree(tmp);
	return(tmp1);
}
char *GUIfloatamt(double amount,int len)
{
	char *tmp=NULL,*tmp1=NULL,*temp=NULL;
	char negflag=FALSE;
	
	tmp=floatamt(amount,len);
	if(tmp==NULL) return(NULL);
	unpad(tmp);
	tmp1=Rmalloc(RDAstrlen(tmp)+2);
	temp=RDAstrstr(tmp,"-");
	if(temp!=NULL)
	{
		negflag=TRUE;
		*temp=0;
	}
	sprintf(tmp1,"%c%s",(negflag ? '-':' '),tmp);
	if(tmp!=NULL) Rfree(tmp);
	return(tmp1);
}
/* famt- format regular dollar string */
char *famt(double amount,int len)
{
	
	int x,y;
	int length;
	int neg_flag=FALSE;
	char *ret_string=NULL, *val_string=NULL;
/*
	char currtype='$';
*/

	if(amount<0)
	{
		neg_flag=TRUE;
		amount *= (-1);
	}
	val_string=(char *)Rmalloc(2*len+1);
	for(x=0;x<len;x++) val_string[x]=' ';
	sprintf(stemp,"%.02f",amount/100.0);
	ret_string=stralloc(stemp);
	length=RDAstrlen(ret_string);
	if(length>len)
	{
		ret_string[len]=0;
		length=RDAstrlen(ret_string);
	}
	val_string[len]=0;
	for(x=0,y=length-1;y>-1;)
	{
		if(((x-2)%4==0)&&(x!=2))
		{
			if(x==(len-2)) break;
			val_string[x++]=',';
		}
		else 
		{
			val_string[x++]=ret_string[y--];
		}
	}
	length=RDAstrlen(val_string);
	if(ret_string!=NULL) Rfree(ret_string);
	ret_string=(char *)Rmalloc(len+1);
	memset(ret_string,0,sizeof(ret_string));
	for(x=0,y=len-2;(x<(length-1)&&y>-1);)
    	{
		if(y<0)
		{
			prterr("Error field ran over it's length in famt.");
    			break;
   		}
		ret_string[y--]=val_string[x++];
	}
	for(;y>0;--y) ret_string[y]=' ';
	ret_string[len-1]=(neg_flag?'-':' ');
/*
	ret_string[0]=currtype;
*/
	ret_string[0]=RDA_CURRENCY_SYMBOL;
	for(x=0;x<len;++x) val_string[x]=ret_string[x];
	val_string[len]=0;
	if(ret_string!=NULL) Rfree(ret_string);
	return(val_string);
}
/* uintamt - format integer with negitive ability */
char *uintamt(long amount,int len)
{
	int x;
	int neg_flag=FALSE;
	char *val_string=NULL;

	if(amount<0)
	{
		neg_flag=TRUE;
		amount*=(-1);
	}
	memset(stemp,0,101);
	sprintf(stemp,"%*d%c",len-1,amount,(neg_flag==TRUE?'-':' '));
	val_string=(char *)Rmalloc(2*len+1);
	for(x=0;x<len-1;x++) val_string[x]=stemp[x];
	val_string[len-1]=(neg_flag?'-':' ');
	val_string[len]=0;
	return(val_string);
}
/* floatamt- format float with negitive ability */
char *floatamt(double amount,int len)
{
	int x,y,length;
	int neg_flag=FALSE;
	char *ret_string=NULL,*val_string=NULL,padchar=' ';

	if(amount<0)
	{
		neg_flag=TRUE;
		amount*=(-1);
	}
	sprintf(stemp,"%.09f",amount);
	ret_string=stralloc(stemp);
	length=RDAstrlen(ret_string);
	val_string=(char *)Rmalloc(2*len+2);
	for(x=length-1;x>(-1);x--)
	{
		if(ret_string[x]=='0') 
		{
			ret_string[x]=0;
		} else if (ret_string[x]=='.') 
		{
			ret_string[x]=0;
			break;
		} else break;
	}
	length=RDAstrlen(ret_string);
	y=(len-length);
	x=0;
	if(y>(-1))
	{
		for(x=0;x<y;x++)
		{
			val_string[x]=padchar;
		}
	} 
	if(y<0) y=0;
	for(x=0;y<(len);x++)
	{
		val_string[y++]=ret_string[x];
	}
	if(ret_string!=NULL) Rfree(ret_string);
	val_string[len]=(neg_flag?'-':' ');
	val_string[len+1]=0;
	return(val_string);
}
char *famtncomma(double amount,int len,char padchar)
{
	char *tmp=NULL;
	char neg_amt=FALSE;
	int x;

	if(amount<0) 
	{
		amount*=(-1);
		neg_amt=TRUE;
	}
	sprintf(stemp,"%*.02f",len-1,(amount/100.0));
	for(x=0;(!isdigit(stemp[x]) && stemp[x]!='.');x++)
	{
		if(stemp[x]==' ') stemp[x]=padchar;
	}
	stemp[len-1]=(neg_amt ? '-':' ');
	tmp=stralloc(stemp);
	tmp[len]=0;
	return(tmp);
}
char *famtncnc(double amount,int len,char padchar)
{
	char *tmp=NULL;
	char neg_amt=FALSE;
	int x=0;

	if(amount<0) 
	{
		amount*=(-1);
		neg_amt=TRUE;
	}
	sprintf(stemp,"$%*.0f%c",len-2,RDAroundup(amount/100.0),(neg_amt ? '-':' '));
	for(x=len-2;x>0;x--) if(stemp[x]==' ') stemp[x]=padchar;
	tmp=stralloc(stemp);
	tmp[len]=0;
	return(tmp);
}
char *famtncents(double amount,int len,char padchar)
{
	
	int x,y;
	int length;
	int neg_flag=FALSE;
	char *ret_string, *val_string;
/*
	char currtype='$';
*/

	if(amount<0)
	{
		neg_flag=TRUE;
		amount*=(-1);
	}
	val_string=(char *)Rmalloc(2*len+1);
	for(x=0;x<len;x++) val_string[x]=padchar;
	sprintf(stemp,"%.0f",round(amount/100.0));
	ret_string=stralloc(stemp);
	length=RDAstrlen(ret_string);
	if(length>len)
	{
		ret_string[len]=0;
		length=RDAstrlen(ret_string);
	}
	val_string[len]=0;
	for(x=0,y=length-1;(y>-1&&x<len);)
	{
		if(((x+1)%4==0))
		{
			if(x==len-2) break;
			val_string[x++]=',';
		} else {
			val_string[x++]=ret_string[y--];
		}
	}
	length=RDAstrlen(val_string);
	if(ret_string!=NULL) Rfree(ret_string);
	ret_string=(char *)Rmalloc(len+1);
	memset(ret_string,0,sizeof(ret_string));
	for(x=0,y=len-2;(x<(length-1)&&y>-1);)
        {
		if(y<0)
		{
			prterr("Error field ran over it's length");
                       	break;
               	}
		ret_string[y--]=val_string[x++];
	}
        for(;y>0;--y) ret_string[y]=padchar;
	ret_string[len-1]=(neg_flag?'-':' ');
/*
	ret_string[0]=currtype;
*/
	ret_string[0]=RDA_CURRENCY_SYMBOL;
	for(x=0;x<len;++x) val_string[x]=ret_string[x];
	val_string[len]=0;
	if(ret_string!=NULL) Rfree(ret_string);
	return(val_string);
}
char *famtpad(double amount,int len,char pad)
{
	
	int x,y;
	int length;
	int neg_flag=FALSE;
	char *ret_string, *val_string;
/*
	char currtype='$';
*/

	if(amount<0)
	{
		neg_flag=TRUE;
		amount*=(-1);
	}
	val_string=(char *)Rmalloc(2*len+1);
	for(x=0;x<len;x++) val_string[x]=pad;
	sprintf(stemp,"%.02f",amount/100.0);
	ret_string=stralloc(stemp);
	length=RDAstrlen(ret_string);
	if(length>len)
	{
		ret_string[len]=0;
		length=RDAstrlen(ret_string);
	}
	val_string[len]=0;
	for(x=0,y=length-1;y>-1;)
	{
		if(((x-2)%4==0)&&(x!=2))
		{
			if(x==len-2) break;
			val_string[x++]=',';
		} else val_string[x++]=ret_string[y--];
	}
	length=RDAstrlen(val_string);
	if(ret_string!=NULL) Rfree(ret_string);
	ret_string=(char *)Rmalloc(len+1);
	memset(ret_string,0,sizeof(ret_string));
	for(x=0,y=len-2;x<(length-1);)
        {
		if(y<0)
		{
			prterr("Error field ran over it's length");
                       	break;
               	}
		ret_string[y--]=val_string[x++];
	}
        for(;y>0;--y) ret_string[y]=pad;
	ret_string[len-1]=(neg_flag?'-':' ');
/*
	ret_string[0]=currtype;
*/
	ret_string[0]=RDA_CURRENCY_SYMBOL;
	for(x=0;x<len;++x) val_string[x]=ret_string[x];
	val_string[len]=0;
	if(ret_string!=NULL) Rfree(ret_string);
	return(val_string);
}
/* famtnbk Print Dollar amounts with negatives as brackets */
char *famtnbk(double amount,int len,char padchar)
{
	char *string1=NULL,*string2=NULL;
	double amt1=0;

	if(amount<0) amt1=(-1)*amount;
		else amt1=amount;
	string1=famtpad(amt1,len-1,padchar);
	string2=(char *)Rmalloc(RDAstrlen(string1)+2);
	if(amount<0)
	{
		sprintf(string2,"(%.*s)",len-2,string1);
	} else {
		sprintf(string2," %*s",len-1,string1);
	}
	if(string1!=NULL) Rfree(string1);
	return(string2);
}
char *famtnbknc(double amount,int len,char padchar)
{
	int x,y;
	int length;
	int neg_flag=FALSE;
	char *ret_string, *val_string;
/*
	char currtype='$';
*/

	if(amount<0)
	{
		neg_flag=TRUE;
		amount*=(-1);
	}
	val_string=(char *)Rmalloc(2*len+1);
	for(x=0;x<len;x++) val_string[x]=padchar;
	sprintf(stemp,"%.0f",RDAroundup(amount/100.0));
	ret_string=stralloc(stemp);
	length=RDAstrlen(ret_string);
	if(length>len)
	{
		ret_string[len]=0;
		length=RDAstrlen(ret_string);
	}
	val_string[len]=0;
	for(x=0,y=length-1;y>-1;)
	{
		if((x+1)%4==0)
		{
			if(neg_flag&&x==len-3) break;
			if(x==len-2) break;
			val_string[x++]=',';
		} else val_string[x++]=ret_string[y--];
	}
	if(ret_string!=NULL) Rfree(ret_string);
	ret_string=(char *)Rmalloc(len+1);
	memset(ret_string,0,sizeof(ret_string));
	length=RDAstrlen(val_string);
	for(x=0,y=(len-2);(x<(length-1)&&y>-1);)
	{
		if(y<0)
		{
			prterr("Error field ran over it's length");
			break;
		}
		ret_string[y--]=val_string[x++];
	}
	for(;y>0;--y) ret_string[y]=padchar;
	if(neg_flag==TRUE)
	{
		ret_string[0]='(';
		ret_string[len-1]=')';
		ret_string[1]=RDA_CURRENCY_SYMBOL;
	} else { 
		ret_string[0]=' ';
		ret_string[1]=RDA_CURRENCY_SYMBOL;
	}
	for(x=0;x<len;++x) val_string[x]=ret_string[x];
	val_string[len]=0;
	if(ret_string!=NULL) Rfree(ret_string);
	return(val_string);
}
char *famtnbkncomma(double amount,int len,char padchar)
{
	char *tmp=NULL;
	char neg_amt=FALSE;
	int x;

	if(amount<0) 
	{
		amount*=(-1);
		neg_amt=TRUE;
	}
	if(neg_amt) sprintf(stemp,"($%*.02f)",len-3,(amount/100.0));
		else sprintf(stemp," $%*.02f",len-2,(amount/100.0));
	for(x=len-3;x>0;x--) if(stemp[x]==' ') stemp[x]=padchar;
	tmp=stralloc(stemp);
	tmp[len]=0;
	return(tmp);
}
char *famtnbkncnc(double amount,int len,char padchar)
{
	char *tmp=NULL;
	char neg_amt=FALSE;
	int x;

	if(amount<0) 
	{
		amount*=(-1);
		neg_amt=TRUE;
	}
	if(neg_amt) sprintf(stemp,"($%*.0f)",len-3,RDAroundup(amount/100.0));
		else sprintf(stemp," $%*.0f",len-2,RDAroundup(amount/100.0));
	for(x=len-3;x>0;x--) if(stemp[x]==' ') stemp[x]=padchar;
	tmp=stralloc(stemp);
	tmp[len]=0;
	return(tmp);
}
/* ufloatamt- format unsigned float */
char *ufloatamt(double amount,int len)
{
	int x,y,length;
	char *ret_string, *val_string, padchar=' ';

	sprintf(stemp,"%f",amount);
	ret_string=stralloc(stemp);
	length=RDAstrlen(ret_string);
	val_string=(char *)Rmalloc(2*len+1);
	for(x=length-1;x>-1;x--)
	{
		if(ret_string[x]=='0') 
		{
			ret_string[x]=0;
		} else if (ret_string[x]=='.') 
		{
			ret_string[x]=0;
			break;
		} else break;
	}
	length=RDAstrlen(ret_string);
	y=(len-length);
	x=0;
	if(y>(-1))
	{
		for(x=0;x<y;++x)
		{
			val_string[x]=padchar;
		}
	}
	if(y<0) y=0;
	for(x=y,y=0;x<len;++x,++y)
	{
		val_string[x]=ret_string[y];
	}
	if(ret_string!=NULL) Rfree(ret_string);
	val_string[len]=0;
	return(val_string);
}
char *famt_inttoencrypted1(double amount,int len,char pad)
{
	
	int x,y;
	int length;
	int neg_flag=FALSE;
	char *ret_string, *val_string;

	if(amount<0)
	{
		neg_flag=TRUE;
		amount=amount*(-1);
	} else {
		neg_flag=FALSE;
	}
	val_string=(char *)Rmalloc(2*len+1);
	for(x=0;x<len;x++) val_string[x]=pad;
	sprintf(stemp,"%.00f",amount);
	ret_string=stralloc(stemp);
	length=RDAstrlen(ret_string);
	if(length>len)
	{
		ret_string[len]=0;
		length=RDAstrlen(ret_string);
	}
	val_string[len]=0;
	for(x=0,y=length-1;y>-1;)
	{
		val_string[x++]=ret_string[y--];
	}
	length=RDAstrlen(val_string);
	if(ret_string!=NULL) Rfree(ret_string);
	ret_string=(char *)Rmalloc(len+1);
	memset(ret_string,0,sizeof(ret_string));
	for(x=1,y=len-2;x<(length-1);)
        {
		if(y<0)
		{
			prterr("Error field ran over it's length");
                       	break;
               	}
		ret_string[y--]=val_string[x++];
	}
        for(;y>=0;--y) ret_string[y]=pad;
	switch(val_string[0])
	{
		case '0':
				if(neg_flag==FALSE)
				{
					ret_string[len-1]='{';
				} else {
					ret_string[len-1]='}';
				}
				break;
		case '1':
				if(neg_flag==FALSE)
				{
					ret_string[len-1]='A';
				} else {
					ret_string[len-1]='J';
				}
				break;
		case '2':
				if(neg_flag==FALSE)
				{
					ret_string[len-1]='B';
				} else {
					ret_string[len-1]='K';
				}
				break;
		case '3':
				if(neg_flag==FALSE)
				{
					ret_string[len-1]='C';
				} else {
					ret_string[len-1]='L';
				}
				break;
		case '4':
				if(neg_flag==FALSE)
				{
					ret_string[len-1]='D';
				} else {
					ret_string[len-1]='M';
				}
				break;
		case '5':
				if(neg_flag==FALSE)
				{
					ret_string[len-1]='E';
				} else {
					ret_string[len-1]='N';
				}
				break;
		case '6':
				if(neg_flag==FALSE)
				{
					ret_string[len-1]='F';
				} else {
					ret_string[len-1]='O';
				}
				break;
		case '7':
				if(neg_flag==FALSE)
				{
					ret_string[len-1]='G';
				} else {
					ret_string[len-1]='P';
				}
				break;
		case '8':
				if(neg_flag==FALSE)
				{
					ret_string[len-1]='H';
				} else {
					ret_string[len-1]='Q';
				}
				break;
		case '9':
				if(neg_flag==FALSE)
				{
					ret_string[len-1]='I';
				} else {
					ret_string[len-1]='R';
				}
				break;
		default:
				prterr("Error unrecongnized value.");
				ret_string[len-1]='{';
				break;
	}
	for(x=0;x<len;++x)
	{
		val_string[x]=ret_string[x];
	}
	val_string[len]=0;
	if(ret_string!=NULL) Rfree(ret_string);
	return(val_string);
}
