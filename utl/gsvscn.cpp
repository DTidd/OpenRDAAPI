/* dogsv.c - Do Generic Setup Variables */
/*lint -library */
#ifndef WIN32
#define __NAM__ "gsvscn.lnx"
#endif
#ifdef WIN32
#define __NAM__ "gsvscn.exe"
#endif
#include <app.hpp>


#include <mix.hpp>
/*CPP_OPART gsvcm */
/*ADDLIB mix */
/*ADDLIB olh */
/*ADDLIB sec */
/*ADDLIB trans */
/*ADDLIB gui */
/*ADDLIB nrd */
/*ADDLIB misc */

#define GetAllSetupVariables(a)	xGetAllSetupVariables(a,__LINE__,__FILE__)
extern void xGetAllSetupVariables(char *,int,char *);

#ifdef CPPMAIN
int c_main(int argc,char **argv)
#else
int main(int argc,char **argv)
#endif
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,udesc=FALSE;
	RDAGenericSetup *g=NULL;
	int x=0;

	if(argc<2) std::exit;
	if(argc>2)
	{
		if(!RDAstrcmp(argv[2],"TRUE") || !RDAstrcmp(argv[2],"true")) udesc=TRUE;
	} 
	if(InitializeSubsystems(argc,argv,argv[1],"MAKE GENERIC SETUP SCREEN"))
	{
		return;
	}
	GetAllSetupVariables(argv[1]);

	scrn=RDAscrnNEW(argv[1],"GENERIC SETUP VARIABLES");
	if(scrn!=NULL)
	{
		if(Internal_RDAGenericSetup!=NULL)
		{
			for(x=0,g=Internal_RDAGenericSetup;x<Internal_num_RDAGenericSetup;++x,++g)
			{
				if(udesc)
				{
					ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
					ADVaddwdgt(scrn,5,"",g->desc,"","",0,0,0,"","","","");
					ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
				}
				ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
				switch(g->type)
				{
					case SCROLLEDTEXT:
						ADVaddwdgt(scrn,5,"",g->label,"","",0,0,0,"","","","");
						ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
						ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
						addwdgt(scrn,0,g->name,g->name,NULL,NULL,5,g->length,0,NULL,NULL,NULL);
						break;
					case VARIABLETEXT:
					case PLAINTEXT:
					case DATES:
					case TIMES:
					case SOCSECNUM:
					case PHONE:
					case CUSTOMTYPE:
					case ZIPCODE:
					case EXPENDITURE:
					case REVENUE:
					case BALANCESHEET:
					case BEGINNINGBALANCE:
					case OPTIONALFIELDS:
						ADVaddwdgt(scrn,5,"",g->label,"","",0,0,0,"","","","");
						addwdgt(scrn,0,g->name,g->name,NULL,NULL,0,g->length,0,NULL,NULL,NULL);
						break;
					case SHORTV:
					case SSHORTV:
						ADVaddwdgt(scrn,5,"",g->label,"","",0,0,0,"","","","");
						addwdgt(scrn,0,g->name,g->name,NULL,NULL,0,g->length,0,NULL,NULL,NULL);
						break;
					case LONGV:
					case SLONGV:
						ADVaddwdgt(scrn,5,"",g->label,"","",0,0,0,"","","","");
						addwdgt(scrn,0,g->name,g->name,NULL,NULL,0,g->length,0,NULL,NULL,NULL);
						break;
					case DOLLARS:
					case DOLLARS_NOCENTS:
					case DECIMALV:
					case DOUBLEV:
					case SDOUBLEV:
					case SDECIMALV:
						ADVaddwdgt(scrn,5,"",g->label,"","",0,0,0,"","","","");
						addwdgt(scrn,0,g->name,g->name,NULL,NULL,0,g->length,0,NULL,NULL,NULL);
						break;
					case BOOLNS:
						addwdgt(scrn,9,g->name,g->label,NULL,NULL,0,1,0,NULL,NULL,NULL);
						break;
					case CHARACTERS:
						ADVaddwdgt(scrn,5,"",g->label,"","",0,0,0,"","","","");
						addwdgt(scrn,0,g->name,g->name,NULL,NULL,0,g->length,0,NULL,NULL,NULL);
						break;
					default:
						prterr("Error Unrecognized Field type [%d] for resource [%s] not added to Screen.",g->type,g->name); 
						break;
				}
				ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
			}
		}		
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,3,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,6,"SAVE","Save","","",0,0,23,"","","","");
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,"","","","");
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,4,"","","","",0,0,0,"","","","");
		ADVaddwdgt(scrn,2,"","","","",0,0,0,"","","","");

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(argv[1])+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,argv[1]);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,argv[1]);
#endif
		if(writescrnbin(defdir,scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(RDAstrlen(scrn->module)+RDAstrlen(scrn->name)+100+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [%s] Screen [%s], Can Not Save Screen!",(scrn->module!=NULL?scrn->module:""),(scrn->name!=NULL?scrn->name:""));
			ERRORDIALOG("Cannot Save Screen",temp1,NULL,FALSE);
			prterr(temp1);
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
	ShutdownSubsystems();
	std::exit;
}
