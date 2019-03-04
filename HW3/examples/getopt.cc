// https://linux.die.net/man/3/getopt
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    int flags, opt;
    int nsecs, tfnd;

    nsecs = 0;
    tfnd = 0;
    flags = 0;
    opterr = 0;
    while ((opt = getopt(argc, argv, "nt::")) != -1) {
        switch (opt) {
        case 'n':
            flags = 1;
            break;
        case 't':
            nsecs = atoi(optarg);
            tfnd = 1;
            break;
        default: /* '?' */
            
            fprintf(stderr, "Usage: %s [-t nsecs] [-n] name\n",
                    argv[0]);
            cout << char(optopt) << optind << argv[optind][2] << endl;
            exit(EXIT_FAILURE);
        }
    }

   printf("flags=%d; tfnd=%d; optind=%d\n", flags, tfnd, optind);

   if (optind >= argc) {
        fprintf(stderr, "Expected argument after options\n");
        exit(EXIT_FAILURE);
    }

   printf("name argument = %s\n", argv[optind]);

   /* Other code omitted */

   exit(EXIT_SUCCESS);
}
