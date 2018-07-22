#include "error_msg.h"

#include <stdlib.h>
#include <stdio.h>

void err_print_and_quit(char* msg, int exit_status)
{
  fprintf(stderr, "%s\n", msg);
  exit(exit_status);
}

void err_out_of_mem()
{
  err_print_and_quit("ERROR: Out of memory", 42);
}
