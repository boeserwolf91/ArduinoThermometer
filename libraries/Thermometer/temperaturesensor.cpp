#include "temperaturesensor.h"

TemperatureSensor::TemperatureSensor(int pin, int voltage)
{
    this->pin = pin;
    this->voltage = voltage;
}

float TemperatureSensor::readTemperature(int mode)
{
    Serial.println("Temperatursensor wird ausgelesen");
    int value = analogRead(pin);
    Serial.print("\tWert: ");
    Serial.println(value);

    // read: http://learn.adafruit.com/tmp36-temperature-sensor/using-a-temp-sensor
    float millivolts = value * (this->voltage / 1024.0);
    float temperature = millivolts / 10; // sensor output is 10mV per degree Celsius

    if(mode == this->MODE_CELSIUS)
    {
        Serial.print("\tCelsius: ");
        Serial.println(temperature);
    }
    else if(mode == this->MODE_KELVIN)
    {
        temperature += 273.15;

        Serial.print("\tKelvin: ");
        Serial.println(temperature);
    }
    else if(mode == this->MODE_FAHRENHEIT)
    {
        temperature = (temperature * 9.0 / 5.0) + 32.0;

        Serial.print("\tFahrenheit: ");
        Serial.println(temperature);
    }
    else
    {
        Serial.print("Ungueltiger Modus ");
        Serial.print(mode);
        Serial.println("wurde uebergeben.");
        return -1.0;
    }
    return temperature;
}
