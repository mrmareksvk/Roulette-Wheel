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
#define LED_OUT 195
#define LED_IN 49

// VARIABLES and CONSTANTS
const byte ser_max_size = 200;
char ser_buffer[ser_max_size]; // change for longer commands
byte msg_size = 0;
bool msg_complete = false;

byte segment_start = 0;
byte segment_stop;
byte segment_step;

// INITIALISATIONS
Adafruit_NeoPixel strip_out = Adafruit_NeoPixel(LED_OUT, PIN_OUT, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_in = Adafruit_NeoPixel(LED_IN, PIN_IN, NEO_GRB + NEO_KHZ800);

StaticJsonDocument<ser_max_size> com_doc; // stored in STACK (DynamicJsonDocument in HEAP)

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
  int s_LEDstart = LED_OUT;
  int seg = 0;
  int runs = 3000 / segment_step;
  int segment_run = 0;

  int run_color1 = random(255);
  int run_color2 = random(255);
  int run_color3 = random(255);

  clearStrip(1);
  //{5 - 12    115 , 7 = 108 , d 12 = 9, last end 12}
  // 11 2 , 40 = 75, 74 / 12 = 6.1, end 2 .
  for (int run = 0; run < runs; run++) {
    if (segment_run == 0) {
      seg = segment_start;
    } else {
      seg = 0;
    }
    for (int segment = seg; segment < 12; segment++) {
      if (segment == 0) {
        s_LEDstart = LED_OUT;
      } else {
        s_LEDstart = LED_OUT - (segment * 16); // 16 ~= # of segment LEDs
      }
      // if segment run = maximum a segment + 1 == segment_end, break - nechod do posledneho foru
      if (segment_run == (runs - 1)) {
        // Serial.println("break");
        return;
      }
      for (int LED = s_LEDstart; LED > s_LEDstart - 16; LED--) {
        // Serial.println(LED); // DEBUG
        strip_out.setPixelColor(LED, strip_out.Color(run_color1, run_color2, run_color3));
        if (LED == s_LEDstart - 15) {
          // do this only in last iteration (substraction error)
          strip_out.setPixelColor(LED - 1, strip_out.Color(run_color1, run_color2, run_color3));
          strip_out.setPixelColor(LED - 2, strip_out.Color(run_color1, run_color2, run_color3));
          strip_out.setPixelColor(LED - 3, strip_out.Color(run_color1, run_color2, run_color3));
        }
      }
      strip_out.show();
      delay(segment_step);
      segment_run++;
    }
  }
}

void readSerial(void) { // TODO - use native ArduinoJson reader
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') { //  end terminator: LF (line feed "\n") / null ("\0")
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
      }
    }
  }
}

void handleMessage(void) {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

  // Serial.println(ser_buffer);
  DeserializationError err = deserializeJson(com_doc, ser_buffer);
  if (err) {
    // Serial.println("deserialization error");
    return;
  } else {
    segment_start = com_doc["att"]["start"];
    segment_stop = com_doc["att"]["stop"];
    segment_step = com_doc["att"]["t"];
    outSegmentRoulette();
  }
}


void setup() {
  Serial.begin(115200); // same speed as Raspberry
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
    msg_complete = false;
    msg_size = 0;
  }

}
