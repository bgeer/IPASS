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
    hwlib::wait_ms(1000); //Wait till terminal is started
    auto miso = hwlib::target::pin_in(hwlib::target::pins::d50);
    auto sclk = hwlib::target::pin_out(hwlib::target::pins::d48);
    auto ss = hwlib::target::pin_out(hwlib::target::pins::d49);
    auto mosi = hwlib::target::pin_out(hwlib::target::pins::d51);
    auto reset = hwlib::target::pin_out(hwlib::target::pins::d52);
   

    spiSetup bus(sclk, mosi, miso);
    // uint8_t addr = 0x37;
    // bus.writeByteInRegister(addr, 0x05, ss);
    // uint8_t x = bus.getByteFromRegister(addr, ss);
    // printByte(x);
    // hwlib::cout<<"int: "<<x <<"\n";

    MFRC522 rfid(bus, ss, reset);

    uint8_t y = rfid.getVersion();
    printByte(y);

return 0;

}