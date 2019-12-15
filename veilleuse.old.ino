int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 5;    // the debounce time; increase if the output flickers


void setup() {
  Serial.begin(9600);
  pinMode(A0,INPUT);
  pinMode(A4,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
}

void loop() {
  int potar = analogRead(A0);//0 volume 0, 1024 max
  int micro = analogRead(A4);
  //int micro = digitalRead(A4);
  int inter = digitalRead(A2);

  //Serial.print("Potar"+potar);
  //Serial.print("Micro"+micro);
  //Serial.print("Inter");
  //Serial.println(inter);
  //Serial.print("Micro");
  Serial.println(micro);
  
  //if(micro==HIGH){
    //Serial.println("Bébé pleure");
  //}
  //delay(1000);
  // read the state of the switch into a local variable:
  int reading = digitalRead(A3);
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

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        Serial.println("Changement mode");
      }
    }
  }
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}
