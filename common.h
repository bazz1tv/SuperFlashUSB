#pragma once

#define FIXED_CONTROL_ENDPOINT_SIZE      64

#define IN_BULK_ENDPOINT 0x81
#define OUT_BULK_ENDPOINT 0x02
#define IN_BULK_ENDPOINT_ADDR  1			  
#define OUT_BULK_ENDPOINT_ADDR 2

typedef unsigned char byte;

#define BAUDRATE 1000000

#define DATA_PORT PORTB
#define DATA_DIR DDRB
#define DATA_PIN PINB

// FF = FlipFlop
#define FF_PORT PORTD
#define FF_DIR DDRD
#define SNES_CTRL_PORT PORTC
#define SNES_CTRL_DIR DDRC

#define CART (1 << 2)
#define WR (1 << 4)
#define RD (1 << 5)

#define FF_ADDR_LOW (1)
#define FF_ADDR_HIGH (1 << 1)
#define FF_ADDR_BANK (1 << 2)
#define CLK_THING   (1 << 3)
//#define FF_CLK (1 << 3)



#define READ_PACKET_SIZE 12

#define ID 0
#define LOCK_BITS 1

#define READ_IDENTIFIER_CODES 0
#define ERASE 1
#define ERASE_SECTOR 2
#define READ 3
#define FLASH 4
#define READ_SRAM 5
#define WRITE_SRAM 6
#define IDLE 100