/*
Topic 7.2
Objective:
1. Create two tasks, Producer and Consumer.
2. Send msg from Producer to Consumer and send reply back to Producer.
*/

#define CONFIG_FREERTOS_HZ 1000

// Declaration of msg queues.
QueueHandle_t msg_queue_producer;
QueueHandle_t msg_queue_consumer;

// Declaration of tasks.
TaskHandle_t Producer;
TaskHandle_t Consumer;

void Producer_Task(void *parameter) {
    // Producer message to send.
    char msg[] = "Hello from Producer!";

    // Producer buffer.
    char buffer_producer[64];

    // Amount of ticks to block.
    TickType_t TicksBlock = 500;
    TickType_t TicksBlock2 = 0;

    // Process the message.
    int pro;

    while (1) {
        // Producer sends message to queue.
        if (xQueueSend(msg_queue_producer, &msg, TicksBlock) == pdTRUE) {
            Serial.println("Producer: Message sent!");
            Serial.println("");
        }
        // Suspends Producer task, waiting for Consumer to receive msg.
        vTaskSuspend(Producer);
        vTaskDelay(2500);

        // Producer receives msg from Consumer.
        Serial.println("=======================================================");
        Serial.println("Producer: Waiting for reply...");
        if (xQueueReceive(msg_queue_consumer, &buffer_producer, TicksBlock2) == pdTRUE) {
            Serial.print("Producer - Message from Consumer: ");
            Serial.println(buffer_producer);
            Serial.println("=======================================================");
            Serial.println("");
        }
        // Resume Consumer task, lets it wait for next message.
        vTaskResume(Consumer);
    }
}

void Consumer_Task(void *parameter) {
    // Consumer reply message.
    char reply[] = "Hello from Consumer!";

    // Consumer buffer.
    char buffer_consumer[64];

    // Amount of ticks to block.
    TickType_t TicksBlock = 0;
    TickType_t TicksBlock2 = 500;

    // Process the message.
    int con;

    while (1) {
        // Consumer receives message from queue.
        Serial.println("=======================================================");
        Serial.println("Consumer: Waiting for message from Producer...");
        if (xQueueReceive(msg_queue_producer, &buffer_consumer, TicksBlock) == pdTRUE) {
            Serial.print("Consumer - Message from Producer: ");
            Serial.println(buffer_consumer);
            Serial.println("=======================================================");
            Serial.println("");
            con = 1;
        } else {
            con = 0;
        }
        // Resumes Producer task to receive msg from Consumer.
        vTaskResume(Producer);

        // Consumer sends reply to queue.
        if (con = 1) {
            if (xQueueSend(msg_queue_consumer, &reply, TicksBlock2) == pdTRUE) {
                con = 0;
                Serial.println("Consumer: Reply sent!");
                Serial.println("");
            }
        }
        // Suspends Consumer waiting for Producer to receive msg.
        vTaskSuspend(Consumer);
        vTaskDelay(2500);
    }
}

void setup() {
    Serial.begin(115200);

    // Create msg queues for producer and consumer.
    msg_queue_producer = xQueueCreate(10, sizeof(char) * 21);
    msg_queue_consumer = xQueueCreate(10, sizeof(char) * 21);

    pinMode(LED_BUILTIN, OUTPUT);

    // Create Producer and Consumer tasks.
    xTaskCreate(Producer_Task, "Producer_Task", 10000, NULL, 1, &Producer);
    xTaskCreate(Consumer_Task, "Consumer_Task", 10000, NULL, 1, &Consumer);
}

void loop() {
}
