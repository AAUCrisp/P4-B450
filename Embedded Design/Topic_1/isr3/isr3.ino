const byte ledPin = LED_BUILTIN;
const byte interruptPin = 2;

volatile byte state = LOW;  // to avoid register optimization

void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(2, INPUT_PULLUP);

    // Configure external interrupt 0 to generate interrupt request
    // on the falling edge of INT0(from the vector table), using the
    // External Interrupt Control Register(EICRA)
    EICRA = 0x02;  // set falling edge

    // Enables external interrupt 0 using the
    // External Interrupt Mask Register(EIMSK)
    EIMSK |= 0x01;  // enable ISR 0
                    // FOR ARDUINO MEGA YOU CAN USE INT0 .. INT5
                    // INT6 and INT7 is not connected to the pins and is only om the cpu
                    // EIMSK so INT7 is bit 7 in EIMSK etc

    // intr falling,rising etc,
    // EICRA is or INT0 -> INT3
    // EICRB is for INT4-INT7
    // https://ww1.microchip.com/downloads/en/devicedoc/atmel-2549-8-bit-avr-microcontroller-atmega640-1280-1281-2560-2561_datasheet.pdf
    // EICRA, EICRB on p 110,111
    // EIMSK on p 111
}

// ISR(INT1_vect) etc

void loop() {
    digitalWrite(ledPin, state);
}

volatile char xx = 0;

// Interrupt Service Routine (ISR) + specification of pin D2 using vector table for interrupt.
ISR(INT0_vect) {
    xx++;
}

/*
   volatile char xx = 0;

 ISR for pin D2 which corresponds to INT0_vect in the vector table.
ISR(INT0_vect)
{
 174:	1f 92       	push	r1
 176:	0f 92       	push	r0
 178:	0f b6       	in	r0, 0x3f	; 63
 17a:	0f 92       	push	r0
 17c:	11 24       	eor	r1, r1
 17e:	8f 93       	push	r24
 xx++;
 180:	80 91 00 01 	lds	r24, 0x0100	; 0x800100 <_edata>
 184:	8f 5f       	subi	r24, 0xFF	; 255
 186:	80 93 00 01 	sts	0x0100, r24	; 0x800100 <_edata>
}
 18a:	8f 91       	pop	r24
 18c:	0f 90       	pop	r0
 18e:	0f be       	out	0x3f, r0	; 63
 190:	0f 90       	pop	r0
 192:	1f 90       	pop	r1
 194:	18 95       	reti

  }*/
