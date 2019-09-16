#include "colorsensor.h"



ColorSensor::ColorSensor(int nSensorID) : AbstractSensor(nSensorID)
{
    m_iDeviceHandleId = wiringPiI2CSetup(0x29);
    wiringPiI2CWriteReg8(m_iDeviceHandleId, 0x80|0x00, 0x01|0x02);
    wiringPiI2CWriteReg8(m_iDeviceHandleId, 0x80|0x01, 0xD6);  // Intervalltime =100ms
    wiringPiI2CWriteReg8(m_iDeviceHandleId, 0x80|0x03, 0xff);

    red = 0;
    green = 0;
    blue = 0;
    brightness = 0;
    m_nColor = 0;

    m_pTimerUpdate->setInterval(100);


}

ColorSensor::~ColorSensor()
{

    emit sgn_Finished();
    std::cout << "Kill ColorSensor" << std::endl;
}



void ColorSensor::slot_Process()
{
    red = wiringPiI2CReadReg16(m_iDeviceHandleId, 0x80|0x16);
    green = wiringPiI2CReadReg16(m_iDeviceHandleId, 0x80|0x18);
    blue = wiringPiI2CReadReg16(m_iDeviceHandleId, 0x80|0x1A);
    brightness = wiringPiI2CReadReg16(m_iDeviceHandleId, 0x80|0x14);



    if(red/green > 2 && red/blue > 2)
    {
        m_Mutex.lock();
        m_nColor = nColorRed;
        m_Mutex.unlock();
    }
    else if(green/red > 1.5 && green/blue > 1.3 && brightness < 25000)
    {
        m_Mutex.lock();
        m_nColor = nColorGreen;
        m_Mutex.unlock();
    }
    else if(blue-red > 50 && blue-green > 20 && brightness > 2000) //2500   blue-red > 50 && blue-green > 20 , blue/red > 1.1 && blue/green > 1
    {
        m_Mutex.lock();
        m_nColor = nColorBlue;
        m_Mutex.unlock();
    }
    else if(green/red > 0.8 && red/blue > 1.8)
    {
        m_Mutex.lock();
        m_nColor = nColorYellow;
        m_Mutex.unlock();
    }
    else if(brightness > 5000)
    {
        m_Mutex.lock();
        m_nColor = nColorWhite;
        m_Mutex.unlock();
    }
    else if(brightness < 2000)
    {
        m_Mutex.lock();
        m_nColor = nColorBlack;
        m_Mutex.unlock();
    }
    else
    {
        m_Mutex.lock();
        m_nColor = nColorUnknown;
        m_Mutex.unlock();
    }
    emit sgn_NewSensorData(m_nSensorID, m_nColor);
}

void ColorSensor::slot_StopProcess()
{
    m_Mutex.lock();
    m_nColor = nColorUnknown;
    m_Mutex.unlock();
    emit sgn_NewSensorData(m_nSensorID, m_nColor);

    m_pTimerUpdate->stop();
}
