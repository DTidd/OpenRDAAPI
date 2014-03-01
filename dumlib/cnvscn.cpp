/* cnvscn.c - Binary functions for Convert Screens */
#include <cnvscn.hpp>

RDAcnvscn *xRDAcnvscnNEW(int line,char *file)
{
	RDAcnvscn *a=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG RDAcnvscnNEW at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	a=Rmalloc(sizeof(RDAcnvscn));
	a->set_which=0;
	a->match_type=0;
	a->pat_scn=FALSE;
	a->scn_pattern=NULL;
	a->rng_scn=FALSE;
	a->from_scn=NULL;
	a->to_scn=NULL;
	a->all_dirs=FALSE;
	a->directory=NULL;
	a->all_libs=FALSE;	
	a->library=NULL;
	a->from.type=0;
	a->from.resource_name=NULL;
	a->from.resource_num=0;
	a->from.label=NULL;
	a->from.pixmap=NULL;
	a->from.XHTML_Label=NULL;
	a->from.rows=0;
	a->from.cols=0;
	a->from.rtype=0;
	a->from.expression=NULL;
	a->from.editable_expression=NULL;
	a->from.sensitive_expression=NULL;
	a->from.transversal_expression=NULL;
	a->to.type=0;
	a->to.resource_name=NULL;
	a->to.resource_num=0;
	a->to.label=NULL;
	a->to.pixmap=NULL;
	a->to.XHTML_Label=NULL;
	a->to.rows=0;
	a->to.cols=0;
	a->to.rtype=0;
	a->to.expression=NULL;
	a->to.editable_expression=NULL;
	a->to.sensitive_expression=NULL;
	a->to.transversal_expression=NULL;
	return(a);
}
void xFreeRDAcnvscn(RDAcnvscn *a,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("DIAG FreeRDAcnvscn at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(a!=NULL)
	{
		if(a->scn_pattern!=NULL) Rfree(a->scn_pattern);
		if(a->from_scn!=NULL) Rfree(a->from_scn);
		if(a->to_scn!=NULL) Rfree(a->to_scn);
		if(a->directory!=NULL) Rfree(a->directory);
		if(a->library!=NULL) Rfree(a->library);
		if(a->from.resource_name!=NULL) Rfree(a->from.resource_name);
		if(a->from.label!=NULL) Rfree(a->from.label);
		if(a->from.pixmap!=NULL) Rfree(a->from.pixmap);
		if(a->from.XHTML_Label!=NULL) Rfree(a->from.XHTML_Label);
		if(a->from.expression!=NULL) Rfree(a->from.expression);
		if(a->from.editable_expression!=NULL) Rfree(a->from.editable_expression);
		if(a->from.sensitive_expression!=NULL) Rfree(a->from.sensitive_expression);
		if(a->from.transversal_expression!=NULL) Rfree(a->from.transversal_expression);
		if(a->to.resource_name!=NULL) Rfree(a->to.resource_name);
		if(a->to.label!=NULL) Rfree(a->to.label);
		if(a->to.pixmap!=NULL) Rfree(a->to.pixmap);
		if(a->to.XHTML_Label!=NULL) Rfree(a->to.XHTML_Label);
		if(a->to.expression!=NULL) Rfree(a->to.expression);
		if(a->to.editable_expression!=NULL) Rfree(a->to.editable_expression);
		if(a->to.sensitive_expression!=NULL) Rfree(a->to.sensitive_expression);
		if(a->to.transversal_expression!=NULL) Rfree(a->to.transversal_expression);
		Rfree(a);
	}
}
static char *str_replace(char *string,char *from,char *to)
{
	int x=0,y=0;
	int offset_start=0;
	int offset_end=0;
	char *tmp=NULL;
	char *new_string=NULL;
	char *new_final_string=NULL;
	int new_string_length=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("str_replace(): INITIAL str_replace(%s,%s,%s)",string,from,to);TRACE;
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	tmp=RDAstrstr(string,from);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("str_replace(): tmp=[%s]",tmp);TRACE; 
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(tmp!=NULL)
	{
		new_string_length=RDAstrlen(string)+RDAstrlen(to)-RDAstrlen(from);
#ifdef USE_RDA_DIAGNOSTICS
		if(diagapps)
		{
			prterr("str_replace(): new_string_length=[%d]",new_string_length);TRACE; 
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		new_string=Rmalloc(new_string_length+1);
		memset(new_string,0,new_string_length+1);
		offset_start=((int)(tmp-(string)));
		offset_end=((int)(offset_start+RDAstrlen(from)));
#ifdef USE_RDA_DIAGNOSTICS
		if(diagapps)
		{
			prterr("str_replace(): string=[%s] / offset_start=[%d] to offset_start=[%d]",string,offset_start,offset_end);TRACE; 
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		for(x=0;x<offset_start;++x)
		{
			new_string[x]=string[x];
		}
#ifdef USE_RDA_DIAGNOSTICS
		if(diagapps)
		{
			prterr("str_replace(): new_string=[%s]",new_string);TRACE; 
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		for(y=0;y<(RDAstrlen(to));++y)
		{
			new_string[x++]=to[y];
		}
#ifdef USE_RDA_DIAGNOSTICS
		if(diagapps)
		{
			prterr("str_replace(): new_string=[%s]",new_string);TRACE; 
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
		if(offset_end<RDAstrlen(string))
		{
			for(y=offset_end;y<(RDAstrlen(string));++y)
			{
				new_string[x++]=string[y];
			}
		}
#ifdef USE_RDA_DIAGNOSTICS
		if(diagapps)
		{
			prterr("str_replace(): new_string=[%s]",new_string);TRACE; 
		}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("str_replace(): INTERMEDIATE VALUE: new_string=[%s]",new_string);TRACE; 
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	tmp=RDAstrstr(new_string,from);
	if(tmp!=NULL)
	{
		new_final_string=str_replace(new_string,from,to);
		if(new_string!=NULL) Rfree(new_string);
	} else {
		new_final_string=stralloc(new_string);
	}
#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("str_replace(): RETURNING: new_final_string=[%s]",new_final_string);TRACE; 
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	return(new_final_string);
}

static APPlib *makeLiblist(RDAcnvscn *csn)
{
	int x,y;
	short fileno=(-1),ef=FALSE;
	APPlib *libavl=NULL,*liblist=NULL;
	char *temp=NULL,deleteflag=FALSE,*user=NULL,*tmp=NULL;
	char *temp1=NULL;

	libavl=APPlibNEW();
	liblist=APPlibNEW();
	if(csn->all_dirs)
	{
		temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+12);
#ifndef WIN32
		sprintf(temp,"%s/rda",CURRENTDIRECTORY);
#endif
#ifdef WIN32
		sprintf(temp,"%s\\rda",CURRENTDIRECTORY);
#endif
		addAPPlib(libavl,temp);
#ifndef WIN32
		sprintf(temp,"%s/site",CURRENTDIRECTORY);
#endif
#ifdef WIN32
		sprintf(temp,"%s\\site",CURRENTDIRECTORY);
#endif
		addAPPlib(libavl,temp);
		if(temp!=NULL) Rfree(temp);
		fileno=OPNNRD("SECURITY","USERS");
		if(fileno!=(-1))
		{
			ef=FRSNRD(fileno,1);
			while(!ef)
			{
				FINDFLDGETCHAR(fileno,"DELETEFLAG",&deleteflag);
				if(!deleteflag)
				{
					FINDFLDGETSTRING(fileno,"USER IDENTIFICATION",&user);
					temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(user)+10);
#ifndef WIN32
					sprintf(temp,"%s/%s",CURRENTDIRECTORY,user);
#endif
#ifdef WIN32
					sprintf(temp,"%s\\%s",CURRENTDIRECTORY,user);
#endif
					addAPPlib(libavl,temp);
					if(temp!=NULL) Rfree(temp);
					
				}
				ef=NXTNRD(fileno,1);
			}
			if(user!=NULL) Rfree(user);
			CLSNRD(fileno);
		}
	} else {
		addAPPlib(libavl,csn->directory);
	}
	if(csn->all_libs==TRUE)
	{
		for(y=0;y<libavl->numlibs;++y)
		{
			temp=Rmalloc(RDAstrlen(libavl->libs[y])+5);
#ifndef WIN32
			sprintf(temp,"%s/",libavl->libs[y]);
#endif
#ifdef WIN32
			sprintf(temp,"%s\\",libavl->libs[y]);
#endif
			for(x=0;findfile(temp,"*.SCN",&tmp,(int)x+1);++x) 
			{
				temp1=Rmalloc(RDAstrlen(temp)+RDAstrlen(tmp)+2);
				sprintf(temp1,"%s%s",temp,tmp);
				addAPPlib(liblist,temp1);
				if(temp1!=NULL) Rfree(temp1);
			}
			if(temp!=NULL) Rfree(temp);
		}
	} else {
		for(y=0;y<libavl->numlibs;++y)
		{
			temp=Rmalloc(RDAstrlen(libavl->libs[y])+RDAstrlen(csn->library)+8);
#ifndef WIN32
			sprintf(temp,"%s/%s.SCN",libavl->libs[y],csn->library);
#endif
#ifdef WIN32
			sprintf(temp,"%s\\%s.SCN",libavl->libs[y],csn->library);
#endif
			addAPPlib(liblist,temp);
			if(temp!=NULL) Rfree(temp);
		}
	}
	if(libavl!=NULL) freeapplib(libavl);
	return(liblist);
}
void xExecute_RDAcnvscn(RDAcnvscn *csn,short do_diag,int line,char *file)
{
	int y,x;
	int q=0;
	char *e=NULL;
	char *temp=NULL;
	APPlib *scrlist=NULL,*liblist=NULL;
	RDAscrn *scn=NULL;
	RDArsrc *tmprsrc=NULL;
	RDAwdgt *wdgt,*chgwdgt=NULL;
	short changedit=FALSE,test1=FALSE;


#ifdef USE_RDA_DIAGNOSTICS
	if(diagapps)
	{
		prterr("xExecute_RDAcnvscn(): INITIAL");TRACE;
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	liblist=makeLiblist(csn);
	if(liblist!=NULL)
	{
	for(q=0;q<liblist->numlibs;++q)
	{
	scrlist=getunsortedlibnames(liblist->libs[q],FALSE);
	x=0;
	if(scrlist!=NULL)
	{
		if(do_diag==TRUE)
		{
		e=Rmalloc(RDAstrlen(liblist->libs[q])+20);
		sprintf(e,"Converting Library %s",liblist->libs[q]);
		tmprsrc=odiagscrn("DIAGNOSTIC SCREEN","GUI",e,NULL,scrlist->numlibs);
		if(e!=NULL) Rfree(e);
		if(tmprsrc!=NULL)
		{
			if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE)) ForceWindowUpdate(tmprsrc);
		}
		}
		for(y=0;y<scrlist->numlibs;++y)
		{
		if((csn->rng_scn==FALSE) || ((csn->rng_scn==TRUE) && (RDAstrcmp(scrlist->libs[y],csn->from_scn)>=0) && (RDAstrcmp(scrlist->libs[y],csn->to_scn)<=0)))
		{
		if((csn->pat_scn==FALSE) || ((csn->pat_scn==TRUE) && patmatch(csn->scn_pattern,scrlist->libs[y])))
		{
		
			scn=RDAscrnNEW(liblist->libs[q],scrlist->libs[y]);
			changedit=FALSE;
			if(!getscrnbin(liblist->libs[q],scn,FALSE))
			{
				if(scn->wdgts!=NULL)
				{
					for(x=0,wdgt=scn->wdgts;x<scn->numwdgts;++x,++wdgt)
					{
						chgwdgt=NULL;
						switch(csn->set_which)
						{
							default:
							case 0:
								chgwdgt=scn->wdgts+x;
								break;
							case 1:
								if(x>0)
								{
									chgwdgt=scn->wdgts+(x-1);
								}
								break;
							case 2:
								if((x+1)<scn->numwdgts)
								{		
									chgwdgt=scn->wdgts+(x+1);
								}
								break;
							case 3:
								if(x>1)
								{
									chgwdgt=scn->wdgts+(x-2);
								}
								break;
							case 4:
								if((x+2)<scn->numwdgts)
								{		
									chgwdgt=scn->wdgts+(x+2);
								}
								break;
						}
						if(csn->match_type==3) /* Pattern Match Name */
						{
							if((csn->from.type!=5) && !isEMPTY(csn->from.resource_name) && !isEMPTY(wdgt->resource_name)) test1=patmatch(csn->from.resource_name,wdgt->resource_name); 
							else if((csn->from.type==5) && !isEMPTY(csn->from.label) && !isEMPTY(wdgt->label)) test1=patmatch(csn->from.label,wdgt->label);
							else test1=FALSE;
							if(test1==TRUE && chgwdgt->type==csn->to.type)
							{
						
								chgwdgt->type=csn->to.type;
								chgwdgt->rtype=csn->to.rtype;
								if(chgwdgt->label!=NULL) Rfree(chgwdgt->label);
								if(chgwdgt->pixmap!=NULL) Rfree(chgwdgt->pixmap);
								if(chgwdgt->XHTML_Label!=NULL) Rfree(chgwdgt->XHTML_Label);
								if(!isEMPTY(csn->to.resource_name))
								{
									if(chgwdgt->resource_name!=NULL) Rfree(chgwdgt->resource_name);
									chgwdgt->resource_name=stralloc(csn->to.resource_name);
								}
								if(csn->to.label!=NULL)
									
									chgwdgt->label=stralloc(csn->to.label);
								if(((chgwdgt->type==5||chgwdgt->type==9)&&chgwdgt->rtype==1)||
								   ((chgwdgt->type==6||chgwdgt->type==14)&&chgwdgt->rtype==5))
								{
									if(csn->to.pixmap!=NULL)
										chgwdgt->pixmap=stralloc(csn->to.pixmap);
								} 
								if(csn->to.XHTML_Label!=NULL)
									chgwdgt->XHTML_Label=stralloc(csn->to.XHTML_Label);
								chgwdgt->rows=csn->to.rows;
								chgwdgt->cols=csn->to.cols;
								if(chgwdgt->expression!=NULL) Rfree(chgwdgt->expression);
								chgwdgt->expression=stralloc(csn->to.expression);
								if(chgwdgt->editable_expression!=NULL) Rfree(chgwdgt->editable_expression);
								chgwdgt->editable_expression=stralloc(csn->to.editable_expression);
								if(chgwdgt->sensitive_expression!=NULL) Rfree(chgwdgt->sensitive_expression);
								chgwdgt->sensitive_expression=stralloc(chgwdgt->sensitive_expression);
								if(chgwdgt->transversal_expression!=NULL) Rfree(chgwdgt->transversal_expression);
								chgwdgt->transversal_expression=stralloc(csn->to.transversal_expression);
								changedit=TRUE;
							}
						} else if(csn->match_type==0) /* Match Name Only */ 
						{
							if(((csn->from.type!=5) && !RDAstrcmp(wdgt->resource_name,csn->from.resource_name)) || ((csn->from.type==5) && !RDAstrcmp(wdgt->label,csn->from.label)))
							{
						

								if(chgwdgt->resource_name!=NULL)
									Rfree(chgwdgt->resource_name);
								chgwdgt->resource_name=NULL;
								chgwdgt->type=csn->to.type;
								chgwdgt->rtype=csn->to.rtype;
								if(chgwdgt->label!=NULL) Rfree(chgwdgt->label);
								if(chgwdgt->pixmap!=NULL) Rfree(chgwdgt->pixmap);
								if(chgwdgt->XHTML_Label!=NULL) Rfree(chgwdgt->XHTML_Label);
								if(csn->to.resource_name!=NULL)
									chgwdgt->resource_name=stralloc(csn->to.resource_name);
								if(csn->to.label!=NULL)
									chgwdgt->label=stralloc(csn->to.label);
								if(((chgwdgt->type==5||chgwdgt->type==9)&&chgwdgt->rtype==1)||
								   ((chgwdgt->type==6||chgwdgt->type==14)&&chgwdgt->rtype==5))
								{
									if(csn->to.pixmap!=NULL)
										chgwdgt->pixmap=stralloc(csn->to.pixmap);
								} 
								if(csn->to.XHTML_Label!=NULL)
									chgwdgt->XHTML_Label=stralloc(csn->to.XHTML_Label);
								chgwdgt->rows=csn->to.rows;
								chgwdgt->cols=csn->to.cols;
								if(chgwdgt->expression!=NULL) Rfree(wdgt->expression);
								chgwdgt->expression=stralloc(csn->to.expression);
								if(chgwdgt->editable_expression!=NULL) Rfree(wdgt->editable_expression);
								chgwdgt->editable_expression=stralloc(csn->to.editable_expression);
								if(chgwdgt->sensitive_expression!=NULL) Rfree(chgwdgt->sensitive_expression);
								chgwdgt->sensitive_expression=stralloc(chgwdgt->sensitive_expression);
								if(chgwdgt->transversal_expression!=NULL) Rfree(chgwdgt->transversal_expression);
								chgwdgt->transversal_expression=stralloc(csn->to.transversal_expression);
								changedit=TRUE;
							}
						} else if(csn->match_type==1) /* Match All Resources */
						{
							if(!RDAstrcmp(wdgt->resource_name,csn->from.resource_name) && 
								!RDAstrcmp(wdgt->label,csn->from.label) && 
								!RDAstrcmp(wdgt->pixmap,csn->from.pixmap) && 
								!RDAstrcmp(wdgt->XHTML_Label,csn->from.XHTML_Label) && 
								wdgt->type==csn->from.type && 
								wdgt->rows==csn->from.rows && 
								wdgt->cols==csn->from.cols &&
								wdgt->rtype==csn->from.rtype)
							{
								if(chgwdgt->resource_name!=NULL)
									Rfree(chgwdgt->resource_name);
								chgwdgt->resource_name=NULL;
								chgwdgt->type=csn->to.type;
								chgwdgt->rtype=csn->to.rtype;
								if(chgwdgt->label!=NULL) Rfree(chgwdgt->label);
								if(chgwdgt->pixmap!=NULL) Rfree(chgwdgt->pixmap);
								if(chgwdgt->XHTML_Label!=NULL) Rfree(chgwdgt->XHTML_Label);
								if(csn->to.resource_name!=NULL)
									chgwdgt->resource_name=stralloc(csn->to.resource_name);
								if(csn->to.label!=NULL)
									chgwdgt->label=stralloc(csn->to.label);
								if(((chgwdgt->type==5||chgwdgt->type==9)&&chgwdgt->rtype==1)||
								   ((chgwdgt->type==6||chgwdgt->type==14)&&chgwdgt->rtype==5))
								{
									if(csn->to.pixmap!=NULL)
										chgwdgt->pixmap=stralloc(csn->to.pixmap);
								} 
								if(csn->to.XHTML_Label!=NULL)
									chgwdgt->XHTML_Label=stralloc(csn->to.XHTML_Label);
								chgwdgt->rows=csn->to.rows;
								chgwdgt->cols=csn->to.cols;
								if(chgwdgt->expression!=NULL) Rfree(chgwdgt->expression);
								chgwdgt->expression=stralloc(csn->to.expression);
								if(chgwdgt->editable_expression!=NULL) Rfree(chgwdgt->editable_expression);
								chgwdgt->editable_expression=stralloc(csn->to.editable_expression);
								if(chgwdgt->sensitive_expression!=NULL) Rfree(chgwdgt->sensitive_expression);
								chgwdgt->sensitive_expression=stralloc(chgwdgt->sensitive_expression);
								if(chgwdgt->transversal_expression!=NULL) Rfree(chgwdgt->transversal_expression);
								chgwdgt->transversal_expression=stralloc(csn->to.transversal_expression);
								changedit=TRUE;
							}
						} else if(csn->match_type==2) /* Match Name Substring Only */
						{
							if(((csn->from.type!=5) && (!isEMPTY(wdgt->resource_name)) && (!isEMPTY(csn->from.resource_name))) || ((csn->from.type==5) && (!isEMPTY(wdgt->label)) && (!isEMPTY(csn->from.label))))
							{
							if(((csn->from.type!=5) && RDAstrstr(wdgt->resource_name,csn->from.resource_name)!=NULL) || ((csn->from.type==5) && RDAstrstr(wdgt->label,csn->from.label)!=NULL))
							{
/*
								chgwdgt->type=csn->to.type;
								chgwdgt->rtype=csn->to.rtype;
								if(chgwdgt->label!=NULL) Rfree(chgwdgt->label);
								if(chgwdgt->pixmap!=NULL) Rfree(chgwdgt->pixmap);
								if(chgwdgt->XHTML_Label!=NULL) Rfree(chgwdgt->XHTML_Label);
*/
								if(!isEMPTY(csn->to.resource_name))
								{
									temp=str_replace(chgwdgt->resource_name,csn->from.resource_name,csn->to.resource_name);
									if(chgwdgt->resource_name!=NULL)
									{
										Rfree(chgwdgt->resource_name);
										chgwdgt->resource_name=NULL;
									}
									if(!isEMPTY(temp))
									{
										chgwdgt->resource_name=stralloc(temp);
										if(temp!=NULL) Rfree(temp);
									}
								}
/*
								if(csn->to.label!=NULL)
								{
									chgwdgt->label=stralloc(csn->to.label);
								}
								if(((chgwdgt->type==5||chgwdgt->type==9)&&chgwdgt->rtype==1)||
								   ((chgwdgt->type==6||chgwdgt->type==14)&&chgwdgt->rtype==5))
								{
									if(csn->to.pixmap!=NULL)
									{
										chgwdgt->pixmap=stralloc(csn->to.pixmap);
									}
								} 
								if(csn->to.XHTML_Label!=NULL)
								{
									chgwdgt->XHTML_Label=stralloc(csn->to.XHTML_Label);
								}
								if(chgwdgt->rows!=0) chgwdgt->rows=csn->to.rows;
								if(chgwdgt->cols!=0) chgwdgt->cols=csn->to.cols;
*/
								if(!isEMPTY(csn->to.expression))
								{
									temp=str_replace(chgwdgt->expression,csn->from.expression,csn->to.expression);
									if(chgwdgt->expression!=NULL)
									{
										Rfree(chgwdgt->expression);
										chgwdgt->expression=NULL;
									}
									if(!isEMPTY(temp))
									{
										chgwdgt->expression=stralloc(temp);
										if(temp!=NULL) Rfree(temp);
									}
								}
								if(!isEMPTY(csn->to.editable_expression))
								{
									temp=str_replace(chgwdgt->editable_expression,csn->from.editable_expression,csn->to.editable_expression);
									if(chgwdgt->editable_expression!=NULL)
									{
										Rfree(chgwdgt->editable_expression);
										chgwdgt->editable_expression=NULL;
									}
									if(!isEMPTY(temp))
									{
										chgwdgt->editable_expression=stralloc(temp);
										if(temp!=NULL) Rfree(temp);
									}
								}
								if(!isEMPTY(csn->to.sensitive_expression))
								{
									temp=str_replace(chgwdgt->sensitive_expression,csn->from.sensitive_expression,csn->to.sensitive_expression);
									if(chgwdgt->sensitive_expression!=NULL)
									{
										Rfree(chgwdgt->sensitive_expression);
										chgwdgt->sensitive_expression=NULL;
									}
									if(!isEMPTY(temp))
									{
										chgwdgt->sensitive_expression=stralloc(temp);
										if(temp!=NULL) Rfree(temp);
									}
								}
								if(!isEMPTY(csn->to.transversal_expression))
								{
									temp=str_replace(chgwdgt->transversal_expression,csn->from.transversal_expression,csn->to.transversal_expression);
									if(chgwdgt->transversal_expression!=NULL)
									{
										Rfree(chgwdgt->transversal_expression);
										chgwdgt->transversal_expression=NULL;
									}
									if(!isEMPTY(temp))
									{
										chgwdgt->transversal_expression=stralloc(temp);
										if(temp!=NULL) Rfree(temp);
									}
								}
								changedit=TRUE;
							}
							}
						}
					}
				}
			}
			if(changedit) writescrnbin(liblist->libs[q],scn);
			if(tmprsrc!=NULL) update_diagnostic(tmprsrc, (changedit ? TRUE:FALSE));
			if(scn!=NULL) free_scrn(scn);
		}
		}
		}
	}
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
	}
	if(scrlist!=NULL) freeapplib(scrlist);
	}
	freeapplib(liblist);
	}
}
