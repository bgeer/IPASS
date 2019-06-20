#include "MFRC522.hpp"


MFRC522::MFRC522(spiSetup& bus, hwlib::pin_out& slaveSel, hwlib::pin_out& reset):
    bus( bus ),
    slaveSel( slaveSel),
    reset ( reset )
    {}

uint8_t MFRC522::readRegister(REG regAddress){
    return bus.getByteFromRegister((uint8_t)regAddress, slaveSel);
}

uint8_t MFRC522::getVersion(){
    return readRegister(VersionReg);
}