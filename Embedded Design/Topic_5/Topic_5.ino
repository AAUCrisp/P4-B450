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

void print_task(void *parameter) {
    while (1) {
        Serial.print("Count: ");
        Serial.println(*(int *)parameter);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void increment_task(void *parameter) {
    while (1) {
        a++;
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void setup() {
    Serial.begin(115200);

    xTaskCreate(print_task, "Print", 10000, (void *)&a, 1, NULL);
    xTaskCreate(increment_task, "Counter", 10000, NULL, 2, NULL);
    delay(3000);
}

void loop() {
}
