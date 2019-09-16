#include "dcmotor.h"


DcMotor::DcMotor(int nPinMotorA, int nPinMotorB, int nPinMotorE)
{
    m_nPinMotorA = nPinMotorA;
    m_nPinMotorB = nPinMotorB;
    m_nPinMotorE = nPinMotorE;
    SetPins();
}

DcMotor::~DcMotor()
{
    StopMotor();
    std::cout << "Kill DcMotor" << std::endl;
}

void DcMotor::SetPins()
{
    if((0 <= m_nPinMotorA && m_nPinMotorA <= 29) && (0 <= m_nPinMotorB && m_nPinMotorB <= 29) && (0 <= m_nPinMotorE && m_nPinMotorE <= 29))
    {
        pinMode(m_nPinMotorA, OUTPUT);
        pinMode(m_nPinMotorB, OUTPUT);
        pinMode(m_nPinMotorE, PWM_OUTPUT);
        softPwmCreate(m_nPinMotorE,0,255);
    }
    else
    {
        std::cout << "wrong pin number" << std::endl;
    }
}

void DcMotor::Forward()
{

    digitalWrite(m_nPinMotorA, 1);
    digitalWrite(m_nPinMotorB, 0);
}

void DcMotor::Backward()
{
    digitalWrite(m_nPinMotorA, 0);
    digitalWrite(m_nPinMotorB, 1);
}

void DcMotor::StopMotor()
{
    digitalWrite(m_nPinMotorA, LOW);
    digitalWrite(m_nPinMotorB, LOW);
}

void DcMotor::SetPWM(int nPWM)
{
    m_nPWM = nPWM;

    if(0 <= m_nPWM && m_nPWM <= 255)
    {
        nPWM = m_nPWM;
    }
    else
    {
        nPWM = 255;
    }

    softPwmWrite(m_nPinMotorE, nPWM);
}
