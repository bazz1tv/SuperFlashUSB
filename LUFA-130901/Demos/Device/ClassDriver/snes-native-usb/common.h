#pragma once

#define IN_BULK_ENDPOINT 0x81
#define OUT_BULK_ENDPOINT 0x02
#define IN_BULK_ENDPOINT_ADDR  1			  
#define OUT_BULK_ENDPOINT_ADDR 2

#define BAUDRATE 1000000

#define GET_CHIPID 0
#define ERASE 1
#define ERASE_SECTOR 2
#define READ 3
#define FLASH 4
#define READ_SRAM 5
#define WRITE_SRAM 6