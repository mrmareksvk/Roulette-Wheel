/*
   Embedded program Arduino Serial and Light Control
   For Project #2 Roulette wheel - KEA PTI 5th. semester
 */

// LIBRARIES, IMPORTS
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>

// DEFINITIONS
#define PIN_OUT 5
#define PIN_IN 6
#define LED_OUT 194
#define LED_IN 49

#define DEBUG false // <-printouts

// VARIABLES and CONSTANTS
const byte ser_max_size = 100; // change for longer commands
char ser_buffer[ser_max_size];
byte msg_size = 0;
bool msg_complete = false;

byte segment_start = 0;
byte segment_step;
byte segment_runs;
byte segment_stop;

// INITIALISATIONS
Adafruit_NeoPixel strip_out = Adafruit_NeoPixel(LED_OUT, PIN_OUT, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_in = Adafruit_NeoPixel(LED_IN, PIN_IN, NEO_GRB + NEO_KHZ800);

StaticJsonDocument<ser_max_size> com_doc; // stored in STACK

// FUNCTIONS
void clearStrip(int strip) { // 1 == outer strip | 2 == inner strip || 12 == both
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
                strip_out.show();
                for (int LED = 0; LED < LED_IN; LED++) {
                        strip_in.setPixelColor(LED, 0);
                }
                strip_in.show();
        }
}

void outSegmentRoulette(void) {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

        int seg = 0;
        int LEDoutRange = LED_OUT;

        int run_color1 = random(255);
        int run_color2 = random(255);
        int run_color3 = random(255);

        clearStrip(12);

        for (int run = 1; run <= segment_runs + 1; run++) {
                if (run == 1) {
                        seg = segment_start - 1;
                        // Serial.println(seg+1); // DEBUG
                }
                // loop done - reset position, change color
                if (seg >= 12 && run != 1) {
                        seg = 0;
                        run_color1 = random(255);
                        run_color2 = random(255);
                        run_color3 = random(255);
                }

                if (seg == 0) {
                        clearStrip(12);
                        LEDoutRange = LED_OUT;
                } else {
                        LEDoutRange = LED_OUT - (seg * 16);
                }

                for (int LED = LEDoutRange; LED > LEDoutRange -16; LED--) {
                        // Clear old pixels
                        if (LED > LED_OUT - 16) {
                                // PASS - NOCLEAR}
                        } else {
                                strip_out.setPixelColor(LED+16, 0);
                        }
                        strip_out.setPixelColor(LED, strip_out.Color(run_color1, run_color2, run_color3));
                        if (LED == LEDoutRange - 15 && seg == 11) {
                                strip_out.setPixelColor(LED - 1, strip_out.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(LED - 2, strip_out.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(LED - 3, strip_out.Color(run_color1, run_color2, run_color3));
                        }
                }
                strip_out.show();
                seg++;
                if (DEBUG) {
                        Serial.print("run: ");
                        Serial.println(run);
                        Serial.print("segReal: "); Serial.println(seg);
                }
                // if (run == segment_runs + 1) Serial.println(seg); // DEBUG

                delay(segment_step);
        }
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // DEBUG
}

void readSerial(void) {
        while (Serial.available()) {
                char c = Serial.read();
                if (c == '\n') { //  end terminator: LF (line feed "\n")
                        msg_complete = true;
                        msg_size++;
                        break;
                }
                else {
                        if (msg_size < ser_max_size - 1) {
                                ser_buffer[msg_size] = c;
                                msg_size++;
                        }
                        else {
                                // if overflow
                                if (DEBUG) Serial.println("BUFF OVERFLOW");
                        }
                }
        }
}

void handleMessage(void) {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

        if (DEBUG) Serial.println(ser_buffer);

        DeserializationError err = deserializeJson(com_doc, ser_buffer);
        if (err) {
                if (DEBUG) Serial.println(">deserialization error");

                msg_size = 0;
                msg_complete = false;
                return;
        } else {
                if (DEBUG) Serial.println(">cmd start");

                segment_start = com_doc["at"]["strt"];
                segment_step = com_doc["at"]["t"];
                segment_runs = com_doc["at"]["r"];
                segment_stop = com_doc["at"]["stp"];

                outSegmentRoulette();

                msg_size = 0;
                msg_complete = false;
        }
}


void setup() {
        Serial.begin(115200); // same speed as on Raspberry
        pinMode(LED_BUILTIN, OUTPUT);

        strip_out.begin();
        strip_in.begin();

        clearStrip(12);

        randomSeed(analogRead(0));
}


void loop() {
        if (Serial.available()) {
                readSerial();
        }

        if (msg_complete) {
                handleMessage();
        }
}
