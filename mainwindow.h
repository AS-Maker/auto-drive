//!
//! \file mainwindow.h
//! \author Team-3
//! \brief Graphical User Interface
//!
//! Erstellt die grafische Oberfläche zum Steuern des Roboters
//!

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QLineEdit>
#include <QString>
#include <QDebug>
#include <iostream>
#include <QFileDialog>
#include "xmlreader.h"
#include "statemachine.h"

namespace Ui
{
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    //! \brief Konstruktor
    //!
    //! Erstellt ein Objekt der Klasse QMainWindow.
    //! Verknüpft die Signale der GUI mit den Signalen dieses Objekts,
    //!

    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

signals:

    void sgn_Forward();

    void sgn_Backward();

    void sgn_Left();

    void sgn_Right();

    void sgn_Stop();


    void sgn_FollowLine();

    void sgn_Odometry();

    void sgn_SetSpeed(int nSpeed);

    void sgn_Started_LineSensor();

    void sgn_Finished_LineSensor();

    void sgn_Started_Encoder();

    void sgn_Finished_Encoder();

    void sgn_Started_ColorSensor();

    void sgn_Finished_ColorSensor();


    void sgn_Started_UltrasonicSensor();

    void sgn_Finished_UltrasonicSensor();

    //statemachine

    void sgn_startStateMachine();

    //Sonstige
    void sgn_speedChanged(double);

    void sgn_PIDChanged(int ID, int value);

    void sgn_pathToXML(QString);

    void sgn_wareaufnehmen();
    void sgn_wareabliefern();
    void sgn_setDesiredColor(int);


public slots:

    void slot_ButtonForward();


    void slot_ButtonBackward();


    void slot_ButtonLeft();


    void slot_ButtonRight();


    void slot_ButtonStop();



    void slot_sendXMLPath();

    //! \brief Darstellung des Jobs auf der GUI
    //!
    //! Zeigt den Job mit den einzelnen Aufträgen an.
    //!
    //! \OrderList [in] job Beinhaltet ausgelesenen Job mit all seinen Aufträgen und Daten
    void slot_displayJob(OrderList job);

    void slot_displayCurrentLogisticState(int currentState, int currentOrder);

    void slot_stateMachineDone();

    void slot_startLogistic();







    void slot_ButtonsFollowLine();


    void slot_ButtonOdometry();

    //! Methode zum Einblenden der GUI-Elemente bei Odometrie.
    void slot_ShowButtonsOdometry();




    void slot_NewSensorData(int nSensorID, int nSensordaten);

    void closeEvent(QCloseEvent *);

private slots:

    void on_checkBox_Encoder_toggled(bool);


    void on_checkBox_LineSensor_toggled(bool);
    void on_checkBox_ColorSensor_toggled(bool);

    void on_checkBox_UltrasonicSensor_toggled(bool);

    void on_pushButton_Odometrie_clicked();

private:
    Ui::MainWindow *ui;
    OrderList m_orderList_job;
    StateMachine *m_StateMaschine;
};

#endif // MAINWINDOW_H
