const byte ledPin = LED_BUILTIN;
const byte interruptPin0 = 2;
const byte interruptPin1 = 3;

volatile byte state = LOW;  // to avoid register optimization
volatile boolean ledON = false;
volatile int cnt0 = 0, cnt1 = 0;

// Interrupt Service Routine (ISR)
void onnn() {
    cnt0++;
    ledON = true;  // interrupt is dsiabled when running this code
}

// Interrupt Service Routine (ISR)
void offf() {
    cnt1++;
    ledON = false;
}

void setup() {
    // Setup internal pull-up resistors
    pinMode(interruptPin0, INPUT_PULLUP);
    pinMode(interruptPin1, INPUT_PULLUP);

    Serial.begin(115200);

    // Setup interrupt handlers
    // digitalPinToInterrput function simplifies the use of interrupts pin,
    // instead of calling interrupt vector table pins.
    attachInterrupt(digitalPinToInterrupt(interruptPin0), onnn, CHANGE);
    attachInterrupt(digitalPinToInterrupt(interruptPin1), offf, CHANGE);
}

void loop() {
    digitalWrite(ledPin, ledON);
    Serial.print("Count: ");
    Serial.print(cnt0);
    Serial.print("; Count: ");
    Serial.println(cnt1);
}
