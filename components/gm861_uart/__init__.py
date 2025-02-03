import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor
from esphome.const import CONF_ID, CONF_HEARTBEAT

DEPENDENCIES = ["uart"]
CODEOWNERS = ["@mudpoet"]

gm861_uart_ns = cg.esphome_ns.namespace("gm861_uart")
GM861UARTComponent = gm861_uart_ns.class_(
    "GM861UARTComponent", cg.Component, uart.UARTDevice
)

CONF_BARCODE = "barcode_sensor"
CONF_HEARTBEAT_STATUS = "heartbeat_status"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(GM861UARTComponent),
        cv.Optional(CONF_BARCODE): sensor.sensor_schema(),
        cv.Optional(CONF_HEARTBEAT_STATUS): sensor.sensor_schema(),
    }
).extend(uart.UART_DEVICE_SCHEMA).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)

    if CONF_BARCODE in config:
        barcode_sensor = yield sensor.new_sensor(config[CONF_BARCODE])
        cg.add(var.set_barcode_sensor(barcode_sensor))

    if CONF_HEARTBEAT_STATUS in config:
        heartbeat_sensor = yield sensor.new_sensor(config[CONF_HEARTBEAT_STATUS])
        cg.add(var.set_heartbeat_sensor(heartbeat_sensor))
