#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"


void statusMenu(char *file){
    if (strlen(file) > 0){
        cJSON *json = cJSON_Parse(file);

        system("clear");

        int temperature = cJSON_GetObjectItemCaseSensitive(json, "temperature")->valueint;
        int humidity = cJSON_GetObjectItemCaseSensitive(json, "humidity")->valueint;
        int pplCount = cJSON_GetObjectItemCaseSensitive(json, "total_people")->valueint;
        int *name = cJSON_GetObjectItemCaseSensitive(json, "nome")->valuestring;

        cJSON *outputs = cJSON_GetObjectItemCaseSensitive(json, "output");
        cJSON *inputs = cJSON_GetObjectItemCaseSensitive(json, "input");
        cJSON *pin = NULL;


        printf("Monitoramento da %s | STATUS:\n", name);

        printf("  Temperatura (⁰C)\t  Umidade\t  N⁰ de pessoas\n");
        printf("|| %d⁰C\t|| %d%%\t|| %d\t\n", temperature, humidity, pplCount);

        printf("\n\n==================== Saída ====================\n\n");
        printf("  Pino\t  Valor\t  Tag\n");

        cJSON_ArrayForEach(pin, outputs){
            char *status;
            int gpio = cJSON_GetObjectItemCaseSensitive(pin, "gpio")->valueint;
            char *tag = cJSON_GetObjectItemCaseSensitive(pin, "tags")->valuestring;
            int value = cJSON_GetObjectItemCaseSensitive(pin, "value")->valueint;

            if(value) status = "ON";
            else status = "OFF";
            printf("|| %02d\t|| %s\t|| %s\t\n", gpio, status, tag);
        }

        printf("\n\n==================== Entrada ====================\n\n");
        printf("  Pino\t  Valor\t  Tag\n");  
            
        cJSON_ArrayForEach(pin, inputs){
            char *status;
            int gpio = cJSON_GetObjectItemCaseSensitive(pin, "gpio")->valueint;
            char *tag = cJSON_GetObjectItemCaseSensitive(pin, "tags")->valuestring;
            int value = cJSON_GetObjectItemCaseSensitive(pin, "value")->valueint;

            if(value) status = "ON";
            else status = "OFF";
            printf("|| %02d\t|| %s\t|| %s\t\n", gpio, status, tag);
        }
    }
}