/***************************************************
Lights+Sound for B9 Robot.
Uses DFPlayer - A Mini MP3 Player For Arduino and FastLED library for addressable RGB lights

****************************************************/

#include "Arduino.h"
#include "FastLED.h"
#include "DFRobotDFPlayerMini.h"
#include <ESP32Servo.h>

// Servo related stuff
Servo servo;
// Published values for SG90 servos; adjust if needed
int minUs = 900;
int maxUs = 2100;
int servoPin = 15;

int pos = 0;      // position in degrees
ESP32PWM pwm;
void setupServo() {
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  servo.setPeriodHertz(50);      // Standard 50hz servo
  servo.attach(servoPin, minUs, maxUs);
}


// FastLED related stuff
#define NUM_LEDS 150  // maximum length per set
#define NUM_SETS 1 
#define DATA_PIN1 21 

CRGB led_set[NUM_SETS+1][NUM_LEDS];
int set_length[NUM_SETS+1]={0 /* dummy */, 20 /* set 1 */};

static long cycle_count=0;
static int red=0;
static int blue=0;
static int green=0;
static int brightness=255;
static int intensity=128;

#define RGBLED_TYPE WS2812B
void setupLED() {
    FastLED.addLeds<RGBLED_TYPE, DATA_PIN1, GRB>(led_set[1], set_length[1]);
    FastLED.setBrightness(brightness);
}


void updateLeds(CHSV color){
  for (int setNum=1; setNum<= NUM_SETS; setNum++) {
    for (int j=set_length[setNum]-1; j; j--) {
      led_set[setNum][j]=led_set[setNum][j-1];
    }
    led_set[setNum][0]=color;  
  }
}

// DFPlayer related stuff
#define DFPLAYER_RX 19
#define DFPLAYER_TX 18
HardwareSerial dfpSerial(1);  // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);


void setupDFPlayer() 
{
  dfpSerial.begin(9600,SERIAL_8N1,DFPLAYER_RX,DFPLAYER_TX);
  delay(5000);
  
  if (!myDFPlayer.begin(dfpSerial)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
      FastLED.show();
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.volume(5);  //Set volume value. From 0 to 30
  delay(500);
  myDFPlayer.volume(5);  //Set volume value. From 0 to 30
  delay(500);
  myDFPlayer.play(1);  //Play the first mp3

}


void setup()
{
  Serial.begin(115200);
  setupServo();
  setupLED();
  setupDFPlayer();
}

void loop()
{
  static unsigned long timer = millis();
  static unsigned long ledtimer = millis();
  static unsigned long servotimer = millis();
  
  static int count=0;
  
  if (millis() - timer > 8000) {
    timer = millis();
    myDFPlayer.next();  //Play next mp3 every 3 second.
    Serial.println(F("next song"));
    
  }
  
  if (millis() - ledtimer > 100) {
    ledtimer = millis();
    //updateLeds(CHSV(126,255,count%256));
    updateLeds(CHSV(count++%256,250,100)); // Hue, Saturation, Value in the range {0,255). 
    FastLED.show();
  }

  if (millis() - servotimer > 1000) {
    servotimer = millis();
    int pos = ((servotimer % 10000)/1000) * 18; // 18 degree steps, every second, repeats every 10 seconds. 10 steps.  180 degree span
    servo.write(pos);    
  }

  
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  
}
