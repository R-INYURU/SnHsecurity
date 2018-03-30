//  Receiver code.
#include <ESP8266WiFi.h>
#include <SPI.h>
#include "RF24.h"
#include "nRF24L01.h"

RF24 radio(2, 15);
WiFiClient client;

const char* ssid     = "LG hotspot";
const char* password = "192837465";

const char* host = "192.168.43.196";
const char* url = "/hssec/r/?";
const char* host2 = "192.168.43.51";
const char* url2 = "/?";
const int httpPort = 80;

uint64_t rAddresses[] = {
  0x7878787878LL,
  0xB3B4B5B6F1LL, // Tx & Rx addresses for gas and bme280
  0xB3B4B5B6CDLL, // Tx & Rx addresses for motion
  0xB3B4B5B6A3LL, // Tx & Rx addresses for door and window O/C detector
  0xB3B4B5B60FLL,
  0xB3B4B5B605LL
};

void setup() {
  yield();
  Serial.begin(115200);
  pinMode(18, OUTPUT);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password); //works!
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Started receiving.");

  radio.begin();
  radio.setChannel(120);

  radio.setPALevel(RF24_PA_MAX);
  radio.setRetries(15, 15);

  // Open a writing and reading pipe on each radio, with opposite addresses
  for (int i = 0; i < 6; i++) {
    radio.openReadingPipe(i, rAddresses[i]);
  }
  // Writing and Reading pipe for gas and bme280
  // Writing and Reading pipe for motion
  // Writing and Reading pipe for door and window O/C detector

  // Start the radio listening for data
  radio.startListening();

  Blink(1);
}

void loop() {
  yield();
  byte rxPipe = 0;  // Holds the pipe address of the received data
  char rxPyld[32];   // Holds the payload received
  char AckPyld[10] = "OK umusa";   //Acknowledge payload

  while (radio.available()) {  // Check to which payload we received payload
    radio.read( &rxPyld, 32);
    radio.stopListening();
    radio.openWritingPipe(rAddresses[rxPipe]);  // Open pipe from which we have received
    radio.write(&AckPyld, sizeof(AckPyld));
    radio.startListening();
    Serial.println(rxPyld);

    if (!client.connect(host, httpPort)) { //works!
      Serial.println("connection failed");
      return;
    }
    else {
      client.print(String("GET ") + url + rxPyld + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Connection: close\r\n\r\n");
      client.print(String("GET ") + url2 + rxPyld +" HTTP/1.1\r\n" +
                   "Host: " + host2 + "\r\n" +
                   "Connection: close\r\n\r\n");
      Blink(2);
    }

  }
} // Loop

void Blink(int num) {

  if (num > 1)
    num = 1;

  for (int i = 0; i < num; i++) {
    digitalWrite(18, LOW);
    delay(500);
    digitalWrite(18, HIGH);
    delay(500);
  }
}

