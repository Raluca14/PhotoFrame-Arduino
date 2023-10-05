#include <Wire.h>
#include <AHTxx.h>
#include <Adafruit_GFX.h>    // include Adafruit graphics library

#include <SPI.h>             // include Arduino SPI library
#include <SD.h>              // include Arduino SD library

#include <Adafruit_ST7735.h> // Hardware-specific library

float ahtValue;                               //to store T/RH result

AHTxx aht20(AHTXX_ADDRESS_X38, AHT2x_SENSOR); //sensor address, sensor type

// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define TFT_CS     10
#define TFT_RST    9  // you can also connect this to the Arduino reset
                      // in which case, set this #define pin to 0!
#define TFT_DC     8

#define button   2  // button pin
#define pink   7  // button pin
#define black   6  // button pin
#define icey   5  // button pin
#define LED A1

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

void setup()
{
 pinMode(LED, OUTPUT);
  
  pinMode(button, INPUT_PULLUP);
  pinMode(pink, INPUT_PULLUP);
  pinMode(black, INPUT_PULLUP);
  pinMode(icey, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println();
  
  while (aht20.begin() != true)
  {
    Serial.println(F("AHT2x not connected or fail to load calibration coefficient")); //(F()) save string to flash & keeps dynamic memory free

    delay(5000);
  }

  Serial.println(F("AHT20 OK"));

  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  Serial.println("Initialized screen");
  uint16_t time = millis();
  tft.fillScreen(ST7735_BLACK);
  time = millis() - time;

  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("failed!");
    while(1);  // stay here
  }
  Serial.println("OK!");
 
  File root = SD.open("/");  // open SD card main root
  printDirectory(root, 0);   // print all files names and sizes
  root.close();              // close the opened root


}

int a=0;
int filtru=0;
void loop()
{
  File root = SD.open("/");  // open SD card main root
 

  if(a==1) 
  {  digitalWrite(LED, HIGH);
     tft.fillScreen(ST7735_BLACK);
 

  ahtValue = aht20.readTemperature(); //read 6-bytes via I2C, takes 80 milliseconds

  Serial.print(F("Temperature...: "));
  tft.setFont();
 // tft.setCursor(10,5);  // (x,y)
  tft.setTextSize(1);
 
  tft.setCursor(20, 10);
  tft.println("Temperatura:");  // Text or value to print
  
  if (ahtValue != AHTXX_ERROR) //AHTXX_ERROR = 255, library returns 255 if error occurs
  {
    Serial.print(ahtValue);
    tft.setTextSize(3);
    tft.setCursor(0, 50);
    tft.print(ahtValue);
    tft.print(" C");
     tft.setTextSize(1);
 

    Serial.println(F(" +-0.3C"));
  }
  else
  {
    printStatus(); //print temperature command status

    if   (aht20.softReset() == true) Serial.println(F("reset success")); //as the last chance to make it alive
    else                             Serial.println(F("reset failed"));
  }

  //delay(2000); //measurement with high frequency leads to heating of the sensor, see NOTE

  ahtValue = aht20.readHumidity(); //read another 6-bytes via I2C, takes 80 milliseconds

  Serial.print(F("Humidity......: "));
  
  tft.setTextSize(1);
 
  tft.setCursor(20, 80);
  tft.println("Umiditate:");
  if (ahtValue != AHTXX_ERROR) //AHTXX_ERROR = 255, library returns 255 if error occurs
  {
    Serial.print(ahtValue);
    tft.setTextSize(3);
    tft.setCursor(0, 100);
    tft.print(ahtValue);
    
     tft.setTextSize(1);
     

    Serial.println(F(" +-0.3C"));
  }
  else
  {
    printStatus(); //print humidity command status
  }


  

  delay(5000); //recomended polling frequency 8sec..30sec
  if(digitalRead(button)== LOW) {
     a=0;
     delay(500);
     digitalWrite(LED, LOW);
 
   }  
  }
  //DERULARE IMAGINI
  else{
    //digitalWrite(LED, HIGH);
    tft.fillScreen(ST7735_BLACK);
    while (true) {
      
      if(digitalRead(pink)== LOW) {
          
            if(filtru==1) filtru=0;
            else filtru=1;
            delay(500);
            
      } 
      if(digitalRead(black)== LOW) {
         
            if(filtru==2) filtru=0;
            else filtru=2;
            delay(500);
            
      } 
      if(digitalRead(icey)== LOW) {
          
            if(filtru==3) filtru=0;
            else filtru=3;
            delay(500);
            
      }    
    File entry =  root.openNextFile();  // open file
 
    if (! entry) {
      // no more files
      root.close();
      return;
    }
 
    uint8_t nameSize = String(entry.name()).length();  // get file name size
    String str1 = String(entry.name()).substring( nameSize - 4 );  // save the last 4 characters (file extension)
  
    if ( str1.equalsIgnoreCase(".bmp") )  // if the file has '.bmp' extensio
       bmpDraw(entry.name(), 0, 0,filtru);
     
    entry.close();  // close the file
 
      delay(2000);
        digitalWrite(LED, HIGH);
        if(digitalRead(button)== LOW) {
          digitalWrite(LED, LOW);
            a=1;
            delay(500);
            break;
            
   }
 
    }
}
}

void printStatus()
{
  switch (aht20.getStatus())
  {
    case AHTXX_NO_ERROR:
      Serial.println(F("no error"));
      break;

    case AHTXX_BUSY_ERROR:
      Serial.println(F("sensor busy, increase polling time"));
      break;

    case AHTXX_ACK_ERROR:
      Serial.println(F("sensor didn't return ACK, not connected, broken, long wires (reduce speed), bus locked by slave (increase stretch limit)"));
      break;

    case AHTXX_DATA_ERROR:
      Serial.println(F("received data smaller than expected, not connected, broken, long wires (reduce speed), bus locked by slave (increase stretch limit)"));
      break;

    case AHTXX_CRC8_ERROR:
      Serial.println(F("computed CRC8 not match received CRC8, this feature supported only by AHT2x sensors"));
      break;

    default:
      Serial.println(F("unknown status"));    
      break;
  }
}

#define BUFFPIXEL 20
void bmpDraw(char *filename, uint8_t x, uint16_t y, int filtru) {
 
  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();
 
  if((x >= tft.width()) || (y >= tft.height())) return;
 
  Serial.println();
  Serial.print(F("Loading image '"));
  Serial.print(filename);
  Serial.println('\'');
 
  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.print(F("File not found"));
    return;
  }
 
  // Parse BMP header
  if(read16(bmpFile) == 0x4d42) { // BMP signature
    Serial.print(F("File size: ")); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignorecreator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed
 
        goodBmp = true; //Supported BMP format -- proceed!
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);
 
        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;
 
        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }
 
        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= tft.width())  w = tft.width()  - x;
        if((y+h-1) >= tft.height()) h = tft.height() - y;
 
        // Set TFT address window to clipped image bounds
        tft.startWrite();
        tft.setAddrWindow(x, y, w, h);
 
        for (row=0; row<h; row++) { // For each scanline...
 
          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip){ // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
           
          }
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if(bmpFile.position() != pos) { // Need seek?
            tft.endWrite();
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }
 
          for (col=0; col<w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
              tft.startWrite();
            }
 
            // Convert pixel from BMP to TFT format, push to display

            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r= sdbuffer[buffidx++];
            int g1, r1, b1;

            if(filtru==1){ //roz
            g = 0;
            b = (r + b) / 2;
            r = (r + b) / 2;

            }
            if(filtru==2){ //alb si negru


            g1 = (r + b + g) / 3;
            b1 = (r + g + b) / 3;
            r1 = (g + b + r) / 3;

            r = r1;
            g = g1;
            b = b1;              
            }
            if(filtru==3){
            g1 = (r + b) / 2;
            b1 = (r + g) / 2;
            r1 = (g + b) / 2;

            r = r1;
            g = g1;
            b = b1;
            }
           
            tft.pushColor(tft.color565(r,g,b));
          } // end pixel
        } // end scanline
        tft.endWrite();
        Serial.print(F("Loaded in "));
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }
 bmpFile.close();
  if(!goodBmp) Serial.println(F("BMP format not recognized."));
}


uint16_t read16(File f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}
 
uint32_t read32(File f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}
 
 
void printDirectory(File dir, int numTabs) {
  while (true) {
 
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

