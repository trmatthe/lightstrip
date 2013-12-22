// define output pins as LED colours. Not using 3-5 as PWM on a Mega is shared with these pins.
const int rLED=11;
const int gLED=12;
const int bLED=13;

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

// define masks for extracting colour bytes from triple
const unsigned long maskRED=0xff0000UL;
const unsigned long maskGREEN=0x00ff00UL;
const unsigned long maskBLUE=0x0000ffUL;

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

