//
// Created by Isaak van Luijk on 16/06/2021.
//
#include "hwlib.hpp"
#include "GY-33.h"
/// @file
/// \brief
/// Defines for basis registers
/// \details
/// First we have registers to sett for the way of setting everyhing
/// Then we have the registers for reading the color data.
#define enableRegister 0x00
#define Atime 0x01
#define i2cAddress 0x29
#define controlRegister 0x0F
#define commandByte 0x80

#define clearLow 0x14
#define clearHigh 0x15
#define redLow 0x16
#define redHigh 0x17
#define greenLow 0x18
#define greenHigh 0x19
#define blueLow 0x1A
#define blueHigh 0x1B

GY_33::GY_33(hwlib::i2c_bus & i2c) :
i2c(i2c) {}


/// \brief
/// write to register
/// \details
/// This function write a given byte to a register.
/// It is send with the commandByte.
/// This sets the one bit up to let it read data.
void GY_33::write(uint8_t address, uint8_t data){
    auto trans = i2c.write(i2cAddress);
    trans.write(commandByte|address);
    trans.write(data);
}

/// \brief
/// read from register
/// \details
/// This function reads the byte of a register.
/// It returns the data. It is send with the commandByte.
/// This sets the one bit up to let it read data.
/// \note
/// If you read only 0 then you read to fast.
uint8_t GY_33::read(uint8_t address){
    uint8_t data = 0;
//    uint8_t IntergrationTime = 0xFF;
    hwlib::wait_ms(24);
    i2c.write(i2cAddress).write(commandByte | address);
//    hwlib::wait_ms((256 - IntergrationTime) * 12 / 5 + 1);
    i2c.read(i2cAddress).read(data);
    return data;
}

/// \brief
/// make 2 bytes of 8bit, one 16 bit byte.
/// \details
/// The colors are read as two seperated 8 bytes. By fusing these two together we get the compleet falue.
/// It returns the fushed byte.
uint16_t samenvoegen(uint8_t HIGH, uint8_t LOW){
    uint16_t together = HIGH;
    together <<= 8;
    together += LOW;
    return together;
}

/// \brief
/// Reads RGB ones
/// \details
/// This only read the basic colours: Red, Green and Blue.
/// It returns an INT: 1=Red, 2=Green, 3=Blue, 0=Error.
/// This sends the most out coming color back.
int GY_33::readBasicColors(){
    setInterrupt(false);
    hwlib::wait_ms(60);
    uint8_t IntergrationTime = 0xFF;
    write(enableRegister,0x01);
    hwlib::wait_ms(3);
    write(enableRegister,0x03);
    hwlib::wait_ms((256 - IntergrationTime) * 12 / 5 + 1);

//    uint8_t clearLowData = read(clearLow);
//    uint8_t clearHighData = read(clearHigh);
//    uint16_t clearData= samenvoegen(clearHighData,clearLowData);

    uint8_t redLowData = read(redLow);
    uint8_t redHighData = read(redHigh);
    uint16_t redData = samenvoegen(redHighData, redLowData);

    uint8_t greenLowData = read(greenLow);
    uint8_t greenHighData = read(greenHigh);
    uint16_t greenData = samenvoegen(greenHighData, greenLowData);

    uint8_t blueLowData = read(blueLow);
    uint8_t blueHighData = read(blueHigh);
    uint16_t blueData = samenvoegen(blueHighData, blueLowData);

    hwlib::wait_ms((256 - IntergrationTime) * 12 / 5 + 1);

    write(enableRegister, 0);

    setInterrupt(true);

    if(redData > blueData && redData > greenData){// && clearData>=0
        return 1;
    }
    else if(greenData > redData && greenData > blueData){//&& clearData>=0
        return 2;
    }
    else if(blueData > redData && blueData > greenData){//&& clearData>=0
        return 3;
    }
    else{
        return 0;
    }

}

/// \brief
/// Read Clear Light.
/// \details
///It reads the registers of Clear Light.
/// Returns the value in INT.
int GY_33::readClearLight() {
    setInterrupt(false);
    hwlib::wait_ms(60);
    uint8_t IntergrationTime = 0xFF;
    write(enableRegister,0x01);
    hwlib::wait_ms(3);
    write(enableRegister,0x03);
    hwlib::wait_ms((256 - IntergrationTime) * 12 / 5 + 1);

    uint8_t clearLowData = read(clearLow);
    uint8_t clearHighData = read(clearHigh);
    uint16_t clearData= samenvoegen(clearHighData,clearLowData);

    write(enableRegister, 0);
    setInterrupt(true);
    return clearData;
}

/// \brief
/// Set Intergration Time.
/// \details
/// Set the intergration time from ms in hex
void GY_33::setIntergrationTime(float timeMS) {
    uint8_t intergrationTime = 256 - (timeMS / 2.4);
    write(Atime, intergrationTime);
}

/// \brief
/// Read Intergration Time.
/// \details
/// Read the Intergration time from hex in ms.
float GY_33::readIntergrationTime() {
    float intergrationTime = 2.4 * (256 - read(Atime));
    return intergrationTime;
}

/// \brief
/// Set sensetivity of the sensor
/// \details
/// The amounth of light that comes in can be set.
/// There are only 4 options of sensetivity.
/// By sending 0,4,16, 60 to the control register. Now you just give 1 to 4.
/// 4 is here the most and 1 the less.
void GY_33::setGAIN(int sensetivity = 1){
    if(sensetivity == 1) {
        write(controlRegister,0x00);
    }
    else if(sensetivity == 2){
        write(controlRegister,0x01);
    }
    else if(sensetivity == 3){
        write(controlRegister,0x02);
    }
    else if(sensetivity == 4){
        write(controlRegister,0x03);
    }
    else{
        return;
    }
}

/// \brief
/// Set interrupt.
/// \details
/// This sets in the enable register the interrupt.
/// This is set bij de bool.
/// This is needed for the sensor so it can create his own interrupts or not.
void GY_33::setInterrupt(bool i) {
    uint8_t r = read(enableRegister);
    if (i) {
        r |= 0x10;
    } else {
        r &= ~0x01;
    }
    write(enableRegister, r);
}


//void readAdvancedeColors(){
    // Rood = min rood = FF
    // Rood = max rood = FF

    // Rood = min groen = FF
    // Rood = min groen = FF


    // Bij rood is er rood en blauw en het minste groen
    // Bij Groen is er Groen en blauw en het minste rood
    // Bij blauw is er blauw
//}

