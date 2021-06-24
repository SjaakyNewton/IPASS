//
// Created by Isaak van Luijk on 15/06/2021.
//

/// @file
/// \brief
/// This is the documentation of de libary/class for a PCA9685 chip.
/// \details
/// This libary uses a few functions. But more advanced functies use more simple functions.
/// You can still acces the simpeler functions.
/// \note
/// This Libary is made with the idea for serving servo's

#ifndef PCA9685_HPP
#define PCA9685_HPP

#include "hwlib.hpp"

/// \brief
/// Here is de class foundation.
/// \details
/// The class is made from de register ports of de chip.
/// To work with a servo you need to know what registers it uses.
/// I am working on more standard modes that can be used.
class PCA9685 {
private:
    //Ik maak er een i2c_bus van om er later naar te kunnen schrijven.
    //Nu is het waarschijnlijk handig als ik een class hiervoor maak die het uitbreid naar de GY_33 of wel een functie
    //maak die er voor mij direct een i2c_bus van maakt.
    hwlib::i2c_bus &i2c;
    uint8_t i2cAddress;
    uint8_t On_High;
    uint8_t On_Low;
    uint8_t Off_High;
    uint8_t Off_Low;

/// \brief
/// The constructer and functions
/// \details
/// The constructer is set with two array's. The first of the array for the HIGH and the second for the LOW.
/// You can give the i2c address but the standard one = 0x40;
public:
//    PCA9685(hwlib::i2c_bus &i2c, uint8_t On_High, uint8_t On_Low, uint8_t Off_High, uint8_t Off_Low, uint8_t i2cAddress = 0x40);
    PCA9685(hwlib::i2c_bus &i2c, uint8_t pin, uint8_t i2cAddress = 0x40);

    void modeOffSleep();
    void modeZero();
    void modeDefault();
    void write(uint8_t adres, uint8_t data);
    void read(uint8_t adres);
    void frequentie(float Hz);
    void setServo(int graden);
    void setServoHalfOpen();
    void setServoOpen();
    void setServoDicht();
    void printRegisters();
};

#endif //V1OOPC_EXAMPLES_PCA9685_H
