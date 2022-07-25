#include <QCoreApplication>
#include <QUdpSocket>

QByteArray receiveUDPData() {
  auto *socket = new QUdpSocket;
  socket->writeDatagram("get", QHostAddress::LocalHost, 6688);
  socket->waitForReadyRead(3000);

  QByteArray datagram;
  QHostAddress sender;
  quint16 senderPort;

  if (socket->hasPendingDatagrams()) {
    datagram.resize(socket->pendingDatagramSize());
    socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
    qDebug() << "received:" << datagram;
  }

  socket->deleteLater();

  return datagram;
}

int main(int argc, char *argv[]) {
  for (int i = 0; i < 10; ++i) {
    receiveUDPData();
  }
}
