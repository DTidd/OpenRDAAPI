#include <windows.h>
#include <cstdio>
#define BUFSIZE 1024
#define PIPE_TIMEOUT 5000
#define TRACE   fprintf(stderr,"FILE: [%s] LINE: [%d] DATE: [%s]\n",__FILE__,__LINE__,__DATE__);FlushFileBuffers(stderr)

#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{

	HANDLE hFile;
	BOOL flg;
	DWORD dwWrite;
	char szPipeUpdate[200];
	int x=0;
	FILE *infile;
	int bytes_read=0;
	char *str;



	for(x=0;x<60;x++) {
		hFile=CreateFile("\\\\.\\pipe\\SamplePipe",GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
		if(hFile == INVALID_HANDLE_VALUE) {
			Sleep(1000);
			printf("CreateFile failed for Named Pipe client [%d]\n",x);
		}else{
			printf("CreateFile sucessful [%d]\n",x);
			break;
		}
	}
		strcpy(szPipeUpdate,"Data from Named Pipe client for createnamedpipe");
		if(hFile == INVALID_HANDLE_VALUE) {
			printf("CreateFile failed for Named Pipe client\n" );
		}else{
			
			infile=fopen("C:\\testin.txt","rb");
			str=malloc(4097);
			memset(str,0,4097);

/*
				TRACE;
	        		bytes_read=fread(str,1,4096,infile);
				TRACE;
				flg = WriteFile(hFile,str,bytes_read,&dwWrite,NULL);
				if (FALSE == flg) {
					printf("WriteFile failed for Named Pipe client\n");
				}else{
					printf("WriteFile succeeded for Named Pipe client\n");
				}

*/
			
			while(!feof(infile)) {
	        		bytes_read=fread(str,1,4096,infile);
				printf("fread read [%d] bytes\n",bytes_read);

				TRACE;
	        		//fwrite(str,1,bytes_read,hFile);
				flg = WriteFile(hFile,str,bytes_read,&dwWrite,NULL);
				if (FALSE == flg) {
					printf("WriteFile failed for Named Pipe client\n");
				}else{
					printf("WriteFile succeeded for Named Pipe client\n");
				}

			}
			
			fclose(infile);



			/*
			for(x=60;x!=0;x--) {
				flg = WriteFile(hFile, szPipeUpdate, strlen(szPipeUpdate), &dwWrite, NULL);
				if (FALSE == flg)
				{
					printf("WriteFile failed for Named Pipe client\n");
				}
				else
				{
					printf("WriteFile succeeded for Named Pipe client\n");
				}
			}
			*/
			CloseHandle(hFile);
		}
 
 }

