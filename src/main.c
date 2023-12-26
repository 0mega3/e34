/**
 * Vim-like UI engine.
 * @copyright 2023 by Denis Khorkin
 * @license BSD-3-Clause
 * @author Denis Khorkin <dkh01101111@gmail.com>
 * @version 0.1.0
 * @file
 */


/* Import
 */

/* A common library */
#include "common.h"

/* Text-based user interface (TUI) */
#include "curses_ui.h"


int
main(int argc, char* argv[])
{
    switch (argc) {
        case 2:
            if (strcmp(argv[1], "tui") == 0) {
                curses_main();
            } else {
                fprintf(stderr, "Usage: %s tui\n", argv[0]);
                exit(EXIT_FAILURE);
            }
            break;
        default:
            fprintf(stderr, "Usage: %s tui\n", argv[0]);
            exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
