#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int start() {
  printf("%s\n", "started");
  return 0;
}

int next() {
  printf("%s\n", "next please");
  return 0; 
}

int cancel() {
  printf("%s\n", "cancelled");
  return 0;
}

int main(int argc, char *argv[]){
  if (argc != 2) {
    printf("Veuillez préciser un et un seul argument (start / next / cancel)\n");
    return -1;
  }
  
  if (!strcmp(argv[1], "start")) {
    start();
    return 0;
  }

  if (!strcmp(argv[1], "next")) {
    next();
    return 0;
  }

  if (!strcmp(argv[1], "cancel")) {
    cancel();
    return 0;
  }

  printf("%s\n", "Veuillez sélectionner un argument valide (start / next / cancel)");
  return -1;
}
