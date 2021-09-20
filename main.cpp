#include <QCoreApplication>
#include "service.h"


//для запуска ка консольное приложение запускать с параметром -e
int main(int argc, char *argv[])
{
    Service service(argc, argv);

    return service.exec();
}
