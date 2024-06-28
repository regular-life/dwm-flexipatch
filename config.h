/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int default_border = 1;  /* border pixel of windows */
static const unsigned int borderpx       = 1;  /* border pixel of windows */
static const unsigned int snap           = 10;  /* snap pixel */
static const unsigned int gappih         = 5;  /* horiz inner gap between windows */
static const unsigned int gappiv         = 5;  /* vert inner gap between windows */
static const unsigned int gappoh         = 10;  /* horiz outer gap between windows and screen edge */
static const unsigned int gappov         = 10;  /* vert outer gap between windows and screen edge */
static const int smartgaps_fact          = 1;   /* gap factor when there is only one client; 0 = no gaps, 3 = 3x outer gaps */
static const int showbar                 = 1;   /* 0 means no bar */
static const int topbar                  = 1;   /* 0 means bottom bar */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int showsystray             = 1;   /* 0 means no systray */
/* Indicators: see patch/bar_indicators.h for options */
static int tagindicatortype              = INDICATOR_TOP_LEFT_SQUARE;
static int tiledindicatortype            = INDICATOR_NONE;
static int floatindicatortype            = INDICATOR_TOP_LEFT_SQUARE;

static int fakefsindicatortype           = INDICATOR_PLUS;
static int floatfakefsindicatortype      = INDICATOR_PLUS_AND_LARGER_SQUARE;

static const char *fonts[]          = { "saucecodepro nerd font:size=9","siji:9", "MesloLGS NF:size=9","font awesome:size=9"};
static const char dmenufont[]            = "saucecodepro nerd font:size=9";

#include "color.h"
// static char normfgcolor[]                = "#ededed";
// static char normbgcolor[]                = "#000000";
// static char normbordercolor[]            = "#0f0d0d";
// static char normfloatcolor[]             = "#db8fd9";

// static char selfgcolor[]                 = "#eeeeee";
// static char selbgcolor[]                 = "#ab1916";
// static char selbordercolor[]             = "#ab1916";
// static char selfloatcolor[]              = "#ab1916";

// static char titlenormfgcolor[]           = "#ededed";
// static char titlenormbgcolor[]           = "#000000";
// static char titlenormbordercolor[]       = "#0f0d0d";
// static char titlenormfloatcolor[]        = "#db8fd9";

// static char titleselfgcolor[]            = "#eeeeee";
// static char titleselbgcolor[]            = "#ab1916";
// static char titleselbordercolor[]        = "#ab1916";
// static char titleselfloatcolor[]         = "#ab1916";

// static char tagsnormfgcolor[]            = "#ededed";
// static char tagsnormbgcolor[]            = "#000000";
// static char tagsnormbordercolor[]        = "#0f0d0d";
// static char tagsnormfloatcolor[]         = "#db8fd9";

// static char tagsselfgcolor[]             = "#eeeeee";
// static char tagsselbgcolor[]             = "#ab1916";
// static char tagsselbordercolor[]         = "#ab1916";
// static char tagsselfloatcolor[]          = "#ab1916";

// static char hidfgcolor[]                 = "#ab1916";
// static char hidbgcolor[]                 = "#000000";
// static char hidbordercolor[]             = "#ab1916";
// static char hidfloatcolor[]              = "#f76e0c";

// static char urgfgcolor[]                 = "#ededed";
// static char urgbgcolor[]                 = "#000000";
// static char urgbordercolor[]             = "#ff0000";
// static char urgfloatcolor[]              = "#db8fd9";


static const unsigned int baralpha = OPAQUE;
static const unsigned int borderalpha = OPAQUE;
static const unsigned int alphas[][3] = {
	/*                       fg      bg        border     */
	[SchemeNorm]         = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]          = { OPAQUE, baralpha, borderalpha },
	[SchemeTitleNorm]    = { OPAQUE, baralpha, borderalpha },
	[SchemeTitleSel]     = { OPAQUE, baralpha, borderalpha },
	[SchemeTagsNorm]     = { OPAQUE, baralpha, borderalpha },
	[SchemeTagsSel]      = { OPAQUE, baralpha, borderalpha },
	[SchemeHidNorm]          = { OPAQUE, baralpha, borderalpha },
	[SchemeHidSel]          = { OPAQUE, baralpha, borderalpha },
	[SchemeUrg]          = { OPAQUE, baralpha, borderalpha },
};

static char *colors[][ColCount] = {
	/*                       fg                bg                border                float */
	[SchemeNorm]         = { normfgcolor,      normbgcolor,      normbordercolor,      normfloatcolor },
	[SchemeSel]          = { selfgcolor,       selbgcolor,       selbordercolor,       selfloatcolor },
	[SchemeTitleNorm]    = { titlenormfgcolor, titlenormbgcolor, titlenormbordercolor, titlenormfloatcolor },
	[SchemeTitleSel]     = { titleselfgcolor,  titleselbgcolor,  titleselbordercolor,  titleselfloatcolor },
	[SchemeTagsNorm]     = { tagsnormfgcolor,  tagsnormbgcolor,  tagsnormbordercolor,  tagsnormfloatcolor },
	[SchemeTagsSel]      = { tagsselfgcolor,   tagsselbgcolor,   tagsselbordercolor,   tagsselfloatcolor },
	[SchemeHidNorm]          = { hidfgcolor,       hidbgcolor,       hidbordercolor,       hidfloatcolor },
	[SchemeHidSel]          = { hidfgcolor,       hidbgcolor,       hidbordercolor,       hidfloatcolor },
	[SchemeUrg]          = { urgfgcolor,       urgbgcolor,       urgbordercolor,       urgfloatcolor },
};



#if SCRATCHPADS_PATCH
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL };
static Sp scratchpads[] = {
   /* name          cmd  */
   {"spterm",      spcmd1},
};
#endif // SCRATCHPADS_PATCH


/* Tags
 * In a traditional dwm the number of tags in use can be changed simply by changing the number
 * of strings in the tags array. This build does things a bit different which has some added
 * benefits. If you need to change the number of tags here then change the NUMTAGS macro in dwm.c.
 *
 * Examples:
 *
 *  1) static char *tagicons[][NUMTAGS*2] = {
 *         [DEFAULT_TAGS] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "G", "H", "I" },
 *     }
 *
 *  2) static char *tagicons[][1] = {
 *         [DEFAULT_TAGS] = { "•" },
 *     }
 *
 * The first example would result in the tags on the first monitor to be 1 through 9, while the
 * tags for the second monitor would be named A through I. A third monitor would start again at
 * 1 through 9 while the tags on a fourth monitor would also be named A through I. Note the tags
 * count of NUMTAGS*2 in the array initialiser which defines how many tag text / icon exists in
 * the array. This can be changed to *3 to add separate icons for a third monitor.
 *
 * For the second example each tag would be represented as a bullet point. Both cases work the
 * same from a technical standpoint - the icon index is derived from the tag index and the monitor
 * index. If the icon index is is greater than the number of tag icons then it will wrap around
 * until it an icon matches. Similarly if there are two tag icons then it would alternate between
 * them. This works seamlessly with alternative tags and alttagsdecoration patches.
 */
static char *tagicons[][NUMTAGS] = {
//  [DEFAULT_TAGS]        = { "1", "2", "3", "4", "5", "6", "7", "8", "9" },
	[DEFAULT_TAGS]        = { "", "", "", "", "", "", "", "8", "9" },
	[ALTERNATIVE_TAGS]    = { "A", "B", "C", "D", "E", "F", "G", "H", "I" },
	[ALT_TAGS_DECORATION] = { "<1>", "<2>", "<3>", "<4>", "<5>", "<6>", "<7>", "<8>", "<9>" },
};
/* There are two options when it comes to per-client rules:
 *  - a typical struct table or
 *  - using the RULE macro
 *
 * A traditional struct table looks like this:
 *    // class      instance  title  wintype  tags mask  isfloating  monitor
 *    { "Gimp",     NULL,     NULL,  NULL,    1 << 4,    0,          -1 },
 *    { "Firefox",  NULL,     NULL,  NULL,    1 << 7,    0,          -1 },
 *
 * The RULE macro has the default values set for each field allowing you to only
 * specify the values that are relevant for your rule, e.g.
 *
 *    RULE(.class = "Gimp", .tags = 1 << 4)
 *    RULE(.class = "Firefox", .tags = 1 << 7)
 *
 * Refer to the Rule struct definition for the list of available fields depending on
 * the patches you enable.
 */
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 *	WM_WINDOW_ROLE(STRING) = role
	 *	_NET_WM_WINDOW_TYPE(ATOM) = wintype
	 */
	RULE(.wintype = WTYPE "DIALOG", .isfloating = 1)
	RULE(.wintype = WTYPE "UTILITY", .isfloating = 1)
	RULE(.wintype = WTYPE "TOOLBAR", .isfloating = 1)
	RULE(.wintype = WTYPE "SPLASH", .isfloating = 1)
	RULE(.class = "flameshot", .isfloating = 1)
	RULE(.class = "Gimp", .isfloating = 1)
	RULE(.class = "Firefox", .tags = 0)
	#if SCRATCHPADS_PATCH
	RULE(.instance = "spterm", .tags = SPTAG(0), .isfloating = 1)
	#endif // SCRATCHPADS_PATCH
};



/* Bar rules allow you to configure what is shown where on the bar, as well as
 * introducing your own bar modules.
 *
 *    monitor:
 *      -1  show on all monitors
 *       0  show on monitor 0
 *      'A' show on active monitor (i.e. focused / selected) (or just -1 for active?)
 *    bar - bar index, 0 is default, 1 is extrabar
 *    alignment - how the module is aligned compared to other modules
 *    widthfunc, drawfunc, clickfunc - providing bar module width, draw and click functions
 *    name - does nothing, intended for visual clue and for logging / debugging
 */
static const BarRule barrules[] = {
    /* monitor  bar    alignment         widthfunc        drawfunc        clickfunc        hoverfunc    name */
    { -1,       0,     BAR_ALIGN_LEFT,   width_tags,      draw_tags,      click_tags,      NULL,        "tags" },
    {  0,       0,     BAR_ALIGN_RIGHT,  width_systray,   draw_systray,   click_systray,   NULL,        "systray" },
    { -1,       0,     BAR_ALIGN_LEFT,   width_ltsymbol,  draw_ltsymbol,  click_ltsymbol,  NULL,        "layout" },
    { 'A',      0,     BAR_ALIGN_RIGHT,  width_status2d,  draw_status2d,  click_status2d,  NULL,        "status2d" },
    { -1,       0,     BAR_ALIGN_NONE,   width_awesomebar,draw_awesomebar,click_awesomebar,NULL,        "awesomebar" },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int nstack      = 0;    /* number of clients in primary stack area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */



static const Layout layouts[] = {
	/* symbol     arrange function, { nmaster, nstack, layout, master axis, stack axis, secondary stack axis, symbol func } */
	{ "[]=",      flextile,         { -1, -1, SPLIT_VERTICAL, TOP_TO_BOTTOM, TOP_TO_BOTTOM, 0, NULL } }, // default tile layout
 	// { "><>",      NULL,             {0} },    /* no layout function means floating behavior */
	{ "[M]",      flextile,         { -1, -1, NO_SPLIT, MONOCLE, MONOCLE, 0, NULL } }, // monocle
	{ "TTT",      flextile,         { -1, -1, SPLIT_HORIZONTAL, LEFT_TO_RIGHT, LEFT_TO_RIGHT, 0, NULL } }, // bstack
	// { "|||",      flextile,         { -1, -1, SPLIT_VERTICAL, LEFT_TO_RIGHT, TOP_TO_BOTTOM, 0, NULL } }, // columns (col) layout
	// { ">M>",      flextile,         { -1, -1, FLOATING_MASTER, LEFT_TO_RIGHT, LEFT_TO_RIGHT, 0, NULL } }, // floating master
	// { "[D]",      flextile,         { -1, -1, SPLIT_VERTICAL, TOP_TO_BOTTOM, MONOCLE, 0, NULL } }, // deck
	// { "===",      flextile,         { -1, -1, SPLIT_HORIZONTAL, LEFT_TO_RIGHT, TOP_TO_BOTTOM, 0, NULL } }, // bstackhoriz
	// { "|M|",      flextile,         { -1, -1, SPLIT_CENTERED_VERTICAL, LEFT_TO_RIGHT, TOP_TO_BOTTOM, TOP_TO_BOTTOM, NULL } }, // centeredmaster
	// { "-M-",      flextile,         { -1, -1, SPLIT_CENTERED_HORIZONTAL, TOP_TO_BOTTOM, LEFT_TO_RIGHT, LEFT_TO_RIGHT, NULL } }, // centeredmaster horiz
	// { ":::",      flextile,         { -1, -1, NO_SPLIT, GAPPLESSGRID, GAPPLESSGRID, 0, NULL } }, // gappless grid
	// { "[\\]",     flextile,         { -1, -1, NO_SPLIT, DWINDLE, DWINDLE, 0, NULL } }, // fibonacci dwindle
	// { "(@)",      flextile,         { -1, -1, NO_SPLIT, SPIRAL, SPIRAL, 0, NULL } }, // fibonacci spiral
	// { "[T]",      flextile,         { -1, -1, SPLIT_VERTICAL, LEFT_TO_RIGHT, TATAMI, 0, NULL } }, // tatami mats
	// { "[]=",      tile,             {0} },
	// { "[M]",      monocle,          {0} },
	// { "TTT",      bstack,           {0} },
	/* { NULL,       NULL,             {0} }, */
};


/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },



/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {
	"dmenu_run",
	"-m", dmenumon,
	"-fn", dmenufont,
	"-nb", normbgcolor,
	"-nf", normfgcolor,
	"-sb", selbgcolor,
	"-sf", selfgcolor,
	NULL
};
static const char *termcmd[]  = { "st", NULL };
static const char *firefox[]  = { "firefox", NULL };
static const char *subl[]  = { "subl", NULL };
static const char *pcmanfm[]  = { "pcmanfm", NULL };
static const char *okular[]  = { "okular", NULL };
static const char *discord[]  = { "discord", NULL };
static const char *volinc[] = {"volume.sh", "up"};
static const char *voldec[] = {"volume.sh", "down"};
static const char *volmute[] = {"volume.sh",  "mute"};
static const char *plrplay[] = {"playerctl", "--player=mpd,firefox, vlc", "play-pause"};
static const char *plrnxt[] = {"playerctl", "--player=mpd,firefox, vlc", "next"};
static const char *plrprev[] = {"playerctl", "--player=mpd,firefox, vlc", "previous"};
// static const char *roficmd[]  = { "/home/sophos/.config/rofi/launchers/text/launcher.sh"};
// static const char *music[] = {"st", "-e" , "/home/sophos/.config/ncmpcpp/ncmpcpp-ueberzug/ncmpcpp-ueberzug"};
static const char *spotify[] = {"spotify-launcher", NULL};
static const char *switchWallpaper[] = {"forWallpaper"} ;
static const char *switchLockscreen[] = {"forLockscreen"} ;
static const char *suspend_and_lock[] = {"sh", "-c", "systemctl suspend && betterlockscreen -l", NULL};
static const char *brave[] = {"brave-browser-nightly", NULL };
static const char *news[] = {"st", "-e" ,"newsboat"};
//static const char *plrplay[] = {"playerctl", "play-pause"};
//static const char *plrnxt[] = {"playerctl",  "next"};
//static const char *plrprev[] = {"playerctl",  "previous"};
static Key keys[] = {
	/* modifier                     key            function                argument */
  
    { MODKEY|ShiftMask,             XK_b, 	       setborderpx,    			{.i = default_border } },
    { MODKEY|ShiftMask,             XK_minus, 	   setborderpx,    			{.i = -1 } },
    { MODKEY|ShiftMask,             XK_equal, 	   setborderpx,    			{.i = +1 } },
	#if SCRATCHPAD_ALT_1_PATCH
	{ MODKEY,                       XK_minus,      scratchpad_show,        {0} },
	{ MODKEY|ShiftMask,             XK_minus,      scratchpad_hide,        {0} },
    { MODKEY,                       XK_equal,      scratchpad_remove,      {0} },
	#elif SCRATCHPADS_PATCH
	{ MODKEY,                       XK_0,          view,                   {.ui = ~SPTAGMASK } },
	{ MODKEY|ShiftMask,             XK_0,          tag,                    {.ui = ~SPTAGMASK } },
	#else
	{ MODKEY,                       XK_0,          view,                   {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,          tag,                    {.ui = ~0 } },
	#endif // SCRATCHPAD_ALT_1_PATCH
	#if SCRATCHPADS_PATCH
	{ MODKEY|Mod1Mask,              XK_grave,      togglescratch,          {.ui = 0 } },
	{ MODKEY|ControlMask,           XK_grave,      setscratch,             {.ui = 0 } },
	{ MODKEY|ShiftMask,             XK_grave,      removescratch,          {.ui = 0 } },
	#endif // SCRATCHPADS_PATCH
	// { MODKEY,                       XK_p,           spawn, 		       {.v = music} },
	{ MODKEY,                       XK_n,           spawn, 		       {.v = news} },
	{ MODKEY|ShiftMask,             XK_y,           togglefakefullscreen,  {0} },
	{ 0,                            XK_F8,          spawn, 		       {.v = plrplay} },
	{ 0,                            XK_F7,          spawn, 		       {.v = plrprev} },
	{ 0,                            XK_F9,          spawn, 		       {.v = plrnxt} },
        

	{ 0,                            XF86XK_AudioRaiseVolume, spawn,        {.v = volinc   } },
    { 0,                            XF86XK_AudioLowerVolume, spawn,        {.v = voldec } },
	{ 0,                            XF86XK_AudioMute, spawn,               {.v = volmute } },
        
	{ MODKEY,                       XK_Return,     spawn, 		       		{.v = termcmd } },
	{ MODKEY,                       XK_grave,      cyclelayout, 	       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Return,     spawn,                  {.v = dmenucmd } },
  // { MODKEY|ShiftMask,             XK_Return,     spawn,                  {.v = roficmd} }, 
	{ MODKEY,	                	XK_f,          spawn,          	       {.v = firefox } },
	{ MODKEY,	                	XK_s,          spawn,  		       	   {.v = subl } },
	{ MODKEY,	                	XK_e,          spawn,          	       {.v = pcmanfm } },
	{ MODKEY,	                	XK_o,          spawn,                  {.v = okular } },
	{ MODKEY,	                	XK_d,          spawn,                  {.v = discord } },
	{ MODKEY, 					 	XK_p,          spawn,                  {.v = spotify } },
	{ MODKEY,             			XK_Escape,     spawn,                  {.v = suspend_and_lock }	},
	{ MODKEY|ControlMask,			XK_l,	       spawn,				   {.v = switchLockscreen} },
	{ MODKEY|ControlMask, 			XK_w, 		   spawn,			       {.v = switchWallpaper} },
	{ MODKEY, 						XK_r, 		   spawn,				   {.v = brave}},
	
	{ MODKEY,                       XK_b,          togglebar,              {0} },
	{ MODKEY,                       XK_j,          focusstack,             {.i = +1 } },
	{ MODKEY,                       XK_k,          focusstack,             {.i = -1 } },
	{ MODKEY,                       XK_Left,       focusdir,               {.i = 0 } }, // left
	{ MODKEY,                       XK_Right,      focusdir,               {.i = 1 } }, // right
	{ MODKEY,                       XK_Up,         focusdir,               {.i = 2 } }, // up
	{ MODKEY,                       XK_Down,       focusdir,               {.i = 3 } }, // down
	{ Mod1Mask,                      XK_Tab,        swapfocus,              {.i = -1 } },
	{ MODKEY,                       XK_i,          incnmaster,             {.i = +1 } },
	{ MODKEY,                       XK_d,          incnmaster,             {.i = -1 } },
	{ MODKEY|ControlMask,           XK_i,          incnstack,              {.i = +1 } },
	{ MODKEY|ControlMask,           XK_u,          incnstack,              {.i = -1 } },
	{ MODKEY,                       XK_h,          setmfact,               {.f = -0.05} },
	{ MODKEY,                       XK_l,          setmfact,               {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,          setcfact,               {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,          setcfact,               {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,          setcfact,               {0} },
	{ MODKEY|Mod4Mask,              XK_Down,       moveresize,             {.v = "0x 25y 0w 0h" } },
	{ MODKEY|Mod4Mask,              XK_Up,         moveresize,             {.v = "0x -25y 0w 0h" } },
	{ MODKEY|Mod4Mask,              XK_Right,      moveresize,             {.v = "25x 0y 0w 0h" } },
	{ MODKEY|Mod4Mask,              XK_Left,       moveresize,             {.v = "-25x 0y 0w 0h" } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_Down,       moveresize,             {.v = "0x 0y 0w 25h" } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_Up,         moveresize,             {.v = "0x 0y 0w -25h" } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_Right,      moveresize,             {.v = "0x 0y 25w 0h" } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_Left,       moveresize,             {.v = "0x 0y -25w 0h" } },
	{ MODKEY|ShiftMask,             XK_j,          movestack,              {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,          movestack,              {.i = -1 } },
	{ MODKEY|ShiftMask,              XK_u,          incrgaps,               {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_u,          incrgaps,               {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_i,          incrigaps,              {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_i,          incrigaps,              {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_o,          incrogaps,              {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_o,          incrogaps,              {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_6,          incrihgaps,             {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_6,          incrihgaps,             {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_7,          incrivgaps,             {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_7,          incrivgaps,             {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_8,          incrohgaps,             {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_8,          incrohgaps,             {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_9,          incrovgaps,             {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_9,          incrovgaps,             {.i = -1 } },
	{ Mod1Mask|ShiftMask,             XK_0,        togglegaps,             {0} },
	{ MODKEY|ShiftMask,    XK_0,          defaultgaps,            {0} },
	{ MODKEY,                       XK_Tab,        view,                   {0} },
	{ MODKEY|ControlMask,           XK_z,          showhideclient,         {0} },
	{ MODKEY, 			            XK_w,          killclient,             {0} },
	{ MODKEY|ShiftMask,             XK_q,          quit,                   {0} },
	#if RESTARTSIG_PATCH
	{ MODKEY|ControlMask|ShiftMask, XK_q,          quit,                   {1} },
	#endif // RESTARTSIG_PATCH
	{ MODKEY,                       XK_t,          setlayout,              {.v = &layouts[0]} },
	{ MODKEY,                       XK_m,          setlayout,              {.v = &layouts[1]} },
	{ MODKEY|ControlMask,           XK_t,          rotatelayoutaxis,       {.i = +1 } },   /* flextile, 1 = layout axis */
	{ MODKEY|ControlMask,           XK_Tab,        rotatelayoutaxis,       {.i = +2 } },   /* flextile, 2 = master axis */
	{ MODKEY|ControlMask|ShiftMask, XK_Tab,        rotatelayoutaxis,       {.i = +3 } },   /* flextile, 3 = stack axis */
	{ MODKEY|ControlMask|Mod1Mask,  XK_Tab,        rotatelayoutaxis,       {.i = +4 } },   /* flextile, 4 = secondary stack axis */
	{ MODKEY|Mod5Mask,              XK_t,          rotatelayoutaxis,       {.i = -1 } },   /* flextile, 1 = layout axis */
	{ MODKEY|Mod5Mask,              XK_Tab,        rotatelayoutaxis,       {.i = -2 } },   /* flextile, 2 = master axis */
	{ MODKEY|Mod5Mask|ShiftMask,    XK_Tab,        rotatelayoutaxis,       {.i = -3 } },   /* flextile, 3 = stack axis */
	{ MODKEY|Mod5Mask|Mod1Mask,     XK_Tab,        rotatelayoutaxis,       {.i = -4 } },   /* flextile, 4 = secondary stack axis */
	{ MODKEY|ControlMask,           XK_Return,     mirrorlayout,           {0} },          /* flextile, flip master and stack areas */
	{ MODKEY,                       XK_space,      setlayout,              {0} },
	{ MODKEY|ShiftMask,             XK_space,      togglefloating,         {0} },
	{ MODKEY,                       XK_0,          view,                   {.ui = ~0 } },
	{ MODKEY,                       XK_comma,      focusmon,               {.i = -1 } },
	{ MODKEY,                       XK_period,     focusmon,               {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,      tagmon,                 {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,     tagmon,                 {.i = +1 } },
	{ MODKEY|ControlMask,           XK_comma,      cyclelayout,            {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period,     cyclelayout,            {.i = +1 } },
	TAGKEYS(                        XK_1,                                  0)
	TAGKEYS(                        XK_2,                                  1)
	TAGKEYS(                        XK_3,                                  2)
	TAGKEYS(                        XK_4,                                  3)
	TAGKEYS(                        XK_5,                                  4)
	TAGKEYS(                        XK_6,                                  5)
	TAGKEYS(                        XK_7,                                  6)
	TAGKEYS(                        XK_8,                                  7)
	TAGKEYS(                        XK_9,                                  8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask           button          function        argument */
	{ ClkLtSymbol,          0,                   Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,                   Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,                   Button1,        togglewin,      {0} },
	{ ClkWinTitle,          0,                   Button3,        showhideclient, {0} },
	{ ClkWinTitle,          0,                   Button2,        zoom,           {0} },
	{ ClkStatusText,        0,                   Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,              Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,              Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,              Button3,        resizemouse,    {0} },
	{ ClkClientWin,         MODKEY|ShiftMask,    Button3,        dragcfact,      {0} },
	{ ClkClientWin,         MODKEY|ShiftMask,    Button1,        dragmfact,      {0} },
	{ ClkTagBar,            0,                   Button1,        view,           {0} },
	{ ClkTagBar,            0,                   Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,              Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,              Button3,        toggletag,      {0} },
};

// IPC_PATCH

 #if IPC_PATCH 
 static const char *ipcsockpath = "/tmp/dwm.sock"; 
 static IPCCommand ipccommands[] = { 
 	IPCCOMMAND( focusmon, 1, {ARG_TYPE_SINT} ), 
 	IPCCOMMAND( focusstack, 1, {ARG_TYPE_SINT} ), 
 	IPCCOMMAND( incnmaster, 1, {ARG_TYPE_SINT} ), 
 	IPCCOMMAND( killclient, 1, {ARG_TYPE_SINT} ), 
 	IPCCOMMAND( quit, 1, {ARG_TYPE_NONE} ), 
 	IPCCOMMAND( setlayoutsafe, 1, {ARG_TYPE_PTR} ), 
 	IPCCOMMAND( setmfact, 1, {ARG_TYPE_FLOAT} ), 
 	IPCCOMMAND( setstatus, 1, {ARG_TYPE_STR} ), 
 	IPCCOMMAND( tag, 1, {ARG_TYPE_UINT} ), 
 	IPCCOMMAND( tagmon, 1, {ARG_TYPE_UINT} ), 
 	IPCCOMMAND( togglebar, 1, {ARG_TYPE_NONE} ), 
 	IPCCOMMAND( togglefloating, 1, {ARG_TYPE_NONE} ), 
 	IPCCOMMAND( toggletag, 1, {ARG_TYPE_UINT} ), 
 	IPCCOMMAND( toggleview, 1, {ARG_TYPE_UINT} ), 
 	IPCCOMMAND( view, 1, {ARG_TYPE_UINT} ), 
 	IPCCOMMAND( zoom, 1, {ARG_TYPE_NONE} ), 
 	#if BAR_ALTERNATIVE_TAGS_PATCH 
 	IPCCOMMAND( togglealttag, 1, {ARG_TYPE_NONE} ), 
 	#endif // BAR_ALTERNATIVE_TAGS_PATCH 
 	#if BAR_TAGGRID_PATCH 
 	IPCCOMMAND( switchtag, 1, {ARG_TYPE_UINT} ), 
 	#endif // BAR_TAGGRID_PATCH 
 	#if CFACTS_PATCH 
 	IPCCOMMAND( setcfact, 1, {ARG_TYPE_FLOAT} ), 
 	#endif // CFACTS_PATCH 
 	#if CYCLELAYOUTS_PATCH 
 	IPCCOMMAND( cyclelayout, 1, {ARG_TYPE_SINT} ), 
 	#endif // CYCLELAYOUTS_PATCH 
 	#if EXRESIZE_PATCH 
 	IPCCOMMAND( explace, 1, {ARG_TYPE_UINT} ), 
 	IPCCOMMAND( togglehorizontalexpand, 1, {ARG_TYPE_SINT} ), 
 	IPCCOMMAND( toggleverticalexpand, 1, {ARG_TYPE_SINT} ), 
 	IPCCOMMAND( togglemaximize, 1, {ARG_TYPE_SINT} ), 
 	#endif // EXRESIZE_PATCH 
 	#if !FAKEFULLSCREEN_PATCH && FAKEFULLSCREEN_CLIENT_PATCH 
 	IPCCOMMAND( togglefakefullscreen, 1, {ARG_TYPE_NONE} ), 
 	#endif // FAKEFULLSCREEN_CLIENT_PATCH 
 	#if FLOATPOS_PATCH 
 	IPCCOMMAND( floatpos, 1, {ARG_TYPE_STR} ), 
 	#endif // FLOATPOS_PATCH 
 	#if FULLSCREEN_PATCH 
 	IPCCOMMAND( fullscreen, 1, {ARG_TYPE_NONE} ), 
 	#endif // FULLSCREEN_PATCH 
 	#if FLEXTILE_DELUXE_LAYOUT 
 	IPCCOMMAND( incnstack, 1, {ARG_TYPE_SINT} ), 
 	IPCCOMMAND( rotatelayoutaxis, 1, {ARG_TYPE_SINT} ), 
 	IPCCOMMAND( setlayoutaxisex, 1, {ARG_TYPE_SINT} ), 
 	IPCCOMMAND( mirrorlayout, 1, {ARG_TYPE_NONE} ), 
 	#endif // FLEXTILE_DELUXE_LAYOUT 
 	#if FOCUSURGENT_PATCH 
 	IPCCOMMAND( focusurgent, 1, {ARG_TYPE_NONE} ), 
 	#endif // FOCUSURGENT_PATCH 
 	#if FOCUSADJACENTTAG_PATCH 
 	IPCCOMMAND( viewtoleft, 1, {ARG_TYPE_NONE} ), 
 	IPCCOMMAND( viewtoright, 1, {ARG_TYPE_NONE} ), 
 	IPCCOMMAND( tagtoleft, 1, {ARG_TYPE_NONE} ), 
 	IPCCOMMAND( tagtoright, 1, {ARG_TYPE_NONE} ), 
 	IPCCOMMAND( tagandviewtoleft, 1, {ARG_TYPE_NONE} ), 
 	IPCCOMMAND( tagandviewtoright, 1, {ARG_TYPE_NONE} ), 
 	#endif // FOCUSADJACENTTAG_PATCH 
 	#if INPLACEROTATE_PATCH 
 	IPCCOMMAND( inplacerotate, 1, {ARG_TYPE_SINT} ), 
 	#endif // INPLACEROTATE_PATCH 
 	#if KEYMODES_PATCH 
 	IPCCOMMAND( setkeymode, 1, {ARG_TYPE_UINT} ), 
 	#endif // KEYMODES_PATCH 
 	#if MAXIMIZE_PATCH 
 	IPCCOMMAND( togglehorizontalmax, 1, {ARG_TYPE_NONE} ), 
 	IPCCOMMAND( toggleverticalmax, 1, {ARG_TYPE_NONE} ), 
 	IPCCOMMAND( togglemax, 1, {ARG_TYPE_NONE} ), 
 	#endif // MAXIMIZE_PATCH 
 	#if MPDCONTROL_PATCH 
 	IPCCOMMAND( mpdchange, 1, {ARG_TYPE_SINT} ), 
 	IPCCOMMAND( mpdcontrol, 1, {ARG_TYPE_NONE} ), 
 	#endif // MPDCONTROL_PATCH 
 	#if MOVEPLACE_PATCH 
 	IPCCOMMAND( moveplace, 1, {ARG_TYPE_UINT} ), 
 	#endif // MOVEPLACE_PATCH 
 	#if MOVERESIZE_PATCH 
 	IPCCOMMAND( moveresize, 1, {ARG_TYPE_STR} ), 
 	#endif // MOVERESIZE_PATCH 
 	#if RIODRAW_PATCH 
 	IPCCOMMAND( rioresize, 1, {ARG_TYPE_NONE} ), 
 	#endif // RIODRAW_PATCH 
 	#if PUSH_PATCH || PUSH_NO_MASTER_PATCH 
 	IPCCOMMAND( pushdown, 1, {ARG_TYPE_NONE} ), 
 	IPCCOMMAND( pushup, 1, {ARG_TYPE_NONE} ), 
 	#endif // PUSH_PATCH / PUSH_NO_MASTER_PATCH 
 	#if ROTATESTACK_PATCH 
 	IPCCOMMAND( rotatestack, 1, {ARG_TYPE_SINT} ), 
 	#endif // ROTATESTACK_PATCH 
 	#if SCRATCHPADS_PATCH 
 	IPCCOMMAND( togglescratch, 1, {ARG_TYPE_UINT} ), 
 	#endif // SCRATCHPADS_PATCH 
 	#if SELFRESTART_PATCH 
 	IPCCOMMAND( self_restart, 1, {ARG_TYPE_NONE} ), 
 	#endif // SELFRESTART_PATCH 
 	#if SETBORDERPX_PATCH 
 	IPCCOMMAND( setborderpx, 1, {ARG_TYPE_SINT} ), 
 	#endif // SETBORDERPX_PATCH 
 	#if SHIFTVIEW_PATCH 
 	IPCCOMMAND( shiftview, 1, {ARG_TYPE_SINT} ), 
 	#endif // SHIFTVIEW_PATCH 
 	#if SHIFTVIEW_CLIENTS_PATCH 
 	IPCCOMMAND( shiftviewclients, 1, {ARG_TYPE_SINT} ), 
 	#endif // SHIFTVIEW_CLIENTS_PATCH 
 	#if STACKER_PATCH 
 	IPCCOMMAND( pushstack, 1, {ARG_TYPE_SINT} ), 
 	#endif // STACKER_PATCH 
 	#if STICKY_PATCH 
 	IPCCOMMAND( togglesticky, 1, {ARG_TYPE_NONE} ), 
 	#endif // STICKY_PATCH 
 	#if SWAPFOCUS_PATCH && PERTAG_PATCH 
 	IPCCOMMAND( swapfocus, 1, {ARG_TYPE_SINT} ), 
 	#endif // SWAPFOCUS_PATCH 
 	#if SWITCHCOL_PATCH 
 	IPCCOMMAND( switchcol, 1, {ARG_TYPE_NONE} ), 
 	#endif // SWITCHCOL_PATCH 
 	#if TAGALLMON_PATCH 
 	IPCCOMMAND( tagallmon, 1, {ARG_TYPE_SINT} ), 
 	#endif // TAGALLMON_PATCH 
 	#if TAGOTHERMONITOR_PATCH 
 	IPCCOMMAND( tagnextmonex, 1, {ARG_TYPE_UINT} ), 
 	IPCCOMMAND( tagprevmonex, 1, {ARG_TYPE_UINT} ), 
 	#endif // TAGOTHERMONITOR_PATCH 
 	#if TAGSWAPMON_PATCH 
 	IPCCOMMAND( tagswapmon, 1, {ARG_TYPE_SINT} ), 
 	#endif // TAGSWAPMON_PATCH 
 	#if TOGGLEFULLSCREEN_PATCH 
 	IPCCOMMAND( togglefullscreen, 1, {ARG_TYPE_NONE} ), 
 	#endif // TOGGLEFULLSCREEN_PATCH 
 	#if TRANSFER_PATCH 
 	IPCCOMMAND( transfer, 1, {ARG_TYPE_NONE} ), 
 	#endif // TRANSFER_PATCH 
 	#if TRANSFER_ALL_PATCH 
 	IPCCOMMAND( transferall, 1, {ARG_TYPE_NONE} ), 
 	#endif // TRANSFER_ALL_PATCH 
 	#if UNFLOATVISIBLE_PATCH 
 	IPCCOMMAND( unfloatvisible, 1, {ARG_TYPE_NONE} ), 
 	#endif // UNFLOATVISIBLE_PATCH 
 	#if VANITYGAPS_PATCH 
 	IPCCOMMAND( incrgaps, 1, {ARG_TYPE_SINT} ), 
 	IPCCOMMAND( incrigaps, 1, {ARG_TYPE_SINT} ), 
 	IPCCOMMAND( incrogaps, 1, {ARG_TYPE_SINT} ), 
 	IPCCOMMAND( incrihgaps, 1, {ARG_TYPE_SINT} ), 
 	IPCCOMMAND( incrivgaps, 1, {ARG_TYPE_SINT} ), 
 	IPCCOMMAND( incrohgaps, 1, {ARG_TYPE_SINT} ), 
 	IPCCOMMAND( incrovgaps, 1, {ARG_TYPE_SINT} ), 
 	IPCCOMMAND( togglegaps, 1, {ARG_TYPE_NONE} ), 
 	IPCCOMMAND( defaultgaps, 1, {ARG_TYPE_NONE} ), 
 	IPCCOMMAND( setgapsex, 1, {ARG_TYPE_SINT} ), 
 	#endif // VANITYGAPS_PATCH 
 	#if WINVIEW_PATCH 
 	IPCCOMMAND( winview, 1, {ARG_TYPE_NONE} ), 
 	#endif // WINVIEW_PATCH 
 	#if XRDB_PATCH && !BAR_VTCOLORS_PATCH 
 	IPCCOMMAND( xrdb, 1, {ARG_TYPE_NONE} ), 
 	#endif // XRDB_PATCH 
 }; 
 #endif // IPC_PATCH 