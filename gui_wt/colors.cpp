#include<gui.hpp>
#include<guip.hpp>

/* COLORS */
/* Green     RGB 55 57 20 HSV  64 66 57   Hex 8b9100 */
/* Purple    RGB 30 19 43 HSV 268 55 43   Hex 4f2d7f */
/* backgrd   RGB 84 82 84 HSV 300  1 84   Hex d5d2d5 */
/* Select    RGB  0 100 0 HSV 120 100 100 Hex 00ff00  */
/* button    RGB 100 92 80 HSV 35 20 100  Hex ebebc8 */
/* quit      RGB 100 100 0 HSV 60 100 100 Hex ffff00 */
/* input backRGB 100 95 90 HSV 29 10 100  Hex fff2e6 */

int currentRDAColorProfile=0;

/* Color Profiles 
 *
 * Test Directory Pink
 * Purple
 * Blue (Windows)
 * Black/Silver
 * Orange ffdf48
 * Teal
 * Blue
 * Gray
 */

char *BASE_Background[numRDAColorProfiles+1]={"#d5d2d5","#d5d2d5","#d5d2d5","#e9e8e8","#ffdf48","#7affe8","#02c0ff","#c0c0c0"};
char *BASE_Base[numRDAColorProfiles+1]={"#fff2e6","#fff2e6","#fff2e6","#f1f2f1","#fff2e6","#fff2e6","#fff2e6","#fff2e6"};
char *BASE_Foreground[numRDAColorProfiles+1]={"#ff64bf","#4f2d7f","#3333a4","black","#6b300d","#022720","#08042b","#040505"};
char *BASE_Highlight[numRDAColorProfiles+1]={"#ff64bf","#4f2d7f","#3333a4","black","#6b300d","#022720","#08042b","#3169c5"};
char *BASE_Button[numRDAColorProfiles+1]={"#ebebc8","#ebebc8","#ebebc8","#e9e8e8","#ffdf48","#7affe8","#02c0ff","#c0c0c0"};
char *BASE_HighlightedText[numRDAColorProfiles+1]={"white","white","white","white","white","white","white","white"};
char *BASE_ButtonText[numRDAColorProfiles+1]={"black","black","black","black","black","black","black","black"};
char *BASE_Text[numRDAColorProfiles+1]={"black","black","black","black","black","black","black","black"};
char *WARNING_Background[numRDAColorProfiles+1]={"#f7fc6c","#f7fc6c","#e5ea53","#e5ea53","#e5da53","#e5da53","#e5da53","#e5da53"};
char *WARNING_Base[numRDAColorProfiles+1]={"#fff2e6","#fff2e6","#fff2e6","#fff2e6","#fff2e6","#fff2e6","#fff2e6","#fff2e6"};
char *WARNING_Foreground[numRDAColorProfiles+1]={"#ff64bf","#4f2d7f","#3333a4","black","#6b300d","#022720","#08042b","#040505"};
char *WARNING_Highlight[numRDAColorProfiles+1]={"#ff64bf","#4f2d7f","#3333a4","black","#6b300d","#022720","#08042b","#040505"};
char *WARNING_Button[numRDAColorProfiles+1]={"#ebebc8","#ebebc8","#ebebc8","#ebebc8","#ebebc8","#ebebc8","#ebebc8","#ebebc8"};
char *WARNING_HighlightedText[numRDAColorProfiles+1]={"white","white","white","white","white","white","white","white"};
char *WARNING_ButtonText[numRDAColorProfiles+1]={"black","black","black","black","black","black","black","black"};
char *WARNING_Text[numRDAColorProfiles+1]={"black","black","black","black","black","black","black","black"};
char *ERROR_Background[numRDAColorProfiles+1]={"red","red","red","red","red","red","red","red"};
char *ERROR_Base[numRDAColorProfiles+1]={"#fff2e6","#fff2e6","#fff2e6","#f1f2f1","#fff2e6","#fff2e6","#fff2e6","#fff2e6"};
char *ERROR_Foreground[numRDAColorProfiles+1]={"#ff64bf","#4f2d7f","#3333a4","black","#6b300d","#022720","#08042b","#040505"};
char *ERROR_Highlight[numRDAColorProfiles+1]={"#ff64bf","#4f2d7f","#3333a4","black","#6b300d","#022720","#08042b","#040505"};
char *ERROR_Button[numRDAColorProfiles+1]={"#ebebc8","#ebebc8","#ebebc8","#ebebc8","#ebebc8","#ebebc8","#ebebc8","#ebebc8"};
char *ERROR_HighlightedText[numRDAColorProfiles+1]={"white","white","white","white","white","white","white","white"};
char *ERROR_ButtonText[numRDAColorProfiles+1]={"black","black","black","black","black","black","black","black"};
char *ERROR_Text[numRDAColorProfiles+1]={"black","black","black","black","black","black","black","black"};
char *HELP_Background[numRDAColorProfiles+1]={"#d5d2d5","#d5d2d5","#d5d2d5","#fcfcfc","#ffdf48","#7affe8","#02c0ff","#c0c0c0"};
char *HELP_Base[numRDAColorProfiles+1]={"#fff2e6","#fff2e6","#fff2e6","#f1f2f1","#fff2e6","#fff2e6","#fff2e6","#fff2e6"};
char *HELP_Foreground[numRDAColorProfiles+1]={"#ff64bf","#4f2d7f","#3333a4","black","#6b300d","#022720","#08042b","#040505"};
char *HELP_Highlight[numRDAColorProfiles+1]={"#ff64bf","#4f2d7f","#3333a4","black","#6b300d","#022720","#08042b","#040505"};
char *HELP_Button[numRDAColorProfiles+1]={"#ebebc8","#ebebc8","#ebebc8","#ebebc8","#ebebc8","#ebebc8","#ebebc8","#ebebc8"};
char *HELP_HighlightedText[numRDAColorProfiles+1]={"white","white","white","white","white","white","white","white"};
char *HELP_ButtonText[numRDAColorProfiles+1]={"black","black","black","black","black","black","black","black"};
char *HELP_Text[numRDAColorProfiles+1]={"black","black","black","black","black","black","black","black"};
char *BUTTON_HELP_ButtonText[numRDAColorProfiles+1]={"black","black","black","black","black","black","black","black"};
char *BUTTON_HELP_Text[numRDAColorProfiles+1]={"black","black","black","black","black","black","black","black"};
char *BUTTON_HELP_Button[numRDAColorProfiles+1]={"#4f2d7f","#4f2d7f","#3333a4","black","#6b300d","#022720","#08042b","#4f2d7f"};
char *BUTTON_SAVE_ButtonText[numRDAColorProfiles+1]={"black","black","black","black","black","black","black","black"};
char *BUTTON_SAVE_Text[numRDAColorProfiles+1]={"black","black","black","black","black","black","black","black"};
char *BUTTON_SAVE_Button[numRDAColorProfiles+1]={"#8e9300","#8e9300","#2eac2b","#2eac2b","#2eac2b","#2eac2b","#2eac2b","#008500"};
char *BUTTON_FASTSAVE_Button[numRDAColorProfiles+1]={"#5a601d","#5a601d","#5a601d","#5a601d","#5a601d","#5a601d","#5a601d","#5a601d"};
char *BUTTON_QUIT_ButtonText[numRDAColorProfiles+1]={"black","black","black","black","black","black","black","black"};
char *BUTTON_QUIT_Text[numRDAColorProfiles+1]={"black","black","black","black","black","black","black","black"};
char *BUTTON_QUIT_Button[numRDAColorProfiles+1]={"#f7fc6c","#f7fc6c","#e5ea53","#e5ea53","#e5ea53","#e5ea53","#e5ea53","#e5ea53"};
char *BUTTON_SELECT_ButtonText[numRDAColorProfiles+1]={"black","black","black","black","black","black","black","black"};
char *BUTTON_SELECT_Text[numRDAColorProfiles+1]={"black","black","black","black","black","black","black","black"};
char *BUTTON_SELECT_Button[numRDAColorProfiles+1]={"#cb792d","#cb792d","#cb8c2d","#b8944d","#b8944d","#b8944d","#b8944d","#b8944d"};
char *BUTTON_ADD_ButtonText[numRDAColorProfiles+1]={"black","black","black","black","black","black","black","black"};
char *BUTTON_ADD_Text[numRDAColorProfiles+1]={"black","black","black","black","black","black","black","black"};
char *BUTTON_ADD_Button[numRDAColorProfiles+1]={"#95b9d6","#95b9d6","#8da4c9","#657bea","#657bea","#657bea","#657bea","#3169c5"};
char *BUTTON_DELETE_ButtonText[numRDAColorProfiles+1]={"black","black","black","black","black","black","black","black"};
char *BUTTON_DELETE_Text[numRDAColorProfiles+1]={"black","black","black","black","black","black","black","black"};
char *BUTTON_DELETE_Button[numRDAColorProfiles+1]={"#a52020","#a52020","#c92727","#c92727","#c92727","#c92727","#c92727","#c92727"};

void xSetupWidgetColorPalette(RDArmem *member,int line,char *file)
{
	WCssStyleSheet *RStyle=NULL;
	RDArsrc *rsrc=member->parent;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG SetupWidgetColorPalette for [%s] at line [%d] program [%s].",(member->rscrname!=NULL ? member->rscrname:""),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
	if(member->field_type==BOOLNS)
	{
	} else if(member->field_type==SCROLLEDLIST)
	{
	} else if(member->field_type==SCROLLEDTEXT)
	{
	} else if(member->field_type==BUTTONS)
	{
	} else {
		SetupWindowColorPalette((Wt::WWidget *)member->w,rsrc->screen);
	}
}
void xSetupWindowColorPalette(Wt::WWidget *w,char *name,int line,char *file)
{
	char flip=FALSE;

#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG SetupWindowColorPalette for [%s] at line [%d] program [%s].",(name!=NULL ? name:""),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
}
void xSetupOtherWidgetColorPalette(Wt::WWidget *w,char *name,int line,char *file)
{
#ifdef USE_RDA_DIAGNOSTICS
	if(diaggui)
	{
		prterr("DIAG SetupOtherWidgetColorPalette for [%s] at line [%d] program [%s].",(name!=NULL ? name:""),line,file);
	}
#endif /* USE_RDA_DIAGNOSTICS */
/* ACTIVE */
}
void SetupStyleSheet(Wt::WTextEdit *TE)
{
}
