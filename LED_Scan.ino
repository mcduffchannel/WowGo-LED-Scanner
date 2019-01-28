/*=============================================
WOWGO ESC v1.5 LED Scanner
CODE BASHED BY MCDUFFCHANNEL
Intended for an ATTiny85

Project URL: https://www.mcduffchannel.com/441683542
Email: mcduffchannel@gmail.com
=============================================*/

/* Circuit uses approx ~0.4Amp during scan upto ~1.0Amp with brake applied */


#include <Adafruit_NeoPixel.h>

/* Pin used for the brake signal.
NOTE: that the brake signal should be LOW when applied and HIGH what not applied. */

#define BRAKELINE 3 

/* Pin used to send the signal to the LED Strip.
NOTE: I used a LUMINOSUM individually addressable LED strip from AMAZON. */
#define PIXELPIN 0

/* Number of LED's used in each array per side NOT total LED's */
#define NUMPIXELS 28

/* Speed in milliseconds the Brake light envelops the board */
#define BRKSPEED 10

/* The slowest you want the scan to be as well as the fastest
NOTE: lower the number, the faster */
#define MINSPEED 65
#define MAXSPEED 25

/* Define the RGB value of the scan
NOTE: Default set to white light  */
#define RD_VAL 255
#define GN_VAL 255
#define BU_VAL 255

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXELPIN, NEO_GRB + NEO_KHZ800);

int y = 0;
int pinLvl;
int scanSpeed;

void setup() {
  pixels.begin();
  pinMode(BRAKELINE, INPUT);
}

void loop() {
   
  for (int i=4; i > -4; i--) {
    clrScan();  // Clears all LED values
    wtScan(i); // Sets scan pattern offset by the loop value

    // Reads input from throttle and defines speed of scan based of earlier definitions
    pinLvl = 1023 - analogRead(A1);
    scanSpeed = (float)MAXSPEED + ((((float)MINSPEED - float(MAXSPEED)) / (float)1023) * (float)pinLvl);
    delay(scanSpeed);
    
    pixels.show();

 go_again:
    if(digitalRead(BRAKELINE) == LOW){
      rdScan();
    
      for (int x=y; x > -1; x--){
        wtScan(x);
        delay(BRKSPEED);
        pixels.show();
        if(digitalRead(BRAKELINE) == LOW){
        goto go_again;
        }
      }
    }     
  }
}


void wtScan(int i){
    pixels.setPixelColor(i+25, pixels.Color(RD_VAL, GN_VAL, BU_VAL));
    pixels.setPixelColor(i+24, pixels.Color(RD_VAL, GN_VAL, BU_VAL));

    pixels.setPixelColor(i+17, pixels.Color(RD_VAL, GN_VAL, BU_VAL));
    pixels.setPixelColor(i+16, pixels.Color(RD_VAL, GN_VAL, BU_VAL));
  
    pixels.setPixelColor(i+9, pixels.Color(RD_VAL, GN_VAL, BU_VAL));
    pixels.setPixelColor(i+8, pixels.Color(RD_VAL, GN_VAL, BU_VAL));

    pixels.setPixelColor(i+1, pixels.Color(RD_VAL, GN_VAL, BU_VAL));
    pixels.setPixelColor(i+0, pixels.Color(RD_VAL, GN_VAL, BU_VAL));      
}

void clrScan(){
  for (int i=0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
}

void rdScan(){
    for (int i=y; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(RD_VAL, GN_VAL, BU_VAL));
    }
  
    for (int x=0; x < NUMPIXELS; x++){
      pixels.setPixelColor(x, pixels.Color(255, 0, 0));
      pixels.show();
      y = x;
      delay(BRKSPEED);
      
      if(digitalRead(BRAKELINE) == HIGH){
         return;
      }
    }
    
    while(digitalRead(BRAKELINE) == LOW){
      //Pauses code while the brake is applied
    }
}

