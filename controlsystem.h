//!
//! \file controlsystem.h
//! \author Team-3
//! \date 7-Januar -2018


//!
//!  Hier wird eine Verknuepfung von MainWindow (Viewer) und MobilePlatform (Model)
//!  erstellt.
//!
#ifndef CONTROLSYSTEM_H
#define CONTROLSYSTEM_H
#include <mainwindow.h>
#include <mobileplatform.h>
#include <QObject>


class ControlSystem : public QObject
{
    Q_OBJECT

public:
    //! Konstruktor
    //!
    //! Erstellt ein Objekt ControlSystem. Des Weiteren werden hier
    //! die Objekte MobilePlatform und MainWindow erstellt und die Verbindung
    //! zwischen Model und View festgelegt.
    explicit ControlSystem(QObject *parent = 0);

    //! Destruktor
    //!
    //! Loescht das Objekt ControlSystem.
    //!
    //! \post Zudem werden die Objekte des Models und des Views zerstoert
    ~ControlSystem();

private:
    //!
    //! \brief m_pMainWindow Erstellt ein Objekt der MainWindow
    //!
    MainWindow *m_pMainWindow;

    //!
    //! \brief m_pMobilePlatform Erstellt ein Objekt der MobilePlatform
    //!
    MobilePlatform *m_pMobilePlatform;

};

#endif // CONTROLSYSTEM_H
