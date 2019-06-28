#include "MFRC522.hpp"
void printByte2(uint8_t &byte){
    hwlib::cout<<"Byte: ";
    for(int i = 7; i >= 0; i--){
        hwlib::cout<<((byte & (1<<i)) !=0);
    }
    hwlib::cout<<'\n';
}

MFRC522::MFRC522(spiSetup& bus, hwlib::pin_out& slaveSel, hwlib::pin_out& reset):
    bus( bus ),
    slaveSel( slaveSel),
    reset ( reset )
    {}

uint8_t MFRC522::readRegister(uint8_t regAddress){
    return bus.getByteFromRegister((uint8_t)regAddress, slaveSel);
}

void MFRC522::readRegister(uint8_t regAddress, int amountOfBytes, uint8_t data[]){
    bus.getBytesFromRegister(regAddress, data, amountOfBytes, slaveSel);
}

void MFRC522::writeRegister(uint8_t regAddress, uint8_t newByte){
    bus.writeByteInRegister(regAddress, newByte, slaveSel);
}

void MFRC522::writeRegister(uint8_t regAddress, uint8_t writeBytes[], int amountOfBytes){
    bus.writeBytesinRegister(regAddress, writeBytes, amountOfBytes, slaveSel);
}

void MFRC522::setBitMask(uint8_t regAddress, uint8_t mask){
    uint8_t byteNow = readRegister(regAddress);
    writeRegister(regAddress, byteNow | mask);
}

void MFRC522::clearBitMask(uint8_t regAddress, uint8_t mask){
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
    writeRegister(CommandReg, (uint8_t)cmdSoftReset);
    hwlib::wait_ms(150);
    waitForBootUp();
}


void MFRC522::initialize(){
    hardReset();

    writeRegister(TModeReg, 0x80); //start the auto time
    writeRegister(TxModeReg, 0x00); //set tx and rx to 106kb transfer and receive speed.
    writeRegister(RxModeReg, 0x00);

    writeRegister(TPrescalerReg, 0xA9); //169 for a 30khz timer = 25us
    writeRegister(TReloadRegH, 0x03);   //169 in bits (0x03E8)
    writeRegister(TReloadRegL, 0xE8);   //169 in bits (0x03E8)

    writeRegister(TxASKReg, 0x40); //100%ask becuase we use mifare card and that is rfid and not nfc
    writeRegister(ModeReg, 0x3D);   //crc init value 0x6363
    //turn antennas on(true is on)
    stateAntennas(true);

}

void MFRC522::clearFIFOBuffer(const uint8_t amntOfBytes){
    writeRegister(FIFOLevelReg, 0x80);  //clears internal fifo buffer read and write pointer.
    uint8_t newFIFOBytes[amntOfBytes] = {0};
    writeRegister(FIFODataReg, newFIFOBytes, amntOfBytes);  //write amount of 0x00 to fifo buffer

}

void MFRC522::clearInternalBuffer(){
    clearFIFOBuffer(25);
    writeRegister(CommandReg, 0x01);
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
    clearInternalBuffer();
    //3 enable self test by writing 0x09 to the autotest register
    writeRegister(AutoTestReg, 0x09);
    //4 write 00h to the fifo buffer
    writeRegister(FIFODataReg, 0x00);
    //5 start sefttest with the CalcCRC command
    writeRegister(CommandReg, 0x03);
    //6 the self test in initiated
    uint8_t amount;
    for(uint8_t i = 0; i < 0xFF; i++){
        amount = readRegister(FIFOLevelReg);
        if( amount >= 64){
            break;
        }
    }
    uint8_t result[64];
    readRegister(FIFODataReg, 64, result);
    //control fifo bytes with the given bytes in datasheet

    writeRegister(AutoTestReg, 0x00);

    for(uint8_t i = 0; i < 64; i++){
        hwlib::cout<<result[i] <<" :: " << i << "\n";
    }
    if(firmwareVersion == 0x91){
        for(uint8_t i = 0; i < 64; i++){
            if(result[i] != selfTestFIFOBufferV1[i]){
                hwlib::cout<<"v1 wrong";
                return false;
            }
        }
        return true;
    }else if(firmwareVersion == 0x92){
        for(uint8_t i = 0; i < 64; i++){
            hwlib::cout<<result[i]<<"  ::  " << selfTestFIFOBufferV2[i] << "\n";
            if(result[i] != selfTestFIFOBufferV2[i]){
                hwlib::cout<<"v2 wrong";
                return false;
            }
        }
        return true;
    }else{
        return false;
    }
}