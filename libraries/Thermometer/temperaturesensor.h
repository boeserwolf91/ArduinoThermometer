#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#include "Arduino.h"

/**
 * Diese Klasse dient zur Ansteuerung
 * des Temperatursensors LM35 mit dem Arduino.
 */
class TemperatureSensor
{
    public:
        /**
         * Mode-Variablen zum Temperatur
         * lesen. Gibt an, in welcher Einheit
         * die Temperatur zurueckgegeben wird.
         */
        static const int MODE_CELSIUS = 1;
        static const int MODE_KELVIN = 2;
        static const int MODE_FAHRENHEIT = 3;

    private:
        /**
         * Der analoge Pin, an dem der Sensor
         * angeschlossen ist.
         */
        int pin;
        /**
         * Die Voltzahl in mV mit der Sensor
         * betrieben wird.
         */
        int voltage;

    public:
        /**
         * Konstruktur erstellt Objekt der Klasse.
         * analogPin - Pin, an dem Sensor angeschlossen ist.
         * voltage - mV-Wert mit welcher Sensor betrieben wird.
         */
        TemperatureSensor(int analogPin, int voltage);
        /**
         * liest die Temperatur aus und gibt diese zurueck
         * mode - gibt an, in welcher Einheit Wert
         * zurueckgegeben werden soll.
         */
        float readTemperature(int mode);
};

#endif // TEMPERATURESENSOR_H
