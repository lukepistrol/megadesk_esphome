#pragma once

#include "esphome/core/component.h"
#include "esphome/components/number/number.h"
#include "megadesk.h"

namespace esphome {
namespace megadesk {

/// Base class for Megadesk number entities
class MegadeskNumberBase : public number::Number, public Component {
 public:
  void set_parent(MegadeskComponent *parent) { this->parent_ = parent; }
  void set_min_raw(int min_raw) { this->min_raw_ = min_raw; }
  void set_max_raw(int max_raw) { this->max_raw_ = max_raw; }
  
  float get_setup_priority() const override { return setup_priority::DATA - 1.0f; }

 protected:
  MegadeskComponent *parent_{nullptr};
  int min_raw_{300};
  int max_raw_{6012};
};

/// Number entity for desk height in centimeters
class MegadeskHeightNumber : public MegadeskNumberBase {
 public:
  void setup() override;
  void dump_config() override;
  void control(float value) override;
  
  void set_min_cm(float min_cm) { this->min_cm_ = min_cm; }
  void set_max_cm(float max_cm) { this->max_cm_ = max_cm; }
  
  /// Called by the parent component when raw height changes
  void update_from_raw(int raw_value);

 protected:
  float min_cm_{65.8f};
  float max_cm_{122.0f};
  
  /// Convert raw value to centimeters
  float raw_to_cm_(int raw);
  /// Convert centimeters to raw value
  int cm_to_raw_(float cm);
};

/// Number entity for desk height in raw units
class MegadeskHeightRawNumber : public MegadeskNumberBase {
 public:
  void setup() override;
  void dump_config() override;
  void control(float value) override;
  
  /// Called by the parent component when raw height changes
  void update_from_raw(int raw_value);
};

}  // namespace megadesk
}  // namespace esphome
