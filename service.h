#ifndef SERVICE_H
#define SERVICE_H

#include <QtService/QtService>
#include <QString>
#include "tsysteminfo.h"
#include <QSettings>

class Service : public QtService<QCoreApplication>
{
public:
   Service(int argc, char **argv);
   ~Service();

protected:
    void start(); //Запус сервиса
    void pause(); //Установка сервиса на паузу
    void resume(); //Востановление сервиса после паузы
    void stop(); //Остановка сервиса
private:
    QSettings *Config; //Файл конфигурации службы
    TSystemInfo *SystemInfo;
};


#endif // SERVICE_H
