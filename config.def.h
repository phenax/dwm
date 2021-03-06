/* See LICENSE file for copyright and license details. */
#include "layouts/horizgrid.c"
#include "layouts/tile.c"
#include "layouts/tilehoriz.c"
#include "layouts/deck.c"
#include "layouts/monocle.c"

// NOTE: If you change the order here, change it in dwmc as well
enum { VTILE, FLOAT, MONOCLE, GRID, DECK, HTILE };

static const Layout layouts[] = {
/* index      symbol              function */
  [VTILE]   = { "tall",      tile },
	[FLOAT]   = { "float",     NULL },
	[MONOCLE] = { "moncle",    monocle },
	[GRID]    = { "grid",      horizgrid },
	[DECK]    = { "deck",      deck },
	[HTILE]   = { "wide",      tilehoriz },
};


/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const int gappx              = 4;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 18;       /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const int center_windowname  = 0;        /* 0 is the default dwm behavior. 1 will center the title */
static const char *fonts[]          = { "JetBrainsMono Nerd Font:size=8", "Font Awesome 5 Free:size=7", "Symbols Nerd Font:size=7" };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*                 fg           bg           border   */
       [SchemeNorm] =   { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  =   { selfgcolor,  selbgcolor,  selbordercolor  },
       [SchemeTitle] =  { "#ffffff",   "#1f1c29",   "#ffffff" },
};

/* False means using the scroll wheel on a window will not change focus */
static const short int focusonwheelscroll = 0;
static const short int forward_click = 1;

/* tagging */
static const char *tags[]    = { "1", "2", "3", "4", "5", "6: www", "7: meet", "8", "9: notes" };

// (index: int, name: char*) => format_string, ...values in the same order as they appear in format_string
#define TAG_NAME_FORMATTER(index, name) "%d: %s", index, name

static const Rule rules[] = {
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",         NULL,       NULL,       0,            1,           -1 },
  { "xfreerdp",     NULL,       NULL,       0,            0,           -1 },
	//{ "qutebrowser",  NULL,       NULL,       1 << 6,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.60; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int attachbelow = 1;    /* 1 means attach after the currently active window */

static const char autostart_cmd[] = "~/nixos/packages/dwm/autostart.sh &";

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
static const char *termcmd[]  = { "sensible-terminal", NULL };

#include <X11/XF86keysym.h>
#include "movestack.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },             // Quit window
	{ MODKEY|ControlMask,           XK_Delete, cleartag,       {0} },             // Clear tag
	{ MODKEY|ShiftMask,             XK_r,      quit,           {0} },             // Restart dwm
	{ MODKEY|ControlMask,           XK_Return, spawn,          {.v = termcmd } }, // Terminal
	{ MODKEY,                       XK_b,      togglebar,      {0} },             // Toggle status bar

  // Focus
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ Mod1Mask,                     XK_Tab,    focusmtoggle,   {.i = -1 } },
	{ MODKEY,                       XK_Tab,    view,           {0} }, // Toggle last tag

	// Resize
	{ MODKEY|Mod1Mask,              XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY|Mod1Mask,              XK_l,      setmfact,       {.f = +0.05} },

	// Move window
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },

	// Change master count
	{ MODKEY,                       XK_equal,  incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_minus,  incnmaster,     {.i = -1 } },

  // Layouts
	{ MODKEY,                       XK_f,      fullscreen,     {0} },                     // (f) Full screen

  // Multimonitor
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } }, // Focus prev monitor
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } }, // Focus next monitor
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

	// Tags
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

	// All tags
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	// { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
};

void
setlayoutex(const Arg *arg)
{
	setlayout(&((Arg) { .v = &layouts[arg->i] }));
}

void
viewex(const Arg *arg)
{
	view(&((Arg) { .ui = 1 << arg->ui }));
}

void
viewall(const Arg *arg)
{
	view(&((Arg){.ui = ~0}));
}

void
toggleviewex(const Arg *arg)
{
	toggleview(&((Arg) { .ui = 1 << arg->ui }));
}

void
tagex(const Arg *arg)
{
	tag(&((Arg) { .ui = 1 << arg->ui }));
}

void
toggletagex(const Arg *arg)
{
	toggletag(&((Arg) { .ui = 1 << arg->ui }));
}

void
tagall(const Arg *arg)
{
	tag(&((Arg){.ui = ~0}));
}

/* signal definitions */
/* signum must be greater than 0 */
/* trigger signals using `xsetroot -name "fsignal:<signame> [<type> <value>]"` */
static Signal signals[] = {
	/* signum           function */
	{ "focusstack",     focusstack },
	{ "settagname",     settagname },
	{ "setmfact",       setmfact },
	{ "togglebar",      togglebar },
	{ "incnmaster",     incnmaster },
	{ "togglefloating", togglefloating },
	{ "focusmon",       focusmon },
	{ "tagmon",         tagmon },
	{ "view",           view },
	{ "viewall",        viewall },
	{ "viewex",         viewex },
	{ "toggleview",     view },
	{ "toggleviewex",   toggleviewex },
	{ "tag",            tag },
	{ "tagall",         tagall },
	{ "tagex",          tagex },
	{ "toggletag",      tag },
	{ "toggletagex",    toggletagex },
	{ "killclient",     killclient },
	{ "quit",           quit },
	{ "setlayout",      setlayout },
	{ "setlayoutex",    setlayoutex },
};
