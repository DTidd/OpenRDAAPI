/* dfscndef.c - Define Screen Defaults */
#ifndef WIN32
#define __NAM__ "dfscndef.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dfscndef.exe"
#endif
#include <app.hpp>





#include <mix.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

static APPlib *deflt_bins=NULL,*defscn_defs=NULL,*dir_libs=NULL;
static APPlib *screendef=NULL,*ftypes=NULL;
static RDAdefault *defdata=NULL;
static void makespacescreendef(int),deletedfscreendef(RDArsrc *);
static void quitdfscreendef(RDArsrc *,RDArsrc *),okdfscreendef(RDArsrc *,RDArsrc *);
static void makemtnscreen(RDArsrc *,RDArsrc *,char *,char *);
static void newdfscreendef(RDArsrc *,RDArsrc *),makescreendeflist(void);
static void getscndefentries(RDArsrc *),getdirscndef(RDArsrc *);
static void savescreendef(RDArsrc *,RDArsrc *),quitscreendef(RDArsrc *,RDArsrc *);
static void edit_resource(RDArsrc *),delete_rsc(RDArsrc *);
static void add_rsc_above(RDArsrc *),add_rsc_below(RDArsrc *);
static void saveresource(RDArsrc *,RDArsrc *);
static void save_resource_below(RDArsrc *,RDArsrc *);
static void save_resource_above(RDArsrc *,RDArsrc *);
static void screendefscreen(RDArsrc *,DEFfield *,void (*)(...),void (*)(...));
static void deletetest(RDArsrc *);
static void printdefaultcb(RDArsrc *);
static void printdefault(RDArsrc *);
static void quit_printdefault(RDArsrc *);
static RDAdefault *mtndefaults=NULL,*editdefaults=NULL;
static char changedvalues=FALSE;

static void printdefault(RDArsrc *prsrc)
{
	RDA_PFILE *fp=NULL;
	int x,pages=0;
	DEFfield *field=NULL;
	char *outdevice=NULL,*temp=NULL,*message=NULL;
	char *date=NULL,*timex=NULL,print_style=TRUE;

	readwidget(prsrc,"DEVICE");
	FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
	readwidget(prsrc,"NORMAL PRINT");
	FINDRSCGETCHAR(prsrc,"NORMAL PRINT",&print_style);
	if(outdevice==NULL)
	{
		ERRORDIALOG("Output Device Failure","There was no output device specified.",NULL,FALSE);
		return;
	}
	fp=RDA_popen(outdevice,"w");
	if(fp!=NULL)
	{
		SelectPrintType(outdevice,print_style,fp);
		RDA_fprintf(fp,"%*sXPERT SCREEN DEFAULTS DEFINED            Page: %*d\r\n",25,"",8,++pages);
		temp=stralloc(defdata->module);
		temp[RDAstrlen(temp)-4]=0;
		date=GETCURRENTDATE();
		timex=GETCURRENTTIME();
		RDA_fprintf(fp,"%*sModule [%s]  Screen [%s]%*sDate: %s\r\n",
			((80-(21+RDAstrlen(temp)-4+RDAstrlen(defdata->name)))/2),"",
			temp,defdata->name,
			(((80-(21+RDAstrlen(temp)-4+RDAstrlen(defdata->name)))/2)-13),"",
			date);
		RDA_fprintf(fp,"%*s Executed By: %s%*sTime: %s\r\n\r\n",
			((80-(15+RDAstrlen(USERLOGIN)))/2),"",
			USERLOGIN,
			(((80-(15+RDAstrlen(USERLOGIN)))/2)-13),"",
			timex);
		if(date!=NULL) Rfree(date);
		if(timex!=NULL) Rfree(timex);
		if(defdata->input_focus==NULL)
		{
			RDA_fprintf(fp,"Input Focus:  None Saved.\r\n");
		} else {
			RDA_fprintf(fp,"Input Focus:\r\n");
			RDA_fprintf(fp,"   %s\r\n",defdata->input_focus);
		}	
		if(defdata->field==NULL || defdata->num<1)
		{
			RDA_fprintf(fp,"Screen Resources Defaults:  None Saved.\r\n");
		} else {
			RDA_fprintf(fp,"Screen Resources Defaults:\r\n");
			for(x=0,field=defdata->field;x<defdata->num;++x,++field)
			{
				RDA_fprintf(fp,"  Number: [%d]  Resource Name: [%s]\r\n",x,field->name);
				RDA_fprintf(fp,"    Type: [%d] [%s] Reset Status [%s]\r\n",field->type,standardfieldtypes[field->type],(field->reset ? "True":"False"));
				switch(field->type)
				{
					case SCROLLEDTEXT:
					case VARIABLETEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PLAINTEXT:
					case ZIPCODE:
					case PHONE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case CUSTOMTYPE:
					case OPTIONALFIELDS:
						RDA_fprintf(fp,"       Value [%s]",(field->data.string_value!=NULL ? field->data.string_value:"NULL"));
						break;
					case BOOLNS:
						RDA_fprintf(fp,"       Value [%d] [%s]",*field->data.string_value,(*field->data.string_value ? "Yes":"No"));
						break;
					case CHARACTERS:
						RDA_fprintf(fp,"       Value [%c]",*field->data.string_value);
						break;
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case SDECIMALV:
						RDA_fprintf(fp,"       Value [%f]",*field->data.float_value);
						break;
					case SHORTV:
					case SSHORTV:
						RDA_fprintf(fp,"       Value [%d]",*field->data.short_value);
						break;
					case LONGV:
					case SLONGV:
					case SCROLLEDLIST:
						RDA_fprintf(fp,"       Value [%d]",*field->data.integer_value);
						break;
					case BUTTONS:
						break;
					default:
						prterr("Field type [%d] not found for field [%s] in PRINT DEFAULTS.",field->type,(field->name==NULL?"":field->name));
						break;
				}
			}
			RDA_fprintf(fp,"\r\n");
		}
		RDA_fprintf(fp,"END RDAdefault [%s] [%s]\r\n",temp,defdata->name);
		if(temp!=NULL) Rfree(temp);
		RDA_pclose(fp);
		if(outdevice!=NULL) Rfree(outdevice);
	} else {
		message=Rmalloc(RDAstrlen(outdevice)+69);
		sprintf(message,"The output attempt to the device [%s] failed when trying to open pipe.",outdevice);
		ERRORDIALOG("DEVICE COMMUNICATIONS FAILED!",message,NULL,FALSE);
		prterr(message);
		if(message!=NULL) Rfree(message);
	}
}
static void quit_printdefault(RDArsrc *prsrc)
{
	if(prsrc!=NULL) free_rsrc(prsrc);
}
static void printdefaultcb(RDArsrc *mtnrsrc)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL,normal_print=TRUE;

	prsrc=RDArsrcNEW("GUI","PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addbtnrsrc(prsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	addstdrsrc(prsrc,"NORMAL PRINT",BOOLNS,1,&normal_print,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,printdefault,NULL);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_printdefault,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PRINT SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		quit_printdefault(prsrc);
	}
}
void makespacescreendef(int pos)
{
	short x,y;
	DEFfield *temp,*temp1,*new_df;

	++defdata->num;
	new_df=Rmalloc(defdata->num*sizeof(DEFfield));
	y=0;
	for(x=0,temp=defdata->field;x<(defdata->num-1);++x,++temp)
	{
		temp1=new_df+y;
		if(x==pos)
		{
			temp1->type=0;
			temp1->name=NULL;
			temp1->reset=FALSE;
			++y;
			temp1=new_df+y;
			if(temp1->type==0)
				temp1->data.string_value=NULL;
		}
		temp1->type=temp->type;
		temp1->reset=temp->reset;
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
			temp->name=NULL;
		} else temp1->name=NULL;
		switch(temp1->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case PLAINTEXT:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case ZIPCODE:
			case PHONE:
			case SOCSECNUM:
			case DATES:
			case TIMES:
			case CUSTOMTYPE:
			case OPTIONALFIELDS:
				if(temp->data.string_value!=NULL)
				{
					temp1->data.string_value=stralloc(temp->data.string_value);
					Rfree(temp->data.string_value);
					temp->data.string_value=NULL;
				} else temp1->data.string_value=NULL;
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case SDECIMALV:
			case SDOUBLEV:
			case DOUBLEV:
				if(temp->data.float_value!=NULL)
				{
					temp1->data.float_value=Rmalloc(sizeof(double));
					*temp1->data.float_value=*temp->data.float_value;
					Rfree(temp->data.float_value);
					temp->data.float_value=NULL;
				} else temp1->data.float_value=NULL;
				break;	
			case LONGV:
			case SLONGV:
			case SCROLLEDLIST:
				if(temp->data.integer_value!=NULL)
				{
					temp1->data.integer_value=Rmalloc(sizeof(int));
					*temp1->data.integer_value=*temp->data.integer_value;
					Rfree(temp->data.integer_value);
					temp->data.integer_value=NULL;
				} else temp1->data.integer_value=NULL;
				break;
			case SSHORTV:
			case SHORTV:
				if(temp->data.short_value!=NULL)
				{
					temp1->data.short_value=Rmalloc(sizeof(short));
					*temp1->data.short_value=*temp->data.short_value;
					Rfree(temp->data.short_value);
					temp->data.short_value=NULL;
				} else temp1->data.short_value=NULL;
				break;
			case BOOLNS:
			case CHARACTERS:
				if(temp->data.string_value!=NULL)
				{
					temp1->data.string_value=Rmalloc(1);
					*temp1->data.string_value=*temp->data.string_value;
					Rfree(temp->data.string_value);
					temp->data.string_value=NULL;
				} else temp1->data.string_value=NULL;
				break;
			case BUTTONS:
				break;
			default:
				prterr("Error Unrecognized field type.");
				break;
		}
		++y;
	}
	Rfree(defdata->field);
	defdata->field=Rmalloc(defdata->num*sizeof(DEFfield));
	for(x=0,temp1=defdata->field,temp=new_df;x<defdata->num;++x,
		++temp,++temp1)
	{
		temp1->type=temp->type;
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
			temp->name=NULL;
		} else temp1->name=NULL;
		temp1->reset=temp->reset;
		switch(temp1->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case PLAINTEXT:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case ZIPCODE:
			case PHONE:
			case SOCSECNUM:
			case DATES:
			case TIMES:
			case CUSTOMTYPE:
			case OPTIONALFIELDS:
				if(temp->data.string_value!=NULL)
				{
					temp1->data.string_value=stralloc(temp->data.string_value);
					Rfree(temp->data.string_value);
					temp->data.string_value=NULL;
				} else temp1->data.string_value=NULL;
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case SDECIMALV:
			case SDOUBLEV:
			case DOUBLEV:
				if(temp->data.float_value!=NULL)
				{
					temp1->data.float_value=Rmalloc(sizeof(double));
					*temp1->data.float_value=*temp->data.float_value;
					Rfree(temp->data.float_value);
					temp->data.float_value=NULL;
				} else temp1->data.float_value=NULL;
				break;	
			case LONGV:
			case SLONGV:
			case SCROLLEDLIST:
				if(temp->data.integer_value!=NULL)
				{
					temp1->data.integer_value=Rmalloc(sizeof(int));
					*temp1->data.integer_value=*temp->data.integer_value;
					Rfree(temp->data.integer_value);
					temp->data.integer_value=NULL;
				} else temp1->data.integer_value=NULL;
				break;
			case SSHORTV:
			case SHORTV:
				if(temp->data.short_value!=NULL)
				{
					temp1->data.short_value=Rmalloc(sizeof(short));
					*temp1->data.short_value=*temp->data.short_value;
					Rfree(temp->data.short_value);
					temp->data.short_value=NULL;
				} else temp1->data.short_value=NULL;
				break;
			case BOOLNS:
			case CHARACTERS:
				if(temp->data.string_value!=NULL)
				{
					temp1->data.string_value=Rmalloc(1);
					*temp1->data.string_value=*temp->data.string_value;
					Rfree(temp->data.string_value);
					temp->data.string_value=NULL;
				} else temp1->data.string_value=NULL;
				break;
			case BUTTONS:
				break;
			default:
				prterr("Error Unrecognized field type.");
				break;
		}
	}
	if(new_df!=NULL) { Rfree(new_df); new_df=NULL; }
}
void deletedfscreendef(RDArsrc *mainrsrc)
{
	int l,s;
	char *libx=NULL,*directory=NULL;

	readwidget(mainrsrc,"SCREEN DEFAULT LIBRARIES");
	if(FINDRSCGETINT(mainrsrc,"SCREEN DEFAULT LIBRARIES",&l)) return;
	readwidget(mainrsrc,"DIRECTORY");
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	if(FINDRSCGETINT(mainrsrc,"SCREEN DEFAULT LIST",&s)) return;
	libx=Rmalloc(RDAstrlen(directory)+RDAstrlen(deflt_bins->libs[l])+6);
#ifndef WIN32
	sprintf(libx,"%s/%s.DEF",directory,deflt_bins->libs[l]);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\%s.DEF",directory,deflt_bins->libs[l]);
#endif
	deletelibbin(libx,defscn_defs->libs[s]);
	if(defscn_defs!=NULL) { freeapplib(defscn_defs); defscn_defs=NULL; }
	defscn_defs=getlibnames(libx,FALSE);
	if(defscn_defs==NULL)
	{
		defscn_defs=APPlibNEW();
		addAPPlib(defscn_defs,"Contains No Screen Defaults Definitions");
	}
	if(libx!=NULL) Rfree(libx);
	if(directory!=NULL) Rfree(directory);
	if(s>=defscn_defs->numlibs) s=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"SCREEN DEFAULT LIST",s,defscn_defs))
		updatersrc(mainrsrc,"SCREEN DEFAULT LIST");
}
void deletetest(RDArsrc *mainrsrc)
{
	int s;

	readwidget(mainrsrc,"SCREEN DEFAULT LIST");
	if(FINDRSCGETINT(mainrsrc,"SCREEN DEFAULT LIST",&s)) return;
	if(!RDAstrcmp(defscn_defs->libs[s],"Contains No Screen Defautl Definitions")) return;
	WARNINGDIALOG("DELETE WARNING DIALOG SCREEN","DELETE THIS SCREEN DEFAULT?","Are you sure that you want to DELETE this SCREEN DEFAULT DEFINITION?",deletedfscreendef,NULL,FALSE,mainrsrc);
}
void quitdfscreendef(RDArsrc *mainrsrc,RDArsrc *mtnrsrc)
{
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	if(mtnrsrc!=NULL) free_rsrc(mtnrsrc);
	if(screendef!=NULL) freeapplib(screendef);
	if(deflt_bins!=NULL) freeapplib(deflt_bins);
	if(defscn_defs!=NULL) freeapplib(defscn_defs);
	if(dir_libs!=NULL) freeapplib(dir_libs);
	ShutdownSubsystems();
}
void makemtnscreen(RDArsrc *mtnrsrc,RDArsrc *mainrsrc,char *libname,char *sname)
{
	makescreendeflist();
	FINDRSCSETSTRING(mtnrsrc,"SCREEN DEFAULT LIBRARY",(libname!=NULL ? libname:""));
	FINDRSCSETSTRING(mtnrsrc,"SCREEN NAME",(sname!=NULL ? sname:""));
	FINDRSCSETSTRING(mtnrsrc,"INPUT FOCUS",(defdata->input_focus!=NULL ? defdata->input_focus:""));
	FINDRSCLISTAPPlib(mtnrsrc,"RESOURCE LIST",0,screendef);
	changedvalues=FALSE;
	mtndefaults=GetDefaults(mtnrsrc);
	APPmakescrn(mtnrsrc,TRUE,quitscreendef,mainrsrc,FALSE);
}
void okdfscreendef(RDArsrc *mainrsrc,RDArsrc *mtnrsrc)
{
	int s=0,h=0;
	char *dirx=NULL,*dira=NULL,*deff=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirx);
	FINDRSCGETINT(mainrsrc,"SCREEN DEFAULT LIBRARIES",&s); 
	FINDRSCGETINT(mainrsrc,"SCREEN DEFAULT LIST",&h);
	dira=Rmalloc(RDAstrlen(dirx)+RDAstrlen(deflt_bins->libs[s])+6);
#ifndef WIN32
	sprintf(dira,"%s/%s.DEF",dirx,deflt_bins->libs[s]);
#endif
#ifdef WIN32
	sprintf(dira,"%s\\%s.DEF",dirx,deflt_bins->libs[s]);
#endif
	if(!RDAstrcmp(defscn_defs->libs[h],"Contains No Screen Default Definitions"))
	{
		deff=NULL;
	} else deff=stralloc(defscn_defs->libs[h]);
	if(defdata!=NULL) FreeRDAdefault(defdata);
	defdata=RDAdefaultNEW(dira,deff);
	if(getdefaultbin(dira,deff,defdata,FALSE)==0)
	{
		dira[RDAstrlen(dira)-4]=0;
		makemtnscreen(mtnrsrc,mainrsrc,dira,deff);
	} 
	if(deff!=NULL) Rfree(deff);
	if(dirx!=NULL) Rfree(dirx);
	if(dira!=NULL) Rfree(dira);
}
void newdfscreendef(RDArsrc *mainrsrc,RDArsrc *mtnrsrc)
{
	int s=0;
	char *dirx=NULL,*dira=NULL,*deflib=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirx);
	FINDRSCGETINT(mainrsrc,"SCREEN DEFAULT LIBRARIES",&s); 
	if(RDAstrcmp(deflt_bins->libs[s],"Directory Contains No Libraries"))
	{
		deflib=stralloc(deflt_bins->libs[s]);
	} else deflib=stralloc(" ");
	dira=Rmalloc(RDAstrlen(dirx)+RDAstrlen(deflib)+2);
#ifndef WIN32
	sprintf(dira,"%s/%s",dirx,deflib);
#endif
#ifdef WIN32
	sprintf(dira,"%s\\%s",dirx,deflib);
#endif
	if(defdata!=NULL) FreeRDAdefault(defdata);
	defdata=RDAdefaultNEW(dira,NULL);
	if(defdata->field==NULL) defdata->field=Rmalloc(sizeof(DEFfield));
	else defdata->field=Rrealloc(defdata->field,sizeof(DEFfield));
	makemtnscreen(mtnrsrc,mainrsrc,dira,NULL);
	if(deflib!=NULL) Rfree(deflib);
	if(dirx!=NULL) Rfree(dirx);
	if(dira!=NULL) Rfree(dira);
}
void makescreendeflist(void)
{
	short x;
	char *s=NULL,*tmp=NULL;
	DEFfield *h;

	if(screendef!=NULL) { freeapplib(screendef); screendef=NULL; }
	screendef=APPlibNEW();
	if(defdata->num<1 || defdata->field==NULL)
	{
		if(screendef->numlibs<1)
		{
			addAPPlib(screendef,"No Resources Defaults Defined");
		}
	} else if(defdata->num==1 && defdata->field->name==NULL)
	{
		if(screendef->numlibs<1)
		{
			addAPPlib(screendef,"No Resources Defaults Defined");
		}
	} else {
		for(x=0,h=defdata->field;x<defdata->num;++x,++h)
		{
			if(tmp!=NULL) Rfree(tmp)
			switch(h->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case PLAINTEXT:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case ZIPCODE:
				case PHONE:
				case SOCSECNUM:
				case DATES:
				case TIMES:
				case CUSTOMTYPE:
				case OPTIONALFIELDS:
					tmp=stralloc(h->data.string_value);
					break;
				case DOLLARS:
					sprintf(stemp,"$ %.2f",(*h->data.float_value/100));
					tmp=stralloc(stemp);
					break;	
				case DOLLARS_NOCENTS:
					sprintf(stemp,"$ %.0f",*h->data.float_value);
					tmp=stralloc(stemp);
					break;	
				case DECIMALV:
				case SDECIMALV:
				case SDOUBLEV:
				case DOUBLEV:
					sprintf(stemp,"%f",*h->data.float_value);
					tmp=stralloc(stemp);
					break;	
				case LONGV:
				case SLONGV:
				case SCROLLEDLIST:
					sprintf(stemp,"%d",*h->data.integer_value);
					tmp=stralloc(stemp);
					break;
				case SSHORTV:
				case SHORTV:
					sprintf(stemp,"%d",*h->data.short_value);
					tmp=stralloc(stemp);
					break;
				case BOOLNS:
					sprintf(stemp,"%s",(*h->data.string_value==TRUE?"TRUE":"FALSE"));
					tmp=stralloc(stemp);
					break;
				case CHARACTERS:
					sprintf(stemp,"%c",*h->data.string_value);
					tmp=stralloc(stemp);
					break;
				case BUTTONS:
				default:
					tmp=NULL;
					prterr("Invalid Field Type for Screen Defaults.  Field Name=[%s], Field Type=[%d].",(h->name==NULL?"":h->name),h->type);
					break;
			}
			s=Rmalloc(6+RDAstrlen(h->name)+RDAstrlen(fieldtypedesc[h->type])+(tmp==NULL?RDAstrlen("NULL"):RDAstrlen(tmp))+60+1);
			sprintf(s,"[%5d] Resource: [%s] Type: [%s] Reset Status: [%s] Value: [%s]",
				(x+1),
				(h->name==NULL?"NULL":h->name),
				fieldtypedesc[h->type],
				(h->reset ? "True":"False"),
				(tmp==NULL?"":tmp));
			addAPPlib(screendef,s);
			if(s!=NULL) { Rfree(s); s=NULL; }
			if(tmp!=NULL) { Rfree(tmp); tmp=NULL; }
		}
	}
}
void quitresource(RDArsrc *editrsrc,RDArsrc *mtnrsrc)
{
	if(editrsrc!=NULL)
	{
		killwindow(editrsrc);
		free_rsrc(editrsrc);
	}
	if(ftypes!=NULL) freeapplib(ftypes);
	if(editdefaults!=NULL) FreeRDAdefault(editdefaults);
}
void quitresourcetest(RDArsrc *editrsrc,RDArsrc *mtnrsrc)
{
	readallwidgets(editrsrc);
	if(COMPARE_RSRCS(editrsrc,editdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this Resource Default Definition.\nExiting now without saving will loose these changes.",saveresource,quitresource,FALSE,2,editrsrc,mtnrsrc,NULL);
	} else {
		quitresource(editrsrc,mtnrsrc);
	}
}
void screendefscreen(RDArsrc *mtnrsrc,DEFfield *field,
	void (*save_function)(...),void (*quit_functiontest)(...))
{
	char *hlib=NULL;
	char *sname=NULL,reset_value=TRUE;
	char *tmp=NULL;
	int x=0,fieldtype=0;
	RDArsrc *editrsrc=NULL;

	readwidget(mtnrsrc,"SCREEN DEFAULT LIBRARY");
	readwidget(mtnrsrc,"SCREEN NAME");
	editrsrc=RDArsrcNEW("GUI","MAINTAIN RESOURCE DEFAULTS");
	addstdrsrc(editrsrc,"SCREEN DEFAULT LIBRARY",VARIABLETEXT,80,NULL,FALSE);
	addstdrsrc(editrsrc,"SCREEN NAME",VARIABLETEXT,80,NULL,FALSE);
	addstdrsrc(editrsrc,"RESOURCE NAME",VARIABLETEXT,80,NULL,TRUE);
	if(ftypes!=NULL) freeapplib(ftypes);
	ftypes=APPlibNEW();
	for(x=0;x<28;++x)
	{
		addAPPlib(ftypes,standardfieldtypes[x]);
	}
	if(field!=NULL)
	{
		fieldtype=(int)field->type;
		reset_value=(char)field->reset;
	}
	addlstrsrc(editrsrc,"RESOURCE TYPES",&fieldtype,TRUE,NULL,ftypes->numlibs,&ftypes->libs,NULL);
	addstdrsrc(editrsrc,"RESET VALUE",BOOLNS,1,&reset_value,TRUE);
	addsctrsrc(editrsrc,"RESOURCE VALUE",0,NULL,TRUE);
	addrfcbrsrc(editrsrc,"SAVE",TRUE,save_function,mtnrsrc);
	addrfcbrsrc(editrsrc,"QUIT",TRUE,quit_functiontest,mtnrsrc);
	addbtnrsrc(editrsrc,"HELP",TRUE,screenhelp,editrsrc);
	addbtnrsrc(editrsrc,"PRINT RESOURCES",TRUE,printrsrcs,editrsrc);
	if(!FINDRSCGETSTRING(mtnrsrc,"SCREEN DEFAULT LIBRARY",&hlib))
	{
		FINDRSCSETSTRING(editrsrc,"SCREEN DEFAULT LIBRARY",hlib);
	}
	if(hlib!=NULL) Rfree(hlib);
	if(!FINDRSCGETSTRING(mtnrsrc,"SCREEN NAME",&sname))
	{
		FINDRSCSETSTRING(editrsrc,"SCREEN NAME",sname);
	}
	if(sname!=NULL) Rfree(sname);
	if(field!=NULL)
	{
		FINDRSCSETSTRING(editrsrc,"RESOURCE NAME",field->name);
		switch(field->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case PLAINTEXT:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case ZIPCODE:
			case PHONE:
			case SOCSECNUM:
			case DATES:
			case TIMES:
			case CUSTOMTYPE:
			case OPTIONALFIELDS:
				tmp=field->data.string_value;
				break;
			case DOLLARS_NOCENTS:
				sprintf(stemp,"$ %.0f",*field->data.float_value);
				tmp=stralloc(stemp);
				break;
			case DOLLARS:
				sprintf(stemp,"$ %.2f",(*field->data.float_value/100));
				tmp=stralloc(stemp);
				break;	
			case DECIMALV:
			case SDECIMALV:
			case SDOUBLEV:
			case DOUBLEV:
				sprintf(stemp,"%f",*field->data.float_value);
				tmp=stralloc(stemp);
				break;	
			case LONGV:
			case SLONGV:
			case SCROLLEDLIST:
				sprintf(stemp,"%d",*field->data.integer_value);
				tmp=stralloc(stemp);
				break;
			case SSHORTV:
			case SHORTV:
				sprintf(stemp,"%d",*field->data.short_value);
				tmp=stralloc(stemp);
				break;
			case BOOLNS:
				sprintf(stemp,"%s",(*field->data.string_value==TRUE?"TRUE":"FALSE"));
				tmp=stralloc(stemp);
				break;
			case CHARACTERS:
				sprintf(stemp,"%c",*field->data.string_value);
				tmp=stralloc(stemp);
				break;
			case BUTTONS:
				tmp=NULL;
				break;
			default:
				prterr("Error Unrecognized field type.");
				break;
		}
		FINDRSCSETSTRING(editrsrc,"RESOURCE VALUE",(tmp!=NULL ? tmp : ""));
	}
	editdefaults=GetDefaults(editrsrc);
	APPmakescrn(editrsrc,TRUE,quitresource,mtnrsrc,FALSE);
}
void saveresource(RDArsrc *editrsrc,RDArsrc *mtnrsrc)
{
	int l=0,x=0,y=0,integer_value=0;
	double float_value=0.0;
	DEFfield *fds=NULL;
	char *test=NULL,*rsrcvalue=NULL,*tmp=NULL,char_value,reset_value=FALSE;

	readallwidgets(editrsrc);
	if(FINDRSCGETINT(mtnrsrc,"RESOURCE LIST",&l)) return;
	fds=defdata->field+l;
	if(!FINDRSCGETSTRING(editrsrc,"RESOURCE NAME",&test))
	{
		if(test!=NULL && RDAstrlen(test)>0)
		{
			fds->name=stralloc(test);
			Rfree(test);
			test=NULL;
		}
	} else return;	
	if(FINDRSCGETSTRING(editrsrc,"RESOURCE VALUE",&rsrcvalue)) return;
	FINDRSCGETCHAR(editrsrc,"RESET VALUE",&reset_value);
	if(!FINDRSCGETINT(editrsrc,"RESOURCE TYPES",&x))
	{
		fds->type=x;
		fds->reset=reset_value;
		switch(x)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case PLAINTEXT:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case ZIPCODE:
			case PHONE:
			case SOCSECNUM:
			case DATES:
			case TIMES:
			case CUSTOMTYPE:
			case OPTIONALFIELDS:
				if(fds->data.string_value!=NULL)
				{
					Rfree(fds->data.string_value);
					fds->data.string_value=NULL;
				}
				fds->data.string_value=stralloc(rsrcvalue);
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case DOUBLEV:
			case SDECIMALV:
			case SDOUBLEV:
				if(fds->data.float_value==NULL)
				{
					fds->data.float_value=Rmalloc(sizeof(double));
				}
				for(y=0;y<RDAstrlen(rsrcvalue);++y)
				{
					if(rsrcvalue[y]=='$') rsrcvalue[y]=' ';
				}
				float_value=(atof(rsrcvalue)*100);
				*fds->data.float_value=float_value;
				break;	
			case SHORTV:
			case SSHORTV:
				if(fds->data.short_value==NULL)
				{
					fds->data.short_value=Rmalloc(sizeof(short));
				}
				integer_value=Ratoi(rsrcvalue);
				*fds->data.short_value=(short)integer_value;
				break;
			case LONGV:
			case SLONGV:
			case SCROLLEDLIST:
				if(fds->data.integer_value==NULL)
				{
					fds->data.integer_value=Rmalloc(sizeof(int));
				}
				integer_value=Ratoi(rsrcvalue);
				*fds->data.integer_value=integer_value;
				break;
			case BOOLNS:
				if(fds->data.string_value==NULL)
				{
					fds->data.string_value=Rmalloc(1);
				}
				integer_value=Ratoi(rsrcvalue);
				stoupper(rsrcvalue);
				tmp=stralloc(rsrcvalue);
				if(!strncmp(tmp,"TRUE",4)) 
					*fds->data.string_value=TRUE;
				else if(!strncmp(tmp,"FALSE",5)) 
					*fds->data.string_value=FALSE;
				else if(integer_value==TRUE)
					*fds->data.string_value=TRUE;
				else if(integer_value==FALSE)
					*fds->data.string_value=FALSE;
				break;
			case CHARACTERS:
				if(fds->data.string_value==NULL)
				{
					fds->data.string_value=Rmalloc(1);
				}
				char_value=rsrcvalue[0];
				*fds->data.string_value=char_value;
				break;
			case BUTTONS:
			default:
				break;
		}
	}
	if(tmp!=NULL) Rfree(tmp);
	makescreendeflist();
	if(!FINDRSCLISTAPPlib(mtnrsrc,"RESOURCE LIST",l,screendef))
		updatersrc(mtnrsrc,"RESOURCE LIST");
	if(COMPARE_RSRCS(editrsrc,editdefaults,2))
	{
		changedvalues=TRUE;
	}
	quitresource(editrsrc,mtnrsrc);
}
void save_resource_above(RDArsrc *editrsrc,RDArsrc *mtnrsrc)
{
	int x;

	if(FINDRSCGETINT(mtnrsrc,"RESOURCE LIST",&x)) return;
	makespacescreendef(x);
	saveresource(editrsrc,mtnrsrc);
}
void save_resource_below(RDArsrc *editrsrc,RDArsrc *mtnrsrc)
{
	int x;

	if(FINDRSCGETINT(mtnrsrc,"RESOURCE LIST",&x)) return;
	makespacescreendef(++x);
	if(!FINDRSCSETINT(mtnrsrc,"RESOURCE LIST",x))
		updatersrc(mtnrsrc,"RESOURCE LIST");
	saveresource(editrsrc,mtnrsrc);
}
void quit_resource_abovetest(RDArsrc *editrsrc,RDArsrc *mtnrsrc)
{
	readallwidgets(editrsrc);
	if(COMPARE_RSRCS(editrsrc,editdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this Resource Default Definition.\nExiting now without saving will loose these changes.",save_resource_above,quitresource,FALSE,2,editrsrc,mtnrsrc,NULL);
	} else {
		quitresource(editrsrc,mtnrsrc);
	}
}
void quit_resource_belowtest(RDArsrc *editrsrc,RDArsrc *mtnrsrc)
{
	readallwidgets(editrsrc);
	if(COMPARE_RSRCS(editrsrc,editdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this Resource Default Definition.\nExiting now without saving will loose these changes.",save_resource_below,quitresource,FALSE,2,editrsrc,mtnrsrc,NULL);
	} else {
		quitresource(editrsrc,mtnrsrc);
	}
}
void add_rsc_above(RDArsrc *mtnrsrc)
{
	screendefscreen(mtnrsrc,NULL,save_resource_above,quit_resource_abovetest);
}
void add_rsc_below(RDArsrc *mtnrsrc)
{
	screendefscreen(mtnrsrc,NULL,save_resource_below,quit_resource_belowtest);
}
void delete_rsc(RDArsrc *mtnrsrc)
{
	DEFfield *temp=NULL,*temp1=NULL,*new_screendefs=NULL;
	int x,y,value;

	readallwidgets(mtnrsrc);
	if(FINDRSCGETINT(mtnrsrc,"RESOURCE LIST",&value)) return;
	if(screendef->numlibs<2 || !RDAstrcmp(screendef->libs[0],"No Resources Defaults Defined")) return;
	new_screendefs=Rmalloc((defdata->num)*sizeof(DEFfield));
	y=0;
	for(x=0,temp=defdata->field;x<defdata->num;++x,++temp)
	{
		temp1=new_screendefs+y;
		if(x==value)
		{
			temp1->type=0;
			temp1->reset=FALSE;
			if(temp->name!=NULL)
			{
				Rfree(temp->name);
				temp->name=NULL;
			}
			switch(temp1->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case PLAINTEXT:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case ZIPCODE:
				case PHONE:
				case SOCSECNUM:
				case DATES:
				case TIMES:
				case CUSTOMTYPE:
				case OPTIONALFIELDS:
					if(temp->data.string_value!=NULL)
					{
						Rfree(temp->data.string_value);
						temp->data.string_value=NULL;
					}
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case SDECIMALV:
				case SDOUBLEV:
				case DOUBLEV:
					if(temp->data.float_value!=NULL)
					{
						Rfree(temp->data.float_value);
						temp->data.float_value=NULL;
					}
					break;	
				case LONGV:
				case SLONGV:
				case SCROLLEDLIST:
					if(temp->data.integer_value!=NULL)
					{
						Rfree(temp->data.integer_value);
						temp->data.integer_value=NULL;
					}
					break;
				case SSHORTV:
				case SHORTV:
					if(temp->data.short_value!=NULL)
					{
						Rfree(temp->data.short_value);
						temp->data.short_value=NULL;
					}
					break;
				case BOOLNS:
				case CHARACTERS:
					if(temp->data.string_value!=NULL)
					{
						Rfree(temp->data.string_value);
						temp1->data.string_value=NULL;
					}
					break;
				case BUTTONS:
					break;
				default:
					prterr("Error Unrecognized field type.");
					break;
			}
		} else {
			temp1->type=temp->type;
			temp1->reset=temp->reset;
			if(temp->name!=NULL)
			{
				temp1->name=stralloc(temp->name);
				Rfree(temp->name);
			} else temp1->name=NULL;
			switch(temp1->type)
			{
				case SCROLLEDTEXT:
				case VARIABLETEXT:
				case PLAINTEXT:
				case EXPENDITURE:
				case REVENUE:
				case BALANCESHEET:
				case BEGINNINGBALANCE:
				case ZIPCODE:
				case PHONE:
				case SOCSECNUM:
				case DATES:
				case TIMES:
				case CUSTOMTYPE:
				case OPTIONALFIELDS:
					if(temp->data.string_value!=NULL)
					{
						temp1->data.string_value=stralloc(temp->data.string_value);
						temp->data.string_value=NULL;
					} else temp1->data.string_value=NULL;
					break;
				case DOLLARS:
				case DOLLARS_NOCENTS:
				case DECIMALV:
				case SDECIMALV:
				case SDOUBLEV:
				case DOUBLEV:
					if(temp->data.float_value!=NULL)
					{
						temp1->data.float_value=Rmalloc(sizeof(double));
						*temp1->data.float_value=*temp->data.float_value;
						Rfree(temp->data.float_value);
						temp->data.float_value=NULL;
					} else temp1->data.float_value=NULL;
					break;	
				case LONGV:
				case SLONGV:
				case SCROLLEDLIST:
					if(temp->data.integer_value!=NULL)
					{
						temp1->data.integer_value=Rmalloc(sizeof(int));
						*temp1->data.integer_value=*temp->data.integer_value;
						Rfree(temp->data.integer_value);
					} else temp1->data.integer_value=NULL;
					break;
				case SSHORTV:
				case SHORTV:
					if(temp->data.short_value!=NULL)
					{
						temp1->data.short_value=Rmalloc(sizeof(short));
						*temp1->data.short_value=*temp->data.short_value;
						Rfree(temp->data.short_value);
						temp->data.short_value=NULL;
					} else temp1->data.short_value=NULL;
					break;
				case BOOLNS:
				case CHARACTERS:
					if(temp->data.string_value!=NULL)
					{
						temp1->data.string_value=Rmalloc(1);
						*temp1->data.string_value=*temp->data.string_value;
						Rfree(temp->data.string_value);
						temp->data.string_value=NULL;
					} else temp1->data.string_value=NULL;
					break;
				case BUTTONS:
					break;
				default:
					prterr("Error Unrecognized field type.");
					break;
			}
			++y;
		}
	}
	Rfree(defdata->field);
	--defdata->num;
	defdata->field=Rmalloc(defdata->num*sizeof(DEFfield));
	for(x=0,temp1=defdata->field,temp=new_screendefs;x<defdata->num;++x,
		++temp,++temp1)
	{
		temp1->type=temp->type;
		temp1->reset=temp->reset;
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		switch(temp1->type)
		{
			case SCROLLEDTEXT:
			case VARIABLETEXT:
			case PLAINTEXT:
			case EXPENDITURE:
			case REVENUE:
			case BALANCESHEET:
			case BEGINNINGBALANCE:
			case ZIPCODE:
			case PHONE:
			case SOCSECNUM:
			case DATES:
			case TIMES:
			case CUSTOMTYPE:
			case OPTIONALFIELDS:
				if(temp->data.string_value!=NULL)
				{
					temp1->data.string_value=stralloc(temp->data.string_value);
					Rfree(temp->data.string_value);
					temp->data.string_value=NULL;
				} else temp1->data.string_value=NULL;
				break;
			case DOLLARS:
			case DOLLARS_NOCENTS:
			case DECIMALV:
			case SDECIMALV:
			case SDOUBLEV:
			case DOUBLEV:
				if(temp->data.float_value!=NULL)
				{
					temp1->data.float_value=Rmalloc(sizeof(double));
					*temp1->data.float_value=*temp->data.float_value;
					Rfree(temp->data.float_value);
					temp->data.float_value=NULL;
				} else temp1->data.float_value=NULL;
				break;	
			case LONGV:
			case SLONGV:
			case SCROLLEDLIST:
				if(temp->data.integer_value!=NULL)
				{
					temp1->data.integer_value=Rmalloc(sizeof(int));
					*temp1->data.integer_value=*temp->data.integer_value;
					Rfree(temp->data.integer_value);
					temp->data.integer_value=NULL;
				} else temp1->data.integer_value=NULL;
				break;
			case SSHORTV:
			case SHORTV:
				if(temp->data.short_value!=NULL)
				{
					temp1->data.short_value=Rmalloc(sizeof(short));
					*temp1->data.short_value=*temp->data.short_value;
					Rfree(temp->data.short_value);
					temp->data.short_value=NULL;
				} else temp1->data.short_value=NULL;
				break;
			case BOOLNS:
			case CHARACTERS:
				if(temp->data.string_value!=NULL)
				{
					temp1->data.string_value=Rmalloc(1);
					*temp1->data.string_value=*temp->data.string_value;
					Rfree(temp->data.string_value);
					temp->data.string_value=NULL;
				} else temp1->data.string_value=NULL;
				break;
			case BUTTONS:
				break;
			default:
				prterr("Error Unrecognized field type.");
				break;
		}
	}
	Rfree(new_screendefs);
	makescreendeflist();
	if(value>=screendef->numlibs) value=(screendef->numlibs)-1;
	if(!FINDRSCLISTAPPlib(mtnrsrc,"RESOURCE LIST",value,screendef))
		updatersrc(mtnrsrc,"RESOURCE LIST");
	changedvalues=TRUE;
}
void edit_resource(RDArsrc *mtnrsrc)
{
	int x=0;
	DEFfield *f=NULL;

	if(FINDRSCGETINT(mtnrsrc,"RESOURCE LIST",&x)) return;
	f=defdata->field+x;
	screendefscreen(mtnrsrc,f,saveresource,quitresourcetest);
}
void quitscreendef(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		NULL_RSCS(mtnrsrc);
	}
	if(defdata!=NULL) FreeRDAdefault(defdata);
	if(screendef!=NULL) freeapplib(screendef);
	if(mtndefaults!=NULL) FreeRDAdefault(mtndefaults);
}
void quitscreendeftest(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	readallwidgets(mtnrsrc);
	if(COMPARE_RSRCS(mtnrsrc,mtndefaults,0) || changedvalues)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this Screen Default Definition.\nExiting now without saving will loose these changes.",savescreendef,quitscreendef,FALSE,2,mtnrsrc,mainrsrc,NULL);
	} else {
		quitscreendef(mtnrsrc,mainrsrc);
	}
}
void savescreendef(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	char *libname=NULL,*libx=NULL;

	readallwidgets(mtnrsrc);
	if(FINDRSCGETSTRING(mtnrsrc,"INPUT FOCUS",&defdata->input_focus)) return;
	if(FINDRSCGETSTRING(mtnrsrc,"SCREEN NAME",&defdata->name)) return;
	if(FINDRSCGETSTRING(mtnrsrc,"SCREEN DEFAULT LIBRARY",&libname))
	{
		prterr("Error Couldn't write screen because the library name resource was not found.");
		return;
	}
	libx=Rmalloc(RDAstrlen(libname)+5);
	sprintf(libx,"%s.DEF",libname);
	writedefaultbin(libx,defdata);
	getscndefentries(mainrsrc);
	if(libname!=NULL) { Rfree(libname); libname=NULL; }
	if(libx!=NULL) { Rfree(libx); libx=NULL; }
	if(COMPARE_RSRCS(mtnrsrc,mtndefaults,0) || changedvalues)
	{
		changedvalues=TRUE;
	}
	quitscreendef(mtnrsrc,mainrsrc);
} 
void getdirscndef(RDArsrc *mainrsrc)
{
	int x,selectedl=0;
	short y=0;
	char *tmp=NULL,*directory=NULL,*dirx=NULL;
	RDArmem *member;

	if((y=FINDRSC(mainrsrc,"DIRECTORY"))!=(-1)) 
	{
		member=mainrsrc->rscs+y;
		if(IsWidgetDisplayed(member))
		{
			FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory);
		} else {
			if(FINDRSCGETINT(mainrsrc,"DIRECTORY LIST",&x)) return;
			directory=stralloc(dir_libs->libs[x]);
		}
	}
	FINDRSCGETINT(mainrsrc,"SCREEN DEFAULT LIBRARIES",&selectedl);
	if(deflt_bins!=NULL) { freeapplib(deflt_bins); deflt_bins=NULL; }
	deflt_bins=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(directory)+2);
#ifndef WIN32
	sprintf(dirx,"%s/",directory);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\",directory);
#endif
	for(x=0;findfile(dirx,"*.DEF",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(deflt_bins,tmp);
	}
	if(deflt_bins->numlibs<1)
	{
		addAPPlib(deflt_bins,"Directory Contains No Libraries");
	}
	if(directory!=NULL) Rfree(directory);
	if(dirx!=NULL) Rfree(dirx);
	if(tmp!=NULL) Rfree(tmp);
	SORTAPPlib(deflt_bins);
	if(selectedl>=deflt_bins->numlibs) selectedl=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"SCREEN DEFAULT LIBRARIES",selectedl,deflt_bins))
		updatersrc(mainrsrc,"SCREEN DEFAULT LIBRARIES");
}
void getscndefentries(RDArsrc *mainrsrc)
{
	int selected,selectedh=0;
	char *libx=NULL;
	char *dirx=NULL;

	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirx)) return;
	if(FINDRSCGETINT(mainrsrc,"SCREEN DEFAULT LIBRARIES",&selected)) return;
	if(defscn_defs!=NULL) { freeapplib(defscn_defs); defscn_defs=NULL; }
	if(!RDAstrcmp(deflt_bins->libs[selected],"Directory Contains No Libraries"))
	{
		defscn_defs=APPlibNEW();
		addAPPlib(defscn_defs,"Contains No Screen Default Definitions");
	} else {
		FINDRSCGETINT(mainrsrc,"SCREEN DEFAULT LIST",&selectedh);
		libx=Rmalloc(RDAstrlen(dirx)+RDAstrlen(deflt_bins->libs[selected])+6);
#ifndef WIN32
		sprintf(libx,"%s/%s.DEF",dirx,deflt_bins->libs[selected]);
#endif
#ifdef WIN32
		sprintf(libx,"%s\\%s.DEF",dirx,deflt_bins->libs[selected]);
#endif
		defscn_defs=getlibnames(libx,FALSE);
		if(dirx!=NULL) Rfree(dirx);
		if(libx!=NULL) Rfree(libx);
		if(defscn_defs==NULL)
		{
			defscn_defs=APPlibNEW();
			addAPPlib(defscn_defs,
				"Contains No Screen Default Definitions");
		} else {
			if(selectedh>=defscn_defs->numlibs) selectedh=0;
		}
	}
	if(!FINDRSCLISTAPPlib(mainrsrc,"SCREEN DEFAULT LIST"
			,selectedh,defscn_defs))
		updatersrc(mainrsrc,"SCREEN DEFAULT LIST");
}
static void setdirlist(RDArsrc *mainrsrc)
{
	int x=0;
	short y=0;
	char *directory=NULL;
	char inlist=FALSE;
	RDArmem *member;

	readwidget(mainrsrc,"DIRECTORY");
	if(FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&directory)) return;
	inlist=FALSE;
	for(x=0;x<dir_libs->numlibs;++x)
	{
		if(!RDAstrcmp(dir_libs->libs[x],directory))
		{
			inlist=TRUE;
			break;
		}
	}
	if(inlist!=TRUE)
	{
		addAPPlib(dir_libs,directory);
		x=dir_libs->numlibs-1;
	}
	if((y=FINDRSC(mainrsrc,"DIRECTORY LIST"))!=(-1)) 
	{
		member=mainrsrc->rscs+y;
		if(IsWidgetDisplayed(member))
		{
			FINDRSCLISTAPPlib(mainrsrc,"DIRECTORY LIST",x,dir_libs);
			updatersrc(mainrsrc,"DIRECTORY LIST");
			getdirscndef(mainrsrc);
			getscndefentries(mainrsrc);
		}
	}
	if(directory!=NULL) Rfree(directory);
}
static void getdir(RDArsrc *mainrsrc)
{
	int s=0;
	char *dirstr=NULL;

	if(FINDRSCGETINT(mainrsrc,"DIRECTORY LIST",&s)) return;
	if(!FINDRSCGETSTRING(mainrsrc,"DIRECTORY",&dirstr))
	{
		if(RDAstrcmp(dirstr,dir_libs->libs[s]))
		{
			FINDRSCSETSTRING(mainrsrc,"DIRECTORY",dir_libs->libs[s]);
			updatersrc(mainrsrc,"DIRECTORY");
		}
	}
	if(dirstr!=NULL) Rfree(dirstr);
	getdirscndef(mainrsrc);
	getscndefentries(mainrsrc);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	char *defdir=NULL;
	int w=2,x=0,m=0,s=0;
	RDArsrc *mainrsrc=NULL,*mtnrsrc=NULL;

	if(InitializeSubsystems(argc,argv,"GUI","DEFINE SCREEN DEFAULTS")) 
	{
		return;
	}
	mainrsrc=RDArsrcNEW("GUI","DEFINE SCREEN DEFAULTS");
	mtnrsrc=RDArsrcNEW("GUI","MAINTAIN SCREEN DEFAULTS");
	dir_libs=APPlibNEW();
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(USERLOGIN)+2);
#ifndef WIN32
	sprintf(defdir,"%s/%s",CURRENTDIRECTORY,USERLOGIN);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\%s",CURRENTDIRECTORY,USERLOGIN);
#endif
	addAPPlib(dir_libs,defdir);
	if(defdir!=NULL) Rfree(defdir);
	if(!isEMPTY(RDA_GROUP))
	{
	w=3;
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(RDA_GROUP)+2);
#ifndef WIN32
	sprintf(defdir,"%s/%s",CURRENTDIRECTORY,RDA_GROUP);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\%s",CURRENTDIRECTORY,RDA_GROUP);
#endif
	addAPPlib(dir_libs,defdir);
	if(defdir!=NULL) Rfree(defdir);
	}
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(defdir,"%s/site",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\site",CURRENTDIRECTORY);
#endif
	addAPPlib(dir_libs,defdir);
	if(defdir!=NULL) Rfree(defdir);
	defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+5);
#ifndef WIN32
	sprintf(defdir,"%s/rda",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(defdir,"%s\\rda",CURRENTDIRECTORY);
#endif
	addAPPlib(dir_libs,defdir);
	if(defdir!=NULL) Rfree(defdir);
	addlstrsrc(mainrsrc,"DIRECTORY LIST",&w,TRUE,getdir,dir_libs->numlibs,&dir_libs->libs,mainrsrc);
	if(defdir!=NULL) Rfree(defdir);
	FINDRSCLISTAPPlib(mainrsrc,"DIRECTORY LIST",w,dir_libs);
	addstdrsrc(mainrsrc,"DIRECTORY",VARIABLETEXT,0,dir_libs->libs[w],TRUE);
	FINDRSCSETFUNC(mainrsrc,"DIRECTORY",setdirlist,mainrsrc);
	if(defdir!=NULL) Rfree(defdir);
	deflt_bins=APPlibNEW();
	defscn_defs=APPlibNEW();
	addlstrsrc(mainrsrc,"SCREEN DEFAULT LIBRARIES",&x,TRUE,getscndefentries,
		deflt_bins->numlibs,&deflt_bins->libs,NULL);
	getdirscndef(mainrsrc);
	addlstrsrc(mainrsrc,"SCREEN DEFAULT LIST",&x,TRUE,NULL,defscn_defs->numlibs,
		&defscn_defs->libs,NULL);
	getscndefentries(mainrsrc);
	if(argc>1 && deflt_bins!=NULL)
	{
		for(m=0;m<deflt_bins->numlibs;++m)
		{
			if(!RDAstrcmp(deflt_bins->libs[m],argv[1])) break;
		}
		if(m<deflt_bins->numlibs)
		{
			FINDRSCSETINT(mainrsrc,"SCREEN DEFAULT LIBRARIES",m);
		}
		else
		{
			m=0;
			FINDRSCSETINT(mainrsrc,"SCREEN DEFAULT LIBRARIES",m);
		}
		getscndefentries(mainrsrc);
	}
	if(argc>2 && defscn_defs!=NULL)
	{
		for(s=0;s<defscn_defs->numlibs;++s)
		{
			if(!RDAstrcmp(defscn_defs->libs[s],argv[2])) break;
		}
		if(s<defscn_defs->numlibs)
		{
			FINDRSCSETINT(mainrsrc,"SCREEN DEFAULT LIST",s);
		}
		else
		{
			s=0;
			FINDRSCSETINT(mainrsrc,"SCREEN DEFAULT LIST",s);
		}
	}
	addbtnrsrc(mainrsrc,"ADD",TRUE,newdfscreendef,mtnrsrc);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,okdfscreendef,mtnrsrc);
	addbtnrsrc(mainrsrc,"DELETE",TRUE,deletetest,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitdfscreendef,mtnrsrc);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addstdrsrc(mtnrsrc,"SCREEN DEFAULT LIBRARY",VARIABLETEXT,80,NULL,TRUE);
	addstdrsrc(mtnrsrc,"SCREEN NAME",VARIABLETEXT,80,NULL,TRUE);
	addstdrsrc(mtnrsrc,"INPUT FOCUS",VARIABLETEXT,80,NULL,TRUE);
	defdata=RDAdefaultNEW(NULL,NULL);
	makescreendeflist();
	addlstrsrc(mtnrsrc,"RESOURCE LIST",0,TRUE,NULL,screendef->numlibs,
		&screendef->libs,NULL);
	addbtnrsrc(mtnrsrc,"ADD ABOVE",TRUE,add_rsc_above,NULL);
	addbtnrsrc(mtnrsrc,"ADD BELOW",TRUE,add_rsc_below,NULL);
	addbtnrsrc(mtnrsrc,"SELECT",TRUE,edit_resource,NULL);
	addbtnrsrc(mtnrsrc,"DELETE RESOURCE",TRUE,delete_rsc,NULL);
	addrfcbrsrc(mtnrsrc,"SAVE",TRUE,savescreendef,mainrsrc);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quitscreendeftest,mainrsrc);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT DEFAULTS",TRUE,printdefaultcb,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,quitdfscreendef,mtnrsrc,TRUE);
}
