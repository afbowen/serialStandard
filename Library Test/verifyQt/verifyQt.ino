#include <serialstandard.h>

serialStandard PC(0, 115200);

//Test sketch to read incoming data from Qt
//Send whatever is read from PC (Serial0) to OLED via TWI

#include <SPI.h>                //For OLED display
#include <Wire.h>               //For VL53L0X
#include <Adafruit_GFX.h>       //For graphics
#include <Adafruit_SSD1306.h>   //For display
#include <Adafruit_VL53L0X.h>   //For rangefinder 
#include <Adafruit_TSL2561_U.h> //For LUX

//OLED DISPLAY//
#define OLED_DC     6
#define OLED_CS     7
#define OLED_RESET  8
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

void setup() {
  Serial.begin(115200);
  PC.begin();

  //OLED DISPLAY//
  display.begin(SSD1306_SWITCHCAPVCC);  //Initializes with Adafruit splashscreen
  display.clearDisplay();   //Clear display buffer (Adafruit splashscreen)
  display.display();        //Displays buffer (empty in this case)
  
}

void loop() {

}

void serialEvent()
{
  packetContents_t packet;
  packet = PC.serialCommandHandler();

  //Print to Serial0
  Serial.print(packet.opCode); Serial.println(packet.opVal);

  //Print to OLED
  prepareOLED();
  display.print(packet.opCode); display.print(packet.opVal);
  display.display();
  
}

void prepareOLED()
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);    
}

