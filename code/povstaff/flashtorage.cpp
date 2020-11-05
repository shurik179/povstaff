#include "flashstorage.h"
#include "config.h"
Adafruit_FlashTransport_QSPI flashTransport;
Adafruit_SPIFlash flash(&flashTransport);
FatFileSystem fatfs;
Adafruit_USBD_MSC usb_msc;


/*
 *    General file access functions
 */

uint16_t read16(File &f) {
    uint16_t result;
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read(); // MSB
    return result;
}

uint32_t read32(File &f) {
    uint32_t result;
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read();
    ((uint8_t *)&result)[2] = f.read();
    ((uint8_t *)&result)[3] = f.read(); // MSB
    return result;
}

int readLine(File &f, char  l[]){
    int pos=0;
    char c;
    while (f.available()&&(pos<MAX_LINE_LENGTH)) {
        c=f.read();//read next character
        if (c=='\n') {
            l[pos]=NULL; //terminate string
            return pos;
        }
        if (pos<MAX_LINE_LENGTH) { l[pos++]=c;}
    }
    //if we are here and not yet exited, it means we reached end of file or end of buffer before meeting \n
    l[pos]=NULL;
    return pos;
}

/*
 *  USB MASS STORAGE functions
 *
 */
//initializing mass storage object usb_msc
// assumes that flash and fatfs have already been initilaized
void msc_init(){



    // Set disk vendor id, product id and revision with string up to 8, 16, 4 characters respectively
    usb_msc.setID("Adafruit", "External Flash", "1.0");

    // Set callback
    usb_msc.setReadWriteCallback(msc_read_cb, msc_write_cb, msc_flush_cb);

    // Set disk size, block size should be 512 regardless of spi flash page size
    usb_msc.setCapacity(flash.size()/512, 512);

    // MSC is ready for read/write
    usb_msc.setUnitReady(true);

    usb_msc.begin();

}


// Callback invoked when received READ10 command.
// Copy disk's data to buffer (up to bufsize) and
// return number of copied bytes (must be multiple of block size)
int32_t msc_read_cb (uint32_t lba, void* buffer, uint32_t bufsize){
    // Note: SPIFLash Bock API: readBlocks/writeBlocks/syncBlocks
    // already include 4K sector caching internally. We don't need to cache it, yahhhh!!
    return flash.readBlocks(lba, (uint8_t*) buffer, bufsize/512) ? bufsize : -1;
}




// Callback invoked when received WRITE10 command.
// Process data in buffer to disk's storage and
// return number of written bytes (must be multiple of block size)
int32_t msc_write_cb (uint32_t lba, uint8_t* buffer, uint32_t bufsize){
    digitalWrite(LED_BUILTIN, HIGH);

    // Note: SPIFLash Bock API: readBlocks/writeBlocks/syncBlocks
    // already include 4K sector caching internally. We don't need to cache it, yahhhh!!
    return flash.writeBlocks(lba, buffer, bufsize/512) ? bufsize : -1;
}

// Callback invoked when WRITE10 command is completed (status received and accepted by host).
// used to flush any pending cache.
void msc_flush_cb (void){
    // sync with flash
    flash.syncBlocks();

    // clear file system's cache to force refresh
    fatfs.cacheClear();
    digitalWrite(LED_BUILTIN, LOW);
}
