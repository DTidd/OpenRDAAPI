/* MBPLUS.H */
#ifndef MBPLUS_H
#define MBPLUS_H
#include <mix.hpp>

struct finadss {
	char deflen[10];
	char deftitle[10][11];
	char defoff[10];
	char vtab[10];
	char htab[10];
	char type[10];
};
extern struct finadss finadsd;

/* variable file */
extern struct vacf
{
	char mjr_obj;
	char obj_no;
	char amen_bud;
	char start_mth;
	char sq[7][4];		/* old file */
	char ln[7][4];		/* old file */
	char it[7][4];		/* old file */
	char ll[10][4];		/* digit length, but if header is longer it is this */
	char bl[10][4];		/* actual entry length of each digit */
	char bs_code [14][10];
	char header [10][4][5];
/*
	FD-LOC-PGM-PJT-FUNC-OBJ
	10-0  -000-000-2210-110

ll)	2  3   3   3   4    3   0   0   0   0
bl)	2  1   3   3   4    3   0   0   0   0
*/
} varac;
extern char partbs,offbs;

#define findpart(a,b)	xfindpart(a,b,__LINE__,__FILE__)
short xfindpart(char *,short,int,char *);
#define offpart(a,b)	xoffpart(a,b,__LINE__,__FILE__)
short xoffpart(short,short,int,char *);
#define acread()	xacread(__LINE__,__FILE__)
short xacread(int,char *);
#define adread()	xadread(__LINE__,__FILE__)
short xadread(int,char *);
void MakeXpertAccountStructure(void);
void MBPlusFINSBC(void);


struct OFCtypes
{
	short code;	
	char **title;
	char **fldname;
	short fldtype[25];
	char type[25];
	unsigned len[25];
};
typedef struct OFCtypes OFCtype;

struct MBOFCs
{
	DBsort *sort;
	short type; /* 0 - manual structure, 1-from database */
	short confileno;
	char *conname;
	char *fldname;
	short count;
	int num;
	OFCtype *t;
};
typedef struct MBOFCs MBOFC;

#define MBOFCNEW(a,b)	xMBOFCNEW(a,b,__LINE__,__FILE__)
MBOFC *xMBOFCNEW(DBsort *,short,int,char *);
#define FreeMBOFC(a)	xFreeMBOFC(a,__LINE__,__FILE__)
void xFreeMBOFC(MBOFC *,int,char *);
#define MBOFC_MANUAL(a,b,c,d,e)	xMBOFC_MANUAL(a,b,c,d,e,__LINE__,__FILE__)
MBOFC *xMBOFC_MANUAL(DBsort *,short,char [][11],char *,char *,int,char *);
#define MBOFC_DYNAMIC(a,b,c,d,e)	xMBOFC_DYNAMIC(a,b,c,d,e,__LINE__,__FILE__)
MBOFC *xMBOFC_DYNAMIC(DBsort *,short,short,char *,char *,int,char *);
#define GatherCombos(a)	xGatherCombos(a,__LINE__,__FILE__)
void xGatherCombos(MBOFC *,int,char *);
#define SetFieldNames(a)	xSetFieldNames(a,__LINE__,__FILE__)
void xSetFieldNames(MBOFC *,int,char *);
#define MBOFC_addDBfield(a)	xMBOFC_addDBfield(a,__LINE__,__FILE__)
void xMBOFC_addDBfield(MBOFC *,int,char *);
#endif /* MBPLUS_H */
