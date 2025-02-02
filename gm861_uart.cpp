#include "esphome.h"

class GM861UART : public Component, public UARTDevice {
 public:
  // Constructor
  GM861UART(UARTComponent *parent) : UARTDevice(parent) {}

  // Heartbeat packet and expected response
  const uint8_t heartbeat_packet[9] = {0x7E, 0x00, 0x0A, 0x01, 0x00, 0x00, 0x00, 0x30, 0x1A};
  const uint8_t heartbeat_response[7] = {0x03, 0x00, 0x00, 0x01, 0x00, 0x33, 0x31};

  // Counter for consecutive failed responses
  int failed_response_count = 0;
  bool heartbeat_expected = false;  // Flag to indicate if heartbeat response is expected

// This function is called when data is received
void on_receive(const uint8_t *data, size_t length) {
  // Check if we are expecting a heartbeat response
  if (heartbeat_expected) {
    // Check if the received data matches the expected heartbeat response
    if (length == sizeof(heartbeat_response) && memcmp(data, heartbeat_response, length) == 0) {
      ESP_LOGI("GM861", "Received expected heartbeat response.");
      failed_response_count = 0;  // Reset the counter on success
    } else {
      // Log unexpected response with received data
      ESP_LOGW("GM861", "Unexpected response received for heartbeat: %s", format_data(data, length).c_str());
      failed_response_count++;
      handle_failed_responses();
    }
    heartbeat_expected = false;  // Reset the flag after checking
  } else {
    // Handle normal communication (e.g., barcode data)
    handle_normal_communication(data, length);
  }
}

// Helper function to format data as a hex string
std::string format_data(const uint8_t *data, size_t length) {
  std::string formatted_data;
  for (size_t i = 0; i < length; i++) {
    formatted_data += "0x" + String(data[i], HEX) + " ";  // Format each byte as hex
  }
  return formatted_data;
}

  // Handle normal communication data
  void handle_normal_communication(const uint8_t *data, size_t length) {
    // Process the received data (e.g., barcode)
    std::string barcode(reinterpret_cast<const char*>(data), length);
    ESP_LOGI("GM861", "Received barcode data: %s", barcode.c_str());
    
    // You can add more processing here, like sending the barcode to Home Assistant
  }

  // Handle failed responses
  void handle_failed_responses() {
    if (failed_response_count >= 3) {
      ESP_LOGE("GM861", "No correct reply received for three consecutive attempts.");
      // Handle the failure accordingly (e.g., notify, reset, etc.)
      // You can add your custom handling logic here
    }
  }

  // Send the heartbeat packet
  void send_heartbeat() {
    this->write_array(heartbeat_packet, sizeof(heartbeat_packet));
    ESP_LOGD("GM861", "Sent heartbeat packet.");
    heartbeat_expected = true;  // Set the flag to expect a heartbeat response
  }

  // This function is called to initialize the component
  void setup() override {
    // Set up the UART listener
    this->set_on_receive_callback([this](const uint8_t *data, size_t length) {
      this->on_receive(data, length);
    });

    // Set up a periodic heartbeat
    App.register_interval(10000, [this]() { this->send_heartbeat(); });  // 10 seconds
  }
};
