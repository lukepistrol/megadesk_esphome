#include "number.h"
#include "esphome/core/log.h"

namespace esphome {
namespace megadesk {

static const char *const TAG = "megadesk.number";

// MegadeskHeightNumber implementation

void MegadeskHeightNumber::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Megadesk Height Number...");
}

void MegadeskHeightNumber::dump_config() {
  LOG_NUMBER("", "Megadesk Height (cm)", this);
  ESP_LOGCONFIG(TAG, "  Min CM: %.1f", this->min_cm_);
  ESP_LOGCONFIG(TAG, "  Max CM: %.1f", this->max_cm_);
  ESP_LOGCONFIG(TAG, "  Min Raw: %d", this->min_raw_);
  ESP_LOGCONFIG(TAG, "  Max Raw: %d", this->max_raw_);
}

void MegadeskHeightNumber::control(float value) {
  int raw = this->cm_to_raw_(value);
  ESP_LOGD(TAG, "Setting height to %.1f cm (raw: %d)", value, raw);
  
  // Send command to desk
  char buf[20];
  sprintf(buf, "<=%d,.", raw);
  this->parent_->write_str(buf);
  
  // Publish state immediately for responsive UI
  this->publish_state(value);
}

void MegadeskHeightNumber::update_from_raw(int raw_value) {
  float cm = this->raw_to_cm_(raw_value);
  // Round to 1 decimal place
  cm = floorf(cm * 10.0f) / 10.0f;
  this->publish_state(cm);
}

float MegadeskHeightNumber::raw_to_cm_(int raw) {
  // Linear interpolation: cm = ((raw - min_raw) * (max_cm - min_cm)) / (max_raw - min_raw) + min_cm
  return (((float)(raw - this->min_raw_) * (this->max_cm_ - this->min_cm_)) / 
          (float)(this->max_raw_ - this->min_raw_)) + this->min_cm_;
}

int MegadeskHeightNumber::cm_to_raw_(float cm) {
  // Inverse linear interpolation: raw = ((cm - min_cm) * (max_raw - min_raw)) / (max_cm - min_cm) + min_raw
  return (int)((((cm - this->min_cm_) * (float)(this->max_raw_ - this->min_raw_)) / 
                (this->max_cm_ - this->min_cm_)) + (float)this->min_raw_);
}

// MegadeskHeightRawNumber implementation

void MegadeskHeightRawNumber::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Megadesk Height Raw Number...");
}

void MegadeskHeightRawNumber::dump_config() {
  LOG_NUMBER("", "Megadesk Height (raw)", this);
  ESP_LOGCONFIG(TAG, "  Min Raw: %d", this->min_raw_);
  ESP_LOGCONFIG(TAG, "  Max Raw: %d", this->max_raw_);
}

void MegadeskHeightRawNumber::control(float value) {
  int raw = (int)value;
  ESP_LOGD(TAG, "Setting raw height to %d", raw);
  
  // Send command to desk
  char buf[20];
  sprintf(buf, "<=%d,.", raw);
  this->parent_->write_str(buf);
  
  // Publish state immediately for responsive UI
  this->publish_state(value);
}

void MegadeskHeightRawNumber::update_from_raw(int raw_value) {
  this->publish_state((float)raw_value);
}

}  // namespace megadesk
}  // namespace esphome
