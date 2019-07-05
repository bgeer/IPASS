// -----------------------------------------------------------
// (C) Copyright Bas van der Geer 2019.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// -----------------------------------------------------------

#include "MFRC522.hpp"
#include "spiSetup.hpp"


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
    auto greenLed = hwlib::target::pin_out(hwlib::target::pins::d46);
    auto redLed = hwlib::target::pin_out(hwlib::target::pins::d47);
   
    spiSetup bus(sclk, mosi, miso);

    MFRC522 rfid(bus, ss, reset);

    
    rfid.test();

    
    while(true){
        bool card = rfid.cardCheck();
        if(card){
            greenLed.write(1);
            greenLed.flush();
            redLed.write(0);
            redLed.flush();
        }else{
            greenLed.write(0);
            greenLed.flush();
            redLed.write(1);
            redLed.flush();
        }
    }

return 0;

}