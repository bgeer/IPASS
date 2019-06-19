#include "spiSetup.hpp"

spiSetup::spiSetup(hwlib::pin_out& scl, hwlib::pin_out& mosi, hwlib::pin_in& miso):
	hwlib::spi_bus_bit_banged_sclk_mosi_miso(scl, mosi, miso)
{}

uint8_t spiSetup::getReadByte(const uint8_t regAddress) {
	return (((regAddress << 1) & WRITE_MASK) | READ_MASK);
}

uint8_t spiSetup::getWriteByte(const uint8_t regAddress) {
    return ((regAddress << 1) & WRITE_MASK);
}

uint8_t spiSetup::getByteFromRegister(const uint8_t regAddress, hwlib::pin_out& slaveSel) {
    transaction(slaveSel).write(getReadByte(regAddress));
    return transaction(slaveSel).read_byte();
}

void spiSetup::writeByteInRegister(const uint8_t regAddress, uint8_t writeByte, hwlib::pin_out& slaveSel) {
    transaction(slaveSel).write(getWriteByte(regAddress));
    transaction(slaveSel).write(writeByte);
}
