#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QSettings>
#include <QTimer>

void setAppAutoRun(bool isAutoRun) {
  QSettings reg(R"(HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run)", QSettings::NativeFormat);

  QString appName = QCoreApplication::applicationName();

  if (isAutoRun) {
    reg.setValue(appName, QDir::toNativeSeparators(QCoreApplication::applicationFilePath()));
  } else {
    reg.remove(appName);
  }
}

class Core : public QObject {
  Q_OBJECT

public:
  Core(QObject *parent = nullptr) : QObject(parent){};

public slots:
  void run() {
    //    setAppAutoRun(true);
    setAppAutoRun(false);
    emit finished();
  };

signals:
  void finished();
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
