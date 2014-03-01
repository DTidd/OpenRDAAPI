#include <cstdio>
#include <mix.hpp>
#include <mkmsc.hpp>


#if !defined(LINUX) && !defined(LINUX2_2) && !defined(UBUNTU_OS)
#define MODE_RWXRWX___ (S_IRWXU|S_IRWXG)
#endif /* ifndef LINUX */
#if defined(LINUX) || defined(LINUX2_2) || defined(UBUNTU_OS)|| defined(UBUNTU_OS)
#define MODE_RWXRWX___ (00770)
#endif /* ifdef LINUX */

void SCRIPT_FILLED_xpm()
{
	FILE *fp=NULL;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("FILLED.xpm")+15);
#ifdef WIN32
	sprintf(temp,"%s\\pixmaps\\FILLED.xpm",CURRENTDIRECTORY);
#endif
#ifndef WIN32
	sprintf(temp,"%s/pixmaps/FILLED.xpm",CURRENTDIRECTORY);
#endif
	fp=fopen(temp,"w+b");
	if(temp!=NULL) Rfree(temp);

	fprintf(fp,"/* XPM */\n");
	fprintf(fp,"static char * FILLED_xpm[] = {\n");
	fprintf(fp,"\"32 32 1 1\",\n");
	fprintf(fp,"\" 	c black\",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \"};\n");
	fclose(fp);
}
void SCRIPT_NONE_xpm()
{
	FILE *fp=NULL;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("NONE.xpm")+15);
#ifdef WIN32
	sprintf(temp,"%s\\pixmaps\\NONE.xpm",CURRENTDIRECTORY);
#endif
#ifndef WIN32
	sprintf(temp,"%s/pixmaps/NONE.xpm",CURRENTDIRECTORY);
#endif
	fp=fopen(temp,"w+b");
	if(temp!=NULL) Rfree(temp);

	fprintf(fp,"/* XPM */\n");
	fprintf(fp,"static char * NONE [] = {\n");
	fprintf(fp,"\"32 32 1 1\",\n");
	fprintf(fp,"\"       s None  c None\",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \",\n");
	fprintf(fp,"\"                                \"};\n");
	fclose(fp);
}
void SCRIPT_xpertrc_ltgrn()
{
	FILE *fp=NULL;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("xpertrc_ltgrn")+15);
#ifdef WIN32
	sprintf(temp,"%s\\rda\\xpertrc_ltgrn",CURRENTDIRECTORY);
#endif
#ifndef WIN32
	sprintf(temp,"%s/rda/xpertrc_ltgrn",CURRENTDIRECTORY);
#endif
	fp=fopen(temp,"w+b");
	if(temp!=NULL) Rfree(temp);

	fprintf(fp,"## RDA's GTK Resource File Definition\n");
	fprintf(fp,"##\n");
#ifdef WIN32
	fprintf(fp,"module_path \".\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"module_path \"/rda/xpgms:/rda/xpgmsgtk\"\n");
#endif
	fprintf(fp,"pixmap_path \"./pixmaps\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	fprintf(fp,"style \"default\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"  bg[NORMAL] = \"#c0c0c0\"\n");
	fprintf(fp,"  bg[PRELIGHT] = \"#c0c0c0\"\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"  font = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"  fontset = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"  engine \"redmond95\" \n");
	fprintf(fp,"  {\n");
	fprintf(fp,"  }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"menu\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#  bg[PRELIGHT] = { 0.0, 0.0, 0.6 }\n");
	fprintf(fp,"#  fg[PRELIGHT] = { 1.0, 1.0, 1.0 }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"list\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#   bg[NORMAL] = { 0.9, 0.9, 0.9 }\n");
	fprintf(fp,"#   bg[PRELIGHT] = { 0.9, 0.9, 0.9 }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"widget_class \"*MenuItem*\" style \"menu\"\n");
	fprintf(fp,"widget_class \"*Menu*\" style \"menu\"\n");
	fprintf(fp,"widget_class \"GtkCTree\" style \"list\"\n");
	fprintf(fp,"widget_class \"GtkCList\" style \"list\"\n");
	fprintf(fp,"style \"window\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#  bg_pixmap[NORMAL] = \"warning.xpm\"\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"scale\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#  fg[NORMAL] = { 1.0, 0, 0 }\n");
	fprintf(fp,"#  bg[PRELIGHT] = { 1.0, 1.0, 0 }\n");
	fprintf(fp,"#  bg_pixmap[NORMAL] = \"<parent>\"\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"button\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#   fg[PRELIGHT] = { 1.0, 1.0, 1.0 }\n");
	fprintf(fp,"#   bg[PRELIGHT] = { 0, 0, 0.75 }\n");
	fprintf(fp,"#   bg[PRELIGHT] = { 0.75, 0, 0x00 }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"toggle_button\" = \"button\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#   fg[NORMAL] = { 1.0, 0, 0 }\n");
	fprintf(fp,"#   fg[ACTIVE] = { 1.0, 0, 0 }\n");
	fprintf(fp,"#   bg_pixmap[NORMAL] = \"<parent>\"\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"text\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#   bg_pixmap[NORMAL] = \"marble.xpm\"\n");
	fprintf(fp,"#   text[NORMAL] = { 0.2, 0.2, 1.0 }\n");
	fprintf(fp,"#   fg[NORMAL] = { 1.0, 1.0, 1.0 }\n");
	fprintf(fp,"#   base[NORMAL] = { 0.0, 0.0, 0.0 }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"slider\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#   fg[NORMAL] = { 1.0, 1.0, 1.0 }\n");
	fprintf(fp,"#   bg[NORMAL] = { 0.0, 0.0, 1.0 }\n");
	fprintf(fp,"#   bg[ACTIVE] = { 0.0 ,0.0, 0.5 }\n");
	fprintf(fp,"#   bg[PRELIGHT] = { 0.75 ,0.75, 1.0 }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"ruler\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#   font = '-adobe-helvetica-medium-r-normal--*-80-*-*-*-*-*-*'\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"curve\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#   fg[NORMAL] = { 58000, 0, 0 }			# red\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"red-bar\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#   bg[PRELIGHT] = { 0.95, .55, 0.55 }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"# override testgtk2, introduce the green color in the button list\n");
	fprintf(fp,"style 'button_list' = 'button'\n");
	fprintf(fp,"{\n");
	fprintf(fp,"   font = \"-adobe-helvetica-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"   bg[PRELIGHT] = { 0, 0.75, 0x00 }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"main window.*GtkScrolledWindow.*GtkButton*\" style \"button_list\"\n");
	fprintf(fp,"# common default\n");
	fprintf(fp,"class \"GtkWidget\" style \"default\"\n");
	fprintf(fp,"class \"GtkScrollbar\" style \"red-bar\"\n");
	fprintf(fp,"widget_class \"GtkWindow\" style \"window\"\n");
	fprintf(fp,"widget_class \"GtkDialog\" style \"window\"\n");
	fprintf(fp,"widget_class \"GtkFileSelection\" style \"window\"\n");
	fprintf(fp,"widget_class \"*Gtk*Scale\" style \"scale\"\n");
	fprintf(fp,"widget_class \"*GtkCheckButton*\" style \"toggle_button\"\n");
	fprintf(fp,"widget_class \"*GtkRadioButton*\" style \"toggle_button\"\n");
	fprintf(fp,"widget_class \"*GtkButton*\" style \"button\"\n");
	fprintf(fp,"widget_class \"*Ruler\" style \"ruler\"\n");
	fprintf(fp,"widget_class \"*GtkText\" style \"text\"\n");
	fprintf(fp,"widget \"*GtkCurve\" style \"curve\"\n");
	fprintf(fp,"binding \"test1\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"  bind \"<ctrl>1\" {\n");
	fprintf(fp,"    \"debug-msg\" (\"hallo and\") \n");
	fprintf(fp,"    \"debug-msg\" (\"huhu\")\n");
	fprintf(fp,"  }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"binding \"test2\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"  bind \"<ctrl>1\" {\n");
	fprintf(fp,"    \"debug-msg\" (\"jup!\") \n");
	fprintf(fp,"  }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"# possible priorities are (in ascending order):\n");
	fprintf(fp,"# lowest\n");
	fprintf(fp,"# gtk         (used by gtk for internal class bindings)\n");
	fprintf(fp,"# application (for hard coded bindings on application basis)\n");
	fprintf(fp,"# rc          (used implicitel by rc files)\n");
	fprintf(fp,"# highest\n");
	fprintf(fp,"class \"GtkButton\" binding \"test1\"           # implicit : rc\n");
	fprintf(fp,"class \"GtkButton\" binding : highest \"test2\" # override \"rc\" priority\n");
	fprintf(fp,"binding \"clist-test\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"  bind \"j\" {\n");
	fprintf(fp,"    \"scroll-vertical\" (step-backward, 0.0)\n");
	fprintf(fp,"  }\n");
	fprintf(fp,"  bind \"k\" {\n");
	fprintf(fp,"    \"scroll-vertical\" (step-forward, 0.0)\n");
	fprintf(fp,"  }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"class \"GtkCList\" binding \"clist-test\"\n");
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	fprintf(fp,"\n");

	fprintf(fp,"style \"RDAWindowDefault\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {0.80, 1.00, 0.72}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"class \"GtkWindow\" style \"RDAWindowDefault\"\n");
	fprintf(fp,"class \"GtkScrolledWindow\" style \"RDAWindowDefault\"\n");
	fprintf(fp,"class \"GtkViewport\" style \"RDAWindowDefault\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"RDASaveWarning\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {1.00, 0.00, 0.00}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*SAVE WARNING DIALOG*\" style \"RDASaveWarning\"\n");
	fprintf(fp,"widget \"XPERT*QUIT WARNING DIALOG*\" style \"RDASaveWarning\"\n");
	fprintf(fp,"widget \"XPERT*ERROR DIALOG SCREEN*\" style \"RDASaveWarning\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"RDA_Labels\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"##	fg[NORMAL] = {1.00, 1.00, 1.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"class \"GtkLabel\" style \"RDA_Labels\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"RDA_ScrollBar\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {0.31, 0.70, 0.73}\n");
	fprintf(fp,"	bg[PRELIGHT] = {1.00, 0.00, 0.00}\n");
	fprintf(fp,"}\n");
	fprintf(fp,"class \"GtkScrollbar\" style \"RDA_ScrollBar\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"RDADefaultButton\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {1.00, 0.92, 0.80}\n");
	fprintf(fp,"	bg[PRELIGHT] = {0.32, 0.81, 1.00}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"class \"GtkButton\" style \"RDADefaultButton\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"defaultwidget\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	fg[INSENSITIVE] = {0.80, 1.00, 0.72}\n");
	fprintf(fp,"	bg[INSENSITIVE] = {0.80, 1.00, 0.72}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
	fprintf(fp,"	bg[NORMAL] = {1.00, 1.00, 1.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"	bg[PRELIGHT] = {0.32, 0.81, 1.00}\n");
	fprintf(fp,"}\n");
	fprintf(fp,"class \"GtkCombo\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkEntry\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkPlaintext\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkFormattedtext\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkUnsignednumber\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkUnsigneddecimal\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkSignednumber\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkSigneddecimal\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkRDAList\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkCList\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkCurrency100\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkCurrencync\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkRDAText\" style \"defaultwidget\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"toggle_button\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#	bg_pixmap[ACTIVE] = \"FILLED.xpm\"\n");
	fprintf(fp,"#	bg_pixmap[NORMAL] = \"NONE.xpm\"\n");
	fprintf(fp,"#	bg[NORMAL] = {0.31, 0.70, 0.73}\n");
	fprintf(fp,"	bg[ACTIVE] = {1.00, 0.00, 0.00}\n");
	fprintf(fp,"}\n");
	fprintf(fp,"class \"*GtkRDACheckButton*\" style \"toggle_button\"\n");
	fprintf(fp,"class \"*GtkRDAToggleButton*\" style \"toggle_button\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"RDA_QuitButton\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {0.98, 0.97, 0.27}\n");
	fprintf(fp,"	bg[PRELIGHT] = {0.32, 0.81, 1.00}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*QUIT\" style \"RDA_QuitButton\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"RDA_DeleteButton\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {1.00, 0.00, 0.00}\n");
	fprintf(fp,"	bg[PRELIGHT] = {0.32, 0.81, 1.00}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*.DELETE\" style \"RDA_DeleteButton\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"RDA_HelpButton\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {0.73, 0.48, 0.00}\n");
	fprintf(fp,"	bg[PRELIGHT] = {0.32, 0.81, 1.00}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*.HELP\" style \"RDA_HelpButton\"\n");
	fprintf(fp,"widget \"XPERT*.CYBERSUPPORT\" style \"RDA_HelpButton\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"RDA_AddButton\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {0.79, 0.85, 0.94}\n");
	fprintf(fp,"	bg[PRELIGHT] = {0.32, 0.81, 1.00}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*.ADD\" style \"RDA_AddButton\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"RDA_SelectButton\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {0.00, 1.00, 0.00}\n");
	fprintf(fp,"	bg[PRELIGHT] = {0.32, 0.81, 1.00}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*SELECT\" style \"RDA_SelectButton\"\n");
	fprintf(fp,"widget \"XPERT*SELECT ALL\" style \"RDA_SelectButton\"\n");
	fprintf(fp,"widget \"XPERT*SAVE\" style \"RDA_SelectButton\"\n");
	fprintf(fp,"widget \"XPERT*SAVE NO UPDATE\" style \"RDA_SelectButton\"\n");
	fprintf(fp,"widget \"XPERT*CONTINUE\" style \"RDA_SelectButton\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"RDAOUTwidget\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	fg[INSENSITIVE] = {0.31, 0.70, 0.73}\n");
	fprintf(fp,"	bg[INSENSITIVE] = {0.31, 0.70, 0.73}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-60-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-60-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-60-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-60-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT.UTILITIES.TEXT BROWSER.TEXT\" style \"RDAOUTwidget\"\n");
	fprintf(fp,"style \"BUBBLEHELP\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	fg[INSENSITIVE] = {0.00, 0.00, 1.00}\n");
	fprintf(fp,"	bg[NORMAL] = {0.98, 0.97, 0.27}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-150-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-150-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-140-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-140-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"*BUBBLE*HELP*\" style \"BUBBLEHELP\"\n");
	fclose(fp);
}

void SCRIPT_xpertrc()
{
	FILE *fp=NULL;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("xpertrc_orig")+15);
#ifdef WIN32
	sprintf(temp,"%s\\rda\\xpertrc_orig",CURRENTDIRECTORY);
#endif
#ifndef WIN32
	sprintf(temp,"%s/rda/xpertrc_orig",CURRENTDIRECTORY);
#endif
	fp=fopen(temp,"w+b");
	if(temp!=NULL) Rfree(temp);

#ifdef WIN32
	fprintf(fp,"module_path \".\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"module_path \"/rda/xpgms:/rda/xpgmsgtk\"\n");
#endif
	fprintf(fp,"pixmap_path \"./pixmaps\"\n");
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	fprintf(fp,"style \"default\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"  bg[NORMAL] = \"#c0c0c0\"\n");
	fprintf(fp,"  bg[PRELIGHT] = \"#c0c0c0\"\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"  font = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"  fontset = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"  engine \"redmond95\" \n");
	fprintf(fp,"  {\n");
	fprintf(fp,"  }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"menu\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#  bg[PRELIGHT] = { 0.0, 0.0, 0.6 }\n");
	fprintf(fp,"#  fg[PRELIGHT] = { 1.0, 1.0, 1.0 }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"list\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#   bg[NORMAL] = { 0.9, 0.9, 0.9 }\n");
	fprintf(fp,"#   bg[PRELIGHT] = { 0.9, 0.9, 0.9 }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"widget_class \"*MenuItem*\" style \"menu\"\n");
	fprintf(fp,"widget_class \"*Menu*\" style \"menu\"\n");
	fprintf(fp,"widget_class \"GtkCTree\" style \"list\"\n");
	fprintf(fp,"widget_class \"GtkCList\" style \"list\"\n");
	fprintf(fp,"style \"window\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#  bg_pixmap[NORMAL] = \"warning.xpm\"\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"scale\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#  fg[NORMAL] = { 1.0, 0, 0 }\n");
	fprintf(fp,"#  bg[PRELIGHT] = { 1.0, 1.0, 0 }\n");
	fprintf(fp,"#  bg_pixmap[NORMAL] = \"<parent>\"\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"button\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#   fg[PRELIGHT] = { 1.0, 1.0, 1.0 }\n");
	fprintf(fp,"#   bg[PRELIGHT] = { 0, 0, 0.75 }\n");
	fprintf(fp,"#   bg[PRELIGHT] = { 0.75, 0, 0x00 }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"toggle_button\" = \"button\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#   fg[NORMAL] = { 1.0, 0, 0 }\n");
	fprintf(fp,"#   fg[ACTIVE] = { 1.0, 0, 0 }\n");
	fprintf(fp,"#   bg_pixmap[NORMAL] = \"<parent>\"\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"text\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#   bg_pixmap[NORMAL] = \"marble.xpm\"\n");
	fprintf(fp,"#   text[NORMAL] = { 0.2, 0.2, 1.0 }\n");
	fprintf(fp,"#   fg[NORMAL] = { 1.0, 1.0, 1.0 }\n");
	fprintf(fp,"#   base[NORMAL] = { 0.0, 0.0, 0.0 }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"slider\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#   fg[NORMAL] = { 1.0, 1.0, 1.0 }\n");
	fprintf(fp,"#   bg[NORMAL] = { 0.0, 0.0, 1.0 }\n");
	fprintf(fp,"#   bg[ACTIVE] = { 0.0 ,0.0, 0.5 }\n");
	fprintf(fp,"#   bg[PRELIGHT] = { 0.75 ,0.75, 1.0 }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"ruler\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#   font = '-adobe-helvetica-medium-r-normal--*-80-*-*-*-*-*-*'\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"curve\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#   fg[NORMAL] = { 58000, 0, 0 }			# red\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"red-bar\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#   bg[PRELIGHT] = { 0.95, .55, 0.55 }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"# override testgtk2, introduce the green color in the button list\n");
	fprintf(fp,"style 'button_list' = 'button'\n");
	fprintf(fp,"{\n");
	fprintf(fp,"   font = \"-adobe-helvetica-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"   bg[PRELIGHT] = { 0, 0.75, 0x00 }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"main window.*GtkScrolledWindow.*GtkButton*\" style \"button_list\"\n");
	fprintf(fp,"# common default\n");
	fprintf(fp,"class \"GtkWidget\" style \"default\"\n");
	fprintf(fp,"class \"GtkScrollbar\" style \"red-bar\"\n");
	fprintf(fp,"widget_class \"GtkWindow\" style \"window\"\n");
	fprintf(fp,"widget_class \"GtkDialog\" style \"window\"\n");
	fprintf(fp,"widget_class \"GtkFileSelection\" style \"window\"\n");
	fprintf(fp,"widget_class \"*Gtk*Scale\" style \"scale\"\n");
	fprintf(fp,"widget_class \"*GtkCheckButton*\" style \"toggle_button\"\n");
	fprintf(fp,"widget_class \"*GtkRadioButton*\" style \"toggle_button\"\n");
	fprintf(fp,"widget_class \"*GtkButton*\" style \"button\"\n");
	fprintf(fp,"widget_class \"*Ruler\" style \"ruler\"\n");
	fprintf(fp,"widget_class \"*GtkText\" style \"text\"\n");
	fprintf(fp,"widget \"*GtkCurve\" style \"curve\"\n");
	fprintf(fp,"binding \"test1\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"  bind \"<ctrl>1\" {\n");
	fprintf(fp,"    \"debug-msg\" (\"hallo and\") \n");
	fprintf(fp,"    \"debug-msg\" (\"huhu\")\n");
	fprintf(fp,"  }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"binding \"test2\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"  bind \"<ctrl>1\" {\n");
	fprintf(fp,"    \"debug-msg\" (\"jup!\") \n");
	fprintf(fp,"  }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"# possible priorities are (in ascending order):\n");
	fprintf(fp,"# lowest\n");
	fprintf(fp,"# gtk         (used by gtk for internal class bindings)\n");
	fprintf(fp,"# application (for hard coded bindings on application basis)\n");
	fprintf(fp,"# rc          (used implicitel by rc files)\n");
	fprintf(fp,"# highest\n");
	fprintf(fp,"class \"GtkButton\" binding \"test1\"           # implicit : rc\n");
	fprintf(fp,"class \"GtkButton\" binding : highest \"test2\" # override \"rc\" priority\n");
	fprintf(fp,"binding \"clist-test\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"  bind \"j\" {\n");
	fprintf(fp,"    \"scroll-vertical\" (step-backward, 0.0)\n");
	fprintf(fp,"  }\n");
	fprintf(fp,"  bind \"k\" {\n");
	fprintf(fp,"    \"scroll-vertical\" (step-forward, 0.0)\n");
	fprintf(fp,"  }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"class \"GtkCList\" binding \"clist-test\"\n");
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	fprintf(fp,"style \"RDAWindowDefault\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {0.83, 0.83, 0.83}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
	fprintf(fp,"}\n");
	fprintf(fp,"class \"GtkWindow\" style \"RDAWindowDefault\"\n");
	fprintf(fp,"class \"GtkScrolledWindow\" style \"RDAWindowDefault\"\n");
	fprintf(fp,"class \"GtkViewport\" style \"RDAWindowDefault\"\n");
	fprintf(fp,"style \"RDASaveWarning\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {1.00, 0.00, 0.00}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*SAVE WARNING DIALOG*\" style \"RDASaveWarning\"\n");
	fprintf(fp,"widget \"XPERT*QUIT WARNING DIALOG*\" style \"RDASaveWarning\"\n");
	fprintf(fp,"widget \"XPERT*ERROR DIALOG SCREEN*\" style \"RDASaveWarning\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"RDA_Labels\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"##	fg[NORMAL] = {1.00, 1.00, 1.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"class \"GtkLabel\" style \"RDA_Labels\"\n");
	fprintf(fp,"style \"RDA_ScrollBar\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {0.83, 0.83, 0.83}\n");
	fprintf(fp,"	bg[PRELIGHT] = {1.00, 0.00, 0.00}\n");
	fprintf(fp,"}\n");
	fprintf(fp,"class \"GtkScrollbar\" style \"RDA_ScrollBar\"\n");
	fprintf(fp,"style \"RDADefaultButton\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {1.00, 0.92, 0.80}\n");
	fprintf(fp,"	bg[PRELIGHT] = {0.00, 0.00, 1.00}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"class \"GtkButton\" style \"RDADefaultButton\"\n");
	fprintf(fp,"style \"defaultwidget\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	fg[INSENSITIVE] = {0.31, 0.70, 0.73}\n");
	fprintf(fp,"	bg[INSENSITIVE] = {0.31, 0.70, 0.73}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
	fprintf(fp,"	bg[NORMAL] = {1.00, 1.00, 1.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"	bg[PRELIGHT] = {0.00, 0.00, 1.00}\n");
	fprintf(fp,"}\n");
	fprintf(fp,"class \"GtkCombo\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkEntry\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkPlaintext\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkFormattedtext\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkUnsignednumber\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkUnsigneddecimal\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkSignednumber\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkSigneddecimal\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkRDAList\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkCList\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkCurrency100\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkCurrencync\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkRDAText\" style \"defaultwidget\"\n");
	fprintf(fp,"style \"toggle_button\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#	bg_pixmap[ACTIVE] = \"FILLED.xpm\"\n");
	fprintf(fp,"#	bg_pixmap[NORMAL] = \"NONE.xpm\"\n");
	fprintf(fp,"#	bg[NORMAL] = {0.31, 0.70, 0.73}\n");
	fprintf(fp,"	bg[ACTIVE] = {1.00, 0.00, 0.00}\n");
	fprintf(fp,"}\n");
	fprintf(fp,"class \"*GtkCheckButton*\" style \"toggle_button\"\n");
	fprintf(fp,"class \"*GtkToggleButton*\" style \"toggle_button\"\n");
	fprintf(fp,"class \"*GtkRDACheckButton*\" style \"toggle_button\"\n");
	fprintf(fp,"class \"*GtkRDAToggleButton*\" style \"toggle_button\"\n");
	fprintf(fp,"style \"RDA_QuitButton\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {0.98, 0.97, 0.27}\n");
	fprintf(fp,"	bg[PRELIGHT] = {0.00, 0.00, 1.00}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*QUIT\" style \"RDA_QuitButton\"\n");
	fprintf(fp,"style \"RDA_DeleteButton\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {1.00, 0.00, 0.00}\n");
	fprintf(fp,"	bg[PRELIGHT] = {0.00, 0.00, 1.00}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*QUIT\" style \"RDA_QuitButton\"\n");
	fprintf(fp,"style \"RDA_DeleteButton\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {1.00, 0.00, 0.00}\n");
	fprintf(fp,"	bg[PRELIGHT] = {0.00, 0.00, 1.00}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*.DELETE\" style \"RDA_DeleteButton\"\n");
	fprintf(fp,"style \"RDA_HelpButton\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {0.73, 0.48, 0.00}\n");
	fprintf(fp,"	bg[PRELIGHT] = {0.00, 0.00, 1.00}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*.HELP\" style \"RDA_HelpButton\"\n");
	fprintf(fp,"widget \"XPERT*.CYBERSUPPORT\" style \"RDA_HelpButton\"\n");
	fprintf(fp,"style \"RDA_AddButton\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {0.79, 0.85, 0.94}\n");
	fprintf(fp,"	bg[PRELIGHT] = {0.00, 0.00, 1.00}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*.ADD\" style \"RDA_AddButton\"\n");
	fprintf(fp,"style \"RDA_SelectButton\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {0.00, 1.00, 0.00}\n");
	fprintf(fp,"	bg[PRELIGHT] = {0.00, 0.00, 1.00}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*SELECT\" style \"RDA_SelectButton\"\n");
	fprintf(fp,"widget \"XPERT*SELECT ALL\" style \"RDA_SelectButton\"\n");
	fprintf(fp,"widget \"XPERT*SAVE\" style \"RDA_SelectButton\"\n");
	fprintf(fp,"widget \"XPERT*SAVE NO UPDATE\" style \"RDA_SelectButton\"\n");
	fprintf(fp,"widget \"XPERT*CONTINUE\" style \"RDA_SelectButton\"\n");
	fprintf(fp,"style \"RDAOUTwidget\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	fg[INSENSITIVE] = {0.31, 0.70, 0.73}\n");
	fprintf(fp,"	bg[INSENSITIVE] = {0.31, 0.70, 0.73}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-60-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-60-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-60-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-60-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT.UTILITIES.TEXT BROWSER.TEXT\" style \"RDAOUTwidget\"\n");
	fprintf(fp,"style \"BUBBLEHELP\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	fg[INSENSITIVE] = {0.00, 0.00, 1.00}\n");
	fprintf(fp,"	bg[NORMAL] = {0.98, 0.97, 0.27}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-150-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-150-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-140-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-140-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"*BUBBLE*HELP*\" style \"BUBBLEHELP\"\n");
	fclose(fp);
}
void SCRIPT_xpertrc_grey()
{
	FILE *fp=NULL;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("xpertrc_grey")+15);
#ifdef WIN32
	sprintf(temp,"%s\\rda\\xpertrc_grey",CURRENTDIRECTORY);
#endif
#ifndef WIN32
	sprintf(temp,"%s/rda/xpertrc_grey",CURRENTDIRECTORY);
#endif
	fp=fopen(temp,"w+b");
	if(temp!=NULL) Rfree(temp);

	fprintf(fp,"## RDA's GTK Resource File Definition\n");
	fprintf(fp,"##\n");
#ifdef WIN32
	fprintf(fp,"module_path \".\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"module_path \"/rda/xpgms:/rda/xpgmsgtk\"\n");
#endif
	fprintf(fp,"pixmap_path \"./pixmaps\"\n");
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	fprintf(fp,"style \"default\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"  bg[NORMAL] = \"#c0c0c0\"\n");
	fprintf(fp,"  bg[PRELIGHT] = \"#c0c0c0\"\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"  font = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"  fontset = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"  engine \"redmond95\" \n");
	fprintf(fp,"  {\n");
	fprintf(fp,"  }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"menu\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#  bg[PRELIGHT] = { 0.0, 0.0, 0.6 }\n");
	fprintf(fp,"#  fg[PRELIGHT] = { 1.0, 1.0, 1.0 }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"list\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#   bg[NORMAL] = { 0.9, 0.9, 0.9 }\n");
	fprintf(fp,"#   bg[PRELIGHT] = { 0.9, 0.9, 0.9 }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"widget_class \"*MenuItem*\" style \"menu\"\n");
	fprintf(fp,"widget_class \"*Menu*\" style \"menu\"\n");
	fprintf(fp,"widget_class \"GtkCTree\" style \"list\"\n");
	fprintf(fp,"widget_class \"GtkCList\" style \"list\"\n");
	fprintf(fp,"style \"window\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#  bg_pixmap[NORMAL] = \"warning.xpm\"\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"scale\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#  fg[NORMAL] = { 1.0, 0, 0 }\n");
	fprintf(fp,"#  bg[PRELIGHT] = { 1.0, 1.0, 0 }\n");
	fprintf(fp,"#  bg_pixmap[NORMAL] = \"<parent>\"\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"button\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#   fg[PRELIGHT] = { 1.0, 1.0, 1.0 }\n");
	fprintf(fp,"#   bg[PRELIGHT] = { 0, 0, 0.75 }\n");
	fprintf(fp,"#   bg[PRELIGHT] = { 0.75, 0, 0x00 }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"toggle_button\" = \"button\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#   fg[NORMAL] = { 1.0, 0, 0 }\n");
	fprintf(fp,"#   fg[ACTIVE] = { 1.0, 0, 0 }\n");
	fprintf(fp,"#   bg_pixmap[NORMAL] = \"<parent>\"\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"text\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#   bg_pixmap[NORMAL] = \"marble.xpm\"\n");
	fprintf(fp,"#   text[NORMAL] = { 0.2, 0.2, 1.0 }\n");
	fprintf(fp,"#   fg[NORMAL] = { 1.0, 1.0, 1.0 }\n");
	fprintf(fp,"#   base[NORMAL] = { 0.0, 0.0, 0.0 }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"slider\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#   fg[NORMAL] = { 1.0, 1.0, 1.0 }\n");
	fprintf(fp,"#   bg[NORMAL] = { 0.0, 0.0, 1.0 }\n");
	fprintf(fp,"#   bg[ACTIVE] = { 0.0 ,0.0, 0.5 }\n");
	fprintf(fp,"#   bg[PRELIGHT] = { 0.75 ,0.75, 1.0 }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"ruler\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#   font = '-adobe-helvetica-medium-r-normal--*-80-*-*-*-*-*-*'\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"curve\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#   fg[NORMAL] = { 58000, 0, 0 }			# red\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"red-bar\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"#   bg[PRELIGHT] = { 0.95, .55, 0.55 }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"# override testgtk2, introduce the green color in the button list\n");
	fprintf(fp,"style 'button_list' = 'button'\n");
	fprintf(fp,"{\n");
	fprintf(fp,"   font = \"-adobe-helvetica-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"   bg[PRELIGHT] = { 0, 0.75, 0x00 }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"main window.*GtkScrolledWindow.*GtkButton*\" style \"button_list\"\n");
	fprintf(fp,"# common default\n");
	fprintf(fp,"class \"GtkWidget\" style \"default\"\n");
	fprintf(fp,"class \"GtkScrollbar\" style \"red-bar\"\n");
	fprintf(fp,"widget_class \"GtkWindow\" style \"window\"\n");
	fprintf(fp,"widget_class \"GtkDialog\" style \"window\"\n");
	fprintf(fp,"widget_class \"GtkFileSelection\" style \"window\"\n");
	fprintf(fp,"widget_class \"*Gtk*Scale\" style \"scale\"\n");
	fprintf(fp,"widget_class \"*GtkCheckButton*\" style \"toggle_button\"\n");
	fprintf(fp,"widget_class \"*GtkRadioButton*\" style \"toggle_button\"\n");
	fprintf(fp,"widget_class \"*GtkButton*\" style \"button\"\n");
	fprintf(fp,"widget_class \"*Ruler\" style \"ruler\"\n");
	fprintf(fp,"widget_class \"*GtkText\" style \"text\"\n");
	fprintf(fp,"widget \"*GtkCurve\" style \"curve\"\n");
	fprintf(fp,"binding \"test1\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"  bind \"<ctrl>1\" {\n");
	fprintf(fp,"    \"debug-msg\" (\"hallo and\") \n");
	fprintf(fp,"    \"debug-msg\" (\"huhu\")\n");
	fprintf(fp,"  }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"binding \"test2\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"  bind \"<ctrl>1\" {\n");
	fprintf(fp,"    \"debug-msg\" (\"jup!\") \n");
	fprintf(fp,"  }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"# possible priorities are (in ascending order):\n");
	fprintf(fp,"# lowest\n");
	fprintf(fp,"# gtk         (used by gtk for internal class bindings)\n");
	fprintf(fp,"# application (for hard coded bindings on application basis)\n");
	fprintf(fp,"# rc          (used implicitel by rc files)\n");
	fprintf(fp,"# highest\n");
	fprintf(fp,"class \"GtkButton\" binding \"test1\"           # implicit : rc\n");
	fprintf(fp,"class \"GtkButton\" binding : highest \"test2\" # override \"rc\" priority\n");
	fprintf(fp,"binding \"clist-test\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"  bind \"j\" {\n");
	fprintf(fp,"    \"scroll-vertical\" (step-backward, 0.0)\n");
	fprintf(fp,"  }\n");
	fprintf(fp,"  bind \"k\" {\n");
	fprintf(fp,"    \"scroll-vertical\" (step-forward, 0.0)\n");
	fprintf(fp,"  }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"class \"GtkCList\" binding \"clist-test\"\n");
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"RDA_Main_Menu\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {0.80, 0.80, 0.80}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT.UTILITIES.\" style \"RDA_Main_Menu\"\n");
	fprintf(fp,"style \"RDAWindowDefault\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"##	bg[NORMAL] = {0.31, 0.70, 0.67}\n");
	fprintf(fp,"	bg[NORMAL] = {0.80, 0.80, 0.80}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"class \"GtkWindow\" style \"RDAWindowDefault\"\n");
	fprintf(fp,"class \"GtkScrolledWindow\" style \"RDAWindowDefault\"\n");
	fprintf(fp,"class \"GtkViewport\" style \"RDAWindowDefault\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"RDASaveWarning\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {1.00, 0.00, 0.00}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*SAVE WARNING DIALOG*\" style \"RDASaveWarning\"\n");
	fprintf(fp,"widget \"XPERT*QUIT WARNING DIALOG*\" style \"RDASaveWarning\"\n");
	fprintf(fp,"widget \"XPERT*ERROR DIALOG SCREEN*\" style \"RDASaveWarning\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"RDA_Labels\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"##	fg[NORMAL] = {1.00, 1.00, 1.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"class \"GtkLabel\" style \"RDA_Labels\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"RDA_ScrollBar\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"##	bg[NORMAL] = {0.31, 0.70, 0.73}\n");
	fprintf(fp,"	bg[NORMAL] = {0.00, 0.00, 1.00}\n");
	fprintf(fp,"##	bg[PRELIGHT] = {1.00, 0.00, 0.00}\n");
	fprintf(fp,"	bg[PRELIGHT] = {0.38, 0.70, 1.00}\n");
	fprintf(fp,"	bg[SELECTED] = {0.00, 0.00, 1.00}\n");
	fprintf(fp,"}\n");
	fprintf(fp,"class \"GtkScrollbar\" style \"RDA_ScrollBar\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"RDADefaultButton\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"##	bg[NORMAL] = {1.00, 0.92, 0.80}\n");
	fprintf(fp,"	bg[NORMAL] = {0.70, 0.70, 0.70}\n");
	fprintf(fp,"##	bg[PRELIGHT] = {0.00, 0.00, 1.00}\n");
	fprintf(fp,"	bg[PRELIGHT] = {0.38, 0.70, 1.00}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
	fprintf(fp,"	bg[SELECTED] = {0.00, 0.00, 1.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"class \"GtkButton\" style \"RDADefaultButton\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"defaultwidget\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	fg[INSENSITIVE] = {0.80, 0.80, 0.80}\n");
	fprintf(fp,"	bg[INSENSITIVE] = {0.80, 0.80, 0.80}\n");
	fprintf(fp,"	bg[SELECTED] = {0.00, 0.00, 1.00}\n");
	fprintf(fp,"	bg[NORMAL] = {1.00, 1.00, 1.00}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"##	bg[PRELIGHT] = {0.00, 0.00, 1.00}\n");
	fprintf(fp,"	bg[PRELIGHT] = {0.38, 0.70, 1.00}\n");
	fprintf(fp,"}\n");
	fprintf(fp,"class \"GtkCombo\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkEntry\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkPlaintext\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkFormattedtext\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkUnsignednumber\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkUnsigneddecimal\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkSignednumber\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkSigneddecimal\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkRDAList\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkCList\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkCurrency100\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkCurrencync\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkRDAText\" style \"defaultwidget\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"toggle_button\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg_pixmap[ACTIVE] = \"FILLED.xpm\"\n");
	fprintf(fp,"	bg_pixmap[NORMAL] = \"NONE.xpm\"\n");
	fprintf(fp,"	bg[SELECTED] = {0.00, 0.00, 1.00}\n");
	fprintf(fp,"##	bg[NORMAL] = {0.31, 0.70, 0.73}\n");
	fprintf(fp,"	bg[NORMAL] = {0.80, 0.80, 0.80}\n");
	fprintf(fp,"	bg[ACTIVE] = {1.00, 0.00, 0.00}\n");
	fprintf(fp,"}\n");
	fprintf(fp,"class \"*GtkCheckButton*\" style \"toggle_button\"\n");
	fprintf(fp,"class \"*GtkToggleButton*\" style \"toggle_button\"\n");
	fprintf(fp,"class \"*GtkRDACheckButton*\" style \"toggle_button\"\n");
	fprintf(fp,"class \"*GtkRDAToggleButton*\" style \"toggle_button\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"RDA_QuitButton\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {0.98, 0.97, 0.27}\n");
	fprintf(fp,"	bg[PRELIGHT] = {0.38, 0.70, 1.00}\n");
	fprintf(fp,"##	bg[PRELIGHT] = {0.00, 0.00, 1.00}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*QUIT\" style \"RDA_QuitButton\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"RDA_DeleteButton\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {1.00, 0.00, 0.00}\n");
	fprintf(fp,"##	bg[PRELIGHT] = {0.00, 0.00, 1.00}\n");
	fprintf(fp,"	bg[PRELIGHT] = {0.38, 0.70, 1.00}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*.DELETE\" style \"RDA_DeleteButton\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"RDA_HelpButton\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {0.73, 0.48, 0.00}\n");
	fprintf(fp,"##	bg[PRELIGHT] = {0.00, 0.00, 1.00}\n");
	fprintf(fp,"	bg[PRELIGHT] = {0.38, 0.70, 1.00}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*.HELP\" style \"RDA_HelpButton\"\n");
	fprintf(fp,"widget \"XPERT*.CYBERSUPPORT\" style \"RDA_HelpButton\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"RDA_AddButton\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {0.79, 0.85, 0.94}\n");
	fprintf(fp,"##	bg[PRELIGHT] = {0.00, 0.00, 1.00}\n");
	fprintf(fp,"	bg[PRELIGHT] = {0.38, 0.70, 1.00}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*.ADD\" style \"RDA_AddButton\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"RDA_SelectButton\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {0.00, 1.00, 0.00}\n");
	fprintf(fp,"##	bg[PRELIGHT] = {0.00, 0.00, 1.00}\n");
	fprintf(fp,"	bg[PRELIGHT] = {0.38, 0.70, 1.00}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*SELECT\" style \"RDA_SelectButton\"\n");
	fprintf(fp,"widget \"XPERT*SELECT ALL\" style \"RDA_SelectButton\"\n");
	fprintf(fp,"widget \"XPERT*SAVE\" style \"RDA_SelectButton\"\n");
	fprintf(fp,"widget \"XPERT*SAVE NO UPDATE\" style \"RDA_SelectButton\"\n");
	fprintf(fp,"widget \"XPERT*CONTINUE\" style \"RDA_SelectButton\"\n");
	fprintf(fp,"##\n");
	fprintf(fp,"style \"RDAOUTwidget\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	fg[INSENSITIVE] = {0.80, 0.80, 0.80}\n");
	fprintf(fp,"	bg[INSENSITIVE] = {0.80, 0.80, 0.80}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-*-r-normal-*-*-60-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-*-r-normal-*-*-60-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-60-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-60-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT.UTILITIES.TEXT*BROWSER.TEXT\" style \"RDAOUTwidget\"\n");
	fprintf(fp,"style \"BUBBLEHELP\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	fg[INSENSITIVE] = {0.00, 0.00, 1.00}\n");
	fprintf(fp,"	bg[NORMAL] = {0.98, 0.97, 0.27}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-150-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-150-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"	font = \"-*-fixed-medium-r-normal-*-*-140-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-fixed-medium-r-normal-*-*-140-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"*BUBBLE*HELP*\" style \"BUBBLEHELP\"\n");
	fclose(fp);
}
void SCRIPT_xpertrc_new()
{
	FILE *fp=NULL;
	char *temp=NULL;

	temp=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+RDAstrlen("xpertrc")+15);
#ifdef WIN32
	sprintf(temp,"%s\\rda\\xpertrc",CURRENTDIRECTORY);
#endif
#ifndef WIN32
	sprintf(temp,"%s/rda/xpertrc",CURRENTDIRECTORY);
#endif
	fp=fopen(temp,"w+b");

	fprintf(fp,"module_path \"/rda/xpgms:/rda/xpgmsgtk\"\n");
	fprintf(fp,"pixmap_path \"./pixmaps\"\n");
	fprintf(fp,"style \"default\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"  bg[NORMAL] = \"#eeeacd\"\n");
	fprintf(fp,"  bg[PRELIGHT] = \"#b6a3d7\"\n");
	fprintf(fp,"  bg[SELECTED] = \"#4f2d7f\"\n");
	fprintf(fp,"  bg[INSENSITIVE] = \"#eeeacd\"\n");
	fprintf(fp,"  bg[ACTIVE] = \"#4f2d7f\"\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"  font = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"  fontset = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"  engine \"redmond95\" \n");
	fprintf(fp,"  {\n");
	fprintf(fp,"  }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"style \"RDAScreenHelp\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = \"#4f2d7f\"\n");
	fprintf(fp,"	fg[NORMAL] = \"#eeeacd\" \n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"  font = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"  fontset = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*SCREEN HELP\" style \"RDAScreenHelp\"\n");
	fprintf(fp,"widget \"XPERT*RESOURCE HELP\" style \"RDAScreenHelp\"\n");
	fprintf(fp,"style 'button_list' = 'button'\n");
	fprintf(fp,"{\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-adobe-helvetica-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"   font = \"-adobe-helvetica-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"   bg[NORMAL] = \"#ebebc8\"\n");
	fprintf(fp,"   bg[PRELIGHT] = \"#b6a3d7\"\n");
	fprintf(fp,"   fg[NORMAL] = { 0.00, 0.00, 0.00 }\n");
	fprintf(fp,"   bg[INSENSITIVE] = \"#eeeacd\"\n");
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"main window.*GtkScrolledWindow.*GtkButton*\" style \"button_list\"\n");
	fprintf(fp,"# common default\n");
	fprintf(fp,"class \"GtkWidget\" style \"default\"\n");
	fprintf(fp,"binding \"test1\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"  bind \"<ctrl>1\" {\n");
	fprintf(fp,"    \"debug-msg\" (\"hallo and\") \n");
	fprintf(fp,"    \"debug-msg\" (\"huhu\")\n");
	fprintf(fp,"  }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"binding \"test2\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"  bind \"<ctrl>1\" {\n");
	fprintf(fp,"    \"debug-msg\" (\"jup!\") \n");
	fprintf(fp,"  }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"# possible priorities are (in ascending order):\n");
	fprintf(fp,"# lowest\n");
	fprintf(fp,"# gtk         (used by gtk for internal class bindings)\n");
	fprintf(fp,"# application (for hard coded bindings on application basis)\n");
	fprintf(fp,"# rc          (used implicitel by rc files)\n");
	fprintf(fp,"# highest\n");
	fprintf(fp,"class \"GtkButton\" binding \"test1\"           # implicit : rc\n");
	fprintf(fp,"class \"GtkButton\" binding : highest \"test2\" # override \"rc\" priority\n");
	fprintf(fp,"binding \"clist-test\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"  bind \"j\" {\n");
	fprintf(fp,"    \"scroll-vertical\" (step-backward, 0.0)\n");
	fprintf(fp,"  }\n");
	fprintf(fp,"  bind \"k\" {\n");
	fprintf(fp,"    \"scroll-vertical\" (step-forward, 0.0)\n");
	fprintf(fp,"  }\n");
	fprintf(fp,"}\n");
	fprintf(fp,"class \"GtkCList\" binding \"clist-test\"\n");
	fprintf(fp,"style \"RDAWindowDefault\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {0.83, 0.83, 0.83}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
	fprintf(fp,"}\n");
	fprintf(fp,"class \"GtkWindow\" style \"RDAWindowDefault\"\n");
	fprintf(fp,"class \"GtkScrolledWindow\" style \"RDAWindowDefault\"\n");
	fprintf(fp,"class \"GtkViewport\" style \"RDAWindowDefault\"\n");
	fprintf(fp,"style \"RDASaveWarning\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {0.83, 0.83, 0.83}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"  font = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"  fontset = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*SAVE WARNING DIALOG*\" style \"RDASaveWarning\"\n");
	fprintf(fp,"widget \"XPERT*QUIT WARNING DIALOG*\" style \"RDASaveWarning\"\n");
	fprintf(fp,"widget \"XPERT*ERROR DIALOG SCREEN*\" style \"RDASaveWarning\"\n");
	fprintf(fp,"style \"RDA_Labels\"\n");
	fprintf(fp,"{\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"  font = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"  fontset = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"class \"GtkLabel\" style \"RDA_Labels\"\n");
	fprintf(fp,"style \"RDA_ScrollBar\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = {0.83, 0.83, 0.83}\n");
	fprintf(fp,"        bg[PRELIGHT] = \"#b6a3d7\"\n");
	fprintf(fp,"}\n");
	fprintf(fp,"class \"GtkScrollbar\" style \"RDA_ScrollBar\"\n");
	fprintf(fp,"style \"RDADefaultButton\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"   	bg[NORMAL] = \"#ebebc8\"\n");
	fprintf(fp,"        bg[PRELIGHT] = \"#b6a3d7\"\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"  font = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"  fontset = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"class \"GtkButton\" style \"RDADefaultButton\"\n");
	fprintf(fp,"style \"defaultwidget\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	fg[INSENSITIVE] = {0.31, 0.70, 0.73}\n");
	fprintf(fp,"	bg[INSENSITIVE] = {0.31, 0.70, 0.73}\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
	fprintf(fp,"	bg[NORMAL] = {1.00, 1.00, 1.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"  font = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"  fontset = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"        bg[PRELIGHT] = \"#b6a3d7\"\n");
	fprintf(fp,"}\n");
	fprintf(fp,"class \"GtkCombo\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkEntry\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkPlaintext\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkFormattedtext\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkUnsignednumber\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkUnsigneddecimal\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkSignednumber\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkSigneddecimal\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkRDAList\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkCList\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkCurrency100\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkCurrencync\" style \"defaultwidget\"\n");
	fprintf(fp,"class \"GtkRDAText\" style \"defaultwidget\"\n");
	fprintf(fp,"style \"RDA_QuitButton\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = \"#f7fc6c\"\n");
	fprintf(fp,"        bg[PRELIGHT] = \"#b6a3d7\"\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"  font = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"  fontset = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*QUIT\" style \"RDA_QuitButton\"\n");
	fprintf(fp,"style \"RDA_DeleteButton\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = \"#a52020\"\n");
	fprintf(fp,"        bg[PRELIGHT] = \"#b6a3d7\"\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"  font = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"  fontset = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*.DELETE\" style \"RDA_DeleteButton\"\n");
	fprintf(fp,"style \"RDA_HelpButton\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = \"#4f2d7f\"\n");
	fprintf(fp,"        bg[PRELIGHT] = \"#b6a3d7\"\n");
	fprintf(fp,"	fg[NORMAL] = \"#eeeacd\" \n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"  font = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"  fontset = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*.HELP\" style \"RDA_HelpButton\"\n");
	fprintf(fp,"widget \"XPERT*.CYBERSUPPORT\" style \"RDA_HelpButton\"\n");
	fprintf(fp,"style \"RDA_AddButton\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = \"#95b9d6\"\n");
	fprintf(fp,"        bg[PRELIGHT] = \"#b6a3d7\"\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"  font = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"  fontset = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*.ADD\" style \"RDA_AddButton\"\n");
	fprintf(fp,"style \"RDA_SelectButton\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	bg[NORMAL] = \"#8e9300\"\n");
	fprintf(fp,"        bg[PRELIGHT] = \"#b6a3d7\"\n");
	fprintf(fp,"	fg[NORMAL] = {0.00, 0.00, 0.00}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-120-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"  font = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
	fprintf(fp,"  fontset = \"-*-fixed-medium-r-normal--*-120-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT*SELECT\" style \"RDA_SelectButton\"\n");
	fprintf(fp,"widget \"XPERT*SELECT ALL\" style \"RDA_SelectButton\"\n");
	fprintf(fp,"widget \"XPERT*SAVE\" style \"RDA_SelectButton\"\n");
	fprintf(fp,"widget \"XPERT*SAVE NO UPDATE\" style \"RDA_SelectButton\"\n");
	fprintf(fp,"widget \"XPERT*CONTINUE\" style \"RDA_SelectButton\"\n");
	fprintf(fp,"style \"RDAOUTwidget\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	fg[INSENSITIVE] = {0.31, 0.70, 0.73}\n");
	fprintf(fp,"	bg[INSENSITIVE] = {0.31, 0.70, 0.73}\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-60-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-60-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"  font = \"-*-fixed-medium-r-normal--*-60-*-*-*-*-*-*\"\n");
	fprintf(fp,"  fontset = \"-*-fixed-medium-r-normal--*-60-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"XPERT.UTILITIES.TEXT BROWSER.TEXT\" style \"RDAOUTwidget\"\n");
	fprintf(fp,"style \"BUBBLEHELP\"\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	fg[INSENSITIVE] = {0.00, 0.00, 0.00}\n");
	fprintf(fp,"	bg[NORMAL] = \"#f7fc6c\"\n");
#ifdef WIN32
	fprintf(fp,"	font = \"-*-courier new-medium-r-normal-*-*-150-*-*-*-*-*-*\"\n");
	fprintf(fp,"	fontset = \"-*-courier new-medium-r-normal-*-*-150-*-*-*-*-*-*\"\n");
#endif
#ifndef WIN32
	fprintf(fp,"  font = \"-*-fixed-medium-r-normal--*-140-*-*-*-*-*-*\"\n");
	fprintf(fp,"  fontset = \"-*-fixed-medium-r-normal--*-140-*-*-*-*-*-*\"\n");
#endif
	fprintf(fp,"}\n");
	fprintf(fp,"widget \"*BUBBLE*HELP*\" style \"BUBBLEHELP\"\n");
	fclose(fp);
	if(temp!=NULL) Rfree(temp);
}
