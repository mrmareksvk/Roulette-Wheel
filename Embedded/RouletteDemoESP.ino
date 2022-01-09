/*
   Embedded program for ESP32 MQTT and LED Control (Roulette)
   For Project #2 Roulette wheel - KEA PTI 5th. semester
 */

// LIBRARIES, IMPORTS
#include <Adafruit_NeoPixel.h>

#include "secret.h"
#include <WiFi.h>

#include <ArduinoJson.h>
#include <PubSubClient.h>

// DEFINITIONS
#define DEBUG false // <-print outs
#define LED_BUILTIN 2

#define PIN_OUT 13
#define PIN_IN 14
#define LED_OUT 194
#define LED_IN 48

#define MQTT_SERVER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_SUBTOPIC "kea/guld"
#define MQTT_SNDTOPIC "kea/guldbar"
#define DEVICE_NAME "rlt1demo"

// VARIABLES and CONSTANTS
const byte buf_max_size = 100; // change for longer commands

byte segment_start = 0;
byte segment_step = 0;
byte segment_runs = 0;
byte segment_stop = 0;

// TIMERS
unsigned long mqtt_time = 0, last_reconn = 0;

// INITIALISATIONS
// WiFi
WiFiClient espClient;
const char* ssid = SECRET_SSID;                   // Secret.h
const char* password = SECRET_PSW;                // Secret.h

// MQTT Setup
PubSubClient mqttClient(espClient);

// LED Strips
Adafruit_NeoPixel strip_out = Adafruit_NeoPixel(LED_OUT, PIN_OUT, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_in = Adafruit_NeoPixel(LED_IN, PIN_IN, NEO_GRB + NEO_KHZ800);

StaticJsonDocument<buf_max_size> com_doc;

// FUNCTIONS
void clearStrip(int strip) { // 1 = outer strip | 2 = inner strip | 12 = both
        if (strip == 1) {
                for (int LED = 0; LED < LED_OUT; LED++) {
                        strip_out.setPixelColor(LED, 0);
                }
                strip_out.show();
        } else if (strip == 2) {
                for (int LED = 0; LED < LED_IN; LED++) {
                        strip_in.setPixelColor(LED, 0);
                }
                strip_in.show();
        } else if (strip == 12) {
                for (int LED = 0; LED < LED_OUT; LED++) {
                        strip_out.setPixelColor(LED, 0);
                }
                for (int LED = 0; LED < LED_IN; LED++) {
                        strip_in.setPixelColor(LED, 0);
                }
                strip_out.show();
                strip_in.show();
        }
}

void RouletteGame(void) {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

        int seg = 0;

        int run_color1 = map(esp_random(),0,UINT32_MAX, 1,255);
        int run_color2 = map(esp_random(),0,UINT32_MAX, 1,255);
        int run_color3 = map(esp_random(),0,UINT32_MAX, 1,255);

        clearStrip(12);

        for (int run = 1; run <= segment_runs + 1; run++) {
                if (run == 1) {
                        seg = segment_start;
                }

                // loop done - reset position, change color
                if (seg > 12 && run != 1) {
                        seg = 1;
                        run_color1 = map(esp_random(),0,UINT32_MAX, 1,255);
                        run_color2 = map(esp_random(),0,UINT32_MAX, 1,255);
                        run_color3 = map(esp_random(),0,UINT32_MAX, 1,255);
                }

                if (DEBUG) Serial.println(seg);

                switch (seg)
                {
                case 1:
                        for (int LED = 194; LED >= 178; LED--) {
                                strip_out.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(LED-179, 0);
                        }
                        for (int LED = 48; LED >= 44; LED--) {
                                strip_in.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_in.setPixelColor(LED-45, 0);
                        }
                        break;
                case 2:
                        for (int LED = 178; LED >= 162; LED--) {
                                strip_out.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(LED+16, 0);
                        }
                        for (int LED = 44; LED >= 40; LED--) {
                                strip_in.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_in.setPixelColor(LED+4, 0);
                        }
                        break;
                case 3:
                        for (int LED = 162; LED >= 146; LED--) {
                                strip_out.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(LED+16, 0);
                        }
                        for (int LED = 40; LED >= 36; LED--) {
                                strip_in.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_in.setPixelColor(LED+4, 0);
                        }
                        break;
                case 4:
                        for (int LED = 146; LED >= 130; LED--) {
                                strip_out.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(LED+16, 0);
                        }
                        for (int LED = 36; LED >= 32; LED--) {
                                strip_in.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_in.setPixelColor(LED+4, 0);
                        }
                        break;
                case 5:
                        for (int LED = 130; LED >= 114; LED--) {
                                strip_out.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(LED+16, 0);
                        }
                        for (int LED = 32; LED >= 28; LED--) {
                                strip_in.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_in.setPixelColor(LED+4, 0);
                        }
                        break;
                case 6:
                        for (int LED = 114; LED >= 98; LED--) {
                                strip_out.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(LED+16, 0);
                        }
                        for (int LED = 28; LED >= 24; LED--) {
                                strip_in.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_in.setPixelColor(LED+4, 0);
                        }
                        break;
                case 7:
                        for (int LED = 98; LED >= 82; LED--) {
                                strip_out.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(LED+16, 0);
                        }
                        for (int LED = 24; LED >= 20; LED--) {
                                strip_in.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_in.setPixelColor(LED+4, 0);
                        }
                        break;
                case 8:
                        for (int LED = 82; LED >= 66; LED--) {
                                strip_out.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(LED+16, 0);
                        }
                        for (int LED = 20; LED >= 16; LED--) {
                                strip_in.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_in.setPixelColor(LED+4, 0);
                        }
                        break;
                case 9:
                        for (int LED = 66; LED >= 50; LED--) {
                                strip_out.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(LED+16, 0);
                        }
                        for (int LED = 16; LED >= 12; LED--) {
                                strip_in.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_in.setPixelColor(LED+4, 0);
                        }
                        break;
                case 10:
                        for (int LED = 50; LED >= 34; LED--) {
                                strip_out.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(LED+16, 0);
                        }
                        for (int LED = 12; LED >= 8; LED--) {
                                strip_in.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_in.setPixelColor(LED+4, 0);
                        }
                        break;
                case 11:
                        for (int LED = 34; LED >= 18; LED--) {
                                strip_out.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(LED+16, 0);
                        }
                        for (int LED = 8; LED >= 4; LED--) {
                                strip_in.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_in.setPixelColor(LED+4, 0);
                        }
                        break;
                case 12:
                        for (int LED = 18; LED >= 0; LED--) {
                                strip_out.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(LED+16, 0);
                        }
                        for (int LED = 4; LED >= 0; LED--) {
                                strip_in.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_in.setPixelColor(LED+4, 0);
                        }
                        break;
                }

                strip_out.show();
                strip_in.show();

                seg++;
                if (DEBUG) {
                        Serial.print("run: ");
                        Serial.println(run);
                        Serial.print("segReal: "); Serial.println(seg);
                }
                delay(segment_step);
        }
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // DEBUG
}

void WiFiSetup(void) {
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
        if (DEBUG) {
                Serial.print("Connecting to: ");
                Serial.println(ssid);
        }

        while (WiFi.waitForConnectResult() != WL_CONNECTED) {
                if (DEBUG) Serial.print(".");
        }

        if (DEBUG) {
                Serial.println("");
                Serial.print("Connected to ");
                Serial.println(ssid);
                Serial.print("IP address: ");
                Serial.println(WiFi.localIP());
        }
}

void callback(char* topic, byte* payload, unsigned int length) {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

        DeserializationError err = deserializeJson(com_doc, payload, length);
        if (err) {
                if (DEBUG) Serial.println(">deserialization error");
                return;
        } else {
                if (DEBUG) {
                        Serial.println(">cmd start");
                        serializeJsonPretty(com_doc, Serial);
                }

                segment_start = com_doc["at"]["strt"];
                segment_step = com_doc["at"]["t"];
                segment_runs = com_doc["at"]["r"];
                segment_stop = com_doc["at"]["stp"];

                RouletteGame();
        }
}

boolean MQTTsetup(void) {
        mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
        mqttClient.setBufferSize(buf_max_size);
        mqttClient.setKeepAlive(30);
        mqttClient.setCallback(callback);

        if (mqttClient.connect(DEVICE_NAME)) {
                mqttClient.subscribe(MQTT_SUBTOPIC);
                mqttClient.publish(MQTT_SNDTOPIC, "rlt1demo conn");
                if (DEBUG) Serial.println("MQTT conn");
        }
        return mqttClient.connected();
}

void MQTTloop () {
        // Connection loss
        if (!mqttClient.connected()) {
                if (millis() >= last_reconn + 3000) {
                        last_reconn = millis();
                        if (MQTTreconnect()) {
                                // Sucesfull reconnect
                                last_reconn = 0;
                        }
                }
        } else {
                // Connected - keep alive
                if (millis() >= mqtt_time + 1000) {
                        mqtt_time = millis();
                        mqttClient.loop();
                }
        }
}

boolean MQTTreconnect(void) {
        if (mqttClient.connect(DEVICE_NAME)) {
                mqttClient.subscribe(MQTT_SUBTOPIC);
                mqttClient.publish(MQTT_SNDTOPIC, "rlt1demo reconn");
        }
        return mqttClient.connected();
}

void setup(void) {
        Serial.begin(115200);
        Serial.println("SETUP"); // DEBUG
        pinMode(LED_BUILTIN, OUTPUT);

        WiFiSetup();
        MQTTsetup();

        strip_out.begin();
        strip_in.begin();

        clearStrip(12);
}


void loop(void) {
        MQTTloop();
}
