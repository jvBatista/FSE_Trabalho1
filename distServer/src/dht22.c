#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_TIMINGS 100
#define MAX_STACK 5
#define DHT_PIN 29 
#define DEBUG 0

/*
 *  dht.c:
 *      Author: Juergen Wolf-Hofer
 *      based on / adapted from http://www.uugear.com/portfolio/read-dht1122-temperature-humidity-sensor-from-raspberry-pi/
 *	reads temperature and humidity from DHT11 or DHT22 sensor and outputs according to selected mode
 */

void read_dht_data(int *temperatura, int *humidade, int contador)
{
    uint8_t ultimo_estado = HIGH;
    uint8_t aux_contador = 0;
    uint8_t j = 0;

    int data[5] = {0, 0, 0, 0, 0};
    int aux = 0;

    pinMode(DHT_PIN, OUTPUT);
    digitalWrite(DHT_PIN, LOW);
    delay(18);
    pinMode(DHT_PIN, INPUT);

    for (uint8_t i = 0; i < MAX_TIMINGS; i++){
        aux_contador = 0;
        while (digitalRead(DHT_PIN) == ultimo_estado)
        {
            aux_contador++;
            delayMicroseconds(1);
            if (aux_contador == 255) break;
        }
        ultimo_estado = digitalRead(DHT_PIN);
        if (aux_contador == 255) break;
        if ((i >= 4) && (i % 2 == 0)){
            data[j / 8] <<= 1;
            if (aux_contador > 16) data[j / 8] |= 1;
            j++;
        }
    }

    if ((j >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))){
        float aux_humidade; 
        float aux_fahrenheit;
        float celsius;

        aux_humidade = (float)((data[0] << 8) + data[1]) / 10;
        if (aux_humidade > 100) aux_humidade = data[0]; 
        celsius = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
        if (celsius> 125) celsius = data[2]; 
        if (data[2] & 0x80)celsius = -celsius;
        
        aux_fahrenheit = celsius * 1.8f + 32;

        *humidade = aux_humidade;
        *temperatura = celsius;
        if (DEBUG) printf( "read_dht_data() Humidity = %.1f %% Temperature = %.1f *C (%.1f *F)\n", humidade, temperatura, aux_fahrenheit);
    }
    else{
		if (DEBUG) printf( "read_dht_data() Data not good, skip\n" );
        printf("Realizando nova leitura\n");
        delay(100);
        if (contador != MAX_STACK) read_dht_data(temperatura, humidade, contador + 1);
        else{
            *humidade *= 1;
            *temperatura *= 1;
            printf("Sensor DHT22 não lido.\nUtlizando valor da leitura anterior\n");
        }
    }
}

