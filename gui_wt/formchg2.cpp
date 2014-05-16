#include<guip.hpp>
#include<gui.hpp>

// define __USE_ANIMATIONS__


/*  Size of Browse Inner Viewport / Window in generic all browser context 

var w=window.innerWidth ||  document.documentElement.clientWidth || 
	document.body.clientWidth;

var h=window.innerHeight || document.documentElement.clientHeight ||
	document.body.clientHeight;

*/

#define _USING_OUR_CALLBACKS_
RDArmem *NoLosingFocus=NULL;
#define LIMIT_SCROLLING_SIZES
#define FLAT_DOCK_BUTTON

FILE *cssNames=NULL;
short PUSHBUTTON_STYLE=0;
char USE_BROWSELISTLABEL=FALSE;
char GUIstemp[1024];
#define USE_FGBG_COLOR
#define LOSING_FOCUS
#ifndef INT386
#define INSTALL_CALLBACKS
#endif
#ifdef RS6000
#endif
#ifdef SCO386
#endif
#ifdef SCO50
#endif
#ifdef SCO50P
#endif

#define _ANY_ACCTDIM_	"[a-zA-Z0-9._ ]"
#define _N_ACCTDIM_	"[0-9]"
#define _A_ACCTDIM_	"[a-zA-Z]"

char *RemoveSpaces(char *S)
{
	int x=0,y=0,len=0;
	char *temp=NULL,*temp1=NULL,*temp2=NULL;
	
	if(!isEMPTY(S))
	{
		temp=stralloc(S);
		len=RDAstrlen(temp);
		for(x=len-1;x>0;--x)
		{
			temp1=temp+x;
			if(*temp1==' ')
			{
				for(y=x;y<len-1;++y)
				{
					temp2=temp+(y+1);
					*temp1=*temp2;
					++temp1;
				}
				*temp2=0;
				--len;
			}
		}
	}
	return(temp);
}
char *InputFieldStyleClass(RDArmem *m)
{
	return(RemoveSpaces(m->rscrname));
}
char *ScreenStyleClass(RDArsrc *r)
{
	return(RemoveSpaces(r->screen));
}
char *ModuleScreenStyleClass(RDArsrc *r)
{
	char *n=NULL,*s=NULL;

	s=ScreenStyleClass(r);
	n=Rmalloc(RDAstrlen(r->module)+RDAstrlen(s)+3);
	sprintf(n,"%s %s",(r->module!=NULL ? r->module:""),(s!=NULL ? s:""));
	if(s!=NULL) Rfree(s);
	return(n);
}
char *CreateRegExpString(char *def)
{
	int x=0,len=0,count=0,spaces=0;;
	char *temp=NULL,last=0,*re=NULL,*which=NULL;

	if(isEMPTY(def)) return(NULL);
	temp=def;
	len=0;
	while(*temp)
	{
		if(last==0) last=*temp;
		else if((*temp!=' ') && last==*temp) ++len;
		else if((*temp!=' ') && last!=*temp)
		{
			if(last=='N') which=_N_ACCTDIM_;
			else if(last=='A') which=_A_ACCTDIM_;
			else which=_ANY_ACCTDIM_;
			if(re!=NULL)
			{
				re=Rrealloc(re,count+RDAstrlen(which)+5);
			} else {
				re=Rmalloc(RDAstrlen(which)+4);
			}
			if(count>0)
			{
				sprintf(&re[count],"-%s{%d}",which,len+1);
				count+=RDAstrlen(which)+4;	
			} else {
				sprintf(&re[count],"%s{%d}",which,len+1);
				count+=RDAstrlen(which)+3;
			}
			len=0;
			last=0;
		} else if(*temp==' ')
		{
			if(last!=0)
			{
			if(last=='N') which=_N_ACCTDIM_;
			else if(last=='A') which=_A_ACCTDIM_;
			else which=_ANY_ACCTDIM_;
			if(re!=NULL)
			{
				re=Rrealloc(re,count+RDAstrlen(which)+5);
			} else {
				re=Rmalloc(RDAstrlen(which)+4);
			}
			if(count>0)
			{
				sprintf(&re[count],"-%s{%d}",which,len+1);
				count+=RDAstrlen(which)+4;	
			} else {
				sprintf(&re[count],"%s{%d}",which,len+1);
				count+=RDAstrlen(which)+3;
			}
			last=0;
			len=0;
			}
			spaces=0;
			while(*temp==' ' && *temp)
			{
				++spaces;
				++temp;
			}
			if(re!=NULL)
			{
				re=Rrealloc(re,count+spaces+1);
			} else {
				re=Rmalloc(spaces+1);
			}
			if(count>0)
			{
				sprintf(&re[count],"%*s",spaces," ");
				count+=spaces;	
			} else {
				sprintf(&re[count],"%*s",spaces," ");
				count+=spaces;
			}
			spaces=0;
			last=0;
			len=0;
		}
		++temp;
	}
	if(last=='N') which=_N_ACCTDIM_;
	else if(last=='A') which=_A_ACCTDIM_;
	else which=_ANY_ACCTDIM_;
	if(re!=NULL)
	{
		re=Rrealloc(re,count+RDAstrlen(which)+5);
	} else {
		re=Rmalloc(RDAstrlen(which)+4);
	}
	if(count>0)
	{
		sprintf(&re[count],"-%s{%d}",which,len+1);
		count+=RDAstrlen(which)+4;	
	} else {
		sprintf(&re[count],"%s{%d}",which,len+1);
		count+=RDAstrlen(which)+3;
	}
	unpad(re);
	return(re);
}
Wt::WFileUpload *CreateFileUpload(RDArmem *member,Wt::WContainerWidget *c)
{
	Wt::WFileUpload *fu=NULL;
	Wt::WLineEdit *LE=(Wt::WLineEdit *)member->w;
	Wt::WProgressBar *pBar=NULL;
	Wt::WString text;
	std::string sfn;
	char xtemp[1024];

	fu=new Wt::WFileUpload(c);
	fu->setMultiple(FALSE);
	pBar=new Wt::WProgressBar();
	fu->setProgressBar(pBar);
	fu->setMargin(5,Wt::Right);
	if(member->rtype==4)
	{
/* prefer text */
/*
		std::string filter_string("*.csv,*.tsv,*.txt");
		fu->setFilters(filter_string);
*/
	} else if(member->rtype==5)
	{
/* any file */
	} else if(member->rtype==6)
	{
/* directory */
	} else if(member->rtype==7)
	{
/* new file */
	}

	fu->changed().connect(std::bind([=] () { 
		text=fu->clientFileName();
		if(fu->canUpload() && access(text.toUTF8().c_str(),R_OK))
		{
			fu->upload(); 
			text=fu->clientFileName();
			LE->setText(text);
			LE->setReadOnly(TRUE);
		} else {
			WMessageBox::show("Unable to Upload","Do not have permission to upload the selected file and it cannot exceed the size.",Ok|Cancel);
		}
	}));

	fu->uploaded().connect(std::bind([=]() {
		text=fu->clientFileName();
		LE->setText(text);
		pBar->setDisabled(TRUE);
		fu->stealSpooledFile();
		sfn=fu->spoolFileName();
		sfn=fu->spoolFileName();
		memset(xtemp,0,1024);
		sprintf(xtemp,"%s/%s",CURRENTDIRECTORY,text.toUTF8().c_str());
		AbsRDAMoveFile(sfn.c_str(),xtemp);
		LE->setReadOnly(FALSE);
		fu->setDisabled(FALSE);
		fu->refresh();
	}));
}
/*-------------------------------------------------------------------------
MAKEFIELD - function to create field widgets 
---------------------------------------------------------------------------*/
static void LPMactivatefunction(RDArmem *member)
{
#ifdef _USING_OUR_CALLBACKS_
	RDArsrc *rsrc;
	RDArmem *BSelect=NULL;
	int y=0;

	if(member->editable==FALSE || member->user_editable==FALSE) return;
	if(member->sensitive==FALSE || member->user_sensitive==FALSE) return;
	rsrc=member->parent;
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG activatefunction for Resource [%s] on Screen [%s] [%s].",member->rscrname,rsrc->module,rsrc->screen);
	}
#endif /* USE_RDA_DIAGNOSTICS */
#ifdef TIMEOUT_CURSORS
	TimeoutCursors(rsrc,TRUE);
#endif
	if(member->editable) 
	{
		if(RDA_CaptureAutoComplete!=NULL)
		{
			readmember(member);
			RDA_CaptureAutoComplete(rsrc->module,rsrc->screen,member->rscrname,member->value.string_value);
		}
		ExecuteRDArmemFunction(member);
	}
	y=FINDRSC(rsrc,"SELECT");
	if(y>(-1))
	{
		BSelect=rsrc->rscs+y;
		ExecuteRDArmemFunction(BSelect);
	}
	if(!isEMPTY(member->transversal_expression))
	{
		ComputeNewTransversalADV(rsrc,rsrc->EvalStr,rsrc->EvalStrArgs,rsrc->SubFunc,rsrc->SubFuncArgs,member->transversal_expression);
	}
#ifdef TIMEOUT_CURSORS
	TimeoutCursors(rsrc,FALSE);
#endif
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG exiting activatefunction for Resource [%s] on Screen [%s] [%s].",member->rscrname,rsrc->module,rsrc->screen);
	}
#endif /* USE_RDA_DIAGNOSTICS */
#endif /* _USING_OUR_CALLBACKS_ */
}
void checktexteditable_autocomplete(RDArmem *member)
{
#ifdef _USING_OUR_CALLBACKS_
	Wt::WString *text1=NULL,text2;
	std::string s1;
	Wt::WLineEdit *TE=(Wt::WLineEdit *)member->w;
	char *temp=NULL,*temp2=NULL;
	RDArsrc *parent=NULL;
	int p=0;

	if(member->app_update==TRUE) return;
	member->app_update=TRUE;
	if(member->editable && member->user_editable)
	{
		if(RDA_AutoComplete!=NULL)
		{
			parent=(RDArsrc *)member->parent;
			text2=TE->text();
			s1=text2.toUTF8();
			temp=s1.c_str();
			p=TE->cursorPosition();
			if(RDAstrlen(temp)>(p)) temp[p]=0;
			temp2=RDA_AutoComplete(parent->module,parent->screen,member->rscrname,temp);
			if(!isEMPTY(temp2))
			{
				text1=new Wt::WString(temp2,UTF8);
			} else {
				text1=new Wt::WString(temp,UTF8);
			}
			TE->setText(*text1);
			text1->~WString();
			if(temp2!=NULL) Rfree(temp2);
		}
	} else {
		text1=new Wt::WString(member->value.string_value,UTF8);
		TE->setText(*text1);
		text1->~WString();
	}
	member->app_update=FALSE;
#endif /* _USING_OUR_CALLBACKS_ */
}
void checktexteditable(RDArmem *member)
{
#ifdef _USING_OUR_CALLBACKS_
	Wt::WString *text1=NULL;
	Wt::WLineEdit *TE=(Wt::WLineEdit *)member->w;

	if(member->app_update==TRUE) return;
	member->app_update=TRUE;
	if(member->editable && member->user_editable)
	{
	} else {
		text1 = new Wt::WString(member->value.string_value,UTF8);
		TE->setText(*text1);
		text1->~WString();
	}
	member->app_update=FALSE;
#endif /* _USING_OUR_CALLBACKS_ */
}
void scrolledtext_length(RDArmem *member)
{
#ifdef _USING_OUR_CALLBACKS_
	Wt::WString text,*text1=NULL;
	Wt::WTextEdit *TE=NULL;
	Wt::WTextArea *TA=NULL;
	std::string s1;
	char *t=NULL;
	int length=0,p=0;

	if(member->app_update==TRUE) return;
	member->app_update=TRUE;
	switch(member->rtype)
	{
		case 0:
		case 1:
			TA=(Wt::WTextArea *)member->w;
			break;
		case 2:	
			TE=(Wt::WTextEdit *)member->w;
			break;
	}
	if(member->editable && member->user_editable)
	{
		NoLosingFocus=member;
		switch(member->rtype)
		{
			case 0:
			case 1:
				p=TA->cursorPosition();
				text=TA->text();
				break;
			case 2:	
				p=TE->cursorPosition();
				text=TE->text();
				break;
		}
		s1=text.toUTF8();
		if(s1.length()>member->field_length && member->field_length>0) 
		{
			s1.erase(member->field_length,s1.length()-member->field_length);
			text1 = new Wt::WString(s1,UTF8);	
			switch(member->rtype)
			{
				case 0:
				case 1:
					TA->setText(*text1);
					break;
				case 2:	
					TE->setText(*text1);
					break;
			}
			text1->~WString();
		} 
	} else {
		text1=new Wt::WString(member->value.string_value,UTF8);
		switch(member->rtype)
		{
			case 0:
			case 1:
				TA->setText(*text1);
				break;
			case 2:	
				TE->setText(*text1);
				break;
		}
		text1->~WString();
	}
	member->app_update=FALSE;
#endif /* _USING_OUR_CALLBACKS_ */
}
void makefield(Wt::WWidget *parent,RDArmem *member,
	char *label,char *XHTML_Label,char *pixmap,short rows,short cols,int rtype,
	Wt::WTable *Parent_Table,int table_row,int table_col,
	int table_row_span,int table_col_span)
{
	NumericField *NF=NULL;
	Wt::WWidget *WW=NULL;
	Wt::WFormWidget *wFormW=NULL;
	Wt::WImage *myImage=NULL;
	Wt::WLink *WK=NULL;
	Wt::WString *temp_xstr=NULL,*c=NULL;
	char *dashes,*rname=NULL,MFstemp[101];
	char *temp_str=NULL,*temp=NULL,*temp1=NULL,*temp2=NULL;
	RDArsrc *rsx=NULL;
	Wt::WContainerWidget *form1=NULL;
	Wt::WLayout *daL=NULL;
	Wt::WVBoxLayout *form=NULL;
	Wt::WSelectionBox *LB=NULL;
	Wt::WLineEdit *LE=NULL;
	Wt::WLineEdit *bLE=NULL;
	Wt::WDateEdit *DE=NULL;
	Wt::WDate wDate;
	Wt::WTextArea *TA=NULL;
	Wt::WTextEdit *TE=NULL;
	Wt::WCheckBox *wCB=NULL;
	Wt::WAbstractToggleButton *wATB=NULL;
	Wt::WPushButton *pB=NULL;
	Wt::WComboBox *cB=NULL;
	Wt::WTableColumn *tColumn=NULL;
	Wt::WValidator *Valid=NULL;
	Wt::WDoubleValidator *doubleValid=NULL;
	Wt::WFileUpload *wfu=NULL;
	Wt::WDateValidator *dateValid=NULL;
	Wt::WIntValidator *intValid=NULL;
	Wt::WRegExpValidator *regexValid=NULL;
	Wt::WTableCell *tCell=NULL;
	Wt::WProgressBar *progBar=NULL;
	Wt::WStandardItem *wSI=NULL;
	Wt::WStandardItemModel *wSIM=NULL;
	Wt::WLength wH,wL,spc;
	Wt::WCalendar *DE_Cal=NULL;
	Wt::WAnimation aMaze;
	Wt::WWebWidget *WWeb=NULL;
	RDArmem *browse_list_desc=NULL;
	Wt::WText *hold=NULL,*flatpB=NULL;
	int h=0,w=0,longest=0;
	int z=0,digs=0,iv=0;
	int Dm=0,Dd=0,Dy=0;
	RDAacct *holdacct=NULL;
	std::string *s1=NULL;
	double pMin=0.0,pMax=0.0,pVal=0.0;
	char *mssc=NULL,*fssc=NULL;
#ifdef __USE_ANIMATIONS__
	Wt::WAnimation fadeout(Wt::WAnimation::Fade,Wt::WAnimation::Linear,250);
	Wt::WAnimation fadein(Wt::WAnimation::Pop,Wt::WAnimation::Linear,250);
#endif /* __USE_ANIMATIONS__ */

	if(member==NULL)
	{
		prterr("ERROR Resource pointer RDArmem *member=NULL.");
		return;
	}
	rsx=member->parent;
	member->Table=Parent_Table;
	member->table_row=table_row;
	member->table_col=table_col;
	if(Parent_Table!=NULL)
	{
		tCell=Parent_Table->elementAt(table_row,table_col);
		member->tCell=tCell;
		tColumn=Parent_Table->columnAt(table_col);
	}
	member->rtype=rtype;
	if(!RDAstrcmp(member->rscrname,"PREVIOUS ADD's") && member->field_type==SCROLLEDLIST && rows==1) member->rows=2; 
	else member->rows=rows;
	member->cols=cols;
	if(!isEMPTY(XHTML_Label)) member->label=stralloc(XHTML_Label);
		else member->XHTML_Label=NULL;
	if(member->label==NULL && !isEMPTY(label)) member->label=stralloc(label);
	else if(member->label==NULL) member->label=stralloc(member->rscrname);
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG makefield Creating Resource [%s] for Screen [%s] [%s] Type [%s] label [%s] pixmap [%s] length [%d] rows [%d] cols [%d]",
			(member->rscrname!=NULL ? member->rscrname:""),
			rsx->module,rsx->screen,
			standardfieldtypes[member->field_type],
			(label!=NULL?label:""),
			(pixmap!=NULL?pixmap:""),
			member->field_length,rows,cols);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	member->w=NULL;
	temp=Rmalloc(RDAstrlen(rsx->module)+RDAstrlen(rsx->screen)+RDAstrlen(member->rscrname)+12);
	sprintf(temp,"%s-%s-%s",(rsx->module!=NULL ? rsx->module:""),
		(rsx->screen!=NULL ? rsx->screen:""),
		(member->rscrname!=NULL ? member->rscrname:""));
	dashes=adddashes(temp);
	if(temp!=NULL) Rfree(temp);
	s1 = new string(dashes);
	switch(member->field_type)
	{
		case VARIABLETEXT:
			if(!USER_INTERFACE)
			{
				if(member->rtype==4 || member->rtype==5 || member->rtype==6 || member->rtype==7)
				{
					if(Parent_Table!=NULL)
					{
						form1 = new Wt::WContainerWidget(tCell);
					} else {
						form1 = new Wt::WContainerWidget(parent);
					}
					daL=(Wt::WLayout *)form1;
					daL->setContentsMargins(0,0,0,0);
					LE=new Wt::WLineEdit();
					member->w=(Wt::WWidget *)LE;
					wfu=CreateFileUpload(member,form1);
					form1->addWidget(LE);
				} else {
					LE=new Wt::WLineEdit((Wt::WWidget *)parent);
				}
				WW=(Wt::WWidget *)LE;
				mssc=ModuleScreenStyleClass(rsx);
				fssc=InputFieldStyleClass(member);
				memset(GUIstemp,0,1024);
				sprintf(GUIstemp,"OpenRDA %s %s VariableText",mssc,fssc);
				WW->addStyleClass(GUIstemp);
				if(mssc!=NULL) Rfree(mssc);
				if(fssc!=NULL) Rfree(fssc);
				member->w=(Wt::WWidget *)LE;
				if(Parent_Table!=NULL)
				{
					tCell->addWidget((Wt::WWidget *)member->w);
				}
				wFormW=(Wt::WFormWidget *)LE;
				if(member->field_length!=0)
				{
					if(cols<=0)
					{	
						cols=member->field_length;
					}
					LE->setMaxLength(member->field_length);
				}
				if(cols>0)
				{
					h=cols+2;
				} else h=22;
				LE->setTextSize(h);
				
				
				if(Parent_Table!=NULL)
				{
					tColumn->setWidth(h);
				}
				if(member->rtype==3) /* Password */
				{
					LE->setEchoMode(1);
				} else {
					LE->setEchoMode(0);
				}
/*
				member->validobject = new Wt::WRegExpValidator("[A-Za-z0-9_!@#$%^&*()-+=/<>.,?~:;\[\]{}|]{*}");
*/
				member->validobject = new Wt::WValidator((Wt::WObject *)member->w);
				if(member->validobject!=NULL) LE->setValidator(member->validobject);
				if(!isEMPTY(member->value.string_value))
				{
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Setting Resource [%s]'s Initial value [%s].",member->rscrname,member->value.string_value);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					temp_xstr = new Wt::WString(member->value.string_value,UTF8);
					LE->setText(*temp_xstr);
					temp_xstr->~WString();
					LE->validate();
				}
				if(!member->editable)
				{
					ADVMEMBERSETEDITABLE(member,FALSE,FALSE);
				} else if(!isEMPTY(member->editable_expression))
				{
					if(rsx->EvalFuncArgs!=NULL)
					{
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx,rsx->EvalFuncArgs);
/*lint +e746*/
					} else {
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx);
/*lint +e746*/
					}
					ADVMEMBERSETEDITABLE(member,member->user_editable,TRUE);
				}
				if(!RDAstrcmp(member->rscrname,"BROWSE LIST LPM"))
				{
					LE->keyWentUp().connect(boost::bind(&activatefunction,member));
					LE->enterPressed().connect(boost::bind(&LPMactivatefunction,member));
				} else {
					LE->keyWentUp().connect(boost::bind(&checktexteditable_autocomplete,member));
					LE->enterPressed().connect(boost::bind(&activatefunction,member));
				}
				wFormW->blurred().connect(boost::bind(&losingfocusfunction,member));
				wFormW->focussed().connect(boost::bind(&gainingfocusfunction,member));
			}
			break;
		case OPTIONALFIELDS:
		case PLAINTEXT:
			if(!USER_INTERFACE)
			{
				if(member->rtype==4 || member->rtype==5 || member->rtype==6 || member->rtype==7)
				{
					if(Parent_Table!=NULL)
					{
						form1 = new Wt::WContainerWidget(tCell);
					} else {
						form1 = new Wt::WContainerWidget(parent);
					}
					daL=(Wt::WLayout *)form1;
					daL->setContentsMargins(0,0,0,0);
					LE=new Wt::WLineEdit();
					member->w=(Wt::WWidget *)LE;
					wfu=CreateFileUpload(member,form1);
					form1->addWidget(LE);
				} else {
					LE=new Wt::WLineEdit((Wt::WWidget *)parent);
				}
				WW=(Wt::WWidget *)LE;
				member->w=(Wt::WWidget *)LE;
				if(Parent_Table!=NULL)
				{
					tCell->addWidget((Wt::WWidget *)member->w);
				}
				mssc=ModuleScreenStyleClass(rsx);
				fssc=InputFieldStyleClass(member);
				memset(GUIstemp,0,1024);
				if(member->field_type==PLAINTEXT)
				{
					sprintf(GUIstemp,"OpenRDA %s %s PlainText",mssc,fssc);
				} else {
					sprintf(GUIstemp,"OpenRDA %s %s OptionalFields",mssc,fssc);
				}
				WW->addStyleClass(GUIstemp);
				if(mssc!=NULL) Rfree(mssc);
				if(fssc!=NULL) Rfree(fssc);
				wFormW=(Wt::WFormWidget *)LE;
				if(Parent_Table!=NULL)
				{
					tCell->addWidget((Wt::WWidget *)member->w);
				}
				if(member->field_length!=0)
				{
					if(cols<=0)
					{	
						cols=member->field_length;
					}
					LE->setMaxLength(member->field_length);
				}
				if(cols>0)
				{
					h=cols+2;
				} else h=22;
				LE->setTextSize(h);
				
				if(member->rtype==3) /* Password */
				{
					LE->setEchoMode(1);
				} else {
					LE->setEchoMode(0);
				}
/*
				member->validobject = new Wt::WRegExpValidator("[A-Za-z0-9_!@#$%^&*()-+=/<>.,?~:;\[\]{}|]{*}");
*/
				member->validobject = new Wt::WValidator((Wt::WObject *)member->w);
				if(member->validobject!=NULL) LE->setValidator(member->validobject);
				if(!isEMPTY(member->value.string_value))
				{
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Setting Resource [%s]'s Initial value [%s].",member->rscrname,member->value.string_value);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					temp_xstr = new Wt::WString(member->value.string_value,UTF8);
					LE->setText(*temp_xstr);
					temp_xstr->~WString();
					LE->validate();
				}
				if(!member->editable)
				{
					ADVMEMBERSETEDITABLE(member,FALSE,FALSE);
				} else if(!isEMPTY(member->editable_expression))
				{
					if(rsx->EvalFuncArgs!=NULL)
					{
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx,rsx->EvalFuncArgs);
/*lint +e746*/
					} else {
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx);
/*lint +e746*/
					}
					ADVMEMBERSETEDITABLE(member,member->user_editable,TRUE);
				}
				if(!RDAstrcmp(member->rscrname,"BROWSE LIST LPM"))
				{
					LE->keyWentUp().connect(boost::bind(&activatefunction,member));
					LE->enterPressed().connect(boost::bind(&LPMactivatefunction,member));
				} else {
					LE->keyWentUp().connect(boost::bind(&checktexteditable_autocomplete,member));
					LE->enterPressed().connect(boost::bind(&activatefunction,member));
				}
				wFormW->blurred().connect(boost::bind(&losingfocusfunction,member));
				wFormW->focussed().connect(boost::bind(&gainingfocusfunction,member));
			}
			break;
		case BOOLNS:
			if(!USER_INTERFACE)
			{
				c = new Wt::WString(member->label);
				if(Parent_Table!=NULL)
				{
					wCB=new Wt::WCheckBox(*c,tCell);
				} else {
					wCB=new Wt::WCheckBox(*c,parent);	
				}
				member->w=(Wt::WWidget *)wCB;
				WW=(Wt::WWidget *)wCB;
				mssc=ModuleScreenStyleClass(rsx);
				fssc=InputFieldStyleClass(member);
				memset(GUIstemp,0,1024);
				sprintf(GUIstemp,"OpenRDA %s %s CheckBox",mssc,fssc);
				WW->addStyleClass(GUIstemp);
				if(mssc!=NULL) Rfree(mssc);
				if(fssc!=NULL) Rfree(fssc);
				c->~WString();
				wATB=(Wt::WAbstractToggleButton *)wCB;
				wL=Wt::WLength(RDAstrlen(member->label)+4);
				wH=member->w->minimumHeight();
				if(Parent_Table!=NULL)
				{
					tColumn->setWidth(RDAstrlen(member->label)+4);
				}
				if(*member->value.string_value==TRUE)
				{
					wCB->setChecked();
				} else wCB->setUnChecked();	

				if(rtype==1)
				{
				}
				if(!member->editable)
				{
					ADVMEMBERSETEDITABLE(member,FALSE,FALSE);
				} else if(!isEMPTY(member->editable_expression))
				{
					if(rsx->EvalFuncArgs!=NULL)
					{
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx,rsx->EvalFuncArgs);
/*lint +e746*/
					} else {
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx);
/*lint +e746*/
					}
					ADVMEMBERSETEDITABLE(member,member->user_editable,TRUE);
				}
				wCB->checked().connect(boost::bind(&toggleactivatefunction,member));
				wCB->unChecked().connect(boost::bind(&toggleactivatefunction,member));
			}
			break;
		case CHARACTERS:
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG Setting Resource [%s]'s Initial value [%c].",member->rscrname,*member->value.string_value);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			sprintf(stemp,"%c",*member->value.string_value);
			if(!USER_INTERFACE)
			{
				LE=new Wt::WLineEdit((Wt::WWidget *)parent);
				WW=(Wt::WWidget *)LE;
				mssc=ModuleScreenStyleClass(rsx);
				fssc=InputFieldStyleClass(member);
				memset(GUIstemp,0,1024);
				sprintf(GUIstemp,"OpenRDA %s %s Characters",mssc,fssc);
				WW->addStyleClass(GUIstemp);
				if(mssc!=NULL) Rfree(mssc);
				if(fssc!=NULL) Rfree(fssc);
				member->w=(Wt::WWidget *)LE;
				wFormW=(Wt::WFormWidget *)LE;
				if(Parent_Table!=NULL)
				{
					tCell->addWidget((Wt::WWidget *)member->w);
				}
				member->field_length=1;
				cols=1;
				LE->setTextSize(cols);
				LE->setMaxLength(member->field_length);
				
				if(Parent_Table!=NULL)
				{
					tColumn->setWidth(cols);
				}
				if(member->rtype==3) /* Password */
				{
					LE->setEchoMode(1);
				} else {
					LE->setEchoMode(0);
				}
				member->validobject = new Wt::WRegExpValidator("[A-Za-z0-9_ !@#$%^&*()-+=/<>.?~:;[]{}|]*");
				if(member->validobject!=NULL) LE->setValidator(member->validobject);
				if(!isEMPTY(member->value.string_value))
				{
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Setting Resource [%s]'s Initial value [%s].",member->rscrname,member->value.string_value);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					temp_xstr = new Wt::WString(stemp,UTF8);
					LE->setText(*temp_xstr);
					temp_xstr->~WString();
					LE->validate();
				}
				if(!member->editable)
				{
					ADVMEMBERSETEDITABLE(member,FALSE,FALSE);
				} else if(!isEMPTY(member->editable_expression))
				{
					if(rsx->EvalFuncArgs!=NULL)
					{
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx,rsx->EvalFuncArgs);
/*lint +e746*/
					} else {
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx);
/*lint +e746*/
					}
					ADVMEMBERSETEDITABLE(member,member->user_editable,TRUE);
				}
				LE->keyWentUp().connect(boost::bind(&activatefunction,member));
				LE->enterPressed().connect(boost::bind(&activatefunction,member));
				wFormW->blurred().connect(boost::bind(&losingfocusfunction,member));
				wFormW->focussed().connect(boost::bind(&gainingfocusfunction,member));
			}
			break;
		case PHONE:
		case SOCSECNUM:
		case ZIPCODE:
			if(!USER_INTERFACE)
			{
				LE=new Wt::WLineEdit((Wt::WWidget *)parent);
				WW=(Wt::WWidget *)LE;
				member->w=(Wt::WWidget *)LE;
				mssc=ModuleScreenStyleClass(rsx);
				fssc=InputFieldStyleClass(member);
				memset(GUIstemp,0,1024);
				if(member->field_type==PHONE)
				{
					sprintf(GUIstemp,"OpenRDA %s %s PhoneNumber",mssc,fssc);
				} else if(member->field_type==SOCSECNUM)
				{
					sprintf(GUIstemp,"OpenRDA %s %s SocialSecurityNumber",mssc,fssc);
				} else {
					sprintf(GUIstemp,"OpenRDA %s %s ZipCode",mssc,fssc);
				}
				WW->addStyleClass(GUIstemp);
				if(mssc!=NULL) Rfree(mssc);
				if(fssc!=NULL) Rfree(fssc);
				wFormW=(Wt::WFormWidget *)LE;
				if(Parent_Table!=NULL)
				{
					tCell->addWidget((Wt::WWidget *)member->w);
				}
				if(member->field_length!=0)
				{
					if(cols<=0)
					{	
						cols=member->field_length;
					}
					LE->setMaxLength(member->field_length);
				}
				if(cols>0)
				{
					h=cols+2;
				} else h=22;
				LE->setTextSize(h);
				
				if(Parent_Table!=NULL)
				{
					tColumn->setWidth(h);
				}
				if(member->rtype==3) /* Password */
				{
					LE->setEchoMode(1);
				} else {
					LE->setEchoMode(0);
				}
				memset(stemp,0,101);
				c=NULL;
				if(member->field_type==PHONE) 
				{
					member->definition=stralloc("nnn-nnn-nnnn");	
					member->validobject = new Wt::WRegExpValidator("[0-9]{3}-[0-9]{3}-[0-9]{4}");
					sprintf(stemp,"999-999-9999");
					c=new Wt::WString(stemp);
					LE->setInputMask(*c);
				} else if(member->field_type==SOCSECNUM)
				{
					member->definition=stralloc("nnn-nn-nnnn");	
					member->validobject = new Wt::WRegExpValidator("[0-9]{3}-[0-9]{2}-[0-9]{4}");
					sprintf(stemp,"999-99-9999");
					c=new Wt::WString(stemp);
					LE->setInputMask(*c);
				} else if(member->field_type==ZIPCODE)
				{
					member->definition=stralloc("nnnnn-nnnn");	
					member->validobject = new Wt::WRegExpValidator("([0-9]{5})|([0-9]{5}-[0-9]{4})");
					sprintf(stemp,"99999-9999");
					c=new Wt::WString(stemp);
					LE->setInputMask(*c);
				}
				if(c!=NULL) c->~WString();

				if(member->validobject!=NULL) LE->setValidator(member->validobject);
				if(!isEMPTY(member->value.string_value))
				{
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Setting Resource [%s]'s Initial value [%s].",member->rscrname,member->value.string_value);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					temp_xstr = new Wt::WString(member->value.string_value,UTF8);
					LE->setText(*temp_xstr);
					temp_xstr->~WString();
					LE->validate();
				}
				if(!member->editable)
				{
					ADVMEMBERSETEDITABLE(member,FALSE,FALSE);
				} else if(!isEMPTY(member->editable_expression))
				{
					if(rsx->EvalFuncArgs!=NULL)
					{
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx,rsx->EvalFuncArgs);
/*lint +e746*/
					} else {
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx);
/*lint +e746*/
					}
					ADVMEMBERSETEDITABLE(member,member->user_editable,TRUE);
				}
/*
				LE->keyPressed().connect(std::bind([=] (const Wt::WKeyEvent& e) { formattedstring_callback(member,e); }, std::placeholders::_1));
				LE->keyWentUp().connect(std::bind([=] (const Wt::WKeyEvent& e) { formattedstring_callbackKeyUp(member,e); }, std::placeholders::_1));
*/
				LE->enterPressed().connect(boost::bind(&activatefunction,member));
				wFormW->blurred().connect(boost::bind(&losingfocusfunction,member));
				wFormW->focussed().connect(boost::bind(&gainingfocusfunction,member));
			}
			break;
		case DATES:
			if(!USER_INTERFACE)
			{
				if(member->rtype==0)
				{
					LE=new Wt::WLineEdit((Wt::WWidget *)parent);
					WW=(Wt::WWidget *)LE;
					member->w=(Wt::WWidget *)LE;
					mssc=ModuleScreenStyleClass(rsx);
					fssc=InputFieldStyleClass(member);
					memset(GUIstemp,0,1024);
					sprintf(GUIstemp,"OpenRDA %s %s Date",mssc,fssc);
					WW->addStyleClass(GUIstemp);
					if(mssc!=NULL) Rfree(mssc);
					if(fssc!=NULL) Rfree(fssc);

					wFormW=(Wt::WFormWidget *)LE;
					if(Parent_Table!=NULL)
					{
						tCell->addWidget((Wt::WWidget *)member->w);
					}
					if(member->field_length!=0)
					{
						if(cols<=0)
						{	
							cols=member->field_length;
						}
						LE->setMaxLength(member->field_length);
					}
					if(cols>0)
					{
						h=cols+2;
					} else h=12;
					LE->setTextSize(h);
					
					if(Parent_Table!=NULL)
					{
						tColumn->setWidth(h);
					}
					if(member->rtype==3) /* Password */
					{
						LE->setEchoMode(1);
					} else {
						LE->setEchoMode(0);
					}
					member->definition=stralloc("nn/nn/nnnn");	
					memset(stemp,0,101);
					sprintf(stemp,"99/99/9999");
					c=new Wt::WString(stemp);
					LE->setInputMask(*c);
					c->~WString();
					member->validobject = new Wt::WDateValidator("MM/dd/yyyy");
/*
					member->validobject = new Wt::WRegExpValidator("([0-9]{2}/[0-9]{2}/[0-9]{4})");
*/
					if(member->validobject!=NULL) LE->setValidator(member->validobject);
					if(!isEMPTY(member->value.string_value))
					{
#ifdef USE_RDA_DIAGNOSTICS
						if(diaggui || diaggui_field)
						{
							prterr("DIAG Setting Resource [%s]'s Initial value [%s].",member->rscrname,member->value.string_value);
						}
#endif /* USE_RDA_DIAGNOSTICS */
						temp_xstr = new Wt::WString(member->value.string_value,UTF8);
						LE->setText(*temp_xstr);
						temp_xstr->~WString();
						LE->validate();
					}
					if(!member->editable)
					{
						ADVMEMBERSETEDITABLE(member,FALSE,FALSE);
					} else if(!isEMPTY(member->editable_expression))
					{
						if(rsx->EvalFuncArgs!=NULL)
						{
/*lint -e746*/
							member->user_editable=rsx->EvalFunc(member->editable_expression,rsx,rsx->EvalFuncArgs);
/*lint +e746*/
						} else {
/*lint -e746*/
							member->user_editable=rsx->EvalFunc(member->editable_expression,rsx);
/*lint +e746*/
						}
						ADVMEMBERSETEDITABLE(member,member->user_editable,TRUE);
					}
/*
					LE->keyPressed().connect(std::bind([=] (const Wt::WKeyEvent& e) { formattedstring_callback(member,e); }, std::placeholders::_1));
					LE->keyWentUp().connect(std::bind([=] (const Wt::WKeyEvent& e) { formattedstring_callbackKeyUp(member,e); }, std::placeholders::_1));
*/
					LE->enterPressed().connect(boost::bind(&activatefunction,member));
					wFormW->blurred().connect(boost::bind(&losingfocusfunction,member));
					wFormW->focussed().connect(boost::bind(&gainingfocusfunction,member));
				} else {
					DE=new Wt::WDateEdit((Wt::WWidget *)parent);
					LE=(Wt::WLineEdit *)DE;
					member->validobject = new Wt::WDateValidator("MM/dd/yyyy");
					if(member->validobject!=NULL) LE->setValidator(member->validobject);
					WW=(Wt::WWidget *)LE;
					mssc=ModuleScreenStyleClass(rsx);
					fssc=InputFieldStyleClass(member);
					memset(GUIstemp,0,1024);
					sprintf(GUIstemp,"OpenRDA %s %s DateEdit",mssc,fssc);
					WW->addStyleClass(GUIstemp);
					if(mssc!=NULL) Rfree(mssc);
					if(fssc!=NULL) Rfree(fssc);
					wFormW=(Wt::WFormWidget *)DE;
					member->w=(Wt::WWidget *)DE;
					DE->setFormat("MM/dd/yyyy");
					DE->setTextSize(10);
					DE->setMaxLength(10);
					DE_Cal=DE->calendar();
					DE_Cal->setFirstDayOfWeek(7);
/*
					aMaze =Wt::WAnimation(Wt::WAnimation::AnimationEffect::SlideInFromLeft | Wt::WAnimation::AnimationEffect::Fade,Wt::WAnimation::TimingFunction::EaseInOut,250);
					DE->setHidden(FALSE,aMaze);
*/
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
					if(!member->editable)
					{
						ADVMEMBERSETEDITABLE(member,FALSE,FALSE);
					} else if(!isEMPTY(member->editable_expression))
					{
						if(rsx->EvalFuncArgs!=NULL)
						{
/*lint -e746*/
							member->user_editable=rsx->EvalFunc(member->editable_expression,rsx,rsx->EvalFuncArgs);
/*lint +e746*/
						} else {
/*lint -e746*/
							member->user_editable=rsx->EvalFunc(member->editable_expression,rsx);
/*lint +e746*/
						}
						ADVMEMBERSETEDITABLE(member,member->user_editable,TRUE);
					}
/*
					LE->keyPressed().connect(std::bind([=] (const Wt::WKeyEvent& e) { formattedstring_callback(member,e); }, std::placeholders::_1));
					LE->keyWentUp().connect(std::bind([=] (const Wt::WKeyEvent& e) { formattedstring_callback(member,e); }, std::placeholders::_1));
*/
					LE->enterPressed().connect(boost::bind(&activatefunction,member));
					wFormW->blurred().connect(boost::bind(&losingfocusfunction,member));
					wFormW->focussed().connect(boost::bind(&gainingfocusfunction,member));
				}
			}
			break;
		case TIMES:
			if(!USER_INTERFACE)
			{
				if(member->rtype==0)
				{
					LE=new Wt::WLineEdit((Wt::WWidget *)parent);
					WW=(Wt::WWidget *)LE;
					member->w=(Wt::WWidget *)LE;
					mssc=ModuleScreenStyleClass(rsx);
					fssc=InputFieldStyleClass(member);
					memset(GUIstemp,0,1024);
					sprintf(GUIstemp,"OpenRDA %s %s Time",mssc,fssc);
					WW->addStyleClass(GUIstemp);
					if(mssc!=NULL) Rfree(mssc);
					if(fssc!=NULL) Rfree(fssc);
					wFormW=(Wt::WFormWidget *)LE;
					if(Parent_Table!=NULL)
					{
						tCell->addWidget((Wt::WWidget *)member->w);
					}
					if(member->field_length!=0)
					{
						if(cols<=0)
						{	
							cols=member->field_length;
						}
						LE->setMaxLength(member->field_length);
					}
					if(cols>0)
					{
						h=cols+2;
					} else h=10;
					LE->setTextSize(h);
					member->definition=stralloc("nn:nn:nn");	
					memset(stemp,0,101);
					sprintf(stemp,"00:00:00");
					c=new WString(stemp);
					LE->setInputMask(*c);
					c->~WString();
					member->validobject = new Wt::WRegExpValidator("[0-9]{2}:[0-9]{2}:[0-9]{2}");
					if(member->validobject!=NULL) LE->setValidator(member->validobject);
					
					if(Parent_Table!=NULL)
					{
						tColumn->setWidth(h);
					}
					if(member->rtype==3) /* Password */
					{
						LE->setEchoMode(1);
					} else {
						LE->setEchoMode(0);
					}
					if(!isEMPTY(member->value.string_value))
					{
#ifdef USE_RDA_DIAGNOSTICS
						if(diaggui || diaggui_field)
						{
							prterr("DIAG Setting Resource [%s]'s Initial value [%s].",member->rscrname,member->value.string_value);
						}
#endif /* USE_RDA_DIAGNOSTICS */
						temp_xstr = new Wt::WString(member->value.string_value,UTF8);
						LE->setText(*temp_xstr);
						temp_xstr->~WString();
						LE->validate();
					}
					if(!member->editable)
					{
						ADVMEMBERSETEDITABLE(member,FALSE,FALSE);
					} else if(!isEMPTY(member->editable_expression))
					{
						if(rsx->EvalFuncArgs!=NULL)
						{
/*lint -e746*/
							member->user_editable=rsx->EvalFunc(member->editable_expression,rsx,rsx->EvalFuncArgs);
/*lint +e746*/
						} else {
/*lint -e746*/
							member->user_editable=rsx->EvalFunc(member->editable_expression,rsx);
/*lint +e746*/
						}
						ADVMEMBERSETEDITABLE(member,member->user_editable,TRUE);
					}
/*
					LE->keyPressed().connect(std::bind([=] (const Wt::WKeyEvent& e) { formattedstring_callback(member,e); }, std::placeholders::_1));
					LE->keyWentUp().connect(std::bind([=] (const Wt::WKeyEvent& e) { formattedstring_callbackKeyUp(member,e); }, std::placeholders::_1));
*/
					LE->enterPressed().connect(boost::bind(&activatefunction,member));
					wFormW->blurred().connect(boost::bind(&losingfocusfunction,member));
					wFormW->focussed().connect(boost::bind(&gainingfocusfunction,member));
				} else { /* No Wt::WTimeEdit */
				}
			}
			break;
		case CUSTOMTYPE:
		case EXPENDITURE:
		case REVENUE:
		case BALANCESHEET:
		case BEGINNINGBALANCE: 
			if(!USER_INTERFACE)
			{
				if(member->field_type==CUSTOMTYPE)
				{
					member->items=rtype;
					if(CUSTOM_INPUTS_ACCOUNT->num>rtype) holdacct=CUSTOM_INPUTS_ACCOUNT->codetype+rtype;
				} else if(member->field_type==EXPENDITURE)
				{
					if(member->rtype) member->items=member->rtype-1;
					if(EXPENDITURE_ACCOUNT->num>member->items) holdacct=EXPENDITURE_ACCOUNT->codetype+member->items; 
				} else if(member->field_type==REVENUE)
				{
					if(member->rtype) member->items=member->rtype-1;  
					if(REVENUE_ACCOUNT->num>member->items) holdacct=REVENUE_ACCOUNT->codetype+member->items; 
				} else if(member->field_type==BALANCESHEET)
				{
					if(member->rtype) member->items=member->rtype-1;  
					if(BALANCE_SHEET_ACCOUNT->num>member->items) holdacct=BALANCE_SHEET_ACCOUNT->codetype+member->items; 
				} else if(member->field_type==BEGINNINGBALANCE)
				{
					if(member->rtype) member->items=member->rtype-1;  
					if(BEGINNING_BALANCE_ACCOUNT->num>member->items) holdacct=BEGINNING_BALANCE_ACCOUNT->codetype+member->items; 
				}
				if(holdacct!=NULL) member->definition=fullacctdef(holdacct);
				LE=new Wt::WLineEdit((Wt::WWidget *)parent);
				WW=(Wt::WWidget *)LE;
				member->w=(Wt::WWidget *)LE;
				if(!isEMPTY(member->definition))
				{
					for(temp1=member->definition;*temp1;++temp1) 
					{
						if(*temp1=='*') *temp1='x';
						if(*temp1=='X') *temp1='x';
						if(*temp1=='N') *temp1='n';
						if(*temp1=='A') *temp1='a';
					}
					memset(MFstemp,0,101);
					memset(MFstemp,'*',member->field_length);
					MFstemp[member->field_length]=0;
					if(RDAstrcmp(member->definition,MFstemp))
					{
						temp=CreateRegExpString(member->definition);
						if(diaggui)
						{
							prterr("DIAG Resource [%s] Type [%s] Regex Created: [%s]",member->rscrname, standardfieldtypes[member->field_type], (temp!=NULL ? temp:""));
						}
						c=new Wt::WString(member->definition);
						LE->setInputMask(*c);
						c->~WString();
					} else temp=NULL;
				}
				if(!isEMPTY(temp))
				{
					member->validobject = new Wt::WRegExpValidator(temp);
				} else {
					member->validobject = new Wt::WValidator();
				}
				mssc=ModuleScreenStyleClass(rsx);
				fssc=InputFieldStyleClass(member);
				memset(GUIstemp,0,1024);

				if(member->field_type==CUSTOMTYPE)
				{
					sprintf(GUIstemp,"OpenRDA %s %s CustomInputField",mssc,fssc);
				} else if(member->field_type==EXPENDITURE)
				{
					sprintf(GUIstemp,"OpenRDA %s %s ExpenditureSubsidiaryAccount",mssc,fssc);
				} else if(member->field_type==REVENUE)
				{
					sprintf(GUIstemp,"OpenRDA %s %s RevenueSubsidiaryAccount",mssc,fssc);
				} else if(member->field_type==BALANCESHEET)
				{
					sprintf(GUIstemp,"OpenRDA %s %s GeneralLedgerAccount",mssc,fssc);
				} else if(member->field_type==BEGINNINGBALANCE)
				{
					sprintf(GUIstemp,"OpenRDA %s %s BeginningBalanceAccount",mssc,fssc);
				}
				WW->addStyleClass(GUIstemp);
				if(mssc!=NULL) Rfree(mssc);
				if(fssc!=NULL) Rfree(fssc);
				wFormW=(Wt::WFormWidget *)LE;
				if(member->validobject!=NULL) LE->setValidator(member->validobject);
				if(temp!=NULL) Rfree(temp);
				if(Parent_Table!=NULL)
				{
					tCell->addWidget((Wt::WWidget *)member->w);
				}
				if(member->field_length!=0)
				{
					if(cols<=0)
					{	
						cols=member->field_length;
					}
					LE->setMaxLength(member->field_length);
				}
				if(cols>0)
				{
					h=cols+2;
				} else h=22;
				LE->setTextSize(h);
				
				if(Parent_Table!=NULL)
				{
					tColumn->setWidth(h);
				}
				if(member->rtype==3) /* Password */
				{
					LE->setEchoMode(1);
				} else {
					LE->setEchoMode(0);
				}
				if(!isEMPTY(member->value.string_value))
				{
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Setting Resource [%s]'s Initial value [%s].",member->rscrname,member->value.string_value);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					temp_xstr = new Wt::WString(member->value.string_value,UTF8);
					LE->setText(*temp_xstr);
					temp_xstr->~WString();
					if(member->validobject!=NULL) LE->validate();
				}
				if(!member->editable)
				{
					ADVMEMBERSETEDITABLE(member,FALSE,FALSE);
				} else if(!isEMPTY(member->editable_expression))
				{
					if(rsx->EvalFuncArgs!=NULL)
					{
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx,rsx->EvalFuncArgs);
/*lint +e746*/
					} else {
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx);
/*lint +e746*/
					}
					ADVMEMBERSETEDITABLE(member,member->user_editable,TRUE);
				}
				LE->keyWentUp().connect(boost::bind(&checktexteditable_autocomplete,member));
				LE->enterPressed().connect(boost::bind(&activatefunction,member));
				wFormW->blurred().connect(boost::bind(&losingfocusfunction,member));
				wFormW->focussed().connect(boost::bind(&gainingfocusfunction,member));
			}
			break;
		case DOLLARS_NOCENTS:
			if(!USER_INTERFACE)
			{
				NF=new NumericField((Wt::WWidget *)parent);
				LE=(Wt::WLineEdit *)NF;
				WW=(Wt::WWidget *)LE;
				mssc=ModuleScreenStyleClass(rsx);
				fssc=InputFieldStyleClass(member);
				memset(GUIstemp,0,1024);
				sprintf(GUIstemp,"OpenRDA %s %s WholeDollars",mssc,fssc);
				WW->addStyleClass(GUIstemp);
				if(mssc!=NULL) Rfree(mssc);
				if(fssc!=NULL) Rfree(fssc);
				member->w=(NumericField *)NF;
				wFormW=(Wt::WFormWidget *)NF;
				if(Parent_Table!=NULL)
				{
					tCell->addWidget((Wt::WWidget *)member->w);
				}
				memset(GUIstemp,0,1024);
				sprintf(GUIstemp,"%c",RDA_CURRENCY_SYMBOL);
				temp_xstr=new Wt::WString(GUIstemp);
				NF->setCurrencyInput(*temp_xstr,0," ");
				temp_xstr->~WString();
				if(member->field_length!=0)
				{
					if(cols<=0)
					{	
						cols=member->field_length;
					}
				}
				if(cols>0)
				{
					h=cols+2;
				} else h=22;
				digs=cols-2-(cols-2)/4;
				NF->setDigits(digs);
				NF->setDecimal(FALSE);
				NF->setAllowNegative(TRUE);
				NF->setValue((double)*member->value.float_value);
				LE->setTextSize(h);
				
				if(Parent_Table!=NULL)
				{
					tColumn->setWidth(h);
				}
				member->validobject = new Wt::WValidator();
				if(member->validobject!=NULL) LE->setValidator(member->validobject);
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Setting Resource [%s]'s Initial value [%s].",member->rscrname,member->value.string_value);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				if(member->validobject!=NULL) LE->validate();
				if(!member->editable)
				{
					ADVMEMBERSETEDITABLE(member,FALSE,FALSE);
				} else if(!isEMPTY(member->editable_expression))
				{
					if(rsx->EvalFuncArgs!=NULL)
					{
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx,rsx->EvalFuncArgs);
/*lint +e746*/
					} else {
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx);
/*lint +e746*/
					}
					ADVMEMBERSETEDITABLE(member,member->user_editable,TRUE);
				}
				LE->enterPressed().connect(boost::bind(&activatefunction,member));
				wFormW->blurred().connect(boost::bind(&losingfocusfunction,member));
				wFormW->focussed().connect(boost::bind(&gainingfocusfunction,member));
			}
			break;
		case DOLLARS:
			if(!USER_INTERFACE)
			{
				NF=new NumericField((Wt::WWidget *)parent);
				LE=(Wt::WLineEdit *)NF;	
				WW=(Wt::WWidget *)NF;
				mssc=ModuleScreenStyleClass(rsx);
				fssc=InputFieldStyleClass(member);
				memset(GUIstemp,0,1024);
				sprintf(GUIstemp,"OpenRDA %s %s Dollars",mssc,fssc);
				WW->addStyleClass(GUIstemp);
				if(mssc!=NULL) Rfree(mssc);
				if(fssc!=NULL) Rfree(fssc);
				member->w=(Wt::WWidget *)NF;
				wFormW=(Wt::WFormWidget *)LE;
				if(Parent_Table!=NULL)
				{
					tCell->addWidget((Wt::WWidget *)member->w);
				}
				memset(GUIstemp,0,1024);
				sprintf(GUIstemp,"%c",RDA_CURRENCY_SYMBOL);
				temp_xstr=new Wt::WString(GUIstemp);
				NF->setCurrencyInput(*temp_xstr,2," ");
				temp_xstr->~WString();
				if(member->field_length!=0)
				{
					if(cols<=0)
					{	
						cols=member->field_length;
					}
				}
				if(cols>0)
				{
					h=cols+2;
				} else h=22;
				digs=cols-2-(cols-2)/4;
				NF->setDigits(digs);
				NF->setDecimal(TRUE);
				NF->setAllowNegative(TRUE);
				NF->setValue((double)*member->value.float_value/100);
				LE->setTextSize(h);
				
				if(Parent_Table!=NULL)
				{
					tColumn->setWidth(h);
				}
				member->validobject = new Wt::WValidator();
				if(member->validobject!=NULL) LE->setValidator(member->validobject);
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Setting Resource [%s]'s Initial value [%s].",member->rscrname,member->value.string_value);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				if(member->validobject!=NULL) LE->validate();
				if(!member->editable)
				{
					ADVMEMBERSETEDITABLE(member,FALSE,FALSE);
				} else if(!isEMPTY(member->editable_expression))
				{
					if(rsx->EvalFuncArgs!=NULL)
					{
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx,rsx->EvalFuncArgs);
/*lint +e746*/
					} else {
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx);
/*lint +e746*/
					}
					ADVMEMBERSETEDITABLE(member,member->user_editable,TRUE);
				}
				LE->enterPressed().connect(boost::bind(&activatefunction,member));
				wFormW->blurred().connect(boost::bind(&losingfocusfunction,member));
				wFormW->focussed().connect(boost::bind(&gainingfocusfunction,member));
			}
			break;
		case DECIMALV:
			if(!USER_INTERFACE)
			{
				NF=new NumericField((Wt::WWidget *)parent);
				LE=(Wt::WLineEdit *)NF;	
				WW=(Wt::WWidget *)NF;
				mssc=ModuleScreenStyleClass(rsx);
				fssc=InputFieldStyleClass(member);
				memset(GUIstemp,0,1024);
				sprintf(GUIstemp,"OpenRDA %s %s UnsignedDecimal",mssc,fssc);
				WW->addStyleClass(GUIstemp);
				if(mssc!=NULL) Rfree(mssc);
				if(fssc!=NULL) Rfree(fssc);
				member->w=(Wt::WWidget *)NF;
				wFormW=(Wt::WFormWidget *)NF;
				if(member->field_length!=0)
				{
					if(cols<=0)
					{	
						cols=member->field_length;
					}
				}
				digs=cols;
				NF->setDigits(digs);
				NF->setDecimal(TRUE);
				NF->setAllowNegative(FALSE);
				NF->setValue((double)*member->value.float_value);
				if(Parent_Table!=NULL)
				{
					tCell->addWidget((Wt::WWidget *)member->w);
				}
				if(cols>0)
				{
					h=cols+2;
				} else h=22;
				LE->setTextSize(h);
				
				if(Parent_Table!=NULL)
				{
					tColumn->setWidth(h);
				}
				member->validobject = new Wt::WValidator((Wt::WObject *)member->w);
				if(member->validobject!=NULL) LE->setValidator(member->validobject);
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Setting Resource [%s]'s Initial value [%s].",member->rscrname,member->value.string_value);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				if(member->validobject!=NULL) LE->validate();
				if(!member->editable)
				{
					ADVMEMBERSETEDITABLE(member,FALSE,FALSE);
				} else if(!isEMPTY(member->editable_expression))
				{
					if(rsx->EvalFuncArgs!=NULL)
					{
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx,rsx->EvalFuncArgs);
/*lint +e746*/
					} else {
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx);
/*lint +e746*/
					}
					ADVMEMBERSETEDITABLE(member,member->user_editable,TRUE);
				}
				LE->enterPressed().connect(boost::bind(&activatefunction,member));
				wFormW->blurred().connect(boost::bind(&losingfocusfunction,member));
				wFormW->focussed().connect(boost::bind(&gainingfocusfunction,member));
			}
			break;
		case SDECIMALV:
			if(!USER_INTERFACE)
			{
				NF=new NumericField((Wt::WWidget *)parent);
				LE=(Wt::WLineEdit *)NF;	
				WW=(Wt::WWidget *)NF;
				mssc=ModuleScreenStyleClass(rsx);
				fssc=InputFieldStyleClass(member);
				memset(GUIstemp,0,1024);
				sprintf(GUIstemp,"OpenRDA %s %s SignedDecimal",mssc,fssc);
				WW->addStyleClass(GUIstemp);
				if(mssc!=NULL) Rfree(mssc);
				if(fssc!=NULL) Rfree(fssc);
				member->w=(Wt::WWidget *)LE;
				wFormW=(Wt::WFormWidget *)LE;
				if(Parent_Table!=NULL)
				{
					tCell->addWidget((Wt::WWidget *)member->w);
				}
				if(member->field_length!=0)
				{
					if(cols<=0)
					{	
						cols=member->field_length;
					}
				}
				if(cols>0)
				{
					h=cols+2;
				} else h=22;
				digs=cols;
				NF->setDigits(digs);
				NF->setDecimal(TRUE);
				NF->setAllowNegative(TRUE);
				NF->setValue((double)*member->value.float_value);
				LE->setTextSize(h);
				
				if(Parent_Table!=NULL)
				{
					tColumn->setWidth(h);
				}
				member->validobject = new Wt::WValidator((Wt::WObject *)member->w);
				if(member->validobject!=NULL) LE->setValidator(member->validobject);
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Setting Resource [%s]'s Initial value [%s].",member->rscrname,member->value.string_value);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				if(member->validobject!=NULL) LE->validate();
				if(!member->editable)
				{
					ADVMEMBERSETEDITABLE(member,FALSE,FALSE);
				} else if(!isEMPTY(member->editable_expression))
				{
					if(rsx->EvalFuncArgs!=NULL)
					{
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx,rsx->EvalFuncArgs);
/*lint +e746*/
					} else {
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx);
/*lint +e746*/
					}
					ADVMEMBERSETEDITABLE(member,member->user_editable,TRUE);
				}
				LE->enterPressed().connect(boost::bind(&activatefunction,member));
				wFormW->blurred().connect(boost::bind(&losingfocusfunction,member));
				wFormW->focussed().connect(boost::bind(&gainingfocusfunction,member));
			}
			break;
		case SDOUBLEV:
			if(!USER_INTERFACE)
			{
				NF=new NumericField((Wt::WWidget *)parent);
				LE=(Wt::WLineEdit *)NF;
				WW=(Wt::WWidget *)NF;
				mssc=ModuleScreenStyleClass(rsx);
				fssc=InputFieldStyleClass(member);
				memset(GUIstemp,0,1024);
				sprintf(GUIstemp,"OpenRDA %s %s SignedDouble",mssc,fssc);
				WW->addStyleClass(GUIstemp);
				if(mssc!=NULL) Rfree(mssc);
				if(fssc!=NULL) Rfree(fssc);
				member->w=(Wt::WWidget *)LE;
				wFormW=(Wt::WFormWidget *)LE;
				if(Parent_Table!=NULL)
				{
					tCell->addWidget((Wt::WWidget *)member->w);
				}
				if(member->field_length!=0)
				{
					if(cols<=0)
					{	
						cols=member->field_length;
					}
				}
				if(cols>0)
				{
					h=cols+2;
				} else h=22;
				digs=cols;
				NF->setDigits(digs);
				NF->setDecimal(FALSE);
				NF->setAllowNegative(TRUE);
				NF->setValue((double)*member->value.float_value);
				LE->setTextSize(h);
				
				if(Parent_Table!=NULL)
				{
					tColumn->setWidth(h);
				}
				member->validobject = new Wt::WValidator((Wt::WObject *)member->w);
				if(member->validobject!=NULL) LE->setValidator(member->validobject);
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Setting Resource [%s]'s Initial value [%s].",member->rscrname,member->value.string_value);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				if(member->validobject!=NULL) LE->validate();
				if(!member->editable)
				{
					ADVMEMBERSETEDITABLE(member,FALSE,FALSE);
				} else if(!isEMPTY(member->editable_expression))
				{
					if(rsx->EvalFuncArgs!=NULL)
					{
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx,rsx->EvalFuncArgs);
/*lint +e746*/
					} else {
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx);
/*lint +e746*/
					}
					ADVMEMBERSETEDITABLE(member,member->user_editable,TRUE);
				}
				LE->enterPressed().connect(boost::bind(&activatefunction,member));
				wFormW->blurred().connect(boost::bind(&losingfocusfunction,member));
				wFormW->focussed().connect(boost::bind(&gainingfocusfunction,member));
			}
			break;
		case DOUBLEV:
			if(!USER_INTERFACE)
			{
				NF=new NumericField((Wt::WWidget *)parent);
				LE=(Wt::WLineEdit *)NF;
				WW=(Wt::WWidget *)NF;
				mssc=ModuleScreenStyleClass(rsx);
				fssc=InputFieldStyleClass(member);
				memset(GUIstemp,0,1024);
				sprintf(GUIstemp,"OpenRDA %s %s UnsignedDouble",mssc,fssc);
				WW->addStyleClass(GUIstemp);
				if(mssc!=NULL) Rfree(mssc);
				if(fssc!=NULL) Rfree(fssc);
				member->w=(Wt::WWidget *)LE;
				wFormW=(Wt::WFormWidget *)LE;
				if(Parent_Table!=NULL)
				{
					tCell->addWidget((Wt::WWidget *)member->w);
				}
				if(member->field_length!=0)
				{
					if(cols<=0)
					{	
						cols=member->field_length;
					}
				}
				if(cols>0)
				{
					h=cols+2;
				} else h=22;
				digs=cols;
				NF->setDigits(digs);
				NF->setDecimal(FALSE);
				NF->setAllowNegative(FALSE);
				NF->setValue((double)*member->value.float_value);
				LE->setTextSize(h);
				
				if(Parent_Table!=NULL)
				{
					tColumn->setWidth(h);
				}
				member->validobject = new Wt::WValidator((Wt::WObject *)member->w);
				if(member->validobject!=NULL) LE->setValidator(member->validobject);
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Setting Resource [%s]'s Initial value [%s].",member->rscrname,member->value.string_value);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				if(member->validobject!=NULL) LE->validate();
				if(!member->editable)
				{
					ADVMEMBERSETEDITABLE(member,FALSE,FALSE);
				} else if(!isEMPTY(member->editable_expression))
				{
					if(rsx->EvalFuncArgs!=NULL)
					{
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx,rsx->EvalFuncArgs);
/*lint +e746*/
					} else {
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx);
/*lint +e746*/
					}
					ADVMEMBERSETEDITABLE(member,member->user_editable,TRUE);
				}
				LE->enterPressed().connect(boost::bind(&activatefunction,member));
				wFormW->blurred().connect(boost::bind(&losingfocusfunction,member));
				wFormW->focussed().connect(boost::bind(&gainingfocusfunction,member));
			}
			break;
		case SHORTV:
		case LONGV:
			if(!USER_INTERFACE)
			{
				NF=new NumericField((Wt::WWidget *)parent);
				LE=(Wt::WLineEdit *)NF;
				WW=(Wt::WWidget *)NF;
				mssc=ModuleScreenStyleClass(rsx);
				fssc=InputFieldStyleClass(member);
				memset(GUIstemp,0,1024);
				if(member->field_type==LONGV)
				{
					sprintf(GUIstemp,"OpenRDA %s %s UnsignedInteger",mssc,fssc);
				} else {
					sprintf(GUIstemp,"OpenRDA %s %s UnsignedShortInteger",mssc,fssc);
				}
				WW->addStyleClass(GUIstemp);
				if(mssc!=NULL) Rfree(mssc);
				if(fssc!=NULL) Rfree(fssc);
				member->w=(Wt::WWidget *)LE;
				wFormW=(Wt::WFormWidget *)LE;
				if(Parent_Table!=NULL)
				{
					tCell->addWidget((Wt::WWidget *)member->w);
				}
				if(member->field_length!=0)
				{
					if(cols<=0)
					{	
						cols=member->field_length;
					}
				}
				if(cols>0)
				{
					h=cols+2;
				} else h=22;
				digs=cols;
				NF->setDigits(digs);
				NF->setDecimal(FALSE);
				NF->setAllowNegative(FALSE);
				if(member->field_type==LONGV)
				{
					iv=*member->value.integer_value;
				} else {
					iv=*member->value.short_value;
				}
				NF->setValue((int)iv);
				LE->setTextSize(h);
				
				if(Parent_Table!=NULL)
				{
					tColumn->setWidth(h);
				}
				member->validobject = new Wt::WValidator((Wt::WObject *)member->w);
				if(member->validobject!=NULL) LE->setValidator(member->validobject);
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					if(member->field_type==LONGV)
					{
						prterr("DIAG Setting Resource [%s]'s Initial value [%d].",member->rscrname,*member->value.integer_value);
					} else {
						prterr("DIAG Setting Resource [%s]'s Initial value [%d].",member->rscrname,*member->value.short_value);
					}
				}
#endif /* USE_RDA_DIAGNOSTICS */
				if(member->validobject!=NULL) LE->validate();
				if(!member->editable)
				{
					ADVMEMBERSETEDITABLE(member,FALSE,FALSE);
				} else if(!isEMPTY(member->editable_expression))
				{
					if(rsx->EvalFuncArgs!=NULL)
					{
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx,rsx->EvalFuncArgs);
/*lint +e746*/
					} else {
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx);
/*lint +e746*/
					}
					ADVMEMBERSETEDITABLE(member,member->user_editable,TRUE);
				}
				LE->enterPressed().connect(boost::bind(&activatefunction,member));
				wFormW->blurred().connect(boost::bind(&losingfocusfunction,member));
				wFormW->focussed().connect(boost::bind(&gainingfocusfunction,member));
			}
			break;
		case SSHORTV:
		case SLONGV:
			if(!USER_INTERFACE)
			{
				NF=new NumericField((Wt::WWidget *)parent);
				LE=(Wt::WLineEdit *)NF;
				WW=(Wt::WWidget *)NF;
				mssc=ModuleScreenStyleClass(rsx);
				fssc=InputFieldStyleClass(member);
				memset(GUIstemp,0,1024);
				if(member->field_type==SLONGV)
				{
					sprintf(GUIstemp,"OpenRDA %s %s SignedInteger",mssc,fssc);
				} else {
					sprintf(GUIstemp,"OpenRDA %s %s SignedShortInteger",mssc,fssc);
				}
				WW->addStyleClass(GUIstemp);
				if(mssc!=NULL) Rfree(mssc);
				if(fssc!=NULL) Rfree(fssc);
				member->w=(Wt::WWidget *)LE;
				wFormW=(Wt::WFormWidget *)LE;
				if(Parent_Table!=NULL)
				{
					tCell->addWidget((Wt::WWidget *)member->w);
				}
				if(member->field_length!=0)
				{
					if(cols<=0)
					{	
						cols=member->field_length;
					}
				}
				if(cols>0)
				{
					h=cols+2;
				} else h=22;
				digs=cols;
				NF->setDigits(digs);
				NF->setDecimal(FALSE);
				NF->setAllowNegative(TRUE);
				if(member->field_type==LONGV)
				{
					iv=*member->value.integer_value;
				} else {
					iv=*member->value.short_value;
				}
				NF->setValue((int)iv);
				LE->setTextSize(h);
				
				if(Parent_Table!=NULL)
				{
					tColumn->setWidth(h);
				}
				member->validobject = new Wt::WValidator((Wt::WObject *)member->w);
				if(member->validobject!=NULL) LE->setValidator(member->validobject);
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					if(member->field_type==SLONGV)
					{
						prterr("DIAG Setting Resource [%s]'s Initial value [%d].",member->rscrname,*member->value.integer_value);
					} else {
						prterr("DIAG Setting Resource [%s]'s Initial value [%d].",member->rscrname,*member->value.short_value);
					}
				}
#endif /* USE_RDA_DIAGNOSTICS */
				if(member->validobject!=NULL) LE->validate();
				if(!member->editable)
				{
					ADVMEMBERSETEDITABLE(member,FALSE,FALSE);
				} else if(!isEMPTY(member->editable_expression))
				{
					if(rsx->EvalFuncArgs!=NULL)
					{
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx,rsx->EvalFuncArgs);
/*lint +e746*/
					} else {
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx);
/*lint +e746*/
					}
					ADVMEMBERSETEDITABLE(member,member->user_editable,TRUE);
				}
				LE->enterPressed().connect(boost::bind(&activatefunction,member));
				wFormW->blurred().connect(boost::bind(&losingfocusfunction,member));
				wFormW->focussed().connect(boost::bind(&gainingfocusfunction,member));
			}
			break;
		case BUTTONS:
			if(!USER_INTERFACE)
			{
				if(PUSHBUTTON_STYLE==2 && rtype>6 && rtype!=20 && rtype!=21 && rtype!=41 && rtype!=42 && rtype!=62 && rtype!=63) rtype=0;

				if(rtype==5)
				{
					if(isEMPTY(pixmap)) 
					{
						rtype=0;
						member->rtype=0;
					}
				} else if(rtype<=4)
				{	
					if(!RDAstrcmp(member->label,"FY..."))
					{
						rtype=61;
					} else if(!RDAstrcmp(member->label,"..."))
					{
						rtype=59;
					} else if((!strncmp(member->rscrname,"LOAD ",5)) && (!RDAstrstr(member->rscrname,"LOAD VALUE") && !RDAstrstr(member->rscrname,"LOAD CONTROL") && !RDAstrstr(member->rscrname,"LOAD OPERATOR") && !RDAstrstr(member->rscrname,"LOAD OPERAND") && !RDAstrstr(member->rscrname,"OUTPUT") && !RDAstrstr(member->rscrname,"DEVICE") && !RDAstrstr(member->rscrname,"LOAD GROUPER")) && (!RDAstrstr(rsx->screen,"MAINTAIN SCROLLED LIST")))
					{
						if(PUSHBUTTON_STYLE==2) rtype=42;
							else rtype=6;	
					} else if(!RDAstrcmp(member->rscrname,"REDO"))
					{
						rtype=8;	
					} else if(!RDAstrcmp(member->rscrname,"UNDO"))
					{
						rtype=9;	
					} else if((!RDAstrcmp(member->rscrname,"PRINT BROWSE") || !RDAstrcmp(member->rscrname,"PRINT")) && (!RDAstrstr(rsx->screen," ALIGNMENT TEST")))
					{
						rtype=7;	
					} else if(!RDAstrcmp(member->rscrname,"DEFINE LIST"))
					{
						rtype=15;	
					} else if(!RDAstrcmp(member->rscrname,"SEARCH BUTTON"))
					{
						rtype=12;	
					} else if((!RDAstrcmp(member->rscrname,"QUIT")) && !RDAstrstr(rsx->screen,"MENU") && ((FINDRSC(rsx,"MAIN MENU"))==(-1)))
					{
						if(PUSHBUTTON_STYLE<2) rtype=14;	
					} else if((!RDAstrcmp(member->rscrname,"HELP")) && !RDAstrstr(rsx->screen,"MENU") && ((FINDRSC(rsx,"MAIN MENU"))==(-1)))
					{
						if(PUSHBUTTON_STYLE<2) rtype=11;	
					} else if(!RDAstrcmp(member->rscrname,"BROWSE"))
					{
						if(PUSHBUTTON_STYLE<2) rtype=67;
					} else if(!RDAstrcmp(member->rscrname,"SELECT"))
					{
						if(RDAstrstr(rsx->screen," DEFINE LIST") && (PUSHBUTTON_STYLE<2)) rtype=15;
						else if(RDAstrstr(rsx->screen," SEARCH BROWSE") && (PUSHBUTTON_STYLE<2)) rtype=12;
						else if(RDAstrstr(rsx->screen,"IMPORT/EXPORT RANGE SCREEN") && (PUSHBUTTON_STYLE<2)) rtype=25;
						else if(PUSHBUTTON_STYLE<2) rtype=68;
					} else if(!RDAstrcmp(member->rscrname,"RUN REPORT"))
					{
						if(PUSHBUTTON_STYLE<2) rtype=25;
					} else if(!RDAstrcmp(member->rscrname,"SAVE") && RDAstrstr(rsx->screen,"MAINTAIN SCREEN"))
					{
						if(PUSHBUTTON_STYLE<2) rtype=54;	
					} else if((!RDAstrcmp(member->rscrname,"SAVE")) && (FINDRSC(rsx,"SAVE NO UPDATE")==(-1)))
					{
						if(PUSHBUTTON_STYLE<2) rtype=23;	
					} else if(!RDAstrcmp(member->rscrname,"SAVE"))
					{
						if(PUSHBUTTON_STYLE<2) rtype=23;	
					} else if(!RDAstrcmp(member->rscrname,"SAVE NO UPDATE"))
					{
						if(PUSHBUTTON_STYLE<2) rtype=23;	
					} else if(!RDAstrcmp(member->rscrname,"DEFAULTS"))
					{
						if(PUSHBUTTON_STYLE<2) rtype=17;	
					} else if(!RDAstrcmp(member->rscrname,"RESET DEFAULTS"))
					{
						if(PUSHBUTTON_STYLE<2) rtype=18;	
					} else if(!RDAstrcmp(member->rscrname,"ADD"))
					{
						if(PUSHBUTTON_STYLE<2) rtype=16;	
					} else if(!strncmp(member->rscrname,"ADD ABOVE",9))
					{
						rtype=57;	
					} else if(!strncmp(member->rscrname,"ADD BELOW",9))
					{
						rtype=58;	
					} else if(!strncmp(member->rscrname,"COPY ",5))
					{
						if(PUSHBUTTON_STYLE<2) rtype=49;	
					} else if(RDAstrstr(member->rscrname,"PRINT ERROR"))
					{
						rtype=55;	
					} else if(RDAstrstr(member->rscrname,"PRINT WARNING"))
					{
						rtype=56;	
					} else if(!RDAstrcmp(member->label,"URL")) 
					{
						rtype=32;
					}
				}
				if(PUSHBUTTON_STYLE==1 && rtype>5) rtype=0;
				if(!RDAstrcmp(member->label,"Clear (Search & LPM)"))
				{
					c=new Wt::WString("Clear (Search and LPM)");
				} else c=new Wt::WString(member->label);
				pB=NULL;
				myImage=NULL;
#ifdef USE_IMAGE_AS_PUSHBUTTON
				if(rtype==0)
				{
#endif /* USE_IMAGE_AS_PUSHBUTTON */
					if(Parent_Table!=NULL)
					{
						pB=new Wt::WPushButton((Wt::WContainerWidget *)tCell);
					} else {
						pB=new Wt::WPushButton((Wt::WContainerWidget *)parent);
					}
					member->w=(Wt::WWidget *)pB;
					WW=(Wt::WWidget *)pB;
					mssc=ModuleScreenStyleClass(rsx);
					fssc=InputFieldStyleClass(member);
					memset(GUIstemp,0,1024);
					sprintf(GUIstemp,"OpenRDA %s %s PushButton",mssc,fssc);
					WW->addStyleClass(GUIstemp);
					if(mssc!=NULL) Rfree(mssc);
					if(fssc!=NULL) Rfree(fssc);
					wFormW=(Wt::WFormWidget *)pB;
					pB->setDefault(FALSE);
#ifdef USE_IMAGE_AS_PUSHBUTTON
				} else {
					if(Parent_Table!=NULL)
					{
						myImage=new Wt::WImage((Wt::WContainerWidget *)tCell);
					} else {
						myImage=new Wt::WImage((Wt::WContainerWidget *)parent);
					}
					member->w=(Wt::WWidget *)myImage;
					WW=(Wt::WWidget *)myImage;
					mssc=ModuleScreenStyleClass(rsx);
					fssc=InputFieldStyleClass(member);
					memset(GUIstemp,0,1024);
					sprintf(GUIstemp,"OpenRDA %s %s ImageButton",mssc,fssc);
					WW->addStyleClass(GUIstemp);
					if(mssc!=NULL) Rfree(mssc);
					if(fssc!=NULL) Rfree(fssc);
					spc=Wt::WLength(72,Wt::WLength::Point);
					WW->setMaximumSize(spc,spc);
					wFormW=(Wt::WFormWidget *)myImage;
				}
#endif /* USE_IMAGE_AS_PUSHBUTTON */
				if(Parent_Table!=NULL)
				{	
					tCell->addWidget((Wt::WWidget *)member->w);
				}
				if(rtype==0) pB->setText(*c);
				if((rtype>=1 && rtype<=4))
				{
				}
				if((rtype>=1 && rtype<=4) || (rtype>=6 && rtype<=68))
				{
					if(!RDAstrcmp(member->label,"...") || !RDAstrcmp(member->label,"FY...") && isEMPTY(member->XHTML_Label))
					{
						if(c!=NULL) c->~WString();
						c=new Wt::WString(member->rscrname);
					}
					wFormW->setToolTip(*c,PlainText);
					if(c!=NULL) c->~WString();
				}
				rname=NULL;
				if((rtype>=1 && rtype<=4) || (rtype>=6 && rtype<=68))
				{
					switch(rtype)
					{
						default:
							rname=NULL;
							break;
						case 1: /* right */
							pB->addStyleClass("Right-Arrow");
							rname=stralloc("resources/OpenRDA/forward.png");
							break;
						case 2: /* left */
							pB->addStyleClass("Left-Arrow");
							rname=stralloc("resources/OpenRDA/back.png");
							break;
						case 3: /* up */
							pB->addStyleClass("Up-Arrow");
							rname=stralloc("resources/OpenRDA/up.png");
							break;
						case 4: /* down */
							pB->addStyleClass("Down-Arrow");
							rname=stralloc("resources/OpenRDA/down.png");
							break;
						case 6: /* find */
							pB->addStyleClass("Find");
							rname=stralloc("resources/OpenRDA/find.png");
							break;
						case 7: /* Printer */
							pB->addStyleClass("Printer");
							rname=stralloc("resources/OpenRDA/printer.png");
							break;
						case 8: /* Redo */
							pB->addStyleClass("Redo");
							rname=stralloc("resources/OpenRDA/redo.png");
							break;
						case 9: /* undo */
							pB->addStyleClass("Undo");
							rname=stralloc("resources/OpenRDA/undo.png");
							break;
						case 10: /* findfile */
							pB->addStyleClass("Find-File");
							rname=stralloc("resources/OpenRDA/filefind.png");
							break;
						case 11: /* help */
							pB->addStyleClass("Help");
							rname=stralloc("resources/OpenRDA/help.png");
							break;
						case 12: /* Search */
							if(!RDAstrstr(rsx->screen,"SEARCH BROWSE")) 
							{
								pB->addStyleClass("SearchButton");
								rname=stralloc("resources/OpenRDA/search.png");
							} else {
								pB->addStyleClass("SearchButton Color");
								rname=stralloc("resources/OpenRDA/search_color.png");
							}
							break;
						case 13: /* Edit */
							pB->addStyleClass("Edit");
							rname=stralloc("resources/OpenRDA/edit.png");
							break;
						case 14: /* Exit */
							pB->addStyleClass("Quit");
							rname=stralloc("resources/OpenRDA/exit.png");
							break;
						case 15: /* definelist */
							if(!RDAstrstr(rsx->screen,"DEFINE LIST")) 
							{
								pB->addStyleClass("DefineList");
								rname=stralloc("resources/OpenRDA/definelist.png");
							} else {
								pB->addStyleClass("DefineList Color");
								rname=stralloc("resources/OpenRDA/definelist_color.png");
							}
							break;
						case 16: /* add-create */
							pB->addStyleClass("Add");
							rname=stralloc("resources/OpenRDA/add.png");
							break;
						case 17: /* save defaults */
							pB->addStyleClass("SaveScreenDefaults");
							rname=stralloc("resources/OpenRDA/savedefaults.png");
							break;
						case 18: /* reset defaults */
							pB->addStyleClass("ResetScreenDefaults");
							rname=stralloc("resources/OpenRDA/resetdefaults.png");
							break;
						case 19: /* cabinet */
							pB->addStyleClass("FilingCabinet");
							rname=stralloc("resources/OpenRDA/cabinet.png");
							break;
						case 20: /* mail_send */
							pB->addStyleClass("SendEmail");
							rname=stralloc("resources/OpenRDA/mail_send.png");
							break;
						case 21: /* scanner */
							pB->addStyleClass("Scanner");
							rname=stralloc("resources/OpenRDA/scanner.png");
							break;
						case 22: /* wizard */
							pB->addStyleClass("Wizard");
							rname=stralloc("resources/OpenRDA/wizard.png");
							break;
						case 23: /* save */
							pB->addStyleClass("SaveFile");
							rname=stralloc("resources/OpenRDA/filesave.png");
							break;
						case 24: /* fastsave */
							pB->addStyleClass("FastSave");
							rname=stralloc("resources/OpenRDA/fastsave.png");
							break;
						case 25: /* exec */
							pB->addStyleClass("Execute");
							rname=stralloc("resources/OpenRDA/exec.png");
							break;
						case 26: /* html */
							pB->addStyleClass("HTML");
							rname=stralloc("resources/OpenRDA/html.png");
							break;
						case 27: /* pdf */
							pB->addStyleClass("PortableDocumentFormat");
							rname=stralloc("resources/OpenRDA/pdf.png");
							break;
						case 28: /* zip */
							pB->addStyleClass("Zip");
							rname=stralloc("resources/OpenRDA/zip.png");
							break;
						case 29: /* display */
							pB->addStyleClass("Display");
							rname=stralloc("resources/OpenRDA/display.png");
							break;
						case 30: /* archive */
							pB->addStyleClass("Archive");
							rname=stralloc("resources/OpenRDA/archive.png");
							break;
						case 31: /* folder */
							pB->addStyleClass("Folder");
							rname=stralloc("resources/OpenRDA/folder.png");
							break;
						case 32: /* www */
							pB->addStyleClass("World-Wide-Web");
							rname=stralloc("resources/OpenRDA/www.png");
							break;
						case 33: /* delete */
							pB->addStyleClass("Delete");
							rname=stralloc("resources/OpenRDA/delete.png");
							break;
						case 34: /* configure */
							pB->addStyleClass("Configure");
							rname=stralloc("resources/OpenRDA/configure.png");
							break;
						case 35: /* looknfeel */
							pB->addStyleClass("LooknFeel");
							rname=stralloc("resources/OpenRDA/looknfeel.png");
							break;
						case 36: /* view */
							pB->addStyleClass("View");
							rname=stralloc("resources/OpenRDA/view.png");
							break;
						case 37: /* view_detailed */
							pB->addStyleClass("ViewDetails");
							rname=stralloc("resources/OpenRDA/view_detailed.png");
							break;
						case 38: /* MainMenu */
							pB->addStyleClass("MainMenu");
							rname=stralloc("resources/OpenRDA/mainmenu.png");
							break;
						case 39: /* Open_Source */
							pB->addStyleClass("OpenSource");
							rname=stralloc("resources/OpenRDA/open_source.png");
							break;
						case 40: /* GoogleMaps */
							pB->addStyleClass("GoogleMaps");
							rname=stralloc("resources/OpenRDA/GoogleMaps.png");
							break;
						case 41: /* top */
							pB->addStyleClass("TopofList");
							rname=stralloc("resources/OpenRDA/top.png");
							break;
						case 42: /* bottom */
							pB->addStyleClass("BottomofList");
							rname=stralloc("resources/OpenRDA/bottom.png");
							break;
						case 43: /* announcement */
							pB->addStyleClass("Announcement");
							rname=stralloc("resources/OpenRDA/announcement.png");
							break;
						case 44: /* chat */
							pB->addStyleClass("Chat");
							rname=stralloc("resources/OpenRDA/chat.png");
							break;
						case 45: /* download */
							pB->addStyleClass("Download");
							rname=stralloc("resources/OpenRDA/download.png");
							break;
						case 46: /* upload */
							pB->addStyleClass("Upload");
							rname=stralloc("resources/OpenRDA/upload.png");
							break;
						case 47: /* news */
							pB->addStyleClass("News");
							rname=stralloc("resources/OpenRDA/news.png");
							break;
						case 48: /* documentation */
							pB->addStyleClass("Documentation");
							rname=stralloc("resources/OpenRDA/documentation.png");
							break;
						case 49: /* copy */
							pB->addStyleClass("Copy");
							rname=stralloc("resources/OpenRDA/copy.png");
							break;
						case 50: /* video */
							pB->addStyleClass("Video");
							rname=stralloc("resources/OpenRDA/video.png");
							break;
						case 51: /* ksnapshot */
							pB->addStyleClass("ScreenSnapShot");
							rname=stralloc("resources/OpenRDA/ksnapshot.png");
							break;
						case 52: /* konqueror */
							pB->addStyleClass("Konqueror");
							rname=stralloc("resources/OpenRDA/konqueror.png");
							break;
						case 53: /* cybersupport */
							pB->addStyleClass("CuberSupport");
							rname=stralloc("resources/OpenRDA/cybersupport.png");
							break;
						case 54: /* saveupdate */
							pB->addStyleClass("SaveandUpdate");
							rname=stralloc("resources/OpenRDA/saveupdate.png");
							break;
						case 55: /* printerror */
							pB->addStyleClass("PrintErrors");
							rname=stralloc("resources/OpenRDA/printerror.png");
							break;
						case 56: /* printwarning */
							pB->addStyleClass("PrintWarnings");
							rname=stralloc("resources/OpenRDA/printwarning.png");
							break;
						case 57: /* addabove */
							pB->addStyleClass("AddAbove");
							rname=stralloc("resources/OpenRDA/addabove.png");
							break;
						case 58: /* addbelow */
							pB->addStyleClass("AddBelow");
							rname=stralloc("resources/OpenRDA/addbelow.png");
							break;
						case 59: /* dotdotdot */
							pB->addStyleClass("DotDotDot");
							rname=stralloc("resources/OpenRDA/dotdotdot.png");
							break;
						case 60: /* tuxCloud */
							pB->addStyleClass("TuxCloud");
							rname=stralloc("resources/OpenRDA/tuxCloud.png");
							break;
						case 61: /* FYdotdot */
							pB->addStyleClass("FYDotDotDot");
							rname=stralloc("resources/OpenRDA/FYdotdot.png");
							break;
						case 62: /* csv */
							pB->addStyleClass("CSV");
							rname=stralloc("resources/OpenRDA/csv.png");
							break;
						case 63: /* paperclip */
							pB->addStyleClass("Paperclip");
							rname=stralloc("resources/OpenRDA/paperclip.png");
							break;
						case 64: /* delete small*/
							pB->addStyleClass("DeleteSmall");
							rname=stralloc("resources/OpenRDA/delete_small.png");
							break;
						case 65: /* user */
							pB->addStyleClass("UserProfile");
							rname=stralloc("resources/OpenRDA/user.png");
							break;
						case 66: /* wiki */
							pB->addStyleClass("Wiki");
							rname=stralloc("resources/OpenRDA/wiki.png");
							break;
						case 67: /* browse */
							pB->addStyleClass("Browse");
							rname=stralloc("resources/OpenRDA/browse.png");
							break;
						case 68: /* select */
							pB->addStyleClass("Select");
							rname=stralloc("resources/OpenRDA/select.png");
							break;
					}
					if(!isEMPTY(rname))
					{
						WK = new WLink(rname);
						if(pB!=NULL) pB->setIcon(*WK);
							else myImage->setImageLink(*WK);
					}
				} else if(rtype==5)
				{
					WK = new WLink(pixmap);
					if(pB!=NULL) pB->setIcon(*WK);
						else myImage->setImageLink(*WK);
				}
				if(!RDAstrcmp(member->rscrname,"QUIT"))
				{
					pB->addStyleClass("Quit");
				} else if(!RDAstrcmp(member->rscrname,"SAVE"))
				{
					pB->addStyleClass("SaveFile");
				} else if(!RDAstrcmp(member->rscrname,"FAST SAVE"))
				{
					pB->addStyleClass("FastSave");
				} else if(!RDAstrcmp(member->rscrname,"SELECT"))
				{
					pB->addStyleClass("Select");
				} else if(!RDAstrcmp(member->rscrname,"HELP"))
				{
					pB->addStyleClass("Help");
				} else if(!RDAstrcmp(member->rscrname,"ADD"))
				{
					pB->addStyleClass("Add2");
				} else if(!RDAstrcmp(member->rscrname,"RUN REPORT"))
				{
					pB->addStyleClass("RunReport");
				}
/*lint -e64 */
				if(pB!=NULL)
				{
					if(member->type==1 || member->type==2)
					{
/* runfunctioncallback */
						pB->clicked().connect(boost::bind(&runfunctioncallback,member));
					} else if(member->type==3)
					{
/* runfuncexitcallback */
						pB->clicked().connect(boost::bind(&runfuncexitcallback,member));
					} else if(member->type==4)
					{
/* runfunckwincallback */
						pB->clicked().connect(boost::bind(&runfunckwincallback,member));
					} else if(member->type==5)
					{
/* runfunckwincallback */
						pB->clicked().connect(boost::bind(&runfunckwincallback,member));
					}
				} else {
					if(member->type==1 || member->type==2)
					{
/* runfunctioncallback */
						myImage->clicked().connect(boost::bind(&runfunctioncallback,member));
					} else if(member->type==3)
					{
/* runfuncexitcallback */
						myImage->clicked().connect(boost::bind(&runfuncexitcallback,member));
					} else if(member->type==4)
					{
/* runfunckwincallback */
						myImage->clicked().connect(boost::bind(&runfunckwincallback,member));
					} else if(member->type==5)
					{
/* runfunckwincallback */
						myImage->clicked().connect(boost::bind(&runfunckwincallback,member));
					}
				}
/*lint +e64 */
				if(!member->editable)
				{
					ADVMEMBERSETEDITABLE(member,FALSE,FALSE);
				} else if(!isEMPTY(member->editable_expression))
				{
					if(rsx->EvalFuncArgs!=NULL)
					{
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx,rsx->EvalFuncArgs);
/*lint +e746*/
					} else {
/*lint -e746*/
						member->user_editable=rsx->EvalFunc(member->editable_expression,rsx);
/*lint +e746*/
					}
					ADVMEMBERSETEDITABLE(member,member->user_editable,TRUE);
				}
				member->rtype=rtype;
			}
			break;
		case SCROLLEDTEXT: 
			if(!USER_INTERFACE)
			{
				if(!isEMPTY(member->value.string_value))
				{
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui || diaggui_field)
					{
						prterr("DIAG Setting Resource [%s]'s Initial value [%s].",member->rscrname,(member->value.string_value!=NULL ? member->value.string_value:""));
					}
#endif /* USE_RDA_DIAGNOSTICS */
					temp_xstr=new Wt::WString(member->value.string_value);
				} else {
					temp_xstr=new Wt::WString("");
				}
				if(rtype==0)
				{
					if(Parent_Table!=NULL)
					{
						TA=new Wt::WTextArea(*temp_xstr,(Wt::WContainerWidget *)tCell);
						tCell->addWidget((Wt::WWidget *)TA);
					} else TA=new Wt::WTextArea(*temp_xstr,(Wt::WContainerWidget *)parent);
					TA->setColumns(cols);
					TA->setRows(rows);
					member->w=(Wt::WWidget *)TA;	
					WW=(Wt::WWidget *)TA;
					mssc=ModuleScreenStyleClass(rsx);
					fssc=InputFieldStyleClass(member);
					memset(GUIstemp,0,1024);
					sprintf(GUIstemp,"OpenRDA %s %s TextArea",mssc,fssc);
					WW->addStyleClass(GUIstemp);
					if(mssc!=NULL) Rfree(mssc);
					if(fssc!=NULL) Rfree(fssc);
					wFormW=(Wt::WFormWidget *)TA;
					if(!member->editable)
					{
						ADVMEMBERSETEDITABLE(member,FALSE,FALSE);
					} else if(!isEMPTY(member->editable_expression))
					{
						if(rsx->EvalFuncArgs!=NULL)
						{
/*lint -e746*/
							member->user_editable=rsx->EvalFunc(member->editable_expression,rsx,rsx->EvalFuncArgs);
/*lint +e746*/
						} else {
/*lint -e746*/
							member->user_editable=rsx->EvalFunc(member->editable_expression,rsx);
/*lint +e746*/
						}
						ADVMEMBERSETEDITABLE(member,member->user_editable,TRUE);
					}
					TA->changed().connect(boost::bind(&scrolledtext_length,member));
					wFormW->blurred().connect(boost::bind(&losingfocusfunction,member));
					wFormW->focussed().connect(boost::bind(&gainingfocusfunction,member));
				} else if(rtype==1)
				{
					if(Parent_Table!=NULL)
					{
						TA=new Wt::WTextArea(*temp_xstr,(Wt::WContainerWidget *)tCell);
						tCell->addWidget((Wt::WWidget *)TA);
					} else TA=new Wt::WTextArea(*temp_xstr,(Wt::WContainerWidget *)parent);
					member->w=(Wt::WWidget *)TA;	
					TA->setColumns(cols);
					TA->setRows(rows);
					WW=(Wt::WWidget *)TA;
					mssc=ModuleScreenStyleClass(rsx);
					fssc=InputFieldStyleClass(member);
					memset(GUIstemp,0,1024);
					sprintf(GUIstemp,"OpenRDA %s %s TextArea",mssc,fssc);
					WW->addStyleClass(GUIstemp);
					if(mssc!=NULL) Rfree(mssc);
					if(fssc!=NULL) Rfree(fssc);
					wFormW=(Wt::WFormWidget *)TA;
					if(!member->editable)
					{
						ADVMEMBERSETEDITABLE(member,FALSE,FALSE);
					} else if(!isEMPTY(member->editable_expression))
					{
						if(rsx->EvalFuncArgs!=NULL)
						{
/*lint -e746*/
							member->user_editable=rsx->EvalFunc(member->editable_expression,rsx,rsx->EvalFuncArgs);
/*lint +e746*/
						} else {
/*lint -e746*/
							member->user_editable=rsx->EvalFunc(member->editable_expression,rsx);
/*lint +e746*/
						}
						ADVMEMBERSETEDITABLE(member,member->user_editable,TRUE);
					}
					TA->changed().connect(boost::bind(&scrolledtext_length,member));
					wFormW->blurred().connect(boost::bind(&losingfocusfunction,member));
					wFormW->focussed().connect(boost::bind(&gainingfocusfunction,member));
				} else if(rtype==2)
				{
					if(Parent_Table!=NULL)
					{
						TE=new Wt::WTextEdit(*temp_xstr,(Wt::WContainerWidget *)tCell);
						tCell->addWidget((Wt::WWidget *)TE);
					} else TE=new Wt::WTextEdit(*temp_xstr,(Wt::WContainerWidget *)parent);
					TE->setColumns(cols);
					TE->setRows(rows);
					member->w=(Wt::WWidget *)TE;	
					WW=(Wt::WWidget *)TE;
					wH=Wt::WLength(200,Wt::WLength::Pixel);
					wL=Wt::WLength(600,Wt::WLength::Pixel);
					TE->setMinimumSize(wL,wH);
					TE->setConfigurationSetting("browser_spellcheck",TRUE);
					TE->setConfigurationSetting("skins",(const char *)"highcontrast");
/*
					TA=(Wt::WTextArea *)TE;
					TA->setColumns(cols);
					TA->setRows(rows);
*/
					mssc=ModuleScreenStyleClass(rsx);
					fssc=InputFieldStyleClass(member);
					memset(GUIstemp,0,1024);
					sprintf(GUIstemp,"OpenRDA %s %s TextEdit",mssc,fssc);
					WW->addStyleClass(GUIstemp);
					if(mssc!=NULL) Rfree(mssc);
					if(fssc!=NULL) Rfree(fssc);
					wFormW=(Wt::WFormWidget *)TE;
					if(!member->editable)
					{
						ADVMEMBERSETEDITABLE(member,FALSE,FALSE);
					} else if(!isEMPTY(member->editable_expression))
					{
						if(rsx->EvalFuncArgs!=NULL)
						{
/*lint -e746*/
							member->user_editable=rsx->EvalFunc(member->editable_expression,rsx,rsx->EvalFuncArgs);
/*lint +e746*/
						} else {
/*lint -e746*/
							member->user_editable=rsx->EvalFunc(member->editable_expression,rsx);
/*lint +e746*/
						}
						ADVMEMBERSETEDITABLE(member,member->user_editable,TRUE);
					}
					TE->changed().connect(boost::bind(&scrolledtext_length,member));
					wFormW->blurred().connect(boost::bind(&losingfocusfunction,member));
					wFormW->focussed().connect(boost::bind(&gainingfocusfunction,member));
				}
				if(temp_xstr!=NULL) temp_xstr->~WString();
			}
			break;
		case SCROLLEDLIST:
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field) 
			{ 
				prterr("DIAG Resource [%s] has [%d] items.",member->rscrname,member->items); 
			}
#endif /* USE_RDA_DIAGNOSTICS */
			if(!USER_INTERFACE)
			{
				for(w=0;w<member->items;++w)
				{
					h=RDAstrlen((*(member->list[0]+w)));
					if(h>longest) longest=h;
				}
				if(!RDAstrcmp(member->rscrname,"BROWSE LIST") && USE_BROWSELISTLABEL)
				{
					browse_list_desc=rsx->rscs+FINDRSC(rsx,"BROWSE LIST DESCRIPTION");
					if(Parent_Table!=NULL)
					{
						form1=new Wt::WContainerWidget();
						form1->setOverflow(WContainerWidget::OverflowVisible,Vertical);	
						form1->setOverflow(WContainerWidget::OverflowVisible,Horizontal);	
						form=new Wt::WVBoxLayout();
						form1->setLayout(form);
						tCell->addWidget((Wt::WWidget *)form1);
					} else {
						form1=new Wt::WContainerWidget((Wt::WContainerWidget *)parent);
						form1->setOverflow(WContainerWidget::OverflowVisible,Vertical);	
						form1->setOverflow(WContainerWidget::OverflowVisible,Horizontal);	
						form=new Wt::WVBoxLayout();
						form1->setLayout(form);
					}
					daL=(Wt::WLayout *)form1;
					daL->setContentsMargins(0,0,0,0);
					bLE=new Wt::WLineEdit((Wt::WWidget *)form1);
					bLE->setReadOnly(TRUE);
					WW=(Wt::WWidget *)bLE;
					mssc=ModuleScreenStyleClass(rsx);
					fssc=InputFieldStyleClass(member);
					memset(GUIstemp,0,1024);
					sprintf(GUIstemp,"OpenRDA %s %s BrowseListLabel",mssc,fssc);
					WW->addStyleClass(GUIstemp);
					if(mssc!=NULL) Rfree(mssc);
					if(fssc!=NULL) Rfree(fssc);
					WW->setMaximumSize(Wt::WLength(1600),Wt::WLength::Auto);
					browse_list_desc->w=(Wt::WWidget *)bLE;
					c=new Wt::WString(browse_list_desc->value.string_value,UTF8);
					bLE->setText(*c);
					if(c!=NULL) c->~WString();
					form->addWidget((Wt::WWidget *)bLE,10,Wt::AlignLeft | Wt::AlignJustify);
					LB=new Wt::WSelectionBox(form1);
					WW=(Wt::WWidget *)LB;
					mssc=ModuleScreenStyleClass(rsx);
					fssc=InputFieldStyleClass(member);
					memset(GUIstemp,0,1024);
					sprintf(GUIstemp,"OpenRDA %s %s BrowseList",mssc,fssc);
					WW->addStyleClass(GUIstemp);
					if(mssc!=NULL) Rfree(mssc);
					if(fssc!=NULL) Rfree(fssc);
					WW->setMaximumSize(Wt::WLength(1600),Wt::WLength::Auto);
					cB=(Wt::WComboBox *)LB;
					wSIM=new Wt::WStandardItemModel();
					cB->setModel(wSIM);
					member->wSIM=wSIM;
					member->w=(Wt::WWidget *)LB;
					WW=(Wt::WWidget *)LB;
	
					wFormW=(Wt::WFormWidget *)LB;
					form->addWidget((Wt::WWidget *)LB,10,Wt::AlignLeft | Wt::AlignJustify);
					LB->setVerticalSize(member->rows);
					if(*member->value.integer_value<0)
					{
						*member->value.integer_value=member->items-1;
					} else if(*member->value.integer_value>member->items)
					{
						*member->value.integer_value=0;
					}
					LB->activated().connect(boost::bind(&list_callback,member));
					LB->doubleClicked().connect(boost::bind(&list_doubleclick_callback,member));
					temp1=NULL;
					member->wSIM->clear();
					for(w=0;w<member->items;++w)
					{
						wSI=new WStandardItem();
						wSI->setFlags(wSI->flags() | Wt::ItemIsXHTMLText);
/* can use UTF8 encoding instead of WString */
/* replace each space with 2-3 char sequence */
/* 0xC2 0xA0 2 byte sequence of UTF8 */
						temp1=((*(member->list[0]+w)));
						temp_str=RDA_EncodeWhiteSpace(temp1);
						temp_xstr = new Wt::WString(temp_str,UTF8);
						if(temp_str!=NULL) Rfree(temp_str);
						wSI->setText(*temp_xstr);
						temp_xstr->~WString();
						member->wSIM->appendRow(wSI);
					}
					LB->setCurrentIndex(*member->value.integer_value);
					if(!member->editable)
					{
						ADVMEMBERSETEDITABLE(member,FALSE,FALSE);
					} else if(!isEMPTY(member->editable_expression))
					{
						if(rsx->EvalFuncArgs!=NULL)
						{
/*lint -e746*/
							member->user_editable=rsx->EvalFunc(member->editable_expression,rsx,rsx->EvalFuncArgs);
/*lint +e746*/
						} else {
/*lint -e746*/
							member->user_editable=rsx->EvalFunc(member->editable_expression,rsx);
/*lint +e746*/
						}
						ADVMEMBERSETEDITABLE(member,member->user_editable,TRUE);
					}
				} else if(member->rows<2)
				{
					if(Parent_Table!=NULL)
					{
						cB=new Wt::WComboBox((Wt::WContainerWidget *)tCell);
						tCell->addWidget((Wt::WWidget *)cB);
					} else {
						cB=new Wt::WComboBox((Wt::WContainerWidget *)parent);
					}
					member->w=(Wt::WWidget *)cB;
					WW=(Wt::WWidget *)cB;
					mssc=ModuleScreenStyleClass(rsx);
					fssc=InputFieldStyleClass(member);
					memset(GUIstemp,0,1024);
					sprintf(GUIstemp,"OpenRDA %s %s ComboBox",mssc,fssc);
					WW->addStyleClass(GUIstemp);
					if(mssc!=NULL) Rfree(mssc);
					if(fssc!=NULL) Rfree(fssc);
					wFormW=(Wt::WFormWidget *)cB;
					WW->setMaximumSize(Wt::WLength(1600),Wt::WLength::Auto);
					for(w=0;w<member->items;++w)
					{
						temp_xstr = new Wt::WString((*(member->list[0]+w)),UTF8);
						cB->addItem(*temp_xstr);
						temp_xstr->~WString();
					}
					if(*member->value.integer_value<0 || *member->value.integer_value>member->items) *member->value.integer_value=0;
					cB->setCurrentIndex(*member->value.integer_value);
					cB->setValueText(cB->itemText(*member->value.integer_value));
					cB->clicked().connect(boost::bind(&list_callback,member));
					cB->activated().connect(boost::bind(&list_callback,member));
					if(!member->editable)
					{
						ADVMEMBERSETEDITABLE(member,FALSE,FALSE);
					} else if(!isEMPTY(member->editable_expression))
					{
						if(rsx->EvalFuncArgs!=NULL)
						{
/*lint -e746*/
							member->user_editable=rsx->EvalFunc(member->editable_expression,rsx,rsx->EvalFuncArgs);
/*lint +e746*/
						} else {
/*lint -e746*/
							member->user_editable=rsx->EvalFunc(member->editable_expression,rsx);
/*lint +e746*/
						}
						ADVMEMBERSETEDITABLE(member,member->user_editable,TRUE);
					}
				} else {
					if(Parent_Table!=NULL)
					{
						LB=new Wt::WSelectionBox((Wt::WContainerWidget *)tCell);
						tCell->addWidget((Wt::WWidget *)LB);
					} else {
						LB=new Wt::WSelectionBox((Wt::WContainerWidget *)parent);
					}
					member->w=(Wt::WWidget *)LB;
					WW=(Wt::WWidget *)LB;
					mssc=ModuleScreenStyleClass(rsx);
					fssc=InputFieldStyleClass(member);
					memset(GUIstemp,0,1024);
					sprintf(GUIstemp,"OpenRDA %s %s ScrolledList",mssc,fssc);
					WW->addStyleClass(GUIstemp);
					if(mssc!=NULL) Rfree(mssc);
					if(fssc!=NULL) Rfree(fssc);
					cB=(Wt::WComboBox *)LB;
					wFormW=(Wt::WFormWidget *)LB;
					wSIM=new Wt::WStandardItemModel();
					cB->setModel(wSIM);
					member->wSIM=wSIM;
					LB->setVerticalSize(member->rows);
					WW->setMaximumSize(Wt::WLength(1600),Wt::WLength::Auto);
					if(*member->value.integer_value<0)
					{
						*member->value.integer_value=member->items-1;
					} else if(*member->value.integer_value>=member->items)
					{
						*member->value.integer_value=0;
					}
					LB->activated().connect(boost::bind(&list_callback,member));
					if(!RDAstrcmp(member->rscrname,"BROWSE LIST"))
					{
						LB->doubleClicked().connect(boost::bind(&list_doubleclick_callback,member));
					}
					temp1=NULL;
					member->wSIM->clear();
					for(w=0;w<member->items;++w)
					{
						wSI=new WStandardItem();
						wSI->setFlags(wSI->flags() | Wt::ItemIsXHTMLText);
/* can use UTF8 encoding instead of WString */
/* replace each space with 2-3 char sequence */
/* 0xC2 0xA0 2 byte sequence of UTF8 */
						temp1=stralloc((*(member->list[0]+w)));
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
						temp_xstr = new Wt::WString(temp_str,UTF8);
						if(temp_str!=NULL) Rfree(temp_str);
						wSI->setText(*temp_xstr);
						temp_xstr->~WString();
						member->wSIM->appendRow(wSI);
						if(temp1!=NULL) Rfree(temp1);
					}
					LB->setCurrentIndex(*member->value.integer_value);
					if(!member->editable)
					{
						ADVMEMBERSETEDITABLE(member,FALSE,FALSE);
					} else if(!isEMPTY(member->editable_expression))
					{
						if(rsx->EvalFuncArgs!=NULL)
						{
/*lint -e746*/
							member->user_editable=rsx->EvalFunc(member->editable_expression,rsx,rsx->EvalFuncArgs);
/*lint +e746*/
						} else {
/*lint -e746*/
							member->user_editable=rsx->EvalFunc(member->editable_expression,rsx);
/*lint +e746*/
						}
						ADVMEMBERSETEDITABLE(member,member->user_editable,TRUE);
					}
				}
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG Setting Resource [%s]'s Initial value [%d].",member->rscrname,*member->value.integer_value);
				}
#endif /* USE_RDA_DIAGNOSTICS */
			}
			break;
		case PROGRESS_BAR:
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field) 
			{ 
				prterr("DIAG Resource [%s] has [%d] items.",member->rscrname,member->items); 
			}
#endif /* USE_RDA_DIAGNOSTICS */
			if(Parent_Table!=NULL)
			{
				progBar=new Wt::WProgressBar((Wt::WContainerWidget *)tCell);
			} else progBar=new Wt::WProgressBar((Wt::WContainerWidget *)parent);
			member->w=(Wt::WWidget *)progBar;
			pMin=0;
			pMax=member->items;
			progBar->setRange(pMin,pMax);
			pVal=*member->value.integer_value;
			progBar->setValue(pVal);
			WW=(Wt::WWidget *)progBar;
			mssc=ModuleScreenStyleClass(rsx);
			fssc=InputFieldStyleClass(member);
			memset(GUIstemp,0,1024);
			sprintf(GUIstemp,"OpenRDA %s %s ProgressBar",mssc,fssc);
			WW->addStyleClass(GUIstemp);
			if(mssc!=NULL) Rfree(mssc);
			if(fssc!=NULL) Rfree(fssc);
			if(Parent_Table!=NULL)
			{
				tCell->addWidget((Wt::WWidget *)member->w);
			}
			break;
		default:
			prterr("Error field type [%d] unrecognized for member [%s] ",member->field_type,member->rscrname);
			break;
	}
	s1->~string();
	if(!USER_INTERFACE)
	{
		if(member->w!=NULL)
		{
			if((((member->field_type!=BUTTONS && member->field_type!=BOOLNS) || (member->rtype>0 && member->rtype<6)) && (member->field_type!=SCROLLEDLIST) && member->field_type!=SCROLLEDTEXT))
			{
				if(member->field_type==DATES && member->rtype==0)
				{
				}
			} else if(member->field_type==BUTTONS && RDAstrstr(member->rscrname,":DELETE"))
			{
				w=16;
				WW=(Wt::WWidget *)member->w;
				wL=Wt::WLength(w);
				WW->setWidth(wL);
				h=16;
				wH=Wt::WLength(h);
				WW->setHeight(wH);
			} else if(member->field_type==SCROLLEDTEXT || (member->field_type==SCROLLEDLIST && member->rows>1))
			{
			}
		}
		WW=(Wt::WWidget *)member->w;
		wFormW=(Wt::WFormWidget *)member->w;
		WW->setDisabled((member->sensitive ? FALSE:TRUE));
		if(member->field_type==BUTTONS)
		{
			if(member->sensitive==FALSE && ((member->rtype>=1 && member->rtype<=4) || (member->rtype>=6 && member->rtype<=68)))
			{
				wFormW=(Wt::WFormWidget *)member->w;
/* Remove Tool Tip? Maybe not needed with Wt */
			}
		}
	}
	WW=(Wt::WWidget *)member->w;
	WW->hiddenKeepsGeometry();
	memberrequired(member);

#ifdef USE_RDA_DIAGNOSTICS
	if(diagcss)
	{
		if(cssNames!=NULL)
		{
			fprintf(cssNames,"\"%s\",\"%s\"\r\n",member->rscrname,WW->styleClass().toUTF8().c_str());
		}
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(!member->sensitive)
	{
		if(!isEMPTY(member->sensitive_expression))
		{
			if(rsx->EvalFuncArgs!=NULL)
			{
/*lint -e746*/
				member->user_sensitive=rsx->EvalFunc(member->sensitive_expression,rsx,rsx->EvalFuncArgs);
/*lint +e746*/
			} else {
/*lint -e746*/
				member->user_sensitive=rsx->EvalFunc(member->sensitive_expression,rsx);
/*lint +e746*/
			}
			if(!USER_INTERFACE)
			{
				if(member->w!=NULL)
				{
					if(member->user_sensitive==TRUE)
					{
						WW->setDisabled(FALSE);
#ifdef __USE_ANIMATIONS__
						WW->setHidden(FALSE,fadein);
#endif /* __USE_ANIMATIONS__ */
					} else {
						WW->setDisabled(TRUE);
#ifdef __USE_ANIMATIONS__
						WW->setHidden(TRUE,fadeout);
#endif /* __USE_ANIMATIONS__ */
					}
					if(member->field_type==BUTTONS)
					{
						if(member->user_sensitive==FALSE && ((member->rtype>=1 && member->rtype<=4) || (member->rtype>=6 && member->rtype<=68)))
						{
							wFormW=(Wt::WFormWidget *)member->w;
/* Remove Tool Tip? Maybe not needed with Wt */
						}
					}
				}
			} else {
				prterr("Error Character Interface doesn't support FINDRSCSETSENSITIVE.");
			}
			member->user_sensitive=member->user_sensitive;
		}
	}
	if(dashes!=NULL) Rfree(dashes);
}
