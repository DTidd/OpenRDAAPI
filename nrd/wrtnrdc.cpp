#include <nrdp.hpp>
#ifdef UNIXWARE7
#include <errno.h>
#endif
#ifdef UNIXWARE21
#include <errno.h>
#endif

#ifdef USE_CTREE
short WRTNRDctree(short fileno,short trnmode,char *fieldname)
{
	NRDbuffer *NRDtmp;
	unsigned varlen=0,length,lenx=0;
	short return_value=0,useit=0,ckeyno=0;
	NRDfield *field;
	void *target=NULL;
	LONG setx=0;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG Writing current record to file [%s] trnmode [%d] fieldname [%s] using C-tree Engine.",NRDbuf[fileno].name,trnmode,(fieldname!=NULL ? fieldname:""));
	}
#endif /* USE_RDA_DIAGNOSTICS */
	NRDtmp=NRDbuf+fileno;
	RecordNRDconnect(NRDtmp->servername,NRDtmp->db_engine);
	length=CTREERecordLength(NRDtmp,TRUE);
#ifdef USE_RDA_DIAGNOSTICS
	if(diagnrd)
	{
		prterr("DIAG Length of data record [%d]",length);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,length+1);
	memset(NRDtmp->record_data,0,NRDtmp->dlen);
	if(fieldname!=NULL && RDAstrlen(fieldname)>0)
	{
		field=xFLDNRD(fileno,fieldname,__LINE__,__FILE__);
		if(field->type!=SCROLLEDLIST && field->type!=LONGV && 
			field->type!=SLONGV && field->type!=SHORTV &&
			field->type!=SSHORTV && field->type!=DOUBLEV &&
			field->type!=SDOUBLEV) field=NULL;
	} else field=NULL;
	if(trnmode && field!=NULL)/*get next tran num automatically*/
	{
		SetTrnModeField(fileno,fieldname);
		lenx=CTREERecordLength(NRDtmp,TRUE);
		QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,lenx+1);
		assignCTREERecordData(NRDtmp,TRUE);
		if(NRDtmp->varfile==TRUE)
		{
			return_value=ADDVREC(NRDtmp->filenum,NRDtmp->record_data,length);
		} else {
			return_value=ADDREC(NRDtmp->filenum,NRDtmp->record_data);
		}
		if(return_value)
		{
			prterr("ADD RECORDS %s %s",ctdb_errors(errno),NRDtmp->name);
#ifdef USE_RDA_DIAGNOSTICS
		} else {
			if(diagnrd) { prterr("DIAG exiting wrtnrd OK, used ADDVREC."); }
#endif /* USE_RDA_DIAGNOSTICS */
		}
	} else {
		SETCURI(NRDtmp->filenum,setx,NULL,0);
		lenx=CTREERecordLength(NRDtmp,TRUE);
		QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,lenx+1);
		memset(NRDtmp->record_data,0,NRDtmp->dlen);
		assignCTREERecordData(NRDtmp,TRUE);
		target=maketarget(fileno,1,&useit);
		ckeyno=NRDtmp->filenum+1;
		return_value=EQLREC(ckeyno,target,NRDtmp->record_data);
		if(!return_value)
		{
			if(NRDtmp->varfile==TRUE)
			{
				varlen=GETVLEN(NRDtmp->filenum);
				QUICKALLOC(NRDtmp->record_data,NRDtmp->dlen,varlen+1);
				return_value=REDVREC(NRDtmp->filenum,NRDtmp->record_data,
					varlen);
				assignCTREERecordData(NRDtmp,TRUE);
				return_value=RWTVREC(NRDtmp->filenum,
					NRDtmp->record_data,length);
			} else {
				assignCTREERecordData(NRDtmp,TRUE);
				return_value=RWTREC(NRDtmp->filenum,NRDtmp->record_data);
			}
			if(return_value)
			{
				prterr("ReWrite Errors %s %s",ctdb_errors(errno),NRDtmp->name);
#ifdef USE_RDA_DIAGNOSTICS
			} else if(diagnrd) 
			{ 
				prterr("DIAG exiting wrtnrd OK, used ReWrite.");
#endif /* USE_RDA_DIAGNOSTICS */
			}
			UPDCURI(NRDtmp->filenum,SWTCURI);
		} else {
			assignCTREERecordData(NRDtmp,TRUE);
			if(NRDtmp->varfile==TRUE)
			{
				return_value=ADDVREC(NRDtmp->filenum,NRDtmp->record_data,length);
			} else {
				return_value=ADDREC(NRDtmp->filenum,NRDtmp->record_data);
			}
			if(return_value)
			{
				prterr("Write Errors %s %s",ctdb_errors(errno),NRDtmp->name);
#ifdef USE_RDA_DIAGNOSTICS
			} else {
				if(diagnrd) { prterr("DIAG exiting wrtnrd OK, used Add/Write."); }
#endif /* USE_RDA_DIAGNOSTICS */
			} 
		}
		if(target!=NULL) Rfree(target);
	} 
	return(return_value);
}
#endif
