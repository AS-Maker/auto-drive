//!
//! \file drivecontrol.h
//! \author Team-3
//! \date 7-Januar -2018

//! \brief Klasse zur Geschwindigkeitsregelung mit PID Regler. Fahren ist mit Geradelinie, und mit Winkel moeglich.

//!

#ifndef DRIVECONTROL_H
#define DRIVECONTROL_H

#include <QObject>
#include <iostream>
#include <QTimer>
#include <QMutex>
#include <QThread>
#include "dcmotor.h"
#include "encoder.h"


#define Pi 3.1415926
#define dTimeIntervallMotion 0.1
class drivecontrol: public QObject

{
    Q_OBJECT

public:
    //!
    //! \brief drivecontrol: Konstruktor initialisiert alle Variablen und Timer
    //! \param dcMotorLeft: Ist Pointer zum Objekt fuer den linken Motor
    //! \param dcMotorRight: Ist Pointer zum Objekt fuer den rechten Motor
    //! \param EncoderLeft: Ist Pointer zum Objekt fuer den linken Encoder
    //! \param EncoderRight: Ist Pointer zum Objekt fuer den rechten Encoder
    //!
    drivecontrol(DcMotor &dcMotorLeft, DcMotor &dcMotorRight, Encoder &EncoderLeft, Encoder &EncoderRight);

   // bool isReady();



    //!
    //! Destruktor: loescht Timer und setzt boolsche odometry Variable auf fasle
    //!
    ~drivecontrol();
    //!
    //! \brief m_pDcEncoderLeft
    //!
    Encoder *m_pDcEncoderLeft;
    Encoder *m_pDcEncoderRight;
    DcMotor *m_pDcMotorLeft;
    DcMotor *m_pDcMotorRight;
    QTimer *m_pTimerUpdateSpeed;

    bool m_bOdometryIsRunning;

    void DriveLine(double dDistance);
    void DriveAngle(int nAngle);
private:

    //static const double dTimeIntervallMotion=0.1;
    //!
    //! \brief PID-Rgler Kp, Ki, Kd fuer den linken und den rechten Motor
    //!

    static const int m_dKp_MotorLeft_Motion=105;
    static const int m_dKp_MotorRight_Motion=100;
    static const int m_dKi_MotorLeft_Motion=155;
    static const int m_dKi_MotorRight_Motion=150;
    static const int m_dKd_MotorLeft_Motion=10;
    static const int m_dKd_MotorRight_Motion=10;

    //!
    //! \brief Parameter des Wagens
    //!
    const double dDistancePerWheelRotation=0.2; //Pi*Reifensdurchmesser(63.67mm)
    const int nCompleteScopeTics=1200;
    const int nCompleteScopeGradiant=360;
    const double dCentreDistance=0.1105;

    //!
    //! \brief Parameter fuer den Encoder
    //!

    int m_nCounterTicsLeft;
    int m_nCounterTicsLeft_old;
    int m_nCounterTicsLeft_Dif;
    int m_nCounterTicsRight;
    int m_nCounterTicsRight_old;
    int m_nCounterTicsRight_Dif;

    //!
    //! \brief Parameter fuer die Geschwindigkeitsregelung fuer die Motoren
    //!

    double m_dSpeedMotorLeft;
    double m_dSpeedMotorLeft_soll;
    double m_dSpeedMotorRight;
    double m_dSpeedMotorRight_soll;
    static const int m_nPWM_max = 100;
    static const int m_nPWM_min = 0;
    int m_nPWMMotorLeft;
    int m_nPWMMotorRight;

    double m_dDistanceLeft;
    double m_dDistanceRight;
    double Difference_Speed_Left;
    double Difference_Speed_Left_alt;
    double Sum_Difference_Speed_Left;
    double Difference_Speed_Right;
    double Difference_Speed_Right_alt;
    double Sum_Difference_Speed_Right;

    double m_dDistanceLeft_soll;
    double m_dDistanceRight_soll;

    QMutex m_Mutex;



public  slots:
    void slot_SpeedControl();
signals:
    void sgn_Finished();

};

#endif // DRIVECONTROL_H
