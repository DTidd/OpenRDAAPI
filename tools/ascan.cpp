#ifndef WIN32
#define __NAM__ "ascan.lnx"
#endif
#ifdef WIN32
#define __NAM__ "ascan.exe"
#endif
#include <app.h>
#include <string.h>
#include <miscfunc.h>
/*ADDLIB misc */

#define MAX	300
#define FMX	(MAX-5)

char ascan[MAX][200];
short count[MAX];

void main()
{
	int no=0;
	int x,y;
	char addr[20];
	char *temp;

	while(gets(ascan[no])!=NULL)
	{
		temp=strchr(ascan[no],' ');
		if(temp!=NULL)
		{
			count[no]=1;
			*temp=0;
			if(RDAstrlen(ascan[no])<sizeof(addr))
			{
				strcpy(addr,ascan[no]);
				*temp=' ';
				if(no==0)
				{
					++no;
				} else {
					for(x=0;x<no;++x)
					{
						temp=strchr(ascan[x],' ');
						if(temp!=NULL)
						{
							*temp=0;
							if(!RDAstrcmp(ascan[x],addr)) break;
							*temp=' ';
						}
						++count[x];
					}
					if(x<no)
					{
						for(;x<no;++x)
						{
							 strcpy(ascan[x],ascan[x+1]);
							 count[x]=count[x+1]+1;
						}
						--no;
					} else {
						if(strstr(ascan[no],"FREE")!=NULL)
						{
							fprintf(stdout,"%s\n",ascan[no]);
						} else {
							++no;
							if(no>FMX) count[0]=FMX+1;
						}
					}
				}
				for(x=0,y=0;x<no;++x) if(count[x]>FMX)
				{
					fprintf(stdout,"%s\n",ascan[x]);
				} else {
					if(x!=y)
					{
						strcpy(ascan[y],ascan[x]);
						count[y]=count[x];
					}
					++y;
				}
				no-=(x-y);
				fflush(stdout);
			} else fprintf(stdout,"%s\n",ascan[no]);
		} else fprintf(stdout,"%s\n",ascan[no]);
	}
	for(x=0;x<no;++x) fprintf(stdout,"%s\n",ascan[x]);
}
