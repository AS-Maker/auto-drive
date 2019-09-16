#include "abstractsensor.h"

AbstractSensor::AbstractSensor(int nSensorID)
{

    m_nSensorID = nSensorID;

    //Timereinstellungen und Connect
    m_pTimerUpdate = new QTimer();
    m_pTimerUpdate->setInterval(50);
    m_pTimerUpdate->setSingleShot(false);
    connect(m_pTimerUpdate, SIGNAL(timeout()), this, SLOT(slot_Process()));
    m_pTimerUpdate->start();
}

AbstractSensor::~AbstractSensor()
{
    m_pTimerUpdate->stop();
    delete m_pTimerUpdate;
    std::cout << "Kill AbstractSensor" << std::endl;
}

void AbstractSensor::SetPin(int nPinSensor)
{
    if(0 <= nPinSensor && nPinSensor <= 29)
    {
        pinMode(nPinSensor,INPUT);
    }
    else
    {
        std::cout << "wrong pin number" << std::endl;
    }
}


void AbstractSensor::slot_StartProcess()
{
    m_pTimerUpdate->start();
}


void AbstractSensor::slot_StopProcess()
{

}

void AbstractSensor::slot_ShowValue()
{

}

