#include <mix.hpp>

struct Route_Statistics
{
	int route;
	char *name;
	char *routestring;
	char *survey1;
	char *survey2;
	char *message;
	char mobile_amr;
	int num_readings;
	int num_demand;
	int num_keyed;
	int num_probed;
	int num_radio;
	int num_wand;
	int num_customers;
	int num_meters;
	int num_gas;
	int num_water;
	int num_electric;
	int num_location;
	int num_extra;
	char *time_allowed;
};
typedef struct Route_Statistics Route_Statistic;

extern char *module,*ITRON_FILENAME,*CycleDate;
extern DBsort *sortfile;
extern short UseRadio,UseWand,UseProbe,utbrte,NumberKeys;
extern RDArsrc *MAINrsrc;

FILE *outfile=NULL;
char ItronLine[127];
short filenum=(-1);

static void WriteItronLine()
{
	fprintf(outfile,"%s\r\n",ItronLine);
}
static void ZeroRouteStatistics(Route_Statistic *CR)
{
	CR->route=(-1);
	if(CR->name!=NULL) Rfree(CR->name);
	if(CR->routestring!=NULL) Rfree(CR->routestring);
	if(CR->survey1!=NULL) Rfree(CR->survey1);
	if(CR->survey2!=NULL) Rfree(CR->survey2);
	if(CR->message!=NULL) Rfree(CR->message);
	if(CR->time_allowed!=NULL) Rfree(CR->time_allowed);
	CR->mobile_amr=FALSE;
	CR->num_readings=0;
	CR->num_demand=0;
	CR->num_keyed=0;
	CR->num_probed=0;
	CR->num_radio=0;
	CR->num_wand=0;
	CR->num_customers=0;
	CR->num_meters=0;
	CR->num_gas=0;
	CR->num_water=0;
	CR->num_electric=0;
	CR->num_location=0;
	CR->num_extra=0;
}
void GatherRouteStatistics(Route_Statistic *CR,int NewRoute)
{
	int route=0,svc=0,last_svc=0;
	short ef=(-1),ctype=0,rtype=0;
	char *custid=NULL,*last_custid=NULL,*temp=NULL;

	ZeroRouteStatistics(CR);
	CR->route=NewRoute;
	ZERNRD(utbrte);
	FINDFLDSETINT(utbrte,"ROUTE NUMBER",NewRoute);
	if(ADVEQLNRDsec(utbrte,1,SCRNvirtualSubData,MAINrsrc)) KEYNRD(utbrte,1);
	FINDFLDGETSTRING(utbrte,"ROUTE NAME",&CR->name);
	FINDFLDGETSTRING(utbrte,"TIME ALLOWED",&temp);
	CR->routestring=Rmalloc(9);
	sprintf(CR->routestring,"01%06d",NewRoute);
	CR->time_allowed=Rmalloc(11);
	if(isEMPTY(temp))
	{
		if(temp!=NULL) Rfree(temp);
		temp=Rmalloc(11);
		sprintf(temp,"08:00:00");
	}
	sprintf(CR->time_allowed,"%.2s%.2s%.2s",&temp[0],&temp[3],&temp[7]);
	if(temp!=NULL) Rfree(temp);

	FINDFLDGETSTRING(utbrte,"SURVEY TEXT 1",&CR->survey1);
	FINDFLDGETSTRING(utbrte,"SURVEY TEXT 2",&CR->survey2);
	if(isEMPTY(CR->survey1))
	{
		if(CR->survey1!=NULL) Rfree(CR->survey1);
	}	
	if(isEMPTY(CR->survey2))
	{
		if(CR->survey2!=NULL) Rfree(CR->survey2);
	}
	FINDFLDGETSTRING(utbrte,"ROUTE MESSAGE",&CR->message);
	if(isEMPTY(CR->message))
	{
		if(CR->message!=NULL) Rfree(CR->message);
	}
	FINDFLDGETCHAR(utbrte,"MOBILE AMR",&CR->mobile_amr);
	if(filenum!=(-1))
	{
		ZERNRD(filenum);
		FINDFLDSETINT(filenum,"ROUTE NUMBER",NewRoute);
		ef=GTENRD(filenum,1);
		while(!ef)
		{
			FINDFLDGETINT(filenum,"ROUTE NUMBER",&route);
			if(route!=NewRoute) break;
			FINDFLDGETSTRING(filenum,"CUSTOMER IDENTIFICATION",&custid);
			FINDFLDGETINT(filenum,"SERVICE NUMBER",&svc);
			if(RDAstrcmp(custid,last_custid) || svc!=last_svc)
			{
				++CR->num_customers;
				if(last_custid!=NULL) Rfree(last_custid);
				last_custid=stralloc(custid);
				last_svc=svc;
			}
			++CR->num_readings;
			++CR->num_meters;
			FINDFLDGETSHORT(filenum,"COLLECTION TYPE",&ctype);
			switch(ctype)
			{
				default:
				case 0:
					++CR->num_keyed;
					break;
				case 1:
					++CR->num_probed;
					break;
				case 2:
					++CR->num_wand;
					break;
				case 3:
					++CR->num_radio;
					break;
			}
			FINDFLDGETSHORT(filenum,"READER TYPE",&rtype);
			switch(rtype)
			{
				default:
				case 0:
					++CR->num_water;
					break;
				case 1:
					++CR->num_electric;
					break;
				case 2:
					++CR->num_demand;
					break;
				case 3:
					++CR->num_gas;
					break;
				case 4:
					++CR->num_location;
					break;
				case 5:
					++CR->num_extra;
					break;
			}
			ef=NXTNRD(filenum,1);
		}
	}
}

int CountRoutes()
{
	int count=0,route=(-1),last_route=(-1);
	short ef=FALSE;

	ZERNRD(sortfile->fileno);
	ef=FRSNRD(sortfile->fileno,1);
	while(!ef)
	{
		FINDFLDGETINT(sortfile->fileno,"ROUTE NUMBER",&route);
		if(route!=last_route)
		{
			++count;
			last_route=route;
		}
		ef=NXTNRD(sortfile->fileno,1);
	}
	return(count);
}
static double FixReading(double r,int dec_pos)
{
	char *temp=NULL,*temp1=NULL,*temp2=NULL;
	double rv=0l;

	temp=Rmalloc(35);
	sprintf(temp,"%.*f",dec_pos,r);
	if(temp!=NULL)
	{
		memset(stemp,0,101);
		temp2=stemp;
		for(temp1=temp;*temp1;++temp1)
		{
			if(isdigit(*temp1))
			{
				*temp2=*temp1;
				++temp2;
			}
		}
		rv=round(atof(stemp));
		return(rv);
	}
	return(0);
}
static void DoCustomer(Route_Statistic *CR,char *custid,int svc)
{
	int NumMeters=0,route=0,route1=0,serviceno=0;
	short ef=0;
	char *custid1=NULL,*custname=NULL,*addr1=NULL,*resid=NULL;

	ZERNRD(filenum);
	COPYFIELD(sortfile->fileno,filenum,"ROUTE NUMBER");
	COPYFIELD(sortfile->fileno,filenum,"SEQUENCE NUMBER");
	COPYFIELD(sortfile->fileno,filenum,"CUSTOMER IDENTIFICATION");
	FINDFLDGETINT(filenum,"ROUTE NUMBER",&route);
	COPYFIELD(sortfile->fileno,filenum,"SERVICE NUMBER");
	ef=GTENRD(filenum,1);
	while(!ef)
	{
		FINDFLDGETINT(filenum,"ROUTE NUMBER",&route1);
		FINDFLDGETSTRING(filenum,"CUSTOMER IDENTIFICATION",&custid1);
		FINDFLDGETINT(filenum,"SERVICE NUMBER",&serviceno);
		if(route!=route1 || RDAstrcmp(custid,custid1) || serviceno!=svc) break;
		++NumMeters;
		ef=NXTNRD(filenum,1);
	}
	if(custid1!=NULL) Rfree(custid1);
	FINDFLDGETSTRING(sortfile->fileno,"CUSTOMER NAME",&custname);
	FINDFLDGETSTRING(sortfile->fileno,"ADDRESS 1",&addr1);
	FINDFLDGETSTRING(sortfile->fileno,"NAME OF RESIDENT",&resid);
	FINDFLDGETINT(sortfile->fileno,"SERVICE NUMBER",&serviceno);
	if(RDAstrlen(custname)>20) custname[20]=0;
	memset(ItronLine,0,127);
	if(RDAstrlen(resid)>20) resid[20]=0;
	sprintf(ItronLine,"CUS%8s%03d%-15s %04d%-20s%-40s  0%-20sN        ",
		CR->routestring,NumMeters,custid,serviceno,custname,addr1,resid);
	if(custname!=NULL) Rfree(custname);
	if(addr1!=NULL) Rfree(addr1);
	if(resid!=NULL) Rfree(resid);
	WriteItronLine();	
}
static void DoMeter(Route_Statistic *CR)
{
	short btype=0,rtype=0,readtime=0;
	char readtype=0,*probeid=NULL,active=FALSE;
	char *serialnumber=NULL,*location=NULL,mc=0,le=' ';
	double seq=0;

	FINDFLDGETSHORT(sortfile->fileno,"BILLING TYPE",&btype);
	FINDFLDGETSHORT(sortfile->fileno,"READING TYPE",&rtype);
	FINDFLDGETSTRING(sortfile->fileno,"PROBE ID",&probeid);
	FINDFLDGETSTRING(sortfile->fileno,"SERIAL NUMBER",&serialnumber);
	FINDFLDGETDOUBLE(sortfile->fileno,"SEQUENCE NUMBER",&seq);
	FINDFLDGETSTRING(sortfile->fileno,"LOCATION",&location);
	FINDFLDGETSHORT(sortfile->fileno,"READ TIME",&readtime);
	FINDFLDGETCHAR(sortfile->fileno,"ACTIVE",&active);
	switch(btype)
	{
		default:
		case 0: /* N/A */
			readtype='R';
			break;
		case 1:
			readtype='R';
			break;
		case 2:
			readtype='C';
			break;
		case 3:
			readtype='I';
			break;
	}
	switch(rtype)
	{
		default:
		case 0: /* water */
			mc='W';
			break;
		case 1: /* electric */
		case 2: /* demand */
			mc='E';
			break;
		case 3: /* gas */
			mc='G';
			break;
	}
	memset(ItronLine,0,127);
	sprintf(ItronLine,"MTR%8s%03d  00       0  0%c%c%-14s%-12s  00%8.0f%-20s 00 00 000 %c%c%c%03d%20s",
		CR->routestring,(rtype==2 ? 2:1),readtype,(active ? 'A':'I'),
		(probeid!=NULL ? probeid:""),
		(serialnumber!=NULL ? serialnumber:""),seq,
		(location!=NULL ? location:""),
		'N',mc,le,readtime,"");
	if(probeid!=NULL) Rfree(probeid);
	if(serialnumber!=NULL) Rfree(serialnumber);
	if(location!=NULL) Rfree(location);
	WriteItronLine();	
}
static void DoReading(Route_Statistic *CR)
{
	short rtype=0,prmpt=0,rd=0,ccode=0,valid=0,vc=0;
	short dials=0,dp=0,ct=0;
	char *readid=NULL,pcode=0,rm=0;
	int high2=0,low2=0,pcreep=0,ncreep=0;
	double high1=0,low1=0;
	int mltplr=0;
	long demand=0,aveuse1=0,stddev1=0,prev_reading=0;
	double stddev=0.0,aveuse=0.0,preading=0.0,demandd=0;

	FINDFLDGETSHORT(sortfile->fileno,"READING TYPE",&rtype);
	FINDFLDGETSTRING(sortfile->fileno,"READING ID",&readid);
	FINDFLDGETSHORT(sortfile->fileno,"PROMPT CODE",&prmpt);
	FINDFLDGETSHORT(sortfile->fileno,"READ DIRECTION",&rd);
	FINDFLDGETSHORT(sortfile->fileno,"COMPARE CODE",&ccode);
	FINDFLDGETSHORT(sortfile->fileno,"DIALS",&dials);
	FINDFLDGETSHORT(sortfile->fileno,"DECIMAL POSITIONS",&dp);
	FINDFLDGETINT(sortfile->fileno,"HIGH2 PERCENTAGE",&high2);
	FINDFLDGETINT(sortfile->fileno,"LOW2 PERCENTAGE",&low2);
	FINDFLDGETINT(sortfile->fileno,"POSITIVE DIAL CR->EP",&pcreep);
	FINDFLDGETINT(sortfile->fileno,"NEGATIVE DIAL CR->EP",&ncreep);
	FINDFLDGETDOUBLE(sortfile->fileno,"STANDARD DEVIATION",&stddev);
	FINDFLDGETDOUBLE(sortfile->fileno,"AVERAGE READING",&aveuse);
	FINDFLDGETDOUBLE(sortfile->fileno,"PREVIOUS READING",&preading);
	FINDFLDGETDOUBLE(sortfile->fileno,"DEMAND READING",&demandd);
	FINDFLDGETINT(sortfile->fileno,"MULTIPLIER",&mltplr);
	FINDFLDGETSHORT(sortfile->fileno,"VALIDATION CODE",&valid);
	switch(valid)
	{
		default:
			vc=valid;
			break;
		case 8:
			vc=99;
			break;
	}
	prev_reading=FixReading(preading,dp);
	demand=FixReading(demandd,dp);
	aveuse1=FixReading(aveuse,dp);
	stddev1=FixReading(stddev,dp);
	switch(prmpt)
	{
		default:
		case 0:
			pcode='Y';
			break;
		case 1:
			pcode='N';
			break;
		case 2:
			pcode='M';
			break;
		case 3:
			pcode='F';
			break;
		case 4:
			pcode='P';
			break;
	}
	FINDFLDGETSHORT(sortfile->fileno,"COLLECTION TYPE",&ct);
	switch(ct)
	{
		default:
		case 0:
			rm='K';
			break;
		case 1:
			rm='P';
			break;
		case 2:
			rm='W';
			break;
		case 3:
			rm='R';
			break;
	}
	memset(ItronLine,0,127);
	high1=round((prev_reading+aveuse1+stddev1));
	low1=round((prev_reading+aveuse1-stddev1));
	if(high1<0) high1=0;
	if(low1<0) low1=0;
	if(RDAstrlen(readid)>4) readid[4]=0;
	sprintf(ItronLine,"RDG%-8s%-4s%c%c%03d%03d   %02d%02d%c%010d%010.0f%010.0f%06ld01%05ld0 01%06ld%06ld%05ld%31s",
		CR->routestring,(readid!=NULL ? readid:""),pcode,
		(rd==0 ? 'L':'R'),ccode,vc,dials,dp,rm,prev_reading,
		high1,low1,mltplr,pcreep,high2,low2,ncreep,"");
	WriteItronLine();	
	if(rtype==2) /* Demand Meter */
	{
		sprintf(ItronLine,"RDG%-8s%-4s%c%c%03d%03d   %02d%02d%c%010d%010.0f%010.0f%06d01%05d0 01%06d%06d%05d%31s",
			CR->routestring,"DMD ",pcode,
			(rd==0 ? 'L':'R'),ccode,vc,dials,dp,rm,prev_reading,
			high1,low1,mltplr,pcreep,high2,low2,ncreep,"");
		WriteItronLine();	
	}
	if(readid!=NULL) Rfree(readid);
}
static void DoProbe(Route_Statistic *CR)
{
	char *deviceid=NULL,*timname=NULL,*probeid=NULL,*caddr=NULL;
	char *mtrcfg=NULL,*pword=NULL,dchain=FALSE,dst=FALSE,vr=FALSE;
	char rr=FALSE,rd=FALSE,dstu=FALSE,sc=FALSE,ef=FALSE,*et=NULL;
	int tza=0;

	FINDFLDGETSTRING(sortfile->fileno,"DEVICE ID",&deviceid);
	FINDFLDGETSTRING(sortfile->fileno,"WAND PROGRAM",&timname);
	FINDFLDGETSTRING(sortfile->fileno,"PROBE ID",&probeid);
	FINDFLDGETSTRING(sortfile->fileno,"METER CONFIG ID",&mtrcfg);
	FINDFLDGETSTRING(sortfile->fileno,"DEVICE PASSWORD",&pword);
	FINDFLDGETCHAR(sortfile->fileno,"DAISY CHAINED",&dchain);
	FINDFLDGETCHAR(sortfile->fileno,"DST INDICATOR",&dst);
	FINDFLDGETSTRING(sortfile->fileno,"COMMUNICATION ADDRESS",&caddr);
	FINDFLDGETINT(sortfile->fileno,"TIME ZONE ADJUST",&tza);
	FINDFLDGETCHAR(sortfile->fileno,"VISUAL READ",&vr);
	FINDFLDGETCHAR(sortfile->fileno,"REGISTER READ INDICATOR",&rr);
	FINDFLDGETCHAR(sortfile->fileno,"RESET DEMAND INDICATOR",&rd);
	FINDFLDGETCHAR(sortfile->fileno,"DST UPDATE INDICATOR",&dstu);
	FINDFLDGETCHAR(sortfile->fileno,"STATUS CHECK INDICATOR",&sc);
	FINDFLDGETCHAR(sortfile->fileno,"ENCODER",&ef);
	FINDFLDGETSTRING(sortfile->fileno,"ENCODER TYPE",&et);
	memset(ItronLine,0,127);
	sprintf(ItronLine,"PRB%-8s%-14s%-20s%-8s%-4s%-20sY%c%c%-2s%c%04d %c%c%c%c%c%c%-32s",
		CR->routestring,(probeid!=NULL ? probeid:""),
		(deviceid!=NULL ? deviceid:""),(mtrcfg!=NULL ? mtrcfg:""),
		(timname!=NULL ? timname:""),
		(pword!=NULL ? pword:""),(dchain ? 'Y':'N'),(dst ? 'Y':'N'),
		(caddr!=NULL ? caddr:""),(tza<0 ? '-':'+'),
		(tza<0 ? (tza*(-1)):tza),(vr ? 'Y':'N'),(rr ? 'Y':'N'),
		(rd ? 'Y':'N'),(dstu ? 'Y':'N'),(sc ? 'Y':'N'),
		(ef ? 'Y':'N'),(et!=NULL ? et:""));
	WriteItronLine();	
	if(deviceid!=NULL) Rfree(deviceid);
	if(timname!=NULL) Rfree(timname);
	if(probeid!=NULL) Rfree(probeid);
	if(caddr!=NULL) Rfree(caddr);
	if(et!=NULL) Rfree(et);
	if(pword!=NULL) Rfree(pword);
	if(mtrcfg!=NULL) Rfree(mtrcfg);
}
static void DoWand(Route_Statistic *CR)
{
	char *deviceid=NULL,*timname=NULL,tamper=FALSE;
	short nodes=0;

	memset(ItronLine,0,127);
	FINDFLDGETSTRING(sortfile->fileno,"DEVICE ID",&deviceid);
	FINDFLDGETSTRING(sortfile->fileno,"WAND PROGRAM",&timname);
	FINDFLDGETSHORT(sortfile->fileno,"NODE NUMBER",&nodes);
	FINDFLDGETCHAR(sortfile->fileno,"NO TAMPER CHECK",&tamper);
	sprintf(ItronLine,"WRR%-8s%-14s%4s      %02d%c%90s",
		CR->routestring,(deviceid!=NULL ? deviceid:""),
		(timname!=NULL ? timname:""),nodes,(tamper ? 'Y':'N'),
		"");
	if(deviceid!=NULL) Rfree(deviceid);
	if(timname!=NULL) Rfree(timname);
	WriteItronLine();	
}
static void DoRadio(Route_Statistic *CR)
{
	char *timname=NULL,*probeid=NULL,*tamper=NULL;
	char concentrator=FALSE;
	short ga=0,rt=0;
	double rf=0.0;

	FINDFLDGETSTRING(sortfile->fileno,"WAND PROGRAM",&timname);
	FINDFLDGETSTRING(sortfile->fileno,"PROBE ID",&probeid);
	FINDFLDGETSHORT(sortfile->fileno,"GEOGRAPHIC AREA",&ga);
	FINDFLDGETDOUBLE(sortfile->fileno,"RF FREQUENCY",&rf);
	FINDFLDGETSHORT(sortfile->fileno,"RF TONE",&rt);
	FINDFLDGETSTRING(sortfile->fileno,"TAMPER STATUS",&tamper);
	FINDFLDGETCHAR(sortfile->fileno,"CONCENTRATOR INDICATOR",&concentrator);
	memset(ItronLine,0,127);
	if(RDAstrlen(probeid)>8) probeid[8]=0;
	if(RDAstrlen(tamper)>2) tamper[2]=0;
	sprintf(ItronLine,"RFF%-8s%-8s%6s%-4s%7s%02d%12.6f%04d%10s%02s%c%59s",
		CR->routestring,(probeid!=NULL ? probeid:""),"",
		(timname!=NULL ? timname:""),"",ga,rf,rt,"",
		(tamper!=NULL ? tamper:""),(concentrator ? 'Y':'N'),"");
	WriteItronLine();	
	if(timname!=NULL) Rfree(timname);
	if(probeid!=NULL) Rfree(probeid);
}
void CreateItronFile()
{
	RDArsrc *tmprsrc=NULL;
	char *fname=NULL,*custid=NULL,*last_custid=NULL;
	char survey=FALSE;
	int route_count=0,route=(-1),svc=0,last_svc=0;
	short ctype=(-1);
	short ef=0;
	Route_Statistic CR;

	fname=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen(ITRON_FILENAME)+50);
#ifndef WIN32
	sprintf(fname,"%s/%s",CURRENTDIRECTORY,ITRON_FILENAME);
#endif /* Not WIN32 */
#ifdef WIN32
	sprintf(fname,"%s\\%s",CURRENTDIRECTORY,(ITRON_FILENAME!=NULL ? ITRON_FILENAME:""));
#endif /* WIN32 */
	outfile=fopen(fname,"w+b");	
	if(outfile==NULL)
	{
		prterr("Error:  Couldn't create output file [%s].",(ITRON_FILENAME!=NULL ? ITRON_FILENAME:""));
		return;
	}
	filenum=OPNNRD(sortfile->module,sortfile->filename);
	memset(ItronLine,0,127);
	sprintf(ItronLine,"FHDN%c%8s01%c%cN%108s",(UseProbe ? 'Y':'N'),"",
		(UseRadio ? 'Y':'N'),(UseWand ? 'Y':'N'),"");
	WriteItronLine();	
	route_count=CountRoutes();
	memset(ItronLine,0,127);
	sprintf(ItronLine,"CHD01%04d%8s%109s",route_count,CycleDate,"");
	WriteItronLine();	
	tmprsrc=diagscrn(sortfile->fileno,"DIAGNOSTIC SCREEN",module,
		"Creating Itron File",NULL);
	if(tmprsrc!=NULL)
	{
		if(!APPmakescrn(tmprsrc,TRUE,NULL,NULL,FALSE))
		{
			ForceWindowUpdate(tmprsrc);
		} else {
		}
	}
	CR.route=(-1);
	CR.name=NULL;
	CR.routestring=NULL;
	CR.survey1=NULL;
	CR.survey2=NULL;
	CR.message=NULL;
	CR.time_allowed=NULL;
	ZeroRouteStatistics(&CR);
	ZERNRD(sortfile->fileno);
	ef=FRSNRD(sortfile->fileno,1);
	while(!ef)
	{
		FINDFLDGETINT(sortfile->fileno,"ROUTE NUMBER",&route);
		if(CR.route!=route)
		{
			if(CR.route!=(-1))
			{
 /* Route Footer */
				memset(ItronLine,0,127);
				if(CR.survey1!=NULL || CR.survey2!=NULL) survey=TRUE;
					else survey=FALSE;		
				sprintf(ItronLine,"RTR%8s%c%c%04d%04d%04d%04d%04d%04d%04d%04d%6s%04d%04d%04d%04d%04d      00  %04d%c%40s",
					CR.routestring,(survey==TRUE ? 'Y':'N'),
					(CR.message!=NULL ? 'Y':'N'),
					NumberKeys,CR.num_readings,
					CR.num_demand,CR.num_keyed,
					CR.num_probed,CR.num_radio,
					CR.num_customers,CR.num_meters,
					CR.time_allowed,CR.num_gas,CR.num_water,
					CR.num_electric,CR.num_location,
					CR.num_extra,CR.num_wand,
					(CR.mobile_amr ? 'M':' '),"");
				WriteItronLine();	
			}
			GatherRouteStatistics(&CR,route);
/* Route Header */
			memset(ItronLine,0,127);
			sprintf(ItronLine,"RHD%8s%c%c%04d%04d%04d%04d%04d%04d%04d%04d%6s%04d%04d%04d%04d%04d      00  %04d%c%40s",
				CR.routestring,(survey==TRUE ? 'Y':'N'),
				(CR.message!=NULL ? 'Y':'N'),NumberKeys,
				CR.num_readings,CR.num_demand,CR.num_keyed,
				CR.num_probed,CR.num_radio,CR.num_customers,
				CR.num_meters,CR.time_allowed,CR.num_gas,
				CR.num_water,CR.num_electric,CR.num_location,
				CR.num_extra,CR.num_wand,
				(CR.mobile_amr ? 'M':' '),"");
			WriteItronLine();	
/* Route Survey */
			if(CR.survey1!=NULL || CR.survey2!=NULL)
			{
				memset(ItronLine,0,127);
				sprintf(ItronLine,"RTS%8s%-40s%-40s%35s",
					CR.routestring,
					(CR.survey1!=NULL ? CR.survey1:""),
					(CR.survey2!=NULL ? CR.survey2:""),"");
				WriteItronLine();	
			}
/* Route Message */
			if(CR.message!=NULL)
			{
				memset(ItronLine,0,127);
				sprintf(ItronLine,"RTM%8s%-18s%05d%-75s%15s",CR.routestring,(CR.name!=NULL ? CR.name:""),CR.route,(CR.message!=NULL ? CR.message:""),"");
				WriteItronLine();	
			}
		}
		FINDFLDGETSTRING(sortfile->fileno,"CUSTOMER IDENTIFICATION",&custid);
		FINDFLDGETINT(sortfile->fileno,"SERVICE NUMBER",&svc);
/* Customer */
		if(RDAstrcmp(custid,last_custid) || svc!=last_svc)
		{
			if(last_custid!=NULL) Rfree(last_custid);
			last_custid=stralloc(custid);
			last_svc=svc;
			DoCustomer(&CR,custid,svc);
		}
		FINDFLDGETSHORT(sortfile->fileno,"COLLECTION TYPE",&ctype);
		switch(ctype)
		{
			default:
			case 0: /* keyed */
/* Meter */
				DoMeter(&CR);
/* Reading */
				DoReading(&CR);
				break;
			case 1: /* probed */
/* Meter */
				DoMeter(&CR);
/* PRB */
				DoProbe(&CR);
/* Reading */
				DoReading(&CR);
				break;
			case 2: /* wand */
/* Meter */
				DoMeter(&CR);
/* Reading */
				DoReading(&CR);
/* WRR */
				DoWand(&CR);
				break;
			case 3: /* radio */
/* Meter */
				DoMeter(&CR);
/* Reading */
				DoReading(&CR);
/* RFF */
				DoRadio(&CR);
				break;
		}
		if(tmprsrc!=NULL) update_diagnostic(tmprsrc,TRUE);
		ef=NXTNRD(sortfile->fileno,1);
	}
 /* Route Footer */
	memset(ItronLine,0,127);
	if(CR.survey1!=NULL || CR.survey2!=NULL) survey=TRUE;
		else survey=FALSE;		
	sprintf(ItronLine,"RTR%8s%c%c%04d%04d%04d%04d%04d%04d%04d%04d%6s%04d%04d%04d%04d%04d      01  %04d%c%40s",
		CR.routestring,(survey==TRUE ? 'Y':'N'),
		(CR.message!=NULL ? 'Y':'N'),
		NumberKeys,CR.num_readings,
		CR.num_demand,CR.num_keyed,
		CR.num_probed,CR.num_radio,
		CR.num_customers,CR.num_meters,
		CR.time_allowed,CR.num_gas,CR.num_water,
		CR.num_electric,CR.num_location,
		CR.num_extra,CR.num_wand,
		(CR.mobile_amr ? 'M':' '),"");
	WriteItronLine();	

	memset(ItronLine,0,127);
	sprintf(ItronLine,"CTR01%04d%8s%109s",route_count,CycleDate,"");
	WriteItronLine();	
	memset(ItronLine,0,127);
	sprintf(ItronLine,"FTRN%c%8s01%c%cN%108s",(UseProbe ? 'Y':'N'),"",
		(UseRadio ? 'Y':'N'),(UseWand ? 'Y':'N'),"");
	WriteItronLine();	
	fclose(outfile);
	if(tmprsrc!=NULL)
	{
		killwindow(tmprsrc);
		free_rsrc(tmprsrc);
		tmprsrc=NULL;
	}
	CLSNRD(filenum);
}
