# WiSer Source Code

This directory contains the WiSer project source code built using the PlatformIO extension in VS Code.

## Compile and Upload

1. Press the "BOOT" button on WiSer-USB/WiSer-TTL and connect it to the USB connector on your computer.
2. After switching to boot mode, the board will accept the new firmware.
3. To build, click on the "Build" button in the PlatformIO toolbar at the bottom of the VS Code window.
4. To upload, click on the "Upload" button in the PlatformIO toolbar at the bottom of the VS Code window.
5. Verify that your code is compiled, uploaded, and executed on the WiSer board.

## Configuration MACROS

1. Before compilation, change the value of "DEVICE_CONFIG_MODE" to either "DEVICE_CONFIG_MODE_USB" for WiSer-USB
   or "DEVICE_CONFIG_MODE_UART" for WiSer-UART in `config.h`.
2. Change the value of "APP_ESPNOW_USE_NVS_PEER_MAC" to either 0 to use the default peer MAC address stored in "s_app_peer_mac"
   or 1 to use the pre-paired peer MAC address stored in the NVS memory during production.

### Notes

`platformio.ini` contains your project settings. More information about this file can be found [here](https://docs.platformio.org/en/stable/projectconf/index.html).