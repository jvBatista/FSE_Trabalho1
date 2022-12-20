#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <pthread.h>
#include <signal.h>

#include "cJSON.h"
#include "home.h"
#include "file.h"

#define IP "192.168.0.53"
#define PORT 10000
#define MAX_SIZE 5000
#define true 1
#define false 0

pthread_t SOCKET_GET, SOCKET_SEND, MENU_ID;
int clientSocket, id;
char *file, *jsonData, gpio = false;

void receiveData(){
  char *data = malloc(MAX_SIZE);
  int bufferSize = recv(clientSocket, data, MAX_SIZE, 0);
  if (bufferSize) strcpy(jsonData, data);
  
  free(data);
  sleep(1);
}

void sendData(){
  int fileSize = strlen(file);
  
  if (fileSize > 0){
    if (!gpio)
      send(clientSocket, file, fileSize, 0);
    else{
      char *pin[5];
      printf("\nDigite o pino cujo valor deseja alterar: ");
      scanf("%s", pin);

      send(clientSocket, pin, 5*sizeof(char), 0);
      gpio = false;
    }
  }
  sleep(1);
}

void changeSignal(int signal){
  printf("\nAguarde...\n");

  if (signal == SIGQUIT) gpio = true;
  if (signal == SIGTSTP){
    if(id == 1) id = 2;
    else id = 2; 
    strcpy(file, readFile(id));
  }
}

void handleServer(int id){
  signal(SIGQUIT, changeSignal);
  signal(SIGTSTP, changeSignal);

  int serverSocket;
  struct sockaddr_in serverAddr, clientAddr;
  unsigned int clientSize;
  int opt = 1;

  if ((serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) perror("Socket Error");

  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = inet_addr(IP);
  serverAddr.sin_port = htons(PORT);

  setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  if (bind(serverSocket, &serverAddr, sizeof(serverAddr)) < 0) perror("Bind Error");
  if (listen(serverSocket, 10) < 0) perror("Listen Error");

  file = malloc(MAX_SIZE);
  jsonData = malloc(MAX_SIZE);
  strcpy(file, readFile(id));
  clientSize = sizeof(clientAddr);
  
  if ((clientSocket = accept(serverSocket, &clientAddr,&clientSize)) < 0) perror("Accept Error");

  while (1){
    pthread_create(&MENU_ID, NULL, statusMenu, jsonData);
    pthread_join(MENU_ID, NULL);

    pthread_create(&SOCKET_GET, NULL, receiveData, NULL);
    pthread_create(&SOCKET_SEND, NULL, sendData, NULL);
    pthread_join(SOCKET_GET, NULL);
    pthread_join(SOCKET_SEND, NULL);

    sleep(1);
  }

  close(serverSocket);
}