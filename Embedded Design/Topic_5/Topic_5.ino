/*
Topic 5
Objective:
1. Design program with 1 high priority task running at Ts = 100 ms,
   and print out shared variable int.
2. Second task with low priority updating,
   and incrementing shared variable int every 2 s.
*/

#define CONFIG_FREERTOS_HZ 1000

char Str[] = "Hello world";
int a = 1;

void t1(void *parameter) {
    while (1) {
        // Serial.println(*(char *)parameter);
        Serial.print("Count: ");
        Serial.println(*(int *)parameter);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void t2(void *parameter) {
    while (1) {
        a++;
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void setup() {
    Serial.begin(115200);

    xTaskCreate(t1, "t1", 10000, (void *)&a, 2, NULL);
    // xTaskCreate(t1, "t1", 10000, (void *) &Str,  2, NULL);
    xTaskCreate(t2, "t2", 10000, NULL, 1, NULL);
    delay(3000);
}

void loop() {
}
