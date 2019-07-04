// -----------------------------------------------------------
// (C) Copyright Bas van der Geer 2019.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// -----------------------------------------------------------

#ifndef SPISETUP_HPP
#define SPISETUP_HPP

#include "hwlib.hpp"

/// @file

/// \brief
/// Class to make a spiBus
/// \details
/// This class make a spiBus with  hwlib::spi_bus_bit_banged_sclk_mosi_miso. 
/// There are some basic Spi methods so you can easily communicate with the chip.
/// This class uses hwlib (https://github.com/wovo/hwlib)
class spiSetup : public hwlib::spi_bus_bit_banged_sclk_mosi_miso {
private:
    /// \brief Read Mask
    /// \detail
    ///
	const uint8_t READ_MASK = 0x80; // 1000 0000    mask to get right value of the register byte to read it

    /// \brief Write Mask
    /// \detail
    ///
    const uint8_t WRITE_MASK = 0x7E; // 0111 1110   mask to get right value of the register byte to write to 

    /// \brief Get the read byte.
    /// \detail
    ///
    uint8_t getReadByte(const uint8_t chipRegAddress);

    /// \brief Get the write byte.
    /// \detail
    ///
    uint8_t getWriteByte(const uint8_t chipRegAdress);

    /// \brief *********** DELETE *******************
    /// \detail
    ///
    void printByte(uint8_t &byte);
public:
    /// \brief Constructor for spiSetup class
    /// \detail
    ///
    spiSetup(hwlib::pin_out &scl, hwlib::pin_out &mosi, hwlib::pin_in &miso);

    /// \brief Get byte from register.
    /// \detail
    ///
    uint8_t getByteFromRegister(const uint8_t chipRegAddress, hwlib::pin_out& slaveSelect);

    /// \brief get bytes from register.
    /// \detail
    ///
    void getBytesFromRegister(const uint8_t chipRegAddress, uint8_t data[], uint8_t amountOfBytes, hwlib::pin_out& slaveSelect);

    /// \brief Write byte into register.
    /// \detail
    ///
    void writeByteInRegister(const uint8_t chipRegAddress, uint8_t writeByte, hwlib::pin_out& slaveSelect);

    /// \brief  Write bytes into register.
    /// \detail
    ///
    void writeBytesinRegister(const uint8_t chipRegAddress, uint8_t writeBytes[], int amountOfBytes, hwlib::pin_out& slaveSel);
};

#endif //SPISETUP_HPP