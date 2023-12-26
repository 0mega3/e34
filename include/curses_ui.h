/**
 * Curses UI.
 * @copyright 2023 by Denis Khorkin
 * @license BSD-3-Clause
 * @author Denis Khorkin <dkh01101111@gmail.com>
 * @version 0.1.0
 * @file
 */


#ifndef CURSES_UI_H
#define CURSES_UI_H


/* Feature Test Macro Requirements for glibc
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE /* struct sigaction, sigemptyset, sigaction */
#endif


/* Import
 */

/* The curses library */
#include <curses.h>

/* The C standard library */
#include <locale.h>         /* setlocale */
#include <signal.h>         /* signal */

/* The C POSIX library */
#include <unistd.h>         /* alarm, sysconf */


/* Macros and constants
 */

#define E34_VERSION "0.1.0"

#define BUFSZ 1024          /* The maximum size of the window stack. */

/* Jump constants to terminate the outer loop */
#define CONTINUE 1
#define BREAK    0

/* CLI errors */
#define E1 "E1: Not an editor command: "


/* UDTs
 */

typedef enum {
    INTRO = 0,
    STARWARS,
    TRON
} startscreen_e;

typedef enum {
    DEFAULT = 0,
    GRUVBOX_DARK_MEDIUM,
    GRUVBOX_LIGHT_MEDIUM
} colorscheme_e;

typedef enum {
    NORMAL_MODE = 0,
    TEST_MODE,
    GAME_MODE,
    BATTLE_MODE
} access_mode_e;

typedef enum {
    DISCONNECTED = 0,
    CONNECTED
} connection_status_e;

typedef enum {
    NONAME = 0,
    HELP
} winname_e;

typedef struct {
    WINDOW          *win;
    startscreen_e   startscreen;
} workarea_t;

typedef struct {
    WINDOW              *win;
    access_mode_e       access_mode;
    connection_status_e connection_status;
    winname_e           tab;
    chtype              separator;
} statusline_t;

typedef struct {
    WINDOW  *win;
} cmdline_t;

typedef struct {
    winname_e   name;
    WINDOW      *win;
    long        depth;      /* The depth of the hidden part of the window. */
} stackwin_t;

typedef struct {
    WINDOW  *win;
} hborder_t;

typedef struct {
    stackwin_t  stwin;      /* The stack window. */
    hborder_t   hborder;    /* A horizontal border of the stack window. */
} winentry_t;

typedef struct {
    int         top;        /* The top is the focus ring. */
    winentry_t  buf[BUFSZ];
} winstack_t;

typedef struct {
    workarea_t      workarea;
    statusline_t    statusline;
    cmdline_t       cmdline;
    colorscheme_e   colorscheme;
    winstack_t      wstack; /* Window stack. */
} state_t;


/* Function prototypes
 */

/**
 * Make a text-based user interface (TUI).
 */
extern int curses_main(void);


#endif /* CURSES_UI_H */
