#include "display.h"

// zur Struktur:

display_pins init_display_pins(int RS, int RW, int E, int DB0, int DB1, int DB2, int DB3, int DB4, int DB5, int DB6, int DB7, int RES)
{
    Serial.println("Displaypins werden initialisiert.");
    display_pins pins;

    pins.RS = RS;
    pins.RW = RW;
    pins.E = E;
    pins.RES = RES;

    pins.DB[0] = DB0;
    pins.DB[1] = DB1;
    pins.DB[2] = DB2;
    pins.DB[3] = DB3;
    pins.DB[4] = DB4;
    pins.DB[5] = DB5;
    pins.DB[6] = DB6;
    pins.DB[7] = DB7;

    return pins;
}

//zur Klasse:

Display::Display(display_pins pins)
{
    Serial.println("Displayobjekt wird erstellt.");
    this->pins = pins;
    this->characterLength = 0;
    /*
     * PinMode jedes Pins auf OUTPUT setzen und Startwert zuordnen
     */
    for(int i = 0; i < 8; i++)
    {
        pinMode(pins.DB[i], OUTPUT);
        digitalWrite(pins.DB[i], LOW);
    }
    pinMode(pins.RS, OUTPUT);
    digitalWrite(pins.RS, LOW);
    pinMode(pins.RW, OUTPUT);
    digitalWrite(pins.RW, LOW);
    pinMode(pins.RES, OUTPUT);
    digitalWrite(pins.RES, HIGH);
    pinMode(pins.E, OUTPUT);
    digitalWrite(pins.E, LOW);
}

int Display::getBit(int value, int bitNumber)
{
    if( (value & (1 << bitNumber)) != 0)
    {
        return HIGH;
    }
    return LOW;
}

void Display::write(int value, int duration)
{
    digitalWrite(pins.E, HIGH);

    this->write(value);

    digitalWrite(pins.E, LOW);
    delayMicroseconds(duration);
}

void Display::write(int value)
{
    for(int i = 0; i < 8; i++)
    {
        digitalWrite(pins.DB[i], getBit(value, i));
    }
}

void Display::executeCommand(int value, int duration)
{
    digitalWrite(pins.RS, LOW);
    digitalWrite(pins.RW, LOW);

    this->write(value, duration);
}

void Display::writeData(int value)
{
    digitalWrite(pins.RS, HIGH);
    digitalWrite(pins.RW, LOW);

    this->write(value, 43);
}

void Display::init()
{
    Serial.println("Display wird initialisiert!");
    this->reset();    // Display reseten um alte Einstellungen zu loeschen

    /*
     * eigentliche Initialisierung
     */
    digitalWrite(pins.RS, LOW);
    digitalWrite(pins.RW, LOW);
    delay(2);

    this->write(0x34, 39); // 8-Bit Datenlaenge BIT RE=1
    this->write(0x09, 39); // 4-Zeilen Modus
    this->write(0x30, 39); // 8-Bit Datenlaenge BIT RE=0
    this->write(0x0c, 39); // Display ein, Cursor ein, Cursor blinkt nicht
    this->write(0x01, 1600);// Display loeschen, Cursor auf 1, Spalte von 1.Zeile
    this->write(0x06, 39); // Cursor Auto-Increment
    Serial.println("Display wurde initialisiert!");
}

void Display::reset()
{
    Serial.println("Display wird resettet!");
    digitalWrite(pins.RES, LOW);
    delayMicroseconds(50);
    digitalWrite(pins.RES, HIGH);

    this->characterLength = 0;
}

void Display::clearScreen()
{
    /*
     * Befehl um Display zu loeschen
     * und Cursor auf 1 Spalte, 1 Zeile
     * zu setzen.
     */
    this->executeCommand(0x01, 1600);

    this->characterLength = 0;
}

void Display::writeString(const char *text)
{
    digitalWrite(pins.RS, HIGH);
    digitalWrite(pins.RW, LOW);

    int length = strlen(text);
    for(int i = 0; i < length; i++)
    {
        if(text[i] == '\n')
        {
            while(this->characterLength % 20 != 0)
            {
                this->write(' ', 43);
                this->characterLength++;
            }
        }
        else
        {
            this->write(text[i], 43);
            this->characterLength++;
        }
    }
}
