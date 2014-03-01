
void makeMaintainScreen(MaintainMaster *MTN)
{
	RDAscrn *SCREEN=NULL;
	char *lib=NULL,*temp1=NULL,*temp=NULL,*temp2=NULL;
	char *rscrlabel=NULL;
	int x=0,y=0,total_len=0;
	NRDfield *field=NULL;
	NRDkey *key=NULL;
	NRDpart *part=NULL;
	short fileno=(-1);
	MaintainButton *button=NULL;

	fileno=OPNNRD(MTN->module,MTN->mainfile);
	if(fileno==(-1))
	{
		prterr("Error:  Unable to open file [%s][%s].",MTN->module,MTN->mainfile);
		return;
	}
	SCREEN=RDAscrnNEW(MTN->module,MTN->maintain_screen);

	field=FLDNRD(fileno,"DELETEFLAG");
	if(field!=NULL)
	{
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		temp=ReturnScreenName(MTN->mainfile,field);
		addwdgt(SCREEN,9,temp,"Delete this record",NULL,NULL,0,0,0,NULL,NULL,NULL);
		if(temp!=NULL) Rfree(temp);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}

	key=KEYPOINTER(fileno);
	if(key!=NULL)
	{
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		total_len=0;
		for(y=0,part=key->part;y<key->numparts;++y,++part)
		{
			if(RDAstrcmp(part->name,"DELETEFLAG"))
			{
				field=FLDNRD(fileno,part->name);
				if(field!=NULL)
				{
					if(total_len>80)
					{
						addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						total_len=0;
					}	
					temp=ReturnScreenName(MTN->mainfile,field);
					rscrlabel=ReturnScreenLabel(field->name);
					total_len+=RDAstrlen(rscrlabel);
					temp1=Rmalloc(RDAstrlen(rscrlabel)+10);
					sprintf(temp1,"%s:",rscrlabel);
					addwdgt(SCREEN,5,NULL,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
					if((y+1)==key->numparts)
					{
						temp2=Rmalloc(RDAstrlen(key->name)+10);
						sprintf(temp2,"PREVIOUS %s",key->name);
						addwdgt(SCREEN,6,temp2,"Previous",NULL,NULL,0,0,2,NULL,NULL,NULL);
						if(temp2!=NULL) Rfree(temp2);
						total_len+=10;
					}
					if(field->len<20)
					{
						addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
					} else if((y+1<key->numparts))
					{
						addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
						addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
						total_len=0;
					} else {
						addwdgt(SCREEN,0,temp,temp1,NULL,NULL,0,field->len,0,NULL,NULL,NULL);
					}
					total_len+=field->len;
					if((y+1)==key->numparts)
					{
						temp2=Rmalloc(RDAstrlen(key->name)+10);
						sprintf(temp2,"NEXT %s",key->name);
						addwdgt(SCREEN,6,temp2,"Next",NULL,NULL,0,0,1,NULL,NULL,NULL);
						if(temp2!=NULL) Rfree(temp2);
						total_len+=10;
					}
					if(temp!=NULL) Rfree(temp);
					if(temp1!=NULL) Rfree(temp1);
					if(rscrlabel!=NULL) Rfree(rscrlabel);
				}
			}
		}
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}







	if(fileno!=(-1)) CLSNRD(fileno);
	if(MTN->buttons!=NULL && MTN->numbuttons>1)
	{
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"BROWSE","Browse",NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"RESET DEFAULTS","Reset",NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"DEFAULTS","Save Defaults",NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);


		if((MTN->buttons!=NULL) && MTN->numbuttons>1)
		{
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			for(x=0,button=MTN->buttons;x<MTN->numbuttons;++x,++button)
			{
				temp1=ReturnScreenLabel((button->listname!=NULL ? button->listname:button->buttonname));
				addwdgt(SCREEN,6,button->buttonname,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
				if(temp1!=NULL) Rfree(temp1);
			}
			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
			addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		}



		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"SAVE","Save",NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"SAVE NO UPDATE","Fast Save",NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"QUIT","Cancel",NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	} else {
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"BROWSE","Browse",NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"RESET DEFAULTS","Reset",NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"DEFAULTS","Save Defaults",NULL,NULL,0,0,0,NULL,NULL,NULL);
		if((MTN->buttons!=NULL) && MTN->numbuttons==1)
		{
			button=MTN->buttons;
			temp1=ReturnScreenLabel((button->listname!=NULL ? button->listname:button->buttonname));
			addwdgt(SCREEN,6,button->buttonname,temp1,NULL,NULL,0,0,0,NULL,NULL,NULL);
			if(temp1!=NULL) Rfree(temp1);
		}
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,3,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,1,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"SAVE","Save",NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"SAVE NO UPDATE","Fast Save",NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"QUIT","Cancel",NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,6,"HELP","Help",NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,4,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
		addwdgt(SCREEN,2,NULL,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL);
	}

	lib=Rmalloc(RDAstrlen(MTN->module)+RDAstrlen(CURRENTDIRECTORY)+11);
#ifndef WIN32
	sprintf(lib,"%s/rda/%s.SCN",CURRENTDIRECTORY,MTN->module);
#endif
#ifdef WIN32
	sprintf(lib,"%s\\rda\\%s.SCN",CURRENTDIRECTORY,MTN->module);
#endif
	if(writescrnbin(lib,SCREEN))
	{
		ERRORDIALOG("Cannot Save Screen","This screen cannot be saved to this module's rda directory.  Please check permissions and retry.",NULL,FALSE);		
	}
	if(SCREEN!=NULL) free_scrn(SCREEN);
}
