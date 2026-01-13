#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace megadesk {

class MegadeskComponent : public Component, public uart::UARTDevice {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_raw_height_sensor(sensor::Sensor *sensor) { raw_height_sensor_ = sensor; }
  void set_min_height_sensor(sensor::Sensor *sensor) { min_height_sensor_ = sensor; }
  void set_max_height_sensor(sensor::Sensor *sensor) { max_height_sensor_ = sensor; }
  void set_audio_sensor(sensor::Sensor *sensor) { audio_sensor_ = sensor; }
  void set_two_button_sensor(sensor::Sensor *sensor) { two_button_sensor_ = sensor; }

 protected:
  int read_digits_();
  void recv_data_();
  void parse_data_(uint8_t control, uint8_t command, uint16_t position, uint8_t push_addr);

  sensor::Sensor *raw_height_sensor_{nullptr};
  sensor::Sensor *min_height_sensor_{nullptr};
  sensor::Sensor *max_height_sensor_{nullptr};
  sensor::Sensor *audio_sensor_{nullptr};
  sensor::Sensor *two_button_sensor_{nullptr};

  int digits_{0};
  uint16_t received_bytes_[4]{0};
  uint8_t receive_index_{0};
};

}  // namespace megadesk
}  // namespace esphome
