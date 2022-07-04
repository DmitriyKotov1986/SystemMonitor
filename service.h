#ifndef SERVICE_H
#define SERVICE_H

#include <QtService/QtService>
#include <QString>
#include <QSettings>
#include "ttasks.h"

class Service : public QtService<QCoreApplication>
{
private:
    TTasks *Tasks;

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

};


#endif // SERVICE_H
