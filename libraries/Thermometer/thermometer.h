#ifndef THERMOMETER_H
#define THERMOMETER_H

#include "Arduino.h"
#include "display.h"
#include "temperaturesensor.h"

/**
 * Struktur dient zur Definition
 * von Zeichen, die auf
 * 4x3-Zeilennmatrix geschrieben werden muessen.
 */
typedef struct thermometer_char
{
    /**
     * Zeichen, um dass es dich dabei handelt
     */
    char character;
    /**
     * Zeilenmatrix
     */
    int values[4][3];
} thermometer_char;

/**
 * Klasse Thermometer dient als Schnittstelle
 * zwischen Display und TemperaturSensor.
 * Diese soll Daten vom Sensor lesen und auf
 * Display schreiben
 */
class Thermometer
{
    private:
        /**
         * Display auf den geschrieben werden soll
         */
        Display *display;
        /**
         * Sensor von dem gelesen werden soll
         */
        TemperatureSensor *sensor;

        /**
         * alte Temperaturen die gemessen wurden.
         */
        float oldTemperature[14];
        /**
         * Temperatur lesen und mit alten
         * Temperaturen verrechnen
         */
        float readTemperature();

        /**
         * Attribut enthaelt eigene Zeichen, die fuer
         * die Ausgabe programmiert werden muessen
         * (Schriftart)
         */
        thermometer_char chars[12];
        /**
         * Gibt den index des Zeichens zurueck
         * (aus der Schriftart)
         */
        int indexOf(char character);
        /**
         * laed die komplette Schriftart
         */
        void loadFonts();
        /**
         * setzt einzelne Zeichen der Schriftart
         */
        void setFont(int index, char character, int values[4][3]);
        /**
         * Schreibt den Text mit der eigenen Schriftart auf das
         * Display
         */
        void writeTemperature(char* text);
    public:
        /**
         * Konstruktor erstellt Thermometerobjekt
         * displayPins - pins an denen Display angeschlossen ist
         * analogPin - pin an dem Temperatursensor angeschlossen ist
         * voltage - mV Wert, mit dem Tempsensor betrieben wird.
         */
        Thermometer(display_pins displayPins, int analogPin, int voltage);
        /**
         * schreibt die aktuelle Temperatur auf das Display
         */
        void writeTemperature();
        /**
         * initialisiert das Thermometer
         */
        void init();
};

#endif // THERMOMETER_H
