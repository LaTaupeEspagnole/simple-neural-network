#include "network.h"

#include "error_msg.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
  argc = argc;
  argv = argv;

  unsigned layers[3] = {2, 3, 1};
  struct network* network = init_network(3, layers);
  if (!network)
    err_print_and_quit("ERROR: Can't create network", 2);

  char* serialized = serialize_network(network);
  if (!serialized)
    err_print_and_quit("ERROR: Can't serialize the network", 3);

  print_network(network);

  return 0;
}
