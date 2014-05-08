//
// Copyright (C) 2009-2014 RDA systems Inc.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
// Linking this library statically or dynamically with other modules is making
// a combined work based on this library.  Thus, the terms and conditions of
// the GNU General Public License cover the whole combination.
//

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <unistd.h>
#include "vmime/vmime.hpp"
#ifdef WIN32
#include "vmime/platforms/windows/windowsHandler.hpp"
#else
#include "vmime/platforms/posix/posixHandler.hpp"
#endif
#include "vmime/streamContentHandler.hpp"
#ifndef WIN32
#include "vmime/net/serviceInfos.hpp"
#include "vmime/net/sendmail/sendmailServiceInfos.hpp"
#include "vmime/security/cert/certificateChain.hpp"
#include "vmime/net/smtp/SMTPServiceInfos.hpp"
#endif
#include "vmime/fileAttachment.hpp"
#include "vmime/exception.hpp"
#include "vmime/streamContentHandler.hpp"
#include "vmime/contentDispositionField.hpp"
#ifdef WIN32
#define BUFSIZE 1024
#define PIPE_TIMEOUT 5000
#include <process.h>
#endif

#ifdef WIN32
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <io.h>
extern "C" int win32_exec_handles(HANDLE **procinfo,HANDLE *stdioh_0,HANDLE *stdioh_1,HANDLE *stdioh_2,char *option,int wait);

extern "C" PHANDLE win32_popen_h(HANDLE **procinfo,char *cmdline,char *mode);
extern "C" int win32_pclose_h(HANDLE **procinfo,PHANDLE *pipe);
extern "C" FILE* win32_popen(HANDLE **procinfo,char *cmdline,char *mode);
extern "C" int win32_pclose(HANDLE **procinfo,FILE *fp);
extern "C" int bufferwrite(FILE *fp,const char *value,int size,int maxbuf);
#endif

#define TRACE  std::cerr << "TRACE: " << __LINE__ << std::endl;

// Code for CommandLineToArgvA from GPL3 project latexdaemon 
// at url http://code.google.com/p/latexdaemon/source/browse/latexdaemon/CommandLineToArgv.cpp?r=30
#ifdef WIN32
#ifndef WIN32_CONSOLE
PCHAR* CommandLineToArgvA( PCHAR CmdLine, int* _argc )
{
	PCHAR* argv;
	PCHAR _argv;
	size_t len;
	int   argc;
	CHAR  a;
	size_t i,j;

	BOOLEANin_QM;
	BOOLEANin_TEXT;
	BOOLEANin_SPACE;

	len = strlen(CmdLine);
	i = ((len+2)/2)*sizeof(PVOID) + sizeof(PVOID);

	argv = (PCHAR*)GlobalAlloc(GMEM_FIXED,(i + (len+2)*sizeof(CHAR)));

	_argv = (PCHAR)(((PUCHAR)argv)+i);

	argc = 0;
	argv[argc] = _argv;
	in_QM = FALSE;
	in_TEXT = FALSE;
	in_SPACE = TRUE;
	i = 0;
	j = 0;

	while( a = CmdLine[i] ) {
		if(in_QM) {
			 if(a == '\"') {
				in_QM = FALSE;
			 } else {
				_argv[j] = a;
				j++;
			 }
		} else {
			 switch(a) {
			 case '\"':
				in_QM = TRUE;
				in_TEXT = TRUE;
				if(in_SPACE) {
					argv[argc] = _argv+j;
					argc++;
				}
				in_SPACE = FALSE;
				break;
			 case ' ':
			 case '\t':
			 case '\n':
			 case '\r':
				if(in_TEXT) {
					_argv[j] = '\0';
					j++;
				}
				in_TEXT = FALSE;
				in_SPACE = TRUE;
				break;
			 default:
				in_TEXT = TRUE;
				if(in_SPACE) {
					argv[argc] = _argv+j;
					argc++;
				}
				_argv[j] = a;
				j++;
				in_SPACE = FALSE;
				break;
			 }
		}
		i++;
	}
	_argv[j] = '\0';
	argv[argc] = NULL;

	(*_argc) = argc;
	return argv;
}
#endif
#endif


using namespace std;
using namespace vmime;
#ifndef WIN32
using namespace net;
using namespace sendmail;
using namespace security;
using namespace cert;
using namespace smtp;
#endif
using namespace utility;

#ifdef DONT_COMPILE
//#ifndef WIN32
class myCertVerifier:public vmime::security::cert::certificateVerifier
{
public :
void verify(vmime::ref <certificateChain> certs )
{
        // Obtain the subject's certificate
/*
        vmime::ref <vmime::security::cert::certificate> cert = chain->getAt(0);
        std::cout << std::endl;
        std::cout << "Server sent a '" << cert->getType() << " '" << " certificate ." << std::endl ;
        std::cout << "Do you want to accept this certificate? (Y/n) ";
        std::cout.flush( );
        std::string answer;
        std::getline (std::cin,answer);
        if (answer.length() != 0 && (answer[0] == 'Y' | | answer[0] == 'y ' ))
*/
        return; // OK, we trust the certificate
        // Don't trust this certificate
//        throw exceptions::certificate_verification_exception( );
}
};
#endif

struct SENDMAIL_VARIABLES 
{
	char *from_name;
	char *from_addr;
	char *subject;
	char *body;
	char *tolist;
	char *cclist;
	char *bcclist;
	char *replyto;
	char *srcuser;
	
	//  Need some type of arrays or pointers to hold filename, file type, display name in email, and maybe even the actual file data in memory...
	char *fname[10];	
	char *mtype[10];	
	char *dname[10];	
	char *fdata[10];	
	int ignerr;
	int rdadiag;

};
typedef struct SENDMAIL_VARIABLES MAIL_VARS;


/*
fileAttachment::fileAttachmentfromcstring(char data, const mediaType& type, const text& desc)
{
        m_type = type;
        m_desc = desc;

	//  Need to read the char type data variable into a stringstream then set the input handler to the vmime type....
        setDataSTDIN();

        m_encoding = encoding::decide(m_data);
}

fileAttachment::fileAttachmentSTDIN(const mediaType& type, const text& desc)
{
        m_type = type;
        m_desc = desc;

        setDataSTDIN();

        m_encoding = encoding::decide(m_data);
}

void fileAttachment::setDataSTDIN()
{
        std::ifstream* file = new std::ifstream();
        file->open(filename.c_str(), std::ios::in | std::ios::binary);

        if (!*file)
        {
                delete file;
                throw exceptions::open_file_error();
        }

        ref <utility::inputStream> is = vmime::create <utility::inputStreamPointerAdapter>(file, true);

        m_data = vmime::create <streamContentHandler>(is, 0);
}

*/
char *getEnvVar(std::string const& key)
{
	char const* val = getenv(key.c_str()); 

	if(val!=NULL)
	{
		std::string temp(val);
		temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());
		temp.erase(std::remove(temp.begin(), temp.end(), '\r'), temp.end());
		//return((char*)temp.c_str());
		
		const std::string::size_type size = temp.size();
		char *buffer = new char[size + 1];   //we need extra char for NUL
		memcpy(buffer, temp.c_str(), size + 1);
		return(buffer);
	}else{
		return(NULL);
	}
}
void DelayAfterSending()
{
	int delay_seconds=0;
	char *vtemp=NULL;

	vtemp=getenv("FORCE_SMTP_DELAY");

	if(vtemp!=NULL)
	{
		try
		{
			stringstream delay_str(vtemp);
			if( (delay_str >> delay_seconds).fail() )
			{
				delay_seconds=5;
			}
		}
		catch (std::exception& e)
		{
			delay_seconds=0;
		}
		//std::cout << "delay string: [" << vtemp << "]   delay int [" << delay_seconds << "]" << std::endl;
#ifndef WIN32
		sleep(delay_seconds);
#else
		Sleep((delay_seconds*1000));
#endif
	}
}

int sendattach(SENDMAIL_VARIABLES *mvars)
{
#ifndef WIN32
	vmime::platform::setHandler<vmime::platforms::posix::posixHandler>();
#else
	vmime::platform::setHandler<vmime::platforms::windows::windowsHandler>();
#endif


	
	if((mvars->from_addr==NULL)&&(mvars->tolist==NULL)&&(mvars->cclist==NULL)&&(mvars->bcclist==NULL)&&(mvars->from_addr==NULL)) { 
		cerr << "No Email Recipients Specified..."  <<endl ;	
		ostringstream errmsg;
		std::string errstring;
#ifdef WIN32
		errmsg << "ok.exe ERRORDIALOG \"ERROR SENDING EMAIL\" \"No Email Recipients Specified..." << endl << endl << endl << endl
#else
		errmsg << "ok.lnx ERRORDIALOG \"ERROR SENDING EMAIL\" \"No Email Recipients Specified..." << endl << endl << endl << endl
#endif
		<< (mvars->from_name==NULL ? "" : "From Name: ") << (mvars->from_name==NULL ? "" : mvars->from_name) << (mvars->from_name==NULL ? "" : "\n" )
		<< (mvars->from_addr==NULL ? "" : "From Address: ") << (mvars->from_addr==NULL ? "" : mvars->from_addr) << (mvars->from_addr==NULL ? "" : "\n" )
		<< (mvars->tolist==NULL ? "" : "TO: ") << (mvars->tolist==NULL ? "" : mvars->tolist) << (mvars->tolist==NULL ? "" : "\n" )
		<< (mvars->cclist==NULL ? "" : "CC: ") << (mvars->cclist==NULL ? "" : mvars->cclist) << (mvars->cclist==NULL ? "" : "\n" )
		<< (mvars->bcclist==NULL ? "" : "BCC: ") << (mvars->bcclist==NULL ? "" : mvars->bcclist) << (mvars->bcclist==NULL ? "" : "\n" )
		<< (mvars->subject==NULL ? "" : "Subject: ") << (mvars->subject==NULL ? "" : mvars->subject) << (mvars->subject==NULL ? "" : "\n" )
		<< (mvars->fname[0]==NULL ? "" : "Filename 0: ") << (mvars->fname[0]==NULL ? "" : mvars->fname[0]) << (mvars->fname[0]==NULL ? "" : "\n" )
		<< (mvars->fname[1]==NULL ? "" : "Filename 1: ") << (mvars->fname[1]==NULL ? "" : mvars->fname[1]) << (mvars->fname[1]==NULL ? "" : "\n" )
		<< (mvars->fname[2]==NULL ? "" : "Filename 2: ") << (mvars->fname[2]==NULL ? "" : mvars->fname[2]) << (mvars->fname[2]==NULL ? "" : "\n" )
		<< (mvars->fname[3]==NULL ? "" : "Filename 3: ") << (mvars->fname[3]==NULL ? "" : mvars->fname[3]) << (mvars->fname[3]==NULL ? "" : "\n" )
		<< (mvars->fname[4]==NULL ? "" : "Filename 4: ") << (mvars->fname[4]==NULL ? "" : mvars->fname[4]) << (mvars->fname[4]==NULL ? "" : "\n" )
		<< (mvars->fname[5]==NULL ? "" : "Filename 5: ") << (mvars->fname[5]==NULL ? "" : mvars->fname[5]) << (mvars->fname[5]==NULL ? "" : "\n" )
		<< (mvars->fname[6]==NULL ? "" : "Filename 6: ") << (mvars->fname[6]==NULL ? "" : mvars->fname[6]) << (mvars->fname[6]==NULL ? "" : "\n" )
		<< (mvars->fname[7]==NULL ? "" : "Filename 7: ") << (mvars->fname[7]==NULL ? "" : mvars->fname[7]) << (mvars->fname[7]==NULL ? "" : "\n" )
		<< (mvars->fname[8]==NULL ? "" : "Filename 8: ") << (mvars->fname[8]==NULL ? "" : mvars->fname[8]) << (mvars->fname[8]==NULL ? "" : "\n" )
		<< (mvars->fname[9]==NULL ? "" : "Filename 9: ") << (mvars->fname[9]==NULL ? "" : mvars->fname[9]) << (mvars->fname[9]==NULL ? "" : "\n" )
		<< (mvars->dname[0]==NULL ? "" : "Attachment 0: ") << (mvars->dname[0]==NULL ? "" : mvars->dname[0]) << (mvars->dname[0]==NULL ? "" : "\n" )
		<< (mvars->dname[1]==NULL ? "" : "Attachment 1: ") << (mvars->dname[1]==NULL ? "" : mvars->dname[1]) << (mvars->dname[1]==NULL ? "" : "\n" )
		<< (mvars->dname[2]==NULL ? "" : "Attachment 2: ") << (mvars->dname[2]==NULL ? "" : mvars->dname[2]) << (mvars->dname[2]==NULL ? "" : "\n" )
		<< (mvars->dname[3]==NULL ? "" : "Attachment 3: ") << (mvars->dname[3]==NULL ? "" : mvars->dname[3]) << (mvars->dname[3]==NULL ? "" : "\n" )
		<< (mvars->dname[4]==NULL ? "" : "Attachment 4: ") << (mvars->dname[4]==NULL ? "" : mvars->dname[4]) << (mvars->dname[4]==NULL ? "" : "\n" )
		<< (mvars->dname[5]==NULL ? "" : "Attachment 5: ") << (mvars->dname[5]==NULL ? "" : mvars->dname[5]) << (mvars->dname[5]==NULL ? "" : "\n" )
		<< (mvars->dname[6]==NULL ? "" : "Attachment 6: ") << (mvars->dname[6]==NULL ? "" : mvars->dname[6]) << (mvars->dname[6]==NULL ? "" : "\n" )
		<< (mvars->dname[7]==NULL ? "" : "Attachment 7: ") << (mvars->dname[7]==NULL ? "" : mvars->dname[7]) << (mvars->dname[7]==NULL ? "" : "\n" )
		<< (mvars->dname[8]==NULL ? "" : "Attachment 8: ") << (mvars->dname[8]==NULL ? "" : mvars->dname[8]) << (mvars->dname[8]==NULL ? "" : "\n" )
		<< (mvars->dname[9]==NULL ? "" : "Attachment 9: ") << (mvars->dname[9]==NULL ? "" : mvars->dname[9]) << (mvars->dname[9]==NULL ? "" : "\n" )
		<< (mvars->mtype[0]==NULL ? "" : "MimeType 0: ") << (mvars->mtype[0]==NULL ? "" : mvars->mtype[0]) << (mvars->mtype[0]==NULL ? "" : "\n" )
		<< (mvars->mtype[1]==NULL ? "" : "MimeType 1: ") << (mvars->mtype[1]==NULL ? "" : mvars->mtype[1]) << (mvars->mtype[1]==NULL ? "" : "\n" )
		<< (mvars->mtype[2]==NULL ? "" : "MimeType 2: ") << (mvars->mtype[2]==NULL ? "" : mvars->mtype[2]) << (mvars->mtype[2]==NULL ? "" : "\n" )
		<< (mvars->mtype[3]==NULL ? "" : "MimeType 3: ") << (mvars->mtype[3]==NULL ? "" : mvars->mtype[3]) << (mvars->mtype[3]==NULL ? "" : "\n" )
		<< (mvars->mtype[4]==NULL ? "" : "MimeType 4: ") << (mvars->mtype[4]==NULL ? "" : mvars->mtype[4]) << (mvars->mtype[4]==NULL ? "" : "\n" )
		<< (mvars->mtype[5]==NULL ? "" : "MimeType 5: ") << (mvars->mtype[5]==NULL ? "" : mvars->mtype[5]) << (mvars->mtype[5]==NULL ? "" : "\n" )
		<< (mvars->mtype[6]==NULL ? "" : "MimeType 6: ") << (mvars->mtype[6]==NULL ? "" : mvars->mtype[6]) << (mvars->mtype[6]==NULL ? "" : "\n" )
		<< (mvars->mtype[7]==NULL ? "" : "MimeType 7: ") << (mvars->mtype[7]==NULL ? "" : mvars->mtype[7]) << (mvars->mtype[7]==NULL ? "" : "\n" )
		<< (mvars->mtype[8]==NULL ? "" : "MimeType 8: ") << (mvars->mtype[8]==NULL ? "" : mvars->mtype[8]) << (mvars->mtype[8]==NULL ? "" : "\n" )
		<< (mvars->mtype[9]==NULL ? "" : "MimeType 9: ") << (mvars->mtype[9]==NULL ? "" : mvars->mtype[9]) << (mvars->mtype[9]==NULL ? "" : "\n" )
		<< (mvars->fdata[0]==NULL ? "" : "File Data 0: ") << (mvars->fdata[0]==NULL ? "" : "TRUE") << (mvars->fdata[0]==NULL ? "" : "\n" )
		<< (mvars->fdata[1]==NULL ? "" : "File Data 1: ") << (mvars->fdata[1]==NULL ? "" : "TRUE") << (mvars->fdata[1]==NULL ? "" : "\n" )
		<< (mvars->fdata[2]==NULL ? "" : "File Data 2: ") << (mvars->fdata[2]==NULL ? "" : "TRUE") << (mvars->fdata[2]==NULL ? "" : "\n" )
		<< (mvars->fdata[3]==NULL ? "" : "File Data 3: ") << (mvars->fdata[3]==NULL ? "" : "TRUE") << (mvars->fdata[3]==NULL ? "" : "\n" )
		<< (mvars->fdata[4]==NULL ? "" : "File Data 4: ") << (mvars->fdata[4]==NULL ? "" : "TRUE") << (mvars->fdata[4]==NULL ? "" : "\n" )
		<< (mvars->fdata[5]==NULL ? "" : "File Data 5: ") << (mvars->fdata[5]==NULL ? "" : "TRUE") << (mvars->fdata[5]==NULL ? "" : "\n" )
		<< (mvars->fdata[6]==NULL ? "" : "File Data 6: ") << (mvars->fdata[6]==NULL ? "" : "TRUE") << (mvars->fdata[6]==NULL ? "" : "\n" )
		<< (mvars->fdata[7]==NULL ? "" : "File Data 7: ") << (mvars->fdata[7]==NULL ? "" : "TRUE") << (mvars->fdata[7]==NULL ? "" : "\n" )
		<< (mvars->fdata[8]==NULL ? "" : "File Data 8: ") << (mvars->fdata[8]==NULL ? "" : "TRUE") << (mvars->fdata[8]==NULL ? "" : "\n" )
		<< (mvars->fdata[9]==NULL ? "" : "File Data 9: ") << (mvars->fdata[9]==NULL ? "" : "TRUE") << (mvars->fdata[9]==NULL ? "" : "\n" )
		<< (mvars->body==NULL ? "" : "Body: ") << (mvars->body==NULL ? "" : mvars->body) << (mvars->body==NULL ? "" : "\n" )
		<< "\"" << endl;
		errstring=errmsg.str();
		//std::cerr << errstring;
#ifdef WIN32
		if(!mvars->ignerr) {
			//system("ok.exe ERRORDIALOG \"ERROR SENDING EMAIL\" \"No Email Recipients Specified...\"");
			system(errstring.c_str());
		}
#else
		if(!mvars->ignerr) {
			//system("ok.lnx ERRORDIALOG \"ERROR SENDING EMAIL\" \"No Email Recipients Specified...\"");
			system(errstring.c_str());
		}
#endif
		return (1); 
	}

	try
	{

		vmime::messageBuilder mb;
		
		ostringstream ostream1;
		if((mvars->from_name==NULL)&&(mvars->from_addr!=NULL)) {
			ostream1 << " <" << mvars->from_addr << ">";
		}else if((mvars->from_name!=NULL)&&(mvars->from_addr!=NULL)) {
			ostream1 << mvars->from_name << " <" << mvars->from_addr << ">";
		}else{	
		}
		//std::cout <<stream1.str() << std::endl;

		// Fill in the basic fields
		mb.setExpeditor(vmime::mailbox(ostream1.str()));

		vmime::addressList from;
		from.appendAddress(vmime::create <vmime::mailbox>(ostream1.str()));
		mb.setRecipients(from);

		ostream1.clear();  // This may be in the wrong place....




		istringstream istream1;
		std::string name;
		vmime::addressList to;
		if(mvars->tolist!=NULL) {
			if((strstr(mvars->tolist,","))==NULL) {
				//std::string tostr = .str();
				to.appendAddress(vmime::create <vmime::mailbox>(mvars->tolist));
				mb.setRecipients(to);
			}else{
				// Needs a for/while loop using strtok or similar...
				//std::string tostr(mvars->tolist);
				//tostr1.erase(std::remove(tostr1.begin(), tostr1.end(), '\n'), tostr1.end());
				//tostr1.erase(std::remove(tostr1.begin(), tostr1.end(), '\r'), tostr1.end());
				//stringstream strstrm(tostr.c_str());
				stringstream strstrm(mvars->tolist);
				char num1[200];
				while(1) {
					strstrm.getline(&num1[0],200,',');
					//cout << num1 << std::endl;
					to.appendAddress(vmime::create <vmime::mailbox>(num1));
					mb.setRecipients(to);
					if((strlen(num1))==0) {
						break;
					}
				}
			}
			istream1.clear();
		}

		// Subject Header
		if(mvars->subject==NULL) {
			mb.setSubject(vmime::text(""));
		}else{
			mb.setSubject(vmime::text(mvars->subject));
		}

		// Message body
		if(mvars->body==NULL) {
			mb.getTextPart()->setText(vmime::create <vmime::stringContentHandler>(""));
		} else if((mvars->body!=NULL)&&(!strcmp(mvars->body,"stdin"))) {
			vmime::string stdin_body((std::istreambuf_iterator<char>(cin)), std::istreambuf_iterator<char>());
			mb.getTextPart()->setText(vmime::create <vmime::stringContentHandler>(stdin_body));
			
		}else{
			mb.getTextPart()->setText(vmime::create <vmime::stringContentHandler>(mvars->body));
		}

		if(mvars->cclist!=NULL) {
			vmime::addressList cc;
			if((strstr(mvars->cclist,","))==NULL) {
				cc.appendAddress(vmime::create <vmime::mailbox>(mvars->cclist));
				mb.setCopyRecipients(cc);
			}else{
				// Needs a for/while loop using strtok or similar...
				stringstream strstrm(mvars->cclist);
				char num1[200];
				while(1) {
					strstrm.getline(&num1[0],200,',');
					//cout << num1 << std::endl;
	
					cc.appendAddress(vmime::create <vmime::mailbox>(num1));
					mb.setCopyRecipients(cc);
	
					if((strlen(num1))==0) {
						break;
					}
				}
			}
		}

		// This section needs to be done for BCC
		if(mvars->bcclist!=NULL) {
			vmime::addressList bcc;
			if((strstr(mvars->bcclist,","))==NULL) {
				bcc.appendAddress(vmime::create <vmime::mailbox>(mvars->bcclist));
				mb.setBlindCopyRecipients(bcc);
			}else{
				// Needs a for/while loop using strtok or similar...
				stringstream strstrm(mvars->bcclist);
				char num1[200];
				while(1) {
					strstrm.getline(&num1[0],200,',');
					//cout << num1 << std::endl;
	
					bcc.appendAddress(vmime::create <vmime::mailbox>(num1));
					mb.setBlindCopyRecipients(bcc);
	
					if((strlen(num1))==0) {
						break;
					}
				}
			}
		}
                
		int x;

		vmime::string s1;
		s1 = "This is a string that really should be the contents of a file.";



		for(x=0;x<10;x++) {
			//TRACE;
			if((mvars->fname[x]!=NULL)&&(!strcmp(mvars->fname[x],"stdin"))) {
				//std::string stdin_str((std::istreambuf_iterator<char>(cin)), std::istreambuf_iterator<char>());
				//vmime::string stdin_str;
				//stdin_str = "This is a string that really should be the contents of a file.";
				//std::cout << "The stdin_str size is: " << stdin_str.length() << endl;
				//ref <utility::inputStream> is = vmime::create <utility::inputStreamStringAdapter>(stdin_str);
#ifdef WIN32
				if (_setmode(_fileno(stdin), _O_BINARY) == -1)
		 			std::cerr << "ERROR: while converting cin to binary:" << strerror(errno) << endl;
#endif
				ref <utility::inputStream> is = vmime::create <utility::inputStreamPointerAdapter>(&cin,true);
				vmime::ref <vmime::fileAttachment> a = vmime::create <vmime::fileAttachment>
				(
					is,                                 
					vmime::word("Report.txt"),                     // default filename 
					vmime::mediaType("application/octet-stream"),  // default content type
					vmime::text("Report.txt")                      // default description
				);
				a->getFileInfo().setFilename("Report.txt");
				mb.appendAttachment(a);
		

			} else if((mvars->fname[x]!=NULL)&&(!strcmp(mvars->fname[x],"stdin"))&&(mvars->dname[x]==NULL)&&(mvars->mtype[x]!=NULL)) {
				if(mvars->rdadiag) { TRACE; }
				//vmime::string stdin_str((std::istreambuf_iterator<char>(cin)), std::istreambuf_iterator<char>());
				//std::cout << "The stdin_str size is: " << stdin_str.length() << endl
				//ref <utility::inputStream> is = vmime::create <utility::inputStreamStringAdapter>(stdin_str);
#ifdef WIN32
				if (_setmode(_fileno(stdin), _O_BINARY) == -1)
					std::cerr << "ERROR: while converting cin to binary:" << strerror(errno) << endl;
#endif
				ref <utility::inputStream> is = vmime::create <utility::inputStreamPointerAdapter>(&cin,true);
				vmime::ref <vmime::fileAttachment> a = vmime::create <vmime::fileAttachment>
				(
					is,                                 
					vmime::word("Report.txt"),          // default filename
					vmime::mediaType(mvars->mtype[x]),  // set content type
					vmime::text("Report.txt")           // default description
				);
				// If the MIME TYPE is application/pdf or similar, the default filename should be changed...
				a->getFileInfo().setFilename("Report.txt");
				mb.appendAttachment(a);
		
			} else if((mvars->fname[x]!=NULL)&&(!strcmp(mvars->fname[x],"stdin"))&&(mvars->dname[x]!=NULL)&&(mvars->mtype[x]==NULL)) {
				if(mvars->rdadiag) { TRACE; }
				//vmime::string stdin_str((std::istreambuf_iterator<char>(cin)), std::istreambuf_iterator<char>());
				//std::cout << "The stdin_str size is: " << strlen(stdin_str.cstr) << endl
				//ref <utility::inputStream> is = vmime::create <utility::inputStreamStringAdapter>(stdin_str);
#ifdef WIN32
				if (_setmode(_fileno(stdin), _O_BINARY) == -1)
					std:cerr << "ERROR: while converting cin to binary:" << strerror(errno) << endl;
#endif
				ref <utility::inputStream> is = vmime::create <utility::inputStreamPointerAdapter>(&cin,true);
				vmime::ref <vmime::fileAttachment> a = vmime::create <vmime::fileAttachment>
				(
					is,                                 
					vmime::word(mvars->dname[x]),                  // specified filename
					vmime::mediaType("application/octet-stream"),  // default content type
					vmime::text(mvars->dname[x])                   // specified description
				);
				a->getFileInfo().setFilename(mvars->dname[x]);
				mb.appendAttachment(a);
		
			} else if((mvars->fname[x]!=NULL)&&(!strcmp(mvars->fname[x],"stdin"))&&(mvars->dname[x]!=NULL)&&(mvars->mtype[x]!=NULL)) {
				if(mvars->rdadiag) { TRACE; }
				//vmime::string stdin_str((std::istreambuf_iterator<char>(cin)), std::istreambuf_iterator<char>());
				//std::cout << "The stdin_str size is: " << stdin_str.length() << endl
				//ref <utility::inputStream> is = vmime::create <utility::inputStreamStringAdapter>(stdin_str);
#ifdef WIN32
				if (_setmode(_fileno(stdin), _O_BINARY) == -1)
					std::cerr << "ERROR: while converting cin to binary:" << strerror(errno) << endl;
#endif
				ref <utility::inputStream> is = vmime::create <utility::inputStreamPointerAdapter>(&cin,true);
				vmime::ref <vmime::fileAttachment> a = vmime::create <vmime::fileAttachment>
				(
					is,                                 
					vmime::word(mvars->dname[x]),       // specified filename
					vmime::mediaType(mvars->mtype[x]),  // set content type
					vmime::text(mvars->dname[x])        // specified description
				);
				a->getFileInfo().setFilename(mvars->dname[x]);
				mb.appendAttachment(a);
		
			}else if((mvars->fname[x]!=NULL)&&(!strcmp(mvars->fname[x],"stdin3"))&&(mvars->dname[x]==NULL)&&(mvars->mtype[x]==NULL)) {
				if(mvars->rdadiag) { TRACE; }
				ref <utility::inputStream> is = vmime::create <utility::inputStreamPointerAdapter>(&cin,true);
				if(mvars->rdadiag) { TRACE; }
				vmime::ref <vmime::fileAttachment> a = vmime::create <vmime::fileAttachment>
				(
					is,                                 
					vmime::word("Report3.txt"),
					vmime::mediaType("application/octet-stream"),    // content type
					vmime::text("Report3.txt")                       // description
				);
				if(mvars->rdadiag) { TRACE; }

/*
				vmime::ref <vmime::fileAttachment> a = vmime::create <vmime::fileAttachment>
				(
					"/proc/self/fd/0",                             // linux cheat to read stdin using file name    
					vmime::mediaType("application/octet-stream"),  // content type
					vmime::text("Report.txt")                      // description
				);
*/
				if(mvars->rdadiag) { TRACE; }
				a->getFileInfo().setFilename("Report.txt");
				if(mvars->rdadiag) { TRACE; }
				mb.appendAttachment(a);
				if(mvars->rdadiag) { TRACE; }
			}else if((mvars->fname[x]!=NULL)&&(!strcmp(mvars->fname[x],"stdin2"))&&(mvars->dname[x]==NULL)&&(mvars->mtype[x]==NULL)) {
				if(mvars->rdadiag) { TRACE; }
				std::string stdin_str((std::istreambuf_iterator<char>(cin)), std::istreambuf_iterator<char>());
				ref <utility::inputStream> is = vmime::create <utility::inputStreamStringAdapter>(stdin_str);
				//ref <utility::inputStream> is = vmime::create <utility::inputStreamStringAdapter>(mvars->fdata[x]);
				//ref <utility::inputStream> is = vmime::create <utility::inputStreamStringAdapter>(s1);
				vmime::ref <vmime::fileAttachment> a = vmime::create <vmime::fileAttachment>
				(
					is,                                 
					vmime::word("Report.txt"),
					vmime::mediaType("application/octet-stream"),  // content type
					vmime::text("Report.txt")                      // description
				);
				a->getFileInfo().setFilename("Report.txt");
				mb.appendAttachment(a);
			}else if((mvars->fname[x]!=NULL)&&(mvars->mtype[x]!=NULL)&&(mvars->dname[x]!=NULL)) {
				if(mvars->rdadiag) { TRACE; }
				vmime::ref <vmime::fileAttachment> a = vmime::create <vmime::fileAttachment>
				(
					mvars->fname[x],                    // full path to file
					vmime::mediaType(mvars->mtype[x]),  // content type
					vmime::text(mvars->dname[x])        // description
				);
				a->getFileInfo().setFilename(mvars->dname[x]);
				//a->getFileInfo().setCreationDate(vmime::datetime("30 Apr 2003 14:30:00 +0200"));
				//mb.attach(a);
				mb.appendAttachment(a);
			}else if((mvars->fname[x]!=NULL)&&(mvars->mtype[x]==NULL)&&(mvars->dname[x]!=NULL)) {
				if(mvars->rdadiag) { 
					TRACE; 
					std::cerr << "FNAME " << mvars->fname[x] <<endl;
					std::cerr << "DNAME " << mvars->dname[x] <<endl;
				}
				vmime::ref <vmime::fileAttachment> a = vmime::create <vmime::fileAttachment>
				(
					mvars->fname[x], vmime::mediaType("application/octet-stream"), vmime::text(mvars->dname[x])        
				);
				a->getFileInfo().setFilename(mvars->dname[x]);
				mb.appendAttachment(a);
			}else if((mvars->fname[x]!=NULL)&&(mvars->mtype[x]==NULL)&&(mvars->dname[x]==NULL)) {
				if(mvars->rdadiag) { TRACE; }
				// Need a cross platform basename function used on fname before setting attachment name to it.
				vmime::ref <vmime::fileAttachment> a = vmime::create <vmime::fileAttachment>
				(
					mvars->fname[x], vmime::mediaType("application/octet-stream"), vmime::text(mvars->fname[x])        
				);
				a->getFileInfo().setFilename(mvars->fname[x]);
				mb.appendAttachment(a);
			}else if((mvars->fname[x]!=NULL)&&(mvars->mtype[x]!=NULL)&&(mvars->dname[x]==NULL)) {
				if(mvars->rdadiag) { TRACE; }
				// Need a cross platform basename function used on fname before setting attachment name to it.
				vmime::ref <vmime::fileAttachment> a = vmime::create <vmime::fileAttachment>
				(
					mvars->fname[x], vmime::mediaType(mvars->mtype[x]), vmime::text(mvars->fname[x])        
				);
				a->getFileInfo().setFilename(mvars->fname[x]);
				mb.appendAttachment(a);

/*
			}else{
				TRACE;
				std::cout << "ELSE condition of add attachment.  Should never reach here." <<endl ;
*/
			}

		}


		// Email Construction
		vmime::ref <vmime::message> msg = mb.construct();

		// Add reply to header if value exists.
		if(mvars->replyto!=NULL)
		{
			vmime::ref <vmime::header> hdr = msg->getHeader();
			vmime::headerFieldFactory *hfFactory = vmime::headerFieldFactory::getInstance();		
			vmime::ref <vmime::headerField> ReplyToField =  hfFactory->create(vmime::fields::REPLY_TO);
			ReplyToField->setValue(vmime::text (mvars->replyto));
			hdr->appendField(ReplyToField);
		}

		// Raw text generation
		const vmime::string dataToSend = msg->generate();

		// Print it to stdout for testing
		// ostream1 <<  dataToSend ;
		FILE *pipe=NULL;
#ifdef WIN32
		HANDLE *procinfo;
		pipe=win32_popen(&procinfo,"msmtp.exe -t","wb");
		bufferwrite(pipe,dataToSend.c_str(),(strlen(dataToSend.c_str())),65535);
		//fclose(pipe);
		//WaitForSingleObject(procinfo,INFINITE);
		win32_pclose(&procinfo,pipe);
		
		// win32_plose is broken.  Does not wait on passed object...
		// using fclose and WaitForSingleObject works in parent...
		//win32_pclose(&procinfo,pipe);


		//system("msmtp.exe -d -t <C:\\email.txt");
		//RunSilent("msmtp.exe -t <C:\\email.txt");
		//system_command("msmtp.exe -d -t <C:\\email.txt");
		//RunSilent("msmtp.exe -d -t <\\\\pipe\\SamplePipe");
#else
		char const *approot=getenv("VM_HOST_ROOT");
		if(approot!=NULL) 
		{
			std::string msmtpcmd("msmtp -C ");
			msmtpcmd.append(approot);
			msmtpcmd.append("/approot/msmtprc -X ");
			msmtpcmd.append(approot);
			msmtpcmd.append("/approot/openrda/");
			if(mvars->srcuser!=NULL)
			{
				msmtpcmd.append(mvars->srcuser);
				msmtpcmd.append("/msmtp.log -t");
			}else{
				msmtpcmd.append("/unknown-msmtp.log -t");
			}
			pipe=popen(msmtpcmd.c_str(),"w");
		}else{
			pipe=popen("msmtp -t","w");
		}
		if(pipe==NULL) {
			cerr <<std::endl <<"ERROR SENDING EMAIL: MSMTP Failed.  Ensure MSMTP Is Installed..." <<std::endl;
			if(!mvars->ignerr) {
				system("ok.lnx ERRORDIALOG \"ERROR SENDING EMAIL\" \"MSMTP Failed.  Ensure MSMTP Is Installed...\"");
			}
		}else{
			fwrite(dataToSend.c_str(),(strlen(dataToSend.c_str())),1,pipe);
		}

		if(mvars->rdadiag) { 
			cerr <<std::endl <<std::endl <<dataToSend.c_str() <<std::endl <<std::endl;
		}
		if(pipe!=NULL) { pclose(pipe); }
		DelayAfterSending();
		
		/* Crash is coming from the implicit destructor when this function returns
		TRACE;
		~msg( );
		TRACE;
		*/

#endif
		return(0);
		//fwrite(ostream1.str(),(strlen(ostream1.str())),1,pipe);
		//strcpy(firstName, my_string.c_str());
		//std::cout << "strlen is " << strlen(ostream1.str()) std::endl;
	
		
		//std::cout << ostream1.str() << std::endl;
		//std::cout << dataToSend << std::endl;

	}
	// VMime exception
	catch (vmime::exception& e)
	{
		std::cerr << "vmime::exception: " << e.what() << std::endl;
		ostringstream verrmsg;
		std::string verrstring;
#ifdef WIN32
		verrmsg << "ok.exe ERRORDIALOG \"ERROR SENDING EMAIL\" \"vmime::exception: " << e.what() << endl << endl << endl << endl
#else
		verrmsg << "ok.lnx ERRORDIALOG \"ERROR SENDING EMAIL\" \"vmime::exception: " << e.what() << endl << endl << endl << endl
#endif
		<< (mvars->from_name==NULL ? "" : "From Name: ") << (mvars->from_name==NULL ? "" : mvars->from_name) << (mvars->from_name==NULL ? "" : "\n" )
		<< (mvars->from_addr==NULL ? "" : "From Address: ") << (mvars->from_addr==NULL ? "" : mvars->from_addr) << (mvars->from_addr==NULL ? "" : "\n" )
		<< (mvars->tolist==NULL ? "" : "TO: ") << (mvars->tolist==NULL ? "" : mvars->tolist) << (mvars->tolist==NULL ? "" : "\n" )
		<< (mvars->cclist==NULL ? "" : "CC: ") << (mvars->cclist==NULL ? "" : mvars->cclist) << (mvars->cclist==NULL ? "" : "\n" )
		<< (mvars->bcclist==NULL ? "" : "BCC: ") << (mvars->bcclist==NULL ? "" : mvars->bcclist) << (mvars->bcclist==NULL ? "" : "\n" )
		<< (mvars->subject==NULL ? "" : "Subject: ") << (mvars->subject==NULL ? "" : mvars->subject) << (mvars->subject==NULL ? "" : "\n" )
		<< (mvars->fname[0]==NULL ? "" : "Filename 0: ") << (mvars->fname[0]==NULL ? "" : mvars->fname[0]) << (mvars->fname[0]==NULL ? "" : "\n" )
		<< (mvars->fname[1]==NULL ? "" : "Filename 1: ") << (mvars->fname[1]==NULL ? "" : mvars->fname[1]) << (mvars->fname[1]==NULL ? "" : "\n" )
		<< (mvars->fname[2]==NULL ? "" : "Filename 2: ") << (mvars->fname[2]==NULL ? "" : mvars->fname[2]) << (mvars->fname[2]==NULL ? "" : "\n" )
		<< (mvars->fname[3]==NULL ? "" : "Filename 3: ") << (mvars->fname[3]==NULL ? "" : mvars->fname[3]) << (mvars->fname[3]==NULL ? "" : "\n" )
		<< (mvars->fname[4]==NULL ? "" : "Filename 4: ") << (mvars->fname[4]==NULL ? "" : mvars->fname[4]) << (mvars->fname[4]==NULL ? "" : "\n" )
		<< (mvars->fname[5]==NULL ? "" : "Filename 5: ") << (mvars->fname[5]==NULL ? "" : mvars->fname[5]) << (mvars->fname[5]==NULL ? "" : "\n" )
		<< (mvars->fname[6]==NULL ? "" : "Filename 6: ") << (mvars->fname[6]==NULL ? "" : mvars->fname[6]) << (mvars->fname[6]==NULL ? "" : "\n" )
		<< (mvars->fname[7]==NULL ? "" : "Filename 7: ") << (mvars->fname[7]==NULL ? "" : mvars->fname[7]) << (mvars->fname[7]==NULL ? "" : "\n" )
		<< (mvars->fname[8]==NULL ? "" : "Filename 8: ") << (mvars->fname[8]==NULL ? "" : mvars->fname[8]) << (mvars->fname[8]==NULL ? "" : "\n" )
		<< (mvars->fname[9]==NULL ? "" : "Filename 9: ") << (mvars->fname[9]==NULL ? "" : mvars->fname[9]) << (mvars->fname[9]==NULL ? "" : "\n" )
		<< (mvars->dname[0]==NULL ? "" : "Attachment 0: ") << (mvars->dname[0]==NULL ? "" : mvars->dname[0]) << (mvars->dname[0]==NULL ? "" : "\n" )
		<< (mvars->dname[1]==NULL ? "" : "Attachment 1: ") << (mvars->dname[1]==NULL ? "" : mvars->dname[1]) << (mvars->dname[1]==NULL ? "" : "\n" )
		<< (mvars->dname[2]==NULL ? "" : "Attachment 2: ") << (mvars->dname[2]==NULL ? "" : mvars->dname[2]) << (mvars->dname[2]==NULL ? "" : "\n" )
		<< (mvars->dname[3]==NULL ? "" : "Attachment 3: ") << (mvars->dname[3]==NULL ? "" : mvars->dname[3]) << (mvars->dname[3]==NULL ? "" : "\n" )
		<< (mvars->dname[4]==NULL ? "" : "Attachment 4: ") << (mvars->dname[4]==NULL ? "" : mvars->dname[4]) << (mvars->dname[4]==NULL ? "" : "\n" )
		<< (mvars->dname[5]==NULL ? "" : "Attachment 5: ") << (mvars->dname[5]==NULL ? "" : mvars->dname[5]) << (mvars->dname[5]==NULL ? "" : "\n" )
		<< (mvars->dname[6]==NULL ? "" : "Attachment 6: ") << (mvars->dname[6]==NULL ? "" : mvars->dname[6]) << (mvars->dname[6]==NULL ? "" : "\n" )
		<< (mvars->dname[7]==NULL ? "" : "Attachment 7: ") << (mvars->dname[7]==NULL ? "" : mvars->dname[7]) << (mvars->dname[7]==NULL ? "" : "\n" )
		<< (mvars->dname[8]==NULL ? "" : "Attachment 8: ") << (mvars->dname[8]==NULL ? "" : mvars->dname[8]) << (mvars->dname[8]==NULL ? "" : "\n" )
		<< (mvars->dname[9]==NULL ? "" : "Attachment 9: ") << (mvars->dname[9]==NULL ? "" : mvars->dname[9]) << (mvars->dname[9]==NULL ? "" : "\n" )
		<< (mvars->mtype[0]==NULL ? "" : "MimeType 0: ") << (mvars->mtype[0]==NULL ? "" : mvars->mtype[0]) << (mvars->mtype[0]==NULL ? "" : "\n" )
		<< (mvars->mtype[1]==NULL ? "" : "MimeType 1: ") << (mvars->mtype[1]==NULL ? "" : mvars->mtype[1]) << (mvars->mtype[1]==NULL ? "" : "\n" )
		<< (mvars->mtype[2]==NULL ? "" : "MimeType 2: ") << (mvars->mtype[2]==NULL ? "" : mvars->mtype[2]) << (mvars->mtype[2]==NULL ? "" : "\n" )
		<< (mvars->mtype[3]==NULL ? "" : "MimeType 3: ") << (mvars->mtype[3]==NULL ? "" : mvars->mtype[3]) << (mvars->mtype[3]==NULL ? "" : "\n" )
		<< (mvars->mtype[4]==NULL ? "" : "MimeType 4: ") << (mvars->mtype[4]==NULL ? "" : mvars->mtype[4]) << (mvars->mtype[4]==NULL ? "" : "\n" )
		<< (mvars->mtype[5]==NULL ? "" : "MimeType 5: ") << (mvars->mtype[5]==NULL ? "" : mvars->mtype[5]) << (mvars->mtype[5]==NULL ? "" : "\n" )
		<< (mvars->mtype[6]==NULL ? "" : "MimeType 6: ") << (mvars->mtype[6]==NULL ? "" : mvars->mtype[6]) << (mvars->mtype[6]==NULL ? "" : "\n" )
		<< (mvars->mtype[7]==NULL ? "" : "MimeType 7: ") << (mvars->mtype[7]==NULL ? "" : mvars->mtype[7]) << (mvars->mtype[7]==NULL ? "" : "\n" )
		<< (mvars->mtype[8]==NULL ? "" : "MimeType 8: ") << (mvars->mtype[8]==NULL ? "" : mvars->mtype[8]) << (mvars->mtype[8]==NULL ? "" : "\n" )
		<< (mvars->mtype[9]==NULL ? "" : "MimeType 9: ") << (mvars->mtype[9]==NULL ? "" : mvars->mtype[9]) << (mvars->mtype[9]==NULL ? "" : "\n" )
		<< (mvars->fdata[0]==NULL ? "" : "File Data 0: ") << (mvars->fdata[0]==NULL ? "" : "TRUE") << (mvars->fdata[0]==NULL ? "" : "\n" )
		<< (mvars->fdata[1]==NULL ? "" : "File Data 1: ") << (mvars->fdata[1]==NULL ? "" : "TRUE") << (mvars->fdata[1]==NULL ? "" : "\n" )
		<< (mvars->fdata[2]==NULL ? "" : "File Data 2: ") << (mvars->fdata[2]==NULL ? "" : "TRUE") << (mvars->fdata[2]==NULL ? "" : "\n" )
		<< (mvars->fdata[3]==NULL ? "" : "File Data 3: ") << (mvars->fdata[3]==NULL ? "" : "TRUE") << (mvars->fdata[3]==NULL ? "" : "\n" )
		<< (mvars->fdata[4]==NULL ? "" : "File Data 4: ") << (mvars->fdata[4]==NULL ? "" : "TRUE") << (mvars->fdata[4]==NULL ? "" : "\n" )
		<< (mvars->fdata[5]==NULL ? "" : "File Data 5: ") << (mvars->fdata[5]==NULL ? "" : "TRUE") << (mvars->fdata[5]==NULL ? "" : "\n" )
		<< (mvars->fdata[6]==NULL ? "" : "File Data 6: ") << (mvars->fdata[6]==NULL ? "" : "TRUE") << (mvars->fdata[6]==NULL ? "" : "\n" )
		<< (mvars->fdata[7]==NULL ? "" : "File Data 7: ") << (mvars->fdata[7]==NULL ? "" : "TRUE") << (mvars->fdata[7]==NULL ? "" : "\n" )
		<< (mvars->fdata[8]==NULL ? "" : "File Data 8: ") << (mvars->fdata[8]==NULL ? "" : "TRUE") << (mvars->fdata[8]==NULL ? "" : "\n" )
		<< (mvars->fdata[9]==NULL ? "" : "File Data 9: ") << (mvars->fdata[9]==NULL ? "" : "TRUE") << (mvars->fdata[9]==NULL ? "" : "\n" )
		<< (mvars->body==NULL ? "" : "Body: ") << (mvars->body==NULL ? "" : mvars->body) << (mvars->body==NULL ? "" : "\n" )
		<< "\"" << endl;
		verrstring=verrmsg.str();
		//std::cerr << errstring;
#ifdef WIN32
		if(!mvars->ignerr) {
			//system("ok.exe ERRORDIALOG \"ERROR SENDING EMAIL\" \"No Email Recipients Specified...\"");
			system(verrstring.c_str());
		}
#else
		if(!mvars->ignerr) {
			//system("ok.lnx ERRORDIALOG \"ERROR SENDING EMAIL\" \"No Email Recipients Specified...\"");
			system(verrstring.c_str());
		}
#endif
		return(1);
		throw;
	}
	// Standard exception
	catch (std::exception& e)
	{
		std::cerr << "std::exception: " << e.what() << std::endl;
		ostringstream serrmsg;
		std::string serrstring;
#ifdef WIN32
		serrmsg << "ok.exe ERRORDIALOG \"ERROR SENDING EMAIL\" \"std::exception: " << e.what() << endl << endl << endl << endl
#else
		serrmsg << "ok.lnx ERRORDIALOG \"ERROR SENDING EMAIL\" \"std::exception: " << e.what() << endl << endl << endl << endl
#endif
		<< (mvars->from_name==NULL ? "" : "From Name: ") << (mvars->from_name==NULL ? "" : mvars->from_name) << (mvars->from_name==NULL ? "" : "\n" )
		<< (mvars->from_addr==NULL ? "" : "From Address: ") << (mvars->from_addr==NULL ? "" : mvars->from_addr) << (mvars->from_addr==NULL ? "" : "\n" )
		<< (mvars->tolist==NULL ? "" : "TO: ") << (mvars->tolist==NULL ? "" : mvars->tolist) << (mvars->tolist==NULL ? "" : "\n" )
		<< (mvars->cclist==NULL ? "" : "CC: ") << (mvars->cclist==NULL ? "" : mvars->cclist) << (mvars->cclist==NULL ? "" : "\n" )
		<< (mvars->bcclist==NULL ? "" : "BCC: ") << (mvars->bcclist==NULL ? "" : mvars->bcclist) << (mvars->bcclist==NULL ? "" : "\n" )
		<< (mvars->subject==NULL ? "" : "Subject: ") << (mvars->subject==NULL ? "" : mvars->subject) << (mvars->subject==NULL ? "" : "\n" )
		<< (mvars->fname[0]==NULL ? "" : "Filename 0: ") << (mvars->fname[0]==NULL ? "" : mvars->fname[0]) << (mvars->fname[0]==NULL ? "" : "\n" )
		<< (mvars->fname[1]==NULL ? "" : "Filename 1: ") << (mvars->fname[1]==NULL ? "" : mvars->fname[1]) << (mvars->fname[1]==NULL ? "" : "\n" )
		<< (mvars->fname[2]==NULL ? "" : "Filename 2: ") << (mvars->fname[2]==NULL ? "" : mvars->fname[2]) << (mvars->fname[2]==NULL ? "" : "\n" )
		<< (mvars->fname[3]==NULL ? "" : "Filename 3: ") << (mvars->fname[3]==NULL ? "" : mvars->fname[3]) << (mvars->fname[3]==NULL ? "" : "\n" )
		<< (mvars->fname[4]==NULL ? "" : "Filename 4: ") << (mvars->fname[4]==NULL ? "" : mvars->fname[4]) << (mvars->fname[4]==NULL ? "" : "\n" )
		<< (mvars->fname[5]==NULL ? "" : "Filename 5: ") << (mvars->fname[5]==NULL ? "" : mvars->fname[5]) << (mvars->fname[5]==NULL ? "" : "\n" )
		<< (mvars->fname[6]==NULL ? "" : "Filename 6: ") << (mvars->fname[6]==NULL ? "" : mvars->fname[6]) << (mvars->fname[6]==NULL ? "" : "\n" )
		<< (mvars->fname[7]==NULL ? "" : "Filename 7: ") << (mvars->fname[7]==NULL ? "" : mvars->fname[7]) << (mvars->fname[7]==NULL ? "" : "\n" )
		<< (mvars->fname[8]==NULL ? "" : "Filename 8: ") << (mvars->fname[8]==NULL ? "" : mvars->fname[8]) << (mvars->fname[8]==NULL ? "" : "\n" )
		<< (mvars->fname[9]==NULL ? "" : "Filename 9: ") << (mvars->fname[9]==NULL ? "" : mvars->fname[9]) << (mvars->fname[9]==NULL ? "" : "\n" )
		<< (mvars->dname[0]==NULL ? "" : "Attachment 0: ") << (mvars->dname[0]==NULL ? "" : mvars->dname[0]) << (mvars->dname[0]==NULL ? "" : "\n" )
		<< (mvars->dname[1]==NULL ? "" : "Attachment 1: ") << (mvars->dname[1]==NULL ? "" : mvars->dname[1]) << (mvars->dname[1]==NULL ? "" : "\n" )
		<< (mvars->dname[2]==NULL ? "" : "Attachment 2: ") << (mvars->dname[2]==NULL ? "" : mvars->dname[2]) << (mvars->dname[2]==NULL ? "" : "\n" )
		<< (mvars->dname[3]==NULL ? "" : "Attachment 3: ") << (mvars->dname[3]==NULL ? "" : mvars->dname[3]) << (mvars->dname[3]==NULL ? "" : "\n" )
		<< (mvars->dname[4]==NULL ? "" : "Attachment 4: ") << (mvars->dname[4]==NULL ? "" : mvars->dname[4]) << (mvars->dname[4]==NULL ? "" : "\n" )
		<< (mvars->dname[5]==NULL ? "" : "Attachment 5: ") << (mvars->dname[5]==NULL ? "" : mvars->dname[5]) << (mvars->dname[5]==NULL ? "" : "\n" )
		<< (mvars->dname[6]==NULL ? "" : "Attachment 6: ") << (mvars->dname[6]==NULL ? "" : mvars->dname[6]) << (mvars->dname[6]==NULL ? "" : "\n" )
		<< (mvars->dname[7]==NULL ? "" : "Attachment 7: ") << (mvars->dname[7]==NULL ? "" : mvars->dname[7]) << (mvars->dname[7]==NULL ? "" : "\n" )
		<< (mvars->dname[8]==NULL ? "" : "Attachment 8: ") << (mvars->dname[8]==NULL ? "" : mvars->dname[8]) << (mvars->dname[8]==NULL ? "" : "\n" )
		<< (mvars->dname[9]==NULL ? "" : "Attachment 9: ") << (mvars->dname[9]==NULL ? "" : mvars->dname[9]) << (mvars->dname[9]==NULL ? "" : "\n" )
		<< (mvars->mtype[0]==NULL ? "" : "MimeType 0: ") << (mvars->mtype[0]==NULL ? "" : mvars->mtype[0]) << (mvars->mtype[0]==NULL ? "" : "\n" )
		<< (mvars->mtype[1]==NULL ? "" : "MimeType 1: ") << (mvars->mtype[1]==NULL ? "" : mvars->mtype[1]) << (mvars->mtype[1]==NULL ? "" : "\n" )
		<< (mvars->mtype[2]==NULL ? "" : "MimeType 2: ") << (mvars->mtype[2]==NULL ? "" : mvars->mtype[2]) << (mvars->mtype[2]==NULL ? "" : "\n" )
		<< (mvars->mtype[3]==NULL ? "" : "MimeType 3: ") << (mvars->mtype[3]==NULL ? "" : mvars->mtype[3]) << (mvars->mtype[3]==NULL ? "" : "\n" )
		<< (mvars->mtype[4]==NULL ? "" : "MimeType 4: ") << (mvars->mtype[4]==NULL ? "" : mvars->mtype[4]) << (mvars->mtype[4]==NULL ? "" : "\n" )
		<< (mvars->mtype[5]==NULL ? "" : "MimeType 5: ") << (mvars->mtype[5]==NULL ? "" : mvars->mtype[5]) << (mvars->mtype[5]==NULL ? "" : "\n" )
		<< (mvars->mtype[6]==NULL ? "" : "MimeType 6: ") << (mvars->mtype[6]==NULL ? "" : mvars->mtype[6]) << (mvars->mtype[6]==NULL ? "" : "\n" )
		<< (mvars->mtype[7]==NULL ? "" : "MimeType 7: ") << (mvars->mtype[7]==NULL ? "" : mvars->mtype[7]) << (mvars->mtype[7]==NULL ? "" : "\n" )
		<< (mvars->mtype[8]==NULL ? "" : "MimeType 8: ") << (mvars->mtype[8]==NULL ? "" : mvars->mtype[8]) << (mvars->mtype[8]==NULL ? "" : "\n" )
		<< (mvars->mtype[9]==NULL ? "" : "MimeType 9: ") << (mvars->mtype[9]==NULL ? "" : mvars->mtype[9]) << (mvars->mtype[9]==NULL ? "" : "\n" )
		<< (mvars->fdata[0]==NULL ? "" : "File Data 0: ") << (mvars->fdata[0]==NULL ? "" : "TRUE") << (mvars->fdata[0]==NULL ? "" : "\n" )
		<< (mvars->fdata[1]==NULL ? "" : "File Data 1: ") << (mvars->fdata[1]==NULL ? "" : "TRUE") << (mvars->fdata[1]==NULL ? "" : "\n" )
		<< (mvars->fdata[2]==NULL ? "" : "File Data 2: ") << (mvars->fdata[2]==NULL ? "" : "TRUE") << (mvars->fdata[2]==NULL ? "" : "\n" )
		<< (mvars->fdata[3]==NULL ? "" : "File Data 3: ") << (mvars->fdata[3]==NULL ? "" : "TRUE") << (mvars->fdata[3]==NULL ? "" : "\n" )
		<< (mvars->fdata[4]==NULL ? "" : "File Data 4: ") << (mvars->fdata[4]==NULL ? "" : "TRUE") << (mvars->fdata[4]==NULL ? "" : "\n" )
		<< (mvars->fdata[5]==NULL ? "" : "File Data 5: ") << (mvars->fdata[5]==NULL ? "" : "TRUE") << (mvars->fdata[5]==NULL ? "" : "\n" )
		<< (mvars->fdata[6]==NULL ? "" : "File Data 6: ") << (mvars->fdata[6]==NULL ? "" : "TRUE") << (mvars->fdata[6]==NULL ? "" : "\n" )
		<< (mvars->fdata[7]==NULL ? "" : "File Data 7: ") << (mvars->fdata[7]==NULL ? "" : "TRUE") << (mvars->fdata[7]==NULL ? "" : "\n" )
		<< (mvars->fdata[8]==NULL ? "" : "File Data 8: ") << (mvars->fdata[8]==NULL ? "" : "TRUE") << (mvars->fdata[8]==NULL ? "" : "\n" )
		<< (mvars->fdata[9]==NULL ? "" : "File Data 9: ") << (mvars->fdata[9]==NULL ? "" : "TRUE") << (mvars->fdata[9]==NULL ? "" : "\n" )
		<< (mvars->body==NULL ? "" : "Body: ") << (mvars->body==NULL ? "" : mvars->body) << (mvars->body==NULL ? "" : "\n" )
		<< "\"" << endl;
		serrstring=serrmsg.str();
		//std::cerr << errstring;
#ifdef WIN32
		if(!mvars->ignerr) {
			//system("ok.exe ERRORDIALOG \"ERROR SENDING EMAIL\" \"No Email Recipients Specified...\"");
			system(serrstring.c_str());
		}
#else
		if(!mvars->ignerr) {
			//system("ok.lnx ERRORDIALOG \"ERROR SENDING EMAIL\" \"No Email Recipients Specified...\"");
			system(serrstring.c_str());
		}
#endif
		return(1);
		throw;
	}
	return(0);
}

#ifdef WIN32
#ifndef WIN32_CONSOLE
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	int argc=0;
	char **argv=NULL;
	argv=CommandLineToArgvA(lpCmdLine,&argc);
	main(argc,argv);
}
#endif
#endif
int main(int argc, char* argv[])
{
	int x;
	int ret;
	char stemp[512];
	char *rdadiag=NULL;
	char *temp1=NULL;
	MAIL_VARS *mvars = new MAIL_VARS;

	//mvars->cclist = new char[((strlen(argv[1]))+1)];

	//mvars->cclist=malloc((1+(strlen(argv[1])))*sizeof(char *));
	

	rdadiag=getenv("RDADIAG2");
	if((rdadiag!=NULL)&&(strstr(rdadiag,"MISC"))) {
		mvars->rdadiag=1;
		cerr << "RDA EMAIL DIAGNOSTICS ENABLED" << endl;
	}else{
		mvars->rdadiag=0;
	}
		

	mvars->from_name=NULL;
	mvars->from_addr=NULL;
	mvars->subject=NULL;
	mvars->body=NULL;
	mvars->tolist=NULL;
	mvars->cclist=NULL;
	mvars->bcclist=NULL;
	mvars->replyto=NULL;
	mvars->srcuser=NULL;
	mvars->ignerr=0;
	
	//  Need some type of arrays or pointers to hold filename, file type, display name in email, and maybe even the actual file data in memeory...
	for(x=0;x<10;x++) {
		
		//std::cout << "x:" << x << std::endl;
		mvars->fname[x]=NULL;	
		mvars->mtype[x]=NULL;	
		mvars->dname[x]=NULL;	
		mvars->fdata[x]=NULL;	
	}

	temp1=getEnvVar("VMIME_SRCDOMAIN");
	mvars->srcuser=getEnvVar("VMIME_LOGIN_ID");

	if((temp1!=NULL)&&(mvars->srcuser!=NULL))
	{
		memset(stemp,0,512);
		sprintf(stemp,"%s@%s",mvars->srcuser,temp1);
		delete temp1;

		mvars->from_name=getEnvVar("VMIME_FROM_NAME");
		mvars->from_addr=new char[(strlen(stemp))+1];
		strcpy(mvars->from_addr,stemp);
		mvars->subject=getEnvVar("VMIME_SUBJECT");
		mvars->body=getenv("VMIME_BODY");
		mvars->tolist=getEnvVar("VMIME_TO");
		mvars->cclist=getEnvVar("VMIME_CC");
		mvars->bcclist=getEnvVar("VMIME_BCC");
		mvars->replyto=getEnvVar("VMIME_FROM_ADDR");
	}else{
		mvars->from_name=getEnvVar("VMIME_FROM_NAME");
		mvars->from_addr=getEnvVar("VMIME_FROM_ADDR");
		mvars->subject=getEnvVar("VMIME_SUBJECT");
		mvars->body=getenv("VMIME_BODY");
		mvars->tolist=getEnvVar("VMIME_TO");
		mvars->cclist=getEnvVar("VMIME_CC");
		mvars->bcclist=getEnvVar("VMIME_BCC");
	}

//	mvars->fname[0] = new char[50];
//	mvars->mtype[0] = new char[50];
//	mvars->dname[0] = new char[50];
//	sprintf(mvars->fname[0],"%s","/etc/hosts");	
//	sprintf(mvars->mtype[0],"%s","application/octet-stream");	
//	sprintf(mvars->dname[0],"%s","hosts");	

	for(x=0;x<10;x++) {
		memset(stemp,0,512);
		sprintf(stemp,"VMIME_FNAME%d",x);
		mvars->fname[x]=getenv(stemp);	
		memset(stemp,0,512);
		sprintf(stemp,"VMIME_MTYPE%d",x);
		mvars->mtype[x]=getenv(stemp);	
		memset(stemp,0,512);
		sprintf(stemp,"VMIME_DNAME%d",x);
		mvars->dname[x]=getenv(stemp);	
		memset(stemp,0,512);
		sprintf(stemp,"VMIME_FDATA%d",x);
		mvars->fdata[x]=getenv(stemp);	
	}
	ret=0;
	ret=sendattach(mvars);
	//delete mvars;



/*
	std::cout << "TRACE:" << __LINE__  << std::endl;
	memset(mvars->cclist,0,(1+(strlen(argv[1]))));
	std::cout << "TRACE:" << __LINE__  << std::endl;
	sprintf(mvars->cclist,"%s",argv[1]);
	std::cout << "TRACE:" << __LINE__  << std::endl;

	std::cout << "TRACE:" << __LINE__  << std::endl;
	std::cout << "cclist contains:" << mvars->cclist << std::endl;
	std::cout << "TRACE:" << __LINE__  << std::endl;

	std::cout << "TRACE:" << __LINE__  << std::endl;
	if((strstr(argv[0],"sendmail"))!=NULL) {
	
		std::cout << "strstr NULL..." << std::endl;
		std::cout << argv[0] << std::endl;
		if(argc==7) {
			sendattach2_6(argc,argv);
		}else if(argc==3) {
			std::cout << "sendmail2_2." << std::endl;
			sendattach2_2(argc,argv);
		} else {
			std::cout << "sendattach2 wrong number of arguments: " << argc <<"." << std::endl
			<<"argv0=" <<argv[0] << std::endl
			<<"argv1=" <<argv[1] << std::endl
			<<"argv2=" <<argv[2] << std::endl
			<<"argv3=" <<argv[3] << std::endl
			<<"argv4=" <<argv[4] << std::endl
			<<"argv5=" <<argv[5] << std::endl
			<<"argv6=" <<argv[6] << std::endl
			<<"argv7=" <<argv[7] << std::endl
			  << std::endl;
		}
		
	} else {
		std::cout << "strstr NOT NULL..." << std::endl;
	}
	//vmime::sendmailTransport::m_sendmailPath
	sendmailServiceInfos x;

*/
	return(ret);
}



