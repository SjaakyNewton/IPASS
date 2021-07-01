//
// Created by Isaak van Luijk on 15/06/2021.
//
/// @file
/// \brief
/// The CPP file for the .H
/// \details
/// All the functions are set in this file.

#include "PCA9685.hpp"

/// \brief
/// Defines for basis registers
/// \details
/// These registers are made so I only have to give a pin number. With those numbers I can go on.
/// Credits tho Rogier van Rooijen with the idea
/// \note
/// This only works if you just use one PCA9685 chip, two or more hasn't bin tested
#define pinOnLow 0x06
#define pinOnHigh 0x07
#define pinOffLow 0x08
#define pinOffHigh 0x09
#define mode1Register 0x00

PCA9685::PCA9685(hwlib::i2c_bus &i2c, uint8_t pin, uint8_t i2cAddress):
i2c(i2c),
i2cAddress(i2cAddress),
On_High(pin * 4 + pinOnHigh),
On_Low(pin * 4 + pinOnLow),
Off_High(pin * 4 + pinOffHigh),
Off_Low(pin * 4 + pinOffLow)
{}

/// \brief
/// Set SLEEP on 0 in MODE1 register.
/// \details
/// This function sets the sleep mode on 0 in the MODE1 register.
/// There is a wait moment for the register to progress the change.
/// This is needed for the SLEEP register
/// \note
/// Only the ALLCALL stays on 1, rest is 0.
void PCA9685::modeOffSleep(){
    auto trans = i2c.write(i2cAddress);
    trans.write(mode1Register);
    trans.write(0x01);
    hwlib::wait_ms(1);
}

/// \brief
/// Set ALL on 0 in MODE1 register.
/// \details
/// This function sets ALL the bits 0 in the MODE1 register.
/// There is a wait moment for the register to progress the change.
/// This is needed for the SLEEP register
void PCA9685::modeZero(){
    auto trans = i2c.write(i2cAddress);
    trans.write(mode1Register);
    trans.write(0x00);
    hwlib::wait_ms(1);
}

/// \brief
/// Set the MODE1 to default.
/// \details
/// Set the MODE1 back to its origenal state.
/// This is: 00010001
void PCA9685::modeDefault(){
    auto trans = i2c.write(i2cAddress);
    trans.write(mode1Register);
    trans.write(0x11);
    hwlib::wait_ms(1);
}

/// \brief
/// Write 8 bits to specific register.
/// \details
/// It writes first to set the adres of the register.
/// Then it sends the data to the register.
/// The register is specific. The On_LOW, On_HIGH, Off_LOW or Off_HIGH needs to be given.
/// You can't send 16 byts in ones.
/// You can also send to other registers.
/// \note
/// Some functions use this already and are set for a steady use.
/// If you wanne do it your self you have to re-giff the register.
void PCA9685::write(uint8_t adres, uint8_t data) {
    auto trans = i2c.write(i2cAddress);
    trans.write(adres);
    trans.write(data);
}

/// \brief
/// Reads the data out register.
/// \details
/// The given address of the register will be read and given back.
/// This is only one register at a time.
void PCA9685::read(uint8_t adres){
    uint8_t data;
    i2c.write(i2cAddress).write(adres);
    i2c.read(i2cAddress).read(data);
    hwlib::cout<<data<<" "<<adres<<"\n";
}

/// \brief
/// Set the frequantion of the PWM signal.
/// \details
/// Given is the Hz. This is made a float so the code didn't said something about it.
/// The Hz will switched to the binary time of the clock and 12 bits for the PWM signal.
/// (This formule can be found in the DataSheet.)
/// The frequantion will be writen to the PreScale register.
/// \note
/// For the servo you need 50 Hz
void PCA9685::frequentie(float Hz){
    float MHz = 25;
    float round = 4096 * Hz;
    round = MHz / round;
    int afgerond = round * 1000000 -1;
    write(0xFE,afgerond);
}

/// \brief
/// Set Servo in specific degree.
/// \details
/// With the frequentie function the servo can listen to the PWM signal.
/// Here we give the degree how it needs to stand.
/// With the formula we translate it to the registers data type.
/// We make it a 16 bits to two seperated 8 bits. This is for easy degree reading and translating.
/// \note
/// We make the servo start at the beginning of a new round/PWM wave.
void PCA9685::setServo(int graden){
    int gradenInByte = graden * 2 + 102;
    uint8_t begin = 0x00;
    uint16_t totaal = gradenInByte;
    uint8_t dataHigh = totaal>>8;
    uint8_t dataLow = totaal;
    write(On_High, begin);
    write(On_Low, begin);
    write(Off_High,dataHigh);
    write(Off_Low,dataLow);
}


/// \brief
/// Standard servo option.
/// \details
/// This is sett on a standard servo option.
/// Set it one 90 degrees.
void PCA9685::setServo90(){
    setServo(90);
}

/// \brief
/// Standard servo option.
/// \details
/// This is sett on a standard servo option.
/// Set it one 180 degrees.
void PCA9685::setServo180(){
    setServo(180);
}

/// \brief
/// Standard servo option.
/// \details
/// This is sett on a standard servo option.
/// Set it one 0 degrees.
void PCA9685::setServo0(){
    setServo(0);
}

//void PCA9685::printRegisters(){
//    hwlib::cout<<On_Low<<"\n";
//    hwlib::cout<<On_High<<"\n";
//    hwlib::cout<<Off_Low<<"\n";
//    hwlib::cout<<Off_High<<"\n";
//}