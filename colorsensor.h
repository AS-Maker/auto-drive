//!
//! \file colorsensor.h
//! \author Team-3
//! \date 7-Januar -2018

//! \brief Klasse, in der die Differenzierung verschiedener Farben gemacht wird.
//!
//!  Der Sensor kann zwischen den Farben "Blau", "Rot", "Gruen",
//!  "Gelb", "Weiß" und "Schwarz" unterscheiden. Dies wird gemacht, dass für
//!  jede Farbe spezielle Farbverhaeltnisse aus dem RGB-Bereich definiert werden.


#ifndef COLORSENSOR_H
#define COLORSENSOR_H
#include <abstractsensor.h>
#include <wiringPiI2C.h>

#define nColorRed 1
#define nColorGreen 2
#define nColorBlue 3
#define nColorYellow 4
#define nColorWhite 5
#define nColorBlack 6
#define nColorUnknown 7

class ColorSensor : public AbstractSensor
{
public:

    //! Konstruktor
    //!
    //! Setzt die uebergebenen Pins.
    //!
    //! \param [in] nSensorID Pin fuer SensorID.
    //!
    //! \pre Wert des Pins liegt zwischen 0 und 29.

    ColorSensor(int nSensorID);

    //! Destruktor
    //!
    //! Zerstoert das Objekt ColorSensor.
    //!
    //! \post Alle in der Klasse erzeugten Objekte werden wieder geloescht.

    ~ColorSensor();


    //!
    //! \brief m_nColor ist die Farbe des detektierten Bild
    //!
    int m_nColor;

public slots:


    //!
    //! \brief slot_StartProcess() wird vom Timer m_pTimerUpdate getriggert und berechnet die aktuell detektierte Farbe.
    //!
    void slot_Process();

    //!
    //! \brief slot_StopProcess stoppt die Funktion slot_Process.
    //!

    void slot_StopProcess();

private:

    float red;
    float green;
    float blue;
    float brightness;

    //!
    //! \brief m_iDeviceHandleId Speichert den uebergebenen Pin des Sensors
    //!
    int m_iDeviceHandleId;

    //!
    //! \brief bColorSensorRunning Boolsche Variable zum Laufen/Beenden der Auslese-Routine
    //!
    bool bColorSensorRunning;


};

#endif // COLORSENSOR_H
