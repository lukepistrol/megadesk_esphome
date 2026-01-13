import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.const import (
    UNIT_CENTIMETER,
    DEVICE_CLASS_DISTANCE,
)
from . import megadesk_ns, MegadeskComponent, CONF_MEGADESK_ID

DEPENDENCIES = ["megadesk"]

CONF_HEIGHT = "height"
CONF_HEIGHT_RAW = "height_raw"
CONF_MIN_RAW = "min_raw"
CONF_MAX_RAW = "max_raw"
CONF_MIN_CM = "min_cm"
CONF_MAX_CM = "max_cm"

ICON_HEIGHT = "mdi:ruler"

MegadeskHeightNumber = megadesk_ns.class_("MegadeskHeightNumber", number.Number, cg.Component)
MegadeskHeightRawNumber = megadesk_ns.class_("MegadeskHeightRawNumber", number.Number, cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_MEGADESK_ID): cv.use_id(MegadeskComponent),
        cv.Optional(CONF_HEIGHT): number.number_schema(
            MegadeskHeightNumber,
            unit_of_measurement=UNIT_CENTIMETER,
            device_class=DEVICE_CLASS_DISTANCE,
            icon=ICON_HEIGHT,
        ).extend(
            {
                cv.Optional(CONF_MIN_CM, default=65.8): cv.float_,
                cv.Optional(CONF_MAX_CM, default=122.0): cv.float_,
                cv.Optional(CONF_MIN_RAW, default=300): cv.int_,
                cv.Optional(CONF_MAX_RAW, default=6012): cv.int_,
            }
        ),
        cv.Optional(CONF_HEIGHT_RAW): number.number_schema(
            MegadeskHeightRawNumber,
            icon=ICON_HEIGHT,
        ).extend(
            {
                cv.Optional(CONF_MIN_RAW, default=300): cv.int_,
                cv.Optional(CONF_MAX_RAW, default=6012): cv.int_,
            }
        ),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_MEGADESK_ID])

    if height_config := config.get(CONF_HEIGHT):
        num = await number.new_number(
            height_config,
            min_value=height_config[CONF_MIN_CM],
            max_value=height_config[CONF_MAX_CM],
            step=0.1,
        )
        await cg.register_component(num, height_config)
        cg.add(num.set_parent(parent))
        cg.add(num.set_min_raw(height_config[CONF_MIN_RAW]))
        cg.add(num.set_max_raw(height_config[CONF_MAX_RAW]))
        cg.add(num.set_min_cm(height_config[CONF_MIN_CM]))
        cg.add(num.set_max_cm(height_config[CONF_MAX_CM]))
        cg.add(parent.set_height_number(num))

    if height_raw_config := config.get(CONF_HEIGHT_RAW):
        num = await number.new_number(
            height_raw_config,
            min_value=height_raw_config[CONF_MIN_RAW],
            max_value=height_raw_config[CONF_MAX_RAW],
            step=1,
        )
        await cg.register_component(num, height_raw_config)
        cg.add(num.set_parent(parent))
        cg.add(num.set_min_raw(height_raw_config[CONF_MIN_RAW]))
        cg.add(num.set_max_raw(height_raw_config[CONF_MAX_RAW]))
        cg.add(parent.set_height_raw_number(num))
