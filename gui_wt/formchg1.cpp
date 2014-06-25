#include<guip.hpp>
#include<gui.hpp>

#define __NEED_WDIALOG_LAYOUT__

char AT_LEAST_ONE_WINDOW=FALSE;
Wt::WContainerWidget *QN2IT_PARENT=NULL;
short QN2IT_PARENT_TYPE=0;
char SIMPLE_SCREENS=FALSE,CENTER_WINDOWS=FALSE;

int CurNumQn2it=0;

void ClearRootContainer()
{
	WContainerWidget *c=NULL;

	c=RDAMAINWIDGET->root();
	c->clear();
}

#define USING_REDIRECT
void xOpenRDAWiki(char *page,int line,char *file)
{
#ifdef USING_REDIRECT
	char *user="openrda",*passw="cW9jd3ExRXhWS1U5";
	char *wiki_rdrct=NULL,*temp=NULL,*temp1=NULL;
	char *url=NULL;
	int len=0;
	base64_encodestate enstate;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagolh)
	{
		prterr("DIAG OpenRDAWiki with Page [%s] at line [%d] file [%s].",(page!=NULL ? page:""),line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(!isEMPTY(page)) temp=stralloc(page);
		else temp=stralloc("Category:Modules");
	base64_init_encodestate(&enstate);
	len=RDAstrlen(temp);
	wiki_rdrct=Rmalloc((len+1)*2);
	temp1=Rmalloc((len+1)*2);
	len=base64_encode_block(temp,len,temp1,&enstate);
	sprintf(wiki_rdrct,"%s%s",wiki_rdrct,temp1);
	memset(temp1,0,RDAstrlen(temp)+1);
	len=base64_encode_blockend(temp1,&enstate);
	sprintf(wiki_rdrct,"%s%s",wiki_rdrct,temp1);
	url=Rmalloc(256);
	sprintf(url,"http://wiki.openrda.com/index.php?user=%s&passw=%s&redirect=%s",user,passw,(wiki_rdrct!=NULL ? wiki_rdrct:""));
	GUI_OpenURL(url);
	if(temp!=NULL) Rfree(temp);
	if(temp1!=NULL) Rfree(temp1);
	if(wiki_rdrct!=NULL) Rfree(wiki_rdrct);
#else 
	char *url=NULL;

	if(!isEMPTY(page))
	{
		url=Rmalloc(RDAstrlen(page)+40);
		sprintf(url,"http://wiki.openrda.com/wiki/index.php/%s",(page!=NULL ? page:""));
	} else {
		url=stralloc("http://wiki.openrda.com/wiki/index.php/Category:Modules");
	}
	GUI_OpenURL(url);
	if(url!=NULL) Rfree(url);
#endif /* USING_REDIRECT */
}
void WikiSupport(RDArsrc *rs)
{
	char *temp2=NULL,*page=NULL;

	if(rs!=NULL)
	{
		page=Rmalloc(RDAstrlen(rs->module)+RDAstrlen(rs->screen)+2);
		temp2=adddashes(rs->screen);
		sprintf(page,"%s_%s",(rs->module!=NULL ? rs->module:""),(temp2!=NULL ? temp2:""));
		if(temp2!=NULL) Rfree(temp2);
	}
	OpenRDAWiki(page);
	if(page!=NULL) Rfree(page);
}
static void CyberSupport(RDArsrc *rsrcx)
{
	APPlib *args=NULL,*envpx=NULL;
	char temp1[512],*cyber_product=NULL,*cyber_process=NULL;

	args=APPlibNEW();
	addAPPlib(args,"OLHELP");
	addAPPlib(args,"CYBERSUPPORT");
	envpx=APPlibNEW();
	cyber_product=RDA_GetEnv("CYBER_PRODUCT");
	if(!isEMPTY(cyber_product) && (!RDAstrcmp(rsrcx->module,"UTILITIES") && !RDAstrcmp(rsrcx->screen,"TEXT BROWSER")) || (!RDAstrcmp(rsrcx->module,"RPTGEN") && !RDAstrcmp(rsrcx->screen,"RUN REPORT CHAIN WINDOW")))
	{
		sprintf(temp1,"ADD_PRODUCT=%s",cyber_product);
	} else {
		sprintf(temp1,"ADD_PRODUCT=%s",((rsrcx->module!=NULL ? rsrcx->module:"")));
	}
	addAPPlib(envpx,temp1);
	memset(temp1,0,512);
	cyber_process=RDA_GetEnv("CYBER_PROCESS");
	if(!isEMPTY(cyber_product) && (!RDAstrcmp(rsrcx->module,"UTILITIES") && !RDAstrcmp(rsrcx->screen,"TEXT BROWSER")) || (!RDAstrcmp(rsrcx->module,"RPTGEN") && !RDAstrcmp(rsrcx->screen,"RUN REPORT CHAIN WINDOW")))
	{
		sprintf(temp1,"ADD_PROCESS=%s",cyber_process);
	} else {
		sprintf(temp1,"ADD_PROCESS=%s",(rsrcx->screen!=NULL ? rsrcx->screen:""));
	}
	addAPPlib(envpx,temp1);
	ADVExecuteProgram("doadd",args,envpx);
	if(args!=NULL) freeapplib(args);
	if(envpx!=NULL) freeapplib(envpx);
}
/*lint -e715 */
void guihelpfunction(RDArmem *m)
{
	RDArsrc *rsrc;
	RDArmem *member=NULL;
	Wt::WFormWidget *FW=NULL;
	int x=0;

	rsrc=m->parent;
	FW=(Wt::WFormWidget *)m->w;
	if(FW->hasFocus())
	{
		member=m;
	} else {
		for(x=0,member=rsrc->rscs;x<rsrc->numrscs;++x,++member)
		{
			if(member->w!=NULL)
			{
				FW=(Wt::WFormWidget *)member->w;
				if(FW->hasFocus()) break;
			}
		}
		if(x>=rsrc->numrscs) member=m;
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG guihelpfunction Help Function for Resource [%s] on Screen [%s] [%s].",member->rscrname,rsrc->module,rsrc->screen);
	}
#endif /* USE_RDA_DIAGNOSTICS */
#ifdef TIMEOUT_CURSORS
	TimeoutCursors(rsrc,TRUE);
#endif
	if(member->helpfunction!=NULL)
	{
/*lint -e746 */
		member->helpfunction((void *)member->parent,(void *)member);
/*lint +e746 */
	}
#ifdef TIMEOUT_CURSORS
	TimeoutCursors(rsrc,FALSE);
#endif
}
/*lint +e715 */
short checkdiff(RDArmem *member,RDArmem *mem)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG checkdiff for Resource [%s].",member->rscrname);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	switch(member->field_type)
	{
		case SCROLLEDLIST:
		case LONGV:
		case SLONGV:
		case PROGRESS_BAR:
			if(*mem->value.integer_value!=*member->value.integer_value) 
			{
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG exiting checkdiff for Resource [%s].",member->rscrname);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				return(TRUE);
			} else return(FALSE);
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
			if(isEMPTY(member->value.string_value) &&
				isEMPTY(mem->value.string_value))
			{
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG exiting checkdiff for Resource [%s].",member->rscrname);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				return(FALSE);
			} else {
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG exiting checkdiff for Resource [%s].",member->rscrname);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				return(RDAstrcmp(member->value.string_value,mem->value.string_value));
			}
		case DOLLARS:
		case DOLLARS_NOCENTS:
		case DECIMALV:
		case DOUBLEV:
		case SDOUBLEV:
		case SDECIMALV:
			if(*mem->value.float_value!=*member->value.float_value) 
			{
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG exiting checkdiff for Resource [%s].",member->rscrname);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				return(TRUE);
			} else {
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG exiting checkdiff for Resource [%s].",member->rscrname);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				return(FALSE);
			}
		case SHORTV:
		case SSHORTV:
			if(*mem->value.short_value!=*member->value.short_value) 
			{
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG exiting checkdiff for Resource [%s].",member->rscrname);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				return(TRUE);
			} else {
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG exiting checkdiff for Resource [%s].",member->rscrname);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				return(FALSE);
			}
		case CHARACTERS:
		case BOOLNS:
			if(*mem->value.string_value!=*member->value.string_value) 
			{
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG exiting checkdiff for Resource [%s].",member->rscrname);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				return(TRUE);
			} else { 
#ifdef USE_RDA_DIAGNOSTICS
				if(diaggui || diaggui_field)
				{
					prterr("DIAG exiting checkdiff for Resource [%s].",member->rscrname);
				}
#endif /* USE_RDA_DIAGNOSTICS */
				return(FALSE);
			}
		case BUTTONS:
#ifdef USE_RDA_DIAGNOSTICS
			if(diaggui || diaggui_field)
			{
				prterr("DIAG exiting checkdiff for Resource [%s].",member->rscrname);
			}
#endif /* USE_RDA_DIAGNOSTICS */
			return(TRUE);
		default:
			prterr("Error field type [%d] is invalid for Resource [%s].",member->field_type,member->rscrname);
			return(FALSE);
	}
}
/*lint -e715 */
void losingfocusfunction(RDArmem *member)
{
	RDArsrc *rsrc;
	short test=FALSE;
	RDArmem *mem=NULL;
	int p=0;
	std::stringstream s1;
	Wt::WTextArea *TA=NULL;
/*
	Wt::WWidget *WW=NULL;
*/
	Wt::WLineEdit *LE=NULL;

	if(member->editable==FALSE || member->user_editable==FALSE) return;
	if(member->app_update==TRUE) return;
	rsrc=(RDArsrc *)member->parent;
	if(member->field_type==DATES && member->rtype==0)
	{
		LE=(Wt::WLineEdit *)member->w;
		if(LE->validate()==Wt::WValidator::State::Invalid)
		{
			LE->setFocus();
			return;
		}
	} else if(RDA_CaptureAutoComplete!=NULL && (member->field_type==VARIABLETEXT || member->field_type==PLAINTEXT || member->field_type==ZIPCODE))
	{
		readmember(member);
		RDA_CaptureAutoComplete(rsrc->module,rsrc->screen,member->rscrname,member->value.string_value);
	}
	member->app_update=TRUE;
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG losingfocusfunction for Resource [%s] on Screen [%s] [%s].",member->rscrname,rsrc->module,rsrc->screen);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	switch(member->field_type)
	{
		case SCROLLEDLIST:
		case CHARACTERS:
		case BOOLNS:
		case BUTTONS:
			break;
		case SCROLLEDTEXT:
			TA=(Wt::WTextArea *)member->w;
			p=TA->cursorPosition();
			break;
		default:
			LE=(Wt::WLineEdit *)member->w;
			LE->validate();
			p=LE->cursorPosition();
			break;
	}
#ifdef TIMEOUT_CURSORS
	TimeoutCursors(rsrc,TRUE);
#endif
	if(member->field_type!=BUTTONS && member->field_type!=PROGRESS_BAR)
	{
		mem=Rmalloc(sizeof(RDArmem));
		mem->field_type=member->field_type;
		mem->field_length=member->field_length;
		mem->editable=member->editable;
		mem->items=member->items;
		mem->arglist=member->arglist;
		mem->callbacktype=member->callbacktype;
		mem->sensitive=member->sensitive;
		mem->list=member->list;
		mem->type=member->type;
		mem->w=member->w;
		mem->rscrname=stralloc(member->rscrname);
		mem->parent=member->parent;
		switch(member->field_type)
		{
			case SCROLLEDLIST:
				mem->value.integer_value=Rmalloc(sizeof(int));
				mem->dlen=sizeof(int);
				*mem->value.integer_value=*member->value.integer_value;
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
				mem->dlen=0;
				if(!isEMPTY(member->value.string_value))
				{
					mem->value.string_value=stralloc(member->value.string_value);
					mem->dlen=RDAstrlen(member->value.string_value)+1;
				} else {
					mem->value.string_value=NULL;
				}
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDOUBLEV:
			case SDECIMALV:
				mem->dlen=sizeof(double);
				mem->value.float_value=Rmalloc(sizeof(double));
				*mem->value.float_value=*member->value.float_value;
				break;
			case SHORTV:
			case SSHORTV:
				mem->dlen=sizeof(short);
				mem->value.short_value=Rmalloc(sizeof(short));
				*mem->value.short_value=*member->value.short_value;
				break;
			case LONGV:
			case SLONGV:
				mem->dlen=sizeof(int);
				mem->value.integer_value=Rmalloc(sizeof(int));
				*mem->value.integer_value=*member->value.integer_value;
				break;
			case CHARACTERS:
				mem->dlen=1;
				mem->value.string_value=Rmalloc(1);	
				*mem->value.string_value=*member->value.string_value;
				break;
			case BOOLNS:
				mem->dlen=1;
				mem->value.string_value=Rmalloc(1);	
				if(*member->value.string_value) *mem->value.string_value=TRUE;
					else *mem->value.string_value=FALSE;
				break;
			case BUTTONS:
				mem->dlen=0;
				break;
			case PROGRESS_BAR:
				break;
			default:
				prterr("Error field type [%d] is invalid for Resource [%s].",member->field_type,member->rscrname);
				break;
		}
		readmember(mem);
		test=checkdiff(member,mem);
		if(mem!=NULL)
		{
			if(mem->rscrname!=NULL) Rfree(mem->rscrname);
			switch(mem->field_type)
			{
				case SCROLLEDLIST:
				case LONGV:
				case SLONGV:
					if(mem->value.integer_value!=NULL) 
						Rfree(mem->value.integer_value);
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
					if(mem->value.string_value!=NULL)
					{
						Rfree(mem->value.string_value);
					}
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case DOUBLEV:
				case SDOUBLEV:
				case SDECIMALV:
					if(mem->value.float_value!=NULL) 
						Rfree(mem->value.float_value);
					break;
				case SHORTV:
				case SSHORTV:
					if(mem->value.short_value!=NULL) 
						Rfree(mem->value.short_value);
					break;
				case CHARACTERS:
				case BOOLNS:
					if(mem->value.string_value!=NULL) 
						Rfree(mem->value.string_value);
					break;
				case BUTTONS:
				case PROGRESS_BAR:
					break;
				default:
					prterr("Error field type [%d] is invalid for Resource [%s].",mem->field_type,mem->rscrname);
					break;
			}
			Rfree(mem);
		}
		if(test)
		{
			ExecuteRDArmemFunction(member);
		}
	} else if(member->field_type!=PROGRESS_BAR)
	{
		ExecuteRDArmemFunction(member);
		if(!isEMPTY(member->transversal_expression))
		{
			ComputeNewTransversalADV(rsrc,rsrc->EvalStr,rsrc->EvalStrArgs,rsrc->SubFunc,rsrc->SubFuncArgs,member->transversal_expression);
		}
	}
#ifdef TIMEOUT_CURSORS
	TimeoutCursors(rsrc,FALSE);
#endif
// set cursor position
	switch(member->field_type)
	{
		case SCROLLEDLIST:
		case CHARACTERS:
		case BOOLNS:
		case BUTTONS:
		case PROGRESS_BAR:
			break;
		case SCROLLEDTEXT:
			break;
		default:
/*
			WW=(Wt::WWidget *)LE;
			s1 << WW->jsRef() << ".selectionStart = " << p << ";";
			WW->doJavaScript(s1.str());
*/
			break;
	}
	member->app_update=FALSE;
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG exiting losingfocusfunction for Resource [%s] on Screen [%s] [%s].",member->rscrname,rsrc->module,rsrc->screen);
	}
#endif /* USE_RDA_DIAGNOSTICS */
}
void membersetcursor(RDArmem *member)
{
	std::stringstream ss1;
	int cp=0,cp1=0,col=(member->cols!=0 ? member->cols:member->field_length);
	Wt::WWidget *WW=(Wt::WWidget *)member->w;

	switch(member->field_type)
	{
		case SCROLLEDTEXT:
		case VARIABLETEXT:
		case PLAINTEXT:
			cp=0;
			cp1=RDAstrlen(member->value.string_value);
			ss1 << WW->jsRef() << ".selectionStart = " << cp << ";" << WW->jsRef() << ".selectionEnd = " << cp1 << ";";
			WW->doJavaScript(ss1.str());
			break;
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
		case CHARACTERS:
			cp=0;
			if(isEMPTY(member->value.string_value))
			{
				cp1=RDAstrlen(member->value.string_value);
				ss1 << WW->jsRef() << ".selectionStart = " << cp << ";" << WW->jsRef() << ".selectionEnd = " << cp1 << ";";
			} else {
				ss1 << WW->jsRef() << ".selectionStart = " << cp << ";" << WW->jsRef() << ".selectionEnd = " << cp+1 << ";";
			}
			WW->doJavaScript(ss1.str());
			break;
		case DOLLARS:
		case DOLLARS_NOCENTS:
			cp=1;
			cp1=col;
			ss1 << WW->jsRef() << ".selectionStart = " << cp << ";" << WW->jsRef() << ".selectionEnd = " << cp1 << ";";
			WW->doJavaScript(ss1.str());
			break;
		case SDOUBLEV:
		case SDECIMALV:
		case SSHORTV:
		case SLONGV:
			cp=0;
			cp=col;
			ss1 << WW->jsRef() << ".selectionStart = " << cp << ";" << WW->jsRef() << ".selectionEnd = " << cp << ";";
			WW->doJavaScript(ss1.str());
			break;
		case SHORTV:
		case DECIMALV:
		case DOUBLEV:
		case LONGV:
			cp=0;
			cp1=col;
			ss1 << WW->jsRef() << ".selectionStart = " << cp << ";" << WW->jsRef() << ".selectionEnd = " << cp1 << ";";
			WW->doJavaScript(ss1.str());
			break;
		case SCROLLEDLIST:
		case BOOLNS:
		case BUTTONS:
		case PROGRESS_BAR:
			break;
		default:
			prterr("Error field type [%d] is invalid for Resource [%s].",member->field_type,member->rscrname);
			break;
	}
}
void gainingfocusfunction(RDArmem *member)
{
	RDArsrc *rsrc=(RDArsrc *)member->parent;
	Wt::WFormWidget *wFormW=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG gainingfocusfunction for Resource [%s] on Screen [%s] [%s].",member->rscrname,rsrc->module,rsrc->screen);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	wFormW=(Wt::WFormWidget *)member->w;
	if(NoLosingFocus!=NULL && NoLosingFocus!=member)
	{
		losingfocusfunction(NoLosingFocus);
	}
	NoLosingFocus=member;

	if(rsrc->input_focus!=NULL) Rfree(rsrc->input_focus);
	rsrc->input_focus=stralloc(member->rscrname);
	NoLosingFocus=member;
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG exiting gainingfocusfunction for Resource [%s] on Screen [%s] [%s].",member->rscrname,rsrc->module,rsrc->screen);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	membersetcursor(member);
}
void activatefunction(RDArmem *member)
{
	RDArsrc *rsrc;

	if(member->editable==FALSE || member->user_editable==FALSE) return;
	if(member->sensitive==FALSE || member->user_sensitive==FALSE) return;
	rsrc=(RDArsrc *)member->parent;
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
		if(RDA_CaptureAutoComplete!=NULL && (member->field_type==VARIABLETEXT || member->field_type==PLAINTEXT || member->field_type==ZIPCODE))
		{
			readmember(member);
			RDA_CaptureAutoComplete(rsrc->module,rsrc->screen,member->rscrname,member->value.string_value);
		}
		ExecuteRDArmemFunction(member);
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
}
/*lint +e715 */
/*lint +e715 */
/*lint -e715 */
void toggleactivatefunction(RDArmem *member)
{
	RDArsrc *rsrc;
	Wt::WAbstractToggleButton *aTB=NULL;

	if(member->app_update==TRUE) return;
	member->app_update=TRUE;
	if(NoLosingFocus!=NULL && NoLosingFocus!=member)
	{
		losingfocusfunction(NoLosingFocus);
	}
	NoLosingFocus=member;
	if(member->editable==FALSE || member->user_editable==FALSE) 
	{
		aTB=(Wt::WAbstractToggleButton *)member->w;
		aTB->setChecked((*member->value.string_value ? TRUE:FALSE));
	} else {
		rsrc=(RDArsrc *)member->parent;
#ifdef USE_RDA_DIAGNOSTICS
		if(diaggui || diaggui_field)
		{
			prterr("DIAG toggleactivatefunction for Resource [%s] on Screen [%s] [%s].",member->rscrname,rsrc->module,rsrc->screen);
		}
#endif /* USE_RDA_DIAGNOSTICS */
#ifdef TIMEOUT_CURSORS
		TimeoutCursors(rsrc,TRUE);
#endif
		if(member->editable && member->user_editable)
		{
			ExecuteRDArmemFunction(member);
			if(!isEMPTY(member->transversal_expression))
			{
				ComputeNewTransversalADV(rsrc,rsrc->EvalStr,rsrc->EvalStrArgs,rsrc->SubFunc,rsrc->SubFuncArgs,member->transversal_expression);
			}
		}
#ifdef TIMEOUT_CURSORS
		TimeoutCursors(rsrc,FALSE);
#endif
	}
	member->app_update=FALSE;
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field)
	{
		prterr("DIAG exiting toggleactivatefunction for Resource [%s] on Screen [%s] [%s].",member->rscrname,rsrc->module,rsrc->screen);
	}
#endif /* USE_RDA_DIAGNOSTICS */
}
int xnumperline(RDAscrn *scn,int curnum,int line,char *file)
{
	int x,count=0,boxcount=0;
	RDAwdgt *wdgt;
	char box=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG numperline Find Number Widgets per Line Starting at [%d] in screen [%s] [%s] at line [%d] program [%s].",curnum,scn->module,scn->name,line,file);  
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=(curnum+1);x<scn->numwdgts;++x)
	{
		wdgt=scn->wdgts+x;
		if(wdgt->resource_num!=(-1))
		{
		switch(wdgt->type)
		{
			case 10: /* Frame */
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
			case 9: /* toggle buttons */
			case 11:/* Seperator */
			case 14:/* optional screen */
			case 5: /* labels */
			case 22: /* progress_bar */
				if(!box) ++count;
				break;
			case 1: /* new line */
			case 2: /* end line */
				if(!box)
				{
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui)
					{
						prterr("DIAG numperline Starting at [%d] on Screen [%s] [%s] Returning [%d] at line [%d] program [%s].",curnum,scn->module,scn->name,count,line,file);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					return count;
				}
				break;
			case 20: /* new tab container */
			case 21: /* end tab container */
			case 24: /* new popup menu    */
			case 25: /* end popup menu    */
			case 26: /* new popup menu    */
			case 27: /* end popup menu    */
			case 28: /* end popup menu    */
			case 29: /* end popup menu    */
			case 30: /* end popup menu    */
			case 31: /* end popup menu    */
			case 32: /* new panel         */
			case 33: /* end panel     */
				break;
			case 23: /* tab bar */
				if(!box)
				{
					box=TRUE;
					++count;
				}
				break;
			case 12: /* scrolled window */
			case 3: /* new box */
				if(!box)
				{
					box=TRUE;
					++count;
				}
				++boxcount;
				break;
			case 13: /* end scrolled window */
			case 4: /* end box */
				--boxcount;
				if(boxcount==0) box=FALSE;
				break;
			default:
				prterr("Error Unrecognized Widget Type.");
				break;
		}
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG numperline Starting at [%d] on Screen [%s] [%s] Returning [%d] at line [%d] program [%s].",curnum,scn->module,scn->name,count,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return count;
}
int xfindmaxlines(RDAscrn *scn,int line,char *file)
{
	int x,boxcount=0,count=0,line_height=1;
	RDAwdgt *wdgt;
	char box=FALSE,onaline=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG Find Maximum Lines on Screen [%s] [%s] at line [%d] program [%s].",scn->module,scn->name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=0,wdgt=scn->wdgts;x<scn->numwdgts;++x,++wdgt)
	{
		if(wdgt->resource_num!=(-1))
		{
		switch(wdgt->type)
		{
			case 15: /* custom input fields */
			case 16: /* expenditure fields */
			case 17: /* revenue fields */
			case 18: /* balance sheet fields */
			case 19: /* beginning balance fields */
			case 0: /* fields */
			case 6: /* special controls */
			case 7: /* scrolled list */
			case 8: /* scrolled text */
			case 9: /* togglebutton */
			case 5: /* label */
			case 14: /* optional screen */
			case 11:
			case 20: /* new tab container */
			case 21: /* end tab container */
			case 22: /* progress bar */
			case 23: /* new tab bar */
			case 24: /* new popup menu    */
			case 25: /* end popup menu    */
			case 26: /* new popup menu    */
			case 27: /* end popup menu    */
			case 28: /* end popup menu    */
			case 29: /* end popup menu    */
			case 30: /* new popup menu    */
			case 31: /* end popup menu    */
			case 32: /* new panel */
			case 33: /* end panel */
				break;
			case 1: /* new line */
				if(!box)
				{
					if(x>0) count+=line_height;
					line_height=1;
				}
				onaline=TRUE;
				break;
			case 10:
			case 2: /* end line */
				onaline=FALSE;
				break;
			case 12:
			case 3: /* new box */
				if(onaline==FALSE && boxcount==0) count+=line_height;
				++boxcount;
				box=TRUE;
				break;
			case 13:
			case 4: /* end box */
				--boxcount;
				if(!boxcount) box=FALSE;
				break;
			default:
				prterr("Error Widget Type [%d] is invalid for Widget [%d] on Screen [%s] [%s].",wdgt->type,x,scn->module,scn->name);
				break;
		}
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG findmaxlines for Screen [%s] [%s] Returning [%d] to line [%d] program [%s].",scn->module,scn->name,count,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return count;
}
int xboxmaxcols(RDAscrn *scn,int curnum,int line,char *file)
{
	int x,count=0,hold=0,boxcount=1;
	RDAwdgt *wdgt;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG boxmaxcols Find Maximum Number Columns in Box/Scrolled Wiundow for Screen [%s] [%s] Starting at [%d] at line [%d] program [%s].",scn->module,scn->name,curnum,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=(curnum+1);x<scn->numwdgts;++x)
	{
		wdgt=scn->wdgts+x;
		if(wdgt->resource_num!=(-1))
		{
		switch(wdgt->type)
		{
			case 10: /* frame */
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
			case 22: /* progress bar */
			case 23: /* new tab bar */
				if(boxcount==1) ++count;
				break;
			case 1: /* new line */
			case 2: /* end line */
			case 30: /* new header */
			case 31: /* end header */
			case 32: /* new panel */
			case 33: /* end panel */
				if(boxcount==1)
				{
					if(hold<count) hold=count;
					count=0;
				}
				break;
			case 3: /* new box */
			case 12: /* new scrolled window */
			case 20: /* new tab container */
			case 24: /* new popup menu    */
			case 26: /* new toolbar */
			case 28: /* new table */
				if(boxcount==1) ++count;
				++boxcount;
				break;
			case 4: /* end box */
			case 13: /* end scrolled window */
			case 21: /* end tab container */
			case 25: /* end popup menu    */
			case 27: /* end toolbar */
			case 29: /* end table */
				--boxcount;
				if(boxcount==0)
				{
					if(hold<count) hold=count;
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui)
					{
						prterr("DIAG boxmaxcols For Screen [%s] [%s] starting at [%d] Returning [%d] to line [%d] program [%s].",scn->module,scn->name,curnum,hold,line,file);
					} 
#endif /* USE_RDA_DIAGNOSTICS */
					return(hold);
				}
				break;
			default:
				prterr("Error Widget Type [%d] is invalid for Widget [%d] on Screen [%s] [%s] at line [%d] program [%s].",wdgt->type,x,scn->module,scn->name,x,line,file);
				break;
		}
		}
	}
	if(hold<count) hold=count;
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG boxmaxcols For Screen [%s] [%s] starting at [%d] Returning [%d] to line [%d] program [%s].",scn->module,scn->name,curnum,hold,line,file);
	} 
#endif /* USE_RDA_DIAGNOSTICS */
	return(hold);
}
int xboxmaxrows(RDAscrn *scn,int curnum,int line,char *file)
{
	int x,count=0,wcount=0,boxcount=1;
	RDAwdgt *wdgt;
	
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui) 
	{ 
		prterr("DIAG boxmaxrows Find Maximum Number of Rows in Box on Screen [%s] [%s] Starting with Widget [%d] at line [%d] program [%s].",scn->module,scn->name,curnum,line,file); 
	}
#endif /* USE_RDA_DIAGNOSTICS */
	for(x=(curnum+1);x<scn->numwdgts;++x)
	{
		wdgt=scn->wdgts+x;
		if(wdgt->resource_num!=(-1))
		{
		switch(wdgt->type)
		{
			case 10: /* frame */
				break;
			case 15: /* custom input fields */
			case 16: /* expenditure fields */
			case 17: /* revenue fields */
			case 18: /* balance sheet fields */
			case 19: /* beginnging balance fields */
			case 0: /* fields */
			case 6: /* special controls */
			case 7: /* scrolled list */
			case 8: /* scrolled text */
			case 9: /* toggle button */
			case 14: /* optional screens */
			case 11: /* seperator */
			case 5:  /* label */
			case 22: /* progress_bar */
			case 23: /* new tab bar */
				if(boxcount==1) ++wcount;
				break;
			case 1:  /* new line */
			case 30: /* new header */
			case 32: /* new panel    */
				if(boxcount==1) wcount=0;
				break;
			case 2:  /* end line */
			case 31: /* end header */
			case 33: /* end panel */
				if(boxcount==1) if(wcount>0) ++count;
				break;
			case 3: /* new box */
			case 12: /* new scrolled window */
			case 20: /* new tab container */
			case 24: /* new popup menu    */
			case 26: /* new toolbar  */
			case 28: /* new table    */
				if(boxcount==1) ++wcount;
				++boxcount;
				break;
			case 4: /* end box */
			case 13: /* end scrolled window */
			case 21: /* end tab container */
			case 25: /* end popup menu    */
			case 27: /* end toolbar  */
			case 29: /* end table */
				--boxcount;
				if(boxcount==0)
				{
#ifdef USE_RDA_DIAGNOSTICS
					if(diaggui)
					{
						prterr("DIAG boxmaxrows For Screen [%s] [%s] starting at [%d] Returning [%d] to line [%d] program [%s].",scn->module,scn->name,curnum,count,line,file);
					}
#endif /* USE_RDA_DIAGNOSTICS */
					if(count==0) ++count;
					return(count);
				}
				break;
			default:
				prterr("Error Widget Type [%d] is invalid for Widget [%d] on Screen [%d] [%d] at line [%d] program [%s].",wdgt->type,x,scn->module,scn->name,line,file);
				break;
		}		
		}
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG boxmaxrows For Screen [%s] [%s] starting at [%d] Returning [%d] to line [%d] program [%s].",scn->module,scn->name,count,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	return(count);
}
static void CloseEventFunc(RDArmem *member)
{
	ExecuteRDArmemFunction(member);
}
void RDArsrcKeyPressed(RDArsrc *r,Wt::WKeyEvent &e)
{
	RDArmem *mem=NULL;
	int x=0;

	if(!e.modifiers())
	{
		switch(e.key())
		{
			case Key_F5:
				CyberSupport(r);
				break;
			case Key_F6:
				WikiSupport(r);
				break;
			case Key_Q:
				x=FINDRSC(r,"QUIT");
				if(x>(-1))
				{
					mem=r->rscs+x;
					CloseEventFunc(mem);
				}
				break;
			case Key_S:
				x=FINDRSC(r,"SAVE");
				if(x>(-1))
				{
					mem=r->rscs+x;
				}
				if(mem==NULL)
				{
					x=FINDRSC(r,"SELECT");
					if(x>(-1))
					{
						mem=r->rscs+x;
					}	
				}
				if(mem!=NULL)
				{
					ExecuteRDArmemFunction(mem);
				}
				break;
			case Key_P:
				mem=NULL;
				x=FINDRSC(r,"PRINT");
				if(x>(-1))
				{
					mem=r->rscs+x;
					ExecuteRDArmemFunction(mem);
				}
				if(mem==NULL)
				{
					x=FINDRSC(r,"PRINT BROWSE");
					if(x>(-1))
					{
						mem=r->rscs+x;
					}
				}
				if(mem!=NULL)
				{
					ExecuteRDArmemFunction(mem);
				}

				break;
			case Key_Up:
				x=FINDRSC(r,"TOP");
				if(x>(-1))
				{
					mem=r->rscs+x;
					ExecuteRDArmemFunction(mem);
				}
				break;
			case Key_Down:
				x=FINDRSC(r,"BOTTOM");
				if(x>(-1))
				{
					mem=r->rscs+x;
					ExecuteRDArmemFunction(mem);
				}
				break;	
			default:
				break;
		}
	} else  if(e.modifiers()==Key_Control)
	{
		switch(e.key())
		{
			case Key_A:
				x=FINDRSC(r,"ADD");
				if(x>(-1))
				{
					mem=r->rscs+x;
					ExecuteRDArmemFunction(mem);
				}
				break;
			case Key_B:
				x=FINDRSC(r,"BROWSE");
				if(x>(-1))
				{
					mem=r->rscs+x;
					ExecuteRDArmemFunction(mem);
				}
				break;
			case Key_D:
				x=FINDRSC(r,"DEFAULTS");
				if(x>(-1))
				{
					mem=r->rscs+x;
					ExecuteRDArmemFunction(mem);
				} 
				break;
			case Key_F:
				x=FINDRSC(r,"SEARCH BROWSE");
				if(x>(-1))
				{
					mem=r->rscs+x;
					ExecuteRDArmemFunction(mem);
				} 
				break;
			case Key_H:
				x=FINDRSC(r,"HELP");
				if(x>(-1))
				{
					mem=r->rscs+x;
					ExecuteRDArmemFunction(mem);
				} 
				break;
			case Key_L:
				x=FINDRSC(r,"DEFINE LIST");
				if(x>(-1))
				{
					mem=r->rscs+x;
					ExecuteRDArmemFunction(mem);
				} 
				break;
			case Key_P:
				mem=NULL;
				x=FINDRSC(r,"PRINT");
				if(x>(-1))
				{
					mem=r->rscs+x;
				} 
				if(mem==NULL)
				{
					x=FINDRSC(r,"PRINT BROWSE");
					if(x>(-1)) mem=r->rscs+x;
				}
				if(mem!=NULL) ExecuteRDArmemFunction(mem);
				break;
			case Key_Q:
				x=FINDRSC(r,"QUIT");
				if(x>(-1))
				{
					mem=r->rscs+x;
					ExecuteRDArmemFunction(mem);
				} 
				break;
			case Key_R:
				mem=NULL;
				x=FINDRSC(r,"RUN REPORT");
				if(x>(-1))
				{
					mem=r->rscs+x;
				} 
				if(mem==NULL)
				{
					x=FINDRSC(r,"RESET DEFAULTS");
					if(x>(-1)) mem=r->rscs+x;
				}
				if(mem!=NULL) ExecuteRDArmemFunction(mem);
				break;
			case Key_S:
				mem=NULL;
				x=FINDRSC(r,"SAVE");
				if(x>(-1))
				{
					mem=r->rscs+x;
				} 
				if(mem==NULL)
				{
					x=FINDRSC(r,"SELECT");
					if(x>(-1)) mem=r->rscs+x;
				}
				if(mem!=NULL) ExecuteRDArmemFunction(mem);
				break;
			default:
				break;
		}
	}
}
short isParentOf(Wt::WContainerWidget *parent,Wt::WWidget *child)
{
	Wt::WWidget *w=NULL;

	w=child;
	while(w!=NULL)
	{
		if(parent==w->parent()) return(TRUE);
		w=w->parent();
	}
	return(FALSE);
}
//*---------------------------------------------------------------------------
//	makescrn - Program to make a custom screen from resources.
//
//WAnchor:  ->setTarget() {"TargetSelf" (replace), 
//			"TargetThisWindow" (top level from of the application),
//			"TargetNewWindow" (separate new tab or window)};
//
//* Close Window */
//function close_window()
//{
//	if(confirm("Close Window?"))
//	{
//		close();
//	}
//}
//Close window:  <a href="javascript:close_window();">close</a>
//               <a href="#" onclick="close_window();return false;">close</a>
//
//* Google Docs Renderer */
//<iframe src="http://docs.google.com/gview?url=http://path.com/to/your/pdf.pdf&embedded=true" style="width:600px; height:500px;" frameborder="0"></iframe>
//* pdf.js zip in /work/dct/tmp */
//* PDFObject Example */
//	<script type="text/javascript" srv="pdfobject.js"></script>
//	<script type="text/javascript">
//		window.onload=function(){
//			var success = new PDFObject({ url: "sample.pdf" }).embed();
//		};
//	</scirpt>
//---------------------------------------------------------------------------*/

short xmakescrn(RDArsrc  *scrnrscr,short modalx,short (*EvalFunc)(...),void *EvalFuncArgs,void (*SetOSWidgets)(RDArsrc *),char *(*EvalStr)(...),void *EvalStrArgs,void (*SubFunc)(...),void *SubFuncArgs,RDArsrc  *parent,int line,char *file)
{
	char *dashes,*temp=NULL,*title=NULL;
	int x=0,y=0;
	RDArmem *member=NULL;
#ifdef _DEFER_RENDERING_ATTEMPT_
	Wt::WApplication *myAPP=NULL;
#endif /* _DEFER_RENDERING_ATTEMPT_ */
	Wt::WString *qc=NULL;
	Wt::WWidget *last=NULL,*pnt=NULL;
	Wt::WWidget *pw=NULL,*LIW=NULL;
	Wt::WFormWidget *WF=NULL;
	Wt::WContainerWidget *rC=NULL,*dC=NULL;
	Wt::WKeyEvent *e=NULL;
	RDAScrolledList *SList=NULL;
	int FrameW=0,FrameH=0,dt_W=0,dt_H=0,posx=0,posy=0;
	std::string *s1=NULL;
	char using_js=FALSE,using_ajax=FALSE,*hold_input_field=NULL;
	Wt::WWebWidget *WWeb=NULL;
	Wt::WContainerWidget *TabD=NULL;

	if(RDA_NOGUI==TRUE)
	{
		prterr("Looking for [%s] [%s] and is not found.",(scrnrscr->module!=NULL ? scrnrscr->module:""),(scrnrscr->screen!=NULL ? scrnrscr->screen:""));TRACE;
		prterr("Error Attempting to make a screen when environment setup for NO User Interface!");
		std::exit;
	}
	AT_LEAST_ONE_WINDOW=TRUE;
	scrnrscr->EvalFunc=EvalFunc;
	scrnrscr->EvalFuncArgs=EvalFuncArgs;
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui || diaggui_field) 
	{ 
		prterr("DIAG ADVmakescrn Creating custom screen [%s] [%s] at line [%d] program [%s].",(scrnrscr->module!=NULL ? scrnrscr->module:""),(scrnrscr->screen!=NULL ? scrnrscr->screen:""),line,file); 
	}
#endif /* USE_RDA_DIAGNOSTICS */
	scrnrscr->EvalStr=EvalStr;
	scrnrscr->EvalStrArgs=EvalStrArgs;
	if(scrnrscr->SubFunc==NULL) 
	{
		scrnrscr->SubFunc=SubFunc;
		scrnrscr->SubFuncArgs=SubFuncArgs;
	}
	if(scrnrscr->scn==NULL)
	{
		scrnrscr->scn=RDAscrnNEW(scrnrscr->module,scrnrscr->screen);
		if(findscrnbin(scrnrscr->scn)) 
		{
			prterr("Error Couldn't Find Custom Screen Definition [%s] [%s].",
				(scrnrscr->module!=NULL?scrnrscr->module:""),
				(scrnrscr->screen!=NULL?scrnrscr->screen:""));
			free_scrn(scrnrscr->scn);
			scrnrscr->scn=NULL;
			return(-1);
		} 
	}
	if(SetOSWidgets!=NULL)
	{
/*lint -e746 */
		SetOSWidgets(scrnrscr);
/*lint +e746 */
	}
	setrscnum(scrnrscr,EvalFunc,EvalFuncArgs);
	scrnrscr->modal=modalx;
/*Initialize application*/
	if(!USER_INTERFACE)
	{
/* form window */
		dashes=stripdashes(scrnrscr->scn->name);
		if(!isEMPTY(VERSION_DATE) && !isEMPTY(PROGRAM_NAME))
		{
			temp=Rmalloc(RDAstrlen(dashes)+RDAstrlen(PROGRAM_NAME)+RDAstrlen(VERSION_DATE)+19);
#ifdef RDA_64BITS
			sprintf(temp,"%s%s (64 BITS %s-%s)",(SIMPLE_SCREENS ? "EZ ":""),dashes,PROGRAM_NAME,VERSION_DATE);
#endif
#ifndef RDA_64BITS
			sprintf(temp,"%s%s (32 BITS %s-%s)",(SIMPLE_SCREENS ? "EZ ":""),dashes,PROGRAM_NAME,VERSION_DATE);
#endif
		} else {
			temp=Rmalloc(RDAstrlen(dashes)+7);
			sprintf(temp,"%s%s",(SIMPLE_SCREENS ? "EZ ":""),(dashes!=NULL ? dashes:""));
		}
		rC=RDAMAINWIDGET->root();
#ifdef _USE_GOOGLE_ANALYTICS_ 
		RDAMAINWIDGET->doJavaScript(googleCmd);
#endif /* _USE_GOOGLE_ANALYTICS_  */
		if(rC->count()!=0 && inside_rfkw==TRUE && WindowCount==1)
		{
			if(rfkw_rsrc!=NULL)
			{
				rfkw_rsrc->swidget->clear();
				NULL_RSCS(rfkw_rsrc);
			}
			rfkw_rsrc=NULL;
			inside_rfkw=(-1);
		}
		if(rC->count()==0 && RDAstrcmp(scrnrscr->screen,"DIAGNOSTIC SCREEN") && QN2IT_PARENT_TYPE==0)
		{
			RDAMAINWIDGET->setTitle(scrnrscr->scn->name);
			scrnrscr->primary=NULL;
			scrnrscr->swidget=RDAMAINWIDGET->root();
			scrnrscr->swidget->setMaximumSize(Wt::WLength(100,Wt::WLength::Percentage),Wt::WLength(100,Wt::WLength::Percentage));
			scrnrscr->swidget->setOverflow(WContainerWidget::OverflowVisible,Vertical);	
			scrnrscr->swidget->setOverflow(WContainerWidget::OverflowVisible,Horizontal);	
		} else if(QN2IT_PARENT_TYPE==1 && QN2IT_PARENT!=NULL)
		{
			scrnrscr->primary=NULL;
			scrnrscr->swidget=QN2IT_PARENT;
		} else if(QN2IT_PARENT_TYPE==0)
		{
#ifdef _DEFER_RENDERING_ATTEMPT_
			if(!RDAstrcmp(scrnrscr->screen,"DIAGNOSTIC SCREEN"))
			{
				myAPP=RDAMAINWIDGET->instance();
				myAPP->deferRendering();
			} 	
#endif /* _DEFER_RENDERING_ATTEMPT_ */
			qc=new WString(scrnrscr->scn->name);
			scrnrscr->primary=new WDialog(*qc,(Wt::WObject *)rC);
			if(!modalx)
			{
				scrnrscr->primary->setModal(FALSE);
			} else {
				scrnrscr->primary->setModal(TRUE);
			}
			if(qc!=NULL) qc->~WString();
			scrnrscr->primary->setClosable(FALSE);
			scrnrscr->primary->setTitleBarEnabled(TRUE);
			scrnrscr->primary->setInline(FALSE);
			scrnrscr->primary->setResizable(TRUE);
			scrnrscr->primary->setMaximumSize(Wt::WLength::Auto,Wt::WLength(98,Wt::WLength::Percentage));
			scrnrscr->swidget=scrnrscr->primary->contents();
			scrnrscr->swidget->setOverflow(WContainerWidget::OverflowVisible,Vertical);	
			scrnrscr->swidget->setOverflow(WContainerWidget::OverflowVisible,Horizontal);	
		}
		scrnrscr->has_large_table=FALSE;
/*setup screen*/
		crtwdgts((WWidget *)scrnrscr->swidget,scrnrscr->scn,scrnrscr,temp);
		hold_input_field=stralloc(scrnrscr->input_focus);
		if(temp!=NULL) Rfree(temp);
		if(dashes!=NULL) Rfree(dashes);
		if(scrnrscr->primary!=NULL)
		{
			scrnrscr->primary->show();
		} else {
			scrnrscr->swidget->show();
		}
		++WindowCount;
		if(scrnrscr->lists!=NULL)
		{
			for(y=0,SList=scrnrscr->lists;y<scrnrscr->numlists;++y,++SList)
			{
				if(SList->contype==3)
				{
					x=FINDRSC(scrnrscr,SList->name);
					if(x>(-1))
					{
						member=scrnrscr->rscs+x;
						if(member->w!=NULL)
						{
							ExecuteRDArmemFunction(member);	
						}
					}	
				}	
			}
		}
		if(scrnrscr->input_focus!=NULL) Rfree(scrnrscr->input_focus);
		scrnrscr->input_focus=stralloc(hold_input_field);
		if(hold_input_field!=NULL) Rfree(hold_input_field);
		if(scrnrscr->input_focus!=NULL)
		{
			x=FINDRSC(scrnrscr,scrnrscr->input_focus);
			if(x!=(-1))
			{
				member=scrnrscr->rscs+x;
				if(member->w!=NULL)
				{
					WF=(Wt::WFormWidget *)member->w;
					WF->setFocus(TRUE);
					if(scrnrscr->has_tabbar)
					{
						if(scrnrscr->tabbar!=NULL)
						{
							for(y=0;y<scrnrscr->tabbar->count();++y)
							{
								TabD=scrnrscr->tabbar->widget(y);
								if(isParentOf(TabD,member->w)) 
								{
									scrnrscr->tabbar->setCurrentWidget((Wt::WWidget *)TabD);
									break;
								}
							}
						}
					}
				}
			}
		}
	}
/*
	scrnrscr->swidget->keyPressed().connect(boost::bind(&RDArsrcKeyPressed,scrnrscr,*e));
*/

	if(PP_translate_GUIFUNCALL!=NULL)
	{
/*lint -e746 */
		PP_translate_GUIFUNCALL(scrnrscr);
/*lint +e746 */
	}
	SetAllEditableAndSensitive(scrnrscr);
	return(0);
}
