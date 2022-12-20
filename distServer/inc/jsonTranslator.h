#ifndef JSONTRANSLATOR_H_
#define JSONTRANSLATOR_H__H_

#include "cJSON.h";

typedef struct Server{
    int port;
    char *IP, *name;
    cJSON *outputs, *inputs;
} Server;

void stringJson(char *jsonData, Server *server);
void jsonFinal(char **jsonData, Server *server, int temperature, int humidity, int *pplCount);

#endif 