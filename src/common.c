/* common.c -- See common.h for copyright and info */


/* Import
 */

#include "common.h"


/**
 * Does the element occur in the array of strings?
 *
 * @param s char pointer to the string.
 * @param ss const char pointer to the array of strings.
 * @param cnt the length of the array of strings.
 */
int elem(char *s, const char *ss[], int cnt) {
    for(int i = 0; i < cnt; i++)
        if(strcmp(s, ss[i]) == 0) { /* are equal */
            return 1;
        }

    return 0;
}

/**
 * Count the number of lines in a file.
 *
 * @param filename char pointer to the file name.
 */
long lsize(const char *filename) {
    FILE    *fp;
    char    c;
    long    cnt = 0;

    if ((fp = fopen(filename, "r")) == 0) {
        errExit("Cannot open input-file");
    }

    while ((c = getc(fp)) != EOF)
        if (c == '\n')
            cnt += 1;

    fclose(fp);

    return cnt;
}

/**
 * Read the content of a file to a string.
 *
 * @param filename char pointer to the file name.
 */
char *read_file(const char *filename) {
    FILE    *fp;
    struct  stat sb;
    char    *buf;
    size_t  len;

    if (stat(filename, &sb) != 0
            || !S_ISREG(sb.st_mode)) {
        errExit("Input is not a file");
    }

    if (sb.st_size == 0) {
        errExit("Input is empty");
    }

    if ((fp = fopen(filename, "r")) == 0) {
        errExit("Cannot open input-file");
    }

    if ((buf = malloc((size_t) sb.st_size + 1)) == 0) {
        errExit("Cannot allocate memory for input-file");
    }

    len = fread(buf, sizeof(char), (size_t) sb.st_size, fp);
    fclose(fp);

    if (len > (size_t) sb.st_size)
        len = (size_t) sb.st_size;
    buf[len] = '\0';

    return buf;
}
