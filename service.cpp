#include "service.h"
#include "tsysteminfo.h"

Service::Service(int argc, char **argv)
    : QtService<QCoreApplication>(argc, argv, "Monitor Service")
{
    try {
        setServiceDescription("Monitor Service");
        setServiceFlags(QtServiceBase::CanBeSuspended);

    }  catch (...) {
        qCritical() << "Criticall error on costruction service";
    };
}

Service::~Service()
{
    try {
        ;
     }  catch (...) {
        qCritical() << "Critical error on decostruction service";
    }
}

void Service::start()
{
    try {
        qDebug() << "Start service";

        Config = new QSettings(application()->applicationDirPath() + "/MonitorConfig.ini", QSettings::IniFormat);

     //   SystemInfo = new TSystemInfo(Config);


    //    SystemInfo->Print();

    }  catch (...) {
        qCritical() << "Critical error on start service";
    }

}

void Service::pause()
{
    try {
        qDebug() << "Pause service";
    }  catch (...) {
        qCritical() << "Critical error on pause service";
    }
}

void Service::resume()
{
    try {
         qDebug() << "Resume service";
    }  catch (...) {
        qCritical() << "Critical error on resume service";
    }
}

void Service::stop()
{
    try {
        qDebug() << "Stop service";

        delete SystemInfo;

    }  catch (...) {
        qCritical() << "Critical error on stop service";
    }
}


