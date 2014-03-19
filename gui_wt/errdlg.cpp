/* errdlg.c - Error Dialog */
#include<gui.hpp>
#include<guip.hpp>

char USE_SPEECH_SYNTHESIZER=FALSE;

void exitdlg(RDArsrc *parent,ErrorDialogArg *e)
{
	short xexit=FALSE;

	xexit=e->exit;
	if(e->rsrc!=NULL) free_rsrc(e->rsrc);
/*lint -e746 */
	if(e->function!=NULL) e->function();
/*lint +e746 */
	Rfree(e);
}
void xERRORDIALOG(char *name,char *description,void (*function)(...),char exit_req,int line,char *file)
{
	ErrorDialogArg *e;
	RDArmem *member=NULL;
	short y=0;
	FILE *p=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG Creating Error Dialog [%s] at line [%d] program [%s].",name,line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	e=Rmalloc(sizeof(ErrorDialogArg));
	e->function=function;
	e->rsrc=RDArsrcNEW("GUI","ERROR DIALOG SCREEN");
	e->exit=exit_req;
	addstdrsrcnh(e->rsrc,"ERROR NAME",VARIABLETEXT,0,name,FALSE);
	addstdrsrcnh(e->rsrc,"ERROR DESCRIPTION",SCROLLEDTEXT,0,description,FALSE);
	if(exit_req)
	{
		addrfexrsrcnh(e->rsrc,"QUIT",TRUE,exitdlg,e);
	} else {
		addrfkwrsrcnh(e->rsrc,"QUIT",TRUE,exitdlg,e);
	}
	if(makescrn(e->rsrc,TRUE))
	{
		prterr("Can't make ERROR DIALOG SCREEN screen.");
		exitdlg(e->rsrc,e);
	} else {
		if((y=FINDRSC(e->rsrc,"QUIT"))!=(-1))
		{
			member=e->rsrc->rscs+y;
			if(IsWidgetDisplayed(member))
			{
				FINDRSCSETINPUTFOCUS(e->rsrc,"QUIT");
			}
		}
		ForceWindowUpdate(e->rsrc);
	}
	if(USE_SPEECH_SYNTHESIZER)
	{
		p=popen("festival --tts","w");
		if(p!=NULL)
		{
			fprintf(p,"%s",(description!=NULL ? description:""));
			fflush(p);
			pclose(p);	
		} else {
			prterr("Error:  Couldn't open festival.");
		}
	}
}
