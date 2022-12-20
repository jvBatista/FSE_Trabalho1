#include <stdio.h>
#include "centralServer.h";
#include "home.h";

int main(){
    int id;

    system("clear");
    printf("FSE-2022.2 | TRABALHO 1\n");
    printf("SISTEMA DE AUTOMAÇÃO PREDIAL\n");

    while (1){
        printf("- Digite:\n\n1, para monitorar Sala 01\n2, para monitorar Sala 02\n\n");
        scanf("%d", &id);

        if (id < 1 || id > 2)
            printf("\nComando inválido! Tente novamente...\n");
        else
            break;
    }

    handleServer(id);
    return 0;
}

