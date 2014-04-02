#include <misc.hpp>
#include <gui.hpp>
#include <guip.hpp>
#include <nrd.hpp>
#include <olh.hpp>
#include <mix.hpp>
#include <curl/curl.h>
#include <curl/easy.h>

struct MemoryStruct {
	char *memory;
	size_t size;
};
size_t WriteMemoryCallback(void *ptr,size_t size,size_t nmemb,void *data)
{
	register int realsize = size*nmemb;
	struct MemoryStruct *mem=(struct MemoryStruct *)data;

	mem->memory=(char *)realloc(mem->memory,mem->size+realsize+1);
	if(mem->memory) 
	{
		memcpy(&(mem->memory[mem->size]),ptr,realsize);
		mem->size+=realsize;
		mem->memory[mem->size]=0;
	}
	return(realsize);
}
Wt::WContainerWidget *CreateFeed(char *url)
{
	Wt::WContainerWidget *RDA_Feed=NULL;
	Wt::WVBoxLayout *vbox=NULL;
	Wt::WLength spc;
	CURL *RDAcurl=NULL;
	struct MemoryStruct chunk;
	Wt::WTextEdit *myTE=NULL;
	char *temp3=NULL;
	Wt::WString *s=NULL;

	RDA_Feed = new Wt::WContainerWidget();
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	RDA_Feed->setPadding(spc,All);
	vbox=new Wt::WVBoxLayout();
	vbox->setContentsMargins(0,0,0,0);
	RDA_Feed->setLayout(vbox);
	chunk.memory=NULL;
	chunk.size=0;
	RDAcurl=curl_easy_init();
	if(RDAcurl)
	{
		curl_easy_setopt(RDAcurl,CURLOPT_URL,url);
		curl_easy_setopt(RDAcurl,CURLOPT_WRITEFUNCTION,WriteMemoryCallback);
		curl_easy_setopt(RDAcurl,CURLOPT_FILE,(void *)&chunk);
		curl_easy_setopt(RDAcurl,CURLOPT_VERBOSE,FALSE);
		curl_easy_perform(RDAcurl);
		curl_easy_cleanup(RDAcurl);
	} else {
		prterr("Error: curl_easy_init failed.");
	}
	if(chunk.memory!=NULL) 
	{
		temp3=EscXHTMLLabel(chunk.memory,0);
		s=new Wt::WString(temp3,UTF8);
		myTE = new Wt::WTextEdit(*s,(Wt::WContainerWidget *)RDA_Feed);
		myTE->setReadOnly(TRUE);
		vbox->addWidget(myTE,500);
		s->~WString();
		if(temp3!=NULL) Rfree(temp3);
	}
	return(RDA_Feed);
}
