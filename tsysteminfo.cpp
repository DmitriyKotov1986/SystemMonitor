#include "tsysteminfo.h"
#include <QProcess>
#include <QtDebug>
#include <QSettings>
#include <QTextStream>

QString TSystemInfo::NextKey(const QString & OldKey) { //генерирует имя для нового ключа
    if (!CurrentNumberKey.contains(OldKey)) {
        CurrentNumberKey.insert(OldKey, 0);
        return OldKey + "/0";
    }
    else {
       ++CurrentNumberKey[OldKey];
       return OldKey + "/" + QString::number(CurrentNumberKey[OldKey]);
    }
}

void TSystemInfo::Parser(const QString &Group, const QByteArray &str)
{
    //QString s(str);
    //qDebug() << str;
    QTextStream TS(str);

    while (!TS.atEnd()) {
      QString tmp = TS.readLine();
   //   qDebug() << tmp;
      if (tmp != "\r") {
        size_t position = tmp.indexOf('=');
        QString Key = tmp.left(position);
        QString Value = tmp.right(tmp.length() - position - 1);
        Value.chop(1);
        Key = NextKey(Group + "/" + Key);
        Info.insert(Key, Value);
        //if (!Info.contains(Key)) Info.insert(Key, Value); //если такой ключ попался впервые
        //else { //если такой ключ уже был
        //   Info.insert(NextKey(Key), Value);
        //   Info.insert(Key + "/0", Info[Key]); //переименовываем первый записанный ключ
        //   Info.erase(Info.find(Key));
        //}
      }
    }
}

TSystemInfo::TSystemInfo(const QString& FileName)
{
    qDebug() << "Start get information about system. File:" <<  FileName;

    QSettings ini(FileName, QSettings::IniFormat);

    //получаем путь к файлу с командой wnim
    ini.beginGroup("SYSTEM");
    QString WMICFileName = ini.value("wmic", "").toString();
    if (WMICFileName == "") ;
    uint32_t GroupListCount = ini.value("GroupCount", 0).toUInt();
    QStringList GroupList;
    for (uint32_t i = 0; i < GroupListCount; ++i) GroupList << ini.value("Group" + QString::number(i), "").toString();
    ini.endGroup();

    //qDebug() << GroupList;

    for (auto Item : GroupList) {
        ini.beginGroup(Item);
        uint32_t Count = ini.value("Count", 0).toUInt();
        QStringList Keys(Item);
        Keys << "GET";
        for (uint32_t i = 0; i < Count; ++i) {
            Keys << ini.value("Key" + QString::number(i), "").toString();
            if (i != Count - 1) Keys << ",";
        }
        ini.endGroup();
        Keys << "/FORMAT:LIST";

        //выполняем команду
        QProcess *cmd = new QProcess();
        cmd->start(WMICFileName ,Keys);
        cmd->waitForFinished(10000);
        //qDebug() << cmd->readAllStandardOutput();
        Parser(Item, cmd->readAllStandardOutput());
        delete cmd;
    }
  //  qDebug() << Result;

}

TSystemInfo::~TSystemInfo()
{

}

void TSystemInfo::Print()
{
    for (const auto & Item: Info.toStdMap()) {
        qDebug() << Item.first << Item.second;
    }
}
