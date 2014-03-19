/* dfprt.c - Define Output Devices */
#ifndef WIN32
#define __NAM__ "dfprt.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dfprt.exe"
#endif
#include <app.hpp>


#include <misc.hpp>


#include <gui.hpp>
#include <nrd.hpp>
#include <trans.hpp>
#include <mix.hpp>
#include <rptgen.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */
/*ADDLIB rpt */

static char *module="RPTGEN";
static APPlib *out_devs=NULL,*varavl=NULL,*prttypes=NULL;
static RDApdev *pdev=NULL;

static void deletedfout(RDArsrc *),quitdfout(RDArsrc *),okdfout(RDArsrc *);
static void makemtnscreen(RDArsrc *),newdfout(RDArsrc *);
static void savepdev(RDArsrc *,RDArsrc *);
static void quitpdev(RDArsrc *,RDArsrc *);
static void quitpdevtest(RDArsrc *,RDArsrc *);
static void getpdeventries(RDArsrc *),savepvar(RDArsrc *,RDArsrc *);
static void savepvarbelow(RDArsrc *,RDArsrc *);
static void savepvarabove(RDArsrc *,RDArsrc *);
static void quitpvar(RDArsrc *,RDArsrc *),quitpvartest(RDArsrc *,RDArsrc *);
static void quitpvarbelowtest(RDArsrc *,RDArsrc *);
static void quitpvarabovetest(RDArsrc *,RDArsrc *);
static void fixdevoutput(FILE *,char *,int *,int *,char *);
static void printdefinition(RDArsrc *,void (*)(...));
static void printdev(FILE *);
static void printall(FILE *);
static void printdev1(FILE *,RDApdev *);
static void printdevfp(RDArsrc *);
static void printheader(FILE *,int *,int *,char *,char *);
static void checkheader(FILE *,int *,int *,char *);
static void quit_print_definition(RDArsrc *);
static RDAdefault *editdefaults=NULL,*mtndefaults=NULL;
static char changedvalues=FALSE;

static void deletedfout(RDArsrc *mainrsrc)
{
	int s=0;
	char *libx=NULL;

	readallwidgets(mainrsrc); 
	if(FINDRSCGETINT(mainrsrc,"OUTPUT DEVICE LIST",&s)) return;
	if(!RDAstrcmp(out_devs->libs[s],"Contains No Output Devices")) return;
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+16);
#ifndef WIN32
	sprintf(libx,"%s/rda/PRTDEV.STP",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\PRTDEV.STP",CURRENTDIRECTORY);
#endif
	if(deletelibbin(libx,out_devs->libs[s]))
	{
		ERRORDIALOG("Cannot Delete Output Device","Error attempting to delete the output device.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
	} else {
	freeapplib(out_devs); 
	out_devs=getlibnames(libx,FALSE);
	if(out_devs==NULL)
	{
		out_devs=APPlibNEW();
		addAPPlib(out_devs,"Contains No Menu Definitions");
	}
	if(!FINDRSCLISTAPPlib(mainrsrc,"OUTPUT DEVICE LIST",0,out_devs))
		updatersrc(mainrsrc,"OUTPUT DEVICE LIST");
	}
	if(libx!=NULL) Rfree(libx);

}
static void deletedfouttest(RDArsrc *mainrsrc)
{
	WARNINGDIALOG("DELETE WARNING DIALOG SCREEN","DELETE DEVICE DEFINITION?","Are you sure that you want to DELETE this DEVICE DEFINITION?",deletedfout,NULL,FALSE,mainrsrc);
}
static void quitdfout(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	if(out_devs!=NULL) freeapplib(out_devs);
	if(mtndefaults!=NULL) FreeRDAdefault(mtndefaults);
	if(editdefaults!=NULL) FreeRDAdefault(editdefaults);
	ShutdownSubsystems();
}
static void makespacepvar(int pos)
{
	short x,y;
	RDApvar *temp,*temp1,*new_fields;

	++pdev->num;
	new_fields=Rmalloc(pdev->num*sizeof(RDApvar));
	y=0;
	for(x=0,temp=pdev->pvar;x<(pdev->num-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->name=NULL;
			temp1->command=NULL;
			temp1->chgsize=0;
			temp1->pitch=0;
			temp1->points=0;
			temp1->type=0;
			++y;
			temp1=new_fields+y;
		}
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		if(temp->command!=NULL)
		{
			temp1->command=stralloc(temp->command);
			Rfree(temp->command);
		} else temp1->command=NULL;
		temp1->type=temp->type;
		temp1->chgsize=temp->chgsize;
		temp1->pitch=temp->pitch;
		temp1->points=temp->points;
		++y;
	}
	Rfree(pdev->pvar);
	pdev->pvar=Rmalloc(pdev->num*sizeof(RDApvar));
	for(x=0,temp1=pdev->pvar,temp=new_fields;x<pdev->num;++x,
		++temp,++temp1)
	{
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		if(temp->command!=NULL)
		{
			temp1->command=stralloc(temp->command);
			Rfree(temp->command);
		} else temp1->command=NULL;
		temp1->type=temp->type;
		temp1->chgsize=temp->chgsize;
		temp1->pitch=temp->pitch;
		temp1->points=temp->points;
	}
	Rfree(new_fields);
}
void getpvars()
{
	int x,len=0;
	RDApvar *s;
	char *tmp=NULL;

	if(varavl!=NULL) freeapplib(varavl);
	varavl=APPlibNEW();
	for(x=0,s=pdev->pvar;x<pdev->num;++x,++s)
	{
		len=((s->name!=NULL ? RDAstrlen(s->name) : 0) +
			(s->command!=NULL ? RDAstrlen(s->command) : 0) + 2);
		if(tmp!=NULL) 
		{
			tmp=Rrealloc(tmp,len);
		} else {
			tmp=Rmalloc(len);
		}
		memset(tmp,0,len);
		sprintf(tmp,"%s %s",(s->name!=NULL?s->name:""),(s->command!=NULL?s->command:""));
		addAPPlib(varavl,tmp);
	}
	if(varavl->numlibs<1)
	{
		addAPPlib(varavl,"No Variables Defined");
	}
}
void quitpvar(RDArsrc *editrsrc,RDArsrc *mtnrsrc)
{
	if(prttypes!=NULL) freeapplib(prttypes);
	if(editrsrc!=NULL)
	{
		killwindow(editrsrc);
		free_rsrc(editrsrc);
	}
}
void quitpvarbelowtest(RDArsrc *editrsrc,RDArsrc *mtnrsrc)
{
	readallwidgets(editrsrc);
	if(COMPARE_RSRCS(editrsrc,editdefaults,2))
	{
		changedvalues=TRUE;
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the DEVICE VARIABLES.\nDo you want to Save these changes?",savepvarbelow,quitpvar,FALSE,2,editrsrc,mtnrsrc,NULL);
	} else {
		quitpvar(editrsrc,mtnrsrc);
	}
}
void quitpvarabovetest(RDArsrc *editrsrc,RDArsrc *mtnrsrc)
{
	readallwidgets(editrsrc);
	if(COMPARE_RSRCS(editrsrc,editdefaults,2))
	{
		changedvalues=TRUE;
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the DEVICE VARIABLES.\nDo you want to Save these changes?",savepvarabove,quitpvar,FALSE,2,editrsrc,mtnrsrc,NULL);
	} else {
		quitpvar(editrsrc,mtnrsrc);
	}
}
void quitpvartest(RDArsrc *editrsrc,RDArsrc *mtnrsrc)
{
	readallwidgets(editrsrc);
	if(COMPARE_RSRCS(editrsrc,editdefaults,2))
	{
		changedvalues=TRUE;
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the DEVICE VARIABLES.\nDo you want to Save these changes?",savepvar,quitpvar,FALSE,2,editrsrc,mtnrsrc,NULL);
	} else {
		quitpvar(editrsrc,mtnrsrc);
	}
}
void savepvar(RDArsrc *editrsrc,RDArsrc *mtnrsrc)
{
	int value=0;
	RDApvar *s;

	readallwidgets(editrsrc);
	if(FINDRSCGETINT(mtnrsrc,"DEVICE VARIABLES",&value)) return;
	if(pdev->pvar==NULL) 
	{
		pdev->pvar=Rmalloc(sizeof(RDApvar));
		pdev->num=1;
	}
	s=pdev->pvar+value;
	FINDRSCGETSTRING(editrsrc,"NAME",&s->name);
	FINDRSCGETSTRING(editrsrc,"COMMAND",&s->command);
	FINDRSCGETSHORT(editrsrc,"CHANGE SIZE",&s->chgsize);
	FINDRSCGETINT(editrsrc,"TYPES",&s->type);
	FINDRSCGETSHORT(editrsrc,"PITCH",&s->pitch);
	FINDRSCGETSHORT(editrsrc,"POINTS",&s->points);
	getpvars();
	if(!FINDRSCLISTAPPlib(mtnrsrc,"DEVICE VARIABLES",value,varavl))
		updatersrc(mtnrsrc,"DEVICE VARIABLES");
	if(COMPARE_RSRCS(editrsrc,editdefaults,2))
	{
		changedvalues=TRUE;
	}
	quitpvar(editrsrc,mtnrsrc);
}
void savepvarabove(RDArsrc *editrsrc,RDArsrc *mtnrsrc)
{
	int x;

	if(FINDRSCGETINT(mtnrsrc,"DEVICE VARIABLES",&x)) return;
	makespacepvar(x);
	savepvar(editrsrc,mtnrsrc);
}
void savepvarbelow(RDArsrc *editrsrc,RDArsrc *mtnrsrc)
{
	int x;

	if(FINDRSCGETINT(mtnrsrc,"DEVICE VARIABLES",&x)) return;
	makespacepvar(++x);
	FINDRSCSETINT(mtnrsrc,"DEVICE VARIABLES",x);
	savepvar(editrsrc,mtnrsrc);
}
void pdevscreen(RDArsrc *mtnrsrc,int num,char *name,int type,char *command,short chgsize,short pitch,short points,void (*savefunction)(...),void (*quitfunction)(...))
{
	char bools=FALSE;
	RDArsrc *editrsrc=NULL;

	editrsrc=RDArsrcNEW(module,"MAINTAIN DEVICE VARIABLES");
	addstdrsrc(editrsrc,"NUMBER",LONGV,8,&num,FALSE);
	addstdrsrc(editrsrc,"NAME",VARIABLETEXT,0,name,TRUE);
	addstdrsrc(editrsrc,"COMMAND",VARIABLETEXT,0,command,TRUE);
	if(chgsize) bools=TRUE;
	if(prttypes!=NULL) freeapplib(prttypes);
	prttypes=APPlibNEW();
	addAPPlib(prttypes,"Original");
	addAPPlib(prttypes,"DeReferenced File");
	addAPPlib(prttypes,"Binary File");
	addlstrsrc(editrsrc,"TYPES",&type,TRUE,NULL,prttypes->numlibs,
		&prttypes->libs,NULL);
	addstdrsrc(editrsrc,"CHANGE SIZE",BOOLNS,1,&bools,TRUE);
	addstdrsrc(editrsrc,"PITCH",SHORTV,2,&pitch,TRUE);
	addstdrsrc(editrsrc,"POINTS",SHORTV,2,&points,TRUE);
	addrfcbrsrc(editrsrc,"SAVE",TRUE,savefunction,mtnrsrc);
	addrfcbrsrc(editrsrc,"QUIT",TRUE,quitfunction,mtnrsrc);
	addbtnrsrc(editrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(editrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	editdefaults=GetDefaults(editrsrc);
	APPmakescrn(editrsrc,TRUE,quitpvar,mtnrsrc,FALSE);
}
void srtaddabove(RDArsrc *mtnrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(mtnrsrc,"DEVICE VARIABLES",&selected)) return;
	if(varavl->numlibs==1 && !RDAstrcmp(varavl->libs[0],"No Variables Defined")) return;
	pdevscreen(mtnrsrc,++selected,NULL,0,NULL,FALSE,0,0,savepvarabove,quitpvarabovetest);
}
void srtaddbelow(RDArsrc *mtnrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(mtnrsrc,"DEVICE VARIABLES",&selected)) return;
	if(varavl->numlibs==1 && !RDAstrcmp(varavl->libs[0],"No Variables Defined")) return;
	selected+=2;
	pdevscreen(mtnrsrc,selected,NULL,0,NULL,FALSE,0,0,savepvarbelow,quitpvarbelowtest);
}
void srtdelete(RDArsrc *mtnrsrc)
{
	short x,y;
	RDApvar *temp,*temp1,*new_fields;
	int value;

	readallwidgets(mtnrsrc);
	if(FINDRSCGETINT(mtnrsrc,"DEVICE VARIABLES",&value)) return;
	if(pdev->num==1 || (varavl->numlibs==1 && !RDAstrcmp(varavl->libs[0],"No Variables Defined"))) return;
	new_fields=Rmalloc((pdev->num-1)*sizeof(RDApvar));
	y=0;
	for(x=0,temp=pdev->pvar;x<pdev->num;++x,++temp)
	{
		temp1=new_fields+y;
		if(x!=value)
		{
			if(temp->name!=NULL)
			{
				temp1->name=stralloc(temp->name);
				Rfree(temp->name);
			} else temp1->name=NULL;
			temp1->type=temp->type;
			if(temp->command!=NULL)
			{
				temp1->command=stralloc(temp->command);
				Rfree(temp->command);
			} else temp1->command=NULL;
			temp1->chgsize=temp->chgsize;
			temp1->pitch=temp->pitch;
			temp1->points=temp->points;
			++y;
		} else {
			if(temp->name!=NULL) Rfree(temp->name);
			if(temp->command!=NULL) Rfree(temp->command);
		}
	}
	Rfree(pdev->pvar);
	--pdev->num;
	pdev->pvar=Rmalloc(pdev->num*sizeof(RDApvar));
	for(x=0,temp1=pdev->pvar,temp=new_fields;x<pdev->num;++x,
		++temp,++temp1)
	{
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->type=temp->type;
		if(temp->command!=NULL)
		{
			temp1->command=stralloc(temp->command);
			Rfree(temp->command);
		} else temp1->command=NULL;
		temp1->chgsize=temp->chgsize;
		temp1->pitch=temp->pitch;
		temp1->points=temp->points;
	}
	Rfree(new_fields);
	getpvars();
	if(value+1>varavl->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(mtnrsrc,"DEVICE VARIABLES",value,varavl))
		updatersrc(mtnrsrc,"DEVICE VARIABLES");
	changedvalues=TRUE;
}
void srtcopy(RDArsrc *mtnrsrc)
{
	int selected=0;
	RDApvar *s;

	if(FINDRSCGETINT(mtnrsrc,"DEVICE VARIABLES",&selected)) return;
	s=pdev->pvar+selected;
	if(varavl->numlibs==1 && !RDAstrcmp(varavl->libs[0],"No Variables Defined")) return;
	selected+=2;
	if(s!=NULL)
	{
		pdevscreen(mtnrsrc,selected,s->name,s->type,s->command,s->chgsize,s->pitch,s->points,savepvarbelow,quitpvarbelowtest);
	} else {
		pdevscreen(mtnrsrc,selected,NULL,0,NULL,FALSE,0,0,savepvarbelow,quitpvarbelowtest);
	}
}
void srtedit(RDArsrc *mtnrsrc)
{
	int selected=0;
	RDApvar *s;

	if(FINDRSCGETINT(mtnrsrc,"DEVICE VARIABLES",&selected)) return;
	s=pdev->pvar+selected;
	if(s!=NULL)
	{
		pdevscreen(mtnrsrc,++selected,s->name,s->type,s->command,s->chgsize,s->pitch,s->points,savepvar,quitpvartest);
	} else {
		pdevscreen(mtnrsrc,++selected,NULL,0,NULL,FALSE,0,0,savepvar,quitpvartest);
	}
}
static void quitpdevtest(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	readallwidgets(mtnrsrc);
	if(COMPARE_RSRCS(mtnrsrc,mtndefaults,0) || changedvalues)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this DEVICE DEFINITION.\nDo you want to Save these changes?",savepdev,quitpdev,FALSE,2,mtnrsrc,mainrsrc,NULL);
	} else {
		quitpdev(mtnrsrc,mainrsrc);
	}
}
static void quitpdev(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
	if(mtndefaults!=NULL) FreeRDAdefault(mtndefaults);
	if(pdev!=NULL) free_pdev(pdev);
	changedvalues=FALSE;
}
static void printall(FILE *fp)
{
	int x;
	char *deff=NULL;
	RDApdev *dev=NULL;

	if(!RDAstrcmp(out_devs->libs[0],"Contains No Output Devices")) return;
	for(x=0;x<out_devs->numlibs;++x)
	{
		deff=stralloc(out_devs->libs[x]);
		dev=RDApdevNEW(deff);	
		if(!getpdevbin(dev)) printdev1(fp,dev);
		if(dev!=NULL) free_pdev(dev);
		if(deff!=NULL) Rfree(deff);
		if(x+1<out_devs->numlibs) fprintf(fp,"\f");
	}
}
static void printdev(FILE *fp)
{
	printdev1(fp,pdev);
}
static void fixdevoutput(FILE *fp,char *data,int *lines,int *pages,char *devname)
{
	unsigned y=0,x=0,length=0,line_length=79;
	char *temp=NULL,*temp1=NULL;

	if(!isEMPTY(data))
	{
		checkheader(fp,lines,pages,devname);
		temp=strchr(data,'\n');
		temp1=strchr(data,'\r');
		if(temp==NULL && temp1==NULL) length=RDAstrlen(data);
		else if(temp==NULL) length=temp1-data;
		else if(temp1==NULL) length=temp-data;
		else if(temp<temp1) length=temp-data;
		else length=temp1-data;
		for(x=0,y=0;x<length;++x,++y)
		{
			if(data[x]=='\t')
			{
				if(y+9>line_length) break;
				y+=7;
			}
			if(y+2>line_length) break;
		}
		if(x<length)
		{
			fprintf(fp,"%.*s\r\n",x+1,data);
			++*lines;
			fixdevoutput(fp,data+x+1,lines,pages,devname);
		} else {
			fprintf(fp,"%.*s\r\n",length,data);
			++*lines;
			if(temp!=NULL || temp1!=NULL) fixdevoutput(fp,data+length+1,lines,pages,devname);
		}
	} 
}
static void printdev1(FILE *fp,RDApdev *dev)
{
	int lines=0,pages=0,x;
	char *temp=NULL;
	RDApvar *pvar;

	if(dev==NULL) return;
	temp=stralloc("XPERT OUTPUT DEVICE DEFINITION"); 
	printheader(fp,&lines,&pages,temp,dev->name);
	if(temp!=NULL) Rfree(temp);
	for(x=0,pvar=dev->pvar;x<dev->num;++x,++pvar,++lines)
	{
		temp=Rmalloc(31+(pvar->name!=NULL?RDAstrlen(pvar->name):0)
				+(pvar->command!=NULL?RDAstrlen(pvar->command):0));
		sprintf(temp,"Variable Name: [%s] Command: [%s]\r\n",
			(pvar->name!=NULL?pvar->name:""),
			(pvar->command!=NULL?pvar->command:""));
		fixdevoutput(fp,temp,&lines,&pages,dev->name);
		if(temp!=NULL) Rfree(temp);
		fprintf(fp,"Change Size: [%s] Pitch: [%d] Points: [%d]\r\n",
			(pvar->chgsize?"TRUE":"FALSE"),pvar->pitch,pvar->points);
		++lines;
		checkheader(fp,&lines,&pages,dev->name);
		fprintf(fp,"\r\n");
	}
}
static void quit_print_definition(RDArsrc *prsrc)
{
	if(prsrc!=NULL) Rfree(prsrc);
}
static void checkheader(FILE *fp,int *lines,int *pages,char *devname)
{
	char *temp=NULL;

	if(*lines>57)
	{
		fprintf(fp,"\f");
		temp=stralloc("XPERT MAINTAIN MASTER DEFINITION"); 
		printheader(fp,lines,pages,temp,devname);
		if(temp!=NULL) Rfree(temp);
	}
}
static void printheader(FILE *fp,int *lines,int *pages,char *header,char *devname)
{
	char *date=NULL,*timex=NULL;
	int lead_ws=0,tail_ws=0;
	unsigned line_length=79;

	date=GETCURRENTDATE();
	timex=GETCURRENTTIME();
	fprintf(fp,"\r\n");
	*lines=1;
	lead_ws=(int)(RDAroundup((double)(line_length-(2+RDAstrlen(header)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(2+RDAstrlen(header)))/2)-14));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(header)+2+14+lead_ws+tail_ws)>line_length) header[(line_length-14-lead_ws-tail_ws)]=0;
	fprintf(fp,"%*s %s %*sPage: %*d\r\n",
		lead_ws,"",header,tail_ws,"",8,++*pages);
	++*lines;
	lead_ws=(int)(RDAroundup((double)(line_length-(15+RDAstrlen(devname)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(15+RDAstrlen(devname)))/2)-14));
	if(tail_ws<0) tail_ws=0;
	fprintf(fp,"%*s Device Name: %s %*sDate: %s\r\n",
		lead_ws,"",(devname!=NULL?devname:""),tail_ws,"",date);
	++*lines;
	lead_ws=(int)(RDAroundup(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(15+RDAstrlen(USERLOGIN)))/2)-14));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(USERLOGIN)+14+15+lead_ws+tail_ws)>line_length)
	{
		fprintf(fp,"%*s %s %*sTime: %s\r\n",
			(int)(RDAroundup(((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2))),"",
			USERLOGIN,
			(int)(Rint((((double)(line_length-(2+RDAstrlen(USERLOGIN)))/2)-14))),"",
			timex);
		++*lines;
	} else {
		fprintf(fp,"%*s Executed By: %s %*sTime: %s\r\n",
			lead_ws,"",USERLOGIN,tail_ws,"",timex);
		++*lines;
	}
	fprintf(fp,"\r\n");
	++*lines;
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
}
static void printdefinition(RDArsrc *mtn,void (*printfunc)(...))
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL;

	prsrc=RDArsrcNEW(module,"PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addbtnrsrc(prsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
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
static void printfp(RDArsrc *prsrc,void (*pfunc)(...))
{
	char *outdevice=NULL,*temp=NULL;
	FILE *fp=NULL;
	readwidget(prsrc,"DEVICE");
        FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
	if(outdevice!=NULL)
	{
		fp=popen(outdevice,"w");
		if(fp!=NULL)
		{
/*lint -e746 */
			pfunc(fp);
/*lint +e746 */
			pclose(fp);
		} else {
			temp=Rmalloc(RDAstrlen(outdevice)+70);
			sprintf(temp,"The output attempt to the DEVICE [%s] failed when trying to open pipe.",outdevice);
			WARNINGDIALOG("WARNING DIALOG SCREEN","DEVICE COMMUNICATIONS FAILED!",temp,NULL,NULL,TRUE,NULL);
			prterr(temp);
			if(temp!=NULL) Rfree(temp);
		}
		if(outdevice!=NULL) Rfree(outdevice);
	} else {
		ERRORDIALOG("Output Device Failure","There was no output device specified.",NULL,FALSE);
	}
}
static void printallfp(RDArsrc *prsrc)
{
	printfp(prsrc,printall);
}
static void printdevfp(RDArsrc *prsrc)
{
	printfp(prsrc,printdev);
}
static void s_feeder(RDArsrc *mtnrsrc)
{
	char line_58=FALSE;

	readwidget(mtnrsrc,"58 LINE SHEET FEEDER");
	FINDRSCGETCHAR(mtnrsrc,"58 LINE SHEET FEEDER",&line_58);
	if(line_58==TRUE)
	{
		FINDRSCSETEDITABLE(mtnrsrc,"PAGE LENGTH",FALSE);
		FINDRSCSETSENSITIVE(mtnrsrc,"PAGE LENGTH",FALSE);
	} else {
		FINDRSCSETEDITABLE(mtnrsrc,"PAGE LENGTH",TRUE);
		FINDRSCSETSENSITIVE(mtnrsrc,"PAGE LENGTH",TRUE);
	}
}
void makemtnscreen(RDArsrc *mainrsrc)
{
	int x=0;
	RDArsrc *mtnrsrc=NULL;

	mtnrsrc=RDArsrcNEW(module,"MAINTAIN OUTPUT DEVICE");
	getpvars();
	addstdrsrc(mtnrsrc,"DEVICE NAME",VARIABLETEXT,0,pdev->name,TRUE);
	addstdrsrc(mtnrsrc,"EXTRA FORMFEED",BOOLNS,1,&pdev->formfeed,TRUE);
	addstdrsrc(mtnrsrc,"58 LINE SHEET FEEDER",BOOLNS,1,&pdev->line_58,TRUE);
	addstdrsrc(mtnrsrc,"ACTIVE",BOOLNS,1,&pdev->active,TRUE);
	FINDRSCSETFUNC(mtnrsrc,"58 LINE SHEET FEEDER",s_feeder,NULL);
	addstdrsrc(mtnrsrc,"PAGE LENGTH",DECIMALV,8,&pdev->page_length,TRUE);
	addlstrsrc(mtnrsrc,"DEVICE VARIABLES",&x,TRUE,NULL,varavl->numlibs,
		&varavl->libs,NULL);
	addbtnrsrc(mtnrsrc,"ADD ABOVE",TRUE,srtaddabove,NULL);
	addbtnrsrc(mtnrsrc,"ADD BELOW",TRUE,srtaddbelow,NULL);
	addbtnrsrc(mtnrsrc,"DELETE",TRUE,srtdelete,NULL);
	addbtnrsrc(mtnrsrc,"SELECT",TRUE,srtedit,NULL);
	addbtnrsrc(mtnrsrc,"COPY",TRUE,srtcopy,NULL);
	addrfcbrsrc(mtnrsrc,"SAVE",TRUE,savepdev,mainrsrc);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quitpdevtest,mainrsrc);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
/*lint -e611 */
	addrfcbrsrc(mtnrsrc,"PRINT DEFINITION",TRUE,printdefinition,(void*)printdevfp);
/*lint +e611 */
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	s_feeder(mtnrsrc);
	mtndefaults=GetDefaults(mtnrsrc);
	APPmakescrn(mtnrsrc,TRUE,quitpdev,mainrsrc,FALSE);
}
void okdfout(RDArsrc *mainrsrc)
{
	int h=0;
	char *deff=NULL;

	readallwidgets(mainrsrc);
	FINDRSCGETINT(mainrsrc,"OUTPUT DEVICE LIST",&h);
	if(RDAstrcmp(out_devs->libs[h],"Contains No Output Devices"))
	{
		deff=stralloc(out_devs->libs[h]);
		pdev=RDApdevNEW(deff);
		if(getpdevbin(pdev))
		{
			ERRORDIALOG("Cannot Read Output Device","Error in permissions, the user cannot read the output device binary library.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
			if(pdev!=NULL) free_pdev(pdev);
			if(deff!=NULL) Rfree(deff);
			return;
		}
		if(deff!=NULL) Rfree(deff);
	} else return;
	makemtnscreen(mainrsrc);
}
void newdfout(RDArsrc *mainrsrc)
{
	pdev=RDApdevNEW(NULL);
	makemtnscreen(mainrsrc);
}
void savepdev(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	readallwidgets(mtnrsrc);
	if(FINDRSCGETSTRING(mtnrsrc,"DEVICE NAME",&pdev->name)) return;
	FINDRSCGETCHAR(mtnrsrc,"EXTRA FORMFEED",&pdev->formfeed);
	FINDRSCGETCHAR(mtnrsrc,"58 LINE SHEET FEEDER",&pdev->line_58);
	FINDRSCGETCHAR(mtnrsrc,"ACTIVE",&pdev->active);
	FINDRSCGETDOUBLE(mtnrsrc,"PAGE LENGTH",&pdev->page_length);
	if(pdev->name==NULL)
	{
		ERRORDIALOG("DEVICE NAME NULL","The device name may not be null.  It must contain some letters, numbers, etc. Please enter a device name.",NULL,FALSE);
	} else if(pdev->num==0)
	{
		ERRORDIALOG("NO VARIABLES","There is no reason to record a device name that contains no variables.",NULL,FALSE);
	} else {
		writepdevbin(pdev);
		getpdeventries(mainrsrc);
	}
	quitpdev(mtnrsrc,mainrsrc); 
} 
void getpdeventries(RDArsrc *mainrsrc)
{
	char *libx=NULL;

	freeapplib(out_devs);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+16);
#ifndef WIN32
	sprintf(libx,"%s/rda/PRTDEV.STP",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\PRTDEV.STP",CURRENTDIRECTORY);
#endif
	out_devs=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	if(out_devs==NULL)
	{
		out_devs=APPlibNEW();
		addAPPlib(out_devs,"Contains No Output Devices");
	}
	if(!FINDRSCLISTAPPlib(mainrsrc,"OUTPUT DEVICE LIST",0,out_devs))
		updatersrc(mainrsrc,"OUTPUT DEVICE LIST");
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0;
	RDArsrc *mainrsrc=NULL;

	if(InitializeSubsystems(argc,argv,module,"DEFINE OUTPUT DEVICES")) 
	{
		return;
	}
	mainrsrc=RDArsrcNEW(module,"DEFINE OUTPUT DEVICES");
	out_devs=APPlibNEW();
	addlstrsrc(mainrsrc,"OUTPUT DEVICE LIST",&x,TRUE,NULL,out_devs->numlibs,
		&out_devs->libs,NULL);
	getpdeventries(mainrsrc);
	addbtnrsrc(mainrsrc,"ADD",TRUE,newdfout,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,okdfout,NULL);
	addbtnrsrc(mainrsrc,"DELETE",TRUE,deletedfouttest,NULL);
	addrfexrsrc(mainrsrc,"QUIT",TRUE,quitdfout,NULL);
	addrfcbrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
/*lint -e611 */
	addrfcbrsrc(mainrsrc,"PRINT",TRUE,printdefinition,(void*)printallfp);
/*lint +e611 */
	addrfcbrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(mainrsrc,TRUE,quitdfout,NULL,TRUE);
}
