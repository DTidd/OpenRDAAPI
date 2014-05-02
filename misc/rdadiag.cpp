/*rdadiag.c - function for setting up diagnostic messages from env variable*/
#include <misc.hpp>
FILE *RDA_STDERR=NULL;
char *(*UI_RDA_GetEnv)(char *,int,char *);
int (*UI_RDA_SetEnv)(char *,char *,int,char *);
int (*UI_RDA_UnSetEnv)(char *,int,char *);
int (*UI_RDA_PutEnv)(char *,int,char *);

char *(*BCryptValue)(char *);

/*put diag test variables here*/
char diagalloc=0;
char diaggui=0;
char diagnrd=0;
char diagmyisam=0;
char diagmysql=0;
char diagmix=0;
char diagbin=0;
char diagproc=0;
char diagolh=0;
char diagsec=0;
char diagblb=0;
char diagtrans=0;
char diagrptgen=0;
char diageval=0;
char diagvf=0;
char diagrf=0;
char diagrdadta=0;
char diagallocx=0;
char diagmisc=0;
char diagbadalloc=0;
char diagimport=0;
char diagvirtual=0;
char diagexeprog=0;
char diagrptgen_slteval=0;
char diagrptgen_select=0;
char diagrptgen_sltall=0;
char diagrptgen_sltrec=0;
char diagrptgen_outall=0;
char diagrptgen_outexp=0;
char diagrptgen_output=0;
char diagrptgen_outline=0;
char diagrptgen_field=0;
char diaggui_field=0;
char diagnrd_field=0;
char diageval_field=0;
char diagsec_field=0;
char diagvirtual_field=0;
char diagimport_field=0;
char diagolh_field=0;
char diagmix_field=0;
char diagywin=0;
char diagapps=0;
char diagtable=0;
char diagmatrix=0;
char diaglist=0;
char diaginformix_isam=0;
char diaginformix_standard=0;
char diagstandard_sql=0;
char diagibm_db2=0;
char diagadd=0;
char diagprt=0;
char diaglic=0;
char diagcss=0;
char AlreadyInitialized=FALSE;


#ifdef WIN32
int xWIN32TRACE(int line,char *file)
{
	FILE *fp=NULL;
	char userid[41];
	char temp[101];
	DWORD l=40;

	GetUserName(userid,&l);
	sprintf(temp,"%s.ERR",userid);
	
	if(NULL==(fp=fopen(temp,"a+b"))) 
	{
		prterr("Cannot open TRACE log file.\n");
		return(1);
	}else{
		fprintf(fp,"TRACE:   FILE:  [%s]   LINE:  [%d]\r\n",file,line);
		fclose(fp);
		return(0);
	}
}

void RotateErrorLog()
{
	char userid[41];
	char temp1[101];
	char temp2[101];
	DWORD l=40;
	GetUserName(userid,&l);
	sprintf(temp1,"%s.ERR",userid);
	sprintf(temp2,"%s-last.ERR",userid);
	if(FileExists(temp1))
	{
		RDAMoveFile(temp1,temp2);
	}
}
#endif

void initrdadiag()
{
	char *tmp;
	char *x;

	if(RDA_NOGUI)
	{
		AlreadyInitialized=FALSE;
		UI_RDA_GetEnv=NULL;
		UI_RDA_SetEnv=NULL;
		UI_RDA_UnSetEnv=NULL;
		UI_RDA_PutEnv=NULL;
		UI_GetDocRoot=NULL;
		UI_GetAppRoot=NULL;
		UI_OpenWindow=NULL;
		UI_OpenWindowWait=NULL;
		BCryptValue=NULL;
	}
#ifdef WIN32
	RDA_STDERR=NULL;
#endif
#ifndef WIN32
	RDA_STDERR=stderr;
#endif
	tmp=RDA_GetEnv("RDADIAG");
	if(tmp!=NULL)
	{
		x=strtok(tmp," ");
		while(x!=NULL)
		{
			/*put test for env variable here*/
			if(!RDAstrcmp(x,"ALLOC")) diagalloc=1;
			if(!RDAstrcmp(x,"ALLOCX")) diagallocx=1;
			if(!RDAstrcmp(x,"BADALLOC")) diagbadalloc=1;
			if(!RDAstrcmp(x,"CSS")) diagcss=TRUE;
			if(!RDAstrcmp(x,"GUI")) diaggui=1;
			if(!RDAstrcmp(x,"NRD")) diagnrd=1;
			if(!RDAstrcmp(x,"MIX")) diagmix=1;
			if(!RDAstrcmp(x,"BIN")) diagbin=1;
			if(!RDAstrcmp(x,"BINLIB")) diagblb=1;
			if(!RDAstrcmp(x,"PROC")) diagproc=1;
			if(!RDAstrcmp(x,"SEC")) diagsec=1;
			if(!RDAstrcmp(x,"OLH")) diagolh=1;
			if(!RDAstrcmp(x,"TRANS")) diagtrans=1;
			if(!RDAstrcmp(x,"RPTGEN")) diagrptgen=1;
			if(!RDAstrcmp(x,"EVAL")) diageval=1;
			if(!RDAstrcmp(x,"VF")) diagvf=1;
			if(!RDAstrcmp(x,"RF")) diagrf=1;
			if(!RDAstrcmp(x,"RDADTA")) diagrdadta=1;
			if(!RDAstrcmp(x,"MISC")) diagmisc=1;
			if(!RDAstrcmp(x,"IMPORT")) diagimport=1;
			if(!RDAstrcmp(x,"VIRTUAL")) diagvirtual=1;
			if(!RDAstrcmp(x,"EXEPROG")) diagexeprog=1;
			if(!RDAstrcmp(x,"RPTGEN_SLTEVAL")) diagrptgen_slteval=1;
			if(!RDAstrcmp(x,"RPTGEN_SLTALL")) diagrptgen_sltall=1;
			if(!RDAstrcmp(x,"RPTGEN_SELECT")) diagrptgen_select=1;
			if(!RDAstrcmp(x,"RPTGEN_SLTREC")) diagrptgen_sltrec=1;
			if(!RDAstrcmp(x,"RPTGEN_OUTALL")) diagrptgen_outall=1;
			if(!RDAstrcmp(x,"RPTGEN_OUTEXP")) diagrptgen_outexp=1;
			if(!RDAstrcmp(x,"RPTGEN_OUTPUT")) diagrptgen_output=1;
			if(!RDAstrcmp(x,"RPTGEN_OUTLINE")) diagrptgen_outline=1;
			if(!RDAstrcmp(x,"FIELD_RPTGEN")) diagrptgen_field=1;
			if(!RDAstrcmp(x,"FIELD_GUI")) diaggui_field=1;
			if(!RDAstrcmp(x,"FIELD_NRD")) diagnrd_field=1;
			if(!RDAstrcmp(x,"FIELD_EVAL")) diageval_field=1;
			if(!RDAstrcmp(x,"FIELD_SEC")) diagsec_field=1;
			if(!RDAstrcmp(x,"FIELD_VIRTUAL")) diagvirtual_field=1;
			if(!RDAstrcmp(x,"FIELD_IMPORT")) diagimport_field=1;
			if(!RDAstrcmp(x,"FIELD_OLH")) diagolh_field=1;
			if(!RDAstrcmp(x,"FIELD_MIX")) diagmix_field=1;
			if(!RDAstrcmp(x,"YWIN")) diagywin=1;
			if(!RDAstrcmp(x,"APPLICATION")) diagapps=1;
			if(!RDAstrcmp(x,"TABLE")) diagtable=1;
			if(!RDAstrcmp(x,"MATRIX")) diagmatrix=1;
			if(!RDAstrcmp(x,"LIST")) diaglist=1;
			if(!RDAstrcmp(x,"INFORMIX_ISAM")) diaginformix_isam=1;
			if(!RDAstrcmp(x,"INFORMIX_STANDARD")) diaginformix_standard=1;
			if(!RDAstrcmp(x,"STANDARD_SQL")) diagstandard_sql=1;
			if(!RDAstrcmp(x,"IBM_DB2")) diagibm_db2=1;
			if(!RDAstrcmp(x,"POWERADD")) diagadd=1;
			if(!RDAstrcmp(x,"RDAPRT")) diagprt=1;
			if(!RDAstrcmp(x,"RDALIC")) diaglic=1;
			if(!RDAstrcmp(x,"MYISAM")) diagmyisam=1;
			if(!RDAstrcmp(x,"MYSQL")) diagmysql=1;
			x=strtok(NULL," ");
		}
	}
/*
	diageval=TRUE;TRACE;
	diagalloc=TRUE;diaggui=TRUE;diageval=TRUE;diagnrd=TRUE;diagmix=TRUE;diagvirtual=TRUE;diagsec=TRUE;;TRACE;
	diaggui=TRUE;diageval=TRUE;diagnrd=TRUE;diagmix=TRUE;diagvirtual=TRUE;diagcss=TRUE;TRACE;
	diaggui=TRUE;diageval=TRUE;diagnrd=TRUE;diagmix=TRUE;diagvirtual=TRUE;;TRACE;
	diaggui=TRUE;diageval=TRUE;diagnrd=TRUE;diagmix=TRUE;diagvirtual=TRUE;diagsec=TRUE;diagmisc=TRUE;TRACE;
	diagcss=TRUE;
	diagalloc=TRUE;diaggui=TRUE;diageval=TRUE;diagnrd=TRUE;diagmix=TRUE;diagvirtual=TRUE;diagsec=TRUE;diagmisc=TRUE;;TRACE;
	diaggui=TRUE;TRACE;
*/
}

