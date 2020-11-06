#include "staff.h"

void POVstaff::begin(){
    pinMode(PIN_VSENSE, INPUT);
#ifdef PIN_CLOCK
    pinMode(PIN_CLOCK, OUTPUT);
    pinMode(PIN_DATA, OUTPUT);
#endif
    //Serial.println("Starting...");
    _pixels.begin();
    _pixels.clear(); // Make sure strip is clear
    _pixels.show();  // before measuring battery
    //init external flash
    flash.begin();
    // Open file system on the flash
    if ( !fatfs.begin(&flash) ) {
        Serial.begin(9600);
        Serial.println("Error: filesystem doesn't not exist. Please try SdFat_format example to make one.");
        while(1) yield();
    }
    //initialize the IMU, using I2C address 0x69
    if (!_mpu.begin(0x69)) {
        Serial.begin(9600);
        Serial.println("Failed to find MPU6050 chip");
    }

    //configure  IMU
    _mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
    _mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
    _mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    accel = _mpu.getAccelerometerSensor();
    gyro = _mpu.getGyroSensor();

    //Serial.println("POVstaff initilaized");
}

uint16_t POVstaff::batteryVoltage(){
    //because of voltage divider, VREF corresponds to read value of  1023/2=512
    //since battery is connected to VHI through a diode, we need to add back 100mV lost on the diode.
    return ((analogRead(PIN_VSENSE)*VREF)/512  + 100) ;
}

void POVstaff::showVoltage(){
    uint16_t level=0;
    uint8_t i, value;
    uint32_t c;
    _pixels.clear();
    //how many pixels should be lighted?
    if (batteryVoltage()<=VOLTAGE_MIN) {
        level=1;
    } else {
        level = ((uint32_t)NUM_PIXELS*(batteryVoltage()-VOLTAGE_MIN))/(VOLTAGE_MAX-VOLTAGE_MIN);
    }
    if (level>NUM_PIXELS) level=NUM_PIXELS;
    //Serial.println(level);
    //fill strip with gradient color
    for (i=0; i< level; i++){
        value= i*255/NUM_PIXELS; //gradually changes from 0 to 255
        c=((255-value)<<16)|(value<<8); //mixture of red and green
        _pixels.setPixelColor(i, c);
    }
    _pixels.show();
    //Serial.println("Showing level");
    delay(2000); //keep for 2 seconds
    _pixels.clear();
    _pixels.show();
    lastUpdate=micros();
    //Serial.println("Showing levle done");
}

void POVstaff::clear(){
    _pixels.clear();
    _pixels.show();
    lastUpdate=micros();
}

void POVstaff::show(){
    _pixels.show();
    lastUpdate=micros();
}

void POVstaff::setPixel(uint16_t i, uint32_t c){
    _pixels.setPixelColor(i,c);
}

void POVstaff::showLine(byte * line, uint16_t size){
    uint16_t i,pos;
    uint8_t r,g,b;
    for (i=0; i<NUM_PIXELS; i++) {
        if (i<size) {
            pos=3*i;
            //using bgr order
            b=line[pos++];
            g=line[pos++];
            r=line[pos];
            _pixels.setPixelColor(i,r,g,b);
        } else {
           _pixels.setPixelColor(i,OFF);
        }
    }
    _pixels.show();
    lastUpdate=micros();
}

void POVstaff::blink(){
    uint16_t i;
    uint8_t line[3*NUM_PIXELS];
    for (i=0; i<3*NUM_PIXELS;  i++) line[i]=0;
    _pixels.clear();
    for (i=0; 8*i<NUM_PIXELS; i++) {
        line[24*i+2]=255;//set pixel with index 8i red. Recall that the color order is BGR
    }
    showLine(line,  3*NUM_PIXELS);
    delay(500);
    _pixels.clear(); _pixels.show();
    delay(500);
    showLine(line,  3*NUM_PIXELS);
    delay(500);
    _pixels.clear();        _pixels.show();
}

void POVstaff::setImage(BMPimage * image){
    if (image->isLoaded()) {
        currentImage = image;
        currentLine=-1;
        lastUpdate = micros();
    } else {
        Serial.println("Image not loaded in memory yet");
    }
}

void POVstaff::showNextLine(){
    if (currentImage) {
        //move to next line
        currentLine++;
        if (currentLine == currentImage->height()) {currentLine=0;}
        showLine(currentImage->line(currentLine), currentImage->width());
    }
}

uint32_t POVstaff::timeSinceUpdate(){
    return (micros()-lastUpdate);
}

float POVstaff::rotationSpeed(){
    /*IMU.readGyroscope(x, y, z);
    return sqrtf(x*x+y*y+z*z);*/
    sensors_event_t g;
    gyro->getEvent(&g);
    return (RAD_TO_DEG*sqrtf(g.gyro.x*g.gyro.x +g.gyro.y*g.gyro.y+g.gyro.z*g.gyro.z));
}

bool POVstaff::atRest(){
    sensors_event_t a;
    accel->getEvent(&a);
    return ( (fabs(a.acceleration.x)<3.0) && (rotationSpeed()<30.0)); //x-component of accel< 3m/s^2 and rotation speed < 30 deg/s
}



float POVstaff::getCurrent(BMPimage image){
    float currentTotal=0.0;
    float currentLine=0.0;
    int height=image.height();
    int width=image.width();
    uint8_t b,g,r;//levels
    byte * pos;
    if (! image.isLoaded()){
        return 0.0;
    }
    for (int i=0; i<height; i++){
        //compute total current for line i
        currentLine=0.0;
        for (int j=0; j<width; j++){
            pos=image.line(i); //pointer to start of i-th line
            b=*pos; pos++;
            g=*pos; pos++;
            r=*pos; pos++;
            currentLine+=b*CURRENT_B+g*CURRENT_G+r*CURRENT_R;
        }
        //now, remember to divide by 255
        currentLine/=255.0;
        currentTotal+=currentLine;
    }
    return currentTotal;
}


#ifdef PIN_CLOCK
Adafruit_DotStar _pixels=Adafruit_DotStar(NUM_PIXELS, PIN_DATA, PIN_CLOCK,DOTSTAR_BGR);
#else
Adafruit_DotStar _pixels=Adafruit_DotStar(NUM_PIXELS, DOTSTAR_BGR);
#endif
POVstaff staff;
Adafruit_MPU6050 _mpu;
