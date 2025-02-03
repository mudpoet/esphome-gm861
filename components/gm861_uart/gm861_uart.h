#include "esphome.h"
#include "text_sensor/text_sensor.h"
#include "binary_sensor/binary_sensor.h"

namespace esphome {
namespace gm861_uart {

class GM861UARTComponent : public Component, public uart::UARTDevice {
 public:
  void setup() override;
  void loop() override;
  void set_barcode_sensor(text_sensor::TextSensor *sensor) { barcode_sensor_ = sensor; }
  void set_heartbeat_sensor(binary_sensor::BinarySensor *sensor) { heartbeat_sensor_ = sensor; }
  bool is_connected() const { return consecutive_failures_ < 3; }
  int get_consecutive_failures() const { return consecutive_failures_; }

 protected:
  void send_heartbeat_();
  void process_data_(const std::vector<uint8_t> &data);
  void handle_heartbeat_response_(const std::vector<uint8_t> &response);
  void handle_barcode_data_(const std::vector<uint8_t> &data);

  text_sensor::TextSensor *barcode_sensor_{nullptr};
  binary_sensor::BinarySensor *heartbeat_sensor_{nullptr};
  uint32_t last_heartbeat_time_{0};
  uint8_t consecutive_failures_{0};
  bool expecting_response_{false};
  static const uint32_t HEARTBEAT_INTERVAL = 10000;

  const std::vector<uint8_t> HEARTBEAT_PACKET = {0x7E, 0x00, 0x0A, 0x01, 0x00, 0x00, 0x00, 0x30, 0x1A};
  const std::vector<uint8_t> EXPECTED_RESPONSE = {0x03, 0x00, 0x00, 0x01, 0x00, 0x33, 0x31};
};

}  // namespace gm861_uart
}  // namespace esphome
