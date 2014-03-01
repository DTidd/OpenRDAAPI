/* pptfunc.c - Personal Property Functions */
#include <pptmgt.hpp>
#include <mix.hpp>

char *PPTMGTTranTypeFields[4]={"TAX","PENALTY","INTEREST","PAID"};

char *xMakePersonalPropertyField(char *classid,short trantype,
	RDApptmgt *pptmgt,int line,char *file)
{
	char *tmp=NULL;
	char *Name=NULL;
	PPTclas *c;
	int x;

	if(!trantype)
	{
		if(!isEMPTY(classid))
		{
		x=Ratoi(classid)-1;
		} else x=0;
		if(x<0) x=0;
		if(x<pptmgt->num && x>=0 && pptmgt->classes!=NULL)
		{
			c=pptmgt->classes+x;
			Name=c->name;
		} else Name=NULL;
		tmp=Rmalloc(RDAstrlen(PPTMGTTranTypeFields[trantype])+RDAstrlen(Name)+9);
		sprintf(tmp,"%s %s AMOUNT",Name,PPTMGTTranTypeFields[trantype]);
	} else {
		tmp=Rmalloc(RDAstrlen(PPTMGTTranTypeFields[trantype])+8);
		sprintf(tmp,"%s AMOUNT",PPTMGTTranTypeFields[trantype]);
	}
	return(tmp);
}

void xUpdatePersonalProperty(short taxdwr,short pptmstr,short pptnew,
	short pptrn,short pptpd,double amount,void (*SubFunc)(...),
	void *args,RDApptmgt *pptmgt,int line,char *file)
{
	RDATData *prev=NULL;
	double total=0.0;
	int y=0,trantype=0,yearid=0,trans=0,paynum=(-1),numflds=0,x;
	short ef=FALSE;
	char *temp=NULL,*propertyid=NULL,*ownerid=NULL,dist=FALSE;
	char *nam=NULL,*classid=NULL,freepptmgt=FALSE;
	char deleteflag=FALSE;
	NRDfield *fields=NULL,*field;
	PPTclas *class;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		if(pptpd!=(-1))
		{
			prterr("DIAG UpdatePersonalPropertyPayments at line [%d] program [%s].",line,file);
		} else {
			prterr("DIAG UpdatePersonalPropertyTransactions at line [%d] program [%s].",line,file);
		}
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(pptpd!=(-1))
	{
		trantype=3;
		FINDFLDGETSTRING(pptpd,"DISTRICT IDENTIFICATION",&propertyid);
		FINDFLDGETSTRING(pptpd,"OWNER IDENTIFICATION",&ownerid);
		FINDFLDGETINT(pptpd,"TAX YEAR",&yearid);
		FINDFLDGETINT(pptpd,"TRANSACTION NUMBER",&trans);
		FINDFLDGETINT(pptpd,"PAYMENT NUMBER",&paynum);
		FIELDCOPY(pptpd,"ENTRY DATE",pptmstr,"LAST PAID DATE");
		FINDFLDSETSTRING(pptrn,"DISTRICT IDENTIFICATION",propertyid);
		FINDFLDSETSTRING(pptrn,"OWNER IDENTIFICATION",ownerid);
		FINDFLDSETINT(pptrn,"TAX YEAR",yearid);
		FINDFLDSETINT(pptrn,"TRANSACTION NUMBER",trans);
		LOCNRDFILE(pptrn);
		if(SubFunc!=NULL) ef=ADVEQLNRDsec(pptrn,1,SubFunc,args);
			else ef=EQLNRDsec(pptrn,1);
		if(!ef)
		{
			prev=RDATDataNEW(pptrn);
			FINDFLDGETDOUBLE(pptrn,"OUTSTANDING AMOUNT",&total);
			total-=amount;
			FINDFLDSETDOUBLE(pptrn,"OUTSTANDING AMOUNT",total);
			if(SubFunc!=NULL) ADVWRTTRANSsec(pptrn,0,NULL,prev,SubFunc,args);
				else WRTTRANSsec(pptrn,0,NULL,prev);
			if(prev!=NULL) FreeRDATData(prev);
			FINDFLDGETSTRING(pptrn,"CLASS IDENTIFICATION",&classid);
		} else {
			prterr("Error Personal Property Transaction Master not found for Payment Record [%s][%s][%d][%d][%d] at line [%d] program [%s].",propertyid,ownerid,yearid,trans,paynum,line,file);
		}
		UNLNRDFILE(pptrn);
		FINDFLDGETCHAR(pptpd,"CASHED OUT",&dist);
		FINDFLDGETSTRING(pptpd,"CASH DRAWER",&temp);
		if(!dist && !isEMPTY(temp))
		{
			if(taxdwr!=(-1))
			{
				FINDFLDSETSTRING(taxdwr,"CASH DRAWER",temp);
				LOCNRDFILE(taxdwr);
				if(SubFunc!=NULL) ef=ADVEQLNRDsec(taxdwr,1,SubFunc,args);
					else ef=EQLNRDsec(taxdwr,1);
				if(!ef)
				{
					prev=RDATDataNEW(taxdwr);
					FINDFLDGETDOUBLE(taxdwr,"COLLECTED AMOUNT",&total);
					total=total+amount;
					FINDFLDSETDOUBLE(taxdwr,"COLLECTED AMOUNT",total);
					if(SubFunc!=NULL) ADVWRTTRANSsec(taxdwr,0,NULL,prev,SubFunc,args);
						else WRTTRANSsec(taxdwr,0,NULL,prev);
					if(prev!=NULL) FreeRDATData(prev);
				}
				UNLNRDFILE(taxdwr);
			}
		}
	} else {
		FINDFLDGETSTRING(pptrn,"DISTRICT IDENTIFICATION",&propertyid);
		FINDFLDGETSTRING(pptrn,"OWNER IDENTIFICATION",&ownerid);
		FINDFLDGETINT(pptrn,"TAX YEAR",&yearid);
		FINDFLDGETINT(pptrn,"TRANSACTION TYPE",&trantype);
		FINDFLDGETSTRING(pptrn,"CLASS IDENTIFICATION",&classid);
	}
	FINDFLDSETSTRING(pptmstr,"DISTRICT IDENTIFICATION",propertyid);
	FINDFLDSETSTRING(pptmstr,"OWNER IDENTIFICATION",ownerid);
	FINDFLDSETINT(pptmstr,"TAX YEAR",yearid);
	total=0.0;
	LOCNRDFILE(pptmstr);
	if(SubFunc!=NULL) ef=ADVEQLNRDsec(pptmstr,1,SubFunc,args);
		else ef=EQLNRDsec(pptmstr,1);
	if(ef)
	{
		KEYNRD(pptmstr,1);
		if(pptnew!=(-1))
		{
			FINDFLDSETSTRING(pptnew,"DISTRICT IDENTIFICATION",
				propertyid);
			FINDFLDSETSTRING(pptnew,"OWNER IDENTIFICATION",ownerid);
			if(SubFunc!=NULL) ef=ADVEQLNRDsec(pptnew,1,SubFunc,args);
				else ef=EQLNRDsec(pptnew,1);
			if(!ef)
			{
				numflds=NUMFLDS(pptnew);
				fields=FLDPOINTER(pptnew);
				if(fields!=NULL)
				{
					for(x=0,field=fields;x<numflds;++x,++field)
					{
						COPYFIELD(pptnew,pptmstr,field->name);
					}
				}
				FIELDCOPY(pptnew,"ADDRESS 1",pptmstr,"STATEMENT ADDRESS 1");
				FIELDCOPY(pptnew,"ADDRESS 2",pptmstr,"STATEMENT ADDRESS 2");
				FIELDCOPY(pptnew,"CITY",pptmstr,"STATEMENT CITY");
				FIELDCOPY(pptnew,"STATE",pptmstr,"STATEMENT STATE");
				FIELDCOPY(pptnew,"ZIP",pptmstr,"STATEMENT ZIP CODE");
			}
			FINDFLDSETCHAR(pptmstr,"DELETEFLAG",deleteflag);
		}
	} else {
		prev=RDATDataNEW(pptmstr);
		if(pptnew!=(-1) && !trantype)
		{
			FINDFLDSETSTRING(pptnew,"DISTRICT IDENTIFICATION",
				propertyid);
			FINDFLDSETSTRING(pptnew,"OWNER IDENTIFICATION",ownerid);
			if(SubFunc!=NULL) ef=ADVEQLNRDsec(pptnew,1,SubFunc,args);
				else ef=EQLNRDsec(pptnew,1);
			if(!ef)
			{
				if(pptmgt==NULL)
				{
					freepptmgt=TRUE;
					pptmgt=RDApptmgtNEW();
					if(getpptmgtbin(pptmgt))
					{
						free_pptmgt(pptmgt);
						freepptmgt=FALSE;
					}
				}
				if(pptmgt!=NULL)
				{
					if(pptmgt->classes!=NULL)
					{
						for(y=0,class=pptmgt->classes;y<pptmgt->num;
							++y,++class)
						{
							nam=Rmalloc(RDAstrlen(class->name)+7);
							sprintf(nam,"%s VALUE",class->name);
							COPYFIELD(pptnew,pptmstr,nam);
							if(nam!=NULL) Rfree(nam);
						}
					}
					if(freepptmgt) 
					{
						free_pptmgt(pptmgt);
						freepptmgt=FALSE;
					}
				}
			}
		}
	}
	if(pptpd!=(-1)) trantype=3;
	temp=MakePersonalPropertyField(classid,trantype,pptmgt);
	if(temp!=NULL)
	{
		FINDFLDGETDOUBLE(pptmstr,temp,&total);
		total+=amount;
		FINDFLDSETDOUBLE(pptmstr,temp,total);
		if(SubFunc!=NULL) ADVWRTTRANSsec(pptmstr,0,NULL,prev,SubFunc,args);
			else WRTTRANSsec(pptmstr,0,NULL,prev);
		Rfree(temp);
	}
	if(prev!=NULL) FreeRDATData(prev);
	UNLNRDFILE(pptmstr);
	if(classid!=NULL) Rfree(classid);
	if(propertyid!=NULL) Rfree(propertyid);
	if(ownerid!=NULL) Rfree(ownerid);
}
short xgetpptmgtbin(RDApptmgt *pptmgt,int line,char *file)
{
	short x,test=FALSE;
	BIN *bin;
	char *libname=NULL;
	int y=0;
	PPTclas *class;

	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+17);
#ifndef WIN32
	sprintf(libname,"%s/rda/PPTMGT.STP",CURRENTDIRECTORY);	
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\PPTMGT.STP",CURRENTDIRECTORY);	
#endif
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG getpptmgtbin Reading PPTMGT Setup Variables from [%s] at line [%d] program [%s].",libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=getlibbin(libname,"PPTMGT SETUP",TRUE);
	if(bin==NULL) 
	{
		prterr("Error PPTMGT Setup Variables were not found in Library [%s] or the Library doesn't exist at line [%d] program [%s].",libname,line,file);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	x=BINgetshort(bin);
	if(x!=(-1701))
	{
		prterr("Error Invalid version or bad binary in Library [%s] cannot read PPTMGT Setup Variables at line [%d] program [%s].",libname,line,file);
		BINfree(bin);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	test=BINgetshort(bin);
	if(test) pptmgt->commissioner=BINgetstring(bin);
		else pptmgt->commissioner=NULL;
	test=BINgetshort(bin);
	if(test) pptmgt->comm_addr1=BINgetstring(bin);
		else pptmgt->comm_addr1=NULL;
	test=BINgetshort(bin);
	if(test) pptmgt->comm_addr2=BINgetstring(bin);
		else pptmgt->comm_addr2=NULL;
	test=BINgetshort(bin);
	if(test) pptmgt->comm_city=BINgetstring(bin);
		else pptmgt->comm_city=NULL;
	test=BINgetshort(bin);
	if(test) pptmgt->comm_state=BINgetstring(bin);
		else pptmgt->comm_state=NULL;
	test=BINgetshort(bin);
	if(test) pptmgt->comm_zip=BINgetstring(bin);
		else pptmgt->comm_zip=NULL;
	test=BINgetshort(bin);
	if(test) pptmgt->comm_phone=BINgetstring(bin);
		else pptmgt->comm_phone=NULL;
	test=BINgetshort(bin);
	if(test) pptmgt->treasurer=BINgetstring(bin);
		else pptmgt->treasurer=NULL;
	test=BINgetshort(bin);
	if(test) pptmgt->treas_addr1=BINgetstring(bin);
		else pptmgt->treas_addr1=NULL;
	test=BINgetshort(bin);
	if(test) pptmgt->treas_addr2=BINgetstring(bin);
		else pptmgt->treas_addr2=NULL;
	test=BINgetshort(bin);
	if(test) pptmgt->treas_city=BINgetstring(bin);
		else pptmgt->treas_city=NULL;
	test=BINgetshort(bin);
	if(test) pptmgt->treas_state=BINgetstring(bin);
		else pptmgt->treas_state=NULL;
	test=BINgetshort(bin);
	if(test) pptmgt->treas_zip=BINgetstring(bin);
		else pptmgt->treas_zip=NULL;
	test=BINgetshort(bin);
	if(test) pptmgt->treas_phone=BINgetstring(bin);
		else pptmgt->treas_phone=NULL;
	pptmgt->num=BINgetint(bin);
	if(pptmgt->num>0)
	{
		pptmgt->classes=Rmalloc(pptmgt->num*sizeof(PPTclas));
		for(y=0,class=pptmgt->classes;y<pptmgt->num;++y,++class)
		{
			test=BINgetshort(bin);
			if(test) class->name=BINgetstring(bin);
				else class->name=NULL;
			test=BINgetshort(bin);
			if(test) class->desc=BINgetstring(bin);
				else class->desc=NULL;
		}
	} else pptmgt->classes=NULL;
	BINfree(bin);
	if(libname!=NULL) Rfree(libname);
	return(0);
}
short xwritepptmgtbin(RDApptmgt *pptmgt,int line,char *file)
{
	BIN *bin;
	char *libname=NULL;
	int y;
	PPTclas *class;

	libname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+17);
#ifndef WIN32
	sprintf(libname,"%s/rda/PPTMGT.STP",CURRENTDIRECTORY);	
#endif
#ifdef WIN32
	sprintf(libname,"%s\\rda\\PPTMGT.STP",CURRENTDIRECTORY);	
#endif
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG writepptmgtbin Writing PPTMGT Setup Variables to Library [%s] at line [%d] program [%s].",libname,line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	bin=BINnew();
	BINaddshort(bin,-1701);	/*type identifier*/
	if(!isEMPTY(pptmgt->commissioner))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,pptmgt->commissioner);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(pptmgt->comm_addr1))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,pptmgt->comm_addr1);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(pptmgt->comm_addr2))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,pptmgt->comm_addr2);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(pptmgt->comm_city))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,pptmgt->comm_city);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(pptmgt->comm_state))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,pptmgt->comm_state);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(pptmgt->comm_zip))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,pptmgt->comm_zip);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(pptmgt->comm_phone))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,pptmgt->comm_phone);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(pptmgt->treasurer))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,pptmgt->treasurer);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(pptmgt->treas_addr1))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,pptmgt->treas_addr1);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(pptmgt->treas_addr2))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,pptmgt->treas_addr2);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(pptmgt->treas_city))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,pptmgt->treas_city);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(pptmgt->treas_state))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,pptmgt->treas_state);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(pptmgt->treas_zip))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,pptmgt->treas_zip);
	} else BINaddshort(bin,FALSE);
	if(!isEMPTY(pptmgt->treas_phone))
	{
		BINaddshort(bin,TRUE);
		BINaddstring(bin,pptmgt->treas_phone);
	} else BINaddshort(bin,FALSE);
	if(pptmgt->classes!=NULL)
	{
		BINaddint(bin,pptmgt->num);
		for(y=0,class=pptmgt->classes;y<pptmgt->num;++y,++class)
		{
			if(!isEMPTY(class->name))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,class->name);
			} else BINaddshort(bin,FALSE);
			if(!isEMPTY(class->desc))
			{
				BINaddshort(bin,TRUE);
				BINaddstring(bin,class->desc);
			} else BINaddshort(bin,FALSE);
		}
	} else BINaddint(bin,0);
	if(writelibbin(libname,bin,"PPTMGT SETUP"))
	{
		prterr("Error Can't write PPTMGT Setup Variables in binary Library [%s] at line [%d] program [%s].",libname,line,file);
		if(libname!=NULL) Rfree(libname);
		return(-1);
	}
	if(libname!=NULL) Rfree(libname);
	return(0);
}
RDApptmgt *xRDApptmgtNEW(int line,char *file)
{
	RDApptmgt *tmp;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG RDApptmgtNEW Creating a structure for the Fincancial Management Setup Variables at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	tmp=Rmalloc(sizeof(RDApptmgt));
	tmp->commissioner=NULL;
	tmp->comm_addr1=NULL;
	tmp->comm_addr2=NULL;
	tmp->comm_city=NULL;
	tmp->comm_state=NULL;
	tmp->comm_zip=NULL;
	tmp->comm_phone=NULL;
	tmp->treasurer=NULL;
	tmp->treas_addr1=NULL;
	tmp->treas_addr2=NULL;
	tmp->treas_city=NULL;
	tmp->treas_state=NULL;
	tmp->treas_zip=NULL;
	tmp->treas_phone=NULL;
	tmp->num=0;
	tmp->classes=NULL;
	return(tmp);
}
void xfree_pptmgt(RDApptmgt *pptmgt,int line,char *file)
{
	int y=0;
	PPTclas *class;

#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG free_pptmgt freeing memory used for the PPTMGT Setup Variables at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(pptmgt!=NULL) 
	{
		if(pptmgt->commissioner!=NULL) Rfree(pptmgt->commissioner);
		if(pptmgt->comm_addr1!=NULL) Rfree(pptmgt->comm_addr1);
		if(pptmgt->comm_addr2!=NULL) Rfree(pptmgt->comm_addr2);
		if(pptmgt->comm_city!=NULL) Rfree(pptmgt->comm_city);
		if(pptmgt->comm_state!=NULL) Rfree(pptmgt->comm_state);
		if(pptmgt->comm_zip!=NULL) Rfree(pptmgt->comm_zip);
		if(pptmgt->comm_phone!=NULL) Rfree(pptmgt->comm_phone);
		if(pptmgt->treasurer!=NULL) Rfree(pptmgt->treasurer);
		if(pptmgt->treas_addr1!=NULL) Rfree(pptmgt->treas_addr1);
		if(pptmgt->treas_addr2!=NULL) Rfree(pptmgt->treas_addr2);
		if(pptmgt->treas_city!=NULL) Rfree(pptmgt->treas_city);
		if(pptmgt->treas_state!=NULL) Rfree(pptmgt->treas_state);
		if(pptmgt->treas_zip!=NULL) Rfree(pptmgt->treas_zip);
		if(pptmgt->treas_phone!=NULL) Rfree(pptmgt->treas_phone);
		if(pptmgt->classes!=NULL)
		{
			for(y=0,class=pptmgt->classes;y<pptmgt->num;++y,++class)
			{
				if(class->name!=NULL) Rfree(class->name);
				if(class->desc!=NULL) Rfree(class->desc);
			}
			Rfree(pptmgt->classes);
		}
		Rfree(pptmgt);
	}
}
short xGetPPTMGTSetup(RDApptmgt *pptsetup,void (*qfunc)(void *),void *arg,int line,
	char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diagmix)
	{
		prterr("DIAG GetPPTMGTSetup at line [%d] program [%s].",line,file);
	}
#endif /* ifdef USE_RDA_DIAGNOSTICS */
	if(getpptmgtbin(pptsetup)==(-1))
	{
		ERRORDIALOG("PPTMGT Setup Not Available","The PPTMGT Setup File cannot opened.\n",NULL,TRUE);
		ShutdownSecurity();
		RDAAPPMAINLOOP();
		if(qfunc!=NULL) qfunc(arg);
		return(-1);
	}
	return(0);
}
