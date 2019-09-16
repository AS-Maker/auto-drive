//!
//! \file ultrasonic.h
//! \author Team-3
//! \date 7-Januar -2018

//! \brief Klasse, in der die Distanz vom Boden zu einem Objekt in [cm] berechnet wird.
//!


#ifndef ULTRASONICSENSOR_H
#define ULTRASONICSENSOR_H
#include <abstractsensor.h>
#include <QElapsedTimer>

class UltrasonicSensor : public AbstractSensor
{

public:
    //! Konstruktor
    //!
    //! Setzt die uebergebenen Pins.
    //!
    //! \param [in] nSensorID Pin fuer SensorID.
    //! \param [in] nPinUltrasonicSensor Pin fuer Ultraschall-Sensor.
    //!
    //! \pre Wert des Pins liegt zwischen 0 und 29.
    UltrasonicSensor(int nSensorID, int nPinUltrasonicSensor);

    //! Destruktor
    //! Zerstoert das Objekt UltrasonicSensor.
    //!
    //! \post Alle in der Klasse erzeugten Objekte werden wieder geloescht.

    ~UltrasonicSensor();

    //! \pre Wert des Pins liegt zwischen 0 und 29.
    //!
    //! \post Wenn Precondition erfuellt, wird der Pin gesetzt. Andernsfalls gibt die Methode false zurueck.

    //! Methode prueft, ob der uebergebene Pin zwischen 0 und 29 liegt.

    bool bSetPin();




public slots:


    //! Diese Funktion wird vom Timer m_pTimerUpdate getriggert und berechnet den aktuell detektierten Abstand in [cm].
    void slot_Process();

    //! Diese Funktion stoppt die Funktion slot_Process.
    void slot_StopProcess();

    //! Gibt die Distanz von Ultraschall-Sensor bis Objekt in cm wieder
    float m_fdistanceCM;

private:

    //!Speichert den uebergebenen Pin des Sensors
    int m_nPinUltrasonicSensor;

    //! Gibt die Distanz von Ultraschall-Sensor bis Objekt in cm wieder
   // float m_fdistanceCM;

    //! Startzeitpunkt der Sensorinternen Signalverarbeitung
    unsigned int startTime;

    //! Endzeitpunkt der Sensorinternen Signalverarbeitung
    unsigned int endTime;

    //! Boolsche Variable zum Laufen/Beenden der Auslese-Routine
    bool m_bUltrasonicSensorRunning;





};

#endif // ULTRASONICSENSOR_H
