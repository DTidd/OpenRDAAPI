#include <stdarg.h>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstddef>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PIPEBLOCKSIZE 512

int main(int argc,char **argv)
{
	FILE *fp=NULL;
	char inbuf[PIPEBLOCKSIZE];

	fprintf(stderr,"argc [%d] argv[0] [%s]\n",argc,argv[0]);
#ifndef WIN32
	fp=fopen("/srv/wt1.openrda.net/approot/xpert-mysql/dct2126.pdf","rb");
#else
	fp=fopen(argv[1],"rbc");
#endif
	fprintf(stdin,"<html>\r\n");
	fprintf(stdin,"<head>\r\n");
	fprintf(stdin,"</head>\r\n");
	fprintf(stdin,"<body>\r\n");
	fprintf(stdin,"Content-Disposition: inline\r\n");
	fprintf(stdin,"Content-Type: application/pdf\r\n\r\n");
	if(fp!=NULL)
	{
		while(fgets(inbuf,PIPEBLOCKSIZE,fp)!=NULL)
		{
			fprintf(stdin,"%s",inbuf);
			memset(inbuf,0,PIPEBLOCKSIZE);
		}
		fclose(fp);
	} else {
		fprintf(stderr,"Error:  File [%s] unable to be read.\n",argv[1]);
	}
	fprintf(stdin,"</body>\r\n");
	fprintf(stdin,"</html>\r\n");
/*
	unlink(argv[1]);
*/
	return(0);
}

