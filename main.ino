#include "IRremote.h"

int receiver = 10; // Signal Pin of IR receiver to Arduino Digital Pin 11
int startFan = -1;

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'

//define the pins used 
#define BLUE 3
#define GREEN 5
#define RED 6
#define ENABLE 11
#define DIRA 9
#define DIRB 12

int i;
int tempPin = 0;

void setup()
{
//set the pins used by the LED as output
pinMode(RED, OUTPUT);
pinMode(GREEN, OUTPUT);
pinMode(BLUE, OUTPUT);


digitalWrite(RED, HIGH);
digitalWrite(GREEN, LOW);
digitalWrite(BLUE, LOW);

pinMode(ENABLE,OUTPUT);
pinMode(DIRA,OUTPUT);
pinMode(DIRB,OUTPUT);
Serial.begin(9600);

irrecv.enableIRIn(); // Start the receiver
}

// define variables
int redValue;
int greenValue;
int blueValue;

// main loop
void loop()
{ 
  int tempReading = analogRead(tempPin);
  // This is OK
  double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
  float tempC = tempK - 273.15;            // Convert Kelvin to Celcius
  float tempF = (tempC * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
  Serial.println("Fan off. Current temp:");
  Serial.println(tempC);
#define delayTime 10 // fading time between colors

if (tempC <= 26 && tempC >= 25) {
  greenValue = 255;
  redValue = 0;
  blueValue = 0;
  analogWrite(RED, redValue);
  analogWrite(GREEN, greenValue);
  analogWrite(BLUE, blueValue);
  delay(delayTime);
}

if (tempC > 26) {
  greenValue = 0;
  redValue = 255;
  blueValue = 0;
  analogWrite(RED, redValue);
  analogWrite(GREEN, greenValue);
  analogWrite(BLUE, blueValue);
  delay(delayTime);
  if (irrecv.decode(&results))  // have we received an IR signal?
  {
    Serial.println("switch");
    switch (results.value)

    {

      case 0xFFA857:        // VOL+ button pressed
        startFan = 1;
        Serial.println("up");
        break;

      case 0xFFE01F:         // VOL- button pressed
        startFan = 0;
        Serial.println("down");
        break;

      default: 
         Serial.println(results.value, HEX);
    }
    delay(1000);
    irrecv.resume();  // receive the next value
  }
  if (startFan == 1) {
    while (tempC > 26) {
      digitalWrite(ENABLE,HIGH); //enable on
      digitalWrite(DIRA,LOW); //one way
      digitalWrite(DIRB,HIGH);
      int tempReading = analogRead(tempPin);
      // This is OK
      double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
      tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
      float tempC = tempK - 273.15;            // Convert Kelvin to Celcius
      float tempF = (tempC * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
      Serial.println("Fan on. Current temp:");
      Serial.println(tempC);
      if (irrecv.decode(&results))  // have we received an IR signal?
      {
        Serial.println("switch");
        switch (results.value)

        {

          case 0xFFA857:        // VOL+ button pressed
            startFan = 1;
            Serial.println("up");
            break;

          case 0xFFE01F:         // VOL- button pressed
            startFan = 0;
            Serial.println("down");
            break;

          default: 
            Serial.println(results.value, HEX);
        }
        delay(1000);
        irrecv.resume();  // receive the next value
      }
      if (startFan == 0) {
        break;
      }
      if (tempC < 26) {
        digitalWrite(ENABLE,LOW);
        startFan = 0;
        break;
      }
    }
    digitalWrite(ENABLE,LOW);
    startFan = 0;
  } else if (startFan == 0) {
    digitalWrite(ENABLE,LOW);
  }
}

if (tempC < 25) {
  greenValue = 0;
  redValue = 0;
  blueValue = 255;
  analogWrite(RED, redValue);
  analogWrite(GREEN, greenValue);
  analogWrite(BLUE, blueValue);
  delay(delayTime);
}
delay(delayTime);
}

