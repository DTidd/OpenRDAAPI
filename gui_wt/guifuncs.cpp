#include <guip.hpp>
#include <gui.hpp>
#include <sec.hpp>
#include <nrd.hpp>
#include <nrdp.hpp>
#include <nrdp1.hpp>

#define SINGLE_THREADED_APPLICATION

// define __USE_ANIMATIONS__

/*lint -e537*/
#include<limits.h>
/*lint +e537*/

#ifdef WIN32
#include <stdio.h>
#include <windows.h>  
#endif

#include <Wt/Auth/AuthModel>
#include <Wt/Auth/AuthWidget>
#include <Wt/Auth/PasswordService>

#include <Session.hpp>
#include <SingleThreadedApplication.hpp>

APPlib *CMAIN_ARGS=NULL,*OpenRDA_Cookies=NULL;
char Skip_OpenRDA_Cookies=FALSE;
int RDA_argc=0;
char **RDA_argv;
extern char *PROGRAM_NAME;
extern char *VERSION_DATE;
extern void c_main(int,char **);
char using_js=FALSE,using_ajax=FALSE;
char inside_rfkw=FALSE; 
RDArsrc *rfkw_rsrc=NULL;
short USE_DIAGNOSTIC_SCREENS=TRUE,MODULE_GROUP=0;


char *(*RDA_AutoComplete)(char *,char *,char *,char *);
void (*RDA_CaptureAutoComplete)(char *,char *,char *,char *);


Cursor RDA_cursorShape=ArrowCursor;
	/* if !defined(SCO50) && !defined(SCO50P) 
	   NOTE: Have to remove because of account callback not removing 
	         when set to NULL in apps.  Have good solution for acctcb, 
	         accfuncs, timecb, datecb, ssncb, phonecb, and ssncb on my 
	         box backed up that will even beep to the user when attemps 
	         to update are invalid and unsuccessfull it is a more 
 	         generic solution and can be used for custom type callbacks 
                 that use pattern matching and need more than '-' 
	         as a delimiter. */
short USER_INTERFACE=0;
short GUI_INTERFACE=0;
int RDA_FONTSIZE=8;
void (*PP_translate_GUIFUNC)(char **);
void (*PP_translate_GUIFUNCALL)(RDArsrc *);
char *RDA_GROUP=NULL,SKIP_DOMENU_TRANSVERSAL=FALSE;
Wt::WApplication *RDAMAINWIDGET=NULL;
std::string RDA_session;
int WindowCount=0;
int RDA_UserAgent=0;


#ifdef _USE_GOOGLE_ANALYTICS_ 

// pass smodule and screen name to _trackPageview 

std::string googleCmd = 
	"if (window.pageTracker) {"
	"""try {"
	""  "setTimeout(function() {"
	""  "window.pageTracker._trackPageview(\""
	+ RDAMAINWIDGET->environment().deploymentPath()+RDAMAINWIDGET->internalPath()+"\");"
	""  "}, 1000);"
	"""} catch (e) { }"
	"}";
#endif /* _USE_GOOGLE_ANALYTICS_  */

char *standardfieldtypes[29]=
{
	"Variable Text Field",
	"Plain Text Field",
	"Formatted Dollar",
	"Phone Number",
	"Custom Types",
	"Zip Code",
	"Unsigned Decimal Value",
	"Unsigned Short Integer",
	"Unsigned Long Integer",
	"Unsigned Float Value",
	"Boolean Value",
	"Single Character",
	"Button",
	"ScrolledList",
	"ScrolledText",
	"Social Security #",
	"Date",
	"Time",
	"Signed Short Integer",
	"Signed Long Integer",
	"Signed Decimal Value",
	"Signed Float Value",
	"Expenditure Account",
	"Revenue Account",
	"Balance Sheet Account",
	"Beginning Balance Account",
	"Optional Field",
	"Formatted Dollar-No Cents",
	"Progress Bar",
};

char *wdgttypes[34]={
	"Standard Resource",
	"New Line",
	"End Line",
	"New Box",
	"End Box",
	"Label",
	"Button",
	"Scrolled List",
	"Scrolled Text",
	"Toggle Button (Boolean)",
	"Frame",
	"Seperator",
	"New Scrolled Window",
	"End Scrolled Window",
	"Optional Screen",
	"Custom Input Field",
	"Expenditure Field",
	"Revenue Field",
	"Balance Sheet Field",
	"Beginning Balance Field",
	"New Tabbed Container",
	"End Tabbed Container",
	"Progress Bar",
	"Tab Bar",
	"Start Popup Menu",
	"End Popup Menu",
	"Start Toolbar Menu",
	"End Toolbar Menu",
	"Start Table",
	"End Table",
	"Start Header",
	"End Header",
	"Start Panel",
	"End Panel",
};
char *membertypes[5]={
	"Standard Resource",
	"Button Resource",
	"Control Button - RF",
	"Control Button - RF & Exit",
	"Control Button - RF & KW"
};
class BlockingDialog : public Wt::WDialog
{
public:
  BlockingDialog(const std::string& url)
    : closedSignal_(this, "closed")
  {
    setWindowTitle("Blocked in dialog");

    contents()->addWidget
      (new Wt::WText("<p>Please use the newly opened window.</p>"
		     "<p>Continue here when done.</p>"));

    closedSignal_.connect(this, &BlockingDialog::accept);

    /*
     * See also: https://developer.mozilla.org/en-US/docs/Web/API/Window.open
     * for more options on the new window.
     */
    std::string js = 
      "var win = window.open('" + url + "');"
      "if (win) {" // not blocked
      """var timer = setInterval(function() {"
      """""if (win.closed) {"
      ""    "clearInterval(timer);"
      +      closedSignal_.createCall() + ";"
      +   "}"
      """}, 500);"
      "}";

    doJavaScript(js);
  }

private:
  Wt::JSignal<> closedSignal_;
};

void openWindowWait(const std::string& url)
{
  BlockingDialog d(url);
  d.exec();
}


void GUI_OpenWindowWait(char *windowData)
{
	openWindowWait(windowData);
}
void GUI_OpenURL(char *windowData)
{
	std::string url=windowData;

	std::string openWindowCmd=
		("window.open("+Wt::WWebWidget::jsStringLiteral(url)+");");
	RDAMAINWIDGET->doJavaScript(openWindowCmd);
}
void GUI_OpenWindow(char *windowData)
{
	std::stringstream ss1;

	ss1 << "/" << windowData ;
	std::string url=ss1.str();

	std::string openWindowCmd=
		("window.open("+Wt::WWebWidget::jsStringLiteral(url)+");");
	RDAMAINWIDGET->doJavaScript(openWindowCmd);
}
OpenRDALoadingIndicator::OpenRDALoadingIndicator()
{
	WApplication *app=NULL;
	WImage *img=NULL;

	app=Wt::WApplication::instance();
	setInline(false);
	cover_=new Wt::WContainerWidget(this);
	center_=new Wt::WContainerWidget(this);
	img=new Wt::WImage(Wt::WLink("resources/OpenRDA/openrda.png"),center_);
	img->setMargin(7,Top | Bottom);
	text_=new Wt::WText("Calculating...",center_);
	text_->setInline(false);
	text_->setMargin(Wt::WLength::Auto,Left | Right);
	if(app->environment().agentIsIE())
		app->styleSheet().addRule("body","height:100%; margin: 0;");
		app->styleSheet().addRule("div#"+cover_->id(),std::string()+
			"background: #DDDDDD;"
			"height: 100%; width: 100%;"
			"top: 0px; left: 0px;"
			"opacity: 0.5; position: absolute;"
			"-khtml-opacity: 0.5;"
			"z-index: 10000;"+(app->environment().agentIsIE() ? 
			"filter: alpha(opacity=50);"	
			:
			"-moz-opacity:0.5;"
			"-moz-background-clip: -moz-initial;"
			"-moz-background-origin: -moz-initial;"
			"-moz-background-inline-policy: -moz-initial;"));
		app->styleSheet().addRule("div#" + center_->id(),
			"background:  white;"
			"border: 3px solid #401a6b;"
			"z-index: 10001; visibility: visible;"
			"position: absolute; left: 50%; top: 50%;"
			"margin-left: -120px; margin-top: -100px;"
			"width: 300px; height: 160px;"
			"font-family: arial,sans-serif;"
			"text-align: center",
			"color: #004e00;");
}
void OpenRDALoadingIndicator::setMessage(const WString& text)
{
	text_->setText(text);
}	
extern OpenRDALoadingIndicator *myOpenRDALoadingIndicator;
extern DiagnosticLoadingIndicator *myDiagnosticLoadingIndicator;

char *RDA_EncodeWhiteSpace(char *temp1)
{
	int z=0;
	char *temp_str=NULL,*temp=NULL;
	
	if(isEMPTY(temp1)) return(NULL);
	temp_str=Rmalloc((RDAstrlen(temp1)*2)+1);
	z=0;
	for(temp=temp1;*temp;++temp)
	{
		if(*temp==' ') 
		{
			temp_str[z]=0xC2;
			temp_str[z+1]=0xA0;
			z+=2;
		} else {
			temp_str[z]=*temp;
			++z;
		}
	}	
	return(temp_str);
}

char *WT_GetAppRoot()
{
	char *temp=NULL;
	
	temp=getenv("WT_APP_ROOT");
	return(temp);
}
char *WT_GetDocRoot()
{
	std::string s1;
	char *temp=NULL;
	
	if(RDAMAINWIDGET!=NULL)
	{
		s1=RDAMAINWIDGET->docRoot();
		if(!s1.empty()) temp=stralloc(s1.c_str());
	}
	return(temp);
}
char *WT_RDA_GetEnv(char *name,int line,char *file)
{
	char *temp=NULL;
	Wt::WApplication *myAPP=NULL;
	std::string *n1=NULL;
	const std::string *v1=NULL;

	if(diaggui)
	{
		prterr("DIAG WT_RDA_GetEnv Cookie [%s] at line [%d] program [%s].",(name!=NULL ? name:""),line,(file!=NULL ? file:""));
	}
	if(isEMPTY(name)) return(NULL);
	if(FINDAPPLIBELEMENT(OpenRDA_Cookies,name)!=(-1))
	{
		temp=getenv(name);
		return(temp);
	} else {
		myAPP=Wt::WApplication::instance();
		n1=new string(name);
		v1=myAPP->environment().getCookieValue(*n1);

// get corrupted strings attempting to read the cookies

		if(v1!=NULL)
		{
			if(!v1->empty())
			{
				temp=v1->c_str();
			}
		}
		n1->~string();
		if(RDAstrcmp(name,"RDADIAG"))
		{
			WT_RDA_UnSetEnv(name,__LINE__,__FILE__);
		}
		return(temp);
	}
}

int WT_RDA_SetEnv(char *name,char *value,int line,char *file)
{
	Wt::WApplication *myAPP=NULL;
	std::string *n1=NULL,*v1=NULL,i,*p=NULL,*hname=NULL;
	char *temp=NULL,*x=NULL;

	if(diaggui)
	{
		prterr("DIAG WT_RDA_SetEnv Cookie [%s] with value [%s] at line [%d] program [%s].",(name!=NULL ? name:""),(value!=NULL ? value:""),line,(file!=NULL ? file:""));
	}
	setenv(name,value,TRUE);
	myAPP=Wt::WApplication::instance();
	n1=new string(name);
	if(Skip_OpenRDA_Cookies==FALSE) 
	{
		addAPPlib(OpenRDA_Cookies,name);
	}
	v1=new string(value);
/*
	p=new string("/xpert-mysql");
*/
	i=myAPP->environment().hostName();
	temp=stralloc(i.c_str());
	for(x=temp;*x;++x) if(*x=='/' || *x==':') x=0;
	hname=new string(temp);
	p=new string("/");

/*  wait for https, can't use this until after authentication 

	myAPP->setCookie(*n1,*v1,3600,*hname,*p,TRUE);
*/

	myAPP->setCookie(*n1,*v1,3600,*hname,*p,FALSE);
	n1->~string();
	hname->~string();
	v1->~string();
	p->~string();
	return(0);
}

int WT_RDA_PutEnv(char *namevalue,int line,char *file)
{
	Wt::WApplication *myAPP=NULL;
	std::string *n1=NULL,*v1=NULL,i,*p=NULL,*hname=NULL;
	char *temp2=NULL,*name=NULL,*value=NULL;
	char *temp=NULL,*x=NULL;

	if(diaggui)
	{
		prterr("DIAG WT_RDA_PutEnv Cookie [%s] at line [%d] program [%s].",(namevalue!=NULL ? namevalue:""),line,(file!=NULL ? file:""));
	}
	name=stralloc(namevalue);
	temp2=strchr(name,'=');
	if(temp2!=NULL)
	{
		*temp2=0;
		++temp2;
	}
	if(Skip_OpenRDA_Cookies==FALSE) 
	{
		addAPPlib(OpenRDA_Cookies,name);
	}
	myAPP=Wt::WApplication::instance();
	n1=new string(name);
	if((RDAstrlen(namevalue)-1)==RDAstrlen(name)) 
	{
		v1=new string("");
		value=NULL;
	} else {
		value=stralloc(temp2);
		v1=new string(temp2);
	}
	setenv(name,value,TRUE);
	i=myAPP->environment().hostName();
	temp=i.c_str();
	for(x=temp;*x;++x) if(*x=='/' || *x==':') x=0;
	hname=new string(temp);
	p=new string("/");
/*
// can't use this until after authentication 

	myAPP->setCookie(*n1,*v1,3600,*hname,*p,TRUE);

*/

	myAPP->setCookie(*n1,*v1,3600,*hname,*p,FALSE);
	n1->~string();
	v1->~string();
	hname->~string();
	p->~string();
	if(name!=NULL) Rfree(name);
	if(value!=NULL) Rfree(value);
	return(0);
}
int WT_RDA_UnSetEnv(char *name,int line,char *file)
{
	Wt::WApplication *myAPP=NULL;
	std::string *n1,i,*p,*hname=NULL;
	char *temp=NULL,*x=NULL;

	if(diaggui)
	{
		prterr("DIAG WT_RDA_UnSetEnv Cookie [%s] at line [%d] program [%s].",(name!=NULL ? name:""),line,(file!=NULL ? file:""));
	}
	if(isEMPTY(name)) return;
	if(RDAstrcmp(name,"RDADIAG"))
	{
		unsetenv(name);
	}	
	myAPP=Wt::WApplication::instance();
	n1=new string(name);
	i=myAPP->environment().hostName();
	temp=stralloc(i.c_str());
	for(x=temp;*x;++x) if(*x=='/' || *x==':') x=0;
	hname=new string(temp);
	if(temp!=NULL) Rfree(temp);
	p=new string("/");
	if(RDAstrcmp(name,"RDADIAG"))
	{
	myAPP->removeCookie(*n1,*hname,*p);
	}
	n1->~string();
	hname->~string();
	p->~string();
	return(0);
}
char *WtPassWdHash(char *p)
{
	char *ph=NULL;
	Wt::WString pw=p;
	PasswordHash result;

	result=Session::passwordAuth().verifier()->hashPassword(pw); 
	ph=stralloc(result.value().c_str());
	return(ph);
}
void NULL_RSCS(RDArsrc *r)
{
	int x;
	RDArmem *mem;

	if(r!=NULL)
	{
		r->primary=NULL;
		r->swidget=NULL;
		r->window_toolbar=NULL;
		for(x=0,mem=r->rscs;x<r->numrscs;++x,++mem)
		{
			if(!USER_INTERFACE)
			{
			}
			mem->w=NULL;
			mem->popup_w=NULL;
			mem->toolbar_w=NULL;
			mem->popup_id=0;
			if(mem->wSIM!=NULL)
			{
				mem->wSIM->clear();
			}
			mem->wSIM=NULL;
			mem->Table=NULL;
			mem->validobject=NULL;
			mem->tCell=NULL;
		}
	}
}
void xComputeNewTransversal(RDArsrc *r,char *(*EvalStr)(...),void *EvalStrArgs,void (*SubFunc)(...),
	void *SubFuncArgs,char *e,int line,char *file)
{
	char *newrx=NULL;

	if(SKIP_DOMENU_TRANSVERSAL==TRUE && !strncmp(PROGRAM_NAME,"domenu",6)) return;
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG ComputeNewTransversal with the expression %.255s at line [%d] program [%s].",(e!=NULL?e:""),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(r!=NULL)	
	{
		if(IsScreenDisplayed(r)) 
		{
			if(EvalStr!=NULL && SubFunc!=NULL && !isEMPTY(e))
			{
				if(EvalStrArgs!=NULL)
				{
/*lint -e746*/
					newrx=EvalStr(e,SubFunc,SubFuncArgs,EvalStrArgs);
/*lint +e746*/
				} else {
/*lint -e746*/
					newrx=EvalStr(e,SubFunc,SubFuncArgs);
/*lint +e746*/
				} 
				if(!isEMPTY(newrx))
				{
					FINDRSCSETINPUTFOCUS(r,(char *)newrx);
				}
				if(newrx!=NULL) Rfree(newrx);
			}
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG exiting ComputeNewTransversal at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
}

void loggedIn()
{
	c_main(CMAIN_ARGS->numlibs,CMAIN_ARGS->libs);
}


/* Specialized authmodel which always indicates 'remember-me' */
class RDAAuthModel : public Wt::Auth::AuthModel
{
public:
  RDAAuthModel (const Wt::Auth::AuthService &baseAuth,
		Wt::Auth::AbstractUserDatabase &users,
		Wt::WObject* parent=0)
    : Wt::Auth::AuthModel(baseAuth, users, parent)
  { }

  virtual bool login(Login& login)
  {
    if (Wt::Auth::AuthModel::login(login)) {
      Wt::WApplication *app = Wt::WApplication::instance();
      app->setCookie(baseAuth()->authTokenCookieName(),
		     baseAuth()->createAuthToken(login.user()),
		     baseAuth()->authTokenValidity() * 60,
                     std::string(), "/");
    
      return true;
    } else
      return false;
  }
};

#ifdef SINGLE_THREADED_APPLICATION
/* Application which authenticates before calling loggedIn() */
class AuthApplication : public SingleThreadedApplication
{
public:
  AuthApplication(const Wt::WEnvironment& env)
    : SingleThreadedApplication(env)
  { }

  virtual void create() {
	Wt::WString *c=NULL;
	int x=0;
	char tempx[512];
#ifdef WT_FASTCGI
	std::string pv[99],url;
	char *pname=NULL,*temp=NULL;
#endif
/* NEED THE ITEMS BELOW SUBSTANTIATED IN ORDER TO READ USERS TABLES FOR AUTHENTICATION */

/* Initialize Database, Security, Misc */
	RDA_STDERR=stderr;

	UI_RDA_GetEnv=WT_RDA_GetEnv;
	UI_RDA_SetEnv=WT_RDA_SetEnv;
	UI_RDA_PutEnv=WT_RDA_PutEnv;
	UI_RDA_UnSetEnv=WT_RDA_UnSetEnv;
	UI_GetDocRoot=WT_GetDocRoot;
	UI_GetAppRoot=WT_GetAppRoot;
	UI_OpenWindow=GUI_OpenWindow;
	UI_OpenWindowWait=GUI_OpenWindowWait;
	BCryptValue=WtPassWdHash;

	AlreadyInitialized=FALSE;
	RDA_NOGUI=FALSE;
	SEC_USERS_FILENO=(-1);
	SEC_TOKEN_FILENO=(-1);
	SETCWD();
	NRDbuf=NULL;	
	no_buffers=0;
	dbcons.s=NULL;
	dbcons.num=0;
#ifdef USE_BERKELEY_BTREE
	retval=INITNRDberkeley();
	if(retval)
	{
		prterr("Error INITNRDberkeley Failed to Initialize the Berkeley Btree Engine.");
	}
#endif
#ifdef USE_MYISAM
	INITNRDmyisam();
#endif
#ifdef USE_MYSQL
	INITNRDmysql();
#endif
#ifdef USE_CTREE
#ifdef XXXXX
	ctWNGV=NULL;
#endif
	retval=INITNRDctree();
	if(retval) 
	{
		prterr("Error INITNRDctree Failed to Initialize the Faircom Engines.");
	}
#endif
#ifdef INFORMIX_ISAM
	retval=INITNRDisam();
	if(retval) 
	{
		prterr("Error INITNRDisam Failed to Initialize the Informix Engines.");
	}
#endif
/* appRoot needed to open files */
	SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	SEC_TOKEN_FILENO=OPNNRD("SECURITY","TOKENS");
	if(diaggui)
	{
		std::cerr << "SEC_USERS_FILENO [" << SEC_USERS_FILENO << "]" << std::endl;
		std::cerr << "SEC_TOKEN_FILENO [" << SEC_TOKEN_FILENO << "]" << std::endl;
	}
	AlreadyInitialized=TRUE;
/* NEED THE ITEMS ABOVE SUBSTANTIATED IN ORDER TO READ USERS TABLES FOR AUTHENTICATION */
	RDAMAINWIDGET=this;
#ifdef _USE_GOOGLE_ANALYTICS_ 
	RDAMAINWIDGET->require("http://www.google-analytics.com/ga.js");


// form of url but doens't have to be a true url 

	RDAMAINWIDGET->doJavaScript(
		"window.pageTracker=_gat._getTracker('UA-41161936-2');"
		"window.pageTracker.initData();"
		"window.pageTracker._trackPageview();"
	);
#endif /* _USE_GOOGLE_ANALYTICS_  */
	
	RDAMAINWIDGET->setCssTheme("polished");

	RDAMAINWIDGET->useStyleSheet(Wt::WLink("/resources/OpenRDA/OpenRDA4.0.css"));
	memset(tempx,0,512);
	sprintf(tempx,"%s/LoginTemplates",CURRENTDIRECTORY);
	RDAMAINWIDGET->messageResourceBundle().use(tempx);


	RDA_session=RDAMAINWIDGET->sessionId();
	RDA_UserAgent=RDAMAINWIDGET->environment().agent();
	using_ajax=RDAMAINWIDGET->environment().ajax();
	using_js=RDAMAINWIDGET->environment().javaScript();
	RDAMAINWIDGET->setLoadingIndicator(new OpenRDALoadingIndicator());
	c=new WString("Please wait while we get that for you...");
	RDAMAINWIDGET->loadingIndicator()->setMessage(*c);
	c->~WString();
	OpenRDA_Cookies=APPlibNEW();
	CMAIN_ARGS=APPlibNEW();
#ifndef WT_FASTCGI
	for(x=0;x<RDA_argc;++x)
	{
		if(!RDAstrcmp(RDA_argv[x],"client"))
		{
			break;
		} else if(!strncmp(RDA_argv[x],"--",2)) 
		{
			++x;
		} else {
			addAPPlib(CMAIN_ARGS,RDA_argv[x]);
		}
	}
#else 
	url=RDAMAINWIDGET->url();
	pname=url.c_str();
	for(temp=pname;*temp;++temp)
	{
		if(*temp=='?' || *temp==' ') 
		{
			*temp=0;
			break;
		}
	}
	addAPPlib(CMAIN_ARGS,&pname[1]);
	for(x=0;x<99;++x)
	{
		memset(stemp,0,101);
		sprintf(stemp,"arg%d",(x+1));
		if(!RDAMAINWIDGET->environment().getParameterValues(stemp).empty())
		{
			pv[x]=RDAMAINWIDGET->environment().getParameterValues(stemp)[0];
			addAPPlib(CMAIN_ARGS,pv[x].c_str());
		} else break;
	}		
#endif /* FASTCGI */
    session_.login().changed().connect(this, &AuthApplication::authEvent);

    authWidget_ = new Wt::Auth::AuthWidget(session_.login());

    authWidget_->setModel(new RDAAuthModel(Session::auth(), session_.users(),
					   this));

    authWidget_->model()->addPasswordAuth(&Session::passwordAuth());

    root()->addWidget(authWidget_);

    processCookie();
  }

  virtual void destroy() {
    root()->clear();
  }

  void processCookie() {
    authWidget_->processEnvironment();
  }

  void authEvent() {
    if (session_.login().loggedIn()) {
      const Wt::Auth::User& u = session_.login().user();

      root()->clear();
      authWidget_ = 0;

      loggedIn();
    } else {
      Wt::log("notice") << "User logged out.";
    }
  }

protected:
  virtual void threadNotify(const Wt::WEvent& event) {
    if (event.eventType() == Wt::UserEvent) {
      if (session_.login().loggedIn()) {
	session_.users().extendAuthTokenValidity(session_.login().user());
      }
    }

    SingleThreadedApplication::threadNotify(event);
  }

private:
  Session session_;
  Wt::Auth::AuthWidget *authWidget_;
};
#else /* WHEN NOT SINGLE_THREADED_APPLICATION */
/* Application which authenticates before calling loggedIn() */
class AuthApplication : public Wt::WApplication 
{
public:
  AuthApplication(const Wt::WEnvironment& env)
    : Wt::WApplication(env)
  { 

	Wt::WString *c=NULL;
	int x=0;
	char tempx[512];
#ifdef WT_FASTCGI
	std::string pv[99],url;
	char *pname=NULL,*temp=NULL;
#endif
/* NEED THE ITEMS BELOW SUBSTANTIATED IN ORDER TO READ USERS TABLES FOR AUTHENTICATION */

/* Initialize Database, Security, Misc */
	RDA_STDERR=stderr;

	UI_RDA_GetEnv=WT_RDA_GetEnv;
	UI_RDA_SetEnv=WT_RDA_SetEnv;
	UI_RDA_PutEnv=WT_RDA_PutEnv;
	UI_RDA_UnSetEnv=WT_RDA_UnSetEnv;
	UI_GetDocRoot=WT_GetDocRoot;
	UI_GetAppRoot=WT_GetAppRoot;
	UI_OpenWindow=GUI_OpenWindow;
	UI_OpenWindowWait=GUI_OpenWindowWait;
	BCryptValue=WtPassWdHash;

	AlreadyInitialized=FALSE;
	RDA_NOGUI=FALSE;
	SEC_USERS_FILENO=(-1);
	SEC_TOKEN_FILENO=(-1);
	SETCWD();
	NRDbuf=NULL;	
	no_buffers=0;
	dbcons.s=NULL;
	dbcons.num=0;
#ifdef USE_BERKELEY_BTREE
	retval=INITNRDberkeley();
	if(retval)
	{
		prterr("Error INITNRDberkeley Failed to Initialize the Berkeley Btree Engine.");
	}
#endif
#ifdef USE_MYISAM
	INITNRDmyisam();
#endif
#ifdef USE_MYSQL
	INITNRDmysql();
#endif
#ifdef USE_CTREE
#ifdef XXXXX
	ctWNGV=NULL;
#endif
	retval=INITNRDctree();
	if(retval) 
	{
		prterr("Error INITNRDctree Failed to Initialize the Faircom Engines.");
	}
#endif
#ifdef INFORMIX_ISAM
	retval=INITNRDisam();
	if(retval) 
	{
		prterr("Error INITNRDisam Failed to Initialize the Informix Engines.");
	}
#endif
/* appRoot needed to open files */
	SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");
	SEC_TOKEN_FILENO=OPNNRD("SECURITY","TOKENS");
	if(diaggui)
	{
		std::cerr << "SEC_USERS_FILENO [" << SEC_USERS_FILENO << "]" << std::endl;
		std::cerr << "SEC_TOKEN_FILENO [" << SEC_TOKEN_FILENO << "]" << std::endl;
	}
	AlreadyInitialized=TRUE;
/* NEED THE ITEMS ABOVE SUBSTANTIATED IN ORDER TO READ USERS TABLES FOR AUTHENTICATION */
	RDAMAINWIDGET=this;
#ifdef _USE_GOOGLE_ANALYTICS_ 
	RDAMAINWIDGET->require("http://www.google-analytics.com/ga.js");


// form of url but doens't have to be a true url 

	RDAMAINWIDGET->doJavaScript(
		"window.pageTracker=_gat._getTracker('UA-41161936-2');"
		"window.pageTracker.initData();"
		"window.pageTracker._trackPageview();"
	);
#endif /* _USE_GOOGLE_ANALYTICS_  */
	
	RDAMAINWIDGET->setCssTheme("polished");

	RDAMAINWIDGET->useStyleSheet(Wt::WLink("/resources/OpenRDA/OpenRDA4.0.css"));
	memset(tempx,0,512);
	sprintf(tempx,"%s/LoginTemplates",CURRENTDIRECTORY);
	RDAMAINWIDGET->messageResourceBundle().use(tempx);


	RDA_session=RDAMAINWIDGET->sessionId();
	RDA_UserAgent=RDAMAINWIDGET->environment().agent();
	using_ajax=RDAMAINWIDGET->environment().ajax();
	using_js=RDAMAINWIDGET->environment().javaScript();
	RDAMAINWIDGET->setLoadingIndicator(new OpenRDALoadingIndicator());
	c=new WString("Please wait while we get that for you...");
	RDAMAINWIDGET->loadingIndicator()->setMessage(*c);
	c->~WString();
	OpenRDA_Cookies=APPlibNEW();
	CMAIN_ARGS=APPlibNEW();
#ifndef WT_FASTCGI
	for(x=0;x<RDA_argc;++x)
	{
		if(!RDAstrcmp(RDA_argv[x],"client"))
		{
			break;
		} else if(!strncmp(RDA_argv[x],"--",2)) 
		{
			++x;
		} else {
			addAPPlib(CMAIN_ARGS,RDA_argv[x]);
		}
	}
#else 
	url=RDAMAINWIDGET->url();
	pname=url.c_str();
	for(temp=pname;*temp;++temp)
	{
		if(*temp=='?' || *temp==' ') 
		{
			*temp=0;
			break;
		}
	}
	addAPPlib(CMAIN_ARGS,&pname[1]);
	for(x=0;x<99;++x)
	{
		memset(stemp,0,101);
		sprintf(stemp,"arg%d",(x+1));
		if(!RDAMAINWIDGET->environment().getParameterValues(stemp).empty())
		{
			pv[x]=RDAMAINWIDGET->environment().getParameterValues(stemp)[0];
			addAPPlib(CMAIN_ARGS,pv[x].c_str());
		} else break;
	}		
#endif /* FASTCGI */
    session_.login().changed().connect(this, &AuthApplication::authEvent);

    authWidget_ = new Wt::Auth::AuthWidget(session_.login());

    authWidget_->setModel(new RDAAuthModel(Session::auth(), session_.users(),
					   this));

    authWidget_->model()->addPasswordAuth(&Session::passwordAuth());

    root()->addWidget(authWidget_);

    processCookie();
  }

  virtual void destroy() {
    root()->clear();
  }

  void processCookie() {
    authWidget_->processEnvironment();
  }

  void authEvent() {
    if (session_.login().loggedIn()) {
      const Wt::Auth::User& u = session_.login().user();

      root()->clear();
      authWidget_ = 0;

      loggedIn();
    } else {
      Wt::log("notice") << "User logged out.";
    }
  }

protected:
  virtual void threadNotify(const Wt::WEvent& event) {
    if (event.eventType() == Wt::UserEvent) {
      if (session_.login().loggedIn()) {
	session_.users().extendAuthTokenValidity(session_.login().user());
      }
    }

    WApplication::notify(event);
  }

private:
  Session session_;
  Wt::Auth::AuthWidget *authWidget_;
};
#endif /* SINGLE_THREADED_APPLICATION */

Wt::WApplication *createApplication(const WEnvironment& env)
{

/* Though these need Wt::WApplication to identify appRoot to complete */

	AuthApplication *app = new AuthApplication(env);

	return (app);
}

#ifdef WIN32
static char *READargument(char **temp,short *Texit)
{
	int count=0;
	char *tmp=NULL,quotes=FALSE,tread[2];
	char *sometemp=NULL;
	char t=0;

	sometemp=*temp;
	t=sometemp[0];
	if(t==' ') 
	{
		++(*temp);
		++sometemp;
	}
	if(!*Texit && !isEMPTY(sometemp))
	{
		while(TRUE)
		{
			memset(tread,0,2);
			tread[0]=(*sometemp);
			if(isEMPTY(tread))
			{
				*Texit=TRUE;
				break;
			}
			if(!quotes && tread[0]=='\"') 
			{
				quotes=TRUE;
			} else if(quotes && strchr("\"",tread[0]))
			{
				quotes=FALSE;
				++(*temp);
				++sometemp;
				break;
			} else if(!quotes && strchr("\t\n",tread[0]) && count>0) 
			{
				break;
			} else if(!quotes && tread[0]==' ' && count>0)
			{
				break;
			} else if(!quotes && strchr("\t\n",tread[0]))
			{
			} else {
				if(tmp!=NULL)
				{
					tmp=Rrealloc(tmp,count+1);
				} else tmp=Rmalloc(2);
				tmp[count]=tread[0];
				tmp[count+1]='\0';
				++count;
			}
			++(*temp);
			++sometemp;
		}
	}
	return(tmp);
}
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                      LPTSTR lpCmdLine, int nCmdShow)
{
	APPlib *a=NULL;
	char *x=NULL,*temp=NULL;
	short Texit=FALSE;

	a=APPlibNEW();
	addAPPlib(a,"PROGRAM.EXE");
	temp=lpCmdLine;
	while(Texit==FALSE)
	{
		x=READargument(&temp,&Texit);
		if(!isEMPTY(x))
		{
			addAPPlib(a,x);
		} else break;
		if(x!=NULL) Rfree(x);
	}
	if(x!=NULL) Rfree(x);
	WRun(a->numlibs,a->libs,&createApplication);
	sleep(5);
	return(0);
}
#else
int main(int argc,char *argv[])
{
	int x=0;

	RDAMAINWIDGET=NULL;
	Skip_OpenRDA_Cookies=FALSE;

	RDA_argc=argc;
	RDA_argv=argv;

	Session::configureAuth();

	WRun(argc,argv,&createApplication);
	sleep(5);
	return(0);
}
#endif

void xINITGUI(int argc,char *argv[],char *d,int line,char *file)
{
	char *temp=NULL,*pname=NULL;
	Wt::WString *c=NULL;

	USE_DIAGNOSTIC_SCREENS=FALSE;
	MODULE_GROUP=0;
	inside_rfkw=FALSE;
	rfkw_rsrc=NULL;
	INSIDE_PASTE=FALSE;
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		/*
		   prterr causes RDADIAG=GUI to crash on windows. 
		   The  prterr() command works fine
		*/
		prterr("DIAG Initializing Gui Interface at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(diaggui)
	{
		prterr("DIAG Using_JavaScript [%s] Using_Ajax [%s] UserAgent [%d] \n\n",(using_js ? "Yes":"No"),(using_ajax ? "Yes":"No"),RDA_UserAgent);
	}
	UI_RDA_GetEnv=WT_RDA_GetEnv;
	UI_RDA_SetEnv=WT_RDA_SetEnv;
	UI_RDA_PutEnv=WT_RDA_PutEnv;
	UI_RDA_UnSetEnv=WT_RDA_UnSetEnv;
	UI_GetDocRoot=WT_GetDocRoot;
	UI_GetAppRoot=WT_GetAppRoot;
	UI_OpenWindow=GUI_OpenWindow;
	UI_OpenWindowWait=GUI_OpenWindowWait;
	BCryptValue=WtPassWdHash;

	RDA_AutoComplete=NULL;
	RDA_CaptureAutoComplete=NULL;
	WindowCount=0;
	if(USERLOGIN==NULL) SETUSERLOGIN();
	if(CURRENTDIRECTORY==NULL) SETCWD();
	if(RDA_NOGUI==FALSE)
	{
		temp=RDA_GetEnv("RDA_NOGUI");
		if(!isEMPTY(temp)) RDA_NOGUI=TRUE;
			else RDA_NOGUI=FALSE;
	}

	if(RDA_NOGUI==0)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagcss)
		{
			memset(GUIstemp,0,1024);
/*
			sprintf(GUIstemp,"%s/OpenRDA.csstags.csv",CURRENTDIRECTORY);
*/
			sprintf(GUIstemp,"%s/resources/OpenRDA/OpenRDA.csstags.csv",DOCUMENTROOT);
#ifndef WIN32
			cssNames=fopen(GUIstemp,"a+b");
#else
			cssNames=fopen(GUIstemp,"abc");
#endif
			if(cssNames==NULL)
			{
				std::cerr << "ERROR:  OpenRDA.csstags.csv didn't get created." << std::endl;
			}
		}
#endif /* USE_RDA_DIAGNOSTICS */
		USER_INTERFACE=FALSE;
		temp=Rmalloc(RDAstrlen(PROGRAM_NAME)+RDAstrlen(VERSION_DATE)+19);
#ifdef WIN32
		sprintf(temp,"%s-%s",PROGRAM_NAME,VERSION_DATE);
#endif /* WIN32 */
#ifdef RDA_64BITS
		sprintf(temp,"%s-%s 64 BITS",PROGRAM_NAME,VERSION_DATE);
#endif
#ifndef RDA_64BITS
		sprintf(temp,"%s-%s 32 BITS",PROGRAM_NAME,VERSION_DATE);
#endif
		pname=stralloc(PROGRAM_NAME);
		c=new WString(temp);
		RDAMAINWIDGET->setTitle(*c);
		c->~WString();
#define USE_CONFIRM_CLOSE_MESSAGE
#ifdef USE_CONFIRM_CLOSE_MESSAGE
		c=new WString("Closing the browser tab will abruptly stop your application and not close gracefully.  This will cause you problems restarting restricted processes.  Please use the \"Quit\" button within the application.");
		RDAMAINWIDGET->setConfirmCloseMessage(*c);
		c->~WString();
#endif /* USE_CONFIRM_CLOSE_MESSAGE */
		if(temp!=NULL) Rfree(temp);

		USER_INTERFACE=FALSE;
		if(!USER_INTERFACE)
		{
			GUI_INTERFACE=0;
/*lint -e64 */
/*lint +e64 */
		}
		if(temp!=NULL) Rfree(temp);
	} /* END OF RDA_NOGUI */
	EXPENDITURE_ACCOUNT=RDAacctNEW(EXP_ACCT);
	getacctbin(EXPENDITURE_ACCOUNT);
	REVENUE_ACCOUNT=RDAacctNEW(REV_ACCT);
	getacctbin(REVENUE_ACCOUNT);
	BALANCE_SHEET_ACCOUNT=RDAacctNEW(BAL_ACCT);
	getacctbin(BALANCE_SHEET_ACCOUNT);
	BEGINNING_BALANCE_ACCOUNT=RDAacctNEW(BEG_ACCT);
	getacctbin(BEGINNING_BALANCE_ACCOUNT);
	CUSTOM_INPUTS_ACCOUNT=RDAacctNEW(CUSTOM_ACCT);
	getacctbin(CUSTOM_INPUTS_ACCOUNT);
	CustomTableFunction=NULL;
	PP_translate_GUIFUNCALL=NULL;
	PP_translate_GUIFUNC=NULL;
}
void xEXITGUI(int line,char *file)
{
	std::string s1("http:/");
	Wt::WString *c=NULL;
	int x=0;
	

#ifdef USE_CONFIRM_CLOSE_MESSAGE
	c=new WString("");
	RDAMAINWIDGET->setConfirmCloseMessage(*c);
	c->~WString();
#endif /* USE_CONFIRM_CLOSE_MESSAGE */
#ifdef WT_FASTCGI
	std::string closeWindowCmd=("window.close();");
#endif

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG EXITGUI Exiting the Graphical User Interface Subsystem at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
#ifdef USE_RDA_DIAGNOSTICS
	if(diagcss)
	{
		fclose(cssNames);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(EXPENDITURE_ACCOUNT!=NULL) FreeRDAacctmstr(EXPENDITURE_ACCOUNT);
	if(REVENUE_ACCOUNT!=NULL) FreeRDAacctmstr(REVENUE_ACCOUNT);
	if(BALANCE_SHEET_ACCOUNT!=NULL) FreeRDAacctmstr(BALANCE_SHEET_ACCOUNT);
	if(BEGINNING_BALANCE_ACCOUNT!=NULL) FreeRDAacctmstr(BEGINNING_BALANCE_ACCOUNT);
	if(CUSTOM_INPUTS_ACCOUNT!=NULL) FreeRDAacctmstr(CUSTOM_INPUTS_ACCOUNT);
	if(CMAIN_ARGS!=NULL) freeapplib(CMAIN_ARGS);
	if(OpenRDA_Cookies!=NULL)
	{
		for(x=0;x<OpenRDA_Cookies->numlibs;++x)
		{
			WT_RDA_UnSetEnv(OpenRDA_Cookies->libs[x],__LINE__,__FILE__);
		}
		freeapplib(OpenRDA_Cookies);
		OpenRDA_Cookies=NULL;
	}
#ifdef WT_FASTCGI
	if(!RDAstrcmp(PROGRAM_NAME,"openrda.lnx") || !RDAstrcmp(PROGRAM_NAME,"openrda.exe"))
	{
		s1+RDAMAINWIDGET->environment().hostName();
		RDAMAINWIDGET->redirect(s1);
	} else {
		RDAMAINWIDGET->doJavaScript(closeWindowCmd);
	}
#else
	s1+RDAMAINWIDGET->environment().hostName();
	RDAMAINWIDGET->redirect(s1);
#endif
	if(RDAMAINWIDGET!=NULL)
	{
		c=new WString("We'll see you soon...");
		RDAMAINWIDGET->loadingIndicator()->setMessage(*c);
		c->~WString();
		RDAMAINWIDGET->root()->clear();
		RDAMAINWIDGET->quit();
	}
	RDAMAINWIDGET=NULL;
	if(LastGroupDefault!=NULL) Rfree(LastGroupDefault);
}
int xRDAAPPMAINLOOP(int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG RDAAPPMAINLOOP Initializing RDAAPPMAINLOOP at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!USER_INTERFACE)
	{
	}
	return(0);
}

/*---------------------------------------------------------------------------
	This function free's the specified RDAscrn structure
---------------------------------------------------------------------------*/
void xfree_scrn(RDAscrn *screen,int line,char *file)
{
	int x;
	RDAwdgt *wdgt;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG Freeing Screen [%s] at line# [%d] program [%s].",(screen->name!=NULL ? screen->name:""),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(screen!=NULL)
	{
		if(screen->module!=NULL) Rfree(screen->module);
		if(screen->name!=NULL) Rfree(screen->name);
		if(screen->wdgts!=NULL)
		{
			for(x=0,wdgt=screen->wdgts;x<screen->numwdgts;++x,++wdgt)
			{
				if(wdgt->resource_name!=NULL) 
					Rfree(wdgt->resource_name);
				if(wdgt->label!=NULL) Rfree(wdgt->label);
				if(wdgt->expression!=NULL) Rfree(wdgt->expression);
				if(wdgt->sensitive_expression!=NULL) Rfree(wdgt->sensitive_expression);
				if(wdgt->editable_expression!=NULL) Rfree(wdgt->editable_expression);
				if(wdgt->transversal_expression!=NULL) Rfree(wdgt->transversal_expression);
			}
			Rfree(screen->wdgts); 
		}
		Rfree(screen);
	}
}
void xFreeRDAScrolledList(RDAScrolledList *a,int line,char *file)
{
	int x;
	RDAconfld *c;
	DFincvir *i=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG FreeRDAScrolledList at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(a!=NULL)
	{
		if(a->module!=NULL) Rfree(a->module);
		if(a->name!=NULL) Rfree(a->name);
		if(a->select_formula!=NULL) Rfree(a->select_formula);
		if(a->special_function!=NULL) Rfree(a->special_function);
		if(a->incvir!=NULL)
		{
			for(x=0,i=a->incvir;x<a->numincvirs;++x,++i)
			{
				if(i->module!=NULL) Rfree(i->module);
				if(i->file!=NULL) Rfree(i->file);
				if(i->keyname!=NULL) Rfree(i->keyname);
			}
			Rfree(a->incvir);
		}
		if(a->initfld!=NULL)
		{
			for(x=0,c=a->initfld;x<a->num;++x,++c)
			{
				if(c->fldname!=NULL) Rfree(c->fldname);
				if(c->conname!=NULL) Rfree(c->conname);
			}
			Rfree(a->initfld);
		}
		if(a->ffield!=NULL) Rfree(a->ffield);
		if(a->ffinfo!=NULL)
		{
			if(a->ffinfo->module!=NULL) Rfree(a->ffinfo->module);
			if(a->ffinfo->file!=NULL) Rfree(a->ffinfo->file);
			if(a->ffinfo->keyname!=NULL) Rfree(a->ffinfo->keyname);
			Rfree(a->ffinfo);
		}
		if(a->conmod!=NULL) Rfree(a->conmod);
		if(a->confil!=NULL) Rfree(a->confil);
		if(a->confld!=NULL) Rfree(a->confld);
		if(a->list!=NULL) freeapplib(a->list);
		if(a->dlist!=NULL) freeapplib(a->dlist);
		if(a->format_formula!=NULL) Rfree(a->format_formula);
		if(a->unformat_formula!=NULL) Rfree(a->unformat_formula);
		if(a->reformat_formula!=NULL) Rfree(a->reformat_formula);
		if(a->desc!=NULL) Rfree(a->desc);
		Rfree(a);
	}
}
/*---------------------------------------------------------------------------
	This function free's the specified RDArsrc structure
---------------------------------------------------------------------------*/
void xfree_rsrc(RDArsrc *rscrs,int line,char *file)
{
	int x,y;
	RDArmem *member;
	RDAconfld *c;
	RDAvirtual *v;
	DFincvir *i;
	RDAScrolledList *list=NULL;
	RDALoadValue *load=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui) { prterr("DIAG free_rsrc Freeing Resources for Screen [%s] [%s] at line# [%d] program [%s].",rscrs->module,rscrs->screen,line,file); }
#endif /* USE_RDA_DIAGNOSTICS */
	if(rscrs!=NULL)
	{
		if(rscrs->module!=NULL) Rfree(rscrs->module);
		if(rscrs->screen!=NULL) Rfree(rscrs->screen);
		if(rscrs->scn!=NULL) free_scrn(rscrs->scn);
		if(rscrs->input_focus!=NULL) Rfree(rscrs->input_focus);
		rscrs->window_toolbar=NULL;
		if(rscrs->rscs!=NULL)
		{
			for(x=0,member=rscrs->rscs;x<rscrs->numrscs;++x,++member)
			{
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui)
				{
					prterr("DIAG Freeing member [%s] type [%d] [%s] at line# [%d] program [%s].",member->rscrname,member->field_type,standardfieldtypes[member->field_type],__LINE__,__FILE__);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				if(member->rscrname!=NULL) Rfree(member->rscrname);
				if(member->editable_expression!=NULL) Rfree(member->editable_expression);
				if(member->sensitive_expression!=NULL) Rfree(member->sensitive_expression);
				if(member->transversal_expression!=NULL) Rfree(member->transversal_expression);
				if(member->label!=NULL) Rfree(member->label);
				if(member->XHTML_Label!=NULL) Rfree(member->XHTML_Label);
				if(member->definition!=NULL) Rfree(member->definition);
				if(member->wSIM!=NULL)
				{
					member->wSIM->clear();
				}
				member->wSIM=NULL;
				switch(member->field_type)
				{
					case SCROLLEDLIST:
						if(member->value.integer_value!=NULL) Rfree(member->value.integer_value);
						break;
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case BOOLNS:
					case CHARACTERS:
					case PHONE:
					case CUSTOMTYPE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case ZIPCODE:
						if(member->value.string_value!=NULL) Rfree(member->value.string_value);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						if(member->value.float_value!=NULL) Rfree(member->value.float_value);
						break;
					case SHORTV:
					case SSHORTV:
						if(member->value.short_value!=NULL) Rfree(member->value.short_value);
						break;
					case PROGRESS_BAR:
					case LONGV:
					case SLONGV:
						if(member->value.integer_value!=NULL) Rfree(member->value.integer_value);
						break;
					case BUTTONS:
						break;
					default:
						prterr("Error field type [%s] invalid for Resource [%s] on Screen [%s] [%s] at line [%d] program [%s].",member->field_type,member->rscrname,rscrs->module,rscrs->screen,line,file);
						break;
				}
			}
			Rfree(rscrs->rscs); 
		}
		if(rscrs->incvir!=NULL)
		{
			for(x=0,i=rscrs->incvir;x<rscrs->numvir;++x,++i)
			{
				if(i->module!=NULL) Rfree(i->module);
				if(i->file!=NULL) Rfree(i->file);
				if(i->keyname!=NULL) Rfree(i->keyname);
			}
			Rfree(rscrs->incvir);
			rscrs->incvir=NULL;
			rscrs->numvir=0;
		}
		if(rscrs->virflds!=NULL)
		{
			for(x=0,v=rscrs->virflds;x<rscrs->num;++x,++v)
			{
				if(v->name!=NULL) Rfree(v->name);
				if(v->expression!=NULL) Rfree(v->expression);
				switch(v->type)
				{
					case SCROLLEDTEXT:
					case SOCSECNUM:
					case DATES:
					case TIMES:
					case VARIABLETEXT:
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case ZIPCODE:
					case BOOLNS:
					case CHARACTERS:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						if(v->value.string_value!=NULL) 
							Rfree(v->value.string_value);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDECIMALV:
					case SDOUBLEV:
						if(v->value.float_value!=NULL) 
							Rfree(v->value.float_value);
						break;
					case SHORTV:
					case SSHORTV:
						if(v->value.short_value!=NULL) 
							Rfree(v->value.short_value);
						break;
					case SLONGV:
					case LONGV:
					case SCROLLEDLIST:
					case PROGRESS_BAR:
						if(v->value.integer_value!=NULL)
							Rfree(v->value.integer_value);
						break;
					case BUTTONS:
						break;
					default:
						prterr("Error field type [%d] invalid for virtual field [%s] on Screen [%s] [%s] at line [%d] program [%s].",v->type,v->name,rscrs->module,rscrs->screen,line,file);
						break;
				}
			}
			Rfree(rscrs->virflds);
			rscrs->virflds=NULL;
			rscrs->num=0;
		}
		if(rscrs->lists!=NULL)
		{
			for(x=0,list=rscrs->lists;x<rscrs->numlists;++x,++list)
			{
				if(list->module!=NULL) Rfree(list->module);
				if(list->name!=NULL) Rfree(list->name);
				if(list->select_formula!=NULL) Rfree(list->select_formula);
				if(list->special_function!=NULL) Rfree(list->special_function);
				if(list->incvir!=NULL)
				{
					for(y=0,i=list->incvir;y<list->numincvirs;++y,++i)
					{
						if(i->module!=NULL) Rfree(i->module);
						if(i->file!=NULL) Rfree(i->file);
						if(i->keyname!=NULL) Rfree(i->keyname);
					}
					Rfree(list->incvir);
				}
				if(list->initfld!=NULL)
				{
					for(y=0,c=list->initfld;y<list->num;++y,++c)
					{
						if(c->fldname!=NULL) Rfree(c->fldname);
						if(c->conname!=NULL) Rfree(c->conname);
					}
					Rfree(list->initfld);
				}
				if(list->ffield!=NULL) Rfree(list->ffield);
				if(list->ffinfo!=NULL)
				{
					if(list->ffinfo->module!=NULL) Rfree(list->ffinfo->module);
					if(list->ffinfo->file!=NULL) Rfree(list->ffinfo->file);
					if(list->ffinfo->keyname!=NULL) Rfree(list->ffinfo->keyname);
					Rfree(list->ffinfo);
				}
				if(list->conmod!=NULL) Rfree(list->conmod);
				if(list->confil!=NULL) Rfree(list->confil);
				if(list->confld!=NULL) Rfree(list->confld);
				if(list->list!=NULL) freeapplib(list->list);
				if(list->format_formula!=NULL) Rfree(list->format_formula);
				if(list->unformat_formula!=NULL) Rfree(list->unformat_formula);
				if(list->reformat_formula!=NULL) Rfree(list->reformat_formula);
				if(list->desc!=NULL) Rfree(list->desc);
			}
			Rfree(rscrs->lists);
			rscrs->lists=NULL;
			rscrs->numlists=0;
		}
		if(rscrs->loads!=NULL)
		{
			for(x=0,load=rscrs->loads;x<rscrs->numloads;++x,++load)
			{
				if(load->module!=NULL) Rfree(load->module);
				if(load->name!=NULL) Rfree(load->name);
				if(load->incvir!=NULL)
				{
					for(y=0,i=load->incvir;y<load->numincvirs;++y,++i)
					{
						if(i->module!=NULL) Rfree(i->module);
						if(i->file!=NULL) Rfree(i->file);
						if(i->keyname!=NULL) Rfree(i->keyname);
					}
					Rfree(load->incvir);
				}
				if(load->conname!=NULL) Rfree(load->conname);
				if(load->dlist!=NULL) freeapplib(load->dlist);
				if(load->desc!=NULL) Rfree(load->desc);
				if(load->mlist!=NULL) freeapplib(load->mlist);
			}
			Rfree(rscrs->loads);
			rscrs->loads=NULL;
			rscrs->numloads=0;
		}
		Rfree(rscrs);
	}
}
/*---------------------------------------------------------------------------
	This function adds widgets to a structure specified as RDAscrn
---------------------------------------------------------------------------*/
void xaddwdgt(RDAscrn *screen,short type,char *resource_name,char *label,
	char *pixmap,char *XHTML_Label,short rows,short cols,int rtype,
	char *expression,char *editable_expression,char *sensitive_expression,
	char *transversal_expression,int line,char *file)
{
	RDAwdgt *wdgt=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG addwdgt [%s] type [%s] label [%s] pixmap [%s] XHTML_Label [%s] rows [%d] cols [%d] to screen [%s] at line [%d] program [%s].",
			(resource_name!=NULL?resource_name:""),
			wdgttypes[type],
			(label!=NULL?label:""),
			(pixmap!=NULL?pixmap:""),
			(XHTML_Label!=NULL?XHTML_Label:""),
			rows,cols,
			(screen->name!=NULL?screen->name:""),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(screen->wdgts!=NULL)
	{
		screen->wdgts=Rrealloc((void *)screen->wdgts,(screen->numwdgts+1)*
			sizeof(RDAwdgt));
	} else {
		screen->wdgts=Rmalloc(sizeof(RDAwdgt));
	}
	wdgt=screen->wdgts+screen->numwdgts;
	++screen->numwdgts;
	wdgt->type=type;
	if(XHTML_Label!=NULL) 
	{
		wdgt->XHTML_Label=stralloc(XHTML_Label);
	} else {
		wdgt->XHTML_Label=NULL;
	}
	if(resource_name!=NULL)
	{
		 wdgt->resource_name=stralloc(resource_name);
	} else {
		wdgt->resource_name=NULL;
	}
	wdgt->resource_num=0;
	wdgt->rtype=rtype;
	if(label!=NULL)
	{
		 wdgt->label=stralloc(label);
	} else {
		wdgt->label=NULL;
	}
	if(pixmap!=NULL)
	{
		 wdgt->pixmap=stralloc(pixmap);
	} else {
		wdgt->pixmap=NULL;
	}
	wdgt->cols=cols;
	wdgt->rows=rows;
	if(!isEMPTY(expression))
	{
		wdgt->expression=stralloc(expression);
	} else { 
		wdgt->expression=NULL;
	}
	if(!isEMPTY(editable_expression))
	{
		wdgt->editable_expression=stralloc(editable_expression);
	} else { 
		wdgt->editable_expression=NULL;
	}
	if(!isEMPTY(sensitive_expression))
	{
		wdgt->sensitive_expression=stralloc(sensitive_expression);
	} else {
		wdgt->sensitive_expression=NULL;
	}
	if(!isEMPTY(transversal_expression))
	{
		wdgt->transversal_expression=stralloc(transversal_expression);
	} else {
		wdgt->transversal_expression=NULL;
	}
}
/*---------------------------------------------------------------------------
	This function adds resources to the structure specified as RDArsrc
---------------------------------------------------------------------------*/
void xaddrsrc(RDArsrc *resource,short type,char *rscr,short field_type,
	unsigned field_length,void *value,char editable,void (*function)(...),
	short items,char ***list,void *arglist,void (*helpfunction)(...),
	short function_type,int line,char *file)
{
	RDArmem *member;
	short x;

	if(resource==NULL)
	{
		prterr("Error in addrsrc RDArsrc at line [%d] program [%s].  Pointer is NULL.",line,file);
		return;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG addrsrc [%d] [%s] type [%d] [%s] field type [%d] [%s] length [%d] editable [%s] no items [%d] to Screen [%s] [%s] at line [%d] program [%s].",
			resource->numrscs,(rscr!=NULL ? rscr:""),
			type,membertypes[type],field_type,
			standardfieldtypes[field_type],field_length,(editable ? 
			"Yes":"No"),items,resource->module,resource->screen,
			line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(resource->rscs!=NULL)
	{
		resource->rscs=Rrealloc((void *)resource->rscs,(resource->numrscs+1)*sizeof(RDArmem));
	} else resource->rscs=Rmalloc(sizeof(RDArmem));	
	member=resource->rscs+resource->numrscs;
	++resource->numrscs;
	member->type=type;
	member->rscrname=stralloc(rscr);
	member->field_type=field_type;
	member->field_length=field_length;
	member->editable=editable;
	member->sensitive=TRUE;
	member->function=function;
	member->items=items;
	member->rtype=0;
	member->w=NULL;
	member->arglist=arglist;
	member->parent=resource;
	member->helpfunction=helpfunction;
	member->user_editable=editable;
	member->editable_expression=NULL;
	member->user_sensitive=TRUE;
	member->sensitive_expression=NULL;
	member->transversal_expression=NULL;
	member->app_update=FALSE;
	member->label=NULL;
	member->XHTML_Label=NULL;
	member->definition=NULL;
	member->toolbar_w=NULL;
	member->popup_w=NULL;
	member->popup_id=(-1);

	member->Table=NULL;
	member->table_row=0;
	member->table_row_span=0;
	member->table_col=0;
	member->table_col_span=0;
	member->validobject=NULL;
	member->wSIM=NULL;
	member->tCell=NULL;
	member->function_type=function_type;
	if(field_type==CUSTOMTYPE) member->callbacktype=TRUE;
		else member->callbacktype=FALSE;
	switch(field_type)
	{
		case SCROLLEDLIST:
			member->list=list;
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] has [%d] items at line [%d] program [%s].",member->rscrname,items,line,file);
				for(x=0;x<items;++x)
				{
					prterr("DIAG Resource [%s] scrolled list item #%d [%s].",member->rscrname,x,*(list[0]+x));
				}
			}
#endif /* USE_RDA_DIAGNOSTICS */
			member->value.integer_value=Rmalloc(sizeof(int));
			member->dlen=sizeof(int);
			if(value!=NULL)
			{
				*member->value.integer_value=*(int *)value;
			} else *member->value.integer_value=0;
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] has value [%d].",member->rscrname,*member->value.integer_value);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			break;
		case SCROLLEDTEXT:
		case VARIABLETEXT:
		case DATES:
		case TIMES:
		case SOCSECNUM:
		case PLAINTEXT:
		case PHONE:
		case CUSTOMTYPE:
		case ZIPCODE:
		case EXPENDITURE:
		case REVENUE:
		case BALANCESHEET:
		case BEGINNINGBALANCE:
		case OPTIONALFIELDS:
			if(!isEMPTY(value))
			{
				member->value.string_value=stralloc((char *)value);
				member->dlen=RDAstrlen(member->value.string_value)+1;
			} else {
				member->value.string_value=NULL;
				member->dlen=0;
			}
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] has value [%s].",member->rscrname,(member->value.string_value!=NULL ? member->value.string_value:""));
			}
#endif /* USE_RDA_DIAGNOSTICS */
			break;
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case DECIMALV:
		case DOUBLEV:
		case SDOUBLEV:
		case SDECIMALV:
			member->dlen=sizeof(double);
			member->value.float_value=Rmalloc(sizeof(double));
			if(value!=NULL)
			{
				*member->value.float_value=*(double *)value;
			} else *member->value.float_value=0;
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] has value [%f].",member->rscrname,*member->value.float_value);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			break;
		case SHORTV:
		case SSHORTV:
			member->dlen=sizeof(short);
			member->value.short_value=Rmalloc(sizeof(short));
			if(value!=NULL)
			{
				*member->value.short_value=*(short *)value;
			} else *member->value.short_value=0;
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] has value [%d].",member->rscrname,*member->value.short_value);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			break;
		case PROGRESS_BAR:
		case LONGV:
		case SLONGV:
			member->dlen=sizeof(int);
			member->value.integer_value=Rmalloc(sizeof(int));
			if(value!=NULL)
			{
				*member->value.integer_value=*(int *)value;
			} else *member->value.integer_value=0;
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] has value [%d].",member->rscrname,*member->value.integer_value);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			break;
		case CHARACTERS:
			member->dlen=1;
			member->value.string_value=Rmalloc(1);	
			*member->value.string_value=*(char *)value;
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] value [%c].",
					member->rscrname,
					*member->value.string_value); 
			}
#endif /* USE_RDA_DIAGNOSTICS */
			break;
		case BOOLNS:
			member->dlen=1;
			member->value.string_value=Rmalloc(1);	
			if(*(char *)value) *member->value.string_value=TRUE;
				else *member->value.string_value=FALSE;
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] has value [%s].",
					member->rscrname,
					(*member->value.string_value ? 
					"True":"False"));
			}
#endif /* USE_RDA_DIAGNOSTICS */
			break;
		case BUTTONS:
			member->dlen=0;
			break;
		default:
			prterr("Error field type [%d] is invalid for Resource [%s] on Screen [%s] [%s] at line [%d] program [%s].",member->field_type,member->rscrname,resource->module,resource->screen,line,file);
			break;
	}
}
short xupdatemember(RDArmem *member,int line,char *file)
{
	char *value=NULL,*temp1=NULL,*temp2=NULL;
	void (*func)(...)=NULL;
	RDArsrc *rs;
	Wt::WString *text=NULL;
	Wt::WComboBox *CB=NULL;
	Wt::WSelectionBox *SB=NULL;
	Wt::WTextArea *TA=NULL;
	Wt::WTextEdit *TE=NULL;
	Wt::WLineEdit *LE=NULL;
	Wt::WWebWidget *WW=NULL;
	Wt::WDateEdit *DE=NULL;
	Wt::WText *wText=NULL;
	Wt::WStandardItem *wSI=NULL;
	Wt::WDate wDate;
	Wt::WProgressBar *pBar=NULL;
	Wt::WAbstractToggleButton *aTB=NULL;
	char *temp_str=NULL,*temp=NULL;
	int x=0,cp=0;
	std::stringstream ss1;
	int z=0;
	int Dm=0,Dd=0,Dy=0;
	double pMin=0.0,pMax=0.0,pVal=0.0;

	if(member==NULL)
	{
		prterr("Error in updatemember, member is NULL at line [%d] program [%s].",line,file);
		return(FALSE);
	}
	rs=member->parent;
	if(!IsScreenDisplayed(rs)) return(FALSE);
	member->app_update=TRUE;
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG updatemember Updating Resource [%s] on Screen [%s] [%s] at line [%d] program [%s].",member->rscrname,rs->module,rs->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(member->type<1 && ((!USER_INTERFACE && member->w!=NULL)))
	{
		WW=(Wt::WWebWidget *)member->w;
		switch(member->field_type)
		{
			case SCROLLEDLIST:
				CB=(Wt::WComboBox *)member->w;
				SB=(Wt::WSelectionBox *)member->w;
				if(member->function!=NULL)
				{
					func=member->function;
					member->function=NULL;
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Resource [%s] Type [%d] [%s] Field Type [%d] [%s] has [%d] items.",member->rscrname,member->type,membertypes[member->type],member->field_type,standardfieldtypes[member->field_type],member->items);
				}
#endif /* USE_RDA_DIAGNOSTICS */

				if(member->items>0)
				{
					if(member->rows>1)
					{
						SB->clearSelection();
						if(member->wSIM!=NULL)
						{
							member->wSIM->clear();
							z=0;
							temp1=NULL;
							for(x=0;x<member->items;++x)
							{
								wSI=new WStandardItem();
								wSI->setFlags(wSI->flags() | Wt::ItemIsXHTMLText);
								temp1=((*(member->list[0]+x)));
								temp_str=RDA_EncodeWhiteSpace(temp1);
								text = new Wt::WString(temp_str,UTF8);
								if(temp_str!=NULL) Rfree(temp_str);
								wSI->setText(*text);
								text->~WString();
								member->wSIM->appendRow(wSI);
							}
						} else {
							CB->clear();
							for(x=0;x<member->items;++x)
							{
								text = new Wt::WString((*(member->list[0]+x)),UTF8);
								CB->addItem(*text);
								text->~WString();
							}
						}
						SB->setVerticalSize(member->rows);
						CB->setCurrentIndex(*member->value.integer_value);
#ifdef USE_RDA_DIAGNOSTICS
						if(diaggui || diaggui_field)
						{
							prterr("DIAG Resource [%s] has value [%d].",member->rscrname,*member->value.integer_value);
						}
#endif /* USE_RDA_DIAGNOSTICS */
					} else {
						CB->clear();
						for(x=0;x<member->items;++x)
						{
							text = new Wt::WString((*(member->list[0]+x)),UTF8);
							CB->addItem(*text);
							text->~WString();
						}
						CB->setCurrentIndex(*member->value.integer_value);
						CB->setValueText(CB->itemText(*member->value.integer_value));
#ifdef USE_RDA_DIAGNOSTICS
						if(diaggui || diaggui_field)
						{
							prterr("DIAG Resource [%s] has value [%d].",member->rscrname,*member->value.integer_value);
						}
#endif /* USE_RDA_DIAGNOSTICS */
					}
				} else {
					prterr("Error Resource [%s] on Screen [%s] [%s] Field Type [%s] has no items in it's scrolled list at line [%d] program [%s].  A List must have at least 1 item.",
						member->rscrname,
						rs->module,rs->screen,
						standardfieldtypes[member->field_type],
						line,file);
				}
				member->function=func;
				break;
			case SCROLLEDTEXT:
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Resource [%s] is type [%d] [%s] has field type [%d] [%s] length [%d] value [%s].",
						member->rscrname,member->type,
						membertypes[member->type],
						member->field_type,
						standardfieldtypes[member->field_type],
						member->field_length,
						standardfieldtypes[member->field_type],
						(member->value.string_value!=NULL ? member->value.string_value:"")); 
				}
#endif /* USE_RDA_DIAGNOSTICS */
				if(!USER_INTERFACE)
				{
					if(!isEMPTY(member->value.string_value))
					{
						text=new Wt::WString(member->value.string_value);
					} else text=new Wt::WString("");
					if(member->rtype<2) 
					{
						TA=(Wt::WTextArea *)member->w;	
						TA->setText(*text);
					} else {
						TE=(Wt::WTextEdit *)member->w;
						TE->setText(*text);
					}
					if(text!=NULL) text->~WString();
				}
				break;
			case VARIABLETEXT:
			case PLAINTEXT:
			case CHARACTERS:
			case OPTIONALFIELDS:
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Resource [%s] is type [%d] [%s] has field type [%d] [%s] length [%d] value [%s].",
						member->rscrname,member->type,
						membertypes[member->type],
						member->field_type,
						standardfieldtypes[member->field_type],
						member->field_length,
						(member->value.string_value!=NULL ? member->value.string_value:"")); 
				}
#endif /* USE_RDA_DIAGNOSTICS */
				if(!USER_INTERFACE)
				{
					LE=(Wt::WLineEdit *)member->w;
					if(!isEMPTY(member->value.string_value))
					{
						text=new Wt::WString(member->value.string_value);
					} else text=new Wt::WString(""); 
					LE->setText(*text);
					if(text!=NULL) text->~WString();
					if(member->validobject!=NULL) LE->validate();
				}
				break;
			case DATES:
				if(!USER_INTERFACE)
				{
					if(member->rtype==0)
					{
						LE=(Wt::WLineEdit *)member->w;
						if(!isEMPTY(member->value.string_value))
						{
							text=new Wt::WString(member->value.string_value);
						} else text=new Wt::WString("");
						LE->setText(*text);
						if(text!=NULL) text->~WString();
						LE->validate();
					} else {
						DE=(Wt::WDateEdit *)member->w;
						if(!isEMPTY(member->value.string_value))
						{
							sprintf(stemp,"%.2s",&member->value.string_value[0]);
							Dm=atoi(stemp);
							sprintf(stemp,"%.2s",&member->value.string_value[3]);
							Dd=atoi(stemp);
							sprintf(stemp,"%.4s",&member->value.string_value[6]);
							Dy=atoi(stemp);
						} else {
							sprintf(stemp,"%.2s",&CURRENT_DATE10[0]);
							Dm=atoi(stemp);
							sprintf(stemp,"%.2s",&CURRENT_DATE10[3]);
							Dd=atoi(stemp);
							sprintf(stemp,"%.4s",&CURRENT_DATE10[6]);
							Dy=atoi(stemp);
						}
						wDate=Wt::WDate(Dy,Dm,Dd);
						DE->setDate(wDate);	
						
					}
				}
				break;
			case TIMES:
				if(!USER_INTERFACE)
				{
					if(member->rtype==0)
					{
						LE=(Wt::WLineEdit *)member->w;
						if(!isEMPTY(member->value.string_value))
						{
							text=new Wt::WString(member->value.string_value);
						} else text=new Wt::WString("");
						LE->setText(*text);
						if(text!=NULL) text->~WString();
						if(member->validobject!=NULL) LE->validate();
					} else {
					}
				}
				break;
			case SOCSECNUM:
			case PHONE:
			case CUSTOMTYPE:
			case ZIPCODE:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Resource [%s] is type [%d] [%s] has field type [%d] [%s] length [%d] value [%s].",
						member->rscrname,member->type,
						membertypes[member->type],
						member->field_type,
						standardfieldtypes[member->field_type],
						member->field_length,
						(member->value.string_value!=NULL ? member->value.string_value:"")); 
				}
#endif /* USE_RDA_DIAGNOSTICS */
				LE=(Wt::WLineEdit *)member->w;
				if(!isEMPTY(member->value.string_value))
				{
					text=new Wt::WString(member->value.string_value);
				} else text=new Wt::WString("");
				LE->setText(*text);
				if(text!=NULL) text->~WString();
				if(member->validobject!=NULL) LE->validate();
				break;
			case DOLLARS_NOCENTS:
				value=famtncents((*member->value.float_value*100),
					(member->cols?member->cols:member->field_length),
					' ');
				unpad(value);
				if(!USER_INTERFACE)
				{
					LE=(Wt::WLineEdit *)member->w;
					text=new Wt::WString(value);
					LE->setText(*text);
					cp=(member->cols?member->cols:member->field_length)-1;
					ss1 << WW->jsRef() << ".selectionStart = " << cp-1 << ";" << WW->jsRef() << ".selectionEnd = " << cp << ";";
					WW->doJavaScript(ss1.str());
					if(text!=NULL) text->~WString();
					if(member->validobject!=NULL) LE->validate();
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Resource [%s] is type [%d] [%s] has field type [%d] [%s] length [%d] value [%s].",
						member->rscrname,
						member->type,
						membertypes[member->type],
						member->field_type,
						standardfieldtypes[member->field_type],
						member->field_length,value);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case DOLLARS:
				value=famt(*member->value.float_value,
					(member->cols?member->cols:member->field_length));
				unpad(value);
				if(!USER_INTERFACE)
				{
					LE=(Wt::WLineEdit *)member->w;
					text=new Wt::WString(value);
					LE->setText(*text);
					cp=(member->cols?member->cols:member->field_length)-1;
					ss1 << WW->jsRef() << ".selectionStart = " << cp-1 << ";" << WW->jsRef() << ".selectionEnd = " << cp << ";";
					WW->doJavaScript(ss1.str());
					if(text!=NULL) text->~WString();
					if(member->validobject!=NULL) LE->validate();

				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Resource [%s] is type [%d] [%s] has field type [%d] [%s] length [%d] value [%s].",
						member->rscrname,
						member->type,
						membertypes[member->type],
						member->field_type,
						standardfieldtypes[member->field_type],
						member->field_length,value);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SDECIMALV:
				value=floatamt(*member->value.float_value,
					(member->cols?(member->cols-1):(member->field_length-1)));
				unpad(value);
				if(!USER_INTERFACE)
				{
					LE=(Wt::WLineEdit *)member->w;
					text=new Wt::WString(value);
					LE->setText(*text);
					cp=(member->cols?member->cols:member->field_length)-1;
					ss1 << WW->jsRef() << ".selectionStart = " << cp-1 << ";" << WW->jsRef() << ".selectionEnd = " << cp << ";";
					WW->doJavaScript(ss1.str());
					if(text!=NULL) text->~WString();
					if(member->validobject!=NULL) LE->validate();
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Resource [%s] is type [%d] [%s] has field type [%d] [%s] length [%d] value [%s].",
						member->rscrname,
						member->type,
						membertypes[member->type],
						member->field_type,
						standardfieldtypes[member->field_type],
						member->field_length,value);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case DECIMALV:
				value=ufloatamt(*member->value.float_value,
					(member->cols?member->cols:member->field_length));
				unpad(value);
				if(!USER_INTERFACE)
				{
					LE=(Wt::WLineEdit *)member->w;
					text=new Wt::WString(value);
					LE->setText(*text);
					if(text!=NULL) text->~WString();
					if(member->validobject!=NULL) LE->validate();
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Resource [%s] Type [%d] [%s] Field type [%d] [%s] length [%d] value [%s].",
						member->rscrname,
						member->type,
						membertypes[member->type],
						member->field_type,
						standardfieldtypes[member->field_type],
						member->field_length,value);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SDOUBLEV:
				value=floatamt(*member->value.float_value,
					(member->cols?(member->cols-1):(member->field_length-1)));
				unpad(value);
				if(!USER_INTERFACE)
				{
					LE=(Wt::WLineEdit *)member->w;
					text=new Wt::WString(value);
					LE->setText(*text);
					cp=(member->cols?member->cols:member->field_length)-1;
					ss1 << WW->jsRef() << ".selectionStart = " << cp-1 << ";" << WW->jsRef() << ".selectionEnd = " << cp << ";";
					WW->doJavaScript(ss1.str());
					if(text!=NULL) text->~WString();
					if(member->validobject!=NULL) LE->validate();
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Resource [%s] Type [%d] [%s] Field Type [%d] [%s] length [%d] value [%s].",
						member->rscrname,
						member->type,
						membertypes[member->type],
						member->field_type,
						standardfieldtypes[member->field_type],
						member->field_length,value);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case DOUBLEV:
				if(member->field_length>member->cols)
				{
					value=Rmalloc(member->field_length+1);
				} else value=Rmalloc(member->cols+1);
				sprintf(value,"%*.0f",(member->cols ?
					member->cols:member->field_length),
					*member->value.float_value);
				if(!USER_INTERFACE)
				{
					LE=(Wt::WLineEdit *)member->w;
					text=new Wt::WString(value);
					LE->setText(*text);
					if(text!=NULL) text->~WString();
					if(member->validobject!=NULL) LE->validate();
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Resource [%s] Type [%d] [%s] Field Type [%d] [%s] length [%d] value [%s].",
						member->rscrname,
						member->type,
						membertypes[member->type],
						member->field_type,
						standardfieldtypes[member->field_type],
						member->field_length,value);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SHORTV:
				memset(stemp,0,101);
				sprintf(stemp,"%*d",(member->cols ? member->cols:member->field_length),*member->value.short_value);
				value=stralloc(stemp);
				if(!USER_INTERFACE)
				{
					LE=(Wt::WLineEdit *)member->w;
					text=new Wt::WString(value);
					LE->setText(*text);
					if(text!=NULL) text->~WString();
					if(member->validobject!=NULL) LE->validate();
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Resource [%s] Type [%d] [%s] Field Type [%d] [%s] length [%d] value [%s].",
						member->rscrname,
						member->type,
						membertypes[member->type],
						member->field_type,
						standardfieldtypes[member->field_type],
						member->field_length,value);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SSHORTV:
				value=sintamt(*member->value.short_value,
					(member->cols?member->cols:member->field_length));
				unpad(value);
				if(!USER_INTERFACE)
				{
					LE=(Wt::WLineEdit *)member->w;
					text=new Wt::WString(value);
					LE->setText(*text);
					cp=(member->cols?member->cols:member->field_length)-1;
					ss1 << WW->jsRef() << ".selectionStart = " << cp-1 << ";" << WW->jsRef() << ".selectionEnd = " << cp << ";";
					WW->doJavaScript(ss1.str());
					if(text!=NULL) text->~WString();
					if(member->validobject!=NULL) LE->validate();
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Resource [%s] Type [%d] [%s] Field Type [%d] [%s] length [%d] value [%s].",
						member->rscrname,
						member->type,
						membertypes[member->type],
						member->field_type,
						standardfieldtypes[member->field_type],
						member->field_length,value);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case LONGV:
				memset(stemp,0,101);
				sprintf(stemp,"%*d",(member->cols ? member->cols:member->field_length),*member->value.integer_value);
				value=stralloc(stemp);
				if(!USER_INTERFACE)
				{
					LE=(Wt::WLineEdit *)member->w;
					text=new Wt::WString(value);
					LE->setText(*text);
					if(text!=NULL) text->~WString();
					if(member->validobject!=NULL) LE->validate();
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Resource [%s] Type [%d] [%s] Field Type [%d] [%s] length [%d] value [%s].",
						member->rscrname,
						member->type,
						membertypes[member->type],
						member->field_type,
						standardfieldtypes[member->field_type],
						member->field_length,value);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case PROGRESS_BAR:
				if(!USER_INTERFACE)
				{
					pBar=(Wt::WProgressBar *)member->w;
					pVal=(*member->value.integer_value);
					pMin=0;
					pMax=member->items;
					pBar->setRange(pMin,pMax);
					pBar->setValue(pVal);
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Resource [%s] Type [%d] [%s] Field Type [%d] [%s] length [%d] value [%s].",
						member->rscrname,
						member->type,
						membertypes[member->type],
						member->field_type,
						standardfieldtypes[member->field_type],
						member->field_length,value);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case SLONGV:
				value=sintamt(*member->value.integer_value,
					(member->cols?member->cols:member->field_length));
				if(!USER_INTERFACE)
				{
					LE=(Wt::WLineEdit *)member->w;
					text=new Wt::WString(value);
					LE->setText(*text);
					cp=(member->cols?member->cols:member->field_length)-1;
					ss1 << WW->jsRef() << ".selectionStart = " << cp-1 << ";" << WW->jsRef() << ".selectionEnd = " << cp << ";";
					WW->doJavaScript(ss1.str());
					if(text!=NULL) text->~WString();
					if(member->validobject!=NULL) LE->validate();
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Resource [%s] Type [%d] [%s] Field Type [%d] [%s] length [%d] value [%s].",
						member->rscrname,
						member->type,
						membertypes[member->type],
						member->field_type,
						standardfieldtypes[member->field_type],
						member->field_length,
						value);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				break;
			case BOOLNS:
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Resource [%s] Type [%d] [%s] Field Type [%d] [%s] length [%d] value [%s].",
						member->rscrname,	
						member->type,
						membertypes[member->type],
						member->field_type,
						standardfieldtypes[member->field_type],
						member->field_length,
						(member->value.string_value?"Yes":"No"));
				}
#endif /* USE_RDA_DIAGNOSTICS */
				aTB=(Wt::WAbstractToggleButton *)member->w;
				aTB->setChecked((*member->value.string_value ? TRUE:FALSE));
				break;
			default:
				prterr("Error Field Type [%d] is invalid for Resource [%s] on Screen [%s] [%s] at line [%d] program [%s].",
					member->field_type,
					member->rscrname,
					rs->module,rs->screen,
					line,file);
				member->app_update=FALSE;
				return(FALSE);
		}
		if(value!=NULL) Rfree(value);
	}
	member->app_update=FALSE;
	return(TRUE);
}
/*---------------------------------------------------------------------------
	updateallrsrc	Function to update all resources for a window
---------------------------------------------------------------------------*/
void xupdateallrsrc(RDArsrc *rsrc,int line,char *file)
{	
	RDArmem *member;
	short x;
	
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui) { prterr("DIAG updateallrsrc Updating All Resources on Screen [%s] [%s] at line# [%d] program [%s].",rsrc->module,rsrc->screen,line,file); }
#endif /* USE_RDA_DIAGNOSTICS */
	if(rsrc!=NULL)
	{
		if(rsrc->rscs!=NULL)
		{
			for(x=0,member=rsrc->rscs;x<rsrc->numrscs;++x,++member)
			{
				if(member->type<1 && (member->w!=NULL)) updatemember(member);
			}	
		}
	}
}
/*---------------------------------------------------------------------------
	SetAllLoadsEditableAndSensitive - Set's the editable and sensitive 
	values for all RDALoadValue resources......
---------------------------------------------------------------------------*/
void xSetAllLoadsEditableAndSensitive(RDArsrc *rsrc,int line,char *file)
{
	RDArmem *load_member=NULL;
	RDArmem *connect_member=NULL;
	int x=0,c=(-1),l=(-1);
	RDALoadValue *load=NULL;
	char *temp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG SetAllLoadsEditableAndSensitive for Window [%s][%s] at line [%d] program [%s].",rsrc->module,rsrc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(rsrc!=NULL)
	{
		if(rsrc->loads!=NULL)
		{
			for(x=0,load=rsrc->loads;x<rsrc->numloads;++x,++load)
			{
				c=FINDRSC(rsrc,load->conname);
				if(c>(-1))
				{
					connect_member=rsrc->rscs+c;
				} else {
					connect_member=NULL;
				}
				if(temp!=NULL) Rfree(temp);
				temp=Rmalloc(RDAstrlen(load->name)+12);
				sprintf(temp,"%s LOAD VALUE",load->name);
				l=FINDRSC(rsrc,temp);
				if(l>(-1))
				{
					load_member=rsrc->rscs+l;
				} else {
					load_member=NULL;
				}
				if(connect_member!=NULL && load_member!=NULL)
				{
					if(isEMPTY(load_member->editable_expression))
					{
						if(connect_member->editable==FALSE || connect_member->user_editable==FALSE)
						{
							FINDRSCSETEDITABLE(rsrc,temp,FALSE);
						} else {
							FINDRSCSETEDITABLE(rsrc,temp,TRUE);
						}
					}
					if(isEMPTY(load_member->sensitive_expression))
					{
						if(connect_member->sensitive==FALSE || connect_member->user_sensitive==FALSE)
						{
							FINDRSCSETSENSITIVE(rsrc,temp,FALSE);
						} else {
							FINDRSCSETSENSITIVE(rsrc,temp,TRUE);
						}
					}
				}
				if(temp!=NULL) Rfree(temp);
				temp=Rmalloc(RDAstrlen(load->name)+15);
				sprintf(temp,"%s LOAD OPERATOR",load->name);
				l=FINDRSC(rsrc,temp);
				if(l>(-1))
				{
					load_member=rsrc->rscs+l;
				} else {
					load_member=NULL;
				}
				if(connect_member!=NULL && load_member!=NULL)
				{
					if(isEMPTY(load_member->editable_expression))
					{
						if(connect_member->editable==FALSE || connect_member->user_editable==FALSE)
						{
							FINDRSCSETEDITABLE(rsrc,temp,FALSE);
						} else {
							FINDRSCSETEDITABLE(rsrc,temp,TRUE);
						}
					}
					if(isEMPTY(load_member->sensitive_expression))
					{
						if(connect_member->sensitive==FALSE || connect_member->user_sensitive==FALSE)
						{
							FINDRSCSETSENSITIVE(rsrc,temp,FALSE);
						} else {
							FINDRSCSETSENSITIVE(rsrc,temp,TRUE);
						}
					}
				}
				if(temp!=NULL) Rfree(temp);
				temp=Rmalloc(RDAstrlen(load->name)+14);
				sprintf(temp,"%s LOAD GROUPER",load->name);
				l=FINDRSC(rsrc,temp);
				if(l>(-1))
				{
					load_member=rsrc->rscs+l;
				} else {
					load_member=NULL;
				}
				if(connect_member!=NULL && load_member!=NULL)
				{
					if(isEMPTY(load_member->editable_expression))
					{
						if(connect_member->editable==FALSE || connect_member->user_editable==FALSE)
						{
							FINDRSCSETEDITABLE(rsrc,temp,FALSE);
						} else {
							FINDRSCSETEDITABLE(rsrc,temp,TRUE);
						}
					}
					if(isEMPTY(load_member->sensitive_expression))
					{
						if(connect_member->sensitive==FALSE || connect_member->user_sensitive==FALSE)
						{
							FINDRSCSETSENSITIVE(rsrc,temp,FALSE);
						} else {
							FINDRSCSETSENSITIVE(rsrc,temp,TRUE);
						}
					}
				}
				if(temp!=NULL) Rfree(temp);
				temp=Rmalloc(RDAstrlen(load->name)+14);
				sprintf(temp,"%s LOAD CONTROL",load->name);
				l=FINDRSC(rsrc,temp);
				if(l>(-1))
				{
					load_member=rsrc->rscs+l;
				} else {
					load_member=NULL;
				}
				if(connect_member!=NULL && load_member!=NULL)
				{
					if(isEMPTY(load_member->editable_expression))
					{
						if(connect_member->editable==FALSE || connect_member->user_editable==FALSE)
						{
							FINDRSCSETEDITABLE(rsrc,temp,FALSE);
						} else {
							FINDRSCSETEDITABLE(rsrc,temp,TRUE);
						}
					}
					if(isEMPTY(load_member->sensitive_expression))
					{
						if(connect_member->sensitive==FALSE || connect_member->user_sensitive==FALSE)
						{
							FINDRSCSETSENSITIVE(rsrc,temp,FALSE);
						} else {
							FINDRSCSETSENSITIVE(rsrc,temp,TRUE);
						}
					}
				}
			}
		}
	}
}
/*---------------------------------------------------------------------------
	SetAllListEditableAndSensitive - Set's the editable and sensitive 
	values for all RDAScrolledList resources......
---------------------------------------------------------------------------*/
void xSetAllListEditableAndSensitive(RDArsrc *rsrc,int line,char *file)
{
	RDArmem *list_member=NULL;
	RDArmem *connect_member=NULL;
	int x=0,c=(-1),l=(-1);
	RDAScrolledList *list=NULL;
	char *temp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG SetAllListEditableAndSensitive for Window [%s][%s] at line [%d] program [%s].",rsrc->module,rsrc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(rsrc!=NULL)
	{
		if(rsrc->lists!=NULL)
		{
			for(x=0,list=rsrc->lists;x<rsrc->numlists;++x,++list)
			{
				if(!list->contype) /* connecting TO a file field */
				{
					temp=Rmalloc(RDAstrlen(list->confil)+RDAstrlen(list->confld)+7);
					sprintf(temp,"[%s][%s]",(list->confil!=NULL?list->confil:""),(list->confld!=NULL?list->confld:""));
				} else {  
					temp=stralloc(list->conmod);
				}
				c=FINDRSC(rsrc,temp);
				if(c>(-1))
				{
					connect_member=rsrc->rscs+c;
				} else {
					connect_member=NULL;
				}
				if(temp!=NULL) Rfree(temp);
				l=FINDRSC(rsrc,list->name);
				if(l>(-1))
				{
					list_member=rsrc->rscs+l;
				} else {
					list_member=NULL;
				}
				if(connect_member!=NULL && list_member!=NULL)
				{
					if(isEMPTY(list_member->editable_expression))
					{
						if(connect_member->editable==FALSE || connect_member->user_editable==FALSE)
						{
							FINDRSCSETEDITABLE(rsrc,list->name,FALSE);
						} else {
							FINDRSCSETEDITABLE(rsrc,list->name,TRUE);
						}
					}
					if(isEMPTY(list_member->sensitive_expression))
					{
						if(connect_member->sensitive==FALSE || connect_member->user_sensitive==FALSE)
						{
							FINDRSCSETSENSITIVE(rsrc,list->name,FALSE);
						} else {
							FINDRSCSETSENSITIVE(rsrc,list->name,TRUE);
						}
					}
				}
			}
		}
	}
}
/*---------------------------------------------------------------------------
	SetAllEditableAndSensitive - Set's the editable and sensitive values for
	all member resources......
---------------------------------------------------------------------------*/
void xSetAllEditableAndSensitive(RDArsrc *rsrc,int line,char *file)
{
	RDArmem *member=NULL;
	int x=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG SetAllEditableAndSensitive for Window [%s][%s] at line [%d] program [%s].",rsrc->module,rsrc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(rsrc!=NULL)
	{
		for(x=0,member=rsrc->rscs;x<rsrc->numrscs;++x,++member)
		{
			if(member->editable) 
			{
				if(!isEMPTY(member->editable_expression))
				{
					if(rsrc->EvalFuncArgs!=NULL)
					{
/*lint -e746*/
						member->user_editable=rsrc->EvalFunc(member->editable_expression,rsrc,rsrc->EvalFuncArgs);
/*lint +e746*/
					} else {
/*lint -e746*/
						member->user_editable=rsrc->EvalFunc(member->editable_expression,rsrc);
/*lint +e746*/
					}
					ADVMEMBERSETEDITABLE(member,member->user_editable,TRUE);
				} else {
					member->user_editable=member->editable;
				}
			} 
			if(member->sensitive)
			{
				if(!isEMPTY(member->sensitive_expression))
				{
					if(rsrc->EvalFuncArgs!=NULL)
					{
/*lint -e746*/
						member->user_sensitive=rsrc->EvalFunc(member->sensitive_expression,rsrc,rsrc->EvalFuncArgs);
/*lint +e746*/
					} else {
/*lint -e746*/
						member->user_sensitive=rsrc->EvalFunc(member->sensitive_expression,rsrc);
/*lint +e746*/
					}
					if(!USER_INTERFACE)
					{

						ADVMEMBERSETSENSITIVE(member,member->user_sensitive,TRUE);
#ifdef XXX
						if(member->w!=NULL)
						{
							WWidget_setEnabled((WWidget *)member->w,(member->user_sensitive ? TRUE:FALSE));
						}
#endif /* XXX */
					} else {
						prterr("Error Character Interface doesn't support FINDRSCSETSENSITIVE.");
					}
				} else {
					member->user_sensitive=member->sensitive;
				}
			}
		}
		SetAllListEditableAndSensitive(rsrc);
		SetAllLoadsEditableAndSensitive(rsrc);
	}
}
/*---------------------------------------------------------------------------
	updatersrc	Function updates a window(screen) widget with the
			value of it's resource.
---------------------------------------------------------------------------*/
short xupdatersrc(RDArsrc *rsrc,char *fieldname,int line,char *file)
{
	RDArmem *member;
	short x;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui) 
	{ 
		prterr("DIAG updatersrc Updating Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(fieldname!=NULL ? fieldname:""),rsrc->module,rsrc->screen,line,file); 
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if((x=FINDRSC(rsrc,fieldname))==(-1))
	{
		prterr("Error Resource [%s] not found on Screen [%s] [%s] at line [%d] program [%s]. Cannot update specified resource.",fieldname,rsrc->module,rsrc->screen,line,file);
		return(FALSE);
	}
	member=rsrc->rscs+x;
	return(updatemember(member));
}
/*---------------------------------------------------------------------------
	writescrnbin 	Function creates a bin file from screen structure.
---------------------------------------------------------------------------*/
short xwritescrnbin(char *libname,RDAscrn *screen,int line,char *file)
{
	short x;
	BIN *bin;
	RDAwdgt *wdgt;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG writescrnbin Writing Screen Binary [%s] into library [%s] at line [%d] program [%s].",(screen->name!=NULL ? screen->name:""),libname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	bin=BINnew();
	BINaddshort(bin,-1688);	
	BINaddstring(bin,screen->name);
	BINaddint(bin,screen->numwdgts);
	for(x=0,wdgt=screen->wdgts+x;x<screen->numwdgts;++x,++wdgt)
	{
		BINaddshort(bin,wdgt->type);
		switch(wdgt->type)
		{
			case 15: /* Custom Input Fields */
			case 16: /* Expenditure Fields */
			case 17: /* Revenue Fields */
			case 18: /* Balance Sheet Fields */
			case 19: /* Beginning Balance Fields */
				if(!isEMPTY(wdgt->resource_name))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->resource_name);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(wdgt->label))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->label);
				} else BINaddshort(bin,FALSE);
				BINaddshort(bin,wdgt->cols);
				BINaddint(bin,wdgt->rtype);
				if(!isEMPTY(wdgt->editable_expression))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->editable_expression);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(wdgt->sensitive_expression))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->sensitive_expression);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(wdgt->transversal_expression))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->transversal_expression);
				} else BINaddshort(bin,FALSE);
				break;
			case 0:	/* Standard Resource type */
			case 22: /* Progress Bar */
				if(!isEMPTY(wdgt->resource_name))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->resource_name);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(wdgt->label))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->label);
				} else BINaddshort(bin,FALSE);
				BINaddshort(bin,wdgt->cols);
				BINaddint(bin,wdgt->rtype);
				if(!isEMPTY(wdgt->editable_expression))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->editable_expression);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(wdgt->sensitive_expression))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->sensitive_expression);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(wdgt->transversal_expression))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->transversal_expression);
				} else BINaddshort(bin,FALSE);
				break;
			case 12: /* New Scrolled Window */
				BINaddshort(bin,wdgt->rows);
				BINaddshort(bin,wdgt->cols);
				break;
			case 1: /* start row */
			case 2: /* end row */
			case 3: /* start box */
			case 4: /* end box */
			case 13: /* End Scrolled Window */
			case 21: /* End Tabbed Container */
			case 25: /* End Popup Menu */
			case 27: /* End Toolbar */
			case 29: /* End Table */
			case 30: /* Start Header */
			case 31: /* End Header */
			case 33: /* End Panel */
				break;
			case 11: /* seperator */
			case 10: /* frame */
			case 23: /* New Tab Bar */
				BINaddint(bin,wdgt->rtype);
				break;
			case 20: /* New Tabbed Container */
			case 24: /* New Popup Menu */
			case 26: /* New ToolBar    */
			case 28: /* New Table      */
			case 32: /* Start Panel */
			case 5: /* labels */
				if(!isEMPTY(wdgt->label))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->label);
				} else BINaddshort(bin,FALSE);
				BINaddint(bin,wdgt->rtype);
				if(wdgt->rtype==1)
				{
					if(!isEMPTY(wdgt->pixmap))
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,wdgt->pixmap);
					} else BINaddshort(bin,FALSE);
				}
				if(!isEMPTY(wdgt->XHTML_Label))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->XHTML_Label);
				} else BINaddshort(bin,FALSE);
				break;
			case 6:	/* buttons  */
			case 14: /* optional screens */
				if(!isEMPTY(wdgt->resource_name))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->resource_name);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(wdgt->label))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->label);
				} else BINaddshort(bin,FALSE);
				BINaddint(bin,wdgt->rtype);
				if(wdgt->rtype==5)
				{
					if(!isEMPTY(wdgt->pixmap))
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,wdgt->pixmap);
					} else BINaddshort(bin,FALSE);
				}
				if(!isEMPTY(wdgt->XHTML_Label))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->XHTML_Label);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(wdgt->editable_expression))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->editable_expression);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(wdgt->sensitive_expression))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->sensitive_expression);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(wdgt->transversal_expression))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->transversal_expression);
				} else BINaddshort(bin,FALSE);
				break;
			case 9: /* toggle button */
				if(!isEMPTY(wdgt->resource_name))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->resource_name);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(wdgt->label))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->label);
				} else BINaddshort(bin,FALSE);
				BINaddint(bin,wdgt->rtype);
				if(wdgt->rtype==1)
				{
					if(!isEMPTY(wdgt->pixmap))
					{
						BINaddshort(bin,TRUE);
						BINaddstring(bin,wdgt->pixmap);
					} else BINaddshort(bin,FALSE);
				}
				if(!isEMPTY(wdgt->XHTML_Label))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->XHTML_Label);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(wdgt->editable_expression))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->editable_expression);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(wdgt->sensitive_expression))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->sensitive_expression);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(wdgt->transversal_expression))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->transversal_expression);
				} else BINaddshort(bin,FALSE);
				break;
			case 7: /* scrolled list */
				if(!isEMPTY(wdgt->resource_name))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->resource_name);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(wdgt->label))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->label);
				} else BINaddshort(bin,FALSE);
				BINaddshort(bin,wdgt->rows);
				if(!isEMPTY(wdgt->editable_expression))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->editable_expression);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(wdgt->sensitive_expression))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->sensitive_expression);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(wdgt->transversal_expression))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->transversal_expression);
				} else BINaddshort(bin,FALSE);
				break;
			case 8:	/* scrolled text */
				if(!isEMPTY(wdgt->resource_name))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->resource_name);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(wdgt->label))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->label);
				} else BINaddshort(bin,FALSE);
				BINaddshort(bin,wdgt->rows);
				BINaddshort(bin,wdgt->cols);
				BINaddint(bin,wdgt->rtype);
				if(!isEMPTY(wdgt->editable_expression))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->editable_expression);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(wdgt->sensitive_expression))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->sensitive_expression);
				} else BINaddshort(bin,FALSE);
				if(!isEMPTY(wdgt->transversal_expression))
				{
					BINaddshort(bin,TRUE);
					BINaddstring(bin,wdgt->transversal_expression);
				} else BINaddshort(bin,FALSE);
				break;
			default:
				prterr("Error Widget Type [%d] invalid on Screen [%s] [%s] at line [%d] program [%s].",wdgt->type,screen->module,screen->name,line,file);
				break;
		}
		if(!isEMPTY(wdgt->expression))
		{
			BINaddshort(bin,TRUE);
			BINaddstring(bin,wdgt->expression);
		} else BINaddshort(bin,FALSE);
	}
	if(writelibbin(libname,bin,screen->name))
	{
		prterr("Error Can't write screen binary [%s] into screen library [%s] at line [%d] program [%s].",(screen->name!=NULL ? screen->name:""),libname,line,file);
		return(-1);
	}
	return(0);
}
short xfindscrnbin(RDAscrn *screen,int line,char *file)
{
	short ef=(-1);
	char *libname=NULL,*dashes=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field) 
	{ 
		prterr("DIAG findscrnbin Looking for Screen [%s] [%s] using findscrnbin at line [%d] program [%s].",screen->module,screen->name,line,file); 
	}
#endif /* USE_RDA_DIAGNOSTICS */
	dashes=adddashes(screen->module);
	libname=Rmalloc(RDAstrlen(USERLOGIN)+RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(dashes)+7);
#ifndef WIN32
	sprintf(libname,"%s/%s/%s.SCN",CURRENTDIRECTORY,USERLOGIN,dashes);
#endif
#ifdef WIN32
	sprintf(libname,"%s\\%s\\%s.SCN",CURRENTDIRECTORY,USERLOGIN,dashes);
#endif
	if(!getscrnbin(libname,screen,FALSE)) ef=FALSE;
	if(ef && (!isEMPTY(RDA_GROUP)))
	{
		if(libname!=NULL) 
		{
			libname=Rrealloc((void *)libname,RDAstrlen(CURRENTDIRECTORY)+
				RDAstrlen(screen->module)+RDAstrlen(RDA_GROUP)+10);
		} else {
			libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+
					RDAstrlen(screen->module)+RDAstrlen(RDA_GROUP)+10);
		}
#ifndef WIN32
		sprintf(libname,"%s/%s/%s.SCN",CURRENTDIRECTORY,RDA_GROUP,dashes);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\%s\\%s.SCN",CURRENTDIRECTORY,RDA_GROUP,dashes);
#endif
		if(!getscrnbin(libname,screen,FALSE)) ef=FALSE;
	}
	if(ef)
	{
		if(libname!=NULL) 
		{
			libname=Rrealloc((void *)libname,RDAstrlen(CURRENTDIRECTORY)+
				RDAstrlen(screen->module)+11);
		} else {
			libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+
				RDAstrlen(screen->module)+11);
		}
#ifndef WIN32
		sprintf(libname,"%s/site/%s.SCN",CURRENTDIRECTORY,dashes);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\site\\%s.SCN",CURRENTDIRECTORY,dashes);
#endif
		if(!getscrnbin(libname,screen,FALSE)) ef=FALSE;
	}
	if(ef)
	{
		if(libname!=NULL) 
		{
			libname=Rrealloc((void *)libname,RDAstrlen(CURRENTDIRECTORY)+
				RDAstrlen(screen->module)+10);
		} else {
			libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+
					RDAstrlen(screen->module)+10);
		}
#ifndef WIN32
		sprintf(libname,"%s/rda/%s.SCN",CURRENTDIRECTORY,dashes);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,dashes);
#endif
		if(!getscrnbin(libname,screen,TRUE)) ef=FALSE;
	}
	if(ef)
	{
		prterr("Error Can't Find Screen [%s] [%s] at line [%d] program [%s].",screen->module,screen->name,line,file);
	}
	if(dashes!=NULL) Rfree(dashes);
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		if(ef)
		{
			prterr("DIAG findscrnbin Looking for Screen [%s] [%s] at line [%d] program [%s] has NOT been found in Screen Library [%s].",screen->module,screen->name,line,file,libname);
		} else {
			prterr("DIAG findscrnbin Looking for Screen [%s] [%s] at line [%d] program [%s] has been found in Screen Library [%s].",screen->module,screen->name,line,file,libname);
		}
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(libname!=NULL) Rfree(libname);
	return(ef);
}
/*---------------------------------------------------------------------------
	getscrnbin 	Function reads a bin file from screen structure.
---------------------------------------------------------------------------*/
short xgetscrnbin(char *libname,RDAscrn *screen,short showerror,int line,char *file)
{
	short x,test=FALSE;
	BIN *bin;
	RDAwdgt *wdgt;
	char *testx=NULL,donewscreen=FALSE,*temp=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG getscrnbin Attempting to Read Screen [%s] [%s] from library [%s] at line [%d] program [%s].",screen->module,screen->name,libname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	bin=getlibbin(libname,screen->name,showerror);
	if(bin==NULL) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diaggui)
		{
			prterr("Error getscrnbin's Attempt to Read Screen [%s] [%s] from Library [%s] at line [%d] program [%s] has failed.",screen->module,screen->name,libname,line,file);
		}
#endif /* USE_RDA_DIAGNOSTICS */
		return(-1);
	}
	x=BINgetshort(bin);
	if(x!=(-1688) && x!=(-1689) && x!=(-1690) && x!=(-1691) && x!=(-1692) && x!=(-1693) && x!=(-1694) && x!=(-1695) && x!=(-1696) && x!=(-1697) && x!=(-1698) && x!=(-1701) && x!=(-1702) && x!=(-1703) && x!=(-1700) && x!=(-1699))
	{
		prterr("Error Invalid version or bad binary for Screen [%s] [%s] in Library [%s] at line [%d] program [%s].",screen->module,screen->name,libname,line,file);
		BINfree(bin);
		return(-1);
	}
	if(x==(-1688)) donewscreen=15;
	else if(x==(-1689)) donewscreen=14;
	else if(x==(-1690)) donewscreen=13;
	else if(x==(-1691)) donewscreen=12;
	else if(x==(-1692)) donewscreen=11;
	else if(x==(-1693)) donewscreen=10;
	else if(x==(-1694)) donewscreen=9;
	else if(x==(-1695)) donewscreen=8;
	else if(x==(-1696)) donewscreen=7;
	else if(x==(-1697)) donewscreen=6;
	else if(x==(-1698)) donewscreen=5;
	else if(x==(-1699)) donewscreen=4;
	else if(x==(-1702)) donewscreen=TRUE;
	else if(x==(-1703)) donewscreen=2;
	else if(x==(-1700)) donewscreen=3;
	else donewscreen=FALSE;
	testx=BINgetstring(bin);
	if(testx!=NULL) Rfree(testx);
	screen->numwdgts=BINgetint(bin);
	screen->wdgts=Rmalloc(screen->numwdgts*sizeof(struct RDAwdgts));
	for(x=0,wdgt=screen->wdgts;x<screen->numwdgts;++x,++wdgt)
	{
		wdgt->type=BINgetshort(bin);
		wdgt->resource_num=0;
		wdgt->transversal_expression=NULL;
		wdgt->sensitive_expression=NULL;
		wdgt->editable_expression=NULL;
		wdgt->expression=NULL;
		wdgt->resource_name=NULL;
		wdgt->label=NULL;
		wdgt->XHTML_Label=NULL;
		wdgt->pixmap=NULL;
		switch(wdgt->type)
		{
			case 15: /* Custom Input Fields */
			case 16: /* Expenditure Fields */
			case 17: /* Revenue Fields */
			case 18: /* Balance Sheet Fields */
			case 19: /* Beginning Balance Fields */
				if(donewscreen>6)
				{
					test=BINgetshort(bin);
					if(test) wdgt->resource_name=BINgetstring(bin);
						else wdgt->resource_name=NULL;
					test=BINgetshort(bin);
					if(test) wdgt->label=BINgetstring(bin);
						else wdgt->label=NULL;
					wdgt->rows=0;
					wdgt->cols=BINgetshort(bin);
					wdgt->rtype=BINgetint(bin);
					wdgt->pixmap=NULL;
					if(donewscreen>11)
					{
						test=BINgetshort(bin);
						if(test) wdgt->editable_expression=BINgetstring(bin);
							else wdgt->editable_expression=NULL;
						test=BINgetshort(bin);
						if(test) wdgt->sensitive_expression=BINgetstring(bin);
							else wdgt->sensitive_expression=NULL;
					}
					if(donewscreen>12)
					{
						test=BINgetshort(bin);
						if(test) wdgt->transversal_expression=BINgetstring(bin);
							else wdgt->transversal_expression=NULL;
					} else wdgt->transversal_expression=NULL;
				} else {
					prterr("Error Widget type [%d] invalid for Screen Widget [%d] on Screen [%s] [%s] at line [%d] program [%s].",wdgt->type,x,screen->module,screen->name,line,file);
					wdgt->resource_name=NULL;
					wdgt->label=NULL;
					wdgt->pixmap=NULL;
					wdgt->rows=0;
					wdgt->cols=0;
					wdgt->rtype=0;
				}
				break;
			case 22: /* Progress Bar */
			case 0:	/* for fields */
				if(donewscreen>2)
				{
					test=BINgetshort(bin);
					if(test) wdgt->resource_name=BINgetstring(bin);
						else wdgt->resource_name=NULL;
				} else wdgt->resource_name=BINgetstring(bin);;
				if(donewscreen>4)
				{
					test=BINgetshort(bin);
					if(test) wdgt->label=BINgetstring(bin);
						else wdgt->label=NULL;
				} else wdgt->label=NULL;
				wdgt->rows=0;
				if(donewscreen) wdgt->cols=BINgetshort(bin);
					else wdgt->cols=0;
				if(donewscreen>13)
				{
					wdgt->rtype=BINgetint(bin);
				} else wdgt->rtype=0;
				wdgt->pixmap=NULL;
				if(donewscreen>11)
				{
					test=BINgetshort(bin);
					if(test) wdgt->editable_expression=BINgetstring(bin);
						else wdgt->editable_expression=NULL;
					test=BINgetshort(bin);
					if(test) wdgt->sensitive_expression=BINgetstring(bin);
						else wdgt->sensitive_expression=NULL;
				}
				if(donewscreen>12)
				{
					test=BINgetshort(bin);
					if(test) wdgt->transversal_expression=BINgetstring(bin);
						else wdgt->transversal_expression=NULL;
				} else wdgt->transversal_expression=NULL;
				break;
			case 1: /* new line */
			case 2: /* end line */
			case 3: /* new box */
			case 4: /* end box */
			case 21: /* End Tabbed Container */
			case 25: /* End Popup Menu */
			case 27: /* End Toolbar */
			case 29: /* End Table */
			case 30: /* Start Header */
			case 31: /* End Header */
			case 33: /* End Panel */
				wdgt->resource_name=NULL;
				wdgt->label=NULL;
				wdgt->pixmap=NULL;
				wdgt->rows=0;
				wdgt->cols=0;
				wdgt->rtype=0;
				break;
			case 20: /* New Tabbed Container */
			case 24: /* New Popup Menu */
			case 26: /* New Toolbar */
			case 28: /* New Table */
			case 32: /* Start Panel */
			case 5: /* labels */
				wdgt->resource_name=NULL;
				if(donewscreen>2)
				{
					test=BINgetshort(bin);
					if(test) wdgt->label=BINgetstring(bin);
						else wdgt->label=NULL;
				} else wdgt->label=BINgetstring(bin);
				if(donewscreen>8)
				{
					if(donewscreen>10)
					{
						wdgt->rtype=BINgetint(bin);
						if(wdgt->rtype==1)
						{
							test=BINgetshort(bin);
							if(test) wdgt->pixmap=BINgetstring(bin);
								else wdgt->pixmap=NULL;
						} else {
							wdgt->pixmap=NULL;
						}
					} else {
						test=BINgetshort(bin);
						if(test) wdgt->pixmap=BINgetstring(bin);
							else wdgt->pixmap=NULL;
						wdgt->rtype=BINgetint(bin);
					}
				} else {
					wdgt->pixmap=NULL;
					wdgt->rtype=0;
				}
				if(donewscreen>14)
				{
					test=BINgetshort(bin);
					if(test) wdgt->XHTML_Label=BINgetstring(bin);
						else wdgt->XHTML_Label=NULL;
				} else if(donewscreen>9)
				{
					test=BINgetshort(bin);
					if(test) temp=BINgetstring(bin);
					if(temp!=NULL) Rfree(temp);
				}
				wdgt->rows=0;
				wdgt->cols=0;
				break;
			case 6: /* buttons */
				if(donewscreen>2)
				{
					test=BINgetshort(bin);
					if(test) wdgt->resource_name=BINgetstring(bin);
						else wdgt->resource_name=NULL;
				} else wdgt->resource_name=BINgetstring(bin);
				if(donewscreen>2)
				{
					test=BINgetshort(bin);
					if(test) wdgt->label=BINgetstring(bin);
						else wdgt->label=NULL;
				} else wdgt->label=BINgetstring(bin);
				if(donewscreen>4)
				{
					wdgt->rtype=BINgetint(bin);
				} else {
					wdgt->rtype=0;
				}
				if(donewscreen>8)
				{
					if(donewscreen>10)
					{
						if(wdgt->rtype==5)
						{
							test=BINgetshort(bin);
							if(test) wdgt->pixmap=BINgetstring(bin);
								else wdgt->pixmap=NULL;
						} else {
							wdgt->pixmap=NULL;
						}
					} else {
						test=BINgetshort(bin);
						if(test) wdgt->pixmap=BINgetstring(bin);
							else wdgt->pixmap=NULL;
					}
				} else {
					wdgt->pixmap=NULL;
				}
				if(donewscreen>14)
				{
					test=BINgetshort(bin);
					if(test) wdgt->XHTML_Label=BINgetstring(bin);
						else wdgt->XHTML_Label=NULL;
				} else if(donewscreen>9)
				{
					test=BINgetshort(bin);
					if(test) temp=BINgetstring(bin);
					if(temp!=NULL) Rfree(temp);
				}
				if(donewscreen>11)
				{
					test=BINgetshort(bin);
					if(test) wdgt->editable_expression=BINgetstring(bin);
						else wdgt->editable_expression=NULL;
					test=BINgetshort(bin);
					if(test) wdgt->sensitive_expression=BINgetstring(bin);
						else wdgt->sensitive_expression=NULL;
				}
				if(donewscreen>12)
				{
					test=BINgetshort(bin);
					if(test) wdgt->transversal_expression=BINgetstring(bin);
						else wdgt->transversal_expression=NULL;
				} else wdgt->transversal_expression=NULL;
				wdgt->rows=0;
				wdgt->cols=0;
				break;
			case 14:/* optional screens */
				if(donewscreen>5)
				{
					test=BINgetshort(bin);
					if(test) wdgt->resource_name=BINgetstring(bin);
						else wdgt->resource_name=NULL;
					test=BINgetshort(bin);
					if(test) wdgt->label=BINgetstring(bin);
						else wdgt->label=NULL;
					if(donewscreen>8)
					{
						if(donewscreen>10)
						{
							wdgt->rtype=BINgetint(bin);
							if(wdgt->rtype==5)
							{
								test=BINgetshort(bin);
								if(test) wdgt->pixmap=BINgetstring(bin);
									else wdgt->pixmap=NULL;
							} else {
								wdgt->pixmap=NULL;
							}
						} else {
							test=BINgetshort(bin);
							if(test) wdgt->pixmap=BINgetstring(bin);
								else wdgt->pixmap=NULL;
						}
					} else {
						wdgt->pixmap=NULL;
					}
					if(donewscreen>14)
					{
						test=BINgetshort(bin);
						if(test) wdgt->XHTML_Label=BINgetstring(bin);
							else wdgt->XHTML_Label=NULL;
					} else if(donewscreen>9)
					{
						test=BINgetshort(bin);
						if(test) temp=BINgetstring(bin);
						if(temp!=NULL) Rfree(temp);
					}
					if(donewscreen<11)
					{
						wdgt->rtype=BINgetint(bin);
					}
					wdgt->rows=0;
					wdgt->cols=0;
					if(donewscreen>11)
					{
						test=BINgetshort(bin);
						if(test) wdgt->editable_expression=BINgetstring(bin);
							else wdgt->editable_expression=NULL;
						test=BINgetshort(bin);
						if(test) wdgt->sensitive_expression=BINgetstring(bin);
							else wdgt->sensitive_expression=NULL;
					}
					if(donewscreen>12)
					{
						test=BINgetshort(bin);
						if(test) wdgt->transversal_expression=BINgetstring(bin);
							else wdgt->transversal_expression=NULL;
					} else wdgt->transversal_expression=NULL;
				} else {
					if(donewscreen>2)
					{
						test=BINgetshort(bin);
						if(test) wdgt->resource_name=BINgetstring(bin);
							else wdgt->resource_name=NULL;
					} else wdgt->resource_name=BINgetstring(bin);
					if(donewscreen>2)
					{
						test=BINgetshort(bin);
						if(test) wdgt->label=BINgetstring(bin);
							else wdgt->label=NULL;
					} else wdgt->label=BINgetstring(bin);
					wdgt->rows=0;
					wdgt->cols=0;
					wdgt->rtype=0;
					wdgt->pixmap=NULL;
				}
				break;
			case 9: /* toggle buttons */
				if(donewscreen>2)
				{
					test=BINgetshort(bin);
					if(test) wdgt->resource_name=BINgetstring(bin);
						else wdgt->resource_name=NULL;
				} else wdgt->resource_name=BINgetstring(bin);
				if(donewscreen>2)
				{
					test=BINgetshort(bin);
					if(test) wdgt->label=BINgetstring(bin);
						else wdgt->label=NULL;
				} else wdgt->label=BINgetstring(bin);
				if(donewscreen>8)
				{
					if(donewscreen>10)
					{
						wdgt->rtype=BINgetint(bin);
						if(wdgt->rtype==1)
						{
							test=BINgetshort(bin);
							if(test) wdgt->pixmap=BINgetstring(bin);
								else wdgt->pixmap=NULL;
						} else {
							wdgt->pixmap=NULL;
						}
					} else {
						test=BINgetshort(bin);
						if(test) wdgt->pixmap=BINgetstring(bin);
							else wdgt->pixmap=NULL;
						wdgt->rtype=BINgetint(bin);
					}
				} else {
					wdgt->pixmap=NULL;
					wdgt->rtype=0;
				}
				if(donewscreen>9)
				{
					test=BINgetshort(bin);
					if(test) temp=BINgetstring(bin);
					if(temp!=NULL) Rfree(temp);
				}
				wdgt->rows=0;
				wdgt->cols=0;
				if(donewscreen>11)
				{
					test=BINgetshort(bin);
					if(test) wdgt->editable_expression=BINgetstring(bin);
						else wdgt->editable_expression=NULL;
					test=BINgetshort(bin);
					if(test) wdgt->sensitive_expression=BINgetstring(bin);
						else wdgt->sensitive_expression=NULL;
				}
				if(donewscreen>12)
				{
					test=BINgetshort(bin);
					if(test) wdgt->transversal_expression=BINgetstring(bin);
						else wdgt->transversal_expression=NULL;
				} else wdgt->transversal_expression=NULL;
				break;
			case 7: /* scrolled list */
				if(donewscreen>2)
				{
					test=BINgetshort(bin);
					if(test) wdgt->resource_name=BINgetstring(bin);
						else wdgt->resource_name=NULL;
				} else wdgt->resource_name=BINgetstring(bin);
				if(donewscreen>4)
				{
					test=BINgetshort(bin);
					if(test) wdgt->label=BINgetstring(bin);
						else wdgt->label=NULL;
				} else wdgt->label=NULL;
				wdgt->rows=BINgetshort(bin);
				wdgt->cols=0;
				wdgt->rtype=0;
				wdgt->pixmap=NULL;
				if(donewscreen>11)
				{
					test=BINgetshort(bin);
					if(test) wdgt->editable_expression=BINgetstring(bin);
						else wdgt->editable_expression=NULL;
					test=BINgetshort(bin);
					if(test) wdgt->sensitive_expression=BINgetstring(bin);
						else wdgt->sensitive_expression=NULL;
				}
				if(donewscreen>12)
				{
					test=BINgetshort(bin);
					if(test) wdgt->transversal_expression=BINgetstring(bin);
						else wdgt->transversal_expression=NULL;
				} else wdgt->transversal_expression=NULL;
				break;
			case 8: /* scrolled text */
				if(donewscreen>2)
				{
					test=BINgetshort(bin);
					if(test) wdgt->resource_name=BINgetstring(bin);
						else wdgt->resource_name=NULL;
				} else wdgt->resource_name=BINgetstring(bin);
				if(donewscreen>4)
				{
					test=BINgetshort(bin);
					if(test) wdgt->label=BINgetstring(bin);
						else wdgt->label=NULL;
				} else wdgt->label=NULL;
				wdgt->rows=BINgetshort(bin);
				wdgt->cols=BINgetshort(bin);
				if(donewscreen>1)
				{
					wdgt->rtype=BINgetint(bin);
				} else wdgt->rtype=0;
				wdgt->pixmap=NULL;
				if(donewscreen>11)
				{
					test=BINgetshort(bin);
					if(test) wdgt->editable_expression=BINgetstring(bin);
						else wdgt->editable_expression=NULL;
					test=BINgetshort(bin);
					if(test) wdgt->sensitive_expression=BINgetstring(bin);
						else wdgt->sensitive_expression=NULL;
				}
				if(donewscreen>12)
				{
					test=BINgetshort(bin);
					if(test) wdgt->transversal_expression=BINgetstring(bin);
						else wdgt->transversal_expression=NULL;
				} else wdgt->transversal_expression=NULL;
				break;
			case 10: /* frame */
			case 11: /* seperator */
			case 23: /* New Tab Bar */
				if(donewscreen>1)
				{
					wdgt->rtype=BINgetint(bin);
				} else {
					prterr("Error Widget type [%d] invalid for Screen Widget [%d] on Screen [%s] [%s] at line [%d] program [%s].",wdgt->type,x,screen->module,screen->name,line,file);
					wdgt->rtype=0;
				}
				wdgt->resource_name=NULL;
				wdgt->label=NULL;
				wdgt->pixmap=NULL;
				wdgt->rows=0;
				wdgt->cols=0;
				break;
			case 12: /* New Scrolled Window */
			case 13: /* End Scrolled Window */
				if(donewscreen<2)
				{
					prterr("Error Widget type [%d] invalid for Screen Widget [%d] on Screen [%s] [%s] at line [%d] program [%s].",wdgt->type,x,screen->module,screen->name,line,file);
				}
				wdgt->resource_name=NULL;
				wdgt->label=NULL;
				wdgt->pixmap=NULL;
				if(donewscreen>7 && wdgt->type==12)
				{
					wdgt->rows=BINgetshort(bin);
					wdgt->cols=BINgetshort(bin);
				} else {
					wdgt->rows=0;
					wdgt->cols=0;
				}
				wdgt->rtype=0;
				break;
			default:
				prterr("Error Widget type [%d] invalid for Screen Widget [%d] on Screen [%s] [%s] at line [%d] program [%s].",wdgt->type,x,screen->module,screen->name,line,file);
				wdgt->resource_name=NULL;
				wdgt->label=NULL;
				wdgt->pixmap=NULL;
				wdgt->rows=0;
				wdgt->cols=0;
				wdgt->rtype=0;
				break;
		}
		if(donewscreen>3)
		{
			test=BINgetshort(bin);
			if(test) wdgt->expression=BINgetstring(bin);
				else wdgt->expression=NULL;
		}
	}
	BINfree(bin);
	return(0);
}
/*---------------------------------------------------------------------------
	FINDWDGT	Function to find a widget based on the widget name 
---------------------------------------------------------------------------*/
short xFINDWDGT(RDAscrn *scn,char *name,int line,char *file)
{
	short x;
	RDAwdgt *wdgt;

	if(scn==NULL)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diaggui || diaggui_field)
		{
			prterr("Error RDAscrn pointer is NULL.");
		}
#endif /* USE_RDA_DIAGNOSTICS */
		return(-1);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDWDGT Searching for Widget [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(name!=NULL ? name:""),(scn->module!=NULL ? scn->module:""),(scn->name!=NULL ? scn->name:""),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,wdgt=scn->wdgts;x<scn->numwdgts;++x,++wdgt)
	{
		if(!RDAstrcmp(wdgt->resource_name,name)) return(x);
	} 
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("Error Widget [%s] is not found on Screen [%s] [%s] at line [%d] program [%s].",name,scn->module,scn->name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(-1);
}
/*---------------------------------------------------------------------------
	FINDRSC	Function to find a resource based on the resource name 
---------------------------------------------------------------------------*/
short xFINDRSC(RDArsrc *rsc,char *name,int line,char *file)
{
	short x;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSC Searching for resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(rsc==NULL) return(-1);
	if(rsc->rscs!=NULL)
	{
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field) 
			{ 
				prterr("DIAG Found Resource [%s] on Screen [%s] [%s] at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file); 
			}
#endif /* USE_RDA_DIAGNOSTICS */
			return(x);
		}
	}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field) 
	{ 
		prterr("Error Resource [%s] not found on Screen [%s] [%s] at line [%d] program [%s].",(name==NULL?"":name),(rsc->module==NULL?"":rsc->module),(rsc->screen==NULL?"":rsc->screen),line,file); 
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(-1); 
}
int xDeActivateLine(RDAscrn *scn,int curnum,int line,char *file)
{
	int x,count=1,boxcount=0;
	RDAwdgt *wdgt;
	char box=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG DeActivateLine DeActivate a line of widgets on Screen [%s] [%s] starting at [%d] at line [%d] program [%s].",scn->module,scn->name,curnum,line,file);  
	}
#endif /* USE_RDA_DIAGNOSTICS */
	wdgt=scn->wdgts+curnum;
	wdgt->resource_num=(-1);
	for(x=(curnum+1);x<scn->numwdgts;++x)
	{
		wdgt=scn->wdgts+x;
		++count;
		if(wdgt->resource_num!=(-1))
		{
		switch(wdgt->type)
		{
			case 10: /* Frame */
				wdgt->resource_num=(-1);
				break;
			case 15: /* custom input fields */
			case 16: /* expenditure fields */
			case 17: /* revenue fields */
			case 18: /* balance sheet fields */
			case 19: /* beginning balance fields */
			case 22: /* Progress Bar */
			case 0: /* fields */
			case 6: /* special controls */
			case 7: /* scrolled list */
			case 8: /* scrolled text */
			case 9:
			case 14: /* optional screens */
			case 11: /* Seperator */
				wdgt->resource_num=(-1);
				break;
			case 23: /* New Tabbed Container */
			case 5: /* label */
				wdgt->resource_num=(-1);
				break;
			case 1: /* new line */
			case 2: /* end line */
				wdgt->resource_num=(-1);
				if(!box)
				{
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui)
					{
						prterr("DIAG DeActivateLine on Screen [%s] [%s] returning [%d] deactivated at line [%d] program [%s].",scn->module,scn->name,count,line,file);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					return count;
				}
				break;
			case 3: /* new box */
			case 12: /* scrolled window */
			case 20: /* New Tabbed Container */
			case 24: /* New Tabbed Container */
			case 26: /* New Tabbed Container */
			case 28: /* New Tabbed Container */
			case 30: /* New Tabbed Container */
			case 32: /* New Panel */
				wdgt->resource_num=(-1);
				if(!box) box=TRUE;
				++boxcount;
				break;
			case 13: /* end scrolled window */
			case 4: /* end box */
			case 21: /* End Tabbed Container */
			case 25: /* End Tabbed Container */
			case 27: /* End Tabbed Container */
			case 29: /* End Tabbed Container */
			case 31: /* End Tabbed Container */
			case 33: /* End Panel */
				--boxcount;
				if(boxcount==0) box=FALSE;
				wdgt->resource_num=(-1);
				break;
			default:
				prterr("Error Widget Type [%d] incorrect for Widget [%d] on Screen [%s] [%s] at line [%d] program [%s].",wdgt->type,x,scn->module,scn->name,line,file);
				break;
		}
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG DeActivateLine on Screen [%s] [%s] returning [%d] deactivated at line [%d] program [%s].",scn->module,scn->name,count,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return count;
}
int xDeActivateTabContainer(RDAscrn *scn,int curnum,int line,char *file)
{
	int x,count=1,boxcount=0;
	RDAwdgt *wdgt;
	char box=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG DeActivateTabContainer DeActivate a Tab container of widgets on Screen [%s] [%s] starting at [%d] at line [%d] program [%s].",scn->module,scn->name,curnum,line,file);  
	}
#endif /* USE_RDA_DIAGNOSTICS */
	wdgt=scn->wdgts+curnum;
	wdgt->resource_num=(-1);
	for(x=(curnum+1);x<scn->numwdgts;++x)
	{
		wdgt=scn->wdgts+x;
		++count;
		if(wdgt->resource_num!=(-1))
		{
		switch(wdgt->type)
		{
			case 10: /* Frame */
				wdgt->resource_num=(-1);
				break;
			case 15: /* custom input fields */
			case 16: /* expenditure fields */
			case 17: /* revenue fields */
			case 18: /* balance sheet fields */
			case 19: /* beginning balance fields */
			case 22: /* Progress Bar */
			case 0: /* fields */
			case 6: /* special controls */
			case 7: /* scrolled list */
			case 8: /* scrolled text */
			case 9:
			case 14: /* optional screens */
			case 11: /* Seperator */
			case 23: /* New Tabbed Bar */
				wdgt->resource_num=(-1);
				break;
			case 5: /* label */
				wdgt->resource_num=(-1);
				break;
			case 1: /* new line */
			case 2: /* end line */
				wdgt->resource_num=(-1);
				break;
				wdgt->resource_num=(-1);
				break;
			case 21: /* End Tabbed Container */
				wdgt->resource_num=(-1);
				--boxcount;
				if(boxcount==0) box=FALSE;
				if(!box)
				{
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui)
					{
						prterr("DIAG DeActivateLine on Screen [%s] [%s] returning [%d] deactivated at line [%d] program [%s].",scn->module,scn->name,count,line,file);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					return count;
				}
				break;
			case 20: /* New Tabbed Container */
			case 12: /* scrolled window */
			case 3: /* new box */
			case 24: /* New Popup Container */
			case 26: /* New Toolbar Container */
			case 28: /* New Toolbar Container */
			case 30: /* New Toolbar Container */
			case 32: /* New Panel */
				wdgt->resource_num=(-1);
				if(!box) box=TRUE;
				++boxcount;
				break;
			case 13: /* end scrolled window */
			case 4: /* end box */
			case 25: /* End Popup */
			case 27: /* End Toolbar Container */
			case 29: /* End Toolbar Container */
			case 31: /* End Toolbar Container */
			case 33: /* End Panel */
				--boxcount;
				if(boxcount==0) box=FALSE;
				wdgt->resource_num=(-1);
				break;
			default:
				prterr("Error Widget Type [%d] incorrect for Widget [%d] on Screen [%s] [%s] at line [%d] program [%s].",wdgt->type,x,scn->module,scn->name,line,file);
				break;
		}
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG DeActivateTabContainer on Screen [%s] [%s] returning [%d] deactivated at line [%d] program [%s].",scn->module,scn->name,count,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return count;
}
int xDeActivateScrolledWindow(RDAscrn *scn,int curnum,int line,char *file)
{
	int x,count=1,boxcount=0;
	RDAwdgt *wdgt;
	char box=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG DeActivateScrolledWindow DeActivate a ScrolledWindow of widgets on Screen [%s] [%s] starting at [%d] at line [%d] program [%s].",scn->module,scn->name,curnum,line,file);  
	}
#endif /* USE_RDA_DIAGNOSTICS */
	wdgt=scn->wdgts+curnum;
	wdgt->resource_num=(-1);
	for(x=(curnum+1);x<scn->numwdgts;++x)
	{
		wdgt=scn->wdgts+x;
		++count;
		if(wdgt->resource_num!=(-1))
		{
		switch(wdgt->type)
		{
			case 10: /* Frame */
				wdgt->resource_num=(-1);
				break;
			case 15: /* custom input fields */
			case 16: /* expenditure fields */
			case 17: /* revenue fields */
			case 18: /* balance sheet fields */
			case 19: /* beginning balance fields */
			case 22: /* Progress Bar */
			case 0: /* fields */
			case 6: /* special controls */
			case 7: /* scrolled list */
			case 8: /* scrolled text */
			case 9:
			case 14: /* optional screens */
			case 11: /* Seperator */
				wdgt->resource_num=(-1);
				break;
			case 23: /* New Tabbed Container */
			case 5: /* label */
				wdgt->resource_num=(-1);
				break;
			case 1: /* new line */
			case 2: /* end line */
				wdgt->resource_num=(-1);
				break;
			case 13: /* end scrolled window */
				wdgt->resource_num=(-1);
				--boxcount;
				if(boxcount==0) box=FALSE;
				if(!box)
				{
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui)
					{
						prterr("DIAG DeActivateLine on Screen [%s] [%s] returning [%d] deactivated at line [%d] program [%s].",scn->module,scn->name,count,line,file);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					return count;
				}
				break;
			case 3: /* new box */
			case 12: /* scrolled window */
			case 20: /* New Tabbed Container */
			case 24: /* New Tabbed Container */
			case 26: /* New Tabbed Container */
			case 28: /* New Tabbed Container */
			case 30: /* New Tabbed Container */
			case 32: /* New Panel */
				wdgt->resource_num=(-1);
				if(!box) box=TRUE;
				++boxcount;
				break;
			case 4: /* end box */
			case 21: /* End Tabbed Container */
			case 25: /* End Tabbed Container */
			case 27: /* End Tabbed Container */
			case 29: /* End Tabbed Container */
			case 31: /* End Tabbed Container */
			case 33: /* End Panel */
				--boxcount;
				if(boxcount==0) box=FALSE;
				wdgt->resource_num=(-1);
				break;
			default:
				prterr("Error Widget Type [%d] incorrect for Widget [%d] on Screen [%s] [%s] at line [%d] program [%s].",wdgt->type,x,scn->module,scn->name,line,file);
				break;
		}
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG DeActivateScrolledWindow on Screen [%s] [%s] returning [%d] deactivated at line [%d] program [%s].",scn->module,scn->name,count,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return count;
}
int xDeActivateBox(RDAscrn *scn,int curnum,int line,char *file)
{
	int x,count=1,boxcount=1;
	RDAwdgt *wdgt;
	char box=FALSE;
	
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui) 
	{ 
		prterr("DIAG DeActivateBox DeActivating Widgets on Screen [%s] [%s] in an entire box starting at [%d] at line [%d] program [%s].",scn->module,scn->name,curnum,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	wdgt=scn->wdgts+curnum;
	wdgt->resource_num=(-1);
	for(x=(curnum+1);x<scn->numwdgts;++x)
	{
		wdgt=scn->wdgts+x;
		++count;
		if(wdgt->resource_num!=(-1))
		{
		switch(wdgt->type)
		{
			case 10: /* frame */
				wdgt->resource_num=(-1);
				break;
			case 15: /* custom input fields */
			case 16: /* expenditure fields */
			case 17: /* revenue fields */
			case 18: /* balance sheet fields */
			case 19: /* beginning balance fields */
			case 0: /* fields */
			case 6: /* special controls */
			case 7: /* scrolled list */
			case 8: /* scrolled text */
			case 9: /* toggle button */
			case 11: /* seperator */
			case 14: /* optional screens */
			case 5: /* label */
			case 22: /* Progress Bar */
			case 23: /* New Tab Bar */
				wdgt->resource_num=(-1);
				break;
			case 1: /* new line */
				wdgt->resource_num=(-1);
				break;
			case 2: /* end line */
				wdgt->resource_num=(-1);
				break;
			case 3: /* new box */
			case 12: /* new scrolled window */
			case 20: /* New Tab Container */
			case 24: /* New Tab Bar */
			case 26: /* New Tab Bar */
			case 28: /* New Tab Bar */
			case 30: /* New Tab Bar */
			case 32: /* New Panel */
				++boxcount;
				box=TRUE;
				wdgt->resource_num=(-1);
				break;
			case 13: /* end scrolled window */
			case 21: /* End Tabbed Container */
			case 25: /* End Tabbed Container */
			case 27: /* End Tabbed Container */
			case 29: /* End Tabbed Container */
			case 31: /* End Tabbed Container */
			case 33: /* End Panel */
				--boxcount;
				if(boxcount==0) box=FALSE;
				wdgt->resource_num=(-1);
				break;
			case 4: /* end box */
				wdgt->resource_num=(-1);
				--boxcount;
				if(boxcount==0) box=FALSE;
				if(!box)
				{
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui)
					{
						prterr("DIAG DeActivateBox on Screen [%s] [%s] starting with Widget [%d] Returning box count [%d] at line [%d] program [%s].",scn->module,scn->name,count,line,file);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					return(count);
				}
				break;
			default:
				prterr("Error Widget Type [%d] incorrect for Widget [%d] on Screen [%s] [%s] at line [%d] program [%s].",wdgt->type,x,scn->module,scn->name,line,file);
				break;
		}		
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG DeActivateBox on Screen [%s] [%s] starting with Widget [%d] Returning box count [%d] at line [%d] program [%s].",scn->module,scn->name,count,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(count);
}
int xDeActivatePopup(RDAscrn *scn,int curnum,int line,char *file)
{
	int x,count=1,boxcount=1;
	RDAwdgt *wdgt;
	char box=FALSE;
	
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui) 
	{ 
		prterr("DIAG DeActivatePopup DeActivating Widgets on Screen [%s] [%s] in an entire Popup Menu starting at [%d] at line [%d] program [%s].",scn->module,scn->name,curnum,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	wdgt=scn->wdgts+curnum;
	wdgt->resource_num=(-1);
	for(x=(curnum+1);x<scn->numwdgts;++x)
	{
		wdgt=scn->wdgts+x;
		++count;
		if(wdgt->resource_num!=(-1))
		{
		switch(wdgt->type)
		{
			case 10: /* frame */
				wdgt->resource_num=(-1);
				break;
			case 15: /* custom input fields */
			case 16: /* expenditure fields */
			case 17: /* revenue fields */
			case 18: /* balance sheet fields */
			case 19: /* beginning balance fields */
			case 0: /* fields */
			case 6: /* special controls */
			case 7: /* scrolled list */
			case 8: /* scrolled text */
			case 9: /* toggle button */
			case 11: /* seperator */
			case 14: /* optional screens */
			case 5: /* label */
			case 22: /* Progress Bar */
			case 23: /* New Tab Bar */
				wdgt->resource_num=(-1);
				break;
			case 1: /* new line */
				wdgt->resource_num=(-1);
				break;
			case 2: /* end line */
				wdgt->resource_num=(-1);
				break;
				wdgt->resource_num=(-1);
				break;
			case 3: /* new box */
			case 12: /* new scrolled window */
			case 20: /* New Tab Container */
			case 24: /* New Tab Bar */
			case 26: /* New Tab Bar */
			case 28: /* New Tab Bar */
			case 30: /* New Tab Bar */
			case 32: /* New Panel */
				++boxcount;
				box=TRUE;
				wdgt->resource_num=(-1);
				break;
			case 25: /* End Popup  */
				wdgt->resource_num=(-1);
				--boxcount;
				if(boxcount==0) box=FALSE;
				if(!box)
				{
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui)
					{
						prterr("DIAG DeActivateBox on Screen [%s] [%s] starting with Widget [%d] Returning box count [%d] at line [%d] program [%s].",scn->module,scn->name,count,line,file);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					return(count);
				}
				break;
			case 4: /* end box */
			case 13: /* end scrolled window */
			case 21: /* End Tab Container */
			case 27: /* End ToolBar */
			case 29: /* End ToolBar */
			case 31: /* End ToolBar */
			case 33: /* End Panel */
				--boxcount;
				if(boxcount==0) box=FALSE;
				wdgt->resource_num=(-1);
				break;
			default:
				prterr("Error Widget Type [%d] incorrect for Widget [%d] on Screen [%s] [%s] at line [%d] program [%s].",wdgt->type,x,scn->module,scn->name,line,file);
				break;
		}		
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG DeActivatePopup on Screen [%s] [%s] starting with Widget [%d] Returning Popup count [%d] at line [%d] program [%s].",scn->module,scn->name,count,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(count);
}
int xDeActivateToolbar(RDAscrn *scn,int curnum,int line,char *file)
{
	int x,count=1,boxcount=1;
	RDAwdgt *wdgt;
	
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui) 
	{ 
		prterr("DIAG DeActivateToolBar DeActivating Widgets on Screen [%s] [%s] in an entire Toolbar Container starting at [%d] at line [%d] program [%s].",scn->module,scn->name,curnum,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	wdgt=scn->wdgts+curnum;
	wdgt->resource_num=(-1);
	for(x=(curnum+1);x<scn->numwdgts;++x)
	{
		wdgt=scn->wdgts+x;
		++count;
		if(wdgt->resource_num!=(-1))
		{
		switch(wdgt->type)
		{
			case 10: /* frame */
				wdgt->resource_num=(-1);
				break;
			case 15: /* custom input fields */
			case 16: /* expenditure fields */
			case 17: /* revenue fields */
			case 18: /* balance sheet fields */
			case 19: /* beginning balance fields */
			case 0: /* fields */
			case 6: /* special controls */
			case 7: /* scrolled list */
			case 8: /* scrolled text */
			case 9: /* toggle button */
			case 11: /* seperator */
			case 14: /* optional screens */
			case 5: /* label */
			case 20: /* New Tab Container */
			case 21: /* End Tab Container */
			case 22: /* Progress Bar */
			case 23: /* New Tab Bar */
			case 24: /* New Tab Bar */
			case 25: /* New Tab Bar */
			case 28: /* New Tab Bar */
			case 30: /* New Tab Bar */
			case 32: /* New Panel */
				wdgt->resource_num=(-1);
				break;
			case 1: /* new line */
				wdgt->resource_num=(-1);
				break;
			case 2: /* end line */
				wdgt->resource_num=(-1);
				break;
			case 3: /* new box */
			case 12: /* new scrolled window */
			case 4: /* end box */
			case 13: /* end scrolled window */
			case 29: /* end scrolled window */
			case 31: /* end scrolled window */
			case 33: /* end Panel */
				wdgt->resource_num=(-1);
				break;
			case 26: /* New Tab Bar */
				++boxcount;
				wdgt->resource_num=(-1);
				break;
			case 27: /* New Tab Bar */
				--boxcount;
				wdgt->resource_num=(-1);
				if(boxcount==0)
				{
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui)
					{
						prterr("DIAG DeActivateBox on Screen [%s] [%s] starting with Widget [%d] Returning box count [%d] at line [%d] program [%s].",scn->module,scn->name,count,line,file);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					return(count);
				}
				break;
			default:
				prterr("Error Widget Type [%d] incorrect for Widget [%d] on Screen [%s] [%s] at line [%d] program [%s].",wdgt->type,x,scn->module,scn->name,line,file);
				break;
		}		
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG DeActivateBox on Screen [%s] [%s] starting with Widget [%d] Returning box count [%d] at line [%d] program [%s].",scn->module,scn->name,count,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(count);
}
/*---------------------------------------------------------------------------
	Set RDAscrn resource numbers from RDArsrc structure 
---------------------------------------------------------------------------*/
void xsetrscnum(RDArsrc *rsc,short (*EvalFunc)(...),void *EvalFuncArgs,int line,char *file)
{
	int x,norsc;
	RDAwdgt *wdgt;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui) 
	{ 
		prterr("DIAG setrscnum Setting Resource Numbers on Screen [%s] [%s] at line [%d] program [%s].",rsc->module,rsc->screen,line,file); 
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(rsc!=NULL) 
	{
	if(EvalFunc==NULL)
	{
		for(x=0,wdgt=rsc->scn->wdgts;x<rsc->scn->numwdgts;++x,++wdgt)
		{
			if((wdgt->type==15 || wdgt->type==16 || 
				wdgt->type==17 || wdgt->type==18 || 
				wdgt->type==19 || wdgt->type==0 || 
				wdgt->type==6 || wdgt->type==7 || 
				wdgt->type==8 || wdgt->type==9 ||
				wdgt->type==22) && 
				wdgt->resource_name!=NULL)
			{
				norsc=xFINDRSC(rsc,wdgt->resource_name,__LINE__,__FILE__);
				if(norsc==(-1)) 
				{
					if(!RDAstrcmp(wdgt->resource_name,"QUIT") && !RDAstrcmp(rsc->screen,"DIAGNOSTIC SCREEN"))
					{
						wdgt->resource_num=(-1);
					} else {
						prterr("Error Resource [%s] Not Found on Screen [%s] [%s] at line [%d] program [%s].",wdgt->resource_name,rsc->module,rsc->screen,line,file);
						wdgt->resource_num=(-1);
					}
				} else wdgt->resource_num=norsc;
			}
		}
	} else {
/* Evaluating Expression on widgets */
		for(x=0;x<rsc->scn->numwdgts;++x)
		{
			wdgt=rsc->scn->wdgts+x;
			if(wdgt->resource_num!=(-1))
			{
			switch(wdgt->type)
			{
				case 15: /* custom input fields */
				case 16: /* expenditure fields */
				case 17: /* revenue fields */
				case 18: /* balance sheet fields */
				case 19: /* beginning balance fields */
				case 0: /* standard resource */
				case 6: /* button */
				case 7: /* scrolled list */
				case 8: /* scrolled text */
				case 9: /* toggle button */
				case 14: /* optional screens */
				case 22: /* progress bar */
					norsc=xFINDRSC(rsc,wdgt->resource_name,__LINE__,__FILE__);
					if(norsc==(-1)) 
					{
						if((!RDAstrcmp(wdgt->resource_name,"QUIT") && (!RDAstrcmp(rsc->screen,"DIAGNOSTIC SCREEN") || !strncmp(rsc->screen,"WARNING",7))) || !RDAstrcmp(wdgt->resource_name,"BROWSE"))
						{
							wdgt->resource_num=(-1);
						} else {
#ifdef USE_RDA_DIAGNOSTICS
							if(diaggui || diaggui_field) 
							{ 
								prterr("Error Resource [%s] Not Found on Screen [%s] [%s] at line [%d] program [%s].",wdgt->resource_name,rsc->module,rsc->screen,line,file);
							}
#endif /* USE_RDA_DIAGNOSTICS */
							wdgt->resource_num=(-1);
						}
					} else {
						if(PP_translate_GUIFUNC!=NULL)
						{
/*lint -e746 */
							PP_translate_GUIFUNC(&wdgt->expression);
/*lint +e746 */
						}
						if(EvalFuncArgs!=NULL)
						{
/*lint -e746 */
							if(EvalFunc(wdgt->expression,rsc,EvalFuncArgs))
/*lint +e746 */
							{
								wdgt->resource_num=norsc;
							} else wdgt->resource_num=(-1);
						} else {
/*lint -e746 */
							if(EvalFunc(wdgt->expression,rsc))
/*lint +e746 */
							{
								wdgt->resource_num=norsc;
							} else wdgt->resource_num=(-1);
						}
					}
					break;
				case 1: /* new line */
				case 32: /* new Panel */
					if(PP_translate_GUIFUNC!=NULL)
					{
/*lint -e746 */
						PP_translate_GUIFUNC(&wdgt->expression);
/*lint +e746 */
					}
					if(EvalFuncArgs!=NULL)
					{
/*lint -e746 */
						if(EvalFunc(wdgt->expression,rsc,EvalFuncArgs))
/*lint +e746 */
						{
							wdgt->resource_num=0;
						} else {
							norsc=DeActivateLine(rsc->scn,x);
							x+=norsc-1;
						}
					} else {
/*lint -e746 */
						if(EvalFunc(wdgt->expression,rsc))
/*lint +e746 */
						{
							wdgt->resource_num=0;
						} else {
							norsc=DeActivateLine(rsc->scn,x);
							x+=norsc-1;
						}
					}
					break;
				case 2: /* end line */
				case 4: /* end box */
				case 5: /* label */
				case 10: /* frame */
				case 11: /* seperator */
				case 13: /* end scrolled window */
				case 21: /* end tab container */
				case 25: /* end popup menu */
				case 27: /* end popup menu */
				case 23: /* New Tab Bar */
				case 29: /* end table */
				case 31: /* end header */
				case 33: /* end panel */
					if(PP_translate_GUIFUNC!=NULL)
					{
/*lint -e746 */
						PP_translate_GUIFUNC(&wdgt->expression);
/*lint +e746 */
					}
					if(EvalFuncArgs!=NULL)
					{
/*lint -e746 */
						if(EvalFunc(wdgt->expression,rsc,EvalFuncArgs)) wdgt->resource_num=0;
/*lint +e746 */
							else wdgt->resource_num=(-1);
					} else {
/*lint -e746 */
						if(EvalFunc(wdgt->expression,rsc)) wdgt->resource_num=0;
/*lint +e746 */
							else wdgt->resource_num=(-1);
					}
					break;
				case 3: /* new box */
					if(PP_translate_GUIFUNC!=NULL)
					{
/*lint -e746 */
						PP_translate_GUIFUNC(&wdgt->expression);
/*lint +e746 */
					}
					if(EvalFuncArgs!=NULL)
					{
/*lint -e746 */
						if(EvalFunc(wdgt->expression,rsc,EvalFuncArgs))
/*lint +e746 */
						{
							wdgt->resource_num=0;
						} else {
							norsc=DeActivateBox(rsc->scn,x);
							x+=norsc-1;
						}
					} else {
/*lint -e746 */
						if(EvalFunc(wdgt->expression,rsc))
/*lint +e746 */
						{
							wdgt->resource_num=0;
						} else {
							norsc=DeActivateBox(rsc->scn,x);
							x+=norsc-1;
						}
					}
					break;
				case 12: /* new scrolled window */
					if(PP_translate_GUIFUNC!=NULL)
					{
/*lint -e746 */
						PP_translate_GUIFUNC(&wdgt->expression);
/*lint +e746 */
					}
					if(EvalFuncArgs!=NULL)
					{
/*lint -e746 */
						if(EvalFunc(wdgt->expression,rsc,EvalFuncArgs))
/*lint +e746 */
						{
							wdgt->resource_num=0;
						} else {
							norsc=DeActivateScrolledWindow(rsc->scn,x);
							x+=norsc-1;
						}
					} else {
/*lint -e746 */
						if(EvalFunc(wdgt->expression,rsc))
/*lint +e746 */
						{
							wdgt->resource_num=0;
						} else {
							norsc=DeActivateScrolledWindow(rsc->scn,x);
							x+=norsc-1;
						}
					}
					break;
				case 20: /* new tab container */
					if(PP_translate_GUIFUNC!=NULL)
					{
/*lint -e746 */
						PP_translate_GUIFUNC(&wdgt->expression);
/*lint +e746 */
					}
					if(EvalFuncArgs!=NULL)
					{
/*lint -e746 */
						if(EvalFunc(wdgt->expression,rsc,EvalFuncArgs))
/*lint +e746 */
						{
							wdgt->resource_num=0;
						} else {
							norsc=DeActivateTabContainer(rsc->scn,x);
							x+=norsc-1;
						}
					} else {
/*lint -e746 */
						if(EvalFunc(wdgt->expression,rsc))
/*lint +e746 */
						{
							wdgt->resource_num=0;
						} else {
							norsc=DeActivateTabContainer(rsc->scn,x);
							x+=norsc-1;
						}
					}
					break;
				case 24: /* new popup menu */
					if(PP_translate_GUIFUNC!=NULL)
					{
/*lint -e746 */
						PP_translate_GUIFUNC(&wdgt->expression);
/*lint +e746 */
					}
					if(EvalFuncArgs!=NULL)
					{
/*lint -e746 */
						if(EvalFunc(wdgt->expression,rsc,EvalFuncArgs))
/*lint +e746 */
						{
							wdgt->resource_num=0;
						} else {
							norsc=DeActivatePopup(rsc->scn,x);
							x+=norsc-1;
						}
					} else {
/*lint -e746 */
						if(EvalFunc(wdgt->expression,rsc))
/*lint +e746 */
						{
							wdgt->resource_num=0;
						} else {
							norsc=DeActivatePopup(rsc->scn,x);
							x+=norsc-1;
						}
					}
					break;
				case 26: /* new toolbar */
				case 28: /* new table */
				case 30: /* new header */
					if(PP_translate_GUIFUNC!=NULL)
					{
/*lint -e746 */
						PP_translate_GUIFUNC(&wdgt->expression);
/*lint +e746 */
					}
					if(EvalFuncArgs!=NULL)
					{
/*lint -e746 */
						if(EvalFunc(wdgt->expression,rsc,EvalFuncArgs))
/*lint +e746 */
						{
							wdgt->resource_num=0;
						} else {
							norsc=DeActivateToolbar(rsc->scn,x);
							x+=norsc-1;
						}
					} else {
/*lint -e746 */
						if(EvalFunc(wdgt->expression,rsc))
/*lint +e746 */
						{
							wdgt->resource_num=0;
						} else {
							norsc=DeActivateToolbar(rsc->scn,x);
							x+=norsc-1;
						}
					}
					break;
				default:
					prterr("Error Widget Type [%d] is invalid for Widget [%d] on Screen [%s] [%s] at line [%d] program [%s].",wdgt->type,rsc->module,rsc->screen,line,file);
					break;
			}
			}
		}
	}
	}
}
/*---------------------------------------------------------------------------
	list_callback	Generic List Callback Function 
---------------------------------------------------------------------------*/
/*lint -e715 */
void list_doubleclick_callback(RDArmem *member)
{
	RDArsrc *rsrc;
	RDArmem *s=NULL;
	Wt::WComboBox *CB=NULL;
	int x=0;

	rsrc=member->parent;
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field) 
	{ 
		prterr("DIAG list_doubleclick_callback Executing the List Double-Click Callback for Resource [%s] on Screen [%s] [%s].",member->rscrname,rsrc->module,rsrc->screen);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(NoLosingFocus!=NULL && NoLosingFocus!=member)
	{
		losingfocusfunction(NoLosingFocus);
		NoLosingFocus=NULL;
	}
	NoLosingFocus=member;
	if(member->editable && member->user_editable)
	{
		CB=(Wt::WComboBox *)member->w;
		*member->value.integer_value=CB->currentIndex();
	}
	if(rsrc->rscs!=NULL)
	{
		for(x=0,s=rsrc->rscs;x<rsrc->numrscs;++x,++s)
		{
			if(!RDAstrcmp(s->rscrname,"SELECT")) break;
		}
		if(x<rsrc->numrscs)
		{
			ExecuteRDArmemFunction(s);
		}
	}
}
void list_callback(RDArmem *member)
{
	Wt::WComboBox *CB=NULL;
	RDArsrc *rsrc;

	if(member->app_update==TRUE) return;
	member->app_update=TRUE;
	rsrc=member->parent;
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field) 
	{ 
		prterr("DIAG list_callback Executing the List Callback for Resource [%s] on Screen [%s] [%s].",member->rscrname,rsrc->module,rsrc->screen);
	}
	if(NoLosingFocus!=NULL && NoLosingFocus!=member)
	{
		losingfocusfunction(NoLosingFocus);
		NoLosingFocus=NULL;
	}
	NoLosingFocus=member;
#endif /* USE_RDA_DIAGNOSTICS */
	if(rsrc->input_focus!=NULL) Rfree(rsrc->input_focus);
	rsrc->input_focus=stralloc(member->rscrname);
#ifdef TIMEOUT_CURSORS
	TimeoutCursors(rsrc,TRUE);
#endif
	CB=(Wt::WComboBox *)member->w;
	if(member->editable && member->user_editable)
	{
		*member->value.integer_value=CB->currentIndex();
		ExecuteRDArmemFunction(member);
		if(!isEMPTY(member->transversal_expression))
		{
			ComputeNewTransversalADV(rsrc,rsrc->EvalStr,rsrc->EvalStrArgs,rsrc->SubFunc,rsrc->SubFuncArgs,member->transversal_expression);
		}
	} else {
		CB->setCurrentIndex(*member->value.integer_value);
	}
	member->app_update=FALSE;
#ifdef TIMEOUT_CURSORS
	TimeoutCursors(rsrc,FALSE);
#endif
}
/*lint +e715 */
void xkillwindow(RDArsrc *r,int line,char *file)
{
	RDArmem *m;
	int y,x=0;
	Wt::WContainerWidget *cW=NULL;
	Wt::WDialog *wD=r->primary;
	std::string *s1=NULL;
#ifdef _DEFER_RENDERING_ATTEMPT_
	Wt::WApplication *myAPP=NULL;
#endif /* _DEFER_RENDERING_ATTEMPT_ */

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG killwindow Killing/Removing Screen [%s] [%s] at line [%d] program [%s].",r->module,r->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!IsScreenDisplayed(r)) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diaggui || diaggui_field)
		{
			prterr("DIAG killwindow returning without killing anything, window not displayed....");
		}
#endif /* USE_RDA_DIAGNOSTICS */
		return;
	}
#ifdef _DEFER_RENDERING_ATTEMPT_
	if(!RDAstrcmp(r->screen,"DIAGNOSTIC SCREEN"))
	{
		myAPP=RDAMAINWIDGET->instance();
		myAPP->resumeRendering();
	}	
#endif /* _DEFER_RENDERING_ATTEMPT_ */
	if((r->swidget!=NULL) && r->rscs!=NULL)
	{
		if(r->primary==NULL)
		{
			cW=(Wt::WContainerWidget *)r->swidget;
			cW->clear();
		} else {
			r->primary->accept();
		}
		r->swidget=NULL;
		r->primary=NULL;
	}
	--WindowCount;
	if(WindowCount<1)
	{
/* use javascript to kill window */
	}
}
/*---------------------------------------------------------------------------
	Callback function used to run a function passed as an argument 
---------------------------------------------------------------------------*/
/*lint -e715 */
void runfuncexitcallback(RDArmem *member)
{
	RDArsrc *rs=NULL;
	char *memname=NULL,*modname=NULL,*scrname=NULL,editable=FALSE;
	RDArmem *m=NULL;
	int y=0,pn=0;
	Wt::WDialog *wD=NULL;
	Wt::WContainerWidget *cW=NULL;
	std::string *s1=NULL;
	Wt::WString *c=NULL;

	if(member->editable==FALSE || member->user_editable==FALSE) return;
	if(member->sensitive==FALSE || member->user_sensitive==FALSE) return;
	if(NoLosingFocus!=NULL)
	{
		losingfocusfunction(NoLosingFocus);
		NoLosingFocus=NULL;
	}
	rs=(RDArsrc *)member->parent;
	wD=rs->primary;
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field) 
	{ 
		memname=stralloc(member->rscrname);
		modname=stralloc(rs->module);
		scrname=stralloc(rs->screen);
		prterr("DIAG runfuncexitcallback Run Button's Function and Exit if it's the last window for Resource [%s] on Screen [%s] [%s].",member->rscrname,rs->module,rs->screen);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	cW=(Wt::WContainerWidget *)rs->swidget;
	if(member->editable && member->user_editable) editable=TRUE;
		else editable=FALSE;
	if(editable)
	{
		if(rs->input_focus!=NULL) Rfree(rs->input_focus);
		rs->input_focus=stralloc(member->rscrname);
#ifdef TIMEOUT_CURSORS
		TimeoutCursors(rs,TRUE);
#endif
		if(member->function!=NULL)
		{
			ExecuteRDArmemFunction(member);
		}
#ifdef TIMEOUT_CURSORS
		TimeoutCursors(rs,FALSE);
#endif
		if(wD!=NULL) 
		{
			wD->accept();
/*
			wD->~WDialog();
*/
		}
#ifdef USE_RDA_DIAGNOSTICS
		if(diaggui || diaggui_field) 
		{ 
			prterr("DIAG runfuncexitcallback Destroyed screen [%s] [%s].",modname,scrname);
			if(memname!=NULL) Rfree(memname);
			if(modname!=NULL) Rfree(modname);
			if(scrname!=NULL) Rfree(scrname);
		}
#endif 
		--WindowCount;
		if(WindowCount<1) 
		{
		}
	}
}
/*lint +e715 */
/*---------------------------------------------------------------------------
	Callback function used to run a function passed as an argument and
		remove the current window
---------------------------------------------------------------------------*/
/*lint -e715 */
void runfunckwincallback(RDArmem *member)
{
	RDArsrc *rs;
	Wt::WContainerWidget *cW=NULL;
	Wt::WDialog *pwidget=NULL;
	char *memname=NULL,*modname=NULL,*scrname=NULL,editable=FALSE;
	RDArmem *mem=NULL;
	int y=0,pn=0;
	std::string *s1=NULL;
#ifdef _DEFER_RENDERING_ATTEMPT_
	Wt::WApplication *myAPP=NULL;
#endif /* _DEFER_RENDERING_ATTEMPT_ */

	if(member->editable==FALSE || member->user_editable==FALSE) return;
	if(member->sensitive==FALSE || member->user_sensitive==FALSE) return;
	if(NoLosingFocus!=NULL)
	{
		losingfocusfunction(NoLosingFocus);
		NoLosingFocus=NULL;
	}
	rs=(RDArsrc *)member->parent;
	inside_rfkw=TRUE;
	rfkw_rsrc=rs;
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field) 
	{ 
		memname=stralloc(member->rscrname);
		modname=stralloc(rs->module);
		scrname=stralloc(rs->screen);
		prterr("DIAG runfunckwincallback Run Function and Kill Window for Resource [%s] on Screen [%s] [%s].",member->rscrname,rs->module,rs->screen); 
	}
#endif /* USE_RDA_DIAGNOSTICS */
#ifdef _DEFER_RENDERING_ATTEMPT_
	if(!RDAstrcmp(rs->screen,"DIAGNOSTIC SCREEN"))
	{
		myAPP=RDAMAINWIDGET->instance();
		myAPP->resumeRendering();
	}	
#endif /* _DEFER_RENDERING_ATTEMPT_ */
	if((rs->primary!=NULL) && rs->rscs!=NULL)
	{
		pwidget=rs->primary;
	}
	cW=(Wt::WContainerWidget *)rs->swidget;
	if(member->editable && member->user_editable) editable=TRUE;
		else editable=FALSE;
	if(editable)
	{
		if(rs->input_focus!=NULL) Rfree(rs->input_focus);
		rs->input_focus=stralloc(member->rscrname);
#ifdef TIMEOUT_CURSORS
		TimeoutCursors(rs,TRUE);
#endif
		if(member->function!=NULL)
		{
			ExecuteRDArmemFunction(member);
		}
#ifdef TIMEOUT_CURSORS
		TimeoutCursors(rs,FALSE);
#endif
		if(inside_rfkw!=(-1)) cW->clear();
		if(pwidget!=NULL) 
		{
			pwidget->accept();
/*
			pwidget->~WDialog();
*/
		}
#ifdef USE_RDA_DIAGNOSTICS
		if(diaggui || diaggui_field) 
		{ 
			prterr("DIAG runfunckwincallback Destroyed screen [%s] [%s].",modname,scrname);
			if(memname!=NULL) Rfree(memname);
			if(modname!=NULL) Rfree(modname);
			if(scrname!=NULL) Rfree(scrname);
		}
#endif
		--WindowCount;
		if(WindowCount<1)
		{
/* use javascript to kill window */
		}
	}
	inside_rfkw=FALSE;
	rfkw_rsrc=NULL;
}
/*lint +e715 */
/*---------------------------------------------------------------------------
	Callback function used to run a function passed as an argument 
---------------------------------------------------------------------------*/
/*lint -e715 */
void runfunctioncallback(RDArmem *member)
{
	RDArsrc *rsrc;
	short editable=FALSE;

	if(member->editable==FALSE || member->user_editable==FALSE) return;
	if(member->sensitive==FALSE || member->user_sensitive==FALSE) return;
	if(NoLosingFocus!=NULL)
	{
		losingfocusfunction(NoLosingFocus);
		NoLosingFocus=NULL;
	}
	rsrc=member->parent;
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field) 
	{ 
		prterr("DIAG runfunctioncallback Run Function Callback for Resource [%s] on Screen [%s] [%s].",member->rscrname,rsrc->module,rsrc->screen); 
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(member->editable && member->user_editable) editable=TRUE;
		else editable=FALSE;
	if(editable)
	{
		if(rsrc->input_focus!=NULL) Rfree(rsrc->input_focus);
		rsrc->input_focus=stralloc(member->rscrname);
#ifdef TIMEOUT_CURSORS
		TimeoutCursors(rsrc,TRUE);
#endif
		if(member->function!=NULL)
		{
			if(!isEMPTY(member->transversal_expression))
			{
				ComputeNewTransversalADV(rsrc,rsrc->EvalStr,rsrc->EvalStrArgs,rsrc->SubFunc,rsrc->SubFuncArgs,member->transversal_expression);
			}
			ExecuteRDArmemFunction(member);
		}
#ifdef TIMEOUT_CURSORS
		TimeoutCursors(rsrc,FALSE);
#endif
	}
}
/*lint +e715 */
char *FormattedValidator(char *i,char *mask,short type)
{
	char *rstring=NULL,*temp=NULL,*temp1=NULL;
	int x=0;

	if(isEMPTY(mask))
	{
		rstring=stralloc(i);
		return(rstring);
	}
	if(!isEMPTY(i))
	{
		rstring=stralloc(i);
		for(x=0,temp=rstring,temp1=mask;(x<RDAstrlen(i));++x,++temp,++temp1)
		{
			if(*temp1=='*')
			{
				break;
			} else if(*temp1==' ') 
			{
				if((*temp!=' ') && type==0)
				{
					*temp=0;
					break;
				}
			} else if(*temp1==':' || *temp1=='-' || *temp1=='/')
			{
				if(*temp!=*temp1) 
				{
					*temp=0;
					break;
				}
			} else if(*temp1=='0')
			{
				if((!isdigit(*temp)) && ((*temp!=' ') || type!=1))
				{
					*temp=0;
					break;
				}
			} else if(*temp1=='a')
			{
				if((!isalpha(*temp)) && ((*temp!=' ') || type!=1))
				{
					*temp=0;
					break;
				}
			} else if((*temp1=='n'))
			{
				if((!isalpha(*temp) && !isdigit(*temp)) && ((*temp!=' ') || type!=1))
				{
					*temp=0;
					break;
				}
			}
		}
		if(!isEMPTY(rstring) && RDAstrlen(rstring)>0)
		{
			--x;
			for(;x>(-1);--x)
			{
				temp=rstring+x;
				temp1=mask+x;
				if(*temp1=='*')
				{
					break;
				} else if(*temp1==' ') 
				{
					if(*temp==' ') *temp=0;
				} else if(*temp1==':' || *temp1=='-' || *temp1=='/')
				{
					if(*temp==*temp1) *temp=0;
				} else break;
			}
		}	
	}
	return(rstring);
}
/*---------------------------------------------------------------------------
	readwidget - gets the value of a widget and load it in value field 
---------------------------------------------------------------------------*/
void xreadmember(RDArmem *member,int line,char *file)
{
	unsigned l;
	char *value=NULL,*value2use=NULL;
	Wt::WString text;
	Wt::WDate QD;
	Wt::WDateEdit *DE=NULL;
	Wt::WLineEdit *LE=NULL;
	Wt::WTextArea *TA=NULL;
	Wt::WTextEdit *TE=NULL;
	Wt::WAbstractToggleButton *aTB=NULL;
	Wt::WProgressBar *pBar=NULL;
	Wt::WComboBox *CB=NULL;
	std::string s1;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG readmember Reading Resource [%s] Editable [%s] Type [%d] Field Type [%d] [%s].",member->rscrname,(member->editable ? "Yes":"No"),member->type,member->field_type,standardfieldtypes[member->field_type]);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(member->w!=NULL)
	{
	switch(member->field_type)
	{
		case SCROLLEDLIST:
			if(!USER_INTERFACE)
			{
				CB=(Wt::WComboBox *)member->w;
				*member->value.integer_value=CB->currentIndex();
			}
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] has value [%d].",
					member->rscrname,*member->value.integer_value);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			break;
		case VARIABLETEXT:
		case PLAINTEXT:
		case SOCSECNUM:
		case PHONE:
		case ZIPCODE:
		case OPTIONALFIELDS:
			LE=(Wt::WLineEdit *)member->w;
			text=LE->text();
			s1=text.toUTF8();
			value=stralloc(s1.c_str());
			if(!isEMPTY(value)) unpad(value);
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] has value [%s].",member->rscrname,(value!=NULL ? value:""));
			}
#endif /* USE_RDA_DIAGNOSTICS */
			if(!isEMPTY(value))
			{
				l=RDAstrlen(value);
				QUICKALLOC(member->value.string_value,member->dlen,l+1);
				memcpy(member->value.string_value,(char *)value,l+1);
			} else if(member->dlen>0)
			{
				memset(member->value.string_value,0,member->dlen);
			} else {
				member->value.string_value=NULL;
				member->dlen=0;
			}
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] has value [%s].",member->rscrname,(member->value.string_value!=NULL ? member->value.string_value:"NULL"));
			}
#endif /* USE_RDA_DIAGNOSTICS */
			break;
		case SCROLLEDTEXT:
			if(member->rtype<2)
			{
				TA=(Wt::WTextArea *)member->w;
				text=TA->text();
			} else {
				TE=(Wt::WTextEdit *)member->w;
				text=TE->text();
			}
			s1=text.toUTF8();
			value=stralloc(s1.c_str());
			if(!isEMPTY(value)) unpad(value);
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] has value [%s].",member->rscrname,(value!=NULL ? value:""));
			}
#endif /* USE_RDA_DIAGNOSTICS */
			if(!isEMPTY(value))
			{
				l=RDAstrlen(value);
				QUICKALLOC(member->value.string_value,member->dlen,l+1);
				memcpy(member->value.string_value,(char *)value,l+1);
			} else if(member->dlen>0)
			{
				memset(member->value.string_value,0,member->dlen);
			} else {
				member->value.string_value=NULL;
				member->dlen=0;
			}
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] has value [%s].",member->rscrname,(member->value.string_value!=NULL ? member->value.string_value:"NULL"));
			}
#endif /* USE_RDA_DIAGNOSTICS */
			break;
		case DATES:
			if(member->rtype==0)
			{
				LE=(Wt::WLineEdit *)member->w;
				text=LE->text();
				s1=text.toUTF8();
				value=stralloc(s1.c_str());
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Resource [%s] has value [%s].",member->rscrname,(value!=NULL ? value:""));
				}
#endif /* USE_RDA_DIAGNOSTICS */
				if(!isEMPTY(value))
				{
					l=RDAstrlen(value);
					QUICKALLOC(member->value.string_value,member->dlen,l+1);
					memcpy(member->value.string_value,(char *)value,l+1);
				} else if(member->dlen>0)
				{
					memset(member->value.string_value,0,member->dlen);
				} else {
					member->value.string_value=NULL;
					member->dlen=0;
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Resource [%s] has value [%s].",member->rscrname,(member->value.string_value!=NULL ? member->value.string_value:"NULL"));
				}
#endif /* USE_RDA_DIAGNOSTICS */
			} else {
				DE=(Wt::WDateEdit *)member->w;
				QD=DE->date();
				text=QD.toString();
				s1=text.toUTF8();
				value=stralloc(s1.c_str());
				l=RDAstrlen(value);
				QUICKALLOC(member->value.string_value,member->dlen,l+1);
				memcpy(member->value.string_value,(char *)value,l+1);
			}
			break;
		case TIMES:
			if(member->rtype==0)
			{
				LE=(Wt::WLineEdit *)member->w;
				text=LE->text();
				s1=text.toUTF8();
				value=stralloc(s1.c_str());
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Resource [%s] has value [%s].",member->rscrname,(value!=NULL ? value:""));
				}
#endif /* USE_RDA_DIAGNOSTICS */
				if(!isEMPTY(value))
				{
					l=RDAstrlen(value);
					QUICKALLOC(member->value.string_value,member->dlen,l+1);
					memcpy(member->value.string_value,(char *)value,l+1);
				} else if(member->dlen>0)
				{
					memset(member->value.string_value,0,member->dlen);
				} else {
					member->value.string_value=NULL;
					member->dlen=0;
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Resource [%s] has value [%s].",member->rscrname,(member->value.string_value!=NULL ? member->value.string_value:"NULL"));
				}
#endif /* USE_RDA_DIAGNOSTICS */
			} else {
			}
			break;
		case CUSTOMTYPE:
		case EXPENDITURE:
		case REVENUE:
		case BALANCESHEET:
		case BEGINNINGBALANCE:
			LE=(Wt::WLineEdit *)member->w;
			text=LE->text();
			s1=text.toUTF8();
			value=stralloc(s1.c_str());
			unpad(value);
			if((RDAstrstr(member->rscrname," IMAGE") && member->callbacktype==1))
			{
				value2use=stralloc(value);
			} else {
				value2use=FormattedValidator(value,member->definition,0);
			}
			if(member->field_type!=CUSTOMTYPE)
			{
			while(!isEMPTY(value2use))
			{
				l=RDAstrlen(value2use);
				if((value2use[l-1]=='-') || (value2use[l-1]=='/') || (value2use[l-1]==':'))
				{
					value2use[l-1]=0;
					unpad(value2use);
				} else {
					break;
				}
			}
			}
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] has value [%s].",member->rscrname,(value2use!=NULL ? value2use:""));
			}
#endif /* USE_RDA_DIAGNOSTICS */
			if(!isEMPTY(value2use))
			{
				l=RDAstrlen(value2use);
				QUICKALLOC(member->value.string_value,member->dlen,l+1);
				memcpy(member->value.string_value,(char *)value2use,l+1);
			} else if(member->dlen>0)
			{
				memset(member->value.string_value,0,member->dlen);
			} else {
				member->value.string_value=NULL;
				member->dlen=0;
			}
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] has value [%s].",member->rscrname,(member->value.string_value!=NULL ? member->value.string_value:"NULL"));
			}
#endif /* USE_RDA_DIAGNOSTICS */
			if(value2use!=NULL) Rfree(value2use);
			break;
		case BOOLNS:
			if(!USER_INTERFACE)
			{
				aTB=(Wt::WAbstractToggleButton *)member->w;
				if(aTB->isChecked())
				{
					*member->value.string_value=TRUE;
				} else *member->value.string_value=FALSE;
			}
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] has value [%s].",member->rscrname,(*member->value.string_value ? "True":"False"));
			}
#endif /* USE_RDA_DIAGNOSTICS */
			break;
		case CHARACTERS:
			LE=(Wt::WLineEdit *)member->w;
			text=LE->text();
			s1=text.toUTF8();
			value=stralloc(s1.c_str());
			if(!isEMPTY(value))
			{
				*member->value.string_value=*value;
			} else *member->value.string_value=0;
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] has value [%c].",
					member->rscrname,*member->value.string_value);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			break;
		case DOLLARS:
		case DOLLARS_NOCENTS:
			LE=(Wt::WLineEdit *)member->w;
			text=LE->text();
			s1=text.toUTF8();
			value=stralloc(s1.c_str());
			*member->value.float_value=stripfamt(value);
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] has value [%f].",
					member->rscrname,*member->value.float_value);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			break;
		case DECIMALV:
			LE=(Wt::WLineEdit *)member->w;
			text=LE->text();
			s1=text.toUTF8();
			value=stralloc(s1.c_str());
			if(!isEMPTY(value))
			{
			*member->value.float_value=atof((char *)value);
			} else *member->value.float_value=0;
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] has value [%f].",
					member->rscrname,*member->value.float_value);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			break;
		case SDECIMALV:
			LE=(Wt::WLineEdit *)member->w;
			text=LE->text();
			s1=text.toUTF8();
			value=stralloc(s1.c_str());
			*member->value.float_value=convneg(value);
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] has value [%f].",
					member->rscrname,*member->value.float_value);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			break;
		case SDOUBLEV:
			LE=(Wt::WLineEdit *)member->w;
			text=LE->text();
			s1=text.toUTF8();
			value=stralloc(s1.c_str());
			*member->value.float_value=convneg(value);
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] has value [%f].",
					member->rscrname,*member->value.float_value);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			break;
		case DOUBLEV:
			LE=(Wt::WLineEdit *)member->w;
			text=LE->text();
			s1=text.toUTF8();
			value=stralloc(s1.c_str());
			if(!isEMPTY(value))
			{
				*member->value.float_value=atof((char *)value);
			} else *member->value.float_value=0;
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] has value [%f].",
					member->rscrname,*member->value.float_value);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			break;
		case SHORTV:
			LE=(Wt::WLineEdit *)member->w;
			text=LE->text();
			s1=text.toUTF8();
			value=stralloc(s1.c_str());
			if(!isEMPTY(value))
			{
				*member->value.short_value=Ratoi((char *)value); 
			} else *member->value.short_value=0;
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] has value [%d].",
					member->rscrname,*member->value.short_value);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			break;
		case SSHORTV:
			LE=(Wt::WLineEdit *)member->w;
			text=LE->text();
			s1=text.toUTF8();
			value=stralloc(s1.c_str());
			*member->value.short_value=convshortneg((char *)value); 
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] has value [%d].",
					member->rscrname,*member->value.short_value);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			break;
		case LONGV:
			LE=(Wt::WLineEdit *)member->w;
			text=LE->text();
			s1=text.toUTF8();
			value=stralloc(s1.c_str());
			if(!isEMPTY(value))
			{
				*member->value.integer_value=Ratoi((char *)value);
			} else *member->value.integer_value=0;
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] has value [%d].",
					member->rscrname,*member->value.integer_value);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			break;
		case SLONGV:
			LE=(Wt::WLineEdit *)member->w;
			text=LE->text();
			s1=text.toUTF8();
			value=stralloc(s1.c_str());
			*member->value.integer_value=convintneg((char *)value);

#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] has value [%d].",
					member->rscrname,*member->value.integer_value);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			break;
		case PROGRESS_BAR:
			break;
		default:
			prterr("Error Field Type [%d] is invalid for Resource [%s] at line [%d] program [%s].",member->rscrname,line,file);
			break;
	}
	}
	if(value!=NULL) Rfree(value);
}
void xreadwidget(RDArsrc *rsc,char *rsrcname,int line,char *file)
{
	short x;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui) 
	{ 
		prterr("DIAG readwidget Read Widget [%s]'s contents off Screen [%s] [%s] at line# [%d] program [%s].",(rsrcname!=NULL ? rsrcname:""),rsc->module,rsc->screen,line,file); 
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if((x=xFINDRSC(rsc,rsrcname,__LINE__,__FILE__))==(-1))
	{
		prterr("Error Resource [%s] not found on Screen [%s] [%s] at line [%d] program [%s].",rsrcname,rsc->module,rsc->screen,line,file);
		return;
	}
	member=rsc->rscs+x;
	if(!USER_INTERFACE)
	{
		if(member->editable && member->user_editable && member->type==0 && member->w!=NULL)
		{
			readmember(member);
		}
	}
}
/*---------------------------------------------------------------------------
	readallwidgets - gets the values of the widgets and loads it in value 
---------------------------------------------------------------------------*/
void xreadallwidgets(RDArsrc *rsc,short geteditable,short getsensitive,int line,char *file)
{
	short x;
	RDArmem *member;
	short editable=TRUE;
	short sensitive=TRUE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui) 
	{ 
		prterr("DIAG readallwidgets Read all widgets on Screen [%s] [%s] at line# [%d] program [%s].",rsc->module,rsc->screen,line,file); 
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!USER_INTERFACE)
	{
		for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
		{
			if(member->editable && member->user_editable && member->type==0 && member->w!=NULL)
			{
				readmember(member);
				if(geteditable==TRUE) MEMBERGETEDITABLE(member,&editable);
				if(getsensitive==TRUE) MEMBERGETSENSITIVE(member,&sensitive);
			}
		}
	}
}
short xFINDRSCGETTABLECOLUMN(RDArsrc *rsc,char *name,int line,char *file)
{
	short x;
	RDArmem *member;
	int row=(-1);

	if(rsc!=NULL)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diaggui || diaggui_field)
		{
			prterr("DIAG FINDRSCGETTABLECOLUMN Getting Current Table Column from Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
		}
#endif /* USE_RDA_DIAGNOSTICS */
		for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
		{
			if(!RDAstrcmp(member->rscrname,name))
			{
				if(member->tCell!=NULL)
				{
					row=member->tCell->column();
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG FINDRSCGETTABLECOLUMN [%s] returning [%d] ",member->rscrname,row);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					return(row);
				}
			}
		}
	}
	return(-1);
}
short xFINDRSCGETTABLEROW(RDArsrc *rsc,char *name,int line,char *file)
{
	short x;
	RDArmem *member;
	int row=(-1);

	if(rsc!=NULL)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diaggui || diaggui_field)
		{
			prterr("DIAG FINDRSCGETTABLEROW Getting Current Table Row from Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
		}
#endif /* USE_RDA_DIAGNOSTICS */
		for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
		{
			if(!RDAstrcmp(member->rscrname,name))
			{
				if(member->tCell!=NULL)
				{
					row=member->tCell->row();
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG FINDRSCGETTABLEROW [%s] returning [%d] ",member->rscrname,row);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					return(row);
				}
			}
		}
	}
	return(-1);
}
short xFINDRSCSETTABLEFUNC(RDArsrc *rsc,char *name,void (*func)(...),void *arglist,
	short function_type,int line,char *file)
{
	short x;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCSETFUNC Setting activate function for Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			member->table_function=func;
			member->table_arglist=arglist;
			member->table_function_type=function_type;
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG FINDRSCSETFUNC Resource [%s] on Screen [%s] [%s] has activate function and arguments set at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			return(0);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("Error Resource [%s] not found on Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(-1); 
}
/*---------------------------------------------------------------------------
xFINDRSCGETSTRING	Function to find a resource in a resource structure
			and get it's string value.
---------------------------------------------------------------------------*/
short xFINDRSCGETSTRING(RDArsrc *rsc,char *name,char **value,int line,
	char *file)
{
	short x;
	RDArmem *member;

	if(rsc!=NULL)
	{
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCGETSTRING Getting String Value for Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			switch(member->field_type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case PLAINTEXT:
				case SOCSECNUM:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					if(!isEMPTY(member->value.string_value))
					{
						if(member->dlen>RDAstrlen(*value) || *value==NULL)
						{
#ifdef USE_RDA_DIAGNOSTICS
							if(diagallocx && diagmisc)
							{
								if(*value!=NULL) 
								{
			 						*value=xRrealloc((void *)*value,member->dlen+1,line,file);
								} else {
									*value=xRmalloc(member->dlen+1,line,file);
						 		}
							} else {
								if(*value!=NULL) 
								{
			 						*value=Rrealloc((void *)*value,member->dlen+1);
								} else {
									*value=Rmalloc(member->dlen+1);
						 		}
							}
#else
							if(*value!=NULL) 
							{
			 					*value=Rrealloc((void *)*value,member->dlen+1);
							} else {
								*value=Rmalloc(member->dlen+1);
							}
#endif /* USE_RDA_DIAGNOSTICS */
						}
						memcpy(*value,member->value.string_value,member->dlen);
					} else {
						if(*value!=NULL) Rfree(*value);
						*value=NULL;
					}
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG FINDRSCGETSTRING Resource [%s] on Screen [%s] [%s] has value set at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				case BOOLNS:
				case CHARACTERS:
					prterr("Error the FINDRSCGETSTRING at line [%d] program [%s] should be replaced with FINDRSCGETCHAR.",line,file);
					SEEMEMBER(member);TRACE;
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					prterr("Error the FINDRSCGETSTRING at line [%d] program [%s] should be replaced with FINDRSCGETDOUBLE.",line,file);
					SEEMEMBER(member);TRACE;
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
				case PROGRESS_BAR:
					prterr("Error the FINDRSCGETSTRING at line [%d] program [%s] should be replaced with FINDRSCGETINT.",line,file);
					SEEMEMBER(member);TRACE;
					break;
				case SHORTV:
				case SSHORTV:
					prterr("Error the FINDRSCGETSTRING at line [%d] program [%s] should be replaced with FINDRSCGETSHORT.",line,file);
					SEEMEMBER(member);TRACE;
					break;
				default:
					prterr("Error Field Type [%d] is invalid for Resource [%s] on Screen [%s] [%s] at line [%d] program [%s].",member->field_type,member->rscrname,rsc->module,rsc->screen,line,file);
					break;
			}
			return(0);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("Error Resource [%s] not found on Screen [%s] [%s] at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	} else {
		prterr("Error rsc is NULL.");TRACE;
	}
	return(-1); 
}
/*---------------------------------------------------------------------------
	FINDRSCLISTAPPlib
---------------------------------------------------------------------------*/
short xFINDRSCLISTAPPlib(RDArsrc *rsc,char *name,int position,APPlib *app,
	int line,char *file)
{
	short x;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCLISTAPPlib Setting Scrolled List Using APPlib Structure for Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			if(member->field_type==SCROLLEDLIST)
			{
				*member->value.integer_value=position;
				member->items=app->numlibs;
				member->list=&app->libs;
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG FINDRSCLISTAPPlib Scrolled List Resource [%s] on Screen [%s] [%s] is set at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
					SEEMEMBER(member);TRACE;
				}
#endif /* USE_RDA_DIAGNOSTICS */
				return(0);
			}
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("Error Resource [%s] not found on Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(-1); 
}
/*---------------------------------------------------------------------------
	FINDRSCSETFUNC
---------------------------------------------------------------------------*/
short xFINDRSCSETFUNC(RDArsrc *rsc,char *name,void (*func)(...),void *arglist,
	short function_type,int line,char *file)
{
	short x;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCSETFUNC Setting activate function for Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			member->function=func;
			member->arglist=arglist;
			member->function_type=function_type;
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG FINDRSCSETFUNC Resource [%s] on Screen [%s] [%s] has activate function and arguments set at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			return(0);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("Error Resource [%s] not found on Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(-1); 
}
/*---------------------------------------------------------------------------
	FINDRSCSETLIST
---------------------------------------------------------------------------*/
short xFINDRSCSETLIST(RDArsrc *rsc,char *name,int position,int items,
	char ***list,int line,char *file)
{
	short x;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCSETLIST Setting Scrolled List Resource [%s] on Screen [%s] [%s] to position [%d] number items [%d] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,position,items,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			if(member->field_type==SCROLLEDLIST)
			{
				*member->value.integer_value=position;
				member->items=items;
				member->list=list;
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG FINDRSCSETLIST Scrolled List Resource [%s] on Screen [%s] [%s] is set at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
					SEEMEMBER(member);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				return(0);
			}
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("Error Resource [%s] not found on Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(-1); 
}
/*---------------------------------------------------------------------------
xFINDRSCGETVALUE	Function to find a resource in a resource structure
			and get it's numerical value.
---------------------------------------------------------------------------*/
short xFINDRSCGETVALUE(RDArsrc *rsc,char *name,void *value,int line,char *file)
{
	short x;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCGETVALUE Getting value of Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			switch(member->field_type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case PLAINTEXT:
				case PHONE:
				case SOCSECNUM:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BEGINNINGBALANCE:
				case BALANCESHEET:
				case OPTIONALFIELDS:
					prterr("Error Resource [%s] on Screen [%s] [%s] Requires the Use xFINDRSCGETSTRING at line [%d] program [%s].",member->rscrname,rsc->module,rsc->screen,line,file);
					break;
				case BOOLNS:
				case CHARACTERS:
					*(char *)value=*member->value.string_value;
					if(!RDAstrcmp(USERLOGIN,"sjs") || 
						!RDAstrcmp(USERLOGIN,"bld"))
					{
						prterr("WARNING the FINDRSCGETVALUE at line [%d] program [%s] should be replaced with FINDRSCGETCHAR.",line,file);
					}
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has found value at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case SDECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
					*(double *)value=*member->value.float_value;
					if(!RDAstrcmp(USERLOGIN,"sjs") || 
						!RDAstrcmp(USERLOGIN,"bld"))
					{
						prterr("WARNING the FINDRSCGETVALUE at line [%d] program [%s] should be replaced with FINDRSCGETDOUBLE.",line,file);
					}
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has found value at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				case SHORTV:
				case SSHORTV:
					*(short *)value=*member->value.short_value;
					if(!RDAstrcmp(USERLOGIN,"sjs") || 
						!RDAstrcmp(USERLOGIN,"bld"))
					{
						prterr("WARNING the FINDRSCGETVALUE at line [%d] program [%s] should be replaced with FINDRSCGETSHORT.",line,file);
					}
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has found value at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				case PROGRESS_BAR:
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					*(int *)value=*member->value.integer_value;
					if(!RDAstrcmp(USERLOGIN,"sjs") || 
						!RDAstrcmp(USERLOGIN,"bld"))
					{
						prterr("WARNING the FINDRSCGETVALUE at line [%d] program [%s] should be replaced with FINDRSCGETINT.",line,file);
					}
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has found value at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				default:
					prterr("Error Field Type [%d] is invalid for Resource [%s] on Screen [%s] [%s] at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
					break;
			}
			return(0);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("Error Resource [%s] not found Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(-1); 
}
short xFINDRSCGETCHAR(RDArsrc *rsc,char *name,char *value,int line,char *file)
{
	short x;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCGETCHAR Getting value of Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			switch(member->field_type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case PLAINTEXT:
				case PHONE:
				case SOCSECNUM:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BEGINNINGBALANCE:
				case BALANCESHEET:
				case OPTIONALFIELDS:
					prterr("Error the FINDRSCGETCHAR at line [%d] program [%s] should be replaced with FINDRSCGETSTRING.",line,file);
					SEEMEMBER(member);
					break;
				case BOOLNS:
				case CHARACTERS:
					*(char *)value=*member->value.string_value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has found value at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case SDECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
					if(*member->value.float_value>-127 &&
						*member->value.float_value<128)
					{
						*(char *)value=*(char *)member->value.float_value;
#ifdef USE_RDA_DIAGNOSTICS
						if(diaggui || diaggui_field)
						{
							prterr("DIAG Resource [%s] on Screen [%s] [%s] has found value at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
							SEEMEMBER(member);
						}
#endif /* USE_RDA_DIAGNOSTICS */
					} else {
						prterr("Error in precision the FINDRSCGETCHAR at line [%d] program [%s] should be replaced with FINDRSCGETDOUBLE.",line,file);
						SEEMEMBER(member);
					}
					break;
				case SHORTV:
				case SSHORTV:
					if(*member->value.short_value>-127 &&
						*member->value.short_value<128)
					{
						*(char *)value=*(short *)member->value.short_value;
#ifdef USE_RDA_DIAGNOSTICS
						if(diaggui || diaggui_field)
						{
							prterr("DIAG Resource [%s] on Screen [%s] [%s] has found value at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
							SEEMEMBER(member);
						}
#endif /* USE_RDA_DIAGNOSTICS */
					} else {
						prterr("Error in precision the FINDRSCGETCHAR at line [%d] program [%s] should be replaced with FINDRSCGETSHORT.",line,file);
						SEEMEMBER(member);
					}
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
				case PROGRESS_BAR:
					if(*member->value.integer_value>-127 &&
						*member->value.integer_value<128)
					{
						*(char *)value=*(int *)member->value.integer_value;
#ifdef USE_RDA_DIAGNOSTICS
						if(diaggui || diaggui_field)
						{
							prterr("DIAG Resource [%s] on Screen [%s] [%s] has found value at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
							SEEMEMBER(member);
						}
#endif /* USE_RDA_DIAGNOSTICS */
					} else {
						prterr("Error in precision the FINDRSCGETCHAR at line [%d] program [%s] should be replaced with FINDRSCGETINT.",line,file);
						SEEMEMBER(member);
					}
					break;
				default:
					prterr("Error Field Type [%d] is invalid for Resource [%s] on Screen [%s] [%s] at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
					break;
			}
			return(0);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("Error Resource [%s] not found Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(-1); 
}
short xFINDRSCGETSHORT(RDArsrc *rsc,char *name,short *value,int line,char *file)
{
	short x;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCGETSHORT Getting value of Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			switch(member->field_type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case PLAINTEXT:
				case PHONE:
				case SOCSECNUM:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BEGINNINGBALANCE:
				case BALANCESHEET:
				case OPTIONALFIELDS:
					prterr("Error the FINDRSCGETSHORT at line [%d] program [%s] should be replaced with FINDRSCGETSTRING.",line,file);
					SEEMEMBER(member);
					break;
				case BOOLNS:
				case CHARACTERS:
					*value=*member->value.string_value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has found value at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case SDECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
					if(*member->value.float_value<SHRT_MAX)
					{
/*lint -e524*/
						*value=*member->value.float_value;
/*lint +e524*/
#ifdef USE_RDA_DIAGNOSTICS
						if(diaggui || diaggui_field)
						{
							prterr("DIAG Resource [%s] on Screen [%s] [%s] has found value at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
							SEEMEMBER(member);
						}
#endif /* USE_RDA_DIAGNOSTICS */
					} else {
						prterr("Error in precision the FINDRSCGETSHORT at line [%d] program [%s] should be replaced with FINDRSCGETDOUBLE.",line,file);
						SEEMEMBER(member);TRACE;
					}
					break;
				case SHORTV:
				case SSHORTV:
					*value=*member->value.short_value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has found value at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
				case PROGRESS_BAR:
					if(*member->value.integer_value<SHRT_MAX)
					{
/*lint -e537*/
						*value=*member->value.integer_value;
/*lint +e537*/
#ifdef USE_RDA_DIAGNOSTICS
						if(diaggui || diaggui_field)
						{
							prterr("DIAG Resource [%s] on Screen [%s] [%s] has found value at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
							SEEMEMBER(member);
						}
#endif /* USE_RDA_DIAGNOSTICS */
					} else {
						prterr("Error in precision the FINDRSCGETSHORT at line [%d] program [%s] should be replaced with FINDRSCGETDOUBLE.",line,file);
						SEEMEMBER(member);TRACE;
					}
					break;
				default:
					prterr("Error Field Type [%d] is invalid for Resource [%s] on Screen [%s] [%s] at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
					break;
			}
			return(0);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("Error Resource [%s] not found Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(-1); 
}
short xFINDRSCGETINT(RDArsrc *rsc,char *name,int *value,int line,char *file)
{
	short x;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCGETINT Getting value of Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			switch(member->field_type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case PLAINTEXT:
				case PHONE:
				case SOCSECNUM:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BEGINNINGBALANCE:
				case BALANCESHEET:
				case OPTIONALFIELDS:
					prterr("Error the FINDRSCGETINT at line [%d] program [%s] should be replaced with FINDRSCGETSTRING.",line,file);
					SEEMEMBER(member);
					break;
				case BOOLNS:
				case CHARACTERS:
					*value=*member->value.string_value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has found value at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case SDECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
					if(*member->value.float_value<INT_MAX)
					{
/*lint -e524*/
						*value=*member->value.float_value;
/*lint +e524*/
#ifdef USE_RDA_DIAGNOSTICS
						if(diaggui || diaggui_field)
						{
							prterr("DIAG Resource [%s] on Screen [%s] [%s] has found value at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
							SEEMEMBER(member);
						}
#endif /* USE_RDA_DIAGNOSTICS */
					} else {
						prterr("Error in precision the FINDRSCGETINT at line [%d] program [%s] should be replaced with FINDRSCGETDOUBLE.",line,file);
						SEEMEMBER(member);TRACE;
					}
					break;
				case SHORTV:
				case SSHORTV:
					*value=*member->value.short_value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has found value at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				case SCROLLEDLIST:
#ifdef XXXX
					if(IsWidgetDisplayed(member))
					{
						if(member->rows>1)
						{
							*member->value.integer_value=qt_QListBox_currentItem((qt_QListBox *)member->w);
						} else {
							*member->value.integer_value=qt_QComboBox_currentItem((qt_QComboBox *)member->w);
						}
					}
#endif /* XXXX */
					*value=*member->value.integer_value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has found value at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				case PROGRESS_BAR:
				case LONGV:
				case SLONGV:
					*value=*member->value.integer_value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has found value at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				default:
					prterr("Error Field Type [%d] is invalid for Resource [%s] on Screen [%s] [%s] at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
					break;
			}
			return(0);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("Error Resource [%s] not found Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(-1); 
}
short xFINDRSCGETDOUBLE(RDArsrc *rsc,char *name,double *value,int line,char *file)
{
	short x;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCGETDOUBLE Getting value of Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			switch(member->field_type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case PLAINTEXT:
				case PHONE:
				case SOCSECNUM:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BEGINNINGBALANCE:
				case BALANCESHEET:
				case OPTIONALFIELDS:
					prterr("Error the FINDRSCGETDOUBLE at line [%d] program [%s] should be replaced with FINDRSCGETSTRING.",line,file);
					SEEMEMBER(member);
					break;
				case BOOLNS:
				case CHARACTERS:
					*value=*member->value.string_value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has found value at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case SDECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
					*value=*member->value.float_value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has found value at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				case SHORTV:
				case SSHORTV:
					*value=*member->value.short_value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has found value at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				case PROGRESS_BAR:
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					*value=*member->value.integer_value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has found value at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				default:
					prterr("Error Field Type [%d] is invalid for Resource [%s] on Screen [%s] [%s] at line [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
					break;
			}
			return(0);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("Error Resource [%s] not found Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(-1); 
}
/*---------------------------------------------------------------------------
	FINDRSCSETVALUE	Function to find a resource in a resource structure
			and set it's value.
---------------------------------------------------------------------------*/
short xFINDRSCSETVALUE(RDArsrc *rsc,char *name,void *value,int line,char *file)
{
	short x;
	unsigned lf=0;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCSETVALUE Setting Value for Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			switch(member->field_type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case SOCSECNUM:
				case TIMES:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					if(!isEMPTY(value))
					{
						lf=RDAstrlen(value);
						QUICKALLOC(member->value.string_value,member->dlen,lf+1);
						memcpy(member->value.string_value,value,lf+1);
					} else if(member->dlen>0)
					{
						memset(member->value.string_value,0,member->dlen);
					} else {
						member->value.string_value=NULL;
						member->dlen=0;
					}
					if(member->dlen>member->field_length && member->field_length>0) member->value.string_value[member->field_length]=0;
					if(!RDAstrcmp(USERLOGIN,"sjs") || 
						!RDAstrcmp(USERLOGIN,"bld"))
					{
						prterr("WARNING the FINDRSCSETVALUE at line [%d] program [%s] should be replaced with FINDRSCSETSTRING.",line,file);
					}
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has value set at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				case BOOLNS:
				case CHARACTERS:
					*member->value.string_value=*(char *)value;
					if(!RDAstrcmp(USERLOGIN,"sjs") || 
						!RDAstrcmp(USERLOGIN,"bld"))
					{
						prterr("WARNING the FINDRSCSETVALUE at line [%d] program [%s] should be replaced with FINDRSCSETCHAR.",line,file);
					}
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has value set at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case SDECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
					*member->value.float_value=*(double *)value;
					if(!RDAstrcmp(USERLOGIN,"sjs") || 
						!RDAstrcmp(USERLOGIN,"bld"))
					{
						prterr("WARNING the FINDRSCSETVALUE at line [%d] program [%s] should be replaced with FINDRSCSETDOUBLE.",line,file);
					}
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has value set at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				case SHORTV:
				case SSHORTV:
					*member->value.short_value=*(short *)value;
					if(!RDAstrcmp(USERLOGIN,"sjs") || 
						!RDAstrcmp(USERLOGIN,"bld"))
					{
						prterr("WARNING the FINDRSCSETVALUE at line [%d] program [%s] should be replaced with FINDRSCSETSHORT.",line,file);
					}
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has value set at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				case PROGRESS_BAR:
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					*member->value.integer_value=*(int *)value;
					if(!RDAstrcmp(USERLOGIN,"sjs") || 
						!RDAstrcmp(USERLOGIN,"bld"))
					{
						prterr("WARNING the FINDRSCSETVALUE at line [%d] program [%s] should be replaced with FINDRSCSETINT.",line,file);
					}
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has value set at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				default:
					prterr("Error Field Type [%d] is invalid for Resource [%s] on Screen [%s] [%s] at line [%d] program [%s].",member->rscrname,rsc->module,rsc->screen,line,file);
					break;
			}
			return(0);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("Error Resource [%s] not found on Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(-1); 
}
short xFINDRSCSETSTRING(RDArsrc *rsc,char *name,char *value,int line,char *file)
{
	short x;
	unsigned lf=0;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCSETSTRING Setting Value for Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			switch(member->field_type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case SOCSECNUM:
				case TIMES:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					if(!isEMPTY(value))
					{
						lf=RDAstrlen(value);
						QUICKALLOC(member->value.string_value,member->dlen,lf+1);
						memcpy(member->value.string_value,value,lf+1);
					} else if(member->dlen>0)
					{
						memset(member->value.string_value,0,member->dlen);
					} else {
						member->value.string_value=NULL;
						member->dlen=0;
					}
					if(member->dlen>member->field_length && member->field_length>0) member->value.string_value[member->field_length]=0;
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has value set at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				case BOOLNS:
				case CHARACTERS:
					prterr("Error the FINDRSCSETSTRING at line [%d] program [%s] should be replaced with FINDRSCSETCHAR.",line,file);
					SEEMEMBER(member);TRACE;
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case SDECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
					prterr("Error the FINDRSCSETSTRING at line [%d] program [%s] should be replaced with FINDRSCSETDOUBLE.",line,file);
					SEEMEMBER(member);TRACE;
					break;
				case SHORTV:
				case SSHORTV:
					prterr("Error the FINDRSCSETSTRING at line [%d] program [%s] should be replaced with FINDRSCSETSHORT.",line,file);
					SEEMEMBER(member);TRACE;
					break;
				case PROGRESS_BAR:
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					prterr("Error the FINDRSCSETSTRING at line [%d] program [%s] should be replaced with FINDRSCSETINT.",line,file);
					SEEMEMBER(member);TRACE;
					break;
				default:
					prterr("Error Field Type [%d] is invalid for Resource [%s] on Screen [%s] [%s] at line [%d] program [%s].",member->rscrname,rsc->module,rsc->screen,line,file);
					break;
			}
			return(0);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("Error Resource [%s] not found on Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(-1); 
}
short xFINDRSCSETCHAR(RDArsrc *rsc,char *name,char value,int line,char *file)
{
	short x;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCSETVALUE Setting Value for Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			switch(member->field_type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case SOCSECNUM:
				case TIMES:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					prterr("Error the FINDRSCSETCHAR function at line [%d] program [%s] should be replaced with FINDRSCSETSTRING.",line,file);
					SEEMEMBER(member);TRACE;
					break;
				case BOOLNS:
				case CHARACTERS:
					*member->value.string_value=value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has value set at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case SDECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
					*member->value.float_value=value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has value set at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				case SHORTV:
				case SSHORTV:
					*member->value.short_value=value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has value set at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				case PROGRESS_BAR:
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					*member->value.integer_value=value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has value set at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				default:
					prterr("Error Field Type [%d] is invalid for Resource [%s] on Screen [%s] [%s] at line [%d] program [%s].",member->rscrname,rsc->module,rsc->screen,line,file);
					break;
			}
			return(0);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("Error Resource [%s] not found on Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(-1); 
}
short xFINDRSCSETSHORT(RDArsrc *rsc,char *name,short value,int line,char *file)
{
	short x;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCSETSHORT Setting Value for Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			switch(member->field_type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case SOCSECNUM:
				case TIMES:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					prterr("Error the FINDRSCSETSHORT at line [%d] program [%s] should be replaced with FINDRSCSETSTRING.",line,file);
					SEEMEMBER(member);TRACE;
					break;
				case BOOLNS:
				case CHARACTERS:
					if(value>-127 && value<128)
					{
						*member->value.string_value=value;
#ifdef USE_RDA_DIAGNOSTICS
						if(diaggui || diaggui_field)
						{
							prterr("DIAG Resource [%s] on Screen [%s] [%s] has value set at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
							SEEMEMBER(member);TRACE;
						}
#endif /* USE_RDA_DIAGNOSTICS */
					} else {
						prterr("Error in precision the using FINDRSCSETSHORT at line [%d] program [%s] replace with FINDRSCSETCHAR or switch field type to short integer.");
						SEEMEMBER(member);TRACE;
					}
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case SDECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
					*member->value.float_value=value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has value set at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				case SHORTV:
				case SSHORTV:
					*member->value.short_value=value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has value set at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				case PROGRESS_BAR:
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					*member->value.integer_value=value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has value set at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				default:
					prterr("Error Field Type [%d] is invalid for Resource [%s] on Screen [%s] [%s] at line [%d] program [%s].",member->rscrname,rsc->module,rsc->screen,line,file);
					break;
			}
			return(0);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("Error Resource [%s] not found on Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(-1); 
}
short xFINDRSCSETINT(RDArsrc *rsc,char *name,int value,int line,char *file)
{
	short x;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCSETINT Setting Value for Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			switch(member->field_type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case SOCSECNUM:
				case TIMES:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					prterr("Error the FINDRSCSETINT at line [%d] program [%s] should be replaced with FINDRSCSETSTRING.",line,file);
					SEEMEMBER(member);TRACE;
					break;
				case BOOLNS:
				case CHARACTERS:
					if(value>-127 && value<128)
					{
						*member->value.string_value=value;
#ifdef USE_RDA_DIAGNOSTICS
						if(diaggui || diaggui_field)
						{
							prterr("DIAG Resource [%s] on Screen [%s] [%s] has value set at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
							SEEMEMBER(member);TRACE;
						}
#endif /* USE_RDA_DIAGNOSTICS */
					} else {
						prterr("Error in precision the FINDRSCSETINT at line [%d] program [%s] should be replaced with a FINDRSCSETCHAR.",line,file);
						SEEMEMBER(member);TRACE;
					}
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case SDECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
					*member->value.float_value=value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has value set at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				case SHORTV:
				case SSHORTV:
					if(value<SHRT_MAX)
					{
						*member->value.short_value=value;
#ifdef USE_RDA_DIAGNOSTICS
						if(diaggui || diaggui_field)
						{
							prterr("DIAG Resource [%s] on Screen [%s] [%s] has value set at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
							SEEMEMBER(member);TRACE;
						}
#endif /* USE_RDA_DIAGNOSTICS */
					} else {
						prterr("Error in precision the FINDRSCSETINT at line [%d] program [%s] should be replaced with a FINDRSCSETSHORT.",line,file);
						SEEMEMBER(member);TRACE;
					}
					break;
				case PROGRESS_BAR:
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					*member->value.integer_value=value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has value set at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				default:
					prterr("Error Field Type [%d] is invalid for Resource [%s] on Screen [%s] [%s] at line [%d] program [%s].",member->rscrname,rsc->module,rsc->screen,line,file);
					break;
			}
			return(0);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("Error Resource [%s] not found on Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(-1); 
}
short xFINDRSCSETDOUBLE(RDArsrc *rsc,char *name,double value,int line,char *file)
{
	short x;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCSETDOUBLE Setting Value for Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			switch(member->field_type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case SOCSECNUM:
				case TIMES:
				case PLAINTEXT:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
					prterr("Error the FINDRSCSETDOUBLE at line [%d] program [%s] should be replaced with FINDRSCSETSTRING.",line,file);
					SEEMEMBER(member);TRACE;
					break;
				case BOOLNS:
				case CHARACTERS:
					if(value>-127 && value<128)
					{
/*lint -e524*/
						*member->value.string_value=value;
/*lint +e524*/
#ifdef USE_RDA_DIAGNOSTICS
						if(diaggui || diaggui_field)
						{
							prterr("DIAG Resource [%s] on Screen [%s] [%s] has value set at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
							SEEMEMBER(member);TRACE;
						}
#endif /* USE_RDA_DIAGNOSTICS */
					} else {
						prterr("Error in precision the FINDRSCSETDOUBLE at line [%d] program [%s] should be replaced with a FINDRSCSETCHAR.",line,file);
						SEEMEMBER(member);TRACE;
					}
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case SDECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
					*member->value.float_value=value;
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Resource [%s] on Screen [%s] [%s] has value set at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
						SEEMEMBER(member);TRACE;
					}
#endif /* USE_RDA_DIAGNOSTICS */
					break;
				case SHORTV:
				case SSHORTV:
					if(value<SHRT_MAX)
					{
/*lint -e524*/
						*member->value.short_value=value;
/*lint +e524*/
#ifdef USE_RDA_DIAGNOSTICS
						if(diaggui || diaggui_field)
						{
							prterr("DIAG Resource [%s] on Screen [%s] [%s] has value set at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
							SEEMEMBER(member);TRACE;
						}
#endif /* USE_RDA_DIAGNOSTICS */
					} else {
						prterr("Error in precision the FINDRSCSETDOUBLE at line [%d] program [%s] should be replaced with a FINDRSCSETSHORT.",line,file);
						SEEMEMBER(member);TRACE;
					}
					break;
				case PROGRESS_BAR:
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					if(value<INT_MAX)
					{
/*lint -e524*/
						*member->value.integer_value=value;
/*lint +e524*/
#ifdef USE_RDA_DIAGNOSTICS
						if(diaggui || diaggui_field)
						{
							prterr("DIAG Resource [%s] on Screen [%s] [%s] has value set at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
							SEEMEMBER(member);TRACE;
						}
#endif /* USE_RDA_DIAGNOSTICS */
					} else {
						prterr("Error in precision the FINDRSCSETDOUBLE at line [%d] program [%s] should be replaced with a FINDRSCSETINT.",line,file);
						SEEMEMBER(member);TRACE;
					}
					break;
				default:
					prterr("Error Field Type [%d] is invalid for Resource [%s] on Screen [%s] [%s] at line [%d] program [%s].",member->rscrname,rsc->module,rsc->screen,line,file);
					break;
			}
			return(0);
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("Error Resource [%s] not found on Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(-1); 
}
/*----------------------------------------------------------------------------
	RDAscrnNEW 	sets up a new screen structure
-----------------------------------------------------------------------------*/
RDAscrn *xRDAscrnNEW(char *module,char *name,int line,char *file)
{
	RDAscrn *t;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG RDAscrnNEW Creating New Screen Widget Structure for Module [%s] Screen [%s] at line# [%d] program [%s].",module,name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	t=Rmalloc(sizeof(RDAscrn));
	t->numwdgts=0;
	if(module!=NULL) t->module=stralloc(module);
		else t->module=NULL;
	if(name!=NULL) t->name=stralloc(name);
		else t->name=NULL;
	t->wdgts=NULL;
	return(t);
}
/*----------------------------------------------------------------------------
	RDArsrcNEW 	sets up a new resource structure
-----------------------------------------------------------------------------*/
RDArsrc *xRDArsrcNEW(char *module,char *screen,int line,char *file)
{
	RDArsrc *t;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG RDArsrcNEW Creating New Resource Structure for Module [%s] Screen [%s] at line# [%d] program [%s].",module,screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	t=Rmalloc(sizeof(RDArsrc));
/* add these later */
	if(module!=NULL) t->module=stralloc(module);
		else t->module=NULL;
	if(screen!=NULL) t->screen=stralloc(screen);
		else t->screen=NULL;
	t->input_focus=NULL;
	t->numrscs=0;
	t->rscs=NULL;
	t->num=0;
	t->numlists=0;
	t->lists=NULL;
	t->numloads=0;
	t->loads=NULL;
	t->numvir=0;
	t->primary=NULL;
	t->swidget=NULL;
	t->virflds=NULL;
	t->incvir=NULL;
	t->SetFunc=NULL;
	t->SetFuncArgs=NULL;
	t->scn=NULL;
	t->EvalStr=NULL;
	t->EvalStrArgs=NULL;
	t->SubFunc=NULL;
	t->SubFuncArgs=NULL;	
	t->modal=FALSE;
	t->has_tabbar=FALSE;
	t->has_large_table=FALSE;
	t->window_toolbar=NULL;
	return(t);
}
short xFINDRSCSHOWVALUE(RDArsrc *rsc,char *name,int line,char *file)
{
	short x=0;
	RDArmem *member=NULL;

	prterr("FINDRSCSHOWVALUE at line [%d] program [%s].",line,file);
	if((x=FINDRSC(rsc,name))==(-1))
	{
		prterr("Error Resource [%s] not found on Screen [%s] [%s] at line [%d] program [%s]. Cannot update specified resource.",name,rsc->module,rsc->screen,line,file);
		return(FALSE);
	}
	member=rsc->rscs+x;
	SEEMEMBER(member);
	return(TRUE);
}
void xSEERSRCS(RDArsrc *rsrc,int line,char *file)
{
	int x;
	RDArmem *member;
	DFincvir *i;
	RDAvirtual *v;
	RDAScrolledList *list=NULL;
	RDALoadValue *load=NULL;

	prterr("DIAG SEERSRCS Showing Resources Available on Screen [%s] [%s] at line [%d] program [%s].",rsrc->module,rsrc->screen,line,file);
	if(rsrc->incvir!=NULL)
	{
		prterr("Files Available:\n");
		for(x=0,i=rsrc->incvir;x<rsrc->numvir;++x,++i)
		{
			prterr("\tModule [%s] File [%s] Keyname [%s] File Number [%d]\n",(i->module!=NULL ? i->module:""),(i->file!=NULL ? i->file:""),(i->keyname!=NULL ? i->keyname:""),i->fileno);
		}
	}
	if(rsrc->virflds!=NULL)
	{
		prterr("Virtual Field Definitions Available:\n");
		for(x=0,v=rsrc->virflds;x<rsrc->num;++x,++v)
		{
			prterr("\tName [%s] Type [%d] When [%d] Length [%d] Expression [%s]\n",v->name,v->type,v->when,v->len,v->expression);
		}
	}
	if(rsrc->lists!=NULL)
	{
		prterr("RDAScrolledList Definitions Available:\n");
		for(x=0,list=rsrc->lists;x<rsrc->numlists;++x,++list)
		{
			prterr("\tName [%s] Connect Type: [%d] Module:[%s] File:[%s] Field:[%s]\n",list->name,list->contype,list->conmod,list->confil,list->confld);
		}
	}
	if(rsrc->loads!=NULL)
	{
		prterr("RDALoadValue Definitions Available:\n");
		for(x=0,load=rsrc->loads;x<rsrc->numloads;++x,++load)
		{
			prterr("\tName [%s] Module: [%s] Connect Name: [%s]\n",load->name,load->module,load->conname);
		}
	}
	for(x=0,member=rsrc->rscs;x<rsrc->numrscs;++x,++member)
	{
		SEEMEMBER(member);
	}
}
void xSEEMEMBER(RDArmem *member,int line,char *file)
{
	prterr("Resource: [%s]  Type: [%d]\n",member->rscrname,member->type);
	prterr("   Field type: [%d][%s]  Field length: [%d]\n",member->field_type,standardfieldtypes[member->field_type],member->field_length);
	if(!USER_INTERFACE)
	{
		prterr("      Editable: [%s]  Widget exists: [%s]  ",(member->editable ? "Yes":"No"),(member->w!=NULL ? "Yes":"No"));
	}
	switch(member->field_type)
	{
		case SCROLLEDTEXT:
		case VARIABLETEXT:
		case DATES:
		case TIMES:
		case PLAINTEXT:
		case SOCSECNUM:
		case PHONE:
		case CUSTOMTYPE:
		case ZIPCODE:
		case EXPENDITURE:
		case REVENUE:
		case BALANCESHEET:
		case BEGINNINGBALANCE:
		case OPTIONALFIELDS:
			prterr("Value: [%s] ",(member->value.string_value!=NULL ? member->value.string_value:"NULL"));
			break;
		case BOOLNS:
			prterr("Value: [%s] ",(*member->value.string_value ? "Yes":"No"));

			break;
		case CHARACTERS:
			prterr("Value: [%c] ",*member->value.string_value);
			break;
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case DECIMALV:
		case SDECIMALV:
		case DOUBLEV:
		case SDOUBLEV:
			prterr("Value: [%f] ",*member->value.float_value);
			break;
		case SHORTV:
		case SSHORTV:
			prterr("Value: [%d] ",*member->value.short_value);
			break;
		case PROGRESS_BAR:
		case SCROLLEDLIST:
		case LONGV:
		case SLONGV:
			prterr("Value: [%d] ",*member->value.integer_value);
			break;
		case BUTTONS:
			break;
		default:
			prterr("/nError Field Type [%d] is invalid for Resource [%s] at line [%d] program [%s].",member->field_type,member->rscrname,line,file);
			break;
	}
	prterr("\n");
}
void xSEESCRN(RDAscrn *s,int line,char *file)
{
	int x;
	RDAwdgt *w;

	prterr("DIAG SEESCRN Showing RDAscrn Elements for Screen [%s] [%s] at line [%d] program [%s].",(s->module!=NULL ? s->module:"Null"),(s->name!=NULL ? s->name:"Null"),line,file);
	for(x=0,w=s->wdgts;x<s->numwdgts;++x,++w)
	{
		prterr("Widget [%d] Type [%d] [%s] Resource Name [%s] Number [%d] Label [%s] Rows [%d] Cols [%d]\n",x,w->type,wdgttypes[w->type],(w->resource_name!=NULL ? w->resource_name:""),w->resource_num,(w->label!=NULL ? w->label:""),w->rows,w->cols);
	}	
	prterr("\n");
}
short xMEMBERGETVISABLE(RDArmem *member,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG MEMBERGETVISABLE Visable Status is [%s] for Widget [%s] at line [%d] program [%s].",(member->w==NULL?"True":"False"),member->rscrname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!USER_INTERFACE)
	{
		if(member->w==NULL) return(1);
			else return(-1);
	}
}
short xFINDRSCGETVISABLE(RDArsrc *rsc,char *name,short *visable,int line,char *file)
{
	short x=FALSE;
	RDArmem *member=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCGETVISABLE Assertaining if Widget [%s] is on the Screen [%s][%s] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if((x=FINDRSC(rsc,name))!=(-1))
	{
		member=rsc->rscs+x;
		*visable=MEMBERGETVISABLE(member);
		return(x);
	} else {
		return(x);
	}
}
int xFINDRSCGETCURSORPOSITION(RDArsrc *rsc,char *name,int line,char *file)
{
	int x,p=0,i=0,y=0;
	RDArmem *member;
	Wt::WTextArea *TA=NULL;
	Wt::WTextEdit *TE=NULL;
	Wt::WLineEdit *LE=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCGETCURSORPOSITION Getting Position of Cursor in Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			switch(member->field_type)
			{
				case SCROLLEDTEXT:
					if(!USER_INTERFACE) 
					{
						if(member->rtype<2)
						{
							TA=(Wt::WTextArea *)member->w;
							i=TA->cursorPosition();
						} else {
							TE=(Wt::WTextEdit *)member->w;
							i=TE->cursorPosition();
						}
						return(i);
					} else {
						prterr("Error Character Interface FINDRSCGETCURSORPOSITION function is unavailable.");
						return(-1);
					}
				case DATES:
					if(!USER_INTERFACE) 
					{
						if(member->rtype==0)
						{
							LE=(Wt::WLineEdit *)member->w;
							return(LE->cursorPosition());
						}
						return(0);
					} else {
						prterr("Error Character Interface FINDRSCGETCURSORPOSITION function is unavailable.");
						return(-1);
					}
					break;
				case VARIABLETEXT:
				case PLAINTEXT:
				case TIMES:
				case SOCSECNUM:
				case PHONE:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case OPTIONALFIELDS:
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case SDECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SHORTV:
				case SSHORTV:
				case LONGV:
				case SLONGV:
					if(!USER_INTERFACE) 
					{
						LE=(Wt::WLineEdit *)member->w;
						return(LE->cursorPosition());
					} else {
						prterr("Error Character Interface FINDRSCGETCURSORPOSITION function is unavailable.");
						return(-1);
					}
				case CHARACTERS:
				case BOOLNS:
				case SCROLLEDLIST:
				case PROGRESS_BAR:
					return(-1);
				default:
					prterr("Error Field Type [%d] invalid for Resource [%s] on Screen [%s] [%s] at line [%d] program [%s].",member->field_type,member->rscrname,rsc->module,rsc->screen,line,file);
					return(-1);
			}
		}
	}
	prterr("Error Resource [%s] not found on Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	return(-1); 
}
short xFINDRSCSETCURSOR(RDArsrc *rsc,char *name,int position,
	int line,char *file)
{
	short x;
	RDArmem *member;
	int p=0,i=0,tp=0,lp=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCSETCURSOR Setting Cursor Position for Resource [%s] on Screen [%s] [%s] to position [%d] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,position,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			if(!USER_INTERFACE)
			{
				switch(member->field_type)
				{
					case SCROLLEDTEXT:
						break;
					case VARIABLETEXT:
					case PLAINTEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PHONE:
					case CUSTOMTYPE:
					case ZIPCODE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case SDECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SHORTV:
					case SSHORTV:
					case LONGV:
					case SLONGV:
						if(!USER_INTERFACE) 
						{
						} else {
							prterr("Error Character Interface FINDRSCGETCURSORPOSITION function is unavailable.");
						}
						break;
					case CHARACTERS:
					case BOOLNS:
					case SCROLLEDLIST:
					case PROGRESS_BAR:
						break;
					default:
						prterr("Error Field Type [%d] invalid for Resource [%s] on Screen [%s] [%s] at line [%d] program [%s].",member->field_type,member->rscrname,rsc->module,rsc->screen,line,file);
						break;
				}
			}
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] on Screen [%s] [%s] has Cursor position [%d] set at line [%d] program [%s].",name,rsc->module,rsc->screen,position,line,file);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			return(0);
		}
	}
	prterr("Error Resource [%s] not found on Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	return(-1); 
}
short xFINDRSCGETACCTTYPE(RDArsrc *rsc,char *name,short *acctype,int line,char *file)
{
	short x;
	RDArmem *member=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCGETACCTTYPE Getting Account Type Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			switch(member->field_type)
			{
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case CUSTOMTYPE:
					if(member->field_type==EXPENDITURE)
					{
						*acctype=EXP_ACCT;
					} else if(member->field_type==REVENUE)
					{
						*acctype=REV_ACCT;
					} else if(member->field_type==BALANCESHEET)
					{
						*acctype=BAL_ACCT;
					} else if(member->field_type==BEGINNINGBALANCE)
					{
						*acctype=BEG_ACCT;
					} else if(member->field_type==CUSTOMTYPE)
					{
						*acctype=CUSTOM_ACCT;
					} else {
						prterr("Error Resource [%s] on Screen [%s] [%s] has the Invalid Field Type [%d] at line [%d] program [%s].",name,rsc->module,rsc->screen,member->field_type,line,file);
						*acctype=EXP_ACCT;
						return(-1);
					}
					break;
				default:
					prterr("Error Resource [%s] on Screen [%s] [%s] has the wrong field type [%d] [%s] to require FINDRSCGETACCTTYPE at line [%d] program [%s].",name,rsc->module,rsc->screen,member->field_type,standardfieldtypes[member->field_type],line,file);
					return(-1);
			}
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] on Screen [%s] [%s] has had account type changed to [%d] at line [%d] program [%s].",name,rsc->module,rsc->screen,*acctype,line,file);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			return(0);
		}
	}
	prterr("Error Resource [%s] not found on Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	return(-1); 
}
short xMEMBERSETEDITABLE(RDArmem *member,short editable,short which,int line,char *file)
{
	RDArsrc *rsc;
	Wt::WLineEdit *LE=NULL;
	Wt::WComboBox *LB=NULL;
	Wt::WString *c=NULL;
	Wt::WFormWidget *wFormW=NULL;
	Wt::WWidget *WW=NULL;
#ifdef __USE_ANIMATIONS__
	Wt::WAnimation fadeout(Wt::WAnimation::Fade,Wt::WAnimation::Linear,250);
	Wt::WAnimation fadein(Wt::WAnimation::Pop,Wt::WAnimation::Linear,250);
#endif /* __USE_ANIMATIONS__ */

	rsc=member->parent;
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG MEMBERSETEDITABLE Setting Editable Status for Resource [%s] to editable [%s] on Screen [%s] [%s] at line [%d] program [%s].",member->rscrname,(editable ? "True":"False"),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!which)
	{
		member->editable=editable;
	}
	member->user_editable=editable;
	if(!USER_INTERFACE)
	{
		WW=(Wt::WWidget *)member->w;
		wFormW=(Wt::WFormWidget *)member->w;
		if(member->w!=NULL && member->field_type!=BUTTONS)
		{ 
			wFormW->setReadOnly((editable ? FALSE:TRUE));
		} else if(member->w!=NULL && member->popup_w==NULL && member->field_type==BUTTONS)
		{
			wFormW->setEnabled((editable ? TRUE:FALSE));
#ifdef __USE_ANIMATIONS__
			if(editable==FALSE)
			{
				WW->animateHide(fadeout);
			} else {
				WW->animateShow(fadein);
			}
#endif /* __USE_ANIMATIONS__ */
		} else if(member->popup_w!=NULL && member->field_type==BUTTONS)
		{
			member->popup_w->setItemDisabled(member->menuitem,(editable ? FALSE:TRUE));
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG Resource [%s] on Screen [%s] [%s] has editable status set [%s] at line# [%d] program [%s].",member->rscrname,rsc->module,rsc->screen,(editable ? "Yes":"No"),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(0);
}
short xFINDRSCSETEDITABLE(RDArsrc *rsc,char *name,short editable,int line,char *file)
{
	short x;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCSETEDITABLE Setting Editable Status of Resource [%s] on Screen [%s] [%s] to [%s] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,(editable ? "Yes":"No"),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			return(xMEMBERSETEDITABLE(member,editable,FALSE,line,file));
		}
	}
	prterr("Error Resource [%s] not found on Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	return(-1); 
}
short xMEMBERGETEDITABLE(RDArmem *member,short *editable,int line,char *file)
{
	RDArsrc *rsc;
	Wt::WFormWidget *wFormW=NULL;

	rsc=member->parent;
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG MEMBERGETEDITABLE Getting Editable Status for Resource [%s] on Screen [%s] [%s] at line [%d] program [%s].",member->rscrname,rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!USER_INTERFACE)
	{
		if(member->menuitem==NULL)
		{
			wFormW=(Wt::WFormWidget *)member->w;
			*editable=wFormW->isReadOnly();
		} else {
			*editable=member->menuitem->isSelectable();
		}
	}
	member->user_editable=*editable;
	member->editable=*editable;
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG Resource [%s] on Screen [%s] [%s] has editable status of [%s] at line# [%d] program [%s].",member->rscrname,rsc->module,rsc->screen,(*editable?"Yes":"No"),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(0);
}
short xFINDRSCGETEDITABLE(RDArsrc *rsc,char *name,short *editable,int line,char *file)
{
	short x;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCGETEDITABLE Getting Editable Status of Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			return(xMEMBERGETEDITABLE(member,editable,line,file));
		}
	}
	prterr("Error Resource [%s] not found on Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	return(-1); 
}
void xTimeoutCursors(RDArsrc *rsrc,int on,int line,char *file)
{
	Wt::WWidget *WW=NULL;
	Wt::WCssDecorationStyle DS;

	Cursor widget_cursor=ArrowCursor;
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG TimeoutCursors Status [%s] at line [%d] program [%s] ",(on ? "Timeout":"Time in"),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(rsrc->swidget==NULL) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diaggui) 
		{
			prterr("DIAG Error Widget==NULL in TimeoutCursors at line [%d] program [%s].",line,file);
		}
#endif /* USE_RDA_DIAGNOSTICS */
		return;
	}
	WW=(Wt::WWidget *)rsrc->swidget;
	DS=WW->decorationStyle();
	widget_cursor=DS.cursor();
	if(widget_cursor!=NULL)
	{
		if(on)
		{
			RDA_cursorShape=WaitCursor;
		} else {
			RDA_cursorShape=ArrowCursor;
		}
		DS.setCursor(RDA_cursorShape);
	}
}
short xMEMBERSETSENSITIVE(RDArmem *member,short sensitive,short which,int line,char *file)
{
	Wt::WImage *MyP=NULL;
	Wt::WString *c=NULL;
	Wt::WWidget *WW=NULL;
	Wt::WFormWidget *wFormW=NULL;
#ifdef __USE_ANIMATIONS__
	Wt::WAnimation fadeout(Wt::WAnimation::Fade,Wt::WAnimation::Linear,250);
	Wt::WAnimation fadein(Wt::WAnimation::Pop,Wt::WAnimation::Linear,250);
#endif /* __USE_ANIMATIONS__ */

	if(!which)
	{
		member->sensitive=sensitive;
	} else member->user_sensitive=sensitive;
	if(!USER_INTERFACE)
	{
		WW=(Wt::WWidget *)member->w;
		wFormW=(Wt::WFormWidget *)member->w;
		if(member->w!=NULL && member->field_type!=BUTTONS)
		{ 
			wFormW->setReadOnly((sensitive ? FALSE:TRUE));
			wFormW->setEnabled((sensitive ? TRUE:FALSE));
#ifdef __USE_ANIMATIONS__
			if(sensitive==FALSE)
			{
				WW->animateHide(fadeout);
			} else {
				WW->animateShow(fadein);
			}
#endif /* __USE_ANIMATIONS__ */
		} else if(member->w!=NULL && member->popup_w==NULL && member->field_type==BUTTONS)
		{
			wFormW->setEnabled((sensitive ? TRUE:FALSE));
#ifdef __USE_ANIMATIONS__
			if(sensitive==FALSE)
			{
				WW->animateHide(fadeout);
			} else {
				WW->animateShow(fadein);
			}
#endif /* __USE_ANIMATIONS__ */
		} else if(member->popup_w!=NULL && member->field_type==BUTTONS)
		{
			member->popup_w->setItemDisabled(member->menuitem,(sensitive ? FALSE:TRUE));
		}
	} else {
		prterr("Error Character Interface doesn't support FINDRSCSETSENSITIVE at line# [%d] program [%s].",line,file);
	}
	member->user_sensitive=sensitive;
	return(0);
}
short xFINDRSCSETSENSITIVE(RDArsrc *rsc,char *name,short sensitive,int line,char *file)
{
	short x;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCSETSENSITIVE Setting sensitive status [%s] for Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(sensitive ? "Yes":"No"),(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			MEMBERSETSENSITIVE(member,sensitive);
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] on Screen [%s] [%s] has sensitive set [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,(sensitive ? "True":"False"),line,file);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			return(0);
		}
	}
	prterr("Error Resource [%s] not found on Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	return(-1); 
}
short xMEMBERGETSENSITIVE(RDArmem *member,short *sensitive,int line,char *file)
{
	RDArsrc *rsc;
	Wt::WWebWidget *WebW=NULL;
	Wt::WFormWidget *wFormW=NULL;

	rsc=member->parent;
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG MEMBERGETSENSITIVE Getting Sensitive Status for Resource [%s] on Screen [%s] [%s] at line [%d] program [%s].",member->rscrname,rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!USER_INTERFACE)
	{
		if(member->menuitem==NULL)
		{
			WebW=(Wt::WWebWidget *)member->w;
			*sensitive=WebW->isEnabled();
		} else {
			*sensitive=member->menuitem->isSelectable();
		}
	}
	member->user_sensitive=*sensitive;
	member->sensitive=*sensitive;
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG Resource [%s] on Screen [%s] [%s] has sensitive status of [%s] at line# [%d] program [%s].",member->rscrname,rsc->module,rsc->screen,(*sensitive?"Yes":"No"),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(0);
}
short xFINDRSCGETSENSITIVE(RDArsrc *rsc,char *name,short *sensitive,int line,char *file)
{
	short x;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCGETSENSITIVE Getting Sensitive Status of Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			return(xMEMBERGETSENSITIVE(member,sensitive,line,file));
		}
	}
	prterr("Error Resource [%s] not found on Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	return(-1); 
}
short checkforinterrupt(RDArsrc *r)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG checkforinterrupt on Screen [%s] [%s].",r->module,r->screen);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!IsScreenDisplayed(r)) return(-1);
	if(!USER_INTERFACE)
	{
	}
	return(FALSE);
}
void xForceWindowUpdate(RDArsrc *r,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG ForceWindowUpdate on Screen [%s] [%s] at line [%d] program [%s].",r->module,r->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!IsScreenDisplayed(r)) return;
	if(!USER_INTERFACE)
	{
	}
}
short xFINDRSCSETINPUTFOCUS(RDArsrc *rsc,char *name,int line,char *file)
{
	short x;
	RDArmem *member;
	Wt::WFormWidget *wFormW=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCSETINPUTFOCUS Setting input focus to Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(rsc->input_focus!=NULL) Rfree(rsc->input_focus);
	if(name!=NULL) rsc->input_focus=stralloc(name);
		else rsc->input_focus=NULL;
	if(rsc->swidget!=NULL)
	{
		for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
		{
			if(!RDAstrcmp(member->rscrname,name))
			{
				if(!USER_INTERFACE)
				{
					if(member->w!=NULL)
					{
						wFormW=(Wt::WFormWidget *)member->w;
						wFormW->setFocus();
					}
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Resource [%s] on Screen [%s] [%s] has input focus at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				return(0);
			}
		}
		prterr("Error INPUT FOCUS Resource [%s] not found on Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
		return(-1); 
	} else return(0);
}
short xRSRCCOPY(RDArsrc *frrsrc,char *rsrcname1,RDArsrc *torsrc,char *rsrcname2,int line,char *file)
{
	int frx=0,tox=0;
	unsigned lf=0;
	RDArmem *frmember=NULL,*tomember=NULL;

	if(frrsrc!=NULL && torsrc!=NULL)
	{
#ifdef USE_RDA_DIAGNOSTICS
        	if(diaggui || diaggui_field)
        	{
			prterr("DIAG RSRCCOPY Copying Rescource [%s] from Module [%s] Screen [%s] to Resource [%s] in Module [%s] Screen [%s] at line [%d] program [%s].",
				(rsrcname1!=NULL ? rsrcname1:"Null"),
				frrsrc->module,frrsrc->screen,
				(rsrcname2!=NULL ? rsrcname2:"Null"),
				torsrc->module,torsrc->screen,line,file);
        	}
#endif /* USE_RDA_DIAGNOSTICS */
		if((frx=FINDRSC(frrsrc,rsrcname1))==(-1)) return(-1);
		if((tox=FINDRSC(torsrc,rsrcname2))==(-1)) return(-1);
		frmember=frrsrc->rscs+frx;
		tomember=torsrc->rscs+tox;
		if(frmember->field_type==tomember->field_type)
		{
			switch(frmember->field_type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case DATES:
				case TIMES:
				case PLAINTEXT:
				case PHONE:
				case SOCSECNUM:
				case CUSTOMTYPE:
				case ZIPCODE:
				case EXPENDITURE:
				case REVENUE:
				case BEGINNINGBALANCE:
				case BALANCESHEET:
				case OPTIONALFIELDS:
					if(!isEMPTY(frmember->value.string_value))
					{
						lf=RDAstrlen(frmember->value.string_value);
						QUICKALLOC(tomember->value.string_value,tomember->dlen,lf+1);
						memcpy(tomember->value.string_value,frmember->value.string_value,lf+1);
					} else if(tomember->dlen>0)
					{
						memset(tomember->value.string_value,0,tomember->dlen);
					} else {
						tomember->value.string_value=NULL;
						tomember->dlen=0;
					}
					break;
				case BOOLNS:
				case CHARACTERS:
					*tomember->value.string_value=*frmember->value.string_value;
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case SDECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
					*tomember->value.float_value=*frmember->value.float_value;
					break;
				case SHORTV:
				case SSHORTV:
					*tomember->value.short_value=*frmember->value.short_value;
					break;
				case PROGRESS_BAR:
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					*tomember->value.integer_value=*frmember->value.integer_value;
					break;
				default:
					prterr("Error Field Type [%d] is invalid for Resource [%s] on Screen [%s] [%s] and Resource [%s] on Screen [%s] [%s] at line [%d] program [%s].",
						frmember->field_type,rsrcname1,
						frrsrc->module,frrsrc->screen,
						rsrcname2,torsrc->module,
						frrsrc->screen,line,file);
					break;
			}
			return(0);
		} else {
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] on Screen [%s] [%s] and Resource [%s] on Screen [%s] [%s] has different Field Types.  Can't Copy these fields at line [%d] program [%s].",
					rsrcname1,
					frrsrc->module,
					frrsrc->screen,
					rsrcname2,
					torsrc->module,
					frrsrc->screen,
					line,file);
				SEEMEMBER(frmember);
				SEEMEMBER(tomember);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			return(-1);
		}
	} else {
#ifdef USE_RDA_DIAGNOSTICS
		if(diaggui || diaggui_field)
		{
			prterr("Error (RDArsrc *)==NULL.");
		}
#endif /* USE_RDA_DIAGNOSTICS */
		return(-1);
	}
}
short xFINDRSCSETCALLBACKTYPE(RDArsrc *rsc,char *name,short ctype,int line,
	char *file)
{
	short x;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCSETCALLBACKTYPE Setting Callback Type to [%s] for Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(ctype ? "True":"False"),(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			switch(member->field_type)
			{
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case CUSTOMTYPE:
					member->callbacktype=ctype;
					break;
				default:
					prterr("Error Resource [%s] on Screen [%s] [%s] has the wrong field type [%d] [%s] to require a definition type at line [%d] program [%s].",name,rsc->module,rsc->screen,member->field_type,standardfieldtypes[member->field_type],line,file);
					return(-1);
			}
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] on Screen [%s] [%s] has callback type [%s] at line [%d] program [%s].",name,rsc->module,rsc->screen,(ctype ? "True":"False"),line,file);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			return(0);
		}
	}
	prterr("Error Resource [%s] not found on Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	return(-1); 
}
void xExecuteRDArmemFunction(RDArmem *member,int line,char *file)
{
	RDArsrc *r=NULL;
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG ExecuteRDArmemFunction for member [%s] at line [%d] program [%s].",member->rscrname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(member!=NULL) 
	{
		r=member->parent;
		if(member->function!=NULL)
		{
/*lint -e746 */
			if(!member->function_type)
			{
				if(member->arglist!=NULL)
				{
					member->function(member->parent,member->arglist);
				} else {
					member->function(member->parent);
				}
			} else {
				if(member->arglist!=NULL)
				{
					member->function(member,member->arglist);
				} else {
					member->function(member);
				}
			}
/*lint +e746 */
		}
	}
}
void xSETTITLE(RDArsrc *r,char *title,int line,char *file)
{
	WString *text=NULL;
	
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG SETTITLE Setting/Changing Title Bar to [%s] for Screen [%s] [%s] at line [%d] program [%s].",title,r->module,r->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!USER_INTERFACE)
	{
		if(r->primary!=NULL)
		{
			text=new Wt::WString(title);
			r->primary->setWindowTitle(*text);
			text->~WString();
		}
	}
}
void xFreeRDALoadValue(RDALoadValue *RLV,int line,char *file)
{
	int x;
	DFincvir *i=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG FreeRDALoadValue at line [%d] program [%s].",line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(RLV!=NULL)
	{
		if(RLV->module!=NULL) Rfree(RLV->module);
		if(RLV->name!=NULL) Rfree(RLV->name);
		if(RLV->incvir!=NULL)
		{
			for(x=0,i=RLV->incvir;x<RLV->numincvirs;++x,++i)
			{
				if(i->module!=NULL) Rfree(i->module);
				if(i->file!=NULL) Rfree(i->file);
				if(i->keyname!=NULL) Rfree(i->keyname);
			}
			Rfree(RLV->incvir);
		}
		if(RLV->conname!=NULL) Rfree(RLV->conname);
		if(RLV->dlist!=NULL) freeapplib(RLV->dlist);
		if(RLV->desc!=NULL) Rfree(RLV->desc);
		if(RLV->mlist!=NULL) freeapplib(RLV->mlist);
		Rfree(RLV);
	}
}

short xMEMBERSETDEFTYPE(RDArmem *member,short acctype,int line,char *file)
{
	RDAacct *holdacct=NULL;
	char *temp2=NULL,MFstemp[101];
	Wt::WLineEdit *LE=NULL;
	Wt::WRegExpValidator *REv=NULL;

	if(member->definition!=NULL) Rfree(member->definition);
	switch(member->field_type)
	{
		case EXPENDITURE:
			if(!member->rtype) 
			{
				member->items=acctype;
			}
			if(EXPENDITURE_ACCOUNT!=NULL)
			{
				if(EXPENDITURE_ACCOUNT->num>member->items)
				{
					holdacct=EXPENDITURE_ACCOUNT->codetype+member->items;
				}
			}
			if(holdacct!=NULL)
			{
				member->definition=fullacctdef(holdacct);
			}
			break;
		case REVENUE:
			if(!member->rtype) 
			{
				member->items=acctype;
			}
			if(REVENUE_ACCOUNT->num>member->items)
			{
				holdacct=REVENUE_ACCOUNT->codetype+member->items;
			}
			if(holdacct!=NULL)
			{
				member->definition=fullacctdef(holdacct);
			}
			break;
		case BALANCESHEET:
			if(!member->rtype) 
			{
				member->items=acctype;
			}
			if(BALANCE_SHEET_ACCOUNT->num>member->items)
			{
				holdacct=BALANCE_SHEET_ACCOUNT->codetype+member->items;
			}
			if(holdacct!=NULL)
			{
				member->definition=fullacctdef(holdacct);
			}
			break;
		case BEGINNINGBALANCE:
			if(!member->rtype) 
			{
				member->items=acctype;
			}
			if(BEGINNING_BALANCE_ACCOUNT->num>member->items)
			{
				holdacct=BEGINNING_BALANCE_ACCOUNT->codetype+member->items;
			}
			if(holdacct!=NULL)
			{
				member->definition=fullacctdef(holdacct);
			}
			break;
		case CUSTOMTYPE:
			member->rtype=acctype;
			if(CUSTOM_INPUTS_ACCOUNT->num>member->rtype)
			{
				holdacct=CUSTOM_INPUTS_ACCOUNT->codetype+member->rtype;
			}
			if(holdacct!=NULL)
			{
				member->definition=fullacctdef(holdacct);
			}
			break;
		default:
			return(-1);
	}
	if(member->w!=NULL)
	{
		memset(MFstemp,0,101);
		memset(MFstemp,'*',member->field_length);
		MFstemp[member->field_length]=0;
		if(RDAstrcmp(member->definition,MFstemp))
		{
			temp2=CreateRegExpString(member->definition);
			if(diaggui)
			{
				prterr("DIAG Resource [%s] Type [%s] Regex Created: [%s]",member->rscrname, standardfieldtypes[member->field_type], (temp2!=NULL ? temp2:""));
			}
			LE=(Wt::WLineEdit *)member->w;
			REv=(Wt::WRegExpValidator *)member->validobject;
			REv->~WRegExpValidator();
			member->validobject = new Wt::WRegExpValidator(temp2);
			if(member->validobject!=NULL) LE->setValidator(member->validobject);
		} else temp2=NULL;
		if(temp2!=NULL) Rfree(temp2);
	}
	updatemember(member);
	return(0);
}
short xFINDRSCSETDEFTYPE(RDArsrc *rsc,char *name,short acctype,
	int line,char *file)
{
	short x;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCSETDEFTYPE Setting Definition Type to [%d] for Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",acctype,(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			xMEMBERSETDEFTYPE(member,acctype,line,file);
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] on Screen [%s] [%s] has definition type [%d] at line [%d] program [%s].",name,rsc->module,rsc->screen,acctype,line,file);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			return(0);
		}
	}
	prterr("Error Resource [%s] not found on Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	return(-1); 
}
short xFINDRSCGETDEFTYPE(RDArsrc *rsc,char *name,short *deftype,int line,char *file)
{
	short x;
	RDArmem *member=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCGETDEFTYPE Getting Definition Type Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			switch(member->field_type)
			{
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
					*deftype=member->items;
					break;
				case CUSTOMTYPE:
					*deftype=member->rtype;
					break;
				default:
					prterr("Error Resource [%s] on Screen [%s] [%s] has the wrong field type [%d] [%s] to require FINDRSCGETDEFTYPE at line [%d] program [%s].",name,rsc->module,rsc->screen,member->field_type,standardfieldtypes[member->field_type],line,file);
					return(-1);
			}
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] on Screen [%s] [%s] has had definition type changed to [%d] at line [%d] program [%s].",name,rsc->module,rsc->screen,*deftype,line,file);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			return(0);
		}
	}
	prterr("Error Resource [%s] not found on Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	return(-1); 
}
short xMEMBERSETACCTTYPE(RDArmem *member,short acctype,int line,char *file)
{
	RDArsrc *rsc=NULL;
	RDAacct *holdacct=NULL;
	char *temp2=NULL;
	Wt::WLineEdit *LE=NULL;
	Wt::WRegExpValidator *REv=NULL;

	rsc=(RDArsrc *)member->parent;
	switch(member->field_type)
	{
		case EXPENDITURE:
		case REVENUE:
		case BALANCESHEET:
		case BEGINNINGBALANCE:
		case CUSTOMTYPE:
			switch(acctype)
			{
				case EXP_ACCT:
					member->field_type=EXPENDITURE;
					break;
				case REV_ACCT:
					member->field_type=REVENUE;
					break;
				case BAL_ACCT:
					member->field_type=BALANCESHEET;
					break;
				case BEG_ACCT:
					member->field_type=BEGINNINGBALANCE;
					break;
				case CUSTOM_ACCT:
					member->field_type=CUSTOMTYPE;
					break;
				default:
					return(-1);
			}
			break;
		default:
			return(-1);
	}
	if(member->definition!=NULL) Rfree(member->definition);
	switch(member->field_type)
	{
		case EXPENDITURE:
			if(EXPENDITURE_ACCOUNT->num>member->items)
			{
				holdacct=EXPENDITURE_ACCOUNT->codetype+member->items;
			}
			if(holdacct!=NULL)
			{
				member->definition=fullacctdef(holdacct);
			}
			break;
		case REVENUE:
			if(REVENUE_ACCOUNT->num>member->items)
			{
				holdacct=REVENUE_ACCOUNT->codetype+member->items;
			}
			if(holdacct!=NULL)
			{
				member->definition=fullacctdef(holdacct);
			}
			break;
		case BALANCESHEET:
			if(BALANCE_SHEET_ACCOUNT->num>member->items)
			{
				holdacct=BALANCE_SHEET_ACCOUNT->codetype+member->items;
			}
			if(holdacct!=NULL)
			{
				member->definition=fullacctdef(holdacct);
			}
			break;
		case BEGINNINGBALANCE:
			if(BEGINNING_BALANCE_ACCOUNT->num>member->items)
			{
				holdacct=BEGINNING_BALANCE_ACCOUNT->codetype+member->items;
			}
			if(holdacct!=NULL)
			{
				member->definition=fullacctdef(holdacct);
			}
			break;
		case CUSTOMTYPE:
			if(CUSTOM_INPUTS_ACCOUNT->num>member->items)
			{
				holdacct=CUSTOM_INPUTS_ACCOUNT->codetype+member->items;
			}
			if(holdacct!=NULL)
			{
				member->definition=fullacctdef(holdacct);
			}
			break;
		default:
			return(-1);
	}
	if(member->w!=NULL)
	{
		temp2=CreateRegExpString(member->definition);
		if(diaggui)
		{
			prterr("Regex Created: [%s]",temp2);
		}
		LE=(Wt::WLineEdit *)member->w;
		REv=(Wt::WRegExpValidator *)member->validobject;
		REv->~WRegExpValidator();
		member->validobject = new Wt::WRegExpValidator(temp2);
		if(member->validobject!=NULL) LE->setValidator(member->validobject);
		if(temp2!=NULL) Rfree(temp2);
	}
	updatemember(member);
	return(0);
}
short xFINDRSCSETACCTTYPE(RDArsrc *rsc,char *name,short acctype,
	int line,char *file)
{
	short x;
	RDArmem *member;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG FINDRSCSETACCTTYPE Setting Account Type to [%d] for Resource [%s] on Screen [%s] [%s] at line# [%d] program [%s].",acctype,(name!=NULL ? name:""),rsc->module,rsc->screen,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,member=rsc->rscs;x<rsc->numrscs;++x,++member)
	{
		if(!RDAstrcmp(member->rscrname,name))
		{
			xMEMBERSETACCTTYPE(member,acctype,line,file);
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Resource [%s] on Screen [%s] [%s] has account type [%d] at line [%d] program [%s].",name,rsc->module,rsc->screen,acctype,line,file);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			return(0);
		}
	}
	prterr("Error Resource [%s] not found on Screen [%s] [%s] at line# [%d] program [%s].",name,rsc->module,rsc->screen,line,file);
	return(-1); 
}
/*---------------------------------------------------------------------------
	getmenubin 	Function reads a menu item.
---------------------------------------------------------------------------*/
short xgetmenubin(char *libname,RDAmenu *menu,int line,char *file)
{
	short x;
	BIN *bin;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG getmenubin Attempting to Reading Menu Binary [%s] from Menu Library [%s] at line [%d] program [%s].",menu->name,libname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	bin=getlibbin(libname,menu->name,TRUE);
	if(bin==NULL) 
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diaggui || diaggui_field)
		{
			prterr("Error Attempt to Read Menu Binary [%s] from Menu Library [%s] FAILED at line [%d] program [%s].",menu->name,libname,line,file); 
		}
#endif /* USE_RDA_DIAGNOSTICS */
		return(-1);
	}
	x=BINgetshort(bin);
	if(x!=(-1701))
	{
		prterr("Error Invalid version or bad binary for Menu [%s] in Menu Binary Library [%s] at line [%d] program [%s].",menu->name,libname,line,file);
		BINfree(bin);
		return(-1);
	}
	if(menu->name!=NULL) Rfree(menu->name);
	menu->name=BINgetstring(bin);
	menu->command=BINgetstring(bin);
	BINfree(bin);
	return(0);
}
/*---------------------------------------------------------------------------
	writemenubin 	Function creates a bin file from menu structure.
---------------------------------------------------------------------------*/
short xwritemenubin(char *libname,RDAmenu *menu,int line,char *file)
{
	BIN *bin;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG writemenubin Writing Menu [%s] into Menu Binary Library [%s] at line [%d] program [%s].",(menu->name!=NULL ? menu->name:""),libname,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	bin=BINnew();
	BINaddshort(bin,-1701);	/*type identifier*/
	BINaddstring(bin,menu->name);
	BINaddstring(bin,menu->command);
	if(writelibbin(libname,bin,menu->name))
	{
		prterr("Error Can't write menu binary [%s] into menu library [%s] at line [%d] program [%s].",(menu->name!=NULL ? menu->name:""),libname,line,file);
		return(-1);
	}
	return(0);
}
RDAmenu *xRDAmenuNEW(char *name,char *command,int line,char *file)
{
	RDAmenu *tmp;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG RDAmenuNEW Creating a Menu Structure for [%s] command [%s] at line [%d] program [%s].",(name!=NULL ? name:""),(command!=NULL ? command:""),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(RDAmenu));
	if(name!=NULL)
	{
		tmp->name=stralloc(name);
	} else tmp->name=NULL;
	if(command!=NULL)
	{
		tmp->command=stralloc(command);
	} else tmp->command=NULL;
	return(tmp);
}
void xfree_menu(RDAmenu *menu,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG free_menu Freeing Menu Structure [%s] at line [%d] program [%s].",menu->name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(menu!=NULL)
	{
		if(menu->name!=NULL) Rfree(menu->name);
		if(menu->command!=NULL) Rfree(menu->command);
		Rfree(menu);
	}
}
char *GenericName(char *filename)
{
	if(RDAstrstr(filename,".pdf")) return("document.pdf");
	else if(RDAstrstr(filename,".xml")) return("data.xml");
	else if(RDAstrstr(filename,".png")) return("image.png");
	else if(RDAstrstr(filename,".jpg") || RDAstrstr(filename,".jpeg")) 
		return("image.jpeg");
	else if(RDAstrstr(filename,".gif")) return("image.gif");
	else if(RDAstrstr(filename,".htm") || RDAstrstr(filename,".html"))
		return("text.html");
	else if(RDAstrstr(filename,".js")) return("text.js");
	else if(RDAstrstr(filename,".css")) return("text.css");
	else if(RDAstrstr(filename,".oga") || RDAstrstr(filename,".ogg")) 
		return("audio.ogg");
	else if(RDAstrstr(filename,".ogv")) return("video.ogg");
	else if(RDAstrstr(filename,".swv")) 
		return("flash.swv");
	else if(RDAstrstr(filename,".mp4")) return("video.mp4");
	else if(RDAstrstr(filename,".mv4")) return("video.mp4");
	else if(RDAstrstr(filename,".mp3")) return("video.mp3");
	else if(RDAstrstr(filename,".svg")) return("imagexml.svg");
	else if(RDAstrstr(filename,".webm")) return("video.webm");
	else if(RDAstrstr(filename,".csv")) 
		return("delimited.csv");
	else if(RDAstrstr(filename,".tsv")) 
		return("delimited.tsv");
	else return("plain.txt");
}
char *DetermineMimeType(char *filename)
{
	if(RDAstrstr(filename,".pdf")) return("application/pdf");
	else if(RDAstrstr(filename,".xml")) return("application/xml");
	else if(RDAstrstr(filename,".png")) return("image/png");
	else if(RDAstrstr(filename,".jpg") || RDAstrstr(filename,".jpeg")) 
		return("image/jpeg");
	else if(RDAstrstr(filename,".gif")) return("image/gif");
	else if(RDAstrstr(filename,".htm") || RDAstrstr(filename,".html"))
		return("text/html");
	else if(RDAstrstr(filename,".js")) return("text/javascript");
	else if(RDAstrstr(filename,".css")) return("text/css");
	else if(RDAstrstr(filename,".oga") || RDAstrstr(filename,".ogg")) 
		return("audio/ogg");
	else if(RDAstrstr(filename,".ogv")) return("video/ogg");
	else if(RDAstrstr(filename,".swv")) 
		return("application/x-shockwave-flash");
	else if(RDAstrstr(filename,".mp4")) return("video/mp4");
	else if(RDAstrstr(filename,".mv4")) return("video/mp4");
	else if(RDAstrstr(filename,".mp3")) return("video/mp3");
	else if(RDAstrstr(filename,".svg")) return("image/svg+xml");
	else if(RDAstrstr(filename,".webm")) return("video/webm");
	else return("plain/txt");
}
void DisplayFile(char *filename)
{
	Wt::WFileResource *mf=NULL;
	char *f2=NULL,*extn=NULL;

	mf=new Wt::WFileResource(RDAMAINWIDGET);
	f2=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(filename)+4);
	sprintf(f2,"%s/%s",CURRENTDIRECTORY,filename);
	extn=DetermineMimeType(filename);
	mf->setMimeType(extn);
	mf->setFileName(f2);
	mf->setDispositionType(Wt::WResource::DispositionType::Attachment);
	mf->suggestFileName(filename);
	std::string url(mf->url());
	BlockingDialog d(url);
  	d.exec();
}
static char *strippathfromfile(char *filename)
{
	char *temp=NULL,*n=NULL;
	int x=0;

	x=RDAstrlen(filename);
	for(x=RDAstrlen(filename)-1;x>(-1);--x)
	{
		temp=filename+x;
		if(*temp=='/' || *temp=='\\') break;
	}
	if(x<0)
	{
		n=stralloc(filename);
	} else {
		
		n=Rmalloc(RDAstrlen(filename)-x+1);
		sprintf(n,"%s",&filename[x+1]);
	}
	return(n);
}
void DisplayRelativeFile(char *filename)
{
	Wt::WFileResource *mf=NULL;
	char *f2=NULL,*extn=NULL;
	char *name=NULL;

	if(isEMPTY(filename)) return;
	mf=new Wt::WFileResource(RDAMAINWIDGET);
	extn=DetermineMimeType(filename);
	mf->setMimeType(extn);
	mf->setFileName(filename);
	mf->setDispositionType(Wt::WResource::DispositionType::Attachment);
	name=strippathfromfile(filename);	
	mf->suggestFileName(name);
	if(name!=NULL) Rfree(name);
	std::string url(mf->url());
  	BlockingDialog d(url);
  	d.exec();
}
char *xUTF8famt(double a,int len,int line,char *file)
{
	char *r=NULL,*v=NULL,*temp=NULL;
	short l=0,z=0;

	if(diaggui)
	{
		prterr("DIAG UTF8famt value [%f] len [%d] at line [%d] program [%s].",a,len,line,file);
	}
	v=famt(a,len);
	l=RDAstrlen(v)*2+1;
	r=Rmalloc(l);
	memset(r,0,l);
	z=0;
	for(temp=v;*temp;++temp)
	{
		if(*temp==' ' || *temp=='$') 
		{
			r[z]=0xC2;
			r[z+1]=0xA0;
			z+=2;
		} else {
			r[z]=*temp;
			++z;
		}
	}	
	return(r);
}
