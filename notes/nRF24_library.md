# [nRF24](https://www.nordicsemi.com/files/Product/data_sheet/nRF24L01_Product_Specification_v2_0.pdf) [Library](https://www.github.com/maniacbug/RF24/) 
## Design Goals 

* Maximally compliant with the intended operation of the chip 
* Easy for beginners to use.  
* Consumed with a public interface that's similir to other Arduino standard libraries.

## RF24 Class Member List

### Public member function

##### Primary public interface

These are the main methods you need to operate the chip.  

[`RF24(uint8_t _cepin, uint8_t _cspin)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Constructor.  
[`void begin(void)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Begin operation of the chip.  
[`void startListening(void)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Start listening for incoming messages.  
[`void stopListening(void)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Stop listening for incoming messages.  
[`void write(const void *buf, uint8_t len)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Write to the open writing pipe.  
[`void available(void)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Test whether there are bytes available to be read.  
[`bool read(void *buf, uint8_t len)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Read the payload.  
[`void openWritingPipe(uint64_t address)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Open a pipe for writing.  
[`void openReadingPipe(uint8_t number, uint64_t address)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Open a pipe for reading.  

##### Optional configurators

Methods you can use to get or set the configuration of the chip.  

[`void setRetries(uint8_t delay, uint8_t count)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Set the number and delay of retries upon failed submit.  
[`void setChannel(uint8_t channel)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Set RF communication channel.  
[`void setPayloadSize(uint8_t size)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Set static payload size.  
[`uint8_t getPayloadSize(void)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Get static payload size.  
[`uint8_t getDynamicPayloadSize(void)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Get dynamic payload size.  
[`void enableAckPaylaod(void)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Enable custom payloads on the acknowledge packets.  
[`void enableDynamicPayloads(void)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Enable dynamically-sized payloads.  
[`bool isPVariant(void)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Determine whether the hardware is an nRF24L01+ or not.  
[`void setAutoAck(bool enable)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Enable or disable auto-acknowledge packets.  
[`void setAutoAck(uint8_t pipe,bool enable)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Enable auto-acknowledge packets on a per pipeline basis.  
[`void setPALevel(rf24_pa_dbm_e level)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Set power amplifier(PA) level to one of four levels.  
[`rf24_pa_dbm_e getPALevel(void)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Fetches the current PA level.  
[`bool setDataRate(rf24_datarate_e speed)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Set the transmission data rate.  
[`rf24_datarate_e getDataRate(void)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Fetches the transmission data rate.  
[`void setCRCLength(rf24_crclength_e length)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Set the CRC length.  
[`rf24_crclength_e getCRCLength(void)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Get the CRC length.  
[`void disableCRC(void)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Disable CRC validation.  

##### Advanced operation

Methods you can use to drive the chip in more advanced ways.  

[`void printDetails(void)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Print a giant block of debugging information to **stdout**.  
[`void powerDown(void)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Enter low-power mode.  
[`void powerUp(void)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Leave low-power mode - making radio more responsive.  
[`bool available(uint8_t *pipe_num)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Test whether there are bytes available to be read.  
[`void startWrite(const void *buf, uint8_t len)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Non-blocking write to the open writing pipe.  
[`void writeAckPayload(uint8_t pipe, const void *buf, uint8_t len)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Write an ack payload for the specified pipe.  
[`bool isAckPayloadAvailable(void)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Determine if an ack payload was received in the most recent call to `write()`.  
[`void whatHappened(bool &tx_ok, bool &tx_fail, bool &rx_ready)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Call this when you get an interrupt to find out why.  
[`bool testCarrier(void)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Test whether there was a carrier on the line for the previous listening period.  
[`bool testRPD(void)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Test whether a signal (carrier or otherwise) greater than or equal to -64dBm is present on the channel.  

### Protected Member Functions  

##### Low-level internal interface.  

Protected methods that address the chip directly (not for regular users).  

[`void csn(int mode)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Set chip select pin.  
[`void ce(int level)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Set chip enable.  
[`uint8_t read_register(uint8_t reg, uint8_t *buf, uint8_t len)`][1] [**protected**]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Read a chunk of data in from a register.  
[`uint8_t read_register(uint8_t reg)`][1] [**protected**]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Read single byte from a register.  
[`uint8_t write_register(uint8_t reg, const uint8_t *buf, uint8_t len)`][1] [**protected**]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Write a chunk of data to a register.  
[`uint8_t write_register(uint8_t reg, uint8_t value)`][1] [**protected**]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Write a single byte to a register.  
[`uint8_t write_payload(const void *buf, uint8_t len)`][1] [**protected**]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Write the transmit payload.  
[`uint8_t read_payload(void *buf, uint8_t len)`][1] [**protected**]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Read the receice payload.  
[`uint8_t flush_rx(void)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Empty the receive buffer.  
[`uint8_t flush_tx(void)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Empty the transmit buffer.  
[`uint8_t get_status(void)`][1]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Retrive the current status of the chip.  
[`void print_status(uint8_t status)`][1] [**protected**]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Decode and print the given status to **stdout**.  
[`void print_observe_tx(uint8_t value)`][1] [**protected**]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Decode and print the given *'observe_tx'* value to **stdout**.  
[`void print_byte_register(prog_char *name, uint8_t reg, uint8_t qty=1)`][1] [**protected**]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Print the name and value of an 8-bit register to **stdout**.  
[`void print_address_register(prog_char *name, uint8_t reg, uint8_t qty=1)`][1] [**protected**]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Print the name and value of a 40-bit address register to stdout.  
[`void toggle_features(void)`][1] [**protected**]  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Turn *on* or *off* the special features of the chip.  

[1]: https://maniacbug.github.io/RF24/classRF24.html

## RF24.h File Reference

#### [Classes][1]

```c++
class RF24
```  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Driver for nRF24L01(+) 2.4GHz Wireless Transceiver.  

#### [Enumerations](https://maniacbug.github.io/RF24/RF24_8h.html)

```c++
enum rf24_pa_dbm_e{
	RF24_PA_MIN = 0, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX, RF24_PA_ERROR
}
```
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Power Amplifier level.  
```c++
enum rf24_datarate_e{
	RF24_1MBPS = 0, RF24_2MBPS, RF24_250KBPS
}
```  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Datarate(data transmission speed).  
```c++
enum rf24_crclength_e{
	RF24_CRC_DISABLED = 0, RF24_CRC_8, RF24_CRC_16
}
```  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;CRC length.  

