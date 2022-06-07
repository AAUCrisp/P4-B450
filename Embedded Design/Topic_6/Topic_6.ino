/*
Objective:
1. Receive msgs in ISR from task.
2. Send reply to task by using another queue.

Requirements:
1. First msg queue (1) from task to ISR consists of size 7 ints.
2. Other msg queue (2) from ISR to task of size 6 ints.
3. ISR triggered by external interrupt (pulling LOW).
4. ISR receives message from first msg queue (1).
5. If value == 123, increment local counter, send counter no. to task.

6. Tasks loop every second.
7. Use random interval for values 122-124 (both inclusive?).
8. Send values to ISR via msg queue (1) from task.
9. Check if value is received from ISR msg queue (2).
10. Print received value.
*/

#define CONFIG_FREERTOS_HZ 1000

// Declaration of interrupt pin, random integer, counter, integer buffer for task and ISR.
int interruptPin = 23;
int rand_int;
int buffer_ISR;
int buffer_task = 0;

// Makes the variable able to change inside ISR function.
static volatile int count = 0;

// Declaration of msg queues.
QueueHandle_t msg_queue_task;
QueueHandle_t msg_queue_ISR;
BaseType_t msg_queue_send_task;

// Task function to send random values from 122-124 to ISR queue.
void Task(void *parameter) {
    int res;

    while (1) {
        // Check msg_queue_task for count number.
        if (pdTRUE == xQueueReceive(msg_queue_ISR, &buffer_task, 500)) {
            res = 0;
        } else {
            res = -1;
        }
        if (res == 0) {
            Serial.println("");
            Serial.println("==============================");
            Serial.print("Count from interrupt: ");
            Serial.println(buffer_task);
            Serial.println("==============================");
            Serial.println("");
        }

        // Generate random int ranging from 122-124
        // and send count to task.
        rand_int = random(122, 125);
        msg_queue_send_task = xQueueSend(msg_queue_task, &rand_int, 0);

        // If successful return pdTRUE
        // and print rand_int number.
        if (msg_queue_send_task == pdTRUE) {
            Serial.print("Rand_int: ");
            Serial.println(rand_int);
        }
        vTaskDelay(250);
    }
}

// ISR function to receive random values from task.
void IRAM_ATTR handleInterrupt() {
    // Check msg_queue_task for random values.
    if (pdTRUE == xQueueReceiveFromISR(msg_queue_task, &buffer_ISR, 0)) {
        Serial.println("");
        Serial.println("==============================");
        Serial.print("Value from task: ");
        Serial.println(buffer_ISR);

        // If value == 123 increment count and send to task.
        if (buffer_ISR == 123) {
            Serial.println("Value was == 123");
            Serial.println("==============================");
            Serial.println("");
            count++;
            xQueueSendFromISR(msg_queue_ISR, (void *)&count, NULL);
            digitalWrite(LED_BUILTIN, HIGH);
        } else {
            Serial.println("Value was not == 123");
            Serial.println("==============================");
            Serial.println("");
            digitalWrite(LED_BUILTIN, LOW);
        }
    }
}

void setup() {
    Serial.begin(115200);

    // Create msg queues for task and ISR.
    msg_queue_task = xQueueCreate(7, sizeof(int));
    msg_queue_ISR = xQueueCreate(6, sizeof(int));

    pinMode(interruptPin, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);

    // Attach interrupt to handleInterrupt function
    // and create task to run Task function.
    attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);
    xTaskCreate(Task, "Task", 10000, NULL, 3, NULL);
}

void loop() {
}
