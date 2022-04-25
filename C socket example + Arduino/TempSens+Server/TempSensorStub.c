//TempSensorStub.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void* TempGen(void* arg){ //Pointer til void som return type
    int Count = 10; //Antal målinger
    float Avg[Count], sum, n;
    float avg;

    srand(time(NULL)); //Genererer pseudo-random numre

    for(int i = 0; i < Count; i++){ //Temp generatoren
        n=((float)rand()/(float)RAND_MAX)*(50.0); //Generer en random float fra 0.0 - 50.0
        Avg[i] = n; //Indsæt i Avg array
        sum += Avg[i]; //Tager summen
        //printf("Temp: %f\n", Avg[i]);
        int sleep = usleep(1*100000); //Vent 1 sekund
    }
    avg = sum / Count; //Tag gennemsnittet og læg i avg variablen
    printf("Avg is: %0.1f\n", avg);

    float *result = (float*)malloc(sizeof(result)); //Laver en pointer 'result' og allokerer en blok af hukommelse. Den returnerer en pointer til den allokerede hukommelse "float" 
    *result = avg; //Lægger værdien af avg over i pointeren result
    printf("Thread address: %p\n", result); //Tjekker addressen

    return (void*) result; //Returnerer void * (pointeren) result
}