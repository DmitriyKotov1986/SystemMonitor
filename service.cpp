#include "service.h"
#include "tsysteminfo.h"

Service::Service(int argc, char **argv)
    : QtService<QCoreApplication>(argc, argv, "")
{
    try {
        setServiceDescription("");
        setServiceFlags(QtServiceBase::CanBeSuspended);

        qDebug() << "Constraction service";

     }  catch (...) {
        qCritical() << "Criticall error on costruction service";
    }
}

Service::~Service()
{
    try {
        qDebug() << "Deconstraction service";

    }  catch (...) {
        qCritical() << "Critical error on decostruction service";
    }
}

void Service::start()
{
    try {
        qDebug() << "Start service";

        TSystemInfo SystemInfo("SystemInfoConfig.ini");

        SystemInfo.Print();
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
    }  catch (...) {
        qCritical() << "Critical error on stop service";
    }
}


