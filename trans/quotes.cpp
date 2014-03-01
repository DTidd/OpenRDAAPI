/* quotes.c - Quotation Mark functions */
#include<transp.hpp>

void sub_quotes(char **holdvarx)
{
	char *temp1=NULL,*temp2=NULL;
	int z=0,x=0,y=0,len=0;

	if(holdvarx==NULL) return;
	temp2=*holdvarx;
	if(temp2==NULL) return;
	len=RDAstrlen(temp2);
	if(len<1) return;
	temp1=strchr(temp2,'\"');

	/* skip the quote if it is the first character */
	if(temp1==temp2) 
	{
		temp1=strchr(temp2+1,'\"');
	}

	/* check for case of no valid imbedded quotes found */
	if((temp1==NULL) || ((int)(temp1-temp2))==(len-1)) return; 

	temp2[0]=016;
	temp2[len-1]=016;
	x=0;
	while(temp1!=NULL)
	{
		temp1=strchr((temp2+((int)(temp1-temp2)+1)),'\"');
		++x;
	}
	temp1=Rmalloc(len+x+1);
	for(z=0;z<len;++z)
	{
		if(temp2[z]!='\"')
		{
			temp1[y]=temp2[z];
			++y;
		} else {
			temp1[y]='\\';
			++y;
			temp1[y]=temp2[z];
			++y;
		}
	}
	temp1[len+x]='\0';
	for(x=0;temp1[x]!=0;++x) 
	{
		if(temp1[x]==016) temp1[x]='\"';
	}
	if(temp1[x-2]=='\\') temp1[x-2]=016;
	if(temp2!=NULL) Rfree(temp2);
	(*holdvarx)=temp1;
}
char *reset_quotes(char *string)
{
	char *temp1=NULL,*temp2=NULL;
	int x,y,offset=0;

	if(!isEMPTY(string))
	{
	temp1=stralloc(string);
	temp2=RDAstrstr(temp1,"\\\"");
	if(!isEMPTY(temp2))
	{
		y=((int)(temp2-temp1));
		if(y>(RDAstrlen(string)-2)) return(temp1); 
		while(TRUE)
		{
			if(temp2==NULL) break;
			offset=y=((int)(temp2-temp1));
			x=RDAstrlen(temp1);
			if(x>0 && y>=0 && y<=x)
			{
				while(y<x)
				{
					temp1[y]=temp1[y+1];
					y++;
				}
			}
			temp2=RDAstrstr(temp1+(offset+1),"\\\"");
		}
		temp2=strchr(temp1,016);
		if(!isEMPTY(temp2))
		{
			x=((int)(temp2-temp1));
			if(x>RDAstrlen(temp1)) return(temp1); 
			for(y=x;temp1[y]!=0;++y)
			{
				if(temp1[y]==016) temp1[y]='\\';
			}
		}
		return(temp1);
	} else {
		temp2=strchr(temp1,016);
		if(!isEMPTY(temp2))
		{
			x=((int)(temp2-temp1));
			if(x>RDAstrlen(temp1)) return(temp1); 
			for(y=x;temp1[y]!=0;++y)
			{
				if(temp1[y]==016) temp1[y]='\\';
			}
		}
		return(temp1);
	}
	} else {
		return(NULL);
	}
}
