#include <stdio.h>
#include <string.h>
#include "splitargs.h"

const char * input_cmd[] = {
        "date -u --set \"2019-10-23 14:20:30.1234\"",
        "ls -l /home/",
        "touch \"/home/developer/space cowboy\"",
        "a fairly \"wild west\" function",
        "test\"me\"out",
        "This Sequence Will Exceed The Boundaries Of The Basic Parsing Engine Because It is So Long",
};

int main() {
    char ** results = NULL;

    /*
     * Run through all the strings in the input array and display the arguments.
     */
    for (size_t i = 0 ; i < sizeof(input_cmd)/sizeof(char *) ; i++) {
        if (split_args(input_cmd[i], &results)) {
            display_args(stdout, results);
            free_args(results);
        } else {
            perror("display_args");
        }
    }

    return 0;
}