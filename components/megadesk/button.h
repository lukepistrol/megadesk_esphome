#pragma once

#include "esphome/core/component.h"
#include "esphome/components/button/button.h"
#include "megadesk.h"

namespace esphome {
namespace megadesk {

/// Base class for all Megadesk buttons
class MegadeskButtonBase : public button::Button, public Component {
 public:
  void set_parent(MegadeskComponent *parent) { this->parent_ = parent; }
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

 protected:
  MegadeskComponent *parent_{nullptr};
  
  /// Helper to send commands to the desk
  void send_command_(const char *cmd);
  void send_commands_(const char *cmd1, const char *cmd2);
};

// Position recall buttons
class SittingButton : public MegadeskButtonBase {
 public:
  void press_action() override;
};

class StandingButton : public MegadeskButtonBase {
 public:
  void press_action() override;
};

class RelaxedButton : public MegadeskButtonBase {
 public:
  void press_action() override;
};

// Movement buttons
class MoveUpButton : public MegadeskButtonBase {
 public:
  void press_action() override;
};

class MoveDownButton : public MegadeskButtonBase {
 public:
  void press_action() override;
};

// System buttons
class RebootButton : public MegadeskButtonBase {
 public:
  void press_action() override;
};

class CalibrateButton : public MegadeskButtonBase {
 public:
  void press_action() override;
};

// Position save buttons
class SetSittingButton : public MegadeskButtonBase {
 public:
  void press_action() override;
};

class SetStandingButton : public MegadeskButtonBase {
 public:
  void press_action() override;
};

class SetRelaxedButton : public MegadeskButtonBase {
 public:
  void press_action() override;
};

// Height limit buttons
class SetMinHeightButton : public MegadeskButtonBase {
 public:
  void press_action() override;
};

class SetMaxHeightButton : public MegadeskButtonBase {
 public:
  void press_action() override;
};

}  // namespace megadesk
}  // namespace esphome
