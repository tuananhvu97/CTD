#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm.h"
#define DEFAULT_STACK_SIZE 2048
#define DEFAULT_CODE_SIZE 1024

extern int stackSize;
extern int codeSize;


/******************************************************************/

int main(int argc, char *argv[]) {
  int i;
  FILE* f;

  stackSize = DEFAULT_STACK_SIZE;
  codeSize = DEFAULT_CODE_SIZE;

  if (argc <= 1) {
    printf("kplrun: no input file.\n");
    return -1;
  }

  f = fopen(argv[1],"r");
	    
  if (f == NULL) {
    printf("kplrun: Can\'t read input file!\n");
    return -1;
  }

  initVM();
  if (loadExecutable(f) == 0) {
    printf("kplrun: Wrong executable format!\n");
    fclose(f);
    cleanVM();
    return -1;
  }
  fclose(f);

  switch (run()) {
  case PS_DIVIDE_BY_ZERO:
    printf("Runtime error: Divide by zero!\n");
    break;
  case PS_STACK_OVERFLOW:
    printf("Runtime error: Stack overflow!\n");
    break;
  case PS_IO_ERROR:
    printf("Runtime error: IO error!\n");
    break;
  default:
    break;
  }
  cleanVM();
  return 0;
}
