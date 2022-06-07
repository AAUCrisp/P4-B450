/*
Topic 7.1
Objective:
1. Create dummy program that simulates an elevator.
2. Create dummy functions to simulate elevator functions.
*/

void IRAM_ATTR handleInterrupt1() {
    if (ButtonSpam1) {
        Serial.println("Stop idiot");
        do nothing
    } else {
        ResumeTask(1);
        CallTask1();
    }
}
void IRAM_ATTR handleInterrupt2() {
    if (ButtonSpam2) {
        Serial.println("Stop idiot");
        do nothing
    } else {
        ResumeTask(2);
        CallTask2();
    }
}
void IRAM_ATTR handleInterrupt3() {
    if (ButtonSpam3) {
        Serial.println("Stop idiot");
        do nothing
    } else {
        ResumeTask(3);
        CallTask3();
    }
}

void Task1() {
    if (ElevatorStatus == stopped) {
        SuspendTask(1);
    } else {
        GoToFloor1();
    }
}
void Task2() {
    if (ElevatorStatus == stopped) {
        SuspendTask(2);
    } else {
        GoToFloor2();
    }
}
void Task3() {
    if (ElevatorStatus == stopped) {
        SuspendTask(3);
    } else {
        GoToFloor3();
    }
}

void main() {
    pinMode(Floor1, FALLING);
    pinMode(Floor2, FALLING);
    pinMode(Floor3, FALLING);
    attachInterrupt(Floor1, handleInterrupt1);
    attachInterrupt(Floor2, handleInterrupt2);
    attachInterrupt(Floor3, handleInterrupt3);

    ElevatorStatus(Elevator == Stopped || Elevator == Running);

    xTaskCreate(ToFloor1, task1);
    xTaskCreate(ToFloor2, task2);
    xTaskCreate(ToFloor3, task3);
}

void loop() {
}