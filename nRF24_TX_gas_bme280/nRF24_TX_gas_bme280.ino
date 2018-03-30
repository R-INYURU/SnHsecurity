#include<math.h>
#include<BME280I2C.h>
#include<Wire.h>
#include<RF24.h>
#include<nRF24L01.h>
#include<SPI.h>

#define POINTS  9

BME280I2C BME;
RF24 radio(7,8);

uint64_t address = 0xB3B4B5B6F1LL;

float Ro = 98800, Vref = 5.0, R1 = 7700;
// Marked points on MQ-2 curve for LPG
float ltx[POINTS] = {200, 500, 800, 1000, 1600, 2000, 3000, 5000, 10000};
float lty[POINTS] = {1.67, 1.11, 0.88, 0.78, 0.64, 0.56, 0.46, 0.36, 0.26};
//loop control variables
int counter1, counter2;
int temp;
int rawAnalogValue[100]; 

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if(!BME.begin()){
//    Serial.println("Could not find BME280 sensor!");
    while(1);
  }

  radio.begin();
  radio.setChannel(120);
  radio.setAutoAck(1);
  radio.setPALevel(RF24_PA_MAX);
  radio.setRetries(15, 15);
  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(address);
  radio.openReadingPipe(1, address);
  // Start the radio listening for data
  radio.startListening();
}

void loop() {
  
  BME280_data();
  gas_concetration();

//  delay(1000);
}

void TX_data(char *payload, char *type){
  unsigned long started_at_time = micros();
  boolean timeout = false;

  radio.stopListening();
  char txPyld[32];
  
  strcpy(txPyld, "d=2&v=");
  strcat(txPyld, payload);
  strcat(txPyld, "&t=");
  strcat(txPyld, type);
  
//  Serial.print("Paylaod: ");
//  Serial.print(txPyld);
//  Serial.print(" | Size is: ");
//  Serial.println(sizeof(txPyld));
  
  if (radio.write(&txPyld, sizeof(txPyld))) {
//    Serial.println(F("Tx success"));

  }
  else {
//    Serial.println(F("Tx failed")); 
    // Resend the failed data
    return;
  }
  
  radio.startListening();
  while (!radio.available()) {
    if (micros() - started_at_time > 300000 ) {
      timeout = true;
      break;
    }
  }

  if ( timeout ) {
    Serial.println(F("Response failed, timed out."));
  }
  else {
    char AckPyld[10]; // Store received message
    radio.read( &AckPyld, sizeof(AckPyld) );
    Serial.print("Ack msg: ");
    Serial.println(AckPyld);
  }
}
///////////////////////////////////////////////////////////
void BME280_data(){
  float Temperature(NAN), Humidity(NAN), Pressure(NAN);
  char TempStr[6], HumStr[6], PresStr[10];
  
  Temperature = BME.temp();
  dtostrf(Temperature, 3, 2, TempStr);
  TX_data(TempStr, "6");
  delay(1000);
//  Serial.print("Temp: ");
//  Serial.print(TempStr);
//  Serial.println("°C");
  Humidity = BME.hum();
  dtostrf(Humidity, 3, 2, HumStr);
//  TX_data(HumStr, "8");
//  delay(1000);
//  Serial.print("Hum: ");  
//  Serial.print(Humidity);
//  Serial.println("% RH");
  Pressure = BME.pres();
  dtostrf(Pressure, 7, 2, PresStr);
  TX_data(PresStr, "7");
  delay(1000);
//  Serial.print("Pres: ");
//  Serial.print(PresStr);
//  Serial.println(" pa");
  
}
///////////////////////////////////////////////////////////
void gas_concetration(){
  char concentrationStr[10];
  float gasConcentration, Vs, Rs, slope, Y, Yo, Xo;
  // read 100 Values from analog Port and store it in raw array
  for(counter1=0; counter1<100; counter1++){
    rawAnalogValue[counter1]=analogRead(A0);      
  }
  // Swap the data and find median value
  for(counter1=0; counter1<100; counter1++){      
    for(counter2=0; counter2<100-counter1-1; counter2++){      
        if(rawAnalogValue[counter2]<rawAnalogValue[counter2+1]){
          temp=rawAnalogValue[counter2];      
          rawAnalogValue[counter2]=rawAnalogValue[counter2+1];      
          rawAnalogValue[counter2+1]=temp;      
        }
    }
  }
  //The middle value of the sorted values gives median
  Vs = Vref - rawAnalogValue[50] * Vref / 1024.0;
  //Finding the resistance of the sensor
  Rs = (R1 * Vs) / (Vref - Vs);
  //Finding the Rs/Ro Ratio
  Y = Rs / Ro;
  //Finding the slope and Initial Values using the ratio
  slope = find_slope(Y);
  Xo = find_Xo(Y);
  Yo = find_Yo(Y);
  //Finding the concentration and Limit the Output to MQ2's range
  gasConcentration = Xo * pow(Y / Yo, slope);
  if (gasConcentration > 10000.00) {
    gasConcentration = 10000.00;
  }
//  else if (gasConcentration < 200.00) {
//    gasConcentration = 200.00;
//  }
  dtostrf(gasConcentration, 6, 2, concentrationStr);
  TX_data(concentrationStr, "2");
  delay(1000);
//  Serial.print("Gas concentration: ");
//  Serial.println(gasConcentration);
}
///////////////////////////////////////////////////////////
float find_slope(float y) {
  for (counter1 = 0; counter1 < POINTS-1; counter1++) {
    if (y < lty[counter1] && y > lty[counter1 + 1]) {
      break;
    }
  }
  if (y > lty[0]) {
    counter1=0;
  }
  if (counter1 >= POINTS-1) {
    counter1 = POINTS-2;
  }
  return log(lty[counter1 + 1] / lty[counter1]) / log(ltx[counter1 + 1] / ltx[counter1]);
}
///////////////////////////////////////////////////////////
float find_Xo(float y) {
  for (counter1 = 0; counter1 < POINTS-1; counter1++) {
    if (y < lty[counter1] && y > lty[counter1 + 1]) {
      break;
    }
  }
  if (y > lty[0]) {
    counter1=0;
  }
  if (counter1 >= POINTS-1) {
    counter1 = POINTS-2;
  }
  return ltx[counter1];
}
///////////////////////////////////////////////////////////
float find_Yo(float y) {
  for (counter1 = 0; counter1 < POINTS-1; counter1++) {
    if (y < lty[counter1] && y > lty[counter1 + 1]) {
      break;
    }
  } 
  if (y > lty[0]) {
    counter1=0;
  }
  if (counter1 >= POINTS-1) {
    counter1 = POINTS-2;
  }
  return lty[counter1];
}
