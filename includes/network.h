#ifndef NETWORK_H
# define NETWORK_H

# include "libmatrix.h"

struct network
{
  /* Network Structure */
  unsigned nb_layers;
  unsigned* size_layers;
  struct mat** weights_biases;

  /* Metadata */
  char* activation_func;
  unsigned nb_epoch;
  float learning_rate;
  float last_succes_rate;
  unsigned long time_runing;
};

void print_network(struct network* network);

struct network* init_network(unsigned nb_layers, unsigned* size_layers);

char* serialize_network(struct network* network);

struct network* deserialize_network(char* str);

#endif /* !NETWORK_H */
