/*lint -library */
#ifndef WIN32
#define __NAM__ "custdef.lnx"
#endif
#ifdef WIN32
#define __NAM__ "custdef.exe"
#endif
#include <app.hpp>
#include <cnvscn.hpp>
/*ADDLIB cscn */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="GUI";
APPlib *CustomFields=NULL;
APPlib *makeSCNDEFLiblist(void);
static void printheader(RDA_PFILE *,int *,int *,char *,char *,char *);
static void checkheader(RDA_PFILE *,int *,int *,char *,char *);
static char *ContainsACustomField(char *);
char *RDA_FormatOutputDefaultCustom(char *,char *,char *);
static void ChangeScreenDefault_ContainsCustom(RDA_PFILE *,RDAdefault *,char *,char *,char *,int *,int *);

static void quitfunc(RDArsrc *r)
{
	if(r!=NULL) free_rsrc(r);
	if(CustomFields!=NULL) freeapplib(CustomFields);
	ShutdownSubsystems();
}
static APPlib *makeLiblist()
{
	short fileno=(-1),ef=FALSE;
	APPlib *libavl=NULL;
	char *temp=NULL,deleteflag=FALSE,*user=NULL;

	libavl=APPlibNEW();
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
	return(libavl);
}
static void checkheader(RDA_PFILE *fp,int *lines,int *pages,char *from_mod,char *to_mod)
{
	char *temp=NULL;

	if(*lines>60 || (*lines==0 && *pages==0))
	{
		if(*lines>60)
		{
			RDA_fprintf(fp,"\f");
		}
		if(XPERT_SETUP->software_type==2)
		{
			temp=stralloc("MBGUI SCREEN WIDGET CUSTOM VALUES PREVIEW"); 
			printheader(fp,lines,pages,from_mod,to_mod,temp);
			if(temp!=NULL) Rfree(temp);
		} else {
			temp=stralloc("XPERT SCREEN WIDGET CUSTOM VALUES PREVIEW"); 
			printheader(fp,lines,pages,from_mod,to_mod,temp);
			if(temp!=NULL) Rfree(temp);
		}
	}
}
static void printheader(RDA_PFILE *fp,int *lines,int *pages,char *from_mod,char *to_mod,char *header)
{
	char *date=NULL,*timex=NULL;
	int lead_ws=0,tail_ws=0;
	unsigned line_length=131;

	date=GETCURRENTDATE10();
	timex=GETCURRENTTIME();
	RDA_fprintf(fp,"\r\n");
	*lines=1;
	lead_ws=(int)(RDAroundup((double)(line_length-(2+RDAstrlen(header)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(2+RDAstrlen(header)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(header)+2+16+lead_ws+tail_ws)>line_length) header[(line_length-16-lead_ws-tail_ws)]=0;
	RDA_fprintf(fp,"%*s %s %*sPage: %*d\r\n",
		lead_ws,"",header,tail_ws,"",10,++*pages);
	++*lines;
	if(isEMPTY(from_mod) && isEMPTY(to_mod))
	{
		lead_ws=(int)(RDAroundup((double)(line_length-(10+3))/2));
		if(lead_ws<0) lead_ws=0;
		tail_ws=(int)(Rint(((double)(line_length-(10+3))/2)-16));
		if(tail_ws<0) tail_ws=0;
		RDA_fprintf(fp,"%*s Module: ALL %*sDate: %s\r\n",
			lead_ws,"",tail_ws,"",date);
		++*lines;
	} else {
		lead_ws=(int)(RDAroundup((double)(line_length-(23+RDAstrlen(from_mod)+RDAstrlen(to_mod)))/2));
		if(lead_ws<0) lead_ws=0;
		tail_ws=(int)(Rint(((double)(line_length-(23+RDAstrlen(from_mod)+RDAstrlen(to_mod)))/2)-16));
		if(tail_ws<0) tail_ws=0;
		RDA_fprintf(fp,"%*s Module: from (%s) to (%s) %*sDate: %s\r\n",
			lead_ws,"",(from_mod!=NULL?from_mod:""),(to_mod!=NULL?to_mod:""),tail_ws,"",date);
		++*lines;
	}
	lead_ws=(int)(RDAroundup(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)-16));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(USERLOGIN)+16+15+lead_ws+tail_ws)>line_length)
	{
		RDA_fprintf(fp,"%*s %s %*sTime:   %s\r\n",
			(int)(RDAroundup(((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2))),"",
			USERLOGIN,
			(int)(Rint((((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2)-16))),"",
			timex);
		++*lines;
	} else {
		RDA_fprintf(fp,"%*s Executed By: %s %*sTime:   %s\r\n",
			lead_ws,"",USERLOGIN,tail_ws,"",timex);
		++*lines;
	}
	RDA_fprintf(fp,"\r\n");
	++*lines;
	RDA_fprintf(fp,"    IDENTIFIER                               OFFSET REASON                                                                         \r\n");
	++*lines;
	RDA_fprintf(fp,"    ---------------------------------------- ------ -------------------------------------------------------------------------------\r\n");
	++*lines;
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
}
char *RDA_FormatOutputDefaultCustom(char *id,char *offset1,char *reason)
{
	char *temp=NULL;
	short id_length=40;
	char id_string[41];
	short offset1_length=6;
	char offset1_string[7];
	short reason_length=79;
	char reason_string[80];
	int x=0;

	memset(id_string,0,41);
	for(x=0;x<id_length;++x)
	{
		if(x<RDAstrlen(id))
		{
			id_string[x]=id[x];
		} else {
			id_string[x]=' ';
		}
	}
	memset(offset1_string,0,7);
	for(x=0;x<offset1_length;++x)
	{
		if(x<RDAstrlen(offset1))
		{
			offset1_string[x]=offset1[x];
		} else {
			offset1_string[x]=' ';
		}
	}
	memset(reason_string,0,80);
	for(x=0;x<reason_length;++x)
	{
		if(x<RDAstrlen(reason))
		{
			reason_string[x]=reason[x];
		} else {
			reason_string[x]=' ';
		}
	}	
	temp=Rmalloc(RDAstrlen(id_string)+RDAstrlen(offset1_string)+RDAstrlen(reason_string)+6+1);
	sprintf(temp,"    %s %s %s",id_string,offset1_string,reason_string);
	return(temp);
}
static char *ContainsACustomField(char *expression)
{
	int x=0;
	
	if(isEMPTY(expression))
	{
		return(NULL);
	}
	if(CustomFields!=NULL)
	{
		for(x=0;x<CustomFields->numlibs;++x)
		{
			if(RDAstrstr(expression,CustomFields->libs[x])!=NULL)
			{
				return(CustomFields->libs[x]);
			}
		}
	}
	return(NULL);
}
static void ChangeScreenDefault_ContainsCustom(RDA_PFILE *fp1,RDAdefault *p,char *lib,char *from_mod,char *to_mod,int *lines,int *pages)
{
	int x=0;
	DEFfield *temp=NULL;
	char *tempstring=NULL;
/*
	short done_it=FALSE;
*/
	char *temp1=NULL,*temp2=NULL,*temp3=NULL;
	char *string_temp=NULL;
	int *foundone=NULL;

	if(p!=NULL)
	{
		if(foundone!=NULL) Rfree(foundone);
		foundone=Rmalloc(sizeof(int)); 
		for(x=0,temp=p->field;x<(p->num-1);++x,++temp)
		{
			if(temp->type==CUSTOMTYPE)
			{
				if(*foundone==0) 
				{
					*foundone=1;
					string_temp=Rmalloc(RDAstrlen(lib)+RDAstrlen(p->name)+23+1); 
					sprintf(string_temp,"SCREEN DEFAULT: [%s][%s]",
						(lib!=NULL?lib:""),
						(p->name!=NULL?p->name:""));
					RDA_fprintf(fp1,"%s\r\n",string_temp);
					if(string_temp!=NULL) Rfree(string_temp);
					checkheader(fp1,lines,pages,from_mod,to_mod);
				}	
				temp1=Rmalloc(RDAstrlen(temp->name)+1);
				sprintf(temp1,"%s",(temp->name!=NULL?temp->name:""));
				temp2=Rmalloc(6+1);
				sprintf(temp2,"%.6d",(x+1));
				temp3=Rmalloc(31+1);
				sprintf(temp3,"Custom Field in Screen Default");
				string_temp=RDA_FormatOutputDefaultCustom(temp1,temp2,temp3);
				if(temp1!=NULL) Rfree(temp1);
				if(temp2!=NULL) Rfree(temp2);
				if(temp3!=NULL) Rfree(temp3);
				RDA_fprintf(fp1,"%s\r\n",string_temp);
				if(string_temp!=NULL) Rfree(string_temp);
				checkheader(fp1,lines,pages,from_mod,to_mod);
			}
			tempstring=ContainsACustomField(temp->name);
			if(tempstring!=NULL)
			{
				if(*foundone==0) 
				{
					*foundone=1;
					string_temp=Rmalloc(RDAstrlen(lib)+RDAstrlen(p->name)+22+1); 
					sprintf(string_temp,"SCREEN DEFAULT: [%s][%s]",
						(lib!=NULL?lib:""),
						(p->name!=NULL?p->name:""));
					RDA_fprintf(fp1,"%s\r\n",string_temp);
					if(string_temp!=NULL) Rfree(string_temp);
					checkheader(fp1,lines,pages,from_mod,to_mod);
				}	
				temp1=Rmalloc(RDAstrlen(temp->name)+1);
				sprintf(temp1,"%s",(temp->name!=NULL?temp->name:""));
				temp2=Rmalloc(6+1);
				sprintf(temp2,"%.6d",(x+1));
				temp3=Rmalloc(RDAstrlen(tempstring)+32+1);
				sprintf(temp3,"(%s) Custom Field in Database File",tempstring);
				string_temp=RDA_FormatOutputDefaultCustom(temp1,temp2,temp3);
				if(temp1!=NULL) Rfree(temp1);
				if(temp2!=NULL) Rfree(temp2);
				if(temp3!=NULL) Rfree(temp3);
				RDA_fprintf(fp1,"%s\r\n",string_temp);
				if(string_temp!=NULL) Rfree(string_temp);
				checkheader(fp1,lines,pages,from_mod,to_mod);
			}
			tempstring=NULL;
		}
		if(diagapps)
		{
			prterr("No Changes Found [%s][%s].\r\n",lib,p->name);TRACE;
		}
	}
}
static void setsensitivecallback(RDArsrc *mainrsrc)
{
	char rng_mod=FALSE;

	readallwidgets(mainrsrc);
	FINDRSCGETCHAR(mainrsrc,"RANGE ON MODULE",&rng_mod);
	if(rng_mod==TRUE)
	{
		FINDRSCSETSENSITIVE(mainrsrc,"FROM MODULE",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"FROM MODULE",TRUE);
		FINDRSCSETSENSITIVE(mainrsrc,"TO MODULE",TRUE);
		FINDRSCSETEDITABLE(mainrsrc,"TO MODULE",TRUE);
	} else {
		FINDRSCSETSENSITIVE(mainrsrc,"FROM MODULE",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"FROM MODULE",FALSE);
		FINDRSCSETSENSITIVE(mainrsrc,"TO MODULE",FALSE);
		FINDRSCSETEDITABLE(mainrsrc,"TO MODULE",FALSE);
	}
	updateallrsrc(mainrsrc);
}
/*
static void datedef(RDArsrc *mainrsrc)
*/
static void custdef(RDArsrc *mainrsrc)
{
	RDA_PFILE *fp1=NULL;
	char *dirx=NULL,*tmp=NULL,*libx=NULL,*temp=NULL;
	char rng_mod=FALSE,*mod_from=NULL,*mod_to=NULL;
	int x=0,y=0,z=0;
	APPlib *modlst=NULL,*file_def=NULL;
	APPlib *liblist=NULL;
	short fileno=(-1),nofields=0;
	NRDfield *fields=NULL,*field=NULL;
	DFvirtual *d=NULL;
	RDAdefault *RDA_DEFDATA=NULL;
	int lines=0;
	int pages=0;
	char *outputdevice=NULL;
/*
	char dont_do_it=TRUE;
*/
	char *prtmessage=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETCHAR(mainrsrc,"RANGE ON MODULE",&rng_mod);
	if(rng_mod==TRUE)
	{
		FINDRSCGETSTRING(mainrsrc,"FROM MODULE",&mod_from);
		FINDRSCGETSTRING(mainrsrc,"TO MODULE",&mod_to);
	} else {
	}
	FINDRSCGETSTRING(mainrsrc,"OUTPUT DEVICE",&outputdevice);
/*
	FINDRSCGETCHAR(mainrsrc,"PREVIEW ONLY",&dont_do_it);
*/
	if(CustomFields!=NULL) freeapplib(CustomFields);
	if((fp1=RDA_popen(outputdevice,"w"))!=NULL)
	{
		CustomFields=APPlibNEW();
		dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+8);
#ifndef WIN32
		sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
		sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
		modlst=APPlibNEW();
		for(x=0;findfile(dirx,"*.FIL",&tmp,(int)x+1);++x) 
		{
			tmp[RDAstrlen(tmp)-4]=0;
			if(RDAstrcmp(tmp,"RPTGEN"))
			{
				addAPPlib(modlst,tmp);
			}
		}
		if(tmp!=NULL) Rfree(tmp);
		if(dirx!=NULL) Rfree(dirx);
		if(modlst->numlibs<1)
		{
			if(modlst!=NULL) freeapplib(modlst);
			prterr("Error No Databases for any module to search with....");
			return;
		}
		for(x=0;x<modlst->numlibs;++x)
		{
			if(file_def!=NULL) freeapplib(file_def); 
			libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlst->libs[x])+12);
#ifndef WIN32
			sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modlst->libs[x]);
#endif
#ifdef WIN32
			sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modlst->libs[x]);
#endif
			file_def=getlibnames(libx,TRUE);
			if(libx!=NULL) Rfree(libx);
			if(file_def!=NULL)
			{
				for(y=0;y<file_def->numlibs;++y)
				{
					fileno=OPNNRD(modlst->libs[x],file_def->libs[y]);
					if(fileno!=(-1))
					{
						fields=FLDPOINTER(fileno);
						nofields=NUMFLDS(fileno);
						if(fields!=NULL)
						{
							for(z=0,field=fields;z<nofields;++z,++field)
							{
								if(field->type==CUSTOMTYPE)
								{
									temp=Rmalloc(RDAstrlen(file_def->libs[y])+RDAstrlen(field->name)+4+1);
									sprintf(temp,"[%s][%s]",file_def->libs[y],field->name);
									addAPPlib(CustomFields,temp);
									if(temp!=NULL) Rfree(temp);
									temp=Rmalloc(RDAstrlen(modlst->libs[x])+RDAstrlen(file_def->libs[y])+RDAstrlen(field->name)+7);
									sprintf(temp,"[%s][%s][%s]",modlst->libs[x],file_def->libs[y],field->name);
									addAPPlib(CustomFields,temp);
									if(temp!=NULL) Rfree(temp);
								}
							}
						}
						CLSNRD(fileno);
					} else {
						prterr("Error:  Search will not include fields from database [%s][%s], it was unable to be opened.",modlst->libs[x],file_def->libs[y]);
					}
				}
				if(file_def!=NULL) freeapplib(file_def);
			}
		}
		if(diagapps)
		{
			prterr("List of file fields to look for:.\n");SEEAPPlib(CustomFields);TRACE;
		}
		if(modlst!=NULL) freeapplib(modlst);
		dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+8);
#ifndef WIN32
		sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
		sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
		modlst=APPlibNEW();
		for(x=0;findfile(dirx,"*.VIR",&tmp,(int)x+1);++x) 
		{
			tmp[RDAstrlen(tmp)-4]=0;
			if(rng_mod==TRUE)
			{
				if(((RDAstrcmp(tmp,mod_from)>=0) && 
				   (RDAstrcmp(tmp,mod_to)<=0)))
				{
					addAPPlib(modlst,tmp);
				}
			} else { 
				addAPPlib(modlst,tmp);
			}
		}
		if(tmp!=NULL) Rfree(tmp);
		if(dirx!=NULL) Rfree(dirx);
		if(modlst->numlibs<1)
		{
			freeapplib(modlst);
			prterr("Error No Virtual Binary Libaries for any module....");
			return;
		}
		for(x=0;x<modlst->numlibs;++x)
		{
			if(file_def!=NULL) freeapplib(file_def); 
			libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlst->libs[x])+12);
#ifndef WIN32
			sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,modlst->libs[x]);
#endif
#ifdef WIN32
			sprintf(libx,"%s\\rda\\%s.VIR",CURRENTDIRECTORY,modlst->libs[x]);
#endif
			file_def=getlibnames(libx,TRUE);
			if(libx!=NULL) Rfree(libx);
			if(file_def!=NULL)
			{
				for(y=0;y<file_def->numlibs;++y)
				{
					d=getDFvirtual(modlst->libs[x],file_def->libs[y]);
					if(d!=NULL)
					{
						if(d->type==CUSTOMTYPE)
						{
							temp=Rmalloc(RDAstrlen(d->name)+3);
							sprintf(temp,"[%s]",(d->name!=NULL ?
								d->name:""));
							addAPPlib(CustomFields,temp);
							if(temp!=NULL) Rfree(temp);
						}
						FreeDFvirtual(d);
					}
				}
				freeapplib(file_def);
			}
	
		}
		if(diagapps)
		{
			prterr("List of file and virtual fields to look for:.\n");SEEAPPlib(CustomFields);TRACE;
		}
		if(modlst!=NULL) freeapplib(modlst);
		liblist=makeLiblist();
		if(diagapps)
		{
			prterr("List of library locations to look in:.\n");SEEAPPlib(liblist);TRACE;
		}
		lines=0;
		pages=0;
		checkheader(fp1,&lines,&pages,mod_from,mod_to);
		if(liblist!=NULL)
		{
			if(liblist->libs!=NULL)
			{
				for(z=0;z<liblist->numlibs;++z)
				{
					modlst=APPlibNEW();
					dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+8);
#ifndef WIN32
					sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
					sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
					for(x=0;findfile(dirx,"*.DEF",&tmp,(int)x+1);++x) 
					{
						tmp[RDAstrlen(tmp)-4]=0;
						if(rng_mod==TRUE)
						{
							if(((RDAstrcmp(tmp,mod_from)>=0) && 
				   				(RDAstrcmp(tmp,mod_to)<=0)))
							{
								addAPPlib(modlst,tmp);
							}
						} else { 
							addAPPlib(modlst,tmp);
						}
					}
					dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+8);
#ifndef WIN32
					sprintf(dirx,"%s/site/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
					sprintf(dirx,"%s\\site\\",CURRENTDIRECTORY);
#endif
					for(x=0;findfile(dirx,"*.DEF",&tmp,(int)x+1);++x) 
					{
						tmp[RDAstrlen(tmp)-4]=0;
						if(rng_mod==TRUE)
						{
							if(((RDAstrcmp(tmp,mod_from)>=0) && 
				   				(RDAstrcmp(tmp,mod_to)<=0)))
							{
								addAPPlibNoDuplicates(modlst,tmp);
							}
						} else { 
							addAPPlibNoDuplicates(modlst,tmp);
						}
					}
					if(tmp!=NULL) Rfree(tmp);
					if(dirx!=NULL) Rfree(dirx);
					if(diagapps)
					{
						prterr("List of binaries to check:.\n");SEEAPPlib(modlst);TRACE;
					}
					if(modlst->libs!=NULL)
					{
						for(x=0;x<modlst->numlibs;++x)
						{
							if(file_def!=NULL) freeapplib(file_def); 
							libx=Rmalloc(RDAstrlen(liblist->libs[z])+RDAstrlen(modlst->libs[x])+12);
#ifndef WIN32
							sprintf(libx,"%s/%s.DEF",liblist->libs[z],modlst->libs[x]);
#endif
#ifdef WIN32
							sprintf(libx,"%s\\%s.DEF",liblist->libs[z],modlst->libs[x]);
#endif
							file_def=getlibnames(libx,TRUE);
							if(diagapps)
							{
								prterr("List of Screen Defaults being examined:.\n");SEEAPPlib(file_def);TRACE;
							}
							if(file_def!=NULL)
							{
								for(y=0;y<file_def->numlibs;++y)
								{
									RDA_DEFDATA=RDAdefaultNEW(libx,file_def->libs[y]);
									if(diagapps)
									{
										prterr("Opening [%s][%s].\r\n",libx,file_def->libs[y]);TRACE;
									}
						
									if(getdefaultbin(libx,file_def->libs[y],RDA_DEFDATA,FALSE)==0)
									{
										if(diagapps)
										{
											prterr("Found and Processing [%s][%s].\r\n",libx,file_def->libs[y]);TRACE;
										}
										ChangeScreenDefault_ContainsCustom(fp1,RDA_DEFDATA,libx,mod_from,mod_to,&lines,&pages);		
									} else {
										if(diagapps)
										{
											prterr("NOT FOUND [%s][%s].\r\n",libx,file_def->libs[y]);TRACE;
										}
										prterr("Error:  Couldn't read Screen Default [%s] from binary library [%s].",file_def->libs[y],libx);
									}
									if(RDA_DEFDATA!=NULL) FreeRDAdefault(RDA_DEFDATA);
								}
								freeapplib(file_def);
							}
							if(libx!=NULL) Rfree(libx);
						}
					}
					if(modlst!=NULL) freeapplib(modlst);
				}
			}
		}
		if(modlst!=NULL) freeapplib(modlst);
		RDA_pclose(fp1);
		quitfunc(mainrsrc);
	} else {
		prtmessage=Rmalloc(RDAstrlen(outputdevice)+69);
		sprintf(prtmessage,"The output device [%s] does not exist or cannot otherwise be opened!",outputdevice);
		ERRORDIALOG("Output Device Failure",prtmessage,quitfunc,FALSE);
		if(prtmessage!=NULL) Rfree(prtmessage);
	}
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	char b=FALSE;
	RDArsrc *mainrsrc=NULL;

	if(InitializeSubsystems(argc,argv,module,"FIND CUSTOM INPUT FIELDS IN SCREEN DEFAULTS")) 
	{
		return;
	}
	mainrsrc=RDArsrcNEW(module,"FIND CUSTOM INPUT FIELDS IN SCREEN DEFAULTS");
	addstdrsrc(mainrsrc,"RANGE ON MODULE",BOOLNS,1,&b,TRUE);
	addstdrsrc(mainrsrc,"FROM MODULE",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mainrsrc,"TO MODULE",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mainrsrc,"OUTPUT DEVICE",VARIABLETEXT,0,NULL,TRUE);
/*
	b=TRUE;
	addstdrsrc(mainrsrc,"PREVIEW ONLY",BOOLNS,1,&b,TRUE);
*/
	DefaultScreens(mainrsrc);
	SetRangersrcsensitive(mainrsrc);
	FINDRSCSETFUNC(mainrsrc,"RANGE ON MODULE",setsensitivecallback,NULL);
	addbtnrsrc(mainrsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	addrfexrsrc(mainrsrc,"SELECT",TRUE,custdef,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitfunc,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,quitfunc,NULL,TRUE);
}
