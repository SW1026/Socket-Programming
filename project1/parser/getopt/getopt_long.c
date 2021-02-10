#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

void help(char *argv)
{
    printf("Usage : %s options\n", argv);
    printf("--debug\n--create\n--file [file name]\n--help\n");
}

int verbose_flag;
int main (int argc, char **argv)
{
  int c;

  verbose_flag = 0;
  while (1)
    {
      static struct option long_options[] =
        {
          {"debug", no_argument,        NULL, 'd'},
          {"verbose", no_argument,       &verbose_flag, 1},
          {"create", no_argument,       0, 'c'},
          {"help",   no_argument,       0, 'h'},
          {"file",  required_argument,  0, 'f'},
          {0, 0, 0, 0}
        };
      /* getopt_long stores the option index here. */
      int option_index;

      c = getopt_long (argc, argv, "dchf:",
                       long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1)
        break;

      switch (c)
      {
          case 0:
            break;

          case 'd':
            printf("debug mode\n");
            break;

          case 'c':
            printf("create mode\n");
            break;

          case 'f':
            printf ("file name is `%s'\n", optarg);
            break;

          case 'h':
            help(argv[0]);
            break;

          case '?':
            help(argv[0]);
            break;
          //default:
          //   help(argv[0]);
       }
    }
    if(verbose_flag)
      printf("verbos flag is set\n");
    return 1;
}
