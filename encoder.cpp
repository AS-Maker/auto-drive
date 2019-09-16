#include "encoder.h"



Encoder::Encoder(int nSensorID, int nPinEncoderA, int nPinEncoderB) : AbstractSensor(nSensorID)
{
    EncodeEnable=1;
    m_nPinEncoderA = nPinEncoderA;
    m_nPinEncoderB = nPinEncoderB;
    SetPin(m_nPinEncoderA);
    SetPin(m_nPinEncoderB);
    m_nCounterTics = 0;
    m_nWheelDegree = 0;


    connect(m_pTimerUpdate, SIGNAL(timeout()), this, SLOT(slot_ShowValue()));


}

Encoder::~Encoder()
{

    emit sgn_Finished();
    EncodeEnable=0;
    std::cout << "Kill Encoder" << std::endl;
}

void Encoder::slot_StartProcess()
{
    m_Mutex.lock();
    m_nCounterTics = 0;
    m_Mutex.unlock();
    m_pTimerUpdate->start();
}

void Encoder::slot_Process()
{
    m_nStatusHallSensorA_old = digitalRead(m_nPinEncoderA);
    m_nStatusHallSensorB_old = digitalRead(m_nPinEncoderB);

    while(EncodeEnable==1)
    {
        QThread::usleep(1);

        m_nStatusHallSensorA = digitalRead(m_nPinEncoderA);
        m_nStatusHallSensorB = digitalRead(m_nPinEncoderB);

        if(m_nStatusHallSensorA==m_nStatusHallSensorB_old){
            m_Mutex.lock();
            m_nCounterTics = m_nCounterTics - 1;
            m_Mutex.unlock();
        }
        if(m_nStatusHallSensorB==m_nStatusHallSensorA_old){
            m_Mutex.lock();
            m_nCounterTics = m_nCounterTics + 1;
            m_Mutex.unlock();
        }
m_Mutex.lock();
        m_nWheelDegree = double(m_nCounterTics*nCompleteScopeDegree/nCompleteScopeTics);
m_Mutex.unlock();
        m_nStatusHallSensorA_old = m_nStatusHallSensorA;
        m_nStatusHallSensorB_old = m_nStatusHallSensorB;

        QCoreApplication::processEvents();
    }
}

int Encoder::nGetCounterTics()
{
    m_Mutex.lock();
    int temp =  m_nCounterTics;
    m_Mutex.unlock();
    return temp;
}

int Encoder::nResetCounterTics()
{
    m_Mutex.lock();
    m_nCounterTics = 0;
    int temp =  m_nCounterTics;
    m_Mutex.unlock();
    return temp;
}

void Encoder::slot_ShowValue()
{
    emit sgn_NewSensorData(m_nSensorID, m_nWheelDegree);
}


void Encoder::slot_StopProcess()
{
    m_pTimerUpdate->stop();
}

