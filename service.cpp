#include <QFileInfo>
#include <QDir>
#include "service.h"
#include "ttasks.h"

Service::Service(int argc, char **argv)
    : QtService<QCoreApplication>(argc, argv, "Monitor Service")

{
    setServiceDescription("Monitor Service");
    setServiceFlags(QtServiceBase::CanBeSuspended);
}

Service::~Service()
{
}

void Service::start()
{
    try {
        QString ConfigFileName = application()->applicationDirPath() + "/SystemMonitor.ini";

        QDir::setCurrent(application()->applicationDirPath()); //меняем текущую директорию

        qDebug() << "Start service";
        qDebug() << "Reading configuration from :" + ConfigFileName;

        QFileInfo FileInfo(ConfigFileName);
        if (!FileInfo.exists()) {
            qDebug() << "Configuration file not found.";
            throw std::runtime_error("Configuration file not found.");
        }

        Config = new QSettings(ConfigFileName, QSettings::IniFormat);

        Tasks = new TTasks(*Config);



        Tasks->StartAll();

    }  catch (const std::exception &e) {
        qCritical() << "Critical error on start service. Message:" << e.what();
        exit(-1);
    }
}

void Service::pause()
{
    try {
        Tasks->StopAll();
    }  catch (const std::exception &e) {
        qCritical() << "Critical error on pause service. Message:" << e.what();
        exit(-1);
    }
}

void Service::resume()
{
    try {
         Tasks->StartAll();
    }  catch (const std::exception &e) {
        qCritical() << "Critical error on resume service. Message:" << e.what();
        exit(-1);
    }
}

void Service::stop()
{
    try {
        Tasks->StopAll();
        delete Tasks;
    }  catch (const std::exception &e) {
        qCritical() << "Critical error on stop service. Message:" << e.what();
        exit(-1);
    }
}



