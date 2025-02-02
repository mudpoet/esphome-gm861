#ifndef GM861_UART_H
#define GM861_UART_H

#include "esphome.h"

class GM861UARTComponent : public Component, public UARTDevice {
 public:
  GM861UARTComponent(UARTComponent *parent) : UARTDevice(parent) {}

  void setup() override;
  void loop() override;

 protected:
  void send_heartbeat();
  void process_data(const std::vector<uint8_t> &data);
  void handle_response(const std::vector<uint8_t> &response);

  uint32_t last_heartbeat_time_{0};
  uint8_t consecutive_failures_{0};
  static const uint32_t HEARTBEAT_INTERVAL = 10000; // 10 seconds

  // Heartbeat packet and expected response
  const std::vector<uint8_t> HEARTBEAT_PACKET = {0x7E, 0x00, 0x0A, 0x01, 0x00, 0x00, 0x00, 0x30, 0x1A};
  const std::vector<uint8_t> EXPECTED_RESPONSE = {0x03, 0x00, 0x00, 0x01, 0x00, 0x33, 0x31};
};

#endif  // GM861_UART_H
