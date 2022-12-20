
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <pthread.h>

#include "cJSON.h"
#include "gpio.h"
#include "dht22.h"
#include "jsonTranslator.h"

#define IP "192.168.0.52"
#define PORT 10200
#define MAX_SIZE 5000

pthread_t SOCKET_GET, SOCKET_SEND;
int temperature = 0, humidity = 0, pplCount = 0, toggleValue = 0, clientSocket;
char *msg, *data;

void receiveDataRequest(){
    int i;
    char num[5];
    recv(clientSocket, data, MAX_SIZE, 0);

    while (data[i] != '\0'){
        if(i > 3)
            break;
        i++;
    }

    if (i < 3){
        printf("porta %d recebida", atoi(num));
        toggleValue = 1;
        strcpy(num, data);
        int value = readGPIO(atoi(num));

        if (!value){
            printf("Ligando %d\n", atoi(num));
            switchPin(atoi(num), 1);
        }
        else{
            printf("Desligando %d\n", atoi(num));
            switchPin(atoi(num), 0);
        }
    } else{
        toggleValue = 0;
        strcpy(msg, data);
    }

    sleep(1);
}

void sendData(){
    if (strlen(msg) > 0 && !toggleValue){
        char *final = malloc(MAX_SIZE);
        Server *serverConfig = malloc(sizeof(Server));

        stringJson(msg, serverConfig);
        read_dht_data(&temperature, &humidity, 0);

        jsonFinal(&final, serverConfig, temperature, humidity, &pplCount);
        send(clientSocket, final, MAX_SIZE, 0);

        free(final);
        free(serverConfig);
    }

    sleep(1);
}

void handleServer(){
    struct sockaddr_in serverAddr;
    unsigned short serverPort;
    char *serverIP;

    msg = calloc(MAX_SIZE, sizeof(char));
    data = calloc(MAX_SIZE, sizeof(char));
    serverIP = inet_addr(IP);
    serverPort = PORT;

    if ((clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        perror("Socket");
        exit(0);
    }
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = serverIP;
    serverAddr.sin_port = htons(serverPort);

    while (connect(clientSocket, &serverAddr, sizeof(serverAddr)) < 0);

    while (1){
        pthread_create(&SOCKET_GET, NULL, receiveDataRequest, NULL);
        pthread_join(SOCKET_GET, NULL);

        pthread_create(&SOCKET_SEND, NULL, sendData, NULL);
        pthread_join(SOCKET_SEND, NULL);
    }
}