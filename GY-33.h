//
// Created by Isaak van Luijk on 16/06/2021.
//

#ifndef GY_33_H
#define GY_33_H

#define clearLow 0x14
#define clearHigh 0x15
#define redLow 0x16
#define redHigh 0x17
#define greenLow 0x18
#define greenHigh 0x19
#define blueLow 0x1A
#define blueHigh 0x1B

class GY_33{
private:
    //Ik maak er een i2c_bus van om er later naar te kunnen schrijven.
    //Nu is het waarschijnlijk handig als ik een class hiervoor maak die het uitbreid naar de GY_33 of wel een functie
    //maak die er voor mij direct een i2c_bus van maakt.
    hwlib::i2c_bus & i2c;
public:
    GY_33(hwlib::i2c_bus & i2c);
    void write(uint8_t address, uint8_t data);
    uint8_t read(uint8_t address);
    void readColors();
    void setIntergrationTime(float timeMS);
    float readIntergrationTime();
    void setGAIN(int sensitivity);
    void setInterrupt(bool i);
};

#endif //GY_33_H
