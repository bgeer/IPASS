#ifndef SPISETUP_HPP
#define SPISETUP_HPP

#include "hwlib.hpp"

class spiSetup : public hwlib::spi_bus_bit_banged_sclk_mosi_miso {
private:
	const uint8_t READ_MASK = 0x80; // 1000 0000
    const uint8_t WRITE_MASK = 0x7E; // 0111 1110
    uint8_t getReadByte(const uint8_t chipRegAddress);
    uint8_t getWriteByte(const uint8_t chipRegAdress);
    void printByte(uint8_t &byte);
public:
    spiSetup(hwlib::pin_out &scl, hwlib::pin_out &mosi, hwlib::pin_in &miso);

    uint8_t getByteFromRegister(const uint8_t chipRegAddress, hwlib::pin_out& slaveSelect);
    void getBytesFromRegister(const uint8_t chipRegAddress, uint8_t data[], uint8_t amountOfBytes, hwlib::pin_out& slaveSelect);
    void writeByteInRegister(const uint8_t chipRegAddress, uint8_t writeByte, hwlib::pin_out& slaveSelect);
    void writeBytesinRegister(const uint8_t chipRegAddress, uint8_t writeBytes[], int amountOfBytes, hwlib::pin_out& slaveSel);
};

#endif //SPISETUP_HPP