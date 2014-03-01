/* reqaddc.c - Add Requisition Multiple Account Maintenance */
#include <cstdio>

#include <mix.hpp>
#include <reqmgt.hpp>

extern short expnum,eyrnum,balnum,byrnum,finyrnum,banknum;
extern APPlib *yearavl,*acclst,*acctavl,*debitavl,*creditavl,*DEFtypes;
extern APPlib *defdebitavl,*defcreditavl,*defpayableavl;
extern REQACC *ACCOUNTS;
extern char changedvalues,*module;
extern void getacctdesc(RDArsrc *);
extern void acctsearch(RDArsrc *,short *,short);
extern void acctsearchcb(RDArsrc *,short *);
extern void getsearchstr(RDArsrc *,short *);
extern void changeDEFtype(RDArsrc *,short);
extern short acctcodenum,debitcodenum,creditcodenum;
extern short defdebitcodenum,defcreditcodenum,defpayablecodenum;
extern void getBANKids(RDArsrc *);
extern void setBANKidscb(RDArsrc *);
extern void BANKidscb(RDArsrc *);
extern APPlib *BANKids;

RDAdefault *accdefaults=NULL;
void edit_account(RDArsrc *);
void delete_account(RDArsrc *);
void add_account_below(RDArsrc *);
void add_account_above(RDArsrc *);
void makeaccountlist(void);
void mkacclst(ReqAcctStruct *);
RDArsrc *defineacclst=NULL;
extern void DEFtypecb(RDArsrc *);
static void makespaceaccountlist(int);
static void quit_accounttest(RDArsrc *,RDArsrc *);
static void quit_account(RDArsrc *,RDArsrc *);
static void save_account(RDArsrc *,RDArsrc *);
static void save_account_below(RDArsrc *,RDArsrc *);
static void save_account_above(RDArsrc *,RDArsrc *);
static void accountscreen(RDArsrc *,short,char *,char *,char *,char,double,
	double,char *,char *,char *,char *,void (*)(...),void (*)(...));
static void quit_account_abovetest(RDArsrc *,RDArsrc *);
static void quit_account_belowtest(RDArsrc *,RDArsrc *);
static void setprorate(RDArsrc *);
void mk_define_account_list(RDArsrc *);

void mk_define_account_list(RDArsrc *defineacclst)
{
	short l,count;

	count=1;
	l=2;
	addstdrsrc(defineacclst,"ACCOUNT NUMBER POSITION",SHORTV,3,&count,TRUE);
	addstdrsrc(defineacclst,"ACCOUNT NUMBER LENGTH",SHORTV,3,&l,TRUE);
	++count;
	l=2;
	addstdrsrc(defineacclst,"DEFINITION TYPE POSITION",SHORTV,3,&count,TRUE);
	addstdrsrc(defineacclst,"DEFINITION TYPE LENGTH",SHORTV,3,&l,TRUE);
	++count;
	l=30;
	addstdrsrc(defineacclst,"ACCOUNT CODE POSITION",SHORTV,3,&count,TRUE);
	addstdrsrc(defineacclst,"ACCOUNT CODE LENGTH",SHORTV,3,&l,TRUE);
	++count;
	l=15;
	addstdrsrc(defineacclst,"DEBIT CODE POSITION",SHORTV,3,&count,TRUE);
	addstdrsrc(defineacclst,"DEBIT CODE LENGTH",SHORTV,3,&l,TRUE);
	++count;
	l=15;
	addstdrsrc(defineacclst,"CREDIT CODE POSITION",SHORTV,3,&count,TRUE);
	addstdrsrc(defineacclst,"CREDIT CODE LENGTH",SHORTV,3,&l,TRUE);
	++count;
	l=1;
	addstdrsrc(defineacclst,"PRORATE POSITION",SHORTV,3,&count,TRUE);
	addstdrsrc(defineacclst,"PRORATE LENGTH",SHORTV,3,&l,TRUE);
	++count;
	l=8;
	addstdrsrc(defineacclst,"PERCENTAGE POSITION",SHORTV,3,&count,TRUE);
	addstdrsrc(defineacclst,"PERCENTAGE LENGTH",SHORTV,3,&l,TRUE);
	++count;
	l=15;
	addstdrsrc(defineacclst,"AMOUNT POSITION",SHORTV,3,&count,TRUE);
	addstdrsrc(defineacclst,"AMOUNT LENGTH",SHORTV,3,&l,TRUE);
	++count;
	l=15;
	addstdrsrc(defineacclst,"ENCUMBRANCE AMOUNT POSITION",SHORTV,3,&count,TRUE);
	addstdrsrc(defineacclst,"ENCUMBRANCE AMOUNT LENGTH",SHORTV,3,&l,TRUE);
	++count;
	l=15;
	addstdrsrc(defineacclst,"PRE-ENCUMBRANCE AMOUNT POSITION",SHORTV,3,&count,TRUE);
	addstdrsrc(defineacclst,"PRE-ENCUMBRANCE AMOUNT LENGTH",SHORTV,3,&l,TRUE);
	++count;
	l=15;
	addstdrsrc(defineacclst,"DEFAULT DEBIT CODE POSITION",SHORTV,3,&count,TRUE);
	addstdrsrc(defineacclst,"DEFAULT DEBIT CODE LENGTH",SHORTV,3,&l,TRUE);
	++count;
	l=15;
	addstdrsrc(defineacclst,"DEFAULT CREDIT CODE POSITION",SHORTV,3,&count,TRUE);
	addstdrsrc(defineacclst,"DEFAULT CREDIT CODE LENGTH",SHORTV,3,&l,TRUE);
	++count;
	l=15;
	addstdrsrc(defineacclst,"DEFAULT PAYABLE CODE POSITION",SHORTV,3,&count,TRUE);
	addstdrsrc(defineacclst,"DEFAULT PAYABLE CODE LENGTH",SHORTV,3,&l,TRUE);
	++count;
	l=25;
	addstdrsrc(defineacclst,"DEFAULT BANK IDENTIFICATION POSITION",SHORTV,3,&count,TRUE);
	addstdrsrc(defineacclst,"DEFAULT BANK IDENTIFICATION LENGTH",SHORTV,3,&l,TRUE);

	addbtnrsrc(defineacclst,"DEFAULTS",TRUE,SaveDefaultsUsedOnly,NULL);
	addrfkwrsrc(defineacclst,"SELECT",TRUE,NULL,NULL);
	addrfkwrsrc(defineacclst,"QUIT",TRUE,NULL_RSCS,NULL);
	addrfcbrsrc(defineacclst,"HELP",TRUE,screenhelp,NULL);
	addrfcbrsrc(defineacclst,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	DefaultScreens(defineacclst);
}
void mkacclst(ReqAcctStruct *fields)
{
	short x,cur=1,length=0,lcount=0;
	RDArmem *len_member,*pos_member;
	char *s=NULL,*hold=NULL,*temp=NULL;

	if(acclst!=NULL)
	{
		if(acclst->numlibs==1 && !RDAstrcmp(acclst->libs[0],
		"No Accounts Defined"))
		{
			freeapplib(acclst);
			acclst=APPlibNEW();
		}
	} else {
		acclst=APPlibNEW();
	}
	for(x=0,length=0;x<(defineacclst->numrscs-5);x+=2)
	{
		pos_member=defineacclst->rscs+x;
		len_member=defineacclst->rscs+(x+1);
		if(*pos_member->value.short_value>0)
		{
			length+=(*len_member->value.short_value+2);
		}
	}
	hold=Rmalloc(length+1);
	lcount=0;
	cur=0;
	while(cur<(defineacclst->numrscs-5))
	{
		for(x=0;x<(defineacclst->numrscs-5);x+=2)
		{
			pos_member=defineacclst->rscs+x;
			len_member=defineacclst->rscs+(x+1);
			if(*pos_member->value.short_value>0 &&
				*len_member->value.short_value>0)
			{
				if(*pos_member->value.short_value==cur)
				{
					s=stralloc(pos_member->rscrname);
					s[RDAstrlen(s)-9]=0;
					if(!RDAstrcmp("ACCOUNT NUMBER",s))
					{
						sprintf(&hold[lcount],"%*d ",*len_member->value.short_value,fields->accno);
						lcount+=*len_member->value.short_value+1;
					} else if(!RDAstrcmp("DEFINITION TYPE",s)) 
					{
						sprintf(&hold[lcount],"%*d ",*len_member->value.short_value,fields->deftype);
						lcount+=*len_member->value.short_value+1;

					} else if(!RDAstrcmp("ACCOUNT CODE",s)) 
					{
						if(*len_member->value.short_value<RDAstrlen(fields->account))
						{
							sprintf(&hold[lcount],"%.*s ",*len_member->value.short_value,(fields->account!=NULL?fields->account:""));
						} else {
							sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,(fields->account!=NULL?fields->account:""));
						}
						lcount+=*len_member->value.short_value+1;
					} else if(!RDAstrcmp("DEBIT CODE",s)) 
					{
						if(*len_member->value.short_value<RDAstrlen(fields->account))
						{
							sprintf(&hold[lcount],"%.*s ",*len_member->value.short_value,(fields->debit!=NULL?fields->debit:""));
						} else {
							sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,(fields->debit!=NULL?fields->debit:""));
						}
						lcount+=*len_member->value.short_value+1;
					} else if(!RDAstrcmp("CREDIT CODE",s)) 
					{
						if(*len_member->value.short_value<RDAstrlen(fields->account))
						{
							sprintf(&hold[lcount],"%.*s ",*len_member->value.short_value,(fields->credit!=NULL?fields->credit:""));
						} else {
							sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,(fields->credit!=NULL?fields->credit:""));
						}
						lcount+=*len_member->value.short_value+1;
					} else if(!RDAstrcmp("PRORATE",s)) 
					{
						sprintf(&hold[lcount],"%c ",(fields->prorate ? 'Y':'N'));
						lcount+=*len_member->value.short_value+1;
					} else if(!RDAstrcmp("PERCENTAGE",s)) 
					{
						sprintf(&hold[lcount],"%*f ",*len_member->value.short_value,fields->percentage);
						lcount+=*len_member->value.short_value+1;
					} else if(!RDAstrcmp("AMOUNT",s)) 
					{
						temp=famt(fields->amount,*len_member->value.short_value);
						sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,temp);
						if(temp!=NULL) Rfree(temp);
						lcount+=*len_member->value.short_value+1;
					} else if(!RDAstrcmp("ENCUMBRANCE AMOUNT",s)) 
					{
						temp=famt(fields->enc,*len_member->value.short_value);
						sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,temp);
						if(temp!=NULL) Rfree(temp);
						lcount+=*len_member->value.short_value+1;
					} else if(!RDAstrcmp("PRE-ENCUMBRANCE AMOUNT",s)) 
					{
						temp=famt(fields->pre_enc,*len_member->value.short_value);
						sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,temp);
						if(temp!=NULL) Rfree(temp);
						lcount+=*len_member->value.short_value+1;
					} else if(!RDAstrcmp("DEFAULT DEBIT CODE",s)) 
					{
						if(*len_member->value.short_value<RDAstrlen(fields->account))
						{
							sprintf(&hold[lcount],"%.*s ",*len_member->value.short_value,(fields->defdebit!=NULL?fields->defdebit:""));
						} else {
							sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,(fields->defdebit!=NULL?fields->defdebit:""));
						}
						lcount+=*len_member->value.short_value+1;
					} else if(!RDAstrcmp("DEFAULT CREDIT CODE",s)) 
					{
						if(*len_member->value.short_value<RDAstrlen(fields->account))
						{
							sprintf(&hold[lcount],"%.*s ",*len_member->value.short_value,(fields->defcredit!=NULL?fields->defcredit:""));
						} else {
							sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,(fields->defcredit!=NULL?fields->defcredit:""));
						}
						lcount+=*len_member->value.short_value+1;
					} else if(!RDAstrcmp("DEFAULT PAYABLE CODE",s)) 
					{
						if(*len_member->value.short_value<RDAstrlen(fields->account))
						{
							sprintf(&hold[lcount],"%.*s ",*len_member->value.short_value,(fields->defpayable!=NULL?fields->defpayable:""));
						} else {
							sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,(fields->defpayable!=NULL?fields->defpayable:""));
						}
						lcount+=*len_member->value.short_value+1;
					} else if(!RDAstrcmp("DEFAULT BANK IDENTIFICATION",s)) 
					{
						if(*len_member->value.short_value<RDAstrlen(fields->account))
						{
							sprintf(&hold[lcount],"%.*s ",*len_member->value.short_value,(fields->defbankid!=NULL?fields->defbankid:""));
						} else {
							sprintf(&hold[lcount],"%-*s ",*len_member->value.short_value,(fields->defbankid!=NULL?fields->defbankid:""));
						}
						lcount+=*len_member->value.short_value+1;
					} else {
						prterr("Error field [%s] not found.",s);
						if(s!=NULL) Rfree(s);
					}
				}
			}
		}
		++cur;
	}
	addAPPlib(acclst,hold);
	if(hold!=NULL) Rfree(hold);
/*
		for(x=0,fields=ACCOUNTS->a;x<ACCOUNTS->num;++x,++fields)
		{
			amt=famt(fields->enc,15);
			if(fields->prorate)
			{
				l=((fields->account!=NULL ? RDAstrlen(fields->account):0)+
					(fields->debit!=NULL ? RDAstrlen(fields->debit):0)+
					(fields->credit!=NULL ? RDAstrlen(fields->credit):0)+
					(amt!=NULL ? RDAstrlen(amt):0)+
					(fields->defdebit!=NULL ? RDAstrlen(fields->defdebit):0)+
					(fields->defcredit!=NULL ? RDAstrlen(fields->defcredit):0)+
					(fields->defpayable!=NULL ? RDAstrlen(fields->defpayable):0)+
					(fields->defbankid!=NULL ? RDAstrlen(fields->defbankid):0)+81);
				s=Rmalloc(l);
				sprintf(s,"Acc# [%d] Def# [%d] %s %s %s Enc: %s Prorated: %f (%s %s %s %s)",
					x,fields->deftype,
					(fields->account!=NULL?fields->account:""),
					(fields->debit!=NULL?fields->debit:""),
					(fields->credit!=NULL?fields->credit:""),
					amt,fields->percentage,
					(fields->defdebit!=NULL?fields->defdebit:""),
					(fields->defcredit!=NULL?fields->defcredit:""),
					(fields->defpayable!=NULL?fields->defpayable:""),
					(fields->defbankid!=NULL?fields->defbankid:""));
			} else if(((x+1)>=ACCOUNTS->num) && fields->amount==0.0)
			{
				l=((fields->account!=NULL ? RDAstrlen(fields->account):0)+
					(fields->debit!=NULL ? RDAstrlen(fields->debit):0)+
					(fields->credit!=NULL ? RDAstrlen(fields->credit):0)+
					(amt!=NULL ? RDAstrlen(amt):0)+
					(fields->defdebit!=NULL ? RDAstrlen(fields->defdebit):0)+
					(fields->defcredit!=NULL ? RDAstrlen(fields->defcredit):0)+
					(fields->defpayable!=NULL ? RDAstrlen(fields->defpayable):0)+
					(fields->defbankid!=NULL ? RDAstrlen(fields->defbankid):0)+80);
				s=Rmalloc(l);
				sprintf(s,"Acc# [%d] Def# [%d] %s %s %s Enc: %s All Remaining (%s %s %s %s)",
					x,fields->deftype,
					(fields->account!=NULL?fields->account:""),
					(fields->debit!=NULL?fields->debit:""),
					(fields->credit!=NULL?fields->credit:""),
					(fields->defdebit!=NULL?fields->defdebit:""),amt,
					(fields->defcredit!=NULL?fields->defcredit:""),
					(fields->defpayable!=NULL?fields->defpayable:""),
					(fields->defbankid!=NULL?fields->defbankid:""));
			} else {
				temp=famt(fields->amount,15);
				l=((fields->account!=NULL?RDAstrlen(fields->account):0)+
					(fields->debit!=NULL?RDAstrlen(fields->debit):0)+
					(fields->credit!=NULL?RDAstrlen(fields->credit):0)+
					(temp!=NULL?RDAstrlen(temp):0)+(amt!=NULL?RDAstrlen(amt):0)+
					(fields->defdebit!=NULL?RDAstrlen(fields->defdebit):0)+
					(fields->defcredit!=NULL?RDAstrlen(fields->defcredit):0)+
					(fields->defpayable!=NULL?RDAstrlen(fields->defpayable):0)+
					(fields->defbankid!=NULL?RDAstrlen(fields->defbankid):0)+
					80);
				s=Rmalloc(l);
				sprintf(s,"Acc# [%d] Def# [%d] %s %s %s Enc: %s Next Amt Up To: %s (%s %s %s %s)",
					x,fields->deftype,
					(fields->account!=NULL?fields->account:""),
					(fields->debit!=NULL?fields->debit:""),
					(fields->credit!=NULL?fields->credit:""),
					amt,temp,
					(fields->defdebit!=NULL?fields->defdebit:""),
					(fields->defcredit!=NULL?fields->defcredit:""),
					(fields->defpayable!=NULL?fields->defpayable:""),
					(fields->defbankid!=NULL?fields->defbankid:""));
				if(temp!=NULL) Rfree(temp);
			}
			if(amt!=NULL) Rfree(amt);
			addAPPlib(acclst,s);
			if(s!=NULL) Rfree(s);
		}
	}
	if(acclst->numlibs<1)
	{
		addAPPlib(acclst,"No Accounts Defined");
	}
*/
}
void makeaccountlist()
{
	int x=0;
	ReqAcctStruct *fields=NULL;

	if(acclst!=NULL) freeapplib(acclst);
	acclst=APPlibNEW();
	if(ACCOUNTS->a!=NULL)
	{
		for(x=0,fields=ACCOUNTS->a;x<ACCOUNTS->num;++x,++fields)
		{
			mkacclst(fields);
		}
	}
	if(acclst->numlibs<1)
	{
		addAPPlib(acclst,"No Accounts Defined");
	}
}
static void makespaceaccountlist(int pos)
{
	short x,y;
	ReqAcctStruct *temp=NULL,*temp1=NULL,*new_fields=NULL;

	++ACCOUNTS->num;
	new_fields=Rmalloc(ACCOUNTS->num*sizeof(ReqAcctStruct));
	y=0;
	for(x=0,temp=ACCOUNTS->a;x<(ACCOUNTS->num-1);++x,++temp)
	{
		temp1=new_fields+y;
		if(x==pos)
		{
			temp1->deftype=0;
			temp1->account=NULL;
			temp1->debit=NULL;
			temp1->credit=NULL;
			temp1->prorate=FALSE;
			temp1->percentage=0;
			temp1->amount=0;
			temp1->enc=0;
			++y;
			temp1=new_fields+y;
		}
		temp1->deftype=temp->deftype;
		if(!isEMPTY(temp->account))
		{
			temp1->account=stralloc(temp->account);
			Rfree(temp->account);
		} else temp1->account=NULL;
		if(!isEMPTY(temp->debit))
		{
			temp1->debit=stralloc(temp->debit);
			Rfree(temp->debit);
		} else temp1->debit=NULL;
		if(!isEMPTY(temp->credit))
		{
			temp1->credit=stralloc(temp->credit);
			Rfree(temp->credit);
		} else temp1->credit=NULL;
		temp1->prorate=temp->prorate;
		temp1->percentage=temp->percentage;
		temp1->amount=temp->amount;
		temp1->enc=temp->enc;
		if(!isEMPTY(temp->defdebit))
		{
			temp1->defdebit=stralloc(temp->defdebit);
			Rfree(temp->defdebit);
		} else temp1->defdebit=NULL;
		if(!isEMPTY(temp->defcredit))
		{
			temp1->defcredit=stralloc(temp->defcredit);
			Rfree(temp->defcredit);
		} else temp1->defcredit=NULL;
		if(!isEMPTY(temp->defpayable))
		{
			temp1->defpayable=stralloc(temp->defpayable);
			Rfree(temp->defpayable);
		} else temp1->defpayable=NULL;
		if(!isEMPTY(temp->defbankid))
		{
			temp1->defbankid=stralloc(temp->defbankid);
			Rfree(temp->defbankid);
		} else temp1->defbankid=NULL;
		++y;
	}
	Rfree(ACCOUNTS->a);
	ACCOUNTS->a=Rmalloc(ACCOUNTS->num*sizeof(ReqAcctStruct));
	for(x=0,temp1=ACCOUNTS->a,temp=new_fields;x<ACCOUNTS->num;++x,
		++temp,++temp1)
	{
		temp1->deftype=temp->deftype;
		if(!isEMPTY(temp->account))
		{
			temp1->account=stralloc(temp->account);
			Rfree(temp->account);
		} else temp1->account=NULL;
		if(!isEMPTY(temp->debit))
		{
			temp1->debit=stralloc(temp->debit);
			Rfree(temp->debit);
		} else temp1->debit=NULL;
		if(!isEMPTY(temp->credit))
		{
			temp1->credit=stralloc(temp->credit);
			Rfree(temp->credit);
		} else temp1->credit=NULL;
		temp1->prorate=temp->prorate;
		temp1->percentage=temp->percentage;
		temp1->amount=temp->amount;
		temp1->enc=temp->enc;
		if(!isEMPTY(temp->defdebit))
		{
			temp1->defdebit=stralloc(temp->defdebit);
			Rfree(temp->defdebit);
		} else temp1->defdebit=NULL;
		if(!isEMPTY(temp->defcredit))
		{
			temp1->defcredit=stralloc(temp->defcredit);
			Rfree(temp->defcredit);
		} else temp1->defcredit=NULL;
		if(!isEMPTY(temp->defpayable))
		{
			temp1->defpayable=stralloc(temp->defpayable);
			Rfree(temp->defpayable);
		} else temp1->defpayable=NULL;
		if(!isEMPTY(temp->defpayable))
		{
			temp1->defpayable=stralloc(temp->defpayable);
			Rfree(temp->defpayable);
		} else temp1->defpayable=NULL;
	}
	Rfree(new_fields);
}
static void quit_account(RDArsrc *accrsrc,RDArsrc *parent)
{
	if(accrsrc!=NULL) 
	{
		killwindow(accrsrc);
		free_rsrc(accrsrc);
	}
	if(accdefaults!=NULL) FreeRDAdefault(accdefaults);
	if(DEFtypes!=NULL) freeapplib(DEFtypes);
}
static void quit_accounttest(RDArsrc *accrsrc,RDArsrc *parent)
{
	readallwidgets(accrsrc);
	if(COMPARE_RSRCS(accrsrc,accdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the Account Definition.\nDo you want to Save these changes?",save_account,quit_account,FALSE,2,accrsrc,parent,NULL);
	} else {
		quit_account(accrsrc,parent);
	}
}
static void save_account(RDArsrc *accrsrc,RDArsrc *parent)
{
	int value=0,selecteddef=0;
	ReqAcctStruct *i=NULL;
	double amt=0.0;

	if(FINDRSCGETINT(parent,"ACCOUNT LIST",&value)) return;
	readwidget(parent,"[REQITM][AMOUNT]");
	FINDRSCGETDOUBLE(parent,"[REQITM][AMOUNT]",&amt);
	i=ACCOUNTS->a+value;
	readallwidgets(accrsrc);
	i->accno=value;
	FINDRSCGETINT(accrsrc,"DEFINITION TYPES",&selecteddef);
	i->deftype=(short)selecteddef;
	FINDRSCGETSTRING(accrsrc,"ACCOUNT CODE",&i->account);
	FINDRSCGETSTRING(accrsrc,"DEBIT CODE",&i->debit);
	FINDRSCGETSTRING(accrsrc,"CREDIT CODE",&i->credit);
	FINDRSCGETCHAR(accrsrc,"PRORATE",&i->prorate);
	if(i->prorate)
	{
		FINDRSCGETDOUBLE(accrsrc,"PERCENTAGE",&i->percentage);
		i->amount=0.0;
	} else {
		FINDRSCGETDOUBLE(accrsrc,"AMOUNT",&i->amount);
		i->percentage=0.0;
	}
	FINDRSCGETSTRING(accrsrc,"DEFAULT DEBIT CODE",&i->defdebit);
	FINDRSCGETSTRING(accrsrc,"DEFAULT CREDIT CODE",&i->defcredit);
	FINDRSCGETSTRING(accrsrc,"DEFAULT PAYABLE CODE",&i->defpayable);
	FINDRSCGETSTRING(accrsrc,"DEFAULT BANK IDENTIFICATION",&i->defbankid);
	ComputeREQACC(ACCOUNTS,amt);
	makeaccountlist();
	if(!FINDRSCLISTAPPlib(parent,"ACCOUNT LIST",value,acclst))
		updatersrc(parent,"ACCOUNT LIST");
	if(COMPARE_RSRCS(accrsrc,accdefaults,2))
	{
		changedvalues=TRUE;
	}
	quit_account(accrsrc,parent);
}
static void save_account_above(RDArsrc *accrsrc,RDArsrc *parent)
{
	int x;

	if(FINDRSCGETINT(parent,"ACCOUNT LIST",&x)) return;
	makespaceaccountlist(x);
	save_account(accrsrc,parent);
}
static void save_account_below(RDArsrc *accrsrc,RDArsrc *parent)
{
	int x;

	if(FINDRSCGETINT(parent,"ACCOUNT LIST",&x)) return;
	makespaceaccountlist(++x);
	FINDRSCSETINT(parent,"ACCOUNT LIST",x);
	updatersrc(parent,"ACCOUNT LIST");
	save_account(accrsrc,parent);
}
static void quit_account_abovetest(RDArsrc *accrsrc,RDArsrc *parent)
{
	readallwidgets(accrsrc);
	if(COMPARE_RSRCS(accrsrc,accdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the Account Definition.\nDo you want to Save these changes?",save_account_above,quit_account,FALSE,2,accrsrc,parent,NULL);
	} else {
		quit_account(accrsrc,parent);
	}
}
static void quit_account_belowtest(RDArsrc *accrsrc,RDArsrc *parent)
{
	readallwidgets(accrsrc);
	if(COMPARE_RSRCS(accrsrc,accdefaults,2))
	{
		ADVWARNINGDIALOG("SAVE WARNING DIALOG SCREEN","CHANGES NOT SAVED!","Changes have been made to the Account Definition.\nDo you want to Save these changes?",save_account_below,quit_account,FALSE,2,accrsrc,parent,NULL);
	} else {
		quit_account(accrsrc,parent);
	}
}
static void setprorate(RDArsrc *accrsrc)
{
	char prorate=FALSE;

	readwidget(accrsrc,"PRORATE");
	FINDRSCGETCHAR(accrsrc,"PRORATE",&prorate);
	if(prorate)
	{
		FINDRSCSETEDITABLE(accrsrc,"AMOUNT",FALSE);
		FINDRSCSETSENSITIVE(accrsrc,"AMOUNT",FALSE);
		FINDRSCSETSENSITIVE(accrsrc,"PERCENTAGE",TRUE);
		FINDRSCSETEDITABLE(accrsrc,"PERCENTAGE",TRUE);
	} else {
		FINDRSCSETEDITABLE(accrsrc,"PERCENTAGE",FALSE);
		FINDRSCSETSENSITIVE(accrsrc,"PERCENTAGE",FALSE);
		FINDRSCSETSENSITIVE(accrsrc,"AMOUNT",TRUE);
		FINDRSCSETEDITABLE(accrsrc,"AMOUNT",TRUE);
	}
}
static void accountscreen(RDArsrc *parent,short deftype,
	char *accountx,char *debitx,char *creditx,char prorate,double perc,
	double amount,char *defdebitx,char *defcreditx,char *defpayablex,
	char *defbankid,void (*savefunc)(...),void (*quitfunctest)(...))
{
	int y=0,x=0,start=0,selBANKid=0;
	RDAacct *acc=NULL;
	RDArsrc *accrsrc=NULL;
	short edit_rsrc=TRUE;

	accrsrc=RDArsrcNEW(module,"ADD REQUISITION MAINTAIN ACCOUNTS W/O LISTS");
	addDFincvir(accrsrc,"FINMGT","FINBACC",NULL,balnum);
	addDFincvir(accrsrc,"FINMGT","FINEACC",NULL,expnum);
	addDFincvir(accrsrc,"FINMGT","FINBYR",NULL,byrnum);
	addDFincvir(accrsrc,"FINMGT","FINEYR",NULL,eyrnum);
	addDFincvir(accrsrc,"FINMGT","FINYEAR",NULL,finyrnum);
	GET_SCREEN_VIRTUALS(accrsrc,0);
	if(DEFtypes!=NULL) freeapplib(DEFtypes);
	DEFtypes=APPlibNEW();
	if(EXPENDITURE_ACCOUNT!=NULL)
	{
		if(EXPENDITURE_ACCOUNT->codetype!=NULL)
		{
			for(x=0,acc=EXPENDITURE_ACCOUNT->codetype;
				x<EXPENDITURE_ACCOUNT->num;++x,++acc)
			{
				addAPPlib(DEFtypes,acc->name);
			}
		}
	} 
	if(DEFtypes->numlibs<1)
	{
		addAPPlib(DEFtypes,"No Definitions Available");
	}
	start=deftype;
	addlstrsrc(accrsrc,"DEFINITION TYPES",&start,TRUE,DEFtypecb,
		DEFtypes->numlibs,&DEFtypes->libs,NULL);
	addstdrsrc(accrsrc,"ACCOUNT DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
	addstdrsrc(accrsrc,"ACCOUNT CODE",EXPENDITURE,40,accountx,TRUE);
	FINDRSCSETFUNC(accrsrc,"ACCOUNT CODE",acctsearchcb,&acctcodenum);
	addstdrsrc(accrsrc,"DEBIT CODE DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
	addstdrsrc(accrsrc,"DEBIT CODE",BALANCESHEET,40,debitx,TRUE);
	debitcodenum=1;
	FINDRSCSETFUNC(accrsrc,"DEBIT CODE",acctsearchcb,&debitcodenum);
	addstdrsrc(accrsrc,"CREDIT CODE DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
	addstdrsrc(accrsrc,"CREDIT CODE",BALANCESHEET,40,creditx,TRUE);
	creditcodenum=2;
	FINDRSCSETFUNC(accrsrc,"CREDIT CODE",acctsearchcb,&creditcodenum);
	addstdrsrc(accrsrc,"DEFAULT DEBIT CODE DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
	addstdrsrc(accrsrc,"DEFAULT DEBIT CODE",BALANCESHEET,40,defdebitx,TRUE);
	defdebitcodenum=3;
	FINDRSCSETFUNC(accrsrc,"DEFAULT DEBIT CODE",acctsearchcb,&defdebitcodenum);
	addstdrsrc(accrsrc,"DEFAULT CREDIT CODE DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
	addstdrsrc(accrsrc,"DEFAULT CREDIT CODE",BALANCESHEET,40,defcreditx,TRUE);
	defcreditcodenum=4;
	FINDRSCSETFUNC(accrsrc,"DEFAULT CREDIT CODE",acctsearchcb,&defcreditcodenum);
	addstdrsrc(accrsrc,"DEFAULT PAYABLE CODE DESCRIPTION",VARIABLETEXT,40,NULL,FALSE);
	addstdrsrc(accrsrc,"DEFAULT PAYABLE CODE",BALANCESHEET,40,defpayablex,TRUE);
	defpayablecodenum=5;
	FINDRSCSETFUNC(accrsrc,"DEFAULT PAYABLE CODE",acctsearchcb,&defpayablecodenum);
	addstdrsrc(accrsrc,"DEFAULT BANK IDENTIFICATION",VARIABLETEXT,0,defbankid,(edit_rsrc==TRUE?TRUE:FALSE));
	if(banknum!=(-1))
	{
		file2rsrc(banknum,accrsrc,FALSE);
		getBANKids(accrsrc);
		start=0;
		addlstrsrc(accrsrc,
			"DEFAULT BANK IDENTIFICATIONS",
			&start,(edit_rsrc==TRUE?TRUE:FALSE),
			BANKidscb,BANKids->numlibs,
			&BANKids->libs,NULL);
			FINDRSCSETFUNC(accrsrc,"DEFAULT BANK IDENTIFICATION",setBANKidscb,NULL);
		if(!isEMPTY(defbankid)) 
		{
			setBANKidscb(accrsrc);
		} else {
			FINDRSCGETINT(accrsrc,"BANK IDENTIFICATIONS",&selBANKid);
			if(selBANKid>=BANKids->numlibs) selBANKid=0;
			if(RDAstrcmp(BANKids->libs[selBANKid],"No Banks Defined."))
			{
				FINDRSCSETSTRING(accrsrc,"DEFAULT BANK IDENTIFICATION",BANKids->libs[selBANKid]);
			} else {
				FINDRSCSETSTRING(accrsrc,"DEFAULT BANK IDENTIFICATION",NULL);
			}
		}
	}
	addstdrsrc(accrsrc,"PRORATE",BOOLNS,1,&prorate,TRUE);
	FINDRSCSETFUNC(accrsrc,"PRORATE",setprorate,NULL);
	addstdrsrc(accrsrc,"PERCENTAGE",DECIMALV,8,&perc,TRUE);
	addstdrsrc(accrsrc,"AMOUNT",DOLLARS,20,&amount,TRUE);
	setprorate(accrsrc);
	FINDRSCGETINT(parent,"FISCAL YEARS",&y);
	addlstrsrc(accrsrc,"FISCAL YEARS",&y,FALSE,NULL,
		yearavl->numlibs,&yearavl->libs,NULL);
	FINDRSCGETINT(parent,"FISCAL YEARS",&y);
	FINDRSCSETINT(accrsrc,"FISCAL YEARS",y);
	changeDEFtype(accrsrc,TRUE);
	FINDRSCSETSTRING(accrsrc,"ACCOUNT CODE",accountx);
	FINDRSCSETSTRING(accrsrc,"DEBIT CODE",debitx);
	FINDRSCSETSTRING(accrsrc,"CREDIT CODE",creditx);
	FINDRSCSETSTRING(accrsrc,"DEFAULT DEBIT CODE",defdebitx);
	FINDRSCSETSTRING(accrsrc,"DEFAULT CREDIT CODE",defcreditx);
	FINDRSCSETSTRING(accrsrc,"DEFAULT PAYABLE CODE",defpayablex);
	acctsearch(accrsrc,&acctcodenum,FALSE);
	acctsearch(accrsrc,&debitcodenum,FALSE);
	acctsearch(accrsrc,&creditcodenum,FALSE);
	acctsearch(accrsrc,&defdebitcodenum,FALSE);
	acctsearch(accrsrc,&defcreditcodenum,FALSE);
	acctsearch(accrsrc,&defpayablecodenum,FALSE);
	addbtnrsrc(accrsrc,"SAVE",TRUE,savefunc,parent);
	addbtnrsrc(accrsrc,"QUIT",TRUE,quitfunctest,parent);
	addbtnrsrc(accrsrc,"HELP",TRUE,screenhelp,NULL);
	addbtnrsrc(accrsrc,"PRINT RESOURCES",TRUE,printrsrcs,NULL);
	accdefaults=GetDefaults(accrsrc);
	if(!APPmakescrn(accrsrc,TRUE,quit_account,parent,FALSE))
	{
		setprorate(accrsrc);
	}
}
void add_account_above(RDArsrc *parent)
{
	int selected=0;
	double zer=0;

	if(FINDRSCGETINT(parent,"ACCOUNT LIST",&selected)) return;
	if(acclst->numlibs==1 && !RDAstrcmp(acclst->libs[0],"No Accounts Defined")) 
		return;
	accountscreen(parent,0,NULL,NULL,NULL,FALSE,zer,zer,
		NULL,NULL,NULL,NULL,save_account_above,quit_account_abovetest);
}
void add_account_below(RDArsrc *parent)
{
	int selected=0;
	double zer=0;

	if(FINDRSCGETINT(parent,"ACCOUNT LIST",&selected)) return;
	if(acclst->numlibs==1 && !RDAstrcmp(acclst->libs[0],"No Accounts Defined"))
		return;
	accountscreen(parent,0,NULL,NULL,NULL,FALSE,zer,zer,
		NULL,NULL,NULL,NULL,save_account_below,quit_account_belowtest);
}
void edit_account(RDArsrc *parent)
{
	int x;
	ReqAcctStruct *i=NULL;

	if(FINDRSCGETINT(parent,"ACCOUNT LIST",&x)) return;
	if(ACCOUNTS->a==NULL)
	{
		ACCOUNTS->a=Rmalloc(sizeof(ReqAcctStruct));
		i=ACCOUNTS->a;
		i->deftype=0;
		i->account=NULL;
		i->debit=NULL;
		i->credit=NULL;
		i->prorate=FALSE;
		i->percentage=0.0;
		i->amount=0.0;
		i->defdebit=NULL;
		i->defcredit=NULL;
		i->defpayable=NULL;
		i->defbankid=NULL;
		ACCOUNTS->num=1;
	}
	i=ACCOUNTS->a+x;
	accountscreen(parent,i->deftype,i->account,i->debit,i->credit,
		i->prorate,i->percentage,i->amount,
		i->defdebit,i->defcredit,i->defpayable,i->defbankid,
		save_account,quit_accounttest);
}
void delete_account(RDArsrc *parent)
{
	short x,y;
	ReqAcctStruct *temp=NULL,*temp1=NULL,*new_fields=NULL;
	int value=0;
	double amt=0.0;

	readallwidgets(parent);
	FINDRSCGETDOUBLE(parent,"[REQITM][AMOUNT]",&amt);
	if(FINDRSCGETINT(parent,"ACCOUNT LIST",&value)) return;
	if(ACCOUNTS->num<2) return;
	new_fields=Rmalloc((ACCOUNTS->num-1)*sizeof(ReqAcctStruct));
	y=0;
	for(x=0,temp=ACCOUNTS->a;x<ACCOUNTS->num;++x,++temp)
	{
		temp1=new_fields+y;
		if(x!=value)
		{
			temp1->deftype=temp->deftype;
			if(!isEMPTY(temp->account))
			{
				temp1->account=stralloc(temp->account);
				Rfree(temp->account);
			} else temp1->account=NULL;
			if(!isEMPTY(temp->debit))
			{
				temp1->debit=stralloc(temp->debit);
				Rfree(temp->debit);
			} else temp1->debit=NULL;
			if(!isEMPTY(temp->credit))
			{
				temp1->credit=stralloc(temp->credit);
				Rfree(temp->credit);
			} else temp1->credit=NULL;
			temp1->prorate=temp->prorate;
			temp1->percentage=temp->percentage;
			temp1->amount=temp->amount;
			temp1->enc=temp->enc;
			if(!isEMPTY(temp->defdebit))
			{
				temp1->account=stralloc(temp->defdebit);
				Rfree(temp->defdebit);
			} else temp1->defdebit=NULL;
			if(!isEMPTY(temp->defcredit))
			{
				temp1->defcredit=stralloc(temp->defcredit);
				Rfree(temp->defcredit);
			} else temp1->defcredit=NULL;
			if(!isEMPTY(temp->defpayable))
			{
				temp1->defpayable=stralloc(temp->defpayable);
				Rfree(temp->defpayable);
			} else temp1->defpayable=NULL;
			if(!isEMPTY(temp->defbankid))
			{
				temp1->defbankid=stralloc(temp->defbankid);
				Rfree(temp->defbankid);
			} else temp1->defbankid=NULL;
			++y;
		} else {
			if(temp->account!=NULL) Rfree(temp->account);
			if(temp->debit!=NULL) Rfree(temp->debit);
			if(temp->credit!=NULL) Rfree(temp->credit);
			if(temp->defdebit!=NULL) Rfree(temp->defdebit);
			if(temp->defcredit!=NULL) Rfree(temp->defcredit);
			if(temp->defpayable!=NULL) Rfree(temp->defpayable);
			if(temp->defbankid!=NULL) Rfree(temp->defbankid);
		}
	}
	Rfree(ACCOUNTS->a);
	--ACCOUNTS->num;
	ACCOUNTS->a=Rmalloc(ACCOUNTS->num*sizeof(ReqAcctStruct));
	for(x=0,temp1=ACCOUNTS->a,temp=new_fields;x<ACCOUNTS->num;++x,
		++temp,++temp1)
	{
		temp1->deftype=temp->deftype;
		if(!isEMPTY(temp->account))
		{
			temp1->account=stralloc(temp->account);
			Rfree(temp->account);
		} else temp1->account=NULL;
		if(!isEMPTY(temp->debit))
		{
			temp1->debit=stralloc(temp->debit);
			Rfree(temp->debit);
		} else temp1->debit=NULL;
		if(!isEMPTY(temp->credit))
		{
			temp1->credit=stralloc(temp->credit);
			Rfree(temp->credit);
		} else temp1->credit=NULL;
		temp1->prorate=temp->prorate;
		temp1->percentage=temp->percentage;
		temp1->amount=temp->amount;
		if(!isEMPTY(temp->defdebit))
		{
			temp1->defdebit=stralloc(temp->defdebit);
			Rfree(temp->defdebit);
		} else temp1->defdebit=NULL;
		if(!isEMPTY(temp->defcredit))
		{
			temp1->defcredit=stralloc(temp->defcredit);
			Rfree(temp->defcredit);
		} else temp1->defcredit=NULL;
	}
	Rfree(new_fields);
	ComputeREQACC(ACCOUNTS,amt);
	makeaccountlist();
	if(value>=acclst->numlibs) value=0;
	if(!FINDRSCLISTAPPlib(parent,"ACCOUNT LIST",value,acclst))
		updatersrc(parent,"ACCOUNT LIST");
	changedvalues=TRUE;
}
