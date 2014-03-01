#ifndef GUIP_H
#define GUIP_H
#include <gui.hpp>
#include <misc.hpp>
#include <Wt/WAbstractArea>
#include <Wt/WAbstractItemDelegate>
#include <Wt/WAbstractItemModel>
#include <Wt/WAbstractItemView>
#include <Wt/WAbstractListModel>
#include <Wt/WAbstractProxyModel>
#include <Wt/WAbstractSpinBox>
#include <Wt/WAbstractTableModel>
#include <Wt/WAbstractToggleButton>
#include <Wt/WApplication>
#include <Wt/WAnchor>
#include <Wt/WBootstrapTheme>
#include <Wt/WBreak>
#include <Wt/WBorder>
#include <Wt/WBorderLayout>
#include <Wt/WBrush>
#include <Wt/WCalendar>
#include <Wt/WCheckBox>
#include <Wt/WCompositeWidget>
#include <Wt/WContainerWidget>
#include <Wt/WCssStyleSheet>
#include <Wt/WCssDecorationStyle>
#include <Wt/WDate>
#include <Wt/WDatePicker>
#include <Wt/WDateEdit>
#include <Wt/WDateValidator>
#include <Wt/WDefaultLayout>
#include <Wt/WDefaultLoadingIndicator>
#include <Wt/WDialog>
#include <Wt/WDoubleSpinBox>
#include <Wt/WDoubleValidator>
#include <Wt/WEnvironment>
#include <Wt/WEvent>
#include <Wt/WFileResource>
#include <Wt/WFileUpload>
#include <Wt/WFitLayout>
#include <Wt/WFlags>
#include <Wt/WFont>
#include <Wt/WFontMetrics>
#include <Wt/WFormWidget>
#include <Wt/WGoogleMap>
#include <Wt/WGridLayout>
#include <Wt/WGroupBox>
#include <Wt/WHBoxLayout>
#include <Wt/WHTML5Audio>
#include <Wt/WHTML5Video>
#include <Wt/WInPlaceEdit>
#include <Wt/WInteractWidget>
#include <Wt/WIntValidator>
#include <Wt/WImage>
#include <Wt/WJavaScript>
#include <Wt/WJavaScriptSlot>
#include <Wt/WLabel>
#include <Wt/WLayout>
#include <Wt/WLayoutItem>
#include <Wt/WLayoutItemImpl>
#include <Wt/WLength>
#include <Wt/WLineEdit>
#include <Wt/WLoadingIndicator>
#include <Wt/WLogger>
#include <Wt/WMemoryResource>
#include <Wt/WMenu>
#include <Wt/WMenuItem>
#include <Wt/WMessageBox>
#include <Wt/WMessageResourceBundle>
#include <Wt/WMessageResources>
#include <Wt/WModelIndex>
#include <Wt/WNavigationBar>
#include <Wt/WObject>
#include <Wt/WOverlayLoadingIndicator>
#include <Wt/WPaintDevice>
#include <Wt/WPaintedWidget>
#include <Wt/WPainter>
#include <Wt/WPanel>
#include <Wt/WPdfImage>
#include <Wt/WPen>
#include <Wt/WPoint>
#include <Wt/WPolygonArea>
#include <Wt/WPopupMenu>
#include <Wt/WPopupMenuItem>
#include <Wt/WProgressBar>
#include <Wt/WPushButton>
#include <Wt/WRadioButton>
#include <Wt/WRasterImage>
#include <Wt/WRectArea>
#include <Wt/WRectF>
#include <Wt/WRegExp>
#include <Wt/WRegExpValidator>
#include <Wt/WResource>
#include <Wt/WScrollArea>
#include <Wt/WScrollBar>
#include <Wt/WSelectionBox>
#include <Wt/WServer>
#include <Wt/WShadow>
#include <Wt/WSignal>
#include <Wt/WSignalMapper>
#include <Wt/WSlider>
#include <Wt/WSocketNotifier>
#include <Wt/WSound>
#include <Wt/WSpinBox>
#include <Wt/WStackedWidget>
#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>
#include <Wt/WStatelessSlot>
#include <Wt/WStreamResource>
#include <Wt/WString>
#include <Wt/WStringListModel>
#include <Wt/WStringUtil>
#include <Wt/WSubMenuItem>
#include <Wt/WSuggestionPopup>
#include <Wt/WSvgImage>
#include <Wt/WTable>
#include <Wt/WTableCell>
#include <Wt/WTableColumn>
#include <Wt/WTableRow>
#include <Wt/WTableView>
#include <Wt/WTabWidget>
#include <Wt/WTemplate>
#include <Wt/WText>
#include <Wt/WTextArea>
#include <Wt/WTextEdit>
#include <Wt/WTheme>
#include <Wt/WTime>
#include <Wt/WTimer>
#include <Wt/WTimerWidget>
#include <Wt/WToolBar>
#include <Wt/WTransform>
#include <Wt/WTree>
#include <Wt/WTreeNode>
#include <Wt/WTreeTable>
#include <Wt/WTreeTableNode>
#include <Wt/WTreeView>
#include <Wt/WValidationStatus>
#include <Wt/WValidator>
#include <Wt/WVBoxLayout>
#include <Wt/WVectorImage>
#include <Wt/WViewWidget>
#include <Wt/WVirtualImage>
#include <Wt/WVmlImage>
#include <Wt/WWidget>
#include <Wt/WWebWidget>
#include <Wt/WWidgetItem>

#include <Wt/Auth/Login>
#include <Wt/Auth/AuthWidget>
#include <Wt/Auth/AuthService>
#include <Wt/Auth/HashFunction>
#include <Wt/Auth/PasswordService>
#include <Wt/Auth/PasswordStrengthValidator>
#include <Wt/Auth/PasswordVerifier>
#include <Wt/Auth/GoogleService>
#include <Wt/Auth/Dbo/UserDatabase>
#include <Wt/Auth/Dbo/AuthInfo>

#include <Wt/Dbo/Session>
#include <Wt/Dbo/ptr>
#include <Wt/Dbo/Impl>
#include <Wt/Dbo/backend/Sqlite3>
#include <Wt/Dbo/Types>
#include <Wt/Dbo/WtSqlTraits>

#include <Wt/Render/WPdfRenderer>
#include <Wt/Render/WTextRenderer>
#include <Wt/Utils>

#ifdef _USE_GOOGLE_ANALYTICS_ 
extern std::string googleCmd;
#endif /* _USE_GOOGLE_ANALYTICS_  */

extern std::string openWindowCmd;
extern short USER_INTERFACE;
extern char INSIDE_PASTE;
extern char using_js,using_ajax;
extern char inside_rfkw;
extern RDArsrc *rfkw_rsrc;

void crtwdgts(Wt::WWidget *,RDAscrn *,RDArsrc *,char *);
void makefield(Wt::WWidget *,RDArmem *,char *,char *,short,short,int,Wt::WTable *,int,int,int,int);
#define setrscnum(a,b,c)	xsetrscnum(a,b,c,__LINE__,__FILE__)
void xsetrscnum(RDArsrc *,short (*)(...),void *,int,char *);

WImage *RDA_Read_Xpm_To_QPixmap(char *);
extern char GUIstemp[512];

/* CALLBACKS */
short makeacct(char *,char *,char *,int,char*,unsigned,short);
short isacctstr(char *,char *,int,short);
#define crtline(a,b,c,d,e)	xcrtline(a,b,c,d,e,__LINE__,__FILE__)
int xcrtline(Wt::WWidget *,RDAscrn *,RDArsrc *rsrc,int *,Wt::WWidget **,int,char *);
#define crtbox(a,b,c,d,e)	xcrtbox(a,b,c,d,e,__LINE__,__FILE__)
int xcrtbox(Wt::WWidget *,RDAscrn *,RDArsrc *,int *,Wt::WWidget **,int,char *);
#define crttab(a,b,c,d,e)	xcrttab(a,b,c,d,e,__LINE__,__FILE__)
int xcrttab(Wt::WWidget *,RDAscrn *,RDArsrc *,int *,Wt::WWidget **,int,char *);
#define crtpopup(a,b,c,d,e,f)	xcrtpopup(a,b,c,d,e,f,__LINE__,__FILE__)
int xcrtpopup(Wt::WWidget *,RDAscrn *,RDArsrc *,int *,Wt::WWidget **,short,int,char *);
#define crttoolbarcontainer(a,b,c,d,e)	xcrttoolbarcontainer(a,b,c,d,e,__LINE__,__FILE__)
int xcrttoolbarcontainer(Wt::WWidget *,RDAscrn *,RDArsrc *,int *,Wt::WWidget **,int,char *);
#define crttablecontainer(a,b,c,d,e,f)	xcrttablecontainer(a,b,c,d,e,f,__LINE__,__FILE__)
int xcrttablecontainer(Wt::WWidget *,RDAscrn *,RDArsrc *,int *,Wt::WWidget **,int,int,char *);
#define crtheadercontainer(a,b,c,d,e,f)	xcrtheadercontainer(a,b,c,d,e,f,__LINE__,__FILE__)
void xcrtheadercontainer(Wt::WTable *,RDAscrn *,RDArsrc *,int *,Wt::WTableRow **,int,int,char *);
#define crtscrollwindow(a,b,c,d,e)	xcrtscrollwindow(a,b,c,d,e,__LINE__,__FILE__)
int xcrtscrollwindow(Wt::WWidget *,RDAscrn *,RDArsrc *,int *,Wt::WWidget **,int,char *);
#define numperline(a,b)	xnumperline(a,b,__LINE__,__FILE__)
int xnumperline(RDAscrn *,int,int,char *);
#define findmaxlines(a)	xfindmaxlines(a,__LINE__,__FILE__)
int xfindmaxlines(RDAscrn *,int,char *);
#define boxmaxcols(a,b)	xboxmaxcols(a,b,__LINE__,__FILE__)
int xboxmaxcols(RDAscrn *,int,int,char *);
#define boxmaxrows(a,b)	xboxmaxrows(a,b,__LINE__,__FILE__)
int xboxmaxrows(RDAscrn *,int,int,char *);
void guihelpfunction(RDArmem *);
void losingfocusfunction(RDArmem *);
void gainingfocusfunction(RDArmem *);
void activatefunction(RDArmem *);
void toggleactivatefunction(RDArmem *);
void runfunctioncallback(RDArmem *);
void runfunckwincallback(RDArmem *);
void runfuncexitcallback(RDArmem *);
void list_doubleclick_callback(RDArmem *);
void list_callback(RDArmem *);
#define ContainsWtHeader(a,b)	xContainsWtHeader(a,b,__LINE__,__FILE__)
int xContainsWtHeader(RDAscrn *,int,int,char *);
extern int GUI_CHILD_WINDOW_x,GUI_CHILD_WINDOW_y;
Wt::WApplication *createApplication(const WEnvironment&);
#define MergeInputStrings(a,b,c,d,e,f)	xMergeInputStrings(a,b,c,d,e,f,__LINE__,__FILE__)
char *xMergeInputStrings(char *,char *,char *,int,int,short,int,char *);
char *GUIstripdigits(char *);
char *GUIstripdigitsdecimal(char *);
char *FormattedValidator(char *,char *,short);

class OpenRDALoadingIndicator : public Wt::WContainerWidget, 
				public Wt::WLoadingIndicator
{
public:
	OpenRDALoadingIndicator();

	virtual Wt::WWidget *widget() 
	{ 	
		return this; 
	}
	virtual void setMessage(const Wt::WString& text);

private:
	Wt::WContainerWidget *cover_;
	Wt::WContainerWidget *center_;
	Wt::WText *text_;
};
class DiagnosticLoadingIndicator : public Wt::WContainerWidget, 
				public Wt::WLoadingIndicator
{
public:
	DiagnosticLoadingIndicator();

	virtual Wt::WWidget *widget() 
	{ 	
		return this; 
	}
	virtual void setMessage(const Wt::WString& text);
	Wt::WText *text_;
	Wt::WText *status_;
	Wt::WProgressBar *pbar_;
	Wt::WLineEdit *selected_;
	Wt::WLineEdit *read_;
	Wt::WLineEdit *total_;
	Wt::WLineEdit *start_time_;
	Wt::WLineEdit *current_time_;
	Wt::WLineEdit *elapsed_time_;
	Wt::WLineEdit *estimated_completion_;
	Wt::WLineEdit *estimated_duration_;
	Wt::WLineEdit *remaining_time_;
	Wt::WLineEdit *increment_;
	Wt::WPushButton *resume_;
	Wt::WPushButton *suspend_;
	Wt::WPushButton *quit_;
	Wt::WPushButton *help_;

private:
	Wt::WContainerWidget *cover_;
	Wt::WContainerWidget *center_;
};

extern OpenRDALoadingIndicator *myOpenRDALoadingIndicator;
extern DiagnosticLoadingIndicator *myDiagnosticLoadingIndicator;

#endif /* GUIP_H */
