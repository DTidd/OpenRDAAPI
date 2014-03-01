#include <trans.hpp>
#include <quotes.hpp>

short xWRTTRANS(short fileno,short trnmode,char *fieldname,RDATData *previous,
	int line,char *file)
{
	NRDfield *fielddata=NULL,*tmp,*fld,*field,*pfld;
	int numflds=0,x;
	short return_val=(-1),free_hold=FALSE;
	RDATData *HoldRDATData=NULL;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagtrans)
	{
		prterr("DIAG: Testing Transaction Log availability.");
	}
#endif /* USE_RDA_DIAGNOSTICS */
	LOCNRDFILE(fileno);
	if(dotrans)
	{
#ifdef USE_RDA_DIAGNOSTICS
		if(diagtrans)
		{
			prterr("DIAG WRTTRANS Writing Record in File [%d] [%s] transaction mode [%d] transaction field [%s] at line [%d] program [%s].",fileno,FILENAME(fileno),trnmode,(fieldname!=NULL?fieldname:"NULL"),line,file);
		}
#endif /* USE_RDA_DIAGNOSTICS */
		if(!isEMPTY(fieldname))
		{
			field=xFLDNRD(fileno,fieldname,__LINE__,__FILE__);
			if(field->type!=SCROLLEDLIST && field->type!=LONGV && 
				field->type!=SLONGV && field->type!=SHORTV &&
				field->type!=SSHORTV && field->type!=DOUBLEV &&
				field->type!=SDOUBLEV) field=NULL;
		} else field=NULL;
		if(trnmode && field!=NULL)/*get next tran num automatically*/
		{
			if(DATABASE_ENGINE(fileno)==NRDMySQLAutoEngine)
			{
				return_val=WRTNRD(fileno,trnmode,fieldname);
				numflds=NUMFLDS(fileno);
				fielddata=FLDPOINTER(fileno);
				for(x=0,fld=fielddata;x<numflds;++x,++fld)
				{
					WriteDATTRNLog(fileno,NULL,fld);
				}		
			} else {
				SetTrnModeField(fileno,fieldname);
				numflds=NUMFLDS(fileno);
				fielddata=FLDPOINTER(fileno);
				for(x=0,fld=fielddata;x<numflds;++x,++fld)
				{
					WriteDATTRNLog(fileno,NULL,fld);
				}		
				return_val=WRTNRD(fileno,0,NULL);
			}
		} else {
			numflds=NUMFLDS(fileno);
			if(numflds>0)
			{
				if(previous==NULL)
				{
					free_hold=TRUE;
					HoldRDATData=RDATDataNEW(fileno);
					ClearRDATData(HoldRDATData);
				} else {
					HoldRDATData=previous;
					free_hold=FALSE;
				}
				fielddata=Rmalloc(numflds*sizeof(NRDfield));
				for(x=0,fld=fielddata;x<numflds;++x,++fld) 
				{
					tmp=FLDNUM(fileno,x);
					cpfldinfo(fld,tmp);
				}
				if(!EQLNRD(fileno,1))
				{
					for(x=0,fld=fielddata,pfld=HoldRDATData->field;
						x<numflds;++x,++fld,++pfld) 
					{
						tmp=FLDNUM(fileno,x);
						if(COMPARENRDfield(pfld,fld))
						{
							NRD2NRD(fld,tmp);
							WriteDATTRNLog(fileno,pfld,fld);
							FreeNRDfield(fld);
						} else {
							FreeNRDfield(fld);
						}
					}
					if(fielddata!=NULL) Rfree(fielddata);
					fielddata=NULL;
				} else {
					for(x=0,fld=fielddata;x<numflds;++x,++fld) 
					{
						WriteDATTRNLog(fileno,NULL,fld);
/* Record all fields as new from=NULL */
						FreeNRDfield(fld);
					}
					Rfree(fielddata);
					fielddata=NULL;
/* creating new record */
				}
				return_val=WRTNRD(fileno,0,NULL);
				if(free_hold)
				{
					FreeRDATData(HoldRDATData);
					free_hold=FALSE;
				}
			} else {
				prterr("Error File [%s] [%s] has no fields to write at line [%d] program [%s].",MODULENAME(fileno),FILENAME(fileno),line,file);
			}
		}
	} else {
		return_val=WRTNRD(fileno,trnmode,fieldname);	
	}
	UNLNRDFILE(fileno);
	return(return_val);
}
