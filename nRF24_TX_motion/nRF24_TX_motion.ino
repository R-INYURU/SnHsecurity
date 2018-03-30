// Transmitter code.
#include <SPI.h>
#include "RF24.h"
RF24 radio(7, 8);

uint64_t address  = 0xB3B4B5B6CDLL;

int sensorPin = 3;
//int ledPin = 9;
bool pinState = false;
unsigned long start_time = 0;
int fileFolder = 100;

void setup() {
  Serial.begin(115200);
  Serial.println(F("Ready for sending"));
  pinMode(sensorPin, INPUT);
//  pinMode(ledPin, OUTPUT);
  
  radio.begin();
  radio.setChannel(120);

  radio.setPALevel(RF24_PA_MAX);
  radio.setRetries(15, 15);

  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(address);
  radio.openReadingPipe(0, address);

  // Start the radio listening for data
  radio.startListening();

  //device initializing
  unsigned long sysInit = 0;
  while(micros() - sysInit < 30000000){
    Serial.print('.');
  }
  Serial.println();
}

void loop() {

  radio.stopListening();

  if ((digitalRead(sensorPin) == HIGH) && (pinState == false)) {    //send msg when object appeares

//    digitalWrite(ledPin, HIGH);
    Serial.println(">>>>>>>>>>Object detected");
    pinState = true;
    calc_time_spent(true);    // activate the count object is detected
    fileFolder ++;
    char Time[5];
    sprintf(Time, "%d", fileFolder);
    Tx_data("1", "1", Time);
  }

  else if ((digitalRead(sensorPin)) == LOW && (pinState == true)) {   //send a msg when object disappeares

//    digitalWrite(ledPin, LOW);
    Serial.println("<<<<<<<<<<Object disappered");
    Tx_data("0", "1", NULL);
    calc_time_spent(false);   // print the total time the object spent.
    pinState = false;
  }

}
///////////////////////////////////////////////////////////////////
void Tx_data(char *payload, char *type, char *folderName) {
  bool timeout = false;

  char txPyld[32];

  strcpy(txPyld, "d=5&v=");
  strcat(txPyld, payload);
  strcat(txPyld, "&t=");
  strcat(txPyld, type);
  strcat(txPyld, "&i=");
  strcat(txPyld, folderName);
  
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
    unsigned long spent_time = (end_time - start_time) - 2500000;
    char Time[15];
    sprintf(Time, "%lu", spent_time);
    Serial.print(spent_time);
    Serial.println(F(" microseconds"));
    Serial.println(Time);
//    Tx_data(Time, "2");
  }
}

