#include "funcionesSetup.h"
#include "Config.h"

extern Adafruit_SSD1306 display;


void configDisplay()
{
    // ------------------------- Movidas del display --------------------
if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
display.display();
  delay(2000); 

  // Clear the buffer
  display.clearDisplay();
  display.display();
  delay(2000); 
  display.clearDisplay();


}
