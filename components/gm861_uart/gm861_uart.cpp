#include "gm861_uart.h"

namespace esphome {
namespace gm861_uart {

static const char *TAG = "gm861_uart";

void GM861UARTComponent::setup() {
  ESP_LOGI(TAG, "GM861 UART component initialized");
}

void GM861UARTComponent::loop() {
  uint32_t now = millis();

  // Send heartbeat every 10 seconds
  if (now - last_heartbeat_time_ >= HEARTBEAT_INTERVAL) {
    send_heartbeat_();
    last_heartbeat_time_ = now;
    last_heartbeat_sent_time_ = now;
    expecting_response_ = true;
  }

  // Check for heartbeat timeout
  if (expecting_response_ && (now - last_heartbeat_sent_time_ >= HEARTBEAT_TIMEOUT)) {
    check_heartbeat_timeout_();
  }

  // Read UART data
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
      reset_communication_();
    }
  }
}

void GM861UARTComponent::handle_barcode_data_(const std::vector<uint8_t> &data) {
  std::string barcode(data.begin(), data.end());
  ESP_LOGI(TAG, "Barcode scanned: %s", barcode.c_str());
  if (barcode_sensor_) barcode_sensor_->publish_state(barcode);
}

void GM861UARTComponent::check_heartbeat_timeout_() {
  ESP_LOGW(TAG, "Heartbeat response timeout!");
  consecutive_failures_++;
  expecting_response_ = false;
  
  if (consecutive_failures_ >= 3) {
    reset_communication_();
  }
}

void GM861UARTComponent::reset_communication_() {
  // GM861 Hardware Reset Command (from datasheet)
  const std::vector<uint8_t> RESET_CMD = {0x7E, 0x00, 0x09, 0x01, 0x00, 0x00, 0x00, 0x31, 0x1A};
  ESP_LOGI(TAG, "Attempting communication reset...");
  
  write_array(RESET_CMD);
  delay(500);  // Allow time for device reset
  flush();      // Clear UART buffers
  setup();      // Re-initialize component
  
  consecutive_failures_ = 0;
  last_heartbeat_time_ = millis();
  ESP_LOGI(TAG, "Communication reset complete");
}

void GM861UARTComponent::reset_communication() {
  reset_communication_();
}

}  // namespace gm861_uart
}  // namespace esphome
