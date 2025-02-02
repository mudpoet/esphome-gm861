# GM861 UART Component for ESPHome

This is a custom ESPHome component for the GM861 barcode and QR code reader. It communicates with the GM861 device over UART and includes a heartbeat mechanism to ensure the device is responsive.

## Features
- Sends a heartbeat packet every 10 seconds.
- Logs unexpected responses and errors if the heartbeat fails 3 consecutive times.
- Logs barcode and QR code data received from the device.

## Installation
### Add this repository to your ESPHome configuration:
   ```yaml
   external_components:
     - source:
         type: git
         url: https://github.com/mudpoet/gm861_uart.git
       components: [gm861_uart]

### Sample yaml File:
   ```yaml
   esphome:
     name: gm861_example
     platform: ESP32
     board: esp32dev
   
   wifi:
     ssid: "your_wifi_ssid"
     password: "your_wifi_password"
   
   logger:
     level: DEBUG
   
   uart:
     tx_pin: GPIO1
     rx_pin: GPIO3
     baud_rate: 9600
     parity: NONE
     stop_bits: 1
   
   external_components:
     - source:
         type: git
         url: https://github.com/mudpoet/gm861_uart.git
       components: [gm861_uart]
   
   custom_component:
     - platform: gm861_uart
       id: gm861_uart_component
   
   api:
   ota:
