#include "spiSetup.hpp"

spiSetup::spiSetup(hwlib::pin_out& scl, hwlib::pin_out& mosi, hwlib::pin_in& miso): //constructor for a spibus using hwlib
	hwlib::spi_bus_bit_banged_sclk_mosi_miso(scl, mosi, miso)
{}

void spiSetup::printByte(uint8_t &byte){        //function to printbytes in the develop process
    hwlib::cout<<"Byte: ";
    for(int i = 7; i >= 0; i--){
        hwlib::cout<<((byte & (1<<i)) !=0);
    }
    hwlib::cout<<'\n';
}

uint8_t spiSetup::getReadByte(const uint8_t regAddress) {   //function to transfer a regAddress to the right byte so you can read it 
	return (((regAddress << 1) & WRITE_MASK) | READ_MASK);
}

uint8_t spiSetup::getWriteByte(const uint8_t regAddress) {  //function to transfer a regAddress to the rigth byte so you can write to it
    return ((regAddress << 1) & WRITE_MASK);
}

uint8_t spiSetup::getByteFromRegister(const uint8_t regAddress, hwlib::pin_out& slaveSel) { //function to read one byte from a register
	const uint8_t amountOfBytes = 2;
	uint8_t write[amountOfBytes] = {getReadByte(regAddress), 0};
	uint8_t read[amountOfBytes] = {0, 0};
	transaction(slaveSel).write_and_read(amountOfBytes, write, read);
	return read[1];
}

void spiSetup::getBytesFromRegister(const uint8_t regAddress, uint8_t data[], uint8_t amountOfBytes, hwlib::pin_out& slaveSel) {    //function to 
	const uint16_t arraySize = amountOfBytes + 1;       //read multiple bytes from a register and put these in an given array
	uint8_t write[arraySize] = {0};
	uint8_t readByte = getReadByte(regAddress);
    for(uint8_t i = 0; i < amountOfBytes; i++){
        write[i] = readByte;
    }
	uint8_t read[arraySize] = {0};
	transaction(slaveSel).write_and_read(arraySize, write, read);
	for(uint8_t i = 0; i < amountOfBytes; i++) {
		data[i] = read[i + 1];
	}
}

void spiSetup::writeByteInRegister(const uint8_t regAddress, uint8_t writeByte, hwlib::pin_out& slaveSel) { //function to write one byte to a register
	uint8_t write[2] = {getWriteByte(regAddress), writeByte};
	transaction(slaveSel).write_and_read(2, write, nullptr);
}

void spiSetup::writeBytesinRegister(const uint8_t regAddress, uint8_t writeBytes[], int amountOfBytes, hwlib::pin_out& slaveSel){ //function to write  
    const uint8_t arraySize = amountOfBytes + 1;            //multiple bytes to a register.
    uint8_t write[arraySize] = {0x00};
    write[0] = getWriteByte(regAddress);
    for(uint8_t i = 0; i < arraySize; i++){
        if(i + 1 < arraySize){
            write[i+1] = writeBytes[i];
        }

    }
    transaction(slaveSel).write_and_read(arraySize, write, nullptr);
}