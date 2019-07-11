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

/// @brief
/// Class to make a spiBus
/// @detail
/// This class make a spiBus with  hwlib::spi_bus_bit_banged_sclk_mosi_miso. 
/// There are some basic Spi methods so you can easily communicate with the chip.
/// This class uses hwlib (https://github.com/wovo/hwlib)
class spiSetup : public hwlib::spi_bus_bit_banged_sclk_mosi_miso {
private:
    /// @brief Read Mask
    /// @detail
    /// This mask is to read out of a register. 
	const uint8_t READ_MASK = 0x80; // 1000 0000    mask to get right value of the register byte to read it

    /// @brief Write Mask
    /// @detail
    /// This mask is to write to a register.
    const uint8_t WRITE_MASK = 0x7E; // 0111 1110   mask to get right value of the register byte to write to 

    /// @brief Get the read byte.
    /// @detail
    /// This functions transfers the register address to the right byte.
    /// So this byte can be send with spi to the chip.
    uint8_t getReadByte(const uint8_t regAddress);

    /// @brief Get the write byte.
    /// @detail
    /// This function transfers the register address to the right byte.
    /// So this byte can be send with spi to the chip.
    uint8_t getWriteByte(const uint8_t regAdress);
    void printByte(uint8_t &byte);
public:
    /// @brief Constructor for spiSetup class
    /// @detail
    /// This is the constructor of the spiSetup. It uses pin_out and pin_in from hwlib.
    /// This spiSetup can be used for other chips also.
    /// @param scl This is the slave select pin.
    /// @param mosi This is the mosi pin for the spibus.
    /// @pararm miso This is the miso pin for the spibus.
    spiSetup(hwlib::pin_out &scl, hwlib::pin_out &mosi, hwlib::pin_in &miso);

    /// @brief Get byte from register.
    /// @detail
    /// This method return one byte from the register that given as parameter.
    /// @param regAddress The adress you want to get the byte from.
    /// @param slaveSel The chip you want to communicate with.
    uint8_t getByteFromRegister(const uint8_t regAddress, hwlib::pin_out& slaveSel);

    /// @brief get bytes from register.
    /// @detail
    /// This method returns multiple bytes from a register.
    /// It will put the bytes fro  the register in the given data array.
    /// @param regAddress The address you want to get multiple bytes from.
    /// @param data The array where the bytes from the address are stored in.
    /// @param amountOfBytes The amount of bytes you want to read from the given address.
    /// @param The chip you want to communicate with.
    void getBytesFromRegister(const uint8_t regAddress, uint8_t data[], uint8_t amountOfBytes, hwlib::pin_out& slaveSel);

    /// @brief Write byte into register.
    /// @detail
    /// This method will write one single byte in to a register.
    /// @param regAddress The address you want to write a byte to.
    /// @param writeByte The byte you want to write to the given address.
    /// @param slaveSel The chip you want to communicate with.
    void writeByteInRegister(const uint8_t regAddress, uint8_t writeByte, hwlib::pin_out& slaveSel);

    /// @brief  Write bytes into register.
    /// @detail
    /// This method writes several bytes into a register. You can give an array of the data you want to write to a certain register.
    /// @param regAddress The address you want to write multiple bytes into.
    /// @param writeBytes The bytes you want to write to the address.
    /// @param amountOfBytes The amount of bytes you want to write to the address.
    /// @param slaveSel The chip you want to communicate with.
    void writeBytesinRegister(const uint8_t regAddress, uint8_t writeBytes[], int amountOfBytes, hwlib::pin_out& slaveSel);
};

#endif //SPISETUP_HPP