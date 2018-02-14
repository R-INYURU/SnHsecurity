# Power Management and Sleep modes in AVR(ATmega328p)

## Overview  
Sleep modes enable the application to shutdown unusedmodules in the MCU, thereby saving power.The device provides various sleep modes allowing the user to tailor the power consumption to the application requirements.

## Sleep Modes  
The picture below shows the different sleep modes, BOD disable ability and their wake-up sources.  
![Img: Different sleep modes](http://xtronix.in/blog/wp-content/uploads/2014/08/avr-sleep-modes.png)  

#### Idle Mode
This mode stops the CPU but allows the SPI, USART, Analog Comparator, 2-wire Serial Interface, Timer/Counter, Watchdog, and Interrupt system to continue operating. Basically Basically idle mode halts CLK _cpu_ and CLK _flash_ while other continues to run.  

This mode enables the MCU to wake up from external triggered interrupts as well as internal ones like the Timer Overflow and USART Transmit Complete interrupts.  
If waking up from the Analog Comparator interrupt is not required, the Analog Comparator can be powered down by setting the `ACD` bit in the Analog Comparator Control and Status Register - `ACSR`. This will reduce power consumption in idle mode.  

When the `SM[2:0]` bits are written to `000`, the SLEEP instruction makes the MCU enter idle mode.  

#### ADC Noise Reduction Mode
In this mode CPU is stoped but ADC, External Interrupts, 2-wire Serial Interface address watch, Timer/Counter2, and Watchdog continues to operate. Basically CLK _i/o_, CLK _cpu_, and CLK _flash_ are halted while others continue to run.  
This mode improves the noise environment for the ADC, enabling higher resolution measurements. If the ADC is enabled, a convesion starts automaticaly when this mode is entered.  

Apart from the ADC Convesion Complete interrup, only these events can wake up the MCU from ADC Noise Reduction mode:
* External Reset
* Watchdog System Reset
* Watchdog Interrupt
* Brown-out Reset
* 2-wire Serial Interface address match
* Timer/Counter2 interrupt
* SPM/EEPROM ready interrupt
* Pin change interrupt

When the `SM[2:0]` bits are written to `001`, the SLEEP instruction makes the MCU  enter ADC Noise Reduction mode.  

**Note:** Timer/Counter2 will only keep running in asynchronous mode.

#### Power-Down Mode

In this mode the External Oscillator is stopped whill External interrupts, 2-wire Serial interface address watch, and the Watchdog continues to operate. Basically all generated clocks are halted, allows operation of asynchronous modules only.  

Only one of these events can wake up the MCU:
* Extrenal Reset
* Watchdog System Reset
* Watchdog Interrupt
* Brown-out Reset
* 2-wire Serial Interface address match
* External level interrupt on INT
* Pin change interrupt

When the `SM[2:0]` bits are written to `010`, the SLEEP instruction makes the MCU enter in Power-Down mode.  

**Note:** `if` a level triggered interrupt is used for wake-up from Power-Down, the required level must be held long enough for the MCU to complete the wake-up and trigger the level interrupt. `else` the MCU will still wake up, but no interrupt will be generated.  
>The start-up time is defined by the SUT and CKSEL Fuses.

#### Power-save Mode

This mode is identical to Power-down, with one exception:  

If Timer/Counter2 is enabled, it will keep running during sleep. The device can wake up from either Timer Overflow or Output Compare event from Timer/Counter2.

If Timer/Counter2 is not running, Power-down mode is recommended instead of Power-save mode.

The Timer/Counter2 can be clocked both synchronously and asynchronously in Powe-save mode.  

When the `SM[2:0]` bits are written to `011`, the SLEEP instruction makes the MCU enter Power-save mode.

#### Standby Mode

This mode is identical to Power-down with the exception that the oscillator is kept running. From the Standby mode, the device wakes up in six clock cyles.

When the `SM[2:0]` bits are written to `110` and the external crystal/resonator clock option is selected, the SLEEP instruction makes the MCU entr Standby mode.

#### Extended Standby Mode

This mode is identical to Power-save mode with the exception that the oscillator is kept running. From Extended Standby mode, the device wakes up in six clock cycles.

When the `SM[2:0]` bits are written `111` and the external crystal/resonator clock option is selected, the SLEEP instruction make the MCU enter Extended Standby mode.

#### Power Reduction Register

The Power Reduction Register(PRR) provides a method to stop the clock to individual peripherals to reduce power consumption.  

The current state of the peripheral is frozen and the I/O registers can not be read or written. Resources used by the peripheral when stopping the clock will remain occupied, hence the peripheral should in most cases be disabled before stopping the clock.  
Waking up a module, which is done by clearing the corresponding bit in the PRR, puts the module in the same state as before shutdown.  

Module shutdown can be used in Idle mode and Active mode to significantly reduce the overall power consumption. In all other sleep modes, the clock is already stopped.

### Minimizing Power Consumption

There are several possibilities to consider when trying to minimize the power consumption in an AVR controlled system.  

In general, sleep modes should be used as much as possible, and the sleep mode should be selected so that as few as possible of the device's functions are operating. *All function not needed should be disabled.*  

The following module may need special consideration when trying to achieve the lowest possible power consumption.

##### Analog to Digital Converter

If enabled, the ADC will be enabled in all sleep modes. To save power the ADC should be disabled before entering any sleep mode.  
When the ADC is turned *off* and *on* again, the next conversion will be an extended conversion.  

##### Analog Comparator

When entering idle mode, the Analog Comparator should be disabled if not used.  
When entering ADC Noise Reduction mode, the Analog Comparator should be disabled.  
In other sleep modes, the Analog Comparator is automatically disabled. However, if the Analog Comparator is set up to use the internal Voltage Reference as input, theAnalog Comparator should be disabled in all sleep modes.  

##### Brown-Out Detector

If the Brown-Out Detector(BOD) in not needed by the application, this module should be turned off.  
If the `BOD` is enabled by the `BODLEVEL Fuses`, it will be enabled in all sleep modes, and hence, always consume power.  

##### Internal Voltage Reference

The internal Voltage Reference will be enabled when needed by *the Brown-Out Detection*, *the Analog Comparator* or *the Analog-to-Digital Converter*.  
If these module are disabled the internal voltage will be disabled and it will not consume power. When turned on again, the user must allow the *reference* to start up before the output is used. If the *reference* was kept on in sleep mode, the output can be used immediately.  

##### Watchdog Timer

If the Watchdog Timer is not needed in the application, the module should be turned off.  
If the Watchdog Timer is enabled, it will be enabled in all sleep modes and hence always consume power.  

##### Port Pins

When entering a sleep mode, all port pins should be configured to use minimum power. The most important is to ensure no pins drives resistive loads.  
In sleep modes the input buffers of the device will be disabled, this ensures that no power is consumed by the input logic whe not needed.  
In some cases, the input logic is neede for detecting wake-up conditions, and it will then be enabled.  

For the analog input pin, the digital input buffer should be disabled at all times.  
An analog signal level close to Vcc/2 on an input pin can cause significant current even in active mode.  
Digital input buffers can be disabled by writing to the Digital Input Disable Registers(`DIDR0` for `ADC`, `DIDR1` for `AC`).  

##### On-chip Debug System

If the On-chip debug system is enabled by the Fuse and the chip enters sleep mode, the main clock source is enabled and hence always consumes power.  


