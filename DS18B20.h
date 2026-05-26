#ifndef _DS18B20_H_
#define	_DS18B20_H_

unsigned char Reset(void);
unsigned char ReadBit(void);
void WriteBit(unsigned char dBit);
unsigned char ReadByte(void);
void WriteByte(unsigned char Dout);
int ReadTemp(void);

#endif
