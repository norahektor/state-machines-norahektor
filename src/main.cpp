
#include <Arduino.h>

const int BUTTON_PIN = 12;
const int LED_PIN = 10;
const int DEBOUNCE_DELAY = 50;

enum ButtonState{
  UP,
  PRESSED,
  DOWN,
  RELEASED
};

enum SystemState{
  LED_ON,
  LED_OFF
};

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
}

int lastButtonState = HIGH; //knappen ska vara uppe i början (ej nedtryckt)

ButtonState buttonRead() {    //definierar en debouncing-funktion som kallas på när 
                              //knapptryck ska kollas efter

  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {   //kollar om knappens tillstånd har ändrats sen sist (oavsett upp eller ned)
    delay(DEBOUNCE_DELAY);
    reading = digitalRead(BUTTON_PIN);  //kollar igen

    if (reading!= lastButtonState) {    //om knappen efter delay fortfarande har ett ändrat tillstånd
      lastButtonState = reading;        // ändras det tillståndet till lastState för att reset

      if (lastButtonState == LOW) {     //om tillståndet är nedtryckt ska funktionen returnera
                                        // PRESSED --> detta tas hand om i void loop()!
        Serial.println("PRESSED");
        return PRESSED;

      }

      else {                            // om tillståndet inte är nedtryckt (HIGH) ska funktionen 
                                        // returnera RELEASED --> detta tas hand om i void loop()!
        Serial.println("RELEASED");
        return RELEASED;
      }
    }
  }
  //här tar if-satserna SLUT för NÄR KNAPPENS TILLSTÅND HAR ÄNDRATS!

  if (lastButtonState == LOW) {         // om tillståndet inte ändrats och är LOW/HIGH returnerar funktionen
                                        // returneras DOWN respektive HIGH --> tas om hand i loop
    return DOWN;
  }
  else {
    return UP;
  }
}

SystemState state = LED_OFF;            //utgångsläge för vår LEDs state är LED_OFF, dvs LOW

void loop() {

ButtonState buttonState = buttonRead();  //kallar på funktionen som läser av och debouncear knappen!

switch (state) {

  case LED_OFF:
    if (buttonState == PRESSED) {
      Serial.println("Turning on LED"); 
      digitalWrite(LED_PIN, HIGH);
      state = LED_ON;
    }
    break;

  case LED_ON:                          
    if (buttonState == RELEASED) {
      Serial.println("Turning off LED");
      digitalWrite(LED_PIN, LOW);
      state = LED_OFF;
    }
    break;
  }
}