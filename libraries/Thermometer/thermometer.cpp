#include "thermometer.h"

Thermometer::Thermometer(display_pins displayPins, int analogPin, int voltage)
{
    display = new Display(displayPins);
    sensor = new TemperatureSensor(analogPin, voltage);
}

void Thermometer::init()
{
    display->init();

    // Feld um spezifische Zeichen zu erstellen!
    int characters[8][8] =
    {
        {0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0F},
        {0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F},
        {0x00, 0x00, 0x00, 0x00, 0x10, 24, 28, 30},
        {31, 31, 31, 31, 30, 28, 24, 16},
        {16, 24, 28, 30, 31, 31, 31, 31},
        {15, 7, 3, 1, 0, 0, 0, 0},
        {31, 31, 31, 31, 0, 0, 0, 0},
        {30, 28, 24, 16, 0, 0, 0, 0}
    };
    // Wechseln zum CR RAM
    display->executeCommand(0x40, 39);
    // spezifische Zeichen einlesen!
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            display->writeData(characters[i][j]);
        }
    }
    // Zurueck zum DD RAM wechseln
    display->executeCommand(0x80, 39);

    //alte Temperaturen initialisieren
    for(int i = 0; i < 14; i++)
    {
        oldTemperature[i] = 0.0;
    }

    this->loadFonts();


    display->writeString("Bitte warten...");

    for(int i = 0; i < 15; i++)
    {
        this->readTemperature();
    }
}

void Thermometer::loadFonts()
{
    int zero[4][3] =
    {
        {0x00, 0x01, 0x02},
        {0x1f, 0x20, 0x1f},
        {0x1f, 0x20, 0x1f},
        {0x05, 0x06, 0x07}
    };
    setFont(0, '0', zero);

    int one[4][3] =
    {
        {0x00, 0x01, 0x20},
        {0x20, 0x1f, 0x20},
        {0x20, 0x1f, 0x20},
        {0x06, 0x06, 0x06}
    };
    setFont(1, '1', one);

    int two[4][3] =
    {
        {0x00, 0x01, 0x02},
        {0x00, 0x01, 0x03},
        {0x1f, 0x20, 0x20},
        {0x06, 0x06, 0x06}
    };
    setFont(2, '2', two);

    int three[4][3] =
    {
        {0x00, 0x01, 0x02},
        {0x20, 0x00, 0x03},
        {0x20, 0x05, 0x04},
        {0x05, 0x06, 0x07}
    };
    setFont(3, '3', three);

    int four[4][3] =
    {
        {0x00, 0x20, 0x01},
        {0x1f, 0x20, 0x1f},
        {0x06, 0x06, 0x1f},
        {0x20, 0x20, 0x06}
    };
    setFont(4, '4', four);

    int five[4][3] =
    {
        {0x01, 0x01, 0x01},
        {0x1f, 0x01, 0x02},
        {0x20, 0x20, 0x1f},
        {0x06, 0x06, 0x07}
    };
    setFont(5, '5', five);

    int six[4][3] =
    {
        {0x00, 0x01, 0x02},
        {0x1f, 0x01, 0x02},
        {0x1f, 0x20, 0x1f},
        {0x05, 0x06, 0x07}
    };
    setFont(6, '6', six);

    int seven[4][3] =
    {
        {0x01, 0x01, 0x01},
        {0x20, 0x00, 0x03},
        {0x20, 0x1f, 0x20},
        {0x20, 0x06, 0x20}
    };
    setFont(7, '7', seven);

    int eight[4][3] =
    {
        {0x00, 0x01, 0x02},
        {0x1f, 0x01, 0x1f},
        {0x1f, 0x20, 0x1f},
        {0x05, 0x06, 0x07}
    };
    setFont(8, '8', eight);

    int nine[4][3] =
    {
        {0x00, 0x01, 0x02},
        {0x1f, 0x01, 0x1f},
        {0x20, 0x20, 0x1f},
        {0x05, 0x06, 0x07}
    };
    setFont(9, '9', nine);

    int c[4][3] =
    {
        {0x00, 0x01, 0x02},
        {0x1f, 0x20, 0x20},
        {0x1f, 0x20, 0x20},
        {0x05, 0x06, 0x07}
    };
    setFont(10, 'c', c);

    int minusChar[4][3] =
    {
        {0x20, 0x20, 0x20},
        {0x20, 0x20, 0x20},
        {0x06, 0x06, 0x06},
        {0x20, 0x20, 0x20}
    };
    setFont(11, '-', minusChar);
}

void Thermometer::setFont(int index, char character, int values[4][3])
{
    thermometer_char charac;
    charac.character = character;

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            charac.values[i][j] = values[i][j];
        }
    }

    chars[index] = charac;
}

int Thermometer::indexOf(char character)
{
    for(int i = 0; i < 12; i++)
    {
        if(chars[i].character == character)
        {
            return i;
        }
    }
    return -1;
}

float Thermometer::readTemperature()
{
    int hasTemperature = 0;
    float totalTemp = 0;
    while(this->oldTemperature[hasTemperature] != 0 && hasTemperature < 14)
    {
        totalTemp += this->oldTemperature[hasTemperature++];
    }

    for(int i = 0; i < 15 - hasTemperature; i++)
    {
        totalTemp += sensor->readTemperature(sensor->MODE_CELSIUS);
        delay(100); // wait a little
    }
    totalTemp /= 15;

    if(hasTemperature == 14)
    {
        for(int i = 0; i < 13; i++)
        {
            this->oldTemperature[i] = this->oldTemperature[i + 1];
        }
        hasTemperature = 8;
    }
    this->oldTemperature[hasTemperature] = totalTemp;
    return totalTemp;
}

void Thermometer::writeTemperature()
{
    float temperature = this->readTemperature();

    char *buffer;
    buffer = (char *) malloc(sizeof(char) * 6);
    /*
     * Funktion castet float zum String und schreibt dieses in den letzten Parameter.
     * Der erste Parameter nimmt den zu castenden float wert an
     * Der zweite gibt an, wie lang der String sein soll.
     * Der dritte Parameter gibt die Nachkommastellen an
     */
    dtostrf(temperature, 5, 2, buffer);
    this->writeTemperature(buffer);
}

void Thermometer::writeTemperature(char* text)
{
    int indexes[5];
    int currentSign = 0;
    int index = 0;

    if(text[0] == '-')
    {
        indexes[0] = indexOf('-');
        currentSign++;
    }
    else
    {
        indexes[0] = -1;
    }
    index++;

    while(text[currentSign] != '\0' && currentSign < 6)
    {
        if(text[currentSign] == '.')
        {
            if(index < 3)
            {
                indexes[index] = indexes[index - 1];
                indexes[index - 1] = indexOf('0');
                index++;
            }
        }
        else
        {
            if(text[currentSign] == ' ')
            {
                text[currentSign] = '0';
            }
            indexes[index++] = indexOf(text[currentSign]);
        }
        currentSign++;
    }
    indexes[index++] = indexOf('c');

    if(index != 6)
    {
        //display->writeString("Zeichenkette ist zu kurz!");
        return;
    }

    thermometer_char chars[6];
    for(int i = 0; i < 6; i++)
    {
        if(indexes[i] < 0 && i > 0)
        {
            /*
            char buffer[80];
            sprintf(buffer, "Konnte Index nicht erstellen.\nIndexe %d, %d, %d, %d, %d!", indexes[0], indexes[1], indexes[2], indexes[3], indexes[4], indexes[5]);
            display->writeString(buffer);
            */
            return;
        }
        if(indexes[i] >= 0)
        {
            chars[i] = this->chars[indexes[i]];
        }
    }

    display->clearScreen();

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 20; j++)
        {
            if(j < 3 && indexes[0] != -1)
            {
                display->writeData(chars[0].values[i][j]);
            }
            else if(j > 2 && j < 6)
            {
                display->writeData(chars[1].values[i][j - 3]);
            }
            else if(j > 5 && j < 9)
            {
                display->writeData(chars[2].values[i][j - 6]);
            }
            else if(j > 9 && j < 13)
            {
                display->writeData(chars[3].values[i][j - 10]);
            }
            else if(j > 12 && j < 16)
            {
                display->writeData(chars[4].values[i][j - 13]);
            }
            else if(j > 16 && j < 20)
            {
                display->writeData(chars[5].values[i][j - 17]);
            }
            else if(i == 0 && j == 16)
            {
                display->writeData(0x6f);
            }
            else if(i == 3 && j == 9)
            {
                display->writeData(0xd2);
            }
            else
            {
                display->writeData(0x20);
            }
        }
    }
}
