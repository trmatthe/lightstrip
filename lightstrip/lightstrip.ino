/*
LightStrip v0.2

*/

#define dbg  //Serial.print
#define dbgln //Serial.println

// define output pins as LED colours
const int rLED=3;
const int gLED=4;
const int bLED=5;

// value of 0 skips bias adjust. Value is % of full intensity when set to 'max'
const float rAdj=0;
const float gAdj=0.80;
const float bAdj=0.65;

// autoset the bias adjust bools
const boolean rBias=(rAdj != 0) ? true : false;
const boolean gBias=(gAdj != 0) ? true : false;
const boolean bBias=(bAdj != 0) ? true : false;

// define fixed colours.
const unsigned long RED=0xff0000UL;
const unsigned long GREEN=0x00ff00UL;
const unsigned long BLUE=0x0000ffUL;
const unsigned long PURPLE=0xff00ffUL;
const unsigned long CYAN=0x00ffffUL;
const unsigned long YELLOW=0xffff00UL;
const unsigned long WHITE=0xffffffUL;
const unsigned long BLACK=0x000000UL;

// define sequence modes
const int mFADEW=1;
const int mFADEB=2;
const int mBLEND=4;
const int mSTROBE=8;
const int mSTROBEB=16;
const int mSTROBEW=32;

const int transitionSteps=32; // Number of steps in fades/blends

// define masks for extracting colour bytes from triple
const unsigned long maskRED=0xff0000UL;
const unsigned long maskGREEN=0x00ff00UL;
const unsigned long maskBLUE=0x0000ffUL;

// define some basic colour_arrays
unsigned long caRAINBOW[]={RED, YELLOW, GREEN, BLUE, PURPLE};
unsigned long caRB[]={RED, BLUE};
unsigned long caWRWGWB[]={WHITE, RED, WHITE, GREEN, WHITE, BLUE};
unsigned long caXRXGXB[]={BLACK, RED, BLACK, GREEN, BLACK, BLUE};
unsigned long caRGB[]={RED, GREEN, BLUE};
unsigned long caWHITE[]={WHITE};

// code!

// set LED connected pins as outputs.
void setup() {
  for (int i=0; i<3; i++)
    pinMode(rLED+i, OUTPUT);
  
  // setup serial for debug messages
  Serial.begin(9600);
}

void loop() {
//  showColour(0xffffff);
  showSequence(caRAINBOW, 5, 20, 30, mBLEND);
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
  if (rBias)
    setLED(rLED, ((colour & maskRED) >> 16) * rAdj);
  else
    setLED(rLED, (colour & maskRED) >> 16);
    
  if (gBias)
    setLED(gLED, ((colour & maskGREEN) >> 8) * gAdj);
  else
    setLED(gLED, ((colour & maskGREEN) >> 8));
    
  if (bBias)
    setLED(bLED, (colour & maskBLUE) * bAdj);
  else
    setLED(gLED, ((colour & maskGREEN) >> 8)); 
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
