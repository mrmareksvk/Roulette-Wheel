  

    #include <Adafruit_NeoPixel.h>
    //#include <Arduino_JSON>
  


    //──────────────────────┤         User Variables          ├────────────────────────────

    #define PIN_OUT 5
    #define PIN_IN 6     //data pin used in for neopixel
    #define LED_OUT 195
    #define LED_IN 49
    #define DIR 1  //set to 1 for clockwise rotation, 0 for counter clockwise.

    const byte ser_max_size = 200;
    char ser_buffer[ser_max_size]; // change for longer commands
    byte msg_size = 0;
    bool msg_complete = false;
    
    int spins = 10;        //how many times to 'spin before starting to slow down and settle
    int current_out = 0;
  
    int pos = 0;          //used to track position of lit segment


    //───────────────────────────────────────────────────────────────────────


    // Parameter 1 = number of pixels in strip, value at 16 for NeoPixel Ring.
    // Parameter 2 = pin number (most are valid)
    // Parameter 3 = pixel type flags, add together as needed:
    //   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
    //   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
    Adafruit_NeoPixel strip_out = Adafruit_NeoPixel(LED_OUT, PIN_OUT, NEO_GRB + NEO_KHZ800);
    Adafruit_NeoPixel strip_in = Adafruit_NeoPixel(LED_IN, PIN_IN, NEO_GRB + NEO_KHZ800);


    //──────────────────────┤       SETUP Function       ├────────────────────────────

    void setup() {
      pinMode(LED_BUILTIN, OUTPUT);
      strip_out.begin();
      strip_in.begin();
      clr();  //clear the display for good measure.
      clr2();
      strip_out.show(); // Initialize all pixels to 'off'
      strip_in.show();
      Serial.begin(115200);

      //debug
      randomSeed(analogRead(0));

    }


    //──────────────────────┤         Main Loop          ├────────────────────────────

    void loop() {
/*
        if (Serial.available())readSerial();
        if (msg_complete) {
            handleMessage();
            msg_complete = false;
            msg_size = 0;
}
  */  
      testroullette();
    }


    //──────────────────────┤  Roulette Wheel Function   ├────────────────────────────

    void testroullette() 
    {
      int s_LEDstart = 0;
      
      int run_color1 = random(255);
      int run_color2 = random(255);
      int run_color3 = random(255);
      
      clr();
      //{}
      for (int segment = 0; segment < 12;  segment++){
        if (segment == 0) {
          s_LEDstart = 0;
        } else {
          s_LEDstart = segment * 16;
          }  
        for (int LED = s_LEDstart; LED < s_LEDstart + 16; LED ++){
          Serial.println(LED);
          //delay(100);
          strip_out.setPixelColor(LED, strip_out.Color(run_color1, run_color2, run_color3));
          if(LED == s_LEDstart + 15) {
            // do function maybe so one command hidden
            strip_out.setPixelColor(LED+1, strip_out.Color(run_color1, run_color2, run_color3));
            strip_out.setPixelColor(LED+2, strip_out.Color(run_color1, run_color2, run_color3));
            strip_out.setPixelColor(LED+3, strip_out.Color(run_color1, run_color2, run_color3));
            }
        }
        strip_out.show();
        delay(500);
      }
     }
    //──────────────────────┤ Clear all Pixels Function ├────────────────────────────

    //clears the display
    void clr(){
      for(int LED=0; LED<LED_OUT; LED++) {
        strip_out.setPixelColor(LED, 0); 
      } 
      strip_out.show();
      }
      
      void clr2(){
      for(int i=0; i<49; i++) {
        strip_in.setPixelColor(i, 0);
        strip_in.show();
        
      } 
    }

    void handleMessage(void) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    Serial.println(ser_buffer);
}
void readSerial(void) {
while (Serial.available()) {
char c = Serial.read();
if (c == '\n') { // end terminator: LF (line feed) / null ("\0")
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
// Message too long!
}
}
}
}
