# Gas Sensor

## MQ-2 sensor

### Introduction

The **MQ-2** Gas sensor module can detect **Smoke** and **Gas** leakage in home and industry. It is suitable for detecting **Hydogen**(H2), **LPG**, **Butane**, **Propane**, **Methane**, **Alcohol** and in used indoor at room temperature. Due to its fast response time and high sensitivity, measurements can be taken as soon as possible.

### Features

* Wide detection scope
* High sensitivity and fast response
* Long life and stable
* Simple drive circuit

### Application

They are useful in gas leakage detection and smoke.  

### Specification

##### Standard work condition

![Img: standard work condition](https://github.com/R-INYURU/SnHsecurity/blob/master/notes/img/MQ-2_standard_work_condition.png)  

##### Environment condition

![Img: environment condition](https://github.com/R-INYURU/SnHsecurity/blob/master/notes/img/MQ-2_environment_condition.png)  

##### Sensitivity characteristic

![Img: sensitivity characteristic](https://github.com/R-INYURU/SnHsecurity/blob/master/notes/img/MQ-2_sensitivity_characteristic.png)  

##### Structure and configuration

![Img: structure & configuration](https://github.com/R-INYURU/SnHsecurity/blob/master/notes/img/MQ-2_structure_n_configuratio,png)  

Structure and configuration of MQ-2 gas sensor is composed by micro AL<sub>2</sub>O<sub>3</sub> ceramic tube, Tin Dioxide (SnO<sub>2</sub>) sensitive layer, measuring electrode and heater are fixed into a crust made by plastic and stainless steel net.  
The heater provides necessary work conditions for work of sensitive components.  
The enveloped MQ-2 have 6 pin, 4 of them are used to fetch signals, and other 2 are used for providing heating current.  

### How it works

The **MQ-2** has an electrochemical sensor, which changes its resistance for different concentrations of varied gasses. The sensor is connected in series with a variable resistor to form a voltage divider circuit(*Img: 1*), and the variable resistor is used to change sensitivity.  
When one of the mentioned gaseous elements comes in contact with the sensor after heating, the sensor's resistance change. The change in the resistance changes the voltage across the sensor, and this voltage can be read by a microcontroller. The voltage value can be used to find the resistance of the sensor by knowing the reference voltage and the other resistor's resistance. The sensitivity characteristic curve(*Img: 2*) is shown for different type of gasses.  

[Img: 1]  
![Img: Voltage divider](https://github.com/R-INYURU/SnHsecurity/blob/master/notes/img/MQ-2_voltage_divider.jpg)  

[Img: 2]  
![Img: sensitivity](https://github.com/R-INYURU/SnHsecurity/blob/master/notes/img/MQ-2_sensitivity.png)  

where,  
1. Ro is the resistance of the sensor in clean air.
2. Rs is the resistance of sensor when exposed to gasses.

### Programming

#### Basic program

This program returns the voltage read from the sensor which is connected to pin `A0` of the arduino. The value returned can be used as a *threshold* to detect the increase/decrease in gas concetration.

```C++

void setup(){
	Serial.begin(115200);
}

void loop(){
	float sensor_volt;
	float sensorValue;
	
	sensorValue = analogRead(A0);
	sensor_volt = sensorValue / 1023.0 * 5.0;
	
	Serial.print("sensor volt: ");
	Serial.print(sensor_volt);
	Serial.println("V");
	
	delay(1000);
}
```

#### Callibration

The MQ-2 sensor needs to be calibrated be using to a specific gas. They are many variables needed to be considered when calibrating the sensor.  

##### Load-resistor

The sensor needs a load-resistor at the output to ground. It's value could be form 2K<sub>ohm</sub> to 47K<sub>ohm</sub>. The lower the value, the less sensitive. The higher the value, the less accurate for higher concentrations of gas.  
Choosing a good value for the load-resistor is only valid after the burn-in time.  

#### Logics

Because the MQ-2 need some burn-in time to start produce approximate data, it is better to calculate time passed since the sensor has been powered ON.  

