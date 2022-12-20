#include "jsonTranslator.h";
#include "gpio.h";
#include <string.h>

void stringJson(char *jsonData, Server *server){
    cJSON *json = cJSON_Parse(jsonData);

    server->name = malloc(sizeof(char)*20);
    strcpy(server->name, cJSON_GetObjectItemCaseSensitive(json, "nome")->valuestring);

    server->outputs = cJSON_GetObjectItemCaseSensitive(json, "outputs");
    server->inputs = cJSON_GetObjectItemCaseSensitive(json, "inputs");
}

cJSON* getArrayData(char *gpioType, Server *server, int *pplCount){
    cJSON *gpio;
    if (strcmp(gpioType, "outputs") != 0) gpio = server->outputs;
    else gpio = server->inputs;

    cJSON *gpioArray = cJSON_CreateArray();
    int length = cJSON_GetArraySize(gpio);

    for (int j = 0; j < length; j++){
        cJSON *item = cJSON_GetArrayItem(gpio, j);
        char *type = cJSON_GetObjectItemCaseSensitive(item, "type")->valuestring;

        if (strcmp(type, "contagem") != 0){
            cJSON *arrayItem = cJSON_CreateObject();
            char *tag = cJSON_GetObjectItemCaseSensitive(item, "tag")->valuestring;
            int gpioPin = cJSON_GetObjectItemCaseSensitive(item, "gpio")->valueint;
            int value = readGPIO(gpio);

            cJSON_AddNumberToObject(arrayItem, "gpio", gpioPin);
            cJSON_AddStringToObject(arrayItem, "type", type);
            cJSON_AddStringToObject(arrayItem, "tag", tag);
            cJSON_AddNumberToObject(arrayItem, "value", value);
            cJSON_AddItemToArray(gpioArray, arrayItem);
        } else {
            int gpioPin = cJSON_GetObjectItemCaseSensitive(item, "gpio")->valueint;
            if(gpioPin == 20) *pplCount++;
            else *pplCount--;
        }
    }

    return gpioArray;
}

void jsonFinal(char **jsonData, Server *server, int temperature, int humidity, int *pplCount){
    cJSON *gpioObject = cJSON_CreateObject();

    cJSON_AddItemToObject(gpioObject, "outputs", getArrayData("outputs", server, pplCount));
    cJSON_AddItemToObject(gpioObject, "inputs", getArrayData("inputs", server, pplCount));


    cJSON_AddStringToObject(gpioObject,"nome", server->name);
    cJSON_AddNumberToObject(gpioObject, "temperature", temperature);
    cJSON_AddNumberToObject(gpioObject, "humidity", humidity);
    cJSON_AddNumberToObject(gpioObject, "total_people", *pplCount);

    *jsonData = malloc(5000);
    strcpy(*jsonData, cJSON_Print(gpioObject));
}