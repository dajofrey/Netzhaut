/* See LICENSE for license details. */
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <pwd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#include <wchar.h>

#include "config.def.h"
#include "st.h"
#include "win.h"

#if   defined(__linux)
 #include <pty.h>
#elif defined(__OpenBSD__) || defined(__NetBSD__) || defined(__APPLE__)
 #include <util.h>
#elif defined(__FreeBSD__) || defined(__DragonFly__)
 #include <libutil.h>
#endif

static void execsh(char *, char **);
static void stty(char **);
static void ttywriteraw(ST *st, const char *, size_t);

static void csidump(ST *st);
static void csihandle(ST *st);
static void csiparse(ST *st);
static void csireset(ST *st);
static int eschandle(ST *st, uchar);
static void strdump(ST *st);
static void strhandle(ST *st);
static void strparse(ST *st);
static void strreset(ST *st);

static void tprinter(ST *st, char *, size_t);
static void tdumpsel(ST *st);
static void tdumpline(ST *st, int);
static void tdump(ST *st);
static void tclearregion(ST *st, int, int, int, int);
static void tcursor(ST *st, int);
static void tdeletechar(ST *st, int);
static void tdeleteline(ST *st, int);
static void tinsertblank(ST *st, int);
static void tinsertblankline(ST *st, int);
static int tlinelen(ST *st, int);
static void tmoveto(ST *st, int, int);
static void tmoveato(ST *st, int, int);
static void tnewline(ST *st, int);
static void tputtab(ST *st, int);
static void tputc(ST *st, Rune);
static void treset(ST *st);
static void tscrollup(ST *st, int, int);
static void tscrolldown(ST *st, int, int);
static void tsetattr(ST *st, const int *, int);
static void tsetchar(ST *st, Rune, const Glyph *, int, int);
static void tsetdirt(ST *st, int, int);
static void tsetscroll(ST *st, int, int);
static void tswapscreen(ST *st);
static void tsetmode(ST *st, int, int, const int *, int);
static int twrite(ST *st, const char *, int, int);
static void tfulldirt(ST *st);
static void tcontrolcode(ST *st, uchar);
static void tdectest(ST *st, char);
static void tdefutf8(ST *st, char);
static int32_t tdefcolor(const int *, int *, int);
static void tdeftran(ST *st, char);
static void tstrsequence(ST *st, uchar);

static void selnormalize(ST *st);
static void selscroll(ST *st, int, int);
static void selsnap(ST *st, int *, int *, int);

static size_t utf8decode(ST *st, const char *, Rune *, size_t);
static Rune utf8decodebyte(ST *st, char, size_t *);
static char utf8encodebyte(Rune, size_t);
static size_t utf8validate(Rune *, size_t);

static char *base64dec(const char *);
static char base64dec_getc(const char **);

static ssize_t xwrite(int, const char *, size_t);

ssize_t
xwrite(int fd, const char *s, size_t len)
{
	size_t aux = len;
	ssize_t r;

	while (len > 0) {
		r = write(fd, s, len);
		if (r < 0)
			return r;
		len -= r;
		s += r;
	}

	return aux;
}

void *
xmalloc(size_t len)
{
	void *p;

	if (!(p = malloc(len)))
		die("malloc: %s\n", strerror(errno));

	return p;
}

void *
xrealloc(void *p, size_t len)
{
	if ((p = realloc(p, len)) == NULL)
		die("realloc: %s\n", strerror(errno));

	return p;
}

char *
xstrdup(const char *s)
{
	char *p;

	if ((p = strdup(s)) == NULL)
		die("strdup: %s\n", strerror(errno));

	return p;
}

size_t
utf8decode(ST *st, const char *c, Rune *u, size_t clen)
{
	size_t i, j, len, type;
	Rune udecoded;

	*u = UTF_INVALID;
	if (!clen)
		return 0;
	udecoded = utf8decodebyte(st, c[0], &len);
	if (!BETWEEN(len, 1, UTF_SIZ))
		return 1;
	for (i = 1, j = 1; i < clen && j < len; ++i, ++j) {
		udecoded = (udecoded << 6) | utf8decodebyte(st, c[i], &type);
		if (type != 0)
			return j;
	}
	if (j < len)
		return 0;
	*u = udecoded;
	utf8validate(u, len);

	return len;
}

Rune
utf8decodebyte(ST *st, char c, size_t *i)
{

        const uchar utfbyte[UTF_SIZ + 1] = {0x80,    0, 0xC0, 0xE0, 0xF0};
        const uchar utfmask[UTF_SIZ + 1] = {0xC0, 0x80, 0xE0, 0xF0, 0xF8};
	for (*i = 0; *i < LEN(utfmask); ++(*i))
		if (((uchar)c & utfmask[*i]) == utfbyte[*i])
			return (uchar)c & ~utfmask[*i];

	return 0;
}

size_t
utf8encode(Rune u, char *c)
{
	size_t len, i;

	len = utf8validate(&u, 0);
	if (len > UTF_SIZ)
		return 0;

	for (i = len - 1; i != 0; --i) {
		c[i] = utf8encodebyte(u, 0);
		u >>= 6;
	}
	c[0] = utf8encodebyte(u, len);

	return len;
}

char
utf8encodebyte(Rune u, size_t i)
{

        const uchar utfbyte[UTF_SIZ + 1] = {0x80,    0, 0xC0, 0xE0, 0xF0}; 
        const uchar utfmask[UTF_SIZ + 1] = {0xC0, 0x80, 0xE0, 0xF0, 0xF8};
	return utfbyte[i] | (u & ~utfmask[i]);
}

size_t
utf8validate(Rune *u, size_t i)
{
        const Rune utfmin[UTF_SIZ + 1] = {       0,    0,  0x80,  0x800,  0x10000}; 
        const Rune utfmax[UTF_SIZ + 1] = {0x10FFFF, 0x7F, 0x7FF, 0xFFFF, 0x10FFFF}; 
	if (!BETWEEN(*u, utfmin[i], utfmax[i]) || BETWEEN(*u, 0xD800, 0xDFFF))
		*u = UTF_INVALID;
	for (i = 1; *u > utfmax[i]; ++i)
		;

	return i;
}

static const char base64_digits[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 0, 0, 0,
	63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0, 0, 0, -1, 0, 0, 0, 0, 1,
	2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
	22, 23, 24, 25, 0, 0, 0, 0, 0, 0, 26, 27, 28, 29, 30, 31, 32, 33, 34,
	35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

char
base64dec_getc(const char **src)
{
	while (**src && !isprint(**src))
		(*src)++;
	return **src ? *((*src)++) : '=';  /* emulate padding if string ends */
}

char *
base64dec(const char *src)
{
	size_t in_len = strlen(src);
	char *result, *dst;

	if (in_len % 4)
		in_len += 4 - (in_len % 4);
	result = dst = xmalloc(in_len / 4 * 3 + 1);
	while (*src) {
		int a = base64_digits[(unsigned char) base64dec_getc(&src)];
		int b = base64_digits[(unsigned char) base64dec_getc(&src)];
		int c = base64_digits[(unsigned char) base64dec_getc(&src)];
		int d = base64_digits[(unsigned char) base64dec_getc(&src)];

		/* invalid input. 'a' can be -1, e.g. if src is "\n" (c-str) */
		if (a == -1 || b == -1)
			break;

		*dst++ = (a << 2) | ((b & 0x30) >> 4);
		if (c == -1)
			break;
		*dst++ = ((b & 0x0f) << 4) | ((c & 0x3c) >> 2);
		if (d == -1)
			break;
		*dst++ = ((c & 0x03) << 6) | d;
	}
	*dst = '\0';
	return result;
}

void
selinit(ST *st)
{
	st->sel.mode = SEL_IDLE;
	st->sel.snap = 0;
	st->sel.ob.x = -1;
}

int
tlinelen(ST *st, int y)
{
	int i = st->col;

	if (st->line[y][i - 1].mode & ATTR_WRAP)
		return i;

	while (i > 0 && st->line[y][i - 1].u == ' ')
		--i;

	return i;
}

void
selstart(ST *st, int col, int row, int snap)
{
	selclear(st);
	st->sel.mode = SEL_EMPTY;
	st->sel.type = SEL_REGULAR;
	st->sel.alt = IS_SET(TERM_MODE_ALTSCREEN);
	st->sel.snap = snap;
	st->sel.oe.x = st->sel.ob.x = col;
	st->sel.oe.y = st->sel.ob.y = row;
	selnormalize(st);

	if (st->sel.snap != 0)
		st->sel.mode = SEL_READY;
	tsetdirt(st, st->sel.nb.y, st->sel.ne.y);
}

void
selextend(ST *st, int col, int row, int type, int done)
{
	int oldey, oldex, oldsby, oldsey, oldtype;

	if (st->sel.mode == SEL_IDLE)
		return;
	if (done && st->sel.mode == SEL_EMPTY) {
		selclear(st);
		return;
	}

	oldey = st->sel.oe.y;
	oldex = st->sel.oe.x;
	oldsby = st->sel.nb.y;
	oldsey = st->sel.ne.y;
	oldtype = st->sel.type;

	st->sel.oe.x = col;
	st->sel.oe.y = row;
	selnormalize(st);
	st->sel.type = type;

	if (oldey != st->sel.oe.y || oldex != st->sel.oe.x || oldtype != st->sel.type || st->sel.mode == SEL_EMPTY)
		tsetdirt(st, MIN(st->sel.nb.y, oldsby), MAX(st->sel.ne.y, oldsey));

	st->sel.mode = done ? SEL_IDLE : SEL_READY;
}

void
selnormalize(ST *st)
{
	int i;

	if (st->sel.type == SEL_REGULAR && st->sel.ob.y != st->sel.oe.y) {
		st->sel.nb.x = st->sel.ob.y < st->sel.oe.y ? st->sel.ob.x : st->sel.oe.x;
		st->sel.ne.x = st->sel.ob.y < st->sel.oe.y ? st->sel.oe.x : st->sel.ob.x;
	} else {
		st->sel.nb.x = MIN(st->sel.ob.x, st->sel.oe.x);
		st->sel.ne.x = MAX(st->sel.ob.x, st->sel.oe.x);
	}
	st->sel.nb.y = MIN(st->sel.ob.y, st->sel.oe.y);
	st->sel.ne.y = MAX(st->sel.ob.y, st->sel.oe.y);

	selsnap(st, &st->sel.nb.x, &st->sel.nb.y, -1);
	selsnap(st, &st->sel.ne.x, &st->sel.ne.y, +1);

	/* expand selection over line breaks */
	if (st->sel.type == SEL_RECTANGULAR)
		return;
	i = tlinelen(st, st->sel.nb.y);
	if (i < st->sel.nb.x)
		st->sel.nb.x = i;
	if (tlinelen(st, st->sel.ne.y) <= st->sel.ne.x)
		st->sel.ne.x = st->col - 1;
}

int
selected(ST *st, int x, int y)
{
	if (st->sel.mode == SEL_EMPTY || st->sel.ob.x == -1 ||
			st->sel.alt != IS_SET(TERM_MODE_ALTSCREEN))
		return 0;

	if (st->sel.type == SEL_RECTANGULAR)
		return BETWEEN(y, st->sel.nb.y, st->sel.ne.y)
		    && BETWEEN(x, st->sel.nb.x, st->sel.ne.x);

	return BETWEEN(y, st->sel.nb.y, st->sel.ne.y)
	    && (y != st->sel.nb.y || x >= st->sel.nb.x)
	    && (y != st->sel.ne.y || x <= st->sel.ne.x);
}

void
selsnap(ST *st, int *x, int *y, int direction)
{
	int newx, newy, xt, yt;
	int delim, prevdelim;
	const Glyph *gp, *prevgp;

	switch (st->sel.snap) {
	case SNAP_WORD:
		/*
		 * Snap around if the word wraps around at the end or
		 * beginning of a line.
		 */
		prevgp = &st->line[*y][*x];
		prevdelim = ISDELIM(prevgp->u);
		for (;;) {
			newx = *x + direction;
			newy = *y;
			if (!BETWEEN(newx, 0, st->col - 1)) {
				newy += direction;
				newx = (newx + st->col) % st->col;
				if (!BETWEEN(newy, 0, st->row - 1))
					break;

				if (direction > 0)
					yt = *y, xt = *x;
				else
					yt = newy, xt = newx;
				if (!(st->line[yt][xt].mode & ATTR_WRAP))
					break;
			}

			if (newx >= tlinelen(st, newy))
				break;

			gp = &st->line[newy][newx];
			delim = ISDELIM(gp->u);
			if (!(gp->mode & ATTR_WDUMMY) && (delim != prevdelim
					|| (delim && gp->u != prevgp->u)))
				break;

			*x = newx;
			*y = newy;
			prevgp = gp;
			prevdelim = delim;
		}
		break;
	case SNAP_LINE:
		/*
		 * Snap around if the the previous line or the current one
		 * has set ATTR_WRAP at its end. Then the whole next or
		 * previous line will be selected.
		 */
		*x = (direction < 0) ? 0 : st->col - 1;
		if (direction < 0) {
			for (; *y > 0; *y += direction) {
				if (!(st->line[*y-1][st->col-1].mode
						& ATTR_WRAP)) {
					break;
				}
			}
		} else if (direction > 0) {
			for (; *y < st->row-1; *y += direction) {
				if (!(st->line[*y][st->col-1].mode
						& ATTR_WRAP)) {
					break;
				}
			}
		}
		break;
	}
}

char *
getsel(ST *st)
{
	char *str, *ptr;
	int y, bufsize, lastx, linelen;
	const Glyph *gp, *last;

	if (st->sel.ob.x == -1)
		return NULL;

	bufsize = (st->col+1) * (st->sel.ne.y-st->sel.nb.y+1) * UTF_SIZ;
	ptr = str = xmalloc(bufsize);

	/* append every set & selected glyph to the selection */
	for (y = st->sel.nb.y; y <= st->sel.ne.y; y++) {
		if ((linelen = tlinelen(st, y)) == 0) {
			*ptr++ = '\n';
			continue;
		}

		if (st->sel.type == SEL_RECTANGULAR) {
			gp = &st->line[y][st->sel.nb.x];
			lastx = st->sel.ne.x;
		} else {
			gp = &st->line[y][st->sel.nb.y == y ? st->sel.nb.x : 0];
			lastx = (st->sel.ne.y == y) ? st->sel.ne.x : st->col-1;
		}
		last = &st->line[y][MIN(lastx, linelen-1)];
		while (last >= gp && last->u == ' ')
			--last;

		for ( ; gp <= last; ++gp) {
			if (gp->mode & ATTR_WDUMMY)
				continue;

			ptr += utf8encode(gp->u, ptr);
		}

		/*
		 * Copy and pasting of line endings is inconsistent
		 * in the inconsistent stinal and GUI world.
		 * The best solution seems like to produce '\n' when
		 * something is copied from st and convert '\n' to
		 * '\r', when something to be pasted is received by
		 * st.
		 * FIXME: Fix the computer world.
		 */
		if ((y < st->sel.ne.y || lastx >= linelen) &&
		    (!(last->mode & ATTR_WRAP) || st->sel.type == SEL_RECTANGULAR))
			*ptr++ = '\n';
	}
	*ptr = 0;
	return str;
}

void
selclear(ST *st)
{
	if (st->sel.ob.x == -1)
		return;
	st->sel.mode = SEL_IDLE;
	st->sel.ob.x = -1;
	tsetdirt(st, st->sel.nb.y, st->sel.ne.y);
}

void
die(const char *errstr, ...)
{
	va_list ap;

	va_start(ap, errstr);
	vfprintf(stderr, errstr, ap);
	va_end(ap);
	exit(1);
}

void
execsh(char *cmd, char **args)
{
	char *sh, *prog, *arg;
	const struct passwd *pw;

	errno = 0;
	if ((pw = getpwuid(getuid())) == NULL) {
		if (errno)
			die("getpwuid: %s\n", strerror(errno));
		else
			die("who are you?\n");
	}

	if ((sh = getenv("SHELL")) == NULL)
		sh = (pw->pw_shell[0]) ? pw->pw_shell : cmd;

	if (args) {
		prog = args[0];
		arg = NULL;
	} else if (scroll) {
		prog = scroll;
		arg = utmp ? utmp : sh;
	} else if (utmp) {
		prog = utmp;
		arg = NULL;
	} else {
		prog = sh;
		arg = NULL;
	}
	DEFAULT(args, ((char *[]) {prog, arg, NULL}));

	unsetenv("COLUMNS");
	unsetenv("LINES");
	unsetenv("TERMCAP");
	setenv("LOGNAME", pw->pw_name, 1);
	setenv("USER", pw->pw_name, 1);
	setenv("SHELL", sh, 1);
	setenv("HOME", pw->pw_dir, 1);
	setenv("TERM", termname, 1);

	signal(SIGCHLD, SIG_DFL);
	signal(SIGHUP, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
	signal(SIGALRM, SIG_DFL);

	execvp(prog, args);
	_exit(1);
}

void
stty(char **args)
{
	char cmd[_POSIX_ARG_MAX], **p, *q, *s;
	size_t n, siz;

	if ((n = strlen(stty_args)) > sizeof(cmd)-1)
		die("incorrect stty parameters\n");
	memcpy(cmd, stty_args, n);
	q = cmd + n;
	siz = sizeof(cmd) - n;
	for (p = args; p && (s = *p); ++p) {
		if ((n = strlen(s)) > siz-1)
			die("stty parameter length too long\n");
		*q++ = ' ';
		memcpy(q, s, n);
		q += n;
		siz -= n + 1;
	}
	*q = '\0';
	if (system(cmd) != 0)
		perror("Couldn't call stty");
}

int
ttynew(ST *st, const char *line, char *cmd, const char *out, char **args)
{
	int m, s;

	if (out) {
		st->mode |= TERM_MODE_PRINT;
		st->iofd = (!strcmp(out, "-")) ?
			  1 : open(out, O_WRONLY | O_CREAT, 0666);
		if (st->iofd < 0) {
			fprintf(stderr, "Error opening %s:%s\n",
				out, strerror(errno));
		}
	}

	if (line) {
		if ((st->cmdfd = open(line, O_RDWR)) < 0)
			die("open line '%s' failed: %s\n",
			    line, strerror(errno));
		dup2(st->cmdfd, 0);
		stty(args);
		return st->cmdfd;
	}

	/* seems to work fine on linux, openbsd and freebsd */
	if (openpty(&m, &s, NULL, NULL, NULL) < 0)
		die("openpty failed: %s\n", strerror(errno));

	switch (st->pid = fork()) {
	case -1:
		die("fork failed: %s\n", strerror(errno));
		break;
	case 0:
		close(st->iofd);
		close(m);
		setsid(); /* create a new process group */
		dup2(s, 0);
		dup2(s, 1);
		dup2(s, 2);
		if (ioctl(s, TIOCSCTTY, NULL) < 0)
			die("ioctl TIOCSCTTY failed: %s\n", strerror(errno));
		if (s > 2)
			close(s);
#ifdef __OpenBSD__
		if (pledge("stdio getpw proc exec", NULL) == -1)
			die("pledge\n");
#endif
		execsh(cmd, args);
		break;
	default:
#ifdef __OpenBSD__
		if (pledge("stdio rpath tty proc", NULL) == -1)
			die("pledge\n");
#endif
		close(s);
		st->cmdfd = m;
		break;
	}
	return st->cmdfd;
}

size_t
ttyread(ST *st)
{
	static char buf[BUFSIZ];
	static int buflen = 0;
	int ret, written;

	/* append read bytes to unprocessed bytes */
	ret = read(st->cmdfd, buf+buflen, LEN(buf)-buflen);

	switch (ret) {
	case  0:
	case -1:
		st->close = 1;
                break;
	default:
		buflen += ret;
		written = twrite(st, buf, buflen, 0);
		buflen -= written;
		/* keep any incomplete UTF-8 byte sequence for the next call */
		if (buflen > 0)
			memmove(buf, buf + written, buflen);
		return ret;
	}
}

void
ttywrite(ST *st, const char *s, size_t n, int may_echo)
{
	const char *next;

	if (may_echo && IS_SET(TERM_MODE_ECHO))
		twrite(st, s, n, 1);

	if (!IS_SET(TERM_MODE_CRLF)) {
		ttywriteraw(st, s, n);
		return;
	}

	/* This is similar to how the kernel handles ONLCR for ttys */
	while (n > 0) {
		if (*s == '\r') {
			next = s + 1;
			ttywriteraw(st, "\r\n", 2);
		} else {
			next = memchr(s, '\r', n);
			DEFAULT(next, s + n);
			ttywriteraw(st, s, next - s);
		}
		n -= next - s;
		s = next;
	}
}

void
ttywriteraw(ST *st, const char *s, size_t n)
{
	fd_set wfd, rfd;
	ssize_t r;
	size_t lim = 256;

	/*
	 * Remember that we are using a pty, which might be a modem line.
	 * Writing too much will clog the line. That's why we are doing this
	 * dance.
	 * FIXME: Migrate the world to Plan 9.
	 */
	while (n > 0) {
		FD_ZERO(&wfd);
		FD_ZERO(&rfd);
		FD_SET(st->cmdfd, &wfd);
		FD_SET(st->cmdfd, &rfd);

		/* Check if we can write. */
		if (pselect(st->cmdfd+1, &rfd, &wfd, NULL, NULL, NULL) < 0) {
			if (errno == EINTR)
				continue;
			die("select failed: %s\n", strerror(errno));
		}
		if (FD_ISSET(st->cmdfd, &wfd)) {
			/*
			 * Only write the bytes written by ttywrite() or the
			 * default of 256. This seems to be a reasonable value
			 * for a serial line. Bigger values might clog the I/O.
			 */
			if ((r = write(st->cmdfd, s, (n < lim)? n : lim)) < 0)
				goto write_error;
			if (r < n) {
				/*
				 * We weren't able to write out everything.
				 * This means the buffer is getting full
				 * again. Empty it.
				 */
				if (n < lim)
					lim = ttyread(st);
				n -= r;
				s += r;
			} else {
				/* All bytes have been written. */
				break;
			}
		}
		if (FD_ISSET(st->cmdfd, &rfd))
			lim = ttyread(st);
	}
	return;

write_error:
	die("write error on tty: %s\n", strerror(errno));
}

void
ttyresize(ST *st, int tw, int th)
{
	struct winsize w;

	w.ws_row = st->row;
	w.ws_col = st->col;
	w.ws_xpixel = tw;
	w.ws_ypixel = th;
	if (ioctl(st->cmdfd, TIOCSWINSZ, &w) < 0)
		fprintf(stderr, "Couldn't set window size: %s\n", strerror(errno));
}

void
ttyhangup(ST *st)
{
	/* Send SIGHUP to shell */
	kill(st->pid, SIGTERM);
}

int
tattrset(ST *st, int attr)
{
	int i, j;

	for (i = 0; i < st->row-1; i++) {
		for (j = 0; j < st->col-1; j++) {
			if (st->line[i][j].mode & attr)
				return 1;
		}
	}

	return 0;
}

void
tsetdirt(ST *st, int top, int bot)
{
	int i;

	LIMIT(top, 0, st->row-1);
	LIMIT(bot, 0, st->row-1);

	for (i = top; i <= bot; i++)
		st->dirty[i] = 1;
}

void
tsetdirtattr(ST *st, int attr)
{
	int i, j;

	for (i = 0; i < st->row-1; i++) {
		for (j = 0; j < st->col-1; j++) {
			if (st->line[i][j].mode & attr) {
				tsetdirt(st, i, i);
				break;
			}
		}
	}
}

void
tfulldirt(ST *st)
{
	tsetdirt(st, 0, st->row-1);
}

void
tcursor(ST *st, int mode)
{
	static TCursor c[2];
	int alt = IS_SET(TERM_MODE_ALTSCREEN);

	if (mode == CURSOR_SAVE) {
		c[alt] = st->c;
	} else if (mode == CURSOR_LOAD) {
		st->c = c[alt];
		tmoveto(st, c[alt].x, c[alt].y);
	}
}

void
treset(ST *st)
{
	uint i;

	st->c = (TCursor){{
		.mode = ATTR_NULL,
		.fg = defaultfg,
		.bg = defaultbg
	}, .x = 0, .y = 0, .state = CURSOR_DEFAULT};

	memset(st->tabs, 0, st->col * sizeof(*st->tabs));
	for (i = tabspaces; i < st->col; i += tabspaces)
		st->tabs[i] = 1;
	st->top = 0;
	if (st->history != NULL) {
		for (int i = 0; i < st->scrollup; ++i) {
			free(st->history[i]);
		}
		free(st->history);
		free(st->historyCols);
	}
	st->history = NULL;
        st->close = 0;
	st->scrollup = 0;
        st->maxScrollup = 100;
	st->bot = st->row - 1;
	st->mode = TERM_MODE_WRAP|TERM_MODE_UTF8;
	memset(st->trantbl, CS_USA, sizeof(st->trantbl));
	st->charset = 0;

	for (i = 0; i < 2; i++) {
		tmoveto(st, 0, 0);
		tcursor(st, CURSOR_SAVE);
		tclearregion(st, 0, 0, st->col-1, st->row-1);
		tswapscreen(st);
	}
}

ST*
tnew(int col, int row)
{
	ST *st = malloc(sizeof(ST));
        if (!st) {return NULL;}
        *st = (ST){ .c = { .attr = { .fg = defaultfg, .bg = defaultbg } } };
        st->iofd = 1;
	tresize(st, col, row);
	treset(st);
        return st;
}

void
tfree(ST *st)
{
	if (st->history != NULL) {
		for (int i = 0; i < st->scrollup; ++i) {
		    free(st->history[i]);
		}
		free(st->history);
		free(st->historyCols);
	}
 
	/* resize each row to new width, zero-pad if needed */
	for (int i = 0; i < st->row; i++) {
		free(st->line[i]);
		free(st->alt[i]);
	}
	
	free(st->line);
	free(st->alt);  
	free(st->dirty);
	free(st->tabs); 

        free(st);
}

void
tswapscreen(ST *st)
{
	Line *tmp = st->line;

	st->line = st->alt;
	st->alt = tmp;
	st->mode ^= TERM_MODE_ALTSCREEN;
	tfulldirt(st);
}

void
tscrolldown(ST *st, int orig, int n)
{
	int i;
	Line temp;

	LIMIT(n, 0, st->bot-orig+1);

	tsetdirt(st, orig, st->bot-n);
	tclearregion(st, 0, st->bot-n+1, st->col-1, st->bot);

	for (i = st->bot; i >= orig+n; i--) {
		temp = st->line[i];
		st->line[i] = st->line[i-n];
		st->line[i-n] = temp;
	}

	selscroll(st, orig, n);
}

void
tscrollup(ST *st, int orig, int n)
{
	int i;
	Line temp;

	LIMIT(n, 0, st->bot-orig+1);

	tclearregion(st, 0, orig, st->col-1, orig+n-1);
	tsetdirt(st, orig+n, st->bot);

	for (i = orig; i <= st->bot-n; i++) {
		temp = st->line[i];
		st->line[i] = st->line[i+n];
		st->line[i+n] = temp;
	}

	selscroll(st, orig, -n);
}

void
selscroll(ST *st, int orig, int n)
{
	if (st->sel.ob.x == -1)
		return;

	if (BETWEEN(st->sel.nb.y, orig, st->bot) != BETWEEN(st->sel.ne.y, orig, st->bot)) {
		selclear(st);
	} else if (BETWEEN(st->sel.nb.y, orig, st->bot)) {
		st->sel.ob.y += n;
		st->sel.oe.y += n;
		if (st->sel.ob.y < st->top || st->sel.ob.y > st->bot ||
		    st->sel.oe.y < st->top || st->sel.oe.y > st->bot) {
			selclear(st);
		} else {
			selnormalize(st);
		}
	}
}

void
tnewline(ST *st, int first_col)
{
	int y = st->c.y;
	if (y == st->bot && st->scrollup < st->maxScrollup) {
		if (!st->history) {
			st->history = malloc(sizeof(Glyph*));
			st->history[0] = malloc(sizeof(Glyph)*st->col);
			st->historyCols = malloc(sizeof(int));
		} else {
			st->history = realloc(st->history, sizeof(Glyph*)*(st->scrollup+1));
			st->history[st->scrollup] = malloc(sizeof(Glyph)*st->col);
			st->historyCols = realloc(st->historyCols, sizeof(int)*(st->scrollup+1));
		}
                st->historyCols[st->scrollup] = st->col;
		memcpy(st->history[st->scrollup++], st->line[0], sizeof(Glyph)*st->col);
		tscrollup(st, st->top, 1);
        } else if (y == st->bot && st->scrollup == st->maxScrollup) {
                free(st->history[0]);
                for (int i = 0; i < st->maxScrollup-1; ++i) {
                        st->history[i] = st->history[i+1];
                        st->historyCols[i] = st->historyCols[i+1];
                }
                st->historyCols[st->maxScrollup-1] = st->col;
		st->history[st->maxScrollup-1] = malloc(sizeof(Glyph)*st->col);
		memcpy(st->history[st->maxScrollup-1], st->line[0], sizeof(Glyph)*st->col);
		tscrollup(st, st->top, 1);
	} else {
		y++;
	}
	tmoveto(st, first_col ? 0 : st->c.x, y);
}

void
csiparse(ST *st)
{
	char *p = st->csiescseq.buf, *np;
	long int v;

	st->csiescseq.narg = 0;
	if (*p == '?') {
		st->csiescseq.priv = 1;
		p++;
	}

	st->csiescseq.buf[st->csiescseq.len] = '\0';
	while (p < st->csiescseq.buf+st->csiescseq.len) {
		np = NULL;
		v = strtol(p, &np, 10);
		if (np == p)
			v = 0;
		if (v == LONG_MAX || v == LONG_MIN)
			v = -1;
		st->csiescseq.arg[st->csiescseq.narg++] = v;
		p = np;
		if (*p != ';' || st->csiescseq.narg == ESC_ARG_SIZ)
			break;
		p++;
	}
	st->csiescseq.mode[0] = *p++;
	st->csiescseq.mode[1] = (p < st->csiescseq.buf+st->csiescseq.len) ? *p : '\0';
}

/* for absolute user moves, when decom is set */
void
tmoveato(ST *st, int x, int y)
{
	tmoveto(st, x, y + ((st->c.state & CURSOR_ORIGIN) ? st->top: 0));
}

void
tmoveto(ST *st, int x, int y)
{
	int miny, maxy;

	if (st->c.state & CURSOR_ORIGIN) {
		miny = st->top;
		maxy = st->bot;
	} else {
		miny = 0;
		maxy = st->row - 1;
	}
	st->c.state &= ~CURSOR_WRAPNEXT;
	st->c.x = LIMIT(x, 0, st->col-1);
	st->c.y = LIMIT(y, miny, maxy);
}

void
tsetchar(ST *st, Rune u, const Glyph *attr, int x, int y)
{
	static const char *vt100_0[62] = { /* 0x41 - 0x7e */
		"↑", "↓", "→", "←", "█", "▚", "☃", /* A - G */
		0, 0, 0, 0, 0, 0, 0, 0, /* H - O */
		0, 0, 0, 0, 0, 0, 0, 0, /* P - W */
		0, 0, 0, 0, 0, 0, 0, " ", /* X - _ */
		"◆", "▒", "␉", "␌", "␍", "␊", "°", "±", /* ` - g */
		"␤", "␋", "┘", "┐", "┌", "└", "┼", "⎺", /* h - o */
		"⎻", "─", "⎼", "⎽", "├", "┤", "┴", "┬", /* p - w */
		"│", "≤", "≥", "π", "≠", "£", "·", /* x - ~ */
	};

	/*
	 * The table is proudly stolen from rxvt.
	 */
	if (st->trantbl[st->charset] == CS_GRAPHIC0 &&
	   BETWEEN(u, 0x41, 0x7e) && vt100_0[u - 0x41])
		utf8decode(st, vt100_0[u - 0x41], &u, UTF_SIZ);

	if (st->line[y][x].mode & ATTR_WIDE) {
		if (x+1 < st->col) {
			st->line[y][x+1].u = ' ';
			st->line[y][x+1].mode &= ~ATTR_WDUMMY;
		}
	} else if (st->line[y][x].mode & ATTR_WDUMMY) {
		st->line[y][x-1].u = ' ';
		st->line[y][x-1].mode &= ~ATTR_WIDE;
	}

	st->dirty[y] = 1;
	st->line[y][x] = *attr;
	st->line[y][x].u = u;
}

void
tclearregion(ST *st, int x1, int y1, int x2, int y2)
{
	int x, y, temp;
	Glyph *gp;

	if (x1 > x2)
		temp = x1, x1 = x2, x2 = temp;
	if (y1 > y2)
		temp = y1, y1 = y2, y2 = temp;

	LIMIT(x1, 0, st->col-1);
	LIMIT(x2, 0, st->col-1);
	LIMIT(y1, 0, st->row-1);
	LIMIT(y2, 0, st->row-1);

	for (y = y1; y <= y2; y++) {
		st->dirty[y] = 1;
		for (x = x1; x <= x2; x++) {
			gp = &st->line[y][x];
			if (selected(st, x, y))
				selclear(st);
			gp->fg = st->c.attr.fg;
			gp->bg = st->c.attr.bg;
			gp->mode = 0;
			gp->u = ' ';
		}
	}
}

void
tdeletechar(ST *st, int n)
{
	int dst, src, size;
	Glyph *line;

	LIMIT(n, 0, st->col - st->c.x);

	dst = st->c.x;
	src = st->c.x + n;
	size = st->col - src;
	line = st->line[st->c.y];

	memmove(&line[dst], &line[src], size * sizeof(Glyph));
	tclearregion(st, st->col-n, st->c.y, st->col-1, st->c.y);
}

void
tinsertblank(ST *st, int n)
{
	int dst, src, size;
	Glyph *line;

	LIMIT(n, 0, st->col - st->c.x);

	dst = st->c.x + n;
	src = st->c.x;
	size = st->col - dst;
	line = st->line[st->c.y];

	memmove(&line[dst], &line[src], size * sizeof(Glyph));
	tclearregion(st, src, st->c.y, dst - 1, st->c.y);
}

void
tinsertblankline(ST *st, int n)
{
	if (BETWEEN(st->c.y, st->top, st->bot))
		tscrolldown(st, st->c.y, n);
}

void
tdeleteline(ST *st, int n)
{
	if (BETWEEN(st->c.y, st->top, st->bot))
		tscrollup(st, st->c.y, n);
}

int32_t
tdefcolor(const int *attr, int *npar, int l)
{
	int32_t idx = -1;
	uint r, g, b;

	switch (attr[*npar + 1]) {
	case 2: /* direct color in RGB space */
		if (*npar + 4 >= l) {
			fprintf(stderr,
				"erresc(38): Incorrect number of parameters (%d)\n",
				*npar);
			break;
		}
		r = attr[*npar + 2];
		g = attr[*npar + 3];
		b = attr[*npar + 4];
		*npar += 4;
		if (!BETWEEN(r, 0, 255) || !BETWEEN(g, 0, 255) || !BETWEEN(b, 0, 255))
			fprintf(stderr, "erresc: bad rgb color (%u,%u,%u)\n",
				r, g, b);
		else
			idx = TRUECOLOR(r, g, b);
		break;
	case 5: /* indexed color */
		if (*npar + 2 >= l) {
			fprintf(stderr,
				"erresc(38): Incorrect number of parameters (%d)\n",
				*npar);
			break;
		}
		*npar += 2;
		if (!BETWEEN(attr[*npar], 0, 255))
			fprintf(stderr, "erresc: bad fgcolor %d\n", attr[*npar]);
		else
			idx = attr[*npar];
		break;
	case 0: /* implemented defined (only foreground) */
	case 1: /* transparent */
	case 3: /* direct color in CMY space */
	case 4: /* direct color in CMYK space */
	default:
		fprintf(stderr,
		        "erresc(38): gfx attr %d unknown\n", attr[*npar]);
		break;
	}

	return idx;
}

void
tsetattr(ST *st, const int *attr, int l)
{
	int i;
	int32_t idx;

	for (i = 0; i < l; i++) {
		switch (attr[i]) {
		case 0:
			st->c.attr.mode &= ~(
				ATTR_BOLD       |
				ATTR_FAINT      |
				ATTR_ITALIC     |
				ATTR_UNDERLINE  |
				ATTR_BLINK      |
				ATTR_REVERSE    |
				ATTR_INVISIBLE  |
				ATTR_STRUCK     );
			st->c.attr.fg = defaultfg;
			st->c.attr.bg = defaultbg;
			break;
		case 1:
			st->c.attr.mode |= ATTR_BOLD;
			break;
		case 2:
			st->c.attr.mode |= ATTR_FAINT;
			break;
		case 3:
			st->c.attr.mode |= ATTR_ITALIC;
			break;
		case 4:
			st->c.attr.mode |= ATTR_UNDERLINE;
			break;
		case 5: /* slow blink */
			/* FALLTHROUGH */
		case 6: /* rapid blink */
			st->c.attr.mode |= ATTR_BLINK;
			break;
		case 7:
			st->c.attr.mode |= ATTR_REVERSE;
			break;
		case 8:
			st->c.attr.mode |= ATTR_INVISIBLE;
			break;
		case 9:
			st->c.attr.mode |= ATTR_STRUCK;
			break;
		case 22:
			st->c.attr.mode &= ~(ATTR_BOLD | ATTR_FAINT);
			break;
		case 23:
			st->c.attr.mode &= ~ATTR_ITALIC;
			break;
		case 24:
			st->c.attr.mode &= ~ATTR_UNDERLINE;
			break;
		case 25:
			st->c.attr.mode &= ~ATTR_BLINK;
			break;
		case 27:
			st->c.attr.mode &= ~ATTR_REVERSE;
			break;
		case 28:
			st->c.attr.mode &= ~ATTR_INVISIBLE;
			break;
		case 29:
			st->c.attr.mode &= ~ATTR_STRUCK;
			break;
		case 38:
			if ((idx = tdefcolor(attr, &i, l)) >= 0)
				st->c.attr.fg = idx;
			break;
		case 39:
			st->c.attr.fg = defaultfg;
			break;
		case 48:
			if ((idx = tdefcolor(attr, &i, l)) >= 0)
				st->c.attr.bg = idx;
			break;
		case 49:
			st->c.attr.bg = defaultbg;
			break;
		default:
			if (BETWEEN(attr[i], 30, 37)) {
				st->c.attr.fg = attr[i] - 30;
			} else if (BETWEEN(attr[i], 40, 47)) {
				st->c.attr.bg = attr[i] - 40;
			} else if (BETWEEN(attr[i], 90, 97)) {
				st->c.attr.fg = attr[i] - 90 + 8;
			} else if (BETWEEN(attr[i], 100, 107)) {
				st->c.attr.bg = attr[i] - 100 + 8;
			} else {
				fprintf(stderr,
					"erresc(default): gfx attr %d unknown\n",
					attr[i]);
				csidump(st);
			}
			break;
		}
	}
}

void
tsetscroll(ST *st, int t, int b)
{
	int temp;

	LIMIT(t, 0, st->row-1);
	LIMIT(b, 0, st->row-1);
	if (t > b) {
		temp = t;
		t = b;
		b = temp;
	}
	st->top = t;
	st->bot = b;
}

void
static xsetmode(ST *st, int set, unsigned int flags)
{
        int mode = st->windowMode;
        MODBIT(st->windowMode, set, flags);
        if ((st->windowMode & MODE_REVERSE) != (mode & MODE_REVERSE))
                redraw(st);
}

void
tsetmode(ST *st, int priv, int set, const int *args, int narg)
{
	int alt; const int *lim;

	for (lim = args + narg; args < lim; ++args) {
		if (priv) {
			switch (*args) {
			case 1: /* DECCKM -- Cursor key */
				xsetmode(st, set, MODE_APPCURSOR);
				break;
			case 5: /* DECSCNM -- Reverse video */
  				xsetmode(st, set, MODE_REVERSE);
				break;
			case 6: /* DECOM -- Origin */
				MODBIT(st->c.state, set, CURSOR_ORIGIN);
				tmoveato(st, 0, 0);
				break;
			case 7: /* DECAWM -- Auto wrap */
				MODBIT(st->mode, set, TERM_MODE_WRAP);
				break;
			case 0:  /* Error (IGNORED) */
			case 2:  /* DECANM -- ANSI/VT52 (IGNORED) */
			case 3:  /* DECCOLM -- Column  (IGNORED) */
			case 4:  /* DECSCLM -- Scroll (IGNORED) */
			case 8:  /* DECARM -- Auto repeat (IGNORED) */
			case 18: /* DECPFF -- Printer feed (IGNORED) */
			case 19: /* DECPEX -- Printer extent (IGNORED) */
			case 42: /* DECNRCM -- National characters (IGNORED) */
			case 12: /* att610 -- Start blinking cursor (IGNORED) */
				break;
			case 25: /* DECTCEM -- Text Cursor Enable Mode */
				xsetmode(st, !set, MODE_HIDE);
				break;
			case 9:    /* X10 mouse compatibility mode */
//				xsetpoinstotion(0);
				xsetmode(st, 0, MODE_MOUSE);
				xsetmode(st, set, MODE_MOUSEX10);
				break;
			case 1000: /* 1000: report button press */
//				xsetpoinstotion(0);
				xsetmode(st, 0, MODE_MOUSE);
				xsetmode(st, set, MODE_MOUSEBTN);
				break;
			case 1002: /* 1002: report motion on button press */
//				xsetpoinstotion(0);
				xsetmode(st, 0, MODE_MOUSE);
				xsetmode(st, set, MODE_MOUSEMOTION);
				break;
			case 1003: /* 1003: enable all mouse motions */
//				xsetpoinstotion(set);
				xsetmode(st, 0, MODE_MOUSE);
				xsetmode(st, set, MODE_MOUSEMANY);
				break;
			case 1004: /* 1004: send focus events to tty */
				xsetmode(st, set, MODE_FOCUS);
				break;
			case 1006: /* 1006: extended reporting mode */
				xsetmode(st, set, MODE_MOUSESGR);
				break;
			case 1034:
				xsetmode(st, set, MODE_8BIT);
				break;
			case 1049: /* swap screen & set/restore cursor as xst */
				if (!allowaltscreen)
					break;
				tcursor(st, (set) ? CURSOR_SAVE : CURSOR_LOAD);
				/* FALLTHROUGH */
			case 47: /* swap screen */
			case 1047:
				if (!allowaltscreen)
					break;
				alt = IS_SET(TERM_MODE_ALTSCREEN);
				if (alt) {
					tclearregion(st, 0, 0, st->col-1,
							st->row-1);
				}
				if (set ^ alt) /* set is always 1 or 0 */
					tswapscreen(st);
				if (*args != 1049)
					break;
				/* FALLTHROUGH */
			case 1048:
				tcursor(st, (set) ? CURSOR_SAVE : CURSOR_LOAD);
				break;
			case 2004: /* 2004: bracketed paste mode */
				xsetmode(st, set, MODE_BRCKTPASTE);
				break;
			/* Not implemented mouse modes. See comments there. */
			case 1001: /* mouse highlight mode; can hang the
				      stinal by design when implemented. */
			case 1005: /* UTF-8 mouse mode; will confuse
				      applications not supporting UTF-8
				      and luit. */
			case 1015: /* urxvt mangled mouse mode; incompatible
				      and can be mistaken for other control
				      codes. */
				break;
			default:
				fprintf(stderr,
					"erresc: unknown private set/reset mode %d\n",
					*args);
				break;
			}
		} else {
			switch (*args) {
			case 0:  /* Error (IGNORED) */
				break;
			case 2:
				xsetmode(st, set, MODE_KBDLOCK);
				break;
			case 4:  /* IRM -- Insertion-replacement */
				MODBIT(st->mode, set, TERM_MODE_INSERT);
				break;
			case 12: /* SRM -- Send/Receive */
				MODBIT(st->mode, !set, TERM_MODE_ECHO);
				break;
			case 20: /* LNM -- Linefeed/new line */
				MODBIT(st->mode, set, TERM_MODE_CRLF);
				break;
			default:
				fprintf(stderr,
					"erresc: unknown set/reset mode %d\n",
					*args);
				break;
			}
		}
	}
}

void
csihandle(ST *st)
{
	char buf[40];
	int len;

	switch (st->csiescseq.mode[0]) {
	default:
	unknown:
		fprintf(stderr, "erresc: unknown csi ");
		csidump(st);
		/* die(""); */
		break;
	case '@': /* ICH -- Insert <n> blank char */
		DEFAULT(st->csiescseq.arg[0], 1);
		tinsertblank(st, st->csiescseq.arg[0]);
		break;
	case 'A': /* CUU -- Cursor <n> Up */
		DEFAULT(st->csiescseq.arg[0], 1);
		tmoveto(st, st->c.x, st->c.y-st->csiescseq.arg[0]);
		break;
	case 'B': /* CUD -- Cursor <n> Down */
	case 'e': /* VPR --Cursor <n> Down */
		DEFAULT(st->csiescseq.arg[0], 1);
		tmoveto(st, st->c.x, st->c.y+st->csiescseq.arg[0]);
		break;
	case 'i': /* MC -- Media Copy */
		switch (st->csiescseq.arg[0]) {
		case 0:
			tdump(st);
			break;
		case 1:
			tdumpline(st, st->c.y);
			break;
		case 2:
			tdumpsel(st);
			break;
		case 4:
			st->mode &= ~TERM_MODE_PRINT;
			break;
		case 5:
			st->mode |= TERM_MODE_PRINT;
			break;
		}
		break;
	case 'c': /* DA -- Device Attributes */
		if (st->csiescseq.arg[0] == 0)
			ttywrite(st, vtiden, strlen(vtiden), 0);
		break;
	case 'b': /* REP -- if last char is printable print it <n> more times */
		DEFAULT(st->csiescseq.arg[0], 1);
		if (st->lastc)
			while (st->csiescseq.arg[0]-- > 0)
				tputc(st, st->lastc);
		break;
	case 'C': /* CUF -- Cursor <n> Forward */
	case 'a': /* HPR -- Cursor <n> Forward */
		DEFAULT(st->csiescseq.arg[0], 1);
		tmoveto(st, st->c.x+st->csiescseq.arg[0], st->c.y);
		break;
	case 'D': /* CUB -- Cursor <n> Backward */
		DEFAULT(st->csiescseq.arg[0], 1);
		tmoveto(st, st->c.x-st->csiescseq.arg[0], st->c.y);
		break;
	case 'E': /* CNL -- Cursor <n> Down and first col */
		DEFAULT(st->csiescseq.arg[0], 1);
		tmoveto(st, 0, st->c.y+st->csiescseq.arg[0]);
		break;
	case 'F': /* CPL -- Cursor <n> Up and first col */
		DEFAULT(st->csiescseq.arg[0], 1);
		tmoveto(st, 0, st->c.y-st->csiescseq.arg[0]);
		break;
	case 'g': /* TBC -- Tabulation clear */
		switch (st->csiescseq.arg[0]) {
		case 0: /* clear current tab stop */
			st->tabs[st->c.x] = 0;
			break;
		case 3: /* clear all the tabs */
			memset(st->tabs, 0, st->col * sizeof(*st->tabs));
			break;
		default:
			goto unknown;
		}
		break;
	case 'G': /* CHA -- Move to <col> */
	case '`': /* HPA */
		DEFAULT(st->csiescseq.arg[0], 1);
		tmoveto(st, st->csiescseq.arg[0]-1, st->c.y);
		break;
	case 'H': /* CUP -- Move to <row> <col> */
	case 'f': /* HVP */
		DEFAULT(st->csiescseq.arg[0], 1);
		DEFAULT(st->csiescseq.arg[1], 1);
		tmoveato(st, st->csiescseq.arg[1]-1, st->csiescseq.arg[0]-1);
		break;
	case 'I': /* CHT -- Cursor Forward Tabulation <n> tab stops */
		DEFAULT(st->csiescseq.arg[0], 1);
		tputtab(st, st->csiescseq.arg[0]);
		break;
	case 'J': /* ED -- Clear screen */
		switch (st->csiescseq.arg[0]) {
		case 0: /* below */
			tclearregion(st, st->c.x, st->c.y, st->col-1, st->c.y);
			if (st->c.y < st->row-1) {
				tclearregion(st, 0, st->c.y+1, st->col-1,
						st->row-1);
			}
			break;
		case 1: /* above */
			if (st->c.y > 1)
				tclearregion(st, 0, 0, st->col-1, st->c.y-1);
			tclearregion(st, 0, st->c.y, st->c.x, st->c.y);
			break;
		case 2: /* all */
			tclearregion(st, 0, 0, st->col-1, st->row-1);
			break;
		default:
			goto unknown;
		}
		break;
	case 'K': /* EL -- Clear line */
		switch (st->csiescseq.arg[0]) {
		case 0: /* right */
			tclearregion(st, st->c.x, st->c.y, st->col-1,
					st->c.y);
			break;
		case 1: /* left */
			tclearregion(st, 0, st->c.y, st->c.x, st->c.y);
			break;
		case 2: /* all */
			tclearregion(st, 0, st->c.y, st->col-1, st->c.y);
			break;
		}
		break;
	case 'S': /* SU -- Scroll <n> line up */
		DEFAULT(st->csiescseq.arg[0], 1);
		tscrollup(st, st->top, st->csiescseq.arg[0]);
		break;
	case 'T': /* SD -- Scroll <n> line down */
		DEFAULT(st->csiescseq.arg[0], 1);
		tscrolldown(st, st->top, st->csiescseq.arg[0]);
		break;
	case 'L': /* IL -- Insert <n> blank lines */
		DEFAULT(st->csiescseq.arg[0], 1);
		tinsertblankline(st, st->csiescseq.arg[0]);
		break;
	case 'l': /* RM -- Reset Mode */
		tsetmode(st, st->csiescseq.priv, 0, st->csiescseq.arg, st->csiescseq.narg);
		break;
	case 'M': /* DL -- Delete <n> lines */
		DEFAULT(st->csiescseq.arg[0], 1);
		tdeleteline(st, st->csiescseq.arg[0]);
		break;
	case 'X': /* ECH -- Erase <n> char */
		DEFAULT(st->csiescseq.arg[0], 1);
		tclearregion(st, st->c.x, st->c.y,
				st->c.x + st->csiescseq.arg[0] - 1, st->c.y);
		break;
	case 'P': /* DCH -- Delete <n> char */
		DEFAULT(st->csiescseq.arg[0], 1);
		tdeletechar(st, st->csiescseq.arg[0]);
		break;
	case 'Z': /* CBT -- Cursor Backward Tabulation <n> tab stops */
		DEFAULT(st->csiescseq.arg[0], 1);
		tputtab(st, -st->csiescseq.arg[0]);
		break;
	case 'd': /* VPA -- Move to <row> */
		DEFAULT(st->csiescseq.arg[0], 1);
		tmoveato(st, st->c.x, st->csiescseq.arg[0]-1);
		break;
	case 'h': /* SM -- Set stinal mode */
		tsetmode(st, st->csiescseq.priv, 1, st->csiescseq.arg, st->csiescseq.narg);
		break;
	case 'm': /* SGR -- STinal attribute (color) */
		tsetattr(st, st->csiescseq.arg, st->csiescseq.narg);
		break;
	case 'n': /* DSR – Device Status Report (cursor position) */
		if (st->csiescseq.arg[0] == 6) {
			len = snprintf(buf, sizeof(buf), "\033[%i;%iR",
					st->c.y+1, st->c.x+1);
			ttywrite(st, buf, len, 0);
		}
		break;
	case 'r': /* DECSTBM -- Set Scrolling Region */
		if (st->csiescseq.priv) {
			goto unknown;
		} else {
			DEFAULT(st->csiescseq.arg[0], 1);
			DEFAULT(st->csiescseq.arg[1], st->row);
			tsetscroll(st, st->csiescseq.arg[0]-1, st->csiescseq.arg[1]-1);
			tmoveato(st, 0, 0);
		}
		break;
	case 's': /* DECSC -- Save cursor position (ANSI.SYS) */
		tcursor(st, CURSOR_SAVE);
		break;
	case 'u': /* DECRC -- Restore cursor position (ANSI.SYS) */
		tcursor(st, CURSOR_LOAD);
		break;
	case ' ':
		switch (st->csiescseq.mode[1]) {
		case 'q': /* DECSCUSR -- Set Cursor Style */
//			if (xsetcursor(st, st->csiescseq.arg[0]))
//				goto unknown;
			break;
		default:
			goto unknown;
		}
		break;
	}
}

void
csidump(ST *st)
{
	size_t i;
	uint c;

	fprintf(stderr, "ESC[");
	for (i = 0; i < st->csiescseq.len; i++) {
		c = st->csiescseq.buf[i] & 0xff;
		if (isprint(c)) {
			putc(c, stderr);
		} else if (c == '\n') {
			fprintf(stderr, "(\\n)");
		} else if (c == '\r') {
			fprintf(stderr, "(\\r)");
		} else if (c == 0x1b) {
			fprintf(stderr, "(\\e)");
		} else {
			fprintf(stderr, "(%02x)", c);
		}
	}
	putc('\n', stderr);
}

void
csireset(ST *st)
{
	memset(&st->csiescseq, 0, sizeof(st->csiescseq));
}

void
osc4_color_response(ST *st, int num)
{
	int n;
	char buf[32];
	unsigned char r, g, b;

//	if (xgetcolor(num, &r, &g, &b)) {
//		fprintf(stderr, "erresc: failed to fetch osc4 color %d\n", num);
//		return;
//	}

	n = snprintf(buf, sizeof buf, "\033]4;%d;rgb:%02x%02x/%02x%02x/%02x%02x\007",
		     num, r, r, g, g, b, b);

	ttywrite(st, buf, n, 1);
}

void
osc_color_response(ST *st, int index, int num)
{
	int n;
	char buf[32];
	unsigned char r, g, b;

//	if (xgetcolor(index, &r, &g, &b)) {
//		fprintf(stderr, "erresc: failed to fetch osc color %d\n", index);
//		return;
//	}

	n = snprintf(buf, sizeof buf, "\033]%d;rgb:%02x%02x/%02x%02x/%02x%02x\007",
		     num, r, r, g, g, b, b);

	ttywrite(st, buf, n, 1);
}

void
strhandle(ST *st)
{
	char *p = NULL, *dec;
	int j, narg, par;

	st->esc &= ~(ESC_STR_END|ESC_STR);
	strparse(st);
	par = (narg = st->strescseq.narg) ? atoi(st->strescseq.args[0]) : 0;

	switch (st->strescseq.type) {
	case ']': /* OSC -- Operating System Command */
		switch (par) {
		case 0:
			if (narg > 1) {
//				xsettitle(st->strescseq.args[1]);
//				xseticontitle(st->strescseq.args[1]);
			}
			return;
		case 1:
			if (narg > 1)
//				xseticontitle(st->strescseq.args[1]);
			return;
		case 2:
			if (narg > 1)
//				xsettitle(st->strescseq.args[1]);
			return;
		case 52:
			if (narg > 2 && allowwindowops) {
				dec = base64dec(st->strescseq.args[2]);
				if (dec) {
//					xsetsel(dec);
//					xclipcopy();
				} else {
					fprintf(stderr, "erresc: invalid base64\n");
				}
			}
			return;
		case 10:
			if (narg < 2)
				break;

			p = st->strescseq.args[1];

			if (!strcmp(p, "?"))
				osc_color_response(st, defaultfg, 10);
//			else if (xsetcolorname(defaultfg, p))
//				fprintf(stderr, "erresc: invalid foreground color: %s\n", p);
			else
				redraw(st);
			return;
		case 11:
			if (narg < 2)
				break;

			p = st->strescseq.args[1];

			if (!strcmp(p, "?"))
				osc_color_response(st, defaultbg, 11);
//			else if (xsetcolorname(defaultbg, p))
//				fprintf(stderr, "erresc: invalid background color: %s\n", p);
			else
				redraw(st);
			return;
		case 12:
			if (narg < 2)
				break;

			p = st->strescseq.args[1];

			if (!strcmp(p, "?"))
				osc_color_response(st, defaultcs, 12);
//			else if (xsetcolorname(defaultcs, p))
//				fprintf(stderr, "erresc: invalid cursor color: %s\n", p);
			else
				redraw(st);
			return;
		case 4: /* color set */
			if (narg < 3)
				break;
			p = st->strescseq.args[2];
			/* FALLTHROUGH */
		case 104: /* color reset */
			j = (narg > 1) ? atoi(st->strescseq.args[1]) : -1;

			if (p && !strcmp(p, "?")) {
				osc4_color_response(st, j);
//			else if (xsetcolorname(j, p)) {
//				if (par == 104 && narg <= 1)
//					return; /* color reset without parameter */
//				fprintf(stderr, "erresc: invalid color j=%d, p=%s\n",
//				        j, p ? p : "(null)");
			} else {
				/*
				 * TODO if defaultbg color is changed, borders
				 * are dirty
				 */
				redraw(st);
			}
			return;
		}
		break;
	case 'k': /* old title set compatibility */
//		xsettitle(st->strescseq.args[0]);
		return;
	case 'P': /* DCS -- Device Control String */
	case '_': /* APC -- Application Program Command */
	case '^': /* PM -- Privacy Message */
		return;
	}

	fprintf(stderr, "erresc: unknown str ");
	strdump(st);
}

void
strparse(ST *st)
{
	int c;
	char *p = st->strescseq.buf;

	st->strescseq.narg = 0;
	st->strescseq.buf[st->strescseq.len] = '\0';

	if (*p == '\0')
		return;

	while (st->strescseq.narg < STR_ARG_SIZ) {
		st->strescseq.args[st->strescseq.narg++] = p;
		while ((c = *p) != ';' && c != '\0')
			++p;
		if (c == '\0')
			return;
		*p++ = '\0';
	}
}

void
strdump(ST *st)
{
	size_t i;
	uint c;

	fprintf(stderr, "ESC%c", st->strescseq.type);
	for (i = 0; i < st->strescseq.len; i++) {
		c = st->strescseq.buf[i] & 0xff;
		if (c == '\0') {
			putc('\n', stderr);
			return;
		} else if (isprint(c)) {
			putc(c, stderr);
		} else if (c == '\n') {
			fprintf(stderr, "(\\n)");
		} else if (c == '\r') {
			fprintf(stderr, "(\\r)");
		} else if (c == 0x1b) {
			fprintf(stderr, "(\\e)");
		} else {
			fprintf(stderr, "(%02x)", c);
		}
	}
	fprintf(stderr, "ESC\\\n");
}

void
strreset(ST *st)
{
	st->strescseq = (STREscape){
		.buf = xrealloc(st->strescseq.buf, STR_BUF_SIZ),
		.siz = STR_BUF_SIZ,
	};
}

void
sendbreak(ST *st, const Arg *arg)
{
	if (tcsendbreak(st->cmdfd, 0))
		perror("Error sending break");
}

void
tprinter(ST *st, char *s, size_t len)
{
	if (st->iofd != -1 && xwrite(st->iofd, s, len) < 0) {
		perror("Error writing to output file");
		close(st->iofd);
		st->iofd = -1;
	}
}

void
toggleprinter(ST *st, const Arg *arg)
{
	st->mode ^= TERM_MODE_PRINT;
}

void
printscreen(ST *st, const Arg *arg)
{
	tdump(st);
}

void
printsel(ST *st, const Arg *arg)
{
	tdumpsel(st);
}

void
tdumpsel(ST *st)
{
	char *ptr;

	if ((ptr = getsel(st))) {
		tprinter(st, ptr, strlen(ptr));
		free(ptr);
	}
}

void
tdumpline(ST *st, int n)
{
	char buf[UTF_SIZ];
	const Glyph *bp, *end;

	bp = &st->line[n][0];
	end = &bp[MIN(tlinelen(st, n), st->col) - 1];
	if (bp != end || bp->u != ' ') {
		for ( ; bp <= end; ++bp)
			tprinter(st, buf, utf8encode(bp->u, buf));
	}
	tprinter(st, "\n", 1);
}

void
tdump(ST *st)
{
	int i;

	for (i = 0; i < st->row; ++i)
		tdumpline(st, i);
}

void
tputtab(ST *st, int n)
{
	uint x = st->c.x;

	if (n > 0) {
		while (x < st->col && n--)
			for (++x; x < st->col && !st->tabs[x]; ++x)
				/* nothing */ ;
	} else if (n < 0) {
		while (x > 0 && n++)
			for (--x; x > 0 && !st->tabs[x]; --x)
				/* nothing */ ;
	}
	st->c.x = LIMIT(x, 0, st->col-1);
}

void
tdefutf8(ST *st, char ascii)
{
	if (ascii == 'G')
		st->mode |= TERM_MODE_UTF8;
	else if (ascii == '@')
		st->mode &= ~TERM_MODE_UTF8;
}

void
tdeftran(ST *st, char ascii)
{
	static char cs[] = "0B";
	static int vcs[] = {CS_GRAPHIC0, CS_USA};
	char *p;

	if ((p = strchr(cs, ascii)) == NULL) {
		fprintf(stderr, "esc unhandled charset: ESC ( %c\n", ascii);
	} else {
		st->trantbl[st->icharset] = vcs[p - cs];
	}
}

void
tdectest(ST *st, char c)
{
	int x, y;

	if (c == '8') { /* DEC screen alignment test. */
		for (x = 0; x < st->col; ++x) {
			for (y = 0; y < st->row; ++y)
				tsetchar(st, 'E', &st->c.attr, x, y);
		}
	}
}

void
tstrsequence(ST *st, uchar c)
{
	switch (c) {
	case 0x90:   /* DCS -- Device Control String */
		c = 'P';
		break;
	case 0x9f:   /* APC -- Application Program Command */
		c = '_';
		break;
	case 0x9e:   /* PM -- Privacy Message */
		c = '^';
		break;
	case 0x9d:   /* OSC -- Operating System Command */
		c = ']';
		break;
	}
	strreset(st);
	st->strescseq.type = c;
	st->esc |= ESC_STR;
}

void
tcontrolcode(ST *st, uchar ascii)
{
	switch (ascii) {
	case '\t':   /* HT */
		tputtab(st, 1);
		return;
	case '\b':   /* BS */
		tmoveto(st, st->c.x-1, st->c.y);
		return;
	case '\r':   /* CR */
		tmoveto(st, 0, st->c.y);
		return;
	case '\f':   /* LF */
	case '\v':   /* VT */
	case '\n':   /* LF */
		/* go to first col if the mode is set */
		tnewline(st, IS_SET(TERM_MODE_CRLF));
		return;
	case '\a':   /* BEL */
		if (st->esc & ESC_STR_END) {
			/* backwards compatibility to xst */
			strhandle(st);
		} else {
//			xbell();
		}
		break;
	case '\033': /* ESC */
		csireset(st);
		st->esc &= ~(ESC_CSI|ESC_ALTCHARSET|ESC_TEST);
		st->esc |= ESC_START;
		return;
	case '\016': /* SO (LS1 -- Locking shift 1) */
	case '\017': /* SI (LS0 -- Locking shift 0) */
		st->charset = 1 - (ascii - '\016');
		return;
	case '\032': /* SUB */
		tsetchar(st, '?', &st->c.attr, st->c.x, st->c.y);
		/* FALLTHROUGH */
	case '\030': /* CAN */
		csireset(st);
		break;
	case '\005': /* ENQ (IGNORED) */
	case '\000': /* NUL (IGNORED) */
	case '\021': /* XON (IGNORED) */
	case '\023': /* XOFF (IGNORED) */
	case 0177:   /* DEL (IGNORED) */
		return;
	case 0x80:   /* TODO: PAD */
	case 0x81:   /* TODO: HOP */
	case 0x82:   /* TODO: BPH */
	case 0x83:   /* TODO: NBH */
	case 0x84:   /* TODO: IND */
		break;
	case 0x85:   /* NEL -- Next line */
		tnewline(st, 1); /* always go to first col */
		break;
	case 0x86:   /* TODO: SSA */
	case 0x87:   /* TODO: ESA */
		break;
	case 0x88:   /* HTS -- Horizontal tab stop */
		st->tabs[st->c.x] = 1;
		break;
	case 0x89:   /* TODO: HTJ */
	case 0x8a:   /* TODO: VTS */
	case 0x8b:   /* TODO: PLD */
	case 0x8c:   /* TODO: PLU */
	case 0x8d:   /* TODO: RI */
	case 0x8e:   /* TODO: SS2 */
	case 0x8f:   /* TODO: SS3 */
	case 0x91:   /* TODO: PU1 */
	case 0x92:   /* TODO: PU2 */
	case 0x93:   /* TODO: STS */
	case 0x94:   /* TODO: CCH */
	case 0x95:   /* TODO: MW */
	case 0x96:   /* TODO: SPA */
	case 0x97:   /* TODO: EPA */
	case 0x98:   /* TODO: SOS */
	case 0x99:   /* TODO: SGCI */
		break;
	case 0x9a:   /* DECID -- Identify STinal */
		ttywrite(st, vtiden, strlen(vtiden), 0);
		break;
	case 0x9b:   /* TODO: CSI */
	case 0x9c:   /* TODO: ST */
		break;
	case 0x90:   /* DCS -- Device Control String */
	case 0x9d:   /* OSC -- Operating System Command */
	case 0x9e:   /* PM -- Privacy Message */
	case 0x9f:   /* APC -- Application Program Command */
		tstrsequence(st, ascii);
		return;
	}
	/* only CAN, SUB, \a and C1 chars interrupt a sequence */
	st->esc &= ~(ESC_STR_END|ESC_STR);
}

/*
 * returns 1 when the sequence is finished and it hasn't to read
 * more characters for this sequence, otherwise 0
 */
int
eschandle(ST *st, uchar ascii)
{
	switch (ascii) {
	case '[':
		st->esc |= ESC_CSI;
		return 0;
	case '#':
		st->esc |= ESC_TEST;
		return 0;
	case '%':
		st->esc |= ESC_UTF8;
		return 0;
	case 'P': /* DCS -- Device Control String */
	case '_': /* APC -- Application Program Command */
	case '^': /* PM -- Privacy Message */
	case ']': /* OSC -- Operating System Command */
	case 'k': /* old title set compatibility */
		tstrsequence(st, ascii);
		return 0;
	case 'n': /* LS2 -- Locking shift 2 */
	case 'o': /* LS3 -- Locking shift 3 */
		st->charset = 2 + (ascii - 'n');
		break;
	case '(': /* GZD4 -- set primary charset G0 */
	case ')': /* G1D4 -- set secondary charset G1 */
	case '*': /* G2D4 -- set tertiary charset G2 */
	case '+': /* G3D4 -- set quaternary charset G3 */
		st->icharset = ascii - '(';
		st->esc |= ESC_ALTCHARSET;
		return 0;
	case 'D': /* IND -- Linefeed */
		if (st->c.y == st->bot) {
			tscrollup(st, st->top, 1);
		} else {
			tmoveto(st, st->c.x, st->c.y+1);
		}
		break;
	case 'E': /* NEL -- Next line */
		tnewline(st, 1); /* always go to first col */
		break;
	case 'H': /* HTS -- Horizontal tab stop */
		st->tabs[st->c.x] = 1;
		break;
	case 'M': /* RI -- Reverse index */
		if (st->c.y == st->top) {
			tscrolldown(st, st->top, 1);
		} else {
			tmoveto(st, st->c.x, st->c.y-1);
		}
		break;
	case 'Z': /* DECID -- Identify STinal */
		ttywrite(st, vtiden, strlen(vtiden), 0);
		break;
	case 'c': /* RIS -- Reset to initial state */
		treset(st);
		resettitle();
//		xloadcols();
		break;
	case '=': /* DECPAM -- Application keypad */
		xsetmode(st, 1, MODE_APPKEYPAD);
		break;
	case '>': /* DECPNM -- Normal keypad */
		xsetmode(st, 0, MODE_APPKEYPAD);
		break;
	case '7': /* DECSC -- Save Cursor */
		tcursor(st, CURSOR_SAVE);
		break;
	case '8': /* DECRC -- Restore Cursor */
		tcursor(st, CURSOR_LOAD);
		break;
	case '\\': /* ST -- String STinator */
		if (st->esc & ESC_STR_END)
			strhandle(st);
		break;
	default:
		fprintf(stderr, "erresc: unknown sequence ESC 0x%02X '%c'\n",
			(uchar) ascii, isprint(ascii)? ascii:'.');
		break;
	}
	return 1;
}

void
tputc(ST *st, Rune u)
{
	char c[UTF_SIZ];
	int control;
	int width, len;
	Glyph *gp;

	control = ISCONTROL(u);
	if (u < 127 || !IS_SET(TERM_MODE_UTF8)) {
		c[0] = u;
		width = len = 1;
	} else {
		len = utf8encode(u, c);
		if (!control && (width = wcwidth(u)) == -1)
			width = 1;
	}

	if (IS_SET(TERM_MODE_PRINT))
		tprinter(st, c, len);

	/*
	 * STR sequence must be checked before anything else
	 * because it uses all following characters until it
	 * receives a ESC, a SUB, a ST or any other C1 control
	 * character.
	 */
	if (st->esc & ESC_STR) {
		if (u == '\a' || u == 030 || u == 032 || u == 033 ||
		   ISCONTROLC1(u)) {
			st->esc &= ~(ESC_START|ESC_STR);
			st->esc |= ESC_STR_END;
			goto check_control_code;
		}

		if (st->strescseq.len+len >= st->strescseq.siz) {
			/*
			 * Here is a bug in stinals. If the user never sends
			 * some code to stop the str or esc command, then st
			 * will stop responding. But this is better than
			 * silently failing with unknown characters. At least
			 * then users will report back.
			 *
			 * In the case users ever get fixed, here is the code:
			 */
			/*
			 * st->esc = 0;
			 * strhandle(st);
			 */
			if (st->strescseq.siz > (SIZE_MAX - UTF_SIZ) / 2)
				return;
			st->strescseq.siz *= 2;
			st->strescseq.buf = xrealloc(st->strescseq.buf, st->strescseq.siz);
		}

		memmove(&st->strescseq.buf[st->strescseq.len], c, len);
		st->strescseq.len += len;
		return;
	}

check_control_code:
	/*
	 * Actions of control codes must be performed as soon they arrive
	 * because they can be embedded inside a control sequence, and
	 * they must not cause conflicts with sequences.
	 */
	if (control) {
		tcontrolcode(st, u);
		/*
		 * control codes are not shown ever
		 */
		if (!st->esc)
			st->lastc = 0;
		return;
	} else if (st->esc & ESC_START) {
		if (st->esc & ESC_CSI) {
			st->csiescseq.buf[st->csiescseq.len++] = u;
			if (BETWEEN(u, 0x40, 0x7E)
					|| st->csiescseq.len >= \
					sizeof(st->csiescseq.buf)-1) {
				st->esc = 0;
				csiparse(st);
				csihandle(st);
			}
			return;
		} else if (st->esc & ESC_UTF8) {
			tdefutf8(st, u);
		} else if (st->esc & ESC_ALTCHARSET) {
			tdeftran(st, u);
		} else if (st->esc & ESC_TEST) {
			tdectest(st, u);
		} else {
			if (!eschandle(st, u))
				return;
			/* sequence already finished */
		}
		st->esc = 0;
		/*
		 * All characters which form part of a sequence are not
		 * printed
		 */
		return;
	}
	if (selected(st, st->c.x, st->c.y))
		selclear(st);

	gp = &st->line[st->c.y][st->c.x];
	if (IS_SET(TERM_MODE_WRAP) && (st->c.state & CURSOR_WRAPNEXT)) {
		gp->mode |= ATTR_WRAP;
		tnewline(st, 1);
		gp = &st->line[st->c.y][st->c.x];
	}

	if (IS_SET(TERM_MODE_INSERT) && st->c.x+width < st->col)
		memmove(gp+width, gp, (st->col - st->c.x - width) * sizeof(Glyph));

	if (st->c.x+width > st->col) {
		tnewline(st, 1);
		gp = &st->line[st->c.y][st->c.x];
	}

	tsetchar(st, u, &st->c.attr, st->c.x, st->c.y);
	st->lastc = u;

	if (width == 2) {
		gp->mode |= ATTR_WIDE;
		if (st->c.x+1 < st->col) {
			if (gp[1].mode == ATTR_WIDE && st->c.x+2 < st->col) {
				gp[2].u = ' ';
				gp[2].mode &= ~ATTR_WDUMMY;
			}
			gp[1].u = '\0';
			gp[1].mode = ATTR_WDUMMY;
		}
	}
	if (st->c.x+width < st->col) {
		tmoveto(st, st->c.x+width, st->c.y);
	} else {
		st->c.state |= CURSOR_WRAPNEXT;
	}
}

int
twrite(ST *st, const char *buf, int buflen, int show_ctrl)
{
	int charsize;
	Rune u;
	int n;

	for (n = 0; n < buflen; n += charsize) {
		if (IS_SET(TERM_MODE_UTF8)) {
			/* process a complete utf8 char */
			charsize = utf8decode(st, buf + n, &u, buflen - n);
			if (charsize == 0)
				break;
		} else {
			u = buf[n] & 0xFF;
			charsize = 1;
		}
		if (show_ctrl && ISCONTROL(u)) {
			if (u & 0x80) {
				u &= 0x7f;
				tputc(st, '^');
				tputc(st, '[');
			} else if (u != '\n' && u != '\r' && u != '\t') {
				u ^= 0x40;
				tputc(st, '^');
			}
		}
		tputc(st, u);
	}
	return n;
}

void
tresize(ST *st, int col, int row)
{
	int i;
	int minrow = MIN(row, st->row);
	int mincol = MIN(col, st->col);
	int *bp;
	TCursor c;

	if (col < 1 || row < 1) {
		fprintf(stderr,
		        "tresize: error resizing to %dx%d\n", col, row);
		return;
	}

	/*
	 * slide screen to keep cursor where we expect it -
	 * tscrollup would work here, but we can optimize to
	 * memmove because we're freeing the earlier lines
	 */
	for (i = 0; i <= st->c.y - row; i++) {
		free(st->line[i]);
		free(st->alt[i]);
	}
	/* ensure that both src and dst are not NULL */
	if (i > 0) {
		memmove(st->line, st->line + i, row * sizeof(Line));
		memmove(st->alt, st->alt + i, row * sizeof(Line));
	}
	for (i += row; i < st->row; i++) {
		free(st->line[i]);
		free(st->alt[i]);
	}

	/* resize to new height */
	st->line = xrealloc(st->line, row * sizeof(Line));
	st->alt  = xrealloc(st->alt,  row * sizeof(Line));
	st->dirty = xrealloc(st->dirty, row * sizeof(*st->dirty));
	st->tabs = xrealloc(st->tabs, col * sizeof(*st->tabs));

	/* resize each row to new width, zero-pad if needed */
	for (i = 0; i < minrow; i++) {
		st->line[i] = xrealloc(st->line[i], col * sizeof(Glyph));
		st->alt[i]  = xrealloc(st->alt[i],  col * sizeof(Glyph));
	}

	/* allocate any new rows */
	for (/* i = minrow */; i < row; i++) {
		st->line[i] = xmalloc(col * sizeof(Glyph));
		st->alt[i] = xmalloc(col * sizeof(Glyph));
	}
	if (col > st->col) {
		bp = st->tabs + st->col;

		memset(bp, 0, sizeof(*st->tabs) * (col - st->col));
		while (--bp > st->tabs && !*bp)
			/* nothing */ ;
		for (bp += tabspaces; bp < st->tabs + col; bp += tabspaces)
			*bp = 1;
	}
	/* update stinal size */
	st->col = col;
	st->row = row;
	/* reset scrolling region */
	tsetscroll(st, 0, row-1);
	/* make use of the LIMIT in tmoveto */
	tmoveto(st, st->c.x, st->c.y);
	/* Clearing both screens (it makes dirty all lines) */
	c = st->c;
	for (i = 0; i < 2; i++) {
		if (mincol < col && 0 < minrow) {
			tclearregion(st, mincol, 0, col - 1, minrow - 1);
		}
		if (0 < col && minrow < row) {
			tclearregion(st, 0, minrow, col - 1, row - 1);
		}
		tswapscreen(st);
		tcursor(st, CURSOR_LOAD);
	}
	st->c = c;
}

void
resettitle(void)
{
//	xsettitle(NULL);
}

void
redraw(ST *st)
{
	tfulldirt(st);
}

void
cresize(ST *st, int width, int height)
{
//        int col, row;
//
//        if (width != 0)
//                win.w = width;
//        if (height != 0)
//                win.h = height;
//
//        col = (win.w - 2 * borderpx) / win.cw;
//        row = (win.h - 2 * borderpx) / win.ch;
//        col = MAX(1, col);
//        row = MAX(1, row);
//
//        tresize(col, row);
//        xresize(col, row);
//        ttyresize(win.tw, win.th);
        tresize(st, width, height);
        ttyresize(st, width, height);
}

