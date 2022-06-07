/*
Topic 4
Objective:
1. Design a program that blinks 10 times a second.
2. Write a function that prints counts ever 400 ms.

*/
#define CONFIG_FREERTOS_HZ 1000

void t1(void *parameter) {
    int count = 0;
    long start = 0;
    long stop = 0;
    long sum = 0;
    
    while (1) {
        // Tasks are delayed 50 ms each, or 100 ms in total,
        // in order for it to blink 10 times a second.
        // start = xTaskGetTickCount();
        digitalWrite(LED_BUILTIN, HIGH);
        vTaskDelay(50 / portTICK_PERIOD_MS);  // Blocks task for 50 ms.
        digitalWrite(LED_BUILTIN, LOW);
        vTaskDelay(50 / portTICK_PERIOD_MS);  // Blocks task for 50 ms.
        /*stop = xTaskGetTickCount();
        count++;
        sum += ((stop - start) / portTICK_PERIOD_MS);

        if (sum == 1000) {
            Serial.println("====================");
            Serial.print("Time taken: ");
            Serial.println(sum);
            Serial.print("count task1: ");
            Serial.println(count);
            Serial.println("====================");
            sum = 0;
            count = 0;
        }
        */
    }
}

void t2(void *parameter) {
    int count = 0;
    while (1) {
        count++;
        Serial.print("Count task2: ");
        Serial.println(count);
        vTaskDelay(400 / portTICK_PERIOD_MS);  // Blocks task for 400 ms.
    }
}

void setup() {
    Serial.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);

    xTaskCreate(t1, "BlinkLED", 10000, NULL, 1, NULL);
    xTaskCreate(t2, "Counter", 10000, NULL, 1, NULL);
}

void loop() {
}
