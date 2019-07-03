#ifndef MFRC522_HPP
#define MFRC522_HPP

#include "hwlib.hpp"
#include "spiSetup.hpp"


class MFRC522 {
private:
    spiSetup &bus;
    hwlib::pin_out& slaveSel;
    hwlib::pin_out& reset;


public:
    //const static uint8_t reserved = 0x00;
    const static uint8_t CommandReg = 0x01;
    const static uint8_t ComIEnReg = 0x02;
    const static uint8_t DivIEnReg = 0x03;
    const static uint8_t ComIrqReg = 0x04;
    const static uint8_t DivIrqReg = 0x05;
    const static uint8_t ErrorReg = 0x06;
    const static uint8_t Status1Reg = 0x07;
    const static uint8_t Status2Reg = 0x08;
    const static uint8_t FIFODataReg = 0x09;
    const static uint8_t FIFOLevelReg = 0x0A;
    const static uint8_t WaterLevelReg = 0x0B;
    const static uint8_t ControlReg = 0x0C;
    const static uint8_t BitFramingReg = 0x0D;
    const static uint8_t CollReg = 0x0E;
    //const static uint8_t reserved = 0x0F;
    //const static uint8_t reserved = 0x10;
    const static uint8_t ModeReg = 0x11;
    const static uint8_t TxModeReg = 0x12;
    const static uint8_t RxModeReg = 0x13;
    const static uint8_t TxControlReg = 0x14;
    const static uint8_t TxASKReg = 0x15;
    const static uint8_t TxSelReg = 0x16;
    const static uint8_t RxSelReg = 0x17;
    const static uint8_t RxThresholdReg = 0x18;
    const static uint8_t DemodReg = 0x19;
    //const static uint8_t reserved = 0x1A;
    //const static uint8_t reserved = 0x1B;
    const static uint8_t MfTxReg = 0x1C;
    const static uint8_t MfRxReg = 0x1D;
    //const static uint8_t reserved = 0x1E;
    const static uint8_t SerialSpeedReg = 0x1F;
    //const static uint8_t reserved = 0x20;
    const static uint8_t CRCResultRegH = 0x21;
    const static uint8_t CRCResultRegL = 0x22;
    //const static uint8_t reserved = 0x23;
    const static uint8_t ModWidthReg = 0x24;
    //const static uint8_t reserved = 0x25;
    const static uint8_t RFCfgReg = 0x26;
    const static uint8_t GsNReg = 0x27;
    const static uint8_t CWGsPReg = 0x28;
    const static uint8_t ModGsPReg = 0x29;
    const static uint8_t TModeReg = 0x2A;
    const static uint8_t TPrescalerReg = 0x2B;
    const static uint8_t TReloadRegH = 0x2C;
    const static uint8_t TReloadRegL = 0x2D;
    const static uint8_t TCounterValueRegH = 0x2E;
    const static uint8_t TCounterValueRegL = 0x2F;
    //const static uint8_t reserved = 0x30;
    const static uint8_t TestSel1Reg = 0x31;
    const static uint8_t TestSel2Reg = 0x32;
    const static uint8_t TestPinEnReg = 0x33;
    const static uint8_t TestPinValueReg = 0x34;
    const static uint8_t TestBusReg = 0x35;
    const static uint8_t AutoTestReg = 0x36;
    const static uint8_t VersionReg = 0x37;
    const static uint8_t AnalogTestReg = 0x38;
    const static uint8_t TestDAC1Reg = 0x39;
    const static uint8_t TestDAC2Reg = 0x3A;
    const static uint8_t TestADCReg = 0x3B;
    //const static uint8_t reserved = 0x3C;
    //const static uint8_t reserved = 0x3D;
    //const static uint8_t reserved = 0x3E;
    //const static uint8_t reserved = 0x3F;

    // chip commands
    const static uint8_t cmdIdle = 0x00;
    const static uint8_t cmdMem = 0x01;
    const static uint8_t cmdGenerateRandomID = 0x02;
    const static uint8_t cmdCalcCRC = 0x03;
    const static uint8_t cmdTransmit = 0x04;
    const static uint8_t cmdNoCmdChange = 0x07;
    const static uint8_t cmdReceive = 0x08;
    const static uint8_t cmdTransceive = 0x0C;
    //const static uint8_t reserved = 0x0D;
    const static uint8_t cmdMFAuthent = 0x0E;
    const static uint8_t cmdSoftReset = 0x0F;

    // // MIFARE card commands
    const static uint8_t mifareReqa = 0x26;
    const static uint8_t mifareWupa = 0x52;
    const static uint8_t mifareSelCL1 = 0x93;
    const static uint8_t mifareSelCL2 = 0x95;
    const static uint8_t mifareSelCL3 = 0x97;
    const static uint8_t mifareHalt = 0x50;
    const static uint8_t mifareAuthKeyA = 0x60;
    const static uint8_t mifareAuthKeyB = 0x61;
    const static uint8_t mifareRead = 0x30;
    const static uint8_t mifareWrite = 0xA0;
    const static uint8_t mifareDecrement = 0xC0;
    const static uint8_t mifareIncrement = 0xC1;
    const static uint8_t mifareRestire = 0xC2;
    const static uint8_t mifareTransfer = 0xB0;

    //Com status
    const static uint8_t OkStatus = 0x00;
    const static uint8_t ProtocolErr = 0x01;
    const static uint8_t ParityErr = 0x02;
    const static uint8_t CRCErr = 0x03;
    const static uint8_t CollErr = 0x04;
    const static uint8_t BufferOvrlErr = 0x05;
    const static uint8_t TempErr = 0x06;
    const static uint8_t WrErr = 0x07;
    const static uint8_t TimeOut = 0x08;


    //max fifo bytes
    const uint8_t FIFOAmountOfBytes = 64;

    //self test result out of datasheet for version 1
	const uint8_t selfTestFIFOBufferV1[64] {
		0x00, 0xC6, 0x37, 0xD5, 0x32, 0xB7, 0x57, 0x5C,
		0xC2, 0xD8, 0x7C, 0x4D, 0xD9, 0x70, 0xC7, 0x73,
		0x10, 0xE6, 0xD2, 0xAA, 0x5E, 0xA1, 0x3E, 0x5A,
		0x14, 0xAF, 0x30, 0x61, 0xC9, 0x70, 0xDB, 0x2E,
		0x64, 0x22, 0x72, 0xB5, 0xBD, 0x65, 0xF4, 0xEC,
		0x22, 0xBC, 0xD3, 0x72, 0x35, 0xCD, 0xAA, 0x41,
		0x1F, 0xA7, 0xF3, 0x53, 0x14, 0xDE, 0x7E, 0x02,
		0xD9, 0x0F, 0xB5, 0x5E, 0x25, 0x1D, 0x29, 0x79
	};

    //self test result out of datasheet for version 2
	const uint8_t selfTestFIFOBufferV2[64] {
		0x00, 0xEB, 0x66, 0xBA, 0x57, 0xBF, 0x23, 0x95,
		0xD0, 0xE3, 0x0D, 0x3D, 0x27, 0x89, 0x5C, 0xDE,
		0x9D, 0x3B, 0xA7, 0x00, 0x21, 0x5B, 0x89, 0x82,
		0x51, 0x3A, 0xEB, 0x02, 0x0C, 0xA5, 0x00, 0x49,
		0x7C, 0x84, 0x4D, 0xB3, 0xCC, 0xD2, 0x1B, 0x81,
		0x5D, 0x48, 0x76, 0xD5, 0x71, 0x61, 0x21, 0xA9,
		0x86, 0x96, 0x83, 0x38, 0xCF, 0x9D, 0x5B, 0x6D,
		0xDC, 0x15, 0xBA, 0x3E, 0x7D, 0x95, 0x3B, 0x2F
    };


    //constructor
    MFRC522(spiSetup& bus, hwlib::pin_out& slaveSel, hwlib::pin_out& reset);

    //read one byte out of a register
    uint8_t readRegister(uint8_t regAddress);

    //read multiple bytes out of a register
    void readRegister(uint8_t regAddress, int amountOfBytes, uint8_t data[]);

    //write one byte toregister
    void writeRegister(uint8_t regAddress, uint8_t newByte);

    //write multiple bytes to registers
    void writeRegister(uint8_t regAddress, uint8_t writeBytes[], int amountOfBytes);

    //set certain bits
    void setBitMask(uint8_t regAddress, uint8_t mask);

    //clear certain bits
    void clearBitMask(uint8_t regAddress, uint8_t mask);

    //turn the antennas on or off 8.6.3
    void stateAntennas(bool state);

    //get firmware version
    uint8_t getVersion();

    //wait till the chip is out of power down mode
    void waitForBootUp();

    //hard reset RST: must be low voor 100ns
    void hardReset();

    //soft reset
    void softReset();

    //clears the fifo buffe
    void clearFIFOBuffer(const uint8_t amntOfBytes = 64);

    //clears the internal buffer
    void clearInternalBuffer();

    //initialize
    void initialize();

    //does the self test 
    bool selfTest();

    //checks the errorReg for errors else returns okStatus.
    uint8_t checkError();

    //this functions return a status of the communication
    uint8_t communicate(uint8_t cmd, uint8_t sendData[], int sendDataLength, uint8_t receivedData[], int receivedDataLength);

    uint8_t isCardPresented();
    


};


#endif      //MFRC522_HPP