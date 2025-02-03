# GM861 UART Component for ESPHome

This is a custom ESPHome component for the GM861 Bar Code Reader Module. It communicates with the GM861 device over UART and includes a heartbeat mechanism to ensure the device is responsive.

## Features
- Sends a heartbeat packet every 10 seconds.
- Logs unexpected responses and errors if the heartbeat fails 3 consecutive times.
- Logs barcode and QR code data received from the device.

## Installation
Add this repository to your ESPHome configuration:
   ```yaml
   external_components:
     - source:
         type: git
         url: https://github.com/mudpoet/gm861_uart.git
       components: [gm861_uart]
   ```
