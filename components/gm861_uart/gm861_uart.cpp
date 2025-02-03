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
  if (data == EXPECTED_RESPONSE) {
    ESP_LOGD(TAG, "Heartbeat OK");
    if (heartbeat_sensor_) heartbeat_sensor_->publish_state(true);
    consecutive_failures_ = 0;
  } else {
    std::string barcode(data.begin(), data.end());
    ESP_LOGI(TAG, "Barcode: %s", barcode.c_str());
    if (barcode_sensor_) barcode_sensor_->publish_state(barcode);
  }
}

}  // namespace gm861_uart
}  // namespace esphome
