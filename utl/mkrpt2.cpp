/* Printer Definition (EPSON-OLD DOT MATRIX)  */
#include <cstdio>
#include <mkmsc.hpp>
#ifdef WIN32
#include <sys/types.h>
#include <sys/stat.h>
#endif

extern APPlib *errorlist;
#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */

void CheckFontViewPDF()
{
#ifdef WIN32
	if(AbsFileExists("\\rda\\fonts\\Arial.afm")) { prterr("Found \\rda\\fonts\\Arial.afm"); }	
	if(AbsFileExists("\\rda\\fonts\\Arial.pfb")) { prterr("Found \\rda\\fonts\\Arial.pfb"); }	
	if(AbsFileExists("\\rda\\fonts\\c0419bt_.afm")) { prterr("Found \\rda\\fonts\\c0419bt_.afm"); }	
	if(AbsFileExists("\\rda\\fonts\\c0419bt_.pfb")) { prterr("Found \\rda\\fonts\\c0419bt_.pfb"); }	

	if(!AbsFileExists("\\rda\\fonts\\Arial.afm")) { prterr("Not Found \\rda\\fonts\\Arial.afm"); }	
	if(!AbsFileExists("\\rda\\fonts\\Arial.pfb")) { prterr("Not Found \\rda\\fonts\\Arial.pfb"); }	
	if(!AbsFileExists("\\rda\\fonts\\c0419bt_.afm")) { prterr("Not Found \\rda\\fonts\\c0419bt_.afm"); }	
	if(!AbsFileExists("\\rda\\fonts\\c0419bt_.pfb")) { prterr("Not Found \\rda\\fonts\\c0419bt_.pfb"); }	

	if((!AbsFileExists("\\rda\\fonts\\Arial.afm")) || (!AbsFileExists("\\rda\\fonts\\Arial.pfb")) || (!AbsFileExists("\\rda\\fonts\\c0419bt_.afm")) || (!AbsFileExists("\\rda\\fonts\\c0419bt_.pfb")))
	{
		ShellExecute(NULL,"open","openrda-font-installer.exe",NULL,NULL,SW_SHOWNORMAL);
	}
#else
	if(AbsFileExists("/rda/fonts/Arial.afm")) { prterr("Found /rda/fonts/Arial.afm"); }	
	if(AbsFileExists("/rda/fonts/Arial.pfb")) { prterr("Found /rda/fonts/Arial.pfb"); }	
	if(AbsFileExists("/rda/fonts/c0419bt_.afm")) { prterr("Found /rda/fonts/c0419bt_.afm"); }	
	if(AbsFileExists("/rda/fonts/c0419bt_.pfb")) { prterr("Found /rda/fonts/c0419bt_.pfb"); }	

	if(!AbsFileExists("/rda/fonts/Arial.afm")) { prterr("Not Found /rda/fonts/Arial.afm"); }	
	if(!AbsFileExists("/rda/fonts/Arial.pfb")) { prterr("Not Found /rda/fonts/Arial.pfb"); }	
	if(!AbsFileExists("/rda/fonts/c0419bt_.afm")) { prterr("Not Found /rda/fonts/c0419bt_.afm"); }	
	if(!AbsFileExists("/rda/fonts/c0419bt_.pfb")) { prterr("Not Found /rda/fonts/c0419bt_.pfb"); }	
#endif
}

void IBM_EMULATION()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("IBM EMULATION");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		addRDApvar(PRTDEF,"NORMAL TEXT","\\18\\27\\50\\27\\65\\10",1,10,12);
		addRDApvar(PRTDEF,"COMPRESSED TEXT","\\15\\27\\48",1,17,9);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(20+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [EPSON-OLD DOT MATRIX], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void EPSON_OLD_DOT_MATRIX()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("EPSON-OLD DOT MATRIX");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		addRDApvar(PRTDEF,"NORMAL TEXT","\\18",1,10,12);
		addRDApvar(PRTDEF,"COMPRESSED TEXT","\\15",1,17,9);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(20+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [EPSON-OLD DOT MATRIX], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void EPSON_DFX1()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("EPSON DFX1");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		addRDApvar(PRTDEF,"NLQ","\\27\\120\\1",0,0,0);
		addRDApvar(PRTDEF,"DRAFT","\\27\\120\\0",0,0,0);
		addRDApvar(PRTDEF,"LEFT JUSTIFY","\\27\\97\\48",0,0,0);
		addRDApvar(PRTDEF,"CENTER TEXT","\\27\\97\\49",0,0,0);
		addRDApvar(PRTDEF,"RIGHT JUSTIFY","\\27\\97\\50",0,0,0);
		addRDApvar(PRTDEF,"ITALICS","\\27\\52",0,0,0);
		addRDApvar(PRTDEF,"ITALICS OFF","\\27\\53",0,0,0);
		addRDApvar(PRTDEF,"NLQ ROMAN","\\27\\107\\0",0,0,0);
		addRDApvar(PRTDEF,"NLQ SANS SERIF","\\27\\107\\1",0,0,0);
		addRDApvar(PRTDEF,"NLQ COURIER","\\27\\107\\2",0,0,0);
		addRDApvar(PRTDEF,"NLQ PRESTIGE","\\27\\107\\3",0,0,0);
		addRDApvar(PRTDEF,"NLQ SCRIPT","\\27\\107\\4",0,0,0);
		addRDApvar(PRTDEF,"NLQ BOLD PS","\\27\\107\\6",0,0,0);
		addRDApvar(PRTDEF,"NORMAL TEXT","\\18",1,10,12);
		addRDApvar(PRTDEF,"COMPRESSED TEXT","\\15",1,17,9);
		addRDApvar(PRTDEF,"UNDERLINE START","\\27\\45\\1",0,0,0);
		addRDApvar(PRTDEF,"UNDERLINE STOP","\\27\\45\\0",0,0,0);
		addRDApvar(PRTDEF,"NO LEFT MARGIN","\\27\\108\\0",0,0,0);
		addRDApvar(PRTDEF,"PICA NO RIGHT MARGIN","\\27\\81\\132",0,0,0);
		addRDApvar(PRTDEF,"PICA NO MARGINS","\\27\\88\\0\\132",0,0,0);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(10+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [EPSON DFX1], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void EPSON_EMULATION()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("EPSON LQ EMULATION");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		addRDApvar(PRTDEF,"NLQ","\\27\\120\\1",0,0,0);
		addRDApvar(PRTDEF,"DRAFT","\\27\\120\\0",0,0,0);
		addRDApvar(PRTDEF,"ITALICS","\\27\\52",0,0,0);
		addRDApvar(PRTDEF,"ITALICS OFF","\\27\\53",0,0,0);
		addRDApvar(PRTDEF,"NLQ ROMAN","\\27\\107\\0",0,0,0);
		addRDApvar(PRTDEF,"NLQ SANS SERIF","\\27\\107\\1",0,0,0);
		addRDApvar(PRTDEF,"NLQ COURIER","\\27\\107\\2",0,0,0);
		addRDApvar(PRTDEF,"NLQ PRESTIGE","\\27\\107\\3",0,0,0);
		addRDApvar(PRTDEF,"NLQ SCRIPT","\\27\\107\\4",0,0,0);
		addRDApvar(PRTDEF,"NLQ BOLD PS","\\27\\107\\6",0,0,0);
		addRDApvar(PRTDEF,"NORMAL TEXT","\\18\\27\\50",1,10,12);
		addRDApvar(PRTDEF,"COMPRESSED TEXT","\\15\\27\\48",1,17,9);
		addRDApvar(PRTDEF,"UNDERLINE START","\\27\\45\\1",0,0,0);
		addRDApvar(PRTDEF,"UNDERLINE STOP","\\27\\45\\0",0,0,0);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [EPSON EMULATION], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void EPSON_STAR_NX1000C()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("EPSON-STAR NX1000C");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		addRDApvar(PRTDEF,"NLQ","\\27\\120\\1",0,0,0);
		addRDApvar(PRTDEF,"DRAFT","\\27\\73\\0",0,0,0);
		addRDApvar(PRTDEF,"ITALICS","\\40\\40\\73\\41\\41\\49",0,0,0);
		addRDApvar(PRTDEF,"ITALICS OFF","\\40\\40\\73\\41\\41\\48",0,0,0);
		addRDApvar(PRTDEF,"NLQ COURIER","\\27\\107\\0",0,0,0);
		addRDApvar(PRTDEF,"NLQ SANS SERIF","\\40\\40\\70\\41\\41\\49",0,0,0);
		addRDApvar(PRTDEF,"NLQ ORATOR W/SMALL CAPS","\\40\\40\\70\\41\\41\\50",0,0,0);
		addRDApvar(PRTDEF,"NLQ ORATOR","\\40\\40\\70\\41\\41\\51",0,0,0);
		addRDApvar(PRTDEF,"NLQ SCRIPT","\\27\\107\\4",0,0,0);
		addRDApvar(PRTDEF,"NLQ BOLD PS","\\27\\107\\6",0,0,0);
		addRDApvar(PRTDEF,"NORMAL TEXT","\\18",1,10,12);
		addRDApvar(PRTDEF,"EMPHASIZED PRINTING ON","\\27\\69",0,0,0);
		addRDApvar(PRTDEF,"EMPHASIZED PRINTING OFF","\\27\\70",0,0,0);
		addRDApvar(PRTDEF,"DOUBLE STRIKE ON","\\27\\71",0,0,0);
		addRDApvar(PRTDEF,"DOUBLE STRIKE OFF","\\27\\72",0,0,0);
		addRDApvar(PRTDEF,"COMPRESSED TEXT","\\15",1,17,9);
		addRDApvar(PRTDEF,"UNDERLINE START","\\27\\45\\1",0,0,0);
		addRDApvar(PRTDEF,"UNDERLINE STOP","\\27\\45\\0",0,0,0);
		addRDApvar(PRTDEF,"OVERLINING START","\\27\\95\\48",0,0,0);
		addRDApvar(PRTDEF,"OVERLINING STOP","\\27\\95\\48",0,0,0);
		addRDApvar(PRTDEF,"SUPERSCRIPT START","\\27\\83\\0",0,0,0);
		addRDApvar(PRTDEF,"SUBSCRIPT STOP","\\27\\83\\1",0,0,0);
		addRDApvar(PRTDEF,"SUPER/SUB STOP","\\27\\84",0,0,0);
		addRDApvar(PRTDEF,"BLACK","\\27\\114\\0",0,0,0);
		addRDApvar(PRTDEF,"RED","\\27\\114\\1",0,0,0);
		addRDApvar(PRTDEF,"BLUE","\\27\\114\\2",0,0,0);
		addRDApvar(PRTDEF,"VOILET","\\27\\114\\3",0,0,0);
		addRDApvar(PRTDEF,"YELLOW","\\27\\114\\4",0,0,0);
		addRDApvar(PRTDEF,"ORANGE","\\27\\114\\5",0,0,0);
		addRDApvar(PRTDEF,"GREEN","\\27\\114\\6",0,0,0);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(18+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [EPSON-STAR NX1000C], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void HP_LASER_JET_III()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("HP LASER JET III");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		PRTDEF->page_length=10;
		addRDApvar(PRTDEF,"RESET PRINTER","\\027E",0,0,0);
		addRDApvar(PRTDEF,"NORMAL TEXT","\\027\\038\\108\\054\\068\\027\\038\\107\\048\\083",1,10,12);
		addRDApvar(PRTDEF,"COMPRESSED TEXT","\\027\\038\\108\\056\\068\\027\\038\\107\\050\\083",1,17,9);
		addRDApvar(PRTDEF,"PUSH CURSOR","\\027\\038\\102\\048\\083",0,0,0);
		addRDApvar(PRTDEF,"POP CURSOR","\\027\\038\\102\\049\\083",0,0,0);
		addRDApvar(PRTDEF,"ELITE PRINT","\\027\\038\\108\\056\\068\\027\\038\\107\\052\\083",256,12,9);
		addRDApvar(PRTDEF,"PORTRAIT","\\027\\038\\108\\048\\079",0,0,0);
		addRDApvar(PRTDEF,"LANDSCAPE","\\027\\038\\108\\049\\079",0,0,0);
		addRDApvar(PRTDEF,"REVERSE PORTRAIT","\\027\\038\\108\\050\\079",0,0,0);
		addRDApvar(PRTDEF,"REVERSE LANDSCAPE","\\027\\038\\108\\051\\079",0,0,0);
		addRDApvar(PRTDEF,"HALF LINE FEED","\\027\\061",0,0,0);
		addRDApvar(PRTDEF,"NEG Q LINE FEED","\\027\\038\\097\\045\\051\\048\\086",0,0,0);
		addRDApvar(PRTDEF,"POS Q LINE FEED","\\027\\038\\097\\043\\051\\048\\086",0,0,0);
		ADVaddRDApvar(PRTDEF,"FW2_01A.PCL","FW2_01A.PCL",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE LOCK","keylock.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE UNLOCK","keyunlock.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE FILE","./sig.dat",0,0,0,1);
		addRDApvar(PRTDEF,"SIMPLEX","\\027\\038\\108\\048\\083",0,0,0);
		addRDApvar(PRTDEF,"DUPLEX LONG","\\027\\038\\108\\049\\083",0,0,0);
		addRDApvar(PRTDEF,"DUPLEX SHORT","\\027\\038\\108\\050\\083",0,0,0);
		addRDApvar(PRTDEF,"ESCAPE","\\027",0,0,0);
#ifndef WIN32
		ADVaddRDApvar(PRTDEF,"W2FORM-4PP","./forms/W2FORM-4PP.pcl",0,0,0,1);
#endif
#ifdef WIN32
		ADVaddRDApvar(PRTDEF,"W2FORM-4PP",".\\forms\\W2FORM-4PP.pcl",0,0,0,1);
#endif
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(16+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [HP LASER JET III], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void HP_LASER_4000()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;
	char stemp2[101];
	FILE *fp=NULL;
	int w=0,x=0,y=0,z=0;;

	PRTDEF=RDApdevNEW("HP LASER 4000n");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		PRTDEF->page_length=10;
		addRDApvar(PRTDEF,"RESET PRINTER","\\027E",0,0,0);
		addRDApvar(PRTDEF,"SIGNATURE DIMM 1","\\027\\038\\102\\048\\083\\027\\038\\102\\049\\048\\048\\048\\051\\089\\027\\038\\102\\050\\088\\027\\038\\102\\049\\083",0,0,0);
		addRDApvar(PRTDEF,"SIGNATURE DIMM A","\\027&f10003Y\\027&f2X",0,0,0);
		addRDApvar(PRTDEF,"PUSH CURSOR","\\027\\038\\102\\048\\083",0,0,0);
		addRDApvar(PRTDEF,"POP CURSOR","\\027\\038\\102\\049\\083",0,0,0);
		addRDApvar(PRTDEF,"NORMAL TEXT","\\027\\038\\108\\054\\068\\027\\038\\107\\048\\083",1,10,12);
		addRDApvar(PRTDEF,"COMPRESSED TEXT","\\027\\038\\108\\056\\068\\027\\038\\107\\050\\083",1,17,9);
		addRDApvar(PRTDEF,"ELITE PRINT","\\027\\038\\108\\056\\068\\027\\038\\107\\052\\083",256,12,9);
		addRDApvar(PRTDEF,"PAGE_SIZE_LETTER","\\027\\038\\108\\050\\065",0,0,0);
		addRDApvar(PRTDEF,"PAGE_SIZE_LEGAL","\\027\\038\\108\\051\\065",0,0,0);
		addRDApvar(PRTDEF,"PAGE_SIZE_LEDGER","\\027\\038\\108\\054\\065",0,0,0);
		addRDApvar(PRTDEF,"PORTRAIT","\\027\\038\\108\\048\\079",0,0,0);
		addRDApvar(PRTDEF,"LANDSCAPE","\\027\\038\\108\\049\\079",0,0,0);
		addRDApvar(PRTDEF,"SIMPLEX","\\027\\038\\108\\048\\083",0,0,0);
		addRDApvar(PRTDEF,"DUPLEX LONG","\\027\\038\\108\\049\\083",0,0,0);
		addRDApvar(PRTDEF,"DUPLEX SHORT","\\027\\038\\108\\050\\083",0,0,0);
		addRDApvar(PRTDEF,"REVERSE PORTRAIT","\\027\\038\\108\\050\\079",0,0,0);
		addRDApvar(PRTDEF,"REVERSE LANDSCAPE","\\027\\038\\108\\051\\079",0,0,0);
		addRDApvar(PRTDEF,"ASCII SQUARE","\\254",0,0,0);
		addRDApvar(PRTDEF,"ESCAPE","\\027",0,0,0);
		addRDApvar(PRTDEF,"PCL JOB HEADER","\\027\\037\\045\\049\\050\\051\\052\\053\\088\\064\\080\\074\\076\\032\\069\\078\\084\\069\\082\\032\\076\\065\\078\\071\\085\\065\\071\\069\\061\\080\\067\\076\\013\\010\\027\\069",0,0,0);
		addRDApvar(PRTDEF,"PCL JOB FOOTER","\\027\\037\\045\\049\\050\\051\\052\\053\\088",0,0,0);
		addRDApvar(PRTDEF,"HALF LINE FEED","\\027\\061",0,0,0);
		addRDApvar(PRTDEF,"NEG Q LINE FEED","\\027\\038\\097\\045\\051\\048\\086",0,0,0);
		addRDApvar(PRTDEF,"POS Q LINE FEED","\\027\\038\\097\\043\\051\\048\\086",0,0,0);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE LOCK","keylock.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE UNLOCK","keyunlock.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE FILE","./sig.dat",0,0,0,1);
#ifndef WIN32
		ADVaddRDApvar(PRTDEF,"W2FORM-4PP","./forms/W2FORM-4PP.pcl",0,0,0,1);
#endif
#ifdef WIN32
		ADVaddRDApvar(PRTDEF,"W2FORM-4PP",".\\forms\\W2FORM-4PP.pcl",0,0,0,1);
#endif

		addRDApvar(PRTDEF,"USE DEFAULT PRIMARY FONT","\\027\\040\\051\\064",0,0,0);
		addRDApvar(PRTDEF,"USE FONT ARIAL 10","\\027\\040\\057\\085\\027\\040\\115\\049\\112\\049\\048\\118\\048\\115\\048\\098\\049\\054\\054\\048\\050\\084",0,0,0);
		addRDApvar(PRTDEF,"USE FONT ARIAL 12","\\027\\040\\057\\085\\027\\040\\115\\049\\112\\049\\050\\118\\048\\115\\048\\098\\049\\054\\054\\048\\050\\084",0,0,0);
		addRDApvar(PRTDEF,"USE FONT ARIAL 14","\\027\\040\\057\\085\\027\\040\\115\\049\\112\\049\\052\\118\\048\\115\\048\\098\\049\\054\\054\\048\\050\\084",0,0,0);
		addRDApvar(PRTDEF,"USE FONT ARIAL 16","\\027\\040\\057\\085\\027\\040\\115\\049\\112\\049\\054\\118\\048\\115\\048\\098\\049\\054\\054\\048\\050\\084",0,0,0);
		addRDApvar(PRTDEF,"USE FONT ARIAL 18","\\027\\040\\057\\085\\027\\040\\115\\049\\112\\049\\056\\118\\048\\115\\048\\098\\049\\054\\054\\048\\050\\084",0,0,0);

		addRDApvar(PRTDEF,"USE FONT ARIAL BOLD 10","\\027\\040\\057\\085\\027\\040\\115\\049\\112\\049\\048\\118\\048\\115\\051\\098\\049\\054\\054\\048\\050\\084",0,0,0);
		addRDApvar(PRTDEF,"USE FONT ARIAL BOLD 12","\\027\\040\\057\\085\\027\\040\\115\\049\\112\\049\\050\\118\\048\\115\\051\\098\\049\\054\\054\\048\\050\\084",0,0,0);
		addRDApvar(PRTDEF,"USE FONT ARIAL BOLD 14","\\027\\040\\057\\085\\027\\040\\115\\049\\112\\049\\052\\118\\048\\115\\051\\098\\049\\054\\054\\048\\050\\084",0,0,0);
		addRDApvar(PRTDEF,"USE FONT ARIAL BOLD 16","\\027\\040\\057\\085\\027\\040\\115\\049\\112\\049\\054\\118\\048\\115\\051\\098\\049\\054\\054\\048\\050\\084",0,0,0);
		addRDApvar(PRTDEF,"USE FONT ARIAL BOLD 18","\\027\\040\\057\\085\\027\\040\\115\\049\\112\\049\\056\\118\\048\\115\\051\\098\\049\\054\\054\\048\\050\\084",0,0,0);

		addRDApvar(PRTDEF,"USE FONT ARIAL ITALIC 10","\\027\\040\\057\\085\\027\\040\\115\\049\\112\\049\\048\\118\\049\\115\\048\\098\\049\\054\\054\\048\\050\\084",0,0,0);
		addRDApvar(PRTDEF,"USE FONT ARIAL ITALIC 12","\\027\\040\\057\\085\\027\\040\\115\\049\\112\\049\\050\\118\\049\\115\\048\\098\\049\\054\\054\\048\\050\\084",0,0,0);
		addRDApvar(PRTDEF,"USE FONT ARIAL ITALIC 14","\\027\\040\\057\\085\\027\\040\\115\\049\\112\\049\\052\\118\\049\\115\\048\\098\\049\\054\\054\\048\\050\\084",0,0,0);
		addRDApvar(PRTDEF,"USE FONT ARIAL ITALIC 16","\\027\\040\\057\\085\\027\\040\\115\\049\\112\\049\\054\\118\\049\\115\\048\\098\\049\\054\\054\\048\\050\\084",0,0,0);
		addRDApvar(PRTDEF,"USE FONT ARIAL ITALIC 18","\\027\\040\\057\\085\\027\\040\\115\\049\\112\\049\\056\\118\\049\\115\\048\\098\\049\\054\\054\\048\\050\\084",0,0,0);

		addRDApvar(PRTDEF,"USE FONT ARIAL BOLD ITALIC 10","\\027\\040\\057\\085\\027\\040\\115\\049\\112\\049\\048\\118\\049\\115\\051\\098\\049\\054\\054\\048\\050\\084",0,0,0);
		addRDApvar(PRTDEF,"USE FONT ARIAL BOLD ITALIC 12","\\027\\040\\057\\085\\027\\040\\115\\049\\112\\049\\050\\118\\049\\115\\051\\098\\049\\054\\054\\048\\050\\084",0,0,0);
		addRDApvar(PRTDEF,"USE FONT ARIAL BOLD ITALIC 14","\\027\\040\\057\\085\\027\\040\\115\\049\\112\\049\\052\\118\\049\\115\\051\\098\\049\\054\\054\\048\\050\\084",0,0,0);
		addRDApvar(PRTDEF,"USE FONT ARIAL BOLD ITALIC 16","\\027\\040\\057\\085\\027\\040\\115\\049\\112\\049\\054\\118\\049\\115\\051\\098\\049\\054\\054\\048\\050\\084",0,0,0);
		addRDApvar(PRTDEF,"USE FONT ARIAL BOLD ITALIC 18","\\027\\040\\057\\085\\027\\040\\115\\049\\112\\049\\056\\118\\049\\115\\051\\098\\049\\054\\054\\048\\050\\084",0,0,0);
	
#ifndef WIN32
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_40","./fonts/pcl/rda_40_mrvcode39extxs_10.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_43","./fonts/pcl/rda_43_mrvcode39extxs_16.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_44","./fonts/pcl/rda_44_mrvcode39extxs_18.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_45","./fonts/pcl/rda_45_mrvcode39extxsa_10.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_46","./fonts/pcl/rda_46_mrvcode39extxsa_12.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_47","./fonts/pcl/rda_47_mrvcode39extxsa_14.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_48","./fonts/pcl/rda_48_mrvcode39extxsa_16.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_49","./fonts/pcl/rda_49_mrvcode39extxsa_18.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_60","./fonts/pcl/rda_60_idautomation_micr6.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_61","./fonts/pcl/rda_61_idautomation_micr6b.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_62","./fonts/pcl/rda_62_idautomation_micr6l.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_63","./fonts/pcl/rda_63_idautomation_micr6n1.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_64","./fonts/pcl/rda_64_idautomation_micr6n2.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_65","./fonts/pcl/rda_65_idautomation_micr6w1.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_66","./fonts/pcl/rda_66_idautomation_micr6w2.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_67","./fonts/pcl/rda_67_idautomation_micr3.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_68","./fonts/pcl/rda_68_idautomation_micr3b.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_69","./fonts/pcl/rda_69_idautomation_micr3l.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_70","./fonts/pcl/rda_70_idautomation_micr3n.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_71","./fonts/pcl/rda_71_idautomation_micr3w.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_72","./fonts/pcl/rda_72_idautomation_sblb3-12.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_73","./fonts/pcl/rda_73_idautomation_sblb3-14.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_74","./fonts/pcl/rda_74_idautomation_sblb3-18.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_75","./fonts/pcl/rda_75_idautomation_sblb3-24.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_76","./fonts/pcl/rda_76_idautomation_secb3-12.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_77","./fonts/pcl/rda_77_idautomation_secb3-14.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_78","./fonts/pcl/rda_78_idautomation_secb3-18.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_79","./fonts/pcl/rda_79_idautomation_sehb3-18.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_80","./fonts/pcl/rda_80_idautomation_secb3-24.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_81","./fonts/pcl/rda_81_idautomation_sehb3-24.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_82","./fonts/pcl/rda_82_idautomation_secw3-12.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_83","./fonts/pcl/rda_83_idautomation_secw3-14.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_84","./fonts/pcl/rda_84_idautomation_secw3-18.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_85","./fonts/pcl/rda_85_idautomation_sehw3-18.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_86","./fonts/pcl/rda_86_idautomation_secw3-24.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_87","./fonts/pcl/rda_87_idautomation_sehw3-24.sfp",0,0,0,2);
		addRDApvar(PRTDEF,"UNLOAD_SOFTFONTS","\\027\\042\\099\\049\\048\\100\\050\\102\\027\\042\\099\\049\\049\\100\\050\\102\\027\\042\\099\\049\\050\\100\\050\\102\\027\\042\\099\\049\\051\\100\\050\\102\\027\\042\\099\\049\\052\\100\\050\\102\\027\\042\\099\\049\\053\\100\\050\\102\\027\\042\\099\\049\\054\\100\\050\\102\\027\\042\\099\\049\\055\\100\\050\\102\\027\\042\\099\\049\\056\\100\\050\\102\\027\\042\\099\\049\\057\\100\\050\\102\\027\\042\\099\\050\\048\\100\\050\\102\\027\\042\\099\\050\\049\\100\\050\\102\\027\\042\\099\\050\\050\\100\\050\\102\\027\\042\\099\\050\\051\\100\\050\\102\\027\\042\\099\\050\\052\\100\\050\\102\\027\\042\\099\\050\\053\\100\\050\\102\\027\\042\\099\\050\\054\\100\\050\\102\\027\\042\\099\\050\\055\\100\\050\\102\\027\\042\\099\\050\\056\\100\\050\\102\\027\\042\\099\\050\\057\\100\\050\\102\\027\\042\\099\\051\\048\\100\\050\\102\\027\\042\\099\\051\\049\\100\\050\\102\\027\\042\\099\\051\\050\\100\\050\\102\\027\\042\\099\\051\\051\\100\\050\\102\\027\\042\\099\\051\\052\\100\\050\\102\\027\\042\\099\\051\\053\\100\\050\\102\\027\\042\\099\\051\\054\\100\\050\\102\\027\\042\\099\\051\\055\\100\\050\\102\\027\\042\\099\\051\\056\\100\\050\\102\\027\\042\\099\\051\\057\\100\\050\\102\\027\\042\\099\\052\\048\\100\\050\\102\\027\\042\\099\\052\\049\\100\\050\\102\\027\\042\\099\\052\\050\\100\\050\\102\\027\\042\\099\\052\\051\\100\\050\\102\\027\\042\\099\\052\\052\\100\\050\\102\\027\\042\\099\\052\\053\\100\\050\\102\\027\\042\\099\\052\\054\\100\\050\\102\\027\\042\\099\\052\\055\\100\\050\\102\\027\\042\\099\\052\\056\\100\\050\\102\\027\\042\\099\\052\\057\\100\\050\\102\\027\\042\\099\\053\\048\\100\\050\\102\\027\\042\\099\\053\\049\\100\\050\\102\\027\\042\\099\\053\\050\\100\\050\\102\\027\\042\\099\\053\\051\\100\\050\\102\\027\\042\\099\\053\\052\\100\\050\\102\\027\\042\\099\\053\\053\\100\\050\\102\\027\\042\\099\\053\\054\\100\\050\\102\\027\\042\\099\\053\\055\\100\\050\\102\\027\\042\\099\\053\\056\\100\\050\\102\\027\\042\\099\\053\\057\\100\\050\\102\\027\\042\\099\\054\\048\\100\\050\\102\\027\\042\\099\\054\\049\\100\\050\\102\\027\\042\\099\\054\\050\\100\\050\\102\\027\\042\\099\\054\\051\\100\\050\\102\\027\\042\\099\\054\\052\\100\\050\\102\\027\\042\\099\\054\\053\\100\\050\\102\\027\\042\\099\\054\\054\\100\\050\\102\\027\\042\\099\\054\\055\\100\\050\\102\\027\\042\\099\\054\\056\\100\\050\\102\\027\\042\\099\\054\\057\\100\\050\\102\\027\\042\\099\\055\\048\\100\\050\\102\\027\\042\\099\\055\\049\\100\\050\\102\\027\\042\\099\\055\\050\\100\\050\\102\\027\\042\\099\\055\\051\\100\\050\\102\\027\\042\\099\\055\\052\\100\\050\\102\\027\\042\\099\\055\\053\\100\\050\\102\\027\\042\\099\\055\\054\\100\\050\\102\\027\\042\\099\\055\\055\\100\\050\\102\\027\\042\\099\\055\\056\\100\\050\\102\\027\\042\\099\\055\\057\\100\\050\\102\\027\\042\\099\\056\\048\\100\\050\\102\\027\\042\\099\\056\\049\\100\\050\\102\\027\\042\\099\\056\\050\\100\\050\\102\\027\\042\\099\\056\\051\\100\\050\\102\\027\\042\\099\\056\\052\\100\\050\\102\\027\\042\\099\\056\\053\\100\\050\\102\\027\\042\\099\\056\\054\\100\\050\\102\\027\\042\\099\\056\\055\\100\\050\\102\\027\\042\\099\\056\\056\\100\\050\\102\\027\\042\\099\\056\\057\\100\\050\\102\\027\\042\\099\\057\\048\\100\\050\\102\\027\\042\\099\\057\\049\\100\\050\\102\\027\\042\\099\\057\\050\\100\\050\\102\\027\\042\\099\\057\\051\\100\\050\\102\\027\\042\\099\\057\\052\\100\\050\\102\\027\\042\\099\\057\\053\\100\\050\\102\\027\\042\\099\\057\\054\\100\\050\\102\\027\\042\\099\\057\\055\\100\\050\\102\\027\\042\\099\\057\\056\\100\\050\\102\\027\\042\\099\\057\\057\\100\\050\\102\\027\\042\\099\\049\\048\\048\\100\\050\\102\\027\\042\\099\\049\\048\\049\\100\\050\\102\\027\\042\\099\\049\\048\\050\\100\\050\\102\\027\\042\\099\\049\\048\\051\\100\\050\\102\\027\\042\\099\\049\\048\\052\\100\\050\\102\\027\\042\\099\\049\\048\\053\\100\\050\\102\\027\\042\\099\\049\\048\\054\\100\\050\\102\\027\\042\\099\\049\\048\\055\\100\\050\\102\\027\\042\\099\\049\\048\\056\\100\\050\\102\\027\\042\\099\\049\\048\\057\\100\\050\\102\\027\\042\\099\\049\\049\\048\\100\\050\\102\\027\\042\\099\\049\\049\\049\\100\\050\\102\\027\\042\\099\\049\\049\\050\\100\\050\\102\\027\\042\\099\\049\\049\\051\\100\\050\\102\\027\\042\\099\\049\\049\\052\\100\\050\\102\\027\\042\\099\\049\\049\\053\\100\\050\\102\\027\\042\\099\\049\\049\\054\\100\\050\\102\\027\\042\\099\\049\\049\\055\\100\\050\\102\\027\\042\\099\\049\\049\\056\\100\\050\\102\\027\\042\\099\\049\\049\\057\\100\\050\\102\\027\\042\\099\\049\\050\\048\\100\\050\\102\\027\\042\\099\\049\\050\\049\\100\\050\\102\\027\\042\\099\\049\\050\\050\\100\\050\\102\\027\\042\\099\\049\\050\\051\\100\\050\\102\\027\\042\\099\\049\\050\\052\\100\\050\\102\\027\\042\\099\\049\\050\\053\\100\\050\\102\\027\\042\\099\\049\\050\\054\\100\\050\\102\\027\\042\\099\\049\\050\\055\\100\\050\\102\\027\\042\\099\\049\\050\\056\\100\\050\\102\\027\\042\\099\\049\\050\\057\\100\\050\\102\\027\\042\\099\\049\\051\\048\\100\\050\\102\\027\\042\\099\\049\\051\\049\\100\\050\\102\\027\\042\\099\\049\\051\\050\\100\\050\\102\\027\\042\\099\\049\\051\\051\\100\\050\\102\\027\\042\\099\\049\\051\\052\\100\\050\\102\\027\\042\\099\\049\\051\\053\\100\\050\\102\\027\\042\\099\\049\\051\\054\\100\\050\\102\\027\\042\\099\\049\\051\\055\\100\\050\\102\\027\\042\\099\\049\\051\\056\\100\\050\\102\\027\\042\\099\\049\\051\\057\\100\\050\\102\\027\\042\\099\\049\\052\\048\\100\\050\\102\\027\\042\\099\\049\\052\\049\\100\\050\\102\\027\\042\\099\\049\\052\\050\\100\\050\\102\\027\\042\\099\\049\\052\\051\\100\\050\\102\\027\\042\\099\\049\\052\\052\\100\\050\\102\\027\\042\\099\\049\\052\\053\\100\\050\\102\\027\\042\\099\\049\\052\\054\\100\\050\\102\\027\\042\\099\\049\\052\\055\\100\\050\\102\\027\\042\\099\\049\\052\\056\\100\\050\\102\\027\\042\\099\\049\\052\\057\\100\\050\\102\\027\\038\\102\\054\\088",0,0,0);
#endif
#ifdef WIN32
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_40",".\\fonts\\pcl\\rda_40_mrvcode39extxs_10.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_43",".\\fonts\\pcl\\rda_43_mrvcode39extxs_16.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_44",".\\fonts\\pcl\\rda_44_mrvcode39extxs_18.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_45",".\\fonts\\pcl\\rda_45_mrvcode39extxs_10.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_46",".\\fonts\\pcl\\rda_46_mrvcode39extxs_12.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_47",".\\fonts\\pcl\\rda_47_mrvcode39extxs_14.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_48",".\\fonts\\pcl\\rda_48_mrvcode39extxs_16.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_49",".\\fonts\\pcl\\rda_49_mrvcode39extxs_18.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_60",".\\fonts\\pcl\\rda_60_idautomation_micr6.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_61",".\\fonts\\pcl\\rda_61_idautomation_micr6b.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_62",".\\fonts\\pcl\\rda_62_idautomation_micr6l.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_63",".\\fonts\\pcl\\rda_63_idautomation_micr6n1.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_64",".\\fonts\\pcl\\rda_64_idautomation_micr6n2.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_65",".\\fonts\\pcl\\rda_65_idautomation_micr6w1.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_66",".\\fonts\\pcl\\rda_66_idautomation_micr6w2.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_67",".\\fonts\\pcl\\rda_67_idautomation_micr3.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_68",".\\fonts\\pcl\\rda_68_idautomation_micr3b.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_69",".\\fonts\\pcl\\rda_69_idautomation_micr3l.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_70",".\\fonts\\pcl\\rda_70_idautomation_micr3n.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_71",".\\fonts\\pcl\\rda_71_idautomation_micr3w.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_72",".\\fonts\\pcl\\rda_72_idautomation_sblb3-12.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_73",".\\fonts\\pcl\\rda_73_idautomation_sblb3-14.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_74",".\\fonts\\pcl\\rda_74_idautomation_sblb3-18.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_75",".\\fonts\\pcl\\rda_75_idautomation_sblb3-24.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_76",".\\fonts\\pcl\\rda_76_idautomation_secb3-12.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_77",".\\fonts\\pcl\\rda_77_idautomation_secb3-14.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_78",".\\fonts\\pcl\\rda_78_idautomation_secb3-18.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_79",".\\fonts\\pcl\\rda_79_idautomation_sehb3-18.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_80",".\\fonts\\pcl\\rda_80_idautomation_secb3-24.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_81",".\\fonts\\pcl\\rda_81_idautomation_sehb3-24.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_82",".\\fonts\\pcl\\rda_82_idautomation_secw3-12.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_83",".\\fonts\\pcl\\rda_83_idautomation_secw3-14.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_84",".\\fonts\\pcl\\rda_84_idautomation_secw3-18.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_85",".\\fonts\\pcl\\rda_85_idautomation_sehw3-18.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_86",".\\fonts\\pcl\\rda_86_idautomation_secw3-24.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_87",".\\fonts\\pcl\\rda_87_idautomation_sehw3-24.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"UNLOAD_SOFTFONTS",".\\fonts\\pcl\\unloadfonts.pcl",0,0,0,2);
#endif
		addRDApvar(PRTDEF,"USE_SOFTFONT_40","\\027\\040\\048\\089\\027\\040\\115\\049\\112\\049\\048\\046\\048\\118\\048\\115\\048\\098\\051\\051\\050\\056\\048\\084",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_43","\\027\\040\\048\\089\\027\\040\\115\\049\\112\\049\\054\\046\\048\\118\\048\\115\\048\\098\\051\\051\\050\\056\\048\\084",0,0,0);

		addRDApvar(PRTDEF,"USE_SOFTFONT_44","\\027\\040\\048\\089\\027\\040\\115\\049\\112\\049\\056\\046\\048\\118\\048\\115\\048\\098\\051\\051\\050\\056\\048\\084",0,0,0);

		addRDApvar(PRTDEF,"USE_SOFTFONT_45","\\027\\040\\048\\089\\027\\040\\115\\049\\112\\049\\048\\046\\048\\118\\048\\115\\048\\098\\051\\051\\050\\056\\053\\084",0,0,0);

		addRDApvar(PRTDEF,"USE_SOFTFONT_46","\\027\\040\\048\\089\\027\\040\\115\\049\\112\\049\\050\\046\\048\\118\\048\\115\\048\\098\\051\\051\\050\\056\\053\\084",0,0,0);

		addRDApvar(PRTDEF,"USE_SOFTFONT_47","\\027\\040\\048\\089\\027\\040\\115\\049\\112\\049\\052\\046\\048\\118\\048\\115\\048\\098\\051\\051\\050\\056\\053\\084",0,0,0);

		addRDApvar(PRTDEF,"USE_SOFTFONT_48","\\027\\040\\048\\089\\027\\040\\115\\049\\112\\049\\054\\046\\048\\118\\048\\115\\048\\098\\051\\051\\050\\056\\053\\084",0,0,0);

		addRDApvar(PRTDEF,"USE_SOFTFONT_49","\\027\\040\\048\\089\\027\\040\\115\\049\\112\\049\\056\\046\\048\\118\\048\\115\\048\\098\\051\\051\\050\\056\\053\\084",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_60","\\027\\040\\054\\048\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_61","\\027\\040\\054\\049\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_62","\\027\\040\\054\\050\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_63","\\027\\040\\054\\051\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_64","\\027\\040\\054\\052\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_65","\\027\\040\\054\\053\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_66","\\027\\040\\054\\054\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_67","\\027\\040\\054\\055\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_68","\\027\\040\\054\\056\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_69","\\027\\040\\054\\057\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_70","\\027\\040\\055\\048\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_71","\\027\\040\\055\\049\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_72","\\027\\040\\055\\050\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_73","\\027\\040\\055\\051\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_74","\\027\\040\\055\\052\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_75","\\027\\040\\055\\053\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_76","\\027\\040\\055\\054\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_77","\\027\\040\\055\\055\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_78","\\027\\040\\055\\056\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_79","\\027\\040\\055\\057\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_80","\\027\\040\\056\\048\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_81","\\027\\040\\056\\049\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_82","\\027\\040\\056\\050\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_83","\\027\\040\\056\\051\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_84","\\027\\040\\056\\052\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_85","\\027\\040\\056\\053\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_86","\\027\\040\\056\\054\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_87","\\027\\040\\056\\055\\088",0,0,0);
		addRDApvar(PRTDEF,"RESET FONT","\\027\\040\\051\\064",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_40","\\027\\042\\099\\052\\048\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_43","\\027\\042\\099\\052\\051\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_44","\\027\\042\\099\\052\\052\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_45","\\027\\042\\099\\052\\053\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_46","\\027\\042\\099\\052\\054\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_47","\\027\\042\\099\\052\\055\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_48","\\027\\042\\099\\052\\056\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_49","\\027\\042\\099\\052\\057\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_60","\\027\\042\\099\\054\\048\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_61","\\027\\042\\099\\054\\049\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_62","\\027\\042\\099\\054\\050\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_63","\\027\\042\\099\\054\\051\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_64","\\027\\042\\099\\054\\052\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_65","\\027\\042\\099\\054\\053\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_66","\\027\\042\\099\\054\\054\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_67","\\027\\042\\099\\054\\055\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_68","\\027\\042\\099\\054\\056\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_69","\\027\\042\\099\\054\\057\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_70","\\027\\042\\099\\055\\048\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_71","\\027\\042\\099\\055\\049\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_72","\\027\\042\\099\\055\\050\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_73","\\027\\042\\099\\055\\051\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_74","\\027\\042\\099\\055\\052\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_75","\\027\\042\\099\\055\\053\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_76","\\027\\042\\099\\055\\054\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_77","\\027\\042\\099\\055\\055\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_78","\\027\\042\\099\\055\\056\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_79","\\027\\042\\099\\055\\057\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_80","\\027\\042\\099\\056\\048\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_81","\\027\\042\\099\\056\\049\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_82","\\027\\042\\099\\056\\050\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_83","\\027\\042\\099\\056\\051\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_84","\\027\\042\\099\\056\\052\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_85","\\027\\042\\099\\056\\053\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_86","\\027\\042\\099\\056\\054\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_87","\\027\\042\\099\\056\\055\\100\\050\\070",0,0,0);

		addRDApvar(PRTDEF,"DELETE_SOFTFONT_200","\\027\\042\\099\\050\\048\\048\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_201","\\027\\042\\099\\050\\048\\049\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"USE_PCLMACRO_1000","\\027\\038\\102\\049\\048\\048\\048\\121\\051\\120",0,0,0); /* MACRO 1000 = VATANGIBLE LAST/ONLY DATA PAGE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1001","\\027\\038\\102\\049\\048\\048\\049\\121\\051\\120",0,0,0); /* MACRO 1001 = VATANGIBLE SIGNATURE PAGE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1002","\\027\\038\\102\\049\\048\\048\\050\\121\\051\\120",0,0,0); /* MACRO 1002 = VATANGIBLE ALL BUT LAST DATA PAGE OF MULTIPLE PAGES*/
		addRDApvar(PRTDEF,"USE_PCLMACRO_1003","\\027\\038\\102\\049\\048\\048\\051\\121\\051\\120",0,0,0); /* MACRO 1003 = FIFA PRP/RLST CITATION CANTON */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1004","\\027\\038\\102\\049\\048\\048\\052\\121\\051\\120",0,0,0); /* MACRO 1004 = VATANGIBLE PRIOR YEAR LAST/ONLY DATA PAGE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1005","\\027\\038\\102\\049\\048\\048\\053\\121\\051\\120",0,0,0); /* MACRO 1005 = VATANGIBLE PRIOR YEAR SIGNATURE PAGE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1006","\\027\\038\\102\\049\\048\\048\\054\\121\\051\\120",0,0,0); /* MACRO 1006 = VATANGIBLE PRIOR YEAR ALL BUT LAST DATA PAGE OF MULTIPLE PAGES*/
		addRDApvar(PRTDEF,"USE_PCLMACRO_1010","\\027\\038\\102\\049\\048\\049\\048\\121\\051\\120",0,0,0); /* MACRO 1010 = DELINQUENT MADISON PP*/
		addRDApvar(PRTDEF,"USE_PCLMACRO_1011","\\027\\038\\102\\049\\048\\049\\049\\121\\051\\120",0,0,0); /* MACRO 1011 = DELINQUENT MADISON RE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1012","\\027\\038\\102\\049\\048\\049\\050\\121\\051\\120",0,0,0); /* MACRO 1012 = DELINQUENT MADISON MISC */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1013","\\027\\038\\102\\049\\048\\049\\051\\121\\051\\120",0,0,0); /* MACRO 1013 = PROPERTY STATEMENT */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1014","\\027\\038\\102\\049\\048\\049\\052\\121\\051\\120",0,0,0); /* MACRO 1014 = REAL ESTATE STATEMENT */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1015","\\027\\038\\102\\049\\048\\049\\053\\121\\051\\120",0,0,0); /* MACRO 1015 = MISC BILLING STATEMENT */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1016","\\027\\038\\102\\049\\048\\049\\054\\121\\051\\120",0,0,0); /* MACRO 1016 = MADISON DMV STOP LETTER FORM */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1017","\\027\\038\\102\\049\\048\\049\\055\\121\\051\\120",0,0,0); /* MACRO 1017 = SUPPLEMENTAL MADISON PP*/
		addRDApvar(PRTDEF,"USE_PCLMACRO_1018","\\027\\038\\102\\049\\048\\049\\056\\121\\051\\120",0,0,0); /* MACRO 1018 = SUPPLEMENTAL MADISON RE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1019","\\027\\038\\102\\049\\048\\049\\057\\121\\051\\120",0,0,0); /* MACRO 1019 = SUPPLEMENTAL MADISON MISC */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1020","\\027\\038\\102\\049\\048\\050\\048\\121\\051\\120",0,0,0); /* MACRO 1020 = MISC BILLING INCOME TAX FORM 759 */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1021","\\027\\038\\102\\049\\048\\050\\049\\121\\051\\120",0,0,0); /* MACRO 1021 = MISC BILLING INTERMDIATE INVOICE TICKET */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1022","\\027\\038\\102\\049\\048\\050\\050\\121\\051\\120",0,0,0); /* MACRO 1022 = MISC BILLING FINAL INVOICE TICKET */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1023","\\027\\038\\102\\049\\048\\050\\051\\121\\051\\120",0,0,0); /* MACRO 1023 = REAL ESTATE NOTICE OF ASSESSMENT */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1024","\\027\\038\\102\\049\\048\\050\\052\\121\\051\\120",0,0,0); /* MACRO 1024 = MISC BILLING ALCOHOL LICENSE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1025","\\027\\038\\102\\049\\048\\050\\053\\121\\051\\120",0,0,0); /* MACRO 1024 = MISC BILLING INVOICE STATMENT */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2000","\\027\\038\\102\\050\\048\\048\\048\\121\\051\\120",0,0,0); /* MACRO 2000 = PAYROLL FEDERAL 941 PAGE 1 */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2001","\\027\\038\\102\\050\\048\\048\\049\\121\\051\\120",0,0,0); /* MACRO 2001 = PAYROLL FEDERAL 941 PAGE 2 */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2002","\\027\\038\\102\\050\\048\\048\\050\\121\\051\\120",0,0,0); /* MACRO 2002 = PAYROLL FEDERAL 941 PAGE 3 */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2003","\\027\\038\\102\\050\\048\\048\\051\\121\\051\\120",0,0,0); /* MACRO 2003 = PAYROLL FEDERAL 941 PAGE 4 */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2005","\\027\\038\\102\\050\\048\\048\\053\\121\\051\\120",0,0,0); /* MACRO 2005 = PAYROLL BENEFITRECAP */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2100","\\027\\038\\102\\050\\049\\048\\048\\121\\051\\120",0,0,0); /* MACRO 2100 = 2010 W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2101","\\027\\038\\102\\050\\049\\048\\049\\121\\051\\120",0,0,0); /* MACRO 2101 = 2010 W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2102","\\027\\038\\102\\050\\049\\048\\050\\121\\051\\120",0,0,0); /* MACRO 2102 = 2010 W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2103","\\027\\038\\102\\050\\049\\048\\051\\121\\051\\120",0,0,0); /* MACRO 2103 = 2010 W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2104","\\027\\038\\102\\050\\049\\048\\052\\121\\051\\120",0,0,0); /* MACRO 2104 = 2010 W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2105","\\027\\038\\102\\050\\049\\048\\053\\121\\051\\120",0,0,0); /* MACRO 2105 = 2010 W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2106","\\027\\038\\102\\050\\049\\048\\054\\121\\051\\120",0,0,0); /* MACRO 2106 = 2011 4 PART W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2107","\\027\\038\\102\\050\\049\\048\\055\\121\\051\\120",0,0,0); /* MACRO 2107 = 2011 4 PART W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2108","\\027\\038\\102\\050\\049\\048\\056\\121\\051\\120",0,0,0); /* MACRO 2108 = 2012 4 PART W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2109","\\027\\038\\102\\050\\049\\048\\057\\121\\051\\120",0,0,0); /* MACRO 2109 = 2012 4 PART W2 FORMS */

		addRDApvar(PRTDEF,"USE_PCLMACRO_3000","\\027\\038\\102\\051\\048\\048\\048\\121\\051\\120",0,0,0); /* MACRO 3000 = VENCHK MICR LONG STUB BOTTOM */
		addRDApvar(PRTDEF,"USE_PCLMACRO_3001","\\027\\038\\102\\051\\048\\048\\049\\121\\051\\120",0,0,0); /* MACRO 3001 = VENCHK MICR SHORT STUB MIDDLE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_3002","\\027\\038\\102\\051\\048\\048\\050\\121\\051\\120",0,0,0); /* MACRO 3002 = VENCHK MICR SHORT STUB SPRINGFIELD MIDDLE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_3003","\\027\\038\\102\\051\\048\\048\\051\\121\\051\\120",0,0,0); /* MACRO 3003 = VENCHK MICR DOUBLE STUB BOTTOM */

		addRDApvar(PRTDEF,"USE_PCLMACRO_3010","\\027\\038\\102\\051\\048\\049\\048\\121\\051\\120",0,0,0); /* MACRO 3010 = VW CHK MICR LONG STUB BOTTOM */
		addRDApvar(PRTDEF,"USE_PCLMACRO_3011","\\027\\038\\102\\051\\048\\049\\049\\121\\051\\120",0,0,0); /* MACRO 3011 = VW CHK MICR SHORT STUB MIDDLE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_3012","\\027\\038\\102\\051\\048\\049\\050\\121\\051\\120",0,0,0); /* MACRO 3012 = VW CHK MICR SHORT STUB SPRINGFIELD MIDDLE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_3013","\\027\\038\\102\\051\\048\\049\\051\\121\\051\\120",0,0,0); /* MACRO 3013 = VW CHK MICR DOUBLE STUB BOTTOM */

		addRDApvar(PRTDEF,"USE_PCLMACRO_3020","\\027\\038\\102\\051\\048\\050\\048\\121\\051\\120",0,0,0); /* MACRO 3020 = PAYCHK MICR LONG STUB BOTTOM */
		w=49;
		y=48;
		z=48;
		for(x=0;x<400;x++)
		{
			memset(stemp,0,101);
			memset(stemp2,0,101);
			sprintf(stemp,"USE_PCLMACRO_%d",(3100+x));
			sprintf(stemp2,"\\027\\038\\102\\051\\%03d\\%03d\\%03d\\121\\051\\120",w,y,z);
			addRDApvar(PRTDEF,stemp,stemp2,0,0,0); /* MACRO 3100-3499 = FULL CHECK FORMS */
			z++;
			if(z==58)
			{
				y++;
				z=48;
			}
			if((x==99)||(x==199)||(x==299)||(x==399))
			{
				w++;
				y=48;
			}
		}

		addRDApvar(PRTDEF,"USE_PCLMACRO_5000","\\027\\038\\102\\053\\048\\048\\048\\121\\051\\120",0,0,0); /* MACRO 5000 = ACTUAL PURCHASE ORDER */
		addRDApvar(PRTDEF,"USE_PCLMACRO_6000","\\027\\038\\102\\054\\048\\048\\048\\121\\051\\120",0,0,0); /* MACRO 6000 = ACTUAL REQUISITION */
		addRDApvar(PRTDEF,"USE_PCLMACRO_7000","\\027\\038\\102\\055\\048\\048\\048\\121\\051\\120",0,0,0); /* MACRO 7000 = OCCTAX CERTIFICATE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_8000","\\027\\038\\102\\056\\048\\048\\048\\121\\051\\120",0,0,0); /* MACRO 8000 = BUILDPERMIT CONSTRUCTION PERMIT */
		addRDApvar(PRTDEF,"USE_PCLMACRO_9000","\\027\\038\\102\\057\\048\\048\\048\\121\\051\\120",0,0,0); /* MACRO 9000 = UTLBLL WORK ORDER */
		addRDApvar(PRTDEF,"USE_PCLMACRO_7005","\\027\\038\\102\\058\\048\\048\\048\\121\\051\\120",0,0,0); /* MACRO 7005 = OCCTAX STATEMENT */
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1000","\\027\\038\\102\\049\\048\\048\\048\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1001","\\027\\038\\102\\049\\048\\048\\049\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1002","\\027\\038\\102\\049\\048\\048\\050\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1003","\\027\\038\\102\\049\\048\\048\\051\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1004","\\027\\038\\102\\049\\048\\048\\052\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1005","\\027\\038\\102\\049\\048\\048\\053\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1006","\\027\\038\\102\\049\\048\\048\\054\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1010","\\027\\038\\102\\049\\048\\049\\048\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1011","\\027\\038\\102\\049\\048\\049\\049\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1012","\\027\\038\\102\\049\\048\\049\\050\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1013","\\027\\038\\102\\049\\048\\049\\051\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1014","\\027\\038\\102\\049\\048\\049\\052\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1015","\\027\\038\\102\\049\\048\\049\\053\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1016","\\027\\038\\102\\049\\048\\049\\054\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1017","\\027\\038\\102\\049\\048\\049\\055\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1018","\\027\\038\\102\\049\\048\\049\\056\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1019","\\027\\038\\102\\049\\048\\049\\057\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1020","\\027\\038\\102\\049\\048\\050\\048\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1021","\\027\\038\\102\\049\\048\\050\\049\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1022","\\027\\038\\102\\049\\048\\050\\050\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1023","\\027\\038\\102\\049\\048\\050\\051\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1024","\\027\\038\\102\\049\\048\\050\\052\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1025","\\027\\038\\102\\049\\048\\050\\053\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2000","\\027\\038\\102\\050\\048\\048\\048\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2001","\\027\\038\\102\\050\\048\\048\\049\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2002","\\027\\038\\102\\050\\048\\048\\050\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2003","\\027\\038\\102\\050\\048\\048\\051\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2005","\\027\\038\\102\\050\\048\\048\\053\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2100","\\027\\038\\102\\050\\049\\048\\048\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2101","\\027\\038\\102\\050\\049\\048\\049\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2102","\\027\\038\\102\\050\\049\\048\\050\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2103","\\027\\038\\102\\050\\049\\048\\051\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2104","\\027\\038\\102\\050\\049\\048\\052\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2105","\\027\\038\\102\\050\\049\\048\\053\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2106","\\027\\038\\102\\050\\049\\048\\054\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2107","\\027\\038\\102\\050\\049\\048\\055\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2108","\\027\\038\\102\\050\\049\\048\\056\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2109","\\027\\038\\102\\050\\049\\048\\057\\121\\056\\120",0,0,0);

		addRDApvar(PRTDEF,"DELETE_PCLMACRO_3000","\\027\\038\\102\\051\\048\\048\\048\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_3001","\\027\\038\\102\\051\\048\\048\\049\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_3002","\\027\\038\\102\\051\\048\\048\\050\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_3003","\\027\\038\\102\\051\\048\\048\\051\\121\\056\\120",0,0,0);

		addRDApvar(PRTDEF,"DELETE_PCLMACRO_3010","\\027\\038\\102\\051\\048\\049\\048\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_3011","\\027\\038\\102\\051\\048\\049\\049\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_3012","\\027\\038\\102\\051\\048\\049\\050\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_3013","\\027\\038\\102\\051\\048\\049\\051\\121\\056\\120",0,0,0);

		addRDApvar(PRTDEF,"DELETE_PCLMACRO_3020","\\027\\038\\102\\051\\048\\050\\048\\121\\056\\120",0,0,0);

		w=49;
		y=48;
		z=48;
		for(x=0;x<400;x++)
		{
			memset(stemp,0,101);
			memset(stemp2,0,101);
			sprintf(stemp,"DELETE_PCLMACRO_%d",(3100+x));
			sprintf(stemp2,"\\027\\038\\102\\051\\%03d\\%03d\\%03d\\121\\056\\120",w,y,z);
			addRDApvar(PRTDEF,stemp,stemp2,0,0,0); /* MACRO 3100-3199 = FULL CHECK FORMS */
			z++;
			if(z==58)
			{
				y++;
				z=48;
			}
			if((x==99)||(x==199)||(x==299)||(x==399))
			{
				w++;
				y=48;
			}
		}
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_5000","\\027\\038\\102\\053\\048\\048\\048\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_6000","\\027\\038\\102\\054\\048\\048\\048\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_7000","\\027\\038\\102\\055\\048\\048\\048\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_8000","\\027\\038\\102\\056\\048\\048\\048\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_9000","\\027\\038\\102\\057\\048\\048\\048\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_7005","\\027\\038\\102\\058\\048\\048\\048\\121\\056\\120",0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD VENPMT SIGNATURE","",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD PAYROLL SIGNATURE","",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD PO SIGNATURE","",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"VENPMT SIGNATURE","vensig.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYROLL SIGNATURE","paysig.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PO SIGNATURE","posig.dat",0,0,0,1);
#ifndef WIN32
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1000","./forms/load_pclmacro_1000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1001","./forms/load_pclmacro_1001.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1002","./forms/load_pclmacro_1002.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1003","./forms/load_pclmacro_1003.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1004","./forms/load_pclmacro_1004.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1005","./forms/load_pclmacro_1005.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1006","./forms/load_pclmacro_1006.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1010","./forms/load_pclmacro_1010.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1011","./forms/load_pclmacro_1011.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1012","./forms/load_pclmacro_1012.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1013","./forms/load_pclmacro_1013.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1014","./forms/load_pclmacro_1014.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1015","./forms/load_pclmacro_1015.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1016","./forms/load_pclmacro_1016.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1017","./forms/load_pclmacro_1017.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1018","./forms/load_pclmacro_1018.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1019","./forms/load_pclmacro_1019.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1020","./forms/load_pclmacro_1020.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1021","./forms/load_pclmacro_1021.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1022","./forms/load_pclmacro_1022.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1023","./forms/load_pclmacro_1023.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1024","./forms/load_pclmacro_1024.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1025","./forms/load_pclmacro_1025.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2000","./forms/load_pclmacro_2000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2001","./forms/load_pclmacro_2001.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2002","./forms/load_pclmacro_2002.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2003","./forms/load_pclmacro_2003.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2005","./forms/load_pclmacro_2005.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2100","./forms/load_pclmacro_2100.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2101","./forms/load_pclmacro_2101.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2102","./forms/load_pclmacro_2102.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2103","./forms/load_pclmacro_2103.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2104","./forms/load_pclmacro_2104.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2105","./forms/load_pclmacro_2105.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2106","./forms/load_pclmacro_2106.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2107","./forms/load_pclmacro_2107.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2108","./forms/load_pclmacro_2108.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2109","./forms/load_pclmacro_2109.pcl",0,0,0,2);

		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3000","./forms/load_pclmacro_3000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3001","./forms/load_pclmacro_3001.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3002","./forms/load_pclmacro_3002.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3003","./forms/load_pclmacro_3003.pcl",0,0,0,2);

		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3010","./forms/load_pclmacro_3010.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3011","./forms/load_pclmacro_3011.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3012","./forms/load_pclmacro_3012.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3013","./forms/load_pclmacro_3013.pcl",0,0,0,2);

		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3020","./forms/load_pclmacro_3020.pcl",0,0,0,2);
		for(x=0;x<400;x++)
		{
			memset(stemp,0,101);
			memset(stemp2,0,101);
			sprintf(stemp,"LOAD_PCLMACRO_%d",(3100+x));
			sprintf(stemp2,"./forms/load_pclmacro_%04d.pcl",(3100+x));
			ADVaddRDApvar(PRTDEF,stemp,stemp2,0,0,0,2); /* MACRO 3100-3499 = FULL CHECK FORMS */
		}
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_5000","./forms/load_pclmacro_5000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_6000","./forms/load_pclmacro_6000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_7000","./forms/load_pclmacro_7000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_7005","./forms/load_pclmacro_7005.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_8000","./forms/load_pclmacro_8000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_9000","./forms/load_pclmacro_9000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"DIRECT DEPOSIT","",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"PAYROLL CHECK","",0,0,0,2);
#endif
#ifdef WIN32
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1000",".\\forms\\load_pclmacro_1000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1001",".\\forms\\load_pclmacro_1001.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1002",".\\forms\\load_pclmacro_1002.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1003",".\\forms\\load_pclmacro_1003.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1004",".\\forms\\load_pclmacro_1004.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1005",".\\forms\\load_pclmacro_1005.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1006",".\\forms\\load_pclmacro_1006.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1010",".\\forms\\load_pclmacro_1010.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1011",".\\forms\\load_pclmacro_1011.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1012",".\\forms\\load_pclmacro_1012.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1013",".\\forms\\load_pclmacro_1013.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1014",".\\forms\\load_pclmacro_1014.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1015",".\\forms\\load_pclmacro_1015.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1016",".\\forms\\load_pclmacro_1016.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1017",".\\forms\\load_pclmacro_1017.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1018",".\\forms\\load_pclmacro_1018.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1019",".\\forms\\load_pclmacro_1019.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1020",".\\forms\\load_pclmacro_1020.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1021",".\\forms\\load_pclmacro_1021.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1022",".\\forms\\load_pclmacro_1022.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1023",".\\forms\\load_pclmacro_1023.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1024",".\\forms\\load_pclmacro_1024.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1025",".\\forms\\load_pclmacro_1025.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2000",".\\forms\\load_pclmacro_2000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2001",".\\forms\\load_pclmacro_2001.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2002",".\\forms\\load_pclmacro_2002.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2003",".\\forms\\load_pclmacro_2003.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2005",".\\forms\\load_pclmacro_2005.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2100",".\\forms\\load_pclmacro_2100.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2101",".\\forms\\load_pclmacro_2101.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2102",".\\forms\\load_pclmacro_2102.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2103",".\\forms\\load_pclmacro_2103.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2104",".\\forms\\load_pclmacro_2104.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2105",".\\forms\\load_pclmacro_2105.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2106",".\\forms\\load_pclmacro_2106.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2107",".\\forms\\load_pclmacro_2107.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2108",".\\forms\\load_pclmacro_2108.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2109",".\\forms\\load_pclmacro_2109.pcl",0,0,0,2);

		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3000",".\\forms\\load_pclmacro_3000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3001",".\\forms\\load_pclmacro_3001.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3002",".\\forms\\load_pclmacro_3002.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3003",".\\forms\\load_pclmacro_3003.pcl",0,0,0,2);

		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3010",".\\forms\\load_pclmacro_3010.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3011",".\\forms\\load_pclmacro_3011.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3012",".\\forms\\load_pclmacro_3012.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3013",".\\forms\\load_pclmacro_3013.pcl",0,0,0,2);

		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3020",".\\forms\\load_pclmacro_3020.pcl",0,0,0,2);
		for(x=0;x<400;x++)
		{
			memset(stemp,0,101);
			memset(stemp2,0,101);
			sprintf(stemp,"LOAD_PCLMACRO_%d",(3100+x));
			sprintf(stemp2,".\\forms\\load_pclmacro_%04d.pcl",x);
			ADVaddRDApvar(PRTDEF,stemp,stemp2,0,0,0,2); /* MACRO 3100-3499 = FULL CHECK FORMS */
		}
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_5000",".\\forms\\load_pclmacro_5000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_6000",".\\forms\\load_pclmacro_6000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_7000",".\\forms\\load_pclmacro_7000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_7005",".\\forms\\load_pclmacro_7005.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_8000",".\\forms\\load_pclmacro_8000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_9000",".\\forms\\load_pclmacro_9000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"DIRECT DEPOSIT","",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"PAYROLL CHECK","",0,0,0,2);
#endif
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(17+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [HP LASER 4000], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
	unlink("keylock.dat");
	fp=fopen("keylock.dat","w");
	if(fp!=NULL)
	{
		fprintf(fp,"~AM0~");
		fclose(fp);
	}
	unlink("keyunlock.dat");
	fp=fopen("keyunlock.dat","w");
	if(fp!=NULL)
	{
		fprintf(fp,"~AM1~");
		fclose(fp);
	}
}
void TXT2PScodes(char *devname,int active)
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;
	char stemp2[101];
	int w=0,x=0,y=0,z=0;;

	PRTDEF=RDApdevNEW(devname);
	PRTDEF->active=active;
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		PRTDEF->page_length=10;
		addRDApvar(PRTDEF,"RESET PRINTER","\\027\\051\\049\\048\\027",0,0,0);
		addRDApvar(PRTDEF,"SIGNATURE DIMM 1","\\027\\051\\049\\049\\027",0,0,0);
		addRDApvar(PRTDEF,"SIGNATURE DIMM A","\\027\\051\\049\\050\\027",0,0,0);
		addRDApvar(PRTDEF,"PUSH CURSOR","\\027\\051\\049\\051\\027",0,0,0);
		addRDApvar(PRTDEF,"POP CURSOR","\\027\\051\\049\\052\\027",0,0,0);
		addRDApvar(PRTDEF,"NORMAL TEXT","\\027\\051\\049\\053\\027",1,10,12);
		addRDApvar(PRTDEF,"COMPRESSED TEXT","\\027\\051\\049\\054\\027",1,17,9);
		addRDApvar(PRTDEF,"ELITE PRINT","\\027\\051\\049\\055\\027",256,12,9);
		addRDApvar(PRTDEF,"PAGE_SIZE_LETTER","\\027\\051\\049\\056\\027",0,0,0);
		addRDApvar(PRTDEF,"PAGE_SIZE_LEGAL","\\027\\051\\049\\057\\027",0,0,0);
		addRDApvar(PRTDEF,"PAGE_SIZE_LEDGER","\\027\\051\\050\\048\\027",0,0,0);
		addRDApvar(PRTDEF,"PAGE_SIZE_POSTER","\\027\\051\\050\\049\\027",0,0,0);
		addRDApvar(PRTDEF,"PORTRAIT","\\027\\051\\051\\048\\027",0,0,0);
		addRDApvar(PRTDEF,"LANDSCAPE","\\027\\051\\051\\049\\027",0,0,0);
		addRDApvar(PRTDEF,"SIMPLEX","\\027\\051\\051\\050\\027",0,0,0);
		addRDApvar(PRTDEF,"DUPLEX LONG","\\027\\051\\051\\051\\027",0,0,0);
		addRDApvar(PRTDEF,"DUPLEX SHORT","\\027\\051\\051\\052\\027",0,0,0);
		addRDApvar(PRTDEF,"REVERSE PORTRAIT","\\027\\051\\051\\053\\027",0,0,0);
		addRDApvar(PRTDEF,"REVERSE LANDSCAPE","\\027\\051\\051\\054\\027",0,0,0);
		addRDApvar(PRTDEF,"ASCII SQUARE","\\254",0,0,0);  
		addRDApvar(PRTDEF,"ESCAPE","\\027\\051\\051\\056\\027",0,0,0);
		addRDApvar(PRTDEF,"PCL JOB HEADER","\\027\\027",0,0,0);
		addRDApvar(PRTDEF,"PCL JOB FOOTER","\\027\\027",0,0,0);
		addRDApvar(PRTDEF,"HALF LINE FEED","\\027\\051\\051\\057\\027",0,0,0);
		addRDApvar(PRTDEF,"NEG Q LINE FEED","\\027\\051\\054\\048\\027",0,0,0);
		addRDApvar(PRTDEF,"POS Q LINE FEED","\\027\\051\\054\\049\\027",0,0,0);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE LOCK","\\027\\051\\052\\053\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"SIGNATURE UNLOCK","\\027\\051\\052\\054\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE FILE","\\027\\051\\052\\056\\027",0,0,0,0);
#ifndef WIN32
		ADVaddRDApvar(PRTDEF,"W2FORM-4PP","./forms/W2FORM-4PP.pcl",0,0,0,1);
#endif
#ifdef WIN32
		ADVaddRDApvar(PRTDEF,"W2FORM-4PP",".\\forms\\W2FORM-4PP.pcl",0,0,0,1);
#endif
		addRDApvar(PRTDEF,"USE DEFAULT PRIMARY FONT","\\027\\027",0,0,0);
/*
#ifndef WIN32
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_40","./fonts/pcl/rda_40_mrvcode39extxs_10.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_43","./fonts/pcl/rda_43_mrvcode39extxs_16.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_44","./fonts/pcl/rda_44_mrvcode39extxs_18.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_45","./fonts/pcl/rda_45_mrvcode39extxsa_10.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_46","./fonts/pcl/rda_46_mrvcode39extxsa_12.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_47","./fonts/pcl/rda_47_mrvcode39extxsa_14.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_48","./fonts/pcl/rda_48_mrvcode39extxsa_16.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_49","./fonts/pcl/rda_49_mrvcode39extxsa_18.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"UNLOAD_SOFTFONTS","\\027\\051\\052\\057\\027",0,0,0,0);
#endif
#ifdef WIN32
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_40",".\\fonts\\pcl\\rda_40_mrvcode39extxs_10.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_43",".\\fonts\\pcl\\rda_43_mrvcode39extxs_16.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_44",".\\fonts\\pcl\\rda_44_mrvcode39extxs_18.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_45",".\\fonts\\pcl\\rda_45_mrvcode39extxs_10.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_46",".\\fonts\\pcl\\rda_46_mrvcode39extxs_12.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_47",".\\fonts\\pcl\\rda_47_mrvcode39extxs_14.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_48",".\\fonts\\pcl\\rda_48_mrvcode39extxs_16.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_49",".\\fonts\\pcl\\rda_49_mrvcode39extxs_18.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"UNLOAD_SOFTFONTS",".\\fonts\\pcl\\unloadfonts.pcl",0,0,0,2);
#endif
		addRDApvar(PRTDEF,"USE_SOFTFONT_40","",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_43","",0,0,0);

		addRDApvar(PRTDEF,"USE_SOFTFONT_44","",0,0,0);

		addRDApvar(PRTDEF,"USE_SOFTFONT_45","",0,0,0);

		addRDApvar(PRTDEF,"USE_SOFTFONT_46","",0,0,0);

		addRDApvar(PRTDEF,"USE_SOFTFONT_47","",0,0,0);

		addRDApvar(PRTDEF,"USE_SOFTFONT_48","",0,0,0);

		addRDApvar(PRTDEF,"USE_SOFTFONT_49","",0,0,0);

*/

		for(x=40;x<88;x++)
		{
			memset(stemp,0,101);
			memset(stemp2,0,101);
			sprintf(stemp,"LOAD_SOFTFONT_%d",(x));
			sprintf(stemp2,"\\027\\027");
			addRDApvar(PRTDEF,stemp,stemp2,0,0,0);  
		}


		for(x=40;x<88;x++)
		{
			memset(stemp,0,101);
			memset(stemp2,0,101);
			sprintf(stemp,"USE_SOFTFONT_%d",(x));
			sprintf(stemp2,"\\027\\027");
			addRDApvar(PRTDEF,stemp,stemp2,0,0,0);  
		}


		for(x=40;x<88;x++)
		{
			memset(stemp,0,101);
			memset(stemp2,0,101);
			sprintf(stemp,"DELETE_SOFTFONT_%d",(x));
			sprintf(stemp2,"\\027\\027");
			addRDApvar(PRTDEF,stemp,stemp2,0,0,0);  
		}


		addRDApvar(PRTDEF,"RESET FONT","\\027\\051\\053\\048\\027",0,0,0);
/*
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_40","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_43","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_44","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_45","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_46","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_47","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_48","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_49","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_200","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_201","",0,0,0);
*/
		addRDApvar(PRTDEF,"USE_PCLMACRO_1000","\\027\\050\\053\\049\\049\\048\\048\\048:0:0\\027",0,0,0); /* MACRO 1000 = VATANGIBLE LAST/ONLY DATA PAGE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1001","\\027\\050\\053\\049\\049\\048\\048\\049:0:0\\027",0,0,0); /* MACRO 1001 = VATANGIBLE SIGNATURE PAGE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1002","\\027\\050\\053\\049\\049\\048\\048\\050:0:0\\027",0,0,0); /* MACRO 1002 = VATANGIBLE ALL BUT LAST DATA PAGE OF MULTIPLE PAGES*/
		addRDApvar(PRTDEF,"USE_PCLMACRO_1003","\\027\\050\\053\\049\\049\\048\\048\\051:0:0\\027",0,0,0); /* MACRO 1003 = FIFA PRP/RLST CITATION CANTON */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1004","\\027\\050\\053\\049\\049\\048\\048\\052:0:0\\027",0,0,0); /* MACRO 1004 = VATANGIBLE PRIOR YEAR LAST/ONLY DATA PAGE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1005","\\027\\050\\053\\049\\049\\048\\048\\053:0:0\\027",0,0,0); /* MACRO 1005 = VATANGIBLE PRIOR YEAR SIGNATURE PAGE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1006","\\027\\050\\053\\049\\049\\048\\048\\054:0:0\\027",0,0,0); /* MACRO 1006 = VATANGIBLE PRIOR YEAR ALL BUT LAST DATA PAGE OF MULTIPLE PAGES*/
		addRDApvar(PRTDEF,"USE_PCLMACRO_1010","\\027\\050\\053\\049\\049\\048\\049\\048:0:0\\027",0,0,0); /* MACRO 1010 = DELINQUENT MADISON PP*/
		addRDApvar(PRTDEF,"USE_PCLMACRO_1011","\\027\\050\\053\\049\\049\\048\\049\\049:0:0\\027",0,0,0); /* MACRO 1011 = DELINQUENT MADISON RE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1012","\\027\\050\\053\\049\\049\\048\\049\\050:0:0\\027",0,0,0); /* MACRO 1012 = DELINQUENT MADISON MISC */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1013","\\027\\050\\053\\049\\049\\048\\049\\051:0:0\\027",0,0,0); /* MACRO 1013 = PROPERTY STATEMENT */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1014","\\027\\050\\053\\049\\049\\048\\049\\052:0:0\\027",0,0,0); /* MACRO 1014 = REAL ESTATE STATEMENT */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1015","\\027\\050\\053\\049\\049\\048\\049\\053:0:0\\027",0,0,0); /* MACRO 1015 = MISC BILLING STATEMENT */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1016","\\027\\050\\053\\049\\049\\048\\049\\054:0:0\\027",0,0,0); /* MACRO 1016 = MADISON DMV STOP LETTER FORM */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1017","\\027\\050\\053\\049\\049\\048\\049\\055:0:0\\027",0,0,0); /* MACRO 1017 = SUPPLEMENTAL MADISON PP*/
		addRDApvar(PRTDEF,"USE_PCLMACRO_1018","\\027\\050\\053\\049\\049\\048\\049\\056:0:0\\027",0,0,0); /* MACRO 1018 = SUPPLEMENTAL MADISON RE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1019","\\027\\050\\053\\049\\049\\048\\049\\057:0:0\\027",0,0,0); /* MACRO 1019 = SUPPLEMENTAL MADISON MISC */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1020","\\027\\050\\053\\049\\049\\048\\050\\048:0:0\\027",0,0,0); /* MACRO 1020 = MISC BILLING INCOME TAX FORM 759 */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1021","\\027\\050\\053\\049\\049\\048\\050\\049:0:0\\027",0,0,0); /* MACRO 1021 = MISC BILLING INTERMDIATE INVOICE TICKET */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1022","\\027\\050\\053\\049\\049\\048\\050\\050:0:0\\027",0,0,0); /* MACRO 1022 = MISC BILLING FINAL INVOICE TICKET */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1023","\\027\\050\\053\\049\\049\\048\\050\\051:0:0\\027",0,0,0); /* MACRO 1023 = REAL ESTATE NOTICE OF ASSESSMENT */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1024","\\027\\050\\053\\049\\049\\048\\050\\052:0:0\\027",0,0,0); /* MACRO 1024 = MISC BILLING ALCOHOL LICENSE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1025","\\027\\050\\053\\049\\049\\048\\050\\053:0:0\\027",0,0,0); /* MACRO 1025 = MISC BILLING INVOICE STATEMENT */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2000","\\027\\050\\053\\049\\050\\048\\048\\048:0:0\\027",0,0,0); /* MACRO 2000 = PAYROLL FEDERAL 941 PAGE 1 */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2001","\\027\\050\\053\\049\\050\\048\\048\\049:0:0\\027",0,0,0); /* MACRO 2001 = PAYROLL FEDERAL 941 PAGE 2 */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2002","\\027\\050\\053\\049\\050\\048\\048\\050:0:0\\027",0,0,0); /* MACRO 2002 = PAYROLL FEDERAL 941 PAGE 3 */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2003","\\027\\050\\053\\049\\050\\048\\048\\051:0:0\\027",0,0,0); /* MACRO 2003 = PAYROLL FEDERAL 941 PAGE 4 */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2005","\\027\\050\\053\\049\\050\\048\\048\\053:0:0\\027",0,0,0); /* MACRO 2005 = PAYROLL BENEFITRECAP */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2100","\\027\\050\\053\\049\\050\\049\\048\\048:0:0\\027",0,0,0); /* MACRO 2100 = 2010 W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2101","\\027\\050\\053\\049\\050\\049\\048\\049:0:0\\027",0,0,0); /* MACRO 2101 = 2010 W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2102","\\027\\050\\053\\049\\050\\049\\048\\050:0:0\\027",0,0,0); /* MACRO 2102 = 2010 W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2103","\\027\\050\\053\\049\\050\\049\\048\\051:0:0\\027",0,0,0); /* MACRO 2103 = 2010 W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2104","\\027\\050\\053\\049\\050\\049\\048\\052:0:0\\027",0,0,0); /* MACRO 2104 = 2010 W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2105","\\027\\050\\053\\049\\050\\049\\048\\053:0:0\\027",0,0,0); /* MACRO 2105 = 2010 W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2106","\\027\\050\\053\\049\\050\\049\\048\\054:0:0\\027",0,0,0); /* MACRO 2106 = 2011 4 PART W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2107","\\027\\050\\053\\049\\050\\049\\048\\055:0:0\\027",0,0,0); /* MACRO 2107 = 2011 4 PART W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2108","\\027\\050\\053\\049\\050\\049\\048\\056:0:0\\027",0,0,0); /* MACRO 2108 = 2012 4 PART W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2109","\\027\\050\\053\\049\\050\\049\\048\\057:0:0\\027",0,0,0); /* MACRO 2109 = 2012 4 PART W2 FORMS */

		addRDApvar(PRTDEF,"USE_PCLMACRO_3000","\\027\\050\\053\\049\\051\\048\\048\\048:0:0\\027",0,0,0); /* MACRO 3000 = VENCHK MICR LONG STUB BOTTOM */
		addRDApvar(PRTDEF,"USE_PCLMACRO_3001","\\027\\050\\053\\049\\051\\048\\048\\049:0:0\\027",0,0,0); /* MACRO 3001 = VENCHK MICR SHORT STUB MIDDLE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_3002","\\027\\050\\053\\049\\051\\048\\048\\050:0:0\\027",0,0,0); /* MACRO 3002 = VENCHK MICR SHORT STUB SPRINGFIELD MIDDLE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_3003","\\027\\050\\053\\049\\051\\048\\048\\051:0:0\\027",0,0,0); /* MACRO 3003 = VENCHK MICR DOUBLE STUB BOTTOM */

		addRDApvar(PRTDEF,"USE_PCLMACRO_3010","\\027\\050\\053\\049\\051\\048\\049\\048:0:0\\027",0,0,0); /* MACRO 3010 = VW CHK MICR LONG STUB BOTTOM */
		addRDApvar(PRTDEF,"USE_PCLMACRO_3011","\\027\\050\\053\\049\\051\\048\\049\\049:0:0\\027",0,0,0); /* MACRO 3011 = VW CHK MICR SHORT STUB MIDDLE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_3012","\\027\\050\\053\\049\\051\\048\\049\\050:0:0\\027",0,0,0); /* MACRO 3012 = VW CHK MICR SHORT STUB SPRINGFIELD MIDDLE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_3013","\\027\\050\\053\\049\\051\\048\\049\\051:0:0\\027",0,0,0); /* MACRO 3013 = VW CHK MICR DOUBLE STUB BOTTOM */

		addRDApvar(PRTDEF,"USE_PCLMACRO_3020","\\027\\050\\053\\049\\051\\048\\050\\048:0:0\\027",0,0,0); /* MACRO 3020 = PAYCHK MICR LONG STUB BOTTOM */
		w=49;
		y=48;
		z=48;
		for(x=0;x<400;x++)
		{
			memset(stemp,0,101);
			memset(stemp2,0,101);
			sprintf(stemp,"USE_PCLMACRO_%d",(3100+x));
			sprintf(stemp2,"\\027\\050\\053\\049\\051\\%03d\\%03d\\%03d:0:0\\027",w,y,z);
			addRDApvar(PRTDEF,stemp,stemp2,0,0,0); /* MACRO 3100-3499 = FULL CHECK FORMS */
			z++;
			if(z==58)
			{
				y++;
				z=48;
			}
			if((x==99)||(x==199)||(x==299)||(x==399))
			{
				w++;
				y=48;
			}
		}


		addRDApvar(PRTDEF,"USE_PCLMACRO_5000","\\027\\050\\053\\049\\053\\048\\048\\048:0:0\\027",0,0,0); /* MACRO 5000 = ACTUAL PURCHASE ORDER */
		addRDApvar(PRTDEF,"USE_PCLMACRO_6000","\\027\\050\\053\\049\\054\\048\\048\\048:0:0\\027",0,0,0); /* MACRO 6000 = ACTUAL REQUISITIONS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_7000","\\027\\050\\053\\049\\055\\048\\048\\048:0:0\\027",0,0,0); /* MACRO 7000 = OCCTAX CERTIFICATE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_8000","\\027\\050\\053\\049\\056\\048\\048\\048:0:0\\027",0,0,0); /* MACRO 8000 = BUILDPERMIT CONTRUCTION PERMIT */
		addRDApvar(PRTDEF,"USE_PCLMACRO_9000","\\027\\050\\053\\049\\057\\048\\048\\048:0:0\\027",0,0,0); /* MACRO 9000 = UTLBLL WORK ORDER */
		addRDApvar(PRTDEF,"USE_PCLMACRO_7005","\\027\\050\\053\\049\\058\\048\\048\\048:0:0\\027",0,0,0); /* MACRO 7000 = OCCTAX STATEMENT */
/*
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1000","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1001","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1002","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1003","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1004","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1005","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1006","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1010","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1011","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1012","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1013","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1014","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1015","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1016","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1017","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1018","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1019","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1020","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1021","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1022","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1023","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1024","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1025","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2000","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2001","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2002","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2003","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2005","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2100","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2101","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2102","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2103","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2104","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2105","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_3000","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_3001","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_3002","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_3003","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_3010","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_3011","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_3012","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_3013","",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_3020","",0,0,0);
		w=49;
		y=48;
		z=48;
		for(x=0;x<400;x++)
		{
			memset(stemp,0,101);
			memset(stemp2,0,101);
			sprintf(stemp,"DELETE_PCLMACRO_%d",(3100+x));
			sprintf(stemp2,"\\027\\038\\102\\051\\%03d\\%03d\\%03d\\121\\051\\120",w,y,z);
			addRDApvar(PRTDEF,stemp,stemp2,0,0,0);  MACRO 3100-3499 = FULL CHECK FORMS 
			z++;
			if(z==58)
			{
				y++;
				z=48;
			}
			if((x==99)||(x==199)||(x==299)||(x==399))
			{
				w++;
				y=48;
			}
		}

		addRDApvar(PRTDEF,"DELETE_PCLMACRO_9000","",0,0,0);
*/
		ADVaddRDApvar(PRTDEF,"LOAD VENPMT SIGNATURE","\\027\\051\\053\\049\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD PAYROLL SIGNATURE","\\027\\051\\053\\050\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"VENPMT SIGNATURE","\\027\\051\\053\\051\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"PAYROLL SIGNATURE","\\027\\051\\053\\052\\027",0,0,0,0);
/* #ifndef WIN32 */
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1000","\\027\\050\\053\\048\\049\\048\\048\\048:./forms/form_1000.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1001","\\027\\050\\053\\048\\049\\048\\048\\049:./forms/form_1001.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1002","\\027\\050\\053\\048\\049\\048\\048\\050:./forms/form_1002.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1003","\\027\\050\\053\\048\\049\\048\\048\\051:./forms/form_1003.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1004","\\027\\050\\053\\048\\049\\048\\048\\052:./forms/form_1004.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1005","\\027\\050\\053\\048\\049\\048\\048\\053:./forms/form_1005.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1006","\\027\\050\\053\\048\\049\\048\\048\\054:./forms/form_1006.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1010","\\027\\050\\053\\048\\049\\048\\049\\048:./forms/form_1010.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1011","\\027\\050\\053\\048\\049\\048\\049\\049:./forms/form_1011.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1012","\\027\\050\\053\\048\\049\\048\\049\\050:./forms/form_1012.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1013","\\027\\050\\053\\048\\049\\048\\049\\051:./forms/form_1013.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1014","\\027\\050\\053\\048\\049\\048\\049\\052:./forms/form_1014.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1015","\\027\\050\\053\\048\\049\\048\\049\\053:./forms/form_1015.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1016","\\027\\050\\053\\048\\049\\048\\049\\054:./forms/form_1016.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1017","\\027\\050\\053\\048\\049\\048\\049\\055:./forms/form_1017.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1018","\\027\\050\\053\\048\\049\\048\\049\\056:./forms/form_1018.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1019","\\027\\050\\053\\048\\049\\048\\040\\057:./forms/form_1019.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1020","\\027\\050\\053\\048\\049\\048\\050\\048:./forms/form_1020.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1021","\\027\\050\\053\\048\\049\\048\\050\\049:./forms/form_1021.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1022","\\027\\050\\053\\048\\049\\048\\050\\050:./forms/form_1022.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1023","\\027\\050\\053\\048\\049\\048\\050\\051:./forms/form_1023.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1024","\\027\\050\\053\\048\\049\\048\\050\\052:./forms/form_1024.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1025","\\027\\050\\053\\048\\049\\048\\050\\053:./forms/form_1025.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2000","\\027\\050\\053\\048\\050\\048\\048\\048:./forms/form_2000.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2001","\\027\\050\\053\\048\\050\\048\\048\\049:./forms/form_2001.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2002","\\027\\050\\053\\048\\050\\048\\048\\050:./forms/form_2002.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2003","\\027\\050\\053\\048\\050\\048\\048\\051:./forms/form_2003.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2005","\\027\\050\\053\\048\\050\\048\\048\\053:./forms/form_2005.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2100","\\027\\050\\053\\048\\050\\049\\048\\048:./forms/form_2100.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2101","\\027\\050\\053\\048\\050\\049\\048\\049:./forms/form_2101.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2102","\\027\\050\\053\\048\\050\\049\\048\\050:./forms/form_2102.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2103","\\027\\050\\053\\048\\050\\049\\048\\051:./forms/form_2103.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2104","\\027\\050\\053\\048\\050\\049\\048\\052:./forms/form_2104.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2105","\\027\\050\\053\\048\\050\\049\\048\\053:./forms/form_2105.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2106","\\027\\050\\053\\048\\050\\049\\048\\054:./forms/form_2106.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2107","\\027\\050\\053\\048\\050\\049\\048\\055:./forms/form_2107.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2108","\\027\\050\\053\\048\\050\\049\\048\\056:./forms/form_2108.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2109","\\027\\050\\053\\048\\050\\049\\048\\057:./forms/form_2109.eps\\027",0,0,0,0);

		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3000","\\027\\050\\053\\048\\051\\048\\048\\048:./forms/form_3000.eps\\027",0,0,0,0); /* MACRO 3000 = VENCHK MICR LONG STUB BOTTOM */
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3001","\\027\\050\\053\\048\\051\\048\\048\\049:./forms/form_3001.eps\\027",0,0,0,0); /* MACRO 3001 = VENCHK MICR SHORT STUB MIDDLE */
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3002","\\027\\050\\053\\048\\051\\048\\048\\050:./forms/form_3002.eps\\027",0,0,0,0); /* MACRO 3002 = VENCHK MICR SHORT STUB SPRINGFIELD MIDDLE */
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3003","\\027\\050\\053\\048\\051\\048\\048\\051:./forms/form_3003.eps\\027",0,0,0,0); /* MACRO 3003 = VENCHK MICR DOUBLE STUB BOTTOM */

		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3010","\\027\\050\\053\\048\\051\\048\\049\\048:./forms/form_3010.eps\\027",0,0,0,0); /* MACRO 3010 = VW CHK MICR LONG STUB BOTTOM */
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3011","\\027\\050\\053\\048\\051\\048\\049\\049:./forms/form_3011.eps\\027",0,0,0,0); /* MACRO 3011 = VW CHK MICR SHORT STUB MIDDLE */
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3012","\\027\\050\\053\\048\\051\\048\\049\\050:./forms/form_3012.eps\\027",0,0,0,0); /* MACRO 3012 = VW CHK MICR SHORT STUB SPRINGFIELD MIDDLE */
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3013","\\027\\050\\053\\048\\051\\048\\049\\051:./forms/form_3013.eps\\027",0,0,0,0); /* MACRO 3013 = VW CHK MICR DOUBLE STUB BOTTOM */

		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3020","\\027\\050\\053\\048\\051\\048\\050\\048:./forms/form_3020.eps\\027",0,0,0,0); /* MACRO 3020 = PAYCHK MICR LONG STUB BOTTOM */

		w=49;
		y=48;
		z=48;
		for(x=0;x<400;x++)
		{
			memset(stemp,0,101);
			memset(stemp2,0,101);
			sprintf(stemp,"LOAD_PCLMACRO_%d",(3100+x));
			sprintf(stemp2,"\\027\\050\\053\\048\\051\\%03d\\%03d\\%03d:./forms/form_%04d.eps\\027",w,y,z,(3100+x));
			addRDApvar(PRTDEF,stemp,stemp2,0,0,0);  /* MACRO 3100-3499 = FULL CHECK FORMS */
			z++;
			if(z==58)
			{
				y++;
				z=48;
			}
			if((x==99)||(x==199)||(x==299)||(x==399))
			{
				w++;
				y=48;
			}
		}

		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_5000","\\027\\050\\053\\048\\053\\048\\048\\048:./forms/form_5000.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_6000","\\027\\050\\053\\048\\054\\048\\048\\048:./forms/form_6000.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_7000","\\027\\050\\053\\048\\055\\048\\048\\048:./forms/form_7000.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_8000","\\027\\050\\053\\048\\056\\048\\048\\048:./forms/form_8000.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_9000","\\027\\050\\053\\048\\057\\048\\048\\048:./forms/form_9000.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_7005","\\027\\050\\053\\048\\058\\048\\048\\048:./forms/form_7005.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"DIRECT DEPOSIT","",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"PAYROLL CHECK","",0,0,0,2);
/* #endif */
#ifdef WIN32_OLD
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1000","\\027\\050\\053\\048\\049\\048\\048\\048:.\\forms\\form_1000.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1001","\\027\\050\\053\\048\\049\\048\\048\\049:.\\forms\\form_1001.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1002","\\027\\050\\053\\048\\049\\048\\048\\050:.\\forms\\form_1002.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1003","\\027\\050\\053\\048\\049\\048\\048\\051:.\\forms\\form_1003.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1004","\\027\\050\\053\\048\\049\\048\\048\\052:.\\forms\\form_1004.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1005","\\027\\050\\053\\048\\049\\048\\048\\053:.\\forms\\form_1005.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1006","\\027\\050\\053\\048\\049\\048\\048\\054:.\\forms\\form_1006.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1010","\\027\\050\\053\\048\\049\\048\\049\\048:.\\forms\\form_1010.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1011","\\027\\050\\053\\048\\049\\048\\049\\049:.\\forms\\form_1011.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1012","\\027\\050\\053\\048\\049\\048\\049\\050:.\\forms\\form_1012.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1013","\\027\\050\\053\\048\\049\\048\\049\\051:.\\forms\\form_1013.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1014","\\027\\050\\053\\048\\049\\048\\049\\052:.\\forms\\form_1014.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1015","\\027\\050\\053\\048\\049\\048\\049\\053:.\\forms\\form_1015.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1016","\\027\\050\\053\\048\\049\\048\\049\\054:.\\forms\\form_1016.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1017","\\027\\050\\053\\048\\049\\048\\049\\055:.\\forms\\form_1017.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1018","\\027\\050\\053\\048\\049\\048\\049\\056:.\\forms\\form_1018.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1019","\\027\\050\\053\\048\\049\\048\\040\\057:.\\forms\\form_1019.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1020","\\027\\050\\053\\048\\049\\048\\050\\048:.\\forms\\form_1020.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1021","\\027\\050\\053\\048\\049\\048\\050\\049:.\\forms\\form_1021.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1022","\\027\\050\\053\\048\\049\\048\\050\\050:.\\forms\\form_1022.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1023","\\027\\050\\053\\048\\049\\048\\050\\051:.\\forms\\form_1023.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1024","\\027\\050\\053\\048\\049\\048\\050\\052:.\\forms\\form_1024.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1025","\\027\\050\\053\\048\\049\\048\\050\\053:.\\forms\\form_1025.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2000","\\027\\050\\053\\048\\050\\048\\048\\048:.\\forms\\form_2000.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2001","\\027\\050\\053\\048\\050\\048\\048\\049:.\\forms\\form_2001.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2002","\\027\\050\\053\\048\\050\\048\\048\\050:.\\forms\\form_2002.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2003","\\027\\050\\053\\048\\050\\048\\048\\051:.\\forms\\form_2003.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2005","\\027\\050\\053\\048\\050\\048\\048\\053:.\\forms\\form_2005.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2100","\\027\\050\\053\\048\\050\\049\\048\\048:.\\forms\\form_2100.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2101","\\027\\050\\053\\048\\050\\049\\048\\049:.\\forms\\form_2101.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2102","\\027\\050\\053\\048\\050\\049\\048\\050:.\\forms\\form_2102.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2103","\\027\\050\\053\\048\\050\\049\\048\\051:.\\forms\\form_2103.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2104","\\027\\050\\053\\048\\050\\049\\048\\052:.\\forms\\form_2104.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2105","\\027\\050\\053\\048\\050\\049\\048\\053:.\\forms\\form_2105.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2106","\\027\\050\\053\\048\\050\\049\\048\\054:.\\forms\\form_2106.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2107","\\027\\050\\053\\048\\050\\049\\048\\055:.\\forms\\form_2107.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2108","\\027\\050\\053\\048\\050\\049\\048\\056:.\\forms\\form_2108.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2109","\\027\\050\\053\\048\\050\\049\\048\\057:.\\forms\\form_2109.eps\\027",0,0,0,0);

		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3000","\\027\\050\\053\\048\\051\\048\\048\\048:.\\forms\\form_3000.eps\\027",0,0,0,0); /* MACRO 3000 = VENCHK MICR LONG STUB BOTTOM */
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3001","\\027\\050\\053\\048\\051\\048\\048\\049:.\\forms\\form_3001.eps\\027",0,0,0,0); /* MACRO 3001 = VENCHK MICR SHORT STUB MIDDLE */
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3002","\\027\\050\\053\\048\\051\\048\\048\\050:.\\forms\\form_3002.eps\\027",0,0,0,0); /* MACRO 3002 = VENCHK MICR SHORT STUB SPRINGFIELD MIDDLE */
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3003","\\027\\050\\053\\048\\051\\048\\048\\051:.\\forms\\form_3003.eps\\027",0,0,0,0); /* MACRO 3003 = VENCHK MICR DOUBLE STUB BOTTOM */

		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3010","\\027\\050\\053\\048\\051\\048\\049\\048:.\\forms\\form_3010.eps\\027",0,0,0,0); /* MACRO 3010 = VW CHK MICR LONG STUB BOTTOM */
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3011","\\027\\050\\053\\048\\051\\048\\049\\049:.\\forms\\form_3011.eps\\027",0,0,0,0); /* MACRO 3011 = VW CHK MICR SHORT STUB MIDDLE */
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3012","\\027\\050\\053\\048\\051\\048\\049\\050:.\\forms\\form_3012.eps\\027",0,0,0,0); /* MACRO 3012 = VW CHK MICR SHORT STUB SPRINGFIELD MIDDLE */
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3013","\\027\\050\\053\\048\\051\\048\\049\\051:.\\forms\\form_3013.eps\\027",0,0,0,0); /* MACRO 3013 = VW CHK MICR DOUBLE STUB BOTTOM */

		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_3020","\\027\\050\\053\\048\\051\\048\\050\\048:.\\forms\\form_3020.eps\\027",0,0,0,0); /* MACRO 3020 = VW CHK MICR LONG STUB BOTTOM */

		w=49;
		y=48;
		z=48;
		for(x=0;x<400;x++)
		{
			memset(stemp,0,101);
			memset(stemp2,0,101);
			sprintf(stemp,"LOAD_PCLMACRO_%d",(3100+x));
			sprintf(stemp2,"\\027\\050\\053\\048\\051\\%03d\\%03d\\%03d:.\\forms\\form_%04d.eps\\027",w,y,z,(3100+x));
			addRDApvar(PRTDEF,stemp,stemp2,0,0,0);  /* MACRO 3100-3499 = FULL CHECK FORMS */
			z++;
			if(z==58)
			{
				y++;
				z=48;
			}
			if((x==99)||(x==199)||(x==299)||(x==399))
			{
				w++;
				y=48;
			}
		}


		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_5000","\\027\\050\\053\\048\\053\\048\\048\\048:.\\forms\\form_5000.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_6000","\\027\\050\\053\\048\\054\\048\\048\\048:.\\forms\\form_6000.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_7000","\\027\\050\\053\\048\\055\\048\\048\\048:.\\forms\\form_7000.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_8000","\\027\\050\\053\\048\\056\\048\\048\\048:.\\forms\\form_8000.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_9000","\\027\\050\\053\\048\\057\\048\\048\\048:.\\forms\\form_9000.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_7000","\\027\\050\\053\\048\\058\\048\\048\\048:.\\forms\\form_7005.eps\\027",0,0,0,0);
		ADVaddRDApvar(PRTDEF,"DIRECT DEPOSIT","\\027\\051\\053\\053\\027",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"PAYROLL CHECK","\\027\\051\\053\\053\\027",0,0,0,2);
#endif
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(17+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [%s], Can Not Save Printer Definition!",devname);
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void TXT2PS()
{
#ifndef WIN32
	TXT2PScodes("viewpdf.lnx",TRUE);
	TXT2PScodes("viewpdf.exe",FALSE);
#endif
#ifdef WIN32
	TXT2PScodes("viewpdf.lnx",FALSE);
	TXT2PScodes("viewpdf.exe",TRUE);
#endif
	TXT2PScodes("TXT2PS",FALSE);
	CheckFontViewPDF();
}
void HP_LASER_8000n()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;
	FILE *fp=NULL;

	PRTDEF=RDApdevNEW("HP LASER 8000n");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		PRTDEF->page_length=10;
		addRDApvar(PRTDEF,"RESET PRINTER","\\027E",0,0,0);
		addRDApvar(PRTDEF,"SIGNATURE DIMM 1","\\027\\038\\102\\048\\083\\027\\038\\102\\049\\048\\048\\048\\051\\089\\027\\038\\102\\050\\088\\027\\038\\102\\049\\083",0,0,0);
		addRDApvar(PRTDEF,"SIGNATURE DIMM A","\\027&f10003Y\\027&f2X",0,0,0);
		addRDApvar(PRTDEF,"PUSH CURSOR","\\027\\038\\102\\048\\083",0,0,0);
		addRDApvar(PRTDEF,"POP CURSOR","\\027\\038\\102\\049\\083",0,0,0);
		addRDApvar(PRTDEF,"NORMAL TEXT","\\027\\038\\108\\054\\068\\027\\038\\107\\048\\083",1,10,12);
		addRDApvar(PRTDEF,"COMPRESSED TEXT","\\027\\038\\108\\056\\068\\027\\038\\107\\050\\083",1,17,9);
		addRDApvar(PRTDEF,"ELITE PRINT","\\027\\038\\108\\056\\068\\027\\038\\107\\052\\083",256,12,9);
		addRDApvar(PRTDEF,"PAGE_SIZE_LETTER","\\027\\038\\108\\050\\065",0,0,0);
		addRDApvar(PRTDEF,"PAGE_SIZE_LEGAL","\\027\\038\\108\\051\\065",0,0,0);
		addRDApvar(PRTDEF,"PAGE_SIZE_LEDGER","\\027\\038\\108\\054\\065",0,0,0);
		addRDApvar(PRTDEF,"PORTRAIT","\\027\\038\\108\\048\\079",0,0,0);
		addRDApvar(PRTDEF,"LANDSCAPE","\\027\\038\\108\\049\\079",0,0,0);
		addRDApvar(PRTDEF,"SIMPLEX","\\027\\038\\108\\048\\083",0,0,0);
		addRDApvar(PRTDEF,"DUPLEX LONG","\\027\\038\\108\\049\\083",0,0,0);
		addRDApvar(PRTDEF,"DUPLEX SHORT","\\027\\038\\108\\050\\083",0,0,0);
		addRDApvar(PRTDEF,"REVERSE PORTRAIT","\\027\\038\\108\\050\\079",0,0,0);
		addRDApvar(PRTDEF,"REVERSE LANDSCAPE","\\027\\038\\108\\051\\079",0,0,0);
		addRDApvar(PRTDEF,"ESCAPE","\\027",0,0,0);
		addRDApvar(PRTDEF,"PCL JOB HEADER","\\027\\037\\045\\049\\050\\051\\052\\053\\088\\064\\080\\074\\076\\032\\069\\078\\084\\069\\082\\032\\076\\065\\078\\071\\085\\065\\071\\069\\061\\080\\067\\076\\013\\010\\027\\069",0,0,0);
		addRDApvar(PRTDEF,"PCL JOB FOOTER","\\027\\037\\045\\049\\050\\051\\052\\053\\088",0,0,0);
		addRDApvar(PRTDEF,"ASCII SQUARE","\\254",0,0,0);
		addRDApvar(PRTDEF,"HALF LINE FEED","\\027\\061",0,0,0);
		addRDApvar(PRTDEF,"NEG Q LINE FEED","\\027\\038\\097\\045\\051\\048\\086",0,0,0);
		addRDApvar(PRTDEF,"POS Q LINE FEED","\\027\\038\\097\\043\\051\\048\\086",0,0,0);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE LOCK","keylock.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE UNLOCK","keyunlock.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE FILE","./sig.dat",0,0,0,1);
#ifndef WIN32
		ADVaddRDApvar(PRTDEF,"W2FORM-4PP","./forms/W2FORM-4PP.pcl",0,0,0,1);
#endif
#ifdef WIN32
		ADVaddRDApvar(PRTDEF,"W2FORM-4PP",".\\forms\\W2FORM-4PP.pcl",0,0,0,1);
#endif
#ifndef WIN32
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_40","fonts/pcl/rda_40_mrvcode39extxs_10.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_43","fonts/pcl/rda_43_mrvcode39extxs_16.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_44","fonts/pcl/rda_44_mrvcode39extxs_18.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_45","fonts/pcl/rda_45_mrvcode39extxsa_10.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_46","fonts/pcl/rda_46_mrvcode39extxsa_12.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_47","fonts/pcl/rda_47_mrvcode39extxsa_14.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_48","fonts/pcl/rda_48_mrvcode39extxsa_16.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_49","fonts/pcl/rda_49_mrvcode39extxsa_18.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_60","./fonts/pcl/rda_60_idautomation_micr6.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_61","./fonts/pcl/rda_61_idautomation_micr6b.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_62","./fonts/pcl/rda_62_idautomation_micr6l.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_63","./fonts/pcl/rda_63_idautomation_micr6n1.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_64","./fonts/pcl/rda_64_idautomation_micr6n2.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_65","./fonts/pcl/rda_65_idautomation_micr6w1.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_66","./fonts/pcl/rda_66_idautomation_micr6w2.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_67","./fonts/pcl/rda_67_idautomation_micr3.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_68","./fonts/pcl/rda_68_idautomation_micr3b.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_69","./fonts/pcl/rda_69_idautomation_micr3l.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_70","./fonts/pcl/rda_70_idautomation_micr3n.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_71","./fonts/pcl/rda_71_idautomation_micr3w.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"UNLOAD_SOFTFONTS","fonts/pcl/unloadfonts.pcl",0,0,0,2);
#endif
#ifdef WIN32
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_40",".\\fonts\\pcl\\rda_40_mrvcode39extxs_10.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_43",".\\fonts\\pcl\\rda_43_mrvcode39extxs_16.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_44",".\\fonts\\pcl\\rda_44_mrvcode39extxs_18.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_45",".\\fonts\\pcl\\rda_45_mrvcode39extxs_10.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_46",".\\fonts\\pcl\\rda_46_mrvcode39extxs_12.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_47",".\\fonts\\pcl\\rda_47_mrvcode39extxs_14.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_48",".\\fonts\\pcl\\rda_48_mrvcode39extxs_16.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_49",".\\fonts\\pcl\\rda_49_mrvcode39extxs_18.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_60",".\\fonts\\pcl\\rda_60_idautomation_micr6.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_61",".\\fonts\\pcl\\rda_61_idautomation_micr6b.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_62",".\\fonts\\pcl\\rda_62_idautomation_micr6l.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_63",".\\fonts\\pcl\\rda_63_idautomation_micr6n1.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_64",".\\fonts\\pcl\\rda_64_idautomation_micr6n2.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_65",".\\fonts\\pcl\\rda_65_idautomation_micr6w1.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_66",".\\fonts\\pcl\\rda_66_idautomation_micr6w2.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_67",".\\fonts\\pcl\\rda_67_idautomation_micr3.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_68",".\\fonts\\pcl\\rda_68_idautomation_micr3b.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_69",".\\fonts\\pcl\\rda_69_idautomation_micr3l.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_70",".\\fonts\\pcl\\rda_70_idautomation_micr3n.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_SOFTFONT_71",".\\fonts\\pcl\\rda_71_idautomation_micr3w.sfp",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"UNLOAD_SOFTFONTS",".\\fonts\\pcl\\unloadfonts.pcl",0,0,0,2);
#endif
		addRDApvar(PRTDEF,"USE_SOFTFONT_40","\\027\\040\\048\\089\\027\\040\\115\\049\\112\\049\\048\\046\\048\\118\\048\\115\\048\\098\\051\\051\\050\\056\\048\\084",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_43","\\027\\040\\048\\089\\027\\040\\115\\049\\112\\049\\054\\046\\048\\118\\048\\115\\048\\098\\051\\051\\050\\056\\048\\084",0,0,0);

		addRDApvar(PRTDEF,"USE_SOFTFONT_44","\\027\\040\\048\\089\\027\\040\\115\\049\\112\\049\\056\\046\\048\\118\\048\\115\\048\\098\\051\\051\\050\\056\\048\\084",0,0,0);

		addRDApvar(PRTDEF,"USE_SOFTFONT_45","\\027\\040\\048\\089\\027\\040\\115\\049\\112\\049\\048\\046\\048\\118\\048\\115\\048\\098\\051\\051\\050\\056\\053\\084",0,0,0);

		addRDApvar(PRTDEF,"USE_SOFTFONT_46","\\027\\040\\048\\089\\027\\040\\115\\049\\112\\049\\050\\046\\048\\118\\048\\115\\048\\098\\051\\051\\050\\056\\053\\084",0,0,0);

		addRDApvar(PRTDEF,"USE_SOFTFONT_47","\\027\\040\\048\\089\\027\\040\\115\\049\\112\\049\\052\\046\\048\\118\\048\\115\\048\\098\\051\\051\\050\\056\\053\\084",0,0,0);

		addRDApvar(PRTDEF,"USE_SOFTFONT_48","\\027\\040\\048\\089\\027\\040\\115\\049\\112\\049\\054\\046\\048\\118\\048\\115\\048\\098\\051\\051\\050\\056\\053\\084",0,0,0);

		addRDApvar(PRTDEF,"USE_SOFTFONT_49","\\027\\040\\048\\089\\027\\040\\115\\049\\112\\049\\056\\046\\048\\118\\048\\115\\048\\098\\051\\051\\050\\056\\053\\084",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_60","\\027\\040\\054\\048\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_61","\\027\\040\\054\\049\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_62","\\027\\040\\054\\050\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_63","\\027\\040\\054\\051\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_64","\\027\\040\\054\\052\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_65","\\027\\040\\054\\053\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_66","\\027\\040\\054\\054\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_67","\\027\\040\\054\\055\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_68","\\027\\040\\054\\056\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_69","\\027\\040\\054\\057\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_70","\\027\\040\\055\\048\\088",0,0,0);
		addRDApvar(PRTDEF,"USE_SOFTFONT_71","\\027\\040\\055\\049\\088",0,0,0);


		addRDApvar(PRTDEF,"RESET FONT","\\027\\040\\051\\064",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_40","\\027\\042\\099\\052\\048\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_43","\\027\\042\\099\\052\\051\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_44","\\027\\042\\099\\052\\052\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_45","\\027\\042\\099\\052\\053\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_46","\\027\\042\\099\\052\\054\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_47","\\027\\042\\099\\052\\055\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_48","\\027\\042\\099\\052\\056\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_49","\\027\\042\\099\\052\\057\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_60","\\027\\042\\099\\054\\048\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_61","\\027\\042\\099\\054\\049\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_62","\\027\\042\\099\\054\\050\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_63","\\027\\042\\099\\054\\051\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_64","\\027\\042\\099\\054\\052\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_65","\\027\\042\\099\\054\\053\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_66","\\027\\042\\099\\054\\054\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_67","\\027\\042\\099\\054\\055\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_68","\\027\\042\\099\\054\\056\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_69","\\027\\042\\099\\054\\057\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_70","\\027\\042\\099\\055\\048\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_71","\\027\\042\\099\\055\\049\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_200","\\027\\042\\099\\050\\048\\048\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"DELETE_SOFTFONT_201","\\027\\042\\099\\050\\048\\049\\100\\050\\070",0,0,0);
		addRDApvar(PRTDEF,"USE_PCLMACRO_1000","\\027\\038\\102\\049\\048\\048\\048\\121\\051\\120",0,0,0); /* MACRO 1000 = VATANGIBLE LAST/ONLY DATA PAGE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1001","\\027\\038\\102\\049\\048\\048\\049\\121\\051\\120",0,0,0); /* MACRO 1001 = VATANGIBLE SIGNATURE PAGE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1002","\\027\\038\\102\\049\\048\\048\\050\\121\\051\\120",0,0,0); /* MACRO 1002 = VATANGIBLE ALL BUT LAST DATA PAGE OF MULTIPLE PAGES*/
		addRDApvar(PRTDEF,"USE_PCLMACRO_1003","\\027\\038\\102\\049\\048\\048\\051\\121\\051\\120",0,0,0); /* MACRO 1003 = FIFA PRP/RLST CITATION CANTON */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1004","\\027\\038\\102\\049\\048\\048\\052\\121\\051\\120",0,0,0); /* MACRO 1004 = VATANGIBLE PRIOR YEAR LAST/ONLY DATA PAGE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1005","\\027\\038\\102\\049\\048\\048\\053\\121\\051\\120",0,0,0); /* MACRO 1005 = VATANGIBLE PRIOR YEAR SIGNATURE PAGE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1006","\\027\\038\\102\\049\\048\\048\\054\\121\\051\\120",0,0,0); /* MACRO 1006 = VATANGIBLE PRIOR YEAR ALL BUT LAST DATA PAGE OF MULTIPLE PAGES*/
		addRDApvar(PRTDEF,"USE_PCLMACRO_1010","\\027\\038\\102\\049\\048\\049\\048\\121\\051\\120",0,0,0); /* MACRO 1010 = DELINQUENT MADISON PP*/
		addRDApvar(PRTDEF,"USE_PCLMACRO_1011","\\027\\038\\102\\049\\048\\049\\049\\121\\051\\120",0,0,0); /* MACRO 1011 = DELINQUENT MADISON RE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1012","\\027\\038\\102\\049\\048\\049\\050\\121\\051\\120",0,0,0); /* MACRO 1012 = DELINQUENT MADISON MISC */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1013","\\027\\038\\102\\049\\048\\049\\051\\121\\051\\120",0,0,0); /* MACRO 1013 = PROPERTY STATEMENT */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1014","\\027\\038\\102\\049\\048\\049\\052\\121\\051\\120",0,0,0); /* MACRO 1014 = REAL ESTATE STATEMENT */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1015","\\027\\038\\102\\049\\048\\049\\053\\121\\051\\120",0,0,0); /* MACRO 1015 = MISC BILLING STATEMENT */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1016","\\027\\038\\102\\049\\048\\049\\054\\121\\051\\120",0,0,0); /* MACRO 1016 = MADISON DMV STOP LETTER FORM */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1017","\\027\\038\\102\\049\\048\\049\\055\\121\\051\\120",0,0,0); /* MACRO 1017 = SUPPLEMENTAL MADISON PP*/
		addRDApvar(PRTDEF,"USE_PCLMACRO_1018","\\027\\038\\102\\049\\048\\049\\056\\121\\051\\120",0,0,0); /* MACRO 1018 = SUPPLEMENTAL MADISON RE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1019","\\027\\038\\102\\049\\048\\049\\057\\121\\051\\120",0,0,0); /* MACRO 1019 = SUPPLEMENTAL MADISON MISC */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1020","\\027\\038\\102\\049\\048\\050\\048\\121\\051\\120",0,0,0); /* MACRO 1020 = MISC BILLING INCOME TAX FORM 759 */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1021","\\027\\038\\102\\049\\048\\050\\049\\121\\051\\120",0,0,0); /* MACRO 1021 = MISC BILLING INTERMEDIATE INVOICE TICKET */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1022","\\027\\038\\102\\049\\048\\050\\050\\121\\051\\120",0,0,0); /* MACRO 1022 = MISC BILLING FINAL INVOICE TICKET */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1023","\\027\\038\\102\\049\\048\\050\\051\\121\\051\\120",0,0,0); /* MACRO 1023 = REAL ESTATE NOTICE OF ASSESSMENT */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1024","\\027\\038\\102\\049\\048\\050\\052\\121\\051\\120",0,0,0); /* MACRO 1024 = MISC BILLING ALCOHL LICENSE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_1025","\\027\\038\\102\\049\\048\\050\\053\\121\\051\\120",0,0,0); /* MACRO 1024 = MISC BILLING INVOICE STATEMENT */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2000","\\027\\038\\102\\050\\048\\048\\048\\121\\051\\120",0,0,0); /* MACRO 2000 = PAYROLL FEDERAL 941 PAGE 1 */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2001","\\027\\038\\102\\050\\048\\048\\049\\121\\051\\120",0,0,0); /* MACRO 2001 = PAYROLL FEDERAL 941 PAGE 2 */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2002","\\027\\038\\102\\050\\048\\048\\050\\121\\051\\120",0,0,0); /* MACRO 2002 = PAYROLL FEDERAL 941 PAGE 3 */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2003","\\027\\038\\102\\050\\048\\048\\051\\121\\051\\120",0,0,0); /* MACRO 2003 = PAYROLL FEDERAL 941 PAGE 4 */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2005","\\027\\038\\102\\050\\048\\048\\053\\121\\051\\120",0,0,0); /* MACRO 2005 = PAYROLL BENEFITRECAP */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2100","\\027\\038\\102\\050\\049\\048\\048\\121\\051\\120",0,0,0); /* MACRO 2100 = 2010 W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2101","\\027\\038\\102\\050\\049\\048\\049\\121\\051\\120",0,0,0); /* MACRO 2101 = 2010 W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2102","\\027\\038\\102\\050\\049\\048\\050\\121\\051\\120",0,0,0); /* MACRO 2102 = 2010 W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2103","\\027\\038\\102\\050\\049\\048\\051\\121\\051\\120",0,0,0); /* MACRO 2103 = 2010 W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2104","\\027\\038\\102\\050\\049\\048\\052\\121\\051\\120",0,0,0); /* MACRO 2104 = 2010 W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2105","\\027\\038\\102\\050\\049\\048\\053\\121\\051\\120",0,0,0); /* MACRO 2105 = 2010 W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2106","\\027\\038\\102\\050\\049\\048\\054\\121\\051\\120",0,0,0); /* MACRO 2106 = 2011 4 PART W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2107","\\027\\038\\102\\050\\049\\048\\055\\121\\051\\120",0,0,0); /* MACRO 2107 = 2011 4 PART W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2108","\\027\\038\\102\\050\\049\\048\\056\\121\\051\\120",0,0,0); /* MACRO 2108 = 2012 4 PART W2 FORMS */
		addRDApvar(PRTDEF,"USE_PCLMACRO_2109","\\027\\038\\102\\050\\049\\048\\057\\121\\051\\120",0,0,0); /* MACRO 2109 = 2012 4 PART W2 FORMS */

		addRDApvar(PRTDEF,"USE_PCLMACRO_5000","\\027\\038\\102\\053\\048\\048\\048\\121\\051\\120",0,0,0); /* MACRO 5000 = ACTUAL PURCHASE ORDER */
		addRDApvar(PRTDEF,"USE_PCLMACRO_6000","\\027\\038\\102\\054\\048\\048\\048\\121\\051\\120",0,0,0); /* MACRO 6000 = ACTUAL REQUISITION */
		addRDApvar(PRTDEF,"USE_PCLMACRO_7000","\\027\\038\\102\\055\\048\\048\\048\\121\\051\\120",0,0,0); /* MACRO 7000 = OCCTAX CERTIFICATE */
		addRDApvar(PRTDEF,"USE_PCLMACRO_8000","\\027\\038\\102\\056\\048\\048\\048\\121\\051\\120",0,0,0); /* MACRO 7000 = BUILDING PERMIT */
		addRDApvar(PRTDEF,"USE_PCLMACRO_9000","\\027\\038\\102\\057\\048\\048\\048\\121\\051\\120",0,0,0); /* MACRO 9000 = UTLBLL WORK ORDER */
		addRDApvar(PRTDEF,"USE_PCLMACRO_7005","\\027\\038\\102\\058\\048\\048\\048\\121\\051\\120",0,0,0); /* MACRO 7005 = OCCTAX STATEMENT */
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1000","\\027\\038\\102\\049\\048\\048\\048\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1001","\\027\\038\\102\\049\\048\\048\\049\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1002","\\027\\038\\102\\049\\048\\048\\050\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1003","\\027\\038\\102\\049\\048\\048\\051\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1004","\\027\\038\\102\\049\\048\\048\\052\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1005","\\027\\038\\102\\049\\048\\048\\053\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1006","\\027\\038\\102\\049\\048\\048\\054\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1010","\\027\\038\\102\\049\\048\\049\\048\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1011","\\027\\038\\102\\049\\048\\049\\049\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1012","\\027\\038\\102\\049\\048\\049\\050\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1013","\\027\\038\\102\\049\\048\\049\\051\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1014","\\027\\038\\102\\049\\048\\049\\052\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1015","\\027\\038\\102\\049\\048\\049\\053\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1016","\\027\\038\\102\\049\\048\\049\\054\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1017","\\027\\038\\102\\049\\048\\049\\055\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1018","\\027\\038\\102\\049\\048\\049\\056\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1019","\\027\\038\\102\\049\\048\\049\\057\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1020","\\027\\038\\102\\049\\048\\050\\048\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1021","\\027\\038\\102\\049\\048\\050\\049\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1022","\\027\\038\\102\\049\\048\\050\\050\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1023","\\027\\038\\102\\049\\048\\050\\051\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1024","\\027\\038\\102\\049\\048\\050\\052\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_1025","\\027\\038\\102\\049\\048\\050\\053\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2000","\\027\\038\\102\\050\\048\\048\\048\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2001","\\027\\038\\102\\050\\048\\048\\049\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2002","\\027\\038\\102\\050\\048\\048\\050\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2003","\\027\\038\\102\\050\\048\\048\\051\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2005","\\027\\038\\102\\050\\048\\048\\053\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2005","\\027\\038\\102\\050\\048\\048\\053\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2100","\\027\\038\\102\\050\\049\\048\\048\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2101","\\027\\038\\102\\050\\049\\048\\049\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2102","\\027\\038\\102\\050\\049\\048\\050\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2103","\\027\\038\\102\\050\\049\\048\\051\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2104","\\027\\038\\102\\050\\049\\048\\052\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2105","\\027\\038\\102\\050\\049\\048\\053\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2106","\\027\\038\\102\\050\\049\\048\\054\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2107","\\027\\038\\102\\050\\049\\048\\055\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2108","\\027\\038\\102\\050\\049\\048\\056\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_2109","\\027\\038\\102\\050\\049\\048\\057\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_5000","\\027\\038\\102\\053\\048\\048\\048\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_6000","\\027\\038\\102\\054\\048\\048\\048\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_7000","\\027\\038\\102\\055\\048\\048\\048\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_8000","\\027\\038\\102\\056\\048\\048\\048\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_9000","\\027\\038\\102\\057\\048\\048\\048\\121\\056\\120",0,0,0);
		addRDApvar(PRTDEF,"DELETE_PCLMACRO_7005","\\027\\038\\102\\058\\048\\048\\048\\121\\056\\120",0,0,0);
		ADVaddRDApvar(PRTDEF,"LOAD VENPMT SIGNATURE","",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD PAYROLL SIGNATURE","",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD PO SIGNATURE","",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"VENPMT SIGNATURE","vensig.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYROLL SIGNATURE","paysig.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PO SIGNATURE","posig.dat",0,0,0,1);
#ifndef WIN32
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1000","./forms/load_pclmacro_1000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1001","./forms/load_pclmacro_1001.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1002","./forms/load_pclmacro_1002.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1003","./forms/load_pclmacro_1003.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1004","./forms/load_pclmacro_1004.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1005","./forms/load_pclmacro_1005.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1006","./forms/load_pclmacro_1006.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1010","./forms/load_pclmacro_1010.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1011","./forms/load_pclmacro_1011.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1012","./forms/load_pclmacro_1012.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1013","./forms/load_pclmacro_1013.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1014","./forms/load_pclmacro_1014.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1015","./forms/load_pclmacro_1015.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1016","./forms/load_pclmacro_1016.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1017","./forms/load_pclmacro_1017.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1018","./forms/load_pclmacro_1018.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1019","./forms/load_pclmacro_1019.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1020","./forms/load_pclmacro_1020.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1021","./forms/load_pclmacro_1021.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1022","./forms/load_pclmacro_1022.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1023","./forms/load_pclmacro_1023.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1024","./forms/load_pclmacro_1024.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1025","./forms/load_pclmacro_1025.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2000","./forms/load_pclmacro_2000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2001","./forms/load_pclmacro_2001.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2002","./forms/load_pclmacro_2002.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2003","./forms/load_pclmacro_2003.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2005","./forms/load_pclmacro_2005.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2100","./forms/load_pclmacro_2100.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2101","./forms/load_pclmacro_2101.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2102","./forms/load_pclmacro_2102.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2103","./forms/load_pclmacro_2103.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2104","./forms/load_pclmacro_2104.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2105","./forms/load_pclmacro_2105.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2106","./forms/load_pclmacro_2106.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2107","./forms/load_pclmacro_2107.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2108","./forms/load_pclmacro_2108.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2109","./forms/load_pclmacro_2109.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_5000","./forms/load_pclmacro_5000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_6000","./forms/load_pclmacro_6000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_7000","./forms/load_pclmacro_7000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_7005","./forms/load_pclmacro_7005.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_8000","./forms/load_pclmacro_8000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_9000","./forms/load_pclmacro_9000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"DIRECT DEPOSIT","",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"PAYROLL CHECK","",0,0,0,2);
#endif
#ifdef WIN32
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1000",".\\forms\\load_pclmacro_1000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1001",".\\forms\\load_pclmacro_1001.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1002",".\\forms\\load_pclmacro_1002.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1003",".\\forms\\load_pclmacro_1003.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1004",".\\forms\\load_pclmacro_1004.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1005",".\\forms\\load_pclmacro_1005.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1006",".\\forms\\load_pclmacro_1006.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1010",".\\forms\\load_pclmacro_1010.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1011",".\\forms\\load_pclmacro_1011.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1012",".\\forms\\load_pclmacro_1012.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1013",".\\forms\\load_pclmacro_1013.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1014",".\\forms\\load_pclmacro_1014.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1015",".\\forms\\load_pclmacro_1015.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1016",".\\forms\\load_pclmacro_1016.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1017",".\\forms\\load_pclmacro_1017.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1018",".\\forms\\load_pclmacro_1018.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1019",".\\forms\\load_pclmacro_1019.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1020",".\\forms\\load_pclmacro_1020.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_1021",".\\forms\\load_pclmacro_1021.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2000",".\\forms\\load_pclmacro_2000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2001",".\\forms\\load_pclmacro_2001.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2002",".\\forms\\load_pclmacro_2002.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2003",".\\forms\\load_pclmacro_2003.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2005",".\\forms\\load_pclmacro_2005.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2100",".\\forms\\load_pclmacro_2100.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2101",".\\forms\\load_pclmacro_2101.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2102",".\\forms\\load_pclmacro_2102.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2103",".\\forms\\load_pclmacro_2103.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2104",".\\forms\\load_pclmacro_2104.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2105",".\\forms\\load_pclmacro_2105.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2106",".\\forms\\load_pclmacro_2106.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2107",".\\forms\\load_pclmacro_2107.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2108",".\\forms\\load_pclmacro_2108.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_2109",".\\forms\\load_pclmacro_2109.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_5000",".\\forms\\load_pclmacro_5000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_6000",".\\forms\\load_pclmacro_6000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_7000",".\\forms\\load_pclmacro_7000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_7005",".\\forms\\load_pclmacro_7005.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_8000",".\\forms\\load_pclmacro_8000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"LOAD_PCLMACRO_9000",".\\forms\\load_pclmacro_9000.pcl",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"DIRECT DEPOSIT","",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"PAYROLL CHECK","",0,0,0,2);
#endif
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(17+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [HP LASER 8000n], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
	unlink("keylock.dat");
	fp=fopen("keylock.dat","w");
	if(fp!=NULL)
	{
		fprintf(fp,"~AM0~");
		fclose(fp);
	}
	unlink("keyunlock.dat");
	fp=fopen("keyunlock.dat","w");
	if(fp!=NULL)
	{
		fprintf(fp,"~AM1~");
		fclose(fp);
	}
}
void CASHDRAWER()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("CASHDRAWER");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		addRDApvar(PRTDEF,"NORMAL TEXT","\\18",1,10,12);
		addRDApvar(PRTDEF,"COMPRESSED TEXT","\\15",1,17,9);
		addRDApvar(PRTDEF,"CUT RECEIPT","&%FC",0,0,0);
		addRDApvar(PRTDEF,"KICK DRAWER","&%D1",0,0,0);
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(10+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [CASHDRAWER], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void HP_DESKJET_600()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("HP DESKJET 600+");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		PRTDEF->page_length=9.9;
		addRDApvar(PRTDEF,"NORMAL TEXT","\\027\\038\\108\\054\\068\\027\\040\\115\\049\\048\\072\\027\\040\\115\\049\\050\\086",1,10,12);
		addRDApvar(PRTDEF,"COMPRESSED TEXT","\\027\\038\\108\\056\\068\\027\\040\\115\\049\\055\\072\\027\\040\\115\\057\\086",1,17,9);
		addRDApvar(PRTDEF,"PORTRAIT","\\027\\038\\108\\048\\079",0,0,0);
		addRDApvar(PRTDEF,"LANDSCAPE","\\027\\038\\108\\049\\079",0,0,0);
		addRDApvar(PRTDEF,"PUSH CURSOR","\\027\\038\\102\\048\\083",0,0,0);
		addRDApvar(PRTDEF,"POP CURSOR","\\027\\038\\102\\049\\083",0,0,0);
		addRDApvar(PRTDEF,"HALF LINE FEED","\\027\\061",0,0,0);
		addRDApvar(PRTDEF,"NEG Q LINE FEED","\\027\\038\\097\\045\\051\\048\\086",0,0,0);
		addRDApvar(PRTDEF,"POS Q LINE FEED","\\027\\038\\097\\043\\051\\048\\086",0,0,0);
		ADVaddRDApvar(PRTDEF,"FW2_01A.PCL","FW2_01A.PCL",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE LOCK","keylock.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE UNLOCK","keyunlock.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE FILE","./sig.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		addRDApvar(PRTDEF,"SIMPLEX","\\027\\038\\108\\048\\083",0,0,0);
		addRDApvar(PRTDEF,"DUPLEX LONG","\\027\\038\\108\\049\\083",0,0,0);
		addRDApvar(PRTDEF,"DUPLEX SHORT","\\027\\038\\108\\050\\083",0,0,0);
		addRDApvar(PRTDEF,"ESCAPE","\\027",0,0,0);
#ifndef WIN32
		ADVaddRDApvar(PRTDEF,"W2FORM-4PP","./forms/W2FORM-4PP.pcl",0,0,0,1);
#endif
#ifdef WIN32
		ADVaddRDApvar(PRTDEF,"W2FORM-4PP",".\\forms\\W2FORM-4PP.pcl",0,0,0,1);
#endif
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [HP DESKJET 600+], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void HP_DESKJET_520()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("HP DESKJET 520");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		PRTDEF->page_length=9.9;
		addRDApvar(PRTDEF,"NORMAL TEXT","\\027\\038\\108\\006\\068\\027\\040\\115\\010\\072\\027\\040\\115\\012\\086",1,10,12);
		addRDApvar(PRTDEF,"COMPRESSED TEXT","\\027\\038\\108\\008\\068\\027\\040\\115\\016\\072\\027\\040\\115\\009\\086",1,17,9);
		addRDApvar(PRTDEF,"PORTRAIT","\\027\\038\\108\\048\\079",0,0,0);
		addRDApvar(PRTDEF,"LANDSCAPE","\\027\\038\\108\\049\\079",0,0,0);
		addRDApvar(PRTDEF,"PUSH CURSOR","\\027\\038\\102\\048\\083",0,0,0);
		addRDApvar(PRTDEF,"POP CURSOR","\\027\\038\\102\\049\\083",0,0,0);
		addRDApvar(PRTDEF,"HALF LINE FEED","\\027\\061",0,0,0);
		addRDApvar(PRTDEF,"NEG Q LINE FEED","\\027\\038\\097\\045\\051\\048\\086",0,0,0);
		addRDApvar(PRTDEF,"POS Q LINE FEED","\\027\\038\\097\\043\\051\\048\\086",0,0,0);
		ADVaddRDApvar(PRTDEF,"FW2_01A.PCL","FW2_01A.PCL",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE LOCK","keylock.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE UNLOCK","keyunlock.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE FILE","./sig.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		addRDApvar(PRTDEF,"SIMPLEX","\\027\\038\\108\\048\\083",0,0,0);
		addRDApvar(PRTDEF,"DUPLEX LONG","\\027\\038\\108\\049\\083",0,0,0);
		addRDApvar(PRTDEF,"DUPLEX SHORT","\\027\\038\\108\\050\\083",0,0,0);
		addRDApvar(PRTDEF,"ESCAPE","\\027",0,0,0);
#ifndef WIN32
		ADVaddRDApvar(PRTDEF,"W2FORM-4PP","./forms/W2FORM-4PP.pcl",0,0,0,1);
#endif
#ifdef WIN32
		ADVaddRDApvar(PRTDEF,"W2FORM-4PP",".\\forms\\W2FORM-4PP.pcl",0,0,0,1);
#endif
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [HP DESKJET 600+], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void PRT_ANADEX()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("ANADEX");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		addRDApvar(PRTDEF,"NORMAL","\\18",1,10,12);
		addRDApvar(PRTDEF,"CONDENSED","\\20",1,17,9);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(6+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [ANADEX], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void PRT_BROTHER()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("BROTHER");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		addRDApvar(PRTDEF,"NORMAL","\\18",1,10,12);
		addRDApvar(PRTDEF,"CONDENSED","\\15",1,17,9);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(7+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [BROTHER], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void PRT_C_ITOH_ELECTRONICS()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("C. ITOH ELECTRONICS");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		addRDApvar(PRTDEF,"NORMAL","\\027\\078",1,10,12);
		addRDApvar(PRTDEF,"CONDENSED","\\027\\081",1,17,9);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(19+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [C. ITOH ELECTRONICS], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void PRT_CENTRONICS()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("CENTRONICS");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		addRDApvar(PRTDEF,"NORMAL","\\18",1,10,12);
		addRDApvar(PRTDEF,"CONDENSED","\\15",1,17,9);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(10+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [CENTRONICS], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void PRT_DATA_ROYAL()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("DATA ROYAL");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		addRDApvar(PRTDEF,"NORMAL","\\027\\054",1,10,12);
		addRDApvar(PRTDEF,"CONDENSED","\\027\\055",1,17,9);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(10+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [DATA ROYAL], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void PRT_DATA_SOUTH()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("DATA SOUTH");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		addRDApvar(PRTDEF,"NORMAL","\\018",1,10,12);
		addRDApvar(PRTDEF,"CONDENSED","\\015",1,17,9);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(10+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [DATA SOUTH], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void PRT_IBM()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("IBM");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		addRDApvar(PRTDEF,"NORMAL","\\018",1,10,12);
		addRDApvar(PRTDEF,"CONDENSED","\\015",1,17,9);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(3+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [IBM], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void PRT_NEC()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("NEC");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		addRDApvar(PRTDEF,"NORMAL","\\018",1,10,12);
		addRDApvar(PRTDEF,"CONDENSED","\\015",1,17,9);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(3+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [NEC], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void PRT_OKIDATA()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("OKIDATA");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		addRDApvar(PRTDEF,"NORMAL","\\030",1,10,12);
		addRDApvar(PRTDEF,"CONDENSED","\\029",1,17,9);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(7+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [OKIDATA], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void PRT_TANDY()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("TANDY");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		addRDApvar(PRTDEF,"NORMAL","\\027\\019",1,10,12);
		addRDApvar(PRTDEF,"CONDENSED","\\027\\020",1,17,9);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(5+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [TANDY], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void PRT_TEXAS_INSTRUMENTS()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("TEXAS INSTRUMENTS");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		addRDApvar(PRTDEF,"NORMAL","\\027\\054",1,10,12);
		addRDApvar(PRTDEF,"CONDENSED","\\027\\055",1,17,9);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(17+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [TEXAS INSTRUMENTS], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void PRT_TOSHIBA()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("TOSHIBA");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		addRDApvar(PRTDEF,"NORMAL","\\027\\093",1,10,12);
		addRDApvar(PRTDEF,"CONDENSED","\\027\\091",1,17,9);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(7+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [TOSHIBA], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void PRT_ZEROX_LETTER_QUALITY()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("ZEROX LETTER QUALITY");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		addRDApvar(PRTDEF,"NORMAL","\\018",1,10,12);
		addRDApvar(PRTDEF,"CONDENSED","\\015",1,17,9);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(20+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [ZEROX LETTER QUALITY], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void PRT_HP_LASER_JET_1100()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("HP LASER JET 1100");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		PRTDEF->page_length=10;
		addRDApvar(PRTDEF,"RESET PRINTER","\\027\\069",0,0,0);
		addRDApvar(PRTDEF,"PORTRAIT","\\027\\038\\108\\048\\079",0,0,0);
		addRDApvar(PRTDEF,"LANDSCAPE","\\027\\038\\108\\049\\079",0,0,0);
		addRDApvar(PRTDEF,"NORMAL TEXT","\\027\\038\\107\\048\\083\\027\\040\\115\\049\\050\\086",1,10,12);
		addRDApvar(PRTDEF,"COMPRESSED TEXT","\\027\\038\\107\\048\\083\\027\\040\\115\\057\\086",1,17,9);
		addRDApvar(PRTDEF,"PUSH CURSOR","\\027\\038\\102\\048\\083",0,0,0);
		addRDApvar(PRTDEF,"POP CURSOR","\\027\\038\\102\\049\\083",0,0,0);
		addRDApvar(PRTDEF,"HALF LINE FEED","\\027\\061",0,0,0);
		addRDApvar(PRTDEF,"NEG Q LINE FEED","\\027\\038\\097\\045\\051\\048\\086",0,0,0);
		addRDApvar(PRTDEF,"POS Q LINE FEED","\\027\\038\\097\\043\\051\\048\\086",0,0,0);
		ADVaddRDApvar(PRTDEF,"FW2_01A.PCL","FW2_01A.PCL",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE LOCK","keylock.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE UNLOCK","keyunlock.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE FILE","./sig.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		addRDApvar(PRTDEF,"SIMPLEX","\\027\\038\\108\\048\\083",0,0,0);
		addRDApvar(PRTDEF,"DUPLEX LONG","\\027\\038\\108\\049\\083",0,0,0);
		addRDApvar(PRTDEF,"DUPLEX SHORT","\\027\\038\\108\\050\\083",0,0,0);
		addRDApvar(PRTDEF,"ESCAPE","\\027",0,0,0);
#ifndef WIN32
		ADVaddRDApvar(PRTDEF,"W2FORM-4PP","./forms/W2FORM-4PP.pcl",0,0,0,1);
#endif
#ifdef WIN32
		ADVaddRDApvar(PRTDEF,"W2FORM-4PP",".\\forms\\W2FORM-4PP.pcl",0,0,0,1);
#endif
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(17+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [HP LASER JET 1100], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void PRT_OfficeJet()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("OfficeJet");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		PRTDEF->page_length=10;
		addRDApvar(PRTDEF,"RESET PRINTER","\\027E",0,0,0);
		addRDApvar(PRTDEF,"SIGNATURE DIMM 1","\\027\\038\\102\\048\\083\\027\\038\\102\\049\\048\\048\\048\\051\\089\\027\\038\\102\\050\\088\\027\\038\\102\\049\\083",0,0,0);
		addRDApvar(PRTDEF,"SIGNATURE DIMM A","\\027&f10003Y\\027&f2X",0,0,0);
		addRDApvar(PRTDEF,"PUSH CURSOR","\\027\\038\\102\\048\\083",0,0,0);
		addRDApvar(PRTDEF,"POP CURSOR","\\027\\038\\102\\049\\083",0,0,0);
		addRDApvar(PRTDEF,"NORMAL TEXT","\\027\\038\\108\\054\\068\\027\\038\\107\\048\\083",1,10,12);
		addRDApvar(PRTDEF,"COMPRESSED TEXT","\\027\\038\\108\\056\\068\\027\\038\\107\\050\\083",1,17,9);
		addRDApvar(PRTDEF,"ELITE PRINT","\\027\\038\\108\\056\\068\\027\\038\\107\\052\\083",1,12,9);
		addRDApvar(PRTDEF,"PORTRAIT","\\027\\038\\108\\048\\079",0,0,0);
		addRDApvar(PRTDEF,"LANDSCAPE","\\027\\038\\108\\049\\079",0,0,0);
		addRDApvar(PRTDEF,"REVERSE PORTRAIT","\\027\\038\\108\\050\\079",0,0,0);
		addRDApvar(PRTDEF,"REVERSE LANDSCAPE","\\027\\038\\108\\051\\079",0,0,0);
		addRDApvar(PRTDEF,"START RASTER GRAPHICS LEFT MOST","\\027\\042\\114\\048\\065",0,0,0);
		addRDApvar(PRTDEF,"START RASTER GRAPHICS","\\027\\042\\114\\049\\065",0,0,0);
		addRDApvar(PRTDEF,"END RASTER GRAPHICS","\\027\\042\\114\\098\\067",0,0,0);
		addRDApvar(PRTDEF,"HALF LINE FEED","\\027\\061",0,0,0);
		addRDApvar(PRTDEF,"NEG Q LINE FEED","\\027\\038\\097\\045\\051\\048\\086",0,0,0);
		addRDApvar(PRTDEF,"POS Q LINE FEED","\\027\\038\\097\\043\\051\\048\\086",0,0,0);
		ADVaddRDApvar(PRTDEF,"FW2_01A.PCL","FW2_01A.PCL",0,0,0,2);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE LOCK","keylock.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE UNLOCK","keyunlock.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE FILE","./sig.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		addRDApvar(PRTDEF,"SIMPLEX","\\027\\038\\108\\048\\083",0,0,0);
		addRDApvar(PRTDEF,"DUPLEX LONG","\\027\\038\\108\\049\\083",0,0,0);
		addRDApvar(PRTDEF,"DUPLEX SHORT","\\027\\038\\108\\050\\083",0,0,0);
		addRDApvar(PRTDEF,"ESCAPE","\\027",0,0,0);
#ifndef WIN32
		ADVaddRDApvar(PRTDEF,"W2FORM-4PP","./forms/W2FORM-4PP.pcl",0,0,0,1);
#endif
#ifdef WIN32
		ADVaddRDApvar(PRTDEF,"W2FORM-4PP",".\\forms\\W2FORM-4PP.pcl",0,0,0,1);
#endif
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(9+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [OfficeJet], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
#ifndef WIN32



void SCRIPT_rdareportcd_lnx()
{
	FILE *fp=NULL;
	char *temp=NULL,*tmpdir=NULL;
	char *libx=NULL,*cdrw_bus=NULL,blankit=FALSE;
	RDAGenericSetup *gsv=NULL;
	short cdrw_speed=1;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("UTILITIES")+11);
#ifndef WIN32
	sprintf(libx,"%s/rda/%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\%s.GSV",CURRENTDIRECTORY,"UTILITIES");
#endif
	gsv=RDAGenericSetupNew("UTILITIES","TEMPORARY DIRECTORY");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			tmpdir=stralloc(gsv->value.string_value);
		} else {
#ifndef WIN32
			tmpdir=stralloc("/tmp");
#endif
#ifdef WIN32
			tmpdir=stralloc("C:\\TEMP");
#endif
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);

	gsv=RDAGenericSetupNew("UTILITIES","CDRW BUS");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			cdrw_bus=stralloc(gsv->value.string_value);
		} else {
			cdrw_bus=NULL;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("UTILITIES","CDRW SPEED");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			cdrw_speed=*gsv->value.short_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	gsv=RDAGenericSetupNew("UTILITIES","CDRW DEFAULT BLANK");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			blankit=*gsv->value.short_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) FreeRDAGenericSetup(gsv);
	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("rdareportcd.lnx")+15);
#ifdef WIN32
	sprintf(temp,"%s\\rdareportcd.lnx",CURRENTDIRECTORY);
#endif
#ifndef WIN32
	sprintf(temp,"%s/rdareportcd.lnx",CURRENTDIRECTORY);
#endif
	unlink(temp);
	fp=fopen(temp,"w+b");

	fprintf(fp,"#!/bin/sh\n");
	fprintf(fp,"##\n");
	fprintf(fp,"# Creates a temporary file from streaming input to burn to a cd.\n");
	fprintf(fp,"##\n");
	fprintf(fp,"cat >/tmp/$$.rdacd\n");
	fprintf(fp,"if [ $# -lt 6 ] && [ $# -gt 1 ]; then\n");
	fprintf(fp,"## These options can either be \"hard\" set or given at the output device window\n");
	fprintf(fp,"    CDRW_DEVICE=$2\n");
	fprintf(fp,"    BLANK_CDRW=$3\n");
	fprintf(fp,"    TMP_DIR=%s\n",tmpdir);
	fprintf(fp,"    DATA_DIR=%s  ## This needs to be hard set...and really should be!\n",CURRENTDIRECTORY);
	fprintf(fp,"else\n");
	fprintf(fp,"    CDRW_DEVICE=%s\n",(cdrw_bus!=NULL ? cdrw_bus:""));
	fprintf(fp,"    BLANK_CDRW=%s\n",(blankit==TRUE ? "TRUE":"FALSE"));
	fprintf(fp,"    TMP_DIR=%s\n",tmpdir);
	fprintf(fp,"    DATA_DIR=%s\n",CURRENTDIRECTORY);
	fprintf(fp,"fi\n");
	fprintf(fp,"## Initializing function.  Sets up environment/defaults.\n");
	fprintf(fp,"Init()\n");
	fprintf(fp,"{\n");
	fprintf(fp,"    if [ \"$REPORT_NAME\" == \"\" ]; then\n");
	fprintf(fp,"	REPORT_NAME=RDAREPORT$$.txt   ## Something generic...\n");
	fprintf(fp,"    fi\n");
	fprintf(fp,"    if [ \"$CDRW_DEVICE\" == \"\" ]; then\n");
	fprintf(fp,"	CDRW_DEVICE=0,0,0   ## Default bus if scsi emulation is enabled.\n");
	fprintf(fp,"    fi\n");
	fprintf(fp,"    case $BLANK_CDRW in\n");
	fprintf(fp,"	Y*|y*) BLANK_CDRW=TRUE;;\n");
	fprintf(fp,"	*) BLANK_CDRW=FALSE;;\n");
	fprintf(fp,"    esac\n");
	fprintf(fp,"    if [ ! -x /tmp/rdacddir ]; then\n");
	fprintf(fp,"	mkdir /tmp/rdacddir\n");
	fprintf(fp,"    else\n");
	fprintf(fp,"	rm -r /tmp/rdacddir\n");
	fprintf(fp,"	mkdir /tmp/rdacddir\n");
	fprintf(fp,"    fi\n");
	fprintf(fp,"    TMP_DIR=rdacddir\n");
	fprintf(fp,"}\n");
	fprintf(fp,"## Exiting function.  Script terminates here...everytime.\n");
	fprintf(fp,"Finished()\n");
	fprintf(fp,"{ \n");
	fprintf(fp,"    if [ -x /tmp/$TMP_DIR ]; then\n");
	fprintf(fp,"	rm -r /tmp/$TMP_DIR\n");
	fprintf(fp,"    fi\n");
	fprintf(fp,"    unset REPORT_NAME\n");
	fprintf(fp,"    unset CDRW_DEVICE\n");
	fprintf(fp,"    unset BLANK_CDRW\n");
	fprintf(fp,"    unset TMP_DIR\n");
	fprintf(fp,"    \n");
	fprintf(fp,"    exit 0\n");
	fprintf(fp,"}\n");
	fprintf(fp,"## Creates a \"running\" logfile and displays it using rdaout.\n");
	fprintf(fp,"DisplayResults()\n");
	fprintf(fp,"{\n");
	fprintf(fp,"    if [ ! -x $DATA_DIR/rptcdlog.log ]; then\n");
	fprintf(fp,"	echo \"                          REPORT CD RECORDING LOG \" >$DATA_DIR/rptcdlog.log\n");
	fprintf(fp,"	echo \"===================================================================\" >$DATA_DIR/rptcdlog.log\n");
	fprintf(fp,"    fi\n");
	fprintf(fp,"    echo \"Created Date:`date`\" >>$DATA_DIR/rptcdlog.log\n");
	fprintf(fp,"    echo \"Created By:  $LOGNAME\" >>$DATA_DIR/rptcdlog.log\n");
	fprintf(fp,"    echo \"File Name Used: $REPORT_NAME\" >>$DATA_DIR/rptcdlog.log\n");
	fprintf(fp,"    echo \"\" >>$DATA_DIR/rptcdlog.log\n");
	fprintf(fp,"    echo \"Diagnostic Output:\" >>$DATA_DIR/rptcdlog.log\n");
	fprintf(fp,"    if test -f /tmp/$TMP_DIR/cdrecord.log \n");
	fprintf(fp,"    then\n");
	fprintf(fp,"	cat /tmp/$TMP_DIR/cdrecord.log >>$DATA_DIR/rptcdlog.log\n");
	fprintf(fp,"    else\n");
	fprintf(fp,"	echo \"ERROR -- No log file was created.  The cd may not have been created successfully.\" >>$DATA_DIR/rptcdlog.log\n");
	fprintf(fp,"    fi\n");
	fprintf(fp,"    echo \"\" >>$DATA_DIR/rptcdlog.log\n");
	fprintf(fp,"    echo \"----------------------------------------------------------------------------\" >>$DATA_DIR/rptcdlog.log\n");
	fprintf(fp,"    \n");
	fprintf(fp,"    cd $DATA_DIR\n");
	fprintf(fp,"    rdaout.lnx rptcdlog.log\n");
	fprintf(fp,"}\n");
	fprintf(fp,"    \n");
	fprintf(fp,"## This is where the magic happens.  Pipes output of mkisofs to cdrecord.\n");
	fprintf(fp,"StartRecording()\n");
	fprintf(fp,"{\n");
	fprintf(fp,"    if [ $BLANK_CDRW == FALSE ]; then\n");
	fprintf(fp,"	mkisofs -R -J -v -l /tmp/$TMP_DIR | cdrecord -v -eject dev=$CDRW_DEVICE - | tee /tmp/$TMP_DIR/cdrecord.log\n");
	fprintf(fp,"    else\n");
	fprintf(fp,"	mkisofs -R -J -v -l /tmp/$TMP_DIR | cdrecord -v -eject blank=all dev=$CDRW_DEVICE - | tee /tmp/$TMP_DIR/cdrecord.log\n");
	fprintf(fp,"    fi\n");
	fprintf(fp,"}\n");
	fprintf(fp,"#cat $4 >/tmp/$$.rdacd\n");
	fprintf(fp,"Init\n");
	fprintf(fp,"mv /tmp/$$.rdacd /tmp/$TMP_DIR/$REPORT_NAME\n");
	fprintf(fp,"StartRecording\n");
	fprintf(fp,"DisplayResults\n");
	fprintf(fp,"Finished\n");
	fclose(fp);
#ifndef WIN32
	chmod(temp,(mode_t)MODE_RWXRWX___);
#endif
	if(temp!=NULL) Rfree(temp);
	if(cdrw_bus!=NULL) Rfree(cdrw_bus);
	if(tmpdir!=NULL) Rfree(tmpdir);
}
static void SCRIPT_sendfile_lnx()
{
	FILE *fp=NULL;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("sendfile.lnx")+15);
#ifdef WIN32
	sprintf(temp,"%s\\sendfile.bat",CURRENTDIRECTORY);
#endif
#ifndef WIN32
	sprintf(temp,"%s/sendfile.lnx",CURRENTDIRECTORY);
#endif
	unlink(temp);
	fp=fopen(temp,"w+b");
	fprintf(fp,"#!/bin/sh\n");
	fprintf(fp,"#Usage: \n");
	fprintf(fp,"#   $1=From Name (Steven J. Saunders)\n");
	fprintf(fp,"#   $2=From email address (sjsaunders@rdasys.com)\n");
	fprintf(fp,"#   $3=Subject \n");
	fprintf(fp,"#   $4=filename\n");
	fprintf(fp,"#   $5=email to list (support@rdasys.com)\n");
	fprintf(fp,"\n");
	fprintf(fp,"export VMIME_FROM_NAME=\"$1\"\n");
	fprintf(fp,"export VMIME_FROM_ADDR=\"$2\"\n");
	fprintf(fp,"export VMIME_TO=\"$5\"\n");
	fprintf(fp,"export VMIME_SUBJECT=\"$3\"\n");
	fprintf(fp,"export BODY=`cat \"$4\"`\n");
	fprintf(fp,"vmime-sendmail.lnx\n");

	fclose(fp);
#ifndef WIN32
	chmod(temp,(mode_t)MODE_RWXRWX___);
#endif
	if(temp!=NULL) Rfree(temp);
}
void SCRIPT_outputemail_lnx()
{
	FILE *fp=NULL;
	char *temp=NULL;

	SCRIPT_sendfile_lnx();
	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("outputemail.lnx")+45);
#ifdef WIN32
	sprintf(temp,"%s\\outputemail.lnx",CURRENTDIRECTORY);
#endif
#ifndef WIN32
	sprintf(temp,"%s/outputemail.lnx",CURRENTDIRECTORY);
#endif
	unlink(temp);
	fp=fopen(temp,"w+b");
	fprintf(fp,"#!/bin/bash\n");
	fprintf(fp,"echo \"outputemail.lnx\" \n");
	fprintf(fp,"#Usage: \n");
	fprintf(fp,"#   $1=email to list (support@rdasys.com or someclient@somewebsite.gov)\n");
	fprintf(fp,"#   $2=Subject \n");
	fprintf(fp,"#   Piped Input will be body of message\n");
	fprintf(fp,"# ARRAY[0]=string\n");
	fprintf(fp,"# ${#STRING[1]}\n");
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	fprintf(fp,"export VMIME_FROM_ADDR=\"`grep from ~/.msmtprc | cut -d' ' -f2`\"\n");
	fprintf(fp,"export VMIME_TO=\"$1\"\n");
	fprintf(fp,"export VMIME_SUBJECT=\"$2\"\n");
	fprintf(fp,"export VMIME_BODY=\"stdin\"\n");
	fprintf(fp,"vmime-sendmail.lnx\n");

	fclose(fp);
#ifndef WIN32
	chmod(temp,(mode_t)MODE_RWXRWX___);
#endif
	if(temp!=NULL) Rfree(temp);
}
void SCRIPT_sendmail_lnx()
{
	FILE *fp=NULL;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("sendmail.lnx")+45);
#ifdef WIN32
	sprintf(temp,"%s\\sendmail.lnx",CURRENTDIRECTORY);
#endif
#ifndef WIN32
	sprintf(temp,"%s/sendmail.lnx",CURRENTDIRECTORY);
#endif
	unlink(temp);
	fp=fopen(temp,"w+b");
	fprintf(fp,"#!/bin/sh\n");
	fprintf(fp,"#Usage: \n");
	fprintf(fp,"#   $1=From Name (Steven J. Saunders)\n");
	fprintf(fp,"#   $2=From email address (sjsaunders@rdasys.com)\n");
	fprintf(fp,"#   $3=Subject \n");
	fprintf(fp,"#   $4=Body\n");
	fprintf(fp,"#   $5=email to list (support@rdasys.com)\n");
	fprintf(fp,"\n");
	fprintf(fp,"export VMIME_FROM_NAME=\"$1\"\n");
	fprintf(fp,"export VMIME_FROM_ADDR=\"$2\"\n");
	fprintf(fp,"export VMIME_TO=\"$5\"\n");
	fprintf(fp,"export VMIME_SUBJECT=\"$3\"\n");
	fprintf(fp,"export VMIME_BODY=\"$4\"\n");
	fprintf(fp,"vmime-sendmail.lnx\n");
	fclose(fp);
#ifndef WIN32
	chmod(temp,(mode_t)MODE_RWXRWX___);
#endif
	if(temp!=NULL) Rfree(temp);
}
void PRT_outputemail_lnx()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("outputemail.lnx");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		ADVaddRDApvar(PRTDEF,"NORMAL TEXT","",1,10,12,1);
		ADVaddRDApvar(PRTDEF,"COMPRESSED TEXT","",1,17,9,1);
		ADVaddRDApvar(PRTDEF,"PUSH CURSOR","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POP CURSOR","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"ELITE PRINT","",256,12,9,1);
		ADVaddRDApvar(PRTDEF,"PORTRAIT","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"LANDSCAPE","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"REVERSE PORTRAIT","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"REVERSE LANDSCAPE","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"HALF LINE FEED","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE LOCK","keylock.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE UNLOCK","keyunlock.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE FILE","./sig.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIMPLEX","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"DUPLEX LONG","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"DUPLEX SHORT","",0,0,0,1);
		PRTDEF->active=TRUE;
		PRTDEF->page_length=0;
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [outputemail.lnx], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void PRT_rdareportcd_lnx()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("rdareportcd.lnx");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		ADVaddRDApvar(PRTDEF,"NORMAL TEXT","",1,10,12,1);
		ADVaddRDApvar(PRTDEF,"COMPRESSED TEXT","",1,17,9,1);
		ADVaddRDApvar(PRTDEF,"PUSH CURSOR","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POP CURSOR","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"ELITE PRINT","",256,12,9,1);
		ADVaddRDApvar(PRTDEF,"PORTRAIT","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"LANDSCAPE","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"REVERSE PORTRAIT","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"REVERSE LANDSCAPE","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"HALF LINE FEED","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE LOCK","keylock.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE UNLOCK","keyunlock.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE FILE","./sig.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIMPLEX","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"DUPLEX LONG","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"DUPLEX SHORT","",0,0,0,1);
		PRTDEF->active=TRUE;
		PRTDEF->page_length=0;
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [rdareportcd.lnx], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
void SCRIPT_sendattach_2_lnx()
{
	FILE *fp=NULL;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("sendattach-2.lnx")+15);
#ifdef WIN32
	sprintf(temp,"%s\\sendattach-2.lnx",CURRENTDIRECTORY);
#endif
#ifndef WIN32
	sprintf(temp,"%s/sendattach-2.lnx",CURRENTDIRECTORY);
#endif
	unlink(temp);
	fp=fopen(temp,"w+b");
	fprintf(fp,"#!/bin/sh\n");
	fprintf(fp,"#Usage: \n");
	fprintf(fp,"#   $1=From Name (Steven J. Saunders)\n");
	fprintf(fp,"#   $2=From email address (sjsaunders@rdasys.com)\n");
	fprintf(fp,"#   $3=Subject \n");
	fprintf(fp,"#   $4=Body\n");
	fprintf(fp,"#   $5=email to list (support@rdasys.com)\n");
	fprintf(fp,"#   $6=Report Name for attachment (TRIAL_BAL.TXT)\n");
	fprintf(fp,"\n");
	fprintf(fp,"if [ \"$#\" == 6 ]\n");
	fprintf(fp,"then\n");
	fprintf(fp,"	export VMIME_FROM_NAME=\"$1\"\n");
	fprintf(fp,"	export VMIME_FROM_ADDR=\"$2\"\n");
	fprintf(fp,"	export VMIME_SUBJECT=\"$3\"\n");
	fprintf(fp,"	export VMIME_BODY=\"$4\"\n");
	fprintf(fp,"	export VMIME_TO=\"$5\"\n");
	fprintf(fp,"	export VMIME_FNAME1=\"stdin\"\n");
	fprintf(fp,"	export VMIME_DNAME1=\"$6\"\n");
	fprintf(fp,"elif [ \"$#\" == 2 ]\n");
	fprintf(fp,"then\n");
	fprintf(fp,"	export VMIME_TO=\"$1\"\n");
	fprintf(fp,"	export VMIME_FROM_ADDR=\"`grep from ~/.msmtprc | cut -d' ' -f2`\"\n");
	fprintf(fp,"	export VMIME_SUBJECT=\"$2\"\n");
	fprintf(fp,"	export VMIME_FNAME1=\"stdin\"\n");
	fprintf(fp,"elif [ \"$#\" == 1 -a `echo \"$1\" | grep -c '@'` -gt 0 ]\n");
	fprintf(fp,"then\n");
	fprintf(fp,"	export VMIME_TO=\"$1\"\n");
	fprintf(fp,"	export VMIME_FROM_ADDR=\"`grep from ~/.msmtprc | cut -d' ' -f2`\"\n");
	fprintf(fp,"	export VMIME_SUBJECT=\"\"\n");
	fprintf(fp,"	export VMIME_FNAME1=\"stdin\"\n");
	fprintf(fp,"elif [ \"$#\" == 3 -a $3 == zip ]\n");
	fprintf(fp,"then\n");
	fprintf(fp,"	export VMIME_TO=\"$1\"\n");
	fprintf(fp,"	export VMIME_FROM_ADDR=\"`grep from ~/.msmtprc | cut -d' ' -f2`\"\n");
	fprintf(fp,"	export VMIME_SUBJECT=\"$2\"\n");
	fprintf(fp,"	export VMIME_FNAME1=\"stdin\"\n");
	fprintf(fp,"fi\n");
	fprintf(fp,"vmime-sendmail.lnx\n");
	
	fclose(fp);
#ifndef WIN32
	chmod(temp,(mode_t)MODE_RWXRWX___);
#endif
	if(temp!=NULL) Rfree(temp);
}
void PRT_sendattach2lnx()
{
	RDApdev *PRTDEF=NULL;
	char *temp1=NULL;
	char *temp2=NULL;

	PRTDEF=RDApdevNEW("sendattach-2.lnx");
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		ADVaddRDApvar(PRTDEF,"NORMAL TEXT","",1,10,12,1);
		ADVaddRDApvar(PRTDEF,"COMPRESSED TEXT","",1,17,9,1);
		ADVaddRDApvar(PRTDEF,"PUSH CURSOR","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POP CURSOR","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"ELITE PRINT","",256,12,9,1);
		ADVaddRDApvar(PRTDEF,"PORTRAIT","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"LANDSCAPE","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"REVERSE PORTRAIT","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"REVERSE LANDSCAPE","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"HALF LINE FEED","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE LOCK","keylock.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE UNLOCK","keyunlock.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE FILE","./sig.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIMPLEX","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"DUPLEX LONG","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"DUPLEX SHORT","",0,0,0,1);
		PRTDEF->active=TRUE;
		PRTDEF->page_length=0;
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(17+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [sendattach-2.lnx], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
#endif /* Not WIN32 */
#define PTESTS	6
void TestOutputDevices()
{
	char *libx=NULL,test[PTESTS];
	RDA_PFILE *fp=NULL;
	RDApdev *tmp=NULL;
	RDApvar *pvar=NULL;
	APPlib *list=NULL;
	int x=0,y=0,epc=0;

	memset(stemp,0,101);
#ifdef WIN32
	sprintf(stemp,"rdaout.exe");
#endif
#ifndef WIN32
	sprintf(stemp,"rdaout.lnx");
#endif
	if((fp=RDA_popen(stemp,"w"))==NULL)
	{
		prterr("Error: Cannot open output device rdaout for reporting.");
		return;
	}
	memset(stemp,0,101);
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+16);
#ifndef WIN32
	sprintf(libx,"%s/rda/PRTDEV.STP",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\PRTDEV.STP",CURRENTDIRECTORY);
#endif
	list=getlibnames(libx,FALSE);
	if(libx!=NULL) Rfree(libx);
	if(list!=NULL)
	{
		for(x=0;x<list->numlibs;++x)
		{
			tmp=RDApdevNEW(list->libs[x]);
			getpdevbin(tmp);
			if(tmp->active==TRUE)
			{
				if(tmp->line_58==TRUE)
				{
					RDA_fprintf(fp,"Warning: Since %s is 58 Line Printer so must rdaout.\r\n",tmp->name);
				}
				if(tmp->page_length!=10 && tmp->page_length!=0)
				{
					RDA_fprintf(fp,"Warning: Since %s has page length!=10 so must rdaout.\r\n",tmp->name);
				}
				memset(test,1,PTESTS);
				if(tmp->pvar!=NULL)
				{
					for(y=0,pvar=tmp->pvar;y<tmp->num;++y,++pvar)
					{
						if(!RDAstrcmp(pvar->name,"NORMAL TEXT"))
						{
							if(pvar->points!=12)
							{
								RDA_fprintf(fp,"Printer: %s has NORMAL TEXT points [%d] should be 12.\r\n",tmp->name,pvar->points);		
								
							} else test[0]=FALSE;
							if(pvar->pitch!=10)
							{
								RDA_fprintf(fp,"Printer: %s has NORMAL TEXT pitch [%d] should be 10.\r\n",tmp->name,pvar->points);		
							} else test[1]=FALSE;
						}
						if(!RDAstrcmp(pvar->name,"COMPRESSED TEXT"))
						{
							if(pvar->points!=9)
							{
								RDA_fprintf(fp,"Printer: %s has COMPRESSED TEXT points [%d] should be 9.\r\n",tmp->name,pvar->points);		
								
							} else test[2]=FALSE;
							if(pvar->pitch!=17)
							{
								RDA_fprintf(fp,"Printer: %s has NORMAL TEXT pitch [%d] should be 17.\r\n",tmp->name,pvar->points);		
							} else test[3]=FALSE;
						}
						if((!RDAstrcmp(pvar->name,"VENTREAS")) && (!RDAstrcmp(pvar->command,"vencrt.txt")))
						{
							test[4]=FALSE;
						}
						if(!RDAstrcmp(pvar->name,"HALF LINE FEED"))
						{
							test[5]=FALSE;
						}
					}
				}
				epc=0;
				for(y=0;y<PTESTS;++y)
				{
					if(test[y]==TRUE)
					{
						++epc;
					}
				}
				if(epc==0)
				{
					RDA_fprintf(fp,"Printer: %s is ready!\r\n",tmp->name);		
				} else {
					RDA_fprintf(fp,"Printer: %s has %d errors.\r\n",tmp->name,epc);		
					for(y=0;y<PTESTS;++y)
					{
						if(test[y]==TRUE)
						{
							switch(y)
							{
								case 0: /* Normal points */
									RDA_fprintf(fp,"\tNORMAL TEXT Points 12.\r\n");
									break;
								case 1: /* Normal pitch */
									RDA_fprintf(fp,"\tNORMAL TEXT Pitch 10.\r\n");
									break;
								case 2: /* Compressed points */
									RDA_fprintf(fp,"\tCOMPRESSED TEXT Points 9.\r\n");
									break;
								case 3: /* Compressed pitch */
									RDA_fprintf(fp,"\tCOMPRESSED TEXT Pitch 17.\r\n");
									break;
								case 4: /* VENTREAS */
									RDA_fprintf(fp,"\tVENTREAS Derefenced File vencrt.txt.\r\n");
									break;
								case 5: /* HALF LINE FEED */
									RDA_fprintf(fp,"\tHALF LINE FEED is not available.\r\n");
									break;
							}
						}
					}
					RDA_fprintf(fp,"\r\n");
				}
			}
			if(tmp!=NULL) Rfree(tmp);
		}
	}
	RDA_pclose(fp);
}
void PRT_rdaout()
{
	RDApdev *PRTDEF=NULL,*origdef=NULL;
	char *temp1=NULL;
	char *temp2=NULL,extraff=FALSE,l58=FALSE;
	double page_length=10;

#ifndef WIN32
	origdef=RDApdevNEW("rdaout.lnx");
#endif
#ifdef WIN32
	origdef=RDApdevNEW("rdaout.exe");
#endif
	if(!getpdevbin(origdef))
	{
		extraff=origdef->formfeed;
		l58=origdef->line_58;
		page_length=origdef->page_length;
	}
	free_pdev(origdef);
#ifndef WIN32
	PRTDEF=RDApdevNEW("rdaout.lnx");
#endif
#ifdef WIN32
	PRTDEF=RDApdevNEW("rdaout.exe");
#endif
	if(temp1!=NULL) Rfree(temp1);
	if(temp2!=NULL) Rfree(temp2);
	if(PRTDEF!=NULL)
	{
		ADVaddRDApvar(PRTDEF,"NORMAL TEXT","",1,10,12,1);
		ADVaddRDApvar(PRTDEF,"COMPRESSED TEXT","",1,17,9,1);
		ADVaddRDApvar(PRTDEF,"PUSH CURSOR","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POP CURSOR","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"ELITE PRINT","",256,12,9,1);
		ADVaddRDApvar(PRTDEF,"PORTRAIT","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"LANDSCAPE","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"REVERSE PORTRAIT","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"REVERSE LANDSCAPE","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"HALF LINE FEED","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"NEG Q LINE FEED","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POS Q LINE FEED","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENTREAS","vencrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYREG","payreg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POCERT","pocrt.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"PAYCKRG","payckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"VENCKRG","venckrg.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE LOCK","keylock.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE UNLOCK","keyunlock.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"POAUDIT","poaudit.txt",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIGNATURE FILE","./sig.dat",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"SIMPLEX","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"DUPLEX LONG","",0,0,0,1);
		ADVaddRDApvar(PRTDEF,"DUPLEX SHORT","",0,0,0,1);
		PRTDEF->active=TRUE;
		if(page_length<10.0) 
		{
			PRTDEF->page_length=page_length;
		} else PRTDEF->page_length=10;
		PRTDEF->formfeed=extraff;
		PRTDEF->line_58=l58;
		if(writepdevbin(PRTDEF))
		{
			if(temp1!=NULL) Rfree(temp1);
			temp1=Rmalloc(15+110+1);
			sprintf(temp1,"PRINTER DEFINITION WRITE ERROR: Definition [rdareportcd.lnx], Can Not Save Printer Definition!");
			prterr(temp1);
			if(errorlist!=NULL)
			{
				addERRlist(&errorlist,temp1);
			}
			if(temp1!=NULL) Rfree(temp1);
			if(temp2!=NULL) Rfree(temp2);
		}
		if(PRTDEF!=NULL) free_pdev(PRTDEF);
	}
}
