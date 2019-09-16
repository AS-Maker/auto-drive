//!
//! \file encoder.h
//! \author Team-3
//! \brief Von AbstractSensor abgeleitete Klasse für den Encoder
//!
//! Klasse für Encoder
//!

#ifndef ENCODER_H
#define ENCODER_H
#include <abstractsensor.h>
#include <stdlib.h>




class Encoder : public AbstractSensor
{

public:

    //! \brief Konstruktor
    //!
    //! Speichert die übergebenen Werte und setzt die Pins als Input.
    //!
    //! \int [in]    pinA  Pin für ersten Hallsensor
    //! \int [in]    pinB  Pin für zweiten Hallsensor
    //! \int [in]    ID    ID des Encoders, mit der neue Encoderwerte gesendet werden

    Encoder(int nSensorID, int nPinEncoderA, int nPinEncoderB);

    //!
    //! Destruktor des Encoders
    //!
    ~Encoder();

    const int nCompleteScopeDegree=360;
    const int nCompleteScopeTics=1200;

    int nGetCounterTics();

    int nResetCounterTics();

public slots:

    void slot_StartProcess();

    void slot_Process();

    void slot_StopProcess();

    void slot_ShowValue();



private:
    bool EncodeEnable;

    int m_nPinEncoderA;

    int m_nPinEncoderB;

    bool bEncoderRunning;

    long int m_nCounterTics;

    int m_nWheelDegree;

    int m_nStatusHallSensorA;

    int m_nStatusHallSensorB;

    int m_nStatusHallSensorA_old;


    int m_nStatusHallSensorB_old;

};

#endif // ENCODER_H
