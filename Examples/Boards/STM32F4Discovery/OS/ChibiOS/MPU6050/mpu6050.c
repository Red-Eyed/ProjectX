#include "mpu6050.h"

msg_t mpu6050_Read(uint8_t addr, uint8_t* out_data)
{
  return i2cMasterTransmitTimeout(&I2CD1, MPU6050_ADDRESS_AD0_LOW, &addr, sizeof(addr), out_data, sizeof(*out_data), TIME_INFINITE);
}

msg_t mpu6050_Write(uint8_t addr, uint8_t data)
{
  uint8_t data_arr[2];
  data_arr[0] = addr;
  data_arr[1] = data;
  int ret = 0;
  ret = i2cMasterTransmitTimeout(&I2CD1, MPU6050_ADDRESS_AD0_LOW, data_arr, sizeof(data_arr), NULL, 0, TIME_INFINITE);
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

  //Disable sleep mode
  ret = mpu6050_Write(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CYCLE_BIT);
  if(ret != 0) return ret;

  return 0;
}
