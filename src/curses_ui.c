/* curses_ui.c -- See curses_ui.h for copyright and info */


/* Import
 */

#include "curses_ui.h"
#include "common.h"


/* Private macros and constants
 */

#define i(color) (color * 1000 / 256)   /* The intensity of the rgb
                                           components in a color. */

/* Commands that the command line interface (aka cli) supports.
   The cli command consists of: :cmd [opt [arg]], which are separated
   by spaces. It can also be used camelCase or СamelСase when
   the commands are written without spaces and with capitalized words.
   Basic cli commands may have shorter equivalents for convenience.
   For example, the command :help has a shorter equivalent :h. */
const char *basic_cmds[] = {        /* Basic commands */
    "quit",     "q",                /* to exit */
    "help",     "h",                /* help */
    "clear",    "cl",               /* clear workarea */
    "intro",                        /* e34's startscreen */
};

const char *conn_cmds[] = {         /* Connection commands */
    "connect game",     "c game",   /* connect to GAME mode */
    "connect test",     "c test",   /* connect to TEST mode */
    "connect battle",   "c battle", /* connect to BATTLE mode */
    "disconnect",       "disc",     /* return  to NORMAL mode */
};

const char *colo_cmds[] = {         /* Colorscheme commands */
    "colorscheme default",              "colo default",
    "colorscheme gruvbox_dark_medium",  "colo gruvbox_dark_medium",
    "colorscheme gruvbox_light_medium", "colo gruvbox_light_medium",
};

const char *sta_cmds[] = {          /* Startscreen commands */
    "startscreen starwars", "sta starwars",
    "startscreen tron",     "sta tron",
};

const char *intro[] = {             /* e34's startscreen */
    "e34 - Vim-like UI engine",
    "",
    "version " E34_VERSION,
    "by Denis Khorkin et al.",
    "e34 is open source and freely distributable",
    "",
    "type   :q<Enter>     to exit ",
    "type   :help<Enter>  for help",
};

const char *starwars[] = {
    "           ########################   ######     ###########             ",
    "         ##########################  ########    #############           ",
    "        ########################### ####  ####   #####   ######          ",
    "         ########      ######       ####  ####   #############           ",
    "           ########    ######      ############  ###########             ",
    "###################    ######     ############## ##### ##################",
    "#################      ######     #####    ##### #####  #################",
    "",
    "#########    ##    #####   ######     ###########        ################",
    "##########  ####  ######  ########    #############    ##################",
    "#######################  ####  ####   #####   ######  ###################",
    "     ##################  ####  ####   #############    ########          ",
    "      ################  ############  ###########        ########        ",
    "       ######  ######  ############## ##### #####################        ",
    "        ####    ####   #####    ##### #####  ##################          "
};

const char *tron[] = {
    "#####    ####################         ######      ##          #####",
    "#####  ########################    ############   ####        #####",
    "##### ########################## #####      ##### ######      #####",
    "      ######                     ####        #### ######## ########",
    "      ######      ###########    #####      ##### #####      ######",
    "      ######      ##### ######     ############   #####        ####",
    "      ######      #####   ######      ######      #####          ##"
};

state_t *state;


/*****************************************************************************
 * Internal functions                                                        *
 *****************************************************************************/

/**
 * Initialize the default colorscheme.
 */
void init_default_colorscheme(void) {
    /* The standard colors (ISO-6429):
     * COLOR_BLACK      = 0,
     * COLOR_RED        = 1,
     * COLOR_GREEN      = 2,
     * COLOR_YELLOW     = 3,
     * COLOR_BLUE       = 4,
     * COLOR_MAGENTA    = 5,
     * COLOR_CYAN       = 6,
     * COLOR_WHITE      = 7. */

    /* Assign terminal default foreground/background colors
       to color number -1. */
    assume_default_colors(-1, -1);  /* Color pair 0 */
    init_pair( 1, COLOR_RED,     -1);
    init_pair( 2, COLOR_GREEN,   -1);
    init_pair( 3, COLOR_YELLOW,  -1);
    init_pair( 4, COLOR_BLUE,    -1);
    init_pair( 5, COLOR_MAGENTA, -1);
    init_pair( 6, COLOR_CYAN,    -1);

    init_pair( 7, COLOR_BLACK,   COLOR_WHITE);
    init_pair( 8, COLOR_RED,     COLOR_WHITE);
    init_pair( 9, COLOR_GREEN,   COLOR_WHITE);
    init_pair(10, COLOR_YELLOW,  COLOR_WHITE);
    init_pair(11, COLOR_BLUE,    COLOR_WHITE);
    init_pair(12, COLOR_MAGENTA, COLOR_WHITE);
    init_pair(13, COLOR_CYAN,    COLOR_WHITE);
}

/**
 * Initialize the gruvbox color scheme (dark version, medium contrast).
 *
 * Gruvbox is a retro groove color scheme created by morhetz
 * <morhetz@gmail.com>.
 */
void init_gruvbox_dark_medium_colorscheme(void) {
    int dark0, dark2, gray, light1,
        bright_red, bright_green, bright_yellow,
        bright_blue, bright_purple, bright_aqua,
        bright_orange;

    /* To avoid affecting .Xresources, the ANSI 0-15 colors
     * are not assigned. */
    init_color(dark0         = 16,  i( 40), i( 40), i( 40));
    init_color(dark2         = 17,  i( 80), i( 73), i( 69));
    init_color(gray          = 18,  i(146), i(131), i(116));
    init_color(bright_red    = 19,  i(251), i( 73), i( 52));
    init_color(bright_green  = 20,  i(184), i(187), i( 38));
    init_color(bright_yellow = 21,  i(250), i(189), i( 47));
    init_color(bright_blue   = 22,  i(131), i(165), i(152));
    init_color(bright_purple = 23,  i(211), i(134), i(155));
    init_color(bright_aqua   = 24,  i(142), i(192), i(124));
    init_color(bright_orange = 25,  i(254), i(128), i( 25));
    init_color(light1        = 26,  i(235), i(219), i(178));

    assume_default_colors(light1, dark0);
    init_pair( 1, bright_red,    dark0);
    init_pair( 2, bright_green,  dark0);
    init_pair( 3, bright_yellow, dark0);
    init_pair( 4, bright_blue,   dark0);
    init_pair( 5, bright_purple, dark0);
    init_pair( 6, bright_aqua,   dark0);

    init_pair( 7, light1,        dark2);
    init_pair( 8, bright_red,    dark2);
    init_pair( 9, bright_green,  dark2);
    init_pair(10, bright_yellow, dark2);
    init_pair(11, bright_blue,   dark2);
    init_pair(12, bright_purple, dark2);
    init_pair(13, bright_aqua,   dark2);

    init_pair(14, gray,          dark0);
    init_pair(15, bright_orange, dark0);
}

/**
 * Initialize the gruvbox color scheme (light version, medium contrast).
 *
 * Gruvbox is a retro groove color scheme created by morhetz
 * <morhetz@gmail.com>.
 */
void init_gruvbox_light_medium_colorscheme(void) {
    int light0, light2, gray, dark1,
        faded_red, faded_green, faded_yellow,
        faded_blue, faded_purple, faded_aqua,
        faded_orange;

    init_color(light0       = 16,   i(251), i(241), i(199));
    init_color(light2       = 17,   i(213), i(196), i(161));
    init_color(gray         = 18,   i(146), i(131), i(116));
    init_color(faded_red    = 19,   i(157), i(  0), i(  6));
    init_color(faded_green  = 21,   i(121), i(116), i( 14));
    init_color(faded_yellow = 22,   i(181), i(118), i( 20));
    init_color(faded_blue   = 23,   i(  7), i(102), i(120));
    init_color(faded_purple = 24,   i(143), i( 63), i(113));
    init_color(faded_aqua   = 25,   i( 66), i(123), i( 88));
    init_color(faded_orange = 26,   i(175), i( 58), i(  3));
    init_color(dark1        = 26,   i( 60), i( 56), i( 54));

    assume_default_colors(dark1, light0);
    init_pair( 1, faded_red,    light0);
    init_pair( 2, faded_green,  light0);
    init_pair( 3, faded_yellow, light0);
    init_pair( 4, faded_blue,   light0);
    init_pair( 5, faded_purple, light0);
    init_pair( 6, faded_aqua,   light0);

    init_pair( 7, dark1,        light2);
    init_pair( 8, faded_red,    light2);
    init_pair( 9, faded_green,  light2);
    init_pair(10, faded_yellow, light2);
    init_pair(11, faded_blue,   light2);
    init_pair(12, faded_purple, light2);
    init_pair(13, faded_aqua,   light2);

    init_pair(14, gray,         light0);
    init_pair(15, faded_orange, light0);
}

/**
 * Initialize the curses system.
 */
void init_curses(void) {
    /* Sets the locale environment according to
       the environment variables. */
    setlocale(LC_ALL, "");

    initscr();              /* Start curses mode */
    if(!has_colors()) {         /* Check terminal color capability */
        printf("Your terminal doesn't support colors\n");
    } else {
        start_color();          /* Start the color functionality */
        if(can_change_color()) {
            switch(state->colorscheme) {
                case GRUVBOX_DARK_MEDIUM:
                    init_gruvbox_dark_medium_colorscheme();
                    break;
                case GRUVBOX_LIGHT_MEDIUM:
                    init_gruvbox_light_medium_colorscheme();
                    break;
                default:
                    /* do nothing */
                    break;
            }
        } else {
            init_default_colorscheme();
        }
    }
    cbreak();               /* Line buffering disabled. pass on everything */
    noecho();               /* Don't echo() while we do getch */
    curs_set(FALSE);        /* Make the cursor invisible */
    keypad(stdscr, TRUE);   /* We get F1, F2 etc..      */
}

/**
 * Create a new window.
 *
 * @param nlines the number of lines.
 * @param ncols the number of columns.
 * @param begin_y the y coordinate of the upper left-hand corner of the window.
 * @param begin_x the x coordinate of the upper left-hand corner of the window.
 */
WINDOW *create_newwin(int nlines, int ncols, int begin_y, int begin_x) {
    WINDOW *win;

    win = newwin(nlines, ncols, begin_y, begin_x);
    box(win, 0, 0);
    wrefresh(win);

    return win;
}

/**
 * Destroy a window.
 *
 * @param win WINDOW pointer to the target window.
 */
void destroy_win(WINDOW *win) {
    wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    /* The parameters taken are
     * 1. win: the window on which to operate
     * 2. ls: character to be used for the left side of the window
     * 3. rs: character to be used for the right side of the window
     * 4. ts: character to be used for the top side of the window
     * 5. bs: character to be used for the bottom side of the window
     * 6. tl: character to be used for the top left corner of the window
     * 7. tr: character to be used for the top right corner of the window
     * 8. bl: character to be used for the bottom left corner of the window
     * 9. br: character to be used for the bottom right corner of the window
     */
    wrefresh(win);
    delwin(win);
}

/**
 * Create a workarea.
 *
 * The concept of the workarea, statusline, and commandline is used.
 */
WINDOW *create_workarea(void) {
    WINDOW *workarea;
    int nlines = LINES - 2, ncols  = COLS,
        starty = 0, startx = 0;

    workarea = newwin(nlines, ncols, starty, startx);
    wrefresh(workarea);

    return workarea;
}

/**
 * Create a statusline.
 *
 * The concept of the workarea, statusline, and commandline is used.
 */
WINDOW *create_statusline(void) {
    WINDOW *statusline;
    int nlines = 1, ncols = COLS,
        starty = LINES - 2, startx = 0;

    statusline = newwin(nlines, ncols, starty, startx);
    wrefresh(statusline);

    return statusline;
}

/**
 * Create a commandline.
 *
 * The concept of the workarea, statusline, and commandline is used.
 */
WINDOW *create_cmdline(void) {
    WINDOW *cmdline;
    int nlines = 1, ncols = COLS,
        starty = LINES - 1, startx = 0;

    cmdline = newwin(nlines, ncols, starty, startx);
    wrefresh(cmdline);

    return cmdline;
}

/**
 * Initialize the ui state.
 */
state_t *init_state(void) {
    state = calloc(1, sizeof(state_t));

    state->workarea.startscreen = INTRO;

    /* It is impossible to connect to the NORMAL mode */
    state->statusline.access_mode       = NORMAL_MODE;
    state->statusline.connection_status = DISCONNECTED;
    state->statusline.tab               = NONAME;
    state->statusline.separator         = ACS_VLINE;

    state->colorscheme = GRUVBOX_DARK_MEDIUM;

    state->wstack.top = -1; /* The window stack is empty. */

    return state;
}

/**
 * Center an array of strings both vertically and horizontally.
 *
 * Consumes all available vertical and horizontal space
 * of the corresponding window.
 *
 * @param win WINDOW pointer to the target window.
 * @param filler const char pointer to the array of strings.
 * @param cnt the length of the array of strings.
 */
void center( WINDOW *win, const char *filler[], int cnt) {
    int nlines, ncols;

    getmaxyx(win, nlines, ncols);
    for(int i = 0; i < cnt; i++)
        mvwaddstr(win, (nlines - cnt) / 2 + i,
                (ncols - strlen(filler[i])) / 2, filler[i]);
}

/**
 * Establishing a signal handler.
 *
 * @param void(*) (int) pointer to a signal-catching function.
 * See 'man sigaction'.
 */
void establish_sighandler(void(*sighandler) (int)) {
    struct sigaction sa;

    sa.sa_handler = sighandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;   /* Restart functions if
                                   interrupted by handler. */
    if (sigaction(SIGWINCH, &sa, NULL) == -1) {
        errExit("sigaction");
    }

}

/**
 * The signal handler function for resizing windows.
 *
 * @param signum identifies the signal whose behavior needs to be controlled,
 * and should be a signal number.
 */
static void winch_handler(int signum) {
    endwin();
    /* Needs to be called after an endwin()
       so ncurses will initialize itself
       with the new terminal dimensions. */
    doupdate();

    wresize(state->workarea.win,   LINES - 2, COLS);
    wresize(state->statusline.win,         1, COLS);
    wresize(state->cmdline.win,            1, COLS);
    if(state->wstack.top != -1) {
        for(int i = 0; i <= state->wstack.top; i++) {
            wresize(state->wstack.buf[i].stwin.win,
                    getmaxy(state->wstack.buf[i].stwin.win), COLS);
            wresize(state->wstack.buf[i].hborder.win, 1, COLS);
        }
    }

    mvwin(state->statusline.win, LINES - 2, 0);
    mvwin(state->cmdline.win,    LINES - 1, 0);

    wnoutrefresh(state->workarea.win);
    wnoutrefresh(state->statusline.win);
    wnoutrefresh(state->cmdline.win);
    /* The order of the update depends on the order of the windows
       in the window stack. */
    if(state->wstack.top != -1) {
        for(int i = 0; i <= state->wstack.top; i++) {
            /* Refresh the stack window. */
            is_pad(state->wstack.buf[i].stwin.win)
                ? pnoutrefresh(state->wstack.buf[i].stwin.win,
                        0, 0, 0, 0, LINES / 2, COLS)
                : wnoutrefresh(state->wstack.buf[i].stwin.win);
            /* Refresh the horizontal border of the stack window. */
            whline(state->wstack.buf[i].hborder.win, '-', COLS);
            wnoutrefresh(state->wstack.buf[i].hborder.win);
        }
    }
    doupdate();
}

/**
 * The signal handler function for clearing the workarea by timeout.
 *
 * @param signum identifies the signal whose behavior needs to be controlled,
 * and should be a signal number.
 */
void alrm_handler(int signum) {
    werase(state->workarea.win);
    wnoutrefresh(state->workarea.win);
    if(state->wstack.top != -1) {
        for(int i = 0; i <= state->wstack.top; i++) {
            is_pad(state->wstack.buf[i].stwin.win)
                ? pnoutrefresh(state->wstack.buf[i].stwin.win,
                        0, 0, 0, 0, LINES / 2, COLS)
                : wnoutrefresh(state->wstack.buf[i].stwin.win);
            touchwin(state->wstack.buf[i].hborder.win);
            wnoutrefresh(state->wstack.buf[i].hborder.win);
        }
    }
    /* If the workarea is cleared
       when entering command mode,
       return focus to the cmdline. */
    wnoutrefresh(state->cmdline.win);
    doupdate();
}

/**
 * Draw a workarea.
 */
void draw_workarea(void) {

    switch(state->workarea.startscreen) {
        case INTRO:
            center(state->workarea.win, intro,
                    sizeof(intro) / sizeof(intro[0]));
            wrefresh(state->workarea.win);
            /* Establish an action for a particular signal. */
            signal(SIGALRM, alrm_handler);
            /* Interrupt itself (a process) in the future by setting a timer.
               When the timer expires, the process receives a signal. */
            alarm(1);
            break;
        default:
            /* do nothing */
            break;
    }

}

/**
 * Set the statusline item.
 *
 * @param item char pointer to the statusline item.
 * @param _attrs identifies only the window attributes (not color).
 * @param attrs identifies the window attributes and color.
 */
void set_statusline_item(char *item, int _attrs, int attrs) {
    /* Manipulating the window attributes and color */
    wattrset(state->statusline.win, attrs);

    mvwaddch(state->statusline.win,  0, getcurx(state->statusline.win), ' ');

    /* Manipulating only the window attributes (not color) */
    wattron(state->statusline.win, _attrs);

    mvwaddstr(state->statusline.win, 0, getcurx(state->statusline.win), item);
    mvwaddch(state->statusline.win,  0, getcurx(state->statusline.win), ' ');

    /* Turn off all attributes */
    wattrset(state->statusline.win, 0);
}

/**
 * Draw a statusline.
 */
void draw_statusline(void) {
    wbkgd(state->statusline.win, COLOR_PAIR(7));
    wmove(state->statusline.win, 0, 0);

    switch(state->statusline.access_mode) {
        case TEST_MODE:
            set_statusline_item("TEST",   A_BOLD, A_REVERSE | COLOR_PAIR(3));
            break;
        case GAME_MODE:
            set_statusline_item("GAME",   A_BOLD, A_REVERSE | COLOR_PAIR(4));
            break;
        case BATTLE_MODE:
            set_statusline_item("BATTLE", A_BOLD, A_REVERSE | COLOR_PAIR(1));
            break;
        default:
            set_statusline_item("NORMAL", A_BOLD, A_REVERSE | COLOR_PAIR(2));
            break;
    }

    switch(state->statusline.connection_status) {
        case CONNECTED:
            set_statusline_item("<~>", A_BOLD, COLOR_PAIR(9));
            break;
        default:
            set_statusline_item("</>", A_BOLD, COLOR_PAIR(8));
            break;
    }

    switch(state->statusline.tab) {
        case HELP:
            set_statusline_item("[Help]",   A_NORMAL, COLOR_PAIR(11));
            break;
        default:
            set_statusline_item("[NoName]", A_NORMAL, COLOR_PAIR(11));
            break;
    }

    wclrtoeol(state->statusline.win);
    wrefresh(state->statusline.win);
}

/**
 * Get the command on the commandline.
 */
char *get_command(void) {
    const size_t    line_max = sysconf(_SC_LINE_MAX);   /* 2048 B */
    char            *cmd = calloc(1, line_max);
    int ch;

    echo();
    curs_set(TRUE);
    keypad(state->cmdline.win, TRUE);
    set_escdelay(100);              /* The default value, 1000 ms. */

    int x = 1;
    while((ch = wgetch(state->cmdline.win))
            != '\n') {              /* C-escape for Enter */
        if(ch == ERR || ch == 0) {
            errExit("Keypress is not available");
        }

        if(ch == 27) {              /* ASCII Control Code for Esc */
            /* Clear cmdline */
            werase(state->cmdline.win);
            wrefresh(state->cmdline.win);
            cmd[0] = '\0';          /* Set the command to zero-length */
            break;
        } else if(ch == KEY_LEFT) {
            if(x >= 2) {
                wmove(state->cmdline.win, 0, --x);
            }
        } else if(ch == KEY_RIGHT) {
            if(x <= strlen(cmd)) {
                wmove(state->cmdline.win, 0, ++x);
            }
        } else if(ch == KEY_BACKSPACE) {
            mvwdelch(state->cmdline.win, 0, --x);
            if(x == 0)
                break;
            if(x - 1 >= 0)          /* Heap BOF protection */
                cmd[x - 1] = '\0';
        } else if(ch == 21) {       /* ASCII Control Code for Ctrl-U */
            wmove(state->cmdline.win, 0, 1);
            wclrtoeol(state->cmdline.win);
            for(int i = 0; i < line_max; i++)
                cmd[i] = '\0';      /* Zero the entire contents
                                       of the command */
            x = 1;
        } else {
            if(x - 1 < line_max) {  /* Heap BOF protection */
                cmd[x - 1] = ch;
                x++;
            }
        }
    }

    noecho();
    curs_set(FALSE);

    return cmd;
}

/**
 * Search for a window in the window stack.
 *
 * @param name the name of the stack window.
 */
int matchwin(winname_e name) {
    for(int i = state->wstack.top; i >= 0; i--) {
        if(state->wstack.buf[i].stwin.name == name) {
            return 1;
        }
    }

    return 0;
}

/**
 * Quit.
 *
 * Quit the current window. Quit e34 if this is the last open window.
 */
int quit(void) {
    if(state->wstack.top == -1) {
        return BREAK; /* Quit e34. */
    }

    /* Quit the current window. */
    delwin(state->wstack.buf[state->wstack.top].stwin.win);
    delwin(state->wstack.buf[state->wstack.top].hborder.win);
    state->wstack.buf[state->wstack.top].stwin.name  = NONAME;
    state->wstack.buf[state->wstack.top].stwin.win   = NULL;
    state->wstack.buf[state->wstack.top].stwin.depth = 0;
    state->wstack.buf[state->wstack.top].hborder.win = NULL;
    state->wstack.top--;

    /* Reset the status line tab. */
    state->statusline.tab = NONAME;
    draw_statusline();

    /* Refresh the entire overlapping window. */
    touchwin(state->workarea.win);
    wrefresh(state->workarea.win);

    return CONTINUE;
}

/**
 * Open a window and display the help file in read-only mode.
 */
void show_help(void) {
    WINDOW  *pad, *hborder;
    long    nlines;
    char    *contents;

    if(state->wstack.top == -1 ||
            (state->wstack.top >= 0 && state->wstack.top < BUFSZ &&
             !matchwin(HELP))) {
        nlines = lsize("doc/help.txt");

        /* Create a new pad data structure. A pad is like a window,
           except that it is not restricted by the screen size. */
        pad = newpad(nlines, COLS);
        keypad(pad, TRUE);

        /* Create a horizontal border. */
        hborder = newwin(1, COLS, LINES / 2, 0);
        whline(hborder, '-', COLS);

        /* Push the window to the top of the window stack. */
        state->wstack.top++;
        state->wstack.buf[state->wstack.top].stwin.name  = HELP;
        state->wstack.buf[state->wstack.top].stwin.win   = pad;
        /* If a window is a pad, then the depth is (nlines - 1) - smaxrow,
           otherwise the depth is 0. The depth is one less, because
           the pminrow parameter is 0 (see curs_pad(3x)). */
        state->wstack.buf[state->wstack.top].stwin.depth =
            (nlines - 1) - LINES / 2;
        state->wstack.buf[state->wstack.top].hborder.win = hborder;

        /* Set the status line tab. */
        state->statusline.tab = HELP;
        draw_statusline();

        /* Write the contents of the file on the given window. */
        contents = read_file("doc/help.txt");
        waddstr(pad, contents);
        free(contents);

        /* Refresh the output. */
        wnoutrefresh(hborder);
        pnoutrefresh(pad, 0, 0, 0, 0, LINES / 2, COLS);
        doupdate();
    }

}

/**
 * Show the default start or intro screen.
 */
void show_intro(void) {
    int     maxy = getmaxy(state->workarea.win);
    int     cnt  = sizeof(intro) / sizeof(intro[0]);
    char    *msg = "Press any key to continue";

    scr_dump("e34scr.out");
    erase();

    center(stdscr, intro, cnt);
    attron(A_BOLD | COLOR_PAIR(4));
    mvaddstr((maxy - cnt) / 2 + cnt + 2, 0, msg);
    attroff(A_BOLD | COLOR_PAIR(4));
    refresh();

    getch();

    scr_restore("e34scr.out");
    doupdate();
}

/**
 * Execute the basic command.
 *
 * @param cmd char pointer to the valid command.
 */
int do_basic(char *cmd) {
    int tr = CONTINUE;

    if(strcmp(cmd, "quit") == 0 ||
            strcmp(cmd, "q") == 0) {
        tr = quit();
    } else if(strcmp(cmd, "help") == 0 ||
            strcmp(cmd, "h") == 0) {
        show_help();
    } else if(strcmp(cmd, "clear") == 0 ||
            strcmp(cmd, "cl") == 0) {
        werase(state->workarea.win);
        wnoutrefresh(state->workarea.win);
        if(state->wstack.top != -1) {
            for(int i = 0; i <= state->wstack.top; i++) {
                is_pad(state->wstack.buf[i].stwin.win)
                    ? pnoutrefresh(state->wstack.buf[i].stwin.win,
                            0, 0, 0, 0, LINES / 2, COLS)
                    : wnoutrefresh(state->wstack.buf[i].stwin.win);
                touchwin(state->wstack.buf[i].hborder.win);
                wnoutrefresh(state->wstack.buf[i].hborder.win);
            }
        }
        doupdate();
    } else if(strcmp(cmd, "intro") == 0) {
        show_intro();
    }

    return tr;
}

/**
 * Execute the connection command.
 *
 * @param cmd char pointer to the valid command.
 */
void do_connect(char *cmd) {
    if(strcmp(cmd, "connect test") == 0 ||
            strcmp(cmd, "c test") == 0) {
        //TODO: Establish a connection.
        state->statusline.access_mode       = TEST_MODE;
        state->statusline.connection_status = CONNECTED;
        draw_statusline();
    } else if(strcmp(cmd, "connect game") == 0 ||
            strcmp(cmd, "c game") == 0) {
        //TODO: Establish a connection.
        state->statusline.access_mode       = GAME_MODE;
        state->statusline.connection_status = CONNECTED;
        draw_statusline();
    } else if(strcmp(cmd, "connect battle") == 0 ||
            strcmp(cmd, "c battle") == 0) {
        //TODO: Establish a connection.
        state->statusline.access_mode       = BATTLE_MODE;
        state->statusline.connection_status = CONNECTED;
        draw_statusline();
    } else if(strcmp(cmd, "disconnect") == 0 ||
            strcmp(cmd, "disc") == 0) {
        //TODO: Disconnect the connection.
        state->statusline.access_mode       = NORMAL_MODE;
        state->statusline.connection_status = DISCONNECTED;
        draw_statusline();
    }

}

/**
 * Execute the color scheme command.
 *
 * @param cmd char pointer to the valid command.
 */
void do_colorscheme(char *cmd) {
    if(strcmp(cmd, "colorscheme default") == 0 ||
            strcmp(cmd, "colo default") == 0) {
        state->colorscheme = DEFAULT;
        init_default_colorscheme();
        wnoutrefresh(state->workarea.win);
        wnoutrefresh(state->statusline.win);
        wnoutrefresh(state->cmdline.win);
        doupdate();
    } else if(strcmp(cmd, "colorscheme gruvbox_dark_medium") == 0 ||
            strcmp(cmd, "colo gruvbox_dark_medium") == 0) {
        if(can_change_color()) {
            state->colorscheme = GRUVBOX_DARK_MEDIUM;
            init_gruvbox_dark_medium_colorscheme();
            wnoutrefresh(state->workarea.win);
            wnoutrefresh(state->statusline.win);
            wnoutrefresh(state->cmdline.win);
            doupdate();
        }
    } else if(strcmp(cmd, "colorscheme gruvbox_light_medium") == 0 ||
            strcmp(cmd, "colo gruvbox_light_medium") == 0) {
        if(can_change_color()) {
            state->colorscheme = GRUVBOX_LIGHT_MEDIUM;
            init_gruvbox_light_medium_colorscheme();
            wnoutrefresh(state->workarea.win);
            wnoutrefresh(state->statusline.win);
            wnoutrefresh(state->cmdline.win);
            doupdate();
        }
    }

}

/**
 * Execute the start screen command.
 *
 * @param cmd char pointer to the valid command.
 */
void do_startscr(char *cmd) {
    if(strcmp(cmd, "startscreen starwars") == 0 ||
            strcmp(cmd, "sta starwars") == 0) {
        werase(state->workarea.win);
        wattron(state->workarea.win, COLOR_PAIR(3));
        center(state->workarea.win, starwars,
                sizeof(starwars) / sizeof(starwars[0]));
        wattroff(state->workarea.win, COLOR_PAIR(3));
        wrefresh(state->workarea.win);
    } else if(strcmp(cmd, "startscreen tron") == 0 ||
            strcmp(cmd, "sta tron") == 0) {
        werase(state->workarea.win);
        wattron(state->workarea.win, COLOR_PAIR(6));
        center(state->workarea.win, tron,
                sizeof(tron) / sizeof(tron[0]));
        wattroff(state->workarea.win, COLOR_PAIR(6));
        wrefresh(state->workarea.win);
    }

}

/**
 * Produce an error on the commandline.
 *
 * If a command supported by the commandline interface (aka cli)
 * is not in the list of commands, an error occurs.
 *
 * @param cmd char pointer to the invalid command.
 */
void do_error(char *cmd) {
    const size_t    line_max = sysconf(_SC_LINE_MAX);
    char            err[line_max];

    /* Stack BOF protection.
       Utility functions from BSD systems.
       The  library  is  used in an overlay mode.
       See libbsd(7). */
    strlcpy(err, E1, sizeof(err));
    strlcat(err, cmd, sizeof(err));

    wattron(state->cmdline.win, A_BOLD | A_REVERSE | COLOR_PAIR(1));
    waddstr(state->cmdline.win, err);
    wattroff(state->cmdline.win, A_BOLD | A_REVERSE | COLOR_PAIR(1));
    wrefresh(state->cmdline.win);
}

/**
 * Execute any of the commands.
 *
 * @param cmd char pointer to the command.
 */
int do_command(char *cmd) {
    int tr = CONTINUE;

    if(strlen(cmd) > 0) {
        /* Clear cmdline */
        werase(state->cmdline.win);
        wrefresh(state->cmdline.win);

        if(elem(cmd, basic_cmds,
                    sizeof(basic_cmds) / sizeof(basic_cmds[0]))) {
            tr = do_basic(cmd);
        } else if(elem(cmd, conn_cmds,
                    sizeof(conn_cmds) / sizeof(conn_cmds[0]))) {
            do_connect(cmd);
        } else if(elem(cmd, colo_cmds,
                    sizeof(colo_cmds) / sizeof(colo_cmds[0]))) {
            do_colorscheme(cmd);
        } else if(elem(cmd, sta_cmds,
                    sizeof(sta_cmds) / sizeof(sta_cmds[0]))) {
            do_startscr(cmd);
        } else {
            do_error(cmd);
        }
    }

    return tr;
}

/**
 * The main event loop.
 */
void event_loop(void) {
    int     tr  = 1;
    int     pos = 0;
    char    *cmd;

    while(tr) {
        int ch = wgetch(state->cmdline.win);
        switch(ch) {
            case ERR:
            case 0:
                errExit("Keypress is not available");
                break;

            case KEY_UP:    /* The window is scrolled down one line */
                if(state->wstack.top != -1) {
                    if(pos > 0) {
                        prefresh(state->wstack.buf[state->wstack.top].stwin.win,
                                --pos, 0, 0, 0, LINES / 2, COLS);
                        ++state->wstack.buf[state->wstack.top].stwin.depth;
                    }
                }
                break;
            case KEY_DOWN:  /* The window is scrolled up one line */
                if(state->wstack.top != -1) {
                    if(state->wstack.buf[state->wstack.top].stwin.depth > 0) {
                        prefresh(state->wstack.buf[state->wstack.top].stwin.win,
                                ++pos, 0, 0, 0, LINES / 2, COLS);
                        --state->wstack.buf[state->wstack.top].stwin.depth;
                    }
                }
                break;

            case ':':       /* Enter command mode */
                /* Clear the error message, if any. */
                werase(state->cmdline.win);
                wrefresh(state->cmdline.win);

                /* If the window is not a pad, and it has been moved
                   or modified since the last call to wrefresh,
                   wrefresh will be called before another character
                   is read. */
                waddch(state->cmdline.win, ch);

                cmd = get_command();
                tr  = do_command(cmd);

                free(cmd);
                break;

            default:
                /* do nothing */
                break;
        }
    }

}


/*****************************************************************************
 * External functions                                                        *
 *****************************************************************************/

/**
 * Make a text-based user interface (TUI).
 */
int curses_main(void)
{
    establish_sighandler(winch_handler);

    state = init_state();

    init_curses();

    state->workarea.win   = create_workarea();;
    state->statusline.win = create_statusline();
    state->cmdline.win    = create_cmdline();

    draw_workarea();
    draw_statusline();

    event_loop();

    destroy_win(state->workarea.win);
    destroy_win(state->statusline.win);
    destroy_win(state->cmdline.win);

    free(state);

    if(has_colors())
        reset_color_pairs();
    endwin();   /* End curses mode */

    return EXIT_SUCCESS;
}
