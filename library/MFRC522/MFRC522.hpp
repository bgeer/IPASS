// -----------------------------------------------------------
// (C) Copyright Bas van der Geer 2019.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// -----------------------------------------------------------

#ifndef MFRC522_HPP
#define MFRC522_HPP

#include "hwlib.hpp"
#include "spiSetup.hpp"

/// @file

/// @brief
/// Class to communicate with MFRC522
/// @detail
/// This is a simple library for the MFRC522 chip.
/// It can read the UUID of a given card.
/// This library uses hwlib (https://github.com/wovo/hwlib)
class MFRC522 {
private:
    /// @brief Using a Spi Bus to communicate with the MFRC522.
    spiSetup &bus;
    /// @brief The pin wich is used to select the right MFRC522.
    hwlib::pin_out& slaveSel;
    /// @brief The pin which is used as reset.
    hwlib::pin_out& reset;
public:
    /// @brief
    /// All the registers of the MFRC522 Chip
    /// @detail
    /// All the registers have there right address, reserved addresses are commented.
    /// If you want to see the registers in more detail take a look in the datasheet:
    /// https://www.nxp.com/docs/en/data-sheet/MFRC522.pdf
    //Page 0: Command and status
    //const static uint8_t reserved         = 0x00;
    const static uint8_t CommandReg         = 0x01;     /// @brief Start and stops commands.
    const static uint8_t ComIEnReg          = 0x02;     /// @brief Enable and disable interrupt request control bits.
    const static uint8_t DivIEnReg          = 0x03;     /// @brief Enable and disable interrupt request control bits.
    const static uint8_t ComIrqReg          = 0x04;     /// @brief Interrupt request bits.
    const static uint8_t DivIrqReg          = 0x05;     /// @brief Interrupt request bits.
    const static uint8_t ErrorReg           = 0x06;     /// @brief Error bits showing the error status of the last command executed.
    const static uint8_t Status1Reg         = 0x07;     /// @brief Communication status bits.
    const static uint8_t Status2Reg         = 0x08;     /// @brief Receiver and transmitter status bits.
    const static uint8_t FIFODataReg        = 0x09;     /// @brief Input and output of 64 byte FIFO buffer.
    const static uint8_t FIFOLevelReg       = 0x0A;     /// @brief Number of bytes stored in the FIFO buffer.
    const static uint8_t WaterLevelReg      = 0x0B;     /// @brief Level for FIFO underflow and overflow warning.
    const static uint8_t ControlReg         = 0x0C;     /// @brief Miscellaneous control registers
    const static uint8_t BitFramingReg      = 0x0D;     /// @brief Adjustments for bit-oriented frames.
    const static uint8_t CollReg            = 0x0E;     /// @brief Bit posistion of the first bit-collision detected on the RF interface.
    //const static uint8_t reserved         = 0x0F; 
    //Page 1: COmmand
    //const static uint8_t reserved         = 0x10;     
    const static uint8_t ModeReg            = 0x11;     /// @brief Defines general modes for transmitting and receiving.
    const static uint8_t TxModeReg          = 0x12;     /// @brief Defines transmission data rate and framing.
    const static uint8_t RxModeReg          = 0x13;     /// @brief Defines reception data rate and framing.
    const static uint8_t TxControlReg       = 0x14;     /// @brief Controls the logical behavior of the antenna driver pins TX1 and TX2.
    const static uint8_t TxASKReg           = 0x15;     /// @brief Controls the setting of the transmission modulation.
    const static uint8_t TxSelReg           = 0x16;     /// @brief Selects the internal sources for the antenna driver.
    const static uint8_t RxSelReg           = 0x17;     /// @brief Selects internal receiver settings.
    const static uint8_t RxThresholdReg     = 0x18;     /// @brief Selects thresholds for the bit decoder.
    const static uint8_t DemodReg           = 0x19;     /// @brief Defines demodlar settings.
    //const static uint8_t reserved         = 0x1A;     
    //const static uint8_t reserved         = 0x1B;
    const static uint8_t MfTxReg            = 0x1C;     /// @brief Controls some MIFARE communication transmit parameters.
    const static uint8_t MfRxReg            = 0x1D;     /// @brief Controls some MIFARE communication receive parameters.
    //const static uint8_t reserved         = 0x1E;
    const static uint8_t SerialSpeedReg     = 0x1F;     /// @brief Selects the speed of the serial UART interface.
    //Page 2: Configuration
    //const static uint8_t reserved         = 0x20; 
    const static uint8_t CRCResultRegH      = 0x21;     /// @brief Shows the MSB and LSB values of the CRC calculation(High).
    const static uint8_t CRCResultRegL      = 0x22;     /// @brief Shows the MSB and LSB values of the CRC calculation(Low).
    //const static uint8_t reserved         = 0x23;
    const static uint8_t ModWidthReg        = 0x24;     /// @brief Controls the ModWidth setting.
    //const static uint8_t reserved         = 0x25;
    const static uint8_t RFCfgReg           = 0x26;     /// @brief Configures the receiver gain.
    const static uint8_t GsNReg             = 0x27;     /// @brief Selects the conductance of the antenna driver pins TX1 and TX2 for modulation.
    const static uint8_t CWGsPReg           = 0x28;     /// @brief Defines the conductance of the p-driver output during periods of no modulation.
    const static uint8_t ModGsPReg          = 0x29;     /// @brief Defines the conductance of the p-driver output during periods of modulation.
    const static uint8_t TModeReg           = 0x2A;     /// @brief Defines settings for the internal timer.
    const static uint8_t TPrescalerReg      = 0x2B;     /// @brief Defines settings for the internal timer.
    const static uint8_t TReloadRegH        = 0x2C;     /// @brief Defines the 16-bit timer reload value(High).
    const static uint8_t TReloadRegL        = 0x2D;     /// @brief Defines the 16-bit timer reload value(Low).
    const static uint8_t TCounterValueRegH  = 0x2E;     /// @brief Shows the 16-bit timer value(High).
    const static uint8_t TCounterValueRegL  = 0x2F;     /// @brief Shows the 16-bit timer value(Low).
    //Page 3: Test register
    //const static uint8_t reserved         = 0x30;
    const static uint8_t TestSel1Reg        = 0x31;     /// @brief General test signal configuration.
    const static uint8_t TestSel2Reg        = 0x32;     /// @brief General test signal configuration and PRBS control.
    const static uint8_t TestPinEnReg       = 0x33;     /// @brief Enables pin output driver on pins D1 to D7.
    const static uint8_t TestPinValueReg    = 0x34;     /// @brief Defines the values for D1 to D7 when it is used as an I/O bus.
    const static uint8_t TestBusReg         = 0x35;     /// @brief Shows the status of the internal test bus.
    const static uint8_t AutoTestReg        = 0x36;     /// @brief Controls the digital self test.
    const static uint8_t VersionReg         = 0x37;     /// @brief Shows the software version.
    const static uint8_t AnalogTestReg      = 0x38;     /// @brief Controls the pins AUX1 and AUX2.
    const static uint8_t TestDAC1Reg        = 0x39;     /// @brief Defines the test value for TestDAC1.
    const static uint8_t TestDAC2Reg        = 0x3A;     /// @brief Defines the test value for TestDAC2.
    const static uint8_t TestADCReg         = 0x3B;     /// @brief Shows the value of ADC I and Q channels.
    //const static uint8_t reserved         = 0x3C;
    //const static uint8_t reserved         = 0x3D;
    //const static uint8_t reserved         = 0x3E;
    //const static uint8_t reserved         = 0x3F;

    /// @brief
    /// All the commands of the MFRC522 Chip
    /// @detail
    /// All the commands have there right value, reserved commands are commented.
    /// If you want to see the commands in more detail take a look in the datasheet table 149:
    /// https://www.nxp.com/docs/en/data-sheet/MFRC522.pdf
    const static uint8_t cmdIdle            = 0x00;     /// @brief No action, cancels the current command execution.
    const static uint8_t cmdMem             = 0x01;     /// @brief Stores 25 bytes into the internal buffer.
    const static uint8_t cmdGenRandomID     = 0x02;     /// @brief Generates a 10-byte random ID number.
    const static uint8_t cmdCalcCRC         = 0x03;     /// @brief Activates the CRC coprocessor or performs a self test.
    const static uint8_t cmdTransmit        = 0x04;     /// @brief Transmits data from the FIFO buffer.
    const static uint8_t cmdNoCmdChange     = 0x07;     /// @brief No command change.
    const static uint8_t cmdReceive         = 0x08;     /// @brief Activates the receiver circuits.
    const static uint8_t cmdTransceive      = 0x0C;     /// @brief Transmits data from FIFO buffer to antenna and automatically activates the receiver after transmission.
    //const static uint8_t reserved         = 0x0D;
    const static uint8_t cmdMFAuthent       = 0x0E;     /// @brief Performs a MIFARE standard authentication as a reader.
    const static uint8_t cmdSoftReset       = 0x0F;     /// @brief resets the MFRC522

    /// @brief
    /// All the commands of the MIFARE Cards
    /// @detail
    /// All the command have there right value, reserved commands are commented.
    /// If you want to see the commands in more detail take a look in the datasheet table 9:
    /// https://www.nxp.com/docs/en/data-sheet/MF1S50YYX_V1.pdf
    const static uint8_t mifareReqa         = 0x26;     
    const static uint8_t mifareWupa         = 0x52;      
    const static uint8_t mifareHalt         = 0x50;     
    const static uint8_t mifareAuthKeyA     = 0x60;     
    const static uint8_t mifareAuthKeyB     = 0x61;
    const static uint8_t mifareCl1          = 0x93; 
    //These commands can only be used after authenticate.    
    const static uint8_t mifareRead         = 0x30;     
    const static uint8_t mifareWrite        = 0xA0;     
    const static uint8_t mifareDecrement    = 0xC0;     
    const static uint8_t mifareIncrement    = 0xC1;     
    const static uint8_t mifareRestore      = 0xC2;     
    const static uint8_t mifareTransfer     = 0xB0;     

    /// @brief
    /// All the errors with a value and OkStatus;
    /// @detail
    /// This are all the errors that can happen and are checked in certain functions. OkStatus is used when
    /// everything went well with for example a communcation. In the datasheet is more in detail what all
    /// errors do and how they get triggerd.
    /// https://www.nxp.com/docs/en/data-sheet/MFRC522.pdf
    const static uint8_t OkStatus           = 0x00;     /// @brief Everything went Ok.
    const static uint8_t ProtocolErr        = 0x01;     /// @brief Protocol
    const static uint8_t ParityErr          = 0x02;     /// @brief
    const static uint8_t CRCErr             = 0x03;     /// @brief
    const static uint8_t CollErr            = 0x04;     /// @brief
    const static uint8_t BufferOvrlErr      = 0x05;     /// @brief
    const static uint8_t TempErr            = 0x06;     /// @brief
    const static uint8_t WrErr              = 0x07;     /// @brief
    const static uint8_t TimeOut            = 0x08;     /// @brief


    /// @brief The maximal amount of fifo bytes able to store.
    const uint8_t FIFOAmountOfBytes = 64;

    /// @brief Self test result out of datasheet for version 1. 
    /// After running the self test this will be in the FIFO Buffer.
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

    /// @brief Self test result out of datasheet for version 2. 
    /// After running the self test this will be in the FIFO Buffer.
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


    /// @brief Constructor for the MFRC522 Class
    /// @param bus this is the SPI bus the MFRC522 uses to communicate.
    /// @param slaveSel the pin used to select which MFRC522 you want to communicate with.
    /// @param reset the pin that is used for a hardReset of the MFRC522.
    MFRC522(spiSetup& bus, hwlib::pin_out& slaveSel, hwlib::pin_out& reset);

//################################################################################################################

    /// @brief Read one byte from register.
    /// @detail
    /// This method returns one byte fro the requested register in an uint8_t type.
    /// This method uses spiBus::getByteFromRegister.
    /// @param regAddress The register address you want to read a byte from.
    uint8_t readRegister(uint8_t regAddress);

    /// @brief Read multiple bytes from register.
    /// @detail
    /// This method returns a filled array that is given as parameter to the function with the reveived data.
    /// This method uses spiBus::getBytesFromRegister.
    /// @param regAddress The register address you want to read multiple bytes from.
    /// @param amountOfBytes The amount of bytes you want to read from the given address.
    /// @param data The array where the bytes are put in from the address you read from.
    void readRegister(uint8_t regAddress, int amountOfBytes, uint8_t data[]);

    /// @brief Write one byte to a register.
    /// @detail
    /// This method writes one byte to the specific register.
    /// It uses spiBus::writeByteInRegister.
    /// @param regAddress The address you want to write a single byte to.
    /// @param newByte The byte you want to write to the given address.
    void writeRegister(uint8_t regAddress, uint8_t newByte);

    //write multiple bytes to registers
    /// @brief Write multiple bytes to a register.
    /// @detail
    /// This method writes multiple bytes given in an array as parameter of the method.
    /// It uses spiBus::writeBytesInRegister.
    /// @regAddress The address you want to write multiple bytes to.
    /// @writeBytes The array where the bytes are in that you want to write to the register.
    /// @amountOfBytes The amount of bytes you want to write to the address.
    void writeRegister(uint8_t regAddress, uint8_t writeBytes[], int amountOfBytes);

//################################################################################################################

    /// @brief Set a bit mask.
    /// @detail
    /// This method set certain bits on in a registers.
    /// @param regAddress The address you want to set a bit mask on.
    /// @param mask The mask you want to set on the given address.
    void setBitMask(uint8_t regAddress, uint8_t mask);

    /// @brief Clear a bit mask.
    /// @detail
    /// This method clears certain bits in a register.
    /// @param regAddress The address you want to clear a bit mask on.
    /// @param mask The mask you want to set on the given address.
    void clearBitMask(uint8_t regAddress, uint8_t mask);

//################################################################################################################

    /// @brief State of Antennas
    /// @detail
    /// This method can turn the antennas of the MFRC522 on or off.
    /// Bool value true is for on and false is for off.
    /// In the datasheet in chapter 8.6.3 is explained how.
    /// @state The state to turn the antenna's on or off.
    void stateAntennas(bool state);

    /// @brief Firware version
    /// @detail
    /// This method return the firware version of the MRFC522.
    /// It returns an uint8_t value.
    uint8_t getVersion();

    /// @brief Wait for the MFRC to startup
    /// @detail
    /// This method wait untill the MRFC522 is booted.
    /// It checks the powerDown bit in the right register and wait untill this bit is cleard.
    void waitForBootUp();

    /// @brief Hard Reset
    /// @detail
    /// This method will hard reset the MFRC522. It makes the RST pin for 105ns low.
    /// In the datasheet is specified it needs to be atleast 100ns low.
    void hardReset();

    /// @brief Soft Reset
    /// @detail
    /// This method soft resets the MFRC522. It sends the soft reset command and
    //// wait untill the MFRC522 is booted again.
    void softReset();

    /// @brief Check for Errors
    /// @detail
    /// This method checks the ErrorReg for certain errors. 
    /// If there is an error in the register this error will be returned.
    uint8_t checkError();

//################################################################################################################

    /// @brief Clears FIFO Buffer
    /// @detail
    /// This method clears the fifo buffer. It fills the FIFO buffer with ZEROES.
    /// The amount of zeroes are given as a parameter to the method.
    /// @param amntOfBytes The amount of bytes you want to clear in the FIFO buffer.
    void clearFIFOBuffer(const uint8_t amntOfBytes = 64);

    /// @brief Clears Internal Buffer
    /// @detail
    /// This method writes 25 bytes of zeroes from the FIFO to the internal buffer.
    /// It uses the cmdMem command to push these bytes from FIFO to internal buffer/.
    void clearInternalBuffer();

//################################################################################################################

    /// @brief Initialize
    /// @detail
    /// This method initialize's the MFRC522.
    /// It will hard reset the chip and initialize all the correct registers.
    /// It starts the auto timer
    /// It sets the transfer rate to 106kb
    /// It will start the 30khz timer for 25us
    /// Sets 100% ask because MIFARE cards are used
    /// Turns the antennas on
    void initialize();

    /// @brief Self Test
    /// @detail
    /// This method performs the self test of the MFRC522.
    /// The self test is explained in the datasheet at 16.1.1
    /// It return a boolean value if the test is passed or not.
    bool selfTest();

    /// @brief Communication
    /// @detail
    /// This method is used to communicate with cards/tags and the MFRC522.
    /// It returns a status of the communcation if it succeeded or not.
    /// If it didnt succeed it wil return the error code.
    /// @param cmd The command you send to the card.
    /// @param sendData The array with the bytes you want to send.
    /// @param sendDataLenght The lenght of the data you want to send.
    /// @param receivedData The received data from the card stored in an array.
    /// @param receivedDataLength The length of the received data.
    uint8_t communicate(uint8_t cmd, uint8_t sendData[], int sendDataLength, uint8_t receivedData[], int receivedDataLength);

    /// @brief Is card presented.
    /// @detail
    /// This function checks if there is a card presented to the MFRC522.
    /// It can only be called once or there needs to be antoher command in between.
    /// It returns an boolean value if the card is presented or not.
    bool isCardPresented();

    /// @brief Card Check
    /// @detail
    /// This method checks if there is a card in range of the MFRC522.
    /// This method can be called multiple times after each other insteal of the isCardPresented function.
    bool cardCheck();

    /// @brief Get card UID
    /// @detail
    /// This method will return the card UID in the array that is given as parameter.
    /// @param UID The UID of the card.
    uint8_t getUID(uint8_t uid[5]);

    /// @brief Wait For UID
    /// @detail
    /// This method wait for the card UID, If a card is presented it will communicate with the card to get the UID of it.
    /// @param UID The UID of the card.
    void waitForUID(uint8_t UID[5]);

    /// @brief Check BCC
    /// @detail
    /// This method check if the received BCC is correct to validate if the UID is valid.
    /// This function xor the 4 UID bytes with each other, this results in the BCC if its correct.
    /// @param UID The UID of the card.
    bool checkBCC(uint8_t UID[5]);

    /// @brief Print UID
    /// @detail
    /// This method prints the UID in hex.
    /// @param UID The UID of the card.
    void printUID(uint8_t UID[5]);

    /// @brief UID Equal
    /// @detail
    /// This method is to check if an UID is equal with an other UID.
    /// This method does not change the given UID's
    /// @param cardUID The UID of the card.
    /// @param checkUID The UID you want to check.
    bool isUIDEqual(const uint8_t cardUID[5], const uint8_t checkUID[4]);

    /// @brief Test Function
    /// @detail
    /// This method tests certain function used in this library.
    void test();

};


#endif      //MFRC522_HPP