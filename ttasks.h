#ifndef TTASKS_H
#define TTASKS_H

#include <QObject>
#include <QSettings>
#include <QSqlDatabase>
#include "ttask.h"

class TTasks : public QObject
{
    Q_OBJECT
private:
  //  typedef QPair<TTask *, QThread *> TTasksAndThread;


    QVector<TTask *> Tasks; //текущие задачи

    QSqlDatabase DB;

    QSettings & Config;

    enum MSG_CODE {CODE_OK, CODE_ERROR, CODE_INFORMATION};

    void SendLogMsg(uint16_t Category, const QString &Msg);
    void AddTask(uint32_t TaskNumber);
    void ClearLog();
public:
    explicit TTasks(QSettings & Config, QObject *parent = nullptr);
    ~TTasks();

    void StartAll();
    void StopAll();

public slots:
    void TaskChangeStatus(const uint32_t TaskNumber, TTaskStatus Status, const QString & LastError);

};

#endif // TTASKS_H
