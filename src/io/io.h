#ifndef IO_H
#define IO_H

unsigned char insb(unsigned short port);
//Input byte from I/O port specified in DX into AL (register addressing mode, not direct addressing mode, memory location specified with ES:(E)DI or RDI.
unsigned short insw(unsigned short port);
//Input word from I/O port specified in DX into AX


void outb(unsigned short port, unsigned char val);
//Output byte in AL to I/O port address in DX.
void outw(unsigned short port, unsigned char val);
//Output byte in AX to I/O port address in DX.

#endif