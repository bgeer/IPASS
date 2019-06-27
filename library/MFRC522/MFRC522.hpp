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

    //registers
    enum REG : uint8_t {
        //command and status
        CommandReg = 0x01,
        ComlEnReg,
        DivlEnReg,
        ComlrqReg,
        DivlrqReg,
        ErrorReg,
        Status1Reg,
        Status2Reg,
        FIFODataReg,
        FIFOLevelReg,
        WaterLevelReg,
        ControlReg,
        BitFramingReg,
        CollReg,
        //commands
        ModeReg = 0x11,
        TxModeReg,
        RxModeReg,
        TxControlReg,
        TxASKReg,
        TxSelReg,
        RxSelReg,
        RxThresholdReg,
        DemodReg,
        MfTxReg = 0x1C,
        MfRxReg,
        SerialSpeedReg = 0x1F,
        //configuration
        CRCResultReg1 = 0x21,
        CRCResultReg2,
        ModWidthReg = 0x24,
        RFCfgReg = 0x26,
        CsNReg,
        CWGsPReg,
        ModGsPReg,
        TModeReg,
        TPrescalerReg,
        TReloadReg1,
        TReloadReg2,
        TCounterValReg,
        //Test register
        TestSel1Reg = 0x31,
        TestSel2Reg,
        TestPinEnReg,
        TestPinValueReg,
        TestBusReg,
        AutoTestReg,
        VersionReg = 0x37,
        AnalogTestReg,
        TestDAC1Reg,
        TestDAC2Reg,
        TestADCReg
    };

    enum CMD : uint8_t {
        idle = 0x00,
        mem,
        generateRandomID,
        calcCRC,
        transmit,
        noCmdChange = 0x07,
        recieve,
        transceive = 0x0C,
        mfAuthent = 0x0E, 
        SoftReset  
    };

    enum MF_CMD : uint8_t {
        request = 0x26,
        wakeUp = 0x52,
        AntiCollisionCl1 = 0x93,
        Selec
    };

    const uint8_t FIFOAmountOfBytes = 64;


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
    uint8_t readRegister(REG regAddress);

    //read multiple bytes out of a register
    void readRegister(REG regAddress, int amountOfBytes, uint8_t data[]);

    //write one byte toregister
    void writeRegister(REG regAddress, uint8_t newByte);

    //write multiple bytes to registers
    void writeRegister(REG regAddress, uint8_t writeBytes[], int amountOfBytes);

    //bit masks
    void setBitMask(REG regAddress, uint8_t mask);
    void clearBitMask(REG regAddress, uint8_t mask);

    //turn the antennas on or off 8.6.3
    void stateAntennas(bool state);

    //get firmware version
    uint8_t getVersion();

    
    void waitForBootUp();

    //hard reset RST: must be low voor 100ns
    void hardReset();

    //soft reset
    void softReset();

    void clearFIFOBuffer(const uint8_t amntOfBytes = 64);

    void clearInternalBuffer();

    //initialize
    void initialize();

    bool selfTest();
    


};


#endif      //MFRC522_HPP