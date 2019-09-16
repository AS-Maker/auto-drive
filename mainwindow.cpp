#include "mainwindow.h"
#include "ui_mainwindow.h"

#define nSensorIDLineSensorLeft 100
#define nSensorIDLineSensorRight 200
#define nSensorIDEncoderLeft 300
#define nSensorIDEncoderRight 400
#define nSensorIDUltrasonicSensor 500
#define nSensorIDColorSensor 600

#define nColorRed 1
#define nColorGreen 2
#define nColorBlue 3
#define nColorYellow 4
#define nColorWhite 5
#define nColorBlack 6
#define nColorUnknown 7

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->SpinBox_Speed, SIGNAL(valueChanged(int)), this, SIGNAL(sgn_SetSpeed(int)));
    connect(ui->PushButton_Forward, SIGNAL(clicked()), this, SIGNAL(sgn_Forward()));
    connect(ui->PushButton_Backward, SIGNAL(clicked()), this, SIGNAL(sgn_Backward()));
    connect(ui->PushButton_Left, SIGNAL(clicked()), this, SIGNAL(sgn_Left()));
    connect(ui->PushButton_Right, SIGNAL(clicked()), this, SIGNAL(sgn_Right()));
    connect(ui->PushButton_Stop, SIGNAL(clicked()), this, SIGNAL(sgn_Stop()));
    connect(ui->PushButton_FollowLine, SIGNAL(clicked()), this, SIGNAL(sgn_FollowLine()));
    connect(this, SIGNAL(sgn_Odometry()),this,SLOT(slot_ButtonOdometry()));
    connect(ui->btnStartStateMachine, SIGNAL(clicked()), this, SLOT(slot_startLogistic()));


    connect(ui->btnGetOrders, SIGNAL(clicked()), this, SLOT(slot_sendXMLPath()));
    connect(ui->pathToFile, SIGNAL(returnPressed()), this, SLOT(slot_sendXMLPath()));


    ui->checkBox_LineSensor->setChecked(true);
    ui->checkBox_Encoder->setChecked(true);
    ui->checkBox_ColorSensor->setChecked(true);
    ui->checkBox_UltrasonicSensor->setChecked(true);



}

MainWindow::~MainWindow()
{
    delete ui;

    std::cout << "Kill MainWindow" << std::endl;

}



//statemachine
void MainWindow::slot_sendXMLPath(){
    QString sPath=ui->pathToFile->text();
    emit sgn_pathToXML(sPath);
    ui->btnStartStateMachine->setEnabled(1);
}

void MainWindow::slot_displayJob(OrderList job){
    ui->OrderList_Lagerist->clear();
    ui->OrderList_Description->clear();
    ui->OrderList_ID->clear();
    ui->OrderList_Warehouse->clear();

    ui->btnStartStateMachine->setEnabled(1);

    this->m_orderList_job=job;
    ui->lblJobID->setText(m_orderList_job.id);
    ui->lblJobDescription->setText(m_orderList_job.description);

    for(int i=0; i<m_orderList_job.orders.length(); i++){
        ui->OrderList_ID->append(m_orderList_job.orders[i].id);
        ui->OrderList_Description->append(m_orderList_job.orders[i].description);

        switch (m_orderList_job.orders[i].storageRack){
        case Order::StorageRack::RACK_RED:
            ui->OrderList_Warehouse->append("Rot");
            break;
        case Order::StorageRack::RACK_GREEN:
            ui->OrderList_Warehouse->append("Grün");
            break;
        case Order::StorageRack::RACK_YELLOW:
            ui->OrderList_Warehouse->append("Gelb");
            break;
        case Order::StorageRack::UNKNOWN_RACK:
            ui->OrderList_Warehouse->append("invalid");
            ui->btnStartStateMachine->setEnabled(0);
            break;
        }

        switch (m_orderList_job.orders[i].Lagerist){
        case Order::WarehouseClerk::Lagerist_A:
            ui->OrderList_Lagerist->append("A");
            break;
        case Order::WarehouseClerk::Lagerist_B:
            ui->OrderList_Lagerist->append("B");
            break;
        case Order::WarehouseClerk::UNKNOWN_Lagerist:
            ui->OrderList_Lagerist->append("invalid");
            ui->btnStartStateMachine->setEnabled(0);
            break;
        }
    }
}

void MainWindow::slot_displayCurrentLogisticState(int currentState, int currentOrder){
    QString sState="";
    bool bAddColor=false;
    bool bAddLagerist=false;
    switch(currentState){
    case fahrezumlagerartikel:
        sState+="Fahre zum Lagerregal ";
        bAddColor=true;
        break;
    case wareaufnehmen:
        sState+="Hole Ware aus Lager ";
        bAddColor=true;
        break;
    case fahrezumlagerist:
        sState+="Fahre zum Lageristen ";
        bAddLagerist=true;
        break;
    case wareabliefern:
        sState+="Abgabe der Ware bei Lageristen ";
        bAddLagerist=true;
        break;
    case fahrezumstart:
        sState+="Auftrag beendet. Fährt zum Start-/Stopp Punkt.";
        break;
    case anfangsstartpunkt:
        sState+="Stehe an Start-/Stopp Punkt.";
        break;
    }
    if (bAddColor){
        switch (m_orderList_job.orders[currentOrder].storageRack){
        case Order::StorageRack::RACK_RED:
            sState+="Rot.";
            break;
        case Order::StorageRack::RACK_GREEN:
            sState+="Grün.";
            break;
        case Order::StorageRack::RACK_YELLOW:
            sState+="Gelb.";
            break;
        case Order::StorageRack::UNKNOWN_RACK:
            sState+="Ungültige Farbe.";
            break;
        }
    }
    if (bAddLagerist){
        switch (m_orderList_job.orders[currentOrder].Lagerist){
        case Order::WarehouseClerk::Lagerist_A:
            sState+="A.";
            break;
        case Order::WarehouseClerk::Lagerist_B:
            sState+="B.";
            break;
        case Order::WarehouseClerk::UNKNOWN_Lagerist:
            sState+="Ungültig.";
            break;
        }
    }
    ui->lblCurrentLogisticState->setText(sState);
}


void MainWindow::slot_stateMachineDone(){

    ui->PushButton_Backward->setEnabled(1);
    ui->PushButton_Forward->setEnabled(1);
    ui->PushButton_Stop->setEnabled(0);
    ui->PushButton_Left->setEnabled(1);
    ui->PushButton_Right->setEnabled(1);
    ui->pushButton_Odometrie->setEnabled(1);
    ui->btnGetOrders->setEnabled(1);
    ui->btnStartStateMachine->setEnabled(1);


    connect(ui->pathToFile, SIGNAL(returnPressed()), this, SLOT(slot_sendXMLPath()));




    ui->lblCurrentLogisticState->setText("");
    ui->lblJobDescription->setText("");
    ui->lblJobID->setText("");

    ui->OrderList_Lagerist->clear();
    ui->OrderList_Description->clear();
    ui->OrderList_ID->clear();
    ui->OrderList_Warehouse->clear();
}



void MainWindow::slot_startLogistic(){
    ui->PushButton_Backward->setEnabled(0);
    ui->PushButton_Forward->setEnabled(0);
    ui->PushButton_Stop->setEnabled(1);
    ui->PushButton_Left->setEnabled(0);
    ui->PushButton_Right->setEnabled(0);
    ui->pushButton_Odometrie->setEnabled(0);
    ui->btnGetOrders->setEnabled(0);
    ui->btnStartStateMachine->setEnabled(0);

    disconnect(ui->pathToFile, SIGNAL(returnPressed()), this, SLOT(slot_sendXMLPath()));




    emit sgn_startStateMachine();
}





void MainWindow::closeEvent(QCloseEvent *)
{
    emit sgn_Stop();
    std::cout << "CloseEvent" << std::endl;
}

void MainWindow::slot_ButtonOdometry()
{
    ui->pushButton_Odometrie->setEnabled(false);
    ui->PushButton_Forward->setEnabled(false);
    ui->PushButton_Backward->setEnabled(false);
    ui->PushButton_Left->setEnabled(false);
    ui->PushButton_Right->setEnabled(false);
    ui->PushButton_Stop->setEnabled(true);
    ui->PushButton_FollowLine->setEnabled(false);
}

void MainWindow::slot_ButtonForward()
{
    ui->PushButton_Forward->setEnabled(false);
    ui->PushButton_Backward->setEnabled(true);
    ui->PushButton_Left->setEnabled(true);
    ui->PushButton_Right->setEnabled(true);
    ui->PushButton_Stop->setEnabled(true);
    ui->PushButton_FollowLine->setEnabled(false);
}

void MainWindow::slot_ButtonBackward()
{
    ui->PushButton_Forward->setEnabled(true);
    ui->PushButton_Backward->setEnabled(false);
    ui->PushButton_Left->setEnabled(true);
    ui->PushButton_Right->setEnabled(true);
    ui->PushButton_Stop->setEnabled(true);
    ui->PushButton_FollowLine->setEnabled(false);
    \
}

void MainWindow::slot_ButtonLeft()
{
    ui->PushButton_Forward->setEnabled(true);
    ui->PushButton_Backward->setEnabled(true);
    ui->PushButton_Left->setEnabled(false);
    ui->PushButton_Right->setEnabled(true);
    ui->PushButton_Stop->setEnabled(true);
    ui->PushButton_FollowLine->setEnabled(false);

}

void MainWindow::slot_ButtonRight()
{
    ui->PushButton_Forward->setEnabled(true);
    ui->PushButton_Backward->setEnabled(true);
    ui->PushButton_Left->setEnabled(true);
    ui->PushButton_Right->setEnabled(false);
    ui->PushButton_Stop->setEnabled(true);
    ui->PushButton_FollowLine->setEnabled(false);

}

void MainWindow::slot_ButtonStop()
{
    ui->PushButton_Forward->setEnabled(true);
    ui->PushButton_Backward->setEnabled(true);
    ui->PushButton_Left->setEnabled(true);
    ui->PushButton_Right->setEnabled(true);

    ui->PushButton_FollowLine->setEnabled(true);
    ui->pushButton_Odometrie->setEnabled(true);
    ui->btnStartStateMachine->setEnabled(true);
    ui->btnGetOrders->setEnabled(true);

}

void MainWindow::slot_ButtonsFollowLine()
{
    ui->PushButton_Forward->setEnabled(false);
    ui->PushButton_Backward->setEnabled(false);
    ui->PushButton_Left->setEnabled(false);
    ui->PushButton_Right->setEnabled(false);
    ui->PushButton_Stop->setEnabled(true);
    ui->PushButton_FollowLine->setEnabled(false);

}

void MainWindow::slot_ShowButtonsOdometry()
{
    ui->PushButton_Forward->setEnabled(true);
    ui->PushButton_Backward->setEnabled(true);
    ui->PushButton_Left->setEnabled(true);
    ui->PushButton_Right->setEnabled(true);
    ui->PushButton_Stop->setEnabled(true);
    ui->PushButton_FollowLine->setEnabled(true);
    ui->pushButton_Odometrie->setEnabled(true);

}

void MainWindow::slot_NewSensorData(int nSensorID, int nSensordaten)
{

    if(nSensorID == nSensorIDLineSensorLeft)
    {
        if(nSensordaten == 0)
        {
            ui->Text_LineSensorLeft->setText("white");
            ui->Text_LineSensorLeft->setStyleSheet("QLineEdit {background-color: white;}");
        }
        else if(nSensordaten == 1)
        {
            ui->Text_LineSensorLeft->setText("black");
            ui->Text_LineSensorLeft->setStyleSheet("QLineEdit {background-color: black;}");
        }
        else
        {
            ui->Text_LineSensorRight->setText("Unkown");
            ui->Text_LineSensorLeft->setStyleSheet("QLineEdit {background-color: grey;}");
        }

        ui->Text_LineSensorLeft->setEnabled(false);
    }
    else if(nSensorID == nSensorIDLineSensorRight)
    {
        if(nSensordaten == 0)
        {
            ui->Text_LineSensorRight->setText("white");
            ui->Text_LineSensorRight->setStyleSheet("QLineEdit {background-color: white;}");
        }
        else if(nSensordaten == 1)
        {
            ui->Text_LineSensorRight->setText("black");
            ui->Text_LineSensorRight->setStyleSheet("QLineEdit {background-color: black;}");
        }
        else
        {
            ui->Text_LineSensorRight->setText("Unkown");
            ui->Text_LineSensorRight->setStyleSheet("QLineEdit {background-color: grey;}");
        }

        ui->Text_LineSensorRight->setEnabled(false);
    }
    else if(nSensorID == nSensorIDEncoderLeft)
    {
        ui->Text_EncoderLeft->setText(QString::number(nSensordaten));
        ui->Text_EncoderLeft->setEnabled(false);
    }
    else if(nSensorID == nSensorIDEncoderRight)
    {
        ui->Text_EncoderRight->setText(QString::number(nSensordaten));
        ui->Text_EncoderRight->setEnabled(false);
    }
    else if(nSensorID == nSensorIDUltrasonicSensor)
    {
        ui->Text_UltrasonicSensor->setText(QString::number(nSensordaten));
        ui->Text_UltrasonicSensor->setEnabled(false);
    }
    else if(nSensorID == nSensorIDColorSensor)
    {

        if(nSensordaten == nColorRed)
        {
            ui->Text_ColorSensor->setText("Red");
            ui->Text_ColorSensor->setStyleSheet("QLineEdit {background-color: red;}");
        }
        else if(nSensordaten == nColorGreen)
        {
            ui->Text_ColorSensor->setText("Green");
            ui->Text_ColorSensor->setStyleSheet("QLineEdit {background-color: green;}");
        }
        else if(nSensordaten == nColorBlue)
        {
            ui->Text_ColorSensor->setText("Blue");
            ui->Text_ColorSensor->setStyleSheet("QLineEdit {background-color: blue;}");
        }
        else if(nSensordaten == nColorYellow)
        {
            ui->Text_ColorSensor->setText("Yellow");
            ui->Text_ColorSensor->setStyleSheet("QLineEdit {background-color: yellow;}");
        }
        else if(nSensordaten == nColorWhite)
        {
            ui->Text_ColorSensor->setText("White");
            ui->Text_ColorSensor->setStyleSheet("QLineEdit {background-color: white;}");
        }
        else if(nSensordaten == nColorBlack)
        {
            ui->Text_ColorSensor->setText("Black");
            ui->Text_ColorSensor->setStyleSheet("QLineEdit {background-color: black;}");
        }
        else
        {
            ui->Text_ColorSensor->setText("Unknown");
            ui->Text_ColorSensor->setStyleSheet("QLineEdit {background-color: white;}");
        }

        ui->Text_ColorSensor->setEnabled(false);
    }
    else
    {
        std::cout<<"Unknown SensorID"<<std::endl;
    }

}




void MainWindow::on_checkBox_Encoder_toggled(bool checked)
{
    if(checked)
    {

        emit sgn_Started_Encoder();
    }
    else
    {
        emit sgn_Finished_Encoder();
    }
}


void MainWindow::on_checkBox_LineSensor_toggled(bool checked)
{
    if(checked)
    {
        emit sgn_Started_LineSensor();
    }
    else
    {
        emit sgn_Finished_LineSensor();
    }
}

void MainWindow::on_checkBox_ColorSensor_toggled(bool checked)
{
    if(checked)
    {
        emit sgn_Started_ColorSensor();
    }
    else
    {
        emit sgn_Finished_ColorSensor();
    }
}

void MainWindow::on_checkBox_UltrasonicSensor_toggled(bool checked)
{
    if(checked)
    {
        emit sgn_Started_UltrasonicSensor();
    }
    else
    {
        emit sgn_Finished_UltrasonicSensor();
    }
}

void MainWindow::on_pushButton_Odometrie_clicked()
{
    emit sgn_Odometry();

}


