//!
//! \file mobilplatform.h
//! \author Team-3
//! \date 7-Januar -2018

//!
//!Hier werden Objekte von DC-Motoren, Encoders, Sensoren, und Threads erstellt
//!


#ifndef MOBILEPLATFORM_H
#define MOBILEPLATFORM_H
//#include <QApplication>
//#include <QState>
//#include <QStateMachine>

#include <dcmotor.h>
#include <linesensor.h>
#include "colorsensor.h"
#include "ultrasonicsensor.h"
#include "drivecontrol.h"
#include "statemachine.h"


#include <encoder.h>
#include <QObject>
#include <QThread>
#include <QTimer>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QMutex>

const int turnleft = 1;
const int turnright = 2;
const int sensorwhite = 0;
const int sensorblack = 1;


class MobilePlatform : public QObject
{
    Q_OBJECT


public:
    //! Konstruktor
    //!
    //! Erstellt ein Objekt der Klasse MobilePlatform. Hier
    //! werden auch alle weiteren spaeter benoetigten Objekte erstellt.

    MobilePlatform();

    //!
    //! Destruktor loescht das Objekt MobilePlatform
    //!

    ~MobilePlatform();


    //! Methode, die bestimmte Signale mit zugehörigen Slots beim erstellen eines Threads verbindet.

    void connectSensorThread(AbstractSensor *Sensor, QThread *Thread);

    //! Methode, die Thread mit objekt von drivecontrol verbindet

    void connectThread(drivecontrol* object, QThread* thread);

    //! Methode, die Thread mit objekt von Statemaschine verbindet

    void connectThread(StateMachine* Object, QThread* Thread);


signals:

    void sgn_ButtonForward();

    void sgn_ButtonBackward();

    void sgn_ButtonLeft();

    void sgn_ButtonRight();

    void sgn_ButtonStop();

    void sgn_ButtonsFollowLine();

    void sgn_NewSensorData(int nSensorID, int nSensordaten);

    void sgn_Started_LineSensor();

    void sgn_Finished_LineSensor();

    void sgn_Started_Encoder();

    void sgn_Finished_Encoder();


    void sgn_Started_ColorSensor();

    void sgn_Finished_ColorSensor();


    void sgn_Started_UltrasonicSensor();


    void sgn_Finished_UltrasonicSensor();

    void sgn_ShowButtonsOdometry();



    void sgn_sendOrderlist(OrderList);
    void sgn_startStateMachine();
    void sgn_pathToXML(QString);
    void sgn_sendCurrenteState(int, int);
    void sgn_setDesiredColorFromGUI(int);
    void sgn_turnIntoRackGUITriggered();
    void sgn_wareabliefern();
    void sgn_stateMachineDone();
    void sgn_SetSpeed(int);


public slots:

    void slot_moveForward();


    void slot_moveBackward();

    void slot_turnLeft();

    void slot_turnRight();

    void slot_stopMotion();

    void slot_SetSpeed(int nSpeed);

    void slot_FollowLine();

    void slot_Odometry();


private:
    int m_nSpeed;

    bool m_bFollowLineIsRunning;


    //!
    //! \brief Motor Links
    //!
    int nPinMotorALeft;

    int nPinMotorBLeft;

    int nPinMotorELeft;

    DcMotor *m_pMotorLeft;

    //!
    //! \brief Motor Rechts
    //!
    int nPinMotorARight;

    int nPinMotorBRight;

    int nPinMotorERight;

    DcMotor *m_pMotorRight;

    //!
    //! \brief Linien-Sensor Links
    //!
    int nPinLineSensorLeft;

    LineSensor *m_pLineSensorLeft;

    QThread *m_pThreadLineLeft;

    //!
    //! \brief Linien-Sensor Rechts
    //!

    int nPinLineSensorRight;

    LineSensor *m_pLineSensorRight;

    QThread *m_pThreadLineRight;

    //!
    //! \brief Encoder Left
    //!

    int nPinEncoderALeft;

    int nPinEncoderBLeft;

    Encoder *m_pEncoderLeft;

    QThread *m_pThreadEncoderLeft;

    //!
    //! \brief Encoder Rechts
    //!
    int nPinEncoderARight;

    int nPinEncoderBRight;

    Encoder *m_pEncoderRight;

    QThread *m_pThreadEncoderRight;

    //!
    //! \brief UltrasonicSensor
    //!
    int nPinUltrasonicSensor;

    UltrasonicSensor *m_pUltrasonicSensor;

    QThread *m_pThreadUltrasonic;

    //!
    //! \brief Zeiger von dem ColorSensor
    //!
    ColorSensor *m_pColorSensor;

    QThread *m_pThreadColor;

    //!
    //! \brief Zeiger von der Odometrie
    //!

   drivecontrol *m_pdrivecontrol;
   QThread *m_pThreadDrivecontrol;
   //!
   //! \brief Zeiger von der StateMaschine
   //!
   StateMachine *m_pStateMaschine;
   QThread *m_pThreadStatemaschine;




   QMutex m_Mutex;

};

#endif // MOBILEPLATFORM_H
