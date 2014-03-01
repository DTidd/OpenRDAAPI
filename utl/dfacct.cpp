#ifndef WIN32
#define __NAM__ "dfacct.lnx"
#endif
#ifdef WIN32
#define __NAM__ "dfacct.exe"
#endif
#include <app.hpp>
/* dfacct.c - Define Account Code */


#include <misc.hpp>


#include <gui.hpp>
#include <nrd.hpp>
#include <olh.hpp>
#include <mix.hpp>
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

#define PRINT_ALL	0
#define PRINT_ACCOUNT	1	
#define PRINT_DEFTYPE	2	
static char *prttypes[]={"PRINT ALL","PRINT ACCOUNT TYPE","PRINT DEFINITION TYPE"};
static char *module="GUI",*display_value=NULL;
static APPlib *accavl=NULL,*accparts=NULL,*defparts=NULL;
static RDAacctmstr *holdacct=NULL;
static RDAacct *acchold=NULL;
static void quitmtnaccttest(RDArsrc *,RDArsrc *),quitmtnacct(RDArsrc *,RDArsrc *);
static void savemtnacct(RDArsrc *,RDArsrc *);
static char savemtnaccttest(RDArsrc *,RDArsrc *);
static void savemtnacct2(RDArsrc *,RDArsrc *);
static void quitaccttest(RDArsrc *,RDArsrc *),quitacct(RDArsrc *,RDArsrc *);
static void saveacct(RDArsrc *,RDArsrc *);
static void saveacct2(RDArsrc *,RDArsrc *);
static char saveaccttest(RDArsrc *,RDArsrc *);
static void quitdfaccttest(RDArsrc *),quitdfacct(RDArsrc *);
static void savedfacct(RDArsrc *);
static void printdefinition(RDArsrc *,void (*)(...));
static void printaccount(RDArsrc *,RDA_PFILE *,int *,int *,unsigned);
static void printaccount1(RDArsrc *,RDAacctmstr *,RDA_PFILE *,int *,int *,unsigned,short);
static void printdeftype(RDArsrc *,RDA_PFILE *,int *,int *,unsigned);
static void printdeftypefp(RDArsrc *,RDArsrc *);
static void printdeftype1(RDArsrc *,int,RDAacctmstr *,RDA_PFILE *,int *,int *,unsigned,short);
static void printfp(RDArsrc *,void (*)(...),RDArsrc *);
static void printheader(RDA_PFILE *,int *,int *,char *,unsigned,short);
static void checkheader(RDA_PFILE *,int *,int *,unsigned,short);
static void quit_print_definition(RDArsrc *);
static RDAdefault *maindefaults=NULL,*accdefaults=NULL,*mtndefaults=NULL;
static int changedvalues=FALSE;

void makedefparts()
{
	int x,y;
	RDAaccpart *accpart;
	RDAacct *acc;
	char *temp=NULL,zeros[101];
	short defcount=0,longest1=0;

	if(defparts!=NULL) freeapplib(defparts);
	defparts=APPlibNEW();
	if(holdacct->num>0)
	{
		for(x=0,acc=holdacct->codetype;x<holdacct->num;++x,++acc)
		{
			if(temp!=NULL)
			{
				temp=Rrealloc(temp,RDAstrlen(acc->name)+7);
				memset(temp,0,RDAstrlen(acc->name)+7);
			} else {
				temp=Rmalloc(RDAstrlen(acc->name)+7);
			}
			sprintf(temp,"%-4d %s ",x,(acc->name!=NULL?acc->name:""));
			defcount=RDAstrlen(acc->name)+6;
			if(acc!=NULL)
			{
				for(y=0,accpart=acc->acct;y<acc->num;++y,++accpart)
				{
					memset(zeros,0,101);
					memset(zeros,'0',100);
					if(accpart->length>RDAstrlen(accpart->name))
					{
						longest1=accpart->length;
					} else {
						longest1=RDAstrlen(accpart->name);
					}
					zeros[longest1]=0;
					temp=Rrealloc(temp,defcount+longest1+2);
					if(y>0)
					{
						sprintf(&temp[defcount],"-%-*s",longest1,
							(accpart->force_zero ? zeros:accpart->definition));
						defcount+=longest1+1;
					} else {
						sprintf(&temp[defcount],"%-*s",longest1,
							(accpart->force_zero ? zeros:accpart->definition));
						defcount+=longest1;
					}
				}
			}
			addAPPlib(defparts,temp);
		}
		if(temp!=NULL) Rfree(temp);
	}
	if(defparts->numlibs<1)
	{
		addAPPlib(defparts,"No Definitions Available");
	}
}
static char *makedisplaystr(RDAacct *acct)
{
	RDAaccpart *accpart=NULL;
	char *displaystr=NULL,zeros[101];
	int x;
	short defcount=0,longest=0,longest1=0;

	if(acct->acct!=NULL)
	{
		for(x=0,accpart=acct->acct;x<acct->num;++x,++accpart)
		{
			if(RDAstrlen(accpart->name)>longest) longest=RDAstrlen(accpart->name);
		}
		for(x=0,accpart=acct->acct;x<acct->num;++x,++accpart)
		{
			memset(zeros,0,101);
			memset(zeros,'0',100);
			if(accpart->length>RDAstrlen(accpart->name))
			{
				longest1=accpart->length;
			} else {
				longest1=RDAstrlen(accpart->name);
			}
			zeros[longest1]=0;
			if(displaystr!=NULL)
			{
				displaystr=Rrealloc(displaystr,defcount+longest1+2);
			} else {
				displaystr=Rmalloc(defcount+longest1+1);
			}
			if(defcount>0)
			{
				sprintf(&displaystr[defcount],"-%-*s",longest1,(accpart->force_zero ? zeros:accpart->definition));
				defcount+=longest1+1;
			} else {
				sprintf(&displaystr[defcount],"%-*s",longest1,(accpart->force_zero ? zeros:accpart->definition));
				defcount+=longest1;
			}
		}
	}
	return(displaystr);
}
void makeaccparts()
{
	int x;
	char *temp=NULL,zeros[101];
	RDAaccpart *accpart;
	short defcount=0,longest=0,longest1=0;

	if(display_value!=NULL) Rfree(display_value);
	display_value=NULL;
	if(accparts!=NULL) freeapplib(accparts);
	accparts=APPlibNEW();
	if(acchold->acct!=NULL)
	{
		for(x=0,accpart=acchold->acct;x<acchold->num;++x,++accpart)
		{
			if(RDAstrlen(accpart->name)>longest) longest=RDAstrlen(accpart->name);
		}
		for(x=0,accpart=acchold->acct;x<acchold->num;++x,++accpart)
		{
			memset(zeros,0,101);
			memset(zeros,'0',100);
			if(temp!=NULL)
			{
				temp=Rrealloc(temp,longest+RDAstrlen(accpart->definition)+5);
				memset(temp,0,(longest+RDAstrlen(accpart->definition)+5));
			} else {
				temp=Rmalloc(longest+RDAstrlen(accpart->definition)+5);
			}
			sprintf(temp,"%-*s %2d %s",longest,accpart->name,accpart->length,accpart->definition);
			addAPPlib(accparts,temp);
			if(accpart->length>RDAstrlen(accpart->name))
			{
				longest1=accpart->length;
			} else {
				longest1=RDAstrlen(accpart->name);
			}
			if(display_value!=NULL)
			{
				display_value=Rrealloc(display_value,defcount+longest1+2);
			} else {
				display_value=Rmalloc(defcount+longest1+1);
			}
			zeros[longest1]=0;
			if(defcount>0)
			{
				sprintf(&display_value[defcount],"-%-*s",longest1,(accpart->force_zero ? zeros:accpart->definition));
				defcount+=longest1+1;
			} else {
				sprintf(&display_value[defcount],"%-*s",longest1,(accpart->force_zero ? zeros:accpart->definition));
				defcount+=longest1;
			}
		}
		if(temp!=NULL) Rfree(temp);
	}
	if(accparts->numlibs<1)
	{
		addAPPlib(accparts,"No Parts Defined");
	}
}
void makespaceacctpart(int pos)
{
	short x,y;
	RDAaccpart *temp,*temp1,*new_helps;

	++acchold->num;
	new_helps=Rmalloc(acchold->num*sizeof(RDAaccpart));
	y=0;
	for(x=0,temp=acchold->acct;x<(acchold->num-1);++x,++temp)
	{
		temp1=new_helps+y;
		if(x==pos)
		{
			temp1->name=NULL;
			temp1->length=0;
			temp1->definition=NULL;
			++y;
			temp1=new_helps+y;
		}
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->length=temp->length;
		if(temp->definition!=NULL)
		{
			temp1->definition=stralloc(temp->definition);
			Rfree(temp->definition);
		} else temp1->definition=NULL;
		++y;
	}
	Rfree(acchold->acct);
	acchold->acct=Rmalloc(acchold->num*sizeof(RDAaccpart));
	for(x=0,temp1=acchold->acct,temp=new_helps;x<acchold->num;++x,
		++temp,++temp1)
	{
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->length=temp->length;
		if(temp->definition!=NULL)
		{
			temp1->definition=stralloc(temp->definition);
			Rfree(temp->definition);
		} else temp1->definition=NULL;
	}
	Rfree(new_helps);
}
static void makespaceacct(int pos)
{
	short x,y;
	int z=0;
	RDAacct *temp,*temp1,*new_fields;
	RDAaccpart *a,*b;

	++holdacct->num;
	new_fields=Rmalloc(holdacct->num*sizeof(RDAacct));
	y=0;
	for(x=0,temp=holdacct->codetype;x<(holdacct->num-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->name=NULL;
			temp1->num=0;
			temp1->acct=NULL;
			++y;
			temp1=new_fields+y;
		}
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->num=temp->num;
		if(temp->acct!=NULL)
		{
			temp1->acct=Rmalloc(temp->num*sizeof(RDAaccpart));
			for(z=0,a=temp1->acct,b=temp->acct;z<temp->num;++z,++a,++b)
			{
				if(b->name!=NULL)
				{
					a->name=stralloc(b->name);
					Rfree(b->name);
				} else a->name=NULL;
				a->length=b->length;
				if(b->definition!=NULL)
				{
					a->definition=stralloc(b->definition);
					Rfree(b->definition);
				} else a->definition=NULL;
			}
		}
		++y;
	}
	Rfree(holdacct->codetype);
	holdacct->codetype=Rmalloc(holdacct->num*sizeof(RDAacct));
	for(x=0,temp1=holdacct->codetype,temp=new_fields;x<holdacct->num;++x,++temp,++temp1)
	{
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->num=temp->num;
		if(temp->acct!=NULL)
		{
			temp1->acct=Rmalloc(temp->num*sizeof(RDAaccpart));
			for(z=0,a=temp1->acct,b=temp->acct;z<temp->num;++z,++a,++b)
			{
				if(b->name!=NULL)
				{
					a->name=stralloc(b->name);
					Rfree(b->name);
				} else a->name=NULL;
				a->length=b->length;
				if(b->definition!=NULL)
				{
					a->definition=stralloc(b->definition);
					Rfree(b->definition);
				} else a->definition=NULL;
			}
		}
	}
	Rfree(new_fields);
}
void quitmtnaccttest(RDArsrc *mtnrsrc,RDArsrc *accrsrc)
{
	readallwidgets(mtnrsrc);
	if(COMPARE_RSRCS(mtnrsrc,mtndefaults,0))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the Account Dimension Definition.\nExiting now without saving will loose these changes.",savemtnacct2,quitmtnacct,FALSE,2,mtnrsrc,accrsrc,NULL);
	} else {
		changedvalues=FALSE;
		quitmtnacct(mtnrsrc,accrsrc);
	}
}
void quitmtnacct(RDArsrc *mtnrsrc,RDArsrc *accrsrc)
{
	if(mtnrsrc!=NULL)
	{
		killwindow(mtnrsrc);
		free_rsrc(mtnrsrc);
	}
	if(mtndefaults!=NULL) FreeRDAdefault(mtndefaults);
}
char savemtnaccttest(RDArsrc *mtnrsrc,RDArsrc *accrsrc)
{
	char *test=NULL;

	readallwidgets(mtnrsrc);
	if(!FINDRSCGETSTRING(mtnrsrc,"NAME",&test))
	{
		if(isEMPTY(test))
		{
			if(test!=NULL) Rfree(test); 
			ERRORDIALOG("MISSING DIMENSION NAME!","Account Dimensions are required to have Names.\nPlease provide this information before saving.",NULL,FALSE);
			return(FALSE);
		} else {
			if(test!=NULL) Rfree(test); 
/*
			savemtnacct(mtnrsrc,accrsrc);
*/
			return(TRUE);
		}
	}
	return(FALSE);
}
void savemtnacct2(RDArsrc *mtnrsrc,RDArsrc *accrsrc)
{
	char *test=NULL;

	readallwidgets(mtnrsrc);
	if(!FINDRSCGETSTRING(mtnrsrc,"NAME",&test))
	{
		if(isEMPTY(test))
		{
			if(test!=NULL) Rfree(test); 
			ERRORDIALOG("MISSING DIMENSION NAME!","Account Dimensions are required to have Names.\nPlease provide this information before saving.",NULL,FALSE);
		} else {
			if(test!=NULL) Rfree(test); 
			savemtnacct(mtnrsrc,accrsrc);
		}
	}
}
void savemtnacct(RDArsrc *mtnrsrc,RDArsrc *accrsrc)
{
	int value;
	RDAaccpart *fds;
	char *test=NULL,*test1=NULL,*hold;
	short x;

	if(FINDRSCGETINT(accrsrc,"ACCOUNT PARTS",&value)) return;
	if(acchold->acct==NULL)
	{
		acchold->acct=Rmalloc(sizeof(RDAaccpart));
		acchold->num=1;
	}
	fds=acchold->acct+value;
	readallwidgets(mtnrsrc);
	if(!FINDRSCGETSTRING(mtnrsrc,"NAME",&test))
	{
		if(!isEMPTY(test))	
		{
			if(RDAstrlen(test)>15) test[15]=0;
			fds->name=stralloc(test);
		}
		if(test!=NULL) Rfree(test); 
	}	
	FINDRSCGETSHORT(mtnrsrc,"LENGTH",&fds->length);
	if(fds->length>15) fds->length=15;
		else if(fds->length<=0) fds->length=1;
	FINDRSCGETCHAR(mtnrsrc,"DEFAULT ZERO",&fds->default_zero);
	FINDRSCGETCHAR(mtnrsrc,"FORCE ZERO",&fds->force_zero);
	FINDRSCGETCHAR(mtnrsrc,"LAST DIMENSION",&fds->last_dimension);
	if(!FINDRSCGETSTRING(mtnrsrc,"DEFINITION",&test1))
	{
		if(isEMPTY(test1))
/*
		if(test1==NULL || RDAstrlen(test1)<1)
*/
		{
			if(test1!=NULL)
			{
				test1=Rrealloc(test1,fds->length+1);
				memset(test1,0,fds->length+1);
			} else {
				test1=Rmalloc(fds->length+1);
			}
			memset(test1,'*',fds->length);
		} else {
			if(RDAstrlen(test1)>fds->length) test1[fds->length]=0;
			if(RDAstrlen(test1)<fds->length)
			{
				test1=Rrealloc(test1,fds->length+1);
				for(x=0,hold=test1;x<fds->length;++x,++hold)
				{
					switch(*hold)
					{
						case 'A':
						case 'a':
							*hold='A';
							break;
						case 'N':
						case 'n':
							*hold='N';
							break;
						case 'X':
						case 'x':
							*hold='X';
							break;
						default:
							*hold='*';
							break;
					}
				}
			} else {
				hold=test1;
				for(;*hold;++hold)
				{
					switch(*hold)
					{
						case 'A':
						case 'a':
							*hold='A';
							break;
						case 'N':
						case 'n':
							*hold='N';
							break;
						case 'X':
						case 'x':
							*hold='X';
							break;
						default:
							*hold='*';
							break;
					}
				}
			}
		}
		fds->definition=stralloc(test1);
		if(test1!=NULL) Rfree(test1);
	}
	makeaccparts();
	if(!FINDRSCSETSTRING(accrsrc,"DISPLAY",display_value))
		updatersrc(accrsrc,"DISPLAY");
	if(!FINDRSCLISTAPPlib(accrsrc,"ACCOUNT PARTS",value,accparts))
		updatersrc(accrsrc,"ACCOUNT PARTS");
/*
	if(COMPARE_RSRCS(mtnrsrc,mtndefaults,0))
	{
*/
	changedvalues=TRUE;
/*
	}
*/
	quitmtnacct(mtnrsrc,accrsrc);
}
void saveaddabove(RDArsrc *mtnrsrc,RDArsrc *accrsrc)
{
	int x;
	char doit=FALSE;

	doit=savemtnaccttest(mtnrsrc,accrsrc);
	if(doit)
	{
		if(FINDRSCGETINT(accrsrc,"ACCOUNT PARTS",&x)) return;
		makespaceacctpart(x);
		savemtnacct(mtnrsrc,accrsrc);
	}
}
void saveaddbelow(RDArsrc *mtnrsrc,RDArsrc *accrsrc)
{
	int x;
	char doit=FALSE;

	doit=savemtnaccttest(mtnrsrc,accrsrc);
	if(doit)
	{
		if(FINDRSCGETINT(accrsrc,"ACCOUNT PARTS",&x)) return;
		makespaceacctpart(++x);
		if(!FINDRSCSETINT(accrsrc,"ACCOUNT PARTS",x))
			updatersrc(accrsrc,"ACCOUNT PARTS");
		savemtnacct(mtnrsrc,accrsrc);
	}
}
void maintain_part(RDArsrc *accrsrc,int number,char *name,short length,char *definition,char d_zero,char f_zero,char l_dim,void (*savefunc)(...))
{
	RDArsrc *mtnrsrc=NULL;

	mtnrsrc=RDArsrcNEW(module,"MAINTAIN ACCOUNT DIMENSION");
	addstdrsrc(mtnrsrc,"NUMBER",LONGV,3,&number,FALSE);
	addstdrsrc(mtnrsrc,"NAME",VARIABLETEXT,15,name,TRUE);
	addstdrsrc(mtnrsrc,"LENGTH",SHORTV,2,&length,TRUE);
	addstdrsrc(mtnrsrc,"DEFINITION",VARIABLETEXT,15,definition,TRUE);
	addstdrsrc(mtnrsrc,"DEFAULT ZERO",BOOLNS,1,&d_zero,TRUE);
	addstdrsrc(mtnrsrc,"FORCE ZERO",BOOLNS,1,&f_zero,TRUE);
	addstdrsrc(mtnrsrc,"LAST DIMENSION",BOOLNS,1,&l_dim,TRUE);
	addrfcbrsrc(mtnrsrc,"SAVE",TRUE,savefunc,accrsrc);
	addrfcbrsrc(mtnrsrc,"QUIT",TRUE,quitmtnaccttest,accrsrc);
	addbtnrsrc(mtnrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(mtnrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	mtndefaults=GetDefaults(mtnrsrc);
	APPmakescrn(mtnrsrc,TRUE,quitmtnacct,accrsrc,FALSE);
}
void addabove(RDArsrc *accrsrc)
{
	int selected=0;

	if(accparts->numlibs==1 && !RDAstrcmp(accparts->libs[0],"No Parts Defined")) 
		return;
	if(FINDRSCGETINT(accrsrc,"ACCOUNT PARTS",&selected)) return;
	maintain_part(accrsrc,selected,NULL,0,NULL,FALSE,FALSE,FALSE,saveaddabove);
}
void addbelow(RDArsrc *accrsrc)
{
	int selected=0;

	if(accparts->numlibs==1 && !RDAstrcmp(accparts->libs[0],"No Parts Defined")) 
		return;
	if(FINDRSCGETINT(accrsrc,"ACCOUNT PARTS",&selected)) return;
	maintain_part(accrsrc,++selected,NULL,0,NULL,FALSE,FALSE,FALSE,saveaddbelow);
}
void editacct(RDArsrc *accrsrc)
{
	int selected=0;
	RDAaccpart *c;

	if(FINDRSCGETINT(accrsrc,"ACCOUNT PARTS",&selected)) return;
	c=acchold->acct+selected;
	if(c!=NULL && acchold->acct!=NULL)
	{
		maintain_part(accrsrc,selected,c->name,c->length,c->definition,c->default_zero,c->force_zero,c->last_dimension,savemtnacct2);
	} else {
		maintain_part(accrsrc,selected,NULL,0,NULL,FALSE,FALSE,FALSE,savemtnacct2);
	}
}
void deleteacct(RDArsrc *accrsrc)
{
	short x,y;
	RDAaccpart *temp,*temp1,*new_helps;
	int value=0;

	if(acchold->num<=1) return;
	readallwidgets(accrsrc);
	if(FINDRSCGETINT(accrsrc,"ACCOUNT PARTS",&value)) return;
	new_helps=Rmalloc((acchold->num-1)*sizeof(RDAaccpart));
	y=0;
	for(x=0,temp=acchold->acct;x<acchold->num;++x,++temp)
	{
		if(x!=value)
		{
			temp1=new_helps+y;
			if(temp->name!=NULL)
			{
				temp1->name=stralloc(temp->name);
				Rfree(temp->name);
			} else temp1->name=NULL;
			temp1->length=temp->length;
			if(temp->definition!=NULL)
			{
				temp1->definition=stralloc(temp->definition);
				Rfree(temp->definition);
			} else temp1->definition=NULL;
			temp1->default_zero=temp->default_zero;
			temp1->force_zero=temp->force_zero;
			temp1->last_dimension=temp->last_dimension;
			++y;
		} else {
			if(temp->name!=NULL) Rfree(temp->name);
			if(temp->definition!=NULL) Rfree(temp->definition);
			
		}
	}
	Rfree(acchold->acct);
	--acchold->num;
	acchold->acct=Rmalloc(acchold->num*sizeof(RDAaccpart));
	for(x=0,temp1=acchold->acct,temp=new_helps;x<acchold->num;++x,
		++temp,++temp1)
	{
		if(temp->name!=NULL)
		{
			temp1->name=stralloc(temp->name);
			Rfree(temp->name);
		} else temp1->name=NULL;
		temp1->length=temp->length;
		if(temp->definition!=NULL)
		{
			temp1->definition=stralloc(temp->definition);
			Rfree(temp->definition);
		} else temp1->definition=NULL;
		temp1->default_zero=temp->default_zero;
		temp1->force_zero=temp->force_zero;
		temp1->last_dimension=temp->last_dimension;
	}
	Rfree(new_helps);
	makeaccparts();
	if(!FINDRSCSETSTRING(accrsrc,"DISPLAY",display_value))
		updatersrc(accrsrc,"DISPLAY");
	if(value>=accparts->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(accrsrc,"ACCOUNT PARTS",value,accparts))
		updatersrc(accrsrc,"ACCOUNT PARTS");
	changedvalues=TRUE;
}
void quitacct(RDArsrc *accrsrc,RDArsrc *mainrsrc)
{
	int x;
	RDAaccpart *accpart=NULL;

	if(accrsrc!=NULL)
	{
		killwindow(accrsrc);
		free_rsrc(accrsrc);
	}
	if(acchold!=NULL)
	{
		if(acchold->acct!=NULL)
		{
			for(x=0,accpart=acchold->acct;x<acchold->num;++x,++accpart)
			{
				if(accpart->name!=NULL) Rfree(accpart->name);
				if(accpart->definition!=NULL) Rfree(accpart->definition);
			}
			Rfree(acchold->acct);
		}
		Rfree(acchold);
	}
	if(accdefaults!=NULL) FreeRDAdefault(accdefaults);
}
void quitaccttest(RDArsrc *accrsrc,RDArsrc *mainrsrc)
{
	readallwidgets(accrsrc);
	if(COMPARE_RSRCS(accrsrc,accdefaults,0))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this ACCOUNT DEFINITION.\nExiting now without saving will loose these changes.",saveacct2,quitacct,FALSE,2,accrsrc,mainrsrc,NULL);
	} else {
		changedvalues=FALSE;
		quitacct(accrsrc,mainrsrc);
	}
}
char saveaccttest(RDArsrc *accrsrc,RDArsrc *mainrsrc)
{
	char *test=NULL;

	readallwidgets(accrsrc);
	FINDRSCGETSTRING(accrsrc,"NAME",&test);
	if(isEMPTY(test))
	{
		if(test!=NULL) Rfree(test);
		ERRORDIALOG("MISSING ACCOUNT DESCRIPTION!","Account Definitions are required to have Descritions/Names.\nPlease provide this information before saving.",NULL,FALSE);
		return(FALSE);
	} else {
		if(test!=NULL) Rfree(test);
		return(TRUE);
/*
		saveacct(accrsrc,mainrsrc);
*/
	}
}
void saveacct2(RDArsrc *accrsrc,RDArsrc *mainrsrc)
{
	char *test=NULL;

	readallwidgets(accrsrc);
	FINDRSCGETSTRING(accrsrc,"NAME",&test);
	if(isEMPTY(test))
	{
		if(test!=NULL) Rfree(test);
		ERRORDIALOG("MISSING ACCOUNT DESCRIPTION!","Account Definitions are required to have Descritions/Names.\nPlease provide this information before saving.",NULL,FALSE);
	} else {
		if(test!=NULL) Rfree(test);
		saveacct(accrsrc,mainrsrc);
	}
}
void saveacct(RDArsrc *accrsrc,RDArsrc *mainrsrc)
{
	int selected=0,x;
	RDAacct *acct;
	RDAaccpart *a,*b;

	readallwidgets(accrsrc);
	if(FINDRSCGETINT(mainrsrc,"DEFINITIONS AVAILABLE",&selected)) return;
	if(holdacct->codetype==NULL)
	{
		holdacct->codetype=Rmalloc(sizeof(RDAacct));
		holdacct->num=1;
	}
	acct=holdacct->codetype+selected;
	FINDRSCGETSTRING(accrsrc,"NAME",&acct->name);
	if(acchold!=NULL)
	{
		acct->num=acchold->num;
		if(acchold->acct!=NULL)
		{
			acct->acct=Rmalloc(acchold->num*sizeof(RDAaccpart));
			for(x=0,a=acct->acct,b=acchold->acct;x<acchold->num;++x,++a,++b)
			{
				if(b->name!=NULL)
				{
					a->name=stralloc(b->name);
					Rfree(b->name);
				} else a->name=NULL;
				a->length=b->length;
				if(b->definition!=NULL)
				{
					a->definition=stralloc(b->definition);
					Rfree(b->definition);
				} else a->definition=NULL;
				a->default_zero=b->default_zero;
				a->force_zero=b->force_zero;
				a->last_dimension=b->last_dimension;
			}
			Rfree(acchold->acct);
		} else {
			acct->acct=NULL;
		}
		Rfree(acchold);
	} else {
		acct->num=0;
	}
	makedefparts();
	if(!FINDRSCLISTAPPlib(mainrsrc,"DEFINITIONS AVAILABLE",0,defparts))
		updatersrc(mainrsrc,"DEFINITIONS AVAILABLE");
/*
	if(COMPARE_RSRCS(accrsrc,accdefaults,0))
	{
*/
	changedvalues=TRUE;
/*
	}
*/
	quitacct(accrsrc,mainrsrc);
}
void saveacctabove(RDArsrc *accrsrc,RDArsrc *mainrsrc)
{
	int x;
	char doit=FALSE;

	doit=saveaccttest(accrsrc,mainrsrc);
	if(doit)
	{
		if(FINDRSCGETINT(mainrsrc,"DEFINITIONS AVAILABLE",&x)) return;
		makespaceacct(x);
		saveacct(accrsrc,mainrsrc);
	}
}
void saveacctbelow(RDArsrc *accrsrc,RDArsrc *mainrsrc)
{
	int x;
	char doit=FALSE;

	doit=saveaccttest(accrsrc,mainrsrc);
	if(doit)
	{
		if(FINDRSCGETINT(mainrsrc,"DEFINITIONS AVAILABLE",&x)) return;
		makespaceacct(++x);
		FINDRSCSETINT(mainrsrc,"DEFINITIONS AVAILABLE",x);
		saveacct(accrsrc,mainrsrc);
	}
}
void managedefinition(RDArsrc *mainrsrc,int num,char *name,int numaccts,RDAaccpart *accpart,void (*savefunc)(...))
{
	int x=0;
	int defno=0;
	RDAaccpart *a1,*a2;
	RDArsrc *accrsrc=NULL;

	if(FINDRSCGETINT(mainrsrc,"DEFINITIONS AVAILABLE",&defno)) return;
	acchold=Rmalloc(sizeof(RDAacct));
	acchold->num=numaccts;
	acchold->acct=NULL;
	if(accpart!=NULL)
	{
		acchold->num=numaccts;
		acchold->acct=Rmalloc(acchold->num*sizeof(RDAaccpart));
		for(x=0,a1=accpart,a2=acchold->acct;x<numaccts;++x,++a1,++a2)
		{
			if(a1->name!=NULL) 
			{
				a2->name=stralloc(a1->name);
			} else a2->name=NULL;
			if(a1->definition!=NULL) 
			{
				a2->definition=stralloc(a1->definition);
			} else a2->definition=NULL;
			a2->length=a1->length;
			a2->default_zero=a1->default_zero;
			a2->force_zero=a1->force_zero;
			a2->last_dimension=a1->last_dimension;
		}
	} else {
		acchold->num=0;
		acchold->acct=NULL;
	}
	accrsrc=RDArsrcNEW(module,"MAINTAIN ACCOUNTS");
	addstdrsrc(accrsrc,"NUMBER",LONGV,4,&num,FALSE);
	addstdrsrc(accrsrc,"NAME",VARIABLETEXT,0,name,TRUE);
	addstdrsrc(accrsrc,"DISPLAY",VARIABLETEXT,0,NULL,FALSE);
	if(accparts!=NULL) freeapplib(accparts);
	accparts=APPlibNEW();
	addlstrsrc(accrsrc,"ACCOUNT PARTS",&x,TRUE,NULL,accparts->numlibs,
		&accparts->libs,NULL);
	addbtnrsrc(accrsrc,"ADD ABOVE",TRUE,addabove,NULL);
	addbtnrsrc(accrsrc,"ADD BELOW",TRUE,addbelow,NULL);
	addbtnrsrc(accrsrc,"SELECT",TRUE,editacct,NULL);
	addbtnrsrc(accrsrc,"DELETE",TRUE,deleteacct,NULL);
	addrfcbrsrc(accrsrc,"SAVE",TRUE,savefunc,mainrsrc);
	addrfcbrsrc(accrsrc,"QUIT",TRUE,quitaccttest,mainrsrc);
	addbtnrsrc(accrsrc,"HELP",TRUE,screenhelp,NULL);
/*lint -e611 */
	addbtnrsrc(accrsrc,"PRINT DEFTYPE",TRUE,printdefinition,(void *)printdeftypefp);
/*lint +e611 */
	addbtnrsrc(accrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	makeaccparts();
	FINDRSCSETSTRING(accrsrc,"DISPLAY",display_value);
	FINDRSCLISTAPPlib(accrsrc,"ACCOUNT PARTS",0,accparts);
	accdefaults=GetDefaults(accrsrc);
	APPmakescrn(accrsrc,TRUE,quitacct,mainrsrc,FALSE);
}
void quitdfaccttest(RDArsrc *mainrsrc)
{
	readallwidgets(mainrsrc);
/*
	if(COMPARE_RSRCS(mainrsrc,maindefaults,0) || changedvalues)
*/
	if(changedvalues)
	{
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this ACCOUNT DEFINITION.\nExiting now without saving will loose these changes.",savedfacct,quitdfacct,FALSE,mainrsrc);
	} else {
		quitdfacct(mainrsrc);
	}
}
void quitdfacct(RDArsrc *mainrsrc)
{
	if(mainrsrc!=NULL)
	{
		killwindow(mainrsrc);
		free_rsrc(mainrsrc);
	}
	if(accavl!=NULL) freeapplib(accavl);
	if(holdacct!=NULL) FreeRDAacctmstr(holdacct);
	if(maindefaults!=NULL) FreeRDAdefault(maindefaults);
	ShutdownSubsystems();
	std::exit;
}
void savedfacct(RDArsrc *mainrsrc)
{
	if(writeacctbin(holdacct))
	{
		ERRORDIALOG("Cannot Write Account Definitions","Cannot save the account code definitions.  Check your permissions, and retry.  Call your installer.",NULL,FALSE);
	}
	changedvalues=FALSE;
}
void changeacctype(RDArsrc *mainrsrc)
{
	int accounttype=0;

	FINDRSCGETINT(mainrsrc,"ACCOUNT TYPES LIST",&accounttype);
	if(holdacct!=NULL) FreeRDAacctmstr(holdacct);
	holdacct=RDAacctNEW(accounttype);
	getacctbin(holdacct);
	makedefparts();
	changedvalues=FALSE;
	if(!FINDRSCLISTAPPlib(mainrsrc,"DEFINITIONS AVAILABLE",0,defparts))
	{
		updatersrc(mainrsrc,"DEFINITIONS AVAILABLE");
	}
}
void changetypes(RDArsrc *mainrsrc)
{
	if(changedvalues)
	{
		WARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to this ACCOUNT DEFINITION.\nProceeding now without saving will loose these changes.",savedfacct,changeacctype,FALSE,mainrsrc);
	} else {
		changeacctype(mainrsrc);
/*
		if(!FINDRSCLISTAPPlib(mainrsrc,"DEFINITIONS AVAILABLE",0,defparts))
		{
			updatersrc(mainrsrc,"DEFINITIONS AVAILABLE");
		}
*/
	}
}
void adddefabove(RDArsrc *mainrsrc)
{
	int selected=0;

	if(defparts->numlibs==1 && !RDAstrcmp(defparts->libs[0],"No Definitions Available")) return;
	if(FINDRSCGETINT(mainrsrc,"DEFINITIONS AVAILABLE",&selected)) return;
	managedefinition(mainrsrc,selected,NULL,0,NULL,saveacctabove);
}
void adddefbelow(RDArsrc *mainrsrc)
{
	int selected=0;

	if(defparts->numlibs==1 && !RDAstrcmp(defparts->libs[0],"No Definitions Available")) return;
	if(FINDRSCGETINT(mainrsrc,"DEFINITIONS AVAILABLE",&selected)) return;
	managedefinition(mainrsrc,++selected,NULL,0,NULL,saveacctbelow);
}
void copydefacct(RDArsrc *mainrsrc)
{
	int selected=0;
	RDAacct *acct;

	if(accparts->numlibs==1 && !RDAstrcmp(accparts->libs[0],"No Parts Defined")) 
		return;
	if(FINDRSCGETINT(mainrsrc,"DEFINITIONS AVAILABLE",&selected)) return;
	acct=holdacct->codetype+selected;
	if(acct!=NULL)
	{
		managedefinition(mainrsrc,++selected,acct->name,acct->num,acct->acct,saveacctbelow);
	}
}
void editdefacct(RDArsrc *mainrsrc)
{
	int selected=0;
	RDAacct *acct;

	if(FINDRSCGETINT(mainrsrc,"DEFINITIONS AVAILABLE",&selected)) return;
	acct=holdacct->codetype+selected;
	if(acct!=NULL)
	{
		managedefinition(mainrsrc,selected,acct->name,acct->num,acct->acct,saveacct2);
	} else {
		managedefinition(mainrsrc,selected,NULL,0,NULL,saveacct2);
	}
}
void deletedefacct(RDArsrc *mainrsrc)
{
	short x,y;
	int z;
	RDAacct *temp,*temp1,*new_fields;
	int value;
	RDAaccpart *a,*b;

	if(FINDRSCGETINT(mainrsrc,"DEFINITIONS AVAILABLE",&value)) return;
	if(holdacct->num<1) return;
	if(holdacct->num>1)
	{
		new_fields=Rmalloc((holdacct->num-1)*sizeof(RDAacct));
		y=0;
		for(x=0,temp=holdacct->codetype;x<holdacct->num;++x,++temp)
		{
			temp1=new_fields+y;
			if(x!=value)
			{
				if(temp->name!=NULL)
				{
					temp1->name=stralloc(temp->name);
					Rfree(temp->name);
				} else temp1->name=NULL;
				temp1->num=temp->num;
				if(temp->acct!=NULL)
				{
					temp1->acct=Rmalloc(temp->num*sizeof(RDAaccpart));
					temp1->num=temp->num;
					for(z=0,a=temp1->acct,b=temp->acct;z<temp->num;++z,++a,++b)
					{
						if(b->name!=NULL)
						{
							a->name=stralloc(b->name);
							Rfree(b->name);
						} else a->name=NULL;
						a->length=b->length;
						if(b->definition!=NULL)
						{
							a->definition=stralloc(b->definition);
							Rfree(b->definition);
						} else a->definition=NULL;
						a->default_zero=b->default_zero;
						a->force_zero=b->force_zero;
						a->last_dimension=b->last_dimension;
					}
				}
				++y;
			}
		}
		Rfree(holdacct->codetype);
		--holdacct->num;
		holdacct->codetype=Rmalloc(holdacct->num*sizeof(RDAacct));
		for(x=0,temp1=holdacct->codetype,temp=new_fields;x<holdacct->num;++x,
			++temp,++temp1)
		{
			if(temp->name!=NULL)
			{
				temp1->name=stralloc(temp->name);
				Rfree(temp->name);
			} else temp1->name=NULL;
			temp1->num=temp->num;
			if(temp->acct!=NULL)
			{
				temp1->acct=Rmalloc(temp->num*sizeof(RDAaccpart));
				temp1->num=temp->num;
				for(z=0,a=temp1->acct,b=temp->acct;z<temp->num;++z,++a,++b)
				{
					if(b->name!=NULL)
					{
						a->name=stralloc(b->name);
						Rfree(b->name);
					} else a->name=NULL;
					a->length=b->length;
					if(b->definition!=NULL)
					{
						a->definition=stralloc(b->definition);
						Rfree(b->definition);
					} else a->definition=NULL;
					a->default_zero=b->default_zero;
					a->force_zero=b->force_zero;
					a->last_dimension=b->last_dimension;
				}
			}
		}
		Rfree(new_fields);
	} else if(holdacct->codetype!=NULL) 
	{
		for(x=0,temp=holdacct->codetype;x<holdacct->num;++x,++temp)
		{
			if(temp->acct!=NULL)
			{
				for(y=0,a=temp->acct;y<temp->num;++y,++a)
				{
					if(a->definition!=NULL) Rfree(a->definition);
					if(a->name!=NULL) Rfree(a->name);
				}
				Rfree(temp->acct);
			}
		}	
		Rfree(holdacct->codetype);
		holdacct->codetype=NULL;
		holdacct->num=0;
	}
	makedefparts();
	if(!FINDRSCLISTAPPlib(mainrsrc,"DEFINITIONS AVAILABLE",0,defparts))
		updatersrc(mainrsrc,"DEFINITIONS AVAILABLE");
	if(maindefaults!=NULL) FreeRDAdefault(maindefaults);
	maindefaults=GetDefaults(mainrsrc);
	changedvalues=TRUE;
}
void deletedefaccttest(RDArsrc *mainrsrc)
{
	WARNINGDIALOG("DELETE WARNING DIALOG SCREEN","DELETE ACCOUNT DEFINITION?","If ACCOUNTS EXIST in the files this action is not recommended, DATA CAN BE LOST.\nThis is a setup procedure and can DAMAGE your DATA if not used properly.\nAre you sure that you want to delete this ACCOUNT DEFINITION?",deletedefacct,NULL,FALSE,mainrsrc);
}
static void quit_print_definition(RDArsrc *prsrc)
{
	if(diaggui)
	{
		prterr("DIAG quit_print_definition Quitting Print Screen Definition.");
	}
	if(prsrc!=NULL) free_rsrc(prsrc);
}	
static void printheader(RDA_PFILE *fp,int *lines,int *pages,char *header,
	unsigned line_length,short printtype)
{
	char *date=NULL,*timex=NULL;
	int lead_ws=0,tail_ws=0;

	date=GETCURRENTDATE();
	timex=GETCURRENTTIME();
	RDA_fprintf(fp,"\n");
	*lines=1;
	lead_ws=(int)(RDAroundup((double)(line_length-(2+RDAstrlen(header)))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(2+RDAstrlen(header)))/2)-14));
	if(tail_ws<0) tail_ws=0;
	if((RDAstrlen(header)+2+14+lead_ws+tail_ws)>line_length) header[(line_length-14-lead_ws-tail_ws)]=0;
	RDA_fprintf(fp,"%*s %s %*sPage: %*d\r\n",
		lead_ws,"",header,tail_ws,"",8,++*pages);
	++*lines;
	lead_ws=(int)(RDAroundup((double)(line_length-(14+RDAstrlen(prttypes[printtype])))/2));
	if(lead_ws<0) lead_ws=0;
	tail_ws=(int)(Rint(((double)(line_length-(14+RDAstrlen(prttypes[printtype])))/2)-14));
	if(tail_ws<0) tail_ws=0;
	RDA_fprintf(fp,"%*s Print Type: %s %*sDate: %s\r\n",
		lead_ws,"",prttypes[printtype],tail_ws,"",date);
	++*lines;
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
		++*lines;
	} else {
		RDA_fprintf(fp,"%*s Executed By: %s %*sTime: %s\r\n",
			lead_ws,"",USERLOGIN,tail_ws,"",timex);
		++*lines;
	}
	RDA_fprintf(fp,"\r\n");
	++*lines;
	if(date!=NULL) Rfree(date);
	if(timex!=NULL) Rfree(timex);
}
static void checkheader(RDA_PFILE *fp,int *lines,int *pages,unsigned line_length,short printtype)
{
	char *temp=NULL;

	if(*lines>57)
	{
		RDA_fprintf(fp,"\f");
		temp=stralloc("XPERT ACCOUNT DEFINITIONS"); 
		printheader(fp,lines,pages,temp,line_length,printtype);
		if(temp!=NULL) Rfree(temp);
	}
}
static void printdefinition(RDArsrc *mtn,void (*printfunc)(...))
{
	RDArsrc *prsrc=NULL;
	char *defaultprinter=NULL,normal_print=TRUE;

	if(diaggui)
	{
		prterr("DIAG printdefinition Displaying the Print Screen in preparation to print account definition.");
	}
	prsrc=RDArsrcNEW(module,"PRINT SCREEN");
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
static void printfp(RDArsrc *prsrc,void (*pfunc)(...),RDArsrc *rsrc)
{
	char *outdevice=NULL,*temp=NULL,print_style=TRUE;
	RDA_PFILE *fp=NULL;
	int pages=0,lines=0;
	unsigned line_length=79;

	readwidget(prsrc,"DEVICE");
        FINDRSCGETSTRING(prsrc,"DEVICE",&outdevice);
	readwidget(prsrc,"NORMAL PRINT");
	FINDRSCGETCHAR(prsrc,"NORMAL PRINT",&print_style);
	if(outdevice!=NULL)
	{
		fp=RDA_popen(outdevice,"w");
		if(fp!=NULL)
		{
			SelectPrintType(outdevice,print_style,fp);
/*lint -e746*/
			pfunc(rsrc,fp,&lines,&pages,line_length);
/*lint +e746*/
			RDA_pclose(fp);
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
static void printall(RDArsrc *rsrc,RDA_PFILE *fp,int *lines,int *pages,unsigned line_length)
{
	char *temp=NULL;
	int x=0;
	RDAacctmstr *lholdacct=NULL;

	temp=stralloc("XPERT ACCOUNT DEFINITIONS"); 
	printheader(fp,lines,pages,temp,line_length,PRINT_ALL);
	if(temp!=NULL) Rfree(temp);
	for(x=0;x<accavl->numlibs;++x)
	{
		lholdacct=RDAacctNEW(x);
		getacctbin(lholdacct);
		if(lholdacct!=NULL)
		{
			if(lholdacct->codetype!=NULL) printaccount1(rsrc,lholdacct,fp,lines,pages,line_length,PRINT_ALL);
			FreeRDAacctmstr(lholdacct);
		}
	}
}
static void printallfp(RDArsrc *prsrc,RDArsrc *rsrc)
{
	printfp(prsrc,printall,rsrc);
}
static void printaccountfp(RDArsrc *prsrc,RDArsrc *rsrc)
{
        if(!RDAstrcmp(defparts->libs[0],"No Definitions Available")) return;
	printfp(prsrc,printaccount,rsrc);
}
static void printdeftypefp(RDArsrc *prsrc,RDArsrc *rsrc)
{
        if(!RDAstrcmp(accparts->libs[0],"No Parts Defined")) return;
	printfp(prsrc,printdeftype,rsrc);
}
static void printdeftype1(RDArsrc *rsrc,int deftype,RDAacctmstr *acctmstr,RDA_PFILE *fp,int *lines,int *pages,unsigned line_length,short printtype)
{
	char *displaystr=NULL;
	RDAacct *acct=NULL;
	RDAaccpart *accpart=NULL;
	int x;

	acct=acctmstr->codetype+deftype;
	if(printtype==PRINT_DEFTYPE)
	{
		RDA_fprintf(fp,"Account Type Description: %s\r\n",accttypes[acctmstr->type]);
		*lines+=2;
		readwidget(rsrc,"DISPLAY");
		FINDRSCGETSTRING(rsrc,"DISPLAY",&displaystr);
		RDA_fprintf(fp,"Definition Type Description: %s\r\n",acct->name);
		++*lines;
	} else {
		displaystr=makedisplaystr(acct);
		checkheader(fp,lines,pages,line_length,printtype);
		RDA_fprintf(fp,"Definition Type Description: %s\r\n",acct->name);
		++*lines;
		checkheader(fp,lines,pages,line_length,printtype);
	}
	RDA_fprintf(fp,"\tDisplay String: %s\r\n",displaystr);
	if(displaystr!=NULL) Rfree(displaystr);
	++*lines;
	checkheader(fp,lines,pages,line_length,printtype);
	RDA_fprintf(fp,"\tAccount Parts:\r\n");
	++*lines;
	for(x=0,accpart=acct->acct;x<acct->num;++x,++accpart)
	{
		checkheader(fp,lines,pages,line_length,printtype);
		RDA_fprintf(fp,"\t\tDimension Name: %s\r\n",(accpart->name!=NULL?accpart->name:""));
		++*lines;
		checkheader(fp,lines,pages,line_length,printtype);
		RDA_fprintf(fp,"\t\t\tLength: %d  Definition: %s Force Zero %s Default Zero %s FINSBC Component %s\r\n",accpart->length,(accpart->definition!=NULL?accpart->definition:""),(accpart->force_zero ? "Yes":"No"),(accpart->default_zero ? "Yes":"No"),(accpart->last_dimension ? "Yes":"No"));
		++*lines;
	}
	RDA_fprintf(fp,"\r\n");	
	++*lines;
}
static void printdeftype(RDArsrc *rsrc,RDA_PFILE *fp,int *lines,int *pages,unsigned line_length)
{
	int deftype=0;
	char *temp=NULL;

	temp=stralloc("XPERT ACCOUNT DEFINITIONS"); 
	printheader(fp,lines,pages,temp,line_length,PRINT_DEFTYPE);
	if(temp!=NULL) Rfree(temp);
	FINDRSCGETINT(rsrc,"NUMBER",&deftype);
	printdeftype1(rsrc,deftype,holdacct,fp,lines,pages,line_length,PRINT_DEFTYPE);
}
static void printaccount1(RDArsrc *rsrc,RDAacctmstr *lholdacct,RDA_PFILE *fp,
	int *lines,int *pages,unsigned line_length,short printtype)
{
	int x;
	
	checkheader(fp,lines,pages,line_length,printtype);
	RDA_fprintf(fp,"Account Type Description: %s\r\n",accttypes[lholdacct->type]);
	++*lines;
	for(x=0;x<lholdacct->num;++x)
	{
		printdeftype1(rsrc,x,lholdacct,fp,lines,pages,line_length,printtype);
	}
}
static void printaccount(RDArsrc *rsrc,RDA_PFILE *fp,int *lines,int *pages,unsigned line_length)
{
	int accttype=0;
	char *temp=NULL;

	temp=stralloc("XPERT ACCOUNT DEFINITIONS"); 
	printheader(fp,lines,pages,temp,line_length,PRINT_ACCOUNT);
	if(temp!=NULL) Rfree(temp);
	readwidget(rsrc,"ACCOUNT TYPES LIST");
	FINDRSCGETINT(rsrc,"ACCOUNT TYPES LIST",&accttype);
	printaccount1(rsrc,holdacct,fp,lines,pages,line_length,PRINT_ACCOUNT);
}
#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	int x=0;
	RDArsrc *mainrsrc=NULL;

	if(InitializeSubsystems(argc,argv,module,"DEFINE ACCOUNTS"))
	{
		return;
	}
	holdacct=RDAacctNEW(EXP_ACCT);
	mainrsrc=RDArsrcNEW(module,"DEFINE ACCOUNTS");
	if(accavl!=NULL) freeapplib(accavl);
	accavl=APPlibNEW();
	addAPPlib(accavl,accttypes[0]); /* Expeditures */
	addAPPlib(accavl,accttypes[1]); /* Revenues */
	addAPPlib(accavl,accttypes[2]); /* Balance Sheet */
	addAPPlib(accavl,accttypes[3]); /* Beginning Balance */
	addAPPlib(accavl,accttypes[4]); /* Custom Inputs */
	x=0;
	addlstrsrc(mainrsrc,"ACCOUNT TYPES LIST",&x,TRUE,changetypes,
		accavl->numlibs,&accavl->libs,NULL);
	if(defparts!=NULL) freeapplib(defparts);
	defparts=APPlibNEW();
	addlstrsrc(mainrsrc,"DEFINITIONS AVAILABLE",&x,TRUE,NULL,
		defparts->numlibs,&defparts->libs,NULL);
	addbtnrsrc(mainrsrc,"ADD ABOVE",TRUE,adddefabove,NULL);
	addbtnrsrc(mainrsrc,"ADD BELOW",TRUE,adddefbelow,NULL);
	addbtnrsrc(mainrsrc,"SELECT",TRUE,editdefacct,NULL);
	addbtnrsrc(mainrsrc,"COPY ACCT",TRUE,copydefacct,NULL);
	addbtnrsrc(mainrsrc,"DELETE",TRUE,deletedefaccttest,NULL);
	addbtnrsrc(mainrsrc,"SAVE",TRUE,savedfacct,NULL);
	addrfcbrsrc(mainrsrc,"QUIT",TRUE,quitdfaccttest,NULL);
	addbtnrsrc(mainrsrc,"HELP",TRUE,screenhelp,NULL);
/*lint -e611 */
	addrfcbrsrc(mainrsrc,"PRINT ACCOUNT",TRUE,printdefinition,(void *)printaccountfp);
	addrfcbrsrc(mainrsrc,"PRINT ALL",TRUE,printdefinition,(void *)printallfp);
/*lint +e611 */
	addbtnrsrc(mainrsrc,"PRINT RESOURCES",TRUE,screenhelp,NULL);
	changetypes(mainrsrc);
	FINDRSCLISTAPPlib(mainrsrc,"DEFINITIONS AVAILABLE",0,defparts);
	maindefaults=GetDefaults(mainrsrc);
	APPmakescrn(mainrsrc,TRUE,quitdfacct,NULL,TRUE);
}
