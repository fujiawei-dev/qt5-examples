#include <QCoreApplication>
#include <QDebug>
#include <QProcess>

void keepProcessRunning() {
  auto *process = new QProcess;

  QString workingDir = QCoreApplication::applicationDirPath();
  process->setWorkingDirectory(workingDir + "/bin");

  QObject::connect(process, &QProcess::readyReadStandardOutput, [process]() {
    qDebug() << "process:" << process->readAllStandardOutput();
  });

  QObject::connect(process, &QProcess::readyReadStandardError, [process]() {
    qDebug() << "process:" << process->readAllStandardError();
  });

  QObject::connect(process, &QProcess::started, []() {
    qDebug() << "process: started";
  });

  QObject::connect(process, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
                   [process](int exitCode, QProcess::ExitStatus exitStatus) {
                     qWarning() << "process: terminated, try to restart";

                     process->start();
                     process->waitForReadyRead(8000);
                   });

  process->start(workingDir + "/bin/ping.exe");
  process->waitForReadyRead(8000);
}

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);

  keepProcessRunning();

  return QCoreApplication::exec();
}
