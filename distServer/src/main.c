#include <wiringPi.h>
#include "distServer.h"

int main(){
    wiringPiSetup();
    handleServer();
}