#include "gm861_uart.h"

namespace esphome {
namespace gm861_uart {

static const char *TAG = "gm861_uart";

void GM861UARTComponent::setup() {
  ESP_LOGI(TAG, "GM861 UART component initialized");
}

void GM861UARTComponent::loop() {
  uint32_t now = millis();

  if (now - last_heartbeat_time_ >= HEARTBEAT_INTERVAL) {
    send_heartbeat_();
    last_heartbeat_time_ = now;
    expecting_response_ = true;
  }

  if (available()) {
    std::vector<uint8_t> data;
    while (available()) {
      data.push_back(read());
    }
    process_data_(data);
  }
}

void GM861UARTComponent::send_heartbeat_() {
  write_array(HEARTBEAT_PACKET);
  ESP_LOGD(TAG, "Heartbeat sent");
}

void GM861UARTComponent::process_data_(const std::vector<uint8_t> &data) {
  if (expecting_response_) {
    handle_heartbeat_response_(data);
    expecting_response_ = false;
  } else {
    handle_barcode_data_(data);
  }
}

void GM861UARTComponent::handle_heartbeat_response_(const std::vector<uint8_t> &response) {
  if (response == EXPECTED_RESPONSE) {
    ESP_LOGD(TAG, "Heartbeat OK");
    consecutive_failures_ = 0;
    if (heartbeat_sensor_) heartbeat_sensor_->publish_state(true);
  } else {
    consecutive_failures_++;
    ESP_LOGW(TAG, "Bad heartbeat response (%d consecutive failures)", consecutive_failures_);
    if (heartbeat_sensor_) heartbeat_sensor_->publish_state(false);
    
    if (consecutive_failures_ >= 3) {
      ESP_LOGE(TAG, "Critical heartbeat failure!");
    }
  }
}

void GM861UARTComponent::handle_barcode_data_(const std::vector<uint8_t> &data) {
  // Add barcode validation logic here if needed
  std::string barcode(data.begin(), data.end());
  ESP_LOGI(TAG, "Barcode scanned: %s", barcode.c_str());
  if (barcode_sensor_) barcode_sensor_->publish_state(barcode);
}

}  // namespace gm861_uart
}  // namespace esphome
