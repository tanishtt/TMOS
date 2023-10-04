#ifndef CLASSIC_KEYBOARD_H
#define CLASSIC_KEYBOARD_H

#define PS2_PORT 0x64//To send a command to the controller, write the command byte to IO port 0x64

#define PS2_COMMAND_ENABLE_FIRST_PORT 0xAE//Enable first PS/2 port.

#define CLASSIC_KEYBOARD_KEY_RELEASED 0x80
#define ISR_KEYBOARD_INTERRUPT 0x21
#define KEYBOARD_INPUT_PORT 0x60//0x60	Read/Write	Data Port


struct keyboard* classic_init();

#endif