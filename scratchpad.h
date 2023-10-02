typedef struct {
	const char *name;
	const void *cmd;
} Sp;

typedef enum  {
	SP_TERMINAL,
	SP_NOTES,
	SP_SYSMONITOR,
	SP_CALENDAR,
	SP_SKETCHPAD,
	SP_CHATGPT,
} SpType;

#define SP_IN_TERM(NAME, SIZE, ...)  \
	{NAME, (const char*[]){"st", "-n", NAME, "-g", SIZE, __VA_ARGS__ }}

#define SP_BROWSER_APP(NAME, SIZE, URL)  \
	{NAME, (const char*[]){"brave", \
		"--class=" NAME, \
		"--user-data-dir=/tmp/brave-scratchpad-instance-" NAME, \
	  "--window-size=" SIZE, \
		"--app=" URL, \
		NULL \
	}}

#define SP_BIND_KEY(MOD, KEY, SP_ID) \
	{ MOD, KEY, togglescratch,  {.ui = SP_ID } }

#define SP_RULE(NAME, SP_ID) \
	/* class     instance    title  tags mask        float  monitor */ \
	{ NULL,	     NAME,       NULL,	SPTAG(SP_ID),	   1,		 -1 }

// terminal size
#define SP_SIZE_T_SM "150x40"
#define SP_SIZE_T_LG "210x45"
#define SP_SIZE_W_LG "1280,720"

static Sp scratchpads[] = {
	[SP_TERMINAL] =
		SP_IN_TERM("spterm", SP_SIZE_T_SM, NULL),

	[SP_NOTES] =
		SP_IN_TERM("spnotes", SP_SIZE_T_LG, "-d", "/home/imsohexy/nixos/extras/notes", "-e",
			"nvim", "index.norg", NULL),

	[SP_SYSMONITOR] =
		SP_IN_TERM("spmon", SP_SIZE_T_LG, "-e", "gotop", NULL),

	[SP_CALENDAR] =
		SP_IN_TERM("spcal", SP_SIZE_T_SM, "-e", "wyrd", NULL),

	[SP_SKETCHPAD] =
		SP_BROWSER_APP("excalidraw.com", SP_SIZE_W_LG, "https://excalidraw.com/"),

	[SP_CHATGPT] =
		SP_BROWSER_APP("chat.openai.com", SP_SIZE_W_LG, "https://chat.openai.com/"),
};

#define SP_RULES \
  SP_RULE("spterm", SP_TERMINAL), \
  SP_RULE("spnotes", SP_NOTES), \
  SP_RULE("spmon", SP_SYSMONITOR), \
  SP_RULE("spcal", SP_CALENDAR), \
  SP_RULE("excalidraw.com", SP_SKETCHPAD), \
  SP_RULE("chat.openai.com", SP_CHATGPT)

