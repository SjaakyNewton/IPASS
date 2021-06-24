#include "hwlib.hpp"
#include "GY-33.h"
#include "PCA9685.hpp"

int main(){
    hwlib::wait_ms(2000);
    auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl);
    auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda);

    auto BUScolorsensor = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
    auto kleurenChip = GY_33(BUScolorsensor);

    auto scl1 = hwlib::target::pin_oc(hwlib::target::pins::scl1);
    auto sda1 = hwlib::target::pin_oc(hwlib::target::pins::sda1);

    auto BUSServoChip = hwlib::i2c_bus_bit_banged_scl_sda(scl1, sda1);
    auto servo1 = PCA9685(BUSServoChip, 0);
    auto servo2 = PCA9685(BUSServoChip, 1);


    servo1.modeDefault();
    servo1.frequentie(50);
    servo1.modeOffSleep();


    kleurenChip.setIntergrationTime(50);
    kleurenChip.setGAIN(1);

    for(;;) {
        servo1.setServoHalfOpen();
        servo2.setServoHalfOpen();
        kleurenChip.setInterrupt(false);
        hwlib::wait_ms(60);
        kleurenChip.readColors();
        kleurenChip.setInterrupt(true);
        hwlib::wait_ms(200);
        servo1.setServoOpen();
        servo2.setServoDicht();
        hwlib::wait_ms(200);
    }
}
