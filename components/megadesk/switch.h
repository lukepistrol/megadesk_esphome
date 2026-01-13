#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include "megadesk.h"

namespace esphome {
namespace megadesk {

class MegadeskSwitchBase : public switch_::Switch, public Component {
 public:
  void set_parent(MegadeskComponent *parent) { this->parent_ = parent; }
  float get_setup_priority() const override { return setup_priority::DATA - 1.0f; }

 protected:
  MegadeskComponent *parent_{nullptr};
};

class AudioSwitch : public MegadeskSwitchBase {
 public:
  void dump_config() override;
  void write_state(bool state) override;
  void update_state(bool state) { this->publish_state(state); }
};

class TwoButtonSwitch : public MegadeskSwitchBase {
 public:
  void dump_config() override;
  void write_state(bool state) override;
  void update_state(bool state) { this->publish_state(state); }
};

}  // namespace megadesk
}  // namespace esphome
