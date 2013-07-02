#ifndef DISPLAY_H
#define DISPLAY_H

#include "Arduino.h"

/**
 * Diese Struktur dient zur Verwaltung der Displaypins.
 * Sie beinhaltet alle Pins, mit dem das Arduino mit dem Display
 * kommunizieren kann.
 */
typedef struct display_pins
{
    int DB[8];  // 8 - Datenbits
    int RS;     // RS-bit gibt an, ob Daten oder Befehle geschrieben werden
    int RW;     // RW-bit gibt an, ob geschrieben oder gelesen werden soll
    int E;      // Enable-bit
    int RES;    // Reset-bit (Reset bei LOW)
} display_pins;

/**
 * Mithilfe dieser Methode laesst sich auf einfacher Art und Weise
 * eine Variable des Datentyps display_pins erstellen und initialisieren.
 */
display_pins init_display_pins(int RS, int RW, int E, int DB0, int DB1, int DB2, int DB3, int DB4, int DB5, int DB6, int DB7, int RES);

/**
 * Die Klasse Display dient zur Ansteuerung des Displays mit dem
 * Arduino
 */
class Display
{
    private:
        /**
         * Attribut beinhaltet Pins an denen das Display
         * angeschlossen ist.
         */
        display_pins pins;
        /**
         * Attribut gibt an, wie viele Zeichen derzeit
         * auf dem Display dargestellt werden.
         * funktioniert nur mit writeString()!!
         */
        int characterLength;
        /**
         * Diese Methode bestimmt ob das Byte aus dem
         * value Parameter 1 oder 0 ist und gibt dies
         * entsprechend zurueck.
         */
        int getBit(int value, int bitNumber);
        /**
         * Funktion schreibt Daten-Byte auf Arduino
         * und setzt Enable-Werte (mit Wartezeit)
         */
        void write(int value, int duration);
        /**
         * Funktion schreibt Daten-Byte auf Arduino
         */
        void write(int value);

    public:
        /**
         * Konstruktor, der Objekt erstellt.
         * Benoetigt eine Variable vom Typ display_pins, welche
         * die verwendeten Pins spezifiziert.
         */
        Display(display_pins displayPins);

        /**
         * Funktion fuehrt einen Befehl auf dem Arduino aus.
         * value - Befehlscode der ausgefuehrt werden soll
         * duration - Abarbeitungszeit, die Display benoetigt,
         *            um Befehl abzuschliessen. (in Mikrosec.)
         */
        void executeCommand(int value, int duration);
        /**
         * Funktion schickt einen Datenbefehl auf das Arduino.
         * benoetigt eine feste Zeit von 43 Mikrosekunden
         */
        void writeData(int value);
        /**
         * Initialisiert den Display, mit Standardeinstellungen
         * 8-Bit Datenmodus, 4 Zeilen, Cursor aus, etc
         */
        void init();
        /**
         * Setzt alle Einstellungen, die irgendwann
         * mal getroffen wurden, zurueck.
         */
        void reset();
        /**
         * loescht den Bildschirminhalt
         */
        void clearScreen();
        /**
         * Schreibt eine Zeichenkette auf das Arduino
         */
        void writeString(const char *text);
};

#endif // DISPLAY_H
