#include <QSqlError>
#include <QSqlQuery>
#include <QRegularExpression>
#include <QDateTime>
#include <QFile>
#include <QDir>
#include "ttasks.h"
#include "ttasks.h"

QString TaskStatusToStr (TTaskStatus TaskStatus) {
    switch (TaskStatus) {
    case TTaskStatus::TASK_STOP     : return "STOP";
    case TTaskStatus::TASK_ERROR    : return "ERROR";
    case TTaskStatus::TASK_RUN      : return "RUN";
    case TTaskStatus::TASK_STARTED  : return "STARTED";
    case TTaskStatus::TASK_FINISHED : return "FINISHED";
    case TTaskStatus::TASK_NOANSWER : return "NOANSWER";
    }
    return "";
}

TTasks::TTasks(QSettings & Config, QObject *parent)
    : QObject(parent)
    , Config(Config)
{
    Config.beginGroup("DATABASE");

    DB = QSqlDatabase::addDatabase(Config.value("Driver", "QODBC").toString(), "MainDB");
    DB.setDatabaseName(Config.value("DataBase", "SystemMonitorDB").toString());
    DB.setUserName(Config.value("UID", "SYSDBA").toString());
    DB.setPassword(Config.value("PWD", "MASTERKEY").toString());
    DB.setConnectOptions(Config.value("ConnectionOprions", "").toString());
    DB.setPort(Config.value("Port", "3051").toUInt());
    DB.setHostName(Config.value("Host", "localhost").toString());
    Config.endGroup();

    if (!DB.open()) {
        qCritical() << "Cannot connect to database. Error: " << DB.lastError().text();
    /*    Config.beginGroup("DATABASE");
        QFile File("c:/tmp/1.log");
        File.open(QIODevice::ReadWrite);
        File.write("Cannot connect to database. Error: " + DB.lastError().text().toUtf8());
        File.write(Config.value("DataBase", "123").toString().toUtf8());
        File.write(Config.value("Driver", "456").toString().toUtf8());
        File.close();
        Config.endGroup();*/
        exit(-200);
    };

    //запускаем задачи
    Config.beginGroup("TASKS");
    uint32_t TasksCount = Config.value("TasksCount", 0).toUInt();
    Config.endGroup();
    Tasks.resize(TasksCount);
    for (uint32_t i = 0; i < TasksCount; ++i) {
        AddTask(i);
    }

    SendLogMsg(MSG_CODE::CODE_OK, "Successfully started");

    ClearLog();
}

TTasks::~TTasks()
{
    for (auto & Item : Tasks) {
        Item->deleteLater();
    }

    SendLogMsg(MSG_CODE::CODE_OK, "Successfully finished");
    DB.close();
}

void TTasks::StartAll()
{
   qDebug() << "Start all";
   for (auto & Item : Tasks) Item->Start();
}

void TTasks::StopAll()
{
    qDebug() << "Stop all";
    for (auto & Item : Tasks) Item->Stop();
}

void TTasks::TaskChangeStatus(const uint32_t TaskNumber, TTaskStatus Status, const QString &LastError)
{
    QString TaskName = Tasks[TaskNumber]->GetConfig().Name;
    SendLogMsg(MSG_CODE::CODE_INFORMATION, "Task "+ TaskName + " change status on " + TaskStatusToStr(Status) +
                                           " Msg: " + LastError);
        //перезапускаем процесс если это указано в настройках
    if ((Status == TTaskStatus::TASK_ERROR) ||
        (Status == TTaskStatus::TASK_FINISHED) ||
        (Status == TTaskStatus::TASK_NOANSWER)) {

        uint32_t TryReload = Tasks[TaskNumber]->GetConfig().TryReload;
        Tasks[TaskNumber]->Stop();

        Tasks[TaskNumber]->deleteLater();

        //qDebug() << "try reload" << TryReload;
        if (TryReload) {
            AddTask(TaskNumber);
            Tasks[TaskNumber]->Start();
        }
    }
 }

void TTasks::SendLogMsg(uint16_t Category, const QString &Msg)
{
    QString Str(Msg);
    //Str.replace(QRegularExpression("'"), "''");
    //Str="";
    qDebug() << Str;
    QSqlQuery QueryLog(DB);
    DB.transaction();
    QString QueryText = "INSERT INTO LOG (CATEGORY, SENDER, MSG) VALUES ( "
                        + QString::number(Category) + ", "
                        "\'SystemMonitor\', "
                        "\'" + Str +"\'"
                        ")";

    if (!QueryLog.exec(QueryText)) {
        qDebug() << "FAIL Cannot execute query. Error: " << QueryLog.lastError().text() << " Query: "<< QueryLog.lastQuery();
        DB.rollback();
        exit(-1);
    }
    if (!DB.commit()) {
        qDebug() << "FAIL Cannot commit transation. Error: " << DB.lastError().text();
        DB.rollback();
        exit(-2);
    };
}

void TTasks::AddTask(uint32_t TaskNumber)
{
    TTask *Task = new TTask(Config, TaskNumber, this);
    QObject::connect(Task, SIGNAL(TaskChangeStatus(const uint32_t, TTaskStatus, const QString &)), this, SLOT(TaskChangeStatus(const uint32_t, TTaskStatus, const QString &)));
    Tasks[TaskNumber] = Task;
}

void TTasks::ClearLog()
{
    QSqlQuery QueryLog(DB);
    DB.transaction();
    QDateTime CurrentDateTime = QDateTime::currentDateTime();
    QString QueryText = "DELETE FROM LOG "
                        "WHERE DATE_TIME < '" + CurrentDateTime.addMonths(-1).toString("yyyy-MM-dd hh:mm:ss.zzz") + "'" ;

    if (!QueryLog.exec(QueryText)) {
        qDebug() << "FAIL Cannot execute query. Error: " << QueryLog.lastError().text() << " Query: "<< QueryLog.lastQuery();
        DB.rollback();
        exit(-1);
    }
    if (!DB.commit()) {
        qDebug() << "FAIL Cannot commit transation. Error: " << DB.lastError().text();
        DB.rollback();
        exit(-2);
    };
}
