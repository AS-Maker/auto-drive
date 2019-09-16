//!
//! \file linesensor.h
//! \author Team-3
//! \brief Klasse zum Auslesen eines Liniensensors
//!


#ifndef LINESENSOR_H
#define LINESENSOR_H
#include <abstractsensor.h>

class LineSensor : public AbstractSensor
{

public:
    //! \brief Konstruktor
    //!
    //! \int [in]   nPinSensor    WiringPi Pin des angeschlossenen Sensor
    //! \int [in]    nSensorID    ID des Sensors, mit der neue Sensorwerte gesendet werden
    LineSensor(int nSensorID, int nPinSensor);

    ~LineSensor();

    int nGetLastStatus();

public slots:



    void slot_Process();

    void slot_StopProcess();

private:

    int m_nPinSensor;

    int m_nLastStatus;



};

#endif // LINESENSOR_H
