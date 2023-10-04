#ifndef CLASSIC_KEYBOARD_H
#define CLASSIC_KEYBOARD_H

#define PS2_PORT 0x64//To send a command to the controller, write the command byte to IO port 0x64

#define PS2_COMMAND_ENABLE_FIRST_PORT 0xAE//Enable first PS/2 port.


struct keyboard* classic_init();

#endif