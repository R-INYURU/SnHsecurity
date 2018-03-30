# NRF24 ATtiny Tx

## Program

I am using the arduino IDE to program the **ATtiny44 MCU** and **Arduino Nano**, and the nRF24 library for transmitting and receiving through nRF24L01+ module. In the arduino IDE am using ATtiny addon board from [Damellis](http://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json).  

My connection are as follow:  

**Arduino Nano**  
| No | nRF24L01+ | Arduino Nano |
|----|-----------|--------------|
| 1  | 3.3v      | 3.3v         |
| 2  | GND       | GND          |
| 3  | CE        | D7           |
| 4  | CSN       | D8           |
| 5  | MOSI      | D11          |
| 6  | SCK       | D13          |
| 7  | MISO      | D12          |
| 8  | IRQ       | NC           |

**ATtiny44 MCU**  
| No | nRF24L01+ | ATtiny44 
-----|-----------|----------
| 1  | VCC  	  | VCC(3.3V)  
| 2  | GND  	  | GND        
| 3  | CE   	  | PA3(pin 3) 
| 4  | CSN  	  | PA2(pin 2) 
| 5  | MOSI 	  | PA6(pin 6) 
| 6  | SCK  	  | PA4(Pin 4) 
| 7  | MISO 	  | PA5(pin 5) 
| 8  | IRQ 		  | NC

On the Receiver side I am using an **Arduino Nano**. The following is the source code for Receiver:  

```C++
#include <SPI.h>
#include "nRF24.h"
#include "nRF24L01.h"

RF24 radio(7,8);

byte addresses[][6] = {"1Node", "2Node"};

void setup(){
	Serial.begin(115200);
	Serial.println("Started receiving");
	
	radio.begin();
	radio.setPALevel(RF24_PA_LOW);
	
	radio.openWritingPipe(addresses[0]);
	radio.openReadingPipe(1, addresses[1]);
	
	radio.startListening();
}

void loop(){
	char rxMsg[32];
	char AckMsg[10] = "ok umusa";
	
	if( radio.available()){
		while(radio.available())
			radio.read(&rxMsg, sizeof(rxMsg));
			
		radio.stopListening();
		radio.write(&AckMsg, sizeof(AckMsg));
		radio.startListening();
		Serial.print(F("Received Msg: "));
		Serial.println(rxMsg);
	}
}
```
The above have been verified using another Arduino Nano as a transmitter and it did work fine.  

Now the following program is for the transmitter(**ATtiny44**):  
```C++
#include "RF24.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 9) // RX: 10 - TX: 9
RF24 radio(3, 2);

byte addresses[][6] = {"1Node", "2Node"};
uint8_t led1 = 1;
uint8_t led2 = 0;

void setup(){
	mySerial.begin(9600);
	mySerial.println("Started Tx");
	
	radio.begin();
	radio.setPALevel(RF24_PA_MAX);
	radio.setRetries(15, 15);
	
	pinMode(led2, OUTPUT); // timeout indicator
	pinMode(led1, OUTPUT); // failed tx indicator
	
	radio.openWritingPipe(addresses[1]);
	radio.openReadingPipe(1, addresses[0]);
	
	radio.startListening();
}

void loop(){
	radio.stopListening();
	
	char txMsg[32] = "hello man";
	
	if(!radio.write(&txMsg, sizeof(txMsg))){
		mySerial.println(F("failed to Tx"));
		digitalWrite(led1, HIGH); // Transmission failed
	}
	
	unsigned long started_at = micros();
	boolean timeout = false;
	while(!radio.available()){
		if(micros - started_at > 300000){
			timeout = true;
			break;	// troubleshoot[1]
		}
	}
	
	if(timeout){
		mySerial.println(F("Timeout"));
		digitalWrite(led2, HIGH); // Acknowledge response timed-out
	}
	else{
		char rxMsg [20];
		radio.read(&rxMsg, sizeof(rxMsg));
	}
	
	delay(500);
	digitalWrite(led1, LOW);
	digitalWrite(led2, LOW);
	mySerial.println(F("Hello));
}
```

## Troubleshooting

On the transmitter(TX) side compilling and uploading is fine but the receiver(RX) is not receiving anything.

I have tried to use the same program of the TX on an Arduino Nano, there was also a problem the TX was able to transmit one time only after it would hang-p. So I passed through the program to see if there is no mistake and I found out that the program was hanginig in the `while(!radio.available())` loop, I had forget to `break` the loop after checking for the *timeout*. Now I have corrected the above program and marked it as ***troubleshoot[1]***.  

I have change back to ATtiny44 on the TX and uploaded the same code as the one I have used in the **Nano** but nothing is showing up. None of the led I have used as indicators is turning ON, this is supposed to mean that the TX is transmitting fine and getting back the Acknoledge byte from the RX, but when I open the Serial monitor on the receiver to see if there is any thing is receiving nothing is showing up.
when I open the Serial monitor for the ATtiny44 it is printing '*hello*' fine.  

Something I forgot to mention is that I had to change pins of `ce` and `csn` on the TX when I used the Arduino Nano to `7` and `8` like this `RF24 radio(7, 8);`. After test the TX on Arduino Nano I forgot to change back the pins to `3` and `2`, I did it but nothing has been received.  

### 19/02/2018

Today I used the [ATTinyCore board](https://github.com/SpenceKonde/ATTinyCore) to see if it can work properly with the RF24 library I using.  
I had to remove the `SoftwareSerial` because it was occuping too much space of the chip. So far nothing has been received on the RX but the *indicator LEDs* are showing that the data are being received.  

