import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import button
from esphome.const import CONF_ID, ENTITY_CATEGORY_CONFIG
from . import megadesk_ns, MegadeskComponent, CONF_MEGADESK_ID

DEPENDENCIES = ["megadesk"]

# Button type constants
CONF_SITTING = "sitting"
CONF_STANDING = "standing"
CONF_RELAXED = "relaxed"
CONF_MOVE_UP = "move_up"
CONF_MOVE_DOWN = "move_down"
CONF_REBOOT = "reboot"
CONF_CALIBRATE = "calibrate"
CONF_SET_SITTING = "set_sitting"
CONF_SET_STANDING = "set_standing"
CONF_SET_RELAXED = "set_relaxed"
CONF_SET_MIN_HEIGHT = "set_min_height"
CONF_SET_MAX_HEIGHT = "set_max_height"

# C++ class references
MegadeskButtonBase = megadesk_ns.class_("MegadeskButtonBase", button.Button, cg.Component)
SittingButton = megadesk_ns.class_("SittingButton", MegadeskButtonBase)
StandingButton = megadesk_ns.class_("StandingButton", MegadeskButtonBase)
RelaxedButton = megadesk_ns.class_("RelaxedButton", MegadeskButtonBase)
MoveUpButton = megadesk_ns.class_("MoveUpButton", MegadeskButtonBase)
MoveDownButton = megadesk_ns.class_("MoveDownButton", MegadeskButtonBase)
RebootButton = megadesk_ns.class_("RebootButton", MegadeskButtonBase)
CalibrateButton = megadesk_ns.class_("CalibrateButton", MegadeskButtonBase)
SetSittingButton = megadesk_ns.class_("SetSittingButton", MegadeskButtonBase)
SetStandingButton = megadesk_ns.class_("SetStandingButton", MegadeskButtonBase)
SetRelaxedButton = megadesk_ns.class_("SetRelaxedButton", MegadeskButtonBase)
SetMinHeightButton = megadesk_ns.class_("SetMinHeightButton", MegadeskButtonBase)
SetMaxHeightButton = megadesk_ns.class_("SetMaxHeightButton", MegadeskButtonBase)

# Button definitions: (class, icon, entity_category)
BUTTON_TYPES = {
    CONF_SITTING: (SittingButton, "mdi:seat-recline-normal", None),
    CONF_STANDING: (StandingButton, "mdi:human-handsup", None),
    CONF_RELAXED: (RelaxedButton, "mdi:seat-recline-extra", None),
    CONF_MOVE_UP: (MoveUpButton, "mdi:transfer-up", None),
    CONF_MOVE_DOWN: (MoveDownButton, "mdi:transfer-down", None),
    CONF_REBOOT: (RebootButton, "mdi:restart", ENTITY_CATEGORY_CONFIG),
    CONF_CALIBRATE: (CalibrateButton, "mdi:tape-measure", ENTITY_CATEGORY_CONFIG),
    CONF_SET_SITTING: (SetSittingButton, "mdi:seat-recline-normal", ENTITY_CATEGORY_CONFIG),
    CONF_SET_STANDING: (SetStandingButton, "mdi:human-handsup", ENTITY_CATEGORY_CONFIG),
    CONF_SET_RELAXED: (SetRelaxedButton, "mdi:seat-recline-extra", ENTITY_CATEGORY_CONFIG),
    CONF_SET_MIN_HEIGHT: (SetMinHeightButton, "mdi:arrow-collapse-down", ENTITY_CATEGORY_CONFIG),
    CONF_SET_MAX_HEIGHT: (SetMaxHeightButton, "mdi:arrow-collapse-up", ENTITY_CATEGORY_CONFIG),
}


def create_button_schema(btn_class, icon, entity_category):
    """Create a button schema with the given class, icon and entity category."""
    schema = button.button_schema(
        btn_class,
        icon=icon,
    )
    if entity_category is not None:
        schema = schema.extend(
            {
                cv.Optional(
                    "entity_category", default=entity_category
                ): cv.entity_category,
            }
        )
    return schema


CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_MEGADESK_ID): cv.use_id(MegadeskComponent),
        **{
            cv.Optional(btn_type): create_button_schema(btn_class, icon, entity_cat)
            for btn_type, (btn_class, icon, entity_cat) in BUTTON_TYPES.items()
        },
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_MEGADESK_ID])

    for btn_type in BUTTON_TYPES:
        if btn_config := config.get(btn_type):
            btn = await button.new_button(btn_config)
            await cg.register_component(btn, btn_config)
            cg.add(btn.set_parent(parent))
