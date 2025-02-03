import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, text_sensor, binary_sensor
from esphome.const import CONF_ID

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
        cv.Optional(CONF_BARCODE): text_sensor.text_sensor_schema(),
        cv.Optional(CONF_HEARTBEAT_STATUS): binary_sensor.binary_sensor_schema(
            device_class="connectivity"
        ),
    }
).extend(uart.UART_DEVICE_SCHEMA).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)

    if CONF_BARCODE in config:
        barcode_sensor = yield text_sensor.new_text_sensor(config[CONF_BARCODE])
        cg.add(var.set_barcode_sensor(barcode_sensor))

    if CONF_HEARTBEAT_STATUS in config:
        heartbeat_sensor = yield binary_sensor.new_binary_sensor(config[CONF_HEARTBEAT_STATUS])
        cg.add(var.set_heartbeat_sensor(heartbeat_sensor))
