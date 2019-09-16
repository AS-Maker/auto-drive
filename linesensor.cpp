#include "linesensor.h"

LineSensor::LineSensor(int nSensorID, int nPinSensor) : AbstractSensor(nSensorID)
{
    m_nPinSensor = nPinSensor;
    SetPin(m_nPinSensor);
    m_nLastStatus = 0;
    m_pTimerUpdate->setInterval(10);

}

LineSensor::~LineSensor()
{
    emit sgn_Finished();
    std::cout << "Kill LineSensor" << std::endl;
}



void LineSensor::slot_Process()
{
    m_Mutex.lock();
     m_nSensordaten = digitalRead(m_nPinSensor);
     m_Mutex.unlock();
     emit sgn_NewSensorData(m_nSensorID, m_nSensordaten);
}

void LineSensor::slot_StopProcess()
{
    m_pTimerUpdate->stop();
}

int LineSensor::nGetLastStatus()
{
    m_Mutex.lock();
    m_nLastStatus = digitalRead(m_nPinSensor);
    m_Mutex.unlock();
    return m_nLastStatus;
}
