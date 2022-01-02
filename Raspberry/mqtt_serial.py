# https://www.eclipse.org/paho/files/jsdoc/index.html
import paho.mqtt.client as mqtt
import json
import serial

PORT_NAME = "/dev/ttyACM0"  # /dev/serial0 /dev/ttyACM0
PORT_BAUD = 115200
END = "\n".encode("utf-8")


def on_connect(client, userdata, flags, rc):
    # print("onConnect")
    print("MQTT connected with result code " + str(rc))

    MQTTclient.subscribe(subtopic)
    ser.flushOutput()
    ser.write("con".encode("utf-8"))
    ser.write(END)


def on_message(client, userdata, msg):
    data = json.loads(str(msg.payload.decode("utf-8")))
    process_message(data)


def process_message(data):
    try:
        data["cmd"]  # DEBUG: remove print
    except KeyError:
        print(">msg without -cmd")
    else:
        # print(repr(data)) # printable representation of the object
        if data["cmd"] == "roulette":
            # attributes = data["att"]
            # ser.write("test\n".encode("utf-8"))
            out_data = json.dumps(data).encode("utf-8")  # print("send")
            print(repr(out_data))
            ser.write(out_data)
            ser.write(END)


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
