# Technical Specifications

|                           | **WiSer-USB**                                                                                                                                                                                                                                                                                                                                                  | **WiSer-TTL**                                                                                                                                                                                                                                                                                                                                                                     |
| ------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Processing Unit**       | Espressif Systems ESP32-S2 Wireless SoC                                                                                                                                                                                                                                                                                                                        | Espressif Systems ESP32-S2 Wireless SoC                                                                                                                                                                                                                                                                                                                                           |
| **Wireless Protocol**     | IEEE 802.11 b/g/n 2.4GHz                                                                                                                                                                                                                                                                                                                                       | IEEE 802.11 b/g/n 2.4GHz                                                                                                                                                                                                                                                                                                                                                          |
| **Supported Platforms**   | Windows, Linux, Mac and Android                                                                                                                                                                                                                                                                                                                                | -                                                                                                                                                                                                                                                                                                                                                                                 |
| **Serial Port**           | <p><strong>Baud Rate:</strong> All standard and custom rates up to 921,600 baud</p><p><strong>Data bits:</strong> 6-bit, 7-bit, 8-bit</p><p><strong>Parity types:</strong> NONE, ODD, EVEN, MARK, and SPACE</p><p><strong>Stop bits:</strong> 1-bit, 1.5-bit, 2-bit</p><p><strong>Flow control:</strong> software (XON/XOFF), hardware (RTS/CTS), and None</p> | -                                                                                                                                                                                                                                                                                                                                                                                 |
| **User Indication**       | 2 red LEDs for RX and TX indication                                                                                                                                                                                                                                                                                                                            | 3 red LEDs for RX, TX and CONN indication                                                                                                                                                                                                                                                                                                                                         |
| **Buttons**               | <p><strong>FIND-PAIR button:</strong> To locate the paired device</p><p><strong>BOOT button:</strong> for FW upgrade</p>                                                                                                                                                                                                                                       | **BOOT button:** for FW upgrade                                                                                                                                                                                                                                                                                                                                                   |
| **Connectors**            | **USB Type-C male plug:** For power supply, serial data communication and firmware upgrade                                                                                                                                                                                                                                                                     | <p><strong>USB Type-C Female port:</strong> For Power supply and firmware upgrade</p><p><strong>Power supply Input Pins:</strong> To supply power using Jumper cables</p><p><strong>Serial Interface Pins:</strong></p><ul><li>3V3, TXD, RXD, RTS, CTS, DTR, VCC, GND</li></ul><p>(Note: 3V3 and VCC are power supply output pins which can be used to power external boards)</p> |
| **IO Pin tolerances**     | -                                                                                                                                                                                                                                                                                                                                                              | <p><strong>Voltage range:</strong> 0 to 3.3V</p><p><strong>Output Pin Current:</strong> 25 mA</p><p><strong>Input Pin Current:</strong> 50nA</p>                                                                                                                                                                                                                                  |
| **Power Supply**          | 5V @ 500mA                                                                                                                                                                                                                                                                                                                                                     | 5V @ 500mA                                                                                                                                                                                                                                                                                                                                                                        |
| **Antenna Type**          | Omnidirectional chip antenna with 0.5 dBi gain (Peak)                                                                                                                                                                                                                                                                                                          | Omnidirectional chip antenna with 0.5 dBi gain (Peak)                                                                                                                                                                                                                                                                                                                             |
| **Dimensions**            | <p><strong>With enclosure:</strong> 37.8 x 20.4 x 8.2 mm</p><p><strong>Without enclosure:</strong> 35.8 x 14.4 x 4.7 mm</p>                                                                                                                                                                                                                                    | <p><strong>With enclosure:</strong> 36.5 x 38.4 x 9.8 mm</p><p><strong>Without enclosure and header pins:</strong> 28 x 29 x 4.3 mm</p>                                                                                                                                                                                                                                           |
| **Weight**                | <p><strong>With enclosure:</strong> 4.5 grams</p><p><strong>Without enclosure:</strong> 1.6 grams</p>                                                                                                                                                                                                                                                          | <p><strong>With enclosure:</strong> 10.5 grams</p><p><strong>Without enclosure:</strong> 3.7 grams</p>                                                                                                                                                                                                                                                                            |
| **Operating Temperature** | -40 to 85 °C                                                                                                                                                                                                                                                                                                                                                   | -40 to 85 °C                                                                                                                                                                                                                                                                                                                                                                      |
| **Humidity**              | < 60% RH (Non-condensing)                                                                                                                                                                                                                                                                                                                                      | < 60% RH (Non-condensing)                                                                                                                                                                                                                                                                                                                                                         |