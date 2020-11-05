#ifndef _BMPIMAGE_H
#define _BMPIMAGE_H
#include <Arduino.h>
#include "flashstorage.h" //all definitions related to flash storage incldued on the board.
                          //in particular, defines filesystem fatfs, where all the files are located

#include "config.h"

/*
 * This class describes a bitmap image. Each object refers to a bmp file on
 * filesystem fatfs.
 * To initialize, call setFilename(), passign to it name of a bitmap file
 * at the root of fatfs filesystem:
 *
 * BMPimage myImage;
 * myImage.setFilename("logo.bmp");
 *
 * For performance reasons, before actually usign the image, you need to load
 * it from filesystem to RAM:
 * myImage.load();
 * All load() operations use the same reserved buffer in RAM, so you can only
 * have one file loaded at a time. Before loading a new file, always unload the
 * previous one:
 * myImage.unload();
 */

class BMPimage {
    public:
        int height()    {return _height; }
        int width()     {return _width;  }
        int rowSize()   {return _rowSize;}
        bool isLoaded() {return _loaded; }
        bool load();
        void unload()   {_loaded=false;  }
        bool loadPattern();
        byte * line(uint16_t n);
        uint32_t pixelColor(uint16_t x,uint16_t  y);
        bool setFilename(char* fn);
        void getFilename(char* fn);
    private:
        char filename[MAX_FILENAME]="";
        int _width=0;
        int _height=0;
        int _rowSize=0;
        int _imageOffset=0;
        bool _loaded=false;
        bool _valid=false;
        friend class BMPimageList;
};

class BMPimageList {
    public:
        /* returns number of images in the list */
        uint16_t count()     {return _numImages;}
        /* pointer to current image in list. If list is empty, returns NULL */
        BMPimage * current();
        /* duration for current image, in seconds */
         uint16_t currentDuration();
        /* moves pointer to next image in the list and returns the pointer.
           If list is empty, returns NULL
           If we are at last image, goes back to  the first one
        */
        BMPimage * next();
        /* sets pointer to the first image in list and returns it.
           If list is empty, returns NULL */
        BMPimage * first();
        /* adds another image to the end of the list and ititialzes it; sets pointer to it
         * and returns the pointer.
         * If addition failed for any reason (exceed max length of list, wrong filename,
         * wrong file format), returns NULL
         */
        BMPimage *  addImage(char * filename, uint16_t duration=0);
        /*
         * Reads filenames from a text file fn and adds them to the list.
         * The file must have one filename per line
         * Maximal length of a filename is MAX_FILENAME (defined  in config.h)
         * returns number of added files
         */
        int addFromFile(char * fn);
        /*
         * For debugging purposes, prints info
         */
         void print();

    private:
        BMPimage images[MAX_FILES];
        uint16_t durations[MAX_FILES];
        uint16_t _currentIndex=0;
        uint16_t _numImages=0;
};



extern byte _buffer[];

#endif
