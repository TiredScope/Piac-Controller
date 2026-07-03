#include "MPU6050.h"
#include "Arduino.h"
#include "MPU6050Module.hpp"

#include <Wire.h>

void MPU6050Module::begin() {
  // Nothing to do
  Wire.begin();

  mpu6050.initialize(ACCEL_FS::A8G, GYRO_FS::G2000DPS);

  if (!mpu6050.testConnection()) {
    MiniCom::debugPrint("Failed to connect to MPU6050");
  }
}

void MPU6050Module::update() {
  unsigned long now = millis();
  if (now - lastSent < reportingDelay) return;
  lastSent = now;

  MessageBuilder::reset(MessageType::M_MPU6050_VALUES, getDiscriminator());

  int16_t ax, ay, az, gx, gy, gz;
  mpu6050.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  MessageBuilder::putI16(ax);
  MessageBuilder::putI16(ay);
  MessageBuilder::putI16(az);

  MessageBuilder::putI16(gx);
  MessageBuilder::putI16(gy);
  MessageBuilder::putI16(gz);

  MiniCom::send(MessageBuilder::build());
}

void MPU6050Module::onMessage(Message m) {
  size_t idx = 0;

  switch (m.getType()) {
    case MessageType::M_MPU6050_SET_REPORTING_DELAY:
      {
        uint32_t delay = m.getU32(idx);
        reportingDelay = delay;
        break;
      }
    case MessageType::M_MPU6050_SET_CALIBRATION_VALUES:
      {
        int16_t xAccelOffset = m.getI16(idx);
        int16_t yAccelOffset = m.getI16(idx);
        int16_t zAccelOffset = m.getI16(idx);

        int16_t xGyroOffset = m.getI16(idx);
        int16_t yGyroOffset = m.getI16(idx);
        int16_t zGyroOffset = m.getI16(idx);

        mpu6050.setXAccelOffset(xAccelOffset);
        mpu6050.setYAccelOffset(yAccelOffset);
        mpu6050.setZAccelOffset(zAccelOffset);

        mpu6050.setXGyroOffset(xGyroOffset);
        mpu6050.setYGyroOffset(yGyroOffset);
        mpu6050.setZGyroOffset(zGyroOffset);
        break;
      }
    case MessageType::M_MPU6050_START_CALIBRATION:
      {
        uint8_t loops = m.getU8(idx);
        MiniCom::debugPrintf("Starting calibration with %d loops", loops);
        mpu6050.CalibrateAccel(loops);
        mpu6050.CalibrateGyro(loops);

        int16_t xAccelOffset = mpu6050.getXAccelOffset();
        int16_t yAccelOffset = mpu6050.getYAccelOffset();
        int16_t zAccelOffset = mpu6050.getZAccelOffset();

        int16_t xGyroOffset = mpu6050.getXGyroOffset();
        int16_t yGyroOffset = mpu6050.getYGyroOffset();
        int16_t zGyroOffset = mpu6050.getZGyroOffset();

        Serial.print('\n'); // Because MPU6050 vomits some stuff via serial

        MessageBuilder::reset(MessageType::M_MPU6050_SET_CALIBRATION_VALUES, getDiscriminator());

        MessageBuilder::putI16(xAccelOffset);
        MessageBuilder::putI16(yAccelOffset);
        MessageBuilder::putI16(zAccelOffset);

        MessageBuilder::putI16(xGyroOffset);
        MessageBuilder::putI16(yGyroOffset);
        MessageBuilder::putI16(zGyroOffset);

        MiniCom::send(MessageBuilder::build());

        break;
      }
    case MessageType::M_MPU6050_SET_RANGES:
      {
        uint8_t accelRange = m.getU8(idx);
        uint8_t gyroRange = m.getU8(idx);

        mpu6050.setFullScaleAccelRange((uint8_t)accelRange);
        mpu6050.setFullScaleGyroRange((uint8_t)gyroRange);
        break;
      }
  }
}