/***************************************************
Lights+Sound for B9 Robot.
Uses DFPlayer - A Mini MP3 Player For Arduino and FastLED library for addressable RGB lights

****************************************************/

#include "Arduino.h"
#include "FastLED.h"
#include "DFRobotDFPlayerMini.h"


// FastLED related stuff
const uint8_t PROGMEM gamma8[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };
/* gamma8 from https://learn.adafruit.com/led-tricks-gamma-correction/the-quick-fix */

const uint8_t PROGMEM degamma8[] = {
  0,
  20, 28, 33, 38, 42, 46, 49, 52, 55, 58, 61, 63, 65, 68, 70, 72, 
  74, 76, 78, 80, 81, 83, 85, 87, 88, 90, 91, 93, 94, 96, 97, 99,
  100, 102, 103, 104, 106, 107, 108, 109, 111, 112, 113, 114, 115, 117, 118, 119, 
  120, 121, 122, 123, 124, 125, 126, 128, 129, 130, 131, 132, 133, 134, 135, 136, 
  136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 147, 148, 149, 150, 
  151, 152, 153, 153, 154, 155, 156, 157, 158, 158, 159, 160, 161, 162, 162, 163, 
  164, 165, 165, 166, 167, 168, 168, 169, 170, 171, 171, 172, 173, 174, 174, 175, 
  176, 176, 177, 178, 178, 179, 180, 181, 181, 182, 183, 183, 184, 185, 185, 186, 
  187, 187, 188, 189, 189, 190, 190, 191, 192, 192, 193, 194, 194, 195, 196, 196, 
  197, 197, 198, 199, 199, 200, 200, 201, 202, 202, 203, 203, 204, 205, 205, 206, 
  206, 207, 208, 208, 209, 209, 210, 210, 211, 212, 212, 213, 213, 214, 214, 215, 
  216, 216, 217, 217, 218, 218, 219, 219, 220, 220, 221, 222, 222, 223, 223, 224, 
  224, 225, 225, 226, 226, 227, 227, 228, 228, 229, 229, 230, 230, 231, 231, 232, 
  232, 233, 233, 234, 234, 235, 235, 236, 236, 237, 237, 238, 238, 239, 239, 240, 
  240, 241, 241, 242, 242, 243, 243, 244, 244, 245, 245, 246, 246, 247, 247, 248, 
  248, 249, 249, 249, 250, 250, 251, 251, 252, 252, 253, 253, 254, 254, 255};


const uint8_t PROGMEM degamma48[] = {
32, 63, 75, 83, 90, 95, 99, 103, 107, 110, 113, 116, 118, 121, 123, 125, 127, 129, 131, 133, 134, 136, 138, 139, 141, 142, 144, 145, 146, 148, 149, 150, 151, 152, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 167, 168, 169, 170, 171, 172, 172, 173, 174, 175, 176, 176, 177, 178, 179, 179, 180, 181, 181, 182, 183, 183, 184, 185, 185, 186, 187, 187, 188, 189, 189, 190, 190, 191, 192, 192, 193, 193, 194, 194, 195, 195, 196, 197, 197, 198, 198, 199, 199, 200, 200, 201, 201, 202, 202, 203, 203, 204, 204, 205, 205, 206, 206, 207, 207, 208, 208, 208, 209, 209, 210, 210, 211, 211, 212, 212, 212, 213, 213, 214, 214, 215, 215, 215, 216, 216, 217, 217, 217, 218, 218, 219, 219, 219, 220, 220, 221, 221, 221, 222, 222, 222, 223, 223, 224, 224, 224, 225, 225, 225, 226, 226, 226, 227, 227, 228, 228, 228, 229, 229, 229, 230, 230, 230, 231, 231, 231, 232, 232, 232, 233, 233, 233, 234, 234, 234, 235, 235, 235, 235, 236, 236, 236, 237, 237, 237, 238, 238, 238, 239, 239, 239, 239, 240, 240, 240, 241, 241, 241, 242, 242, 242, 242, 243, 243, 243, 244, 244, 244, 244, 245, 245, 245, 246, 246, 246, 246, 247, 247, 247, 247, 248, 248, 248, 249, 249, 249, 249, 250, 250, 250, 250, 251, 251, 251, 251, 252, 252, 252, 252, 253, 253, 253, 253, 254, 254, 254, 255, 
};
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
    int r, g, b;
    int dr, dg, db;
    r=(led_set[setNum][0].r);
    g=(led_set[setNum][0].g);
    b=(led_set[setNum][0].b);

    dr=degamma48[r];
    dg=degamma48[g];
    db=degamma48[b]; 
    
    #if 0
    led_set[setNum][0].r=dr;
    led_set[setNum][0].g=dg;
    led_set[setNum][0].b=db;
    #endif
    char s[100];
    sprintf(s,"%d %d %d %d %d %d", r,g,b,dr,dg,db);
    Serial.println(s);
  }
}

// DFPlayer related stuff

HardwareSerial mySerial(1);  // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);





void setup()
{
  setupLED();
  mySerial.begin(9600,SERIAL_8N1,19,18);
  Serial.begin(115200);
  delay(5000);
  
  if (!myDFPlayer.begin(mySerial)) {
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

void loop()
{
  static unsigned long timer = millis();
  static unsigned long ledtimer = millis();
  static int count=0;
  
  if (millis() - timer > 8000) {
    timer = millis();
    myDFPlayer.next();  //Play next mp3 every 3 second.
    Serial.println(F("next song"));
    
  }
  if (millis() - ledtimer > 100) {
    ledtimer = millis();
    //updateLeds(CHSV(126,255,count%256));
    updateLeds(CHSV(count++%256,250,100));
    FastLED.show();
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
