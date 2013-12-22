/*
LightStrip v0.2

*/

#include <IRremote.h>


#define dbg  //Serial.print
#define dbgln //Serial.println

// define output pins as LED colours. Not using 3-5 as PWM on a Mega is shared with these pins.
const int rLED=11;
const int gLED=12;
const int bLED=13;

// value of 1=no bias adjust, < 1 is a scale factor applied to that channel
const float rAdj=1;
const float gAdj=0.80;
const float bAdj=0.65;

// define fixed colours. Remote row/column indicated where appropriate i.e. the mushy vague colour buttons :)
const unsigned long RED=0xff0000; // column 1, row 2
const unsigned long ORANGE1=0xFF6600; // column 1, row 3
const unsigned long ORANGE2=0xE06600; // column 1, row 4
const unsigned long AMBER=0xFFA600; // column 1, row 5
const unsigned long YELLOW=0xffff00; // column 1, row 6

const unsigned long GREEN=0x00ff00; //column 2, row 2
const unsigned long LIGHTGREEN=0x73FF7C; // column 2, row 3
const unsigned long JADE1=0x00994C; // column 2, row 4
const unsigned long JADE2=0x009999; // column 2, row 5
const unsigned long JADE3=0x004C99; // column 2, row 6

const unsigned long BLUE=0x0000ff; // column 3, row 2
const unsigned long PURPLE1=0xff00ff; // column 3, row 4
const unsigned long ROYALBLUE=0x3333FF; // column 3, row 3
const unsigned long CRIMSON=0x330019; // column 3, row 5
const unsigned long PURPLE2=0xCC0066; // column 3, row 6

const unsigned long WHITE=0xffffff; // column 4, row 2
const unsigned long WARMWHITE1=0xFFE5CC; // pinky white column 4, row 3
const unsigned long WARMWHITE2=0xFFFFCC; // yellowy white column 4, row 4
const unsigned long COOLWHITE1=0xCCFFFF; // cyany white column 4, row 5
const unsigned long COOLWHITE2=0xCCFFE5; // greeny white column 4, row 6

const unsigned long CYAN=0x00ffff; // remote misses cyan, I like it tho
const unsigned long BLACK=0x000000;

// define sequence modes
const int mFADEW=1;
const int mFADEB=2;
const int mBLEND=4;
const int mSTROBE=8;
const int mSTROBEB=16;
const int mSTROBEW=32;

// Number of steps in fades/blends
const int transitionSteps=32; 

// define masks for extracting colour bytes from triple
const unsigned long maskRED=0xff0000UL;
const unsigned long maskGREEN=0x00ff00UL;
const unsigned long maskBLUE=0x0000ffUL;

// define some basic colour_arrays
unsigned long caRAINBOW[]={RED, YELLOW, GREEN, BLUE, PURPLE1};
unsigned long caRB[]={RED, BLUE};
unsigned long caWRWGWB[]={WHITE, RED, WHITE, GREEN, WHITE, BLUE};
unsigned long caXRXGXB[]={BLACK, RED, BLACK, GREEN, BLACK, BLUE};
unsigned long caRGB[]={RED, GREEN, BLUE};
unsigned long caWHITE[]={WHITE};

// remote keypad codes
const unsigned long kBRIGHTUP = 0xFF3AC5;
const unsigned long kBRIGHTDOWN = 0xFFBA45;
const unsigned long kPLAY = 0xFF827D;
const unsigned long kPOWER = 0xFF02FD;
const unsigned long kRED = 0xFF1AE5;
const unsigned long kGREEN = 0xFF9A65;
const unsigned long kBLUE = 0xFFA25D;
const unsigned long kWHITE = 0xFF22DD;
const unsigned long kORANGE1 = 0xFF2AD5;
const unsigned long kLIGHTGREEN = 0xFFAA55;
const unsigned long kROYALBLUE = 0xFF926D;
const unsigned long kWARMWHITE1 = 0xFF12ED;
const unsigned long kORANGE2 = 0xFF0AF5;
const unsigned long kJADE1 = 0xFF8A75;
const unsigned long kPURPLE1 = 0xFFB24D;
const unsigned long kWARMWHITE2 = 0xFF32CD;
const unsigned long kAMBER = 0xFF38C7;
const unsigned long kJADE2 = 0xFFB847;
const unsigned long kCRIMSON = 0xFF7887;
const unsigned long kCOOLWHITE1 = 0xFFF807;
const unsigned long kYELLOW = 0xFF18E7;
const unsigned long kJADE3 = 0xFF9867;
const unsigned long kPURPLE2 = 0xFF58A7;
const unsigned long kCOOLWHITE2 = 0xFFD827;
const unsigned long kREDUP = 0xFF28D7;
const unsigned long kGREENUP = 0xFFA857;
const unsigned long kBLUEUP = 0xFF6897;
const unsigned long kQUICK = 0xFFE817;
const unsigned long kREDDOWN = 0xFF08F7;
const unsigned long kGREENDOWN = 0xFF8877;
const unsigned long kBLUEDOWN = 0xFF48B7;
const unsigned long kSLOW = 0xFFC837;
const unsigned long kDIY1 = 0xFF30CF;
const unsigned long kDIY2 = 0xFFB04F;
const unsigned long kDIY3 = 0xFF708F;
const unsigned long kAUTO = 0xFFF00F;
const unsigned long kDIY4 = 0xFF10EF;
const unsigned long kDIY5 = 0xFF906F;
const unsigned long kDIY6 = 0xFF50AF;
const unsigned long kFLASH = 0xFFD02F;
const unsigned long kJUMP3 = 0xFF20DF;
const unsigned long kJUMP7 = 0xFFA05F;
const unsigned long kFADE3 = 0xFF609F;
const unsigned long kFADE7 = 0xFFE01F;

// ir library routines
int IRpin = 2;
IRrecv irrecv(IRpin);
decode_results results;

// code!

// set LED connected pins as outputs.
void setup() {
  for (int i=0; i<3; i++)
    pinMode(rLED+i, OUTPUT);
  
  irrecv.enableIRIn();
  
  // setup serial for debug messages
  Serial.begin(115200);
}

//  showColour(0xffffff);
//   showSequence(caRAINBOW, 5, 150, 50, mBLEND);

void loop() {
  
  unsigned long currColour=0x0L;
  int brightness;
  
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
      case kJADE2:
      case kCRIMSON:
      case kCOOLWHITE1:
      case kYELLOW:
      case kJADE3:
      case kPURPLE2:
      case kCOOLWHITE2:
      case kREDUP:
      case kGREENUP:
      case kBLUEUP:
      case kQUICK:
      case kREDDOWN:
      case kGREENDOWN:
      case kBLUEDOWN:
      case kSLOW:
      case kDIY1:
      case kDIY2:
      case kDIY3:
      case kAUTO:
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
