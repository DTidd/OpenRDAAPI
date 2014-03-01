#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cerrno>
#include <boost/foreach.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <podofo/podofo.h>
#include <uuid/uuid.h>
#include <pwd.h>


#define __VER__ "Early Alpha"
//#define TRACE  std::cerr << "TRACE: " << __LINE__ << std::endl;
#define FTRACE BOOST_LOG_TRIVIAL(fatal) << "TRACE: " << __LINE__ << std::endl;
#define TTRACE BOOST_LOG_TRIVIAL(trace) << "TRACE: " << __LINE__ << std::endl;
#define TRACE BOOST_LOG_TRIVIAL(warning) << "TRACE: " << __LINE__ << std::endl;

//#ifdef USE_RDA_DIAGNOSTICS
#define LOG_TRACE BOOST_LOG_TRIVIAL(trace)  // debug = 6
#define LOG_DEBUG BOOST_LOG_TRIVIAL(debug)  // debug = 5
#define LOG_INFO BOOST_LOG_TRIVIAL(info)    // debug = 4
#define LOG_WARN BOOST_LOG_TRIVIAL(warning) // debug = 3
#define LOG_ERROR BOOST_LOG_TRIVIAL(error)  // debug = 2
#define LOG_FATAL BOOST_LOG_TRIVIAL(fatal)  // debug = 1
/*
#else
#define LOG_TRACE '//' 
#define LOG_DEBUG "//"
#define LOG_INFO //
#define LOG_WARN //
#define LOG_ERROR //
#define LOG_FATAL //
*/
//#endif /* USE_RDA_DIAGNOSTICS */

using namespace std;
using namespace PoDoFo;
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

int DebugEnabled=0;

class ViewPDF_Vars {
public:
	int psfont;
	char *string;
	char *cmdstr;
	int esc_cmd;
	int rda_exec_cmd;
	// RDA PS PRT VARIABLES 
	float textx;
	float texty;
	float lmargin;
	int *font_size;
	int *font_height;
	int formfeed; 
	int pagesizedef;
	int landscape;
	int psimage[10000];
	std::vector<PoDoFo::PdfImage> formimages;
	std::vector<PoDoFo::PdfXObject> pageimages;
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

	std::string input;
	std::string output;
	FILE *outputfp;

	int pushpopx;
	int pushpopy;
	int nlcounter;
	int pgcounter;
	int pgopen;
	int printstyle;

	std::string inputdata;

	// For C style read in
	FILE *fp;
	size_t bytes_read;
	char inputarray[65535];
	// For C style read in

	int prepagecount;
	int prepagecmd[150];
	char *prepagecmdstr[150];

	std::vector<int> vecprepagecmd;
	std::vector<std::string> vecprepagecmdstr;

	int viewfile;
	int removefile;
	int debug;

	ViewPDF_Vars() 
	{
		int x=0;
		psfont=0;
		string=NULL;
		cmdstr=NULL;
		string=new char[512];
		cmdstr=new char[512];
		memset(string,0,512);
		memset(cmdstr,0,512);
		esc_cmd=0;
		rda_exec_cmd=0;
		textx=0.0;
		texty=0.0;
		font_size=0;
		font_height=0;
		formfeed=0;
		pagesizedef=0;
		landscape=0;
		viewfile=1;
		removefile=1;
		for(x=0;x<10000;x++) { psimage[x]=0; }
		for(x=0;x<10000;x++) { pstmpl[x]=0; }
		rdapclborder=1;
		for(x=0;x<150;x++) { font_array[x]=0; }
		for(x=0;x<4;x++) { line_space[x]=0; }
		for(x=0;x<150;x++) { font_options[x]=0; }
		font_set=0;

		if(DebugEnabled==0)
		{
			logging::core::get()->set_filter( logging::trivial::severity >= logging::trivial::fatal );
			//logging::keywords::format = "[%TimeStamp%]: %Message%" ;
			DebugEnabled=1;
		}
		TRACE;
	
	}

	void ParseCmdLine(int argc, char *argv[]) 
	{
		int i;
		char *mytemp=NULL;
		uuid_t out;
		std::string argv_str;
	
		TRACE;
		for (i=1;i<argc;i++) 
		{
			// option -pt# is for page type
			// option -ps# is for print style
			LOG_DEBUG << "Arg [" << argv[i] << "]   Len [" << strlen(argv[i]) << "]" << std::endl;
			
			argv_str.clear();
			argv_str.assign(argv[i]);

			if((strncmp(argv[i],"-i",2))==0) {
				if(argv_str.size()==2) {
					++i;
					input.assign(argv[i]);
				}else{
					input.assign(argv_str.substr(2,argv_str.size()-2));
				}
	
			}else if((strncmp(argv[i],"-o",2))==0) {
				if(argv_str.size()==2) {
					++i;
					output.assign(argv[i]);
				}else{
					output.assign(argv_str.substr(2,argv_str.size()-2));
				}
	
			}else if((strncmp(argv[i],"-wp",2))==0) {
			}else if((strncmp(argv[i],"-d",2))==0) {
				if(argv_str.size()==2) {
					++i;
					argv_str.assign(argv[i]);
				}else{
					argv_str.assign(argv_str.substr(2,argv_str.size()-2));
				}
				mytemp=getenv("VIEWPDF_DEBUG");
				if(mytemp!=NULL) 
				{
					argv_str.assign(mytemp);
				}
				try
				{
					std::stringstream debugss(argv_str);
					if( (debugss >> debug).fail() )
					{
						debug=1;
					}
				}
				catch (std::exception& e)
				{
					debug=1;
				}
				switch(debug)
				{
					case 6:
						logging::core::get()->set_filter( logging::trivial::severity >= logging::trivial::trace );
						break;
					case 5:
						logging::core::get()->set_filter( logging::trivial::severity >= logging::trivial::debug );
						break;
					case 4:
						logging::core::get()->set_filter( logging::trivial::severity >= logging::trivial::info );
						break;
					case 3:
						logging::core::get()->set_filter( logging::trivial::severity >= logging::trivial::warning );
						break;
					case 2:
						logging::core::get()->set_filter( logging::trivial::severity >= logging::trivial::error );
						break;
					case 1:
					default:
						logging::core::get()->set_filter( logging::trivial::severity >= logging::trivial::fatal );
						debug=1;
						break;
				}
				mytemp==NULL;
				LOG_DEBUG << "Debug Level: " << debug << std::endl;

			}else if((strcmp(argv[i],"-pt1"))==0) {
				pagesizedef=1;
				LOG_DEBUG << "Arg Found [" << argv_str << "]" << std::endl;

			} else if((strcmp(argv[i],"-pt2"))==0) {
				pagesizedef=2;
				LOG_DEBUG << "Arg Found [" << argv_str << "]" << std::endl;

			} else if((strcmp(argv[i],"-pt3"))==0) {
				pagesizedef=3;
				LOG_DEBUG << "Arg Found [" << argv_str << "]" << std::endl;

			} else if((strcmp(argv[i],"-pt4"))==0) {
				pagesizedef=4;
				LOG_DEBUG << "Arg Found [" << argv_str << "]" << std::endl;

			} else if((strcmp(argv[i],"-pt5"))==0) {
				pagesizedef=5;
				LOG_DEBUG << "Arg Found [" << argv_str << "]" << std::endl;

			} else if((strcmp(argv[i],"-pt6"))==0) {
				pagesizedef=6;
				LOG_DEBUG << "Arg Found [" << argv_str << "]" << std::endl;

			} else if((strcmp(argv[i],"-pt7"))==0) {
				pagesizedef=7;
				LOG_DEBUG << "Arg Found [" << argv_str << "]" << std::endl;

			} else if((strcmp(argv[i],"-pt8"))==0) {
				pagesizedef=8;
				LOG_DEBUG << "Arg Found [" << argv_str << "]" << std::endl;

			} else if((strcmp(argv[i],"-ps1"))==0) {
				printstyle=1;
				LOG_DEBUG << "Arg Found [" << argv_str << "]" << std::endl;

			} else if((strcmp(argv[i],"-ps2"))==0) {
				printstyle=2;
				LOG_DEBUG << "Arg Found [" << argv_str << "]" << std::endl;

			} else if((strcmp(argv[i],"--dont-view"))==0) {
				viewfile=0;
				LOG_DEBUG << "Arg Found [" << argv_str << "]" << std::endl;

			} else if((strcmp(argv[i],"--dont-delete"))==0) {
				removefile=0;
				LOG_DEBUG << "Arg Found [" << argv_str << "]" << std::endl;

			} else if(((strcmp(argv[i],"-version"))==0)||((strcmp(argv[i],"--version"))==0)) {
				LOG_DEBUG << "ViewPDF built on [" << __VER__ << "]" << std::endl;
			}
		}
					
		mytemp=getenv("VIEWPDF_INPUT_FILENAME");
		if(mytemp!=NULL) { input.assign(mytemp); }
		mytemp==NULL;
	
		mytemp=getenv("VIEWPDF_OUTPUT_FILENAME");
		if(mytemp!=NULL) { output.assign(mytemp); }
		mytemp==NULL;
	
		mytemp=getenv("VIEWPDF_DONT_VIEW");
		if(mytemp!=NULL) 
		{
			if((strcmp(mytemp,"TRUE"))==0) 
			{
				viewfile=0;
			}
		}
		mytemp==NULL;
	
		mytemp=getenv("VIEWPDF_DONT_DELETE");
		if(mytemp!=NULL) 
		{
			if((strcmp(mytemp,"TRUE"))==0) 
			{
				removefile=0;
			}
		}
		mytemp==NULL;
	
		if((output.size()!=0)&&((strcmp(output.c_str(),"-"))==0))
		{
			viewfile=0;
			removefile=0;
		}

		if((strstr(argv[0],"viewpdf"))!=NULL && output.size()==0) 
		{
			LOG_DEBUG << "Program Type is ViewPDF." << std::endl;
	
			uuid_generate_time(out);
			
			mytemp=new char[100];;
			memset(mytemp,0,100);
			uuid_unparse_lower(out,mytemp);
			strcat(mytemp,".pdf");	
			output.assign(mytemp);
			delete[] mytemp;
			LOG_DEBUG << "Output file is [" << output << "]";
			
		}else{
			LOG_DEBUG << "Program Type is TXT2PDF." << std::endl;
		}
	} //ENDFUNC

private:
};










class ViewPDF_Document {
public:
	ViewPDF_Vars vars;
/*
    MyPodofoDocument(std::stringstream *pStringStream) : device(pStringStream), document(&device)
*/
	// EPdfVersion eVersion=ePdfVersion_Default, ePdfWriteMode_Compact,   ePdfWriteMode_Clean
	// PdfEncrypt *pEncrypt=NULL, 
	// EPdfWriteMode eWriteMode=ePdfWriteMode_Default, 
	//ViewPDF_Document(ViewPDF_Vars initvars, const char *pdf_filename) : vars(), document(pdf_filename, ePdfVersion_1_4, PoDoFo::PdfEncrypt *ECrypt=NULL, ePdfWriteMode_Clean)
	ViewPDF_Document(ViewPDF_Vars initvars, const char *pdf_filename) : vars(), document(pdf_filename, ePdfVersion_1_4, NULL, ePdfWriteMode_Clean)
	{
		TRACE;
		vars.input.clear();
		vars.input.assign(initvars.input);
		vars.output.clear();
		vars.output.assign(initvars.output);
		vars.debug=initvars.debug;
		vars.removefile=initvars.removefile;
		vars.viewfile=initvars.viewfile;
		vars.printstyle=initvars.printstyle;
		vars.pagesizedef=initvars.pagesizedef;
		//vars.=initvars.;
		TRACE;
	}

	void InitDoc()
	{
		char *mytemp=NULL;
		char *mystr5=NULL;
		std::string tempstr;
#ifndef WIN32
		struct passwd *passwd;
#endif

		mytemp=getenv("CYBER_PRODUCT");
		if(mytemp==NULL) 
		{ 
			tempstr.assign("UTILITIES");
		}else{
			tempstr.assign(mytemp);
		}
		
		mytemp=getenv("CYBER_PROCESS");
		if(mytemp==NULL) 
		{ 
			tempstr.append(" - TXT2PDF CREATE PDF");
		}else{
			tempstr.append(" - ");
			tempstr.append(mytemp);
		}
		
		mytemp=getenv("REPORT_TITLE");
		if(mytemp==NULL) 
		{ 
			tempstr.append(" --  ");
		}else{
			tempstr.append(" -- ");
			tempstr.append(mytemp);
		}
		
		document.GetInfo()->SetTitle   ( PdfString(tempstr) );
		tempstr.append("    ");
		
		
		mytemp=getenv("REPORT_DESCRIPTION");
		if(mytemp==NULL) 
		{ 
			document.GetInfo()->SetSubject ( PdfString(" ") );
		}else{
			document.GetInfo()->SetSubject ( PdfString(mytemp) );
			tempstr.append(mytemp);
		}

		document.GetInfo()->SetKeywords( PdfString(tempstr) );

		if((passwd=getpwuid(getuid()))==NULL) 
		{
			LOG_DEBUG <<"getpwuid failed.";
			document.GetInfo()->SetAuthor  ( PdfString("testuser") );
		}else{
			document.GetInfo()->SetAuthor  ( PdfString(passwd->pw_name) );
		}
		
#ifdef USING_QT
#else
#endif
		document.GetInfo()->SetCreator ( PdfString("OpenRDA (txt2pdf.lnx)") );
		document.GetInfo()->SetProducer ( PdfString("OpenRDA (PoDoFo 0.9.2)") );
		//document.GetInfo()->SetAuthor  ( PdfString("Robert Goley") );
		//document.GetInfo()->SetTitle   ( PdfString("Hello World") );
		//document.GetInfo()->SetSubject ( PdfString("Testing the PoDoFo PDF Library") );
		//document.GetInfo()->SetKeywords( PdfString("Test;PDF;Hello World;") );


		//Font_Arial = document.CreateFont( "Arial" );
		//if( !Font_Arial )
		//{
		//	PODOFO_RAISE_ERROR( ePdfError_InvalidHandle );
		//}

		//Font_Courier = document.CreateFont( "c0419bt_" );
		//Font_Courier = document.CreateFont( "Courier" );
		Font_Courier = document.CreateFont( "Courier",PdfEncodingFactory::GlobalWinAnsiEncodingInstance(),PdfFontCache::eFontCreationFlags_AutoSelectBase14,1 );
		if( !Font_Courier )
		{
			PODOFO_RAISE_ERROR( ePdfError_InvalidHandle );
		}

	}

	std::string get_whole_file_contents(const char *filename)
	{
		// Read actual whole file into memory.
		// Good for small files or known fixed ones like forms.
		std::ifstream in(filename, std::ios::in | std::ios::binary);
		if (in)
		{
			std::string contents;
			in.seekg(0, std::ios::end);
			contents.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();
			//LOG_DEBUG << "Contents contains: [" << contents <<"]";
			return(contents);
		}
		LOG_ERROR << "Error opening input file [" << filename <<"]";
		throw(errno);
	}

	void ReadParseFromSTDIN()
	{	
		int x=0,y=0;
		int readsize=0,bytes_read=0;
		int n=0;

		vars.fp=stdin;
		while(!feof(vars.fp)) {
			memset(vars.inputarray,0,65535);
	        	bytes_read=fread(vars.inputarray,1,65534,vars.fp);

			if((vars.fp==stdin)&&(bytes_read==0)) {
				//fprintf(stderr,"bytes read [%d] strlen [%d]  y [%d]\n",readsize,(strlen(inputarray)),y);
				y++;
				if(y>2000000) { break; }
			}else if(vars.fp==stdin) {
				y=0;
			}

			for(x=0;x<bytes_read;x++) {
				n=vars.inputarray[x];	
				ParseInput(n);
			}
			readsize=(bytes_read+readsize);
			//fprintf(stderr,"bytes read [%d] str [%d] strlen [%d]\n",readsize,n,(strlen(str)));
		}
		fclose(vars.fp);
	}

	void ReadInputFile(const char *filename)
	{
		vars.inputdata=get_whole_file_contents(filename);
		//LOG_DEBUG << "inputdata contains: [" << vars.inputdata <<"]";
	}

	void SetPrintStyle(int stylecode)
	{
		switch (stylecode) 
		{
			case 1:
			//case 001:
				Font_Courier->SetFontSize( 12.0 );
				Font_Courier->SetFontCharSpace(0.0);
				vars.font_array[0]=vars.font_array[1];
				vars.line_space[0]=12;
				vars.font_set=1;
				vars.printstyle=1;
				//DefaultFont=Font_Courier;
				painter.SetFont(Font_Courier);
				break;
			case 2:
			//case 002:
				Font_Courier->SetFontSize( 7.0 );
				Font_Courier->SetFontCharSpace(0.11);
				//vars.font_array[0]=vars.font_array[1];
				vars.line_space[0]=9;
				vars.font_set=1;
				vars.printstyle=2;
				//DefaultFont=Font_Courier;
				painter.SetFont(Font_Courier);
				break;

			case 0:
			default:
				break;
		}
		/*
		if(pgopen)
		{
		
		}
		*/

/*
					if(vars.printstyle == 1) {
						ExecEscCmd(001);
					} else if(vars.printstyle == 2) {
						ExecEscCmd(002);
					}
*/
	}
	void MyNewPage(int pagecode)
	{
		LOG_DEBUG << "MyNewPage Called";
		switch (pagecode) 
		{
			case 101:
				vars.landscape=0;
				vars.pagesizedef=1;
				break;
			case 102:
				vars.landscape=1;
				vars.pagesizedef=2;
				break;
			case 103:
				vars.landscape=0;
				vars.pagesizedef=3;
				break;
			case 104:
				vars.landscape=1;
				vars.pagesizedef=4;
				break;
			case 105:
				vars.landscape=0;
				vars.pagesizedef=5;
				break;
			case 106:
				vars.landscape=1;
				vars.pagesizedef=6;
				break;
			case 107:
				vars.landscape=0;
				vars.pagesizedef=7;
				break;
			case 108:
				vars.landscape=1;
				vars.pagesizedef=8;
				break;
			case 0:
			default:
				break;
		}

		switch (vars.pagesizedef) 
		{
			case 0:
			case 1:
				//NEW_PAGE_8.5x11_PORTRAIT  (LETTER)
				// Page Left: 0
				// Page Width: 612
				// Page Height: 792
				// Page Bottom: 0
		
				vars.pagewidth=612;
				vars.pageheight=792;
				
				if(vars.rdapclborder) {
					 vars.textx = 18.0; 
					 vars.texty = 748.0; 
					 vars.lmargin = 18.0; 
				} else {
					 vars.textx = 18.0; 
					 vars.texty = 765.0; 
				}

				pPage = document.CreatePage( PdfPage::CreateStandardPageSize( ePdfPageSize_Letter, vars.landscape)  );
				if( !pPage ) 
				{
					PODOFO_RAISE_ERROR( ePdfError_InvalidHandle );
				}
				break;

			case 2:
				//NEW_PAGE_8.5x11_LANDSCAPE  (LETTER)
				vars.pagewidth=792;
				vars.pageheight=612;
				if(vars.rdapclborder) {
					 vars.textx = 18.0; 
					 vars.texty = 568.0; 
					 vars.lmargin = 18.0; 
				} else {
					 vars.textx = 18.0; 
					 vars.texty = 585.0; 
				}

				pPage = document.CreatePage( PdfPage::CreateStandardPageSize( ePdfPageSize_Letter, vars.landscape)  );
				if( !pPage ) 
				{
					PODOFO_RAISE_ERROR( ePdfError_InvalidHandle );
				}
				break;

			case 3:
				// NEW_PAGE_8.5x14_PORTRAIT  (LEGAL)
				vars.pagewidth=612;
				vars.pageheight=1008;
				if(vars.rdapclborder) {
					 vars.textx = 18.0; 
					 vars.texty = 964.0; 
					 vars.lmargin = 18.0; 
				} else {
					 vars.textx = 18.0; 
					 vars.texty = 981.0; 
				}
	
				pPage = document.CreatePage( PdfRect(0,0,vars.pagewidth,vars.pageheight) );
				if( !pPage ) 
				{
					PODOFO_RAISE_ERROR( ePdfError_InvalidHandle );
				}
				break;
			case 4:
				//NEW_PAGE_8.5x14_LANDSCAPE  (LEGAL)
				vars.pagewidth=1008;
				vars.pageheight=612;
				if(vars.rdapclborder) {
					 vars.textx = 18.0; 
					 vars.texty = 568.0; 
					 vars.lmargin = 18.0; 
				} else {
					 vars.textx = 18.0; 
					 vars.texty = 585.0; 
				}
				pPage = document.CreatePage( PdfRect(0,0,vars.pagewidth,vars.pageheight) );
				if( !pPage ) 
				{
					PODOFO_RAISE_ERROR( ePdfError_InvalidHandle );
				}
				break;

			case 5:
				//NEW_PAGE_11x14_PORTRAIT  (POSTER)
				vars.pagewidth=792;
				vars.pageheight=1008;
				if(vars.rdapclborder) {
					 vars.textx = 18.0; 
					 vars.texty = 964.0; 
					 vars.lmargin = 18.0; 
				} else {
					 vars.textx = 18.0; 
					 vars.texty = 981.0; 
				}
				pPage = document.CreatePage( PdfRect(0,0,vars.pagewidth,vars.pageheight) );
				if( !pPage ) 
				{
					PODOFO_RAISE_ERROR( ePdfError_InvalidHandle );
				}
				break;

			case 6:
				//NEW_PAGE_11x14_LANDSCAPE  (POSTER)
				vars.pagewidth=1008;
				vars.pageheight=792;
				if(vars.rdapclborder) {
					 vars.textx = 18.0; 
					 vars.texty = 748.0; 
					 vars.lmargin = 18.0; 
				} else {
					 vars.textx = 18.0; 
					 vars.texty = 765.0; 
				}
				pPage = document.CreatePage( PdfRect(0,0,vars.pagewidth,vars.pageheight) );
				if( !pPage ) 
				{
					PODOFO_RAISE_ERROR( ePdfError_InvalidHandle );
				}
				break;

			case 7:
				//NEW_PAGE_11x17_PORTRAIT  (LEDGER)
				vars.pagewidth=792;
				vars.pageheight=1224;
				if(vars.rdapclborder) {
					 vars.textx = 18.0; 
					 vars.texty = 1180.0; 
					 vars.lmargin = 18.0; 
				} else {
					 vars.textx = 18.0; 
					 vars.texty = 1197.0; 
				}
				pPage = document.CreatePage( PdfRect(0,0,vars.pagewidth,vars.pageheight) );
				if( !pPage ) 
				{
					PODOFO_RAISE_ERROR( ePdfError_InvalidHandle );
				}
				break;

			case 8:
				//NEW_PAGE_11x17_LANDSCAPE  (LEDGER)
				vars.pagewidth=1224;
				vars.pageheight=792;
				if(vars.rdapclborder) {
					 vars.textx = 18.0; 
					 vars.texty = 748.0; 
					 vars.lmargin = 18.0; 
				} else {
					 vars.textx = 18.0; 
					 vars.texty = 765.0; 
				}
				pPage = document.CreatePage( PdfRect(0,0,vars.pagewidth,vars.pageheight) );
				if( !pPage ) 
				{
					PODOFO_RAISE_ERROR( ePdfError_InvalidHandle );
				}
				break;

			default:
				break;

		}
		vars.pgopen=1;
		vars.pgcounter++;
		painter.SetPage( pPage );

		LOG_DEBUG << "MyNewPage Exited";

/*
		if(vars.pagesizedef == 0) {
			ExecEscCmd(101);
		}else if(vars.pagesizedef == 1) {
			ExecEscCmd(101);
		}else if(vars.pagesizedef == 2) {
			ExecEscCmd(102);
		}else if(vars.pagesizedef == 3) {
			ExecEscCmd(103);
		}else if(vars.pagesizedef == 4) {
			ExecEscCmd(104);
		}else if(vars.pagesizedef == 5) {
			ExecEscCmd(105);
		}else if(vars.pagesizedef == 6) {
			ExecEscCmd(106);
		}else if(vars.pagesizedef == 7) {
			ExecEscCmd(107);
		}else if(vars.pagesizedef == 8) {
			ExecEscCmd(108);
		}
*/					

	}

	void ParsePrePageCmds()
	{
		//LOG_INFO << "PROCESS PAGE [" << vars->pgcounter <<"] COUNT [" << myvars->prepagecount <<"]";
		if((vars.pgopen==0)||(vars.formfeed==1)) 
		{
			// Just need a switch case statement here for cmds to process before page is opened.
			// Could likely use return code from this command to decide whether to execute
			// command in the calling function.  Make things simple...
			
			switch (vars.rda_exec_cmd) {
			case 0:
			case 315:
			case 316:
			case 318:
			case 319:
			case 320:
			case 321:
			case 330:
			case 331:
			
				LOG_DEBUG << "RUN PRE PAGE CMD [" << vars.rda_exec_cmd << "]  [" << vars.cmdstr << "]"; 
				ExecEscCmd(vars.rda_exec_cmd);
				//return(1);
				break;
			default: 
				vars.prepagecount++;
				LOG_DEBUG << "STORE PRE PAGE COUNT [" << vars.prepagecount << "] CMD [" << vars.rda_exec_cmd << "]  [" << vars.cmdstr << "]";
				
				vars.vecprepagecmd.push_back(vars.rda_exec_cmd);
				vars.vecprepagecmdstr.push_back(vars.cmdstr);

				//return(0);
				break;
			}
			
/*
		} else if (myvars.pgopen==1) {
			LOG_INFO << "POST PAGE CMD [" << vars.rda_exec_cmd << "]  [" << vars.cmdstr <<"]";
		}else{  
			LOG_INFO << "WIERD PAGE CMD [" << vars.rda_exec_cmd << "]  [" << vars.cmdstr <<"]";
*/	
		}
		return;
	}

	int ProcessPrePageCmds()
	{
		int i=0;
		char *tempstr;

		//LOG_INFO << "PROCESS PAGE [" << vars.pgcounter << "] COUNT [" << vars.prepagecount << "] PGOPEN [" << vars.pgopen << "] FF [" << vars.formfeed << "]";
		while(1)
		{
			if((vars.prepagecmd[(i)]!=0)) 
			{
				LOG_DEBUG << "PROCESS PRE PAGE CMD [" << vars.prepagecmd[(i)] << "] [" << vars.prepagecmdstr[(i)] << "]";
	
				strcpy(vars.cmdstr,vars.vecprepagecmdstr[1].c_str());
				ExecEscCmd(vars.vecprepagecmd[i]);
		/*
			}else{
				LOG_INFO << "PROCESS ZERO OR NULL CMD";
		*/
			}
		
			if(vars.prepagecount==0) { break; }
			if(i==vars.prepagecount) { break; }
			i++;
		}
		vars.prepagecount=0;
		vars.vecprepagecmd.clear();
		vars.vecprepagecmdstr.clear();
			
		return(0);
	}


	void NewPage()
	{
		int x=0;
		//ePdfPageSize_A0  DIN A0
		//ePdfPageSize_A1  DIN A1
		//ePdfPageSize_A2  DIN A2
		//ePdfPageSize_A3  DIN A3
		//ePdfPageSize_A4  DIN A4
		//ePdfPageSize_A5  DIN A5
		//ePdfPageSize_A6  DIN A6
		//ePdfPageSize_Letter  Letter
		//ePdfPageSize_Legal  Legal
		//ePdfPageSize_Tabloid  Tabloid

		// Page Left: 0
		// Page Bottom: 0
		// Page Width: 612
		// Page Height: 792
	
		//pPage = document.CreatePage( PdfPage::CreateStandardPageSize( ePdfPageSize_Letter ) );
		//pPage = document.CreatePage( PdfRect(0,0,612,792) );
		//if( !pPage ) 
		//{
		//	PODOFO_RAISE_ERROR( ePdfError_InvalidHandle );
		//}

		//LOG_FATAL << "Page Left: " << pPage->GetPageSize().GetLeft();
		//LOG_FATAL << "Page Width: " << pPage->GetPageSize().GetWidth();
		//LOG_FATAL << "Page Height: " << pPage->GetPageSize().GetHeight();
		//LOG_FATAL << "Page Bottom: " << pPage->GetPageSize().GetBottom();

		//painter.SetPage( pPage );

/*
		pFont = document.CreateFont("Arial");
		if( !pFont )
		{
			PODOFO_RAISE_ERROR( ePdfError_InvalidHandle );
		}
		pFont->SetFontSize( 18.0 );
		painter.SetFont( pFont );

		Font_Courier->SetFontSize( 12.0 );
		Font_Courier->SetFontCharSpace(0.0);
		vars.line_space[0]=12;

		painter.SetFont(Font_Courier);
*/

/*
			PS_setfont(myvars->ps,myvars->font_array[1],12.0);
			PS_set_value(myvars->ps,"charspacing",0.0);
			myvars->font_array[0]=myvars->font_array[1];
			PS_setfont(myvars->ps,myvars->font_array[1],7.0);
			PS_set_value(myvars->ps,"charspacing",0.11);
			myvars.line_space[0]=9;
*/
		//SAFE_OP( painter.SetColor( 1.0, 0.0, 0.0 ) );

		//painter.SetFont(Font_Courier);

		sprintf(vars.string,"Hello World!  This is a sample string of text to be used in testing output of the PoDoFo Library.");
		for(x=0;x<60;x++)
		{
			ExecEscCmd(001);
			ExecEscCmd(301);
			vars.texty=(vars.texty - vars.line_space[0]);
			//sprintf(vars.string, "%c",0);
			vars.textx=vars.lmargin;

			ExecEscCmd(002);
			ExecEscCmd(301);
			vars.texty=(vars.texty - vars.line_space[0]);
			//sprintf(vars.string, "%c",0);
			vars.textx=vars.lmargin;

		}
	
		painter.FinishPage();
	}


	int ParseInput(int n)
	{
		switch (n) 
		{
			case 10:
				LOG_DEBUG << "NL [" << vars.nlcounter << "]   pagesizedef [" << vars.pagesizedef  << "]   landscape [" << vars.landscape << "]";
				if(vars.formfeed==1) {
					vars.formfeed=0;
					vars.nlcounter=0;
					ExecEscCmd(110);
				}
				//fprintf(stderr,"BEFORE NL X[%5.2f] Y[%5.2f]\n",vars.textx,vars.texty);
				vars.nlcounter++;
				LOG_DEBUG << "Case: NL [" << n << "]";

				if((vars.pgopen==1)&&(vars.nlcounter==1)&&(strlen(vars.string)!=0)) { 
					LOG_DEBUG << "pgopen=1  nlcounter=1  vars.string!=0";
					// This should only apply for lines with only printable text
					ExecEscCmd(301);

					vars.texty=(vars.texty - vars.line_space[0]);
					sprintf(vars.string, "%c",0);
					vars.textx=vars.lmargin;
				}else if((vars.pgopen==0)&&(vars.nlcounter==1)&&(strlen(vars.string)==0)) { 
					LOG_DEBUG << "pgopen=0  nlcounter=1  vars.string==0";
					// This should only apply for first lines with only Esc commands
					if(vars.pagesizedef == 0) {
						ExecEscCmd(101);
					}else if(vars.pagesizedef == 1) {
						ExecEscCmd(101);
					}else if(vars.pagesizedef == 2) {
						ExecEscCmd(102);
					}else if(vars.pagesizedef == 3) {
						ExecEscCmd(103);
					}else if(vars.pagesizedef == 4) {
						ExecEscCmd(104);
					}else if(vars.pagesizedef == 5) {
						ExecEscCmd(105);
					}else if(vars.pagesizedef == 6) {
						ExecEscCmd(106);
					}else if(vars.pagesizedef == 7) {
						ExecEscCmd(107);
					}else if(vars.pagesizedef == 8) {
						ExecEscCmd(108);
					}
					
					if(vars.printstyle == 1) {
						ExecEscCmd(001);
					} else if(vars.printstyle == 2) {
						ExecEscCmd(002);
					}

					vars.texty=(vars.texty - vars.line_space[0]);
					sprintf(vars.string, "%c",0);
					
				}else if((vars.pgopen==0)&&(vars.nlcounter==1)&&(strlen(vars.string)!=0)) { 
					LOG_DEBUG << "pgopen=0  nlcounter=1  vars.string!=0";
					// This should only apply for lines with only printable text
					if(vars.pagesizedef == 0) {
						ExecEscCmd(101);
					}else if(vars.pagesizedef == 1) {
						ExecEscCmd(101);
					}else if(vars.pagesizedef == 2) {
						ExecEscCmd(102);
					}else if(vars.pagesizedef == 3) {
						ExecEscCmd(103);
					}else if(vars.pagesizedef == 4) {
						ExecEscCmd(104);
					}else if(vars.pagesizedef == 5) {
						ExecEscCmd(105);
					}else if(vars.pagesizedef == 6) {
						ExecEscCmd(106);
					}else if(vars.pagesizedef == 7) {
						ExecEscCmd(107);
					}else if(vars.pagesizedef == 8) {
						ExecEscCmd(108);
					}
					
					if(vars.printstyle == 1) {
						ExecEscCmd(001);
					} else if(vars.printstyle == 2) {
						ExecEscCmd(002);
					}
					
					ExecEscCmd(301);

					vars.texty=(vars.texty - vars.line_space[0]);
					sprintf(vars.string, "%c",0);
					vars.textx=vars.lmargin;
				}else if((vars.pgopen==1)&&(vars.nlcounter==1)&&(strlen(vars.string)==0)) { 
					LOG_DEBUG << "pgopen=0  nlcounter=1  vars.string==0";
					// This should only apply for lines with only printable text
					vars.textx=vars.lmargin;
				} else {
					LOG_DEBUG << "NL ELSE COND";
					ExecEscCmd(301);
					vars.texty=(vars.texty - vars.line_space[0]);
					sprintf(vars.string, "%c",0);
					vars.textx=vars.lmargin;
				}
				break;

			case 12:
				//Close Page
				LOG_DEBUG << "Case: FF [" << n << "]";
				vars.formfeed=1;
				break;

			case 13:
				LOG_DEBUG << "Case: CR [" << n << "]";
				break;

			case 27:
				LOG_DEBUG << "Case: ESC ";
				//if((vars.esc_cmd)&&(vars.pgopen==0)&&(vars.nlcounter==0)) 
				if((vars.esc_cmd)&&(vars.pgopen==0)) {
					// Set page type and font type
					// Page type should be first command or this will need to be edited.

					ParsePrePageCmds();

					// Run exec cmd
					vars.esc_cmd=0;
					//ExecEscCmd(vars.rda_exec_cmd);
					vars.rda_exec_cmd=0;
				}else if(vars.esc_cmd) {
					//ParsePrePageCmds(argc,argv);
					// execute command string function
					// Close ESC Quote and exec function
					//vars.rda_exec_cmd=111;
					//fprintf(stderr,"Execute #%d  CMDSTR[%s]\n",vars.rda_exec_cmd,cmdstr);
//					fprintf(stderr,"NORMAL CMD [%d]  [%s]\n",vars.rda_exec_cmd,vars.cmdstr);
					vars.esc_cmd=0;
					if(vars.formfeed==1) {
						ParsePrePageCmds();
					}else{
						ExecEscCmd(vars.rda_exec_cmd);
					}
					vars.rda_exec_cmd=0;
				} else {
					//ParsePrePageCmds(argc,argv);
					// Begin ESC Quoted CMD 
					//ExecEscCmd() call here
					
					// Open CMD ESC

					memset(vars.cmdstr,0,512);
					//sprintf(vars.cmdstr, "%c",0);
					vars.esc_cmd=1;
				}
				break;
			default: 
				
				//fprintf(stderr, "CaseU: [%d] [%c]\n",n,n);
				if(vars.esc_cmd) {
					//fprintf(stderr,"PRE CMDSTR [%s]\n",vars.cmdstr);
					sprintf(vars.cmdstr, "%s%c",vars.cmdstr,n);
					//fprintf(stderr,"POST CMDSTR [%s]\n",vars.cmdstr);
				} else {
					
					if(vars.formfeed==1) {
						ExecEscCmd(301);
						vars.texty=(vars.texty - vars.line_space[0]);
						sprintf(vars.string, "%c",0);

						vars.formfeed=0;
						vars.nlcounter=0;
						ExecEscCmd(110);
					}
					sprintf(vars.string, "%s%c",(vars.string!=NULL ? vars.string:""),n);
				}

				if (vars.esc_cmd && strlen(vars.cmdstr)==3 && vars.rda_exec_cmd==0) {

					//fprintf(stderr, "CMDSTR [%s] RDACMD: [%d]\n",vars.cmdstr,vars.rda_exec_cmd);
					vars.rda_exec_cmd=atoi(vars.cmdstr);

					sprintf(vars.cmdstr, "%c",0);

					//fprintf(stderr, "CMDSTR [%s] RDACMD: [%d]\n",vars.cmdstr,vars.rda_exec_cmd);

				}

				break;

		}
		//if(bytes_read==50) { break;}
		return(0);
	}

	int ParseInputLoop()
	{
		LOG_DEBUG << "Entering ParseInputLoop()";

		
		BOOST_FOREACH(int num,vars.inputdata)
		{
			//LOG_FATAL << "Entering FOREACH LOOP()   num=[" << num << "]";
			ParseInput(num);
		}
	}

	void LoadFormPage()
	{
/*
		FTRACE;
		PoDoFo::PdfVecObjects vecObjects;
		FTRACE;
		PoDoFo::PdfParser parser( &vecObjects );
		FTRACE;
		parser.ParseFile( "/tmp/form_3010.pdf", true );
		FTRACE;
		PoDoFo::PdfImage MyImage(parser.GetObjects());
		FTRACE;
*/
		if(vars.pageimages.size()<1)
		{

			PoDoFo::PdfMemDocument formdoc("/tmp/form_3010.pdf");
			//PdfXObject (const PdfMemDocument &rSourceDoc, int nPage, PdfDocument *pParent, const char *pszPrefix=NULL, bool bUseTrimBox=false);
			//PoDoFo::PdfXObject mypobj( &formdoc, 1, document, NULL, false);
			PoDoFo::PdfXObject mypobj( formdoc, 0, &document);
	
			vars.pageimages.push_back(mypobj);
			//painter.DrawXObject(0,0,&vars.pageimages[0],1,1);
			
			//vars.pageimages.insert(3010,mypobj);
			vars.psimage[3010]=vars.pageimages.size();
			//vars.pageimages[3010]=myobj;
			painter.DrawXObject(0,0,&vars.pageimages[vars.psimage[3010]],1,1);
		}else{
			painter.DrawXObject(0,0,&vars.pageimages[vars.psimage[3010]],1,1);
			//painter.DrawXObject(0,0,&vars.pageimages[3010],1,1);
		}

		//const PdfVecObjects pVecObjects;
		//FTRACE;
		//PdfParser (&pVecObjects, "/tmp/form_3010.pdf", true);
		//FTRACE;
		//PoDoFo::PdfImage MyImage(pVecObjects);

		//PoDoFo::PdfImage MyImage;

		//MyImage.LoadFromFile("/tmp/form_3010.pdf");
		//painter.DrawImage( 70.0, document.GetPage(0)->GetPageSize().GetHeight() - MyImage.GetHeight(), &MyImage );		
		
		//const PoDoFo::PdfMemDocument formdoc("/tmp/form_3010.pdf");
		//const PoDoFo::PdfMemDocument formdoc("/tmp/form_3010.pdf");
		//PdfXObject( const PdfMemDocument & rSourceDoc, int nPage, PdfDocument* pParent, const char* pszPrefix = NULL, bool bUseTrimBox = false );
	
		//PoDoFo::PdfXObject tempxobj(formdoc, 0, document,NULL,false);
	}

	void CloseDoc()
	{
		if(vars.pgopen==1)
		{
			painter.FinishPage();
		}
		document.Close();
	}

void ExecEscCmd(int rda_exec_cmd) 
{
	
	char *mystr1=NULL,*mystr2=NULL,*mystr3=NULL,*mystr4=NULL,*mystr5=NULL;
	float myfloat;
	char temparray[65535];
	size_t size=0;
#ifndef WIN32
	struct passwd *passwd;
#endif

	myfloat=0.0;

	LOG_DEBUG << "page: [" << &pPage << "]   " << " painter [" << &painter << "]   font " << &Font_Courier << "]";
	LOG_DEBUG << "EscCmd: [" << rda_exec_cmd << "]   " << "NL [" << vars.nlcounter << "]";
	LOG_DEBUG << "EscCmd: [" << rda_exec_cmd << "]";

	switch (rda_exec_cmd) 
	{
	case 001:
			//NORMAL
			//fprintf(stderr,"CMD001 [%d] STR[%s]\n",vars.rda_exec_cmd,cmdstr);
			//printf("font1=%d\n",vars.font_array[1]);
			
			SetPrintStyle(1);			
			break;
	case 002:
			//CONDENSED
			//fprintf(stderr,"CMD002 [%d] STR[%s]\n",vars.rda_exec_cmd,cmdstr);

			SetPrintStyle(2);			
			break;
	case 050:
			// Preload/Embedd all commonly used fonts.
			// Should be done in object init using PoDoFo
			break;
	case 051:
			// Multi option command to load specific fonts
			// to be used to load less used fonts.
		/*
			switch(atoi(vars.cmdstr)) {
				case 100:
					vars.font_array[100] = PS_findfont(vars.ps,"VeraMono","",1);
					vars.font_options[100]=0;
					if(vars.font_array[100]==0) { fprintf(stderr,"Could not load font VeraMono.\n"); }
					break;
				case 101:
					vars.font_array[101] = PS_findfont(vars.ps,"c0419bt_","",1);
					vars.font_options[101]=0;
					if(vars.font_array[101]==0) { fprintf(stderr,"Could not load font Courier.\n"); }
					break;
			}
		*/
			break;
	case 003:
			fprintf(stderr,"Unknown Command 003.\n");
			break;
	case 80:
	case 81:
			// Should be managed in object init in PoDoFo
			break;
	case 90:
			//  Handle pre page start issues with Esc Commands consistently.
			//  This option was replaced by the Parse and Process pre page cmd functions.

			// Should be redone to call a NewPage() function of the MyDoc object.

				if(vars.pagesizedef == 0) {
					ExecEscCmd(101);
				}else if(vars.pagesizedef == 1) {
					ExecEscCmd(101);
				}else if(vars.pagesizedef == 2) {
					ExecEscCmd(102);
				}else if(vars.pagesizedef == 3) {
					ExecEscCmd(103);
				}else if(vars.pagesizedef == 4) {
					ExecEscCmd(104);
				}else if(vars.pagesizedef == 5) {
					ExecEscCmd(105);
				}else if(vars.pagesizedef == 6) {
					ExecEscCmd(106);
				}else if(vars.pagesizedef == 7) {
					ExecEscCmd(107);
				}else if(vars.pagesizedef == 8) {
					ExecEscCmd(108);
				}
				
				if(vars.printstyle == 1) {
					ExecEscCmd(001);
				} else if(vars.printstyle == 2) {
					ExecEscCmd(002);
				}


			break;
	case 101:
			//NEW_PAGE_8.5x11_PORTRAIT  (LETTER)
			if(vars.pgopen) { ExecEscCmd(110); }	
		
			//CreateStandardPageSize (const EPdfPageSize ePageSize, bool bLandscape=false)
			//
			//New Vars for this...?
			//
			// EPdfPageSize vars.PageSize=etc
			// vars.landscape=1;
			//

			MyNewPage(101);
			SetPrintStyle(vars.printstyle);
			ProcessPrePageCmds();
			break;
	case 102:
			//NEW_PAGE_8.5x11_PORTRAIT  (LETTER)
			//NEW_PAGE_8.5x11_LANDSCAPE  (LETTER)
			
			//PS_begin_page(vars.ps,792,612);
			
			if(vars.pgopen) { ExecEscCmd(110); }	
		
			MyNewPage(102);
			SetPrintStyle(vars.printstyle);
			ProcessPrePageCmds();
			break;
	case 103:
			//NEW_PAGE_8.5x14_PORTRAIT  (LEGAL)
			//PS_begin_page(vars.ps,612,1008);
			
			if(vars.pgopen) { ExecEscCmd(110); }	
		
			MyNewPage(103);
			SetPrintStyle(vars.printstyle);
			
			ProcessPrePageCmds();
			break;
	case 104:
			//NEW_PAGE_8.5x14_LANDSCAPE  (LEGAL)
			
			//PS_begin_page(vars.ps,1008,612);
			
			if(vars.pgopen) { ExecEscCmd(110); }	

			MyNewPage(104);
			SetPrintStyle(vars.printstyle);

			ProcessPrePageCmds();
			break;
	case 105:
			//NEW_PAGE_11x14_PORTRAIT  (POSTER)
			//PS_begin_page(vars.ps,792,1008);
			
			if(vars.pgopen) { ExecEscCmd(110); }	

			MyNewPage(105);
			SetPrintStyle(vars.printstyle);

			ProcessPrePageCmds();
			break;
	case 106:
			//NEW_PAGE_11x14_LANDSCAPE  (POSTER)
			//PS_begin_page(vars.ps,1008,792);
			
			if(vars.pgopen) { ExecEscCmd(110); }	

			MyNewPage(106);
			SetPrintStyle(vars.printstyle);

			ProcessPrePageCmds();
			break;
	case 107:
			//NEW_PAGE_11x17_PORTRAIT  (LEDGER)
			//PS_begin_page(vars.ps,792,1224);
			
			if(vars.pgopen) { ExecEscCmd(110); }	

			MyNewPage(107);
			SetPrintStyle(vars.printstyle);

			ProcessPrePageCmds();
			break;
	case 108:
			//NEW_PAGE_11x17_LANDSCAPE  (LEDGER)
			//PS_begin_page(vars.ps,1224,792);
			
			if(vars.pgopen) { ExecEscCmd(110); }	

			MyNewPage(108);
			SetPrintStyle(vars.printstyle);

			ProcessPrePageCmds();
			break;
	case 110:
			LOG_DEBUG << "Function 110 Called";
			//PS_ENDPAGE
			//PS_end_page(vars.ps);
			
			// Should call ClosePage() function of MyDoc object
			
			LoadFormPage();
			painter.FinishPage();
			//ExecEscCmd(90); 	
			vars.pgopen=0;
			break;
	case 111:
			// Handled by object init in PoDoFo

			//vars.ps = PS_new();

			break;
	case 112:
			//PS_ENDFILE
			//PS_close(vars.ps);
			break;
	case 113:
			//PS_ENDFILE
			//PS_delete(vars.ps);
			break;
	case 115:
			//SET_FONT
//			vars.font_array[12] = PS_findfont(vars.ps, "Helvetica", "", 0);
//			vars.font_array[0] = vars.font_array[12];
			break;
	case 120:
			//SET_FONTSIZE
//			PS_setfont(vars.ps,vars.font_array[10],12.0);
//			vars.font_array[11] = vars.font_array[0];
//			vars.font_array[0] = vars.font_array[10];
			break;
	case 121:
			// This function will print the current 
			// string and compute the length of that
			// string and increment textx.  It will
			// then change the default font in the array 
			// and send the pslib command to do the same.
			// It will need to clear the string variable.

			//PS_show_xy(ps,string,*textx_pt,*texty_pt);
//			vars.font_array[10] = PS_findfont(vars.ps,"MRVCode39extSA","",1);
//			PS_setfont(vars.ps,vars.font_array[10],12.0);
//			vars.font_array[11] = vars.font_array[0];
//			vars.font_array[0] = vars.font_array[10];
			
			break;
	case 200:
			//IMP_PS_IMAGE
//			vars.psimage[0] = PS_open_image_file(vars.ps,"png","letter.ps", NULL, 0);
			break;
	case 201:
			//IMP_PS_IMAGE
//			vars.psimage[0] = PS_open_image_file(vars.ps,"jpg","letter.ps", NULL, 0);
			break;
	case 202:
			//IMP_PS_IMAGE
//			vars.psimage[0] = PS_open_image_file(vars.ps,"eps",vars.cmdstr, NULL, 0);
			break;
	case 220:
			//Create a page template using an image to 
			//create smaller more efficient output
			//using known psimage id.  PORTRAIT
			//pstmpl=PS_begin_template(ps,"float width","float height")
			//vars.pstmpl=PS_begin_template(vars.ps,596,842);
//			vars.pstmpl[0]=PS_begin_template(vars.ps,596,842);
//			PS_place_image(vars.ps, vars.psimage[0], 0, 0, 1.0);
//			PS_end_template(vars.ps);
			break;
	case 221:
			//Create a page template using an image to 
			//create smaller more efficient output
			//using new psimage from file PORTRAIT
//			vars.psimage[0] = PS_open_image_file(vars.ps,"eps","/tmp/letter.ps",NULL,0);
//			vars.pstmpl[1]=PS_begin_template(vars.ps,612,792);
//			PS_place_image(vars.ps,vars.psimage[0],0,0,1.0);
//			PS_end_template(vars.ps);
			break;
	case 222:
//			PS_place_image(vars.ps,vars.pstmpl[0],0,0,1.0);
			break;
	case 230:
			//PoDoFo::PdfMemDocument formdoc("/tmp/form_3010.pdf");

			//PoDoFo::PdfXObject (formdoc, 0, document,);

			//PdfPage *formpage;
			//formpage=document.GetPage(0); //the page in which I want to insert my image
			//painter.SetPage( p );
			//PoDoFo::PdfImage image( &document );
			//PoDoFo::PdfImage image( &formdoc );
			//image.LoadFromFile("path/to/my/image.png");
			//painter.DrawImage( 70.0, writer.GetPage(0)->GetPageSize().GetHeight() - image.GetHeight(), &image );		
			break;

	case 240:
			// This function moves the cursor to a fixed or relative horizontal position 
			// based on an equivalency with PCL dot size.

			if(vars.cmdstr==NULL) { break; }
			ExecEscCmd(301);
			sprintf(vars.string, "%c",0);
			switch (vars.cmdstr[0]){
			case '-':
					memset(temparray,0,32);
					strcpy(temparray,vars.cmdstr+1);
					myfloat=(atof(temparray));
					vars.textx=(vars.textx - (myfloat * 0.244));
					break;
			case '+':
					memset(temparray,0,32);
					strcpy(temparray,vars.cmdstr+1);
					myfloat=(atof(temparray));
					vars.textx=(vars.textx + (myfloat * 0.244));
					break;
			default:
					vars.pagewidth;
					myfloat=(atof(vars.cmdstr));
					vars.textx=(myfloat * 0.244);
					break;
			}

			if(vars.cmdstr==NULL) { fprintf(stderr,"FUNC 240   cmdstr [%s]  myfloat [%8.4f]  temparray [%s]\n",vars.cmdstr,myfloat,temparray); }
			break;
	case 241:
			// This function moves the cursor to a fixed or relative vertical posistion 
			// based on an equivalency with PCL dot size.
                        //vars.pageheight=792;
			
			if(vars.cmdstr==NULL) { break; }
			ExecEscCmd(301);
			sprintf(vars.string, "%c",0);
			switch (vars.cmdstr[0]){
			case '-':
					memset(temparray,0,32);
					strcpy(temparray,vars.cmdstr+1);
					myfloat=(atof(temparray));
					vars.textx=(vars.texty + (myfloat * 0.2428));
					break;
			case '+':
					memset(temparray,0,32);
					strcpy(temparray,vars.cmdstr+1);
					myfloat=(atof(temparray));
					vars.texty=(vars.texty - (myfloat * 0.2428));
					break;
			default:
					myfloat=(atof(vars.cmdstr));
					vars.texty=(vars.pageheight - (myfloat * 0.2428));
					break;
			}

			if(vars.cmdstr==NULL) { fprintf(stderr,"FUNC 241   cmdstr [%s]  myfloat [%8.4f]  temparray [%s]\n",vars.cmdstr,myfloat,temparray); }
			break;
	case 242:
			// This function moves the cursor to a fixed or relative horizontal position 
			// based on postscript signed float values.

			if(vars.cmdstr==NULL) { break; }
			switch (vars.cmdstr[0]){
			case '-':
					memset(temparray,0,32);
					strcpy(temparray,vars.cmdstr+1);
					myfloat=(atof(temparray));
					vars.textx=(vars.textx - myfloat);
					break;
			case '+':
					memset(temparray,0,32);
					strcpy(temparray,vars.cmdstr+1);
					myfloat=(atof(temparray));
					vars.textx=(vars.textx + myfloat);
					break;
			default:
					myfloat=(atof(vars.cmdstr));
					vars.textx=myfloat;
					break;
			}

			break;
	case 243:
			// This function moves the cursor to a fixed or relative vertical posistion 
			// based on postscript signed float values.

			if(vars.cmdstr==NULL) { break; }
			switch (vars.cmdstr[0]){
			case '-':
					memset(temparray,0,32);
					strcpy(temparray,vars.cmdstr+1);
					myfloat=(atof(temparray));
					vars.texty=(vars.textx - myfloat);
					break;
			case '+':
					memset(temparray,0,32);
					strcpy(temparray,vars.cmdstr+1);
					myfloat=(atof(temparray));
					vars.texty=(vars.textx + myfloat);
					break;
			default:
					vars.pagewidth;
					myfloat=(atof(vars.cmdstr));
					vars.texty=(vars.pageheight - myfloat);
					break;
			}

			break;
	case 250:
			// Load Image Like Similar TO Load PCL MACRO.
			// May want to free the previously loaded images before loading the same number again
			if(strlen(vars.cmdstr)==0) { break; } 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"Loading MACRO from [%s]  \n",mystr1); }
#endif /* USE_RDA_DIAGNOSTICS */
			//printf("pgopen=[%d]  \n",vars.pgopen);
			//if(vars.pgopen==0) {	ExecEscCmd(90,myvars);	}
			//printf("pgopen=[%d]  \n",vars.pgopen);

//			mystr1=strtok(vars.cmdstr,":");
//			mystr2=strtok(NULL,":");
//			if((strncmp(mystr2+((strlen(mystr2)) - 4),".eps",4))==0) {
//				vars.psimage[(atoi(mystr1))]=PS_open_image_file(vars.ps,"eps",mystr2,NULL,(atoi(mystr1)));
//			} else if((strncmp(mystr2+((strlen(mystr2)) - 3),".ps",3))==0) {
//				vars.psimage[(atoi(mystr1))]=PS_open_image_file(vars.ps,"eps",mystr2,NULL,(atoi(mystr1)));
//			} else if((strncmp(mystr2+((strlen(mystr2)) - 4),".png",4))==0) {
//				vars.psimage[(atoi(mystr1))]=PS_open_image_file(vars.ps,"png",mystr2,NULL,(atoi(mystr1)));
//			} else if((strncmp(mystr2+((strlen(mystr2)) - 4),".jpg",4))==0) {
//				vars.psimage[(atoi(mystr1))]=PS_open_image_file(vars.ps,"jpeg",mystr2,NULL,(atoi(mystr1)));
//			} else if((strncmp(mystr2+((strlen(mystr2)) - 5),".jpeg",5))==0) {
//				vars.psimage[(atoi(mystr1))]=PS_open_image_file(vars.ps,"jpeg",mystr2,NULL,(atoi(mystr1)));
//			} else if((strncmp(mystr2+((strlen(mystr2)) - 4),".tif",4))==0) {
//				vars.psimage[(atoi(mystr1))]=PS_open_image_file(vars.ps,"tiff",mystr2,NULL,(atoi(mystr1)));
//			} else if((strncmp(mystr2+((strlen(mystr2)) - 5),".tiff",5))==0) {
//				vars.psimage[(atoi(mystr1))]=PS_open_image_file(vars.ps,"tiff",mystr2,NULL,(atoi(mystr1)));
//			}

			//printf("str1=[%s]  str2=[%s] str3=[%s] str4=[%s] str5=[%s] \n",mystr1,mystr2,mystr3,mystr4,mystr5);
			//printf("psimage=%d\n",vars.psimage[(atoi(mystr1))]);
			break;
	case 251:
			// Print Image Like PCL Call MACRO 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"Using MACRO...\n"); }
#endif /* USE_RDA_DIAGNOSTICS */
			if(strlen(vars.cmdstr)==0) { break; } 
			//printf("pgopen=[%d]  \n",vars.pgopen);
			//if(vars.pgopen==0) {	ExecEscCmd(90,myvars);	}
			//printf("pgopen=[%d]  \n",vars.pgopen);
			mystr1=strtok(vars.cmdstr,":");
			mystr2=strtok(NULL,":");
			mystr3=strtok(NULL,":");
			//printf("pgopen=[%d]  str1=[%s]  str2=[%s] str3=[%s] str4=[%s] str5=[%s] \n",vars.pgopen,mystr1,mystr2,mystr3,mystr4,mystr5);
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"Using MACRO [%d]  \n",(atoi(mystr1))); }
#endif /* USE_RDA_DIAGNOSTICS */
//			PS_place_image(vars.ps,vars.psimage[(atoi(mystr1))],(atoi(mystr2)),(atoi(mystr3)),1.0);
			break;
	case 252:
			//PRT_PS_IMAGE
//			PS_place_image(vars.ps,vars.psimage[0],0,0,1.0);
			break;
	case 253:
			//PRT_PS_IMAGE
			//printf("psimage=%d\n",vars.psimage[0]);
			//printf("cmdstr=%s\n",vars.cmdstr);
			mystr1=strtok(vars.cmdstr,":");
			mystr2=strtok(NULL,":");
			mystr3=strtok(NULL,":");
			mystr4=strtok(NULL,":");
			mystr5=strtok(NULL,":");
			//printf("str1=[%s]  str2=[%s] str3=[%s] str4=[%s] str5=[%s] \n",mystr1,mystr2,mystr3,mystr4,mystr5);
			
//			if(vars.psimage[0]==0) {
//				vars.psimage[0] = PS_open_image_file(vars.ps,"eps","/tmp/letter.ps",NULL,0);
//			} 
//			PS_place_image(vars.ps,vars.psimage[0],0,0,1.0);
			//printf("psimage=%d\n",vars.psimage[0]);
			break;
	case 260:
			// Load Image Like Similar TO Load PCL MACRO.
			if(strlen(vars.cmdstr)==0) { break; } 

			mystr1=strtok(vars.cmdstr,":");
			mystr2=strtok(NULL,":");
			mystr3=strtok(NULL,":");
			mystr4=strtok(NULL,":");

//			DecryptDataFromFile(mystr2,mystr3,&mystr4,&size);
//			if(size!=0) {
//				vars.psimage[(atoi(mystr1))]=PS_open_image(vars.ps,"eps",NULL,mystr4,size,0,0,0,0,NULL);
//			}
			if(mystr4!=NULL) free(mystr4);
			//fprintf(stderr,"PS_open_image result [%d]\n",vars.psimage[(atoi(mystr1))]);

			//printf("str1=[%s]  str2=[%s] str3=[%s] str4=[%s] str5=[%s] \n",mystr1,mystr2,mystr3,mystr4,mystr5);
			//printf("psimage=%d\n",vars.psimage[(atoi(mystr1))]);
			break;
	case 300:
//			PS_stringwidth(vars.ps,vars.string,vars.psfont,12);
			//fprintf(stderr,"PRT_TEXT_XY STRLEN=%s\n",(PS_stringwidth(ps,string,psfont,12)));
			//PRT_TEXT_XY
//			PS_show_xy(vars.ps,vars.string,300,300);
			break;
	case 301:
			
			LOG_DEBUG << "EscCmd: " << rda_exec_cmd << "   X:" << vars.textx << "  Y:" << vars.texty  <<  "   string [" << vars.string << "]";
			LOG_DEBUG << "PrintStyle: " << vars.printstyle; 

			if(vars.pgopen==0)
			{
				LOG_DEBUG << "pgopen=0, running ExecEscCmd(90)"; 
				ExecEscCmd(90);
			}
			//fprintf(stderr,"PRT_TEXT_XY STRLEN=%d\n",(PS_stringwidth(ps,string,psfont,7)));
			//PRT_TEXT_XY
			//fprintf(stderr,"X[%f} Y[%f] MY_X[%d] MY_Y[%d]",*textx_pt,*texty_pt,my_textx,my_texty);
			//PS_show_xy(ps,string,&textx,&texty);
			//fprintf(stderr,"StringWidth=%5.2f\n",PS_stringwidth(ps,string,font_array[0],12));
			
//			PS_show_xy(vars.ps,vars.string,vars.textx,vars.texty);
			painter.DrawText(vars.textx,vars.texty,vars.string);
			
			//PS_show_xy(vars.ps,vars.string,vars.*textx_pt,vars.*texty_pt);


/* TODO: Test case for switching fonts on same line only works with one text line.
 
			myfloat=PS_stringwidth(ps,string,font_array[0],12);
			*textx_pt=(*textx_pt + myfloat);
			
			// Calculate the size of one normal char
			fprintf(stderr,"CharWidth=%5.2f\n",PS_stringwidth(ps,"1",font_array[0],12));
			

			
			font_array[10] = PS_findfont(ps,"MRVCode39extSA","",1);
			PS_setfont(ps,font_array[10],12.0);
			font_array[11] = font_array[0];
			font_array[0] = font_array[10];
			fprintf(stderr,"CharWidth=%5.2f\n",PS_stringwidth(ps,"1",font_array[0],12));
			PS_show_xy(ps,string,*textx_pt,*texty_pt);
			
			font_array[0] = font_array[11];
			PS_setfont(ps,font_array[0],12.0);
			myfloat=PS_stringwidth(ps,string,font_array[0],12);
			*textx_pt=(*textx_pt + myfloat + (PS_stringwidth(ps,"1",font_array[0],12))*2);
			
			PS_show_xy(ps,string,*textx_pt,*texty_pt);
			*textx_pt=(*textx_pt - myfloat);
			*textx_pt=(*textx_pt - myfloat);
*/
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
		//	if(vars.pgopen==0) {
				// This needs to take the page size into account instead of just LETTER/PORTRAIT
				// May not be needed now that prepage commands are processed post page start.
		//		 vars.pushpopx=18.0; 
		//		 vars.pushpopy=748.0; 
		//	} else {
				vars.pushpopx=vars.textx;
				vars.pushpopy=vars.texty;
		//	}
			break;
	case 314:
			// STUB* Variable Name: [POP CURSOR] 
			// PCL Pop equiv command

			// This section added to address X/Y coordinate hopping to ensure 
			// text for previous coordinates prints at correct location when 
			// POP is at the end of a line.  May need to change, testing needed.
			ExecEscCmd(301);
			sprintf(vars.string, "%c",0);
			//
			
			vars.textx=vars.pushpopx;
			vars.texty=vars.pushpopy;
			break;
	case 315:
			// Variable Name: [NORMAL TEXT] 
			//if(vars.pgopen==0) {	ExecEscCmd(90,myvars);	}
			if(vars.pgopen==0) {
				vars.printstyle=1;
			} else {	
				ExecEscCmd(001);
			}
				
			break;
	case 316:
			// Variable Name: [COMPRESSED TEXT] 
			//if(vars.pgopen==0) {	ExecEscCmd(90,myvars);	}
			if(vars.pgopen==0) {
				vars.printstyle=2;
			} else {	
				ExecEscCmd(002);
			}
			break;
	case 317:
			// STUB* Variable Name: [ELITE PRINT] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: ELITE PRINT Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 318:
			// STUB* Variable Name: [PAGE_SIZE_LETTER] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: PAGE_SIZE_LETTER Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			if(vars.landscape==1){
				vars.pagesizedef=2;
			}else{
				vars.pagesizedef=1;
			}

			break;
	case 319:
			// STUB* Variable Name: [PAGE_SIZE_LEGAL] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: PAGE_SIZE_LEGAL Not Implemented [%d].\n",vars.rda_exec_cmd);}
#endif /* USE_RDA_DIAGNOSTICS */
			if(vars.landscape==1){
				vars.pagesizedef=4;
			}else{
				vars.pagesizedef=3;
			}
			break;
	case 320:
			// STUB* Variable Name: [PAGE_SIZE_LEDGER] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: PAGE_SIZE_LEDGER Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			if(vars.landscape==1){
				vars.pagesizedef=8;
			}else{
				vars.pagesizedef=7;
			}
			break;
	case 321:
			// STUB* Variable Name: [PAGE_SIZE_POSTER] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: PAGE_SIZE_POSTER Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			if(vars.landscape==1){
				vars.pagesizedef=6;
			}else{
				vars.pagesizedef=5;
			}
			break;
	case 330:
			// STUB* Variable Name: [PORTRAIT] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: PORTRAIT Partially Implemented for Letter Sized Paper [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			vars.landscape=0;
			switch (vars.pagesizedef) {
				case 2:
				case 4:
				case 6:
				case 8:
					--vars.pagesizedef;
					// Hopefully, this will work for all defined page types.  
					break;
				default:
					// This should ignore any other page size changes for non landscape page sizes.
					break;
			}
			break;
	case 331:
			// STUB* Variable Name: [LANDSCAPE] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: LANDSCAPE Partially Implemented for Letter Sized Paper [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			vars.landscape=1;
			switch (vars.pagesizedef) {
				case 0:
					++vars.pagesizedef;
					++vars.pagesizedef;
					break;
				case 1:
				case 3:
				case 5:
				case 7:
					++vars.pagesizedef;
					// Hopefully, this will work for all defined page types.  
					break;
				default:
					// This should ignore any other page size changes for non portrait page sizes.
					break;
			}
			break;
	case 332:
			// STUB* Variable Name: [SIMPLEX] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: SIMPLEX Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 333:
			// STUB* Variable Name: [DUPLEX LONG] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: DUPLEX LONG Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 334:
			// STUB* Variable Name: [DUPLEX SHORT] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: DUPLEX SHORT Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 335:
			// STUB* Variable Name: [REVERSE PORTRAIT] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: REVERSE PORTRAIT Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 336:
			// STUB* Variable Name: [REVERSE LANDSCAPE] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: REVERSE LANDSCAPE Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 337:
			// STUB* Variable Name: [ASCII SQUARE] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: ASCII SQUARE Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 338:
			// STUB* Variable Name: [ESCAPE] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: ESCAPE Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 339:
			// STUB* Variable Name: [HALF LINE FEED] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: HALF LINE FEED [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			//fprintf(stderr,"pgopen [%d],  nlcounter [%d],  texty [%5.2f]\n",vars.pgopen,vars.nlcounter,vars.texty);
			if(vars.nlcounter==1) {
				if(vars.printstyle == 1) {
					ExecEscCmd(001);
				} else if(vars.printstyle == 2) {
					ExecEscCmd(002);
				}
				vars.texty=(vars.texty - 6);
			}else{
				ExecEscCmd(301);
				sprintf(vars.string, "%c",0);
				vars.texty=(vars.texty - 6);
			}
			//vars.texty=(vars.texty - 6);

			// Not sure what the actual postscript pixel value is for a half line feed.  Testing...
			//vars.texty=(vars.texty - 9);
			
			//vars.texty=(vars.texty - 12);
			
			//vars.texty=(vars.texty - vars.line_space[0]);

			break;
	case 340:
			// STUB* Variable Name: [VENTREAS] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: VENTREAS Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 341:
			// STUB* Variable Name: [PAYREG] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: PAYREG Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 342:
			// STUB* Variable Name: [POCERT] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: POCERT Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 343:
			// STUB* Variable Name: [PAYCKRG] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: PAYCKRG Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 344:
			// STUB* Variable Name: [VENCKRG] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: VENCKRG Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 345:
			// STUB* Variable Name: [SIGNATURE LOCK] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: SIGNATURE LOCK Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 346:
			// STUB* Variable Name: [SIGNATURE UNLOCK] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: SIGNATURE UNLOCK Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 347:
			// STUB* Variable Name: [POAUDIT] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: POAUDIT Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 348:
			// STUB* Variable Name: [SIGNATURE FILE] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: SIGNATURE FILE Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 349:
			// * Variable Name: [UNLOAD_SOFTFONTS] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: UNLOAD_SOFTFONTS Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 350:
			// * Variable Name: [RESET FONT] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: RESET FONT Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 351:
			// * Variable Name: [LOAD VENPMT SIGNATURE] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: LOAD VENPMT SIGNATURE Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			// This will load all available rda-ven[123456789].dat files as separate 
			// signature images if the files exist. File checks will be done with 
			// the stat() function.The image numbers will correspond with the file 
			// number.  The passed string will be XY coordinates for the image placement.
			
			
			// This section may be irrelevant now that code 260 was added.  
			// This part of the code was added to the calling program
			// due to the need to pass password credentials to decrypt.
//			if(!FileExists(NULL,"rda-ven1.dat")) { TRACE; }
//			if(!FileExists(NULL,"rda-ven2.dat")) { TRACE; }
//			if(!FileExists(NULL,"rda-ven3.dat")) { TRACE; }
//			if(!FileExists(NULL,"rda-ven4.dat")) { TRACE; }
//			if(!FileExists(NULL,"rda-ven5.dat")) { TRACE; }
//			if(!FileExists(NULL,"rda-ven6.dat")) { TRACE; }
//			if(!FileExists(NULL,"rda-ven7.dat")) { TRACE; }
//			if(!FileExists(NULL,"rda-ven8.dat")) { TRACE; }
//			if(!FileExists(NULL,"rda-ven9.dat")) { TRACE; }
			break;
	case 352:
			// * Variable Name: [LOAD PAYROLL SIGNATURE] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: LOAD PAYROLL SIGNATURE Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			// This will load all available rda-pay[123456789].dat files as separate 
			// signature images if the files exist. File checks will be done with 
			// the stat() function.The image numbers will correspond with the file 
			// number.  The passed string will be XY coordinates for the image placement.
			// This will need to pass the password as well...
			
			
			// This section may be irrelevant now that code 260 was added.  
			// This part of the code was added to the calling program
			// due to the need to pass password credentials to decrypt.
//			if(!FileExists(NULL,"rda-pay1.dat")) { TRACE; }
//			if(!FileExists(NULL,"rda-pay2.dat")) { TRACE; }
//			if(!FileExists(NULL,"rda-pay3.dat")) { TRACE; }
//			if(!FileExists(NULL,"rda-pay4.dat")) { TRACE; }
//			if(!FileExists(NULL,"rda-pay5.dat")) { TRACE; }
//			if(!FileExists(NULL,"rda-pay6.dat")) { TRACE; }
//			if(!FileExists(NULL,"rda-pay7.dat")) { TRACE; }
//			if(!FileExists(NULL,"rda-pay8.dat")) { TRACE; }
//			if(!FileExists(NULL,"rda-pay9.dat")) { TRACE; }

			break;
	case 353:
			// * Variable Name: [VENPMT SIGNATURE] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: VENPMT SIGNATURE Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			// This will call a general function with one argument only.  The argument will 
			// be a series of single digit numbers.  This is the number of the signature
			// loaded in the correct order to print the signature.  If not corresponding 
			// XY coordinates are set, the signature will not be printed.
			break;
	case 354:
			// * Variable Name: [PAYROLL SIGNATURE] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: PAYROLL SIGNATURE Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			// This will call a general function with one argument only.  The argument will 
			// be a series of single digit numbers.  This is the number of the signature
			// loaded in the correct order to print the signature.  If not corresponding 
			// XY coordinates are set, the signature will not be printed.
			break;
	case 355:
			// * Variable Name: [DIRECT DEPOSIT] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: DIRECT DEPOSIT Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 356:
			// * Variable Name: [PAYROLL CHECK] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: PAYROLL CHECK Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 357:
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 358:
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 359:
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: Not Implemented [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			break;
	case 360:
			// STUB* Variable Name: [NEG Q LINE FEED] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: NEG Q LINE FEED [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			//fprintf(stderr,"pgopen [%d],  nlcounter [%d],  texty [%5.2f]\n",vars.pgopen,vars.nlcounter,vars.texty);
			if(vars.nlcounter==1) {
				if(vars.printstyle == 1) {
					ExecEscCmd(001);
				} else if(vars.printstyle == 2) {
					ExecEscCmd(002);
				}
				vars.texty=(vars.texty + 3);
			}else{
				ExecEscCmd(301);
				sprintf(vars.string, "%c",0);
				vars.texty=(vars.texty + 3);
			}
			break;
	case 361:
			// STUB* Variable Name: [POS Q LINE FEED] 
#ifdef USE_RDA_DIAGNOSTICS
			if(vars.debug) { fprintf(stderr,"STUB: POS Q LINE FEED [%d].\n",vars.rda_exec_cmd); }
#endif /* USE_RDA_DIAGNOSTICS */
			//fprintf(stderr,"pgopen [%d],  nlcounter [%d],  texty [%5.2f]\n",vars.pgopen,vars.nlcounter,vars.texty);
			if(vars.nlcounter==1) {
				if(vars.printstyle == 1) {
					ExecEscCmd(001);
				} else if(vars.printstyle == 2) {
					ExecEscCmd(002);
				}
				vars.texty=(vars.texty - 3);
			}else{
				ExecEscCmd(301);
				sprintf(vars.string, "%c",0);
				vars.texty=(vars.texty - 3);
			}
			break;
	case 399:
			//fprintf(stderr,"%s",cmdstr[4],cmdstr[5]);
			
			//fprintf(stderr,"STRLEN[%d]  %c\n",strlen(cmdstr),cmdstr[3]);
			break;
	default: 
			LOG_DEBUG <<"Unknown Command [" << vars.rda_exec_cmd <<"].";
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


	int LaunchPDFViewer()
	{
		int execstatus=0;
		char *mytemp=NULL;
		char *myenv1=NULL,*myenv2=NULL,*myenv3=NULL;
	
#ifdef WIN32
		DWORD bufsize=0;
#endif
	
		if(vars.viewfile) {
			mytemp=(char *)malloc(sizeof(char *)*512);
#ifndef WIN32
			myenv1=getenv("CYBER_PRODUCT");
			myenv2=getenv("CYBER_PROCESS");
			myenv3=getenv("REPORT_TITLE");
			sprintf(mytemp,"acroread -tempFileTitle \"%s - %s - %s\" -openInNewWindow \"%s\"",(myenv1!=NULL ? myenv1:"UTILITIES"),(myenv2!=NULL ? myenv2:"ViewPDF"),(myenv3!=NULL ? myenv3:vars.output.c_str()),vars.output.c_str());
		
			LOG_DEBUG << "CMD:  [" << mytemp << "]";
			fprintf(stderr,"Output File: %s\n",vars.output.c_str());
	
	 
			execstatus=system(mytemp);
			LOG_DEBUG << "Exit Code: " << WEXITSTATUS(execstatus);
			if(vars.removefile) {
				unlink(vars.output.c_str());
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
			myenv1=(char *)malloc(sizeof(char *)*512);
			GetCurrentDirectoryA(&bufsize,myenv1);
			sprintf(mytemp,"%s\\%s",myenv1,vars.output.c_str());
			fprintf(stderr,"Output File: %s\\%s\n",myenv1,vars.output.c_str());
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

	void HelloWorld()
	{
		pPage = document.CreatePage( PdfPage::CreateStandardPageSize( ePdfPageSize_Letter ) );

		

		if( !pPage ) 
		{
			PODOFO_RAISE_ERROR( ePdfError_InvalidHandle );
		}

		painter.SetPage( pPage );

		pFont = document.CreateFont( "Arial" );
		if( !pFont )
		{
			PODOFO_RAISE_ERROR( ePdfError_InvalidHandle );
		}

		Font_Courier->SetFontSize(12.0);
		painter.SetFont(Font_Courier);

		//pFont->SetFontSize( 18.0 );
		//painter.SetFont( pFont );

		//SAFE_OP( painter.SetColor( 1.0, 0.0, 0.0 ) );

		painter.DrawText( 56.69, pPage->GetPageSize().GetHeight() - 56.69, "Hello World!" );

		painter.FinishPage();

		document.GetInfo()->SetCreator ( PdfString("examplahelloworld - A PoDoFo test application") );
		document.GetInfo()->SetAuthor  ( PdfString("Dominik Seichter") );
		document.GetInfo()->SetTitle   ( PdfString("Hello World") );
		document.GetInfo()->SetSubject ( PdfString("Testing the PoDoFo PDF Library") );
		document.GetInfo()->SetKeywords( PdfString("Test;PDF;Hello World;") );
		
		document.Close();

	}

	void viewdoc()
	{
		//fprintf(stderr,"pointer: [%p]\n",(void*)&document);
		//pPage = document.CreatePage( PoDoFo::PdfPage::CreateStandardPageSize( ePdfPageSize_A4 ) );
		//fprintf(stderr,"pointer: [%p]\n",(void*)&document);
	}

	void newdoc(const char *name)
	{
		fprintf(stderr,"pointer: [%p]\n",(void*)&document);
		PoDoFo::PdfStreamedDocument document2(name);
		//*document=&document2;
		
		//document.PdfStreamedDocument(name);
		fprintf(stderr,"pointer: [%p]\n",(void*)&document);
	}

private:
	PoDoFo::PdfOutputDevice device;
	PoDoFo::PdfStreamedDocument document;
	//PoDoFo::PdfMemDocument document;
	PoDoFo::PdfPainter painter;
	PoDoFo::PdfPage *pPage;
	PoDoFo::PdfPage *LastPage;
	PoDoFo::PdfPage *NextPage;
	PoDoFo::PdfFont *Font_Courier;
	PoDoFo::PdfFont *Font_Arial;
	PoDoFo::PdfFont *pFont;
	PoDoFo::PdfFont *DefaultFont;
	PoDoFo::PdfFont *LastFont;
};

int main(int argc, char *argv[])
{
	int x;
	
	ViewPDF_Vars vars;
	vars.ParseCmdLine(argc,argv);

/*
	vars.viewfile=12;
	LOG_FATAL << "vars " << vars.viewfile;
	LOG_FATAL << "vars " << vars.input;
	LOG_FATAL << "vars " << vars.output;
*/

	ViewPDF_Document MyDoc(vars,vars.output.c_str());
	LOG_WARN << "MyDocvars " << MyDoc.vars.input;
	LOG_WARN << "MyDocvars " << MyDoc.vars.output;

/*
	MyDoc.vars.vecprepagecmd.push_back(12);
	MyDoc.vars.vecprepagecmdstr.push_back("twelve");

	MyDoc.vars.vecprepagecmd.push_back(13);
	MyDoc.vars.vecprepagecmdstr.push_back("thirteen");

	LOG_FATAL << "num: " << MyDoc.vars.vecprepagecmd[0] << "value: " <<  MyDoc.vars.vecprepagecmdstr[0];
	LOG_FATAL << "num: " << MyDoc.vars.vecprepagecmd[1] << "value: " <<  MyDoc.vars.vecprepagecmdstr[1];
	MyDoc.vars.vecprepagecmd.clear();
	MyDoc.vars.vecprepagecmdstr.clear();
	return(0);
*/


	try {
		//MyDoc.HelloWorld();
		MyDoc.InitDoc();

		if(vars.input.size()==0)
		{
			MyDoc.ReadParseFromSTDIN();
		}else{
			MyDoc.ReadInputFile(vars.input.c_str());
			MyDoc.ParseInputLoop();
		}

//		for(x=0;x<500;x++)
//		{
//			MyDoc.MyNewPage(0);
//			MyDoc.NewPage();
//		}
		MyDoc.CloseDoc();
		
		if(vars.viewfile) 
		{
			MyDoc.LaunchPDFViewer();
		}

	} catch( const PdfError & eCode ) {
		// We have to check if an error has occurred.
		// If yes, we return and print an error message
		// to the commandline.
		eCode.PrintErrorMsg();
		return eCode.GetError();
	}


}


