#include "ultrasonicsensor.h"

#define nSpeedofSoundMicroSecpCM 58
#define nOffSetCM 13
#define nTimerSignalGet 1000
#define nTimerSignalProcess 1000

UltrasonicSensor::UltrasonicSensor(int nSensorID, int nPinSensor) : AbstractSensor(nSensorID)
{
    m_nPinUltrasonicSensor = nPinSensor;
    m_fdistanceCM = 0;
    m_bUltrasonicSensorRunning = true;
     m_pTimerUpdate->setInterval(300);

}

UltrasonicSensor::~UltrasonicSensor()
{
    emit sgn_Finished();
    std::cout << "Kill UltrasonicSensor" << std::endl;
}

bool UltrasonicSensor::bSetPin()
{
    if(0 <= m_nPinUltrasonicSensor && m_nPinUltrasonicSensor <= 29)
    {
        return true;
    }
    else
    {
        return false;
    }
}



void UltrasonicSensor::slot_Process()
{
    QElapsedTimer TimerToGetSignal;
    QElapsedTimer TimerToHandleSignal;

    pinMode(m_nPinUltrasonicSensor, OUTPUT);
    digitalWrite(m_nPinUltrasonicSensor, LOW);
    delayMicroseconds(2);
    digitalWrite(m_nPinUltrasonicSensor, HIGH);
    delayMicroseconds(10);
    digitalWrite(m_nPinUltrasonicSensor, LOW);

    TimerToGetSignal.start();
    pinMode(m_nPinUltrasonicSensor, INPUT);


    while(!digitalRead(m_nPinUltrasonicSensor))
    {
        if(TimerToGetSignal.elapsed() > nTimerSignalGet)
        {
            m_bUltrasonicSensorRunning = false;
            emit sgn_Finished();
            break;
        }
        QCoreApplication::processEvents();
    }

    TimerToHandleSignal.start();

    if(m_bUltrasonicSensorRunning)
    {

        startTime = micros();
        while(digitalRead(m_nPinUltrasonicSensor))
        {
            if(TimerToHandleSignal.elapsed() > nTimerSignalProcess)
            {
                m_bUltrasonicSensorRunning = false;
                emit sgn_Finished();
                break;
            }
            QCoreApplication::processEvents();
        }

        if(m_bUltrasonicSensorRunning)
        {
            endTime = micros();
            m_Mutex.lock();
            m_fdistanceCM = float(nOffSetCM + float((endTime - startTime)/nSpeedofSoundMicroSecpCM));
            m_Mutex.unlock();
            emit sgn_NewSensorData(m_nSensorID, m_fdistanceCM);
        }
    }
}

void UltrasonicSensor::slot_StopProcess()
{
    m_pTimerUpdate->stop();
}

