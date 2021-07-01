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
    auto servoInGate = PCA9685(BUSServoChip, 1);
    auto servoOutGate = PCA9685(BUSServoChip, 0);

    auto servoRedGate = PCA9685(BUSServoChip, 2);
    auto servoGreenGate = PCA9685(BUSServoChip, 3);
    auto servoBlueGate = PCA9685(BUSServoChip, 15);

    servoInGate.modeDefault();
    servoInGate.frequentie(50);
    servoInGate.modeOffSleep();

    kleurenChip.setIntergrationTime(2.4);
    kleurenChip.setGAIN(1);

    for(;;) {
        servoInGate.setServo0();
        servoOutGate.setServo90();
        hwlib::wait_ms(1000);

        servoInGate.setServo90();
        hwlib::wait_ms(2000);
        servoInGate.setServo0();
        hwlib::wait_ms(1000);

        int kleur = kleurenChip.readBasicColors();
        if(kleur == 1){
            servoRedGate.setServo0();
            servoGreenGate.setServo90();
            servoBlueGate.setServo90();
        }
        else if(kleur == 2){
            servoRedGate.setServo90();
            servoGreenGate.setServo0();
            servoBlueGate.setServo90();
        }
        else if(kleur == 3){
            servoRedGate.setServo90();
            servoGreenGate.setServo90();
            servoBlueGate.setServo0();
        }else{
            servoRedGate.setServo90();
            servoGreenGate.setServo90();
            servoBlueGate.setServo90();
        }
        servoOutGate.setServo0();
        hwlib::wait_ms(2000);
    }
}
