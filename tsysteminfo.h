#ifndef TSYSTEMINFO_H
#define TSYSTEMINFO_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QByteArray>

class TSystemInfo
{
private:
    QMap <QString, QString> Info;
    QMap <QString, uint32_t> CurrentNumberKey;
    void Parser(const QString &Group, const QByteArray &str);
    QString NextKey(const QString & OldKey);
public:
    TSystemInfo(const QString& FileName);
    ~TSystemInfo();

    void Print();
};

#endif // TSYSTEMINFO_H
