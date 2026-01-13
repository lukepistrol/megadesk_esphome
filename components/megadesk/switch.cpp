#include "switch.h"
#include "esphome/core/log.h"

namespace esphome {
namespace megadesk {

static const char *const TAG = "megadesk.switch";

void AudioSwitch::dump_config() {
  LOG_SWITCH("", "Megadesk Audio Switch", this);
}

void AudioSwitch::write_state(bool state) {
  ESP_LOGD(TAG, "Setting audio to %s", ONOFF(state));
  // Toggle command - send three times as per original implementation
  this->parent_->write_str("<R.17.");
  this->parent_->write_str("<L,17.");
  this->parent_->write_str("<R.17.");
}

void TwoButtonSwitch::dump_config() {
  LOG_SWITCH("", "Megadesk Two Button Switch", this);
}

void TwoButtonSwitch::write_state(bool state) {
  ESP_LOGD(TAG, "Setting two button mode to %s", ONOFF(state));
  // Toggle command - send three times as per original implementation
  this->parent_->write_str("<R.18.");
  this->parent_->write_str("<L,18.");
  this->parent_->write_str("<R.18.");
}

}  // namespace megadesk
}  // namespace esphome
