/*
Based on Pololu's LSM6 library.

Copyright (c) 2016-2022 Pololu Corporation (www.pololu.com)

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

*/

#ifndef LSM6_h
#define LSM6_h

#include <Arduino.h>
#include <Wire.h>

class LSM6
{
  public:
    template <typename T> struct vector
    {
      T x, y, z;
    };

    enum deviceType { device_DS33, device_DSO, device_auto };
    enum sa0State { sa0_low, sa0_high, sa0_auto };

    // register addresses
    enum regAddr
    {
      FUNC_CFG_ACCESS          = 0x01,

      // This register conflicts with a register in the ESP32 with the same
      // name. We call it DSO_PIN_CTRL to work around this problem, but we'll
      // also define PIN_CTRL for use on other platforms.
      DSO_PIN_CTRL             = 0x02, // DSO
#ifndef PIN_CTRL
      PIN_CTRL                 = 0x02, // DSO
#endif

      DS33_FIFO_CTRL1          = 0x06, // DS33
      DS33_FIFO_CTRL2          = 0x07, // DS33
      DS33_FIFO_CTRL3          = 0x08, // DS33
      DS33_FIFO_CTRL4          = 0x09, // DS33
      DS33_FIFO_CTRL5          = 0x0A, // DS33

      DSO_FIFO_CTRL1           = 0x07, // DSO
      DSO_FIFO_CTRL2           = 0x08, // DSO
      DSO_FIFO_CTRL3           = 0x09, // DSO
      DSO_FIFO_CTRL4           = 0x0A, // DSO

      ORIENT_CFG_G             = 0x0B, // DS33
      COUNTER_BDR_REG1         = 0x0B, // DSO
      COUNTER_BDR_REG2         = 0x0C, // DSO
      INT1_CTRL                = 0x0D,
      INT2_CTRL                = 0x0E,
      WHO_AM_I                 = 0x0F,
      CTRL1_XL                 = 0x10,
      CTRL2_G                  = 0x11,
      CTRL3_C                  = 0x12,
      CTRL4_C                  = 0x13,
      CTRL5_C                  = 0x14,
      CTRL6_C                  = 0x15,
      CTRL7_G                  = 0x16,
      CTRL8_XL                 = 0x17,
      CTRL9_XL                 = 0x18,
      CTRL10_C                 = 0x19,
      ALL_INT_SRC              = 0x1A, // DSO
      WAKE_UP_SRC              = 0x1B,
      TAP_SRC                  = 0x1C,
      D6D_SRC                  = 0x1D,
      STATUS_REG               = 0x1E,
      STATUS_SPIAux            = 0x1E, // DSO

      OUT_TEMP_L               = 0x20,
      OUT_TEMP_H               = 0x21,
      OUTX_L_G                 = 0x22,
      OUTX_H_G                 = 0x23,
      OUTY_L_G                 = 0x24,
      OUTY_H_G                 = 0x25,
      OUTZ_L_G                 = 0x26,
      OUTZ_H_G                 = 0x27,
      OUTX_L_XL                = 0x28, // DS33
      OUTX_H_XL                = 0x29, // DS33
      OUTY_L_XL                = 0x2A, // DS33
      OUTY_H_XL                = 0x2B, // DS33
      OUTZ_L_XL                = 0x2C, // DS33
      OUTZ_H_XL                = 0x2D, // DS33
      OUTX_L_A                 = 0x28, // DSO
      OUTX_H_A                 = 0x29, // DSO
      OUTY_L_A                 = 0x2A, // DSO
      OUTY_H_A                 = 0x2B, // DSO
      OUTZ_L_A                 = 0x2C, // DSO
      OUTZ_H_A                 = 0x2D, // DSO

      EMB_FUNC_STATUS_MAINPAGE = 0x35, // DSO
      FSM_STATUS_A_MAINPAGE    = 0x36, // DSO
      FSM_STATUS_B_MAINPAGE    = 0x37, // DSO

      STATUS_MASTER_MAINPAGE   = 0x39, // DSO

      FIFO_STATUS1             = 0x3A,
      FIFO_STATUS2             = 0x3B,
      FIFO_STATUS3             = 0x3C, // DS33
      FIFO_STATUS4             = 0x3D, // DS33
      FIFO_DATA_OUT_L          = 0x3E, // DS33
      FIFO_DATA_OUT_H          = 0x3F, // DS33
      TIMESTAMP0_REG           = 0x40, // DS33
      TIMESTAMP1_REG           = 0x41, // DS33
      TIMESTAMP2_REG           = 0x42, // DS33
      TIMESTAMP0               = 0x40, // DSO
      TIMESTAMP1               = 0x41, // DSO
      TIMESTAMP2               = 0x42, // DSO
      TIMESTAMP3               = 0x43, // DSO

      STEP_TIMESTAMP_L         = 0x49, // DS33
      STEP_TIMESTAMP_H         = 0x4A, // DS33
      DS33_STEP_COUNTER_L      = 0x4B, // DS33 (DSO version in embedded functions regs)
      DS33_STEP_COUNTER_H      = 0x4C, // DS33

      FUNC_SRC                 = 0x53, // DS33

      TAP_CFG0                 = 0x56, // DSO
      TAP_CFG1                 = 0x57, // DSO
      TAP_CFG2                 = 0x58, // DSO
      TAP_CFG                  = 0x58, // DS33
      TAP_THS_6D               = 0x59,
      INT_DUR2                 = 0x5A,
      WAKE_UP_THS              = 0x5B,
      WAKE_UP_DUR              = 0x5C,
      FREE_FALL                = 0x5D,
      MD1_CFG                  = 0x5E,
      MD2_CFG                  = 0x5F,

      I3C_BUS_AVB              = 0x62, // DSO
      INTERNAL_FREQ_FINE       = 0x63, // DSO

      INT_OIS                  = 0x6F, // DSO
      CTRL1_OIS                = 0x70, // DSO
      CTRL2_OIS                = 0x71, // DSO
      CTRL3_OIS                = 0x72, // DSO
      X_OFS_USR                = 0x73, // DSO
      Y_OFS_USR                = 0x74, // DSO
      Z_OFS_USR                = 0x75, // DSO
      FIFO_DATA_OUT_TAG        = 0x78, // DSO
      FIFO_DATA_OUT_X_L        = 0x79, // DSO
      FIFO_DATA_OUT_X_H        = 0x7A, // DSO
      FIFO_DATA_OUT_Y_L        = 0x7B, // DSO
      FIFO_DATA_OUT_Y_H        = 0x7C, // DSO
      FIFO_DATA_OUT_Z_L        = 0x7D, // DSO
      FIFO_DATA_OUT_Z_H        = 0x7E, // DSO

      // LSM6DSO embedded functions registers

      PAGE_SEL               = 0x02,

      EMB_FUNC_EN_A          = 0x04,
      EMB_FUNC_EN_B          = 0x05,

      PAGE_ADDRESS           = 0x08,
      PAGE_VALUE             = 0x09,
      EMB_FUNC_INT1          = 0x0A,
      FSM_INT1_A             = 0x0B,
      FSM_INT1_B             = 0x0C,

      EMB_FUNC_INT2          = 0x0E,
      FSM_INT2_A             = 0x0F,
      FSM_INT2_B             = 0x10,

      EMB_FUNC_STATUS        = 0x12,
      FSM_STATUS_A           = 0x13,
      FSM_STATUS_B           = 0x14,

      PAGE_RW                = 0x17,

      EMB_FUNC_FIFO_CFG      = 0x44,

      FSM_ENABLE_A           = 0x46,
      FSM_ENABLE_B           = 0x47,
      FSM_LONG_COUNTER_L     = 0x48,
      FSM_LONG_COUNTER_H     = 0x49,
      FSM_LONG_COUNTER_CLEAR = 0x4A,

      FSM_OUTS1              = 0x4C,
      FSM_OUTS2              = 0x4D,
      FSM_OUTS3              = 0x4E,
      FSM_OUTS4              = 0x4F,
      FSM_OUTS5              = 0x50,
      FSM_OUTS6              = 0x51,
      FSM_OUTS7              = 0x52,
      FSM_OUTS8              = 0x53,
      FSM_OUTS9              = 0x54,
      FSM_OUTS10             = 0x55,
      FSM_OUTS11             = 0x56,
      FSM_OUTS12             = 0x57,
      FSM_OUTS13             = 0x58,
      FSM_OUTS14             = 0x59,
      FSM_OUTS15             = 0x5A,
      FSM_OUTS16             = 0x5B,

      EMB_FUNC_ODR_CFG_B     = 0x5F,

      DSO_STEP_COUNTER_L     = 0x62,
      DSO_STEP_COUNTER_H     = 0x63,
      EMB_FUNC_SRC           = 0x64,

      EMB_FUNC_INIT_A        = 0x66,
      EMB_FUNC_INIT_B        = 0x67,

      // LSM6DSO embedded advanced features registers, page 0

      MAG_SENSITIVITY_L = 0xBA,
      MAG_SENSITIVITY_H = 0xBB,

      MAG_OFFX_L        = 0xC0,
      MAG_OFFX_H        = 0xC1,
      MAG_OFFY_L        = 0xC2,
      MAG_OFFY_H        = 0xC3,
      MAG_OFFZ_L        = 0xC4,
      MAG_OFFZ_H        = 0xC5,
      MAG_SI_XX_L       = 0xC6,
      MAG_SI_XX_H       = 0xC7,
      MAG_SI_XY_L       = 0xC8,
      MAG_SI_XY_H       = 0xC9,
      MAG_SI_XZ_L       = 0xCA,
      MAG_SI_XZ_H       = 0xCB,
      MAG_SI_YY_L       = 0xCC,
      MAG_SI_YY_H       = 0xCD,
      MAG_SI_YZ_L       = 0xCE,
      MAG_SI_YZ_H       = 0xCF,
      MAG_SI_ZZ_L       = 0xD0,
      MAG_SI_ZZ_H       = 0xD1,

      MAG_CFG_A         = 0xD4,
      MAG_CFG_B         = 0xD5,

      // LSM6DSO embedded advanced features registers, page 1

      FSM_LC_TIMEOUT_L    = 0x7A,
      FSM_LC_TIMEOUT_H    = 0x7B,
      FSM_PROGRAMS        = 0x7C,

      FSM_START_ADD_L     = 0x7E,
      FSM_START_ADD_H     = 0x7F,

      PEDO_CMD_REG        = 0x83,
      PEDO_DEB_STEPS_CONF = 0x84,

      PEDO_SC_DELTAT_L    = 0xD0,
      PEDO_SC_DELTAT_H    = 0xD1,

      // LSM6DSO sensor hub registers

      SENSOR_HUB_1   = 0x02,
      SENSOR_HUB_2   = 0x03,
      SENSOR_HUB_3   = 0x04,
      SENSOR_HUB_4   = 0x05,
      SENSOR_HUB_5   = 0x06,
      SENSOR_HUB_6   = 0x07,
      SENSOR_HUB_7   = 0x08,
      SENSOR_HUB_8   = 0x09,
      SENSOR_HUB_9   = 0x0A,
      SENSOR_HUB_10  = 0x0B,
      SENSOR_HUB_11  = 0x0C,
      SENSOR_HUB_12  = 0x0D,
      SENSOR_HUB_13  = 0x0E,
      SENSOR_HUB_14  = 0x0F,
      SENSOR_HUB_15  = 0x10,
      SENSOR_HUB_16  = 0x11,
      SENSOR_HUB_17  = 0x12,
      SENSOR_HUB_18  = 0x13,
      MASTER_CONFIG  = 0x14,
      SLV0_ADD       = 0x15,
      SLV0_SUBADD    = 0x16,
      SLV0_CONFIG    = 0x17,
      SLV1_ADD       = 0x18,
      SLV1_SUBADD    = 0x19,
      SLV1_CONFIG    = 0x1A,
      SLV2_ADD       = 0x1B,
      SLV2_SUBADD    = 0x1C,
      SLV2_CONFIG    = 0x1D,
      SLV3_ADD       = 0x1E,
      SLV3_SUBADD    = 0x1F,
      SLV3_CONFIG    = 0x20,
      DATAWRITE_SLV0 = 0x21,
      STATUS_MASTER  = 0x22,
    };

    vector<int16_t> a; // accelerometer readings
    vector<int16_t> g; // gyro readings

    uint8_t last_status; // status of last I2C transmission

    LSM6();

    void setBus(TwoWire * bus) { this->bus = bus; }
    TwoWire * getBus() { return bus; }

    bool init(deviceType device = device_auto, sa0State sa0 = sa0_auto);
    deviceType getDeviceType() { return _device; }

    void enableDefault();

    void writeReg(uint8_t reg, uint8_t value);
    uint8_t readReg(uint8_t reg);

    void readAcc();
    void readGyro();
    void read();

    float getSpeed();
    bool isVertical();
    bool isHorizontal();



    // vector functions
    template <typename Ta, typename Tb, typename To> static void vector_cross(const vector<Ta> *a, const vector<Tb> *b, vector<To> *out);
    template <typename Ta, typename Tb> static float vector_dot(const vector<Ta> *a, const vector<Tb> *b);
    static void vector_normalize(vector<float> *a);

  private:
    TwoWire * bus;
    deviceType _device; // chip type
    uint8_t address;

    int16_t testReg(uint8_t address, regAddr reg);
};


template <typename Ta, typename Tb, typename To> void LSM6::vector_cross(const vector<Ta> *a, const vector<Tb> *b, vector<To> *out)
{
  out->x = (a->y * b->z) - (a->z * b->y);
  out->y = (a->z * b->x) - (a->x * b->z);
  out->z = (a->x * b->y) - (a->y * b->x);
}

template <typename Ta, typename Tb> float LSM6::vector_dot(const vector<Ta> *a, const vector<Tb> *b)
{
  return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

#endif
