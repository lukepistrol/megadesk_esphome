#include "megadesk.h"
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
      // non-digit, we're done - return what we have
      return this->digits_;
    }
    // it's a digit, add with base10 shift
    this->digits_ = 10 * this->digits_ + (r - 0x30);
    // keep reading...
  }
  return -1;
}

void MegadeskComponent::recv_data_() {
  const int num_chars = 2;
  const int num_fields = 4;  // read/store all 4 fields for simplicity, use only the last 3.
  int r;  // read char/digit

  // read 2 chars
  while ((this->receive_index_ < num_chars) && ((r = this->read()) != -1)) {
    if ((this->receive_index_ == 0) && ((r != '>') && (r != 'E') && (r != 'D') && (r != '!'))) {
      // first char is not valid control char, keep reading...
      continue;
    }
    this->received_bytes_[this->receive_index_] = r;
    ++this->receive_index_;
  }

  // read ascii digits
  while ((this->receive_index_ >= num_chars) && ((r = this->read_digits_()) != -1)) {
    this->received_bytes_[this->receive_index_] = r;
    this->digits_ = 0;  // clear
    if (++this->receive_index_ == num_fields) {
      // that's all 4 fields. parse/process them now and break out.
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
    ESP_LOGD(TAG, "[raw-height]: %c %c %d %d", control, command, position, push_addr);
    if (this->raw_height_sensor_ != nullptr) {
      this->raw_height_sensor_->publish_state(position);
    }
  } else if (command == 'R') {
    if (push_addr == 11) {
      ESP_LOGD(TAG, "[min-height]: %c %c %d %d", control, command, position, push_addr);
      if (this->min_height_sensor_ != nullptr) {
        this->min_height_sensor_->publish_state(position);
      }
    } else if (push_addr == 12) {
      ESP_LOGD(TAG, "[max-height]: %c %c %d %d", control, command, position, push_addr);
      if (this->max_height_sensor_ != nullptr) {
        this->max_height_sensor_->publish_state(position);
      }
    } else if (push_addr == 17) {
      ESP_LOGD(TAG, "[audio]: %c %c %d %d", control, command, position, push_addr);
      if (this->audio_sensor_ != nullptr) {
        this->audio_sensor_->publish_state(position);
      }
    } else if (push_addr == 18) {
      ESP_LOGD(TAG, "[two-button]: %c %c %d %d", control, command, position, push_addr);
      if (this->two_button_sensor_ != nullptr) {
        this->two_button_sensor_->publish_state(position);
      }
    } else {
      ESP_LOGD(TAG, "[other]: %c %c %d %d", control, command, position, push_addr);
    }
  } else if (command == 'E') {
    ESP_LOGE(TAG, "Error response: %c %c %d %d", control, command, position, push_addr);
  }
}

}  // namespace megadesk
}  // namespace esphome
