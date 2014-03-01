/* Warning Dialog */
#include <mix.hpp>


void canceldlg(RDArsrc *parent,WarningDialogArg *w)
{
	if(w!=NULL)
	{
		if(w->rsrc!=NULL)
		{
			killwindow(w->rsrc);
			free_rsrc(w->rsrc);
		}
		if(*w->cancfunc!=NULL) 
		{
/*lint -e746 */
			switch(w->type)
			{
				case 3: /* 3 arguments to be passed to functions */
					w->cancfunc(w->arglist1,w->arglist2,w->arglist3);
					break;
				case 2: /* 2 arguments to be passed to functions */
					w->cancfunc(w->arglist1,w->arglist2);
					break;
				case 1: /* 1 argument to be passed to functions */
					w->cancfunc(w->arglist1);
					break;
				default : 
				case 0: /* no arguments */
					w->cancfunc();
					break;
			}
/*lint +e746 */
		}
		Rfree(w);
		w=NULL;
	}
	return;
}
void continuedlg(RDArsrc *parent,WarningDialogArg *w)
{
	if(w!=NULL)
	{
		if(w->rsrc!=NULL)
		{
			killwindow(w->rsrc);
			free_rsrc(w->rsrc);
		}
		if(*w->contfunc!=NULL) 
		{
/*lint -e746 */
			switch(w->type)
			{
				case 3: /* 3 arguments to be passed to functions */
					w->contfunc(w->arglist1,w->arglist2,w->arglist3);
					break;
				case 2: /* 2 arguments to be passed to functions */
					w->contfunc(w->arglist1,w->arglist2);
					break;
				case 1: /* 1 argument to be passed to functions */
					w->contfunc(w->arglist1);
					break;
				default : 
				case 0: /* no arguments */
					w->contfunc();
					break;
			}
/*lint +e746 */
		}
		Rfree(w);
		w=NULL;
	}
	return;
}
void xWARNINGDIALOG(char *screen_name,char *header,char *message,void (*contfunc)(...),void (*cancfunc)(...),char fyi,char arg_type,void *arglist1,void *arglist2,void *arglist3,int line,char *file)
{
	WarningDialogArg *w;
	RDArmem *member=NULL;
	short y=0;
	char *s=NULL;
	FILE *p=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix || diagmix_field)
	{
		prterr("DIAG WARNINGDIALOG Screen [%s] Header [%s] Message [%s] at line [%d] program [%s].",screen_name,header,message,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	w=Rmalloc(sizeof(WarningDialogArg));
	w->contfunc=contfunc;
	w->cancfunc=cancfunc;
	w->type=arg_type;
	w->arglist1=NULL;
	w->arglist2=NULL;
	w->arglist3=NULL;
	switch(w->type)
	{
		default : 
			prterr("Error ADVWARNINGDIALOG called with invalid type argument of [%d], valid range is 0 to 3.",w->type);
			break;
		case 3: /* 3 arguments to be passed to functions */
			w->arglist3=arglist3;
			/* fall through */
		case 2: /* 2 arguments to be passed to functions */
			w->arglist2=arglist2;
			/* fall through */
		case 1: /* 1 argument to be passed to functions */
			w->arglist1=arglist1;
			/* fall through */
		case 0: /* no arguments */
			break;
	}
	w->rsrc=RDArsrcNEW("GUI",screen_name);
	addstdrsrc(w->rsrc,"WARNING HEADER",VARIABLETEXT,0,header,FALSE);
	addstdrsrc(w->rsrc,"WARNING MESSAGE",SCROLLEDTEXT,0,message,FALSE);
	addbtnrsrc(w->rsrc,"CONTINUE",TRUE,continuedlg,w);
	if(!fyi) addbtnrsrc(w->rsrc,"QUIT",TRUE,canceldlg,w);
	addbtnrsrc(w->rsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(w->rsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	if(ADVmakescrn(w->rsrc,TRUE))
	{
		prterr("Error can't make custom screen [%s] [%s].",w->rsrc->module,w->rsrc->screen);
	} else {
		if((y=FINDRSC(w->rsrc,"QUIT"))!=(-1))
		{
			member=w->rsrc->rscs+y;
			if(IsWidgetDisplayed(member))
			{
				FINDRSCSETINPUTFOCUS(w->rsrc,"QUIT");
			}
		}
		ForceWindowUpdate(w->rsrc);
	}
	if(USE_SPEECH_SYNTHESIZER)
	{
		s=Rmalloc(RDAstrlen(header)+RDAstrlen(message)+90);
		sprintf(s,"%s\r\n",(message!=NULL ? message:""));
		p=popen("festival --tts","w");
		if(p!=NULL)
		{
			fprintf(p,"%s",s);
			pclose(p);
		} else {
			prterr("Error:  Couldn't open festival.");
		}
		if(s!=NULL) Rfree(s);
	}
}
