/* qn2it */
/*lint -library */
#ifndef WIN32
#define __NAM__ "news.lnx"
#endif
#ifdef WIN32
#define __NAM__ "news.exe"
#endif
#include <app.hpp>


#include <powadd.hpp>
#include <misc.hpp>



#include <gui.hpp>
#include <guip.hpp>
#include <nrd.hpp>
#include <olh.hpp>
#include <mix.hpp>
#include <curl/curl.h>
#include <curl/easy.h>
/*ADDLIB curl */
/*ADDLIB add */
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

qt_QString *LandingPage=NULL,*LandingPageSource=NULL;
qt_QMainWindow *MainWindow=NULL;
qt_QWidget *RDA_News=NULL;
qt_QHBox *MainHBox=NULL;
qt_QScrollBar *vBar=NULL;
int y=0,count=0,News_W=(-1),News_H=(-1);
char no_scroll=FALSE;

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
static void myLinkClicked(qt_QTextBrowser *TE)
{
	qt_QString *s=NULL;
	char *t=NULL;
	APPlib *args=NULL;

	s=qt_QTextBrowser_source((qt_QTextBrowser *)TE);
	t=qt_QString_ascii(s);
	args=APPlibNEW();
	addAPPlib(args,t);
	ExecuteProgram("dohttp",args);
	if(args!=NULL) freeapplib(args);
	if(s!=NULL) qt_del_QString(s);
	qt_QTextEdit_setText((qt_QTextEdit *)TE,LandingPage);
}
static void ChangeScroll(qt_QTextBrowser *TE)
{
	if(!qt_QScrollBar_draggingSlider((qt_QScrollBar *)vBar))
	{
		if(count<0)
		{
			count++;
		} else {
			qt_QScrollView_scrollBy((qt_QScrollView *)TE,0,1);
			y=qt_QScrollBar_value((qt_QScrollBar *)vBar);
			if(y==(qt_QScrollBar_maxValue((qt_QScrollBar *)vBar)))
			{
				++count;
				if(count==20)
				{
					y=0;
					qt_QScrollBar_setValue((qt_QScrollBar *)vBar,y);
					count=(-20);
				}
			}
		}
	}
}
void CreateCanvas()
{
	CURL *RDAcurl=NULL;
	struct MemoryStruct chunk;
	qt_QString *qc=NULL;
	qt_QVBoxLayout *vb=NULL;
	qt_QHBox *hbox=NULL;
	qt_QTextEdit *TE=NULL;
	qt_Slot *mySlot=NULL,*T_Slot=NULL;
	qt_QTimer *myTimer=NULL;

	chunk.memory=NULL;
	chunk.size=0;
	RDAcurl=curl_easy_init();
	if(RDAcurl)
	{
		curl_easy_setopt(RDAcurl,CURLOPT_URL,"http://www.openrda.net/OpenRDALandingPage.php");
#ifdef PRXY
		if(!isEMPTY(prxy))
		{
			curl_easy_setopt(RDAcurl,CURLOPT_PROXY,prxy);
		}
#endif /* PRXY */
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
		RDA_News=(qt_QWidget *)qt_new_QWidget((qt_QWidget *)MainHBox,"RDA NEWS",QT_WDestructiveClose|QT_WStyle_NoBorder);
		if(News_W!=(-1) && News_H!=(-1))
		{
			qt_QWidget_setFixedWidth((qt_QWidget *)RDA_News,News_W);
			qt_QWidget_setFixedHeight((qt_QWidget *)RDA_News,News_H);
		}	
		memset(stemp,0,101);
		qc=qt_new_QString5("http://www.openrda.net/News");
		qt_QWidget_setCaption((qt_QWidget *)RDA_News,qc);
		if(qc!=NULL) qt_del_QString(qc);
		vb=qt_new_QVBoxLayout((qt_QWidget *)RDA_News,0,-1,"VBoxLayout");
		hbox=(qt_QWidget *)qt_new_QHBox((qt_QWidget *)RDA_News,"Row",0);
		qt_QBoxLayout_addWidget((qt_QBoxLayout *)vb,(qt_QWidget *)hbox,0,0);
		TE=qt_new_QTextBrowser((qt_QWidget *)hbox,"RDA NEWS");
		if(no_scroll==FALSE)
		{
			myTimer=qt_new_QTimer((qt_QObject *)TE,"SCROLL TIMER");
			qt_QTimer_start(myTimer,40,FALSE);
			T_Slot=qt_new_UserDataSlot(ChangeScroll,TE);
			qt_QObject_connect1((qt_QObject *)myTimer,"2timeout()",(qt_QObject *)T_Slot,"1UserDataSlot()");
		}
		mySlot=(qt_Slot *)qt_new_UserDataSlot(myLinkClicked,TE);
		qt_QObject_connect1((qt_QObject *)TE,"2sourceChanged(const QString &)",(qt_QObject *)mySlot,"1UserDataSlot()");
		qt_QTextEdit_setTextFormat((qt_QTextEdit *)TE,QT_RichText);
		qt_QTextEdit_setLinkUnderline((qt_QTextEdit *)TE,TRUE);
		vBar=(qt_QScrollBar *)qt_QScrollView_verticalScrollBar((qt_QScrollView *)TE);
		LandingPageSource=qt_new_QString5("");
		LandingPage=qt_new_QString5(chunk.memory);
		qt_QTextEdit_setText((qt_QTextEdit *)TE,LandingPage);
		qt_QWidget_showNormal(RDA_News);
		SetupWindowColorPalette((qt_QWidget *)RDA_News,"RDA NEWS WINDOW");
		if(chunk.memory!=NULL) free(chunk.memory);
	}
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	qt_QString *c=NULL;
	char *temp=NULL;
	qt_QPixmap *MyP=NULL;
#ifndef WIN32
	char *display_device=NULL;
#endif

	News_W=(-1);
	News_H=(-1);
	
#ifdef WIN32
	memset(stemp,0,101);
	sprintf(stemp,"TRUE");
	RDA_SetEnv("NOTRANSACTIONS",stemp);
#else
	display_device=RDA_GetEnv("DISPLAY");
	if(!RDAstrcmp(display_device,":0.0"))
	{
		no_scroll=TRUE;
	}
	memset(stemp,0,101);
	sprintf(stemp,"TRUE");
	RDA_SetEnv("NOTRANSACTIONS",stemp);
#endif
	if(InitializeSubsystems(argc,argv,"OLHELP","RDA NEWS")) 
	{
        	RDAAPPMAINLOOP();
        	return(1);
	}
#ifdef WIN32
	RDA_SetEnv("NOTRANSACTIONS",temp);
#else
	RDA_SetEnv("NOTRANSACTIONS",temp);
#endif
	dotrans=TRUE;
	USING_QN2IT=FALSE;
	count=(-20);

	temp=RDA_GetEnv("RDA_NEWS_WIDTH");
	if(temp!=NULL) News_W=atoi(temp);
	temp=RDA_GetEnv("RDA_NEWS_HEIGHT");
	if(temp!=NULL) News_H=atoi(temp);
	temp=NULL;
	MainWindow=qt_new_QMainWindow((qt_QWidget *)NULL,"RDA NEWS WINDOW",QT_WType_TopLevel|QT_WStyle_NormalBorder|QT_WStyle_Title|QT_WStyle_MinMax);
	if(News_W!=(-1) && News_H!=(-1))
	{
		qt_QWidget_setFixedWidth((qt_QWidget *)MainWindow,News_W+4);
		qt_QWidget_setFixedHeight((qt_QWidget *)MainWindow,News_H+4);
	} else {
		qt_QWidget_setMinimumWidth((qt_QWidget *)MainWindow,724);
		qt_QWidget_setMinimumHeight((qt_QWidget *)MainWindow,484);
	}	
	SetupWindowColorPalette(MainWindow,"RDA NEWS WINDOW");
	MyP=qt_new_QPixmap6(rda_icon_xpm);
	qt_QWidget_setIcon((qt_QWidget *)MainWindow,MyP);
	temp=Rmalloc(RDAstrlen(XPERT_SETUP->company_name)+RDAstrlen(VERSION_DATE)+30);
#ifdef RDA_64BITS
	sprintf(temp,"RDA NEWS (64 BITS %s)",VERSION_DATE);
#endif
#ifndef RDA_64BITS
	sprintf(temp,"RDA NEWS (32 BITS %s)",VERSION_DATE);
#endif
	c=qt_new_QString5(temp);
	if(temp!=NULL) Rfree(temp);
	qt_QWidget_setCaption((qt_QWidget *)MainWindow,c);
	qt_del_QString(c);
	MainHBox=qt_new_QVBox((qt_QWidget *)MainWindow,"Main V Box",0);
	SetupWindowColorPalette((qt_QWidget *)MainHBox,"RDA NEWS WINDOW");
	CreateCanvas();
	qt_QMainWindow_setCentralWidget((qt_QMainWindow *)MainWindow,(qt_QWidget *)MainHBox);
	if(GUI_CHILD_WINDOW_x!=(-1) && GUI_CHILD_WINDOW_y!=(-1))
	{
		qt_QWidget_move((qt_QWidget *)MainWindow,GUI_CHILD_WINDOW_x,GUI_CHILD_WINDOW_y);
	}
	qt_QMainWindow_show(MainWindow);
	++WindowCount;
	qt_QObject_connect1((qt_QObject *)RDAMAINWIDGET,"2lastWindowClosed()",(qt_QObject *)RDAMAINWIDGET,"1quit()");
       	RDAAPPMAINLOOP();
}
