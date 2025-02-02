#include "gm861_uart.h"

static const char *TAG = "gm861_uart";

void GM861UARTComponent::setup() {
  // Initialize UART with the specified parameters
  ESP_LOGI(TAG, "GM861 UART component setup");
}

void GM861UARTComponent::loop() {
  uint32_t now = millis();

  // Send heartbeat every 10 seconds
  if (now - last_heartbeat_time_ >= HEARTBEAT_INTERVAL) {
    send_heartbeat();
    last_heartbeat_time_ = now;
  }

  // Read data from UART
  if (available()) {
    std::vector<uint8_t> data;
    while (available()) {
      data.push_back(read());
    }
    process_data(data);
  }
}

void GM861UARTComponent::send_heartbeat() {
  // Send the heartbeat packet
  write_array(HEARTBEAT_PACKET);
  ESP_LOGD(TAG, "Heartbeat sent");
}

void GM861UARTComponent::process_data(const std::vector<uint8_t> &data) {
  // Check if the data matches the expected heartbeat response
  if (data == EXPECTED_RESPONSE) {
    handle_response(data);
  } else {
    // Log normal data as info
    std::string data_hex;
    for (uint8_t byte : data) {
      char buf[4];
      snprintf(buf, sizeof(buf), "%02X ", byte);
      data_hex += buf;
    }
    ESP_LOGI(TAG, "Data received: %s", data_hex.c_str());
  }
}

void GM861UARTComponent::handle_response(const std::vector<uint8_t> &response) {
  if (response == EXPECTED_RESPONSE) {
    consecutive_failures_ = 0;
    ESP_LOGD(TAG, "Heartbeat response OK");
  } else {
    consecutive_failures_++;
    std::string response_hex;
    for (uint8_t byte : response) {
      char buf[4];
      snprintf(buf, sizeof(buf), "%02X ", byte);
      response_hex += buf;
    }
    ESP_LOGW(TAG, "Unexpected heartbeat response: %s", response_hex.c_str());

    if (consecutive_failures_ >= 3) {
      ESP_LOGE(TAG, "Heartbeat failed 3 consecutive times");
      // Reset the counter or take further action if needed
      consecutive_failures_ = 0;
    }
  }
}
