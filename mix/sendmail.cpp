#include <mix.hpp>

char *GetWebIDGSV()
{
	RDAGenericSetup *gsv=NULL;
	char *libx=NULL,*WebID=NULL;
		
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("UTILITIES")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif


	gsv=RDAGenericSetupNew("UTILITIES","WEB ID");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			WebID=stralloc(gsv->value.string_value);
		} else {
			WebID=NULL;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);

	return(WebID);
}

short xRunVMimeSendmail(MAIL_VARS *email,int line,char *file)
{
	/* This function should take a mail vars structure and
	 * then run the vmime-sendmail program in a custom
	 * child environment so as to protect the parent environment 
	*/
	APPlib *newenv=NULL;
	APPlib *remove_env=NULL;
	std::string tempstr;

	int x=0;
	int status=0;
	char *temp=NULL;

	newenv=APPlibNEW();
	remove_env=APPlibNEW();
	
	addAPPlib(remove_env,"VMIME_WEBID");
	addAPPlib(remove_env,"VMIME_LOGIN_ID");
	addAPPlib(remove_env,"VMIME_FROM_NAME");
	addAPPlib(remove_env,"VMIME_FROM_ADDR");
	addAPPlib(remove_env,"VMIME_SUBJECT");
	addAPPlib(remove_env,"VMIME_BODY");
	addAPPlib(remove_env,"VMIME_TO");
	addAPPlib(remove_env,"VMIME_CC");
	addAPPlib(remove_env,"VMIME_BCC");
	addAPPlib(remove_env,"VMIME_IGNERR");
	addAPPlib(remove_env,"VMIME_FNAME0");
	addAPPlib(remove_env,"VMIME_FNAME1");
	addAPPlib(remove_env,"VMIME_FNAME2");
	addAPPlib(remove_env,"VMIME_FNAME3");
	addAPPlib(remove_env,"VMIME_FNAME4");
	addAPPlib(remove_env,"VMIME_FNAME5");
	addAPPlib(remove_env,"VMIME_FNAME6");
	addAPPlib(remove_env,"VMIME_FNAME7");
	addAPPlib(remove_env,"VMIME_FNAME8");
	addAPPlib(remove_env,"VMIME_FNAME9");

	addAPPlib(remove_env,"VMIME_DNAME0");
	addAPPlib(remove_env,"VMIME_DNAME1");
	addAPPlib(remove_env,"VMIME_DNAME2");
	addAPPlib(remove_env,"VMIME_DNAME3");
	addAPPlib(remove_env,"VMIME_DNAME4");
	addAPPlib(remove_env,"VMIME_DNAME5");
	addAPPlib(remove_env,"VMIME_DNAME6");
	addAPPlib(remove_env,"VMIME_DNAME7");
	addAPPlib(remove_env,"VMIME_DNAME8");
	addAPPlib(remove_env,"VMIME_DNAME9");

	addAPPlib(remove_env,"VMIME_MTYPE0");
	addAPPlib(remove_env,"VMIME_MTYPE1");
	addAPPlib(remove_env,"VMIME_MTYPE2");
	addAPPlib(remove_env,"VMIME_MTYPE3");
	addAPPlib(remove_env,"VMIME_MTYPE4");
	addAPPlib(remove_env,"VMIME_MTYPE5");
	addAPPlib(remove_env,"VMIME_MTYPE6");
	addAPPlib(remove_env,"VMIME_MTYPE7");
	addAPPlib(remove_env,"VMIME_MTYPE8");
	addAPPlib(remove_env,"VMIME_MTYPE9");

	addAPPlib(remove_env,"VMIME_FDATA0");
	addAPPlib(remove_env,"VMIME_FDATA1");
	addAPPlib(remove_env,"VMIME_FDATA2");
	addAPPlib(remove_env,"VMIME_FDATA3");
	addAPPlib(remove_env,"VMIME_FDATA4");
	addAPPlib(remove_env,"VMIME_FDATA5");
	addAPPlib(remove_env,"VMIME_FDATA6");
	addAPPlib(remove_env,"VMIME_FDATA7");
	addAPPlib(remove_env,"VMIME_FDATA8");
	addAPPlib(remove_env,"VMIME_FDATA9");


	if(email->srcdomain!=NULL)
	{
		tempstr.assign("VMIME_SRCDOMAIN=");
		tempstr.append(email->srcdomain);
		addAPPlib(newenv,tempstr.c_str());
		tempstr.clear();
	}
	if(USERLOGIN!=NULL)
	{
		tempstr.assign("VMIME_LOGIN_ID=");
		tempstr.append(USERLOGIN);
		addAPPlib(newenv,tempstr.c_str());
		tempstr.clear();
	}
	if(email->from_name!=NULL)
	{
		tempstr.assign("VMIME_FROM_NAME=");
		tempstr.append(email->from_name);
		addAPPlib(newenv,tempstr.c_str());
		tempstr.clear();
	}
	if(email->from_addr!=NULL)
	{
		tempstr.assign("VMIME_FROM_ADDR=");
		tempstr.append(email->from_addr);
		addAPPlib(newenv,tempstr.c_str());
		tempstr.clear();
	}
	if(email->subject!=NULL)
	{
		tempstr.assign("VMIME_SUBJECT=");
		tempstr.append(email->subject);
		addAPPlib(newenv,tempstr.c_str());
		tempstr.clear();
	}
	if(email->body!=NULL)
	{
		tempstr.assign("VMIME_BODY=");
		tempstr.append(email->body);
		addAPPlib(newenv,tempstr.c_str());
		tempstr.clear();
	}
	if(email->tolist!=NULL)
	{
		tempstr.assign("VMIME_TO=");
		tempstr.append(email->tolist);
		addAPPlib(newenv,tempstr.c_str());
		tempstr.clear();
	}
	if(email->cclist!=NULL)
	{
		tempstr.assign("VMIME_CC=");
		tempstr.append(email->cclist);
		addAPPlib(newenv,tempstr.c_str());
		tempstr.clear();
	}
	if(email->bcclist!=NULL)
	{
		tempstr.assign("VMIME_BCC=");
		tempstr.append(email->bcclist);
		addAPPlib(newenv,tempstr.c_str());
		tempstr.clear();
	}
	if(email->ignerr!=0)
	{
		addAPPlib(newenv,"VMIME_IGNERR=1");
	}
	for(x=0;x<10;x++)
	{
		if(email->fname[x]!=NULL)
		{
			tempstr.assign("VMIME_FNAME");
			tempstr.append(x);
			tempstr.append("=");
			tempstr.append(email->fname[x]);
			addAPPlib(newenv,tempstr.c_str());
			tempstr.clear();
		}
		if(email->dname[x]!=NULL)
		{
			tempstr.assign("VMIME_DNAME");
			tempstr.append(x);
			tempstr.append("=");
			tempstr.append(email->dname[x]);
			addAPPlib(newenv,tempstr.c_str());
			tempstr.clear();
		}
		if(email->mtype[x]!=NULL)
		{
			tempstr.assign("VMIME_MTYPE");
			tempstr.append(x);
			tempstr.append("=");
			tempstr.append(email->mtype[x]);
			addAPPlib(newenv,tempstr.c_str());
			tempstr.clear();
		}
		if(email->fdata[x]!=NULL)
		{
			tempstr.assign("VMIME_FDATA");
			tempstr.append(x);
			tempstr.append("=");
			tempstr.append(email->fdata[x]);
			addAPPlib(newenv,tempstr.c_str());
			tempstr.clear();
		}
	}

	x=RDA_NOGUI;
	RDA_NOGUI=TRUE;
	status=ADV4Execute2Program("vmime-sendmail",NULL,newenv,TRUE,FALSE,remove_env);
	RDA_NOGUI=x;
	prterr("\nvmime-sendmail exited with code [%d]\n",status);
	if(newenv!=NULL) freeapplib(newenv);
	if(remove_env!=NULL) freeapplib(remove_env);
	return(status);
}
