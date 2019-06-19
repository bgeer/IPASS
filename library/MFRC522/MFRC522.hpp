#ifndef MFRC522_HPP
#define MFRC522_HPP

#include "hwlib.hpp"


class MFRC522 {
private:
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
    TReloadReg,
    TCounterValReg,
    //Test register
    TestSel1Reg = 0x31,
    TestSel2Reg,
    TestPinEnReg,
    TestPinValueReg,
    TestBusReg,
    AutoTestReg,
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
    softReset  
};

enum MF_CMD : uint8_t {
    request = 0x26,
    wakeUp = 0x52,
    AntiCollisionCl1 = 0x93,
    Selec
};




};


#endif      //MFRC522_HPP