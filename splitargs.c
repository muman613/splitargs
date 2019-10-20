#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "splitargs.h"

//#define INCLUDE_QUOTE       1

enum parse_state {
    PARSE_NORMAL,
    PARSE_INQUOTE,
};

/**
 * Split input by spaces into arguments.
 *
 * @param input - NULL terminated string to split, quoted text preserves spaces.
 * @param fargs - Pointer to array of string pointers to NULL terminated arguments.
 * @return true upon success, false upon failure and sets errno to reason.
 */
bool split_args(const char * input, char ***fargs) {
    char buffer[1024];
    char * p = (char *)input;
    enum parse_state state = PARSE_NORMAL;
    int buffer_index = 0, arg_index = 0;
    char ** args = NULL;

    args = (char **)malloc(MAX_ARGS * sizeof(char *));
    if (args == NULL) {
        perror("Unable to allocate (malloc) arg array");
        return false;
    }

    memset(args, 0, MAX_ARGS * sizeof(char *));
    memset(buffer, 0, sizeof(buffer));

    while (*p != '\0') {
        // Ensure we don't exceed maximum argument count
        if (arg_index == MAX_ARGS) {
            free_args(args);
            errno = E2BIG;
            return false;
        }

        if (state == PARSE_NORMAL) {
            if (*p == '"') {
#ifdef INCLUDE_QUOTE
                buffer[buffer_index++] = *p;
#endif
                state = PARSE_INQUOTE;
            } else {
                if (*p == ' ') {
                    buffer[buffer_index] = '\0';
#ifdef DEBUG
                    fprintf(stderr, "argument : %s\n", buffer);
#endif
                    args[arg_index++] = strdup(buffer);

                    memset(buffer, 0, sizeof(buffer));
                    buffer_index = 0;
                } else {
                    buffer[buffer_index++] = *p;
                }
            }
        } else if (state == PARSE_INQUOTE) {
            if (*p == '"') {
#ifdef INCLUDE_QUOTE
                buffer[buffer_index++] = *p;
#endif
                state = PARSE_NORMAL;
            } else {
                buffer[buffer_index++] = *p;
            }
        }
        p++;
    }

    // Append the last argument to the array
    if (buffer_index > 0) {
        args[arg_index] = strdup(buffer);
    }

    *fargs = args;

    return true;
}

/**
 * Display all arguments sending output to specified file handle.
 *
 * @param ofp - output file handle, stdout or stderr.
 * @param args - argument list to display
 */
void display_args(FILE *ofp, char **args) {
    int index = 0;

    // Do not segfault if passed a NULL pointer
    if (args == NULL) {
        return;
    }

    fprintf(ofp, "====================================================\n");
    while ((index < MAX_ARGS) && args[index] != NULL) {
        fprintf(ofp,"Argument %-2d : %s\n", index, args[index]);
        index++;
    }
    fprintf(ofp, "====================================================\n");
}

void free_args(char ** args) {
    for (int index = 0 ; index < MAX_ARGS ; index++) {
        if (args[index] != NULL) {
            free(args[index]);
        }
    }

    free(args);
}