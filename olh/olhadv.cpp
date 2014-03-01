/* olhadv.c - Advanced On-Line Help Functions */
#include <olh.hpp>

static void makeconnecttype(char ***connecttype)
{
	*connecttype=Rmalloc(3*sizeof(char *));
	*(connecttype[0]+0)=stralloc("  And  ");
	*(connecttype[0]+1)=stralloc("  Or   ");
	*(connecttype[0]+2)=stralloc("But Not");
}
static void freeconnecttype(char ***connecttype)
{
	short x;

	if(*connecttype!=NULL)
	{
		for(x=0;x<3;++x) Rfree(*(connecttype[0]+x));
		Rfree(*connecttype);
		*connecttype=NULL;
	}
}
void quit_search_help(RDArsrc *parent,SearchHelp *h)
{
	free_rsrc(h->r);
	freeconnecttype(&h->connecttype);
	if(h!=NULL) Rfree(h);
}
void quit_dosearch_help(RDArsrc *parent,SearchHelp *h)
{
	free_rsrc(h->r);
	if(h!=NULL) Rfree(h);
}
void do_search(RDArsrc *parent,SearchHelp *h)
{
	RDArsrc *r=NULL;

	addrfkwrsrc(r,"OK SECTION",oksection,s);
	addrfkwrsrc(r,"QUIT",TRUE,quit_dosearch_help,s);
	addbtnrsrc(r,"HELP",TRUE,screenhelp,NULL);
	if(makescrn(r,TRUE)) 
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom help SELECT SECTION. Check to see the help is available. If it is, call your installer.",NULL,FALSE);
		quit_dosearch_help(s);
	}
	if(hlp!=NULL) Rfree(hlp);
	quit_search_help(h);
}
void searchhelp(RDArsrc *parent,WriteScnHelp *hlp)
{
	RDArsrc *r=NULL;
	char f=FALSE,**connecttype;
	SearchHelp *s;

	s=Rmalloc(sizeof(SearchHelp));
	r=RDArsrcNEW("OLHELP","SEARCH HELP");
	s->r=r;
	makeconnecttype(&connecttype);
	s->connecttype=connecttype;
	addstdrsrc(r,"ALL MANUALS",BOOLNS,1,&f,TRUE);
	addstdrsrc(r,"SEARCH VALUE #1",VARIABLETEXT,0,NULL,TRUE);
	addlstrsrc(r,"CONDITIONAL #1",0,TRUE,NULL,3,&connecttype,NULL);
	addstdrsrc(r,"SEARCH VALUE #2",VARIABLETEXT,0,NULL,TRUE);
	addlstrsrc(r,"CONDITIONAL #2",0,TRUE,NULL,3,&connecttype,NULL);
	addstdrsrc(r,"SEARCH VALUE #3",VARIABLETEXT,0,NULL,TRUE);
	addlstrsrc(r,"CONDITIONAL #3",0,TRUE,NULL,3,&connecttype,NULL);
	addstdrsrc(r,"SEARCH VALUE #4",VARIABLETEXT,0,NULL,TRUE);
	addlstrsrc(r,"CONDITIONAL #4",0,TRUE,NULL,3,&connecttype,NULL);
	addstdrsrc(r,"SEARCH VALUE #5",VARIABLETEXT,0,NULL,TRUE);
	addlstrsrc(r,"CONDITIONAL #5",0,TRUE,NULL,3,&connecttype,NULL);
	addstdrsrc(r,"SEARCH VALUE #6",VARIABLETEXT,0,NULL,TRUE);
	addlstrsrc(r,"CONDITIONAL #6",0,TRUE,NULL,3,&connecttype,NULL);
	addstdrsrc(r,"SEARCH VALUE #7",VARIABLETEXT,0,NULL,TRUE);
	addlstrsrc(r,"CONDITIONAL #7",0,TRUE,NULL,3,&connecttype,NULL);
	addstdrsrc(r,"SEARCH VALUE #8",VARIABLETEXT,0,NULL,TRUE);
	addrfkwrsrc(r,"SEARCH",TRUE,do_search,s);
	addrfkwrsrc(r,"QUIT",TRUE,quit_search_help,s);
	addbtnrsrc(r,"HELP",TRUE,screenhelp,NULL);
	if(makescrn(r,TRUE)) 
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom help SEARCH HELP. Check to see the help is available. If it is, call your installer.",NULL,FALSE);
		quit_search_help(s);
	}
	if(hlp!=NULL) Rfree(hlp);
}
