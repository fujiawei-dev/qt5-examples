#include <QCoreApplication>
#include <QDir>
#include <QProcess>
#include <QThread>
#include <QTimer>
#include <QtConcurrent>

void runProcessForever(const bool *stop) {
  QProcess process;

  QString workingDir = QCoreApplication::applicationDirPath();

  qDebug() << workingDir;

  process.setWorkingDirectory(workingDir + QDir::separator() + "bin");

  QObject::connect(&process, &QProcess::readyReadStandardOutput, [&]() {
    qDebug() << process.readAllStandardOutput();
  });

  QObject::connect(&process, &QProcess::readyReadStandardError, [&]() {
    qDebug() << process.readAllStandardError();
  });

  QObject::connect(&process, &QProcess::started, [&]() {
    qDebug() << "process started";
  });

  QObject::connect(&process, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
                   [&](int exitCode, QProcess::ExitStatus exitStatus) {
                     if (!*stop) {
                       qWarning() << "process exited, try to restart";
                       process.start();
                       qDebug() << process.program() + " " + process.arguments().join(" ");
                       process.waitForFinished(-1);
                     } else {
                       qDebug() << "process finished";
                     }
                   });

  process.start("C:/Developer/anaconda3/python.exe", QStringList() << "main.py");
  qDebug() << process.program() + " " + process.arguments().join(" ");
  process.waitForFinished(-1);
}

class Core : public QObject {
  Q_OBJECT

public:
  Core(QObject *parent = nullptr) : QObject(parent){};

public slots:
  void run() {
    //    runProcessForever();
    bool stop = false;
    auto future = QtConcurrent::run(runProcessForever, &stop);
    qDebug() << "run process forever";
    QThread::sleep(3);
    qDebug() << "slept 3 seconds";
    stop = true;
    emit finished();
  };

signals:
  void finished();

private:
  QProcess *process;
};

#include "main.moc"

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);

  Core *core = new Core(&app);
  // Only for console app. This will run from the application event loop.
  QObject::connect(core, SIGNAL(finished()), &app, SLOT(quit()));
  QTimer::singleShot(0, core, SLOT(run()));

  return QCoreApplication::exec();
}
