/* GUI's - Make Standard Custom Input Fields */
#include <cstdio>
#include <mkmsc.hpp>

extern char *module;
extern APPlib *errorlist;

int addACCTSTRUCT(RDAacctmstr *,char *);
int addACCTSTRUCTPART(RDAacct *,char *,int,char *);
void MAKE_CUSTOM_INPUTS(void);

int addACCTSTRUCT(RDAacctmstr *AcctMstr,char *name)
{
	RDAacct *AcctStruct=NULL;

	if(AcctMstr!=NULL)
	{
		if(AcctMstr->codetype!=NULL)
		{
			AcctMstr->codetype=Rrealloc(AcctMstr->codetype,(AcctMstr->num+1)*sizeof(RDAacct));
		} else {
			AcctMstr->codetype=Rmalloc(sizeof(RDAacct));
			AcctMstr->num=0;
		}
		AcctStruct=AcctMstr->codetype+AcctMstr->num;
		AcctStruct->num=0;
		AcctStruct->acct=NULL;
		AcctStruct->name=stralloc(name);
		return(++AcctMstr->num);
	} else {
		return(-1);
	}
}
int addACCTSTRUCTPART(RDAacct *AcctStruct,char *name,int length,char *definition)
{
	RDAaccpart *AcctPart=NULL;

	if(AcctStruct!=NULL)
	{
		if(AcctStruct->acct!=NULL)
		{
			AcctStruct->acct=Rrealloc(AcctStruct->acct,(AcctStruct->num+1)*sizeof(RDAaccpart));
		} else {
			AcctStruct->acct=Rmalloc(sizeof(RDAaccpart));
			AcctStruct->num=0;
		}
		AcctPart=AcctStruct->acct+AcctStruct->num;
		AcctPart->name=stralloc(name);
		AcctPart->length=length;
		AcctPart->definition=stralloc(definition);
		AcctPart->default_zero=FALSE;
		AcctPart->force_zero=FALSE;
		AcctPart->last_dimension=FALSE;
		return(++AcctStruct->num);
	} else {
		return(-1);
	}
}
void MAKE_CUSTOM_INPUTS()
{
	int x=0,y=0;
	RDAaccpart *accpart=NULL;
	RDAacct *AcctStruct=NULL;
	char *temp1=NULL;

	if(CUSTOM_INPUTS_ACCOUNT==NULL) CUSTOM_INPUTS_ACCOUNT=RDAacctNEW(CUSTOM_ACCT);
	if(CUSTOM_INPUTS_ACCOUNT!=NULL)
	{
		if(CUSTOM_INPUTS_ACCOUNT->num<1)
		{
			FreeRDAacctmstr2(&CUSTOM_INPUTS_ACCOUNT);
			CUSTOM_INPUTS_ACCOUNT=RDAacctNEW(CUSTOM_ACCT);
		}
	}
	y=0;
	if(CUSTOM_INPUTS_ACCOUNT!=NULL)
	{
		y++;
		if(temp1!=NULL) Rfree(temp1);
		temp1=stralloc("YYYY-MM");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"YYYY",4,"NNNN");
				addACCTSTRUCTPART(AcctStruct,"MM",2,"NN");
			}
		} else {
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"YYYY",4,"NNNN");
				addACCTSTRUCTPART(AcctStruct,"MM",2,"NN");
			}
		}
		y++;
		if(temp1!=NULL) Rfree(temp1);
		temp1=stralloc("FEDERAL EIN");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"1",2,"NN");
				addACCTSTRUCTPART(AcctStruct,"2",7,"NNNNNNN");
			}
		} else { 
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"1",2,"NN");
				addACCTSTRUCTPART(AcctStruct,"2",7,"NNNNNNN");
			}
		}
		y++;
		if(temp1!=NULL) Rfree(temp1);
		temp1=stralloc("PO ID FORMAT");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"PART1",15,"***************");
			}
		} else {
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"PART1",15,"***************");
			}
		}
		y++;
		if(temp1!=NULL) Rfree(temp1);
		temp1=stralloc("BUSINESS ID FORMAT");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"PART1",25,"*************************");
			}
		} else {
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"PART1",25,"*************************");
			}
		}
		y++;
		if(temp1!=NULL) Rfree(temp1);
		temp1=stralloc("BUSINESS LICENSE NUMBER");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"PART1",40,"****************************************");
			}
		} else {
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"PART1",40,"****************************************");
			}
		}
		y++;
		if(temp1!=NULL) Rfree(temp1);




		temp1=stralloc("PROPERTY ID FORMAT");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"PART1",30,"******************************");
			}
		} else { 
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"PART1",30,"******************************");
			}
		}
		y++;
		if(temp1!=NULL) Rfree(temp1);
		temp1=stralloc("REAL ESTATE ID FORMAT");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"PART1",40,"****************************************");
			}
		} else {
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"PART1",40,"****************************************");
			}
		}
		y++;
		if(temp1!=NULL) Rfree(temp1);
		temp1=stralloc("EMPLOYEE ID FORMAT");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"PART1",15,"***************");
			}
		} else {
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"PART1",15,"***************");
			}
		}	
		y++;
		if(temp1!=NULL) Rfree(temp1);
		temp1=stralloc("MISC ID FORMAT");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				} 
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"PART1",15,"***************");
			}
		} else {
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"PART1",15,"***************");
			}
		}
		y++;
		if(temp1!=NULL) Rfree(temp1);
		temp1=stralloc("CUSTOMER ID FORMAT");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"PART1",25,"*************************");
			}
		} else {
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"PART1",25,"*************************");
			}
		}
		y++;
		if(temp1!=NULL) Rfree(temp1);
		temp1=stralloc("OWNER ID FORMAT");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"PART1",40,"****************************************");
			}
		} else {
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"PART1",40,"****************************************");
			}
		}
		y++;
		if(temp1!=NULL) Rfree(temp1);
		temp1=stralloc("REQUISITION ID FORMAT");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"PART1",15,"***************");
			}
		} else {
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"PART1",15,"***************");
			}
		}

		y++;
		if(temp1!=NULL) Rfree(temp1);
		temp1=stralloc("RECEIPT ID FORMAT");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"PART1",15,"***************");
			}
		} else {
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"PART1",15,"***************");
			}
		}
		y++;
		if(temp1!=NULL) Rfree(temp1);
		temp1=stralloc("POSITION ID FORMAT");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"PART1",30,"******************************");
			}
		} else {
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"PART1",30,"******************************");
			}
		}
		y++;
		if(temp1!=NULL) Rfree(temp1);

		temp1=stralloc("VISA");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"PART1",40,"****************************************");
			}
		} else {
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"PART1",40,"****************************************");
			}
		}
		y++;
		if(temp1!=NULL) Rfree(temp1);

		temp1=stralloc("MASTERCARD");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"PART1",40,"****************************************");
			}
		} else {
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"PART1",40,"****************************************");
			}
		}
		y++;
		if(temp1!=NULL) Rfree(temp1);


		temp1=stralloc("AMERICAN EXPRESS");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"PART1",40,"****************************************");
			}
		} else {
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"PART1",40,"****************************************");
			}
		}
		y++;
		if(temp1!=NULL) Rfree(temp1);


		temp1=stralloc("DISCOVER");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"PART1",40,"****************************************");
			}
		} else {
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"PART1",40,"****************************************");
			}
		}
		y++;
		if(temp1!=NULL) Rfree(temp1);

		temp1=stralloc("DINER'S CLUB");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"PART1",40,"****************************************");
			}
		} else {
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"PART1",40,"****************************************");
			}
		}
		y++;
		if(temp1!=NULL) Rfree(temp1);

		temp1=stralloc("DEBIT CARD");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"PART1",40,"****************************************");
			}
		} else {
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"PART1",40,"****************************************");
			}
		}
		y++;
		if(temp1!=NULL) Rfree(temp1);


		temp1=stralloc("CHECK PAYEE");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"PART1",40,"****************************************");
			}
		} else {
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"PART1",40,"****************************************");
			}
		}
		y++;
		if(temp1!=NULL) Rfree(temp1);


		temp1=stralloc("BUILDING PERMIT");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"PART1",15,"***************");
			}
		} else {
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"PART1",15,"***************");
			}
		}
		y++;
		if(temp1!=NULL) Rfree(temp1);

		temp1=stralloc("CUSTOMER DEFINITION");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"PART1",15,"***************");
			}
		} else {
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"PART1",15,"***************");
			}
		}
		y++;
		if(temp1!=NULL) Rfree(temp1);

		temp1=stralloc("PROBLEM IDENTIFICATION");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"PART1",15,"***************");
			}
		} else {
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"PART1",15,"***************");
			}
		}
		y++;
		if(temp1!=NULL) Rfree(temp1);

		if(!RDAstrcmp(XPERT_SETUP->zip,"22611"))
		{
		temp1=stralloc("GPS COORDINATES");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"DEG",2,"NN");
				addACCTSTRUCTPART(AcctStruct,"MIN",2,"NN");
				addACCTSTRUCTPART(AcctStruct,"SEC",4,"NNNN");
			}
		} else {
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"DEG",2,"NN");
				addACCTSTRUCTPART(AcctStruct,"MIN",2,"NN");
				addACCTSTRUCTPART(AcctStruct,"SEC",4,"NNNN");
			}
		}
		} else {
		temp1=stralloc("GPS COORDINATES");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"COORDINATE",12,"************");
			}
		} else {
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"COORDINATE",12,"************");
			}
		}
		}
		y++;
		if(temp1!=NULL) Rfree(temp1);
		temp1=stralloc("YYYY-MM-DD");
		if(CUSTOM_INPUTS_ACCOUNT->num>=y)
		{
			AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(y-1);
			if(RDAstrcmp(AcctStruct->name,temp1))
			{
				if(AcctStruct->name!=NULL) Rfree(AcctStruct->name);
				AcctStruct->name=stralloc(temp1);
				for(x=0,accpart=AcctStruct->acct;x<AcctStruct->num;++x,++accpart)
				{
					FreeRDAaccpart(&accpart);
				}
				AcctStruct->num=0;
				addACCTSTRUCTPART(AcctStruct,"YYYY",4,"NNNN");
				addACCTSTRUCTPART(AcctStruct,"MM",2,"NN");
				addACCTSTRUCTPART(AcctStruct,"DD",2,"NN");
			}
		} else {
			x=addACCTSTRUCT(CUSTOM_INPUTS_ACCOUNT,temp1);
			if(x!=(-1))
			{
				AcctStruct=CUSTOM_INPUTS_ACCOUNT->codetype+(x-1);
				addACCTSTRUCTPART(AcctStruct,"YYYY",4,"NNNN");
				addACCTSTRUCTPART(AcctStruct,"MM",2,"NN");
				addACCTSTRUCTPART(AcctStruct,"DD",2,"NN");
			}
		}
		y++;
		if(temp1!=NULL) Rfree(temp1);



	}

	if(writeacctbin(CUSTOM_INPUTS_ACCOUNT))
	{
		if(temp1!=NULL) Rfree(temp1);
		temp1=Rmalloc(75+1);
		sprintf(temp1,"CUSTOM INPUT WRITE ERROR: Can Not Create Custom Inputs for XPERT/MBGUI!");
		prterr(temp1);
		if(temp1!=NULL) Rfree(temp1);
	}
}
static void MNU_ADD_MENU_OPTION()
{
	RDAmenu *menu=NULL;
	char *defdir=NULL;

	menu=RDAmenuNEW("ADD MENU OPTION","addmenu.EXT");
	if(menu!=NULL)
	{
		defdir=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(module)+10);
#ifndef WIN32
		sprintf(defdir,"%s/rda/%s.MNU",CURRENTDIRECTORY,module);
#endif
#ifdef WIN32
		sprintf(defdir,"%s\\rda\\%s.MNU",CURRENTDIRECTORY,module);
#endif
		if(writemenubin(defdir,menu))
		{
			ERRORDIALOG("Cannot Save Menu","This menu may not be saved to the specified library.  Check your permissions and re-try.  Call your installer for further instructions.",NULL,FALSE);
		}
		if(defdir!=NULL) Rfree(defdir);
		if(menu!=NULL) free_menu(menu);
	}
}
static void SCN_ADD_MENU_OPTION()
{
	RDAscrn *scrn=NULL;
	char *defdir=NULL;
	char *temp1=NULL,*temp2=NULL;
	char *temp3=NULL,*temp4=NULL;

	scrn=RDAscrnNEW("GUI","ADD MENU OPTION");
	if(scrn!=NULL)
	{
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Menu Item:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Menu Label:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"MENU ITEM","Menu Item","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"MENU LABEL","Menu Label","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Directory:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"DIRECTORY LIST","Directory List","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"DIRECTORY","Directory","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","LIBRARY LIST","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,10,"","","","",0,0,2,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","SCREEN LIST","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"LIBRARY LIST","Library List","","",5,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"SCREEN LIST","Screen List","","",5,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","Placement:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"PLACEMENT LIST","Placement List","","",4,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,5,"","New Directory:","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,7,"NEW DIRECTORY LIST","New Directory List","","",1,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,0,"NEW DIRECTORY","New Directory","","",0,40,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,4,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,11,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,2,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,3,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,1,"","","","",0,0,0,NULL,NULL,NULL,NULL);
		ADVaddwdgt(scrn,6,"SELECT","Select","","",0,0,36,NULL,NULL,NULL,NULL);
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
			temp1=Rmalloc(15+3+89+1);
			sprintf(temp1,"SCREEN WRITE ERROR: Module [GUI] Screen [ADD MENU OPTION], Can Not Save Maintain Master!");
			prterr(temp1);
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
void ADD_MENU()
{
	MNU_ADD_MENU_OPTION();
	SCN_ADD_MENU_OPTION();
}
