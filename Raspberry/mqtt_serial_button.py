# MQTT_API:
# https://www.eclipse.org/paho/files/jsdoc/index.html
import paho.mqtt.client as mqtt
import json
import serial
import random as rand

# EVENTs for LINUX
from pymouse import PyMouseEvent

# Settings
PORT_NAME = "/dev/ttyACM0"  # /dev/serial0 / COM# /dev/ttyACM0
PORT_BAUD = 115200
MSG_END = "\n".encode("utf-8")

MQTT_HOST = "broker.hivemq.com"
MQTT_PORT = 1883
MQTT_KEEPALIVE = 60
SUBTOPIC = "kea/guld"


""" MQTT RETURN CODE AMSWERS
0	Connection accepted
1	Connection refused, unacceptable protocol version
2	Connection refused, identifier rejected
3	Connection refused, server unavailable
4	Connection refused, bad user name or password
5	Connection refused, not authorized
"""


# Connect / reconnect to MQTT Broker
def on_connect(client, userdata, flags, rc):
    # print("onConnect")
    print("MQTT connected with return code " + str(rc))

    MQTTclient.subscribe(SUBTOPIC)


# Callback on arrival of MQTT message
def on_message(client, userdata, msg):
    data = json.loads(str(msg.payload.decode("utf-8")))
    process_message(data)


# Additional proccesing if needed - in this version only relay
def process_message(data):
    try:
        data["cmd"]  # DEBUG: remove print
    except KeyError:
        print(">msg without -cmd")
    else:
        # print(data)
        if data["cmd"] == "roulette":
            out_data = json.dumps(data).encode("utf-8")
            ser.write(out_data)
            ser.write(MSG_END)
            print(out_data)


# Test if serial port is open
try:
    ser = serial.Serial(
        port=PORT_NAME,
        baudrate=PORT_BAUD,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=1,
    )
except serial.SerialException as e:
    print(">serial error -", e)


MQTTclient = mqtt.Client()
MQTTclient.on_connect = on_connect
MQTTclient.on_message = on_message

MQTTclient.connect(MQTT_HOST, MQTT_PORT, MQTT_KEEPALIVE)

MQTTclient.loop_forever()


# Create event listener for button
class Button(PyMouseEvent):
    def __init__(self):
        PyMouseEvent.__init__(self)

    def click(self, x, y, button, press):
        if button == 2:  # 1 = left, 2 = middle, 3 = right
            if press:
                print("click")
                rand.seed()
                # Compact JSON
                roulette_data = '{{"att":{{"start":{start},"t":{step},"stop":{stop}}},"cmd":"roulette"}}'.format(
                    start=rand.randrange(1, 13),
                    step=rand.randrange(15, 51),
                    stop=rand.randrange(1, 13),
                )
                out_data = json.dumps(roulette_data).encode("utf-8")
                ser.write(out_data)
                ser.write(MSG_END)
        else:  # stop - doesnt work sometimes
            self.stop()


button = Button()
button.run()
