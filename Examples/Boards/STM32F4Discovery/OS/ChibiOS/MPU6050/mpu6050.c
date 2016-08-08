#include "mpu6050.h"

static uint8_t g_addr[1], g_data[1];

msg_t mpu6050_Read(uint8_t addr, uint8_t* out_data)
{
  int ret = 0;
  g_addr[0] = addr;
  g_data[0] = 0;
  ret = i2cMasterTransmitTimeout(&I2CD1, MPU6050_ADDRESS_AD0_LOW, g_addr, sizeof(g_addr), g_data, sizeof(g_data), TIME_INFINITE);
  if(ret != MSG_OK)
    return ret;

  *out_data = g_data[0];

  return 0;
}

msg_t mpu6050_Write(uint8_t addr, uint8_t data)
{
  int ret = 0;
  g_addr[0] = addr;
  g_data[0] = data;
  ret = i2cMasterTransmitTimeout(&I2CD1, MPU6050_ADDRESS_AD0_LOW, g_addr, sizeof(g_addr), NULL, 0, TIME_INFINITE);
  if(ret != MSG_OK) {
    ret = i2cGetErrors(&I2CD1);
    return ret;
  }

  ret = i2cMasterTransmitTimeout(&I2CD1, MPU6050_ADDRESS_AD0_LOW, g_data, sizeof(g_data), NULL, 0, TIME_INFINITE);
  if(ret != MSG_OK) {
    ret = i2cGetErrors(&I2CD1);
    return ret;
  }

  return 0;
}

int mpu6050_init(void)
{
  int ret = 0;

  //Sets sample rate to 8000/1+7 = 1000Hz
  ret = mpu6050_Write(MPU6050_RA_SMPLRT_DIV, 0x07);
  if(ret != 0) return ret;

  //Disable FSync, 256Hz DLPF
  ret = mpu6050_Write(MPU6050_RA_CONFIG, 0x00);
  if(ret != 0) return ret;

  //Disable gyro self tests, scale of 500 degrees/s
  ret = mpu6050_Write(MPU6050_RA_GYRO_CONFIG, 0b00001000);
  if(ret != 0) return ret;

  //Disable accel self tests, scale of +-2g, no DHPF
  ret = mpu6050_Write(MPU6050_RA_ACCEL_CONFIG, 0x00);
  if(ret != 0) return ret;

  //Freefall threshold of |0mg|
  ret = mpu6050_Write(MPU6050_RA_FF_THR, 0x00);
  if(ret != 0) return ret;

  //Freefall duration limit of 0
  ret = mpu6050_Write(MPU6050_RA_FF_DUR, 0x00);
  if(ret != 0) return ret;

  //Motion threshold of 0mg
  ret = mpu6050_Write(MPU6050_RA_MOT_THR, 0x00);
  if(ret != 0) return ret;

  //Motion duration of 0s
  ret = mpu6050_Write(MPU6050_RA_MOT_DUR, 0x00);
  if(ret != 0) return ret;

  //Zero motion threshold
  ret = mpu6050_Write(MPU6050_RA_ZRMOT_THR, 0x00);
  if(ret != 0) return ret;

  //Zero motion duration threshold
  ret = mpu6050_Write(MPU6050_RA_ZRMOT_DUR, 0x00);
  if(ret != 0) return ret;

  //Disable sensor output to FIFO buffer
  ret = mpu6050_Write(MPU6050_RA_FIFO_EN, 0x00);
  if(ret != 0) return ret;

  return 0;
}
