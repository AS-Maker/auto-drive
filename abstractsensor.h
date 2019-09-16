//!
//! \file abstractsensor.h
//! \author Team-3
//! \date 7-Januar -2018

//! \brief Klasse, welche die Basisklasse fuer die Sensoren LineSensor, ColorSensor, Encoder und UltrasonicSensor darstellt.
//!

#ifndef ABSTRACTSENSOR_H
#define ABSTRACTSENSOR_H
#include <QObject>
#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QMutex>
#include <wiringPi.h>
#include <iostream>

class AbstractSensor : public QObject
{
    Q_OBJECT

public:
    //! Konstruktor
    //!
    //! Setzt die uebergebenen Pins.
    //!
    //! \param [in] nSensorID Pin fuer SensorID.
    //!
    //! \pre Wert der Pins liegt zwischen 0 und 29.
    //!
    //! \post Wenn Precondition erfuellt, wird der Pin gesetzt. Andernsfalls gibt die Methode false zurueck.
    AbstractSensor(int nSensorID);
    //!
    //! \brief ~AbstractSensor: Virtual Destruktor in einem AbstractClass Löscht das Abstrakte Objekt des Abstractsensors
    //!
    virtual ~AbstractSensor();
    //!
    //! \brief SetPin
    //! \param [in] nPinSensor des Sensors direkt (Hardware)
    //!
    //! \pre Wert des Pins liegt zwischen 0 und 29.
    //!
    //! \post Wenn Precondition erfuellt, wird der Pin gesetzt. Andernsfalls gibt die Methode false zurueck.
    void SetPin(int nPinSensor);


public slots:
    //! Virtual Funktion zum Starten des Prozesses fuer die Berechnung der Sensordaten.
    virtual void slot_StartProcess();
    //!
    //! \brief slot_Process: Pure Virtual Function weil sie auf Null gesetzt ist. Sie soll bei den ChildClassen Overriden werden
    //!
    virtual void slot_Process()=0;
    //! Virtual Funktion zum Beenden des Prozesses fuer die Berechnung der Sensordaten.
    virtual void slot_StopProcess();
    //! Virtual Funktion zum Anzeigen des Wertes.
    virtual void slot_ShowValue();

signals:
    //! \param [in] SensorID Sensoridentifikationsnummer, welche das Signal mit den Informationen dem richtiten Sensor zuordnet.
    //!
    //! \param [in] Sensordaten Beinhaltet die Sensordaten.
    //!
    //! \post Ermittelte Daten wurden an die Mobile Plattform gesendet.
    //! Funktion zum Senden der Sensor ID und der Daten.
    void sgn_NewSensorData(int nSensorID, int nSensordaten);




    //! Signal zum Beenden des objektspezifischen Threads.
    void sgn_Finished();

protected:

    //!
    //! \brief SensorID eines Sensors einer abgeleiteten Klasse
    //!
    int m_nSensorID;


    //!
    //! \brief An die GUI ausgebener Wert des Sensors einer abgeleiteten Klasse
    //!

    int m_nSensordaten;


    //!
    //! \brief Beinhaltet den Pin eines Sensors einer abgeleiteten Klasse
    //!
    int m_nPinSensor;


    //!
    //! \brief m_pTimerUpdate ist ein Timer der fuer all die abgeleitteten Sensorn benutzt wird
    //!
    QTimer* m_pTimerUpdate;

    QMutex m_Mutex;
};

#endif // ABSTRACTSENSOR_H
