import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, mcp23017
from esphome.const import CONF_ID

#AUTO_LOAD = ["mcp23x17_base"]
#CODEOWNERS = ["@jesserockz"]
DEPENDENCIES = ["i2c"]
MULTI_CONF = True

mcp23017_ns = cg.esphome_ns.namespace("mcp23017")

MCP23017 = mcp23017_ns.class_("MCP23017", i2c.I2CDevice)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.Required(CONF_ID): cv.declare_id(MCP23017),
        }
    )
    .extend(i2c.i2c_device_schema(0x20))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
