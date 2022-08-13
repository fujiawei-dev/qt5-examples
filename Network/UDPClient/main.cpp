#include <QCoreApplication>
#include <QTimer>
#include <QUdpSocket>

class Core : public QObject {
  Q_OBJECT

public:
  Core(QObject *parent = nullptr) : QObject(parent){};

public slots:
  void run() {
    auto *socket = new QUdpSocket;
    socket->writeDatagram("get", QHostAddress::LocalHost, 7890);
    socket->waitForReadyRead(3000);

    QByteArray datagram;
    QHostAddress sender;
    quint16 senderPort;

    if (socket->hasPendingDatagrams()) {
      datagram.resize((int)socket->pendingDatagramSize());
      socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
      qDebug() << "received:" << datagram;
    }

    socket->deleteLater();

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
