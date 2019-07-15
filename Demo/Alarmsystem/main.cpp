#include "MFRC522.hpp"
#include "spiSetup.hpp"
#include "hwlib.hpp"


void lock(hwlib::pin_out &servoPWM, int ms){
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
    auto greenLed = hwlib::target::pin_out(hwlib::target::pins::d46);
    auto redLed = hwlib::target::pin_out(hwlib::target::pins::d47); 
   
    spiSetup bus(sclk, mosi, miso);

    MFRC522 rfid(bus, ss, reset);

    rfid.initialize();

    bool stateLock = 1; //1 is open 0 is closed
    lock(servoPWM, 1.0);

    uint8_t cardUID[5] = {0};
    uint8_t allowedUID[4] = {0xD0, 0x3F, 0x7B, 0xA6};

    while(true){
        rfid.waitForUID(cardUID);
        if(rfid.isUIDEqual(cardUID, allowedUID)){
            greenLed.write(1);
            greenLed.flush();
            if(stateLock){
                lock(servoPWM, 2.0);
                stateLock = 0;
                hwlib::wait_ms(2500);
            }else{
                lock(servoPWM, 1.0);
                stateLock = 1;
                hwlib::wait_ms(2500);
            }
            greenLed.write(0);
            greenLed.flush();
        }else{
            redLed.write(1);
            redLed.flush();
            hwlib::wait_ms(2500);
            redLed.write(0);
            redLed.flush();
        }
        hwlib::wait_ms(50);
    }

return 0;

}