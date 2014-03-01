/* dfnrd.c - Define Datafiles */
#ifndef WIN32
#define __NAM__ "dfnrd.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dfnrd.exe"
#endif
#include <app.hpp>
#ifdef WIN32
#include<cstdlib>
#include<io.h>
#endif
#include<mknrd.hpp>
#include<doheader.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

struct dffiles
{
	short nofields;
	NRDfield *fields;
	short nokeys;
	NRDkey *keys;
};
typedef struct dffiles dffile;

static RDArsrc *fldrsrc=NULL,*mainrsrc=NULL,*mtnfrsrc=NULL,*keypartrsrc=NULL;
static RDArsrc *keyfldrsrc=NULL;
static dffile *datafile;
static APPlib *file_defs=NULL,*field=NULL,*keyfield=NULL,*keyparts=NULL;
static APPlib *ftypes=NULL,*modlst=NULL,*fldlist=NULL,*dbengines=NULL;
static dffile *dffilenew(void);
static void savefile(RDArsrc *);
static void savefile_check(RDArsrc *);
static void freedffile(dffile *),doexit(RDArsrc *);
static void keypartscreen(int,char *,void (*)(...));
void getmodulelist(void),domodulelist(RDArsrc *);
static short makepartlist();
static void makespacefield(int),makespacekeys(int),makespaceparts(int);
static void quit_part(RDArsrc *),quit_parttest(RDArsrc *,void (*)(...));
static void save_part(RDArsrc *);
static void save_part_above(RDArsrc *);
static void save_part_below(RDArsrc *);
static void quit_keyfld(RDArsrc *),quit_keyfldtest(RDArsrc *,void (*)(...));
static void quit_field(RDArsrc *),quit_fieldtest(RDArsrc *,void (*)(...));
static void save_field(RDArsrc *);
static void printfile(RDArsrc *,RDArsrc *);
static NRDpart *holdparts=NULL;
static RDAdefault *mtnfdefaults=NULL,*flddefaults=NULL,*keyflddefaults=NULL;
static RDAdefault *keypartdefaults=NULL;
static short noholdparts=0;
static char changedfile=FALSE,changedfields=FALSE,changedkeys=FALSE;
static void setdbenginescb(RDArsrc *);
static void setdbengines(RDArsrc *);

static void print_header(RDA_PFILE *fp,int *lines,int *pages,
			char *header,unsigned line_length,
			char *module,char *file)
{
	char *date=NULL,*timex=NULL;
	int lead_ws=0,tail_ws=0;

	date=GETCURRENTDATE();
	timex=GETCURRENTTIME();
	RDA_fprintf(fp,"\r\n");
	*lines=1;
	lead_ws=(int)(RDAroundup((double)(line_length-(2+RDAstrlen(header)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(2+RDAstrlen(header)))/2)-14));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(header)+2+14+lead_ws+tail_ws)>line_length) header[(line_length-14-lead_ws-tail_ws)]=0;
	RDA_fprintf(fp,"%*s %s %*sPage: %*d\r\n",
		lead_ws,"",header,tail_ws,"",8,++*pages);
	*lines+=1;
	lead_ws=(int)(RDAroundup((double)(line_length-(15+RDAstrlen(module)+RDAstrlen(file)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint((((double)(line_length-(15+RDAstrlen(module)+RDAstrlen(file)))/2)-14)));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(module)+RDAstrlen(file)+14+15+lead_ws+tail_ws)>(line_length))
	{
		RDA_fprintf(fp,"%*s%s Module %*sDate: %s\r\n",
			(int)(round(((double)(line_length-(8+RDAstrlen(module)))/2))),"",
			module,
			(int)(Rint((((double)(line_length-(8+RDAstrlen(module)))/2)-14))),"",
			date);
		RDA_fprintf(fp,"%*s%s File %*s\r\n",
			(int)(round(((double)(line_length-(6+RDAstrlen(file)))/2))),"",
			file,
			(int)(Rint((((double)(line_length-(6+RDAstrlen(file)))/2)-14))),"");
		*lines+=2;
	} else {
		RDA_fprintf(fp,"%*s %s Module %s File %*sDate: %s\r\n",
			lead_ws,"",module,file,tail_ws,"",date);
		*lines+=1;
	}
	lead_ws=(int)(RDAroundup(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)-14));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(USERLOGIN)+14+15+lead_ws+tail_ws)>line_length)
	{
		RDA_fprintf(fp,"%*s %s %*sTime: %s\r\n",
			(int)(RDAroundup(((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2))),"",
			USERLOGIN,
			(int)(Rint((((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2)-14))),"",
			timex);
		*lines+=1;
	} else {
		RDA_fprintf(fp,"%*s Executed By: %s %*sTime: %s\r\n",
			lead_ws,"",USERLOGIN,tail_ws,"",timex);
		*lines+=1;
	}
	RDA_fprintf(fp,"\r\n");
	*lines+=1;
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
}
static void printfilemf(RDA_PFILE *fp,char *modulename,char *filename)
{
	short engine=0,nofields=0,nokeys=0;
	int x=0,y=0;
	NRDfield *fieldx=NULL,*fld=NULL;
	NRDkey *keyx=NULL,*key;
	NRDpart *keyparts=NULL,*part;
	char *temp=NULL,*serverx=NULL;
	int lines=0,pages=0,length=0;
	unsigned line_length=79;

	if(doheaders)
	{
		print_header(fp,&lines,&pages,"XPERT FILE DEFINITION",
			line_length,modulename,filename);
	} else {
		RDA_fprintf(fp,"Module [%s] File [%s]\r\n",modulename,filename);
		lines+=1;
	}
	if(ADVGETBIN(modulename,filename,&engine,&nofields,&fieldx,&nokeys,&keyx,&serverx))
	{
		RDA_fprintf(fp,"Can't Open Binary: Please check the file definition to resolve this error."); 
	}
	RDA_fprintf(fp,"Engine: [%s]  ",supported_engines[engine]);
	if(!isEMPTY(serverx))
	{
		RDA_fprintf(fp,"Server: [%s]",serverx);
	}
	RDA_fprintf(fp,"\r\n");
	++lines;
	if(doheaders)
	{
	if((lines+1)>58)
	{
		RDA_fprintf(fp,"\f");
		print_header(fp,&lines,&pages,
			"XPERT FILE DEFINITION",
			line_length,modulename,filename);
	}
	}
	if(nokeys>0)
	{
		if(doheaders)
		{
			RDA_fprintf(fp,"\r\nKeys:\r\n");
			lines+=2;
		} else {
			RDA_fprintf(fp,"Keys:\r\n");
			lines+=1;
		}
		if(doheaders)
		{
		if((lines)>58)
		{
			RDA_fprintf(fp,"\f");
			print_header(fp,&lines,&pages,
				"XPERT FILE DEFINITION",
				line_length,modulename,filename);
		}
		}
		for(x=0,key=keyx;x<nokeys;++x,++key)
		{
			RDA_fprintf(fp,"   Key Name: [%s] Number parts: [%d]\r\n",(keyx->name!=NULL ? key->name:""),key->numparts);
			++lines;
			if(doheaders)
			{
			if((lines)>58)
			{
				RDA_fprintf(fp,"\f");
				print_header(fp,&lines,&pages,
					"XPERT FILE DEFINITION",
					line_length,modulename,filename);
			}
			}
			if(key->numparts>0)
			{
				RDA_fprintf(fp,"      Key Parts:\r\n");
				++lines;
				if(doheaders)
				{
				if((lines)>58)
				{
					RDA_fprintf(fp,"\f");
					print_header(fp,&lines,&pages,
						"XPERT FILE DEFINITION",
						line_length,modulename,filename);
				}
				}
				for(y=0,keyparts=key->part;y<key->numparts;++y,++keyparts)
				{
					RDA_fprintf(fp,"         Key Field Name: [%s]\r\n",(keyparts->name!=NULL ? keyparts->name:""));
					++lines;
					if(doheaders)
					{
					if((lines)>58)
					{
						RDA_fprintf(fp,"\f");
						print_header(fp,&lines,&pages,
							"XPERT FILE DEFINITION",
							line_length,modulename,filename);
					}
					}
				}
			}
		}
	}
	if(nofields>0)
	{
		if(doheaders)
		{
			RDA_fprintf(fp,"\r\nFields:\r\n");
			lines+=2;
			if((lines)>58)
			{
				RDA_fprintf(fp,"\f");
				print_header(fp,&lines,&pages,
					"XPERT FILE DEFINITION",
					line_length,modulename,filename);
			}
		} else {
			RDA_fprintf(fp,"Fields:\r\n");
			lines+=1;
		}
		for(x=0,fld=fieldx;x<nofields;++x,++fld)
		{
			if(doheaders)
			{
				length=(RDAstrlen(fld->name)+RDAstrlen(fieldtypedesc[fld->type])+5+45);
				if(length<=line_length)
				{
					temp=Rmalloc(RDAstrlen(fld->name)+RDAstrlen(fieldtypedesc[fld->type])+5+45);
					sprintf(temp,"   Field Name: [%s]   Type: [%s]  Length: [%d]\r\n",(fld->name!=NULL ? fld->name:""),fieldtypedesc[fld->type],fld->len);
					RDA_fprintf(fp,temp);
					if(temp!=NULL) Rfree(temp);
					++lines;
					if((lines)>58)
					{
						RDA_fprintf(fp,"\f");
						print_header(fp,&lines,&pages,
							"XPERT FILE DEFINITION",
							line_length,modulename,filename);
					}
				} else {
					temp=Rmalloc(RDAstrlen(fld->name)+20);
					sprintf(temp,"   Field Name: [%s]\r\n",(fld->name!=NULL ? fld->name:""));
					RDA_fprintf(fp,temp);
					if(temp!=NULL) Rfree(temp);
					++lines;
					if((lines)>58)
					{
						RDA_fprintf(fp,"\f");
						print_header(fp,&lines,&pages,
							"XPERT FILE DEFINITION",
							line_length,modulename,filename);
					}
					temp=Rmalloc(RDAstrlen(fieldtypedesc[fld->type])+5+30);
					sprintf(temp,"     Type: [%s]  Length: [%d]\r\n",fieldtypedesc[fld->type],fld->len);
					RDA_fprintf(fp,temp);
					if(temp!=NULL) Rfree(temp);
					++lines;
					if((lines)>58)
					{
						RDA_fprintf(fp,"\f");
						print_header(fp,&lines,&pages,
							"XPERT FILE DEFINITION",
							line_length,modulename,filename);
					}
				}
			} else {
				RDA_fprintf(fp,"   Field Name: [%s]   Type: [%s]  Length: [%d]\r\n",(fld->name!=NULL ? fld->name:""),fieldtypedesc[fld->type],fld->len);
			}
		}
	}
	if(serverx!=NULL) Rfree(serverx);
	if(fieldx!=NULL)
	{
		for(x=0,fld=fieldx;x<nofields;++x,++fld)
		{
			FreeNRDfield(fld);
		}
		Rfree(fieldx);
	}
	if(keyx!=NULL)
	{
		for(x=0,key=keyx;x<nokeys;++x,++key)
		{
			if(key->name!=NULL) Rfree(key->name);
			if(key->part!=NULL)
			{
				for(y=0,part=key->part;y<key->numparts;++y,++part)
				{
					if(part->name!=NULL) Rfree(part->name);
				}
				Rfree(key->part);
			}
		}
		Rfree(keyx);
	}
}
static void printfile(RDArsrc *parent,RDArsrc *mtn)
{
	RDA_PFILE *fp=NULL;
	char *modulename=NULL,*filename=NULL;
	char *message=NULL,*outdevice=NULL,print_style=TRUE;

	FINDRSCGETSTRING(mtn,"MODULE NAME",&modulename);
	FINDRSCGETSTRING(mtn,"FILENAME",&filename);
	readwidget(parent,"DEVICE");
	FINDRSCGETSTRING(parent,"DEVICE",&outdevice);
	readwidget(parent,"NORMAL PRINT");
	FINDRSCGETCHAR(parent,"NORMAL PRINT",&print_style);
	if(outdevice!=NULL)
	{
		fp=RDA_popen(outdevice,"w");
		if(fp!=NULL)
		{
			SelectPrintType(outdevice,print_style,fp);
			printfilemf(fp,modulename,filename);
			if(doheaders)
			{
				RDA_fprintf(fp,"\f");
			}
			RDA_fprintf(fp,"\r\n");
			RDA_pclose(fp);
		} else {
			message=Rmalloc(RDAstrlen(outdevice)+69);
			sprintf(message,"The output attemp to the device [%s] failed when trying to open pipe.",outdevice);
			ERRORDIALOG("DEVICE COMMUNICATIONS FAILED!",message,NULL,FALSE);
			prterr(message);
			if(message!=NULL) { Rfree(message); message=NULL; }
		}
	}
	if(modulename!=NULL) Rfree(modulename);
	if(filename!=NULL) Rfree(filename);
}
static void printmodule(RDArsrc *parent,RDArsrc *mtn)
{
	RDA_PFILE *fp=NULL;
	int x=0,m=0;
	char *message=NULL,*outdevice=NULL,print_style=TRUE;

	readwidget(mtn,"MODULE LIST");
	FINDRSCGETINT(mtn,"MODULE LIST",&m);
	readwidget(parent,"DEVICE");
	FINDRSCGETSTRING(parent,"DEVICE",&outdevice);
	readwidget(parent,"NORMAL PRINT");
	FINDRSCGETCHAR(parent,"NORMAL PRINT",&print_style);
	if(outdevice!=NULL)
	{
		fp=RDA_popen(outdevice,"w");
		if(fp!=NULL)
		{
			SelectPrintType(outdevice,print_style,fp);
			for(x=0;x<file_defs->numlibs;++x)
			{
				printfilemf(fp,modlst->libs[m],file_defs->libs[x]);
				if(doheaders)
				{
					RDA_fprintf(fp,"\f");
				}
				RDA_fprintf(fp,"\r\n");
			}
			RDA_pclose(fp);
		} else {
			message=Rmalloc(RDAstrlen(outdevice)+69);
			sprintf(message,"The output attemp to the device [%s] failed when trying to open pipe.",outdevice);
			ERRORDIALOG("DEVICE COMMUNICATIONS FAILED!",message,NULL,FALSE);
			prterr(message);
			if(message!=NULL) { Rfree(message); message=NULL; }
		}
	}
}
static void quit_print_definition(RDArsrc *prsrc)
{
	if(diagolh || diagolh_field)
	{
		prterr("DIAG quit_print_definition Quiting Print File Definition."); 
	}
	if(prsrc!=NULL) free_rsrc(prsrc);
}
static void printdefinition(RDArsrc *mtn,void (*printfunc)(...))
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL,normal_print=TRUE;

	if(diagnrd || diagnrd_field)
	{
		prterr("DIAG printdefinition Displaying the Print Screen in preparation to print file definition.");
	}
	prsrc=RDArsrcNEW("DATABASE","PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addbtnrsrc(prsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	addstdrsrc(prsrc,"NORMAL PRINT",BOOLNS,1,&normal_print,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printfunc,mtn);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_print_definition,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PRINT SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		quit_print_definition(prsrc);
	}
}
static dffile *dffilenew()
{
	dffile *tmp;

	tmp=Rmalloc(sizeof(dffile));
	tmp->nofields=0;
	tmp->nokeys=0;
	tmp->fields=NULL;
	tmp->keys=NULL;
	return(tmp);
}
static void freedffile(dffile *tmp)
{
	short x,y;
	NRDfield *f;
	NRDkey *k;
	NRDpart *p;

	if(tmp->fields!=NULL)
	{
		for(x=0,f=tmp->fields;x<tmp->nofields;++x,++f)
		{
			if(f->name!=NULL) Rfree(f->name);
		}
		Rfree(tmp->fields);
	}
	if(tmp->keys!=NULL)
	{
		for(x=0,k=tmp->keys;x<tmp->nokeys;++x,++k)
		{
			if(k->name!=NULL) Rfree(k->name);
			if(k->part!=NULL)
			{
				for(y=0,p=k->part;y<k->numparts;++y,++p)
				{
					if(p->name!=NULL) Rfree(p->name);
				}
				Rfree(k->part);
			}
		}
		Rfree(tmp->keys);
	}
}
static void doexitsub(void)
{
	if(modlst!=NULL) freeapplib(modlst);
	if(dbengines!=NULL) freeapplib(dbengines);
	ShutdownSubsystems();
	std::exit;
}
static void doexit(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	if(keypartrsrc!=NULL) free_rsrc(keypartrsrc);
	if(fldrsrc!=NULL) free_rsrc(fldrsrc);
	if(keyfldrsrc!=NULL) free_rsrc(keyfldrsrc);
	if(mtnfrsrc!=NULL) free_rsrc(mtnfrsrc);
	if(file_defs!=NULL) freeapplib(file_defs);
	if(field!=NULL) freeapplib(field);
	if(keyfield!=NULL) freeapplib(keyfield);
	if(keyparts!=NULL) freeapplib(keyparts);
	if(fldlist!=NULL) freeapplib(fldlist);
	if(ftypes!=NULL) freeapplib(ftypes);
	if(keyflddefaults!=NULL) FreeRDAdefault(keyflddefaults);
	if(keypartdefaults!=NULL) FreeRDAdefault(keypartdefaults);
	if(mtnfdefaults!=NULL) FreeRDAdefault(mtnfdefaults);
	if(flddefaults!=NULL) FreeRDAdefault(flddefaults);
	doexitsub();
}
static void makespacefield(int pos)
{
	short x,y;
	NRDfield *temp,*temp1,*new_fields;

	++datafile->nofields;
	new_fields=Rmalloc(datafile->nofields*sizeof(NRDfield));
	y=0;
	for(x=0,temp=datafile->fields;x<(datafile->nofields-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->name=NULL;
			temp1->type=0;
			temp1->len=0;
			++y;
			temp1=new_fields+y;
		}
		temp1->type=temp->type;
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->len=temp->len;
		++y;
	}
	Rfree(datafile->fields);
	datafile->fields=Rmalloc(datafile->nofields*sizeof(NRDfield));
	for(x=0,temp1=datafile->fields,temp=new_fields;x<datafile->nofields;++x,
		++temp,++temp1)
	{
		temp1->type=temp->type;
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->len=temp->len;
	}
	Rfree(new_fields);
}
static void makespacekeys(int pos)
{
	short x,y,w;
	NRDkey *temp,*new_key,*temp1;
	NRDpart *part1,*part;

	++datafile->nokeys;
	new_key=Rmalloc(datafile->nokeys*sizeof(NRDkey));
	y=0;
	for(x=0,temp=datafile->keys;x<(datafile->nokeys-1);++x,++temp)
	{
		temp1=new_key+y;
		if(x==pos)
		{
			temp1->name=NULL;
			temp1->numparts=1;
			temp1->part=Rmalloc(sizeof(NRDpart));
			part=temp1->part;
			part->name=NULL;
			part->offset=0;
			++y;
			temp1=new_key+y;
		}
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->numparts=temp->numparts;
		temp1->part=Rmalloc(temp1->numparts*sizeof(NRDpart));
		for(w=0,part=temp->part,part1=temp1->part;w<temp->numparts;
			++w,++part,++part1)
		{
			if(part->name!=NULL)
			{
				part1->name=stralloc(part->name);
				Rfree(part->name);
			} else part1->name=NULL;
		}
		Rfree(temp->part);
		++y;
	}
	Rfree(datafile->keys);
	datafile->keys=Rmalloc(datafile->nokeys*sizeof(NRDkey));
	for(x=0,temp1=datafile->keys,temp=new_key;x<datafile->nokeys;++x,++temp,
		++temp1)
	{
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->numparts=temp->numparts;
		if(temp->part!=NULL)
		{
			temp1->part=Rmalloc(temp1->numparts*sizeof(NRDpart));
			for(w=0,part=temp->part,part1=temp1->part;w<temp->numparts;
				++w,++part,++part1)
			{
				if(part->name!=NULL)
				{
					part1->name=stralloc(part->name);
					Rfree(part->name);
				} else part1->name=NULL;
			}
			if(temp->part!=NULL) Rfree(temp->part);
		} else temp1->part=NULL;
	}
	Rfree(new_key);
}
static void makespaceparts(int pos)
{
	int x,y=0;
	NRDpart *temp,*new_part,*temp1;

	++noholdparts;
	new_part=Rmalloc(noholdparts*sizeof(NRDpart));
	for(x=0,temp=holdparts;x<(noholdparts-1);++x,++temp)
	{
		temp1=new_part+y;
		if(x==pos)
		{
			temp1->name=0;
			temp1->offset=0;
			++y;
			temp1=new_part+y;
		}
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->offset=temp->offset;
		++y;
	}
	Rfree(holdparts);
	holdparts=Rmalloc(noholdparts*sizeof(NRDpart));
	for(x=0,temp1=holdparts,temp=new_part;x<noholdparts;++x,++temp,
		++temp1)
	{
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->offset=temp->offset;
	}
	Rfree(new_part);
}
static short makepartlist()
{
	int x,l;
	NRDpart *part;
	char *s;

	if(keyparts!=NULL) freeapplib(keyparts);
	keyparts=APPlibNEW();
	if(holdparts==NULL || noholdparts<=0)
	{
		addAPPlib(keyparts,"No Parts Exist");
		return TRUE;
	}
	for(x=0,part=holdparts;x<noholdparts;++x,++part)
	{
		l=RDAstrlen(part->name)+19;
		s=Rmalloc(l);
		sprintf(s,"%2d Name: [%s]",x+1,(part->name!=NULL ? 
			part->name:""));
		addAPPlib(keyparts,s);
		Rfree(s);
	}
	return TRUE;
}
static void keypartscreen(int partnum,char *name,void (*save_function)(...))
{
	int x,select=0;
	NRDfield *f;

	if(fldlist!=NULL) freeapplib(fldlist);
	fldlist=APPlibNEW();
	for(x=0,f=datafile->fields;x<datafile->nofields;++x,++f)
	{
		if(f->name!=NULL)
		{
			addAPPlib(fldlist,f->name);
			if(!RDAstrcmp(f->name,name)) select=x;
		}
	}
	FINDRSCSETINT(keypartrsrc,"PART NUMBER",partnum);
	if(select>=fldlist->numlibs) select=0;
	FINDRSCLISTAPPlib(keypartrsrc,"KEY PART NAME",select,fldlist);
	FINDRSCSETFUNC(keypartrsrc,"SAVE",save_function,NULL); 
/*lint -e611 */
	FINDRSCSETFUNC(keypartrsrc,"QUIT",quit_parttest,(void *)save_function); 
/*lint +e611 */
	keypartdefaults=GetDefaults(keypartrsrc);
	FINDRSCSETINPUTFOCUS(keypartrsrc,"KEY PART NAME");	
	APPmakescrn(keypartrsrc,TRUE,NULL,NULL,FALSE);
}
static void quit_keyfldcls(RDArsrc *keyfldrsrc)
{
	changedkeys=FALSE;
	quit_keyfld(keyfldrsrc);
}
static void quit_keyfldtest(RDArsrc *keyfldrsrc,void (*save_func)(...))
{
	readallwidgets(keyfldrsrc);
	if(COMPARE_RSRCS(keyfldrsrc,keyflddefaults,0) || changedkeys)
	{
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the KEY DEFINITION!\nDo want to Save these changes?",save_func,quit_keyfldcls,FALSE,keyfldrsrc);
	} else {
		quit_keyfld(keyfldrsrc);
	}
}
static void quit_keyfld(RDArsrc *keyfldrsrc)
{
	int x;
	NRDpart *part;

	if(holdparts!=NULL)
	{
		for(x=0,part=holdparts;x<noholdparts;++x,++part)
		{
			if(part->name!=NULL) Rfree(part->name);
		}
		Rfree(holdparts);
		noholdparts=0;
	}
	if(keyfldrsrc!=NULL)
	{
		killwindow(keyfldrsrc);
		NULL_RSCS(keyfldrsrc);
	}
	if(changedkeys) changedfile=TRUE;
	if(keyflddefaults!=NULL) FreeRDAdefault(keyflddefaults);
}
static void quit_partcls(RDArsrc *keypartrsrc)
{
	changedkeys=FALSE;
	quit_part(keypartrsrc);
}
static void quit_parttest(RDArsrc *keypartrsrc,void (*save_func)(...))
{
	readallwidgets(keypartrsrc);
	if(COMPARE_RSRCS(keypartrsrc,keypartdefaults,2))
	{
/*lint -e611*/
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the KEY PART DEFINITION!\nDo want to Save these changes?",save_func,quit_partcls,FALSE,2,keypartrsrc,(void *)save_func,NULL);
/*lint +e611*/
	} else {
		changedkeys=FALSE;
		quit_part(keypartrsrc);
	}
}
static void quit_part(RDArsrc *keypartrsrc)
{
	if(keypartrsrc!=NULL)
	{
		killwindow(keypartrsrc);
		NULL_RSCS(keypartrsrc);
	}
	if(keypartdefaults!=NULL) FreeRDAdefault(keypartdefaults);
}
static void save_part(RDArsrc *keypartrsrc)
{
	int x,selected=0;
	NRDpart *p;

	readallwidgets(keypartrsrc);
	if(FINDRSCGETINT(keyfldrsrc,"KEY PARTS LIST",&x)) return;
	if(holdparts==NULL)
	{
		holdparts=Rmalloc(sizeof(NRDpart));
		p=holdparts;
		p->name=NULL;
		noholdparts=1;
	}
	p=holdparts+x;
	FINDRSCGETINT(keypartrsrc,"KEY PART NAME",&selected);
	p->name=stralloc(fldlist->libs[selected]);
	makepartlist();
	if(x>=keyparts->numlibs) x=0;
	if(!FINDRSCLISTAPPlib(keyfldrsrc,"KEY PARTS LIST",x,keyparts))
		updatersrc(keyfldrsrc,"KEY PARTS LIST");
	if(COMPARE_RSRCS(keypartrsrc,keypartdefaults,2))
	{
		changedkeys=TRUE;
	}
	quit_part(keypartrsrc);
}
static void save_part_above(RDArsrc *keypartrsrc)
{
	int x;

	if(FINDRSCGETINT(keyfldrsrc,"KEY PARTS LIST",&x)) return;
	makespaceparts(x);
	save_part(keypartrsrc);
}
static void save_part_below(RDArsrc *keypartrsrc)
{
	int x;

	if(FINDRSCGETINT(keyfldrsrc,"KEY PARTS LIST",&x)) return;
	makespaceparts(++x);
	FINDRSCSETINT(keyfldrsrc,"KEY PARTS LIST",x);
	updatersrc(keyfldrsrc,"KEY PARTS LIST");
	save_part(keypartrsrc);
}
static void add_part_above(RDArsrc *keyfldrsrc)
{
	int x;

	if(FINDRSCGETINT(keyfldrsrc,"KEY PARTS LIST",&x)) return;
	if(!RDAstrcmp(keyparts->libs[x],"No Parts Exist"))
	{
		keypartscreen(x,NULL,save_part);
	} else {
		keypartscreen(x,NULL,save_part_above);
	}
}
static void add_part_below(RDArsrc *keyfldrsrc)
{
	int x;

	if(FINDRSCGETINT(keyfldrsrc,"KEY PARTS LIST",&x)) return;
	if(!RDAstrcmp(keyparts->libs[x],"No Parts Exist"))
	{
		keypartscreen(x,NULL,save_part);
	} else {
		keypartscreen(++x,NULL,save_part_below);
	}
}
static void copy_part(RDArsrc *keyfldrsrc)
{
	int x;
	NRDpart *part;

	if(FINDRSCGETINT(keyfldrsrc,"KEY PARTS LIST",&x)) return;
	if(!RDAstrcmp(keyparts->libs[x],"No Parts Exist"))
	{
		keypartscreen(x,NULL,save_part);
	} else {
		part=holdparts+x;
		keypartscreen(++x,part->name,save_part_below);
	}
}
static void edit_part(RDArsrc *keyfldrsrc)
{
	int x;
	NRDpart *part;

	if(FINDRSCGETINT(keyfldrsrc,"KEY PARTS LIST",&x)) return;
	if(!RDAstrcmp(keyparts->libs[x],"No Parts Exist"))
	{
		keypartscreen(x,NULL,save_part);
	} else {
		part=holdparts+x;
		keypartscreen(x,part->name,save_part);
	}
}
static void delete_part(RDArsrc *keyfldrsrc)
{
	int x,y,value;
	NRDpart *temp,*new_part,*temp1;

	if(FINDRSCGETINT(keyfldrsrc,"KEY PARTS LIST",&value)) return;
	if(noholdparts==1) return;
	new_part=Rmalloc((noholdparts-1)*sizeof(NRDpart));
	y=0;
	for(x=0,temp=holdparts;x<noholdparts;++x,++temp)
	{
		if(x!=value)
		{
			temp1=new_part+y;
			if(temp->name!=NULL)
			{
				temp1->name=stralloc(temp->name);
				Rfree(temp->name);
			} else temp1->name=NULL;
			++y;
		} else {
			if(temp->name!=NULL) Rfree(temp->name);
		}
	}
	Rfree(holdparts);
	--noholdparts;
	holdparts=Rmalloc(noholdparts*sizeof(NRDpart));
	for(x=0,temp1=holdparts,temp=new_part;x<noholdparts;++x,++temp,
		++temp1)
	{
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
	}
	Rfree(new_part);
	makepartlist();
	if(value>=keyparts->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(keyfldrsrc,"KEY PARTS LIST",value,keyparts))
		updatersrc(keyfldrsrc,"KEY PARTS LIST");
}
static void makekeyfieldlist()
{
	short x,l;
	NRDkey *keys;
	char *s;

	if(keyfield!=NULL) freeapplib(keyfield);
	keyfield=APPlibNEW();
	for(x=0,keys=datafile->keys;x<datafile->nokeys;++x,++keys)
	{
		l=RDAstrlen(keys->name)+49;
		s=Rmalloc(l);
		sprintf(s,"%d Name: [%s] Number parts [%d]",x+1,(keys->name!=NULL ?
			keys->name:""),keys->numparts);
		addAPPlib(keyfield,s);
		Rfree(s);
	}
}
static void save_key(RDArsrc *keyfldrsrc)
{
	int x,y=0;
	NRDkey *key;
	NRDpart *part,*part1;

	if(FINDRSCGETINT(mtnfrsrc,"KEY LIST",&y)) return;
	key=datafile->keys+y;
	readallwidgets(keyfldrsrc);
	FINDRSCGETSTRING(keyfldrsrc,"KEY NAME",&key->name);
	if(key->part!=NULL)
	{
		for(x=0,part=key->part;x<key->numparts;++x,++part)
		{
			if(part->name!=NULL) Rfree(part->name);
		}
		Rfree(key->part);
		key->numparts=0;
	}
	if(holdparts!=NULL)
	{
		key->numparts=noholdparts;
		key->part=Rmalloc(key->numparts*sizeof(NRDpart));
		for(x=0,part=key->part,part1=holdparts;x<noholdparts;
			++x,++part,++part1)
		{
			if(part1->name!=NULL)
			{
				part->name=stralloc(part1->name);
				Rfree(part1->name);
			} else part->name=NULL;
			part->offset=part1->offset;
		}
		Rfree(holdparts);
		noholdparts=0;
	}
	makekeyfieldlist();
	if(y>=keyfield->numlibs) y=0;
	if(!FINDRSCLISTAPPlib(mtnfrsrc,"KEY LIST",y,keyfield))
		updatersrc(mtnfrsrc,"KEY LIST");
	if(COMPARE_RSRCS(keyfldrsrc,keyflddefaults,2) || changedkeys)
	{
		changedkeys=TRUE;
	}
	quit_keyfld(keyfldrsrc);
}
static void keyfieldscreen(int keynum,char *keyname,int part,
	void (*save_func)(...))
{
	if(!makepartlist()) return;
	FINDRSCSETINT(keyfldrsrc,"KEY NUMBER",keynum);
	FINDRSCSETSTRING(keyfldrsrc,"KEY NAME",(keyname!=NULL ? keyname:""));
	FINDRSCLISTAPPlib(keyfldrsrc,"KEY PARTS LIST",part,keyparts);
	FINDRSCSETFUNC(keyfldrsrc,"SAVE",save_func,NULL);
/*lint -e611 */
	FINDRSCSETFUNC(keyfldrsrc,"QUIT",quit_keyfldtest,(void *)save_func);
/*lint +e611 */
	keyflddefaults=GetDefaults(keyfldrsrc);
	FINDRSCSETINPUTFOCUS(keyfldrsrc,"KEY NAME");	
	APPmakescrn(keyfldrsrc,TRUE,quit_keyfld,NULL,FALSE);
}
static void fieldscreen(int field_number,char *name,int fld,short len,
	void (*save_function)(...))
{
	FINDRSCSETINT(fldrsrc,"FIELD NUMBER",field_number);
	FINDRSCSETSTRING(fldrsrc,"FIELD NAME",(name!=NULL ? name:""));
	FINDRSCLISTAPPlib(fldrsrc,"FIELD TYPES",fld,ftypes);
	FINDRSCSETSHORT(fldrsrc,"FIELD LENGTH",len);
	FINDRSCSETFUNC(fldrsrc,"SAVE",save_function,NULL);
/*lint -e611 */
	FINDRSCSETFUNC(fldrsrc,"QUIT",quit_fieldtest,(void *)save_function);
/*lint +e611 */
	changedfields=FALSE;
	flddefaults=GetDefaults(fldrsrc);
	FINDRSCSETINPUTFOCUS(fldrsrc,"FIELD NAME");
	APPmakescrn(fldrsrc,TRUE,NULL,NULL,FALSE);
}
static void makefieldlist()
{
	int l=0;
	short x;
	NRDfield *fields;
	char *s;

	if(field!=NULL) freeapplib(field);
	field=APPlibNEW();
	for(x=0,fields=datafile->fields;x<datafile->nofields;++x,++fields)
	{
		l=RDAstrlen(standardfieldtypes[fields->type])+RDAstrlen(fields->name)+54;
		s=Rmalloc(l);
		sprintf(s,"%3d Name: [%s] Type: [%s] Length: [%d]",x+1,
			(fields->name!=NULL ? fields->name:""),
			standardfieldtypes[fields->type],fields->len);
		addAPPlib(field,s);
		Rfree(s);
	}
}
static void doexitfile(RDArsrc *mtnfrsrc)
{
	freedffile(datafile);
	if(field!=NULL) freeapplib(field);
	if(keyfield!=NULL) freeapplib(keyfield);
	if(mtnfrsrc!=NULL)
	{
		killwindow(mtnfrsrc);
		NULL_RSCS(mtnfrsrc);
	}
	if(mtnfdefaults!=NULL) FreeRDAdefault(mtnfdefaults);
}
static void doexitfiletest(RDArsrc *mtnfrsrc)
{
	readallwidgets(mtnfrsrc);
	if(COMPARE_RSRCS(mtnfrsrc,mtnfdefaults,0) || changedfile)
	{
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this FILE DEFINITION!\nDo want to Save these changes?",savefile_check,doexitfile,FALSE,mtnfrsrc);
	} else {
		doexitfile(mtnfrsrc);
	}
}
static void delete_key(RDArsrc *fldrsrc)
{
	int klst;
	short x,y,w;
	NRDkey *temp,*new_key,*temp1;
	NRDpart *part1,*part;

	if(FINDRSCGETINT(mtnfrsrc,"KEY LIST",&klst)) return;
	if(datafile->nokeys==1) return;
	new_key=Rmalloc((datafile->nokeys-1)*sizeof(NRDkey));
	y=0;
	for(x=0,temp=datafile->keys;x<datafile->nokeys;++x,++temp)
	{
		if(x!=klst)
		{
			temp1=new_key+y;
			if(temp->name!=NULL)
			{
				temp1->name=stralloc(temp->name);
				Rfree(temp->name);
			} else temp1->name=NULL;
			temp1->numparts=temp->numparts;
			if(temp->part!=NULL)
			{
				temp1->part=Rmalloc(temp1->numparts*sizeof(NRDpart));
				for(w=0,part=temp->part,part1=temp1->part;w<temp->numparts;
					++w,++part,++part1)
				{
					if(part->name!=NULL)
					{
						part1->name=stralloc(part->name);
						Rfree(part->name);
					} else part1->name=NULL;
				}
				Rfree(temp->part);
			} else temp1->part=NULL;
			++y;
		}
	}
	Rfree(datafile->keys);
	--datafile->nokeys;
	datafile->keys=Rmalloc(datafile->nokeys*sizeof(NRDkey));
	for(x=0,temp1=datafile->keys,temp=new_key;x<datafile->nokeys;++x,++temp,
		++temp1)
	{
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp->name=NULL;
		temp1->numparts=temp->numparts;
		if(temp->part!=NULL)
		{
			temp1->part=Rmalloc(temp1->numparts*sizeof(NRDpart));
			for(w=0,part=temp->part,part1=temp1->part;w<temp->numparts;
				++w,++part,++part1)
			{
				if(part->name!=NULL)
				{
					part1->name=stralloc(part->name);
					Rfree(part->name);
				} else part1->name=NULL;
			}
			Rfree(temp->part);
		} else temp1->part=NULL;
	}
	Rfree(new_key);
	makekeyfieldlist();
	if(klst>=keyfield->numlibs) klst=0;
	if(!FINDRSCLISTAPPlib(mtnfrsrc,"KEY LIST",klst,keyfield))
		updatersrc(mtnfrsrc,"KEY LIST");
	changedkeys=TRUE;
}
static void quit_field(RDArsrc *fldrsrc)
{
	if(fldrsrc!=NULL)
	{
		killwindow(fldrsrc);
		NULL_RSCS(fldrsrc);
	}
	if(changedfields) changedfile=TRUE;
	if(flddefaults!=NULL) FreeRDAdefault(flddefaults);
}
static void quit_fieldcls(RDArsrc *fldrsrc)
{
	changedfields=FALSE;
	quit_field(fldrsrc);
}
static void quit_fieldtest(RDArsrc *fldrsrc,void (*save_func)(...))
{
	readallwidgets(fldrsrc);
	if(COMPARE_RSRCS(fldrsrc,flddefaults,2))
	{
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the FIELD DEFINITION!\nDo want to Save these changes?",save_func,quit_fieldcls,FALSE,fldrsrc);
	} else {
		quit_field(fldrsrc);
	}
}
static void save_field(RDArsrc *fldrsrc)
{
	int x,value;
	NRDfield *fds;
	short y;

	if(FINDRSCGETINT(mtnfrsrc,"FIELD LIST",&value)) return;
	fds=datafile->fields+value;
	readallwidgets(fldrsrc);
	FINDRSCGETSTRING(fldrsrc,"FIELD NAME",&fds->name);
	FINDRSCGETINT(fldrsrc,"FIELD TYPES",&x);
	fds->type=x;
	FINDRSCGETSHORT(fldrsrc,"FIELD LENGTH",&y);
	fds->len=y;
	makefieldlist();
	if(value>=field->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(mtnfrsrc,"FIELD LIST",value,field))
		updatersrc(mtnfrsrc,"FIELD LIST");
	if(COMPARE_RSRCS(fldrsrc,flddefaults,2))
	{
		changedfields=TRUE;
	}
	quit_field(fldrsrc);
}
static void save_field_above(RDArsrc *fldrsrc)
{
	int x;

	if(FINDRSCGETINT(mtnfrsrc,"FIELD LIST",&x)) return;
	makespacefield(x);
	save_field(fldrsrc);
}
static void save_field_below(RDArsrc *fldrsrc)
{
	int x;

	if(FINDRSCGETINT(mtnfrsrc,"FIELD LIST",&x)) return;
	makespacefield(++x);
	FINDRSCSETINT(mtnfrsrc,"FIELD LIST",x);
	updatersrc(mtnfrsrc,"FIELD LIST");
	save_field(fldrsrc);
}
static void savefile_check(RDArsrc *mtnfrsrc)
{
	int engine=0,selected=0;
	char *modulename=NULL,*filename=NULL,*serverx=NULL;
	char *errorstring=NULL;
	NRDfield *field;
	NRDpart *part;
	NRDkey *key;
	short x,y,z;
	int keylength=0;
	char *tmp=NULL;

	readallwidgets(mtnfrsrc);
	if(FINDRSCGETINT(mtnfrsrc,"DBENGINES",&selected)) return;
	for(x=0;x<NE(supported_engines);++x)
	{
		if(!RDAstrcmp(supported_engines[x],dbengines->libs[selected]))
			break;
	}
	if(FINDRSCGETSTRING(mtnfrsrc,"MODULE NAME",&modulename)) return;
	memset(stemp,0,101);
#ifndef WIN32
	sprintf(stemp,"%s/rda/%s.FIL",CURRENTDIRECTORY,modulename);
#endif
#ifdef WIN32
	sprintf(stemp,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modulename);
#endif
	if(FINDRSCGETSTRING(mtnfrsrc,"FILENAME",&filename)) return;
	if(x<NE(supported_engines))
	{
		engine=(int)x;
	} else { 
		ERRORDIALOG("INVALID DATABASE ENGINE!","There is an incorrect Database Engine selected that is not supported.  Check your definition and retry.  If problem persist call your installer.",NULL,FALSE);
		prterr("Error invalid database engine selected in library [%s] definition [%s].",stemp,filename);
		return;
	}
	if(FINDRSCGETSTRING(mtnfrsrc,"DATABASE SERVER",&serverx)) return;
	if(engine==NRDCtreeEngine)
	{
		if(isEMPTY(serverx))
		{
			tmp=RDA_GetEnv("CTREE_DEFAULT_SERVER");
			if(isEMPTY(tmp))
			{
				ERRORDIALOG("INVALID SERVER NAME!","There is an incorrect Server Name in the DATABASE SERVER field, Faircom's Ctree requires a Server Name to run correctly.  Check your definition and retry.  If problem persist call your installer.",NULL,FALSE);
				prterr("Error invalid server name selected in library [%s] definition [%s].",stemp,filename);
				return;
			} else {
				prterr("Using DEFAULT_CTREE_SERVER for server name of [%s] in library [%s] definition [%s].",(tmp!=NULL?tmp:""),(stemp!=NULL?stemp:""),(filename!=NULL?filename:""));
			}
		}
		for(x=0,key=datafile->keys;x<datafile->nokeys;++x,++key)
		{
			for(z=0,part=key->part;z<key->numparts;++z,++part)
			{
				for(y=0,field=datafile->fields;y<datafile->nofields;++y,++field)
				{
				if(!RDAstrcmp(field->name,part->name))
				{
					if(field->len<1)
					{
						errorstring=Rmalloc(RDAstrlen(part->name)+230);
						sprintf(errorstring,"The keypart [%s] has a length of zero, Faircom's Ctree database requires all key fields have lengths greater than zero.  Check your definition and retry.  If problem persist call your installer.",(part->name==NULL?"":part->name)
);
						ERRORDIALOG("INVALID KEY PART LENGTH!",errorstring,NULL,FALSE);
						if(errorstring!=NULL) Rfree(errorstring);
						prterr("Error invalid field length selected in library [%s] definition [%s] fieldname [%s].",stemp,filename,(field->name==NULL?"":field->name));
						return;
					}
/*
					switch(field->type)
					{
						case SCROLLEDTEXT:
							ERRORDIALOG("INVALID KEY TYPE!","There is a keypart with an invalid field type of SCROLLEDTEXT in the key definitions.  Faircom's Ctree does not allow for Variable Length Fields in its key definitions.  Check your definition and retry.  If problem 
persist call your installer.",NULL,FALSE);
							prterr("Error Faircom's Ctree Key Part [%s] of invalid type SCROLLEDTEXT in library [%s] definition [%s].",part->name,stemp,filename);
							return;
						case VARIABLETEXT:
							ERRORDIALOG("INVALID KEY TYPE!","There is a keypart with an invalid field type of VARIABLETEXT in the key definitions.  Faircom's Ctree does not allow for Variable Length Fields in its key definitions.  Check your definition and retry.  If problem 
persist call your installer.",NULL,FALSE);
							prterr("Error Faircom's Ctree Key Part [%s] of invalid type VARIABLETEXT in library [%s] definition [%s].",part->name,stemp,filename);
							return;
						case PLAINTEXT:
						case PHONE:
						case CUSTOMTYPE:
						case ZIPCODE:
						case SOCSECNUM:
						case DATES:
						case TIMES:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
							break;
						case DECIMALV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DOUBLEV:
						case SDECIMALV:
						case SDOUBLEV:
						case SHORTV:
						case SSHORTV:
						case LONGV:
						case SLONGV:
						case BOOLNS:
						case CHARACTERS:
						case BUTTONS:
						case SCROLLEDLIST:
							break;
						default:
							prterr("Error unrecognized field type.");
							break;
					}
*/
				}
				}
			}
		}
	} else if(engine==NRDInformixIsamEngine)
	{
		for(x=0,key=datafile->keys;x<datafile->nokeys;++x,++key)
		{
			if(key->numparts>8)
			{
				errorstring=Rmalloc(RDAstrlen(key->name)+450);
				sprintf(errorstring,"There is an error in the total number of all key parts in this file definition, they are more than the limit of 8 keyparts.  Informix Isam database requires the total number of key parts to be equal to or less than 8 in all keys. The total number of key parts in the key [%s] of this definition would be [%d].  Check your definition and retry.  If problem persist call your installer.",key->name,key->numparts);
				ERRORDIALOG("INVALID NUMBER OF KEY PARTS!",errorstring,NULL,FALSE);
				if(errorstring!=NULL) Rfree(errorstring);
				prterr("Error Informix Isam too many Key parts (%d) in key [%s] in library [%s] definition [%s].",key->name,keylength,stemp,filename);
				return;
			}
			keylength=0;
			for(z=0,part=key->part;z<key->numparts;++z,++part)
			{
				for(y=0,field=datafile->fields;y<datafile->nofields;++y,++field)
				{
				if(!RDAstrcmp(field->name,part->name))
				{
					switch(field->type)
					{
						case SCROLLEDTEXT:
						case VARIABLETEXT:
						case PLAINTEXT:
						case PHONE:
						case CUSTOMTYPE:
						case ZIPCODE:
						case SOCSECNUM:
						case DATES:
						case TIMES:
						case EXPENDITURE:
						case REVENUE:
						case BALANCESHEET:
						case BEGINNINGBALANCE:
						case OPTIONALFIELDS:
							keylength+=field->len;
							break;
						case DECIMALV:
						case DOLLARS:
						case DOLLARS_NOCENTS:
						case DOUBLEV:
						case SDECIMALV:
						case SDOUBLEV:
							keylength+=sizeof(double);
							break;
						case SHORTV:
						case SSHORTV:
							keylength+=sizeof(short);
							break;
						case LONGV:
						case SLONGV:
							keylength+=sizeof(int);
							break;
						case BOOLNS:
						case CHARACTERS:
							keylength+=1;
							break;
						case BUTTONS:
						case SCROLLEDLIST:
							break;
						default:
							prterr("Error unrecognized field type.");
							break;
					}
				}
				}
			}
		}
		if(keylength>120)
		{
			errorstring=Rmalloc(350);
			sprintf(errorstring,"There is an error in the total size for all key parts, when added together they are greater than the limit of 120 bytes.  Informix Isam database requires the total key size to be equal to or less than 120 bytes.  The total key size of this definition would be [%d].  Check your definition and retry.  If problem persist call your installer.",keylength);
			ERRORDIALOG("INVALID KEY LENGTH!",errorstring,NULL,FALSE);
			if(errorstring!=NULL) Rfree(errorstring);
			prterr("Error Informix Isam Key too large (%d) in library [%s] definition [%s].",keylength,stemp,filename);
			return;
		}
		for(x=0,field=datafile->fields;x<datafile->nofields;++x,++field)
		{
			if(field->len<1)
			{
				errorstring=Rmalloc(RDAstrlen(field->name)+230);
				sprintf(errorstring,"There is an incorrect Field Lenth on the Field [%s], Informix Isam database requires all fields have lengths greater than zero.  Check your definition and retry.  If problem persist call your installer.",
					(field->name==NULL?"":field->name));
				ERRORDIALOG("INVALID FIELD LENGTH!",errorstring,NULL,FALSE);
				if(errorstring!=NULL) Rfree(errorstring);
				prterr("Error invalid field length selected in library [%s] definition [%s] fieldname [%s].",stemp,filename,(field->name==NULL?"":field->name));
				return;
			}
		}
	}
	for(x=0,key=datafile->keys;x<datafile->nokeys;++x,++key)
	{
		for(z=0,part=key->part;z<key->numparts;++z,++part)
		{
			for(y=0,field=datafile->fields;y<datafile->nofields;++y,++field)
			{
				if(!RDAstrcmp(part->name,field->name)) break;
			}
			if(y>=datafile->nofields)
			{
				errorstring=Rmalloc(RDAstrlen(part->name)+RDAstrlen(key->name)+RDAstrlen(stemp)+RDAstrlen(filename)+200);
				sprintf(errorstring,"Invalid key part name of [%s] in key [%d][%s], it does not exist as a field in the file [%s][%s].  Check your definition and retry.  If problem persist call your installer.",(part->name!=NULL?part->name:""),z,(key->name!=NULL?key->name:""),stemp,(filename!=NULL?filename:""));
				ERRORDIALOG("INVALID KEY PART NAME!",errorstring,NULL,FALSE);
				if(errorstring!=NULL) Rfree(errorstring);
				prterr("Error invalid key part name of [%s] in key [%d][%s] library [%s] definition [%s].",(part->name!=NULL?part->name:""),z,(key->name!=NULL?key->name:""),stemp,(filename!=NULL?filename:""));
				return;
			}
		}
	}
	savefile(mtnfrsrc);
}
static void savefile(RDArsrc *mtnfrsrc)
{
	int fd=(-1),engine,doit=FALSE,selected=0;
	char *modulename=NULL,*filename=NULL,*serverx=NULL;
	short z,x,y,offset;
	short offcount;
	NRDfield *field,*OFIELDS,*field1;
	NRDpart *part;
	NRDkey *key;
	char def_server=FALSE;
	char *tmp=NULL;
	APPlib *nonkeyfld=NULL;

	if(FINDRSCGETSTRING(mtnfrsrc,"MODULE NAME",&modulename)) return;
	if(FINDRSCGETINT(mtnfrsrc,"DBENGINES",&selected)) return;
	if(SEC_USERS_FILENO!=(-1))
	{
		CLSNRD(SEC_USERS_FILENO);
		SEC_USERS_FILENO=(-1);
	}
	for(x=0;x<NE(supported_engines);++x)
	{
		if(!RDAstrcmp(supported_engines[x],dbengines->libs[selected]))
			break;
	}
	if(x<NE(supported_engines))
	{
		engine=x;
	} else engine=0;
	if(FINDRSCGETSTRING(mtnfrsrc,"FILENAME",&filename)) return;
	if(FINDRSCGETSTRING(mtnfrsrc,"DATABASE SERVER",&serverx)) return;
	if(engine==NRDCtreeEngine)
	{
		if(isEMPTY(serverx))
		{
			tmp=RDA_GetEnv("CTREE_DEFAULT_SERVER");
			serverx=stralloc(tmp);
			def_server=TRUE;
		}
	}
	memset(stemp,0,101);
#ifndef WIN32
	sprintf(stemp,"%s/rda/%s.FIL",CURRENTDIRECTORY,modulename);
#endif
#ifdef WIN32
	sprintf(stemp,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modulename);
#endif
	fd=open(stemp,O_RDONLY,0666);
	if(fd!=(-1))
	{
		if(access(stemp,02))
		{
			ERRORDIALOG("Cannot Read File Definition","The user doesn't have permission to read the file definition library.  Check your permissions and retry.  Call your installer.",NULL,FALSE);
			prterr("Error in permission, user doesn't have write access to File Definition Library [%s].",stemp);
			doit=FALSE;
		} else {
			doit=TRUE;
		}
		if(close(fd)==(-1)) { prterr("Error can't close Binary Library [%s].",stemp); }
	} else doit=TRUE;
	if(doit)
	{
	if((engine==NRDMyIsamEngine) || (engine==NRDMySQLXpertEngine) ||
		(engine==NRDMySQLEngine) || (engine==NRDMySQLAutoEngine))
	{
		nonkeyfld=APPlibNEW();
		OFIELDS=Rmalloc(datafile->nofields*sizeof(NRDfield));
		y=0;
		key=datafile->keys+0;
		for(x=0,field=datafile->fields;x<datafile->nofields;++x,++field)
		{
			if(IsKeyPart(key,field->name))
			{
				field1=OFIELDS+y;
				field1->name=stralloc(field->name);
				field1->type=field->type;
				field1->len=field->len;
				++y;
			} else {
				addAPPlib(nonkeyfld,field->name);
			}
		}
		SORTAPPlib(nonkeyfld);
		for(z=0;z<nonkeyfld->numlibs;++z)
		{
			for(x=0,field=datafile->fields;x<datafile->nofields;++x,++field)
			{
				if(!RDAstrcmp(field->name,nonkeyfld->libs[z]) && field->type!=VARIABLETEXT && field->type!=SCROLLEDTEXT && field->len>0)
				{
					field1=OFIELDS+y;
					field1->name=stralloc(field->name);
					field1->type=field->type;
					field1->len=field->len;
					++y;
					break;
				}
			}
		}
		for(z=0;z<nonkeyfld->numlibs;++z)
		{
			for(x=0,field=datafile->fields;x<datafile->nofields;++x,++field)
			{
				if(!RDAstrcmp(field->name,nonkeyfld->libs[z]) && (field->type==VARIABLETEXT || field->type==SCROLLEDTEXT || field->len<1))
				{
					field1=OFIELDS+y;
					field1->name=stralloc(field->name);
					field1->type=field->type;
					field1->len=field->len;
					++y;
					break;
				}
			}
		}
		if(nonkeyfld!=NULL) freeapplib(nonkeyfld);
	} else {
		OFIELDS=Rmalloc(datafile->nofields*sizeof(NRDfield));
		y=0;
		for(x=0,field=datafile->fields;x<datafile->nofields;++x,++field)
		{
			if(field->type!=VARIABLETEXT && field->type!=SCROLLEDTEXT
			 	&& field->len>0)
			{
				field1=OFIELDS+y;
				field1->name=stralloc(field->name);
				field1->type=field->type;
				field1->len=field->len;
				++y;
			}
		}
		for(x=0,field=datafile->fields;x<datafile->nofields;++x,++field)
		{
			if(field->type==VARIABLETEXT || field->type==SCROLLEDTEXT
			 	|| field->len<1)
			{
				field1=OFIELDS+y;
				field1->name=stralloc(field->name);
				field1->type=field->type;
				field1->len=field->len;
				++y;
			}
		}
	}
	for(x=0,field=datafile->fields;x<datafile->nofields;++x,++field)
	{
		if(field->name!=NULL) Rfree(field->name);
	}
	Rfree(datafile->fields);
	datafile->fields=Rmalloc(datafile->nofields*sizeof(NRDfield));
	for(x=0,field1=OFIELDS,field=datafile->fields;x<datafile->nofields;++x,++field,++field1)
	{
		field->name=stralloc(field1->name);
		if(field1->name!=NULL) Rfree(field1->name);
		field->type=field1->type;
		field->len=field1->len;
	}
	Rfree(OFIELDS);
	for(x=0,key=datafile->keys;x<datafile->nokeys;++x,++key)
	{
		for(z=0,part=key->part;z<key->numparts;++z,++part)
		{
			offset=0;
			offcount=(-1);
			for(y=0,field=datafile->fields;y<datafile->nofields;++y,
				++field)
			{
				if(field->type==SCROLLEDTEXT || field->type==VARIABLETEXT || field->len<1)
				{
					++offcount;
				}
				if(!RDAstrcmp(field->name,part->name))
				{
					if(engine==NRDMyIsamEngine)
					{
						part->offset=offset;
					} else if(engine==NRDCtreeEngine || engine==NRDLocalCtreeEngine)
					{
						if(field->type==SCROLLEDTEXT || field->type==VARIABLETEXT || field->len<1)
						{
							part->offset=offcount;
						} else part->offset=offset;
					} else part->offset=offset;
					break;
				}
				switch(field->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
						if(engine==NRDMyIsamEngine)
						{
							offset+=field->len+4;
						} else if(engine==NRDInformixIsamEngine)
						{ 
							offset+=field->len;
						} else offset+=field->len+1;
						break;
					case PLAINTEXT:
					case PHONE:
					case CUSTOMTYPE:
					case ZIPCODE:
					case SOCSECNUM:
					case DATES:
					case TIMES:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						if(engine==NRDMyIsamEngine && field->len==0)
						{
							offset+=field->len+4;
						} else offset+=field->len;
						break;
					case DECIMALV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DOUBLEV:
					case SDECIMALV:
					case SDOUBLEV:
						offset+=sizeof(double);
						break;
					case SHORTV:
					case SSHORTV:
						offset+=sizeof(short);
						break;
					case SCROLLEDLIST:
					case LONGV:
					case SLONGV:
						offset+=sizeof(int);
						break;
					case BOOLNS:
					case CHARACTERS:
						offset+=1;
						break;
					case BUTTONS:
						break;
					default:
						prterr("Error unrecognized field type.");
						break;
				}
			}
		}
	}
	if(def_server==TRUE)
	{
		x=CHANGE_DATABASE_FILE(modulename,engine,filename,datafile->nofields,datafile->fields,datafile->nokeys,datafile->keys,NULL);
	} else {
		x=CHANGE_DATABASE_FILE(modulename,engine,filename,datafile->nofields,datafile->fields,datafile->nokeys,datafile->keys,serverx);
	}
	if(x==(-1))
	{
		prterr("Couldn't create file [%s].",filename);
	} else {
		CLSNRD(x);
	}
	if(SEC_USERS_FILENO==(-1))
	{
		SEC_USERS_FILENO=OPNNRD("SECURITY","USERS");	
	}
	getmodulelist();
	}
	doexitfile(mtnfrsrc);
}
static void createfile(char *modulename,int engine,char *filename,
		int nofields,NRDfield *fieldx,int nokeys,NRDkey *keyx,
		char *servery)
{
	int x=0,fd=(-1);

	memset(stemp,0,101);
#ifndef WIN32
	sprintf(stemp,"%s/rda/%s.FIL",CURRENTDIRECTORY,modulename);
#endif
#ifdef WIN32
	sprintf(stemp,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modulename);
#endif
	fd=open(stemp,O_RDONLY,0666);
	if(fd!=(-1))
	{
		if(access(stemp,02))
		{
			prterr("Error in permission, user doesn't have write access to File Definition Library [%s].",stemp);
		}
		if(close(fd)==(-1)) { prterr("Error can't close Binary Library [%s].",stemp); }
	}
	if((x=OPNNRD(modulename,filename))==(-1))
	{
		DELETENRD(modulename,filename,engine,servery);
		x=CRTNRD(modulename,engine,filename,nofields,fieldx,nokeys,keyx,servery);
		if(x==(-1))
		{
			prterr("Couldn't create Module [%s] File [%s], CRTNRD() failed.",modulename,filename);
		} 
	} else {
		prterr("Couldn't create Module [%s] File [%s], File already exist.",modulename,filename);
	}
} 
static void add_field_above(RDArsrc *mtnfrsrc)
{
	fieldscreen(0,NULL,0,0,save_field_above);
}
static void add_field_below(RDArsrc *mtnfrsrc)
{
	fieldscreen(0,NULL,0,0,save_field_below);
}
static void delete_field(RDArsrc *mtnfrsrc)
{
	short x,y;
	NRDfield *temp,*temp1,*new_fields;
	int value;

	readallwidgets(mtnfrsrc);
	if(FINDRSCGETINT(mtnfrsrc,"FIELD LIST",&value)) return;
	if(datafile->nofields==1) return;
	new_fields=Rmalloc((datafile->nofields-1)*sizeof(NRDfield));
	y=0;
	for(x=0,temp=datafile->fields;x<datafile->nofields;++x,++temp)
	{
		temp1=new_fields+y;
		if(x!=value)
		{
			temp1->type=temp->type;
			if(temp->name!=NULL)
			{
				temp1->name=stralloc(temp->name);
				Rfree(temp->name);
			} else temp1->name=NULL;
			temp1->len=temp->len;
			++y;
		}
	}
	Rfree(datafile->fields);
	--datafile->nofields;
	datafile->fields=Rmalloc(datafile->nofields*sizeof(NRDfield));
	for(x=0,temp1=datafile->fields,temp=new_fields;x<datafile->nofields;++x,
		++temp,++temp1)
	{
		temp1->type=temp->type;
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->len=temp->len;
	}
	Rfree(new_fields);
	makefieldlist();
	if(value>=field->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(mtnfrsrc,"FIELD LIST",value,field))
		updatersrc(mtnfrsrc,"FIELD LIST");
	changedfields=TRUE;
}
static void copy_field(RDArsrc *mtnfrsrc)
{
	int x;
	NRDfield *f;

	if(FINDRSCGETINT(mtnfrsrc,"FIELD LIST",&x)) return;
	f=datafile->fields+x;
	x+=1;
	if(f!=NULL)
	{
		fieldscreen(x,f->name,f->type,f->len,save_field_below);
	} else {
		fieldscreen(x,NULL,0,0,save_field_below);
	}
}
static void edit_field(RDArsrc *mtnfrsrc)
{
	int x;
	NRDfield *f;

	if(FINDRSCGETINT(mtnfrsrc,"FIELD LIST",&x)) return;
	f=datafile->fields+x;
	fieldscreen(x,f->name,f->type,f->len,save_field);
}
static void save_key_above(RDArsrc *keyfldrsrc)
{
	int x;

	if(FINDRSCGETINT(mtnfrsrc,"KEY LIST",&x)) return;
	makespacekeys(x);
	save_key(keyfldrsrc);
}
static void add_key_above(RDArsrc *mtnfrsrc)
{
	int x;

	if(FINDRSCGETINT(mtnfrsrc,"KEY LIST",&x)) return;
	holdparts=NULL;
	noholdparts=0;
	keyfieldscreen(x,NULL,0,save_key_above);
} 
static void save_key_below(RDArsrc *keyfldrsrc)
{
	int x;

	if(FINDRSCGETINT(mtnfrsrc,"KEY LIST",&x)) return;
	makespacekeys(++x);
	FINDRSCSETINT(mtnfrsrc,"KEY LIST",x);
	save_key(keyfldrsrc);
}
static void add_key_below(RDArsrc *mtnfrsrc)
{
	int x;

	if(FINDRSCGETINT(mtnfrsrc,"KEY LIST",&x)) return;
	holdparts=NULL;
	noholdparts=0;
	keyfieldscreen(++x,NULL,0,save_key_below);
}
static void edit_key(RDArsrc *mtnfrsrc)
{
	int x,y;
	NRDkey *k;
	NRDpart *part,*part1;

	if(FINDRSCGETINT(mtnfrsrc,"KEY LIST",&x)) return;
	k=datafile->keys+x;
	if(k->part!=NULL)
	{
		noholdparts=k->numparts;
		holdparts=Rmalloc(k->numparts*sizeof(NRDpart));
		for(y=0,part1=holdparts,part=k->part;y<k->numparts;++y,++part,++part1)
		{
			if(part->name!=NULL)
			{
				part1->name=stralloc(part->name);
			}
			part1->offset=part->offset;
		}
	}
	keyfieldscreen(x,k->name,0,save_key);
}
static void copy_key(RDArsrc *mtnfrsrc)
{
	int x,y;
	NRDkey *k;
	NRDpart *part,*part1;

	if(FINDRSCGETINT(mtnfrsrc,"KEY LIST",&x)) return;
	k=datafile->keys+x;
	if(k->part!=NULL)
	{
		noholdparts=k->numparts;
		holdparts=Rmalloc(k->numparts*sizeof(NRDpart));
		for(y=0,part1=holdparts,part=k->part;y<k->numparts;++y,++part,++part1)
		{
			if(part->name!=NULL)
			{
				part1->name=stralloc(part->name);
			}
			part1->offset=part->offset;
		}
	}
	keyfieldscreen(++x,k->name,0,save_key_below);
}
static void makersrcscreen(char *module,char *deff,short engine,char *server)
{
	int z=0;
	int eng_type;
	char *temp=NULL;

	makefieldlist();
	makekeyfieldlist();
	FINDRSCSETSTRING(mtnfrsrc,"MODULE NAME",(module!=NULL ? module:""));
	FINDRSCSETSTRING(mtnfrsrc,"FILENAME",(deff!=NULL ? deff:""));
	FINDRSCSETSTRING(mtnfrsrc,"DATABASE SERVER",(server!=NULL ? server:""));
	temp=supported_engines[engine];
	for(eng_type=0;eng_type<dbengines->numlibs;++eng_type)
	{
		if(!RDAstrcmp(temp,dbengines->libs[eng_type])) break;
	}
	if(eng_type>=dbengines->numlibs) eng_type=0;
	FINDRSCSETINT(mtnfrsrc,"DBENGINES",eng_type);
	FINDRSCLISTAPPlib(mtnfrsrc,"FIELD LIST",z,field);
	FINDRSCLISTAPPlib(mtnfrsrc,"KEY LIST",z,keyfield);
	changedfile=FALSE;
	setdbengines(mtnfrsrc);
	mtnfdefaults=GetDefaults(mtnfrsrc);
	APPmakescrn(mtnfrsrc,TRUE,NULL,NULL,FALSE);
}
#ifdef USE_CTREE
static void odbcmodule(RDArsrc *mainrsrc)
{
	FILE *fp=NULL;
	int m=0,x=0;
	char tempx[512],tempx[1024];

	readallwidgets(mainrsrc);
	FINDRSCGETINT(mainrsrc,"MODULE LIST",&m);
	if(!RDAstrcmp(file_defs->libs[0],"Contains No File Definitions"))
	{
		return;
	}
	sprintf(stemp,"%s.TXT",modlst->libs[m]);
	memset(tempx,0,1024);
	sprintf(tempx,"%s/%s",CURRENTDIRECTORY,stemp);
	fp=fopen(tempx,"w");
	if(fp!=NULL)
	{
		fprintf(fp,"DATABASE %s.DB\n",modlst->libs[m]);
		fprintf(fp,"\n");
		for(x=0;x<file_defs->numlibs;++x)
		{
#ifdef WIN32
			sprintf(tempx,"TABLE %s(%s\\%s\\%s.dat)",file_defs->libs[x],CURRENTDIRECTORY,modlst->libs[m],file_defs->libs[x]);
#endif
#ifndef WIN32
			sprintf(tempx,"TABLE %s(%s/%s/%s.dat)",file_defs->libs[x],CURRENTDIRECTORY,modlst->libs[m],file_defs->libs[x]);
#endif
			fprintf(fp,"%s\n",tempx);
		}
		fprintf(fp,"\n");
		fclose(fp);
	}
	memset(tempx,0,1024);
	sprintf(stemp,"%s.DB",modlst->libs[m]);
	sprintf(tempx,"%s/%s",CURRENTDIRECTORY,stemp);
	fp=fopen(tempx,"w");
	if(fp!=NULL)
	{
		for(x=0;x<file_defs->numlibs;++x)
		{
#ifdef WIN32
			sprintf(tempx,"%s\t%s\\%s\\%s.dat",file_defs->libs[x],CURRENTDIRECTORY,modlst->libs[m],file_defs->libs[x]);
#endif
#ifndef WIN32
			sprintf(tempx,"%s\t%s/%s/%s.dat",file_defs->libs[x],CURRENTDIRECTORY,modlst->libs[m],file_defs->libs[x]);
#endif
			fprintf(fp,"%s\n",tempx);
		}
		fprintf(fp,"\n");
		fclose(fp);
	}
}
#endif
static void okfunction(RDArsrc *mainrsrc)
{
	int s,m=0;
	char *deff,*serverx=NULL;
	NRDfield *fields;
	short engine;
	NRDkey *keys;
	NRDpart *part;

	readallwidgets(mainrsrc);
	FINDRSCGETINT(mainrsrc,"MODULE LIST",&m);
	if(FINDRSCGETINT(mainrsrc,"FILE LIST",&s)) 
	{
		prterr("Resource %s not found.","FILE LIST");
		deff=NULL;
		serverx=NULL;
	} else {
		if(!RDAstrcmp(file_defs->libs[s],"Contains No File Definitions"))
		{
			deff=NULL;
		} else deff=stralloc(file_defs->libs[s]);
	}
	if(ADVGETBIN(modlst->libs[m],deff,&engine,&datafile->nofields,&datafile->fields,&datafile->nokeys,&datafile->keys,&serverx))
	{
		/* didn't get binary info*/
		datafile->nofields=1;
		datafile->fields=Rmalloc(sizeof(NRDfield));
		fields=datafile->fields;
		fields->type=BOOLNS;
		fields->name=stralloc("DELETEFLAG");
		fields->len=1;
		datafile->nokeys=1;
		datafile->keys=Rmalloc(sizeof(NRDkey));
		keys=datafile->keys;
		keys->name=stralloc("Error No Keys");
		keys->numparts=1;
		part=keys->part;
		part=Rmalloc(sizeof(NRDpart));
		part->name=NULL;
		part->offset=0;
	}
	makersrcscreen(modlst->libs[m],deff,engine,serverx);
	if(serverx!=NULL) Rfree(serverx);
	if(deff!=NULL) Rfree(deff);
}
static void new_file(RDArsrc *mainrsrc)
{
	int m=0;
	NRDfield *fields;
	NRDkey *keys;
	NRDpart *part;

	FINDRSCGETINT(mainrsrc,"MODULE LIST",&m);
	datafile=dffilenew();
	datafile->nofields=1;
	datafile->fields=Rmalloc(sizeof(NRDfield));
	fields=datafile->fields;
	fields->type=BOOLNS;
	fields->name=stralloc("DELETEFLAG");
	fields->len=1;
	datafile->nokeys=1;
	datafile->keys=Rmalloc(sizeof(NRDkey));
	keys=datafile->keys;
	keys->name=stralloc("Error No Keys");
	keys->numparts=1;
	keys->part=Rmalloc(sizeof(NRDpart));
	part=keys->part;
	part->name=NULL;
	part->offset=0;
	makersrcscreen(modlst->libs[m],NULL,0,NULL);
}
static void delete_file(RDArsrc *mainrsrc)
{
	int s,m=0;
	char *libx=NULL,*modulename=NULL,*serverx=NULL;
	NRDkey *key,*keyx=NULL;
	NRDfield *fld,*fieldx=NULL;
	NRDpart *part;
	short x,y,engine,nokeys=0,nofields=0;

	readallwidgets(mainrsrc);
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&m)) return;
	if(FINDRSCGETINT(mainrsrc,"FILE LIST",&s)) return;
	modulename=adddashes(modlst->libs[m]);	
	ADVGETBIN(modulename,file_defs->libs[s],&engine,&nofields,&fieldx,&nokeys,&keyx,&serverx);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modulename)+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modulename);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modulename);
#endif
	if(deletelibbin(libx,file_defs->libs[s]))
	{
		ERRORDIALOG("Cannot Write File Definition","The user doesn't have permission to write the file definition library.  Check your permissions and retry.  Call your installer.",NULL,FALSE);
		prterr("Error in permission, user doesn't have write access to File [%s].",libx);
	} else {
		DELETENRD(modulename,file_defs->libs[s],engine,serverx);
		getmodulelist();
	}
	if(serverx!=NULL) Rfree(serverx);
	if(fieldx!=NULL)
	{
		for(x=0,fld=fieldx;x<nofields;++x,++fld)
		{
			FreeNRDfield(fld);
		}
		Rfree(fieldx);
	}
	if(keyx!=NULL)
	{
		for(x=0,key=keyx;x<nokeys;++x,++key)
		{
			if(key->name!=NULL) Rfree(key->name);
			if(key->part!=NULL)
			{
				for(y=0,part=key->part;y<key->numparts;++y,++part)
				{
					if(part->name!=NULL) Rfree(part->name);
				}
				Rfree(key->part);
			}
		}
		Rfree(keyx);
	}
	if(libx!=NULL) Rfree(libx);
	if(modulename!=NULL) Rfree(modulename);
}
static void deletefiletest(RDArsrc *mainrsrc)
{
	int s=0;

	if(FINDRSCGETINT(mainrsrc,"FILE LIST",&s)) return;
	if(!RDAstrcmp(file_defs->libs[s],"Contains No File Definitions")) return;
	WARNINGDIALOG("DELETE WARNING DIALOG SCREEN","DELETE THIS FILE?","Deleting this file will cause the FILE DEFINITON and the DATA stored the corresponding data files to be lost.\nAre you sure that you want to DELETE this FILE?",
		delete_file,NULL,FALSE,mainrsrc);
}
void getmodulelistonly(void)
{
	int x;
	char *tmp=NULL,*dirx=NULL;

	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	for(x=0;findfile(dirx,"*.FIL",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(modlst,tmp);
	}
	if(modlst->numlibs<1)
	{
		addAPPlib(modlst,"Contains No Database Libraries");
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	SORTAPPlib(modlst);
}
void getmodulelist(void)
{
	int selected=0;

	FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected);
	if(modlst!=NULL) freeapplib(modlst);
	modlst=APPlibNEW();
	getmodulelistonly();
	if(selected>=modlst->numlibs) selected=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"MODULE LIST",selected,modlst))
	{
		updatersrc(mainrsrc,"MODULE LIST");
	}
	domodulelist(mainrsrc);
}
void domodulelist(RDArsrc *mainrsrc)
{
	int selected=0,selectedf=0;
	char *libx=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	FINDRSCGETINT(mainrsrc,"FILE LIST",&selectedf);
	if(file_defs!=NULL) freeapplib(file_defs); 
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlst->libs[selected])+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modlst->libs[selected]);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modlst->libs[selected]);
#endif
	file_defs=getlibnames(libx,TRUE);
	if(libx!=NULL) Rfree(libx);
	if(file_defs==NULL)
	{
		file_defs=APPlibNEW();
		addAPPlib(file_defs,"Contains No File Definitions");
	}
	if(selectedf>=file_defs->numlibs) selectedf=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"FILE LIST",selectedf,file_defs))
		updatersrc(mainrsrc,"FILE LIST");
}
static void setdbenginescb(RDArsrc *mtnrsrc)
{
	readwidget(mtnrsrc,"DBENGINES");
	readwidget(mtnrsrc,"DATABASE SERVER");
	setdbengines(mtnrsrc);
}
static void setdbengines(RDArsrc *mtnrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(mtnrsrc,"DBENGINES",&selected)) return;
	if((!RDAstrcmp(dbengines->libs[selected],"Faircom's Ctree")) || (!strncmp(dbengines->libs[selected],"MySQL Server",12)))
	{
		FINDRSCSETSENSITIVE(mtnrsrc,"DATABASE SERVER",TRUE);
		FINDRSCSETEDITABLE(mtnrsrc,"DATABASE SERVER",TRUE);
	} else {
		FINDRSCSETSENSITIVE(mtnrsrc,"DATABASE SERVER",FALSE);
		FINDRSCSETEDITABLE(mtnrsrc,"DATABASE SERVER",FALSE);
	}
}
void define_file_resources(void)
{
	int y=0;
	short x=0;
	NRDfield *fields;
	NRDkey *keys;
	NRDpart *part;

	datafile=dffilenew();
	datafile->nofields=1;
	datafile->fields=Rmalloc(sizeof(NRDfield));
	fields=datafile->fields;
	fields->type=BOOLNS;
	fields->name=stralloc("DELETEFLAG");
	fields->len=1;
	datafile->nokeys=1;
	datafile->keys=Rmalloc(sizeof(NRDkey));
	keys=datafile->keys;
	keys->name=stralloc("Error No Keys");
	keys->numparts=1;
	keys->part=Rmalloc(sizeof(NRDpart));
	part=keys->part;
	part->name=NULL;
	part->offset=0;
	mtnfrsrc=RDArsrcNEW("DATABASE","MAINTAIN FILE");
	field=APPlibNEW();
	keyfield=APPlibNEW();
	addstdrsrc(mtnfrsrc,"MODULE NAME",PLAINTEXT,15,NULL,TRUE);
	addstdrsrc(mtnfrsrc,"FILENAME",VARIABLETEXT,0,NULL,TRUE);
	addstdrsrc(mtnfrsrc,"DATABASE SERVER",VARIABLETEXT,0,NULL,TRUE);
	addlstrsrc(mtnfrsrc,"DBENGINES",0,TRUE,setdbenginescb,dbengines->numlibs,
		&dbengines->libs,NULL);
	addlstrsrc(mtnfrsrc,"FIELD LIST",0,TRUE,NULL,field->numlibs,
		&field->libs,NULL);
	addbtnrsrc(mtnfrsrc,"ADD FIELD ABOVE",TRUE,add_field_above,NULL);
	addbtnrsrc(mtnfrsrc,"ADD FIELD BELOW",TRUE,add_field_below,NULL);
	addbtnrsrc(mtnfrsrc,"COPY FIELD",TRUE,copy_field,NULL);
	addbtnrsrc(mtnfrsrc,"DELETE FIELD",TRUE,delete_field,NULL);
	addbtnrsrc(mtnfrsrc,"SELECT FIELD",TRUE,edit_field,NULL);
	addlstrsrc(mtnfrsrc,"KEY LIST",0,TRUE,NULL,keyfield->numlibs,
		&keyfield->libs,NULL);
	addbtnrsrc(mtnfrsrc,"ADD KEY ABOVE",TRUE,add_key_above,NULL);
	addbtnrsrc(mtnfrsrc,"ADD KEY BELOW",TRUE,add_key_below,NULL);
	addbtnrsrc(mtnfrsrc,"DELETE KEY",TRUE,delete_key,NULL);
	addbtnrsrc(mtnfrsrc,"SELECT KEY",TRUE,edit_key,NULL);
	addbtnrsrc(mtnfrsrc,"COPY KEY",TRUE,copy_key,NULL);
	addrfcbrsrc(mtnfrsrc,"SAVE",TRUE,savefile_check,NULL);
	addrfcbrsrc(mtnfrsrc,"QUIT",TRUE,doexitfiletest,NULL);
	addrfcbrsrc(mtnfrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mtnfrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
/*lint -e611 */
	addrfcbrsrc(mtnfrsrc,"PRINT FILE DEFINITION",TRUE,printdefinition,(void *)printfile);
/*lint +e611 */
	fldrsrc=RDArsrcNEW("DATABASE","MAINTAIN FIELD");
	if(ftypes!=NULL) freeapplib(ftypes);
	ftypes=APPlibNEW();
	for(x=0;x<28;++x)
	{
		addAPPlib(ftypes,standardfieldtypes[x]);
	}
	addstdrsrc(fldrsrc,"FIELD NUMBER",LONGV,5,&y,FALSE);
	addstdrsrc(fldrsrc,"FIELD NAME",VARIABLETEXT,0,NULL,TRUE);
	addlstrsrc(fldrsrc,"FIELD TYPES",0,TRUE,NULL,ftypes->numlibs,
		&ftypes->libs,NULL);
	addstdrsrc(fldrsrc,"FIELD LENGTH",SHORTV,4,&x,TRUE);
	addrfcbrsrc(fldrsrc,"SAVE",TRUE,save_field,NULL);
/*lint -e611 */
	addrfcbrsrc(fldrsrc,"QUIT",TRUE,quit_fieldtest,(void *)save_field);
/*lint +e611 */
	addrfcbrsrc(fldrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(fldrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	keyfldrsrc=RDArsrcNEW("DATABASE","MAINTAIN KEYFIELD");
	addstdrsrc(keyfldrsrc,"KEY NUMBER",LONGV,5,&y,FALSE);
	addstdrsrc(keyfldrsrc,"KEY NAME",VARIABLETEXT,0,NULL,TRUE);
	keyparts=APPlibNEW();
	addlstrsrc(keyfldrsrc,"KEY PARTS LIST",0,TRUE,NULL,
		keyparts->numlibs,&keyparts->libs,NULL);
	addbtnrsrc(keyfldrsrc,"ADD PART ABOVE",TRUE,add_part_above,NULL);
	addbtnrsrc(keyfldrsrc,"ADD PART BELOW",TRUE,add_part_below,NULL);
	addbtnrsrc(keyfldrsrc,"SELECT",TRUE,edit_part,NULL);
	addbtnrsrc(keyfldrsrc,"COPY",TRUE,copy_part,NULL);
	addbtnrsrc(keyfldrsrc,"DELETE PART",TRUE,delete_part,NULL);
	addrfcbrsrc(keyfldrsrc,"SAVE",TRUE,save_key,NULL);
/*lint -e611 */
	addrfcbrsrc(keyfldrsrc,"QUIT",TRUE,quit_keyfldtest,(void *)save_key);
/*lint +e611 */
	addrfcbrsrc(keyfldrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(keyfldrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	keypartrsrc=RDArsrcNEW("DATABASE","MAINTAIN KEYPART");	
	addstdrsrc(keypartrsrc,"PART NUMBER",LONGV,8,&y,FALSE);
	fldlist=APPlibNEW();
	addlstrsrc(keypartrsrc,"KEY PART NAME",0,TRUE,NULL,
		fldlist->numlibs,&fldlist->libs,NULL);
	addrfcbrsrc(keypartrsrc,"SAVE",TRUE,save_part,NULL);
/*lint -e611 */
	addrfcbrsrc(keypartrsrc,"QUIT",TRUE,quit_parttest,(void *)save_part);
/*lint +e611 */
	addrfcbrsrc(keypartrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(keypartrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0;
	short engine=0,nofields=0,nokeys=0;
	NRDfield *fieldx=NULL;
	NRDkey *keyx=NULL;
	char *servery=NULL;

	if(InitializeSubsystems(argc,argv,"DATABASE","DEFINE FILES")) 
	{
		return;
	}
	DoHeaders();
	if(dbengines!=NULL) freeapplib(dbengines);
	dbengines=APPlibNEW();
	addAPPlib(dbengines,supported_engines[0]);
#ifdef INFORMIX_ISAM
	addAPPlib(dbengines,supported_engines[1]);
#endif
#ifdef USE_MYISAM
	addAPPlib(dbengines,supported_engines[2]);
#endif
#ifdef USE_MYSQL
	addAPPlib(dbengines,supported_engines[3]);
	addAPPlib(dbengines,supported_engines[4]);
	addAPPlib(dbengines,supported_engines[10]);
#endif
#ifdef USE_CTREE
	addAPPlib(dbengines,supported_engines[5]);
	addAPPlib(dbengines,supported_engines[6]);
#endif
#ifdef USE_BERKELEY_BTREE
	addAPPlib(dbengines,supported_engines[7]);
#endif
	modlst=APPlibNEW();
	getmodulelistonly();

	x=0;
	if(argc>1)
	{
		if(argc==2)
		{
			for(x=0;x<modlst->numlibs;++x)
			{
				if(!RDAstrcmp(argv[1],modlst->libs[x])) break;
			}
			if(x>=modlst->numlibs) x=0;
		} else if(!isEMPTY(argv[1]) && !isEMPTY(argv[2]))
		{
			if(!ADVGETBIN(argv[1],argv[2],&engine,&nofields,&fieldx,&nokeys,&keyx,&servery))
			{
				createfile(argv[1],engine,argv[2],nofields,fieldx,nokeys,keyx,servery);
				if(servery!=NULL) Rfree(servery);
/* THIS SHOULD FREE IT's FIELDS AND STUFF */
/* THOUGH IT IS IMMEDIATELY EXITING AND THE SYSTEM SHOULD DEALLOCATE */
/* WHOMEVER WROTE THIS PLEASE CHANGE SO THAT YOU'LL REMEMBER */
			} else {
				prterr("Error - Cannot Read Binary for Module [%s], File [%s].",argv[1],argv[2]);
			}
			doexitsub();
			std::exit;
		}
	}
	mainrsrc=RDArsrcNEW("DATABASE","DEFINE FILES");
	addlstrsrc(mainrsrc,"MODULE LIST",&x,TRUE,domodulelist,modlst->numlibs,
		&modlst->libs,NULL);
	domodulelist(mainrsrc);
	addlstrsrc(mainrsrc,"FILE LIST",0,TRUE,NULL,file_defs->numlibs,
		&file_defs->libs,NULL);
	addrfcbrsrc(mainrsrc,"ADD",TRUE,new_file,NULL);
	addrfcbrsrc(mainrsrc,"DELETE FILE",TRUE,deletefiletest,NULL);
	addrfcbrsrc(mainrsrc,"SELECT",TRUE,okfunction,NULL);
#ifdef USE_CTREE
	if(USE_ODBC==TRUE)
	{
		addrfcbrsrc(mainrsrc,"ODBC",TRUE,odbcmodule,NULL);
	}
#endif
/*lint -e611 */
	addrfcbrsrc(mainrsrc,"PRINT MODULE",TRUE,printdefinition,(void *)printmodule);
/*lint +e611 */
	addrfexrsrc(mainrsrc,"QUIT",TRUE,doexit,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	define_file_resources();
	APPmakescrn(mainrsrc,TRUE,doexit,NULL,TRUE);
}
