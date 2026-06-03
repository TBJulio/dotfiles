#include <X11/XF86keysym.h>

/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = {"W95FA:size=15"};
static const char dmenufont[]       = "W95FA:size=15";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444"; // dark gray
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_silver[]      = "#C0C0C0";
static const char col_black[]       = "#000000";
static const char col_white[]       = "#FFFFFF";
static const char col_gray[]        = "#808080";
static const char col_gray5[]       = "#9B9A99";
static const char col_blue[]        = "#000080"; // royal
static const char col_blue2[]       = "#0000C0"; // true blue
static const char col_red[]         = "#8D130D";

static const char *colors[][4]      = {
			       /* text,      bg,         border,     float */
	[SchemeNorm]      = { col_black, col_blue,   col_blue,  col_silver }, // window unselected
	[SchemeSel]       = { col_black, col_blue,   col_blue,  col_blue2 },  // window selected
	[SchemeTagsNorm]  = { col_black, col_silver, col_black, col_silver }, // tagbar unselected
	[SchemeTagsSel]   = { col_white, col_blue,   col_black, col_blue },   // tagbar selected
	[SchemeInfoNorm]  = { col_white, col_silver, col_black, col_silver }, // infobar unselected
	[SchemeInfoSel]   = { col_black, col_silver, col_black, col_blue },   // infobar selected
	[SchemeStatus]    = { col_black, col_silver, col_black, col_silver }, // sltatus bar
};

/* tagging */
static const char *tags[] = { "","󰈹","","󰉋","󰕼","","󰓓","󰓇","" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                       instance                title                   tags mask               isfloating      monitor */
        { "Alacritty",                  NULL,                   NULL,                   1<<0,                   0,              -1 },
        { "firefox",                    NULL,                   "Picture-in-Picture",   (unsigned int)-1,       1,              -1 },
        { "firefox",                    NULL,                   NULL,                   1<<1,                   0,              -1 },
        { "code",                       NULL,                   NULL,                   1<<2,                   0,              -1 },
	{ "thunar",                     NULL,                   NULL,                   1<<3,                   0,              -1 },
        { "vlc",                        NULL,                   NULL,                   1<<4,                   0,              -1 },
        { "KeePassXC",                  NULL,                   NULL,                   1<<5,                   0,              -1 },
        { "steam",                      NULL,                   NULL,                   1<<6,                   0,              -1 },
        { "Spotify",                    NULL,                   NULL,                   1<<7,                   0,              -1 },
        { "discord",                    NULL,                   NULL,                   1<<8,                   0,              -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 120;  /* refresh rate (per second) for client move/resize */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      viewandlaunch,        {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     	{.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            	{.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      	{.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]			= { "dmenu_run", "-b", "-m", dmenumon, "-fn", dmenufont, "-nb", col_silver, "-nf", col_black, "-sb", col_blue, "-sf", col_white, NULL };
static const char *termcmd[]			= { "alacritty", NULL };
static const char scratchpadname[] 		= "scratchpad";
static const char *scratchpadcmd[] = {
	"alacritty",
	"--title", scratchpadname,
	"--option", "window.dimensions.columns=120",
	"--option", "window.dimensions.lines=34",
	NULL
};
static const char *screenshotcmd[]		= { "sh","-c","scrot -e 'mv $f ~/screenshots/'", NULL };
static const char *screenshotselectcmd[]	= { "sh","-c","scrot -s -e 'mv $f ~/screenshots/'", NULL };
static const char *musiccmd[]			= { "spotify", NULL };

#define SCRIPTS_DIR "/mnt/hdd3/dotfiles/scripts/"
/* click on tag (format: { script, search_string, launch_command, NULL }) */
static const char *firefoxt[]	= { "sh","-c",SCRIPTS_DIR"check_and_launch.sh firefox firefox", NULL };
static const char *codet[]	= { "sh","-c",SCRIPTS_DIR"check_and_launch.sh code code", NULL };
static const char *thunart[]	= { "sh","-c",SCRIPTS_DIR"check_and_launch.sh thunar thunar", NULL };
static const char *vlct[]	= { "sh","-c",SCRIPTS_DIR"check_and_launch.sh vlc vlc", NULL };
static const char *keepasst[]	= { "sh","-c",SCRIPTS_DIR"check_and_launch.sh keepassxc keepassxc", NULL };
static const char *steamt[]	= { "sh","-c",SCRIPTS_DIR"check_and_launch.sh steam steam", NULL };
static const char *spotifyt[]	= { "sh","-c",SCRIPTS_DIR"check_and_launch.sh spotify spotify", NULL };
static const char *discordt[]	= { "sh","-c",SCRIPTS_DIR"check_and_launch.sh discord discord", NULL };

void viewandlaunch(const Arg *arg) {
	view(arg);
	if (arg->ui == 1 << 1)
		spawn(&(Arg){.v = firefoxt});
	else if (arg->ui == 1 << 2)
		spawn(&(Arg){.v = codet});
	else if (arg->ui == 1 << 3)
		spawn(&(Arg){.v = thunart});
	else if (arg->ui == 1 << 4)
		spawn(&(Arg){.v = vlct});
	else if (arg->ui == 1 << 5)
		spawn(&(Arg){.v = keepasst});
	else if (arg->ui == 1 << 6)
		spawn(&(Arg){.v = steamt});
	else if (arg->ui == 1 << 7)
		spawn(&(Arg){.v = spotifyt});
	else if (arg->ui == 1 << 8)
		spawn(&(Arg){.v = discordt});
}

/* media keys */
static const char *upvol[]	= { "sh","-c","pactl set-sink-volume @DEFAULT_SINK@ +5% && "SCRIPTS_DIR"volume-notification.sh", NULL };
static const char *downvol[]	= { "sh","-c","pactl set-sink-volume @DEFAULT_SINK@ -5% && "SCRIPTS_DIR"volume-notification.sh", NULL };
static const char *mutevol[]	= { "sh","-c","pactl set-sink-mute @DEFAULT_SINK@ toggle && "SCRIPTS_DIR"volume-notification.sh", NULL };
static const char *playpause[]	= { "sh","-c","playerctl play-pause", NULL };
static const char *nexttrack[]	= { "sh","-c","playerctl next", NULL };
static const char *prevtrack[]	= { "sh","-c","playerctl previous", NULL };

/* misc */
static const char *git[] 	= { "sh","-c",SCRIPTS_DIR"git-updater.sh > /tmp/git-updater.log 2>&1", NULL };
static const char *dwmmake [] 	= { "sh","-c",SCRIPTS_DIR"dwm-make.sh", NULL };

static const Key keys[] = {
	/* modifier                     key        function        	argument */
	{ MODKEY,                       XK_s,      spawn,          	{.v = dmenucmd } },
					/* ↑ XK_p */
	{ MODKEY,             		XK_z, 	   spawn,          	{.v = termcmd } },
	     /* ↑ |ShiftMask */         /* ↑ XK_Return */
	{ MODKEY,                       XK_a,      togglescratch,       {.v = scratchpadcmd } },
	{ MODKEY,                       XK_b,      togglebar,      	{0} },
	{ MODKEY,                       XK_j,      focusstack,     	{.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     	{.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     	{.i = +1 } },
	{ MODKEY,                       XK_o,      incnmaster,     	{.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       	{.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       	{.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           	{0} },
	{ MODKEY,                       XK_Tab,    view,       		{0} },
	{ MODKEY,             		XK_c,      killclient,     	{0} },
	     /* ↑ |ShiftMask */
	{ MODKEY,                       XK_t,      setlayout,      	{.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      	{.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      	{.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      	{0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, 	{0} },
	{ MODKEY,                       XK_0,      view,           	{.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            	{.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       	{.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       	{.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         	{.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         	{.i = +1 } },
	TAGKEYS(                        XK_1,                      	0)
	TAGKEYS(                        XK_2,                      	1)
	TAGKEYS(                        XK_3,                      	2)
	TAGKEYS(                        XK_4,                      	3)
	TAGKEYS(                        XK_5,                      	4)
	TAGKEYS(                        XK_6,                      	5)
	TAGKEYS(                        XK_7,                      	6)
	TAGKEYS(                        XK_8,                      	7)
	TAGKEYS(                        XK_9,                      	8)
	{ MODKEY,                       XK_Escape, quit,           	{0} },
					/* ↑ XK_q */
	{ 0,                            XK_Print,  spawn,          	{.v = screenshotcmd } },
	{ MODKEY,                       XK_Print,  spawn,          	{.v = screenshotselectcmd } },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          	{.v = musiccmd } },
	{ 0, XF86XK_AudioRaiseVolume,  		   spawn, 		{.v = upvol } },
	{ 0, XF86XK_AudioLowerVolume,  		   spawn, 		{.v = downvol } },
	{ 0, XF86XK_AudioMute,         		   spawn, 		{.v = mutevol } },
	{ 0, XF86XK_AudioPlay,         		   spawn, 		{.v = playpause } },
	{ 0, XF86XK_AudioNext,         		   spawn, 		{.v = nexttrack } },
	{ 0, XF86XK_AudioPrev,         		   spawn, 		{.v = prevtrack } },
	{ MODKEY,                       XK_g,      spawn,               {.v = git } },
	{ MODKEY,                       XK_d,      spawn,               {.v = dwmmake } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,      	Button1,   	viewandlaunch,  {0} },
							     /* ↑ view */
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
