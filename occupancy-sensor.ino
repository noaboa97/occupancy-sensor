#include <Arduino.h>
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip

// USB-C Port facing towards you for left / right 

// Defining the variables for the pins of the components
const int irSensorleft = 2;
const int irSensorright = 32;
const int ledPinleft = 17;
const int ledPinright = 26;

// Defining the IR sensor reading variables
int firstirReadingleft = 0;
int irReadingleft;
int firstirReadingright = 0;
int irReadingright;

// Defining the the variables for time stuff
unsigned long currentTimeright = 0;
unsigned long lastReadTimeright = 0;

unsigned long currentTimeleft = 0;
unsigned long lastReadTimeleft = 0;

unsigned long currentTime = 0;

// Defining the variables for the delay
unsigned int intervalDelay = 1000;
unsigned int sensorTriggerDelay = 1000;

int roomCountLeft = 0;
int roomCountRight = 0;

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h





void setup()
{
  // Serial console baud setting
  Serial.begin(9600);

  // PinMode Settings 
  pinMode(irSensorleft, INPUT);
  pinMode(irSensorright, INPUT);
  pinMode(ledPinleft, OUTPUT);
  pinMode(ledPinright, OUTPUT);

  tft.begin();               // Initialise the display
  tft.fillScreen(TFT_BLACK); // Black screen fill
  tft.setRotation(1);

  // tft.drawString("------------------------------",0,60);
  tft.drawLine(0, 70, 240, 70, TFT_WHITE);
  tft.setTextSize(5);
}
void loop()
{
  // Reading the input of the IR sensor
  irReadingleft = digitalRead(irSensorleft);
  irReadingright = digitalRead(irSensorright);

  // If IR sensor detects motion
  if (irReadingright == LOW)
  {


    // Capture time of the right IR Sensor when it was triggered
    currentTimeright = millis();

    // Loop is so fast so we define a timespan of X for one motion detection
    if (currentTimeright - lastReadTimeright > intervalDelay)
    {
      lastReadTimeright = currentTimeright;
      firstirReadingright = 1;
    }
  }
  else // isReadingright == HIGH
  {
    // Turn off the LED
    digitalWrite(ledPinright, LOW);
  }

  // If IR sensor detects motion
  if (irReadingleft == LOW)
  {


    // Capture time of the right IR Sensor when it was triggered
    currentTimeleft = millis();
    // IR Sensor is noisy so we add a debounce mechanism here
    if (currentTimeleft - lastReadTimeleft > intervalDelay)
    {
      lastReadTimeleft = currentTimeleft;
      firstirReadingright = 1;
    }
  }
  else // isReadingright == HIGH
  {
    // Turn off the LED
    digitalWrite(ledPinleft, LOW);
  }

  // get current time in the loop
  currentTime = millis();

  // Check if it's the first reading 
  if(firstirReadingleft == 1 || firstirReadingright == 1)
  {
    // Left sensor was triggered last and the left sensor was trigger X ms after the right one
    if(lastReadTimeleft > lastReadTimeright && lastReadTimeright + sensorTriggerDelay > lastReadTimeleft)
    {
      // Person walked left
      Serial.println("Left");
      roomCountLeft++;
      //tft.drawString(roomCountLeft,60,120);
      tft.drawNumber(roomCountLeft,120,90);

      // Turn on the LED
      digitalWrite(ledPinleft, HIGH);

      if (roomCountRight > 0)
      {
        roomCountRight--;
        tft.drawNumber(roomCountRight,120,25);
      }

    }
    // Right sensor was triggered last and the right sensor was triggered X ms after the left
    else if (lastReadTimeleft < lastReadTimeright && lastReadTimeleft + sensorTriggerDelay > lastReadTimeright)
    {
      // Person walked right
      Serial.println("Right");
      roomCountRight++;
      //tft.drawString();
      tft.drawNumber(roomCountRight,120,25);
      // Turn on the LED
      digitalWrite(ledPinright, HIGH);


      if (roomCountLeft > 0)
      {
        roomCountLeft--;
        tft.drawNumber(roomCountLeft,120,90);
      }

    }

  }

  // Used only in the first run of the loop after sensor has detected motion otherwise should be 0 because it's never set to 1 
  firstirReadingleft = 0;
  firstirReadingright = 0;



}

