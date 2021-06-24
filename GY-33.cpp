//
// Created by Isaak van Luijk on 16/06/2021.
//
#include "hwlib.hpp"
#include "GY-33.h"

#define enableRegister 0x00
#define Atime 0x01
#define i2cAddress 0x29
#define controlRegister 0x0F
#define commandIets 0x80


GY_33::GY_33(hwlib::i2c_bus & i2c) :
i2c(i2c) {}

void GY_33::write(uint8_t address, uint8_t data){
    auto trans = i2c.write(i2cAddress);
    trans.write(commandIets|address);
    trans.write(data);
}



//Krijg nu steeds vaste waarde van 255.
//hwlib::i2c_bus I2CBUS = i2c;

//Je moet voor elke kleur een register uitlezen
//0x29 is de kleuren sensor zelf. Vervolgens kan ik zien bij page 20 welk register ik moet aanvragen per kleur.
//Hier kan ik de kleur een geheel maken
//En kan ik steeds elke kleur bij elkaar pakken en mogelijk samenvoegen.
//Nu kan ik kleuren hoog en laag aanroepen en kijken wat welke betekend.
uint8_t GY_33::read(uint8_t address){
    uint8_t data = 0;
//    uint8_t IntergrationTime = 0xFF;
    hwlib::wait_ms(24);
    i2c.write(i2cAddress).write(commandIets | address);
//    hwlib::wait_ms((256 - IntergrationTime) * 12 / 5 + 1);
    i2c.read(i2cAddress).read(data);
    //hwlib::cout<<data<<"\n";
    return data;
}

uint16_t samenvoegen(uint8_t HIGH, uint8_t LOW){
    uint16_t together = HIGH;
    together <<= 8;
    together += LOW;
    return together;
}

void GY_33::readColors(){
    uint8_t IntergrationTime = 0xFF;
    write(enableRegister,0x01);
    hwlib::wait_ms(3);
    write(enableRegister,0x03);
    hwlib::wait_ms((256 - IntergrationTime) * 12 / 5 + 1);

    uint8_t clearLowData = read(clearLow);
    uint8_t clearHighData = read(clearHigh);
    uint16_t clearData= samenvoegen(clearHighData,clearLowData);

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

    hwlib::cout<<"red \tgreen \t blue \t clear\n";
    hwlib::cout<<redData<<" \t\t"<<greenData<<" \t\t"<<blueData<<" \t\t"<<clearData<<"\n";
}

//Set the intergration time from ms in hex
void GY_33::setIntergrationTime(float timeMS) {
    uint8_t intergrationTime = 256 - (timeMS / 2.4);
    write(Atime, intergrationTime);
}


//Read the Intergration time from hex in ms.
float GY_33::readIntergrationTime() {
    float intergrationTime = 2.4 * (256 - read(Atime));
    return intergrationTime;
}

// There are only 4 options of sensetivity.
void GY_33::setGAIN(int sensetivity = 0){
    if(sensetivity == 0) {
        write(controlRegister,0x00);
    }
    else if(sensetivity == 4){
        write(controlRegister,0x01);
    }
    else if(sensetivity == 16){
        write(controlRegister,0x02);
    }
    else if(sensetivity == 60){
        write(controlRegister,0x03);
    }
    else{
        return;
    }
}

void GY_33::setInterrupt(bool i) {
    uint8_t r = read(enableRegister);
    if (i) {
        r |= 0x10;
    } else {
        r &= ~0x01;
    }
    write(enableRegister, r);
}


// Rood = min rood = FF
// Rood = max rood = FF

// Rood = min groen = FF
// Rood = min groen = FF


// Bij rood is er rood en blauw en het minste groen
// Bij Groen is er Groen en blauw en het minste rood
// Bij blauw is er blauw
