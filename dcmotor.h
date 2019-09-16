//!
//! \file dcmotor.h
//! \author Team-3
//! \date 7-Januar -2018

//! \brief Klasse, in der die Motoransteuerung gemacht wird.
//!
//!  Als Uebergabeparamter dienen die Pins, die den Eingang für "1, oder High",
//!  "0, oder Low" und das PWM-Signal setzen. Zudem werden hier die Funktionen
//!  "Vorwaerts", "Rueckwaerts" und "Stop" gemacht.

#ifndef DCMOTOR_H
#define DCMOTOR_H
#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>


class DcMotor
{
public:
    //! Konstruktor
    //!
    //! Setzt die uebergebenen Pins.
    //!
    //! \param [in] nPinMotorA Pin fuer die Drehrichtung des Motors.
    //! \param [in] nPinMotorB Pin fuer die Drehrichtung des Motors.
    //! \param [in] nPinMotorE Pin fuer das PWM-Signal.
    //!
    //! \pre Wert des Pins liegt zwischen 0 und 29.
    DcMotor(int nPinMotorA, int nPinMotorB, int nPinMotorE);

    //! Destruktor
    //!
    //! Loescht das Objekt DcMotor.
    ~DcMotor();

    //! \pre Wert der Pins liegt zwischen 0 und 29.
    //!
    //! \post Wenn Precondition erfuellt, werden die Pins gesetzt. Andernsfalls gibt die Methode false zurueck.

    //! Pruefung der Pins. Methode prueft ob die Werte fuer die Pins im erlaubten Bereich liegen.
    void SetPins();

    //! Dreht den Motor vorwaerts.
    void Forward();

    //! Dreht den Motor rueckwaerts.
    void Backward();

    //! Stoppt den Motor.
    void StopMotor();

    //! \param [in] nPWM Uebergebener PWM-Wert.
    //!
    //! \post Uebergebener PWM-Wert wurde gesetzt.

    //! PWM-Wert ermitteln. Die Methode setzt das PWM-Signal auf den int-Wert der uebergeben wurde.
    void SetPWM(int nPWM);

private:
    //!
    //! \brief m_nPinMotorA Beinhaltet uebergebenen High-Pin
    //!
    int m_nPinMotorA;

    //!
    //! \brief m_nPinMotorB Beinhaltet uebergebenen Low-Pin
    //!
    int m_nPinMotorB;

    //!
    //! \brief m_nPinMotorE Beinhaltet uebergebenen Pin fuer das PWM-Signal
    //!
    int m_nPinMotorE;

    //!
    //! \brief m_nPWM Beinhaltet das angelegte PWM-Signal
    //!
    int m_nPWM;
};

#endif // DCMOTOR_H
