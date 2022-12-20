#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *readFile(int path){
  char *filePath;
  long size;

  if (path == 1) filePath = "./config/configuracao_sala_01.json";
  else filePath = "./config/configuracao_sala_02.json";

  FILE *file = fopen(filePath, "r");

  if (file == NULL){
    printf("**ERRO! Não foi possível abrir o file...\n");
    exit(0);
  }

  fseek(file, 0, SEEK_END);
  size = ftell(file);
  fseek(file, 0, SEEK_SET);
  char *buffer = malloc(size + 1);

  if (buffer) fread(buffer, 1, size, file);
  fclose(file);

  return buffer;
}