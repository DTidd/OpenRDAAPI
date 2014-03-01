/* dftable.c - Define Tables */
#ifndef WIN32
#define __NAM__ "dftable.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dftable.exe"
#endif
#include <app.hpp>
#include <rptgen.hpp>
/*ADDLIB rpt */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

char *module="UTILITIES";
RDArsrc *mainrsrc=NULL,*mtnrsrc=NULL,*incrsrc=NULL;
APPlib *modlst=NULL,*tblavl=NULL,*tablelist=NULL;
RDATABLE *DFV=NULL;
static void getvrtentries(RDArsrc *),domodulelist(RDArsrc *);
static void savetable(RDArsrc *,RDArsrc *);
static void quittabletest(RDArsrc *,RDArsrc *),setvaluesx(RDArsrc *);
static void save_value(RDArsrc *,RDArsrc *);
static void quit_value(RDArsrc *,RDArsrc *),quit_valuetest(RDArsrc *,RDArsrc *);
static void quittable(RDArsrc *,RDArsrc *);
static RDAdefault *incdefaults=NULL,*virtdefaults=NULL;
static void print_tablescb(RDArsrc *);
static void print_tables(RDArsrc *,RDArsrc *);
static char changedvalues=FALSE;

static void makevalueslist()
{
	short x;
	TABLEVALUE *v=NULL;
	char *s=NULL;
	double pamt=0.0;

	if(tablelist!=NULL) freeapplib(tablelist);
	tablelist=APPlibNEW();
	if(DFV->values!=NULL)
	{
		pamt=0;
		for(x=0,v=DFV->values;x<DFV->num;++x,++v)
		{
			s=Rmalloc(100);
			sprintf(s,"%20.8f %20.8f %20.8f %20.8f",pamt,v->amount,
				v->base,v->rate);
			pamt=v->amount;
			addAPPlib(tablelist,s);
			Rfree(s);
		}
	}
	if(tablelist->numlibs<1)
	{
		addAPPlib(tablelist,"No Values Defined");
	}
}
static void makespacevaluelist(int pos)
{
	short x,y;
	TABLEVALUE *temp=NULL,*temp1=NULL,*new_fields=NULL;

	++DFV->num;
	new_fields=Rmalloc(DFV->num*sizeof(TABLEVALUE));
	y=0;
	for(x=0,temp=DFV->values;x<(DFV->num-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->amount=0;
			temp1->base=0;
			temp1->rate=0;
			++y;
			temp1=new_fields+y;
		}
		temp1->amount=temp->amount;
		temp1->base=temp->base;
		temp1->rate=temp->rate;
		++y;
	}
	Rfree(DFV->values);
	DFV->values=Rmalloc(DFV->num*sizeof(TABLEVALUE));
	for(x=0,temp1=DFV->values,temp=new_fields;x<DFV->num;++x,
		++temp,++temp1)
	{
		temp1->amount=temp->amount;
		temp1->base=temp->base;
		temp1->rate=temp->rate;
	}
	Rfree(new_fields);
}
static void quit_value(RDArsrc *incrsrc,RDArsrc *mtnrsrc)
{
	if(incrsrc!=NULL) 
	{
		killwindow(incrsrc);
		free_rsrc(incrsrc);
	}
	if(incdefaults!=NULL) FreeRDAdefault(incdefaults);
}
static void quit_valuetest(RDArsrc *incrsrc,RDArsrc *mtnrsrc)
{
	readallwidgets(incrsrc);
	if(COMPARE_RSRCS(incrsrc,incdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to Table Values.\nDo you want to Save these changes?",save_value,quit_value,FALSE,2,incrsrc,mtnrsrc,NULL);
	} else {
		quit_value(incrsrc,mtnrsrc);
	}
}
static void save_value(RDArsrc *incrsrc,RDArsrc *mtnrsrc)
{
	int value=0;
	TABLEVALUE *v=NULL;

	if(FINDRSCGETINT(mtnrsrc,"TABLE VALUES",&value)) return;
	v=DFV->values+value;
	readallwidgets(incrsrc);
	FINDRSCGETDOUBLE(incrsrc,"TO",&v->amount);
	FINDRSCGETDOUBLE(incrsrc,"BASE",&v->base);
	FINDRSCGETDOUBLE(incrsrc,"RATE",&v->rate);
	makevalueslist();
	if(!FINDRSCLISTAPPlib(mtnrsrc,"TABLE VALUES",value,tablelist))
		updatersrc(mtnrsrc,"TABLE VALUES");
	if(COMPARE_RSRCS(incrsrc,incdefaults,2))
	{
		changedvalues=TRUE;
	}
	quit_value(incrsrc,mtnrsrc);
}
static void save_value_above(RDArsrc *incrsrc,RDArsrc *mtnrsrc)
{
	int x;

	if(FINDRSCGETINT(mtnrsrc,"TABLE VALUES",&x)) return;
	makespacevaluelist(x);
	save_value(incrsrc,mtnrsrc);
}
static void save_value_below(RDArsrc *incrsrc,RDArsrc *mtnrsrc)
{
	int x;

	if(FINDRSCGETINT(mtnrsrc,"TABLE VALUES",&x)) return;
	makespacevaluelist(++x);
	FINDRSCSETINT(mtnrsrc,"TABLE VALUES",x);
	updatersrc(mtnrsrc,"TABLE VALUES");
	save_value(incrsrc,mtnrsrc);
}
static void quit_value_abovetest(RDArsrc *incrsrc,RDArsrc *mtnrsrc)
{
	readallwidgets(incrsrc);
	if(COMPARE_RSRCS(incrsrc,incdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the Include Files.\nDo you want to Save these changes?",save_value_above,quit_value,FALSE,2,incrsrc,mtnrsrc,NULL);
	} else {
		quit_value(incrsrc,mtnrsrc);
	}
}
static void quit_value_belowtest(RDArsrc *incrsrc,RDArsrc *mtnrsrc)
{
	readallwidgets(incrsrc);
	if(COMPARE_RSRCS(incrsrc,incdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the Include Files.\nDo you want to Save these changes?",save_value_below,quit_value,FALSE,2,incrsrc,mtnrsrc,NULL);
	} else {
		quit_value(incrsrc,mtnrsrc);
	}
}
static void valuescreen(RDArsrc *mtnrsrc,double from,double amount,double base,
	double rate,void (*savefunc)(...),void (*quitfunctest)(...))
{
	incrsrc=RDArsrcNEW(module,"MAINTAIN TABLE VALUES");
	addstdrsrc(incrsrc,"FROM",SDECIMALV,20,&from,FALSE);
	addstdrsrc(incrsrc,"TO",SDECIMALV,20,&amount,TRUE);
	addstdrsrc(incrsrc,"BASE",SDECIMALV,20,&base,TRUE);
	addstdrsrc(incrsrc,"RATE",SDECIMALV,20,&rate,TRUE);
	addrfcbrsrc(incrsrc,"SAVE",TRUE,savefunc,mtnrsrc);
	addrfcbrsrc(incrsrc,"QUIT",TRUE,quitfunctest,mtnrsrc);
	addbtnrsrc(incrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(incrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	incdefaults=GetDefaults(incrsrc);
	APPmakescrn(incrsrc,TRUE,quit_value,mtnrsrc,FALSE);
	FINDRSCSETINPUTFOCUS(incrsrc,"TO");
}
static void add_value_above(RDArsrc *mtnrsrc)
{
	int selected=0;
	double from=0.0,zamt=0.0;
	TABLEVALUE *v=NULL;

	if(FINDRSCGETINT(mtnrsrc,"TABLE VALUES",&selected)) return;
	if(tablelist->numlibs==1 && !RDAstrcmp(tablelist->libs[0],"No Values Defined")) return;
	if(selected>0)
	{
		v=DFV->values+(selected-1);
		from=v->amount;
	}
	valuescreen(mtnrsrc,from,zamt,zamt,zamt,save_value_above,quit_value_abovetest);
}
static void add_value_below(RDArsrc *mtnrsrc)
{
	int selected=0;
	double zamt=0.0;
	TABLEVALUE *v=NULL;

	if(FINDRSCGETINT(mtnrsrc,"TABLE VALUES",&selected)) return;
	if(tablelist->numlibs==1 && !RDAstrcmp(tablelist->libs[0],"No Values Defined")) return;
	v=DFV->values+selected;

	valuescreen(mtnrsrc,v->amount,zamt,zamt,zamt,save_value_below,quit_value_belowtest);
}
static void edit_value(RDArsrc *mtnrsrc)
{
	int x;
	TABLEVALUE *v,*v1=NULL;
	double from=0;

	if(FINDRSCGETINT(mtnrsrc,"TABLE VALUES",&x)) return;
	if(DFV->values==NULL)
	{
		DFV->values=Rmalloc(sizeof(TABLEVALUE));
		v=DFV->values;
		v->amount=0;
		v->base=0;
		v->rate=0;
		DFV->num=1;
	}
	if(x)
	{
		v1=DFV->values+(x-1);
		from=v1->amount;
	}
	v=DFV->values+x;
	valuescreen(mtnrsrc,from,v->amount,v->base,v->rate,save_value,quit_valuetest);
}
static void delete_value(RDArsrc *mtnrsrc)
{
	short x,y;
	TABLEVALUE *temp=NULL,*temp1=NULL,*new_fields=NULL;
	int value=0;

	readallwidgets(mtnrsrc);
	if(FINDRSCGETINT(mtnrsrc,"TABLE VALUES",&value)) return;
	if(DFV->num<2) return;
	new_fields=Rmalloc((DFV->num-1)*sizeof(TABLEVALUE));
	y=0;
	for(x=0,temp=DFV->values;x<DFV->num;++x,++temp)
	{
		temp1=new_fields+y;
		if(x!=value)
		{
			temp1->amount=temp->amount;
			temp1->base=temp->base;
			temp1->rate=temp->rate;
			++y;
		}
	}
	Rfree(DFV->values);
	--DFV->num;
	DFV->values=Rmalloc(DFV->num*sizeof(TABLEVALUE));
	for(x=0,temp1=DFV->values,temp=new_fields;x<DFV->num;++x,
		++temp,++temp1)
	{
		temp1->amount=temp->amount;
		temp1->base=temp->base;
		temp1->rate=temp->rate;
	}
	Rfree(new_fields);
	makevalueslist();
	if(!FINDRSCLISTAPPlib(mtnrsrc,"TABLE VALUES",value,tablelist))
		updatersrc(mtnrsrc,"TABLE VALUES");
	changedvalues=TRUE;
}
static void quittable(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	changedvalues=FALSE;
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
	if(virtdefaults!=NULL) FreeRDAdefault(virtdefaults);
}
static void quittabletest(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	readallwidgets(mtnrsrc);
	if(COMPARE_RSRCS(mtnrsrc,virtdefaults,2) || changedvalues)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","There are changes to the TABLE DEFINITION.\nDo you want to Save these changes?",savetable,quittable,FALSE,2,mtnrsrc,mainrsrc,NULL);
	} else {
		quittable(mtnrsrc,mainrsrc);
	}
}
static void savetable(RDArsrc *mtnrsrc,RDArsrc *mainrsrc)
{
	char *libname=NULL;
	
	readallwidgets(mtnrsrc);
	FINDRSCGETSTRING(mtnrsrc,"MODULE",&DFV->module);
	FINDRSCGETSTRING(mtnrsrc,"NAME",&DFV->name);
	if(!isEMPTY(DFV->module) && !isEMPTY(DFV->name))
	{
		FINDRSCGETSTRING(mtnrsrc,"DESCRIPTION",&DFV->desc);
		FINDRSCGETSTRING(mtnrsrc,"TITLE 1",&DFV->title1);
		FINDRSCGETSTRING(mtnrsrc,"TITLE 2",&DFV->title2);
		FINDRSCGETCHAR(mtnrsrc,"INCLUSIVE",&DFV->inclusive);
		libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(DFV->module)+10);
#ifndef WIN32
		sprintf(libname,"%s/rda/%s.TBL",CURRENTDIRECTORY,DFV->module);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\rda\\%s.TBL",CURRENTDIRECTORY,DFV->module);
#endif
		if(writeRDATablebin(libname,DFV))
		{
			ERRORDIALOG("Cannot Write RDA Table","Error attempting to write the table.  Check the permissions on the RDA Table Binary Library, and retry.  Call your installer.",NULL,FALSE);
		}
		if(libname!=NULL) Rfree(libname);
		FreeRDATABLE(DFV);
	}
	domodulelist(mainrsrc);
	/*getvrtentries();*/
	quittable(mtnrsrc,mainrsrc);
}
static void TableScreen(RDArsrc *mainrsrc,char *mod,char *name,char *title1,
	char *title2,char *desc,char inclusive,int num,TABLEVALUE *tv)
{
	int typex,x=0;
	RDArsrc *mtnrsrc=NULL;

	mtnrsrc=RDArsrcNEW(module,"MAINTAIN TABLES");
	addstdrsrc(mtnrsrc,"MODULE",VARIABLETEXT,15,(mod!=NULL ? mod:""),
		TRUE);
	addstdrsrc(mtnrsrc,"NAME",VARIABLETEXT,80,(name!=NULL ? name:""),
		TRUE);
	addstdrsrc(mtnrsrc,"TITLE 1",VARIABLETEXT,0,(title1!=NULL ? 
		title1:""),TRUE);
	addstdrsrc(mtnrsrc,"TITLE 2",VARIABLETEXT,0,(title2!=NULL ?
		title2:""),TRUE);
	addstdrsrc(mtnrsrc,"INCLUSIVE",BOOLNS,1,&inclusive,TRUE);
	addsctrsrc(mtnrsrc,"DESCRIPTION",0,(desc!=NULL ? desc:""),TRUE);
	if(tablelist!=NULL) freeapplib(tablelist);
	tablelist=APPlibNEW();
	typex=0;
	addlstrsrc(mtnrsrc,"TABLE VALUES",&typex,TRUE,NULL,
		tablelist->numlibs,&tablelist->libs,NULL);
	addbtnrsrc(mtnrsrc,"ADD VALUE ABOVE",TRUE,add_value_above,NULL);
	addbtnrsrc(mtnrsrc,"ADD VALUE BELOW",TRUE,add_value_below,NULL);
	addbtnrsrc(mtnrsrc,"DELETE VALUE",TRUE,delete_value,NULL);
	addbtnrsrc(mtnrsrc,"SELECT VALUE",TRUE,edit_value,NULL);
	makevalueslist();
	x=0;
	FINDRSCLISTAPPlib(mtnrsrc,"TABLE VALUES",x,tablelist);
	addrfcbrsrc(mtnrsrc,"SAVE",TRUE,savetable,mainrsrc);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quittabletest,mainrsrc);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	virtdefaults=GetDefaults(mtnrsrc);
	APPmakescrn(mtnrsrc,TRUE,quittable,mainrsrc,FALSE);
}
static void oktable(RDArsrc *mainrsrc)
{
	int selected=0,selectedv=0;
	char *libname=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(FINDRSCGETINT(mainrsrc,"TABLES AVAILABLE",&selectedv)) return;
	if(DFV!=NULL) { FreeRDATABLE(DFV); DFV=NULL; }
	if(RDAstrcmp(tblavl->libs[selectedv],"No Table Definitions")) 
	{
		DFV=RDATABLENEW(modlst->libs[selected],tblavl->libs[selectedv]);
		libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlst->libs[selected])+10);
#ifndef WIN32
		sprintf(libname,"%s/rda/%s.TBL",CURRENTDIRECTORY,modlst->libs[selected]);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\rda\\%s.TBL",CURRENTDIRECTORY,modlst->libs[selected]);
#endif
		getRDATablebin(libname,DFV);
		if(libname!=NULL) Rfree(libname);
		if(DFV==NULL)
		{
			ERRORDIALOG("Cannot Read RDA Table","Error in permissions, the user cannot read the table binary library.  Check the permissions, and retry.  Call your installer.",NULL,FALSE);
			return;
		}
	}
	if(DFV==NULL)
	{
		DFV=RDATABLENEW(modlst->libs[selected],NULL);
	}
	TableScreen(mainrsrc,DFV->module,DFV->name,DFV->title1,DFV->title2,
		DFV->desc,DFV->inclusive,DFV->num,DFV->values);
}
static void newtable(RDArsrc *mainrsrc)
{
	int selected=0;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	if(DFV!=NULL) { FreeRDATABLE(DFV); DFV=NULL; }
	DFV=RDATABLENEW(modlst->libs[selected],NULL);
	TableScreen(mainrsrc,DFV->module,DFV->name,DFV->title1,DFV->title2,
		DFV->desc,DFV->inclusive,DFV->num,DFV->values);
}
static void quitdfvir(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL) free_rsrc(mainrsrc);
	if(modlst!=NULL) freeapplib(modlst);
	if(tblavl!=NULL) freeapplib(tblavl);
	if(incdefaults!=NULL) FreeRDAdefault(incdefaults);
	if(virtdefaults!=NULL) FreeRDAdefault(virtdefaults);
	ShutdownSubsystems();
	std::exit;
}
static void getvrtentries(RDArsrc *mainrsrc)
{
	int x,selectedm=0;
	char *tmp=NULL,*dirx=NULL;
	
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(modlst!=NULL) { freeapplib(modlst); modlst=NULL; }
	modlst=APPlibNEW();
	dirx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+6);
#ifndef WIN32
	sprintf(dirx,"%s/rda/",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(dirx,"%s\\rda\\",CURRENTDIRECTORY);
#endif
	for(x=0;findfile(dirx,"*.TBL",&tmp,(int)x+1);++x) 
	{
		tmp[RDAstrlen(tmp)-4]=0;
		addAPPlib(modlst,tmp);
	}
	if(modlst->numlibs<1)
	{
		addAPPlib(modlst,"Contains No Database Libraries");
		selectedm=0;
	} else {
		SORTAPPlib(modlst);
		if(selectedm>=modlst->numlibs) selectedm=0;
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	if(!FINDRSCLISTAPPlib(mainrsrc,"MODULE LIST",selectedm,modlst))
	{
		updatersrc(mainrsrc,"MODULE LIST");
	}
	domodulelist(mainrsrc);
}
static void deletevir(RDArsrc *mainrsrc)
{
	int s,m=0;
	char *libx=NULL,*modulename=NULL;

	readallwidgets(mainrsrc);
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&m)) return;
	if(FINDRSCGETINT(mainrsrc,"TABLES AVAILABLE",&s)) return;
	modulename=adddashes(modlst->libs[m]);	
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modulename)+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.TBL",CURRENTDIRECTORY,modulename);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.TBL",CURRENTDIRECTORY,modulename);
#endif
	if(deletelibbin(libx,tblavl->libs[s]))
	{
		ERRORDIALOG("Cannot Delete Virtual","Error attempting to delete table.  Check the permissions on the RDA Table Binary Library, and retry.  Call your installer.",NULL,FALSE);
	} else getvrtentries(mainrsrc);
	if(libx!=NULL) Rfree(libx);
	if(modulename!=NULL) Rfree(modulename);
}
static void deletevirtest(RDArsrc *mainrsrc)
{
	int s=0;

	if(!RDAstrcmp(tblavl->libs[s],"No Table Definitions")) return;
	WARNINGDIALOG("DELETE WARNING DIALOG SCREEN","DELETE TABLE","Are you sure you want to delete this table?",deletevir,NULL,FALSE,mainrsrc);
}
static void setvaluesx(RDArsrc *mainrsrc)
{
	int selected=0,selectedm=0;
	char *temp=NULL,*title1=NULL,*title2=NULL;
	char *libname=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selectedm)) return;
	if(FINDRSCGETINT(mainrsrc,"TABLES AVAILABLE",&selected)) return;
	if(!RDAstrcmp(tblavl->libs[selected],"No Table Definitions"))
	{
		temp=NULL;
	} else {
		libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlst->libs[selectedm])+10);
#ifndef WIN32
		sprintf(libname,"%s/rda/%s.TBL",CURRENTDIRECTORY,modlst->libs[selectedm]);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\rda\\%s.TBL",CURRENTDIRECTORY,modlst->libs[selectedm]);
#endif
		getRDATabledesc(libname,tblavl->libs[selected],&temp,&title1,&title2);
		if(libname!=NULL) Rfree(libname);
	}
	if(!FINDRSCSETSTRING(mainrsrc,"DESCRIPTION",temp))
		updatersrc(mainrsrc,"DESCRIPTION");
	if(!FINDRSCSETSTRING(mainrsrc,"TITLE 1",title1))
		updatersrc(mainrsrc,"TITLE 1");
	if(!FINDRSCSETSTRING(mainrsrc,"TITLE 2",title2))
		updatersrc(mainrsrc,"TITLE 2");
	if(temp!=NULL) Rfree(temp);
	if(title1!=NULL) Rfree(title1);
	if(title2!=NULL) Rfree(title2);
}
static void domodulelist(RDArsrc *mainrsrc)
{
	int selected=0,selectedf=0;
	char *libx=NULL;

	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	FINDRSCGETINT(mainrsrc,"TABLES AVAILABLE",&selectedf);
	if(tblavl!=NULL) { freeapplib(tblavl); tblavl=NULL; }
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlst->libs[selected])+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.TBL",CURRENTDIRECTORY,modlst->libs[selected]);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.TBL",CURRENTDIRECTORY,modlst->libs[selected]);
#endif
	tblavl=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	if(tblavl==NULL)
	{
		tblavl=APPlibNEW();
		addAPPlib(tblavl,"No Table Definitions");
	}
	if(selectedf>=tblavl->numlibs) selectedf=0;
	if(!FINDRSCLISTAPPlib(mainrsrc,"TABLES AVAILABLE",selectedf,tblavl))
		updatersrc(mainrsrc,"TABLES AVAILABLE");
	setvaluesx(mainrsrc);
}
static void print_tables(RDArsrc *prsrc,RDArsrc *mainrsrc)
{
	RDA_PFILE *fp=NULL;
	int x=0,y=0,selected=0;
	char *tmpx=NULL,*libx=NULL,*odevice=NULL,print_style=TRUE;
	RDATABLE *d=NULL;
	APPlib *virtnames=NULL;
	TABLEVALUE *v=NULL;
	double pamt=0;

	readwidget(prsrc,"DEVICE");
	FINDRSCGETSTRING(prsrc,"DEVICE",&odevice);
	readwidget(prsrc,"NORMAL PRINT");
	FINDRSCGETCHAR(prsrc,"NORMAL PRINT",&print_style);
	if(odevice==NULL)
	{
		ERRORDIALOG("Output Device Failure","There was no device specified.",NULL,FALSE);
		return;
	}
	if(FINDRSCGETINT(mainrsrc,"MODULE LIST",&selected)) return;
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlst->libs[selected])+10);
	tmpx=adddashes(modlst->libs[selected]);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.TBL",CURRENTDIRECTORY,tmpx);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.TBL",CURRENTDIRECTORY,tmpx);
#endif
	virtnames=getlibnames(libx,FALSE);
	if(virtnames!=NULL)
	{
		fp=RDA_popen(odevice,"w");
		if(fp==NULL)
		{
			prterr("Error Couldn't open device.");
			if(virtnames!=NULL) freeapplib(virtnames);
			if(tmpx!=NULL) Rfree(tmpx);
			if(libx!=NULL) Rfree(libx);
			if(odevice!=NULL) Rfree(odevice);
			return;
		} 
		SelectPrintType(odevice,print_style,fp);
		if(odevice!=NULL) Rfree(odevice);
		RDA_fprintf(fp,"TABLES AVAILABLE\r\n");
		for(x=0;x<virtnames->numlibs;++x)
		{
			d=RDATABLENEW(modlst->libs[selected],virtnames->libs[x]);
			getRDATablebin(libx,d);
			if(d!=NULL)
			{
				RDA_fprintf(fp,"Module:%s Name:%s \r\n",d->module,d->name);
				RDA_fprintf(fp,"\tTitle #1:%s\r\n",(d->title1!=NULL ? d->title1:""));
				RDA_fprintf(fp,"\tTitle #2:%s\r\n",(d->title2!=NULL ? d->title2:""));
				RDA_fprintf(fp,"\tDescription:%s\r\n",(d->desc!=NULL ? d->desc:""));
				RDA_fprintf(fp,"\tFrom            To              Base            Rate\r\n");
				RDA_fprintf(fp,"\t--------------- --------------- --------------- ---------\r\n");
				pamt=0;
				for(y=0,v=d->values;y<d->num;++y,++v)
				{
					RDA_fprintf(fp,"\t%15.4f %15.4f %15.4f %15.4f\r\n",pamt,
						v->amount,v->base,v->rate);
					pamt=v->amount;
				}
				RDA_fprintf(fp,"\r\n");
				FreeRDATABLE(d);
				d=NULL;
			}
		}
		RDA_pclose(fp);
		if(virtnames!=NULL) freeapplib(virtnames);
	} else {
		prterr("Error No Table Available.");
	}
	if(libx!=NULL) Rfree(libx);
	if(tmpx!=NULL) Rfree(tmpx);
}
static void quit_print_tables(RDArsrc *prsrc)
{
	free_rsrc(prsrc);
}
static void print_tablescb(RDArsrc *mainrsrc)
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL,normal_print=TRUE;

	prsrc=RDArsrcNEW(module,"PRINT SCREEN");
	defaultprinter=DefaultPrinter();
	addbtnrsrc(prsrc,"LOAD DEVICE NAME",TRUE,LoadDeviceWindow,SelectFuncDevice);
	addstdrsrc(prsrc,"DEVICE",VARIABLETEXT,0,defaultprinter,TRUE);
	addstdrsrc(prsrc,"NORMAL PRINT",BOOLNS,1,&normal_print,TRUE);
	if(defaultprinter!=NULL) Rfree(defaultprinter);
	addrfkwrsrc(prsrc,"PRINT",TRUE,print_tables,mainrsrc);
	addrfkwrsrc(prsrc,"QUIT",TRUE,quit_print_tables,NULL);
	addbtnrsrc(prsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(prsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	addbtnrsrc(prsrc,"DEFAULTS",TRUE,SaveDefaults,NULL);
	DefaultScreens(prsrc);
	if(ADVmakescrn(prsrc,TRUE))
	{
		ERRORDIALOG("MAKESCRN FAILED","The Make Screen function failed for the custom screen PRINT SCREEN. Check to see the screen is available. If it is, call your installer.",NULL,FALSE);
		quit_print_tables(prsrc);
	}
}
static void print_help(void)
{
	prterr("Usage:\n");
	prterr("           Define RDA Tables Command Summary\n");
	prterr("           ------ ---------- ------- -------\n");
	prterr("  Windows     : dftable.EXT [module [table_name]]\n");
	prterr("  Command-line: dftable.EXT module table_name -c command ...\n");
	prterr("    command(s)=\n");
	prterr("              -copy to_module to_name\n");
	prterr("                    (copy table)\n");
	prterr("    examples=\n");
	prterr("     *  dftable.rsc OCCTAX \"1996-CLASS1\" -c -copy OCCTAX \"1997-CLASS1\" \n");
	prterr("      (This command would copy the 1996-CLASS1 table definition \n");
	prterr("       to the table 1997-CLASS1 both in the OCCTAX module).\n");
}
static void copytable(RDArsrc *rsrc,char *copyfrommodule,char *copyfromname,char *copytomodule,char *copytoname)
{
	char *libname=NULL;

	if(DFV!=NULL) { FreeRDATABLE(DFV); DFV=NULL; }
	if(!isEMPTY(copyfrommodule) && !isEMPTY(copyfromname) &&
	!isEMPTY(copytomodule) && !isEMPTY(copytoname))
	{
		DFV=RDATABLENEW(copyfrommodule,copyfromname);
		libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(copyfrommodule)+10);
#ifndef WIN32
		sprintf(libname,"%s/rda/%s.TBL",CURRENTDIRECTORY,copyfrommodule);
#endif
#ifdef WIN32
		sprintf(libname,"%s\\rda\\%s.TBL",CURRENTDIRECTORY,copyfrommodule);
#endif
		getRDATablebin(libname,DFV);
		if(libname!=NULL) Rfree(libname);
		if(DFV==NULL)
		{
			prterr("Error: Cannot read RDA Table Definition, either table does not exist or there is an error in permissions.  The user cannot read the table binary library.  Check the permissions, and retry.  Call your installer.");
			return;
		}
	
		DFV->module=stralloc(copytomodule);
		DFV->name=stralloc(copytoname);
		if(!isEMPTY(DFV->module) && !isEMPTY(DFV->name))
		{
			libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(DFV->module)+10);
#ifndef WIN32
			sprintf(libname,"%s/rda/%s.TBL",CURRENTDIRECTORY,DFV->module);
#endif
#ifdef WIN32
			sprintf(libname,"%s\\rda\\%s.TBL",CURRENTDIRECTORY,DFV->module);
#endif
			if(writeRDATablebin(libname,DFV))
			{
				prterr("Error: Cannot write RDA Table Definition.  Check the permissions on the RDA Table Binary Library, and retry.");
			}
			if(libname!=NULL) Rfree(libname);
			FreeRDATABLE(DFV);
		}
	}
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0;
	int a=0;
	RDArsrc *mainrsrc=NULL;
	char valid_args=TRUE;
	char *copyfrommodule=NULL;
	char *copyfromname=NULL;
	char *copytomodule=NULL;
	char *copytoname=NULL;

	if(InitializeSubsystems(argc,argv,module,"DEFINE TABLES")) 
	{
		return;
	}
	mainrsrc=RDArsrcNEW(module,"DEFINE TABLES");
	x=0;
	if(modlst!=NULL) freeapplib(modlst);
	modlst=APPlibNEW();
	addlstrsrc(mainrsrc,"MODULE LIST",&x,TRUE,domodulelist,
		modlst->numlibs,&modlst->libs,NULL);
	x=0;
	if(tblavl!=NULL) { freeapplib(tblavl); tblavl=NULL; }
	tblavl=APPlibNEW();
	addlstrsrc(mainrsrc,"TABLES AVAILABLE",&x,TRUE,setvaluesx,
		tblavl->numlibs,&tblavl->libs,NULL);
	addstdrsrc(mainrsrc,"TITLE 1",VARIABLETEXT,0,NULL,FALSE);
	addstdrsrc(mainrsrc,"TITLE 2",VARIABLETEXT,0,NULL,FALSE);
	addsctrsrc(mainrsrc,"DESCRIPTION",0,NULL,FALSE);
	getvrtentries(mainrsrc);
	if(argc<4)
	{
		if(argc>1)
		{
			for(x=0;x<modlst->numlibs;++x)
			{
				if(!RDAstrcmp(modlst->libs[x],argv[1])) break;
			}
			if(x>=modlst->numlibs) x=0;
			FINDRSCSETINT(mainrsrc,"MODULE LIST",x);
			domodulelist(mainrsrc);
		}
		if(argc>2)
		{
			for(x=0;x<tblavl->numlibs;++x)
			{
				if(!RDAstrcmp(tblavl->libs[x],argv[2])) break;
			}
			if(x>=tblavl->numlibs) x=0;
			FINDRSCSETINT(mainrsrc,"TABLES AVAILABLE",x);
			setvaluesx(mainrsrc);
		}
		addbtnrsrc(mainrsrc,"ADD",TRUE,newtable,NULL);
		addbtnrsrc(mainrsrc,"SELECT",TRUE,oktable,NULL);
		addbtnrsrc(mainrsrc,"PRINT",TRUE,print_tablescb,NULL);
		addrfexrsrc(mainrsrc,"QUIT",TRUE,quitdfvir,NULL);
		addbtnrsrc(mainrsrc,"DELETE",TRUE,deletevirtest,NULL);
		addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
		addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
		APPmakescrn(mainrsrc,TRUE,quitdfvir,NULL,TRUE);
	} else if(argc>3 && !RDAstrcmp(argv[3],"-c")) 
	{
		for(a=4;a<argc;)
		{
			if(!RDAstrcmp(argv[a],"-copy"))
			{
				if(argc>(a+2))
				{
					copytomodule=stralloc(argv[a+1]);
					copytoname=stralloc(argv[a+2]);
					a+=3;
					copyfrommodule=stralloc(argv[1]);
					copyfromname=stralloc(argv[2]);
				} else {
					valid_args=FALSE;
					break;
				}
			} else {
				valid_args=FALSE;
				break;
			}
			
		}
		if(valid_args==TRUE)
		{
			copytable(NULL,copyfrommodule,copyfromname,copytomodule,copytoname);
		} else {
			print_help();
		}
		if(copyfrommodule!=NULL) Rfree(copyfrommodule);
		if(copyfromname!=NULL) Rfree(copyfromname);
		if(copytomodule!=NULL) Rfree(copytomodule);
		if(copytoname!=NULL) Rfree(copytoname);
	} else {
		print_help();
	}
}
