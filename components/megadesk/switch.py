import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import ENTITY_CATEGORY_CONFIG
from . import megadesk_ns, MegadeskComponent, CONF_MEGADESK_ID

DEPENDENCIES = ["megadesk"]

CONF_AUDIO = "audio"
CONF_TWO_BUTTON = "two_button"

MegadeskSwitchBase = megadesk_ns.class_("MegadeskSwitchBase", switch.Switch, cg.Component)
AudioSwitch = megadesk_ns.class_("AudioSwitch", MegadeskSwitchBase)
TwoButtonSwitch = megadesk_ns.class_("TwoButtonSwitch", MegadeskSwitchBase)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_MEGADESK_ID): cv.use_id(MegadeskComponent),
        cv.Optional(CONF_AUDIO): switch.switch_schema(
            AudioSwitch,
            icon="mdi:music-note",
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
        cv.Optional(CONF_TWO_BUTTON): switch.switch_schema(
            TwoButtonSwitch,
            icon="mdi:gesture-tap-button",
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_MEGADESK_ID])

    if audio_config := config.get(CONF_AUDIO):
        sw = await switch.new_switch(audio_config)
        await cg.register_component(sw, audio_config)
        cg.add(sw.set_parent(parent))
        cg.add(parent.set_audio_switch(sw))

    if two_button_config := config.get(CONF_TWO_BUTTON):
        sw = await switch.new_switch(two_button_config)
        await cg.register_component(sw, two_button_config)
        cg.add(sw.set_parent(parent))
        cg.add(parent.set_two_button_switch(sw))
