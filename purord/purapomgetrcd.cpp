void getrecord(RDArsrc *mtnrsrc,RDATData *previous)
{
	int x=0;
	char *siteid=NULL,*temp=NULL;

/* removing if locked record previously, intentionally before readallwidgets */
	FINDRSCGETSTRING(mtnrsrc,"[PURMST][PO IDENTIFICATION]",&temp);
	ADVUndoIDifnoPO(purids,msrnum,temp,SCRNvirtualSubData,mtnrsrc);
	if(temp!=NULL) Rfree(temp);
	readallwidgets(mtnrsrc);
	FINDRSCGETINT(mtnrsrc,"[PURMST][FISCAL YEAR]",&yearid);
	if(poid!=NULL) Rfree(poid);
	FINDRSCGETSTRING(mtnrsrc,"[PURMST][PO IDENTIFICATION]",&poid);
	ZERNRD(msrnum);
	FINDFLDSETINT(msrnum,"FISCAL YEAR",yearid);
	FINDFLDSETSTRING(msrnum,"PO IDENTIFICATION",poid);
	seteditfilersrc(msrnum,1,mtnrsrc,TRUE,FALSE);
	if(ADVEQLNRDsec(msrnum,1,SCRNvirtualSubData,mtnrsrc))
	{
		KEYNRD(msrnum,1);
		ClearRDATData(previous);
		if(ADVCheckPOID(purids,poid,SCRNvirtualSubData,mtnrsrc))
		{
			ADVAssignPOID(purids,poid,SCRNvirtualSubData,mtnrsrc);	
		} else {
			temp=ADVGetAssignNextPOID(purids,poid,SCRNvirtualSubData,mtnrsrc);
			if(poid!=NULL) Rfree(poid);
			poid=NULL;
			if(temp!=NULL)
			{
				poid=stralloc(temp);
				Rfree(temp);
			}
			FINDFLDSETSTRING(msrnum,"PO IDENTIFICATION",poid);
			FINDRSCSETSTRING(mtnrsrc,"[PURMST][PO IDENTIFICATION]",poid);
			updatersrc(mtnrsrc,"[PURMST][PO IDENTIFICATION]");
			if(!isEMPTY(poid))
			{
				getrecord(mtnrsrc,previous);
			} else {
				WARNINGDIALOG("WARNING DIALOG SCREEN","INVALID PO IDENTIFICATION!","There are no valid PO Identifications Available.  Please create more PO Identifications.",NULL,NULL,TRUE,NULL);
			}
		}
	} else {
		FINDFLDGETSTRING(msrnum,"SITE IDENTIFICATION",&siteid);
		if((x=FINDAPPLIBELEMENT(sitesavl,siteid))==(-1)) x=0;
		if(siteid!=NULL) Rfree(siteid);
		GetRDATData(msrnum,previous);
		filerecord2rsrc(msrnum,mtnrsrc);
	}	
	FINDRSCSETINT(mtnrsrc,"SITE IDENTIFICATIONS",x);
	getsupportingrecords(mtnrsrc,FALSE);
	setaddresses(mtnrsrc);
	computeallSCRNvirtuals(mtnrsrc);
	updateallrsrc(mtnrsrc);
}
