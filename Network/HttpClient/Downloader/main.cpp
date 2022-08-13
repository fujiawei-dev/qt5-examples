#include <QCoreApplication>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);

  QNetworkRequest request;
  request.setUrl(QUrl("http://localhost/version/latest?flag=1"));
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

  QNetworkAccessManager manager;
  QNetworkReply *reply = manager.get(request);

  QObject::connect(reply, &QNetworkReply::metaDataChanged, [=] {
    QList<QByteArray> headers = reply->rawHeaderList();
    for (const QByteArray& header : headers) {
      qDebug() << header << ":" << reply->rawHeader(header);
    }

    QByteArray disposition = reply->rawHeader("Content-Disposition");
    auto filename = disposition.split(';').last().split('=').last();
    qDebug() << "filename:" << filename;
  });

  QFile file("temp.d");
  if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly)) {
    qDebug() << "open file failed";
    return 1;
  }

  QObject::connect(reply, &QNetworkReply::readyRead, [&] {
    file.write( reply->readAll());
  });

  QObject::connect(reply, &QNetworkReply::finished, [&] {
    file.write(reply->readAll());

    QByteArray disposition = reply->rawHeader("Content-Disposition");
    auto filename = disposition.split(';').last().split('=').last();
    if (!filename.isEmpty()) {
      file.rename(filename);
    }

    file.close();
    reply->deleteLater();

    QCoreApplication::quit();
  });

  return QCoreApplication::exec();
}
