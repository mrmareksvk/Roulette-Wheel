    /*
    Neopixel Ring Roulette wheel sketch.
    "Spins" a roulette wheel and lands on a random value.
    I am not 100% satisfied with the aesthetic, but its as close as I can get.
    I would prefer a more realistic spinning to a slow effect with the final numbers slowly dropping onto a value
    similar to the price is right money wheel.
    I tried several approaches, but I can not seem to nail it.
    A linear increment of a delay does not look right

    If anyone can improve upon it,  I would love to see what results they come up with.
    If you make it improve it, let me know at tom[at]damage[dot]cc
    It would be rad to build a full 36+2 position American style roulette wheel out of neopixels!


    Code is based on the adafruit neopixel 'strandtest' code.
    */


    #include <Adafruit_NeoPixel.h>
    long randNumber;  //variable to hold random number value
    int pos = 95;          //used to track position of lit segment


    //──────────────────────┤         User Variables          ├────────────────────────────

    #define PIN 6     //data pin used in for neopixel
    int spins = 10;        //how many times to 'spin before starting to slow down and settle
    #define dir 1  //set to 1 for clockwise rotation, 0 for counter clockwise.

    //───────────────────────────────────────────────────────────────────────


    // Parameter 1 = number of pixels in strip, value at 16 for NeoPixel Ring.
    // Parameter 2 = pin number (most are valid)
    // Parameter 3 = pixel type flags, add together as needed:
    //   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
    //   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
    //   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
    //   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
    Adafruit_NeoPixel strip = Adafruit_NeoPixel(96, PIN, NEO_GRB + NEO_KHZ800);


    //──────────────────────┤       SETUP Function       ├────────────────────────────

    void setup() {
      randomSeed(analogRead(0));  //initialize floating pin (analog pins 1-5) as the seed for the RNG
      strip.begin();
      clr();  //clear the display for good measure.
      strip.show(); // Initialize all pixels to 'off'
      Serial.begin(9600);
    }


    //──────────────────────┤         Main Loop          ├────────────────────────────

    void loop() {

      randNumber=random(0,96);
      Serial.println(randNumber);  //fetch a random number between 1-16

      //call the wheel
               //color used for for odd segments, even segments, and for '0'
      roulette(strip.Color(155,0,0),strip.Color(0,155,0),strip.Color(0,0,0));   
    }


    //──────────────────────┤  Roulette Wheel Function   ├────────────────────────────

    void roulette(uint32_t c,uint32_t d,uint32_t e) {

        //spin the number of times (variable 'spin') at full speed
          for (int i=0; i<96*spins; i++){
               advanceOne(c,d,e, 10);
          }   
         
          //slow down and 'settle' on final number
          for (int i=0; i<(16*25)+randNumber; i++){
               advanceOne(c,d,e, (i*.25)+4);
          }

    //optional press reset button to spin. 
    //while(1);    //wait indefinitely,  press reset button on Arduino to spin again.
      delay(20000);  //reset spins automatically.

    } //end 'roulette' function


    //──────────────────────┤ Advance One Position Function ├────────────────────────────

    //every time this function is called, the led is advanced 1 position on the wheel
    void advanceOne(uint32_t c,uint32_t d,uint32_t e, int wait) {

                //clr();//Clear the display
                //Serial.println();
                  //Serial.println("/advance start")
                  strip.setPixelColor(pos+1 , 0);;
                  if ((pos & 0x01) == 0) {    //check to see if position is an even number
                        
                        strip.setPixelColor(pos, c); //turn on the pixel at position 'pos'
                   } else  //else it is odd
                     {strip.setPixelColor(pos, d);
                   }

                  if (pos == 0) { //or check to see if it is '0'
                     strip.setPixelColor(pos, e);
                   }
                //Serial.println("beforeShow");
                strip.show();
                //Serial.println("afterShow");
                delay(wait);

                if (dir){    //if direction var is set to clockwise
                      pos--;  //decrement the position
                      if(pos<0)  {pos=95;}  //resets running count if < 0}
                }
                    else{
                      pos++;  //increment the position
                      if(pos>95)  {pos=0;}  //resets running count if > 15}
                }
                    //Serial.println("advance end");

    }//end advanceOne function


    //──────────────────────┤ Clear all Pixels Function ├────────────────────────────

    //clears the display
    void clr(){
      for(int i=0; i<96; i++) {
        strip.setPixelColor(i, 0);
        strip.show();
      } 
    }

