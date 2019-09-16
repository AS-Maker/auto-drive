#include "controlsystem.h"
#include "QApplication"
#include "iostream"
#include <wiringPi.h>

int main(int argc, char *argv[])
{
    wiringPiSetup();

    QApplication a(argc, argv);


    ControlSystem c;
    int retVal = a.exec();

    return retVal;
}
