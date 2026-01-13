import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_EMPTY,
    STATE_CLASS_MEASUREMENT,
    UNIT_EMPTY,
    ICON_EMPTY,
)
from . import megadesk_ns, MegadeskComponent, CONF_MEGADESK_ID

DEPENDENCIES = ["megadesk"]

CONF_RAW_HEIGHT = "raw_height"
CONF_MIN_HEIGHT = "min_height"
CONF_MAX_HEIGHT = "max_height"
CONF_AUDIO_ENABLED = "audio_enabled"
CONF_TWO_BUTTON_MODE = "two_button_mode"

ICON_HEIGHT_MIN = "mdi:border-bottom"
ICON_HEIGHT_MAX = "mdi:border-top"
ICON_AUDIO = "mdi:music-note"
ICON_TWO_BUTTON = "mdi:gesture-tap-button"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_MEGADESK_ID): cv.use_id(MegadeskComponent),
        cv.Optional(CONF_RAW_HEIGHT): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_MIN_HEIGHT): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_HEIGHT_MIN,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_MAX_HEIGHT): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_HEIGHT_MAX,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_AUDIO_ENABLED): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_AUDIO,
            accuracy_decimals=0,
        ),
        cv.Optional(CONF_TWO_BUTTON_MODE): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_TWO_BUTTON,
            accuracy_decimals=0,
        ),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_MEGADESK_ID])

    if raw_height_config := config.get(CONF_RAW_HEIGHT):
        sens = await sensor.new_sensor(raw_height_config)
        cg.add(parent.set_raw_height_sensor(sens))

    if min_height_config := config.get(CONF_MIN_HEIGHT):
        sens = await sensor.new_sensor(min_height_config)
        cg.add(parent.set_min_height_sensor(sens))

    if max_height_config := config.get(CONF_MAX_HEIGHT):
        sens = await sensor.new_sensor(max_height_config)
        cg.add(parent.set_max_height_sensor(sens))

    if audio_config := config.get(CONF_AUDIO_ENABLED):
        sens = await sensor.new_sensor(audio_config)
        cg.add(parent.set_audio_sensor(sens))

    if two_button_config := config.get(CONF_TWO_BUTTON_MODE):
        sens = await sensor.new_sensor(two_button_config)
        cg.add(parent.set_two_button_sensor(sens))
