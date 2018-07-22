#include "network.h"

#include "error_msg.h"
#include "buffer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct network* init_network(unsigned nb_layers, unsigned* size_layers)
{
  struct network* network = malloc(sizeof (struct network));
  if (!network)
    err_out_of_mem();

  network->size_layers = malloc(nb_layers * sizeof (unsigned));
  if (!network)
    err_out_of_mem();

  network->weights_biases = malloc(nb_layers * sizeof (struct mat*));
  if (!network)
    err_out_of_mem();

  network->nb_layers = nb_layers;
  for (unsigned i = 0; i < nb_layers; ++i)
    network->size_layers[i] = size_layers[i];
  for (unsigned i = 1; i < nb_layers; ++i)
  {
    network->weights_biases[i] = dec_mat(network->size_layers[i] + 1,
                                         network->size_layers[i - 1] + 1);
    if (!network->weights_biases[i])
      err_print_and_quit("ERROR: Can't declare matrix", 43);
  }
  network->activation_func = NULL;
  network->nb_epoch = 0;
  network->learning_rate = 0.0f;
  network->last_succes_rate = 0.0f;
  network->time_runing = 0;

  return network;
}

void print_network(struct network* network)
{
  printf("Number of layers : %u\n", network->nb_layers);
  printf("Size of each layer : ");
  for (unsigned i = 0; i < network->nb_layers; ++i)
    printf("%u, ", network->size_layers[i]);
  printf("\nActivation function : %s\n", network->activation_func);
  printf("Number of epoch done : %u\n", network->nb_epoch);
  printf("Learning rate : %f\n", network->learning_rate);
  printf("Succes rate : %f%%\n", network->last_succes_rate);
  printf("Running time : %lu seconds\n", network->time_runing);
  printf("Layer matrices : \n");
  for (unsigned i = 1; i < network->nb_layers; ++i)
  {
    print_mat(network->weights_biases[i]);
    printf("\n");
  }
}

char* net_get_next_line(char* str)
{
  while (*str != '\0' && *str != '\n')
    ++str;
  if (*str == '\0')
    return NULL;
  return str + 1;
}

char* serialize_network(struct network* network)
{
  struct buffer buffer = {.size = 0, .buffer = NULL};
  char* tmp = malloc(100 * sizeof(char));
  if (!tmp)
    err_out_of_mem();

  sprintf(tmp, "%u\n", network->nb_layers);
  add_buffer(&buffer, tmp, strlen(tmp));
  for (unsigned i = 0; i < network->nb_layers; ++i)
  {
    sprintf(tmp, "%u\n", network->size_layers[i]);
    add_buffer(&buffer, tmp, strlen(tmp));
  }
  char* tmp2 = NULL;
  for (unsigned i = 1; i < network->nb_layers; ++i)
  {
    tmp2 = serialize_matrix(network->weights_biases[i]);
    add_buffer(&buffer, tmp2, strlen(tmp2));
    free(tmp2);
  }
  if (!network->activation_func)
    add_buffer(&buffer, "\n", 1);
  else
    add_buffer(&buffer, network->activation_func, strlen(network->activation_func));
  sprintf(tmp, "%u\n", network->nb_epoch);
  add_buffer(&buffer, tmp, strlen(tmp));
  sprintf(tmp, "%f\n", network->learning_rate);
  add_buffer(&buffer, tmp, strlen(tmp));
  sprintf(tmp, "%f\n", network->last_succes_rate);
  add_buffer(&buffer, tmp, strlen(tmp));
  sprintf(tmp, "%lu\n", network->time_runing);
  add_buffer(&buffer, tmp, strlen(tmp));

  return buffer.buffer;
}

struct network* deserialize_network(char* str)
{
  struct network *network = malloc(sizeof (struct network));
  if (!network)
    err_out_of_mem();

  if (!sscanf(str, "%u\n", &network->nb_layers))
    err_print_and_quit("ERROR: Parsing nb_layers", 44);
  network->size_layers = malloc(network->nb_layers * sizeof (unsigned));
  str = net_get_next_line(str);
  for (unsigned i = 0; i < network->nb_layers; ++i)
  {
    if (!sscanf(str, "%u\n", network->size_layers + i))
      err_print_and_quit("ERROR: Parsing size_layers", 44);
    str = net_get_next_line(str);
  }
  network->weights_biases = malloc(network->nb_layers * sizeof (struct mat*));
  if (!network->weights_biases)
    err_out_of_mem();
  char* tmp = NULL;
  for (unsigned i = 1; i < network->nb_layers; ++i)
  {
    network->weights_biases[i] = deserialize_matrix(str, &tmp);
    str = tmp;
  }
  if (!sscanf(str, "%s\n", network->activation_func))
    network->activation_func = NULL;
  str = net_get_next_line(str);
  if (!sscanf(str, "%u\n", &network->nb_epoch))
    err_print_and_quit("ERROR: Parsing nb_epoch", 44);
  str = net_get_next_line(str);
  if (!sscanf(str, "%f\n", &network->learning_rate))
    err_print_and_quit("ERROR: Parsing learning_rate", 44);
  str = net_get_next_line(str);
  if (!sscanf(str, "%f\n", &network->last_succes_rate))
    err_print_and_quit("ERROR: Parsing last_succes_rate", 44);
  str = net_get_next_line(str);
  if (!sscanf(str, "%lu\n", &network->time_runing))
    err_print_and_quit("ERROR: Parsing time_runing", 44);

  return network;
}
