// -----------------------------------------------------------
// (C) Copyright Bas van der Geer 2019.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// -----------------------------------------------------------

#include "MFRC522.hpp"
#include "spiSetup.hpp"
#include "hwlib.hpp"


void position(hwlib::pin_out &servoPWM, int ms){
    int count = 0;                  //3 to 0.5 is open or close, full around
    while(count != 15){             //3 close
        servoPWM.write(1);          //0.5 is open
        servoPWM.flush();
        hwlib::wait_ms(ms);
        servoPWM.write(0);
        servoPWM.flush();
        hwlib::wait_ms(20-ms);
        count++;
    }
}

void printByte(uint8_t &byte){
    hwlib::cout<<"Byte: ";
    for(int i = 7; i >= 0; i--){
        hwlib::cout<<((byte & (1<<i)) !=0);
    }
    hwlib::cout<<'\n';
}

int main(){
    hwlib::wait_ms(2000); //Wait till terminal is started
    auto miso = hwlib::target::pin_in(hwlib::target::pins::d50);
    auto sclk = hwlib::target::pin_out(hwlib::target::pins::d48);
    auto ss = hwlib::target::pin_out(hwlib::target::pins::d49);
    auto mosi = hwlib::target::pin_out(hwlib::target::pins::d51);
    auto reset = hwlib::target::pin_out(hwlib::target::pins::d52);
    auto servoPWM = hwlib::target::pin_out(hwlib::target::pins::d53);
   
    spiSetup bus(sclk, mosi, miso);

    MFRC522 rfid(bus, ss, reset);

    uint8_t UID[5] = {0x00};
    // rfid.test();

    // position(servoPWM, 3.0);

    rfid.test();

    rfid.initialize();

    rfid.waitForUID(UID);

    if(!rfid.selectCard(UID)){
        hwlib::cout<<"select working";
    }





return 0;

}