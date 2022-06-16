const byte ledPin = LED_BUILTIN;
const byte interruptPin = 2;

volatile byte state = LOW;      // to avoid register optimization
volatile unsigned int cnt = 0;  // Use of volatiles

// Interrupt Service Routine (ISR)
void blink() {
    cnt++;  // interrupt is disabled when running this code
    digitalWrite(ledPin, HIGH);
}
void setup() {
    pinMode(ledPin, OUTPUT);              // To update LED status
    pinMode(interruptPin, INPUT_PULLUP);  // Internal pull-up resistor Arduino docs/digital-pins

    Serial.begin(115200);

    attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);  // Attach interrupt handler
}
void loop() {
    digitalWrite(ledPin, state);  // Change LED status
    Serial.println(cnt);
}
