#include "controlsystem.h"

ControlSystem::ControlSystem(QObject *parent):
    QObject(parent)
{
    m_pMainWindow = new MainWindow();
    m_pMobilePlatform = new MobilePlatform();

    connect(m_pMobilePlatform, SIGNAL(sgn_ButtonForward()), m_pMainWindow, SLOT(slot_ButtonForward()));
    connect(m_pMobilePlatform, SIGNAL(sgn_ButtonBackward()), m_pMainWindow, SLOT(slot_ButtonBackward()));
    connect(m_pMobilePlatform, SIGNAL(sgn_ButtonLeft()), m_pMainWindow, SLOT(slot_ButtonLeft()));
    connect(m_pMobilePlatform, SIGNAL(sgn_ButtonRight()), m_pMainWindow, SLOT(slot_ButtonRight()));
    connect(m_pMobilePlatform, SIGNAL(sgn_ButtonStop()), m_pMainWindow, SLOT(slot_ButtonStop()));
    connect(m_pMobilePlatform, SIGNAL(sgn_ButtonsFollowLine()),m_pMainWindow, SLOT(slot_ButtonsFollowLine()));


    connect(m_pMobilePlatform, SIGNAL(sgn_ShowButtonsOdometry()),m_pMainWindow,SLOT(slot_ShowButtonsOdometry()));

    connect(m_pMainWindow, SIGNAL(sgn_SetSpeed(int)), m_pMobilePlatform, SLOT(slot_SetSpeed(int)));
    connect(m_pMainWindow, SIGNAL(sgn_SetSpeed(int)), m_pMobilePlatform, SIGNAL(sgn_SetSpeed(int)));
    connect(m_pMainWindow, SIGNAL(sgn_Forward()), m_pMobilePlatform, SLOT(slot_moveForward()));
    connect(m_pMainWindow, SIGNAL(sgn_Stop()), m_pMobilePlatform, SLOT(slot_stopMotion()));

    connect(m_pMainWindow, SIGNAL(sgn_Backward()), m_pMobilePlatform, SLOT(slot_moveBackward()));
    connect(m_pMainWindow, SIGNAL(sgn_Left()), m_pMobilePlatform, SLOT(slot_turnLeft()));
    connect(m_pMainWindow, SIGNAL(sgn_Right()), m_pMobilePlatform, SLOT(slot_turnRight()));
    connect(m_pMainWindow, SIGNAL(sgn_FollowLine()), m_pMobilePlatform, SLOT(slot_FollowLine()));
    connect(m_pMainWindow, SIGNAL(sgn_Odometry()), m_pMobilePlatform, SLOT(slot_Odometry()));
    connect(m_pMainWindow, SIGNAL(sgn_startStateMachine()), m_pMobilePlatform, SIGNAL(sgn_startStateMachine()));

    connect(m_pMobilePlatform, SIGNAL(sgn_NewSensorData(int,int)), m_pMainWindow, SLOT(slot_NewSensorData(int,int)));
    connect(m_pMainWindow, SIGNAL(sgn_Started_LineSensor()), m_pMobilePlatform, SIGNAL(sgn_Started_LineSensor()));
    connect(m_pMainWindow, SIGNAL(sgn_Finished_LineSensor()), m_pMobilePlatform, SIGNAL(sgn_Finished_LineSensor()));
    connect(m_pMainWindow, SIGNAL(sgn_Started_Encoder()), m_pMobilePlatform, SIGNAL(sgn_Started_Encoder()));
    connect(m_pMainWindow, SIGNAL(sgn_Finished_Encoder()), m_pMobilePlatform, SIGNAL(sgn_Finished_Encoder()));
    connect(m_pMainWindow, SIGNAL(sgn_Started_ColorSensor()), m_pMobilePlatform, SIGNAL(sgn_Started_ColorSensor()));
    connect(m_pMainWindow, SIGNAL(sgn_Finished_ColorSensor()), m_pMobilePlatform, SIGNAL(sgn_Finished_ColorSensor()));
    connect(m_pMainWindow, SIGNAL(sgn_Started_UltrasonicSensor()), m_pMobilePlatform, SIGNAL(sgn_Started_UltrasonicSensor()));
    connect(m_pMainWindow, SIGNAL(sgn_Finished_UltrasonicSensor()), m_pMobilePlatform, SIGNAL(sgn_Finished_UltrasonicSensor()));


    connect(m_pMainWindow, SIGNAL(sgn_pathToXML(QString)), m_pMobilePlatform, SIGNAL(sgn_pathToXML(QString)));
    connect(m_pMobilePlatform, SIGNAL(sgn_sendOrderlist(OrderList)), m_pMainWindow, SLOT(slot_displayJob(OrderList)));
    connect(m_pMobilePlatform,SIGNAL(sgn_sendCurrenteState(int,int)), m_pMainWindow, SLOT(slot_displayCurrentLogisticState(int,int)));
    connect(m_pMainWindow,SIGNAL(sgn_wareaufnehmen()),m_pMobilePlatform,SIGNAL(sgn_turnIntoRackGUITriggered()));
    connect(m_pMainWindow,SIGNAL(sgn_setDesiredColor(int)),m_pMobilePlatform,SIGNAL(sgn_setDesiredColorFromGUI(int)));
    connect(m_pMainWindow,SIGNAL(sgn_wareabliefern()),m_pMobilePlatform, SIGNAL(sgn_wareabliefern()));
    connect(m_pMobilePlatform,SIGNAL(sgn_stateMachineDone()),m_pMainWindow,SLOT(slot_stateMachineDone()));


    m_pMainWindow->show();
}

ControlSystem::~ControlSystem()
{
    delete m_pMainWindow;
    delete m_pMobilePlatform;

    std::cout << "Kill ControlSystem" << std::endl;
}
