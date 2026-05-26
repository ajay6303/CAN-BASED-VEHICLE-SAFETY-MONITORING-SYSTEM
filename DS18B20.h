#ifndef _DS18B20_H_
#define	_DS18B20_H_

// --- Public 1-Wire Transmission Methods API ---
unsigned char Reset(void);                 // Dispatches bus reset sequence pulses
unsigned char ReadBit(void);                // Extracts individual status bit components out from data frames
void WriteBit(unsigned char dBit);         // Drives targeted bit logic outputs across the line network
unsigned char ReadByte(void);               // Aggregates bits to assemble standard data bytes
void WriteByte(unsigned char Dout);        // Serializes full bytes out onto active signal pins
int ReadTemp(void);                        // Master control layer extracting raw hex data conversions

#endif
