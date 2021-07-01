//
// Created by Isaak van Luijk on 16/06/2021.
//
/// @file
/// \brief
/// This is the documentation of de libary/class for a TCS3472 chip.
/// \details
/// This libary is at the moment only for the basic colors. In the future I want to add more advanced color reading.
/// You only have to use the SDA, SCL, 3.3V power supply and groun pins.

#ifndef GY_33_H
#define GY_33_H

/// \brief
/// GY_33 class foundation
/// \details
/// The class is made with a i2c bus.
/// With this in the basis we can do everything we want to do.

class GY_33{
private:
    hwlib::i2c_bus & i2c;

/// \brief
/// The constructer and functions
/// \details
/// The constructer is set with one i2c bus.
/// The rest are functions you have to sett.
public:
    GY_33(hwlib::i2c_bus & i2c);
    void write(uint8_t address, uint8_t data);
    uint8_t read(uint8_t address);
    int readBasicColors();
    void setIntergrationTime(float timeMS);
    float readIntergrationTime();
    void setGAIN(int sensitivity);
    void setInterrupt(bool i);
    int readClearLight();
};

#endif //GY_33_H
