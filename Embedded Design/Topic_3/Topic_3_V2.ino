const char STARTBYTE = 0x2;
const char STOPBYTE = 0x3;
const char ESCAPE = 0x10;
unsigned char Data[1024];
void setup() {
  Serial.begin(9600);
  char arr[5] = {0x30,0x30,0x10,0x41,0x00};

  unsigned char *frame = CreateFrame(arr, strlen(arr));

  Serial.print("Encoded Frame: ");
  for (int i= 0; i < 10; i++) {
    Serial.print(frame[i], HEX); Serial.print(" ");
  }

  Serial.println("");
  
  DecodeFrame(frame);
}

unsigned char* CreateFrame(unsigned char* Frame, int len){
  
  int index = 2;
  int CRC = 0;
  Data[0] = STARTBYTE;
  Data[1] = len;
  while(*Frame != NULL) {
    if(*Frame == STARTBYTE || *Frame == STOPBYTE || *Frame == ESCAPE ){
      Data[index] = ESCAPE;
      index = index + 1; 
      Data[index] = *Frame + ESCAPE;
    }else {
      Data[index] = *Frame;
    }
    index = index + 1; 
    CRC += *Frame; 
    Frame++; 
  }
  Data[index] = CRC;
  Data[index + 1] = CRC>>8; 
  Data[index + 2] = STOPBYTE;

  Data[index+3] = NULL;
  return Data;  
}

typedef enum {
  start,
  lenght, 
  data,
  checksum,
  Stop,
  done, 
} states;


void DecodeFrame(unsigned char* Frame){
    int state = start;
    unsigned char output[1024];
    int index = 0;
    int len = 0;
    int CRC2 = 0;
    int Checksum = 0;
    
    while (state != done){
      
    
    switch (state){
      case start:
           index = 0;
           if (*Frame == STARTBYTE) {
            state = lenght; 
           }
           break;
      case lenght:
           len = *Frame;
           Serial.print("Length of data: "); Serial.println(len);
           state = data;
           break;
      case data:
           if (*Frame == ESCAPE){
            Frame++;
            output[index] = *Frame - ESCAPE;
           } else {
            output[index] = *Frame;
           }
           CRC2 += output[index];
           index++;
           if (index == len) {
            state = checksum;
           }
           break;
     case checksum:
          Checksum =  (Frame[1]<<8) + Frame[0];
          Frame++;
          state = Stop;
          break;
     case Stop:
          if (*Frame == STOPBYTE){
            Serial.print("Decoded data: ");
             for (int i= 0; i < len; i++) {
                Serial.print(output[i], HEX); Serial.print(" ");
              }
          Serial.println();
          CRC2 == Checksum ? Serial.print("CRC: OK"):Serial.print("CRC: Not OK");
          
          }
          state = done;
          break;
    }

    ++Frame;
    }
}



void loop() {
  
}
