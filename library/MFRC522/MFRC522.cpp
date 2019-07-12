// -----------------------------------------------------------
// (C) Copyright Bas van der Geer 2019.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// -----------------------------------------------------------

#include "MFRC522.hpp"


MFRC522::MFRC522(spiSetup& bus, hwlib::pin_out& slaveSel, hwlib::pin_out& reset):   //constructor for the class
    bus( bus ),
    slaveSel( slaveSel),
    reset ( reset )
    {}

uint8_t MFRC522::readRegister(uint8_t regAddress){        //read a single byte out of a register
    return bus.getByteFromRegister((uint8_t)regAddress, slaveSel);
}

void MFRC522::readRegister(uint8_t regAddress, int amountOfBytes, uint8_t data[]){  //read multiple bytes out of a register
    bus.getBytesFromRegister(regAddress, data, amountOfBytes, slaveSel);
}

void MFRC522::writeRegister(uint8_t regAddress, uint8_t newByte){   //write a single byte to a register
    bus.writeByteInRegister(regAddress, newByte, slaveSel);
}

void MFRC522::writeRegister(uint8_t regAddress, uint8_t writeBytes[], int amountOfBytes){   //write multiple bytes to a register
    bus.writeBytesinRegister(regAddress, writeBytes, amountOfBytes, slaveSel);
}

//################################################################################################################

void MFRC522::setBitMask(uint8_t regAddress, uint8_t mask){         //turn certain bits on in the given register address
    uint8_t byteNow = readRegister(regAddress);
    writeRegister(regAddress, byteNow | mask);
}

void MFRC522::clearBitMask(uint8_t regAddress, uint8_t mask){       //turn certain bits off in the given register address
    uint8_t byteNow = readRegister(regAddress);
    byteNow = byteNow & ~mask;
    writeRegister(regAddress, byteNow);
}

//################################################################################################################

void MFRC522::stateAntennas(bool state){    //turn the antenna's off or on with a boolean value
    if(state){
        setBitMask(TxControlReg, 0x03); //8.6.3
    }else{
        clearBitMask(TxControlReg, 0x03);
    }
}

uint8_t MFRC522::getVersion(){
    return readRegister(VersionReg);        //this function returns the version of the MFRC522 Chip
}

void MFRC522::waitForBootUp(){
    while(readRegister(CommandReg) & (1<<4)){}      //8.6.2. Checking this register to wait untill the powerdown bit is cleared
}

void MFRC522::hardReset(){  //function to hardReset the MFRC522 by making the RST pin low for 105ns
    reset.write(0);
    reset.flush();
    hwlib::wait_ns(105); //105 to make sure its low long enough, 8.8.6 says it must be 100ns.
    reset.write(1);
    reset.flush();
    waitForBootUp();
}



void printByte2(uint8_t &byte){     //a function to print bytes
    hwlib::cout<<"Byte: ";
    for(int i = 7; i >= 0; i--){
        hwlib::cout<<((byte & (1<<i)) !=0);
    }
    hwlib::cout<<'\n';
}

void MFRC522::softReset(){  //function to softReset the MFRC522 with a command
    writeRegister(CommandReg, cmdSoftReset);
    hwlib::wait_ms(150);
    waitForBootUp();
}

uint8_t MFRC522::checkError(){          //fucntion to check the error register per bit. Each bit has his own error value
    uint8_t errorReg = readRegister(ErrorReg);
    if(errorReg & 0b00000001){
        return ProtocolErr;
    }else if(errorReg & 0b00000010){
        return ParityErr;
    }else if(errorReg & 0b00000100){
        return CRCErr;
    }else if(errorReg & 0b00001000){
        return CollErr;
    }else if(errorReg & 0b00010000){
        return BufferOvrlErr;
    }else if(errorReg & 0b01000000){
        return TempErr;
    }else if(errorReg & 0b10000000){
        return WrErr;
    }else{
        return OkStatus;
    }
}

//################################################################################################################

//clears the fifo buffer with amntOfbytes of zeroes
void MFRC522::clearFIFOBuffer(const uint8_t amntOfBytes){
    writeRegister(FIFOLevelReg, 0x80);  //clears internal fifo buffer read and write pointer.
    uint8_t newFIFOBytes[amntOfBytes] = {0x00};
    writeRegister(FIFODataReg, newFIFOBytes, amntOfBytes);  //write amount of 0x00 to fifo buffer
}

//clears the internal buffer with 25 bytes of zeroes.
void MFRC522::clearInternalBuffer(){
    clearFIFOBuffer(25);
    writeRegister(CommandReg, cmdMem);
}


bool MFRC522::selfTest(){
    //get firmwareVersion
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
    writeRegister(CommandReg, cmdCalcCRC);
    //6 the self test in initiated
    uint8_t amount;
    for(uint8_t i = 0; i < 0xFF; i++){
        amount = readRegister(FIFOLevelReg);    //wait till the FIFO is filled with 64 bytes
        if( amount >= 64){
            break;
        }
    }
    writeRegister(CommandReg, cmdIdle); //stop all cmd's
    uint8_t result[64] = {0};
    readRegister(FIFODataReg, 64, result);
    //control fifo bytes with the given bytes in datasheet

    writeRegister(AutoTestReg, 0x00);
    if(firmwareVersion == 0x91){    //test for firwareversion 1
        for(uint8_t i = 0; i < 64; i++){
            if(result[i] != selfTestFIFOBufferV1[i]){   //checks the buffer with the given value's  out of datasheet
                hwlib::cout<<"Test for firwareVersion 1 did not pass\n";
                return false;
            }
        }
        hwlib::cout<<"Test for firwareVersion 1 passed\n";
        return true;
    }else if(firmwareVersion == 0x92){  //test for firwareversion 2
        for(uint8_t i = 0; i < 64; i++){
            if(result[i] != selfTestFIFOBufferV2[i]){   //checks the buffer with the given value's  out of datasheet
                hwlib::cout<<"Test for firwareVersion 2 did not pass\n";
                return false;
            }
        }
        hwlib::cout<<"Test for firwareVersion 2 passed\n";
        return true;
    }else{
        hwlib::cout<<"No version detected, is the MFRC522 connected correctly?";
        return false;
    }
}

void MFRC522::initialize(){    //initialize the chip when you start it up 
    hardReset();

    writeRegister(TModeReg, 0x80); //start the auto time
    writeRegister(TxModeReg, 0x00); //set tx and rx to 106kb transfer and receive speed.
    writeRegister(RxModeReg, 0x00);
    writeRegister(ModWidthReg, 0x80); //reset ModWidthReg 

    writeRegister(TPrescalerReg, 0xA9); //169 for a 30khz timer = 25us
    writeRegister(TReloadRegH, 0x03);   //169 in bits (0x03E8)
    writeRegister(TReloadRegL, 0xE8);   //169 in bits (0x03E8)

    writeRegister(TxASKReg, 0x40); //100%ask becuase we use mifare card and that is rfid and not nfc
    writeRegister(ModeReg, 0x3D);   //crc init value 0x6363

    writeRegister(RFCfgReg, (0x07<<4));
    //turn antennas on(true is on)
    stateAntennas(true);

}


uint8_t MFRC522::communicate(uint8_t cmd, uint8_t sendData[], int sendDataLength, uint8_t receivedData[] = {0}, int receivedDataLength = 0){
    uint8_t finishedIrq = 0x00; //value of interupts when finished or triggered
    if(cmd == cmdTransceive){   //the right value's for the transceive command
        finishedIrq = 0x30;
    }
    if(cmd == cmdMFAuthent){
        finishedIrq = 0x10;
    }
    writeRegister(CommandReg, cmdIdle); //stop any active command
    writeRegister(ComIrqReg, 0x7F);
    writeRegister(FIFOLevelReg, 0x80); //Flush buffer = 1, Initalize the FIFO


    for(int i = 0; i < sendDataLength; i++){
        writeRegister(FIFODataReg, sendData[i]);
    }
    //execute command
    writeRegister(CommandReg, cmd); //executes the given command as parameter

    if(cmd == cmdTransceive){
        setBitMask(BitFramingReg, 0x80); //StartSend = 1, transmission starts
    }

    int timeOutMS = 25; //maximum timeout time in ms
    uint8_t curInterupt = readRegister(ComIrqReg);  //get the currentinterupt status
    for(int i = 0; !(curInterupt & finishedIrq); i++){   //loops until the time out is reached or triggered by the bit. Or the curInterupt is not equal
        curInterupt = readRegister(ComIrqReg);  //to the finishedIRq anymore
        if((i > timeOutMS) || (curInterupt & 0x01)){    //0x01 is interrupt for timeout
            return TimeOut; //returns there is a timeout can be the interrupt or the ms timeout
        }
        hwlib::wait_ms(1);
    }

    uint8_t error = checkError();   //check for errors in the register and returns this else continue's
    if(error){
        return error;   //returns the error given
    }

    //reading the result of the fifo
    receivedDataLength = readRegister(FIFOLevelReg); //get the lenght of the received data in the FIFO buffer
    readRegister(FIFODataReg, receivedDataLength, receivedData); //reads the received data out of the fifo buffer into the array
    writeRegister(CommandReg, cmdIdle); //stop any commands
    return OkStatus;    //if everything went well return okstatus
}

bool MFRC522::isCardPresented(){     //function does not work yet completly, can only see once if a card is presented.
    //REQA = 26h       both 7 bits 
    //WUPA = 52h
    writeRegister(BitFramingReg, 0x07); //0x07 00000111 indicates 7 bits of REQA and WUPA

    const uint8_t sendDataLength = 1;   //one byte of data is send, the command
	uint8_t sendData[sendDataLength] = {mifareReqa}; //send the mifare request command

	int receivedLength = 2; //returns 2 bytes of data
	uint8_t receivedData[receivedLength] = {0x00}; //array to be filled with the received data

    uint8_t status = communicate(cmdTransceive, sendData, sendDataLength, receivedData, receivedLength); //get the communication status of the chip and card

    if(status != OkStatus){ //checks if the status is OK, if not there is no card presented.
        return false;
    }

    return true;    //if card is presented
}

bool MFRC522::cardCheck(){
    if(isCardPresented()){
        stateAntennas(false);   //mfrc522 needs short pauses between transmitting so turning off antenna's and on again does this
        stateAntennas(true);    //8.5 in https://www.nxp.com/docs/en/data-sheet/MF1S50YYX_V1.pdf specifies it
        return true;
    }
    return false;
}

uint8_t MFRC522::getUID(uint8_t uid[5]){            //Cascade level 1 check that returns the UI
    uint8_t comm[2] = {0x93, 0x20};

    //no REQA or WUPA so 111bit framing can be turned off
    clearBitMask(CollReg, 0x80);
    writeRegister(BitFramingReg, 0x00);

    uint8_t status = communicate(cmdTransceive, comm, 2, uid, 5);
    if(status != OkStatus){
        return status;
    }
    return OkStatus;
}

void MFRC522::waitForUID(uint8_t UID[5]){       //wait for the cards UID and puts this into the array.
    while(true){
        if(isCardPresented()){                  //wait for a card to be presented
            if(getUID(UID)== OkStatus){         //communicates with the card to ge the UID
                return;
            }
        }
    }
}

bool MFRC522::checkBCC(uint8_t UID[5]){     //functios that calculates the BCC to check if the UID is valid
    uint8_t BCC = 0;                        //datasheet says is generated by xor the 4 UID bytes so its unique for each UID
    const uint8_t sizeUID = 4;
    for(int i = 0; i < sizeUID; i++){
        BCC ^= UID[i];
    }
    return (BCC == UID[4]);         //returns if the BCC is the same as the calculated BCC
}

bool MFRC522::isUIDEqual(const uint8_t cardUID[5], const uint8_t checkUID[5]){      //check if two UID's are equal
    for(int i = 0; i < 4; i++){
        if(cardUID[i] != checkUID[i]){
            return false;
        }
    }
    return true;
}


void MFRC522::printUID(uint8_t UID[5]){         //print an UID without the BCC
    hwlib::cout<<
        hwlib::hex << UID[0] << " " <<
        hwlib::hex << UID[1] << " " <<
        hwlib::hex << UID[2] << " " <<
        hwlib::hex << UID[3] << hwlib::endl;
}

uint8_t MFRC522::calculateCRC(uint8_t data[], int lenght, uint8_t result[]){
    writeRegister(CommandReg, cmdIdle); //stop any active commands
    writeRegister(DivIrqReg, 0x04);     //enable crc interrupt
    setBitMask(FIFOLevelReg, 0x80);     //flush the FIFO buffer
    writeRegister(FIFODataReg, data, lenght); //write data to the fifo
    writeRegister(CommandReg, cmdCalcCRC);  //start the CRC command
    int count = 0;
    for(int i = 0; i < 100; i++){   //wait max 100ms
        uint8_t curDivIrq = readRegister(DivIrqReg);
        if(curDivIrq & 0x04){   //CRCirq is triggered so caclucation is done
            break;
        }
        count++;
        hwlib::wait_ms(1);
    }
    if(count >= 100){
        return TimeOut;
    }

    writeRegister(CommandReg, cmdIdle);     //stop any active commands to get the result

    result[0] = readRegister(CRCResultRegL);    //the low bits part of the CRC result
    result[1] = readRegister(CRCResultRegH);    //the high bits part of the CRC result
    return OkStatus;

}

bool MFRC522::selectCard(uint8_t UID[5]){
    int uidIndex = 2;
    uint8_t *receivedBuffer;
    int receivedBufLength;
    uint8_t buffer[9] = {0x00};

    clearBitMask(CollReg, 0x80);

    buffer[0] = 0x93;   //select card command
    buffer[1] = 0x70;

    for(int i = 0; i < 5; i++){         //put the UID with BCC in the buffer
        buffer[uidIndex+i] = UID[i];
    }
    uint8_t BCC = buffer[2] ^ buffer[3] ^ buffer[4] ^ buffer[5]; //calculate BCC
    if(BCC != buffer[6]){   //checks if the BCC is correct with the received one from the card
        return BCCErr;
    }
    uint8_t calcCRCStatus = calculateCRC(buffer, 7, &buffer[7]);    //calculate CRC
    if(calcCRCStatus != OkStatus){                                  //check if the crc is calculated correct.
        hwlib::cout<<"CRCerr\n";
        hwlib::cout<<hwlib::hex<<calcCRCStatus;
        return CRCErr;
    }
    // for(int i = 0; i < 9; i++){                //prints the buffer to test
    //     hwlib::cout<<hwlib::hex<<buffer[i]<<hwlib::endl;
    // }
    // hwlib::cout<<"-----------------\n";
    receivedBuffer = &buffer[6];
    receivedBufLength = 3;
    uint8_t comStatus = communicate(cmdTransceive, buffer, 9, receivedBuffer, receivedBufLength);
    if(comStatus != OkStatus){
        hwlib::cout<<"NOT OK"<<hwlib::endl;
        printByte2(comStatus);
        return comStatus;
    }
    //calculate your own CRC_A to check if its correct.
    calcCRCStatus = calculateCRC(receivedBuffer, 1, &buffer[2]);
    if(calcCRCStatus != OkStatus){
        hwlib::cout<<hwlib::hex<<calcCRCStatus;
        return CRCErr;
    }
    if(buffer[2] != receivedBuffer[1] || buffer[3] != receivedBuffer[2]){   //check the CRC calculated bytes
        hwlib::cout<<"CRC is wrong\n";
        return CRCErr;
    }
    // for(int i = 0; i < 9; i++){                //prints the buffer to test
    //     hwlib::cout<<hwlib::hex<<buffer[i]<<hwlib::endl;
    // }
    return OkStatus;
}

bool MFRC522::authenticateCard(uint8_t cmd, uint8_t blockAddress, uint8_t sectorKey[6], uint8_t uid[4]){
    uint8_t buffer[12] = {0};
    int bufLenght = 12;
    //fill the buffer that is used to communicate with the correct bytes.
    buffer[0] = cmd;
    buffer[1] = blockAddress;
    for(int i = 0; i < 6; i++){
        buffer[2+i] = sectorKey[i];
    }
    for(int i = 0; i < 4; i++){
        buffer[8+i] = uid[i];
    }
    uint8_t status = communicate(cmdMFAuthent, buffer, bufLenght);
    if(status != OkStatus){
        hwlib::cout<<"Not authenticated....\n";
        return status;
    }else{
        hwlib::cout<<"Authenticated...\n";
        return true;
    }

}


void MFRC522::test() {
	initialize();
	//does an initialize at start up of program.
	hwlib::cout << "MFRC522 test\n";

	// self test
    selfTest();
	// Self test uses soft reset so need to initialize again
	initialize();

    //get card uid
	uint8_t uid[5] = {0x00};
    uint8_t authenticatedUID[4] = {0xD0, 0x3F, 0x7B, 0xA6};
    waitForUID(uid);
    printUID(uid);
    //checks if the UID is valid with BCC
    if(checkBCC(uid)){
        hwlib::cout<<"UID is valid\n";
    }
    //check if the UID is equal to a given UID
    if(isUIDEqual(uid, authenticatedUID)){
        hwlib::cout<<"UID is equal\n";
    }else{
        hwlib::cout<<"UID is not equal\n";
    }
}

