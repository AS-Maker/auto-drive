
#include "statemachine.h"
#include "xmlreader.h"

StateMachine::StateMachine(DcMotor &MotorLeft, DcMotor &MotorRight,
                          drivecontrol &drivecontrol, LineSensor &LineSensorLeft,
                           LineSensor &LineSensorRight, ColorSensor &ColorSensor, UltrasonicSensor &Ultrasonic){
    this->m_pMotorLeft=&MotorLeft;
    this->m_pMotorRight=&MotorRight;
    this->m_pdrivecontrol=&drivecontrol;
    this->m_pLineSensorLeft=&LineSensorLeft;
    this->m_pLineSensorRight=&LineSensorRight;
    this->m_pColorSensor=&ColorSensor;
    this->m_pUltrasonic=&Ultrasonic;

    pinMode(m_nGreenLEDPin, OUTPUT);
    pinMode(m_nYellowLEDPin, OUTPUT);
    pinMode(m_nRedLEDPin, OUTPUT);

    digitalWrite(m_nGreenLEDPin,0);
    digitalWrite(m_nRedLEDPin,0);
    digitalWrite(m_nYellowLEDPin,0);

    m_pTimer_LEDBlink = new QTimer();
    m_pTimer_LEDBlink->setSingleShot(FALSE);
    m_pTimer_LEDBlink->setInterval(200);
    connect(m_pTimer_LEDBlink, SIGNAL(timeout()), this, SLOT(slot_LEDBlink()));


    connect(this, SIGNAL(sgn_stopMoving()),this,SLOT(slot_setStopMoving()));

    createStateMachine();

}


void StateMachine::createStateMachine(){
    m_pStateMachine = new QStateMachine();
    m_pAnfangszustand = new QState(); //S1
    m_pvomStartzumLagerArtikel = new QState(); // S2
    m_pFahrenInDenLagerArtikel = new QState(); // S3
    m_pWareAufnehmen = new QState(); // S4
    m_pFahrenZumLagerist = new QState(); // S5
    m_pWareAbliefern = new QState(); // S6
    m_pFahrenZumLagerArtikel = new QState(); // S7
    m_pFahrenZumStartpunkt = new QState(); // S8
    m_pEndeDesAuftrags = new QState(); // S9


    m_pStateMachine->addState(m_pAnfangszustand);
    m_pStateMachine->addState(m_pvomStartzumLagerArtikel);
    m_pStateMachine->addState(m_pFahrenInDenLagerArtikel);
    m_pStateMachine->addState(m_pWareAufnehmen);
    m_pStateMachine->addState(m_pFahrenZumLagerist);
    m_pStateMachine->addState(m_pWareAbliefern);
    m_pStateMachine->addState(m_pFahrenZumLagerArtikel);
    m_pStateMachine->addState(m_pFahrenZumStartpunkt);
    m_pStateMachine->addState(m_pEndeDesAuftrags);


    m_pStateMachine->setInitialState(m_pAnfangszustand);

    m_pAnfangszustand->addTransition(this, SIGNAL(sgn_startStateMachine()), m_pvomStartzumLagerArtikel);

    m_pvomStartzumLagerArtikel->addTransition(this, SIGNAL(sgn_stateDone()), m_pFahrenInDenLagerArtikel);
    m_pvomStartzumLagerArtikel->addTransition(this, SIGNAL(sgn_stopMoving()), m_pEndeDesAuftrags);

    m_pFahrenInDenLagerArtikel->addTransition(this, SIGNAL(sgn_stateDone()), m_pWareAufnehmen);
    m_pFahrenInDenLagerArtikel->addTransition(this, SIGNAL(sgn_stopMoving()), m_pEndeDesAuftrags);


    m_pWareAufnehmen->addTransition(this, SIGNAL(sgn_stateDone()), m_pFahrenZumLagerist);
    m_pWareAufnehmen->addTransition(this, SIGNAL(sgn_stopMoving()), m_pEndeDesAuftrags);

    m_pFahrenZumLagerist->addTransition(this, SIGNAL(sgn_wareAbliefern()), m_pWareAbliefern);
    m_pFahrenZumLagerist->addTransition(this, SIGNAL(sgn_stopMoving()), m_pEndeDesAuftrags);


    m_pWareAbliefern->addTransition(this, SIGNAL(sgn_fahrenZumLagerArtikel()), m_pFahrenZumLagerArtikel);
    m_pWareAbliefern->addTransition(this, SIGNAL(sgn_stopMoving()), m_pEndeDesAuftrags);
    m_pWareAbliefern->addTransition(this, SIGNAL(sgn_fahrenZumStartpunkt()), m_pFahrenZumStartpunkt);

    m_pFahrenZumLagerArtikel->addTransition(this, SIGNAL(sgn_stateDone()), m_pFahrenInDenLagerArtikel);
    m_pFahrenZumLagerArtikel->addTransition(this, SIGNAL(sgn_stopMoving()), m_pEndeDesAuftrags);

    m_pFahrenZumStartpunkt->addTransition(this, SIGNAL(sgn_stateDone()), m_pEndeDesAuftrags);
    m_pFahrenZumStartpunkt->addTransition(this, SIGNAL(sgn_stopMoving()), m_pEndeDesAuftrags);

    m_pEndeDesAuftrags->addTransition(m_pAnfangszustand);

    connect(m_pvomStartzumLagerArtikel, SIGNAL(entered()), this, SLOT(slot_vomStartzumLagerArtikel()));
    connect(m_pFahrenInDenLagerArtikel, SIGNAL(entered()), this, SLOT(slot_fahrenInDenLagerArtikel()));
    connect(m_pWareAufnehmen, SIGNAL(entered()), this, SLOT(slot_wareAufnehmen()));
    connect(m_pFahrenZumLagerist, SIGNAL(entered()), this, SLOT(slot_fahrenZumLagerist()));
    connect(m_pWareAbliefern, SIGNAL(entered()), this, SLOT(slot_wareAbliefern()));
    connect(m_pFahrenZumLagerArtikel, SIGNAL(entered()),this,SLOT(slot_fahrenZumLagerArtikel()));
    connect(m_pFahrenZumStartpunkt, SIGNAL(entered()), this, SLOT(slot_fahrenZumStartpunkt()));
    connect(m_pEndeDesAuftrags,SIGNAL(entered()),this, SLOT(slot_endeDesAuftrags()));


    m_pStateMachine->start();
}

void StateMachine::slot_SetSpeed(int nSpeed)
{
    m_nSpeed = nSpeed;
    m_pMotorLeft->SetPWM(nSpeed);
    m_pMotorRight->SetPWM(nSpeed);
}


void StateMachine::slot_readOrders(QString sPath){
    //QString path="/home/pi/Desktop/example_deliveryPlan2.xml";
    //sPath="/home/pi/Desktop/example_deliveryPlan.xml";

    job = xmlReader::parseFile(sPath);

    qRegisterMetaType<OrderList >("OrderList"); // Registrierung des Typs, um sie bei Signal/Slot Vorgängen anwenden zu können


    emit sgn_sendOrderlist(job);
}

//switch case
void StateMachine::slot_vomStartzumLagerArtikel(){
    m_bStopMoving=false;

    emit sgn_sendCurrenteState(fahrezumlagerartikel,m_nOrdernumber);

    switch (job.orders[m_nOrdernumber].storageRack){
    case Order::StorageRack::RACK_RED:
        m_nCurrentWarehouse=nColorRed;
        m_bClockwiseDriving=TRUE;

        break;
    case Order::StorageRack::RACK_GREEN:
        m_nCurrentWarehouse=nColorGreen;
        m_bClockwiseDriving=FALSE;

        break;
    case Order::StorageRack::RACK_YELLOW:
        m_nCurrentWarehouse=nColorYellow;
        m_bClockwiseDriving=TRUE;

        break;

    case Order::StorageRack::UNKNOWN_RACK:
        m_nCurrentWarehouse=nColorUnknown;

        break;
    }

    waitforcolor=nColorBlue;
    slot_followLine2();
    QThread::msleep(100);
    m_pdrivecontrol->DriveLine(0.02);
    if(m_bClockwiseDriving){
        m_pdrivecontrol->DriveAngle(-45);

    }else{
        m_pdrivecontrol->DriveAngle(45);

    }
    waitforcolor=m_nCurrentWarehouse;
    slot_followLine2();
    QThread::msleep(100);
    m_pdrivecontrol->DriveLine(0.04);
    emit sgn_stateDone();

}

//switch case
void StateMachine::slot_fahrenInDenLagerArtikel(){

    readyfordelivery = true;
    cout<<"redLED blinken"<<endl;
    emit sgn_sendCurrenteState(wareaufnehmen, m_nOrdernumber);
    if(m_bClockwiseDriving){
        switch (job.orders[m_nOrdernumber].storageRack){
        case Order::StorageRack::RACK_RED:
            m_pdrivecontrol->DriveAngle(50);
            break;
        case Order::StorageRack::RACK_GREEN:
            m_pdrivecontrol->DriveAngle(-50);
            break;
        case Order::StorageRack::RACK_YELLOW:
            m_pdrivecontrol->DriveAngle(-50);
            break;
        case Order::StorageRack::UNKNOWN_RACK:

            break;
        }
    } else{
        switch (job.orders[m_nOrdernumber].storageRack){
        case Order::StorageRack::RACK_RED:
            m_pdrivecontrol->DriveAngle(-50);
            break;
        case Order::StorageRack::RACK_GREEN:
            m_pdrivecontrol->DriveAngle(50);
            break;
        case Order::StorageRack::RACK_YELLOW:
            m_pdrivecontrol->DriveAngle(50);
            break;
        case Order::StorageRack::UNKNOWN_RACK:

            break;
        }
    }

    m_pTimer_LEDBlink->start();
    slot_followLine2();
    emit sgn_stateDone();
}


void StateMachine::slot_wareAufnehmen(){

    m_pdrivecontrol->DriveLine(0.2);


QThread::msleep(200);
   cout<<"STOP!"<<endl;
    m_pTimer_LEDBlink->stop();
    setLEDSteady(TRUE);

    m_pdrivecontrol->DriveAngle(180);

    waitforcolor=m_nCurrentWarehouse;
    slot_followLine2();
    QThread::msleep(100);
    m_pdrivecontrol->DriveLine(0.04);


    emit sgn_stateDone();
}

//switch case
void StateMachine::slot_fahrenZumLagerist(){
    emit sgn_sendCurrenteState(fahrezumlagerist, m_nOrdernumber);

    switch (job.orders[m_nOrdernumber].Lagerist){
    case Order::WarehouseClerk::Lagerist_A:
        m_cCurrentLagerist='A';
        m_bClockwiseDriving=false;
        switch (job.orders[m_nOrdernumber].storageRack){
        case Order::StorageRack::RACK_RED:
            m_pdrivecontrol->DriveAngle(-50);
            break;
        case Order::StorageRack::RACK_GREEN:
            m_pdrivecontrol->DriveAngle(50);
            break;
        case Order::StorageRack::RACK_YELLOW:
            m_pdrivecontrol->DriveAngle(50);
            break;
        case Order::StorageRack::UNKNOWN_RACK:

            break;
        }
        break;
    case Order::WarehouseClerk::Lagerist_B:
        m_bClockwiseDriving=true;
        m_cCurrentLagerist='B';
        switch (job.orders[m_nOrdernumber].storageRack){
        case Order::StorageRack::RACK_RED:
            m_pdrivecontrol->DriveAngle(50);
            break;
        case Order::StorageRack::RACK_GREEN:
            m_pdrivecontrol->DriveAngle(-50);
            break;
        case Order::StorageRack::RACK_YELLOW:
            m_pdrivecontrol->DriveAngle(-50);
            break;
        case Order::StorageRack::UNKNOWN_RACK:

            break;
        }
        break;
    case Order::WarehouseClerk::UNKNOWN_Lagerist:
        m_cCurrentLagerist='N';

        break;
    }
    waitforcolor = nColorBlue;
    slot_followLine2();
    QThread::msleep(100);
    m_pdrivecontrol->DriveLine(0.04);

    if(m_bClockwiseDriving){
        m_pdrivecontrol->DriveAngle(50);
    }else{
        m_pdrivecontrol->DriveAngle(-50);
    }
    waitforcolor = nColorBlue;
    slot_followLine2();
    emit sgn_wareAbliefern();
}



void StateMachine::slot_wareAbliefern(){

    emit sgn_sendCurrenteState(wareabliefern, m_nOrdernumber);

    QThread::msleep(200);
    setLEDSteady(FALSE);
    m_bObjectFound = false;
    m_pdrivecontrol->DriveLine(0.08);
    m_pdrivecontrol->DriveAngle(180);
    m_nOrdernumber++;
    if(m_nOrdernumber==job.orders.length())
    {
        emit sgn_sendCurrenteState(fahrezumstart, 0);
        emit sgn_fahrenZumStartpunkt();
    }else
    {
        emit sgn_sendCurrenteState(fahrezumlagerartikel, m_nOrdernumber);
        emit sgn_fahrenZumLagerArtikel();
    }
    waitforcolor = nColorBlue;

    slot_followLine2();
    QThread::msleep(100);
    m_pdrivecontrol->DriveLine(0.04);
}

//switch case
void StateMachine::slot_fahrenZumLagerArtikel()
{
    if(m_cCurrentLagerist=='A'){
        m_pdrivecontrol->DriveAngle(50);
        switch (job.orders[m_nOrdernumber].storageRack){
        case Order::StorageRack::RACK_RED:
            m_nCurrentWarehouse=1;
            m_bClockwiseDriving=TRUE;
            break;
        case Order::StorageRack::RACK_GREEN:
            m_nCurrentWarehouse=2;
            m_bClockwiseDriving=TRUE;
            break;
        case Order::StorageRack::RACK_YELLOW:
            m_nCurrentWarehouse=4;
            m_bClockwiseDriving=TRUE;
            break;
        case Order::StorageRack::UNKNOWN_RACK:
            m_nCurrentWarehouse=0;

            break;
        }
    }else{
        m_pdrivecontrol->DriveAngle(-50);
        switch (job.orders[m_nOrdernumber].storageRack){
        case Order::StorageRack::RACK_RED:
            m_nCurrentWarehouse=1;
            m_bClockwiseDriving=FALSE;
            break;
        case Order::StorageRack::RACK_GREEN:
            m_nCurrentWarehouse=2;
            m_bClockwiseDriving=FALSE;
            break;
        case Order::StorageRack::RACK_YELLOW:
            m_nCurrentWarehouse=4;
            m_bClockwiseDriving=FALSE;
            break;
        case Order::StorageRack::UNKNOWN_RACK:
            m_nCurrentWarehouse=0;

            break;
        }
    }
    waitforcolor = m_nCurrentWarehouse;
    slot_followLine2();
    m_pdrivecontrol->DriveLine(0.04);
    emit sgn_stateDone();
}

//switch case
void StateMachine::slot_fahrenZumStartpunkt(){

    if(m_cCurrentLagerist=='A'){
        m_pdrivecontrol->DriveAngle(-50);
        m_bClockwiseDriving=FALSE;
    }else{
        m_pdrivecontrol->DriveAngle(50);
        m_bClockwiseDriving=TRUE;
    }

    waitforcolor = nColorBlue;
    slot_followLine2();
    m_pdrivecontrol->DriveLine(0.04);
    if(m_bClockwiseDriving){
        m_pdrivecontrol->DriveAngle(-50);
    }else{
        m_pdrivecontrol->DriveAngle(50);
    }

    waitforcolor = nColorBlue;
    slot_followLine2();
}


void StateMachine::slot_endeDesAuftrags(){

    m_pTimer_LEDBlink->stop();


    setLEDSteady(false);


    m_nOrdernumber=0;

    emit sgn_stateMachineDone();

}








void StateMachine::slot_stopWorking(){
    emit sgn_stopMoving();
    emit sgn_finished();
}

void StateMachine::slot_setStopMoving(){
    m_bStopMoving=true;
    m_bObjectFound = false;

    m_pMotorRight->StopMotor();
    m_pMotorLeft->StopMotor();

    cout<<"setstopmoving ausgeführt"<<endl;
}

//
void StateMachine::setLEDSteady(bool state){
    m_bLEDSwitch=state;

    switch (job.orders[m_nOrdernumber].storageRack){
    case Order::StorageRack::RACK_RED:
        digitalWrite(m_nRedLEDPin, m_bLEDSwitch);
        break;
    case Order::StorageRack::RACK_GREEN:
        digitalWrite(m_nGreenLEDPin,m_bLEDSwitch);
        break;
    case Order::StorageRack::RACK_YELLOW:
        digitalWrite(m_nYellowLEDPin,m_bLEDSwitch);
        break;
    case Order::StorageRack::UNKNOWN_RACK:
        cout<<"Ungültiges Rack";
        break;
    }
}



void StateMachine::slot_LEDBlink(){
    setLEDSteady(m_bLEDSwitch);
    m_bLEDSwitch=!m_bLEDSwitch;
}



//Folgende Funktion unterscheidet sich fon der slot_followLine() Funktion die wir in MobilePlatform benutzen.
//Der Unterschied ist, dass diese Funktion Farbsensor, und PID Regler benutzt umd die Farbmarker zu detektieren und entsprechend
//mit definierter Geschwindigkeit und mit definierter Distanz zu bewegen.
void StateMachine::slot_followLine2(){
    m_bFollowLineIsRunning = true;
    int m_nLastTurn=0;
    const int turnleft = 1;
    const int turnright = 2;
    const int sensorwhite = 0;
    const int sensorblack = 1;

    slot_SetSpeed(m_nSpeed);

    while(m_bFollowLineIsRunning == true && !m_bStopMoving)
    {
        int L = m_pLineSensorLeft->nGetLastStatus();
        int R = m_pLineSensorRight->nGetLastStatus();
// Wenn unser farbsensor eine Farbe detektiert, dann soll er aus der while schleife und aus der Funktion. Und gemäß der Farbe soll der Roboter weiter fahren
        if(m_pColorSensor->m_nColor == waitforcolor)
        {
            waitforcolor = 0;
            m_bFollowLineIsRunning = false;
            m_pMotorLeft->StopMotor();
            m_pMotorRight->StopMotor();
            break;
        }
        else if(m_pColorSensor->m_nColor != nColorBlack && m_pColorSensor->m_nColor != nColorWhite && m_pColorSensor->m_nColor !=nColorUnknown)
        {
            m_pdrivecontrol->DriveLine(0.08);
            slot_SetSpeed(m_nSpeed);
        }
        //Falls der Roboter unter die Plexiglasplatte fährt dann soll er aus der while schleife
        if(m_pUltrasonic->m_fdistanceCM<40 && !m_bObjectFound && readyfordelivery == true)
        {
            m_bObjectFound = true;
            m_bFollowLineIsRunning = false;
            readyfordelivery = false;
            m_pMotorLeft->StopMotor();
            m_pMotorRight->StopMotor();
            break;
        }

       //wenn all die Sensoren weiss detektieren dann soll der roboter drehen um wieder an der Linie zu kommen

        if((L==sensorwhite)&&(R==sensorwhite)&&(m_pColorSensor->m_nColor == nColorWhite)){
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



        else if((L==sensorwhite)&&(R==sensorwhite)){

            m_pMotorLeft->Forward();
            m_pMotorRight->Forward();
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

StateMachine::~StateMachine(){

    delete m_pTimer_LEDBlink;


    delete m_pStateMachine;
}


