#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc != 4) {
    printf("Usage: memory-user <start> <limit> <increment>\n");
    exit(-1);
  }

  uint start = atoi(argv[1]);
  uint limit = atoi(argv[2]);
  uint increment = atoi(argv[3]);
  uint i;

  int *array;

  for (i = start; i <= limit; i += increment) {
    printf("allocating %d MiB\n", i);

    array = (int *)malloc(i * 1024 * 1024);
    printf("malloc returned %p\n", array);

    if (array == 0) {
      printf("malloc failed\n");
      exit(-1);
    }

    // ----------------------------------------------------------
    // CASE 2: Touch every page
    // (uncomment this block when you want to test real allocation)
    // ----------------------------------------------------------
    /*
    {
      uint j;
      uint bytes = i * 1024 * 1024;
      for (j = 0; j < bytes / sizeof(int); j += 1024) {
        array[j] = j;
      }
      printf("Touched all pages in %d MiB\n", i);
    }
    */

    // ----------------------------------------------------------
    // CASE 3: Touch 1 out of 16 pages
    // ----------------------------------------------------------
    /*
    {
      uint j;
      uint bytes = i * 1024 * 1024;
      for (j = 0; j < bytes / sizeof(int); j += 1024 * 16) {
        array[j] = j;
      }
      printf("Touched ~1/16 of pages in %d MiB\n", i);
    }
    */

    sleep(50);
    printf("freeing %d MiB\n", i);
    free(array);
    sleep(50);
  }

  exit(0);
}