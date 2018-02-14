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

### How it works

The **MQ-2** has an electrochemical sensor, which changes its resistance for different concentrations of varied gasses. The sensor is connected in series with a variable resistor to form a voltage divider circuit(*Img: 1*), and the variable resistor is used to change sensitivity.  
When one of the mentioned gaseous elements comes in contact with the sensor after heating, the sensor's resistance change. The change in the resistance changes the voltage across the sensor, and this voltage can be read by a microcontroller. The voltage value can be used to find the resistance of the sensor by knowing the reference voltage and the other resistor's resistance. The sensitivity characteristic curve(*Img: 2*) is shown for different type of gasses.  

[Img: 1]  
![Img: Voltage diveder](https://plaground.arduino.cc/uploads/Main/alchoolau5.jpg)  

[Img: 2]  
![Img: sensitivity](https://cdn.instructables.com/FQP/LBGN/IQ8SBBFD/FQPLBGNIQ8SBBFD.MEDIUM.jpg?width=614)  


