typedef struct {
	const char *name;
	const void *cmd;
} Sp;

typedef enum  {
	SP_TERMINAL,
	SP_NOTES,
	SP_SYSMONITOR,
} SpType;

#define SP_IN_TERM(NAME, SIZE, ...)  \
	{NAME, (const char*[]){"st", "-n", NAME, "-g", SIZE, __VA_ARGS__ }}

#define SP_BIND_KEY(MOD, KEY, SP_ID) \
	{ MOD, KEY, togglescratch,  {.ui = SP_ID } }

static Sp scratchpads[] = {
	[SP_TERMINAL] =
		SP_IN_TERM("spterm", "150x40", NULL),

	[SP_NOTES] =
		SP_IN_TERM("spnotes", "210x50", "-d", "/home/imsohexy/nixos/extras/notes", "-e",
			"nvim", "index.norg", NULL),

	[SP_SYSMONITOR] =
		SP_IN_TERM("spmon", "210x50", "-e", "gotop", NULL),
};

#define SP_RULES \
	/* class     instance    title       tags mask               float   monitor */ \
	{ NULL,	     "spterm",   NULL,				SPTAG(SP_TERMINAL),			1,			-1 }, \
	{ NULL,	     "spnotes",	 NULL,				SPTAG(SP_NOTES),				1,			-1 }, \
	{ NULL,	     "spmon",	   NULL,				SPTAG(SP_SYSMONITOR),   1,			-1 }

