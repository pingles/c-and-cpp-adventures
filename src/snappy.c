#include <stdio.h>
#include <stdlib.h>
#include "snappy-c.h"

/* Compile with
 * gcc src/snappy.c -o snappy -Wall -I/usr/local/Cellar/snappy/1.0.3/include -L/usr/local/Cellar/snappy/1.0.3/lib -lsnappy
 */

int
main(void)
{
  int i;
  char c, input[1024];
  
  printf("Provide some input: ");
  do
  {
    scanf("%c", &c);
    input[i] = c;
    i++;
  } while (c != '\n');
  
  size_t output_length = snappy_max_compressed_length(i);
  char* output = (char*)malloc(output_length);

  if (snappy_compress(input, i, output, &output_length) == SNAPPY_OK) {
    printf("Succcess!\n");
    printf("Input size: %d\n", i);
    printf("Output (%zu): ", output_length);
    for (i = 0; i < output_length; i++) {
      printf("%c", output[i]);
    }
  }

  printf("==============================================\n");
  size_t decompressed_length;
  if (snappy_uncompressed_length(output, output_length, &decompressed_length) != SNAPPY_OK) {
    printf("Failed\n");
  }
  char* decompressed = (char*)malloc(decompressed_length);

  if (snappy_uncompress(output, output_length, decompressed, &decompressed_length) == SNAPPY_OK) {
    printf("Success!\n");
    printf("Decompressed (%zu): ", decompressed_length);
    for (i = 0; i < decompressed_length; i++) {
      printf("%c", decompressed[i]);
    }
  }
  printf("\n");

  free(output);
  
  return EXIT_SUCCESS;
}
