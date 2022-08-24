#include <QCoreApplication>
#include <QDebug>
#include <QDateTime>
#include <QTimer>

class Core : public QObject {
  Q_OBJECT

public:
  Core(QObject *parent = nullptr) : QObject(parent) {
    auto current = QDateTime::currentDateTime();
    auto runAt = QTime::fromString("23:59", "hh:mm");

    if (current.time() > runAt) {
      current = current.addDays(1);
    }

    current.setTime(runAt);

    auto diff = QDateTime::currentDateTime().msecsTo(current);

    QTimer::singleShot(diff, [this] {
      auto runTimer = new QTimer(QCoreApplication::instance());
      connect(runTimer, &QTimer::timeout, [this] {
        emit trigger();
      });
      runTimer->start(24 * 3600 * 1000);
    });
  };

public slots:
  void run() {
    emit finished();
  };

signals:
  void finished();
  void trigger();
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
