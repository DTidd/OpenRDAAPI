#include <iostream>

/*
 *  * Now include all podofo header files, to have access
 *   * to all functions of podofo and so that you do not have
 *    * to care about the order of includes.
 *     *
 *      * You should always use podofo.h and not try to include
 *       * the required headers on your own.
 *        */
#include <podofo/podofo.h>

/*
 *  * All podofo classes are member of the PoDoFo namespace.
 *   */
using namespace PoDoFo;


/*
#include <stdarg.h>
#include <cstdio>
#include <cstdlib>
#include <string.h>
*/
#include <sys/types.h>
#include <sys/stat.h>
/*
#include <fcntl.h>
#include <ctime>
#ifndef WIN32
#include </usr/include/libps/pslib.h>
#include <unistd.h>
#include <sys/wait.h>
*/
#include <pwd.h>
#include <uuid/uuid.h>
#include <assert.h>
/*
#endif

#ifdef WIN32
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <rpc.h>
#include <rpcdce.h>
#endif

*/


#ifdef WIN32
#define TRACE   fprintf(stderr,"FILE: [%s] LINE: [%d] DATE: [%s]\n",__FILE__,__LINE__,__DATE__);FlushFileBuffers(stderr);
#else
#define TRACE   fprintf(stderr,"FILE: [%s] LINE: [%d] DATE: [%s]\n",__FILE__,__LINE__,__DATE__);fflush(stderr);
#endif

#define BUFSIZE 65535
#define PIPE_TIMEOUT 5000

class person
{
public:
  std::string name;
  int age;
};

//class D : public B1, private B2
class RDA_PDF_VARIABLES : public PoDoFo::PdfDocument
{
public:
	//PoDoFo::PdfStreamedDocument document;
	//PoDoFo::PdfPage Page;
	//PoDoFo::PdfPainter painter;
	//PoDoFo::PdfFont Font;

	int psfont;
	char *string;
	char *cmdstr;
	int esc_cmd;
	int rda_exec_cmd;
	/* RDA PS PRT VARIABLES */
	float textx;
	float texty;
	int *font_size;
	int *font_height;
	int formfeed; 
	int pagesizedef;
	int landscape;
	int psimage[10000];
	int pstmpl[10000];
	int rdapclborder;
	//  These variables were globals.
	int font_array[150];
	int line_space[4];
	int font_options[150];
	int font_set;
	// These are needed for for ghostscript to use the right page.  Does not allow for changing pagesize mid input at the moment.
	int pagewidth;
	int pageheight;

	char *named_pipe;

	char *input;
	char *output;
	FILE *outputfp;


	int pushpopx;
	int pushpopy;
	int nlcounter;
	int pgcounter;
	int pgopen;
	int printstyle;


	int prepagecount;
	int prepagecmd[150];
	char *prepagecmdstr[150];

	int viewfile;
	int removefile;
	int debug;

};
//typedef struct RDA_PDF_VARIABLES RDA_PDF_VARS;

void ParseCmdLine(int argc, char *argv[],struct RDA_PDF_VARIABLES *myvars);
int ProcessPrePageCmds(struct RDA_PDF_VARIABLES *myvars);
int ParseInput(int argc, char *argv[],struct RDA_PDF_VARIABLES *myvars,int n);
int LaunchPDFViewer(struct RDA_PDF_VARIABLES *myvars);


int FileExists(char *pathname,char *filename)
{

#ifdef WIN32
	return (GetFileAttributes(filename) == INVALID_FILE_ATTRIBUTES) ? -1 : 0;
#else
	return access(filename, R_OK);
#endif
/*
	struct stat *buf;
	if(pathname==NULL) {
		if((stat(filename,buf))==0) {
			return (0);
		}else{
			return (-1);
		}
	//}else{ 
		// This needs to stat() the path to make sure it is a 
		// directory then chdir() to it.  Then in should stat()
		// the filename and return error status of that command.
		// It needs to return to the previous CWD before returning status.
	}
*/
}
char *DecryptDataFromFile(char *filename,char *password,char *inputarray)
{
	char temp1[512];
	FILE *fp=NULL;
	int bytes_read=0,readsize=0,y=0;
	
	memset(temp1,0,512);
#ifndef WIN32
	sprintf(temp1,"echo \"%s\" | gpg --batch --no-tty --status-fd 2 --passphrase-fd 0 --output - --decrypt %s",password,filename);
	fp=popen(temp1,"r");
	if(fp!=NULL)
	{
		while(!feof(fp)) {
			// This needs touching for files over 64k so it is processed in a loop
			memset(inputarray,0,65535);
	        	bytes_read=fread(inputarray,1,65534,fp);
			if(bytes_read==0) {
				//fprintf(stderr,"bytes read [%d] strlen [%d]  y [%d]\n",readsize,(strlen(inputarray)),y);
				y++;
				if(y>2000000) { break; }
			}else {
				y=0;
			}
			readsize=(bytes_read+readsize);
			//fprintf(stderr,"bytes read [%d] str [%d] strlen [%d]\n",readsize,n,(strlen(str)));
		}
		pclose(fp);
	} 
	if((strlen(inputarray))>0) { return inputarray; }
	return NULL;
#endif
}

void ParseCmdLine(int argc, char *argv[],struct RDA_PDF_VARIABLES *myvars) {
	// This function needs to parse the command line
	// and set all relevant values to the myvars structure
	// or execute any needed ExecEscCmd commands.

	int i;
	char *mytemp=NULL;
#ifndef WIN32
	uuid_t out;
#else
	UUID myuuid;
	RPC_CSTR myuuidstr;
#endif
	for (i=1;i<argc;i++) {
		// option -pt# is for page type
		// option -ps# is for print style
		if(myvars->debug) { fprintf(stderr,"Arg [%s]  strlen [%d]\n",argv[i],(strlen(argv[i]))); }
		
		if((strncmp(argv[i],"-i",2))==0) {
			//if((strncmp(mystr2+((strlen(mystr2)) - 4),".eps",4))==0) {

			if((strlen(argv[i]))==2) {
				++i;
				myvars->input=(char*)malloc(sizeof(char*)*((strlen(argv[i]))+1));
				memset(myvars->input,0,(strlen(argv[i]))+1);
				sprintf(myvars->input,"%s",argv[i]);
			} else {
				myvars->input=(char*)malloc(sizeof(char*)*((strlen(argv[i]))+1));
				memset(myvars->input,0,(strlen(argv[i]))+1);
				strncpy(myvars->input,argv[i]+2,(strlen(argv[i]))-2);
			}

		}else if((strncmp(argv[i],"-o",2))==0) {
			if((strlen(argv[i]))==2) {
				++i;
				myvars->output=(char*)malloc(sizeof(char*)*((strlen(argv[i]))+1));
				memset(myvars->output,0,(strlen(argv[i]))+1);
				sprintf(myvars->output,"%s",argv[i]);
			} else {
				myvars->output=(char*)malloc(sizeof(char*)*((strlen(argv[i]))+1));
				memset(myvars->output,0,(strlen(argv[i]))+1);
				strncpy(myvars->output,argv[i]+2,(strlen(argv[i]))-2);
			}
		}else if((strncmp(argv[i],"-d",2))==0) {
			if((strlen(argv[i]))==2) {
				++i;
				myvars->debug=atoi(argv[i]);
			} else {
				// Need to fix to allow -d5 to work using temp string and atoi...
				myvars->debug=1;
			}
		}else if((strcmp(argv[i],"-pt1"))==0) {
			myvars->pagesizedef=1;
			fprintf(stderr,"Arg Found[%s]\n",argv[i]);
		} else if((strcmp(argv[i],"-pt2"))==0) {
			myvars->pagesizedef=2;
			fprintf(stderr,"Arg Found[%s]\n",argv[i]);
		} else if((strcmp(argv[i],"-pt3"))==0) {
			myvars->pagesizedef=3;
			fprintf(stderr,"Arg Found[%s]\n",argv[i]);
		} else if((strcmp(argv[i],"-pt4"))==0) {
			myvars->pagesizedef=4;
			fprintf(stderr,"Arg Found[%s]\n",argv[i]);
		} else if((strcmp(argv[i],"-pt5"))==0) {
			myvars->pagesizedef=5;
			fprintf(stderr,"Arg Found[%s]\n",argv[i]);
		} else if((strcmp(argv[i],"-pt6"))==0) {
			myvars->pagesizedef=6;
			fprintf(stderr,"Arg Found[%s]\n",argv[i]);
		} else if((strcmp(argv[i],"-pt7"))==0) {
			myvars->pagesizedef=7;
			fprintf(stderr,"Arg Found[%s]\n",argv[i]);
		} else if((strcmp(argv[i],"-pt8"))==0) {
			myvars->pagesizedef=8;
			fprintf(stderr,"Arg Found[%s]\n",argv[i]);
		} else if((strcmp(argv[i],"-ps1"))==0) {
			myvars->printstyle=1;
			fprintf(stderr,"Arg Found[%s]\n",argv[i]);
		} else if((strcmp(argv[i],"-ps2"))==0) {
			myvars->printstyle=2;
			fprintf(stderr,"Arg Found[%s]\n",argv[i]);
		} else if((strcmp(argv[i],"--dont-view"))==0) {
			myvars->viewfile=0;
			fprintf(stderr,"Arg Found[%s]\n",argv[i]);
		} else if((strcmp(argv[i],"--dont-delete"))==0) {
			myvars->removefile=0;
			fprintf(stderr,"Arg Found[%s]\n",argv[i]);
		}
	}
				
	mytemp=RDA_GetEnv("VIEWPDF_INPUT_FILENAME");
	if(mytemp!=NULL) {
		myvars->input=(char*)malloc(sizeof(char*)*((strlen(mytemp))+1));
		memset(myvars->input,0,(strlen(mytemp))+1);
		sprintf(myvars->input,"%s",mytemp);
	}

	mytemp=RDA_GetEnv("VIEWPDF_OUTPUT_FILENAME");
	if(mytemp!=NULL) {
		myvars->output=(char*)malloc(sizeof(char*)*((strlen(mytemp))+1));
		memset(myvars->output,0,(strlen(mytemp))+1);
		sprintf(myvars->output,"%s",mytemp);
	}

	mytemp=RDA_GetEnv("VIEWPDF_DONT_VIEW");
	if(mytemp!=NULL) {
		if((strcmp(mytemp,"TRUE"))==0) {
			myvars->viewfile=0;
		}
	}

	mytemp=RDA_GetEnv("VIEWPDF_DONT_DELETE");
	if(mytemp!=NULL) {
		if((strcmp(mytemp,"TRUE"))==0) {
			myvars->removefile=0;
		}
	}





	if((strstr(argv[0],"viewpdf"))!=NULL && myvars->output==NULL) {

		if(myvars->debug)  { fprintf(stderr,"Program Type is ViewPDF [%d].\n",i); }

#ifndef WIN32
		uuid_generate_time(out);
		myvars->output=(char*)malloc(sizeof(char *)*100);
		memset(myvars->output,0,100);
		uuid_unparse_lower(out,myvars->output);

		sprintf(myvars->output,"%s.pdf",myvars->output);
#else
		UuidCreateSequential(&myuuid);
		UuidToStringA(&myuuid,&myuuidstr);
		myvars->output=(char*)malloc(sizeof(char *)*100);
		memset(myvars->output,0,100);
		sprintf(myvars->output,"%s.pdf",myuuidstr);
#endif
		

	}else if(myvars->debug){
		fprintf(stderr,"Program Type is TXT2PS.\n");
	}
	

}
void ExecEscCmd(int rda_exec_cmd,struct RDA_PDF_VARIABLES *myvars) {
	
	char *mystr1=NULL,*mystr2=NULL,*mystr3=NULL,*mystr4=NULL,*mystr5=NULL;
	float myfloat;
	char inputarray[65535];
#ifndef WIN32
	struct passwd *passwd;
#endif

	myfloat=0.0;

	//fprintf(stderr,"rdaexeccmd [%d] cmdstr [%s] string [%s]\n",rda_exec_cmd,myvars->cmdstr,myvars->string);
	
	switch (rda_exec_cmd) {
	case 001:
			//NORMAL
				
			//printf("font1=%d\n",myvars->font_array[1]);
// GOOD				PS_setfont(myvars->ps,myvars->font_array[1],12.0);
// GOOD 			PS_set_value(myvars->ps,"charspacing",0.0);
			myvars->font_array[0]=myvars->font_array[1];
			myvars->line_space[0]=12;
			myvars->font_set=1;
			myvars->printstyle=1;
			break;
	case 002:
			//CONDENSED
// GOOD 			PS_setfont(myvars->ps,myvars->font_array[1],7.0);
// GOOD 			PS_set_value(myvars->ps,"charspacing",0.11);
			myvars->line_space[0]=9;
			myvars->font_set=1;
			myvars->printstyle=2;
				
			break;
	case 050:
			// Preload/Embedd all commonly used fonts.
			PdfEncoding *pEncoding;
			pEncoding = new PdfIdentityEncoding();
			//PdfFont Font = document->CreateFont( "Courier", pEncoding );
// GOOD 			myvars->font_array[1] = PS_findfont(myvars->ps,"c0419bt_","",1);


			
			if(myvars->font_array[1]==0) { printf("Could not load font Courier.\n"); }
			
	
// GOOD 			myvars->font_array[22] = PS_findfont(myvars->ps,"Arial","",1);
// GOOD 			myvars->font_array[23] = PS_findfont(myvars->ps,"Arial_Bold","",1);
			if(myvars->font_array[22]==0) { printf("Could not load font Arial.\n"); }
			if(myvars->font_array[23]==0) { printf("Could not load font Arial Bold.\n"); }

			// Needed for barcodes.  Commenting out because of memory leak in PSLIB on fonts not found.
			//myvars->font_array[10] = PS_findfont(myvars->ps,"MRVCode39extSA","",1);
			
			
			//myvars->font_array[11] = PS_findfont(myvars->ps,"MRVCode39extSA","",1);
			//if(myvars->font_array[10]==0) { printf("Could not load font MRVCode39extSA.\n"); }
			myvars->font_array[0] = myvars->font_array[1];
			if(myvars->font_array[0]==0) { printf("Default Font Invalid.\n"); }
			break;
	case 051:
			// Multi option command to load specific fonts
			// to be used to load less used fonts.
			switch(atoi(myvars->cmdstr)) {
				case 100:
// GOOD 					myvars->font_array[100] = PS_findfont(myvars->ps,"VeraMono","",1);
					myvars->font_options[100]=0;
					if(myvars->font_array[100]==0) { printf("Could not load font VeraMono.\n"); }
					break;
				case 101:
// GOOD 					myvars->font_array[101] = PS_findfont(myvars->ps,"c0419bt_","",1);
					myvars->font_options[101]=0;
					if(myvars->font_array[101]==0) { printf("Could not load font Courier.\n"); }
					break;
			}
			break;
	case 003:
			fprintf(stderr,"Unknown Command 003.\n");
			break;
	case 80:
			//DEF_PARAMS
			//PS_set_parameter(myvars->ps,"warning","true");
// GOOD 			PS_set_parameter(myvars->ps,"warning","false");
// GOOD 			PS_set_parameter(myvars->ps,"imageencoding","hex");

	//		PS_set_parameter(myvars->ps,"BoundingBox","???");
	//		PS_set_parameter(myvars->ps,"Orientation","???");
			
			break;
	case 81:
			//PS_HEADER
			// These need to be set from the command line and from the environment
			// I do not think they can be set after the first page is started...
			// Title/Subject needs to capture REPORT TITLE and OPTIONAL REPORT TITLE variables.
			// Creator needs to be some variation of Xpert and txt2ps...
			// Keywords should include report titles and a date at the minimum.  Maybe module info too?


			//mystr1=(char*)malloc((strlen(RDA_GetEnv("CYBER_PRODUCT")))+3);
			//mystr2=(char*)malloc(512);
			mystr1=RDA_GetEnv("CYBER_PRODUCT");
			if(mystr1==NULL) { 
				mystr1=(char*)malloc(sizeof(char*)*10);
				memset(mystr1,0,10);
				sprintf(mystr1,"UTILITIES");
			}
			
			mystr2=RDA_GetEnv("CYBER_PROCESS");
			if(mystr2==NULL) { 
				mystr2=(char*)malloc(sizeof(char*)*20);
				memset(mystr2,0,20);
				sprintf(mystr2,"TXT2PS CREATE PDF");
			}
			
			mystr3=RDA_GetEnv("REPORT_TITLE");
			if(mystr3==NULL) { 
				mystr3=(char*)malloc(sizeof(char*)*20);
				memset(mystr3,0,20);
				sprintf(mystr3," ");
			}
			
			mystr4=RDA_GetEnv("REPORT_DESCRIPTION");
			if(mystr4==NULL) { 
				mystr4=(char*)malloc(sizeof(char*)*20);
				memset(mystr4,0,20);
				sprintf(mystr4," ");
			}
		
			mystr5=(char*)malloc(sizeof(char*)*((strlen(mystr1))+(strlen(mystr2))+(strlen(mystr3))+(strlen(mystr4))+30));
			memset(mystr5,0,(strlen(mystr1))+(strlen(mystr2))+(strlen(mystr3))+(strlen(mystr4))+30);
			
			sprintf(mystr5,"%s - %s -- %s",mystr1,mystr2,mystr3);
// GOOD 			PS_set_info(myvars->ps,"Title",mystr5);
			document.GetInfo()->SetTitle( PdfString(mystr5) );
			
			sprintf(mystr5,"%s",mystr4);
// GOOD 			PS_set_info(myvars->ps,"Subject",mystr5);
			document.GetInfo()->SetSubject( PdfString(mystr5) );
			
			sprintf(mystr5,"%s - %s -- %s    %s",mystr1,mystr2,mystr3,mystr4);
// GOOD 			PS_set_info(myvars->ps,"Keywords",mystr5);
			document.GetInfo()->SetKeywords( PdfString(mystr5) );

#ifndef WIN32
			//passwd=(char*)malloc(sizeof(passwd));
			if(NULL==(passwd=getpwuid(getuid()))) {
				fprintf(stderr,"getpwuid failed.\n");
				//mystr5=(char*)malloc(sizeof(char*)*20);
				realloc(mystr5,sizeof(char*)*20);
				memset(mystr5,0,20);
				sprintf(mystr5,"%s","testuser");
			}else{
				//mystr5=(char*)malloc(sizeof(char*)*((strlen(passwd->pw_name))+3));
				realloc(mystr5,sizeof(char*)*((strlen(passwd->pw_name))+3));
				memset(mystr5,0,(strlen(passwd->pw_name))+3);
				sprintf(mystr5,"%s",passwd->pw_name);
			}
// GOOD 			PS_set_info(myvars->ps,"Author",mystr5);
			document.GetInfo()->SetAuthor( PdfString(mystr5) );

			// The creator shoudld include the compile date of txt2ps for support purposes...
#ifndef WIN32	
// GOOD 			PS_set_info(myvars->ps,"Creator","OpenRDA (txt2ps.lnx)");
				document.GetInfo()->SetCreator( PdfString("viewpdf.lnx") );
#else
				document.GetInfo()->SetCreator( PdfString("viewpdf.exe") );
				document.GetInfo()->SetApplication( PdfString("viewpdf.exe") );
// GOOD 			PS_set_info(myvars->ps,"Creator","OpenRDA (txt2ps.exe)");
// GOOD 			PS_set_info(myvars->ps,"Application","OpenRDA (viewpdf.exe)");
#endif
			//if((strcmp(mystr1,"UTILITIES"))==0) { free(mystr1); }
			if((strcmp(mystr2,"TXT2PS CREATE PDF"))==0) { free(mystr2); }
			if((strcmp(mystr3," "))==0) { free(mystr3); }
			if((strcmp(mystr4," "))==0) { free(mystr4); }
			free(mystr5);
			//free(passwd); 
			//if(passwd!=NULL) { free(passwd); }
#else
// GOOD 			PS_set_info(myvars->ps,"Author","Robert Goley");
#endif
			break;
	case 101:
			//NEW_PAGE_8.5x11_PORTRAIT  (LETTER)

// GOOD 			PS_begin_page(myvars->ps,612,792);
    			//size = PdfPage::CreateStandardPageSize( ePdfPageSize_Letter, false );
    			//myvars->Page = document->CreatePage( size );
    			//dY    = size.GetHeight() - dY;

			myvars->pagewidth=612;
			myvars->pageheight=792;
			myvars->pgcounter++;
			myvars->pgopen=1;
			
			
			if(myvars->rdapclborder) {
				 myvars->textx = 18.0; 
				 myvars->texty = 748.0; 
			} else {
				 myvars->textx = 18.0; 
				 myvars->texty = 765.0; 
			}

			ProcessPrePageCmds(myvars);
			break;
	case 102:
			//NEW_PAGE_8.5x11_LANDSCAPE  (LETTER)
			
// GOOD 			PS_begin_page(myvars->ps,792,612);
			myvars->pagewidth=792;
			myvars->pageheight=612;
			myvars->pgcounter++;
			myvars->pgopen=1;
			if(myvars->rdapclborder) {
				 myvars->textx = 18.0; 
				 myvars->texty = 568.0; 
			} else {
				 myvars->textx = 18.0; 
				 myvars->texty = 585.0; 
			}
			ProcessPrePageCmds(myvars);
			break;
	case 103:
			//NEW_PAGE_8.5x14_PORTRAIT  (LEGAL)
// GOOD 			PS_begin_page(myvars->ps,612,1008);
			myvars->pagewidth=612;
			myvars->pageheight=1008;
			myvars->pgcounter++;
			myvars->pgopen=1;
			if(myvars->rdapclborder) {
				 myvars->textx = 18.0; 
				 myvars->texty = 964.0; 
			} else {
				 myvars->textx = 18.0; 
				 myvars->texty = 981.0; 
			}
			ProcessPrePageCmds(myvars);
			break;
	case 104:
			//NEW_PAGE_8.5x14_LANDSCAPE  (LEGAL)
			
// GOOD 			PS_begin_page(myvars->ps,1008,612);
			myvars->pagewidth=1008;
			myvars->pageheight=612;
			myvars->pgcounter++;
			myvars->pgopen=1;
			if(myvars->rdapclborder) {
				 myvars->textx = 18.0; 
				 myvars->texty = 568.0; 
			} else {
				 myvars->textx = 18.0; 
				 myvars->texty = 585.0; 
			}
			ProcessPrePageCmds(myvars);
			break;
	case 105:
			//NEW_PAGE_11x14_PORTRAIT  (POSTER)
// GOOD 			PS_begin_page(myvars->ps,792,1008);
			myvars->pagewidth=792;
			myvars->pageheight=1008;
			myvars->pgcounter++;
			myvars->pgopen=1;
			if(myvars->rdapclborder) {
				 myvars->textx = 18.0; 
				 myvars->texty = 964.0; 
			} else {
				 myvars->textx = 18.0; 
				 myvars->texty = 981.0; 
			}
			ProcessPrePageCmds(myvars);
			break;
	case 106:
			//NEW_PAGE_11x14_LANDSCAPE  (POSTER)
// GOOD 			PS_begin_page(myvars->ps,1008,792);
			myvars->pagewidth=1008;
			myvars->pageheight=792;
			myvars->pgcounter++;
			myvars->pgopen=1;
			if(myvars->rdapclborder) {
				 myvars->textx = 18.0; 
				 myvars->texty = 748.0; 
			} else {
				 myvars->textx = 18.0; 
				 myvars->texty = 765.0; 
			}
			ProcessPrePageCmds(myvars);
			break;
	case 107:
			//NEW_PAGE_11x17_PORTRAIT  (LEDGER)
// GOOD 			PS_begin_page(myvars->ps,792,1224);
			myvars->pagewidth=792;
			myvars->pageheight=1224;
			myvars->pgcounter++;
			myvars->pgopen=1;
			if(myvars->rdapclborder) {
				 myvars->textx = 18.0; 
				 myvars->texty = 1180.0; 
			} else {
				 myvars->textx = 18.0; 
				 myvars->texty = 1197.0; 
			}
			ProcessPrePageCmds(myvars);
			break;
	case 108:
			//NEW_PAGE_11x17_LANDSCAPE  (LEDGER)
// GOOD 			PS_begin_page(myvars->ps,1224,792);
			myvars->pagewidth=1224;
			myvars->pageheight=792;
			myvars->pgcounter++;
			myvars->pgopen=1;
			if(myvars->rdapclborder) {
				 myvars->textx = 18.0; 
				 myvars->texty = 748.0; 
			} else {
				 myvars->textx = 18.0; 
				 myvars->texty = 765.0; 
			}
			ProcessPrePageCmds(myvars);
			break;
	case 110:
			//PS_ENDPAGE
// GOOD 			PS_end_page(myvars->ps);
			myvars->pgopen=0;
			break;
	case 111:
// GOOD 			myvars->ps = PS_new();
			break;
	case 112:
			//PS_ENDFILE
// GOOD 			PS_close(myvars->ps);
			break;
	case 113:
			//PS_ENDFILE
// GOOD 			PS_delete(myvars->ps);
			break;
	case 115:
			//SET_FONT
// GOOD			myvars->font_array[12] = PS_findfont(myvars->ps, "Helvetica", "", 0);
			myvars->font_array[0] = myvars->font_array[12];
			break;
	case 120:
			//SET_FONTSIZE
// GOOD 			PS_setfont(myvars->ps,myvars->font_array[10],12.0);
			myvars->font_array[11] = myvars->font_array[0];
			myvars->font_array[0] = myvars->font_array[10];
			break;
	case 121:
			// This function will print the current 
			// string and compute the length of that
			// string and increment textx.  It will
			// then change the default font in the array 
			// and send the pslib command to do the same.
			// It will need to clear the string variable.

			//PS_show_xy(ps,string,*textx_pt,*texty_pt);
// GOOD 			myvars->font_array[10] = PS_findfont(myvars->ps,"MRVCode39extSA","",1);
// GOOD 			PS_setfont(myvars->ps,myvars->font_array[10],12.0);
			myvars->font_array[11] = myvars->font_array[0];
			myvars->font_array[0] = myvars->font_array[10];
			
			break;
	case 200:
			//IMP_PS_IMAGE
// GOOD 			myvars->psimage[0] = PS_open_image_file(myvars->ps,"png","letter.ps", NULL, 0);
			break;
	case 201:
			//IMP_PS_IMAGE
// GOOD 			myvars->psimage[0] = PS_open_image_file(myvars->ps,"jpg","letter.ps", NULL, 0);
			break;
	case 202:
			//IMP_PS_IMAGE
// GOOD 			myvars->psimage[0] = PS_open_image_file(myvars->ps,"eps",myvars->cmdstr, NULL, 0);
			break;
	case 220:
			//Create a page template using an image to 
			//create smaller more efficient output
			//using known psimage id.  PORTRAIT
			//pstmpl=PS_begin_template(ps,"float width","float height")
			//myvars->pstmpl=PS_begin_template(myvars->ps,596,842);
// GOOD 			myvars->pstmpl[0]=PS_begin_template(myvars->ps,596,842);
// GOOD 			PS_place_image(myvars->ps, myvars->psimage[0], 0, 0, 1.0);
// GOOD 			PS_end_template(myvars->ps);
			break;
	case 221:
			//Create a page template using an image to 
			//create smaller more efficient output
			//using new psimage from file PORTRAIT
// GOOD 			myvars->psimage[0] = PS_open_image_file(myvars->ps,"eps","/tmp/letter.ps",NULL,0);
// GOOD 			myvars->pstmpl[1]=PS_begin_template(myvars->ps,612,792);
// GOOD 			PS_place_image(myvars->ps,myvars->psimage[0],0,0,1.0);
// GOOD 			PS_end_template(myvars->ps);
			break;
	case 222:
// GOOD 			PS_place_image(myvars->ps,myvars->pstmpl[0],0,0,1.0);
			break;
	case 250:
/*
			// Load Image Like Similar TO Load PCL MACRO.
			// May want to free the previously loaded images before loading the same number again
			if(strlen(myvars->cmdstr)==0) { break; } 
			mystr1=strtok(myvars->cmdstr,":");
			mystr2=strtok(NULL,":");
			if((strncmp(mystr2+((strlen(mystr2)) - 4),".eps",4))==0) {
				myvars->psimage[(atoi(mystr1))]=PS_open_image_file(myvars->ps,"eps",mystr2,NULL,(atoi(mystr1)));
			} else if((strncmp(mystr2+((strlen(mystr2)) - 3),".ps",3))==0) {
				myvars->psimage[(atoi(mystr1))]=PS_open_image_file(myvars->ps,"eps",mystr2,NULL,(atoi(mystr1)));
			} else if((strncmp(mystr2+((strlen(mystr2)) - 4),".png",4))==0) {
				myvars->psimage[(atoi(mystr1))]=PS_open_image_file(myvars->ps,"png",mystr2,NULL,(atoi(mystr1)));
			} else if((strncmp(mystr2+((strlen(mystr2)) - 4),".jpg",4))==0) {
				myvars->psimage[(atoi(mystr1))]=PS_open_image_file(myvars->ps,"jpeg",mystr2,NULL,(atoi(mystr1)));
			} else if((strncmp(mystr2+((strlen(mystr2)) - 5),".jpeg",5))==0) {
				myvars->psimage[(atoi(mystr1))]=PS_open_image_file(myvars->ps,"jpeg",mystr2,NULL,(atoi(mystr1)));
			} else if((strncmp(mystr2+((strlen(mystr2)) - 4),".tif",4))==0) {
				myvars->psimage[(atoi(mystr1))]=PS_open_image_file(myvars->ps,"tiff",mystr2,NULL,(atoi(mystr1)));
			} else if((strncmp(mystr2+((strlen(mystr2)) - 5),".tiff",5))==0) {
				myvars->psimage[(atoi(mystr1))]=PS_open_image_file(myvars->ps,"tiff",mystr2,NULL,(atoi(mystr1)));
			}
			//printf("str1=[%s]  str2=[%s] str3=[%s] str4=[%s] str5=[%s] \n",mystr1,mystr2,mystr3,mystr4,mystr5);
			//printf("psimage=%d\n",myvars->psimage[(atoi(mystr1))]);
*/
			break;
	case 251:
			// Print Image Like PCL Call MACRO 
			if(strlen(myvars->cmdstr)==0) { break; } 
			//printf("pgopen=[%d]  \n",myvars->pgopen);
			//if(myvars->pgopen==0) {	ExecEscCmd(90,myvars);	}
			//printf("pgopen=[%d]  \n",myvars->pgopen);
			mystr1=strtok(myvars->cmdstr,":");
			mystr2=strtok(NULL,":");
			mystr3=strtok(NULL,":");
			//printf("pgopen=[%d]  str1=[%s]  str2=[%s] str3=[%s] str4=[%s] str5=[%s] \n",myvars->pgopen,mystr1,mystr2,mystr3,mystr4,mystr5);
// GOOD 			PS_place_image(myvars->ps,myvars->psimage[(atoi(mystr1))],(atoi(mystr2)),(atoi(mystr3)),1.0);
			break;
	case 252:
			//PRT_PS_IMAGE
// GOOD 			PS_place_image(myvars->ps,myvars->psimage[0],0,0,1.0);
			break;
	case 253:
			//PRT_PS_IMAGE
			//printf("psimage=%d\n",myvars->psimage[0]);
			//printf("cmdstr=%s\n",myvars->cmdstr);
			mystr1=strtok(myvars->cmdstr,":");
			mystr2=strtok(NULL,":");
			mystr3=strtok(NULL,":");
			mystr4=strtok(NULL,":");
			mystr5=strtok(NULL,":");
			//printf("str1=[%s]  str2=[%s] str3=[%s] str4=[%s] str5=[%s] \n",mystr1,mystr2,mystr3,mystr4,mystr5);
			
			if(myvars->psimage[0]==0) {
// GOOD 				myvars->psimage[0] = PS_open_image_file(myvars->ps,"eps","/tmp/letter.ps",NULL,0);
			} 
// GOOD 			PS_place_image(myvars->ps,myvars->psimage[0],0,0,1.0);
			//printf("psimage=%d\n",myvars->psimage[0]);
			break;
	case 300:
// GOOD 			PS_stringwidth(myvars->ps,myvars->string,myvars->psfont,12);
			//fprintf(stderr,"PRT_TEXT_XY STRLEN=%s\n",(PS_stringwidth(ps,string,psfont,12)));
			//PRT_TEXT_XY
// GOOD 			PS_show_xy(myvars->ps,myvars->string,300,300);
			break;
	case 301:
			//fprintf(stderr,"PRT_TEXT_XY STRLEN=%d\n",(PS_stringwidth(ps,string,psfont,7)));
			//PRT_TEXT_XY
			//fprintf(stderr,"X[%f} Y[%f] MY_X[%d] MY_Y[%d]",*textx_pt,*texty_pt,my_textx,my_texty);
			//PS_show_xy(ps,string,&textx,&texty);
			//fprintf(stderr,"StringWidth=%5.2f\n",PS_stringwidth(ps,string,font_array[0],12));
			
// GOOD 			PS_show_xy(myvars->ps,myvars->string,myvars->textx,myvars->texty);
			
			//PS_show_xy(myvars->ps,myvars->string,myvars->*textx_pt,myvars->*texty_pt);


			break;
	case 310:
 			// STUB* Variable Name: [RESET PRINTER] 
			// Not sure if this should be implemented or ignored yet.
			// It may need to delete all macros or fonts for compatibility.  It is not used much though...
			break;
	case 311:
 			// STUB* Variable Name: [SIGNATURE DIMM 1] 
			// This is slowly going away.  Probably can be ignored.
			break;
	case 312:
			// STUB* Variable Name: [SIGNATURE DIMM A] 
			// This is slowly going away.  Probably can be ignored.
			break;
	case 313:
			// STUB* Variable Name: [PUSH CURSOR] 
			// PCL Push equiv command
		//	if(myvars->pgopen==0) {
				// This needs to take the page size into account instead of just LETTER/PORTRAIT
				// May not be needed now that prepage commands are processed post page start.
		//		 myvars->pushpopx=18.0; 
		//		 myvars->pushpopy=748.0; 
		//	} else {
				myvars->pushpopx=myvars->textx;
				myvars->pushpopy=myvars->texty;
		//	}
			break;
	case 314:
			// STUB* Variable Name: [POP CURSOR] 
			// PCL Pop equiv command
			myvars->textx=myvars->pushpopx;
			myvars->texty=myvars->pushpopy;
			break;
	case 315:
			// Variable Name: [NORMAL TEXT] 
			//if(myvars->pgopen==0) {	ExecEscCmd(90,myvars);	}
			if(myvars->pgopen==0) {
				myvars->printstyle=1;
			} else {	
				ExecEscCmd(001,myvars);
			}
				
			break;
	case 316:
			// Variable Name: [COMPRESSED TEXT] 
			//if(myvars->pgopen==0) {	ExecEscCmd(90,myvars);	}
			if(myvars->pgopen==0) {
				myvars->printstyle=2;
			} else {	
				ExecEscCmd(002,myvars);
			}
			break;
	case 317:
			// STUB* Variable Name: [ELITE PRINT] 
			if(myvars->debug) { fprintf(stderr,"STUB: ELITE PRINT Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			break;
	case 318:
			// STUB* Variable Name: [PAGE_SIZE_LETTER] 
			if(myvars->debug) { fprintf(stderr,"STUB: PAGE_SIZE_LETTER Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			if(myvars->landscape==1){
				myvars->pagesizedef=2;
			}else{
				myvars->pagesizedef=1;
			}

			break;
	case 319:
			// STUB* Variable Name: [PAGE_SIZE_LEGAL] 
			if(myvars->debug) { fprintf(stderr,"STUB: PAGE_SIZE_LEGAL Not Implemented [%d].\n",myvars->rda_exec_cmd);}
			if(myvars->landscape==1){
				myvars->pagesizedef=4;
			}else{
				myvars->pagesizedef=3;
			}
			break;
	case 320:
			// STUB* Variable Name: [PAGE_SIZE_LEDGER] 
			if(myvars->debug) { fprintf(stderr,"STUB: PAGE_SIZE_LEDGER Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			if(myvars->landscape==1){
				myvars->pagesizedef=8;
			}else{
				myvars->pagesizedef=7;
			}
			break;
	case 321:
			// STUB* Variable Name: [PAGE_SIZE_POSTER] 
			if(myvars->debug) { fprintf(stderr,"STUB: PAGE_SIZE_POSTER Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			if(myvars->landscape==1){
				myvars->pagesizedef=6;
			}else{
				myvars->pagesizedef=5;
			}
			break;
	case 330:
			// STUB* Variable Name: [PORTRAIT] 
			if(myvars->debug) { fprintf(stderr,"STUB: PORTRAIT Partially Implemented for Letter Sized Paper [%d].\n",myvars->rda_exec_cmd); }
			myvars->landscape=0;
			switch (myvars->pagesizedef) {
				case 2:
				case 4:
				case 6:
				case 8:
					--myvars->pagesizedef;
					// Hopefully, this will work for all defined page types.  
					break;
				default:
					// This should ignore any other page size changes for non landscape page sizes.
					break;
			}
			break;
	case 331:
			// STUB* Variable Name: [LANDSCAPE] 
			if(myvars->debug) { fprintf(stderr,"STUB: LANDSCAPE Partially Implemented for Letter Sized Paper [%d].\n",myvars->rda_exec_cmd); }
			myvars->landscape=1;
			switch (myvars->pagesizedef) {
				case 0:
					++myvars->pagesizedef;
					++myvars->pagesizedef;
					break;
				case 1:
				case 3:
				case 5:
				case 7:
					++myvars->pagesizedef;
					// Hopefully, this will work for all defined page types.  
					break;
				default:
					// This should ignore any other page size changes for non portrait page sizes.
					break;
			}
			break;
	case 332:
			// STUB* Variable Name: [SIMPLEX] 
			if(myvars->debug) { fprintf(stderr,"STUB: SIMPLEX Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			break;
	case 333:
			// STUB* Variable Name: [DUPLEX LONG] 
			if(myvars->debug) { fprintf(stderr,"STUB: DUPLEX LONG Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			break;
	case 334:
			// STUB* Variable Name: [DUPLEX SHORT] 
			if(myvars->debug) { fprintf(stderr,"STUB: DUPLEX SHORT Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			break;
	case 335:
			// STUB* Variable Name: [REVERSE PORTRAIT] 
			if(myvars->debug) { fprintf(stderr,"STUB: REVERSE PORTRAIT Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			break;
	case 336:
			// STUB* Variable Name: [REVERSE LANDSCAPE] 
			if(myvars->debug) { fprintf(stderr,"STUB: REVERSE LANDSCAPE Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			break;
	case 337:
			// STUB* Variable Name: [ASCII SQUARE] 
			if(myvars->debug) { fprintf(stderr,"STUB: ASCII SQUARE Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			break;
	case 338:
			// STUB* Variable Name: [ESCAPE] 
			if(myvars->debug) { fprintf(stderr,"STUB: ESCAPE Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			break;
	case 339:
			// STUB* Variable Name: [HALF LINE FEED] 
			if(myvars->debug) { fprintf(stderr,"STUB: HALF LINE FEED [%d].\n",myvars->rda_exec_cmd); }
			ExecEscCmd(301,myvars);

			sprintf(myvars->string, "%c",0);
			// Not sure what the actual postscript pixel value is for a half line feed.  Testing...
			//myvars->texty=(myvars->texty - 6);
			//myvars->texty=(myvars->texty - 9);
			myvars->texty=(myvars->texty - 12);
			
			//myvars->texty=(myvars->texty - myvars->line_space[0]);
			break;
	case 340:
			// STUB* Variable Name: [VENTREAS] 
			if(myvars->debug) { fprintf(stderr,"STUB: VENTREAS Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			break;
	case 341:
			// STUB* Variable Name: [PAYREG] 
			if(myvars->debug) { fprintf(stderr,"STUB: PAYREG Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			break;
	case 342:
			// STUB* Variable Name: [POCERT] 
			if(myvars->debug) { fprintf(stderr,"STUB: POCERT Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			break;
	case 343:
			// STUB* Variable Name: [PAYCKRG] 
			if(myvars->debug) { fprintf(stderr,"STUB: PAYCKRG Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			break;
	case 344:
			// STUB* Variable Name: [VENCKRG] 
			if(myvars->debug) { fprintf(stderr,"STUB: VENCKRG Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			break;
	case 345:
			// STUB* Variable Name: [SIGNATURE LOCK] 
			if(myvars->debug) { fprintf(stderr,"STUB: SIGNATURE LOCK Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			break;
	case 346:
			// STUB* Variable Name: [SIGNATURE UNLOCK] 
			if(myvars->debug) { fprintf(stderr,"STUB: SIGNATURE UNLOCK Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			break;
	case 347:
			// STUB* Variable Name: [POAUDIT] 
			if(myvars->debug) { fprintf(stderr,"STUB: POAUDIT Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			break;
	case 348:
			// STUB* Variable Name: [SIGNATURE FILE] 
			if(myvars->debug) { fprintf(stderr,"STUB: SIGNATURE FILE Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			break;
	case 349:
			// * Variable Name: [UNLOAD_SOFTFONTS] 
			if(myvars->debug) { fprintf(stderr,"STUB: UNLOAD_SOFTFONTS Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			break;
	case 350:
			// * Variable Name: [RESET FONT] 
			if(myvars->debug) { fprintf(stderr,"STUB: RESET FONT Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			break;
	case 351:
			// * Variable Name: [LOAD VENPMT SIGNATURE] 
			if(myvars->debug) { fprintf(stderr,"STUB: LOAD VENPMT SIGNATURE Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			// This will load all available rda-ven[123456789].dat files as separate 
			// signature images if the files exist. File checks will be done with 
			// the stat() function.The image numbers will correspond with the file 
			// number.  The passed string will be XY coordinates for the image placement.
			if(!FileExists(NULL,"rda-ven1.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-ven2.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-ven3.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-ven4.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-ven5.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-ven6.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-ven7.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-ven8.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-ven9.dat")) { TRACE; }
			break;
	case 352:
			// * Variable Name: [LOAD PAYROLL SIGNATURE] 
			if(myvars->debug) { fprintf(stderr,"STUB: LOAD PAYROLL SIGNATURE Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			// This will load all available rda-pay[123456789].dat files as separate 
			// signature images if the files exist. File checks will be done with 
			// the stat() function.The image numbers will correspond with the file 
			// number.  The passed string will be XY coordinates for the image placement.
			// This will need to pass the password as well...
			if(!FileExists(NULL,"rda-pay1.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-pay2.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-pay3.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-pay4.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-pay5.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-pay6.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-pay7.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-pay8.dat")) { TRACE; }
			if(!FileExists(NULL,"rda-pay9.dat")) { TRACE; }

			break;
	case 353:
			// * Variable Name: [VENPMT SIGNATURE] 
			if(myvars->debug) { fprintf(stderr,"STUB: VENPMT SIGNATURE Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			// This will call a general function with one argument only.  The argument will 
			// be a series of single digit numbers.  This is the number of the signature
			// loaded in the correct order to print the signature.  If not corresponding 
			// XY coordinates are set, the signature will not be printed.
			break;
	case 354:
			// * Variable Name: [PAYROLL SIGNATURE] 
			if(myvars->debug) { fprintf(stderr,"STUB: PAYROLL SIGNATURE Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			// This will call a general function with one argument only.  The argument will 
			// be a series of single digit numbers.  This is the number of the signature
			// loaded in the correct order to print the signature.  If not corresponding 
			// XY coordinates are set, the signature will not be printed.
			break;
	case 355:
			// * Variable Name: [DIRECT DEPOSIT] 
			if(myvars->debug) { fprintf(stderr,"STUB: DIRECT DEPOSIT Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			break;
	case 356:
			// * Variable Name: [PAYROLL CHECK] 
			if(myvars->debug) { fprintf(stderr,"STUB: PAYROLL CHECK Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			break;
	case 357:
			if(myvars->debug) { fprintf(stderr,"STUB: Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			break;
	case 358:
			if(myvars->debug) { fprintf(stderr,"STUB: Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			break;
	case 359:
			if(myvars->debug) { fprintf(stderr,"STUB: Not Implemented [%d].\n",myvars->rda_exec_cmd); }
			break;
	case 399:
			//fprintf(stderr,"%s",cmdstr[4],cmdstr[5]);
			
			//fprintf(stderr,"STRLEN[%d]  %c\n",strlen(cmdstr),cmdstr[3]);
			break;
	default: 
			if(myvars->debug) { fprintf(stderr,"Unknown Command [%d].\n",myvars->rda_exec_cmd); }
			break;
	}

	// Free resources only used in this function
	mystr1=NULL;
	mystr2=NULL;
	mystr3=NULL;
	mystr4=NULL;
	mystr5=NULL;
	//if(mystr1!=NULL) free(mystr1);
	//if(mystr2!=NULL) free(mystr2);  
	//if(mystr3!=NULL) free(mystr3);
	//if(mystr4!=NULL) free(mystr4);
	//if(mystr5!=NULL) free(mystr5);
	
}



//#define LEFT_BORDER 50

#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	FILE *fp;
	FILE *MYFP;
	struct stat fst;
	size_t bytes_read;
	int n=0;
	int x=0,y=0;
	int readsize=0;
	char *tempstr;
	char inputarray[65535];
	RDA_PDF_VARIABLES::RDA_PDF_VARIABLES *myvars;

#ifdef WIN32
	BOOL fConnected;
	//LPTSTR lpszPipename = "\\\\.\\pipe\\SamplePipe";
	LPTSTR lpszPipename=NULL;
	CHAR chRequest[BUFSIZE];
	DWORD cbBytesRead;
	BOOL fSuccess;
	HANDLE hPipe;
	
#endif

	myvars = new RDA_PDF_VARIABLES::RDA_PDF_VARIABLES();

	// This section sets program defaults 
	// These can be overriden by command lines options
	// or by the options passed to the command interpreter

	//PdfStreamedDocument myvars->document( pszOutput, ePdfVersion_1_4);
	myvars->psfont=0;
	myvars->string=NULL;
	myvars->cmdstr=NULL;
	myvars->string=(char*)malloc(sizeof(char*)*512);
	myvars->cmdstr=(char*)malloc(sizeof(char*)*512);
	memset(myvars->string,0,512);
	memset(myvars->cmdstr,0,512);
	myvars->esc_cmd=0;
	myvars->rda_exec_cmd=0;
	myvars->textx=0.0;
	myvars->texty=0.0;
	myvars->font_size=0;
	myvars->font_height=0;
	myvars->formfeed=0;
	myvars->pagesizedef=0;
	myvars->landscape=0;
	myvars->viewfile=1;
	myvars->removefile=1;
	for(x=0;x<10000;x++) { myvars->psimage[x]=0; }
	for(x=0;x<10000;x++) { myvars->pstmpl[x]=0; }
	myvars->rdapclborder=1;
	for(x=0;x<150;x++) { myvars->font_array[x]=0; }
	for(x=0;x<4;x++) { myvars->line_space[x]=0; }
	for(x=0;x<150;x++) { myvars->font_options[x]=0; }
	myvars->font_set=0;
	// These are needed for for ghostscript to use the right page.  Does not allow for changing pagesize mid input at the moment.
	// Not used anymore, might be used later to handle mtnmst browse printing...
	myvars->pagewidth=0;
	myvars->pageheight=0;

	
#ifdef WIN32
	myvars->named_pipe=NULL;
	myvars->named_pipe=RDA_GetEnv("VIEWPDF_PIPE");
	if(myvars->debug) { fprintf(stderr,"VIEWPDF_PIPE [%s]\n",myvars->named_pipe); }

	if(myvars->named_pipe!=NULL) {
		fprintf(stderr,"VIEWPDF_PIPE SIZE [%d]\n",strlen(myvars->named_pipe));
		lpszPipename=(LPTSTR)malloc((strlen(myvars->named_pipe))+15);
		sprintf(lpszPipename,"\\\\.\\pipe\\%s",myvars->named_pipe);
	}
#endif
	myvars->input=NULL;
	myvars->output=NULL;
	
	myvars->pushpopx=0;
	myvars->pushpopy=0;
	myvars->nlcounter=0;
	myvars->pgcounter=0;
	myvars->pgopen=0;
	myvars->printstyle=1;
	myvars->prepagecount=0;

	for(x=0;x<150;x++) { 
		myvars->prepagecmd[x]=0; 
		myvars->prepagecmdstr[x]=NULL;
		myvars->prepagecmdstr[x]=(char*)malloc(sizeof(char *)*512);
		memset(myvars->prepagecmdstr[x],0,512);
	}
	
	
	ParseCmdLine(argc,argv,myvars);
#ifdef WIN32
	if(myvars->debug==0) { FreeConsole(); }
#endif
// GOOD 	PS_boot();
// GOOD 	myvars->ps = PS_new();
	

/*  Added for file read */


	if(myvars->input==NULL&&myvars->named_pipe==NULL) {
		fprintf(stderr,"input is stdin.\n");
		fp=stdin;

#ifdef WIN32
	}else if(myvars->input==NULL&&myvars->named_pipe!=NULL) {
		fprintf(stderr,"input is named pipe [%s].\n",myvars->named_pipe);
		hPipe=CreateNamedPipe(lpszPipename,PIPE_ACCESS_DUPLEX,PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,PIPE_UNLIMITED_INSTANCES,BUFSIZE,BUFSIZE,PIPE_TIMEOUT,NULL); 
		if (hPipe == INVALID_HANDLE_VALUE) {
			printf("INVALID PIPE HANDLE\n");
			return 1;
		}

		fConnected = ConnectNamedPipe(hPipe, NULL) ?
			TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
		
#endif
		/*
#ifdef WIN32
	}else if(myvars->input==NULL&&myvars->input==NULL&&myvars->named_pipe!=NULL) {
		fprintf(stderr,"input is named pipe [%s].\n",myvars->named_pipe);
		if(NULL==(fp=fopen(myvars->input,"rb"))) {
			fprintf(stderr, "Cannot open input file\n");
			exit(1);
		}
		str=(char*)malloc(sizeof(char *)*100000000);
#endif
*/
	} else {
		if(NULL==(fp=fopen(myvars->input,"r"))) {
			fprintf(stderr, "Cannot open input file\n");
			exit(1);
		}
		//stat(myvars->input, &fst);
		//text = malloc(sizeof(char *)*(fst.st_size+1));
		//str = text;
	}

	if(myvars->output==NULL) {
// GOOD 		if (0 > PS_open_fp(myvars->ps,stdout)) {
// GOOD 			printf("Cannot open PS file descriptor for stdout.\n");
// GOOD 			exit(1);
// GOOD 		}
	}else{
		if((strncmp(myvars->output+((strlen(myvars->output)) - 4),".pdf",4))==0) { 

			//myvars.document = new PdfStreamedDocument( myvars->output, ePdfVersion_1_4);
			// Should use PdfStreamDocument here.
			
		}
	}

	ExecEscCmd(80,myvars);
	ExecEscCmd(81,myvars);
	ExecEscCmd(050,myvars);
	
	if(myvars->debug) {
		fprintf(stderr,"font_array[0]==%d\n",myvars->font_array[0]);
		fprintf(stderr,"font_array[1]==%d\n",myvars->font_array[1]);
		fprintf(stderr,"font_array[10]==%d\n",myvars->font_array[10]);
	}

	myvars->textx = 0.0;
	myvars->texty = 0.0;
	
	//printf(" In Main Before: Pointers for texttx and textty %ul %ul\n", (unsigned long) myvars->textx,myvars->texty);
	
	//printf(" In Main Before: Pointers for texttx and textty %ul %ul\n", (unsigned long) myvars->&textx,myvars->&texty);
	//fprintf(stderr,"\nBEFORE X[%5.2f] Y[%5.2f]\n",textx,texty);
	
	// TODO: command parser to default first page and page size defaults
	// needs to set pagesizedef to 0 for not set, 1 for set(Letter), 
	// other page sizes and types need to be set here and called on the NL
	
	if(myvars->debug) { fprintf(stderr,"BEFORE SWITCH X[%5.2f] Y[%5.2f]\n",myvars->textx,myvars->texty); }
	
	readsize=0;
	if(myvars->named_pipe!=NULL) {
		memset(inputarray,0,BUFSIZE);
#ifdef WIN32
		while(1) {
			fSuccess=ReadFile(hPipe,inputarray,BUFSIZE,&cbBytesRead,NULL); 
			if(myvars->debug) { fprintf(stderr,"bytes read [%d] strlen [%d]\n",readsize,(strlen(inputarray))); }
			inputarray[cbBytesRead] = '\0';
			if (! fSuccess || cbBytesRead == 0) { break; }
			for(x=0;x<cbBytesRead;x++) {
				n=inputarray[x];	
				ParseInput(argc,argv,myvars,n);
			}
			readsize=(cbBytesRead+readsize);
		}
			fprintf(stderr,"bytes read [%d] \n",readsize); 

		ExecEscCmd(110,myvars);
		ExecEscCmd(112,myvars);
		ExecEscCmd(113,myvars);
#endif
		//fwrite(chRequest,1,cbBytesRead,outfile);
		//printf("Size: [%d]  Data: [%s]\n",cbBytesRead,chRequest);

	//}else if(myvars->input!=NULL) {	
	}else{	
		while(!feof(fp)) {
			memset(inputarray,0,65535);
	        	bytes_read=fread(inputarray,1,65534,fp);
			if((fp==stdin)&&(bytes_read==0)) {
				//fprintf(stderr,"bytes read [%d] strlen [%d]  y [%d]\n",readsize,(strlen(inputarray)),y);
				y++;
				if(y>2000000) { break; }
			}else if(fp==stdin) {
				y=0;
			}
			for(x=0;x<bytes_read;x++) {
				n=inputarray[x];	
				ParseInput(argc,argv,myvars,n);
			}
			readsize=(bytes_read+readsize);
			//fprintf(stderr,"bytes read [%d] str [%d] strlen [%d]\n",readsize,n,(strlen(str)));
		}
		fclose(fp);
		ExecEscCmd(110,myvars);
		ExecEscCmd(112,myvars);
		ExecEscCmd(113,myvars);
	}

	// Need to Close PdfDocument here
	//PS_shutdown();
	
	if(((strstr(argv[0],"viewpdf"))!=NULL)&&myvars->debug) {
		fprintf(stderr,"Program Type is ViewPDF.\n");
		LaunchPDFViewer(myvars);
	}else if((strstr(argv[0],"viewpdf"))!=NULL) {
		LaunchPDFViewer(myvars);
	}else if(myvars->debug){
		fprintf(stderr,"Program Type is TXT2PS.\n");
	}
	
	n=0;
	if(myvars->cmdstr!=NULL) free(myvars->cmdstr);
	free(myvars->input);
	free(myvars->output);
	free(myvars->string);

	for(x=0;x<150;x++) { 
		free(myvars->prepagecmdstr[x]);
	}
	if(myvars!=NULL) free(myvars);
// 	fprintf(stderr,"tempstr [%s]\n",tempstr);
/*
	if((strlen(tempstr))!=0) {
			free(tempstr);
	//		pclose(MYFP);
	}
*/
	std::exit;
}

int ParsePrePageCmds(int argc, char *argv[],struct RDA_PDF_VARIABLES *myvars)
{
//	fprintf(stderr,"PROCESS PAGE [%d] COUNT [%d]  \n",(myvars->pgcounter),(myvars->prepagecount));
	if((myvars->pgopen==0)||(myvars->formfeed==1)) {
		

	// Just need a switch case statement here for cmds to process before page is opened.
	// Could likely use return code from this command to decide whether to execute
	// command in the calling function.  Make things simple...
		
		switch (myvars->rda_exec_cmd) {
		case 0:
		case 315:
		case 316:
		case 318:
		case 319:
		case 320:
		case 321:
		case 330:
		case 331:
		
			if(myvars->debug) { fprintf(stderr,"RUN PRE PAGE CMD [%d]  [%s]\n",myvars->rda_exec_cmd,myvars->cmdstr); }
			ExecEscCmd(myvars->rda_exec_cmd,myvars);
			return(1);
			break;
		default: 
			myvars->prepagecount++;
			if(myvars->debug) { fprintf(stderr,"STORE PRE PAGE COUNT [%d] CMD [%d]  [%s]\n",myvars->prepagecount,myvars->rda_exec_cmd,myvars->cmdstr); }

//			fprintf(stderr,"PRE PAGE CMD [%s]\n",myvars->prepagecmd[(myvars->prepagecount)]);
			myvars->prepagecmd[(myvars->prepagecount)]=myvars->rda_exec_cmd;
		//	myvars->prepagecmdstr[(myvars->prepagecount)]=(char*)malloc((strlen(myvars->cmdstr))+1);
			sprintf(myvars->prepagecmdstr[(myvars->prepagecount)],"%s",myvars->cmdstr);
			return(0);
			break;
		}
		
//
//	} else if (myvars->pgopen==1) {
//		fprintf(stderr,"POST PAGE CMD [%d]  [%s]\n",myvars->rda_exec_cmd,myvars->cmdstr);
//	}else{  
//		fprintf(stderr,"WIERD PAGE CMD [%d]  [%s]\n",myvars->rda_exec_cmd,myvars->cmdstr);
//
	}


}
int ProcessPrePageCmds(struct RDA_PDF_VARIABLES *myvars)
{
	int i=0;
	char *tempstr;
//	fprintf(stderr,"PROCESS PAGE [%d] COUNT [%d] PGOPEN [%d] FF [%d]  \n",(myvars->pgcounter),(myvars->prepagecount),(myvars->pgopen),(myvars->formfeed));
	while(1){
		if((myvars->prepagecmd[(i)]!=0)) {
			if(myvars->debug) { fprintf(stderr,"PROCESS PRE PAGE CMD [%d] [%s] \n",myvars->prepagecmd[(i)],myvars->prepagecmdstr[(i)]); }

			tempstr=(char*)malloc(sizeof(char *)*((strlen(myvars->prepagecmdstr[(i)]))+1));
			sprintf(tempstr,"%s",myvars->prepagecmdstr[(i)]);

			
//			sprintf(myvars->cmdstr,"%c",0);
			sprintf(myvars->cmdstr,"%s",(myvars->prepagecmdstr[(i)]));
			ExecEscCmd(myvars->prepagecmd[(i)],myvars);
			sprintf(myvars->cmdstr,"%s",tempstr);


			
			myvars->prepagecmd[(i)]=0;
			//sprintf(myvars->prepagecmdstr[(i)],"%c",0);
			
			//sprintf(tempstr,"%c",0);
			free(tempstr);
	
	/*
		}else{
			fprintf(stderr,"PROCESS ZERO OR NULL CMD \n");
	*/
		}
		
		if(myvars->prepagecount==0) { break; }
		if(i==myvars->prepagecount) { break; }
		i++;
	}
	myvars->prepagecount=0;
		
	return(0);
}

int LaunchPDFViewer(struct RDA_PDF_VARIABLES *myvars)
{
	int execstatus=0;
	char *mytemp=NULL;
	char *myenv1=NULL,*myenv2=NULL,*myenv3=NULL;

#ifdef WIN32
	DWORD bufsize=0;
#endif

	if(myvars->viewfile) {
		mytemp=(char*)malloc(sizeof(char *)*512);
#ifndef WIN32
		myenv1=RDA_GetEnv("CYBER_PRODUCT");
		myenv2=RDA_GetEnv("CYBER_PROCESS");
		myenv3=RDA_GetEnv("REPORT_TITLE");
		sprintf(mytemp,"acroread -tempFileTitle \"%s - %s - %s\" -openInNewWindow \"%s\"",(myenv1!=NULL ? myenv1:"UTILITIES"),(myenv2!=NULL ? myenv2:"ViewPDF"),(myenv3!=NULL ? myenv3:myvars->output),myvars->output);
		if(myvars->debug) {
			fprintf(stderr,"CMD:  [%s]\n",mytemp);
		}else{
			fprintf(stderr,"Output File: %s\n",myvars->output);
		}
	
	 
		execstatus=system(mytemp);
		if(myvars->debug) { fprintf(stderr,"Exit Code: %d\n",WEXITSTATUS(execstatus)); }
		if(myvars->removefile) {
			unlink(myvars->output);
		}
	
#else
	/*
	 * This should read this variable from the registry and then
	 * set mytemp with the command and the filename to use with the exec.
	 * Need to make sure it allows waiting on acrobat to finish as well.  
	 *
[HKEY_CLASSES_ROOT\acrobat\shell\open\command]
@="\"C:\\Program Files\\Adobe\\Reader 9.0\\Reader\\AcroRd32.exe\" /u \"%1\""
	 *
	*/
		myenv1=(char*)malloc(sizeof(char *)*512);
		GetCurrentDirectoryA(&bufsize,myenv1);
		sprintf(mytemp,"%s\\%s",myenv1,myvars->output);
		fprintf(stderr,"Output File: %s\\%s\n",myenv1,myvars->output);
		ShellExecute(NULL,"open",mytemp,NULL,NULL,SW_SHOWNORMAL);
		Sleep(10000);
		while(1) {
			//  Could possibly use MoveFileEx instead if file needs kept.
			//  May also need to stat() to make sure file exists to 
			//  prevent an infinite loop.
			if((unlink(mytemp))==0) { break; }
			Sleep(10000);
		}
		free(myenv1);
#endif
		free(mytemp);
	}
	return(0);
}
int ParseInput(int argc, char *argv[],struct RDA_PDF_VARIABLES *myvars,int n)
{
		switch (n) {
			// 
			// This section may need to be redone to handle extra command parsing on NLs 
			// so that page and font instructions passed via commands will have appropriate
			// variables set before starting the first page.  Likely will be handled by doing 
			// command parsing/setting until the first valid line of printable text is parsed.
			// After the first printable line of text is found, it will then launch all
			// of the commands currently being called before this loop is entered.
			//
			case 10:
				
				if(myvars->debug) { fprintf(stderr, "NL [%d]  pagesizedef [%d]   landscape [%d]\n",myvars->nlcounter,myvars->pagesizedef,myvars->landscape); }
				if(myvars->formfeed==1) {
					myvars->formfeed=0;
					myvars->nlcounter=0;
					ExecEscCmd(110,myvars);
				}
				//fprintf(stderr,"BEFORE NL X[%5.2f] Y[%5.2f]\n",myvars->textx,myvars->texty);
				myvars->nlcounter++;
				//fprintf(stderr, "Case: NL [%d] \n",n);

				// TODO:  If the New page has not been called using a ESC
				// sequence yet, do so now. Default Portrait or 
				// the default set by command line options

				//ExecEscCmd(101,cmdstr,ps,string,psfont,font_size,&textx,&texty,line_space);
					
				//fprintf(stderr, "font_set=[%d] NL Count=[%d]\n",myvars->font_set,myvars->nlcounter);
				
				//fprintf(stderr, "pagesizedef [%d] \n",myvars->pagesizedef);

				if((myvars->pgopen==0)&&(myvars->nlcounter==1)&&(strlen(myvars->string)==0)) { 
					// This should only apply for first lines with only Esc commands
					if(myvars->pagesizedef == 0) {
						ExecEscCmd(101,myvars);
					}else if(myvars->pagesizedef == 1) {
						ExecEscCmd(101,myvars);
					}else if(myvars->pagesizedef == 2) {
						ExecEscCmd(102,myvars);
					}else if(myvars->pagesizedef == 3) {
						ExecEscCmd(103,myvars);
					}else if(myvars->pagesizedef == 4) {
						ExecEscCmd(104,myvars);
					}else if(myvars->pagesizedef == 5) {
						ExecEscCmd(105,myvars);
					}else if(myvars->pagesizedef == 6) {
						ExecEscCmd(106,myvars);
					}else if(myvars->pagesizedef == 7) {
						ExecEscCmd(107,myvars);
					}else if(myvars->pagesizedef == 8) {
						ExecEscCmd(108,myvars);
					}
					
					if(myvars->printstyle == 1) {
						ExecEscCmd(001,myvars);
					} else if(myvars->printstyle == 2) {
						ExecEscCmd(002,myvars);
					}

					myvars->texty=(myvars->texty - myvars->line_space[0]);
					sprintf(myvars->string, "%c",0);
					
				}else if((myvars->pgopen==0)&&(myvars->nlcounter==1)&&(strlen(myvars->string)!=0)) { 
					// This should only apply for lines with only printable text
					if(myvars->pagesizedef == 0) {
						ExecEscCmd(101,myvars);
					}else if(myvars->pagesizedef == 1) {
						ExecEscCmd(101,myvars);
					}else if(myvars->pagesizedef == 2) {
						ExecEscCmd(102,myvars);
					}else if(myvars->pagesizedef == 3) {
						ExecEscCmd(103,myvars);
					}else if(myvars->pagesizedef == 4) {
						ExecEscCmd(104,myvars);
					}else if(myvars->pagesizedef == 5) {
						ExecEscCmd(105,myvars);
					}else if(myvars->pagesizedef == 6) {
						ExecEscCmd(106,myvars);
					}else if(myvars->pagesizedef == 7) {
						ExecEscCmd(107,myvars);
					}else if(myvars->pagesizedef == 8) {
						ExecEscCmd(108,myvars);
					}
					
					if(myvars->printstyle == 1) {
						ExecEscCmd(001,myvars);
					} else if(myvars->printstyle == 2) {
						ExecEscCmd(002,myvars);
					}
					
					ExecEscCmd(301,myvars);

					myvars->texty=(myvars->texty - myvars->line_space[0]);
					sprintf(myvars->string, "%c",0);
				}else if((myvars->pgopen==1)&&(myvars->nlcounter==1)&&(strlen(myvars->string)==0)) { 
					// This should only apply for lines with only printable text
				}else if((myvars->pgopen==1)&&(myvars->nlcounter==1)&&(strlen(myvars->string)!=0)) { 
					// This should only apply for lines with only printable text
					ExecEscCmd(301,myvars);

					myvars->texty=(myvars->texty - myvars->line_space[0]);
					sprintf(myvars->string, "%c",0);
				} else {
					ExecEscCmd(301,myvars);
					myvars->texty=(myvars->texty - myvars->line_space[0]);
					sprintf(myvars->string, "%c",0);
				}
					


				break;
			case 12:
				
				//fprintf(stderr, "Case: FF [%d] \n",n);
				//
				//Close Page
				myvars->formfeed=1;
				break;
			case 13:
				
				//fprintf(stderr, "Case: CR [%d] \n",n);
				break;
			case 27:
				
				//if((myvars->esc_cmd)&&(myvars->pgopen==0)&&(myvars->nlcounter==0)) {
				if((myvars->esc_cmd)&&(myvars->pgopen==0)) {

					ParsePrePageCmds(argc,argv,myvars);

					
					// Run exec cmd
					myvars->esc_cmd=0;
					//ExecEscCmd(myvars->rda_exec_cmd,myvars);
					myvars->rda_exec_cmd=0;
				}else if(myvars->esc_cmd) {
					//ParsePrePageCmds(argc,argv,myvars);
					// execute command string function
					// Close ESC Quote and exec function
					//myvars->rda_exec_cmd=111;
					//fprintf(stderr,"Execute #%d  CMDSTR[%s]\n",myvars->rda_exec_cmd,cmdstr);
//					fprintf(stderr,"NORMAL CMD [%d]  [%s]\n",myvars->rda_exec_cmd,myvars->cmdstr);
					myvars->esc_cmd=0;
					if(myvars->formfeed==1) {
						ParsePrePageCmds(argc,argv,myvars);
					}else{
						ExecEscCmd(myvars->rda_exec_cmd,myvars);
					}
					myvars->rda_exec_cmd=0;
				} else {
					//ParsePrePageCmds(argc,argv,myvars);
					// Begin ESC Quoted CMD 
					//ExecEscCmd() call here
					
					// Open CMD ESC

					memset(myvars->cmdstr,0,512);
					//sprintf(myvars->cmdstr, "%c",0);
					myvars->esc_cmd=1;
				}
				break;
			default: 
				
				//fprintf(stderr, "CaseU: [%d] [%c]\n",n,n);
				if(myvars->esc_cmd) {
					//fprintf(stderr,"PRE CMDSTR [%s]\n",myvars->cmdstr);
					sprintf(myvars->cmdstr, "%s%c",myvars->cmdstr,n);
					//fprintf(stderr,"POST CMDSTR [%s]\n",myvars->cmdstr);
				} else {
					
					if(myvars->formfeed==1) {
						ExecEscCmd(301,myvars);
						myvars->texty=(myvars->texty - myvars->line_space[0]);
						sprintf(myvars->string, "%c",0);

						myvars->formfeed=0;
						myvars->nlcounter=0;
						ExecEscCmd(110,myvars);
					}
					sprintf(myvars->string, "%s%c",(myvars->string!=NULL ? myvars->string:""),n);
				}

				if (myvars->esc_cmd && strlen(myvars->cmdstr)==3 && myvars->rda_exec_cmd==0) {

					//fprintf(stderr, "CMDSTR [%s] RDACMD: [%d]\n",myvars->cmdstr,myvars->rda_exec_cmd);
					myvars->rda_exec_cmd=atoi(myvars->cmdstr);

					sprintf(myvars->cmdstr, "%c",0);

					//fprintf(stderr, "CMDSTR [%s] RDACMD: [%d]\n",myvars->cmdstr,myvars->rda_exec_cmd);

				}

				break;

		}
		//if(bytes_read==50) { break;}

		return(0);

}
