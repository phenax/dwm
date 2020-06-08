/* See LICENSE file for copyright and license details. */
#include "tcl.c"
#include "horizgrid.c"

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
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
static const char *fonts[]          = { "JetBrains Mono:size=8", "Font Awesome 5 Free:size=7", "Symbols Nerd Font:size=7" };
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
       [SchemeTitle] =  { "#ffffff",   "#252835",   "#ffffff" },
};

/* False means using the scroll wheel on a window will not change focus */
static const Bool focusonwheelscroll = False;

/* tagging */
static const char *tags[]    = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const char *tagsalt[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",         NULL,       NULL,       0,            1,           -1 },
	//{ "qutebrowser",  NULL,       NULL,       1 << 6,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.60; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int attachbelow = 1;    /* 1 means attach after the currently active window */

static const char autostart_cmd[] = "~/.bin/with_zsh ~/.config/autostart.sh &";

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "tall",         tile },
	{ "float",        NULL },
	{ "moncle",       monocle },
	{ "grid",         horizgrid },
	{ "center",       tcl },
	{ "deck",         deck },
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
static const char *termcmd[]  = { "sensible-terminal", NULL };

#include "movestack.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },             // Quit window
	{ MODKEY|ShiftMask,             XK_r,      quit,           {0} },             // Restart dwm
	{ MODKEY|ControlMask,           XK_Return, spawn,          {.v = termcmd } }, // Terminal
	{ MODKEY,                       XK_b,      togglebar,      {0} },             // Toggle status bar

  // Focus
	{ MODKEY,                       XK_Right,  focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_Left,   focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_Down,   focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_Up,     focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_Tab,    view,           {0} }, // Toggle last tag

	// Resize
	{ MODKEY|Mod1Mask,              XK_Left,   setmfact,       {.f = -0.05} },
	{ MODKEY|Mod1Mask,              XK_Right,  setmfact,       {.f = +0.05} },

	// Move window
	{ MODKEY|ShiftMask,             XK_Right,  movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Left,   movestack,      {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Down,   movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Up,     movestack,      {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_z,      zoom,           {0} },

	// Change master count
	{ MODKEY,                       XK_equal,  incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_minus,  incnmaster,     {.i = -1 } },

  // Layouts
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },  // (t) Tall
	{ MODKEY          ,             XK_f,      fullscreen,     {0} },                 // (f) Full screen
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[3]} },  // (g) Grid
	{ MODKEY,                       XK_w,      setlayout,      {.v = &layouts[4]} },  // (w) Center stack
	{ MODKEY,                       XK_e,      setlayout,      {.v = &layouts[5]} },  // (e) Deck layout

  // Multimonitor
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } }, // Focus prev monitor
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } }, // Focus next monitor
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

	{ MODKEY,                       XK_n,      togglealttag,   {0} }, // Toggle alt tag names

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
	//{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	//{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	//{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
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
	{ "setmfact",       setmfact },
	{ "togglebar",      togglebar },
	{ "incnmaster",     incnmaster },
	{ "togglefloating", togglefloating },
	{ "focusmon",       focusmon },
	{ "tagmon",         tagmon },
	{ "zoom",           zoom },
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
