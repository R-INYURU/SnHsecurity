# BME280 Sensor

### Introduction

BME280 is a combined digital **humidity**, **pressure** and **temperature** sensor based on proven sensing priniples.  

The humidity sensor provides an extremely fast response time for fast context awareness applications and high overall accuracy over a wide temperature range.  

The pressure sensor is na absolute barometric pressure sensor with extremely high accuracy and resolution and drastically lower noise than Bosch Sensortec BMP180.  

The integrated temperature sensor has been optimized for lowest noise and highest resolution. Its outpur is used for temperature compensation of the pressure adnd humidity sensors and can also be used for estimation of the ambient temperature.  

The sensor provides both SPI and I<sup>2</sup>C interafaces and can be supplied using 1.71 to 3.6 V for the sensor supply V<sub>DD</sub> and 1.2 to 3.6 V for the interface supply V<sub>DDIO</sub>. Measurements can be triggered by the host fo performed in regular intervals. When the sensor is disabled, current consumption drops to 0.1 uA.  

### Sofware program

#### Library selection

I used an Arduino library called [BME280](https;//github.com/finitespace/BME280) by Tyler Glenn. The library compiles fine and I can upload it without any issue, one problem I have it can't measure the humidity. when I read the [datasheet](https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME280_DS001-12.pdf) it shows that the humidity is included in the sensor.  
I have also tried to use [BlueDot](https://github.com/BlueDot-Arduino/BlueDot_BME280) library and It couldn't see if BME280 is connected. I also tried the [Sparkfun BME280](https://github.com/sparkfun/SparkFun_BME280_Arduino_Library), this one it couldn't return the *temperature* and the *humidity* measured by the sensor. There is an [Adafruit](https://github.com/adafruit/Adafruit_BME280_Library) library, it can't detect the sensor connected to the MCU.  
In will conclude that in all 5 libraries I found for **BME280** only 1 library is able to return partiarly usefull data which I can use.  

#### Programming

I have written BME280 program using Arduino IDE for testing the sensor.

```C++
#include <BME280I2C.h>
#include <Wire.h>

BME280I2C BME;

void setup(){
	Serial.begin(115200);
	Wire.begin();
	
	if(!BME.begin()){
		Serial.println("Could not find BME280 sensor!");
		while(1);
	}
}

void loop(){
	BME280Data();
	delay(1000);
}

void printData(float val1, float val2){
	Serial.print("Temp: ");
	Serial.print(val1);
	Serial.print("*C\tHum: ");
	Serial.print(val2);
	Serial.print("% RH");
	Serial.println();
}

void BME280Data(){
	float Temperature(NAN), Humidity(NAN), Pressure(NAN);
	
	Temperature = BME.temp();
	Humidity = BME.hum();
	Pressure = BME.pres();
	
	return printData(Temperature, Humidity);
}
```

This program is working well and printing the value of *temperature* only since the library is not detecting the *humidity*.  
