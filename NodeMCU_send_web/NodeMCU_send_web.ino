#include <ESP8266WiFi.h>
// Use WiFiClient class to create TCP connections
WiFiClient client;

const char* ssid     = "FabLab 2.4Ghz";   // change to your USSID
const char* password = "innovate";        // change to your password

const char* host = "api.thingspeak.com";
const char* url = "/update?api_key=APIkey&";  // change [APIkey] to your API key from thingspeak
const int httpPort = 80;

boolean SerialRx = false;

void setup() {
  Serial.begin(115200);

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
}

void loop() {
  if (Serial.available()) {

    if (!client.connect(host, httpPort)) { //works!
      Serial.println("connection failed");
      return;
    }

    String rxData = Serial.readString();
    Serial.print("Requesting URL: ");
    Serial.print(url);
    Serial.println(rxData);

    // This will send the request to the server
    client.print(String("GET ") + url + rxData + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

    Serial.print(client.read());

    Serial.println();
    Serial.println("closing connection");
  }
}
