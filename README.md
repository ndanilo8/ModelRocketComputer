# Model Rocket Computer
Just an Arduino that records rocket's telemetry and detects apogee for parachute release.

Coded on Visual Studio Code
 - MCU: Seeeduino XIAO,
 - IMU: MPU6050,
 - Barometric Sensor: BMP388


## Libraries used
- [Adafruit BMP3XX Library](https://github.com/adafruit/Adafruit_BMP3XX/)
- [BasicLinearAlgebra](https://github.com/tomstewart89/BasicLinearAlgebra/blob/master/examples/Tensor/Tensor.ino)
- [Chrono](http://github.com/SofaPirate/Chrono)
- [Madgwick](https://github.com/arduino-libraries/MadgwickAHRS)
- [MPU6050](https://github.com/pkourany/I2CDEV_MPU6050)
- [SdFat](https://github.com/greiman/SdFat)
- [SerialTransfer](https://github.com/PowerBroker2/SerialTransfer)

### Check config.h and global.h for settings and pin selection
