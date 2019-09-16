#include "drivecontrol.h"

drivecontrol::drivecontrol(DcMotor &dcMotorLeft, DcMotor &dcMotorRight, Encoder &EncoderLeft, Encoder &EncoderRight)
{

    m_pDcMotorLeft= &dcMotorLeft;
    m_pDcEncoderLeft= &EncoderLeft;

    m_pDcMotorRight= &dcMotorRight;
    m_pDcEncoderRight= &EncoderRight;
    m_pTimerUpdateSpeed = new QTimer();
    m_pTimerUpdateSpeed->setInterval(dTimeIntervallMotion*1000);
    m_pTimerUpdateSpeed->setSingleShot(false);
    connect(m_pTimerUpdateSpeed, SIGNAL(timeout()), this, SLOT(slot_SpeedControl()));

    m_nCounterTicsLeft = 0;
    m_nCounterTicsLeft_old = 0;
    m_nCounterTicsLeft_Dif = 0;
    m_nCounterTicsRight = 0;
    m_nCounterTicsRight_old = 0;
    m_nCounterTicsRight_Dif = 0;
    m_dSpeedMotorLeft = 0;
    m_dSpeedMotorLeft_soll = 0;
    m_dSpeedMotorRight = 0;
    m_dSpeedMotorRight_soll = 0;

    m_nPWMMotorLeft = 0;
    m_nPWMMotorRight = 0;

    m_dDistanceLeft = 0;
    Difference_Speed_Left_alt = 0;
    Difference_Speed_Left = 0;
    Sum_Difference_Speed_Left = 0;
    m_dDistanceRight = 0;
    Difference_Speed_Right_alt = 0;
    Difference_Speed_Right = 0;
    Sum_Difference_Speed_Right = 0;
}

void drivecontrol::slot_SpeedControl()
{

    m_nCounterTicsLeft_old = m_nCounterTicsLeft;
    m_nCounterTicsLeft = qAbs(m_pDcEncoderLeft->nGetCounterTics());
    m_nCounterTicsLeft_Dif = m_nCounterTicsLeft - m_nCounterTicsLeft_old;
    m_dDistanceLeft = double((m_nCounterTicsLeft*dDistancePerWheelRotation)/nCompleteScopeTics);
    m_dSpeedMotorLeft = double((m_nCounterTicsLeft_Dif*dDistancePerWheelRotation)/(nCompleteScopeTics*dTimeIntervallMotion));
    Difference_Speed_Left_alt = Difference_Speed_Left;
    Difference_Speed_Left = m_dSpeedMotorLeft_soll - m_dSpeedMotorLeft;
    Sum_Difference_Speed_Left = Sum_Difference_Speed_Left + Difference_Speed_Left;

    double P_Anteil_MotorLeft_Motion = Difference_Speed_Left * m_dKp_MotorLeft_Motion;
    double I_Anteil_MotorLeft_Motion = Sum_Difference_Speed_Left * dTimeIntervallMotion * m_dKi_MotorLeft_Motion;
    double D_Anteil_MotorLeft_Motion = ((Difference_Speed_Left - Difference_Speed_Left_alt) / dTimeIntervallMotion) * m_dKd_MotorLeft_Motion;

    m_nPWMMotorLeft = int(P_Anteil_MotorLeft_Motion + I_Anteil_MotorLeft_Motion + D_Anteil_MotorLeft_Motion);


    int leftVal = m_nPWMMotorLeft;
    if(leftVal > m_nPWM_max)
    {
        leftVal = m_nPWM_max;
    }
    else if(leftVal < 0)
    {
        leftVal = 0;
    }
    m_pDcMotorLeft->SetPWM(leftVal);

    m_nCounterTicsRight_old = m_nCounterTicsRight;
    m_nCounterTicsRight = qAbs(m_pDcEncoderRight->nGetCounterTics());
    m_nCounterTicsRight_Dif = m_nCounterTicsRight - m_nCounterTicsRight_old;
    m_dDistanceRight = double((m_nCounterTicsRight*dDistancePerWheelRotation)/nCompleteScopeTics);
    m_dSpeedMotorRight = double((m_nCounterTicsRight_Dif*dDistancePerWheelRotation)/(nCompleteScopeTics*dTimeIntervallMotion));
    Difference_Speed_Right_alt = Difference_Speed_Right;
    Difference_Speed_Right = m_dSpeedMotorRight_soll - m_dSpeedMotorRight;
    Sum_Difference_Speed_Right = Sum_Difference_Speed_Right + Difference_Speed_Right;

    double P_Anteil_MotorRight_Motion = Difference_Speed_Right * m_dKp_MotorRight_Motion;
    double I_Anteil_MotorRight_Motion = Sum_Difference_Speed_Right * dTimeIntervallMotion * m_dKi_MotorRight_Motion;
    double D_Anteil_MotorRight_Motion = ((Difference_Speed_Right - Difference_Speed_Right_alt) / dTimeIntervallMotion) * m_dKd_MotorRight_Motion;

    m_nPWMMotorRight = int(P_Anteil_MotorRight_Motion + I_Anteil_MotorRight_Motion + D_Anteil_MotorRight_Motion);

    int rightVal = m_nPWMMotorRight;

    if(rightVal > m_nPWM_max)
    {
        rightVal = m_nPWM_max;
    }
    else if(rightVal < 0)
    {
        rightVal = 0;
    }
    m_pDcMotorRight->SetPWM(rightVal);
}

void drivecontrol::DriveLine(double dDistance)
{
    m_nCounterTicsLeft =m_pDcEncoderLeft->nResetCounterTics();
    m_nCounterTicsRight =m_pDcEncoderRight->nResetCounterTics();

    m_dSpeedMotorRight_soll = 0.12;
    m_dSpeedMotorLeft_soll = 0.12;
    m_nCounterTicsLeft_old = 0;
    m_nCounterTicsLeft = 0;
    m_nCounterTicsLeft_Dif = 0;
    m_dDistanceLeft = 0;
    m_dSpeedMotorLeft = 0;
    Difference_Speed_Left_alt = 0;
    Difference_Speed_Left = 0;
    Sum_Difference_Speed_Left = 0;

    m_nCounterTicsRight_old = 0;
    m_nCounterTicsRight = 0;
    m_nCounterTicsRight_Dif = 0;
    m_dDistanceRight = 0;
    m_dSpeedMotorRight = 0;
    Difference_Speed_Right_alt = 0;
    Difference_Speed_Right = 0;
    Sum_Difference_Speed_Right = 0;

    m_dDistanceLeft_soll = dDistance;
    m_dDistanceRight_soll = dDistance;

    m_pTimerUpdateSpeed->start();

    while(((m_dDistanceLeft < m_dDistanceLeft_soll) || (m_dDistanceRight < m_dDistanceRight_soll)) && (m_bOdometryIsRunning == true))
    {
        m_pDcMotorLeft->Forward();
        m_pDcMotorRight->Forward();
        QCoreApplication::processEvents();
    }

    m_pTimerUpdateSpeed->stop();

    m_pDcMotorLeft->SetPWM(0);
    m_pDcMotorRight->SetPWM(0);
}

void drivecontrol::DriveAngle(int nAngle)
{  
    m_nCounterTicsLeft = m_pDcEncoderLeft->nResetCounterTics();
    m_nCounterTicsRight = m_pDcEncoderRight->nResetCounterTics();

    m_dSpeedMotorRight_soll = 0.12;
    m_dSpeedMotorLeft_soll = 0.12;
    m_nCounterTicsLeft_old = 0;
    m_nCounterTicsLeft = 0;
    m_nCounterTicsLeft_Dif = 0;
    m_dDistanceLeft = 0;
    m_dSpeedMotorLeft = 0;
    Difference_Speed_Left_alt = 0;
    Difference_Speed_Left = 0;
    Sum_Difference_Speed_Left = 0;

    m_nCounterTicsRight_old = 0;
    m_nCounterTicsRight = 0;
    m_nCounterTicsRight_Dif = 0;
    m_dDistanceRight = 0;
    m_dSpeedMotorRight = 0;
    Difference_Speed_Right_alt = 0;
    Difference_Speed_Right = 0;
    Sum_Difference_Speed_Right = 0;

    m_dDistanceLeft_soll = double((Pi*dCentreDistance*qAbs(nAngle))/nCompleteScopeGradiant);
    m_dDistanceRight_soll = double((Pi*dCentreDistance*qAbs(nAngle))/nCompleteScopeGradiant);


    m_pTimerUpdateSpeed->start();

    while(((m_dDistanceLeft < m_dDistanceLeft_soll) || (m_dDistanceRight < m_dDistanceRight_soll)) && (m_bOdometryIsRunning == true))
    {
        if(nAngle > 0)
        {
            m_pDcMotorLeft->Forward();
            m_pDcMotorRight->Backward();
        }
        else
        {
            m_pDcMotorLeft->Backward();
            m_pDcMotorRight->Forward();
        }
        QCoreApplication::processEvents();
    }

    m_pTimerUpdateSpeed->stop();

    m_pDcMotorLeft->SetPWM(0);
    m_pDcMotorRight->SetPWM(0);
}

drivecontrol::~drivecontrol()
{
    emit sgn_Finished();
    m_Mutex.lock();
    m_bOdometryIsRunning = false;
    m_Mutex.unlock();
    m_pTimerUpdateSpeed->stop();
    delete m_pTimerUpdateSpeed;
    std::cout << "Kill drivecontrol" << std::endl;
}
