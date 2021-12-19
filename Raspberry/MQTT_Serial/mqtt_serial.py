# https://www.eclipse.org/paho/files/jsdoc/index.html
import paho.mqtt.client as mqtt
import json

import serial

PORT_NAME = "/dev/serial0"  # /dev/serial0 / COM#
PORT_BAUD = 115200


def on_connect(client, userdata, flags, rc):
    # print("onConnect")
    print("MQTT connected with result code " + str(rc))

    MQTTclient.subscribe(subtopic)


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
        print(data)
        if data["cmd"] == "roulette":
            out_data = json.dumps(data).encode("utf-8")
            ser.write(out_data)
            # print(out_data)


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


host = "broker.hivemq.com"
port = 1883
keepalive = 60
subtopic = "kea/guld"


MQTTclient = mqtt.Client()

MQTTclient.on_connect = on_connect
MQTTclient.on_message = on_message


MQTTclient.connect(host, port, keepalive)

MQTTclient.loop_forever()
