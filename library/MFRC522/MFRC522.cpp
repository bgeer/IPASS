#include "MFRC522.hpp"


MFRC522::MFRC522(spiSetup& bus, hwlib::pin_out& slaveSel, hwlib::pin_out& reset):
    bus( bus ),
    slaveSel( slaveSel),
    reset ( reset )
    {}

uint8_t MFRC522::readRegister(REG regAddress){
    return bus.getByteFromRegister((uint8_t)regAddress, slaveSel);
}

uint8_t MFRC522::readRegister(uint8_t address){
    return bus.getByteFromRegister(address, slaveSel);
}

void MFRC522::writeRegister(REG regAddress, uint8_t newByte){
    bus.writeByteInRegister(regAddress, newByte, slaveSel);
}

void MFRC522::setBitMask(REG regAddress, uint8_t mask){
    uint8_t byteNow = readRegister(regAddress);
    writeRegister(regAddress, byteNow | mask);
}

void MFRC522::clearBitMask(REG regAddress, uint8_t mask){
    uint8_t byteNow = readRegister(regAddress);
    byteNow = byteNow & ~mask;
    writeRegister(regAddress, byteNow);
}

void MFRC522::stateAntennas(bool state){
    if(state){
        setBitMask(TxControlReg, 0x03); //8.6.3
    }else{
        clearBitMask(TxControlReg, 0x03);
    }
}

uint8_t MFRC522::getVersion(){
    return readRegister(VersionReg);
}

void MFRC522::waitForBootUp(){
    while(readRegister(CommandReg) & (1<<4)){}      //8.6.2
}

void MFRC522::hardReset(){
    reset.write(0);
    reset.flush();
    hwlib::wait_ns(105); //105 to make sure its low long enough;
    reset.write(1);
    reset.flush();
    waitForBootUp();
}


void MFRC522::softReset(){
    writeRegister(CommandReg, (uint8_t)SoftReset);
    hwlib::wait_ms(150);
    waitForBootUp();
}


void MFRC522::initialize(){
    hardReset();

    writeRegister(TModeReg, 0x80); //start the auto time
    writeRegister(TxModeReg, 0x00); //set tx and rx to 106kb transfer and receive speed.
    writeRegister(RxModeReg, 0x00);

    writeRegister(TPrescalerReg, 0xA9); //169 for a 30khz timer = 25us
    writeRegister(TReloadReg1, 0x03);   //169 in bits (0x03E8)
    writeRegister(TReloadReg2, 0xE8);   //169 in bits (0x03E8)

    writeRegister(TxASKReg, 0x40); //100%ask becuase we use mifare card and that is rfid and not nfc
    writeRegister(ModeReg, 0x3D);   //crc init value 0x6363
    //turn antennas on(true is on)
    stateAntennas(true);

}

bool MFRC522::selfTest(){
    //get version
    uint8_t firmwareVersion = getVersion();
    if(firmwareVersion != 0x92 && firmwareVersion != 0x91){
        return false;
    }
    //1 perform a softreset
    softReset();
    //2 clear internal buffer by writing 25 bytes of 00h and implement the config command
    
    //3 enable self test by writing 0x09 to the autotest register

    //4 write 00h to the fifo buffer

    //5 start sefttest with the CalcCRC command

    //6 the self test in initiated

    //control fifo bytes with the given bytes in datasheet

    return true;
}