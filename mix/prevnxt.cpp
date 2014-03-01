#include<mix.hpp>

void xGET_PREVIOUS(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER,RDATData *prevmain,
	void (*SubFunc)(...),void *args,short fileno,short keyno,
	short inheritrsrcs,
	void (*NEfunc)(...),void *funcarg1,
	void (*BUEQLfunc)(...),void *funcarg2,
	void (*AUEQLfunc)(...),void *funcarg3,
	int line,char *file)
{
	short key2use=1;

	if(MTNMASTER!=NULL)
	{
		if(KEYNUM(MTNMASTER->passkey->fileno,keyno)!=NULL)
		{
			key2use=keyno;
		} else { 
			key2use=1;
		}
		fileno=MTNMASTER->passkey->fileno;
	} else {
		if(KEYNUM(fileno,keyno)!=NULL)
		{
			key2use=keyno;
		} else { 
			key2use=1;
		}
	}
	readallwidgets(mtnrsrc);
	if(MTNMASTER!=NULL)
	{
		if(!MTNMASTER->file_type)
		{
			rsrc2filerecord(fileno,mtnrsrc);
		} else {
			rsrc2singlefilerecord(fileno,mtnrsrc);
		}
	} else {
		rsrc2filerecord(fileno,mtnrsrc);
	}
	ReadRDAScrolledLists(mtnrsrc);
	if(ADVPRVNRDsec(fileno,key2use,SubFunc,args))
	{
		if(inheritrsrcs) 
		{
			KEYNRD(fileno,key2use);
			if(prevmain!=NULL) ClearRDATData(prevmain);
			if(MTNMASTER!=NULL)
			{
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
			} else {
				ReadRDAScrolledLists(mtnrsrc);
			}
			if(MTNMASTER!=NULL)
			{
				if(!MTNMASTER->file_type)
				{
					updatefilersrc(fileno,mtnrsrc);
					rsrc2filerecord(fileno,mtnrsrc);
				} else {
					updatesinglefilersrc(fileno,mtnrsrc);
					rsrc2singlefilerecord(fileno,mtnrsrc);
				}
			} else {
				updatefilersrc(fileno,mtnrsrc);
				rsrc2filerecord(fileno,mtnrsrc);
			}
		} else { 
/*
			KEYNRD(fileno,key2use);
*/
			ZERNRD(fileno);
			if(prevmain!=NULL) ClearRDATData(prevmain);
			if(MTNMASTER!=NULL)
			{
				ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
			} else {
				ReadRDAScrolledLists(mtnrsrc);
			}
			if(MTNMASTER!=NULL)
			{
				if(!MTNMASTER->file_type)
				{
#ifdef __GTK_H__
					filerecord2rsrc(fileno,mtnrsrc);
					updateallrsrc(mtnrsrc);
#endif /* __GTK_H__ */
#ifndef __GTK_H__
					updatefilerecord2rsrc(fileno,mtnrsrc);	
#endif /* NOT __GTK_H__ */
				} else {
					updatesinglerecord2rsrc(fileno,mtnrsrc);
				}
			} else {
				updatefilerecord2rsrc(fileno,mtnrsrc);
			}
		}
		if(NEfunc!=NULL)
		{
/*lint -e746 */
			if(MTNMASTER!=NULL)
			{
				NEfunc(mtnrsrc,MTNMASTER,SubFunc,args,funcarg1);
			} else {
				NEfunc(mtnrsrc,SubFunc,args,funcarg1);
			}	
/*lint +e746 */
		}
		if(MTNMASTER!=NULL)
		{
			GET_SUPPORTING(mtnrsrc,MTNMASTER,SubFunc,args,TRUE,FALSE);
		}
	} else {
		if(BUEQLfunc!=NULL)
		{
/*lint -e746 */
			if(MTNMASTER!=NULL)
			{
				BUEQLfunc(mtnrsrc,MTNMASTER,SubFunc,args,funcarg2);
			} else {
				BUEQLfunc(mtnrsrc,SubFunc,args,funcarg2);
			}
/*lint +e746 */
		}
		if(prevmain!=NULL) GetRDATData(fileno,prevmain);
		if(AUEQLfunc!=NULL)
		{
/*lint -e746 */
			if(MTNMASTER!=NULL)
			{
				AUEQLfunc(mtnrsrc,MTNMASTER,SubFunc,args,funcarg3);
			} else {
				AUEQLfunc(mtnrsrc,SubFunc,args,funcarg3);
			}
/*lint +e746 */
		} else {
			if(MTNMASTER!=NULL)
			{
				if(!MTNMASTER->file_type)
				{
#ifdef __GTK_H__
					filerecord2rsrc(fileno,mtnrsrc);
					updateallrsrc(mtnrsrc);
#endif /* __GTK_H__ */
#ifndef __GTK_H__
					updatefilerecord2rsrc(fileno,mtnrsrc);	
#endif /* NOT __GTK_H__ */
				} else {
					updatesinglerecord2rsrc(fileno,mtnrsrc);	
				}
			} else {
				updatefilerecord2rsrc(fileno,mtnrsrc);	
			}
		}
		if(MTNMASTER!=NULL)
		{
			GET_SUPPORTING(mtnrsrc,MTNMASTER,SubFunc,args,TRUE,TRUE);
		}
	}
	if(MTNMASTER!=NULL)
	{
		MSTupdateSCRNvirtuals(mtnrsrc,MTNMASTER);
	} else {
		updateSCRNvirtuals(mtnrsrc);
	}
}
void xGET_NEXT(RDArsrc *mtnrsrc,MaintainMaster *MTNMASTER,RDATData *prevmain,
	void (*SubFunc)(...),void *args,short fileno,short keyno,
	short inheritrsrcs,
	void (*NEfunc)(...),void *funcarg1,
	void (*BUEQLfunc)(...),void *funcarg2,
	void (*AUEQLfunc)(...),void *funcarg3,
	int line,char *file)
{
	short key2use=1;

	if(MTNMASTER!=NULL)
	{
		if(KEYNUM(MTNMASTER->passkey->fileno,keyno)!=NULL) key2use=keyno;
			else key2use=1;
		fileno=MTNMASTER->passkey->fileno;
	} else {
		if(KEYNUM(fileno,keyno)!=NULL) key2use=keyno;
			else key2use=1;
	}
	readallwidgets(mtnrsrc);
	if(MTNMASTER!=NULL)
	{
		if(!MTNMASTER->file_type)
		{
			rsrc2filerecord(fileno,mtnrsrc);
		} else {
			rsrc2singlefilerecord(fileno,mtnrsrc);
		}
	} else {
		rsrc2filerecord(fileno,mtnrsrc);
	}
	ReadRDAScrolledLists(mtnrsrc);
	if(ADVNXTNRDsec(fileno,key2use,SubFunc,args))
	{
		KEYNRD(fileno,key2use);
		if(prevmain!=NULL) ClearRDATData(prevmain);
		if(MTNMASTER!=NULL)
		{
			ReadRDAScrolledListsNoPassKeys(mtnrsrc,MTNMASTER->passkey);
		} else {
			ReadRDAScrolledLists(mtnrsrc);
		}
		if(inheritrsrcs) 
		{
			if(MTNMASTER!=NULL)
			{
				if(!MTNMASTER->file_type)
				{
					updatefilersrc(fileno,mtnrsrc);
					rsrc2filerecord(fileno,mtnrsrc);
				} else {
					updatesinglefilersrc(fileno,mtnrsrc);
					rsrc2singlefilerecord(fileno,mtnrsrc);
				}
			} else {
				updatefilersrc(fileno,mtnrsrc);
				rsrc2filerecord(fileno,mtnrsrc);
			}
		} else { 
			if(MTNMASTER!=NULL)
			{
				if(!MTNMASTER->file_type)
				{
#ifdef __GTK_H__
					filerecord2rsrc(fileno,mtnrsrc);
					updateallrsrc(mtnrsrc);
#endif /* __GTK_H__ */
#ifndef __GTK_H__
					updatefilerecord2rsrc(fileno,mtnrsrc);	
#endif /* NOT __GTK_H__ */
				} else {
					updatesinglerecord2rsrc(fileno,mtnrsrc);
				}
			} else {
				updatefilerecord2rsrc(fileno,mtnrsrc);
			}
		}
		if(NEfunc!=NULL)
		{
/*lint -e746 */
			if(MTNMASTER!=NULL)
			{
				NEfunc(mtnrsrc,MTNMASTER,SubFunc,args,funcarg1);
			} else {
				NEfunc(mtnrsrc,SubFunc,args,funcarg1);
			}	
/*lint +e746 */
		}
		if(MTNMASTER!=NULL)
		{
			GET_SUPPORTING(mtnrsrc,MTNMASTER,SubFunc,args,TRUE,FALSE);
		}
	} else {
		if(prevmain!=NULL) GetRDATData(fileno,prevmain);
		if(BUEQLfunc!=NULL)
		{
/*lint -e746 */
			if(MTNMASTER!=NULL)
			{
				BUEQLfunc(mtnrsrc,MTNMASTER,SubFunc,args,funcarg2);
			} else {
				BUEQLfunc(mtnrsrc,SubFunc,args,funcarg2);
			}
/*lint +e746 */
		}
		if(AUEQLfunc!=NULL)
		{
/*lint -e746 */
			if(MTNMASTER!=NULL)
			{
				AUEQLfunc(mtnrsrc,MTNMASTER,SubFunc,args,funcarg3);
			} else {
				AUEQLfunc(mtnrsrc,SubFunc,args,funcarg3);
			}
/*lint +e746 */
		} else {
			if(MTNMASTER!=NULL)
			{
				if(!MTNMASTER->file_type)
				{
#ifdef __GTK_H__
					filerecord2rsrc(fileno,mtnrsrc);
					updateallrsrc(mtnrsrc);
#endif
#ifndef __GTK_H__
					updatefilerecord2rsrc(fileno,mtnrsrc);	
#endif
				} else {
					updatesinglerecord2rsrc(fileno,mtnrsrc);	
				}
			} else {
				updatefilerecord2rsrc(fileno,mtnrsrc);	
			}
		}
		if(MTNMASTER!=NULL)
		{
			GET_SUPPORTING(mtnrsrc,MTNMASTER,SubFunc,args,TRUE,TRUE);
		}
	}
	if(MTNMASTER!=NULL)
	{
		MSTupdateSCRNvirtuals(mtnrsrc,MTNMASTER);
	} else {
		updateSCRNvirtuals(mtnrsrc);
	}
}
void xMakePrevNextButtons(RDArsrc *mtnrsrc,NRDkey *keys,short nokeys,
	void (*PrevFunc)(...),void *PrevFuncArgs,void (*NextFunc)(...),
	void *NextFuncArgs,int line,char *file)
{
	short x=0;
	NRDkey *keyx=NULL;
	char *prevname=NULL,*nextname=NULL;

	if(keys!=NULL)
	{
		for(x=0,keyx=keys;x<nokeys;++x,++keyx)
		{
			if(keyx->name!=NULL)
			{
				if(prevname!=NULL)
				{
					prevname=Rrealloc(prevname,RDAstrlen(keyx->name)+10);
				} else {
					prevname=Rmalloc(RDAstrlen(keyx->name)+10);
				}
				sprintf(prevname,"PREVIOUS %s",keyx->name);
				RMEMaddbtnrsrc(mtnrsrc,prevname,TRUE,PrevFunc,PrevFuncArgs);
				if(nextname!=NULL)
				{
					nextname=Rrealloc(nextname,RDAstrlen(keyx->name)+10);
				} else {
					nextname=Rmalloc(RDAstrlen(keyx->name)+10);
				}
				sprintf(nextname,"NEXT %s",keyx->name);
				RMEMaddbtnrsrc(mtnrsrc,nextname,TRUE,NextFunc,NextFuncArgs);
			}
		}
		if(prevname!=NULL) Rfree(prevname);
		if(nextname!=NULL) Rfree(nextname);
	}
}
