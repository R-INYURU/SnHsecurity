// Door/Window O&C detector
#include <SPI.h>
#include <RF24.h>
RF24 radio(7, 8);

uint64_t address = 0xB3B4B5B6A3LL;

int reedPin = 2;
int ledPin = 9;
bool pinState = false;
unsigned long start_time = 0;

void setup() {
  Serial.begin(115200);
  Serial.println(F("Ready to send"));
  pinMode(reedPin, INPUT);
  pinMode(ledPin, OUTPUT);

  radio.begin();
  radio.setChannel(120);

  radio.setPALevel(RF24_PA_MAX);
  radio.setRetries(15, 15);

  radio.openWritingPipe(address);
  radio.openReadingPipe(1, address);

  radio.startListening();
  
  unsigned long sysInit = 0;

  //device initializing
  while(micros() - sysInit < 30000000){
    Serial.print(".");
  }
  Serial.println();
}

void loop() {
  radio.stopListening();

  if((digitalRead(reedPin) == HIGH) && (pinState == false)){
    pinState = true;
    digitalWrite(ledPin, HIGH);
    Serial.println("-----===>>Door Opened");
    Tx_data("0", "3");
    calc_time_spent(true);
  }

  else if ((digitalRead(reedPin) == LOW) && (pinState == true)){
    pinState = false;
    digitalWrite(ledPin, LOW);
    Serial.println("<<===-----Door closed");
    Tx_data("1", "3");
    calc_time_spent(false);
  }
}
///////////////////////////////////////////////////////////////////
void Tx_data(char *payload, char *type) {
  bool timeout = false;

  char txPyld[32];

  strcpy(txPyld, "d=5&v=");
  strcat(txPyld, payload);
  strcat(txPyld, "&t=");
  strcat(txPyld, type);
  
  Serial.print("Paylaod: ");
  Serial.println(txPyld);

  if (radio.write(&txPyld, sizeof(txPyld))) {
    Serial.println(F("Transmission success"));
  }
  
  else {
    Serial.println(F("Transmission failed"));
    
    return;
  }

  radio.startListening();
  
  unsigned long started_at_time = micros();
  
  while (!radio.available()) {
    if (micros() - started_at_time > 300000 ) {
      timeout = true;
      break;
    }
  }

  if ( timeout ) {
    Serial.println("Response failed, timed out.");
  }
  else {
    char AckPyld[20]; // Store received message
    radio.read( &AckPyld, sizeof(AckPyld) );
    Serial.println(AckPyld);
  }
}
/////////////////////////////////////////////////////////////////////
void calc_time_spent(bool count) {

  if (count == true) { // start count when object is detected
    start_time = micros();
  }
  else { // calculate the total time spent.
    unsigned long end_time = micros();
    
    Serial.print(F("Object spent "));
    unsigned long spent_time = end_time - start_time;
    char Time[15];
    sprintf(Time, "%lu", spent_time);
    Serial.print(spent_time);
    Serial.println(F(" microseconds"));
    Serial.println(Time);
//    Tx_data(Time, "3");
  }
}
