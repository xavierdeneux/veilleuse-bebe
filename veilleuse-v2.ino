#include <Adafruit_NeoPixel.h>
#define PIN 3
#define N_LEDS 38
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

#include <avr/sleep.h>//this AVR library contains the methods that controls the sleep modes
#define interruptPin 2 //Pin we are going to use to wake up the Arduino


#define RAINBOW_MODE 3
#define CHASE_MODE 2
#define SLEEP_MODE 5
#define HEART_MODE 1
#define RANDOM_MODE 4

#define DEBUG_MODE_ENABLED true

int modeNumber = 0;
int startedMode = 0;

int buttonState;             // the current reading from the input pin
int lastButtonState = HIGH;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers



void setup() {
  if(DEBUG_MODE_ENABLED){
    Serial.begin(115200);//Start Serial Comunication
  }
  strip.begin();
  strip.setBrightness(60);
  
  pinMode(interruptPin,INPUT_PULLUP);//Set pin d2 to input using the buildin pullup resistor
    
}

void loop() { 
  randomColors(); 
  switchMode();
}

void startup(){
  modeNumber = 1;
  
}

void switchMode() {
 buttonPushed(); 

 if(DEBUG_MODE_ENABLED){
    Serial.println("Mode "+String(modeNumber) + " / "+String(startedMode));
 }
 
 if(modeNumber == SLEEP_MODE){
    Going_To_Sleep();
 }


 if(modeNumber == RAINBOW_MODE){
  startedMode = RAINBOW_MODE;
  rainbowCycle(20);
 }

 if(modeNumber == CHASE_MODE) {
  startedMode = CHASE_MODE;
  chase(128);
 }

 if(modeNumber == HEART_MODE) {
  startedMode = HEART_MODE;
  heart();
 }

 if(modeNumber == RANDOM_MODE) {
  startedMode = RANDOM_MODE;
  heart();
 }
}

void buttonPushed() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(interruptPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        if(DEBUG_MODE_ENABLED){
          Serial.println("Button pushed");
        }
        modeNumber++;
      }
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}

void Going_To_Sleep(){
    strip.fill(255);
    showStrip();
    delay(500);
    
    sleep_enable();//Enabling sleep mode
    attachInterrupt(0, wakeUp, LOW);//attaching a interrupt to pin d2
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);//Setting the sleep mode, in our case full sleep

    strip.fill(0);
    showStrip();
    
    sleep_cpu();//activating sleep mode
  }

void wakeUp(){
    sleep_disable();//Disable sleep mode
    startup();
    detachInterrupt(0); //Removes the interrupt from pin 2;
}




/*LED STRIP FUNCTIONS*/
static void randomColors(){
  if(modeNumber != RANDOM_MODE){
    return;
  }
  
  int led = random(1,N_LEDS);
  int red = random(0,255);
  int green = random(0,255);
  int blue = random(0,255);
  int timer = random(30,100);
  setPixel(led, red, green, blue);
  showStrip();
  delay(timer);
  
}

static void heart(){
  if(modeNumber != HEART_MODE){
    return;
  }

  int timeTransition = 20;
  
  for(int i = 0;i<=255;i++){
    // Gestion du mode
    buttonPushed();
    if(modeNumber != HEART_MODE){
      break;  
    }

    setAll(0,0,i);
    showStrip();
    delay(timeTransition);
  }

  for(int i = 254;i>=0;i--){
    // Gestion du mode
    buttonPushed();
    if(modeNumber != HEART_MODE){
      break;  
    }
    
    setAll(0,0,i);
    showStrip();
    delay(5);
  }
  
  //strip.setBrightness(60);
  
}

static void chase(uint32_t c) {
  if(modeNumber != CHASE_MODE){
    return;
  }
  for(uint16_t i=0; i<strip.numPixels()+4; i++) {
      // Gestion du mode
      for(int a = 0;a<6;a++){
      buttonPushed();
      if(modeNumber != CHASE_MODE){
        break;  
      }
      delay(50);
      }
     
    
      strip.setPixelColor(i  , c); // Draw new pixel
      strip.setPixelColor(i-4, 0); // Erase pixel a few steps back
      strip.show();
      
  }
}

void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H 
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < strip.numPixels(); i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}

// MODE 1
void rainbowCycle(int SpeedDelay) {
  if(modeNumber != RAINBOW_MODE){
    return;
  }
  byte *c;
  uint16_t i, j;
  
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    
    // Gestion du mode
    buttonPushed();
    if(modeNumber != RAINBOW_MODE){
      break;  
    }
    for(i=0; i< strip.numPixels(); i++) {
      
      c=Wheel(((i * 256 / strip.numPixels()) + j) & 255);
      setPixel(i, *c, *(c+1), *(c+2));
    }
    showStrip();
    delay(SpeedDelay);
  }
}

byte * Wheel(byte WheelPos) {
  static byte c[3];
  
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;
}
