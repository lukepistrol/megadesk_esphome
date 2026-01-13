#include "button.h"
#include "esphome/core/log.h"

namespace esphome {
namespace megadesk {

static const char *const TAG = "megadesk.button";

void MegadeskButtonBase::dump_config() {
  LOG_BUTTON("", "Megadesk Button", this);
}

void MegadeskButtonBase::send_command_(const char *cmd) {
  this->parent_->write_str(cmd);
}

void MegadeskButtonBase::send_commands_(const char *cmd1, const char *cmd2) {
  this->parent_->write_str(cmd1);
  this->parent_->write_str(cmd2);
}

// Position recall buttons
void SittingButton::press_action() {
  ESP_LOGD(TAG, "Sitting button pressed");
  this->send_commands_("<C0.0.", "<l,2.");
}

void StandingButton::press_action() {
  ESP_LOGD(TAG, "Standing button pressed");
  this->send_commands_("<C0.0.", "<L,2.");
}

void RelaxedButton::press_action() {
  ESP_LOGD(TAG, "Relaxed button pressed");
  this->send_commands_("<C0.0.", "<l,3.");
}

// Movement buttons
void MoveUpButton::press_action() {
  ESP_LOGD(TAG, "Move Up button pressed");
  this->send_commands_("<C0.0.", "<+150..");
}

void MoveDownButton::press_action() {
  ESP_LOGD(TAG, "Move Down button pressed");
  this->send_commands_("<C0.0.", "<-150..");
}

// System buttons
void RebootButton::press_action() {
  ESP_LOGD(TAG, "Reboot button pressed");
  this->send_commands_("<C0.0.", "<L,15.");
}

void CalibrateButton::press_action() {
  ESP_LOGD(TAG, "Calibrate button pressed");
  this->send_commands_("<C0.0.", "<L,14.");
}

// Position save buttons
void SetSittingButton::press_action() {
  ESP_LOGD(TAG, "Set Sitting button pressed");
  this->send_commands_("<C0.0.", "<s,2.");
}

void SetStandingButton::press_action() {
  ESP_LOGD(TAG, "Set Standing button pressed");
  this->send_commands_("<C0.0.", "<S,2.");
}

void SetRelaxedButton::press_action() {
  ESP_LOGD(TAG, "Set Relaxed button pressed");
  this->send_commands_("<C0.0.", "<s,3.");
}

// Height limit buttons
void SetMinHeightButton::press_action() {
  ESP_LOGD(TAG, "Set Min Height button pressed");
  this->send_commands_("<C0.0.", "<L,11.");
}

void SetMaxHeightButton::press_action() {
  ESP_LOGD(TAG, "Set Max Height button pressed");
  this->send_commands_("<C0.0.", "<L,12.");
}

}  // namespace megadesk
}  // namespace esphome
