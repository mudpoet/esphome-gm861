#ifndef GM861_UART_H
#define GM861_UART_H

#include "esphome.h"

class GM861UART : public Component, public UARTDevice {
 public:
  // Constructor
  GM861UART(UARTComponent *parent);

  // Heartbeat packet and expected response
  const uint8_t heartbeat_packet[9];
  const uint8_t heartbeat_response[7];

  // Counter for consecutive failed responses
  int failed_response_count;
  bool heartbeat_expected;  // Flag to indicate if heartbeat response is expected

  // This function is called when data is received
  void on_receive(const uint8_t *data, size_t length);

  // Handle normal communication data
  void handle_normal_communication(const uint8_t *data, size_t length);

  // Handle failed responses
  void handle_failed_responses();

  // Send the heartbeat packet
  void send_heartbeat();

  // This function is called to initialize the component
  void setup() override;

  // Helper function to format data as a hex string
  std::string format_data(const uint8_t *data, size_t length);
};

#endif  // GM861_UART_H
