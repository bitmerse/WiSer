# WiSer Setup

### WiSer Devices connection setup <a href="#toc150171902" id="toc150171902"></a>

#### WS-UT-BM Variant <a href="#toc150171903" id="toc150171903"></a>

**Connecting WiSer-USB**

* Locate the WiSer-USB device in WiSer kit.
* Connect the WiSer-USB device to an available USB port on your PC.

![](<../.gitbook/assets/0 (1).png>)

**Connecting WiSer-TTL**

* Locate the WiSer-TTL device in WiSer kit.
* Connect the WiSer-TTL device to the serial interface of your embedded device using jumper cable. You also have the flexibility to connect the device to your embedded system using castellated pins positioned at the edge of the board.

<figure><img src="../.gitbook/assets/16 (1).png" alt=""><figcaption></figcaption></figure>

#### WS-UT-EN Variant

**Connecting WiSer-USB**

* Locate the WiSer-USB device in WiSer kit.
* Connect the WiSer-USB device to an available USB port on your PC.

![](<../.gitbook/assets/2 (1).png>)

**Connecting WiSer-TTL**

* Locate the WiSer-TTL device in WiSer kit.
* Connect the WiSer-TTL device to the serial interface of your embedded device using jumper cable.

<figure><img src="../.gitbook/assets/19 (1).png" alt=""><figcaption></figcaption></figure>

#### WS-UU-EN Variant <a href="#toc150171904" id="toc150171904"></a>

**Connecting WiSer-USB**

* Locate the 2 WiSer-USB devices in WiSer kit.
* Connect one WiSer-USB device to an available USB port on your PC/system and another WiSer-USB device to another host system.

![](<../.gitbook/assets/4 (1).png>)

### Powering On the Devices <a href="#toc150171905" id="toc150171905"></a>

#### WiSer-USB <a href="#toc150171906" id="toc150171906"></a>

There is only 1 way to provide power to the WiSer-USB board:

* Type-C USB port (Male) plug, default power supply (Directly connects to the Type-C port of host system)

#### WiSer-TTL

There are three mutually exclusive ways to provide power to the WiSer-TTL board:

* Type-C USB port (Female) connector, default power supply (The power can be supplied through a power adapter, power bank, or even from the host system itself)
* Through the header pins (J2), where you can connect to the VCC (+5V) and GND pins.
* Via the castellated pins (J3), where you can provide power by connecting to the VCC (+5V) and GND pins.

This versatility in powering options allows you to choose the method that best suits your setup and project requirements.

### LED Indicators <a href="#toc150171908" id="toc150171908"></a>

WiSer devices are equipped with LED status indicators to convey essential information.

#### Rx and Tx LED <a href="#toc150171909" id="toc150171909"></a>

These LEDs illuminate when active serial communication is established between devices, providing visual confirmation of data transfer.

![](<../.gitbook/assets/5 (1).png>)

#### CONN LED <a href="#toc150171910" id="toc150171910"></a>

This LED activates when you press the 'Find Pair' button on the WiSer-USB device.

**CONN LED Indications:**

* **Steady ON for 5 Seconds:** A short press of the 'Find Pair' button initiates this event. Both paired WiSer devices turn ON the CONN LED for 5 seconds, aiding in identifying the paired device when multiple WiSer device pairs are in use.
* **Flashing FAST for 5 Seconds:** Press and hold the 'Find Pair' button for more than 2 seconds to trigger this event. Both paired devices rapidly flash the CONN LED for 5 seconds, signifying that the RTS/CTS hardware flow control is now enabled. In this mode, the ESP32-S2 chip on WiSer-TTL utilizes the RTS and CTS pins for hardware flow control.
* **Flashing SLOW for 5 Seconds:** Another long press of the 'Find Pair' button for more than 2 seconds triggers this event. Both paired devices slowly flash the CONN LED for 5 seconds, indicating that the RTS/CTS hardware flow control is now disabled. In this mode, the RTS and DTR pins can serve as general-purpose I/Os, controllable via USB requests from the host system.

<figure><img src="../.gitbook/assets/40 (1).png" alt=""><figcaption></figcaption></figure>

These LED indicators serve as your visual guides throughout the setup process, ensuring that you have the necessary feedback to facilitate a seamless connection.

Please note that for the WiSer-USB device, the CONN LED is combined with the Tx (Transmit) LED. This means that when you press ‘Find Pair’ button or transmit data, the same LED will provide feedback for both functions.
