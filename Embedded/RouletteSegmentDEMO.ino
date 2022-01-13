/*
   Embedded program Arduino-Raspberry Serial and Light Control (Roulette)
   For Project #2 Roulette wheel - KEA PTI 5th. semester
 */

// LIBRARIES, IMPORTS
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>

// DEFINITIONS
#define PIN_OUT 5
#define PIN_IN 6
#define LED_OUT 194
#define LED_IN 48

#define DEBUG false  // <-print outs

// VARIABLES and CONSTANTS
const byte ser_max_size = 100; // change for longer commands
char ser_buffer[ser_max_size];
byte msg_size = 0;
bool msg_complete = false;

byte segment_start = 0;
byte segment_step = 0;
byte segment_runs = 0;
byte segment_stop = 0;

// INITIALISATIONS
Adafruit_NeoPixel strip_out = Adafruit_NeoPixel(LED_OUT, PIN_OUT, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_in = Adafruit_NeoPixel(LED_IN, PIN_IN, NEO_GRB + NEO_KHZ800);

StaticJsonDocument<ser_max_size> com_doc;

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
                strip_out.show();
                for (int LED = 0; LED < LED_IN; LED++) {
                        strip_in.setPixelColor(LED, 0);
                }
                strip_in.show();
        }
}

void RouletteGame(void) {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

        int seg = 0;

        int run_color1 = random(255);
        int run_color2 = random(255);
        int run_color3 = random(255);

        clearStrip(12);

        for (int run = 1; run <= segment_runs + 1; run++) {
                if (run == 1) {
                        seg = segment_start;
                }

                // loop done - reset position, change color
                if (seg > 12 && run != 1) {
                        seg = 1;
                        run_color1 = random(255);
                        run_color2 = random(255);
                        run_color3 = random(255);
                }

                if (DEBUG) Serial.println(seg);

                switch (seg)
                {
                        // 8 less
                case 1:
                        for (int LED = 188; LED >= 172; LED--) {
                                strip_out.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                int erasePixel = LED+16;
                                if (erasePixel >= LED_OUT) {
                                        erasePixel -= LED_OUT;
                                }
                                strip_out.setPixelColor(erasePixel, 0);
                        }
                        for (int LED = 46; LED >= 42; LED--) {
                                strip_in.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_in.setPixelColor(LED-43, 0);
                        }
                        strip_in.setPixelColor(47, 0);
                        break;
                case 2:
                        for (int LED = 172; LED >= 156; LED--) {
                                strip_out.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(LED+16, 0);
                        }
                        for (int LED = 42; LED >= 38; LED--) {
                                strip_in.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_in.setPixelColor(LED+4, 0);
                        }
                        break;
                case 3:
                        for (int LED = 156; LED >= 140; LED--) {
                                strip_out.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(LED+16, 0);
                        }
                        for (int LED = 38; LED >= 34; LED--) {
                                strip_in.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_in.setPixelColor(LED+4, 0);
                        }
                        break;
                case 4:
                        for (int LED = 140; LED >= 124; LED--) {
                                strip_out.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(LED+16, 0);
                        }
                        for (int LED = 34; LED >= 30; LED--) {
                                strip_in.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_in.setPixelColor(LED+4, 0);
                        }
                        break;
                case 5:
                        for (int LED = 124; LED >= 108; LED--) {
                                strip_out.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(LED+16, 0);
                        }
                        for (int LED = 30; LED >= 26; LED--) {
                                strip_in.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_in.setPixelColor(LED+4, 0);
                        }
                        break;
                case 6:
                        for (int LED = 108; LED >= 92; LED--) {
                                strip_out.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(LED+16, 0);
                        }
                        for (int LED = 26; LED >= 22; LED--) {
                                strip_in.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_in.setPixelColor(LED+4, 0);
                        }
                        break;
                case 7:
                        for (int LED = 92; LED >= 76; LED--) {
                                strip_out.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(LED+16, 0);
                        }
                        for (int LED = 22; LED >= 18; LED--) {
                                strip_in.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_in.setPixelColor(LED+4, 0);
                        }
                        break;
                case 8:
                        for (int LED = 76; LED >= 60; LED--) {
                                strip_out.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(LED+16, 0);
                        }
                        for (int LED = 18; LED >= 14; LED--) {
                                strip_in.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_in.setPixelColor(LED+4, 0);
                        }
                        break;
                case 9:
                        for (int LED = 60; LED >= 44; LED--) {
                                strip_out.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(LED+16, 0);
                        }
                        for (int LED = 14; LED >= 10; LED--) {
                                strip_in.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_in.setPixelColor(LED+4, 0);
                        }
                        break;
                case 10:
                        for (int LED = 44; LED >= 28; LED--) {
                                strip_out.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(LED+16, 0);
                        }
                        for (int LED = 10; LED >= 6; LED--) {
                                strip_in.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_in.setPixelColor(LED+4, 0);
                        }
                        break;
                case 11:
                        for (int LED = 28; LED >= 12; LED--) {
                                strip_out.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(LED+16, 0);
                        }
                        for (int LED = 6; LED >= 2; LED--) {
                                strip_in.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_in.setPixelColor(LED+4, 0);
                        }
                        break;
                case 12:
                        for (int LED = 12; LED >= 0; LED--) {
                                strip_out.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(LED+16, 0);
                        }
                        for (int LED = 194; LED >= 188; LED--) {
                                strip_out.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_out.setPixelColor(194-LED+9, 0);
                        }
                        for (int LED = 2; LED >= 0; LED--) {
                                strip_in.setPixelColor(LED, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_in.setPixelColor(LED+4, 0);
                        }
                                strip_in.setPixelColor(48, strip_in.Color(run_color1, run_color2, run_color3));
                                strip_in.setPixelColor(47, strip_in.Color(run_color1, run_color2, run_color3));
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
                                if (DEBUG) Serial.println("BUFF OVERFLOW");
                                msg_size = 0;
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

                RouletteGame();

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
