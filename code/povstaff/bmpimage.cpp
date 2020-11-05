#include "bmpimage.h"

byte _buffer[BUF_SIZE];

bool BMPimage::setFilename(char * fn) {
    File bmpFile;
    int bmpDepth;
    //first, check if filename exists
    if ((bmpFile = fatfs.open(fn)) == NULL) {
        Serial.print(F("BMPimage setFilename: failed to open file ")); Serial.println(fn);
        _valid=false;
        return false;
    }


    //so, the file exists and is opened
    // Parse BMP header
    if(read16(bmpFile) != 0x4D42) { // BMP signature
        Serial.println(F("BMPimage load: file doesn't look like a BMP"));
        _valid=false;
        bmpFile.close();
        return false;
    }
    //read and ingnore file size
    read32(bmpFile);
    (void)read32(bmpFile); // Read & ignore creator bytes
    _imageOffset = read32(bmpFile); // Start of image data
    //Serial.print(F("Image Offset: ")); Serial.println(_imageOffset, DEC);
    // Read DIB header
    read32(bmpFile);
    _width  = read32(bmpFile);
    _height = read32(bmpFile);
    if(read16(bmpFile) != 1) { // # planes -- must be '1'
        Serial.println(F("BMPimage load: invalid n. of planes"));
        _valid=false;
        bmpFile.close();
        return false;
    }
    bmpDepth = read16(bmpFile); // bits per pixel
    //Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
    if((bmpDepth != 24) || (read32(bmpFile) != 0)) { // 0 = uncompressed {
        Serial.println(F("BMPimage load: invalid pixel format"));
        _width=0;
        _valid=false;
        bmpFile.close();
        return false;
    }
    /*
    Serial.print(F("Image size: "));
    Serial.print(_width);
    Serial.print('x');
    Serial.println(_height);
    */

    // If _height is negative, image is in top-down order.
    // This is not canon but has been observed in the wild.
    if(_height < 0) {
        _height = -_height;
    }
    //now, we have successfully got all the basics
    // BMP rows are padded (if needed) to 4-byte boundary
    _rowSize = (_width * 3 + 3) & ~3;
    //check image size - if it is too large, it will be unusable
    if (_rowSize*_height>BUF_SIZE) {
            Serial.println(F("BMPimage load: file too lrage"));
        _valid=false;
        bmpFile.close();
        return false;
    }

    //Serial.print("Row size: ");Serial.println(_rowSize);
    bmpFile.close();
    strcpy(filename,fn);
    return true;
}


void BMPimage::getFilename(char * fn){
    strcpy(fn, filename);
}

bool BMPimage::load(){
    //now is the time to read actual data into buffer.
    // first, let us make sure it is large enough, to avoid buffer overflow
    uint16_t size= _rowSize * _height;
    //Serial.print("Required buffer size (bytes)"); Serial.println(size);
    if (BUF_SIZE < size ){
        Serial.print(F("BMPimage load: file too largefor buffer of size "));Serial.println(BUF_SIZE);
        return false;
    }
    //if everything is OK, let us just read the file into teh buffer
    /*Serial.print("opening file "); Serial.println(filename);
      delay(3000);
    */

    File bmpFile;
    if ((bmpFile = fatfs.open(filename)) == NULL) {
        Serial.print(F("BMPimage load: failed to open file ")); Serial.println(filename);
        return false;
    }
    /*Serial.println("File opened");
    delay(3000);
    */
    bmpFile.seek(_imageOffset);
    /*Serial.print("Setting offset "); Serial.println(_imageOffset);
    delay(3000);
    */
    bmpFile.read(_buffer, size);
    /*Serial.println("Buffer read");
    delay(3000); */

    //close file
    bmpFile.close();
    // now we can mark the image as loaded
    _loaded = true;
    return true;
}


byte* BMPimage::line(uint16_t n){
    if (_loaded) {
        return (_buffer+n*_rowSize);
    } else {
        return NULL;
    }
}

uint32_t BMPimage::pixelColor(uint16_t x, uint16_t  y){
    uint32_t pos;
    byte b,g,r; //colors
    if (! _loaded) {
        Serial.println("BMPimage pixelColor: image hasn't been loaded yet");
        return 0;
    }
    if ( (x>=_width) || (y>=_height) ) {
        Serial.println("BMPimage pixelColor: invalid coordinates");
        return 0;
    }
    pos=y*_rowSize + 3*x;
    //get colors. Note that in BMP files, they go in BGR order
    b= _buffer[pos++];
    g= _buffer[pos++];
    r= _buffer[pos];
    return (r<<16|g<<8|b);
}

/* *****************************************************
 *  BMP image class
 *******************************************************/


BMPimage * BMPimageList::current(){
    if (_numImages==0) { return NULL; }
    return (&images[_currentIndex]);
}
uint16_t BMPimageList::currentDuration(){
    if (_numImages==0) { return 0; }
    return (durations[_currentIndex]);
}
BMPimage * BMPimageList::next(){
    if (_numImages==0) { return NULL; }
    _currentIndex++;
    if (_currentIndex==_numImages) {_currentIndex=0;}
    return (&images[_currentIndex]);
}
BMPimage * BMPimageList::first(){
    if (_numImages==0) { return NULL; }
    _currentIndex=0;
    return (&images[_currentIndex]);
}

BMPimage * BMPimageList::addImage(char * filename, uint16_t duration){
    if (_numImages==MAX_FILES) {return NULL;}
    //otherwise, let's take the next image and init it
    if (images[_numImages].setFilename(filename)) {
        //successfully added image
        durations[_numImages] = duration;
        _currentIndex=_numImages;
        _numImages++;
        return (&images[_currentIndex]);
    } else {
        //failed to add image
        return NULL;
    }
}

int BMPimageList::addFromFile(char * fn){
    File f;
    //for storing filename
    char filenames[MAX_FILES][MAX_FILENAME];
    uint16_t dur[MAX_FILES];
    int numRead=0;
    int numAdded=0;
    char line[MAX_LINE_LENGTH+1];
    char * fname;
    char * duration_ptr;
    int16_t duration;
    int i;
    if ((f = fatfs.open(fn)) == NULL) {
        //failed to open file
        Serial.print("BMPimageList::addFromFile: failed to open file "); Serial.println(fn);
        return 0;
    }
    //file fn successufully opened
    //let's read its contents in array filenames
    while ( (numRead<MAX_FILES) && (readLine(f, line)) ) {
        //Serial.print("Read line "); Serial.println(line);
        //line read; now let us split it into tokens
        fname = strtok(line," \r\t");
        duration_ptr = strtok(NULL, " \r\t");
        if (fname!=NULL) {
            strcpy(filenames[numRead], fname);
            if (duration_ptr == NULL) {
                duration=0;
            } else {
                duration = atoi(duration_ptr);
            }
            //Serial.println(duration);
            dur[numRead]=duration;
            numRead++;
        }
    }
    f.close();
    //now that we got array of filenames, let us create bmpfile for each of them
    for (i=0; i<numRead; i++){
        if (addImage(filenames[i], dur[i]) != NULL) {
            numAdded++;
        };
    }
    return numAdded;
}

void BMPimageList::print(){
    Serial.print("Image list: total # of files: ");
    Serial.println(_numImages);
    for (int i=0; i<_numImages; i++){
        Serial.print(images[i].filename); Serial.print(": ");
        Serial.print(images[i]._width); Serial.print("x"); Serial.print(images[i]._height);
        Serial.print(";  duration: "); Serial.println(durations[i]);
    }
}
