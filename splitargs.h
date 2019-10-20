#ifndef PARSEARGS_SPLITARGS_H
#define PARSEARGS_SPLITARGS_H

#include <stdbool.h>

bool split_args(const char * input, char ***fargs);
void display_args(FILE *ofp, char **args);
void free_args(char **args);

#define MAX_ARGS        10

#endif //PARSEARGS_SPLITARGS_H
