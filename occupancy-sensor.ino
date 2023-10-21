#include <Arduino.h>

const int irSensorleft = 2;
const int irSensorright = 32;
const int ledPinleft = 17;
const int ledPinright = 26;

int irReadingleft;
int irReadingright;

unsigned long currentTimeright = 0;
unsigned long lastReadTimeright = 0;

unsigned long currentTimeleft = 0;
unsigned long lastReadTimeleft = 0;

unsigned int intervalDelay = 1000;

void setup()
{
  Serial.begin(9600);
  pinMode(irSensorleft, INPUT);
  pinMode(irSensorright, INPUT);
  pinMode(ledPinleft, OUTPUT);
  pinMode(ledPinright, OUTPUT);
}
void loop()
{
  irReadingleft = digitalRead(irSensorleft);
  irReadingright = digitalRead(irSensorright);
  if (irReadingright == LOW)
  {
    digitalWrite(ledPinright, HIGH);
    currentTimeright = millis();
    // IR Sensor is noisy so we add a debounce mechanism here
    if (currentTimeright - lastReadTimeright > intervalDelay)
    {
      lastReadTimeright = currentTimeright;

    }
  }
  else
  {
    digitalWrite(ledPinright, LOW);
  }
  if (irReadingleft == LOW)
  {
    digitalWrite(ledPinleft, HIGH);
    currentTimeleft = millis();
    // IR Sensor is noisy so we add a debounce mechanism here
    if (currentTimeleft - lastReadTimeleft > intervalDelay)
    {
      lastReadTimeleft = currentTimeleft;

    }
  }
  else
  {
    digitalWrite(ledPinleft, LOW);
  }

  if(currentTimeleft != 0 && currentTimeright != 0)
  {
    if(currentTimeleft > currentTimeright)
    {
      // Person ist nach links gelaufen
      Serial.println("Links");

    }
    else if (currentTimeleft < currentTimeright)
    {
      // Person ist nach rechts gelaufen
      Serial.println("Rechts");

    }
    currentTimeleft = 0;
    currentTimeright = 0;
  }

}

