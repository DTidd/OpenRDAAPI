/* dfmtnsub - Define Subordinate Files section of Define Maintain Masters */
#include <cstdio>





#include <mix.hpp>
#include <rptgen.hpp>
#include <ldval.hpp>

extern APPlib *makefieldvallist2mtn(RDAsearch *,int,int,short,char *,char *);
extern APPlib *holdapplib;
/*
extern MaintainMaster *MTNMSTR;
*/
extern RDArsrc *mtnrsrc;
extern char *module,changedvalues;
extern RDAsearch *tempsupport;
extern int tmpnumsupp;
void setconfldlist(RDArsrc *,APPlib *,char *);
static RDAdefault *msubsdefaults=NULL,*econdefaults=NULL;
static char changedsubords=FALSE;
static RDAconfld *holdconfld=NULL;
extern RDAsubord *tempsubord;
extern int tmpnumsub;
extern APPlib *modlstxmtn,*subavlmtn,*filelstmtn,*cmodlstmtn,*subkeylistmtn,*cnfldsmtn,*econfldsmtn;
APPlib *subwriteavl=NULL;
static int holdnum=0;
static int ptrue=TRUE,pfalse=FALSE;
static void loadvalue2mtn(RDArsrc *,char *);
void savesub(RDArsrc *,MaintainMaster *),subedit(RDArsrc *,RDArsrc *),subdelete(RDArsrc *);
void subaddabove(RDArsrc *,RDArsrc *),subaddbelow(RDArsrc *,RDArsrc *);
void getsubords(void);
PowerWrite *tempSubWrites=NULL;
int tmpnumSubWrites=0;
extern void savewrt(RDAsubord *);
extern void wrtcopy(RDArsrc *),wrtedit(RDArsrc *);
extern void getSubWrites(void);
extern void wrtaddbelow(RDArsrc *),wrtaddabove(RDArsrc *);
extern void wrtdelete(RDArsrc *);
extern void addholdvaluessub(APPlib *,char *,char *);

static void loadvalue2mtn(RDArsrc *r,char *fieldname)
{
	int x,selectedm=0,selectedf=0;
	LoadValue *load;
	RDArsrc *ldvalrsrc1=NULL;
	char *modname=NULL,*filename=NULL;

	ldvalrsrc1=RDArsrcNEW("RPTGEN","LOAD VALUE SCREEN");
	load=LoadValueNew(ldvalrsrc1,r,fieldname);
	if(loadlist!=NULL) freeapplib(loadlist);
	FINDRSCGETINT(r,"MODULE",&selectedm);
	FINDRSCGETINT(r,"FILENAME",&selectedf);
	if(RDAstrcmp(modlstxmtn->libs[selectedm],"Contains No Database Libraries"))
	{
		modname=stralloc(modlstxmtn->libs[selectedm]);
	}
	if(RDAstrcmp(filelstmtn->libs[selectedf],"Contains No File Definitions"))
	{
		filename=stralloc(filelstmtn->libs[selectedf]);
	}
	loadlist=makefieldvallist2mtn(tempsupport,tmpnumsupp,tmpnumsupp,TRUE,modname,filename);
	if(modname!=NULL) Rfree(modname);
	if(filename!=NULL) Rfree(filename);
	if(loadlist->numlibs<1)
	{
		addAPPlib(loadlist,"Load List Unavailable");
	}
	x=0;
	if(loadlist->numlibs>1)
	{
		for(x=0;x<loadlist->numlibs;++x)
		{
			if(strncmp(loadlist->libs[x],"GENERIC_SETUP",13)) break;
		}
		if(x>=loadlist->numlibs) x=0;
	}
	addlstrsrc(ldvalrsrc1,"LOAD LIST",&x,TRUE,NULL,loadlist->numlibs,
		&loadlist->libs,NULL);
	addrfkwrsrc(ldvalrsrc1,"SELECT",TRUE,okldval,load);
	addrfkwrsrc(ldvalrsrc1,"QUIT",TRUE,quitldval,load);
	addbtnrsrc(ldvalrsrc1,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(ldvalrsrc1,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(ldvalrsrc1,TRUE,quitldval,load,FALSE);
}
void getsubords()
{
	int x,len=0;
	RDAsubord *subs;
	char *tmp=NULL;

	if(subavlmtn!=NULL) freeapplib(subavlmtn);
	subavlmtn=APPlibNEW();
	if(tempsubord!=NULL)
	{
		for(x=0,subs=tempsubord;x<tmpnumsub;++x,++subs)
		{
			len=RDAstrlen(subs->module)+RDAstrlen(subs->filename)+17+RDAstrlen(subs->cmodule)+RDAstrlen(subs->cfilename)+RDAstrlen(subs->subkeyname);
			if(tmp!=NULL) 
			{
				tmp=Rrealloc(tmp,len);
			} else {
				tmp=Rmalloc(len);
			}
			memset(tmp,0,len);
			sprintf(tmp,"%2d %s %s %s %s %s",x+1,
				(subs->module!=NULL?subs->module:""),
				(subs->filename!=NULL?subs->filename:""),
				(subs->cmodule!=NULL?subs->cmodule:""),
				(subs->cfilename!=NULL?subs->cfilename:""),
				(subs->subkeyname!=NULL?subs->subkeyname:""));
			addAPPlib(subavlmtn,tmp);
		}
	}
	if(subavlmtn->numlibs<1)
	{
		addAPPlib(subavlmtn,"No Files Defined");
	}
}
static void quitmsubo(RDArsrc *msuborsrc,RDArsrc *subrsrc)
{
	int x,y;
	RDAconfld *cf;
	PowerWrite *W=NULL;
	PowerWriteField *WF=NULL;

	if(msuborsrc!=NULL)
	{
		killwindow(msuborsrc);
		free_rsrc(msuborsrc);
	}
	if(tempSubWrites!=NULL)
	{
		for(x=0,W=tempSubWrites;x<tmpnumSubWrites;++x,++W)
		{
			if(W->module!=NULL) Rfree(W->module);
			if(W->filename!=NULL) Rfree(W->filename);
			if(W->expression!=NULL) Rfree(W->expression);
			if(W->fields!=NULL)
			{
				for(y=0,WF=W->fields;y<W->numflds;++y,++WF)
				{
					if(WF->to_name!=NULL) Rfree(WF->to_name);
					if(WF->from_name!=NULL) Rfree(WF->from_name);
				}
				Rfree(W->fields);
			}
			W->numflds=0;
		}
		Rfree(tempSubWrites);
	}
	tempSubWrites=NULL;
	tmpnumSubWrites=0;
	if(holdconfld!=NULL)
	{
		for(x=0,cf=holdconfld;x<holdnum;++x,++cf)
		{
			if(cf->fldname!=NULL) Rfree(cf->fldname);
			if(cf->conname!=NULL) Rfree(cf->conname);
		}
		Rfree(holdconfld);
		holdnum=0;
	}
	if(modlstxmtn!=NULL) freeapplib(modlstxmtn);
	if(filelstmtn!=NULL) freeapplib(filelstmtn);
	if(cmodlstmtn!=NULL) freeapplib(cmodlstmtn);
	if(cnfldsmtn!=NULL) freeapplib(cnfldsmtn);
	if(holdapplib!=NULL) freeapplib(holdapplib);
	if(changedsubords) changedvalues=TRUE;
	if(msubsdefaults!=NULL) FreeRDAdefault(msubsdefaults);
}
static void makespacesubord(int pos)
{
	short x,y,z,w=0;
	RDAsubord *temp,*temp1,*new_fields;
	RDAconfld *confld1,*confld2;
	PowerWrite *W1=NULL,*W2=NULL;
	PowerWriteField *WF1=NULL,*WF2=NULL;

	++tmpnumsub;
	new_fields=Rmalloc(tmpnumsub*sizeof(RDAsubord));
	y=0;
	for(x=0,temp=tempsubord;x<(tmpnumsub-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->module=NULL;
			temp1->filename=NULL;
			temp1->num=0;
			temp1->confld=NULL;
			temp1->cmodule=NULL;
			temp1->cfilename=NULL;
			temp1->subkeyname=NULL;
			temp1->expression=NULL;
			temp1->UsePowerWrite=FALSE;
			temp1->numWrites=0;
			temp1->Writes=NULL;
			++y;
			temp1=new_fields+y;
		}
		if(temp->module!=NULL)
		{
			temp1->module=stralloc(temp->module);
			Rfree(temp->module);
		} else temp1->module=NULL;
		if(temp->filename!=NULL)
		{
			temp1->filename=stralloc(temp->filename);
			Rfree(temp->filename);
		} else temp1->filename=NULL;
		temp1->num=temp->num;
		if(temp->confld!=NULL)
		{
			temp1->confld=Rmalloc(temp->num*sizeof(RDAconfld));
			for(z=0,confld1=temp->confld,confld2=temp1->confld;
				z<temp->num;++z,++confld1,++confld2)
			{
				if(confld1->fldname!=NULL)
				{
					confld2->fldname=stralloc(confld1->fldname);
					Rfree(confld1->fldname);
				} else confld2->fldname=NULL;
				if(confld1->conname!=NULL)
				{
					confld2->conname=stralloc(confld1->conname);
					Rfree(confld1->conname);
				} else confld2->conname=NULL;
			}
			Rfree(temp->confld);
		}
		if(temp->cmodule!=NULL)
		{
			temp1->cmodule=stralloc(temp->cmodule);
			Rfree(temp->cmodule);
		} else temp1->cmodule=NULL;
		if(temp->cfilename!=NULL)
		{
			temp1->cfilename=stralloc(temp->cfilename);
			Rfree(temp->cfilename);
		} else temp1->cfilename=NULL;
		if(temp->subkeyname!=NULL)
		{
			temp1->subkeyname=stralloc(temp->subkeyname);
			Rfree(temp->subkeyname);
		} else temp1->subkeyname=NULL;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		temp1->UsePowerWrite=temp->UsePowerWrite;
		if(temp->Writes!=NULL)
		{
			temp1->numWrites=temp->numWrites;
			temp1->Writes=Rmalloc(temp1->numWrites*sizeof(PowerWrite));
			for(z=0,W1=temp->Writes,W2=temp1->Writes;z<temp1->numWrites;
				++z,++W1,++W2)
			{
				if(W1->module!=NULL)
				{
					W2->module=stralloc(W1->module);
					Rfree(W1->module);
				} else W2->module=NULL;
				if(W1->filename!=NULL)
				{
					W2->filename=stralloc(W1->filename);
					Rfree(W1->filename);
				} else W2->filename=NULL;
				if(W1->expression!=NULL)
				{
					W2->expression=stralloc(W1->expression);
					Rfree(W1->expression);
				} else W2->expression=NULL;
				W2->numflds=0;
				W2->fields=NULL;
				if(W1->fields!=NULL)
				{
					W2->numflds=W1->numflds;
					W2->fields=Rmalloc(W2->numflds*sizeof(PowerWriteField));
					for(w=0,WF1=W1->fields,WF2=W2->fields;w<W2->numflds;
						++w,++WF1,++WF2)
					{
						if(WF1->to_name!=NULL)
						{
							WF2->to_name=stralloc(WF1->to_name);
							Rfree(WF1->to_name);
						} else WF2->to_name=NULL;
						WF2->copy_type=WF1->copy_type;
						if(WF1->from_name!=NULL)
						{
							WF2->from_name=stralloc(WF1->from_name);
							Rfree(WF1->from_name);
						} else WF2->from_name=NULL;
					}
					Rfree(W1->fields);
				} else {
					W2->fields=NULL;
					W2->numflds=0;
				}
			}
			Rfree(temp->Writes);
		} else {
			temp1->Writes=NULL;
			temp1->numWrites=0;
		}
		++y;
	}
	Rfree(tempsubord);
	tempsubord=Rmalloc(tmpnumsub*sizeof(RDAsubord));
	for(x=0,temp1=tempsubord,temp=new_fields;x<tmpnumsub;++x,
		++temp,++temp1)
	{
		if(temp->module!=NULL)
		{
			temp1->module=stralloc(temp->module);
			Rfree(temp->module);
		} else temp1->module=NULL;
		if(temp->filename!=NULL)
		{
			temp1->filename=stralloc(temp->filename);
			Rfree(temp->filename);
		} else temp1->filename=NULL;
		if(temp->cmodule!=NULL)
		{
			temp1->cmodule=stralloc(temp->cmodule);
			Rfree(temp->cmodule);
		} else temp1->cmodule=NULL;
		if(temp->cfilename!=NULL)
		{
			temp1->cfilename=stralloc(temp->cfilename);
			Rfree(temp->cfilename);
		} else temp1->cfilename=NULL;
		temp1->num=temp->num;
		if(temp->confld!=NULL)
		{
			temp1->confld=Rmalloc(temp->num*sizeof(RDAconfld));
			for(z=0,confld1=temp->confld,confld2=temp1->confld;
				z<temp->num;++z,++confld1,++confld2)
			{
				if(confld1->fldname!=NULL)
				{
					confld2->fldname=stralloc(confld1->fldname);
					Rfree(confld1->fldname);
				} else confld2->fldname=NULL;
				if(confld1->conname!=NULL)
				{
					confld2->conname=stralloc(confld1->conname);
					Rfree(confld1->conname);
				} else confld2->conname=NULL;
			}
			Rfree(temp->confld);
		}
		if(temp->subkeyname!=NULL)
		{
			temp1->subkeyname=stralloc(temp->subkeyname);
			Rfree(temp->subkeyname);
		} else temp1->subkeyname=NULL;
		if(temp->expression!=NULL)
		{
			temp1->expression=stralloc(temp->expression);
			Rfree(temp->expression);
		} else temp1->expression=NULL;
		temp1->UsePowerWrite=temp->UsePowerWrite;
		if(temp->Writes!=NULL)
		{
			temp1->numWrites=temp->numWrites;
			temp1->Writes=Rmalloc(temp1->numWrites*sizeof(PowerWrite));
			for(z=0,W1=temp->Writes,W2=temp1->Writes;z<temp1->numWrites;
				++z,++W1,++W2)
			{
				if(W1->module!=NULL)
				{
					W2->module=stralloc(W1->module);
					Rfree(W1->module);
				} else W2->module=NULL;
				if(W1->filename!=NULL)
				{
					W2->filename=stralloc(W1->filename);
					Rfree(W1->filename);
				} else W2->filename=NULL;
				if(W1->expression!=NULL)
				{
					W2->expression=stralloc(W1->expression);
					Rfree(W1->expression);
				} else W2->expression=NULL;
				W2->numflds=0;
				W2->fields=NULL;
				if(W1->fields!=NULL)
				{
					W2->numflds=W1->numflds;
					W2->fields=Rmalloc(W2->numflds*sizeof(PowerWriteField));
					for(w=0,WF1=W1->fields,WF2=W2->fields;w<W2->numflds;
						++w,++WF1,++WF2)
					{
						if(WF1->to_name!=NULL)
						{
							WF2->to_name=stralloc(WF1->to_name);
							Rfree(WF1->to_name);
						} else WF2->to_name=NULL;
						WF2->copy_type=WF1->copy_type;
						if(WF1->from_name!=NULL)
						{
							WF2->from_name=stralloc(WF1->from_name);
							Rfree(WF1->from_name);
						} else WF2->from_name=NULL;
					}
					Rfree(W1->fields);
				} else {
					W2->fields=NULL;
					W2->numflds=0;
				}
			}
			Rfree(temp->Writes);
		} else {
			temp1->Writes=NULL;
			temp1->numWrites=0;
		}
	}
	Rfree(new_fields);
}
static void savemsubo(RDArsrc *msuborsrc,RDArsrc *subrsrc)
{
	int value=0,cmod=0,selectedm=0,selectedf=0,selectsubkey=0;
	RDAsubord *subs;
	int x,y;
	char prevconvalid=FALSE;
	RDAconfld *cf1,*cf2;

	if(FINDRSCGETINT(subrsrc,"SUBORDINATE FILES",&value)) return;
	if(tempsubord==NULL)
	{
		tempsubord=Rmalloc(sizeof(RDAsubord));
		subs=tempsubord+value;
		value=0;
		tmpnumsub=1;
		subs->confld=NULL;
		subs->module=NULL;
		subs->filename=NULL;
		subs->subkeyname=NULL;
		subs->expression=NULL;
		subs->cmodule=NULL;
		subs->cfilename=NULL;
		subs->Writes=NULL;
		subs->numWrites=0;
		subs->UsePowerWrite=FALSE;
	}
	subs=tempsubord+value;
	readallwidgets(msuborsrc);
	FINDRSCGETINT(msuborsrc,"MODULE",&selectedm);
	FINDRSCGETINT(msuborsrc,"FILENAME",&selectedf);
	if(subs->module!=NULL) Rfree(subs->module);
	if(subs->filename!=NULL) Rfree(subs->filename);
	subs->module=stralloc(modlstxmtn->libs[selectedm]);
	subs->filename=stralloc(filelstmtn->libs[selectedf]);
	FINDRSCGETINT(msuborsrc,"CONNECT FILENAME",&cmod);
	FINDRSCGETSTRING(msuborsrc,"EXPRESSION",&subs->expression);
	FINDRSCGETINT(msuborsrc,"KEYNAME",&selectsubkey);
	FINDRSCGETCHAR(msuborsrc,"USE POWER WRITE",&subs->UsePowerWrite);
	subs->subkeyname=stralloc(subkeylistmtn->libs[selectsubkey]);
	if(subs->confld!=NULL)
	{
		for(x=0,cf2=subs->confld;x<subs->num;++x,++cf2)
		{
			if(cf2->fldname!=NULL) Rfree(cf2->fldname);
			if(cf2->conname!=NULL) Rfree(cf2->conname);
		} 
		Rfree(subs->confld);
		subs->confld=NULL;
		subs->num=0;
	}
	if(holdconfld!=NULL)
	{
		prevconvalid=FALSE;
		for(y=0,cf1=holdconfld;y<holdnum;++y,++cf1)
		{
			if(cf1->conname==NULL)
			{
				if(prevconvalid)
				{
					break;
				} else {
					ERRORDIALOG("INVALID SUBORDINATE CONNECTION","The key parts in the Subordinate file are not connected correctly to the main file.  Key parts from no. 1 to n (n = some number greater than 1) should be connected to the main file's fields consecutively, in order for the connection to be correctly maintained.  For a more detailed description of how to correctly define this connection see the help.",NULL,FALSE);
					return;
				}
			} else {
			 	prevconvalid=TRUE;
			}
		}
		subs->num=y;
		subs->confld=Rmalloc((y)*sizeof(RDAconfld));
		for(x=0,cf1=holdconfld,cf2=subs->confld;x<y;++x,++cf1,++cf2)
		{
			if(cf1->fldname!=NULL)
			{
				cf2->fldname=stralloc(cf1->fldname);
				Rfree(cf1->fldname);
			} else cf2->fldname=NULL;
			if(cf1->conname!=NULL)
			{
				cf2->conname=stralloc(cf1->conname);
				Rfree(cf1->conname);
			} else cf2->conname=NULL;
		}
		Rfree(holdconfld);
		holdconfld=NULL;
		holdnum=0;
	} 
	savewrt(subs);
	getsubords();
	if(!FINDRSCLISTAPPlib(subrsrc,"SUBORDINATE FILES",value,subavlmtn))
		updatersrc(subrsrc,"SUBORDINATE FILES");
	if(COMPARE_RSRCS(msuborsrc,msubsdefaults,2))
	{
		changedsubords=TRUE;
	}
	quitmsubo(msuborsrc,subrsrc);
}
static void savemsuboabove(RDArsrc *msuborsrc,RDArsrc *subrsrc)
{
	int x;

	if(FINDRSCGETINT(subrsrc,"SUBORDINATE FILES",&x)) return;
	makespacesubord(x);
	savemsubo(msuborsrc,subrsrc);
}
static void savemsubobelow(RDArsrc *msuborsrc,RDArsrc *subrsrc)
{
	int x;

	if(FINDRSCGETINT(subrsrc,"SUBORDINATE FILES",&x)) return;
	makespacesubord(++x);
	FINDRSCSETINT(subrsrc,"SUBORDINATE FILES",x);
	savemsubo(msuborsrc,subrsrc);
}
static void getmodulelist()
{
	int x;
	char *tmp=NULL,*dirx=NULL;

	if(modlstxmtn!=NULL) freeapplib(modlstxmtn);
	modlstxmtn=APPlibNEW();
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
		addAPPlib(modlstxmtn,tmp);
	}
	if(modlstxmtn->numlibs<1)
	{
		addAPPlib(modlstxmtn,"Contains No Database Libraries");
	}
	if(tmp!=NULL) Rfree(tmp);
	if(dirx!=NULL) Rfree(dirx);
	SORTAPPlib(modlstxmtn);
}
static void getconflds()
{
	int x;
	RDAconfld *cf1;
	char *temp=NULL;

	if(cnfldsmtn!=NULL) freeapplib(cnfldsmtn);
	cnfldsmtn=APPlibNEW();
	for(x=0,cf1=holdconfld;x<holdnum;++x,++cf1)
	{
		if(temp!=NULL)
		{
			temp=Rrealloc(temp,RDAstrlen(cf1->fldname)+RDAstrlen(cf1->conname)+29);
		} else {
			temp=Rmalloc(RDAstrlen(cf1->fldname)+RDAstrlen(cf1->conname)+29);
		}
		sprintf(temp,"%2d Field [%s] connects to %s",(x+1),(cf1->fldname!=NULL?cf1->fldname:""),(cf1->conname!=NULL?cf1->conname:""));
		addAPPlib(cnfldsmtn,temp);
	}
	if(cnfldsmtn->numlibs<1)
	{
		addAPPlib(cnfldsmtn,"No Connecting Fields");
	}
}
static void setconflds(RDArsrc *msuborsrc)
{
	int y,z,selectedm=0,selectedf=0,selectedkey=0,selcf=0;
	NRDfield *fields=NULL,*f;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short nofields=0,nokeys=0,engine=0;
	RDAconfld *cf;
	char *modulename=NULL,*filename=NULL;

	if(holdconfld!=NULL)
	{
		for(z=0,cf=holdconfld;z<holdnum;++z,++cf)
		{
			if(cf->fldname!=NULL) Rfree(cf->fldname);
			if(cf->conname!=NULL) Rfree(cf->conname);
		}
		Rfree(holdconfld);
		holdconfld=NULL;
		holdnum=0;
	}
	if(FINDRSCGETINT(msuborsrc,"MODULE",&selectedm)) return;
	if(FINDRSCGETINT(msuborsrc,"FILENAME",&selectedf)) return;
	if(FINDRSCGETINT(msuborsrc,"KEYNAME",&selectedkey)) return;
	if(selectedm>=modlstxmtn->numlibs) 
	{
		selectedm=0;
	}
	if(selectedf>=filelstmtn->numlibs)
	{
		selectedf=0;
	}
	if(!GETBIN(modlstxmtn->libs[selectedm],filelstmtn->libs[selectedf],&engine,&nofields,&fields,&nokeys,&keys)) 
	{
		if(fields!=NULL)
		{
			for(y=0,f=fields;y<nofields;++y,++f)
			{
				if(f->name!=NULL) Rfree(f->name);
			}
			Rfree(fields);
			fields=NULL;
		}
		if(keys!=NULL)
		{
			for(y=0,keyx=keys;y<nokeys;++y,++keyx)
			{
				if(keyx->part!=NULL && !RDAstrcmp(keyx->name,subkeylistmtn->libs[selectedkey]))
				{
					holdnum=keyx->numparts;
					holdconfld=Rmalloc(keyx->numparts*sizeof(RDAconfld));
					for(z=0,cf=holdconfld,part=keyx->part;z<keyx->numparts;
						++z,++cf,++part)
					{
						if(part->name!=NULL) 
						{
							cf->fldname=stralloc(part->name);
							Rfree(part->name);
						} else cf->fldname=NULL;
					}
					Rfree(keyx->part);
				}
				if(keyx->name!=NULL) Rfree(keyx->name);
			}
			Rfree(keys);
			keys=NULL;
		}
		if(holdconfld!=NULL)
		{
			if(FINDRSCGETINT(msuborsrc,"KEYNAME",&selectedkey)) return;
			if(FINDRSCGETINT(msuborsrc,"CONNECT FILENAME",&selcf)) return;
			filename=stripfilename(cmodlstmtn->libs[selcf]);
			if(filename!=NULL) modulename=stripmodulename(cmodlstmtn->libs[selcf]);
			if(modulename!=NULL && filename!=NULL)
			{
				if(!GETBIN(modulename,filename,&engine,&nofields,&fields,&nokeys,&keys)) 
				{
					if(fields!=NULL)
					{
						for(y=0,f=fields;y<nofields;++y,++f)
						{
							for(z=0,cf=holdconfld;z<holdnum;++z,++cf)
							{
								if(!RDAstrcmp(cf->fldname,
									f->name))
								{
									cf->conname=Rmalloc(RDAstrlen(filename)+RDAstrlen(f->name)+5);
									sprintf(cf->conname,"[%s][%s]",filename,f->name);
								}
							}
							if(f->name!=NULL) Rfree(f->name);
						}
						Rfree(fields);
					}
					if(keys!=NULL)
					{
						for(y=0,keyx=keys;y<nokeys;++y,++keyx)
						{
							if(keyx->name!=NULL) Rfree(keyx->name);
							if(keyx->part!=NULL)
							{
								for(z=0,part=keyx->part;z<keyx->numparts;
									++z,++part)
								{
									if(part->name!=NULL) Rfree(part->name);
								}
								Rfree(keyx->part);
							}
						}
						Rfree(keys);
					}
				}
			}
			if(filename!=NULL) Rfree(filename);
			if(modulename!=NULL) Rfree(modulename);
		}
	}
	getconflds();
	if(!FINDRSCLISTAPPlib(msuborsrc,"CONNECT FIELDNAME",0,cnfldsmtn))
		updatersrc(msuborsrc,"CONNECT FIELDNAME");
}
static void quitecon(RDArsrc *econrsrc,RDArsrc *msuborsrc)
{
	if(econrsrc!=NULL)
	{
		killwindow(econrsrc);
		free_rsrc(econrsrc);
	}
	if(econfldsmtn!=NULL) freeapplib(econfldsmtn);
	if(econdefaults!=NULL) FreeRDAdefault(econdefaults);
}
static void saveecon(RDArsrc *econrsrc,RDArsrc *msuborsrc)
{
	int selected=0,selhold=0;
	char *fldname=NULL;
	RDAconfld *c;

	if(FINDRSCGETINT(msuborsrc,"CONNECT FIELDNAME",&selhold)) return;
	if(FINDRSCGETINT(econrsrc,"CONNECT FIELDS",&selected)) return;
	if(FINDRSCGETSTRING(econrsrc,"FIELD NAME",&fldname)) return;
	if(fldname!=NULL)
	{
		if(holdconfld!=NULL)
		{
			c=holdconfld+selhold;
			if(!RDAstrcmp(c->fldname,fldname))
			{
				c->conname=stralloc(econfldsmtn->libs[selected]);
				getconflds();
				if(!FINDRSCLISTAPPlib(msuborsrc,"CONNECT FIELDNAME",
					selhold,cnfldsmtn))
				{
					updatersrc(msuborsrc,"CONNECT FIELDNAME");
				}
			}
		}
	}
	if(COMPARE_RSRCS(econrsrc,econdefaults,2))
	{
		changedsubords=TRUE;
	}
	quitecon(econrsrc,msuborsrc);
}
static void quitecontest(RDArsrc *econrsrc,RDArsrc *msuborsrc)
{
	readallwidgets(econrsrc);
	if(COMPARE_RSRCS(econrsrc,econdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","MASTER CHANGES NOT SAVED!","Changes have been made to this CONNECT FIELD!\nDo you want to Save these changes?",saveecon,quitecon,FALSE,2,econrsrc,msuborsrc,NULL);
	} else {
		quitecon(econrsrc,msuborsrc);
	}
}
static APPlib *makefieldvallist(RDArsrc *subrsrc)
{
#ifdef ADDING_VIRTUALS
	APPlib *virflist=NULL;
	char addvf=FALSE,*libx=NULL;
	int y,z;
	DFincvir *i,*a;
	DFvirtual *d=NULL;
	short dumb=0;
	RDArsrc *tmprsrc=NULL;
#endif /* Adding Virtuals */
	char *curfile=NULL,*curmod=NULL;
	APPlib *tmp=NULL;
	int x;
	RDAsubord *subs;

	tmp=APPlibNEW();
#ifdef ADDING_VIRTUALS
	tmprsrc=RDArsrcNEW(module,"TEMP");
#endif /* Adding Virtuals */
	readwidget(subrsrc,"MAINFILE");
	FINDRSCGETSTRING(subrsrc,"MODULE",&curmod);
	FINDRSCGETSTRING(subrsrc,"MAINFILE",&curfile);
	if(!isEMPTY(curfile) && !isEMPTY(curmod))
	{
#ifdef ADDING_VIRTUALS
		addDFincvir(tmprsrc,curmod,curfile,NULL,dumb);
#endif /* Adding Virtuals */
		addholdvaluessub(tmp,curmod,curfile);
	}
	if(curfile!=NULL) Rfree(curfile);
	for(x=0,subs=tempsubord;x<tmpnumsub;++x,++subs)
	{
#ifdef ADDING_VIRTUALS
		addDFincvir(tmprsrc,subs->module,subs->filename,NULL,dumb);
#endif /* Adding Virtuals */
		addholdvaluessub(tmp,subs->module,subs->filename);
	}
#ifdef ADDING_VIRTUALS
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(curmod)+10);
	sprintf(libx,"%s/rda/%s.VIR",CURRENTDIRECTORY,(curmod!=NULL ? curmod:""));
	if(curmod!=NULL) Rfree(curmod);
	virflist=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	libx=NULL;
	if(virflist!=NULL)
	{
		for(y=0;y<virflist->numlibs;++y)
		{
			d=getDFvirtual(module,virflist->libs[y]);
			if(d!=NULL)
			{
				addvf=TRUE;
				if(d->incvir!=NULL)
				{
					for(z=0,a=d->incvir;z<d->num;++z,++a)
					{
						for(x=0,i=tmprsrc->incvir;x<tmprsrc->numvir;++x,++i)
						{
							if(!RDAstrcmp(i->module,a->module) && 
								!RDAstrcmp(i->file,a->file)) break;
						}
						if(x>=tmprsrc->numvir) 
						{
							addvf=FALSE;
							break;
						}
					}
				}
				if(addvf)
				{
					libx=Rmalloc(RDAstrlen(d->name)+3);
					sprintf(libx,"[%s]",d->name);
					addAPPlibNoDuplicates(tmp,libx);
					if(libx!=NULL) Rfree(libx);
				}
				FreeDFvirtual(d);
			}
		}
	}
	if(virflist!=NULL) freeapplib(virflist);
	if(libx!=NULL) Rfree(libx);
	loadglobals(tmp);
	if(tmprsrc!=NULL) free_rsrc(tmprsrc);
#endif /* Adding Virtuals */
	return(tmp);
}
void setconfldlist(RDArsrc *rsrc,APPlib *list,char *fieldvalue)
{
	int x=(-1);
	char *warndesc=NULL;

	if(!isEMPTY(fieldvalue))
	{
		if((x=FINDAPPLIBELEMENT(list,fieldvalue))==(-1))
		{
			warndesc=Rmalloc(RDAstrlen(fieldvalue)+250);
			sprintf(warndesc,"The value of [%s] is an Invalid Virtual/File Field Value.  It doesn't exist in this report's Virtual Fields or the connected files.  The List for this field is being set to the value [%s].",(fieldvalue==NULL?"":fieldvalue),list->libs[
0]);
			WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID FIELD VALUE!",warndesc,NULL,NULL,FALSE,NULL);
			Rfree(warndesc);
			x=0;
		}
		FINDRSCSETINT(rsrc,"CONNECT FIELDS",x);
		updatersrc(rsrc,"CONNECT FIELDS");
	}
}
static void editconnect(RDArsrc *msuborsrc,RDArsrc *subrsrc)
{
	int x,selectfld=0;
	char *fldname=NULL,*cfldname=NULL;
	RDAconfld *c;
	RDArsrc *econrsrc=NULL;

	readallwidgets(msuborsrc);
	if(FINDRSCGETINT(msuborsrc,"CONNECT FIELDNAME",&selectfld)) return;
	if(holdconfld!=NULL && selectfld<holdnum)
	{
		c=holdconfld+selectfld;
		fldname=stralloc(c->fldname);
		cfldname=stralloc(c->conname);
	} 
	econrsrc=RDArsrcNEW(module,"EDIT SUBORDINATE CONNECT FIELD");
	addstdrsrc(econrsrc,"FIELD NAME",VARIABLETEXT,0,
		(fldname!=NULL ? fldname:""),FALSE);
	if(fldname!=NULL) Rfree(fldname);
	if(econfldsmtn!=NULL) freeapplib(econfldsmtn);
	econfldsmtn=APPlibNEW();
	x=0;
	econfldsmtn=makefieldvallist(subrsrc);
	if(econfldsmtn->numlibs<1)
	{
		addAPPlib(econfldsmtn,"No Connecting Fields Available");
	}
	addlstrsrc(econrsrc,"CONNECT FIELDS",&x,TRUE,NULL,econfldsmtn->numlibs,
		&econfldsmtn->libs,NULL);
	addrfcbrsrc(econrsrc,"SAVE",TRUE,saveecon,msuborsrc);
	addrfcbrsrc(econrsrc,"QUIT",TRUE,quitecontest,msuborsrc);
	addbtnrsrc(econrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(econrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	APPmakescrn(econrsrc,TRUE,quitecon,msuborsrc,FALSE);
	setconfldlist(econrsrc,econfldsmtn,cfldname);
	if(cfldname!=NULL) Rfree(cfldname);
	econdefaults=GetDefaults(econrsrc);
}
static void dofilelist(RDArsrc *msuborsrc,int *passfunc)
{
	int y,z,selectedm,selectedf;
	NRDfield *fields=NULL,*f;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short nofields=0,nokeys=0,engine=0;

	if(FINDRSCGETINT(msuborsrc,"MODULE",&selectedm)) return;
	if(FINDRSCGETINT(msuborsrc,"FILENAME",&selectedf)) return;
	if(subkeylistmtn!=NULL) freeapplib(subkeylistmtn);
	subkeylistmtn=APPlibNEW();
	if(selectedm>=modlstxmtn->numlibs) 
	{
		selectedm=0;
	}
	if(selectedf>=filelstmtn->numlibs)
	{
		selectedf=0;
	}
	if(!GETBIN(modlstxmtn->libs[selectedm],filelstmtn->libs[selectedf],&engine,&nofields,&fields,&nokeys,&keys)) 
	{
		if(fields!=NULL)
		{
			for(y=0,f=fields;y<nofields;++y,++f)
			{
				if(f->name!=NULL) Rfree(f->name);
			}
			Rfree(fields);
		}
		if(keys!=NULL)
		{
			for(y=0,keyx=keys;y<nokeys;++y,++keyx)
			{
				addAPPlib(subkeylistmtn,keyx->name);
				if(keyx->name!=NULL) Rfree(keyx->name);
				if(keyx->part!=NULL)
				{
					for(z=0,part=keyx->part;z<keyx->numparts;++z,++part)
					{
						if(part->name!=NULL) Rfree(part->name);
					}
					Rfree(keyx->part);
				}
			}
			Rfree(keys);
		}
	}
	if(subkeylistmtn->numlibs<1)
	{
		addAPPlib(subkeylistmtn,"No Keys Available");
	}
	if(!FINDRSCLISTAPPlib(msuborsrc,"KEYNAME",0,subkeylistmtn))
	{
		updatersrc(msuborsrc,"KEYNAME");
		if(*passfunc) setconflds(msuborsrc);
	}
}
static void domodulelist(RDArsrc *msuborsrc,int *passfunc)
{
	int selected=0,selectedf=0;
	char *libx=NULL;

	readwidget(msuborsrc,"MODULE");
	readwidget(msuborsrc,"FILENAME");
	if(FINDRSCGETINT(msuborsrc,"MODULE",&selected)) return;
	if(FINDRSCGETINT(msuborsrc,"FILENAME",&selectedf)) return;
	if(filelstmtn!=NULL) freeapplib(filelstmtn);
	if(selected>=modlstxmtn->numlibs) 
	{
		selected=0;
	}
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(modlstxmtn->libs[selected])+10);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.FIL",CURRENTDIRECTORY,modlstxmtn->libs[selected]);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.FIL",CURRENTDIRECTORY,modlstxmtn->libs[selected]);
#endif
	filelstmtn=getlibnames(libx,TRUE);
	if(libx!=NULL) Rfree(libx);
	if(filelstmtn==NULL)
	{
		filelstmtn=APPlibNEW();
		addAPPlib(filelstmtn,"Contains No File Definitions");
	}
	if(selectedf>=filelstmtn->numlibs) selectedf=0;
	if(!FINDRSCLISTAPPlib(msuborsrc,"FILENAME",selectedf,filelstmtn))
	{
		updatersrc(msuborsrc,"FILENAME");
		dofilelist(msuborsrc,passfunc);
	}
}
static void set_pwrt(RDArsrc *r)
{
	char t=FALSE;

	readwidget(r,"USE POWER WRITE");
	FINDRSCGETCHAR(r,"USE POWER WRITE",&t);
	if(t==FALSE)
	{
		FINDRSCSETEDITABLE(r,"WRITE LIST",FALSE);
		FINDRSCSETEDITABLE(r,"ADD WRITE ABOVE",FALSE);
		FINDRSCSETEDITABLE(r,"ADD WRITE BELOW",FALSE);
		FINDRSCSETEDITABLE(r,"SELECT WRITE",FALSE);
		FINDRSCSETEDITABLE(r,"COPY WRITE",FALSE);
		FINDRSCSETEDITABLE(r,"DELETE WRITE",FALSE);
		FINDRSCSETSENSITIVE(r,"WRITE LIST",FALSE);
		FINDRSCSETSENSITIVE(r,"ADD WRITE ABOVE",FALSE);
		FINDRSCSETSENSITIVE(r,"ADD WRITE BELOW",FALSE);
		FINDRSCSETSENSITIVE(r,"SELECT WRITE",FALSE);
		FINDRSCSETSENSITIVE(r,"COPY WRITE",FALSE);
		FINDRSCSETSENSITIVE(r,"DELETE WRITE",FALSE);
	} else {
		FINDRSCSETSENSITIVE(r,"WRITE LIST",TRUE);
		FINDRSCSETSENSITIVE(r,"ADD WRITE ABOVE",TRUE);
		FINDRSCSETSENSITIVE(r,"ADD WRITE BELOW",TRUE);
		FINDRSCSETSENSITIVE(r,"SELECT WRITE",TRUE);
		FINDRSCSETSENSITIVE(r,"COPY WRITE",TRUE);
		FINDRSCSETSENSITIVE(r,"DELETE WRITE",TRUE);
		FINDRSCSETEDITABLE(r,"WRITE LIST",TRUE);
		FINDRSCSETEDITABLE(r,"ADD WRITE ABOVE",TRUE);
		FINDRSCSETEDITABLE(r,"ADD WRITE BELOW",TRUE);
		FINDRSCSETEDITABLE(r,"SELECT WRITE",TRUE);
		FINDRSCSETEDITABLE(r,"COPY WRITE",TRUE);
		FINDRSCSETEDITABLE(r,"DELETE WRITE",TRUE);
	}
}
static void subordscreen(RDArsrc *subrsrc,int num,
	char *modulex,char *filename,char *cmodule,char *cfilename,
	char *subkeyname,int number,char *expression,RDAconfld *confld,
	char UsePowerWrite,int numWrites,PowerWrite *Writes,
	void (*savefunc)(...),void (*quitfunctest)(...))
{
	int x=0,y=0,selmod=0,selfile=0;
	char *temp=NULL,*tmod=NULL,*curfile=NULL,*curmod=NULL;
/*
	RDAsubord *subs;
*/
	RDArsrc *msuborsrc=NULL;
	RDAconfld *cf,*cf1;
	NRDfield *fields=NULL,*f;
	NRDkey *keys=NULL,*keyx;
	NRDpart *part;
	short nofields=0,nokeys=0,engine=0;
	PowerWrite *W1=NULL,*W2=NULL;
	PowerWriteField *WF1=NULL,*WF2=NULL;

	if(Writes!=NULL && numWrites>0)
	{
		tempSubWrites=Rmalloc(numWrites*sizeof(PowerWrite));
		tmpnumSubWrites=numWrites;
		for(x=0,W1=Writes,W2=tempSubWrites;x<numWrites;++x,++W1,++W2)
		{
			if(W1->module!=NULL) W2->module=stralloc(W1->module);
				else W2->module=NULL;
			if(W1->filename!=NULL) W2->filename=stralloc(W1->filename);
				else W2->filename=NULL;
			if(W1->expression!=NULL) W2->expression=stralloc(W1->expression);
				else W2->expression=NULL;
			W2->numflds=0;
			W2->fields=NULL;
			if(W1->fields!=NULL && W1->numflds>0)
			{
				W2->numflds=W1->numflds;
				W2->fields=Rmalloc(W2->numflds*sizeof(PowerWriteField));
				for(y=0,WF1=W1->fields,WF2=W2->fields;y<W1->numflds;
					++y,++WF1,++WF2)
				{
					if(WF1->to_name!=NULL) WF2->to_name=stralloc(WF1->to_name);
						else WF2->to_name=NULL;
					WF2->copy_type=WF1->copy_type;
					if(WF1->from_name!=NULL) 
					{
						WF2->from_name=stralloc(WF1->from_name);
					} else {
						WF2->from_name=NULL;
					}
				}
			} else {
				W2->fields=NULL;
				W2->numflds=0;
			}
		}
	} else {
		tempSubWrites=NULL;
		tmpnumSubWrites=0;
	}
	msuborsrc=RDArsrcNEW(module,"MAINTAIN SUBORDINATE FILE");
	addstdrsrc(msuborsrc,"NUMBER",LONGV,8,&num,FALSE);
	getmodulelist();
	if(modulex!=NULL)
	{
		for(x=0;x<modlstxmtn->numlibs;++x)
		{
			if(!RDAstrcmp(modlstxmtn->libs[x],modulex)) break;
		}
	}
	if(x>=modlstxmtn->numlibs)
	{
		FINDRSCGETSTRING(mtnrsrc,"MODULE",&tmod);
		for(x=0;x<modlstxmtn->numlibs;++x)
		{
			if(!RDAstrcmp(modlstxmtn->libs[x],tmod)) break;
		}
		if(tmod!=NULL) Rfree(tmod);
		if(x>=modlstxmtn->numlibs) x=0;
	}
	addlstrsrc(msuborsrc,"MODULE",&x,TRUE,domodulelist,modlstxmtn->numlibs,&modlstxmtn->libs,&ptrue);
	if(filelstmtn!=NULL) freeapplib(filelstmtn);
	filelstmtn=APPlibNEW();
	addlstrsrc(msuborsrc,"FILENAME",&x,TRUE,dofilelist,filelstmtn->numlibs,&filelstmtn->libs,&ptrue);
	if(cmodlstmtn!=NULL) freeapplib(cmodlstmtn);
	cmodlstmtn=APPlibNEW();
	readwidget(subrsrc,"MAINFILE");
	FINDRSCGETSTRING(subrsrc,"MAINFILE",&curfile);
	if(!RDAstrcmp(curfile,"SORTFILE"))
	{
		curmod=stralloc("RPTGEN");
	} else {
		readwidget(subrsrc,"MODULE");
		FINDRSCGETSTRING(subrsrc,"MODULE",&curmod);
	}
	if(!isEMPTY(curfile) && !isEMPTY(curmod))
	{
		if(temp!=NULL)
		{
			temp=Rrealloc(temp,RDAstrlen(curmod)+RDAstrlen(curfile)+5);
		} else {
			temp=Rmalloc(RDAstrlen(curmod)+RDAstrlen(curfile)+5);
		}
		sprintf(temp,"[%s][%s]",curmod,curfile);
		addAPPlib(cmodlstmtn,temp);
	}
	if(curmod!=NULL) Rfree(curmod);
	if(curfile!=NULL) Rfree(curfile);
/*
	if(num>1)
	{
		for(x=0,subs=tempsubord;x<(num-1);++x,++subs)
		{
			if(temp!=NULL)
			{
				temp=Rrealloc(temp,RDAstrlen(subs->module)+RDAstrlen(subs->filename)+5);
			} else {
				temp=Rmalloc(RDAstrlen(subs->module)+RDAstrlen(subs->filename)+5);
			}
			sprintf(temp,"[%s][%s]",(subs->module!=NULL ? subs->module:""),
				(subs->filename!=NULL ? subs->filename:""));
			addAPPlib(cmodlstmtn,temp);
		}
	}
	if(temp!=NULL) Rfree(temp);
*/
	if(cmodlstmtn->numlibs<1)
	{
		addAPPlib(cmodlstmtn,"No Files Available");
	}
/*
	for(x=0,subs=tempsubord;x<(num-1);++x,++subs)
	{
		if(!RDAstrcmp(subs->module,cmodule) && !RDAstrcmp(subs->filename,cfilename)) break;
	}
	if(x>=(num-1)) x=0;
*/
	x=0; /* This line may need to be removed if commented out stuff above is put back in.  It assumes there's only one connect filename.  BLD */
	addlstrsrc(msuborsrc,"CONNECT FILENAME",&x,TRUE,setconflds,cmodlstmtn->numlibs,
		&cmodlstmtn->libs,NULL);
	if(cnfldsmtn!=NULL) freeapplib(cnfldsmtn);
	cnfldsmtn=APPlibNEW();
	if(confld!=NULL && number)
	{
		holdnum=number;
		holdconfld=Rmalloc(number*sizeof(RDAconfld));
		for(x=0,cf1=holdconfld,cf=confld;x<number;++x,++cf,++cf1)
		{
			if(temp!=NULL)
			{
				temp=Rrealloc(temp,RDAstrlen(cf->fldname)+RDAstrlen(cf->conname)+29);
			} else {
				temp=Rmalloc(RDAstrlen(cf->fldname)+RDAstrlen(cf->conname)+29);
			}
			cf1->fldname=stralloc(cf->fldname);
			cf1->conname=stralloc(cf->conname);
			sprintf(temp,"%2d Field [%s] connects to %s",x+1,(cf->fldname!=NULL ? cf->fldname:""),(cf->conname!=NULL ? cf->conname:""));
			addAPPlib(cnfldsmtn,temp);
		}
	}
	if(subkeylistmtn!=NULL) freeapplib(subkeylistmtn);
	subkeylistmtn=APPlibNEW();
	x=0;
	addlstrsrc(msuborsrc,"KEYNAME",&x,TRUE,setconflds,subkeylistmtn->numlibs,
		&subkeylistmtn->libs,NULL);
	selmod=0;
	domodulelist(msuborsrc,&pfalse);
	if(modulex!=NULL)
	{
		for(selmod=0;selmod<modlstxmtn->numlibs;++selmod)
		{
			if(!RDAstrcmp(modlstxmtn->libs[selmod],modulex)) break;
		}
	}
	if(selmod>=modlstxmtn->numlibs)
	{
		FINDRSCGETSTRING(mtnrsrc,"MODULE",&tmod);
		for(selmod=0;selmod<modlstxmtn->numlibs;++selmod)
		{
			if(!RDAstrcmp(modlstxmtn->libs[selmod],tmod)) break;
		}
		if(tmod!=NULL) Rfree(tmod);
		if(selmod>=modlstxmtn->numlibs) selmod=0;
	}
	dofilelist(msuborsrc,&pfalse); 
	selfile=0;
	if(filename!=NULL)
	{
		for(selfile=0;selfile<filelstmtn->numlibs;++selfile)
		{
			if(!RDAstrcmp(filelstmtn->libs[selfile],filename)) break;
		}
	}
	if(selfile>=filelstmtn->numlibs)
	{
		selfile=0;
	}
	if(!FINDRSCLISTAPPlib(msuborsrc,"FILENAME",selfile,filelstmtn))
	{
		updatersrc(msuborsrc,"FILENAME");
	}
	if(!GETBIN(modlstxmtn->libs[selmod],filelstmtn->libs[selfile],&engine,&nofields,&fields,&nokeys,&keys)) 
	{
		if(fields!=NULL)
		{
			for(x=0,f=fields;x<nofields;++x,++f)
			{
				if(f->name!=NULL) Rfree(f->name);
			}
			Rfree(fields);
		}
		if(keys!=NULL)
		{
			for(x=0,keyx=keys;x<nokeys;++x,++keyx)
			{
				addAPPlib(subkeylistmtn,keyx->name);
				if(keyx->name!=NULL) Rfree(keyx->name);
				if(keyx->part!=NULL)
				{
					for(y=0,part=keyx->part;y<keyx->numparts;++y,++part)
					{
						if(part->name!=NULL) Rfree(part->name);
					}
					Rfree(keyx->part);
				}
			}
			Rfree(keys);
		}
	}
	if(subkeylistmtn->numlibs<1)
	{
		addAPPlib(subkeylistmtn,"No Keys Available");
	}
	if(subkeyname!=NULL)
	{
		for(x=0;x<subkeylistmtn->numlibs;++x)
		{
			if(!RDAstrcmp(subkeylistmtn->libs[x],subkeyname)) break;
		}
	}
	if(x>=subkeylistmtn->numlibs)
	{
		x=0;
	}
	if(!FINDRSCLISTAPPlib(msuborsrc,"KEYNAME",x,subkeylistmtn))
	{
		updatersrc(msuborsrc,"KEYNAME");
	}
	if(cnfldsmtn->numlibs<1)
	{
		addAPPlib(cnfldsmtn,"No Connecting Fields");
		setconflds(msuborsrc);
	}
	x=0;
	addlstrsrc(msuborsrc,"CONNECT FIELDNAME",&x,TRUE,NULL,cnfldsmtn->numlibs,
		&cnfldsmtn->libs,NULL);
	addbtnrsrc(msuborsrc,"SELECT",TRUE,editconnect,subrsrc);
	addsctrsrc(msuborsrc,"EXPRESSION",0,expression,TRUE);
	addbtnrsrc(msuborsrc,"LOAD VALUE",TRUE,loadvalue2mtn,"EXPRESSION");
	addbtnrsrc(msuborsrc,"LOAD OPERATOR",TRUE,loadoperand,"EXPRESSION");
	addbtnrsrc(msuborsrc,"LOAD GROUPER",TRUE,loadgrouper,"EXPRESSION");
	addbtnrsrc(msuborsrc,"LOAD CONTROL",TRUE,loadcontrol,"EXPRESSION");

	addstdrsrc(msuborsrc,"USE POWER WRITE",BOOLNS,1,&UsePowerWrite,TRUE);
	FINDRSCSETFUNC(msuborsrc,"USE POWER WRITE",set_pwrt,NULL);
	if(subwriteavl!=NULL) freeapplib(subwriteavl);
	subwriteavl=APPlibNEW();
	getSubWrites();
	addlstrsrc(msuborsrc,"WRITE LIST",0,TRUE,NULL,subwriteavl->numlibs,&subwriteavl->libs,NULL);
	addbtnrsrc(msuborsrc,"ADD WRITE ABOVE",TRUE,wrtaddabove,NULL);
	addbtnrsrc(msuborsrc,"ADD WRITE BELOW",TRUE,wrtaddbelow,NULL);
	addbtnrsrc(msuborsrc,"DELETE WRITE",TRUE,wrtdelete,NULL);
	addbtnrsrc(msuborsrc,"COPY WRITE",TRUE,wrtcopy,NULL);
	addbtnrsrc(msuborsrc,"SELECT WRITE",TRUE,wrtedit,NULL);
	set_pwrt(msuborsrc);
	addrfcbrsrc(msuborsrc,"SAVE",TRUE,savefunc,subrsrc);
	addrfcbrsrc(msuborsrc,"QUIT",TRUE,quitfunctest,subrsrc);
	addbtnrsrc(msuborsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(msuborsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	if(temp!=NULL) Rfree(temp);
	msubsdefaults=GetDefaults(msuborsrc);
	changedsubords=FALSE;
	APPmakescrn(msuborsrc,TRUE,quitmsubo,subrsrc,FALSE);
}
static void quitmsubotest(RDArsrc *msuborsrc,RDArsrc *subrsrc)
{
	readallwidgets(msuborsrc);
	if(COMPARE_RSRCS(msuborsrc,msubsdefaults,2) || changedsubords)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","MASTER CHANGES NOT SAVED!","Changes have been made to this SUBORDINATE FILE DEFININTION!\nDo you want to Save these changes?",savemsubo,quitmsubo,FALSE,2,msuborsrc,subrsrc,NULL);
	} else {
		quitmsubo(msuborsrc,subrsrc);
	}
}
static void quitmsuboabovetest(RDArsrc *msuborsrc,RDArsrc *subrsrc)
{
	readallwidgets(msuborsrc);
	if(COMPARE_RSRCS(msuborsrc,msubsdefaults,2) || changedsubords)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","MASTER CHANGES NOT SAVED!","Changes have been made to this SURORDINATE FILE DEFININTION!\nDo you want to Save these changes?",savemsuboabove,quitmsubo,FALSE,2,msuborsrc,subrsrc,NULL);
	} else {
		quitmsubo(msuborsrc,subrsrc);
	}
}
static void quitmsubobelowtest(RDArsrc *msuborsrc,RDArsrc *subrsrc)
{
	readallwidgets(msuborsrc);
	if(COMPARE_RSRCS(msuborsrc,msubsdefaults,2) || changedsubords)
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","MASTER CHANGES NOT SAVED!","Changes have been made to this SUBORDINATE FILE DEFININTION!\nDo you want to Save these changes?",savemsubobelow,quitmsubo,FALSE,2,msuborsrc,subrsrc,NULL);
	} else {
		quitmsubo(msuborsrc,subrsrc);
	}
}
void subaddabove(RDArsrc *subrsrc,RDArsrc *mtnrsrc)
{
	int selected=0;
	char *modx=NULL;

	if(FINDRSCGETINT(subrsrc,"SUBORDINATE FILES",&selected)) return;
	if(selected==0 && !RDAstrcmp(subavlmtn->libs[selected],"No Files Defined"))
		return;
	readwidget(mtnrsrc,"MODULE");
	FINDRSCGETSTRING(mtnrsrc,"MODULE",&modx);
	subordscreen(subrsrc,++selected,modx,NULL,NULL,NULL,NULL,0,
		NULL,NULL,FALSE,0,NULL,savemsuboabove,quitmsuboabovetest);
	if(modx!=NULL) Rfree(modx);
}
void subaddbelow(RDArsrc *subrsrc,RDArsrc *mtnrsrc)
{
	int selected=0;
	char *modx=NULL;

	if(FINDRSCGETINT(subrsrc,"SUBORDINATE FILES",&selected)) return;
	if(selected==0 && !RDAstrcmp(subavlmtn->libs[selected],"No Files Defined"))
		return;
	selected+=2;
	readwidget(mtnrsrc,"MODULE");
	FINDRSCGETSTRING(mtnrsrc,"MODULE",&modx);
	subordscreen(subrsrc,selected,modx,NULL,NULL,NULL,NULL,0,
		NULL,NULL,FALSE,0,NULL,savemsubobelow,quitmsubobelowtest);
	if(modx!=NULL) Rfree(modx);
}
void subdelete(RDArsrc *subrsrc)
{
	short x,y,z;
	RDAsubord *temp,*temp1,*new_fields;
	int value,w=0;
	RDAconfld *confld1,*confld2;
	PowerWrite *W1=NULL,*W2=NULL;
	PowerWriteField *WF1=NULL,*WF2=NULL;

	readallwidgets(subrsrc);
	if(FINDRSCGETINT(subrsrc,"SUBORDINATE FILES",&value)) return;
	if(tmpnumsub<1) return;
	if(tmpnumsub>1)
	{
		new_fields=Rmalloc((tmpnumsub-1)*sizeof(RDAsubord));
		y=0;
		for(x=0,temp=tempsubord;x<tmpnumsub;++x,++temp)
		{
			temp1=new_fields+y;
			if(x!=value)
			{
				if(temp->module!=NULL)
				{
					temp1->module=stralloc(temp->module);
					Rfree(temp->module);
				} else temp1->module=NULL;
				if(temp->filename!=NULL)
				{
					temp1->filename=stralloc(temp->filename);
					Rfree(temp->filename);
				} else temp1->filename=NULL;
				temp1->num=temp->num;
				if(temp->confld!=NULL)
				{
					temp1->confld=Rmalloc(temp->num*sizeof(RDAconfld));
					for(z=0,confld1=temp->confld,confld2=temp1->confld;
						z<temp->num;++z,++confld1,++confld2)
					{
						if(confld1->fldname!=NULL)
						{
							confld2->fldname=stralloc(confld1->fldname);
							Rfree(confld1->fldname);
						} else confld2->fldname=NULL;
						if(confld1->conname!=NULL)
						{
							confld2->conname=stralloc(confld1->conname);
							Rfree(confld1->conname);
						} else confld2->conname=NULL;
					}
					Rfree(temp->confld);
				} else temp1->confld=NULL;
				if(temp->cmodule!=NULL)
				{
					temp1->cmodule=stralloc(temp->cmodule);
					Rfree(temp->cmodule);
				} else temp1->cmodule=NULL;
				if(temp->cfilename!=NULL)
				{
					temp1->cfilename=stralloc(temp->cfilename);
					Rfree(temp->cfilename);
				} else temp1->cfilename=NULL;
				if(temp->subkeyname!=NULL)
				{
					temp1->subkeyname=stralloc(temp->subkeyname);
					Rfree(temp->subkeyname);
				} else temp1->subkeyname=NULL;
				if(temp->expression!=NULL)
				{
					temp1->expression=stralloc(temp->expression);
					Rfree(temp->expression);
				} else temp1->expression=NULL;
				temp1->UsePowerWrite=temp->UsePowerWrite;
				temp1->numWrites=0;
				temp1->Writes=NULL;
				if(temp->Writes!=NULL && 
					temp->numWrites>0)
				{
					temp1->numWrites=temp->numWrites;
					temp1->Writes=Rmalloc(temp1->numWrites*sizeof(PowerWrite));
					for(z=0,W1=temp->Writes,W2=temp1->Writes;z<temp->numWrites;
						++z,++W1,++W2)
					{
						if(W1->module!=NULL)
						{
							W2->module=stralloc(W1->module);
							Rfree(W1->module);
						} else W2->module=NULL;
						if(W1->filename!=NULL)
						{
							W2->filename=stralloc(W1->filename);
							Rfree(W1->filename);
						} else W2->filename=NULL;
						if(W1->expression!=NULL)
						{
							W2->expression=stralloc(W1->expression);
							Rfree(W1->expression);
						} else W2->expression=NULL;
						W2->numflds=0;
						W2->fields=NULL;
						if(W1->fields!=NULL)
						{
							W2->numflds=W1->numflds;
							W2->fields=Rmalloc(W2->numflds*sizeof(PowerWriteField));
							for(w=0,WF1=W1->fields,WF2=W2->fields;w<W2->numflds;
								++w,++WF1,++WF2)
							{
								if(WF1->to_name!=NULL)
								{
									WF2->to_name=stralloc(WF1->to_name);
									Rfree(WF1->to_name);
								} else WF2->to_name=NULL;
								WF2->copy_type=WF1->copy_type;
								if(WF1->from_name!=NULL)
								{
									WF2->from_name=stralloc(WF1->from_name);
									Rfree(WF1->from_name);
								} else WF2->from_name=NULL;
							}
							Rfree(W1->fields);
						} else {
							W2->fields=NULL;
							W2->numflds=0;
						}
					}
					Rfree(temp->Writes);
					temp->numWrites=0;
				}
				++y;
			} else {
				if(temp->module!=NULL) Rfree(temp->module);
				if(temp->filename!=NULL) Rfree(temp->filename);
				if(temp->confld!=NULL)
				{
					for(z=0,confld1=temp->confld;z<temp->num;++z,++confld1)
					{
						if(confld1->fldname!=NULL) Rfree(confld1->fldname);
						if(confld1->conname!=NULL) Rfree(confld1->conname);
					}
					Rfree(temp->confld);
				}
				if(temp->cmodule!=NULL) Rfree(temp->cmodule);
				if(temp->cfilename!=NULL) Rfree(temp->cfilename);
				if(temp->subkeyname!=NULL) Rfree(temp->subkeyname);
				if(temp->expression!=NULL) Rfree(temp->expression);
				if(temp->Writes!=NULL && 
					temp->numWrites>0)
				{
					for(z=0,W1=temp->Writes;z<temp->numWrites;
						++z,++W1)
					{
						if(W1->module!=NULL)
							Rfree(W1->module);
						if(W1->filename!=NULL)
							Rfree(W1->filename);
						if(W1->expression!=NULL)
							Rfree(W1->expression);
						if(W1->fields!=NULL)
						{
							for(w=0,WF1=W1->fields;w<W1->numflds;++w,++WF1)
							{
								if(WF1->to_name!=NULL)
									Rfree(WF1->to_name);
								if(WF1->from_name!=NULL)
									Rfree(WF1->from_name);
							}
							Rfree(W1->fields);
						}
					}
					Rfree(temp->Writes);
					temp->numWrites=0;
				}
			}	
		}
		Rfree(tempsubord);
		--tmpnumsub;
		tempsubord=Rmalloc(tmpnumsub*sizeof(RDAsubord));
		for(x=0,temp1=tempsubord,temp=new_fields;x<tmpnumsub;++x,
			++temp,++temp1)
		{
			if(temp->module!=NULL)
			{
				temp1->module=stralloc(temp->module);
				Rfree(temp->module);
			} else temp1->module=NULL;
			if(temp->filename!=NULL)
			{
				temp1->filename=stralloc(temp->filename);
				Rfree(temp->filename);
			} else temp1->filename=NULL;
			temp1->num=temp->num;
			if(temp->confld!=NULL)
			{
				temp1->confld=Rmalloc(temp->num*sizeof(RDAconfld));
				for(z=0,confld1=temp->confld,confld2=temp1->confld;
					z<temp->num;++z,++confld1,++confld2)
				{
					if(confld1->fldname!=NULL)
					{
						confld2->fldname=stralloc(confld1->fldname);
						Rfree(confld1->fldname);
					} else confld2->fldname=NULL;
					if(confld1->conname!=NULL)
					{
						confld2->conname=stralloc(confld1->conname);
						Rfree(confld1->conname);
					} else confld2->conname=NULL;
				}
				Rfree(temp->confld);
			} else temp1->confld=NULL;
			if(temp->cmodule!=NULL)
			{
				temp1->cmodule=stralloc(temp->cmodule);
				Rfree(temp->cmodule);
			} else temp1->cmodule=NULL;
			if(temp->cfilename!=NULL)
			{
				temp1->cfilename=stralloc(temp->cfilename);
				Rfree(temp->cfilename);
			} else temp1->cfilename=NULL;
			if(temp->subkeyname!=NULL)
			{
				temp1->subkeyname=stralloc(temp->subkeyname);
				Rfree(temp->subkeyname);
			} else temp1->subkeyname=NULL;
			if(temp->expression!=NULL)
			{
				temp1->expression=stralloc(temp->expression);
				Rfree(temp->expression);
			} else temp1->expression=NULL;
			temp1->UsePowerWrite=temp->UsePowerWrite;
			temp1->numWrites=0;
			temp1->Writes=NULL;
			if(temp->Writes!=NULL && 
				temp->numWrites>0)
			{
				temp1->numWrites=temp->numWrites;
				temp1->Writes=Rmalloc(temp1->numWrites*sizeof(PowerWrite));
				for(z=0,W1=temp->Writes,W2=temp1->Writes;z<temp->numWrites;
					++z,++W1,++W2)
				{
					if(W1->module!=NULL)
					{
						W2->module=stralloc(W1->module);
						Rfree(W1->module);
					} else W2->module=NULL;
					if(W1->filename!=NULL)
					{
						W2->filename=stralloc(W1->filename);
						Rfree(W1->filename);
					} else W2->filename=NULL;
					if(W1->expression!=NULL)
					{
						W2->expression=stralloc(W1->expression);
						Rfree(W1->expression);
					} else W2->expression=NULL;
					W2->numflds=0;
					W2->fields=NULL;
					if(W1->fields!=NULL)
					{
						W2->numflds=W1->numflds;
						W2->fields=Rmalloc(W2->numflds*sizeof(PowerWriteField));
						for(y=0,WF1=W1->fields,WF2=W2->fields;y<W2->numflds;
							++y,++WF1,++WF2)
						{
							if(WF1->to_name!=NULL)
							{
								WF2->to_name=stralloc(WF1->to_name);
								Rfree(WF1->to_name);
							} else WF2->to_name=NULL;
							WF2->copy_type=WF1->copy_type;
							if(WF1->from_name!=NULL)
							{
								WF2->from_name=stralloc(WF1->from_name);
								Rfree(WF1->from_name);
							} else WF2->from_name=NULL;
						}
						Rfree(W1->fields);
					} else {
						W2->fields=NULL;
						W2->numflds=0;
					}
				}
				Rfree(temp->Writes);
				temp->numWrites=0;
			}
		}
		Rfree(new_fields);
	} else if(tempsubord!=NULL)
	{
		for(x=0,temp=tempsubord;x<tmpnumsub;++x,++temp)
		{
			if(temp->module!=NULL) Rfree(temp->module);
			if(temp->filename!=NULL) Rfree(temp->filename);
			if(temp->cmodule!=NULL) Rfree(temp->cmodule);
			if(temp->cfilename!=NULL) Rfree(temp->cfilename);
			if(temp->confld!=NULL)
			{
				for(z=0,confld1=temp->confld;z<temp->num;
					++z,++confld1)
				{
					if(confld1->fldname!=NULL) Rfree(confld1->fldname);
					if(confld1->conname!=NULL) Rfree(confld1->conname);
				}
				Rfree(temp->confld);
			}
			if(temp->subkeyname!=NULL) Rfree(temp->subkeyname);
			if(temp->expression!=NULL) Rfree(temp->expression);
			if(temp->Writes!=NULL && 
				temp->numWrites>0)
			{
				for(z=0,W1=temp->Writes;z<temp->numWrites;
					++z,++W1)
				{
					if(W1->module!=NULL)
						Rfree(W1->module);
					if(W1->filename!=NULL)
						Rfree(W1->filename);
					if(W1->expression!=NULL)
						Rfree(W1->expression);
					if(W1->fields!=NULL)
					{
						for(y=0,WF1=W1->fields;y<W1->numflds;++y,++WF1)
						{
							if(WF1->to_name!=NULL)
								Rfree(WF1->to_name);
							if(WF1->from_name!=NULL)
								Rfree(WF1->from_name);
						}
						Rfree(W1->fields);
					}
				}
				Rfree(temp->Writes);
				temp->numWrites=0;
			}
		}
		Rfree(tempsubord);
		tmpnumsub=0;
	}
	getsubords();
	if(value>=subavlmtn->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(subrsrc,"SUBORDINATE FILES",value,subavlmtn))
		updatersrc(subrsrc,"SUBORDINATE FILES");
	changedsubords=TRUE;
}
void subedit(RDArsrc *subrsrc,RDArsrc *mtnrsrc)
{
	int selected=0;
	RDAsubord *subs;
	char *modx=NULL;

	if(FINDRSCGETINT(subrsrc,"SUBORDINATE FILES",&selected)) return;
	if(tempsubord==NULL)
	{
		tempsubord=Rmalloc(sizeof(RDAsubord));
		subs=tempsubord;
		subs->module=NULL;
		subs->filename=NULL;
		subs->subkeyname=NULL;
		subs->expression=NULL;
		subs->num=0;
		subs->cmodule=NULL;
		subs->cfilename=NULL;
		subs->confld=NULL;
		subs->UsePowerWrite=FALSE;
		subs->numWrites=0;
		subs->Writes=NULL;
		tmpnumsub=1;
		selected=0;
	}
	subs=tempsubord+selected;
	if(subs!=NULL)
	{
		subordscreen(subrsrc,++selected,subs->module,subs->filename,subs->cmodule,subs->cfilename,subs->subkeyname,subs->num,subs->expression,subs->confld,subs->UsePowerWrite,subs->numWrites,subs->Writes,savemsubo,quitmsubotest);
	} else {
		readwidget(mtnrsrc,"MODULE");
		FINDRSCGETSTRING(mtnrsrc,"MODULE",&modx);
		subordscreen(subrsrc,++selected,modx,NULL,NULL,NULL,NULL,0,
			NULL,NULL,FALSE,0,NULL,savemsubo,quitmsubotest);
		if(modx!=NULL) Rfree(modx);
	}
}
void savesub(RDArsrc *mtnrsrc,MaintainMaster *MTNMSTR)
{
	int x=0,y=0,z=0;
	RDAsubord *temp1,*temp;
	RDAconfld *confld1,*confld2;
	PowerWrite *W1,*W2;
	PowerWriteField *WF1,*WF2;

	if(MTNMSTR->subord!=NULL)
	{
		for(x=0,temp=MTNMSTR->subord;x<MTNMSTR->subnum;++x,++temp)
		{
			if(temp->module!=NULL) Rfree(temp->module);
			if(temp->filename!=NULL) Rfree(temp->filename);
			if(temp->cmodule!=NULL) Rfree(temp->cmodule);
			if(temp->cfilename!=NULL) Rfree(temp->cfilename);
			if(temp->confld!=NULL)
			{
				for(z=0,confld1=temp->confld;z<temp->num;++z,++confld1)
				{
					if(confld1->fldname!=NULL) Rfree(confld1->fldname);
					if(confld1->conname!=NULL) Rfree(confld1->conname);
				}
				Rfree(temp->confld);
			}
			if(temp->subkeyname!=NULL) Rfree(temp->subkeyname);
			if(temp->expression!=NULL) Rfree(temp->expression);
			if(temp->Writes!=NULL)
			{
				for(y=0,W1=temp->Writes;y<temp->numWrites;++y,++W1)
				{
					if(W1->module!=NULL) Rfree(W1->module);
					if(W1->filename!=NULL) Rfree(W1->filename);
					if(W1->expression!=NULL) Rfree(W1->expression);
					if(W1->fields!=NULL)
					{
						for(z=0,WF1=W1->fields;z<W1->numflds;++z,++WF1)
						{
							if(WF1->to_name!=NULL) Rfree(WF1->to_name);
							if(WF1->from_name!=NULL) Rfree(WF1->from_name);
						}
						Rfree(W1->fields);
					}
				}
				Rfree(temp->Writes);
			}
		}
		Rfree(MTNMSTR->subord);
		MTNMSTR->subord=NULL;
		MTNMSTR->subnum=0;
	}
	if(tempsubord!=NULL)
	{
		MTNMSTR->subnum=tmpnumsub;
		MTNMSTR->subord=Rmalloc(tmpnumsub*sizeof(RDAsubord));
		for(x=0,temp=tempsubord,temp1=MTNMSTR->subord;x<tmpnumsub;++x,++temp,
			++temp1)
		{
			if(temp->module!=NULL)
			{
				temp1->module=stralloc(temp->module);
				Rfree(temp->module);
			} else temp1->module=NULL;
			if(temp->filename!=NULL)
			{
				temp1->filename=stralloc(temp->filename);
				Rfree(temp->filename);
			} else temp1->filename=NULL;
			if(temp->cmodule!=NULL)
			{
				temp1->cmodule=stralloc(temp->cmodule);
				Rfree(temp->cmodule);
			} else temp1->cmodule=NULL;
			if(temp->cfilename!=NULL)
			{
				temp1->cfilename=stralloc(temp->cfilename);
				Rfree(temp->cfilename);
			} else temp1->cfilename=NULL;
			temp1->dtype=temp->dtype;
			temp1->num=temp->num;
			if(temp->confld!=NULL)
			{
				temp1->confld=Rmalloc(temp->num*sizeof(RDAconfld));
				for(z=0,confld1=temp->confld,confld2=temp1->confld;
					z<temp->num;++z,++confld2,++confld1)
				{
					if(confld1->fldname!=NULL)
					{
						confld2->fldname=stralloc(confld1->fldname);
						Rfree(confld1->fldname);
					} else confld2->fldname=NULL;
					if(confld1->conname!=NULL)
					{
						confld2->conname=stralloc(confld1->conname);
						Rfree(confld1->conname);
					} else confld2->conname=NULL;
				}
				Rfree(temp->confld);
			}
			if(temp->subkeyname!=NULL)
			{
				temp1->subkeyname=stralloc(temp->subkeyname);
				Rfree(temp->subkeyname);
			} else temp1->subkeyname=NULL;
			if(temp->expression!=NULL)
			{
				temp1->expression=stralloc(temp->expression);
				Rfree(temp->expression);
			} else temp1->expression=NULL;
			temp1->UsePowerWrite=temp->UsePowerWrite;
			temp1->Writes=NULL;
			temp1->numWrites=0;
			if(temp->Writes!=NULL && 
				temp->numWrites>0)
			{
				temp1->numWrites=temp->numWrites;
				temp1->Writes=Rmalloc(temp1->numWrites*sizeof(PowerWrite));
				for(z=0,W1=temp->Writes,W2=temp1->Writes;z<temp->numWrites;
					++z,++W1,++W2)
				{
					if(W1->module!=NULL)
					{
						W2->module=stralloc(W1->module);
						Rfree(W1->module);
					} else W2->module=NULL;
					if(W1->filename!=NULL)
					{
						W2->filename=stralloc(W1->filename);
						Rfree(W1->filename);
					} else W2->filename=NULL;
					if(W1->expression!=NULL)
					{
						W2->expression=stralloc(W1->expression);
						Rfree(W1->expression);
					} else W2->expression=NULL;
					if(W1->fields!=NULL)
					{
						W2->numflds=W1->numflds;
						W2->fields=Rmalloc(W2->numflds*sizeof(PowerWriteField));
						for(y=0,WF1=W1->fields,WF2=W2->fields;y<W2->numflds;
							++y,++WF1,++WF2)
						{
							if(WF1->to_name!=NULL)
							{
								WF2->to_name=stralloc(WF1->to_name);
								Rfree(WF1->to_name);
							} else WF2->to_name=NULL;
							WF2->copy_type=WF1->copy_type;
							if(WF1->from_name!=NULL)
							{
								WF2->from_name=stralloc(WF1->from_name);
								Rfree(WF1->from_name);
							} else WF2->from_name=NULL;
						}
						Rfree(W1->fields);
					} else {
						W2->fields=NULL;
						W2->numflds=0;
					}
				}
				Rfree(temp->Writes);
				temp->numWrites=0;
			}
		}
		Rfree(tempsubord);
		tmpnumsub=0;
	}
}
