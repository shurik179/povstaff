#ifndef _FLASHSTORAGE_H
#define _FLASHSTORAGE_H
#include <Arduino.h>
#include <SPI.h>
#include "SdFat.h"
#include "Adafruit_SPIFlash.h"
#include "Adafruit_TinyUSB.h"

//general file access functions
//read 16-bit (2 byte) unsigned int from file
uint16_t read16(File &f);
//read 32-bit (4 byte) unsigned int from file
uint32_t read32(File &f);
//read line (until \n ) from file and save it to l
//skips all whitespace (including CR and tab)
int readLine(File &f, char  l[]);



void msc_init();   //initialize mass storage object
//callbacks for read/write requests from connected computer
int32_t msc_read_cb (uint32_t lba, void* buffer, uint32_t bufsize);
int32_t msc_write_cb (uint32_t lba, uint8_t* buffer, uint32_t bufsize);
void msc_flush_cb (void);


//Global variables

extern Adafruit_FlashTransport_QSPI flashTransport;
extern Adafruit_SPIFlash flash;   //flash device
extern FatFileSystem fatfs;       // fat filesystem on flash
extern Adafruit_USBD_MSC usb_msc; // USB mass storage object
#endif
