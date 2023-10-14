/* See LICENSE for license details. */

#include <stdint.h>
#include <sys/types.h>

/* macros */
#define MIN(a, b)		((a) < (b) ? (a) : (b))
#define MAX(a, b)		((a) < (b) ? (b) : (a))
#define LEN(a)			(sizeof(a) / sizeof(a)[0])
#define BETWEEN(x, a, b)	((a) <= (x) && (x) <= (b))
#define DIVCEIL(n, d)		(((n) + ((d) - 1)) / (d))
#define DEFAULT(a, b)		(a) = (a) ? (a) : (b)
#define LIMIT(x, a, b)		(x) = (x) < (a) ? (a) : (x) > (b) ? (b) : (x)
#define ATTRCMP(a, b)		((a).mode != (b).mode || (a).fg != (b).fg || \
				(a).bg != (b).bg)
#define TIMEDIFF(t1, t2)	((t1.tv_sec-t2.tv_sec)*1000 + \
				(t1.tv_nsec-t2.tv_nsec)/1E6)
#define MODBIT(x, set, bit)	((set) ? ((x) |= (bit)) : ((x) &= ~(bit)))

#define TRUECOLOR(r,g,b)	(1 << 24 | (r) << 16 | (g) << 8 | (b))
#define IS_TRUECOL(x)		(1 << 24 & (x))


enum glyph_attribute {
	ATTR_NULL       = 0,
	ATTR_BOLD       = 1 << 0,
	ATTR_FAINT      = 1 << 1,
	ATTR_ITALIC     = 1 << 2,
	ATTR_UNDERLINE  = 1 << 3,
	ATTR_BLINK      = 1 << 4,
	ATTR_REVERSE    = 1 << 5,
	ATTR_INVISIBLE  = 1 << 6,
	ATTR_STRUCK     = 1 << 7,
	ATTR_WRAP       = 1 << 8,
	ATTR_WIDE       = 1 << 9,
	ATTR_WDUMMY     = 1 << 10,
	ATTR_BOLD_FAINT = ATTR_BOLD | ATTR_FAINT,
};

enum selection_mode {
	SEL_IDLE = 0,
	SEL_EMPTY = 1,
	SEL_READY = 2
};

enum selection_type {
	SEL_REGULAR = 1,
	SEL_RECTANGULAR = 2
};

enum selection_snap {
	SNAP_WORD = 1,
	SNAP_LINE = 2
};

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned short ushort;

typedef uint_least32_t Rune;

#define Glyph Glyph_
typedef struct {
	Rune u;           /* character code */
	ushort mode;      /* attribute flags */
	uint32_t fg;      /* foreground  */
	uint32_t bg;      /* background  */
} Glyph;

typedef Glyph *Line;

typedef union {
	int i;
	uint ui;
	float f;
	const void *v;
	const char *s;
} Arg;

/* Arbitrary sizes */
#define UTF_INVALID   0xFFFD
#define UTF_SIZ       4
#define ESC_BUF_SIZ   (128*UTF_SIZ)
#define ESC_ARG_SIZ   16
#define STR_BUF_SIZ   ESC_BUF_SIZ
#define STR_ARG_SIZ   ESC_ARG_SIZ

/* macros */
#define IS_SET(flag)            ((st->mode & (flag)) != 0)
#define ISCONTROLC0(c)          (BETWEEN(c, 0, 0x1f) || (c) == 0x7f)
#define ISCONTROLC1(c)          (BETWEEN(c, 0x80, 0x9f))
#define ISCONTROL(c)            (ISCONTROLC0(c) || ISCONTROLC1(c))
#define ISDELIM(u)              (u && wcschr(worddelimiters, u))

enum term_mode {
        TERM_MODE_WRAP        = 1 << 0,
        TERM_MODE_INSERT      = 1 << 1,
        TERM_MODE_ALTSCREEN   = 1 << 2,
        TERM_MODE_CRLF        = 1 << 3,
        TERM_MODE_ECHO        = 1 << 4,
        TERM_MODE_PRINT       = 1 << 5,
        TERM_MODE_UTF8        = 1 << 6,
};

enum cursor_movement {
        CURSOR_SAVE,
        CURSOR_LOAD
};

enum cursor_state {
        CURSOR_DEFAULT  = 0,
        CURSOR_WRAPNEXT = 1,
        CURSOR_ORIGIN   = 2
};

enum charset {
        CS_GRAPHIC0,
        CS_GRAPHIC1,
        CS_UK,
        CS_USA,
        CS_MULTI,
        CS_GER,
        CS_FIN
};

enum escape_state {
        ESC_START      = 1,
        ESC_CSI        = 2,
        ESC_STR        = 4,  /* DCS, OSC, PM, APC */
        ESC_ALTCHARSET = 8,
        ESC_STR_END    = 16, /* a final string was encountered */
        ESC_TEST       = 32, /* Enter in test mode */
        ESC_UTF8       = 64,
};

typedef struct {
        Glyph attr; /* current char attributes */
        int x;
        int y;
        char state;
} TCursor;

typedef struct Selection {
        int mode;
        int type;
        int snap;
        /*
         * Selection variables:
         * nb – normalized coordinates of the beginning of the selection
         * ne – normalized coordinates of the end of the selection
         * ob – original coordinates of the beginning of the selection
         * oe – original coordinates of the end of the selection
         */
        struct {
                int x, y;
        } nb, ne, ob, oe;

        int alt;
} Selection;

/* CSI Escape sequence structs */
/* ESC '[' [[ [<priv>] <arg> [;]] <mode> [<mode>]] */
typedef struct CSIEscape {
        char buf[ESC_BUF_SIZ]; /* raw string */
        size_t len;            /* raw string length */
        char priv;
        int arg[ESC_ARG_SIZ];
        int narg;              /* nb of args */
        char mode[2];
} CSIEscape;

/* STR Escape sequence structs */
/* ESC type [[ [<priv>] <arg> [;]] <mode>] ESC '\' */
typedef struct STREscape{
        char type;             /* ESC type ... */
        char *buf;             /* allocated raw string */
        size_t siz;            /* allocation size */
        size_t len;            /* raw string length */
        char *args[STR_ARG_SIZ];
        int narg;              /* nb of args */
} STREscape;

/* Internal representation of the screen */
typedef struct ST {
        int row;      /* nb row */
        int col;      /* nb col */
        Line *line;   /* screen */
        Line *alt;    /* alternate screen */
        int *dirty;   /* dirtyness of lines */
        TCursor c;    /* cursor */
        int ocx;      /* old cursor col */
        int ocy;      /* old cursor row */
        int top;      /* top    scroll limit */
        int bot;      /* bottom scroll limit */
        int windowMode;
        int mode;     /* terminal mode flags */
        int esc;      /* escape state flags */
        char trantbl[4]; /* charset table translation */
        int charset;  /* current charset */
        int icharset; /* selected charset for sequence */
        int *tabs;
        Rune lastc;   /* last printed char outside of sequence, 0 if control */
        Selection sel;
        CSIEscape csiescseq;
        STREscape strescseq;
        int iofd;
        int cmdfd;
        int close;
        pid_t pid;
        int scrollup;
        int maxScrollup;
        Glyph **history;
        int *historyCols;
} ST;

void die(const char *, ...);
void redraw(ST *st);

void printscreen(ST *st, const Arg *);
void printsel(ST *st, const Arg *);
void sendbreak(ST *st, const Arg *);
void toggleprinter(ST *st, const Arg *);

int tattrset(ST *st, int);
ST *tnew(int, int);
void tfree(ST *st);
void tresize(ST *st, int, int);
void cresize(ST *st, int width, int height);
void tsetdirtattr(ST *st, int);
void ttyhangup(ST *st);
int ttynew(ST *st, const char *, char *, const char *, char **);
size_t ttyread(ST *st);
void ttyresize(ST *st, int, int);
void ttywrite(ST *st, const char *, size_t, int);

void resettitle(void);

void selclear(ST *st);
void selinit(ST *st);
void selstart(ST *st, int, int, int);
void selextend(ST *st, int, int, int, int);
int selected(ST *st, int, int);
char *getsel(ST *st);

size_t utf8encode(Rune, char *);

void *xmalloc(size_t);
void *xrealloc(void *, size_t);
char *xstrdup(const char *);

int xgetcolor(int x, unsigned char *r, unsigned char *g, unsigned char *b);

/* config.h globals */
extern char *utmp;
extern char *scroll;
extern char *stty_args;
extern char *vtiden;
extern wchar_t *worddelimiters;
extern int allowaltscreen;
extern int allowwindowops;
extern char *termname;
extern unsigned int tabspaces;
extern unsigned int defaultfg;
extern unsigned int defaultbg;
extern unsigned int defaultcs;
