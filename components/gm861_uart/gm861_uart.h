#include "esphome.h"

namespace esphome {
namespace gm861_uart {

class GM861UARTComponent : public Component, public uart::UARTDevice {
 public:
  void setup() override;
  void loop() override;
  void set_barcode_sensor(sensor::Sensor *sensor) { barcode_sensor_ = sensor; }
  void set_heartbeat_sensor(sensor::Sensor *sensor) { heartbeat_sensor_ = sensor; }

 protected:
  void send_heartbeat_();
  void process_data_(const std::vector<uint8_t> &data);
  void handle_response_(const std::vector<uint8_t> &response);

  sensor::Sensor *barcode_sensor_{nullptr};
  sensor::Sensor *heartbeat_sensor_{nullptr};
  uint32_t last_heartbeat_time_{0};
  uint8_t consecutive_failures_{0};
  static const uint32_t HEARTBEAT_INTERVAL = 10000;  // 10 seconds

  const std::vector<uint8_t> HEARTBEAT_PACKET = {0x7E, 0x00, 0x0A, 0x01, 0x00, 0x00, 0x00, 0x30, 0x1A};
  const std::vector<uint8_t> EXPECTED_RESPONSE = {0x03, 0x00, 0x00, 0x01, 0x00, 0x33, 0x31};
};

}  // namespace gm861_uart
}  // namespace esphome
