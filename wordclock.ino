//   Outline of clock for bitmapping reference
//
//          0 1 2 3 4 5 6 7 8 9 10
//        0 I T S X A S H A L F H
//        1 T W E N T Y F I V E A
//        2 Q U A R T E R T E N P
//        3 T I L P A S T O N E P
//        4 T W O B I R T H D A Y
//        5 G T H R E E S E V E N
//        6 F O U R F I V E S I X
//        7 D A N I E L L E T E N
//        8 N I N E I G H T M I D
//        9 Z E L E V E N O O N J
//       10 N I G H T O C L O C K
//
#include <Time.h>
#include <TimeLib.h>
#include <Wire.h>
#include <DS3232RTC.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define ROWS 11
#define COLUMNS 11

//---------------------------------------------------------------
//Bit map for each led, 11-rows
uint16_t wordMap[ROWS];   

//Brightness for each individual LED channel
uint16_t redBright, greenBright, blueBright;

//Word Mode -> 0
//DigitMode -> 1
uint16_t mode;

//LED Locations for HAPPY BIRTHDAY DANIELLE
uint16_t birthdayLEDS[20][2] = {{0,10},{1,10},{2,10},{3,10},{4,3},
                                {4,4},{4,5},{4,6},{4,7},{4,8},{4,9},{4,10},
                                {7,0},{7,1},{7,2},{7,3},{7,4},{7,5},{7,6},{7,7}};

//---------------------------------------------------------------

#define PIN 6

#define keyITS       wordMap[0]  |= 0x700                //0b11100000000
#define keyA         wordMap[0]  |= 0x40                 //0b00001000000

#define keyFIVE      wordMap[1]  |= 0x1e                 //0b00000011110
#define keyTEN       wordMap[2]  |= 0xe                  //0b00000001110
#define keyTWENTY    wordMap[1]  |= 0x7e0                //0b11111100000
#define keyQUARTER   wordMap[2]  |= 0x7f0                //0b11111110000
#define keyHALF      wordMap[0]  |= 0x1e                 //0b00000011110

#define keyTIL       wordMap[3]  |= 0x700                //0b11100000000
#define keyPAST      wordMap[3]  |= 0xf0                 //0b00011110000
#define keyOCLOCK    wordMap[10] |= 0x3f                 //0b00000111111

//#define keyHAPPY     wordMap[0]  |= 0x1, wordMap[1]  |= 0x1, wordMap[2]  |= 0x1, wordMap[3]  |= 0x1, wordMap[4]  |= 0x1
//#define keyBIRTHDAY  wordMap[4]  |= 0xff                //0b00011111111
//#define keyDANIELLE  wordMap[7]  |= 0x7f8               //0b11111111000


#define hourONE      wordMap[3]  |= 0xe                 //0b00000001110
#define hourTWO      wordMap[4]  |= 0x700               //0b11100000000
#define hourTHREE    wordMap[5]  |= 0x3e0               //0b01111100000
#define hourFOUR     wordMap[6]  |= 0x780               //0b11110000000
#define hourFIVE     wordMap[6]  |= 0x78                //0b00001111000
#define hourSIX      wordMap[6]  |= 0x7                 //0b00000000111
#define hourSEVEN    wordMap[5]  |= 0x1f                //0b00000011111
#define hourEIGHT    wordMap[8]  |= 0xf8                //0b00011111000
#define hourNINE     wordMap[8]  |= 0x780               //0b11110000000
#define hourTEN      wordMap[7]  |= 0x7                 //0b00000000111
#define hourELEVEN   wordMap[9]  |= 0x3f0               //0b01111110000
#define hourNOON     wordMap[9]  |= 0x1e                //0b00000011110
#define hourMIDNIGHT wordMap[8]  |= 0x7, wordMap[10]  |= 0x7c0       //0b00000000111 , 0b11111000000



//hours  [hourONE,hourTWO,hourTHREE,hourFOUR,hourFIVE,hourSIX,hourSEVEN,hourEIGHT,hourNINE, hourTEN, hourELEVEN, hourNOON, hourMIDNIGHT];


// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(150, PIN, NEO_GRB + NEO_KHZ800);

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(ROWS, COLUMNS, PIN,
      NEO_MATRIX_TOP  + NEO_MATRIX_LEFT +
      NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
      NEO_GRB         + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {


  Serial.begin(9600);
  
  matrix.begin();
  matrix.show();
  mode = 0;
  
//  setTime(0,05,00,11,2,2017);
//  RTC.set(now());
  redBright = 200;
  greenBright = 200;
  blueBright = 200;
  

  

}

void loop() {
    tmElements_t tm;
    RTC.read(tm);
     
    Serial.print(tm.Hour,DEC);
    Serial.print(":");
    Serial.print(tm.Minute,DEC);
    Serial.print(":");
    Serial.print(tm.Second,DEC);
    Serial.print(" ");
    Serial.print(tm.Month,DEC);
    Serial.print("/");
    Serial.print(tm.Day,DEC);
    Serial.print("/");
    Serial.println(1970+tm.Year,DEC);
    
    

//    if(tm.Hour >= 22 || tm.Hour < 5)
//    {
//      redBright = 128;
//      greenBright = 128;
//      blueBright = 128;
//    }
//    else
//    {
//        redBright = 255;
//        greenBright = 255;
//        blueBright = 255;
//    }

    wordTime(tm);

    //wordTest();
    if(mode == 0)
    {
      matrix.show();
    }
    else if(mode == 1)
    {
      showWordMap();
      if(tm.Day == 4 && tm.Month == 7)
      {
          birthday();
      }
    }
    else if(mode == 2)
    {
      showDigitMap(tm);
    }


    Serial.write(27);       // ESC command
    Serial.print("[2J");    // clear screen command
    Serial.write(27);
    Serial.print("[H");     // cursor to home command

      


}

void showDigitMap(tmElements_t tm) 
{
  uint16_t units, tens;
  char unitsc, tensc;

   
  if (tm.Second % 2) 
  {
    units = tm.Minute % 10;
    tens  = tm.Minute / 10;
  } 
  else 
  {
    units = tm.Hour % 10;
    tens  = tm.Hour / 10;
  }
  matrix.clear();

  unitsc = units & 0xff;
  tensc = tens & 0xff;

  
  Serial.print(tens);
  Serial.println(units);
 
  matrix.drawChar( 0, 1, tens, 0xffff, 0x0000,1);
  matrix.drawChar( 6, 1, units, 0xffff, 0x0000,1);
  matrix.show();
}

void birthday()
{
  uint16_t i, j, k;

//  keyHAPPY;
//  keyBIRTHDAY;
//  keyDANIELLE;

  for(j=0; j<256; j++) 
  { // 5 cycles of all colors on wheel
    for(i=0; i < 20; i++) 
    {
        matrix.drawPixel((uint16_t)birthdayLEDS[i][1],(uint16_t)birthdayLEDS[i][0],Wheel(((i * 256 / 20) + j) & 255));
        //strip.setPixelColor(i, );
    }
        matrix.show();
        delay(20);
  }
}

uint16_t Wheel(uint16_t WheelPos) 
{
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) 
  {
    return matrix.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) 
  {
    WheelPos -= 85;
    return matrix.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return matrix.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
  
void showWordMap()
{
  for( uint16_t row = 0; row < ROWS; row++)
  {
    for( byte column = 0; column < COLUMNS; column++)
    {
      bool onOff = bitRead(wordMap[row],COLUMNS-column-1);
      Serial.print(onOff);
      switch(onOff)
      {
        case 0:
              matrix.drawPixel(column,row,0);
              break;
        case 1:
              matrix.drawPixel(column,row,matrix.Color(redBright,greenBright,blueBright));
              break;
      }
    }
    Serial.println();
    
    wordMap[row] = 0;
  }
  Serial.println("------------------------------------");
  
  matrix.show();
}

void wordTime(tmElements_t tm)
{
  keyITS;
  if (tm.Minute <= 2)
  {
    showHour(tm,0);
    if(tm.Hour != 0 && tm.Hour != 12)
    {
      keyOCLOCK;
    }
  }
  else if(tm.Minute > 2 && tm.Minute <= 7)
  {
    keyFIVE;
    keyPAST;
    showHour(tm,0);
  }
  else if(tm.Minute > 7 && tm.Minute <= 12 )  
  {
    keyTEN;
    keyPAST;
    showHour(tm,0);
  }
  else if(tm.Minute > 12 && tm.Minute <= 17)
  {
    keyA;
    keyQUARTER;
    keyPAST;
    showHour(tm,0);
  }
  else if(tm.Minute > 17 && tm.Minute <= 22)
  {
    keyTWENTY;
    keyPAST;
    showHour(tm,0);
  }
  else if(tm.Minute > 22 && tm.Minute <= 27)
  {
    keyTWENTY;
    keyFIVE;
    keyPAST;
    showHour(tm,0);
  }
  else if(tm.Minute > 27 && tm.Minute <= 32)
  {
    keyHALF;
    keyPAST;
    showHour(tm,0);
  }
  else if(tm.Minute > 32 && tm.Minute <= 37)
  {
    keyTWENTY;    
    keyFIVE;
    keyTIL;
    showHour(tm,1);
  }
  else if(tm.Minute > 37 && tm.Minute <= 42)
  {
    keyTWENTY;
    keyTIL;
    showHour(tm,1);
  }
  else if(tm.Minute > 42 && tm.Minute <= 47)
  {
    keyA;
    keyQUARTER;
    keyTIL;
    showHour(tm,1);
  }
  else if(tm.Minute > 47 && tm.Minute <= 52)
  {
    keyTEN;
    keyTIL;
    showHour(tm,1);
  }
  else if(tm.Minute > 52 && tm.Minute <= 57 )
  {
    keyFIVE;
    keyTIL;
    showHour(tm,1);
  }
  else
  {
    showHour(tm,1);
    if(tm.Hour != 23 && tm.Hour != 11)
    {
      keyOCLOCK;
    }        
  }
}

void showHour(tmElements_t tm, bool when)
{
  if(!when)
  { 
    switch(tm.Hour)
    {
      case 0:
          hourMIDNIGHT;
          break;
      case 1:
      case 13:
          hourONE;
          break;
      case 2:
      case 14:
          hourTWO;
          break;
      case 3:
      case 15:
          hourTHREE;
          break;
      case 4:
      case 16:
          hourFOUR;
          break;
      case 5:
      case 17:
          hourFIVE;
          break;
      case 6:
      case 18:
          hourSIX;
          break;
      case 7:
      case 19:
          hourSEVEN;
          break;
      case 8:
      case 20:
          hourEIGHT;
          break;
      case 9:
      case 21:
          hourNINE;
          break;
      case 10:
      case 22:
          hourTEN;
          break;
      case 11:
      case 23:
          hourELEVEN;
          break;
      case 12:
          hourNOON;
          break;
    }
  }
  else
  {
      switch(tm.Hour)
      {
        case 0:
        case 12:
            hourONE;
            break;
        case 1:
        case 13:
            hourTWO;
            break;
        case 2:
        case 14:
            hourTHREE;
            break;
        case 3:
        case 15:
            hourFOUR;
            break;
        case 4:
        case 16:
            hourFIVE;
            break;
        case 5:
        case 17:
            hourSIX;
            break;
        case 6:
        case 18:
            hourSEVEN;
            break;
        case 7:
        case 19:
            hourEIGHT;
            break;
        case 8:
        case 20:
            hourNINE;
            break;
        case 9:
        case 21:
            hourTEN;
            break;
        case 10:
        case 22:
            hourELEVEN;
            break;
        case 11:
            hourNOON;
            break;
        case 23:
            hourMIDNIGHT;
            break;
           
       }
  }
}
