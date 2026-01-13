#include "megadesk.h"
#include "number.h"
#include "switch.h"
#include "esphome/core/log.h"

namespace esphome {
namespace megadesk {

static const char *const TAG = "megadesk";

void MegadeskComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Megadesk...");
}

void MegadeskComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Megadesk:");
  LOG_SENSOR("  ", "Raw Height", this->raw_height_sensor_);
  LOG_SENSOR("  ", "Min Height", this->min_height_sensor_);
  LOG_SENSOR("  ", "Max Height", this->max_height_sensor_);
  LOG_SENSOR("  ", "Audio Enabled", this->audio_sensor_);
  LOG_SENSOR("  ", "Two Button Mode", this->two_button_sensor_);
  this->check_uart_settings(115200);
}

void MegadeskComponent::loop() {
  while (this->available()) {
    this->recv_data_();
  }
}

int MegadeskComponent::read_digits_() {
  int r;
  while ((r = this->read()) > 0) {
    if ((r < 0x30) || (r > 0x39)) {
      return this->digits_;
    }
    this->digits_ = 10 * this->digits_ + (r - 0x30);
  }
  return -1;
}

void MegadeskComponent::recv_data_() {
  const int num_chars = 2;
  const int num_fields = 4;
  int r;

  while ((this->receive_index_ < num_chars) && ((r = this->read()) != -1)) {
    if ((this->receive_index_ == 0) && ((r != '>') && (r != 'E') && (r != 'D') && (r != '!'))) {
      continue;
    }
    this->received_bytes_[this->receive_index_] = r;
    ++this->receive_index_;
  }

  while ((this->receive_index_ >= num_chars) && ((r = this->read_digits_()) != -1)) {
    this->received_bytes_[this->receive_index_] = r;
    this->digits_ = 0;
    if (++this->receive_index_ == num_fields) {
      this->parse_data_(this->received_bytes_[0],
                        this->received_bytes_[1],
                        this->received_bytes_[2],
                        this->received_bytes_[3]);
      this->receive_index_ = 0;
      return;
    }
  }
}

void MegadeskComponent::parse_data_(uint8_t control, uint8_t command, uint16_t position, uint8_t push_addr) {
  if (control != '>') {
    ESP_LOGE(TAG, "Invalid control byte: %c %c %d %d", control, command, position, push_addr);
    return;
  }

  if (command == '=') {
    ESP_LOGD(TAG, "[raw-height]: %d", position);
    if (this->raw_height_sensor_ != nullptr)
      this->raw_height_sensor_->publish_state(position);
    if (this->height_number_ != nullptr)
      this->height_number_->update_from_raw(position);
    if (this->height_raw_number_ != nullptr)
      this->height_raw_number_->update_from_raw(position);

  } else if (command == 'R') {
    if (push_addr == 11) {
      ESP_LOGD(TAG, "[min-height]: %d", position);
      if (this->min_height_sensor_ != nullptr)
        this->min_height_sensor_->publish_state(position);
    } else if (push_addr == 12) {
      ESP_LOGD(TAG, "[max-height]: %d", position);
      if (this->max_height_sensor_ != nullptr)
        this->max_height_sensor_->publish_state(position);
    } else if (push_addr == 17) {
      ESP_LOGD(TAG, "[audio]: %d", position);
      if (this->audio_sensor_ != nullptr)
        this->audio_sensor_->publish_state(position);
      if (this->audio_switch_ != nullptr)
        this->audio_switch_->update_state(position != 0);
    } else if (push_addr == 18) {
      ESP_LOGD(TAG, "[two-button]: %d", position);
      if (this->two_button_sensor_ != nullptr)
        this->two_button_sensor_->publish_state(position);
      if (this->two_button_switch_ != nullptr)
        this->two_button_switch_->update_state(position != 0);
    }
  } else if (command == 'E') {
    ESP_LOGE(TAG, "Error response: %c %c %d %d", control, command, position, push_addr);
  }
}

}  // namespace megadesk
}  // namespace esphome
