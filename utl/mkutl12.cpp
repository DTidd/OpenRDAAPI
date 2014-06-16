/* UTILITIES's - Make (DOS COPY WINDOW) Screen */
#include <cstdio>
#include <mkmsc.hpp>

extern char *module;
extern APPlib *errorlist;
void DOS_COPY_WINDOW(void);

void DOS_COPY_WINDOW()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("UTILITIES","COPY FILE WINDOW");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,"((GENERIC_SETUP_VARIABLE(\"[UTILITIES][DISPLAY DOSCOPY MEDIA OPTIONS]\")))",NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Device Spec:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DEVICE SPECIFICATION","Device Specification","","",0,20,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Filename:","","",0,0,0,"!((GENERIC_SETUP_VARIABLE(\"[UTILITIES][DISPLAY DOSCOPY MEDIA OPTIONS]\")))",NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","From Filename:","","",0,0,0,"((GENERIC_SETUP_VARIABLE(\"[UTILITIES][DISPLAY DOSCOPY MEDIA OPTIONS]\")))",NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"FROM FILE NAME","From Filename","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","To Filename:","","",0,0,0,"((GENERIC_SETUP_VARIABLE(\"[UTILITIES][DISPLAY DOSCOPY MEDIA OPTIONS]\")))",NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"TO FILE NAME","To Filename","","",0,40,0,"((GENERIC_SETUP_VARIABLE(\"[UTILITIES][DISPLAY DOSCOPY MEDIA OPTIONS]\")))",NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Error List","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"ERROR LIST","Error List","","",5,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"DOWNLOAD","Download","","",0,0,45,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"COPY FILE","Copy File","","",0,0,49,"((GENERIC_SETUP_VARIABLE(\"[UTILITIES][DISPLAY DOSCOPY MEDIA OPTIONS]\")))",NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"ZIP FILE","Zip File","","",0,0,28,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"URL","URL (Internet)","","",0,0,46,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"FORMAT MEDIA","Format Media","","",0,0,0,"((GENERIC_SETUP_VARIABLE(\"[UTILITIES][DISPLAY DOSCOPY MEDIA OPTIONS]\")))",NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"QUIT","Cancel","","",0,0,14,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"HELP","Help","","",0,0,11,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);

		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+12);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.SCN",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,module);
#endif
		if(writescrnbin(defdir,scrn))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+9+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [UTILITIES] Screen [DOS COPY WINDOW], Can Not Save Maintain Master!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(scrn!=NULL) free_scrn(scrn);
	}
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(temp3!=NULL) Rfree(temp3);
	if(temp4!=NULL) Rfree(temp4);
}
