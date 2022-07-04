#include "ttask.h"

TTask::TTask(QSettings & Config, uint32_t TaskNumber, QObject *parent)
    : QObject(parent)
    , Process(new QProcess(parent))
{
    TaskConfig.TaskNumber = TaskNumber;

    Config.beginGroup("TASK" + QString::number(TaskNumber));
    TaskConfig.ExecuteFileName = Config.value("ExecuteFileName", "").toString();
    TaskConfig.WorkingDirectory = Config.value("WorkingDirectory", "").toString();
    TaskConfig.TryReload = Config.value("TryReload", false).toBool();
    TaskConfig.Name = Config.value("Name", "Task" + QString::number(TaskNumber)).toString();
    TaskConfig.Discription = Config.value("Discription", "").toString();
    uint32_t ParamsCount = Config.value("ParamsCount", 0).toInt();
    for (uint32_t i = 0; i < ParamsCount; ++i) {
        TaskConfig.Params.append(Config.value("Param" + QString::number(i), 0).toString());
    }
    Config.endGroup();

    QObject::connect(this, SIGNAL(TaskKill()), Process, SLOT(kill()));
    QObject::connect(this, SIGNAL(TaskTerminate()), Process, SLOT(terminate()));
    QObject::connect(Process, SIGNAL(started()), this, SLOT(TaskStarted()));
    QObject::connect(Process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(TaskFinished(int, QProcess::ExitStatus)));
    QObject::connect(Process, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(TaskErrorOccurred(QProcess::ProcessError)));
}

TTask::~TTask()
{
    Stop();
    Process->deleteLater();
}

void TTask::Start()
{
    TaskStatus = TTaskStatus::TASK_STARTED;
    LastError = "Started task. File: " + TaskConfig.WorkingDirectory + "/" + TaskConfig.ExecuteFileName;
    emit TaskChangeStatus(TaskConfig.TaskNumber, TaskStatus, LastError);

    Process->setWorkingDirectory(TaskConfig.WorkingDirectory);
    Process->start(TaskConfig.WorkingDirectory + "/" + TaskConfig.ExecuteFileName, TaskConfig.Params);
}

void TTask::Stop()
{
    if (Process->state() != QProcess::NotRunning) {
        Process->terminate();
        if (!Process->waitForFinished(5000)) {
            Process->kill();
            Process->waitForFinished(5000);
        }
    }
}

TTaskConfig TTask::GetConfig()
{
    return TaskConfig;
}

TTaskStatus TTask::GetTaskStatus() const
{
    return TaskStatus;
}

QString TTask::GetLastError() const
{
    return LastError;
}

qint64 TTask::GetPID() const
{
    if (Process->state() == QProcess::Running) return Process->processId();
    else return 0;
}

void TTask::TaskStarted()
{
    TaskStatus = TTaskStatus::TASK_RUN;
    LastError = "OK";
    emit TaskChangeStatus(TaskConfig.TaskNumber, TaskStatus, LastError);
}

void TTask::TaskFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    TaskStatus = TTaskStatus::TASK_FINISHED;
    LastError = "Exit code: " + QString::number(exitCode) + " Exit status: " + (exitStatus == QProcess::ExitStatus::NormalExit ? "NORMAL" : "CRASH");
    emit TaskChangeStatus(TaskConfig.TaskNumber, TaskStatus, LastError);
}

void TTask::TaskErrorOccurred(QProcess::ProcessError error)
{
    TaskStatus = TTaskStatus::TASK_ERROR;
    switch (error) {
    case QProcess::ProcessError::FailedToStart : {LastError = "Error: Fail to start"; break;}
    case QProcess::ProcessError::Crashed :       {LastError = "Error: Crashed"; break;}
    case QProcess::ProcessError::Timedout :      {LastError = "Error: Timedout"; break;}
    case QProcess::ProcessError::ReadError :     {LastError = "Error: ReadError"; break;}
    case QProcess::ProcessError::WriteError :    {LastError = "Error: Error: WriteError"; break;}
    case QProcess::ProcessError::UnknownError :  {LastError = "Error: UnknownError"; break;}
    }
    LastError +=  " Output: " + QString(Process->readAll());
    emit TaskChangeStatus(TaskConfig.TaskNumber, TaskStatus, LastError);
}
