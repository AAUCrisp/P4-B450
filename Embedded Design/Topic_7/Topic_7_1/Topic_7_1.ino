#include <stdio.h>
#define CONFIG_FREERTOS_HZ 1000

int circumference = 300;    // In Centimeters
int revolutions = 0;        // Revolutions since last reset (each second)
double total_distance = 0;


void read_rotation() {
    // Reading random things... 
    // Return Random number, with a RPM count.
    
    // Something like a Hall-effect-sensor for each 90 degrees, would give us fairly precise readings.... Technically 1 sensor and 4 magnets though.
    // Hall-effect-sensor would work via interupts (I guess), measuring the amount of interupt pr. second... would have to devide our measurements by 4 then.

    int delay;
    while (1)
    {
        delay = random(1, 500);
        vTaskDelay(delay);
        revolutions++;
    }
}

double distance_traveled(float rpm) {
    float current_distance = rpm * circumference;    // Revolutions times the circumference of the shaft.
    return current_distance;
}


void setup() {
    xTaskCreate(read_rotation, "RPM", 10000, NULL, 3, NULL);
    while (1)
    {
        total_distance += distance_traveled(revolutions);
        revolutions = 0;
        vTaskDelay(1000); // Wait for a second before measuring new distance traveled
    }
}

void loop() {

}