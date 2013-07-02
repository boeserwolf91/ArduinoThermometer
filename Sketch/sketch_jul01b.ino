#include <display.h>
#include <temperaturesensor.h>
#include <thermometer.h>

Thermometer *thermometer;

void setup()
{
    Serial.begin(9600);
   
    display_pins displayPins = init_display_pins(13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2); 
    
    thermometer = new Thermometer(displayPins, 0, 4500);    
    thermometer->init();
}

void loop()
{
    thermometer->writeTemperature();
    delay(10000);
}
