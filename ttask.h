#ifndef TTASK_H
#define TTASK_H

#include <QObject>
#include <QProcess>
#include <QSettings>

enum TTaskStatus {TASK_STOP,        //процесс остановлен (запуска еще небыло)
                  TASK_RUN,         //Процесс запущен и работает
                  TASK_FINISHED,    //процесс успешно завершен
                  TASK_ERROR,       //процесс завершился с ошибкой
                  TASK_STARTED,     //процесс запускается
                  TASK_NOANSWER};   //не получен правильный ответ в теченни заданного времени

struct TTaskConfig {
    QString ExecuteFileName;
    QStringList Params;
    bool TryReload;
    QString WorkingDirectory;
    QString Name;
    QString Discription;
    uint32_t TaskNumber;
};

class TTask : public QObject
{
    Q_OBJECT
private:
    QProcess *Process;
    TTaskStatus TaskStatus = TTaskStatus::TASK_STOP;
    QString LastError;
    TTaskConfig TaskConfig;

public:
    explicit TTask(QSettings & Config, uint32_t TaskNumber, QObject *parent = nullptr);
    ~TTask();

    void Start();   //Запустить процесс
    void Stop();    //остановить процесс

    TTaskConfig GetConfig();
    TTaskStatus GetTaskStatus() const; //возвращает текущий статус процесса
    QString GetLastError() const; //возвращает последнее сообщение об ошибке
    qint64 GetPID() const;

signals:
    void TaskKill(); //отправляется процессу для завершения
    void TaskTerminate();//отправляется процессу для убийства

    void TaskChangeStatus(const uint32_t TaskNumber, TTaskStatus Status, const QString & LastError);

public slots:
    void TaskStarted();  //ловит сообщение о запуске процесса
    void TaskFinished(int exitCode, QProcess::ExitStatus exitStatus = QProcess::ExitStatus::NormalExit); //ловит сообщение о завершении процесса
    void TaskErrorOccurred(QProcess::ProcessError error);//ловит сообщение об ошибке в процессе

};

#endif // TTASK_H
