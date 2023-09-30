/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 4;        /* gaps between windows */
static unsigned int snap      = 32;       /* snap pixel */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static const int focusonwheel       = 0;
static char font[]            = "JetBrainsMono Nerd Font:size=8";
static const char *fonts[]          = { font, "Font Awesome 5 Free:size=7", "Symbols Nerd Font:size=7" };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
// const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL };
// const char *spcmd2[] = {"st", "-n", "spfm", "-g", "144x41", "-e", "ranger", NULL };
// const char *spcmd3[] = {"keepassxc", NULL };

typedef enum  {
	SP_TERMINAL = 0,
	SP_NOTES = 1,
} SpType;

#define SP_SIZE_SMALL "120x34"

#define SP_IN_TERM(NAME, SIZE, ...)  \
	{NAME, (const char*[]){"st", "-n", NAME, "-g", SIZE, __VA_ARGS__ }}

static Sp scratchpads[] = {
	[SP_TERMINAL] = SP_IN_TERM("spterm", SP_SIZE_SMALL, NULL),
	[SP_NOTES]    = SP_IN_TERM("spnotes", SP_SIZE_SMALL, "-d", "/home/imsohexy/nixos/extras/notes", "-e", "nvim", "index.norg", NULL),
};


/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

/* include(s) depending on the tags array */
#include "flextile.h"

static const Rule rules[] = {
	/* class            instance    title       tags mask               float   monitor */
	{ "obs",            NULL,       NULL,       1 << 5,       					0,			-1 },
	{ "easyeffects",    NULL,       NULL,       1 << 4,       					0,			-1 },
	{ NULL,				      "spterm",		NULL,				SPTAG(SP_TERMINAL),			1,			-1 },
	{ NULL,				      "spnotes",	NULL,				SPTAG(SP_NOTES),				1,			-1 },
};

/* layout(s) */
static float mfact     = 0.6; /* factor of master area size [0.05..0.95] */
// static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int attachbelow = 1;    /* 1 means attach after the currently active window */
static const int layoutaxis[] = {
	1, /* layout axis: 1 = x, 2 = y; negative values mirror the layout, setting the master area to the right / bottom instead of left / top */
	2, /* master axis: 1 = x (from left to right), 2 = y (from top to bottom), 3 = z (monocle) */
	2, /* stack axis:  1 = x (from left to right), 2 = y (from top to bottom), 3 = z (monocle) */
};

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "font",               STRING,  &font },
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",          	  	INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	  INTEGER, &topbar },
		// { "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",        INTEGER, &resizehints },
		{ "mfact",      		  	FLOAT,   &mfact },
};

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = (const char*[]){ "st", NULL } } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      shiftmastersplit, {.i = +1} },   /* increase the number of tiled clients in the master area */
	{ MODKEY,                       XK_d,      shiftmastersplit, {.i = -1} },   /* reduce the number of tiled clients in the master area */
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[0]} },
	// { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,             					XK_f,      fullscreen,     {0} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,            						XK_t,  	   togglescratch,  {.ui = SP_TERMINAL } },
	{ MODKEY,            						XK_n,	   	 togglescratch,  {.ui = SP_NOTES } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ControlMask,           XK_t,      rotatelayoutaxis, {.i = 0} },    /* 0 = layout axis */
	{ MODKEY|ControlMask,           XK_Tab,    rotatelayoutaxis, {.i = 1} },    /* 1 = master axis */
	{ MODKEY|ControlMask|ShiftMask, XK_Tab,    rotatelayoutaxis, {.i = 2} },    /* 2 = stack axis */
	{ MODKEY|ControlMask,           XK_Return, mirrorlayout,     {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button1,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

