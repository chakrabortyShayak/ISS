#include "types.h"
#include <cstdio>

unsigned int memory[MEM_SIZE];

void printMemory(){
  int i;
  //printf("---------------------\n");
  for(i=0;i<MEM_SIZE;i++){
    printf("%u\n", memory[i]);
  }
  printf("-------------------------------\n");
}
