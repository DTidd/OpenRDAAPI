#ifndef PACKLIB_H
#define PACKLIB_H
#include <mix.hpp>
#include <mkmsc.hpp>
#include <mkchg.hpp>
#include <mkimp.hpp>

char *fixname(char *);
void makeDefaultMaintainMenu(MaintainMaster *);
void makeDefaultAddMenu(PowerAdd *);
void makeDefaultReportMenu(char *,RDAreport *);
void makeDefaultChangeMenu(char *,CHGstruct *);
void makeDefaultIMPEXPMenu(char *,IEfile *);
void pack_virfld(FILE *,char *,char *,char *);
void pack_menu(FILE *,char *,char *,char *);
void pack_screen(FILE *,char *,char *,char *,char,char,char);
void pack_any_screen(FILE *,char *,char *,char *,char,char,char,char,char);
void pack_add_screen(FILE *,char *,char *,char *,char,char,char);
void pack_report_screen(FILE *,char *,char *,char *,char,char,char);
void pack_def(FILE *,char *,char *,char *,char);
void pack_list(FILE *,char *,char *);
#ifndef WIN32
short system_command_retvalue(char *);
#endif /* WIN32 */
/* From mkscncd */
void makescrncode(RDAscrn *,FILE *,char *,char *,char *);
RDAscrn *okscreen(char *,char *,char *);
/* End from mkscncd */
/* From mkhelpcd */
void makehelpcode(RDAhelp *,FILE *,char *,char *,char *);
RDAhelp *okhelp(char *,char *,char *);
/* End from mkhelpcd */
/* From mkmtncd */
void makeMaintainMastercode(MaintainMaster *,FILE *,char *,char *,char *);
void makeAllMaintainMastercode(MaintainMaster *,FILE *,char *, char *,char *,char);
/* End from mkmtncd */
/* From mkaddcd */
void makeaddcode(PowerAdd *,FILE *,char *,char *,char *);
/* End from mkaddcd */
/* From mklistcd */
void makelistcode(RDAScrolledList *,FILE *,char *,char *);
RDAScrolledList *oklist(char *,char *);
/* End from mklistcd */
/* From mkvircd */
void makevirfldcode(DFvirtual *,FILE *,char *,char *);
DFvirtual *okvirfld(char *libname,char *virfldname);
/* End from mkvircd */
/* From mkmenucd */
void makemenucode(RDAmenu *,FILE *,char *,char *,char *);
RDAmenu *okmenu(char *,char *,char *);
/* End from mkmenucd */
/* From mkloadcd */
void makeloadcode(RDALoadValue *,FILE *,char *,char *);
RDALoadValue *okload(char *,char *);
/* End from mkloadcd */
/* From mkdefcd */
void makedefcode(RDAdefault *,FILE *,char *,char *,char *);
RDAdefault *okdef(char *,char *,char *);
RDAdefault *okdef_noerrors(char *,char *,char *);
RDAdefault *okdef_full(char *,char *,char *,char);
/* End from mkdefcd */
/* From mkrptcd */
void makerptcode(RDAreport *,FILE *,char *,char);
/* End from mkrptcd */
/* From mkchgcd */
void makechgcode(CHGstruct *,FILE *,char *,char *,char *);
/* End from mkchgcd */
/* From mkimpcd */
void makeIMPEXPcode(IEfile *,FILE *,char *,char *);
/* End from mkimpcd */
#endif /* PACKLIB_H */
