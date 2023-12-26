/**
 * Common library.
 * @copyright 2023 by Denis Khorkin
 * @license BSD-3-Clause
 * @author Denis Khorkin <dkh01101111@gmail.com>
 * @version 0.1.0
 * @file
 */


#ifndef COMMON_H
#define COMMON_H


/* Import
 */

/* The C standard library */
#include <stdio.h>          /* FILE, fopen, fread, fclose, getc, perror,
                               fprintf */
#include <errno.h>          /* errno */
#include <stdlib.h>         /* stddef.h (size_t), exit,
                               EXIT_FAILURE, EXIT_SUCCESS */
#include <string.h>         /* strlen, strcmp */

/* The C POSIX library */
#include <sys/stat.h>       /* struct stat, stat, S_ISREG */


/* Macros and constants
 */

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                        } while (0)


/* Function prototypes
 */

/**
 * Does the element occur in the array of strings?
 *
 * @param s char pointer to the string.
 * @param ss const char pointer to the array of strings.
 * @param cnt the length of the array of strings.
 */
extern int elem(char *s, const char *ss[], int cnt);

/**
 * Count the number of lines in a file.
 *
 * @param filename char pointer to the file name.
 */
extern long lsize(const char *filename);

/**
 * Read the content of a file to a string.
 *
 * @param filename char pointer to the file name.
 */
extern char *read_file(const char *filename);


#endif /* COMMON_H */
