/*
LightStrip v0.2

*/

#include <IRremote.h>
#include <EEprom.h> 
#include "lightstrip.h"

#define dbg  Serial.print
#define dbgln Serial.println



// value of 1=no bias adjust, < 1 is a scale factor applied to that channel
float rAdj;
float gAdj;
float bAdj;

struct conf_t {
  float rAdj;
  float gAdj;
  float bAdj;
  unsigned long lastColour;
  int lastBrightness;
};  

// increments for colour up/down and brightness up/down
const int colourInc=10;
const int colourDec=-10;
const int brightInc=10;
const int brightDec=-10;


// define sequence modes
const int mFADEW=1;
const int mFADEB=2;
const int mBLEND=4;
const int mSTROBE=8;
const int mSTROBEB=16;
const int mSTROBEW=32;

// Number of steps in fades/blends
const int transitionSteps=32; 


// define some basic colour_arrays
unsigned long caRAINBOW[]={RED, YELLOW, GREEN, BLUE, PURPLE1};
unsigned long caRB[]={RED, BLUE};
unsigned long caWRWGWB[]={WHITE, RED, WHITE, GREEN, WHITE, BLUE};
unsigned long caXRXGXB[]={BLACK, RED, BLACK, GREEN, BLACK, BLUE};
unsigned long caRGB[]={RED, GREEN, BLUE};
unsigned long caWHITE[]={WHITE};


// ir library routines
int IRpin = 2;
IRrecv irrecv(IRpin);
decode_results results;
conf_t myConf;
// code!

// set LED connected pins as outputs.
void setup() {
  for (int i=0; i<3; i++)
    pinMode(rLED+i, OUTPUT);
  irrecv.enableIRIn();
  myConf.rAdj = 1.0;
  myConf.gAdj = 1.0;
  myConf.bAdj = 1.0;
  myConf.lastColour = 0xFFFFFF;
  myConf.lastBrightness = 255;
  unsigned long tmp=showColour(myConf.lastColour);
  rAdj=myConf.rAdj;
  gAdj=myConf.gAdj;
  bAdj=myConf.bAdj;
  Serial.begin(115200);
  dbg("Size of conf struct: ");
  dbgln(sizeof(myConf));
  dbgln("calling readConfig");
 // readConf();
}

//  showColour(0xffffff);
//   showSequence(caRAINBOW, 5, 150, 50, mBLEND);

void loop() {
  
  static unsigned long currColour, r, g, b;
  static int brightness;
  static bool programMode;
  
  if (irrecv.decode(&results)) {
    switch (results.value) {
      case kBRIGHTUP:
      case kBRIGHTDOWN:
      case kPLAY:
      case kPOWER:
        currColour=showColour(BLACK);
        Serial.println("black");        
        break;
      case kRED:
        currColour=showColour(RED);
        Serial.println("red");
        break;
      case kGREEN:
        currColour=showColour(GREEN);
        Serial.println("green");
        break;
      case kBLUE:
        currColour=showColour(BLUE);
        Serial.println("blue");        
        break;
      case kWHITE:
        currColour=showColour(WHITE);
        Serial.println("white");        
        break;
      case kORANGE1:
        currColour=showColour(ORANGE1);
        Serial.println("orange1");        
        break;      
      case kLIGHTGREEN:
        currColour=showColour(LIGHTGREEN);
        Serial.println("lightgreen");        
        break;
      case kROYALBLUE:
        currColour=showColour(ROYALBLUE);
        Serial.println("royalblue");        
        break;
      case kWARMWHITE1:
        currColour=showColour(WARMWHITE1);
        Serial.println("warmwhite1");        
        break;
      case kORANGE2:
        currColour=showColour(ORANGE2);
        Serial.println("orange2");        
        break;
      case kJADE1:
        currColour=showColour(JADE1);
        Serial.println("jade1");        
        break;
      case kPURPLE1:
        currColour=showColour(PURPLE1);
        Serial.println("purple");        
        break;
      case kWARMWHITE2:
        currColour=showColour(WARMWHITE2);
        Serial.println("warmwhite2");        
        break;
      case kAMBER:
        currColour=showColour(AMBER);
        Serial.println("amber");        
        break;
      case kJADE2:
        currColour=showColour(JADE2);
        Serial.println("JADE2");        
        break;
      case kCRIMSON:
        currColour=showColour(CRIMSON);
        Serial.println("crimson");        
        break;
      case kCOOLWHITE1:
        currColour=showColour(COOLWHITE1);
        Serial.println("coolwhite1");        
        break;
      case kYELLOW:
        currColour=showColour(YELLOW);
        Serial.println("yellow");        
        break;
      case kJADE3:
        currColour=showColour(JADE3);
        Serial.println("jade3");        
        break;
      case kPURPLE2:
        currColour=showColour(PURPLE2);
        Serial.println("purple2");        
        break;
      case kCOOLWHITE2:
        currColour=showColour(COOLWHITE2);
        Serial.println("coolwhite2");        
        break;
      case kREDUP:
        currColour=showColour(stepColour(currColour, colourInc, 0, 0));
        break;
      case kGREENUP:
        currColour=showColour(stepColour(currColour, 0, colourInc, 0));
        break;
      case kBLUEUP:
        currColour=showColour(stepColour(currColour, 0, 0, colourInc));
        break;
      case kQUICK:
      case kREDDOWN:
        currColour=showColour(stepColour(currColour, colourDec, 0, 0));
        break;
      case kGREENDOWN:
        currColour=showColour(stepColour(currColour, 0, colourDec, 0));
        break;      
      case kBLUEDOWN:
        currColour=showColour(stepColour(currColour, 0, 0, colourDec));
        break;
      case kSLOW:
      case kDIY1:
      case kDIY2:
      case kDIY3:
      case kAUTO:
        if (programMode) {
          dbgln("got kAUTO and programMODE was true");
        } 
      case kDIY4:
      case kDIY5:
      case kDIY6:
      case kFLASH:
      case kJUMP3:
      case kJUMP7:
      case kFADE3:
      case kFADE7:
      default:
      break;
    }
    irrecv.resume();
  }
}

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

// Functions

// Sets one colour channel (rLED, gLED, bLED) to specified value.
//  0 = off, 255 = max intensity
void setLED(int channel, int intensity) {
  analogWrite(channel, intensity);
  return; 
}

// Displays a colour specified by #RRGGBB
//  Returns currently displayed RGB value
unsigned long showColour(unsigned long colour) {
  setLED(rLED, ((colour & maskRED) >> 16) * rAdj);
  setLED(gLED, ((colour & maskGREEN) >> 8) * gAdj);
  setLED(bLED, (colour & maskBLUE) * bAdj);
  return colour;
}

// Cycles through a sequence of colours
//  colourlist: ptr to array of colours as 0xRRGGBB[1..x]
//  numColours: number of elements in the array
//  stepDelay: number of ms to pause between each fade step
//  colourDelay: number of ms to pause after transitioning to a colour and before starting next transition
//  mode: method for transitioning. 
//    mFADEB = colourlist[i] down to 0x000000. 0x000000 up to colourlist[i+1] down to 0x000000...
//    mFADEW = as mFADEB but 0xffffff instead of 0x000000...
//    mBLEND = interpolate between colourlist[i] and colourlist[i+1] over transitionSteps (64) steps
//    mSTROBE = colourlist[i], pause colourDelay ms, colourlist[i+1]...
//    mSTROBEB = as MSTROBE but colourList[i], pause colourDelay ms, 0x000000, pause colourDelay ms, colourlist[i+1]...
//    mSTROBEW = as MSTROBEB but with 0xffffff interspersed
void showSequence(unsigned long *colourList, int numColours, int stepDelay, int colourDelay, int mode) {
  int i, j;
  unsigned long curr, target, temp; // current, target & temp colour values
  int rStep, gStep, bStep; // increments for each channel to reach target colour

  switch (mode) {
    case mBLEND:
      for (i=0; i<numColours; i++) {
        target=colourList[(i+1) % numColours];
        curr=colourList[i];
        rStep=(((target & maskRED) >> 16) - ((curr & maskRED) >> 16)) / transitionSteps;
        gStep=(((target & maskGREEN) >> 8) - ((curr & maskGREEN) >> 8)) / transitionSteps;
        bStep=((target & maskBLUE) - (curr & maskBLUE)) / transitionSteps;
        for (j=0; j<transitionSteps; j++) {
          temp=stepColour(curr, rStep, gStep, bStep);       
          curr=showColour(temp);
          if (stepDelay)
            delay(stepDelay);
        }
        curr=showColour(colourList[(i+1) % numColours]); // BUG: due to int division after 32 steps may not be exact colour
        if (colourDelay)
          delay(colourDelay);  
      }
      break;    
    case mSTROBE:
      for (i=0; i < numColours; i++) {
        curr=showColour(colourList[i]);
        if (colourDelay)
          delay(colourDelay);
      }
      break;
    case mSTROBEB:
      for (i=0; i < numColours; i++) {
        curr=showColour(colourList[i]);
        if (colourDelay)
          delay(colourDelay);
        curr=showColour(BLACK);
        if (colourDelay)
          delay(colourDelay);
      }
      break;    
    case mSTROBEW:
      for (i=0; i < numColours; i++) {
        curr=showColour(colourList[i]);
        if (colourDelay)
          delay(colourDelay);
        curr=showColour(WHITE);
        if (colourDelay)
          delay(colourDelay);
      }
      break;
    case mFADEW:
    temp=0xFFFFFF;
    for (i=0; i < numColours; i++) {
        curr=showColour(colourList[i]);
        if (colourDelay)
          delay(colourDelay);
        curr=showColour(WHITE);
        if (colourDelay)
          delay(colourDelay);
      }
    case mFADEB:
    break;    
  }
}

unsigned long stepColour(unsigned long colour, int rStep, int gStep, int bStep) {
  signed long r, g, b;
  
  r=(colour & maskRED) >> 16;
  g=(colour & maskGREEN) >> 8;
  b=colour & maskBLUE;
  // clamp values to avoid under/overflow
  r=
    r + rStep < 1 ? 0x00UL
    : r+rStep > 255 ? 0xFFUL
    : r+= rStep;
 
  g=
    g + gStep < 1 ? 0x00UL
    : g+gStep > 255 ? 0xFFUL
    : g+= gStep;
 
  b=
    b + bStep < 1 ? 0x00UL
    : b+bStep > 255 ? 0xFFUL
    : b+= bStep;
  return (r << 16) + (g << 8) + b;
}

void readConf () {
  
}
