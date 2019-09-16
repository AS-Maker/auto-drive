#include "mobileplatform.h"
#include <QtGui>



#define nSensorIDLineSensorLeft 100
#define nSensorIDLineSensorRight 200
#define nSensorIDEncoderLeft 300
#define nSensorIDEncoderRight 400
#define nSensorIDUltrasonicSensor 500
#define nSensorIDColorSensor 600

MobilePlatform::MobilePlatform()
{

    // Motor Links
    nPinMotorALeft = 12;                   // GPIO 10
    nPinMotorBLeft = 13;                   // GPIO 9
    nPinMotorELeft = 3;                   // GPIO 22
    m_pMotorLeft = new DcMotor(nPinMotorALeft, nPinMotorBLeft, nPinMotorELeft);

    // Motor Rechts
    nPinMotorARight = 15;                 // GPIO 14
    nPinMotorBRight = 16;                 // GPIO 15
    nPinMotorERight = 4;                 // GPIO 23
    m_pMotorRight = new DcMotor(nPinMotorARight, nPinMotorBRight, nPinMotorERight);

    // Linien-Sensor Links
    nPinLineSensorLeft = 24;              // GPIO 19
    m_pLineSensorLeft = new LineSensor(nSensorIDLineSensorLeft, nPinLineSensorLeft);
    m_pThreadLineLeft = new QThread();
    connect(this, SIGNAL(sgn_Started_LineSensor()), m_pLineSensorLeft, SLOT(slot_StartProcess()));
    connect(this, SIGNAL(sgn_Finished_LineSensor()), m_pLineSensorLeft, SLOT(slot_StopProcess()));
    connectSensorThread(m_pLineSensorLeft, m_pThreadLineLeft);

    // Linien-Sensor Rechts
    nPinLineSensorRight = 25;             // GPIO 26
    m_pLineSensorRight = new LineSensor(nSensorIDLineSensorRight, nPinLineSensorRight);
    m_pThreadLineRight = new QThread();
    connect(this, SIGNAL(sgn_Started_LineSensor()), m_pLineSensorRight, SLOT(slot_StartProcess()));
    connect(this, SIGNAL(sgn_Finished_LineSensor()), m_pLineSensorRight, SLOT(slot_StopProcess()));
    connectSensorThread(m_pLineSensorRight, m_pThreadLineRight);

    // Encoder Links
    nPinEncoderALeft = 22;                // GPIO 6
    nPinEncoderBLeft = 23;                // GPIO 13
    m_pEncoderLeft = new Encoder(nSensorIDEncoderLeft, nPinEncoderALeft, nPinEncoderBLeft);
    m_pThreadEncoderLeft = new QThread();
    connect(this, SIGNAL(sgn_Started_Encoder()), m_pEncoderLeft, SLOT(slot_StartProcess()));
    connect(this, SIGNAL(sgn_Finished_Encoder()), m_pEncoderLeft, SLOT(slot_StopProcess()));
    connect(m_pThreadEncoderLeft, SIGNAL(started()), m_pEncoderLeft, SLOT(slot_Process()));
    connectSensorThread(m_pEncoderLeft, m_pThreadEncoderLeft);

    // Encoder Rechts
    nPinEncoderARight = 6;               // GPIO 25
    nPinEncoderBRight = 10;               // GPIO 8
    m_pEncoderRight = new Encoder(nSensorIDEncoderRight, nPinEncoderARight, nPinEncoderBRight);
    m_pThreadEncoderRight = new QThread();
    connect(this, SIGNAL(sgn_Started_Encoder()), m_pEncoderRight, SLOT(slot_StartProcess()));
    connect(this, SIGNAL(sgn_Finished_Encoder()), m_pEncoderRight, SLOT(slot_StopProcess()));
    connect(m_pThreadEncoderRight, SIGNAL(started()), m_pEncoderRight, SLOT(slot_Process()));
    connectSensorThread(m_pEncoderRight, m_pThreadEncoderRight);

    // Ultraschall-Sensor
    nPinUltrasonicSensor = 21;             // GPIO 5
    m_pThreadUltrasonic = new QThread();
    m_pUltrasonicSensor = new UltrasonicSensor(nSensorIDUltrasonicSensor, nPinUltrasonicSensor);
    connect(this, SIGNAL(sgn_Started_UltrasonicSensor()), m_pUltrasonicSensor, SLOT(slot_StartProcess()));
    connect(this, SIGNAL(sgn_Finished_UltrasonicSensor()), m_pUltrasonicSensor, SLOT(slot_StopProcess()));
    connectSensorThread(m_pUltrasonicSensor, m_pThreadUltrasonic);

    // Farb-Sensor
    m_pColorSensor = new ColorSensor(nSensorIDColorSensor);
    m_pThreadColor = new QThread();
    connect(this, SIGNAL(sgn_Started_ColorSensor()), m_pColorSensor, SLOT(slot_StartProcess()));
    connect(this, SIGNAL(sgn_Finished_ColorSensor()), m_pColorSensor, SLOT(slot_StopProcess()));
    connectSensorThread(m_pColorSensor, m_pThreadColor);

    //Odometry
    m_pdrivecontrol = new drivecontrol(*m_pMotorLeft, *m_pMotorRight, *m_pEncoderLeft,*m_pEncoderRight);
    m_pThreadDrivecontrol= new QThread();

    connectThread(m_pdrivecontrol,m_pThreadDrivecontrol);


    //Challenge

    m_pStateMaschine = new StateMachine(*m_pMotorLeft,*m_pMotorRight,*m_pdrivecontrol,*m_pLineSensorLeft,*m_pLineSensorRight,*m_pColorSensor,*m_pUltrasonicSensor );


    connect(m_pStateMaschine, SIGNAL(sgn_sendOrderlist(OrderList)), this, SIGNAL(sgn_sendOrderlist(OrderList)));
    connect(m_pStateMaschine, SIGNAL(sgn_sendCurrenteState(int,int)),this, SIGNAL(sgn_sendCurrenteState(int,int)));
    connect(m_pStateMaschine, SIGNAL(sgn_stateMachineDone()),this,SIGNAL(sgn_stateMachineDone()));

    connect(this, SIGNAL(sgn_SetSpeed(int)), m_pStateMaschine, SLOT(slot_SetSpeed(int)));
    connect(this,SIGNAL(sgn_ButtonStop()),m_pStateMaschine,SIGNAL(sgn_stopMoving()));

    connect(this, SIGNAL(sgn_startStateMachine()), m_pStateMaschine, SIGNAL(sgn_startStateMachine()));
   connect(this, SIGNAL(sgn_pathToXML(QString)), m_pStateMaschine, SLOT(slot_readOrders(QString)));



    m_Mutex.lock();
    m_nSpeed = 0;

    m_bFollowLineIsRunning = true;
    m_Mutex.unlock();

}

MobilePlatform::~MobilePlatform()
{


    delete m_pColorSensor;
    delete m_pUltrasonicSensor;
    delete m_pEncoderRight;
    delete m_pEncoderLeft;
    delete m_pLineSensorRight;
    delete m_pLineSensorLeft;
    delete m_pMotorRight;
    delete m_pMotorLeft;
    delete m_pdrivecontrol;

    std::cout << "Kill MobilePlatform" << std::endl;
}

void MobilePlatform::connectSensorThread(AbstractSensor *Sensor, QThread *Thread)
{
    Sensor->moveToThread(Thread);
    connect(Sensor, SIGNAL(sgn_Finished()), Thread, SLOT(quit()));
    connect(Thread, SIGNAL(finished()), Thread, SLOT(deleteLater()));
    connect(Sensor, SIGNAL(sgn_NewSensorData(int,int)), this, SIGNAL(sgn_NewSensorData(int,int)));
    Thread->start();
}

void MobilePlatform::connectThread(drivecontrol* Object, QThread* Thread){
    Thread = new QThread();
    Object->moveToThread(Thread);

    connect(Object, SIGNAL(sgn_Finished()), Thread,SLOT(quit()));
    connect(Object, SIGNAL(sgn_Finished()), Object, SLOT(deleteLater()));
    connect(Thread, SIGNAL(finished()),Thread, SLOT(deleteLater()));

    Thread->start();
}



void MobilePlatform::slot_moveForward()
{
    slot_SetSpeed(m_nSpeed);
    m_pMotorLeft->Forward();
    m_pMotorRight->Forward();

    emit sgn_ButtonForward();
}

void MobilePlatform::slot_moveBackward()
{
    slot_SetSpeed(m_nSpeed);
    m_pMotorLeft->Backward();
    m_pMotorRight->Backward();

    emit sgn_ButtonBackward();
}

void MobilePlatform::slot_turnLeft()
{
    slot_SetSpeed(m_nSpeed);
    m_pMotorLeft->StopMotor();
    m_pMotorRight->Forward();

    emit sgn_ButtonLeft();
}

void MobilePlatform::slot_turnRight()
{
    slot_SetSpeed(m_nSpeed);
    m_pMotorLeft->Forward();
    m_pMotorRight->StopMotor();

    emit sgn_ButtonRight();
}

void MobilePlatform::slot_stopMotion()
{

    m_pdrivecontrol->m_bOdometryIsRunning=false;
    m_bFollowLineIsRunning = false;



    m_pMotorLeft->StopMotor();
    m_pMotorRight->StopMotor();

    emit sgn_ButtonStop();
}

void MobilePlatform::slot_SetSpeed(int nSpeed)
{
    m_nSpeed = nSpeed;
    m_pMotorLeft->SetPWM(nSpeed);
    m_pMotorRight->SetPWM(nSpeed);
}

void MobilePlatform::slot_FollowLine()
{
    m_bFollowLineIsRunning = true;
    emit sgn_ButtonsFollowLine();
    slot_SetSpeed(m_nSpeed);
    int m_nLastTurn=0;

    while(m_bFollowLineIsRunning == true)
    {
        int L = m_pLineSensorLeft->nGetLastStatus();
        int R = m_pLineSensorRight->nGetLastStatus();

        if(m_pColorSensor->m_nColor == nColorRed || m_pColorSensor->m_nColor == nColorYellow || m_pColorSensor->m_nColor == nColorGreen || m_pColorSensor->m_nColor == nColorBlue)
        {
            m_bFollowLineIsRunning = false;
            m_pMotorLeft->StopMotor();
            m_pMotorRight->StopMotor();
            break;
        }
        if((L==sensorwhite)&&(R==sensorwhite)&&m_pColorSensor->m_nColor == nColorBlack){
            //beide sensoren detektieren weiss. Anfangssituation.
            m_pMotorLeft->Forward();
            m_pMotorRight->Forward();
        }
        if((L==sensorwhite)&&(R==sensorwhite)&&m_pColorSensor->m_nColor == nColorWhite){
            if(m_nLastTurn == turnleft){
                m_pMotorLeft->Backward();
                m_pMotorRight->Forward();

            }
            else {
                // hier korrigieren nach rechts abbiegen
                m_pMotorLeft->Forward();
                m_pMotorRight->Backward();
            }
        }
        //beide sensoren sind gleich bei ablenken dann soll das korrigiert. hier beide sind schwarz
        if(L==sensorblack && R==sensorblack ){
            // hier korrigieren nach links abbiegen falls letztens nach links abgebogen hat
            if(m_nLastTurn == turnleft){
                m_pMotorLeft->StopMotor();
                m_pMotorRight->Forward();

            }
            else {
                // hier korrigieren nach rechts abbiegen
                m_pMotorLeft->Forward();
                m_pMotorRight->StopMotor();
            }
        }
        // wenn nur linker sensor schwarz detektiert dann soll er links abbiegen
        else if(L==sensorblack){
            m_pMotorLeft->StopMotor();
            m_pMotorRight->Forward();
            m_nLastTurn = turnleft;}
        else if(R==sensorblack ) {
            // wenn dann nur rechter sensor schwarz detektiert dann soll er rechts abbiegen
            m_pMotorLeft->Forward();
            m_pMotorRight->StopMotor();
            m_nLastTurn = turnright;}
        QCoreApplication::processEvents();
    }
}



void MobilePlatform::slot_Odometry()
{

    m_Mutex.lock();
    m_pdrivecontrol->m_bOdometryIsRunning = true;
    m_Mutex.unlock();

    if(m_pdrivecontrol->m_bOdometryIsRunning == true && m_nSpeed != 0)
    {
        m_pdrivecontrol->DriveLine(0.8);
        delayMicroseconds(1000);


        m_pdrivecontrol->DriveAngle(180);
        delayMicroseconds(1000);

        m_pdrivecontrol->DriveLine(0.6);
    }

    m_Mutex.lock();
    m_pdrivecontrol->m_bOdometryIsRunning = false;
    m_Mutex.unlock();
    m_pMotorLeft->StopMotor();
    m_pMotorRight->StopMotor();


    emit sgn_ShowButtonsOdometry();
}

