#ifndef SERVICE_H
#define SERVICE_H

#include <QtService/QtService>
#include <QString>

class Service : public QtService<QCoreApplication>
{
public:
   Service(int argc, char **argv);
   ~Service();

protected:
    void start(); //Запус сервиса
    void pause(); //Установка сервиса на Паузу
    void resume(); //Востановление сервиса после паузы
    void stop(); //Остановка сервиса
private:

};


#endif // SERVICE_H
